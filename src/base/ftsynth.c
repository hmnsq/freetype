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


#ಅನ್ತರ್ಭಾವ <freetype/ftsynth.h>
#ಅನ್ತರ್ಭಾವ <freetype/internal/ftdebug.h>
#ಅನ್ತರ್ಭಾವ <freetype/internal/ftobjs.h>
#ಅನ್ತರ್ಭಾವ <freetype/ftoutln.h>
#ಅನ್ತರ್ಭಾವ <freetype/ftbitmap.h>


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು  FT_COMPONENT
#ವ್ಯಾಖ್ಯಾನಿಸು FT_COMPONENT  synth


  /*************************************************************************/
  /*************************************************************************/
  /****                                                                 ****/
  /****   EXPERIMENTAL OBLIQUING SUPPORT                                ****/
  /****                                                                 ****/
  /*************************************************************************/
  /*************************************************************************/

  /* documentation is in ftsynth.h */

  FT_EXPORT_DEF( ಶೂನ್ಯ )
  FT_Glyph_ಸ್ಥಾನ_Oblique( FT_Glyph_ಸ್ಥಾನ  ಸ್ಥಾನ )
  {
    /* Value '0x0366A' corresponds to a shear angle of about 12 degrees. */
    FT_Glyph_ಸ್ಥಾನ_Slant( ಸ್ಥಾನ, 0x0366A, 0 );
  }


  /* documentation is in ftsynth.h */

  FT_EXPORT_DEF( ಶೂನ್ಯ )
  FT_Glyph_ಸ್ಥಾನ_Slant( FT_Glyph_ಸ್ಥಾನ  ಸ್ಥಾನ,
                      FT_Fixed      xslant,
                      FT_Fixed      yslant )
  {
    FT_ಮಾತೃಕೆ    transform;
    FT_Outline*  outline;


    ಆದರೆ ( !ಸ್ಥಾನ )
      ಹಿಂತಿರುಗಿ;

    outline = &ಸ್ಥಾನ->outline;

    /* only oblique outline glyphs */
    ಆದರೆ ( ಸ್ಥಾನ->format != FT_GLYPH_FORMAT_OUTLINE )
      ಹಿಂತಿರುಗಿ;

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

  FT_EXPORT_DEF( ಶೂನ್ಯ )
  FT_Glyph_ಸ್ಥಾನ_Embolden( FT_Glyph_ಸ್ಥಾನ  ಸ್ಥಾನ )
  {
    FT_Glyph_ಸ್ಥಾನ_Adjust_ತೂಕ( ಸ್ಥಾನ, 0x0AAA, 0x0AAA );
  }


  FT_EXPORT_DEF( ಶೂನ್ಯ )
  FT_Glyph_ಸ್ಥಾನ_Adjust_ತೂಕ( FT_Glyph_ಸ್ಥಾನ  ಸ್ಥಾನ,
                             FT_Fixed      xdelta,
                             FT_Fixed      ydelta )
  {
    FT_Library  library;
    FT_ಗಾತ್ರ     ಗಾತ್ರ_2;
    FT_Error    error;
    FT_Pos      xstr, ystr;


    ಆದರೆ ( !ಸ್ಥಾನ )
      ಹಿಂತಿರುಗಿ;

    library = ಸ್ಥಾನ->library;
    ಗಾತ್ರ_2    = ಸ್ಥಾನ->face->ಗಾತ್ರ_2;

    ಆದರೆ ( ಸ್ಥಾನ->format != FT_GLYPH_FORMAT_OUTLINE &&
         ಸ್ಥಾನ->format != FT_GLYPH_FORMAT_BITMAP  )
      ಹಿಂತಿರುಗಿ;

    /* express deltas in pixels in 26.6 format */
    xstr = (FT_Pos)ಗಾತ್ರ_2->metrics.x_ppem * xdelta / 1024;
    ystr = (FT_Pos)ಗಾತ್ರ_2->metrics.y_ppem * ydelta / 1024;

    ಆದರೆ ( ಸ್ಥಾನ->format == FT_GLYPH_FORMAT_OUTLINE )
      FT_Outline_EmboldenXY( &ಸ್ಥಾನ->outline, xstr, ystr );

    ಇಲ್ಲದಿದ್ದರೆ /* slot->format == FT_GLYPH_FORMAT_BITMAP */
    {
      /* round to full pixels */
      xstr &= ~63;
      ಆದರೆ ( xstr == 0 )
        xstr = 1 << 6;
      ystr &= ~63;

      /*
       * XXX: overflow check for 16-bit system, for compatibility
       *      with FT_GlyphSlot_Embolden() since FreeType 2.1.10.
       *      unfortunately, this function returns no information
       *      about the cause of error.
       */
      ಆದರೆ ( ( ystr >> 6 ) > FT_INT_MAX || ( ystr >> 6 ) < FT_INT_MIN )
      {
        FT_TRACE1(( "FT_GlyphSlot_Embolden:" ));
        FT_TRACE1(( "too strong emboldening parameter ystr=%ld\n", ystr ));
        ಹಿಂತಿರುಗಿ;
      }
      error = FT_Glyph_ಸ್ಥಾನ_Own_Bitmap( ಸ್ಥಾನ );
      ಆದರೆ ( error )
        ಹಿಂತಿರುಗಿ;

      error = FT_Bitmap_Embolden( library, &ಸ್ಥಾನ->bitmap, xstr, ystr );
      ಆದರೆ ( error )
        ಹಿಂತಿರುಗಿ;
    }

    ಆದರೆ ( ಸ್ಥಾನ->advance.x )
      ಸ್ಥಾನ->advance.x += xstr;

    ಆದರೆ ( ಸ್ಥಾನ->advance.y )
      ಸ್ಥಾನ->advance.y += ystr;

    ಸ್ಥಾನ->metrics.ಅಗಲ        += xstr;
    ಸ್ಥಾನ->metrics.ಎತ್ತರ       += ystr;
    ಸ್ಥಾನ->metrics.horiAdvance  += xstr;
    ಸ್ಥಾನ->metrics.vertAdvance  += ystr;
    ಸ್ಥಾನ->metrics.horiBearingY += ystr;

    /* XXX: 16-bit overflow case must be excluded before here */
    ಆದರೆ ( ಸ್ಥಾನ->format == FT_GLYPH_FORMAT_BITMAP )
      ಸ್ಥಾನ->bitmap_ತುದಿ += (FT_Int)( ystr >> 6 );
  }


/*ಕೊನೆ*/
