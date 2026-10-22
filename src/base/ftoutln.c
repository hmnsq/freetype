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
#وصف_رد  FT_COMPONENT
#وصف_ڪريو FT_COMPONENT  outline


  ساكن
  ثابت FT_Outline  null_outline = { 0, 0, عدم, عدم, عدم, 0 };


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Outline_Decompose( FT_Outline*              outline,
                        ثابت FT_Outline_Funcs*  func_interface,
                        فراغ*                    user )
  {
#وصف_رد  SCALED
#وصف_ڪريو SCALED( x )  ( (x) * ( 1L << سرڪايو ) - delta )

    FT_ويڪٽر   v_آخري;
    FT_ويڪٽر   v_control;
    FT_ويڪٽر   v_شروعات;

    FT_ويڪٽر*  نقطو;
    FT_ويڪٽر*  حد_بندي;
    FT_Byte*    ٽيگ_فهرست;

    FT_Error    error;

    FT_Int   n;         /* index of contour in outline     */
    FT_Int   پهريون;     /* index of first point in contour */
    FT_Int   آخري;      /* index of last point in contour  */

    FT_Int   ٽيگ;       /* current point's state           */

    FT_Int   سرڪايو;
    FT_Pos   delta;


    إذا ( !outline )
      أرجع FT_THROW( Invalid_Outline );

    إذا ( !func_interface )
      أرجع FT_THROW( Invalid_دليل );

    سرڪايو = func_interface->سرڪايو;
    delta = func_interface->delta;

    آخري = -1;
    لكل ( n = 0; n < outline->n_contours; n++ )
    {
      FT_TRACE5(( "FT_Outline_Decompose: Contour %d\n", n ));

      پهريون = آخري + 1;
      آخري  = outline->contours[n];
      إذا ( آخري < پهريون )
        اذهب Invalid_Outline;

      حد_بندي = outline->points + آخري;

      v_شروعات   = outline->points[پهريون];
      v_شروعات.x = SCALED( v_شروعات.x );
      v_شروعات.y = SCALED( v_شروعات.y );

      v_آخري   = outline->points[آخري];
      v_آخري.x = SCALED( v_آخري.x );
      v_آخري.y = SCALED( v_آخري.y );

      v_control = v_شروعات;

      نقطو = outline->points + پهريون;
      ٽيگ_فهرست  = outline->ٽيگ_فهرست   + پهريون;
      ٽيگ   = FT_وڪر_ٽيگ( ٽيگ_فهرست[0] );

      /* check first point to determine origin */
      إذا ( ٽيگ != FT_وڪر_ٽيگ_ON )
      {
        FT_Int  آخري_ٽيگ = FT_وڪر_ٽيگ( outline->ٽيگ_فهرست[آخري] );


        /* first point is a control point.  Yes, this happens. */
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

          /* if both first and last points are controls,   */
          /* start at their middle and record its position */
          /* for closure                                   */
          v_شروعات.x = ( v_شروعات.x + v_آخري.x ) / 2;
          v_شروعات.y = ( v_شروعات.y + v_آخري.y ) / 2;

          /* v_last = v_start; */
        }
        نقطو--;
        ٽيگ_فهرست--;
      }

      FT_TRACE5(( "  move to (%.2f, %.2f)\n",
                  (مزدوج)v_شروعات.x / 64, (مزدوج)v_شروعات.y / 64 ));
      error = func_interface->move_منزل( &v_شروعات, user );
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


            vec.x = SCALED( نقطو->x );
            vec.y = SCALED( نقطو->y );

            FT_TRACE5(( "  line to (%.2f, %.2f)\n",
                        (مزدوج)vec.x / 64, (مزدوج)vec.y / 64 ));
            error = func_interface->سٽ_منزل( &vec, user );
            إذا ( error )
              اذهب Exit;
            استمر;
          }

        حالة FT_وڪر_ٽيگ_CONIC:  /* consume conic arcs */
          v_control.x = SCALED( نقطو->x );
          v_control.y = SCALED( نقطو->y );

        Do_Conic:
          إذا ( نقطو < حد_بندي )
          {
            FT_ويڪٽر  vec;
            FT_ويڪٽر  v_وچ;


            نقطو++;
            ٽيگ_فهرست++;
            ٽيگ = FT_وڪر_ٽيگ( ٽيگ_فهرست[0] );

            vec.x = SCALED( نقطو->x );
            vec.y = SCALED( نقطو->y );

            إذا ( ٽيگ == FT_وڪر_ٽيگ_ON )
            {
              FT_TRACE5(( "  conic to (%.2f, %.2f)"
                          " with control (%.2f, %.2f)\n",
                          (مزدوج)vec.x / 64,
                          (مزدوج)vec.y / 64,
                          (مزدوج)v_control.x / 64,
                          (مزدوج)v_control.y / 64 ));
              error = func_interface->conic_منزل( &v_control, &vec, user );
              إذا ( error )
                اذهب Exit;
              استمر;
            }

            إذا ( ٽيگ != FT_وڪر_ٽيگ_CONIC )
              اذهب Invalid_Outline;

            v_وچ.x = ( v_control.x + vec.x ) / 2;
            v_وچ.y = ( v_control.y + vec.y ) / 2;

            FT_TRACE5(( "  conic to (%.2f, %.2f)"
                        " with control (%.2f, %.2f)\n",
                        (مزدوج)v_وچ.x / 64,
                        (مزدوج)v_وچ.y / 64,
                        (مزدوج)v_control.x / 64,
                        (مزدوج)v_control.y / 64 ));
            error = func_interface->conic_منزل( &v_control, &v_وچ, user );
            إذا ( error )
              اذهب Exit;

            v_control = vec;
            اذهب Do_Conic;
          }

          FT_TRACE5(( "  conic to (%.2f, %.2f)"
                      " with control (%.2f, %.2f)\n",
                      (مزدوج)v_شروعات.x / 64,
                      (مزدوج)v_شروعات.y / 64,
                      (مزدوج)v_control.x / 64,
                      (مزدوج)v_control.y / 64 ));
          error = func_interface->conic_منزل( &v_control, &v_شروعات, user );
          اذهب Close;

        افتراضي:  /* FT_CURVE_TAG_CUBIC */
          {
            FT_ويڪٽر  vec1, vec2;


            إذا ( نقطو + 1 > حد_بندي                             ||
                 FT_وڪر_ٽيگ( ٽيگ_فهرست[1] ) != FT_وڪر_ٽيگ_CUBIC )
              اذهب Invalid_Outline;

            نقطو += 2;
            ٽيگ_فهرست  += 2;

            vec1.x = SCALED( نقطو[-2].x );
            vec1.y = SCALED( نقطو[-2].y );

            vec2.x = SCALED( نقطو[-1].x );
            vec2.y = SCALED( نقطو[-1].y );

            إذا ( نقطو <= حد_بندي )
            {
              FT_ويڪٽر  vec;
              FT_ويڪٽر  v_وچ;


              ٽيگ = FT_وڪر_ٽيگ( ٽيگ_فهرست[0] );

              vec.x = SCALED( نقطو->x );
              vec.y = SCALED( نقطو->y );

              إذا ( ٽيگ == FT_وڪر_ٽيگ_CUBIC )
              {
                v_وچ.x = ( vec2.x + vec.x ) / 2;
                v_وچ.y = ( vec2.y + vec.y ) / 2;

                vec = v_وچ;
              }
              وإلا إذا ( ٽيگ != FT_وڪر_ٽيگ_ON )
                اذهب Invalid_Outline;

              FT_TRACE5(( "  cubic to (%.2f, %.2f)"
                          " with controls (%.2f, %.2f) and (%.2f, %.2f)\n",
                          (مزدوج)vec.x / 64,
                          (مزدوج)vec.y / 64,
                          (مزدوج)vec1.x / 64,
                          (مزدوج)vec1.y / 64,
                          (مزدوج)vec2.x / 64,
                          (مزدوج)vec2.y / 64 ));
              error = func_interface->cubic_منزل( &vec1, &vec2, &vec, user );
              إذا ( error )
                اذهب Exit;

              إذا ( ٽيگ == FT_وڪر_ٽيگ_CUBIC )
              {
                نقطو--;
                ٽيگ_فهرست--;
              }
              استمر;
            }

            FT_TRACE5(( "  cubic to (%.2f, %.2f)"
                        " with controls (%.2f, %.2f) and (%.2f, %.2f)\n",
                        (مزدوج)v_شروعات.x / 64,
                        (مزدوج)v_شروعات.y / 64,
                        (مزدوج)vec1.x / 64,
                        (مزدوج)vec1.y / 64,
                        (مزدوج)vec2.x / 64,
                        (مزدوج)vec2.y / 64 ));
            error = func_interface->cubic_منزل( &vec1, &vec2, &v_شروعات, user );
            اذهب Close;
          }
        }
      }

      /* close the contour with a line segment */
      FT_TRACE5(( "  line to (%.2f, %.2f)\n",
                  (مزدوج)v_شروعات.x / 64, (مزدوج)v_شروعات.y / 64 ));
      error = func_interface->سٽ_منزل( &v_شروعات, user );

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
                  FT_UInt      انگ_Points,
                  FT_Int       انگ_Contours,
                  FT_Outline  *anoutline )
  {
    FT_Error   error;
    FT_يادگيري  يادگيري;


    إذا ( !library )
      أرجع FT_THROW( Invalid_Library_Handle );

    يادگيري = library->يادگيري;

    إذا ( !anoutline || !يادگيري )
      أرجع FT_THROW( Invalid_دليل );

    *anoutline = null_outline;

    إذا ( انگ_Contours < 0                  ||
         (FT_UInt)انگ_Contours > انگ_Points )
      أرجع FT_THROW( Invalid_دليل );

    إذا ( انگ_Points > FT_OUTLINE_POINTS_MAX )
      أرجع FT_THROW( Array_Too_Large );

    إذا ( FT_NEW_ARRAY( anoutline->points,   انگ_Points   ) ||
         FT_NEW_ARRAY( anoutline->ٽيگ_فهرست,     انگ_Points   ) ||
         FT_NEW_ARRAY( anoutline->contours, انگ_Contours ) )
      اذهب Fail;

    anoutline->n_points    = (FT_UShort)انگ_Points;
    anoutline->n_contours  = (FT_UShort)انگ_Contours;
    anoutline->flags      |= FT_OUTLINE_OWNER;

    أرجع FT_Err_Ok;

  Fail:
    anoutline->flags |= FT_OUTLINE_OWNER;
    FT_Outline_پورو( library, anoutline );

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
      FT_Int  end0, پڇاڙي;
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
        پڇاڙي = outline->contours[n];

        /* note that we don't accept empty contours */
        إذا ( پڇاڙي <= end0 || پڇاڙي >= n_points )
          اذهب Bad;

        end0 = پڇاڙي;
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
  FT_Outline_Copy( ثابت FT_Outline*  ذريعو,
                   FT_Outline        *هدف )
  {
    FT_Int  is_owner;


    إذا ( !ذريعو || !هدف )
      أرجع FT_THROW( Invalid_Outline );

    إذا ( ذريعو->n_points   != هدف->n_points   ||
         ذريعو->n_contours != هدف->n_contours )
      أرجع FT_THROW( Invalid_دليل );

    إذا ( ذريعو == هدف )
      أرجع FT_Err_Ok;

    إذا ( ذريعو->n_points )
    {
      FT_ARRAY_COPY( هدف->points, ذريعو->points, ذريعو->n_points );
      FT_ARRAY_COPY( هدف->ٽيگ_فهرست,   ذريعو->ٽيگ_فهرست,   ذريعو->n_points );
    }

    إذا ( ذريعو->n_contours )
      FT_ARRAY_COPY( هدف->contours, ذريعو->contours, ذريعو->n_contours );

    /* copy all flags, except the `FT_OUTLINE_OWNER' one */
    is_owner      = هدف->flags & FT_OUTLINE_OWNER;
    هدف->flags = ذريعو->flags;

    هدف->flags &= ~FT_OUTLINE_OWNER;
    هدف->flags |= is_owner;

    أرجع FT_Err_Ok;
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Outline_پورو( FT_Library   library,
                   FT_Outline*  outline )
  {
    FT_يادگيري  يادگيري;


    إذا ( !library )
      أرجع FT_THROW( Invalid_Library_Handle );

    إذا ( !outline )
      أرجع FT_THROW( Invalid_Outline );

    يادگيري = library->يادگيري;

    إذا ( !يادگيري )
      أرجع FT_THROW( Invalid_دليل );

    إذا ( outline->flags & FT_OUTLINE_OWNER )
    {
      FT_FREE( outline->points   );
      FT_FREE( outline->ٽيگ_فهرست     );
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
        FT_ويڪٽر*  vec   = outline->points;
        FT_ويڪٽر*  حد_بندي = vec + outline->n_points;


        xMin = xMax = vec->x;
        yMin = yMax = vec->y;
        vec++;

        لكل ( ; vec < حد_بندي; vec++ )
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
                        FT_Pos             x_وٿي,
                        FT_Pos             y_وٿي )
  {
    FT_UShort   n;
    FT_ويڪٽر*  vec;


    إذا ( !outline )
      أرجع;

    vec = outline->points;

    لكل ( n = 0; n < outline->n_points; n++ )
    {
      vec->x = جوڙيو_LONG( vec->x, x_وٿي );
      vec->y = جوڙيو_LONG( vec->y, y_وٿي );
      vec++;
    }
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( فراغ )
  FT_Outline_ابتو( FT_Outline*  outline )
  {
    FT_UShort  n;
    FT_Int     پهريون, آخري;


    إذا ( !outline )
      أرجع;

    آخري = -1;
    لكل ( n = 0; n < outline->n_contours; n++ )
    {
      پهريون = آخري + 1;
      آخري  = outline->contours[n];

      /* Keep the first point unless it is a cubic control.  In that */
      /* case, reverse the entire contour to preserve control pairs, */
      /* including contours with no explicit on-curve points.        */
      إذا ( FT_وڪر_ٽيگ( outline->ٽيگ_فهرست[پهريون] ) != FT_وڪر_ٽيگ_CUBIC )
        پهريون++;

      /* reverse point table */
      {
        FT_ويڪٽر*  p = outline->points + پهريون;
        FT_ويڪٽر*  q = outline->points + آخري;
        FT_ويڪٽر   مٽا_سٽا;


        طالما ( p < q )
        {
          مٽا_سٽا = *p;
          *p   = *q;
          *q   = مٽا_سٽا;
          p++;
          q--;
        }
      }

      /* reverse tags table */
      {
        FT_Byte*  p = outline->ٽيگ_فهرست + پهريون;
        FT_Byte*  q = outline->ٽيگ_فهرست + آخري;


        طالما ( p < q )
        {
          FT_Byte  مٽا_سٽا;


          مٽا_سٽا = *p;
          *p   = *q;
          *q   = مٽا_سٽا;
          p++;
          q--;
        }
      }
    }

    outline->flags ^= FT_OUTLINE_ابتو_FILL;
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Outline_Render( FT_Library         library,
                     FT_Outline*        outline,
                     FT_Raster_Params*  params )
  {
    FT_Error     error;
    FT_Renderer  renderer;
    FT_List_ڳنڍ  ڳنڍ;
    FT_BBox      cbox;


    إذا ( !library )
      أرجع FT_THROW( Invalid_Library_Handle );

    إذا ( !outline )
      أرجع FT_THROW( Invalid_Outline );

    إذا ( !params )
      أرجع FT_THROW( Invalid_دليل );

    FT_Outline_Get_CBox( outline, &cbox );
    إذا ( cbox.xMin < -0x1000000L || cbox.yMin < -0x1000000L ||
         cbox.xMax >  0x1000000L || cbox.yMax >  0x1000000L )
      أرجع FT_THROW( Invalid_Outline );

    renderer = library->cur_renderer;
    ڳنڍ     = library->renderers.سر;

    params->ذريعو = (فراغ*)outline;

    /* preset clip_box for direct mode */
    إذا ( params->flags & FT_RASTER_FLAG_سڌو    &&
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
                                     &ڳنڍ );
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
      أرجع FT_THROW( Invalid_دليل );

    /* other checks are delayed to `FT_Outline_Render' */

    params.هدف = abitmap;
    params.flags  = 0;

    إذا ( abitmap->pixel_گهڻائي == FT_PIXEL_گهڻائي_ناسي  ||
         abitmap->pixel_گهڻائي == FT_PIXEL_گهڻائي_LCD   ||
         abitmap->pixel_گهڻائي == FT_PIXEL_گهڻائي_LCD_V )
      params.flags |= FT_RASTER_FLAG_AA;

    أرجع FT_Outline_Render( library, outline, &params );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( فراغ )
  FT_ويڪٽر_Transform( FT_ويڪٽر*        ويڪٽر,
                       ثابت FT_ميٽرڪس*  ميٽرڪس )
  {
    FT_Pos  xz, yz;


    إذا ( !ويڪٽر || !ميٽرڪس )
      أرجع;

    xz = FT_MulFix( ويڪٽر->x, ميٽرڪس->xx ) +
         FT_MulFix( ويڪٽر->y, ميٽرڪس->xy );

    yz = FT_MulFix( ويڪٽر->x, ميٽرڪس->yx ) +
         FT_MulFix( ويڪٽر->y, ميٽرڪس->yy );

    ويڪٽر->x = xz;
    ويڪٽر->y = yz;
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( فراغ )
  FT_Outline_Transform( ثابت FT_Outline*  outline,
                        ثابت FT_ميٽرڪس*   ميٽرڪس )
  {
    FT_ويڪٽر*  vec;
    FT_ويڪٽر*  حد_بندي;


    إذا ( !outline || !ميٽرڪس || !outline->points )
      أرجع;

    vec   = outline->points;
    حد_بندي = vec + outline->n_points;

    لكل ( ; vec < حد_بندي; vec++ )
      FT_ويڪٽر_Transform( vec, ميٽرڪس );
  }


#إذا 0

#وصف_ڪريو FT_OUTLINE_GET_CONTOUR( outline, c, پهريون, آخري )  \
  افعل                                                       \
  {                                                        \
    (پهريون) = ( c > 0 ) ? (outline)->points +              \
                            (outline)->contours[c - 1] + 1 \
                        : (outline)->points;               \
    (آخري) = (outline)->points + (outline)->contours[c];   \
  } طالما ( 0 )


  /* Is a point in some contour?                     */
  /*                                                 */
  /* We treat every point of the contour as if it    */
  /* it were ON.  That is, we allow false positives, */
  /* but disallow false negatives.  (XXX really?)    */
  ساكن FT_Bool
  ft_contour_has( FT_Outline*  outline,
                  FT_Short     c,
                  FT_ويڪٽر*   نقطو )
  {
    FT_ويڪٽر*  پهريون;
    FT_ويڪٽر*  آخري;
    FT_ويڪٽر*  a;
    FT_ويڪٽر*  b;
    FT_UInt     n = 0;


    FT_OUTLINE_GET_CONTOUR( outline, c, پهريون, آخري );

    لكل ( a = پهريون; a <= آخري; a++ )
    {
      FT_Pos  x;
      FT_Int  intersect;


      b = ( a == آخري ) ? پهريون : a + 1;

      intersect = ( a->y - نقطو->y ) ^ ( b->y - نقطو->y );

      /* a and b are on the same side */
      إذا ( intersect >= 0 )
      {
        إذا ( intersect == 0 && a->y == نقطو->y )
        {
          إذا ( ( a->x <= نقطو->x && b->x >= نقطو->x ) ||
               ( a->x >= نقطو->x && b->x <= نقطو->x ) )
            أرجع 1;
        }

        استمر;
      }

      x = a->x + ( b->x - a->x ) * (نقطو->y - a->y ) / ( b->y - a->y );

      إذا ( x < نقطو->x )
        n++;
      وإلا إذا ( x == نقطو->x )
        أرجع 1;
    }

    أرجع n & 1;
  }


  ساكن FT_Bool
  ft_contour_enclosed( FT_Outline*  outline,
                       FT_UShort    c )
  {
    FT_ويڪٽر*  پهريون;
    FT_ويڪٽر*  آخري;
    FT_Short    i;


    FT_OUTLINE_GET_CONTOUR( outline, c, پهريون, آخري );

    لكل ( i = 0; i < outline->n_contours; i++ )
    {
      إذا ( i != c && ft_contour_has( outline, i, پهريون ) )
      {
        FT_ويڪٽر*  pt;


        لكل ( pt = پهريون + 1; pt <= آخري; pt++ )
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
    FT_ويڪٽر*      پهريون;
    FT_ويڪٽر*      آخري;
    FT_Orientation  orient = FT_ORIENTATION_NONE;


    پهريون = outline->points;
    لكل ( i = 0; i < outline->n_contours; i++, پهريون = آخري + 1 )
    {
      FT_ويڪٽر*  نقطو;
      FT_ويڪٽر*  xmin_نقطو;
      FT_Pos      xmin;


      آخري = outline->points + outline->contours[i];

      /* skip degenerate contours */
      إذا ( آخري < پهريون + 2 )
        استمر;

      إذا ( ft_contour_enclosed( outline, i ) )
        استمر;

      xmin       = پهريون->x;
      xmin_نقطو = پهريون;

      لكل ( نقطو = پهريون + 1; نقطو <= آخري; نقطو++ )
      {
        إذا ( نقطو->x < xmin )
        {
          xmin       = نقطو->x;
          xmin_نقطو = نقطو;
        }
      }

      /* check the orientation of the contour */
      {
        FT_ويڪٽر*      prev;
        FT_ويڪٽر*      ايندڙ;
        FT_Orientation  o;


        prev = ( xmin_نقطو == پهريون ) ? آخري : xmin_نقطو - 1;
        ايندڙ = ( xmin_نقطو == آخري ) ? پهريون : xmin_نقطو + 1;

        إذا ( FT_Atan2( prev->x - xmin_نقطو->x, prev->y - xmin_نقطو->y ) >
             FT_Atan2( ايندڙ->x - xmin_نقطو->x, ايندڙ->y - xmin_نقطو->y ) )
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

#جيڪڏهن_پورو /* 0 */


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
    FT_ويڪٽر*      points;
    FT_Int          c, پهريون, آخري;
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
        أرجع FT_THROW( Invalid_دليل );
      وإلا
        أرجع FT_Err_Ok;
    }

    points = outline->points;

    آخري = -1;
    لكل ( c = 0; c < outline->n_contours; c++ )
    {
      FT_ويڪٽر  اندريون, ٻاهريون, anchor, سرڪايو;
      FT_Fixed   l_اندريون, l_ٻاهريون, l_anchor = 0, l, q, d;
      FT_Int     i, j, k;


      پهريون = آخري + 1;
      آخري  = outline->contours[c];
      l_اندريون  = 0;

      /* pacify compiler */
      اندريون.x = اندريون.y = anchor.x = anchor.y = 0;

      /* Counter j cycles though the points; counter i advances only  */
      /* when points are moved; anchor k marks the first moved point. */
      لكل ( i = آخري, j = پهريون, k = -1;
            j != i && i != k;
            j = j < آخري ? j + 1 : پهريون )
      {
        إذا ( j != k )
        {
          ٻاهريون.x = points[j].x - points[i].x;
          ٻاهريون.y = points[j].y - points[i].y;
          l_ٻاهريون = (FT_Fixed)FT_ويڪٽر_Norm_Len( &ٻاهريون );

          إذا ( l_ٻاهريون == 0 )
            استمر;
        }
        وإلا
        {
          ٻاهريون   = anchor;
          l_ٻاهريون = l_anchor;
        }

        إذا ( l_اندريون != 0 )
        {
          إذا ( k < 0 )
          {
            k        = i;
            anchor   = اندريون;
            l_anchor = l_اندريون;
          }

          d = FT_MulFix( اندريون.x, ٻاهريون.x ) + FT_MulFix( اندريون.y, ٻاهريون.y );

          /* shift only if turn is less than ~160 degrees */
          إذا ( d > -0xF000L )
          {
            d = d + 0x10000L;

            /* shift components along lateral bisector in proper orientation */
            سرڪايو.x = اندريون.y + ٻاهريون.y;
            سرڪايو.y = اندريون.x + ٻاهريون.x;

            إذا ( orientation == FT_ORIENTATION_TRUETYPE )
              سرڪايو.x = -سرڪايو.x;
            وإلا
              سرڪايو.y = -سرڪايو.y;

            /* restrict shift magnitude to better handle collapsing segments */
            q = FT_MulFix( ٻاهريون.x, اندريون.y ) - FT_MulFix( ٻاهريون.y, اندريون.x );
            إذا ( orientation == FT_ORIENTATION_TRUETYPE )
              q = -q;

            l = FT_MIN( l_اندريون, l_ٻاهريون );

            /* non-strict inequalities avoid divide-by-zero when q == l == 0 */
            إذا ( FT_MulFix( xstrength, q ) <= FT_MulFix( l, d ) )
              سرڪايو.x = FT_MulDiv( سرڪايو.x, xstrength, d );
            وإلا
              سرڪايو.x = FT_MulDiv( سرڪايو.x, l, q );


            إذا ( FT_MulFix( ystrength, q ) <= FT_MulFix( l, d ) )
              سرڪايو.y = FT_MulDiv( سرڪايو.y, ystrength, d );
            وإلا
              سرڪايو.y = FT_MulDiv( سرڪايو.y, l, q );
          }
          وإلا
            سرڪايو.x = سرڪايو.y = 0;

          لكل ( ;
                i != j;
                i = i < آخري ? i + 1 : پهريون )
          {
            points[i].x += xstrength + سرڪايو.x;
            points[i].y += ystrength + سرڪايو.y;
          }
        }
        وإلا
          i = j;

        اندريون   = ٻاهريون;
        l_اندريون = l_ٻاهريون;
      }
    }

    أرجع FT_Err_Ok;
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Orientation )
  FT_Outline_Get_Orientation( FT_Outline*  outline )
  {
    FT_ويڪٽر*  points;
    FT_Int      c, n, پهريون, آخري;

#جيڪڏهن_وصف_ٿيل FT_INT64
    FT_Int64    area = 0;
#وإلا
    FT_BBox     cbox = { 0, 0, 0, 0 };
    FT_Int      xshift, yshift;
    FT_ويڪٽر   v_prev, v_cur;
    FT_Pos      area = 0;
#جيڪڏهن_پورو


    إذا ( !outline || outline->n_points <= 0 )
      أرجع FT_ORIENTATION_TRUETYPE;

    /* We use the nonzero winding rule to find the orientation.       */
    /* Since glyph outlines behave much more `regular' than arbitrary */
    /* cubic or quadratic curves, this test deals with the polygon    */
    /* only that is spanned up by the control points.                 */

    points = outline->points;

#جيڪڏهن_وصف_ٿيل FT_INT64

    /* Single-pass shoelace.  The 64-bit accumulator removes the need   */
    /* for the coordinate shift pre-pass (and its `FT_Outline_Get_CBox` */
    /* traversal) that the 32-bit variant below uses.                   */
    آخري = -1;
    لكل ( c = 0; c < outline->n_contours; c++ )
    {
      FT_Pos  prev_x, prev_y;


      پهريون = آخري + 1;
      آخري  = outline->contours[c];

      prev_x = points[آخري].x;
      prev_y = points[آخري].y;

      لكل ( n = پهريون; n <= آخري; n++ )
      {
        FT_Pos  cur_x = points[n].x;
        FT_Pos  cur_y = points[n].y;


        area = جوڙيو_INT64( area,
                          MUL_INT64( SUB_INT64( cur_y, prev_y ),
                                     جوڙيو_INT64( cur_x, prev_x ) ) );

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

    آخري = -1;
    لكل ( c = 0; c < outline->n_contours; c++ )
    {
      پهريون = آخري + 1;
      آخري  = outline->contours[c];

      v_prev.x = points[آخري].x >> xshift;
      v_prev.y = points[آخري].y >> yshift;

      لكل ( n = پهريون; n <= آخري; n++ )
      {
        v_cur.x = points[n].x >> xshift;
        v_cur.y = points[n].y >> yshift;

        area = جوڙيو_LONG( area,
                         MUL_LONG( v_cur.y - v_prev.y,
                                   v_cur.x + v_prev.x ) );

        v_prev = v_cur;
      }
    }

#جيڪڏهن_پورو /* FT_INT64 */

    إذا ( area > 0 )
      أرجع FT_ORIENTATION_POSTSCRIPT;
    وإلا إذا ( area < 0 )
      أرجع FT_ORIENTATION_TRUETYPE;
    وإلا
      أرجع FT_ORIENTATION_NONE;
  }


/*پڇاڙي*/
