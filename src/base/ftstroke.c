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


#ضمن <freetype/ftstroke.h>
#ضمن <freetype/fttrigon.h>
#ضمن <freetype/ftoutln.h>
#ضمن <freetype/internal/ftmemory.h>
#ضمن <freetype/internal/ftdebug.h>
#ضمن <freetype/internal/ftobjs.h>


  /* declare an extern to access `ft_outline_glyph_class' globally */
  /* allocated  in `ftglyph.c'                                     */
  FT_CALLBACK_جدول ثابت FT_Glyph_Class  ft_outline_glyph_class;


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_StrokerBorder )
  FT_Outline_GetInsideBorder( FT_Outline*  outline )
  {
    FT_Orientation  o = FT_Outline_Get_Orientation( outline );


    أرجع o == FT_ORIENTATION_TRUETYPE ? FT_STROKER_BORDER_دایاں
                                        : FT_STROKER_BORDER_بایاں;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_StrokerBorder )
  FT_Outline_GetOutsideBorder( FT_Outline*  outline )
  {
    FT_Orientation  o = FT_Outline_Get_Orientation( outline );


    أرجع o == FT_ORIENTATION_TRUETYPE ? FT_STROKER_BORDER_بایاں
                                        : FT_STROKER_BORDER_دایاں;
  }


  /*************************************************************************/
  /*************************************************************************/
  /*****                                                               *****/
  /*****                      BEZIER COMPUTATIONS                      *****/
  /*****                                                               *****/
  /*************************************************************************/
  /*************************************************************************/

#تعریف_کرو FT_SMALL_CONIC_حد  ( FT_ANGLE_PI / 6 )
#تعریف_کرو FT_SMALL_CUBIC_حد  ( FT_ANGLE_PI / 8 )

#تعریف_کرو FT_EPSILON  2

#تعریف_کرو FT_IS_SMALL( x )  ( (x) > -FT_EPSILON && (x) < FT_EPSILON )


  ساكن FT_Pos
  ft_pos_abs( FT_Pos  x )
  {
    أرجع x >= 0 ? x : -x;
  }


  ساكن فراغ
  ft_conic_ونڈو( FT_سمتیہ*  ادھار )
  {
    FT_Pos  a, b;


    ادھار[4].x = ادھار[2].x;
    a = ادھار[0].x + ادھار[1].x;
    b = ادھار[1].x + ادھار[2].x;
    ادھار[3].x = b >> 1;
    ادھار[2].x = ( a + b ) >> 2;
    ادھار[1].x = a >> 1;

    ادھار[4].y = ادھار[2].y;
    a = ادھار[0].y + ادھار[1].y;
    b = ادھار[1].y + ادھار[2].y;
    ادھار[3].y = b >> 1;
    ادھار[2].y = ( a + b ) >> 2;
    ادھار[1].y = a >> 1;
  }


  ساكن FT_Bool
  ft_conic_is_small_enough( FT_سمتیہ*  ادھار,
                            FT_Angle   *angle_وچ,
                            FT_Angle   *angle_باہرلا )
  {
    FT_سمتیہ  d1, d2;
    FT_Angle   theta;
    FT_Int     close1, close2;


    d1.x = ادھار[1].x - ادھار[2].x;
    d1.y = ادھار[1].y - ادھار[2].y;
    d2.x = ادھار[0].x - ادھار[1].x;
    d2.y = ادھار[0].y - ادھار[1].y;

    close1 = FT_IS_SMALL( d1.x ) && FT_IS_SMALL( d1.y );
    close2 = FT_IS_SMALL( d2.x ) && FT_IS_SMALL( d2.y );

    إذا ( close1 )
    {
      إذا ( close2 )
      {
        /* basically a point;                      */
        /* do nothing to retain original direction */
      }
      وإلا
      {
        *angle_وچ  =
        *angle_باہرلا = FT_Atan2( d2.x, d2.y );
      }
    }
    وإلا /* !close1 */
    {
      إذا ( close2 )
      {
        *angle_وچ  =
        *angle_باہرلا = FT_Atan2( d1.x, d1.y );
      }
      وإلا
      {
        *angle_وچ  = FT_Atan2( d1.x, d1.y );
        *angle_باہرلا = FT_Atan2( d2.x, d2.y );
      }
    }

    theta = ft_pos_abs( FT_Angle_Diff( *angle_وچ, *angle_باہرلا ) );

    أرجع FT_BOOL( theta < FT_SMALL_CONIC_حد );
  }


  ساكن فراغ
  ft_cubic_ونڈو( FT_سمتیہ*  ادھار )
  {
    FT_Pos  a, b, c;


    ادھار[6].x = ادھار[3].x;
    a = ادھار[0].x + ادھار[1].x;
    b = ادھار[1].x + ادھار[2].x;
    c = ادھار[2].x + ادھار[3].x;
    ادھار[5].x = c >> 1;
    c += b;
    ادھار[4].x = c >> 2;
    ادھار[1].x = a >> 1;
    a += b;
    ادھار[2].x = a >> 2;
    ادھار[3].x = ( a + c ) >> 3;

    ادھار[6].y = ادھار[3].y;
    a = ادھار[0].y + ادھار[1].y;
    b = ادھار[1].y + ادھار[2].y;
    c = ادھار[2].y + ادھار[3].y;
    ادھار[5].y = c >> 1;
    c += b;
    ادھار[4].y = c >> 2;
    ادھار[1].y = a >> 1;
    a += b;
    ادھار[2].y = a >> 2;
    ادھار[3].y = ( a + c ) >> 3;
  }


  /* Return the average of `angle1' and `angle2'.            */
  /* This gives correct result even if `angle1' and `angle2' */
  /* have opposite signs.                                    */
  ساكن FT_Angle
  ft_angle_اوسط( FT_Angle  angle1,
                 FT_Angle  angle2 )
  {
    أرجع angle1 + FT_Angle_Diff( angle1, angle2 ) / 2;
  }


  ساكن FT_Bool
  ft_cubic_is_small_enough( FT_سمتیہ*  ادھار,
                            FT_Angle   *angle_وچ,
                            FT_Angle   *angle_mid,
                            FT_Angle   *angle_باہرلا )
  {
    FT_سمتیہ  d1, d2, d3;
    FT_Angle   theta1, theta2;
    FT_Int     close1, close2, close3;


    d1.x = ادھار[2].x - ادھار[3].x;
    d1.y = ادھار[2].y - ادھار[3].y;
    d2.x = ادھار[1].x - ادھار[2].x;
    d2.y = ادھار[1].y - ادھار[2].y;
    d3.x = ادھار[0].x - ادھار[1].x;
    d3.y = ادھار[0].y - ادھار[1].y;

    close1 = FT_IS_SMALL( d1.x ) && FT_IS_SMALL( d1.y );
    close2 = FT_IS_SMALL( d2.x ) && FT_IS_SMALL( d2.y );
    close3 = FT_IS_SMALL( d3.x ) && FT_IS_SMALL( d3.y );

    إذا ( close1 )
    {
      إذا ( close2 )
      {
        إذا ( close3 )
        {
          /* basically a point;                      */
          /* do nothing to retain original direction */
        }
        وإلا /* !close3 */
        {
          *angle_وچ  =
          *angle_mid =
          *angle_باہرلا = FT_Atan2( d3.x, d3.y );
        }
      }
      وإلا /* !close2 */
      {
        إذا ( close3 )
        {
          *angle_وچ  =
          *angle_mid =
          *angle_باہرلا = FT_Atan2( d2.x, d2.y );
        }
        وإلا /* !close3 */
        {
          *angle_وچ  =
          *angle_mid = FT_Atan2( d2.x, d2.y );
          *angle_باہرلا = FT_Atan2( d3.x, d3.y );
        }
      }
    }
    وإلا /* !close1 */
    {
      إذا ( close2 )
      {
        إذا ( close3 )
        {
          *angle_وچ  =
          *angle_mid =
          *angle_باہرلا = FT_Atan2( d1.x, d1.y );
        }
        وإلا /* !close3 */
        {
          *angle_وچ  = FT_Atan2( d1.x, d1.y );
          *angle_باہرلا = FT_Atan2( d3.x, d3.y );
          *angle_mid = ft_angle_اوسط( *angle_وچ, *angle_باہرلا );
        }
      }
      وإلا /* !close2 */
      {
        إذا ( close3 )
        {
          *angle_وچ  = FT_Atan2( d1.x, d1.y );
          *angle_mid =
          *angle_باہرلا = FT_Atan2( d2.x, d2.y );
        }
        وإلا /* !close3 */
        {
          *angle_وچ  = FT_Atan2( d1.x, d1.y );
          *angle_mid = FT_Atan2( d2.x, d2.y );
          *angle_باہرلا = FT_Atan2( d3.x, d3.y );
        }
      }
    }

    theta1 = ft_pos_abs( FT_Angle_Diff( *angle_وچ,  *angle_mid ) );
    theta2 = ft_pos_abs( FT_Angle_Diff( *angle_mid, *angle_باہرلا ) );

    أرجع FT_BOOL( theta1 < FT_SMALL_CUBIC_حد &&
                    theta2 < FT_SMALL_CUBIC_حد );
  }


  /*************************************************************************/
  /*************************************************************************/
  /*****                                                               *****/
  /*****                       STROKE BORDERS                          *****/
  /*****                                                               *****/
  /*************************************************************************/
  /*************************************************************************/

  تعريفنوع تعداد  FT_Stroke_ٹیگ_سوچی
  {
    FT_STROKE_ٹیگ_ON    = 1,   /* on-curve point  */
    FT_STROKE_ٹیگ_CUBIC = 2,   /* cubic off-point */
    FT_STROKE_ٹیگ_BEGIN = 4,   /* sub-path start  */
    FT_STROKE_ٹیگ_سرا   = 8    /* sub-path end    */

  } FT_Stroke_ٹیگ_سوچی_2;

