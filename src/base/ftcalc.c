/****************************************************************************
 *
 * ftcalc.c
 *
 *   Arithmetic computations (body).
 *
 * Copyright (C) 1996-2026 by
 * David Turner, Robert Wilhelm, and Werner Lemberg.
 *
 * This file is part of the FreeType project, and may only be used,
 * modified, and distributed under the terms of the FreeType project
 * license, LICENSE.TXT.  By continuing to use, modify, or distribute
 * this file you indicate that you have read the license and
 * understand and accept it fully.
 *
 */

  /**************************************************************************
   *
   * Support for 1-complement arithmetic has been totally dropped in this
   * release.  You can still write your own code if you need it.
   *
   */

  /**************************************************************************
   *
   * Implementing basic computation routines.
   *
   * FT_MulDiv(), FT_MulFix(), FT_DivFix(), FT_RoundFix(), FT_CeilFix(),
   * and FT_FloorFix() are declared in freetype.h.
   *
   */


#ضمن <freetype/ftglyph.h>
#ضمن <freetype/fttrigon.h>
#ضمن <freetype/internal/ftcalc.h>
#ضمن <freetype/internal/ftdebug.h>
#ضمن <freetype/internal/ftobjs.h>

  /* cancel inlining macro from internal/ftcalc.h */
#اگر_تعریف_شده FT_MulFix
#  undef FT_MulFix
#پایان_اگر


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#لغو_تعریف  FT_COMPONENT
#تعریف_کن FT_COMPONENT  calc


  /* transfer sign, leaving a positive number;                        */
  /* we need an unsigned value to safely negate INT_MIN (or LONG_MIN) */
#تعریف_کن FT_MOVE_SIGN( utype, x, x_unsigned, s ) \
  FT_BEGIN_STMNT                                \
    إذا ( x < 0 )                                \
    {                                           \
      x_unsigned = 0U - (utype)x;               \
      s          = -s;                          \
    }                                           \
    وإلا                                        \
      x_unsigned = (utype)x;                    \
  FT_انتها_STMNT

  /* The following three functions are available regardless of whether */
  /* FT_INT64 is defined.                                              */

  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Fixed )
  FT_دور_Fix( FT_Fixed  a )
  {
    أرجع جمع_کن_LONG( a, 0x8000L - ( a < 0 ) ) & ~0xFFFFL;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Fixed )
  FT_CeilFix( FT_Fixed  a )
  {
    أرجع جمع_کن_LONG( a, 0xFFFFL ) & ~0xFFFFL;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Fixed )
  FT_FloorFix( FT_Fixed  a )
  {
    أرجع a & ~0xFFFFL;
  }

#اگر_تعریف_نشده FT_MSB

  FT_پایه_DEF( FT_Int )
  FT_MSB( FT_UInt32 z )
  {
    FT_Int  لغزش = 0;


    /* determine msb bit index in `shift' */
    إذا ( z & 0xFFFF0000UL )
    {
      z     >>= 16;
      لغزش  += 16;
    }
    إذا ( z & 0x0000FF00UL )
    {
      z     >>= 8;
      لغزش  += 8;
    }
    إذا ( z & 0x000000F0UL )
    {
      z     >>= 4;
      لغزش  += 4;
    }
    إذا ( z & 0x0000000CUL )
    {
      z     >>= 2;
      لغزش  += 2;
    }
    إذا ( z & 0x00000002UL )
    {
   /* z     >>= 1; */
      لغزش  += 1;
    }

    أرجع لغزش;
  }

#پایان_اگر /* !FT_MSB */


  /* documentation is in ftcalc.h */

  FT_پایه_DEF( FT_Fixed )
  FT_Hypot( FT_Fixed  x,
            FT_Fixed  y )
  {
    FT_وکتور  v;


    v.x = x;
    v.y = y;

    أرجع FT_وکتور_درازی( &v );
  }


