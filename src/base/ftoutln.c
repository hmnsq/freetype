/****************************************************************************
 *
 * ftoutln.c
 *
 *   FreeType outline management (body).
 *
 * Copyright (C) 1996-2026 by
 * David Turner, Robert Wilhelm, and Werner Lemberg.
 *
 * This file is part of the FreeType project, and may only be used,
 * modified, and distributed under the terms of the FreeType project
 * license, LICENSE.TXT.  By continuing to use, modify, or distribute
 * this file you indicate that you have read the license and
 * understand and accept it fully.
 *
 */


#समावेश <freetype/ftoutln.h>
#समावेश <freetype/internal/ftobjs.h>
#समावेश <freetype/internal/ftcalc.h>
#समावेश <freetype/internal/ftdebug.h>
#समावेश <freetype/fttrigon.h>


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#अघोषित  FT_COMPONENT
#घोषणा FT_COMPONENT  outline


  अटल
  स्थिर FT_Outline  null_outline = { 0, 0, शून्य, शून्य, शून्य, 0 };


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Outline_Decompose( FT_Outline*              outline,
                        स्थिर FT_Outline_Funcs*  func_interface,
                        व्योम*                    user )
  {
#अघोषित  SCALED
#घोषणा SCALED( x )  ( (x) * ( 1L << shift ) - delta )

    FT_Vector   v_last;
    FT_Vector   v_control;
    FT_Vector   v_start;

    FT_Vector*  point;
    FT_Vector*  limit;
    FT_Byte*    tags;

    FT_Error    error;

    FT_Int   n;         /* index of contour in outline     */
    FT_Int   first;     /* index of first point in contour */
    FT_Int   last;      /* index of last point in contour  */

    FT_Int   tag;       /* current point's state           */

    FT_Int   shift;
    FT_Pos   delta;


    अगर ( !outline )
      वापस FT_THROW( Invalid_Outline );

    अगर ( !func_interface )
      वापस FT_THROW( Invalid_Argument );

    shift = func_interface->shift;
    delta = func_interface->delta;

    last = -1;
    क्रम ( n = 0; n < outline->n_contours; n++ )
    {
      FT_TRACE5(( "FT_Outline_Decompose: Contour %d\n", n ));

      first = last + 1;
      last  = outline->contours[n];
      अगर ( last < first )
        जाओ Invalid_Outline;

      limit = outline->points + last;

      v_start   = outline->points[first];
      v_start.x = SCALED( v_start.x );
      v_start.y = SCALED( v_start.y );

      v_last   = outline->points[last];
      v_last.x = SCALED( v_last.x );
      v_last.y = SCALED( v_last.y );

      v_control = v_start;

      point = outline->points + first;
      tags  = outline->tags   + first;
      tag   = FT_CURVE_TAG( tags[0] );

      /* check first point to determine origin */
      अगर ( tag != FT_CURVE_TAG_ON )
      {
        FT_Int  last_tag = FT_CURVE_TAG( outline->tags[last] );


        /* first point is a control point.  Yes, this happens. */
        अगर ( last_tag == FT_CURVE_TAG_ON )
        {
          /* start at last point if it is on the curve */
          v_start = v_last;
          limit--;
        }
        अन्यथा
        {
          अगर ( last_tag != tag )
            जाओ Invalid_Outline;

          /* if both first and last points are controls,   */
          /* start at their middle and record its position */
          /* for closure                                   */
          v_start.x = ( v_start.x + v_last.x ) / 2;
          v_start.y = ( v_start.y + v_last.y ) / 2;

          /* v_last = v_start; */
        }
        point--;
        tags--;
      }

      FT_TRACE5(( "  move to (%.2f, %.2f)\n",
                  (द्विगुन)v_start.x / 64, (द्विगुन)v_start.y / 64 ));
      error = func_interface->move_to( &v_start, user );
      अगर ( error )
        जाओ Exit;

      जबतक ( point < limit )
      {
        point++;
        tags++;

        tag = FT_CURVE_TAG( tags[0] );
        चयन ( tag )
        {
        हाल FT_CURVE_TAG_ON:  /* emit a single line_to */
          {
            FT_Vector  vec;


            vec.x = SCALED( point->x );
            vec.y = SCALED( point->y );

            FT_TRACE5(( "  line to (%.2f, %.2f)\n",
                        (द्विगुन)vec.x / 64, (द्विगुन)vec.y / 64 ));
            error = func_interface->पंक्ति_to( &vec, user );
            अगर ( error )
              जाओ Exit;
            जारी;
          }

        हाल FT_CURVE_TAG_CONIC:  /* consume conic arcs */
          v_control.x = SCALED( point->x );
          v_control.y = SCALED( point->y );

        Do_Conic:
          अगर ( point < limit )
          {
            FT_Vector  vec;
            FT_Vector  v_मध्य;


            point++;
            tags++;
            tag = FT_CURVE_TAG( tags[0] );

            vec.x = SCALED( point->x );
            vec.y = SCALED( point->y );

            अगर ( tag == FT_CURVE_TAG_ON )
            {
              FT_TRACE5(( "  conic to (%.2f, %.2f)"
                          " with control (%.2f, %.2f)\n",
                          (द्विगुन)vec.x / 64,
                          (द्विगुन)vec.y / 64,
                          (द्विगुन)v_control.x / 64,
                          (द्विगुन)v_control.y / 64 ));
              error = func_interface->conic_to( &v_control, &vec, user );
              अगर ( error )
                जाओ Exit;
              जारी;
            }

            अगर ( tag != FT_CURVE_TAG_CONIC )
              जाओ Invalid_Outline;

            v_मध्य.x = ( v_control.x + vec.x ) / 2;
            v_मध्य.y = ( v_control.y + vec.y ) / 2;

            FT_TRACE5(( "  conic to (%.2f, %.2f)"
                        " with control (%.2f, %.2f)\n",
                        (द्विगुन)v_मध्य.x / 64,
                        (द्विगुन)v_मध्य.y / 64,
                        (द्विगुन)v_control.x / 64,
                        (द्विगुन)v_control.y / 64 ));
            error = func_interface->conic_to( &v_control, &v_मध्य, user );
            अगर ( error )
              जाओ Exit;

            v_control = vec;
            जाओ Do_Conic;
          }

          FT_TRACE5(( "  conic to (%.2f, %.2f)"
                      " with control (%.2f, %.2f)\n",
                      (द्विगुन)v_start.x / 64,
                      (द्विगुन)v_start.y / 64,
                      (द्विगुन)v_control.x / 64,
                      (द्विगुन)v_control.y / 64 ));
          error = func_interface->conic_to( &v_control, &v_start, user );
          जाओ Close;

        शेष:  /* FT_CURVE_TAG_CUBIC */
          {
            FT_Vector  vec1, vec2;


            अगर ( point + 1 > limit                             ||
                 FT_CURVE_TAG( tags[1] ) != FT_CURVE_TAG_CUBIC )
              जाओ Invalid_Outline;

            point += 2;
            tags  += 2;

            vec1.x = SCALED( point[-2].x );
            vec1.y = SCALED( point[-2].y );

            vec2.x = SCALED( point[-1].x );
            vec2.y = SCALED( point[-1].y );

            अगर ( point <= limit )
            {
              FT_Vector  vec;
              FT_Vector  v_मध्य;


              tag = FT_CURVE_TAG( tags[0] );

              vec.x = SCALED( point->x );
              vec.y = SCALED( point->y );

              अगर ( tag == FT_CURVE_TAG_CUBIC )
              {
                v_मध्य.x = ( vec2.x + vec.x ) / 2;
                v_मध्य.y = ( vec2.y + vec.y ) / 2;

                vec = v_मध्य;
              }
              अन्यथा अगर ( tag != FT_CURVE_TAG_ON )
                जाओ Invalid_Outline;

              FT_TRACE5(( "  cubic to (%.2f, %.2f)"
                          " with controls (%.2f, %.2f) and (%.2f, %.2f)\n",
                          (द्विगुन)vec.x / 64,
                          (द्विगुन)vec.y / 64,
                          (द्विगुन)vec1.x / 64,
                          (द्विगुन)vec1.y / 64,
                          (द्विगुन)vec2.x / 64,
                          (द्विगुन)vec2.y / 64 ));
              error = func_interface->cubic_to( &vec1, &vec2, &vec, user );
              अगर ( error )
                जाओ Exit;

              अगर ( tag == FT_CURVE_TAG_CUBIC )
              {
                point--;
                tags--;
              }
              जारी;
            }

            FT_TRACE5(( "  cubic to (%.2f, %.2f)"
                        " with controls (%.2f, %.2f) and (%.2f, %.2f)\n",
                        (द्विगुन)v_start.x / 64,
                        (द्विगुन)v_start.y / 64,
                        (द्विगुन)vec1.x / 64,
                        (द्विगुन)vec1.y / 64,
                        (द्विगुन)vec2.x / 64,
                        (द्विगुन)vec2.y / 64 ));
            error = func_interface->cubic_to( &vec1, &vec2, &v_start, user );
            जाओ Close;
          }
        }
      }

      /* close the contour with a line segment */
      FT_TRACE5(( "  line to (%.2f, %.2f)\n",
                  (द्विगुन)v_start.x / 64, (द्विगुन)v_start.y / 64 ));
      error = func_interface->पंक्ति_to( &v_start, user );

    Close:
      अगर ( error )
        जाओ Exit;
    }

    FT_TRACE5(( "FT_Outline_Decompose: Done\n" ));
    वापस FT_Err_Ok;

  Invalid_Outline:
    error = FT_THROW( Invalid_Outline );
    /* fall through */

  Exit:
    FT_TRACE5(( "FT_Outline_Decompose: Error 0x%x\n", error ));
    वापस error;
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Outline_New( FT_Library   library,
                  FT_UInt      अंक_Points,
                  FT_Int       अंक_Contours,
                  FT_Outline  *anoutline )
  {
    FT_Error   error;
    FT_Memory  memory;


    अगर ( !library )
      वापस FT_THROW( Invalid_Library_Handle );

    memory = library->memory;

    अगर ( !anoutline || !memory )
      वापस FT_THROW( Invalid_Argument );

    *anoutline = null_outline;

    अगर ( अंक_Contours < 0                  ||
         (FT_UInt)अंक_Contours > अंक_Points )
      वापस FT_THROW( Invalid_Argument );

    अगर ( अंक_Points > FT_OUTLINE_POINTS_MAX )
      वापस FT_THROW( Array_Too_Large );

    अगर ( FT_NEW_ARRAY( anoutline->points,   अंक_Points   ) ||
         FT_NEW_ARRAY( anoutline->tags,     अंक_Points   ) ||
         FT_NEW_ARRAY( anoutline->contours, अंक_Contours ) )
      जाओ Fail;

    anoutline->n_points    = (FT_UShort)अंक_Points;
    anoutline->n_contours  = (FT_UShort)अंक_Contours;
    anoutline->flags      |= FT_OUTLINE_OWNER;

    वापस FT_Err_Ok;

  Fail:
    anoutline->flags |= FT_OUTLINE_OWNER;
    FT_Outline_Done( library, anoutline );

    वापस error;
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Outline_Check( FT_Outline*  outline )
  {
    अगर ( outline )
    {
      FT_Int  n_points   = outline->n_points;
      FT_Int  n_contours = outline->n_contours;
      FT_Int  end0, end;
      FT_Int  n;


      FT_TRACE5(( "FT_Outline_Check: contours = %d, points = %d\n",
                  n_contours, n_points ));
      /* empty glyph? */
      अगर ( n_points == 0 && n_contours == 0 )
        वापस FT_Err_Ok;

      /* check point and contour counts */
      अगर ( n_points == 0 || n_contours == 0 )
        जाओ Bad;

      end0 = -1;
      क्रम ( n = 0; n < n_contours; n++ )
      {
        end = outline->contours[n];

        /* note that we don't accept empty contours */
        अगर ( end <= end0 || end >= n_points )
          जाओ Bad;

        end0 = end;
      }

      अगर ( end0 != n_points - 1 )
        जाओ Bad;

      /* XXX: check the tags array */
      वापस FT_Err_Ok;
    }

  Bad:
    वापस FT_THROW( Invalid_Outline );
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Outline_Copy( स्थिर FT_Outline*  source,
                   FT_Outline        *target )
  {
    FT_Int  is_owner;


    अगर ( !source || !target )
      वापस FT_THROW( Invalid_Outline );

    अगर ( source->n_points   != target->n_points   ||
         source->n_contours != target->n_contours )
      वापस FT_THROW( Invalid_Argument );

    अगर ( source == target )
      वापस FT_Err_Ok;

    अगर ( source->n_points )
    {
      FT_ARRAY_COPY( target->points, source->points, source->n_points );
      FT_ARRAY_COPY( target->tags,   source->tags,   source->n_points );
    }

    अगर ( source->n_contours )
      FT_ARRAY_COPY( target->contours, source->contours, source->n_contours );

    /* copy all flags, except the `FT_OUTLINE_OWNER' one */
    is_owner      = target->flags & FT_OUTLINE_OWNER;
    target->flags = source->flags;

    target->flags &= ~FT_OUTLINE_OWNER;
    target->flags |= is_owner;

    वापस FT_Err_Ok;
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Outline_Done( FT_Library   library,
                   FT_Outline*  outline )
  {
    FT_Memory  memory;


    अगर ( !library )
      वापस FT_THROW( Invalid_Library_Handle );

    अगर ( !outline )
      वापस FT_THROW( Invalid_Outline );

    memory = library->memory;

    अगर ( !memory )
      वापस FT_THROW( Invalid_Argument );

    अगर ( outline->flags & FT_OUTLINE_OWNER )
    {
      FT_FREE( outline->points   );
      FT_FREE( outline->tags     );
      FT_FREE( outline->contours );
    }
    *outline = null_outline;

    वापस FT_Err_Ok;
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( व्योम )
  FT_Outline_Get_CBox( स्थिर FT_Outline*  outline,
                       FT_BBox           *acbox )
  {
    FT_Pos  xMin, yMin, xMax, yMax;


    अगर ( outline && acbox )
    {
      अगर ( outline->n_points == 0 )
      {
        xMin = 0;
        yMin = 0;
        xMax = 0;
        yMax = 0;
      }
      अन्यथा
      {
        FT_Vector*  vec   = outline->points;
        FT_Vector*  limit = vec + outline->n_points;


        xMin = xMax = vec->x;
        yMin = yMax = vec->y;
        vec++;

        क्रम ( ; vec < limit; vec++ )
        {
          FT_Pos  x, y;


          x = vec->x;
          अगर ( x < xMin ) xMin = x;
          अगर ( x > xMax ) xMax = x;

          y = vec->y;
          अगर ( y < yMin ) yMin = y;
          अगर ( y > yMax ) yMax = y;
        }
      }
      acbox->xMin = xMin;
      acbox->xMax = xMax;
      acbox->yMin = yMin;
      acbox->yMax = yMax;
    }
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( व्योम )
  FT_Outline_Translate( स्थिर FT_Outline*  outline,
                        FT_Pos             xOffset,
                        FT_Pos             yOffset )
  {
    FT_UShort   n;
    FT_Vector*  vec;


    अगर ( !outline )
      वापस;

    vec = outline->points;

    क्रम ( n = 0; n < outline->n_points; n++ )
    {
      vec->x = ADD_LONG( vec->x, xOffset );
      vec->y = ADD_LONG( vec->y, yOffset );
      vec++;
    }
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( व्योम )
  FT_Outline_Reverse( FT_Outline*  outline )
  {
    FT_UShort  n;
    FT_Int     first, last;


    अगर ( !outline )
      वापस;

    last = -1;
    क्रम ( n = 0; n < outline->n_contours; n++ )
    {
      first = last + 1;
      last  = outline->contours[n];

      /* Keep the first point unless it is a cubic control.  In that */
      /* case, reverse the entire contour to preserve control pairs, */
      /* including contours with no explicit on-curve points.        */
      अगर ( FT_CURVE_TAG( outline->tags[first] ) != FT_CURVE_TAG_CUBIC )
        first++;

      /* reverse point table */
      {
        FT_Vector*  p = outline->points + first;
        FT_Vector*  q = outline->points + last;
        FT_Vector   swap;


        जबतक ( p < q )
        {
          swap = *p;
          *p   = *q;
          *q   = swap;
          p++;
          q--;
        }
      }

      /* reverse tags table */
      {
        FT_Byte*  p = outline->tags + first;
        FT_Byte*  q = outline->tags + last;


        जबतक ( p < q )
        {
          FT_Byte  swap;


          swap = *p;
          *p   = *q;
          *q   = swap;
          p++;
          q--;
        }
      }
    }

    outline->flags ^= FT_OUTLINE_REVERSE_FILL;
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Outline_Render( FT_Library         library,
                     FT_Outline*        outline,
                     FT_Raster_Params*  params )
  {
    FT_Error     error;
    FT_Renderer  renderer;
    FT_List_गाँठ  गाँठ;
    FT_BBox      cbox;


    अगर ( !library )
      वापस FT_THROW( Invalid_Library_Handle );

    अगर ( !outline )
      वापस FT_THROW( Invalid_Outline );

    अगर ( !params )
      वापस FT_THROW( Invalid_Argument );

    FT_Outline_Get_CBox( outline, &cbox );
    अगर ( cbox.xMin < -0x1000000L || cbox.yMin < -0x1000000L ||
         cbox.xMax >  0x1000000L || cbox.yMax >  0x1000000L )
      वापस FT_THROW( Invalid_Outline );

    renderer = library->cur_renderer;
    गाँठ     = library->renderers.शीर्ष_2;

    params->source = (व्योम*)outline;

    /* preset clip_box for direct mode */
    अगर ( params->flags & FT_RASTER_FLAG_DIRECT    &&
         !( params->flags & FT_RASTER_FLAG_CLIP ) )
    {
      params->clip_box.xMin = cbox.xMin >> 6;
      params->clip_box.yMin = cbox.yMin >> 6;
      params->clip_box.xMax = ( cbox.xMax + 63 ) >> 6;
      params->clip_box.yMax = ( cbox.yMax + 63 ) >> 6;
    }

    error = FT_ERR( Cannot_Render_Glyph );
    जबतक ( renderer )
    {
      error = renderer->raster_render( renderer->raster, params );
      अगर ( !error || FT_ERR_NEQ( error, Cannot_Render_Glyph ) )
        अवरोध;

      /* FT_Err_Cannot_Render_Glyph is returned if the render mode   */
      /* is unsupported by the current renderer for this glyph image */
      /* format                                                      */

      /* now, look for another renderer that supports the same */
      /* format                                                */
      renderer = FT_Lookup_Renderer( library, FT_GLYPH_FORMAT_OUTLINE,
                                     &गाँठ );
    }

    वापस error;
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Outline_Get_Bitmap( FT_Library        library,
                         FT_Outline*       outline,
                         स्थिर FT_Bitmap  *abitmap )
  {
    FT_Raster_Params  params;


    अगर ( !abitmap )
      वापस FT_THROW( Invalid_Argument );

    /* other checks are delayed to `FT_Outline_Render' */

    params.target = abitmap;
    params.flags  = 0;

    अगर ( abitmap->pixel_mode == FT_PIXEL_MODE_GRAY  ||
         abitmap->pixel_mode == FT_PIXEL_MODE_LCD   ||
         abitmap->pixel_mode == FT_PIXEL_MODE_LCD_V )
      params.flags |= FT_RASTER_FLAG_AA;

    वापस FT_Outline_Render( library, outline, &params );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( व्योम )
  FT_Vector_Transform( FT_Vector*        vector,
                       स्थिर FT_Matrix*  matrix )
  {
    FT_Pos  xz, yz;


    अगर ( !vector || !matrix )
      वापस;

    xz = FT_MulFix( vector->x, matrix->xx ) +
         FT_MulFix( vector->y, matrix->xy );

    yz = FT_MulFix( vector->x, matrix->yx ) +
         FT_MulFix( vector->y, matrix->yy );

    vector->x = xz;
    vector->y = yz;
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( व्योम )
  FT_Outline_Transform( स्थिर FT_Outline*  outline,
                        स्थिर FT_Matrix*   matrix )
  {
    FT_Vector*  vec;
    FT_Vector*  limit;


    अगर ( !outline || !matrix || !outline->points )
      वापस;

    vec   = outline->points;
    limit = vec + outline->n_points;

    क्रम ( ; vec < limit; vec++ )
      FT_Vector_Transform( vec, matrix );
  }


#अगर 0

#घोषणा FT_OUTLINE_GET_CONTOUR( outline, c, first, last )  \
  करो                                                       \
  {                                                        \
    (first) = ( c > 0 ) ? (outline)->points +              \
                            (outline)->contours[c - 1] + 1 \
                        : (outline)->points;               \
    (last) = (outline)->points + (outline)->contours[c];   \
  } जबतक ( 0 )


  /* Is a point in some contour?                     */
  /*                                                 */
  /* We treat every point of the contour as if it    */
  /* it were ON.  That is, we allow false positives, */
  /* but disallow false negatives.  (XXX really?)    */
  अटल FT_Bool
  ft_contour_has( FT_Outline*  outline,
                  FT_Short     c,
                  FT_Vector*   point )
  {
    FT_Vector*  first;
    FT_Vector*  last;
    FT_Vector*  a;
    FT_Vector*  b;
    FT_UInt     n = 0;


    FT_OUTLINE_GET_CONTOUR( outline, c, first, last );

    क्रम ( a = first; a <= last; a++ )
    {
      FT_Pos  x;
      FT_Int  intersect;


      b = ( a == last ) ? first : a + 1;

      intersect = ( a->y - point->y ) ^ ( b->y - point->y );

      /* a and b are on the same side */
      अगर ( intersect >= 0 )
      {
        अगर ( intersect == 0 && a->y == point->y )
        {
          अगर ( ( a->x <= point->x && b->x >= point->x ) ||
               ( a->x >= point->x && b->x <= point->x ) )
            वापस 1;
        }

        जारी;
      }

      x = a->x + ( b->x - a->x ) * (point->y - a->y ) / ( b->y - a->y );

      अगर ( x < point->x )
        n++;
      अन्यथा अगर ( x == point->x )
        वापस 1;
    }

    वापस n & 1;
  }


  अटल FT_Bool
  ft_contour_enclosed( FT_Outline*  outline,
                       FT_UShort    c )
  {
    FT_Vector*  first;
    FT_Vector*  last;
    FT_Short    i;


    FT_OUTLINE_GET_CONTOUR( outline, c, first, last );

    क्रम ( i = 0; i < outline->n_contours; i++ )
    {
      अगर ( i != c && ft_contour_has( outline, i, first ) )
      {
        FT_Vector*  pt;


        क्रम ( pt = first + 1; pt <= last; pt++ )
          अगर ( !ft_contour_has( outline, i, pt ) )
            वापस 0;

        वापस 1;
      }
    }

    वापस 0;
  }


  /* This version differs from the public one in that each */
  /* part (contour not enclosed in another contour) of the */
  /* outline is checked for orientation.  This is          */
  /* necessary for some buggy CJK fonts.                   */
  अटल FT_Orientation
  ft_outline_get_orientation( FT_Outline*  outline )
  {
    FT_Short        i;
    FT_Vector*      first;
    FT_Vector*      last;
    FT_Orientation  orient = FT_ORIENTATION_NONE;


    first = outline->points;
    क्रम ( i = 0; i < outline->n_contours; i++, first = last + 1 )
    {
      FT_Vector*  point;
      FT_Vector*  xmin_point;
      FT_Pos      xmin;


      last = outline->points + outline->contours[i];

      /* skip degenerate contours */
      अगर ( last < first + 2 )
        जारी;

      अगर ( ft_contour_enclosed( outline, i ) )
        जारी;

      xmin       = first->x;
      xmin_point = first;

      क्रम ( point = first + 1; point <= last; point++ )
      {
        अगर ( point->x < xmin )
        {
          xmin       = point->x;
          xmin_point = point;
        }
      }

      /* check the orientation of the contour */
      {
        FT_Vector*      पूर्व;
        FT_Vector*      अग्र;
        FT_Orientation  o;


        पूर्व = ( xmin_point == first ) ? last : xmin_point - 1;
        अग्र = ( xmin_point == last ) ? first : xmin_point + 1;

        अगर ( FT_Atan2( पूर्व->x - xmin_point->x, पूर्व->y - xmin_point->y ) >
             FT_Atan2( अग्र->x - xmin_point->x, अग्र->y - xmin_point->y ) )
          o = FT_ORIENTATION_POSTSCRIPT;
        अन्यथा
          o = FT_ORIENTATION_TRUETYPE;

        अगर ( orient == FT_ORIENTATION_NONE )
          orient = o;
        अन्यथा अगर ( orient != o )
          वापस FT_ORIENTATION_NONE;
      }
    }

    वापस orient;
  }

#पूर्ण_अगर /* 0 */


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Outline_Embolden( FT_Outline*  outline,
                       FT_Pos       strength )
  {
    वापस FT_Outline_EmboldenXY( outline, strength, strength );
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Outline_EmboldenXY( FT_Outline*  outline,
                         FT_Pos       xstrength,
                         FT_Pos       ystrength )
  {
    FT_Vector*      points;
    FT_Int          c, first, last;
    FT_Orientation  orientation;


    अगर ( !outline )
      वापस FT_THROW( Invalid_Outline );

    xstrength /= 2;
    ystrength /= 2;
    अगर ( xstrength == 0 && ystrength == 0 )
      वापस FT_Err_Ok;

    orientation = FT_Outline_Get_Orientation( outline );
    अगर ( orientation == FT_ORIENTATION_NONE )
    {
      अगर ( outline->n_contours )
        वापस FT_THROW( Invalid_Argument );
      अन्यथा
        वापस FT_Err_Ok;
    }

    points = outline->points;

    last = -1;
    क्रम ( c = 0; c < outline->n_contours; c++ )
    {
      FT_Vector  in, out, anchor, shift;
      FT_Fixed   l_in, l_out, l_anchor = 0, l, q, d;
      FT_Int     i, j, k;


      first = last + 1;
      last  = outline->contours[c];
      l_in  = 0;

      /* pacify compiler */
      in.x = in.y = anchor.x = anchor.y = 0;

      /* Counter j cycles though the points; counter i advances only  */
      /* when points are moved; anchor k marks the first moved point. */
      क्रम ( i = last, j = first, k = -1;
            j != i && i != k;
            j = j < last ? j + 1 : first )
      {
        अगर ( j != k )
        {
          out.x = points[j].x - points[i].x;
          out.y = points[j].y - points[i].y;
          l_out = (FT_Fixed)FT_Vector_NormLen( &out );

          अगर ( l_out == 0 )
            जारी;
        }
        अन्यथा
        {
          out   = anchor;
          l_out = l_anchor;
        }

        अगर ( l_in != 0 )
        {
          अगर ( k < 0 )
          {
            k        = i;
            anchor   = in;
            l_anchor = l_in;
          }

          d = FT_MulFix( in.x, out.x ) + FT_MulFix( in.y, out.y );

          /* shift only if turn is less than ~160 degrees */
          अगर ( d > -0xF000L )
          {
            d = d + 0x10000L;

            /* shift components along lateral bisector in proper orientation */
            shift.x = in.y + out.y;
            shift.y = in.x + out.x;

            अगर ( orientation == FT_ORIENTATION_TRUETYPE )
              shift.x = -shift.x;
            अन्यथा
              shift.y = -shift.y;

            /* restrict shift magnitude to better handle collapsing segments */
            q = FT_MulFix( out.x, in.y ) - FT_MulFix( out.y, in.x );
            अगर ( orientation == FT_ORIENTATION_TRUETYPE )
              q = -q;

            l = FT_MIN( l_in, l_out );

            /* non-strict inequalities avoid divide-by-zero when q == l == 0 */
            अगर ( FT_MulFix( xstrength, q ) <= FT_MulFix( l, d ) )
              shift.x = FT_MulDiv( shift.x, xstrength, d );
            अन्यथा
              shift.x = FT_MulDiv( shift.x, l, q );


            अगर ( FT_MulFix( ystrength, q ) <= FT_MulFix( l, d ) )
              shift.y = FT_MulDiv( shift.y, ystrength, d );
            अन्यथा
              shift.y = FT_MulDiv( shift.y, l, q );
          }
          अन्यथा
            shift.x = shift.y = 0;

          क्रम ( ;
                i != j;
                i = i < last ? i + 1 : first )
          {
            points[i].x += xstrength + shift.x;
            points[i].y += ystrength + shift.y;
          }
        }
        अन्यथा
          i = j;

        in   = out;
        l_in = l_out;
      }
    }

    वापस FT_Err_Ok;
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Orientation )
  FT_Outline_Get_Orientation( FT_Outline*  outline )
  {
    FT_Vector*  points;
    FT_Int      c, n, first, last;

#अगर_घोषित FT_INT64
    FT_Int64    area = 0;
#अन्यथा
    FT_BBox     cbox = { 0, 0, 0, 0 };
    FT_Int      xshift, yshift;
    FT_Vector   v_पूर्व, v_cur;
    FT_Pos      area = 0;
#पूर्ण_अगर


    अगर ( !outline || outline->n_points <= 0 )
      वापस FT_ORIENTATION_TRUETYPE;

    /* We use the nonzero winding rule to find the orientation.       */
    /* Since glyph outlines behave much more `regular' than arbitrary */
    /* cubic or quadratic curves, this test deals with the polygon    */
    /* only that is spanned up by the control points.                 */

    points = outline->points;

#अगर_घोषित FT_INT64

    /* Single-pass shoelace.  The 64-bit accumulator removes the need   */
    /* for the coordinate shift pre-pass (and its `FT_Outline_Get_CBox` */
    /* traversal) that the 32-bit variant below uses.                   */
    last = -1;
    क्रम ( c = 0; c < outline->n_contours; c++ )
    {
      FT_Pos  पूर्व_x, पूर्व_y;


      first = last + 1;
      last  = outline->contours[c];

      पूर्व_x = points[last].x;
      पूर्व_y = points[last].y;

      क्रम ( n = first; n <= last; n++ )
      {
        FT_Pos  cur_x = points[n].x;
        FT_Pos  cur_y = points[n].y;


        area = ADD_INT64( area,
                          MUL_INT64( SUB_INT64( cur_y, पूर्व_y ),
                                     ADD_INT64( cur_x, पूर्व_x ) ) );

        पूर्व_x = cur_x;
        पूर्व_y = cur_y;
      }
    }

#अन्यथा /* !FT_INT64 */

    FT_Outline_Get_CBox( outline, &cbox );

    /* Handle collapsed outlines to avoid undefined FT_MSB. */
    अगर ( cbox.xMin == cbox.xMax || cbox.yMin == cbox.yMax )
      वापस FT_ORIENTATION_NONE;

    /* Reject values large outlines. */
    अगर ( cbox.xMin < -0x1000000L || cbox.yMin < -0x1000000L ||
         cbox.xMax >  0x1000000L || cbox.yMax >  0x1000000L )
      वापस FT_ORIENTATION_NONE;

    xshift = FT_MSB( (FT_UInt32)( FT_ABS( cbox.xMax ) |
                                  FT_ABS( cbox.xMin ) ) ) - 14;
    xshift = FT_MAX( xshift, 0 );

    yshift = FT_MSB( (FT_UInt32)( cbox.yMax - cbox.yMin ) ) - 14;
    yshift = FT_MAX( yshift, 0 );

    last = -1;
    क्रम ( c = 0; c < outline->n_contours; c++ )
    {
      first = last + 1;
      last  = outline->contours[c];

      v_पूर्व.x = points[last].x >> xshift;
      v_पूर्व.y = points[last].y >> yshift;

      क्रम ( n = first; n <= last; n++ )
      {
        v_cur.x = points[n].x >> xshift;
        v_cur.y = points[n].y >> yshift;

        area = ADD_LONG( area,
                         MUL_LONG( v_cur.y - v_पूर्व.y,
                                   v_cur.x + v_पूर्व.x ) );

        v_पूर्व = v_cur;
      }
    }

#पूर्ण_अगर /* FT_INT64 */

    अगर ( area > 0 )
      वापस FT_ORIENTATION_POSTSCRIPT;
    अन्यथा अगर ( area < 0 )
      वापस FT_ORIENTATION_TRUETYPE;
    अन्यथा
      वापस FT_ORIENTATION_NONE;
  }


/* END */
