#include <iostream>
#include <fstream>
#include <sstream>
#include <boost/spirit/include/qi.hpp>
#include <boost/filesystem.hpp>
#include <boost/make_shared.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/program_options.hpp>
#include "iodl/parser/skeleton.hpp"
#include "iodl/parser/iodl.hpp"
#include "iodl/generator/context.hpp"
#include "iodl/generator/attribute_set.hpp"
#include "iodl/generator/string.hpp"
#include "iodl/generator/value_representation.hpp"
#include "iodl/generator/container.hpp"
#include "iodl/generator/iod.hpp"
#include "iodl/skeleton/text.hpp"
#include "dcmtk/dcmdata/dcdeftag.h"

struct CommandLineOptions
{
    boost::filesystem::path              skeletonDirectory;
    std::string                          copyright;
    std::string                          author;
    std::string                          implementationModule;
    std::string                          fallbackHeaderIfdefGuard;
    std::vector<boost::filesystem::path> inputFiles;
    bool                                 help;
};

class CommandLineOptionsParser : public boost::program_options::options_description
{
public:
    CommandLineOptionsParser( CommandLineOptions& options )
    : boost::program_options::options_description( "Allowed options" )
    , m_Data( options )
    {
        add_options()
            ( "help,h", "Print this text." )
            (
                "skeleton-directory,s",
                boost::program_options::value<boost::filesystem::path>( &m_Data.skeletonDirectory )->value_name( "<path>" ),
                "Path to the directory containing the skeleton files that should be used."
            )
            (
                "copyright,c",
                boost::program_options::value<std::string>( &m_Data.copyright )->value_name( "<date-range>" ),
                "The copyright date (range), e.g. 2015-2016 or just 2016."
            )
            (
                "author,a",
                boost::program_options::value<std::string>( &m_Data.author )->value_name( "<name>" ),
                "The author of the generated file."
            )
            (
                "implementation-module,m",
                boost::program_options::value<std::string>( &m_Data.implementationModule )->value_name( "<name>" ),
                "The DCMTK module string, e.g. dcmiod."
            )
            (
                "fallback-header-ifdef-guard",
                boost::program_options::value<std::string>( &m_Data.fallbackHeaderIfdefGuard )->value_name( "<string>" ),
                "The header ifdef guard string to use when no such string can be guessed (for example because the generated code is written to standard output)."
            )
        ;
    }

    bool operator()( int argc, char** argv )
    {
        boost::program_options::options_description hidden( "hidden" );
        hidden.add_options()
        (
            "input-files",
            boost::program_options::value<std::vector<boost::filesystem::path> >( &m_Data.inputFiles ),
            "input-files"
        );

        boost::program_options::options_description options( "options" );
        options.add( *this ).add( hidden );

        boost::program_options::positional_options_description positional;
        positional.add( "input-files", -1 );

        boost::program_options::variables_map vm;
        try
        {
            boost::program_options::store
            (
                boost::program_options::command_line_parser( argc, argv )
                    .options( options )
                    .positional( positional )
                    .run(),
                vm
            );
            boost::program_options::notify( vm );
        }
        catch( boost::program_options::error& e )
        {
            std::cerr << e.what() << std::endl;
            return false;
        }
        catch( ... )
        {
            return false;
        }

        m_Data.help = vm.count( "help" ) != 0;

        return !m_Data.inputFiles.empty();
    }

private:
    CommandLineOptions& m_Data;
};