#اگر_تعریف_شده FT_INT64


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Long )
  FT_MulDiv( FT_Long  a_,
             FT_Long  b_,
             FT_Long  c_ )
  {
    FT_Long    s = 1;
    FT_UInt64  a, b, c, d;


    FT_MOVE_SIGN( FT_UInt64, a_, a, s );
    FT_MOVE_SIGN( FT_UInt64, b_, b, s );
    FT_MOVE_SIGN( FT_UInt64, c_, c, s );

    d = c > 0 ? ( a * b + ( c >> 1 ) ) / c
              : 0x7FFFFFFFUL;

    أرجع MUL_LONG( s, d );
  }


  /* documentation is in ftcalc.h */

  FT_پایه_DEF( FT_Long )
  FT_Mul_Div_No_دور( FT_Long  a_,
                      FT_Long  b_,
                      FT_Long  c_ )
  {
    FT_Long    s = 1;
    FT_UInt64  a, b, c, d;


    FT_MOVE_SIGN( FT_UInt64, a_, a, s );
    FT_MOVE_SIGN( FT_UInt64, b_, b, s );
    FT_MOVE_SIGN( FT_UInt64, c_, c, s );

    d = c > 0 ? a * b / c
              : 0x7FFFFFFFUL;

    أرجع MUL_LONG( s, d );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Long )
  FT_MulFix( FT_Long  a_,
             FT_Long  b_ )
  {
#اگر_تعریف_شده FT_CONFIG_OPTION_INLINE_MULFIX

    أرجع FT_MulFix_64( a_, b_ );

#وإلا

    FT_Int64  ab = MUL_INT64( a_, b_ );

    /* this requires arithmetic right shift of signed numbers */
    أرجع (FT_Long)( ( ab + 0x8000L + ( ab >> 63 ) ) >> 16 );

#پایان_اگر /* FT_CONFIG_OPTION_INLINE_MULFIX */
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Long )
  FT_DivFix( FT_Long  a_,
             FT_Long  b_ )
  {
    FT_Long    s = 1;
    FT_UInt64  a, b, q;


    FT_MOVE_SIGN( FT_UInt64, a_, a, s );
    FT_MOVE_SIGN( FT_UInt64, b_, b, s );

    q = b > 0 ? ( ( a << 16 ) + ( b >> 1 ) ) / b
              : 0x7FFFFFFFUL;

    أرجع MUL_LONG( s, q );
  }


