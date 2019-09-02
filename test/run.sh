mkdir output
rm output/*

cp template1.ods output
cp test1.ods output
cp test2.ods output
cp test3.ods output

../bin/report data1.xml output/template1.ods
../bin/report data1.xml output/test1.ods
../bin/report data1.xml output/test2.ods
../bin/testtool output/
