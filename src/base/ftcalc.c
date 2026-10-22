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


#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/ftglyph.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/fttrigon.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftcalc.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftdebug.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftobjs.h>

  /* cancel inlining macro from internal/ftcalc.h */
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ FT_MulFix
#  undef FT_MulFix
#ਜੇ_ਅੰਤ


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ  FT_COMPONENT
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FT_COMPONENT  calc


  /* transfer sign, leaving a positive number;                        */
  /* we need an unsigned value to safely negate INT_MIN (or LONG_MIN) */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FT_MOVE_SIGN( utype, x, x_unsigned, s ) \
  FT_BEGIN_STMNT                                \
    ਜੇ ( x < 0 )                                \
    {                                           \
      x_unsigned = 0U - (utype)x;               \
      s          = -s;                          \
    }                                           \
    ਨਹੀਂਤਾਂ                                        \
      x_unsigned = (utype)x;                    \
  FT_ਸਿਰਾ_STMNT

  /* The following three functions are available regardless of whether */
  /* FT_INT64 is defined.                                              */

  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Fixed )
  FT_ਗੇੜਾ_Fix( FT_Fixed  a )
  {
    ਵਾਪਸ ਜੋੜੋ_LONG( a, 0x8000L - ( a < 0 ) ) & ~0xFFFFL;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Fixed )
  FT_CeilFix( FT_Fixed  a )
  {
    ਵਾਪਸ ਜੋੜੋ_LONG( a, 0xFFFFL ) & ~0xFFFFL;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Fixed )
  FT_FloorFix( FT_Fixed  a )
  {
    ਵਾਪਸ a & ~0xFFFFL;
  }

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ FT_MSB

  FT_ਅਧਾਰ_DEF( FT_Int )
  FT_MSB( FT_UInt32 z )
  {
    FT_Int  ਖਿਸਕਾਓ = 0;


    /* determine msb bit index in `shift' */
    ਜੇ ( z & 0xFFFF0000UL )
    {
      z     >>= 16;
      ਖਿਸਕਾਓ  += 16;
    }
    ਜੇ ( z & 0x0000FF00UL )
    {
      z     >>= 8;
      ਖਿਸਕਾਓ  += 8;
    }
    ਜੇ ( z & 0x000000F0UL )
    {
      z     >>= 4;
      ਖਿਸਕਾਓ  += 4;
    }
    ਜੇ ( z & 0x0000000CUL )
    {
      z     >>= 2;
      ਖਿਸਕਾਓ  += 2;
    }
    ਜੇ ( z & 0x00000002UL )
    {
   /* z     >>= 1; */
      ਖਿਸਕਾਓ  += 1;
    }

    ਵਾਪਸ ਖਿਸਕਾਓ;
  }

#ਜੇ_ਅੰਤ /* !FT_MSB */


  /* documentation is in ftcalc.h */

  FT_ਅਧਾਰ_DEF( FT_Fixed )
  FT_Hypot( FT_Fixed  x,
            FT_Fixed  y )
  {
    FT_ਸਦਿਸ਼  v;


    v.x = x;
    v.y = y;

    ਵਾਪਸ FT_ਸਦਿਸ਼_ਲੰਮਾਈ( &v );
  }


#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ FT_INT64


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

    ਵਾਪਸ MUL_LONG( s, d );
  }


  /* documentation is in ftcalc.h */

  FT_ਅਧਾਰ_DEF( FT_Long )
  FT_Mul_Div_No_ਗੇੜਾ( FT_Long  a_,
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

    ਵਾਪਸ MUL_LONG( s, d );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Long )
  FT_MulFix( FT_Long  a_,
             FT_Long  b_ )
  {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ FT_CONFIG_OPTION_INLINE_MULFIX

    ਵਾਪਸ FT_MulFix_64( a_, b_ );

#ਨਹੀਂਤਾਂ

    FT_Int64  ab = MUL_INT64( a_, b_ );

    /* this requires arithmetic right shift of signed numbers */
    ਵਾਪਸ (FT_Long)( ( ab + 0x8000L + ( ab >> 63 ) ) >> 16 );

#ਜੇ_ਅੰਤ /* FT_CONFIG_OPTION_INLINE_MULFIX */
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

    ਵਾਪਸ MUL_LONG( s, q );
  }


