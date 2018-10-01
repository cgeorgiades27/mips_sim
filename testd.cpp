#include <iostream>
#include "decode.cpp"

#define MAXLINE 80

char line[MAXLINE];
unsigned int funct_, shamt_, rd_, rs_, rt_, opcode_, addr_, immed_;
unsigned int instruction;
void decode(unsigned int instruction);

int main()
{
  while(fgets(line, MAXLINE, stdin))
    {
      sscanf(line, "%x", &instruction);
      printf("instruction parsed: %x\n", instruction);
      decode(instruction);
    }
return 0;
}
