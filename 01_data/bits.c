/* 
 * CS:APP Data Lab 
 * 
 * <abb00717>
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
  3. Has unpredictable behavior when shifting if the shift amount
     is less than 0 or greater than 31.


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

For the problems that require you to implement floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants. You can use any arithmetic,
logical, or comparison operations on int or unsigned data.

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
  2. Each function has a maximum number of operations (integer, logical,
     or comparison) that you are allowed to use for your implementation
     of the function.  The max operator count is checked by dlc.
     Note that assignment ('=') is not counted; you may use as many of
     these as you want without penalty.
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
//1
/* 
 * bitXor - x^y using only ~ and & 
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 14
 *   Rating: 1
 */
int bitXor(int x, int y) {
    return (~(~x & ~y)) & ~(x & y);
}
/* 
 * tmin - return minimum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void) {
    return 1 << 31;
}
//2
/*
 * isTmax - returns 1 if x is the maximum, two's complement number,
 *     and 0 otherwise 
 *   Legal ops: ! ~ & ^ | +
 *   Max ops: 10
 *   Rating: 1
 */
int isTmax(int x) {
    // (INT_MAX || -1) && (x != -1) 
    // x == y => !(x ^ y)
    // For INT_MAX && -1, x + 1 == ~x
    return !((x + 1) ^ ~x) & !!(x + 1);
}
/* 
 * allOddBits - return 1 if all odd-numbered bits in word set to 1
 *   where bits are numbered from 0 (least significant) to 31 (most significant)
 *   Examples allOddBits(0xFFFFFFFD) = 0, allOddBits(0xAAAAAAAA) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allOddBits(int x) {
    int mask = 0xAA;
    mask = mask << 8 | mask;
    mask = mask << 16 | mask;
    return !((x & mask) ^ mask);
}
/* 
 * negate - return -x 
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) {
    return ~x + 1;
}
//3
/* 
 * isAsciiDigit - return 1 if 0x30 <= x <= 0x39 (ASCII codes for characters '0' to '9')
 *   Example: isAsciiDigit(0x35) = 1.
 *            isAsciiDigit(0x3a) = 0.
 *            isAsciiDigit(0x05) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 3
 */
int isAsciiDigit(int x) {
    return (!(((x >> 4) << 4 ) ^ 48)) & ((!(x & 4) & !(x & 2)) | !(x & 8));
}
/* 
 * conditional - same as x ? y : z 
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int conditional(int x, int y, int z) {
    int bit = !(x);
    bit = (bit << 31) >> 31;

    return (bit & z) | (~bit & y);
}
/* 
 * isLessOrEqual - if x <= y  then return 1, else return 0 
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y) {
    int x_sign = (x >> 31);
    int y_sign = (y >> 31);
    int same_sign = !(x_sign ^ y_sign);
    // If y - x is positive, the result_sign would be 0, or it would be 0xfff...
    int result_sign = (y + (~x + 1)) >> 31;

    // if (same_sign)
    //     // You don't have to worry about overflow if it's same_sign
    //     return result_sign == 0;
    // else
    //     return y_sign == 0 && x_sign == 1;
    return (!same_sign & !y_sign & x_sign) | (same_sign & !result_sign);
}
//4
/* 
 * logicalNeg - implement the ! operator, using all of 
 *              the legal operators except !
 *   Examples: logicalNeg(3) = 0, logicalNeg(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int logicalNeg(int x) {
    // negative: x >> 31 == 1;
    // positive: ~x + 1 >> 31 == 1;
    // 0:        ~x + 1 >> 31 == 0;
    int negative = x >> 31;
    int positive = (~x + 1) >> 31;
    
    int bit = negative | positive;
    return (~bit & 1);
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
    // Let's say we have x = 000000000010010010111...
    // We want to find out how many bits we really need to represent it, x => 010010010...
    //
    // 1. Trim the useless 0
    // 2. Add the sign bit back to the front
    //
    // the final answer would be bits + 2;
    // the +2 is for the sign bit and that first 1 we found

    int bits, check, shift_amount, special_case;
    // Make this negative a positive so we can treat 'em all the same.
    x = (x ^ (x >> 31));

    bits = 0;
    check = !!(x >> 16); // If there's any 1 on the left, check_16 would be 1
                         // which means we nead the bits extend to at least 16.
    shift_amount = check << 4;
    bits = bits + shift_amount; // 2^4 or 0^4
    x = x >> shift_amount;

    check = !!(x >> 8);
    shift_amount = check << 3;
    bits = bits + shift_amount;
    x = x >> shift_amount;

    check = !!(x >> 4);
    shift_amount = check << 2;
    bits = bits + shift_amount;
    x = x >> shift_amount;

    check = !!(x >> 2);
    shift_amount = check << 1;
    bits = bits + shift_amount;
    x = x >> shift_amount;

    check = !!(x >> 1);
    shift_amount = check;
    bits = bits + shift_amount;

    // if bits and x are equal to 0, it means there's a special case 0 & 1
    // they all need only 1 bit.
    // return (bits | x) == 0 ? 1 : bits + 2;
    // return (bits + 2) - (!(bits | x));
    special_case = ~(!(bits | x)) + 1;
    return (special_case & 1) | ((~special_case) & (bits + 2));
}
//float
/* 
 * floatScale2 - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned floatScale2(unsigned uf) {
    // * 2 == << 1
    // 101.0011100 => 1010.011100
    
    int sign = uf & 0x80000000;
    int exp = uf & 0x7F800000;
    int frac = uf & 0x7FFFFF;

    if (exp == 0x7F800000) {
        return uf;
    }

    if (exp == 0) {
        frac = frac << 1;
        return sign | frac;
    }

    exp += 0x800000;
    return sign | exp | frac;
}
/* 
 * floatFloat2Int - Return bit-level equivalent of expression (int) f
 *   for floating point argument f.
 *   Argument is passed as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point value.
 *   Anything out of range (including NaN and infinity) should return
 *   0x80000000u.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
int floatFloat2Int(unsigned uf) {
    int result;

    int sign = uf & 0x80000000;
    int exp = uf & 0x7F800000;
    int frac = uf & 0x7FFFFF;
    exp = exp >> 23;

    // Nan/Inf/Overflow
    // -2^31 <= int <= 2^31 - 1
    // Legal Bound: exp - 127 <= 30
    // => exp > 157 == Illegal
    if (exp > 0x9d) {
        return 0x80000000u;
    }

    // Under 0
    if (exp < 127) {
        return 0;
    }

    // Normalized
    result = 1 << 23;
    result += frac;
    result = result << (exp - 127);
    result = result >> 23;

    if (sign == 0x0) {
        return result;
    }

    return (~result) + 1;
}
/* 
 * floatPower2 - Return bit-level equivalent of the expression 2.0^x
 *   (2.0 raised to the power x) for any 32-bit integer x.
 *
 *   The unsigned value that is returned should have the identical bit
 *   representation as the single-precision floating-point number 2.0^x.
 *   If the result is too small to be represented as a denorm, return
 *   0. If too large, return +INF.
 * 
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. Also if, while 
 *   Max ops: 30 
 *   Rating: 4
 */
unsigned floatPower2(int x) {
    int e;

    if (x >= 128) {
        // INF in float bit pattern 
        return 0x7F800000;
    }

    // -127 - 23 = -150
    // any value below 1/(2^150) would be 0
    if (x <= -150) {
        return 0;
    }

    e = x + 127;
    e = e << 23;

    return e;
}
