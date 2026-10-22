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


#ಅನ್ತರ್ಭಾವ <freetype/ftstroke.h>
#ಅನ್ತರ್ಭಾವ <freetype/fttrigon.h>
#ಅನ್ತರ್ಭಾವ <freetype/ftoutln.h>
#ಅನ್ತರ್ಭಾವ <freetype/internal/ftmemory.h>
#ಅನ್ತರ್ಭಾವ <freetype/internal/ftdebug.h>
#ಅನ್ತರ್ಭಾವ <freetype/internal/ftobjs.h>


  /* declare an extern to access `ft_outline_glyph_class' globally */
  /* allocated  in `ftglyph.c'                                     */
  FT_CALLBACK_ಕೋಷ್ಟಕ ಸ್ಥಿರಾಂಕ FT_Glyph_Class  ft_outline_glyph_class;


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_StrokerBorder )
  FT_Outline_GetInsideBorder( FT_Outline*  outline )
  {
    FT_Orientation  o = FT_Outline_Get_Orientation( outline );


    ಹಿಂತಿರುಗಿ o == FT_ORIENTATION_TRUETYPE ? FT_STROKER_BORDER_ಬಲ
                                        : FT_STROKER_BORDER_ಎಡ;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_StrokerBorder )
  FT_Outline_GetOutsideBorder( FT_Outline*  outline )
  {
    FT_Orientation  o = FT_Outline_Get_Orientation( outline );


    ಹಿಂತಿರುಗಿ o == FT_ORIENTATION_TRUETYPE ? FT_STROKER_BORDER_ಎಡ
                                        : FT_STROKER_BORDER_ಬಲ;
  }


  /*************************************************************************/
  /*************************************************************************/
  /*****                                                               *****/
  /*****                      BEZIER COMPUTATIONS                      *****/
  /*****                                                               *****/
  /*************************************************************************/
  /*************************************************************************/

#ವ್ಯಾಖ್ಯಾನಿಸು FT_SMALL_CONIC_ಮಿತಿ  ( FT_ANGLE_PI / 6 )
#ವ್ಯಾಖ್ಯಾನಿಸು FT_SMALL_CUBIC_ಮಿತಿ  ( FT_ANGLE_PI / 8 )

#ವ್ಯಾಖ್ಯಾನಿಸು FT_EPSILON  2

#ವ್ಯಾಖ್ಯಾನಿಸು FT_IS_SMALL( x )  ( (x) > -FT_EPSILON && (x) < FT_EPSILON )


  ಸ್ಥಿರ FT_Pos
  ft_pos_abs( FT_Pos  x )
  {
    ಹಿಂತಿರುಗಿ x >= 0 ? x : -x;
  }


  ಸ್ಥಿರ ಶೂನ್ಯ
  ft_conic_ವಿಭಜಿಸು( FT_ಸದಿಶ*  ತಳ )
  {
    FT_Pos  a, b;


    ತಳ[4].x = ತಳ[2].x;
    a = ತಳ[0].x + ತಳ[1].x;
    b = ತಳ[1].x + ತಳ[2].x;
    ತಳ[3].x = b >> 1;
    ತಳ[2].x = ( a + b ) >> 2;
    ತಳ[1].x = a >> 1;

    ತಳ[4].y = ತಳ[2].y;
    a = ತಳ[0].y + ತಳ[1].y;
    b = ತಳ[1].y + ತಳ[2].y;
    ತಳ[3].y = b >> 1;
    ತಳ[2].y = ( a + b ) >> 2;
    ತಳ[1].y = a >> 1;
  }


  ಸ್ಥಿರ FT_Bool
  ft_conic_is_small_enough( FT_ಸದಿಶ*  ತಳ,
                            FT_Angle   *angle_ಒಳ,
                            FT_Angle   *angle_ಹೊರಹೋಗು )
  {
    FT_ಸದಿಶ  d1, d2;
    FT_Angle   theta;
    FT_Int     close1, close2;


    d1.x = ತಳ[1].x - ತಳ[2].x;
    d1.y = ತಳ[1].y - ತಳ[2].y;
    d2.x = ತಳ[0].x - ತಳ[1].x;
    d2.y = ತಳ[0].y - ತಳ[1].y;

    close1 = FT_IS_SMALL( d1.x ) && FT_IS_SMALL( d1.y );
    close2 = FT_IS_SMALL( d2.x ) && FT_IS_SMALL( d2.y );

    ಆದರೆ ( close1 )
    {
      ಆದರೆ ( close2 )
      {
        /* basically a point;                      */
        /* do nothing to retain original direction */
      }
      ಇಲ್ಲದಿದ್ದರೆ
      {
        *angle_ಒಳ  =
        *angle_ಹೊರಹೋಗು = FT_Atan2( d2.x, d2.y );
      }
    }
    ಇಲ್ಲದಿದ್ದರೆ /* !close1 */
    {
      ಆದರೆ ( close2 )
      {
        *angle_ಒಳ  =
        *angle_ಹೊರಹೋಗು = FT_Atan2( d1.x, d1.y );
      }
      ಇಲ್ಲದಿದ್ದರೆ
      {
        *angle_ಒಳ  = FT_Atan2( d1.x, d1.y );
        *angle_ಹೊರಹೋಗು = FT_Atan2( d2.x, d2.y );
      }
    }

    theta = ft_pos_abs( FT_Angle_Diff( *angle_ಒಳ, *angle_ಹೊರಹೋಗು ) );

    ಹಿಂತಿರುಗಿ FT_BOOL( theta < FT_SMALL_CONIC_ಮಿತಿ );
  }


  ಸ್ಥಿರ ಶೂನ್ಯ
  ft_cubic_ವಿಭಜಿಸು( FT_ಸದಿಶ*  ತಳ )
  {
    FT_Pos  a, b, c;


    ತಳ[6].x = ತಳ[3].x;
    a = ತಳ[0].x + ತಳ[1].x;
    b = ತಳ[1].x + ತಳ[2].x;
    c = ತಳ[2].x + ತಳ[3].x;
    ತಳ[5].x = c >> 1;
    c += b;
    ತಳ[4].x = c >> 2;
    ತಳ[1].x = a >> 1;
    a += b;
    ತಳ[2].x = a >> 2;
    ತಳ[3].x = ( a + c ) >> 3;

    ತಳ[6].y = ತಳ[3].y;
    a = ತಳ[0].y + ತಳ[1].y;
    b = ತಳ[1].y + ತಳ[2].y;
    c = ತಳ[2].y + ತಳ[3].y;
    ತಳ[5].y = c >> 1;
    c += b;
    ತಳ[4].y = c >> 2;
    ತಳ[1].y = a >> 1;
    a += b;
    ತಳ[2].y = a >> 2;
    ತಳ[3].y = ( a + c ) >> 3;
  }


  /* Return the average of `angle1' and `angle2'.            */
  /* This gives correct result even if `angle1' and `angle2' */
  /* have opposite signs.                                    */
  ಸ್ಥಿರ FT_Angle
  ft_angle_ಸರಾಸರಿ( FT_Angle  angle1,
                 FT_Angle  angle2 )
  {
    ಹಿಂತಿರುಗಿ angle1 + FT_Angle_Diff( angle1, angle2 ) / 2;
  }


  ಸ್ಥಿರ FT_Bool
  ft_cubic_is_small_enough( FT_ಸದಿಶ*  ತಳ,
                            FT_Angle   *angle_ಒಳ,
                            FT_Angle   *angle_mid,
                            FT_Angle   *angle_ಹೊರಹೋಗು )
  {
    FT_ಸದಿಶ  d1, d2, d3;
    FT_Angle   theta1, theta2;
    FT_Int     close1, close2, close3;


    d1.x = ತಳ[2].x - ತಳ[3].x;
    d1.y = ತಳ[2].y - ತಳ[3].y;
    d2.x = ತಳ[1].x - ತಳ[2].x;
    d2.y = ತಳ[1].y - ತಳ[2].y;
    d3.x = ತಳ[0].x - ತಳ[1].x;
    d3.y = ತಳ[0].y - ತಳ[1].y;

    close1 = FT_IS_SMALL( d1.x ) && FT_IS_SMALL( d1.y );
    close2 = FT_IS_SMALL( d2.x ) && FT_IS_SMALL( d2.y );
    close3 = FT_IS_SMALL( d3.x ) && FT_IS_SMALL( d3.y );

    ಆದರೆ ( close1 )
    {
      ಆದರೆ ( close2 )
      {
        ಆದರೆ ( close3 )
        {
          /* basically a point;                      */
          /* do nothing to retain original direction */
        }
        ಇಲ್ಲದಿದ್ದರೆ /* !close3 */
        {
          *angle_ಒಳ  =
          *angle_mid =
          *angle_ಹೊರಹೋಗು = FT_Atan2( d3.x, d3.y );
        }
      }
      ಇಲ್ಲದಿದ್ದರೆ /* !close2 */
      {
        ಆದರೆ ( close3 )
        {
          *angle_ಒಳ  =
          *angle_mid =
          *angle_ಹೊರಹೋಗು = FT_Atan2( d2.x, d2.y );
        }
        ಇಲ್ಲದಿದ್ದರೆ /* !close3 */
        {
          *angle_ಒಳ  =
          *angle_mid = FT_Atan2( d2.x, d2.y );
          *angle_ಹೊರಹೋಗು = FT_Atan2( d3.x, d3.y );
        }
      }
    }
    ಇಲ್ಲದಿದ್ದರೆ /* !close1 */
    {
      ಆದರೆ ( close2 )
      {
        ಆದರೆ ( close3 )
        {
          *angle_ಒಳ  =
          *angle_mid =
          *angle_ಹೊರಹೋಗು = FT_Atan2( d1.x, d1.y );
        }
        ಇಲ್ಲದಿದ್ದರೆ /* !close3 */
        {
          *angle_ಒಳ  = FT_Atan2( d1.x, d1.y );
          *angle_ಹೊರಹೋಗು = FT_Atan2( d3.x, d3.y );
          *angle_mid = ft_angle_ಸರಾಸರಿ( *angle_ಒಳ, *angle_ಹೊರಹೋಗು );
        }
      }
      ಇಲ್ಲದಿದ್ದರೆ /* !close2 */
      {
        ಆದರೆ ( close3 )
        {
          *angle_ಒಳ  = FT_Atan2( d1.x, d1.y );
          *angle_mid =
          *angle_ಹೊರಹೋಗು = FT_Atan2( d2.x, d2.y );
        }
        ಇಲ್ಲದಿದ್ದರೆ /* !close3 */
        {
          *angle_ಒಳ  = FT_Atan2( d1.x, d1.y );
          *angle_mid = FT_Atan2( d2.x, d2.y );
          *angle_ಹೊರಹೋಗು = FT_Atan2( d3.x, d3.y );
        }
      }
    }

    theta1 = ft_pos_abs( FT_Angle_Diff( *angle_ಒಳ,  *angle_mid ) );
    theta2 = ft_pos_abs( FT_Angle_Diff( *angle_mid, *angle_ಹೊರಹೋಗು ) );

    ಹಿಂತಿರುಗಿ FT_BOOL( theta1 < FT_SMALL_CUBIC_ಮಿತಿ &&
                    theta2 < FT_SMALL_CUBIC_ಮಿತಿ );
  }


  /*************************************************************************/
  /*************************************************************************/
  /*****                                                               *****/
  /*****                       STROKE BORDERS                          *****/
  /*****                                                               *****/
  /*************************************************************************/
  /*************************************************************************/

  ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಎಣಿಕೆ  FT_Stroke_ಟ್ಯಾಗುಗಳು
  {
    FT_STROKE_ಟ್ಯಾಗ್_ON    = 1,   /* on-curve point  */
    FT_STROKE_ಟ್ಯಾಗ್_CUBIC = 2,   /* cubic off-point */
    FT_STROKE_ಟ್ಯಾಗ್_BEGIN = 4,   /* sub-path start  */
    FT_STROKE_ಟ್ಯಾಗ್_ಕೊನೆ   = 8    /* sub-path end    */

  } FT_Stroke_ಟ್ಯಾಗುಗಳು_2;

