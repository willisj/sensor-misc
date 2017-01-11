
CXX = g++ -std=gnu++11
required_libs = -lpthread -I . -pthread

module: 
	$(CXX) $(required_libs) $(debug_flags) -c misc_sensor.cpp misc_sensor.hpp

example: module
	$(CXX) $(required_libs) $(debug_flags) -o misc_sensor_test misc.cpp misc_sensor.o

debug: debug_flags = -ggdb -O0
debug: module

all: module example 

clean:
	rm misc_sensor.o&
	rm misc_sensor_test&
	rm misc_sensor.hpp.gch
