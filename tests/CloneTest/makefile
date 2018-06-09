# file: makefile
# next line has to be changed to the installation-path of CppUnit
CPPUNIT_PATH=/opt/cppunit

itest: CloneTest.o
	g++ -o test ../../ThirdParty/PathBox.cpp ../../UI/CloneWindow.cpp ../../GitCommand/Clone.cpp CloneTest.o test.cpp -L${CPPUNIT_PATH}/lib -lcppunit -lbe -ltracker -lgit2 -llocalestub -g 

clean:
	rm -f *.o test
