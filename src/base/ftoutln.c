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
#تعریف_لغوه  FT_COMPONENT
#تعریف_کړه FT_COMPONENT  outline


  ساكن
  ثابت FT_Outline  null_outline = { 0, 0, عدم, عدم, عدم, 0 };


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Outline_Decompose( FT_Outline*              outline,
                        ثابت FT_Outline_Funcs*  func_interface,
                        فراغ*                    user )
  {
#تعریف_لغوه  SCALED
#تعریف_کړه SCALED( x )  ( (x) * ( 1L << ښويدنه ) - delta )

    FT_ویکټور   v_وروستی;
    FT_ویکټور   v_control;
    FT_ویکټور   v_پیل;

    FT_ویکټور*  ټکی;
    FT_ویکټور*  حد;
    FT_Byte*    نښانونه;

    FT_Error    error;

    FT_Int   n;         /* index of contour in outline     */
    FT_Int   لومړی;     /* index of first point in contour */
    FT_Int   وروستی;      /* index of last point in contour  */

    FT_Int   نښان;       /* current point's state           */

    FT_Int   ښويدنه;
    FT_Pos   delta;


    إذا ( !outline )
      أرجع FT_THROW( Invalid_Outline );

    إذا ( !func_interface )
      أرجع FT_THROW( Invalid_دلیل );

    ښويدنه = func_interface->ښويدنه;
    delta = func_interface->delta;

    وروستی = -1;
    لكل ( n = 0; n < outline->n_contours; n++ )
    {
      FT_TRACE5(( "FT_Outline_Decompose: Contour %d\n", n ));

      لومړی = وروستی + 1;
      وروستی  = outline->contours[n];
      إذا ( وروستی < لومړی )
        اذهب Invalid_Outline;

      حد = outline->points + وروستی;

      v_پیل   = outline->points[لومړی];
      v_پیل.x = SCALED( v_پیل.x );
      v_پیل.y = SCALED( v_پیل.y );

      v_وروستی   = outline->points[وروستی];
      v_وروستی.x = SCALED( v_وروستی.x );
      v_وروستی.y = SCALED( v_وروستی.y );

      v_control = v_پیل;

      ټکی = outline->points + لومړی;
      نښانونه  = outline->نښانونه   + لومړی;
      نښان   = FT_منحني_نښان( نښانونه[0] );

      /* check first point to determine origin */
      إذا ( نښان != FT_منحني_نښان_ON )
      {
        FT_Int  وروستی_نښان = FT_منحني_نښان( outline->نښانونه[وروستی] );


        /* first point is a control point.  Yes, this happens. */
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

          /* if both first and last points are controls,   */
          /* start at their middle and record its position */
          /* for closure                                   */
          v_پیل.x = ( v_پیل.x + v_وروستی.x ) / 2;
          v_پیل.y = ( v_پیل.y + v_وروستی.y ) / 2;

          /* v_last = v_start; */
        }
        ټکی--;
        نښانونه--;
      }

      FT_TRACE5(( "  move to (%.2f, %.2f)\n",
                  (مزدوج)v_پیل.x / 64, (مزدوج)v_پیل.y / 64 ));
      error = func_interface->move_منزل( &v_پیل, user );
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


            vec.x = SCALED( ټکی->x );
            vec.y = SCALED( ټکی->y );

            FT_TRACE5(( "  line to (%.2f, %.2f)\n",
                        (مزدوج)vec.x / 64, (مزدوج)vec.y / 64 ));
            error = func_interface->کرښه_منزل( &vec, user );
            إذا ( error )
              اذهب Exit;
            استمر;
          }

        حالة FT_منحني_نښان_CONIC:  /* consume conic arcs */
          v_control.x = SCALED( ټکی->x );
          v_control.y = SCALED( ټکی->y );

        Do_Conic:
          إذا ( ټکی < حد )
          {
            FT_ویکټور  vec;
            FT_ویکټور  v_منځ;


            ټکی++;
            نښانونه++;
            نښان = FT_منحني_نښان( نښانونه[0] );

            vec.x = SCALED( ټکی->x );
            vec.y = SCALED( ټکی->y );

            إذا ( نښان == FT_منحني_نښان_ON )
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

            إذا ( نښان != FT_منحني_نښان_CONIC )
              اذهب Invalid_Outline;

            v_منځ.x = ( v_control.x + vec.x ) / 2;
            v_منځ.y = ( v_control.y + vec.y ) / 2;

            FT_TRACE5(( "  conic to (%.2f, %.2f)"
                        " with control (%.2f, %.2f)\n",
                        (مزدوج)v_منځ.x / 64,
                        (مزدوج)v_منځ.y / 64,
                        (مزدوج)v_control.x / 64,
                        (مزدوج)v_control.y / 64 ));
            error = func_interface->conic_منزل( &v_control, &v_منځ, user );
            إذا ( error )
              اذهب Exit;

            v_control = vec;
            اذهب Do_Conic;
          }

          FT_TRACE5(( "  conic to (%.2f, %.2f)"
                      " with control (%.2f, %.2f)\n",
                      (مزدوج)v_پیل.x / 64,
                      (مزدوج)v_پیل.y / 64,
                      (مزدوج)v_control.x / 64,
                      (مزدوج)v_control.y / 64 ));
          error = func_interface->conic_منزل( &v_control, &v_پیل, user );
          اذهب Close;

        افتراضي:  /* FT_CURVE_TAG_CUBIC */
          {
            FT_ویکټور  vec1, vec2;


            إذا ( ټکی + 1 > حد                             ||
                 FT_منحني_نښان( نښانونه[1] ) != FT_منحني_نښان_CUBIC )
              اذهب Invalid_Outline;

            ټکی += 2;
            نښانونه  += 2;

            vec1.x = SCALED( ټکی[-2].x );
            vec1.y = SCALED( ټکی[-2].y );

            vec2.x = SCALED( ټکی[-1].x );
            vec2.y = SCALED( ټکی[-1].y );

            إذا ( ټکی <= حد )
            {
              FT_ویکټور  vec;
              FT_ویکټور  v_منځ;


              نښان = FT_منحني_نښان( نښانونه[0] );

              vec.x = SCALED( ټکی->x );
              vec.y = SCALED( ټکی->y );

              إذا ( نښان == FT_منحني_نښان_CUBIC )
              {
                v_منځ.x = ( vec2.x + vec.x ) / 2;
                v_منځ.y = ( vec2.y + vec.y ) / 2;

                vec = v_منځ;
              }
              وإلا إذا ( نښان != FT_منحني_نښان_ON )
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

              إذا ( نښان == FT_منحني_نښان_CUBIC )
              {
                ټکی--;
                نښانونه--;
              }
              استمر;
            }

            FT_TRACE5(( "  cubic to (%.2f, %.2f)"
                        " with controls (%.2f, %.2f) and (%.2f, %.2f)\n",
                        (مزدوج)v_پیل.x / 64,
                        (مزدوج)v_پیل.y / 64,
                        (مزدوج)vec1.x / 64,
                        (مزدوج)vec1.y / 64,
                        (مزدوج)vec2.x / 64,
                        (مزدوج)vec2.y / 64 ));
            error = func_interface->cubic_منزل( &vec1, &vec2, &v_پیل, user );
            اذهب Close;
          }
        }
      }

      /* close the contour with a line segment */
      FT_TRACE5(( "  line to (%.2f, %.2f)\n",
                  (مزدوج)v_پیل.x / 64, (مزدوج)v_پیل.y / 64 ));
      error = func_interface->کرښه_منزل( &v_پیل, user );

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
    FT_حافظه  حافظه;


    إذا ( !library )
      أرجع FT_THROW( Invalid_Library_Handle );

    حافظه = library->حافظه;

    إذا ( !anoutline || !حافظه )
      أرجع FT_THROW( Invalid_دلیل );

    *anoutline = null_outline;

    إذا ( عدد_Contours < 0                  ||
         (FT_UInt)عدد_Contours > عدد_Points )
      أرجع FT_THROW( Invalid_دلیل );

    إذا ( عدد_Points > FT_OUTLINE_POINTS_MAX )
      أرجع FT_THROW( Array_Too_Large );

    إذا ( FT_NEW_ARRAY( anoutline->points,   عدد_Points   ) ||
         FT_NEW_ARRAY( anoutline->نښانونه,     عدد_Points   ) ||
         FT_NEW_ARRAY( anoutline->contours, عدد_Contours ) )
      اذهب Fail;

    anoutline->n_points    = (FT_UShort)عدد_Points;
    anoutline->n_contours  = (FT_UShort)عدد_Contours;
    anoutline->flags      |= FT_OUTLINE_OWNER;

    أرجع FT_Err_Ok;

  Fail:
    anoutline->flags |= FT_OUTLINE_OWNER;
    FT_Outline_بشپړ( library, anoutline );

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
      FT_Int  end0, پای_ټکی;
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
        پای_ټکی = outline->contours[n];

        /* note that we don't accept empty contours */
        إذا ( پای_ټکی <= end0 || پای_ټکی >= n_points )
          اذهب Bad;

        end0 = پای_ټکی;
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
  FT_Outline_Copy( ثابت FT_Outline*  سرچینه,
                   FT_Outline        *موخه )
  {
    FT_Int  is_owner;


    إذا ( !سرچینه || !موخه )
      أرجع FT_THROW( Invalid_Outline );

    إذا ( سرچینه->n_points   != موخه->n_points   ||
         سرچینه->n_contours != موخه->n_contours )
      أرجع FT_THROW( Invalid_دلیل );

    إذا ( سرچینه == موخه )
      أرجع FT_Err_Ok;

    إذا ( سرچینه->n_points )
    {
      FT_ARRAY_COPY( موخه->points, سرچینه->points, سرچینه->n_points );
      FT_ARRAY_COPY( موخه->نښانونه,   سرچینه->نښانونه,   سرچینه->n_points );
    }

    إذا ( سرچینه->n_contours )
      FT_ARRAY_COPY( موخه->contours, سرچینه->contours, سرچینه->n_contours );

    /* copy all flags, except the `FT_OUTLINE_OWNER' one */
    is_owner      = موخه->flags & FT_OUTLINE_OWNER;
    موخه->flags = سرچینه->flags;

    موخه->flags &= ~FT_OUTLINE_OWNER;
    موخه->flags |= is_owner;

    أرجع FT_Err_Ok;
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Outline_بشپړ( FT_Library   library,
                   FT_Outline*  outline )
  {
    FT_حافظه  حافظه;


    إذا ( !library )
      أرجع FT_THROW( Invalid_Library_Handle );

    إذا ( !outline )
      أرجع FT_THROW( Invalid_Outline );

    حافظه = library->حافظه;

    إذا ( !حافظه )
      أرجع FT_THROW( Invalid_دلیل );

    إذا ( outline->flags & FT_OUTLINE_OWNER )
    {
      FT_FREE( outline->points   );
      FT_FREE( outline->نښانونه     );
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
        FT_ویکټور*  vec   = outline->points;
        FT_ویکټور*  حد = vec + outline->n_points;


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
                        FT_Pos             x_دننه_واټن,
                        FT_Pos             y_دننه_واټن )
  {
    FT_UShort   n;
    FT_ویکټور*  vec;


    إذا ( !outline )
      أرجع;

    vec = outline->points;

    لكل ( n = 0; n < outline->n_points; n++ )
    {
      vec->x = جمع_کړه_LONG( vec->x, x_دننه_واټن );
      vec->y = جمع_کړه_LONG( vec->y, y_دننه_واټن );
      vec++;
    }
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( فراغ )
  FT_Outline_سرچپه( FT_Outline*  outline )
  {
    FT_UShort  n;
    FT_Int     لومړی, وروستی;


    إذا ( !outline )
      أرجع;

    وروستی = -1;
    لكل ( n = 0; n < outline->n_contours; n++ )
    {
      لومړی = وروستی + 1;
      وروستی  = outline->contours[n];

      /* Keep the first point unless it is a cubic control.  In that */
      /* case, reverse the entire contour to preserve control pairs, */
      /* including contours with no explicit on-curve points.        */
      إذا ( FT_منحني_نښان( outline->نښانونه[لومړی] ) != FT_منحني_نښان_CUBIC )
        لومړی++;

      /* reverse point table */
      {
        FT_ویکټور*  p = outline->points + لومړی;
        FT_ویکټور*  q = outline->points + وروستی;
        FT_ویکټور   بدلون;


        طالما ( p < q )
        {
          بدلون = *p;
          *p   = *q;
          *q   = بدلون;
          p++;
          q--;
        }
      }

      /* reverse tags table */
      {
        FT_Byte*  p = outline->نښانونه + لومړی;
        FT_Byte*  q = outline->نښانونه + وروستی;


        طالما ( p < q )
        {
          FT_Byte  بدلون;


          بدلون = *p;
          *p   = *q;
          *q   = بدلون;
          p++;
          q--;
        }
      }
    }

    outline->flags ^= FT_OUTLINE_سرچپه_FILL;
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Outline_Render( FT_Library         library,
                     FT_Outline*        outline,
                     FT_Raster_Params*  params )
  {
    FT_Error     error;
    FT_Renderer  renderer;
    FT_List_غوټه  غوټه;
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
    غوټه     = library->renderers.سر_2;

    params->سرچینه = (فراغ*)outline;

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
                                     &غوټه );
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

    params.موخه = abitmap;
    params.flags  = 0;

    إذا ( abitmap->pixel_ډېرپېښ == FT_PIXEL_ډېرپېښ_خړ  ||
         abitmap->pixel_ډېرپېښ == FT_PIXEL_ډېرپېښ_LCD   ||
         abitmap->pixel_ډېرپېښ == FT_PIXEL_ډېرپېښ_LCD_V )
      params.flags |= FT_RASTER_FLAG_AA;

    أرجع FT_Outline_Render( library, outline, &params );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( فراغ )
  FT_ویکټور_Transform( FT_ویکټور*        ویکټور,
                       ثابت FT_ماتریکس*  ماتریکس )
  {
    FT_Pos  xz, yz;


    إذا ( !ویکټور || !ماتریکس )
      أرجع;

    xz = FT_MulFix( ویکټور->x, ماتریکس->xx ) +
         FT_MulFix( ویکټور->y, ماتریکس->xy );

    yz = FT_MulFix( ویکټور->x, ماتریکس->yx ) +
         FT_MulFix( ویکټور->y, ماتریکس->yy );

    ویکټور->x = xz;
    ویکټور->y = yz;
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( فراغ )
  FT_Outline_Transform( ثابت FT_Outline*  outline,
                        ثابت FT_ماتریکس*   ماتریکس )
  {
    FT_ویکټور*  vec;
    FT_ویکټور*  حد;


    إذا ( !outline || !ماتریکس || !outline->points )
      أرجع;

    vec   = outline->points;
    حد = vec + outline->n_points;

    لكل ( ; vec < حد; vec++ )
      FT_ویکټور_Transform( vec, ماتریکس );
  }


#إذا 0

#تعریف_کړه FT_OUTLINE_GET_CONTOUR( outline, c, لومړی, وروستی )  \
  افعل                                                       \
  {                                                        \
    (لومړی) = ( c > 0 ) ? (outline)->points +              \
                            (outline)->contours[c - 1] + 1 \
                        : (outline)->points;               \
    (وروستی) = (outline)->points + (outline)->contours[c];   \
  } طالما ( 0 )


  /* Is a point in some contour?                     */
  /*                                                 */
  /* We treat every point of the contour as if it    */
  /* it were ON.  That is, we allow false positives, */
  /* but disallow false negatives.  (XXX really?)    */
  ساكن FT_Bool
  ft_contour_has( FT_Outline*  outline,
                  FT_Short     c,
                  FT_ویکټور*   ټکی )
  {
    FT_ویکټور*  لومړی;
    FT_ویکټور*  وروستی;
    FT_ویکټور*  a;
    FT_ویکټور*  b;
    FT_UInt     n = 0;


    FT_OUTLINE_GET_CONTOUR( outline, c, لومړی, وروستی );

    لكل ( a = لومړی; a <= وروستی; a++ )
    {
      FT_Pos  x;
      FT_Int  intersect;


      b = ( a == وروستی ) ? لومړی : a + 1;

      intersect = ( a->y - ټکی->y ) ^ ( b->y - ټکی->y );

      /* a and b are on the same side */
      إذا ( intersect >= 0 )
      {
        إذا ( intersect == 0 && a->y == ټکی->y )
        {
          إذا ( ( a->x <= ټکی->x && b->x >= ټکی->x ) ||
               ( a->x >= ټکی->x && b->x <= ټکی->x ) )
            أرجع 1;
        }

        استمر;
      }

      x = a->x + ( b->x - a->x ) * (ټکی->y - a->y ) / ( b->y - a->y );

      إذا ( x < ټکی->x )
        n++;
      وإلا إذا ( x == ټکی->x )
        أرجع 1;
    }

    أرجع n & 1;
  }


  ساكن FT_Bool
  ft_contour_enclosed( FT_Outline*  outline,
                       FT_UShort    c )
  {
    FT_ویکټور*  لومړی;
    FT_ویکټور*  وروستی;
    FT_Short    i;


    FT_OUTLINE_GET_CONTOUR( outline, c, لومړی, وروستی );

    لكل ( i = 0; i < outline->n_contours; i++ )
    {
      إذا ( i != c && ft_contour_has( outline, i, لومړی ) )
      {
        FT_ویکټور*  pt;


        لكل ( pt = لومړی + 1; pt <= وروستی; pt++ )
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
    FT_ویکټور*      لومړی;
    FT_ویکټور*      وروستی;
    FT_Orientation  orient = FT_ORIENTATION_NONE;


    لومړی = outline->points;
    لكل ( i = 0; i < outline->n_contours; i++, لومړی = وروستی + 1 )
    {
      FT_ویکټور*  ټکی;
      FT_ویکټور*  xmin_ټکی;
      FT_Pos      xmin;


      وروستی = outline->points + outline->contours[i];

      /* skip degenerate contours */
      إذا ( وروستی < لومړی + 2 )
        استمر;

      إذا ( ft_contour_enclosed( outline, i ) )
        استمر;

      xmin       = لومړی->x;
      xmin_ټکی = لومړی;

      لكل ( ټکی = لومړی + 1; ټکی <= وروستی; ټکی++ )
      {
        إذا ( ټکی->x < xmin )
        {
          xmin       = ټکی->x;
          xmin_ټکی = ټکی;
        }
      }

      /* check the orientation of the contour */
      {
        FT_ویکټور*      prev;
        FT_ویکټور*      بل;
        FT_Orientation  o;


        prev = ( xmin_ټکی == لومړی ) ? وروستی : xmin_ټکی - 1;
        بل = ( xmin_ټکی == وروستی ) ? لومړی : xmin_ټکی + 1;

        إذا ( FT_Atan2( prev->x - xmin_ټکی->x, prev->y - xmin_ټکی->y ) >
             FT_Atan2( بل->x - xmin_ټکی->x, بل->y - xmin_ټکی->y ) )
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

#د_که_پای /* 0 */


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
    FT_ویکټور*      points;
    FT_Int          c, لومړی, وروستی;
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

    وروستی = -1;
    لكل ( c = 0; c < outline->n_contours; c++ )
    {
      FT_ویکټور  دننه, بهر, anchor, ښويدنه;
      FT_Fixed   l_دننه, l_بهر, l_anchor = 0, l, q, d;
      FT_Int     i, j, k;


      لومړی = وروستی + 1;
      وروستی  = outline->contours[c];
      l_دننه  = 0;

      /* pacify compiler */
      دننه.x = دننه.y = anchor.x = anchor.y = 0;

      /* Counter j cycles though the points; counter i advances only  */
      /* when points are moved; anchor k marks the first moved point. */
      لكل ( i = وروستی, j = لومړی, k = -1;
            j != i && i != k;
            j = j < وروستی ? j + 1 : لومړی )
      {
        إذا ( j != k )
        {
          بهر.x = points[j].x - points[i].x;
          بهر.y = points[j].y - points[i].y;
          l_بهر = (FT_Fixed)FT_ویکټور_Norm_Len( &بهر );

          إذا ( l_بهر == 0 )
            استمر;
        }
        وإلا
        {
          بهر   = anchor;
          l_بهر = l_anchor;
        }

        إذا ( l_دننه != 0 )
        {
          إذا ( k < 0 )
          {
            k        = i;
            anchor   = دننه;
            l_anchor = l_دننه;
          }

          d = FT_MulFix( دننه.x, بهر.x ) + FT_MulFix( دننه.y, بهر.y );

          /* shift only if turn is less than ~160 degrees */
          إذا ( d > -0xF000L )
          {
            d = d + 0x10000L;

            /* shift components along lateral bisector in proper orientation */
            ښويدنه.x = دننه.y + بهر.y;
            ښويدنه.y = دننه.x + بهر.x;

            إذا ( orientation == FT_ORIENTATION_TRUETYPE )
              ښويدنه.x = -ښويدنه.x;
            وإلا
              ښويدنه.y = -ښويدنه.y;

            /* restrict shift magnitude to better handle collapsing segments */
            q = FT_MulFix( بهر.x, دننه.y ) - FT_MulFix( بهر.y, دننه.x );
            إذا ( orientation == FT_ORIENTATION_TRUETYPE )
              q = -q;

            l = FT_MIN( l_دننه, l_بهر );

            /* non-strict inequalities avoid divide-by-zero when q == l == 0 */
            إذا ( FT_MulFix( xstrength, q ) <= FT_MulFix( l, d ) )
              ښويدنه.x = FT_MulDiv( ښويدنه.x, xstrength, d );
            وإلا
              ښويدنه.x = FT_MulDiv( ښويدنه.x, l, q );


            إذا ( FT_MulFix( ystrength, q ) <= FT_MulFix( l, d ) )
              ښويدنه.y = FT_MulDiv( ښويدنه.y, ystrength, d );
            وإلا
              ښويدنه.y = FT_MulDiv( ښويدنه.y, l, q );
          }
          وإلا
            ښويدنه.x = ښويدنه.y = 0;

          لكل ( ;
                i != j;
                i = i < وروستی ? i + 1 : لومړی )
          {
            points[i].x += xstrength + ښويدنه.x;
            points[i].y += ystrength + ښويدنه.y;
          }
        }
        وإلا
          i = j;

        دننه   = بهر;
        l_دننه = l_بهر;
      }
    }

    أرجع FT_Err_Ok;
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Orientation )
  FT_Outline_Get_Orientation( FT_Outline*  outline )
  {
    FT_ویکټور*  points;
    FT_Int      c, n, لومړی, وروستی;

#که_تعریف_شوی FT_INT64
    FT_Int64    area = 0;
#وإلا
    FT_BBox     cbox = { 0, 0, 0, 0 };
    FT_Int      xshift, yshift;
    FT_ویکټور   v_prev, v_cur;
    FT_Pos      area = 0;
#د_که_پای


    إذا ( !outline || outline->n_points <= 0 )
      أرجع FT_ORIENTATION_TRUETYPE;

    /* We use the nonzero winding rule to find the orientation.       */
    /* Since glyph outlines behave much more `regular' than arbitrary */
    /* cubic or quadratic curves, this test deals with the polygon    */
    /* only that is spanned up by the control points.                 */

    points = outline->points;

#که_تعریف_شوی FT_INT64

    /* Single-pass shoelace.  The 64-bit accumulator removes the need   */
    /* for the coordinate shift pre-pass (and its `FT_Outline_Get_CBox` */
    /* traversal) that the 32-bit variant below uses.                   */
    وروستی = -1;
    لكل ( c = 0; c < outline->n_contours; c++ )
    {
      FT_Pos  prev_x, prev_y;


      لومړی = وروستی + 1;
      وروستی  = outline->contours[c];

      prev_x = points[وروستی].x;
      prev_y = points[وروستی].y;

      لكل ( n = لومړی; n <= وروستی; n++ )
      {
        FT_Pos  cur_x = points[n].x;
        FT_Pos  cur_y = points[n].y;


        area = جمع_کړه_INT64( area,
                          MUL_INT64( SUB_INT64( cur_y, prev_y ),
                                     جمع_کړه_INT64( cur_x, prev_x ) ) );

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

    وروستی = -1;
    لكل ( c = 0; c < outline->n_contours; c++ )
    {
      لومړی = وروستی + 1;
      وروستی  = outline->contours[c];

      v_prev.x = points[وروستی].x >> xshift;
      v_prev.y = points[وروستی].y >> yshift;

      لكل ( n = لومړی; n <= وروستی; n++ )
      {
        v_cur.x = points[n].x >> xshift;
        v_cur.y = points[n].y >> yshift;

        area = جمع_کړه_LONG( area,
                         MUL_LONG( v_cur.y - v_prev.y,
                                   v_cur.x + v_prev.x ) );

        v_prev = v_cur;
      }
    }

#د_که_پای /* FT_INT64 */

    إذا ( area > 0 )
      أرجع FT_ORIENTATION_POSTSCRIPT;
    وإلا إذا ( area < 0 )
      أرجع FT_ORIENTATION_TRUETYPE;
    وإلا
      أرجع FT_ORIENTATION_NONE;
  }


/*پای_ټکی*/