#ਨਹੀਂਤਾਂ /* !FT_INT64 */


  ਸਥਿਰ ਖਾਲੀ
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


  ਸਥਿਰ FT_UInt32
  ft_div64by32( FT_UInt32  hi,
                FT_UInt32  lo,
                FT_UInt32  y )
  {
    FT_UInt32  r, q;
    FT_Int     i;


    ਜੇ ( hi >= y )
      ਵਾਪਸ (FT_UInt32)0x7FFFFFFFL;

    /* We shift as many bits as we can into the high register, perform     */
    /* 32-bit division with modulo there, then work through the remaining  */
    /* bits with long division. This optimization is especially noticeable */
    /* for smaller dividends that barely use the high register.            */

    i = 31 - FT_MSB( hi );
    r = ( hi << i ) | ( lo >> ( 32 - i ) ); lo <<= i; /* left 64-bit shift */
    q = r / y;
    r -= q * y;   /* remainder */

    i = 32 - i;   /* bits remaining in low register */
    ਕਰੋ
    {
      q <<= 1;
      r   = ( r << 1 ) | ( lo >> 31 ); lo <<= 1;

      ਜੇ ( r >= y )
      {
        r -= y;
        q |= 1;
      }
    } ਜਦੋਂ ( --i );

    ਵਾਪਸ q;
  }


  ਸਥਿਰ ਖਾਲੀ
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

    ਜੇ ( c == 0 )
      a = 0x7FFFFFFFUL;

    ਨਹੀਂਤਾਂ ਜੇ ( a + b <= 129894UL - ( c >> 17 ) )
      a = ( a * b + ( c >> 1 ) ) / c;

    ਨਹੀਂਤਾਂ
    {
      FT_Int64  ਆਰਜ਼ੀ, temp2;


      ft_multo64( a, b, &ਆਰਜ਼ੀ );

      temp2.hi = 0;
      temp2.lo = c >> 1;

      FT_Add64( &ਆਰਜ਼ੀ, &temp2, &ਆਰਜ਼ੀ );

      /* last attempt to ditch long division */
      a = ( ਆਰਜ਼ੀ.hi == 0 ) ? ਆਰਜ਼ੀ.lo / c
                           : ft_div64by32( ਆਰਜ਼ੀ.hi, ਆਰਜ਼ੀ.lo, c );
    }

    ਵਾਪਸ MUL_LONG( s, a );
  }


  FT_ਅਧਾਰ_DEF( FT_Long )
  FT_Mul_Div_No_ਗੇੜਾ( FT_Long  a_,
                      FT_Long  b_,
                      FT_Long  c_ )
  {
    FT_Long    s = 1;
    FT_UInt32  a, b, c;


    /* XXX: this function does not allow 64-bit arguments */

    FT_MOVE_SIGN( FT_UInt32, a_, a, s );
    FT_MOVE_SIGN( FT_UInt32, b_, b, s );
    FT_MOVE_SIGN( FT_UInt32, c_, c, s );

    ਜੇ ( c == 0 )
      a = 0x7FFFFFFFUL;

    ਨਹੀਂਤਾਂ ਜੇ ( a + b <= 131071UL )
      a = a * b / c;

    ਨਹੀਂਤਾਂ
    {
      FT_Int64  ਆਰਜ਼ੀ;


      ft_multo64( a, b, &ਆਰਜ਼ੀ );

      /* last attempt to ditch long division */
      a = ( ਆਰਜ਼ੀ.hi == 0 ) ? ਆਰਜ਼ੀ.lo / c
                           : ft_div64by32( ਆਰਜ਼ੀ.hi, ਆਰਜ਼ੀ.lo, c );
    }

    ਵਾਪਸ MUL_LONG( s, a );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Long )
  FT_MulFix( FT_Long  a_,
             FT_Long  b_ )
  {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ FT_MULFIX_ASSEMBLER

    ਵਾਪਸ FT_MULFIX_ASSEMBLER( a_, b_ );

#ਜਾਂਜੇ 0

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
    sa = ( a_ >> ( ਆਕਾਰ ( a_ ) * 8 - 1 ) );
    a  = ( a_ ^ sa ) - sa;
    sb = ( b_ >> ( ਆਕਾਰ ( b_ ) * 8 - 1 ) );
    b  = ( b_ ^ sb ) - sb;

    a = (FT_UInt32)a_;
    b = (FT_UInt32)b_;

    ਜੇ ( a + ( b >> 8 ) <= 8190UL )
      a = ( a * b + 0x8000U ) >> 16;
    ਨਹੀਂਤਾਂ
    {
      FT_UInt32  al = a & 0xFFFFUL;


      a = ( a >> 16 ) * b + al * ( b >> 16 ) +
          ( ( al * ( b & 0xFFFFUL ) + 0x8000UL ) >> 16 );
    }

    sa ^= sb;
    a   = ( a ^ sa ) - sa;

    ਵਾਪਸ (FT_Long)a;

#ਨਹੀਂਤਾਂ /* 0 */

    FT_Long    s = 1;
    FT_UInt32  a, b;


    /* XXX: this function does not allow 64-bit arguments */

    FT_MOVE_SIGN( FT_UInt32, a_, a, s );
    FT_MOVE_SIGN( FT_UInt32, b_, b, s );

    ਜੇ ( a + ( b >> 8 ) <= 8190UL )
      a = ( a * b + 0x8000UL ) >> 16;
    ਨਹੀਂਤਾਂ
    {
      FT_UInt32  al = a & 0xFFFFUL;


      a = ( a >> 16 ) * b + al * ( b >> 16 ) +
          ( ( al * ( b & 0xFFFFUL ) + 0x8000UL ) >> 16 );
    }

    ਵਾਪਸ MUL_LONG( s, a );

#ਜੇ_ਅੰਤ /* 0 */

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

    ਜੇ ( b == 0 )
    {
      /* check for division by 0 */
      q = 0x7FFFFFFFUL;
    }
    ਨਹੀਂਤਾਂ ਜੇ ( a <= 65535UL - ( b >> 17 ) )
    {
      /* compute result directly */
      q = ( ( a << 16 ) + ( b >> 1 ) ) / b;
    }
    ਨਹੀਂਤਾਂ
    {
      /* we need more bits; we have to do it by hand */
      FT_Int64  ਆਰਜ਼ੀ, temp2;


      ਆਰਜ਼ੀ.hi  = a >> 16;
      ਆਰਜ਼ੀ.lo  = a << 16;
      temp2.hi = 0;
      temp2.lo = b >> 1;

      FT_Add64( &ਆਰਜ਼ੀ, &temp2, &ਆਰਜ਼ੀ );
      q = ft_div64by32( ਆਰਜ਼ੀ.hi, ਆਰਜ਼ੀ.lo, b );
    }

    ਵਾਪਸ MUL_LONG( s, q );
  }


