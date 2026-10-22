/****************************************************************************
 *
 * fttrigon.c
 *
 *   FreeType trigonometric functions (body).
 *
 * Copyright (C) 2001-2026 by
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
   * This is a fixed-point CORDIC implementation of trigonometric
   * functions as well as transformations between Cartesian and polar
   * coordinates.  The angles are represented as 16.16 fixed-point values
   * in degrees, i.e., the angular resolution is 2^-16 degrees.  Note that
   * only vectors longer than 2^16*180/pi (or at least 22 bits) on a
   * discrete Cartesian grid can have the same or better angular
   * resolution.  Therefore, to maintain this precision, some functions
   * require an interim upscaling of the vectors, whereas others operate
   * with 24-bit long vectors directly.
   *
   */

#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftobjs.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftcalc.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/fttrigon.h>


  /* the Cordic shrink factor 0.858785336480436 * 2^32 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FT_TRIG_SCALE      0xDBD95B16UL

  /* the highest bit in overflow-safe vector components, */
  /* MSB of 0.858785336480436 * sqrt(0.5) * 2^30         */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FT_TRIG_ਸੁਰੱਖਿਅਤ_MSB   29

  /* this table was generated for FT_PI = 180L << 16, i.e. degrees */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FT_TRIG_MAX_ITERS  23

  ਸਥਿਰ ਅਚਲ FT_Angle
  ft_trig_arctan_ਸਾਰਣੀ[] =
  {
    1740967L, 919879L, 466945L, 234379L, 117304L, 58666L, 29335L,
    14668L, 7334L, 3667L, 1833L, 917L, 458L, 229L, 115L,
    57L, 29L, 14L, 7L, 4L, 2L, 1L
  };


#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ FT_INT64

  /* multiply a given value by the CORDIC shrink factor */
  ਸਥਿਰ FT_Fixed
  ft_trig_downscale( FT_Fixed  val )
  {
    FT_Int  s = 1;


    ਜੇ ( val < 0 )
    {
       val = -val;
       s = -1;
    }

    /* 0x40000000 comes from regression analysis between true */
    /* and CORDIC hypotenuse, so it minimizes the error       */
    val = (FT_Fixed)(
            ( (FT_UInt64)val * FT_TRIG_SCALE + 0x40000000UL ) >> 32 );

    ਵਾਪਸ s < 0 ? -val : val;
  }

#ਨਹੀਂਤਾਂ /* !FT_INT64 */

  /* multiply a given value by the CORDIC shrink factor */
  ਸਥਿਰ FT_Fixed
  ft_trig_downscale( FT_Fixed  val )
  {
    FT_Int     s = 1;
    FT_UInt32  lo1, hi1, lo2, hi2, lo, hi, i1, i2;


    ਜੇ ( val < 0 )
    {
       val = -val;
       s = -1;
    }

    lo1 = (FT_UInt32)val & 0x0000FFFFU;
    hi1 = (FT_UInt32)val >> 16;
    lo2 = FT_TRIG_SCALE & 0x0000FFFFU;
    hi2 = FT_TRIG_SCALE >> 16;

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

    /* 0x40000000 comes from regression analysis between true */
    /* and CORDIC hypotenuse, so it minimizes the error       */

    /* Check carry overflow of lo + 0x40000000 */
    lo += 0x40000000UL;
    hi += ( lo < 0x40000000UL );

    val = (FT_Fixed)hi;

    ਵਾਪਸ s < 0 ? -val : val;
  }

