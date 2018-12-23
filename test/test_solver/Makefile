
# Where to find user code.
USER_DIR = /home/tianju/Dropbox/work/work_station/APC524_final_project/local_git/multiphase
# Flags passed to the C++ compiler.
# CXXFLAGS += -g -Wall -Wextra -pthread -std=c++11

CXXFLAGS += -Wall -std=c++11

TESTFLAGS = -fPIC -O2 -g -Wall -lpthread 


CXX = g++
# All tests produced by this Makefile.  Remember to add new tests you
# created to the list.
TESTS = cell_unittest.bin

# Solver related flags
INC_DIR = $(USER_DIR)/solver/include
LIB_DIR = $(USER_DIR)/eigen
CFLAGS = -I$(INC_DIR) -I$(LIB_DIR) 


# House-keeping build targets.

all : $(TESTS)

cell.o : $(USER_DIR)/solver/source/cell.cpp 
	$(CXX) $(CXXFLAGS) $(CFLAGS) -c $(USER_DIR)/solver/source/cell.cpp

cell_unittest.o : cell_unittest.cpp 
	$(CXX) $(CXXFLAGS) $(CFLAGS) -c cell_unittest.cpp

$(TESTS) : cell.o cell_unittest.o 
	$(CXX) $(CXXFLAGS) $(CFLAGS) $^ libgtest.a -o $@ $(TESTFLAGS)


clean:
	rm *.o
	rm ./cell_unittest.bin

# all:
#	gcc -c ./*.cc
#	g++ ./sample1_unittest.cc sample1.o libgtest.a -o test.bin -fPIC -O2 -g -Wall -lpthread