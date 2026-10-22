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


#شامل <freetype/ftoutln.h>
#شامل <freetype/internal/ftobjs.h>
#شامل <freetype/internal/ftcalc.h>
#شامل <freetype/internal/ftdebug.h>
#شامل <freetype/fttrigon.h>


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#تعریف_منسوخ  FT_COMPONENT
#تعریف_کرو FT_COMPONENT  outline


  ساکن
  ثابت FT_Outline  null_outline = { 0, 0, تہی, تہی, تہی, 0 };


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Outline_Decompose( FT_Outline*              outline,
                        ثابت FT_Outline_Funcs*  func_interface,
                        خالی*                    user )
  {
#تعریف_منسوخ  SCALED
#تعریف_کرو SCALED( x )  ( (x) * ( 1L << سرکاؤ ) - delta )

    FT_سمتیہ   v_آخری;
    FT_سمتیہ   v_control;
    FT_سمتیہ   v_آغاز;

    FT_سمتیہ*  نقطہ;
    FT_سمتیہ*  حد_بندی;
    FT_Byte*    لیبل_فہرست;

    FT_Error    error;

    FT_Int   n;         /* index of contour in outline     */
    FT_Int   پہلا;     /* index of first point in contour */
    FT_Int   آخری;      /* index of last point in contour  */

    FT_Int   لیبل;       /* current point's state           */

    FT_Int   سرکاؤ;
    FT_Pos   delta;


    اگر ( !outline )
      واپس FT_THROW( Invalid_Outline );

    اگر ( !func_interface )
      واپس FT_THROW( Invalid_دلیل );

    سرکاؤ = func_interface->سرکاؤ;
    delta = func_interface->delta;

    آخری = -1;
    برائے ( n = 0; n < outline->n_contours; n++ )
    {
      FT_TRACE5(( "FT_Outline_Decompose: Contour %d\n", n ));

      پہلا = آخری + 1;
      آخری  = outline->contours[n];
      اگر ( آخری < پہلا )
        جاؤ Invalid_Outline;

      حد_بندی = outline->points + آخری;

      v_آغاز   = outline->points[پہلا];
      v_آغاز.x = SCALED( v_آغاز.x );
      v_آغاز.y = SCALED( v_آغاز.y );

      v_آخری   = outline->points[آخری];
      v_آخری.x = SCALED( v_آخری.x );
      v_آخری.y = SCALED( v_آخری.y );

      v_control = v_آغاز;

      نقطہ = outline->points + پہلا;
      لیبل_فہرست  = outline->لیبل_فہرست   + پہلا;
      لیبل   = FT_خم_لیبل( لیبل_فہرست[0] );

      /* check first point to determine origin */
      اگر ( لیبل != FT_خم_لیبل_ON )
      {
        FT_Int  آخری_لیبل = FT_خم_لیبل( outline->لیبل_فہرست[آخری] );


        /* first point is a control point.  Yes, this happens. */
        اگر ( آخری_لیبل == FT_خم_لیبل_ON )
        {
          /* start at last point if it is on the curve */
          v_آغاز = v_آخری;
          حد_بندی--;
        }
        ورنہ
        {
          اگر ( آخری_لیبل != لیبل )
            جاؤ Invalid_Outline;

          /* if both first and last points are controls,   */
          /* start at their middle and record its position */
          /* for closure                                   */
          v_آغاز.x = ( v_آغاز.x + v_آخری.x ) / 2;
          v_آغاز.y = ( v_آغاز.y + v_آخری.y ) / 2;

          /* v_last = v_start; */
        }
        نقطہ--;
        لیبل_فہرست--;
      }

      FT_TRACE5(( "  move to (%.2f, %.2f)\n",
                  (دوہرا)v_آغاز.x / 64, (دوہرا)v_آغاز.y / 64 ));
      error = func_interface->move_مقصد( &v_آغاز, user );
      اگر ( error )
        جاؤ Exit;

      جب ( نقطہ < حد_بندی )
      {
        نقطہ++;
        لیبل_فہرست++;

        لیبل = FT_خم_لیبل( لیبل_فہرست[0] );
        انتخاب ( لیبل )
        {
        حالت FT_خم_لیبل_ON:  /* emit a single line_to */
          {
            FT_سمتیہ  vec;


            vec.x = SCALED( نقطہ->x );
            vec.y = SCALED( نقطہ->y );

            FT_TRACE5(( "  line to (%.2f, %.2f)\n",
                        (دوہرا)vec.x / 64, (دوہرا)vec.y / 64 ));
            error = func_interface->سطر_مقصد( &vec, user );
            اگر ( error )
              جاؤ Exit;
            جاری;
          }

        حالت FT_خم_لیبل_CONIC:  /* consume conic arcs */
          v_control.x = SCALED( نقطہ->x );
          v_control.y = SCALED( نقطہ->y );

        Do_Conic:
          اگر ( نقطہ < حد_بندی )
          {
            FT_سمتیہ  vec;
            FT_سمتیہ  v_درمیان;


            نقطہ++;
            لیبل_فہرست++;
            لیبل = FT_خم_لیبل( لیبل_فہرست[0] );

            vec.x = SCALED( نقطہ->x );
            vec.y = SCALED( نقطہ->y );

            اگر ( لیبل == FT_خم_لیبل_ON )
            {
              FT_TRACE5(( "  conic to (%.2f, %.2f)"
                          " with control (%.2f, %.2f)\n",
                          (دوہرا)vec.x / 64,
                          (دوہرا)vec.y / 64,
                          (دوہرا)v_control.x / 64,
                          (دوہرا)v_control.y / 64 ));
              error = func_interface->conic_مقصد( &v_control, &vec, user );
              اگر ( error )
                جاؤ Exit;
              جاری;
            }

            اگر ( لیبل != FT_خم_لیبل_CONIC )
              جاؤ Invalid_Outline;

            v_درمیان.x = ( v_control.x + vec.x ) / 2;
            v_درمیان.y = ( v_control.y + vec.y ) / 2;

            FT_TRACE5(( "  conic to (%.2f, %.2f)"
                        " with control (%.2f, %.2f)\n",
                        (دوہرا)v_درمیان.x / 64,
                        (دوہرا)v_درمیان.y / 64,
                        (دوہرا)v_control.x / 64,
                        (دوہرا)v_control.y / 64 ));
            error = func_interface->conic_مقصد( &v_control, &v_درمیان, user );
            اگر ( error )
              جاؤ Exit;

            v_control = vec;
            جاؤ Do_Conic;
          }

          FT_TRACE5(( "  conic to (%.2f, %.2f)"
                      " with control (%.2f, %.2f)\n",
                      (دوہرا)v_آغاز.x / 64,
                      (دوہرا)v_آغاز.y / 64,
                      (دوہرا)v_control.x / 64,
                      (دوہرا)v_control.y / 64 ));
          error = func_interface->conic_مقصد( &v_control, &v_آغاز, user );
          جاؤ Close;

        طےشدہ:  /* FT_CURVE_TAG_CUBIC */
          {
            FT_سمتیہ  vec1, vec2;


            اگر ( نقطہ + 1 > حد_بندی                             ||
                 FT_خم_لیبل( لیبل_فہرست[1] ) != FT_خم_لیبل_CUBIC )
              جاؤ Invalid_Outline;

            نقطہ += 2;
            لیبل_فہرست  += 2;

            vec1.x = SCALED( نقطہ[-2].x );
            vec1.y = SCALED( نقطہ[-2].y );

            vec2.x = SCALED( نقطہ[-1].x );
            vec2.y = SCALED( نقطہ[-1].y );

            اگر ( نقطہ <= حد_بندی )
            {
              FT_سمتیہ  vec;
              FT_سمتیہ  v_درمیان;


              لیبل = FT_خم_لیبل( لیبل_فہرست[0] );

              vec.x = SCALED( نقطہ->x );
              vec.y = SCALED( نقطہ->y );

              اگر ( لیبل == FT_خم_لیبل_CUBIC )
              {
                v_درمیان.x = ( vec2.x + vec.x ) / 2;
                v_درمیان.y = ( vec2.y + vec.y ) / 2;

                vec = v_درمیان;
              }
              ورنہ اگر ( لیبل != FT_خم_لیبل_ON )
                جاؤ Invalid_Outline;

              FT_TRACE5(( "  cubic to (%.2f, %.2f)"
                          " with controls (%.2f, %.2f) and (%.2f, %.2f)\n",
                          (دوہرا)vec.x / 64,
                          (دوہرا)vec.y / 64,
                          (دوہرا)vec1.x / 64,
                          (دوہرا)vec1.y / 64,
                          (دوہرا)vec2.x / 64,
                          (دوہرا)vec2.y / 64 ));
              error = func_interface->cubic_مقصد( &vec1, &vec2, &vec, user );
              اگر ( error )
                جاؤ Exit;

              اگر ( لیبل == FT_خم_لیبل_CUBIC )
              {
                نقطہ--;
                لیبل_فہرست--;
              }
              جاری;
            }

            FT_TRACE5(( "  cubic to (%.2f, %.2f)"
                        " with controls (%.2f, %.2f) and (%.2f, %.2f)\n",
                        (دوہرا)v_آغاز.x / 64,
                        (دوہرا)v_آغاز.y / 64,
                        (دوہرا)vec1.x / 64,
                        (دوہرا)vec1.y / 64,
                        (دوہرا)vec2.x / 64,
                        (دوہرا)vec2.y / 64 ));
            error = func_interface->cubic_مقصد( &vec1, &vec2, &v_آغاز, user );
            جاؤ Close;
          }
        }
      }

      /* close the contour with a line segment */
      FT_TRACE5(( "  line to (%.2f, %.2f)\n",
                  (دوہرا)v_آغاز.x / 64, (دوہرا)v_آغاز.y / 64 ));
      error = func_interface->سطر_مقصد( &v_آغاز, user );

    Close:
      اگر ( error )
        جاؤ Exit;
    }

    FT_TRACE5(( "FT_Outline_Decompose: Done\n" ));
    واپس FT_Err_Ok;

  Invalid_Outline:
    error = FT_THROW( Invalid_Outline );
    /* fall through */

  Exit:
    FT_TRACE5(( "FT_Outline_Decompose: Error 0x%x\n", error ));
    واپس error;
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


    اگر ( !library )
      واپس FT_THROW( Invalid_Library_Handle );

    حافظہ = library->حافظہ;

    اگر ( !anoutline || !حافظہ )
      واپس FT_THROW( Invalid_دلیل );

    *anoutline = null_outline;

    اگر ( عدد_Contours < 0                  ||
         (FT_UInt)عدد_Contours > عدد_Points )
      واپس FT_THROW( Invalid_دلیل );

    اگر ( عدد_Points > FT_OUTLINE_POINTS_MAX )
      واپس FT_THROW( Array_Too_Large );

    اگر ( FT_NEW_ARRAY( anoutline->points,   عدد_Points   ) ||
         FT_NEW_ARRAY( anoutline->لیبل_فہرست,     عدد_Points   ) ||
         FT_NEW_ARRAY( anoutline->contours, عدد_Contours ) )
      جاؤ Fail;

    anoutline->n_points    = (FT_UShort)عدد_Points;
    anoutline->n_contours  = (FT_UShort)عدد_Contours;
    anoutline->flags      |= FT_OUTLINE_OWNER;

    واپس FT_Err_Ok;

  Fail:
    anoutline->flags |= FT_OUTLINE_OWNER;
    FT_Outline_مکمل( library, anoutline );

    واپس error;
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Outline_Check( FT_Outline*  outline )
  {
    اگر ( outline )
    {
      FT_Int  n_points   = outline->n_points;
      FT_Int  n_contours = outline->n_contours;
      FT_Int  end0, انتہا;
      FT_Int  n;


      FT_TRACE5(( "FT_Outline_Check: contours = %d, points = %d\n",
                  n_contours, n_points ));
      /* empty glyph? */
      اگر ( n_points == 0 && n_contours == 0 )
        واپس FT_Err_Ok;

      /* check point and contour counts */
      اگر ( n_points == 0 || n_contours == 0 )
        جاؤ Bad;

      end0 = -1;
      برائے ( n = 0; n < n_contours; n++ )
      {
        انتہا = outline->contours[n];

        /* note that we don't accept empty contours */
        اگر ( انتہا <= end0 || انتہا >= n_points )
          جاؤ Bad;

        end0 = انتہا;
      }

      اگر ( end0 != n_points - 1 )
        جاؤ Bad;

      /* XXX: check the tags array */
      واپس FT_Err_Ok;
    }

  Bad:
    واپس FT_THROW( Invalid_Outline );
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Outline_Copy( ثابت FT_Outline*  ماخذ,
                   FT_Outline        *ہدف )
  {
    FT_Int  is_owner;


    اگر ( !ماخذ || !ہدف )
      واپس FT_THROW( Invalid_Outline );

    اگر ( ماخذ->n_points   != ہدف->n_points   ||
         ماخذ->n_contours != ہدف->n_contours )
      واپس FT_THROW( Invalid_دلیل );

    اگر ( ماخذ == ہدف )
      واپس FT_Err_Ok;

    اگر ( ماخذ->n_points )
    {
      FT_ARRAY_COPY( ہدف->points, ماخذ->points, ماخذ->n_points );
      FT_ARRAY_COPY( ہدف->لیبل_فہرست,   ماخذ->لیبل_فہرست,   ماخذ->n_points );
    }

    اگر ( ماخذ->n_contours )
      FT_ARRAY_COPY( ہدف->contours, ماخذ->contours, ماخذ->n_contours );

    /* copy all flags, except the `FT_OUTLINE_OWNER' one */
    is_owner      = ہدف->flags & FT_OUTLINE_OWNER;
    ہدف->flags = ماخذ->flags;

    ہدف->flags &= ~FT_OUTLINE_OWNER;
    ہدف->flags |= is_owner;

    واپس FT_Err_Ok;
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Outline_مکمل( FT_Library   library,
                   FT_Outline*  outline )
  {
    FT_حافظہ  حافظہ;


    اگر ( !library )
      واپس FT_THROW( Invalid_Library_Handle );

    اگر ( !outline )
      واپس FT_THROW( Invalid_Outline );

    حافظہ = library->حافظہ;

    اگر ( !حافظہ )
      واپس FT_THROW( Invalid_دلیل );

    اگر ( outline->flags & FT_OUTLINE_OWNER )
    {
      FT_FREE( outline->points   );
      FT_FREE( outline->لیبل_فہرست     );
      FT_FREE( outline->contours );
    }
    *outline = null_outline;

    واپس FT_Err_Ok;
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( خالی )
  FT_Outline_Get_CBox( ثابت FT_Outline*  outline,
                       FT_BBox           *acbox )
  {
    FT_Pos  xMin, yMin, xMax, yMax;


    اگر ( outline && acbox )
    {
      اگر ( outline->n_points == 0 )
      {
        xMin = 0;
        yMin = 0;
        xMax = 0;
        yMax = 0;
      }
      ورنہ
      {
        FT_سمتیہ*  vec   = outline->points;
        FT_سمتیہ*  حد_بندی = vec + outline->n_points;


        xMin = xMax = vec->x;
        yMin = yMax = vec->y;
        vec++;

        برائے ( ; vec < حد_بندی; vec++ )
        {
          FT_Pos  x, y;


          x = vec->x;
          اگر ( x < xMin ) xMin = x;
          اگر ( x > xMax ) xMax = x;

          y = vec->y;
          اگر ( y < yMin ) yMin = y;
          اگر ( y > yMax ) yMax = y;
        }
      }
      acbox->xMin = xMin;
      acbox->xMax = xMax;
      acbox->yMin = yMin;
      acbox->yMax = yMax;
    }
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( خالی )
  FT_Outline_Translate( ثابت FT_Outline*  outline,
                        FT_Pos             x_انحراف,
                        FT_Pos             y_انحراف )
  {
    FT_UShort   n;
    FT_سمتیہ*  vec;


    اگر ( !outline )
      واپس;

    vec = outline->points;

    برائے ( n = 0; n < outline->n_points; n++ )
    {
      vec->x = جوڑو_LONG( vec->x, x_انحراف );
      vec->y = جوڑو_LONG( vec->y, y_انحراف );
      vec++;
    }
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( خالی )
  FT_Outline_الٹ( FT_Outline*  outline )
  {
    FT_UShort  n;
    FT_Int     پہلا, آخری;


    اگر ( !outline )
      واپس;

    آخری = -1;
    برائے ( n = 0; n < outline->n_contours; n++ )
    {
      پہلا = آخری + 1;
      آخری  = outline->contours[n];

      /* Keep the first point unless it is a cubic control.  In that */
      /* case, reverse the entire contour to preserve control pairs, */
      /* including contours with no explicit on-curve points.        */
      اگر ( FT_خم_لیبل( outline->لیبل_فہرست[پہلا] ) != FT_خم_لیبل_CUBIC )
        پہلا++;

      /* reverse point table */
      {
        FT_سمتیہ*  p = outline->points + پہلا;
        FT_سمتیہ*  q = outline->points + آخری;
        FT_سمتیہ   تبادلہ;


        جب ( p < q )
        {
          تبادلہ = *p;
          *p   = *q;
          *q   = تبادلہ;
          p++;
          q--;
        }
      }

      /* reverse tags table */
      {
        FT_Byte*  p = outline->لیبل_فہرست + پہلا;
        FT_Byte*  q = outline->لیبل_فہرست + آخری;


        جب ( p < q )
        {
          FT_Byte  تبادلہ;


          تبادلہ = *p;
          *p   = *q;
          *q   = تبادلہ;
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


    اگر ( !library )
      واپس FT_THROW( Invalid_Library_Handle );

    اگر ( !outline )
      واپس FT_THROW( Invalid_Outline );

    اگر ( !params )
      واپس FT_THROW( Invalid_دلیل );

    FT_Outline_Get_CBox( outline, &cbox );
    اگر ( cbox.xMin < -0x1000000L || cbox.yMin < -0x1000000L ||
         cbox.xMax >  0x1000000L || cbox.yMax >  0x1000000L )
      واپس FT_THROW( Invalid_Outline );

    renderer = library->cur_renderer;
    گرہ     = library->renderers.سر;

    params->ماخذ = (خالی*)outline;

    /* preset clip_box for direct mode */
    اگر ( params->flags & FT_RASTER_FLAG_براہ_راست    &&
         !( params->flags & FT_RASTER_FLAG_CLIP ) )
    {
      params->clip_box.xMin = cbox.xMin >> 6;
      params->clip_box.yMin = cbox.yMin >> 6;
      params->clip_box.xMax = ( cbox.xMax + 63 ) >> 6;
      params->clip_box.yMax = ( cbox.yMax + 63 ) >> 6;
    }

    error = FT_ERR( Cannot_Render_Glyph );
    جب ( renderer )
    {
      error = renderer->raster_render( renderer->raster, params );
      اگر ( !error || FT_ERR_NEQ( error, Cannot_Render_Glyph ) )
        توڑو;

      /* FT_Err_Cannot_Render_Glyph is returned if the render mode   */
      /* is unsupported by the current renderer for this glyph image */
      /* format                                                      */

      /* now, look for another renderer that supports the same */
      /* format                                                */
      renderer = FT_Lookup_Renderer( library, FT_GLYPH_FORMAT_OUTLINE,
                                     &گرہ );
    }

    واپس error;
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Outline_Get_Bitmap( FT_Library        library,
                         FT_Outline*       outline,
                         ثابت FT_Bitmap  *abitmap )
  {
    FT_Raster_Params  params;


    اگر ( !abitmap )
      واپس FT_THROW( Invalid_دلیل );

    /* other checks are delayed to `FT_Outline_Render' */

    params.ہدف = abitmap;
    params.flags  = 0;

    اگر ( abitmap->pixel_منوال == FT_PIXEL_منوال_سرمئی  ||
         abitmap->pixel_منوال == FT_PIXEL_منوال_LCD   ||
         abitmap->pixel_منوال == FT_PIXEL_منوال_LCD_V )
      params.flags |= FT_RASTER_FLAG_AA;

    واپس FT_Outline_Render( library, outline, &params );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( خالی )
  FT_سمتیہ_Transform( FT_سمتیہ*        سمتیہ,
                       ثابت FT_میٹرکس*  میٹرکس )
  {
    FT_Pos  xz, yz;


    اگر ( !سمتیہ || !میٹرکس )
      واپس;

    xz = FT_MulFix( سمتیہ->x, میٹرکس->xx ) +
         FT_MulFix( سمتیہ->y, میٹرکس->xy );

    yz = FT_MulFix( سمتیہ->x, میٹرکس->yx ) +
         FT_MulFix( سمتیہ->y, میٹرکس->yy );

    سمتیہ->x = xz;
    سمتیہ->y = yz;
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( خالی )
  FT_Outline_Transform( ثابت FT_Outline*  outline,
                        ثابت FT_میٹرکس*   میٹرکس )
  {
    FT_سمتیہ*  vec;
    FT_سمتیہ*  حد_بندی;


    اگر ( !outline || !میٹرکس || !outline->points )
      واپس;

    vec   = outline->points;
    حد_بندی = vec + outline->n_points;

    برائے ( ; vec < حد_بندی; vec++ )
      FT_سمتیہ_Transform( vec, میٹرکس );
  }


#اگر 0

#تعریف_کرو FT_OUTLINE_GET_CONTOUR( outline, c, پہلا, آخری )  \
  کرو                                                       \
  {                                                        \
    (پہلا) = ( c > 0 ) ? (outline)->points +              \
                            (outline)->contours[c - 1] + 1 \
                        : (outline)->points;               \
    (آخری) = (outline)->points + (outline)->contours[c];   \
  } جب ( 0 )


  /* Is a point in some contour?                     */
  /*                                                 */
  /* We treat every point of the contour as if it    */
  /* it were ON.  That is, we allow false positives, */
  /* but disallow false negatives.  (XXX really?)    */
  ساکن FT_Bool
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

    برائے ( a = پہلا; a <= آخری; a++ )
    {
      FT_Pos  x;
      FT_Int  intersect;


      b = ( a == آخری ) ? پہلا : a + 1;

      intersect = ( a->y - نقطہ->y ) ^ ( b->y - نقطہ->y );

      /* a and b are on the same side */
      اگر ( intersect >= 0 )
      {
        اگر ( intersect == 0 && a->y == نقطہ->y )
        {
          اگر ( ( a->x <= نقطہ->x && b->x >= نقطہ->x ) ||
               ( a->x >= نقطہ->x && b->x <= نقطہ->x ) )
            واپس 1;
        }

        جاری;
      }

      x = a->x + ( b->x - a->x ) * (نقطہ->y - a->y ) / ( b->y - a->y );

      اگر ( x < نقطہ->x )
        n++;
      ورنہ اگر ( x == نقطہ->x )
        واپس 1;
    }

    واپس n & 1;
  }


  ساکن FT_Bool
  ft_contour_enclosed( FT_Outline*  outline,
                       FT_UShort    c )
  {
    FT_سمتیہ*  پہلا;
    FT_سمتیہ*  آخری;
    FT_Short    i;


    FT_OUTLINE_GET_CONTOUR( outline, c, پہلا, آخری );

    برائے ( i = 0; i < outline->n_contours; i++ )
    {
      اگر ( i != c && ft_contour_has( outline, i, پہلا ) )
      {
        FT_سمتیہ*  pt;


        برائے ( pt = پہلا + 1; pt <= آخری; pt++ )
          اگر ( !ft_contour_has( outline, i, pt ) )
            واپس 0;

        واپس 1;
      }
    }

    واپس 0;
  }


  /* This version differs from the public one in that each */
  /* part (contour not enclosed in another contour) of the */
  /* outline is checked for orientation.  This is          */
  /* necessary for some buggy CJK fonts.                   */
  ساکن FT_Orientation
  ft_outline_get_orientation( FT_Outline*  outline )
  {
    FT_Short        i;
    FT_سمتیہ*      پہلا;
    FT_سمتیہ*      آخری;
    FT_Orientation  orient = FT_ORIENTATION_NONE;


    پہلا = outline->points;
    برائے ( i = 0; i < outline->n_contours; i++, پہلا = آخری + 1 )
    {
      FT_سمتیہ*  نقطہ;
      FT_سمتیہ*  xmin_نقطہ;
      FT_Pos      xmin;


      آخری = outline->points + outline->contours[i];

      /* skip degenerate contours */
      اگر ( آخری < پہلا + 2 )
        جاری;

      اگر ( ft_contour_enclosed( outline, i ) )
        جاری;

      xmin       = پہلا->x;
      xmin_نقطہ = پہلا;

      برائے ( نقطہ = پہلا + 1; نقطہ <= آخری; نقطہ++ )
      {
        اگر ( نقطہ->x < xmin )
        {
          xmin       = نقطہ->x;
          xmin_نقطہ = نقطہ;
        }
      }

      /* check the orientation of the contour */
      {
        FT_سمتیہ*      پچھلا;
        FT_سمتیہ*      اگلا;
        FT_Orientation  o;


        پچھلا = ( xmin_نقطہ == پہلا ) ? آخری : xmin_نقطہ - 1;
        اگلا = ( xmin_نقطہ == آخری ) ? پہلا : xmin_نقطہ + 1;

        اگر ( FT_Atan2( پچھلا->x - xmin_نقطہ->x, پچھلا->y - xmin_نقطہ->y ) >
             FT_Atan2( اگلا->x - xmin_نقطہ->x, اگلا->y - xmin_نقطہ->y ) )
          o = FT_ORIENTATION_POSTSCRIPT;
        ورنہ
          o = FT_ORIENTATION_TRUETYPE;

        اگر ( orient == FT_ORIENTATION_NONE )
          orient = o;
        ورنہ اگر ( orient != o )
          واپس FT_ORIENTATION_NONE;
      }
    }

    واپس orient;
  }

#اگر_ختم /* 0 */


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Outline_Embolden( FT_Outline*  outline,
                       FT_Pos       strength )
  {
    واپس FT_Outline_EmboldenXY( outline, strength, strength );
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


    اگر ( !outline )
      واپس FT_THROW( Invalid_Outline );

    xstrength /= 2;
    ystrength /= 2;
    اگر ( xstrength == 0 && ystrength == 0 )
      واپس FT_Err_Ok;

    orientation = FT_Outline_Get_Orientation( outline );
    اگر ( orientation == FT_ORIENTATION_NONE )
    {
      اگر ( outline->n_contours )
        واپس FT_THROW( Invalid_دلیل );
      ورنہ
        واپس FT_Err_Ok;
    }

    points = outline->points;

    آخری = -1;
    برائے ( c = 0; c < outline->n_contours; c++ )
    {
      FT_سمتیہ  اندر, برآمد, anchor, سرکاؤ;
      FT_Fixed   l_اندر, l_برآمد, l_anchor = 0, l, q, d;
      FT_Int     i, j, k;


      پہلا = آخری + 1;
      آخری  = outline->contours[c];
      l_اندر  = 0;

      /* pacify compiler */
      اندر.x = اندر.y = anchor.x = anchor.y = 0;

      /* Counter j cycles though the points; counter i advances only  */
      /* when points are moved; anchor k marks the first moved point. */
      برائے ( i = آخری, j = پہلا, k = -1;
            j != i && i != k;
            j = j < آخری ? j + 1 : پہلا )
      {
        اگر ( j != k )
        {
          برآمد.x = points[j].x - points[i].x;
          برآمد.y = points[j].y - points[i].y;
          l_برآمد = (FT_Fixed)FT_سمتیہ_Norm_Len( &برآمد );

          اگر ( l_برآمد == 0 )
            جاری;
        }
        ورنہ
        {
          برآمد   = anchor;
          l_برآمد = l_anchor;
        }

        اگر ( l_اندر != 0 )
        {
          اگر ( k < 0 )
          {
            k        = i;
            anchor   = اندر;
            l_anchor = l_اندر;
          }

          d = FT_MulFix( اندر.x, برآمد.x ) + FT_MulFix( اندر.y, برآمد.y );

          /* shift only if turn is less than ~160 degrees */
          اگر ( d > -0xF000L )
          {
            d = d + 0x10000L;

            /* shift components along lateral bisector in proper orientation */
            سرکاؤ.x = اندر.y + برآمد.y;
            سرکاؤ.y = اندر.x + برآمد.x;

            اگر ( orientation == FT_ORIENTATION_TRUETYPE )
              سرکاؤ.x = -سرکاؤ.x;
            ورنہ
              سرکاؤ.y = -سرکاؤ.y;

            /* restrict shift magnitude to better handle collapsing segments */
            q = FT_MulFix( برآمد.x, اندر.y ) - FT_MulFix( برآمد.y, اندر.x );
            اگر ( orientation == FT_ORIENTATION_TRUETYPE )
              q = -q;

            l = FT_MIN( l_اندر, l_برآمد );

            /* non-strict inequalities avoid divide-by-zero when q == l == 0 */
            اگر ( FT_MulFix( xstrength, q ) <= FT_MulFix( l, d ) )
              سرکاؤ.x = FT_MulDiv( سرکاؤ.x, xstrength, d );
            ورنہ
              سرکاؤ.x = FT_MulDiv( سرکاؤ.x, l, q );


            اگر ( FT_MulFix( ystrength, q ) <= FT_MulFix( l, d ) )
              سرکاؤ.y = FT_MulDiv( سرکاؤ.y, ystrength, d );
            ورنہ
              سرکاؤ.y = FT_MulDiv( سرکاؤ.y, l, q );
          }
          ورنہ
            سرکاؤ.x = سرکاؤ.y = 0;

          برائے ( ;
                i != j;
                i = i < آخری ? i + 1 : پہلا )
          {
            points[i].x += xstrength + سرکاؤ.x;
            points[i].y += ystrength + سرکاؤ.y;
          }
        }
        ورنہ
          i = j;

        اندر   = برآمد;
        l_اندر = l_برآمد;
      }
    }

    واپس FT_Err_Ok;
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Orientation )
  FT_Outline_Get_Orientation( FT_Outline*  outline )
  {
    FT_سمتیہ*  points;
    FT_Int      c, n, پہلا, آخری;

#اگر_تعریف_شدہ FT_INT64
    FT_Int64    area = 0;
#ورنہ
    FT_BBox     cbox = { 0, 0, 0, 0 };
    FT_Int      xshift, yshift;
    FT_سمتیہ   v_پچھلا, v_cur;
    FT_Pos      area = 0;
#اگر_ختم


    اگر ( !outline || outline->n_points <= 0 )
      واپس FT_ORIENTATION_TRUETYPE;

    /* We use the nonzero winding rule to find the orientation.       */
    /* Since glyph outlines behave much more `regular' than arbitrary */
    /* cubic or quadratic curves, this test deals with the polygon    */
    /* only that is spanned up by the control points.                 */

    points = outline->points;

#اگر_تعریف_شدہ FT_INT64

    /* Single-pass shoelace.  The 64-bit accumulator removes the need   */
    /* for the coordinate shift pre-pass (and its `FT_Outline_Get_CBox` */
    /* traversal) that the 32-bit variant below uses.                   */
    آخری = -1;
    برائے ( c = 0; c < outline->n_contours; c++ )
    {
      FT_Pos  پچھلا_x, پچھلا_y;


      پہلا = آخری + 1;
      آخری  = outline->contours[c];

      پچھلا_x = points[آخری].x;
      پچھلا_y = points[آخری].y;

      برائے ( n = پہلا; n <= آخری; n++ )
      {
        FT_Pos  cur_x = points[n].x;
        FT_Pos  cur_y = points[n].y;


        area = جوڑو_INT64( area,
                          MUL_INT64( SUB_INT64( cur_y, پچھلا_y ),
                                     جوڑو_INT64( cur_x, پچھلا_x ) ) );

        پچھلا_x = cur_x;
        پچھلا_y = cur_y;
      }
    }

#ورنہ /* !FT_INT64 */

    FT_Outline_Get_CBox( outline, &cbox );

    /* Handle collapsed outlines to avoid undefined FT_MSB. */
    اگر ( cbox.xMin == cbox.xMax || cbox.yMin == cbox.yMax )
      واپس FT_ORIENTATION_NONE;

    /* Reject values large outlines. */
    اگر ( cbox.xMin < -0x1000000L || cbox.yMin < -0x1000000L ||
         cbox.xMax >  0x1000000L || cbox.yMax >  0x1000000L )
      واپس FT_ORIENTATION_NONE;

    xshift = FT_MSB( (FT_UInt32)( FT_ABS( cbox.xMax ) |
                                  FT_ABS( cbox.xMin ) ) ) - 14;
    xshift = FT_MAX( xshift, 0 );

    yshift = FT_MSB( (FT_UInt32)( cbox.yMax - cbox.yMin ) ) - 14;
    yshift = FT_MAX( yshift, 0 );

    آخری = -1;
    برائے ( c = 0; c < outline->n_contours; c++ )
    {
      پہلا = آخری + 1;
      آخری  = outline->contours[c];

      v_پچھلا.x = points[آخری].x >> xshift;
      v_پچھلا.y = points[آخری].y >> yshift;

      برائے ( n = پہلا; n <= آخری; n++ )
      {
        v_cur.x = points[n].x >> xshift;
        v_cur.y = points[n].y >> yshift;

        area = جوڑو_LONG( area,
                         MUL_LONG( v_cur.y - v_پچھلا.y,
                                   v_cur.x + v_پچھلا.x ) );

        v_پچھلا = v_cur;
      }
    }

#اگر_ختم /* FT_INT64 */

    اگر ( area > 0 )
      واپس FT_ORIENTATION_POSTSCRIPT;
    ورنہ اگر ( area < 0 )
      واپس FT_ORIENTATION_TRUETYPE;
    ورنہ
      واپس FT_ORIENTATION_NONE;
  }


/*انتہا*/
