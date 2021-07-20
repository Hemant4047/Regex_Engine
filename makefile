test_regex: engine.o regex.o testbase.o test_regex.o
	g++ engine.o regex.o testbase.o test_regex.o -o test_regex

test_regex.o: ./tester/test_regex.cpp
	g++ -c ./tester/test_regex.cpp

testbase.o: ./tester/testbase.cpp	
	g++ -c ./tester/testbase.cpp

# test: engine.o regex.o
# 	g++ engine.o regex.o -o test

engine.o: engine.cpp
	g++ -c engine.cpp

regex.o: regex.cpp
	g++ -c regex.cpp

clean: 
	del *.o *.exe