#وإلا /* !FT_INT64 */


  ساكن فراغ
  ft_multo64( FT_UInt32  x,
              FT_UInt32  y,
              FT_Int64  *z )
  {
    FT_UInt32  lo1, hi1, lo2, hi2, lo, hi, i1, i2;


    lo1 = x & 0x0000FFFFU;  hi1 = x >> 16;
    lo2 = y & 0x0000FFFFU;  hi2 = y >> 16;

    lo = lo1 * lo2;
    i1 = lo1 * hi2;
    i2 = lo2 * hi1;
    hi = hi1 * hi2;

    /* Check carry overflow of i1 + i2 */
    i1 += i2;
    hi += (FT_UInt32)( i1 < i2 ) << 16;

    hi += i1 >> 16;
    i1  = i1 << 16;

    /* Check carry overflow of i1 + lo */
    lo += i1;
    hi += ( lo < i1 );

    z->lo = lo;
    z->hi = hi;
  }


  ساكن FT_UInt32
  ft_div64by32( FT_UInt32  hi,
                FT_UInt32  lo,
                FT_UInt32  y )
  {
    FT_UInt32  r, q;
    FT_Int     i;


    إذا ( hi >= y )
      أرجع (FT_UInt32)0x7FFFFFFFL;

    /* We shift as many bits as we can into the high register, perform     */
    /* 32-bit division with modulo there, then work through the remaining  */
    /* bits with long division. This optimization is especially noticeable */
    /* for smaller dividends that barely use the high register.            */

    i = 31 - FT_MSB( hi );
    r = ( hi << i ) | ( lo >> ( 32 - i ) ); lo <<= i; /* left 64-bit shift */
    q = r / y;
    r -= q * y;   /* remainder */

    i = 32 - i;   /* bits remaining in low register */
    افعل
    {
      q <<= 1;
      r   = ( r << 1 ) | ( lo >> 31 ); lo <<= 1;

      إذا ( r >= y )
      {
        r -= y;
        q |= 1;
      }
    } طالما ( --i );

    أرجع q;
  }


  ساكن فراغ
  FT_Add64( FT_Int64*  x,
            FT_Int64*  y,
            FT_Int64  *z )
  {
    FT_UInt32  lo, hi;


    lo = x->lo + y->lo;
    hi = x->hi + y->hi + ( lo < x->lo );

    z->lo = lo;
    z->hi = hi;
  }


  /*  The FT_MulDiv function has been optimized thanks to ideas from     */
  /*  Graham Asher and Alexei Podtelezhnikov.  The trick is to optimize  */
  /*  a rather common case when everything fits within 32-bits.          */
  /*                                                                     */
  /*  We compute 'a*b+c/2', then divide it by 'c' (all positive values). */
  /*                                                                     */
  /*  The product of two positive numbers never exceeds the square of    */
  /*  its mean values.  Therefore, we always avoid the overflow by       */
  /*  imposing                                                           */
  /*                                                                     */
  /*    (a + b) / 2 <= sqrt(X - c/2)    ,                                */
  /*                                                                     */
  /*  where X = 2^32 - 1, the maximum unsigned 32-bit value, and using   */
  /*  unsigned arithmetic.  Now we replace `sqrt' with a linear function */
  /*  that is smaller or equal for all values of c in the interval       */
  /*  [0;X/2]; it should be equal to sqrt(X) and sqrt(3X/4) at the       */
  /*  endpoints.  Substituting the linear solution and explicit numbers  */
  /*  we get                                                             */
  /*                                                                     */
  /*    a + b <= 131071.99 - c / 122291.84    .                          */
  /*                                                                     */
  /*  In practice, we should use a faster and even stronger inequality   */
  /*                                                                     */
  /*    a + b <= 131071 - (c >> 16)                                      */
  /*                                                                     */
  /*  or, alternatively,                                                 */
  /*                                                                     */
  /*    a + b <= 129894 - (c >> 17)    .                                 */
  /*                                                                     */
  /*  FT_MulFix, on the other hand, is optimized for a small value of    */
  /*  the first argument, when the second argument can be much larger.   */
  /*  This can be achieved by scaling the second argument and the limit  */
  /*  in the above inequalities.  For example,                           */
  /*                                                                     */
  /*    a + (b >> 8) <= (131071 >> 4)                                    */
  /*                                                                     */
  /*  covers the practical range of use. The actual test below is a bit  */
  /*  tighter to avoid the border case overflows.                        */
  /*                                                                     */
  /*  In the case of FT_DivFix, the exact overflow check                 */
  /*                                                                     */
  /*    a << 16 <= X - c/2                                               */
  /*                                                                     */
  /*  is scaled down by 2^16 and we use                                  */
  /*                                                                     */
  /*    a <= 65535 - (c >> 17)    .                                      */

  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Long )
  FT_MulDiv( FT_Long  a_,
             FT_Long  b_,
             FT_Long  c_ )
  {
    FT_Long    s = 1;
    FT_UInt32  a, b, c;


    /* XXX: this function does not allow 64-bit arguments */

    FT_MOVE_SIGN( FT_UInt32, a_, a, s );
    FT_MOVE_SIGN( FT_UInt32, b_, b, s );
    FT_MOVE_SIGN( FT_UInt32, c_, c, s );

    إذا ( c == 0 )
      a = 0x7FFFFFFFUL;

    وإلا إذا ( a + b <= 129894UL - ( c >> 17 ) )
      a = ( a * b + ( c >> 1 ) ) / c;

    وإلا
    {
      FT_Int64  موقت, temp2;


      ft_multo64( a, b, &موقت );

      temp2.hi = 0;
      temp2.lo = c >> 1;

      FT_Add64( &موقت, &temp2, &موقت );

      /* last attempt to ditch long division */
      a = ( موقت.hi == 0 ) ? موقت.lo / c
                           : ft_div64by32( موقت.hi, موقت.lo, c );
    }

    أرجع MUL_LONG( s, a );
  }


  FT_پایه_DEF( FT_Long )
  FT_Mul_Div_No_دور( FT_Long  a_,
                      FT_Long  b_,
                      FT_Long  c_ )
  {
    FT_Long    s = 1;
    FT_UInt32  a, b, c;


    /* XXX: this function does not allow 64-bit arguments */

    FT_MOVE_SIGN( FT_UInt32, a_, a, s );
    FT_MOVE_SIGN( FT_UInt32, b_, b, s );
    FT_MOVE_SIGN( FT_UInt32, c_, c, s );

    إذا ( c == 0 )
      a = 0x7FFFFFFFUL;

    وإلا إذا ( a + b <= 131071UL )
      a = a * b / c;

    وإلا
    {
      FT_Int64  موقت;


      ft_multo64( a, b, &موقت );

      /* last attempt to ditch long division */
      a = ( موقت.hi == 0 ) ? موقت.lo / c
                           : ft_div64by32( موقت.hi, موقت.lo, c );
    }

    أرجع MUL_LONG( s, a );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Long )
  FT_MulFix( FT_Long  a_,
             FT_Long  b_ )
  {
#اگر_تعریف_شده FT_MULFIX_ASSEMBLER

    أرجع FT_MULFIX_ASSEMBLER( a_, b_ );

#وإلاإذا 0

    /*
     * This code is nonportable.  See comment below.
     *
     * However, on a platform where right-shift of a signed quantity fills
     * the leftmost bits by copying the sign bit, it might be faster.
     */

    FT_Long    sa, sb;
    FT_UInt32  a, b;


    /*
     * This is a clever way of converting a signed number `a' into its
     * absolute value (stored back into `a') and its sign.  The sign is
     * stored in `sa'; 0 means `a' was positive or zero, and -1 means `a'
     * was negative.  (Similarly for `b' and `sb').
     *
     * Unfortunately, it doesn't work (at least not portably).
     *
     * It makes the assumption that right-shift on a negative signed value
     * fills the leftmost bits by copying the sign bit.  This is wrong.
     * According to K&R 2nd ed, section `A7.8 Shift Operators' on page 206,
     * the result of right-shift of a negative signed value is
     * implementation-defined.  At least one implementation fills the
     * leftmost bits with 0s (i.e., it is exactly the same as an unsigned
     * right shift).  This means that when `a' is negative, `sa' ends up
     * with the value 1 rather than -1.  After that, everything else goes
     * wrong.
     */
    sa = ( a_ >> ( حجم ( a_ ) * 8 - 1 ) );
    a  = ( a_ ^ sa ) - sa;
    sb = ( b_ >> ( حجم ( b_ ) * 8 - 1 ) );
    b  = ( b_ ^ sb ) - sb;

    a = (FT_UInt32)a_;
    b = (FT_UInt32)b_;

    إذا ( a + ( b >> 8 ) <= 8190UL )
      a = ( a * b + 0x8000U ) >> 16;
    وإلا
    {
      FT_UInt32  al = a & 0xFFFFUL;


      a = ( a >> 16 ) * b + al * ( b >> 16 ) +
          ( ( al * ( b & 0xFFFFUL ) + 0x8000UL ) >> 16 );
    }

    sa ^= sb;
    a   = ( a ^ sa ) - sa;

    أرجع (FT_Long)a;

#وإلا /* 0 */

    FT_Long    s = 1;
    FT_UInt32  a, b;


    /* XXX: this function does not allow 64-bit arguments */

    FT_MOVE_SIGN( FT_UInt32, a_, a, s );
    FT_MOVE_SIGN( FT_UInt32, b_, b, s );

    إذا ( a + ( b >> 8 ) <= 8190UL )
      a = ( a * b + 0x8000UL ) >> 16;
    وإلا
    {
      FT_UInt32  al = a & 0xFFFFUL;


      a = ( a >> 16 ) * b + al * ( b >> 16 ) +
          ( ( al * ( b & 0xFFFFUL ) + 0x8000UL ) >> 16 );
    }

    أرجع MUL_LONG( s, a );

#پایان_اگر /* 0 */

  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Long )
  FT_DivFix( FT_Long  a_,
             FT_Long  b_ )
  {
    FT_Long    s = 1;
    FT_UInt32  a, b, q;


    /* XXX: this function does not allow 64-bit arguments */

    FT_MOVE_SIGN( FT_UInt32, a_, a, s );
    FT_MOVE_SIGN( FT_UInt32, b_, b, s );

    إذا ( b == 0 )
    {
      /* check for division by 0 */
      q = 0x7FFFFFFFUL;
    }
    وإلا إذا ( a <= 65535UL - ( b >> 17 ) )
    {
      /* compute result directly */
      q = ( ( a << 16 ) + ( b >> 1 ) ) / b;
    }
    وإلا
    {
      /* we need more bits; we have to do it by hand */
      FT_Int64  موقت, temp2;


      موقت.hi  = a >> 16;
      موقت.lo  = a << 16;
      temp2.hi = 0;
      temp2.lo = b >> 1;

      FT_Add64( &موقت, &temp2, &موقت );
      q = ft_div64by32( موقت.hi, موقت.lo, b );
    }

    أرجع MUL_LONG( s, q );
  }


