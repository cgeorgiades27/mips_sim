CPP = g++
DEBUG = -O0 -g
C11 = -std=c++11

all: decode.exe clean

decode.exe: decode9.o
	@echo "Creating..."
	$(CPP) -o $@ $< $(C11)

decode9.o: decode9.cpp
	@echo "Checking..."
	$(CPP) $(DEBUG) -c decode9.cpp $(C11)

clean:
	@echo "Cleaning..."
	rm -f *.o *~ \#*\#
