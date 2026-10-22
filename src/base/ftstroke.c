/****************************************************************************
 *
 * ftstroke.c
 *
 *   FreeType path stroker (body).
 *
 * Copyright (C) 2002-2026 by
 * David Turner, Robert Wilhelm, and Werner Lemberg.
 *
 * This file is part of the FreeType project, and may only be used,
 * modified, and distributed under the terms of the FreeType project
 * license, LICENSE.TXT.  By continuing to use, modify, or distribute
 * this file you indicate that you have read the license and
 * understand and accept it fully.
 *
 */


#అన్తర్భావ <freetype/ftstroke.h>
#అన్తర్భావ <freetype/fttrigon.h>
#అన్తర్భావ <freetype/ftoutln.h>
#అన్తర్భావ <freetype/internal/ftmemory.h>
#అన్తర్భావ <freetype/internal/ftdebug.h>
#అన్తర్భావ <freetype/internal/ftobjs.h>


  /* declare an extern to access `ft_outline_glyph_class' globally */
  /* allocated  in `ftglyph.c'                                     */
  FT_CALLBACK_పట్టిక స్థిరరాశి FT_Glyph_Class  ft_outline_glyph_class;


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_StrokerBorder )
  FT_Outline_GetInsideBorder( FT_Outline*  outline )
  {
    FT_Orientation  o = FT_Outline_Get_Orientation( outline );


    తిరిగి o == FT_ORIENTATION_TRUETYPE ? FT_STROKER_BORDER_కుడి
                                        : FT_STROKER_BORDER_ఎడమ;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_StrokerBorder )
  FT_Outline_GetOutsideBorder( FT_Outline*  outline )
  {
    FT_Orientation  o = FT_Outline_Get_Orientation( outline );


    తిరిగి o == FT_ORIENTATION_TRUETYPE ? FT_STROKER_BORDER_ఎడమ
                                        : FT_STROKER_BORDER_కుడి;
  }


  /*************************************************************************/
  /*************************************************************************/
  /*****                                                               *****/
  /*****                      BEZIER COMPUTATIONS                      *****/
  /*****                                                               *****/
  /*************************************************************************/
  /*************************************************************************/

#నిర్వచించు FT_SMALL_CONIC_పరిమితి  ( FT_ANGLE_PI / 6 )
#నిర్వచించు FT_SMALL_CUBIC_పరిమితి  ( FT_ANGLE_PI / 8 )

#నిర్వచించు FT_EPSILON  2

#నిర్వచించు FT_IS_SMALL( x )  ( (x) > -FT_EPSILON && (x) < FT_EPSILON )


  స్థిర FT_Pos
  ft_pos_abs( FT_Pos  x )
  {
    తిరిగి x >= 0 ? x : -x;
  }


  స్థిర శూన్య
  ft_conic_విభజించు( FT_సదిశ*  పునాది )
  {
    FT_Pos  a, b;


    పునాది[4].x = పునాది[2].x;
    a = పునాది[0].x + పునాది[1].x;
    b = పునాది[1].x + పునాది[2].x;
    పునాది[3].x = b >> 1;
    పునాది[2].x = ( a + b ) >> 2;
    పునాది[1].x = a >> 1;

    పునాది[4].y = పునాది[2].y;
    a = పునాది[0].y + పునాది[1].y;
    b = పునాది[1].y + పునాది[2].y;
    పునాది[3].y = b >> 1;
    పునాది[2].y = ( a + b ) >> 2;
    పునాది[1].y = a >> 1;
  }


  స్థిర FT_Bool
  ft_conic_is_small_enough( FT_సదిశ*  పునాది,
                            FT_Angle   *angle_లోపల,
                            FT_Angle   *angle_బహిర్గతం )
  {
    FT_సదిశ  d1, d2;
    FT_Angle   theta;
    FT_Int     close1, close2;


    d1.x = పునాది[1].x - పునాది[2].x;
    d1.y = పునాది[1].y - పునాది[2].y;
    d2.x = పునాది[0].x - పునాది[1].x;
    d2.y = పునాది[0].y - పునాది[1].y;

    close1 = FT_IS_SMALL( d1.x ) && FT_IS_SMALL( d1.y );
    close2 = FT_IS_SMALL( d2.x ) && FT_IS_SMALL( d2.y );

    ఒకవేళ ( close1 )
    {
      ఒకవేళ ( close2 )
      {
        /* basically a point;                      */
        /* do nothing to retain original direction */
      }
      లేకపోతే
      {
        *angle_లోపల  =
        *angle_బహిర్గతం = FT_Atan2( d2.x, d2.y );
      }
    }
    లేకపోతే /* !close1 */
    {
      ఒకవేళ ( close2 )
      {
        *angle_లోపల  =
        *angle_బహిర్గతం = FT_Atan2( d1.x, d1.y );
      }
      లేకపోతే
      {
        *angle_లోపల  = FT_Atan2( d1.x, d1.y );
        *angle_బహిర్గతం = FT_Atan2( d2.x, d2.y );
      }
    }

    theta = ft_pos_abs( FT_Angle_Diff( *angle_లోపల, *angle_బహిర్గతం ) );

    తిరిగి FT_BOOL( theta < FT_SMALL_CONIC_పరిమితి );
  }


  స్థిర శూన్య
  ft_cubic_విభజించు( FT_సదిశ*  పునాది )
  {
    FT_Pos  a, b, c;


    పునాది[6].x = పునాది[3].x;
    a = పునాది[0].x + పునాది[1].x;
    b = పునాది[1].x + పునాది[2].x;
    c = పునాది[2].x + పునాది[3].x;
    పునాది[5].x = c >> 1;
    c += b;
    పునాది[4].x = c >> 2;
    పునాది[1].x = a >> 1;
    a += b;
    పునాది[2].x = a >> 2;
    పునాది[3].x = ( a + c ) >> 3;

    పునాది[6].y = పునాది[3].y;
    a = పునాది[0].y + పునాది[1].y;
    b = పునాది[1].y + పునాది[2].y;
    c = పునాది[2].y + పునాది[3].y;
    పునాది[5].y = c >> 1;
    c += b;
    పునాది[4].y = c >> 2;
    పునాది[1].y = a >> 1;
    a += b;
    పునాది[2].y = a >> 2;
    పునాది[3].y = ( a + c ) >> 3;
  }


  /* Return the average of `angle1' and `angle2'.            */
  /* This gives correct result even if `angle1' and `angle2' */
  /* have opposite signs.                                    */
  స్థిర FT_Angle
  ft_angle_సగటు( FT_Angle  angle1,
                 FT_Angle  angle2 )
  {
    తిరిగి angle1 + FT_Angle_Diff( angle1, angle2 ) / 2;
  }


  స్థిర FT_Bool
  ft_cubic_is_small_enough( FT_సదిశ*  పునాది,
                            FT_Angle   *angle_లోపల,
                            FT_Angle   *angle_mid,
                            FT_Angle   *angle_బహిర్గతం )
  {
    FT_సదిశ  d1, d2, d3;
    FT_Angle   theta1, theta2;
    FT_Int     close1, close2, close3;


    d1.x = పునాది[2].x - పునాది[3].x;
    d1.y = పునాది[2].y - పునాది[3].y;
    d2.x = పునాది[1].x - పునాది[2].x;
    d2.y = పునాది[1].y - పునాది[2].y;
    d3.x = పునాది[0].x - పునాది[1].x;
    d3.y = పునాది[0].y - పునాది[1].y;

    close1 = FT_IS_SMALL( d1.x ) && FT_IS_SMALL( d1.y );
    close2 = FT_IS_SMALL( d2.x ) && FT_IS_SMALL( d2.y );
    close3 = FT_IS_SMALL( d3.x ) && FT_IS_SMALL( d3.y );

    ఒకవేళ ( close1 )
    {
      ఒకవేళ ( close2 )
      {
        ఒకవేళ ( close3 )
        {
          /* basically a point;                      */
          /* do nothing to retain original direction */
        }
        లేకపోతే /* !close3 */
        {
          *angle_లోపల  =
          *angle_mid =
          *angle_బహిర్గతం = FT_Atan2( d3.x, d3.y );
        }
      }
      లేకపోతే /* !close2 */
      {
        ఒకవేళ ( close3 )
        {
          *angle_లోపల  =
          *angle_mid =
          *angle_బహిర్గతం = FT_Atan2( d2.x, d2.y );
        }
        లేకపోతే /* !close3 */
        {
          *angle_లోపల  =
          *angle_mid = FT_Atan2( d2.x, d2.y );
          *angle_బహిర్గతం = FT_Atan2( d3.x, d3.y );
        }
      }
    }
    లేకపోతే /* !close1 */
    {
      ఒకవేళ ( close2 )
      {
        ఒకవేళ ( close3 )
        {
          *angle_లోపల  =
          *angle_mid =
          *angle_బహిర్గతం = FT_Atan2( d1.x, d1.y );
        }
        లేకపోతే /* !close3 */
        {
          *angle_లోపల  = FT_Atan2( d1.x, d1.y );
          *angle_బహిర్గతం = FT_Atan2( d3.x, d3.y );
          *angle_mid = ft_angle_సగటు( *angle_లోపల, *angle_బహిర్గతం );
        }
      }
      లేకపోతే /* !close2 */
      {
        ఒకవేళ ( close3 )
        {
          *angle_లోపల  = FT_Atan2( d1.x, d1.y );
          *angle_mid =
          *angle_బహిర్గతం = FT_Atan2( d2.x, d2.y );
        }
        లేకపోతే /* !close3 */
        {
          *angle_లోపల  = FT_Atan2( d1.x, d1.y );
          *angle_mid = FT_Atan2( d2.x, d2.y );
          *angle_బహిర్గతం = FT_Atan2( d3.x, d3.y );
        }
      }
    }

    theta1 = ft_pos_abs( FT_Angle_Diff( *angle_లోపల,  *angle_mid ) );
    theta2 = ft_pos_abs( FT_Angle_Diff( *angle_mid, *angle_బహిర్గతం ) );

    తిరిగి FT_BOOL( theta1 < FT_SMALL_CUBIC_పరిమితి &&
                    theta2 < FT_SMALL_CUBIC_పరిమితి );
  }


  /*************************************************************************/
  /*************************************************************************/
  /*****                                                               *****/
  /*****                       STROKE BORDERS                          *****/
  /*****                                                               *****/
  /*************************************************************************/
  /*************************************************************************/

  రకనిర్వచనం గణన  FT_Stroke_ట్యాగులు
  {
    FT_STROKE_ట్యాగ్_ON    = 1,   /* on-curve point  */
    FT_STROKE_ట్యాగ్_CUBIC = 2,   /* cubic off-point */
    FT_STROKE_ట్యాగ్_BEGIN = 4,   /* sub-path start  */
    FT_STROKE_ట్యాగ్_చివర   = 8    /* sub-path end    */

  } FT_Stroke_ట్యాగులు_2;

