CXX=g++
CXXFLAGS=-std=c++11

default: runfile

runfile: main.cpp helper.h helper.cpp formatter.h formatter.cpp rom.h rom.cpp
	$(CXX) $(CXXFLAGS) main.cpp helper.cpp formatter.cpp rom.cpp -o runfile

clean:
	rm -f *.o
	rm -f runfile

# runfile.o: main.cpp
# 	$(CXX) -c main.cpp -o runfile.o
 
# runfile: runfile.o
# 	$(CXX) runfile.o  -o runfile

# clean:
# 	rm -f *.o
# 	rm -f runfile