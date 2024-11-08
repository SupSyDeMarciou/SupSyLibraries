// https://opensource.apple.com/source/gcc/gcc-5026/zlib/contrib/puff/puff.c.auto.html
// Just modified for const (I'm tired of this compiler...)

/*
 * Inflate source to dest.  On return, destlen and sourcelen are updated to the
 * size of the uncompressed data and the size of the deflate data respectively.
 * On success, the return value of puff() is zero.  If there is an error in the
 * source data, i.e. it is not in the deflate format, then a negative value is
 * returned.  If there is not enough input available or there is not enough
 * output space, then a positive error is returned.  In that case, destlen and
 * sourcelen are not updated to facilitate retrying from the beginning with the
 * provision of more input data or more output space.  In the case of invalid
 * inflate data (a negative error), the dest and source pointers are updated to
 * facilitate the debugging of deflators.
 *
 * puff() also has a mode to determine the size of the uncompressed output with
 * no output written.  For this dest must be (unsigned char *)0.  In this case,
 * the input value of *destlen is ignored, and on return *destlen is set to the
 * size of the uncompressed output.
 *
 * The return codes are:
 *
 *   2:  available inflate data did not terminate
 *   1:  output space exhausted before completing inflate
 *   0:  successful inflate
 *  -1:  invalid block type (type == 3)
 *  -2:  stored block length did not match one's complement
 *  -3:  dynamic block code description: too many length or distance codes
 *  -4:  dynamic block code description: code lengths codes incomplete
 *  -5:  dynamic block code description: repeat lengths with no first length
 *  -6:  dynamic block code description: repeat more than specified lengths
 *  -7:  dynamic block code description: invalid literal/length code lengths
 *  -8:  dynamic block code description: invalid distance code lengths
 *  -9:  invalid literal/length or distance code in fixed or dynamic block
 * -10:  distance is too far back in fixed or dynamic block
 *
 * Format notes:
 *
 * - Three bits are read for each block to determine the kind of block and
 *   whether or not it is the last block.  Then the block is decoded and the
 *   process repeated if it was not the last block.
 *
 * - The leftover bits in the last byte of the deflate data after the last
 *   block (if it was a fixed or dynamic block) are undefined and have no
 *   expected values to check.
 */

// int puff_(unsigned char *dest,           /* pointer to destination pointer */
//          unsigned long *destlen,        /* amount of output space */
//          const unsigned char *source,         /* pointer to source data pointer */
//          unsigned long *sourcelen);      /* amount of input available */

// #ifndef SGE_DONT_USE_PNG
// #   define puff(dest, destlen, source, sourcelen) puff_(dest, destlen, source, sourcelen)
// #else
// #   include "inout.h"
// #   define puff(dest, destlen, source, sourcelen) failWithError("PNG format has been deactivated!");
// #endif

#   include "inout.h"
int puff(unsigned char *dest, unsigned long *destlen, const unsigned char *source, unsigned long *sourcelen) { failWithError("PNG format has been deactivated!"); return 0; }