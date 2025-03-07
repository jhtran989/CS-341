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

    // IMPORTANT: all variables must be declared first before referencing
    // them afterwards (i.e. before line 377 where result is referenced)
    int numShifts = n << 3;
    int mask = 0xff;
    int adjustedMask = mask << numShifts;
    int shiftedC = c << numShifts;

    int result = x | adjustedMask;
    result = result ^ adjustedMask;

    return result | shiftedC;
}

/*
 * TMax - return maximum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmax(void) {
    /*
     * we know an int in C has length of 32 bits
     *
     * general approach is to get a bit mask of all 1s and bitwise or it with
     * a bit mask where only the most significant bit (sign bit) is a 1
     *
     * since we know that 0 ^ a = a for any given bit a, we can leave 0s to
     * the right of the right most 1 in the second bit mask to retain the 1s
     * in those positions in the original mask
     *
     * to get the maximum number in two's complement, we just need all 1s
     * except the most significant bit and to zero that bit, we can bitwise
     * or it with 1 at that position to get 0
     */

    int maxBitMask = (~0);
    int leftBitMask = 1 << 31; // only left most bit (most significant bit) has
    // a 1

    int result = maxBitMask ^ leftBitMask;

    return result;
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
    /*
     * we know that C uses arithmetic right shift and an int has a length of 32
     * bits (4 bytes)
     *
     * first, we need to check if there's all 0s or 1s to left of the first n-1
     * bits of x (stored in the leftBits variable)
     *
     * we need to shift n - 1 bits to preserve the sign bit for comparison
     * (need to account for positive numbers where the most significant bit
     * holds the sign, which will be compared with the rest of the bits to
     * the left)
     *
     * for a clever trick, subtracting 1 from x can be achieved by
     * ~0 + x = x - 1
     *
     * the arithmetic right shift will extend the sign bit to fill in the new
     * bits from right shifting, so the only way that x will fit in n bits is
     * if all bits to the left of the first n+1 bits of x are either 0s
     * (positive value) or 1s (negative value)
     *
     * to make sure they all match, we can just bitwise xor the two and take
     * the logical not since the bitwise xor should return 0 if all the bits
     * match (and remembering to take the logical not to get 1 as our final
     * output)
     */

    int leftBits = x >> ((~0) + n); // shift by n - 1 bits
    int signBitMask = x >> 31; // all 1s or all 0s depending on the sign bit
    // of x

    return !(leftBits ^ signBitMask);
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
    /*
     * a condensed algorithm compared to the one in the
     * trueFiveEighths() function shown below
     *
     * remember to round towards zero
     *
     * for positive values of x, it is pretty simple as we just need to shift x
     * by n to divide x by 2^n, taking care of any remainders by removing them
     *
     * however, the negative values of x will need some more work: since
     * shifting x will actually round x down instead of to zero, we just need
     * to add 1 to x to offset the default behavior of rounding down by shifting
     *
     * so, we keep track of all the bits that would have been lost by
     * shifting x by n by creating a mask of all 1s of length n and bitwise
     * and-ing it with x
     *
     * then we just check if the fractional part is nonzero (making sure that
     * this only applies to negative values by bitwise and-ing with the sign
     * bit as negative values have a sign bit of 1)
     *
     * to get our final result, we just add this offset to the original
     * division by shifting
     */

    int signBit = (x >> 31) & 0x01;
    int rawDivision = x >> n;

    int fractionalMask = (1 << n) + (~0);
    int fractionalPart = fractionalMask & x;

    int negativeOffSet = signBit & (!(!fractionalPart));
    int result = rawDivision + negativeOffSet;

//    printf("Inputs - x: %x, n: %d\n", x, n);
//    printf("Result: %x\n\n", result);

    return result;
}

/*
 * isEqual - return 1 if x == y, and 0 otherwise 
 *   Examples: isEqual(5,5) = 1, isEqual(4,5) = 0
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int isEqual(int x, int y) {
    /*
     * we know an int in C has length of 32 bits
     *
     * pretty straightforward, but this general approach was used in the
     * fitsBits() function above -- if all the bits of x and y match, the
     * bitwise xor should return 0, which we could just take logical not to
     * get our final output
     */

    return !(x ^ y);
}

/*
 * isPositive - return 1 if x > 0, return 0 otherwise 
 *   Example: isPositive(-1) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 3
 */
