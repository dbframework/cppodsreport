cmake_minimum_required(VERSION 3.6)

project(cppodsreport VERSION 1.1.0)

set(SRC_LIB src/cppodsreport.cpp 
            src/odsfile.cpp 
            src/cziparchive.cpp 
            src/odscell.cpp 
            src/odscelladdress.cpp 
            src/odscellrange.cpp 
            src/odsformula.cpp 
            src/odstable.cpp 
            src/odstablerow.cpp 
            src/parser.cpp 
            src/xmldom.cpp 
            src/ziparchivefile.cpp
            src/zipfilestat.cpp
)

set(SRC_REPORT  src/report.cpp 
                src/datasourceimpl.cpp 
                src/xmldom.cpp
)

set(SRC_TEST test/main.cpp
             test/cppodsreporttest.cpp
             test/test3.cpp
)

#setup libzip paths
find_library(ZIP_LIB zip)
find_path(ZIP_H "zip.h")
find_path(ZIPCONF_H "zipconf.h")
set(LIBZIP_INCLUDE ${ZIP_H} ${ZIPCONF_H})
add_library(zip SHARED IMPORTED)
if(WIN32 OR CYGWIN)
  set_property(TARGET zip PROPERTY IMPORTED_IMPLIB ${ZIP_LIB})
else(WIN32 OR CYGWIN)
  set_property(TARGET zip PROPERTY IMPORTED_LOCATION ${ZIP_LIB})
endif(WIN32 OR CYGWIN)

#find qt
#set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(Qt5Core REQUIRED)
find_package(Qt5Xml REQUIRED)

#cppodsreport library
add_library(cppodsreport SHARED ${SRC_LIB})
target_include_directories(cppodsreport PUBLIC include)
target_include_directories(cppodsreport PRIVATE ${LIBZIP_INCLUDE})
target_link_libraries(cppodsreport Qt5::Core Qt5::Xml)
target_link_libraries(cppodsreport zip)
target_compile_definitions(cppodsreport PRIVATE CPPODSREPORT_EXPORT)
if(WIN32 AND NOT CYGWIN)
  target_compile_definitions(cppodsreport PUBLIC CPPODSREPORT_WIN)
endif(WIN32 AND NOT CYGWIN)
set_property(TARGET cppodsreport PROPERTY CXX_STANDARD 11)
set_property(TARGET cppodsreport PROPERTY CXX_STANDARD_REQUIRED ON)

#report tool
add_executable(report ${SRC_REPORT})
target_link_libraries(report cppodsreport)
target_link_libraries(cppodsreport Qt5::Core Qt5::Xml)
set_property(TARGET report PROPERTY CXX_STANDARD 11)
set_property(TARGET report PROPERTY CXX_STANDARD_REQUIRED ON)

#test tool
add_executable(testtool ${SRC_TEST})
target_link_libraries(testtool cppodsreport)
set_property(TARGET testtool PROPERTY CXX_STANDARD 11)
set_property(TARGET testtool PROPERTY CXX_STANDARD_REQUIRED ON)