#నిర్వచించు  FT_STROKE_ట్యాగ్_BEGIN_చివర  ( FT_STROKE_ట్యాగ్_BEGIN | FT_STROKE_ట్యాగ్_చివర )

  రకనిర్వచనం నిర్మాణం  FT_StrokeBorderRec_
  {
    FT_UInt     అంకె_points;
    FT_UInt     max_points;
    FT_సదిశ*  points;
    FT_Byte*    ట్యాగులు;
    FT_Bool     movable;  /* TRUE for ends of lineto borders */
    FT_Int      ప్రారంభం;    /* index of current sub-path start point */
    FT_జ్ఞాపకం   జ్ఞాపకం;
    FT_Bool     valid;

  } FT_StrokeBorderRec, *FT_StrokeBorder;


  స్థిర FT_Error
  ft_stroke_border_grow( FT_StrokeBorder  border,
                         FT_UInt          new_points )
  {
    FT_UInt   old_max = border->max_points;
    FT_UInt   new_max = border->అంకె_points + new_points;
    FT_Error  error   = FT_Err_Ok;


    ఒకవేళ ( new_max > old_max )
    {
      FT_UInt    cur_max = old_max;
      FT_జ్ఞాపకం  జ్ఞాపకం  = border->జ్ఞాపకం;


      అయితే ( cur_max < new_max )
        cur_max += ( cur_max >> 1 ) + 16;

      ఒకవేళ ( FT_RENEW_ARRAY( border->points, old_max, cur_max ) ||
           FT_RENEW_ARRAY( border->ట్యాగులు,   old_max, cur_max ) )
        వెళ్ళు Exit;

      border->max_points = cur_max;
    }

  Exit:
    తిరిగి error;
  }


  స్థిర శూన్య
  ft_stroke_border_close( FT_StrokeBorder  border,
                          FT_Bool          తిరగేయి )
  {
    FT_UInt  ప్రారంభం = (FT_UInt)border->ప్రారంభం;
    FT_UInt  లెక్క = border->అంకె_points;


    FT_ASSERT( border->ప్రారంభం >= 0 );

    /* don't record empty paths! */
    ఒకవేళ ( లెక్క <= ప్రారంభం + 1U )
      border->అంకె_points = ప్రారంభం;
    లేకపోతే
    {
      /* copy the last point to the start of this sub-path, since */
      /* it contains the `adjusted' starting coordinates          */
      border->అంకె_points    = --లెక్క;
      border->points[ప్రారంభం] = border->points[లెక్క];
      border->ట్యాగులు[ప్రారంభం]   = border->ట్యాగులు[లెక్క];

      ఒకవేళ ( తిరగేయి )
      {
        /* reverse the points */
        {
          FT_సదిశ*  vec1 = border->points + ప్రారంభం + 1;
          FT_సదిశ*  vec2 = border->points + లెక్క - 1;


          కొరకు ( ; vec1 < vec2; vec1++, vec2-- )
          {
            FT_సదిశ  tmp;


            tmp   = *vec1;
            *vec1 = *vec2;
            *vec2 = tmp;
          }
        }

        /* then the tags */
        {
          FT_Byte*  tag1 = border->ట్యాగులు + ప్రారంభం + 1;
          FT_Byte*  tag2 = border->ట్యాగులు + లెక్క - 1;


          కొరకు ( ; tag1 < tag2; tag1++, tag2-- )
          {
            FT_Byte  tmp;


            tmp   = *tag1;
            *tag1 = *tag2;
            *tag2 = tmp;
          }
        }
      }

      border->ట్యాగులు[ప్రారంభం    ] |= FT_STROKE_ట్యాగ్_BEGIN;
      border->ట్యాగులు[లెక్క - 1] |= FT_STROKE_ట్యాగ్_చివర;
    }

    border->ప్రారంభం   = -1;
    border->movable = FALSE;
  }


  స్థిర FT_Error
  ft_stroke_border_lineto( FT_StrokeBorder  border,
                           FT_సదిశ*       గమ్యం,
                           FT_Bool          movable )
  {
    FT_Error  error = FT_Err_Ok;


    FT_ASSERT( border->ప్రారంభం >= 0 );

    ఒకవేళ ( border->movable )
    {
      /* move last point */
      border->points[border->అంకె_points - 1] = *గమ్యం;
    }
    లేకపోతే
    {
      /* don't add zero-length lineto, but always add moveto */
      ఒకవేళ ( border->అంకె_points > (FT_UInt)border->ప్రారంభం                     &&
           FT_IS_SMALL( border->points[border->అంకె_points - 1].x - గమ్యం->x ) &&
           FT_IS_SMALL( border->points[border->అంకె_points - 1].y - గమ్యం->y ) )
        తిరిగి error;

      /* add one point */
      error = ft_stroke_border_grow( border, 1 );
      ఒకవేళ ( !error )
      {
        FT_సదిశ*  vec = border->points + border->అంకె_points;
        FT_Byte*    ట్యాగ్ = border->ట్యాగులు   + border->అంకె_points;


        vec[0] = *గమ్యం;
        ట్యాగ్[0] = FT_STROKE_ట్యాగ్_ON;

        border->అంకె_points += 1;
      }
    }
    border->movable = movable;
    తిరిగి error;
  }


  స్థిర FT_Error
  ft_stroke_border_conicto( FT_StrokeBorder  border,
                            FT_సదిశ*       control,
                            FT_సదిశ*       గమ్యం )
  {
    FT_Error  error;


    FT_ASSERT( border->ప్రారంభం >= 0 );

    error = ft_stroke_border_grow( border, 2 );
    ఒకవేళ ( !error )
    {
      FT_సదిశ*  vec = border->points + border->అంకె_points;
      FT_Byte*    ట్యాగ్ = border->ట్యాగులు   + border->అంకె_points;


      vec[0] = *control;
      vec[1] = *గమ్యం;

      ట్యాగ్[0] = 0;
      ట్యాగ్[1] = FT_STROKE_ట్యాగ్_ON;

      border->అంకె_points += 2;
    }

    border->movable = FALSE;

    తిరిగి error;
  }


  స్థిర FT_Error
  ft_stroke_border_cubicto( FT_StrokeBorder  border,
                            FT_సదిశ*       control1,
                            FT_సదిశ*       control2,
                            FT_సదిశ*       గమ్యం )
  {
    FT_Error  error;


    FT_ASSERT( border->ప్రారంభం >= 0 );

    error = ft_stroke_border_grow( border, 3 );
    ఒకవేళ ( !error )
    {
      FT_సదిశ*  vec = border->points + border->అంకె_points;
      FT_Byte*    ట్యాగ్ = border->ట్యాగులు   + border->అంకె_points;


      vec[0] = *control1;
      vec[1] = *control2;
      vec[2] = *గమ్యం;

      ట్యాగ్[0] = FT_STROKE_ట్యాగ్_CUBIC;
      ట్యాగ్[1] = FT_STROKE_ట్యాగ్_CUBIC;
      ట్యాగ్[2] = FT_STROKE_ట్యాగ్_ON;

      border->అంకె_points += 3;
    }

    border->movable = FALSE;

    తిరిగి error;
  }


