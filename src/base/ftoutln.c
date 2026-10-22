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


#అన్తర్భావ <freetype/ftoutln.h>
#అన్తర్భావ <freetype/internal/ftobjs.h>
#అన్తర్భావ <freetype/internal/ftcalc.h>
#అన్తర్భావ <freetype/internal/ftdebug.h>
#అన్తర్భావ <freetype/fttrigon.h>


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#నిర్వచనం_రద్దు  FT_COMPONENT
#నిర్వచించు FT_COMPONENT  outline


  స్థిర
  స్థిరరాశి FT_Outline  null_outline = { 0, 0, ఖాళీ, ఖాళీ, ఖాళీ, 0 };


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Outline_Decompose( FT_Outline*              outline,
                        స్థిరరాశి FT_Outline_Funcs*  func_interface,
                        శూన్య*                    user )
  {
#నిర్వచనం_రద్దు  SCALED
#నిర్వచించు SCALED( x )  ( (x) * ( 1L << జరుపు ) - delta )

    FT_సదిశ   v_చివరి;
    FT_సదిశ   v_control;
    FT_సదిశ   v_ప్రారంభం;

    FT_సదిశ*  బిందువు;
    FT_సదిశ*  హద్దు;
    FT_Byte*    ట్యాగులు;

    FT_Error    error;

    FT_Int   n;         /* index of contour in outline     */
    FT_Int   మొదటి;     /* index of first point in contour */
    FT_Int   చివరి;      /* index of last point in contour  */

    FT_Int   ట్యాగ్;       /* current point's state           */

    FT_Int   జరుపు;
    FT_Pos   delta;


    ఒకవేళ ( !outline )
      తిరిగి FT_THROW( Invalid_Outline );

    ఒకవేళ ( !func_interface )
      తిరిగి FT_THROW( Invalid_వాదం );

    జరుపు = func_interface->జరుపు;
    delta = func_interface->delta;

    చివరి = -1;
    కొరకు ( n = 0; n < outline->n_contours; n++ )
    {
      FT_TRACE5(( "FT_Outline_Decompose: Contour %d\n", n ));

      మొదటి = చివరి + 1;
      చివరి  = outline->contours[n];
      ఒకవేళ ( చివరి < మొదటి )
        వెళ్ళు Invalid_Outline;

      హద్దు = outline->points + చివరి;

      v_ప్రారంభం   = outline->points[మొదటి];
      v_ప్రారంభం.x = SCALED( v_ప్రారంభం.x );
      v_ప్రారంభం.y = SCALED( v_ప్రారంభం.y );

      v_చివరి   = outline->points[చివరి];
      v_చివరి.x = SCALED( v_చివరి.x );
      v_చివరి.y = SCALED( v_చివరి.y );

      v_control = v_ప్రారంభం;

      బిందువు = outline->points + మొదటి;
      ట్యాగులు  = outline->ట్యాగులు   + మొదటి;
      ట్యాగ్   = FT_వక్రం_ట్యాగ్( ట్యాగులు[0] );

      /* check first point to determine origin */
      ఒకవేళ ( ట్యాగ్ != FT_వక్రం_ట్యాగ్_ON )
      {
        FT_Int  చివరి_ట్యాగ్ = FT_వక్రం_ట్యాగ్( outline->ట్యాగులు[చివరి] );


        /* first point is a control point.  Yes, this happens. */
        ఒకవేళ ( చివరి_ట్యాగ్ == FT_వక్రం_ట్యాగ్_ON )
        {
          /* start at last point if it is on the curve */
          v_ప్రారంభం = v_చివరి;
          హద్దు--;
        }
        లేకపోతే
        {
          ఒకవేళ ( చివరి_ట్యాగ్ != ట్యాగ్ )
            వెళ్ళు Invalid_Outline;

          /* if both first and last points are controls,   */
          /* start at their middle and record its position */
          /* for closure                                   */
          v_ప్రారంభం.x = ( v_ప్రారంభం.x + v_చివరి.x ) / 2;
          v_ప్రారంభం.y = ( v_ప్రారంభం.y + v_చివరి.y ) / 2;

          /* v_last = v_start; */
        }
        బిందువు--;
        ట్యాగులు--;
      }

      FT_TRACE5(( "  move to (%.2f, %.2f)\n",
                  (ద్విగుణ)v_ప్రారంభం.x / 64, (ద్విగుణ)v_ప్రారంభం.y / 64 ));
      error = func_interface->move_గమ్యం( &v_ప్రారంభం, user );
      ఒకవేళ ( error )
        వెళ్ళు Exit;

      అయితే ( బిందువు < హద్దు )
      {
        బిందువు++;
        ట్యాగులు++;

        ట్యాగ్ = FT_వక్రం_ట్యాగ్( ట్యాగులు[0] );
        ఎంపిక ( ట్యాగ్ )
        {
        స్థితి FT_వక్రం_ట్యాగ్_ON:  /* emit a single line_to */
          {
            FT_సదిశ  vec;


            vec.x = SCALED( బిందువు->x );
            vec.y = SCALED( బిందువు->y );

            FT_TRACE5(( "  line to (%.2f, %.2f)\n",
                        (ద్విగుణ)vec.x / 64, (ద్విగుణ)vec.y / 64 ));
            error = func_interface->పంక్తి_గమ్యం( &vec, user );
            ఒకవేళ ( error )
              వెళ్ళు Exit;
            కొనసాగు;
          }

        స్థితి FT_వక్రం_ట్యాగ్_CONIC:  /* consume conic arcs */
          v_control.x = SCALED( బిందువు->x );
          v_control.y = SCALED( బిందువు->y );

        Do_Conic:
          ఒకవేళ ( బిందువు < హద్దు )
          {
            FT_సదిశ  vec;
            FT_సదిశ  v_మధ్య;


            బిందువు++;
            ట్యాగులు++;
            ట్యాగ్ = FT_వక్రం_ట్యాగ్( ట్యాగులు[0] );

            vec.x = SCALED( బిందువు->x );
            vec.y = SCALED( బిందువు->y );

            ఒకవేళ ( ట్యాగ్ == FT_వక్రం_ట్యాగ్_ON )
            {
              FT_TRACE5(( "  conic to (%.2f, %.2f)"
                          " with control (%.2f, %.2f)\n",
                          (ద్విగుణ)vec.x / 64,
                          (ద్విగుణ)vec.y / 64,
                          (ద్విగుణ)v_control.x / 64,
                          (ద్విగుణ)v_control.y / 64 ));
              error = func_interface->conic_గమ్యం( &v_control, &vec, user );
              ఒకవేళ ( error )
                వెళ్ళు Exit;
              కొనసాగు;
            }

            ఒకవేళ ( ట్యాగ్ != FT_వక్రం_ట్యాగ్_CONIC )
              వెళ్ళు Invalid_Outline;

            v_మధ్య.x = ( v_control.x + vec.x ) / 2;
            v_మధ్య.y = ( v_control.y + vec.y ) / 2;

            FT_TRACE5(( "  conic to (%.2f, %.2f)"
                        " with control (%.2f, %.2f)\n",
                        (ద్విగుణ)v_మధ్య.x / 64,
                        (ద్విగుణ)v_మధ్య.y / 64,
                        (ద్విగుణ)v_control.x / 64,
                        (ద్విగుణ)v_control.y / 64 ));
            error = func_interface->conic_గమ్యం( &v_control, &v_మధ్య, user );
            ఒకవేళ ( error )
              వెళ్ళు Exit;

            v_control = vec;
            వెళ్ళు Do_Conic;
          }

          FT_TRACE5(( "  conic to (%.2f, %.2f)"
                      " with control (%.2f, %.2f)\n",
                      (ద్విగుణ)v_ప్రారంభం.x / 64,
                      (ద్విగుణ)v_ప్రారంభం.y / 64,
                      (ద్విగుణ)v_control.x / 64,
                      (ద్విగుణ)v_control.y / 64 ));
          error = func_interface->conic_గమ్యం( &v_control, &v_ప్రారంభం, user );
          వెళ్ళు Close;

        అప్రమేయ:  /* FT_CURVE_TAG_CUBIC */
          {
            FT_సదిశ  vec1, vec2;


            ఒకవేళ ( బిందువు + 1 > హద్దు                             ||
                 FT_వక్రం_ట్యాగ్( ట్యాగులు[1] ) != FT_వక్రం_ట్యాగ్_CUBIC )
              వెళ్ళు Invalid_Outline;

            బిందువు += 2;
            ట్యాగులు  += 2;

            vec1.x = SCALED( బిందువు[-2].x );
            vec1.y = SCALED( బిందువు[-2].y );

            vec2.x = SCALED( బిందువు[-1].x );
            vec2.y = SCALED( బిందువు[-1].y );

            ఒకవేళ ( బిందువు <= హద్దు )
            {
              FT_సదిశ  vec;
              FT_సదిశ  v_మధ్య;


              ట్యాగ్ = FT_వక్రం_ట్యాగ్( ట్యాగులు[0] );

              vec.x = SCALED( బిందువు->x );
              vec.y = SCALED( బిందువు->y );

              ఒకవేళ ( ట్యాగ్ == FT_వక్రం_ట్యాగ్_CUBIC )
              {
                v_మధ్య.x = ( vec2.x + vec.x ) / 2;
                v_మధ్య.y = ( vec2.y + vec.y ) / 2;

                vec = v_మధ్య;
              }
              లేకపోతే ఒకవేళ ( ట్యాగ్ != FT_వక్రం_ట్యాగ్_ON )
                వెళ్ళు Invalid_Outline;

              FT_TRACE5(( "  cubic to (%.2f, %.2f)"
                          " with controls (%.2f, %.2f) and (%.2f, %.2f)\n",
                          (ద్విగుణ)vec.x / 64,
                          (ద్విగుణ)vec.y / 64,
                          (ద్విగుణ)vec1.x / 64,
                          (ద్విగుణ)vec1.y / 64,
                          (ద్విగుణ)vec2.x / 64,
                          (ద్విగుణ)vec2.y / 64 ));
              error = func_interface->cubic_గమ్యం( &vec1, &vec2, &vec, user );
              ఒకవేళ ( error )
                వెళ్ళు Exit;

              ఒకవేళ ( ట్యాగ్ == FT_వక్రం_ట్యాగ్_CUBIC )
              {
                బిందువు--;
                ట్యాగులు--;
              }
              కొనసాగు;
            }

            FT_TRACE5(( "  cubic to (%.2f, %.2f)"
                        " with controls (%.2f, %.2f) and (%.2f, %.2f)\n",
                        (ద్విగుణ)v_ప్రారంభం.x / 64,
                        (ద్విగుణ)v_ప్రారంభం.y / 64,
                        (ద్విగుణ)vec1.x / 64,
                        (ద్విగుణ)vec1.y / 64,
                        (ద్విగుణ)vec2.x / 64,
                        (ద్విగుణ)vec2.y / 64 ));
            error = func_interface->cubic_గమ్యం( &vec1, &vec2, &v_ప్రారంభం, user );
            వెళ్ళు Close;
          }
        }
      }

      /* close the contour with a line segment */
      FT_TRACE5(( "  line to (%.2f, %.2f)\n",
                  (ద్విగుణ)v_ప్రారంభం.x / 64, (ద్విగుణ)v_ప్రారంభం.y / 64 ));
      error = func_interface->పంక్తి_గమ్యం( &v_ప్రారంభం, user );

    Close:
      ఒకవేళ ( error )
        వెళ్ళు Exit;
    }

    FT_TRACE5(( "FT_Outline_Decompose: Done\n" ));
    తిరిగి FT_Err_Ok;

  Invalid_Outline:
    error = FT_THROW( Invalid_Outline );
    /* fall through */

  Exit:
    FT_TRACE5(( "FT_Outline_Decompose: Error 0x%x\n", error ));
    తిరిగి error;
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Outline_New( FT_Library   library,
                  FT_UInt      అంకె_Points,
                  FT_Int       అంకె_Contours,
                  FT_Outline  *anoutline )
  {
    FT_Error   error;
    FT_జ్ఞాపకం  జ్ఞాపకం;


    ఒకవేళ ( !library )
      తిరిగి FT_THROW( Invalid_Library_Handle );

    జ్ఞాపకం = library->జ్ఞాపకం;

    ఒకవేళ ( !anoutline || !జ్ఞాపకం )
      తిరిగి FT_THROW( Invalid_వాదం );

    *anoutline = null_outline;

    ఒకవేళ ( అంకె_Contours < 0                  ||
         (FT_UInt)అంకె_Contours > అంకె_Points )
      తిరిగి FT_THROW( Invalid_వాదం );

    ఒకవేళ ( అంకె_Points > FT_OUTLINE_POINTS_MAX )
      తిరిగి FT_THROW( Array_Too_Large );

    ఒకవేళ ( FT_NEW_ARRAY( anoutline->points,   అంకె_Points   ) ||
         FT_NEW_ARRAY( anoutline->ట్యాగులు,     అంకె_Points   ) ||
         FT_NEW_ARRAY( anoutline->contours, అంకె_Contours ) )
      వెళ్ళు Fail;

    anoutline->n_points    = (FT_UShort)అంకె_Points;
    anoutline->n_contours  = (FT_UShort)అంకె_Contours;
    anoutline->flags      |= FT_OUTLINE_OWNER;

    తిరిగి FT_Err_Ok;

  Fail:
    anoutline->flags |= FT_OUTLINE_OWNER;
    FT_Outline_పూర్తి( library, anoutline );

    తిరిగి error;
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Outline_Check( FT_Outline*  outline )
  {
    ఒకవేళ ( outline )
    {
      FT_Int  n_points   = outline->n_points;
      FT_Int  n_contours = outline->n_contours;
      FT_Int  end0, చివర;
      FT_Int  n;


      FT_TRACE5(( "FT_Outline_Check: contours = %d, points = %d\n",
                  n_contours, n_points ));
      /* empty glyph? */
      ఒకవేళ ( n_points == 0 && n_contours == 0 )
        తిరిగి FT_Err_Ok;

      /* check point and contour counts */
      ఒకవేళ ( n_points == 0 || n_contours == 0 )
        వెళ్ళు Bad;

      end0 = -1;
      కొరకు ( n = 0; n < n_contours; n++ )
      {
        చివర = outline->contours[n];

        /* note that we don't accept empty contours */
        ఒకవేళ ( చివర <= end0 || చివర >= n_points )
          వెళ్ళు Bad;

        end0 = చివర;
      }

      ఒకవేళ ( end0 != n_points - 1 )
        వెళ్ళు Bad;

      /* XXX: check the tags array */
      తిరిగి FT_Err_Ok;
    }

  Bad:
    తిరిగి FT_THROW( Invalid_Outline );
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Outline_Copy( స్థిరరాశి FT_Outline*  ఆధారం,
                   FT_Outline        *లక్ష్యం )
  {
    FT_Int  is_owner;


    ఒకవేళ ( !ఆధారం || !లక్ష్యం )
      తిరిగి FT_THROW( Invalid_Outline );

    ఒకవేళ ( ఆధారం->n_points   != లక్ష్యం->n_points   ||
         ఆధారం->n_contours != లక్ష్యం->n_contours )
      తిరిగి FT_THROW( Invalid_వాదం );

    ఒకవేళ ( ఆధారం == లక్ష్యం )
      తిరిగి FT_Err_Ok;

    ఒకవేళ ( ఆధారం->n_points )
    {
      FT_ARRAY_COPY( లక్ష్యం->points, ఆధారం->points, ఆధారం->n_points );
      FT_ARRAY_COPY( లక్ష్యం->ట్యాగులు,   ఆధారం->ట్యాగులు,   ఆధారం->n_points );
    }

    ఒకవేళ ( ఆధారం->n_contours )
      FT_ARRAY_COPY( లక్ష్యం->contours, ఆధారం->contours, ఆధారం->n_contours );

    /* copy all flags, except the `FT_OUTLINE_OWNER' one */
    is_owner      = లక్ష్యం->flags & FT_OUTLINE_OWNER;
    లక్ష్యం->flags = ఆధారం->flags;

    లక్ష్యం->flags &= ~FT_OUTLINE_OWNER;
    లక్ష్యం->flags |= is_owner;

    తిరిగి FT_Err_Ok;
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Outline_పూర్తి( FT_Library   library,
                   FT_Outline*  outline )
  {
    FT_జ్ఞాపకం  జ్ఞాపకం;


    ఒకవేళ ( !library )
      తిరిగి FT_THROW( Invalid_Library_Handle );

    ఒకవేళ ( !outline )
      తిరిగి FT_THROW( Invalid_Outline );

    జ్ఞాపకం = library->జ్ఞాపకం;

    ఒకవేళ ( !జ్ఞాపకం )
      తిరిగి FT_THROW( Invalid_వాదం );

    ఒకవేళ ( outline->flags & FT_OUTLINE_OWNER )
    {
      FT_FREE( outline->points   );
      FT_FREE( outline->ట్యాగులు     );
      FT_FREE( outline->contours );
    }
    *outline = null_outline;

    తిరిగి FT_Err_Ok;
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( శూన్య )
  FT_Outline_Get_CBox( స్థిరరాశి FT_Outline*  outline,
                       FT_BBox           *acbox )
  {
    FT_Pos  xMin, yMin, xMax, yMax;


    ఒకవేళ ( outline && acbox )
    {
      ఒకవేళ ( outline->n_points == 0 )
      {
        xMin = 0;
        yMin = 0;
        xMax = 0;
        yMax = 0;
      }
      లేకపోతే
      {
        FT_సదిశ*  vec   = outline->points;
        FT_సదిశ*  హద్దు = vec + outline->n_points;


        xMin = xMax = vec->x;
        yMin = yMax = vec->y;
        vec++;

        కొరకు ( ; vec < హద్దు; vec++ )
        {
          FT_Pos  x, y;


          x = vec->x;
          ఒకవేళ ( x < xMin ) xMin = x;
          ఒకవేళ ( x > xMax ) xMax = x;

          y = vec->y;
          ఒకవేళ ( y < yMin ) yMin = y;
          ఒకవేళ ( y > yMax ) yMax = y;
        }
      }
      acbox->xMin = xMin;
      acbox->xMax = xMax;
      acbox->yMin = yMin;
      acbox->yMax = yMax;
    }
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( శూన్య )
  FT_Outline_Translate( స్థిరరాశి FT_Outline*  outline,
                        FT_Pos             x_స్థానభ్రంశం,
                        FT_Pos             y_స్థానభ్రంశం )
  {
    FT_UShort   n;
    FT_సదిశ*  vec;


    ఒకవేళ ( !outline )
      తిరిగి;

    vec = outline->points;

    కొరకు ( n = 0; n < outline->n_points; n++ )
    {
      vec->x = కూడు_LONG( vec->x, x_స్థానభ్రంశం );
      vec->y = కూడు_LONG( vec->y, y_స్థానభ్రంశం );
      vec++;
    }
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( శూన్య )
  FT_Outline_తిరగేయి( FT_Outline*  outline )
  {
    FT_UShort  n;
    FT_Int     మొదటి, చివరి;


    ఒకవేళ ( !outline )
      తిరిగి;

    చివరి = -1;
    కొరకు ( n = 0; n < outline->n_contours; n++ )
    {
      మొదటి = చివరి + 1;
      చివరి  = outline->contours[n];

      /* Keep the first point unless it is a cubic control.  In that */
      /* case, reverse the entire contour to preserve control pairs, */
      /* including contours with no explicit on-curve points.        */
      ఒకవేళ ( FT_వక్రం_ట్యాగ్( outline->ట్యాగులు[మొదటి] ) != FT_వక్రం_ట్యాగ్_CUBIC )
        మొదటి++;

      /* reverse point table */
      {
        FT_సదిశ*  p = outline->points + మొదటి;
        FT_సదిశ*  q = outline->points + చివరి;
        FT_సదిశ   మార్పిడి;


        అయితే ( p < q )
        {
          మార్పిడి = *p;
          *p   = *q;
          *q   = మార్పిడి;
          p++;
          q--;
        }
      }

      /* reverse tags table */
      {
        FT_Byte*  p = outline->ట్యాగులు + మొదటి;
        FT_Byte*  q = outline->ట్యాగులు + చివరి;


        అయితే ( p < q )
        {
          FT_Byte  మార్పిడి;


          మార్పిడి = *p;
          *p   = *q;
          *q   = మార్పిడి;
          p++;
          q--;
        }
      }
    }

    outline->flags ^= FT_OUTLINE_తిరగేయి_FILL;
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Outline_Render( FT_Library         library,
                     FT_Outline*        outline,
                     FT_Raster_Params*  params )
  {
    FT_Error     error;
    FT_Renderer  renderer;
    FT_List_కణుపు  కణుపు;
    FT_BBox      cbox;


    ఒకవేళ ( !library )
      తిరిగి FT_THROW( Invalid_Library_Handle );

    ఒకవేళ ( !outline )
      తిరిగి FT_THROW( Invalid_Outline );

    ఒకవేళ ( !params )
      తిరిగి FT_THROW( Invalid_వాదం );

    FT_Outline_Get_CBox( outline, &cbox );
    ఒకవేళ ( cbox.xMin < -0x1000000L || cbox.yMin < -0x1000000L ||
         cbox.xMax >  0x1000000L || cbox.yMax >  0x1000000L )
      తిరిగి FT_THROW( Invalid_Outline );

    renderer = library->cur_renderer;
    కణుపు     = library->renderers.తల;

    params->ఆధారం = (శూన్య*)outline;

    /* preset clip_box for direct mode */
    ఒకవేళ ( params->flags & FT_RASTER_FLAG_ప్రత్యక్ష    &&
         !( params->flags & FT_RASTER_FLAG_CLIP ) )
    {
      params->clip_box.xMin = cbox.xMin >> 6;
      params->clip_box.yMin = cbox.yMin >> 6;
      params->clip_box.xMax = ( cbox.xMax + 63 ) >> 6;
      params->clip_box.yMax = ( cbox.yMax + 63 ) >> 6;
    }

    error = FT_ERR( Cannot_Render_Glyph );
    అయితే ( renderer )
    {
      error = renderer->raster_render( renderer->raster, params );
      ఒకవేళ ( !error || FT_ERR_NEQ( error, Cannot_Render_Glyph ) )
        ఆపు;

      /* FT_Err_Cannot_Render_Glyph is returned if the render mode   */
      /* is unsupported by the current renderer for this glyph image */
      /* format                                                      */

      /* now, look for another renderer that supports the same */
      /* format                                                */
      renderer = FT_Lookup_Renderer( library, FT_GLYPH_FORMAT_OUTLINE,
                                     &కణుపు );
    }

    తిరిగి error;
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Outline_Get_Bitmap( FT_Library        library,
                         FT_Outline*       outline,
                         స్థిరరాశి FT_Bitmap  *abitmap )
  {
    FT_Raster_Params  params;


    ఒకవేళ ( !abitmap )
      తిరిగి FT_THROW( Invalid_వాదం );

    /* other checks are delayed to `FT_Outline_Render' */

    params.లక్ష్యం = abitmap;
    params.flags  = 0;

    ఒకవేళ ( abitmap->pixel_బహుళకం == FT_PIXEL_బహుళకం_బూడిద  ||
         abitmap->pixel_బహుళకం == FT_PIXEL_బహుళకం_LCD   ||
         abitmap->pixel_బహుళకం == FT_PIXEL_బహుళకం_LCD_V )
      params.flags |= FT_RASTER_FLAG_AA;

    తిరిగి FT_Outline_Render( library, outline, &params );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( శూన్య )
  FT_సదిశ_Transform( FT_సదిశ*        సదిశ,
                       స్థిరరాశి FT_మాతృక*  మాతృక )
  {
    FT_Pos  xz, yz;


    ఒకవేళ ( !సదిశ || !మాతృక )
      తిరిగి;

    xz = FT_MulFix( సదిశ->x, మాతృక->xx ) +
         FT_MulFix( సదిశ->y, మాతృక->xy );

    yz = FT_MulFix( సదిశ->x, మాతృక->yx ) +
         FT_MulFix( సదిశ->y, మాతృక->yy );

    సదిశ->x = xz;
    సదిశ->y = yz;
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( శూన్య )
  FT_Outline_Transform( స్థిరరాశి FT_Outline*  outline,
                        స్థిరరాశి FT_మాతృక*   మాతృక )
  {
    FT_సదిశ*  vec;
    FT_సదిశ*  హద్దు;


    ఒకవేళ ( !outline || !మాతృక || !outline->points )
      తిరిగి;

    vec   = outline->points;
    హద్దు = vec + outline->n_points;

    కొరకు ( ; vec < హద్దు; vec++ )
      FT_సదిశ_Transform( vec, మాతృక );
  }


#ఒకవేళ 0

#నిర్వచించు FT_OUTLINE_GET_CONTOUR( outline, c, మొదటి, చివరి )  \
  చేయి                                                       \
  {                                                        \
    (మొదటి) = ( c > 0 ) ? (outline)->points +              \
                            (outline)->contours[c - 1] + 1 \
                        : (outline)->points;               \
    (చివరి) = (outline)->points + (outline)->contours[c];   \
  } అయితే ( 0 )


  /* Is a point in some contour?                     */
  /*                                                 */
  /* We treat every point of the contour as if it    */
  /* it were ON.  That is, we allow false positives, */
  /* but disallow false negatives.  (XXX really?)    */
  స్థిర FT_Bool
  ft_contour_has( FT_Outline*  outline,
                  FT_Short     c,
                  FT_సదిశ*   బిందువు )
  {
    FT_సదిశ*  మొదటి;
    FT_సదిశ*  చివరి;
    FT_సదిశ*  a;
    FT_సదిశ*  b;
    FT_UInt     n = 0;


    FT_OUTLINE_GET_CONTOUR( outline, c, మొదటి, చివరి );

    కొరకు ( a = మొదటి; a <= చివరి; a++ )
    {
      FT_Pos  x;
      FT_Int  intersect;


      b = ( a == చివరి ) ? మొదటి : a + 1;

      intersect = ( a->y - బిందువు->y ) ^ ( b->y - బిందువు->y );

      /* a and b are on the same side */
      ఒకవేళ ( intersect >= 0 )
      {
        ఒకవేళ ( intersect == 0 && a->y == బిందువు->y )
        {
          ఒకవేళ ( ( a->x <= బిందువు->x && b->x >= బిందువు->x ) ||
               ( a->x >= బిందువు->x && b->x <= బిందువు->x ) )
            తిరిగి 1;
        }

        కొనసాగు;
      }

      x = a->x + ( b->x - a->x ) * (బిందువు->y - a->y ) / ( b->y - a->y );

      ఒకవేళ ( x < బిందువు->x )
        n++;
      లేకపోతే ఒకవేళ ( x == బిందువు->x )
        తిరిగి 1;
    }

    తిరిగి n & 1;
  }


  స్థిర FT_Bool
  ft_contour_enclosed( FT_Outline*  outline,
                       FT_UShort    c )
  {
    FT_సదిశ*  మొదటి;
    FT_సదిశ*  చివరి;
    FT_Short    i;


    FT_OUTLINE_GET_CONTOUR( outline, c, మొదటి, చివరి );

    కొరకు ( i = 0; i < outline->n_contours; i++ )
    {
      ఒకవేళ ( i != c && ft_contour_has( outline, i, మొదటి ) )
      {
        FT_సదిశ*  pt;


        కొరకు ( pt = మొదటి + 1; pt <= చివరి; pt++ )
          ఒకవేళ ( !ft_contour_has( outline, i, pt ) )
            తిరిగి 0;

        తిరిగి 1;
      }
    }

    తిరిగి 0;
  }


  /* This version differs from the public one in that each */
  /* part (contour not enclosed in another contour) of the */
  /* outline is checked for orientation.  This is          */
  /* necessary for some buggy CJK fonts.                   */
  స్థిర FT_Orientation
  ft_outline_get_orientation( FT_Outline*  outline )
  {
    FT_Short        i;
    FT_సదిశ*      మొదటి;
    FT_సదిశ*      చివరి;
    FT_Orientation  orient = FT_ORIENTATION_NONE;


    మొదటి = outline->points;
    కొరకు ( i = 0; i < outline->n_contours; i++, మొదటి = చివరి + 1 )
    {
      FT_సదిశ*  బిందువు;
      FT_సదిశ*  xmin_బిందువు;
      FT_Pos      xmin;


      చివరి = outline->points + outline->contours[i];

      /* skip degenerate contours */
      ఒకవేళ ( చివరి < మొదటి + 2 )
        కొనసాగు;

      ఒకవేళ ( ft_contour_enclosed( outline, i ) )
        కొనసాగు;

      xmin       = మొదటి->x;
      xmin_బిందువు = మొదటి;

      కొరకు ( బిందువు = మొదటి + 1; బిందువు <= చివరి; బిందువు++ )
      {
        ఒకవేళ ( బిందువు->x < xmin )
        {
          xmin       = బిందువు->x;
          xmin_బిందువు = బిందువు;
        }
      }

      /* check the orientation of the contour */
      {
        FT_సదిశ*      prev;
        FT_సదిశ*      తదుపరి;
        FT_Orientation  o;


        prev = ( xmin_బిందువు == మొదటి ) ? చివరి : xmin_బిందువు - 1;
        తదుపరి = ( xmin_బిందువు == చివరి ) ? మొదటి : xmin_బిందువు + 1;

        ఒకవేళ ( FT_Atan2( prev->x - xmin_బిందువు->x, prev->y - xmin_బిందువు->y ) >
             FT_Atan2( తదుపరి->x - xmin_బిందువు->x, తదుపరి->y - xmin_బిందువు->y ) )
          o = FT_ORIENTATION_POSTSCRIPT;
        లేకపోతే
          o = FT_ORIENTATION_TRUETYPE;

        ఒకవేళ ( orient == FT_ORIENTATION_NONE )
          orient = o;
        లేకపోతే ఒకవేళ ( orient != o )
          తిరిగి FT_ORIENTATION_NONE;
      }
    }

    తిరిగి orient;
  }

#అయితే_ముగింపు /* 0 */


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Outline_Embolden( FT_Outline*  outline,
                       FT_Pos       strength )
  {
    తిరిగి FT_Outline_EmboldenXY( outline, strength, strength );
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Outline_EmboldenXY( FT_Outline*  outline,
                         FT_Pos       xstrength,
                         FT_Pos       ystrength )
  {
    FT_సదిశ*      points;
    FT_Int          c, మొదటి, చివరి;
    FT_Orientation  orientation;


    ఒకవేళ ( !outline )
      తిరిగి FT_THROW( Invalid_Outline );

    xstrength /= 2;
    ystrength /= 2;
    ఒకవేళ ( xstrength == 0 && ystrength == 0 )
      తిరిగి FT_Err_Ok;

    orientation = FT_Outline_Get_Orientation( outline );
    ఒకవేళ ( orientation == FT_ORIENTATION_NONE )
    {
      ఒకవేళ ( outline->n_contours )
        తిరిగి FT_THROW( Invalid_వాదం );
      లేకపోతే
        తిరిగి FT_Err_Ok;
    }

    points = outline->points;

    చివరి = -1;
    కొరకు ( c = 0; c < outline->n_contours; c++ )
    {
      FT_సదిశ  లోపల, బహిర్గతం, anchor, జరుపు;
      FT_Fixed   l_లోపల, l_బహిర్గతం, l_anchor = 0, l, q, d;
      FT_Int     i, j, k;


      మొదటి = చివరి + 1;
      చివరి  = outline->contours[c];
      l_లోపల  = 0;

      /* pacify compiler */
      లోపల.x = లోపల.y = anchor.x = anchor.y = 0;

      /* Counter j cycles though the points; counter i advances only  */
      /* when points are moved; anchor k marks the first moved point. */
      కొరకు ( i = చివరి, j = మొదటి, k = -1;
            j != i && i != k;
            j = j < చివరి ? j + 1 : మొదటి )
      {
        ఒకవేళ ( j != k )
        {
          బహిర్గతం.x = points[j].x - points[i].x;
          బహిర్గతం.y = points[j].y - points[i].y;
          l_బహిర్గతం = (FT_Fixed)FT_సదిశ_Norm_Len( &బహిర్గతం );

          ఒకవేళ ( l_బహిర్గతం == 0 )
            కొనసాగు;
        }
        లేకపోతే
        {
          బహిర్గతం   = anchor;
          l_బహిర్గతం = l_anchor;
        }

        ఒకవేళ ( l_లోపల != 0 )
        {
          ఒకవేళ ( k < 0 )
          {
            k        = i;
            anchor   = లోపల;
            l_anchor = l_లోపల;
          }

          d = FT_MulFix( లోపల.x, బహిర్గతం.x ) + FT_MulFix( లోపల.y, బహిర్గతం.y );

          /* shift only if turn is less than ~160 degrees */
          ఒకవేళ ( d > -0xF000L )
          {
            d = d + 0x10000L;

            /* shift components along lateral bisector in proper orientation */
            జరుపు.x = లోపల.y + బహిర్గతం.y;
            జరుపు.y = లోపల.x + బహిర్గతం.x;

            ఒకవేళ ( orientation == FT_ORIENTATION_TRUETYPE )
              జరుపు.x = -జరుపు.x;
            లేకపోతే
              జరుపు.y = -జరుపు.y;

            /* restrict shift magnitude to better handle collapsing segments */
            q = FT_MulFix( బహిర్గతం.x, లోపల.y ) - FT_MulFix( బహిర్గతం.y, లోపల.x );
            ఒకవేళ ( orientation == FT_ORIENTATION_TRUETYPE )
              q = -q;

            l = FT_MIN( l_లోపల, l_బహిర్గతం );

            /* non-strict inequalities avoid divide-by-zero when q == l == 0 */
            ఒకవేళ ( FT_MulFix( xstrength, q ) <= FT_MulFix( l, d ) )
              జరుపు.x = FT_MulDiv( జరుపు.x, xstrength, d );
            లేకపోతే
              జరుపు.x = FT_MulDiv( జరుపు.x, l, q );


            ఒకవేళ ( FT_MulFix( ystrength, q ) <= FT_MulFix( l, d ) )
              జరుపు.y = FT_MulDiv( జరుపు.y, ystrength, d );
            లేకపోతే
              జరుపు.y = FT_MulDiv( జరుపు.y, l, q );
          }
          లేకపోతే
            జరుపు.x = జరుపు.y = 0;

          కొరకు ( ;
                i != j;
                i = i < చివరి ? i + 1 : మొదటి )
          {
            points[i].x += xstrength + జరుపు.x;
            points[i].y += ystrength + జరుపు.y;
          }
        }
        లేకపోతే
          i = j;

        లోపల   = బహిర్గతం;
        l_లోపల = l_బహిర్గతం;
      }
    }

    తిరిగి FT_Err_Ok;
  }


  /* documentation is in ftoutln.h */

  FT_EXPORT_DEF( FT_Orientation )
  FT_Outline_Get_Orientation( FT_Outline*  outline )
  {
    FT_సదిశ*  points;
    FT_Int      c, n, మొదటి, చివరి;

#నిర్వచితమైతే FT_INT64
    FT_Int64    area = 0;
#లేకపోతే
    FT_BBox     cbox = { 0, 0, 0, 0 };
    FT_Int      xshift, yshift;
    FT_సదిశ   v_prev, v_cur;
    FT_Pos      area = 0;
#అయితే_ముగింపు


    ఒకవేళ ( !outline || outline->n_points <= 0 )
      తిరిగి FT_ORIENTATION_TRUETYPE;

    /* We use the nonzero winding rule to find the orientation.       */
    /* Since glyph outlines behave much more `regular' than arbitrary */
    /* cubic or quadratic curves, this test deals with the polygon    */
    /* only that is spanned up by the control points.                 */

    points = outline->points;

#నిర్వచితమైతే FT_INT64

    /* Single-pass shoelace.  The 64-bit accumulator removes the need   */
    /* for the coordinate shift pre-pass (and its `FT_Outline_Get_CBox` */
    /* traversal) that the 32-bit variant below uses.                   */
    చివరి = -1;
    కొరకు ( c = 0; c < outline->n_contours; c++ )
    {
      FT_Pos  prev_x, prev_y;


      మొదటి = చివరి + 1;
      చివరి  = outline->contours[c];

      prev_x = points[చివరి].x;
      prev_y = points[చివరి].y;

      కొరకు ( n = మొదటి; n <= చివరి; n++ )
      {
        FT_Pos  cur_x = points[n].x;
        FT_Pos  cur_y = points[n].y;


        area = కూడు_INT64( area,
                          MUL_INT64( SUB_INT64( cur_y, prev_y ),
                                     కూడు_INT64( cur_x, prev_x ) ) );

        prev_x = cur_x;
        prev_y = cur_y;
      }
    }

#లేకపోతే /* !FT_INT64 */

    FT_Outline_Get_CBox( outline, &cbox );

    /* Handle collapsed outlines to avoid undefined FT_MSB. */
    ఒకవేళ ( cbox.xMin == cbox.xMax || cbox.yMin == cbox.yMax )
      తిరిగి FT_ORIENTATION_NONE;

    /* Reject values large outlines. */
    ఒకవేళ ( cbox.xMin < -0x1000000L || cbox.yMin < -0x1000000L ||
         cbox.xMax >  0x1000000L || cbox.yMax >  0x1000000L )
      తిరిగి FT_ORIENTATION_NONE;

    xshift = FT_MSB( (FT_UInt32)( FT_ABS( cbox.xMax ) |
                                  FT_ABS( cbox.xMin ) ) ) - 14;
    xshift = FT_MAX( xshift, 0 );

    yshift = FT_MSB( (FT_UInt32)( cbox.yMax - cbox.yMin ) ) - 14;
    yshift = FT_MAX( yshift, 0 );

    చివరి = -1;
    కొరకు ( c = 0; c < outline->n_contours; c++ )
    {
      మొదటి = చివరి + 1;
      చివరి  = outline->contours[c];

      v_prev.x = points[చివరి].x >> xshift;
      v_prev.y = points[చివరి].y >> yshift;

      కొరకు ( n = మొదటి; n <= చివరి; n++ )
      {
        v_cur.x = points[n].x >> xshift;
        v_cur.y = points[n].y >> yshift;

        area = కూడు_LONG( area,
                         MUL_LONG( v_cur.y - v_prev.y,
                                   v_cur.x + v_prev.x ) );

        v_prev = v_cur;
      }
    }

#అయితే_ముగింపు /* FT_INT64 */

    ఒకవేళ ( area > 0 )
      తిరిగి FT_ORIENTATION_POSTSCRIPT;
    లేకపోతే ఒకవేళ ( area < 0 )
      తిరిగి FT_ORIENTATION_TRUETYPE;
    లేకపోతే
      తిరిగి FT_ORIENTATION_NONE;
  }


/*చివర*/
