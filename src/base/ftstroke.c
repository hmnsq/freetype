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


    أرجع o == FT_ORIENTATION_TRUETYPE ? FT_STROKER_BORDER_راست
                                        : FT_STROKER_BORDER_چپ;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_StrokerBorder )
  FT_Outline_GetOutsideBorder( FT_Outline*  outline )
  {
    FT_Orientation  o = FT_Outline_Get_Orientation( outline );


    أرجع o == FT_ORIENTATION_TRUETYPE ? FT_STROKER_BORDER_چپ
                                        : FT_STROKER_BORDER_راست;
  }


  /*************************************************************************/
  /*************************************************************************/
  /*****                                                               *****/
  /*****                      BEZIER COMPUTATIONS                      *****/
  /*****                                                               *****/
  /*************************************************************************/
  /*************************************************************************/

#تعریف_کن FT_SMALL_CONIC_آستانه  ( FT_ANGLE_PI / 6 )
#تعریف_کن FT_SMALL_CUBIC_آستانه  ( FT_ANGLE_PI / 8 )

#تعریف_کن FT_EPSILON  2

#تعریف_کن FT_IS_SMALL( x )  ( (x) > -FT_EPSILON && (x) < FT_EPSILON )


  ساكن FT_Pos
  ft_pos_abs( FT_Pos  x )
  {
    أرجع x >= 0 ? x : -x;
  }


  ساكن فراغ
  ft_conic_تقسیم( FT_وکتور*  پایه )
  {
    FT_Pos  a, b;


    پایه[4].x = پایه[2].x;
    a = پایه[0].x + پایه[1].x;
    b = پایه[1].x + پایه[2].x;
    پایه[3].x = b >> 1;
    پایه[2].x = ( a + b ) >> 2;
    پایه[1].x = a >> 1;

    پایه[4].y = پایه[2].y;
    a = پایه[0].y + پایه[1].y;
    b = پایه[1].y + پایه[2].y;
    پایه[3].y = b >> 1;
    پایه[2].y = ( a + b ) >> 2;
    پایه[1].y = a >> 1;
  }


  ساكن FT_Bool
  ft_conic_is_small_enough( FT_وکتور*  پایه,
                            FT_Angle   *angle_درون,
                            FT_Angle   *angle_بیرون )
  {
    FT_وکتور  d1, d2;
    FT_Angle   theta;
    FT_Int     close1, close2;


    d1.x = پایه[1].x - پایه[2].x;
    d1.y = پایه[1].y - پایه[2].y;
    d2.x = پایه[0].x - پایه[1].x;
    d2.y = پایه[0].y - پایه[1].y;

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
        *angle_درون  =
        *angle_بیرون = FT_Atan2( d2.x, d2.y );
      }
    }
    وإلا /* !close1 */
    {
      إذا ( close2 )
      {
        *angle_درون  =
        *angle_بیرون = FT_Atan2( d1.x, d1.y );
      }
      وإلا
      {
        *angle_درون  = FT_Atan2( d1.x, d1.y );
        *angle_بیرون = FT_Atan2( d2.x, d2.y );
      }
    }

    theta = ft_pos_abs( FT_Angle_Diff( *angle_درون, *angle_بیرون ) );

    أرجع FT_BOOL( theta < FT_SMALL_CONIC_آستانه );
  }


  ساكن فراغ
  ft_cubic_تقسیم( FT_وکتور*  پایه )
  {
    FT_Pos  a, b, c;


    پایه[6].x = پایه[3].x;
    a = پایه[0].x + پایه[1].x;
    b = پایه[1].x + پایه[2].x;
    c = پایه[2].x + پایه[3].x;
    پایه[5].x = c >> 1;
    c += b;
    پایه[4].x = c >> 2;
    پایه[1].x = a >> 1;
    a += b;
    پایه[2].x = a >> 2;
    پایه[3].x = ( a + c ) >> 3;

    پایه[6].y = پایه[3].y;
    a = پایه[0].y + پایه[1].y;
    b = پایه[1].y + پایه[2].y;
    c = پایه[2].y + پایه[3].y;
    پایه[5].y = c >> 1;
    c += b;
    پایه[4].y = c >> 2;
    پایه[1].y = a >> 1;
    a += b;
    پایه[2].y = a >> 2;
    پایه[3].y = ( a + c ) >> 3;
  }


  /* Return the average of `angle1' and `angle2'.            */
  /* This gives correct result even if `angle1' and `angle2' */
  /* have opposite signs.                                    */
  ساكن FT_Angle
  ft_angle_میانگین( FT_Angle  angle1,
                 FT_Angle  angle2 )
  {
    أرجع angle1 + FT_Angle_Diff( angle1, angle2 ) / 2;
  }


  ساكن FT_Bool
  ft_cubic_is_small_enough( FT_وکتور*  پایه,
                            FT_Angle   *angle_درون,
                            FT_Angle   *angle_mid,
                            FT_Angle   *angle_بیرون )
  {
    FT_وکتور  d1, d2, d3;
    FT_Angle   theta1, theta2;
    FT_Int     close1, close2, close3;


    d1.x = پایه[2].x - پایه[3].x;
    d1.y = پایه[2].y - پایه[3].y;
    d2.x = پایه[1].x - پایه[2].x;
    d2.y = پایه[1].y - پایه[2].y;
    d3.x = پایه[0].x - پایه[1].x;
    d3.y = پایه[0].y - پایه[1].y;

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
          *angle_درون  =
          *angle_mid =
          *angle_بیرون = FT_Atan2( d3.x, d3.y );
        }
      }
      وإلا /* !close2 */
      {
        إذا ( close3 )
        {
          *angle_درون  =
          *angle_mid =
          *angle_بیرون = FT_Atan2( d2.x, d2.y );
        }
        وإلا /* !close3 */
        {
          *angle_درون  =
          *angle_mid = FT_Atan2( d2.x, d2.y );
          *angle_بیرون = FT_Atan2( d3.x, d3.y );
        }
      }
    }
    وإلا /* !close1 */
    {
      إذا ( close2 )
      {
        إذا ( close3 )
        {
          *angle_درون  =
          *angle_mid =
          *angle_بیرون = FT_Atan2( d1.x, d1.y );
        }
        وإلا /* !close3 */
        {
          *angle_درون  = FT_Atan2( d1.x, d1.y );
          *angle_بیرون = FT_Atan2( d3.x, d3.y );
          *angle_mid = ft_angle_میانگین( *angle_درون, *angle_بیرون );
        }
      }
      وإلا /* !close2 */
      {
        إذا ( close3 )
        {
          *angle_درون  = FT_Atan2( d1.x, d1.y );
          *angle_mid =
          *angle_بیرون = FT_Atan2( d2.x, d2.y );
        }
        وإلا /* !close3 */
        {
          *angle_درون  = FT_Atan2( d1.x, d1.y );
          *angle_mid = FT_Atan2( d2.x, d2.y );
          *angle_بیرون = FT_Atan2( d3.x, d3.y );
        }
      }
    }

    theta1 = ft_pos_abs( FT_Angle_Diff( *angle_درون,  *angle_mid ) );
    theta2 = ft_pos_abs( FT_Angle_Diff( *angle_mid, *angle_بیرون ) );

    أرجع FT_BOOL( theta1 < FT_SMALL_CUBIC_آستانه &&
                    theta2 < FT_SMALL_CUBIC_آستانه );
  }


  /*************************************************************************/
  /*************************************************************************/
  /*****                                                               *****/
  /*****                       STROKE BORDERS                          *****/
  /*****                                                               *****/
  /*************************************************************************/
  /*************************************************************************/

  تعريفنوع تعداد  FT_Stroke_برچسبها
  {
    FT_STROKE_برچسب_ON    = 1,   /* on-curve point  */
    FT_STROKE_برچسب_CUBIC = 2,   /* cubic off-point */
    FT_STROKE_برچسب_BEGIN = 4,   /* sub-path start  */
    FT_STROKE_برچسب_انتها   = 8    /* sub-path end    */

  } FT_Stroke_برچسبها_2;