#تعریف_کرو  FT_STROKE_ٹیگ_BEGIN_سرا  ( FT_STROKE_ٹیگ_BEGIN | FT_STROKE_ٹیگ_سرا )

  تعريفنوع بنية  FT_StrokeBorderRec_
  {
    FT_UInt     عدد_points;
    FT_UInt     max_points;
    FT_سمتیہ*  points;
    FT_Byte*    ٹیگ_سوچی;
    FT_Bool     movable;  /* TRUE for ends of lineto borders */
    FT_Int      شروع;    /* index of current sub-path start point */
    FT_حافظہ   حافظہ;
    FT_Bool     valid;

  } FT_StrokeBorderRec, *FT_StrokeBorder;


  ساكن FT_Error
  ft_stroke_border_grow( FT_StrokeBorder  border,
                         FT_UInt          new_points )
  {
    FT_UInt   old_max = border->max_points;
    FT_UInt   new_max = border->عدد_points + new_points;
    FT_Error  error   = FT_Err_Ok;


    إذا ( new_max > old_max )
    {
      FT_UInt    cur_max = old_max;
      FT_حافظہ  حافظہ  = border->حافظہ;


      طالما ( cur_max < new_max )
        cur_max += ( cur_max >> 1 ) + 16;

      إذا ( FT_RENEW_ARRAY( border->points, old_max, cur_max ) ||
           FT_RENEW_ARRAY( border->ٹیگ_سوچی,   old_max, cur_max ) )
        اذهب Exit;

      border->max_points = cur_max;
    }

  Exit:
    أرجع error;
  }


  ساكن فراغ
  ft_stroke_border_close( FT_StrokeBorder  border,
                          FT_Bool          الٹ )
  {
    FT_UInt  شروع = (FT_UInt)border->شروع;
    FT_UInt  گنتی = border->عدد_points;


    FT_ASSERT( border->شروع >= 0 );

    /* don't record empty paths! */
    إذا ( گنتی <= شروع + 1U )
      border->عدد_points = شروع;
    وإلا
    {
      /* copy the last point to the start of this sub-path, since */
      /* it contains the `adjusted' starting coordinates          */
      border->عدد_points    = --گنتی;
      border->points[شروع] = border->points[گنتی];
      border->ٹیگ_سوچی[شروع]   = border->ٹیگ_سوچی[گنتی];

      إذا ( الٹ )
      {
        /* reverse the points */
        {
          FT_سمتیہ*  vec1 = border->points + شروع + 1;
          FT_سمتیہ*  vec2 = border->points + گنتی - 1;


          لكل ( ; vec1 < vec2; vec1++, vec2-- )
          {
            FT_سمتیہ  tmp;


            tmp   = *vec1;
            *vec1 = *vec2;
            *vec2 = tmp;
          }
        }

        /* then the tags */
        {
          FT_Byte*  tag1 = border->ٹیگ_سوچی + شروع + 1;
          FT_Byte*  tag2 = border->ٹیگ_سوچی + گنتی - 1;


          لكل ( ; tag1 < tag2; tag1++, tag2-- )
          {
            FT_Byte  tmp;


            tmp   = *tag1;
            *tag1 = *tag2;
            *tag2 = tmp;
          }
        }
      }

      border->ٹیگ_سوچی[شروع    ] |= FT_STROKE_ٹیگ_BEGIN;
      border->ٹیگ_سوچی[گنتی - 1] |= FT_STROKE_ٹیگ_سرا;
    }

    border->شروع   = -1;
    border->movable = FALSE;
  }


  ساكن FT_Error
  ft_stroke_border_lineto( FT_StrokeBorder  border,
                           FT_سمتیہ*       منزل,
                           FT_Bool          movable )
  {
    FT_Error  error = FT_Err_Ok;


    FT_ASSERT( border->شروع >= 0 );

    إذا ( border->movable )
    {
      /* move last point */
      border->points[border->عدد_points - 1] = *منزل;
    }
    وإلا
    {
      /* don't add zero-length lineto, but always add moveto */
      إذا ( border->عدد_points > (FT_UInt)border->شروع                     &&
           FT_IS_SMALL( border->points[border->عدد_points - 1].x - منزل->x ) &&
           FT_IS_SMALL( border->points[border->عدد_points - 1].y - منزل->y ) )
        أرجع error;

      /* add one point */
      error = ft_stroke_border_grow( border, 1 );
      إذا ( !error )
      {
        FT_سمتیہ*  vec = border->points + border->عدد_points;
        FT_Byte*    ٹیگ = border->ٹیگ_سوچی   + border->عدد_points;


        vec[0] = *منزل;
        ٹیگ[0] = FT_STROKE_ٹیگ_ON;

        border->عدد_points += 1;
      }
    }
    border->movable = movable;
    أرجع error;
  }


  ساكن FT_Error
  ft_stroke_border_conicto( FT_StrokeBorder  border,
                            FT_سمتیہ*       control,
                            FT_سمتیہ*       منزل )
  {
    FT_Error  error;


    FT_ASSERT( border->شروع >= 0 );

    error = ft_stroke_border_grow( border, 2 );
    إذا ( !error )
    {
      FT_سمتیہ*  vec = border->points + border->عدد_points;
      FT_Byte*    ٹیگ = border->ٹیگ_سوچی   + border->عدد_points;


      vec[0] = *control;
      vec[1] = *منزل;

      ٹیگ[0] = 0;
      ٹیگ[1] = FT_STROKE_ٹیگ_ON;

      border->عدد_points += 2;
    }

    border->movable = FALSE;

    أرجع error;
  }


  ساكن FT_Error
  ft_stroke_border_cubicto( FT_StrokeBorder  border,
                            FT_سمتیہ*       control1,
                            FT_سمتیہ*       control2,
                            FT_سمتیہ*       منزل )
  {
    FT_Error  error;


    FT_ASSERT( border->شروع >= 0 );

    error = ft_stroke_border_grow( border, 3 );
    إذا ( !error )
    {
      FT_سمتیہ*  vec = border->points + border->عدد_points;
      FT_Byte*    ٹیگ = border->ٹیگ_سوچی   + border->عدد_points;


      vec[0] = *control1;
      vec[1] = *control2;
      vec[2] = *منزل;

      ٹیگ[0] = FT_STROKE_ٹیگ_CUBIC;
      ٹیگ[1] = FT_STROKE_ٹیگ_CUBIC;
      ٹیگ[2] = FT_STROKE_ٹیگ_ON;

      border->عدد_points += 3;
    }

    border->movable = FALSE;

    أرجع error;
  }