#ವ್ಯಾಖ್ಯಾನಿಸು  FT_STROKE_ಟ್ಯಾಗ್_BEGIN_ಕೊನೆ  ( FT_STROKE_ಟ್ಯಾಗ್_BEGIN | FT_STROKE_ಟ್ಯಾಗ್_ಕೊನೆ )

  ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ  FT_StrokeBorderRec_
  {
    FT_UInt     ಅಂಕೆ_points;
    FT_UInt     max_points;
    FT_ಸದಿಶ*  points;
    FT_Byte*    ಟ್ಯಾಗುಗಳು;
    FT_Bool     movable;  /* TRUE for ends of lineto borders */
    FT_Int      ಆರಂಭ;    /* index of current sub-path start point */
    FT_ಸ್ಮರಣೆ   ಸ್ಮರಣೆ;
    FT_Bool     valid;

  } FT_StrokeBorderRec, *FT_StrokeBorder;


  ಸ್ಥಿರ FT_Error
  ft_stroke_border_grow( FT_StrokeBorder  border,
                         FT_UInt          new_points )
  {
    FT_UInt   old_max = border->max_points;
    FT_UInt   new_max = border->ಅಂಕೆ_points + new_points;
    FT_Error  error   = FT_Err_Ok;


    ಆದರೆ ( new_max > old_max )
    {
      FT_UInt    cur_max = old_max;
      FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ  = border->ಸ್ಮರಣೆ;


      ಆಗ ( cur_max < new_max )
        cur_max += ( cur_max >> 1 ) + 16;

      ಆದರೆ ( FT_RENEW_ARRAY( border->points, old_max, cur_max ) ||
           FT_RENEW_ARRAY( border->ಟ್ಯಾಗುಗಳು,   old_max, cur_max ) )
        ಹೋಗು Exit;

      border->max_points = cur_max;
    }

  Exit:
    ಹಿಂತಿರುಗಿ error;
  }


  ಸ್ಥಿರ ಶೂನ್ಯ
  ft_stroke_border_close( FT_StrokeBorder  border,
                          FT_Bool          ಹಿಮ್ಮುಖ )
  {
    FT_UInt  ಆರಂಭ = (FT_UInt)border->ಆರಂಭ;
    FT_UInt  ಎಣಿಕೆ_2 = border->ಅಂಕೆ_points;


    FT_ASSERT( border->ಆರಂಭ >= 0 );

    /* don't record empty paths! */
    ಆದರೆ ( ಎಣಿಕೆ_2 <= ಆರಂಭ + 1U )
      border->ಅಂಕೆ_points = ಆರಂಭ;
    ಇಲ್ಲದಿದ್ದರೆ
    {
      /* copy the last point to the start of this sub-path, since */
      /* it contains the `adjusted' starting coordinates          */
      border->ಅಂಕೆ_points    = --ಎಣಿಕೆ_2;
      border->points[ಆರಂಭ] = border->points[ಎಣಿಕೆ_2];
      border->ಟ್ಯಾಗುಗಳು[ಆರಂಭ]   = border->ಟ್ಯಾಗುಗಳು[ಎಣಿಕೆ_2];

      ಆದರೆ ( ಹಿಮ್ಮುಖ )
      {
        /* reverse the points */
        {
          FT_ಸದಿಶ*  vec1 = border->points + ಆರಂಭ + 1;
          FT_ಸದಿಶ*  vec2 = border->points + ಎಣಿಕೆ_2 - 1;


          ಗಾಗಿ ( ; vec1 < vec2; vec1++, vec2-- )
          {
            FT_ಸದಿಶ  tmp;


            tmp   = *vec1;
            *vec1 = *vec2;
            *vec2 = tmp;
          }
        }

        /* then the tags */
        {
          FT_Byte*  tag1 = border->ಟ್ಯಾಗುಗಳು + ಆರಂಭ + 1;
          FT_Byte*  tag2 = border->ಟ್ಯಾಗುಗಳು + ಎಣಿಕೆ_2 - 1;


          ಗಾಗಿ ( ; tag1 < tag2; tag1++, tag2-- )
          {
            FT_Byte  tmp;


            tmp   = *tag1;
            *tag1 = *tag2;
            *tag2 = tmp;
          }
        }
      }

      border->ಟ್ಯಾಗುಗಳು[ಆರಂಭ    ] |= FT_STROKE_ಟ್ಯಾಗ್_BEGIN;
      border->ಟ್ಯಾಗುಗಳು[ಎಣಿಕೆ_2 - 1] |= FT_STROKE_ಟ್ಯಾಗ್_ಕೊನೆ;
    }

    border->ಆರಂಭ   = -1;
    border->movable = FALSE;
  }


  ಸ್ಥಿರ FT_Error
  ft_stroke_border_lineto( FT_StrokeBorder  border,
                           FT_ಸದಿಶ*       ಗಮ್ಯ,
                           FT_Bool          movable )
  {
    FT_Error  error = FT_Err_Ok;


    FT_ASSERT( border->ಆರಂಭ >= 0 );

    ಆದರೆ ( border->movable )
    {
      /* move last point */
      border->points[border->ಅಂಕೆ_points - 1] = *ಗಮ್ಯ;
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
      /* don't add zero-length lineto, but always add moveto */
      ಆದರೆ ( border->ಅಂಕೆ_points > (FT_UInt)border->ಆರಂಭ                     &&
           FT_IS_SMALL( border->points[border->ಅಂಕೆ_points - 1].x - ಗಮ್ಯ->x ) &&
           FT_IS_SMALL( border->points[border->ಅಂಕೆ_points - 1].y - ಗಮ್ಯ->y ) )
        ಹಿಂತಿರುಗಿ error;

      /* add one point */
      error = ft_stroke_border_grow( border, 1 );
      ಆದರೆ ( !error )
      {
        FT_ಸದಿಶ*  vec = border->points + border->ಅಂಕೆ_points;
        FT_Byte*    ಟ್ಯಾಗ್ = border->ಟ್ಯಾಗುಗಳು   + border->ಅಂಕೆ_points;


        vec[0] = *ಗಮ್ಯ;
        ಟ್ಯಾಗ್[0] = FT_STROKE_ಟ್ಯಾಗ್_ON;

        border->ಅಂಕೆ_points += 1;
      }
    }
    border->movable = movable;
    ಹಿಂತಿರುಗಿ error;
  }


  ಸ್ಥಿರ FT_Error
  ft_stroke_border_conicto( FT_StrokeBorder  border,
                            FT_ಸದಿಶ*       control,
                            FT_ಸದಿಶ*       ಗಮ್ಯ )
  {
    FT_Error  error;


    FT_ASSERT( border->ಆರಂಭ >= 0 );

    error = ft_stroke_border_grow( border, 2 );
    ಆದರೆ ( !error )
    {
      FT_ಸದಿಶ*  vec = border->points + border->ಅಂಕೆ_points;
      FT_Byte*    ಟ್ಯಾಗ್ = border->ಟ್ಯಾಗುಗಳು   + border->ಅಂಕೆ_points;


      vec[0] = *control;
      vec[1] = *ಗಮ್ಯ;

      ಟ್ಯಾಗ್[0] = 0;
      ಟ್ಯಾಗ್[1] = FT_STROKE_ಟ್ಯಾಗ್_ON;

      border->ಅಂಕೆ_points += 2;
    }

    border->movable = FALSE;

    ಹಿಂತಿರುಗಿ error;
  }


  ಸ್ಥಿರ FT_Error
  ft_stroke_border_cubicto( FT_StrokeBorder  border,
                            FT_ಸದಿಶ*       control1,
                            FT_ಸದಿಶ*       control2,
                            FT_ಸದಿಶ*       ಗಮ್ಯ )
  {
    FT_Error  error;


    FT_ASSERT( border->ಆರಂಭ >= 0 );

    error = ft_stroke_border_grow( border, 3 );
    ಆದರೆ ( !error )
    {
      FT_ಸದಿಶ*  vec = border->points + border->ಅಂಕೆ_points;
      FT_Byte*    ಟ್ಯಾಗ್ = border->ಟ್ಯಾಗುಗಳು   + border->ಅಂಕೆ_points;


      vec[0] = *control1;
      vec[1] = *control2;
      vec[2] = *ಗಮ್ಯ;

      ಟ್ಯಾಗ್[0] = FT_STROKE_ಟ್ಯಾಗ್_CUBIC;
      ಟ್ಯಾಗ್[1] = FT_STROKE_ಟ್ಯಾಗ್_CUBIC;
      ಟ್ಯಾಗ್[2] = FT_STROKE_ಟ್ಯಾಗ್_ON;

      border->ಅಂಕೆ_points += 3;
    }

    border->movable = FALSE;

    ಹಿಂತಿರುಗಿ error;
  }


