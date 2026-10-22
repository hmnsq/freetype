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
#تعریف_رد  FT_COMPONENT
#تعریف_کرو FT_COMPONENT  outline


  ساكن
  ثابت FT_Outline  null_outline = { 0, 0, عدم, عدم, عدم, 0 };


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Outline_Decompose( FT_Outline*              outline,
                        ثابت FT_Outline_Funcs*  func_interface,
                        فراغ*                    user )
  {
#تعریف_رد  SCALED
#تعریف_کرو SCALED( x )  ( (x) * ( 1L << کھسکاؤ ) - delta )

    FT_سمتیہ   v_آخری;
    FT_سمتیہ   v_control;
    FT_سمتیہ   v_شروع;

    FT_سمتیہ*  نقطہ;
    FT_سمتیہ*  حد_بندی;
    FT_Byte*    ٹیگ_سوچی;

    FT_Error    error;

    FT_Int   n;         /* index of contour in outline     */
    FT_Int   پہلا;     /* index of first point in contour */
    FT_Int   آخری;      /* index of last point in contour  */

    FT_Int   ٹیگ;       /* current point's state           */

    FT_Int   کھسکاؤ;
    FT_Pos   delta;


    إذا ( !outline )
      أرجع FT_THROW( Invalid_Outline );

    إذا ( !func_interface )
      أرجع FT_THROW( Invalid_دلیل );

    کھسکاؤ = func_interface->کھسکاؤ;
    delta = func_interface->delta;

    آخری = -1;
    لكل ( n = 0; n < outline->n_contours; n++ )
    {
      FT_TRACE5(( "FT_Outline_Decompose: Contour %d\n", n ));

      پہلا = آخری + 1;
      آخری  = outline->contours[n];
      إذا ( آخری < پہلا )
        اذهب Invalid_Outline;

      حد_بندی = outline->points + آخری;

      v_شروع   = outline->points[پہلا];
      v_شروع.x = SCALED( v_شروع.x );
      v_شروع.y = SCALED( v_شروع.y );

      v_آخری   = outline->points[آخری];
      v_آخری.x = SCALED( v_آخری.x );
      v_آخری.y = SCALED( v_آخری.y );

      v_control = v_شروع;

      نقطہ = outline->points + پہلا;
      ٹیگ_سوچی  = outline->ٹیگ_سوچی   + پہلا;
      ٹیگ   = FT_ونگ_ٹیگ( ٹیگ_سوچی[0] );

      /* check first point to determine origin */
      إذا ( ٹیگ != FT_ونگ_ٹیگ_ON )
      {
        FT_Int  آخری_ٹیگ = FT_ونگ_ٹیگ( outline->ٹیگ_سوچی[آخری] );


        /* first point is a control point.  Yes, this happens. */
        إذا ( آخری_ٹیگ == FT_ونگ_ٹیگ_ON )
        {
          /* start at last point if it is on the curve */
          v_شروع = v_آخری;
          حد_بندی--;
        }
        وإلا
        {
          إذا ( آخری_ٹیگ != ٹیگ )
            اذهب Invalid_Outline;

          /* if both first and last points are controls,   */
          /* start at their middle and record its position */
          /* for closure                                   */
          v_شروع.x = ( v_شروع.x + v_آخری.x ) / 2;
          v_شروع.y = ( v_شروع.y + v_آخری.y ) / 2;

          /* v_last = v_start; */
        }
        نقطہ--;
        ٹیگ_سوچی--;
      }

      FT_TRACE5(( "  move to (%.2f, %.2f)\n",
                  (مزدوج)v_شروع.x / 64, (مزدوج)v_شروع.y / 64 ));
      error = func_interface->move_منزل( &v_شروع, user );
      إذا ( error )
        اذهب Exit;

      طالما ( نقطہ < حد_بندی )
      {
        نقطہ++;
        ٹیگ_سوچی++;

        ٹیگ = FT_ونگ_ٹیگ( ٹیگ_سوچی[0] );
        بدل ( ٹیگ )
        {
        حالة FT_ونگ_ٹیگ_ON:  /* emit a single line_to */
          {
            FT_سمتیہ  vec;


            vec.x = SCALED( نقطہ->x );
            vec.y = SCALED( نقطہ->y );

            FT_TRACE5(( "  line to (%.2f, %.2f)\n",
                        (مزدوج)vec.x / 64, (مزدوج)vec.y / 64 ));
            error = func_interface->سطر_منزل( &vec, user );
            إذا ( error )
              اذهب Exit;
            استمر;
          }

        حالة FT_ونگ_ٹیگ_CONIC:  /* consume conic arcs */
          v_control.x = SCALED( نقطہ->x );
          v_control.y = SCALED( نقطہ->y );

        Do_Conic:
          إذا ( نقطہ < حد_بندی )
          {
            FT_سمتیہ  vec;
            FT_سمتیہ  v_درمیان;


            نقطہ++;
            ٹیگ_سوچی++;
            ٹیگ = FT_ونگ_ٹیگ( ٹیگ_سوچی[0] );

            vec.x = SCALED( نقطہ->x );
            vec.y = SCALED( نقطہ->y );

            إذا ( ٹیگ == FT_ونگ_ٹیگ_ON )
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

            إذا ( ٹیگ != FT_ونگ_ٹیگ_CONIC )
              اذهب Invalid_Outline;

            v_درمیان.x = ( v_control.x + vec.x ) / 2;
            v_درمیان.y = ( v_control.y + vec.y ) / 2;

            FT_TRACE5(( "  conic to (%.2f, %.2f)"
                        " with control (%.2f, %.2f)\n",
                        (مزدوج)v_درمیان.x / 64,
                        (مزدوج)v_درمیان.y / 64,
                        (مزدوج)v_control.x / 64,
                        (مزدوج)v_control.y / 64 ));
            error = func_interface->conic_منزل( &v_control, &v_درمیان, user );
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
          error = func_interface->conic_منزل( &v_control, &v_شروع, user );
          اذهب Close;

        افتراضي:  /* FT_CURVE_TAG_CUBIC */
          {
            FT_سمتیہ  vec1, vec2;


            إذا ( نقطہ + 1 > حد_بندی                             ||
                 FT_ونگ_ٹیگ( ٹیگ_سوچی[1] ) != FT_ونگ_ٹیگ_CUBIC )
              اذهب Invalid_Outline;

            نقطہ += 2;
            ٹیگ_سوچی  += 2;

            vec1.x = SCALED( نقطہ[-2].x );
            vec1.y = SCALED( نقطہ[-2].y );

            vec2.x = SCALED( نقطہ[-1].x );
            vec2.y = SCALED( نقطہ[-1].y );

            إذا ( نقطہ <= حد_بندی )
            {
              FT_سمتیہ  vec;
              FT_سمتیہ  v_درمیان;


              ٹیگ = FT_ونگ_ٹیگ( ٹیگ_سوچی[0] );

              vec.x = SCALED( نقطہ->x );
              vec.y = SCALED( نقطہ->y );

              إذا ( ٹیگ == FT_ونگ_ٹیگ_CUBIC )
              {
                v_درمیان.x = ( vec2.x + vec.x ) / 2;
                v_درمیان.y = ( vec2.y + vec.y ) / 2;

                vec = v_درمیان;
              }
              وإلا إذا ( ٹیگ != FT_ونگ_ٹیگ_ON )
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

              إذا ( ٹیگ == FT_ونگ_ٹیگ_CUBIC )
              {
                نقطہ--;
                ٹیگ_سوچی--;
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
            error = func_interface->cubic_منزل( &vec1, &vec2, &v_شروع, user );
            اذهب Close;
          }
        }
      }

      /* close the contour with a line segment */
      FT_TRACE5(( "  line to (%.2f, %.2f)\n",
                  (مزدوج)v_شروع.x / 64, (مزدوج)v_شروع.y / 64 ));
      error = func_interface->سطر_منزل( &v_شروع, user );

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
                  FT_UInt      عدد_Points,
                  FT_Int       عدد_Contours,
                  FT_Outline  *anoutline )
  {
    FT_Error   error;
    FT_حافظہ  حافظہ;


    إذا ( !library )
      أرجع FT_THROW( Invalid_Library_Handle );

    حافظہ = library->حافظہ;

    إذا ( !anoutline || !حافظہ )
      أرجع FT_THROW( Invalid_دلیل );

    *anoutline = null_outline;

    إذا ( عدد_Contours < 0                  ||
         (FT_UInt)عدد_Contours > عدد_Points )
      أرجع FT_THROW( Invalid_دلیل );

    إذا ( عدد_Points > FT_OUTLINE_POINTS_MAX )
      أرجع FT_THROW( Array_Too_Large );

    إذا ( FT_NEW_ARRAY( anoutline->points,   عدد_Points   ) ||
         FT_NEW_ARRAY( anoutline->ٹیگ_سوچی,     عدد_Points   ) ||
         FT_NEW_ARRAY( anoutline->contours, عدد_Contours ) )
      اذهب Fail;

    anoutline->n_points    = (FT_UShort)عدد_Points;
    anoutline->n_contours  = (FT_UShort)عدد_Contours;
    anoutline->flags      |= FT_OUTLINE_OWNER;

    أرجع FT_Err_Ok;

  Fail:
    anoutline->flags |= FT_OUTLINE_OWNER;
    FT_Outline_پورا( library, anoutline );

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
      FT_Int  end0, سرا;
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
        سرا = outline->contours[n];

        /* note that we don't accept empty contours */
        إذا ( سرا <= end0 || سرا >= n_points )
          اذهب Bad;

        end0 = سرا;
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
  FT_Outline_Copy( ثابت FT_Outline*  سروت,
                   FT_Outline        *نشانہ )
  {
    FT_Int  is_owner;


    إذا ( !سروت || !نشانہ )
      أرجع FT_THROW( Invalid_Outline );

    إذا ( سروت->n_points   != نشانہ->n_points   ||
         سروت->n_contours != نشانہ->n_contours )
      أرجع FT_THROW( Invalid_دلیل );

    إذا ( سروت == نشانہ )
      أرجع FT_Err_Ok;

    إذا ( سروت->n_points )
    {
      FT_ARRAY_COPY( نشانہ->points, سروت->points, سروت->n_points );
      FT_ARRAY_COPY( نشانہ->ٹیگ_سوچی,   سروت->ٹیگ_سوچی,   سروت->n_points );
    }

    إذا ( سروت->n_contours )
      FT_ARRAY_COPY( نشانہ->contours, سروت->contours, سروت->n_contours );

    /* copy all flags, except the `FT_OUTLINE_OWNER' one */
    is_owner      = نشانہ->flags & FT_OUTLINE_OWNER;
    نشانہ->flags = سروت->flags;

    نشانہ->flags &= ~FT_OUTLINE_OWNER;
    نشانہ->flags |= is_owner;

    أرجع FT_Err_Ok;
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Outline_پورا( FT_Library   library,
                   FT_Outline*  outline )
  {
    FT_حافظہ  حافظہ;


    إذا ( !library )
      أرجع FT_THROW( Invalid_Library_Handle );

    إذا ( !outline )
      أرجع FT_THROW( Invalid_Outline );

    حافظہ = library->حافظہ;

    إذا ( !حافظہ )
      أرجع FT_THROW( Invalid_دلیل );

    إذا ( outline->flags & FT_OUTLINE_OWNER )
    {
      FT_FREE( outline->points   );
      FT_FREE( outline->ٹیگ_سوچی     );
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
        FT_سمتیہ*  vec   = outline->points;
        FT_سمتیہ*  حد_بندی = vec + outline->n_points;


        xMin = xMax = vec->x;
        yMin = yMax = vec->y;
        vec++;

        لكل ( ; vec < حد_بندی; vec++ )
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
                        FT_Pos             x_وتھ,
                        FT_Pos             y_وتھ )
  {
    FT_UShort   n;
    FT_سمتیہ*  vec;


    إذا ( !outline )
      أرجع;

    vec = outline->points;

    لكل ( n = 0; n < outline->n_points; n++ )
    {
      vec->x = جوڑو_LONG( vec->x, x_وتھ );
      vec->y = جوڑو_LONG( vec->y, y_وتھ );
      vec++;
    }
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( فراغ )
  FT_Outline_الٹ( FT_Outline*  outline )
  {
    FT_UShort  n;
    FT_Int     پہلا, آخری;


    إذا ( !outline )
      أرجع;

    آخری = -1;
    لكل ( n = 0; n < outline->n_contours; n++ )
    {
      پہلا = آخری + 1;
      آخری  = outline->contours[n];

      /* Keep the first point unless it is a cubic control.  In that */
      /* case, reverse the entire contour to preserve control pairs, */
      /* including contours with no explicit on-curve points.        */
      إذا ( FT_ونگ_ٹیگ( outline->ٹیگ_سوچی[پہلا] ) != FT_ونگ_ٹیگ_CUBIC )
        پہلا++;

      /* reverse point table */
      {
        FT_سمتیہ*  p = outline->points + پہلا;
        FT_سمتیہ*  q = outline->points + آخری;
        FT_سمتیہ   وٹاندرا;


        طالما ( p < q )
        {
          وٹاندرا = *p;
          *p   = *q;
          *q   = وٹاندرا;
          p++;
          q--;
        }
      }

      /* reverse tags table */
      {
        FT_Byte*  p = outline->ٹیگ_سوچی + پہلا;
        FT_Byte*  q = outline->ٹیگ_سوچی + آخری;


        طالما ( p < q )
        {
          FT_Byte  وٹاندرا;


          وٹاندرا = *p;
          *p   = *q;
          *q   = وٹاندرا;
          p++;
          q--;
        }
      }
    }

    outline->flags ^= FT_OUTLINE_الٹ_FILL;
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Outline_Render( FT_Library         library,
                     FT_Outline*        outline,
                     FT_Raster_Params*  params )
  {
    FT_Error     error;
    FT_Renderer  renderer;
    FT_List_گرہ  گرہ;
    FT_BBox      cbox;


    إذا ( !library )
      أرجع FT_THROW( Invalid_Library_Handle );

    إذا ( !outline )
      أرجع FT_THROW( Invalid_Outline );

    إذا ( !params )
      أرجع FT_THROW( Invalid_دلیل );

    FT_Outline_Get_CBox( outline, &cbox );
    إذا ( cbox.xMin < -0x1000000L || cbox.yMin < -0x1000000L ||
         cbox.xMax >  0x1000000L || cbox.yMax >  0x1000000L )
      أرجع FT_THROW( Invalid_Outline );

    renderer = library->cur_renderer;
    گرہ     = library->renderers.سر;

    params->سروت = (فراغ*)outline;

    /* preset clip_box for direct mode */
    إذا ( params->flags & FT_RASTER_FLAG_سدھا    &&
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
                                     &گرہ );
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
      أرجع FT_THROW( Invalid_دلیل );

    /* other checks are delayed to `FT_Outline_Render' */

    params.نشانہ = abitmap;
    params.flags  = 0;

    إذا ( abitmap->pixel_منوال == FT_PIXEL_منوال_سلیٹی  ||
         abitmap->pixel_منوال == FT_PIXEL_منوال_LCD   ||
         abitmap->pixel_منوال == FT_PIXEL_منوال_LCD_V )
      params.flags |= FT_RASTER_FLAG_AA;

    أرجع FT_Outline_Render( library, outline, &params );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( فراغ )
  FT_سمتیہ_Transform( FT_سمتیہ*        سمتیہ,
                       ثابت FT_میٹرکس*  میٹرکس )
  {
    FT_Pos  xz, yz;


    إذا ( !سمتیہ || !میٹرکس )
      أرجع;

    xz = FT_MulFix( سمتیہ->x, میٹرکس->xx ) +
         FT_MulFix( سمتیہ->y, میٹرکس->xy );

    yz = FT_MulFix( سمتیہ->x, میٹرکس->yx ) +
         FT_MulFix( سمتیہ->y, میٹرکس->yy );

    سمتیہ->x = xz;
    سمتیہ->y = yz;
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( فراغ )
  FT_Outline_Transform( ثابت FT_Outline*  outline,
                        ثابت FT_میٹرکس*   میٹرکس )
  {
    FT_سمتیہ*  vec;
    FT_سمتیہ*  حد_بندی;


    إذا ( !outline || !میٹرکس || !outline->points )
      أرجع;

    vec   = outline->points;
    حد_بندی = vec + outline->n_points;

    لكل ( ; vec < حد_بندی; vec++ )
      FT_سمتیہ_Transform( vec, میٹرکس );
  }


#إذا 0

#تعریف_کرو FT_OUTLINE_GET_CONTOUR( outline, c, پہلا, آخری )  \
  افعل                                                       \
  {                                                        \
    (پہلا) = ( c > 0 ) ? (outline)->points +              \
                            (outline)->contours[c - 1] + 1 \
                        : (outline)->points;               \
    (آخری) = (outline)->points + (outline)->contours[c];   \
  } طالما ( 0 )


  /* Is a point in some contour?                     */
  /*                                                 */
  /* We treat every point of the contour as if it    */
  /* it were ON.  That is, we allow false positives, */
  /* but disallow false negatives.  (XXX really?)    */
  ساكن FT_Bool
  ft_contour_has( FT_Outline*  outline,
                  FT_Short     c,
                  FT_سمتیہ*   نقطہ )
  {
    FT_سمتیہ*  پہلا;
    FT_سمتیہ*  آخری;
    FT_سمتیہ*  a;
    FT_سمتیہ*  b;
    FT_UInt     n = 0;


    FT_OUTLINE_GET_CONTOUR( outline, c, پہلا, آخری );

    لكل ( a = پہلا; a <= آخری; a++ )
    {
      FT_Pos  x;
      FT_Int  intersect;


      b = ( a == آخری ) ? پہلا : a + 1;

      intersect = ( a->y - نقطہ->y ) ^ ( b->y - نقطہ->y );

      /* a and b are on the same side */
      إذا ( intersect >= 0 )
      {
        إذا ( intersect == 0 && a->y == نقطہ->y )
        {
          إذا ( ( a->x <= نقطہ->x && b->x >= نقطہ->x ) ||
               ( a->x >= نقطہ->x && b->x <= نقطہ->x ) )
            أرجع 1;
        }

        استمر;
      }

      x = a->x + ( b->x - a->x ) * (نقطہ->y - a->y ) / ( b->y - a->y );

      إذا ( x < نقطہ->x )
        n++;
      وإلا إذا ( x == نقطہ->x )
        أرجع 1;
    }

    أرجع n & 1;
  }


  ساكن FT_Bool
  ft_contour_enclosed( FT_Outline*  outline,
                       FT_UShort    c )
  {
    FT_سمتیہ*  پہلا;
    FT_سمتیہ*  آخری;
    FT_Short    i;


    FT_OUTLINE_GET_CONTOUR( outline, c, پہلا, آخری );

    لكل ( i = 0; i < outline->n_contours; i++ )
    {
      إذا ( i != c && ft_contour_has( outline, i, پہلا ) )
      {
        FT_سمتیہ*  pt;


        لكل ( pt = پہلا + 1; pt <= آخری; pt++ )
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
    FT_سمتیہ*      پہلا;
    FT_سمتیہ*      آخری;
    FT_Orientation  orient = FT_ORIENTATION_NONE;


    پہلا = outline->points;
    لكل ( i = 0; i < outline->n_contours; i++, پہلا = آخری + 1 )
    {
      FT_سمتیہ*  نقطہ;
      FT_سمتیہ*  xmin_نقطہ;
      FT_Pos      xmin;


      آخری = outline->points + outline->contours[i];

      /* skip degenerate contours */
      إذا ( آخری < پہلا + 2 )
        استمر;

      إذا ( ft_contour_enclosed( outline, i ) )
        استمر;

      xmin       = پہلا->x;
      xmin_نقطہ = پہلا;

      لكل ( نقطہ = پہلا + 1; نقطہ <= آخری; نقطہ++ )
      {
        إذا ( نقطہ->x < xmin )
        {
          xmin       = نقطہ->x;
          xmin_نقطہ = نقطہ;
        }
      }

      /* check the orientation of the contour */
      {
        FT_سمتیہ*      prev;
        FT_سمتیہ*      اگلا;
        FT_Orientation  o;


        prev = ( xmin_نقطہ == پہلا ) ? آخری : xmin_نقطہ - 1;
        اگلا = ( xmin_نقطہ == آخری ) ? پہلا : xmin_نقطہ + 1;

        إذا ( FT_Atan2( prev->x - xmin_نقطہ->x, prev->y - xmin_نقطہ->y ) >
             FT_Atan2( اگلا->x - xmin_نقطہ->x, اگلا->y - xmin_نقطہ->y ) )
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

#جے_انت /* 0 */


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
    FT_سمتیہ*      points;
    FT_Int          c, پہلا, آخری;
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
        أرجع FT_THROW( Invalid_دلیل );
      وإلا
        أرجع FT_Err_Ok;
    }

    points = outline->points;

    آخری = -1;
    لكل ( c = 0; c < outline->n_contours; c++ )
    {
      FT_سمتیہ  وچ, باہرلا, anchor, کھسکاؤ;
      FT_Fixed   l_وچ, l_باہرلا, l_anchor = 0, l, q, d;
      FT_Int     i, j, k;


      پہلا = آخری + 1;
      آخری  = outline->contours[c];
      l_وچ  = 0;

      /* pacify compiler */
      وچ.x = وچ.y = anchor.x = anchor.y = 0;

      /* Counter j cycles though the points; counter i advances only  */
      /* when points are moved; anchor k marks the first moved point. */
      لكل ( i = آخری, j = پہلا, k = -1;
            j != i && i != k;
            j = j < آخری ? j + 1 : پہلا )
      {
        إذا ( j != k )
        {
          باہرلا.x = points[j].x - points[i].x;
          باہرلا.y = points[j].y - points[i].y;
          l_باہرلا = (FT_Fixed)FT_سمتیہ_Norm_Len( &باہرلا );

          إذا ( l_باہرلا == 0 )
            استمر;
        }
        وإلا
        {
          باہرلا   = anchor;
          l_باہرلا = l_anchor;
        }

        إذا ( l_وچ != 0 )
        {
          إذا ( k < 0 )
          {
            k        = i;
            anchor   = وچ;
            l_anchor = l_وچ;
          }

          d = FT_MulFix( وچ.x, باہرلا.x ) + FT_MulFix( وچ.y, باہرلا.y );

          /* shift only if turn is less than ~160 degrees */
          إذا ( d > -0xF000L )
          {
            d = d + 0x10000L;

            /* shift components along lateral bisector in proper orientation */
            کھسکاؤ.x = وچ.y + باہرلا.y;
            کھسکاؤ.y = وچ.x + باہرلا.x;

            إذا ( orientation == FT_ORIENTATION_TRUETYPE )
              کھسکاؤ.x = -کھسکاؤ.x;
            وإلا
              کھسکاؤ.y = -کھسکاؤ.y;

            /* restrict shift magnitude to better handle collapsing segments */
            q = FT_MulFix( باہرلا.x, وچ.y ) - FT_MulFix( باہرلا.y, وچ.x );
            إذا ( orientation == FT_ORIENTATION_TRUETYPE )
              q = -q;

            l = FT_MIN( l_وچ, l_باہرلا );

            /* non-strict inequalities avoid divide-by-zero when q == l == 0 */
            إذا ( FT_MulFix( xstrength, q ) <= FT_MulFix( l, d ) )
              کھسکاؤ.x = FT_MulDiv( کھسکاؤ.x, xstrength, d );
            وإلا
              کھسکاؤ.x = FT_MulDiv( کھسکاؤ.x, l, q );


            إذا ( FT_MulFix( ystrength, q ) <= FT_MulFix( l, d ) )
              کھسکاؤ.y = FT_MulDiv( کھسکاؤ.y, ystrength, d );
            وإلا
              کھسکاؤ.y = FT_MulDiv( کھسکاؤ.y, l, q );
          }
          وإلا
            کھسکاؤ.x = کھسکاؤ.y = 0;

          لكل ( ;
                i != j;
                i = i < آخری ? i + 1 : پہلا )
          {
            points[i].x += xstrength + کھسکاؤ.x;
            points[i].y += ystrength + کھسکاؤ.y;
          }
        }
        وإلا
          i = j;

        وچ   = باہرلا;
        l_وچ = l_باہرلا;
      }
    }

    أرجع FT_Err_Ok;
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Orientation )
  FT_Outline_Get_Orientation( FT_Outline*  outline )
  {
    FT_سمتیہ*  points;
    FT_Int      c, n, پہلا, آخری;

#جے_تعریف_شدہ FT_INT64
    FT_Int64    area = 0;
#وإلا
    FT_BBox     cbox = { 0, 0, 0, 0 };
    FT_Int      xshift, yshift;
    FT_سمتیہ   v_prev, v_cur;
    FT_Pos      area = 0;
#جے_انت


    إذا ( !outline || outline->n_points <= 0 )
      أرجع FT_ORIENTATION_TRUETYPE;

    /* We use the nonzero winding rule to find the orientation.       */
    /* Since glyph outlines behave much more `regular' than arbitrary */
    /* cubic or quadratic curves, this test deals with the polygon    */
    /* only that is spanned up by the control points.                 */

    points = outline->points;

#جے_تعریف_شدہ FT_INT64

    /* Single-pass shoelace.  The 64-bit accumulator removes the need   */
    /* for the coordinate shift pre-pass (and its `FT_Outline_Get_CBox` */
    /* traversal) that the 32-bit variant below uses.                   */
    آخری = -1;
    لكل ( c = 0; c < outline->n_contours; c++ )
    {
      FT_Pos  prev_x, prev_y;


      پہلا = آخری + 1;
      آخری  = outline->contours[c];

      prev_x = points[آخری].x;
      prev_y = points[آخری].y;

      لكل ( n = پہلا; n <= آخری; n++ )
      {
        FT_Pos  cur_x = points[n].x;
        FT_Pos  cur_y = points[n].y;


        area = جوڑو_INT64( area,
                          MUL_INT64( SUB_INT64( cur_y, prev_y ),
                                     جوڑو_INT64( cur_x, prev_x ) ) );

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

    آخری = -1;
    لكل ( c = 0; c < outline->n_contours; c++ )
    {
      پہلا = آخری + 1;
      آخری  = outline->contours[c];

      v_prev.x = points[آخری].x >> xshift;
      v_prev.y = points[آخری].y >> yshift;

      لكل ( n = پہلا; n <= آخری; n++ )
      {
        v_cur.x = points[n].x >> xshift;
        v_cur.y = points[n].y >> yshift;

        area = جوڑو_LONG( area,
                         MUL_LONG( v_cur.y - v_prev.y,
                                   v_cur.x + v_prev.x ) );

        v_prev = v_cur;
      }
    }

#جے_انت /* FT_INT64 */

    إذا ( area > 0 )
      أرجع FT_ORIENTATION_POSTSCRIPT;
    وإلا إذا ( area < 0 )
      أرجع FT_ORIENTATION_TRUETYPE;
    وإلا
      أرجع FT_ORIENTATION_NONE;
  }


/*سرا*/
