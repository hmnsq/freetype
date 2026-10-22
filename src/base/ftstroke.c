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


#अन्तर्भाव <freetype/ftstroke.h>
#अन्तर्भाव <freetype/fttrigon.h>
#अन्तर्भाव <freetype/ftoutln.h>
#अन्तर्भाव <freetype/internal/ftmemory.h>
#अन्तर्भाव <freetype/internal/ftdebug.h>
#अन्तर्भाव <freetype/internal/ftobjs.h>


  /* declare an extern to access `ft_outline_glyph_class' globally */
  /* allocated  in `ftglyph.c'                                     */
  FT_CALLBACK_सारणी ध्रुव FT_Glyph_Class  ft_outline_glyph_class;


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_StrokerBorder )
  FT_Outline_GetInsideBorder( FT_Outline*  outline )
  {
    FT_Orientation  o = FT_Outline_Get_Orientation( outline );


    प्रत्यागम o == FT_ORIENTATION_TRUETYPE ? FT_STROKER_BORDER_दक्षिणः
                                        : FT_STROKER_BORDER_वामः;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_StrokerBorder )
  FT_Outline_GetOutsideBorder( FT_Outline*  outline )
  {
    FT_Orientation  o = FT_Outline_Get_Orientation( outline );


    प्रत्यागम o == FT_ORIENTATION_TRUETYPE ? FT_STROKER_BORDER_वामः
                                        : FT_STROKER_BORDER_दक्षिणः;
  }


  /*************************************************************************/
  /*************************************************************************/
  /*****                                                               *****/
  /*****                      BEZIER COMPUTATIONS                      *****/
  /*****                                                               *****/
  /*************************************************************************/
  /*************************************************************************/

#परिभाषय FT_SMALL_CONIC_देहली  ( FT_ANGLE_PI / 6 )
#परिभाषय FT_SMALL_CUBIC_देहली  ( FT_ANGLE_PI / 8 )

#परिभाषय FT_EPSILON  2

#परिभाषय FT_IS_SMALL( x )  ( (x) > -FT_EPSILON && (x) < FT_EPSILON )


  स्थिर FT_Pos
  ft_pos_abs( FT_Pos  x )
  {
    प्रत्यागम x >= 0 ? x : -x;
  }


  स्थिर शून्य
  ft_conic_विभज( FT_सदिशम्*  आधारः )
  {
    FT_Pos  a, b;


    आधारः[4].x = आधारः[2].x;
    a = आधारः[0].x + आधारः[1].x;
    b = आधारः[1].x + आधारः[2].x;
    आधारः[3].x = b >> 1;
    आधारः[2].x = ( a + b ) >> 2;
    आधारः[1].x = a >> 1;

    आधारः[4].y = आधारः[2].y;
    a = आधारः[0].y + आधारः[1].y;
    b = आधारः[1].y + आधारः[2].y;
    आधारः[3].y = b >> 1;
    आधारः[2].y = ( a + b ) >> 2;
    आधारः[1].y = a >> 1;
  }


  स्थिर FT_Bool
  ft_conic_is_small_enough( FT_सदिशम्*  आधारः,
                            FT_Angle   *angle_अन्तर्,
                            FT_Angle   *angle_बहिर्गमनम् )
  {
    FT_सदिशम्  d1, d2;
    FT_Angle   theta;
    FT_Int     close1, close2;


    d1.x = आधारः[1].x - आधारः[2].x;
    d1.y = आधारः[1].y - आधारः[2].y;
    d2.x = आधारः[0].x - आधारः[1].x;
    d2.y = आधारः[0].y - आधारः[1].y;

    close1 = FT_IS_SMALL( d1.x ) && FT_IS_SMALL( d1.y );
    close2 = FT_IS_SMALL( d2.x ) && FT_IS_SMALL( d2.y );

    यदि ( close1 )
    {
      यदि ( close2 )
      {
        /* basically a point;                      */
        /* do nothing to retain original direction */
      }
      अन्यथा
      {
        *angle_अन्तर्  =
        *angle_बहिर्गमनम् = FT_Atan2( d2.x, d2.y );
      }
    }
    अन्यथा /* !close1 */
    {
      यदि ( close2 )
      {
        *angle_अन्तर्  =
        *angle_बहिर्गमनम् = FT_Atan2( d1.x, d1.y );
      }
      अन्यथा
      {
        *angle_अन्तर्  = FT_Atan2( d1.x, d1.y );
        *angle_बहिर्गमनम् = FT_Atan2( d2.x, d2.y );
      }
    }

    theta = ft_pos_abs( FT_Angle_Diff( *angle_अन्तर्, *angle_बहिर्गमनम् ) );

    प्रत्यागम FT_BOOL( theta < FT_SMALL_CONIC_देहली );
  }


  स्थिर शून्य
  ft_cubic_विभज( FT_सदिशम्*  आधारः )
  {
    FT_Pos  a, b, c;


    आधारः[6].x = आधारः[3].x;
    a = आधारः[0].x + आधारः[1].x;
    b = आधारः[1].x + आधारः[2].x;
    c = आधारः[2].x + आधारः[3].x;
    आधारः[5].x = c >> 1;
    c += b;
    आधारः[4].x = c >> 2;
    आधारः[1].x = a >> 1;
    a += b;
    आधारः[2].x = a >> 2;
    आधारः[3].x = ( a + c ) >> 3;

    आधारः[6].y = आधारः[3].y;
    a = आधारः[0].y + आधारः[1].y;
    b = आधारः[1].y + आधारः[2].y;
    c = आधारः[2].y + आधारः[3].y;
    आधारः[5].y = c >> 1;
    c += b;
    आधारः[4].y = c >> 2;
    आधारः[1].y = a >> 1;
    a += b;
    आधारः[2].y = a >> 2;
    आधारः[3].y = ( a + c ) >> 3;
  }


  /* Return the average of `angle1' and `angle2'.            */
  /* This gives correct result even if `angle1' and `angle2' */
  /* have opposite signs.                                    */
  स्थिर FT_Angle
  ft_angle_माध्यम्( FT_Angle  angle1,
                 FT_Angle  angle2 )
  {
    प्रत्यागम angle1 + FT_Angle_Diff( angle1, angle2 ) / 2;
  }


  स्थिर FT_Bool
  ft_cubic_is_small_enough( FT_सदिशम्*  आधारः,
                            FT_Angle   *angle_अन्तर्,
                            FT_Angle   *angle_mid,
                            FT_Angle   *angle_बहिर्गमनम् )
  {
    FT_सदिशम्  d1, d2, d3;
    FT_Angle   theta1, theta2;
    FT_Int     close1, close2, close3;


    d1.x = आधारः[2].x - आधारः[3].x;
    d1.y = आधारः[2].y - आधारः[3].y;
    d2.x = आधारः[1].x - आधारः[2].x;
    d2.y = आधारः[1].y - आधारः[2].y;
    d3.x = आधारः[0].x - आधारः[1].x;
    d3.y = आधारः[0].y - आधारः[1].y;

    close1 = FT_IS_SMALL( d1.x ) && FT_IS_SMALL( d1.y );
    close2 = FT_IS_SMALL( d2.x ) && FT_IS_SMALL( d2.y );
    close3 = FT_IS_SMALL( d3.x ) && FT_IS_SMALL( d3.y );

    यदि ( close1 )
    {
      यदि ( close2 )
      {
        यदि ( close3 )
        {
          /* basically a point;                      */
          /* do nothing to retain original direction */
        }
        अन्यथा /* !close3 */
        {
          *angle_अन्तर्  =
          *angle_mid =
          *angle_बहिर्गमनम् = FT_Atan2( d3.x, d3.y );
        }
      }
      अन्यथा /* !close2 */
      {
        यदि ( close3 )
        {
          *angle_अन्तर्  =
          *angle_mid =
          *angle_बहिर्गमनम् = FT_Atan2( d2.x, d2.y );
        }
        अन्यथा /* !close3 */
        {
          *angle_अन्तर्  =
          *angle_mid = FT_Atan2( d2.x, d2.y );
          *angle_बहिर्गमनम् = FT_Atan2( d3.x, d3.y );
        }
      }
    }
    अन्यथा /* !close1 */
    {
      यदि ( close2 )
      {
        यदि ( close3 )
        {
          *angle_अन्तर्  =
          *angle_mid =
          *angle_बहिर्गमनम् = FT_Atan2( d1.x, d1.y );
        }
        अन्यथा /* !close3 */
        {
          *angle_अन्तर्  = FT_Atan2( d1.x, d1.y );
          *angle_बहिर्गमनम् = FT_Atan2( d3.x, d3.y );
          *angle_mid = ft_angle_माध्यम्( *angle_अन्तर्, *angle_बहिर्गमनम् );
        }
      }
      अन्यथा /* !close2 */
      {
        यदि ( close3 )
        {
          *angle_अन्तर्  = FT_Atan2( d1.x, d1.y );
          *angle_mid =
          *angle_बहिर्गमनम् = FT_Atan2( d2.x, d2.y );
        }
        अन्यथा /* !close3 */
        {
          *angle_अन्तर्  = FT_Atan2( d1.x, d1.y );
          *angle_mid = FT_Atan2( d2.x, d2.y );
          *angle_बहिर्गमनम् = FT_Atan2( d3.x, d3.y );
        }
      }
    }

    theta1 = ft_pos_abs( FT_Angle_Diff( *angle_अन्तर्,  *angle_mid ) );
    theta2 = ft_pos_abs( FT_Angle_Diff( *angle_mid, *angle_बहिर्गमनम् ) );

    प्रत्यागम FT_BOOL( theta1 < FT_SMALL_CUBIC_देहली &&
                    theta2 < FT_SMALL_CUBIC_देहली );
  }


  /*************************************************************************/
  /*************************************************************************/
  /*****                                                               *****/
  /*****                       STROKE BORDERS                          *****/
  /*****                                                               *****/
  /*************************************************************************/
  /*************************************************************************/

  प्रकारनिर्देश गणना  FT_Stroke_चिह्नकानि
  {
    FT_STROKE_चिह्नकम्_ON    = 1,   /* on-curve point  */
    FT_STROKE_चिह्नकम्_CUBIC = 2,   /* cubic off-point */
    FT_STROKE_चिह्नकम्_BEGIN = 4,   /* sub-path start  */
    FT_STROKE_चिह्नकम्_अन्तः   = 8    /* sub-path end    */

  } FT_Stroke_चिह्नकानि_2;