#ವ್ಯಾಖ್ಯಾನಿಸು FT_ARC_CUBIC_ANGLE  ( FT_ANGLE_PI / 2 )


  ಸ್ಥಿರ FT_Error
  ft_stroke_border_arcto( FT_StrokeBorder  border,
                          FT_ಸದಿಶ*       center,
                          FT_Fixed         radius,
                          FT_Angle         angle_ಆರಂಭ,
                          FT_Angle         angle_diff )
  {
    FT_Fixed   coef;
    FT_ಸದಿಶ  a0, a1, a2, a3;
    FT_Int     i, arcs = 1;
    FT_Error   error = FT_Err_Ok;


    /* number of cubic arcs to draw */
    ಆಗ (  angle_diff > FT_ARC_CUBIC_ANGLE * arcs ||
            -angle_diff > FT_ARC_CUBIC_ANGLE * arcs )
      arcs++;

    /* control tangents */
    coef  = FT_Tan( angle_diff / ( 4 * arcs ) );
    coef += coef / 3;

    /* compute start and first control point */
    FT_ಸದಿಶ_From_Polar( &a0, radius, angle_ಆರಂಭ );
    a1.x = FT_MulFix( -a0.y, coef );
    a1.y = FT_MulFix(  a0.x, coef );

    a0.x += center->x;
    a0.y += center->y;
    a1.x += a0.x;
    a1.y += a0.y;

    ಗಾಗಿ ( i = 1; i <= arcs; i++ )
    {
      /* compute end and second control point */
      FT_ಸದಿಶ_From_Polar( &a3, radius,
                            angle_ಆರಂಭ + i * angle_diff / arcs );
      a2.x = FT_MulFix(  a3.y, coef );
      a2.y = FT_MulFix( -a3.x, coef );

      a3.x += center->x;
      a3.y += center->y;
      a2.x += a3.x;
      a2.y += a3.y;

      /* add cubic arc */
      error = ft_stroke_border_cubicto( border, &a1, &a2, &a3 );
      ಆದರೆ ( error )
        ನಿಲ್ಲಿಸು;

      /* a0 = a3; */
      a1.x = a3.x - a2.x + a3.x;
      a1.y = a3.y - a2.y + a3.y;
    }

    ಹಿಂತಿರುಗಿ error;
  }


  ಸ್ಥಿರ FT_Error
  ft_stroke_border_moveto( FT_StrokeBorder  border,
                           FT_ಸದಿಶ*       ಗಮ್ಯ )
  {
    /* close current open path if any ? */
    ಆದರೆ ( border->ಆರಂಭ >= 0 )
      ft_stroke_border_close( border, FALSE );

    border->ಆರಂಭ = (FT_Int)border->ಅಂಕೆ_points;
    border->movable = FALSE;

    ಹಿಂತಿರುಗಿ ft_stroke_border_lineto( border, ಗಮ್ಯ, FALSE );
  }


  ಸ್ಥಿರ ಶೂನ್ಯ
  ft_stroke_border_init( FT_StrokeBorder  border,
                         FT_ಸ್ಮರಣೆ        ಸ್ಮರಣೆ )
  {
    border->ಸ್ಮರಣೆ = ಸ್ಮರಣೆ;
    border->points = ಖಾಲಿ;
    border->ಟ್ಯಾಗುಗಳು   = ಖಾಲಿ;

    border->ಅಂಕೆ_points = 0;
    border->max_points = 0;
    border->ಆರಂಭ      = -1;
    border->valid      = FALSE;
  }


  ಸ್ಥಿರ ಶೂನ್ಯ
  ft_stroke_border_reset( FT_StrokeBorder  border )
  {
    border->ಅಂಕೆ_points = 0;
    border->ಆರಂಭ      = -1;
    border->valid      = FALSE;
  }


  ಸ್ಥಿರ ಶೂನ್ಯ
  ft_stroke_border_ಮುಗಿದ( FT_StrokeBorder  border )
  {
    FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ = border->ಸ್ಮರಣೆ;


    FT_FREE( border->points );
    FT_FREE( border->ಟ್ಯಾಗುಗಳು );

    border->ಅಂಕೆ_points = 0;
    border->max_points = 0;
    border->ಆರಂಭ      = -1;
    border->valid      = FALSE;
  }


  ಸ್ಥಿರ FT_Error
  ft_stroke_border_get_counts( FT_StrokeBorder  border,
                               FT_UInt         *anum_points,
                               FT_UInt         *anum_contours )
  {
    FT_Error  error        = FT_Err_Ok;
    FT_UInt   ಅಂಕೆ_points   = 0;
    FT_UInt   ಅಂಕೆ_contours = 0;

    FT_UInt     ಎಣಿಕೆ_2      = border->ಅಂಕೆ_points;
    FT_Byte*    ಟ್ಯಾಗುಗಳು       = border->ಟ್ಯಾಗುಗಳು;
    FT_Int      ಒಳ_contour = 0;


    ಗಾಗಿ ( ; ಎಣಿಕೆ_2 > 0; ಎಣಿಕೆ_2--, ಅಂಕೆ_points++, ಟ್ಯಾಗುಗಳು++ )
    {
      ಆದರೆ ( ಟ್ಯಾಗುಗಳು[0] & FT_STROKE_ಟ್ಯಾಗ್_BEGIN )
      {
        ಆದರೆ ( ಒಳ_contour != 0 )
          ಹೋಗು Fail;

        ಒಳ_contour = 1;
      }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ( ಒಳ_contour == 0 )
        ಹೋಗು Fail;

      ಆದರೆ ( ಟ್ಯಾಗುಗಳು[0] & FT_STROKE_ಟ್ಯಾಗ್_ಕೊನೆ )
      {
        ಒಳ_contour = 0;
        ಅಂಕೆ_contours++;
      }
    }

    ಆದರೆ ( ಒಳ_contour != 0 )
      ಹೋಗು Fail;

    border->valid = TRUE;

  Exit:
    *anum_points   = ಅಂಕೆ_points;
    *anum_contours = ಅಂಕೆ_contours;
    ಹಿಂತಿರುಗಿ error;

  Fail:
    ಅಂಕೆ_points   = 0;
    ಅಂಕೆ_contours = 0;
    error        = FT_THROW( Invalid_Outline );
    ಹೋಗು Exit;
  }


  ಸ್ಥಿರ ಶೂನ್ಯ
  ft_stroke_border_export( FT_StrokeBorder  border,
                           FT_Outline*      outline )
  {
    /* copy point locations */
    ಆದರೆ ( border->ಅಂಕೆ_points )
      FT_ARRAY_COPY( outline->points + outline->n_points,
                     border->points,
                     border->ಅಂಕೆ_points );

    /* copy tags */
    {
      FT_UInt   ಎಣಿಕೆ_2 = border->ಅಂಕೆ_points;
      FT_Byte*  ಪಠ  = border->ಟ್ಯಾಗುಗಳು;
      FT_Byte*  ಲಿಖ = outline->ಟ್ಯಾಗುಗಳು + outline->n_points;


      ಗಾಗಿ ( ; ಎಣಿಕೆ_2 > 0; ಎಣಿಕೆ_2--, ಪಠ++, ಲಿಖ++ )
      {
        ಆದರೆ ( *ಪಠ & FT_STROKE_ಟ್ಯಾಗ್_ON )
          *ಲಿಖ = FT_ವಕ್ರ_ಟ್ಯಾಗ್_ON;
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ( *ಪಠ & FT_STROKE_ಟ್ಯಾಗ್_CUBIC )
          *ಲಿಖ = FT_ವಕ್ರ_ಟ್ಯಾಗ್_CUBIC;
        ಇಲ್ಲದಿದ್ದರೆ
          *ಲಿಖ = FT_ವಕ್ರ_ಟ್ಯಾಗ್_CONIC;
      }
    }

    /* copy contours */
    {
      FT_UInt     ಎಣಿಕೆ_2 = border->ಅಂಕೆ_points;
      FT_Byte*    ಟ್ಯಾಗುಗಳು  = border->ಟ್ಯಾಗುಗಳು;
      FT_UShort*  ಲಿಖ = outline->contours + outline->n_contours;
      FT_UShort   idx   = outline->n_points;


      ಗಾಗಿ ( ; ಎಣಿಕೆ_2 > 0; ಎಣಿಕೆ_2--, ಟ್ಯಾಗುಗಳು++, idx++ )
      {
        ಆದರೆ ( *ಟ್ಯಾಗುಗಳು & FT_STROKE_ಟ್ಯಾಗ್_ಕೊನೆ )
        {
          *ಲಿಖ++ = idx;
          outline->n_contours++;
        }
      }
    }

    outline->n_points += (FT_UShort)border->ಅಂಕೆ_points;

    FT_ASSERT( FT_Outline_Check( outline ) == 0 );
  }


  /*************************************************************************/
  /*************************************************************************/
  /*****                                                               *****/
  /*****                           STROKER                             *****/
  /*****                                                               *****/
  /*************************************************************************/
  /*************************************************************************/

