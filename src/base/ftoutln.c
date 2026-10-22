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


#ضمن <freetype/ftoutln.h>
#ضمن <freetype/internal/ftobjs.h>
#ضمن <freetype/internal/ftcalc.h>
#ضمن <freetype/internal/ftdebug.h>
#ضمن <freetype/fttrigon.h>


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#undef  FT_COMPONENT
#define FT_COMPONENT  outline


  ساكن
  ثابت FT_Outline  null_outline = { 0, 0, عدم, عدم, عدم, 0 };


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Outline_Decompose( FT_Outline*              outline,
                        ثابت FT_Outline_Funcs*  func_interface,
                        فراغ*                    user )
  {
#undef  SCALED
#define SCALED( x )  ( (x) * ( 1L << shift ) - delta )

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


    إذا ( !outline )
      أرجع FT_THROW( Invalid_Outline );

    إذا ( !func_interface )
      أرجع FT_THROW( Invalid_Argument );

    shift = func_interface->shift;
    delta = func_interface->delta;

    last = -1;
    لكل ( n = 0; n < outline->n_contours; n++ )
    {
      FT_TRACE5(( "FT_Outline_Decompose: Contour %d\n", n ));

      first = last + 1;
      last  = outline->contours[n];
      إذا ( last < first )
        اذهب Invalid_Outline;

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
      إذا ( tag != FT_CURVE_TAG_ON )
      {
        FT_Int  last_tag = FT_CURVE_TAG( outline->tags[last] );


        /* first point is a control point.  Yes, this happens. */
        إذا ( last_tag == FT_CURVE_TAG_ON )
        {
          /* start at last point if it is on the curve */
          v_start = v_last;
          limit--;
        }
        وإلا
        {
          إذا ( last_tag != tag )
            اذهب Invalid_Outline;

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
                  (مزدوج)v_start.x / 64, (مزدوج)v_start.y / 64 ));
      error = func_interface->move_to( &v_start, user );
      إذا ( error )
        اذهب Exit;

      طالما ( point < limit )
      {
        point++;
        tags++;

        tag = FT_CURVE_TAG( tags[0] );
        بدل ( tag )
        {
        حالة FT_CURVE_TAG_ON:  /* emit a single line_to */
          {
            FT_Vector  vec;


            vec.x = SCALED( point->x );
            vec.y = SCALED( point->y );

            FT_TRACE5(( "  line to (%.2f, %.2f)\n",
                        (مزدوج)vec.x / 64, (مزدوج)vec.y / 64 ));
            error = func_interface->سطر_to( &vec, user );
            إذا ( error )
              اذهب Exit;
            استمر;
          }

        حالة FT_CURVE_TAG_CONIC:  /* consume conic arcs */
          v_control.x = SCALED( point->x );
          v_control.y = SCALED( point->y );

        Do_Conic:
          إذا ( point < limit )
          {
            FT_Vector  vec;
            FT_Vector  v_وسط;


            point++;
            tags++;
            tag = FT_CURVE_TAG( tags[0] );

            vec.x = SCALED( point->x );
            vec.y = SCALED( point->y );

            إذا ( tag == FT_CURVE_TAG_ON )
            {
              FT_TRACE5(( "  conic to (%.2f, %.2f)"
                          " with control (%.2f, %.2f)\n",
                          (مزدوج)vec.x / 64,
                          (مزدوج)vec.y / 64,
                          (مزدوج)v_control.x / 64,
                          (مزدوج)v_control.y / 64 ));
              error = func_interface->conic_to( &v_control, &vec, user );
              إذا ( error )
                اذهب Exit;
              استمر;
            }

            إذا ( tag != FT_CURVE_TAG_CONIC )
              اذهب Invalid_Outline;

            v_وسط.x = ( v_control.x + vec.x ) / 2;
            v_وسط.y = ( v_control.y + vec.y ) / 2;

            FT_TRACE5(( "  conic to (%.2f, %.2f)"
                        " with control (%.2f, %.2f)\n",
                        (مزدوج)v_وسط.x / 64,
                        (مزدوج)v_وسط.y / 64,
                        (مزدوج)v_control.x / 64,
                        (مزدوج)v_control.y / 64 ));
            error = func_interface->conic_to( &v_control, &v_وسط, user );
            إذا ( error )
              اذهب Exit;

            v_control = vec;
            اذهب Do_Conic;
          }

          FT_TRACE5(( "  conic to (%.2f, %.2f)"
                      " with control (%.2f, %.2f)\n",
                      (مزدوج)v_start.x / 64,
                      (مزدوج)v_start.y / 64,
                      (مزدوج)v_control.x / 64,
                      (مزدوج)v_control.y / 64 ));
          error = func_interface->conic_to( &v_control, &v_start, user );
          اذهب Close;

        افتراضي:  /* FT_CURVE_TAG_CUBIC */
          {
            FT_Vector  vec1, vec2;


            إذا ( point + 1 > limit                             ||
                 FT_CURVE_TAG( tags[1] ) != FT_CURVE_TAG_CUBIC )
              اذهب Invalid_Outline;

            point += 2;
            tags  += 2;

            vec1.x = SCALED( point[-2].x );
            vec1.y = SCALED( point[-2].y );

            vec2.x = SCALED( point[-1].x );
            vec2.y = SCALED( point[-1].y );

            إذا ( point <= limit )
            {
              FT_Vector  vec;
              FT_Vector  v_وسط;


              tag = FT_CURVE_TAG( tags[0] );

              vec.x = SCALED( point->x );
              vec.y = SCALED( point->y );

              إذا ( tag == FT_CURVE_TAG_CUBIC )
              {
                v_وسط.x = ( vec2.x + vec.x ) / 2;
                v_وسط.y = ( vec2.y + vec.y ) / 2;

                vec = v_وسط;
              }
              وإلا إذا ( tag != FT_CURVE_TAG_ON )
                اذهب Invalid_Outline;

              FT_TRACE5(( "  cubic to (%.2f, %.2f)"
                          " with controls (%.2f, %.2f) and (%.2f, %.2f)\n",
                          (مزدوج)vec.x / 64,
                          (مزدوج)vec.y / 64,
                          (مزدوج)vec1.x / 64,
                          (مزدوج)vec1.y / 64,
                          (مزدوج)vec2.x / 64,
                          (مزدوج)vec2.y / 64 ));
              error = func_interface->cubic_to( &vec1, &vec2, &vec, user );
              إذا ( error )
                اذهب Exit;

              إذا ( tag == FT_CURVE_TAG_CUBIC )
              {
                point--;
                tags--;
              }
              استمر;
            }

            FT_TRACE5(( "  cubic to (%.2f, %.2f)"
                        " with controls (%.2f, %.2f) and (%.2f, %.2f)\n",
                        (مزدوج)v_start.x / 64,
                        (مزدوج)v_start.y / 64,
                        (مزدوج)vec1.x / 64,
                        (مزدوج)vec1.y / 64,
                        (مزدوج)vec2.x / 64,
                        (مزدوج)vec2.y / 64 ));
            error = func_interface->cubic_to( &vec1, &vec2, &v_start, user );
            اذهب Close;
          }
        }
      }

      /* close the contour with a line segment */
      FT_TRACE5(( "  line to (%.2f, %.2f)\n",
                  (مزدوج)v_start.x / 64, (مزدوج)v_start.y / 64 ));
      error = func_interface->سطر_to( &v_start, user );

    Close:
      إذا ( error )
        اذهب Exit;
    }

    FT_TRACE5(( "FT_Outline_Decompose: Done\n" ));
    أرجع FT_Err_Ok;

  Invalid_Outline:
    error = FT_THROW( Invalid_Outline );
    /* fall through */

  Exit:
    FT_TRACE5(( "FT_Outline_Decompose: Error 0x%x\n", error ));
    أرجع error;
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Outline_New( FT_Library   library,
                  FT_UInt      رقم_Points,
                  FT_Int       رقم_Contours,
                  FT_Outline  *anoutline )
  {
    FT_Error   error;
    FT_Memory  memory;


    إذا ( !library )
      أرجع FT_THROW( Invalid_Library_Handle );

    memory = library->memory;

    إذا ( !anoutline || !memory )
      أرجع FT_THROW( Invalid_Argument );

    *anoutline = null_outline;

    إذا ( رقم_Contours < 0                  ||
         (FT_UInt)رقم_Contours > رقم_Points )
      أرجع FT_THROW( Invalid_Argument );

    إذا ( رقم_Points > FT_OUTLINE_POINTS_MAX )
      أرجع FT_THROW( Array_Too_Large );

    إذا ( FT_NEW_ARRAY( anoutline->points,   رقم_Points   ) ||
         FT_NEW_ARRAY( anoutline->tags,     رقم_Points   ) ||
         FT_NEW_ARRAY( anoutline->contours, رقم_Contours ) )
      اذهب Fail;

    anoutline->n_points    = (FT_UShort)رقم_Points;
    anoutline->n_contours  = (FT_UShort)رقم_Contours;
    anoutline->flags      |= FT_OUTLINE_OWNER;

    أرجع FT_Err_Ok;

  Fail:
    anoutline->flags |= FT_OUTLINE_OWNER;
    FT_Outline_Done( library, anoutline );

    أرجع error;
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Outline_Check( FT_Outline*  outline )
  {
    إذا ( outline )
    {
      FT_Int  n_points   = outline->n_points;
      FT_Int  n_contours = outline->n_contours;
      FT_Int  end0, end;
      FT_Int  n;


      FT_TRACE5(( "FT_Outline_Check: contours = %d, points = %d\n",
                  n_contours, n_points ));
      /* empty glyph? */
      إذا ( n_points == 0 && n_contours == 0 )
        أرجع FT_Err_Ok;

      /* check point and contour counts */
      إذا ( n_points == 0 || n_contours == 0 )
        اذهب Bad;

      end0 = -1;
      لكل ( n = 0; n < n_contours; n++ )
      {
        end = outline->contours[n];

        /* note that we don't accept empty contours */
        إذا ( end <= end0 || end >= n_points )
          اذهب Bad;

        end0 = end;
      }

      إذا ( end0 != n_points - 1 )
        اذهب Bad;

      /* XXX: check the tags array */
      أرجع FT_Err_Ok;
    }

  Bad:
    أرجع FT_THROW( Invalid_Outline );
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Outline_Copy( ثابت FT_Outline*  source,
                   FT_Outline        *target )
  {
    FT_Int  is_owner;


    إذا ( !source || !target )
      أرجع FT_THROW( Invalid_Outline );

    إذا ( source->n_points   != target->n_points   ||
         source->n_contours != target->n_contours )
      أرجع FT_THROW( Invalid_Argument );

    إذا ( source == target )
      أرجع FT_Err_Ok;

    إذا ( source->n_points )
    {
      FT_ARRAY_COPY( target->points, source->points, source->n_points );
      FT_ARRAY_COPY( target->tags,   source->tags,   source->n_points );
    }

    إذا ( source->n_contours )
      FT_ARRAY_COPY( target->contours, source->contours, source->n_contours );

    /* copy all flags, except the `FT_OUTLINE_OWNER' one */
    is_owner      = target->flags & FT_OUTLINE_OWNER;
    target->flags = source->flags;

    target->flags &= ~FT_OUTLINE_OWNER;
    target->flags |= is_owner;

    أرجع FT_Err_Ok;
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Outline_Done( FT_Library   library,
                   FT_Outline*  outline )
  {
    FT_Memory  memory;


    إذا ( !library )
      أرجع FT_THROW( Invalid_Library_Handle );

    إذا ( !outline )
      أرجع FT_THROW( Invalid_Outline );

    memory = library->memory;

    إذا ( !memory )
      أرجع FT_THROW( Invalid_Argument );

    إذا ( outline->flags & FT_OUTLINE_OWNER )
    {
      FT_FREE( outline->points   );
      FT_FREE( outline->tags     );
      FT_FREE( outline->contours );
    }
    *outline = null_outline;

    أرجع FT_Err_Ok;
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( فراغ )
  FT_Outline_Get_CBox( ثابت FT_Outline*  outline,
                       FT_BBox           *acbox )
  {
    FT_Pos  xMin, yMin, xMax, yMax;


    إذا ( outline && acbox )
    {
      إذا ( outline->n_points == 0 )
      {
        xMin = 0;
        yMin = 0;
        xMax = 0;
        yMax = 0;
      }
      وإلا
      {
        FT_Vector*  vec   = outline->points;
        FT_Vector*  limit = vec + outline->n_points;


        xMin = xMax = vec->x;
        yMin = yMax = vec->y;
        vec++;

        لكل ( ; vec < limit; vec++ )
        {
          FT_Pos  x, y;


          x = vec->x;
          إذا ( x < xMin ) xMin = x;
          إذا ( x > xMax ) xMax = x;

          y = vec->y;
          إذا ( y < yMin ) yMin = y;
          إذا ( y > yMax ) yMax = y;
        }
      }
      acbox->xMin = xMin;
      acbox->xMax = xMax;
      acbox->yMin = yMin;
      acbox->yMax = yMax;
    }
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( فراغ )
  FT_Outline_Translate( ثابت FT_Outline*  outline,
                        FT_Pos             xOffset,
                        FT_Pos             yOffset )
  {
    FT_UShort   n;
    FT_Vector*  vec;


    إذا ( !outline )
      أرجع;

    vec = outline->points;

    لكل ( n = 0; n < outline->n_points; n++ )
    {
      vec->x = ADD_LONG( vec->x, xOffset );
      vec->y = ADD_LONG( vec->y, yOffset );
      vec++;
    }
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( فراغ )
  FT_Outline_Reverse( FT_Outline*  outline )
  {
    FT_UShort  n;
    FT_Int     first, last;


    إذا ( !outline )
      أرجع;

    last = -1;
    لكل ( n = 0; n < outline->n_contours; n++ )
    {
      first = last + 1;
      last  = outline->contours[n];

      /* Keep the first point unless it is a cubic control.  In that */
      /* case, reverse the entire contour to preserve control pairs, */
      /* including contours with no explicit on-curve points.        */
      إذا ( FT_CURVE_TAG( outline->tags[first] ) != FT_CURVE_TAG_CUBIC )
        first++;

      /* reverse point table */
      {
        FT_Vector*  p = outline->points + first;
        FT_Vector*  q = outline->points + last;
        FT_Vector   swap;


        طالما ( p < q )
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


        طالما ( p < q )
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
    FT_List_عقدة  عقدة;
    FT_BBox      cbox;


    إذا ( !library )
      أرجع FT_THROW( Invalid_Library_Handle );

    إذا ( !outline )
      أرجع FT_THROW( Invalid_Outline );

    إذا ( !params )
      أرجع FT_THROW( Invalid_Argument );

    FT_Outline_Get_CBox( outline, &cbox );
    إذا ( cbox.xMin < -0x1000000L || cbox.yMin < -0x1000000L ||
         cbox.xMax >  0x1000000L || cbox.yMax >  0x1000000L )
      أرجع FT_THROW( Invalid_Outline );

    renderer = library->cur_renderer;
    عقدة     = library->renderers.head;

    params->source = (فراغ*)outline;

    /* preset clip_box for direct mode */
    إذا ( params->flags & FT_RASTER_FLAG_DIRECT    &&
         !( params->flags & FT_RASTER_FLAG_CLIP ) )
    {
      params->clip_box.xMin = cbox.xMin >> 6;
      params->clip_box.yMin = cbox.yMin >> 6;
      params->clip_box.xMax = ( cbox.xMax + 63 ) >> 6;
      params->clip_box.yMax = ( cbox.yMax + 63 ) >> 6;
    }

    error = FT_ERR( Cannot_Render_Glyph );
    طالما ( renderer )
    {
      error = renderer->raster_render( renderer->raster, params );
      إذا ( !error || FT_ERR_NEQ( error, Cannot_Render_Glyph ) )
        اكسر;

      /* FT_Err_Cannot_Render_Glyph is returned if the render mode   */
      /* is unsupported by the current renderer for this glyph image */
      /* format                                                      */

      /* now, look for another renderer that supports the same */
      /* format                                                */
      renderer = FT_Lookup_Renderer( library, FT_GLYPH_FORMAT_OUTLINE,
                                     &عقدة );
    }

    أرجع error;
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Outline_Get_Bitmap( FT_Library        library,
                         FT_Outline*       outline,
                         ثابت FT_Bitmap  *abitmap )
  {
    FT_Raster_Params  params;


    إذا ( !abitmap )
      أرجع FT_THROW( Invalid_Argument );

    /* other checks are delayed to `FT_Outline_Render' */

    params.target = abitmap;
    params.flags  = 0;

    إذا ( abitmap->pixel_mode == FT_PIXEL_MODE_GRAY  ||
         abitmap->pixel_mode == FT_PIXEL_MODE_LCD   ||
         abitmap->pixel_mode == FT_PIXEL_MODE_LCD_V )
      params.flags |= FT_RASTER_FLAG_AA;

    أرجع FT_Outline_Render( library, outline, &params );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( فراغ )
  FT_Vector_Transform( FT_Vector*        vector,
                       ثابت FT_Matrix*  matrix )
  {
    FT_Pos  xz, yz;


    إذا ( !vector || !matrix )
      أرجع;

    xz = FT_MulFix( vector->x, matrix->xx ) +
         FT_MulFix( vector->y, matrix->xy );

    yz = FT_MulFix( vector->x, matrix->yx ) +
         FT_MulFix( vector->y, matrix->yy );

    vector->x = xz;
    vector->y = yz;
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( فراغ )
  FT_Outline_Transform( ثابت FT_Outline*  outline,
                        ثابت FT_Matrix*   matrix )
  {
    FT_Vector*  vec;
    FT_Vector*  limit;


    إذا ( !outline || !matrix || !outline->points )
      أرجع;

    vec   = outline->points;
    limit = vec + outline->n_points;

    لكل ( ; vec < limit; vec++ )
      FT_Vector_Transform( vec, matrix );
  }


#إذا 0

#define FT_OUTLINE_GET_CONTOUR( outline, c, first, last )  \
  افعل                                                       \
  {                                                        \
    (first) = ( c > 0 ) ? (outline)->points +              \
                            (outline)->contours[c - 1] + 1 \
                        : (outline)->points;               \
    (last) = (outline)->points + (outline)->contours[c];   \
  } طالما ( 0 )


  /* Is a point in some contour?                     */
  /*                                                 */
  /* We treat every point of the contour as if it    */
  /* it were ON.  That is, we allow false positives, */
  /* but disallow false negatives.  (XXX really?)    */
  ساكن FT_Bool
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

    لكل ( a = first; a <= last; a++ )
    {
      FT_Pos  x;
      FT_Int  intersect;


      b = ( a == last ) ? first : a + 1;

      intersect = ( a->y - point->y ) ^ ( b->y - point->y );

      /* a and b are on the same side */
      إذا ( intersect >= 0 )
      {
        إذا ( intersect == 0 && a->y == point->y )
        {
          إذا ( ( a->x <= point->x && b->x >= point->x ) ||
               ( a->x >= point->x && b->x <= point->x ) )
            أرجع 1;
        }

        استمر;
      }

      x = a->x + ( b->x - a->x ) * (point->y - a->y ) / ( b->y - a->y );

      إذا ( x < point->x )
        n++;
      وإلا إذا ( x == point->x )
        أرجع 1;
    }

    أرجع n & 1;
  }


  ساكن FT_Bool
  ft_contour_enclosed( FT_Outline*  outline,
                       FT_UShort    c )
  {
    FT_Vector*  first;
    FT_Vector*  last;
    FT_Short    i;


    FT_OUTLINE_GET_CONTOUR( outline, c, first, last );

    لكل ( i = 0; i < outline->n_contours; i++ )
    {
      إذا ( i != c && ft_contour_has( outline, i, first ) )
      {
        FT_Vector*  pt;


        لكل ( pt = first + 1; pt <= last; pt++ )
          إذا ( !ft_contour_has( outline, i, pt ) )
            أرجع 0;

        أرجع 1;
      }
    }

    أرجع 0;
  }


  /* This version differs from the public one in that each */
  /* part (contour not enclosed in another contour) of the */
  /* outline is checked for orientation.  This is          */
  /* necessary for some buggy CJK fonts.                   */
  ساكن FT_Orientation
  ft_outline_get_orientation( FT_Outline*  outline )
  {
    FT_Short        i;
    FT_Vector*      first;
    FT_Vector*      last;
    FT_Orientation  orient = FT_ORIENTATION_NONE;


    first = outline->points;
    لكل ( i = 0; i < outline->n_contours; i++, first = last + 1 )
    {
      FT_Vector*  point;
      FT_Vector*  xmin_point;
      FT_Pos      xmin;


      last = outline->points + outline->contours[i];

      /* skip degenerate contours */
      إذا ( last < first + 2 )
        استمر;

      إذا ( ft_contour_enclosed( outline, i ) )
        استمر;

      xmin       = first->x;
      xmin_point = first;

      لكل ( point = first + 1; point <= last; point++ )
      {
        إذا ( point->x < xmin )
        {
          xmin       = point->x;
          xmin_point = point;
        }
      }

      /* check the orientation of the contour */
      {
        FT_Vector*      prev;
        FT_Vector*      next;
        FT_Orientation  o;


        prev = ( xmin_point == first ) ? last : xmin_point - 1;
        next = ( xmin_point == last ) ? first : xmin_point + 1;

        إذا ( FT_Atan2( prev->x - xmin_point->x, prev->y - xmin_point->y ) >
             FT_Atan2( next->x - xmin_point->x, next->y - xmin_point->y ) )
          o = FT_ORIENTATION_POSTSCRIPT;
        وإلا
          o = FT_ORIENTATION_TRUETYPE;

        إذا ( orient == FT_ORIENTATION_NONE )
          orient = o;
        وإلا إذا ( orient != o )
          أرجع FT_ORIENTATION_NONE;
      }
    }

    أرجع orient;
  }

#endif /* 0 */


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Outline_Embolden( FT_Outline*  outline,
                       FT_Pos       strength )
  {
    أرجع FT_Outline_EmboldenXY( outline, strength, strength );
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


    إذا ( !outline )
      أرجع FT_THROW( Invalid_Outline );

    xstrength /= 2;
    ystrength /= 2;
    إذا ( xstrength == 0 && ystrength == 0 )
      أرجع FT_Err_Ok;

    orientation = FT_Outline_Get_Orientation( outline );
    إذا ( orientation == FT_ORIENTATION_NONE )
    {
      إذا ( outline->n_contours )
        أرجع FT_THROW( Invalid_Argument );
      وإلا
        أرجع FT_Err_Ok;
    }

    points = outline->points;

    last = -1;
    لكل ( c = 0; c < outline->n_contours; c++ )
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
      لكل ( i = last, j = first, k = -1;
            j != i && i != k;
            j = j < last ? j + 1 : first )
      {
        إذا ( j != k )
        {
          out.x = points[j].x - points[i].x;
          out.y = points[j].y - points[i].y;
          l_out = (FT_Fixed)FT_Vector_NormLen( &out );

          إذا ( l_out == 0 )
            استمر;
        }
        وإلا
        {
          out   = anchor;
          l_out = l_anchor;
        }

        إذا ( l_in != 0 )
        {
          إذا ( k < 0 )
          {
            k        = i;
            anchor   = in;
            l_anchor = l_in;
          }

          d = FT_MulFix( in.x, out.x ) + FT_MulFix( in.y, out.y );

          /* shift only if turn is less than ~160 degrees */
          إذا ( d > -0xF000L )
          {
            d = d + 0x10000L;

            /* shift components along lateral bisector in proper orientation */
            shift.x = in.y + out.y;
            shift.y = in.x + out.x;

            إذا ( orientation == FT_ORIENTATION_TRUETYPE )
              shift.x = -shift.x;
            وإلا
              shift.y = -shift.y;

            /* restrict shift magnitude to better handle collapsing segments */
            q = FT_MulFix( out.x, in.y ) - FT_MulFix( out.y, in.x );
            إذا ( orientation == FT_ORIENTATION_TRUETYPE )
              q = -q;

            l = FT_MIN( l_in, l_out );

            /* non-strict inequalities avoid divide-by-zero when q == l == 0 */
            إذا ( FT_MulFix( xstrength, q ) <= FT_MulFix( l, d ) )
              shift.x = FT_MulDiv( shift.x, xstrength, d );
            وإلا
              shift.x = FT_MulDiv( shift.x, l, q );


            إذا ( FT_MulFix( ystrength, q ) <= FT_MulFix( l, d ) )
              shift.y = FT_MulDiv( shift.y, ystrength, d );
            وإلا
              shift.y = FT_MulDiv( shift.y, l, q );
          }
          وإلا
            shift.x = shift.y = 0;

          لكل ( ;
                i != j;
                i = i < last ? i + 1 : first )
          {
            points[i].x += xstrength + shift.x;
            points[i].y += ystrength + shift.y;
          }
        }
        وإلا
          i = j;

        in   = out;
        l_in = l_out;
      }
    }

    أرجع FT_Err_Ok;
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Orientation )
  FT_Outline_Get_Orientation( FT_Outline*  outline )
  {
    FT_Vector*  points;
    FT_Int      c, n, first, last;

#ifdef FT_INT64
    FT_Int64    area = 0;
#وإلا
    FT_BBox     cbox = { 0, 0, 0, 0 };
    FT_Int      xshift, yshift;
    FT_Vector   v_prev, v_cur;
    FT_Pos      area = 0;
#endif


    إذا ( !outline || outline->n_points <= 0 )
      أرجع FT_ORIENTATION_TRUETYPE;

    /* We use the nonzero winding rule to find the orientation.       */
    /* Since glyph outlines behave much more `regular' than arbitrary */
    /* cubic or quadratic curves, this test deals with the polygon    */
    /* only that is spanned up by the control points.                 */

    points = outline->points;

#ifdef FT_INT64

    /* Single-pass shoelace.  The 64-bit accumulator removes the need   */
    /* for the coordinate shift pre-pass (and its `FT_Outline_Get_CBox` */
    /* traversal) that the 32-bit variant below uses.                   */
    last = -1;
    لكل ( c = 0; c < outline->n_contours; c++ )
    {
      FT_Pos  prev_x, prev_y;


      first = last + 1;
      last  = outline->contours[c];

      prev_x = points[last].x;
      prev_y = points[last].y;

      لكل ( n = first; n <= last; n++ )
      {
        FT_Pos  cur_x = points[n].x;
        FT_Pos  cur_y = points[n].y;


        area = ADD_INT64( area,
                          MUL_INT64( SUB_INT64( cur_y, prev_y ),
                                     ADD_INT64( cur_x, prev_x ) ) );

        prev_x = cur_x;
        prev_y = cur_y;
      }
    }

#وإلا /* !FT_INT64 */

    FT_Outline_Get_CBox( outline, &cbox );

    /* Handle collapsed outlines to avoid undefined FT_MSB. */
    إذا ( cbox.xMin == cbox.xMax || cbox.yMin == cbox.yMax )
      أرجع FT_ORIENTATION_NONE;

    /* Reject values large outlines. */
    إذا ( cbox.xMin < -0x1000000L || cbox.yMin < -0x1000000L ||
         cbox.xMax >  0x1000000L || cbox.yMax >  0x1000000L )
      أرجع FT_ORIENTATION_NONE;

    xshift = FT_MSB( (FT_UInt32)( FT_ABS( cbox.xMax ) |
                                  FT_ABS( cbox.xMin ) ) ) - 14;
    xshift = FT_MAX( xshift, 0 );

    yshift = FT_MSB( (FT_UInt32)( cbox.yMax - cbox.yMin ) ) - 14;
    yshift = FT_MAX( yshift, 0 );

    last = -1;
    لكل ( c = 0; c < outline->n_contours; c++ )
    {
      first = last + 1;
      last  = outline->contours[c];

      v_prev.x = points[last].x >> xshift;
      v_prev.y = points[last].y >> yshift;

      لكل ( n = first; n <= last; n++ )
      {
        v_cur.x = points[n].x >> xshift;
        v_cur.y = points[n].y >> yshift;

        area = ADD_LONG( area,
                         MUL_LONG( v_cur.y - v_prev.y,
                                   v_cur.x + v_prev.x ) );

        v_prev = v_cur;
      }
    }

#endif /* FT_INT64 */

    إذا ( area > 0 )
      أرجع FT_ORIENTATION_POSTSCRIPT;
    وإلا إذا ( area < 0 )
      أرجع FT_ORIENTATION_TRUETYPE;
    وإلا
      أرجع FT_ORIENTATION_NONE;
  }


/* END */
