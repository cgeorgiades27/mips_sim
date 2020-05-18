CPP = g++
DEBUG = -O0 -g
C11 = -std=c++11

all: sim.exe clean

sim.exe: sim.o
	@echo "Creating..."
	$(CPP) -o $@ $< $(C11)

sim.o: sim.cpp
	@echo "Checking..."
	$(CPP) $(DEBUG) -c sim.cpp $(C11)

clean:
	@echo "Cleaning..."
	rm -f *.o *~ \#*\#
