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


    أرجع o == FT_ORIENTATION_TRUETYPE ? FT_STROKER_BORDER_ساڄو
                                        : FT_STROKER_BORDER_کاٻو;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_StrokerBorder )
  FT_Outline_GetOutsideBorder( FT_Outline*  outline )
  {
    FT_Orientation  o = FT_Outline_Get_Orientation( outline );


    أرجع o == FT_ORIENTATION_TRUETYPE ? FT_STROKER_BORDER_کاٻو
                                        : FT_STROKER_BORDER_ساڄو;
  }


  /*************************************************************************/
  /*************************************************************************/
  /*****                                                               *****/
  /*****                      BEZIER COMPUTATIONS                      *****/
  /*****                                                               *****/
  /*************************************************************************/
  /*************************************************************************/

#وصف_ڪريو FT_SMALL_CONIC_حد  ( FT_ANGLE_PI / 6 )
#وصف_ڪريو FT_SMALL_CUBIC_حد  ( FT_ANGLE_PI / 8 )

#وصف_ڪريو FT_EPSILON  2

#وصف_ڪريو FT_IS_SMALL( x )  ( (x) > -FT_EPSILON && (x) < FT_EPSILON )


  ساكن FT_Pos
  ft_pos_abs( FT_Pos  x )
  {
    أرجع x >= 0 ? x : -x;
  }


  ساكن فراغ
  ft_conic_ورهايو( FT_ويڪٽر*  بنياد )
  {
    FT_Pos  a, b;


    بنياد[4].x = بنياد[2].x;
    a = بنياد[0].x + بنياد[1].x;
    b = بنياد[1].x + بنياد[2].x;
    بنياد[3].x = b >> 1;
    بنياد[2].x = ( a + b ) >> 2;
    بنياد[1].x = a >> 1;

    بنياد[4].y = بنياد[2].y;
    a = بنياد[0].y + بنياد[1].y;
    b = بنياد[1].y + بنياد[2].y;
    بنياد[3].y = b >> 1;
    بنياد[2].y = ( a + b ) >> 2;
    بنياد[1].y = a >> 1;
  }


  ساكن FT_Bool
  ft_conic_is_small_enough( FT_ويڪٽر*  بنياد,
                            FT_Angle   *angle_اندريون,
                            FT_Angle   *angle_ٻاهريون )
  {
    FT_ويڪٽر  d1, d2;
    FT_Angle   theta;
    FT_Int     close1, close2;


    d1.x = بنياد[1].x - بنياد[2].x;
    d1.y = بنياد[1].y - بنياد[2].y;
    d2.x = بنياد[0].x - بنياد[1].x;
    d2.y = بنياد[0].y - بنياد[1].y;

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
        *angle_اندريون  =
        *angle_ٻاهريون = FT_Atan2( d2.x, d2.y );
      }
    }
    وإلا /* !close1 */
    {
      إذا ( close2 )
      {
        *angle_اندريون  =
        *angle_ٻاهريون = FT_Atan2( d1.x, d1.y );
      }
      وإلا
      {
        *angle_اندريون  = FT_Atan2( d1.x, d1.y );
        *angle_ٻاهريون = FT_Atan2( d2.x, d2.y );
      }
    }

    theta = ft_pos_abs( FT_Angle_Diff( *angle_اندريون, *angle_ٻاهريون ) );

    أرجع FT_BOOL( theta < FT_SMALL_CONIC_حد );
  }


  ساكن فراغ
  ft_cubic_ورهايو( FT_ويڪٽر*  بنياد )
  {
    FT_Pos  a, b, c;


    بنياد[6].x = بنياد[3].x;
    a = بنياد[0].x + بنياد[1].x;
    b = بنياد[1].x + بنياد[2].x;
    c = بنياد[2].x + بنياد[3].x;
    بنياد[5].x = c >> 1;
    c += b;
    بنياد[4].x = c >> 2;
    بنياد[1].x = a >> 1;
    a += b;
    بنياد[2].x = a >> 2;
    بنياد[3].x = ( a + c ) >> 3;

    بنياد[6].y = بنياد[3].y;
    a = بنياد[0].y + بنياد[1].y;
    b = بنياد[1].y + بنياد[2].y;
    c = بنياد[2].y + بنياد[3].y;
    بنياد[5].y = c >> 1;
    c += b;
    بنياد[4].y = c >> 2;
    بنياد[1].y = a >> 1;
    a += b;
    بنياد[2].y = a >> 2;
    بنياد[3].y = ( a + c ) >> 3;
  }


  /* Return the average of `angle1' and `angle2'.            */
  /* This gives correct result even if `angle1' and `angle2' */
  /* have opposite signs.                                    */
  ساكن FT_Angle
  ft_angle_سراسري( FT_Angle  angle1,
                 FT_Angle  angle2 )
  {
    أرجع angle1 + FT_Angle_Diff( angle1, angle2 ) / 2;
  }


  ساكن FT_Bool
  ft_cubic_is_small_enough( FT_ويڪٽر*  بنياد,
                            FT_Angle   *angle_اندريون,
                            FT_Angle   *angle_mid,
                            FT_Angle   *angle_ٻاهريون )
  {
    FT_ويڪٽر  d1, d2, d3;
    FT_Angle   theta1, theta2;
    FT_Int     close1, close2, close3;


    d1.x = بنياد[2].x - بنياد[3].x;
    d1.y = بنياد[2].y - بنياد[3].y;
    d2.x = بنياد[1].x - بنياد[2].x;
    d2.y = بنياد[1].y - بنياد[2].y;
    d3.x = بنياد[0].x - بنياد[1].x;
    d3.y = بنياد[0].y - بنياد[1].y;

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
          *angle_اندريون  =
          *angle_mid =
          *angle_ٻاهريون = FT_Atan2( d3.x, d3.y );
        }
      }
      وإلا /* !close2 */
      {
        إذا ( close3 )
        {
          *angle_اندريون  =
          *angle_mid =
          *angle_ٻاهريون = FT_Atan2( d2.x, d2.y );
        }
        وإلا /* !close3 */
        {
          *angle_اندريون  =
          *angle_mid = FT_Atan2( d2.x, d2.y );
          *angle_ٻاهريون = FT_Atan2( d3.x, d3.y );
        }
      }
    }
    وإلا /* !close1 */
    {
      إذا ( close2 )
      {
        إذا ( close3 )
        {
          *angle_اندريون  =
          *angle_mid =
          *angle_ٻاهريون = FT_Atan2( d1.x, d1.y );
        }
        وإلا /* !close3 */
        {
          *angle_اندريون  = FT_Atan2( d1.x, d1.y );
          *angle_ٻاهريون = FT_Atan2( d3.x, d3.y );
          *angle_mid = ft_angle_سراسري( *angle_اندريون, *angle_ٻاهريون );
        }
      }
      وإلا /* !close2 */
      {
        إذا ( close3 )
        {
          *angle_اندريون  = FT_Atan2( d1.x, d1.y );
          *angle_mid =
          *angle_ٻاهريون = FT_Atan2( d2.x, d2.y );
        }
        وإلا /* !close3 */
        {
          *angle_اندريون  = FT_Atan2( d1.x, d1.y );
          *angle_mid = FT_Atan2( d2.x, d2.y );
          *angle_ٻاهريون = FT_Atan2( d3.x, d3.y );
        }
      }
    }

    theta1 = ft_pos_abs( FT_Angle_Diff( *angle_اندريون,  *angle_mid ) );
    theta2 = ft_pos_abs( FT_Angle_Diff( *angle_mid, *angle_ٻاهريون ) );

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

  تعريفنوع تعداد  FT_Stroke_ٽيگ_فهرست
  {
    FT_STROKE_ٽيگ_ON    = 1,   /* on-curve point  */
    FT_STROKE_ٽيگ_CUBIC = 2,   /* cubic off-point */
    FT_STROKE_ٽيگ_BEGIN = 4,   /* sub-path start  */
    FT_STROKE_ٽيگ_پڇاڙي   = 8    /* sub-path end    */

  } FT_Stroke_ٽيگ_فهرست_2;