#परिभाषय  FT_STROKE_चिह्नकम्_BEGIN_अन्तः  ( FT_STROKE_चिह्नकम्_BEGIN | FT_STROKE_चिह्नकम्_अन्तः )

  प्रकारनिर्देश संरचना  FT_StrokeBorderRec_
  {
    FT_UInt     अङ्कः_points;
    FT_UInt     max_points;
    FT_सदिशम्*  points;
    FT_Byte*    चिह्नकानि;
    FT_Bool     movable;  /* TRUE for ends of lineto borders */
    FT_Int      start;    /* index of current sub-path start point */
    FT_स्मृतिः   स्मृतिः;
    FT_Bool     valid;

  } FT_StrokeBorderRec, *FT_StrokeBorder;


  स्थिर FT_Error
  ft_stroke_border_grow( FT_StrokeBorder  border,
                         FT_UInt          new_points )
  {
    FT_UInt   old_max = border->max_points;
    FT_UInt   new_max = border->अङ्कः_points + new_points;
    FT_Error  error   = FT_Err_Ok;


    यदि ( new_max > old_max )
    {
      FT_UInt    cur_max = old_max;
      FT_स्मृतिः  स्मृतिः  = border->स्मृतिः;


      यावत् ( cur_max < new_max )
        cur_max += ( cur_max >> 1 ) + 16;

      यदि ( FT_RENEW_ARRAY( border->points, old_max, cur_max ) ||
           FT_RENEW_ARRAY( border->चिह्नकानि,   old_max, cur_max ) )
        गच्छ Exit;

      border->max_points = cur_max;
    }

  Exit:
    प्रत्यागम error;
  }


  स्थिर शून्य
  ft_stroke_border_close( FT_StrokeBorder  border,
                          FT_Bool          reverse )
  {
    FT_UInt  start = (FT_UInt)border->start;
    FT_UInt  गणनम् = border->अङ्कः_points;


    FT_ASSERT( border->start >= 0 );

    /* don't record empty paths! */
    यदि ( गणनम् <= start + 1U )
      border->अङ्कः_points = start;
    अन्यथा
    {
      /* copy the last point to the start of this sub-path, since */
      /* it contains the `adjusted' starting coordinates          */
      border->अङ्कः_points    = --गणनम्;
      border->points[start] = border->points[गणनम्];
      border->चिह्नकानि[start]   = border->चिह्नकानि[गणनम्];

      यदि ( reverse )
      {
        /* reverse the points */
        {
          FT_सदिशम्*  vec1 = border->points + start + 1;
          FT_सदिशम्*  vec2 = border->points + गणनम् - 1;


          प्रति ( ; vec1 < vec2; vec1++, vec2-- )
          {
            FT_सदिशम्  tmp;


            tmp   = *vec1;
            *vec1 = *vec2;
            *vec2 = tmp;
          }
        }

        /* then the tags */
        {
          FT_Byte*  tag1 = border->चिह्नकानि + start + 1;
          FT_Byte*  tag2 = border->चिह्नकानि + गणनम् - 1;


          प्रति ( ; tag1 < tag2; tag1++, tag2-- )
          {
            FT_Byte  tmp;


            tmp   = *tag1;
            *tag1 = *tag2;
            *tag2 = tmp;
          }
        }
      }

      border->चिह्नकानि[start    ] |= FT_STROKE_चिह्नकम्_BEGIN;
      border->चिह्नकानि[गणनम् - 1] |= FT_STROKE_चिह्नकम्_अन्तः;
    }

    border->start   = -1;
    border->movable = FALSE;
  }


  स्थिर FT_Error
  ft_stroke_border_lineto( FT_StrokeBorder  border,
                           FT_सदिशम्*       गन्तव्यम्,
                           FT_Bool          movable )
  {
    FT_Error  error = FT_Err_Ok;


    FT_ASSERT( border->start >= 0 );

    यदि ( border->movable )
    {
      /* move last point */
      border->points[border->अङ्कः_points - 1] = *गन्तव्यम्;
    }
    अन्यथा
    {
      /* don't add zero-length lineto, but always add moveto */
      यदि ( border->अङ्कः_points > (FT_UInt)border->start                     &&
           FT_IS_SMALL( border->points[border->अङ्कः_points - 1].x - गन्तव्यम्->x ) &&
           FT_IS_SMALL( border->points[border->अङ्कः_points - 1].y - गन्तव्यम्->y ) )
        प्रत्यागम error;

      /* add one point */
      error = ft_stroke_border_grow( border, 1 );
      यदि ( !error )
      {
        FT_सदिशम्*  vec = border->points + border->अङ्कः_points;
        FT_Byte*    चिह्नकम् = border->चिह्नकानि   + border->अङ्कः_points;


        vec[0] = *गन्तव्यम्;
        चिह्नकम्[0] = FT_STROKE_चिह्नकम्_ON;

        border->अङ्कः_points += 1;
      }
    }
    border->movable = movable;
    प्रत्यागम error;
  }


  स्थिर FT_Error
  ft_stroke_border_conicto( FT_StrokeBorder  border,
                            FT_सदिशम्*       control,
                            FT_सदिशम्*       गन्तव्यम् )
  {
    FT_Error  error;


    FT_ASSERT( border->start >= 0 );

    error = ft_stroke_border_grow( border, 2 );
    यदि ( !error )
    {
      FT_सदिशम्*  vec = border->points + border->अङ्कः_points;
      FT_Byte*    चिह्नकम् = border->चिह्नकानि   + border->अङ्कः_points;


      vec[0] = *control;
      vec[1] = *गन्तव्यम्;

      चिह्नकम्[0] = 0;
      चिह्नकम्[1] = FT_STROKE_चिह्नकम्_ON;

      border->अङ्कः_points += 2;
    }

    border->movable = FALSE;

    प्रत्यागम error;
  }


  स्थिर FT_Error
  ft_stroke_border_cubicto( FT_StrokeBorder  border,
                            FT_सदिशम्*       control1,
                            FT_सदिशम्*       control2,
                            FT_सदिशम्*       गन्तव्यम् )
  {
    FT_Error  error;


    FT_ASSERT( border->start >= 0 );

    error = ft_stroke_border_grow( border, 3 );
    यदि ( !error )
    {
      FT_सदिशम्*  vec = border->points + border->अङ्कः_points;
      FT_Byte*    चिह्नकम् = border->चिह्नकानि   + border->अङ्कः_points;


      vec[0] = *control1;
      vec[1] = *control2;
      vec[2] = *गन्तव्यम्;

      चिह्नकम्[0] = FT_STROKE_चिह्नकम्_CUBIC;
      चिह्नकम्[1] = FT_STROKE_चिह्नकम्_CUBIC;
      चिह्नकम्[2] = FT_STROKE_चिह्नकम्_ON;

      border->अङ्कः_points += 3;
    }

    border->movable = FALSE;

    प्रत्यागम error;
  }


