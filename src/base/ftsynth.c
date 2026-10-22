/****************************************************************************
 *
 * ftsynth.c
 *
 *   FreeType synthesizing code for emboldening and slanting (body).
 *
 * Copyright (C) 2000-2026 by
 * David Turner, Robert Wilhelm, and Werner Lemberg.
 *
 * This file is part of the FreeType project, and may only be used,
 * modified, and distributed under the terms of the FreeType project
 * license, LICENSE.TXT.  By continuing to use, modify, or distribute
 * this file you indicate that you have read the license and
 * understand and accept it fully.
 *
 */


#అన్తర్భావ <freetype/ftsynth.h>
#అన్తర్భావ <freetype/internal/ftdebug.h>
#అన్తర్భావ <freetype/internal/ftobjs.h>
#అన్తర్భావ <freetype/ftoutln.h>
#అన్తర్భావ <freetype/ftbitmap.h>


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#నిర్వచనం_రద్దు  FT_COMPONENT
#నిర్వచించు FT_COMPONENT  synth


  /*************************************************************************/
  /*************************************************************************/
  /****                                                                 ****/
  /****   EXPERIMENTAL OBLIQUING SUPPORT                                ****/
  /****                                                                 ****/
  /*************************************************************************/
  /*************************************************************************/

  /* documentation is in ftsynth.h */

  FT_EXPORT_DEF( శూన్య )
  FT_Glyph_స్థానం_Oblique( FT_Glyph_స్థానం  స్థానం )
  {
    /* Value '0x0366A' corresponds to a shear angle of about 12 degrees. */
    FT_Glyph_స్థానం_Slant( స్థానం, 0x0366A, 0 );
  }


  /* documentation is in ftsynth.h */

  FT_EXPORT_DEF( శూన్య )
  FT_Glyph_స్థానం_Slant( FT_Glyph_స్థానం  స్థానం,
                      FT_Fixed      xslant,
                      FT_Fixed      yslant )
  {
    FT_మాతృక    transform;
    FT_Outline*  outline;


    ఒకవేళ ( !స్థానం )
      తిరిగి;

    outline = &స్థానం->outline;

    /* only oblique outline glyphs */
    ఒకవేళ ( స్థానం->format != FT_GLYPH_FORMAT_OUTLINE )
      తిరిగి;

    /* we don't touch the advance width */

    /* For italic, simply apply a shear transform */
    transform.xx = 0x10000L;
    transform.yx = -yslant;

    transform.xy = xslant;
    transform.yy = 0x10000L;

    FT_Outline_Transform( outline, &transform );
  }


  /*************************************************************************/
  /*************************************************************************/
  /****                                                                 ****/
  /****   EXPERIMENTAL EMBOLDENING SUPPORT                              ****/
  /****                                                                 ****/
  /*************************************************************************/
  /*************************************************************************/


  /* documentation is in ftsynth.h */

  FT_EXPORT_DEF( శూన్య )
  FT_Glyph_స్థానం_Embolden( FT_Glyph_స్థానం  స్థానం )
  {
    FT_Glyph_స్థానం_Adjust_బరువు( స్థానం, 0x0AAA, 0x0AAA );
  }


  FT_EXPORT_DEF( శూన్య )
  FT_Glyph_స్థానం_Adjust_బరువు( FT_Glyph_స్థానం  స్థానం,
                             FT_Fixed      xdelta,
                             FT_Fixed      ydelta )
  {
    FT_Library  library;
    FT_కొలత     కొలత;
    FT_Error    error;
    FT_Pos      xstr, ystr;


    ఒకవేళ ( !స్థానం )
      తిరిగి;

    library = స్థానం->library;
    కొలత    = స్థానం->face->కొలత;

    ఒకవేళ ( స్థానం->format != FT_GLYPH_FORMAT_OUTLINE &&
         స్థానం->format != FT_GLYPH_FORMAT_BITMAP  )
      తిరిగి;

    /* express deltas in pixels in 26.6 format */
    xstr = (FT_Pos)కొలత->metrics.x_ppem * xdelta / 1024;
    ystr = (FT_Pos)కొలత->metrics.y_ppem * ydelta / 1024;

    ఒకవేళ ( స్థానం->format == FT_GLYPH_FORMAT_OUTLINE )
      FT_Outline_EmboldenXY( &స్థానం->outline, xstr, ystr );

    లేకపోతే /* slot->format == FT_GLYPH_FORMAT_BITMAP */
    {
      /* round to full pixels */
      xstr &= ~63;
      ఒకవేళ ( xstr == 0 )
        xstr = 1 << 6;
      ystr &= ~63;

      /*
       * XXX: overflow check for 16-bit system, for compatibility
       *      with FT_GlyphSlot_Embolden() since FreeType 2.1.10.
       *      unfortunately, this function returns no information
       *      about the cause of error.
       */
      ఒకవేళ ( ( ystr >> 6 ) > FT_INT_MAX || ( ystr >> 6 ) < FT_INT_MIN )
      {
        FT_TRACE1(( "FT_GlyphSlot_Embolden:" ));
        FT_TRACE1(( "too strong emboldening parameter ystr=%ld\n", ystr ));
        తిరిగి;
      }
      error = FT_Glyph_స్థానం_Own_Bitmap( స్థానం );
      ఒకవేళ ( error )
        తిరిగి;

      error = FT_Bitmap_Embolden( library, &స్థానం->bitmap, xstr, ystr );
      ఒకవేళ ( error )
        తిరిగి;
    }

    ఒకవేళ ( స్థానం->advance.x )
      స్థానం->advance.x += xstr;

    ఒకవేళ ( స్థానం->advance.y )
      స్థానం->advance.y += ystr;

    స్థానం->metrics.వెడల్పు        += xstr;
    స్థానం->metrics.ఎత్తు       += ystr;
    స్థానం->metrics.horiAdvance  += xstr;
    స్థానం->metrics.vertAdvance  += ystr;
    స్థానం->metrics.horiBearingY += ystr;

    /* XXX: 16-bit overflow case must be excluded before here */
    ఒకవేళ ( స్థానం->format == FT_GLYPH_FORMAT_BITMAP )
      స్థానం->bitmap_శిఖరం += (FT_Int)( ystr >> 6 );
  }


/*చివర*/