#تعریف_کرو FT_ARC_CUBIC_ANGLE  ( FT_ANGLE_PI / 2 )


  ساكن FT_Error
  ft_stroke_border_arcto( FT_StrokeBorder  border,
                          FT_سمتیہ*       center,
                          FT_Fixed         radius,
                          FT_Angle         angle_شروع,
                          FT_Angle         angle_diff )
  {
    FT_Fixed   coef;
    FT_سمتیہ  a0, a1, a2, a3;
    FT_Int     i, arcs = 1;
    FT_Error   error = FT_Err_Ok;


    /* number of cubic arcs to draw */
    طالما (  angle_diff > FT_ARC_CUBIC_ANGLE * arcs ||
            -angle_diff > FT_ARC_CUBIC_ANGLE * arcs )
      arcs++;

    /* control tangents */
    coef  = FT_Tan( angle_diff / ( 4 * arcs ) );
    coef += coef / 3;

    /* compute start and first control point */
    FT_سمتیہ_From_Polar( &a0, radius, angle_شروع );
    a1.x = FT_MulFix( -a0.y, coef );
    a1.y = FT_MulFix(  a0.x, coef );

    a0.x += center->x;
    a0.y += center->y;
    a1.x += a0.x;
    a1.y += a0.y;

    لكل ( i = 1; i <= arcs; i++ )
    {
      /* compute end and second control point */
      FT_سمتیہ_From_Polar( &a3, radius,
                            angle_شروع + i * angle_diff / arcs );
      a2.x = FT_MulFix(  a3.y, coef );
      a2.y = FT_MulFix( -a3.x, coef );

      a3.x += center->x;
      a3.y += center->y;
      a2.x += a3.x;
      a2.y += a3.y;

      /* add cubic arc */
      error = ft_stroke_border_cubicto( border, &a1, &a2, &a3 );
      إذا ( error )
        اكسر;

      /* a0 = a3; */
      a1.x = a3.x - a2.x + a3.x;
      a1.y = a3.y - a2.y + a3.y;
    }

    أرجع error;
  }


  ساكن FT_Error
  ft_stroke_border_moveto( FT_StrokeBorder  border,
                           FT_سمتیہ*       منزل )
  {
    /* close current open path if any ? */
    إذا ( border->شروع >= 0 )
      ft_stroke_border_close( border, FALSE );

    border->شروع = (FT_Int)border->عدد_points;
    border->movable = FALSE;

    أرجع ft_stroke_border_lineto( border, منزل, FALSE );
  }


  ساكن فراغ
  ft_stroke_border_init( FT_StrokeBorder  border,
                         FT_حافظہ        حافظہ )
  {
    border->حافظہ = حافظہ;
    border->points = عدم;
    border->ٹیگ_سوچی   = عدم;

    border->عدد_points = 0;
    border->max_points = 0;
    border->شروع      = -1;
    border->valid      = FALSE;
  }


  ساكن فراغ
  ft_stroke_border_reset( FT_StrokeBorder  border )
  {
    border->عدد_points = 0;
    border->شروع      = -1;
    border->valid      = FALSE;
  }


  ساكن فراغ
  ft_stroke_border_پورا( FT_StrokeBorder  border )
  {
    FT_حافظہ  حافظہ = border->حافظہ;


    FT_FREE( border->points );
    FT_FREE( border->ٹیگ_سوچی );

    border->عدد_points = 0;
    border->max_points = 0;
    border->شروع      = -1;
    border->valid      = FALSE;
  }


  ساكن FT_Error
  ft_stroke_border_get_counts( FT_StrokeBorder  border,
                               FT_UInt         *anum_points,
                               FT_UInt         *anum_contours )
  {
    FT_Error  error        = FT_Err_Ok;
    FT_UInt   عدد_points   = 0;
    FT_UInt   عدد_contours = 0;

    FT_UInt     گنتی      = border->عدد_points;
    FT_Byte*    ٹیگ_سوچی       = border->ٹیگ_سوچی;
    FT_Int      وچ_contour = 0;


    لكل ( ; گنتی > 0; گنتی--, عدد_points++, ٹیگ_سوچی++ )
    {
      إذا ( ٹیگ_سوچی[0] & FT_STROKE_ٹیگ_BEGIN )
      {
        إذا ( وچ_contour != 0 )
          اذهب Fail;

        وچ_contour = 1;
      }
      وإلا إذا ( وچ_contour == 0 )
        اذهب Fail;

      إذا ( ٹیگ_سوچی[0] & FT_STROKE_ٹیگ_سرا )
      {
        وچ_contour = 0;
        عدد_contours++;
      }
    }

    إذا ( وچ_contour != 0 )
      اذهب Fail;

    border->valid = TRUE;

  Exit:
    *anum_points   = عدد_points;
    *anum_contours = عدد_contours;
    أرجع error;

  Fail:
    عدد_points   = 0;
    عدد_contours = 0;
    error        = FT_THROW( Invalid_Outline );
    اذهب Exit;
  }


  ساكن فراغ
  ft_stroke_border_export( FT_StrokeBorder  border,
                           FT_Outline*      outline )
  {
    /* copy point locations */
    إذا ( border->عدد_points )
      FT_ARRAY_COPY( outline->points + outline->n_points,
                     border->points,
                     border->عدد_points );

    /* copy tags */
    {
      FT_UInt   گنتی = border->عدد_points;
      FT_Byte*  اقرأ  = border->ٹیگ_سوچی;
      FT_Byte*  اكتب = outline->ٹیگ_سوچی + outline->n_points;


      لكل ( ; گنتی > 0; گنتی--, اقرأ++, اكتب++ )
      {
        إذا ( *اقرأ & FT_STROKE_ٹیگ_ON )
          *اكتب = FT_ونگ_ٹیگ_ON;
        وإلا إذا ( *اقرأ & FT_STROKE_ٹیگ_CUBIC )
          *اكتب = FT_ونگ_ٹیگ_CUBIC;
        وإلا
          *اكتب = FT_ونگ_ٹیگ_CONIC;
      }
    }

    /* copy contours */
    {
      FT_UInt     گنتی = border->عدد_points;
      FT_Byte*    ٹیگ_سوچی  = border->ٹیگ_سوچی;
      FT_UShort*  اكتب = outline->contours + outline->n_contours;
      FT_UShort   idx   = outline->n_points;


      لكل ( ; گنتی > 0; گنتی--, ٹیگ_سوچی++, idx++ )
      {
        إذا ( *ٹیگ_سوچی & FT_STROKE_ٹیگ_سرا )
        {
          *اكتب++ = idx;
          outline->n_contours++;
        }
      }
    }

    outline->n_points += (FT_UShort)border->عدد_points;

    FT_ASSERT( FT_Outline_Check( outline ) == 0 );
  }


  /*************************************************************************/
  /*************************************************************************/
  /*****                                                               *****/
  /*****                           STROKER                             *****/
  /*****                                                               *****/
  /*************************************************************************/
  /*************************************************************************/

