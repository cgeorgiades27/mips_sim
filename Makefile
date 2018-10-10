CPP = g++
DEBUG = -O0 -g
C11 = -std=c++11

all: decode.exe clean

decode.exe: decode8.o
	@echo "Creating..."
	$(CPP) -o $@ $< $(C11)

decode8.o: decode8.cpp
	@echo "Checking..."
	$(CPP) $(DEBUG) -c decode8.cpp $(C11)

clean:
	@echo "Cleaning..."
	rm -f *.o *~ \#*\#
