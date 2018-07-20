# file: makefile
# next line has to be changed to the installation-path of CppUnit
CPPUNIT_PATH=/opt/cppunit

test: LogTest.o
	g++ -o test ../../Utils.cpp ../../UI/CredentialsWindow.cpp ../../UI/ConflictsWindow.cpp ../../GitCommand/Commit.cpp ../../UI/CommitWindow.cpp ../../GitCommand/Init.cpp ../../UI/LogWindow.cpp ../../GitCommand/Log.cpp ../../GitCommand/Add.cpp LogTest.o test.cpp -L${CPPUNIT_PATH}/lib -lcppunit -lbe -ltracker -lgit2 -llocalestub -g

clean:
	rm -f *.o test
