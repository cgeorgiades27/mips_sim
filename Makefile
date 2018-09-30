CPP = g++
DEBUG = -O0 -g
C11 = -std=c++11

all: decode3.exe clean

decode3.exe: decode-3.o
	@echo "Creating..."
	$(CPP) -o $@ $< $(C11)

decode-3.o: decode-3.cpp 
	@echo "Checking..."
	$(CPP) $(DEBUG) -c decode-3.cpp $(C11)

clean:
	@echo "Cleaning..."
	rm -f *.o *~ \#*\#
