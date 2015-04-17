
OBJECTS = $(subst cpp,o,$(wildcard *.cpp))
OBJECTS += $(subst cpp,o,$(wildcard */*.cpp))

CFLAGS = -std=c++11 -Ofast -DDIMACS
#CFLAGS = -std=c++11 -g
EXECUTABLE = SteinerExact

%.o: %.cpp
	g++ -I. $(CFLAGS) -c -o $@ $^

$(EXECUTABLE): $(OBJECTS)
	g++ $(CFLAGS) -o $@ $^


clean:
	rm -f */*.o *.o $(EXECUTABLE)
