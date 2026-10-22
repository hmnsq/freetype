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
#لغو_تعریف  FT_COMPONENT
#تعریف_کن FT_COMPONENT  outline


  ساكن
  ثابت FT_Outline  null_outline = { 0, 0, عدم, عدم, عدم, 0 };


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Outline_Decompose( FT_Outline*              outline,
                        ثابت FT_Outline_Funcs*  func_interface,
                        فراغ*                    user )
  {
#لغو_تعریف  SCALED
#تعریف_کن SCALED( x )  ( (x) * ( 1L << لغزش ) - delta )

    FT_وکتور   v_آخرین;
    FT_وکتور   v_control;
    FT_وکتور   v_شروع;

    FT_وکتور*  نقطه;
    FT_وکتور*  حد;
    FT_Byte*    برچسبها;

    FT_Error    error;

    FT_Int   n;         /* index of contour in outline     */
    FT_Int   اولی;     /* index of first point in contour */
    FT_Int   آخرین;      /* index of last point in contour  */

    FT_Int   برچسب;       /* current point's state           */

    FT_Int   لغزش;
    FT_Pos   delta;


    إذا ( !outline )
      أرجع FT_THROW( Invalid_Outline );

    إذا ( !func_interface )
      أرجع FT_THROW( Invalid_آرگومان );

    لغزش = func_interface->لغزش;
    delta = func_interface->delta;

    آخرین = -1;
    لكل ( n = 0; n < outline->n_contours; n++ )
    {
      FT_TRACE5(( "FT_Outline_Decompose: Contour %d\n", n ));

      اولی = آخرین + 1;
      آخرین  = outline->contours[n];
      إذا ( آخرین < اولی )
        اذهب Invalid_Outline;

      حد = outline->points + آخرین;

      v_شروع   = outline->points[اولی];
      v_شروع.x = SCALED( v_شروع.x );
      v_شروع.y = SCALED( v_شروع.y );

      v_آخرین   = outline->points[آخرین];
      v_آخرین.x = SCALED( v_آخرین.x );
      v_آخرین.y = SCALED( v_آخرین.y );

      v_control = v_شروع;

      نقطه = outline->points + اولی;
      برچسبها  = outline->برچسبها   + اولی;
      برچسب   = FT_منحنی_برچسب( برچسبها[0] );

      /* check first point to determine origin */
      إذا ( برچسب != FT_منحنی_برچسب_ON )
      {
        FT_Int  آخرین_برچسب = FT_منحنی_برچسب( outline->برچسبها[آخرین] );


        /* first point is a control point.  Yes, this happens. */
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

          /* if both first and last points are controls,   */
          /* start at their middle and record its position */
          /* for closure                                   */
          v_شروع.x = ( v_شروع.x + v_آخرین.x ) / 2;
          v_شروع.y = ( v_شروع.y + v_آخرین.y ) / 2;

          /* v_last = v_start; */
        }
        نقطه--;
        برچسبها--;
      }

      FT_TRACE5(( "  move to (%.2f, %.2f)\n",
                  (مزدوج)v_شروع.x / 64, (مزدوج)v_شروع.y / 64 ));
      error = func_interface->move_مقصد( &v_شروع, user );
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


            vec.x = SCALED( نقطه->x );
            vec.y = SCALED( نقطه->y );

            FT_TRACE5(( "  line to (%.2f, %.2f)\n",
                        (مزدوج)vec.x / 64, (مزدوج)vec.y / 64 ));
            error = func_interface->سطر_مقصد( &vec, user );
            إذا ( error )
              اذهب Exit;
            استمر;
          }

        حالة FT_منحنی_برچسب_CONIC:  /* consume conic arcs */
          v_control.x = SCALED( نقطه->x );
          v_control.y = SCALED( نقطه->y );

        Do_Conic:
          إذا ( نقطه < حد )
          {
            FT_وکتور  vec;
            FT_وکتور  v_میانه;


            نقطه++;
            برچسبها++;
            برچسب = FT_منحنی_برچسب( برچسبها[0] );

            vec.x = SCALED( نقطه->x );
            vec.y = SCALED( نقطه->y );

            إذا ( برچسب == FT_منحنی_برچسب_ON )
            {
              FT_TRACE5(( "  conic to (%.2f, %.2f)"
                          " with control (%.2f, %.2f)\n",
                          (مزدوج)vec.x / 64,
                          (مزدوج)vec.y / 64,
                          (مزدوج)v_control.x / 64,
                          (مزدوج)v_control.y / 64 ));
              error = func_interface->conic_مقصد( &v_control, &vec, user );
              إذا ( error )
                اذهب Exit;
              استمر;
            }

            إذا ( برچسب != FT_منحنی_برچسب_CONIC )
              اذهب Invalid_Outline;

            v_میانه.x = ( v_control.x + vec.x ) / 2;
            v_میانه.y = ( v_control.y + vec.y ) / 2;

            FT_TRACE5(( "  conic to (%.2f, %.2f)"
                        " with control (%.2f, %.2f)\n",
                        (مزدوج)v_میانه.x / 64,
                        (مزدوج)v_میانه.y / 64,
                        (مزدوج)v_control.x / 64,
                        (مزدوج)v_control.y / 64 ));
            error = func_interface->conic_مقصد( &v_control, &v_میانه, user );
            إذا ( error )
              اذهب Exit;

            v_control = vec;
            اذهب Do_Conic;
          }

          FT_TRACE5(( "  conic to (%.2f, %.2f)"
                      " with control (%.2f, %.2f)\n",
                      (مزدوج)v_شروع.x / 64,
                      (مزدوج)v_شروع.y / 64,
                      (مزدوج)v_control.x / 64,
                      (مزدوج)v_control.y / 64 ));
          error = func_interface->conic_مقصد( &v_control, &v_شروع, user );
          اذهب Close;

        افتراضي:  /* FT_CURVE_TAG_CUBIC */
          {
            FT_وکتور  vec1, vec2;


            إذا ( نقطه + 1 > حد                             ||
                 FT_منحنی_برچسب( برچسبها[1] ) != FT_منحنی_برچسب_CUBIC )
              اذهب Invalid_Outline;

            نقطه += 2;
            برچسبها  += 2;

            vec1.x = SCALED( نقطه[-2].x );
            vec1.y = SCALED( نقطه[-2].y );

            vec2.x = SCALED( نقطه[-1].x );
            vec2.y = SCALED( نقطه[-1].y );

            إذا ( نقطه <= حد )
            {
              FT_وکتور  vec;
              FT_وکتور  v_میانه;


              برچسب = FT_منحنی_برچسب( برچسبها[0] );

              vec.x = SCALED( نقطه->x );
              vec.y = SCALED( نقطه->y );

              إذا ( برچسب == FT_منحنی_برچسب_CUBIC )
              {
                v_میانه.x = ( vec2.x + vec.x ) / 2;
                v_میانه.y = ( vec2.y + vec.y ) / 2;

                vec = v_میانه;
              }
              وإلا إذا ( برچسب != FT_منحنی_برچسب_ON )
                اذهب Invalid_Outline;

              FT_TRACE5(( "  cubic to (%.2f, %.2f)"
                          " with controls (%.2f, %.2f) and (%.2f, %.2f)\n",
                          (مزدوج)vec.x / 64,
                          (مزدوج)vec.y / 64,
                          (مزدوج)vec1.x / 64,
                          (مزدوج)vec1.y / 64,
                          (مزدوج)vec2.x / 64,
                          (مزدوج)vec2.y / 64 ));
              error = func_interface->cubic_مقصد( &vec1, &vec2, &vec, user );
              إذا ( error )
                اذهب Exit;

              إذا ( برچسب == FT_منحنی_برچسب_CUBIC )
              {
                نقطه--;
                برچسبها--;
              }
              استمر;
            }

            FT_TRACE5(( "  cubic to (%.2f, %.2f)"
                        " with controls (%.2f, %.2f) and (%.2f, %.2f)\n",
                        (مزدوج)v_شروع.x / 64,
                        (مزدوج)v_شروع.y / 64,
                        (مزدوج)vec1.x / 64,
                        (مزدوج)vec1.y / 64,
                        (مزدوج)vec2.x / 64,
                        (مزدوج)vec2.y / 64 ));
            error = func_interface->cubic_مقصد( &vec1, &vec2, &v_شروع, user );
            اذهب Close;
          }
        }
      }

      /* close the contour with a line segment */
      FT_TRACE5(( "  line to (%.2f, %.2f)\n",
                  (مزدوج)v_شروع.x / 64, (مزدوج)v_شروع.y / 64 ));
      error = func_interface->سطر_مقصد( &v_شروع, user );

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
    FT_حافظه  حافظه;


    إذا ( !library )
      أرجع FT_THROW( Invalid_Library_Handle );

    حافظه = library->حافظه;

    إذا ( !anoutline || !حافظه )
      أرجع FT_THROW( Invalid_آرگومان );

    *anoutline = null_outline;

    إذا ( رقم_Contours < 0                  ||
         (FT_UInt)رقم_Contours > رقم_Points )
      أرجع FT_THROW( Invalid_آرگومان );

    إذا ( رقم_Points > FT_OUTLINE_POINTS_MAX )
      أرجع FT_THROW( Array_Too_Large );

    إذا ( FT_NEW_ARRAY( anoutline->points,   رقم_Points   ) ||
         FT_NEW_ARRAY( anoutline->برچسبها,     رقم_Points   ) ||
         FT_NEW_ARRAY( anoutline->contours, رقم_Contours ) )
      اذهب Fail;

    anoutline->n_points    = (FT_UShort)رقم_Points;
    anoutline->n_contours  = (FT_UShort)رقم_Contours;
    anoutline->flags      |= FT_OUTLINE_OWNER;

    أرجع FT_Err_Ok;

  Fail:
    anoutline->flags |= FT_OUTLINE_OWNER;
    FT_Outline_انجامشده( library, anoutline );

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
      FT_Int  end0, انتها;
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
        انتها = outline->contours[n];

        /* note that we don't accept empty contours */
        إذا ( انتها <= end0 || انتها >= n_points )
          اذهب Bad;

        end0 = انتها;
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
  FT_Outline_Copy( ثابت FT_Outline*  منبع,
                   FT_Outline        *هدف )
  {
    FT_Int  is_owner;


    إذا ( !منبع || !هدف )
      أرجع FT_THROW( Invalid_Outline );

    إذا ( منبع->n_points   != هدف->n_points   ||
         منبع->n_contours != هدف->n_contours )
      أرجع FT_THROW( Invalid_آرگومان );

    إذا ( منبع == هدف )
      أرجع FT_Err_Ok;

    إذا ( منبع->n_points )
    {
      FT_ARRAY_COPY( هدف->points, منبع->points, منبع->n_points );
      FT_ARRAY_COPY( هدف->برچسبها,   منبع->برچسبها,   منبع->n_points );
    }

    إذا ( منبع->n_contours )
      FT_ARRAY_COPY( هدف->contours, منبع->contours, منبع->n_contours );

    /* copy all flags, except the `FT_OUTLINE_OWNER' one */
    is_owner      = هدف->flags & FT_OUTLINE_OWNER;
    هدف->flags = منبع->flags;

    هدف->flags &= ~FT_OUTLINE_OWNER;
    هدف->flags |= is_owner;

    أرجع FT_Err_Ok;
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Outline_انجامشده( FT_Library   library,
                   FT_Outline*  outline )
  {
    FT_حافظه  حافظه;


    إذا ( !library )
      أرجع FT_THROW( Invalid_Library_Handle );

    إذا ( !outline )
      أرجع FT_THROW( Invalid_Outline );

    حافظه = library->حافظه;

    إذا ( !حافظه )
      أرجع FT_THROW( Invalid_آرگومان );

    إذا ( outline->flags & FT_OUTLINE_OWNER )
    {
      FT_FREE( outline->points   );
      FT_FREE( outline->برچسبها     );
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
        FT_وکتور*  vec   = outline->points;
        FT_وکتور*  حد = vec + outline->n_points;


        xMin = xMax = vec->x;
        yMin = yMax = vec->y;
        vec++;

        لكل ( ; vec < حد; vec++ )
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
                        FT_Pos             x_جابه_جایی,
                        FT_Pos             y_جابه_جایی )
  {
    FT_UShort   n;
    FT_وکتور*  vec;


    إذا ( !outline )
      أرجع;

    vec = outline->points;

    لكل ( n = 0; n < outline->n_points; n++ )
    {
      vec->x = جمع_کن_LONG( vec->x, x_جابه_جایی );
      vec->y = جمع_کن_LONG( vec->y, y_جابه_جایی );
      vec++;
    }
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( فراغ )
  FT_Outline_معکوس( FT_Outline*  outline )
  {
    FT_UShort  n;
    FT_Int     اولی, آخرین;


    إذا ( !outline )
      أرجع;

    آخرین = -1;
    لكل ( n = 0; n < outline->n_contours; n++ )
    {
      اولی = آخرین + 1;
      آخرین  = outline->contours[n];

      /* Keep the first point unless it is a cubic control.  In that */
      /* case, reverse the entire contour to preserve control pairs, */
      /* including contours with no explicit on-curve points.        */
      إذا ( FT_منحنی_برچسب( outline->برچسبها[اولی] ) != FT_منحنی_برچسب_CUBIC )
        اولی++;

      /* reverse point table */
      {
        FT_وکتور*  p = outline->points + اولی;
        FT_وکتور*  q = outline->points + آخرین;
        FT_وکتور   جابجا;


        طالما ( p < q )
        {
          جابجا = *p;
          *p   = *q;
          *q   = جابجا;
          p++;
          q--;
        }
      }

      /* reverse tags table */
      {
        FT_Byte*  p = outline->برچسبها + اولی;
        FT_Byte*  q = outline->برچسبها + آخرین;


        طالما ( p < q )
        {
          FT_Byte  جابجا;


          جابجا = *p;
          *p   = *q;
          *q   = جابجا;
          p++;
          q--;
        }
      }
    }

    outline->flags ^= FT_OUTLINE_معکوس_FILL;
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Outline_Render( FT_Library         library,
                     FT_Outline*        outline,
                     FT_Raster_Params*  params )
  {
    FT_Error     error;
    FT_Renderer  renderer;
    FT_List_گره  گره;
    FT_BBox      cbox;


    إذا ( !library )
      أرجع FT_THROW( Invalid_Library_Handle );

    إذا ( !outline )
      أرجع FT_THROW( Invalid_Outline );

    إذا ( !params )
      أرجع FT_THROW( Invalid_آرگومان );

    FT_Outline_Get_CBox( outline, &cbox );
    إذا ( cbox.xMin < -0x1000000L || cbox.yMin < -0x1000000L ||
         cbox.xMax >  0x1000000L || cbox.yMax >  0x1000000L )
      أرجع FT_THROW( Invalid_Outline );

    renderer = library->cur_renderer;
    گره     = library->renderers.سر;

    params->منبع = (فراغ*)outline;

    /* preset clip_box for direct mode */
    إذا ( params->flags & FT_RASTER_FLAG_مستقیم    &&
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
                                     &گره );
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
      أرجع FT_THROW( Invalid_آرگومان );

    /* other checks are delayed to `FT_Outline_Render' */

    params.هدف = abitmap;
    params.flags  = 0;

    إذا ( abitmap->pixel_نما == FT_PIXEL_نما_خاکستری  ||
         abitmap->pixel_نما == FT_PIXEL_نما_LCD   ||
         abitmap->pixel_نما == FT_PIXEL_نما_LCD_V )
      params.flags |= FT_RASTER_FLAG_AA;

    أرجع FT_Outline_Render( library, outline, &params );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( فراغ )
  FT_وکتور_Transform( FT_وکتور*        وکتور,
                       ثابت FT_ماتریس*  ماتریس )
  {
    FT_Pos  xz, yz;


    إذا ( !وکتور || !ماتریس )
      أرجع;

    xz = FT_MulFix( وکتور->x, ماتریس->xx ) +
         FT_MulFix( وکتور->y, ماتریس->xy );

    yz = FT_MulFix( وکتور->x, ماتریس->yx ) +
         FT_MulFix( وکتور->y, ماتریس->yy );

    وکتور->x = xz;
    وکتور->y = yz;
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( فراغ )
  FT_Outline_Transform( ثابت FT_Outline*  outline,
                        ثابت FT_ماتریس*   ماتریس )
  {
    FT_وکتور*  vec;
    FT_وکتور*  حد;


    إذا ( !outline || !ماتریس || !outline->points )
      أرجع;

    vec   = outline->points;
    حد = vec + outline->n_points;

    لكل ( ; vec < حد; vec++ )
      FT_وکتور_Transform( vec, ماتریس );
  }


#إذا 0

#تعریف_کن FT_OUTLINE_GET_CONTOUR( outline, c, اولی, آخرین )  \
  افعل                                                       \
  {                                                        \
    (اولی) = ( c > 0 ) ? (outline)->points +              \
                            (outline)->contours[c - 1] + 1 \
                        : (outline)->points;               \
    (آخرین) = (outline)->points + (outline)->contours[c];   \
  } طالما ( 0 )


  /* Is a point in some contour?                     */
  /*                                                 */
  /* We treat every point of the contour as if it    */
  /* it were ON.  That is, we allow false positives, */
  /* but disallow false negatives.  (XXX really?)    */
  ساكن FT_Bool
  ft_contour_has( FT_Outline*  outline,
                  FT_Short     c,
                  FT_وکتور*   نقطه )
  {
    FT_وکتور*  اولی;
    FT_وکتور*  آخرین;
    FT_وکتور*  a;
    FT_وکتور*  b;
    FT_UInt     n = 0;


    FT_OUTLINE_GET_CONTOUR( outline, c, اولی, آخرین );

    لكل ( a = اولی; a <= آخرین; a++ )
    {
      FT_Pos  x;
      FT_Int  intersect;


      b = ( a == آخرین ) ? اولی : a + 1;

      intersect = ( a->y - نقطه->y ) ^ ( b->y - نقطه->y );

      /* a and b are on the same side */
      إذا ( intersect >= 0 )
      {
        إذا ( intersect == 0 && a->y == نقطه->y )
        {
          إذا ( ( a->x <= نقطه->x && b->x >= نقطه->x ) ||
               ( a->x >= نقطه->x && b->x <= نقطه->x ) )
            أرجع 1;
        }

        استمر;
      }

      x = a->x + ( b->x - a->x ) * (نقطه->y - a->y ) / ( b->y - a->y );

      إذا ( x < نقطه->x )
        n++;
      وإلا إذا ( x == نقطه->x )
        أرجع 1;
    }

    أرجع n & 1;
  }


  ساكن FT_Bool
  ft_contour_enclosed( FT_Outline*  outline,
                       FT_UShort    c )
  {
    FT_وکتور*  اولی;
    FT_وکتور*  آخرین;
    FT_Short    i;


    FT_OUTLINE_GET_CONTOUR( outline, c, اولی, آخرین );

    لكل ( i = 0; i < outline->n_contours; i++ )
    {
      إذا ( i != c && ft_contour_has( outline, i, اولی ) )
      {
        FT_وکتور*  pt;


        لكل ( pt = اولی + 1; pt <= آخرین; pt++ )
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
    FT_وکتور*      اولی;
    FT_وکتور*      آخرین;
    FT_Orientation  orient = FT_ORIENTATION_NONE;


    اولی = outline->points;
    لكل ( i = 0; i < outline->n_contours; i++, اولی = آخرین + 1 )
    {
      FT_وکتور*  نقطه;
      FT_وکتور*  xmin_نقطه;
      FT_Pos      xmin;


      آخرین = outline->points + outline->contours[i];

      /* skip degenerate contours */
      إذا ( آخرین < اولی + 2 )
        استمر;

      إذا ( ft_contour_enclosed( outline, i ) )
        استمر;

      xmin       = اولی->x;
      xmin_نقطه = اولی;

      لكل ( نقطه = اولی + 1; نقطه <= آخرین; نقطه++ )
      {
        إذا ( نقطه->x < xmin )
        {
          xmin       = نقطه->x;
          xmin_نقطه = نقطه;
        }
      }

      /* check the orientation of the contour */
      {
        FT_وکتور*      prev;
        FT_وکتور*      بعدی;
        FT_Orientation  o;


        prev = ( xmin_نقطه == اولی ) ? آخرین : xmin_نقطه - 1;
        بعدی = ( xmin_نقطه == آخرین ) ? اولی : xmin_نقطه + 1;

        إذا ( FT_Atan2( prev->x - xmin_نقطه->x, prev->y - xmin_نقطه->y ) >
             FT_Atan2( بعدی->x - xmin_نقطه->x, بعدی->y - xmin_نقطه->y ) )
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

#پایان_اگر /* 0 */


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
    FT_وکتور*      points;
    FT_Int          c, اولی, آخرین;
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
        أرجع FT_THROW( Invalid_آرگومان );
      وإلا
        أرجع FT_Err_Ok;
    }

    points = outline->points;

    آخرین = -1;
    لكل ( c = 0; c < outline->n_contours; c++ )
    {
      FT_وکتور  درون, بیرون, anchor, لغزش;
      FT_Fixed   l_درون, l_بیرون, l_anchor = 0, l, q, d;
      FT_Int     i, j, k;


      اولی = آخرین + 1;
      آخرین  = outline->contours[c];
      l_درون  = 0;

      /* pacify compiler */
      درون.x = درون.y = anchor.x = anchor.y = 0;

      /* Counter j cycles though the points; counter i advances only  */
      /* when points are moved; anchor k marks the first moved point. */
      لكل ( i = آخرین, j = اولی, k = -1;
            j != i && i != k;
            j = j < آخرین ? j + 1 : اولی )
      {
        إذا ( j != k )
        {
          بیرون.x = points[j].x - points[i].x;
          بیرون.y = points[j].y - points[i].y;
          l_بیرون = (FT_Fixed)FT_وکتور_Norm_Len( &بیرون );

          إذا ( l_بیرون == 0 )
            استمر;
        }
        وإلا
        {
          بیرون   = anchor;
          l_بیرون = l_anchor;
        }

        إذا ( l_درون != 0 )
        {
          إذا ( k < 0 )
          {
            k        = i;
            anchor   = درون;
            l_anchor = l_درون;
          }

          d = FT_MulFix( درون.x, بیرون.x ) + FT_MulFix( درون.y, بیرون.y );

          /* shift only if turn is less than ~160 degrees */
          إذا ( d > -0xF000L )
          {
            d = d + 0x10000L;

            /* shift components along lateral bisector in proper orientation */
            لغزش.x = درون.y + بیرون.y;
            لغزش.y = درون.x + بیرون.x;

            إذا ( orientation == FT_ORIENTATION_TRUETYPE )
              لغزش.x = -لغزش.x;
            وإلا
              لغزش.y = -لغزش.y;

            /* restrict shift magnitude to better handle collapsing segments */
            q = FT_MulFix( بیرون.x, درون.y ) - FT_MulFix( بیرون.y, درون.x );
            إذا ( orientation == FT_ORIENTATION_TRUETYPE )
              q = -q;

            l = FT_MIN( l_درون, l_بیرون );

            /* non-strict inequalities avoid divide-by-zero when q == l == 0 */
            إذا ( FT_MulFix( xstrength, q ) <= FT_MulFix( l, d ) )
              لغزش.x = FT_MulDiv( لغزش.x, xstrength, d );
            وإلا
              لغزش.x = FT_MulDiv( لغزش.x, l, q );


            إذا ( FT_MulFix( ystrength, q ) <= FT_MulFix( l, d ) )
              لغزش.y = FT_MulDiv( لغزش.y, ystrength, d );
            وإلا
              لغزش.y = FT_MulDiv( لغزش.y, l, q );
          }
          وإلا
            لغزش.x = لغزش.y = 0;

          لكل ( ;
                i != j;
                i = i < آخرین ? i + 1 : اولی )
          {
            points[i].x += xstrength + لغزش.x;
            points[i].y += ystrength + لغزش.y;
          }
        }
        وإلا
          i = j;

        درون   = بیرون;
        l_درون = l_بیرون;
      }
    }

    أرجع FT_Err_Ok;
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Orientation )
  FT_Outline_Get_Orientation( FT_Outline*  outline )
  {
    FT_وکتور*  points;
    FT_Int      c, n, اولی, آخرین;

#اگر_تعریف_شده FT_INT64
    FT_Int64    area = 0;
#وإلا
    FT_BBox     cbox = { 0, 0, 0, 0 };
    FT_Int      xshift, yshift;
    FT_وکتور   v_prev, v_cur;
    FT_Pos      area = 0;
#پایان_اگر


    إذا ( !outline || outline->n_points <= 0 )
      أرجع FT_ORIENTATION_TRUETYPE;

    /* We use the nonzero winding rule to find the orientation.       */
    /* Since glyph outlines behave much more `regular' than arbitrary */
    /* cubic or quadratic curves, this test deals with the polygon    */
    /* only that is spanned up by the control points.                 */

    points = outline->points;

#اگر_تعریف_شده FT_INT64

    /* Single-pass shoelace.  The 64-bit accumulator removes the need   */
    /* for the coordinate shift pre-pass (and its `FT_Outline_Get_CBox` */
    /* traversal) that the 32-bit variant below uses.                   */
    آخرین = -1;
    لكل ( c = 0; c < outline->n_contours; c++ )
    {
      FT_Pos  prev_x, prev_y;


      اولی = آخرین + 1;
      آخرین  = outline->contours[c];

      prev_x = points[آخرین].x;
      prev_y = points[آخرین].y;

      لكل ( n = اولی; n <= آخرین; n++ )
      {
        FT_Pos  cur_x = points[n].x;
        FT_Pos  cur_y = points[n].y;


        area = جمع_کن_INT64( area,
                          MUL_INT64( SUB_INT64( cur_y, prev_y ),
                                     جمع_کن_INT64( cur_x, prev_x ) ) );

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

    آخرین = -1;
    لكل ( c = 0; c < outline->n_contours; c++ )
    {
      اولی = آخرین + 1;
      آخرین  = outline->contours[c];

      v_prev.x = points[آخرین].x >> xshift;
      v_prev.y = points[آخرین].y >> yshift;

      لكل ( n = اولی; n <= آخرین; n++ )
      {
        v_cur.x = points[n].x >> xshift;
        v_cur.y = points[n].y >> yshift;

        area = جمع_کن_LONG( area,
                         MUL_LONG( v_cur.y - v_prev.y,
                                   v_cur.x + v_prev.x ) );

        v_prev = v_cur;
      }
    }

#پایان_اگر /* FT_INT64 */

    إذا ( area > 0 )
      أرجع FT_ORIENTATION_POSTSCRIPT;
    وإلا إذا ( area < 0 )
      أرجع FT_ORIENTATION_TRUETYPE;
    وإلا
      أرجع FT_ORIENTATION_NONE;
  }


/*انتها*/