#నిర్వచించు FT_ARC_CUBIC_ANGLE  ( FT_ANGLE_PI / 2 )


  స్థిర FT_Error
  ft_stroke_border_arcto( FT_StrokeBorder  border,
                          FT_సదిశ*       center,
                          FT_Fixed         radius,
                          FT_Angle         angle_ప్రారంభం,
                          FT_Angle         angle_diff )
  {
    FT_Fixed   coef;
    FT_సదిశ  a0, a1, a2, a3;
    FT_Int     i, arcs = 1;
    FT_Error   error = FT_Err_Ok;


    /* number of cubic arcs to draw */
    అయితే (  angle_diff > FT_ARC_CUBIC_ANGLE * arcs ||
            -angle_diff > FT_ARC_CUBIC_ANGLE * arcs )
      arcs++;

    /* control tangents */
    coef  = FT_Tan( angle_diff / ( 4 * arcs ) );
    coef += coef / 3;

    /* compute start and first control point */
    FT_సదిశ_From_Polar( &a0, radius, angle_ప్రారంభం );
    a1.x = FT_MulFix( -a0.y, coef );
    a1.y = FT_MulFix(  a0.x, coef );

    a0.x += center->x;
    a0.y += center->y;
    a1.x += a0.x;
    a1.y += a0.y;

    కొరకు ( i = 1; i <= arcs; i++ )
    {
      /* compute end and second control point */
      FT_సదిశ_From_Polar( &a3, radius,
                            angle_ప్రారంభం + i * angle_diff / arcs );
      a2.x = FT_MulFix(  a3.y, coef );
      a2.y = FT_MulFix( -a3.x, coef );

      a3.x += center->x;
      a3.y += center->y;
      a2.x += a3.x;
      a2.y += a3.y;

      /* add cubic arc */
      error = ft_stroke_border_cubicto( border, &a1, &a2, &a3 );
      ఒకవేళ ( error )
        ఆపు;

      /* a0 = a3; */
      a1.x = a3.x - a2.x + a3.x;
      a1.y = a3.y - a2.y + a3.y;
    }

    తిరిగి error;
  }


  స్థిర FT_Error
  ft_stroke_border_moveto( FT_StrokeBorder  border,
                           FT_సదిశ*       గమ్యం )
  {
    /* close current open path if any ? */
    ఒకవేళ ( border->ప్రారంభం >= 0 )
      ft_stroke_border_close( border, FALSE );

    border->ప్రారంభం = (FT_Int)border->అంకె_points;
    border->movable = FALSE;

    తిరిగి ft_stroke_border_lineto( border, గమ్యం, FALSE );
  }


  స్థిర శూన్య
  ft_stroke_border_init( FT_StrokeBorder  border,
                         FT_జ్ఞాపకం        జ్ఞాపకం )
  {
    border->జ్ఞాపకం = జ్ఞాపకం;
    border->points = ఖాళీ;
    border->ట్యాగులు   = ఖాళీ;

    border->అంకె_points = 0;
    border->max_points = 0;
    border->ప్రారంభం      = -1;
    border->valid      = FALSE;
  }


  స్థిర శూన్య
  ft_stroke_border_reset( FT_StrokeBorder  border )
  {
    border->అంకె_points = 0;
    border->ప్రారంభం      = -1;
    border->valid      = FALSE;
  }


  స్థిర శూన్య
  ft_stroke_border_పూర్తి( FT_StrokeBorder  border )
  {
    FT_జ్ఞాపకం  జ్ఞాపకం = border->జ్ఞాపకం;


    FT_FREE( border->points );
    FT_FREE( border->ట్యాగులు );

    border->అంకె_points = 0;
    border->max_points = 0;
    border->ప్రారంభం      = -1;
    border->valid      = FALSE;
  }


  స్థిర FT_Error
  ft_stroke_border_get_counts( FT_StrokeBorder  border,
                               FT_UInt         *anum_points,
                               FT_UInt         *anum_contours )
  {
    FT_Error  error        = FT_Err_Ok;
    FT_UInt   అంకె_points   = 0;
    FT_UInt   అంకె_contours = 0;

    FT_UInt     లెక్క      = border->అంకె_points;
    FT_Byte*    ట్యాగులు       = border->ట్యాగులు;
    FT_Int      లోపల_contour = 0;


    కొరకు ( ; లెక్క > 0; లెక్క--, అంకె_points++, ట్యాగులు++ )
    {
      ఒకవేళ ( ట్యాగులు[0] & FT_STROKE_ట్యాగ్_BEGIN )
      {
        ఒకవేళ ( లోపల_contour != 0 )
          వెళ్ళు Fail;

        లోపల_contour = 1;
      }
      లేకపోతే ఒకవేళ ( లోపల_contour == 0 )
        వెళ్ళు Fail;

      ఒకవేళ ( ట్యాగులు[0] & FT_STROKE_ట్యాగ్_చివర )
      {
        లోపల_contour = 0;
        అంకె_contours++;
      }
    }

    ఒకవేళ ( లోపల_contour != 0 )
      వెళ్ళు Fail;

    border->valid = TRUE;

  Exit:
    *anum_points   = అంకె_points;
    *anum_contours = అంకె_contours;
    తిరిగి error;

  Fail:
    అంకె_points   = 0;
    అంకె_contours = 0;
    error        = FT_THROW( Invalid_Outline );
    వెళ్ళు Exit;
  }


  స్థిర శూన్య
  ft_stroke_border_export( FT_StrokeBorder  border,
                           FT_Outline*      outline )
  {
    /* copy point locations */
    ఒకవేళ ( border->అంకె_points )
      FT_ARRAY_COPY( outline->points + outline->n_points,
                     border->points,
                     border->అంకె_points );

    /* copy tags */
    {
      FT_UInt   లెక్క = border->అంకె_points;
      FT_Byte*  పఠ  = border->ట్యాగులు;
      FT_Byte*  లిఖ = outline->ట్యాగులు + outline->n_points;


      కొరకు ( ; లెక్క > 0; లెక్క--, పఠ++, లిఖ++ )
      {
        ఒకవేళ ( *పఠ & FT_STROKE_ట్యాగ్_ON )
          *లిఖ = FT_వక్రం_ట్యాగ్_ON;
        లేకపోతే ఒకవేళ ( *పఠ & FT_STROKE_ట్యాగ్_CUBIC )
          *లిఖ = FT_వక్రం_ట్యాగ్_CUBIC;
        లేకపోతే
          *లిఖ = FT_వక్రం_ట్యాగ్_CONIC;
      }
    }

    /* copy contours */
    {
      FT_UInt     లెక్క = border->అంకె_points;
      FT_Byte*    ట్యాగులు  = border->ట్యాగులు;
      FT_UShort*  లిఖ = outline->contours + outline->n_contours;
      FT_UShort   idx   = outline->n_points;


      కొరకు ( ; లెక్క > 0; లెక్క--, ట్యాగులు++, idx++ )
      {
        ఒకవేళ ( *ట్యాగులు & FT_STROKE_ట్యాగ్_చివర )
        {
          *లిఖ++ = idx;
          outline->n_contours++;
        }
      }
    }

    outline->n_points += (FT_UShort)border->అంకె_points;

    FT_ASSERT( FT_Outline_Check( outline ) == 0 );
  }


  /*************************************************************************/
  /*************************************************************************/
  /*****                                                               *****/
  /*****                           STROKER                             *****/
  /*****                                                               *****/
  /*************************************************************************/
  /*************************************************************************/

