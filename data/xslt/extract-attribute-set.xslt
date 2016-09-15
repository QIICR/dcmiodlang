<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:doc="http://docbook.org/ns/docbook"
  xmlns:xl="http://www.w3.org/1999/xlink"
  xmlns:func="http://exslt.org/functions"
  xmlns:ext="http://exslt.org/common"
  exclude-result-prefixes="ext"
  extension-element-prefixes="func"
>

<xsl:param name="section"/>
<xsl:param name="table"/>
<xsl:param name="keyword"/>
<xsl:output method="text"/>

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

<func:function name="func:printAttributes">
  <xsl:param name="nestingLevel"/>
  <xsl:param name="row"/>
  <xsl:param name="tagColumn"/>
  <xsl:param name="nameColumn"/>
  <xsl:param name="typeColumn"/>
  <xsl:param name="descriptionColumn"/>
  <func:result>
  <xsl:if test="$row">
  <xsl:variable name="nextRowNesting" select="$row[1]/following-sibling::doc:tr[1]//*[contains(text(),'>')]"/>
  <xsl:variable name="nextNestingLevel" select="string-length($nextRowNesting)-string-length(translate($nextRowNesting,'>',''))"/>
  <xsl:text>&#xA;</xsl:text>
  <xsl:value-of select="func:indent($nestingLevel)"/>
  <xsl:choose>
    <xsl:when test="$row[1]/doc:td[@colspan&gt;1]">
      <xsl:text>include( </xsl:text>
      <xsl:variable name="linkend" select="$row[1]//doc:xref/@linkend"/>
      <xsl:variable name="include" select="$row[1]//*[contains(translate(text(),'INCLUDE','include'),'include')]"/>
      <xsl:variable name="emphasis" select="$row[1]/doc:td[@colspan&gt;1]//doc:emphasis"/>
      <xsl:choose>
        <xsl:when test="contains($linkend, 'table_')">
          <xsl:value-of select="document('iod-dict.xml')//*[@table=substring-after($linkend, 'table_')]/@keyword"/>
        </xsl:when>
        <xsl:when test="contains($linkend, 'sect_')">
          <xsl:value-of select="document('iod-dict.xml')//*[@section=substring-after($linkend, 'sect_')]/@keyword"/>
        </xsl:when>
        <xsl:when test="$include">
          <xsl:value-of select="substring-before(concat(translate(substring-before(substring-after($include,'&quot;'),'&quot;'),' ',''), 'Macro'), 'Macro')"/>
        </xsl:when>
        <xsl:when test="$emphasis">
          <xsl:text>FIXME: </xsl:text>
          <xsl:value-of select="$emphasis"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:message terminate="yes">
            <xsl:text>Unable to understand what is presumed to be an include statement!</xsl:text>
          </xsl:message>
        </xsl:otherwise>
      </xsl:choose>
      <xsl:text> )</xsl:text>
    </xsl:when>
    <xsl:otherwise>
      <xsl:for-each select="$row[1]/doc:td[$descriptionColumn]/doc:para">
        <xsl:value-of select="func:printAsComment($nestingLevel,text())"/>
      </xsl:for-each>
      <xsl:value-of select="$row[1]/doc:td[$tagColumn]/doc:para"/>
      <xsl:text> </xsl:text>
      <xsl:value-of select="translate($row[1]/doc:td[$nameColumn]/doc:para,' >','')"/>
      <xsl:text> type</xsl:text>
      <xsl:value-of select="$row[1]/doc:td[$typeColumn]/doc:para"/>
      <xsl:if test="$nextNestingLevel>$nestingLevel">
        <xsl:text>&#xA;</xsl:text>
        <xsl:value-of select="func:indent($nestingLevel)"/>
        <xsl:text>{</xsl:text>
        <xsl:value-of select="func:printAttributes($nextNestingLevel,$row[1]/following-sibling::doc:tr[1],$tagColumn,$nameColumn,$typeColumn,$descriptionColumn)"/>
        <xsl:value-of select="func:indent($nestingLevel)"/>
        <xsl:text>}</xsl:text>
      </xsl:if>
    </xsl:otherwise>
  </xsl:choose>
  <xsl:text>;&#xA;</xsl:text>
  <xsl:if test="$nextNestingLevel>=$nestingLevel">
    <xsl:value-of select="func:printAttributes($nestingLevel,$row[1]/following-sibling::doc:tr[(not($nestingLevel) and not(.//*[contains(text(),'>')])) or ($nestingLevel>0 and .//*[string-length(text())-string-length(translate(text(),'>',''))=$nestingLevel])][1],$tagColumn,$nameColumn,$typeColumn,$descriptionColumn)"/>
  </xsl:if>
  </xsl:if>
  </func:result>