#پایان_اگر /* !FT_INT64 */


  /* documentation is in ftglyph.h */

  FT_EXPORT_DEF( فراغ )
  FT_ماتریس_Multiply( ثابت FT_ماتریس*  a,
                      FT_ماتریس        *b )
  {
    FT_Fixed  xx, xy, yx, yy;


    إذا ( !a || !b )
      أرجع;

    xx = جمع_کن_LONG( FT_MulFix( a->xx, b->xx ),
                   FT_MulFix( a->xy, b->yx ) );
    xy = جمع_کن_LONG( FT_MulFix( a->xx, b->xy ),
                   FT_MulFix( a->xy, b->yy ) );
    yx = جمع_کن_LONG( FT_MulFix( a->yx, b->xx ),
                   FT_MulFix( a->yy, b->yx ) );
    yy = جمع_کن_LONG( FT_MulFix( a->yx, b->xy ),
                   FT_MulFix( a->yy, b->yy ) );

    b->xx = xx;
    b->xy = xy;
    b->yx = yx;
    b->yy = yy;
  }


  /* documentation is in ftglyph.h */

  FT_EXPORT_DEF( FT_Error )
  FT_ماتریس_Invert( FT_ماتریس*  ماتریس )
  {
    FT_Pos  delta, xx, yy;


    إذا ( !ماتریس )
      أرجع FT_THROW( Invalid_آرگومان );

    /* compute discriminant */
    delta = FT_MulFix( ماتریس->xx, ماتریس->yy ) -
            FT_MulFix( ماتریس->xy, ماتریس->yx );

    إذا ( !delta )
      أرجع FT_THROW( Invalid_آرگومان );  /* matrix can't be inverted */

    ماتریس->xy = -FT_DivFix( ماتریس->xy, delta );
    ماتریس->yx = -FT_DivFix( ماتریس->yx, delta );

    xx = ماتریس->xx;
    yy = ماتریس->yy;

    ماتریس->xx = FT_DivFix( yy, delta );
    ماتریس->yy = FT_DivFix( xx, delta );

    أرجع FT_Err_Ok;
  }


  /* documentation is in ftcalc.h */

  FT_پایه_DEF( فراغ )
  FT_ماتریس_Multiply_Scaled( ثابت FT_ماتریس*  a,
                             FT_ماتریس        *b,
                             FT_Long           scaling )
  {
    FT_Fixed  xx, xy, yx, yy;

    FT_Long   val = 0x10000L * scaling;


    إذا ( !a || !b )
      أرجع;

    xx = جمع_کن_LONG( FT_MulDiv( a->xx, b->xx, val ),
                   FT_MulDiv( a->xy, b->yx, val ) );
    xy = جمع_کن_LONG( FT_MulDiv( a->xx, b->xy, val ),
                   FT_MulDiv( a->xy, b->yy, val ) );
    yx = جمع_کن_LONG( FT_MulDiv( a->yx, b->xx, val ),
                   FT_MulDiv( a->yy, b->yx, val ) );
    yy = جمع_کن_LONG( FT_MulDiv( a->yx, b->xy, val ),
                   FT_MulDiv( a->yy, b->yy, val ) );

    b->xx = xx;
    b->xy = xy;
    b->yx = yx;
    b->yy = yy;
  }


  /* documentation is in ftcalc.h */

  FT_پایه_DEF( FT_Bool )
  FT_ماتریس_Check( ثابت FT_ماتریس*  ماتریس )
  {
    FT_Fixed  xx, xy, yx, yy;
    FT_ULong  val, abs_det, frob_sq;
    FT_Int    لغزش;


    إذا ( !ماتریس )
      أرجع 0;

    xx  = ماتریس->xx;
    xy  = ماتریس->xy;
    yx  = ماتریس->yx;
    yy  = ماتریس->yy;
    val = ULABS( xx ) | ULABS( xy ) | ULABS( yx ) | ULABS( yy );

    /* we only handle non-zero 32-bit values */
    إذا ( !val || val > 0x7FFFFFFFUL )
      أرجع 0;

    /* Scale matrix to avoid overflows */
    لغزش = FT_MSB( val ) - 4 * حجم ( FT_ULong ) + 2;

    إذا ( لغزش > 0 )
    {
      xx >>= لغزش;
      xy >>= لغزش;
      yx >>= لغزش;
      yy >>= لغزش;
    }

    abs_det = ULABS( xx * yy - xy * yx );
    frob_sq = (FT_ULong)( xx * xx ) + (FT_ULong)( xy * xy ) +
              (FT_ULong)( yx * yx ) + (FT_ULong)( yy * yy );

    إذا ( abs_det <= frob_sq / 32 )
      أرجع 0;

    أرجع 1;
  }


  /* documentation is in ftcalc.h */

  FT_پایه_DEF( فراغ )
  FT_وکتور_Transform_Scaled( FT_وکتور*        وکتور,
                              ثابت FT_ماتریس*  ماتریس,
                              FT_Long           scaling )
  {
    FT_Pos   xz, yz;

    FT_Long  val = 0x10000L * scaling;


    إذا ( !وکتور || !ماتریس )
      أرجع;

    xz = جمع_کن_LONG( FT_MulDiv( وکتور->x, ماتریس->xx, val ),
                   FT_MulDiv( وکتور->y, ماتریس->xy, val ) );
    yz = جمع_کن_LONG( FT_MulDiv( وکتور->x, ماتریس->yx, val ),
                   FT_MulDiv( وکتور->y, ماتریس->yy, val ) );

    وکتور->x = xz;
    وکتور->y = yz;
  }


  /* documentation is in ftcalc.h */

  FT_پایه_DEF( FT_UInt32 )
  FT_وکتور_Norm_Len( FT_وکتور*  وکتور )
  {
    FT_Int32   x_ = وکتور->x;
    FT_Int32   y_ = وکتور->y;
    FT_Int32   b, z;
    FT_UInt32  x, y, u, v, l;
    FT_Int     sx = 1, sy = 1, لغزش;


    FT_MOVE_SIGN( FT_UInt32, x_, x, sx );
    FT_MOVE_SIGN( FT_UInt32, y_, y, sy );

    /* trivial cases */
    إذا ( x == 0 )
    {
      إذا ( y > 0 )
        وکتور->y = sy * 0x10000;
      أرجع y;
    }
    وإلا إذا ( y == 0 )
    {
      إذا ( x > 0 )
        وکتور->x = sx * 0x10000;
      أرجع x;
    }

    /* Estimate length and prenormalize by shifting so that */
    /* the new approximate length is between 2/3 and 4/3.   */
    /* The magic constant 0xAAAAAAAAUL (2/3 of 2^32) helps  */
    /* achieve this in 16.16 fixed-point representation.    */
    l = x > y ? x + ( y >> 1 )
              : y + ( x >> 1 );

    لغزش  = 31 - FT_MSB( l );
    لغزش -= 15 + ( l >= ( 0xAAAAAAAAUL >> لغزش ) );

    إذا ( لغزش > 0 )
    {
      x <<= لغزش;
      y <<= لغزش;

      /* re-estimate length for tiny vectors */
      l = x > y ? x + ( y >> 1 )
                : y + ( x >> 1 );
    }
    وإلا
    {
      x >>= -لغزش;
      y >>= -لغزش;
      l >>= -لغزش;
    }

    /* lower linear approximation for reciprocal length minus one */
    b = 0x10000 - (FT_Int32)l;

    x_ = (FT_Int32)x;
    y_ = (FT_Int32)y;

    /* Newton's iterations */
    افعل
    {
      u = (FT_UInt32)( x_ + ( x_ * b >> 16 ) );
      v = (FT_UInt32)( y_ + ( y_ * b >> 16 ) );

      /* Normalized squared length in the parentheses approaches 2^32. */
      /* On two's complement systems, converting to signed gives the   */
      /* difference with 2^32 even if the expression wraps around.     */
      z = -(FT_Int32)( u * u + v * v ) / 0x200;
      z = z * ( ( 0x10000 + b ) >> 8 ) / 0x10000;

      b += z;

    } طالما ( z > 0 );

    وکتور->x = sx < 0 ? -(FT_Pos)u : (FT_Pos)u;
    وکتور->y = sy < 0 ? -(FT_Pos)v : (FT_Pos)v;

    /* Conversion to signed helps to recover from likely wrap around */
    /* in calculating the prenormalized length, because it gives the */
    /* correct difference with 2^32 on two's complement systems.     */
    l = (FT_UInt32)( 0x10000 + (FT_Int32)( u * x + v * y ) / 0x10000 );
    إذا ( لغزش > 0 )
      l = ( l + ( 1 << ( لغزش - 1 ) ) ) >> لغزش;
    وإلا
      l <<= -لغزش;

    أرجع l;
  }


  /* documentation is in ftcalc.h */

  FT_پایه_DEF( FT_UInt32 )
  FT_SqrtFixed( FT_UInt32  v )
  {
    إذا ( v == 0 )
      أرجع 0;

#اگر_تعریف_نشده FT_INT64

    /* Algorithm by Christophe Meessen (1993) with overflow fixed and     */
    /* rounding added.  Any unsigned fixed 16.16 argument is acceptable.  */
    /* However, this algorithm is slower than the Babylonian method with  */
    /* a good initial guess.  We only use it for large 32-bit values when */
    /* 64-bit computations are not desirable.                             */
    وإلا إذا ( v > 0x10000U )
    {
      FT_UInt32  r = v >> 1;
      FT_UInt32  q = ( v & 1 ) << 15;
      FT_UInt32  b = 0x20000000;
      FT_UInt32  t;


      افعل
      {
        t = q + b;
        إذا ( r >= t )
        {
          r -= t;
          q  = t + b;  /* equivalent to q += 2*b */
        }
        r <<= 1;
        b >>= 1;

      } طالما ( b > 0x10 );  /* exactly 25 cycles */

      أرجع ( q + 0x40 ) >> 7;
    }
    وإلا
    {
      FT_UInt32  r = ( v << 16 ) - 1;

#وإلا /* FT_INT64 */

    وإلا
    {
      FT_UInt64  r = ( (FT_UInt64)v << 16 ) - 1;

#پایان_اگر /* FT_INT64 */

      FT_UInt32  q = 1 << ( ( 17 + FT_MSB( v ) ) >> 1 );
      FT_UInt32  t;


      /* Babylonian method with rounded-up division */
      افعل
      {
        t = q;
        q = ( t + (FT_UInt32)( r / t ) + 1 ) >> 1;

      } طالما ( q != t );  /* less than 6 cycles */

      أرجع q;
    }
  }


  /* documentation is in ftcalc.h */

  FT_پایه_DEF( FT_Int )
  ft_گوشه_orientation( FT_Pos  درون_x,
                         FT_Pos  درون_y,
                         FT_Pos  بیرون_x,
                         FT_Pos  بیرون_y )
  {
    /* we silently ignore overflow errors since such large values */
    /* lead to even more (harmless) rendering errors later on     */

#اگر_تعریف_شده FT_INT64

    FT_Int64  delta = SUB_INT64( MUL_INT64( درون_x, بیرون_y ),
                                 MUL_INT64( درون_y, بیرون_x ) );


    أرجع ( delta > 0 ) - ( delta < 0 );

#وإلا

    FT_Int64  z1, z2;
    FT_Int    نتیجه;


    إذا ( ULABS( درون_x ) + ULABS( بیرون_y ) <= 92681UL )
    {
      z1.lo = (FT_UInt32)درون_x * (FT_UInt32)بیرون_y;
      z1.hi = (FT_UInt32)( (FT_Int32)z1.lo >> 31 );  /* sign-expansion */
    }
    وإلا
      ft_multo64( (FT_UInt32)درون_x, (FT_UInt32)بیرون_y, &z1 );

    إذا ( ULABS( درون_y ) + ULABS( بیرون_x ) <= 92681UL )
    {
      z2.lo = (FT_UInt32)درون_y * (FT_UInt32)بیرون_x;
      z2.hi = (FT_UInt32)( (FT_Int32)z2.lo >> 31 );  /* sign-expansion */
    }
    وإلا
      ft_multo64( (FT_UInt32)درون_y, (FT_UInt32)بیرون_x, &z2 );

    إذا      ( (FT_Int32)z1.hi > (FT_Int32)z2.hi )
      نتیجه = +1;
    وإلا إذا ( (FT_Int32)z1.hi < (FT_Int32)z2.hi )
      نتیجه = -1;
    وإلا إذا ( z1.lo > z2.lo )
      نتیجه = +1;
    وإلا إذا ( z1.lo < z2.lo )
      نتیجه = -1;
    وإلا
      نتیجه =  0;

    /* XXX: only the sign of return value, +1/0/-1 must be used */
    أرجع نتیجه;

#پایان_اگر
  }


  /* documentation is in ftcalc.h */

  FT_پایه_DEF( FT_Int )
  ft_گوشه_is_flat( FT_Pos  درون_x,
                     FT_Pos  درون_y,
                     FT_Pos  بیرون_x,
                     FT_Pos  بیرون_y )
  {
    FT_Pos  ax = درون_x + بیرون_x;
    FT_Pos  ay = درون_y + بیرون_y;

    FT_Pos  d_درون, d_بیرون, d_hypot;


    /* The idea of this function is to compare the length of the */
    /* hypotenuse with the `in' and `out' length.  The `corner'  */
    /* represented by `in' and `out' is flat if the hypotenuse's */
    /* length isn't too large.                                   */
    /*                                                           */
    /* This approach has the advantage that the angle between    */
    /* `in' and `out' is not checked.  In case one of the two    */
    /* vectors is `dominant', that is, much larger than the      */
    /* other vector, we thus always have a flat corner.          */
    /*                                                           */
    /*                hypotenuse                                 */
    /*       x---------------------------x                       */
    /*        \                      /                           */
    /*         \                /                                */
    /*      in  \          /  out                                */
    /*           \    /                                          */
    /*            o                                              */
    /*نقطه*/

    d_درون    = FT_HYPOT(  درون_x,  درون_y );
    d_بیرون   = FT_HYPOT( بیرون_x, بیرون_y );
    d_hypot = FT_HYPOT(    ax,    ay );

    /* now do a simple length comparison: */
    /*                                    */
    /*   d_in + d_out < 17/16 d_hypot     */

    أرجع ( d_درون + d_بیرون - d_hypot ) < ( d_hypot >> 4 );
  }


/*انتها*/