#ਜੇ_ਅੰਤ /* !FT_INT64 */


  /* undefined and never called for zero vector */
  ਸਥਿਰ FT_Int
  ft_trig_prenorm( FT_ਸਦਿਸ਼*  vec )
  {
    FT_Pos  x, y;
    FT_Int  ਖਿਸਕਾਓ;


    x = vec->x;
    y = vec->y;

    ਖਿਸਕਾਓ = FT_MSB( (FT_UInt32)( FT_ABS( x ) | FT_ABS( y ) ) );

    ਜੇ ( ਖਿਸਕਾਓ <= FT_TRIG_ਸੁਰੱਖਿਅਤ_MSB )
    {
      ਖਿਸਕਾਓ  = FT_TRIG_ਸੁਰੱਖਿਅਤ_MSB - ਖਿਸਕਾਓ;
      vec->x = (FT_Pos)( (FT_ULong)x << ਖਿਸਕਾਓ );
      vec->y = (FT_Pos)( (FT_ULong)y << ਖਿਸਕਾਓ );
    }
    ਨਹੀਂਤਾਂ
    {
      ਖਿਸਕਾਓ -= FT_TRIG_ਸੁਰੱਖਿਅਤ_MSB;
      vec->x = x >> ਖਿਸਕਾਓ;
      vec->y = y >> ਖਿਸਕਾਓ;
      ਖਿਸਕਾਓ  = -ਖਿਸਕਾਓ;
    }

    ਵਾਪਸ ਖਿਸਕਾਓ;
  }


  ਸਥਿਰ ਖਾਲੀ
  ft_trig_pseudo_rotate( FT_ਸਦਿਸ਼*  vec,
                         FT_Angle    theta )
  {
    FT_Int           i;
    FT_Fixed         x, y, xtemp, b;
    ਅਚਲ FT_Angle  *arctanptr;


    x = vec->x;
    y = vec->y;

    /* Rotate inside [-PI/4,PI/4] sector */
    ਜਦੋਂ ( theta < -FT_ANGLE_PI4 )
    {
      xtemp  =  y;
      y      = -x;
      x      =  xtemp;
      theta +=  FT_ANGLE_PI2;
    }

    ਜਦੋਂ ( theta > FT_ANGLE_PI4 )
    {
      xtemp  = -y;
      y      =  x;
      x      =  xtemp;
      theta -=  FT_ANGLE_PI2;
    }

    arctanptr = ft_trig_arctan_ਸਾਰਣੀ;

    /* Pseudorotations, with right shifts.  To avoid an unpredictable */
    /* branch on the sign of theta, `m` is 0 or -1 and negates with   */
    /* `( v ^ m ) - m`.  For x and y, the negation is done before the */
    /* rounding shift: as 2b = 2^i, `-( ( v + b ) >> i )` equals      */
    /* `( ~v + b ) >> i` exactly.                                     */
    ਲਈ ( i = 1, b = 1; i < FT_TRIG_MAX_ITERS; b <<= 1, i++ )
    {
      FT_Fixed  m = -(FT_Fixed)( theta < 0 );
      FT_Fixed  a = *arctanptr++;


      xtemp = x - ( ( ( y ^ m ) + b ) >> i );
      y     = y + ( ( ( x ^ m ) + b ) >> i );
      x     = xtemp;
      theta = theta - ( ( a ^ m ) - m );
    }

    vec->x = x;
    vec->y = y;
  }


  ਸਥਿਰ ਖਾਲੀ
  ft_trig_pseudo_polarize( FT_ਸਦਿਸ਼*  vec )
  {
    FT_Angle         theta;
    FT_Int           i;
    FT_Fixed         x, y, xtemp, b;
    ਅਚਲ FT_Angle  *arctanptr;


    x = vec->x;
    y = vec->y;

    /* Get the vector into [-PI/4,PI/4] sector */
    ਜੇ ( y > x )
    {
      ਜੇ ( y > -x )
      {
        theta =  FT_ANGLE_PI2;
        xtemp =  y;
        y     = -x;
        x     =  xtemp;
      }
      ਨਹੀਂਤਾਂ
      {
        theta =  y > 0 ? FT_ANGLE_PI : -FT_ANGLE_PI;
        x     = -x;
        y     = -y;
      }
    }
    ਨਹੀਂਤਾਂ
    {
      ਜੇ ( y < -x )
      {
        theta = -FT_ANGLE_PI2;
        xtemp = -y;
        y     =  x;
        x     =  xtemp;
      }
      ਨਹੀਂਤਾਂ
      {
        theta = 0;
      }
    }

    arctanptr = ft_trig_arctan_ਸਾਰਣੀ;

    /* Pseudorotations, with right shifts.  As in the rotation above, */
    /* `m` is 0 or -1 and negates with `( v ^ m ) - m`.  Here, `m`    */
    /* depends on y, so x and y are negated after the shift to keep   */
    /* the shift off y's dependency chain.                            */
    ਲਈ ( i = 1, b = 1; i < FT_TRIG_MAX_ITERS; b <<= 1, i++ )
    {
      FT_Fixed  m = -(FT_Fixed)( y < 0 );
      FT_Fixed  a = *arctanptr++;


      xtemp = x + ( ( ( y ^ m ) + b ) >> i );
      y     = y - ( ( ( x ^ m ) + b ) >> i );
      x     = xtemp;
      theta = theta + ( ( a ^ m ) - m );
    }

    /* round theta to acknowledge its error that mostly comes */
    /* from accumulated rounding errors in the arctan table   */
    theta = ( theta - ( theta < 0 ) + 8 ) & ~0xF;

    vec->x = x;
    vec->y = theta;
  }


  /* documentation is in fttrigon.h */

  FT_EXPORT_DEF( FT_Fixed )
  FT_Cos( FT_Angle  angle )
  {
    FT_ਸਦਿਸ਼  v;


    FT_ਸਦਿਸ਼_Unit( &v, angle );

    ਵਾਪਸ v.x;
  }


  /* documentation is in fttrigon.h */

  FT_EXPORT_DEF( FT_Fixed )
  FT_Sin( FT_Angle  angle )
  {
    FT_ਸਦਿਸ਼  v;


    FT_ਸਦਿਸ਼_Unit( &v, angle );

    ਵਾਪਸ v.y;
  }


  /* documentation is in fttrigon.h */

  FT_EXPORT_DEF( FT_Fixed )
  FT_Tan( FT_Angle  angle )
  {
    FT_ਸਦਿਸ਼  v = { 1 << 24, 0 };


    ft_trig_pseudo_rotate( &v, angle );

    ਵਾਪਸ FT_DivFix( v.y, v.x );
  }


  /* documentation is in fttrigon.h */

  FT_EXPORT_DEF( FT_Angle )
  FT_Atan2( FT_Fixed  dx,
            FT_Fixed  dy )
  {
    FT_ਸਦਿਸ਼  v;


    ਜੇ ( dx == 0 && dy == 0 )
      ਵਾਪਸ 0;

    v.x = dx;
    v.y = dy;
    ft_trig_prenorm( &v );
    ft_trig_pseudo_polarize( &v );

    ਵਾਪਸ v.y;
  }


  /* documentation is in fttrigon.h */

  FT_EXPORT_DEF( ਖਾਲੀ )
  FT_ਸਦਿਸ਼_Unit( FT_ਸਦਿਸ਼*  vec,
                  FT_Angle    angle )
  {
    ਜੇ ( !vec )
      ਵਾਪਸ;

    vec->x = FT_TRIG_SCALE >> 8;
    vec->y = 0;
    ft_trig_pseudo_rotate( vec, angle );
    vec->x = ( vec->x + 0x80L ) >> 8;
    vec->y = ( vec->y + 0x80L ) >> 8;
  }


  /* documentation is in fttrigon.h */

  FT_EXPORT_DEF( ਖਾਲੀ )
  FT_ਸਦਿਸ਼_Rotate( FT_ਸਦਿਸ਼*  vec,
                    FT_Angle    angle )
  {
    FT_Int     ਖਿਸਕਾਓ;
    FT_ਸਦਿਸ਼  v;


    ਜੇ ( !vec || !angle )
      ਵਾਪਸ;

    v = *vec;

    ਜੇ ( v.x == 0 && v.y == 0 )
      ਵਾਪਸ;

    ਖਿਸਕਾਓ = ft_trig_prenorm( &v );
    ft_trig_pseudo_rotate( &v, angle );
    v.x = ft_trig_downscale( v.x );
    v.y = ft_trig_downscale( v.y );

    ਜੇ ( ਖਿਸਕਾਓ > 0 )
    {
      FT_Int32  half = (FT_Int32)1L << ( ਖਿਸਕਾਓ - 1 );


      vec->x = ( v.x + half - ( v.x < 0 ) ) >> ਖਿਸਕਾਓ;
      vec->y = ( v.y + half - ( v.y < 0 ) ) >> ਖਿਸਕਾਓ;
    }
    ਨਹੀਂਤਾਂ
    {
      ਖਿਸਕਾਓ  = -ਖਿਸਕਾਓ;
      vec->x = (FT_Pos)( (FT_ULong)v.x << ਖਿਸਕਾਓ );
      vec->y = (FT_Pos)( (FT_ULong)v.y << ਖਿਸਕਾਓ );
    }
  }


  /* documentation is in fttrigon.h */

  FT_EXPORT_DEF( FT_Fixed )
  FT_ਸਦਿਸ਼_ਲੰਮਾਈ( FT_ਸਦਿਸ਼*  vec )
  {
    FT_Int     ਖਿਸਕਾਓ;
    FT_ਸਦਿਸ਼  v;


    ਜੇ ( !vec )
      ਵਾਪਸ 0;

    v = *vec;

    /* handle trivial cases */
    ਜੇ ( v.x == 0 )
    {
      ਵਾਪਸ FT_ABS( v.y );
    }
    ਨਹੀਂਤਾਂ ਜੇ ( v.y == 0 )
    {
      ਵਾਪਸ FT_ABS( v.x );
    }

    /* general case */
    ਖਿਸਕਾਓ = ft_trig_prenorm( &v );
    ft_trig_pseudo_polarize( &v );

    v.x = ft_trig_downscale( v.x );

    ਜੇ ( ਖਿਸਕਾਓ > 0 )
      ਵਾਪਸ ( v.x + ( 1L << ( ਖਿਸਕਾਓ - 1 ) ) ) >> ਖਿਸਕਾਓ;

    ਵਾਪਸ (FT_Fixed)( (FT_UInt32)v.x << -ਖਿਸਕਾਓ );
  }


  /* documentation is in fttrigon.h */

  FT_EXPORT_DEF( ਖਾਲੀ )
  FT_ਸਦਿਸ਼_Polarize( FT_ਸਦਿਸ਼*  vec,
                      FT_Fixed   *ਲੰਮਾਈ,
                      FT_Angle   *angle )
  {
    FT_Int     ਖਿਸਕਾਓ;
    FT_ਸਦਿਸ਼  v;


    ਜੇ ( !vec || !ਲੰਮਾਈ || !angle )
      ਵਾਪਸ;

    v = *vec;

    ਜੇ ( v.x == 0 && v.y == 0 )
      ਵਾਪਸ;

    ਖਿਸਕਾਓ = ft_trig_prenorm( &v );
    ft_trig_pseudo_polarize( &v );

    v.x = ft_trig_downscale( v.x );

    *ਲੰਮਾਈ = ਖਿਸਕਾਓ >= 0 ?                      ( v.x >>  ਖਿਸਕਾਓ )
                         : (FT_Fixed)( (FT_UInt32)v.x << -ਖਿਸਕਾਓ );
    *angle  = v.y;
  }


  /* documentation is in fttrigon.h */

  FT_EXPORT_DEF( ਖਾਲੀ )
  FT_ਸਦਿਸ਼_From_Polar( FT_ਸਦਿਸ਼*  vec,
                        FT_Fixed    ਲੰਮਾਈ,
                        FT_Angle    angle )
  {
    ਜੇ ( !vec )
      ਵਾਪਸ;

    vec->x = ਲੰਮਾਈ;
    vec->y = 0;

    FT_ਸਦਿਸ਼_Rotate( vec, angle );
  }


  /* documentation is in fttrigon.h */

  FT_EXPORT_DEF( FT_Angle )
  FT_Angle_Diff( FT_Angle  angle1,
                 FT_Angle  angle2 )
  {
    FT_Angle  delta = angle2 - angle1;


    ਜਦੋਂ ( delta <= -FT_ANGLE_PI )
      delta += FT_ANGLE_2PI;

    ਜਦੋਂ ( delta > FT_ANGLE_PI )
      delta -= FT_ANGLE_2PI;

    ਵਾਪਸ delta;
  }


/*ਸਿਰਾ*/
