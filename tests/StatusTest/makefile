# file: makefile
# next line has to be changed to the installation-path of CppUnit
CPPUNIT_PATH=/opt/cppunit

test: StatusTest.o
	g++ -o test ../../GitCommand/Init.cpp ../../GitCommand/Status.cpp StatusTest.o test.cpp -L${CPPUNIT_PATH}/lib -lcppunit -lbe -ltracker -lgit2 -g

clean:
	rm -f *.o test