int isPositive(int x) {
    /*
     * we know an int in C has length of 32 bits
     *
     * pretty straightforward, but we need to check the sign bit of x (if
     * it's 0 for positive values) and whether x is not zero (applying two
     * logical nots to get 1 if x is nonzero and 0 if x is zero), bitwise
     * and-ing the two conditions together
     */

    int signBit = (x >> 31) & 0x01;

    return !signBit & !(!x);
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
    /*
     * we know an int in C has length of 32 bits
     *
     * the basic approach is to turn the subtraction into 2s complement addition
     *
     * first, we need to recognize that the only way that overflow could
     * occur with subtraction is if x and y have different signs (+ - -
     * or - - +), so we keep track of the sign bits for x and y
     *
     * then we check the condition we just mentioned (different sign bits)
     * and if the resulting 2s complement addition results in a sign change
     * with respect to x (since we're doing x-y)
     *
     * so, we just compare if the two sign bits (for x and the resulting
     * computation) are the same
     *
     * to produce our final output, we can safely output a 1 if the sign bits
     * of x and y are the same or the sign bits are not the same for x and y,
     * but the sign of x and the final subtraction are the same (using the
     * same structure from the isEqual() and fitsBits() functions above)
     *
     * explain subtractSignBit
     */

    int y2complement = (~y) + 1; // 2s complement of y (-> -y)

    int xSignBit = (x >> 31) & 0x01;
    int ySignBit = (y >> 31) & 0x01;

    int signCheck = xSignBit ^ ySignBit;
    int signToMatch = xSignBit;

    int subtract = x + y2complement;
    int subtractSignBit = (subtract >> 31) & 0x01;

//    printf("(!signCheck): %d\n", (!signCheck));
//    printf("subtractSignBit: %d\n", subtractSignBit);
//    printf("signToMatch: %d\n\n", signToMatch);

    return (!signCheck) | (!(subtractSignBit ^ signToMatch));
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
    /*
     * instead of going with a more brute-force approach, we decided to
     * find a way to split our given string x into groups of 2 (as done with
     * a few of the functions above). This time, the approach seemed
     * similar to binary search where each successive split of x into two
     * groups will now depend on whether the left half is nonzero, as will be
     * explained below
     *
     * first, to consolidate both cases where x is positive (including 0) and
     * when x is negative, we decided to make sure to bitwise complement x if
     * x is negative (positiveX variable). This makes sure the same procedure
     * is done for both cases since we can't use any conditionals. To prove
     * this is plausible, we can provide a mapping of the negative values of
     * x with the positive values
     *
     * a simple case when x is three bits long (can be easily generalized to
     * an arbitrary length) -- -4(100) and 3(011), -3(101) and 2 (010),
     * -2(110) and 1(001), and -1(111) and 0(000). Since the interval of
     * possible values [-4, 3] is possible for 3 bits, we can see that the
     * pairings are just the bitwise complements of each other
     *
     * mapping used for the halves of x: 1 in left half, 0 in right half --
     * stored in checkHalf[16, 8, 4, 2] where [16, 8, 4, 2] refers to the
     * variables defined below with the number inside the [] represents the
     * length of the halves we're looking at
     *
     * also, the current half-length is appended after the positiveX variable
     * (positiveX[16, 8, 4, 2] after the respective searches were completed
     * to differentiate the length of x we are now looking at) to avoid having
     * to declare all the variables at the beginning than if we just used
     * positiveX through all the searches -- stricter form of C declarations
     * from dlc
     *
     * to count up all the bits up to the most significant 1 bit in x (for
     * positive cases since we converted the negative values to positive
     * values), we just need to keep track of which half we are looking at
     * with each binary search step. For example, if we find that x is in the
     * left half after the first step (halves of 16 bits), then we add the 16
     * bits to the total and shift x, so we're just looking at that half.
     * However, if we find that x is in the right half instead (so the left
     * half is all 0s), then we don't add anything to the total and continue
     * our search with the right half instead. The half chosen is stored in
     * checkHalf[16, 8, 4, 2] (as mentioned above) and the number of bits we
     * found the most significant 1 bit to contain is the half is stored in
     * the atLeast[16, 8, 4, 2]Bits variable (just stores [16, 8, 4, 2] or 0
     * depending on the current iteration of the search and which half the
     * most significant 1 bit is found in -- stored in checkHalf[16, 8, 4, 2])
     *
     * we need to add 1 at the end since we made sure to convert x to a
     * positive form (if needed when x is negative) and an extra bit is
     * needed to carry the sign bit (e.g., for the case of 7 -- 111 -- we
     * need four bits instead of 3 to store it as a signed int -> 0111)
     *
     * the finalBit variable is for checking if our last bit is a 1 (no more
     * cutting our current sequence of x in halves and occurs when the most
     * significant 1 bit is in the 4th bit position (3rd index) in the most
     * significant byte position of x) -- e.g., 1001 -> 10 -> 1, or when the
     * most significant byte of x is at least 0x8
     */

    int signBitMask = x >> 31; // all 0s or all 1s depending on sign of x
    int positiveX = (x & (~signBitMask)) | ((~x) & signBitMask);

    int checkHalf16 = !(!(positiveX >> 16));
    int atLeast16Bits = checkHalf16 << 4;
    int positiveX16 = positiveX >> atLeast16Bits;

    int checkHalf8 = !(!(positiveX16 >> 8));
    int atLeast8Bits = checkHalf8 << 3;
    int positiveX8 = positiveX16 >> atLeast8Bits;

    int checkHalf4 = !(!(positiveX8 >> 4));
    int atLeast4Bits = checkHalf4 << 2;
    int positiveX4 = positiveX8 >> atLeast4Bits;

    int checkHalf2 = !(!(positiveX4 >> 2));
    int atLeast2Bits = checkHalf2 << 1;
    int positiveX2 = positiveX4 >> atLeast2Bits;

    int checkHalf1 = !(!(positiveX2 >> 1));
    int atLeast1Bits = checkHalf1 << 0;
    int positiveX1 = positiveX2 >> atLeast1Bits;

    int finalBit = positiveX1 & 0x01;

    int result = atLeast16Bits + atLeast8Bits + atLeast4Bits + atLeast2Bits
            + atLeast1Bits + finalBit + 1;

    return result;
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
    /*
     * we know an int in C has length of 32 bits
     *
     * we can make use of the fact that the sign bit is the most significant
     * bit of uf
     *
     * the biggest challenge for this puzzle was to fit everything within the
     * max ops. -- the value for the exponent bit mask was hardcoded (since the
     * exponent part is 8 bits long, enough to fit in one byte)
     *
     * the actual procedure to take the absolute value of uf is pretty
     * straightforward (just turn the sign bit to zero, which can easily be
     * done by creating a bit mask with all 1s except the sign bit and
     * bitwise and-ing it with uf)
     *
     * however, we need to check if uf is NaN (just return the input if so,
     * as described in the original description of the function above)
     *
     * so, we need to check both the exponent part and the fraction part of
     * uf before finding out the absolute value of uf
     *
     * as discussed above, we can use our exponent bit mask to find the
     * exponent part by shifting uf so the least significant bit of uf
     * corresponds with the least significant bit of the exponent and bitwise
     * and-ing it with the mask (easier comparison with the exponent mask
     * since the exponent part is already shifted to the same positions as
     * the mask)
     *
     * then since we only need to make sure the fraction part is nonzero, we
     * can condense trying to find the exact fractional part by just shifting
     * uf to the left by 9 (8 + 1 for the exponent part and the sign bit,
     * respectively) since the newly added bits are zeroes, leaving only the
     * fraction bits for comparison
     *
     * if both the exponent part are all 1s and the fraction part is nonzero
     * (definition of NaN), then we can return early with the argument (as
     * instructed above)
     */

    int fractionLength = 23;

    // hardcode ((0x1 << exponentLength) - 1) to 0xff (since the exponent
    // part is 8 bits)
    int exponentBitMask = 0xff;
    unsigned int exponentPart = (uf >> fractionLength) & exponentBitMask;

    // change (32 - fractionLength) to 9 -- reduce ops. count by 1
    unsigned int fractionCondition = uf << 9;

    // moved declarations before if statement
    unsigned int leftBitMask;
    unsigned int rightBitMask;

    if ((exponentPart == exponentBitMask)
        && fractionCondition != 0) {
        return uf;
    }

    leftBitMask = 1 << 31; // changed to hardcode 0x800000000
    rightBitMask = ~leftBitMask; // a bit mask of all 1s except the most
    // significant bit

//    printf("float input: %x\n", uf);
//    printf("output absolute value: %x\n\n", uf & rightBitMask);

    return uf & rightBitMask;
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
    /*
     * a similar approach to the float_abs() function above, but this time,
     * we need to also account for the infinities in our early conditional
     * return (just checking if the exponent part is all 1s for our special
     * cases of the infinities and NaN)
     *
     * after that, we need to preserve the sign bit for our final output
     * since we'll be manipulating the rest of the bits depending on a few
     * conditionals
     *
     * if the exponent part is not zero, then we can multiply uf by just
     * adding one to the current exponent (making sure to zero out the
     * exponent part of uf and bitwise or-ing it with uf since the values of
     * the exponent bits could change drastically with the addition of 1)
     *
     * however, if a denormalized value of uf is encountered (exponent part
     * is zero), then we have to multiply 2 to fraction part instead (for the
     * case where a 1 is in the most significant bit of the fraction part,
     * the shift to the left by 1 would be fine since it would cross into the
     * exponent part, taking care of the multiplication)
     *
     * this was kept tracked of by a flag and shifting uf only if uf wasn't
     * already 0 (since 0 times anything is still 0)
     *
     * to get the final result, we just need to bitwise or the sign bit back
     * into our previous result (zeroing the sign bit of this previous result
     * beforehand)
     */

    unsigned fractionLength = 23;

    // hardcode to 0xff
    unsigned exponentBitMask = 0xff;
    unsigned shiftedExponentBitMask = exponentBitMask << fractionLength;
    unsigned exponentPart = uf & shiftedExponentBitMask;

    // change 32 - fractionLength = 9
    unsigned fractionCondition = uf << 9;

    // moved declarations before if statement
    unsigned int leftBitMask;
    unsigned int rightBitMask;
    unsigned int signBit;
    unsigned int newExponent = 0;
    unsigned int rawMultiply2 = uf;
    unsigned int result;
    unsigned int zeroExponentFlag = 0x00;

//    printf("current exponent: %x\n", exponentPart >> fractionLength);
//    printf("fractional condition: %x\n", fractionCondition);

    // remove fractional condition -- should also apply to +- infinity
    if (((exponentPart >> fractionLength) == exponentBitMask)) {
        //printf("current exponent: %x\n", exponentPart >> fractionLength);
        return uf;
    }

    leftBitMask = 1 << 31;
    rightBitMask = ~leftBitMask; // a bit mask of all 1s except the most
    // significant bit

    signBit = leftBitMask & uf;

//    printf("sign bit: %d\n", !!signBit);
//    printf("exponent part: %x\n", exponentPart);

    if (exponentPart != 0) {
        newExponent = exponentPart + (1 << fractionLength);
    } else {
        zeroExponentFlag = 0x01;
    }

    rawMultiply2 = rawMultiply2 | shiftedExponentBitMask;
    rawMultiply2 = rawMultiply2 ^ shiftedExponentBitMask;

    rawMultiply2 = rawMultiply2 | newExponent; // just add 1 to
    // the exponent (base 2)

    // has to be done AFTER the exponent has been updated
    if (zeroExponentFlag && fractionCondition != 0) {
        rawMultiply2 = rawMultiply2 << 1;
    }

//    printf("new exponent: %x\n", newExponent);
//    printf("raw multiply: %x\n", rawMultiply2);

    result = (rawMultiply2 & rightBitMask) | signBit;

    return result;
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
    /*
     * we can recognize that we can split 5/8 into 1/2 + 1/8 (fractional powers
     * of 2)
     *
     * this is a similar approach to the divpwr2() function above, but
     * there's a possibility that remainder could add up to an extra 1
     *
     * the negative correction is also a little more complicated this time
     * (described below in the code)
     *
     * example case where a 1 occurs in both cases for the 2^-1 place:
     * half: *.1
     * eighth: *.111
     *
     * if halves add together to result in extra addition of 1
     *
     * the most important thing we need to consider are ALL the bit that are
     * part of the remainder when dividing by 8 (actually, not really as
     * displayed in the example above)
     *
     * for half, need to keep track of the least significant bit
     *
     * for eighth, need to keep track of the last three least significant bits
     */

    int xHalf = x >> 1;
    int xEighth = x >> 3;

    int carryHalf = x & 0x01; // bit representation of 0000 0001
    //int carryEighth = (x >> 2) & 0x01;
    int carryEighth = x & 0x07; // bit representation of 0000 0111

    int negativeCorrection = (x >> 31) & 0x07; // 0 for positive values and
    // 111 (7) for negative values (shifting along rounds down values,
    // including negative values, so it is corrected by added 1 if x is not
    // evenly divisible by 8 -- remedied by adding 7/8 instead of 1, which
    // after the shift, results in 0 if either the half or eighth are nonzero)

//    printf("half: %x\n", xHalf);
//    printf("eighth: %x\n", xEighth);
//    printf("half carry: %d\n", carryHalf);
//    printf("eighth carry: %d\n", carryEighth);

    // the carries for the half and the eighth (along with the negative
    // correction) are added in terms of eighths first before converting back
    // to its original numerical value
    return xHalf + xEighth + (((carryHalf << 2) + carryEighth +
        negativeCorrection) >> 3);
}
