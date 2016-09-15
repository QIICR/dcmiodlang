#pragma once
#include <boost/spirit/include/qi.hpp>
#include "iodl/ast/iodl.hpp"
#include "iodl/parser/grammar/template_declaration.hpp"
#include "iodl/parser/grammar/attribute_set.hpp"
#include "iodl/parser/grammar/iod.hpp"
#include "iodl/parser/grammar/files.hpp"
#include "iodl/parser/grammar/property.hpp"

namespace iodl { namespace parser { namespace grammar {
    template<typename Iterator,typename Skipper>
    struct IODL : boost::spirit::qi::grammar
    <
        Iterator,
        ast::IODL(Status&),
        boost::spirit::qi::locals<std::vector<std::string> >,
        Skipper
    >
    {
        struct TemplateCreationVisitor : boost::static_visitor<void>
        {
            TemplateCreationVisitor( std::vector<std::string>& parameters )
            : parameters( parameters )
            {

            }

            void operator()( ast::AttributeSet& as ) const
            {
                std::swap( as.templateParameters, parameters );
            }

            void operator()( ast::IOD& iod ) const
            {
                std::swap( iod.templateParameters, parameters );
            }

            template<typename T>
            void operator()( T& ) const
            {
                // TODO: handle error
            }

            std::vector<std::string>& parameters;
        };

        static void make_template( ast::Definition& result,
                                   std::vector<std::string>& parameters,
                                   const ast::Definition& d )
        {
            result = d;
            boost::apply_visitor( TemplateCreationVisitor( parameters ), result );
        }

        IODL()
        : IODL::base_type( start )
        , iod()
        , template_declaration( iod.keyword )
        {
            using boost::phoenix::bind;
            using namespace boost::spirit::qi::labels;

            start = *( ( template_definition(_r1) | definition(_r1) ) >> boost::spirit::qi::lit( ';' ) );

            template_definition =  template_declaration[_a=_1]
                                >> templatable_definition(_r1)[bind(&make_template,_val,_a,_1)]
                                ;

            definition = templatable_definition(_r1) | files | property;

            templatable_definition = iod | attributeSet(_r1);
        }

        boost::spirit::qi::rule<Iterator,ast::IODL(Status&),boost::spirit::qi::locals<std::vector<std::string> >,Skipper> start;
        boost::spirit::qi::rule<Iterator,ast::Definition(Status&),boost::spirit::qi::locals<std::vector<std::string> >,Skipper> template_definition;
        boost::spirit::qi::rule<Iterator,ast::Definition(Status&),Skipper> definition, templatable_definition;
        IOD<Iterator,Skipper> iod;
        TemplateDeclaration<Iterator,Skipper> template_declaration;
        AttributeSet<Iterator,Skipper> attributeSet;
        Files<Iterator,Skipper> files;
        Property<Iterator,Skipper> property;
    };
}}}
