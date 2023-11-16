# Define the compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -g

# Define the executable name
TARGET = serverS serverM

# List all the object files
OBJS = serverS.o utils.o

# The first rule is the one executed when no parameters are fed into the makefile
all: $(TARGET)

# Rule for creating the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Rule for creating object file test1.o
serverS.o: serverS.cpp utils.h
	$(CXX) $(CXXFLAGS) -c serverS.cpp

# Rule for creating object file test2.o
utils.o: utils.cpp utils.h
	$(CXX) $(CXXFLAGS) -c utils.cpp

#Rule for creating object file serverM.o
serverM.o: serverM.cpp
	$(CXX) $(CXXFLAGS) -c serverM.cpp

# Rule for cleaning up
clean:
	rm -f $(TARGET) $(OBJS)
