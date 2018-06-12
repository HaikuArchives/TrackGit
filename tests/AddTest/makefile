# file: makefile
# next line has to be changed to the installation-path of CppUnit
CPPUNIT_PATH=/opt/cppunit

test: AddTest.o
	g++ -o test ../../GitCommand/Init.cpp ../../UI/StatusWindow.cpp ../../GitCommand/Status.cpp ../../GitCommand/Add.cpp AddTest.o test.cpp -L${CPPUNIT_PATH}/lib -lcppunit -lbe -ltracker -lgit2 -llocalestub -g

clean:
	rm -f *.o test
