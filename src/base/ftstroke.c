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


    أرجع o == FT_ORIENTATION_TRUETYPE ? FT_STROKER_BORDER_ښی
                                        : FT_STROKER_BORDER_کیڼ;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_StrokerBorder )
  FT_Outline_GetOutsideBorder( FT_Outline*  outline )
  {
    FT_Orientation  o = FT_Outline_Get_Orientation( outline );


    أرجع o == FT_ORIENTATION_TRUETYPE ? FT_STROKER_BORDER_کیڼ
                                        : FT_STROKER_BORDER_ښی;
  }


  /*************************************************************************/
  /*************************************************************************/
  /*****                                                               *****/
  /*****                      BEZIER COMPUTATIONS                      *****/
  /*****                                                               *****/
  /*************************************************************************/
  /*************************************************************************/

#تعریف_کړه FT_SMALL_CONIC_پوله  ( FT_ANGLE_PI / 6 )
#تعریف_کړه FT_SMALL_CUBIC_پوله  ( FT_ANGLE_PI / 8 )

#تعریف_کړه FT_EPSILON  2

#تعریف_کړه FT_IS_SMALL( x )  ( (x) > -FT_EPSILON && (x) < FT_EPSILON )


  ساكن FT_Pos
  ft_pos_abs( FT_Pos  x )
  {
    أرجع x >= 0 ? x : -x;
  }


  ساكن فراغ
  ft_conic_وېشل( FT_ویکټور*  بنسټ )
  {
    FT_Pos  a, b;


    بنسټ[4].x = بنسټ[2].x;
    a = بنسټ[0].x + بنسټ[1].x;
    b = بنسټ[1].x + بنسټ[2].x;
    بنسټ[3].x = b >> 1;
    بنسټ[2].x = ( a + b ) >> 2;
    بنسټ[1].x = a >> 1;

    بنسټ[4].y = بنسټ[2].y;
    a = بنسټ[0].y + بنسټ[1].y;
    b = بنسټ[1].y + بنسټ[2].y;
    بنسټ[3].y = b >> 1;
    بنسټ[2].y = ( a + b ) >> 2;
    بنسټ[1].y = a >> 1;
  }


  ساكن FT_Bool
  ft_conic_is_small_enough( FT_ویکټور*  بنسټ,
                            FT_Angle   *angle_دننه,
                            FT_Angle   *angle_بهر )
  {
    FT_ویکټور  d1, d2;
    FT_Angle   theta;
    FT_Int     close1, close2;


    d1.x = بنسټ[1].x - بنسټ[2].x;
    d1.y = بنسټ[1].y - بنسټ[2].y;
    d2.x = بنسټ[0].x - بنسټ[1].x;
    d2.y = بنسټ[0].y - بنسټ[1].y;

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
        *angle_دننه  =
        *angle_بهر = FT_Atan2( d2.x, d2.y );
      }
    }
    وإلا /* !close1 */
    {
      إذا ( close2 )
      {
        *angle_دننه  =
        *angle_بهر = FT_Atan2( d1.x, d1.y );
      }
      وإلا
      {
        *angle_دننه  = FT_Atan2( d1.x, d1.y );
        *angle_بهر = FT_Atan2( d2.x, d2.y );
      }
    }

    theta = ft_pos_abs( FT_Angle_Diff( *angle_دننه, *angle_بهر ) );

    أرجع FT_BOOL( theta < FT_SMALL_CONIC_پوله );
  }


  ساكن فراغ
  ft_cubic_وېشل( FT_ویکټور*  بنسټ )
  {
    FT_Pos  a, b, c;


    بنسټ[6].x = بنسټ[3].x;
    a = بنسټ[0].x + بنسټ[1].x;
    b = بنسټ[1].x + بنسټ[2].x;
    c = بنسټ[2].x + بنسټ[3].x;
    بنسټ[5].x = c >> 1;
    c += b;
    بنسټ[4].x = c >> 2;
    بنسټ[1].x = a >> 1;
    a += b;
    بنسټ[2].x = a >> 2;
    بنسټ[3].x = ( a + c ) >> 3;

    بنسټ[6].y = بنسټ[3].y;
    a = بنسټ[0].y + بنسټ[1].y;
    b = بنسټ[1].y + بنسټ[2].y;
    c = بنسټ[2].y + بنسټ[3].y;
    بنسټ[5].y = c >> 1;
    c += b;
    بنسټ[4].y = c >> 2;
    بنسټ[1].y = a >> 1;
    a += b;
    بنسټ[2].y = a >> 2;
    بنسټ[3].y = ( a + c ) >> 3;
  }


  /* Return the average of `angle1' and `angle2'.            */
  /* This gives correct result even if `angle1' and `angle2' */
  /* have opposite signs.                                    */
  ساكن FT_Angle
  ft_angle_منځنۍ( FT_Angle  angle1,
                 FT_Angle  angle2 )
  {
    أرجع angle1 + FT_Angle_Diff( angle1, angle2 ) / 2;
  }


  ساكن FT_Bool
  ft_cubic_is_small_enough( FT_ویکټور*  بنسټ,
                            FT_Angle   *angle_دننه,
                            FT_Angle   *angle_mid,
                            FT_Angle   *angle_بهر )
  {
    FT_ویکټور  d1, d2, d3;
    FT_Angle   theta1, theta2;
    FT_Int     close1, close2, close3;


    d1.x = بنسټ[2].x - بنسټ[3].x;
    d1.y = بنسټ[2].y - بنسټ[3].y;
    d2.x = بنسټ[1].x - بنسټ[2].x;
    d2.y = بنسټ[1].y - بنسټ[2].y;
    d3.x = بنسټ[0].x - بنسټ[1].x;
    d3.y = بنسټ[0].y - بنسټ[1].y;

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
          *angle_دننه  =
          *angle_mid =
          *angle_بهر = FT_Atan2( d3.x, d3.y );
        }
      }
      وإلا /* !close2 */
      {
        إذا ( close3 )
        {
          *angle_دننه  =
          *angle_mid =
          *angle_بهر = FT_Atan2( d2.x, d2.y );
        }
        وإلا /* !close3 */
        {
          *angle_دننه  =
          *angle_mid = FT_Atan2( d2.x, d2.y );
          *angle_بهر = FT_Atan2( d3.x, d3.y );
        }
      }
    }
    وإلا /* !close1 */
    {
      إذا ( close2 )
      {
        إذا ( close3 )
        {
          *angle_دننه  =
          *angle_mid =
          *angle_بهر = FT_Atan2( d1.x, d1.y );
        }
        وإلا /* !close3 */
        {
          *angle_دننه  = FT_Atan2( d1.x, d1.y );
          *angle_بهر = FT_Atan2( d3.x, d3.y );
          *angle_mid = ft_angle_منځنۍ( *angle_دننه, *angle_بهر );
        }
      }
      وإلا /* !close2 */
      {
        إذا ( close3 )
        {
          *angle_دننه  = FT_Atan2( d1.x, d1.y );
          *angle_mid =
          *angle_بهر = FT_Atan2( d2.x, d2.y );
        }
        وإلا /* !close3 */
        {
          *angle_دننه  = FT_Atan2( d1.x, d1.y );
          *angle_mid = FT_Atan2( d2.x, d2.y );
          *angle_بهر = FT_Atan2( d3.x, d3.y );
        }
      }
    }

    theta1 = ft_pos_abs( FT_Angle_Diff( *angle_دننه,  *angle_mid ) );
    theta2 = ft_pos_abs( FT_Angle_Diff( *angle_mid, *angle_بهر ) );

    أرجع FT_BOOL( theta1 < FT_SMALL_CUBIC_پوله &&
                    theta2 < FT_SMALL_CUBIC_پوله );
  }


  /*************************************************************************/
  /*************************************************************************/
  /*****                                                               *****/
  /*****                       STROKE BORDERS                          *****/
  /*****                                                               *****/
  /*************************************************************************/
  /*************************************************************************/

  تعريفنوع تعداد  FT_Stroke_نښانونه
  {
    FT_STROKE_نښان_ON    = 1,   /* on-curve point  */
    FT_STROKE_نښان_CUBIC = 2,   /* cubic off-point */
    FT_STROKE_نښان_BEGIN = 4,   /* sub-path start  */
    FT_STROKE_نښان_پای_ټکی   = 8    /* sub-path end    */

  } FT_Stroke_نښانونه_2;

