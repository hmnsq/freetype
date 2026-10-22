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
  FT_CALLBACK_TABLE ध्रुव FT_Glyph_Class  ft_outline_glyph_class;


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_StrokerBorder )
  FT_Outline_GetInsideBorder( FT_Outline*  outline )
  {
    FT_Orientation  o = FT_Outline_Get_Orientation( outline );


    प्रत्यागम o == FT_ORIENTATION_TRUETYPE ? FT_STROKER_BORDER_उजवा
                                        : FT_STROKER_BORDER_डावा;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_StrokerBorder )
  FT_Outline_GetOutsideBorder( FT_Outline*  outline )
  {
    FT_Orientation  o = FT_Outline_Get_Orientation( outline );


    प्रत्यागम o == FT_ORIENTATION_TRUETYPE ? FT_STROKER_BORDER_डावा
                                        : FT_STROKER_BORDER_उजवा;
  }


  /*************************************************************************/
  /*************************************************************************/
  /*****                                                               *****/
  /*****                      BEZIER COMPUTATIONS                      *****/
  /*****                                                               *****/
  /*************************************************************************/
  /*************************************************************************/

#define FT_SMALL_CONIC_THRESHOLD  ( FT_ANGLE_PI / 6 )
#define FT_SMALL_CUBIC_THRESHOLD  ( FT_ANGLE_PI / 8 )

#define FT_EPSILON  2

#define FT_IS_SMALL( x )  ( (x) > -FT_EPSILON && (x) < FT_EPSILON )


  स्थिर FT_Pos
  ft_pos_abs( FT_Pos  x )
  {
    प्रत्यागम x >= 0 ? x : -x;
  }


  स्थिर शून्य
  ft_conic_split( FT_Vector*  base )
  {
    FT_Pos  a, b;


    base[4].x = base[2].x;
    a = base[0].x + base[1].x;
    b = base[1].x + base[2].x;
    base[3].x = b >> 1;
    base[2].x = ( a + b ) >> 2;
    base[1].x = a >> 1;

    base[4].y = base[2].y;
    a = base[0].y + base[1].y;
    b = base[1].y + base[2].y;
    base[3].y = b >> 1;
    base[2].y = ( a + b ) >> 2;
    base[1].y = a >> 1;
  }


  स्थिर FT_Bool
  ft_conic_is_small_enough( FT_Vector*  base,
                            FT_Angle   *angle_in,
                            FT_Angle   *angle_out )
  {
    FT_Vector  d1, d2;
    FT_Angle   theta;
    FT_Int     close1, close2;


    d1.x = base[1].x - base[2].x;
    d1.y = base[1].y - base[2].y;
    d2.x = base[0].x - base[1].x;
    d2.y = base[0].y - base[1].y;

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
        *angle_in  =
        *angle_out = FT_Atan2( d2.x, d2.y );
      }
    }
    अन्यथा /* !close1 */
    {
      यदि ( close2 )
      {
        *angle_in  =
        *angle_out = FT_Atan2( d1.x, d1.y );
      }
      अन्यथा
      {
        *angle_in  = FT_Atan2( d1.x, d1.y );
        *angle_out = FT_Atan2( d2.x, d2.y );
      }
    }

    theta = ft_pos_abs( FT_Angle_Diff( *angle_in, *angle_out ) );

    प्रत्यागम FT_BOOL( theta < FT_SMALL_CONIC_THRESHOLD );
  }


  स्थिर शून्य
  ft_cubic_split( FT_Vector*  base )
  {
    FT_Pos  a, b, c;


    base[6].x = base[3].x;
    a = base[0].x + base[1].x;
    b = base[1].x + base[2].x;
    c = base[2].x + base[3].x;
    base[5].x = c >> 1;
    c += b;
    base[4].x = c >> 2;
    base[1].x = a >> 1;
    a += b;
    base[2].x = a >> 2;
    base[3].x = ( a + c ) >> 3;

    base[6].y = base[3].y;
    a = base[0].y + base[1].y;
    b = base[1].y + base[2].y;
    c = base[2].y + base[3].y;
    base[5].y = c >> 1;
    c += b;
    base[4].y = c >> 2;
    base[1].y = a >> 1;
    a += b;
    base[2].y = a >> 2;
    base[3].y = ( a + c ) >> 3;
  }


  /* Return the average of `angle1' and `angle2'.            */
  /* This gives correct result even if `angle1' and `angle2' */
  /* have opposite signs.                                    */
  स्थिर FT_Angle
  ft_angle_mean( FT_Angle  angle1,
                 FT_Angle  angle2 )
  {
    प्रत्यागम angle1 + FT_Angle_Diff( angle1, angle2 ) / 2;
  }


  स्थिर FT_Bool
  ft_cubic_is_small_enough( FT_Vector*  base,
                            FT_Angle   *angle_in,
                            FT_Angle   *angle_mid,
                            FT_Angle   *angle_out )
  {
    FT_Vector  d1, d2, d3;
    FT_Angle   theta1, theta2;
    FT_Int     close1, close2, close3;


    d1.x = base[2].x - base[3].x;
    d1.y = base[2].y - base[3].y;
    d2.x = base[1].x - base[2].x;
    d2.y = base[1].y - base[2].y;
    d3.x = base[0].x - base[1].x;
    d3.y = base[0].y - base[1].y;

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
          *angle_in  =
          *angle_mid =
          *angle_out = FT_Atan2( d3.x, d3.y );
        }
      }
      अन्यथा /* !close2 */
      {
        यदि ( close3 )
        {
          *angle_in  =
          *angle_mid =
          *angle_out = FT_Atan2( d2.x, d2.y );
        }
        अन्यथा /* !close3 */
        {
          *angle_in  =
          *angle_mid = FT_Atan2( d2.x, d2.y );
          *angle_out = FT_Atan2( d3.x, d3.y );
        }
      }
    }
    अन्यथा /* !close1 */
    {
      यदि ( close2 )
      {
        यदि ( close3 )
        {
          *angle_in  =
          *angle_mid =
          *angle_out = FT_Atan2( d1.x, d1.y );
        }
        अन्यथा /* !close3 */
        {
          *angle_in  = FT_Atan2( d1.x, d1.y );
          *angle_out = FT_Atan2( d3.x, d3.y );
          *angle_mid = ft_angle_mean( *angle_in, *angle_out );
        }
      }
      अन्यथा /* !close2 */
      {
        यदि ( close3 )
        {
          *angle_in  = FT_Atan2( d1.x, d1.y );
          *angle_mid =
          *angle_out = FT_Atan2( d2.x, d2.y );
        }
        अन्यथा /* !close3 */
        {
          *angle_in  = FT_Atan2( d1.x, d1.y );
          *angle_mid = FT_Atan2( d2.x, d2.y );
          *angle_out = FT_Atan2( d3.x, d3.y );
        }
      }
    }

    theta1 = ft_pos_abs( FT_Angle_Diff( *angle_in,  *angle_mid ) );
    theta2 = ft_pos_abs( FT_Angle_Diff( *angle_mid, *angle_out ) );

    प्रत्यागम FT_BOOL( theta1 < FT_SMALL_CUBIC_THRESHOLD &&
                    theta2 < FT_SMALL_CUBIC_THRESHOLD );
  }


  /*************************************************************************/
  /*************************************************************************/
  /*****                                                               *****/
  /*****                       STROKE BORDERS                          *****/
  /*****                                                               *****/
  /*************************************************************************/
  /*************************************************************************/

  प्रकारनिर्देश गणना  FT_StrokeTags_
  {
    FT_STROKE_TAG_ON    = 1,   /* on-curve point  */
    FT_STROKE_TAG_CUBIC = 2,   /* cubic off-point */
    FT_STROKE_TAG_BEGIN = 4,   /* sub-path start  */
    FT_STROKE_TAG_END   = 8    /* sub-path end    */

  } FT_StrokeTags;

