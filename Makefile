CPP = g++
DEBUG = -O0 -g
C11 = -std=c++11

all: decode2.exe

decode2.exe: decode-2.o
	@echo "Creating..."
	$(CPP) -o $@ $< $(C11)

decode-2.o: decode-2.cpp 
	@echo "Checking..."
	$(CPP) $(DEBUG) -c decode-2.cpp $(C11)

clean:
	@echo "Cleaning..."
	rm -f *.o *~ \#*\#