#تعریف_کړه  FT_STROKE_نښان_BEGIN_پای_ټکی  ( FT_STROKE_نښان_BEGIN | FT_STROKE_نښان_پای_ټکی )

  تعريفنوع بنية  FT_StrokeBorderRec_
  {
    FT_UInt     عدد_points;
    FT_UInt     max_points;
    FT_ویکټور*  points;
    FT_Byte*    نښانونه;
    FT_Bool     movable;  /* TRUE for ends of lineto borders */
    FT_Int      پیل;    /* index of current sub-path start point */
    FT_حافظه   حافظه;
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
      FT_حافظه  حافظه  = border->حافظه;


      طالما ( cur_max < new_max )
        cur_max += ( cur_max >> 1 ) + 16;

      إذا ( FT_RENEW_ARRAY( border->points, old_max, cur_max ) ||
           FT_RENEW_ARRAY( border->نښانونه,   old_max, cur_max ) )
        اذهب Exit;

      border->max_points = cur_max;
    }

  Exit:
    أرجع error;
  }


  ساكن فراغ
  ft_stroke_border_close( FT_StrokeBorder  border,
                          FT_Bool          سرچپه )
  {
    FT_UInt  پیل = (FT_UInt)border->پیل;
    FT_UInt  شمېر = border->عدد_points;


    FT_ASSERT( border->پیل >= 0 );

    /* don't record empty paths! */
    إذا ( شمېر <= پیل + 1U )
      border->عدد_points = پیل;
    وإلا
    {
      /* copy the last point to the start of this sub-path, since */
      /* it contains the `adjusted' starting coordinates          */
      border->عدد_points    = --شمېر;
      border->points[پیل] = border->points[شمېر];
      border->نښانونه[پیل]   = border->نښانونه[شمېر];

      إذا ( سرچپه )
      {
        /* reverse the points */
        {
          FT_ویکټور*  vec1 = border->points + پیل + 1;
          FT_ویکټور*  vec2 = border->points + شمېر - 1;


          لكل ( ; vec1 < vec2; vec1++, vec2-- )
          {
            FT_ویکټور  tmp;


            tmp   = *vec1;
            *vec1 = *vec2;
            *vec2 = tmp;
          }
        }

        /* then the tags */
        {
          FT_Byte*  tag1 = border->نښانونه + پیل + 1;
          FT_Byte*  tag2 = border->نښانونه + شمېر - 1;


          لكل ( ; tag1 < tag2; tag1++, tag2-- )
          {
            FT_Byte  tmp;


            tmp   = *tag1;
            *tag1 = *tag2;
            *tag2 = tmp;
          }
        }
      }

      border->نښانونه[پیل    ] |= FT_STROKE_نښان_BEGIN;
      border->نښانونه[شمېر - 1] |= FT_STROKE_نښان_پای_ټکی;
    }

    border->پیل   = -1;
    border->movable = FALSE;
  }


  ساكن FT_Error
  ft_stroke_border_lineto( FT_StrokeBorder  border,
                           FT_ویکټور*       منزل,
                           FT_Bool          movable )
  {
    FT_Error  error = FT_Err_Ok;


    FT_ASSERT( border->پیل >= 0 );

    إذا ( border->movable )
    {
      /* move last point */
      border->points[border->عدد_points - 1] = *منزل;
    }
    وإلا
    {
      /* don't add zero-length lineto, but always add moveto */
      إذا ( border->عدد_points > (FT_UInt)border->پیل                     &&
           FT_IS_SMALL( border->points[border->عدد_points - 1].x - منزل->x ) &&
           FT_IS_SMALL( border->points[border->عدد_points - 1].y - منزل->y ) )
        أرجع error;

      /* add one point */
      error = ft_stroke_border_grow( border, 1 );
      إذا ( !error )
      {
        FT_ویکټور*  vec = border->points + border->عدد_points;
        FT_Byte*    نښان = border->نښانونه   + border->عدد_points;


        vec[0] = *منزل;
        نښان[0] = FT_STROKE_نښان_ON;

        border->عدد_points += 1;
      }
    }
    border->movable = movable;
    أرجع error;
  }


  ساكن FT_Error
  ft_stroke_border_conicto( FT_StrokeBorder  border,
                            FT_ویکټور*       control,
                            FT_ویکټور*       منزل )
  {
    FT_Error  error;


    FT_ASSERT( border->پیل >= 0 );

    error = ft_stroke_border_grow( border, 2 );
    إذا ( !error )
    {
      FT_ویکټور*  vec = border->points + border->عدد_points;
      FT_Byte*    نښان = border->نښانونه   + border->عدد_points;


      vec[0] = *control;
      vec[1] = *منزل;

      نښان[0] = 0;
      نښان[1] = FT_STROKE_نښان_ON;

      border->عدد_points += 2;
    }

    border->movable = FALSE;

    أرجع error;
  }


  ساكن FT_Error
  ft_stroke_border_cubicto( FT_StrokeBorder  border,
                            FT_ویکټور*       control1,
                            FT_ویکټور*       control2,
                            FT_ویکټور*       منزل )
  {
    FT_Error  error;


    FT_ASSERT( border->پیل >= 0 );

    error = ft_stroke_border_grow( border, 3 );
    إذا ( !error )
    {
      FT_ویکټور*  vec = border->points + border->عدد_points;
      FT_Byte*    نښان = border->نښانونه   + border->عدد_points;


      vec[0] = *control1;
      vec[1] = *control2;
      vec[2] = *منزل;

      نښان[0] = FT_STROKE_نښان_CUBIC;
      نښان[1] = FT_STROKE_نښان_CUBIC;
      نښان[2] = FT_STROKE_نښان_ON;

      border->عدد_points += 3;
    }

    border->movable = FALSE;

    أرجع error;
  }


