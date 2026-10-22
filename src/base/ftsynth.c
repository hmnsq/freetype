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
#وصف_رد  FT_COMPONENT
#وصف_ڪريو FT_COMPONENT  synth


  /*************************************************************************/
  /*************************************************************************/
  /****                                                                 ****/
  /****   EXPERIMENTAL OBLIQUING SUPPORT                                ****/
  /****                                                                 ****/
  /*************************************************************************/
  /*************************************************************************/

  /* documentation is in ftsynth.h */

  FT_EXPORT_DEF( فراغ )
  FT_Glyph_جڳهه_Oblique( FT_Glyph_جڳهه  جڳهه )
  {
    /* Value '0x0366A' corresponds to a shear angle of about 12 degrees. */
    FT_Glyph_جڳهه_Slant( جڳهه, 0x0366A, 0 );
  }


  /* documentation is in ftsynth.h */

  FT_EXPORT_DEF( فراغ )
  FT_Glyph_جڳهه_Slant( FT_Glyph_جڳهه  جڳهه,
                      FT_Fixed      xslant,
                      FT_Fixed      yslant )
  {
    FT_ميٽرڪس    transform;
    FT_Outline*  outline;


    إذا ( !جڳهه )
      أرجع;

    outline = &جڳهه->outline;

    /* only oblique outline glyphs */
    إذا ( جڳهه->format != FT_GLYPH_FORMAT_OUTLINE )
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
  FT_Glyph_جڳهه_Embolden( FT_Glyph_جڳهه  جڳهه )
  {
    FT_Glyph_جڳهه_Adjust_وزن( جڳهه, 0x0AAA, 0x0AAA );
  }


  FT_EXPORT_DEF( فراغ )
  FT_Glyph_جڳهه_Adjust_وزن( FT_Glyph_جڳهه  جڳهه,
                             FT_Fixed      xdelta,
                             FT_Fixed      ydelta )
  {
    FT_Library  library;
    FT_سائيز     سائيز;
    FT_Error    error;
    FT_Pos      xstr, ystr;


    إذا ( !جڳهه )
      أرجع;

    library = جڳهه->library;
    سائيز    = جڳهه->face->سائيز;

    إذا ( جڳهه->format != FT_GLYPH_FORMAT_OUTLINE &&
         جڳهه->format != FT_GLYPH_FORMAT_BITMAP  )
      أرجع;

    /* express deltas in pixels in 26.6 format */
    xstr = (FT_Pos)سائيز->metrics.x_ppem * xdelta / 1024;
    ystr = (FT_Pos)سائيز->metrics.y_ppem * ydelta / 1024;

    إذا ( جڳهه->format == FT_GLYPH_FORMAT_OUTLINE )
      FT_Outline_EmboldenXY( &جڳهه->outline, xstr, ystr );

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
      error = FT_Glyph_جڳهه_Own_Bitmap( جڳهه );
      إذا ( error )
        أرجع;

      error = FT_Bitmap_Embolden( library, &جڳهه->bitmap, xstr, ystr );
      إذا ( error )
        أرجع;
    }

    إذا ( جڳهه->advance.x )
      جڳهه->advance.x += xstr;

    إذا ( جڳهه->advance.y )
      جڳهه->advance.y += ystr;

    جڳهه->metrics.ويڪر        += xstr;
    جڳهه->metrics.اوچائي       += ystr;
    جڳهه->metrics.horiAdvance  += xstr;
    جڳهه->metrics.vertAdvance  += ystr;
    جڳهه->metrics.horiBearingY += ystr;

    /* XXX: 16-bit overflow case must be excluded before here */
    إذا ( جڳهه->format == FT_GLYPH_FORMAT_BITMAP )
      جڳهه->bitmap_چوٽي += (FT_Int)( ystr >> 6 );
  }


/*پڇاڙي*/
