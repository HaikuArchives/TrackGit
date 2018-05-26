# file: makefile
# next line has to be changed to the installation-path of CppUnit
CPPUNIT_PATH=/opt/cppunit

itest: InitTest.o
	g++ -o test ../../GitCommand/Init.cpp InitTest.o test.cpp -L${CPPUNIT_PATH}/lib -lcppunit -lbe -ltracker -lgit2 -g

clean:
	rm -f *.o test