#تعریف_کړه FT_ARC_CUBIC_ANGLE  ( FT_ANGLE_PI / 2 )


  ساكن FT_Error
  ft_stroke_border_arcto( FT_StrokeBorder  border,
                          FT_ویکټور*       center,
                          FT_Fixed         radius,
                          FT_Angle         angle_پیل,
                          FT_Angle         angle_diff )
  {
    FT_Fixed   coef;
    FT_ویکټور  a0, a1, a2, a3;
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
    FT_ویکټور_From_Polar( &a0, radius, angle_پیل );
    a1.x = FT_MulFix( -a0.y, coef );
    a1.y = FT_MulFix(  a0.x, coef );

    a0.x += center->x;
    a0.y += center->y;
    a1.x += a0.x;
    a1.y += a0.y;

    لكل ( i = 1; i <= arcs; i++ )
    {
      /* compute end and second control point */
      FT_ویکټور_From_Polar( &a3, radius,
                            angle_پیل + i * angle_diff / arcs );
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
                           FT_ویکټور*       منزل )
  {
    /* close current open path if any ? */
    إذا ( border->پیل >= 0 )
      ft_stroke_border_close( border, FALSE );

    border->پیل = (FT_Int)border->عدد_points;
    border->movable = FALSE;

    أرجع ft_stroke_border_lineto( border, منزل, FALSE );
  }


  ساكن فراغ
  ft_stroke_border_init( FT_StrokeBorder  border,
                         FT_حافظه        حافظه )
  {
    border->حافظه = حافظه;
    border->points = عدم;
    border->نښانونه   = عدم;

    border->عدد_points = 0;
    border->max_points = 0;
    border->پیل      = -1;
    border->valid      = FALSE;
  }


  ساكن فراغ
  ft_stroke_border_reset( FT_StrokeBorder  border )
  {
    border->عدد_points = 0;
    border->پیل      = -1;
    border->valid      = FALSE;
  }


  ساكن فراغ
  ft_stroke_border_بشپړ( FT_StrokeBorder  border )
  {
    FT_حافظه  حافظه = border->حافظه;


    FT_FREE( border->points );
    FT_FREE( border->نښانونه );

    border->عدد_points = 0;
    border->max_points = 0;
    border->پیل      = -1;
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

    FT_UInt     شمېر      = border->عدد_points;
    FT_Byte*    نښانونه       = border->نښانونه;
    FT_Int      دننه_contour = 0;


    لكل ( ; شمېر > 0; شمېر--, عدد_points++, نښانونه++ )
    {
      إذا ( نښانونه[0] & FT_STROKE_نښان_BEGIN )
      {
        إذا ( دننه_contour != 0 )
          اذهب Fail;

        دننه_contour = 1;
      }
      وإلا إذا ( دننه_contour == 0 )
        اذهب Fail;

      إذا ( نښانونه[0] & FT_STROKE_نښان_پای_ټکی )
      {
        دننه_contour = 0;
        عدد_contours++;
      }
    }

    إذا ( دننه_contour != 0 )
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
      FT_UInt   شمېر = border->عدد_points;
      FT_Byte*  اقرأ  = border->نښانونه;
      FT_Byte*  اكتب = outline->نښانونه + outline->n_points;


      لكل ( ; شمېر > 0; شمېر--, اقرأ++, اكتب++ )
      {
        إذا ( *اقرأ & FT_STROKE_نښان_ON )
          *اكتب = FT_منحني_نښان_ON;
        وإلا إذا ( *اقرأ & FT_STROKE_نښان_CUBIC )
          *اكتب = FT_منحني_نښان_CUBIC;
        وإلا
          *اكتب = FT_منحني_نښان_CONIC;
      }
    }

    /* copy contours */
    {
      FT_UInt     شمېر = border->عدد_points;
      FT_Byte*    نښانونه  = border->نښانونه;
      FT_UShort*  اكتب = outline->contours + outline->n_contours;
      FT_UShort   idx   = outline->n_points;


      لكل ( ; شمېر > 0; شمېر--, نښانونه++, idx++ )
      {
        إذا ( *نښانونه & FT_STROKE_نښان_پای_ټکی )
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

#تعریف_کړه FT_SIDE_منزل_ROTATE( s )   ( FT_ANGLE_PI2 - (s) * FT_ANGLE_PI )

  تعريفنوع بنية  FT_StrokerRec_
  {
    FT_Angle             angle_دننه;             /* direction into curr join */
    FT_Angle             angle_بهر;            /* direction out of join  */
    FT_ویکټور            center;               /* current position */
    FT_Fixed             کرښه_اوږدتیا;          /* length of last lineto */
    FT_Bool              لومړی_ټکی;          /* is this the start? */
    FT_Bool              subpath_open;         /* is the subpath open? */
    FT_Angle             subpath_angle;        /* subpath start direction */
    FT_ویکټور            subpath_پیل;        /* subpath start position */
    FT_Fixed             subpath_کرښه_اوږدتیا;  /* subpath start lineto len */
    FT_Bool              handle_wide_strokes;  /* use wide strokes logic? */

    FT_Stroker_کرښه_Cap   کرښه_cap;
    FT_Stroker_کرښه_Join  کرښه_join;
    FT_Stroker_کرښه_Join  کرښه_join_saved;
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
      أرجع FT_THROW( Invalid_دلیل );

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
  FT_Stroker_سېټ( FT_Stroker           stroker,
                  FT_Fixed             radius,
                  FT_Stroker_کرښه_Cap   کرښه_cap,
                  FT_Stroker_کرښه_Join  کرښه_join,
                  FT_Fixed             miter_حد )
  {
    إذا ( !stroker )
      أرجع;

    stroker->radius      = radius;
    stroker->کرښه_cap    = کرښه_cap;
    stroker->کرښه_join   = کرښه_join;
    stroker->miter_حد = miter_حد;

    /* ensure miter limit has sensible value */
    إذا ( stroker->miter_حد < 0x10000L )
      stroker->miter_حد = 0x10000L;

    /* save line join style:                                           */
    /* line join style can be temporarily changed when stroking curves */
    stroker->کرښه_join_saved = کرښه_join;

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
  FT_Stroker_بشپړ( FT_Stroker  stroker )
  {
    إذا ( stroker )
    {
      FT_حافظه  حافظه = stroker->library->حافظه;


      ft_stroke_border_بشپړ( &stroker->borders[0] );
      ft_stroke_border_بشپړ( &stroker->borders[1] );

      stroker->library = عدم;
      FT_FREE( stroker );
    }
  }


  /* create a circular arc at a corner or cap */
  ساكن FT_Error
  ft_stroker_arcto( FT_Stroker  stroker,
                    FT_Int      side )
  {
    FT_Angle         ټول, rotate;
    FT_Fixed         radius = stroker->radius;
    FT_Error         error  = FT_Err_Ok;
    FT_StrokeBorder  border = stroker->borders + side;


    rotate = FT_SIDE_منزل_ROTATE( side );

    ټول = FT_Angle_Diff( stroker->angle_دننه, stroker->angle_بهر );
    إذا ( ټول == FT_ANGLE_PI )
      ټول = -rotate * 2;

    error = ft_stroke_border_arcto( border,
                                    &stroker->center,
                                    radius,
                                    stroker->angle_دننه + rotate,
                                    ټول );
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


    إذا ( stroker->کرښه_cap == FT_STROKER_LINECAP_پړاو )
    {
      /* add a round cap */
      stroker->angle_دننه  = angle;
      stroker->angle_بهر = angle + FT_ANGLE_PI;

      error = ft_stroker_arcto( stroker, side );
    }
    وإلا
    {
      /* add a square or butt cap */
      FT_ویکټور        منځ, delta;
      FT_Fixed         radius = stroker->radius;
      FT_StrokeBorder  border = stroker->borders + side;


      /* compute middle point and first angle point */
      FT_ویکټور_From_Polar( &منځ, radius, angle );
      delta.x = side ?  منځ.y : -منځ.y;
      delta.y = side ? -منځ.x :  منځ.x;

      إذا ( stroker->کرښه_cap == FT_STROKER_LINECAP_مربع )
      {
        منځ.x += stroker->center.x;
        منځ.y += stroker->center.y;
      }
      وإلا  /* FT_STROKER_LINECAP_BUTT */
      {
        منځ.x  = stroker->center.x;
        منځ.y  = stroker->center.y;
      }

      delta.x  += منځ.x;
      delta.y  += منځ.y;

      error = ft_stroke_border_lineto( border, &delta, FALSE );
      إذا ( error )
        اذهب Exit;

      /* compute second angle point */
      delta.x = منځ.x - delta.x + منځ.x;
      delta.y = منځ.y - delta.y + منځ.y;

      error = ft_stroke_border_lineto( border, &delta, FALSE );
    }

  Exit:
    أرجع error;
  }


  /* process an inside corner, i.e. compute intersection */
  ساكن FT_Error
  ft_stroker_inside( FT_Stroker  stroker,
                     FT_Int      side,
                     FT_Fixed    کرښه_اوږدتیا )
  {
    FT_StrokeBorder  border = stroker->borders + side;
    FT_Angle         phi, theta, rotate;
    FT_Fixed         اوږدتیا;
    FT_ویکټور        sigma = { 0, 0 };
    FT_ویکټور        delta;
    FT_Error         error = FT_Err_Ok;
    FT_Bool          intersect;          /* use intersection of lines? */


    rotate = FT_SIDE_منزل_ROTATE( side );

    theta = FT_Angle_Diff( stroker->angle_دننه, stroker->angle_بهر ) / 2;

    /* Only intersect borders if between two lineto's and both */
    /* lines are long enough (line_length is zero for curves). */
    /* Also avoid U-turns of nearly 180 degree.                */
    إذا ( !border->movable || کرښه_اوږدتیا == 0  ||
         theta > 0x59C000 || theta < -0x59C000 )
      intersect = FALSE;
    وإلا
    {
      /* compute minimum required length of lines */
      FT_Fixed  min_اوږدتیا;


      FT_ویکټور_Unit( &sigma, theta );
      min_اوږدتیا =
        ft_pos_abs( FT_MulDiv( stroker->radius, sigma.y, sigma.x ) );

      intersect = FT_BOOL( min_اوږدتیا                         &&
                           stroker->کرښه_اوږدتیا >= min_اوږدتیا &&
                           کرښه_اوږدتیا          >= min_اوږدتیا );
    }

    إذا ( !intersect )
    {
      FT_ویکټور_From_Polar( &delta, stroker->radius,
                            stroker->angle_بهر + rotate );
      delta.x += stroker->center.x;
      delta.y += stroker->center.y;

      border->movable = FALSE;
    }
    وإلا
    {
      /* compute median angle */
      phi = stroker->angle_دننه + theta + rotate;

      اوږدتیا = FT_DivFix( stroker->radius, sigma.x );

      FT_ویکټور_From_Polar( &delta, اوږدتیا, phi );
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
                      FT_Fixed    کرښه_اوږدتیا )
  {
    FT_StrokeBorder  border = stroker->borders + side;
    FT_Error         error;
    FT_Angle         rotate;


    إذا ( stroker->کرښه_join == FT_STROKER_LINEJOIN_پړاو )
      error = ft_stroker_arcto( stroker, side );
    وإلا
    {
      /* this is a mitered (pointed) or beveled (truncated) corner */
      FT_Fixed   radius = stroker->radius;
      FT_ویکټور  sigma = { 0, 0 };
      FT_Angle   theta = 0, phi = 0;
      FT_Bool    bevel, fixed_bevel;


      rotate = FT_SIDE_منزل_ROTATE( side );

      bevel =
        FT_BOOL( stroker->کرښه_join == FT_STROKER_LINEJOIN_BEVEL );

      fixed_bevel =
        FT_BOOL( stroker->کرښه_join != FT_STROKER_LINEJOIN_MITER_VARIABLE );

      /* check miter limit first */
      إذا ( !bevel )
      {
        theta = FT_Angle_Diff( stroker->angle_دننه, stroker->angle_بهر ) / 2;

        إذا ( theta == FT_ANGLE_PI2 )
          theta = -rotate;

        phi = stroker->angle_دننه + theta + rotate;

        FT_ویکټور_From_Polar( &sigma, stroker->miter_حد, theta );

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
          FT_ویکټور  delta;


          /* add bevel */
          FT_ویکټور_From_Polar( &delta,
                                radius,
                                stroker->angle_بهر + rotate );
          delta.x += stroker->center.x;
          delta.y += stroker->center.y;

          border->movable = FALSE;
          error = ft_stroke_border_lineto( border, &delta, FALSE );
        }
        وإلا /* variable bevel or clipped miter */
        {
          /* the miter is truncated */
          FT_ویکټور  منځ, delta;
          FT_Fixed   coef;


          /* compute middle point and first angle point */
          FT_ویکټور_From_Polar( &منځ,
                                FT_MulFix( radius, stroker->miter_حد ),
                                phi );

          coef    = FT_DivFix(  0x10000L - sigma.x, sigma.y );
          delta.x = FT_MulFix(  منځ.y, coef );
          delta.y = FT_MulFix( -منځ.x, coef );

          منځ.x += stroker->center.x;
          منځ.y += stroker->center.y;
          delta.x  += منځ.x;
          delta.y  += منځ.y;

          error = ft_stroke_border_lineto( border, &delta, FALSE );
          إذا ( error )
            اذهب Exit;

          /* compute second angle point */
          delta.x = منځ.x - delta.x + منځ.x;
          delta.y = منځ.y - delta.y + منځ.y;

          error = ft_stroke_border_lineto( border, &delta, FALSE );
          إذا ( error )
            اذهب Exit;

          /* finally, add an end point; only needed if not lineto */
          /* (line_length is zero for curves)                     */
          إذا ( کرښه_اوږدتیا == 0 )
          {
            FT_ویکټور_From_Polar( &delta,
                                  radius,
                                  stroker->angle_بهر + rotate );

            delta.x += stroker->center.x;
            delta.y += stroker->center.y;

            error = ft_stroke_border_lineto( border, &delta, FALSE );
          }
        }
      }
      وإلا /* this is a miter (intersection) */
      {
        FT_Fixed   اوږدتیا;
        FT_ویکټور  delta;


        اوږدتیا = FT_MulDiv( stroker->radius, stroker->miter_حد, sigma.x );

        FT_ویکټور_From_Polar( &delta, اوږدتیا, phi );
        delta.x += stroker->center.x;
        delta.y += stroker->center.y;

        error = ft_stroke_border_lineto( border, &delta, FALSE );
        إذا ( error )
          اذهب Exit;

        /* now add an end point; only needed if not lineto */
        /* (line_length is zero for curves)                */
        إذا ( کرښه_اوږدتیا == 0 )
        {
          FT_ویکټور_From_Polar( &delta,
                                stroker->radius,
                                stroker->angle_بهر + rotate );
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
  ft_stroker_process_کونج( FT_Stroker  stroker,
                             FT_Fixed    کرښه_اوږدتیا )
  {
    FT_Error  error = FT_Err_Ok;
    FT_Angle  turn;
    FT_Int    inside_side;


    turn = FT_Angle_Diff( stroker->angle_دننه, stroker->angle_بهر );

    /* no specific corner processing is required if the turn is 0 */
    إذا ( turn == 0 )
      اذهب Exit;

    /* when we turn to the right, the inside side is 0 */
    /* otherwise, the inside side is 1 */
    inside_side = ( turn < 0 );

    /* process the inside side */
    error = ft_stroker_inside( stroker, inside_side, کرښه_اوږدتیا );
    إذا ( error )
      اذهب Exit;

    /* process the outside side */
    error = ft_stroker_outside( stroker, !inside_side, کرښه_اوږدتیا );

  Exit:
    أرجع error;
  }


  /* add two points to the left and right borders corresponding to the */
  /* start of the subpath                                              */
  ساكن FT_Error
  ft_stroker_subpath_پیل( FT_Stroker  stroker,
                            FT_Angle    پیل_angle,
                            FT_Fixed    کرښه_اوږدتیا )
  {
    FT_ویکټور        delta;
    FT_ویکټور        ټکی;
    FT_Error         error;
    FT_StrokeBorder  border;


    FT_ویکټور_From_Polar( &delta, stroker->radius,
                          پیل_angle + FT_ANGLE_PI2 );

    ټکی.x = stroker->center.x + delta.x;
    ټکی.y = stroker->center.y + delta.y;

    border = stroker->borders;
    error = ft_stroke_border_moveto( border, &ټکی );
    إذا ( error )
      اذهب Exit;

    ټکی.x = stroker->center.x - delta.x;
    ټکی.y = stroker->center.y - delta.y;

    border++;
    error = ft_stroke_border_moveto( border, &ټکی );

    /* save angle, position, and line length for last join */
    /* (line_length is zero for curves)                    */
    stroker->subpath_angle       = پیل_angle;
    stroker->لومړی_ټکی         = FALSE;
    stroker->subpath_کرښه_اوږدتیا = کرښه_اوږدتیا;

  Exit:
    أرجع error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Stroker_کرښه_منزل( FT_Stroker  stroker,
                     FT_ویکټور*  منزل )
  {
    FT_Error         error = FT_Err_Ok;
    FT_StrokeBorder  border;
    FT_ویکټور        delta;
    FT_Angle         angle;
    FT_Int           side;
    FT_Fixed         کرښه_اوږدتیا;


    إذا ( !stroker || !منزل )
      أرجع FT_THROW( Invalid_دلیل );

    delta.x = منزل->x - stroker->center.x;
    delta.y = منزل->y - stroker->center.y;

    /* a zero-length lineto is a no-op; avoid creating a spurious corner */
    إذا ( delta.x == 0 && delta.y == 0 )
       اذهب Exit;

    /* compute length of line */
    کرښه_اوږدتیا = FT_ویکټور_اوږدتیا( &delta );

    angle = FT_Atan2( delta.x, delta.y );
    FT_ویکټور_From_Polar( &delta, stroker->radius, angle + FT_ANGLE_PI2 );

    /* process corner if necessary */
    إذا ( stroker->لومړی_ټکی )
    {
      /* This is the first segment of a subpath.  We need to     */
      /* add a point to each border at their respective starting */
      /* point locations.                                        */
      error = ft_stroker_subpath_پیل( stroker, angle, کرښه_اوږدتیا );
      إذا ( error )
        اذهب Exit;
    }
    وإلا
    {
      /* process the current corner */
      stroker->angle_بهر = angle;
      error = ft_stroker_process_کونج( stroker, کرښه_اوږدتیا );
      إذا ( error )
        اذهب Exit;
    }

    /* now add a line segment to both the `inside' and `outside' paths */
    لكل ( border = stroker->borders, side = 1; side >= 0; side--, border++ )
    {
      FT_ویکټور  ټکی;


      ټکی.x = منزل->x + delta.x;
      ټکی.y = منزل->y + delta.y;

      /* the ends of lineto borders are movable */
      error = ft_stroke_border_lineto( border, &ټکی, TRUE );
      إذا ( error )
        اذهب Exit;

      delta.x = -delta.x;
      delta.y = -delta.y;
    }

    stroker->angle_دننه    = angle;
    stroker->center      = *منزل;
    stroker->کرښه_اوږدتیا = کرښه_اوږدتیا;

  Exit:
    أرجع error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Stroker_Conic_منزل( FT_Stroker  stroker,
                      FT_ویکټور*  control,
                      FT_ویکټور*  منزل )
  {
    FT_Error    error = FT_Err_Ok;
    FT_ویکټور   bez_ډېری[34];
    FT_ویکټور*  arc;
    FT_ویکټور*  حد = bez_ډېری + 30;
    FT_Bool     لومړی_arc = TRUE;


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

    arc    = bez_ډېری;
    arc[0] = *منزل;
    arc[1] = *control;
    arc[2] = stroker->center;

    افعل
    {
      FT_Angle  angle_دننه, angle_بهر;


      /* initialize with current direction */
      angle_دننه = angle_بهر = stroker->angle_دننه;

      إذا ( arc < حد                                             &&
           !ft_conic_is_small_enough( arc, &angle_دننه, &angle_بهر ) )
      {
        إذا ( stroker->لومړی_ټکی )
          stroker->angle_دننه = angle_دننه;

        ft_conic_وېشل( arc );
        arc += 2;
        استمر;
      }

      إذا ( لومړی_arc )
      {
        لومړی_arc = FALSE;

        /* process corner if necessary */
        إذا ( stroker->لومړی_ټکی )
          error = ft_stroker_subpath_پیل( stroker, angle_دننه, 0 );
        وإلا
        {
          stroker->angle_بهر = angle_دننه;
          error = ft_stroker_process_کونج( stroker, 0 );
        }
      }
      وإلا إذا ( ft_pos_abs( FT_Angle_Diff( stroker->angle_دننه, angle_دننه ) ) >
                  FT_SMALL_CONIC_پوله / 4                             )
      {
        /* if the deviation from one arc to the next is too great, */
        /* add a round corner                                      */
        stroker->center    = arc[2];
        stroker->angle_بهر = angle_دننه;
        stroker->کرښه_join = FT_STROKER_LINEJOIN_پړاو;

        error = ft_stroker_process_کونج( stroker, 0 );

        /* reinstate line join style */
        stroker->کرښه_join = stroker->کرښه_join_saved;
      }

      إذا ( error )
        اذهب Exit;

      /* the arc's angle is small enough; we can add it directly to each */
      /* border                                                          */
      {
        FT_ویکټور        ctrl, پای_ټکی;
        FT_Angle         theta, phi, rotate, alpha0 = 0;
        FT_Fixed         اوږدتیا;
        FT_StrokeBorder  border;
        FT_Int           side;


        theta  = FT_Angle_Diff( angle_دننه, angle_بهر ) / 2;
        phi    = angle_دننه + theta;
        اوږدتیا = FT_DivFix( stroker->radius, FT_Cos( theta ) );

        /* compute direction of original arc */
        إذا ( stroker->handle_wide_strokes )
          alpha0 = FT_Atan2( arc[0].x - arc[2].x, arc[0].y - arc[2].y );

        لكل ( border = stroker->borders, side = 0;
              side <= 1;
              side++, border++ )
        {
          rotate = FT_SIDE_منزل_ROTATE( side );

          /* compute control point */
          FT_ویکټور_From_Polar( &ctrl, اوږدتیا, phi + rotate );
          ctrl.x += arc[1].x;
          ctrl.y += arc[1].y;

          /* compute end point */
          FT_ویکټور_From_Polar( &پای_ټکی, stroker->radius, angle_بهر + rotate );
          پای_ټکی.x += arc[0].x;
          پای_ټکی.y += arc[0].y;

          إذا ( stroker->handle_wide_strokes )
          {
            FT_ویکټور  پیل;
            FT_Angle   alpha1;


            /* determine whether the border radius is greater than the */
            /* radius of curvature of the original arc                 */
            پیل = border->points[border->عدد_points - 1];

            alpha1 = FT_Atan2( پای_ټکی.x - پیل.x, پای_ټکی.y - پیل.y );

            /* is the direction of the border arc opposite to */
            /* that of the original arc? */
            إذا ( ft_pos_abs( FT_Angle_Diff( alpha0, alpha1 ) ) >
                   FT_ANGLE_PI / 2                             )
            {
              FT_Angle   beta, gamma;
              FT_ویکټور  bvec, delta;
              FT_Fixed   blen, sinA, sinB, alen;


              /* use the sine rule to find the intersection point */
              beta  = FT_Atan2( arc[2].x - پیل.x, arc[2].y - پیل.y );
              gamma = FT_Atan2( arc[0].x - پای_ټکی.x,   arc[0].y - پای_ټکی.y );

              bvec.x = پای_ټکی.x - پیل.x;
              bvec.y = پای_ټکی.y - پیل.y;

              blen = FT_ویکټور_اوږدتیا( &bvec );

              sinA = ft_pos_abs( FT_Sin( alpha1 - gamma ) );
              sinB = ft_pos_abs( FT_Sin( beta - gamma ) );

              alen = FT_MulDiv( blen, sinA, sinB );

              FT_ویکټور_From_Polar( &delta, alen, beta );
              delta.x += پیل.x;
              delta.y += پیل.y;

              /* circumnavigate the negative sector backwards */
              border->movable = FALSE;
              error = ft_stroke_border_lineto( border, &delta, FALSE );
              إذا ( error )
                اذهب Exit;
              error = ft_stroke_border_lineto( border, &پای_ټکی, FALSE );
              إذا ( error )
                اذهب Exit;
              error = ft_stroke_border_conicto( border, &ctrl, &پیل );
              إذا ( error )
                اذهب Exit;
              /* and then move to the endpoint */
              error = ft_stroke_border_lineto( border, &پای_ټکی, FALSE );
              إذا ( error )
                اذهب Exit;

              استمر;
            }

            /* else fall through */
          }

          /* simply add an arc */
          error = ft_stroke_border_conicto( border, &ctrl, &پای_ټکی );
          إذا ( error )
            اذهب Exit;
        }
      }

      stroker->angle_دننه = angle_بهر;

      إذا ( arc == bez_ډېری )
        اكسر;
      arc -= 2;
    } طالما ( 1 );

    stroker->center      = *منزل;
    stroker->کرښه_اوږدتیا = 0;

  Exit:
    أرجع error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Stroker_Cubic_منزل( FT_Stroker  stroker,
                      FT_ویکټور*  control1,
                      FT_ویکټور*  control2,
                      FT_ویکټور*  منزل )
  {
    FT_Error    error = FT_Err_Ok;
    FT_ویکټور   bez_ډېری[37];
    FT_ویکټور*  arc;
    FT_ویکټور*  حد = bez_ډېری + 32;
    FT_Bool     لومړی_arc = TRUE;


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

    arc    = bez_ډېری;
    arc[0] = *منزل;
    arc[1] = *control2;
    arc[2] = *control1;
    arc[3] = stroker->center;

    افعل
    {
      FT_Angle  angle_دننه, angle_mid, angle_بهر;


      /* initialize with current direction */
      angle_دننه = angle_بهر = angle_mid = stroker->angle_دننه;

      إذا ( arc < حد                                         &&
           !ft_cubic_is_small_enough( arc, &angle_دننه,
                                      &angle_mid, &angle_بهر ) )
      {
        إذا ( stroker->لومړی_ټکی )
          stroker->angle_دننه = angle_دننه;

        ft_cubic_وېشل( arc );
        arc += 3;
        استمر;
      }

      إذا ( لومړی_arc )
      {
        لومړی_arc = FALSE;

        /* process corner if necessary */
        إذا ( stroker->لومړی_ټکی )
          error = ft_stroker_subpath_پیل( stroker, angle_دننه, 0 );
        وإلا
        {
          stroker->angle_بهر = angle_دننه;
          error = ft_stroker_process_کونج( stroker, 0 );
        }
      }
      وإلا إذا ( ft_pos_abs( FT_Angle_Diff( stroker->angle_دننه, angle_دننه ) ) >
                  FT_SMALL_CUBIC_پوله / 4                             )
      {
        /* if the deviation from one arc to the next is too great, */
        /* add a round corner                                      */
        stroker->center    = arc[3];
        stroker->angle_بهر = angle_دننه;
        stroker->کرښه_join = FT_STROKER_LINEJOIN_پړاو;

        error = ft_stroker_process_کونج( stroker, 0 );

        /* reinstate line join style */
        stroker->کرښه_join = stroker->کرښه_join_saved;
      }

      إذا ( error )
        اذهب Exit;

      /* the arc's angle is small enough; we can add it directly to each */
      /* border                                                          */
      {
        FT_ویکټور        ctrl1, ctrl2, پای_ټکی;
        FT_Angle         theta1, phi1, theta2, phi2, rotate, alpha0 = 0;
        FT_Fixed         length1, length2;
        FT_StrokeBorder  border;
        FT_Int           side;


        theta1  = FT_Angle_Diff( angle_دننه,  angle_mid ) / 2;
        theta2  = FT_Angle_Diff( angle_mid, angle_بهر ) / 2;
        phi1    = ft_angle_منځنۍ( angle_دننه,  angle_mid );
        phi2    = ft_angle_منځنۍ( angle_mid, angle_بهر );
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
          FT_ویکټور_From_Polar( &ctrl1, length1, phi1 + rotate );
          ctrl1.x += arc[2].x;
          ctrl1.y += arc[2].y;

          FT_ویکټور_From_Polar( &ctrl2, length2, phi2 + rotate );
          ctrl2.x += arc[1].x;
          ctrl2.y += arc[1].y;

          /* compute end point */
          FT_ویکټور_From_Polar( &پای_ټکی, stroker->radius, angle_بهر + rotate );
          پای_ټکی.x += arc[0].x;
          پای_ټکی.y += arc[0].y;

          إذا ( stroker->handle_wide_strokes )
          {
            FT_ویکټور  پیل;
            FT_Angle   alpha1;


            /* determine whether the border radius is greater than the */
            /* radius of curvature of the original arc                 */
            پیل = border->points[border->عدد_points - 1];

            alpha1 = FT_Atan2( پای_ټکی.x - پیل.x, پای_ټکی.y - پیل.y );

            /* is the direction of the border arc opposite to */
            /* that of the original arc? */
            إذا ( ft_pos_abs( FT_Angle_Diff( alpha0, alpha1 ) ) >
                   FT_ANGLE_PI / 2                             )
            {
              FT_Angle   beta, gamma;
              FT_ویکټور  bvec, delta;
              FT_Fixed   blen, sinA, sinB, alen;


              /* use the sine rule to find the intersection point */
              beta  = FT_Atan2( arc[3].x - پیل.x, arc[3].y - پیل.y );
              gamma = FT_Atan2( arc[0].x - پای_ټکی.x,   arc[0].y - پای_ټکی.y );

              bvec.x = پای_ټکی.x - پیل.x;
              bvec.y = پای_ټکی.y - پیل.y;

              blen = FT_ویکټور_اوږدتیا( &bvec );

              sinA = ft_pos_abs( FT_Sin( alpha1 - gamma ) );
              sinB = ft_pos_abs( FT_Sin( beta - gamma ) );

              alen = FT_MulDiv( blen, sinA, sinB );

              FT_ویکټور_From_Polar( &delta, alen, beta );
              delta.x += پیل.x;
              delta.y += پیل.y;

              /* circumnavigate the negative sector backwards */
              border->movable = FALSE;
              error = ft_stroke_border_lineto( border, &delta, FALSE );
              إذا ( error )
                اذهب Exit;
              error = ft_stroke_border_lineto( border, &پای_ټکی, FALSE );
              إذا ( error )
                اذهب Exit;
              error = ft_stroke_border_cubicto( border,
                                                &ctrl2,
                                                &ctrl1,
                                                &پیل );
              إذا ( error )
                اذهب Exit;
              /* and then move to the endpoint */
              error = ft_stroke_border_lineto( border, &پای_ټکی, FALSE );
              إذا ( error )
                اذهب Exit;

              استمر;
            }

            /* else fall through */
          }

          /* simply add an arc */
          error = ft_stroke_border_cubicto( border, &ctrl1, &ctrl2, &پای_ټکی );
          إذا ( error )
            اذهب Exit;
        }
      }

      stroker->angle_دننه = angle_بهر;

      إذا ( arc == bez_ډېری )
        اكسر;
      arc -= 3;
    } طالما ( 1 );

    stroker->center      = *منزل;
    stroker->کرښه_اوږدتیا = 0;

  Exit:
    أرجع error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Stroker_Begin_Sub_لار( FT_Stroker  stroker,
                           FT_ویکټور*  منزل,
                           FT_Bool     open )
  {
    إذا ( !stroker || !منزل )
      أرجع FT_THROW( Invalid_دلیل );

    /* We cannot process the first point, because there is not enough      */
    /* information regarding its corner/cap.  The latter will be processed */
    /* in the `FT_Stroker_EndSubPath' routine.                             */
    /*                                                                     */
    stroker->لومړی_ټکی  = TRUE;
    stroker->center       = *منزل;
    stroker->subpath_open = open;

    /* Determine if we need to check whether the border radius is greater */
    /* than the radius of curvature of a curve, to handle this case       */
    /* specially.  This is only required if bevel joins or butt caps may  */
    /* be created, because round & miter joins and round & square caps    */
    /* cover the negative sector created with wide strokes.               */
    stroker->handle_wide_strokes =
      FT_BOOL( stroker->کرښه_join != FT_STROKER_LINEJOIN_پړاو  ||
               ( stroker->subpath_open                        &&
                 stroker->کرښه_cap == FT_STROKER_LINECAP_BUTT ) );

    /* record the subpath start point for each border */
    stroker->subpath_پیل = *منزل;

    stroker->angle_دننه = 0;

    أرجع FT_Err_Ok;
  }


  ساكن FT_Error
  ft_stroker_جمع_کړه_سرچپه_کیڼ( FT_Stroker  stroker,
                               FT_Bool     open )
  {
    FT_StrokeBorder  ښی = stroker->borders + 0;
    FT_StrokeBorder  کیڼ  = stroker->borders + 1;
    FT_Int           new_points;
    FT_Error         error = FT_Err_Ok;


    FT_ASSERT( کیڼ->پیل >= 0 );

    new_points = (FT_Int)کیڼ->عدد_points - کیڼ->پیل;
    إذا ( new_points > 0 )
    {
      error = ft_stroke_border_grow( ښی, (FT_UInt)new_points );
      إذا ( error )
        اذهب Exit;

      {
        FT_ویکټور*  dst_ټکی = ښی->points + ښی->عدد_points;
        FT_Byte*    dst_نښان   = ښی->نښانونه   + ښی->عدد_points;
        FT_ویکټور*  src_ټکی = کیڼ->points  + کیڼ->عدد_points - 1;
        FT_Byte*    src_نښان   = کیڼ->نښانونه    + کیڼ->عدد_points - 1;


        طالما ( src_ټکی >= کیڼ->points + کیڼ->پیل )
        {
          *dst_ټکی = *src_ټکی;
          *dst_نښان   = *src_نښان;

          إذا ( open )
            dst_نښان[0] &= ~FT_STROKE_نښان_BEGIN_پای_ټکی;
          وإلا
          {
            FT_Byte  ttag =
                       (FT_Byte)( dst_نښان[0] & FT_STROKE_نښان_BEGIN_پای_ټکی );


            /* switch begin/end tags if necessary */
            إذا ( ttag == FT_STROKE_نښان_BEGIN ||
                 ttag == FT_STROKE_نښان_پای_ټکی   )
              dst_نښان[0] ^= FT_STROKE_نښان_BEGIN_پای_ټکی;
          }

          src_ټکی--;
          src_نښان--;
          dst_ټکی++;
          dst_نښان++;
        }
      }

      کیڼ->عدد_points   = (FT_UInt)کیڼ->پیل;
      ښی->عدد_points += (FT_UInt)new_points;

      ښی->movable = FALSE;
      کیڼ->movable  = FALSE;
    }

  Exit:
    أرجع error;
  }


  /* documentation is in ftstroke.h */

  /* there's a lot of magic in this function! */
  FT_EXPORT_DEF( FT_Error )
  FT_Stroker_پای_ټکی_Sub_لار( FT_Stroker  stroker )
  {
    FT_Error  error = FT_Err_Ok;


    إذا ( !stroker )
    {
      error = FT_THROW( Invalid_دلیل );
      اذهب Exit;
    }

    /* don't try to end the path if no segments have been generated */
    إذا ( stroker->لومړی_ټکی )
      اذهب Exit;

    إذا ( stroker->subpath_open )
    {
      FT_StrokeBorder  ښی = stroker->borders;


      /* All right, this is an opened path, we need to add a cap between */
      /* right & left, add the reverse of left, then add a final cap     */
      /* between left & right.                                           */
      error = ft_stroker_cap( stroker, stroker->angle_دننه, 0 );
      إذا ( error )
        اذهب Exit;

      /* add reversed points from `left' to `right' */
      error = ft_stroker_جمع_کړه_سرچپه_کیڼ( stroker, TRUE );
      إذا ( error )
        اذهب Exit;

      /* now add the final cap */
      stroker->center = stroker->subpath_پیل;
      error = ft_stroker_cap( stroker,
                              stroker->subpath_angle + FT_ANGLE_PI, 0 );
      إذا ( error )
        اذهب Exit;

      /* Now end the right subpath accordingly.  The left one is */
      /* rewind and doesn't need further processing.             */
      ft_stroke_border_close( ښی, FALSE );
    }
    وإلا
    {
      /* close the path if needed */
      إذا ( !FT_IS_SMALL( stroker->center.x - stroker->subpath_پیل.x ) ||
           !FT_IS_SMALL( stroker->center.y - stroker->subpath_پیل.y ) )
      {
         error = FT_Stroker_کرښه_منزل( stroker, &stroker->subpath_پیل );
         إذا ( error )
           اذهب Exit;
      }

      /* process the corner */
      stroker->angle_بهر = stroker->subpath_angle;

      error = ft_stroker_process_کونج( stroker,
                                         stroker->subpath_کرښه_اوږدتیا );
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

    إذا ( border == FT_STROKER_BORDER_کیڼ  ||
         border == FT_STROKER_BORDER_ښی )
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
    FT_Stroker_ExportBorder( stroker, FT_STROKER_BORDER_کیڼ, outline );
    FT_Stroker_ExportBorder( stroker, FT_STROKER_BORDER_ښی, outline );
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
    FT_ویکټور   v_وروستی;
    FT_ویکټور   v_control;
    FT_ویکټور   v_پیل;

    FT_ویکټور*  ټکی;
    FT_ویکټور*  حد;
    FT_Byte*    نښانونه;

    FT_Error    error;

    FT_Int      n;         /* index of contour in outline     */
    FT_Int      لومړی;     /* index of first point in contour */
    FT_Int      وروستی;      /* index of last point in contour  */

    FT_Int      نښان;       /* current point's state           */


    إذا ( !outline )
      أرجع FT_THROW( Invalid_Outline );

    إذا ( !stroker )
      أرجع FT_THROW( Invalid_دلیل );

    FT_Stroker_Rewind( stroker );

    وروستی = -1;
    لكل ( n = 0; n < outline->n_contours; n++ )
    {
      لومړی = وروستی + 1;
      وروستی  = outline->contours[n];

      /* skip empty points; we don't stroke these */
      إذا ( وروستی <= لومړی )
        استمر;

      حد = outline->points + وروستی;

      v_پیل = outline->points[لومړی];
      v_وروستی  = outline->points[وروستی];

      v_control = v_پیل;

      ټکی = outline->points + لومړی;
      نښانونه  = outline->نښانونه   + لومړی;
      نښان   = FT_منحني_نښان( نښانونه[0] );

      /* check first point to determine origin */
      إذا ( نښان != FT_منحني_نښان_ON )
      {
        FT_Int  وروستی_نښان = FT_منحني_نښان( outline->نښانونه[وروستی] );


        /* First point is a control point.  Yes, this happens. */
        إذا ( وروستی_نښان == FT_منحني_نښان_ON )
        {
          /* start at last point if it is on the curve */
          v_پیل = v_وروستی;
          حد--;
        }
        وإلا
        {
          إذا ( وروستی_نښان != نښان )
            اذهب Invalid_Outline;

          /* if both first and last points are controls, */
          /* start at their middle                       */
          v_پیل.x = ( v_پیل.x + v_وروستی.x ) / 2;
          v_پیل.y = ( v_پیل.y + v_وروستی.y ) / 2;
        }
        ټکی--;
        نښانونه--;
      }

      error = FT_Stroker_Begin_Sub_لار( stroker, &v_پیل, opened );
      إذا ( error )
        اذهب Exit;

      طالما ( ټکی < حد )
      {
        ټکی++;
        نښانونه++;

        نښان = FT_منحني_نښان( نښانونه[0] );
        بدل ( نښان )
        {
        حالة FT_منحني_نښان_ON:  /* emit a single line_to */
          {
            FT_ویکټور  vec;


            vec.x = ټکی->x;
            vec.y = ټکی->y;

            error = FT_Stroker_کرښه_منزل( stroker, &vec );
            إذا ( error )
              اذهب Exit;
            استمر;
          }

        حالة FT_منحني_نښان_CONIC:  /* consume conic arcs */
          v_control.x = ټکی->x;
          v_control.y = ټکی->y;

        Do_Conic:
          إذا ( ټکی < حد )
          {
            FT_ویکټور  vec;
            FT_ویکټور  v_منځ;


            ټکی++;
            نښانونه++;
            نښان = FT_منحني_نښان( نښانونه[0] );

            vec = ټکی[0];

            إذا ( نښان == FT_منحني_نښان_ON )
            {
              error = FT_Stroker_Conic_منزل( stroker, &v_control, &vec );
              إذا ( error )
                اذهب Exit;
              استمر;
            }

            إذا ( نښان != FT_منحني_نښان_CONIC )
              اذهب Invalid_Outline;

            v_منځ.x = ( v_control.x + vec.x ) / 2;
            v_منځ.y = ( v_control.y + vec.y ) / 2;

            error = FT_Stroker_Conic_منزل( stroker, &v_control, &v_منځ );
            إذا ( error )
              اذهب Exit;

            v_control = vec;
            اذهب Do_Conic;
          }

          error = FT_Stroker_Conic_منزل( stroker, &v_control, &v_پیل );
          اذهب Close;

        افتراضي:  /* FT_CURVE_TAG_CUBIC */
          {
            FT_ویکټور  vec1, vec2;


            إذا ( ټکی + 1 > حد                             ||
                 FT_منحني_نښان( نښانونه[1] ) != FT_منحني_نښان_CUBIC )
              اذهب Invalid_Outline;

            ټکی += 2;
            نښانونه  += 2;

            vec1 = ټکی[-2];
            vec2 = ټکی[-1];

            إذا ( ټکی <= حد )
            {
              FT_ویکټور  vec;
              FT_ویکټور  v_منځ;


              نښان = FT_منحني_نښان( نښانونه[0] );
              vec = ټکی[0];

              إذا ( نښان == FT_منحني_نښان_CUBIC )
              {
                v_منځ.x = ( vec2.x + vec.x ) / 2;
                v_منځ.y = ( vec2.y + vec.y ) / 2;

                vec = v_منځ;
              }
              وإلا إذا ( نښان != FT_منحني_نښان_ON )
                اذهب Invalid_Outline;

              error = FT_Stroker_Cubic_منزل( stroker, &vec1, &vec2, &vec );
              إذا ( error )
                اذهب Exit;

              إذا ( نښان == FT_منحني_نښان_CUBIC )
              {
                ټکی--;
                نښانونه--;
              }
              استمر;
            }

            error = FT_Stroker_Cubic_منزل( stroker, &vec1, &vec2, &v_پیل );
            اذهب Close;
          }
        }
      }

    Close:
      إذا ( error )
        اذهب Exit;

      error = FT_Stroker_پای_ټکی_Sub_لار( stroker );
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

      FT_Outline_بشپړ( glyph->library, outline );

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
      FT_بشپړ_Glyph( *pglyph );

    *pglyph = glyph;
    اذهب Exit;

  Fail:
    FT_بشپړ_Glyph( glyph );
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
        إذا ( border == FT_STROKER_BORDER_کیڼ )
          border = FT_STROKER_BORDER_ښی;
        وإلا
          border = FT_STROKER_BORDER_کیڼ;
      }

      error = FT_Stroker_ParseOutline( stroker, outline, FALSE );
      إذا ( error )
        اذهب Fail;

      FT_Stroker_GetBorderCounts( stroker, border,
                                  &عدد_points, &عدد_contours );

      FT_Outline_بشپړ( glyph->library, outline );

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
      FT_بشپړ_Glyph( *pglyph );

    *pglyph = glyph;
    اذهب Exit;

  Fail:
    FT_بشپړ_Glyph( glyph );
    glyph = عدم;

    إذا ( !destroy )
      *pglyph = عدم;

  Exit:
    أرجع error;
  }


/*پای_ټکی*/