#وصف_ڪريو  FT_STROKE_ٽيگ_BEGIN_پڇاڙي  ( FT_STROKE_ٽيگ_BEGIN | FT_STROKE_ٽيگ_پڇاڙي )

  تعريفنوع بنية  FT_StrokeBorderRec_
  {
    FT_UInt     انگ_points;
    FT_UInt     max_points;
    FT_ويڪٽر*  points;
    FT_Byte*    ٽيگ_فهرست;
    FT_Bool     movable;  /* TRUE for ends of lineto borders */
    FT_Int      شروعات;    /* index of current sub-path start point */
    FT_يادگيري   يادگيري;
    FT_Bool     valid;

  } FT_StrokeBorderRec, *FT_StrokeBorder;


  ساكن FT_Error
  ft_stroke_border_grow( FT_StrokeBorder  border,
                         FT_UInt          new_points )
  {
    FT_UInt   old_max = border->max_points;
    FT_UInt   new_max = border->انگ_points + new_points;
    FT_Error  error   = FT_Err_Ok;


    إذا ( new_max > old_max )
    {
      FT_UInt    cur_max = old_max;
      FT_يادگيري  يادگيري  = border->يادگيري;


      طالما ( cur_max < new_max )
        cur_max += ( cur_max >> 1 ) + 16;

      إذا ( FT_RENEW_ARRAY( border->points, old_max, cur_max ) ||
           FT_RENEW_ARRAY( border->ٽيگ_فهرست,   old_max, cur_max ) )
        اذهب Exit;

      border->max_points = cur_max;
    }

  Exit:
    أرجع error;
  }


  ساكن فراغ
  ft_stroke_border_close( FT_StrokeBorder  border,
                          FT_Bool          ابتو )
  {
    FT_UInt  شروعات = (FT_UInt)border->شروعات;
    FT_UInt  ڳڻپ = border->انگ_points;


    FT_ASSERT( border->شروعات >= 0 );

    /* don't record empty paths! */
    إذا ( ڳڻپ <= شروعات + 1U )
      border->انگ_points = شروعات;
    وإلا
    {
      /* copy the last point to the start of this sub-path, since */
      /* it contains the `adjusted' starting coordinates          */
      border->انگ_points    = --ڳڻپ;
      border->points[شروعات] = border->points[ڳڻپ];
      border->ٽيگ_فهرست[شروعات]   = border->ٽيگ_فهرست[ڳڻپ];

      إذا ( ابتو )
      {
        /* reverse the points */
        {
          FT_ويڪٽر*  vec1 = border->points + شروعات + 1;
          FT_ويڪٽر*  vec2 = border->points + ڳڻپ - 1;


          لكل ( ; vec1 < vec2; vec1++, vec2-- )
          {
            FT_ويڪٽر  tmp;


            tmp   = *vec1;
            *vec1 = *vec2;
            *vec2 = tmp;
          }
        }

        /* then the tags */
        {
          FT_Byte*  tag1 = border->ٽيگ_فهرست + شروعات + 1;
          FT_Byte*  tag2 = border->ٽيگ_فهرست + ڳڻپ - 1;


          لكل ( ; tag1 < tag2; tag1++, tag2-- )
          {
            FT_Byte  tmp;


            tmp   = *tag1;
            *tag1 = *tag2;
            *tag2 = tmp;
          }
        }
      }

      border->ٽيگ_فهرست[شروعات    ] |= FT_STROKE_ٽيگ_BEGIN;
      border->ٽيگ_فهرست[ڳڻپ - 1] |= FT_STROKE_ٽيگ_پڇاڙي;
    }

    border->شروعات   = -1;
    border->movable = FALSE;
  }


  ساكن FT_Error
  ft_stroke_border_lineto( FT_StrokeBorder  border,
                           FT_ويڪٽر*       منزل,
                           FT_Bool          movable )
  {
    FT_Error  error = FT_Err_Ok;


    FT_ASSERT( border->شروعات >= 0 );

    إذا ( border->movable )
    {
      /* move last point */
      border->points[border->انگ_points - 1] = *منزل;
    }
    وإلا
    {
      /* don't add zero-length lineto, but always add moveto */
      إذا ( border->انگ_points > (FT_UInt)border->شروعات                     &&
           FT_IS_SMALL( border->points[border->انگ_points - 1].x - منزل->x ) &&
           FT_IS_SMALL( border->points[border->انگ_points - 1].y - منزل->y ) )
        أرجع error;

      /* add one point */
      error = ft_stroke_border_grow( border, 1 );
      إذا ( !error )
      {
        FT_ويڪٽر*  vec = border->points + border->انگ_points;
        FT_Byte*    ٽيگ = border->ٽيگ_فهرست   + border->انگ_points;


        vec[0] = *منزل;
        ٽيگ[0] = FT_STROKE_ٽيگ_ON;

        border->انگ_points += 1;
      }
    }
    border->movable = movable;
    أرجع error;
  }


  ساكن FT_Error
  ft_stroke_border_conicto( FT_StrokeBorder  border,
                            FT_ويڪٽر*       control,
                            FT_ويڪٽر*       منزل )
  {
    FT_Error  error;


    FT_ASSERT( border->شروعات >= 0 );

    error = ft_stroke_border_grow( border, 2 );
    إذا ( !error )
    {
      FT_ويڪٽر*  vec = border->points + border->انگ_points;
      FT_Byte*    ٽيگ = border->ٽيگ_فهرست   + border->انگ_points;


      vec[0] = *control;
      vec[1] = *منزل;

      ٽيگ[0] = 0;
      ٽيگ[1] = FT_STROKE_ٽيگ_ON;

      border->انگ_points += 2;
    }

    border->movable = FALSE;

    أرجع error;
  }


  ساكن FT_Error
  ft_stroke_border_cubicto( FT_StrokeBorder  border,
                            FT_ويڪٽر*       control1,
                            FT_ويڪٽر*       control2,
                            FT_ويڪٽر*       منزل )
  {
    FT_Error  error;


    FT_ASSERT( border->شروعات >= 0 );

    error = ft_stroke_border_grow( border, 3 );
    إذا ( !error )
    {
      FT_ويڪٽر*  vec = border->points + border->انگ_points;
      FT_Byte*    ٽيگ = border->ٽيگ_فهرست   + border->انگ_points;


      vec[0] = *control1;
      vec[1] = *control2;
      vec[2] = *منزل;

      ٽيگ[0] = FT_STROKE_ٽيگ_CUBIC;
      ٽيگ[1] = FT_STROKE_ٽيگ_CUBIC;
      ٽيگ[2] = FT_STROKE_ٽيگ_ON;

      border->انگ_points += 3;
    }

    border->movable = FALSE;

    أرجع error;
  }