</func:function>

<xsl:template match="/">
  <xsl:variable name="set">
    <xsl:choose>
      <xsl:when test="$section">
        <xsl:element name="section">sect_<xsl:value-of select="$section"/></xsl:element>
        <xsl:choose>
          <xsl:when test="$table">
            <xsl:element name="table">table_<xsl:value-of select="$table"/></xsl:element>
            <xsl:element name="keyword"><xsl:value-of select="document('iod-dict.xml')//*[@table=$table]/@keyword"/></xsl:element>
            <xsl:element name="name"><xsl:value-of select="document('iod-dict.xml')//*[@table=$table]/@name"/></xsl:element>
            <xsl:element name="flavor"><xsl:value-of select="name(document('iod-dict.xml')//*[@table=$table])"/></xsl:element>
          </xsl:when>
          <xsl:otherwise>
            <xsl:element name="table"><xsl:value-of select="//doc:section[@xml:id=concat('sect_',$section)]/doc:table/@xml:id"/></xsl:element>
            <xsl:element name="keyword"><xsl:value-of select="document('iod-dict.xml')//*[@section=$section]/@keyword"/></xsl:element>
            <xsl:element name="name"><xsl:value-of select="document('iod-dict.xml')//*[@section=$section]/@name"/></xsl:element>
            <xsl:element name="flavor"><xsl:value-of select="name(document('iod-dict.xml')//*[@section=$section])"/></xsl:element>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>
      <xsl:when test="$keyword">
        <xsl:element name="section">sect_<xsl:value-of select="document('iod-dict.xml')//*[@keyword=$keyword]/@section"/></xsl:element>
        <xsl:element name="keyword"><xsl:value-of select="$keyword"/></xsl:element>
        <xsl:element name="name"><xsl:value-of select="document('iod-dict.xml')//*[@keyword=$keyword]/@name"/></xsl:element>
        <xsl:element name="flavor"><xsl:value-of select="name(document('iod-dict.xml')//*[@keyword=$keyword])"/></xsl:element>
        <xsl:variable name="testTable" select="document('iod-dict.xml')//*[@keyword=$keyword]/@table"/>
        <xsl:choose>
          <xsl:when test="$testTable">
            <xsl:element name="table">table_<xsl:value-of select="$testTable"/></xsl:element>
          </xsl:when>
          <xsl:otherwise>
            <xsl:element name="table"><xsl:value-of select="//doc:section[@xml:id=concat('sect_', document('iod-dict.xml')//*[@keyword=$keyword]/@section)]/doc:table/@xml:id"/></xsl:element>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>
      <xsl:otherwise>
        <xsl:message terminate="yes">
          <xsl:text>Either &lt;section> or &lt;keyword> argument must be given!</xsl:text>
        </xsl:message>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:variable>
  <xsl:variable name="head" select="//doc:table[@xml:id=ext:node-set($set)/table]/doc:thead/doc:tr"/>
  <xsl:variable name="nameColumn" select="count($head/doc:th[.//*[text()='Name']]/preceding-sibling::doc:th)+1"/>
  <xsl:variable name="tagColumn" select="count($head/doc:th[.//*[text()='Tag']]/preceding-sibling::doc:th)+1"/>
  <xsl:variable name="typeColumn" select="count($head/doc:th[.//*[text()='Type']]/preceding-sibling::doc:th)+1"/>
  <xsl:variable name="descriptionColumn" select="count($head/doc:th[.//*[text()='Attribute Description']]/preceding-sibling::doc:th)+1"/>
  <xsl:choose>
    <xsl:when test="ext:node-set($set)/flavor='functional-group'">functional_group</xsl:when>
    <xsl:otherwise><xsl:value-of select="ext:node-set($set)/flavor"/></xsl:otherwise>
  </xsl:choose>
  <xsl:text> </xsl:text>
  <xsl:value-of select="ext:node-set($set)/keyword"/>
  <xsl:text> &quot;</xsl:text>
  <xsl:value-of select="ext:node-set($set)/name"/>
  <xsl:text>&quot;&#xA;{</xsl:text>
  <xsl:value-of select="func:printAttributes(0,//doc:table[@xml:id=ext:node-set($set)/table]/doc:tbody/doc:tr[1],$tagColumn,$nameColumn,$typeColumn,$descriptionColumn)"/>
  <xsl:text>};</xsl:text>
</xsl:template>

</xsl:stylesheet>
