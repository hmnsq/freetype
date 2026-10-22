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

#అన్తర్భావ <freetype/internal/ftobjs.h>
#అన్తర్భావ <freetype/internal/ftcalc.h>
#అన్తర్భావ <freetype/fttrigon.h>


  /* the Cordic shrink factor 0.858785336480436 * 2^32 */
#నిర్వచించు FT_TRIG_SCALE      0xDBD95B16UL

  /* the highest bit in overflow-safe vector components, */
  /* MSB of 0.858785336480436 * sqrt(0.5) * 2^30         */
#నిర్వచించు FT_TRIG_సురక్షిత_MSB   29

  /* this table was generated for FT_PI = 180L << 16, i.e. degrees */
#నిర్వచించు FT_TRIG_MAX_ITERS  23

  స్థిర స్థిరరాశి FT_Angle
  ft_trig_arctan_పట్టిక[] =
  {
    1740967L, 919879L, 466945L, 234379L, 117304L, 58666L, 29335L,
    14668L, 7334L, 3667L, 1833L, 917L, 458L, 229L, 115L,
    57L, 29L, 14L, 7L, 4L, 2L, 1L
  };


#నిర్వచితమైతే FT_INT64

  /* multiply a given value by the CORDIC shrink factor */
  స్థిర FT_Fixed
  ft_trig_downscale( FT_Fixed  val )
  {
    FT_Int  s = 1;


    ఒకవేళ ( val < 0 )
    {
       val = -val;
       s = -1;
    }

    /* 0x40000000 comes from regression analysis between true */
    /* and CORDIC hypotenuse, so it minimizes the error       */
    val = (FT_Fixed)(
            ( (FT_UInt64)val * FT_TRIG_SCALE + 0x40000000UL ) >> 32 );

    తిరిగి s < 0 ? -val : val;
  }

#లేకపోతే /* !FT_INT64 */

  /* multiply a given value by the CORDIC shrink factor */
  స్థిర FT_Fixed
  ft_trig_downscale( FT_Fixed  val )
  {
    FT_Int     s = 1;
    FT_UInt32  lo1, hi1, lo2, hi2, lo, hi, i1, i2;


    ఒకవేళ ( val < 0 )
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

    తిరిగి s < 0 ? -val : val;
  }

