/* 
 * CS:APP Data Lab 
 * 
 * <Please put your name and userid here>
 * Name: John Tran
 * Userid: 101821704
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implent floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function. 
     The max operator count is checked by dlc. Note that '=' is not 
     counted; you may use as many of these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif
/* Copyright (C) 1991-2016 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */
/* This header is separate from features.h so that the compiler can
   include it implicitly at the start of every compilation.  It must
   not itself include <features.h> or any other header that includes
   <features.h> because the implicit include comes before any feature
   test macros that may be defined in a source file before it first
   explicitly includes a system header.  GCC knows the name of this
   header in order to preinclude it.  */
/* glibc's intent is to support the IEC 559 math functionality, real
   and complex.  If the GCC (4.9 and later) predefined macros
   specifying compiler intent are available, use them to determine
   whether the overall intent is to support these features; otherwise,
   presume an older compiler has intent to support these features and
   define these macros by default.  */
/* wchar_t uses Unicode 8.0.0.  Version 8.0 of the Unicode Standard is
   synchronized with ISO/IEC 10646:2014, plus Amendment 1 (published
   2015-05-15).  */
/* We do not support C11 <threads.h>.  */

/* 
 * allEvenBits - return 1 if all even-numbered bits in word set to 1
 *   Examples allEvenBits(0xFFFFFFFE) = 0, allEvenBits(0x55555555) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allEvenBits(int x) {
    /*
     * use stacking of bits in reducing powers of 2 to quickly find a
     * repeated pattern of "01" and bitwise and at each step -- basically
     * bitwise and the first half of x with the second half and keeping track
     * if every other bit starting at the least significant bit (see
     * at the end of the comment) is a 1
     *
     * the only way for a 1 to persist in the least significant bit after
     * each step is for every other bit starting at the least significant bit
     * to be a 1 (if there is even a single bit along the steps where there
     * is a 0 bit, then the bitwise and would propagate the 0 down the steps
     * to the final two bits since any bit anded with 0 is 0)
     *
     * since we just need to look at every other bit starting at the least
     * significant bit (see below), we bitwise and it with the mask 0x01 at
     * the end to make sure that bit is a 1 after the successive bitwise ands
     *
     * even-numbered bits start at index 0 (least significant bit)
     */

    int even16 = x & (x >> 16);
    int even8 = even16 & (even16 >> 8);
    int even4 = even8 & (even8 >> 4);
    int even2 = even4 & (even4 >> 2);

    return even2 & 0x1;
}

/*
 * bitParity - returns 1 if x contains an odd number of 0's
 *   Examples: bitParity(5) = 0, bitParity(7) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 4
 */
int bitParity(int x) {
    /*
     * stacking of bits (as with the allEvenBits() function above) in reducing
     * powers of 2 to quickly find if there's an odd number of 0s -- stepping
     * through the algorithm, the first 16 bits of x are compared with the
     * last 16 bits of x (shifted towards the least significant bit for
     * easier checking at the end) and bitwise xor-ed together
     *
     * this results in a 0 if both bits are 0 or 1 (we don't care about two
     * 1s since we're just looking at the number of 0s, and two 0s is even,
     * so can safely ignore it -- basically results in (mod 2) arithmetic
     * with the number of 0s at the end)
     *
     * otherwise, this results in a 1 if there is a pairing of a 0 and a 1
     * (one each) and keeps track of a single 0 present (stored as a 1)
     * in each subsequent groupings (split into two groups), the first half
     * is bitwise xor-ed together with the second half, keeping track if there
     * is a single 0 in each bit (e.g., 0000 -> 00 -> 0; 1000 -> 10 -> 1)
     *
     * this continues until all bits in x are checked and the result stored
     * in the least significant bit (since left logical shifts are used) as
     * a 0 if there is effectively an even number of 0s (including no 0s) or
     * a 1 (a 1 persists in the successive xor operations of the first and
     * second half if 1 is in any of the bits in the previous xor operation,
     * and it is xor-ed with a 0 all the way to the final two bit comparison
     * -- meaning that the 0s are a result of encountering an even number of
     * 0s along the way)
     */
    int parity16 = x ^ (x >> 16);
    int parity8 = parity16 ^ (parity16 >> 8);
    int parity4 = parity8 ^ (parity8 >> 4);
    int parity2 = parity4 ^ (parity4 >> 2);
    int parity1 = parity2 ^ (parity2 >> 1);

    return parity1 & 0x01;
}

/*
 * bitXor - x^y using only ~ and & 
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 14
 *   Rating: 1
 */
int bitXor(int x, int y) {
    /*
     *  since we can only use the bit operators ~ and &, this hinted the use
     *  of the bitwise nand operation (not of x and y)
     *
     *  comparing the truth tables of nand(x, y) and nand(~x, ~y), we found
     *  that the and of the two operations resulted in our bitwise xor of x
     *  and y
     */

    return (~(x & y)) & (~((~x) & (~y)));
}

/*
 * leastBitPos - return a mask that marks the position of the
 *               least significant 1 bit. If x == 0, return 0
 *   Example: leastBitPos(96) = 0x20
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2 
 */