#define  FT_STROKE_TAG_BEGIN_END  ( FT_STROKE_TAG_BEGIN | FT_STROKE_TAG_END )

  प्रकारनिर्देश संरचना  FT_StrokeBorderRec_
  {
    FT_UInt     अंक_points;
    FT_UInt     max_points;
    FT_Vector*  points;
    FT_Byte*    tags;
    FT_Bool     movable;  /* TRUE for ends of lineto borders */
    FT_Int      start;    /* index of current sub-path start point */
    FT_Memory   memory;
    FT_Bool     valid;

  } FT_StrokeBorderRec, *FT_StrokeBorder;


  स्थिर FT_Error
  ft_stroke_border_grow( FT_StrokeBorder  border,
                         FT_UInt          new_points )
  {
    FT_UInt   old_max = border->max_points;
    FT_UInt   new_max = border->अंक_points + new_points;
    FT_Error  error   = FT_Err_Ok;


    यदि ( new_max > old_max )
    {
      FT_UInt    cur_max = old_max;
      FT_Memory  memory  = border->memory;


      यावत् ( cur_max < new_max )
        cur_max += ( cur_max >> 1 ) + 16;

      यदि ( FT_RENEW_ARRAY( border->points, old_max, cur_max ) ||
           FT_RENEW_ARRAY( border->tags,   old_max, cur_max ) )
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
    FT_UInt  मोजणी = border->अंक_points;


    FT_ASSERT( border->start >= 0 );

    /* don't record empty paths! */
    यदि ( मोजणी <= start + 1U )
      border->अंक_points = start;
    अन्यथा
    {
      /* copy the last point to the start of this sub-path, since */
      /* it contains the `adjusted' starting coordinates          */
      border->अंक_points    = --मोजणी;
      border->points[start] = border->points[मोजणी];
      border->tags[start]   = border->tags[मोजणी];

      यदि ( reverse )
      {
        /* reverse the points */
        {
          FT_Vector*  vec1 = border->points + start + 1;
          FT_Vector*  vec2 = border->points + मोजणी - 1;


          प्रति ( ; vec1 < vec2; vec1++, vec2-- )
          {
            FT_Vector  tmp;


            tmp   = *vec1;
            *vec1 = *vec2;
            *vec2 = tmp;
          }
        }

        /* then the tags */
        {
          FT_Byte*  tag1 = border->tags + start + 1;
          FT_Byte*  tag2 = border->tags + मोजणी - 1;


          प्रति ( ; tag1 < tag2; tag1++, tag2-- )
          {
            FT_Byte  tmp;


            tmp   = *tag1;
            *tag1 = *tag2;
            *tag2 = tmp;
          }
        }
      }

      border->tags[start    ] |= FT_STROKE_TAG_BEGIN;
      border->tags[मोजणी - 1] |= FT_STROKE_TAG_END;
    }

    border->start   = -1;
    border->movable = FALSE;
  }


  स्थिर FT_Error
  ft_stroke_border_lineto( FT_StrokeBorder  border,
                           FT_Vector*       to,
                           FT_Bool          movable )
  {
    FT_Error  error = FT_Err_Ok;


    FT_ASSERT( border->start >= 0 );

    यदि ( border->movable )
    {
      /* move last point */
      border->points[border->अंक_points - 1] = *to;
    }
    अन्यथा
    {
      /* don't add zero-length lineto, but always add moveto */
      यदि ( border->अंक_points > (FT_UInt)border->start                     &&
           FT_IS_SMALL( border->points[border->अंक_points - 1].x - to->x ) &&
           FT_IS_SMALL( border->points[border->अंक_points - 1].y - to->y ) )
        प्रत्यागम error;

      /* add one point */
      error = ft_stroke_border_grow( border, 1 );
      यदि ( !error )
      {
        FT_Vector*  vec = border->points + border->अंक_points;
        FT_Byte*    tag = border->tags   + border->अंक_points;


        vec[0] = *to;
        tag[0] = FT_STROKE_TAG_ON;

        border->अंक_points += 1;
      }
    }
    border->movable = movable;
    प्रत्यागम error;
  }


  स्थिर FT_Error
  ft_stroke_border_conicto( FT_StrokeBorder  border,
                            FT_Vector*       control,
                            FT_Vector*       to )
  {
    FT_Error  error;


    FT_ASSERT( border->start >= 0 );

    error = ft_stroke_border_grow( border, 2 );
    यदि ( !error )
    {
      FT_Vector*  vec = border->points + border->अंक_points;
      FT_Byte*    tag = border->tags   + border->अंक_points;


      vec[0] = *control;
      vec[1] = *to;

      tag[0] = 0;
      tag[1] = FT_STROKE_TAG_ON;

      border->अंक_points += 2;
    }

    border->movable = FALSE;

    प्रत्यागम error;
  }


  स्थिर FT_Error
  ft_stroke_border_cubicto( FT_StrokeBorder  border,
                            FT_Vector*       control1,
                            FT_Vector*       control2,
                            FT_Vector*       to )
  {
    FT_Error  error;


    FT_ASSERT( border->start >= 0 );

    error = ft_stroke_border_grow( border, 3 );
    यदि ( !error )
    {
      FT_Vector*  vec = border->points + border->अंक_points;
      FT_Byte*    tag = border->tags   + border->अंक_points;


      vec[0] = *control1;
      vec[1] = *control2;
      vec[2] = *to;

      tag[0] = FT_STROKE_TAG_CUBIC;
      tag[1] = FT_STROKE_TAG_CUBIC;
      tag[2] = FT_STROKE_TAG_ON;

      border->अंक_points += 3;
    }

    border->movable = FALSE;

    प्रत्यागम error;
  }


#define FT_ARC_CUBIC_ANGLE  ( FT_ANGLE_PI / 2 )


  स्थिर FT_Error
  ft_stroke_border_arcto( FT_StrokeBorder  border,
                          FT_Vector*       center,
                          FT_Fixed         radius,
                          FT_Angle         angle_start,
                          FT_Angle         angle_diff )
  {
    FT_Fixed   coef;
    FT_Vector  a0, a1, a2, a3;
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
    FT_Vector_From_Polar( &a0, radius, angle_start );
    a1.x = FT_MulFix( -a0.y, coef );
    a1.y = FT_MulFix(  a0.x, coef );

    a0.x += center->x;
    a0.y += center->y;
    a1.x += a0.x;
    a1.y += a0.y;

    प्रति ( i = 1; i <= arcs; i++ )
    {
      /* compute end and second control point */
      FT_Vector_From_Polar( &a3, radius,
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
                           FT_Vector*       to )
  {
    /* close current open path if any ? */
    यदि ( border->start >= 0 )
      ft_stroke_border_close( border, FALSE );

    border->start = (FT_Int)border->अंक_points;
    border->movable = FALSE;

    प्रत्यागम ft_stroke_border_lineto( border, to, FALSE );
  }


  स्थिर शून्य
  ft_stroke_border_init( FT_StrokeBorder  border,
                         FT_Memory        memory )
  {
    border->memory = memory;
    border->points = रिक्त;
    border->tags   = रिक्त;

    border->अंक_points = 0;
    border->max_points = 0;
    border->start      = -1;
    border->valid      = FALSE;
  }


  स्थिर शून्य
  ft_stroke_border_reset( FT_StrokeBorder  border )
  {
    border->अंक_points = 0;
    border->start      = -1;
    border->valid      = FALSE;
  }


  स्थिर शून्य
  ft_stroke_border_done( FT_StrokeBorder  border )
  {
    FT_Memory  memory = border->memory;


    FT_FREE( border->points );
    FT_FREE( border->tags );

    border->अंक_points = 0;
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
    FT_UInt   अंक_points   = 0;
    FT_UInt   अंक_contours = 0;

    FT_UInt     मोजणी      = border->अंक_points;
    FT_Byte*    tags       = border->tags;
    FT_Int      in_contour = 0;


    प्रति ( ; मोजणी > 0; मोजणी--, अंक_points++, tags++ )
    {
      यदि ( tags[0] & FT_STROKE_TAG_BEGIN )
      {
        यदि ( in_contour != 0 )
          गच्छ Fail;

        in_contour = 1;
      }
      अन्यथा यदि ( in_contour == 0 )
        गच्छ Fail;

      यदि ( tags[0] & FT_STROKE_TAG_END )
      {
        in_contour = 0;
        अंक_contours++;
      }
    }

    यदि ( in_contour != 0 )
      गच्छ Fail;

    border->valid = TRUE;

  Exit:
    *anum_points   = अंक_points;
    *anum_contours = अंक_contours;
    प्रत्यागम error;

  Fail:
    अंक_points   = 0;
    अंक_contours = 0;
    error        = FT_THROW( Invalid_Outline );
    गच्छ Exit;
  }


  स्थिर शून्य
  ft_stroke_border_export( FT_StrokeBorder  border,
                           FT_Outline*      outline )
  {
    /* copy point locations */
    यदि ( border->अंक_points )
      FT_ARRAY_COPY( outline->points + outline->n_points,
                     border->points,
                     border->अंक_points );

    /* copy tags */
    {
      FT_UInt   मोजणी = border->अंक_points;
      FT_Byte*  पठ  = border->tags;
      FT_Byte*  लिख = outline->tags + outline->n_points;


      प्रति ( ; मोजणी > 0; मोजणी--, पठ++, लिख++ )
      {
        यदि ( *पठ & FT_STROKE_TAG_ON )
          *लिख = FT_CURVE_TAG_ON;
        अन्यथा यदि ( *पठ & FT_STROKE_TAG_CUBIC )
          *लिख = FT_CURVE_TAG_CUBIC;
        अन्यथा
          *लिख = FT_CURVE_TAG_CONIC;
      }
    }

    /* copy contours */
    {
      FT_UInt     मोजणी = border->अंक_points;
      FT_Byte*    tags  = border->tags;
      FT_UShort*  लिख = outline->contours + outline->n_contours;
      FT_UShort   idx   = outline->n_points;


      प्रति ( ; मोजणी > 0; मोजणी--, tags++, idx++ )
      {
        यदि ( *tags & FT_STROKE_TAG_END )
        {
          *लिख++ = idx;
          outline->n_contours++;
        }
      }
    }

    outline->n_points += (FT_UShort)border->अंक_points;

    FT_ASSERT( FT_Outline_Check( outline ) == 0 );
  }


  /*************************************************************************/
  /*************************************************************************/
  /*****                                                               *****/
  /*****                           STROKER                             *****/
  /*****                                                               *****/
  /*************************************************************************/
  /*************************************************************************/

#define FT_SIDE_TO_ROTATE( s )   ( FT_ANGLE_PI2 - (s) * FT_ANGLE_PI )

  प्रकारनिर्देश संरचना  FT_StrokerRec_
  {
    FT_Angle             angle_in;             /* direction into curr join */
    FT_Angle             angle_out;            /* direction out of join  */
    FT_Vector            center;               /* current position */
    FT_Fixed             ओळ_length;          /* length of last lineto */
    FT_Bool              first_point;          /* is this the start? */
    FT_Bool              subpath_open;         /* is the subpath open? */
    FT_Angle             subpath_angle;        /* subpath start direction */
    FT_Vector            subpath_start;        /* subpath start position */
    FT_Fixed             subpath_ओळ_length;  /* subpath start lineto len */
    FT_Bool              handle_wide_strokes;  /* use wide strokes logic? */

    FT_Stroker_ओळ_Cap   ओळ_cap;
    FT_Stroker_ओळ_Join  ओळ_join;
    FT_Stroker_ओळ_Join  ओळ_join_saved;
    FT_Fixed             miter_limit;
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
    FT_Memory   memory;
    FT_Stroker  stroker = रिक्त;


    यदि ( !library )
      प्रत्यागम FT_THROW( Invalid_Library_Handle );

    यदि ( !astroker )
      प्रत्यागम FT_THROW( Invalid_Argument );

    memory = library->memory;

    यदि ( !FT_NEW( stroker ) )
    {
      stroker->library = library;

      ft_stroke_border_init( &stroker->borders[0], memory );
      ft_stroke_border_init( &stroker->borders[1], memory );
    }

    *astroker = stroker;

    प्रत्यागम error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( शून्य )
  FT_Stroker_Set( FT_Stroker           stroker,
                  FT_Fixed             radius,
                  FT_Stroker_ओळ_Cap   ओळ_cap,
                  FT_Stroker_ओळ_Join  ओळ_join,
                  FT_Fixed             miter_limit )
  {
    यदि ( !stroker )
      प्रत्यागम;

    stroker->radius      = radius;
    stroker->ओळ_cap    = ओळ_cap;
    stroker->ओळ_join   = ओळ_join;
    stroker->miter_limit = miter_limit;

    /* ensure miter limit has sensible value */
    यदि ( stroker->miter_limit < 0x10000L )
      stroker->miter_limit = 0x10000L;

    /* save line join style:                                           */
    /* line join style can be temporarily changed when stroking curves */
    stroker->ओळ_join_saved = ओळ_join;

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
  FT_Stroker_Done( FT_Stroker  stroker )
  {
    यदि ( stroker )
    {
      FT_Memory  memory = stroker->library->memory;


      ft_stroke_border_done( &stroker->borders[0] );
      ft_stroke_border_done( &stroker->borders[1] );

      stroker->library = रिक्त;
      FT_FREE( stroker );
    }
  }


  /* create a circular arc at a corner or cap */
  स्थिर FT_Error
  ft_stroker_arcto( FT_Stroker  stroker,
                    FT_Int      side )
  {
    FT_Angle         total, rotate;
    FT_Fixed         radius = stroker->radius;
    FT_Error         error  = FT_Err_Ok;
    FT_StrokeBorder  border = stroker->borders + side;


    rotate = FT_SIDE_TO_ROTATE( side );

    total = FT_Angle_Diff( stroker->angle_in, stroker->angle_out );
    यदि ( total == FT_ANGLE_PI )
      total = -rotate * 2;

    error = ft_stroke_border_arcto( border,
                                    &stroker->center,
                                    radius,
                                    stroker->angle_in + rotate,
                                    total );
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


    यदि ( stroker->ओळ_cap == FT_STROKER_LINECAP_ROUND )
    {
      /* add a round cap */
      stroker->angle_in  = angle;
      stroker->angle_out = angle + FT_ANGLE_PI;

      error = ft_stroker_arcto( stroker, side );
    }
    अन्यथा
    {
      /* add a square or butt cap */
      FT_Vector        मधला, delta;
      FT_Fixed         radius = stroker->radius;
      FT_StrokeBorder  border = stroker->borders + side;


      /* compute middle point and first angle point */
      FT_Vector_From_Polar( &मधला, radius, angle );
      delta.x = side ?  मधला.y : -मधला.y;
      delta.y = side ? -मधला.x :  मधला.x;

      यदि ( stroker->ओळ_cap == FT_STROKER_LINECAP_SQUARE )
      {
        मधला.x += stroker->center.x;
        मधला.y += stroker->center.y;
      }
      अन्यथा  /* FT_STROKER_LINECAP_BUTT */
      {
        मधला.x  = stroker->center.x;
        मधला.y  = stroker->center.y;
      }

      delta.x  += मधला.x;
      delta.y  += मधला.y;

      error = ft_stroke_border_lineto( border, &delta, FALSE );
      यदि ( error )
        गच्छ Exit;

      /* compute second angle point */
      delta.x = मधला.x - delta.x + मधला.x;
      delta.y = मधला.y - delta.y + मधला.y;

      error = ft_stroke_border_lineto( border, &delta, FALSE );
    }

  Exit:
    प्रत्यागम error;
  }


  /* process an inside corner, i.e. compute intersection */
  स्थिर FT_Error
  ft_stroker_inside( FT_Stroker  stroker,
                     FT_Int      side,
                     FT_Fixed    ओळ_length )
  {
    FT_StrokeBorder  border = stroker->borders + side;
    FT_Angle         phi, theta, rotate;
    FT_Fixed         length;
    FT_Vector        sigma = { 0, 0 };
    FT_Vector        delta;
    FT_Error         error = FT_Err_Ok;
    FT_Bool          intersect;          /* use intersection of lines? */


    rotate = FT_SIDE_TO_ROTATE( side );

    theta = FT_Angle_Diff( stroker->angle_in, stroker->angle_out ) / 2;

    /* Only intersect borders if between two lineto's and both */
    /* lines are long enough (line_length is zero for curves). */
    /* Also avoid U-turns of nearly 180 degree.                */
    यदि ( !border->movable || ओळ_length == 0  ||
         theta > 0x59C000 || theta < -0x59C000 )
      intersect = FALSE;
    अन्यथा
    {
      /* compute minimum required length of lines */
      FT_Fixed  min_length;


      FT_Vector_Unit( &sigma, theta );
      min_length =
        ft_pos_abs( FT_MulDiv( stroker->radius, sigma.y, sigma.x ) );

      intersect = FT_BOOL( min_length                         &&
                           stroker->ओळ_length >= min_length &&
                           ओळ_length          >= min_length );
    }

    यदि ( !intersect )
    {
      FT_Vector_From_Polar( &delta, stroker->radius,
                            stroker->angle_out + rotate );
      delta.x += stroker->center.x;
      delta.y += stroker->center.y;

      border->movable = FALSE;
    }
    अन्यथा
    {
      /* compute median angle */
      phi = stroker->angle_in + theta + rotate;

      length = FT_DivFix( stroker->radius, sigma.x );

      FT_Vector_From_Polar( &delta, length, phi );
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
                      FT_Fixed    ओळ_length )
  {
    FT_StrokeBorder  border = stroker->borders + side;
    FT_Error         error;
    FT_Angle         rotate;


    यदि ( stroker->ओळ_join == FT_STROKER_LINEJOIN_ROUND )
      error = ft_stroker_arcto( stroker, side );
    अन्यथा
    {
      /* this is a mitered (pointed) or beveled (truncated) corner */
      FT_Fixed   radius = stroker->radius;
      FT_Vector  sigma = { 0, 0 };
      FT_Angle   theta = 0, phi = 0;
      FT_Bool    bevel, fixed_bevel;


      rotate = FT_SIDE_TO_ROTATE( side );

      bevel =
        FT_BOOL( stroker->ओळ_join == FT_STROKER_LINEJOIN_BEVEL );

      fixed_bevel =
        FT_BOOL( stroker->ओळ_join != FT_STROKER_LINEJOIN_MITER_VARIABLE );

      /* check miter limit first */
      यदि ( !bevel )
      {
        theta = FT_Angle_Diff( stroker->angle_in, stroker->angle_out ) / 2;

        यदि ( theta == FT_ANGLE_PI2 )
          theta = -rotate;

        phi = stroker->angle_in + theta + rotate;

        FT_Vector_From_Polar( &sigma, stroker->miter_limit, theta );

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
          FT_Vector  delta;


          /* add bevel */
          FT_Vector_From_Polar( &delta,
                                radius,
                                stroker->angle_out + rotate );
          delta.x += stroker->center.x;
          delta.y += stroker->center.y;

          border->movable = FALSE;
          error = ft_stroke_border_lineto( border, &delta, FALSE );
        }
        अन्यथा /* variable bevel or clipped miter */
        {
          /* the miter is truncated */
          FT_Vector  मधला, delta;
          FT_Fixed   coef;


          /* compute middle point and first angle point */
          FT_Vector_From_Polar( &मधला,
                                FT_MulFix( radius, stroker->miter_limit ),
                                phi );

          coef    = FT_DivFix(  0x10000L - sigma.x, sigma.y );
          delta.x = FT_MulFix(  मधला.y, coef );
          delta.y = FT_MulFix( -मधला.x, coef );

          मधला.x += stroker->center.x;
          मधला.y += stroker->center.y;
          delta.x  += मधला.x;
          delta.y  += मधला.y;

          error = ft_stroke_border_lineto( border, &delta, FALSE );
          यदि ( error )
            गच्छ Exit;

          /* compute second angle point */
          delta.x = मधला.x - delta.x + मधला.x;
          delta.y = मधला.y - delta.y + मधला.y;

          error = ft_stroke_border_lineto( border, &delta, FALSE );
          यदि ( error )
            गच्छ Exit;

          /* finally, add an end point; only needed if not lineto */
          /* (line_length is zero for curves)                     */
          यदि ( ओळ_length == 0 )
          {
            FT_Vector_From_Polar( &delta,
                                  radius,
                                  stroker->angle_out + rotate );

            delta.x += stroker->center.x;
            delta.y += stroker->center.y;

            error = ft_stroke_border_lineto( border, &delta, FALSE );
          }
        }
      }
      अन्यथा /* this is a miter (intersection) */
      {
        FT_Fixed   length;
        FT_Vector  delta;


        length = FT_MulDiv( stroker->radius, stroker->miter_limit, sigma.x );

        FT_Vector_From_Polar( &delta, length, phi );
        delta.x += stroker->center.x;
        delta.y += stroker->center.y;

        error = ft_stroke_border_lineto( border, &delta, FALSE );
        यदि ( error )
          गच्छ Exit;

        /* now add an end point; only needed if not lineto */
        /* (line_length is zero for curves)                */
        यदि ( ओळ_length == 0 )
        {
          FT_Vector_From_Polar( &delta,
                                stroker->radius,
                                stroker->angle_out + rotate );
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
                             FT_Fixed    ओळ_length )
  {
    FT_Error  error = FT_Err_Ok;
    FT_Angle  turn;
    FT_Int    inside_side;


    turn = FT_Angle_Diff( stroker->angle_in, stroker->angle_out );

    /* no specific corner processing is required if the turn is 0 */
    यदि ( turn == 0 )
      गच्छ Exit;

    /* when we turn to the right, the inside side is 0 */
    /* otherwise, the inside side is 1 */
    inside_side = ( turn < 0 );

    /* process the inside side */
    error = ft_stroker_inside( stroker, inside_side, ओळ_length );
    यदि ( error )
      गच्छ Exit;

    /* process the outside side */
    error = ft_stroker_outside( stroker, !inside_side, ओळ_length );

  Exit:
    प्रत्यागम error;
  }


  /* add two points to the left and right borders corresponding to the */
  /* start of the subpath                                              */
  स्थिर FT_Error
  ft_stroker_subpath_start( FT_Stroker  stroker,
                            FT_Angle    start_angle,
                            FT_Fixed    ओळ_length )
  {
    FT_Vector        delta;
    FT_Vector        point;
    FT_Error         error;
    FT_StrokeBorder  border;


    FT_Vector_From_Polar( &delta, stroker->radius,
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
    stroker->subpath_ओळ_length = ओळ_length;

  Exit:
    प्रत्यागम error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Stroker_ओळ_To( FT_Stroker  stroker,
                     FT_Vector*  to )
  {
    FT_Error         error = FT_Err_Ok;
    FT_StrokeBorder  border;
    FT_Vector        delta;
    FT_Angle         angle;
    FT_Int           side;
    FT_Fixed         ओळ_length;


    यदि ( !stroker || !to )
      प्रत्यागम FT_THROW( Invalid_Argument );

    delta.x = to->x - stroker->center.x;
    delta.y = to->y - stroker->center.y;

    /* a zero-length lineto is a no-op; avoid creating a spurious corner */
    यदि ( delta.x == 0 && delta.y == 0 )
       गच्छ Exit;

    /* compute length of line */
    ओळ_length = FT_Vector_Length( &delta );

    angle = FT_Atan2( delta.x, delta.y );
    FT_Vector_From_Polar( &delta, stroker->radius, angle + FT_ANGLE_PI2 );

    /* process corner if necessary */
    यदि ( stroker->first_point )
    {
      /* This is the first segment of a subpath.  We need to     */
      /* add a point to each border at their respective starting */
      /* point locations.                                        */
      error = ft_stroker_subpath_start( stroker, angle, ओळ_length );
      यदि ( error )
        गच्छ Exit;
    }
    अन्यथा
    {
      /* process the current corner */
      stroker->angle_out = angle;
      error = ft_stroker_process_corner( stroker, ओळ_length );
      यदि ( error )
        गच्छ Exit;
    }

    /* now add a line segment to both the `inside' and `outside' paths */
    प्रति ( border = stroker->borders, side = 1; side >= 0; side--, border++ )
    {
      FT_Vector  point;


      point.x = to->x + delta.x;
      point.y = to->y + delta.y;

      /* the ends of lineto borders are movable */
      error = ft_stroke_border_lineto( border, &point, TRUE );
      यदि ( error )
        गच्छ Exit;

      delta.x = -delta.x;
      delta.y = -delta.y;
    }

    stroker->angle_in    = angle;
    stroker->center      = *to;
    stroker->ओळ_length = ओळ_length;

  Exit:
    प्रत्यागम error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Stroker_ConicTo( FT_Stroker  stroker,
                      FT_Vector*  control,
                      FT_Vector*  to )
  {
    FT_Error    error = FT_Err_Ok;
    FT_Vector   bez_चळत[34];
    FT_Vector*  arc;
    FT_Vector*  limit = bez_चळत + 30;
    FT_Bool     first_arc = TRUE;


    यदि ( !stroker || !control || !to )
    {
      error = FT_THROW( Invalid_Argument );
      गच्छ Exit;
    }

    /* if all control points are coincident, this is a no-op; */
    /* avoid creating a spurious corner                       */
    यदि ( FT_IS_SMALL( stroker->center.x - control->x ) &&
         FT_IS_SMALL( stroker->center.y - control->y ) &&
         FT_IS_SMALL( control->x        - to->x      ) &&
         FT_IS_SMALL( control->y        - to->y      ) )
    {
       stroker->center = *to;
       गच्छ Exit;
    }

    arc    = bez_चळत;
    arc[0] = *to;
    arc[1] = *control;
    arc[2] = stroker->center;

    कुरु
    {
      FT_Angle  angle_in, angle_out;


      /* initialize with current direction */
      angle_in = angle_out = stroker->angle_in;

      यदि ( arc < limit                                             &&
           !ft_conic_is_small_enough( arc, &angle_in, &angle_out ) )
      {
        यदि ( stroker->first_point )
          stroker->angle_in = angle_in;

        ft_conic_split( arc );
        arc += 2;
        अनुवर्त;
      }

      यदि ( first_arc )
      {
        first_arc = FALSE;

        /* process corner if necessary */
        यदि ( stroker->first_point )
          error = ft_stroker_subpath_start( stroker, angle_in, 0 );
        अन्यथा
        {
          stroker->angle_out = angle_in;
          error = ft_stroker_process_corner( stroker, 0 );
        }
      }
      अन्यथा यदि ( ft_pos_abs( FT_Angle_Diff( stroker->angle_in, angle_in ) ) >
                  FT_SMALL_CONIC_THRESHOLD / 4                             )
      {
        /* if the deviation from one arc to the next is too great, */
        /* add a round corner                                      */
        stroker->center    = arc[2];
        stroker->angle_out = angle_in;
        stroker->ओळ_join = FT_STROKER_LINEJOIN_ROUND;

        error = ft_stroker_process_corner( stroker, 0 );

        /* reinstate line join style */
        stroker->ओळ_join = stroker->ओळ_join_saved;
      }

      यदि ( error )
        गच्छ Exit;

      /* the arc's angle is small enough; we can add it directly to each */
      /* border                                                          */
      {
        FT_Vector        ctrl, end;
        FT_Angle         theta, phi, rotate, alpha0 = 0;
        FT_Fixed         length;
        FT_StrokeBorder  border;
        FT_Int           side;


        theta  = FT_Angle_Diff( angle_in, angle_out ) / 2;
        phi    = angle_in + theta;
        length = FT_DivFix( stroker->radius, FT_Cos( theta ) );

        /* compute direction of original arc */
        यदि ( stroker->handle_wide_strokes )
          alpha0 = FT_Atan2( arc[0].x - arc[2].x, arc[0].y - arc[2].y );

        प्रति ( border = stroker->borders, side = 0;
              side <= 1;
              side++, border++ )
        {
          rotate = FT_SIDE_TO_ROTATE( side );

          /* compute control point */
          FT_Vector_From_Polar( &ctrl, length, phi + rotate );
          ctrl.x += arc[1].x;
          ctrl.y += arc[1].y;

          /* compute end point */
          FT_Vector_From_Polar( &end, stroker->radius, angle_out + rotate );
          end.x += arc[0].x;
          end.y += arc[0].y;

          यदि ( stroker->handle_wide_strokes )
          {
            FT_Vector  start;
            FT_Angle   alpha1;


            /* determine whether the border radius is greater than the */
            /* radius of curvature of the original arc                 */
            start = border->points[border->अंक_points - 1];

            alpha1 = FT_Atan2( end.x - start.x, end.y - start.y );

            /* is the direction of the border arc opposite to */
            /* that of the original arc? */
            यदि ( ft_pos_abs( FT_Angle_Diff( alpha0, alpha1 ) ) >
                   FT_ANGLE_PI / 2                             )
            {
              FT_Angle   beta, gamma;
              FT_Vector  bvec, delta;
              FT_Fixed   blen, sinA, sinB, alen;


              /* use the sine rule to find the intersection point */
              beta  = FT_Atan2( arc[2].x - start.x, arc[2].y - start.y );
              gamma = FT_Atan2( arc[0].x - end.x,   arc[0].y - end.y );

              bvec.x = end.x - start.x;
              bvec.y = end.y - start.y;

              blen = FT_Vector_Length( &bvec );

              sinA = ft_pos_abs( FT_Sin( alpha1 - gamma ) );
              sinB = ft_pos_abs( FT_Sin( beta - gamma ) );

              alen = FT_MulDiv( blen, sinA, sinB );

              FT_Vector_From_Polar( &delta, alen, beta );
              delta.x += start.x;
              delta.y += start.y;

              /* circumnavigate the negative sector backwards */
              border->movable = FALSE;
              error = ft_stroke_border_lineto( border, &delta, FALSE );
              यदि ( error )
                गच्छ Exit;
              error = ft_stroke_border_lineto( border, &end, FALSE );
              यदि ( error )
                गच्छ Exit;
              error = ft_stroke_border_conicto( border, &ctrl, &start );
              यदि ( error )
                गच्छ Exit;
              /* and then move to the endpoint */
              error = ft_stroke_border_lineto( border, &end, FALSE );
              यदि ( error )
                गच्छ Exit;

              अनुवर्त;
            }

            /* else fall through */
          }

          /* simply add an arc */
          error = ft_stroke_border_conicto( border, &ctrl, &end );
          यदि ( error )
            गच्छ Exit;
        }
      }

      stroker->angle_in = angle_out;

      यदि ( arc == bez_चळत )
        भङ्ग;
      arc -= 2;
    } यावत् ( 1 );

    stroker->center      = *to;
    stroker->ओळ_length = 0;

  Exit:
    प्रत्यागम error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Stroker_CubicTo( FT_Stroker  stroker,
                      FT_Vector*  control1,
                      FT_Vector*  control2,
                      FT_Vector*  to )
  {
    FT_Error    error = FT_Err_Ok;
    FT_Vector   bez_चळत[37];
    FT_Vector*  arc;
    FT_Vector*  limit = bez_चळत + 32;
    FT_Bool     first_arc = TRUE;


    यदि ( !stroker || !control1 || !control2 || !to )
    {
      error = FT_THROW( Invalid_Argument );
      गच्छ Exit;
    }

    /* if all control points are coincident, this is a no-op; */
    /* avoid creating a spurious corner */
    यदि ( FT_IS_SMALL( stroker->center.x - control1->x ) &&
         FT_IS_SMALL( stroker->center.y - control1->y ) &&
         FT_IS_SMALL( control1->x       - control2->x ) &&
         FT_IS_SMALL( control1->y       - control2->y ) &&
         FT_IS_SMALL( control2->x       - to->x       ) &&
         FT_IS_SMALL( control2->y       - to->y       ) )
    {
       stroker->center = *to;
       गच्छ Exit;
    }

    arc    = bez_चळत;
    arc[0] = *to;
    arc[1] = *control2;
    arc[2] = *control1;
    arc[3] = stroker->center;

    कुरु
    {
      FT_Angle  angle_in, angle_mid, angle_out;


      /* initialize with current direction */
      angle_in = angle_out = angle_mid = stroker->angle_in;

      यदि ( arc < limit                                         &&
           !ft_cubic_is_small_enough( arc, &angle_in,
                                      &angle_mid, &angle_out ) )
      {
        यदि ( stroker->first_point )
          stroker->angle_in = angle_in;

        ft_cubic_split( arc );
        arc += 3;
        अनुवर्त;
      }

      यदि ( first_arc )
      {
        first_arc = FALSE;

        /* process corner if necessary */
        यदि ( stroker->first_point )
          error = ft_stroker_subpath_start( stroker, angle_in, 0 );
        अन्यथा
        {
          stroker->angle_out = angle_in;
          error = ft_stroker_process_corner( stroker, 0 );
        }
      }
      अन्यथा यदि ( ft_pos_abs( FT_Angle_Diff( stroker->angle_in, angle_in ) ) >
                  FT_SMALL_CUBIC_THRESHOLD / 4                             )
      {
        /* if the deviation from one arc to the next is too great, */
        /* add a round corner                                      */
        stroker->center    = arc[3];
        stroker->angle_out = angle_in;
        stroker->ओळ_join = FT_STROKER_LINEJOIN_ROUND;

        error = ft_stroker_process_corner( stroker, 0 );

        /* reinstate line join style */
        stroker->ओळ_join = stroker->ओळ_join_saved;
      }

      यदि ( error )
        गच्छ Exit;

      /* the arc's angle is small enough; we can add it directly to each */
      /* border                                                          */
      {
        FT_Vector        ctrl1, ctrl2, end;
        FT_Angle         theta1, phi1, theta2, phi2, rotate, alpha0 = 0;
        FT_Fixed         length1, length2;
        FT_StrokeBorder  border;
        FT_Int           side;


        theta1  = FT_Angle_Diff( angle_in,  angle_mid ) / 2;
        theta2  = FT_Angle_Diff( angle_mid, angle_out ) / 2;
        phi1    = ft_angle_mean( angle_in,  angle_mid );
        phi2    = ft_angle_mean( angle_mid, angle_out );
        length1 = FT_DivFix( stroker->radius, FT_Cos( theta1 ) );
        length2 = FT_DivFix( stroker->radius, FT_Cos( theta2 ) );

        /* compute direction of original arc */
        यदि ( stroker->handle_wide_strokes )
          alpha0 = FT_Atan2( arc[0].x - arc[3].x, arc[0].y - arc[3].y );

        प्रति ( border = stroker->borders, side = 0;
              side <= 1;
              side++, border++ )
        {
          rotate = FT_SIDE_TO_ROTATE( side );

          /* compute control points */
          FT_Vector_From_Polar( &ctrl1, length1, phi1 + rotate );
          ctrl1.x += arc[2].x;
          ctrl1.y += arc[2].y;

          FT_Vector_From_Polar( &ctrl2, length2, phi2 + rotate );
          ctrl2.x += arc[1].x;
          ctrl2.y += arc[1].y;

          /* compute end point */
          FT_Vector_From_Polar( &end, stroker->radius, angle_out + rotate );
          end.x += arc[0].x;
          end.y += arc[0].y;

          यदि ( stroker->handle_wide_strokes )
          {
            FT_Vector  start;
            FT_Angle   alpha1;


            /* determine whether the border radius is greater than the */
            /* radius of curvature of the original arc                 */
            start = border->points[border->अंक_points - 1];

            alpha1 = FT_Atan2( end.x - start.x, end.y - start.y );

            /* is the direction of the border arc opposite to */
            /* that of the original arc? */
            यदि ( ft_pos_abs( FT_Angle_Diff( alpha0, alpha1 ) ) >
                   FT_ANGLE_PI / 2                             )
            {
              FT_Angle   beta, gamma;
              FT_Vector  bvec, delta;
              FT_Fixed   blen, sinA, sinB, alen;


              /* use the sine rule to find the intersection point */
              beta  = FT_Atan2( arc[3].x - start.x, arc[3].y - start.y );
              gamma = FT_Atan2( arc[0].x - end.x,   arc[0].y - end.y );

              bvec.x = end.x - start.x;
              bvec.y = end.y - start.y;

              blen = FT_Vector_Length( &bvec );

              sinA = ft_pos_abs( FT_Sin( alpha1 - gamma ) );
              sinB = ft_pos_abs( FT_Sin( beta - gamma ) );

              alen = FT_MulDiv( blen, sinA, sinB );

              FT_Vector_From_Polar( &delta, alen, beta );
              delta.x += start.x;
              delta.y += start.y;

              /* circumnavigate the negative sector backwards */
              border->movable = FALSE;
              error = ft_stroke_border_lineto( border, &delta, FALSE );
              यदि ( error )
                गच्छ Exit;
              error = ft_stroke_border_lineto( border, &end, FALSE );
              यदि ( error )
                गच्छ Exit;
              error = ft_stroke_border_cubicto( border,
                                                &ctrl2,
                                                &ctrl1,
                                                &start );
              यदि ( error )
                गच्छ Exit;
              /* and then move to the endpoint */
              error = ft_stroke_border_lineto( border, &end, FALSE );
              यदि ( error )
                गच्छ Exit;

              अनुवर्त;
            }

            /* else fall through */
          }

          /* simply add an arc */
          error = ft_stroke_border_cubicto( border, &ctrl1, &ctrl2, &end );
          यदि ( error )
            गच्छ Exit;
        }
      }

      stroker->angle_in = angle_out;

      यदि ( arc == bez_चळत )
        भङ्ग;
      arc -= 3;
    } यावत् ( 1 );

    stroker->center      = *to;
    stroker->ओळ_length = 0;

  Exit:
    प्रत्यागम error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Stroker_BeginSubPath( FT_Stroker  stroker,
                           FT_Vector*  to,
                           FT_Bool     open )
  {
    यदि ( !stroker || !to )
      प्रत्यागम FT_THROW( Invalid_Argument );

    /* We cannot process the first point, because there is not enough      */
    /* information regarding its corner/cap.  The latter will be processed */
    /* in the `FT_Stroker_EndSubPath' routine.                             */
    /*                                                                     */
    stroker->first_point  = TRUE;
    stroker->center       = *to;
    stroker->subpath_open = open;

    /* Determine if we need to check whether the border radius is greater */
    /* than the radius of curvature of a curve, to handle this case       */
    /* specially.  This is only required if bevel joins or butt caps may  */
    /* be created, because round & miter joins and round & square caps    */
    /* cover the negative sector created with wide strokes.               */
    stroker->handle_wide_strokes =
      FT_BOOL( stroker->ओळ_join != FT_STROKER_LINEJOIN_ROUND  ||
               ( stroker->subpath_open                        &&
                 stroker->ओळ_cap == FT_STROKER_LINECAP_BUTT ) );

    /* record the subpath start point for each border */
    stroker->subpath_start = *to;

    stroker->angle_in = 0;

    प्रत्यागम FT_Err_Ok;
  }


  स्थिर FT_Error
  ft_stroker_add_reverse_डावा( FT_Stroker  stroker,
                               FT_Bool     open )
  {
    FT_StrokeBorder  उजवा = stroker->borders + 0;
    FT_StrokeBorder  डावा  = stroker->borders + 1;
    FT_Int           new_points;
    FT_Error         error = FT_Err_Ok;


    FT_ASSERT( डावा->start >= 0 );

    new_points = (FT_Int)डावा->अंक_points - डावा->start;
    यदि ( new_points > 0 )
    {
      error = ft_stroke_border_grow( उजवा, (FT_UInt)new_points );
      यदि ( error )
        गच्छ Exit;

      {
        FT_Vector*  dst_point = उजवा->points + उजवा->अंक_points;
        FT_Byte*    dst_tag   = उजवा->tags   + उजवा->अंक_points;
        FT_Vector*  src_point = डावा->points  + डावा->अंक_points - 1;
        FT_Byte*    src_tag   = डावा->tags    + डावा->अंक_points - 1;


        यावत् ( src_point >= डावा->points + डावा->start )
        {
          *dst_point = *src_point;
          *dst_tag   = *src_tag;

          यदि ( open )
            dst_tag[0] &= ~FT_STROKE_TAG_BEGIN_END;
          अन्यथा
          {
            FT_Byte  ttag =
                       (FT_Byte)( dst_tag[0] & FT_STROKE_TAG_BEGIN_END );


            /* switch begin/end tags if necessary */
            यदि ( ttag == FT_STROKE_TAG_BEGIN ||
                 ttag == FT_STROKE_TAG_END   )
              dst_tag[0] ^= FT_STROKE_TAG_BEGIN_END;
          }

          src_point--;
          src_tag--;
          dst_point++;
          dst_tag++;
        }
      }

      डावा->अंक_points   = (FT_UInt)डावा->start;
      उजवा->अंक_points += (FT_UInt)new_points;

      उजवा->movable = FALSE;
      डावा->movable  = FALSE;
    }

  Exit:
    प्रत्यागम error;
  }


  /* documentation is in ftstroke.h */

  /* there's a lot of magic in this function! */
  FT_EXPORT_DEF( FT_Error )
  FT_Stroker_EndSubPath( FT_Stroker  stroker )
  {
    FT_Error  error = FT_Err_Ok;


    यदि ( !stroker )
    {
      error = FT_THROW( Invalid_Argument );
      गच्छ Exit;
    }

    /* don't try to end the path if no segments have been generated */
    यदि ( stroker->first_point )
      गच्छ Exit;

    यदि ( stroker->subpath_open )
    {
      FT_StrokeBorder  उजवा = stroker->borders;


      /* All right, this is an opened path, we need to add a cap between */
      /* right & left, add the reverse of left, then add a final cap     */
      /* between left & right.                                           */
      error = ft_stroker_cap( stroker, stroker->angle_in, 0 );
      यदि ( error )
        गच्छ Exit;

      /* add reversed points from `left' to `right' */
      error = ft_stroker_add_reverse_डावा( stroker, TRUE );
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
      ft_stroke_border_close( उजवा, FALSE );
    }
    अन्यथा
    {
      /* close the path if needed */
      यदि ( !FT_IS_SMALL( stroker->center.x - stroker->subpath_start.x ) ||
           !FT_IS_SMALL( stroker->center.y - stroker->subpath_start.y ) )
      {
         error = FT_Stroker_ओळ_To( stroker, &stroker->subpath_start );
         यदि ( error )
           गच्छ Exit;
      }

      /* process the corner */
      stroker->angle_out = stroker->subpath_angle;

      error = ft_stroker_process_corner( stroker,
                                         stroker->subpath_ओळ_length );
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
    FT_UInt   अंक_points = 0, अंक_contours = 0;
    FT_Error  error;


    यदि ( !stroker || border > 1 )
    {
      error = FT_THROW( Invalid_Argument );
      गच्छ Exit;
    }

    error = ft_stroke_border_get_counts( stroker->borders + border,
                                         &अंक_points, &अंक_contours );
  Exit:
    यदि ( anum_points )
      *anum_points = अंक_points;

    यदि ( anum_contours )
      *anum_contours = अंक_contours;

    प्रत्यागम error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Stroker_GetCounts( FT_Stroker  stroker,
                        FT_UInt    *anum_points,
                        FT_UInt    *anum_contours )
  {
    FT_UInt   count1, count2, अंक_points   = 0;
    FT_UInt   count3, count4, अंक_contours = 0;
    FT_Error  error;


    यदि ( !stroker )
    {
      error = FT_THROW( Invalid_Argument );
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

    अंक_points   = count1 + count3;
    अंक_contours = count2 + count4;

  Exit:
    यदि ( anum_points )
      *anum_points   = अंक_points;

    यदि ( anum_contours )
      *anum_contours = अंक_contours;

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

    यदि ( border == FT_STROKER_BORDER_डावा  ||
         border == FT_STROKER_BORDER_उजवा )
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
    FT_Stroker_ExportBorder( stroker, FT_STROKER_BORDER_डावा, outline );
    FT_Stroker_ExportBorder( stroker, FT_STROKER_BORDER_उजवा, outline );
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
    FT_Vector   v_last;
    FT_Vector   v_control;
    FT_Vector   v_start;

    FT_Vector*  point;
    FT_Vector*  limit;
    FT_Byte*    tags;

    FT_Error    error;

    FT_Int      n;         /* index of contour in outline     */
    FT_Int      first;     /* index of first point in contour */
    FT_Int      last;      /* index of last point in contour  */

    FT_Int      tag;       /* current point's state           */


    यदि ( !outline )
      प्रत्यागम FT_THROW( Invalid_Outline );

    यदि ( !stroker )
      प्रत्यागम FT_THROW( Invalid_Argument );

    FT_Stroker_Rewind( stroker );

    last = -1;
    प्रति ( n = 0; n < outline->n_contours; n++ )
    {
      first = last + 1;
      last  = outline->contours[n];

      /* skip empty points; we don't stroke these */
      यदि ( last <= first )
        अनुवर्त;

      limit = outline->points + last;

      v_start = outline->points[first];
      v_last  = outline->points[last];

      v_control = v_start;

      point = outline->points + first;
      tags  = outline->tags   + first;
      tag   = FT_CURVE_TAG( tags[0] );

      /* check first point to determine origin */
      यदि ( tag != FT_CURVE_TAG_ON )
      {
        FT_Int  last_tag = FT_CURVE_TAG( outline->tags[last] );


        /* First point is a control point.  Yes, this happens. */
        यदि ( last_tag == FT_CURVE_TAG_ON )
        {
          /* start at last point if it is on the curve */
          v_start = v_last;
          limit--;
        }
        अन्यथा
        {
          यदि ( last_tag != tag )
            गच्छ Invalid_Outline;

          /* if both first and last points are controls, */
          /* start at their middle                       */
          v_start.x = ( v_start.x + v_last.x ) / 2;
          v_start.y = ( v_start.y + v_last.y ) / 2;
        }
        point--;
        tags--;
      }

      error = FT_Stroker_BeginSubPath( stroker, &v_start, opened );
      यदि ( error )
        गच्छ Exit;

      यावत् ( point < limit )
      {
        point++;
        tags++;

        tag = FT_CURVE_TAG( tags[0] );
        विकल्प ( tag )
        {
        पक्ष FT_CURVE_TAG_ON:  /* emit a single line_to */
          {
            FT_Vector  vec;


            vec.x = point->x;
            vec.y = point->y;

            error = FT_Stroker_ओळ_To( stroker, &vec );
            यदि ( error )
              गच्छ Exit;
            अनुवर्त;
          }

        पक्ष FT_CURVE_TAG_CONIC:  /* consume conic arcs */
          v_control.x = point->x;
          v_control.y = point->y;

        Do_Conic:
          यदि ( point < limit )
          {
            FT_Vector  vec;
            FT_Vector  v_मधला;


            point++;
            tags++;
            tag = FT_CURVE_TAG( tags[0] );

            vec = point[0];

            यदि ( tag == FT_CURVE_TAG_ON )
            {
              error = FT_Stroker_ConicTo( stroker, &v_control, &vec );
              यदि ( error )
                गच्छ Exit;
              अनुवर्त;
            }

            यदि ( tag != FT_CURVE_TAG_CONIC )
              गच्छ Invalid_Outline;

            v_मधला.x = ( v_control.x + vec.x ) / 2;
            v_मधला.y = ( v_control.y + vec.y ) / 2;

            error = FT_Stroker_ConicTo( stroker, &v_control, &v_मधला );
            यदि ( error )
              गच्छ Exit;

            v_control = vec;
            गच्छ Do_Conic;
          }

          error = FT_Stroker_ConicTo( stroker, &v_control, &v_start );
          गच्छ Close;

        स्वभाव:  /* FT_CURVE_TAG_CUBIC */
          {
            FT_Vector  vec1, vec2;


            यदि ( point + 1 > limit                             ||
                 FT_CURVE_TAG( tags[1] ) != FT_CURVE_TAG_CUBIC )
              गच्छ Invalid_Outline;

            point += 2;
            tags  += 2;

            vec1 = point[-2];
            vec2 = point[-1];

            यदि ( point <= limit )
            {
              FT_Vector  vec;
              FT_Vector  v_मधला;


              tag = FT_CURVE_TAG( tags[0] );
              vec = point[0];

              यदि ( tag == FT_CURVE_TAG_CUBIC )
              {
                v_मधला.x = ( vec2.x + vec.x ) / 2;
                v_मधला.y = ( vec2.y + vec.y ) / 2;

                vec = v_मधला;
              }
              अन्यथा यदि ( tag != FT_CURVE_TAG_ON )
                गच्छ Invalid_Outline;

              error = FT_Stroker_CubicTo( stroker, &vec1, &vec2, &vec );
              यदि ( error )
                गच्छ Exit;

              यदि ( tag == FT_CURVE_TAG_CUBIC )
              {
                point--;
                tags--;
              }
              अनुवर्त;
            }

            error = FT_Stroker_CubicTo( stroker, &vec1, &vec2, &v_start );
            गच्छ Close;
          }
        }
      }

    Close:
      यदि ( error )
        गच्छ Exit;

      error = FT_Stroker_EndSubPath( stroker );
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
    FT_Error  error = FT_THROW( Invalid_Argument );
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
      FT_UInt          अंक_points, अंक_contours;


      error = FT_Stroker_ParseOutline( stroker, outline, FALSE );
      यदि ( error )
        गच्छ Fail;

      error = FT_Stroker_GetCounts( stroker, &अंक_points, &अंक_contours );
      यदि ( error )
        गच्छ Fail;

      FT_Outline_Done( glyph->library, outline );

      error = FT_Outline_New( glyph->library,
                              अंक_points,
                              (FT_Int)अंक_contours,
                              outline );
      यदि ( error )
        गच्छ Fail;

      outline->n_points   = 0;
      outline->n_contours = 0;

      FT_Stroker_Export( stroker, outline );
    }

    यदि ( destroy )
      FT_Done_Glyph( *pglyph );

    *pglyph = glyph;
    गच्छ Exit;

  Fail:
    FT_Done_Glyph( glyph );
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
    FT_Error  error = FT_THROW( Invalid_Argument );
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
      FT_UInt           अंक_points, अंक_contours;


      border = FT_Outline_GetOutsideBorder( outline );
      यदि ( inside )
      {
        यदि ( border == FT_STROKER_BORDER_डावा )
          border = FT_STROKER_BORDER_उजवा;
        अन्यथा
          border = FT_STROKER_BORDER_डावा;
      }

      error = FT_Stroker_ParseOutline( stroker, outline, FALSE );
      यदि ( error )
        गच्छ Fail;

      FT_Stroker_GetBorderCounts( stroker, border,
                                  &अंक_points, &अंक_contours );

      FT_Outline_Done( glyph->library, outline );

      error = FT_Outline_New( glyph->library,
                              अंक_points,
                              (FT_Int)अंक_contours,
                              outline );
      यदि ( error )
        गच्छ Fail;

      outline->n_points   = 0;
      outline->n_contours = 0;

      FT_Stroker_ExportBorder( stroker, border, outline );
    }

    यदि ( destroy )
      FT_Done_Glyph( *pglyph );

    *pglyph = glyph;
    गच्छ Exit;

  Fail:
    FT_Done_Glyph( glyph );
    glyph = रिक्त;

    यदि ( !destroy )
      *pglyph = रिक्त;

  Exit:
    प्रत्यागम error;
  }


/* END */
