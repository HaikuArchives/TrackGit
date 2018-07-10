# file: makefile
# next line has to be changed to the installation-path of CppUnit
CPPUNIT_PATH=/opt/cppunit

test: CommitTest.o
	g++ -o test ../../Utils.cpp ../../UI/CredentialsWindow.cpp ../../UI/ConflictsWindow.cpp ../../GitCommand/Commit.cpp ../../UI/CommitWindow.cpp ../../GitCommand/Init.cpp ../../UI/StatusWindow.cpp ../../GitCommand/Status.cpp ../../GitCommand/Add.cpp CommitTest.o test.cpp -L${CPPUNIT_PATH}/lib -lcppunit -lbe -ltracker -lgit2 -llocalestub -g

clean:
	rm -f *.o test