class SkeletonBasedGenerator
: public iodl::generator::OutputStreamContext
, public boost::static_visitor<iodl::Status>
{
public:
    SkeletonBasedGenerator( const CommandLineOptions& options )
    : iodl::generator::OutputStreamContext( std::cout )
    , m_Options( options )
    {

    }

    iodl::Status operator()( const iodl::ast::AttributeSet& attr )
    {
        Includes includes;
        for
        (
            iodl::ast::AttributeSet::const_iterator it = attr.begin();
            it != attr.end();
            ++it
        )
        for
        (
            std::vector<iodl::ast::Attribute::Argument>::const_iterator jt = it->arguments.begin();
            jt != it->arguments.end();
            ++jt
        )
        if( const iodl::ast::Attribute::MacroReference* pMacro = boost::get<iodl::ast::Attribute::MacroReference>( &*jt ) )
        {
            includes( m_FileMap, *pMacro );
        }
        iodl::generator::StackedComponentMapContext context( *this );
        context.addComponent( entityTypeToString( flavorToEntityType( attr.flavor ) ), boost::make_shared<iodl::generator::AttributeSet>( attr ) );
        context.addComponent( "includes", iodl::generator::make_container<iodl::generator::StringReference>( includes ) );
        return generate( flavorToEntityType( attr.flavor ), context, attr.keyword );
    }

    iodl::Status operator()( const iodl::ast::IOD& iod )
    {
        Includes includes;
        includes( m_FileMap, iod.base );
        for
        (
            std::vector<iodl::ast::IOD::ModuleReference>::const_iterator it = iod.modules.begin();
            it != iod.modules.end();
            ++it
        )
        {
            includes( m_FileMap, *it );
        }
        for
        (
            std::vector<iodl::ast::IOD::FunctionalGroupReference>::const_iterator it = iod.functionalGroups.begin();
            it != iod.functionalGroups.end();
            ++it
        )
        {
            includes( m_FileMap, *it );
        }
        iodl::generator::StackedComponentMapContext context( *this );
        context.addComponent( "iod", boost::make_shared<iodl::generator::IOD>( iod ) );
        context.addComponent( "includes", iodl::generator::make_container<iodl::generator::StringReference>( includes ) );
        return generate( IOD, context, iod.keyword );
    }

    iodl::Status operator()( const iodl::ast::Files& files )
    {
        FileMapIterator it = m_FileMap.insert( std::make_pair( files.keyword, std::map<std::string,std::string>() ) ).first;
        it->second.insert( files.begin(), files.end() );
        return iodl::Status();
    }

    iodl::Status operator()( const std::pair<std::string,std::string>& property )
    {
        addComponent( property.first, boost::make_shared<iodl::generator::StringReference>( property.second ) );
        return iodl::Status();
    }

private:
    typedef std::map<std::string,std::map<std::string,std::string> >::iterator FileMapIterator;

    enum EntityType
    {
        VALUE_SET_CONSTRAINT,
        MACRO,
        MODULE,
        FUNCTIONAL_GROUP,
        IOD,
        INVALID_ENTITY_TYPE
    };

    struct Includes : std::set<std::string>
    {
        void operator()( const std::map<std::string,std::map<std::string,std::string> >& fileMap,
                         const std::string& keyword )
        {
            std::map<std::string,std::map<std::string,std::string> >::const_iterator files = fileMap.find( keyword );
            if( files != fileMap.end() )
            {
                std::map<std::string,std::string>::const_iterator include = files->second.find( "include" );
                if( include != files->second.end() )
                    insert( include->second );
            }
        }
    };

    static EntityType flavorToEntityType( iodl::ast::AttributeSet::Flavor flavor )
    {
        switch( flavor )
        {
        case iodl::ast::AttributeSet::MACRO:
            return MACRO;

        case iodl::ast::AttributeSet::MODULE:
            return MODULE;

        case iodl::ast::AttributeSet::FUNCTIONAL_GROUP:
            return FUNCTIONAL_GROUP;

        default:
            return INVALID_ENTITY_TYPE;
        }
    }

    static std::string entityTypeToString( EntityType type )
    {
        switch( type )
        {
        case VALUE_SET_CONSTRAINT:
            return std::string(); // TODO

        case MACRO:
            return "macro";

        case MODULE:
            return "module";

        case FUNCTIONAL_GROUP:
            return "functional_group";

        case IOD:
            return "iod";

        default:
            return std::string();
        }
    }

    std::pair<iodl::Status,iodl::generator::Component> getSkeleton( EntityType type, const std::string& outputFileKeyword )
    {
        std::pair<std::map<std::pair<EntityType,std::string>,iodl::generator::Component>::iterator,bool> result = m_Skeletons.insert
        (
            std::make_pair( std::make_pair( type, outputFileKeyword ), iodl::generator::Component() )
        );
        if( result.second )
        {
            boost::filesystem::path filename = m_Options.skeletonDirectory / ( entityTypeToString( type ) + '_' + outputFileKeyword + ".iodskel" );
            iodl::parser::Skeleton parseSkeleton( filename.string() );
            if( !parseSkeleton )
            {
                m_Skeletons.erase( result.first );
                return std::make_pair( iodl::Status( "Failed to open skeleton file \"" + filename.string() + '"' ), iodl::generator::Component() );
            }
            iodl::Status status = parseSkeleton( result.first->second );
            if( !status )
            {
                m_Skeletons.erase( result.first );
                return std::make_pair( status, iodl::generator::Component() );
            }
        }
        return std::make_pair( iodl::Status(), result.first->second );
    }

    bool setHeaderIfdefGuard( iodl::generator::Context& context, FileMapIterator outputFiles )
    {
        using boost::spirit::qi::alpha;
        using boost::spirit::qi::lit;
        using boost::spirit::qi::char_;
        using boost::spirit::qi::attr;
        using boost::spirit::qi::omit;

        if( outputFiles != m_FileMap.end() )
        {
            std::map<std::string,std::string>::const_iterator header = outputFiles->second.find( "header" );
            if( header != outputFiles->second.end() )
            {
                const std::string filename = boost::filesystem::path( header->second ).filename().string();
                std::string::const_iterator it = filename.begin();
                std::string guard;
                if( boost::spirit::qi::parse( it, filename.end(), +( alpha | char_( '_' ) | ( lit( '.' ) >> attr( '_' ) ) | omit[char_] ), guard ) && it == filename.end() && !guard.empty() )
                {
                    boost::to_upper( guard );
                    context.addComponent( "header_ifdef_guard", boost::make_shared<iodl::skeleton::Text>( iodl::Status::FilePosition( boost::spirit::classic::file_position() ), guard ) );
                }
            }
        }
        return false;
    }

    iodl::Status generate( EntityType type, iodl::generator::Context& context, const std::string& keyword )
    {
        FileMapIterator outputFiles = m_FileMap.find( keyword );
        if( outputFiles != m_FileMap.end() )
        {
            if( !setHeaderIfdefGuard( context, outputFiles ) )
                context.addComponent( "header_ifdef_guard", boost::make_shared<iodl::generator::StringReference>( m_Options.fallbackHeaderIfdefGuard ) );
            std::map<std::string,std::string>::const_iterator include = outputFiles->second.find( "include" );
            if( include != outputFiles->second.end() )
                context.addComponent( "my_include", boost::make_shared<iodl::generator::StringReference>( include->second ) );
            for
            (
                std::map<std::string,std::string>::const_iterator output = outputFiles->second.begin();
                output != outputFiles->second.end();
                ++output
            )
            if( output != include )
            {
                std::pair<iodl::Status,iodl::generator::Component> skeleton = getSkeleton( type, output->first );
                if( !skeleton.first )
                    return skeleton.first;
                boost::filesystem::create_directories( boost::filesystem::path( output->second ).parent_path() );
                std::ofstream outputFile( output->second.c_str() );
                if( !outputFile )
                    return iodl::Status( "Failed to open output file: \"" + output->second + '"' );
                std::cout << "Generating \"" << output->second << "\"...";
                iodl::generator::StackedOutputStreamContext outputFileContext( context, outputFile );
                iodl::Status status = skeleton.second.generate( outputFileContext );
                if( !status )
                {
                    std::cout << " failed" << std::endl;
                    return status;
                }
                std::cout << " ok" << std::endl;
            }
        }
        return iodl::Status();
    }

    const CommandLineOptions& m_Options;
    std::map<std::string,std::map<std::string,std::string> > m_FileMap;
    std::map<std::pair<EntityType,std::string>,iodl::generator::Component> m_Skeletons;
};

