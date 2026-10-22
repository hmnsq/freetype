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

#अन्तर्भाव <freetype/internal/ftobjs.h>
#अन्तर्भाव <freetype/internal/ftcalc.h>
#अन्तर्भाव <freetype/fttrigon.h>


  /* the Cordic shrink factor 0.858785336480436 * 2^32 */
#परिभाषय FT_TRIG_SCALE      0xDBD95B16UL

  /* the highest bit in overflow-safe vector components, */
  /* MSB of 0.858785336480436 * sqrt(0.5) * 2^30         */
#परिभाषय FT_TRIG_SAFE_MSB   29

  /* this table was generated for FT_PI = 180L << 16, i.e. degrees */
#परिभाषय FT_TRIG_MAX_ITERS  23

  स्थिर ध्रुव FT_Angle
  ft_trig_arctan_सारणी[] =
  {
    1740967L, 919879L, 466945L, 234379L, 117304L, 58666L, 29335L,
    14668L, 7334L, 3667L, 1833L, 917L, 458L, 229L, 115L,
    57L, 29L, 14L, 7L, 4L, 2L, 1L
  };


#यदि_परिभाषितम् FT_INT64

  /* multiply a given value by the CORDIC shrink factor */
  स्थिर FT_Fixed
  ft_trig_downscale( FT_Fixed  val )
  {
    FT_Int  s = 1;


    यदि ( val < 0 )
    {
       val = -val;
       s = -1;
    }

    /* 0x40000000 comes from regression analysis between true */
    /* and CORDIC hypotenuse, so it minimizes the error       */
    val = (FT_Fixed)(
            ( (FT_UInt64)val * FT_TRIG_SCALE + 0x40000000UL ) >> 32 );

    प्रत्यागम s < 0 ? -val : val;
  }

#अन्यथा /* !FT_INT64 */

  /* multiply a given value by the CORDIC shrink factor */
  स्थिर FT_Fixed
  ft_trig_downscale( FT_Fixed  val )
  {
    FT_Int     s = 1;
    FT_UInt32  lo1, hi1, lo2, hi2, lo, hi, i1, i2;


    यदि ( val < 0 )
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

    प्रत्यागम s < 0 ? -val : val;
  }