#అయితే_ముగింపు /* !FT_INT64 */


  /* undefined and never called for zero vector */
  స్థిర FT_Int
  ft_trig_prenorm( FT_సదిశ*  vec )
  {
    FT_Pos  x, y;
    FT_Int  జరుపు;


    x = vec->x;
    y = vec->y;

    జరుపు = FT_MSB( (FT_UInt32)( FT_ABS( x ) | FT_ABS( y ) ) );

    ఒకవేళ ( జరుపు <= FT_TRIG_సురక్షిత_MSB )
    {
      జరుపు  = FT_TRIG_సురక్షిత_MSB - జరుపు;
      vec->x = (FT_Pos)( (FT_ULong)x << జరుపు );
      vec->y = (FT_Pos)( (FT_ULong)y << జరుపు );
    }
    లేకపోతే
    {
      జరుపు -= FT_TRIG_సురక్షిత_MSB;
      vec->x = x >> జరుపు;
      vec->y = y >> జరుపు;
      జరుపు  = -జరుపు;
    }

    తిరిగి జరుపు;
  }


  స్థిర శూన్య
  ft_trig_pseudo_rotate( FT_సదిశ*  vec,
                         FT_Angle    theta )
  {
    FT_Int           i;
    FT_Fixed         x, y, xtemp, b;
    స్థిరరాశి FT_Angle  *arctanptr;


    x = vec->x;
    y = vec->y;

    /* Rotate inside [-PI/4,PI/4] sector */
    అయితే ( theta < -FT_ANGLE_PI4 )
    {
      xtemp  =  y;
      y      = -x;
      x      =  xtemp;
      theta +=  FT_ANGLE_PI2;
    }

    అయితే ( theta > FT_ANGLE_PI4 )
    {
      xtemp  = -y;
      y      =  x;
      x      =  xtemp;
      theta -=  FT_ANGLE_PI2;
    }

    arctanptr = ft_trig_arctan_పట్టిక;

    /* Pseudorotations, with right shifts.  To avoid an unpredictable */
    /* branch on the sign of theta, `m` is 0 or -1 and negates with   */
    /* `( v ^ m ) - m`.  For x and y, the negation is done before the */
    /* rounding shift: as 2b = 2^i, `-( ( v + b ) >> i )` equals      */
    /* `( ~v + b ) >> i` exactly.                                     */
    కొరకు ( i = 1, b = 1; i < FT_TRIG_MAX_ITERS; b <<= 1, i++ )
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


  స్థిర శూన్య
  ft_trig_pseudo_polarize( FT_సదిశ*  vec )
  {
    FT_Angle         theta;
    FT_Int           i;
    FT_Fixed         x, y, xtemp, b;
    స్థిరరాశి FT_Angle  *arctanptr;


    x = vec->x;
    y = vec->y;

    /* Get the vector into [-PI/4,PI/4] sector */
    ఒకవేళ ( y > x )
    {
      ఒకవేళ ( y > -x )
      {
        theta =  FT_ANGLE_PI2;
        xtemp =  y;
        y     = -x;
        x     =  xtemp;
      }
      లేకపోతే
      {
        theta =  y > 0 ? FT_ANGLE_PI : -FT_ANGLE_PI;
        x     = -x;
        y     = -y;
      }
    }
    లేకపోతే
    {
      ఒకవేళ ( y < -x )
      {
        theta = -FT_ANGLE_PI2;
        xtemp = -y;
        y     =  x;
        x     =  xtemp;
      }
      లేకపోతే
      {
        theta = 0;
      }
    }

    arctanptr = ft_trig_arctan_పట్టిక;

    /* Pseudorotations, with right shifts.  As in the rotation above, */
    /* `m` is 0 or -1 and negates with `( v ^ m ) - m`.  Here, `m`    */
    /* depends on y, so x and y are negated after the shift to keep   */
    /* the shift off y's dependency chain.                            */
    కొరకు ( i = 1, b = 1; i < FT_TRIG_MAX_ITERS; b <<= 1, i++ )
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
    FT_సదిశ  v;


    FT_సదిశ_Unit( &v, angle );

    తిరిగి v.x;
  }


  /* documentation is in fttrigon.h */

  FT_EXPORT_DEF( FT_Fixed )
  FT_Sin( FT_Angle  angle )
  {
    FT_సదిశ  v;


    FT_సదిశ_Unit( &v, angle );

    తిరిగి v.y;
  }


  /* documentation is in fttrigon.h */

  FT_EXPORT_DEF( FT_Fixed )
  FT_Tan( FT_Angle  angle )
  {
    FT_సదిశ  v = { 1 << 24, 0 };


    ft_trig_pseudo_rotate( &v, angle );

    తిరిగి FT_DivFix( v.y, v.x );
  }


  /* documentation is in fttrigon.h */

  FT_EXPORT_DEF( FT_Angle )
  FT_Atan2( FT_Fixed  dx,
            FT_Fixed  dy )
  {
    FT_సదిశ  v;


    ఒకవేళ ( dx == 0 && dy == 0 )
      తిరిగి 0;

    v.x = dx;
    v.y = dy;
    ft_trig_prenorm( &v );
    ft_trig_pseudo_polarize( &v );

    తిరిగి v.y;
  }


  /* documentation is in fttrigon.h */

  FT_EXPORT_DEF( శూన్య )
  FT_సదిశ_Unit( FT_సదిశ*  vec,
                  FT_Angle    angle )
  {
    ఒకవేళ ( !vec )
      తిరిగి;

    vec->x = FT_TRIG_SCALE >> 8;
    vec->y = 0;
    ft_trig_pseudo_rotate( vec, angle );
    vec->x = ( vec->x + 0x80L ) >> 8;
    vec->y = ( vec->y + 0x80L ) >> 8;
  }


  /* documentation is in fttrigon.h */

  FT_EXPORT_DEF( శూన్య )
  FT_సదిశ_Rotate( FT_సదిశ*  vec,
                    FT_Angle    angle )
  {
    FT_Int     జరుపు;
    FT_సదిశ  v;


    ఒకవేళ ( !vec || !angle )
      తిరిగి;

    v = *vec;

    ఒకవేళ ( v.x == 0 && v.y == 0 )
      తిరిగి;

    జరుపు = ft_trig_prenorm( &v );
    ft_trig_pseudo_rotate( &v, angle );
    v.x = ft_trig_downscale( v.x );
    v.y = ft_trig_downscale( v.y );

    ఒకవేళ ( జరుపు > 0 )
    {
      FT_Int32  half = (FT_Int32)1L << ( జరుపు - 1 );


      vec->x = ( v.x + half - ( v.x < 0 ) ) >> జరుపు;
      vec->y = ( v.y + half - ( v.y < 0 ) ) >> జరుపు;
    }
    లేకపోతే
    {
      జరుపు  = -జరుపు;
      vec->x = (FT_Pos)( (FT_ULong)v.x << జరుపు );
      vec->y = (FT_Pos)( (FT_ULong)v.y << జరుపు );
    }
  }


  /* documentation is in fttrigon.h */

  FT_EXPORT_DEF( FT_Fixed )
  FT_సదిశ_నిడివి( FT_సదిశ*  vec )
  {
    FT_Int     జరుపు;
    FT_సదిశ  v;


    ఒకవేళ ( !vec )
      తిరిగి 0;

    v = *vec;

    /* handle trivial cases */
    ఒకవేళ ( v.x == 0 )
    {
      తిరిగి FT_ABS( v.y );
    }
    లేకపోతే ఒకవేళ ( v.y == 0 )
    {
      తిరిగి FT_ABS( v.x );
    }

    /* general case */
    జరుపు = ft_trig_prenorm( &v );
    ft_trig_pseudo_polarize( &v );

    v.x = ft_trig_downscale( v.x );

    ఒకవేళ ( జరుపు > 0 )
      తిరిగి ( v.x + ( 1L << ( జరుపు - 1 ) ) ) >> జరుపు;

    తిరిగి (FT_Fixed)( (FT_UInt32)v.x << -జరుపు );
  }


  /* documentation is in fttrigon.h */

  FT_EXPORT_DEF( శూన్య )
  FT_సదిశ_Polarize( FT_సదిశ*  vec,
                      FT_Fixed   *నిడివి,
                      FT_Angle   *angle )
  {
    FT_Int     జరుపు;
    FT_సదిశ  v;


    ఒకవేళ ( !vec || !నిడివి || !angle )
      తిరిగి;

    v = *vec;

    ఒకవేళ ( v.x == 0 && v.y == 0 )
      తిరిగి;

    జరుపు = ft_trig_prenorm( &v );
    ft_trig_pseudo_polarize( &v );

    v.x = ft_trig_downscale( v.x );

    *నిడివి = జరుపు >= 0 ?                      ( v.x >>  జరుపు )
                         : (FT_Fixed)( (FT_UInt32)v.x << -జరుపు );
    *angle  = v.y;
  }


  /* documentation is in fttrigon.h */

  FT_EXPORT_DEF( శూన్య )
  FT_సదిశ_From_Polar( FT_సదిశ*  vec,
                        FT_Fixed    నిడివి,
                        FT_Angle    angle )
  {
    ఒకవేళ ( !vec )
      తిరిగి;

    vec->x = నిడివి;
    vec->y = 0;

    FT_సదిశ_Rotate( vec, angle );
  }


  /* documentation is in fttrigon.h */

  FT_EXPORT_DEF( FT_Angle )
  FT_Angle_Diff( FT_Angle  angle1,
                 FT_Angle  angle2 )
  {
    FT_Angle  delta = angle2 - angle1;


    అయితే ( delta <= -FT_ANGLE_PI )
      delta += FT_ANGLE_2PI;

    అయితే ( delta > FT_ANGLE_PI )
      delta -= FT_ANGLE_2PI;

    తిరిగి delta;
  }


/*చివర*/