int main( int argc, char** argv )
{
    CommandLineOptions options;
    {
        CommandLineOptionsParser parseCommandLine( options );

        if( !parseCommandLine( argc, argv ) )
        {
            std::cerr << "Invalid arguments, usage: iodc [options] <input-files>" << std::endl
                      << parseCommandLine << std::endl;
            return 1;
        }

        if( options.help )
        {
            std::cout << "Usage: iodc [options] <input-files>" << std::endl
                      << parseCommandLine << std::endl;
            return 0;
        }
    }

    iodl::ast::IODL ast;

    for
    (
        std::vector<boost::filesystem::path>::const_iterator it = options.inputFiles.begin();
        it != options.inputFiles.end();
        ++it
    )
    {
        iodl::parser::IODL parse( it->string() );
        if( !parse )
        {
            std::cerr << "Error: Failed to open input file " << *it << std::endl;
            return 1;
        }

        iodl::Status status = parse( ast );
        if( !status )
        {
            std::cerr << status << std::endl;
            return 1;
        }
    }

    SkeletonBasedGenerator generate( options );

    generate.addComponent( "VR", iodl::generator::ValueRepresentation::VR.base() );
    if( !options.copyright.empty() )
        generate.addComponent( "copyright", boost::make_shared<iodl::generator::StringReference>( options.copyright ) );
    if( !options.author.empty() )
        generate.addComponent( "author", boost::make_shared<iodl::generator::StringReference>( options.author ) );
    if( !options.implementationModule.empty() )
        generate.addComponent( "implementation_module", boost::make_shared<iodl::generator::StringReference>( options.implementationModule ) );

    for
    (
        iodl::ast::IODL::const_iterator it = ast.begin();
        it != ast.end();
        ++it
    )
    {
        iodl::Status status = boost::apply_visitor( generate, *it );
        if( !status )
        {
            std::cerr << status << std::endl;
            return 1;
        }
    }

    return 0;
}
