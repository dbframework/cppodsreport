/*
Copyright (c) 2017-2019 Sidorov Dmitry

Permission is hereby granted, free of charge, to any person obtaining a copy of this software
and associated documentation files (the "Software"), to deal in the Software without restriction, 
including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, 
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial 
portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT 
LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN 
NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.  
*/
#ifndef ODSCONST_H
#define ODSCONST_H

namespace cppodsreport {

#define ODS_VERSION "1.2"

#define ODS_NS_OFFICE "urn:oasis:names:tc:opendocument:xmlns:office:1.0"
#define ODS_NS_TABLE "urn:oasis:names:tc:opendocument:xmlns:table:1.0"
#define ODS_NS_TEXT "urn:oasis:names:tc:opendocument:xmlns:text:1.0"
#define ODS_NS_SCRIPT "urn:oasis:names:tc:opendocument:xmlns:script:1.0"
#define ODS_NS_OF "urn:oasis:names:tc:opendocument:xmlns:of:1.2"
#define ODS_NS_STYLE "urn:oasis:names:tc:opendocument:xmlns:style:1.0"
#define ODS_NS_STYLE_XSL_COMP "urn:oasis:names:tc:opendocument:xmlns:xsl-fo-compatible:1.0"
#define ODS_NS_DRAW "urn:oasis:names:tc:opendocument:xmlns:drawing:1.0"
#define ODS_NS_XLINK "http://www.w3.org/1999/xlink"
#define ODS_NS_DC "http://purl.org/dc/elements/1.1/"
#define ODS_NS_META "urn:oasis:names:tc:opendocument:xmlns:meta:1.0"
#define ODS_NS_NUMBER "urn:oasis:names:tc:opendocument:xmlns:datastyle:1.0"
#define ODS_NS_PRESENTATION "urn:oasis:names:tc:opendocument:xmlns:presentation:1.0"
#define ODS_NS_SVG "urn:oasis:names:tc:opendocument:xmlns:svg-compatible:1.0"
#define ODS_NS_CHART "urn:oasis:names:tc:opendocument:xmlns:chart:1.0"
#define ODS_NS_DR3D "urn:oasis:names:tc:opendocument:xmlns:dr3d:1.0"
#define ODS_NS_MATH "http://www.w3.org/1998/Math/MathML"
#define ODS_NS_FORM "urn:oasis:names:tc:opendocument:xmlns:form:1.0"
#define ODS_NS_RDF "http://www.w3.org/1999/02/22-rdf-syntax-ns#"
#define ODS_NS_RDF_NS0 "http://docs.oasis-open.org/ns/office/1.2/meta/pkg#"
#define ODS_NS_MANIFEST "urn:oasis:names:tc:opendocument:xmlns:manifest:1.0"

#define ODS_NSP_OFFICE "office"
#define ODS_NSP_TABLE "table"
#define ODS_NSP_TEXT "text"
#define ODS_NSP_SCRIPT "script"
#define ODS_NSP_OF "of"
#define ODS_NSP_STYLE "style"
#define ODS_NSP_STYLE_XSL_COMP "fo"
#define ODS_NSP_DRAW "draw"
#define ODS_NSP_XLINK "xlink"
#define ODS_NSP_DC "dc"
#define ODS_NSP_META "meta"
#define ODS_NSP_NUMBER "number"
#define ODS_NSP_PRESENTATION "presentation"
#define ODS_NSP_SVG "svg"
#define ODS_NSP_CHART "chart"
#define ODS_NSP_DR3D "dr3d"
#define ODS_NSP_MATH "math"
#define ODS_NSP_FORM "form"
#define ODS_NSP_RDF "rdf"
#define ODS_NSP_RDF_NS0 "ns0"
#define ODS_NSP_MANIFEST "manifest"

#define ODS_ELEMENT_TABLE "table"
#define ODS_ELEMENT_ROW "table-row"
#define ODS_ELEMENT_CELL "table-cell"
#define ODS_ELEMENT_COVERED_CELL "covered-table-cell"
#define ODS_ELEMENT_DOC_CONTENT "document-content"
#define ODS_ELEMENT_TEXTP "p"
#define ODS_ELEMENT_BODY "body"
#define ODS_ELEMENT_SPREADSHEET "spreadsheet"
#define ODS_ELEMENT_RDF "RDF"
#define ODS_ELEMENT_RDF_DESC "Description"
#define ODS_ELEMENT_RDF_TYPE "type"
#define ODS_ELEMENT_RDF_HASPART "hasPart"
#define ODS_ELEMENT_XML "?xml"
#define ODS_ELEMENT_MANIFEST "manifest"
#define ODS_ELEMENT_MANIFEST_ENTRY "file-entry"
 
#define ODS_ATTR_COL_REP "number-columns-repeated"
#define ODS_ATTR_ROW_REP "number-rows-repeated"
#define ODS_ATTR_FORMULA "formula"
#define ODS_ATTR_NAME "name"
#define ODS_ATTR_VALUE_TYPE "value-type"
#define ODS_ATTR_VALUE "value"
#define ODS_ATTR_VERSION "version"
#define ODS_ATTR_RDF_RESOURCE "resource"
#define ODS_ATTR_RDF_ABOUT "about"
#define ODS_ATTR_MANIFEST_PATH "full-path"
#define ODS_ATTR_MANIFEST_MIMETYPE "media-type"

#define ODSFILE_CONTENT_FILE_NAME "content.xml"
#define ODSFILE_MIMETYPE_FILE_NAME "mimetype"
#define ODSFILE_RDF_FILE_NAME "manifest.rdf"
#define ODSFILE_META_DIR "META-INF"
#define ODSFILE_MANIFEST_FILE_NAME "META-INF/manifest.xml"

#define ODS_RDF_RESOURCE_CONTENT "http://docs.oasis-open.org/ns/office/1.2/meta/odf#ContentFile"
#define ODS_RDF_RESOURCE_DOCUMENT "http://docs.oasis-open.org/ns/office/1.2/meta/pkg#Document"    

#define ODS_MIMETYPE_SPREADSHEET "application/vnd.oasis.opendocument.spreadsheet"
#define ODS_MIMETYPE_XML "text/xml"
#define ODS_MIMETYPE_RDF_XML "application/rdf+xml"

}

#endif // ODSCONST_H
