# Define the compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -g

# Define the executable name
TARGETS = serverS

# List all the object files
OBJS = serverS.o utils.o

TARGETM = serverM
OBJM = serverM.o utils.o

TARGETL = serverL
OBJL = serverL.o utils.o

TARGETH = serverH
OBJH = serverH.o utils.o

# The first rule is the one executed when no parameters are fed into the makefile
all: $(TARGETS) $(TARGETM) $(TARGETL) $(TARGETH)

# Rule for creating the executable
$(TARGETS): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGETS) $(OBJS)

$(TARGETM): $(OBJM)
	$(CXX) $(CXXFLAGS) -o $(TARGETM) $(OBJM)

$(TARGETL): $(OBJL)
	$(CXX) $(CXXFLAGS) -o $(TARGETL) $(OBJL)

$(TARGETH): $(OBJH)
	$(CXX) $(CXXFLAGS) -o $(TARGETH) $(OBJH)

# Rule for creating object file test1.o
serverS.o: serverS.cpp utils.h
	$(CXX) $(CXXFLAGS) -c serverS.cpp

# Rule for creating object file test2.o
utils.o: utils.cpp utils.h
	$(CXX) $(CXXFLAGS) -c utils.cpp

#Rule for creating object file serverM.o
serverM.o: serverM.cpp utils.h
	$(CXX) $(CXXFLAGS) -c serverM.cpp

serverL.o: serverL.cpp utils.h
	$(CXX) $(CXXFLAGS) -c serverL.cpp

serverH.o: serverH.cpp utils.h
	$(CXX) $(CXXFLAGS) -c serverH.cpp

# Rule for cleaning up
clean:
	rm -f $(TARGETS) $(OBJS) $(TARGETM) $(OBJM) $(TARGETH) $(OBJH) $(TARGETL) $(OBJL)