#تعریف_کن  FT_STROKE_برچسب_BEGIN_انتها  ( FT_STROKE_برچسب_BEGIN | FT_STROKE_برچسب_انتها )

  تعريفنوع بنية  FT_StrokeBorderRec_
  {
    FT_UInt     رقم_points;
    FT_UInt     max_points;
    FT_وکتور*  points;
    FT_Byte*    برچسبها;
    FT_Bool     movable;  /* TRUE for ends of lineto borders */
    FT_Int      شروع;    /* index of current sub-path start point */
    FT_حافظه   حافظه;
    FT_Bool     valid;

  } FT_StrokeBorderRec, *FT_StrokeBorder;


  ساكن FT_Error
  ft_stroke_border_grow( FT_StrokeBorder  border,
                         FT_UInt          new_points )
  {
    FT_UInt   old_max = border->max_points;
    FT_UInt   new_max = border->رقم_points + new_points;
    FT_Error  error   = FT_Err_Ok;


    إذا ( new_max > old_max )
    {
      FT_UInt    cur_max = old_max;
      FT_حافظه  حافظه  = border->حافظه;


      طالما ( cur_max < new_max )
        cur_max += ( cur_max >> 1 ) + 16;

      إذا ( FT_RENEW_ARRAY( border->points, old_max, cur_max ) ||
           FT_RENEW_ARRAY( border->برچسبها,   old_max, cur_max ) )
        اذهب Exit;

      border->max_points = cur_max;
    }

  Exit:
    أرجع error;
  }


  ساكن فراغ
  ft_stroke_border_close( FT_StrokeBorder  border,
                          FT_Bool          معکوس )
  {
    FT_UInt  شروع = (FT_UInt)border->شروع;
    FT_UInt  شمار = border->رقم_points;


    FT_ASSERT( border->شروع >= 0 );

    /* don't record empty paths! */
    إذا ( شمار <= شروع + 1U )
      border->رقم_points = شروع;
    وإلا
    {
      /* copy the last point to the start of this sub-path, since */
      /* it contains the `adjusted' starting coordinates          */
      border->رقم_points    = --شمار;
      border->points[شروع] = border->points[شمار];
      border->برچسبها[شروع]   = border->برچسبها[شمار];

      إذا ( معکوس )
      {
        /* reverse the points */
        {
          FT_وکتور*  vec1 = border->points + شروع + 1;
          FT_وکتور*  vec2 = border->points + شمار - 1;


          لكل ( ; vec1 < vec2; vec1++, vec2-- )
          {
            FT_وکتور  tmp;


            tmp   = *vec1;
            *vec1 = *vec2;
            *vec2 = tmp;
          }
        }

        /* then the tags */
        {
          FT_Byte*  tag1 = border->برچسبها + شروع + 1;
          FT_Byte*  tag2 = border->برچسبها + شمار - 1;


          لكل ( ; tag1 < tag2; tag1++, tag2-- )
          {
            FT_Byte  tmp;


            tmp   = *tag1;
            *tag1 = *tag2;
            *tag2 = tmp;
          }
        }
      }

      border->برچسبها[شروع    ] |= FT_STROKE_برچسب_BEGIN;
      border->برچسبها[شمار - 1] |= FT_STROKE_برچسب_انتها;
    }

    border->شروع   = -1;
    border->movable = FALSE;
  }


  ساكن FT_Error
  ft_stroke_border_lineto( FT_StrokeBorder  border,
                           FT_وکتور*       مقصد,
                           FT_Bool          movable )
  {
    FT_Error  error = FT_Err_Ok;


    FT_ASSERT( border->شروع >= 0 );

    إذا ( border->movable )
    {
      /* move last point */
      border->points[border->رقم_points - 1] = *مقصد;
    }
    وإلا
    {
      /* don't add zero-length lineto, but always add moveto */
      إذا ( border->رقم_points > (FT_UInt)border->شروع                     &&
           FT_IS_SMALL( border->points[border->رقم_points - 1].x - مقصد->x ) &&
           FT_IS_SMALL( border->points[border->رقم_points - 1].y - مقصد->y ) )
        أرجع error;

      /* add one point */
      error = ft_stroke_border_grow( border, 1 );
      إذا ( !error )
      {
        FT_وکتور*  vec = border->points + border->رقم_points;
        FT_Byte*    برچسب = border->برچسبها   + border->رقم_points;


        vec[0] = *مقصد;
        برچسب[0] = FT_STROKE_برچسب_ON;

        border->رقم_points += 1;
      }
    }
    border->movable = movable;
    أرجع error;
  }


  ساكن FT_Error
  ft_stroke_border_conicto( FT_StrokeBorder  border,
                            FT_وکتور*       control,
                            FT_وکتور*       مقصد )
  {
    FT_Error  error;


    FT_ASSERT( border->شروع >= 0 );

    error = ft_stroke_border_grow( border, 2 );
    إذا ( !error )
    {
      FT_وکتور*  vec = border->points + border->رقم_points;
      FT_Byte*    برچسب = border->برچسبها   + border->رقم_points;


      vec[0] = *control;
      vec[1] = *مقصد;

      برچسب[0] = 0;
      برچسب[1] = FT_STROKE_برچسب_ON;

      border->رقم_points += 2;
    }

    border->movable = FALSE;

    أرجع error;
  }


  ساكن FT_Error
  ft_stroke_border_cubicto( FT_StrokeBorder  border,
                            FT_وکتور*       control1,
                            FT_وکتور*       control2,
                            FT_وکتور*       مقصد )
  {
    FT_Error  error;


    FT_ASSERT( border->شروع >= 0 );

    error = ft_stroke_border_grow( border, 3 );
    إذا ( !error )
    {
      FT_وکتور*  vec = border->points + border->رقم_points;
      FT_Byte*    برچسب = border->برچسبها   + border->رقم_points;


      vec[0] = *control1;
      vec[1] = *control2;
      vec[2] = *مقصد;

      برچسب[0] = FT_STROKE_برچسب_CUBIC;
      برچسب[1] = FT_STROKE_برچسب_CUBIC;
      برچسب[2] = FT_STROKE_برچسب_ON;

      border->رقم_points += 3;
    }

    border->movable = FALSE;

    أرجع error;
  }


