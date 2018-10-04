CPP = g++
DEBUG = -O0 -g
C11 = -std=c++11

all: main4.exe clean

main4.exe: main-4.o
	@echo "Creating..."
	$(CPP) -o $@ $< $(C11)

main-4.o: main-4.cpp
	@echo "Checking..."
	$(CPP) $(DEBUG) -c main-4.cpp $(C11)

clean:
	@echo "Cleaning..."
	rm -f *.o *~ \#*\#
