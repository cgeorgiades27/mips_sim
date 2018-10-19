CPP = g++
DEBUG = -O0 -g
C11 = -std=c++11

all: decode.exe clean

decode.exe: decode12.o
	@echo "Creating..."
	$(CPP) -o $@ $< $(C11)

decode12.o: decode12.cpp
	@echo "Checking..."
	$(CPP) $(DEBUG) -c decode12.cpp $(C11)

clean:
	@echo "Cleaning..."
	rm -f *.o *~ \#*\#
