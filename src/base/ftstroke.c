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


#شامل <freetype/ftstroke.h>
#شامل <freetype/fttrigon.h>
#شامل <freetype/ftoutln.h>
#شامل <freetype/internal/ftmemory.h>
#شامل <freetype/internal/ftdebug.h>
#شامل <freetype/internal/ftobjs.h>


  /* declare an extern to access `ft_outline_glyph_class' globally */
  /* allocated  in `ftglyph.c'                                     */
  FT_CALLBACK_جدول ثابت FT_Glyph_Class  ft_outline_glyph_class;


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_StrokerBorder )
  FT_Outline_GetInsideBorder( FT_Outline*  outline )
  {
    FT_Orientation  o = FT_Outline_Get_Orientation( outline );


    واپس o == FT_ORIENTATION_TRUETYPE ? FT_STROKER_BORDER_دایاں
                                        : FT_STROKER_BORDER_بایاں;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_StrokerBorder )
  FT_Outline_GetOutsideBorder( FT_Outline*  outline )
  {
    FT_Orientation  o = FT_Outline_Get_Orientation( outline );


    واپس o == FT_ORIENTATION_TRUETYPE ? FT_STROKER_BORDER_بایاں
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


  ساکن FT_Pos
  ft_pos_abs( FT_Pos  x )
  {
    واپس x >= 0 ? x : -x;
  }


  ساکن خالی
  ft_conic_بانٹو( FT_سمتیہ*  بنیاد )
  {
    FT_Pos  a, b;


    بنیاد[4].x = بنیاد[2].x;
    a = بنیاد[0].x + بنیاد[1].x;
    b = بنیاد[1].x + بنیاد[2].x;
    بنیاد[3].x = b >> 1;
    بنیاد[2].x = ( a + b ) >> 2;
    بنیاد[1].x = a >> 1;

    بنیاد[4].y = بنیاد[2].y;
    a = بنیاد[0].y + بنیاد[1].y;
    b = بنیاد[1].y + بنیاد[2].y;
    بنیاد[3].y = b >> 1;
    بنیاد[2].y = ( a + b ) >> 2;
    بنیاد[1].y = a >> 1;
  }


  ساکن FT_Bool
  ft_conic_is_small_enough( FT_سمتیہ*  بنیاد,
                            FT_Angle   *angle_اندر,
                            FT_Angle   *angle_برآمد )
  {
    FT_سمتیہ  d1, d2;
    FT_Angle   theta;
    FT_Int     close1, close2;


    d1.x = بنیاد[1].x - بنیاد[2].x;
    d1.y = بنیاد[1].y - بنیاد[2].y;
    d2.x = بنیاد[0].x - بنیاد[1].x;
    d2.y = بنیاد[0].y - بنیاد[1].y;

    close1 = FT_IS_SMALL( d1.x ) && FT_IS_SMALL( d1.y );
    close2 = FT_IS_SMALL( d2.x ) && FT_IS_SMALL( d2.y );

    اگر ( close1 )
    {
      اگر ( close2 )
      {
        /* basically a point;                      */
        /* do nothing to retain original direction */
      }
      ورنہ
      {
        *angle_اندر  =
        *angle_برآمد = FT_Atan2( d2.x, d2.y );
      }
    }
    ورنہ /* !close1 */
    {
      اگر ( close2 )
      {
        *angle_اندر  =
        *angle_برآمد = FT_Atan2( d1.x, d1.y );
      }
      ورنہ
      {
        *angle_اندر  = FT_Atan2( d1.x, d1.y );
        *angle_برآمد = FT_Atan2( d2.x, d2.y );
      }
    }

    theta = ft_pos_abs( FT_Angle_Diff( *angle_اندر, *angle_برآمد ) );

    واپس FT_BOOL( theta < FT_SMALL_CONIC_حد );
  }


  ساکن خالی
  ft_cubic_بانٹو( FT_سمتیہ*  بنیاد )
  {
    FT_Pos  a, b, c;


    بنیاد[6].x = بنیاد[3].x;
    a = بنیاد[0].x + بنیاد[1].x;
    b = بنیاد[1].x + بنیاد[2].x;
    c = بنیاد[2].x + بنیاد[3].x;
    بنیاد[5].x = c >> 1;
    c += b;
    بنیاد[4].x = c >> 2;
    بنیاد[1].x = a >> 1;
    a += b;
    بنیاد[2].x = a >> 2;
    بنیاد[3].x = ( a + c ) >> 3;

    بنیاد[6].y = بنیاد[3].y;
    a = بنیاد[0].y + بنیاد[1].y;
    b = بنیاد[1].y + بنیاد[2].y;
    c = بنیاد[2].y + بنیاد[3].y;
    بنیاد[5].y = c >> 1;
    c += b;
    بنیاد[4].y = c >> 2;
    بنیاد[1].y = a >> 1;
    a += b;
    بنیاد[2].y = a >> 2;
    بنیاد[3].y = ( a + c ) >> 3;
  }


  /* Return the average of `angle1' and `angle2'.            */
  /* This gives correct result even if `angle1' and `angle2' */
  /* have opposite signs.                                    */
  ساکن FT_Angle
  ft_angle_اوسط( FT_Angle  angle1,
                 FT_Angle  angle2 )
  {
    واپس angle1 + FT_Angle_Diff( angle1, angle2 ) / 2;
  }


  ساکن FT_Bool
  ft_cubic_is_small_enough( FT_سمتیہ*  بنیاد,
                            FT_Angle   *angle_اندر,
                            FT_Angle   *angle_mid,
                            FT_Angle   *angle_برآمد )
  {
    FT_سمتیہ  d1, d2, d3;
    FT_Angle   theta1, theta2;
    FT_Int     close1, close2, close3;


    d1.x = بنیاد[2].x - بنیاد[3].x;
    d1.y = بنیاد[2].y - بنیاد[3].y;
    d2.x = بنیاد[1].x - بنیاد[2].x;
    d2.y = بنیاد[1].y - بنیاد[2].y;
    d3.x = بنیاد[0].x - بنیاد[1].x;
    d3.y = بنیاد[0].y - بنیاد[1].y;

    close1 = FT_IS_SMALL( d1.x ) && FT_IS_SMALL( d1.y );
    close2 = FT_IS_SMALL( d2.x ) && FT_IS_SMALL( d2.y );
    close3 = FT_IS_SMALL( d3.x ) && FT_IS_SMALL( d3.y );

    اگر ( close1 )
    {
      اگر ( close2 )
      {
        اگر ( close3 )
        {
          /* basically a point;                      */
          /* do nothing to retain original direction */
        }
        ورنہ /* !close3 */
        {
          *angle_اندر  =
          *angle_mid =
          *angle_برآمد = FT_Atan2( d3.x, d3.y );
        }
      }
      ورنہ /* !close2 */
      {
        اگر ( close3 )
        {
          *angle_اندر  =
          *angle_mid =
          *angle_برآمد = FT_Atan2( d2.x, d2.y );
        }
        ورنہ /* !close3 */
        {
          *angle_اندر  =
          *angle_mid = FT_Atan2( d2.x, d2.y );
          *angle_برآمد = FT_Atan2( d3.x, d3.y );
        }
      }
    }
    ورنہ /* !close1 */
    {
      اگر ( close2 )
      {
        اگر ( close3 )
        {
          *angle_اندر  =
          *angle_mid =
          *angle_برآمد = FT_Atan2( d1.x, d1.y );
        }
        ورنہ /* !close3 */
        {
          *angle_اندر  = FT_Atan2( d1.x, d1.y );
          *angle_برآمد = FT_Atan2( d3.x, d3.y );
          *angle_mid = ft_angle_اوسط( *angle_اندر, *angle_برآمد );
        }
      }
      ورنہ /* !close2 */
      {
        اگر ( close3 )
        {
          *angle_اندر  = FT_Atan2( d1.x, d1.y );
          *angle_mid =
          *angle_برآمد = FT_Atan2( d2.x, d2.y );
        }
        ورنہ /* !close3 */
        {
          *angle_اندر  = FT_Atan2( d1.x, d1.y );
          *angle_mid = FT_Atan2( d2.x, d2.y );
          *angle_برآمد = FT_Atan2( d3.x, d3.y );
        }
      }
    }

    theta1 = ft_pos_abs( FT_Angle_Diff( *angle_اندر,  *angle_mid ) );
    theta2 = ft_pos_abs( FT_Angle_Diff( *angle_mid, *angle_برآمد ) );

    واپس FT_BOOL( theta1 < FT_SMALL_CUBIC_حد &&
                    theta2 < FT_SMALL_CUBIC_حد );
  }


  /*************************************************************************/
  /*************************************************************************/
  /*****                                                               *****/
  /*****                       STROKE BORDERS                          *****/
  /*****                                                               *****/
  /*************************************************************************/
  /*************************************************************************/

  قسمتعریف شمار  FT_Stroke_لیبل_فہرست
  {
    FT_STROKE_لیبل_ON    = 1,   /* on-curve point  */
    FT_STROKE_لیبل_CUBIC = 2,   /* cubic off-point */
    FT_STROKE_لیبل_BEGIN = 4,   /* sub-path start  */
    FT_STROKE_لیبل_انتہا   = 8    /* sub-path end    */

  } FT_Stroke_لیبل_فہرست_2;

#تعریف_کرو  FT_STROKE_لیبل_BEGIN_انتہا  ( FT_STROKE_لیبل_BEGIN | FT_STROKE_لیبل_انتہا )

  قسمتعریف ساخت  FT_StrokeBorderRec_
  {
    FT_UInt     عدد_points;
    FT_UInt     max_points;
    FT_سمتیہ*  points;
    FT_Byte*    لیبل_فہرست;
    FT_Bool     movable;  /* TRUE for ends of lineto borders */
    FT_Int      آغاز;    /* index of current sub-path start point */
    FT_حافظہ   حافظہ;
    FT_Bool     valid;

  } FT_StrokeBorderRec, *FT_StrokeBorder;


  ساکن FT_Error
  ft_stroke_border_grow( FT_StrokeBorder  border,
                         FT_UInt          new_points )
  {
    FT_UInt   old_max = border->max_points;
    FT_UInt   new_max = border->عدد_points + new_points;
    FT_Error  error   = FT_Err_Ok;


    اگر ( new_max > old_max )
    {
      FT_UInt    cur_max = old_max;
      FT_حافظہ  حافظہ  = border->حافظہ;


      جب ( cur_max < new_max )
        cur_max += ( cur_max >> 1 ) + 16;

      اگر ( FT_RENEW_ARRAY( border->points, old_max, cur_max ) ||
           FT_RENEW_ARRAY( border->لیبل_فہرست,   old_max, cur_max ) )
        جاؤ Exit;

      border->max_points = cur_max;
    }

  Exit:
    واپس error;
  }


  ساکن خالی
  ft_stroke_border_close( FT_StrokeBorder  border,
                          FT_Bool          الٹ )
  {
    FT_UInt  آغاز = (FT_UInt)border->آغاز;
    FT_UInt  گنتی = border->عدد_points;


    FT_ASSERT( border->آغاز >= 0 );

    /* don't record empty paths! */
    اگر ( گنتی <= آغاز + 1U )
      border->عدد_points = آغاز;
    ورنہ
    {
      /* copy the last point to the start of this sub-path, since */
      /* it contains the `adjusted' starting coordinates          */
      border->عدد_points    = --گنتی;
      border->points[آغاز] = border->points[گنتی];
      border->لیبل_فہرست[آغاز]   = border->لیبل_فہرست[گنتی];

      اگر ( الٹ )
      {
        /* reverse the points */
        {
          FT_سمتیہ*  vec1 = border->points + آغاز + 1;
          FT_سمتیہ*  vec2 = border->points + گنتی - 1;


          برائے ( ; vec1 < vec2; vec1++, vec2-- )
          {
            FT_سمتیہ  tmp;


            tmp   = *vec1;
            *vec1 = *vec2;
            *vec2 = tmp;
          }
        }

        /* then the tags */
        {
          FT_Byte*  tag1 = border->لیبل_فہرست + آغاز + 1;
          FT_Byte*  tag2 = border->لیبل_فہرست + گنتی - 1;


          برائے ( ; tag1 < tag2; tag1++, tag2-- )
          {
            FT_Byte  tmp;


            tmp   = *tag1;
            *tag1 = *tag2;
            *tag2 = tmp;
          }
        }
      }

      border->لیبل_فہرست[آغاز    ] |= FT_STROKE_لیبل_BEGIN;
      border->لیبل_فہرست[گنتی - 1] |= FT_STROKE_لیبل_انتہا;
    }

    border->آغاز   = -1;
    border->movable = FALSE;
  }


  ساکن FT_Error
  ft_stroke_border_lineto( FT_StrokeBorder  border,
                           FT_سمتیہ*       مقصد,
                           FT_Bool          movable )
  {
    FT_Error  error = FT_Err_Ok;


    FT_ASSERT( border->آغاز >= 0 );

    اگر ( border->movable )
    {
      /* move last point */
      border->points[border->عدد_points - 1] = *مقصد;
    }
    ورنہ
    {
      /* don't add zero-length lineto, but always add moveto */
      اگر ( border->عدد_points > (FT_UInt)border->آغاز                     &&
           FT_IS_SMALL( border->points[border->عدد_points - 1].x - مقصد->x ) &&
           FT_IS_SMALL( border->points[border->عدد_points - 1].y - مقصد->y ) )
        واپس error;

      /* add one point */
      error = ft_stroke_border_grow( border, 1 );
      اگر ( !error )
      {
        FT_سمتیہ*  vec = border->points + border->عدد_points;
        FT_Byte*    لیبل = border->لیبل_فہرست   + border->عدد_points;


        vec[0] = *مقصد;
        لیبل[0] = FT_STROKE_لیبل_ON;

        border->عدد_points += 1;
      }
    }
    border->movable = movable;
    واپس error;
  }


  ساکن FT_Error
  ft_stroke_border_conicto( FT_StrokeBorder  border,
                            FT_سمتیہ*       control,
                            FT_سمتیہ*       مقصد )
  {
    FT_Error  error;


    FT_ASSERT( border->آغاز >= 0 );

    error = ft_stroke_border_grow( border, 2 );
    اگر ( !error )
    {
      FT_سمتیہ*  vec = border->points + border->عدد_points;
      FT_Byte*    لیبل = border->لیبل_فہرست   + border->عدد_points;


      vec[0] = *control;
      vec[1] = *مقصد;

      لیبل[0] = 0;
      لیبل[1] = FT_STROKE_لیبل_ON;

      border->عدد_points += 2;
    }

    border->movable = FALSE;

    واپس error;
  }


  ساکن FT_Error
  ft_stroke_border_cubicto( FT_StrokeBorder  border,
                            FT_سمتیہ*       control1,
                            FT_سمتیہ*       control2,
                            FT_سمتیہ*       مقصد )
  {
    FT_Error  error;


    FT_ASSERT( border->آغاز >= 0 );

    error = ft_stroke_border_grow( border, 3 );
    اگر ( !error )
    {
      FT_سمتیہ*  vec = border->points + border->عدد_points;
      FT_Byte*    لیبل = border->لیبل_فہرست   + border->عدد_points;


      vec[0] = *control1;
      vec[1] = *control2;
      vec[2] = *مقصد;

      لیبل[0] = FT_STROKE_لیبل_CUBIC;
      لیبل[1] = FT_STROKE_لیبل_CUBIC;
      لیبل[2] = FT_STROKE_لیبل_ON;

      border->عدد_points += 3;
    }

    border->movable = FALSE;

    واپس error;
  }


#تعریف_کرو FT_ARC_CUBIC_ANGLE  ( FT_ANGLE_PI / 2 )


  ساکن FT_Error
  ft_stroke_border_arcto( FT_StrokeBorder  border,
                          FT_سمتیہ*       center,
                          FT_Fixed         radius,
                          FT_Angle         angle_آغاز,
                          FT_Angle         angle_diff )
  {
    FT_Fixed   coef;
    FT_سمتیہ  a0, a1, a2, a3;
    FT_Int     i, arcs = 1;
    FT_Error   error = FT_Err_Ok;


    /* number of cubic arcs to draw */
    جب (  angle_diff > FT_ARC_CUBIC_ANGLE * arcs ||
            -angle_diff > FT_ARC_CUBIC_ANGLE * arcs )
      arcs++;

    /* control tangents */
    coef  = FT_Tan( angle_diff / ( 4 * arcs ) );
    coef += coef / 3;

    /* compute start and first control point */
    FT_سمتیہ_From_Polar( &a0, radius, angle_آغاز );
    a1.x = FT_MulFix( -a0.y, coef );
    a1.y = FT_MulFix(  a0.x, coef );

    a0.x += center->x;
    a0.y += center->y;
    a1.x += a0.x;
    a1.y += a0.y;

    برائے ( i = 1; i <= arcs; i++ )
    {
      /* compute end and second control point */
      FT_سمتیہ_From_Polar( &a3, radius,
                            angle_آغاز + i * angle_diff / arcs );
      a2.x = FT_MulFix(  a3.y, coef );
      a2.y = FT_MulFix( -a3.x, coef );

      a3.x += center->x;
      a3.y += center->y;
      a2.x += a3.x;
      a2.y += a3.y;

      /* add cubic arc */
      error = ft_stroke_border_cubicto( border, &a1, &a2, &a3 );
      اگر ( error )
        توڑو;

      /* a0 = a3; */
      a1.x = a3.x - a2.x + a3.x;
      a1.y = a3.y - a2.y + a3.y;
    }

    واپس error;
  }


  ساکن FT_Error
  ft_stroke_border_moveto( FT_StrokeBorder  border,
                           FT_سمتیہ*       مقصد )
  {
    /* close current open path if any ? */
    اگر ( border->آغاز >= 0 )
      ft_stroke_border_close( border, FALSE );

    border->آغاز = (FT_Int)border->عدد_points;
    border->movable = FALSE;

    واپس ft_stroke_border_lineto( border, مقصد, FALSE );
  }


  ساکن خالی
  ft_stroke_border_init( FT_StrokeBorder  border,
                         FT_حافظہ        حافظہ )
  {
    border->حافظہ = حافظہ;
    border->points = تہی;
    border->لیبل_فہرست   = تہی;

    border->عدد_points = 0;
    border->max_points = 0;
    border->آغاز      = -1;
    border->valid      = FALSE;
  }


  ساکن خالی
  ft_stroke_border_reset( FT_StrokeBorder  border )
  {
    border->عدد_points = 0;
    border->آغاز      = -1;
    border->valid      = FALSE;
  }


  ساکن خالی
  ft_stroke_border_مکمل( FT_StrokeBorder  border )
  {
    FT_حافظہ  حافظہ = border->حافظہ;


    FT_FREE( border->points );
    FT_FREE( border->لیبل_فہرست );

    border->عدد_points = 0;
    border->max_points = 0;
    border->آغاز      = -1;
    border->valid      = FALSE;
  }


  ساکن FT_Error
  ft_stroke_border_get_counts( FT_StrokeBorder  border,
                               FT_UInt         *anum_points,
                               FT_UInt         *anum_contours )
  {
    FT_Error  error        = FT_Err_Ok;
    FT_UInt   عدد_points   = 0;
    FT_UInt   عدد_contours = 0;

    FT_UInt     گنتی      = border->عدد_points;
    FT_Byte*    لیبل_فہرست       = border->لیبل_فہرست;
    FT_Int      اندر_contour = 0;


    برائے ( ; گنتی > 0; گنتی--, عدد_points++, لیبل_فہرست++ )
    {
      اگر ( لیبل_فہرست[0] & FT_STROKE_لیبل_BEGIN )
      {
        اگر ( اندر_contour != 0 )
          جاؤ Fail;

        اندر_contour = 1;
      }
      ورنہ اگر ( اندر_contour == 0 )
        جاؤ Fail;

      اگر ( لیبل_فہرست[0] & FT_STROKE_لیبل_انتہا )
      {
        اندر_contour = 0;
        عدد_contours++;
      }
    }

    اگر ( اندر_contour != 0 )
      جاؤ Fail;

    border->valid = TRUE;

  Exit:
    *anum_points   = عدد_points;
    *anum_contours = عدد_contours;
    واپس error;

  Fail:
    عدد_points   = 0;
    عدد_contours = 0;
    error        = FT_THROW( Invalid_Outline );
    جاؤ Exit;
  }


  ساکن خالی
  ft_stroke_border_export( FT_StrokeBorder  border,
                           FT_Outline*      outline )
  {
    /* copy point locations */
    اگر ( border->عدد_points )
      FT_ARRAY_COPY( outline->points + outline->n_points,
                     border->points,
                     border->عدد_points );

    /* copy tags */
    {
      FT_UInt   گنتی = border->عدد_points;
      FT_Byte*  اقرأ  = border->لیبل_فہرست;
      FT_Byte*  اكتب = outline->لیبل_فہرست + outline->n_points;


      برائے ( ; گنتی > 0; گنتی--, اقرأ++, اكتب++ )
      {
        اگر ( *اقرأ & FT_STROKE_لیبل_ON )
          *اكتب = FT_خم_لیبل_ON;
        ورنہ اگر ( *اقرأ & FT_STROKE_لیبل_CUBIC )
          *اكتب = FT_خم_لیبل_CUBIC;
        ورنہ
          *اكتب = FT_خم_لیبل_CONIC;
      }
    }

    /* copy contours */
    {
      FT_UInt     گنتی = border->عدد_points;
      FT_Byte*    لیبل_فہرست  = border->لیبل_فہرست;
      FT_UShort*  اكتب = outline->contours + outline->n_contours;
      FT_UShort   idx   = outline->n_points;


      برائے ( ; گنتی > 0; گنتی--, لیبل_فہرست++, idx++ )
      {
        اگر ( *لیبل_فہرست & FT_STROKE_لیبل_انتہا )
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

#تعریف_کرو FT_SIDE_مقصد_ROTATE( s )   ( FT_ANGLE_PI2 - (s) * FT_ANGLE_PI )

  قسمتعریف ساخت  FT_StrokerRec_
  {
    FT_Angle             angle_اندر;             /* direction into curr join */
    FT_Angle             angle_برآمد;            /* direction out of join  */
    FT_سمتیہ            center;               /* current position */
    FT_Fixed             سطر_لمبائی;          /* length of last lineto */
    FT_Bool              پہلا_نقطہ;          /* is this the start? */
    FT_Bool              subpath_open;         /* is the subpath open? */
    FT_Angle             subpath_angle;        /* subpath start direction */
    FT_سمتیہ            subpath_آغاز;        /* subpath start position */
    FT_Fixed             subpath_سطر_لمبائی;  /* subpath start lineto len */
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
    FT_Stroker  stroker = تہی;


    اگر ( !library )
      واپس FT_THROW( Invalid_Library_Handle );

    اگر ( !astroker )
      واپس FT_THROW( Invalid_دلیل );

    حافظہ = library->حافظہ;

    اگر ( !FT_NEW( stroker ) )
    {
      stroker->library = library;

      ft_stroke_border_init( &stroker->borders[0], حافظہ );
      ft_stroke_border_init( &stroker->borders[1], حافظہ );
    }

    *astroker = stroker;

    واپس error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( خالی )
  FT_Stroker_سیٹ( FT_Stroker           stroker,
                  FT_Fixed             radius,
                  FT_Stroker_سطر_Cap   سطر_cap,
                  FT_Stroker_سطر_Join  سطر_join,
                  FT_Fixed             miter_حد_بندی )
  {
    اگر ( !stroker )
      واپس;

    stroker->radius      = radius;
    stroker->سطر_cap    = سطر_cap;
    stroker->سطر_join   = سطر_join;
    stroker->miter_حد_بندی = miter_حد_بندی;

    /* ensure miter limit has sensible value */
    اگر ( stroker->miter_حد_بندی < 0x10000L )
      stroker->miter_حد_بندی = 0x10000L;

    /* save line join style:                                           */
    /* line join style can be temporarily changed when stroking curves */
    stroker->سطر_join_saved = سطر_join;

    FT_Stroker_Rewind( stroker );
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( خالی )
  FT_Stroker_Rewind( FT_Stroker  stroker )
  {
    اگر ( stroker )
    {
      ft_stroke_border_reset( &stroker->borders[0] );
      ft_stroke_border_reset( &stroker->borders[1] );
    }
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( خالی )
  FT_Stroker_مکمل( FT_Stroker  stroker )
  {
    اگر ( stroker )
    {
      FT_حافظہ  حافظہ = stroker->library->حافظہ;


      ft_stroke_border_مکمل( &stroker->borders[0] );
      ft_stroke_border_مکمل( &stroker->borders[1] );

      stroker->library = تہی;
      FT_FREE( stroker );
    }
  }


  /* create a circular arc at a corner or cap */
  ساکن FT_Error
  ft_stroker_arcto( FT_Stroker  stroker,
                    FT_Int      side )
  {
    FT_Angle         کل, rotate;
    FT_Fixed         radius = stroker->radius;
    FT_Error         error  = FT_Err_Ok;
    FT_StrokeBorder  border = stroker->borders + side;


    rotate = FT_SIDE_مقصد_ROTATE( side );

    کل = FT_Angle_Diff( stroker->angle_اندر, stroker->angle_برآمد );
    اگر ( کل == FT_ANGLE_PI )
      کل = -rotate * 2;

    error = ft_stroke_border_arcto( border,
                                    &stroker->center,
                                    radius,
                                    stroker->angle_اندر + rotate,
                                    کل );
    border->movable = FALSE;
    واپس error;
  }


  /* add a cap at the end of an opened path */
  ساکن FT_Error
  ft_stroker_cap( FT_Stroker  stroker,
                  FT_Angle    angle,
                  FT_Int      side )
  {
    FT_Error  error = FT_Err_Ok;


    اگر ( stroker->سطر_cap == FT_STROKER_LINECAP_باری )
    {
      /* add a round cap */
      stroker->angle_اندر  = angle;
      stroker->angle_برآمد = angle + FT_ANGLE_PI;

      error = ft_stroker_arcto( stroker, side );
    }
    ورنہ
    {
      /* add a square or butt cap */
      FT_سمتیہ        درمیان, delta;
      FT_Fixed         radius = stroker->radius;
      FT_StrokeBorder  border = stroker->borders + side;


      /* compute middle point and first angle point */
      FT_سمتیہ_From_Polar( &درمیان, radius, angle );
      delta.x = side ?  درمیان.y : -درمیان.y;
      delta.y = side ? -درمیان.x :  درمیان.x;

      اگر ( stroker->سطر_cap == FT_STROKER_LINECAP_مربع )
      {
        درمیان.x += stroker->center.x;
        درمیان.y += stroker->center.y;
      }
      ورنہ  /* FT_STROKER_LINECAP_BUTT */
      {
        درمیان.x  = stroker->center.x;
        درمیان.y  = stroker->center.y;
      }

      delta.x  += درمیان.x;
      delta.y  += درمیان.y;

      error = ft_stroke_border_lineto( border, &delta, FALSE );
      اگر ( error )
        جاؤ Exit;

      /* compute second angle point */
      delta.x = درمیان.x - delta.x + درمیان.x;
      delta.y = درمیان.y - delta.y + درمیان.y;

      error = ft_stroke_border_lineto( border, &delta, FALSE );
    }

  Exit:
    واپس error;
  }


  /* process an inside corner, i.e. compute intersection */
  ساکن FT_Error
  ft_stroker_inside( FT_Stroker  stroker,
                     FT_Int      side,
                     FT_Fixed    سطر_لمبائی )
  {
    FT_StrokeBorder  border = stroker->borders + side;
    FT_Angle         phi, theta, rotate;
    FT_Fixed         لمبائی;
    FT_سمتیہ        sigma = { 0, 0 };
    FT_سمتیہ        delta;
    FT_Error         error = FT_Err_Ok;
    FT_Bool          intersect;          /* use intersection of lines? */


    rotate = FT_SIDE_مقصد_ROTATE( side );

    theta = FT_Angle_Diff( stroker->angle_اندر, stroker->angle_برآمد ) / 2;

    /* Only intersect borders if between two lineto's and both */
    /* lines are long enough (line_length is zero for curves). */
    /* Also avoid U-turns of nearly 180 degree.                */
    اگر ( !border->movable || سطر_لمبائی == 0  ||
         theta > 0x59C000 || theta < -0x59C000 )
      intersect = FALSE;
    ورنہ
    {
      /* compute minimum required length of lines */
      FT_Fixed  min_لمبائی;


      FT_سمتیہ_Unit( &sigma, theta );
      min_لمبائی =
        ft_pos_abs( FT_MulDiv( stroker->radius, sigma.y, sigma.x ) );

      intersect = FT_BOOL( min_لمبائی                         &&
                           stroker->سطر_لمبائی >= min_لمبائی &&
                           سطر_لمبائی          >= min_لمبائی );
    }

    اگر ( !intersect )
    {
      FT_سمتیہ_From_Polar( &delta, stroker->radius,
                            stroker->angle_برآمد + rotate );
      delta.x += stroker->center.x;
      delta.y += stroker->center.y;

      border->movable = FALSE;
    }
    ورنہ
    {
      /* compute median angle */
      phi = stroker->angle_اندر + theta + rotate;

      لمبائی = FT_DivFix( stroker->radius, sigma.x );

      FT_سمتیہ_From_Polar( &delta, لمبائی, phi );
      delta.x += stroker->center.x;
      delta.y += stroker->center.y;
    }

    error = ft_stroke_border_lineto( border, &delta, FALSE );

    واپس error;
  }


  /* process an outside corner, i.e. compute bevel/miter/round */
  ساکن FT_Error
  ft_stroker_outside( FT_Stroker  stroker,
                      FT_Int      side,
                      FT_Fixed    سطر_لمبائی )
  {
    FT_StrokeBorder  border = stroker->borders + side;
    FT_Error         error;
    FT_Angle         rotate;


    اگر ( stroker->سطر_join == FT_STROKER_LINEJOIN_باری )
      error = ft_stroker_arcto( stroker, side );
    ورنہ
    {
      /* this is a mitered (pointed) or beveled (truncated) corner */
      FT_Fixed   radius = stroker->radius;
      FT_سمتیہ  sigma = { 0, 0 };
      FT_Angle   theta = 0, phi = 0;
      FT_Bool    bevel, fixed_bevel;


      rotate = FT_SIDE_مقصد_ROTATE( side );

      bevel =
        FT_BOOL( stroker->سطر_join == FT_STROKER_LINEJOIN_BEVEL );

      fixed_bevel =
        FT_BOOL( stroker->سطر_join != FT_STROKER_LINEJOIN_MITER_VARIABLE );

      /* check miter limit first */
      اگر ( !bevel )
      {
        theta = FT_Angle_Diff( stroker->angle_اندر, stroker->angle_برآمد ) / 2;

        اگر ( theta == FT_ANGLE_PI2 )
          theta = -rotate;

        phi = stroker->angle_اندر + theta + rotate;

        FT_سمتیہ_From_Polar( &sigma, stroker->miter_حد_بندی, theta );

        /* is miter limit exceeded? */
        اگر ( sigma.x < 0x10000L )
        {
          /* don't create variable bevels for very small deviations; */
          /* FT_Sin(x) = 0 for x <= 57                               */
          اگر ( fixed_bevel || ft_pos_abs( theta ) > 57 )
            bevel = TRUE;
        }
      }

      اگر ( bevel )  /* this is a bevel (broken angle) */
      {
        اگر ( fixed_bevel )
        {
          /* the outer corners are simply joined together */
          FT_سمتیہ  delta;


          /* add bevel */
          FT_سمتیہ_From_Polar( &delta,
                                radius,
                                stroker->angle_برآمد + rotate );
          delta.x += stroker->center.x;
          delta.y += stroker->center.y;

          border->movable = FALSE;
          error = ft_stroke_border_lineto( border, &delta, FALSE );
        }
        ورنہ /* variable bevel or clipped miter */
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
          اگر ( error )
            جاؤ Exit;

          /* compute second angle point */
          delta.x = درمیان.x - delta.x + درمیان.x;
          delta.y = درمیان.y - delta.y + درمیان.y;

          error = ft_stroke_border_lineto( border, &delta, FALSE );
          اگر ( error )
            جاؤ Exit;

          /* finally, add an end point; only needed if not lineto */
          /* (line_length is zero for curves)                     */
          اگر ( سطر_لمبائی == 0 )
          {
            FT_سمتیہ_From_Polar( &delta,
                                  radius,
                                  stroker->angle_برآمد + rotate );

            delta.x += stroker->center.x;
            delta.y += stroker->center.y;

            error = ft_stroke_border_lineto( border, &delta, FALSE );
          }
        }
      }
      ورنہ /* this is a miter (intersection) */
      {
        FT_Fixed   لمبائی;
        FT_سمتیہ  delta;


        لمبائی = FT_MulDiv( stroker->radius, stroker->miter_حد_بندی, sigma.x );

        FT_سمتیہ_From_Polar( &delta, لمبائی, phi );
        delta.x += stroker->center.x;
        delta.y += stroker->center.y;

        error = ft_stroke_border_lineto( border, &delta, FALSE );
        اگر ( error )
          جاؤ Exit;

        /* now add an end point; only needed if not lineto */
        /* (line_length is zero for curves)                */
        اگر ( سطر_لمبائی == 0 )
        {
          FT_سمتیہ_From_Polar( &delta,
                                stroker->radius,
                                stroker->angle_برآمد + rotate );
          delta.x += stroker->center.x;
          delta.y += stroker->center.y;

          error = ft_stroke_border_lineto( border, &delta, FALSE );
        }
      }
    }

  Exit:
    واپس error;
  }


  ساکن FT_Error
  ft_stroker_process_کونا( FT_Stroker  stroker,
                             FT_Fixed    سطر_لمبائی )
  {
    FT_Error  error = FT_Err_Ok;
    FT_Angle  turn;
    FT_Int    inside_side;


    turn = FT_Angle_Diff( stroker->angle_اندر, stroker->angle_برآمد );

    /* no specific corner processing is required if the turn is 0 */
    اگر ( turn == 0 )
      جاؤ Exit;

    /* when we turn to the right, the inside side is 0 */
    /* otherwise, the inside side is 1 */
    inside_side = ( turn < 0 );

    /* process the inside side */
    error = ft_stroker_inside( stroker, inside_side, سطر_لمبائی );
    اگر ( error )
      جاؤ Exit;

    /* process the outside side */
    error = ft_stroker_outside( stroker, !inside_side, سطر_لمبائی );

  Exit:
    واپس error;
  }


  /* add two points to the left and right borders corresponding to the */
  /* start of the subpath                                              */
  ساکن FT_Error
  ft_stroker_subpath_آغاز( FT_Stroker  stroker,
                            FT_Angle    آغاز_angle,
                            FT_Fixed    سطر_لمبائی )
  {
    FT_سمتیہ        delta;
    FT_سمتیہ        نقطہ;
    FT_Error         error;
    FT_StrokeBorder  border;


    FT_سمتیہ_From_Polar( &delta, stroker->radius,
                          آغاز_angle + FT_ANGLE_PI2 );

    نقطہ.x = stroker->center.x + delta.x;
    نقطہ.y = stroker->center.y + delta.y;

    border = stroker->borders;
    error = ft_stroke_border_moveto( border, &نقطہ );
    اگر ( error )
      جاؤ Exit;

    نقطہ.x = stroker->center.x - delta.x;
    نقطہ.y = stroker->center.y - delta.y;

    border++;
    error = ft_stroke_border_moveto( border, &نقطہ );

    /* save angle, position, and line length for last join */
    /* (line_length is zero for curves)                    */
    stroker->subpath_angle       = آغاز_angle;
    stroker->پہلا_نقطہ         = FALSE;
    stroker->subpath_سطر_لمبائی = سطر_لمبائی;

  Exit:
    واپس error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Stroker_سطر_مقصد( FT_Stroker  stroker,
                     FT_سمتیہ*  مقصد )
  {
    FT_Error         error = FT_Err_Ok;
    FT_StrokeBorder  border;
    FT_سمتیہ        delta;
    FT_Angle         angle;
    FT_Int           side;
    FT_Fixed         سطر_لمبائی;


    اگر ( !stroker || !مقصد )
      واپس FT_THROW( Invalid_دلیل );

    delta.x = مقصد->x - stroker->center.x;
    delta.y = مقصد->y - stroker->center.y;

    /* a zero-length lineto is a no-op; avoid creating a spurious corner */
    اگر ( delta.x == 0 && delta.y == 0 )
       جاؤ Exit;

    /* compute length of line */
    سطر_لمبائی = FT_سمتیہ_لمبائی( &delta );

    angle = FT_Atan2( delta.x, delta.y );
    FT_سمتیہ_From_Polar( &delta, stroker->radius, angle + FT_ANGLE_PI2 );

    /* process corner if necessary */
    اگر ( stroker->پہلا_نقطہ )
    {
      /* This is the first segment of a subpath.  We need to     */
      /* add a point to each border at their respective starting */
      /* point locations.                                        */
      error = ft_stroker_subpath_آغاز( stroker, angle, سطر_لمبائی );
      اگر ( error )
        جاؤ Exit;
    }
    ورنہ
    {
      /* process the current corner */
      stroker->angle_برآمد = angle;
      error = ft_stroker_process_کونا( stroker, سطر_لمبائی );
      اگر ( error )
        جاؤ Exit;
    }

    /* now add a line segment to both the `inside' and `outside' paths */
    برائے ( border = stroker->borders, side = 1; side >= 0; side--, border++ )
    {
      FT_سمتیہ  نقطہ;


      نقطہ.x = مقصد->x + delta.x;
      نقطہ.y = مقصد->y + delta.y;

      /* the ends of lineto borders are movable */
      error = ft_stroke_border_lineto( border, &نقطہ, TRUE );
      اگر ( error )
        جاؤ Exit;

      delta.x = -delta.x;
      delta.y = -delta.y;
    }

    stroker->angle_اندر    = angle;
    stroker->center      = *مقصد;
    stroker->سطر_لمبائی = سطر_لمبائی;

  Exit:
    واپس error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Stroker_Conic_مقصد( FT_Stroker  stroker,
                      FT_سمتیہ*  control,
                      FT_سمتیہ*  مقصد )
  {
    FT_Error    error = FT_Err_Ok;
    FT_سمتیہ   bez_ڈھیر[34];
    FT_سمتیہ*  arc;
    FT_سمتیہ*  حد_بندی = bez_ڈھیر + 30;
    FT_Bool     پہلا_arc = TRUE;


    اگر ( !stroker || !control || !مقصد )
    {
      error = FT_THROW( Invalid_دلیل );
      جاؤ Exit;
    }

    /* if all control points are coincident, this is a no-op; */
    /* avoid creating a spurious corner                       */
    اگر ( FT_IS_SMALL( stroker->center.x - control->x ) &&
         FT_IS_SMALL( stroker->center.y - control->y ) &&
         FT_IS_SMALL( control->x        - مقصد->x      ) &&
         FT_IS_SMALL( control->y        - مقصد->y      ) )
    {
       stroker->center = *مقصد;
       جاؤ Exit;
    }

    arc    = bez_ڈھیر;
    arc[0] = *مقصد;
    arc[1] = *control;
    arc[2] = stroker->center;

    کرو
    {
      FT_Angle  angle_اندر, angle_برآمد;


      /* initialize with current direction */
      angle_اندر = angle_برآمد = stroker->angle_اندر;

      اگر ( arc < حد_بندی                                             &&
           !ft_conic_is_small_enough( arc, &angle_اندر, &angle_برآمد ) )
      {
        اگر ( stroker->پہلا_نقطہ )
          stroker->angle_اندر = angle_اندر;

        ft_conic_بانٹو( arc );
        arc += 2;
        جاری;
      }

      اگر ( پہلا_arc )
      {
        پہلا_arc = FALSE;

        /* process corner if necessary */
        اگر ( stroker->پہلا_نقطہ )
          error = ft_stroker_subpath_آغاز( stroker, angle_اندر, 0 );
        ورنہ
        {
          stroker->angle_برآمد = angle_اندر;
          error = ft_stroker_process_کونا( stroker, 0 );
        }
      }
      ورنہ اگر ( ft_pos_abs( FT_Angle_Diff( stroker->angle_اندر, angle_اندر ) ) >
                  FT_SMALL_CONIC_حد / 4                             )
      {
        /* if the deviation from one arc to the next is too great, */
        /* add a round corner                                      */
        stroker->center    = arc[2];
        stroker->angle_برآمد = angle_اندر;
        stroker->سطر_join = FT_STROKER_LINEJOIN_باری;

        error = ft_stroker_process_کونا( stroker, 0 );

        /* reinstate line join style */
        stroker->سطر_join = stroker->سطر_join_saved;
      }

      اگر ( error )
        جاؤ Exit;

      /* the arc's angle is small enough; we can add it directly to each */
      /* border                                                          */
      {
        FT_سمتیہ        ctrl, انتہا;
        FT_Angle         theta, phi, rotate, alpha0 = 0;
        FT_Fixed         لمبائی;
        FT_StrokeBorder  border;
        FT_Int           side;


        theta  = FT_Angle_Diff( angle_اندر, angle_برآمد ) / 2;
        phi    = angle_اندر + theta;
        لمبائی = FT_DivFix( stroker->radius, FT_Cos( theta ) );

        /* compute direction of original arc */
        اگر ( stroker->handle_wide_strokes )
          alpha0 = FT_Atan2( arc[0].x - arc[2].x, arc[0].y - arc[2].y );

        برائے ( border = stroker->borders, side = 0;
              side <= 1;
              side++, border++ )
        {
          rotate = FT_SIDE_مقصد_ROTATE( side );

          /* compute control point */
          FT_سمتیہ_From_Polar( &ctrl, لمبائی, phi + rotate );
          ctrl.x += arc[1].x;
          ctrl.y += arc[1].y;

          /* compute end point */
          FT_سمتیہ_From_Polar( &انتہا, stroker->radius, angle_برآمد + rotate );
          انتہا.x += arc[0].x;
          انتہا.y += arc[0].y;

          اگر ( stroker->handle_wide_strokes )
          {
            FT_سمتیہ  آغاز;
            FT_Angle   alpha1;


            /* determine whether the border radius is greater than the */
            /* radius of curvature of the original arc                 */
            آغاز = border->points[border->عدد_points - 1];

            alpha1 = FT_Atan2( انتہا.x - آغاز.x, انتہا.y - آغاز.y );

            /* is the direction of the border arc opposite to */
            /* that of the original arc? */
            اگر ( ft_pos_abs( FT_Angle_Diff( alpha0, alpha1 ) ) >
                   FT_ANGLE_PI / 2                             )
            {
              FT_Angle   beta, gamma;
              FT_سمتیہ  bvec, delta;
              FT_Fixed   blen, sinA, sinB, alen;


              /* use the sine rule to find the intersection point */
              beta  = FT_Atan2( arc[2].x - آغاز.x, arc[2].y - آغاز.y );
              gamma = FT_Atan2( arc[0].x - انتہا.x,   arc[0].y - انتہا.y );

              bvec.x = انتہا.x - آغاز.x;
              bvec.y = انتہا.y - آغاز.y;

              blen = FT_سمتیہ_لمبائی( &bvec );

              sinA = ft_pos_abs( FT_Sin( alpha1 - gamma ) );
              sinB = ft_pos_abs( FT_Sin( beta - gamma ) );

              alen = FT_MulDiv( blen, sinA, sinB );

              FT_سمتیہ_From_Polar( &delta, alen, beta );
              delta.x += آغاز.x;
              delta.y += آغاز.y;

              /* circumnavigate the negative sector backwards */
              border->movable = FALSE;
              error = ft_stroke_border_lineto( border, &delta, FALSE );
              اگر ( error )
                جاؤ Exit;
              error = ft_stroke_border_lineto( border, &انتہا, FALSE );
              اگر ( error )
                جاؤ Exit;
              error = ft_stroke_border_conicto( border, &ctrl, &آغاز );
              اگر ( error )
                جاؤ Exit;
              /* and then move to the endpoint */
              error = ft_stroke_border_lineto( border, &انتہا, FALSE );
              اگر ( error )
                جاؤ Exit;

              جاری;
            }

            /* else fall through */
          }

          /* simply add an arc */
          error = ft_stroke_border_conicto( border, &ctrl, &انتہا );
          اگر ( error )
            جاؤ Exit;
        }
      }

      stroker->angle_اندر = angle_برآمد;

      اگر ( arc == bez_ڈھیر )
        توڑو;
      arc -= 2;
    } جب ( 1 );

    stroker->center      = *مقصد;
    stroker->سطر_لمبائی = 0;

  Exit:
    واپس error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Stroker_Cubic_مقصد( FT_Stroker  stroker,
                      FT_سمتیہ*  control1,
                      FT_سمتیہ*  control2,
                      FT_سمتیہ*  مقصد )
  {
    FT_Error    error = FT_Err_Ok;
    FT_سمتیہ   bez_ڈھیر[37];
    FT_سمتیہ*  arc;
    FT_سمتیہ*  حد_بندی = bez_ڈھیر + 32;
    FT_Bool     پہلا_arc = TRUE;


    اگر ( !stroker || !control1 || !control2 || !مقصد )
    {
      error = FT_THROW( Invalid_دلیل );
      جاؤ Exit;
    }

    /* if all control points are coincident, this is a no-op; */
    /* avoid creating a spurious corner */
    اگر ( FT_IS_SMALL( stroker->center.x - control1->x ) &&
         FT_IS_SMALL( stroker->center.y - control1->y ) &&
         FT_IS_SMALL( control1->x       - control2->x ) &&
         FT_IS_SMALL( control1->y       - control2->y ) &&
         FT_IS_SMALL( control2->x       - مقصد->x       ) &&
         FT_IS_SMALL( control2->y       - مقصد->y       ) )
    {
       stroker->center = *مقصد;
       جاؤ Exit;
    }

    arc    = bez_ڈھیر;
    arc[0] = *مقصد;
    arc[1] = *control2;
    arc[2] = *control1;
    arc[3] = stroker->center;

    کرو
    {
      FT_Angle  angle_اندر, angle_mid, angle_برآمد;


      /* initialize with current direction */
      angle_اندر = angle_برآمد = angle_mid = stroker->angle_اندر;

      اگر ( arc < حد_بندی                                         &&
           !ft_cubic_is_small_enough( arc, &angle_اندر,
                                      &angle_mid, &angle_برآمد ) )
      {
        اگر ( stroker->پہلا_نقطہ )
          stroker->angle_اندر = angle_اندر;

        ft_cubic_بانٹو( arc );
        arc += 3;
        جاری;
      }

      اگر ( پہلا_arc )
      {
        پہلا_arc = FALSE;

        /* process corner if necessary */
        اگر ( stroker->پہلا_نقطہ )
          error = ft_stroker_subpath_آغاز( stroker, angle_اندر, 0 );
        ورنہ
        {
          stroker->angle_برآمد = angle_اندر;
          error = ft_stroker_process_کونا( stroker, 0 );
        }
      }
      ورنہ اگر ( ft_pos_abs( FT_Angle_Diff( stroker->angle_اندر, angle_اندر ) ) >
                  FT_SMALL_CUBIC_حد / 4                             )
      {
        /* if the deviation from one arc to the next is too great, */
        /* add a round corner                                      */
        stroker->center    = arc[3];
        stroker->angle_برآمد = angle_اندر;
        stroker->سطر_join = FT_STROKER_LINEJOIN_باری;

        error = ft_stroker_process_کونا( stroker, 0 );

        /* reinstate line join style */
        stroker->سطر_join = stroker->سطر_join_saved;
      }

      اگر ( error )
        جاؤ Exit;

      /* the arc's angle is small enough; we can add it directly to each */
      /* border                                                          */
      {
        FT_سمتیہ        ctrl1, ctrl2, انتہا;
        FT_Angle         theta1, phi1, theta2, phi2, rotate, alpha0 = 0;
        FT_Fixed         length1, length2;
        FT_StrokeBorder  border;
        FT_Int           side;


        theta1  = FT_Angle_Diff( angle_اندر,  angle_mid ) / 2;
        theta2  = FT_Angle_Diff( angle_mid, angle_برآمد ) / 2;
        phi1    = ft_angle_اوسط( angle_اندر,  angle_mid );
        phi2    = ft_angle_اوسط( angle_mid, angle_برآمد );
        length1 = FT_DivFix( stroker->radius, FT_Cos( theta1 ) );
        length2 = FT_DivFix( stroker->radius, FT_Cos( theta2 ) );

        /* compute direction of original arc */
        اگر ( stroker->handle_wide_strokes )
          alpha0 = FT_Atan2( arc[0].x - arc[3].x, arc[0].y - arc[3].y );

        برائے ( border = stroker->borders, side = 0;
              side <= 1;
              side++, border++ )
        {
          rotate = FT_SIDE_مقصد_ROTATE( side );

          /* compute control points */
          FT_سمتیہ_From_Polar( &ctrl1, length1, phi1 + rotate );
          ctrl1.x += arc[2].x;
          ctrl1.y += arc[2].y;

          FT_سمتیہ_From_Polar( &ctrl2, length2, phi2 + rotate );
          ctrl2.x += arc[1].x;
          ctrl2.y += arc[1].y;

          /* compute end point */
          FT_سمتیہ_From_Polar( &انتہا, stroker->radius, angle_برآمد + rotate );
          انتہا.x += arc[0].x;
          انتہا.y += arc[0].y;

          اگر ( stroker->handle_wide_strokes )
          {
            FT_سمتیہ  آغاز;
            FT_Angle   alpha1;


            /* determine whether the border radius is greater than the */
            /* radius of curvature of the original arc                 */
            آغاز = border->points[border->عدد_points - 1];

            alpha1 = FT_Atan2( انتہا.x - آغاز.x, انتہا.y - آغاز.y );

            /* is the direction of the border arc opposite to */
            /* that of the original arc? */
            اگر ( ft_pos_abs( FT_Angle_Diff( alpha0, alpha1 ) ) >
                   FT_ANGLE_PI / 2                             )
            {
              FT_Angle   beta, gamma;
              FT_سمتیہ  bvec, delta;
              FT_Fixed   blen, sinA, sinB, alen;


              /* use the sine rule to find the intersection point */
              beta  = FT_Atan2( arc[3].x - آغاز.x, arc[3].y - آغاز.y );
              gamma = FT_Atan2( arc[0].x - انتہا.x,   arc[0].y - انتہا.y );

              bvec.x = انتہا.x - آغاز.x;
              bvec.y = انتہا.y - آغاز.y;

              blen = FT_سمتیہ_لمبائی( &bvec );

              sinA = ft_pos_abs( FT_Sin( alpha1 - gamma ) );
              sinB = ft_pos_abs( FT_Sin( beta - gamma ) );

              alen = FT_MulDiv( blen, sinA, sinB );

              FT_سمتیہ_From_Polar( &delta, alen, beta );
              delta.x += آغاز.x;
              delta.y += آغاز.y;

              /* circumnavigate the negative sector backwards */
              border->movable = FALSE;
              error = ft_stroke_border_lineto( border, &delta, FALSE );
              اگر ( error )
                جاؤ Exit;
              error = ft_stroke_border_lineto( border, &انتہا, FALSE );
              اگر ( error )
                جاؤ Exit;
              error = ft_stroke_border_cubicto( border,
                                                &ctrl2,
                                                &ctrl1,
                                                &آغاز );
              اگر ( error )
                جاؤ Exit;
              /* and then move to the endpoint */
              error = ft_stroke_border_lineto( border, &انتہا, FALSE );
              اگر ( error )
                جاؤ Exit;

              جاری;
            }

            /* else fall through */
          }

          /* simply add an arc */
          error = ft_stroke_border_cubicto( border, &ctrl1, &ctrl2, &انتہا );
          اگر ( error )
            جاؤ Exit;
        }
      }

      stroker->angle_اندر = angle_برآمد;

      اگر ( arc == bez_ڈھیر )
        توڑو;
      arc -= 3;
    } جب ( 1 );

    stroker->center      = *مقصد;
    stroker->سطر_لمبائی = 0;

  Exit:
    واپس error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Stroker_Begin_Sub_راستہ( FT_Stroker  stroker,
                           FT_سمتیہ*  مقصد,
                           FT_Bool     open )
  {
    اگر ( !stroker || !مقصد )
      واپس FT_THROW( Invalid_دلیل );

    /* We cannot process the first point, because there is not enough      */
    /* information regarding its corner/cap.  The latter will be processed */
    /* in the `FT_Stroker_EndSubPath' routine.                             */
    /*                                                                     */
    stroker->پہلا_نقطہ  = TRUE;
    stroker->center       = *مقصد;
    stroker->subpath_open = open;

    /* Determine if we need to check whether the border radius is greater */
    /* than the radius of curvature of a curve, to handle this case       */
    /* specially.  This is only required if bevel joins or butt caps may  */
    /* be created, because round & miter joins and round & square caps    */
    /* cover the negative sector created with wide strokes.               */
    stroker->handle_wide_strokes =
      FT_BOOL( stroker->سطر_join != FT_STROKER_LINEJOIN_باری  ||
               ( stroker->subpath_open                        &&
                 stroker->سطر_cap == FT_STROKER_LINECAP_BUTT ) );

    /* record the subpath start point for each border */
    stroker->subpath_آغاز = *مقصد;

    stroker->angle_اندر = 0;

    واپس FT_Err_Ok;
  }


  ساکن FT_Error
  ft_stroker_جوڑو_الٹ_بایاں( FT_Stroker  stroker,
                               FT_Bool     open )
  {
    FT_StrokeBorder  دایاں = stroker->borders + 0;
    FT_StrokeBorder  بایاں  = stroker->borders + 1;
    FT_Int           new_points;
    FT_Error         error = FT_Err_Ok;


    FT_ASSERT( بایاں->آغاز >= 0 );

    new_points = (FT_Int)بایاں->عدد_points - بایاں->آغاز;
    اگر ( new_points > 0 )
    {
      error = ft_stroke_border_grow( دایاں, (FT_UInt)new_points );
      اگر ( error )
        جاؤ Exit;

      {
        FT_سمتیہ*  dst_نقطہ = دایاں->points + دایاں->عدد_points;
        FT_Byte*    dst_لیبل   = دایاں->لیبل_فہرست   + دایاں->عدد_points;
        FT_سمتیہ*  src_نقطہ = بایاں->points  + بایاں->عدد_points - 1;
        FT_Byte*    src_لیبل   = بایاں->لیبل_فہرست    + بایاں->عدد_points - 1;


        جب ( src_نقطہ >= بایاں->points + بایاں->آغاز )
        {
          *dst_نقطہ = *src_نقطہ;
          *dst_لیبل   = *src_لیبل;

          اگر ( open )
            dst_لیبل[0] &= ~FT_STROKE_لیبل_BEGIN_انتہا;
          ورنہ
          {
            FT_Byte  ttag =
                       (FT_Byte)( dst_لیبل[0] & FT_STROKE_لیبل_BEGIN_انتہا );


            /* switch begin/end tags if necessary */
            اگر ( ttag == FT_STROKE_لیبل_BEGIN ||
                 ttag == FT_STROKE_لیبل_انتہا   )
              dst_لیبل[0] ^= FT_STROKE_لیبل_BEGIN_انتہا;
          }

          src_نقطہ--;
          src_لیبل--;
          dst_نقطہ++;
          dst_لیبل++;
        }
      }

      بایاں->عدد_points   = (FT_UInt)بایاں->آغاز;
      دایاں->عدد_points += (FT_UInt)new_points;

      دایاں->movable = FALSE;
      بایاں->movable  = FALSE;
    }

  Exit:
    واپس error;
  }


  /* documentation is in ftstroke.h */

  /* there's a lot of magic in this function! */
  FT_EXPORT_DEF( FT_Error )
  FT_Stroker_انتہا_Sub_راستہ( FT_Stroker  stroker )
  {
    FT_Error  error = FT_Err_Ok;


    اگر ( !stroker )
    {
      error = FT_THROW( Invalid_دلیل );
      جاؤ Exit;
    }

    /* don't try to end the path if no segments have been generated */
    اگر ( stroker->پہلا_نقطہ )
      جاؤ Exit;

    اگر ( stroker->subpath_open )
    {
      FT_StrokeBorder  دایاں = stroker->borders;


      /* All right, this is an opened path, we need to add a cap between */
      /* right & left, add the reverse of left, then add a final cap     */
      /* between left & right.                                           */
      error = ft_stroker_cap( stroker, stroker->angle_اندر, 0 );
      اگر ( error )
        جاؤ Exit;

      /* add reversed points from `left' to `right' */
      error = ft_stroker_جوڑو_الٹ_بایاں( stroker, TRUE );
      اگر ( error )
        جاؤ Exit;

      /* now add the final cap */
      stroker->center = stroker->subpath_آغاز;
      error = ft_stroker_cap( stroker,
                              stroker->subpath_angle + FT_ANGLE_PI, 0 );
      اگر ( error )
        جاؤ Exit;

      /* Now end the right subpath accordingly.  The left one is */
      /* rewind and doesn't need further processing.             */
      ft_stroke_border_close( دایاں, FALSE );
    }
    ورنہ
    {
      /* close the path if needed */
      اگر ( !FT_IS_SMALL( stroker->center.x - stroker->subpath_آغاز.x ) ||
           !FT_IS_SMALL( stroker->center.y - stroker->subpath_آغاز.y ) )
      {
         error = FT_Stroker_سطر_مقصد( stroker, &stroker->subpath_آغاز );
         اگر ( error )
           جاؤ Exit;
      }

      /* process the corner */
      stroker->angle_برآمد = stroker->subpath_angle;

      error = ft_stroker_process_کونا( stroker,
                                         stroker->subpath_سطر_لمبائی );
      اگر ( error )
        جاؤ Exit;

      /* then end our two subpaths */
      ft_stroke_border_close( stroker->borders + 0, FALSE );
      ft_stroke_border_close( stroker->borders + 1, TRUE );
    }

  Exit:
    واپس error;
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


    اگر ( !stroker || border > 1 )
    {
      error = FT_THROW( Invalid_دلیل );
      جاؤ Exit;
    }

    error = ft_stroke_border_get_counts( stroker->borders + border,
                                         &عدد_points, &عدد_contours );
  Exit:
    اگر ( anum_points )
      *anum_points = عدد_points;

    اگر ( anum_contours )
      *anum_contours = عدد_contours;

    واپس error;
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


    اگر ( !stroker )
    {
      error = FT_THROW( Invalid_دلیل );
      جاؤ Exit;
    }

    error = ft_stroke_border_get_counts( stroker->borders + 0,
                                         &count1, &count2 );
    اگر ( error )
      جاؤ Exit;

    error = ft_stroke_border_get_counts( stroker->borders + 1,
                                         &count3, &count4 );
    اگر ( error )
      جاؤ Exit;

    عدد_points   = count1 + count3;
    عدد_contours = count2 + count4;

  Exit:
    اگر ( anum_points )
      *anum_points   = عدد_points;

    اگر ( anum_contours )
      *anum_contours = عدد_contours;

    واپس error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( خالی )
  FT_Stroker_ExportBorder( FT_Stroker        stroker,
                           FT_StrokerBorder  border,
                           FT_Outline*       outline )
  {
    اگر ( !stroker || !outline )
      واپس;

    اگر ( border == FT_STROKER_BORDER_بایاں  ||
         border == FT_STROKER_BORDER_دایاں )
    {
      FT_StrokeBorder  sborder = & stroker->borders[border];


      اگر ( sborder->valid )
        ft_stroke_border_export( sborder, outline );
    }
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( خالی )
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
    FT_سمتیہ   v_آغاز;

    FT_سمتیہ*  نقطہ;
    FT_سمتیہ*  حد_بندی;
    FT_Byte*    لیبل_فہرست;

    FT_Error    error;

    FT_Int      n;         /* index of contour in outline     */
    FT_Int      پہلا;     /* index of first point in contour */
    FT_Int      آخری;      /* index of last point in contour  */

    FT_Int      لیبل;       /* current point's state           */


    اگر ( !outline )
      واپس FT_THROW( Invalid_Outline );

    اگر ( !stroker )
      واپس FT_THROW( Invalid_دلیل );

    FT_Stroker_Rewind( stroker );

    آخری = -1;
    برائے ( n = 0; n < outline->n_contours; n++ )
    {
      پہلا = آخری + 1;
      آخری  = outline->contours[n];

      /* skip empty points; we don't stroke these */
      اگر ( آخری <= پہلا )
        جاری;

      حد_بندی = outline->points + آخری;

      v_آغاز = outline->points[پہلا];
      v_آخری  = outline->points[آخری];

      v_control = v_آغاز;

      نقطہ = outline->points + پہلا;
      لیبل_فہرست  = outline->لیبل_فہرست   + پہلا;
      لیبل   = FT_خم_لیبل( لیبل_فہرست[0] );

      /* check first point to determine origin */
      اگر ( لیبل != FT_خم_لیبل_ON )
      {
        FT_Int  آخری_لیبل = FT_خم_لیبل( outline->لیبل_فہرست[آخری] );


        /* First point is a control point.  Yes, this happens. */
        اگر ( آخری_لیبل == FT_خم_لیبل_ON )
        {
          /* start at last point if it is on the curve */
          v_آغاز = v_آخری;
          حد_بندی--;
        }
        ورنہ
        {
          اگر ( آخری_لیبل != لیبل )
            جاؤ Invalid_Outline;

          /* if both first and last points are controls, */
          /* start at their middle                       */
          v_آغاز.x = ( v_آغاز.x + v_آخری.x ) / 2;
          v_آغاز.y = ( v_آغاز.y + v_آخری.y ) / 2;
        }
        نقطہ--;
        لیبل_فہرست--;
      }

      error = FT_Stroker_Begin_Sub_راستہ( stroker, &v_آغاز, opened );
      اگر ( error )
        جاؤ Exit;

      جب ( نقطہ < حد_بندی )
      {
        نقطہ++;
        لیبل_فہرست++;

        لیبل = FT_خم_لیبل( لیبل_فہرست[0] );
        انتخاب ( لیبل )
        {
        حالت FT_خم_لیبل_ON:  /* emit a single line_to */
          {
            FT_سمتیہ  vec;


            vec.x = نقطہ->x;
            vec.y = نقطہ->y;

            error = FT_Stroker_سطر_مقصد( stroker, &vec );
            اگر ( error )
              جاؤ Exit;
            جاری;
          }

        حالت FT_خم_لیبل_CONIC:  /* consume conic arcs */
          v_control.x = نقطہ->x;
          v_control.y = نقطہ->y;

        Do_Conic:
          اگر ( نقطہ < حد_بندی )
          {
            FT_سمتیہ  vec;
            FT_سمتیہ  v_درمیان;


            نقطہ++;
            لیبل_فہرست++;
            لیبل = FT_خم_لیبل( لیبل_فہرست[0] );

            vec = نقطہ[0];

            اگر ( لیبل == FT_خم_لیبل_ON )
            {
              error = FT_Stroker_Conic_مقصد( stroker, &v_control, &vec );
              اگر ( error )
                جاؤ Exit;
              جاری;
            }

            اگر ( لیبل != FT_خم_لیبل_CONIC )
              جاؤ Invalid_Outline;

            v_درمیان.x = ( v_control.x + vec.x ) / 2;
            v_درمیان.y = ( v_control.y + vec.y ) / 2;

            error = FT_Stroker_Conic_مقصد( stroker, &v_control, &v_درمیان );
            اگر ( error )
              جاؤ Exit;

            v_control = vec;
            جاؤ Do_Conic;
          }

          error = FT_Stroker_Conic_مقصد( stroker, &v_control, &v_آغاز );
          جاؤ Close;

        طےشدہ:  /* FT_CURVE_TAG_CUBIC */
          {
            FT_سمتیہ  vec1, vec2;


            اگر ( نقطہ + 1 > حد_بندی                             ||
                 FT_خم_لیبل( لیبل_فہرست[1] ) != FT_خم_لیبل_CUBIC )
              جاؤ Invalid_Outline;

            نقطہ += 2;
            لیبل_فہرست  += 2;

            vec1 = نقطہ[-2];
            vec2 = نقطہ[-1];

            اگر ( نقطہ <= حد_بندی )
            {
              FT_سمتیہ  vec;
              FT_سمتیہ  v_درمیان;


              لیبل = FT_خم_لیبل( لیبل_فہرست[0] );
              vec = نقطہ[0];

              اگر ( لیبل == FT_خم_لیبل_CUBIC )
              {
                v_درمیان.x = ( vec2.x + vec.x ) / 2;
                v_درمیان.y = ( vec2.y + vec.y ) / 2;

                vec = v_درمیان;
              }
              ورنہ اگر ( لیبل != FT_خم_لیبل_ON )
                جاؤ Invalid_Outline;

              error = FT_Stroker_Cubic_مقصد( stroker, &vec1, &vec2, &vec );
              اگر ( error )
                جاؤ Exit;

              اگر ( لیبل == FT_خم_لیبل_CUBIC )
              {
                نقطہ--;
                لیبل_فہرست--;
              }
              جاری;
            }

            error = FT_Stroker_Cubic_مقصد( stroker, &vec1, &vec2, &v_آغاز );
            جاؤ Close;
          }
        }
      }

    Close:
      اگر ( error )
        جاؤ Exit;

      error = FT_Stroker_انتہا_Sub_راستہ( stroker );
      اگر ( error )
        جاؤ Exit;
    }

    واپس FT_Err_Ok;

  Exit:
    واپس error;

  Invalid_Outline:
    واپس FT_THROW( Invalid_Outline );
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Glyph_Stroke( FT_Glyph    *pglyph,
                   FT_Stroker   stroker,
                   FT_Bool      destroy )
  {
    FT_Error  error = FT_THROW( Invalid_دلیل );
    FT_Glyph  glyph = تہی;


    اگر ( !pglyph )
      جاؤ Exit;

    glyph = *pglyph;
    اگر ( !glyph || glyph->clazz != &ft_outline_glyph_class )
      جاؤ Exit;

    {
      FT_Glyph  copy;


      error = FT_Glyph_Copy( glyph, &copy );
      اگر ( error )
        جاؤ Exit;

      glyph = copy;
    }

    {
      FT_OutlineGlyph  oglyph  = (FT_OutlineGlyph)glyph;
      FT_Outline*      outline = &oglyph->outline;
      FT_UInt          عدد_points, عدد_contours;


      error = FT_Stroker_ParseOutline( stroker, outline, FALSE );
      اگر ( error )
        جاؤ Fail;

      error = FT_Stroker_GetCounts( stroker, &عدد_points, &عدد_contours );
      اگر ( error )
        جاؤ Fail;

      FT_Outline_مکمل( glyph->library, outline );

      error = FT_Outline_New( glyph->library,
                              عدد_points,
                              (FT_Int)عدد_contours,
                              outline );
      اگر ( error )
        جاؤ Fail;

      outline->n_points   = 0;
      outline->n_contours = 0;

      FT_Stroker_Export( stroker, outline );
    }

    اگر ( destroy )
      FT_مکمل_Glyph( *pglyph );

    *pglyph = glyph;
    جاؤ Exit;

  Fail:
    FT_مکمل_Glyph( glyph );
    glyph = تہی;

    اگر ( !destroy )
      *pglyph = تہی;

  Exit:
    واپس error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Glyph_StrokeBorder( FT_Glyph    *pglyph,
                         FT_Stroker   stroker,
                         FT_Bool      inside,
                         FT_Bool      destroy )
  {
    FT_Error  error = FT_THROW( Invalid_دلیل );
    FT_Glyph  glyph = تہی;


    اگر ( !pglyph )
      جاؤ Exit;

    glyph = *pglyph;
    اگر ( !glyph || glyph->clazz != &ft_outline_glyph_class )
      جاؤ Exit;

    {
      FT_Glyph  copy;


      error = FT_Glyph_Copy( glyph, &copy );
      اگر ( error )
        جاؤ Exit;

      glyph = copy;
    }

    {
      FT_OutlineGlyph   oglyph  = (FT_OutlineGlyph)glyph;
      FT_StrokerBorder  border;
      FT_Outline*       outline = &oglyph->outline;
      FT_UInt           عدد_points, عدد_contours;


      border = FT_Outline_GetOutsideBorder( outline );
      اگر ( inside )
      {
        اگر ( border == FT_STROKER_BORDER_بایاں )
          border = FT_STROKER_BORDER_دایاں;
        ورنہ
          border = FT_STROKER_BORDER_بایاں;
      }

      error = FT_Stroker_ParseOutline( stroker, outline, FALSE );
      اگر ( error )
        جاؤ Fail;

      FT_Stroker_GetBorderCounts( stroker, border,
                                  &عدد_points, &عدد_contours );

      FT_Outline_مکمل( glyph->library, outline );

      error = FT_Outline_New( glyph->library,
                              عدد_points,
                              (FT_Int)عدد_contours,
                              outline );
      اگر ( error )
        جاؤ Fail;

      outline->n_points   = 0;
      outline->n_contours = 0;

      FT_Stroker_ExportBorder( stroker, border, outline );
    }

    اگر ( destroy )
      FT_مکمل_Glyph( *pglyph );

    *pglyph = glyph;
    جاؤ Exit;

  Fail:
    FT_مکمل_Glyph( glyph );
    glyph = تہی;

    اگر ( !destroy )
      *pglyph = تہی;

  Exit:
    واپس error;
  }


/*انتہا*/
