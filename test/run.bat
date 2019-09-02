del /q output
mkdir output

copy /b template1.ods output\*
copy /b test1.ods output\*
copy /b test2.ods output\*
copy /b test3.ods output\*

..\bin\report data1.xml output\template1.ods
..\bin\report data1.xml output\test1.ods
..\bin\report data1.xml output\test2.ods
..\bin\testtool output\
