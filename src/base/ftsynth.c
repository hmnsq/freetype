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


#अन्तर्भाव <freetype/ftsynth.h>
#अन्तर्भाव <freetype/internal/ftdebug.h>
#अन्तर्भाव <freetype/internal/ftobjs.h>
#अन्तर्भाव <freetype/ftoutln.h>
#अन्तर्भाव <freetype/ftbitmap.h>


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#undef  FT_COMPONENT
#define FT_COMPONENT  synth


  /*************************************************************************/
  /*************************************************************************/
  /****                                                                 ****/
  /****   EXPERIMENTAL OBLIQUING SUPPORT                                ****/
  /****                                                                 ****/
  /*************************************************************************/
  /*************************************************************************/

  /* documentation is in ftsynth.h */

  FT_EXPORT_DEF( शून्य )
  FT_GlyphSlot_Oblique( FT_GlyphSlot  slot )
  {
    /* Value '0x0366A' corresponds to a shear angle of about 12 degrees. */
    FT_GlyphSlot_Slant( slot, 0x0366A, 0 );
  }


  /* documentation is in ftsynth.h */

  FT_EXPORT_DEF( शून्य )
  FT_GlyphSlot_Slant( FT_GlyphSlot  slot,
                      FT_Fixed      xslant,
                      FT_Fixed      yslant )
  {
    FT_Matrix    transform;
    FT_Outline*  outline;


    यदि ( !slot )
      फर्काऊ;

    outline = &slot->outline;

    /* only oblique outline glyphs */
    यदि ( slot->format != FT_GLYPH_FORMAT_OUTLINE )
      फर्काऊ;

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

  FT_EXPORT_DEF( शून्य )
  FT_GlyphSlot_Embolden( FT_GlyphSlot  slot )
  {
    FT_GlyphSlot_AdjustWeight( slot, 0x0AAA, 0x0AAA );
  }


  FT_EXPORT_DEF( शून्य )
  FT_GlyphSlot_AdjustWeight( FT_GlyphSlot  slot,
                             FT_Fixed      xdelta,
                             FT_Fixed      ydelta )
  {
    FT_Library  library;
    FT_Size     size;
    FT_Error    error;
    FT_Pos      xstr, ystr;


    यदि ( !slot )
      फर्काऊ;

    library = slot->library;
    size    = slot->face->size;

    यदि ( slot->format != FT_GLYPH_FORMAT_OUTLINE &&
         slot->format != FT_GLYPH_FORMAT_BITMAP  )
      फर्काऊ;

    /* express deltas in pixels in 26.6 format */
    xstr = (FT_Pos)size->metrics.x_ppem * xdelta / 1024;
    ystr = (FT_Pos)size->metrics.y_ppem * ydelta / 1024;

    यदि ( slot->format == FT_GLYPH_FORMAT_OUTLINE )
      FT_Outline_EmboldenXY( &slot->outline, xstr, ystr );

    अन्यथा /* slot->format == FT_GLYPH_FORMAT_BITMAP */
    {
      /* round to full pixels */
      xstr &= ~63;
      यदि ( xstr == 0 )
        xstr = 1 << 6;
      ystr &= ~63;

      /*
       * XXX: overflow check for 16-bit system, for compatibility
       *      with FT_GlyphSlot_Embolden() since FreeType 2.1.10.
       *      unfortunately, this function returns no information
       *      about the cause of error.
       */
      यदि ( ( ystr >> 6 ) > FT_INT_MAX || ( ystr >> 6 ) < FT_INT_MIN )
      {
        FT_TRACE1(( "FT_GlyphSlot_Embolden:" ));
        FT_TRACE1(( "too strong emboldening parameter ystr=%ld\n", ystr ));
        फर्काऊ;
      }
      error = FT_GlyphSlot_Own_Bitmap( slot );
      यदि ( error )
        फर्काऊ;

      error = FT_Bitmap_Embolden( library, &slot->bitmap, xstr, ystr );
      यदि ( error )
        फर्काऊ;
    }

    यदि ( slot->advance.x )
      slot->advance.x += xstr;

    यदि ( slot->advance.y )
      slot->advance.y += ystr;

    slot->metrics.width        += xstr;
    slot->metrics.height       += ystr;
    slot->metrics.horiAdvance  += xstr;
    slot->metrics.vertAdvance  += ystr;
    slot->metrics.horiBearingY += ystr;

    /* XXX: 16-bit overflow case must be excluded before here */
    यदि ( slot->format == FT_GLYPH_FORMAT_BITMAP )
      slot->bitmap_शीर्ष += (FT_Int)( ystr >> 6 );
  }


/* END */
