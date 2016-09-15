<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:doc="http://docbook.org/ns/docbook"
  xmlns:xl="http://www.w3.org/1999/xlink"
  xmlns:func="http://exslt.org/functions"
  extension-element-prefixes="func"
>

<xsl:param name="section"/>
<xsl:param name="keyword"/>
<xsl:output method="text"/>

<func:function name="func:make-keyword">
  <xsl:param name="text"/>
  <func:result>
    <xsl:value-of select="translate($text, ' /-:*,\', '')"/>
  </func:result>
</func:function>

<func:function name="func:indent">
  <xsl:param name="nestingLevel"/>
  <func:result>
    <xsl:for-each select="(//node())[$nestingLevel+1&gt;=position()]">
      <xsl:text>  </xsl:text>
    </xsl:for-each>
  </func:result>
</func:function>

<func:function name="func:printAsComment">
  <xsl:param name="nestingLevel"/>
  <xsl:param name="text"/>
  <func:result>
  <xsl:if test="$text and string-length(normalize-space($text))">
    <xsl:text>// </xsl:text>
    <xsl:value-of select="normalize-space(substring-before(concat($text,'&#xA;'),'&#xA;'))"/>
    <xsl:text>&#xA;</xsl:text>
    <xsl:value-of select="func:indent($nestingLevel)"/>
    <xsl:value-of select="func:printAsComment($nestingLevel,substring-after($text,'&#xA;'))"/>
  </xsl:if>
  </func:result>
</func:function>

<xsl:template match="/">
  <xsl:variable name="iodSection">
    <xsl:choose>
      <xsl:when test="$section">
        <xsl:value-of select="$section"/>
      </xsl:when>
      <xsl:when test="$keyword">
        <xsl:value-of select="document('iod-dict.xml')/dicom-iods/iods/iod[@keyword=$keyword]/@section"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:message terminate="yes">
          <xsl:text>Either &lt;section> or &lt;keyword> argument must be given!</xsl:text>
        </xsl:message>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:variable>
  <xsl:variable name="iodKeyword" select="document('iod-dict.xml')/dicom-iods/iods/iod[@section=$iodSection]/@keyword"/>
  <xsl:variable name="iodName" select="document('iod-dict.xml')/dicom-iods/iods/iod[@section=$iodSection]/@name"/>
  <xsl:text>iod </xsl:text>
  <xsl:value-of select="$iodKeyword"/>
  <xsl:text> "</xsl:text>
  <xsl:value-of select="$iodName"/>
  <xsl:text>"&#xA;{&#xA;</xsl:text>
  <xsl:for-each select="//doc:section[@xml:id=concat('sect_', $iodSection) and @status=2]/doc:section[@status=3 and ./doc:table]">
    <xsl:choose>
      <xsl:when test="substring-before(doc:title, 'Module')">
        <xsl:text>modules:&#xA;</xsl:text>
        <xsl:variable name="head" select="doc:table/doc:thead/doc:tr"/>
        <xsl:variable name="ieColumn" select="count($head/doc:th[.//*[text()='IE']]/preceding-sibling::doc:th)+1"/>
        <xsl:variable name="moduleColumn" select="count($head/doc:th[.//*[text()='Module']]/preceding-sibling::doc:th)+1"/>
        <xsl:variable name="referenceColumn" select="count($head/doc:th[.//*[text()='Reference']]/preceding-sibling::doc:th)+1"/>
        <xsl:variable name="usageColumn" select="count($head/doc:th[.//*[text()='Usage']]/preceding-sibling::doc:th)+1"/>
        <xsl:for-each select="doc:table/doc:tbody/doc:tr">
          <xsl:variable name="modSection" select="substring-after(doc:td[$referenceColumn + count(../doc:td) - 4]/doc:para/doc:xref/@linkend, 'sect_')"/>
          <xsl:text>    </xsl:text>
          <xsl:value-of select="document('iod-dict.xml')/dicom-iods/modules/module[@section=$modSection]/@keyword"/>
          <xsl:text> </xsl:text>
          <xsl:value-of select="substring(doc:td[$usageColumn + count(../doc:td) - 4]/doc:para, 1, 1)"/>
          <xsl:text>;&#xA;</xsl:text>
        </xsl:for-each>
        <xsl:text>&#xA;</xsl:text>
      </xsl:when>
      <xsl:when test="substring-before(doc:title, 'Functional Group')">
        <xsl:text>functional_groups:&#xA;</xsl:text>
        <xsl:variable name="head" select="doc:table/doc:thead/doc:tr"/>
        <xsl:variable name="macroColumn" select="count($head/doc:th[.//*[text()='Functional Group Macro']]/preceding-sibling::doc:th)+1"/>
        <xsl:variable name="sectionColumn" select="count($head/doc:th[.//*[text()='Section']]/preceding-sibling::doc:th)+1"/>
        <xsl:variable name="usageColumn" select="count($head/doc:th[.//*[text()='Usage']]/preceding-sibling::doc:th)+1"/>
        <xsl:for-each select="doc:table/doc:tbody/doc:tr">
          <xsl:variable name="fgSection" select="substring-after(doc:td[$sectionColumn]/doc:para/doc:xref/@linkend, 'sect_')"/>
          <xsl:text>    </xsl:text>
          <xsl:value-of select="document('iod-dict.xml')/dicom-iods/functional-groups/functional-group[@section=$fgSection]/@keyword"/>
          <xsl:text> </xsl:text>
          <xsl:value-of select="substring(normalize-space(doc:td[$usageColumn]), 1, 1)"/>
          <xsl:text>;&#xA;</xsl:text>
        </xsl:for-each>
      </xsl:when>
    </xsl:choose>
  </xsl:for-each>
  <xsl:text>};</xsl:text>
</xsl:template>

</xsl:stylesheet>
