CPP = g++
DEBUG = -O0 -g
C11 = -std=c++11

all: decode.exe clean

decode.exe: decode7.o
	@echo "Creating..."
	$(CPP) -o $@ $< $(C11)

decode7.o: decode7.cpp
	@echo "Checking..."
	$(CPP) $(DEBUG) -c decode7.cpp $(C11)

clean:
	@echo "Cleaning..."
	rm -f *.o *~ \#*\#
