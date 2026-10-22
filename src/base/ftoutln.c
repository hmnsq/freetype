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


#अन्तर्भाव <freetype/ftoutln.h>
#अन्तर्भाव <freetype/internal/ftobjs.h>
#अन्तर्भाव <freetype/internal/ftcalc.h>
#अन्तर्भाव <freetype/internal/ftdebug.h>
#अन्तर्भाव <freetype/fttrigon.h>


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#अपरिभाषय  FT_COMPONENT
#परिभाषय FT_COMPONENT  outline


  स्थिर
  ध्रुव FT_Outline  null_outline = { 0, 0, रिक्त, रिक्त, रिक्त, 0 };


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Outline_Decompose( FT_Outline*              outline,
                        ध्रुव FT_Outline_Funcs*  func_interface,
                        शून्य*                    user )
  {
#अपरिभाषय  SCALED
#परिभाषय SCALED( x )  ( (x) * ( 1L << सरणम् ) - delta )

    FT_सदिशम्   v_चरमः;
    FT_सदिशम्   v_control;
    FT_सदिशम्   v_start;

    FT_सदिशम्*  point;
    FT_सदिशम्*  सीमा;
    FT_Byte*    चिह्नकानि;

    FT_Error    error;

    FT_Int   n;         /* index of contour in outline     */
    FT_Int   first;     /* index of first point in contour */
    FT_Int   चरमः;      /* index of last point in contour  */

    FT_Int   चिह्नकम्;       /* current point's state           */

    FT_Int   सरणम्;
    FT_Pos   delta;


    यदि ( !outline )
      प्रत्यागम FT_THROW( Invalid_Outline );

    यदि ( !func_interface )
      प्रत्यागम FT_THROW( Invalid_तर्कः );

    सरणम् = func_interface->सरणम्;
    delta = func_interface->delta;

    चरमः = -1;
    प्रति ( n = 0; n < outline->n_contours; n++ )
    {
      FT_TRACE5(( "FT_Outline_Decompose: Contour %d\n", n ));

      first = चरमः + 1;
      चरमः  = outline->contours[n];
      यदि ( चरमः < first )
        गच्छ Invalid_Outline;

      सीमा = outline->points + चरमः;

      v_start   = outline->points[first];
      v_start.x = SCALED( v_start.x );
      v_start.y = SCALED( v_start.y );

      v_चरमः   = outline->points[चरमः];
      v_चरमः.x = SCALED( v_चरमः.x );
      v_चरमः.y = SCALED( v_चरमः.y );

      v_control = v_start;

      point = outline->points + first;
      चिह्नकानि  = outline->चिह्नकानि   + first;
      चिह्नकम्   = FT_वक्रम्_चिह्नकम्( चिह्नकानि[0] );

      /* check first point to determine origin */
      यदि ( चिह्नकम् != FT_वक्रम्_चिह्नकम्_ON )
      {
        FT_Int  चरमः_चिह्नकम् = FT_वक्रम्_चिह्नकम्( outline->चिह्नकानि[चरमः] );


        /* first point is a control point.  Yes, this happens. */
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

          /* if both first and last points are controls,   */
          /* start at their middle and record its position */
          /* for closure                                   */
          v_start.x = ( v_start.x + v_चरमः.x ) / 2;
          v_start.y = ( v_start.y + v_चरमः.y ) / 2;

          /* v_last = v_start; */
        }
        point--;
        चिह्नकानि--;
      }

      FT_TRACE5(( "  move to (%.2f, %.2f)\n",
                  (द्विगुण)v_start.x / 64, (द्विगुण)v_start.y / 64 ));
      error = func_interface->move_गन्तव्यम्( &v_start, user );
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


            vec.x = SCALED( point->x );
            vec.y = SCALED( point->y );

            FT_TRACE5(( "  line to (%.2f, %.2f)\n",
                        (द्विगुण)vec.x / 64, (द्विगुण)vec.y / 64 ));
            error = func_interface->पङ्क्तिः_गन्तव्यम्( &vec, user );
            यदि ( error )
              गच्छ Exit;
            अनुवर्त;
          }

        पक्ष FT_वक्रम्_चिह्नकम्_CONIC:  /* consume conic arcs */
          v_control.x = SCALED( point->x );
          v_control.y = SCALED( point->y );

        Do_Conic:
          यदि ( point < सीमा )
          {
            FT_सदिशम्  vec;
            FT_सदिशम्  v_मध्यः;


            point++;
            चिह्नकानि++;
            चिह्नकम् = FT_वक्रम्_चिह्नकम्( चिह्नकानि[0] );

            vec.x = SCALED( point->x );
            vec.y = SCALED( point->y );

            यदि ( चिह्नकम् == FT_वक्रम्_चिह्नकम्_ON )
            {
              FT_TRACE5(( "  conic to (%.2f, %.2f)"
                          " with control (%.2f, %.2f)\n",
                          (द्विगुण)vec.x / 64,
                          (द्विगुण)vec.y / 64,
                          (द्विगुण)v_control.x / 64,
                          (द्विगुण)v_control.y / 64 ));
              error = func_interface->conic_गन्तव्यम्( &v_control, &vec, user );
              यदि ( error )
                गच्छ Exit;
              अनुवर्त;
            }

            यदि ( चिह्नकम् != FT_वक्रम्_चिह्नकम्_CONIC )
              गच्छ Invalid_Outline;

            v_मध्यः.x = ( v_control.x + vec.x ) / 2;
            v_मध्यः.y = ( v_control.y + vec.y ) / 2;

            FT_TRACE5(( "  conic to (%.2f, %.2f)"
                        " with control (%.2f, %.2f)\n",
                        (द्विगुण)v_मध्यः.x / 64,
                        (द्विगुण)v_मध्यः.y / 64,
                        (द्विगुण)v_control.x / 64,
                        (द्विगुण)v_control.y / 64 ));
            error = func_interface->conic_गन्तव्यम्( &v_control, &v_मध्यः, user );
            यदि ( error )
              गच्छ Exit;

            v_control = vec;
            गच्छ Do_Conic;
          }

          FT_TRACE5(( "  conic to (%.2f, %.2f)"
                      " with control (%.2f, %.2f)\n",
                      (द्विगुण)v_start.x / 64,
                      (द्विगुण)v_start.y / 64,
                      (द्विगुण)v_control.x / 64,
                      (द्विगुण)v_control.y / 64 ));
          error = func_interface->conic_गन्तव्यम्( &v_control, &v_start, user );
          गच्छ Close;

        स्वभाव:  /* FT_CURVE_TAG_CUBIC */
          {
            FT_सदिशम्  vec1, vec2;


            यदि ( point + 1 > सीमा                             ||
                 FT_वक्रम्_चिह्नकम्( चिह्नकानि[1] ) != FT_वक्रम्_चिह्नकम्_CUBIC )
              गच्छ Invalid_Outline;

            point += 2;
            चिह्नकानि  += 2;

            vec1.x = SCALED( point[-2].x );
            vec1.y = SCALED( point[-2].y );

            vec2.x = SCALED( point[-1].x );
            vec2.y = SCALED( point[-1].y );

            यदि ( point <= सीमा )
            {
              FT_सदिशम्  vec;
              FT_सदिशम्  v_मध्यः;


              चिह्नकम् = FT_वक्रम्_चिह्नकम्( चिह्नकानि[0] );

              vec.x = SCALED( point->x );
              vec.y = SCALED( point->y );

              यदि ( चिह्नकम् == FT_वक्रम्_चिह्नकम्_CUBIC )
              {
                v_मध्यः.x = ( vec2.x + vec.x ) / 2;
                v_मध्यः.y = ( vec2.y + vec.y ) / 2;

                vec = v_मध्यः;
              }
              अन्यथा यदि ( चिह्नकम् != FT_वक्रम्_चिह्नकम्_ON )
                गच्छ Invalid_Outline;

              FT_TRACE5(( "  cubic to (%.2f, %.2f)"
                          " with controls (%.2f, %.2f) and (%.2f, %.2f)\n",
                          (द्विगुण)vec.x / 64,
                          (द्विगुण)vec.y / 64,
                          (द्विगुण)vec1.x / 64,
                          (द्विगुण)vec1.y / 64,
                          (द्विगुण)vec2.x / 64,
                          (द्विगुण)vec2.y / 64 ));
              error = func_interface->cubic_गन्तव्यम्( &vec1, &vec2, &vec, user );
              यदि ( error )
                गच्छ Exit;

              यदि ( चिह्नकम् == FT_वक्रम्_चिह्नकम्_CUBIC )
              {
                point--;
                चिह्नकानि--;
              }
              अनुवर्त;
            }

            FT_TRACE5(( "  cubic to (%.2f, %.2f)"
                        " with controls (%.2f, %.2f) and (%.2f, %.2f)\n",
                        (द्विगुण)v_start.x / 64,
                        (द्विगुण)v_start.y / 64,
                        (द्विगुण)vec1.x / 64,
                        (द्विगुण)vec1.y / 64,
                        (द्विगुण)vec2.x / 64,
                        (द्विगुण)vec2.y / 64 ));
            error = func_interface->cubic_गन्तव्यम्( &vec1, &vec2, &v_start, user );
            गच्छ Close;
          }
        }
      }

      /* close the contour with a line segment */
      FT_TRACE5(( "  line to (%.2f, %.2f)\n",
                  (द्विगुण)v_start.x / 64, (द्विगुण)v_start.y / 64 ));
      error = func_interface->पङ्क्तिः_गन्तव्यम्( &v_start, user );

    Close:
      यदि ( error )
        गच्छ Exit;
    }

    FT_TRACE5(( "FT_Outline_Decompose: Done\n" ));
    प्रत्यागम FT_Err_Ok;

  Invalid_Outline:
    error = FT_THROW( Invalid_Outline );
    /* fall through */

  Exit:
    FT_TRACE5(( "FT_Outline_Decompose: Error 0x%x\n", error ));
    प्रत्यागम error;
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Outline_New( FT_Library   library,
                  FT_UInt      अङ्कः_Points,
                  FT_Int       अङ्कः_Contours,
                  FT_Outline  *anoutline )
  {
    FT_Error   error;
    FT_स्मृतिः  स्मृतिः;


    यदि ( !library )
      प्रत्यागम FT_THROW( Invalid_Library_Handle );

    स्मृतिः = library->स्मृतिः;

    यदि ( !anoutline || !स्मृतिः )
      प्रत्यागम FT_THROW( Invalid_तर्कः );

    *anoutline = null_outline;

    यदि ( अङ्कः_Contours < 0                  ||
         (FT_UInt)अङ्कः_Contours > अङ्कः_Points )
      प्रत्यागम FT_THROW( Invalid_तर्कः );

    यदि ( अङ्कः_Points > FT_OUTLINE_POINTS_MAX )
      प्रत्यागम FT_THROW( Array_Too_Large );

    यदि ( FT_NEW_ARRAY( anoutline->points,   अङ्कः_Points   ) ||
         FT_NEW_ARRAY( anoutline->चिह्नकानि,     अङ्कः_Points   ) ||
         FT_NEW_ARRAY( anoutline->contours, अङ्कः_Contours ) )
      गच्छ Fail;

    anoutline->n_points    = (FT_UShort)अङ्कः_Points;
    anoutline->n_contours  = (FT_UShort)अङ्कः_Contours;
    anoutline->flags      |= FT_OUTLINE_OWNER;

    प्रत्यागम FT_Err_Ok;

  Fail:
    anoutline->flags |= FT_OUTLINE_OWNER;
    FT_Outline_समाप्तम्( library, anoutline );

    प्रत्यागम error;
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Outline_Check( FT_Outline*  outline )
  {
    यदि ( outline )
    {
      FT_Int  n_points   = outline->n_points;
      FT_Int  n_contours = outline->n_contours;
      FT_Int  end0, अन्तः;
      FT_Int  n;


      FT_TRACE5(( "FT_Outline_Check: contours = %d, points = %d\n",
                  n_contours, n_points ));
      /* empty glyph? */
      यदि ( n_points == 0 && n_contours == 0 )
        प्रत्यागम FT_Err_Ok;

      /* check point and contour counts */
      यदि ( n_points == 0 || n_contours == 0 )
        गच्छ Bad;

      end0 = -1;
      प्रति ( n = 0; n < n_contours; n++ )
      {
        अन्तः = outline->contours[n];

        /* note that we don't accept empty contours */
        यदि ( अन्तः <= end0 || अन्तः >= n_points )
          गच्छ Bad;

        end0 = अन्तः;
      }

      यदि ( end0 != n_points - 1 )
        गच्छ Bad;

      /* XXX: check the tags array */
      प्रत्यागम FT_Err_Ok;
    }

  Bad:
    प्रत्यागम FT_THROW( Invalid_Outline );
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Outline_Copy( ध्रुव FT_Outline*  source,
                   FT_Outline        *target )
  {
    FT_Int  is_owner;


    यदि ( !source || !target )
      प्रत्यागम FT_THROW( Invalid_Outline );

    यदि ( source->n_points   != target->n_points   ||
         source->n_contours != target->n_contours )
      प्रत्यागम FT_THROW( Invalid_तर्कः );

    यदि ( source == target )
      प्रत्यागम FT_Err_Ok;

    यदि ( source->n_points )
    {
      FT_ARRAY_COPY( target->points, source->points, source->n_points );
      FT_ARRAY_COPY( target->चिह्नकानि,   source->चिह्नकानि,   source->n_points );
    }

    यदि ( source->n_contours )
      FT_ARRAY_COPY( target->contours, source->contours, source->n_contours );

    /* copy all flags, except the `FT_OUTLINE_OWNER' one */
    is_owner      = target->flags & FT_OUTLINE_OWNER;
    target->flags = source->flags;

    target->flags &= ~FT_OUTLINE_OWNER;
    target->flags |= is_owner;

    प्रत्यागम FT_Err_Ok;
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Outline_समाप्तम्( FT_Library   library,
                   FT_Outline*  outline )
  {
    FT_स्मृतिः  स्मृतिः;


    यदि ( !library )
      प्रत्यागम FT_THROW( Invalid_Library_Handle );

    यदि ( !outline )
      प्रत्यागम FT_THROW( Invalid_Outline );

    स्मृतिः = library->स्मृतिः;

    यदि ( !स्मृतिः )
      प्रत्यागम FT_THROW( Invalid_तर्कः );

    यदि ( outline->flags & FT_OUTLINE_OWNER )
    {
      FT_FREE( outline->points   );
      FT_FREE( outline->चिह्नकानि     );
      FT_FREE( outline->contours );
    }
    *outline = null_outline;

    प्रत्यागम FT_Err_Ok;
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( शून्य )
  FT_Outline_Get_CBox( ध्रुव FT_Outline*  outline,
                       FT_BBox           *acbox )
  {
    FT_Pos  xMin, yMin, xMax, yMax;


    यदि ( outline && acbox )
    {
      यदि ( outline->n_points == 0 )
      {
        xMin = 0;
        yMin = 0;
        xMax = 0;
        yMax = 0;
      }
      अन्यथा
      {
        FT_सदिशम्*  vec   = outline->points;
        FT_सदिशम्*  सीमा = vec + outline->n_points;


        xMin = xMax = vec->x;
        yMin = yMax = vec->y;
        vec++;

        प्रति ( ; vec < सीमा; vec++ )
        {
          FT_Pos  x, y;


          x = vec->x;
          यदि ( x < xMin ) xMin = x;
          यदि ( x > xMax ) xMax = x;

          y = vec->y;
          यदि ( y < yMin ) yMin = y;
          यदि ( y > yMax ) yMax = y;
        }
      }
      acbox->xMin = xMin;
      acbox->xMax = xMax;
      acbox->yMin = yMin;
      acbox->yMax = yMax;
    }
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( शून्य )
  FT_Outline_Translate( ध्रुव FT_Outline*  outline,
                        FT_Pos             x_विस्थापनम्,
                        FT_Pos             y_विस्थापनम् )
  {
    FT_UShort   n;
    FT_सदिशम्*  vec;


    यदि ( !outline )
      प्रत्यागम;

    vec = outline->points;

    प्रति ( n = 0; n < outline->n_points; n++ )
    {
      vec->x = सङ्कलय_LONG( vec->x, x_विस्थापनम् );
      vec->y = सङ्कलय_LONG( vec->y, y_विस्थापनम् );
      vec++;
    }
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( शून्य )
  FT_Outline_Reverse( FT_Outline*  outline )
  {
    FT_UShort  n;
    FT_Int     first, चरमः;


    यदि ( !outline )
      प्रत्यागम;

    चरमः = -1;
    प्रति ( n = 0; n < outline->n_contours; n++ )
    {
      first = चरमः + 1;
      चरमः  = outline->contours[n];

      /* Keep the first point unless it is a cubic control.  In that */
      /* case, reverse the entire contour to preserve control pairs, */
      /* including contours with no explicit on-curve points.        */
      यदि ( FT_वक्रम्_चिह्नकम्( outline->चिह्नकानि[first] ) != FT_वक्रम्_चिह्नकम्_CUBIC )
        first++;

      /* reverse point table */
      {
        FT_सदिशम्*  p = outline->points + first;
        FT_सदिशम्*  q = outline->points + चरमः;
        FT_सदिशम्   swap;


        यावत् ( p < q )
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
        FT_Byte*  p = outline->चिह्नकानि + first;
        FT_Byte*  q = outline->चिह्नकानि + चरमः;


        यावत् ( p < q )
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
    FT_List_ग्रन्थिः  ग्रन्थिः;
    FT_BBox      cbox;


    यदि ( !library )
      प्रत्यागम FT_THROW( Invalid_Library_Handle );

    यदि ( !outline )
      प्रत्यागम FT_THROW( Invalid_Outline );

    यदि ( !params )
      प्रत्यागम FT_THROW( Invalid_तर्कः );

    FT_Outline_Get_CBox( outline, &cbox );
    यदि ( cbox.xMin < -0x1000000L || cbox.yMin < -0x1000000L ||
         cbox.xMax >  0x1000000L || cbox.yMax >  0x1000000L )
      प्रत्यागम FT_THROW( Invalid_Outline );

    renderer = library->cur_renderer;
    ग्रन्थिः     = library->renderers.शीर्ष;

    params->source = (शून्य*)outline;

    /* preset clip_box for direct mode */
    यदि ( params->flags & FT_RASTER_FLAG_प्रत्यक्षम्    &&
         !( params->flags & FT_RASTER_FLAG_CLIP ) )
    {
      params->clip_box.xMin = cbox.xMin >> 6;
      params->clip_box.yMin = cbox.yMin >> 6;
      params->clip_box.xMax = ( cbox.xMax + 63 ) >> 6;
      params->clip_box.yMax = ( cbox.yMax + 63 ) >> 6;
    }

    error = FT_ERR( Cannot_Render_Glyph );
    यावत् ( renderer )
    {
      error = renderer->raster_render( renderer->raster, params );
      यदि ( !error || FT_ERR_NEQ( error, Cannot_Render_Glyph ) )
        भङ्ग;

      /* FT_Err_Cannot_Render_Glyph is returned if the render mode   */
      /* is unsupported by the current renderer for this glyph image */
      /* format                                                      */

      /* now, look for another renderer that supports the same */
      /* format                                                */
      renderer = FT_Lookup_Renderer( library, FT_GLYPH_FORMAT_OUTLINE,
                                     &ग्रन्थिः );
    }

    प्रत्यागम error;
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Outline_Get_Bitmap( FT_Library        library,
                         FT_Outline*       outline,
                         ध्रुव FT_Bitmap  *abitmap )
  {
    FT_Raster_Params  params;


    यदि ( !abitmap )
      प्रत्यागम FT_THROW( Invalid_तर्कः );

    /* other checks are delayed to `FT_Outline_Render' */

    params.target = abitmap;
    params.flags  = 0;

    यदि ( abitmap->pixel_बहुलकम् == FT_PIXEL_बहुलकम्_GRAY  ||
         abitmap->pixel_बहुलकम् == FT_PIXEL_बहुलकम्_LCD   ||
         abitmap->pixel_बहुलकम् == FT_PIXEL_बहुलकम्_LCD_V )
      params.flags |= FT_RASTER_FLAG_AA;

    प्रत्यागम FT_Outline_Render( library, outline, &params );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( शून्य )
  FT_सदिशम्_Transform( FT_सदिशम्*        सदिशम्,
                       ध्रुव FT_Matrix*  matrix )
  {
    FT_Pos  xz, yz;


    यदि ( !सदिशम् || !matrix )
      प्रत्यागम;

    xz = FT_MulFix( सदिशम्->x, matrix->xx ) +
         FT_MulFix( सदिशम्->y, matrix->xy );

    yz = FT_MulFix( सदिशम्->x, matrix->yx ) +
         FT_MulFix( सदिशम्->y, matrix->yy );

    सदिशम्->x = xz;
    सदिशम्->y = yz;
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( शून्य )
  FT_Outline_Transform( ध्रुव FT_Outline*  outline,
                        ध्रुव FT_Matrix*   matrix )
  {
    FT_सदिशम्*  vec;
    FT_सदिशम्*  सीमा;


    यदि ( !outline || !matrix || !outline->points )
      प्रत्यागम;

    vec   = outline->points;
    सीमा = vec + outline->n_points;

    प्रति ( ; vec < सीमा; vec++ )
      FT_सदिशम्_Transform( vec, matrix );
  }


#यदि 0

#परिभाषय FT_OUTLINE_GET_CONTOUR( outline, c, first, चरमः )  \
  कुरु                                                       \
  {                                                        \
    (first) = ( c > 0 ) ? (outline)->points +              \
                            (outline)->contours[c - 1] + 1 \
                        : (outline)->points;               \
    (चरमः) = (outline)->points + (outline)->contours[c];   \
  } यावत् ( 0 )


  /* Is a point in some contour?                     */
  /*                                                 */
  /* We treat every point of the contour as if it    */
  /* it were ON.  That is, we allow false positives, */
  /* but disallow false negatives.  (XXX really?)    */
  स्थिर FT_Bool
  ft_contour_has( FT_Outline*  outline,
                  FT_Short     c,
                  FT_सदिशम्*   point )
  {
    FT_सदिशम्*  first;
    FT_सदिशम्*  चरमः;
    FT_सदिशम्*  a;
    FT_सदिशम्*  b;
    FT_UInt     n = 0;


    FT_OUTLINE_GET_CONTOUR( outline, c, first, चरमः );

    प्रति ( a = first; a <= चरमः; a++ )
    {
      FT_Pos  x;
      FT_Int  intersect;


      b = ( a == चरमः ) ? first : a + 1;

      intersect = ( a->y - point->y ) ^ ( b->y - point->y );

      /* a and b are on the same side */
      यदि ( intersect >= 0 )
      {
        यदि ( intersect == 0 && a->y == point->y )
        {
          यदि ( ( a->x <= point->x && b->x >= point->x ) ||
               ( a->x >= point->x && b->x <= point->x ) )
            प्रत्यागम 1;
        }

        अनुवर्त;
      }

      x = a->x + ( b->x - a->x ) * (point->y - a->y ) / ( b->y - a->y );

      यदि ( x < point->x )
        n++;
      अन्यथा यदि ( x == point->x )
        प्रत्यागम 1;
    }

    प्रत्यागम n & 1;
  }


  स्थिर FT_Bool
  ft_contour_enclosed( FT_Outline*  outline,
                       FT_UShort    c )
  {
    FT_सदिशम्*  first;
    FT_सदिशम्*  चरमः;
    FT_Short    i;


    FT_OUTLINE_GET_CONTOUR( outline, c, first, चरमः );

    प्रति ( i = 0; i < outline->n_contours; i++ )
    {
      यदि ( i != c && ft_contour_has( outline, i, first ) )
      {
        FT_सदिशम्*  pt;


        प्रति ( pt = first + 1; pt <= चरमः; pt++ )
          यदि ( !ft_contour_has( outline, i, pt ) )
            प्रत्यागम 0;

        प्रत्यागम 1;
      }
    }

    प्रत्यागम 0;
  }


  /* This version differs from the public one in that each */
  /* part (contour not enclosed in another contour) of the */
  /* outline is checked for orientation.  This is          */
  /* necessary for some buggy CJK fonts.                   */
  स्थिर FT_Orientation
  ft_outline_get_orientation( FT_Outline*  outline )
  {
    FT_Short        i;
    FT_सदिशम्*      first;
    FT_सदिशम्*      चरमः;
    FT_Orientation  orient = FT_ORIENTATION_NONE;


    first = outline->points;
    प्रति ( i = 0; i < outline->n_contours; i++, first = चरमः + 1 )
    {
      FT_सदिशम्*  point;
      FT_सदिशम्*  xmin_point;
      FT_Pos      xmin;


      चरमः = outline->points + outline->contours[i];

      /* skip degenerate contours */
      यदि ( चरमः < first + 2 )
        अनुवर्त;

      यदि ( ft_contour_enclosed( outline, i ) )
        अनुवर्त;

      xmin       = first->x;
      xmin_point = first;

      प्रति ( point = first + 1; point <= चरमः; point++ )
      {
        यदि ( point->x < xmin )
        {
          xmin       = point->x;
          xmin_point = point;
        }
      }

      /* check the orientation of the contour */
      {
        FT_सदिशम्*      पूर्व;
        FT_सदिशम्*      अग्र;
        FT_Orientation  o;


        पूर्व = ( xmin_point == first ) ? चरमः : xmin_point - 1;
        अग्र = ( xmin_point == चरमः ) ? first : xmin_point + 1;

        यदि ( FT_Atan2( पूर्व->x - xmin_point->x, पूर्व->y - xmin_point->y ) >
             FT_Atan2( अग्र->x - xmin_point->x, अग्र->y - xmin_point->y ) )
          o = FT_ORIENTATION_POSTSCRIPT;
        अन्यथा
          o = FT_ORIENTATION_TRUETYPE;

        यदि ( orient == FT_ORIENTATION_NONE )
          orient = o;
        अन्यथा यदि ( orient != o )
          प्रत्यागम FT_ORIENTATION_NONE;
      }
    }

    प्रत्यागम orient;
  }

#यदि_अन्तः /* 0 */


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Outline_Embolden( FT_Outline*  outline,
                       FT_Pos       strength )
  {
    प्रत्यागम FT_Outline_EmboldenXY( outline, strength, strength );
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Outline_EmboldenXY( FT_Outline*  outline,
                         FT_Pos       xstrength,
                         FT_Pos       ystrength )
  {
    FT_सदिशम्*      points;
    FT_Int          c, first, चरमः;
    FT_Orientation  orientation;


    यदि ( !outline )
      प्रत्यागम FT_THROW( Invalid_Outline );

    xstrength /= 2;
    ystrength /= 2;
    यदि ( xstrength == 0 && ystrength == 0 )
      प्रत्यागम FT_Err_Ok;

    orientation = FT_Outline_Get_Orientation( outline );
    यदि ( orientation == FT_ORIENTATION_NONE )
    {
      यदि ( outline->n_contours )
        प्रत्यागम FT_THROW( Invalid_तर्कः );
      अन्यथा
        प्रत्यागम FT_Err_Ok;
    }

    points = outline->points;

    चरमः = -1;
    प्रति ( c = 0; c < outline->n_contours; c++ )
    {
      FT_सदिशम्  अन्तर्, बहिर्गमनम्, anchor, सरणम्;
      FT_Fixed   l_अन्तर्, l_बहिर्गमनम्, l_anchor = 0, l, q, d;
      FT_Int     i, j, k;


      first = चरमः + 1;
      चरमः  = outline->contours[c];
      l_अन्तर्  = 0;

      /* pacify compiler */
      अन्तर्.x = अन्तर्.y = anchor.x = anchor.y = 0;

      /* Counter j cycles though the points; counter i advances only  */
      /* when points are moved; anchor k marks the first moved point. */
      प्रति ( i = चरमः, j = first, k = -1;
            j != i && i != k;
            j = j < चरमः ? j + 1 : first )
      {
        यदि ( j != k )
        {
          बहिर्गमनम्.x = points[j].x - points[i].x;
          बहिर्गमनम्.y = points[j].y - points[i].y;
          l_बहिर्गमनम् = (FT_Fixed)FT_सदिशम्_Norm_Len( &बहिर्गमनम् );

          यदि ( l_बहिर्गमनम् == 0 )
            अनुवर्त;
        }
        अन्यथा
        {
          बहिर्गमनम्   = anchor;
          l_बहिर्गमनम् = l_anchor;
        }

        यदि ( l_अन्तर् != 0 )
        {
          यदि ( k < 0 )
          {
            k        = i;
            anchor   = अन्तर्;
            l_anchor = l_अन्तर्;
          }

          d = FT_MulFix( अन्तर्.x, बहिर्गमनम्.x ) + FT_MulFix( अन्तर्.y, बहिर्गमनम्.y );

          /* shift only if turn is less than ~160 degrees */
          यदि ( d > -0xF000L )
          {
            d = d + 0x10000L;

            /* shift components along lateral bisector in proper orientation */
            सरणम्.x = अन्तर्.y + बहिर्गमनम्.y;
            सरणम्.y = अन्तर्.x + बहिर्गमनम्.x;

            यदि ( orientation == FT_ORIENTATION_TRUETYPE )
              सरणम्.x = -सरणम्.x;
            अन्यथा
              सरणम्.y = -सरणम्.y;

            /* restrict shift magnitude to better handle collapsing segments */
            q = FT_MulFix( बहिर्गमनम्.x, अन्तर्.y ) - FT_MulFix( बहिर्गमनम्.y, अन्तर्.x );
            यदि ( orientation == FT_ORIENTATION_TRUETYPE )
              q = -q;

            l = FT_MIN( l_अन्तर्, l_बहिर्गमनम् );

            /* non-strict inequalities avoid divide-by-zero when q == l == 0 */
            यदि ( FT_MulFix( xstrength, q ) <= FT_MulFix( l, d ) )
              सरणम्.x = FT_MulDiv( सरणम्.x, xstrength, d );
            अन्यथा
              सरणम्.x = FT_MulDiv( सरणम्.x, l, q );


            यदि ( FT_MulFix( ystrength, q ) <= FT_MulFix( l, d ) )
              सरणम्.y = FT_MulDiv( सरणम्.y, ystrength, d );
            अन्यथा
              सरणम्.y = FT_MulDiv( सरणम्.y, l, q );
          }
          अन्यथा
            सरणम्.x = सरणम्.y = 0;

          प्रति ( ;
                i != j;
                i = i < चरमः ? i + 1 : first )
          {
            points[i].x += xstrength + सरणम्.x;
            points[i].y += ystrength + सरणम्.y;
          }
        }
        अन्यथा
          i = j;

        अन्तर्   = बहिर्गमनम्;
        l_अन्तर् = l_बहिर्गमनम्;
      }
    }

    प्रत्यागम FT_Err_Ok;
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Orientation )
  FT_Outline_Get_Orientation( FT_Outline*  outline )
  {
    FT_सदिशम्*  points;
    FT_Int      c, n, first, चरमः;

#यदि_परिभाषितम् FT_INT64
    FT_Int64    area = 0;
#अन्यथा
    FT_BBox     cbox = { 0, 0, 0, 0 };
    FT_Int      xshift, yshift;
    FT_सदिशम्   v_पूर्व, v_cur;
    FT_Pos      area = 0;
#यदि_अन्तः


    यदि ( !outline || outline->n_points <= 0 )
      प्रत्यागम FT_ORIENTATION_TRUETYPE;

    /* We use the nonzero winding rule to find the orientation.       */
    /* Since glyph outlines behave much more `regular' than arbitrary */
    /* cubic or quadratic curves, this test deals with the polygon    */
    /* only that is spanned up by the control points.                 */

    points = outline->points;

#यदि_परिभाषितम् FT_INT64

    /* Single-pass shoelace.  The 64-bit accumulator removes the need   */
    /* for the coordinate shift pre-pass (and its `FT_Outline_Get_CBox` */
    /* traversal) that the 32-bit variant below uses.                   */
    चरमः = -1;
    प्रति ( c = 0; c < outline->n_contours; c++ )
    {
      FT_Pos  पूर्व_x, पूर्व_y;


      first = चरमः + 1;
      चरमः  = outline->contours[c];

      पूर्व_x = points[चरमः].x;
      पूर्व_y = points[चरमः].y;

      प्रति ( n = first; n <= चरमः; n++ )
      {
        FT_Pos  cur_x = points[n].x;
        FT_Pos  cur_y = points[n].y;


        area = सङ्कलय_INT64( area,
                          MUL_INT64( SUB_INT64( cur_y, पूर्व_y ),
                                     सङ्कलय_INT64( cur_x, पूर्व_x ) ) );

        पूर्व_x = cur_x;
        पूर्व_y = cur_y;
      }
    }

#अन्यथा /* !FT_INT64 */

    FT_Outline_Get_CBox( outline, &cbox );

    /* Handle collapsed outlines to avoid undefined FT_MSB. */
    यदि ( cbox.xMin == cbox.xMax || cbox.yMin == cbox.yMax )
      प्रत्यागम FT_ORIENTATION_NONE;

    /* Reject values large outlines. */
    यदि ( cbox.xMin < -0x1000000L || cbox.yMin < -0x1000000L ||
         cbox.xMax >  0x1000000L || cbox.yMax >  0x1000000L )
      प्रत्यागम FT_ORIENTATION_NONE;

    xshift = FT_MSB( (FT_UInt32)( FT_ABS( cbox.xMax ) |
                                  FT_ABS( cbox.xMin ) ) ) - 14;
    xshift = FT_MAX( xshift, 0 );

    yshift = FT_MSB( (FT_UInt32)( cbox.yMax - cbox.yMin ) ) - 14;
    yshift = FT_MAX( yshift, 0 );

    चरमः = -1;
    प्रति ( c = 0; c < outline->n_contours; c++ )
    {
      first = चरमः + 1;
      चरमः  = outline->contours[c];

      v_पूर्व.x = points[चरमः].x >> xshift;
      v_पूर्व.y = points[चरमः].y >> yshift;

      प्रति ( n = first; n <= चरमः; n++ )
      {
        v_cur.x = points[n].x >> xshift;
        v_cur.y = points[n].y >> yshift;

        area = सङ्कलय_LONG( area,
                         MUL_LONG( v_cur.y - v_पूर्व.y,
                                   v_cur.x + v_पूर्व.x ) );

        v_पूर्व = v_cur;
      }
    }

#यदि_अन्तः /* FT_INT64 */

    यदि ( area > 0 )
      प्रत्यागम FT_ORIENTATION_POSTSCRIPT;
    अन्यथा यदि ( area < 0 )
      प्रत्यागम FT_ORIENTATION_TRUETYPE;
    अन्यथा
      प्रत्यागम FT_ORIENTATION_NONE;
  }


/*अन्तः*/