#تعریف_کن FT_ARC_CUBIC_ANGLE  ( FT_ANGLE_PI / 2 )


  ساكن FT_Error
  ft_stroke_border_arcto( FT_StrokeBorder  border,
                          FT_وکتور*       center,
                          FT_Fixed         radius,
                          FT_Angle         angle_شروع,
                          FT_Angle         angle_diff )
  {
    FT_Fixed   coef;
    FT_وکتور  a0, a1, a2, a3;
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
    FT_وکتور_From_Polar( &a0, radius, angle_شروع );
    a1.x = FT_MulFix( -a0.y, coef );
    a1.y = FT_MulFix(  a0.x, coef );

    a0.x += center->x;
    a0.y += center->y;
    a1.x += a0.x;
    a1.y += a0.y;

    لكل ( i = 1; i <= arcs; i++ )
    {
      /* compute end and second control point */
      FT_وکتور_From_Polar( &a3, radius,
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
                           FT_وکتور*       مقصد )
  {
    /* close current open path if any ? */
    إذا ( border->شروع >= 0 )
      ft_stroke_border_close( border, FALSE );

    border->شروع = (FT_Int)border->رقم_points;
    border->movable = FALSE;

    أرجع ft_stroke_border_lineto( border, مقصد, FALSE );
  }


  ساكن فراغ
  ft_stroke_border_init( FT_StrokeBorder  border,
                         FT_حافظه        حافظه )
  {
    border->حافظه = حافظه;
    border->points = عدم;
    border->برچسبها   = عدم;

    border->رقم_points = 0;
    border->max_points = 0;
    border->شروع      = -1;
    border->valid      = FALSE;
  }


  ساكن فراغ
  ft_stroke_border_reset( FT_StrokeBorder  border )
  {
    border->رقم_points = 0;
    border->شروع      = -1;
    border->valid      = FALSE;
  }


  ساكن فراغ
  ft_stroke_border_انجامشده( FT_StrokeBorder  border )
  {
    FT_حافظه  حافظه = border->حافظه;


    FT_FREE( border->points );
    FT_FREE( border->برچسبها );

    border->رقم_points = 0;
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
    FT_UInt   رقم_points   = 0;
    FT_UInt   رقم_contours = 0;

    FT_UInt     شمار      = border->رقم_points;
    FT_Byte*    برچسبها       = border->برچسبها;
    FT_Int      درون_contour = 0;


    لكل ( ; شمار > 0; شمار--, رقم_points++, برچسبها++ )
    {
      إذا ( برچسبها[0] & FT_STROKE_برچسب_BEGIN )
      {
        إذا ( درون_contour != 0 )
          اذهب Fail;

        درون_contour = 1;
      }
      وإلا إذا ( درون_contour == 0 )
        اذهب Fail;

      إذا ( برچسبها[0] & FT_STROKE_برچسب_انتها )
      {
        درون_contour = 0;
        رقم_contours++;
      }
    }

    إذا ( درون_contour != 0 )
      اذهب Fail;

    border->valid = TRUE;

  Exit:
    *anum_points   = رقم_points;
    *anum_contours = رقم_contours;
    أرجع error;

  Fail:
    رقم_points   = 0;
    رقم_contours = 0;
    error        = FT_THROW( Invalid_Outline );
    اذهب Exit;
  }


  ساكن فراغ
  ft_stroke_border_export( FT_StrokeBorder  border,
                           FT_Outline*      outline )
  {
    /* copy point locations */
    إذا ( border->رقم_points )
      FT_ARRAY_COPY( outline->points + outline->n_points,
                     border->points,
                     border->رقم_points );

    /* copy tags */
    {
      FT_UInt   شمار = border->رقم_points;
      FT_Byte*  اقرأ  = border->برچسبها;
      FT_Byte*  اكتب = outline->برچسبها + outline->n_points;


      لكل ( ; شمار > 0; شمار--, اقرأ++, اكتب++ )
      {
        إذا ( *اقرأ & FT_STROKE_برچسب_ON )
          *اكتب = FT_منحنی_برچسب_ON;
        وإلا إذا ( *اقرأ & FT_STROKE_برچسب_CUBIC )
          *اكتب = FT_منحنی_برچسب_CUBIC;
        وإلا
          *اكتب = FT_منحنی_برچسب_CONIC;
      }
    }

    /* copy contours */
    {
      FT_UInt     شمار = border->رقم_points;
      FT_Byte*    برچسبها  = border->برچسبها;
      FT_UShort*  اكتب = outline->contours + outline->n_contours;
      FT_UShort   idx   = outline->n_points;


      لكل ( ; شمار > 0; شمار--, برچسبها++, idx++ )
      {
        إذا ( *برچسبها & FT_STROKE_برچسب_انتها )
        {
          *اكتب++ = idx;
          outline->n_contours++;
        }
      }
    }

    outline->n_points += (FT_UShort)border->رقم_points;

    FT_ASSERT( FT_Outline_Check( outline ) == 0 );
  }


  /*************************************************************************/
  /*************************************************************************/
  /*****                                                               *****/
  /*****                           STROKER                             *****/
  /*****                                                               *****/
  /*************************************************************************/
  /*************************************************************************/

#تعریف_کن FT_SIDE_مقصد_ROTATE( s )   ( FT_ANGLE_PI2 - (s) * FT_ANGLE_PI )

  تعريفنوع بنية  FT_StrokerRec_
  {
    FT_Angle             angle_درون;             /* direction into curr join */
    FT_Angle             angle_بیرون;            /* direction out of join  */
    FT_وکتور            center;               /* current position */
    FT_Fixed             سطر_درازی;          /* length of last lineto */
    FT_Bool              اولی_نقطه;          /* is this the start? */
    FT_Bool              subpath_open;         /* is the subpath open? */
    FT_Angle             subpath_angle;        /* subpath start direction */
    FT_وکتور            subpath_شروع;        /* subpath start position */
    FT_Fixed             subpath_سطر_درازی;  /* subpath start lineto len */
    FT_Bool              handle_wide_strokes;  /* use wide strokes logic? */

    FT_Stroker_سطر_Cap   سطر_cap;
    FT_Stroker_سطر_Join  سطر_join;
    FT_Stroker_سطر_Join  سطر_join_saved;
    FT_Fixed             miter_حد;
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
    FT_حافظه   حافظه;
    FT_Stroker  stroker = عدم;


    إذا ( !library )
      أرجع FT_THROW( Invalid_Library_Handle );

    إذا ( !astroker )
      أرجع FT_THROW( Invalid_آرگومان );

    حافظه = library->حافظه;

    إذا ( !FT_NEW( stroker ) )
    {
      stroker->library = library;

      ft_stroke_border_init( &stroker->borders[0], حافظه );
      ft_stroke_border_init( &stroker->borders[1], حافظه );
    }

    *astroker = stroker;

    أرجع error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( فراغ )
  FT_Stroker_مجموعه( FT_Stroker           stroker,
                  FT_Fixed             radius,
                  FT_Stroker_سطر_Cap   سطر_cap,
                  FT_Stroker_سطر_Join  سطر_join,
                  FT_Fixed             miter_حد )
  {
    إذا ( !stroker )
      أرجع;

    stroker->radius      = radius;
    stroker->سطر_cap    = سطر_cap;
    stroker->سطر_join   = سطر_join;
    stroker->miter_حد = miter_حد;

    /* ensure miter limit has sensible value */
    إذا ( stroker->miter_حد < 0x10000L )
      stroker->miter_حد = 0x10000L;

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
  FT_Stroker_انجامشده( FT_Stroker  stroker )
  {
    إذا ( stroker )
    {
      FT_حافظه  حافظه = stroker->library->حافظه;


      ft_stroke_border_انجامشده( &stroker->borders[0] );
      ft_stroke_border_انجامشده( &stroker->borders[1] );

      stroker->library = عدم;
      FT_FREE( stroker );
    }
  }


  /* create a circular arc at a corner or cap */
  ساكن FT_Error
  ft_stroker_arcto( FT_Stroker  stroker,
                    FT_Int      side )
  {
    FT_Angle         مجموع, rotate;
    FT_Fixed         radius = stroker->radius;
    FT_Error         error  = FT_Err_Ok;
    FT_StrokeBorder  border = stroker->borders + side;


    rotate = FT_SIDE_مقصد_ROTATE( side );

    مجموع = FT_Angle_Diff( stroker->angle_درون, stroker->angle_بیرون );
    إذا ( مجموع == FT_ANGLE_PI )
      مجموع = -rotate * 2;

    error = ft_stroke_border_arcto( border,
                                    &stroker->center,
                                    radius,
                                    stroker->angle_درون + rotate,
                                    مجموع );
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


    إذا ( stroker->سطر_cap == FT_STROKER_LINECAP_دور )
    {
      /* add a round cap */
      stroker->angle_درون  = angle;
      stroker->angle_بیرون = angle + FT_ANGLE_PI;

      error = ft_stroker_arcto( stroker, side );
    }
    وإلا
    {
      /* add a square or butt cap */
      FT_وکتور        میانه, delta;
      FT_Fixed         radius = stroker->radius;
      FT_StrokeBorder  border = stroker->borders + side;


      /* compute middle point and first angle point */
      FT_وکتور_From_Polar( &میانه, radius, angle );
      delta.x = side ?  میانه.y : -میانه.y;
      delta.y = side ? -میانه.x :  میانه.x;

      إذا ( stroker->سطر_cap == FT_STROKER_LINECAP_مربع )
      {
        میانه.x += stroker->center.x;
        میانه.y += stroker->center.y;
      }
      وإلا  /* FT_STROKER_LINECAP_BUTT */
      {
        میانه.x  = stroker->center.x;
        میانه.y  = stroker->center.y;
      }

      delta.x  += میانه.x;
      delta.y  += میانه.y;

      error = ft_stroke_border_lineto( border, &delta, FALSE );
      إذا ( error )
        اذهب Exit;

      /* compute second angle point */
      delta.x = میانه.x - delta.x + میانه.x;
      delta.y = میانه.y - delta.y + میانه.y;

      error = ft_stroke_border_lineto( border, &delta, FALSE );
    }

  Exit:
    أرجع error;
  }


  /* process an inside corner, i.e. compute intersection */
  ساكن FT_Error
  ft_stroker_inside( FT_Stroker  stroker,
                     FT_Int      side,
                     FT_Fixed    سطر_درازی )
  {
    FT_StrokeBorder  border = stroker->borders + side;
    FT_Angle         phi, theta, rotate;
    FT_Fixed         درازی;
    FT_وکتور        sigma = { 0, 0 };
    FT_وکتور        delta;
    FT_Error         error = FT_Err_Ok;
    FT_Bool          intersect;          /* use intersection of lines? */


    rotate = FT_SIDE_مقصد_ROTATE( side );

    theta = FT_Angle_Diff( stroker->angle_درون, stroker->angle_بیرون ) / 2;

    /* Only intersect borders if between two lineto's and both */
    /* lines are long enough (line_length is zero for curves). */
    /* Also avoid U-turns of nearly 180 degree.                */
    إذا ( !border->movable || سطر_درازی == 0  ||
         theta > 0x59C000 || theta < -0x59C000 )
      intersect = FALSE;
    وإلا
    {
      /* compute minimum required length of lines */
      FT_Fixed  min_درازی;


      FT_وکتور_Unit( &sigma, theta );
      min_درازی =
        ft_pos_abs( FT_MulDiv( stroker->radius, sigma.y, sigma.x ) );

      intersect = FT_BOOL( min_درازی                         &&
                           stroker->سطر_درازی >= min_درازی &&
                           سطر_درازی          >= min_درازی );
    }

    إذا ( !intersect )
    {
      FT_وکتور_From_Polar( &delta, stroker->radius,
                            stroker->angle_بیرون + rotate );
      delta.x += stroker->center.x;
      delta.y += stroker->center.y;

      border->movable = FALSE;
    }
    وإلا
    {
      /* compute median angle */
      phi = stroker->angle_درون + theta + rotate;

      درازی = FT_DivFix( stroker->radius, sigma.x );

      FT_وکتور_From_Polar( &delta, درازی, phi );
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
                      FT_Fixed    سطر_درازی )
  {
    FT_StrokeBorder  border = stroker->borders + side;
    FT_Error         error;
    FT_Angle         rotate;


    إذا ( stroker->سطر_join == FT_STROKER_LINEJOIN_دور )
      error = ft_stroker_arcto( stroker, side );
    وإلا
    {
      /* this is a mitered (pointed) or beveled (truncated) corner */
      FT_Fixed   radius = stroker->radius;
      FT_وکتور  sigma = { 0, 0 };
      FT_Angle   theta = 0, phi = 0;
      FT_Bool    bevel, fixed_bevel;


      rotate = FT_SIDE_مقصد_ROTATE( side );

      bevel =
        FT_BOOL( stroker->سطر_join == FT_STROKER_LINEJOIN_BEVEL );

      fixed_bevel =
        FT_BOOL( stroker->سطر_join != FT_STROKER_LINEJOIN_MITER_VARIABLE );

      /* check miter limit first */
      إذا ( !bevel )
      {
        theta = FT_Angle_Diff( stroker->angle_درون, stroker->angle_بیرون ) / 2;

        إذا ( theta == FT_ANGLE_PI2 )
          theta = -rotate;

        phi = stroker->angle_درون + theta + rotate;

        FT_وکتور_From_Polar( &sigma, stroker->miter_حد, theta );

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
          FT_وکتور  delta;


          /* add bevel */
          FT_وکتور_From_Polar( &delta,
                                radius,
                                stroker->angle_بیرون + rotate );
          delta.x += stroker->center.x;
          delta.y += stroker->center.y;

          border->movable = FALSE;
          error = ft_stroke_border_lineto( border, &delta, FALSE );
        }
        وإلا /* variable bevel or clipped miter */
        {
          /* the miter is truncated */
          FT_وکتور  میانه, delta;
          FT_Fixed   coef;


          /* compute middle point and first angle point */
          FT_وکتور_From_Polar( &میانه,
                                FT_MulFix( radius, stroker->miter_حد ),
                                phi );

          coef    = FT_DivFix(  0x10000L - sigma.x, sigma.y );
          delta.x = FT_MulFix(  میانه.y, coef );
          delta.y = FT_MulFix( -میانه.x, coef );

          میانه.x += stroker->center.x;
          میانه.y += stroker->center.y;
          delta.x  += میانه.x;
          delta.y  += میانه.y;

          error = ft_stroke_border_lineto( border, &delta, FALSE );
          إذا ( error )
            اذهب Exit;

          /* compute second angle point */
          delta.x = میانه.x - delta.x + میانه.x;
          delta.y = میانه.y - delta.y + میانه.y;

          error = ft_stroke_border_lineto( border, &delta, FALSE );
          إذا ( error )
            اذهب Exit;

          /* finally, add an end point; only needed if not lineto */
          /* (line_length is zero for curves)                     */
          إذا ( سطر_درازی == 0 )
          {
            FT_وکتور_From_Polar( &delta,
                                  radius,
                                  stroker->angle_بیرون + rotate );

            delta.x += stroker->center.x;
            delta.y += stroker->center.y;

            error = ft_stroke_border_lineto( border, &delta, FALSE );
          }
        }
      }
      وإلا /* this is a miter (intersection) */
      {
        FT_Fixed   درازی;
        FT_وکتور  delta;


        درازی = FT_MulDiv( stroker->radius, stroker->miter_حد, sigma.x );

        FT_وکتور_From_Polar( &delta, درازی, phi );
        delta.x += stroker->center.x;
        delta.y += stroker->center.y;

        error = ft_stroke_border_lineto( border, &delta, FALSE );
        إذا ( error )
          اذهب Exit;

        /* now add an end point; only needed if not lineto */
        /* (line_length is zero for curves)                */
        إذا ( سطر_درازی == 0 )
        {
          FT_وکتور_From_Polar( &delta,
                                stroker->radius,
                                stroker->angle_بیرون + rotate );
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
  ft_stroker_process_گوشه( FT_Stroker  stroker,
                             FT_Fixed    سطر_درازی )
  {
    FT_Error  error = FT_Err_Ok;
    FT_Angle  turn;
    FT_Int    inside_side;


    turn = FT_Angle_Diff( stroker->angle_درون, stroker->angle_بیرون );

    /* no specific corner processing is required if the turn is 0 */
    إذا ( turn == 0 )
      اذهب Exit;

    /* when we turn to the right, the inside side is 0 */
    /* otherwise, the inside side is 1 */
    inside_side = ( turn < 0 );

    /* process the inside side */
    error = ft_stroker_inside( stroker, inside_side, سطر_درازی );
    إذا ( error )
      اذهب Exit;

    /* process the outside side */
    error = ft_stroker_outside( stroker, !inside_side, سطر_درازی );

  Exit:
    أرجع error;
  }


  /* add two points to the left and right borders corresponding to the */
  /* start of the subpath                                              */
  ساكن FT_Error
  ft_stroker_subpath_شروع( FT_Stroker  stroker,
                            FT_Angle    شروع_angle,
                            FT_Fixed    سطر_درازی )
  {
    FT_وکتور        delta;
    FT_وکتور        نقطه;
    FT_Error         error;
    FT_StrokeBorder  border;


    FT_وکتور_From_Polar( &delta, stroker->radius,
                          شروع_angle + FT_ANGLE_PI2 );

    نقطه.x = stroker->center.x + delta.x;
    نقطه.y = stroker->center.y + delta.y;

    border = stroker->borders;
    error = ft_stroke_border_moveto( border, &نقطه );
    إذا ( error )
      اذهب Exit;

    نقطه.x = stroker->center.x - delta.x;
    نقطه.y = stroker->center.y - delta.y;

    border++;
    error = ft_stroke_border_moveto( border, &نقطه );

    /* save angle, position, and line length for last join */
    /* (line_length is zero for curves)                    */
    stroker->subpath_angle       = شروع_angle;
    stroker->اولی_نقطه         = FALSE;
    stroker->subpath_سطر_درازی = سطر_درازی;

  Exit:
    أرجع error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Stroker_سطر_مقصد( FT_Stroker  stroker,
                     FT_وکتور*  مقصد )
  {
    FT_Error         error = FT_Err_Ok;
    FT_StrokeBorder  border;
    FT_وکتور        delta;
    FT_Angle         angle;
    FT_Int           side;
    FT_Fixed         سطر_درازی;


    إذا ( !stroker || !مقصد )
      أرجع FT_THROW( Invalid_آرگومان );

    delta.x = مقصد->x - stroker->center.x;
    delta.y = مقصد->y - stroker->center.y;

    /* a zero-length lineto is a no-op; avoid creating a spurious corner */
    إذا ( delta.x == 0 && delta.y == 0 )
       اذهب Exit;

    /* compute length of line */
    سطر_درازی = FT_وکتور_درازی( &delta );

    angle = FT_Atan2( delta.x, delta.y );
    FT_وکتور_From_Polar( &delta, stroker->radius, angle + FT_ANGLE_PI2 );

    /* process corner if necessary */
    إذا ( stroker->اولی_نقطه )
    {
      /* This is the first segment of a subpath.  We need to     */
      /* add a point to each border at their respective starting */
      /* point locations.                                        */
      error = ft_stroker_subpath_شروع( stroker, angle, سطر_درازی );
      إذا ( error )
        اذهب Exit;
    }
    وإلا
    {
      /* process the current corner */
      stroker->angle_بیرون = angle;
      error = ft_stroker_process_گوشه( stroker, سطر_درازی );
      إذا ( error )
        اذهب Exit;
    }

    /* now add a line segment to both the `inside' and `outside' paths */
    لكل ( border = stroker->borders, side = 1; side >= 0; side--, border++ )
    {
      FT_وکتور  نقطه;


      نقطه.x = مقصد->x + delta.x;
      نقطه.y = مقصد->y + delta.y;

      /* the ends of lineto borders are movable */
      error = ft_stroke_border_lineto( border, &نقطه, TRUE );
      إذا ( error )
        اذهب Exit;

      delta.x = -delta.x;
      delta.y = -delta.y;
    }

    stroker->angle_درون    = angle;
    stroker->center      = *مقصد;
    stroker->سطر_درازی = سطر_درازی;

  Exit:
    أرجع error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Stroker_Conic_مقصد( FT_Stroker  stroker,
                      FT_وکتور*  control,
                      FT_وکتور*  مقصد )
  {
    FT_Error    error = FT_Err_Ok;
    FT_وکتور   bez_پشته[34];
    FT_وکتور*  arc;
    FT_وکتور*  حد = bez_پشته + 30;
    FT_Bool     اولی_arc = TRUE;


    إذا ( !stroker || !control || !مقصد )
    {
      error = FT_THROW( Invalid_آرگومان );
      اذهب Exit;
    }

    /* if all control points are coincident, this is a no-op; */
    /* avoid creating a spurious corner                       */
    إذا ( FT_IS_SMALL( stroker->center.x - control->x ) &&
         FT_IS_SMALL( stroker->center.y - control->y ) &&
         FT_IS_SMALL( control->x        - مقصد->x      ) &&
         FT_IS_SMALL( control->y        - مقصد->y      ) )
    {
       stroker->center = *مقصد;
       اذهب Exit;
    }

    arc    = bez_پشته;
    arc[0] = *مقصد;
    arc[1] = *control;
    arc[2] = stroker->center;

    افعل
    {
      FT_Angle  angle_درون, angle_بیرون;


      /* initialize with current direction */
      angle_درون = angle_بیرون = stroker->angle_درون;

      إذا ( arc < حد                                             &&
           !ft_conic_is_small_enough( arc, &angle_درون, &angle_بیرون ) )
      {
        إذا ( stroker->اولی_نقطه )
          stroker->angle_درون = angle_درون;

        ft_conic_تقسیم( arc );
        arc += 2;
        استمر;
      }

      إذا ( اولی_arc )
      {
        اولی_arc = FALSE;

        /* process corner if necessary */
        إذا ( stroker->اولی_نقطه )
          error = ft_stroker_subpath_شروع( stroker, angle_درون, 0 );
        وإلا
        {
          stroker->angle_بیرون = angle_درون;
          error = ft_stroker_process_گوشه( stroker, 0 );
        }
      }
      وإلا إذا ( ft_pos_abs( FT_Angle_Diff( stroker->angle_درون, angle_درون ) ) >
                  FT_SMALL_CONIC_آستانه / 4                             )
      {
        /* if the deviation from one arc to the next is too great, */
        /* add a round corner                                      */
        stroker->center    = arc[2];
        stroker->angle_بیرون = angle_درون;
        stroker->سطر_join = FT_STROKER_LINEJOIN_دور;

        error = ft_stroker_process_گوشه( stroker, 0 );

        /* reinstate line join style */
        stroker->سطر_join = stroker->سطر_join_saved;
      }

      إذا ( error )
        اذهب Exit;

      /* the arc's angle is small enough; we can add it directly to each */
      /* border                                                          */
      {
        FT_وکتور        ctrl, انتها;
        FT_Angle         theta, phi, rotate, alpha0 = 0;
        FT_Fixed         درازی;
        FT_StrokeBorder  border;
        FT_Int           side;


        theta  = FT_Angle_Diff( angle_درون, angle_بیرون ) / 2;
        phi    = angle_درون + theta;
        درازی = FT_DivFix( stroker->radius, FT_Cos( theta ) );

        /* compute direction of original arc */
        إذا ( stroker->handle_wide_strokes )
          alpha0 = FT_Atan2( arc[0].x - arc[2].x, arc[0].y - arc[2].y );

        لكل ( border = stroker->borders, side = 0;
              side <= 1;
              side++, border++ )
        {
          rotate = FT_SIDE_مقصد_ROTATE( side );

          /* compute control point */
          FT_وکتور_From_Polar( &ctrl, درازی, phi + rotate );
          ctrl.x += arc[1].x;
          ctrl.y += arc[1].y;

          /* compute end point */
          FT_وکتور_From_Polar( &انتها, stroker->radius, angle_بیرون + rotate );
          انتها.x += arc[0].x;
          انتها.y += arc[0].y;

          إذا ( stroker->handle_wide_strokes )
          {
            FT_وکتور  شروع;
            FT_Angle   alpha1;


            /* determine whether the border radius is greater than the */
            /* radius of curvature of the original arc                 */
            شروع = border->points[border->رقم_points - 1];

            alpha1 = FT_Atan2( انتها.x - شروع.x, انتها.y - شروع.y );

            /* is the direction of the border arc opposite to */
            /* that of the original arc? */
            إذا ( ft_pos_abs( FT_Angle_Diff( alpha0, alpha1 ) ) >
                   FT_ANGLE_PI / 2                             )
            {
              FT_Angle   beta, gamma;
              FT_وکتور  bvec, delta;
              FT_Fixed   blen, sinA, sinB, alen;


              /* use the sine rule to find the intersection point */
              beta  = FT_Atan2( arc[2].x - شروع.x, arc[2].y - شروع.y );
              gamma = FT_Atan2( arc[0].x - انتها.x,   arc[0].y - انتها.y );

              bvec.x = انتها.x - شروع.x;
              bvec.y = انتها.y - شروع.y;

              blen = FT_وکتور_درازی( &bvec );

              sinA = ft_pos_abs( FT_Sin( alpha1 - gamma ) );
              sinB = ft_pos_abs( FT_Sin( beta - gamma ) );

              alen = FT_MulDiv( blen, sinA, sinB );

              FT_وکتور_From_Polar( &delta, alen, beta );
              delta.x += شروع.x;
              delta.y += شروع.y;

              /* circumnavigate the negative sector backwards */
              border->movable = FALSE;
              error = ft_stroke_border_lineto( border, &delta, FALSE );
              إذا ( error )
                اذهب Exit;
              error = ft_stroke_border_lineto( border, &انتها, FALSE );
              إذا ( error )
                اذهب Exit;
              error = ft_stroke_border_conicto( border, &ctrl, &شروع );
              إذا ( error )
                اذهب Exit;
              /* and then move to the endpoint */
              error = ft_stroke_border_lineto( border, &انتها, FALSE );
              إذا ( error )
                اذهب Exit;

              استمر;
            }

            /* else fall through */
          }

          /* simply add an arc */
          error = ft_stroke_border_conicto( border, &ctrl, &انتها );
          إذا ( error )
            اذهب Exit;
        }
      }

      stroker->angle_درون = angle_بیرون;

      إذا ( arc == bez_پشته )
        اكسر;
      arc -= 2;
    } طالما ( 1 );

    stroker->center      = *مقصد;
    stroker->سطر_درازی = 0;

  Exit:
    أرجع error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Stroker_Cubic_مقصد( FT_Stroker  stroker,
                      FT_وکتور*  control1,
                      FT_وکتور*  control2,
                      FT_وکتور*  مقصد )
  {
    FT_Error    error = FT_Err_Ok;
    FT_وکتور   bez_پشته[37];
    FT_وکتور*  arc;
    FT_وکتور*  حد = bez_پشته + 32;
    FT_Bool     اولی_arc = TRUE;


    إذا ( !stroker || !control1 || !control2 || !مقصد )
    {
      error = FT_THROW( Invalid_آرگومان );
      اذهب Exit;
    }

    /* if all control points are coincident, this is a no-op; */
    /* avoid creating a spurious corner */
    إذا ( FT_IS_SMALL( stroker->center.x - control1->x ) &&
         FT_IS_SMALL( stroker->center.y - control1->y ) &&
         FT_IS_SMALL( control1->x       - control2->x ) &&
         FT_IS_SMALL( control1->y       - control2->y ) &&
         FT_IS_SMALL( control2->x       - مقصد->x       ) &&
         FT_IS_SMALL( control2->y       - مقصد->y       ) )
    {
       stroker->center = *مقصد;
       اذهب Exit;
    }

    arc    = bez_پشته;
    arc[0] = *مقصد;
    arc[1] = *control2;
    arc[2] = *control1;
    arc[3] = stroker->center;

    افعل
    {
      FT_Angle  angle_درون, angle_mid, angle_بیرون;


      /* initialize with current direction */
      angle_درون = angle_بیرون = angle_mid = stroker->angle_درون;

      إذا ( arc < حد                                         &&
           !ft_cubic_is_small_enough( arc, &angle_درون,
                                      &angle_mid, &angle_بیرون ) )
      {
        إذا ( stroker->اولی_نقطه )
          stroker->angle_درون = angle_درون;

        ft_cubic_تقسیم( arc );
        arc += 3;
        استمر;
      }

      إذا ( اولی_arc )
      {
        اولی_arc = FALSE;

        /* process corner if necessary */
        إذا ( stroker->اولی_نقطه )
          error = ft_stroker_subpath_شروع( stroker, angle_درون, 0 );
        وإلا
        {
          stroker->angle_بیرون = angle_درون;
          error = ft_stroker_process_گوشه( stroker, 0 );
        }
      }
      وإلا إذا ( ft_pos_abs( FT_Angle_Diff( stroker->angle_درون, angle_درون ) ) >
                  FT_SMALL_CUBIC_آستانه / 4                             )
      {
        /* if the deviation from one arc to the next is too great, */
        /* add a round corner                                      */
        stroker->center    = arc[3];
        stroker->angle_بیرون = angle_درون;
        stroker->سطر_join = FT_STROKER_LINEJOIN_دور;

        error = ft_stroker_process_گوشه( stroker, 0 );

        /* reinstate line join style */
        stroker->سطر_join = stroker->سطر_join_saved;
      }

      إذا ( error )
        اذهب Exit;

      /* the arc's angle is small enough; we can add it directly to each */
      /* border                                                          */
      {
        FT_وکتور        ctrl1, ctrl2, انتها;
        FT_Angle         theta1, phi1, theta2, phi2, rotate, alpha0 = 0;
        FT_Fixed         length1, length2;
        FT_StrokeBorder  border;
        FT_Int           side;


        theta1  = FT_Angle_Diff( angle_درون,  angle_mid ) / 2;
        theta2  = FT_Angle_Diff( angle_mid, angle_بیرون ) / 2;
        phi1    = ft_angle_میانگین( angle_درون,  angle_mid );
        phi2    = ft_angle_میانگین( angle_mid, angle_بیرون );
        length1 = FT_DivFix( stroker->radius, FT_Cos( theta1 ) );
        length2 = FT_DivFix( stroker->radius, FT_Cos( theta2 ) );

        /* compute direction of original arc */
        إذا ( stroker->handle_wide_strokes )
          alpha0 = FT_Atan2( arc[0].x - arc[3].x, arc[0].y - arc[3].y );

        لكل ( border = stroker->borders, side = 0;
              side <= 1;
              side++, border++ )
        {
          rotate = FT_SIDE_مقصد_ROTATE( side );

          /* compute control points */
          FT_وکتور_From_Polar( &ctrl1, length1, phi1 + rotate );
          ctrl1.x += arc[2].x;
          ctrl1.y += arc[2].y;

          FT_وکتور_From_Polar( &ctrl2, length2, phi2 + rotate );
          ctrl2.x += arc[1].x;
          ctrl2.y += arc[1].y;

          /* compute end point */
          FT_وکتور_From_Polar( &انتها, stroker->radius, angle_بیرون + rotate );
          انتها.x += arc[0].x;
          انتها.y += arc[0].y;

          إذا ( stroker->handle_wide_strokes )
          {
            FT_وکتور  شروع;
            FT_Angle   alpha1;


            /* determine whether the border radius is greater than the */
            /* radius of curvature of the original arc                 */
            شروع = border->points[border->رقم_points - 1];

            alpha1 = FT_Atan2( انتها.x - شروع.x, انتها.y - شروع.y );

            /* is the direction of the border arc opposite to */
            /* that of the original arc? */
            إذا ( ft_pos_abs( FT_Angle_Diff( alpha0, alpha1 ) ) >
                   FT_ANGLE_PI / 2                             )
            {
              FT_Angle   beta, gamma;
              FT_وکتور  bvec, delta;
              FT_Fixed   blen, sinA, sinB, alen;


              /* use the sine rule to find the intersection point */
              beta  = FT_Atan2( arc[3].x - شروع.x, arc[3].y - شروع.y );
              gamma = FT_Atan2( arc[0].x - انتها.x,   arc[0].y - انتها.y );

              bvec.x = انتها.x - شروع.x;
              bvec.y = انتها.y - شروع.y;

              blen = FT_وکتور_درازی( &bvec );

              sinA = ft_pos_abs( FT_Sin( alpha1 - gamma ) );
              sinB = ft_pos_abs( FT_Sin( beta - gamma ) );

              alen = FT_MulDiv( blen, sinA, sinB );

              FT_وکتور_From_Polar( &delta, alen, beta );
              delta.x += شروع.x;
              delta.y += شروع.y;

              /* circumnavigate the negative sector backwards */
              border->movable = FALSE;
              error = ft_stroke_border_lineto( border, &delta, FALSE );
              إذا ( error )
                اذهب Exit;
              error = ft_stroke_border_lineto( border, &انتها, FALSE );
              إذا ( error )
                اذهب Exit;
              error = ft_stroke_border_cubicto( border,
                                                &ctrl2,
                                                &ctrl1,
                                                &شروع );
              إذا ( error )
                اذهب Exit;
              /* and then move to the endpoint */
              error = ft_stroke_border_lineto( border, &انتها, FALSE );
              إذا ( error )
                اذهب Exit;

              استمر;
            }

            /* else fall through */
          }

          /* simply add an arc */
          error = ft_stroke_border_cubicto( border, &ctrl1, &ctrl2, &انتها );
          إذا ( error )
            اذهب Exit;
        }
      }

      stroker->angle_درون = angle_بیرون;

      إذا ( arc == bez_پشته )
        اكسر;
      arc -= 3;
    } طالما ( 1 );

    stroker->center      = *مقصد;
    stroker->سطر_درازی = 0;

  Exit:
    أرجع error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Stroker_Begin_Sub_مسیر( FT_Stroker  stroker,
                           FT_وکتور*  مقصد,
                           FT_Bool     open )
  {
    إذا ( !stroker || !مقصد )
      أرجع FT_THROW( Invalid_آرگومان );

    /* We cannot process the first point, because there is not enough      */
    /* information regarding its corner/cap.  The latter will be processed */
    /* in the `FT_Stroker_EndSubPath' routine.                             */
    /*                                                                     */
    stroker->اولی_نقطه  = TRUE;
    stroker->center       = *مقصد;
    stroker->subpath_open = open;

    /* Determine if we need to check whether the border radius is greater */
    /* than the radius of curvature of a curve, to handle this case       */
    /* specially.  This is only required if bevel joins or butt caps may  */
    /* be created, because round & miter joins and round & square caps    */
    /* cover the negative sector created with wide strokes.               */
    stroker->handle_wide_strokes =
      FT_BOOL( stroker->سطر_join != FT_STROKER_LINEJOIN_دور  ||
               ( stroker->subpath_open                        &&
                 stroker->سطر_cap == FT_STROKER_LINECAP_BUTT ) );

    /* record the subpath start point for each border */
    stroker->subpath_شروع = *مقصد;

    stroker->angle_درون = 0;

    أرجع FT_Err_Ok;
  }


  ساكن FT_Error
  ft_stroker_جمع_کن_معکوس_چپ( FT_Stroker  stroker,
                               FT_Bool     open )
  {
    FT_StrokeBorder  راست = stroker->borders + 0;
    FT_StrokeBorder  چپ  = stroker->borders + 1;
    FT_Int           new_points;
    FT_Error         error = FT_Err_Ok;


    FT_ASSERT( چپ->شروع >= 0 );

    new_points = (FT_Int)چپ->رقم_points - چپ->شروع;
    إذا ( new_points > 0 )
    {
      error = ft_stroke_border_grow( راست, (FT_UInt)new_points );
      إذا ( error )
        اذهب Exit;

      {
        FT_وکتور*  dst_نقطه = راست->points + راست->رقم_points;
        FT_Byte*    dst_برچسب   = راست->برچسبها   + راست->رقم_points;
        FT_وکتور*  src_نقطه = چپ->points  + چپ->رقم_points - 1;
        FT_Byte*    src_برچسب   = چپ->برچسبها    + چپ->رقم_points - 1;


        طالما ( src_نقطه >= چپ->points + چپ->شروع )
        {
          *dst_نقطه = *src_نقطه;
          *dst_برچسب   = *src_برچسب;

          إذا ( open )
            dst_برچسب[0] &= ~FT_STROKE_برچسب_BEGIN_انتها;
          وإلا
          {
            FT_Byte  ttag =
                       (FT_Byte)( dst_برچسب[0] & FT_STROKE_برچسب_BEGIN_انتها );


            /* switch begin/end tags if necessary */
            إذا ( ttag == FT_STROKE_برچسب_BEGIN ||
                 ttag == FT_STROKE_برچسب_انتها   )
              dst_برچسب[0] ^= FT_STROKE_برچسب_BEGIN_انتها;
          }

          src_نقطه--;
          src_برچسب--;
          dst_نقطه++;
          dst_برچسب++;
        }
      }

      چپ->رقم_points   = (FT_UInt)چپ->شروع;
      راست->رقم_points += (FT_UInt)new_points;

      راست->movable = FALSE;
      چپ->movable  = FALSE;
    }

  Exit:
    أرجع error;
  }


  /* documentation is in ftstroke.h */

  /* there's a lot of magic in this function! */
  FT_EXPORT_DEF( FT_Error )
  FT_Stroker_انتها_Sub_مسیر( FT_Stroker  stroker )
  {
    FT_Error  error = FT_Err_Ok;


    إذا ( !stroker )
    {
      error = FT_THROW( Invalid_آرگومان );
      اذهب Exit;
    }

    /* don't try to end the path if no segments have been generated */
    إذا ( stroker->اولی_نقطه )
      اذهب Exit;

    إذا ( stroker->subpath_open )
    {
      FT_StrokeBorder  راست = stroker->borders;


      /* All right, this is an opened path, we need to add a cap between */
      /* right & left, add the reverse of left, then add a final cap     */
      /* between left & right.                                           */
      error = ft_stroker_cap( stroker, stroker->angle_درون, 0 );
      إذا ( error )
        اذهب Exit;

      /* add reversed points from `left' to `right' */
      error = ft_stroker_جمع_کن_معکوس_چپ( stroker, TRUE );
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
      ft_stroke_border_close( راست, FALSE );
    }
    وإلا
    {
      /* close the path if needed */
      إذا ( !FT_IS_SMALL( stroker->center.x - stroker->subpath_شروع.x ) ||
           !FT_IS_SMALL( stroker->center.y - stroker->subpath_شروع.y ) )
      {
         error = FT_Stroker_سطر_مقصد( stroker, &stroker->subpath_شروع );
         إذا ( error )
           اذهب Exit;
      }

      /* process the corner */
      stroker->angle_بیرون = stroker->subpath_angle;

      error = ft_stroker_process_گوشه( stroker,
                                         stroker->subpath_سطر_درازی );
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
    FT_UInt   رقم_points = 0, رقم_contours = 0;
    FT_Error  error;


    إذا ( !stroker || border > 1 )
    {
      error = FT_THROW( Invalid_آرگومان );
      اذهب Exit;
    }

    error = ft_stroke_border_get_counts( stroker->borders + border,
                                         &رقم_points, &رقم_contours );
  Exit:
    إذا ( anum_points )
      *anum_points = رقم_points;

    إذا ( anum_contours )
      *anum_contours = رقم_contours;

    أرجع error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Stroker_GetCounts( FT_Stroker  stroker,
                        FT_UInt    *anum_points,
                        FT_UInt    *anum_contours )
  {
    FT_UInt   count1, count2, رقم_points   = 0;
    FT_UInt   count3, count4, رقم_contours = 0;
    FT_Error  error;


    إذا ( !stroker )
    {
      error = FT_THROW( Invalid_آرگومان );
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

    رقم_points   = count1 + count3;
    رقم_contours = count2 + count4;

  Exit:
    إذا ( anum_points )
      *anum_points   = رقم_points;

    إذا ( anum_contours )
      *anum_contours = رقم_contours;

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

    إذا ( border == FT_STROKER_BORDER_چپ  ||
         border == FT_STROKER_BORDER_راست )
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
    FT_Stroker_ExportBorder( stroker, FT_STROKER_BORDER_چپ, outline );
    FT_Stroker_ExportBorder( stroker, FT_STROKER_BORDER_راست, outline );
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
    FT_وکتور   v_آخرین;
    FT_وکتور   v_control;
    FT_وکتور   v_شروع;

    FT_وکتور*  نقطه;
    FT_وکتور*  حد;
    FT_Byte*    برچسبها;

    FT_Error    error;

    FT_Int      n;         /* index of contour in outline     */
    FT_Int      اولی;     /* index of first point in contour */
    FT_Int      آخرین;      /* index of last point in contour  */

    FT_Int      برچسب;       /* current point's state           */


    إذا ( !outline )
      أرجع FT_THROW( Invalid_Outline );

    إذا ( !stroker )
      أرجع FT_THROW( Invalid_آرگومان );

    FT_Stroker_Rewind( stroker );

    آخرین = -1;
    لكل ( n = 0; n < outline->n_contours; n++ )
    {
      اولی = آخرین + 1;
      آخرین  = outline->contours[n];

      /* skip empty points; we don't stroke these */
      إذا ( آخرین <= اولی )
        استمر;

      حد = outline->points + آخرین;

      v_شروع = outline->points[اولی];
      v_آخرین  = outline->points[آخرین];

      v_control = v_شروع;

      نقطه = outline->points + اولی;
      برچسبها  = outline->برچسبها   + اولی;
      برچسب   = FT_منحنی_برچسب( برچسبها[0] );

      /* check first point to determine origin */
      إذا ( برچسب != FT_منحنی_برچسب_ON )
      {
        FT_Int  آخرین_برچسب = FT_منحنی_برچسب( outline->برچسبها[آخرین] );


        /* First point is a control point.  Yes, this happens. */
        إذا ( آخرین_برچسب == FT_منحنی_برچسب_ON )
        {
          /* start at last point if it is on the curve */
          v_شروع = v_آخرین;
          حد--;
        }
        وإلا
        {
          إذا ( آخرین_برچسب != برچسب )
            اذهب Invalid_Outline;

          /* if both first and last points are controls, */
          /* start at their middle                       */
          v_شروع.x = ( v_شروع.x + v_آخرین.x ) / 2;
          v_شروع.y = ( v_شروع.y + v_آخرین.y ) / 2;
        }
        نقطه--;
        برچسبها--;
      }

      error = FT_Stroker_Begin_Sub_مسیر( stroker, &v_شروع, opened );
      إذا ( error )
        اذهب Exit;

      طالما ( نقطه < حد )
      {
        نقطه++;
        برچسبها++;

        برچسب = FT_منحنی_برچسب( برچسبها[0] );
        بدل ( برچسب )
        {
        حالة FT_منحنی_برچسب_ON:  /* emit a single line_to */
          {
            FT_وکتور  vec;


            vec.x = نقطه->x;
            vec.y = نقطه->y;

            error = FT_Stroker_سطر_مقصد( stroker, &vec );
            إذا ( error )
              اذهب Exit;
            استمر;
          }

        حالة FT_منحنی_برچسب_CONIC:  /* consume conic arcs */
          v_control.x = نقطه->x;
          v_control.y = نقطه->y;

        Do_Conic:
          إذا ( نقطه < حد )
          {
            FT_وکتور  vec;
            FT_وکتور  v_میانه;


            نقطه++;
            برچسبها++;
            برچسب = FT_منحنی_برچسب( برچسبها[0] );

            vec = نقطه[0];

            إذا ( برچسب == FT_منحنی_برچسب_ON )
            {
              error = FT_Stroker_Conic_مقصد( stroker, &v_control, &vec );
              إذا ( error )
                اذهب Exit;
              استمر;
            }

            إذا ( برچسب != FT_منحنی_برچسب_CONIC )
              اذهب Invalid_Outline;

            v_میانه.x = ( v_control.x + vec.x ) / 2;
            v_میانه.y = ( v_control.y + vec.y ) / 2;

            error = FT_Stroker_Conic_مقصد( stroker, &v_control, &v_میانه );
            إذا ( error )
              اذهب Exit;

            v_control = vec;
            اذهب Do_Conic;
          }

          error = FT_Stroker_Conic_مقصد( stroker, &v_control, &v_شروع );
          اذهب Close;

        افتراضي:  /* FT_CURVE_TAG_CUBIC */
          {
            FT_وکتور  vec1, vec2;


            إذا ( نقطه + 1 > حد                             ||
                 FT_منحنی_برچسب( برچسبها[1] ) != FT_منحنی_برچسب_CUBIC )
              اذهب Invalid_Outline;

            نقطه += 2;
            برچسبها  += 2;

            vec1 = نقطه[-2];
            vec2 = نقطه[-1];

            إذا ( نقطه <= حد )
            {
              FT_وکتور  vec;
              FT_وکتور  v_میانه;


              برچسب = FT_منحنی_برچسب( برچسبها[0] );
              vec = نقطه[0];

              إذا ( برچسب == FT_منحنی_برچسب_CUBIC )
              {
                v_میانه.x = ( vec2.x + vec.x ) / 2;
                v_میانه.y = ( vec2.y + vec.y ) / 2;

                vec = v_میانه;
              }
              وإلا إذا ( برچسب != FT_منحنی_برچسب_ON )
                اذهب Invalid_Outline;

              error = FT_Stroker_Cubic_مقصد( stroker, &vec1, &vec2, &vec );
              إذا ( error )
                اذهب Exit;

              إذا ( برچسب == FT_منحنی_برچسب_CUBIC )
              {
                نقطه--;
                برچسبها--;
              }
              استمر;
            }

            error = FT_Stroker_Cubic_مقصد( stroker, &vec1, &vec2, &v_شروع );
            اذهب Close;
          }
        }
      }

    Close:
      إذا ( error )
        اذهب Exit;

      error = FT_Stroker_انتها_Sub_مسیر( stroker );
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
    FT_Error  error = FT_THROW( Invalid_آرگومان );
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
      FT_UInt          رقم_points, رقم_contours;


      error = FT_Stroker_ParseOutline( stroker, outline, FALSE );
      إذا ( error )
        اذهب Fail;

      error = FT_Stroker_GetCounts( stroker, &رقم_points, &رقم_contours );
      إذا ( error )
        اذهب Fail;

      FT_Outline_انجامشده( glyph->library, outline );

      error = FT_Outline_New( glyph->library,
                              رقم_points,
                              (FT_Int)رقم_contours,
                              outline );
      إذا ( error )
        اذهب Fail;

      outline->n_points   = 0;
      outline->n_contours = 0;

      FT_Stroker_Export( stroker, outline );
    }

    إذا ( destroy )
      FT_انجامشده_Glyph( *pglyph );

    *pglyph = glyph;
    اذهب Exit;

  Fail:
    FT_انجامشده_Glyph( glyph );
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
    FT_Error  error = FT_THROW( Invalid_آرگومان );
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
      FT_UInt           رقم_points, رقم_contours;


      border = FT_Outline_GetOutsideBorder( outline );
      إذا ( inside )
      {
        إذا ( border == FT_STROKER_BORDER_چپ )
          border = FT_STROKER_BORDER_راست;
        وإلا
          border = FT_STROKER_BORDER_چپ;
      }

      error = FT_Stroker_ParseOutline( stroker, outline, FALSE );
      إذا ( error )
        اذهب Fail;

      FT_Stroker_GetBorderCounts( stroker, border,
                                  &رقم_points, &رقم_contours );

      FT_Outline_انجامشده( glyph->library, outline );

      error = FT_Outline_New( glyph->library,
                              رقم_points,
                              (FT_Int)رقم_contours,
                              outline );
      إذا ( error )
        اذهب Fail;

      outline->n_points   = 0;
      outline->n_contours = 0;

      FT_Stroker_ExportBorder( stroker, border, outline );
    }

    إذا ( destroy )
      FT_انجامشده_Glyph( *pglyph );

    *pglyph = glyph;
    اذهب Exit;

  Fail:
    FT_انجامشده_Glyph( glyph );
    glyph = عدم;

    إذا ( !destroy )
      *pglyph = عدم;

  Exit:
    أرجع error;
  }


/*انتها*/
