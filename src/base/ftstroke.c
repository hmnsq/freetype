/****************************************************************************
 *
 * ftstroke.c
 *
 *   FreeType path stroker (body).
 *
 * Copyright (C) 2002-2026 by
 * David Turner, Robert Wilhelm, and Werner Lemberg.
 *
 * This file is part of the FreeType project, and may only be used,
 * modified, and distributed under the terms of the FreeType project
 * license, LICENSE.TXT.  By continuing to use, modify, or distribute
 * this file you indicate that you have read the license and
 * understand and accept it fully.
 *
 */


#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/ftstroke.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/fttrigon.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/ftoutln.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftmemory.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftdebug.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftobjs.h>


  /* declare an extern to access `ft_outline_glyph_class' globally */
  /* allocated  in `ftglyph.c'                                     */
  FT_CALLBACK_ਸਾਰਣੀ ਅਚਲ FT_Glyph_Class  ft_outline_glyph_class;


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_StrokerBorder )
  FT_Outline_GetInsideBorder( FT_Outline*  outline )
  {
    FT_Orientation  o = FT_Outline_Get_Orientation( outline );


    ਵਾਪਸ o == FT_ORIENTATION_TRUETYPE ? FT_STROKER_BORDER_ਸੱਜਾ
                                        : FT_STROKER_BORDER_ਖੱਬਾ;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_StrokerBorder )
  FT_Outline_GetOutsideBorder( FT_Outline*  outline )
  {
    FT_Orientation  o = FT_Outline_Get_Orientation( outline );


    ਵਾਪਸ o == FT_ORIENTATION_TRUETYPE ? FT_STROKER_BORDER_ਖੱਬਾ
                                        : FT_STROKER_BORDER_ਸੱਜਾ;
  }


  /*************************************************************************/
  /*************************************************************************/
  /*****                                                               *****/
  /*****                      BEZIER COMPUTATIONS                      *****/
  /*****                                                               *****/
  /*************************************************************************/
  /*************************************************************************/

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FT_SMALL_CONIC_ਹੱਦ  ( FT_ANGLE_PI / 6 )
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FT_SMALL_CUBIC_ਹੱਦ  ( FT_ANGLE_PI / 8 )

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FT_EPSILON  2

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FT_IS_SMALL( x )  ( (x) > -FT_EPSILON && (x) < FT_EPSILON )


  ਸਥਿਰ FT_Pos
  ft_pos_abs( FT_Pos  x )
  {
    ਵਾਪਸ x >= 0 ? x : -x;
  }


  ਸਥਿਰ ਖਾਲੀ
  ft_conic_ਵੰਡੋ( FT_ਸਦਿਸ਼*  ਅਧਾਰ )
  {
    FT_Pos  a, b;


    ਅਧਾਰ[4].x = ਅਧਾਰ[2].x;
    a = ਅਧਾਰ[0].x + ਅਧਾਰ[1].x;
    b = ਅਧਾਰ[1].x + ਅਧਾਰ[2].x;
    ਅਧਾਰ[3].x = b >> 1;
    ਅਧਾਰ[2].x = ( a + b ) >> 2;
    ਅਧਾਰ[1].x = a >> 1;

    ਅਧਾਰ[4].y = ਅਧਾਰ[2].y;
    a = ਅਧਾਰ[0].y + ਅਧਾਰ[1].y;
    b = ਅਧਾਰ[1].y + ਅਧਾਰ[2].y;
    ਅਧਾਰ[3].y = b >> 1;
    ਅਧਾਰ[2].y = ( a + b ) >> 2;
    ਅਧਾਰ[1].y = a >> 1;
  }


  ਸਥਿਰ FT_Bool
  ft_conic_is_small_enough( FT_ਸਦਿਸ਼*  ਅਧਾਰ,
                            FT_Angle   *angle_ਵਿੱਚ,
                            FT_Angle   *angle_ਬਾਹਰਲਾ )
  {
    FT_ਸਦਿਸ਼  d1, d2;
    FT_Angle   theta;
    FT_Int     close1, close2;


    d1.x = ਅਧਾਰ[1].x - ਅਧਾਰ[2].x;
    d1.y = ਅਧਾਰ[1].y - ਅਧਾਰ[2].y;
    d2.x = ਅਧਾਰ[0].x - ਅਧਾਰ[1].x;
    d2.y = ਅਧਾਰ[0].y - ਅਧਾਰ[1].y;

    close1 = FT_IS_SMALL( d1.x ) && FT_IS_SMALL( d1.y );
    close2 = FT_IS_SMALL( d2.x ) && FT_IS_SMALL( d2.y );

    ਜੇ ( close1 )
    {
      ਜੇ ( close2 )
      {
        /* basically a point;                      */
        /* do nothing to retain original direction */
      }
      ਨਹੀਂਤਾਂ
      {
        *angle_ਵਿੱਚ  =
        *angle_ਬਾਹਰਲਾ = FT_Atan2( d2.x, d2.y );
      }
    }
    ਨਹੀਂਤਾਂ /* !close1 */
    {
      ਜੇ ( close2 )
      {
        *angle_ਵਿੱਚ  =
        *angle_ਬਾਹਰਲਾ = FT_Atan2( d1.x, d1.y );
      }
      ਨਹੀਂਤਾਂ
      {
        *angle_ਵਿੱਚ  = FT_Atan2( d1.x, d1.y );
        *angle_ਬਾਹਰਲਾ = FT_Atan2( d2.x, d2.y );
      }
    }

    theta = ft_pos_abs( FT_Angle_Diff( *angle_ਵਿੱਚ, *angle_ਬਾਹਰਲਾ ) );

    ਵਾਪਸ FT_BOOL( theta < FT_SMALL_CONIC_ਹੱਦ );
  }


  ਸਥਿਰ ਖਾਲੀ
  ft_cubic_ਵੰਡੋ( FT_ਸਦਿਸ਼*  ਅਧਾਰ )
  {
    FT_Pos  a, b, c;


    ਅਧਾਰ[6].x = ਅਧਾਰ[3].x;
    a = ਅਧਾਰ[0].x + ਅਧਾਰ[1].x;
    b = ਅਧਾਰ[1].x + ਅਧਾਰ[2].x;
    c = ਅਧਾਰ[2].x + ਅਧਾਰ[3].x;
    ਅਧਾਰ[5].x = c >> 1;
    c += b;
    ਅਧਾਰ[4].x = c >> 2;
    ਅਧਾਰ[1].x = a >> 1;
    a += b;
    ਅਧਾਰ[2].x = a >> 2;
    ਅਧਾਰ[3].x = ( a + c ) >> 3;

    ਅਧਾਰ[6].y = ਅਧਾਰ[3].y;
    a = ਅਧਾਰ[0].y + ਅਧਾਰ[1].y;
    b = ਅਧਾਰ[1].y + ਅਧਾਰ[2].y;
    c = ਅਧਾਰ[2].y + ਅਧਾਰ[3].y;
    ਅਧਾਰ[5].y = c >> 1;
    c += b;
    ਅਧਾਰ[4].y = c >> 2;
    ਅਧਾਰ[1].y = a >> 1;
    a += b;
    ਅਧਾਰ[2].y = a >> 2;
    ਅਧਾਰ[3].y = ( a + c ) >> 3;
  }


  /* Return the average of `angle1' and `angle2'.            */
  /* This gives correct result even if `angle1' and `angle2' */
  /* have opposite signs.                                    */
  ਸਥਿਰ FT_Angle
  ft_angle_ਔਸਤ( FT_Angle  angle1,
                 FT_Angle  angle2 )
  {
    ਵਾਪਸ angle1 + FT_Angle_Diff( angle1, angle2 ) / 2;
  }


  ਸਥਿਰ FT_Bool
  ft_cubic_is_small_enough( FT_ਸਦਿਸ਼*  ਅਧਾਰ,
                            FT_Angle   *angle_ਵਿੱਚ,
                            FT_Angle   *angle_mid,
                            FT_Angle   *angle_ਬਾਹਰਲਾ )
  {
    FT_ਸਦਿਸ਼  d1, d2, d3;
    FT_Angle   theta1, theta2;
    FT_Int     close1, close2, close3;


    d1.x = ਅਧਾਰ[2].x - ਅਧਾਰ[3].x;
    d1.y = ਅਧਾਰ[2].y - ਅਧਾਰ[3].y;
    d2.x = ਅਧਾਰ[1].x - ਅਧਾਰ[2].x;
    d2.y = ਅਧਾਰ[1].y - ਅਧਾਰ[2].y;
    d3.x = ਅਧਾਰ[0].x - ਅਧਾਰ[1].x;
    d3.y = ਅਧਾਰ[0].y - ਅਧਾਰ[1].y;

    close1 = FT_IS_SMALL( d1.x ) && FT_IS_SMALL( d1.y );
    close2 = FT_IS_SMALL( d2.x ) && FT_IS_SMALL( d2.y );
    close3 = FT_IS_SMALL( d3.x ) && FT_IS_SMALL( d3.y );

    ਜੇ ( close1 )
    {
      ਜੇ ( close2 )
      {
        ਜੇ ( close3 )
        {
          /* basically a point;                      */
          /* do nothing to retain original direction */
        }
        ਨਹੀਂਤਾਂ /* !close3 */
        {
          *angle_ਵਿੱਚ  =
          *angle_mid =
          *angle_ਬਾਹਰਲਾ = FT_Atan2( d3.x, d3.y );
        }
      }
      ਨਹੀਂਤਾਂ /* !close2 */
      {
        ਜੇ ( close3 )
        {
          *angle_ਵਿੱਚ  =
          *angle_mid =
          *angle_ਬਾਹਰਲਾ = FT_Atan2( d2.x, d2.y );
        }
        ਨਹੀਂਤਾਂ /* !close3 */
        {
          *angle_ਵਿੱਚ  =
          *angle_mid = FT_Atan2( d2.x, d2.y );
          *angle_ਬਾਹਰਲਾ = FT_Atan2( d3.x, d3.y );
        }
      }
    }
    ਨਹੀਂਤਾਂ /* !close1 */
    {
      ਜੇ ( close2 )
      {
        ਜੇ ( close3 )
        {
          *angle_ਵਿੱਚ  =
          *angle_mid =
          *angle_ਬਾਹਰਲਾ = FT_Atan2( d1.x, d1.y );
        }
        ਨਹੀਂਤਾਂ /* !close3 */
        {
          *angle_ਵਿੱਚ  = FT_Atan2( d1.x, d1.y );
          *angle_ਬਾਹਰਲਾ = FT_Atan2( d3.x, d3.y );
          *angle_mid = ft_angle_ਔਸਤ( *angle_ਵਿੱਚ, *angle_ਬਾਹਰਲਾ );
        }
      }
      ਨਹੀਂਤਾਂ /* !close2 */
      {
        ਜੇ ( close3 )
        {
          *angle_ਵਿੱਚ  = FT_Atan2( d1.x, d1.y );
          *angle_mid =
          *angle_ਬਾਹਰਲਾ = FT_Atan2( d2.x, d2.y );
        }
        ਨਹੀਂਤਾਂ /* !close3 */
        {
          *angle_ਵਿੱਚ  = FT_Atan2( d1.x, d1.y );
          *angle_mid = FT_Atan2( d2.x, d2.y );
          *angle_ਬਾਹਰਲਾ = FT_Atan2( d3.x, d3.y );
        }
      }
    }

    theta1 = ft_pos_abs( FT_Angle_Diff( *angle_ਵਿੱਚ,  *angle_mid ) );
    theta2 = ft_pos_abs( FT_Angle_Diff( *angle_mid, *angle_ਬਾਹਰਲਾ ) );

    ਵਾਪਸ FT_BOOL( theta1 < FT_SMALL_CUBIC_ਹੱਦ &&
                    theta2 < FT_SMALL_CUBIC_ਹੱਦ );
  }


  /*************************************************************************/
  /*************************************************************************/
  /*****                                                               *****/
  /*****                       STROKE BORDERS                          *****/
  /*****                                                               *****/
  /*************************************************************************/
  /*************************************************************************/

  ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਗਿਣਤੀ  FT_Stroke_ਟੈਗ_ਸੂਚੀ
  {
    FT_STROKE_ਟੈਗ_ON    = 1,   /* on-curve point  */
    FT_STROKE_ਟੈਗ_CUBIC = 2,   /* cubic off-point */
    FT_STROKE_ਟੈਗ_BEGIN = 4,   /* sub-path start  */
    FT_STROKE_ਟੈਗ_ਸਿਰਾ   = 8    /* sub-path end    */

  } FT_Stroke_ਟੈਗ_ਸੂਚੀ_2;

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ  FT_STROKE_ਟੈਗ_BEGIN_ਸਿਰਾ  ( FT_STROKE_ਟੈਗ_BEGIN | FT_STROKE_ਟੈਗ_ਸਿਰਾ )

  ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ  FT_StrokeBorderRec_
  {
    FT_UInt     ਅੰਕ_points;
    FT_UInt     max_points;
    FT_ਸਦਿਸ਼*  points;
    FT_Byte*    ਟੈਗ_ਸੂਚੀ;
    FT_Bool     movable;  /* TRUE for ends of lineto borders */
    FT_Int      ਸ਼ੁਰੂ;    /* index of current sub-path start point */
    FT_ਸਮ੍ਰਿਤੀ   ਸਮ੍ਰਿਤੀ;
    FT_Bool     valid;

  } FT_StrokeBorderRec, *FT_StrokeBorder;


  ਸਥਿਰ FT_Error
  ft_stroke_border_grow( FT_StrokeBorder  border,
                         FT_UInt          new_points )
  {
    FT_UInt   old_max = border->max_points;
    FT_UInt   new_max = border->ਅੰਕ_points + new_points;
    FT_Error  error   = FT_Err_Ok;


    ਜੇ ( new_max > old_max )
    {
      FT_UInt    cur_max = old_max;
      FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ  = border->ਸਮ੍ਰਿਤੀ;


      ਜਦੋਂ ( cur_max < new_max )
        cur_max += ( cur_max >> 1 ) + 16;

      ਜੇ ( FT_RENEW_ARRAY( border->points, old_max, cur_max ) ||
           FT_RENEW_ARRAY( border->ਟੈਗ_ਸੂਚੀ,   old_max, cur_max ) )
        ਜਾਓ Exit;

      border->max_points = cur_max;
    }

  Exit:
    ਵਾਪਸ error;
  }


  ਸਥਿਰ ਖਾਲੀ
  ft_stroke_border_close( FT_StrokeBorder  border,
                          FT_Bool          ਉਲਟ )
  {
    FT_UInt  ਸ਼ੁਰੂ = (FT_UInt)border->ਸ਼ੁਰੂ;
    FT_UInt  ਗਿਣਤੀ_2 = border->ਅੰਕ_points;


    FT_ASSERT( border->ਸ਼ੁਰੂ >= 0 );

    /* don't record empty paths! */
    ਜੇ ( ਗਿਣਤੀ_2 <= ਸ਼ੁਰੂ + 1U )
      border->ਅੰਕ_points = ਸ਼ੁਰੂ;
    ਨਹੀਂਤਾਂ
    {
      /* copy the last point to the start of this sub-path, since */
      /* it contains the `adjusted' starting coordinates          */
      border->ਅੰਕ_points    = --ਗਿਣਤੀ_2;
      border->points[ਸ਼ੁਰੂ] = border->points[ਗਿਣਤੀ_2];
      border->ਟੈਗ_ਸੂਚੀ[ਸ਼ੁਰੂ]   = border->ਟੈਗ_ਸੂਚੀ[ਗਿਣਤੀ_2];

      ਜੇ ( ਉਲਟ )
      {
        /* reverse the points */
        {
          FT_ਸਦਿਸ਼*  vec1 = border->points + ਸ਼ੁਰੂ + 1;
          FT_ਸਦਿਸ਼*  vec2 = border->points + ਗਿਣਤੀ_2 - 1;


          ਲਈ ( ; vec1 < vec2; vec1++, vec2-- )
          {
            FT_ਸਦਿਸ਼  tmp;


            tmp   = *vec1;
            *vec1 = *vec2;
            *vec2 = tmp;
          }
        }

        /* then the tags */
        {
          FT_Byte*  tag1 = border->ਟੈਗ_ਸੂਚੀ + ਸ਼ੁਰੂ + 1;
          FT_Byte*  tag2 = border->ਟੈਗ_ਸੂਚੀ + ਗਿਣਤੀ_2 - 1;


          ਲਈ ( ; tag1 < tag2; tag1++, tag2-- )
          {
            FT_Byte  tmp;


            tmp   = *tag1;
            *tag1 = *tag2;
            *tag2 = tmp;
          }
        }
      }

      border->ਟੈਗ_ਸੂਚੀ[ਸ਼ੁਰੂ    ] |= FT_STROKE_ਟੈਗ_BEGIN;
      border->ਟੈਗ_ਸੂਚੀ[ਗਿਣਤੀ_2 - 1] |= FT_STROKE_ਟੈਗ_ਸਿਰਾ;
    }

    border->ਸ਼ੁਰੂ   = -1;
    border->movable = FALSE;
  }


  ਸਥਿਰ FT_Error
  ft_stroke_border_lineto( FT_StrokeBorder  border,
                           FT_ਸਦਿਸ਼*       ਮੰਜ਼ਿਲ,
                           FT_Bool          movable )
  {
    FT_Error  error = FT_Err_Ok;


    FT_ASSERT( border->ਸ਼ੁਰੂ >= 0 );

    ਜੇ ( border->movable )
    {
      /* move last point */
      border->points[border->ਅੰਕ_points - 1] = *ਮੰਜ਼ਿਲ;
    }
    ਨਹੀਂਤਾਂ
    {
      /* don't add zero-length lineto, but always add moveto */
      ਜੇ ( border->ਅੰਕ_points > (FT_UInt)border->ਸ਼ੁਰੂ                     &&
           FT_IS_SMALL( border->points[border->ਅੰਕ_points - 1].x - ਮੰਜ਼ਿਲ->x ) &&
           FT_IS_SMALL( border->points[border->ਅੰਕ_points - 1].y - ਮੰਜ਼ਿਲ->y ) )
        ਵਾਪਸ error;

      /* add one point */
      error = ft_stroke_border_grow( border, 1 );
      ਜੇ ( !error )
      {
        FT_ਸਦਿਸ਼*  vec = border->points + border->ਅੰਕ_points;
        FT_Byte*    ਟੈਗ = border->ਟੈਗ_ਸੂਚੀ   + border->ਅੰਕ_points;


        vec[0] = *ਮੰਜ਼ਿਲ;
        ਟੈਗ[0] = FT_STROKE_ਟੈਗ_ON;

        border->ਅੰਕ_points += 1;
      }
    }
    border->movable = movable;
    ਵਾਪਸ error;
  }


  ਸਥਿਰ FT_Error
  ft_stroke_border_conicto( FT_StrokeBorder  border,
                            FT_ਸਦਿਸ਼*       control,
                            FT_ਸਦਿਸ਼*       ਮੰਜ਼ਿਲ )
  {
    FT_Error  error;


    FT_ASSERT( border->ਸ਼ੁਰੂ >= 0 );

    error = ft_stroke_border_grow( border, 2 );
    ਜੇ ( !error )
    {
      FT_ਸਦਿਸ਼*  vec = border->points + border->ਅੰਕ_points;
      FT_Byte*    ਟੈਗ = border->ਟੈਗ_ਸੂਚੀ   + border->ਅੰਕ_points;


      vec[0] = *control;
      vec[1] = *ਮੰਜ਼ਿਲ;

      ਟੈਗ[0] = 0;
      ਟੈਗ[1] = FT_STROKE_ਟੈਗ_ON;

      border->ਅੰਕ_points += 2;
    }

    border->movable = FALSE;

    ਵਾਪਸ error;
  }


  ਸਥਿਰ FT_Error
  ft_stroke_border_cubicto( FT_StrokeBorder  border,
                            FT_ਸਦਿਸ਼*       control1,
                            FT_ਸਦਿਸ਼*       control2,
                            FT_ਸਦਿਸ਼*       ਮੰਜ਼ਿਲ )
  {
    FT_Error  error;


    FT_ASSERT( border->ਸ਼ੁਰੂ >= 0 );

    error = ft_stroke_border_grow( border, 3 );
    ਜੇ ( !error )
    {
      FT_ਸਦਿਸ਼*  vec = border->points + border->ਅੰਕ_points;
      FT_Byte*    ਟੈਗ = border->ਟੈਗ_ਸੂਚੀ   + border->ਅੰਕ_points;


      vec[0] = *control1;
      vec[1] = *control2;
      vec[2] = *ਮੰਜ਼ਿਲ;

      ਟੈਗ[0] = FT_STROKE_ਟੈਗ_CUBIC;
      ਟੈਗ[1] = FT_STROKE_ਟੈਗ_CUBIC;
      ਟੈਗ[2] = FT_STROKE_ਟੈਗ_ON;

      border->ਅੰਕ_points += 3;
    }

    border->movable = FALSE;

    ਵਾਪਸ error;
  }


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FT_ARC_CUBIC_ANGLE  ( FT_ANGLE_PI / 2 )


  ਸਥਿਰ FT_Error
  ft_stroke_border_arcto( FT_StrokeBorder  border,
                          FT_ਸਦਿਸ਼*       center,
                          FT_Fixed         radius,
                          FT_Angle         angle_ਸ਼ੁਰੂ,
                          FT_Angle         angle_diff )
  {
    FT_Fixed   coef;
    FT_ਸਦਿਸ਼  a0, a1, a2, a3;
    FT_Int     i, arcs = 1;
    FT_Error   error = FT_Err_Ok;


    /* number of cubic arcs to draw */
    ਜਦੋਂ (  angle_diff > FT_ARC_CUBIC_ANGLE * arcs ||
            -angle_diff > FT_ARC_CUBIC_ANGLE * arcs )
      arcs++;

    /* control tangents */
    coef  = FT_Tan( angle_diff / ( 4 * arcs ) );
    coef += coef / 3;

    /* compute start and first control point */
    FT_ਸਦਿਸ਼_From_Polar( &a0, radius, angle_ਸ਼ੁਰੂ );
    a1.x = FT_MulFix( -a0.y, coef );
    a1.y = FT_MulFix(  a0.x, coef );

    a0.x += center->x;
    a0.y += center->y;
    a1.x += a0.x;
    a1.y += a0.y;

    ਲਈ ( i = 1; i <= arcs; i++ )
    {
      /* compute end and second control point */
      FT_ਸਦਿਸ਼_From_Polar( &a3, radius,
                            angle_ਸ਼ੁਰੂ + i * angle_diff / arcs );
      a2.x = FT_MulFix(  a3.y, coef );
      a2.y = FT_MulFix( -a3.x, coef );

      a3.x += center->x;
      a3.y += center->y;
      a2.x += a3.x;
      a2.y += a3.y;

      /* add cubic arc */
      error = ft_stroke_border_cubicto( border, &a1, &a2, &a3 );
      ਜੇ ( error )
        ਰੋਕੋ;

      /* a0 = a3; */
      a1.x = a3.x - a2.x + a3.x;
      a1.y = a3.y - a2.y + a3.y;
    }

    ਵਾਪਸ error;
  }


  ਸਥਿਰ FT_Error
  ft_stroke_border_moveto( FT_StrokeBorder  border,
                           FT_ਸਦਿਸ਼*       ਮੰਜ਼ਿਲ )
  {
    /* close current open path if any ? */
    ਜੇ ( border->ਸ਼ੁਰੂ >= 0 )
      ft_stroke_border_close( border, FALSE );

    border->ਸ਼ੁਰੂ = (FT_Int)border->ਅੰਕ_points;
    border->movable = FALSE;

    ਵਾਪਸ ft_stroke_border_lineto( border, ਮੰਜ਼ਿਲ, FALSE );
  }


  ਸਥਿਰ ਖਾਲੀ
  ft_stroke_border_init( FT_StrokeBorder  border,
                         FT_ਸਮ੍ਰਿਤੀ        ਸਮ੍ਰਿਤੀ )
  {
    border->ਸਮ੍ਰਿਤੀ = ਸਮ੍ਰਿਤੀ;
    border->points = ਨਿਰਾ;
    border->ਟੈਗ_ਸੂਚੀ   = ਨਿਰਾ;

    border->ਅੰਕ_points = 0;
    border->max_points = 0;
    border->ਸ਼ੁਰੂ      = -1;
    border->valid      = FALSE;
  }


  ਸਥਿਰ ਖਾਲੀ
  ft_stroke_border_reset( FT_StrokeBorder  border )
  {
    border->ਅੰਕ_points = 0;
    border->ਸ਼ੁਰੂ      = -1;
    border->valid      = FALSE;
  }


  ਸਥਿਰ ਖਾਲੀ
  ft_stroke_border_ਪੂਰਾ( FT_StrokeBorder  border )
  {
    FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ = border->ਸਮ੍ਰਿਤੀ;


    FT_FREE( border->points );
    FT_FREE( border->ਟੈਗ_ਸੂਚੀ );

    border->ਅੰਕ_points = 0;
    border->max_points = 0;
    border->ਸ਼ੁਰੂ      = -1;
    border->valid      = FALSE;
  }


  ਸਥਿਰ FT_Error
  ft_stroke_border_get_counts( FT_StrokeBorder  border,
                               FT_UInt         *anum_points,
                               FT_UInt         *anum_contours )
  {
    FT_Error  error        = FT_Err_Ok;
    FT_UInt   ਅੰਕ_points   = 0;
    FT_UInt   ਅੰਕ_contours = 0;

    FT_UInt     ਗਿਣਤੀ_2      = border->ਅੰਕ_points;
    FT_Byte*    ਟੈਗ_ਸੂਚੀ       = border->ਟੈਗ_ਸੂਚੀ;
    FT_Int      ਵਿੱਚ_contour = 0;


    ਲਈ ( ; ਗਿਣਤੀ_2 > 0; ਗਿਣਤੀ_2--, ਅੰਕ_points++, ਟੈਗ_ਸੂਚੀ++ )
    {
      ਜੇ ( ਟੈਗ_ਸੂਚੀ[0] & FT_STROKE_ਟੈਗ_BEGIN )
      {
        ਜੇ ( ਵਿੱਚ_contour != 0 )
          ਜਾਓ Fail;

        ਵਿੱਚ_contour = 1;
      }
      ਨਹੀਂਤਾਂ ਜੇ ( ਵਿੱਚ_contour == 0 )
        ਜਾਓ Fail;

      ਜੇ ( ਟੈਗ_ਸੂਚੀ[0] & FT_STROKE_ਟੈਗ_ਸਿਰਾ )
      {
        ਵਿੱਚ_contour = 0;
        ਅੰਕ_contours++;
      }
    }

    ਜੇ ( ਵਿੱਚ_contour != 0 )
      ਜਾਓ Fail;

    border->valid = TRUE;

  Exit:
    *anum_points   = ਅੰਕ_points;
    *anum_contours = ਅੰਕ_contours;
    ਵਾਪਸ error;

  Fail:
    ਅੰਕ_points   = 0;
    ਅੰਕ_contours = 0;
    error        = FT_THROW( Invalid_Outline );
    ਜਾਓ Exit;
  }


  ਸਥਿਰ ਖਾਲੀ
  ft_stroke_border_export( FT_StrokeBorder  border,
                           FT_Outline*      outline )
  {
    /* copy point locations */
    ਜੇ ( border->ਅੰਕ_points )
      FT_ARRAY_COPY( outline->points + outline->n_points,
                     border->points,
                     border->ਅੰਕ_points );

    /* copy tags */
    {
      FT_UInt   ਗਿਣਤੀ_2 = border->ਅੰਕ_points;
      FT_Byte*  ਪਠ  = border->ਟੈਗ_ਸੂਚੀ;
      FT_Byte*  ਲਿਖ = outline->ਟੈਗ_ਸੂਚੀ + outline->n_points;


      ਲਈ ( ; ਗਿਣਤੀ_2 > 0; ਗਿਣਤੀ_2--, ਪਠ++, ਲਿਖ++ )
      {
        ਜੇ ( *ਪਠ & FT_STROKE_ਟੈਗ_ON )
          *ਲਿਖ = FT_ਵਕਰ_ਟੈਗ_ON;
        ਨਹੀਂਤਾਂ ਜੇ ( *ਪਠ & FT_STROKE_ਟੈਗ_CUBIC )
          *ਲਿਖ = FT_ਵਕਰ_ਟੈਗ_CUBIC;
        ਨਹੀਂਤਾਂ
          *ਲਿਖ = FT_ਵਕਰ_ਟੈਗ_CONIC;
      }
    }

    /* copy contours */
    {
      FT_UInt     ਗਿਣਤੀ_2 = border->ਅੰਕ_points;
      FT_Byte*    ਟੈਗ_ਸੂਚੀ  = border->ਟੈਗ_ਸੂਚੀ;
      FT_UShort*  ਲਿਖ = outline->contours + outline->n_contours;
      FT_UShort   idx   = outline->n_points;


      ਲਈ ( ; ਗਿਣਤੀ_2 > 0; ਗਿਣਤੀ_2--, ਟੈਗ_ਸੂਚੀ++, idx++ )
      {
        ਜੇ ( *ਟੈਗ_ਸੂਚੀ & FT_STROKE_ਟੈਗ_ਸਿਰਾ )
        {
          *ਲਿਖ++ = idx;
          outline->n_contours++;
        }
      }
    }

    outline->n_points += (FT_UShort)border->ਅੰਕ_points;

    FT_ASSERT( FT_Outline_Check( outline ) == 0 );
  }


  /*************************************************************************/
  /*************************************************************************/
  /*****                                                               *****/
  /*****                           STROKER                             *****/
  /*****                                                               *****/
  /*************************************************************************/
  /*************************************************************************/

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FT_SIDE_ਮੰਜ਼ਿਲ_ROTATE( s )   ( FT_ANGLE_PI2 - (s) * FT_ANGLE_PI )

  ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ  FT_StrokerRec_
  {
    FT_Angle             angle_ਵਿੱਚ;             /* direction into curr join */
    FT_Angle             angle_ਬਾਹਰਲਾ;            /* direction out of join  */
    FT_ਸਦਿਸ਼            center;               /* current position */
    FT_Fixed             ਸਤਰ_ਲੰਮਾਈ;          /* length of last lineto */
    FT_Bool              ਪਹਿਲਾ_ਬਿੰਦੂ;          /* is this the start? */
    FT_Bool              subpath_open;         /* is the subpath open? */
    FT_Angle             subpath_angle;        /* subpath start direction */
    FT_ਸਦਿਸ਼            subpath_ਸ਼ੁਰੂ;        /* subpath start position */
    FT_Fixed             subpath_ਸਤਰ_ਲੰਮਾਈ;  /* subpath start lineto len */
    FT_Bool              handle_wide_strokes;  /* use wide strokes logic? */

    FT_Stroker_ਸਤਰ_Cap   ਸਤਰ_cap;
    FT_Stroker_ਸਤਰ_Join  ਸਤਰ_join;
    FT_Stroker_ਸਤਰ_Join  ਸਤਰ_join_saved;
    FT_Fixed             miter_ਹੱਦਬੰਦੀ;
    FT_Fixed             radius;

    FT_StrokeBorderRec   borders[2];
    FT_Library           library;

  } FT_StrokerRec;


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Stroker_New( FT_Library   library,
                  FT_Stroker  *astroker )
  {
    FT_Error    error;           /* assigned in FT_NEW */
    FT_ਸਮ੍ਰਿਤੀ   ਸਮ੍ਰਿਤੀ;
    FT_Stroker  stroker = ਨਿਰਾ;


    ਜੇ ( !library )
      ਵਾਪਸ FT_THROW( Invalid_Library_Handle );

    ਜੇ ( !astroker )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    ਸਮ੍ਰਿਤੀ = library->ਸਮ੍ਰਿਤੀ;

    ਜੇ ( !FT_NEW( stroker ) )
    {
      stroker->library = library;

      ft_stroke_border_init( &stroker->borders[0], ਸਮ੍ਰਿਤੀ );
      ft_stroke_border_init( &stroker->borders[1], ਸਮ੍ਰਿਤੀ );
    }

    *astroker = stroker;

    ਵਾਪਸ error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( ਖਾਲੀ )
  FT_Stroker_ਸੈੱਟ( FT_Stroker           stroker,
                  FT_Fixed             radius,
                  FT_Stroker_ਸਤਰ_Cap   ਸਤਰ_cap,
                  FT_Stroker_ਸਤਰ_Join  ਸਤਰ_join,
                  FT_Fixed             miter_ਹੱਦਬੰਦੀ )
  {
    ਜੇ ( !stroker )
      ਵਾਪਸ;

    stroker->radius      = radius;
    stroker->ਸਤਰ_cap    = ਸਤਰ_cap;
    stroker->ਸਤਰ_join   = ਸਤਰ_join;
    stroker->miter_ਹੱਦਬੰਦੀ = miter_ਹੱਦਬੰਦੀ;

    /* ensure miter limit has sensible value */
    ਜੇ ( stroker->miter_ਹੱਦਬੰਦੀ < 0x10000L )
      stroker->miter_ਹੱਦਬੰਦੀ = 0x10000L;

    /* save line join style:                                           */
    /* line join style can be temporarily changed when stroking curves */
    stroker->ਸਤਰ_join_saved = ਸਤਰ_join;

    FT_Stroker_Rewind( stroker );
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( ਖਾਲੀ )
  FT_Stroker_Rewind( FT_Stroker  stroker )
  {
    ਜੇ ( stroker )
    {
      ft_stroke_border_reset( &stroker->borders[0] );
      ft_stroke_border_reset( &stroker->borders[1] );
    }
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( ਖਾਲੀ )
  FT_Stroker_ਪੂਰਾ( FT_Stroker  stroker )
  {
    ਜੇ ( stroker )
    {
      FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ = stroker->library->ਸਮ੍ਰਿਤੀ;


      ft_stroke_border_ਪੂਰਾ( &stroker->borders[0] );
      ft_stroke_border_ਪੂਰਾ( &stroker->borders[1] );

      stroker->library = ਨਿਰਾ;
      FT_FREE( stroker );
    }
  }


  /* create a circular arc at a corner or cap */
  ਸਥਿਰ FT_Error
  ft_stroker_arcto( FT_Stroker  stroker,
                    FT_Int      side )
  {
    FT_Angle         ਕੁੱਲ, rotate;
    FT_Fixed         radius = stroker->radius;
    FT_Error         error  = FT_Err_Ok;
    FT_StrokeBorder  border = stroker->borders + side;


    rotate = FT_SIDE_ਮੰਜ਼ਿਲ_ROTATE( side );

    ਕੁੱਲ = FT_Angle_Diff( stroker->angle_ਵਿੱਚ, stroker->angle_ਬਾਹਰਲਾ );
    ਜੇ ( ਕੁੱਲ == FT_ANGLE_PI )
      ਕੁੱਲ = -rotate * 2;

    error = ft_stroke_border_arcto( border,
                                    &stroker->center,
                                    radius,
                                    stroker->angle_ਵਿੱਚ + rotate,
                                    ਕੁੱਲ );
    border->movable = FALSE;
    ਵਾਪਸ error;
  }


  /* add a cap at the end of an opened path */
  ਸਥਿਰ FT_Error
  ft_stroker_cap( FT_Stroker  stroker,
                  FT_Angle    angle,
                  FT_Int      side )
  {
    FT_Error  error = FT_Err_Ok;


    ਜੇ ( stroker->ਸਤਰ_cap == FT_STROKER_LINECAP_ਗੇੜਾ )
    {
      /* add a round cap */
      stroker->angle_ਵਿੱਚ  = angle;
      stroker->angle_ਬਾਹਰਲਾ = angle + FT_ANGLE_PI;

      error = ft_stroker_arcto( stroker, side );
    }
    ਨਹੀਂਤਾਂ
    {
      /* add a square or butt cap */
      FT_ਸਦਿਸ਼        ਵਿਚਕਾਰ, delta;
      FT_Fixed         radius = stroker->radius;
      FT_StrokeBorder  border = stroker->borders + side;


      /* compute middle point and first angle point */
      FT_ਸਦਿਸ਼_From_Polar( &ਵਿਚਕਾਰ, radius, angle );
      delta.x = side ?  ਵਿਚਕਾਰ.y : -ਵਿਚਕਾਰ.y;
      delta.y = side ? -ਵਿਚਕਾਰ.x :  ਵਿਚਕਾਰ.x;

      ਜੇ ( stroker->ਸਤਰ_cap == FT_STROKER_LINECAP_ਵਰਗਫਲ )
      {
        ਵਿਚਕਾਰ.x += stroker->center.x;
        ਵਿਚਕਾਰ.y += stroker->center.y;
      }
      ਨਹੀਂਤਾਂ  /* FT_STROKER_LINECAP_BUTT */
      {
        ਵਿਚਕਾਰ.x  = stroker->center.x;
        ਵਿਚਕਾਰ.y  = stroker->center.y;
      }

      delta.x  += ਵਿਚਕਾਰ.x;
      delta.y  += ਵਿਚਕਾਰ.y;

      error = ft_stroke_border_lineto( border, &delta, FALSE );
      ਜੇ ( error )
        ਜਾਓ Exit;

      /* compute second angle point */
      delta.x = ਵਿਚਕਾਰ.x - delta.x + ਵਿਚਕਾਰ.x;
      delta.y = ਵਿਚਕਾਰ.y - delta.y + ਵਿਚਕਾਰ.y;

      error = ft_stroke_border_lineto( border, &delta, FALSE );
    }

  Exit:
    ਵਾਪਸ error;
  }


  /* process an inside corner, i.e. compute intersection */
  ਸਥਿਰ FT_Error
  ft_stroker_inside( FT_Stroker  stroker,
                     FT_Int      side,
                     FT_Fixed    ਸਤਰ_ਲੰਮਾਈ )
  {
    FT_StrokeBorder  border = stroker->borders + side;
    FT_Angle         phi, theta, rotate;
    FT_Fixed         ਲੰਮਾਈ;
    FT_ਸਦਿਸ਼        sigma = { 0, 0 };
    FT_ਸਦਿਸ਼        delta;
    FT_Error         error = FT_Err_Ok;
    FT_Bool          intersect;          /* use intersection of lines? */


    rotate = FT_SIDE_ਮੰਜ਼ਿਲ_ROTATE( side );

    theta = FT_Angle_Diff( stroker->angle_ਵਿੱਚ, stroker->angle_ਬਾਹਰਲਾ ) / 2;

    /* Only intersect borders if between two lineto's and both */
    /* lines are long enough (line_length is zero for curves). */
    /* Also avoid U-turns of nearly 180 degree.                */
    ਜੇ ( !border->movable || ਸਤਰ_ਲੰਮਾਈ == 0  ||
         theta > 0x59C000 || theta < -0x59C000 )
      intersect = FALSE;
    ਨਹੀਂਤਾਂ
    {
      /* compute minimum required length of lines */
      FT_Fixed  min_ਲੰਮਾਈ;


      FT_ਸਦਿਸ਼_Unit( &sigma, theta );
      min_ਲੰਮਾਈ =
        ft_pos_abs( FT_MulDiv( stroker->radius, sigma.y, sigma.x ) );

      intersect = FT_BOOL( min_ਲੰਮਾਈ                         &&
                           stroker->ਸਤਰ_ਲੰਮਾਈ >= min_ਲੰਮਾਈ &&
                           ਸਤਰ_ਲੰਮਾਈ          >= min_ਲੰਮਾਈ );
    }

    ਜੇ ( !intersect )
    {
      FT_ਸਦਿਸ਼_From_Polar( &delta, stroker->radius,
                            stroker->angle_ਬਾਹਰਲਾ + rotate );
      delta.x += stroker->center.x;
      delta.y += stroker->center.y;

      border->movable = FALSE;
    }
    ਨਹੀਂਤਾਂ
    {
      /* compute median angle */
      phi = stroker->angle_ਵਿੱਚ + theta + rotate;

      ਲੰਮਾਈ = FT_DivFix( stroker->radius, sigma.x );

      FT_ਸਦਿਸ਼_From_Polar( &delta, ਲੰਮਾਈ, phi );
      delta.x += stroker->center.x;
      delta.y += stroker->center.y;
    }

    error = ft_stroke_border_lineto( border, &delta, FALSE );

    ਵਾਪਸ error;
  }


  /* process an outside corner, i.e. compute bevel/miter/round */
  ਸਥਿਰ FT_Error
  ft_stroker_outside( FT_Stroker  stroker,
                      FT_Int      side,
                      FT_Fixed    ਸਤਰ_ਲੰਮਾਈ )
  {
    FT_StrokeBorder  border = stroker->borders + side;
    FT_Error         error;
    FT_Angle         rotate;


    ਜੇ ( stroker->ਸਤਰ_join == FT_STROKER_LINEJOIN_ਗੇੜਾ )
      error = ft_stroker_arcto( stroker, side );
    ਨਹੀਂਤਾਂ
    {
      /* this is a mitered (pointed) or beveled (truncated) corner */
      FT_Fixed   radius = stroker->radius;
      FT_ਸਦਿਸ਼  sigma = { 0, 0 };
      FT_Angle   theta = 0, phi = 0;
      FT_Bool    bevel, fixed_bevel;


      rotate = FT_SIDE_ਮੰਜ਼ਿਲ_ROTATE( side );

      bevel =
        FT_BOOL( stroker->ਸਤਰ_join == FT_STROKER_LINEJOIN_BEVEL );

      fixed_bevel =
        FT_BOOL( stroker->ਸਤਰ_join != FT_STROKER_LINEJOIN_MITER_VARIABLE );

      /* check miter limit first */
      ਜੇ ( !bevel )
      {
        theta = FT_Angle_Diff( stroker->angle_ਵਿੱਚ, stroker->angle_ਬਾਹਰਲਾ ) / 2;

        ਜੇ ( theta == FT_ANGLE_PI2 )
          theta = -rotate;

        phi = stroker->angle_ਵਿੱਚ + theta + rotate;

        FT_ਸਦਿਸ਼_From_Polar( &sigma, stroker->miter_ਹੱਦਬੰਦੀ, theta );

        /* is miter limit exceeded? */
        ਜੇ ( sigma.x < 0x10000L )
        {
          /* don't create variable bevels for very small deviations; */
          /* FT_Sin(x) = 0 for x <= 57                               */
          ਜੇ ( fixed_bevel || ft_pos_abs( theta ) > 57 )
            bevel = TRUE;
        }
      }

      ਜੇ ( bevel )  /* this is a bevel (broken angle) */
      {
        ਜੇ ( fixed_bevel )
        {
          /* the outer corners are simply joined together */
          FT_ਸਦਿਸ਼  delta;


          /* add bevel */
          FT_ਸਦਿਸ਼_From_Polar( &delta,
                                radius,
                                stroker->angle_ਬਾਹਰਲਾ + rotate );
          delta.x += stroker->center.x;
          delta.y += stroker->center.y;

          border->movable = FALSE;
          error = ft_stroke_border_lineto( border, &delta, FALSE );
        }
        ਨਹੀਂਤਾਂ /* variable bevel or clipped miter */
        {
          /* the miter is truncated */
          FT_ਸਦਿਸ਼  ਵਿਚਕਾਰ, delta;
          FT_Fixed   coef;


          /* compute middle point and first angle point */
          FT_ਸਦਿਸ਼_From_Polar( &ਵਿਚਕਾਰ,
                                FT_MulFix( radius, stroker->miter_ਹੱਦਬੰਦੀ ),
                                phi );

          coef    = FT_DivFix(  0x10000L - sigma.x, sigma.y );
          delta.x = FT_MulFix(  ਵਿਚਕਾਰ.y, coef );
          delta.y = FT_MulFix( -ਵਿਚਕਾਰ.x, coef );

          ਵਿਚਕਾਰ.x += stroker->center.x;
          ਵਿਚਕਾਰ.y += stroker->center.y;
          delta.x  += ਵਿਚਕਾਰ.x;
          delta.y  += ਵਿਚਕਾਰ.y;

          error = ft_stroke_border_lineto( border, &delta, FALSE );
          ਜੇ ( error )
            ਜਾਓ Exit;

          /* compute second angle point */
          delta.x = ਵਿਚਕਾਰ.x - delta.x + ਵਿਚਕਾਰ.x;
          delta.y = ਵਿਚਕਾਰ.y - delta.y + ਵਿਚਕਾਰ.y;

          error = ft_stroke_border_lineto( border, &delta, FALSE );
          ਜੇ ( error )
            ਜਾਓ Exit;

          /* finally, add an end point; only needed if not lineto */
          /* (line_length is zero for curves)                     */
          ਜੇ ( ਸਤਰ_ਲੰਮਾਈ == 0 )
          {
            FT_ਸਦਿਸ਼_From_Polar( &delta,
                                  radius,
                                  stroker->angle_ਬਾਹਰਲਾ + rotate );

            delta.x += stroker->center.x;
            delta.y += stroker->center.y;

            error = ft_stroke_border_lineto( border, &delta, FALSE );
          }
        }
      }
      ਨਹੀਂਤਾਂ /* this is a miter (intersection) */
      {
        FT_Fixed   ਲੰਮਾਈ;
        FT_ਸਦਿਸ਼  delta;


        ਲੰਮਾਈ = FT_MulDiv( stroker->radius, stroker->miter_ਹੱਦਬੰਦੀ, sigma.x );

        FT_ਸਦਿਸ਼_From_Polar( &delta, ਲੰਮਾਈ, phi );
        delta.x += stroker->center.x;
        delta.y += stroker->center.y;

        error = ft_stroke_border_lineto( border, &delta, FALSE );
        ਜੇ ( error )
          ਜਾਓ Exit;

        /* now add an end point; only needed if not lineto */
        /* (line_length is zero for curves)                */
        ਜੇ ( ਸਤਰ_ਲੰਮਾਈ == 0 )
        {
          FT_ਸਦਿਸ਼_From_Polar( &delta,
                                stroker->radius,
                                stroker->angle_ਬਾਹਰਲਾ + rotate );
          delta.x += stroker->center.x;
          delta.y += stroker->center.y;

          error = ft_stroke_border_lineto( border, &delta, FALSE );
        }
      }
    }

  Exit:
    ਵਾਪਸ error;
  }


  ਸਥਿਰ FT_Error
  ft_stroker_process_ਕੋਨਾ( FT_Stroker  stroker,
                             FT_Fixed    ਸਤਰ_ਲੰਮਾਈ )
  {
    FT_Error  error = FT_Err_Ok;
    FT_Angle  turn;
    FT_Int    inside_side;


    turn = FT_Angle_Diff( stroker->angle_ਵਿੱਚ, stroker->angle_ਬਾਹਰਲਾ );

    /* no specific corner processing is required if the turn is 0 */
    ਜੇ ( turn == 0 )
      ਜਾਓ Exit;

    /* when we turn to the right, the inside side is 0 */
    /* otherwise, the inside side is 1 */
    inside_side = ( turn < 0 );

    /* process the inside side */
    error = ft_stroker_inside( stroker, inside_side, ਸਤਰ_ਲੰਮਾਈ );
    ਜੇ ( error )
      ਜਾਓ Exit;

    /* process the outside side */
    error = ft_stroker_outside( stroker, !inside_side, ਸਤਰ_ਲੰਮਾਈ );

  Exit:
    ਵਾਪਸ error;
  }


  /* add two points to the left and right borders corresponding to the */
  /* start of the subpath                                              */
  ਸਥਿਰ FT_Error
  ft_stroker_subpath_ਸ਼ੁਰੂ( FT_Stroker  stroker,
                            FT_Angle    ਸ਼ੁਰੂ_angle,
                            FT_Fixed    ਸਤਰ_ਲੰਮਾਈ )
  {
    FT_ਸਦਿਸ਼        delta;
    FT_ਸਦਿਸ਼        ਬਿੰਦੂ;
    FT_Error         error;
    FT_StrokeBorder  border;


    FT_ਸਦਿਸ਼_From_Polar( &delta, stroker->radius,
                          ਸ਼ੁਰੂ_angle + FT_ANGLE_PI2 );

    ਬਿੰਦੂ.x = stroker->center.x + delta.x;
    ਬਿੰਦੂ.y = stroker->center.y + delta.y;

    border = stroker->borders;
    error = ft_stroke_border_moveto( border, &ਬਿੰਦੂ );
    ਜੇ ( error )
      ਜਾਓ Exit;

    ਬਿੰਦੂ.x = stroker->center.x - delta.x;
    ਬਿੰਦੂ.y = stroker->center.y - delta.y;

    border++;
    error = ft_stroke_border_moveto( border, &ਬਿੰਦੂ );

    /* save angle, position, and line length for last join */
    /* (line_length is zero for curves)                    */
    stroker->subpath_angle       = ਸ਼ੁਰੂ_angle;
    stroker->ਪਹਿਲਾ_ਬਿੰਦੂ         = FALSE;
    stroker->subpath_ਸਤਰ_ਲੰਮਾਈ = ਸਤਰ_ਲੰਮਾਈ;

  Exit:
    ਵਾਪਸ error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Stroker_ਸਤਰ_ਮੰਜ਼ਿਲ( FT_Stroker  stroker,
                     FT_ਸਦਿਸ਼*  ਮੰਜ਼ਿਲ )
  {
    FT_Error         error = FT_Err_Ok;
    FT_StrokeBorder  border;
    FT_ਸਦਿਸ਼        delta;
    FT_Angle         angle;
    FT_Int           side;
    FT_Fixed         ਸਤਰ_ਲੰਮਾਈ;


    ਜੇ ( !stroker || !ਮੰਜ਼ਿਲ )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    delta.x = ਮੰਜ਼ਿਲ->x - stroker->center.x;
    delta.y = ਮੰਜ਼ਿਲ->y - stroker->center.y;

    /* a zero-length lineto is a no-op; avoid creating a spurious corner */
    ਜੇ ( delta.x == 0 && delta.y == 0 )
       ਜਾਓ Exit;

    /* compute length of line */
    ਸਤਰ_ਲੰਮਾਈ = FT_ਸਦਿਸ਼_ਲੰਮਾਈ( &delta );

    angle = FT_Atan2( delta.x, delta.y );
    FT_ਸਦਿਸ਼_From_Polar( &delta, stroker->radius, angle + FT_ANGLE_PI2 );

    /* process corner if necessary */
    ਜੇ ( stroker->ਪਹਿਲਾ_ਬਿੰਦੂ )
    {
      /* This is the first segment of a subpath.  We need to     */
      /* add a point to each border at their respective starting */
      /* point locations.                                        */
      error = ft_stroker_subpath_ਸ਼ੁਰੂ( stroker, angle, ਸਤਰ_ਲੰਮਾਈ );
      ਜੇ ( error )
        ਜਾਓ Exit;
    }
    ਨਹੀਂਤਾਂ
    {
      /* process the current corner */
      stroker->angle_ਬਾਹਰਲਾ = angle;
      error = ft_stroker_process_ਕੋਨਾ( stroker, ਸਤਰ_ਲੰਮਾਈ );
      ਜੇ ( error )
        ਜਾਓ Exit;
    }

    /* now add a line segment to both the `inside' and `outside' paths */
    ਲਈ ( border = stroker->borders, side = 1; side >= 0; side--, border++ )
    {
      FT_ਸਦਿਸ਼  ਬਿੰਦੂ;


      ਬਿੰਦੂ.x = ਮੰਜ਼ਿਲ->x + delta.x;
      ਬਿੰਦੂ.y = ਮੰਜ਼ਿਲ->y + delta.y;

      /* the ends of lineto borders are movable */
      error = ft_stroke_border_lineto( border, &ਬਿੰਦੂ, TRUE );
      ਜੇ ( error )
        ਜਾਓ Exit;

      delta.x = -delta.x;
      delta.y = -delta.y;
    }

    stroker->angle_ਵਿੱਚ    = angle;
    stroker->center      = *ਮੰਜ਼ਿਲ;
    stroker->ਸਤਰ_ਲੰਮਾਈ = ਸਤਰ_ਲੰਮਾਈ;

  Exit:
    ਵਾਪਸ error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Stroker_Conic_ਮੰਜ਼ਿਲ( FT_Stroker  stroker,
                      FT_ਸਦਿਸ਼*  control,
                      FT_ਸਦਿਸ਼*  ਮੰਜ਼ਿਲ )
  {
    FT_Error    error = FT_Err_Ok;
    FT_ਸਦਿਸ਼   bez_ਢੇਰ[34];
    FT_ਸਦਿਸ਼*  arc;
    FT_ਸਦਿਸ਼*  ਹੱਦਬੰਦੀ = bez_ਢੇਰ + 30;
    FT_Bool     ਪਹਿਲਾ_arc = TRUE;


    ਜੇ ( !stroker || !control || !ਮੰਜ਼ਿਲ )
    {
      error = FT_THROW( Invalid_ਦਲੀਲ );
      ਜਾਓ Exit;
    }

    /* if all control points are coincident, this is a no-op; */
    /* avoid creating a spurious corner                       */
    ਜੇ ( FT_IS_SMALL( stroker->center.x - control->x ) &&
         FT_IS_SMALL( stroker->center.y - control->y ) &&
         FT_IS_SMALL( control->x        - ਮੰਜ਼ਿਲ->x      ) &&
         FT_IS_SMALL( control->y        - ਮੰਜ਼ਿਲ->y      ) )
    {
       stroker->center = *ਮੰਜ਼ਿਲ;
       ਜਾਓ Exit;
    }

    arc    = bez_ਢੇਰ;
    arc[0] = *ਮੰਜ਼ਿਲ;
    arc[1] = *control;
    arc[2] = stroker->center;

    ਕਰੋ
    {
      FT_Angle  angle_ਵਿੱਚ, angle_ਬਾਹਰਲਾ;


      /* initialize with current direction */
      angle_ਵਿੱਚ = angle_ਬਾਹਰਲਾ = stroker->angle_ਵਿੱਚ;

      ਜੇ ( arc < ਹੱਦਬੰਦੀ                                             &&
           !ft_conic_is_small_enough( arc, &angle_ਵਿੱਚ, &angle_ਬਾਹਰਲਾ ) )
      {
        ਜੇ ( stroker->ਪਹਿਲਾ_ਬਿੰਦੂ )
          stroker->angle_ਵਿੱਚ = angle_ਵਿੱਚ;

        ft_conic_ਵੰਡੋ( arc );
        arc += 2;
        ਜਾਰੀ;
      }

      ਜੇ ( ਪਹਿਲਾ_arc )
      {
        ਪਹਿਲਾ_arc = FALSE;

        /* process corner if necessary */
        ਜੇ ( stroker->ਪਹਿਲਾ_ਬਿੰਦੂ )
          error = ft_stroker_subpath_ਸ਼ੁਰੂ( stroker, angle_ਵਿੱਚ, 0 );
        ਨਹੀਂਤਾਂ
        {
          stroker->angle_ਬਾਹਰਲਾ = angle_ਵਿੱਚ;
          error = ft_stroker_process_ਕੋਨਾ( stroker, 0 );
        }
      }
      ਨਹੀਂਤਾਂ ਜੇ ( ft_pos_abs( FT_Angle_Diff( stroker->angle_ਵਿੱਚ, angle_ਵਿੱਚ ) ) >
                  FT_SMALL_CONIC_ਹੱਦ / 4                             )
      {
        /* if the deviation from one arc to the next is too great, */
        /* add a round corner                                      */
        stroker->center    = arc[2];
        stroker->angle_ਬਾਹਰਲਾ = angle_ਵਿੱਚ;
        stroker->ਸਤਰ_join = FT_STROKER_LINEJOIN_ਗੇੜਾ;

        error = ft_stroker_process_ਕੋਨਾ( stroker, 0 );

        /* reinstate line join style */
        stroker->ਸਤਰ_join = stroker->ਸਤਰ_join_saved;
      }

      ਜੇ ( error )
        ਜਾਓ Exit;

      /* the arc's angle is small enough; we can add it directly to each */
      /* border                                                          */
      {
        FT_ਸਦਿਸ਼        ctrl, ਸਿਰਾ;
        FT_Angle         theta, phi, rotate, alpha0 = 0;
        FT_Fixed         ਲੰਮਾਈ;
        FT_StrokeBorder  border;
        FT_Int           side;


        theta  = FT_Angle_Diff( angle_ਵਿੱਚ, angle_ਬਾਹਰਲਾ ) / 2;
        phi    = angle_ਵਿੱਚ + theta;
        ਲੰਮਾਈ = FT_DivFix( stroker->radius, FT_Cos( theta ) );

        /* compute direction of original arc */
        ਜੇ ( stroker->handle_wide_strokes )
          alpha0 = FT_Atan2( arc[0].x - arc[2].x, arc[0].y - arc[2].y );

        ਲਈ ( border = stroker->borders, side = 0;
              side <= 1;
              side++, border++ )
        {
          rotate = FT_SIDE_ਮੰਜ਼ਿਲ_ROTATE( side );

          /* compute control point */
          FT_ਸਦਿਸ਼_From_Polar( &ctrl, ਲੰਮਾਈ, phi + rotate );
          ctrl.x += arc[1].x;
          ctrl.y += arc[1].y;

          /* compute end point */
          FT_ਸਦਿਸ਼_From_Polar( &ਸਿਰਾ, stroker->radius, angle_ਬਾਹਰਲਾ + rotate );
          ਸਿਰਾ.x += arc[0].x;
          ਸਿਰਾ.y += arc[0].y;

          ਜੇ ( stroker->handle_wide_strokes )
          {
            FT_ਸਦਿਸ਼  ਸ਼ੁਰੂ;
            FT_Angle   alpha1;


            /* determine whether the border radius is greater than the */
            /* radius of curvature of the original arc                 */
            ਸ਼ੁਰੂ = border->points[border->ਅੰਕ_points - 1];

            alpha1 = FT_Atan2( ਸਿਰਾ.x - ਸ਼ੁਰੂ.x, ਸਿਰਾ.y - ਸ਼ੁਰੂ.y );

            /* is the direction of the border arc opposite to */
            /* that of the original arc? */
            ਜੇ ( ft_pos_abs( FT_Angle_Diff( alpha0, alpha1 ) ) >
                   FT_ANGLE_PI / 2                             )
            {
              FT_Angle   beta, gamma;
              FT_ਸਦਿਸ਼  bvec, delta;
              FT_Fixed   blen, sinA, sinB, alen;


              /* use the sine rule to find the intersection point */
              beta  = FT_Atan2( arc[2].x - ਸ਼ੁਰੂ.x, arc[2].y - ਸ਼ੁਰੂ.y );
              gamma = FT_Atan2( arc[0].x - ਸਿਰਾ.x,   arc[0].y - ਸਿਰਾ.y );

              bvec.x = ਸਿਰਾ.x - ਸ਼ੁਰੂ.x;
              bvec.y = ਸਿਰਾ.y - ਸ਼ੁਰੂ.y;

              blen = FT_ਸਦਿਸ਼_ਲੰਮਾਈ( &bvec );

              sinA = ft_pos_abs( FT_Sin( alpha1 - gamma ) );
              sinB = ft_pos_abs( FT_Sin( beta - gamma ) );

              alen = FT_MulDiv( blen, sinA, sinB );

              FT_ਸਦਿਸ਼_From_Polar( &delta, alen, beta );
              delta.x += ਸ਼ੁਰੂ.x;
              delta.y += ਸ਼ੁਰੂ.y;

              /* circumnavigate the negative sector backwards */
              border->movable = FALSE;
              error = ft_stroke_border_lineto( border, &delta, FALSE );
              ਜੇ ( error )
                ਜਾਓ Exit;
              error = ft_stroke_border_lineto( border, &ਸਿਰਾ, FALSE );
              ਜੇ ( error )
                ਜਾਓ Exit;
              error = ft_stroke_border_conicto( border, &ctrl, &ਸ਼ੁਰੂ );
              ਜੇ ( error )
                ਜਾਓ Exit;
              /* and then move to the endpoint */
              error = ft_stroke_border_lineto( border, &ਸਿਰਾ, FALSE );
              ਜੇ ( error )
                ਜਾਓ Exit;

              ਜਾਰੀ;
            }

            /* else fall through */
          }

          /* simply add an arc */
          error = ft_stroke_border_conicto( border, &ctrl, &ਸਿਰਾ );
          ਜੇ ( error )
            ਜਾਓ Exit;
        }
      }

      stroker->angle_ਵਿੱਚ = angle_ਬਾਹਰਲਾ;

      ਜੇ ( arc == bez_ਢੇਰ )
        ਰੋਕੋ;
      arc -= 2;
    } ਜਦੋਂ ( 1 );

    stroker->center      = *ਮੰਜ਼ਿਲ;
    stroker->ਸਤਰ_ਲੰਮਾਈ = 0;

  Exit:
    ਵਾਪਸ error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Stroker_Cubic_ਮੰਜ਼ਿਲ( FT_Stroker  stroker,
                      FT_ਸਦਿਸ਼*  control1,
                      FT_ਸਦਿਸ਼*  control2,
                      FT_ਸਦਿਸ਼*  ਮੰਜ਼ਿਲ )
  {
    FT_Error    error = FT_Err_Ok;
    FT_ਸਦਿਸ਼   bez_ਢੇਰ[37];
    FT_ਸਦਿਸ਼*  arc;
    FT_ਸਦਿਸ਼*  ਹੱਦਬੰਦੀ = bez_ਢੇਰ + 32;
    FT_Bool     ਪਹਿਲਾ_arc = TRUE;


    ਜੇ ( !stroker || !control1 || !control2 || !ਮੰਜ਼ਿਲ )
    {
      error = FT_THROW( Invalid_ਦਲੀਲ );
      ਜਾਓ Exit;
    }

    /* if all control points are coincident, this is a no-op; */
    /* avoid creating a spurious corner */
    ਜੇ ( FT_IS_SMALL( stroker->center.x - control1->x ) &&
         FT_IS_SMALL( stroker->center.y - control1->y ) &&
         FT_IS_SMALL( control1->x       - control2->x ) &&
         FT_IS_SMALL( control1->y       - control2->y ) &&
         FT_IS_SMALL( control2->x       - ਮੰਜ਼ਿਲ->x       ) &&
         FT_IS_SMALL( control2->y       - ਮੰਜ਼ਿਲ->y       ) )
    {
       stroker->center = *ਮੰਜ਼ਿਲ;
       ਜਾਓ Exit;
    }

    arc    = bez_ਢੇਰ;
    arc[0] = *ਮੰਜ਼ਿਲ;
    arc[1] = *control2;
    arc[2] = *control1;
    arc[3] = stroker->center;

    ਕਰੋ
    {
      FT_Angle  angle_ਵਿੱਚ, angle_mid, angle_ਬਾਹਰਲਾ;


      /* initialize with current direction */
      angle_ਵਿੱਚ = angle_ਬਾਹਰਲਾ = angle_mid = stroker->angle_ਵਿੱਚ;

      ਜੇ ( arc < ਹੱਦਬੰਦੀ                                         &&
           !ft_cubic_is_small_enough( arc, &angle_ਵਿੱਚ,
                                      &angle_mid, &angle_ਬਾਹਰਲਾ ) )
      {
        ਜੇ ( stroker->ਪਹਿਲਾ_ਬਿੰਦੂ )
          stroker->angle_ਵਿੱਚ = angle_ਵਿੱਚ;

        ft_cubic_ਵੰਡੋ( arc );
        arc += 3;
        ਜਾਰੀ;
      }

      ਜੇ ( ਪਹਿਲਾ_arc )
      {
        ਪਹਿਲਾ_arc = FALSE;

        /* process corner if necessary */
        ਜੇ ( stroker->ਪਹਿਲਾ_ਬਿੰਦੂ )
          error = ft_stroker_subpath_ਸ਼ੁਰੂ( stroker, angle_ਵਿੱਚ, 0 );
        ਨਹੀਂਤਾਂ
        {
          stroker->angle_ਬਾਹਰਲਾ = angle_ਵਿੱਚ;
          error = ft_stroker_process_ਕੋਨਾ( stroker, 0 );
        }
      }
      ਨਹੀਂਤਾਂ ਜੇ ( ft_pos_abs( FT_Angle_Diff( stroker->angle_ਵਿੱਚ, angle_ਵਿੱਚ ) ) >
                  FT_SMALL_CUBIC_ਹੱਦ / 4                             )
      {
        /* if the deviation from one arc to the next is too great, */
        /* add a round corner                                      */
        stroker->center    = arc[3];
        stroker->angle_ਬਾਹਰਲਾ = angle_ਵਿੱਚ;
        stroker->ਸਤਰ_join = FT_STROKER_LINEJOIN_ਗੇੜਾ;

        error = ft_stroker_process_ਕੋਨਾ( stroker, 0 );

        /* reinstate line join style */
        stroker->ਸਤਰ_join = stroker->ਸਤਰ_join_saved;
      }

      ਜੇ ( error )
        ਜਾਓ Exit;

      /* the arc's angle is small enough; we can add it directly to each */
      /* border                                                          */
      {
        FT_ਸਦਿਸ਼        ctrl1, ctrl2, ਸਿਰਾ;
        FT_Angle         theta1, phi1, theta2, phi2, rotate, alpha0 = 0;
        FT_Fixed         length1, length2;
        FT_StrokeBorder  border;
        FT_Int           side;


        theta1  = FT_Angle_Diff( angle_ਵਿੱਚ,  angle_mid ) / 2;
        theta2  = FT_Angle_Diff( angle_mid, angle_ਬਾਹਰਲਾ ) / 2;
        phi1    = ft_angle_ਔਸਤ( angle_ਵਿੱਚ,  angle_mid );
        phi2    = ft_angle_ਔਸਤ( angle_mid, angle_ਬਾਹਰਲਾ );
        length1 = FT_DivFix( stroker->radius, FT_Cos( theta1 ) );
        length2 = FT_DivFix( stroker->radius, FT_Cos( theta2 ) );

        /* compute direction of original arc */
        ਜੇ ( stroker->handle_wide_strokes )
          alpha0 = FT_Atan2( arc[0].x - arc[3].x, arc[0].y - arc[3].y );

        ਲਈ ( border = stroker->borders, side = 0;
              side <= 1;
              side++, border++ )
        {
          rotate = FT_SIDE_ਮੰਜ਼ਿਲ_ROTATE( side );

          /* compute control points */
          FT_ਸਦਿਸ਼_From_Polar( &ctrl1, length1, phi1 + rotate );
          ctrl1.x += arc[2].x;
          ctrl1.y += arc[2].y;

          FT_ਸਦਿਸ਼_From_Polar( &ctrl2, length2, phi2 + rotate );
          ctrl2.x += arc[1].x;
          ctrl2.y += arc[1].y;

          /* compute end point */
          FT_ਸਦਿਸ਼_From_Polar( &ਸਿਰਾ, stroker->radius, angle_ਬਾਹਰਲਾ + rotate );
          ਸਿਰਾ.x += arc[0].x;
          ਸਿਰਾ.y += arc[0].y;

          ਜੇ ( stroker->handle_wide_strokes )
          {
            FT_ਸਦਿਸ਼  ਸ਼ੁਰੂ;
            FT_Angle   alpha1;


            /* determine whether the border radius is greater than the */
            /* radius of curvature of the original arc                 */
            ਸ਼ੁਰੂ = border->points[border->ਅੰਕ_points - 1];

            alpha1 = FT_Atan2( ਸਿਰਾ.x - ਸ਼ੁਰੂ.x, ਸਿਰਾ.y - ਸ਼ੁਰੂ.y );

            /* is the direction of the border arc opposite to */
            /* that of the original arc? */
            ਜੇ ( ft_pos_abs( FT_Angle_Diff( alpha0, alpha1 ) ) >
                   FT_ANGLE_PI / 2                             )
            {
              FT_Angle   beta, gamma;
              FT_ਸਦਿਸ਼  bvec, delta;
              FT_Fixed   blen, sinA, sinB, alen;


              /* use the sine rule to find the intersection point */
              beta  = FT_Atan2( arc[3].x - ਸ਼ੁਰੂ.x, arc[3].y - ਸ਼ੁਰੂ.y );
              gamma = FT_Atan2( arc[0].x - ਸਿਰਾ.x,   arc[0].y - ਸਿਰਾ.y );

              bvec.x = ਸਿਰਾ.x - ਸ਼ੁਰੂ.x;
              bvec.y = ਸਿਰਾ.y - ਸ਼ੁਰੂ.y;

              blen = FT_ਸਦਿਸ਼_ਲੰਮਾਈ( &bvec );

              sinA = ft_pos_abs( FT_Sin( alpha1 - gamma ) );
              sinB = ft_pos_abs( FT_Sin( beta - gamma ) );

              alen = FT_MulDiv( blen, sinA, sinB );

              FT_ਸਦਿਸ਼_From_Polar( &delta, alen, beta );
              delta.x += ਸ਼ੁਰੂ.x;
              delta.y += ਸ਼ੁਰੂ.y;

              /* circumnavigate the negative sector backwards */
              border->movable = FALSE;
              error = ft_stroke_border_lineto( border, &delta, FALSE );
              ਜੇ ( error )
                ਜਾਓ Exit;
              error = ft_stroke_border_lineto( border, &ਸਿਰਾ, FALSE );
              ਜੇ ( error )
                ਜਾਓ Exit;
              error = ft_stroke_border_cubicto( border,
                                                &ctrl2,
                                                &ctrl1,
                                                &ਸ਼ੁਰੂ );
              ਜੇ ( error )
                ਜਾਓ Exit;
              /* and then move to the endpoint */
              error = ft_stroke_border_lineto( border, &ਸਿਰਾ, FALSE );
              ਜੇ ( error )
                ਜਾਓ Exit;

              ਜਾਰੀ;
            }

            /* else fall through */
          }

          /* simply add an arc */
          error = ft_stroke_border_cubicto( border, &ctrl1, &ctrl2, &ਸਿਰਾ );
          ਜੇ ( error )
            ਜਾਓ Exit;
        }
      }

      stroker->angle_ਵਿੱਚ = angle_ਬਾਹਰਲਾ;

      ਜੇ ( arc == bez_ਢੇਰ )
        ਰੋਕੋ;
      arc -= 3;
    } ਜਦੋਂ ( 1 );

    stroker->center      = *ਮੰਜ਼ਿਲ;
    stroker->ਸਤਰ_ਲੰਮਾਈ = 0;

  Exit:
    ਵਾਪਸ error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Stroker_Begin_Sub_ਰਸਤਾ( FT_Stroker  stroker,
                           FT_ਸਦਿਸ਼*  ਮੰਜ਼ਿਲ,
                           FT_Bool     open )
  {
    ਜੇ ( !stroker || !ਮੰਜ਼ਿਲ )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    /* We cannot process the first point, because there is not enough      */
    /* information regarding its corner/cap.  The latter will be processed */
    /* in the `FT_Stroker_EndSubPath' routine.                             */
    /*                                                                     */
    stroker->ਪਹਿਲਾ_ਬਿੰਦੂ  = TRUE;
    stroker->center       = *ਮੰਜ਼ਿਲ;
    stroker->subpath_open = open;

    /* Determine if we need to check whether the border radius is greater */
    /* than the radius of curvature of a curve, to handle this case       */
    /* specially.  This is only required if bevel joins or butt caps may  */
    /* be created, because round & miter joins and round & square caps    */
    /* cover the negative sector created with wide strokes.               */
    stroker->handle_wide_strokes =
      FT_BOOL( stroker->ਸਤਰ_join != FT_STROKER_LINEJOIN_ਗੇੜਾ  ||
               ( stroker->subpath_open                        &&
                 stroker->ਸਤਰ_cap == FT_STROKER_LINECAP_BUTT ) );

    /* record the subpath start point for each border */
    stroker->subpath_ਸ਼ੁਰੂ = *ਮੰਜ਼ਿਲ;

    stroker->angle_ਵਿੱਚ = 0;

    ਵਾਪਸ FT_Err_Ok;
  }


  ਸਥਿਰ FT_Error
  ft_stroker_ਜੋੜੋ_ਉਲਟ_ਖੱਬਾ( FT_Stroker  stroker,
                               FT_Bool     open )
  {
    FT_StrokeBorder  ਸੱਜਾ = stroker->borders + 0;
    FT_StrokeBorder  ਖੱਬਾ  = stroker->borders + 1;
    FT_Int           new_points;
    FT_Error         error = FT_Err_Ok;


    FT_ASSERT( ਖੱਬਾ->ਸ਼ੁਰੂ >= 0 );

    new_points = (FT_Int)ਖੱਬਾ->ਅੰਕ_points - ਖੱਬਾ->ਸ਼ੁਰੂ;
    ਜੇ ( new_points > 0 )
    {
      error = ft_stroke_border_grow( ਸੱਜਾ, (FT_UInt)new_points );
      ਜੇ ( error )
        ਜਾਓ Exit;

      {
        FT_ਸਦਿਸ਼*  dst_ਬਿੰਦੂ = ਸੱਜਾ->points + ਸੱਜਾ->ਅੰਕ_points;
        FT_Byte*    dst_ਟੈਗ   = ਸੱਜਾ->ਟੈਗ_ਸੂਚੀ   + ਸੱਜਾ->ਅੰਕ_points;
        FT_ਸਦਿਸ਼*  src_ਬਿੰਦੂ = ਖੱਬਾ->points  + ਖੱਬਾ->ਅੰਕ_points - 1;
        FT_Byte*    src_ਟੈਗ   = ਖੱਬਾ->ਟੈਗ_ਸੂਚੀ    + ਖੱਬਾ->ਅੰਕ_points - 1;


        ਜਦੋਂ ( src_ਬਿੰਦੂ >= ਖੱਬਾ->points + ਖੱਬਾ->ਸ਼ੁਰੂ )
        {
          *dst_ਬਿੰਦੂ = *src_ਬਿੰਦੂ;
          *dst_ਟੈਗ   = *src_ਟੈਗ;

          ਜੇ ( open )
            dst_ਟੈਗ[0] &= ~FT_STROKE_ਟੈਗ_BEGIN_ਸਿਰਾ;
          ਨਹੀਂਤਾਂ
          {
            FT_Byte  ttag =
                       (FT_Byte)( dst_ਟੈਗ[0] & FT_STROKE_ਟੈਗ_BEGIN_ਸਿਰਾ );


            /* switch begin/end tags if necessary */
            ਜੇ ( ttag == FT_STROKE_ਟੈਗ_BEGIN ||
                 ttag == FT_STROKE_ਟੈਗ_ਸਿਰਾ   )
              dst_ਟੈਗ[0] ^= FT_STROKE_ਟੈਗ_BEGIN_ਸਿਰਾ;
          }

          src_ਬਿੰਦੂ--;
          src_ਟੈਗ--;
          dst_ਬਿੰਦੂ++;
          dst_ਟੈਗ++;
        }
      }

      ਖੱਬਾ->ਅੰਕ_points   = (FT_UInt)ਖੱਬਾ->ਸ਼ੁਰੂ;
      ਸੱਜਾ->ਅੰਕ_points += (FT_UInt)new_points;

      ਸੱਜਾ->movable = FALSE;
      ਖੱਬਾ->movable  = FALSE;
    }

  Exit:
    ਵਾਪਸ error;
  }


  /* documentation is in ftstroke.h */

  /* there's a lot of magic in this function! */
  FT_EXPORT_DEF( FT_Error )
  FT_Stroker_ਸਿਰਾ_Sub_ਰਸਤਾ( FT_Stroker  stroker )
  {
    FT_Error  error = FT_Err_Ok;


    ਜੇ ( !stroker )
    {
      error = FT_THROW( Invalid_ਦਲੀਲ );
      ਜਾਓ Exit;
    }

    /* don't try to end the path if no segments have been generated */
    ਜੇ ( stroker->ਪਹਿਲਾ_ਬਿੰਦੂ )
      ਜਾਓ Exit;

    ਜੇ ( stroker->subpath_open )
    {
      FT_StrokeBorder  ਸੱਜਾ = stroker->borders;


      /* All right, this is an opened path, we need to add a cap between */
      /* right & left, add the reverse of left, then add a final cap     */
      /* between left & right.                                           */
      error = ft_stroker_cap( stroker, stroker->angle_ਵਿੱਚ, 0 );
      ਜੇ ( error )
        ਜਾਓ Exit;

      /* add reversed points from `left' to `right' */
      error = ft_stroker_ਜੋੜੋ_ਉਲਟ_ਖੱਬਾ( stroker, TRUE );
      ਜੇ ( error )
        ਜਾਓ Exit;

      /* now add the final cap */
      stroker->center = stroker->subpath_ਸ਼ੁਰੂ;
      error = ft_stroker_cap( stroker,
                              stroker->subpath_angle + FT_ANGLE_PI, 0 );
      ਜੇ ( error )
        ਜਾਓ Exit;

      /* Now end the right subpath accordingly.  The left one is */
      /* rewind and doesn't need further processing.             */
      ft_stroke_border_close( ਸੱਜਾ, FALSE );
    }
    ਨਹੀਂਤਾਂ
    {
      /* close the path if needed */
      ਜੇ ( !FT_IS_SMALL( stroker->center.x - stroker->subpath_ਸ਼ੁਰੂ.x ) ||
           !FT_IS_SMALL( stroker->center.y - stroker->subpath_ਸ਼ੁਰੂ.y ) )
      {
         error = FT_Stroker_ਸਤਰ_ਮੰਜ਼ਿਲ( stroker, &stroker->subpath_ਸ਼ੁਰੂ );
         ਜੇ ( error )
           ਜਾਓ Exit;
      }

      /* process the corner */
      stroker->angle_ਬਾਹਰਲਾ = stroker->subpath_angle;

      error = ft_stroker_process_ਕੋਨਾ( stroker,
                                         stroker->subpath_ਸਤਰ_ਲੰਮਾਈ );
      ਜੇ ( error )
        ਜਾਓ Exit;

      /* then end our two subpaths */
      ft_stroke_border_close( stroker->borders + 0, FALSE );
      ft_stroke_border_close( stroker->borders + 1, TRUE );
    }

  Exit:
    ਵਾਪਸ error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Stroker_GetBorderCounts( FT_Stroker        stroker,
                              FT_StrokerBorder  border,
                              FT_UInt          *anum_points,
                              FT_UInt          *anum_contours )
  {
    FT_UInt   ਅੰਕ_points = 0, ਅੰਕ_contours = 0;
    FT_Error  error;


    ਜੇ ( !stroker || border > 1 )
    {
      error = FT_THROW( Invalid_ਦਲੀਲ );
      ਜਾਓ Exit;
    }

    error = ft_stroke_border_get_counts( stroker->borders + border,
                                         &ਅੰਕ_points, &ਅੰਕ_contours );
  Exit:
    ਜੇ ( anum_points )
      *anum_points = ਅੰਕ_points;

    ਜੇ ( anum_contours )
      *anum_contours = ਅੰਕ_contours;

    ਵਾਪਸ error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Stroker_GetCounts( FT_Stroker  stroker,
                        FT_UInt    *anum_points,
                        FT_UInt    *anum_contours )
  {
    FT_UInt   count1, count2, ਅੰਕ_points   = 0;
    FT_UInt   count3, count4, ਅੰਕ_contours = 0;
    FT_Error  error;


    ਜੇ ( !stroker )
    {
      error = FT_THROW( Invalid_ਦਲੀਲ );
      ਜਾਓ Exit;
    }

    error = ft_stroke_border_get_counts( stroker->borders + 0,
                                         &count1, &count2 );
    ਜੇ ( error )
      ਜਾਓ Exit;

    error = ft_stroke_border_get_counts( stroker->borders + 1,
                                         &count3, &count4 );
    ਜੇ ( error )
      ਜਾਓ Exit;

    ਅੰਕ_points   = count1 + count3;
    ਅੰਕ_contours = count2 + count4;

  Exit:
    ਜੇ ( anum_points )
      *anum_points   = ਅੰਕ_points;

    ਜੇ ( anum_contours )
      *anum_contours = ਅੰਕ_contours;

    ਵਾਪਸ error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( ਖਾਲੀ )
  FT_Stroker_ExportBorder( FT_Stroker        stroker,
                           FT_StrokerBorder  border,
                           FT_Outline*       outline )
  {
    ਜੇ ( !stroker || !outline )
      ਵਾਪਸ;

    ਜੇ ( border == FT_STROKER_BORDER_ਖੱਬਾ  ||
         border == FT_STROKER_BORDER_ਸੱਜਾ )
    {
      FT_StrokeBorder  sborder = & stroker->borders[border];


      ਜੇ ( sborder->valid )
        ft_stroke_border_export( sborder, outline );
    }
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( ਖਾਲੀ )
  FT_Stroker_Export( FT_Stroker   stroker,
                     FT_Outline*  outline )
  {
    FT_Stroker_ExportBorder( stroker, FT_STROKER_BORDER_ਖੱਬਾ, outline );
    FT_Stroker_ExportBorder( stroker, FT_STROKER_BORDER_ਸੱਜਾ, outline );
  }


  /* documentation is in ftstroke.h */

  /*
   * The following is very similar to FT_Outline_Decompose, except
   * that we do support opened paths, and do not scale the outline.
   */
  FT_EXPORT_DEF( FT_Error )
  FT_Stroker_ParseOutline( FT_Stroker   stroker,
                           FT_Outline*  outline,
                           FT_Bool      opened )
  {
    FT_ਸਦਿਸ਼   v_ਆਖਰੀ;
    FT_ਸਦਿਸ਼   v_control;
    FT_ਸਦਿਸ਼   v_ਸ਼ੁਰੂ;

    FT_ਸਦਿਸ਼*  ਬਿੰਦੂ;
    FT_ਸਦਿਸ਼*  ਹੱਦਬੰਦੀ;
    FT_Byte*    ਟੈਗ_ਸੂਚੀ;

    FT_Error    error;

    FT_Int      n;         /* index of contour in outline     */
    FT_Int      ਪਹਿਲਾ;     /* index of first point in contour */
    FT_Int      ਆਖਰੀ;      /* index of last point in contour  */

    FT_Int      ਟੈਗ;       /* current point's state           */


    ਜੇ ( !outline )
      ਵਾਪਸ FT_THROW( Invalid_Outline );

    ਜੇ ( !stroker )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    FT_Stroker_Rewind( stroker );

    ਆਖਰੀ = -1;
    ਲਈ ( n = 0; n < outline->n_contours; n++ )
    {
      ਪਹਿਲਾ = ਆਖਰੀ + 1;
      ਆਖਰੀ  = outline->contours[n];

      /* skip empty points; we don't stroke these */
      ਜੇ ( ਆਖਰੀ <= ਪਹਿਲਾ )
        ਜਾਰੀ;

      ਹੱਦਬੰਦੀ = outline->points + ਆਖਰੀ;

      v_ਸ਼ੁਰੂ = outline->points[ਪਹਿਲਾ];
      v_ਆਖਰੀ  = outline->points[ਆਖਰੀ];

      v_control = v_ਸ਼ੁਰੂ;

      ਬਿੰਦੂ = outline->points + ਪਹਿਲਾ;
      ਟੈਗ_ਸੂਚੀ  = outline->ਟੈਗ_ਸੂਚੀ   + ਪਹਿਲਾ;
      ਟੈਗ   = FT_ਵਕਰ_ਟੈਗ( ਟੈਗ_ਸੂਚੀ[0] );

      /* check first point to determine origin */
      ਜੇ ( ਟੈਗ != FT_ਵਕਰ_ਟੈਗ_ON )
      {
        FT_Int  ਆਖਰੀ_ਟੈਗ = FT_ਵਕਰ_ਟੈਗ( outline->ਟੈਗ_ਸੂਚੀ[ਆਖਰੀ] );


        /* First point is a control point.  Yes, this happens. */
        ਜੇ ( ਆਖਰੀ_ਟੈਗ == FT_ਵਕਰ_ਟੈਗ_ON )
        {
          /* start at last point if it is on the curve */
          v_ਸ਼ੁਰੂ = v_ਆਖਰੀ;
          ਹੱਦਬੰਦੀ--;
        }
        ਨਹੀਂਤਾਂ
        {
          ਜੇ ( ਆਖਰੀ_ਟੈਗ != ਟੈਗ )
            ਜਾਓ Invalid_Outline;

          /* if both first and last points are controls, */
          /* start at their middle                       */
          v_ਸ਼ੁਰੂ.x = ( v_ਸ਼ੁਰੂ.x + v_ਆਖਰੀ.x ) / 2;
          v_ਸ਼ੁਰੂ.y = ( v_ਸ਼ੁਰੂ.y + v_ਆਖਰੀ.y ) / 2;
        }
        ਬਿੰਦੂ--;
        ਟੈਗ_ਸੂਚੀ--;
      }

      error = FT_Stroker_Begin_Sub_ਰਸਤਾ( stroker, &v_ਸ਼ੁਰੂ, opened );
      ਜੇ ( error )
        ਜਾਓ Exit;

      ਜਦੋਂ ( ਬਿੰਦੂ < ਹੱਦਬੰਦੀ )
      {
        ਬਿੰਦੂ++;
        ਟੈਗ_ਸੂਚੀ++;

        ਟੈਗ = FT_ਵਕਰ_ਟੈਗ( ਟੈਗ_ਸੂਚੀ[0] );
        ਚੋਣ ( ਟੈਗ )
        {
        ਹਾਲਤ FT_ਵਕਰ_ਟੈਗ_ON:  /* emit a single line_to */
          {
            FT_ਸਦਿਸ਼  vec;


            vec.x = ਬਿੰਦੂ->x;
            vec.y = ਬਿੰਦੂ->y;

            error = FT_Stroker_ਸਤਰ_ਮੰਜ਼ਿਲ( stroker, &vec );
            ਜੇ ( error )
              ਜਾਓ Exit;
            ਜਾਰੀ;
          }

        ਹਾਲਤ FT_ਵਕਰ_ਟੈਗ_CONIC:  /* consume conic arcs */
          v_control.x = ਬਿੰਦੂ->x;
          v_control.y = ਬਿੰਦੂ->y;

        Do_Conic:
          ਜੇ ( ਬਿੰਦੂ < ਹੱਦਬੰਦੀ )
          {
            FT_ਸਦਿਸ਼  vec;
            FT_ਸਦਿਸ਼  v_ਵਿਚਕਾਰ;


            ਬਿੰਦੂ++;
            ਟੈਗ_ਸੂਚੀ++;
            ਟੈਗ = FT_ਵਕਰ_ਟੈਗ( ਟੈਗ_ਸੂਚੀ[0] );

            vec = ਬਿੰਦੂ[0];

            ਜੇ ( ਟੈਗ == FT_ਵਕਰ_ਟੈਗ_ON )
            {
              error = FT_Stroker_Conic_ਮੰਜ਼ਿਲ( stroker, &v_control, &vec );
              ਜੇ ( error )
                ਜਾਓ Exit;
              ਜਾਰੀ;
            }

            ਜੇ ( ਟੈਗ != FT_ਵਕਰ_ਟੈਗ_CONIC )
              ਜਾਓ Invalid_Outline;

            v_ਵਿਚਕਾਰ.x = ( v_control.x + vec.x ) / 2;
            v_ਵਿਚਕਾਰ.y = ( v_control.y + vec.y ) / 2;

            error = FT_Stroker_Conic_ਮੰਜ਼ਿਲ( stroker, &v_control, &v_ਵਿਚਕਾਰ );
            ਜੇ ( error )
              ਜਾਓ Exit;

            v_control = vec;
            ਜਾਓ Do_Conic;
          }

          error = FT_Stroker_Conic_ਮੰਜ਼ਿਲ( stroker, &v_control, &v_ਸ਼ੁਰੂ );
          ਜਾਓ Close;

        ਮੂਲ:  /* FT_CURVE_TAG_CUBIC */
          {
            FT_ਸਦਿਸ਼  vec1, vec2;


            ਜੇ ( ਬਿੰਦੂ + 1 > ਹੱਦਬੰਦੀ                             ||
                 FT_ਵਕਰ_ਟੈਗ( ਟੈਗ_ਸੂਚੀ[1] ) != FT_ਵਕਰ_ਟੈਗ_CUBIC )
              ਜਾਓ Invalid_Outline;

            ਬਿੰਦੂ += 2;
            ਟੈਗ_ਸੂਚੀ  += 2;

            vec1 = ਬਿੰਦੂ[-2];
            vec2 = ਬਿੰਦੂ[-1];

            ਜੇ ( ਬਿੰਦੂ <= ਹੱਦਬੰਦੀ )
            {
              FT_ਸਦਿਸ਼  vec;
              FT_ਸਦਿਸ਼  v_ਵਿਚਕਾਰ;


              ਟੈਗ = FT_ਵਕਰ_ਟੈਗ( ਟੈਗ_ਸੂਚੀ[0] );
              vec = ਬਿੰਦੂ[0];

              ਜੇ ( ਟੈਗ == FT_ਵਕਰ_ਟੈਗ_CUBIC )
              {
                v_ਵਿਚਕਾਰ.x = ( vec2.x + vec.x ) / 2;
                v_ਵਿਚਕਾਰ.y = ( vec2.y + vec.y ) / 2;

                vec = v_ਵਿਚਕਾਰ;
              }
              ਨਹੀਂਤਾਂ ਜੇ ( ਟੈਗ != FT_ਵਕਰ_ਟੈਗ_ON )
                ਜਾਓ Invalid_Outline;

              error = FT_Stroker_Cubic_ਮੰਜ਼ਿਲ( stroker, &vec1, &vec2, &vec );
              ਜੇ ( error )
                ਜਾਓ Exit;

              ਜੇ ( ਟੈਗ == FT_ਵਕਰ_ਟੈਗ_CUBIC )
              {
                ਬਿੰਦੂ--;
                ਟੈਗ_ਸੂਚੀ--;
              }
              ਜਾਰੀ;
            }

            error = FT_Stroker_Cubic_ਮੰਜ਼ਿਲ( stroker, &vec1, &vec2, &v_ਸ਼ੁਰੂ );
            ਜਾਓ Close;
          }
        }
      }

    Close:
      ਜੇ ( error )
        ਜਾਓ Exit;

      error = FT_Stroker_ਸਿਰਾ_Sub_ਰਸਤਾ( stroker );
      ਜੇ ( error )
        ਜਾਓ Exit;
    }

    ਵਾਪਸ FT_Err_Ok;

  Exit:
    ਵਾਪਸ error;

  Invalid_Outline:
    ਵਾਪਸ FT_THROW( Invalid_Outline );
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Glyph_Stroke( FT_Glyph    *pglyph,
                   FT_Stroker   stroker,
                   FT_Bool      destroy )
  {
    FT_Error  error = FT_THROW( Invalid_ਦਲੀਲ );
    FT_Glyph  glyph = ਨਿਰਾ;


    ਜੇ ( !pglyph )
      ਜਾਓ Exit;

    glyph = *pglyph;
    ਜੇ ( !glyph || glyph->clazz != &ft_outline_glyph_class )
      ਜਾਓ Exit;

    {
      FT_Glyph  copy;


      error = FT_Glyph_Copy( glyph, &copy );
      ਜੇ ( error )
        ਜਾਓ Exit;

      glyph = copy;
    }

    {
      FT_OutlineGlyph  oglyph  = (FT_OutlineGlyph)glyph;
      FT_Outline*      outline = &oglyph->outline;
      FT_UInt          ਅੰਕ_points, ਅੰਕ_contours;


      error = FT_Stroker_ParseOutline( stroker, outline, FALSE );
      ਜੇ ( error )
        ਜਾਓ Fail;

      error = FT_Stroker_GetCounts( stroker, &ਅੰਕ_points, &ਅੰਕ_contours );
      ਜੇ ( error )
        ਜਾਓ Fail;

      FT_Outline_ਪੂਰਾ( glyph->library, outline );

      error = FT_Outline_New( glyph->library,
                              ਅੰਕ_points,
                              (FT_Int)ਅੰਕ_contours,
                              outline );
      ਜੇ ( error )
        ਜਾਓ Fail;

      outline->n_points   = 0;
      outline->n_contours = 0;

      FT_Stroker_Export( stroker, outline );
    }

    ਜੇ ( destroy )
      FT_ਪੂਰਾ_Glyph( *pglyph );

    *pglyph = glyph;
    ਜਾਓ Exit;

  Fail:
    FT_ਪੂਰਾ_Glyph( glyph );
    glyph = ਨਿਰਾ;

    ਜੇ ( !destroy )
      *pglyph = ਨਿਰਾ;

  Exit:
    ਵਾਪਸ error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Glyph_StrokeBorder( FT_Glyph    *pglyph,
                         FT_Stroker   stroker,
                         FT_Bool      inside,
                         FT_Bool      destroy )
  {
    FT_Error  error = FT_THROW( Invalid_ਦਲੀਲ );
    FT_Glyph  glyph = ਨਿਰਾ;


    ਜੇ ( !pglyph )
      ਜਾਓ Exit;

    glyph = *pglyph;
    ਜੇ ( !glyph || glyph->clazz != &ft_outline_glyph_class )
      ਜਾਓ Exit;

    {
      FT_Glyph  copy;


      error = FT_Glyph_Copy( glyph, &copy );
      ਜੇ ( error )
        ਜਾਓ Exit;

      glyph = copy;
    }

    {
      FT_OutlineGlyph   oglyph  = (FT_OutlineGlyph)glyph;
      FT_StrokerBorder  border;
      FT_Outline*       outline = &oglyph->outline;
      FT_UInt           ਅੰਕ_points, ਅੰਕ_contours;


      border = FT_Outline_GetOutsideBorder( outline );
      ਜੇ ( inside )
      {
        ਜੇ ( border == FT_STROKER_BORDER_ਖੱਬਾ )
          border = FT_STROKER_BORDER_ਸੱਜਾ;
        ਨਹੀਂਤਾਂ
          border = FT_STROKER_BORDER_ਖੱਬਾ;
      }

      error = FT_Stroker_ParseOutline( stroker, outline, FALSE );
      ਜੇ ( error )
        ਜਾਓ Fail;

      FT_Stroker_GetBorderCounts( stroker, border,
                                  &ਅੰਕ_points, &ਅੰਕ_contours );

      FT_Outline_ਪੂਰਾ( glyph->library, outline );

      error = FT_Outline_New( glyph->library,
                              ਅੰਕ_points,
                              (FT_Int)ਅੰਕ_contours,
                              outline );
      ਜੇ ( error )
        ਜਾਓ Fail;

      outline->n_points   = 0;
      outline->n_contours = 0;

      FT_Stroker_ExportBorder( stroker, border, outline );
    }

    ਜੇ ( destroy )
      FT_ਪੂਰਾ_Glyph( *pglyph );

    *pglyph = glyph;
    ਜਾਓ Exit;

  Fail:
    FT_ਪੂਰਾ_Glyph( glyph );
    glyph = ਨਿਰਾ;

    ਜੇ ( !destroy )
      *pglyph = ਨਿਰਾ;

  Exit:
    ਵਾਪਸ error;
  }


/*ਸਿਰਾ*/
