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


#شامل <freetype/ftsynth.h>
#شامل <freetype/internal/ftdebug.h>
#شامل <freetype/internal/ftobjs.h>
#شامل <freetype/ftoutln.h>
#شامل <freetype/ftbitmap.h>


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#تعریف_منسوخ  FT_COMPONENT
#تعریف_کرو FT_COMPONENT  synth


  /*************************************************************************/
  /*************************************************************************/
  /****                                                                 ****/
  /****   EXPERIMENTAL OBLIQUING SUPPORT                                ****/
  /****                                                                 ****/
  /*************************************************************************/
  /*************************************************************************/

  /* documentation is in ftsynth.h */

  FT_EXPORT_DEF( خالی )
  FT_Glyph_خانہ_Oblique( FT_Glyph_خانہ  خانہ )
  {
    /* Value '0x0366A' corresponds to a shear angle of about 12 degrees. */
    FT_Glyph_خانہ_Slant( خانہ, 0x0366A, 0 );
  }


  /* documentation is in ftsynth.h */

  FT_EXPORT_DEF( خالی )
  FT_Glyph_خانہ_Slant( FT_Glyph_خانہ  خانہ,
                      FT_Fixed      xslant,
                      FT_Fixed      yslant )
  {
    FT_میٹرکس    transform;
    FT_Outline*  outline;


    اگر ( !خانہ )
      واپس;

    outline = &خانہ->outline;

    /* only oblique outline glyphs */
    اگر ( خانہ->format != FT_GLYPH_FORMAT_OUTLINE )
      واپس;

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

  FT_EXPORT_DEF( خالی )
  FT_Glyph_خانہ_Embolden( FT_Glyph_خانہ  خانہ )
  {
    FT_Glyph_خانہ_Adjust_وزن( خانہ, 0x0AAA, 0x0AAA );
  }


  FT_EXPORT_DEF( خالی )
  FT_Glyph_خانہ_Adjust_وزن( FT_Glyph_خانہ  خانہ,
                             FT_Fixed      xdelta,
                             FT_Fixed      ydelta )
  {
    FT_Library  library;
    FT_سائز     سائز;
    FT_Error    error;
    FT_Pos      xstr, ystr;


    اگر ( !خانہ )
      واپس;

    library = خانہ->library;
    سائز    = خانہ->face->سائز;

    اگر ( خانہ->format != FT_GLYPH_FORMAT_OUTLINE &&
         خانہ->format != FT_GLYPH_FORMAT_BITMAP  )
      واپس;

    /* express deltas in pixels in 26.6 format */
    xstr = (FT_Pos)سائز->metrics.x_ppem * xdelta / 1024;
    ystr = (FT_Pos)سائز->metrics.y_ppem * ydelta / 1024;

    اگر ( خانہ->format == FT_GLYPH_FORMAT_OUTLINE )
      FT_Outline_EmboldenXY( &خانہ->outline, xstr, ystr );

    ورنہ /* slot->format == FT_GLYPH_FORMAT_BITMAP */
    {
      /* round to full pixels */
      xstr &= ~63;
      اگر ( xstr == 0 )
        xstr = 1 << 6;
      ystr &= ~63;

      /*
       * XXX: overflow check for 16-bit system, for compatibility
       *      with FT_GlyphSlot_Embolden() since FreeType 2.1.10.
       *      unfortunately, this function returns no information
       *      about the cause of error.
       */
      اگر ( ( ystr >> 6 ) > FT_INT_MAX || ( ystr >> 6 ) < FT_INT_MIN )
      {
        FT_TRACE1(( "FT_GlyphSlot_Embolden:" ));
        FT_TRACE1(( "too strong emboldening parameter ystr=%ld\n", ystr ));
        واپس;
      }
      error = FT_Glyph_خانہ_Own_Bitmap( خانہ );
      اگر ( error )
        واپس;

      error = FT_Bitmap_Embolden( library, &خانہ->bitmap, xstr, ystr );
      اگر ( error )
        واپس;
    }

    اگر ( خانہ->advance.x )
      خانہ->advance.x += xstr;

    اگر ( خانہ->advance.y )
      خانہ->advance.y += ystr;

    خانہ->metrics.چوڑائی        += xstr;
    خانہ->metrics.اونچائی       += ystr;
    خانہ->metrics.horiAdvance  += xstr;
    خانہ->metrics.vertAdvance  += ystr;
    خانہ->metrics.horiBearingY += ystr;

    /* XXX: 16-bit overflow case must be excluded before here */
    اگر ( خانہ->format == FT_GLYPH_FORMAT_BITMAP )
      خانہ->bitmap_چوٹی += (FT_Int)( ystr >> 6 );
  }


/*انتہا*/
