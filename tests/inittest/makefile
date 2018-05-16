# CppUnit-Tutorial
# file: makefile
# next line has to be changed to the installation-path of CppUnit
CPPUNIT_PATH=/opt/cppunit

itest: inittest.o
	g++ -o itest ../../GitCommand/Init.cpp inittest.o itest.cpp -L${CPPUNIT_PATH}/lib -lcppunit -lbe -ltracker -lgit2 -g

clean:
	rm -f *.o ftest
