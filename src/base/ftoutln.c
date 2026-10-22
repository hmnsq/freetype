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


#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/ftoutln.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftobjs.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftcalc.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftdebug.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/fttrigon.h>


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ  FT_COMPONENT
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FT_COMPONENT  outline


  ਸਥਿਰ
  ਅਚਲ FT_Outline  null_outline = { 0, 0, ਨਿਰਾ, ਨਿਰਾ, ਨਿਰਾ, 0 };


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Outline_Decompose( FT_Outline*              outline,
                        ਅਚਲ FT_Outline_Funcs*  func_interface,
                        ਖਾਲੀ*                    user )
  {
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ  SCALED
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SCALED( x )  ( (x) * ( 1L << ਖਿਸਕਾਓ ) - delta )

    FT_ਸਦਿਸ਼   v_ਆਖਰੀ;
    FT_ਸਦਿਸ਼   v_control;
    FT_ਸਦਿਸ਼   v_ਸ਼ੁਰੂ;

    FT_ਸਦਿਸ਼*  ਬਿੰਦੂ;
    FT_ਸਦਿਸ਼*  ਹੱਦਬੰਦੀ;
    FT_Byte*    ਟੈਗ_ਸੂਚੀ;

    FT_Error    error;

    FT_Int   n;         /* index of contour in outline     */
    FT_Int   ਪਹਿਲਾ;     /* index of first point in contour */
    FT_Int   ਆਖਰੀ;      /* index of last point in contour  */

    FT_Int   ਟੈਗ;       /* current point's state           */

    FT_Int   ਖਿਸਕਾਓ;
    FT_Pos   delta;


    ਜੇ ( !outline )
      ਵਾਪਸ FT_THROW( Invalid_Outline );

    ਜੇ ( !func_interface )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    ਖਿਸਕਾਓ = func_interface->ਖਿਸਕਾਓ;
    delta = func_interface->delta;

    ਆਖਰੀ = -1;
    ਲਈ ( n = 0; n < outline->n_contours; n++ )
    {
      FT_TRACE5(( "FT_Outline_Decompose: Contour %d\n", n ));

      ਪਹਿਲਾ = ਆਖਰੀ + 1;
      ਆਖਰੀ  = outline->contours[n];
      ਜੇ ( ਆਖਰੀ < ਪਹਿਲਾ )
        ਜਾਓ Invalid_Outline;

      ਹੱਦਬੰਦੀ = outline->points + ਆਖਰੀ;

      v_ਸ਼ੁਰੂ   = outline->points[ਪਹਿਲਾ];
      v_ਸ਼ੁਰੂ.x = SCALED( v_ਸ਼ੁਰੂ.x );
      v_ਸ਼ੁਰੂ.y = SCALED( v_ਸ਼ੁਰੂ.y );

      v_ਆਖਰੀ   = outline->points[ਆਖਰੀ];
      v_ਆਖਰੀ.x = SCALED( v_ਆਖਰੀ.x );
      v_ਆਖਰੀ.y = SCALED( v_ਆਖਰੀ.y );

      v_control = v_ਸ਼ੁਰੂ;

      ਬਿੰਦੂ = outline->points + ਪਹਿਲਾ;
      ਟੈਗ_ਸੂਚੀ  = outline->ਟੈਗ_ਸੂਚੀ   + ਪਹਿਲਾ;
      ਟੈਗ   = FT_ਵਕਰ_ਟੈਗ( ਟੈਗ_ਸੂਚੀ[0] );

      /* check first point to determine origin */
      ਜੇ ( ਟੈਗ != FT_ਵਕਰ_ਟੈਗ_ON )
      {
        FT_Int  ਆਖਰੀ_ਟੈਗ = FT_ਵਕਰ_ਟੈਗ( outline->ਟੈਗ_ਸੂਚੀ[ਆਖਰੀ] );


        /* first point is a control point.  Yes, this happens. */
        ਜੇ ( ਆਖਰੀ_ਟੈਗ == FT_ਵਕਰ_ਟੈਗ_ON )
        {
          /* start at last point if it is on the curve */
          v_ਸ਼ੁਰੂ = v_ਆਖਰੀ;
          ਹੱਦਬੰਦੀ--;
        }
        ਨਹੀਂਤਾਂ
        {
          ਜੇ ( ਆਖਰੀ_ਟੈਗ != ਟੈਗ )
            ਜਾਓ Invalid_Outline;

          /* if both first and last points are controls,   */
          /* start at their middle and record its position */
          /* for closure                                   */
          v_ਸ਼ੁਰੂ.x = ( v_ਸ਼ੁਰੂ.x + v_ਆਖਰੀ.x ) / 2;
          v_ਸ਼ੁਰੂ.y = ( v_ਸ਼ੁਰੂ.y + v_ਆਖਰੀ.y ) / 2;

          /* v_last = v_start; */
        }
        ਬਿੰਦੂ--;
        ਟੈਗ_ਸੂਚੀ--;
      }

      FT_TRACE5(( "  move to (%.2f, %.2f)\n",
                  (ਦੁੱਗਣਾ)v_ਸ਼ੁਰੂ.x / 64, (ਦੁੱਗਣਾ)v_ਸ਼ੁਰੂ.y / 64 ));
      error = func_interface->move_ਮੰਜ਼ਿਲ( &v_ਸ਼ੁਰੂ, user );
      ਜੇ ( error )
        ਜਾਓ Exit;

      ਜਦੋਂ ( ਬਿੰਦੂ < ਹੱਦਬੰਦੀ )
      {
        ਬਿੰਦੂ++;
        ਟੈਗ_ਸੂਚੀ++;

        ਟੈਗ = FT_ਵਕਰ_ਟੈਗ( ਟੈਗ_ਸੂਚੀ[0] );
        ਚੋਣ ( ਟੈਗ )
        {
        ਹਾਲਤ FT_ਵਕਰ_ਟੈਗ_ON:  /* emit a single line_to */
          {
            FT_ਸਦਿਸ਼  vec;


            vec.x = SCALED( ਬਿੰਦੂ->x );
            vec.y = SCALED( ਬਿੰਦੂ->y );

            FT_TRACE5(( "  line to (%.2f, %.2f)\n",
                        (ਦੁੱਗਣਾ)vec.x / 64, (ਦੁੱਗਣਾ)vec.y / 64 ));
            error = func_interface->ਸਤਰ_ਮੰਜ਼ਿਲ( &vec, user );
            ਜੇ ( error )
              ਜਾਓ Exit;
            ਜਾਰੀ;
          }

        ਹਾਲਤ FT_ਵਕਰ_ਟੈਗ_CONIC:  /* consume conic arcs */
          v_control.x = SCALED( ਬਿੰਦੂ->x );
          v_control.y = SCALED( ਬਿੰਦੂ->y );

        Do_Conic:
          ਜੇ ( ਬਿੰਦੂ < ਹੱਦਬੰਦੀ )
          {
            FT_ਸਦਿਸ਼  vec;
            FT_ਸਦਿਸ਼  v_ਵਿਚਕਾਰ;


            ਬਿੰਦੂ++;
            ਟੈਗ_ਸੂਚੀ++;
            ਟੈਗ = FT_ਵਕਰ_ਟੈਗ( ਟੈਗ_ਸੂਚੀ[0] );

            vec.x = SCALED( ਬਿੰਦੂ->x );
            vec.y = SCALED( ਬਿੰਦੂ->y );

            ਜੇ ( ਟੈਗ == FT_ਵਕਰ_ਟੈਗ_ON )
            {
              FT_TRACE5(( "  conic to (%.2f, %.2f)"
                          " with control (%.2f, %.2f)\n",
                          (ਦੁੱਗਣਾ)vec.x / 64,
                          (ਦੁੱਗਣਾ)vec.y / 64,
                          (ਦੁੱਗਣਾ)v_control.x / 64,
                          (ਦੁੱਗਣਾ)v_control.y / 64 ));
              error = func_interface->conic_ਮੰਜ਼ਿਲ( &v_control, &vec, user );
              ਜੇ ( error )
                ਜਾਓ Exit;
              ਜਾਰੀ;
            }

            ਜੇ ( ਟੈਗ != FT_ਵਕਰ_ਟੈਗ_CONIC )
              ਜਾਓ Invalid_Outline;

            v_ਵਿਚਕਾਰ.x = ( v_control.x + vec.x ) / 2;
            v_ਵਿਚਕਾਰ.y = ( v_control.y + vec.y ) / 2;

            FT_TRACE5(( "  conic to (%.2f, %.2f)"
                        " with control (%.2f, %.2f)\n",
                        (ਦੁੱਗਣਾ)v_ਵਿਚਕਾਰ.x / 64,
                        (ਦੁੱਗਣਾ)v_ਵਿਚਕਾਰ.y / 64,
                        (ਦੁੱਗਣਾ)v_control.x / 64,
                        (ਦੁੱਗਣਾ)v_control.y / 64 ));
            error = func_interface->conic_ਮੰਜ਼ਿਲ( &v_control, &v_ਵਿਚਕਾਰ, user );
            ਜੇ ( error )
              ਜਾਓ Exit;

            v_control = vec;
            ਜਾਓ Do_Conic;
          }

          FT_TRACE5(( "  conic to (%.2f, %.2f)"
                      " with control (%.2f, %.2f)\n",
                      (ਦੁੱਗਣਾ)v_ਸ਼ੁਰੂ.x / 64,
                      (ਦੁੱਗਣਾ)v_ਸ਼ੁਰੂ.y / 64,
                      (ਦੁੱਗਣਾ)v_control.x / 64,
                      (ਦੁੱਗਣਾ)v_control.y / 64 ));
          error = func_interface->conic_ਮੰਜ਼ਿਲ( &v_control, &v_ਸ਼ੁਰੂ, user );
          ਜਾਓ Close;

        ਮੂਲ:  /* FT_CURVE_TAG_CUBIC */
          {
            FT_ਸਦਿਸ਼  vec1, vec2;


            ਜੇ ( ਬਿੰਦੂ + 1 > ਹੱਦਬੰਦੀ                             ||
                 FT_ਵਕਰ_ਟੈਗ( ਟੈਗ_ਸੂਚੀ[1] ) != FT_ਵਕਰ_ਟੈਗ_CUBIC )
              ਜਾਓ Invalid_Outline;

            ਬਿੰਦੂ += 2;
            ਟੈਗ_ਸੂਚੀ  += 2;

            vec1.x = SCALED( ਬਿੰਦੂ[-2].x );
            vec1.y = SCALED( ਬਿੰਦੂ[-2].y );

            vec2.x = SCALED( ਬਿੰਦੂ[-1].x );
            vec2.y = SCALED( ਬਿੰਦੂ[-1].y );

            ਜੇ ( ਬਿੰਦੂ <= ਹੱਦਬੰਦੀ )
            {
              FT_ਸਦਿਸ਼  vec;
              FT_ਸਦਿਸ਼  v_ਵਿਚਕਾਰ;


              ਟੈਗ = FT_ਵਕਰ_ਟੈਗ( ਟੈਗ_ਸੂਚੀ[0] );

              vec.x = SCALED( ਬਿੰਦੂ->x );
              vec.y = SCALED( ਬਿੰਦੂ->y );

              ਜੇ ( ਟੈਗ == FT_ਵਕਰ_ਟੈਗ_CUBIC )
              {
                v_ਵਿਚਕਾਰ.x = ( vec2.x + vec.x ) / 2;
                v_ਵਿਚਕਾਰ.y = ( vec2.y + vec.y ) / 2;

                vec = v_ਵਿਚਕਾਰ;
              }
              ਨਹੀਂਤਾਂ ਜੇ ( ਟੈਗ != FT_ਵਕਰ_ਟੈਗ_ON )
                ਜਾਓ Invalid_Outline;

              FT_TRACE5(( "  cubic to (%.2f, %.2f)"
                          " with controls (%.2f, %.2f) and (%.2f, %.2f)\n",
                          (ਦੁੱਗਣਾ)vec.x / 64,
                          (ਦੁੱਗਣਾ)vec.y / 64,
                          (ਦੁੱਗਣਾ)vec1.x / 64,
                          (ਦੁੱਗਣਾ)vec1.y / 64,
                          (ਦੁੱਗਣਾ)vec2.x / 64,
                          (ਦੁੱਗਣਾ)vec2.y / 64 ));
              error = func_interface->cubic_ਮੰਜ਼ਿਲ( &vec1, &vec2, &vec, user );
              ਜੇ ( error )
                ਜਾਓ Exit;

              ਜੇ ( ਟੈਗ == FT_ਵਕਰ_ਟੈਗ_CUBIC )
              {
                ਬਿੰਦੂ--;
                ਟੈਗ_ਸੂਚੀ--;
              }
              ਜਾਰੀ;
            }

            FT_TRACE5(( "  cubic to (%.2f, %.2f)"
                        " with controls (%.2f, %.2f) and (%.2f, %.2f)\n",
                        (ਦੁੱਗਣਾ)v_ਸ਼ੁਰੂ.x / 64,
                        (ਦੁੱਗਣਾ)v_ਸ਼ੁਰੂ.y / 64,
                        (ਦੁੱਗਣਾ)vec1.x / 64,
                        (ਦੁੱਗਣਾ)vec1.y / 64,
                        (ਦੁੱਗਣਾ)vec2.x / 64,
                        (ਦੁੱਗਣਾ)vec2.y / 64 ));
            error = func_interface->cubic_ਮੰਜ਼ਿਲ( &vec1, &vec2, &v_ਸ਼ੁਰੂ, user );
            ਜਾਓ Close;
          }
        }
      }

      /* close the contour with a line segment */
      FT_TRACE5(( "  line to (%.2f, %.2f)\n",
                  (ਦੁੱਗਣਾ)v_ਸ਼ੁਰੂ.x / 64, (ਦੁੱਗਣਾ)v_ਸ਼ੁਰੂ.y / 64 ));
      error = func_interface->ਸਤਰ_ਮੰਜ਼ਿਲ( &v_ਸ਼ੁਰੂ, user );

    Close:
      ਜੇ ( error )
        ਜਾਓ Exit;
    }

    FT_TRACE5(( "FT_Outline_Decompose: Done\n" ));
    ਵਾਪਸ FT_Err_Ok;

  Invalid_Outline:
    error = FT_THROW( Invalid_Outline );
    /* fall through */

  Exit:
    FT_TRACE5(( "FT_Outline_Decompose: Error 0x%x\n", error ));
    ਵਾਪਸ error;
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Outline_New( FT_Library   library,
                  FT_UInt      ਅੰਕ_Points,
                  FT_Int       ਅੰਕ_Contours,
                  FT_Outline  *anoutline )
  {
    FT_Error   error;
    FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ;


    ਜੇ ( !library )
      ਵਾਪਸ FT_THROW( Invalid_Library_Handle );

    ਸਮ੍ਰਿਤੀ = library->ਸਮ੍ਰਿਤੀ;

    ਜੇ ( !anoutline || !ਸਮ੍ਰਿਤੀ )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    *anoutline = null_outline;

    ਜੇ ( ਅੰਕ_Contours < 0                  ||
         (FT_UInt)ਅੰਕ_Contours > ਅੰਕ_Points )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    ਜੇ ( ਅੰਕ_Points > FT_OUTLINE_POINTS_MAX )
      ਵਾਪਸ FT_THROW( Array_Too_Large );

    ਜੇ ( FT_NEW_ARRAY( anoutline->points,   ਅੰਕ_Points   ) ||
         FT_NEW_ARRAY( anoutline->ਟੈਗ_ਸੂਚੀ,     ਅੰਕ_Points   ) ||
         FT_NEW_ARRAY( anoutline->contours, ਅੰਕ_Contours ) )
      ਜਾਓ Fail;

    anoutline->n_points    = (FT_UShort)ਅੰਕ_Points;
    anoutline->n_contours  = (FT_UShort)ਅੰਕ_Contours;
    anoutline->flags      |= FT_OUTLINE_OWNER;

    ਵਾਪਸ FT_Err_Ok;

  Fail:
    anoutline->flags |= FT_OUTLINE_OWNER;
    FT_Outline_ਪੂਰਾ( library, anoutline );

    ਵਾਪਸ error;
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Outline_Check( FT_Outline*  outline )
  {
    ਜੇ ( outline )
    {
      FT_Int  n_points   = outline->n_points;
      FT_Int  n_contours = outline->n_contours;
      FT_Int  end0, ਸਿਰਾ;
      FT_Int  n;


      FT_TRACE5(( "FT_Outline_Check: contours = %d, points = %d\n",
                  n_contours, n_points ));
      /* empty glyph? */
      ਜੇ ( n_points == 0 && n_contours == 0 )
        ਵਾਪਸ FT_Err_Ok;

      /* check point and contour counts */
      ਜੇ ( n_points == 0 || n_contours == 0 )
        ਜਾਓ Bad;

      end0 = -1;
      ਲਈ ( n = 0; n < n_contours; n++ )
      {
        ਸਿਰਾ = outline->contours[n];

        /* note that we don't accept empty contours */
        ਜੇ ( ਸਿਰਾ <= end0 || ਸਿਰਾ >= n_points )
          ਜਾਓ Bad;

        end0 = ਸਿਰਾ;
      }

      ਜੇ ( end0 != n_points - 1 )
        ਜਾਓ Bad;

      /* XXX: check the tags array */
      ਵਾਪਸ FT_Err_Ok;
    }

  Bad:
    ਵਾਪਸ FT_THROW( Invalid_Outline );
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Outline_Copy( ਅਚਲ FT_Outline*  ਸਰੋਤ,
                   FT_Outline        *ਨਿਸ਼ਾਨਾ )
  {
    FT_Int  is_owner;


    ਜੇ ( !ਸਰੋਤ || !ਨਿਸ਼ਾਨਾ )
      ਵਾਪਸ FT_THROW( Invalid_Outline );

    ਜੇ ( ਸਰੋਤ->n_points   != ਨਿਸ਼ਾਨਾ->n_points   ||
         ਸਰੋਤ->n_contours != ਨਿਸ਼ਾਨਾ->n_contours )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    ਜੇ ( ਸਰੋਤ == ਨਿਸ਼ਾਨਾ )
      ਵਾਪਸ FT_Err_Ok;

    ਜੇ ( ਸਰੋਤ->n_points )
    {
      FT_ARRAY_COPY( ਨਿਸ਼ਾਨਾ->points, ਸਰੋਤ->points, ਸਰੋਤ->n_points );
      FT_ARRAY_COPY( ਨਿਸ਼ਾਨਾ->ਟੈਗ_ਸੂਚੀ,   ਸਰੋਤ->ਟੈਗ_ਸੂਚੀ,   ਸਰੋਤ->n_points );
    }

    ਜੇ ( ਸਰੋਤ->n_contours )
      FT_ARRAY_COPY( ਨਿਸ਼ਾਨਾ->contours, ਸਰੋਤ->contours, ਸਰੋਤ->n_contours );

    /* copy all flags, except the `FT_OUTLINE_OWNER' one */
    is_owner      = ਨਿਸ਼ਾਨਾ->flags & FT_OUTLINE_OWNER;
    ਨਿਸ਼ਾਨਾ->flags = ਸਰੋਤ->flags;

    ਨਿਸ਼ਾਨਾ->flags &= ~FT_OUTLINE_OWNER;
    ਨਿਸ਼ਾਨਾ->flags |= is_owner;

    ਵਾਪਸ FT_Err_Ok;
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Outline_ਪੂਰਾ( FT_Library   library,
                   FT_Outline*  outline )
  {
    FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ;


    ਜੇ ( !library )
      ਵਾਪਸ FT_THROW( Invalid_Library_Handle );

    ਜੇ ( !outline )
      ਵਾਪਸ FT_THROW( Invalid_Outline );

    ਸਮ੍ਰਿਤੀ = library->ਸਮ੍ਰਿਤੀ;

    ਜੇ ( !ਸਮ੍ਰਿਤੀ )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    ਜੇ ( outline->flags & FT_OUTLINE_OWNER )
    {
      FT_FREE( outline->points   );
      FT_FREE( outline->ਟੈਗ_ਸੂਚੀ     );
      FT_FREE( outline->contours );
    }
    *outline = null_outline;

    ਵਾਪਸ FT_Err_Ok;
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( ਖਾਲੀ )
  FT_Outline_Get_CBox( ਅਚਲ FT_Outline*  outline,
                       FT_BBox           *acbox )
  {
    FT_Pos  xMin, yMin, xMax, yMax;


    ਜੇ ( outline && acbox )
    {
      ਜੇ ( outline->n_points == 0 )
      {
        xMin = 0;
        yMin = 0;
        xMax = 0;
        yMax = 0;
      }
      ਨਹੀਂਤਾਂ
      {
        FT_ਸਦਿਸ਼*  vec   = outline->points;
        FT_ਸਦਿਸ਼*  ਹੱਦਬੰਦੀ = vec + outline->n_points;


        xMin = xMax = vec->x;
        yMin = yMax = vec->y;
        vec++;

        ਲਈ ( ; vec < ਹੱਦਬੰਦੀ; vec++ )
        {
          FT_Pos  x, y;


          x = vec->x;
          ਜੇ ( x < xMin ) xMin = x;
          ਜੇ ( x > xMax ) xMax = x;

          y = vec->y;
          ਜੇ ( y < yMin ) yMin = y;
          ਜੇ ( y > yMax ) yMax = y;
        }
      }
      acbox->xMin = xMin;
      acbox->xMax = xMax;
      acbox->yMin = yMin;
      acbox->yMax = yMax;
    }
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( ਖਾਲੀ )
  FT_Outline_Translate( ਅਚਲ FT_Outline*  outline,
                        FT_Pos             x_ਵਿੱਥ,
                        FT_Pos             y_ਵਿੱਥ )
  {
    FT_UShort   n;
    FT_ਸਦਿਸ਼*  vec;


    ਜੇ ( !outline )
      ਵਾਪਸ;

    vec = outline->points;

    ਲਈ ( n = 0; n < outline->n_points; n++ )
    {
      vec->x = ਜੋੜੋ_LONG( vec->x, x_ਵਿੱਥ );
      vec->y = ਜੋੜੋ_LONG( vec->y, y_ਵਿੱਥ );
      vec++;
    }
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( ਖਾਲੀ )
  FT_Outline_ਉਲਟ( FT_Outline*  outline )
  {
    FT_UShort  n;
    FT_Int     ਪਹਿਲਾ, ਆਖਰੀ;


    ਜੇ ( !outline )
      ਵਾਪਸ;

    ਆਖਰੀ = -1;
    ਲਈ ( n = 0; n < outline->n_contours; n++ )
    {
      ਪਹਿਲਾ = ਆਖਰੀ + 1;
      ਆਖਰੀ  = outline->contours[n];

      /* Keep the first point unless it is a cubic control.  In that */
      /* case, reverse the entire contour to preserve control pairs, */
      /* including contours with no explicit on-curve points.        */
      ਜੇ ( FT_ਵਕਰ_ਟੈਗ( outline->ਟੈਗ_ਸੂਚੀ[ਪਹਿਲਾ] ) != FT_ਵਕਰ_ਟੈਗ_CUBIC )
        ਪਹਿਲਾ++;

      /* reverse point table */
      {
        FT_ਸਦਿਸ਼*  p = outline->points + ਪਹਿਲਾ;
        FT_ਸਦਿਸ਼*  q = outline->points + ਆਖਰੀ;
        FT_ਸਦਿਸ਼   ਅਦਲਾ_ਬਦਲੀ;


        ਜਦੋਂ ( p < q )
        {
          ਅਦਲਾ_ਬਦਲੀ = *p;
          *p   = *q;
          *q   = ਅਦਲਾ_ਬਦਲੀ;
          p++;
          q--;
        }
      }

      /* reverse tags table */
      {
        FT_Byte*  p = outline->ਟੈਗ_ਸੂਚੀ + ਪਹਿਲਾ;
        FT_Byte*  q = outline->ਟੈਗ_ਸੂਚੀ + ਆਖਰੀ;


        ਜਦੋਂ ( p < q )
        {
          FT_Byte  ਅਦਲਾ_ਬਦਲੀ;


          ਅਦਲਾ_ਬਦਲੀ = *p;
          *p   = *q;
          *q   = ਅਦਲਾ_ਬਦਲੀ;
          p++;
          q--;
        }
      }
    }

    outline->flags ^= FT_OUTLINE_ਉਲਟ_FILL;
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Outline_Render( FT_Library         library,
                     FT_Outline*        outline,
                     FT_Raster_Params*  params )
  {
    FT_Error     error;
    FT_Renderer  renderer;
    FT_List_ਗੰਢ  ਗੰਢ;
    FT_BBox      cbox;


    ਜੇ ( !library )
      ਵਾਪਸ FT_THROW( Invalid_Library_Handle );

    ਜੇ ( !outline )
      ਵਾਪਸ FT_THROW( Invalid_Outline );

    ਜੇ ( !params )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    FT_Outline_Get_CBox( outline, &cbox );
    ਜੇ ( cbox.xMin < -0x1000000L || cbox.yMin < -0x1000000L ||
         cbox.xMax >  0x1000000L || cbox.yMax >  0x1000000L )
      ਵਾਪਸ FT_THROW( Invalid_Outline );

    renderer = library->cur_renderer;
    ਗੰਢ     = library->renderers.ਸਿਰ;

    params->ਸਰੋਤ = (ਖਾਲੀ*)outline;

    /* preset clip_box for direct mode */
    ਜੇ ( params->flags & FT_RASTER_FLAG_ਸਿੱਧਾ    &&
         !( params->flags & FT_RASTER_FLAG_CLIP ) )
    {
      params->clip_box.xMin = cbox.xMin >> 6;
      params->clip_box.yMin = cbox.yMin >> 6;
      params->clip_box.xMax = ( cbox.xMax + 63 ) >> 6;
      params->clip_box.yMax = ( cbox.yMax + 63 ) >> 6;
    }

    error = FT_ERR( Cannot_Render_Glyph );
    ਜਦੋਂ ( renderer )
    {
      error = renderer->raster_render( renderer->raster, params );
      ਜੇ ( !error || FT_ERR_NEQ( error, Cannot_Render_Glyph ) )
        ਰੋਕੋ;

      /* FT_Err_Cannot_Render_Glyph is returned if the render mode   */
      /* is unsupported by the current renderer for this glyph image */
      /* format                                                      */

      /* now, look for another renderer that supports the same */
      /* format                                                */
      renderer = FT_Lookup_Renderer( library, FT_GLYPH_FORMAT_OUTLINE,
                                     &ਗੰਢ );
    }

    ਵਾਪਸ error;
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Outline_Get_Bitmap( FT_Library        library,
                         FT_Outline*       outline,
                         ਅਚਲ FT_Bitmap  *abitmap )
  {
    FT_Raster_Params  params;


    ਜੇ ( !abitmap )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    /* other checks are delayed to `FT_Outline_Render' */

    params.ਨਿਸ਼ਾਨਾ = abitmap;
    params.flags  = 0;

    ਜੇ ( abitmap->pixel_ਬਹੁਲਕ == FT_PIXEL_ਬਹੁਲਕ_ਸਲੇਟੀ  ||
         abitmap->pixel_ਬਹੁਲਕ == FT_PIXEL_ਬਹੁਲਕ_LCD   ||
         abitmap->pixel_ਬਹੁਲਕ == FT_PIXEL_ਬਹੁਲਕ_LCD_V )
      params.flags |= FT_RASTER_FLAG_AA;

    ਵਾਪਸ FT_Outline_Render( library, outline, &params );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( ਖਾਲੀ )
  FT_ਸਦਿਸ਼_Transform( FT_ਸਦਿਸ਼*        ਸਦਿਸ਼,
                       ਅਚਲ FT_ਮੈਟ੍ਰਿਕਸ*  ਮੈਟ੍ਰਿਕਸ )
  {
    FT_Pos  xz, yz;


    ਜੇ ( !ਸਦਿਸ਼ || !ਮੈਟ੍ਰਿਕਸ )
      ਵਾਪਸ;

    xz = FT_MulFix( ਸਦਿਸ਼->x, ਮੈਟ੍ਰਿਕਸ->xx ) +
         FT_MulFix( ਸਦਿਸ਼->y, ਮੈਟ੍ਰਿਕਸ->xy );

    yz = FT_MulFix( ਸਦਿਸ਼->x, ਮੈਟ੍ਰਿਕਸ->yx ) +
         FT_MulFix( ਸਦਿਸ਼->y, ਮੈਟ੍ਰਿਕਸ->yy );

    ਸਦਿਸ਼->x = xz;
    ਸਦਿਸ਼->y = yz;
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( ਖਾਲੀ )
  FT_Outline_Transform( ਅਚਲ FT_Outline*  outline,
                        ਅਚਲ FT_ਮੈਟ੍ਰਿਕਸ*   ਮੈਟ੍ਰਿਕਸ )
  {
    FT_ਸਦਿਸ਼*  vec;
    FT_ਸਦਿਸ਼*  ਹੱਦਬੰਦੀ;


    ਜੇ ( !outline || !ਮੈਟ੍ਰਿਕਸ || !outline->points )
      ਵਾਪਸ;

    vec   = outline->points;
    ਹੱਦਬੰਦੀ = vec + outline->n_points;

    ਲਈ ( ; vec < ਹੱਦਬੰਦੀ; vec++ )
      FT_ਸਦਿਸ਼_Transform( vec, ਮੈਟ੍ਰਿਕਸ );
  }


#ਜੇ 0

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FT_OUTLINE_GET_CONTOUR( outline, c, ਪਹਿਲਾ, ਆਖਰੀ )  \
  ਕਰੋ                                                       \
  {                                                        \
    (ਪਹਿਲਾ) = ( c > 0 ) ? (outline)->points +              \
                            (outline)->contours[c - 1] + 1 \
                        : (outline)->points;               \
    (ਆਖਰੀ) = (outline)->points + (outline)->contours[c];   \
  } ਜਦੋਂ ( 0 )


  /* Is a point in some contour?                     */
  /*                                                 */
  /* We treat every point of the contour as if it    */
  /* it were ON.  That is, we allow false positives, */
  /* but disallow false negatives.  (XXX really?)    */
  ਸਥਿਰ FT_Bool
  ft_contour_has( FT_Outline*  outline,
                  FT_Short     c,
                  FT_ਸਦਿਸ਼*   ਬਿੰਦੂ )
  {
    FT_ਸਦਿਸ਼*  ਪਹਿਲਾ;
    FT_ਸਦਿਸ਼*  ਆਖਰੀ;
    FT_ਸਦਿਸ਼*  a;
    FT_ਸਦਿਸ਼*  b;
    FT_UInt     n = 0;


    FT_OUTLINE_GET_CONTOUR( outline, c, ਪਹਿਲਾ, ਆਖਰੀ );

    ਲਈ ( a = ਪਹਿਲਾ; a <= ਆਖਰੀ; a++ )
    {
      FT_Pos  x;
      FT_Int  intersect;


      b = ( a == ਆਖਰੀ ) ? ਪਹਿਲਾ : a + 1;

      intersect = ( a->y - ਬਿੰਦੂ->y ) ^ ( b->y - ਬਿੰਦੂ->y );

      /* a and b are on the same side */
      ਜੇ ( intersect >= 0 )
      {
        ਜੇ ( intersect == 0 && a->y == ਬਿੰਦੂ->y )
        {
          ਜੇ ( ( a->x <= ਬਿੰਦੂ->x && b->x >= ਬਿੰਦੂ->x ) ||
               ( a->x >= ਬਿੰਦੂ->x && b->x <= ਬਿੰਦੂ->x ) )
            ਵਾਪਸ 1;
        }

        ਜਾਰੀ;
      }

      x = a->x + ( b->x - a->x ) * (ਬਿੰਦੂ->y - a->y ) / ( b->y - a->y );

      ਜੇ ( x < ਬਿੰਦੂ->x )
        n++;
      ਨਹੀਂਤਾਂ ਜੇ ( x == ਬਿੰਦੂ->x )
        ਵਾਪਸ 1;
    }

    ਵਾਪਸ n & 1;
  }


  ਸਥਿਰ FT_Bool
  ft_contour_enclosed( FT_Outline*  outline,
                       FT_UShort    c )
  {
    FT_ਸਦਿਸ਼*  ਪਹਿਲਾ;
    FT_ਸਦਿਸ਼*  ਆਖਰੀ;
    FT_Short    i;


    FT_OUTLINE_GET_CONTOUR( outline, c, ਪਹਿਲਾ, ਆਖਰੀ );

    ਲਈ ( i = 0; i < outline->n_contours; i++ )
    {
      ਜੇ ( i != c && ft_contour_has( outline, i, ਪਹਿਲਾ ) )
      {
        FT_ਸਦਿਸ਼*  pt;


        ਲਈ ( pt = ਪਹਿਲਾ + 1; pt <= ਆਖਰੀ; pt++ )
          ਜੇ ( !ft_contour_has( outline, i, pt ) )
            ਵਾਪਸ 0;

        ਵਾਪਸ 1;
      }
    }

    ਵਾਪਸ 0;
  }


  /* This version differs from the public one in that each */
  /* part (contour not enclosed in another contour) of the */
  /* outline is checked for orientation.  This is          */
  /* necessary for some buggy CJK fonts.                   */
  ਸਥਿਰ FT_Orientation
  ft_outline_get_orientation( FT_Outline*  outline )
  {
    FT_Short        i;
    FT_ਸਦਿਸ਼*      ਪਹਿਲਾ;
    FT_ਸਦਿਸ਼*      ਆਖਰੀ;
    FT_Orientation  orient = FT_ORIENTATION_NONE;


    ਪਹਿਲਾ = outline->points;
    ਲਈ ( i = 0; i < outline->n_contours; i++, ਪਹਿਲਾ = ਆਖਰੀ + 1 )
    {
      FT_ਸਦਿਸ਼*  ਬਿੰਦੂ;
      FT_ਸਦਿਸ਼*  xmin_ਬਿੰਦੂ;
      FT_Pos      xmin;


      ਆਖਰੀ = outline->points + outline->contours[i];

      /* skip degenerate contours */
      ਜੇ ( ਆਖਰੀ < ਪਹਿਲਾ + 2 )
        ਜਾਰੀ;

      ਜੇ ( ft_contour_enclosed( outline, i ) )
        ਜਾਰੀ;

      xmin       = ਪਹਿਲਾ->x;
      xmin_ਬਿੰਦੂ = ਪਹਿਲਾ;

      ਲਈ ( ਬਿੰਦੂ = ਪਹਿਲਾ + 1; ਬਿੰਦੂ <= ਆਖਰੀ; ਬਿੰਦੂ++ )
      {
        ਜੇ ( ਬਿੰਦੂ->x < xmin )
        {
          xmin       = ਬਿੰਦੂ->x;
          xmin_ਬਿੰਦੂ = ਬਿੰਦੂ;
        }
      }

      /* check the orientation of the contour */
      {
        FT_ਸਦਿਸ਼*      prev;
        FT_ਸਦਿਸ਼*      ਅਗਲਾ;
        FT_Orientation  o;


        prev = ( xmin_ਬਿੰਦੂ == ਪਹਿਲਾ ) ? ਆਖਰੀ : xmin_ਬਿੰਦੂ - 1;
        ਅਗਲਾ = ( xmin_ਬਿੰਦੂ == ਆਖਰੀ ) ? ਪਹਿਲਾ : xmin_ਬਿੰਦੂ + 1;

        ਜੇ ( FT_Atan2( prev->x - xmin_ਬਿੰਦੂ->x, prev->y - xmin_ਬਿੰਦੂ->y ) >
             FT_Atan2( ਅਗਲਾ->x - xmin_ਬਿੰਦੂ->x, ਅਗਲਾ->y - xmin_ਬਿੰਦੂ->y ) )
          o = FT_ORIENTATION_POSTSCRIPT;
        ਨਹੀਂਤਾਂ
          o = FT_ORIENTATION_TRUETYPE;

        ਜੇ ( orient == FT_ORIENTATION_NONE )
          orient = o;
        ਨਹੀਂਤਾਂ ਜੇ ( orient != o )
          ਵਾਪਸ FT_ORIENTATION_NONE;
      }
    }

    ਵਾਪਸ orient;
  }

#ਜੇ_ਅੰਤ /* 0 */


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Outline_Embolden( FT_Outline*  outline,
                       FT_Pos       strength )
  {
    ਵਾਪਸ FT_Outline_EmboldenXY( outline, strength, strength );
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Outline_EmboldenXY( FT_Outline*  outline,
                         FT_Pos       xstrength,
                         FT_Pos       ystrength )
  {
    FT_ਸਦਿਸ਼*      points;
    FT_Int          c, ਪਹਿਲਾ, ਆਖਰੀ;
    FT_Orientation  orientation;


    ਜੇ ( !outline )
      ਵਾਪਸ FT_THROW( Invalid_Outline );

    xstrength /= 2;
    ystrength /= 2;
    ਜੇ ( xstrength == 0 && ystrength == 0 )
      ਵਾਪਸ FT_Err_Ok;

    orientation = FT_Outline_Get_Orientation( outline );
    ਜੇ ( orientation == FT_ORIENTATION_NONE )
    {
      ਜੇ ( outline->n_contours )
        ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );
      ਨਹੀਂਤਾਂ
        ਵਾਪਸ FT_Err_Ok;
    }

    points = outline->points;

    ਆਖਰੀ = -1;
    ਲਈ ( c = 0; c < outline->n_contours; c++ )
    {
      FT_ਸਦਿਸ਼  ਵਿੱਚ, ਬਾਹਰਲਾ, anchor, ਖਿਸਕਾਓ;
      FT_Fixed   l_ਵਿੱਚ, l_ਬਾਹਰਲਾ, l_anchor = 0, l, q, d;
      FT_Int     i, j, k;


      ਪਹਿਲਾ = ਆਖਰੀ + 1;
      ਆਖਰੀ  = outline->contours[c];
      l_ਵਿੱਚ  = 0;

      /* pacify compiler */
      ਵਿੱਚ.x = ਵਿੱਚ.y = anchor.x = anchor.y = 0;

      /* Counter j cycles though the points; counter i advances only  */
      /* when points are moved; anchor k marks the first moved point. */
      ਲਈ ( i = ਆਖਰੀ, j = ਪਹਿਲਾ, k = -1;
            j != i && i != k;
            j = j < ਆਖਰੀ ? j + 1 : ਪਹਿਲਾ )
      {
        ਜੇ ( j != k )
        {
          ਬਾਹਰਲਾ.x = points[j].x - points[i].x;
          ਬਾਹਰਲਾ.y = points[j].y - points[i].y;
          l_ਬਾਹਰਲਾ = (FT_Fixed)FT_ਸਦਿਸ਼_Norm_Len( &ਬਾਹਰਲਾ );

          ਜੇ ( l_ਬਾਹਰਲਾ == 0 )
            ਜਾਰੀ;
        }
        ਨਹੀਂਤਾਂ
        {
          ਬਾਹਰਲਾ   = anchor;
          l_ਬਾਹਰਲਾ = l_anchor;
        }

        ਜੇ ( l_ਵਿੱਚ != 0 )
        {
          ਜੇ ( k < 0 )
          {
            k        = i;
            anchor   = ਵਿੱਚ;
            l_anchor = l_ਵਿੱਚ;
          }

          d = FT_MulFix( ਵਿੱਚ.x, ਬਾਹਰਲਾ.x ) + FT_MulFix( ਵਿੱਚ.y, ਬਾਹਰਲਾ.y );

          /* shift only if turn is less than ~160 degrees */
          ਜੇ ( d > -0xF000L )
          {
            d = d + 0x10000L;

            /* shift components along lateral bisector in proper orientation */
            ਖਿਸਕਾਓ.x = ਵਿੱਚ.y + ਬਾਹਰਲਾ.y;
            ਖਿਸਕਾਓ.y = ਵਿੱਚ.x + ਬਾਹਰਲਾ.x;

            ਜੇ ( orientation == FT_ORIENTATION_TRUETYPE )
              ਖਿਸਕਾਓ.x = -ਖਿਸਕਾਓ.x;
            ਨਹੀਂਤਾਂ
              ਖਿਸਕਾਓ.y = -ਖਿਸਕਾਓ.y;

            /* restrict shift magnitude to better handle collapsing segments */
            q = FT_MulFix( ਬਾਹਰਲਾ.x, ਵਿੱਚ.y ) - FT_MulFix( ਬਾਹਰਲਾ.y, ਵਿੱਚ.x );
            ਜੇ ( orientation == FT_ORIENTATION_TRUETYPE )
              q = -q;

            l = FT_MIN( l_ਵਿੱਚ, l_ਬਾਹਰਲਾ );

            /* non-strict inequalities avoid divide-by-zero when q == l == 0 */
            ਜੇ ( FT_MulFix( xstrength, q ) <= FT_MulFix( l, d ) )
              ਖਿਸਕਾਓ.x = FT_MulDiv( ਖਿਸਕਾਓ.x, xstrength, d );
            ਨਹੀਂਤਾਂ
              ਖਿਸਕਾਓ.x = FT_MulDiv( ਖਿਸਕਾਓ.x, l, q );


            ਜੇ ( FT_MulFix( ystrength, q ) <= FT_MulFix( l, d ) )
              ਖਿਸਕਾਓ.y = FT_MulDiv( ਖਿਸਕਾਓ.y, ystrength, d );
            ਨਹੀਂਤਾਂ
              ਖਿਸਕਾਓ.y = FT_MulDiv( ਖਿਸਕਾਓ.y, l, q );
          }
          ਨਹੀਂਤਾਂ
            ਖਿਸਕਾਓ.x = ਖਿਸਕਾਓ.y = 0;

          ਲਈ ( ;
                i != j;
                i = i < ਆਖਰੀ ? i + 1 : ਪਹਿਲਾ )
          {
            points[i].x += xstrength + ਖਿਸਕਾਓ.x;
            points[i].y += ystrength + ਖਿਸਕਾਓ.y;
          }
        }
        ਨਹੀਂਤਾਂ
          i = j;

        ਵਿੱਚ   = ਬਾਹਰਲਾ;
        l_ਵਿੱਚ = l_ਬਾਹਰਲਾ;
      }
    }

    ਵਾਪਸ FT_Err_Ok;
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Orientation )
  FT_Outline_Get_Orientation( FT_Outline*  outline )
  {
    FT_ਸਦਿਸ਼*  points;
    FT_Int      c, n, ਪਹਿਲਾ, ਆਖਰੀ;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ FT_INT64
    FT_Int64    area = 0;
#ਨਹੀਂਤਾਂ
    FT_BBox     cbox = { 0, 0, 0, 0 };
    FT_Int      xshift, yshift;
    FT_ਸਦਿਸ਼   v_prev, v_cur;
    FT_Pos      area = 0;
#ਜੇ_ਅੰਤ


    ਜੇ ( !outline || outline->n_points <= 0 )
      ਵਾਪਸ FT_ORIENTATION_TRUETYPE;

    /* We use the nonzero winding rule to find the orientation.       */
    /* Since glyph outlines behave much more `regular' than arbitrary */
    /* cubic or quadratic curves, this test deals with the polygon    */
    /* only that is spanned up by the control points.                 */

    points = outline->points;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ FT_INT64

    /* Single-pass shoelace.  The 64-bit accumulator removes the need   */
    /* for the coordinate shift pre-pass (and its `FT_Outline_Get_CBox` */
    /* traversal) that the 32-bit variant below uses.                   */
    ਆਖਰੀ = -1;
    ਲਈ ( c = 0; c < outline->n_contours; c++ )
    {
      FT_Pos  prev_x, prev_y;


      ਪਹਿਲਾ = ਆਖਰੀ + 1;
      ਆਖਰੀ  = outline->contours[c];

      prev_x = points[ਆਖਰੀ].x;
      prev_y = points[ਆਖਰੀ].y;

      ਲਈ ( n = ਪਹਿਲਾ; n <= ਆਖਰੀ; n++ )
      {
        FT_Pos  cur_x = points[n].x;
        FT_Pos  cur_y = points[n].y;


        area = ਜੋੜੋ_INT64( area,
                          MUL_INT64( SUB_INT64( cur_y, prev_y ),
                                     ਜੋੜੋ_INT64( cur_x, prev_x ) ) );

        prev_x = cur_x;
        prev_y = cur_y;
      }
    }

#ਨਹੀਂਤਾਂ /* !FT_INT64 */

    FT_Outline_Get_CBox( outline, &cbox );

    /* Handle collapsed outlines to avoid undefined FT_MSB. */
    ਜੇ ( cbox.xMin == cbox.xMax || cbox.yMin == cbox.yMax )
      ਵਾਪਸ FT_ORIENTATION_NONE;

    /* Reject values large outlines. */
    ਜੇ ( cbox.xMin < -0x1000000L || cbox.yMin < -0x1000000L ||
         cbox.xMax >  0x1000000L || cbox.yMax >  0x1000000L )
      ਵਾਪਸ FT_ORIENTATION_NONE;

    xshift = FT_MSB( (FT_UInt32)( FT_ABS( cbox.xMax ) |
                                  FT_ABS( cbox.xMin ) ) ) - 14;
    xshift = FT_MAX( xshift, 0 );

    yshift = FT_MSB( (FT_UInt32)( cbox.yMax - cbox.yMin ) ) - 14;
    yshift = FT_MAX( yshift, 0 );

    ਆਖਰੀ = -1;
    ਲਈ ( c = 0; c < outline->n_contours; c++ )
    {
      ਪਹਿਲਾ = ਆਖਰੀ + 1;
      ਆਖਰੀ  = outline->contours[c];

      v_prev.x = points[ਆਖਰੀ].x >> xshift;
      v_prev.y = points[ਆਖਰੀ].y >> yshift;

      ਲਈ ( n = ਪਹਿਲਾ; n <= ਆਖਰੀ; n++ )
      {
        v_cur.x = points[n].x >> xshift;
        v_cur.y = points[n].y >> yshift;

        area = ਜੋੜੋ_LONG( area,
                         MUL_LONG( v_cur.y - v_prev.y,
                                   v_cur.x + v_prev.x ) );

        v_prev = v_cur;
      }
    }

#ਜੇ_ਅੰਤ /* FT_INT64 */

    ਜੇ ( area > 0 )
      ਵਾਪਸ FT_ORIENTATION_POSTSCRIPT;
    ਨਹੀਂਤਾਂ ਜੇ ( area < 0 )
      ਵਾਪਸ FT_ORIENTATION_TRUETYPE;
    ਨਹੀਂਤਾਂ
      ਵਾਪਸ FT_ORIENTATION_NONE;
  }


/*ਸਿਰਾ*/