#परिभाषय FT_ARC_CUBIC_ANGLE  ( FT_ANGLE_PI / 2 )


  स्थिर FT_Error
  ft_stroke_border_arcto( FT_StrokeBorder  border,
                          FT_सदिशम्*       center,
                          FT_Fixed         radius,
                          FT_Angle         angle_start,
                          FT_Angle         angle_diff )
  {
    FT_Fixed   coef;
    FT_सदिशम्  a0, a1, a2, a3;
    FT_Int     i, arcs = 1;
    FT_Error   error = FT_Err_Ok;


    /* number of cubic arcs to draw */
    यावत् (  angle_diff > FT_ARC_CUBIC_ANGLE * arcs ||
            -angle_diff > FT_ARC_CUBIC_ANGLE * arcs )
      arcs++;

    /* control tangents */
    coef  = FT_Tan( angle_diff / ( 4 * arcs ) );
    coef += coef / 3;

    /* compute start and first control point */
    FT_सदिशम्_From_Polar( &a0, radius, angle_start );
    a1.x = FT_MulFix( -a0.y, coef );
    a1.y = FT_MulFix(  a0.x, coef );

    a0.x += center->x;
    a0.y += center->y;
    a1.x += a0.x;
    a1.y += a0.y;

    प्रति ( i = 1; i <= arcs; i++ )
    {
      /* compute end and second control point */
      FT_सदिशम्_From_Polar( &a3, radius,
                            angle_start + i * angle_diff / arcs );
      a2.x = FT_MulFix(  a3.y, coef );
      a2.y = FT_MulFix( -a3.x, coef );

      a3.x += center->x;
      a3.y += center->y;
      a2.x += a3.x;
      a2.y += a3.y;

      /* add cubic arc */
      error = ft_stroke_border_cubicto( border, &a1, &a2, &a3 );
      यदि ( error )
        भङ्ग;

      /* a0 = a3; */
      a1.x = a3.x - a2.x + a3.x;
      a1.y = a3.y - a2.y + a3.y;
    }

    प्रत्यागम error;
  }


  स्थिर FT_Error
  ft_stroke_border_moveto( FT_StrokeBorder  border,
                           FT_सदिशम्*       गन्तव्यम् )
  {
    /* close current open path if any ? */
    यदि ( border->start >= 0 )
      ft_stroke_border_close( border, FALSE );

    border->start = (FT_Int)border->अङ्कः_points;
    border->movable = FALSE;

    प्रत्यागम ft_stroke_border_lineto( border, गन्तव्यम्, FALSE );
  }


  स्थिर शून्य
  ft_stroke_border_init( FT_StrokeBorder  border,
                         FT_स्मृतिः        स्मृतिः )
  {
    border->स्मृतिः = स्मृतिः;
    border->points = रिक्त;
    border->चिह्नकानि   = रिक्त;

    border->अङ्कः_points = 0;
    border->max_points = 0;
    border->start      = -1;
    border->valid      = FALSE;
  }


  स्थिर शून्य
  ft_stroke_border_reset( FT_StrokeBorder  border )
  {
    border->अङ्कः_points = 0;
    border->start      = -1;
    border->valid      = FALSE;
  }


  स्थिर शून्य
  ft_stroke_border_समाप्तम्( FT_StrokeBorder  border )
  {
    FT_स्मृतिः  स्मृतिः = border->स्मृतिः;


    FT_FREE( border->points );
    FT_FREE( border->चिह्नकानि );

    border->अङ्कः_points = 0;
    border->max_points = 0;
    border->start      = -1;
    border->valid      = FALSE;
  }


  स्थिर FT_Error
  ft_stroke_border_get_counts( FT_StrokeBorder  border,
                               FT_UInt         *anum_points,
                               FT_UInt         *anum_contours )
  {
    FT_Error  error        = FT_Err_Ok;
    FT_UInt   अङ्कः_points   = 0;
    FT_UInt   अङ्कः_contours = 0;

    FT_UInt     गणनम्      = border->अङ्कः_points;
    FT_Byte*    चिह्नकानि       = border->चिह्नकानि;
    FT_Int      अन्तर्_contour = 0;


    प्रति ( ; गणनम् > 0; गणनम्--, अङ्कः_points++, चिह्नकानि++ )
    {
      यदि ( चिह्नकानि[0] & FT_STROKE_चिह्नकम्_BEGIN )
      {
        यदि ( अन्तर्_contour != 0 )
          गच्छ Fail;

        अन्तर्_contour = 1;
      }
      अन्यथा यदि ( अन्तर्_contour == 0 )
        गच्छ Fail;

      यदि ( चिह्नकानि[0] & FT_STROKE_चिह्नकम्_अन्तः )
      {
        अन्तर्_contour = 0;
        अङ्कः_contours++;
      }
    }

    यदि ( अन्तर्_contour != 0 )
      गच्छ Fail;

    border->valid = TRUE;

  Exit:
    *anum_points   = अङ्कः_points;
    *anum_contours = अङ्कः_contours;
    प्रत्यागम error;

  Fail:
    अङ्कः_points   = 0;
    अङ्कः_contours = 0;
    error        = FT_THROW( Invalid_Outline );
    गच्छ Exit;
  }


  स्थिर शून्य
  ft_stroke_border_export( FT_StrokeBorder  border,
                           FT_Outline*      outline )
  {
    /* copy point locations */
    यदि ( border->अङ्कः_points )
      FT_ARRAY_COPY( outline->points + outline->n_points,
                     border->points,
                     border->अङ्कः_points );

    /* copy tags */
    {
      FT_UInt   गणनम् = border->अङ्कः_points;
      FT_Byte*  पठ  = border->चिह्नकानि;
      FT_Byte*  लिख = outline->चिह्नकानि + outline->n_points;


      प्रति ( ; गणनम् > 0; गणनम्--, पठ++, लिख++ )
      {
        यदि ( *पठ & FT_STROKE_चिह्नकम्_ON )
          *लिख = FT_वक्रम्_चिह्नकम्_ON;
        अन्यथा यदि ( *पठ & FT_STROKE_चिह्नकम्_CUBIC )
          *लिख = FT_वक्रम्_चिह्नकम्_CUBIC;
        अन्यथा
          *लिख = FT_वक्रम्_चिह्नकम्_CONIC;
      }
    }

    /* copy contours */
    {
      FT_UInt     गणनम् = border->अङ्कः_points;
      FT_Byte*    चिह्नकानि  = border->चिह्नकानि;
      FT_UShort*  लिख = outline->contours + outline->n_contours;
      FT_UShort   idx   = outline->n_points;


      प्रति ( ; गणनम् > 0; गणनम्--, चिह्नकानि++, idx++ )
      {
        यदि ( *चिह्नकानि & FT_STROKE_चिह्नकम्_अन्तः )
        {
          *लिख++ = idx;
          outline->n_contours++;
        }
      }
    }

    outline->n_points += (FT_UShort)border->अङ्कः_points;

    FT_ASSERT( FT_Outline_Check( outline ) == 0 );
  }


  /*************************************************************************/
  /*************************************************************************/
  /*****                                                               *****/
  /*****                           STROKER                             *****/
  /*****                                                               *****/
  /*************************************************************************/
  /*************************************************************************/