#ਜੇ_ਅੰਤ /* !FT_INT64 */


  /* documentation is in ftglyph.h */

  FT_EXPORT_DEF( ਖਾਲੀ )
  FT_ਮੈਟ੍ਰਿਕਸ_Multiply( ਅਚਲ FT_ਮੈਟ੍ਰਿਕਸ*  a,
                      FT_ਮੈਟ੍ਰਿਕਸ        *b )
  {
    FT_Fixed  xx, xy, yx, yy;


    ਜੇ ( !a || !b )
      ਵਾਪਸ;

    xx = ਜੋੜੋ_LONG( FT_MulFix( a->xx, b->xx ),
                   FT_MulFix( a->xy, b->yx ) );
    xy = ਜੋੜੋ_LONG( FT_MulFix( a->xx, b->xy ),
                   FT_MulFix( a->xy, b->yy ) );
    yx = ਜੋੜੋ_LONG( FT_MulFix( a->yx, b->xx ),
                   FT_MulFix( a->yy, b->yx ) );
    yy = ਜੋੜੋ_LONG( FT_MulFix( a->yx, b->xy ),
                   FT_MulFix( a->yy, b->yy ) );

    b->xx = xx;
    b->xy = xy;
    b->yx = yx;
    b->yy = yy;
  }


  /* documentation is in ftglyph.h */

  FT_EXPORT_DEF( FT_Error )
  FT_ਮੈਟ੍ਰਿਕਸ_Invert( FT_ਮੈਟ੍ਰਿਕਸ*  ਮੈਟ੍ਰਿਕਸ )
  {
    FT_Pos  delta, xx, yy;


    ਜੇ ( !ਮੈਟ੍ਰਿਕਸ )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    /* compute discriminant */
    delta = FT_MulFix( ਮੈਟ੍ਰਿਕਸ->xx, ਮੈਟ੍ਰਿਕਸ->yy ) -
            FT_MulFix( ਮੈਟ੍ਰਿਕਸ->xy, ਮੈਟ੍ਰਿਕਸ->yx );

    ਜੇ ( !delta )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );  /* matrix can't be inverted */

    ਮੈਟ੍ਰਿਕਸ->xy = -FT_DivFix( ਮੈਟ੍ਰਿਕਸ->xy, delta );
    ਮੈਟ੍ਰਿਕਸ->yx = -FT_DivFix( ਮੈਟ੍ਰਿਕਸ->yx, delta );

    xx = ਮੈਟ੍ਰਿਕਸ->xx;
    yy = ਮੈਟ੍ਰਿਕਸ->yy;

    ਮੈਟ੍ਰਿਕਸ->xx = FT_DivFix( yy, delta );
    ਮੈਟ੍ਰਿਕਸ->yy = FT_DivFix( xx, delta );

    ਵਾਪਸ FT_Err_Ok;
  }


  /* documentation is in ftcalc.h */

  FT_ਅਧਾਰ_DEF( ਖਾਲੀ )
  FT_ਮੈਟ੍ਰਿਕਸ_Multiply_Scaled( ਅਚਲ FT_ਮੈਟ੍ਰਿਕਸ*  a,
                             FT_ਮੈਟ੍ਰਿਕਸ        *b,
                             FT_Long           scaling )
  {
    FT_Fixed  xx, xy, yx, yy;

    FT_Long   val = 0x10000L * scaling;


    ਜੇ ( !a || !b )
      ਵਾਪਸ;

    xx = ਜੋੜੋ_LONG( FT_MulDiv( a->xx, b->xx, val ),
                   FT_MulDiv( a->xy, b->yx, val ) );
    xy = ਜੋੜੋ_LONG( FT_MulDiv( a->xx, b->xy, val ),
                   FT_MulDiv( a->xy, b->yy, val ) );
    yx = ਜੋੜੋ_LONG( FT_MulDiv( a->yx, b->xx, val ),
                   FT_MulDiv( a->yy, b->yx, val ) );
    yy = ਜੋੜੋ_LONG( FT_MulDiv( a->yx, b->xy, val ),
                   FT_MulDiv( a->yy, b->yy, val ) );

    b->xx = xx;
    b->xy = xy;
    b->yx = yx;
    b->yy = yy;
  }


  /* documentation is in ftcalc.h */

  FT_ਅਧਾਰ_DEF( FT_Bool )
  FT_ਮੈਟ੍ਰਿਕਸ_Check( ਅਚਲ FT_ਮੈਟ੍ਰਿਕਸ*  ਮੈਟ੍ਰਿਕਸ )
  {
    FT_Fixed  xx, xy, yx, yy;
    FT_ULong  val, abs_det, frob_sq;
    FT_Int    ਖਿਸਕਾਓ;


    ਜੇ ( !ਮੈਟ੍ਰਿਕਸ )
      ਵਾਪਸ 0;

    xx  = ਮੈਟ੍ਰਿਕਸ->xx;
    xy  = ਮੈਟ੍ਰਿਕਸ->xy;
    yx  = ਮੈਟ੍ਰਿਕਸ->yx;
    yy  = ਮੈਟ੍ਰਿਕਸ->yy;
    val = ULABS( xx ) | ULABS( xy ) | ULABS( yx ) | ULABS( yy );

    /* we only handle non-zero 32-bit values */
    ਜੇ ( !val || val > 0x7FFFFFFFUL )
      ਵਾਪਸ 0;

    /* Scale matrix to avoid overflows */
    ਖਿਸਕਾਓ = FT_MSB( val ) - 4 * ਆਕਾਰ ( FT_ULong ) + 2;

    ਜੇ ( ਖਿਸਕਾਓ > 0 )
    {
      xx >>= ਖਿਸਕਾਓ;
      xy >>= ਖਿਸਕਾਓ;
      yx >>= ਖਿਸਕਾਓ;
      yy >>= ਖਿਸਕਾਓ;
    }

    abs_det = ULABS( xx * yy - xy * yx );
    frob_sq = (FT_ULong)( xx * xx ) + (FT_ULong)( xy * xy ) +
              (FT_ULong)( yx * yx ) + (FT_ULong)( yy * yy );

    ਜੇ ( abs_det <= frob_sq / 32 )
      ਵਾਪਸ 0;

    ਵਾਪਸ 1;
  }


  /* documentation is in ftcalc.h */

  FT_ਅਧਾਰ_DEF( ਖਾਲੀ )
  FT_ਸਦਿਸ਼_Transform_Scaled( FT_ਸਦਿਸ਼*        ਸਦਿਸ਼,
                              ਅਚਲ FT_ਮੈਟ੍ਰਿਕਸ*  ਮੈਟ੍ਰਿਕਸ,
                              FT_Long           scaling )
  {
    FT_Pos   xz, yz;

    FT_Long  val = 0x10000L * scaling;


    ਜੇ ( !ਸਦਿਸ਼ || !ਮੈਟ੍ਰਿਕਸ )
      ਵਾਪਸ;

    xz = ਜੋੜੋ_LONG( FT_MulDiv( ਸਦਿਸ਼->x, ਮੈਟ੍ਰਿਕਸ->xx, val ),
                   FT_MulDiv( ਸਦਿਸ਼->y, ਮੈਟ੍ਰਿਕਸ->xy, val ) );
    yz = ਜੋੜੋ_LONG( FT_MulDiv( ਸਦਿਸ਼->x, ਮੈਟ੍ਰਿਕਸ->yx, val ),
                   FT_MulDiv( ਸਦਿਸ਼->y, ਮੈਟ੍ਰਿਕਸ->yy, val ) );

    ਸਦਿਸ਼->x = xz;
    ਸਦਿਸ਼->y = yz;
  }


  /* documentation is in ftcalc.h */

  FT_ਅਧਾਰ_DEF( FT_UInt32 )
  FT_ਸਦਿਸ਼_Norm_Len( FT_ਸਦਿਸ਼*  ਸਦਿਸ਼ )
  {
    FT_Int32   x_ = ਸਦਿਸ਼->x;
    FT_Int32   y_ = ਸਦਿਸ਼->y;
    FT_Int32   b, z;
    FT_UInt32  x, y, u, v, l;
    FT_Int     sx = 1, sy = 1, ਖਿਸਕਾਓ;


    FT_MOVE_SIGN( FT_UInt32, x_, x, sx );
    FT_MOVE_SIGN( FT_UInt32, y_, y, sy );

    /* trivial cases */
    ਜੇ ( x == 0 )
    {
      ਜੇ ( y > 0 )
        ਸਦਿਸ਼->y = sy * 0x10000;
      ਵਾਪਸ y;
    }
    ਨਹੀਂਤਾਂ ਜੇ ( y == 0 )
    {
      ਜੇ ( x > 0 )
        ਸਦਿਸ਼->x = sx * 0x10000;
      ਵਾਪਸ x;
    }

    /* Estimate length and prenormalize by shifting so that */
    /* the new approximate length is between 2/3 and 4/3.   */
    /* The magic constant 0xAAAAAAAAUL (2/3 of 2^32) helps  */
    /* achieve this in 16.16 fixed-point representation.    */
    l = x > y ? x + ( y >> 1 )
              : y + ( x >> 1 );

    ਖਿਸਕਾਓ  = 31 - FT_MSB( l );
    ਖਿਸਕਾਓ -= 15 + ( l >= ( 0xAAAAAAAAUL >> ਖਿਸਕਾਓ ) );

    ਜੇ ( ਖਿਸਕਾਓ > 0 )
    {
      x <<= ਖਿਸਕਾਓ;
      y <<= ਖਿਸਕਾਓ;

      /* re-estimate length for tiny vectors */
      l = x > y ? x + ( y >> 1 )
                : y + ( x >> 1 );
    }
    ਨਹੀਂਤਾਂ
    {
      x >>= -ਖਿਸਕਾਓ;
      y >>= -ਖਿਸਕਾਓ;
      l >>= -ਖਿਸਕਾਓ;
    }

    /* lower linear approximation for reciprocal length minus one */
    b = 0x10000 - (FT_Int32)l;

    x_ = (FT_Int32)x;
    y_ = (FT_Int32)y;

    /* Newton's iterations */
    ਕਰੋ
    {
      u = (FT_UInt32)( x_ + ( x_ * b >> 16 ) );
      v = (FT_UInt32)( y_ + ( y_ * b >> 16 ) );

      /* Normalized squared length in the parentheses approaches 2^32. */
      /* On two's complement systems, converting to signed gives the   */
      /* difference with 2^32 even if the expression wraps around.     */
      z = -(FT_Int32)( u * u + v * v ) / 0x200;
      z = z * ( ( 0x10000 + b ) >> 8 ) / 0x10000;

      b += z;

    } ਜਦੋਂ ( z > 0 );

    ਸਦਿਸ਼->x = sx < 0 ? -(FT_Pos)u : (FT_Pos)u;
    ਸਦਿਸ਼->y = sy < 0 ? -(FT_Pos)v : (FT_Pos)v;

    /* Conversion to signed helps to recover from likely wrap around */
    /* in calculating the prenormalized length, because it gives the */
    /* correct difference with 2^32 on two's complement systems.     */
    l = (FT_UInt32)( 0x10000 + (FT_Int32)( u * x + v * y ) / 0x10000 );
    ਜੇ ( ਖਿਸਕਾਓ > 0 )
      l = ( l + ( 1 << ( ਖਿਸਕਾਓ - 1 ) ) ) >> ਖਿਸਕਾਓ;
    ਨਹੀਂਤਾਂ
      l <<= -ਖਿਸਕਾਓ;

    ਵਾਪਸ l;
  }


  /* documentation is in ftcalc.h */

  FT_ਅਧਾਰ_DEF( FT_UInt32 )
  FT_SqrtFixed( FT_UInt32  v )
  {
    ਜੇ ( v == 0 )
      ਵਾਪਸ 0;

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ FT_INT64

    /* Algorithm by Christophe Meessen (1993) with overflow fixed and     */
    /* rounding added.  Any unsigned fixed 16.16 argument is acceptable.  */
    /* However, this algorithm is slower than the Babylonian method with  */
    /* a good initial guess.  We only use it for large 32-bit values when */
    /* 64-bit computations are not desirable.                             */
    ਨਹੀਂਤਾਂ ਜੇ ( v > 0x10000U )
    {
      FT_UInt32  r = v >> 1;
      FT_UInt32  q = ( v & 1 ) << 15;
      FT_UInt32  b = 0x20000000;
      FT_UInt32  t;


      ਕਰੋ
      {
        t = q + b;
        ਜੇ ( r >= t )
        {
          r -= t;
          q  = t + b;  /* equivalent to q += 2*b */
        }
        r <<= 1;
        b >>= 1;

      } ਜਦੋਂ ( b > 0x10 );  /* exactly 25 cycles */

      ਵਾਪਸ ( q + 0x40 ) >> 7;
    }
    ਨਹੀਂਤਾਂ
    {
      FT_UInt32  r = ( v << 16 ) - 1;

#ਨਹੀਂਤਾਂ /* FT_INT64 */

    ਨਹੀਂਤਾਂ
    {
      FT_UInt64  r = ( (FT_UInt64)v << 16 ) - 1;

#ਜੇ_ਅੰਤ /* FT_INT64 */

      FT_UInt32  q = 1 << ( ( 17 + FT_MSB( v ) ) >> 1 );
      FT_UInt32  t;


      /* Babylonian method with rounded-up division */
      ਕਰੋ
      {
        t = q;
        q = ( t + (FT_UInt32)( r / t ) + 1 ) >> 1;

      } ਜਦੋਂ ( q != t );  /* less than 6 cycles */

      ਵਾਪਸ q;
    }
  }


  /* documentation is in ftcalc.h */

  FT_ਅਧਾਰ_DEF( FT_Int )
  ft_ਕੋਨਾ_orientation( FT_Pos  ਵਿੱਚ_x,
                         FT_Pos  ਵਿੱਚ_y,
                         FT_Pos  ਬਾਹਰਲਾ_x,
                         FT_Pos  ਬਾਹਰਲਾ_y )
  {
    /* we silently ignore overflow errors since such large values */
    /* lead to even more (harmless) rendering errors later on     */

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ FT_INT64

    FT_Int64  delta = SUB_INT64( MUL_INT64( ਵਿੱਚ_x, ਬਾਹਰਲਾ_y ),
                                 MUL_INT64( ਵਿੱਚ_y, ਬਾਹਰਲਾ_x ) );


    ਵਾਪਸ ( delta > 0 ) - ( delta < 0 );

#ਨਹੀਂਤਾਂ

    FT_Int64  z1, z2;
    FT_Int    ਨਤੀਜਾ;


    ਜੇ ( ULABS( ਵਿੱਚ_x ) + ULABS( ਬਾਹਰਲਾ_y ) <= 92681UL )
    {
      z1.lo = (FT_UInt32)ਵਿੱਚ_x * (FT_UInt32)ਬਾਹਰਲਾ_y;
      z1.hi = (FT_UInt32)( (FT_Int32)z1.lo >> 31 );  /* sign-expansion */
    }
    ਨਹੀਂਤਾਂ
      ft_multo64( (FT_UInt32)ਵਿੱਚ_x, (FT_UInt32)ਬਾਹਰਲਾ_y, &z1 );

    ਜੇ ( ULABS( ਵਿੱਚ_y ) + ULABS( ਬਾਹਰਲਾ_x ) <= 92681UL )
    {
      z2.lo = (FT_UInt32)ਵਿੱਚ_y * (FT_UInt32)ਬਾਹਰਲਾ_x;
      z2.hi = (FT_UInt32)( (FT_Int32)z2.lo >> 31 );  /* sign-expansion */
    }
    ਨਹੀਂਤਾਂ
      ft_multo64( (FT_UInt32)ਵਿੱਚ_y, (FT_UInt32)ਬਾਹਰਲਾ_x, &z2 );

    ਜੇ      ( (FT_Int32)z1.hi > (FT_Int32)z2.hi )
      ਨਤੀਜਾ = +1;
    ਨਹੀਂਤਾਂ ਜੇ ( (FT_Int32)z1.hi < (FT_Int32)z2.hi )
      ਨਤੀਜਾ = -1;
    ਨਹੀਂਤਾਂ ਜੇ ( z1.lo > z2.lo )
      ਨਤੀਜਾ = +1;
    ਨਹੀਂਤਾਂ ਜੇ ( z1.lo < z2.lo )
      ਨਤੀਜਾ = -1;
    ਨਹੀਂਤਾਂ
      ਨਤੀਜਾ =  0;

    /* XXX: only the sign of return value, +1/0/-1 must be used */
    ਵਾਪਸ ਨਤੀਜਾ;

#ਜੇ_ਅੰਤ
  }


  /* documentation is in ftcalc.h */

  FT_ਅਧਾਰ_DEF( FT_Int )
  ft_ਕੋਨਾ_is_flat( FT_Pos  ਵਿੱਚ_x,
                     FT_Pos  ਵਿੱਚ_y,
                     FT_Pos  ਬਾਹਰਲਾ_x,
                     FT_Pos  ਬਾਹਰਲਾ_y )
  {
    FT_Pos  ax = ਵਿੱਚ_x + ਬਾਹਰਲਾ_x;
    FT_Pos  ay = ਵਿੱਚ_y + ਬਾਹਰਲਾ_y;

    FT_Pos  d_ਵਿੱਚ, d_ਬਾਹਰਲਾ, d_hypot;


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
    /*ਬਿੰਦੂ*/

    d_ਵਿੱਚ    = FT_HYPOT(  ਵਿੱਚ_x,  ਵਿੱਚ_y );
    d_ਬਾਹਰਲਾ   = FT_HYPOT( ਬਾਹਰਲਾ_x, ਬਾਹਰਲਾ_y );
    d_hypot = FT_HYPOT(    ax,    ay );

    /* now do a simple length comparison: */
    /*                                    */
    /*   d_in + d_out < 17/16 d_hypot     */

    ਵਾਪਸ ( d_ਵਿੱਚ + d_ਬਾਹਰਲਾ - d_hypot ) < ( d_hypot >> 4 );
  }


/*ਸਿਰਾ*/
