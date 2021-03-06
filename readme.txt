Cppodsreport is a C++ library for generating reports in ODF spreadsheet format (*.ods). The library can be used to:
- create and edit spreadsheet files;
- generate reports by substituting variables in the spreadsheet template with their values.

Cppodsreport uses libzip library (available at https://nih.at/libzip/index.html) and compatible XML parser. The current version supports XML parser from Qt 5 SDK (available at https://www.qt.io/download/ under LGPL v3.0 license). When using XML parser from Qt only Qt5Core and Qt5XML libraries are required.

1. Building the library.

To build cppodsreport library the following software is required:
- CMake, version at least 3.6 (available at https://cmake.org/download/);
- C++ 11 compiler with STL for which CMake can generate project/make file;
- libzip library, version at least 1.1.3;
- Qt SDK, version at least 5.2.0.

The building process consists of 3 steps:
- install required software;
- generate a project file using CMake;
- build generated project file using C++ 11 compiler.
As a result of successful build two modules must be generated: cppodsreport library and report command-line application.

The step of generating a project file using CMake is described later.

First, create a directory where generated project file will be placed. Switch to this directory. Run 
cmake --help
to see available generators. Then run 
cmake -G <generator-name> <path to the directory of cppodsreport library>
CMake will try to find libzip library, Qt library and generate a project file. 

If CMake fails to find libzip files (zip.h, zipconf.h or library itself) add -D CMAKE_PREFIX_PATH=<path list> to cmake options list. <path list> should contain a list of paths, separated with ';', to the following files from libzip library:
- import library under Windows or library itself on other platforms;
- zip.h header file;
- zipconf.h header file.

If CMake fails to find Qt add -D Qt5Core_DIR=<core path> -D Qt5Xml_DIR=<xml path> to cmake options list. Here <core path> is the directory with the cmake configuration file for QtCore, <xml path> is the directory with the cmake configuration file for QtXml.

2. Using report tool.

Report command-line tool uses cppodsreport library to generate report by substituting variables in the spreadsheet template with their values stored in a data file. It serves as the example of an application using cppodsreport library and also can be used to test the library.

Report command-line tool requires two arguments:
report <data file name> <template file name>
<data file name> - name of the file containing variables values. The format of this file is described in section 5.
<template file name> - name of the ODF spreadsheet file with report templates. Template format is described in section 4.

Report command-line tool modifies template file by replacing variables with their values contained in data file.

Cppodsreport source files include sample template file (template1.ods) and data file (data1.xml). This files can be used to test cppodsreport library. The result of running report command-line tool with these files as parameters must be the same as reference file (result1.ods). "The same" here doesn't mean that the files are identical in the sence of binary compare. This mean that they must look identically in some ODF spreadsheet editing tool, such as OpenOffice Calc. This is so because different implementations or versions of xml parsers can produce xml files with different formatting that are identical from the point of view of a ODF spreadsheet editing software.

3. Using the library.

3.1. Using the library to create or edit spreadsheet.

To use the library, first include cppodsreport.h header in your project. This header is in the include subdirectory. It declares functions:

CPPODSREPORT_API cppodsreport::ODFPackage* create();
CPPODSREPORT_API void release(cppodsreport::ODFPackage* p);

Use the create() function to get the pointer to the allocated cppodsreport::ODFPackage instance. Use cppodsreport::ODFPackage methods to open, save and modify the spreadsheet.
See cppodsreport::ODFPackage methods description in the documentation. 

After using cppodsreport::ODFPackage instance release it by call to release(...).

You can use the ODFPackageWrapper class, which allocates cppodsreport::ODFPackage instance in the constructor
and releases it in the destructor.

3.2. Using the library to create report from spreadsheet template.

The process of report creation with cppodsreport library consists of three steps. 

First, create report template file. Template file format is described in section 4.

Second, implement the descendant of cppodsreport::DataSource class in your application. The cppodsreport::DataSource class must implement the retrieval of variable values by their names. See documentation for cppodsreport::DataSource description. 

Third, call the function, exported by the library:
CPPODSREPORT_API bool report(const char* fileName, cppodsreport::DataSource* dataSource, int* intError, int* zipError, int* zlibError, int* sysError);

This function is declared in cppodsreport.h file located in the include subdirectory of cppodsreport. This function generates report by replacing variables in the template file with their values. 

Parameters.
fileName - template file name in UTF-8 encoding. This file will be modified by replacing variables with their values. Template format is described in section 4.
dataSource - a pointer to an instance of cppodsreport::DataSource descendant.
intError - if not null this variable will be filled with internal error code. The internal error codes are listed in odserror.h file located in the include subdirectory.
zipError - if not null this variable will be filled with libzip error code. Libzip error codes are described in zip.h file which is the part of libzip.
zlibError - if not null this variable will be filled with zlib error code. Zlib error codes are described in zlib.h file which is the part of zlib.
sysError - if not null this variable will be filled with errno.

Return value.
The function returns true if success or false otherwise. 

On Windows platform the library additionally exports two more functions:
CPPODSREPORT_API bool reporta(const char* fileName, cppodsreport::DataSource* dataSource, int* intError, int* zipError, int* zlibError, int* sysError);
CPPODSREPORT_API bool reportw(const wchar_t* fileName, cppodsreport::DataSource* dataSource, int* intError, int* zipError, int* zlibError, int* sysError);

They differ from report function only in fileName parameter. The reporta function requires fileName in ANSI (Windows) encoding. The reportw function requires fileName in UTF-16 encoding. 

4. Template file format and template processing.

A template file is an ODF spreadsheet file with special markup of variables and blocks. Variables and blocks are processed by the library. All other content remains unchanged. The library scans template cells row by row downwards starting from top row. Each row is scanned from left to right. If the cell text contains variables or block command (see later) the library processes them.

A block in a template file is some rectangular area of cells. A block in a template file is marked by the command placed in the top-left cell of the block. The command has format: 
{V,<block height>,<block width>,<table name>}

The arguments <block height> and <block width> define block size. They can be positive numbers or * symbol. The * symbol means that the block spans up to the end of the spreadsheet. <Table name> argument is used to identify data the block is associated with. 

The library uses the following concept of report data representation. It is assumed that report data is stored within some data source (data source is the instance of cppodsreport::DataSource descendant, see section 3). The report data is represented within the data source as the set of named tables. 
Each table can have arbitrary number of named columns and rows. <Table name> argument must identify table with which the block is associated.

When the library detects block start command, it calls cppodsreport::DataSource::next method, passing <table name> as a parameter. If cppodsreport::DataSource::next returns true, the library inserts <block height> rows into the document below the block and copies the block there, including the block command. Then the library deletes the block command from the current cell text and continues processing.  If cppodsreport::DataSource::next returns false, the library removes the rows containing the block and continues processing.

So, blocks are the way to reproduce some part of the template as many times as the number of rows in some table of the data source. Note that the cell with the block command may contain some other text and even some variables (see later). It is possible to use nested blocks, but this requires cppodsreport::DataSource descendant to support some kind of table nesting.

Some examples of block commands.
{V,1,*,Table1} - the block consists of the part of the current row starting from cell with the command and ending at the end  of the row. The block is associated with Table1.
{V,2,3,Table2} - the block of cells 2x3 size starting from cell with the command, spanning 3 cells to the right and two cells down. The block is associated with Table2.

Variables in the template file are represented as variable name enclosed in brackets. The variable name must encode table name and column name in the way  cppodsreport::DataSource descendant understands. For example: [Table1.Column2]. The library passes variable name to cppodsreport::DataSource::value method. If it returns true, the library replaces variable with it's value, otherwise it leaves variable unchanged.

5. Report tool data file format.

Report tool data file is a XML file. Cppodsreport source files include schema report.xsd which can be used to validate report tool data file.

All elements and attributes in a data file are declared in namespace "urn:cppodsreport:report:1.0". The document element is  DataSource. The DataSource element must contain at least one Table element. 

Table element must have attributes Name and BlockTable. Name is a string attribute. It must contain table name. BlockTable is a boolean attribute. It must contain true if table is associated with some block in the template or false otherwise. Table element must contain one or more Column elements, followed by zero or more Row elements.

Column element mast have Name attribute. Name is a string attribute. It must contain column name.

Row element must have one or more Cell elements.

Cell element must have attribute ColumnName. ColumnName is a string attribute. It must contain column name which is equal to the Name attribute of one of the Column elements. Cell element must contain Value element or Table element. 

Table element represents nested table. It must have the same elements as Table elements that are direct children of DataSource element. But it mustn't have any attributes. It is supposed that nested table is always related with some block and its name is formed as <ParentTableName>.<ColumnName>.

Value element represents cell value. It is of type string and must contain cell text.

6. Tests.

Some tests for the library can be found in test subdirectory. After installing the library, you can run the tests
using scripts run.bat or run.sh in the test subdirectory. This script creates output subdirectory and *.ods files
in it. To see if the test was successfull, open these files with some ODF spreadsheet editing software. It is not
possible to just binary compare the files because their content depends on XML parser implementation. 

The file template1.ods must look the same as file result1.ods from test subdirectory. In the files test1 - test3 
the sheet "test" must look the same as the sheet "result". In the testsave.ods file content must match the 
description in the first line.
