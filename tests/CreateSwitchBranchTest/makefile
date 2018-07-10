# file: makefile
# next line has to be changed to the installation-path of CppUnit
CPPUNIT_PATH=/opt/cppunit

test: CreateSwitchBranchTest.o
	g++ -o test ../../UI/CreateBranchWindow.cpp ../../UI/SwitchBranchWindow.cpp ../../GitCommand/CreateBranch.cpp ../../GitCommand/SwitchBranch.cpp ../../GitCommand/Init.cpp ../../UI/StatusWindow.cpp ../../GitCommand/Status.cpp CreateSwitchBranchTest.o test.cpp -L${CPPUNIT_PATH}/lib -lcppunit -lbe -ltracker -lgit2 -llocalestub -g

clean:
	rm -f *.o test
