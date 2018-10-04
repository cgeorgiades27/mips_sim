CPP = g++
DEBUG = -O0 -g
C11 = -std=c++11

all: decode.exe clean

decode.exe: decode.o
	@echo "Creating..."
	$(CPP) -o $@ $< $(C11)

decode.o: decode.cpp
	@echo "Checking..."
	$(CPP) $(DEBUG) -c decode.cpp $(C11)

clean:
	@echo "Cleaning..."
	rm -f *.o *~ \#*\#
