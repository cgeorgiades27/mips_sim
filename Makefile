CPP = g++
DEBUG = -O0 -g
C11 = -std=c++11

all: main3.exe clean

main3.exe: main-3.o
	@echo "Creating..."
	$(CPP) -o $@ $< $(C11)

main-3.o: main-3.cpp
	@echo "Checking..."
	$(CPP) $(DEBUG) -c main-3.cpp $(C11)

clean:
	@echo "Cleaning..."
	rm -f *.o *~ \#*\#
