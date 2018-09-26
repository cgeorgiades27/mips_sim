/******************************
 *
 *  Chris Georgiades
 *  CDA3101 - asg 3
 *
 ******************************/

#include <iostream>

/*
        .text
	addu    $s0,$zero,$zero         # s0 = 0
	addu    $s1,$zero,$zero         # s1 = 0
	addiu   $v0,$zero,5             # v0 = read value
	syscall
	sw      $v0,n($gp)              # M[n] = v0
L1:	lw      $s2,n($gp)              # s2 = M[n]
	slt     $t0,$s1,$s2             # if s1 >= s2 then
	beq     $t0,$zero,L2            #    goto L2
	addiu   $v0,$zero,5             # v0 = read value
	syscall
	addu    $s0,$s0,$v0             # s0 += v0
	addiu   $s1,$s1,1               # s1 += 1
	j       L1                      # goto L2
L2:	addu    $a0,$s0,$zero           # a0 = s0
	addiu   $v0,$zero,1             # print a0
	syscall
	addiu   $v0,$zero,10            # exit
	syscall
	.data
n:	      .word   0
*/

std::map<std::string, unsigned int> reg =
{
  {"zero", 0x0},
  {"at", 0x1},
  {"v0", 0x2},
  {"v1", 0x3},
  {"a0", 0x4},
  {"a1", 0x5},
  {"a2", 0x6},
  {"a3", 0x7},
  {"t0", 0x8},
  {"t1", 0x9},
  {"t2", 0xa},
  {"t3", 0xb},
  {"t4", 0xc},
  {"t5", 0xd},
  {"t6", 0xe},
  {"t7", 0xf},
  {"s0", 0x10},
  {"s1", 0x11},
  {"s2", 0x12},
  {"s3", 0x13},
  {"s4", 0x14},
  {"s5", 0x15},
  {"s6", 0x16},
  {"s7", 0x17},
  {"t8", 0x18},
  {"t9", 0x19},
  {"k0", 0x1a},
  {"k1", 0x1b},
  {"gp", 0x1c},
  {"sp", 0x1d},
  {"fp", 0x1e},
  {"ra", 0x1f}
};

std::map<std::string, unsigned int> funct =
{
  {"addiu", 0x9},
  {"addu", 0x21},
  {"beq", 0x4},
  {"j", 0x2},
  {"lw", 0x23},
  {"slt", 0x2a},
  {"sw", 0x2b},
  {"syscall", 0xc}
};

unsigned int rs_:5;
unsigned int rt_:5;
unsigned int opcode_:6;
unsigned int immed_:16;
unsigned int opcode_:6;
unsigned int targaddr_:26;
unsigned int funct_:6;


void addu(unsigned int rd:5, unsigned int rs:5, unsigned int rt:5)
{
  rd = rs + rt;
};

void addiu(unsigned int rt:5, unsigned int rs:5, unsigned int immed:16)
{
  rs = rt + immed;
};

void lw(unsigned int rt:5, int offset, unsigned int a[])
{
  rt = a[offset];
};

void sw(unsigned int rt:5, int offset, unsigned int a[])
{
  a[offset] = rt;
};

void beq(unsigned int rt:5, unsigned int rs:5, std::string label)
{
  if (rt == rs)
    goto label;
}
//j
//void slt(
//syscall

