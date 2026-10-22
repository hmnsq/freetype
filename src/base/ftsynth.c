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


#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/ftsynth.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftdebug.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftobjs.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/ftoutln.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/ftbitmap.h>


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ  FT_COMPONENT
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FT_COMPONENT  synth


  /*************************************************************************/
  /*************************************************************************/
  /****                                                                 ****/
  /****   EXPERIMENTAL OBLIQUING SUPPORT                                ****/
  /****                                                                 ****/
  /*************************************************************************/
  /*************************************************************************/

  /* documentation is in ftsynth.h */

  FT_EXPORT_DEF( ਖਾਲੀ )
  FT_Glyph_ਥਾਂ_Oblique( FT_Glyph_ਥਾਂ  ਥਾਂ )
  {
    /* Value '0x0366A' corresponds to a shear angle of about 12 degrees. */
    FT_Glyph_ਥਾਂ_Slant( ਥਾਂ, 0x0366A, 0 );
  }


  /* documentation is in ftsynth.h */

  FT_EXPORT_DEF( ਖਾਲੀ )
  FT_Glyph_ਥਾਂ_Slant( FT_Glyph_ਥਾਂ  ਥਾਂ,
                      FT_Fixed      xslant,
                      FT_Fixed      yslant )
  {
    FT_ਮੈਟ੍ਰਿਕਸ    transform;
    FT_Outline*  outline;


    ਜੇ ( !ਥਾਂ )
      ਵਾਪਸ;

    outline = &ਥਾਂ->outline;

    /* only oblique outline glyphs */
    ਜੇ ( ਥਾਂ->format != FT_GLYPH_FORMAT_OUTLINE )
      ਵਾਪਸ;

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

  FT_EXPORT_DEF( ਖਾਲੀ )
  FT_Glyph_ਥਾਂ_Embolden( FT_Glyph_ਥਾਂ  ਥਾਂ )
  {
    FT_Glyph_ਥਾਂ_Adjust_ਭਾਰ( ਥਾਂ, 0x0AAA, 0x0AAA );
  }


  FT_EXPORT_DEF( ਖਾਲੀ )
  FT_Glyph_ਥਾਂ_Adjust_ਭਾਰ( FT_Glyph_ਥਾਂ  ਥਾਂ,
                             FT_Fixed      xdelta,
                             FT_Fixed      ydelta )
  {
    FT_Library  library;
    FT_ਮਾਪ     ਮਾਪ;
    FT_Error    error;
    FT_Pos      xstr, ystr;


    ਜੇ ( !ਥਾਂ )
      ਵਾਪਸ;

    library = ਥਾਂ->library;
    ਮਾਪ    = ਥਾਂ->face->ਮਾਪ;

    ਜੇ ( ਥਾਂ->format != FT_GLYPH_FORMAT_OUTLINE &&
         ਥਾਂ->format != FT_GLYPH_FORMAT_BITMAP  )
      ਵਾਪਸ;

    /* express deltas in pixels in 26.6 format */
    xstr = (FT_Pos)ਮਾਪ->metrics.x_ppem * xdelta / 1024;
    ystr = (FT_Pos)ਮਾਪ->metrics.y_ppem * ydelta / 1024;

    ਜੇ ( ਥਾਂ->format == FT_GLYPH_FORMAT_OUTLINE )
      FT_Outline_EmboldenXY( &ਥਾਂ->outline, xstr, ystr );

    ਨਹੀਂਤਾਂ /* slot->format == FT_GLYPH_FORMAT_BITMAP */
    {
      /* round to full pixels */
      xstr &= ~63;
      ਜੇ ( xstr == 0 )
        xstr = 1 << 6;
      ystr &= ~63;

      /*
       * XXX: overflow check for 16-bit system, for compatibility
       *      with FT_GlyphSlot_Embolden() since FreeType 2.1.10.
       *      unfortunately, this function returns no information
       *      about the cause of error.
       */
      ਜੇ ( ( ystr >> 6 ) > FT_INT_MAX || ( ystr >> 6 ) < FT_INT_MIN )
      {
        FT_TRACE1(( "FT_GlyphSlot_Embolden:" ));
        FT_TRACE1(( "too strong emboldening parameter ystr=%ld\n", ystr ));
        ਵਾਪਸ;
      }
      error = FT_Glyph_ਥਾਂ_Own_Bitmap( ਥਾਂ );
      ਜੇ ( error )
        ਵਾਪਸ;

      error = FT_Bitmap_Embolden( library, &ਥਾਂ->bitmap, xstr, ystr );
      ਜੇ ( error )
        ਵਾਪਸ;
    }

    ਜੇ ( ਥਾਂ->advance.x )
      ਥਾਂ->advance.x += xstr;

    ਜੇ ( ਥਾਂ->advance.y )
      ਥਾਂ->advance.y += ystr;

    ਥਾਂ->metrics.ਚੌੜਾਈ        += xstr;
    ਥਾਂ->metrics.ਉਚਾਈ       += ystr;
    ਥਾਂ->metrics.horiAdvance  += xstr;
    ਥਾਂ->metrics.vertAdvance  += ystr;
    ਥਾਂ->metrics.horiBearingY += ystr;

    /* XXX: 16-bit overflow case must be excluded before here */
    ਜੇ ( ਥਾਂ->format == FT_GLYPH_FORMAT_BITMAP )
      ਥਾਂ->bitmap_ਸਿਖਰ += (FT_Int)( ystr >> 6 );
  }


/*ਸਿਰਾ*/
