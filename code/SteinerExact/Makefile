
# C++ compiler
GCC = g++

# Compiler flags
CFLAGS = -std=c++11 -Ofast
#CFLAGS = -std=c++11 -Ofast -DDIMACS 	#Used to output for the DIMACS challenge
#CFLAGS = -std=c++11 -g					#Used for debugging
EXECUTABLE = SteinerExact

# Compile all cpp files 
SOURCES = $(wildcard *.cpp)
SOURCES+= $(wildcard */*.cpp)
OBJECTS = $(SOURCES:.cpp=.o)


%.o: %.cpp
	$(GCC) -I. $(CFLAGS) -c -o $@ $^

$(EXECUTABLE): $(OBJECTS)
	$(GCC) $(CFLAGS) -o $@ $^


clean:
	rm -f */*.o *.o $(EXECUTABLE)