#تعریف_کرو FT_SIDE_منزل_ROTATE( s )   ( FT_ANGLE_PI2 - (s) * FT_ANGLE_PI )

  تعريفنوع بنية  FT_StrokerRec_
  {
    FT_Angle             angle_وچ;             /* direction into curr join */
    FT_Angle             angle_باہرلا;            /* direction out of join  */
    FT_سمتیہ            center;               /* current position */
    FT_Fixed             سطر_لمائی;          /* length of last lineto */
    FT_Bool              پہلا_نقطہ;          /* is this the start? */
    FT_Bool              subpath_open;         /* is the subpath open? */
    FT_Angle             subpath_angle;        /* subpath start direction */
    FT_سمتیہ            subpath_شروع;        /* subpath start position */
    FT_Fixed             subpath_سطر_لمائی;  /* subpath start lineto len */
    FT_Bool              handle_wide_strokes;  /* use wide strokes logic? */

    FT_Stroker_سطر_Cap   سطر_cap;
    FT_Stroker_سطر_Join  سطر_join;
    FT_Stroker_سطر_Join  سطر_join_saved;
    FT_Fixed             miter_حد_بندی;
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
    FT_حافظہ   حافظہ;
    FT_Stroker  stroker = عدم;


    إذا ( !library )
      أرجع FT_THROW( Invalid_Library_Handle );

    إذا ( !astroker )
      أرجع FT_THROW( Invalid_دلیل );

    حافظہ = library->حافظہ;

    إذا ( !FT_NEW( stroker ) )
    {
      stroker->library = library;

      ft_stroke_border_init( &stroker->borders[0], حافظہ );
      ft_stroke_border_init( &stroker->borders[1], حافظہ );
    }

    *astroker = stroker;

    أرجع error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( فراغ )
  FT_Stroker_سیٹ( FT_Stroker           stroker,
                  FT_Fixed             radius,
                  FT_Stroker_سطر_Cap   سطر_cap,
                  FT_Stroker_سطر_Join  سطر_join,
                  FT_Fixed             miter_حد_بندی )
  {
    إذا ( !stroker )
      أرجع;

    stroker->radius      = radius;
    stroker->سطر_cap    = سطر_cap;
    stroker->سطر_join   = سطر_join;
    stroker->miter_حد_بندی = miter_حد_بندی;

    /* ensure miter limit has sensible value */
    إذا ( stroker->miter_حد_بندی < 0x10000L )
      stroker->miter_حد_بندی = 0x10000L;

    /* save line join style:                                           */
    /* line join style can be temporarily changed when stroking curves */
    stroker->سطر_join_saved = سطر_join;

    FT_Stroker_Rewind( stroker );
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( فراغ )
  FT_Stroker_Rewind( FT_Stroker  stroker )
  {
    إذا ( stroker )
    {
      ft_stroke_border_reset( &stroker->borders[0] );
      ft_stroke_border_reset( &stroker->borders[1] );
    }
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( فراغ )
  FT_Stroker_پورا( FT_Stroker  stroker )
  {
    إذا ( stroker )
    {
      FT_حافظہ  حافظہ = stroker->library->حافظہ;


      ft_stroke_border_پورا( &stroker->borders[0] );
      ft_stroke_border_پورا( &stroker->borders[1] );

      stroker->library = عدم;
      FT_FREE( stroker );
    }
  }


  /* create a circular arc at a corner or cap */
  ساكن FT_Error
  ft_stroker_arcto( FT_Stroker  stroker,
                    FT_Int      side )
  {
    FT_Angle         کل, rotate;
    FT_Fixed         radius = stroker->radius;
    FT_Error         error  = FT_Err_Ok;
    FT_StrokeBorder  border = stroker->borders + side;


    rotate = FT_SIDE_منزل_ROTATE( side );

    کل = FT_Angle_Diff( stroker->angle_وچ, stroker->angle_باہرلا );
    إذا ( کل == FT_ANGLE_PI )
      کل = -rotate * 2;

    error = ft_stroke_border_arcto( border,
                                    &stroker->center,
                                    radius,
                                    stroker->angle_وچ + rotate,
                                    کل );
    border->movable = FALSE;
    أرجع error;
  }


  /* add a cap at the end of an opened path */
  ساكن FT_Error
  ft_stroker_cap( FT_Stroker  stroker,
                  FT_Angle    angle,
                  FT_Int      side )
  {
    FT_Error  error = FT_Err_Ok;


    إذا ( stroker->سطر_cap == FT_STROKER_LINECAP_گیڑا )
    {
      /* add a round cap */
      stroker->angle_وچ  = angle;
      stroker->angle_باہرلا = angle + FT_ANGLE_PI;

      error = ft_stroker_arcto( stroker, side );
    }
    وإلا
    {
      /* add a square or butt cap */
      FT_سمتیہ        درمیان, delta;
      FT_Fixed         radius = stroker->radius;
      FT_StrokeBorder  border = stroker->borders + side;


      /* compute middle point and first angle point */
      FT_سمتیہ_From_Polar( &درمیان, radius, angle );
      delta.x = side ?  درمیان.y : -درمیان.y;
      delta.y = side ? -درمیان.x :  درمیان.x;

      إذا ( stroker->سطر_cap == FT_STROKER_LINECAP_مربع )
      {
        درمیان.x += stroker->center.x;
        درمیان.y += stroker->center.y;
      }
      وإلا  /* FT_STROKER_LINECAP_BUTT */
      {
        درمیان.x  = stroker->center.x;
        درمیان.y  = stroker->center.y;
      }

      delta.x  += درمیان.x;
      delta.y  += درمیان.y;

      error = ft_stroke_border_lineto( border, &delta, FALSE );
      إذا ( error )
        اذهب Exit;

      /* compute second angle point */
      delta.x = درمیان.x - delta.x + درمیان.x;
      delta.y = درمیان.y - delta.y + درمیان.y;

      error = ft_stroke_border_lineto( border, &delta, FALSE );
    }

  Exit:
    أرجع error;
  }


  /* process an inside corner, i.e. compute intersection */
  ساكن FT_Error
  ft_stroker_inside( FT_Stroker  stroker,
                     FT_Int      side,
                     FT_Fixed    سطر_لمائی )
  {
    FT_StrokeBorder  border = stroker->borders + side;
    FT_Angle         phi, theta, rotate;
    FT_Fixed         لمائی;
    FT_سمتیہ        sigma = { 0, 0 };
    FT_سمتیہ        delta;
    FT_Error         error = FT_Err_Ok;
    FT_Bool          intersect;          /* use intersection of lines? */


    rotate = FT_SIDE_منزل_ROTATE( side );

    theta = FT_Angle_Diff( stroker->angle_وچ, stroker->angle_باہرلا ) / 2;

    /* Only intersect borders if between two lineto's and both */
    /* lines are long enough (line_length is zero for curves). */
    /* Also avoid U-turns of nearly 180 degree.                */
    إذا ( !border->movable || سطر_لمائی == 0  ||
         theta > 0x59C000 || theta < -0x59C000 )
      intersect = FALSE;
    وإلا
    {
      /* compute minimum required length of lines */
      FT_Fixed  min_لمائی;


      FT_سمتیہ_Unit( &sigma, theta );
      min_لمائی =
        ft_pos_abs( FT_MulDiv( stroker->radius, sigma.y, sigma.x ) );

      intersect = FT_BOOL( min_لمائی                         &&
                           stroker->سطر_لمائی >= min_لمائی &&
                           سطر_لمائی          >= min_لمائی );
    }

    إذا ( !intersect )
    {
      FT_سمتیہ_From_Polar( &delta, stroker->radius,
                            stroker->angle_باہرلا + rotate );
      delta.x += stroker->center.x;
      delta.y += stroker->center.y;

      border->movable = FALSE;
    }
    وإلا
    {
      /* compute median angle */
      phi = stroker->angle_وچ + theta + rotate;

      لمائی = FT_DivFix( stroker->radius, sigma.x );

      FT_سمتیہ_From_Polar( &delta, لمائی, phi );
      delta.x += stroker->center.x;
      delta.y += stroker->center.y;
    }

    error = ft_stroke_border_lineto( border, &delta, FALSE );

    أرجع error;
  }


  /* process an outside corner, i.e. compute bevel/miter/round */
  ساكن FT_Error
  ft_stroker_outside( FT_Stroker  stroker,
                      FT_Int      side,
                      FT_Fixed    سطر_لمائی )
  {
    FT_StrokeBorder  border = stroker->borders + side;
    FT_Error         error;
    FT_Angle         rotate;


    إذا ( stroker->سطر_join == FT_STROKER_LINEJOIN_گیڑا )
      error = ft_stroker_arcto( stroker, side );
    وإلا
    {
      /* this is a mitered (pointed) or beveled (truncated) corner */
      FT_Fixed   radius = stroker->radius;
      FT_سمتیہ  sigma = { 0, 0 };
      FT_Angle   theta = 0, phi = 0;
      FT_Bool    bevel, fixed_bevel;


      rotate = FT_SIDE_منزل_ROTATE( side );

      bevel =
        FT_BOOL( stroker->سطر_join == FT_STROKER_LINEJOIN_BEVEL );

      fixed_bevel =
        FT_BOOL( stroker->سطر_join != FT_STROKER_LINEJOIN_MITER_VARIABLE );

      /* check miter limit first */
      إذا ( !bevel )
      {
        theta = FT_Angle_Diff( stroker->angle_وچ, stroker->angle_باہرلا ) / 2;

        إذا ( theta == FT_ANGLE_PI2 )
          theta = -rotate;

        phi = stroker->angle_وچ + theta + rotate;

        FT_سمتیہ_From_Polar( &sigma, stroker->miter_حد_بندی, theta );

        /* is miter limit exceeded? */
        إذا ( sigma.x < 0x10000L )
        {
          /* don't create variable bevels for very small deviations; */
          /* FT_Sin(x) = 0 for x <= 57                               */
          إذا ( fixed_bevel || ft_pos_abs( theta ) > 57 )
            bevel = TRUE;
        }
      }

      إذا ( bevel )  /* this is a bevel (broken angle) */
      {
        إذا ( fixed_bevel )
        {
          /* the outer corners are simply joined together */
          FT_سمتیہ  delta;


          /* add bevel */
          FT_سمتیہ_From_Polar( &delta,
                                radius,
                                stroker->angle_باہرلا + rotate );
          delta.x += stroker->center.x;
          delta.y += stroker->center.y;

          border->movable = FALSE;
          error = ft_stroke_border_lineto( border, &delta, FALSE );
        }
        وإلا /* variable bevel or clipped miter */
        {
          /* the miter is truncated */
          FT_سمتیہ  درمیان, delta;
          FT_Fixed   coef;


          /* compute middle point and first angle point */
          FT_سمتیہ_From_Polar( &درمیان,
                                FT_MulFix( radius, stroker->miter_حد_بندی ),
                                phi );

          coef    = FT_DivFix(  0x10000L - sigma.x, sigma.y );
          delta.x = FT_MulFix(  درمیان.y, coef );
          delta.y = FT_MulFix( -درمیان.x, coef );

          درمیان.x += stroker->center.x;
          درمیان.y += stroker->center.y;
          delta.x  += درمیان.x;
          delta.y  += درمیان.y;

          error = ft_stroke_border_lineto( border, &delta, FALSE );
          إذا ( error )
            اذهب Exit;

          /* compute second angle point */
          delta.x = درمیان.x - delta.x + درمیان.x;
          delta.y = درمیان.y - delta.y + درمیان.y;

          error = ft_stroke_border_lineto( border, &delta, FALSE );
          إذا ( error )
            اذهب Exit;

          /* finally, add an end point; only needed if not lineto */
          /* (line_length is zero for curves)                     */
          إذا ( سطر_لمائی == 0 )
          {
            FT_سمتیہ_From_Polar( &delta,
                                  radius,
                                  stroker->angle_باہرلا + rotate );

            delta.x += stroker->center.x;
            delta.y += stroker->center.y;

            error = ft_stroke_border_lineto( border, &delta, FALSE );
          }
        }
      }
      وإلا /* this is a miter (intersection) */
      {
        FT_Fixed   لمائی;
        FT_سمتیہ  delta;


        لمائی = FT_MulDiv( stroker->radius, stroker->miter_حد_بندی, sigma.x );

        FT_سمتیہ_From_Polar( &delta, لمائی, phi );
        delta.x += stroker->center.x;
        delta.y += stroker->center.y;

        error = ft_stroke_border_lineto( border, &delta, FALSE );
        إذا ( error )
          اذهب Exit;

        /* now add an end point; only needed if not lineto */
        /* (line_length is zero for curves)                */
        إذا ( سطر_لمائی == 0 )
        {
          FT_سمتیہ_From_Polar( &delta,
                                stroker->radius,
                                stroker->angle_باہرلا + rotate );
          delta.x += stroker->center.x;
          delta.y += stroker->center.y;

          error = ft_stroke_border_lineto( border, &delta, FALSE );
        }
      }
    }

  Exit:
    أرجع error;
  }


  ساكن FT_Error
  ft_stroker_process_کونا( FT_Stroker  stroker,
                             FT_Fixed    سطر_لمائی )
  {
    FT_Error  error = FT_Err_Ok;
    FT_Angle  turn;
    FT_Int    inside_side;


    turn = FT_Angle_Diff( stroker->angle_وچ, stroker->angle_باہرلا );

    /* no specific corner processing is required if the turn is 0 */
    إذا ( turn == 0 )
      اذهب Exit;

    /* when we turn to the right, the inside side is 0 */
    /* otherwise, the inside side is 1 */
    inside_side = ( turn < 0 );

    /* process the inside side */
    error = ft_stroker_inside( stroker, inside_side, سطر_لمائی );
    إذا ( error )
      اذهب Exit;

    /* process the outside side */
    error = ft_stroker_outside( stroker, !inside_side, سطر_لمائی );

  Exit:
    أرجع error;
  }


  /* add two points to the left and right borders corresponding to the */
  /* start of the subpath                                              */
  ساكن FT_Error
  ft_stroker_subpath_شروع( FT_Stroker  stroker,
                            FT_Angle    شروع_angle,
                            FT_Fixed    سطر_لمائی )
  {
    FT_سمتیہ        delta;
    FT_سمتیہ        نقطہ;
    FT_Error         error;
    FT_StrokeBorder  border;


    FT_سمتیہ_From_Polar( &delta, stroker->radius,
                          شروع_angle + FT_ANGLE_PI2 );

    نقطہ.x = stroker->center.x + delta.x;
    نقطہ.y = stroker->center.y + delta.y;

    border = stroker->borders;
    error = ft_stroke_border_moveto( border, &نقطہ );
    إذا ( error )
      اذهب Exit;

    نقطہ.x = stroker->center.x - delta.x;
    نقطہ.y = stroker->center.y - delta.y;

    border++;
    error = ft_stroke_border_moveto( border, &نقطہ );

    /* save angle, position, and line length for last join */
    /* (line_length is zero for curves)                    */
    stroker->subpath_angle       = شروع_angle;
    stroker->پہلا_نقطہ         = FALSE;
    stroker->subpath_سطر_لمائی = سطر_لمائی;

  Exit:
    أرجع error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Stroker_سطر_منزل( FT_Stroker  stroker,
                     FT_سمتیہ*  منزل )
  {
    FT_Error         error = FT_Err_Ok;
    FT_StrokeBorder  border;
    FT_سمتیہ        delta;
    FT_Angle         angle;
    FT_Int           side;
    FT_Fixed         سطر_لمائی;


    إذا ( !stroker || !منزل )
      أرجع FT_THROW( Invalid_دلیل );

    delta.x = منزل->x - stroker->center.x;
    delta.y = منزل->y - stroker->center.y;

    /* a zero-length lineto is a no-op; avoid creating a spurious corner */
    إذا ( delta.x == 0 && delta.y == 0 )
       اذهب Exit;

    /* compute length of line */
    سطر_لمائی = FT_سمتیہ_لمائی( &delta );

    angle = FT_Atan2( delta.x, delta.y );
    FT_سمتیہ_From_Polar( &delta, stroker->radius, angle + FT_ANGLE_PI2 );

    /* process corner if necessary */
    إذا ( stroker->پہلا_نقطہ )
    {
      /* This is the first segment of a subpath.  We need to     */
      /* add a point to each border at their respective starting */
      /* point locations.                                        */
      error = ft_stroker_subpath_شروع( stroker, angle, سطر_لمائی );
      إذا ( error )
        اذهب Exit;
    }
    وإلا
    {
      /* process the current corner */
      stroker->angle_باہرلا = angle;
      error = ft_stroker_process_کونا( stroker, سطر_لمائی );
      إذا ( error )
        اذهب Exit;
    }

    /* now add a line segment to both the `inside' and `outside' paths */
    لكل ( border = stroker->borders, side = 1; side >= 0; side--, border++ )
    {
      FT_سمتیہ  نقطہ;


      نقطہ.x = منزل->x + delta.x;
      نقطہ.y = منزل->y + delta.y;

      /* the ends of lineto borders are movable */
      error = ft_stroke_border_lineto( border, &نقطہ, TRUE );
      إذا ( error )
        اذهب Exit;

      delta.x = -delta.x;
      delta.y = -delta.y;
    }

    stroker->angle_وچ    = angle;
    stroker->center      = *منزل;
    stroker->سطر_لمائی = سطر_لمائی;

  Exit:
    أرجع error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Stroker_Conic_منزل( FT_Stroker  stroker,
                      FT_سمتیہ*  control,
                      FT_سمتیہ*  منزل )
  {
    FT_Error    error = FT_Err_Ok;
    FT_سمتیہ   bez_ڈھیر[34];
    FT_سمتیہ*  arc;
    FT_سمتیہ*  حد_بندی = bez_ڈھیر + 30;
    FT_Bool     پہلا_arc = TRUE;


    إذا ( !stroker || !control || !منزل )
    {
      error = FT_THROW( Invalid_دلیل );
      اذهب Exit;
    }

    /* if all control points are coincident, this is a no-op; */
    /* avoid creating a spurious corner                       */
    إذا ( FT_IS_SMALL( stroker->center.x - control->x ) &&
         FT_IS_SMALL( stroker->center.y - control->y ) &&
         FT_IS_SMALL( control->x        - منزل->x      ) &&
         FT_IS_SMALL( control->y        - منزل->y      ) )
    {
       stroker->center = *منزل;
       اذهب Exit;
    }

    arc    = bez_ڈھیر;
    arc[0] = *منزل;
    arc[1] = *control;
    arc[2] = stroker->center;

    افعل
    {
      FT_Angle  angle_وچ, angle_باہرلا;


      /* initialize with current direction */
      angle_وچ = angle_باہرلا = stroker->angle_وچ;

      إذا ( arc < حد_بندی                                             &&
           !ft_conic_is_small_enough( arc, &angle_وچ, &angle_باہرلا ) )
      {
        إذا ( stroker->پہلا_نقطہ )
          stroker->angle_وچ = angle_وچ;

        ft_conic_ونڈو( arc );
        arc += 2;
        استمر;
      }

      إذا ( پہلا_arc )
      {
        پہلا_arc = FALSE;

        /* process corner if necessary */
        إذا ( stroker->پہلا_نقطہ )
          error = ft_stroker_subpath_شروع( stroker, angle_وچ, 0 );
        وإلا
        {
          stroker->angle_باہرلا = angle_وچ;
          error = ft_stroker_process_کونا( stroker, 0 );
        }
      }
      وإلا إذا ( ft_pos_abs( FT_Angle_Diff( stroker->angle_وچ, angle_وچ ) ) >
                  FT_SMALL_CONIC_حد / 4                             )
      {
        /* if the deviation from one arc to the next is too great, */
        /* add a round corner                                      */
        stroker->center    = arc[2];
        stroker->angle_باہرلا = angle_وچ;
        stroker->سطر_join = FT_STROKER_LINEJOIN_گیڑا;

        error = ft_stroker_process_کونا( stroker, 0 );

        /* reinstate line join style */
        stroker->سطر_join = stroker->سطر_join_saved;
      }

      إذا ( error )
        اذهب Exit;

      /* the arc's angle is small enough; we can add it directly to each */
      /* border                                                          */
      {
        FT_سمتیہ        ctrl, سرا;
        FT_Angle         theta, phi, rotate, alpha0 = 0;
        FT_Fixed         لمائی;
        FT_StrokeBorder  border;
        FT_Int           side;


        theta  = FT_Angle_Diff( angle_وچ, angle_باہرلا ) / 2;
        phi    = angle_وچ + theta;
        لمائی = FT_DivFix( stroker->radius, FT_Cos( theta ) );

        /* compute direction of original arc */
        إذا ( stroker->handle_wide_strokes )
          alpha0 = FT_Atan2( arc[0].x - arc[2].x, arc[0].y - arc[2].y );

        لكل ( border = stroker->borders, side = 0;
              side <= 1;
              side++, border++ )
        {
          rotate = FT_SIDE_منزل_ROTATE( side );

          /* compute control point */
          FT_سمتیہ_From_Polar( &ctrl, لمائی, phi + rotate );
          ctrl.x += arc[1].x;
          ctrl.y += arc[1].y;

          /* compute end point */
          FT_سمتیہ_From_Polar( &سرا, stroker->radius, angle_باہرلا + rotate );
          سرا.x += arc[0].x;
          سرا.y += arc[0].y;

          إذا ( stroker->handle_wide_strokes )
          {
            FT_سمتیہ  شروع;
            FT_Angle   alpha1;


            /* determine whether the border radius is greater than the */
            /* radius of curvature of the original arc                 */
            شروع = border->points[border->عدد_points - 1];

            alpha1 = FT_Atan2( سرا.x - شروع.x, سرا.y - شروع.y );

            /* is the direction of the border arc opposite to */
            /* that of the original arc? */
            إذا ( ft_pos_abs( FT_Angle_Diff( alpha0, alpha1 ) ) >
                   FT_ANGLE_PI / 2                             )
            {
              FT_Angle   beta, gamma;
              FT_سمتیہ  bvec, delta;
              FT_Fixed   blen, sinA, sinB, alen;


              /* use the sine rule to find the intersection point */
              beta  = FT_Atan2( arc[2].x - شروع.x, arc[2].y - شروع.y );
              gamma = FT_Atan2( arc[0].x - سرا.x,   arc[0].y - سرا.y );

              bvec.x = سرا.x - شروع.x;
              bvec.y = سرا.y - شروع.y;

              blen = FT_سمتیہ_لمائی( &bvec );

              sinA = ft_pos_abs( FT_Sin( alpha1 - gamma ) );
              sinB = ft_pos_abs( FT_Sin( beta - gamma ) );

              alen = FT_MulDiv( blen, sinA, sinB );

              FT_سمتیہ_From_Polar( &delta, alen, beta );
              delta.x += شروع.x;
              delta.y += شروع.y;

              /* circumnavigate the negative sector backwards */
              border->movable = FALSE;
              error = ft_stroke_border_lineto( border, &delta, FALSE );
              إذا ( error )
                اذهب Exit;
              error = ft_stroke_border_lineto( border, &سرا, FALSE );
              إذا ( error )
                اذهب Exit;
              error = ft_stroke_border_conicto( border, &ctrl, &شروع );
              إذا ( error )
                اذهب Exit;
              /* and then move to the endpoint */
              error = ft_stroke_border_lineto( border, &سرا, FALSE );
              إذا ( error )
                اذهب Exit;

              استمر;
            }

            /* else fall through */
          }

          /* simply add an arc */
          error = ft_stroke_border_conicto( border, &ctrl, &سرا );
          إذا ( error )
            اذهب Exit;
        }
      }

      stroker->angle_وچ = angle_باہرلا;

      إذا ( arc == bez_ڈھیر )
        اكسر;
      arc -= 2;
    } طالما ( 1 );

    stroker->center      = *منزل;
    stroker->سطر_لمائی = 0;

  Exit:
    أرجع error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Stroker_Cubic_منزل( FT_Stroker  stroker,
                      FT_سمتیہ*  control1,
                      FT_سمتیہ*  control2,
                      FT_سمتیہ*  منزل )
  {
    FT_Error    error = FT_Err_Ok;
    FT_سمتیہ   bez_ڈھیر[37];
    FT_سمتیہ*  arc;
    FT_سمتیہ*  حد_بندی = bez_ڈھیر + 32;
    FT_Bool     پہلا_arc = TRUE;


    إذا ( !stroker || !control1 || !control2 || !منزل )
    {
      error = FT_THROW( Invalid_دلیل );
      اذهب Exit;
    }

    /* if all control points are coincident, this is a no-op; */
    /* avoid creating a spurious corner */
    إذا ( FT_IS_SMALL( stroker->center.x - control1->x ) &&
         FT_IS_SMALL( stroker->center.y - control1->y ) &&
         FT_IS_SMALL( control1->x       - control2->x ) &&
         FT_IS_SMALL( control1->y       - control2->y ) &&
         FT_IS_SMALL( control2->x       - منزل->x       ) &&
         FT_IS_SMALL( control2->y       - منزل->y       ) )
    {
       stroker->center = *منزل;
       اذهب Exit;
    }

    arc    = bez_ڈھیر;
    arc[0] = *منزل;
    arc[1] = *control2;
    arc[2] = *control1;
    arc[3] = stroker->center;

    افعل
    {
      FT_Angle  angle_وچ, angle_mid, angle_باہرلا;


      /* initialize with current direction */
      angle_وچ = angle_باہرلا = angle_mid = stroker->angle_وچ;

      إذا ( arc < حد_بندی                                         &&
           !ft_cubic_is_small_enough( arc, &angle_وچ,
                                      &angle_mid, &angle_باہرلا ) )
      {
        إذا ( stroker->پہلا_نقطہ )
          stroker->angle_وچ = angle_وچ;

        ft_cubic_ونڈو( arc );
        arc += 3;
        استمر;
      }

      إذا ( پہلا_arc )
      {
        پہلا_arc = FALSE;

        /* process corner if necessary */
        إذا ( stroker->پہلا_نقطہ )
          error = ft_stroker_subpath_شروع( stroker, angle_وچ, 0 );
        وإلا
        {
          stroker->angle_باہرلا = angle_وچ;
          error = ft_stroker_process_کونا( stroker, 0 );
        }
      }
      وإلا إذا ( ft_pos_abs( FT_Angle_Diff( stroker->angle_وچ, angle_وچ ) ) >
                  FT_SMALL_CUBIC_حد / 4                             )
      {
        /* if the deviation from one arc to the next is too great, */
        /* add a round corner                                      */
        stroker->center    = arc[3];
        stroker->angle_باہرلا = angle_وچ;
        stroker->سطر_join = FT_STROKER_LINEJOIN_گیڑا;

        error = ft_stroker_process_کونا( stroker, 0 );

        /* reinstate line join style */
        stroker->سطر_join = stroker->سطر_join_saved;
      }

      إذا ( error )
        اذهب Exit;

      /* the arc's angle is small enough; we can add it directly to each */
      /* border                                                          */
      {
        FT_سمتیہ        ctrl1, ctrl2, سرا;
        FT_Angle         theta1, phi1, theta2, phi2, rotate, alpha0 = 0;
        FT_Fixed         length1, length2;
        FT_StrokeBorder  border;
        FT_Int           side;


        theta1  = FT_Angle_Diff( angle_وچ,  angle_mid ) / 2;
        theta2  = FT_Angle_Diff( angle_mid, angle_باہرلا ) / 2;
        phi1    = ft_angle_اوسط( angle_وچ,  angle_mid );
        phi2    = ft_angle_اوسط( angle_mid, angle_باہرلا );
        length1 = FT_DivFix( stroker->radius, FT_Cos( theta1 ) );
        length2 = FT_DivFix( stroker->radius, FT_Cos( theta2 ) );

        /* compute direction of original arc */
        إذا ( stroker->handle_wide_strokes )
          alpha0 = FT_Atan2( arc[0].x - arc[3].x, arc[0].y - arc[3].y );

        لكل ( border = stroker->borders, side = 0;
              side <= 1;
              side++, border++ )
        {
          rotate = FT_SIDE_منزل_ROTATE( side );

          /* compute control points */
          FT_سمتیہ_From_Polar( &ctrl1, length1, phi1 + rotate );
          ctrl1.x += arc[2].x;
          ctrl1.y += arc[2].y;

          FT_سمتیہ_From_Polar( &ctrl2, length2, phi2 + rotate );
          ctrl2.x += arc[1].x;
          ctrl2.y += arc[1].y;

          /* compute end point */
          FT_سمتیہ_From_Polar( &سرا, stroker->radius, angle_باہرلا + rotate );
          سرا.x += arc[0].x;
          سرا.y += arc[0].y;

          إذا ( stroker->handle_wide_strokes )
          {
            FT_سمتیہ  شروع;
            FT_Angle   alpha1;


            /* determine whether the border radius is greater than the */
            /* radius of curvature of the original arc                 */
            شروع = border->points[border->عدد_points - 1];

            alpha1 = FT_Atan2( سرا.x - شروع.x, سرا.y - شروع.y );

            /* is the direction of the border arc opposite to */
            /* that of the original arc? */
            إذا ( ft_pos_abs( FT_Angle_Diff( alpha0, alpha1 ) ) >
                   FT_ANGLE_PI / 2                             )
            {
              FT_Angle   beta, gamma;
              FT_سمتیہ  bvec, delta;
              FT_Fixed   blen, sinA, sinB, alen;


              /* use the sine rule to find the intersection point */
              beta  = FT_Atan2( arc[3].x - شروع.x, arc[3].y - شروع.y );
              gamma = FT_Atan2( arc[0].x - سرا.x,   arc[0].y - سرا.y );

              bvec.x = سرا.x - شروع.x;
              bvec.y = سرا.y - شروع.y;

              blen = FT_سمتیہ_لمائی( &bvec );

              sinA = ft_pos_abs( FT_Sin( alpha1 - gamma ) );
              sinB = ft_pos_abs( FT_Sin( beta - gamma ) );

              alen = FT_MulDiv( blen, sinA, sinB );

              FT_سمتیہ_From_Polar( &delta, alen, beta );
              delta.x += شروع.x;
              delta.y += شروع.y;

              /* circumnavigate the negative sector backwards */
              border->movable = FALSE;
              error = ft_stroke_border_lineto( border, &delta, FALSE );
              إذا ( error )
                اذهب Exit;
              error = ft_stroke_border_lineto( border, &سرا, FALSE );
              إذا ( error )
                اذهب Exit;
              error = ft_stroke_border_cubicto( border,
                                                &ctrl2,
                                                &ctrl1,
                                                &شروع );
              إذا ( error )
                اذهب Exit;
              /* and then move to the endpoint */
              error = ft_stroke_border_lineto( border, &سرا, FALSE );
              إذا ( error )
                اذهب Exit;

              استمر;
            }

            /* else fall through */
          }

          /* simply add an arc */
          error = ft_stroke_border_cubicto( border, &ctrl1, &ctrl2, &سرا );
          إذا ( error )
            اذهب Exit;
        }
      }

      stroker->angle_وچ = angle_باہرلا;

      إذا ( arc == bez_ڈھیر )
        اكسر;
      arc -= 3;
    } طالما ( 1 );

    stroker->center      = *منزل;
    stroker->سطر_لمائی = 0;

  Exit:
    أرجع error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Stroker_Begin_Sub_رستہ( FT_Stroker  stroker,
                           FT_سمتیہ*  منزل,
                           FT_Bool     open )
  {
    إذا ( !stroker || !منزل )
      أرجع FT_THROW( Invalid_دلیل );

    /* We cannot process the first point, because there is not enough      */
    /* information regarding its corner/cap.  The latter will be processed */
    /* in the `FT_Stroker_EndSubPath' routine.                             */
    /*                                                                     */
    stroker->پہلا_نقطہ  = TRUE;
    stroker->center       = *منزل;
    stroker->subpath_open = open;

    /* Determine if we need to check whether the border radius is greater */
    /* than the radius of curvature of a curve, to handle this case       */
    /* specially.  This is only required if bevel joins or butt caps may  */
    /* be created, because round & miter joins and round & square caps    */
    /* cover the negative sector created with wide strokes.               */
    stroker->handle_wide_strokes =
      FT_BOOL( stroker->سطر_join != FT_STROKER_LINEJOIN_گیڑا  ||
               ( stroker->subpath_open                        &&
                 stroker->سطر_cap == FT_STROKER_LINECAP_BUTT ) );

    /* record the subpath start point for each border */
    stroker->subpath_شروع = *منزل;

    stroker->angle_وچ = 0;

    أرجع FT_Err_Ok;
  }


  ساكن FT_Error
  ft_stroker_جوڑو_الٹ_بایاں( FT_Stroker  stroker,
                               FT_Bool     open )
  {
    FT_StrokeBorder  دایاں = stroker->borders + 0;
    FT_StrokeBorder  بایاں  = stroker->borders + 1;
    FT_Int           new_points;
    FT_Error         error = FT_Err_Ok;


    FT_ASSERT( بایاں->شروع >= 0 );

    new_points = (FT_Int)بایاں->عدد_points - بایاں->شروع;
    إذا ( new_points > 0 )
    {
      error = ft_stroke_border_grow( دایاں, (FT_UInt)new_points );
      إذا ( error )
        اذهب Exit;

      {
        FT_سمتیہ*  dst_نقطہ = دایاں->points + دایاں->عدد_points;
        FT_Byte*    dst_ٹیگ   = دایاں->ٹیگ_سوچی   + دایاں->عدد_points;
        FT_سمتیہ*  src_نقطہ = بایاں->points  + بایاں->عدد_points - 1;
        FT_Byte*    src_ٹیگ   = بایاں->ٹیگ_سوچی    + بایاں->عدد_points - 1;


        طالما ( src_نقطہ >= بایاں->points + بایاں->شروع )
        {
          *dst_نقطہ = *src_نقطہ;
          *dst_ٹیگ   = *src_ٹیگ;

          إذا ( open )
            dst_ٹیگ[0] &= ~FT_STROKE_ٹیگ_BEGIN_سرا;
          وإلا
          {
            FT_Byte  ttag =
                       (FT_Byte)( dst_ٹیگ[0] & FT_STROKE_ٹیگ_BEGIN_سرا );


            /* switch begin/end tags if necessary */
            إذا ( ttag == FT_STROKE_ٹیگ_BEGIN ||
                 ttag == FT_STROKE_ٹیگ_سرا   )
              dst_ٹیگ[0] ^= FT_STROKE_ٹیگ_BEGIN_سرا;
          }

          src_نقطہ--;
          src_ٹیگ--;
          dst_نقطہ++;
          dst_ٹیگ++;
        }
      }

      بایاں->عدد_points   = (FT_UInt)بایاں->شروع;
      دایاں->عدد_points += (FT_UInt)new_points;

      دایاں->movable = FALSE;
      بایاں->movable  = FALSE;
    }

  Exit:
    أرجع error;
  }


  /* documentation is in ftstroke.h */

  /* there's a lot of magic in this function! */
  FT_EXPORT_DEF( FT_Error )
  FT_Stroker_سرا_Sub_رستہ( FT_Stroker  stroker )
  {
    FT_Error  error = FT_Err_Ok;


    إذا ( !stroker )
    {
      error = FT_THROW( Invalid_دلیل );
      اذهب Exit;
    }

    /* don't try to end the path if no segments have been generated */
    إذا ( stroker->پہلا_نقطہ )
      اذهب Exit;

    إذا ( stroker->subpath_open )
    {
      FT_StrokeBorder  دایاں = stroker->borders;


      /* All right, this is an opened path, we need to add a cap between */
      /* right & left, add the reverse of left, then add a final cap     */
      /* between left & right.                                           */
      error = ft_stroker_cap( stroker, stroker->angle_وچ, 0 );
      إذا ( error )
        اذهب Exit;

      /* add reversed points from `left' to `right' */
      error = ft_stroker_جوڑو_الٹ_بایاں( stroker, TRUE );
      إذا ( error )
        اذهب Exit;

      /* now add the final cap */
      stroker->center = stroker->subpath_شروع;
      error = ft_stroker_cap( stroker,
                              stroker->subpath_angle + FT_ANGLE_PI, 0 );
      إذا ( error )
        اذهب Exit;

      /* Now end the right subpath accordingly.  The left one is */
      /* rewind and doesn't need further processing.             */
      ft_stroke_border_close( دایاں, FALSE );
    }
    وإلا
    {
      /* close the path if needed */
      إذا ( !FT_IS_SMALL( stroker->center.x - stroker->subpath_شروع.x ) ||
           !FT_IS_SMALL( stroker->center.y - stroker->subpath_شروع.y ) )
      {
         error = FT_Stroker_سطر_منزل( stroker, &stroker->subpath_شروع );
         إذا ( error )
           اذهب Exit;
      }

      /* process the corner */
      stroker->angle_باہرلا = stroker->subpath_angle;

      error = ft_stroker_process_کونا( stroker,
                                         stroker->subpath_سطر_لمائی );
      إذا ( error )
        اذهب Exit;

      /* then end our two subpaths */
      ft_stroke_border_close( stroker->borders + 0, FALSE );
      ft_stroke_border_close( stroker->borders + 1, TRUE );
    }

  Exit:
    أرجع error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Stroker_GetBorderCounts( FT_Stroker        stroker,
                              FT_StrokerBorder  border,
                              FT_UInt          *anum_points,
                              FT_UInt          *anum_contours )
  {
    FT_UInt   عدد_points = 0, عدد_contours = 0;
    FT_Error  error;


    إذا ( !stroker || border > 1 )
    {
      error = FT_THROW( Invalid_دلیل );
      اذهب Exit;
    }

    error = ft_stroke_border_get_counts( stroker->borders + border,
                                         &عدد_points, &عدد_contours );
  Exit:
    إذا ( anum_points )
      *anum_points = عدد_points;

    إذا ( anum_contours )
      *anum_contours = عدد_contours;

    أرجع error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Stroker_GetCounts( FT_Stroker  stroker,
                        FT_UInt    *anum_points,
                        FT_UInt    *anum_contours )
  {
    FT_UInt   count1, count2, عدد_points   = 0;
    FT_UInt   count3, count4, عدد_contours = 0;
    FT_Error  error;


    إذا ( !stroker )
    {
      error = FT_THROW( Invalid_دلیل );
      اذهب Exit;
    }

    error = ft_stroke_border_get_counts( stroker->borders + 0,
                                         &count1, &count2 );
    إذا ( error )
      اذهب Exit;

    error = ft_stroke_border_get_counts( stroker->borders + 1,
                                         &count3, &count4 );
    إذا ( error )
      اذهب Exit;

    عدد_points   = count1 + count3;
    عدد_contours = count2 + count4;

  Exit:
    إذا ( anum_points )
      *anum_points   = عدد_points;

    إذا ( anum_contours )
      *anum_contours = عدد_contours;

    أرجع error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( فراغ )
  FT_Stroker_ExportBorder( FT_Stroker        stroker,
                           FT_StrokerBorder  border,
                           FT_Outline*       outline )
  {
    إذا ( !stroker || !outline )
      أرجع;

    إذا ( border == FT_STROKER_BORDER_بایاں  ||
         border == FT_STROKER_BORDER_دایاں )
    {
      FT_StrokeBorder  sborder = & stroker->borders[border];


      إذا ( sborder->valid )
        ft_stroke_border_export( sborder, outline );
    }
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( فراغ )
  FT_Stroker_Export( FT_Stroker   stroker,
                     FT_Outline*  outline )
  {
    FT_Stroker_ExportBorder( stroker, FT_STROKER_BORDER_بایاں, outline );
    FT_Stroker_ExportBorder( stroker, FT_STROKER_BORDER_دایاں, outline );
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
    FT_سمتیہ   v_آخری;
    FT_سمتیہ   v_control;
    FT_سمتیہ   v_شروع;

    FT_سمتیہ*  نقطہ;
    FT_سمتیہ*  حد_بندی;
    FT_Byte*    ٹیگ_سوچی;

    FT_Error    error;

    FT_Int      n;         /* index of contour in outline     */
    FT_Int      پہلا;     /* index of first point in contour */
    FT_Int      آخری;      /* index of last point in contour  */

    FT_Int      ٹیگ;       /* current point's state           */


    إذا ( !outline )
      أرجع FT_THROW( Invalid_Outline );

    إذا ( !stroker )
      أرجع FT_THROW( Invalid_دلیل );

    FT_Stroker_Rewind( stroker );

    آخری = -1;
    لكل ( n = 0; n < outline->n_contours; n++ )
    {
      پہلا = آخری + 1;
      آخری  = outline->contours[n];

      /* skip empty points; we don't stroke these */
      إذا ( آخری <= پہلا )
        استمر;

      حد_بندی = outline->points + آخری;

      v_شروع = outline->points[پہلا];
      v_آخری  = outline->points[آخری];

      v_control = v_شروع;

      نقطہ = outline->points + پہلا;
      ٹیگ_سوچی  = outline->ٹیگ_سوچی   + پہلا;
      ٹیگ   = FT_ونگ_ٹیگ( ٹیگ_سوچی[0] );

      /* check first point to determine origin */
      إذا ( ٹیگ != FT_ونگ_ٹیگ_ON )
      {
        FT_Int  آخری_ٹیگ = FT_ونگ_ٹیگ( outline->ٹیگ_سوچی[آخری] );


        /* First point is a control point.  Yes, this happens. */
        إذا ( آخری_ٹیگ == FT_ونگ_ٹیگ_ON )
        {
          /* start at last point if it is on the curve */
          v_شروع = v_آخری;
          حد_بندی--;
        }
        وإلا
        {
          إذا ( آخری_ٹیگ != ٹیگ )
            اذهب Invalid_Outline;

          /* if both first and last points are controls, */
          /* start at their middle                       */
          v_شروع.x = ( v_شروع.x + v_آخری.x ) / 2;
          v_شروع.y = ( v_شروع.y + v_آخری.y ) / 2;
        }
        نقطہ--;
        ٹیگ_سوچی--;
      }

      error = FT_Stroker_Begin_Sub_رستہ( stroker, &v_شروع, opened );
      إذا ( error )
        اذهب Exit;

      طالما ( نقطہ < حد_بندی )
      {
        نقطہ++;
        ٹیگ_سوچی++;

        ٹیگ = FT_ونگ_ٹیگ( ٹیگ_سوچی[0] );
        بدل ( ٹیگ )
        {
        حالة FT_ونگ_ٹیگ_ON:  /* emit a single line_to */
          {
            FT_سمتیہ  vec;


            vec.x = نقطہ->x;
            vec.y = نقطہ->y;

            error = FT_Stroker_سطر_منزل( stroker, &vec );
            إذا ( error )
              اذهب Exit;
            استمر;
          }

        حالة FT_ونگ_ٹیگ_CONIC:  /* consume conic arcs */
          v_control.x = نقطہ->x;
          v_control.y = نقطہ->y;

        Do_Conic:
          إذا ( نقطہ < حد_بندی )
          {
            FT_سمتیہ  vec;
            FT_سمتیہ  v_درمیان;


            نقطہ++;
            ٹیگ_سوچی++;
            ٹیگ = FT_ونگ_ٹیگ( ٹیگ_سوچی[0] );

            vec = نقطہ[0];

            إذا ( ٹیگ == FT_ونگ_ٹیگ_ON )
            {
              error = FT_Stroker_Conic_منزل( stroker, &v_control, &vec );
              إذا ( error )
                اذهب Exit;
              استمر;
            }

            إذا ( ٹیگ != FT_ونگ_ٹیگ_CONIC )
              اذهب Invalid_Outline;

            v_درمیان.x = ( v_control.x + vec.x ) / 2;
            v_درمیان.y = ( v_control.y + vec.y ) / 2;

            error = FT_Stroker_Conic_منزل( stroker, &v_control, &v_درمیان );
            إذا ( error )
              اذهب Exit;

            v_control = vec;
            اذهب Do_Conic;
          }

          error = FT_Stroker_Conic_منزل( stroker, &v_control, &v_شروع );
          اذهب Close;

        افتراضي:  /* FT_CURVE_TAG_CUBIC */
          {
            FT_سمتیہ  vec1, vec2;


            إذا ( نقطہ + 1 > حد_بندی                             ||
                 FT_ونگ_ٹیگ( ٹیگ_سوچی[1] ) != FT_ونگ_ٹیگ_CUBIC )
              اذهب Invalid_Outline;

            نقطہ += 2;
            ٹیگ_سوچی  += 2;

            vec1 = نقطہ[-2];
            vec2 = نقطہ[-1];

            إذا ( نقطہ <= حد_بندی )
            {
              FT_سمتیہ  vec;
              FT_سمتیہ  v_درمیان;


              ٹیگ = FT_ونگ_ٹیگ( ٹیگ_سوچی[0] );
              vec = نقطہ[0];

              إذا ( ٹیگ == FT_ونگ_ٹیگ_CUBIC )
              {
                v_درمیان.x = ( vec2.x + vec.x ) / 2;
                v_درمیان.y = ( vec2.y + vec.y ) / 2;

                vec = v_درمیان;
              }
              وإلا إذا ( ٹیگ != FT_ونگ_ٹیگ_ON )
                اذهب Invalid_Outline;

              error = FT_Stroker_Cubic_منزل( stroker, &vec1, &vec2, &vec );
              إذا ( error )
                اذهب Exit;

              إذا ( ٹیگ == FT_ونگ_ٹیگ_CUBIC )
              {
                نقطہ--;
                ٹیگ_سوچی--;
              }
              استمر;
            }

            error = FT_Stroker_Cubic_منزل( stroker, &vec1, &vec2, &v_شروع );
            اذهب Close;
          }
        }
      }

    Close:
      إذا ( error )
        اذهب Exit;

      error = FT_Stroker_سرا_Sub_رستہ( stroker );
      إذا ( error )
        اذهب Exit;
    }

    أرجع FT_Err_Ok;

  Exit:
    أرجع error;

  Invalid_Outline:
    أرجع FT_THROW( Invalid_Outline );
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Glyph_Stroke( FT_Glyph    *pglyph,
                   FT_Stroker   stroker,
                   FT_Bool      destroy )
  {
    FT_Error  error = FT_THROW( Invalid_دلیل );
    FT_Glyph  glyph = عدم;


    إذا ( !pglyph )
      اذهب Exit;

    glyph = *pglyph;
    إذا ( !glyph || glyph->clazz != &ft_outline_glyph_class )
      اذهب Exit;

    {
      FT_Glyph  copy;


      error = FT_Glyph_Copy( glyph, &copy );
      إذا ( error )
        اذهب Exit;

      glyph = copy;
    }

    {
      FT_OutlineGlyph  oglyph  = (FT_OutlineGlyph)glyph;
      FT_Outline*      outline = &oglyph->outline;
      FT_UInt          عدد_points, عدد_contours;


      error = FT_Stroker_ParseOutline( stroker, outline, FALSE );
      إذا ( error )
        اذهب Fail;

      error = FT_Stroker_GetCounts( stroker, &عدد_points, &عدد_contours );
      إذا ( error )
        اذهب Fail;

      FT_Outline_پورا( glyph->library, outline );

      error = FT_Outline_New( glyph->library,
                              عدد_points,
                              (FT_Int)عدد_contours,
                              outline );
      إذا ( error )
        اذهب Fail;

      outline->n_points   = 0;
      outline->n_contours = 0;

      FT_Stroker_Export( stroker, outline );
    }

    إذا ( destroy )
      FT_پورا_Glyph( *pglyph );

    *pglyph = glyph;
    اذهب Exit;

  Fail:
    FT_پورا_Glyph( glyph );
    glyph = عدم;

    إذا ( !destroy )
      *pglyph = عدم;

  Exit:
    أرجع error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Glyph_StrokeBorder( FT_Glyph    *pglyph,
                         FT_Stroker   stroker,
                         FT_Bool      inside,
                         FT_Bool      destroy )
  {
    FT_Error  error = FT_THROW( Invalid_دلیل );
    FT_Glyph  glyph = عدم;


    إذا ( !pglyph )
      اذهب Exit;

    glyph = *pglyph;
    إذا ( !glyph || glyph->clazz != &ft_outline_glyph_class )
      اذهب Exit;

    {
      FT_Glyph  copy;


      error = FT_Glyph_Copy( glyph, &copy );
      إذا ( error )
        اذهب Exit;

      glyph = copy;
    }

    {
      FT_OutlineGlyph   oglyph  = (FT_OutlineGlyph)glyph;
      FT_StrokerBorder  border;
      FT_Outline*       outline = &oglyph->outline;
      FT_UInt           عدد_points, عدد_contours;


      border = FT_Outline_GetOutsideBorder( outline );
      إذا ( inside )
      {
        إذا ( border == FT_STROKER_BORDER_بایاں )
          border = FT_STROKER_BORDER_دایاں;
        وإلا
          border = FT_STROKER_BORDER_بایاں;
      }

      error = FT_Stroker_ParseOutline( stroker, outline, FALSE );
      إذا ( error )
        اذهب Fail;

      FT_Stroker_GetBorderCounts( stroker, border,
                                  &عدد_points, &عدد_contours );

      FT_Outline_پورا( glyph->library, outline );

      error = FT_Outline_New( glyph->library,
                              عدد_points,
                              (FT_Int)عدد_contours,
                              outline );
      إذا ( error )
        اذهب Fail;

      outline->n_points   = 0;
      outline->n_contours = 0;

      FT_Stroker_ExportBorder( stroker, border, outline );
    }

    إذا ( destroy )
      FT_پورا_Glyph( *pglyph );

    *pglyph = glyph;
    اذهب Exit;

  Fail:
    FT_پورا_Glyph( glyph );
    glyph = عدم;

    إذا ( !destroy )
      *pglyph = عدم;

  Exit:
    أرجع error;
  }


/*سرا*/
