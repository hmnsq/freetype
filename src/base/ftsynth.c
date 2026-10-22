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


#ضمن <freetype/ftsynth.h>
#ضمن <freetype/internal/ftdebug.h>
#ضمن <freetype/internal/ftobjs.h>
#ضمن <freetype/ftoutln.h>
#ضمن <freetype/ftbitmap.h>


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#تعریف_رد  FT_COMPONENT
#تعریف_کرو FT_COMPONENT  synth


  /*************************************************************************/
  /*************************************************************************/
  /****                                                                 ****/
  /****   EXPERIMENTAL OBLIQUING SUPPORT                                ****/
  /****                                                                 ****/
  /*************************************************************************/
  /*************************************************************************/

  /* documentation is in ftsynth.h */

  FT_EXPORT_DEF( فراغ )
  FT_Glyph_تھاں_Oblique( FT_Glyph_تھاں  تھاں )
  {
    /* Value '0x0366A' corresponds to a shear angle of about 12 degrees. */
    FT_Glyph_تھاں_Slant( تھاں, 0x0366A, 0 );
  }


  /* documentation is in ftsynth.h */

  FT_EXPORT_DEF( فراغ )
  FT_Glyph_تھاں_Slant( FT_Glyph_تھاں  تھاں,
                      FT_Fixed      xslant,
                      FT_Fixed      yslant )
  {
    FT_میٹرکس    transform;
    FT_Outline*  outline;


    إذا ( !تھاں )
      أرجع;

    outline = &تھاں->outline;

    /* only oblique outline glyphs */
    إذا ( تھاں->format != FT_GLYPH_FORMAT_OUTLINE )
      أرجع;

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

  FT_EXPORT_DEF( فراغ )
  FT_Glyph_تھاں_Embolden( FT_Glyph_تھاں  تھاں )
  {
    FT_Glyph_تھاں_Adjust_بھار( تھاں, 0x0AAA, 0x0AAA );
  }


  FT_EXPORT_DEF( فراغ )
  FT_Glyph_تھاں_Adjust_بھار( FT_Glyph_تھاں  تھاں,
                             FT_Fixed      xdelta,
                             FT_Fixed      ydelta )
  {
    FT_Library  library;
    FT_ناپ     ناپ;
    FT_Error    error;
    FT_Pos      xstr, ystr;


    إذا ( !تھاں )
      أرجع;

    library = تھاں->library;
    ناپ    = تھاں->face->ناپ;

    إذا ( تھاں->format != FT_GLYPH_FORMAT_OUTLINE &&
         تھاں->format != FT_GLYPH_FORMAT_BITMAP  )
      أرجع;

    /* express deltas in pixels in 26.6 format */
    xstr = (FT_Pos)ناپ->metrics.x_ppem * xdelta / 1024;
    ystr = (FT_Pos)ناپ->metrics.y_ppem * ydelta / 1024;

    إذا ( تھاں->format == FT_GLYPH_FORMAT_OUTLINE )
      FT_Outline_EmboldenXY( &تھاں->outline, xstr, ystr );

    وإلا /* slot->format == FT_GLYPH_FORMAT_BITMAP */
    {
      /* round to full pixels */
      xstr &= ~63;
      إذا ( xstr == 0 )
        xstr = 1 << 6;
      ystr &= ~63;

      /*
       * XXX: overflow check for 16-bit system, for compatibility
       *      with FT_GlyphSlot_Embolden() since FreeType 2.1.10.
       *      unfortunately, this function returns no information
       *      about the cause of error.
       */
      إذا ( ( ystr >> 6 ) > FT_INT_MAX || ( ystr >> 6 ) < FT_INT_MIN )
      {
        FT_TRACE1(( "FT_GlyphSlot_Embolden:" ));
        FT_TRACE1(( "too strong emboldening parameter ystr=%ld\n", ystr ));
        أرجع;
      }
      error = FT_Glyph_تھاں_Own_Bitmap( تھاں );
      إذا ( error )
        أرجع;

      error = FT_Bitmap_Embolden( library, &تھاں->bitmap, xstr, ystr );
      إذا ( error )
        أرجع;
    }

    إذا ( تھاں->advance.x )
      تھاں->advance.x += xstr;

    إذا ( تھاں->advance.y )
      تھاں->advance.y += ystr;

    تھاں->metrics.چوڑائی        += xstr;
    تھاں->metrics.اچائی       += ystr;
    تھاں->metrics.horiAdvance  += xstr;
    تھاں->metrics.vertAdvance  += ystr;
    تھاں->metrics.horiBearingY += ystr;

    /* XXX: 16-bit overflow case must be excluded before here */
    إذا ( تھاں->format == FT_GLYPH_FORMAT_BITMAP )
      تھاں->bitmap_چوٹی += (FT_Int)( ystr >> 6 );
  }


/*سرا*/