#यदि_अन्तः /* !FT_INT64 */


  /* undefined and never called for zero vector */
  स्थिर FT_Int
  ft_trig_prenorm( FT_सदिशम्*  vec )
  {
    FT_Pos  x, y;
    FT_Int  सरणम्;


    x = vec->x;
    y = vec->y;

    सरणम् = FT_MSB( (FT_UInt32)( FT_ABS( x ) | FT_ABS( y ) ) );

    यदि ( सरणम् <= FT_TRIG_SAFE_MSB )
    {
      सरणम्  = FT_TRIG_SAFE_MSB - सरणम्;
      vec->x = (FT_Pos)( (FT_ULong)x << सरणम् );
      vec->y = (FT_Pos)( (FT_ULong)y << सरणम् );
    }
    अन्यथा
    {
      सरणम् -= FT_TRIG_SAFE_MSB;
      vec->x = x >> सरणम्;
      vec->y = y >> सरणम्;
      सरणम्  = -सरणम्;
    }

    प्रत्यागम सरणम्;
  }


  स्थिर शून्य
  ft_trig_pseudo_rotate( FT_सदिशम्*  vec,
                         FT_Angle    theta )
  {
    FT_Int           i;
    FT_Fixed         x, y, xtemp, b;
    ध्रुव FT_Angle  *arctanptr;


    x = vec->x;
    y = vec->y;

    /* Rotate inside [-PI/4,PI/4] sector */
    यावत् ( theta < -FT_ANGLE_PI4 )
    {
      xtemp  =  y;
      y      = -x;
      x      =  xtemp;
      theta +=  FT_ANGLE_PI2;
    }

    यावत् ( theta > FT_ANGLE_PI4 )
    {
      xtemp  = -y;
      y      =  x;
      x      =  xtemp;
      theta -=  FT_ANGLE_PI2;
    }

    arctanptr = ft_trig_arctan_सारणी;

    /* Pseudorotations, with right shifts.  To avoid an unpredictable */
    /* branch on the sign of theta, `m` is 0 or -1 and negates with   */
    /* `( v ^ m ) - m`.  For x and y, the negation is done before the */
    /* rounding shift: as 2b = 2^i, `-( ( v + b ) >> i )` equals      */
    /* `( ~v + b ) >> i` exactly.                                     */
    प्रति ( i = 1, b = 1; i < FT_TRIG_MAX_ITERS; b <<= 1, i++ )
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


  स्थिर शून्य
  ft_trig_pseudo_polarize( FT_सदिशम्*  vec )
  {
    FT_Angle         theta;
    FT_Int           i;
    FT_Fixed         x, y, xtemp, b;
    ध्रुव FT_Angle  *arctanptr;


    x = vec->x;
    y = vec->y;

    /* Get the vector into [-PI/4,PI/4] sector */
    यदि ( y > x )
    {
      यदि ( y > -x )
      {
        theta =  FT_ANGLE_PI2;
        xtemp =  y;
        y     = -x;
        x     =  xtemp;
      }
      अन्यथा
      {
        theta =  y > 0 ? FT_ANGLE_PI : -FT_ANGLE_PI;
        x     = -x;
        y     = -y;
      }
    }
    अन्यथा
    {
      यदि ( y < -x )
      {
        theta = -FT_ANGLE_PI2;
        xtemp = -y;
        y     =  x;
        x     =  xtemp;
      }
      अन्यथा
      {
        theta = 0;
      }
    }

    arctanptr = ft_trig_arctan_सारणी;

    /* Pseudorotations, with right shifts.  As in the rotation above, */
    /* `m` is 0 or -1 and negates with `( v ^ m ) - m`.  Here, `m`    */
    /* depends on y, so x and y are negated after the shift to keep   */
    /* the shift off y's dependency chain.                            */
    प्रति ( i = 1, b = 1; i < FT_TRIG_MAX_ITERS; b <<= 1, i++ )
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
    FT_सदिशम्  v;


    FT_सदिशम्_Unit( &v, angle );

    प्रत्यागम v.x;
  }


  /* documentation is in fttrigon.h */

  FT_EXPORT_DEF( FT_Fixed )
  FT_Sin( FT_Angle  angle )
  {
    FT_सदिशम्  v;


    FT_सदिशम्_Unit( &v, angle );

    प्रत्यागम v.y;
  }


  /* documentation is in fttrigon.h */

  FT_EXPORT_DEF( FT_Fixed )
  FT_Tan( FT_Angle  angle )
  {
    FT_सदिशम्  v = { 1 << 24, 0 };


    ft_trig_pseudo_rotate( &v, angle );

    प्रत्यागम FT_DivFix( v.y, v.x );
  }


  /* documentation is in fttrigon.h */

  FT_EXPORT_DEF( FT_Angle )
  FT_Atan2( FT_Fixed  dx,
            FT_Fixed  dy )
  {
    FT_सदिशम्  v;


    यदि ( dx == 0 && dy == 0 )
      प्रत्यागम 0;

    v.x = dx;
    v.y = dy;
    ft_trig_prenorm( &v );
    ft_trig_pseudo_polarize( &v );

    प्रत्यागम v.y;
  }


  /* documentation is in fttrigon.h */

  FT_EXPORT_DEF( शून्य )
  FT_सदिशम्_Unit( FT_सदिशम्*  vec,
                  FT_Angle    angle )
  {
    यदि ( !vec )
      प्रत्यागम;

    vec->x = FT_TRIG_SCALE >> 8;
    vec->y = 0;
    ft_trig_pseudo_rotate( vec, angle );
    vec->x = ( vec->x + 0x80L ) >> 8;
    vec->y = ( vec->y + 0x80L ) >> 8;
  }


  /* documentation is in fttrigon.h */

  FT_EXPORT_DEF( शून्य )
  FT_सदिशम्_Rotate( FT_सदिशम्*  vec,
                    FT_Angle    angle )
  {
    FT_Int     सरणम्;
    FT_सदिशम्  v;


    यदि ( !vec || !angle )
      प्रत्यागम;

    v = *vec;

    यदि ( v.x == 0 && v.y == 0 )
      प्रत्यागम;

    सरणम् = ft_trig_prenorm( &v );
    ft_trig_pseudo_rotate( &v, angle );
    v.x = ft_trig_downscale( v.x );
    v.y = ft_trig_downscale( v.y );

    यदि ( सरणम् > 0 )
    {
      FT_Int32  half = (FT_Int32)1L << ( सरणम् - 1 );


      vec->x = ( v.x + half - ( v.x < 0 ) ) >> सरणम्;
      vec->y = ( v.y + half - ( v.y < 0 ) ) >> सरणम्;
    }
    अन्यथा
    {
      सरणम्  = -सरणम्;
      vec->x = (FT_Pos)( (FT_ULong)v.x << सरणम् );
      vec->y = (FT_Pos)( (FT_ULong)v.y << सरणम् );
    }
  }


  /* documentation is in fttrigon.h */

  FT_EXPORT_DEF( FT_Fixed )
  FT_सदिशम्_दीर्घता( FT_सदिशम्*  vec )
  {
    FT_Int     सरणम्;
    FT_सदिशम्  v;


    यदि ( !vec )
      प्रत्यागम 0;

    v = *vec;

    /* handle trivial cases */
    यदि ( v.x == 0 )
    {
      प्रत्यागम FT_ABS( v.y );
    }
    अन्यथा यदि ( v.y == 0 )
    {
      प्रत्यागम FT_ABS( v.x );
    }

    /* general case */
    सरणम् = ft_trig_prenorm( &v );
    ft_trig_pseudo_polarize( &v );

    v.x = ft_trig_downscale( v.x );

    यदि ( सरणम् > 0 )
      प्रत्यागम ( v.x + ( 1L << ( सरणम् - 1 ) ) ) >> सरणम्;

    प्रत्यागम (FT_Fixed)( (FT_UInt32)v.x << -सरणम् );
  }


  /* documentation is in fttrigon.h */

  FT_EXPORT_DEF( शून्य )
  FT_सदिशम्_Polarize( FT_सदिशम्*  vec,
                      FT_Fixed   *दीर्घता,
                      FT_Angle   *angle )
  {
    FT_Int     सरणम्;
    FT_सदिशम्  v;


    यदि ( !vec || !दीर्घता || !angle )
      प्रत्यागम;

    v = *vec;

    यदि ( v.x == 0 && v.y == 0 )
      प्रत्यागम;

    सरणम् = ft_trig_prenorm( &v );
    ft_trig_pseudo_polarize( &v );

    v.x = ft_trig_downscale( v.x );

    *दीर्घता = सरणम् >= 0 ?                      ( v.x >>  सरणम् )
                         : (FT_Fixed)( (FT_UInt32)v.x << -सरणम् );
    *angle  = v.y;
  }


  /* documentation is in fttrigon.h */

  FT_EXPORT_DEF( शून्य )
  FT_सदिशम्_From_Polar( FT_सदिशम्*  vec,
                        FT_Fixed    दीर्घता,
                        FT_Angle    angle )
  {
    यदि ( !vec )
      प्रत्यागम;

    vec->x = दीर्घता;
    vec->y = 0;

    FT_सदिशम्_Rotate( vec, angle );
  }


  /* documentation is in fttrigon.h */

  FT_EXPORT_DEF( FT_Angle )
  FT_Angle_Diff( FT_Angle  angle1,
                 FT_Angle  angle2 )
  {
    FT_Angle  delta = angle2 - angle1;


    यावत् ( delta <= -FT_ANGLE_PI )
      delta += FT_ANGLE_2PI;

    यावत् ( delta > FT_ANGLE_PI )
      delta -= FT_ANGLE_2PI;

    प्रत्यागम delta;
  }


/*अन्तः*/