int leastBitPos(int x) {
    /*
     * if we take advantage of the bitwise complement of x, then we know the
     * result when this is bitwise and with the original x should be 0
     *
     * however, we can just add 1 to the bitwise complement of x before doing
     * the bitwise and with x to make sure the result is not 0 anymore -- we
     * can prove that one bit will be 1 (our desired mask)
     *
     * the best way is to illustrate this with some examples (in binary
     * notation):
     *
     * x = 0001
     * (~x) + 1 = 1110 + 1 = 1111
     * bitwise and -> 0001
     *
     * we can see from the above example that if the least significant is 1
     * (regardless of the other bits since the complement would have a 0 in
     * the least significant bit and adding a 1 would only change this bit to
     * a 1, leaving the other bits unchanged), then the bitwise and would
     * correctly determine the least significant bit 1 bit as the least
     * significant bit
     *
     * now, let's look at another example where the least significant 1 bit
     * is not the least significant bit:
     *
     * x = 0110 1100
     * (~x) + 1 = 1001 0011 + 1 = 1001 0100
     * bitwise and -> 0000 0100
     *
     * this example is a little more illuminating (more bits to work with and
     * more than one bit contains a 1) as we can see that (~x) + 1 will only
     * change bits up to the least significant 1 bit position in the
     * complement and "restore" the 0s from the original x up to position
     * right before the least significant 1 bit position
     *
     * basically, adding 1 to ~x would match the bits of x up to the least
     * significant 1 bit of x (which means a 1 followed by any number of 0s)
     * while preserving the rest of the complement
     *
     * when we take the bitwise and, the bits to the left would be zeroed out
     * since the complement is preserved, and we are just left with a 1 bit
     * at the least significant 1 bit of x with zeroes following after it
     * (since zeroes are supposed to follow the least significant 1 bit by
     * definition), giving us our desired mask
     */

    return x & ((~x) + 1);
}

/*
 * replaceByte(x,n,c) - Replace byte n in x with c
 *   Bytes numbered from 0 (LSB) to 3 (MSB)
 *   Examples: replaceByte(0x12345678,1,0xab) = 0x1234ab78
 *   You can assume 0 <= n <= 3 and 0 <= c <= 255
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 10
 *   Rating: 3
 */
int replaceByte(int x, int n, int c) {
    /*
     * originally, I had thought of shifting x so there would be 0s at byte n
     * (where c should be). However, I couldn't find a way to get the right
     * bits of x (below byte n) easily without subtraction -- need a bit
     * mask (some consecutive bits of all 1s)
     *
     * so, we came up with an easier way to just zero out byte n in x without
     * needed to shift x
     *
     * before we begin, it is important to note there are 8 bits in one byte
     * and to get the appropriate number of bits to shift given n (in bytes),
     * we can just multiply n by 8 (equivalent to a left shift of 3 -- 2^3 =
     * 8 since each left shift is the same thing as multiplying by 2)
     *
     * first, since we are given that we only need to change one byte of x, a
     * bit mask (all 1s) equal to the length of one byte was created and
     * shifted to byte n (overlap with byte n in x)
     *
     * then, we need to find a way to zero out byte n in x with our bit mask
     * -- we found a clever way by first bitwise or-ing it with x (only byte
     * n is affected since the mask has zeros elsewhere) to make byte n in x
     * all 1s
     *
     * to convert those 1s in byte n of x to 0s, we know that 1 xor 1 is just
     * 0, and we could use the same mask to make this happen (again, the
     * other bits are unaffected since 0 ^ a = a for any bit value a)
     *
     * finally, we could just bitwise or this result with our value of c
     * after shifting it in place
     */

    int numShifts = n << 3;
    int mask = 0xff;
    int adjustedMask = mask << numShifts;
    x = x | adjustedMask;
    x = x ^ adjustedMask;

    // something
    int adjustedC = c << numShifts;

    return x | adjustedC;
}

/*
 * TMax - return maximum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmax(void) {
    /*
     * 
     */
    int maxBitMask = (~0);
    int shiftedBitMask = maxBitMask >> 1;
    int leftMask = ~(shiftedBitMask);

    return leftMask;
}

/*
 * fitsBits - return 1 if x can be represented as an 
 *  n-bit, two's complement integer.
 *   1 <= n <= 32
 *   Examples: fitsBits(5,3) = 0, fitsBits(-4,3) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int fitsBits(int x, int n) {
    return 2;
}

/*
 * divpwr2 - Compute x/(2^n), for 0 <= n <= 30
 *  Round toward zero
 *   Examples: divpwr2(15,1) = 7, divpwr2(-33,4) = -2
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int divpwr2(int x, int n) {
    return 2;
}

/*
 * isEqual - return 1 if x == y, and 0 otherwise 
 *   Examples: isEqual(5,5) = 1, isEqual(4,5) = 0
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int isEqual(int x, int y) {
    return 2;
}

/*
 * isPositive - return 1 if x > 0, return 0 otherwise 
 *   Example: isPositive(-1) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 3
 */
int isPositive(int x) {
    return 2;
}

/*
 * subOK - Determine if can compute x-y without overflow
 *   Example: subOK(0x80000000,0x80000000) = 1,
 *            subOK(0x80000000,0x70000000) = 0, 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 */
int subOK(int x, int y) {
    return 2;
}

/* howManyBits - return the minimum number of bits required to represent x in
 *             two's complement
 *  Examples: howManyBits(12) = 5
 *            howManyBits(298) = 10
 *            howManyBits(-5) = 4
 *            howManyBits(0)  = 1
 *            howManyBits(-1) = 1
 *            howManyBits(0x80000000) = 32
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 90
 *  Rating: 4
 */
int howManyBits(int x) {
    return 0;
}

/*
 * float_abs - Return bit-level equivalent of absolute value of f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument..
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 10
 *   Rating: 2
 */
unsigned float_abs(unsigned uf) {
    return 2;
}

/*
 * float_twice - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_twice(unsigned uf) {
    return 2;
}

/*
 * trueFiveEighths - multiplies by 5/8 rounding toward 0,
 *  avoiding errors due to overflow
 *  Examples: trueFiveEighths(11) = 6
 *            trueFiveEighths(-9) = -5
 *            trueFiveEighths(0x30000000) = 0x1E000000 (no overflow)
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 25
 *  Rating: 4
 */
int trueFiveEighths(int x) {
    return 2;
}
