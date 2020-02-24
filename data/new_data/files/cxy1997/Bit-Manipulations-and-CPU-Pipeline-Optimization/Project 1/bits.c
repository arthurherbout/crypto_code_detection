/* 
 *Lab of Computer Architecture 
 * 
 * Name: Xiangyu Chen, Student ID: 515030910339
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
/* 
 * bitAnd - x&y using only ~ and | 
 *   Example: bitAnd(6, 5) = 4
 *   Legal ops: ~ |
 *   Max ops: 8
 *   Rating: 1
 */
int bitAnd(int x, int y) {
  /* Since ~(x & y) = ~x | ~y, x & y = ~(~(x & y)) = ~(~x | ~y)*/
  return ~(~x | ~y);
}
/* 
 * getByte - Extract byte n from word x
 *   Bytes numbered from 0 (LSB) to 3 (MSB)
 *   Examples: getByte(0x12345678,1) = 0x56
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 */
int getByte(int x, int n) {
  /* The required byte is shift to LSB by (8 * n) bits,
     the calculation of 8 * n is implemented by n << 3,
     0xFF (=0b11111111) serves as a mask to select the least significant byte
     using the & operator. 
  */
  int shift = n << 3; // = n*8
  int mask = 0xFF;
  return (x >> shift) & mask;
}
/* 
 * logicalShift - shift x to the right by n, using a logical shift
 *   Can assume that 0 <= n <= 31
 *   Examples: logicalShift(0x87654321,4) = 0x08765432
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3 
 */
int logicalShift(int x, int n) {
  /* While x is shifted to right by n bits, the most significant bits are
     automatically filled with the MSB of x. In order to assign these bits
     with value 0, a mask 000(n * '0')...111( (32-n) * '1') is used to select
     only the last (32-n) bits. 
  */
  int shifted = x >> n;
  int mask = ~(1 << 31 >> n << 1);
  return shifted & mask;
}
/*
 * bitCount - returns count of number of 1's in word
 *   Examples: bitCount(5) = 2, bitCount(7) = 3
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 40
 *   Rating: 4
 */
int bitCount(int x) {
  /* Consider integer x as bits[31:0].

     Firstly, bits[] is divided into 16 2-bit subarrays and they are added
     separately in the following format:
       bits[1:0] = bits[0] + bits[1]
       bits[3:2] = bits[2] + bits[3]
       ...
       bits[31:30] = bits[30] + bits[31]

     Now we have 16 2-bit integers.
     Secondly, these 16 integers are divided and added again, generating
     8 4-bit integers:
       bits[3:0] = bits[1:0] + bits[3:2]
       ...
       bits[31:28] = bits[29:28] + bits[31:30]

     This process is conducted over and over, turing the 8 4*bit integers into
     4 8-bit integers, and then into 2 16-bit integers, and at last into a
     32-bit integer represent the number of 1's in the word x. 
  */
  int mask = 0x55;
  mask = (mask << 8) | mask;
  mask = (mask << 16) | mask;
  // mask = 0b010101..01
  x = (x & mask) + ((x >> 1) & mask);
  
  mask = 0x33;
  mask = (mask << 8) | mask;
  mask = (mask << 16) | mask;
  // mask = 0b00110011..0011
  x = (x & mask) + ((x >> 2) & mask);
  
  mask = 0x0F;
  mask = (mask << 8) | mask;
  mask = (mask << 16) | mask;
  // mask = 0b00001111..00001111
  x = (x & mask) + ((x >> 4) & mask);
  
  mask = (0x00 << 8) | 0xFF;
  mask = (mask << 16) | mask;
  // mask = 0b00000000111111110000000011111111
  x = (x & mask) + ((x >> 8) & mask);
  
  mask = (0xFF << 8) | 0xFF;
  // mask = 0b00000000000000001111111111111111
  x = (x & mask) + ((x >> 16) & mask);
  return x;
}
/* 
 * bang - Compute !x without using !
 *   Examples: bang(3) = 0, bang(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int bang(int x) {
  /* -x = ~x + 1, the MSB of x and -x are both 0 iff x==0. 

     By doing nor operation to these 2 MSBs, we can determine whether x==0.
  */
  int negx = ~x + 1;
  return ~((x | negx) >> 31) & 1;
}
/* 
 * tmin - return minimum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void) {
  /* -2147483648 = 0b1000...000 (31 * '0'in total) */
  return 1 << 31;
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
  /* Consider x as bits[31:0]

     fitBits(x, n)=1 means bits[i] = bits[31], n <= i < 31

     Firstly, the (n-1) least significant bits are ignored by shifting.

     Then every remaining bit is campared with the most significant bit
     using xor opertion.
  */
  int shift = n + ~0; // = n-1
  return !((x >> shift) ^ (x >> 31));
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
  /* When x >= 0, the result is simply x >> n.
     When x < 0, a bias of 2^n - 1 helps to round the result toward zero.
  */
  int bias = (x >> 31) & ((1 << n) + ~0); // bias = (x<0) ? (2^n - 1) : 0
  return (x + bias) >> n;
}
/* 
 * negate - return -x 
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) {
   /* -x = ~x + 1 */
  return ~x + 1;
}
/* 
 * isPositive - return 1 if x > 0, return 0 otherwise 
 *   Example: isPositive(-1) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 3
 */
