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


#ಅನ್ತರ್ಭಾವ <freetype/ftoutln.h>
#ಅನ್ತರ್ಭಾವ <freetype/internal/ftobjs.h>
#ಅನ್ತರ್ಭಾವ <freetype/internal/ftcalc.h>
#ಅನ್ತರ್ಭಾವ <freetype/internal/ftdebug.h>
#ಅನ್ತರ್ಭಾವ <freetype/fttrigon.h>


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು  FT_COMPONENT
#ವ್ಯಾಖ್ಯಾನಿಸು FT_COMPONENT  outline


  ಸ್ಥಿರ
  ಸ್ಥಿರಾಂಕ FT_Outline  null_outline = { 0, 0, ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ, 0 };


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Outline_Decompose( FT_Outline*              outline,
                        ಸ್ಥಿರಾಂಕ FT_Outline_Funcs*  func_interface,
                        ಶೂನ್ಯ*                    user )
  {
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು  SCALED
#ವ್ಯಾಖ್ಯಾನಿಸು SCALED( x )  ( (x) * ( 1L << ಸರಿಸು ) - delta )

    FT_ಸದಿಶ   v_ಕೊನೆಯ;
    FT_ಸದಿಶ   v_control;
    FT_ಸದಿಶ   v_ಆರಂಭ;

    FT_ಸದಿಶ*  ಬಿಂದು;
    FT_ಸದಿಶ*  ಎಲ್ಲೆ;
    FT_Byte*    ಟ್ಯಾಗುಗಳು;

    FT_Error    error;

    FT_Int   n;         /* index of contour in outline     */
    FT_Int   ಮೊದಲ;     /* index of first point in contour */
    FT_Int   ಕೊನೆಯ;      /* index of last point in contour  */

    FT_Int   ಟ್ಯಾಗ್;       /* current point's state           */

    FT_Int   ಸರಿಸು;
    FT_Pos   delta;


    ಆದರೆ ( !outline )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_Outline );

    ಆದರೆ ( !func_interface )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

    ಸರಿಸು = func_interface->ಸರಿಸು;
    delta = func_interface->delta;

    ಕೊನೆಯ = -1;
    ಗಾಗಿ ( n = 0; n < outline->n_contours; n++ )
    {
      FT_TRACE5(( "FT_Outline_Decompose: Contour %d\n", n ));

      ಮೊದಲ = ಕೊನೆಯ + 1;
      ಕೊನೆಯ  = outline->contours[n];
      ಆದರೆ ( ಕೊನೆಯ < ಮೊದಲ )
        ಹೋಗು Invalid_Outline;

      ಎಲ್ಲೆ = outline->points + ಕೊನೆಯ;

      v_ಆರಂಭ   = outline->points[ಮೊದಲ];
      v_ಆರಂಭ.x = SCALED( v_ಆರಂಭ.x );
      v_ಆರಂಭ.y = SCALED( v_ಆರಂಭ.y );

      v_ಕೊನೆಯ   = outline->points[ಕೊನೆಯ];
      v_ಕೊನೆಯ.x = SCALED( v_ಕೊನೆಯ.x );
      v_ಕೊನೆಯ.y = SCALED( v_ಕೊನೆಯ.y );

      v_control = v_ಆರಂಭ;

      ಬಿಂದು = outline->points + ಮೊದಲ;
      ಟ್ಯಾಗುಗಳು  = outline->ಟ್ಯಾಗುಗಳು   + ಮೊದಲ;
      ಟ್ಯಾಗ್   = FT_ವಕ್ರ_ಟ್ಯಾಗ್( ಟ್ಯಾಗುಗಳು[0] );

      /* check first point to determine origin */
      ಆದರೆ ( ಟ್ಯಾಗ್ != FT_ವಕ್ರ_ಟ್ಯಾಗ್_ON )
      {
        FT_Int  ಕೊನೆಯ_ಟ್ಯಾಗ್ = FT_ವಕ್ರ_ಟ್ಯಾಗ್( outline->ಟ್ಯಾಗುಗಳು[ಕೊನೆಯ] );


        /* first point is a control point.  Yes, this happens. */
        ಆದರೆ ( ಕೊನೆಯ_ಟ್ಯಾಗ್ == FT_ವಕ್ರ_ಟ್ಯಾಗ್_ON )
        {
          /* start at last point if it is on the curve */
          v_ಆರಂಭ = v_ಕೊನೆಯ;
          ಎಲ್ಲೆ--;
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
          ಆದರೆ ( ಕೊನೆಯ_ಟ್ಯಾಗ್ != ಟ್ಯಾಗ್ )
            ಹೋಗು Invalid_Outline;

          /* if both first and last points are controls,   */
          /* start at their middle and record its position */
          /* for closure                                   */
          v_ಆರಂಭ.x = ( v_ಆರಂಭ.x + v_ಕೊನೆಯ.x ) / 2;
          v_ಆರಂಭ.y = ( v_ಆರಂಭ.y + v_ಕೊನೆಯ.y ) / 2;

          /* v_last = v_start; */
        }
        ಬಿಂದು--;
        ಟ್ಯಾಗುಗಳು--;
      }

      FT_TRACE5(( "  move to (%.2f, %.2f)\n",
                  (ದ್ವಿಗುಣ)v_ಆರಂಭ.x / 64, (ದ್ವಿಗುಣ)v_ಆರಂಭ.y / 64 ));
      error = func_interface->move_ಗಮ್ಯ( &v_ಆರಂಭ, user );
      ಆದರೆ ( error )
        ಹೋಗು Exit;

      ಆಗ ( ಬಿಂದು < ಎಲ್ಲೆ )
      {
        ಬಿಂದು++;
        ಟ್ಯಾಗುಗಳು++;

        ಟ್ಯಾಗ್ = FT_ವಕ್ರ_ಟ್ಯಾಗ್( ಟ್ಯಾಗುಗಳು[0] );
        ಆಯ್ಕೆ ( ಟ್ಯಾಗ್ )
        {
        ಸ್ಥಿತಿ FT_ವಕ್ರ_ಟ್ಯಾಗ್_ON:  /* emit a single line_to */
          {
            FT_ಸದಿಶ  vec;


            vec.x = SCALED( ಬಿಂದು->x );
            vec.y = SCALED( ಬಿಂದು->y );

            FT_TRACE5(( "  line to (%.2f, %.2f)\n",
                        (ದ್ವಿಗುಣ)vec.x / 64, (ದ್ವಿಗುಣ)vec.y / 64 ));
            error = func_interface->ಸಾಲು_ಗಮ್ಯ( &vec, user );
            ಆದರೆ ( error )
              ಹೋಗು Exit;
            ಮುಂದುವರಿ;
          }

        ಸ್ಥಿತಿ FT_ವಕ್ರ_ಟ್ಯಾಗ್_CONIC:  /* consume conic arcs */
          v_control.x = SCALED( ಬಿಂದು->x );
          v_control.y = SCALED( ಬಿಂದು->y );

        Do_Conic:
          ಆದರೆ ( ಬಿಂದು < ಎಲ್ಲೆ )
          {
            FT_ಸದಿಶ  vec;
            FT_ಸದಿಶ  v_ಮಧ್ಯ;


            ಬಿಂದು++;
            ಟ್ಯಾಗುಗಳು++;
            ಟ್ಯಾಗ್ = FT_ವಕ್ರ_ಟ್ಯಾಗ್( ಟ್ಯಾಗುಗಳು[0] );

            vec.x = SCALED( ಬಿಂದು->x );
            vec.y = SCALED( ಬಿಂದು->y );

            ಆದರೆ ( ಟ್ಯಾಗ್ == FT_ವಕ್ರ_ಟ್ಯಾಗ್_ON )
            {
              FT_TRACE5(( "  conic to (%.2f, %.2f)"
                          " with control (%.2f, %.2f)\n",
                          (ದ್ವಿಗುಣ)vec.x / 64,
                          (ದ್ವಿಗುಣ)vec.y / 64,
                          (ದ್ವಿಗುಣ)v_control.x / 64,
                          (ದ್ವಿಗುಣ)v_control.y / 64 ));
              error = func_interface->conic_ಗಮ್ಯ( &v_control, &vec, user );
              ಆದರೆ ( error )
                ಹೋಗು Exit;
              ಮುಂದುವರಿ;
            }

            ಆದರೆ ( ಟ್ಯಾಗ್ != FT_ವಕ್ರ_ಟ್ಯಾಗ್_CONIC )
              ಹೋಗು Invalid_Outline;

            v_ಮಧ್ಯ.x = ( v_control.x + vec.x ) / 2;
            v_ಮಧ್ಯ.y = ( v_control.y + vec.y ) / 2;

            FT_TRACE5(( "  conic to (%.2f, %.2f)"
                        " with control (%.2f, %.2f)\n",
                        (ದ್ವಿಗುಣ)v_ಮಧ್ಯ.x / 64,
                        (ದ್ವಿಗುಣ)v_ಮಧ್ಯ.y / 64,
                        (ದ್ವಿಗುಣ)v_control.x / 64,
                        (ದ್ವಿಗುಣ)v_control.y / 64 ));
            error = func_interface->conic_ಗಮ್ಯ( &v_control, &v_ಮಧ್ಯ, user );
            ಆದರೆ ( error )
              ಹೋಗು Exit;

            v_control = vec;
            ಹೋಗು Do_Conic;
          }

          FT_TRACE5(( "  conic to (%.2f, %.2f)"
                      " with control (%.2f, %.2f)\n",
                      (ದ್ವಿಗುಣ)v_ಆರಂಭ.x / 64,
                      (ದ್ವಿಗುಣ)v_ಆರಂಭ.y / 64,
                      (ದ್ವಿಗುಣ)v_control.x / 64,
                      (ದ್ವಿಗುಣ)v_control.y / 64 ));
          error = func_interface->conic_ಗಮ್ಯ( &v_control, &v_ಆರಂಭ, user );
          ಹೋಗು Close;

        ಪೂರ್ವನಿಯೋಜಿತ:  /* FT_CURVE_TAG_CUBIC */
          {
            FT_ಸದಿಶ  vec1, vec2;


            ಆದರೆ ( ಬಿಂದು + 1 > ಎಲ್ಲೆ                             ||
                 FT_ವಕ್ರ_ಟ್ಯಾಗ್( ಟ್ಯಾಗುಗಳು[1] ) != FT_ವಕ್ರ_ಟ್ಯಾಗ್_CUBIC )
              ಹೋಗು Invalid_Outline;

            ಬಿಂದು += 2;
            ಟ್ಯಾಗುಗಳು  += 2;

            vec1.x = SCALED( ಬಿಂದು[-2].x );
            vec1.y = SCALED( ಬಿಂದು[-2].y );

            vec2.x = SCALED( ಬಿಂದು[-1].x );
            vec2.y = SCALED( ಬಿಂದು[-1].y );

            ಆದರೆ ( ಬಿಂದು <= ಎಲ್ಲೆ )
            {
              FT_ಸದಿಶ  vec;
              FT_ಸದಿಶ  v_ಮಧ್ಯ;


              ಟ್ಯಾಗ್ = FT_ವಕ್ರ_ಟ್ಯಾಗ್( ಟ್ಯಾಗುಗಳು[0] );

              vec.x = SCALED( ಬಿಂದು->x );
              vec.y = SCALED( ಬಿಂದು->y );

              ಆದರೆ ( ಟ್ಯಾಗ್ == FT_ವಕ್ರ_ಟ್ಯಾಗ್_CUBIC )
              {
                v_ಮಧ್ಯ.x = ( vec2.x + vec.x ) / 2;
                v_ಮಧ್ಯ.y = ( vec2.y + vec.y ) / 2;

                vec = v_ಮಧ್ಯ;
              }
              ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ( ಟ್ಯಾಗ್ != FT_ವಕ್ರ_ಟ್ಯಾಗ್_ON )
                ಹೋಗು Invalid_Outline;

              FT_TRACE5(( "  cubic to (%.2f, %.2f)"
                          " with controls (%.2f, %.2f) and (%.2f, %.2f)\n",
                          (ದ್ವಿಗುಣ)vec.x / 64,
                          (ದ್ವಿಗುಣ)vec.y / 64,
                          (ದ್ವಿಗುಣ)vec1.x / 64,
                          (ದ್ವಿಗುಣ)vec1.y / 64,
                          (ದ್ವಿಗುಣ)vec2.x / 64,
                          (ದ್ವಿಗುಣ)vec2.y / 64 ));
              error = func_interface->cubic_ಗಮ್ಯ( &vec1, &vec2, &vec, user );
              ಆದರೆ ( error )
                ಹೋಗು Exit;

              ಆದರೆ ( ಟ್ಯಾಗ್ == FT_ವಕ್ರ_ಟ್ಯಾಗ್_CUBIC )
              {
                ಬಿಂದು--;
                ಟ್ಯಾಗುಗಳು--;
              }
              ಮುಂದುವರಿ;
            }

            FT_TRACE5(( "  cubic to (%.2f, %.2f)"
                        " with controls (%.2f, %.2f) and (%.2f, %.2f)\n",
                        (ದ್ವಿಗುಣ)v_ಆರಂಭ.x / 64,
                        (ದ್ವಿಗುಣ)v_ಆರಂಭ.y / 64,
                        (ದ್ವಿಗುಣ)vec1.x / 64,
                        (ದ್ವಿಗುಣ)vec1.y / 64,
                        (ದ್ವಿಗುಣ)vec2.x / 64,
                        (ದ್ವಿಗುಣ)vec2.y / 64 ));
            error = func_interface->cubic_ಗಮ್ಯ( &vec1, &vec2, &v_ಆರಂಭ, user );
            ಹೋಗು Close;
          }
        }
      }

      /* close the contour with a line segment */
      FT_TRACE5(( "  line to (%.2f, %.2f)\n",
                  (ದ್ವಿಗುಣ)v_ಆರಂಭ.x / 64, (ದ್ವಿಗುಣ)v_ಆರಂಭ.y / 64 ));
      error = func_interface->ಸಾಲು_ಗಮ್ಯ( &v_ಆರಂಭ, user );

    Close:
      ಆದರೆ ( error )
        ಹೋಗು Exit;
    }

    FT_TRACE5(( "FT_Outline_Decompose: Done\n" ));
    ಹಿಂತಿರುಗಿ FT_Err_Ok;

  Invalid_Outline:
    error = FT_THROW( Invalid_Outline );
    /* fall through */

  Exit:
    FT_TRACE5(( "FT_Outline_Decompose: Error 0x%x\n", error ));
    ಹಿಂತಿರುಗಿ error;
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Outline_New( FT_Library   library,
                  FT_UInt      ಅಂಕೆ_Points,
                  FT_Int       ಅಂಕೆ_Contours,
                  FT_Outline  *anoutline )
  {
    FT_Error   error;
    FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ;


    ಆದರೆ ( !library )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_Library_Handle );

    ಸ್ಮರಣೆ = library->ಸ್ಮರಣೆ;

    ಆದರೆ ( !anoutline || !ಸ್ಮರಣೆ )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

    *anoutline = null_outline;

    ಆದರೆ ( ಅಂಕೆ_Contours < 0                  ||
         (FT_UInt)ಅಂಕೆ_Contours > ಅಂಕೆ_Points )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

    ಆದರೆ ( ಅಂಕೆ_Points > FT_OUTLINE_POINTS_MAX )
      ಹಿಂತಿರುಗಿ FT_THROW( Array_Too_Large );

    ಆದರೆ ( FT_NEW_ARRAY( anoutline->points,   ಅಂಕೆ_Points   ) ||
         FT_NEW_ARRAY( anoutline->ಟ್ಯಾಗುಗಳು,     ಅಂಕೆ_Points   ) ||
         FT_NEW_ARRAY( anoutline->contours, ಅಂಕೆ_Contours ) )
      ಹೋಗು Fail;

    anoutline->n_points    = (FT_UShort)ಅಂಕೆ_Points;
    anoutline->n_contours  = (FT_UShort)ಅಂಕೆ_Contours;
    anoutline->flags      |= FT_OUTLINE_OWNER;

    ಹಿಂತಿರುಗಿ FT_Err_Ok;

  Fail:
    anoutline->flags |= FT_OUTLINE_OWNER;
    FT_Outline_ಮುಗಿದ( library, anoutline );

    ಹಿಂತಿರುಗಿ error;
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Outline_Check( FT_Outline*  outline )
  {
    ಆದರೆ ( outline )
    {
      FT_Int  n_points   = outline->n_points;
      FT_Int  n_contours = outline->n_contours;
      FT_Int  end0, ಕೊನೆ;
      FT_Int  n;


      FT_TRACE5(( "FT_Outline_Check: contours = %d, points = %d\n",
                  n_contours, n_points ));
      /* empty glyph? */
      ಆದರೆ ( n_points == 0 && n_contours == 0 )
        ಹಿಂತಿರುಗಿ FT_Err_Ok;

      /* check point and contour counts */
      ಆದರೆ ( n_points == 0 || n_contours == 0 )
        ಹೋಗು Bad;

      end0 = -1;
      ಗಾಗಿ ( n = 0; n < n_contours; n++ )
      {
        ಕೊನೆ = outline->contours[n];

        /* note that we don't accept empty contours */
        ಆದರೆ ( ಕೊನೆ <= end0 || ಕೊನೆ >= n_points )
          ಹೋಗು Bad;

        end0 = ಕೊನೆ;
      }

      ಆದರೆ ( end0 != n_points - 1 )
        ಹೋಗು Bad;

      /* XXX: check the tags array */
      ಹಿಂತಿರುಗಿ FT_Err_Ok;
    }

  Bad:
    ಹಿಂತಿರುಗಿ FT_THROW( Invalid_Outline );
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Outline_Copy( ಸ್ಥಿರಾಂಕ FT_Outline*  ಮೂಲ,
                   FT_Outline        *ಗುರಿ )
  {
    FT_Int  is_owner;


    ಆದರೆ ( !ಮೂಲ || !ಗುರಿ )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_Outline );

    ಆದರೆ ( ಮೂಲ->n_points   != ಗುರಿ->n_points   ||
         ಮೂಲ->n_contours != ಗುರಿ->n_contours )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

    ಆದರೆ ( ಮೂಲ == ಗುರಿ )
      ಹಿಂತಿರುಗಿ FT_Err_Ok;

    ಆದರೆ ( ಮೂಲ->n_points )
    {
      FT_ARRAY_COPY( ಗುರಿ->points, ಮೂಲ->points, ಮೂಲ->n_points );
      FT_ARRAY_COPY( ಗುರಿ->ಟ್ಯಾಗುಗಳು,   ಮೂಲ->ಟ್ಯಾಗುಗಳು,   ಮೂಲ->n_points );
    }

    ಆದರೆ ( ಮೂಲ->n_contours )
      FT_ARRAY_COPY( ಗುರಿ->contours, ಮೂಲ->contours, ಮೂಲ->n_contours );

    /* copy all flags, except the `FT_OUTLINE_OWNER' one */
    is_owner      = ಗುರಿ->flags & FT_OUTLINE_OWNER;
    ಗುರಿ->flags = ಮೂಲ->flags;

    ಗುರಿ->flags &= ~FT_OUTLINE_OWNER;
    ಗುರಿ->flags |= is_owner;

    ಹಿಂತಿರುಗಿ FT_Err_Ok;
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Outline_ಮುಗಿದ( FT_Library   library,
                   FT_Outline*  outline )
  {
    FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ;


    ಆದರೆ ( !library )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_Library_Handle );

    ಆದರೆ ( !outline )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_Outline );

    ಸ್ಮರಣೆ = library->ಸ್ಮರಣೆ;

    ಆದರೆ ( !ಸ್ಮರಣೆ )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

    ಆದರೆ ( outline->flags & FT_OUTLINE_OWNER )
    {
      FT_FREE( outline->points   );
      FT_FREE( outline->ಟ್ಯಾಗುಗಳು     );
      FT_FREE( outline->contours );
    }
    *outline = null_outline;

    ಹಿಂತಿರುಗಿ FT_Err_Ok;
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( ಶೂನ್ಯ )
  FT_Outline_Get_CBox( ಸ್ಥಿರಾಂಕ FT_Outline*  outline,
                       FT_BBox           *acbox )
  {
    FT_Pos  xMin, yMin, xMax, yMax;


    ಆದರೆ ( outline && acbox )
    {
      ಆದರೆ ( outline->n_points == 0 )
      {
        xMin = 0;
        yMin = 0;
        xMax = 0;
        yMax = 0;
      }
      ಇಲ್ಲದಿದ್ದರೆ
      {
        FT_ಸದಿಶ*  vec   = outline->points;
        FT_ಸದಿಶ*  ಎಲ್ಲೆ = vec + outline->n_points;


        xMin = xMax = vec->x;
        yMin = yMax = vec->y;
        vec++;

        ಗಾಗಿ ( ; vec < ಎಲ್ಲೆ; vec++ )
        {
          FT_Pos  x, y;


          x = vec->x;
          ಆದರೆ ( x < xMin ) xMin = x;
          ಆದರೆ ( x > xMax ) xMax = x;

          y = vec->y;
          ಆದರೆ ( y < yMin ) yMin = y;
          ಆದರೆ ( y > yMax ) yMax = y;
        }
      }
      acbox->xMin = xMin;
      acbox->xMax = xMax;
      acbox->yMin = yMin;
      acbox->yMax = yMax;
    }
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( ಶೂನ್ಯ )
  FT_Outline_Translate( ಸ್ಥಿರಾಂಕ FT_Outline*  outline,
                        FT_Pos             x_ಸ್ಥಾನಾಂತರ,
                        FT_Pos             y_ಸ್ಥಾನಾಂತರ )
  {
    FT_UShort   n;
    FT_ಸದಿಶ*  vec;


    ಆದರೆ ( !outline )
      ಹಿಂತಿರುಗಿ;

    vec = outline->points;

    ಗಾಗಿ ( n = 0; n < outline->n_points; n++ )
    {
      vec->x = ಕೂಡಿಸು_LONG( vec->x, x_ಸ್ಥಾನಾಂತರ );
      vec->y = ಕೂಡಿಸು_LONG( vec->y, y_ಸ್ಥಾನಾಂತರ );
      vec++;
    }
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( ಶೂನ್ಯ )
  FT_Outline_ಹಿಮ್ಮುಖ( FT_Outline*  outline )
  {
    FT_UShort  n;
    FT_Int     ಮೊದಲ, ಕೊನೆಯ;


    ಆದರೆ ( !outline )
      ಹಿಂತಿರುಗಿ;

    ಕೊನೆಯ = -1;
    ಗಾಗಿ ( n = 0; n < outline->n_contours; n++ )
    {
      ಮೊದಲ = ಕೊನೆಯ + 1;
      ಕೊನೆಯ  = outline->contours[n];

      /* Keep the first point unless it is a cubic control.  In that */
      /* case, reverse the entire contour to preserve control pairs, */
      /* including contours with no explicit on-curve points.        */
      ಆದರೆ ( FT_ವಕ್ರ_ಟ್ಯಾಗ್( outline->ಟ್ಯಾಗುಗಳು[ಮೊದಲ] ) != FT_ವಕ್ರ_ಟ್ಯಾಗ್_CUBIC )
        ಮೊದಲ++;

      /* reverse point table */
      {
        FT_ಸದಿಶ*  p = outline->points + ಮೊದಲ;
        FT_ಸದಿಶ*  q = outline->points + ಕೊನೆಯ;
        FT_ಸದಿಶ   ಅದಲುಬದಲು;


        ಆಗ ( p < q )
        {
          ಅದಲುಬದಲು = *p;
          *p   = *q;
          *q   = ಅದಲುಬದಲು;
          p++;
          q--;
        }
      }

      /* reverse tags table */
      {
        FT_Byte*  p = outline->ಟ್ಯಾಗುಗಳು + ಮೊದಲ;
        FT_Byte*  q = outline->ಟ್ಯಾಗುಗಳು + ಕೊನೆಯ;


        ಆಗ ( p < q )
        {
          FT_Byte  ಅದಲುಬದಲು;


          ಅದಲುಬದಲು = *p;
          *p   = *q;
          *q   = ಅದಲುಬದಲು;
          p++;
          q--;
        }
      }
    }

    outline->flags ^= FT_OUTLINE_ಹಿಮ್ಮುಖ_FILL;
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Outline_Render( FT_Library         library,
                     FT_Outline*        outline,
                     FT_Raster_Params*  params )
  {
    FT_Error     error;
    FT_Renderer  renderer;
    FT_List_ಗಂಟು  ಗಂಟು;
    FT_BBox      cbox;


    ಆದರೆ ( !library )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_Library_Handle );

    ಆದರೆ ( !outline )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_Outline );

    ಆದರೆ ( !params )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

    FT_Outline_Get_CBox( outline, &cbox );
    ಆದರೆ ( cbox.xMin < -0x1000000L || cbox.yMin < -0x1000000L ||
         cbox.xMax >  0x1000000L || cbox.yMax >  0x1000000L )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_Outline );

    renderer = library->cur_renderer;
    ಗಂಟು     = library->renderers.ತಲೆ;

    params->ಮೂಲ = (ಶೂನ್ಯ*)outline;

    /* preset clip_box for direct mode */
    ಆದರೆ ( params->flags & FT_RASTER_FLAG_ನೇರ    &&
         !( params->flags & FT_RASTER_FLAG_CLIP ) )
    {
      params->clip_box.xMin = cbox.xMin >> 6;
      params->clip_box.yMin = cbox.yMin >> 6;
      params->clip_box.xMax = ( cbox.xMax + 63 ) >> 6;
      params->clip_box.yMax = ( cbox.yMax + 63 ) >> 6;
    }

    error = FT_ERR( Cannot_Render_Glyph );
    ಆಗ ( renderer )
    {
      error = renderer->raster_render( renderer->raster, params );
      ಆದರೆ ( !error || FT_ERR_NEQ( error, Cannot_Render_Glyph ) )
        ನಿಲ್ಲಿಸು;

      /* FT_Err_Cannot_Render_Glyph is returned if the render mode   */
      /* is unsupported by the current renderer for this glyph image */
      /* format                                                      */

      /* now, look for another renderer that supports the same */
      /* format                                                */
      renderer = FT_Lookup_Renderer( library, FT_GLYPH_FORMAT_OUTLINE,
                                     &ಗಂಟು );
    }

    ಹಿಂತಿರುಗಿ error;
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Outline_Get_Bitmap( FT_Library        library,
                         FT_Outline*       outline,
                         ಸ್ಥಿರಾಂಕ FT_Bitmap  *abitmap )
  {
    FT_Raster_Params  params;


    ಆದರೆ ( !abitmap )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

    /* other checks are delayed to `FT_Outline_Render' */

    params.ಗುರಿ = abitmap;
    params.flags  = 0;

    ಆದರೆ ( abitmap->pixel_ಬಹುಲಕ == FT_PIXEL_ಬಹುಲಕ_ಬೂದು  ||
         abitmap->pixel_ಬಹುಲಕ == FT_PIXEL_ಬಹುಲಕ_LCD   ||
         abitmap->pixel_ಬಹುಲಕ == FT_PIXEL_ಬಹುಲಕ_LCD_V )
      params.flags |= FT_RASTER_FLAG_AA;

    ಹಿಂತಿರುಗಿ FT_Outline_Render( library, outline, &params );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( ಶೂನ್ಯ )
  FT_ಸದಿಶ_Transform( FT_ಸದಿಶ*        ಸದಿಶ,
                       ಸ್ಥಿರಾಂಕ FT_ಮಾತೃಕೆ*  ಮಾತೃಕೆ )
  {
    FT_Pos  xz, yz;


    ಆದರೆ ( !ಸದಿಶ || !ಮಾತೃಕೆ )
      ಹಿಂತಿರುಗಿ;

    xz = FT_MulFix( ಸದಿಶ->x, ಮಾತೃಕೆ->xx ) +
         FT_MulFix( ಸದಿಶ->y, ಮಾತೃಕೆ->xy );

    yz = FT_MulFix( ಸದಿಶ->x, ಮಾತೃಕೆ->yx ) +
         FT_MulFix( ಸದಿಶ->y, ಮಾತೃಕೆ->yy );

    ಸದಿಶ->x = xz;
    ಸದಿಶ->y = yz;
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( ಶೂನ್ಯ )
  FT_Outline_Transform( ಸ್ಥಿರಾಂಕ FT_Outline*  outline,
                        ಸ್ಥಿರಾಂಕ FT_ಮಾತೃಕೆ*   ಮಾತೃಕೆ )
  {
    FT_ಸದಿಶ*  vec;
    FT_ಸದಿಶ*  ಎಲ್ಲೆ;


    ಆದರೆ ( !outline || !ಮಾತೃಕೆ || !outline->points )
      ಹಿಂತಿರುಗಿ;

    vec   = outline->points;
    ಎಲ್ಲೆ = vec + outline->n_points;

    ಗಾಗಿ ( ; vec < ಎಲ್ಲೆ; vec++ )
      FT_ಸದಿಶ_Transform( vec, ಮಾತೃಕೆ );
  }


#ಆದರೆ 0

#ವ್ಯಾಖ್ಯಾನಿಸು FT_OUTLINE_GET_CONTOUR( outline, c, ಮೊದಲ, ಕೊನೆಯ )  \
  ಮಾಡು                                                       \
  {                                                        \
    (ಮೊದಲ) = ( c > 0 ) ? (outline)->points +              \
                            (outline)->contours[c - 1] + 1 \
                        : (outline)->points;               \
    (ಕೊನೆಯ) = (outline)->points + (outline)->contours[c];   \
  } ಆಗ ( 0 )


  /* Is a point in some contour?                     */
  /*                                                 */
  /* We treat every point of the contour as if it    */
  /* it were ON.  That is, we allow false positives, */
  /* but disallow false negatives.  (XXX really?)    */
  ಸ್ಥಿರ FT_Bool
  ft_contour_has( FT_Outline*  outline,
                  FT_Short     c,
                  FT_ಸದಿಶ*   ಬಿಂದು )
  {
    FT_ಸದಿಶ*  ಮೊದಲ;
    FT_ಸದಿಶ*  ಕೊನೆಯ;
    FT_ಸದಿಶ*  a;
    FT_ಸದಿಶ*  b;
    FT_UInt     n = 0;


    FT_OUTLINE_GET_CONTOUR( outline, c, ಮೊದಲ, ಕೊನೆಯ );

    ಗಾಗಿ ( a = ಮೊದಲ; a <= ಕೊನೆಯ; a++ )
    {
      FT_Pos  x;
      FT_Int  intersect;


      b = ( a == ಕೊನೆಯ ) ? ಮೊದಲ : a + 1;

      intersect = ( a->y - ಬಿಂದು->y ) ^ ( b->y - ಬಿಂದು->y );

      /* a and b are on the same side */
      ಆದರೆ ( intersect >= 0 )
      {
        ಆದರೆ ( intersect == 0 && a->y == ಬಿಂದು->y )
        {
          ಆದರೆ ( ( a->x <= ಬಿಂದು->x && b->x >= ಬಿಂದು->x ) ||
               ( a->x >= ಬಿಂದು->x && b->x <= ಬಿಂದು->x ) )
            ಹಿಂತಿರುಗಿ 1;
        }

        ಮುಂದುವರಿ;
      }

      x = a->x + ( b->x - a->x ) * (ಬಿಂದು->y - a->y ) / ( b->y - a->y );

      ಆದರೆ ( x < ಬಿಂದು->x )
        n++;
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ( x == ಬಿಂದು->x )
        ಹಿಂತಿರುಗಿ 1;
    }

    ಹಿಂತಿರುಗಿ n & 1;
  }


  ಸ್ಥಿರ FT_Bool
  ft_contour_enclosed( FT_Outline*  outline,
                       FT_UShort    c )
  {
    FT_ಸದಿಶ*  ಮೊದಲ;
    FT_ಸದಿಶ*  ಕೊನೆಯ;
    FT_Short    i;


    FT_OUTLINE_GET_CONTOUR( outline, c, ಮೊದಲ, ಕೊನೆಯ );

    ಗಾಗಿ ( i = 0; i < outline->n_contours; i++ )
    {
      ಆದರೆ ( i != c && ft_contour_has( outline, i, ಮೊದಲ ) )
      {
        FT_ಸದಿಶ*  pt;


        ಗಾಗಿ ( pt = ಮೊದಲ + 1; pt <= ಕೊನೆಯ; pt++ )
          ಆದರೆ ( !ft_contour_has( outline, i, pt ) )
            ಹಿಂತಿರುಗಿ 0;

        ಹಿಂತಿರುಗಿ 1;
      }
    }

    ಹಿಂತಿರುಗಿ 0;
  }


  /* This version differs from the public one in that each */
  /* part (contour not enclosed in another contour) of the */
  /* outline is checked for orientation.  This is          */
  /* necessary for some buggy CJK fonts.                   */
  ಸ್ಥಿರ FT_Orientation
  ft_outline_get_orientation( FT_Outline*  outline )
  {
    FT_Short        i;
    FT_ಸದಿಶ*      ಮೊದಲ;
    FT_ಸದಿಶ*      ಕೊನೆಯ;
    FT_Orientation  orient = FT_ORIENTATION_NONE;


    ಮೊದಲ = outline->points;
    ಗಾಗಿ ( i = 0; i < outline->n_contours; i++, ಮೊದಲ = ಕೊನೆಯ + 1 )
    {
      FT_ಸದಿಶ*  ಬಿಂದು;
      FT_ಸದಿಶ*  xmin_ಬಿಂದು;
      FT_Pos      xmin;


      ಕೊನೆಯ = outline->points + outline->contours[i];

      /* skip degenerate contours */
      ಆದರೆ ( ಕೊನೆಯ < ಮೊದಲ + 2 )
        ಮುಂದುವರಿ;

      ಆದರೆ ( ft_contour_enclosed( outline, i ) )
        ಮುಂದುವರಿ;

      xmin       = ಮೊದಲ->x;
      xmin_ಬಿಂದು = ಮೊದಲ;

      ಗಾಗಿ ( ಬಿಂದು = ಮೊದಲ + 1; ಬಿಂದು <= ಕೊನೆಯ; ಬಿಂದು++ )
      {
        ಆದರೆ ( ಬಿಂದು->x < xmin )
        {
          xmin       = ಬಿಂದು->x;
          xmin_ಬಿಂದು = ಬಿಂದು;
        }
      }

      /* check the orientation of the contour */
      {
        FT_ಸದಿಶ*      prev;
        FT_ಸದಿಶ*      ಮುಂದಿನ;
        FT_Orientation  o;


        prev = ( xmin_ಬಿಂದು == ಮೊದಲ ) ? ಕೊನೆಯ : xmin_ಬಿಂದು - 1;
        ಮುಂದಿನ = ( xmin_ಬಿಂದು == ಕೊನೆಯ ) ? ಮೊದಲ : xmin_ಬಿಂದು + 1;

        ಆದರೆ ( FT_Atan2( prev->x - xmin_ಬಿಂದು->x, prev->y - xmin_ಬಿಂದು->y ) >
             FT_Atan2( ಮುಂದಿನ->x - xmin_ಬಿಂದು->x, ಮುಂದಿನ->y - xmin_ಬಿಂದು->y ) )
          o = FT_ORIENTATION_POSTSCRIPT;
        ಇಲ್ಲದಿದ್ದರೆ
          o = FT_ORIENTATION_TRUETYPE;

        ಆದರೆ ( orient == FT_ORIENTATION_NONE )
          orient = o;
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ( orient != o )
          ಹಿಂತಿರುಗಿ FT_ORIENTATION_NONE;
      }
    }

    ಹಿಂತಿರುಗಿ orient;
  }

#ಆದರೆ_ಅಂತ್ಯ /* 0 */


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Outline_Embolden( FT_Outline*  outline,
                       FT_Pos       strength )
  {
    ಹಿಂತಿರುಗಿ FT_Outline_EmboldenXY( outline, strength, strength );
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Outline_EmboldenXY( FT_Outline*  outline,
                         FT_Pos       xstrength,
                         FT_Pos       ystrength )
  {
    FT_ಸದಿಶ*      points;
    FT_Int          c, ಮೊದಲ, ಕೊನೆಯ;
    FT_Orientation  orientation;


    ಆದರೆ ( !outline )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_Outline );

    xstrength /= 2;
    ystrength /= 2;
    ಆದರೆ ( xstrength == 0 && ystrength == 0 )
      ಹಿಂತಿರುಗಿ FT_Err_Ok;

    orientation = FT_Outline_Get_Orientation( outline );
    ಆದರೆ ( orientation == FT_ORIENTATION_NONE )
    {
      ಆದರೆ ( outline->n_contours )
        ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );
      ಇಲ್ಲದಿದ್ದರೆ
        ಹಿಂತಿರುಗಿ FT_Err_Ok;
    }

    points = outline->points;

    ಕೊನೆಯ = -1;
    ಗಾಗಿ ( c = 0; c < outline->n_contours; c++ )
    {
      FT_ಸದಿಶ  ಒಳ, ಹೊರಹೋಗು, anchor, ಸರಿಸು;
      FT_Fixed   l_ಒಳ, l_ಹೊರಹೋಗು, l_anchor = 0, l, q, d;
      FT_Int     i, j, k;


      ಮೊದಲ = ಕೊನೆಯ + 1;
      ಕೊನೆಯ  = outline->contours[c];
      l_ಒಳ  = 0;

      /* pacify compiler */
      ಒಳ.x = ಒಳ.y = anchor.x = anchor.y = 0;

      /* Counter j cycles though the points; counter i advances only  */
      /* when points are moved; anchor k marks the first moved point. */
      ಗಾಗಿ ( i = ಕೊನೆಯ, j = ಮೊದಲ, k = -1;
            j != i && i != k;
            j = j < ಕೊನೆಯ ? j + 1 : ಮೊದಲ )
      {
        ಆದರೆ ( j != k )
        {
          ಹೊರಹೋಗು.x = points[j].x - points[i].x;
          ಹೊರಹೋಗು.y = points[j].y - points[i].y;
          l_ಹೊರಹೋಗು = (FT_Fixed)FT_ಸದಿಶ_Norm_Len( &ಹೊರಹೋಗು );

          ಆದರೆ ( l_ಹೊರಹೋಗು == 0 )
            ಮುಂದುವರಿ;
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
          ಹೊರಹೋಗು   = anchor;
          l_ಹೊರಹೋಗು = l_anchor;
        }

        ಆದರೆ ( l_ಒಳ != 0 )
        {
          ಆದರೆ ( k < 0 )
          {
            k        = i;
            anchor   = ಒಳ;
            l_anchor = l_ಒಳ;
          }

          d = FT_MulFix( ಒಳ.x, ಹೊರಹೋಗು.x ) + FT_MulFix( ಒಳ.y, ಹೊರಹೋಗು.y );

          /* shift only if turn is less than ~160 degrees */
          ಆದರೆ ( d > -0xF000L )
          {
            d = d + 0x10000L;

            /* shift components along lateral bisector in proper orientation */
            ಸರಿಸು.x = ಒಳ.y + ಹೊರಹೋಗು.y;
            ಸರಿಸು.y = ಒಳ.x + ಹೊರಹೋಗು.x;

            ಆದರೆ ( orientation == FT_ORIENTATION_TRUETYPE )
              ಸರಿಸು.x = -ಸರಿಸು.x;
            ಇಲ್ಲದಿದ್ದರೆ
              ಸರಿಸು.y = -ಸರಿಸು.y;

            /* restrict shift magnitude to better handle collapsing segments */
            q = FT_MulFix( ಹೊರಹೋಗು.x, ಒಳ.y ) - FT_MulFix( ಹೊರಹೋಗು.y, ಒಳ.x );
            ಆದರೆ ( orientation == FT_ORIENTATION_TRUETYPE )
              q = -q;

            l = FT_MIN( l_ಒಳ, l_ಹೊರಹೋಗು );

            /* non-strict inequalities avoid divide-by-zero when q == l == 0 */
            ಆದರೆ ( FT_MulFix( xstrength, q ) <= FT_MulFix( l, d ) )
              ಸರಿಸು.x = FT_MulDiv( ಸರಿಸು.x, xstrength, d );
            ಇಲ್ಲದಿದ್ದರೆ
              ಸರಿಸು.x = FT_MulDiv( ಸರಿಸು.x, l, q );


            ಆದರೆ ( FT_MulFix( ystrength, q ) <= FT_MulFix( l, d ) )
              ಸರಿಸು.y = FT_MulDiv( ಸರಿಸು.y, ystrength, d );
            ಇಲ್ಲದಿದ್ದರೆ
              ಸರಿಸು.y = FT_MulDiv( ಸರಿಸು.y, l, q );
          }
          ಇಲ್ಲದಿದ್ದರೆ
            ಸರಿಸು.x = ಸರಿಸು.y = 0;

          ಗಾಗಿ ( ;
                i != j;
                i = i < ಕೊನೆಯ ? i + 1 : ಮೊದಲ )
          {
            points[i].x += xstrength + ಸರಿಸು.x;
            points[i].y += ystrength + ಸರಿಸು.y;
          }
        }
        ಇಲ್ಲದಿದ್ದರೆ
          i = j;

        ಒಳ   = ಹೊರಹೋಗು;
        l_ಒಳ = l_ಹೊರಹೋಗು;
      }
    }

    ಹಿಂತಿರುಗಿ FT_Err_Ok;
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Orientation )
  FT_Outline_Get_Orientation( FT_Outline*  outline )
  {
    FT_ಸದಿಶ*  points;
    FT_Int      c, n, ಮೊದಲ, ಕೊನೆಯ;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ FT_INT64
    FT_Int64    area = 0;
#ಇಲ್ಲದಿದ್ದರೆ
    FT_BBox     cbox = { 0, 0, 0, 0 };
    FT_Int      xshift, yshift;
    FT_ಸದಿಶ   v_prev, v_cur;
    FT_Pos      area = 0;
#ಆದರೆ_ಅಂತ್ಯ


    ಆದರೆ ( !outline || outline->n_points <= 0 )
      ಹಿಂತಿರುಗಿ FT_ORIENTATION_TRUETYPE;

    /* We use the nonzero winding rule to find the orientation.       */
    /* Since glyph outlines behave much more `regular' than arbitrary */
    /* cubic or quadratic curves, this test deals with the polygon    */
    /* only that is spanned up by the control points.                 */

    points = outline->points;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ FT_INT64

    /* Single-pass shoelace.  The 64-bit accumulator removes the need   */
    /* for the coordinate shift pre-pass (and its `FT_Outline_Get_CBox` */
    /* traversal) that the 32-bit variant below uses.                   */
    ಕೊನೆಯ = -1;
    ಗಾಗಿ ( c = 0; c < outline->n_contours; c++ )
    {
      FT_Pos  prev_x, prev_y;


      ಮೊದಲ = ಕೊನೆಯ + 1;
      ಕೊನೆಯ  = outline->contours[c];

      prev_x = points[ಕೊನೆಯ].x;
      prev_y = points[ಕೊನೆಯ].y;

      ಗಾಗಿ ( n = ಮೊದಲ; n <= ಕೊನೆಯ; n++ )
      {
        FT_Pos  cur_x = points[n].x;
        FT_Pos  cur_y = points[n].y;


        area = ಕೂಡಿಸು_INT64( area,
                          MUL_INT64( SUB_INT64( cur_y, prev_y ),
                                     ಕೂಡಿಸು_INT64( cur_x, prev_x ) ) );

        prev_x = cur_x;
        prev_y = cur_y;
      }
    }

#ಇಲ್ಲದಿದ್ದರೆ /* !FT_INT64 */

    FT_Outline_Get_CBox( outline, &cbox );

    /* Handle collapsed outlines to avoid undefined FT_MSB. */
    ಆದರೆ ( cbox.xMin == cbox.xMax || cbox.yMin == cbox.yMax )
      ಹಿಂತಿರುಗಿ FT_ORIENTATION_NONE;

    /* Reject values large outlines. */
    ಆದರೆ ( cbox.xMin < -0x1000000L || cbox.yMin < -0x1000000L ||
         cbox.xMax >  0x1000000L || cbox.yMax >  0x1000000L )
      ಹಿಂತಿರುಗಿ FT_ORIENTATION_NONE;

    xshift = FT_MSB( (FT_UInt32)( FT_ABS( cbox.xMax ) |
                                  FT_ABS( cbox.xMin ) ) ) - 14;
    xshift = FT_MAX( xshift, 0 );

    yshift = FT_MSB( (FT_UInt32)( cbox.yMax - cbox.yMin ) ) - 14;
    yshift = FT_MAX( yshift, 0 );

    ಕೊನೆಯ = -1;
    ಗಾಗಿ ( c = 0; c < outline->n_contours; c++ )
    {
      ಮೊದಲ = ಕೊನೆಯ + 1;
      ಕೊನೆಯ  = outline->contours[c];

      v_prev.x = points[ಕೊನೆಯ].x >> xshift;
      v_prev.y = points[ಕೊನೆಯ].y >> yshift;

      ಗಾಗಿ ( n = ಮೊದಲ; n <= ಕೊನೆಯ; n++ )
      {
        v_cur.x = points[n].x >> xshift;
        v_cur.y = points[n].y >> yshift;

        area = ಕೂಡಿಸು_LONG( area,
                         MUL_LONG( v_cur.y - v_prev.y,
                                   v_cur.x + v_prev.x ) );

        v_prev = v_cur;
      }
    }

#ಆದರೆ_ಅಂತ್ಯ /* FT_INT64 */

    ಆದರೆ ( area > 0 )
      ಹಿಂತಿರುಗಿ FT_ORIENTATION_POSTSCRIPT;
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ( area < 0 )
      ಹಿಂತಿರುಗಿ FT_ORIENTATION_TRUETYPE;
    ಇಲ್ಲದಿದ್ದರೆ
      ಹಿಂತಿರುಗಿ FT_ORIENTATION_NONE;
  }


/*ಕೊನೆ*/