#وصف_ڪريو FT_ARC_CUBIC_ANGLE  ( FT_ANGLE_PI / 2 )


  ساكن FT_Error
  ft_stroke_border_arcto( FT_StrokeBorder  border,
                          FT_ويڪٽر*       center,
                          FT_Fixed         radius,
                          FT_Angle         angle_شروعات,
                          FT_Angle         angle_diff )
  {
    FT_Fixed   coef;
    FT_ويڪٽر  a0, a1, a2, a3;
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
    FT_ويڪٽر_From_Polar( &a0, radius, angle_شروعات );
    a1.x = FT_MulFix( -a0.y, coef );
    a1.y = FT_MulFix(  a0.x, coef );

    a0.x += center->x;
    a0.y += center->y;
    a1.x += a0.x;
    a1.y += a0.y;

    لكل ( i = 1; i <= arcs; i++ )
    {
      /* compute end and second control point */
      FT_ويڪٽر_From_Polar( &a3, radius,
                            angle_شروعات + i * angle_diff / arcs );
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
                           FT_ويڪٽر*       منزل )
  {
    /* close current open path if any ? */
    إذا ( border->شروعات >= 0 )
      ft_stroke_border_close( border, FALSE );

    border->شروعات = (FT_Int)border->انگ_points;
    border->movable = FALSE;

    أرجع ft_stroke_border_lineto( border, منزل, FALSE );
  }


  ساكن فراغ
  ft_stroke_border_init( FT_StrokeBorder  border,
                         FT_يادگيري        يادگيري )
  {
    border->يادگيري = يادگيري;
    border->points = عدم;
    border->ٽيگ_فهرست   = عدم;

    border->انگ_points = 0;
    border->max_points = 0;
    border->شروعات      = -1;
    border->valid      = FALSE;
  }


  ساكن فراغ
  ft_stroke_border_reset( FT_StrokeBorder  border )
  {
    border->انگ_points = 0;
    border->شروعات      = -1;
    border->valid      = FALSE;
  }


  ساكن فراغ
  ft_stroke_border_پورو( FT_StrokeBorder  border )
  {
    FT_يادگيري  يادگيري = border->يادگيري;


    FT_FREE( border->points );
    FT_FREE( border->ٽيگ_فهرست );

    border->انگ_points = 0;
    border->max_points = 0;
    border->شروعات      = -1;
    border->valid      = FALSE;
  }


  ساكن FT_Error
  ft_stroke_border_get_counts( FT_StrokeBorder  border,
                               FT_UInt         *anum_points,
                               FT_UInt         *anum_contours )
  {
    FT_Error  error        = FT_Err_Ok;
    FT_UInt   انگ_points   = 0;
    FT_UInt   انگ_contours = 0;

    FT_UInt     ڳڻپ      = border->انگ_points;
    FT_Byte*    ٽيگ_فهرست       = border->ٽيگ_فهرست;
    FT_Int      اندريون_contour = 0;


    لكل ( ; ڳڻپ > 0; ڳڻپ--, انگ_points++, ٽيگ_فهرست++ )
    {
      إذا ( ٽيگ_فهرست[0] & FT_STROKE_ٽيگ_BEGIN )
      {
        إذا ( اندريون_contour != 0 )
          اذهب Fail;

        اندريون_contour = 1;
      }
      وإلا إذا ( اندريون_contour == 0 )
        اذهب Fail;

      إذا ( ٽيگ_فهرست[0] & FT_STROKE_ٽيگ_پڇاڙي )
      {
        اندريون_contour = 0;
        انگ_contours++;
      }
    }

    إذا ( اندريون_contour != 0 )
      اذهب Fail;

    border->valid = TRUE;

  Exit:
    *anum_points   = انگ_points;
    *anum_contours = انگ_contours;
    أرجع error;

  Fail:
    انگ_points   = 0;
    انگ_contours = 0;
    error        = FT_THROW( Invalid_Outline );
    اذهب Exit;
  }


  ساكن فراغ
  ft_stroke_border_export( FT_StrokeBorder  border,
                           FT_Outline*      outline )
  {
    /* copy point locations */
    إذا ( border->انگ_points )
      FT_ARRAY_COPY( outline->points + outline->n_points,
                     border->points,
                     border->انگ_points );

    /* copy tags */
    {
      FT_UInt   ڳڻپ = border->انگ_points;
      FT_Byte*  اقرأ  = border->ٽيگ_فهرست;
      FT_Byte*  اكتب = outline->ٽيگ_فهرست + outline->n_points;


      لكل ( ; ڳڻپ > 0; ڳڻپ--, اقرأ++, اكتب++ )
      {
        إذا ( *اقرأ & FT_STROKE_ٽيگ_ON )
          *اكتب = FT_وڪر_ٽيگ_ON;
        وإلا إذا ( *اقرأ & FT_STROKE_ٽيگ_CUBIC )
          *اكتب = FT_وڪر_ٽيگ_CUBIC;
        وإلا
          *اكتب = FT_وڪر_ٽيگ_CONIC;
      }
    }

    /* copy contours */
    {
      FT_UInt     ڳڻپ = border->انگ_points;
      FT_Byte*    ٽيگ_فهرست  = border->ٽيگ_فهرست;
      FT_UShort*  اكتب = outline->contours + outline->n_contours;
      FT_UShort   idx   = outline->n_points;


      لكل ( ; ڳڻپ > 0; ڳڻپ--, ٽيگ_فهرست++, idx++ )
      {
        إذا ( *ٽيگ_فهرست & FT_STROKE_ٽيگ_پڇاڙي )
        {
          *اكتب++ = idx;
          outline->n_contours++;
        }
      }
    }

    outline->n_points += (FT_UShort)border->انگ_points;

    FT_ASSERT( FT_Outline_Check( outline ) == 0 );
  }


  /*************************************************************************/
  /*************************************************************************/
  /*****                                                               *****/
  /*****                           STROKER                             *****/
  /*****                                                               *****/
  /*************************************************************************/
  /*************************************************************************/

#وصف_ڪريو FT_SIDE_منزل_ROTATE( s )   ( FT_ANGLE_PI2 - (s) * FT_ANGLE_PI )

  تعريفنوع بنية  FT_StrokerRec_
  {
    FT_Angle             angle_اندريون;             /* direction into curr join */
    FT_Angle             angle_ٻاهريون;            /* direction out of join  */
    FT_ويڪٽر            center;               /* current position */
    FT_Fixed             سٽ_ڊيگهائي;          /* length of last lineto */
    FT_Bool              پهريون_نقطو;          /* is this the start? */
    FT_Bool              subpath_open;         /* is the subpath open? */
    FT_Angle             subpath_angle;        /* subpath start direction */
    FT_ويڪٽر            subpath_شروعات;        /* subpath start position */
    FT_Fixed             subpath_سٽ_ڊيگهائي;  /* subpath start lineto len */
    FT_Bool              handle_wide_strokes;  /* use wide strokes logic? */

    FT_Stroker_سٽ_Cap   سٽ_cap;
    FT_Stroker_سٽ_Join  سٽ_join;
    FT_Stroker_سٽ_Join  سٽ_join_saved;
    FT_Fixed             miter_حد_بندي;
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
    FT_يادگيري   يادگيري;
    FT_Stroker  stroker = عدم;


    إذا ( !library )
      أرجع FT_THROW( Invalid_Library_Handle );

    إذا ( !astroker )
      أرجع FT_THROW( Invalid_دليل );

    يادگيري = library->يادگيري;

    إذا ( !FT_NEW( stroker ) )
    {
      stroker->library = library;

      ft_stroke_border_init( &stroker->borders[0], يادگيري );
      ft_stroke_border_init( &stroker->borders[1], يادگيري );
    }

    *astroker = stroker;

    أرجع error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( فراغ )
  FT_Stroker_سيٽ( FT_Stroker           stroker,
                  FT_Fixed             radius,
                  FT_Stroker_سٽ_Cap   سٽ_cap,
                  FT_Stroker_سٽ_Join  سٽ_join,
                  FT_Fixed             miter_حد_بندي )
  {
    إذا ( !stroker )
      أرجع;

    stroker->radius      = radius;
    stroker->سٽ_cap    = سٽ_cap;
    stroker->سٽ_join   = سٽ_join;
    stroker->miter_حد_بندي = miter_حد_بندي;

    /* ensure miter limit has sensible value */
    إذا ( stroker->miter_حد_بندي < 0x10000L )
      stroker->miter_حد_بندي = 0x10000L;

    /* save line join style:                                           */
    /* line join style can be temporarily changed when stroking curves */
    stroker->سٽ_join_saved = سٽ_join;

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
  FT_Stroker_پورو( FT_Stroker  stroker )
  {
    إذا ( stroker )
    {
      FT_يادگيري  يادگيري = stroker->library->يادگيري;


      ft_stroke_border_پورو( &stroker->borders[0] );
      ft_stroke_border_پورو( &stroker->borders[1] );

      stroker->library = عدم;
      FT_FREE( stroker );
    }
  }


  /* create a circular arc at a corner or cap */
  ساكن FT_Error
  ft_stroker_arcto( FT_Stroker  stroker,
                    FT_Int      side )
  {
    FT_Angle         ڪل, rotate;
    FT_Fixed         radius = stroker->radius;
    FT_Error         error  = FT_Err_Ok;
    FT_StrokeBorder  border = stroker->borders + side;


    rotate = FT_SIDE_منزل_ROTATE( side );

    ڪل = FT_Angle_Diff( stroker->angle_اندريون, stroker->angle_ٻاهريون );
    إذا ( ڪل == FT_ANGLE_PI )
      ڪل = -rotate * 2;

    error = ft_stroke_border_arcto( border,
                                    &stroker->center,
                                    radius,
                                    stroker->angle_اندريون + rotate,
                                    ڪل );
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


    إذا ( stroker->سٽ_cap == FT_STROKER_LINECAP_ڦيرو )
    {
      /* add a round cap */
      stroker->angle_اندريون  = angle;
      stroker->angle_ٻاهريون = angle + FT_ANGLE_PI;

      error = ft_stroker_arcto( stroker, side );
    }
    وإلا
    {
      /* add a square or butt cap */
      FT_ويڪٽر        وچ, delta;
      FT_Fixed         radius = stroker->radius;
      FT_StrokeBorder  border = stroker->borders + side;


      /* compute middle point and first angle point */
      FT_ويڪٽر_From_Polar( &وچ, radius, angle );
      delta.x = side ?  وچ.y : -وچ.y;
      delta.y = side ? -وچ.x :  وچ.x;

      إذا ( stroker->سٽ_cap == FT_STROKER_LINECAP_چورس )
      {
        وچ.x += stroker->center.x;
        وچ.y += stroker->center.y;
      }
      وإلا  /* FT_STROKER_LINECAP_BUTT */
      {
        وچ.x  = stroker->center.x;
        وچ.y  = stroker->center.y;
      }

      delta.x  += وچ.x;
      delta.y  += وچ.y;

      error = ft_stroke_border_lineto( border, &delta, FALSE );
      إذا ( error )
        اذهب Exit;

      /* compute second angle point */
      delta.x = وچ.x - delta.x + وچ.x;
      delta.y = وچ.y - delta.y + وچ.y;

      error = ft_stroke_border_lineto( border, &delta, FALSE );
    }

  Exit:
    أرجع error;
  }


  /* process an inside corner, i.e. compute intersection */
  ساكن FT_Error
  ft_stroker_inside( FT_Stroker  stroker,
                     FT_Int      side,
                     FT_Fixed    سٽ_ڊيگهائي )
  {
    FT_StrokeBorder  border = stroker->borders + side;
    FT_Angle         phi, theta, rotate;
    FT_Fixed         ڊيگهائي;
    FT_ويڪٽر        sigma = { 0, 0 };
    FT_ويڪٽر        delta;
    FT_Error         error = FT_Err_Ok;
    FT_Bool          intersect;          /* use intersection of lines? */


    rotate = FT_SIDE_منزل_ROTATE( side );

    theta = FT_Angle_Diff( stroker->angle_اندريون, stroker->angle_ٻاهريون ) / 2;

    /* Only intersect borders if between two lineto's and both */
    /* lines are long enough (line_length is zero for curves). */
    /* Also avoid U-turns of nearly 180 degree.                */
    إذا ( !border->movable || سٽ_ڊيگهائي == 0  ||
         theta > 0x59C000 || theta < -0x59C000 )
      intersect = FALSE;
    وإلا
    {
      /* compute minimum required length of lines */
      FT_Fixed  min_ڊيگهائي;


      FT_ويڪٽر_Unit( &sigma, theta );
      min_ڊيگهائي =
        ft_pos_abs( FT_MulDiv( stroker->radius, sigma.y, sigma.x ) );

      intersect = FT_BOOL( min_ڊيگهائي                         &&
                           stroker->سٽ_ڊيگهائي >= min_ڊيگهائي &&
                           سٽ_ڊيگهائي          >= min_ڊيگهائي );
    }

    إذا ( !intersect )
    {
      FT_ويڪٽر_From_Polar( &delta, stroker->radius,
                            stroker->angle_ٻاهريون + rotate );
      delta.x += stroker->center.x;
      delta.y += stroker->center.y;

      border->movable = FALSE;
    }
    وإلا
    {
      /* compute median angle */
      phi = stroker->angle_اندريون + theta + rotate;

      ڊيگهائي = FT_DivFix( stroker->radius, sigma.x );

      FT_ويڪٽر_From_Polar( &delta, ڊيگهائي, phi );
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
                      FT_Fixed    سٽ_ڊيگهائي )
  {
    FT_StrokeBorder  border = stroker->borders + side;
    FT_Error         error;
    FT_Angle         rotate;


    إذا ( stroker->سٽ_join == FT_STROKER_LINEJOIN_ڦيرو )
      error = ft_stroker_arcto( stroker, side );
    وإلا
    {
      /* this is a mitered (pointed) or beveled (truncated) corner */
      FT_Fixed   radius = stroker->radius;
      FT_ويڪٽر  sigma = { 0, 0 };
      FT_Angle   theta = 0, phi = 0;
      FT_Bool    bevel, fixed_bevel;


      rotate = FT_SIDE_منزل_ROTATE( side );

      bevel =
        FT_BOOL( stroker->سٽ_join == FT_STROKER_LINEJOIN_BEVEL );

      fixed_bevel =
        FT_BOOL( stroker->سٽ_join != FT_STROKER_LINEJOIN_MITER_VARIABLE );

      /* check miter limit first */
      إذا ( !bevel )
      {
        theta = FT_Angle_Diff( stroker->angle_اندريون, stroker->angle_ٻاهريون ) / 2;

        إذا ( theta == FT_ANGLE_PI2 )
          theta = -rotate;

        phi = stroker->angle_اندريون + theta + rotate;

        FT_ويڪٽر_From_Polar( &sigma, stroker->miter_حد_بندي, theta );

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
          FT_ويڪٽر  delta;


          /* add bevel */
          FT_ويڪٽر_From_Polar( &delta,
                                radius,
                                stroker->angle_ٻاهريون + rotate );
          delta.x += stroker->center.x;
          delta.y += stroker->center.y;

          border->movable = FALSE;
          error = ft_stroke_border_lineto( border, &delta, FALSE );
        }
        وإلا /* variable bevel or clipped miter */
        {
          /* the miter is truncated */
          FT_ويڪٽر  وچ, delta;
          FT_Fixed   coef;


          /* compute middle point and first angle point */
          FT_ويڪٽر_From_Polar( &وچ,
                                FT_MulFix( radius, stroker->miter_حد_بندي ),
                                phi );

          coef    = FT_DivFix(  0x10000L - sigma.x, sigma.y );
          delta.x = FT_MulFix(  وچ.y, coef );
          delta.y = FT_MulFix( -وچ.x, coef );

          وچ.x += stroker->center.x;
          وچ.y += stroker->center.y;
          delta.x  += وچ.x;
          delta.y  += وچ.y;

          error = ft_stroke_border_lineto( border, &delta, FALSE );
          إذا ( error )
            اذهب Exit;

          /* compute second angle point */
          delta.x = وچ.x - delta.x + وچ.x;
          delta.y = وچ.y - delta.y + وچ.y;

          error = ft_stroke_border_lineto( border, &delta, FALSE );
          إذا ( error )
            اذهب Exit;

          /* finally, add an end point; only needed if not lineto */
          /* (line_length is zero for curves)                     */
          إذا ( سٽ_ڊيگهائي == 0 )
          {
            FT_ويڪٽر_From_Polar( &delta,
                                  radius,
                                  stroker->angle_ٻاهريون + rotate );

            delta.x += stroker->center.x;
            delta.y += stroker->center.y;

            error = ft_stroke_border_lineto( border, &delta, FALSE );
          }
        }
      }
      وإلا /* this is a miter (intersection) */
      {
        FT_Fixed   ڊيگهائي;
        FT_ويڪٽر  delta;


        ڊيگهائي = FT_MulDiv( stroker->radius, stroker->miter_حد_بندي, sigma.x );

        FT_ويڪٽر_From_Polar( &delta, ڊيگهائي, phi );
        delta.x += stroker->center.x;
        delta.y += stroker->center.y;

        error = ft_stroke_border_lineto( border, &delta, FALSE );
        إذا ( error )
          اذهب Exit;

        /* now add an end point; only needed if not lineto */
        /* (line_length is zero for curves)                */
        إذا ( سٽ_ڊيگهائي == 0 )
        {
          FT_ويڪٽر_From_Polar( &delta,
                                stroker->radius,
                                stroker->angle_ٻاهريون + rotate );
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
  ft_stroker_process_ڪنڊ( FT_Stroker  stroker,
                             FT_Fixed    سٽ_ڊيگهائي )
  {
    FT_Error  error = FT_Err_Ok;
    FT_Angle  turn;
    FT_Int    inside_side;


    turn = FT_Angle_Diff( stroker->angle_اندريون, stroker->angle_ٻاهريون );

    /* no specific corner processing is required if the turn is 0 */
    إذا ( turn == 0 )
      اذهب Exit;

    /* when we turn to the right, the inside side is 0 */
    /* otherwise, the inside side is 1 */
    inside_side = ( turn < 0 );

    /* process the inside side */
    error = ft_stroker_inside( stroker, inside_side, سٽ_ڊيگهائي );
    إذا ( error )
      اذهب Exit;

    /* process the outside side */
    error = ft_stroker_outside( stroker, !inside_side, سٽ_ڊيگهائي );

  Exit:
    أرجع error;
  }


  /* add two points to the left and right borders corresponding to the */
  /* start of the subpath                                              */
  ساكن FT_Error
  ft_stroker_subpath_شروعات( FT_Stroker  stroker,
                            FT_Angle    شروعات_angle,
                            FT_Fixed    سٽ_ڊيگهائي )
  {
    FT_ويڪٽر        delta;
    FT_ويڪٽر        نقطو;
    FT_Error         error;
    FT_StrokeBorder  border;


    FT_ويڪٽر_From_Polar( &delta, stroker->radius,
                          شروعات_angle + FT_ANGLE_PI2 );

    نقطو.x = stroker->center.x + delta.x;
    نقطو.y = stroker->center.y + delta.y;

    border = stroker->borders;
    error = ft_stroke_border_moveto( border, &نقطو );
    إذا ( error )
      اذهب Exit;

    نقطو.x = stroker->center.x - delta.x;
    نقطو.y = stroker->center.y - delta.y;

    border++;
    error = ft_stroke_border_moveto( border, &نقطو );

    /* save angle, position, and line length for last join */
    /* (line_length is zero for curves)                    */
    stroker->subpath_angle       = شروعات_angle;
    stroker->پهريون_نقطو         = FALSE;
    stroker->subpath_سٽ_ڊيگهائي = سٽ_ڊيگهائي;

  Exit:
    أرجع error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Stroker_سٽ_منزل( FT_Stroker  stroker,
                     FT_ويڪٽر*  منزل )
  {
    FT_Error         error = FT_Err_Ok;
    FT_StrokeBorder  border;
    FT_ويڪٽر        delta;
    FT_Angle         angle;
    FT_Int           side;
    FT_Fixed         سٽ_ڊيگهائي;


    إذا ( !stroker || !منزل )
      أرجع FT_THROW( Invalid_دليل );

    delta.x = منزل->x - stroker->center.x;
    delta.y = منزل->y - stroker->center.y;

    /* a zero-length lineto is a no-op; avoid creating a spurious corner */
    إذا ( delta.x == 0 && delta.y == 0 )
       اذهب Exit;

    /* compute length of line */
    سٽ_ڊيگهائي = FT_ويڪٽر_ڊيگهائي( &delta );

    angle = FT_Atan2( delta.x, delta.y );
    FT_ويڪٽر_From_Polar( &delta, stroker->radius, angle + FT_ANGLE_PI2 );

    /* process corner if necessary */
    إذا ( stroker->پهريون_نقطو )
    {
      /* This is the first segment of a subpath.  We need to     */
      /* add a point to each border at their respective starting */
      /* point locations.                                        */
      error = ft_stroker_subpath_شروعات( stroker, angle, سٽ_ڊيگهائي );
      إذا ( error )
        اذهب Exit;
    }
    وإلا
    {
      /* process the current corner */
      stroker->angle_ٻاهريون = angle;
      error = ft_stroker_process_ڪنڊ( stroker, سٽ_ڊيگهائي );
      إذا ( error )
        اذهب Exit;
    }

    /* now add a line segment to both the `inside' and `outside' paths */
    لكل ( border = stroker->borders, side = 1; side >= 0; side--, border++ )
    {
      FT_ويڪٽر  نقطو;


      نقطو.x = منزل->x + delta.x;
      نقطو.y = منزل->y + delta.y;

      /* the ends of lineto borders are movable */
      error = ft_stroke_border_lineto( border, &نقطو, TRUE );
      إذا ( error )
        اذهب Exit;

      delta.x = -delta.x;
      delta.y = -delta.y;
    }

    stroker->angle_اندريون    = angle;
    stroker->center      = *منزل;
    stroker->سٽ_ڊيگهائي = سٽ_ڊيگهائي;

  Exit:
    أرجع error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Stroker_Conic_منزل( FT_Stroker  stroker,
                      FT_ويڪٽر*  control,
                      FT_ويڪٽر*  منزل )
  {
    FT_Error    error = FT_Err_Ok;
    FT_ويڪٽر   bez_ڍير[34];
    FT_ويڪٽر*  arc;
    FT_ويڪٽر*  حد_بندي = bez_ڍير + 30;
    FT_Bool     پهريون_arc = TRUE;


    إذا ( !stroker || !control || !منزل )
    {
      error = FT_THROW( Invalid_دليل );
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

    arc    = bez_ڍير;
    arc[0] = *منزل;
    arc[1] = *control;
    arc[2] = stroker->center;

    افعل
    {
      FT_Angle  angle_اندريون, angle_ٻاهريون;


      /* initialize with current direction */
      angle_اندريون = angle_ٻاهريون = stroker->angle_اندريون;

      إذا ( arc < حد_بندي                                             &&
           !ft_conic_is_small_enough( arc, &angle_اندريون, &angle_ٻاهريون ) )
      {
        إذا ( stroker->پهريون_نقطو )
          stroker->angle_اندريون = angle_اندريون;

        ft_conic_ورهايو( arc );
        arc += 2;
        استمر;
      }

      إذا ( پهريون_arc )
      {
        پهريون_arc = FALSE;

        /* process corner if necessary */
        إذا ( stroker->پهريون_نقطو )
          error = ft_stroker_subpath_شروعات( stroker, angle_اندريون, 0 );
        وإلا
        {
          stroker->angle_ٻاهريون = angle_اندريون;
          error = ft_stroker_process_ڪنڊ( stroker, 0 );
        }
      }
      وإلا إذا ( ft_pos_abs( FT_Angle_Diff( stroker->angle_اندريون, angle_اندريون ) ) >
                  FT_SMALL_CONIC_حد / 4                             )
      {
        /* if the deviation from one arc to the next is too great, */
        /* add a round corner                                      */
        stroker->center    = arc[2];
        stroker->angle_ٻاهريون = angle_اندريون;
        stroker->سٽ_join = FT_STROKER_LINEJOIN_ڦيرو;

        error = ft_stroker_process_ڪنڊ( stroker, 0 );

        /* reinstate line join style */
        stroker->سٽ_join = stroker->سٽ_join_saved;
      }

      إذا ( error )
        اذهب Exit;

      /* the arc's angle is small enough; we can add it directly to each */
      /* border                                                          */
      {
        FT_ويڪٽر        ctrl, پڇاڙي;
        FT_Angle         theta, phi, rotate, alpha0 = 0;
        FT_Fixed         ڊيگهائي;
        FT_StrokeBorder  border;
        FT_Int           side;


        theta  = FT_Angle_Diff( angle_اندريون, angle_ٻاهريون ) / 2;
        phi    = angle_اندريون + theta;
        ڊيگهائي = FT_DivFix( stroker->radius, FT_Cos( theta ) );

        /* compute direction of original arc */
        إذا ( stroker->handle_wide_strokes )
          alpha0 = FT_Atan2( arc[0].x - arc[2].x, arc[0].y - arc[2].y );

        لكل ( border = stroker->borders, side = 0;
              side <= 1;
              side++, border++ )
        {
          rotate = FT_SIDE_منزل_ROTATE( side );

          /* compute control point */
          FT_ويڪٽر_From_Polar( &ctrl, ڊيگهائي, phi + rotate );
          ctrl.x += arc[1].x;
          ctrl.y += arc[1].y;

          /* compute end point */
          FT_ويڪٽر_From_Polar( &پڇاڙي, stroker->radius, angle_ٻاهريون + rotate );
          پڇاڙي.x += arc[0].x;
          پڇاڙي.y += arc[0].y;

          إذا ( stroker->handle_wide_strokes )
          {
            FT_ويڪٽر  شروعات;
            FT_Angle   alpha1;


            /* determine whether the border radius is greater than the */
            /* radius of curvature of the original arc                 */
            شروعات = border->points[border->انگ_points - 1];

            alpha1 = FT_Atan2( پڇاڙي.x - شروعات.x, پڇاڙي.y - شروعات.y );

            /* is the direction of the border arc opposite to */
            /* that of the original arc? */
            إذا ( ft_pos_abs( FT_Angle_Diff( alpha0, alpha1 ) ) >
                   FT_ANGLE_PI / 2                             )
            {
              FT_Angle   beta, gamma;
              FT_ويڪٽر  bvec, delta;
              FT_Fixed   blen, sinA, sinB, alen;


              /* use the sine rule to find the intersection point */
              beta  = FT_Atan2( arc[2].x - شروعات.x, arc[2].y - شروعات.y );
              gamma = FT_Atan2( arc[0].x - پڇاڙي.x,   arc[0].y - پڇاڙي.y );

              bvec.x = پڇاڙي.x - شروعات.x;
              bvec.y = پڇاڙي.y - شروعات.y;

              blen = FT_ويڪٽر_ڊيگهائي( &bvec );

              sinA = ft_pos_abs( FT_Sin( alpha1 - gamma ) );
              sinB = ft_pos_abs( FT_Sin( beta - gamma ) );

              alen = FT_MulDiv( blen, sinA, sinB );

              FT_ويڪٽر_From_Polar( &delta, alen, beta );
              delta.x += شروعات.x;
              delta.y += شروعات.y;

              /* circumnavigate the negative sector backwards */
              border->movable = FALSE;
              error = ft_stroke_border_lineto( border, &delta, FALSE );
              إذا ( error )
                اذهب Exit;
              error = ft_stroke_border_lineto( border, &پڇاڙي, FALSE );
              إذا ( error )
                اذهب Exit;
              error = ft_stroke_border_conicto( border, &ctrl, &شروعات );
              إذا ( error )
                اذهب Exit;
              /* and then move to the endpoint */
              error = ft_stroke_border_lineto( border, &پڇاڙي, FALSE );
              إذا ( error )
                اذهب Exit;

              استمر;
            }

            /* else fall through */
          }

          /* simply add an arc */
          error = ft_stroke_border_conicto( border, &ctrl, &پڇاڙي );
          إذا ( error )
            اذهب Exit;
        }
      }

      stroker->angle_اندريون = angle_ٻاهريون;

      إذا ( arc == bez_ڍير )
        اكسر;
      arc -= 2;
    } طالما ( 1 );

    stroker->center      = *منزل;
    stroker->سٽ_ڊيگهائي = 0;

  Exit:
    أرجع error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Stroker_Cubic_منزل( FT_Stroker  stroker,
                      FT_ويڪٽر*  control1,
                      FT_ويڪٽر*  control2,
                      FT_ويڪٽر*  منزل )
  {
    FT_Error    error = FT_Err_Ok;
    FT_ويڪٽر   bez_ڍير[37];
    FT_ويڪٽر*  arc;
    FT_ويڪٽر*  حد_بندي = bez_ڍير + 32;
    FT_Bool     پهريون_arc = TRUE;


    إذا ( !stroker || !control1 || !control2 || !منزل )
    {
      error = FT_THROW( Invalid_دليل );
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

    arc    = bez_ڍير;
    arc[0] = *منزل;
    arc[1] = *control2;
    arc[2] = *control1;
    arc[3] = stroker->center;

    افعل
    {
      FT_Angle  angle_اندريون, angle_mid, angle_ٻاهريون;


      /* initialize with current direction */
      angle_اندريون = angle_ٻاهريون = angle_mid = stroker->angle_اندريون;

      إذا ( arc < حد_بندي                                         &&
           !ft_cubic_is_small_enough( arc, &angle_اندريون,
                                      &angle_mid, &angle_ٻاهريون ) )
      {
        إذا ( stroker->پهريون_نقطو )
          stroker->angle_اندريون = angle_اندريون;

        ft_cubic_ورهايو( arc );
        arc += 3;
        استمر;
      }

      إذا ( پهريون_arc )
      {
        پهريون_arc = FALSE;

        /* process corner if necessary */
        إذا ( stroker->پهريون_نقطو )
          error = ft_stroker_subpath_شروعات( stroker, angle_اندريون, 0 );
        وإلا
        {
          stroker->angle_ٻاهريون = angle_اندريون;
          error = ft_stroker_process_ڪنڊ( stroker, 0 );
        }
      }
      وإلا إذا ( ft_pos_abs( FT_Angle_Diff( stroker->angle_اندريون, angle_اندريون ) ) >
                  FT_SMALL_CUBIC_حد / 4                             )
      {
        /* if the deviation from one arc to the next is too great, */
        /* add a round corner                                      */
        stroker->center    = arc[3];
        stroker->angle_ٻاهريون = angle_اندريون;
        stroker->سٽ_join = FT_STROKER_LINEJOIN_ڦيرو;

        error = ft_stroker_process_ڪنڊ( stroker, 0 );

        /* reinstate line join style */
        stroker->سٽ_join = stroker->سٽ_join_saved;
      }

      إذا ( error )
        اذهب Exit;

      /* the arc's angle is small enough; we can add it directly to each */
      /* border                                                          */
      {
        FT_ويڪٽر        ctrl1, ctrl2, پڇاڙي;
        FT_Angle         theta1, phi1, theta2, phi2, rotate, alpha0 = 0;
        FT_Fixed         length1, length2;
        FT_StrokeBorder  border;
        FT_Int           side;


        theta1  = FT_Angle_Diff( angle_اندريون,  angle_mid ) / 2;
        theta2  = FT_Angle_Diff( angle_mid, angle_ٻاهريون ) / 2;
        phi1    = ft_angle_سراسري( angle_اندريون,  angle_mid );
        phi2    = ft_angle_سراسري( angle_mid, angle_ٻاهريون );
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
          FT_ويڪٽر_From_Polar( &ctrl1, length1, phi1 + rotate );
          ctrl1.x += arc[2].x;
          ctrl1.y += arc[2].y;

          FT_ويڪٽر_From_Polar( &ctrl2, length2, phi2 + rotate );
          ctrl2.x += arc[1].x;
          ctrl2.y += arc[1].y;

          /* compute end point */
          FT_ويڪٽر_From_Polar( &پڇاڙي, stroker->radius, angle_ٻاهريون + rotate );
          پڇاڙي.x += arc[0].x;
          پڇاڙي.y += arc[0].y;

          إذا ( stroker->handle_wide_strokes )
          {
            FT_ويڪٽر  شروعات;
            FT_Angle   alpha1;


            /* determine whether the border radius is greater than the */
            /* radius of curvature of the original arc                 */
            شروعات = border->points[border->انگ_points - 1];

            alpha1 = FT_Atan2( پڇاڙي.x - شروعات.x, پڇاڙي.y - شروعات.y );

            /* is the direction of the border arc opposite to */
            /* that of the original arc? */
            إذا ( ft_pos_abs( FT_Angle_Diff( alpha0, alpha1 ) ) >
                   FT_ANGLE_PI / 2                             )
            {
              FT_Angle   beta, gamma;
              FT_ويڪٽر  bvec, delta;
              FT_Fixed   blen, sinA, sinB, alen;


              /* use the sine rule to find the intersection point */
              beta  = FT_Atan2( arc[3].x - شروعات.x, arc[3].y - شروعات.y );
              gamma = FT_Atan2( arc[0].x - پڇاڙي.x,   arc[0].y - پڇاڙي.y );

              bvec.x = پڇاڙي.x - شروعات.x;
              bvec.y = پڇاڙي.y - شروعات.y;

              blen = FT_ويڪٽر_ڊيگهائي( &bvec );

              sinA = ft_pos_abs( FT_Sin( alpha1 - gamma ) );
              sinB = ft_pos_abs( FT_Sin( beta - gamma ) );

              alen = FT_MulDiv( blen, sinA, sinB );

              FT_ويڪٽر_From_Polar( &delta, alen, beta );
              delta.x += شروعات.x;
              delta.y += شروعات.y;

              /* circumnavigate the negative sector backwards */
              border->movable = FALSE;
              error = ft_stroke_border_lineto( border, &delta, FALSE );
              إذا ( error )
                اذهب Exit;
              error = ft_stroke_border_lineto( border, &پڇاڙي, FALSE );
              إذا ( error )
                اذهب Exit;
              error = ft_stroke_border_cubicto( border,
                                                &ctrl2,
                                                &ctrl1,
                                                &شروعات );
              إذا ( error )
                اذهب Exit;
              /* and then move to the endpoint */
              error = ft_stroke_border_lineto( border, &پڇاڙي, FALSE );
              إذا ( error )
                اذهب Exit;

              استمر;
            }

            /* else fall through */
          }

          /* simply add an arc */
          error = ft_stroke_border_cubicto( border, &ctrl1, &ctrl2, &پڇاڙي );
          إذا ( error )
            اذهب Exit;
        }
      }

      stroker->angle_اندريون = angle_ٻاهريون;

      إذا ( arc == bez_ڍير )
        اكسر;
      arc -= 3;
    } طالما ( 1 );

    stroker->center      = *منزل;
    stroker->سٽ_ڊيگهائي = 0;

  Exit:
    أرجع error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Stroker_Begin_Sub_رستو( FT_Stroker  stroker,
                           FT_ويڪٽر*  منزل,
                           FT_Bool     open )
  {
    إذا ( !stroker || !منزل )
      أرجع FT_THROW( Invalid_دليل );

    /* We cannot process the first point, because there is not enough      */
    /* information regarding its corner/cap.  The latter will be processed */
    /* in the `FT_Stroker_EndSubPath' routine.                             */
    /*                                                                     */
    stroker->پهريون_نقطو  = TRUE;
    stroker->center       = *منزل;
    stroker->subpath_open = open;

    /* Determine if we need to check whether the border radius is greater */
    /* than the radius of curvature of a curve, to handle this case       */
    /* specially.  This is only required if bevel joins or butt caps may  */
    /* be created, because round & miter joins and round & square caps    */
    /* cover the negative sector created with wide strokes.               */
    stroker->handle_wide_strokes =
      FT_BOOL( stroker->سٽ_join != FT_STROKER_LINEJOIN_ڦيرو  ||
               ( stroker->subpath_open                        &&
                 stroker->سٽ_cap == FT_STROKER_LINECAP_BUTT ) );

    /* record the subpath start point for each border */
    stroker->subpath_شروعات = *منزل;

    stroker->angle_اندريون = 0;

    أرجع FT_Err_Ok;
  }


  ساكن FT_Error
  ft_stroker_جوڙيو_ابتو_کاٻو( FT_Stroker  stroker,
                               FT_Bool     open )
  {
    FT_StrokeBorder  ساڄو = stroker->borders + 0;
    FT_StrokeBorder  کاٻو  = stroker->borders + 1;
    FT_Int           new_points;
    FT_Error         error = FT_Err_Ok;


    FT_ASSERT( کاٻو->شروعات >= 0 );

    new_points = (FT_Int)کاٻو->انگ_points - کاٻو->شروعات;
    إذا ( new_points > 0 )
    {
      error = ft_stroke_border_grow( ساڄو, (FT_UInt)new_points );
      إذا ( error )
        اذهب Exit;

      {
        FT_ويڪٽر*  dst_نقطو = ساڄو->points + ساڄو->انگ_points;
        FT_Byte*    dst_ٽيگ   = ساڄو->ٽيگ_فهرست   + ساڄو->انگ_points;
        FT_ويڪٽر*  src_نقطو = کاٻو->points  + کاٻو->انگ_points - 1;
        FT_Byte*    src_ٽيگ   = کاٻو->ٽيگ_فهرست    + کاٻو->انگ_points - 1;


        طالما ( src_نقطو >= کاٻو->points + کاٻو->شروعات )
        {
          *dst_نقطو = *src_نقطو;
          *dst_ٽيگ   = *src_ٽيگ;

          إذا ( open )
            dst_ٽيگ[0] &= ~FT_STROKE_ٽيگ_BEGIN_پڇاڙي;
          وإلا
          {
            FT_Byte  ttag =
                       (FT_Byte)( dst_ٽيگ[0] & FT_STROKE_ٽيگ_BEGIN_پڇاڙي );


            /* switch begin/end tags if necessary */
            إذا ( ttag == FT_STROKE_ٽيگ_BEGIN ||
                 ttag == FT_STROKE_ٽيگ_پڇاڙي   )
              dst_ٽيگ[0] ^= FT_STROKE_ٽيگ_BEGIN_پڇاڙي;
          }

          src_نقطو--;
          src_ٽيگ--;
          dst_نقطو++;
          dst_ٽيگ++;
        }
      }

      کاٻو->انگ_points   = (FT_UInt)کاٻو->شروعات;
      ساڄو->انگ_points += (FT_UInt)new_points;

      ساڄو->movable = FALSE;
      کاٻو->movable  = FALSE;
    }

  Exit:
    أرجع error;
  }


  /* documentation is in ftstroke.h */

  /* there's a lot of magic in this function! */
  FT_EXPORT_DEF( FT_Error )
  FT_Stroker_پڇاڙي_Sub_رستو( FT_Stroker  stroker )
  {
    FT_Error  error = FT_Err_Ok;


    إذا ( !stroker )
    {
      error = FT_THROW( Invalid_دليل );
      اذهب Exit;
    }

    /* don't try to end the path if no segments have been generated */
    إذا ( stroker->پهريون_نقطو )
      اذهب Exit;

    إذا ( stroker->subpath_open )
    {
      FT_StrokeBorder  ساڄو = stroker->borders;


      /* All right, this is an opened path, we need to add a cap between */
      /* right & left, add the reverse of left, then add a final cap     */
      /* between left & right.                                           */
      error = ft_stroker_cap( stroker, stroker->angle_اندريون, 0 );
      إذا ( error )
        اذهب Exit;

      /* add reversed points from `left' to `right' */
      error = ft_stroker_جوڙيو_ابتو_کاٻو( stroker, TRUE );
      إذا ( error )
        اذهب Exit;

      /* now add the final cap */
      stroker->center = stroker->subpath_شروعات;
      error = ft_stroker_cap( stroker,
                              stroker->subpath_angle + FT_ANGLE_PI, 0 );
      إذا ( error )
        اذهب Exit;

      /* Now end the right subpath accordingly.  The left one is */
      /* rewind and doesn't need further processing.             */
      ft_stroke_border_close( ساڄو, FALSE );
    }
    وإلا
    {
      /* close the path if needed */
      إذا ( !FT_IS_SMALL( stroker->center.x - stroker->subpath_شروعات.x ) ||
           !FT_IS_SMALL( stroker->center.y - stroker->subpath_شروعات.y ) )
      {
         error = FT_Stroker_سٽ_منزل( stroker, &stroker->subpath_شروعات );
         إذا ( error )
           اذهب Exit;
      }

      /* process the corner */
      stroker->angle_ٻاهريون = stroker->subpath_angle;

      error = ft_stroker_process_ڪنڊ( stroker,
                                         stroker->subpath_سٽ_ڊيگهائي );
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
    FT_UInt   انگ_points = 0, انگ_contours = 0;
    FT_Error  error;


    إذا ( !stroker || border > 1 )
    {
      error = FT_THROW( Invalid_دليل );
      اذهب Exit;
    }

    error = ft_stroke_border_get_counts( stroker->borders + border,
                                         &انگ_points, &انگ_contours );
  Exit:
    إذا ( anum_points )
      *anum_points = انگ_points;

    إذا ( anum_contours )
      *anum_contours = انگ_contours;

    أرجع error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Stroker_GetCounts( FT_Stroker  stroker,
                        FT_UInt    *anum_points,
                        FT_UInt    *anum_contours )
  {
    FT_UInt   count1, count2, انگ_points   = 0;
    FT_UInt   count3, count4, انگ_contours = 0;
    FT_Error  error;


    إذا ( !stroker )
    {
      error = FT_THROW( Invalid_دليل );
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

    انگ_points   = count1 + count3;
    انگ_contours = count2 + count4;

  Exit:
    إذا ( anum_points )
      *anum_points   = انگ_points;

    إذا ( anum_contours )
      *anum_contours = انگ_contours;

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

    إذا ( border == FT_STROKER_BORDER_کاٻو  ||
         border == FT_STROKER_BORDER_ساڄو )
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
    FT_Stroker_ExportBorder( stroker, FT_STROKER_BORDER_کاٻو, outline );
    FT_Stroker_ExportBorder( stroker, FT_STROKER_BORDER_ساڄو, outline );
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
    FT_ويڪٽر   v_آخري;
    FT_ويڪٽر   v_control;
    FT_ويڪٽر   v_شروعات;

    FT_ويڪٽر*  نقطو;
    FT_ويڪٽر*  حد_بندي;
    FT_Byte*    ٽيگ_فهرست;

    FT_Error    error;

    FT_Int      n;         /* index of contour in outline     */
    FT_Int      پهريون;     /* index of first point in contour */
    FT_Int      آخري;      /* index of last point in contour  */

    FT_Int      ٽيگ;       /* current point's state           */


    إذا ( !outline )
      أرجع FT_THROW( Invalid_Outline );

    إذا ( !stroker )
      أرجع FT_THROW( Invalid_دليل );

    FT_Stroker_Rewind( stroker );

    آخري = -1;
    لكل ( n = 0; n < outline->n_contours; n++ )
    {
      پهريون = آخري + 1;
      آخري  = outline->contours[n];

      /* skip empty points; we don't stroke these */
      إذا ( آخري <= پهريون )
        استمر;

      حد_بندي = outline->points + آخري;

      v_شروعات = outline->points[پهريون];
      v_آخري  = outline->points[آخري];

      v_control = v_شروعات;

      نقطو = outline->points + پهريون;
      ٽيگ_فهرست  = outline->ٽيگ_فهرست   + پهريون;
      ٽيگ   = FT_وڪر_ٽيگ( ٽيگ_فهرست[0] );

      /* check first point to determine origin */
      إذا ( ٽيگ != FT_وڪر_ٽيگ_ON )
      {
        FT_Int  آخري_ٽيگ = FT_وڪر_ٽيگ( outline->ٽيگ_فهرست[آخري] );


        /* First point is a control point.  Yes, this happens. */
        إذا ( آخري_ٽيگ == FT_وڪر_ٽيگ_ON )
        {
          /* start at last point if it is on the curve */
          v_شروعات = v_آخري;
          حد_بندي--;
        }
        وإلا
        {
          إذا ( آخري_ٽيگ != ٽيگ )
            اذهب Invalid_Outline;

          /* if both first and last points are controls, */
          /* start at their middle                       */
          v_شروعات.x = ( v_شروعات.x + v_آخري.x ) / 2;
          v_شروعات.y = ( v_شروعات.y + v_آخري.y ) / 2;
        }
        نقطو--;
        ٽيگ_فهرست--;
      }

      error = FT_Stroker_Begin_Sub_رستو( stroker, &v_شروعات, opened );
      إذا ( error )
        اذهب Exit;

      طالما ( نقطو < حد_بندي )
      {
        نقطو++;
        ٽيگ_فهرست++;

        ٽيگ = FT_وڪر_ٽيگ( ٽيگ_فهرست[0] );
        بدل ( ٽيگ )
        {
        حالة FT_وڪر_ٽيگ_ON:  /* emit a single line_to */
          {
            FT_ويڪٽر  vec;


            vec.x = نقطو->x;
            vec.y = نقطو->y;

            error = FT_Stroker_سٽ_منزل( stroker, &vec );
            إذا ( error )
              اذهب Exit;
            استمر;
          }

        حالة FT_وڪر_ٽيگ_CONIC:  /* consume conic arcs */
          v_control.x = نقطو->x;
          v_control.y = نقطو->y;

        Do_Conic:
          إذا ( نقطو < حد_بندي )
          {
            FT_ويڪٽر  vec;
            FT_ويڪٽر  v_وچ;


            نقطو++;
            ٽيگ_فهرست++;
            ٽيگ = FT_وڪر_ٽيگ( ٽيگ_فهرست[0] );

            vec = نقطو[0];

            إذا ( ٽيگ == FT_وڪر_ٽيگ_ON )
            {
              error = FT_Stroker_Conic_منزل( stroker, &v_control, &vec );
              إذا ( error )
                اذهب Exit;
              استمر;
            }

            إذا ( ٽيگ != FT_وڪر_ٽيگ_CONIC )
              اذهب Invalid_Outline;

            v_وچ.x = ( v_control.x + vec.x ) / 2;
            v_وچ.y = ( v_control.y + vec.y ) / 2;

            error = FT_Stroker_Conic_منزل( stroker, &v_control, &v_وچ );
            إذا ( error )
              اذهب Exit;

            v_control = vec;
            اذهب Do_Conic;
          }

          error = FT_Stroker_Conic_منزل( stroker, &v_control, &v_شروعات );
          اذهب Close;

        افتراضي:  /* FT_CURVE_TAG_CUBIC */
          {
            FT_ويڪٽر  vec1, vec2;


            إذا ( نقطو + 1 > حد_بندي                             ||
                 FT_وڪر_ٽيگ( ٽيگ_فهرست[1] ) != FT_وڪر_ٽيگ_CUBIC )
              اذهب Invalid_Outline;

            نقطو += 2;
            ٽيگ_فهرست  += 2;

            vec1 = نقطو[-2];
            vec2 = نقطو[-1];

            إذا ( نقطو <= حد_بندي )
            {
              FT_ويڪٽر  vec;
              FT_ويڪٽر  v_وچ;


              ٽيگ = FT_وڪر_ٽيگ( ٽيگ_فهرست[0] );
              vec = نقطو[0];

              إذا ( ٽيگ == FT_وڪر_ٽيگ_CUBIC )
              {
                v_وچ.x = ( vec2.x + vec.x ) / 2;
                v_وچ.y = ( vec2.y + vec.y ) / 2;

                vec = v_وچ;
              }
              وإلا إذا ( ٽيگ != FT_وڪر_ٽيگ_ON )
                اذهب Invalid_Outline;

              error = FT_Stroker_Cubic_منزل( stroker, &vec1, &vec2, &vec );
              إذا ( error )
                اذهب Exit;

              إذا ( ٽيگ == FT_وڪر_ٽيگ_CUBIC )
              {
                نقطو--;
                ٽيگ_فهرست--;
              }
              استمر;
            }

            error = FT_Stroker_Cubic_منزل( stroker, &vec1, &vec2, &v_شروعات );
            اذهب Close;
          }
        }
      }

    Close:
      إذا ( error )
        اذهب Exit;

      error = FT_Stroker_پڇاڙي_Sub_رستو( stroker );
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
    FT_Error  error = FT_THROW( Invalid_دليل );
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
      FT_UInt          انگ_points, انگ_contours;


      error = FT_Stroker_ParseOutline( stroker, outline, FALSE );
      إذا ( error )
        اذهب Fail;

      error = FT_Stroker_GetCounts( stroker, &انگ_points, &انگ_contours );
      إذا ( error )
        اذهب Fail;

      FT_Outline_پورو( glyph->library, outline );

      error = FT_Outline_New( glyph->library,
                              انگ_points,
                              (FT_Int)انگ_contours,
                              outline );
      إذا ( error )
        اذهب Fail;

      outline->n_points   = 0;
      outline->n_contours = 0;

      FT_Stroker_Export( stroker, outline );
    }

    إذا ( destroy )
      FT_پورو_Glyph( *pglyph );

    *pglyph = glyph;
    اذهب Exit;

  Fail:
    FT_پورو_Glyph( glyph );
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
    FT_Error  error = FT_THROW( Invalid_دليل );
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
      FT_UInt           انگ_points, انگ_contours;


      border = FT_Outline_GetOutsideBorder( outline );
      إذا ( inside )
      {
        إذا ( border == FT_STROKER_BORDER_کاٻو )
          border = FT_STROKER_BORDER_ساڄو;
        وإلا
          border = FT_STROKER_BORDER_کاٻو;
      }

      error = FT_Stroker_ParseOutline( stroker, outline, FALSE );
      إذا ( error )
        اذهب Fail;

      FT_Stroker_GetBorderCounts( stroker, border,
                                  &انگ_points, &انگ_contours );

      FT_Outline_پورو( glyph->library, outline );

      error = FT_Outline_New( glyph->library,
                              انگ_points,
                              (FT_Int)انگ_contours,
                              outline );
      إذا ( error )
        اذهب Fail;

      outline->n_points   = 0;
      outline->n_contours = 0;

      FT_Stroker_ExportBorder( stroker, border, outline );
    }

    إذا ( destroy )
      FT_پورو_Glyph( *pglyph );

    *pglyph = glyph;
    اذهب Exit;

  Fail:
    FT_پورو_Glyph( glyph );
    glyph = عدم;

    إذا ( !destroy )
      *pglyph = عدم;

  Exit:
    أرجع error;
  }


/*پڇاڙي*/