int isPositive(int x) {
  /* If x is not positive, then either x or x-1 is negative. 

     By checking the MSB of both x and x-1, it can be determined
     whether x is positive.
  */
  return !((x | (x + ~0)) >> 31);
}
/* 
 * isLessOrEqual - if x <= y  then return 1, else return 0 
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y) {
  /* If the sign bit of x and y are same, we can simply calculate x-y-1
     with the equation x-y-1 = x + ~y, and x-y-1 < 0 means x <= y.

     If the sign bit of x and y are different, there can be a chance of
     integer overflow. In this case, we only have to check the sign bits
     of x and y, x<=y iff x < 0 and y >= 0.
  */
  int valueDifference = (x + ~y) & (~(x ^ y) >> 31); 
  /* valueDifference = x-y-1, x and y have same signs
                       0, x and y have different signs */
  int signDifference = (x & ~y) >> 31; /* = 0xFFFFFF, x<0 and y>=0
                                            0       , other conditions */
  return ((valueDifference >> 31) | signDifference) & 1;
}
/*
 * ilog2 - return floor(log base 2 of x), where x > 0
 *   Example: ilog2(16) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 90
 *   Rating: 4
 */
int ilog2(int x) {
  /* Beiefly speaking, the goal is to determine the highest bit with value 1.
     Since this is a logarithmic operation, the MSB of x should be 0.
     Here the 'Divide & Conquer' technique is applied.
     Consider x as bits[31:0].
     Consider the result as a 5-bit integer, defined as res[4:0].

     res[4] = 1 iff bits[31:16] > 0
     If res[4]=1, we should then determine res[3] with bits[31:16].
     Otherwise, we should determine res[3] with bits[15:0].

     The operation above can be recursively done to determine each bit of the
     result.
  */
  int res0, res1, res2, res3, res4;
  res4 = !!(x >> 16);
  x = x >> (res4 << 4);
  res3 = !!(x >> 8);
  x = x >> (res3 << 3);
  res2 = !!(x >> 4);
  x = x >> (res2 << 2);
  res1 = !!(x >> 2);
  x = x >> (res1 << 1);
  res0 = !!(x >> 1);
  return (res4 << 4) | (res3 << 3) | (res2 << 2) | (res1 << 1) | res0;
}
/* 
 * float_neg - Return bit-level equivalent of expression -f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 10
 *   Rating: 2
 */
unsigned float_neg(unsigned uf) {
  /* NAN is defined to be floating numbers with an exponent of 255
     and a non-zero mantissa.

     The negation operation can be simply done by changing the sign bit.
  */
  const unsigned signMask = 0x80000000;
  const unsigned exponentMask = 0x7f800000;
  const unsigned mantissaMask = 0x007fffff;
  unsigned exponent = uf & exponentMask; // extract the exponent
  unsigned mantissa = uf & mantissaMask; // extract the mantissa
  if ((exponent ^ exponentMask) || !mantissa) // if uf is not NAN
    uf = uf ^ signMask;  // reverse the sign bit
  return uf;
}
/* 
 * float_i2f - Return bit-level equivalent of expression (float) x
 *   Result is returned as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point values.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_i2f(int x) {
  /* In order to turn the signed integer into a floating point number, 
     we first need to determine its sign as well as the position of its
     highest bit with value 1, which is recorded in the variable 'highest'.

     Then we calculate the exponent with the known position, and rightshift
     the integer to fit the format of mantissa. 

     It should be noted that an additional rounding is compulsory here
     based on the bits lost in rightshifting.
  */
  const unsigned signMask = 0x80000000;
  const unsigned mantissaMask = 0x007fffff;
  int highest = 31; // Record the highest bit with value 1
  unsigned sign = x & signMask; // Record the sign
  int X;
  if (!x) return 0; // Exception handling
  if (sign)
    x = -x; // x is positive now
  while (!(x & signMask) && highest > 0) {
    highest = highest - 1;
    x = x << 1;
  } // Find the the highest bit with value 1 by leftshifting
  X = x; // Preserved for rounding 
  x = (x >> 8) & mantissaMask; // Calculate the mantissa
  if (((X & 0xff) > 0x80) || ((X & 0x180) == 0x180)) x = x + 1; // Rounding
  return x + sign + ((highest + 127) << 23); // exponent = highest + 127
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
  /* There are 3 cases to be dealt with different approaches:
     #1: NaN, feature: exponent=255
       In this case, just return the argument.
     #2: small number, feature: exponent=0
       In this case, we should multiple the mantissa by 2, and preserve the
       exponent. (carry should be considered)
     #3: other floating point numbers
       In this case, we can simply add 1 to the exponent.

     Finally, these segements are assembled, representing the result.
  */

  // define masks 
  const unsigned signMask = 0x80000000;
  const unsigned exponentMask = 0x7f800000;
  const unsigned mantissaMask = 0x007fffff;

  // segementation
  unsigned sign = uf & signMask;
  unsigned exponent = uf & exponentMask;
  unsigned mantissa = uf & mantissaMask;

  if (exponent == exponentMask) return uf; // deals with NaN
  if (!exponent) // so small that exponent = 0(-127)
    mantissa = mantissa << 1;
  else // exponent plus 1
    exponent = exponent + 0x00800000;
  return sign + exponent + mantissa;
}