#నిర్వచించు FT_SIDE_గమ్యం_ROTATE( s )   ( FT_ANGLE_PI2 - (s) * FT_ANGLE_PI )

  రకనిర్వచనం నిర్మాణం  FT_StrokerRec_
  {
    FT_Angle             angle_లోపల;             /* direction into curr join */
    FT_Angle             angle_బహిర్గతం;            /* direction out of join  */
    FT_సదిశ            center;               /* current position */
    FT_Fixed             పంక్తి_నిడివి;          /* length of last lineto */
    FT_Bool              మొదటి_బిందువు;          /* is this the start? */
    FT_Bool              subpath_open;         /* is the subpath open? */
    FT_Angle             subpath_angle;        /* subpath start direction */
    FT_సదిశ            subpath_ప్రారంభం;        /* subpath start position */
    FT_Fixed             subpath_పంక్తి_నిడివి;  /* subpath start lineto len */
    FT_Bool              handle_wide_strokes;  /* use wide strokes logic? */

    FT_Stroker_పంక్తి_Cap   పంక్తి_cap;
    FT_Stroker_పంక్తి_Join  పంక్తి_join;
    FT_Stroker_పంక్తి_Join  పంక్తి_join_saved;
    FT_Fixed             miter_హద్దు;
    FT_Fixed             radius;

    FT_StrokeBorderRec   borders[2];
    FT_Library           library;

  } FT_StrokerRec;


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Stroker_New( FT_Library   library,
                  FT_Stroker  *astroker )
  {
    FT_Error    error;           /* assigned in FT_NEW */
    FT_జ్ఞాపకం   జ్ఞాపకం;
    FT_Stroker  stroker = ఖాళీ;


    ఒకవేళ ( !library )
      తిరిగి FT_THROW( Invalid_Library_Handle );

    ఒకవేళ ( !astroker )
      తిరిగి FT_THROW( Invalid_వాదం );

    జ్ఞాపకం = library->జ్ఞాపకం;

    ఒకవేళ ( !FT_NEW( stroker ) )
    {
      stroker->library = library;

      ft_stroke_border_init( &stroker->borders[0], జ్ఞాపకం );
      ft_stroke_border_init( &stroker->borders[1], జ్ఞాపకం );
    }

    *astroker = stroker;

    తిరిగి error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( శూన్య )
  FT_Stroker_సమితి( FT_Stroker           stroker,
                  FT_Fixed             radius,
                  FT_Stroker_పంక్తి_Cap   పంక్తి_cap,
                  FT_Stroker_పంక్తి_Join  పంక్తి_join,
                  FT_Fixed             miter_హద్దు )
  {
    ఒకవేళ ( !stroker )
      తిరిగి;

    stroker->radius      = radius;
    stroker->పంక్తి_cap    = పంక్తి_cap;
    stroker->పంక్తి_join   = పంక్తి_join;
    stroker->miter_హద్దు = miter_హద్దు;

    /* ensure miter limit has sensible value */
    ఒకవేళ ( stroker->miter_హద్దు < 0x10000L )
      stroker->miter_హద్దు = 0x10000L;

    /* save line join style:                                           */
    /* line join style can be temporarily changed when stroking curves */
    stroker->పంక్తి_join_saved = పంక్తి_join;

    FT_Stroker_Rewind( stroker );
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( శూన్య )
  FT_Stroker_Rewind( FT_Stroker  stroker )
  {
    ఒకవేళ ( stroker )
    {
      ft_stroke_border_reset( &stroker->borders[0] );
      ft_stroke_border_reset( &stroker->borders[1] );
    }
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( శూన్య )
  FT_Stroker_పూర్తి( FT_Stroker  stroker )
  {
    ఒకవేళ ( stroker )
    {
      FT_జ్ఞాపకం  జ్ఞాపకం = stroker->library->జ్ఞాపకం;


      ft_stroke_border_పూర్తి( &stroker->borders[0] );
      ft_stroke_border_పూర్తి( &stroker->borders[1] );

      stroker->library = ఖాళీ;
      FT_FREE( stroker );
    }
  }


  /* create a circular arc at a corner or cap */
  స్థిర FT_Error
  ft_stroker_arcto( FT_Stroker  stroker,
                    FT_Int      side )
  {
    FT_Angle         మొత్తం, rotate;
    FT_Fixed         radius = stroker->radius;
    FT_Error         error  = FT_Err_Ok;
    FT_StrokeBorder  border = stroker->borders + side;


    rotate = FT_SIDE_గమ్యం_ROTATE( side );

    మొత్తం = FT_Angle_Diff( stroker->angle_లోపల, stroker->angle_బహిర్గతం );
    ఒకవేళ ( మొత్తం == FT_ANGLE_PI )
      మొత్తం = -rotate * 2;

    error = ft_stroke_border_arcto( border,
                                    &stroker->center,
                                    radius,
                                    stroker->angle_లోపల + rotate,
                                    మొత్తం );
    border->movable = FALSE;
    తిరిగి error;
  }


  /* add a cap at the end of an opened path */
  స్థిర FT_Error
  ft_stroker_cap( FT_Stroker  stroker,
                  FT_Angle    angle,
                  FT_Int      side )
  {
    FT_Error  error = FT_Err_Ok;


    ఒకవేళ ( stroker->పంక్తి_cap == FT_STROKER_LINECAP_విడత )
    {
      /* add a round cap */
      stroker->angle_లోపల  = angle;
      stroker->angle_బహిర్గతం = angle + FT_ANGLE_PI;

      error = ft_stroker_arcto( stroker, side );
    }
    లేకపోతే
    {
      /* add a square or butt cap */
      FT_సదిశ        మధ్య, delta;
      FT_Fixed         radius = stroker->radius;
      FT_StrokeBorder  border = stroker->borders + side;


      /* compute middle point and first angle point */
      FT_సదిశ_From_Polar( &మధ్య, radius, angle );
      delta.x = side ?  మధ్య.y : -మధ్య.y;
      delta.y = side ? -మధ్య.x :  మధ్య.x;

      ఒకవేళ ( stroker->పంక్తి_cap == FT_STROKER_LINECAP_వర్గం )
      {
        మధ్య.x += stroker->center.x;
        మధ్య.y += stroker->center.y;
      }
      లేకపోతే  /* FT_STROKER_LINECAP_BUTT */
      {
        మధ్య.x  = stroker->center.x;
        మధ్య.y  = stroker->center.y;
      }

      delta.x  += మధ్య.x;
      delta.y  += మధ్య.y;

      error = ft_stroke_border_lineto( border, &delta, FALSE );
      ఒకవేళ ( error )
        వెళ్ళు Exit;

      /* compute second angle point */
      delta.x = మధ్య.x - delta.x + మధ్య.x;
      delta.y = మధ్య.y - delta.y + మధ్య.y;

      error = ft_stroke_border_lineto( border, &delta, FALSE );
    }

  Exit:
    తిరిగి error;
  }


  /* process an inside corner, i.e. compute intersection */
  స్థిర FT_Error
  ft_stroker_inside( FT_Stroker  stroker,
                     FT_Int      side,
                     FT_Fixed    పంక్తి_నిడివి )
  {
    FT_StrokeBorder  border = stroker->borders + side;
    FT_Angle         phi, theta, rotate;
    FT_Fixed         నిడివి;
    FT_సదిశ        sigma = { 0, 0 };
    FT_సదిశ        delta;
    FT_Error         error = FT_Err_Ok;
    FT_Bool          intersect;          /* use intersection of lines? */


    rotate = FT_SIDE_గమ్యం_ROTATE( side );

    theta = FT_Angle_Diff( stroker->angle_లోపల, stroker->angle_బహిర్గతం ) / 2;

    /* Only intersect borders if between two lineto's and both */
    /* lines are long enough (line_length is zero for curves). */
    /* Also avoid U-turns of nearly 180 degree.                */
    ఒకవేళ ( !border->movable || పంక్తి_నిడివి == 0  ||
         theta > 0x59C000 || theta < -0x59C000 )
      intersect = FALSE;
    లేకపోతే
    {
      /* compute minimum required length of lines */
      FT_Fixed  min_నిడివి;


      FT_సదిశ_Unit( &sigma, theta );
      min_నిడివి =
        ft_pos_abs( FT_MulDiv( stroker->radius, sigma.y, sigma.x ) );

      intersect = FT_BOOL( min_నిడివి                         &&
                           stroker->పంక్తి_నిడివి >= min_నిడివి &&
                           పంక్తి_నిడివి          >= min_నిడివి );
    }

    ఒకవేళ ( !intersect )
    {
      FT_సదిశ_From_Polar( &delta, stroker->radius,
                            stroker->angle_బహిర్గతం + rotate );
      delta.x += stroker->center.x;
      delta.y += stroker->center.y;

      border->movable = FALSE;
    }
    లేకపోతే
    {
      /* compute median angle */
      phi = stroker->angle_లోపల + theta + rotate;

      నిడివి = FT_DivFix( stroker->radius, sigma.x );

      FT_సదిశ_From_Polar( &delta, నిడివి, phi );
      delta.x += stroker->center.x;
      delta.y += stroker->center.y;
    }

    error = ft_stroke_border_lineto( border, &delta, FALSE );

    తిరిగి error;
  }


  /* process an outside corner, i.e. compute bevel/miter/round */
  స్థిర FT_Error
  ft_stroker_outside( FT_Stroker  stroker,
                      FT_Int      side,
                      FT_Fixed    పంక్తి_నిడివి )
  {
    FT_StrokeBorder  border = stroker->borders + side;
    FT_Error         error;
    FT_Angle         rotate;


    ఒకవేళ ( stroker->పంక్తి_join == FT_STROKER_LINEJOIN_విడత )
      error = ft_stroker_arcto( stroker, side );
    లేకపోతే
    {
      /* this is a mitered (pointed) or beveled (truncated) corner */
      FT_Fixed   radius = stroker->radius;
      FT_సదిశ  sigma = { 0, 0 };
      FT_Angle   theta = 0, phi = 0;
      FT_Bool    bevel, fixed_bevel;


      rotate = FT_SIDE_గమ్యం_ROTATE( side );

      bevel =
        FT_BOOL( stroker->పంక్తి_join == FT_STROKER_LINEJOIN_BEVEL );

      fixed_bevel =
        FT_BOOL( stroker->పంక్తి_join != FT_STROKER_LINEJOIN_MITER_VARIABLE );

      /* check miter limit first */
      ఒకవేళ ( !bevel )
      {
        theta = FT_Angle_Diff( stroker->angle_లోపల, stroker->angle_బహిర్గతం ) / 2;

        ఒకవేళ ( theta == FT_ANGLE_PI2 )
          theta = -rotate;

        phi = stroker->angle_లోపల + theta + rotate;

        FT_సదిశ_From_Polar( &sigma, stroker->miter_హద్దు, theta );

        /* is miter limit exceeded? */
        ఒకవేళ ( sigma.x < 0x10000L )
        {
          /* don't create variable bevels for very small deviations; */
          /* FT_Sin(x) = 0 for x <= 57                               */
          ఒకవేళ ( fixed_bevel || ft_pos_abs( theta ) > 57 )
            bevel = TRUE;
        }
      }

      ఒకవేళ ( bevel )  /* this is a bevel (broken angle) */
      {
        ఒకవేళ ( fixed_bevel )
        {
          /* the outer corners are simply joined together */
          FT_సదిశ  delta;


          /* add bevel */
          FT_సదిశ_From_Polar( &delta,
                                radius,
                                stroker->angle_బహిర్గతం + rotate );
          delta.x += stroker->center.x;
          delta.y += stroker->center.y;

          border->movable = FALSE;
          error = ft_stroke_border_lineto( border, &delta, FALSE );
        }
        లేకపోతే /* variable bevel or clipped miter */
        {
          /* the miter is truncated */
          FT_సదిశ  మధ్య, delta;
          FT_Fixed   coef;


          /* compute middle point and first angle point */
          FT_సదిశ_From_Polar( &మధ్య,
                                FT_MulFix( radius, stroker->miter_హద్దు ),
                                phi );

          coef    = FT_DivFix(  0x10000L - sigma.x, sigma.y );
          delta.x = FT_MulFix(  మధ్య.y, coef );
          delta.y = FT_MulFix( -మధ్య.x, coef );

          మధ్య.x += stroker->center.x;
          మధ్య.y += stroker->center.y;
          delta.x  += మధ్య.x;
          delta.y  += మధ్య.y;

          error = ft_stroke_border_lineto( border, &delta, FALSE );
          ఒకవేళ ( error )
            వెళ్ళు Exit;

          /* compute second angle point */
          delta.x = మధ్య.x - delta.x + మధ్య.x;
          delta.y = మధ్య.y - delta.y + మధ్య.y;

          error = ft_stroke_border_lineto( border, &delta, FALSE );
          ఒకవేళ ( error )
            వెళ్ళు Exit;

          /* finally, add an end point; only needed if not lineto */
          /* (line_length is zero for curves)                     */
          ఒకవేళ ( పంక్తి_నిడివి == 0 )
          {
            FT_సదిశ_From_Polar( &delta,
                                  radius,
                                  stroker->angle_బహిర్గతం + rotate );

            delta.x += stroker->center.x;
            delta.y += stroker->center.y;

            error = ft_stroke_border_lineto( border, &delta, FALSE );
          }
        }
      }
      లేకపోతే /* this is a miter (intersection) */
      {
        FT_Fixed   నిడివి;
        FT_సదిశ  delta;


        నిడివి = FT_MulDiv( stroker->radius, stroker->miter_హద్దు, sigma.x );

        FT_సదిశ_From_Polar( &delta, నిడివి, phi );
        delta.x += stroker->center.x;
        delta.y += stroker->center.y;

        error = ft_stroke_border_lineto( border, &delta, FALSE );
        ఒకవేళ ( error )
          వెళ్ళు Exit;

        /* now add an end point; only needed if not lineto */
        /* (line_length is zero for curves)                */
        ఒకవేళ ( పంక్తి_నిడివి == 0 )
        {
          FT_సదిశ_From_Polar( &delta,
                                stroker->radius,
                                stroker->angle_బహిర్గతం + rotate );
          delta.x += stroker->center.x;
          delta.y += stroker->center.y;

          error = ft_stroke_border_lineto( border, &delta, FALSE );
        }
      }
    }

  Exit:
    తిరిగి error;
  }


  స్థిర FT_Error
  ft_stroker_process_మూల( FT_Stroker  stroker,
                             FT_Fixed    పంక్తి_నిడివి )
  {
    FT_Error  error = FT_Err_Ok;
    FT_Angle  turn;
    FT_Int    inside_side;


    turn = FT_Angle_Diff( stroker->angle_లోపల, stroker->angle_బహిర్గతం );

    /* no specific corner processing is required if the turn is 0 */
    ఒకవేళ ( turn == 0 )
      వెళ్ళు Exit;

    /* when we turn to the right, the inside side is 0 */
    /* otherwise, the inside side is 1 */
    inside_side = ( turn < 0 );

    /* process the inside side */
    error = ft_stroker_inside( stroker, inside_side, పంక్తి_నిడివి );
    ఒకవేళ ( error )
      వెళ్ళు Exit;

    /* process the outside side */
    error = ft_stroker_outside( stroker, !inside_side, పంక్తి_నిడివి );

  Exit:
    తిరిగి error;
  }


  /* add two points to the left and right borders corresponding to the */
  /* start of the subpath                                              */
  స్థిర FT_Error
  ft_stroker_subpath_ప్రారంభం( FT_Stroker  stroker,
                            FT_Angle    ప్రారంభం_angle,
                            FT_Fixed    పంక్తి_నిడివి )
  {
    FT_సదిశ        delta;
    FT_సదిశ        బిందువు;
    FT_Error         error;
    FT_StrokeBorder  border;


    FT_సదిశ_From_Polar( &delta, stroker->radius,
                          ప్రారంభం_angle + FT_ANGLE_PI2 );

    బిందువు.x = stroker->center.x + delta.x;
    బిందువు.y = stroker->center.y + delta.y;

    border = stroker->borders;
    error = ft_stroke_border_moveto( border, &బిందువు );
    ఒకవేళ ( error )
      వెళ్ళు Exit;

    బిందువు.x = stroker->center.x - delta.x;
    బిందువు.y = stroker->center.y - delta.y;

    border++;
    error = ft_stroke_border_moveto( border, &బిందువు );

    /* save angle, position, and line length for last join */
    /* (line_length is zero for curves)                    */
    stroker->subpath_angle       = ప్రారంభం_angle;
    stroker->మొదటి_బిందువు         = FALSE;
    stroker->subpath_పంక్తి_నిడివి = పంక్తి_నిడివి;

  Exit:
    తిరిగి error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Stroker_పంక్తి_గమ్యం( FT_Stroker  stroker,
                     FT_సదిశ*  గమ్యం )
  {
    FT_Error         error = FT_Err_Ok;
    FT_StrokeBorder  border;
    FT_సదిశ        delta;
    FT_Angle         angle;
    FT_Int           side;
    FT_Fixed         పంక్తి_నిడివి;


    ఒకవేళ ( !stroker || !గమ్యం )
      తిరిగి FT_THROW( Invalid_వాదం );

    delta.x = గమ్యం->x - stroker->center.x;
    delta.y = గమ్యం->y - stroker->center.y;

    /* a zero-length lineto is a no-op; avoid creating a spurious corner */
    ఒకవేళ ( delta.x == 0 && delta.y == 0 )
       వెళ్ళు Exit;

    /* compute length of line */
    పంక్తి_నిడివి = FT_సదిశ_నిడివి( &delta );

    angle = FT_Atan2( delta.x, delta.y );
    FT_సదిశ_From_Polar( &delta, stroker->radius, angle + FT_ANGLE_PI2 );

    /* process corner if necessary */
    ఒకవేళ ( stroker->మొదటి_బిందువు )
    {
      /* This is the first segment of a subpath.  We need to     */
      /* add a point to each border at their respective starting */
      /* point locations.                                        */
      error = ft_stroker_subpath_ప్రారంభం( stroker, angle, పంక్తి_నిడివి );
      ఒకవేళ ( error )
        వెళ్ళు Exit;
    }
    లేకపోతే
    {
      /* process the current corner */
      stroker->angle_బహిర్గతం = angle;
      error = ft_stroker_process_మూల( stroker, పంక్తి_నిడివి );
      ఒకవేళ ( error )
        వెళ్ళు Exit;
    }

    /* now add a line segment to both the `inside' and `outside' paths */
    కొరకు ( border = stroker->borders, side = 1; side >= 0; side--, border++ )
    {
      FT_సదిశ  బిందువు;


      బిందువు.x = గమ్యం->x + delta.x;
      బిందువు.y = గమ్యం->y + delta.y;

      /* the ends of lineto borders are movable */
      error = ft_stroke_border_lineto( border, &బిందువు, TRUE );
      ఒకవేళ ( error )
        వెళ్ళు Exit;

      delta.x = -delta.x;
      delta.y = -delta.y;
    }

    stroker->angle_లోపల    = angle;
    stroker->center      = *గమ్యం;
    stroker->పంక్తి_నిడివి = పంక్తి_నిడివి;

  Exit:
    తిరిగి error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Stroker_Conic_గమ్యం( FT_Stroker  stroker,
                      FT_సదిశ*  control,
                      FT_సదిశ*  గమ్యం )
  {
    FT_Error    error = FT_Err_Ok;
    FT_సదిశ   bez_దొంతర[34];
    FT_సదిశ*  arc;
    FT_సదిశ*  హద్దు = bez_దొంతర + 30;
    FT_Bool     మొదటి_arc = TRUE;


    ఒకవేళ ( !stroker || !control || !గమ్యం )
    {
      error = FT_THROW( Invalid_వాదం );
      వెళ్ళు Exit;
    }

    /* if all control points are coincident, this is a no-op; */
    /* avoid creating a spurious corner                       */
    ఒకవేళ ( FT_IS_SMALL( stroker->center.x - control->x ) &&
         FT_IS_SMALL( stroker->center.y - control->y ) &&
         FT_IS_SMALL( control->x        - గమ్యం->x      ) &&
         FT_IS_SMALL( control->y        - గమ్యం->y      ) )
    {
       stroker->center = *గమ్యం;
       వెళ్ళు Exit;
    }

    arc    = bez_దొంతర;
    arc[0] = *గమ్యం;
    arc[1] = *control;
    arc[2] = stroker->center;

    చేయి
    {
      FT_Angle  angle_లోపల, angle_బహిర్గతం;


      /* initialize with current direction */
      angle_లోపల = angle_బహిర్గతం = stroker->angle_లోపల;

      ఒకవేళ ( arc < హద్దు                                             &&
           !ft_conic_is_small_enough( arc, &angle_లోపల, &angle_బహిర్గతం ) )
      {
        ఒకవేళ ( stroker->మొదటి_బిందువు )
          stroker->angle_లోపల = angle_లోపల;

        ft_conic_విభజించు( arc );
        arc += 2;
        కొనసాగు;
      }

      ఒకవేళ ( మొదటి_arc )
      {
        మొదటి_arc = FALSE;

        /* process corner if necessary */
        ఒకవేళ ( stroker->మొదటి_బిందువు )
          error = ft_stroker_subpath_ప్రారంభం( stroker, angle_లోపల, 0 );
        లేకపోతే
        {
          stroker->angle_బహిర్గతం = angle_లోపల;
          error = ft_stroker_process_మూల( stroker, 0 );
        }
      }
      లేకపోతే ఒకవేళ ( ft_pos_abs( FT_Angle_Diff( stroker->angle_లోపల, angle_లోపల ) ) >
                  FT_SMALL_CONIC_పరిమితి / 4                             )
      {
        /* if the deviation from one arc to the next is too great, */
        /* add a round corner                                      */
        stroker->center    = arc[2];
        stroker->angle_బహిర్గతం = angle_లోపల;
        stroker->పంక్తి_join = FT_STROKER_LINEJOIN_విడత;

        error = ft_stroker_process_మూల( stroker, 0 );

        /* reinstate line join style */
        stroker->పంక్తి_join = stroker->పంక్తి_join_saved;
      }

      ఒకవేళ ( error )
        వెళ్ళు Exit;

      /* the arc's angle is small enough; we can add it directly to each */
      /* border                                                          */
      {
        FT_సదిశ        ctrl, చివర;
        FT_Angle         theta, phi, rotate, alpha0 = 0;
        FT_Fixed         నిడివి;
        FT_StrokeBorder  border;
        FT_Int           side;


        theta  = FT_Angle_Diff( angle_లోపల, angle_బహిర్గతం ) / 2;
        phi    = angle_లోపల + theta;
        నిడివి = FT_DivFix( stroker->radius, FT_Cos( theta ) );

        /* compute direction of original arc */
        ఒకవేళ ( stroker->handle_wide_strokes )
          alpha0 = FT_Atan2( arc[0].x - arc[2].x, arc[0].y - arc[2].y );

        కొరకు ( border = stroker->borders, side = 0;
              side <= 1;
              side++, border++ )
        {
          rotate = FT_SIDE_గమ్యం_ROTATE( side );

          /* compute control point */
          FT_సదిశ_From_Polar( &ctrl, నిడివి, phi + rotate );
          ctrl.x += arc[1].x;
          ctrl.y += arc[1].y;

          /* compute end point */
          FT_సదిశ_From_Polar( &చివర, stroker->radius, angle_బహిర్గతం + rotate );
          చివర.x += arc[0].x;
          చివర.y += arc[0].y;

          ఒకవేళ ( stroker->handle_wide_strokes )
          {
            FT_సదిశ  ప్రారంభం;
            FT_Angle   alpha1;


            /* determine whether the border radius is greater than the */
            /* radius of curvature of the original arc                 */
            ప్రారంభం = border->points[border->అంకె_points - 1];

            alpha1 = FT_Atan2( చివర.x - ప్రారంభం.x, చివర.y - ప్రారంభం.y );

            /* is the direction of the border arc opposite to */
            /* that of the original arc? */
            ఒకవేళ ( ft_pos_abs( FT_Angle_Diff( alpha0, alpha1 ) ) >
                   FT_ANGLE_PI / 2                             )
            {
              FT_Angle   beta, gamma;
              FT_సదిశ  bvec, delta;
              FT_Fixed   blen, sinA, sinB, alen;


              /* use the sine rule to find the intersection point */
              beta  = FT_Atan2( arc[2].x - ప్రారంభం.x, arc[2].y - ప్రారంభం.y );
              gamma = FT_Atan2( arc[0].x - చివర.x,   arc[0].y - చివర.y );

              bvec.x = చివర.x - ప్రారంభం.x;
              bvec.y = చివర.y - ప్రారంభం.y;

              blen = FT_సదిశ_నిడివి( &bvec );

              sinA = ft_pos_abs( FT_Sin( alpha1 - gamma ) );
              sinB = ft_pos_abs( FT_Sin( beta - gamma ) );

              alen = FT_MulDiv( blen, sinA, sinB );

              FT_సదిశ_From_Polar( &delta, alen, beta );
              delta.x += ప్రారంభం.x;
              delta.y += ప్రారంభం.y;

              /* circumnavigate the negative sector backwards */
              border->movable = FALSE;
              error = ft_stroke_border_lineto( border, &delta, FALSE );
              ఒకవేళ ( error )
                వెళ్ళు Exit;
              error = ft_stroke_border_lineto( border, &చివర, FALSE );
              ఒకవేళ ( error )
                వెళ్ళు Exit;
              error = ft_stroke_border_conicto( border, &ctrl, &ప్రారంభం );
              ఒకవేళ ( error )
                వెళ్ళు Exit;
              /* and then move to the endpoint */
              error = ft_stroke_border_lineto( border, &చివర, FALSE );
              ఒకవేళ ( error )
                వెళ్ళు Exit;

              కొనసాగు;
            }

            /* else fall through */
          }

          /* simply add an arc */
          error = ft_stroke_border_conicto( border, &ctrl, &చివర );
          ఒకవేళ ( error )
            వెళ్ళు Exit;
        }
      }

      stroker->angle_లోపల = angle_బహిర్గతం;

      ఒకవేళ ( arc == bez_దొంతర )
        ఆపు;
      arc -= 2;
    } అయితే ( 1 );

    stroker->center      = *గమ్యం;
    stroker->పంక్తి_నిడివి = 0;

  Exit:
    తిరిగి error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Stroker_Cubic_గమ్యం( FT_Stroker  stroker,
                      FT_సదిశ*  control1,
                      FT_సదిశ*  control2,
                      FT_సదిశ*  గమ్యం )
  {
    FT_Error    error = FT_Err_Ok;
    FT_సదిశ   bez_దొంతర[37];
    FT_సదిశ*  arc;
    FT_సదిశ*  హద్దు = bez_దొంతర + 32;
    FT_Bool     మొదటి_arc = TRUE;


    ఒకవేళ ( !stroker || !control1 || !control2 || !గమ్యం )
    {
      error = FT_THROW( Invalid_వాదం );
      వెళ్ళు Exit;
    }

    /* if all control points are coincident, this is a no-op; */
    /* avoid creating a spurious corner */
    ఒకవేళ ( FT_IS_SMALL( stroker->center.x - control1->x ) &&
         FT_IS_SMALL( stroker->center.y - control1->y ) &&
         FT_IS_SMALL( control1->x       - control2->x ) &&
         FT_IS_SMALL( control1->y       - control2->y ) &&
         FT_IS_SMALL( control2->x       - గమ్యం->x       ) &&
         FT_IS_SMALL( control2->y       - గమ్యం->y       ) )
    {
       stroker->center = *గమ్యం;
       వెళ్ళు Exit;
    }

    arc    = bez_దొంతర;
    arc[0] = *గమ్యం;
    arc[1] = *control2;
    arc[2] = *control1;
    arc[3] = stroker->center;

    చేయి
    {
      FT_Angle  angle_లోపల, angle_mid, angle_బహిర్గతం;


      /* initialize with current direction */
      angle_లోపల = angle_బహిర్గతం = angle_mid = stroker->angle_లోపల;

      ఒకవేళ ( arc < హద్దు                                         &&
           !ft_cubic_is_small_enough( arc, &angle_లోపల,
                                      &angle_mid, &angle_బహిర్గతం ) )
      {
        ఒకవేళ ( stroker->మొదటి_బిందువు )
          stroker->angle_లోపల = angle_లోపల;

        ft_cubic_విభజించు( arc );
        arc += 3;
        కొనసాగు;
      }

      ఒకవేళ ( మొదటి_arc )
      {
        మొదటి_arc = FALSE;

        /* process corner if necessary */
        ఒకవేళ ( stroker->మొదటి_బిందువు )
          error = ft_stroker_subpath_ప్రారంభం( stroker, angle_లోపల, 0 );
        లేకపోతే
        {
          stroker->angle_బహిర్గతం = angle_లోపల;
          error = ft_stroker_process_మూల( stroker, 0 );
        }
      }
      లేకపోతే ఒకవేళ ( ft_pos_abs( FT_Angle_Diff( stroker->angle_లోపల, angle_లోపల ) ) >
                  FT_SMALL_CUBIC_పరిమితి / 4                             )
      {
        /* if the deviation from one arc to the next is too great, */
        /* add a round corner                                      */
        stroker->center    = arc[3];
        stroker->angle_బహిర్గతం = angle_లోపల;
        stroker->పంక్తి_join = FT_STROKER_LINEJOIN_విడత;

        error = ft_stroker_process_మూల( stroker, 0 );

        /* reinstate line join style */
        stroker->పంక్తి_join = stroker->పంక్తి_join_saved;
      }

      ఒకవేళ ( error )
        వెళ్ళు Exit;

      /* the arc's angle is small enough; we can add it directly to each */
      /* border                                                          */
      {
        FT_సదిశ        ctrl1, ctrl2, చివర;
        FT_Angle         theta1, phi1, theta2, phi2, rotate, alpha0 = 0;
        FT_Fixed         length1, length2;
        FT_StrokeBorder  border;
        FT_Int           side;


        theta1  = FT_Angle_Diff( angle_లోపల,  angle_mid ) / 2;
        theta2  = FT_Angle_Diff( angle_mid, angle_బహిర్గతం ) / 2;
        phi1    = ft_angle_సగటు( angle_లోపల,  angle_mid );
        phi2    = ft_angle_సగటు( angle_mid, angle_బహిర్గతం );
        length1 = FT_DivFix( stroker->radius, FT_Cos( theta1 ) );
        length2 = FT_DivFix( stroker->radius, FT_Cos( theta2 ) );

        /* compute direction of original arc */
        ఒకవేళ ( stroker->handle_wide_strokes )
          alpha0 = FT_Atan2( arc[0].x - arc[3].x, arc[0].y - arc[3].y );

        కొరకు ( border = stroker->borders, side = 0;
              side <= 1;
              side++, border++ )
        {
          rotate = FT_SIDE_గమ్యం_ROTATE( side );

          /* compute control points */
          FT_సదిశ_From_Polar( &ctrl1, length1, phi1 + rotate );
          ctrl1.x += arc[2].x;
          ctrl1.y += arc[2].y;

          FT_సదిశ_From_Polar( &ctrl2, length2, phi2 + rotate );
          ctrl2.x += arc[1].x;
          ctrl2.y += arc[1].y;

          /* compute end point */
          FT_సదిశ_From_Polar( &చివర, stroker->radius, angle_బహిర్గతం + rotate );
          చివర.x += arc[0].x;
          చివర.y += arc[0].y;

          ఒకవేళ ( stroker->handle_wide_strokes )
          {
            FT_సదిశ  ప్రారంభం;
            FT_Angle   alpha1;


            /* determine whether the border radius is greater than the */
            /* radius of curvature of the original arc                 */
            ప్రారంభం = border->points[border->అంకె_points - 1];

            alpha1 = FT_Atan2( చివర.x - ప్రారంభం.x, చివర.y - ప్రారంభం.y );

            /* is the direction of the border arc opposite to */
            /* that of the original arc? */
            ఒకవేళ ( ft_pos_abs( FT_Angle_Diff( alpha0, alpha1 ) ) >
                   FT_ANGLE_PI / 2                             )
            {
              FT_Angle   beta, gamma;
              FT_సదిశ  bvec, delta;
              FT_Fixed   blen, sinA, sinB, alen;


              /* use the sine rule to find the intersection point */
              beta  = FT_Atan2( arc[3].x - ప్రారంభం.x, arc[3].y - ప్రారంభం.y );
              gamma = FT_Atan2( arc[0].x - చివర.x,   arc[0].y - చివర.y );

              bvec.x = చివర.x - ప్రారంభం.x;
              bvec.y = చివర.y - ప్రారంభం.y;

              blen = FT_సదిశ_నిడివి( &bvec );

              sinA = ft_pos_abs( FT_Sin( alpha1 - gamma ) );
              sinB = ft_pos_abs( FT_Sin( beta - gamma ) );

              alen = FT_MulDiv( blen, sinA, sinB );

              FT_సదిశ_From_Polar( &delta, alen, beta );
              delta.x += ప్రారంభం.x;
              delta.y += ప్రారంభం.y;

              /* circumnavigate the negative sector backwards */
              border->movable = FALSE;
              error = ft_stroke_border_lineto( border, &delta, FALSE );
              ఒకవేళ ( error )
                వెళ్ళు Exit;
              error = ft_stroke_border_lineto( border, &చివర, FALSE );
              ఒకవేళ ( error )
                వెళ్ళు Exit;
              error = ft_stroke_border_cubicto( border,
                                                &ctrl2,
                                                &ctrl1,
                                                &ప్రారంభం );
              ఒకవేళ ( error )
                వెళ్ళు Exit;
              /* and then move to the endpoint */
              error = ft_stroke_border_lineto( border, &చివర, FALSE );
              ఒకవేళ ( error )
                వెళ్ళు Exit;

              కొనసాగు;
            }

            /* else fall through */
          }

          /* simply add an arc */
          error = ft_stroke_border_cubicto( border, &ctrl1, &ctrl2, &చివర );
          ఒకవేళ ( error )
            వెళ్ళు Exit;
        }
      }

      stroker->angle_లోపల = angle_బహిర్గతం;

      ఒకవేళ ( arc == bez_దొంతర )
        ఆపు;
      arc -= 3;
    } అయితే ( 1 );

    stroker->center      = *గమ్యం;
    stroker->పంక్తి_నిడివి = 0;

  Exit:
    తిరిగి error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Stroker_Begin_Sub_మార్గం( FT_Stroker  stroker,
                           FT_సదిశ*  గమ్యం,
                           FT_Bool     open )
  {
    ఒకవేళ ( !stroker || !గమ్యం )
      తిరిగి FT_THROW( Invalid_వాదం );

    /* We cannot process the first point, because there is not enough      */
    /* information regarding its corner/cap.  The latter will be processed */
    /* in the `FT_Stroker_EndSubPath' routine.                             */
    /*                                                                     */
    stroker->మొదటి_బిందువు  = TRUE;
    stroker->center       = *గమ్యం;
    stroker->subpath_open = open;

    /* Determine if we need to check whether the border radius is greater */
    /* than the radius of curvature of a curve, to handle this case       */
    /* specially.  This is only required if bevel joins or butt caps may  */
    /* be created, because round & miter joins and round & square caps    */
    /* cover the negative sector created with wide strokes.               */
    stroker->handle_wide_strokes =
      FT_BOOL( stroker->పంక్తి_join != FT_STROKER_LINEJOIN_విడత  ||
               ( stroker->subpath_open                        &&
                 stroker->పంక్తి_cap == FT_STROKER_LINECAP_BUTT ) );

    /* record the subpath start point for each border */
    stroker->subpath_ప్రారంభం = *గమ్యం;

    stroker->angle_లోపల = 0;

    తిరిగి FT_Err_Ok;
  }


  స్థిర FT_Error
  ft_stroker_కూడు_తిరగేయి_ఎడమ( FT_Stroker  stroker,
                               FT_Bool     open )
  {
    FT_StrokeBorder  కుడి = stroker->borders + 0;
    FT_StrokeBorder  ఎడమ  = stroker->borders + 1;
    FT_Int           new_points;
    FT_Error         error = FT_Err_Ok;


    FT_ASSERT( ఎడమ->ప్రారంభం >= 0 );

    new_points = (FT_Int)ఎడమ->అంకె_points - ఎడమ->ప్రారంభం;
    ఒకవేళ ( new_points > 0 )
    {
      error = ft_stroke_border_grow( కుడి, (FT_UInt)new_points );
      ఒకవేళ ( error )
        వెళ్ళు Exit;

      {
        FT_సదిశ*  dst_బిందువు = కుడి->points + కుడి->అంకె_points;
        FT_Byte*    dst_ట్యాగ్   = కుడి->ట్యాగులు   + కుడి->అంకె_points;
        FT_సదిశ*  src_బిందువు = ఎడమ->points  + ఎడమ->అంకె_points - 1;
        FT_Byte*    src_ట్యాగ్   = ఎడమ->ట్యాగులు    + ఎడమ->అంకె_points - 1;


        అయితే ( src_బిందువు >= ఎడమ->points + ఎడమ->ప్రారంభం )
        {
          *dst_బిందువు = *src_బిందువు;
          *dst_ట్యాగ్   = *src_ట్యాగ్;

          ఒకవేళ ( open )
            dst_ట్యాగ్[0] &= ~FT_STROKE_ట్యాగ్_BEGIN_చివర;
          లేకపోతే
          {
            FT_Byte  ttag =
                       (FT_Byte)( dst_ట్యాగ్[0] & FT_STROKE_ట్యాగ్_BEGIN_చివర );


            /* switch begin/end tags if necessary */
            ఒకవేళ ( ttag == FT_STROKE_ట్యాగ్_BEGIN ||
                 ttag == FT_STROKE_ట్యాగ్_చివర   )
              dst_ట్యాగ్[0] ^= FT_STROKE_ట్యాగ్_BEGIN_చివర;
          }

          src_బిందువు--;
          src_ట్యాగ్--;
          dst_బిందువు++;
          dst_ట్యాగ్++;
        }
      }

      ఎడమ->అంకె_points   = (FT_UInt)ఎడమ->ప్రారంభం;
      కుడి->అంకె_points += (FT_UInt)new_points;

      కుడి->movable = FALSE;
      ఎడమ->movable  = FALSE;
    }

  Exit:
    తిరిగి error;
  }


  /* documentation is in ftstroke.h */

  /* there's a lot of magic in this function! */
  FT_EXPORT_DEF( FT_Error )
  FT_Stroker_చివర_Sub_మార్గం( FT_Stroker  stroker )
  {
    FT_Error  error = FT_Err_Ok;


    ఒకవేళ ( !stroker )
    {
      error = FT_THROW( Invalid_వాదం );
      వెళ్ళు Exit;
    }

    /* don't try to end the path if no segments have been generated */
    ఒకవేళ ( stroker->మొదటి_బిందువు )
      వెళ్ళు Exit;

    ఒకవేళ ( stroker->subpath_open )
    {
      FT_StrokeBorder  కుడి = stroker->borders;


      /* All right, this is an opened path, we need to add a cap between */
      /* right & left, add the reverse of left, then add a final cap     */
      /* between left & right.                                           */
      error = ft_stroker_cap( stroker, stroker->angle_లోపల, 0 );
      ఒకవేళ ( error )
        వెళ్ళు Exit;

      /* add reversed points from `left' to `right' */
      error = ft_stroker_కూడు_తిరగేయి_ఎడమ( stroker, TRUE );
      ఒకవేళ ( error )
        వెళ్ళు Exit;

      /* now add the final cap */
      stroker->center = stroker->subpath_ప్రారంభం;
      error = ft_stroker_cap( stroker,
                              stroker->subpath_angle + FT_ANGLE_PI, 0 );
      ఒకవేళ ( error )
        వెళ్ళు Exit;

      /* Now end the right subpath accordingly.  The left one is */
      /* rewind and doesn't need further processing.             */
      ft_stroke_border_close( కుడి, FALSE );
    }
    లేకపోతే
    {
      /* close the path if needed */
      ఒకవేళ ( !FT_IS_SMALL( stroker->center.x - stroker->subpath_ప్రారంభం.x ) ||
           !FT_IS_SMALL( stroker->center.y - stroker->subpath_ప్రారంభం.y ) )
      {
         error = FT_Stroker_పంక్తి_గమ్యం( stroker, &stroker->subpath_ప్రారంభం );
         ఒకవేళ ( error )
           వెళ్ళు Exit;
      }

      /* process the corner */
      stroker->angle_బహిర్గతం = stroker->subpath_angle;

      error = ft_stroker_process_మూల( stroker,
                                         stroker->subpath_పంక్తి_నిడివి );
      ఒకవేళ ( error )
        వెళ్ళు Exit;

      /* then end our two subpaths */
      ft_stroke_border_close( stroker->borders + 0, FALSE );
      ft_stroke_border_close( stroker->borders + 1, TRUE );
    }

  Exit:
    తిరిగి error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Stroker_GetBorderCounts( FT_Stroker        stroker,
                              FT_StrokerBorder  border,
                              FT_UInt          *anum_points,
                              FT_UInt          *anum_contours )
  {
    FT_UInt   అంకె_points = 0, అంకె_contours = 0;
    FT_Error  error;


    ఒకవేళ ( !stroker || border > 1 )
    {
      error = FT_THROW( Invalid_వాదం );
      వెళ్ళు Exit;
    }

    error = ft_stroke_border_get_counts( stroker->borders + border,
                                         &అంకె_points, &అంకె_contours );
  Exit:
    ఒకవేళ ( anum_points )
      *anum_points = అంకె_points;

    ఒకవేళ ( anum_contours )
      *anum_contours = అంకె_contours;

    తిరిగి error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Stroker_GetCounts( FT_Stroker  stroker,
                        FT_UInt    *anum_points,
                        FT_UInt    *anum_contours )
  {
    FT_UInt   count1, count2, అంకె_points   = 0;
    FT_UInt   count3, count4, అంకె_contours = 0;
    FT_Error  error;


    ఒకవేళ ( !stroker )
    {
      error = FT_THROW( Invalid_వాదం );
      వెళ్ళు Exit;
    }

    error = ft_stroke_border_get_counts( stroker->borders + 0,
                                         &count1, &count2 );
    ఒకవేళ ( error )
      వెళ్ళు Exit;

    error = ft_stroke_border_get_counts( stroker->borders + 1,
                                         &count3, &count4 );
    ఒకవేళ ( error )
      వెళ్ళు Exit;

    అంకె_points   = count1 + count3;
    అంకె_contours = count2 + count4;

  Exit:
    ఒకవేళ ( anum_points )
      *anum_points   = అంకె_points;

    ఒకవేళ ( anum_contours )
      *anum_contours = అంకె_contours;

    తిరిగి error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( శూన్య )
  FT_Stroker_ExportBorder( FT_Stroker        stroker,
                           FT_StrokerBorder  border,
                           FT_Outline*       outline )
  {
    ఒకవేళ ( !stroker || !outline )
      తిరిగి;

    ఒకవేళ ( border == FT_STROKER_BORDER_ఎడమ  ||
         border == FT_STROKER_BORDER_కుడి )
    {
      FT_StrokeBorder  sborder = & stroker->borders[border];


      ఒకవేళ ( sborder->valid )
        ft_stroke_border_export( sborder, outline );
    }
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( శూన్య )
  FT_Stroker_Export( FT_Stroker   stroker,
                     FT_Outline*  outline )
  {
    FT_Stroker_ExportBorder( stroker, FT_STROKER_BORDER_ఎడమ, outline );
    FT_Stroker_ExportBorder( stroker, FT_STROKER_BORDER_కుడి, outline );
  }


  /* documentation is in ftstroke.h */

  /*
   * The following is very similar to FT_Outline_Decompose, except
   * that we do support opened paths, and do not scale the outline.
   */
  FT_EXPORT_DEF( FT_Error )
  FT_Stroker_ParseOutline( FT_Stroker   stroker,
                           FT_Outline*  outline,
                           FT_Bool      opened )
  {
    FT_సదిశ   v_చివరి;
    FT_సదిశ   v_control;
    FT_సదిశ   v_ప్రారంభం;

    FT_సదిశ*  బిందువు;
    FT_సదిశ*  హద్దు;
    FT_Byte*    ట్యాగులు;

    FT_Error    error;

    FT_Int      n;         /* index of contour in outline     */
    FT_Int      మొదటి;     /* index of first point in contour */
    FT_Int      చివరి;      /* index of last point in contour  */

    FT_Int      ట్యాగ్;       /* current point's state           */


    ఒకవేళ ( !outline )
      తిరిగి FT_THROW( Invalid_Outline );

    ఒకవేళ ( !stroker )
      తిరిగి FT_THROW( Invalid_వాదం );

    FT_Stroker_Rewind( stroker );

    చివరి = -1;
    కొరకు ( n = 0; n < outline->n_contours; n++ )
    {
      మొదటి = చివరి + 1;
      చివరి  = outline->contours[n];

      /* skip empty points; we don't stroke these */
      ఒకవేళ ( చివరి <= మొదటి )
        కొనసాగు;

      హద్దు = outline->points + చివరి;

      v_ప్రారంభం = outline->points[మొదటి];
      v_చివరి  = outline->points[చివరి];

      v_control = v_ప్రారంభం;

      బిందువు = outline->points + మొదటి;
      ట్యాగులు  = outline->ట్యాగులు   + మొదటి;
      ట్యాగ్   = FT_వక్రం_ట్యాగ్( ట్యాగులు[0] );

      /* check first point to determine origin */
      ఒకవేళ ( ట్యాగ్ != FT_వక్రం_ట్యాగ్_ON )
      {
        FT_Int  చివరి_ట్యాగ్ = FT_వక్రం_ట్యాగ్( outline->ట్యాగులు[చివరి] );


        /* First point is a control point.  Yes, this happens. */
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

          /* if both first and last points are controls, */
          /* start at their middle                       */
          v_ప్రారంభం.x = ( v_ప్రారంభం.x + v_చివరి.x ) / 2;
          v_ప్రారంభం.y = ( v_ప్రారంభం.y + v_చివరి.y ) / 2;
        }
        బిందువు--;
        ట్యాగులు--;
      }

      error = FT_Stroker_Begin_Sub_మార్గం( stroker, &v_ప్రారంభం, opened );
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


            vec.x = బిందువు->x;
            vec.y = బిందువు->y;

            error = FT_Stroker_పంక్తి_గమ్యం( stroker, &vec );
            ఒకవేళ ( error )
              వెళ్ళు Exit;
            కొనసాగు;
          }

        స్థితి FT_వక్రం_ట్యాగ్_CONIC:  /* consume conic arcs */
          v_control.x = బిందువు->x;
          v_control.y = బిందువు->y;

        Do_Conic:
          ఒకవేళ ( బిందువు < హద్దు )
          {
            FT_సదిశ  vec;
            FT_సదిశ  v_మధ్య;


            బిందువు++;
            ట్యాగులు++;
            ట్యాగ్ = FT_వక్రం_ట్యాగ్( ట్యాగులు[0] );

            vec = బిందువు[0];

            ఒకవేళ ( ట్యాగ్ == FT_వక్రం_ట్యాగ్_ON )
            {
              error = FT_Stroker_Conic_గమ్యం( stroker, &v_control, &vec );
              ఒకవేళ ( error )
                వెళ్ళు Exit;
              కొనసాగు;
            }

            ఒకవేళ ( ట్యాగ్ != FT_వక్రం_ట్యాగ్_CONIC )
              వెళ్ళు Invalid_Outline;

            v_మధ్య.x = ( v_control.x + vec.x ) / 2;
            v_మధ్య.y = ( v_control.y + vec.y ) / 2;

            error = FT_Stroker_Conic_గమ్యం( stroker, &v_control, &v_మధ్య );
            ఒకవేళ ( error )
              వెళ్ళు Exit;

            v_control = vec;
            వెళ్ళు Do_Conic;
          }

          error = FT_Stroker_Conic_గమ్యం( stroker, &v_control, &v_ప్రారంభం );
          వెళ్ళు Close;

        అప్రమేయ:  /* FT_CURVE_TAG_CUBIC */
          {
            FT_సదిశ  vec1, vec2;


            ఒకవేళ ( బిందువు + 1 > హద్దు                             ||
                 FT_వక్రం_ట్యాగ్( ట్యాగులు[1] ) != FT_వక్రం_ట్యాగ్_CUBIC )
              వెళ్ళు Invalid_Outline;

            బిందువు += 2;
            ట్యాగులు  += 2;

            vec1 = బిందువు[-2];
            vec2 = బిందువు[-1];

            ఒకవేళ ( బిందువు <= హద్దు )
            {
              FT_సదిశ  vec;
              FT_సదిశ  v_మధ్య;


              ట్యాగ్ = FT_వక్రం_ట్యాగ్( ట్యాగులు[0] );
              vec = బిందువు[0];

              ఒకవేళ ( ట్యాగ్ == FT_వక్రం_ట్యాగ్_CUBIC )
              {
                v_మధ్య.x = ( vec2.x + vec.x ) / 2;
                v_మధ్య.y = ( vec2.y + vec.y ) / 2;

                vec = v_మధ్య;
              }
              లేకపోతే ఒకవేళ ( ట్యాగ్ != FT_వక్రం_ట్యాగ్_ON )
                వెళ్ళు Invalid_Outline;

              error = FT_Stroker_Cubic_గమ్యం( stroker, &vec1, &vec2, &vec );
              ఒకవేళ ( error )
                వెళ్ళు Exit;

              ఒకవేళ ( ట్యాగ్ == FT_వక్రం_ట్యాగ్_CUBIC )
              {
                బిందువు--;
                ట్యాగులు--;
              }
              కొనసాగు;
            }

            error = FT_Stroker_Cubic_గమ్యం( stroker, &vec1, &vec2, &v_ప్రారంభం );
            వెళ్ళు Close;
          }
        }
      }

    Close:
      ఒకవేళ ( error )
        వెళ్ళు Exit;

      error = FT_Stroker_చివర_Sub_మార్గం( stroker );
      ఒకవేళ ( error )
        వెళ్ళు Exit;
    }

    తిరిగి FT_Err_Ok;

  Exit:
    తిరిగి error;

  Invalid_Outline:
    తిరిగి FT_THROW( Invalid_Outline );
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Glyph_Stroke( FT_Glyph    *pglyph,
                   FT_Stroker   stroker,
                   FT_Bool      destroy )
  {
    FT_Error  error = FT_THROW( Invalid_వాదం );
    FT_Glyph  glyph = ఖాళీ;


    ఒకవేళ ( !pglyph )
      వెళ్ళు Exit;

    glyph = *pglyph;
    ఒకవేళ ( !glyph || glyph->clazz != &ft_outline_glyph_class )
      వెళ్ళు Exit;

    {
      FT_Glyph  copy;


      error = FT_Glyph_Copy( glyph, &copy );
      ఒకవేళ ( error )
        వెళ్ళు Exit;

      glyph = copy;
    }

    {
      FT_OutlineGlyph  oglyph  = (FT_OutlineGlyph)glyph;
      FT_Outline*      outline = &oglyph->outline;
      FT_UInt          అంకె_points, అంకె_contours;


      error = FT_Stroker_ParseOutline( stroker, outline, FALSE );
      ఒకవేళ ( error )
        వెళ్ళు Fail;

      error = FT_Stroker_GetCounts( stroker, &అంకె_points, &అంకె_contours );
      ఒకవేళ ( error )
        వెళ్ళు Fail;

      FT_Outline_పూర్తి( glyph->library, outline );

      error = FT_Outline_New( glyph->library,
                              అంకె_points,
                              (FT_Int)అంకె_contours,
                              outline );
      ఒకవేళ ( error )
        వెళ్ళు Fail;

      outline->n_points   = 0;
      outline->n_contours = 0;

      FT_Stroker_Export( stroker, outline );
    }

    ఒకవేళ ( destroy )
      FT_పూర్తి_Glyph( *pglyph );

    *pglyph = glyph;
    వెళ్ళు Exit;

  Fail:
    FT_పూర్తి_Glyph( glyph );
    glyph = ఖాళీ;

    ఒకవేళ ( !destroy )
      *pglyph = ఖాళీ;

  Exit:
    తిరిగి error;
  }


  /* documentation is in ftstroke.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Glyph_StrokeBorder( FT_Glyph    *pglyph,
                         FT_Stroker   stroker,
                         FT_Bool      inside,
                         FT_Bool      destroy )
  {
    FT_Error  error = FT_THROW( Invalid_వాదం );
    FT_Glyph  glyph = ఖాళీ;


    ఒకవేళ ( !pglyph )
      వెళ్ళు Exit;

    glyph = *pglyph;
    ఒకవేళ ( !glyph || glyph->clazz != &ft_outline_glyph_class )
      వెళ్ళు Exit;

    {
      FT_Glyph  copy;


      error = FT_Glyph_Copy( glyph, &copy );
      ఒకవేళ ( error )
        వెళ్ళు Exit;

      glyph = copy;
    }

    {
      FT_OutlineGlyph   oglyph  = (FT_OutlineGlyph)glyph;
      FT_StrokerBorder  border;
      FT_Outline*       outline = &oglyph->outline;
      FT_UInt           అంకె_points, అంకె_contours;


      border = FT_Outline_GetOutsideBorder( outline );
      ఒకవేళ ( inside )
      {
        ఒకవేళ ( border == FT_STROKER_BORDER_ఎడమ )
          border = FT_STROKER_BORDER_కుడి;
        లేకపోతే
          border = FT_STROKER_BORDER_ఎడమ;
      }

      error = FT_Stroker_ParseOutline( stroker, outline, FALSE );
      ఒకవేళ ( error )
        వెళ్ళు Fail;

      FT_Stroker_GetBorderCounts( stroker, border,
                                  &అంకె_points, &అంకె_contours );

      FT_Outline_పూర్తి( glyph->library, outline );

      error = FT_Outline_New( glyph->library,
                              అంకె_points,
                              (FT_Int)అంకె_contours,
                              outline );
      ఒకవేళ ( error )
        వెళ్ళు Fail;

      outline->n_points   = 0;
      outline->n_contours = 0;

      FT_Stroker_ExportBorder( stroker, border, outline );
    }

    ఒకవేళ ( destroy )
      FT_పూర్తి_Glyph( *pglyph );

    *pglyph = glyph;
    వెళ్ళు Exit;

  Fail:
    FT_పూర్తి_Glyph( glyph );
    glyph = ఖాళీ;

    ఒకవేళ ( !destroy )
      *pglyph = ఖాళీ;

  Exit:
    తిరిగి error;
  }


/*చివర*/