#परिभाषय FT_SIDE_गन्तव्यम्_ROTATE( s )   ( FT_ANGLE_PI2 - (s) * FT_ANGLE_PI )

  प्रकारनिर्देश संरचना  FT_StrokerRec_
  {
    FT_Angle             angle_अन्तर्;             /* direction into curr join */
    FT_Angle             angle_बहिर्गमनम्;            /* direction out of join  */
    FT_सदिशम्            center;               /* current position */
    FT_Fixed             पङ्क्तिः_दीर्घता;          /* length of last lineto */
    FT_Bool              first_point;          /* is this the start? */
    FT_Bool              subpath_open;         /* is the subpath open? */
    FT_Angle             subpath_angle;        /* subpath start direction */
    FT_सदिशम्            subpath_start;        /* subpath start position */
    FT_Fixed             subpath_पङ्क्तिः_दीर्घता;  /* subpath start lineto len */
    FT_Bool              handle_wide_strokes;  /* use wide strokes logic? */

    FT_Stroker_पङ्क्तिः_Cap   पङ्क्तिः_cap;
    FT_Stroker_पङ्क्तिः_Join  पङ्क्तिः_join;
    FT_Stroker_पङ्क्तिः_Join  पङ्क्तिः_join_saved;
    FT_Fixed             miter_सीमा;
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
    FT_स्मृतिः   स्मृतिः;
    FT_Stroker  stroker = रिक्त;


    यदि ( !library )
      प्रत्यागम FT_THROW( Invalid_Library_Handle );

    यदि ( !astroker )
      प्रत्यागम FT_THROW( Invalid_तर्कः );

    स्मृतिः = library->स्मृतिः;

    यदि ( !FT_NEW( stroker ) )
    {
      stroker->library = library;

      ft_stroke_border_init( &stroker->borders[0], स्मृतिः );
      ft_stroke_border_init( &stroker->borders[1], स्मृतिः );
    }

    *astroker = stroker;

    प्रत्यागम error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( शून्य )
  FT_Stroker_समुच्चयः( FT_Stroker           stroker,
                  FT_Fixed             radius,
                  FT_Stroker_पङ्क्तिः_Cap   पङ्क्तिः_cap,
                  FT_Stroker_पङ्क्तिः_Join  पङ्क्तिः_join,
                  FT_Fixed             miter_सीमा )
  {
    यदि ( !stroker )
      प्रत्यागम;

    stroker->radius      = radius;
    stroker->पङ्क्तिः_cap    = पङ्क्तिः_cap;
    stroker->पङ्क्तिः_join   = पङ्क्तिः_join;
    stroker->miter_सीमा = miter_सीमा;

    /* ensure miter limit has sensible value */
    यदि ( stroker->miter_सीमा < 0x10000L )
      stroker->miter_सीमा = 0x10000L;

    /* save line join style:                                           */
    /* line join style can be temporarily changed when stroking curves */
    stroker->पङ्क्तिः_join_saved = पङ्क्तिः_join;

    FT_Stroker_Rewind( stroker );
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( शून्य )
  FT_Stroker_Rewind( FT_Stroker  stroker )
  {
    यदि ( stroker )
    {
      ft_stroke_border_reset( &stroker->borders[0] );
      ft_stroke_border_reset( &stroker->borders[1] );
    }
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( शून्य )
  FT_Stroker_समाप्तम्( FT_Stroker  stroker )
  {
    यदि ( stroker )
    {
      FT_स्मृतिः  स्मृतिः = stroker->library->स्मृतिः;


      ft_stroke_border_समाप्तम्( &stroker->borders[0] );
      ft_stroke_border_समाप्तम्( &stroker->borders[1] );

      stroker->library = रिक्त;
      FT_FREE( stroker );
    }
  }


  /* create a circular arc at a corner or cap */
  स्थिर FT_Error
  ft_stroker_arcto( FT_Stroker  stroker,
                    FT_Int      side )
  {
    FT_Angle         योग, rotate;
    FT_Fixed         radius = stroker->radius;
    FT_Error         error  = FT_Err_Ok;
    FT_StrokeBorder  border = stroker->borders + side;


    rotate = FT_SIDE_गन्तव्यम्_ROTATE( side );

    योग = FT_Angle_Diff( stroker->angle_अन्तर्, stroker->angle_बहिर्गमनम् );
    यदि ( योग == FT_ANGLE_PI )
      योग = -rotate * 2;

    error = ft_stroke_border_arcto( border,
                                    &stroker->center,
                                    radius,
                                    stroker->angle_अन्तर् + rotate,
                                    योग );
    border->movable = FALSE;
    प्रत्यागम error;
  }


  /* add a cap at the end of an opened path */
  स्थिर FT_Error
  ft_stroker_cap( FT_Stroker  stroker,
                  FT_Angle    angle,
                  FT_Int      side )
  {
    FT_Error  error = FT_Err_Ok;


    यदि ( stroker->पङ्क्तिः_cap == FT_STROKER_LINECAP_आवर्तनम् )
    {
      /* add a round cap */
      stroker->angle_अन्तर्  = angle;
      stroker->angle_बहिर्गमनम् = angle + FT_ANGLE_PI;

      error = ft_stroker_arcto( stroker, side );
    }
    अन्यथा
    {
      /* add a square or butt cap */
      FT_सदिशम्        मध्यः, delta;
      FT_Fixed         radius = stroker->radius;
      FT_StrokeBorder  border = stroker->borders + side;


      /* compute middle point and first angle point */
      FT_सदिशम्_From_Polar( &मध्यः, radius, angle );
      delta.x = side ?  मध्यः.y : -मध्यः.y;
      delta.y = side ? -मध्यः.x :  मध्यः.x;

      यदि ( stroker->पङ्क्तिः_cap == FT_STROKER_LINECAP_वर्गफलम् )
      {
        मध्यः.x += stroker->center.x;
        मध्यः.y += stroker->center.y;
      }
      अन्यथा  /* FT_STROKER_LINECAP_BUTT */
      {
        मध्यः.x  = stroker->center.x;
        मध्यः.y  = stroker->center.y;
      }

      delta.x  += मध्यः.x;
      delta.y  += मध्यः.y;

      error = ft_stroke_border_lineto( border, &delta, FALSE );
      यदि ( error )
        गच्छ Exit;

      /* compute second angle point */
      delta.x = मध्यः.x - delta.x + मध्यः.x;
      delta.y = मध्यः.y - delta.y + मध्यः.y;

      error = ft_stroke_border_lineto( border, &delta, FALSE );
    }

  Exit:
    प्रत्यागम error;
  }


  /* process an inside corner, i.e. compute intersection */
  स्थिर FT_Error
  ft_stroker_inside( FT_Stroker  stroker,
                     FT_Int      side,
                     FT_Fixed    पङ्क्तिः_दीर्घता )
  {
    FT_StrokeBorder  border = stroker->borders + side;
    FT_Angle         phi, theta, rotate;
    FT_Fixed         दीर्घता;
    FT_सदिशम्        sigma = { 0, 0 };
    FT_सदिशम्        delta;
    FT_Error         error = FT_Err_Ok;
    FT_Bool          intersect;          /* use intersection of lines? */


    rotate = FT_SIDE_गन्तव्यम्_ROTATE( side );

    theta = FT_Angle_Diff( stroker->angle_अन्तर्, stroker->angle_बहिर्गमनम् ) / 2;

    /* Only intersect borders if between two lineto's and both */
    /* lines are long enough (line_length is zero for curves). */
    /* Also avoid U-turns of nearly 180 degree.                */
    यदि ( !border->movable || पङ्क्तिः_दीर्घता == 0  ||
         theta > 0x59C000 || theta < -0x59C000 )
      intersect = FALSE;
    अन्यथा
    {
      /* compute minimum required length of lines */
      FT_Fixed  min_दीर्घता;


      FT_सदिशम्_Unit( &sigma, theta );
      min_दीर्घता =
        ft_pos_abs( FT_MulDiv( stroker->radius, sigma.y, sigma.x ) );

      intersect = FT_BOOL( min_दीर्घता                         &&
                           stroker->पङ्क्तिः_दीर्घता >= min_दीर्घता &&
                           पङ्क्तिः_दीर्घता          >= min_दीर्घता );
    }

    यदि ( !intersect )
    {
      FT_सदिशम्_From_Polar( &delta, stroker->radius,
                            stroker->angle_बहिर्गमनम् + rotate );
      delta.x += stroker->center.x;
      delta.y += stroker->center.y;

      border->movable = FALSE;
    }
    अन्यथा
    {
      /* compute median angle */
      phi = stroker->angle_अन्तर् + theta + rotate;

      दीर्घता = FT_DivFix( stroker->radius, sigma.x );

      FT_सदिशम्_From_Polar( &delta, दीर्घता, phi );
      delta.x += stroker->center.x;
      delta.y += stroker->center.y;
    }

    error = ft_stroke_border_lineto( border, &delta, FALSE );

    प्रत्यागम error;
  }


  /* process an outside corner, i.e. compute bevel/miter/round */
  स्थिर FT_Error
  ft_stroker_outside( FT_Stroker  stroker,
                      FT_Int      side,
                      FT_Fixed    पङ्क्तिः_दीर्घता )
  {
    FT_StrokeBorder  border = stroker->borders + side;
    FT_Error         error;
    FT_Angle         rotate;


    यदि ( stroker->पङ्क्तिः_join == FT_STROKER_LINEJOIN_आवर्तनम् )
      error = ft_stroker_arcto( stroker, side );
    अन्यथा
    {
      /* this is a mitered (pointed) or beveled (truncated) corner */
      FT_Fixed   radius = stroker->radius;
      FT_सदिशम्  sigma = { 0, 0 };
      FT_Angle   theta = 0, phi = 0;
      FT_Bool    bevel, fixed_bevel;


      rotate = FT_SIDE_गन्तव्यम्_ROTATE( side );

      bevel =
        FT_BOOL( stroker->पङ्क्तिः_join == FT_STROKER_LINEJOIN_BEVEL );

      fixed_bevel =
        FT_BOOL( stroker->पङ्क्तिः_join != FT_STROKER_LINEJOIN_MITER_VARIABLE );

      /* check miter limit first */
      यदि ( !bevel )
      {
        theta = FT_Angle_Diff( stroker->angle_अन्तर्, stroker->angle_बहिर्गमनम् ) / 2;

        यदि ( theta == FT_ANGLE_PI2 )
          theta = -rotate;

        phi = stroker->angle_अन्तर् + theta + rotate;

        FT_सदिशम्_From_Polar( &sigma, stroker->miter_सीमा, theta );

        /* is miter limit exceeded? */
        यदि ( sigma.x < 0x10000L )
        {
          /* don't create variable bevels for very small deviations; */
          /* FT_Sin(x) = 0 for x <= 57                               */
          यदि ( fixed_bevel || ft_pos_abs( theta ) > 57 )
            bevel = TRUE;
        }
      }

      यदि ( bevel )  /* this is a bevel (broken angle) */
      {
        यदि ( fixed_bevel )
        {
          /* the outer corners are simply joined together */
          FT_सदिशम्  delta;


          /* add bevel */
          FT_सदिशम्_From_Polar( &delta,
                                radius,
                                stroker->angle_बहिर्गमनम् + rotate );
          delta.x += stroker->center.x;
          delta.y += stroker->center.y;

          border->movable = FALSE;
          error = ft_stroke_border_lineto( border, &delta, FALSE );
        }
        अन्यथा /* variable bevel or clipped miter */
        {
          /* the miter is truncated */
          FT_सदिशम्  मध्यः, delta;
          FT_Fixed   coef;


          /* compute middle point and first angle point */
          FT_सदिशम्_From_Polar( &मध्यः,
                                FT_MulFix( radius, stroker->miter_सीमा ),
                                phi );

          coef    = FT_DivFix(  0x10000L - sigma.x, sigma.y );
          delta.x = FT_MulFix(  मध्यः.y, coef );
          delta.y = FT_MulFix( -मध्यः.x, coef );

          मध्यः.x += stroker->center.x;
          मध्यः.y += stroker->center.y;
          delta.x  += मध्यः.x;
          delta.y  += मध्यः.y;

          error = ft_stroke_border_lineto( border, &delta, FALSE );
          यदि ( error )
            गच्छ Exit;

          /* compute second angle point */
          delta.x = मध्यः.x - delta.x + मध्यः.x;
          delta.y = मध्यः.y - delta.y + मध्यः.y;

          error = ft_stroke_border_lineto( border, &delta, FALSE );
          यदि ( error )
            गच्छ Exit;

          /* finally, add an end point; only needed if not lineto */
          /* (line_length is zero for curves)                     */
          यदि ( पङ्क्तिः_दीर्घता == 0 )
          {
            FT_सदिशम्_From_Polar( &delta,
                                  radius,
                                  stroker->angle_बहिर्गमनम् + rotate );

            delta.x += stroker->center.x;
            delta.y += stroker->center.y;

            error = ft_stroke_border_lineto( border, &delta, FALSE );
          }
        }
      }
      अन्यथा /* this is a miter (intersection) */
      {
        FT_Fixed   दीर्घता;
        FT_सदिशम्  delta;


        दीर्घता = FT_MulDiv( stroker->radius, stroker->miter_सीमा, sigma.x );

        FT_सदिशम्_From_Polar( &delta, दीर्घता, phi );
        delta.x += stroker->center.x;
        delta.y += stroker->center.y;

        error = ft_stroke_border_lineto( border, &delta, FALSE );
        यदि ( error )
          गच्छ Exit;

        /* now add an end point; only needed if not lineto */
        /* (line_length is zero for curves)                */
        यदि ( पङ्क्तिः_दीर्घता == 0 )
        {
          FT_सदिशम्_From_Polar( &delta,
                                stroker->radius,
                                stroker->angle_बहिर्गमनम् + rotate );
          delta.x += stroker->center.x;
          delta.y += stroker->center.y;

          error = ft_stroke_border_lineto( border, &delta, FALSE );
        }
      }
    }

  Exit:
    प्रत्यागम error;
  }


  स्थिर FT_Error
  ft_stroker_process_corner( FT_Stroker  stroker,
                             FT_Fixed    पङ्क्तिः_दीर्घता )
  {
    FT_Error  error = FT_Err_Ok;
    FT_Angle  turn;
    FT_Int    inside_side;


    turn = FT_Angle_Diff( stroker->angle_अन्तर्, stroker->angle_बहिर्गमनम् );

    /* no specific corner processing is required if the turn is 0 */
    यदि ( turn == 0 )
      गच्छ Exit;

    /* when we turn to the right, the inside side is 0 */
    /* otherwise, the inside side is 1 */
    inside_side = ( turn < 0 );

    /* process the inside side */
    error = ft_stroker_inside( stroker, inside_side, पङ्क्तिः_दीर्घता );
    यदि ( error )
      गच्छ Exit;

    /* process the outside side */
    error = ft_stroker_outside( stroker, !inside_side, पङ्क्तिः_दीर्घता );

  Exit:
    प्रत्यागम error;
  }


  /* add two points to the left and right borders corresponding to the */
  /* start of the subpath                                              */
  स्थिर FT_Error
  ft_stroker_subpath_start( FT_Stroker  stroker,
                            FT_Angle    start_angle,
                            FT_Fixed    पङ्क्तिः_दीर्घता )
  {
    FT_सदिशम्        delta;
    FT_सदिशम्        point;
    FT_Error         error;
    FT_StrokeBorder  border;


    FT_सदिशम्_From_Polar( &delta, stroker->radius,
                          start_angle + FT_ANGLE_PI2 );

    point.x = stroker->center.x + delta.x;
    point.y = stroker->center.y + delta.y;

    border = stroker->borders;
    error = ft_stroke_border_moveto( border, &point );
    यदि ( error )
      गच्छ Exit;

    point.x = stroker->center.x - delta.x;
    point.y = stroker->center.y - delta.y;

    border++;
    error = ft_stroke_border_moveto( border, &point );

    /* save angle, position, and line length for last join */
    /* (line_length is zero for curves)                    */
    stroker->subpath_angle       = start_angle;
    stroker->first_point         = FALSE;
    stroker->subpath_पङ्क्तिः_दीर्घता = पङ्क्तिः_दीर्घता;

  Exit:
    प्रत्यागम error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Stroker_पङ्क्तिः_गन्तव्यम्( FT_Stroker  stroker,
                     FT_सदिशम्*  गन्तव्यम् )
  {
    FT_Error         error = FT_Err_Ok;
    FT_StrokeBorder  border;
    FT_सदिशम्        delta;
    FT_Angle         angle;
    FT_Int           side;
    FT_Fixed         पङ्क्तिः_दीर्घता;


    यदि ( !stroker || !गन्तव्यम् )
      प्रत्यागम FT_THROW( Invalid_तर्कः );

    delta.x = गन्तव्यम्->x - stroker->center.x;
    delta.y = गन्तव्यम्->y - stroker->center.y;

    /* a zero-length lineto is a no-op; avoid creating a spurious corner */
    यदि ( delta.x == 0 && delta.y == 0 )
       गच्छ Exit;

    /* compute length of line */
    पङ्क्तिः_दीर्घता = FT_सदिशम्_दीर्घता( &delta );

    angle = FT_Atan2( delta.x, delta.y );
    FT_सदिशम्_From_Polar( &delta, stroker->radius, angle + FT_ANGLE_PI2 );

    /* process corner if necessary */
    यदि ( stroker->first_point )
    {
      /* This is the first segment of a subpath.  We need to     */
      /* add a point to each border at their respective starting */
      /* point locations.                                        */
      error = ft_stroker_subpath_start( stroker, angle, पङ्क्तिः_दीर्घता );
      यदि ( error )
        गच्छ Exit;
    }
    अन्यथा
    {
      /* process the current corner */
      stroker->angle_बहिर्गमनम् = angle;
      error = ft_stroker_process_corner( stroker, पङ्क्तिः_दीर्घता );
      यदि ( error )
        गच्छ Exit;
    }

    /* now add a line segment to both the `inside' and `outside' paths */
    प्रति ( border = stroker->borders, side = 1; side >= 0; side--, border++ )
    {
      FT_सदिशम्  point;


      point.x = गन्तव्यम्->x + delta.x;
      point.y = गन्तव्यम्->y + delta.y;

      /* the ends of lineto borders are movable */
      error = ft_stroke_border_lineto( border, &point, TRUE );
      यदि ( error )
        गच्छ Exit;

      delta.x = -delta.x;
      delta.y = -delta.y;
    }

    stroker->angle_अन्तर्    = angle;
    stroker->center      = *गन्तव्यम्;
    stroker->पङ्क्तिः_दीर्घता = पङ्क्तिः_दीर्घता;

  Exit:
    प्रत्यागम error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Stroker_Conic_गन्तव्यम्( FT_Stroker  stroker,
                      FT_सदिशम्*  control,
                      FT_सदिशम्*  गन्तव्यम् )
  {
    FT_Error    error = FT_Err_Ok;
    FT_सदिशम्   bez_राशिः[34];
    FT_सदिशम्*  arc;
    FT_सदिशम्*  सीमा = bez_राशिः + 30;
    FT_Bool     first_arc = TRUE;


    यदि ( !stroker || !control || !गन्तव्यम् )
    {
      error = FT_THROW( Invalid_तर्कः );
      गच्छ Exit;
    }

    /* if all control points are coincident, this is a no-op; */
    /* avoid creating a spurious corner                       */
    यदि ( FT_IS_SMALL( stroker->center.x - control->x ) &&
         FT_IS_SMALL( stroker->center.y - control->y ) &&
         FT_IS_SMALL( control->x        - गन्तव्यम्->x      ) &&
         FT_IS_SMALL( control->y        - गन्तव्यम्->y      ) )
    {
       stroker->center = *गन्तव्यम्;
       गच्छ Exit;
    }

    arc    = bez_राशिः;
    arc[0] = *गन्तव्यम्;
    arc[1] = *control;
    arc[2] = stroker->center;

    कुरु
    {
      FT_Angle  angle_अन्तर्, angle_बहिर्गमनम्;


      /* initialize with current direction */
      angle_अन्तर् = angle_बहिर्गमनम् = stroker->angle_अन्तर्;

      यदि ( arc < सीमा                                             &&
           !ft_conic_is_small_enough( arc, &angle_अन्तर्, &angle_बहिर्गमनम् ) )
      {
        यदि ( stroker->first_point )
          stroker->angle_अन्तर् = angle_अन्तर्;

        ft_conic_विभज( arc );
        arc += 2;
        अनुवर्त;
      }

      यदि ( first_arc )
      {
        first_arc = FALSE;

        /* process corner if necessary */
        यदि ( stroker->first_point )
          error = ft_stroker_subpath_start( stroker, angle_अन्तर्, 0 );
        अन्यथा
        {
          stroker->angle_बहिर्गमनम् = angle_अन्तर्;
          error = ft_stroker_process_corner( stroker, 0 );
        }
      }
      अन्यथा यदि ( ft_pos_abs( FT_Angle_Diff( stroker->angle_अन्तर्, angle_अन्तर् ) ) >
                  FT_SMALL_CONIC_देहली / 4                             )
      {
        /* if the deviation from one arc to the next is too great, */
        /* add a round corner                                      */
        stroker->center    = arc[2];
        stroker->angle_बहिर्गमनम् = angle_अन्तर्;
        stroker->पङ्क्तिः_join = FT_STROKER_LINEJOIN_आवर्तनम्;

        error = ft_stroker_process_corner( stroker, 0 );

        /* reinstate line join style */
        stroker->पङ्क्तिः_join = stroker->पङ्क्तिः_join_saved;
      }

      यदि ( error )
        गच्छ Exit;

      /* the arc's angle is small enough; we can add it directly to each */
      /* border                                                          */
      {
        FT_सदिशम्        ctrl, अन्तः;
        FT_Angle         theta, phi, rotate, alpha0 = 0;
        FT_Fixed         दीर्घता;
        FT_StrokeBorder  border;
        FT_Int           side;


        theta  = FT_Angle_Diff( angle_अन्तर्, angle_बहिर्गमनम् ) / 2;
        phi    = angle_अन्तर् + theta;
        दीर्घता = FT_DivFix( stroker->radius, FT_Cos( theta ) );

        /* compute direction of original arc */
        यदि ( stroker->handle_wide_strokes )
          alpha0 = FT_Atan2( arc[0].x - arc[2].x, arc[0].y - arc[2].y );

        प्रति ( border = stroker->borders, side = 0;
              side <= 1;
              side++, border++ )
        {
          rotate = FT_SIDE_गन्तव्यम्_ROTATE( side );

          /* compute control point */
          FT_सदिशम्_From_Polar( &ctrl, दीर्घता, phi + rotate );
          ctrl.x += arc[1].x;
          ctrl.y += arc[1].y;

          /* compute end point */
          FT_सदिशम्_From_Polar( &अन्तः, stroker->radius, angle_बहिर्गमनम् + rotate );
          अन्तः.x += arc[0].x;
          अन्तः.y += arc[0].y;

          यदि ( stroker->handle_wide_strokes )
          {
            FT_सदिशम्  start;
            FT_Angle   alpha1;


            /* determine whether the border radius is greater than the */
            /* radius of curvature of the original arc                 */
            start = border->points[border->अङ्कः_points - 1];

            alpha1 = FT_Atan2( अन्तः.x - start.x, अन्तः.y - start.y );

            /* is the direction of the border arc opposite to */
            /* that of the original arc? */
            यदि ( ft_pos_abs( FT_Angle_Diff( alpha0, alpha1 ) ) >
                   FT_ANGLE_PI / 2                             )
            {
              FT_Angle   beta, gamma;
              FT_सदिशम्  bvec, delta;
              FT_Fixed   blen, sinA, sinB, alen;


              /* use the sine rule to find the intersection point */
              beta  = FT_Atan2( arc[2].x - start.x, arc[2].y - start.y );
              gamma = FT_Atan2( arc[0].x - अन्तः.x,   arc[0].y - अन्तः.y );

              bvec.x = अन्तः.x - start.x;
              bvec.y = अन्तः.y - start.y;

              blen = FT_सदिशम्_दीर्घता( &bvec );

              sinA = ft_pos_abs( FT_Sin( alpha1 - gamma ) );
              sinB = ft_pos_abs( FT_Sin( beta - gamma ) );

              alen = FT_MulDiv( blen, sinA, sinB );

              FT_सदिशम्_From_Polar( &delta, alen, beta );
              delta.x += start.x;
              delta.y += start.y;

              /* circumnavigate the negative sector backwards */
              border->movable = FALSE;
              error = ft_stroke_border_lineto( border, &delta, FALSE );
              यदि ( error )
                गच्छ Exit;
              error = ft_stroke_border_lineto( border, &अन्तः, FALSE );
              यदि ( error )
                गच्छ Exit;
              error = ft_stroke_border_conicto( border, &ctrl, &start );
              यदि ( error )
                गच्छ Exit;
              /* and then move to the endpoint */
              error = ft_stroke_border_lineto( border, &अन्तः, FALSE );
              यदि ( error )
                गच्छ Exit;

              अनुवर्त;
            }

            /* else fall through */
          }

          /* simply add an arc */
          error = ft_stroke_border_conicto( border, &ctrl, &अन्तः );
          यदि ( error )
            गच्छ Exit;
        }
      }

      stroker->angle_अन्तर् = angle_बहिर्गमनम्;

      यदि ( arc == bez_राशिः )
        भङ्ग;
      arc -= 2;
    } यावत् ( 1 );

    stroker->center      = *गन्तव्यम्;
    stroker->पङ्क्तिः_दीर्घता = 0;

  Exit:
    प्रत्यागम error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Stroker_Cubic_गन्तव्यम्( FT_Stroker  stroker,
                      FT_सदिशम्*  control1,
                      FT_सदिशम्*  control2,
                      FT_सदिशम्*  गन्तव्यम् )
  {
    FT_Error    error = FT_Err_Ok;
    FT_सदिशम्   bez_राशिः[37];
    FT_सदिशम्*  arc;
    FT_सदिशम्*  सीमा = bez_राशिः + 32;
    FT_Bool     first_arc = TRUE;


    यदि ( !stroker || !control1 || !control2 || !गन्तव्यम् )
    {
      error = FT_THROW( Invalid_तर्कः );
      गच्छ Exit;
    }

    /* if all control points are coincident, this is a no-op; */
    /* avoid creating a spurious corner */
    यदि ( FT_IS_SMALL( stroker->center.x - control1->x ) &&
         FT_IS_SMALL( stroker->center.y - control1->y ) &&
         FT_IS_SMALL( control1->x       - control2->x ) &&
         FT_IS_SMALL( control1->y       - control2->y ) &&
         FT_IS_SMALL( control2->x       - गन्तव्यम्->x       ) &&
         FT_IS_SMALL( control2->y       - गन्तव्यम्->y       ) )
    {
       stroker->center = *गन्तव्यम्;
       गच्छ Exit;
    }

    arc    = bez_राशिः;
    arc[0] = *गन्तव्यम्;
    arc[1] = *control2;
    arc[2] = *control1;
    arc[3] = stroker->center;

    कुरु
    {
      FT_Angle  angle_अन्तर्, angle_mid, angle_बहिर्गमनम्;


      /* initialize with current direction */
      angle_अन्तर् = angle_बहिर्गमनम् = angle_mid = stroker->angle_अन्तर्;

      यदि ( arc < सीमा                                         &&
           !ft_cubic_is_small_enough( arc, &angle_अन्तर्,
                                      &angle_mid, &angle_बहिर्गमनम् ) )
      {
        यदि ( stroker->first_point )
          stroker->angle_अन्तर् = angle_अन्तर्;

        ft_cubic_विभज( arc );
        arc += 3;
        अनुवर्त;
      }

      यदि ( first_arc )
      {
        first_arc = FALSE;

        /* process corner if necessary */
        यदि ( stroker->first_point )
          error = ft_stroker_subpath_start( stroker, angle_अन्तर्, 0 );
        अन्यथा
        {
          stroker->angle_बहिर्गमनम् = angle_अन्तर्;
          error = ft_stroker_process_corner( stroker, 0 );
        }
      }
      अन्यथा यदि ( ft_pos_abs( FT_Angle_Diff( stroker->angle_अन्तर्, angle_अन्तर् ) ) >
                  FT_SMALL_CUBIC_देहली / 4                             )
      {
        /* if the deviation from one arc to the next is too great, */
        /* add a round corner                                      */
        stroker->center    = arc[3];
        stroker->angle_बहिर्गमनम् = angle_अन्तर्;
        stroker->पङ्क्तिः_join = FT_STROKER_LINEJOIN_आवर्तनम्;

        error = ft_stroker_process_corner( stroker, 0 );

        /* reinstate line join style */
        stroker->पङ्क्तिः_join = stroker->पङ्क्तिः_join_saved;
      }

      यदि ( error )
        गच्छ Exit;

      /* the arc's angle is small enough; we can add it directly to each */
      /* border                                                          */
      {
        FT_सदिशम्        ctrl1, ctrl2, अन्तः;
        FT_Angle         theta1, phi1, theta2, phi2, rotate, alpha0 = 0;
        FT_Fixed         length1, length2;
        FT_StrokeBorder  border;
        FT_Int           side;


        theta1  = FT_Angle_Diff( angle_अन्तर्,  angle_mid ) / 2;
        theta2  = FT_Angle_Diff( angle_mid, angle_बहिर्गमनम् ) / 2;
        phi1    = ft_angle_माध्यम्( angle_अन्तर्,  angle_mid );
        phi2    = ft_angle_माध्यम्( angle_mid, angle_बहिर्गमनम् );
        length1 = FT_DivFix( stroker->radius, FT_Cos( theta1 ) );
        length2 = FT_DivFix( stroker->radius, FT_Cos( theta2 ) );

        /* compute direction of original arc */
        यदि ( stroker->handle_wide_strokes )
          alpha0 = FT_Atan2( arc[0].x - arc[3].x, arc[0].y - arc[3].y );

        प्रति ( border = stroker->borders, side = 0;
              side <= 1;
              side++, border++ )
        {
          rotate = FT_SIDE_गन्तव्यम्_ROTATE( side );

          /* compute control points */
          FT_सदिशम्_From_Polar( &ctrl1, length1, phi1 + rotate );
          ctrl1.x += arc[2].x;
          ctrl1.y += arc[2].y;

          FT_सदिशम्_From_Polar( &ctrl2, length2, phi2 + rotate );
          ctrl2.x += arc[1].x;
          ctrl2.y += arc[1].y;

          /* compute end point */
          FT_सदिशम्_From_Polar( &अन्तः, stroker->radius, angle_बहिर्गमनम् + rotate );
          अन्तः.x += arc[0].x;
          अन्तः.y += arc[0].y;

          यदि ( stroker->handle_wide_strokes )
          {
            FT_सदिशम्  start;
            FT_Angle   alpha1;


            /* determine whether the border radius is greater than the */
            /* radius of curvature of the original arc                 */
            start = border->points[border->अङ्कः_points - 1];

            alpha1 = FT_Atan2( अन्तः.x - start.x, अन्तः.y - start.y );

            /* is the direction of the border arc opposite to */
            /* that of the original arc? */
            यदि ( ft_pos_abs( FT_Angle_Diff( alpha0, alpha1 ) ) >
                   FT_ANGLE_PI / 2                             )
            {
              FT_Angle   beta, gamma;
              FT_सदिशम्  bvec, delta;
              FT_Fixed   blen, sinA, sinB, alen;


              /* use the sine rule to find the intersection point */
              beta  = FT_Atan2( arc[3].x - start.x, arc[3].y - start.y );
              gamma = FT_Atan2( arc[0].x - अन्तः.x,   arc[0].y - अन्तः.y );

              bvec.x = अन्तः.x - start.x;
              bvec.y = अन्तः.y - start.y;

              blen = FT_सदिशम्_दीर्घता( &bvec );

              sinA = ft_pos_abs( FT_Sin( alpha1 - gamma ) );
              sinB = ft_pos_abs( FT_Sin( beta - gamma ) );

              alen = FT_MulDiv( blen, sinA, sinB );

              FT_सदिशम्_From_Polar( &delta, alen, beta );
              delta.x += start.x;
              delta.y += start.y;

              /* circumnavigate the negative sector backwards */
              border->movable = FALSE;
              error = ft_stroke_border_lineto( border, &delta, FALSE );
              यदि ( error )
                गच्छ Exit;
              error = ft_stroke_border_lineto( border, &अन्तः, FALSE );
              यदि ( error )
                गच्छ Exit;
              error = ft_stroke_border_cubicto( border,
                                                &ctrl2,
                                                &ctrl1,
                                                &start );
              यदि ( error )
                गच्छ Exit;
              /* and then move to the endpoint */
              error = ft_stroke_border_lineto( border, &अन्तः, FALSE );
              यदि ( error )
                गच्छ Exit;

              अनुवर्त;
            }

            /* else fall through */
          }

          /* simply add an arc */
          error = ft_stroke_border_cubicto( border, &ctrl1, &ctrl2, &अन्तः );
          यदि ( error )
            गच्छ Exit;
        }
      }

      stroker->angle_अन्तर् = angle_बहिर्गमनम्;

      यदि ( arc == bez_राशिः )
        भङ्ग;
      arc -= 3;
    } यावत् ( 1 );

    stroker->center      = *गन्तव्यम्;
    stroker->पङ्क्तिः_दीर्घता = 0;

  Exit:
    प्रत्यागम error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Stroker_Begin_Sub_मार्गः( FT_Stroker  stroker,
                           FT_सदिशम्*  गन्तव्यम्,
                           FT_Bool     उद्घाटय )
  {
    यदि ( !stroker || !गन्तव्यम् )
      प्रत्यागम FT_THROW( Invalid_तर्कः );

    /* We cannot process the first point, because there is not enough      */
    /* information regarding its corner/cap.  The latter will be processed */
    /* in the `FT_Stroker_EndSubPath' routine.                             */
    /*                                                                     */
    stroker->first_point  = TRUE;
    stroker->center       = *गन्तव्यम्;
    stroker->subpath_open = उद्घाटय;

    /* Determine if we need to check whether the border radius is greater */
    /* than the radius of curvature of a curve, to handle this case       */
    /* specially.  This is only required if bevel joins or butt caps may  */
    /* be created, because round & miter joins and round & square caps    */
    /* cover the negative sector created with wide strokes.               */
    stroker->handle_wide_strokes =
      FT_BOOL( stroker->पङ्क्तिः_join != FT_STROKER_LINEJOIN_आवर्तनम्  ||
               ( stroker->subpath_open                        &&
                 stroker->पङ्क्तिः_cap == FT_STROKER_LINECAP_BUTT ) );

    /* record the subpath start point for each border */
    stroker->subpath_start = *गन्तव्यम्;

    stroker->angle_अन्तर् = 0;

    प्रत्यागम FT_Err_Ok;
  }


  स्थिर FT_Error
  ft_stroker_सङ्कलय_reverse_वामः( FT_Stroker  stroker,
                               FT_Bool     उद्घाटय )
  {
    FT_StrokeBorder  दक्षिणः = stroker->borders + 0;
    FT_StrokeBorder  वामः  = stroker->borders + 1;
    FT_Int           new_points;
    FT_Error         error = FT_Err_Ok;


    FT_ASSERT( वामः->start >= 0 );

    new_points = (FT_Int)वामः->अङ्कः_points - वामः->start;
    यदि ( new_points > 0 )
    {
      error = ft_stroke_border_grow( दक्षिणः, (FT_UInt)new_points );
      यदि ( error )
        गच्छ Exit;

      {
        FT_सदिशम्*  dst_point = दक्षिणः->points + दक्षिणः->अङ्कः_points;
        FT_Byte*    dst_चिह्नकम्   = दक्षिणः->चिह्नकानि   + दक्षिणः->अङ्कः_points;
        FT_सदिशम्*  src_point = वामः->points  + वामः->अङ्कः_points - 1;
        FT_Byte*    src_चिह्नकम्   = वामः->चिह्नकानि    + वामः->अङ्कः_points - 1;


        यावत् ( src_point >= वामः->points + वामः->start )
        {
          *dst_point = *src_point;
          *dst_चिह्नकम्   = *src_चिह्नकम्;

          यदि ( उद्घाटय )
            dst_चिह्नकम्[0] &= ~FT_STROKE_चिह्नकम्_BEGIN_अन्तः;
          अन्यथा
          {
            FT_Byte  ttag =
                       (FT_Byte)( dst_चिह्नकम्[0] & FT_STROKE_चिह्नकम्_BEGIN_अन्तः );


            /* switch begin/end tags if necessary */
            यदि ( ttag == FT_STROKE_चिह्नकम्_BEGIN ||
                 ttag == FT_STROKE_चिह्नकम्_अन्तः   )
              dst_चिह्नकम्[0] ^= FT_STROKE_चिह्नकम्_BEGIN_अन्तः;
          }

          src_point--;
          src_चिह्नकम्--;
          dst_point++;
          dst_चिह्नकम्++;
        }
      }

      वामः->अङ्कः_points   = (FT_UInt)वामः->start;
      दक्षिणः->अङ्कः_points += (FT_UInt)new_points;

      दक्षिणः->movable = FALSE;
      वामः->movable  = FALSE;
    }

  Exit:
    प्रत्यागम error;
  }


  /* documentation is in ftstroke.h */

  /* there's a lot of magic in this function! */
  FT_EXPORT_DEF( FT_Error )
  FT_Stroker_अन्तः_Sub_मार्गः( FT_Stroker  stroker )
  {
    FT_Error  error = FT_Err_Ok;


    यदि ( !stroker )
    {
      error = FT_THROW( Invalid_तर्कः );
      गच्छ Exit;
    }

    /* don't try to end the path if no segments have been generated */
    यदि ( stroker->first_point )
      गच्छ Exit;

    यदि ( stroker->subpath_open )
    {
      FT_StrokeBorder  दक्षिणः = stroker->borders;


      /* All right, this is an opened path, we need to add a cap between */
      /* right & left, add the reverse of left, then add a final cap     */
      /* between left & right.                                           */
      error = ft_stroker_cap( stroker, stroker->angle_अन्तर्, 0 );
      यदि ( error )
        गच्छ Exit;

      /* add reversed points from `left' to `right' */
      error = ft_stroker_सङ्कलय_reverse_वामः( stroker, TRUE );
      यदि ( error )
        गच्छ Exit;

      /* now add the final cap */
      stroker->center = stroker->subpath_start;
      error = ft_stroker_cap( stroker,
                              stroker->subpath_angle + FT_ANGLE_PI, 0 );
      यदि ( error )
        गच्छ Exit;

      /* Now end the right subpath accordingly.  The left one is */
      /* rewind and doesn't need further processing.             */
      ft_stroke_border_close( दक्षिणः, FALSE );
    }
    अन्यथा
    {
      /* close the path if needed */
      यदि ( !FT_IS_SMALL( stroker->center.x - stroker->subpath_start.x ) ||
           !FT_IS_SMALL( stroker->center.y - stroker->subpath_start.y ) )
      {
         error = FT_Stroker_पङ्क्तिः_गन्तव्यम्( stroker, &stroker->subpath_start );
         यदि ( error )
           गच्छ Exit;
      }

      /* process the corner */
      stroker->angle_बहिर्गमनम् = stroker->subpath_angle;

      error = ft_stroker_process_corner( stroker,
                                         stroker->subpath_पङ्क्तिः_दीर्घता );
      यदि ( error )
        गच्छ Exit;

      /* then end our two subpaths */
      ft_stroke_border_close( stroker->borders + 0, FALSE );
      ft_stroke_border_close( stroker->borders + 1, TRUE );
    }

  Exit:
    प्रत्यागम error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Stroker_GetBorderCounts( FT_Stroker        stroker,
                              FT_StrokerBorder  border,
                              FT_UInt          *anum_points,
                              FT_UInt          *anum_contours )
  {
    FT_UInt   अङ्कः_points = 0, अङ्कः_contours = 0;
    FT_Error  error;


    यदि ( !stroker || border > 1 )
    {
      error = FT_THROW( Invalid_तर्कः );
      गच्छ Exit;
    }

    error = ft_stroke_border_get_counts( stroker->borders + border,
                                         &अङ्कः_points, &अङ्कः_contours );
  Exit:
    यदि ( anum_points )
      *anum_points = अङ्कः_points;

    यदि ( anum_contours )
      *anum_contours = अङ्कः_contours;

    प्रत्यागम error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Stroker_GetCounts( FT_Stroker  stroker,
                        FT_UInt    *anum_points,
                        FT_UInt    *anum_contours )
  {
    FT_UInt   count1, count2, अङ्कः_points   = 0;
    FT_UInt   count3, count4, अङ्कः_contours = 0;
    FT_Error  error;


    यदि ( !stroker )
    {
      error = FT_THROW( Invalid_तर्कः );
      गच्छ Exit;
    }

    error = ft_stroke_border_get_counts( stroker->borders + 0,
                                         &count1, &count2 );
    यदि ( error )
      गच्छ Exit;

    error = ft_stroke_border_get_counts( stroker->borders + 1,
                                         &count3, &count4 );
    यदि ( error )
      गच्छ Exit;

    अङ्कः_points   = count1 + count3;
    अङ्कः_contours = count2 + count4;

  Exit:
    यदि ( anum_points )
      *anum_points   = अङ्कः_points;

    यदि ( anum_contours )
      *anum_contours = अङ्कः_contours;

    प्रत्यागम error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( शून्य )
  FT_Stroker_ExportBorder( FT_Stroker        stroker,
                           FT_StrokerBorder  border,
                           FT_Outline*       outline )
  {
    यदि ( !stroker || !outline )
      प्रत्यागम;

    यदि ( border == FT_STROKER_BORDER_वामः  ||
         border == FT_STROKER_BORDER_दक्षिणः )
    {
      FT_StrokeBorder  sborder = & stroker->borders[border];


      यदि ( sborder->valid )
        ft_stroke_border_export( sborder, outline );
    }
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( शून्य )
  FT_Stroker_Export( FT_Stroker   stroker,
                     FT_Outline*  outline )
  {
    FT_Stroker_ExportBorder( stroker, FT_STROKER_BORDER_वामः, outline );
    FT_Stroker_ExportBorder( stroker, FT_STROKER_BORDER_दक्षिणः, outline );
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
    FT_सदिशम्   v_चरमः;
    FT_सदिशम्   v_control;
    FT_सदिशम्   v_start;

    FT_सदिशम्*  point;
    FT_सदिशम्*  सीमा;
    FT_Byte*    चिह्नकानि;

    FT_Error    error;

    FT_Int      n;         /* index of contour in outline     */
    FT_Int      first;     /* index of first point in contour */
    FT_Int      चरमः;      /* index of last point in contour  */

    FT_Int      चिह्नकम्;       /* current point's state           */


    यदि ( !outline )
      प्रत्यागम FT_THROW( Invalid_Outline );

    यदि ( !stroker )
      प्रत्यागम FT_THROW( Invalid_तर्कः );

    FT_Stroker_Rewind( stroker );

    चरमः = -1;
    प्रति ( n = 0; n < outline->n_contours; n++ )
    {
      first = चरमः + 1;
      चरमः  = outline->contours[n];

      /* skip empty points; we don't stroke these */
      यदि ( चरमः <= first )
        अनुवर्त;

      सीमा = outline->points + चरमः;

      v_start = outline->points[first];
      v_चरमः  = outline->points[चरमः];

      v_control = v_start;

      point = outline->points + first;
      चिह्नकानि  = outline->चिह्नकानि   + first;
      चिह्नकम्   = FT_वक्रम्_चिह्नकम्( चिह्नकानि[0] );

      /* check first point to determine origin */
      यदि ( चिह्नकम् != FT_वक्रम्_चिह्नकम्_ON )
      {
        FT_Int  चरमः_चिह्नकम् = FT_वक्रम्_चिह्नकम्( outline->चिह्नकानि[चरमः] );


        /* First point is a control point.  Yes, this happens. */
        यदि ( चरमः_चिह्नकम् == FT_वक्रम्_चिह्नकम्_ON )
        {
          /* start at last point if it is on the curve */
          v_start = v_चरमः;
          सीमा--;
        }
        अन्यथा
        {
          यदि ( चरमः_चिह्नकम् != चिह्नकम् )
            गच्छ Invalid_Outline;

          /* if both first and last points are controls, */
          /* start at their middle                       */
          v_start.x = ( v_start.x + v_चरमः.x ) / 2;
          v_start.y = ( v_start.y + v_चरमः.y ) / 2;
        }
        point--;
        चिह्नकानि--;
      }

      error = FT_Stroker_Begin_Sub_मार्गः( stroker, &v_start, opened );
      यदि ( error )
        गच्छ Exit;

      यावत् ( point < सीमा )
      {
        point++;
        चिह्नकानि++;

        चिह्नकम् = FT_वक्रम्_चिह्नकम्( चिह्नकानि[0] );
        विकल्प ( चिह्नकम् )
        {
        पक्ष FT_वक्रम्_चिह्नकम्_ON:  /* emit a single line_to */
          {
            FT_सदिशम्  vec;


            vec.x = point->x;
            vec.y = point->y;

            error = FT_Stroker_पङ्क्तिः_गन्तव्यम्( stroker, &vec );
            यदि ( error )
              गच्छ Exit;
            अनुवर्त;
          }

        पक्ष FT_वक्रम्_चिह्नकम्_CONIC:  /* consume conic arcs */
          v_control.x = point->x;
          v_control.y = point->y;

        Do_Conic:
          यदि ( point < सीमा )
          {
            FT_सदिशम्  vec;
            FT_सदिशम्  v_मध्यः;


            point++;
            चिह्नकानि++;
            चिह्नकम् = FT_वक्रम्_चिह्नकम्( चिह्नकानि[0] );

            vec = point[0];

            यदि ( चिह्नकम् == FT_वक्रम्_चिह्नकम्_ON )
            {
              error = FT_Stroker_Conic_गन्तव्यम्( stroker, &v_control, &vec );
              यदि ( error )
                गच्छ Exit;
              अनुवर्त;
            }

            यदि ( चिह्नकम् != FT_वक्रम्_चिह्नकम्_CONIC )
              गच्छ Invalid_Outline;

            v_मध्यः.x = ( v_control.x + vec.x ) / 2;
            v_मध्यः.y = ( v_control.y + vec.y ) / 2;

            error = FT_Stroker_Conic_गन्तव्यम्( stroker, &v_control, &v_मध्यः );
            यदि ( error )
              गच्छ Exit;

            v_control = vec;
            गच्छ Do_Conic;
          }

          error = FT_Stroker_Conic_गन्तव्यम्( stroker, &v_control, &v_start );
          गच्छ Close;

        स्वभाव:  /* FT_CURVE_TAG_CUBIC */
          {
            FT_सदिशम्  vec1, vec2;


            यदि ( point + 1 > सीमा                             ||
                 FT_वक्रम्_चिह्नकम्( चिह्नकानि[1] ) != FT_वक्रम्_चिह्नकम्_CUBIC )
              गच्छ Invalid_Outline;

            point += 2;
            चिह्नकानि  += 2;

            vec1 = point[-2];
            vec2 = point[-1];

            यदि ( point <= सीमा )
            {
              FT_सदिशम्  vec;
              FT_सदिशम्  v_मध्यः;


              चिह्नकम् = FT_वक्रम्_चिह्नकम्( चिह्नकानि[0] );
              vec = point[0];

              यदि ( चिह्नकम् == FT_वक्रम्_चिह्नकम्_CUBIC )
              {
                v_मध्यः.x = ( vec2.x + vec.x ) / 2;
                v_मध्यः.y = ( vec2.y + vec.y ) / 2;

                vec = v_मध्यः;
              }
              अन्यथा यदि ( चिह्नकम् != FT_वक्रम्_चिह्नकम्_ON )
                गच्छ Invalid_Outline;

              error = FT_Stroker_Cubic_गन्तव्यम्( stroker, &vec1, &vec2, &vec );
              यदि ( error )
                गच्छ Exit;

              यदि ( चिह्नकम् == FT_वक्रम्_चिह्नकम्_CUBIC )
              {
                point--;
                चिह्नकानि--;
              }
              अनुवर्त;
            }

            error = FT_Stroker_Cubic_गन्तव्यम्( stroker, &vec1, &vec2, &v_start );
            गच्छ Close;
          }
        }
      }

    Close:
      यदि ( error )
        गच्छ Exit;

      error = FT_Stroker_अन्तः_Sub_मार्गः( stroker );
      यदि ( error )
        गच्छ Exit;
    }

    प्रत्यागम FT_Err_Ok;

  Exit:
    प्रत्यागम error;

  Invalid_Outline:
    प्रत्यागम FT_THROW( Invalid_Outline );
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Glyph_Stroke( FT_Glyph    *pglyph,
                   FT_Stroker   stroker,
                   FT_Bool      destroy )
  {
    FT_Error  error = FT_THROW( Invalid_तर्कः );
    FT_Glyph  glyph = रिक्त;


    यदि ( !pglyph )
      गच्छ Exit;

    glyph = *pglyph;
    यदि ( !glyph || glyph->clazz != &ft_outline_glyph_class )
      गच्छ Exit;

    {
      FT_Glyph  copy;


      error = FT_Glyph_Copy( glyph, &copy );
      यदि ( error )
        गच्छ Exit;

      glyph = copy;
    }

    {
      FT_OutlineGlyph  oglyph  = (FT_OutlineGlyph)glyph;
      FT_Outline*      outline = &oglyph->outline;
      FT_UInt          अङ्कः_points, अङ्कः_contours;


      error = FT_Stroker_ParseOutline( stroker, outline, FALSE );
      यदि ( error )
        गच्छ Fail;

      error = FT_Stroker_GetCounts( stroker, &अङ्कः_points, &अङ्कः_contours );
      यदि ( error )
        गच्छ Fail;

      FT_Outline_समाप्तम्( glyph->library, outline );

      error = FT_Outline_New( glyph->library,
                              अङ्कः_points,
                              (FT_Int)अङ्कः_contours,
                              outline );
      यदि ( error )
        गच्छ Fail;

      outline->n_points   = 0;
      outline->n_contours = 0;

      FT_Stroker_Export( stroker, outline );
    }

    यदि ( destroy )
      FT_समाप्तम्_Glyph( *pglyph );

    *pglyph = glyph;
    गच्छ Exit;

  Fail:
    FT_समाप्तम्_Glyph( glyph );
    glyph = रिक्त;

    यदि ( !destroy )
      *pglyph = रिक्त;

  Exit:
    प्रत्यागम error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Glyph_StrokeBorder( FT_Glyph    *pglyph,
                         FT_Stroker   stroker,
                         FT_Bool      inside,
                         FT_Bool      destroy )
  {
    FT_Error  error = FT_THROW( Invalid_तर्कः );
    FT_Glyph  glyph = रिक्त;


    यदि ( !pglyph )
      गच्छ Exit;

    glyph = *pglyph;
    यदि ( !glyph || glyph->clazz != &ft_outline_glyph_class )
      गच्छ Exit;

    {
      FT_Glyph  copy;


      error = FT_Glyph_Copy( glyph, &copy );
      यदि ( error )
        गच्छ Exit;

      glyph = copy;
    }

    {
      FT_OutlineGlyph   oglyph  = (FT_OutlineGlyph)glyph;
      FT_StrokerBorder  border;
      FT_Outline*       outline = &oglyph->outline;
      FT_UInt           अङ्कः_points, अङ्कः_contours;


      border = FT_Outline_GetOutsideBorder( outline );
      यदि ( inside )
      {
        यदि ( border == FT_STROKER_BORDER_वामः )
          border = FT_STROKER_BORDER_दक्षिणः;
        अन्यथा
          border = FT_STROKER_BORDER_वामः;
      }

      error = FT_Stroker_ParseOutline( stroker, outline, FALSE );
      यदि ( error )
        गच्छ Fail;

      FT_Stroker_GetBorderCounts( stroker, border,
                                  &अङ्कः_points, &अङ्कः_contours );

      FT_Outline_समाप्तम्( glyph->library, outline );

      error = FT_Outline_New( glyph->library,
                              अङ्कः_points,
                              (FT_Int)अङ्कः_contours,
                              outline );
      यदि ( error )
        गच्छ Fail;

      outline->n_points   = 0;
      outline->n_contours = 0;

      FT_Stroker_ExportBorder( stroker, border, outline );
    }

    यदि ( destroy )
      FT_समाप्तम्_Glyph( *pglyph );

    *pglyph = glyph;
    गच्छ Exit;

  Fail:
    FT_समाप्तम्_Glyph( glyph );
    glyph = रिक्त;

    यदि ( !destroy )
      *pglyph = रिक्त;

  Exit:
    प्रत्यागम error;
  }


/*अन्तः*/