#ವ್ಯಾಖ್ಯಾನಿಸು FT_SIDE_ಗಮ್ಯ_ROTATE( s )   ( FT_ANGLE_PI2 - (s) * FT_ANGLE_PI )

  ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ  FT_StrokerRec_
  {
    FT_Angle             angle_ಒಳ;             /* direction into curr join */
    FT_Angle             angle_ಹೊರಹೋಗು;            /* direction out of join  */
    FT_ಸದಿಶ            center;               /* current position */
    FT_Fixed             ಸಾಲು_ಉದ್ದಳತೆ;          /* length of last lineto */
    FT_Bool              ಮೊದಲ_ಬಿಂದು;          /* is this the start? */
    FT_Bool              subpath_open;         /* is the subpath open? */
    FT_Angle             subpath_angle;        /* subpath start direction */
    FT_ಸದಿಶ            subpath_ಆರಂಭ;        /* subpath start position */
    FT_Fixed             subpath_ಸಾಲು_ಉದ್ದಳತೆ;  /* subpath start lineto len */
    FT_Bool              handle_wide_strokes;  /* use wide strokes logic? */

    FT_Stroker_ಸಾಲು_Cap   ಸಾಲು_cap;
    FT_Stroker_ಸಾಲು_Join  ಸಾಲು_join;
    FT_Stroker_ಸಾಲು_Join  ಸಾಲು_join_saved;
    FT_Fixed             miter_ಎಲ್ಲೆ;
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
    FT_ಸ್ಮರಣೆ   ಸ್ಮರಣೆ;
    FT_Stroker  stroker = ಖಾಲಿ;


    ಆದರೆ ( !library )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_Library_Handle );

    ಆದರೆ ( !astroker )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

    ಸ್ಮರಣೆ = library->ಸ್ಮರಣೆ;

    ಆದರೆ ( !FT_NEW( stroker ) )
    {
      stroker->library = library;

      ft_stroke_border_init( &stroker->borders[0], ಸ್ಮರಣೆ );
      ft_stroke_border_init( &stroker->borders[1], ಸ್ಮರಣೆ );
    }

    *astroker = stroker;

    ಹಿಂತಿರುಗಿ error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( ಶೂನ್ಯ )
  FT_Stroker_ಗಣ( FT_Stroker           stroker,
                  FT_Fixed             radius,
                  FT_Stroker_ಸಾಲು_Cap   ಸಾಲು_cap,
                  FT_Stroker_ಸಾಲು_Join  ಸಾಲು_join,
                  FT_Fixed             miter_ಎಲ್ಲೆ )
  {
    ಆದರೆ ( !stroker )
      ಹಿಂತಿರುಗಿ;

    stroker->radius      = radius;
    stroker->ಸಾಲು_cap    = ಸಾಲು_cap;
    stroker->ಸಾಲು_join   = ಸಾಲು_join;
    stroker->miter_ಎಲ್ಲೆ = miter_ಎಲ್ಲೆ;

    /* ensure miter limit has sensible value */
    ಆದರೆ ( stroker->miter_ಎಲ್ಲೆ < 0x10000L )
      stroker->miter_ಎಲ್ಲೆ = 0x10000L;

    /* save line join style:                                           */
    /* line join style can be temporarily changed when stroking curves */
    stroker->ಸಾಲು_join_saved = ಸಾಲು_join;

    FT_Stroker_Rewind( stroker );
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( ಶೂನ್ಯ )
  FT_Stroker_Rewind( FT_Stroker  stroker )
  {
    ಆದರೆ ( stroker )
    {
      ft_stroke_border_reset( &stroker->borders[0] );
      ft_stroke_border_reset( &stroker->borders[1] );
    }
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( ಶೂನ್ಯ )
  FT_Stroker_ಮುಗಿದ( FT_Stroker  stroker )
  {
    ಆದರೆ ( stroker )
    {
      FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ = stroker->library->ಸ್ಮರಣೆ;


      ft_stroke_border_ಮುಗಿದ( &stroker->borders[0] );
      ft_stroke_border_ಮುಗಿದ( &stroker->borders[1] );

      stroker->library = ಖಾಲಿ;
      FT_FREE( stroker );
    }
  }


  /* create a circular arc at a corner or cap */
  ಸ್ಥಿರ FT_Error
  ft_stroker_arcto( FT_Stroker  stroker,
                    FT_Int      side )
  {
    FT_Angle         ಒಟ್ಟು, rotate;
    FT_Fixed         radius = stroker->radius;
    FT_Error         error  = FT_Err_Ok;
    FT_StrokeBorder  border = stroker->borders + side;


    rotate = FT_SIDE_ಗಮ್ಯ_ROTATE( side );

    ಒಟ್ಟು = FT_Angle_Diff( stroker->angle_ಒಳ, stroker->angle_ಹೊರಹೋಗು );
    ಆದರೆ ( ಒಟ್ಟು == FT_ANGLE_PI )
      ಒಟ್ಟು = -rotate * 2;

    error = ft_stroke_border_arcto( border,
                                    &stroker->center,
                                    radius,
                                    stroker->angle_ಒಳ + rotate,
                                    ಒಟ್ಟು );
    border->movable = FALSE;
    ಹಿಂತಿರುಗಿ error;
  }


  /* add a cap at the end of an opened path */
  ಸ್ಥಿರ FT_Error
  ft_stroker_cap( FT_Stroker  stroker,
                  FT_Angle    angle,
                  FT_Int      side )
  {
    FT_Error  error = FT_Err_Ok;


    ಆದರೆ ( stroker->ಸಾಲು_cap == FT_STROKER_LINECAP_ಸುತ್ತು )
    {
      /* add a round cap */
      stroker->angle_ಒಳ  = angle;
      stroker->angle_ಹೊರಹೋಗು = angle + FT_ANGLE_PI;

      error = ft_stroker_arcto( stroker, side );
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
      /* add a square or butt cap */
      FT_ಸದಿಶ        ಮಧ್ಯ, delta;
      FT_Fixed         radius = stroker->radius;
      FT_StrokeBorder  border = stroker->borders + side;


      /* compute middle point and first angle point */
      FT_ಸದಿಶ_From_Polar( &ಮಧ್ಯ, radius, angle );
      delta.x = side ?  ಮಧ್ಯ.y : -ಮಧ್ಯ.y;
      delta.y = side ? -ಮಧ್ಯ.x :  ಮಧ್ಯ.x;

      ಆದರೆ ( stroker->ಸಾಲು_cap == FT_STROKER_LINECAP_ವರ್ಗಫಲ )
      {
        ಮಧ್ಯ.x += stroker->center.x;
        ಮಧ್ಯ.y += stroker->center.y;
      }
      ಇಲ್ಲದಿದ್ದರೆ  /* FT_STROKER_LINECAP_BUTT */
      {
        ಮಧ್ಯ.x  = stroker->center.x;
        ಮಧ್ಯ.y  = stroker->center.y;
      }

      delta.x  += ಮಧ್ಯ.x;
      delta.y  += ಮಧ್ಯ.y;

      error = ft_stroke_border_lineto( border, &delta, FALSE );
      ಆದರೆ ( error )
        ಹೋಗು Exit;

      /* compute second angle point */
      delta.x = ಮಧ್ಯ.x - delta.x + ಮಧ್ಯ.x;
      delta.y = ಮಧ್ಯ.y - delta.y + ಮಧ್ಯ.y;

      error = ft_stroke_border_lineto( border, &delta, FALSE );
    }

  Exit:
    ಹಿಂತಿರುಗಿ error;
  }


  /* process an inside corner, i.e. compute intersection */
  ಸ್ಥಿರ FT_Error
  ft_stroker_inside( FT_Stroker  stroker,
                     FT_Int      side,
                     FT_Fixed    ಸಾಲು_ಉದ್ದಳತೆ )
  {
    FT_StrokeBorder  border = stroker->borders + side;
    FT_Angle         phi, theta, rotate;
    FT_Fixed         ಉದ್ದಳತೆ;
    FT_ಸದಿಶ        sigma = { 0, 0 };
    FT_ಸದಿಶ        delta;
    FT_Error         error = FT_Err_Ok;
    FT_Bool          intersect;          /* use intersection of lines? */


    rotate = FT_SIDE_ಗಮ್ಯ_ROTATE( side );

    theta = FT_Angle_Diff( stroker->angle_ಒಳ, stroker->angle_ಹೊರಹೋಗು ) / 2;

    /* Only intersect borders if between two lineto's and both */
    /* lines are long enough (line_length is zero for curves). */
    /* Also avoid U-turns of nearly 180 degree.                */
    ಆದರೆ ( !border->movable || ಸಾಲು_ಉದ್ದಳತೆ == 0  ||
         theta > 0x59C000 || theta < -0x59C000 )
      intersect = FALSE;
    ಇಲ್ಲದಿದ್ದರೆ
    {
      /* compute minimum required length of lines */
      FT_Fixed  min_ಉದ್ದಳತೆ;


      FT_ಸದಿಶ_Unit( &sigma, theta );
      min_ಉದ್ದಳತೆ =
        ft_pos_abs( FT_MulDiv( stroker->radius, sigma.y, sigma.x ) );

      intersect = FT_BOOL( min_ಉದ್ದಳತೆ                         &&
                           stroker->ಸಾಲು_ಉದ್ದಳತೆ >= min_ಉದ್ದಳತೆ &&
                           ಸಾಲು_ಉದ್ದಳತೆ          >= min_ಉದ್ದಳತೆ );
    }

    ಆದರೆ ( !intersect )
    {
      FT_ಸದಿಶ_From_Polar( &delta, stroker->radius,
                            stroker->angle_ಹೊರಹೋಗು + rotate );
      delta.x += stroker->center.x;
      delta.y += stroker->center.y;

      border->movable = FALSE;
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
      /* compute median angle */
      phi = stroker->angle_ಒಳ + theta + rotate;

      ಉದ್ದಳತೆ = FT_DivFix( stroker->radius, sigma.x );

      FT_ಸದಿಶ_From_Polar( &delta, ಉದ್ದಳತೆ, phi );
      delta.x += stroker->center.x;
      delta.y += stroker->center.y;
    }

    error = ft_stroke_border_lineto( border, &delta, FALSE );

    ಹಿಂತಿರುಗಿ error;
  }


  /* process an outside corner, i.e. compute bevel/miter/round */
  ಸ್ಥಿರ FT_Error
  ft_stroker_outside( FT_Stroker  stroker,
                      FT_Int      side,
                      FT_Fixed    ಸಾಲು_ಉದ್ದಳತೆ )
  {
    FT_StrokeBorder  border = stroker->borders + side;
    FT_Error         error;
    FT_Angle         rotate;


    ಆದರೆ ( stroker->ಸಾಲು_join == FT_STROKER_LINEJOIN_ಸುತ್ತು )
      error = ft_stroker_arcto( stroker, side );
    ಇಲ್ಲದಿದ್ದರೆ
    {
      /* this is a mitered (pointed) or beveled (truncated) corner */
      FT_Fixed   radius = stroker->radius;
      FT_ಸದಿಶ  sigma = { 0, 0 };
      FT_Angle   theta = 0, phi = 0;
      FT_Bool    bevel, fixed_bevel;


      rotate = FT_SIDE_ಗಮ್ಯ_ROTATE( side );

      bevel =
        FT_BOOL( stroker->ಸಾಲು_join == FT_STROKER_LINEJOIN_BEVEL );

      fixed_bevel =
        FT_BOOL( stroker->ಸಾಲು_join != FT_STROKER_LINEJOIN_MITER_VARIABLE );

      /* check miter limit first */
      ಆದರೆ ( !bevel )
      {
        theta = FT_Angle_Diff( stroker->angle_ಒಳ, stroker->angle_ಹೊರಹೋಗು ) / 2;

        ಆದರೆ ( theta == FT_ANGLE_PI2 )
          theta = -rotate;

        phi = stroker->angle_ಒಳ + theta + rotate;

        FT_ಸದಿಶ_From_Polar( &sigma, stroker->miter_ಎಲ್ಲೆ, theta );

        /* is miter limit exceeded? */
        ಆದರೆ ( sigma.x < 0x10000L )
        {
          /* don't create variable bevels for very small deviations; */
          /* FT_Sin(x) = 0 for x <= 57                               */
          ಆದರೆ ( fixed_bevel || ft_pos_abs( theta ) > 57 )
            bevel = TRUE;
        }
      }

      ಆದರೆ ( bevel )  /* this is a bevel (broken angle) */
      {
        ಆದರೆ ( fixed_bevel )
        {
          /* the outer corners are simply joined together */
          FT_ಸದಿಶ  delta;


          /* add bevel */
          FT_ಸದಿಶ_From_Polar( &delta,
                                radius,
                                stroker->angle_ಹೊರಹೋಗು + rotate );
          delta.x += stroker->center.x;
          delta.y += stroker->center.y;

          border->movable = FALSE;
          error = ft_stroke_border_lineto( border, &delta, FALSE );
        }
        ಇಲ್ಲದಿದ್ದರೆ /* variable bevel or clipped miter */
        {
          /* the miter is truncated */
          FT_ಸದಿಶ  ಮಧ್ಯ, delta;
          FT_Fixed   coef;


          /* compute middle point and first angle point */
          FT_ಸದಿಶ_From_Polar( &ಮಧ್ಯ,
                                FT_MulFix( radius, stroker->miter_ಎಲ್ಲೆ ),
                                phi );

          coef    = FT_DivFix(  0x10000L - sigma.x, sigma.y );
          delta.x = FT_MulFix(  ಮಧ್ಯ.y, coef );
          delta.y = FT_MulFix( -ಮಧ್ಯ.x, coef );

          ಮಧ್ಯ.x += stroker->center.x;
          ಮಧ್ಯ.y += stroker->center.y;
          delta.x  += ಮಧ್ಯ.x;
          delta.y  += ಮಧ್ಯ.y;

          error = ft_stroke_border_lineto( border, &delta, FALSE );
          ಆದರೆ ( error )
            ಹೋಗು Exit;

          /* compute second angle point */
          delta.x = ಮಧ್ಯ.x - delta.x + ಮಧ್ಯ.x;
          delta.y = ಮಧ್ಯ.y - delta.y + ಮಧ್ಯ.y;

          error = ft_stroke_border_lineto( border, &delta, FALSE );
          ಆದರೆ ( error )
            ಹೋಗು Exit;

          /* finally, add an end point; only needed if not lineto */
          /* (line_length is zero for curves)                     */
          ಆದರೆ ( ಸಾಲು_ಉದ್ದಳತೆ == 0 )
          {
            FT_ಸದಿಶ_From_Polar( &delta,
                                  radius,
                                  stroker->angle_ಹೊರಹೋಗು + rotate );

            delta.x += stroker->center.x;
            delta.y += stroker->center.y;

            error = ft_stroke_border_lineto( border, &delta, FALSE );
          }
        }
      }
      ಇಲ್ಲದಿದ್ದರೆ /* this is a miter (intersection) */
      {
        FT_Fixed   ಉದ್ದಳತೆ;
        FT_ಸದಿಶ  delta;


        ಉದ್ದಳತೆ = FT_MulDiv( stroker->radius, stroker->miter_ಎಲ್ಲೆ, sigma.x );

        FT_ಸದಿಶ_From_Polar( &delta, ಉದ್ದಳತೆ, phi );
        delta.x += stroker->center.x;
        delta.y += stroker->center.y;

        error = ft_stroke_border_lineto( border, &delta, FALSE );
        ಆದರೆ ( error )
          ಹೋಗು Exit;

        /* now add an end point; only needed if not lineto */
        /* (line_length is zero for curves)                */
        ಆದರೆ ( ಸಾಲು_ಉದ್ದಳತೆ == 0 )
        {
          FT_ಸದಿಶ_From_Polar( &delta,
                                stroker->radius,
                                stroker->angle_ಹೊರಹೋಗು + rotate );
          delta.x += stroker->center.x;
          delta.y += stroker->center.y;

          error = ft_stroke_border_lineto( border, &delta, FALSE );
        }
      }
    }

  Exit:
    ಹಿಂತಿರುಗಿ error;
  }


  ಸ್ಥಿರ FT_Error
  ft_stroker_process_ಮೂಲೆ( FT_Stroker  stroker,
                             FT_Fixed    ಸಾಲು_ಉದ್ದಳತೆ )
  {
    FT_Error  error = FT_Err_Ok;
    FT_Angle  turn;
    FT_Int    inside_side;


    turn = FT_Angle_Diff( stroker->angle_ಒಳ, stroker->angle_ಹೊರಹೋಗು );

    /* no specific corner processing is required if the turn is 0 */
    ಆದರೆ ( turn == 0 )
      ಹೋಗು Exit;

    /* when we turn to the right, the inside side is 0 */
    /* otherwise, the inside side is 1 */
    inside_side = ( turn < 0 );

    /* process the inside side */
    error = ft_stroker_inside( stroker, inside_side, ಸಾಲು_ಉದ್ದಳತೆ );
    ಆದರೆ ( error )
      ಹೋಗು Exit;

    /* process the outside side */
    error = ft_stroker_outside( stroker, !inside_side, ಸಾಲು_ಉದ್ದಳತೆ );

  Exit:
    ಹಿಂತಿರುಗಿ error;
  }


  /* add two points to the left and right borders corresponding to the */
  /* start of the subpath                                              */
  ಸ್ಥಿರ FT_Error
  ft_stroker_subpath_ಆರಂಭ( FT_Stroker  stroker,
                            FT_Angle    ಆರಂಭ_angle,
                            FT_Fixed    ಸಾಲು_ಉದ್ದಳತೆ )
  {
    FT_ಸದಿಶ        delta;
    FT_ಸದಿಶ        ಬಿಂದು;
    FT_Error         error;
    FT_StrokeBorder  border;


    FT_ಸದಿಶ_From_Polar( &delta, stroker->radius,
                          ಆರಂಭ_angle + FT_ANGLE_PI2 );

    ಬಿಂದು.x = stroker->center.x + delta.x;
    ಬಿಂದು.y = stroker->center.y + delta.y;

    border = stroker->borders;
    error = ft_stroke_border_moveto( border, &ಬಿಂದು );
    ಆದರೆ ( error )
      ಹೋಗು Exit;

    ಬಿಂದು.x = stroker->center.x - delta.x;
    ಬಿಂದು.y = stroker->center.y - delta.y;

    border++;
    error = ft_stroke_border_moveto( border, &ಬಿಂದು );

    /* save angle, position, and line length for last join */
    /* (line_length is zero for curves)                    */
    stroker->subpath_angle       = ಆರಂಭ_angle;
    stroker->ಮೊದಲ_ಬಿಂದು         = FALSE;
    stroker->subpath_ಸಾಲು_ಉದ್ದಳತೆ = ಸಾಲು_ಉದ್ದಳತೆ;

  Exit:
    ಹಿಂತಿರುಗಿ error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Stroker_ಸಾಲು_ಗಮ್ಯ( FT_Stroker  stroker,
                     FT_ಸದಿಶ*  ಗಮ್ಯ )
  {
    FT_Error         error = FT_Err_Ok;
    FT_StrokeBorder  border;
    FT_ಸದಿಶ        delta;
    FT_Angle         angle;
    FT_Int           side;
    FT_Fixed         ಸಾಲು_ಉದ್ದಳತೆ;


    ಆದರೆ ( !stroker || !ಗಮ್ಯ )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

    delta.x = ಗಮ್ಯ->x - stroker->center.x;
    delta.y = ಗಮ್ಯ->y - stroker->center.y;

    /* a zero-length lineto is a no-op; avoid creating a spurious corner */
    ಆದರೆ ( delta.x == 0 && delta.y == 0 )
       ಹೋಗು Exit;

    /* compute length of line */
    ಸಾಲು_ಉದ್ದಳತೆ = FT_ಸದಿಶ_ಉದ್ದಳತೆ( &delta );

    angle = FT_Atan2( delta.x, delta.y );
    FT_ಸದಿಶ_From_Polar( &delta, stroker->radius, angle + FT_ANGLE_PI2 );

    /* process corner if necessary */
    ಆದರೆ ( stroker->ಮೊದಲ_ಬಿಂದು )
    {
      /* This is the first segment of a subpath.  We need to     */
      /* add a point to each border at their respective starting */
      /* point locations.                                        */
      error = ft_stroker_subpath_ಆರಂಭ( stroker, angle, ಸಾಲು_ಉದ್ದಳತೆ );
      ಆದರೆ ( error )
        ಹೋಗು Exit;
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
      /* process the current corner */
      stroker->angle_ಹೊರಹೋಗು = angle;
      error = ft_stroker_process_ಮೂಲೆ( stroker, ಸಾಲು_ಉದ್ದಳತೆ );
      ಆದರೆ ( error )
        ಹೋಗು Exit;
    }

    /* now add a line segment to both the `inside' and `outside' paths */
    ಗಾಗಿ ( border = stroker->borders, side = 1; side >= 0; side--, border++ )
    {
      FT_ಸದಿಶ  ಬಿಂದು;


      ಬಿಂದು.x = ಗಮ್ಯ->x + delta.x;
      ಬಿಂದು.y = ಗಮ್ಯ->y + delta.y;

      /* the ends of lineto borders are movable */
      error = ft_stroke_border_lineto( border, &ಬಿಂದು, TRUE );
      ಆದರೆ ( error )
        ಹೋಗು Exit;

      delta.x = -delta.x;
      delta.y = -delta.y;
    }

    stroker->angle_ಒಳ    = angle;
    stroker->center      = *ಗಮ್ಯ;
    stroker->ಸಾಲು_ಉದ್ದಳತೆ = ಸಾಲು_ಉದ್ದಳತೆ;

  Exit:
    ಹಿಂತಿರುಗಿ error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Stroker_Conic_ಗಮ್ಯ( FT_Stroker  stroker,
                      FT_ಸದಿಶ*  control,
                      FT_ಸದಿಶ*  ಗಮ್ಯ )
  {
    FT_Error    error = FT_Err_Ok;
    FT_ಸದಿಶ   bez_ರಾಶಿ[34];
    FT_ಸದಿಶ*  arc;
    FT_ಸದಿಶ*  ಎಲ್ಲೆ = bez_ರಾಶಿ + 30;
    FT_Bool     ಮೊದಲ_arc = TRUE;


    ಆದರೆ ( !stroker || !control || !ಗಮ್ಯ )
    {
      error = FT_THROW( Invalid_ವಾದ );
      ಹೋಗು Exit;
    }

    /* if all control points are coincident, this is a no-op; */
    /* avoid creating a spurious corner                       */
    ಆದರೆ ( FT_IS_SMALL( stroker->center.x - control->x ) &&
         FT_IS_SMALL( stroker->center.y - control->y ) &&
         FT_IS_SMALL( control->x        - ಗಮ್ಯ->x      ) &&
         FT_IS_SMALL( control->y        - ಗಮ್ಯ->y      ) )
    {
       stroker->center = *ಗಮ್ಯ;
       ಹೋಗು Exit;
    }

    arc    = bez_ರಾಶಿ;
    arc[0] = *ಗಮ್ಯ;
    arc[1] = *control;
    arc[2] = stroker->center;

    ಮಾಡು
    {
      FT_Angle  angle_ಒಳ, angle_ಹೊರಹೋಗು;


      /* initialize with current direction */
      angle_ಒಳ = angle_ಹೊರಹೋಗು = stroker->angle_ಒಳ;

      ಆದರೆ ( arc < ಎಲ್ಲೆ                                             &&
           !ft_conic_is_small_enough( arc, &angle_ಒಳ, &angle_ಹೊರಹೋಗು ) )
      {
        ಆದರೆ ( stroker->ಮೊದಲ_ಬಿಂದು )
          stroker->angle_ಒಳ = angle_ಒಳ;

        ft_conic_ವಿಭಜಿಸು( arc );
        arc += 2;
        ಮುಂದುವರಿ;
      }

      ಆದರೆ ( ಮೊದಲ_arc )
      {
        ಮೊದಲ_arc = FALSE;

        /* process corner if necessary */
        ಆದರೆ ( stroker->ಮೊದಲ_ಬಿಂದು )
          error = ft_stroker_subpath_ಆರಂಭ( stroker, angle_ಒಳ, 0 );
        ಇಲ್ಲದಿದ್ದರೆ
        {
          stroker->angle_ಹೊರಹೋಗು = angle_ಒಳ;
          error = ft_stroker_process_ಮೂಲೆ( stroker, 0 );
        }
      }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ( ft_pos_abs( FT_Angle_Diff( stroker->angle_ಒಳ, angle_ಒಳ ) ) >
                  FT_SMALL_CONIC_ಮಿತಿ / 4                             )
      {
        /* if the deviation from one arc to the next is too great, */
        /* add a round corner                                      */
        stroker->center    = arc[2];
        stroker->angle_ಹೊರಹೋಗು = angle_ಒಳ;
        stroker->ಸಾಲು_join = FT_STROKER_LINEJOIN_ಸುತ್ತು;

        error = ft_stroker_process_ಮೂಲೆ( stroker, 0 );

        /* reinstate line join style */
        stroker->ಸಾಲು_join = stroker->ಸಾಲು_join_saved;
      }

      ಆದರೆ ( error )
        ಹೋಗು Exit;

      /* the arc's angle is small enough; we can add it directly to each */
      /* border                                                          */
      {
        FT_ಸದಿಶ        ctrl, ಕೊನೆ;
        FT_Angle         theta, phi, rotate, alpha0 = 0;
        FT_Fixed         ಉದ್ದಳತೆ;
        FT_StrokeBorder  border;
        FT_Int           side;


        theta  = FT_Angle_Diff( angle_ಒಳ, angle_ಹೊರಹೋಗು ) / 2;
        phi    = angle_ಒಳ + theta;
        ಉದ್ದಳತೆ = FT_DivFix( stroker->radius, FT_Cos( theta ) );

        /* compute direction of original arc */
        ಆದರೆ ( stroker->handle_wide_strokes )
          alpha0 = FT_Atan2( arc[0].x - arc[2].x, arc[0].y - arc[2].y );

        ಗಾಗಿ ( border = stroker->borders, side = 0;
              side <= 1;
              side++, border++ )
        {
          rotate = FT_SIDE_ಗಮ್ಯ_ROTATE( side );

          /* compute control point */
          FT_ಸದಿಶ_From_Polar( &ctrl, ಉದ್ದಳತೆ, phi + rotate );
          ctrl.x += arc[1].x;
          ctrl.y += arc[1].y;

          /* compute end point */
          FT_ಸದಿಶ_From_Polar( &ಕೊನೆ, stroker->radius, angle_ಹೊರಹೋಗು + rotate );
          ಕೊನೆ.x += arc[0].x;
          ಕೊನೆ.y += arc[0].y;

          ಆದರೆ ( stroker->handle_wide_strokes )
          {
            FT_ಸದಿಶ  ಆರಂಭ;
            FT_Angle   alpha1;


            /* determine whether the border radius is greater than the */
            /* radius of curvature of the original arc                 */
            ಆರಂಭ = border->points[border->ಅಂಕೆ_points - 1];

            alpha1 = FT_Atan2( ಕೊನೆ.x - ಆರಂಭ.x, ಕೊನೆ.y - ಆರಂಭ.y );

            /* is the direction of the border arc opposite to */
            /* that of the original arc? */
            ಆದರೆ ( ft_pos_abs( FT_Angle_Diff( alpha0, alpha1 ) ) >
                   FT_ANGLE_PI / 2                             )
            {
              FT_Angle   beta, gamma;
              FT_ಸದಿಶ  bvec, delta;
              FT_Fixed   blen, sinA, sinB, alen;


              /* use the sine rule to find the intersection point */
              beta  = FT_Atan2( arc[2].x - ಆರಂಭ.x, arc[2].y - ಆರಂಭ.y );
              gamma = FT_Atan2( arc[0].x - ಕೊನೆ.x,   arc[0].y - ಕೊನೆ.y );

              bvec.x = ಕೊನೆ.x - ಆರಂಭ.x;
              bvec.y = ಕೊನೆ.y - ಆರಂಭ.y;

              blen = FT_ಸದಿಶ_ಉದ್ದಳತೆ( &bvec );

              sinA = ft_pos_abs( FT_Sin( alpha1 - gamma ) );
              sinB = ft_pos_abs( FT_Sin( beta - gamma ) );

              alen = FT_MulDiv( blen, sinA, sinB );

              FT_ಸದಿಶ_From_Polar( &delta, alen, beta );
              delta.x += ಆರಂಭ.x;
              delta.y += ಆರಂಭ.y;

              /* circumnavigate the negative sector backwards */
              border->movable = FALSE;
              error = ft_stroke_border_lineto( border, &delta, FALSE );
              ಆದರೆ ( error )
                ಹೋಗು Exit;
              error = ft_stroke_border_lineto( border, &ಕೊನೆ, FALSE );
              ಆದರೆ ( error )
                ಹೋಗು Exit;
              error = ft_stroke_border_conicto( border, &ctrl, &ಆರಂಭ );
              ಆದರೆ ( error )
                ಹೋಗು Exit;
              /* and then move to the endpoint */
              error = ft_stroke_border_lineto( border, &ಕೊನೆ, FALSE );
              ಆದರೆ ( error )
                ಹೋಗು Exit;

              ಮುಂದುವರಿ;
            }

            /* else fall through */
          }

          /* simply add an arc */
          error = ft_stroke_border_conicto( border, &ctrl, &ಕೊನೆ );
          ಆದರೆ ( error )
            ಹೋಗು Exit;
        }
      }

      stroker->angle_ಒಳ = angle_ಹೊರಹೋಗು;

      ಆದರೆ ( arc == bez_ರಾಶಿ )
        ನಿಲ್ಲಿಸು;
      arc -= 2;
    } ಆಗ ( 1 );

    stroker->center      = *ಗಮ್ಯ;
    stroker->ಸಾಲು_ಉದ್ದಳತೆ = 0;

  Exit:
    ಹಿಂತಿರುಗಿ error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Stroker_Cubic_ಗಮ್ಯ( FT_Stroker  stroker,
                      FT_ಸದಿಶ*  control1,
                      FT_ಸದಿಶ*  control2,
                      FT_ಸದಿಶ*  ಗಮ್ಯ )
  {
    FT_Error    error = FT_Err_Ok;
    FT_ಸದಿಶ   bez_ರಾಶಿ[37];
    FT_ಸದಿಶ*  arc;
    FT_ಸದಿಶ*  ಎಲ್ಲೆ = bez_ರಾಶಿ + 32;
    FT_Bool     ಮೊದಲ_arc = TRUE;


    ಆದರೆ ( !stroker || !control1 || !control2 || !ಗಮ್ಯ )
    {
      error = FT_THROW( Invalid_ವಾದ );
      ಹೋಗು Exit;
    }

    /* if all control points are coincident, this is a no-op; */
    /* avoid creating a spurious corner */
    ಆದರೆ ( FT_IS_SMALL( stroker->center.x - control1->x ) &&
         FT_IS_SMALL( stroker->center.y - control1->y ) &&
         FT_IS_SMALL( control1->x       - control2->x ) &&
         FT_IS_SMALL( control1->y       - control2->y ) &&
         FT_IS_SMALL( control2->x       - ಗಮ್ಯ->x       ) &&
         FT_IS_SMALL( control2->y       - ಗಮ್ಯ->y       ) )
    {
       stroker->center = *ಗಮ್ಯ;
       ಹೋಗು Exit;
    }

    arc    = bez_ರಾಶಿ;
    arc[0] = *ಗಮ್ಯ;
    arc[1] = *control2;
    arc[2] = *control1;
    arc[3] = stroker->center;

    ಮಾಡು
    {
      FT_Angle  angle_ಒಳ, angle_mid, angle_ಹೊರಹೋಗು;


      /* initialize with current direction */
      angle_ಒಳ = angle_ಹೊರಹೋಗು = angle_mid = stroker->angle_ಒಳ;

      ಆದರೆ ( arc < ಎಲ್ಲೆ                                         &&
           !ft_cubic_is_small_enough( arc, &angle_ಒಳ,
                                      &angle_mid, &angle_ಹೊರಹೋಗು ) )
      {
        ಆದರೆ ( stroker->ಮೊದಲ_ಬಿಂದು )
          stroker->angle_ಒಳ = angle_ಒಳ;

        ft_cubic_ವಿಭಜಿಸು( arc );
        arc += 3;
        ಮುಂದುವರಿ;
      }

      ಆದರೆ ( ಮೊದಲ_arc )
      {
        ಮೊದಲ_arc = FALSE;

        /* process corner if necessary */
        ಆದರೆ ( stroker->ಮೊದಲ_ಬಿಂದು )
          error = ft_stroker_subpath_ಆರಂಭ( stroker, angle_ಒಳ, 0 );
        ಇಲ್ಲದಿದ್ದರೆ
        {
          stroker->angle_ಹೊರಹೋಗು = angle_ಒಳ;
          error = ft_stroker_process_ಮೂಲೆ( stroker, 0 );
        }
      }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ( ft_pos_abs( FT_Angle_Diff( stroker->angle_ಒಳ, angle_ಒಳ ) ) >
                  FT_SMALL_CUBIC_ಮಿತಿ / 4                             )
      {
        /* if the deviation from one arc to the next is too great, */
        /* add a round corner                                      */
        stroker->center    = arc[3];
        stroker->angle_ಹೊರಹೋಗು = angle_ಒಳ;
        stroker->ಸಾಲು_join = FT_STROKER_LINEJOIN_ಸುತ್ತು;

        error = ft_stroker_process_ಮೂಲೆ( stroker, 0 );

        /* reinstate line join style */
        stroker->ಸಾಲು_join = stroker->ಸಾಲು_join_saved;
      }

      ಆದರೆ ( error )
        ಹೋಗು Exit;

      /* the arc's angle is small enough; we can add it directly to each */
      /* border                                                          */
      {
        FT_ಸದಿಶ        ctrl1, ctrl2, ಕೊನೆ;
        FT_Angle         theta1, phi1, theta2, phi2, rotate, alpha0 = 0;
        FT_Fixed         length1, length2;
        FT_StrokeBorder  border;
        FT_Int           side;


        theta1  = FT_Angle_Diff( angle_ಒಳ,  angle_mid ) / 2;
        theta2  = FT_Angle_Diff( angle_mid, angle_ಹೊರಹೋಗು ) / 2;
        phi1    = ft_angle_ಸರಾಸರಿ( angle_ಒಳ,  angle_mid );
        phi2    = ft_angle_ಸರಾಸರಿ( angle_mid, angle_ಹೊರಹೋಗು );
        length1 = FT_DivFix( stroker->radius, FT_Cos( theta1 ) );
        length2 = FT_DivFix( stroker->radius, FT_Cos( theta2 ) );

        /* compute direction of original arc */
        ಆದರೆ ( stroker->handle_wide_strokes )
          alpha0 = FT_Atan2( arc[0].x - arc[3].x, arc[0].y - arc[3].y );

        ಗಾಗಿ ( border = stroker->borders, side = 0;
              side <= 1;
              side++, border++ )
        {
          rotate = FT_SIDE_ಗಮ್ಯ_ROTATE( side );

          /* compute control points */
          FT_ಸದಿಶ_From_Polar( &ctrl1, length1, phi1 + rotate );
          ctrl1.x += arc[2].x;
          ctrl1.y += arc[2].y;

          FT_ಸದಿಶ_From_Polar( &ctrl2, length2, phi2 + rotate );
          ctrl2.x += arc[1].x;
          ctrl2.y += arc[1].y;

          /* compute end point */
          FT_ಸದಿಶ_From_Polar( &ಕೊನೆ, stroker->radius, angle_ಹೊರಹೋಗು + rotate );
          ಕೊನೆ.x += arc[0].x;
          ಕೊನೆ.y += arc[0].y;

          ಆದರೆ ( stroker->handle_wide_strokes )
          {
            FT_ಸದಿಶ  ಆರಂಭ;
            FT_Angle   alpha1;


            /* determine whether the border radius is greater than the */
            /* radius of curvature of the original arc                 */
            ಆರಂಭ = border->points[border->ಅಂಕೆ_points - 1];

            alpha1 = FT_Atan2( ಕೊನೆ.x - ಆರಂಭ.x, ಕೊನೆ.y - ಆರಂಭ.y );

            /* is the direction of the border arc opposite to */
            /* that of the original arc? */
            ಆದರೆ ( ft_pos_abs( FT_Angle_Diff( alpha0, alpha1 ) ) >
                   FT_ANGLE_PI / 2                             )
            {
              FT_Angle   beta, gamma;
              FT_ಸದಿಶ  bvec, delta;
              FT_Fixed   blen, sinA, sinB, alen;


              /* use the sine rule to find the intersection point */
              beta  = FT_Atan2( arc[3].x - ಆರಂಭ.x, arc[3].y - ಆರಂಭ.y );
              gamma = FT_Atan2( arc[0].x - ಕೊನೆ.x,   arc[0].y - ಕೊನೆ.y );

              bvec.x = ಕೊನೆ.x - ಆರಂಭ.x;
              bvec.y = ಕೊನೆ.y - ಆರಂಭ.y;

              blen = FT_ಸದಿಶ_ಉದ್ದಳತೆ( &bvec );

              sinA = ft_pos_abs( FT_Sin( alpha1 - gamma ) );
              sinB = ft_pos_abs( FT_Sin( beta - gamma ) );

              alen = FT_MulDiv( blen, sinA, sinB );

              FT_ಸದಿಶ_From_Polar( &delta, alen, beta );
              delta.x += ಆರಂಭ.x;
              delta.y += ಆರಂಭ.y;

              /* circumnavigate the negative sector backwards */
              border->movable = FALSE;
              error = ft_stroke_border_lineto( border, &delta, FALSE );
              ಆದರೆ ( error )
                ಹೋಗು Exit;
              error = ft_stroke_border_lineto( border, &ಕೊನೆ, FALSE );
              ಆದರೆ ( error )
                ಹೋಗು Exit;
              error = ft_stroke_border_cubicto( border,
                                                &ctrl2,
                                                &ctrl1,
                                                &ಆರಂಭ );
              ಆದರೆ ( error )
                ಹೋಗು Exit;
              /* and then move to the endpoint */
              error = ft_stroke_border_lineto( border, &ಕೊನೆ, FALSE );
              ಆದರೆ ( error )
                ಹೋಗು Exit;

              ಮುಂದುವರಿ;
            }

            /* else fall through */
          }

          /* simply add an arc */
          error = ft_stroke_border_cubicto( border, &ctrl1, &ctrl2, &ಕೊನೆ );
          ಆದರೆ ( error )
            ಹೋಗು Exit;
        }
      }

      stroker->angle_ಒಳ = angle_ಹೊರಹೋಗು;

      ಆದರೆ ( arc == bez_ರಾಶಿ )
        ನಿಲ್ಲಿಸು;
      arc -= 3;
    } ಆಗ ( 1 );

    stroker->center      = *ಗಮ್ಯ;
    stroker->ಸಾಲು_ಉದ್ದಳತೆ = 0;

  Exit:
    ಹಿಂತಿರುಗಿ error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Stroker_Begin_Sub_ಮಾರ್ಗ( FT_Stroker  stroker,
                           FT_ಸದಿಶ*  ಗಮ್ಯ,
                           FT_Bool     open )
  {
    ಆದರೆ ( !stroker || !ಗಮ್ಯ )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

    /* We cannot process the first point, because there is not enough      */
    /* information regarding its corner/cap.  The latter will be processed */
    /* in the `FT_Stroker_EndSubPath' routine.                             */
    /*                                                                     */
    stroker->ಮೊದಲ_ಬಿಂದು  = TRUE;
    stroker->center       = *ಗಮ್ಯ;
    stroker->subpath_open = open;

    /* Determine if we need to check whether the border radius is greater */
    /* than the radius of curvature of a curve, to handle this case       */
    /* specially.  This is only required if bevel joins or butt caps may  */
    /* be created, because round & miter joins and round & square caps    */
    /* cover the negative sector created with wide strokes.               */
    stroker->handle_wide_strokes =
      FT_BOOL( stroker->ಸಾಲು_join != FT_STROKER_LINEJOIN_ಸುತ್ತು  ||
               ( stroker->subpath_open                        &&
                 stroker->ಸಾಲು_cap == FT_STROKER_LINECAP_BUTT ) );

    /* record the subpath start point for each border */
    stroker->subpath_ಆರಂಭ = *ಗಮ್ಯ;

    stroker->angle_ಒಳ = 0;

    ಹಿಂತಿರುಗಿ FT_Err_Ok;
  }


  ಸ್ಥಿರ FT_Error
  ft_stroker_ಕೂಡಿಸು_ಹಿಮ್ಮುಖ_ಎಡ( FT_Stroker  stroker,
                               FT_Bool     open )
  {
    FT_StrokeBorder  ಬಲ = stroker->borders + 0;
    FT_StrokeBorder  ಎಡ  = stroker->borders + 1;
    FT_Int           new_points;
    FT_Error         error = FT_Err_Ok;


    FT_ASSERT( ಎಡ->ಆರಂಭ >= 0 );

    new_points = (FT_Int)ಎಡ->ಅಂಕೆ_points - ಎಡ->ಆರಂಭ;
    ಆದರೆ ( new_points > 0 )
    {
      error = ft_stroke_border_grow( ಬಲ, (FT_UInt)new_points );
      ಆದರೆ ( error )
        ಹೋಗು Exit;

      {
        FT_ಸದಿಶ*  dst_ಬಿಂದು = ಬಲ->points + ಬಲ->ಅಂಕೆ_points;
        FT_Byte*    dst_ಟ್ಯಾಗ್   = ಬಲ->ಟ್ಯಾಗುಗಳು   + ಬಲ->ಅಂಕೆ_points;
        FT_ಸದಿಶ*  src_ಬಿಂದು = ಎಡ->points  + ಎಡ->ಅಂಕೆ_points - 1;
        FT_Byte*    src_ಟ್ಯಾಗ್   = ಎಡ->ಟ್ಯಾಗುಗಳು    + ಎಡ->ಅಂಕೆ_points - 1;


        ಆಗ ( src_ಬಿಂದು >= ಎಡ->points + ಎಡ->ಆರಂಭ )
        {
          *dst_ಬಿಂದು = *src_ಬಿಂದು;
          *dst_ಟ್ಯಾಗ್   = *src_ಟ್ಯಾಗ್;

          ಆದರೆ ( open )
            dst_ಟ್ಯಾಗ್[0] &= ~FT_STROKE_ಟ್ಯಾಗ್_BEGIN_ಕೊನೆ;
          ಇಲ್ಲದಿದ್ದರೆ
          {
            FT_Byte  ttag =
                       (FT_Byte)( dst_ಟ್ಯಾಗ್[0] & FT_STROKE_ಟ್ಯಾಗ್_BEGIN_ಕೊನೆ );


            /* switch begin/end tags if necessary */
            ಆದರೆ ( ttag == FT_STROKE_ಟ್ಯಾಗ್_BEGIN ||
                 ttag == FT_STROKE_ಟ್ಯಾಗ್_ಕೊನೆ   )
              dst_ಟ್ಯಾಗ್[0] ^= FT_STROKE_ಟ್ಯಾಗ್_BEGIN_ಕೊನೆ;
          }

          src_ಬಿಂದು--;
          src_ಟ್ಯಾಗ್--;
          dst_ಬಿಂದು++;
          dst_ಟ್ಯಾಗ್++;
        }
      }

      ಎಡ->ಅಂಕೆ_points   = (FT_UInt)ಎಡ->ಆರಂಭ;
      ಬಲ->ಅಂಕೆ_points += (FT_UInt)new_points;

      ಬಲ->movable = FALSE;
      ಎಡ->movable  = FALSE;
    }

  Exit:
    ಹಿಂತಿರುಗಿ error;
  }


  /* documentation is in ftstroke.h */

  /* there's a lot of magic in this function! */
  FT_EXPORT_DEF( FT_Error )
  FT_Stroker_ಕೊನೆ_Sub_ಮಾರ್ಗ( FT_Stroker  stroker )
  {
    FT_Error  error = FT_Err_Ok;


    ಆದರೆ ( !stroker )
    {
      error = FT_THROW( Invalid_ವಾದ );
      ಹೋಗು Exit;
    }

    /* don't try to end the path if no segments have been generated */
    ಆದರೆ ( stroker->ಮೊದಲ_ಬಿಂದು )
      ಹೋಗು Exit;

    ಆದರೆ ( stroker->subpath_open )
    {
      FT_StrokeBorder  ಬಲ = stroker->borders;


      /* All right, this is an opened path, we need to add a cap between */
      /* right & left, add the reverse of left, then add a final cap     */
      /* between left & right.                                           */
      error = ft_stroker_cap( stroker, stroker->angle_ಒಳ, 0 );
      ಆದರೆ ( error )
        ಹೋಗು Exit;

      /* add reversed points from `left' to `right' */
      error = ft_stroker_ಕೂಡಿಸು_ಹಿಮ್ಮುಖ_ಎಡ( stroker, TRUE );
      ಆದರೆ ( error )
        ಹೋಗು Exit;

      /* now add the final cap */
      stroker->center = stroker->subpath_ಆರಂಭ;
      error = ft_stroker_cap( stroker,
                              stroker->subpath_angle + FT_ANGLE_PI, 0 );
      ಆದರೆ ( error )
        ಹೋಗು Exit;

      /* Now end the right subpath accordingly.  The left one is */
      /* rewind and doesn't need further processing.             */
      ft_stroke_border_close( ಬಲ, FALSE );
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
      /* close the path if needed */
      ಆದರೆ ( !FT_IS_SMALL( stroker->center.x - stroker->subpath_ಆರಂಭ.x ) ||
           !FT_IS_SMALL( stroker->center.y - stroker->subpath_ಆರಂಭ.y ) )
      {
         error = FT_Stroker_ಸಾಲು_ಗಮ್ಯ( stroker, &stroker->subpath_ಆರಂಭ );
         ಆದರೆ ( error )
           ಹೋಗು Exit;
      }

      /* process the corner */
      stroker->angle_ಹೊರಹೋಗು = stroker->subpath_angle;

      error = ft_stroker_process_ಮೂಲೆ( stroker,
                                         stroker->subpath_ಸಾಲು_ಉದ್ದಳತೆ );
      ಆದರೆ ( error )
        ಹೋಗು Exit;

      /* then end our two subpaths */
      ft_stroke_border_close( stroker->borders + 0, FALSE );
      ft_stroke_border_close( stroker->borders + 1, TRUE );
    }

  Exit:
    ಹಿಂತಿರುಗಿ error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Stroker_GetBorderCounts( FT_Stroker        stroker,
                              FT_StrokerBorder  border,
                              FT_UInt          *anum_points,
                              FT_UInt          *anum_contours )
  {
    FT_UInt   ಅಂಕೆ_points = 0, ಅಂಕೆ_contours = 0;
    FT_Error  error;


    ಆದರೆ ( !stroker || border > 1 )
    {
      error = FT_THROW( Invalid_ವಾದ );
      ಹೋಗು Exit;
    }

    error = ft_stroke_border_get_counts( stroker->borders + border,
                                         &ಅಂಕೆ_points, &ಅಂಕೆ_contours );
  Exit:
    ಆದರೆ ( anum_points )
      *anum_points = ಅಂಕೆ_points;

    ಆದರೆ ( anum_contours )
      *anum_contours = ಅಂಕೆ_contours;

    ಹಿಂತಿರುಗಿ error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Stroker_GetCounts( FT_Stroker  stroker,
                        FT_UInt    *anum_points,
                        FT_UInt    *anum_contours )
  {
    FT_UInt   count1, count2, ಅಂಕೆ_points   = 0;
    FT_UInt   count3, count4, ಅಂಕೆ_contours = 0;
    FT_Error  error;


    ಆದರೆ ( !stroker )
    {
      error = FT_THROW( Invalid_ವಾದ );
      ಹೋಗು Exit;
    }

    error = ft_stroke_border_get_counts( stroker->borders + 0,
                                         &count1, &count2 );
    ಆದರೆ ( error )
      ಹೋಗು Exit;

    error = ft_stroke_border_get_counts( stroker->borders + 1,
                                         &count3, &count4 );
    ಆದರೆ ( error )
      ಹೋಗು Exit;

    ಅಂಕೆ_points   = count1 + count3;
    ಅಂಕೆ_contours = count2 + count4;

  Exit:
    ಆದರೆ ( anum_points )
      *anum_points   = ಅಂಕೆ_points;

    ಆದರೆ ( anum_contours )
      *anum_contours = ಅಂಕೆ_contours;

    ಹಿಂತಿರುಗಿ error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( ಶೂನ್ಯ )
  FT_Stroker_ExportBorder( FT_Stroker        stroker,
                           FT_StrokerBorder  border,
                           FT_Outline*       outline )
  {
    ಆದರೆ ( !stroker || !outline )
      ಹಿಂತಿರುಗಿ;

    ಆದರೆ ( border == FT_STROKER_BORDER_ಎಡ  ||
         border == FT_STROKER_BORDER_ಬಲ )
    {
      FT_StrokeBorder  sborder = & stroker->borders[border];


      ಆದರೆ ( sborder->valid )
        ft_stroke_border_export( sborder, outline );
    }
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( ಶೂನ್ಯ )
  FT_Stroker_Export( FT_Stroker   stroker,
                     FT_Outline*  outline )
  {
    FT_Stroker_ExportBorder( stroker, FT_STROKER_BORDER_ಎಡ, outline );
    FT_Stroker_ExportBorder( stroker, FT_STROKER_BORDER_ಬಲ, outline );
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
    FT_ಸದಿಶ   v_ಕೊನೆಯ;
    FT_ಸದಿಶ   v_control;
    FT_ಸದಿಶ   v_ಆರಂಭ;

    FT_ಸದಿಶ*  ಬಿಂದು;
    FT_ಸದಿಶ*  ಎಲ್ಲೆ;
    FT_Byte*    ಟ್ಯಾಗುಗಳು;

    FT_Error    error;

    FT_Int      n;         /* index of contour in outline     */
    FT_Int      ಮೊದಲ;     /* index of first point in contour */
    FT_Int      ಕೊನೆಯ;      /* index of last point in contour  */

    FT_Int      ಟ್ಯಾಗ್;       /* current point's state           */


    ಆದರೆ ( !outline )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_Outline );

    ಆದರೆ ( !stroker )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

    FT_Stroker_Rewind( stroker );

    ಕೊನೆಯ = -1;
    ಗಾಗಿ ( n = 0; n < outline->n_contours; n++ )
    {
      ಮೊದಲ = ಕೊನೆಯ + 1;
      ಕೊನೆಯ  = outline->contours[n];

      /* skip empty points; we don't stroke these */
      ಆದರೆ ( ಕೊನೆಯ <= ಮೊದಲ )
        ಮುಂದುವರಿ;

      ಎಲ್ಲೆ = outline->points + ಕೊನೆಯ;

      v_ಆರಂಭ = outline->points[ಮೊದಲ];
      v_ಕೊನೆಯ  = outline->points[ಕೊನೆಯ];

      v_control = v_ಆರಂಭ;

      ಬಿಂದು = outline->points + ಮೊದಲ;
      ಟ್ಯಾಗುಗಳು  = outline->ಟ್ಯಾಗುಗಳು   + ಮೊದಲ;
      ಟ್ಯಾಗ್   = FT_ವಕ್ರ_ಟ್ಯಾಗ್( ಟ್ಯಾಗುಗಳು[0] );

      /* check first point to determine origin */
      ಆದರೆ ( ಟ್ಯಾಗ್ != FT_ವಕ್ರ_ಟ್ಯಾಗ್_ON )
      {
        FT_Int  ಕೊನೆಯ_ಟ್ಯಾಗ್ = FT_ವಕ್ರ_ಟ್ಯಾಗ್( outline->ಟ್ಯಾಗುಗಳು[ಕೊನೆಯ] );


        /* First point is a control point.  Yes, this happens. */
        ಆದರೆ ( ಕೊನೆಯ_ಟ್ಯಾಗ್ == FT_ವಕ್ರ_ಟ್ಯಾಗ್_ON )
        {
          /* start at last point if it is on the curve */
          v_ಆರಂಭ = v_ಕೊನೆಯ;
          ಎಲ್ಲೆ--;
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
          ಆದರೆ ( ಕೊನೆಯ_ಟ್ಯಾಗ್ != ಟ್ಯಾಗ್ )
            ಹೋಗು Invalid_Outline;

          /* if both first and last points are controls, */
          /* start at their middle                       */
          v_ಆರಂಭ.x = ( v_ಆರಂಭ.x + v_ಕೊನೆಯ.x ) / 2;
          v_ಆರಂಭ.y = ( v_ಆರಂಭ.y + v_ಕೊನೆಯ.y ) / 2;
        }
        ಬಿಂದು--;
        ಟ್ಯಾಗುಗಳು--;
      }

      error = FT_Stroker_Begin_Sub_ಮಾರ್ಗ( stroker, &v_ಆರಂಭ, opened );
      ಆದರೆ ( error )
        ಹೋಗು Exit;

      ಆಗ ( ಬಿಂದು < ಎಲ್ಲೆ )
      {
        ಬಿಂದು++;
        ಟ್ಯಾಗುಗಳು++;

        ಟ್ಯಾಗ್ = FT_ವಕ್ರ_ಟ್ಯಾಗ್( ಟ್ಯಾಗುಗಳು[0] );
        ಆಯ್ಕೆ ( ಟ್ಯಾಗ್ )
        {
        ಸ್ಥಿತಿ FT_ವಕ್ರ_ಟ್ಯಾಗ್_ON:  /* emit a single line_to */
          {
            FT_ಸದಿಶ  vec;


            vec.x = ಬಿಂದು->x;
            vec.y = ಬಿಂದು->y;

            error = FT_Stroker_ಸಾಲು_ಗಮ್ಯ( stroker, &vec );
            ಆದರೆ ( error )
              ಹೋಗು Exit;
            ಮುಂದುವರಿ;
          }

        ಸ್ಥಿತಿ FT_ವಕ್ರ_ಟ್ಯಾಗ್_CONIC:  /* consume conic arcs */
          v_control.x = ಬಿಂದು->x;
          v_control.y = ಬಿಂದು->y;

        Do_Conic:
          ಆದರೆ ( ಬಿಂದು < ಎಲ್ಲೆ )
          {
            FT_ಸದಿಶ  vec;
            FT_ಸದಿಶ  v_ಮಧ್ಯ;


            ಬಿಂದು++;
            ಟ್ಯಾಗುಗಳು++;
            ಟ್ಯಾಗ್ = FT_ವಕ್ರ_ಟ್ಯಾಗ್( ಟ್ಯಾಗುಗಳು[0] );

            vec = ಬಿಂದು[0];

            ಆದರೆ ( ಟ್ಯಾಗ್ == FT_ವಕ್ರ_ಟ್ಯಾಗ್_ON )
            {
              error = FT_Stroker_Conic_ಗಮ್ಯ( stroker, &v_control, &vec );
              ಆದರೆ ( error )
                ಹೋಗು Exit;
              ಮುಂದುವರಿ;
            }

            ಆದರೆ ( ಟ್ಯಾಗ್ != FT_ವಕ್ರ_ಟ್ಯಾಗ್_CONIC )
              ಹೋಗು Invalid_Outline;

            v_ಮಧ್ಯ.x = ( v_control.x + vec.x ) / 2;
            v_ಮಧ್ಯ.y = ( v_control.y + vec.y ) / 2;

            error = FT_Stroker_Conic_ಗಮ್ಯ( stroker, &v_control, &v_ಮಧ್ಯ );
            ಆದರೆ ( error )
              ಹೋಗು Exit;

            v_control = vec;
            ಹೋಗು Do_Conic;
          }

          error = FT_Stroker_Conic_ಗಮ್ಯ( stroker, &v_control, &v_ಆರಂಭ );
          ಹೋಗು Close;

        ಪೂರ್ವನಿಯೋಜಿತ:  /* FT_CURVE_TAG_CUBIC */
          {
            FT_ಸದಿಶ  vec1, vec2;


            ಆದರೆ ( ಬಿಂದು + 1 > ಎಲ್ಲೆ                             ||
                 FT_ವಕ್ರ_ಟ್ಯಾಗ್( ಟ್ಯಾಗುಗಳು[1] ) != FT_ವಕ್ರ_ಟ್ಯಾಗ್_CUBIC )
              ಹೋಗು Invalid_Outline;

            ಬಿಂದು += 2;
            ಟ್ಯಾಗುಗಳು  += 2;

            vec1 = ಬಿಂದು[-2];
            vec2 = ಬಿಂದು[-1];

            ಆದರೆ ( ಬಿಂದು <= ಎಲ್ಲೆ )
            {
              FT_ಸದಿಶ  vec;
              FT_ಸದಿಶ  v_ಮಧ್ಯ;


              ಟ್ಯಾಗ್ = FT_ವಕ್ರ_ಟ್ಯಾಗ್( ಟ್ಯಾಗುಗಳು[0] );
              vec = ಬಿಂದು[0];

              ಆದರೆ ( ಟ್ಯಾಗ್ == FT_ವಕ್ರ_ಟ್ಯಾಗ್_CUBIC )
              {
                v_ಮಧ್ಯ.x = ( vec2.x + vec.x ) / 2;
                v_ಮಧ್ಯ.y = ( vec2.y + vec.y ) / 2;

                vec = v_ಮಧ್ಯ;
              }
              ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ( ಟ್ಯಾಗ್ != FT_ವಕ್ರ_ಟ್ಯಾಗ್_ON )
                ಹೋಗು Invalid_Outline;

              error = FT_Stroker_Cubic_ಗಮ್ಯ( stroker, &vec1, &vec2, &vec );
              ಆದರೆ ( error )
                ಹೋಗು Exit;

              ಆದರೆ ( ಟ್ಯಾಗ್ == FT_ವಕ್ರ_ಟ್ಯಾಗ್_CUBIC )
              {
                ಬಿಂದು--;
                ಟ್ಯಾಗುಗಳು--;
              }
              ಮುಂದುವರಿ;
            }

            error = FT_Stroker_Cubic_ಗಮ್ಯ( stroker, &vec1, &vec2, &v_ಆರಂಭ );
            ಹೋಗು Close;
          }
        }
      }

    Close:
      ಆದರೆ ( error )
        ಹೋಗು Exit;

      error = FT_Stroker_ಕೊನೆ_Sub_ಮಾರ್ಗ( stroker );
      ಆದರೆ ( error )
        ಹೋಗು Exit;
    }

    ಹಿಂತಿರುಗಿ FT_Err_Ok;

  Exit:
    ಹಿಂತಿರುಗಿ error;

  Invalid_Outline:
    ಹಿಂತಿರುಗಿ FT_THROW( Invalid_Outline );
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Glyph_Stroke( FT_Glyph    *pglyph,
                   FT_Stroker   stroker,
                   FT_Bool      destroy )
  {
    FT_Error  error = FT_THROW( Invalid_ವಾದ );
    FT_Glyph  glyph = ಖಾಲಿ;


    ಆದರೆ ( !pglyph )
      ಹೋಗು Exit;

    glyph = *pglyph;
    ಆದರೆ ( !glyph || glyph->clazz != &ft_outline_glyph_class )
      ಹೋಗು Exit;

    {
      FT_Glyph  copy;


      error = FT_Glyph_Copy( glyph, &copy );
      ಆದರೆ ( error )
        ಹೋಗು Exit;

      glyph = copy;
    }

    {
      FT_OutlineGlyph  oglyph  = (FT_OutlineGlyph)glyph;
      FT_Outline*      outline = &oglyph->outline;
      FT_UInt          ಅಂಕೆ_points, ಅಂಕೆ_contours;


      error = FT_Stroker_ParseOutline( stroker, outline, FALSE );
      ಆದರೆ ( error )
        ಹೋಗು Fail;

      error = FT_Stroker_GetCounts( stroker, &ಅಂಕೆ_points, &ಅಂಕೆ_contours );
      ಆದರೆ ( error )
        ಹೋಗು Fail;

      FT_Outline_ಮುಗಿದ( glyph->library, outline );

      error = FT_Outline_New( glyph->library,
                              ಅಂಕೆ_points,
                              (FT_Int)ಅಂಕೆ_contours,
                              outline );
      ಆದರೆ ( error )
        ಹೋಗು Fail;

      outline->n_points   = 0;
      outline->n_contours = 0;

      FT_Stroker_Export( stroker, outline );
    }

    ಆದರೆ ( destroy )
      FT_ಮುಗಿದ_Glyph( *pglyph );

    *pglyph = glyph;
    ಹೋಗು Exit;

  Fail:
    FT_ಮುಗಿದ_Glyph( glyph );
    glyph = ಖಾಲಿ;

    ಆದರೆ ( !destroy )
      *pglyph = ಖಾಲಿ;

  Exit:
    ಹಿಂತಿರುಗಿ error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Glyph_StrokeBorder( FT_Glyph    *pglyph,
                         FT_Stroker   stroker,
                         FT_Bool      inside,
                         FT_Bool      destroy )
  {
    FT_Error  error = FT_THROW( Invalid_ವಾದ );
    FT_Glyph  glyph = ಖಾಲಿ;


    ಆದರೆ ( !pglyph )
      ಹೋಗು Exit;

    glyph = *pglyph;
    ಆದರೆ ( !glyph || glyph->clazz != &ft_outline_glyph_class )
      ಹೋಗು Exit;

    {
      FT_Glyph  copy;


      error = FT_Glyph_Copy( glyph, &copy );
      ಆದರೆ ( error )
        ಹೋಗು Exit;

      glyph = copy;
    }

    {
      FT_OutlineGlyph   oglyph  = (FT_OutlineGlyph)glyph;
      FT_StrokerBorder  border;
      FT_Outline*       outline = &oglyph->outline;
      FT_UInt           ಅಂಕೆ_points, ಅಂಕೆ_contours;


      border = FT_Outline_GetOutsideBorder( outline );
      ಆದರೆ ( inside )
      {
        ಆದರೆ ( border == FT_STROKER_BORDER_ಎಡ )
          border = FT_STROKER_BORDER_ಬಲ;
        ಇಲ್ಲದಿದ್ದರೆ
          border = FT_STROKER_BORDER_ಎಡ;
      }

      error = FT_Stroker_ParseOutline( stroker, outline, FALSE );
      ಆದರೆ ( error )
        ಹೋಗು Fail;

      FT_Stroker_GetBorderCounts( stroker, border,
                                  &ಅಂಕೆ_points, &ಅಂಕೆ_contours );

      FT_Outline_ಮುಗಿದ( glyph->library, outline );

      error = FT_Outline_New( glyph->library,
                              ಅಂಕೆ_points,
                              (FT_Int)ಅಂಕೆ_contours,
                              outline );
      ಆದರೆ ( error )
        ಹೋಗು Fail;

      outline->n_points   = 0;
      outline->n_contours = 0;

      FT_Stroker_ExportBorder( stroker, border, outline );
    }

    ಆದರೆ ( destroy )
      FT_ಮುಗಿದ_Glyph( *pglyph );

    *pglyph = glyph;
    ಹೋಗು Exit;

  Fail:
    FT_ಮುಗಿದ_Glyph( glyph );
    glyph = ಖಾಲಿ;

    ಆದರೆ ( !destroy )
      *pglyph = ಖಾಲಿ;

  Exit:
    ಹಿಂತಿರುಗಿ error;
  }


/*ಕೊನೆ*/
