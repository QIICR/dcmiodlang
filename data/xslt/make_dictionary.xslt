<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:doc="http://docbook.org/ns/docbook"
  xmlns:xl="http://www.w3.org/1999/xlink"
  xmlns:func="http://exslt.org/functions"
  extension-element-prefixes="func"
>

<xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes"/>

<xsl:template match="/">
  <xsl:element name="dicom-iods">
    <xsl:element name="iods">
      <xsl:for-each select="//doc:section">
        <xsl:variable name="name">
          <xsl:choose>
            <xsl:when test="contains(doc:title, 'IOD')"><xsl:value-of select="normalize-space(substring-before(doc:title, 'IOD'))"/></xsl:when>
            <xsl:otherwise><xsl:value-of select="normalize-space(substring-before(doc:title, 'Information Object'))"/></xsl:otherwise>
          </xsl:choose>
        </xsl:variable>
        <xsl:if test="$name != '' and doc:section/doc:table/doc:thead/doc:tr/doc:th[.//text()='Module']">
          <xsl:element name="iod">
            <xsl:attribute name="section"><xsl:value-of select="substring-after(@xml:id, 'sect_')"/></xsl:attribute>
            <xsl:attribute name="keyword"><xsl:value-of select="translate($name, ' ()/-:*,\', '')"/></xsl:attribute>
            <xsl:attribute name="name"><xsl:value-of select="$name"/></xsl:attribute>
          </xsl:element>
        </xsl:if>
      </xsl:for-each>
    </xsl:element>
    <xsl:element name="modules">
      <xsl:for-each select="//doc:section">
        <xsl:variable name="name" select="normalize-space(substring-before(doc:title, 'Module'))"/>
        <xsl:if test="$name and doc:table[./doc:caption[contains(text(), 'Module')]]/doc:thead/doc:tr/doc:th[.//text()='Tag']">
          <xsl:element name="module">
            <xsl:attribute name="section"><xsl:value-of select="substring-after(@xml:id, 'sect_')"/></xsl:attribute>
            <xsl:attribute name="keyword"><xsl:value-of select="translate($name, ' ()/-:*,\', '')"/></xsl:attribute>
            <xsl:attribute name="name"><xsl:value-of select="$name"/></xsl:attribute>
          </xsl:element>
        </xsl:if>
      </xsl:for-each>
    </xsl:element>
    <xsl:element name="macros">
      <xsl:for-each select="//doc:table">
        <xsl:variable name="name" select="normalize-space(substring-before(doc:caption, 'Macro'))"/>
        <xsl:if test="$name and not(contains(doc:caption, 'Macros') or contains(ancestor::doc:section[2]/doc:title, 'Functional Group')) and doc:thead/doc:tr/doc:th[.//text()='Tag']">
          <xsl:element name="macro">
            <xsl:attribute name="section"><xsl:value-of select="substring-after(ancestor::doc:section[1]/@xml:id, 'sect_')"/></xsl:attribute>
            <xsl:attribute name="table"><xsl:value-of select="substring-after(@xml:id, 'table_')"/></xsl:attribute>
            <xsl:attribute name="keyword"><xsl:value-of select="translate($name, ' ()/-:*,\', '')"/></xsl:attribute>
            <xsl:attribute name="name"><xsl:value-of select="$name"/></xsl:attribute>
          </xsl:element>
        </xsl:if>
      </xsl:for-each>
    </xsl:element>
    <xsl:element name="functional-groups">
      <xsl:for-each select="//doc:table">
        <xsl:variable name="name" select="normalize-space(substring-before(doc:caption, 'Macro'))"/>
        <xsl:if test="$name and contains(ancestor::doc:section[2]/doc:title, 'Functional Group') and doc:thead/doc:tr/doc:th[.//text()='Tag']">
          <xsl:element name="functional-group">
            <xsl:attribute name="section"><xsl:value-of select="substring-after(ancestor::doc:section[1]/@xml:id, 'sect_')"/></xsl:attribute>
            <xsl:attribute name="table"><xsl:value-of select="substring-after(@xml:id, 'table_')"/></xsl:attribute>
            <xsl:attribute name="keyword"><xsl:value-of select="translate($name, ' ()/-:*,\', '')"/></xsl:attribute>
            <xsl:attribute name="name"><xsl:value-of select="$name"/></xsl:attribute>
          </xsl:element>
        </xsl:if>
      </xsl:for-each>
    </xsl:element>
  </xsl:element>
</xsl:template>

</xsl:stylesheet>
