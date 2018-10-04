/******************************
 *
 *  Chris Georgiades
 *  CDA3101 - asg 3
 *
 ******************************/

#include <iostream>
#include <queue>
#include <vector>
#include <map>

void addu(unsigned int rd, unsigned int rs, unsigned int rt)
{
  rd = rs + rt;
}

void addiu(unsigned int rt, unsigned int rs, unsigned int immed)
{
  rs = rt + immed;
}

void lw(unsigned int rt, int offset, unsigned int a[])
{
  rt = a[offset];
}

void sw(unsigned int rt, int offset, unsigned int a[])
{
  a[offset] = rt;
}

void beq(unsigned int rt, unsigned int rs, std::string label)
{
  //if (rt == rs)
    //goto label;
}
//j
//void slt(
//syscall
