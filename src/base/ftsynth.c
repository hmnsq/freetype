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
#لغو_تعریف  FT_COMPONENT
#تعریف_کن FT_COMPONENT  synth


  /*************************************************************************/
  /*************************************************************************/
  /****                                                                 ****/
  /****   EXPERIMENTAL OBLIQUING SUPPORT                                ****/
  /****                                                                 ****/
  /*************************************************************************/
  /*************************************************************************/

  /* documentation is in ftsynth.h */

  FT_EXPORT_DEF( فراغ )
  FT_Glyph_جایگاه_Oblique( FT_Glyph_جایگاه  جایگاه )
  {
    /* Value '0x0366A' corresponds to a shear angle of about 12 degrees. */
    FT_Glyph_جایگاه_Slant( جایگاه, 0x0366A, 0 );
  }


  /* documentation is in ftsynth.h */

  FT_EXPORT_DEF( فراغ )
  FT_Glyph_جایگاه_Slant( FT_Glyph_جایگاه  جایگاه,
                      FT_Fixed      xslant,
                      FT_Fixed      yslant )
  {
    FT_ماتریس    transform;
    FT_Outline*  outline;


    إذا ( !جایگاه )
      أرجع;

    outline = &جایگاه->outline;

    /* only oblique outline glyphs */
    إذا ( جایگاه->format != FT_GLYPH_FORMAT_OUTLINE )
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
  FT_Glyph_جایگاه_Embolden( FT_Glyph_جایگاه  جایگاه )
  {
    FT_Glyph_جایگاه_Adjust_وزن( جایگاه, 0x0AAA, 0x0AAA );
  }


  FT_EXPORT_DEF( فراغ )
  FT_Glyph_جایگاه_Adjust_وزن( FT_Glyph_جایگاه  جایگاه,
                             FT_Fixed      xdelta,
                             FT_Fixed      ydelta )
  {
    FT_Library  library;
    FT_اندازه     اندازه;
    FT_Error    error;
    FT_Pos      xstr, ystr;


    إذا ( !جایگاه )
      أرجع;

    library = جایگاه->library;
    اندازه    = جایگاه->face->اندازه;

    إذا ( جایگاه->format != FT_GLYPH_FORMAT_OUTLINE &&
         جایگاه->format != FT_GLYPH_FORMAT_BITMAP  )
      أرجع;

    /* express deltas in pixels in 26.6 format */
    xstr = (FT_Pos)اندازه->metrics.x_ppem * xdelta / 1024;
    ystr = (FT_Pos)اندازه->metrics.y_ppem * ydelta / 1024;

    إذا ( جایگاه->format == FT_GLYPH_FORMAT_OUTLINE )
      FT_Outline_EmboldenXY( &جایگاه->outline, xstr, ystr );

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
      error = FT_Glyph_جایگاه_Own_Bitmap( جایگاه );
      إذا ( error )
        أرجع;

      error = FT_Bitmap_Embolden( library, &جایگاه->bitmap, xstr, ystr );
      إذا ( error )
        أرجع;
    }

    إذا ( جایگاه->advance.x )
      جایگاه->advance.x += xstr;

    إذا ( جایگاه->advance.y )
      جایگاه->advance.y += ystr;

    جایگاه->metrics.عرض        += xstr;
    جایگاه->metrics.ارتفاع       += ystr;
    جایگاه->metrics.horiAdvance  += xstr;
    جایگاه->metrics.vertAdvance  += ystr;
    جایگاه->metrics.horiBearingY += ystr;

    /* XXX: 16-bit overflow case must be excluded before here */
    إذا ( جایگاه->format == FT_GLYPH_FORMAT_BITMAP )
      جایگاه->bitmap_بالاترین += (FT_Int)( ystr >> 6 );
  }


/*انتها*/
