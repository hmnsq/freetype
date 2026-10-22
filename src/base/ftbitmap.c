/****************************************************************************
 *
 * ftbitmap.c
 *
 *   FreeType utility functions for bitmaps (body).
 *
 * Copyright (C) 2004-2026 by
 * David Turner, Robert Wilhelm, and Werner Lemberg.
 *
 * This file is part of the FreeType project, and may only be used,
 * modified, and distributed under the terms of the FreeType project
 * license, LICENSE.TXT.  By continuing to use, modify, or distribute
 * this file you indicate that you have read the license and
 * understand and accept it fully.
 *
 */


#ಅನ್ತರ್ಭಾವ <freetype/internal/ftdebug.h>

#ಅನ್ತರ್ಭಾವ <freetype/ftbitmap.h>
#ಅನ್ತರ್ಭಾವ <freetype/ftimage.h>
#ಅನ್ತರ್ಭಾವ <freetype/internal/ftobjs.h>


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು  FT_COMPONENT
#ವ್ಯಾಖ್ಯಾನಿಸು FT_COMPONENT  bitmap


  ಸ್ಥಿರ
  ಸ್ಥಿರಾಂಕ FT_Bitmap  null_bitmap = { 0, 0, 0, ಖಾಲಿ, 0, 0, 0, ಖಾಲಿ };


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( ಶೂನ್ಯ )
  FT_Bitmap_Init( FT_Bitmap  *abitmap )
  {
    ಆದರೆ ( abitmap )
      *abitmap = null_bitmap;
  }


  /* deprecated function name; retained for ABI compatibility */

  FT_EXPORT_DEF( ಶೂನ್ಯ )
  FT_Bitmap_New( FT_Bitmap  *abitmap )
  {
    ಆದರೆ ( abitmap )
      *abitmap = null_bitmap;
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Bitmap_Copy( FT_Library        library,
                  ಸ್ಥಿರಾಂಕ FT_Bitmap  *ಮೂಲ,
                  FT_Bitmap        *ಗುರಿ)
  {
    FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ;
    FT_Error   error  = FT_Err_Ok;
    FT_Int     pitch;
    FT_Int     flip;


    ಆದರೆ ( !library )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_Library_Handle );

    ಆದರೆ ( !ಮೂಲ || !ಗುರಿ )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

    ಆದರೆ ( ಮೂಲ == ಗುರಿ )
      ಹಿಂತಿರುಗಿ FT_Err_Ok;

    flip = ( ಮೂಲ->pitch < 0 && ಗುರಿ->pitch > 0 ) ||
           ( ಮೂಲ->pitch > 0 && ಗುರಿ->pitch < 0 );

    ಸ್ಮರಣೆ = library->ಸ್ಮರಣೆ;
    FT_FREE( ಗುರಿ->ಬಫರ್ );

    *ಗುರಿ = *ಮೂಲ;

    ಆದರೆ ( flip )
      ಗುರಿ->pitch = -ಗುರಿ->pitch;

    ಆದರೆ ( !ಮೂಲ->ಬಫರ್ )
      ಹಿಂತಿರುಗಿ FT_Err_Ok;

    pitch  = ಮೂಲ->pitch;
    ಆದರೆ ( pitch < 0 )
      pitch = -pitch;

    FT_MEM_QALLOC_MULT( ಗುರಿ->ಬಫರ್, ಗುರಿ->ಅಡ್ಡಸಾಲುಗಳು, pitch );

    ಆದರೆ ( !error )
    {
      ಆದರೆ ( flip )
      {
        /* take care of bitmap flow */
        FT_UInt   i;
        FT_Byte*  s = ಮೂಲ->ಬಫರ್;
        FT_Byte*  t = ಗುರಿ->ಬಫರ್;


        t += (FT_ULong)pitch * ( ಗುರಿ->ಅಡ್ಡಸಾಲುಗಳು - 1 );

        ಗಾಗಿ ( i = ಗುರಿ->ಅಡ್ಡಸಾಲುಗಳು; i > 0; i-- )
        {
          FT_ARRAY_COPY( t, s, pitch );

          s += pitch;
          t -= pitch;
        }
      }
      ಇಲ್ಲದಿದ್ದರೆ
        FT_MEM_COPY( ಗುರಿ->ಬಫರ್, ಮೂಲ->ಬಫರ್,
                     (FT_Long)ಮೂಲ->ಅಡ್ಡಸಾಲುಗಳು * pitch );
    }

    ಹಿಂತಿರುಗಿ error;
  }


  /* Enlarge `bitmap' horizontally and vertically by `xpixels' */
  /* and `ypixels', respectively.                              */

  ಸ್ಥಿರ FT_Error
  ft_bitmap_assure_ಬಫರ್( FT_ಸ್ಮರಣೆ   ಸ್ಮರಣೆ,
                           FT_Bitmap*  bitmap,
                           FT_UInt     xpixels,
                           FT_UInt     ypixels )
  {
    FT_Error        error;
    ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ    pitch;
    ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ    new_pitch;
    FT_UInt         bpp;
    FT_UInt         ಅಗಲ, ಎತ್ತರ;
    ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ*  ಬಫರ್ = ಖಾಲಿ;


    ಅಗಲ  = bitmap->ಅಗಲ;
    ಎತ್ತರ = bitmap->ಅಡ್ಡಸಾಲುಗಳು;
    pitch  = (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ)FT_ABS( bitmap->pitch );

    ಆಯ್ಕೆ ( bitmap->pixel_ಬಹುಲಕ )
    {
    ಸ್ಥಿತಿ FT_PIXEL_ಬಹುಲಕ_MONO:
      bpp       = 1;
      new_pitch = ( ಅಗಲ + xpixels + 7 ) >> 3;
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ FT_PIXEL_ಬಹುಲಕ_GRAY2:
      bpp       = 2;
      new_pitch = ( ಅಗಲ + xpixels + 3 ) >> 2;
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ FT_PIXEL_ಬಹುಲಕ_GRAY4:
      bpp       = 4;
      new_pitch = ( ಅಗಲ + xpixels + 1 ) >> 1;
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ FT_PIXEL_ಬಹುಲಕ_ಬೂದು:
    ಸ್ಥಿತಿ FT_PIXEL_ಬಹುಲಕ_LCD:
    ಸ್ಥಿತಿ FT_PIXEL_ಬಹುಲಕ_LCD_V:
      bpp       = 8;
      new_pitch = ಅಗಲ + xpixels;
      ನಿಲ್ಲಿಸು;
    ಪೂರ್ವನಿಯೋಜಿತ:
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_Glyph_Format );
    }

    /* if no need to allocate memory */
    ಆದರೆ ( ypixels == 0 && new_pitch <= pitch )
    {
      /* zero the padding */
      FT_UInt  ಬಿಟ್_ಅಗಲ = pitch * 8;
      FT_UInt  ಬಿಟ್_ಕೊನೆಯ  = ( ಅಗಲ + xpixels ) * bpp;


      ಆದರೆ ( ಬಿಟ್_ಕೊನೆಯ < ಬಿಟ್_ಅಗಲ )
      {
        FT_Byte*  ಸಾಲು  = bitmap->ಬಫರ್ + ( ಬಿಟ್_ಕೊನೆಯ >> 3 );
        FT_Byte*  ಕೊನೆ   = bitmap->ಬಫರ್ + pitch;
        FT_UInt   ಸರಿಸು = ಬಿಟ್_ಕೊನೆಯ & 7;
        FT_UInt   ಮುಖವಾಡ  = 0xFF00U >> ಸರಿಸು;
        FT_UInt   ಎಣಿಕೆ_2 = ಎತ್ತರ;


        ಗಾಗಿ ( ; ಎಣಿಕೆ_2 > 0; ಎಣಿಕೆ_2--, ಸಾಲು += pitch, ಕೊನೆ += pitch )
        {
          FT_Byte*  ಲಿಖ = ಸಾಲು;


          ಆದರೆ ( ಸರಿಸು > 0 )
          {
            ಲಿಖ[0] = (FT_Byte)( ಲಿಖ[0] & ಮುಖವಾಡ );
            ಲಿಖ++;
          }
          ಆದರೆ ( ಲಿಖ < ಕೊನೆ )
            FT_MEM_ZERO( ಲಿಖ, ಕೊನೆ - ಲಿಖ );
        }
      }

      ಹಿಂತಿರುಗಿ FT_Err_Ok;
    }

    /* otherwise allocate new buffer */
    ಆದರೆ ( FT_QALLOC_MULT( ಬಫರ್, bitmap->ಅಡ್ಡಸಾಲುಗಳು + ypixels, new_pitch ) )
      ಹಿಂತಿರುಗಿ error;

    /* new rows get added at the top of the bitmap, */
    /* thus take care of the flow direction         */
    ಆದರೆ ( bitmap->pitch > 0 )
    {
      FT_UInt  len = ( ಅಗಲ * bpp + 7 ) >> 3;

      ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ*  ಒಳ  = bitmap->ಬಫರ್;
      ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ*  ಹೊರಹೋಗು = ಬಫರ್;

      ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ*  ಎಲ್ಲೆ = bitmap->ಬಫರ್ + pitch * bitmap->ಅಡ್ಡಸಾಲುಗಳು;
      ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ    delta = new_pitch - len;


      FT_MEM_ZERO( ಹೊರಹೋಗು, new_pitch * ypixels );
      ಹೊರಹೋಗು += new_pitch * ypixels;

      ಆಗ ( ಒಳ < ಎಲ್ಲೆ )
      {
        FT_MEM_COPY( ಹೊರಹೋಗು, ಒಳ, len );
        ಒಳ  += pitch;
        ಹೊರಹೋಗು += len;

        /* we use FT_QALLOC_MULT, which doesn't zero out the buffer;      */
        /* consequently, we have to manually zero out the remaining bytes */
        FT_MEM_ZERO( ಹೊರಹೋಗು, delta );
        ಹೊರಹೋಗು += delta;
      }
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
      FT_UInt  len = ( ಅಗಲ * bpp + 7 ) >> 3;

      ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ*  ಒಳ  = bitmap->ಬಫರ್;
      ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ*  ಹೊರಹೋಗು = ಬಫರ್;

      ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ*  ಎಲ್ಲೆ = bitmap->ಬಫರ್ + pitch * bitmap->ಅಡ್ಡಸಾಲುಗಳು;
      ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ    delta = new_pitch - len;


      ಆಗ ( ಒಳ < ಎಲ್ಲೆ )
      {
        FT_MEM_COPY( ಹೊರಹೋಗು, ಒಳ, len );
        ಒಳ  += pitch;
        ಹೊರಹೋಗು += len;

        FT_MEM_ZERO( ಹೊರಹೋಗು, delta );
        ಹೊರಹೋಗು += delta;
      }

      FT_MEM_ZERO( ಹೊರಹೋಗು, new_pitch * ypixels );
    }

    FT_FREE( bitmap->ಬಫರ್ );
    bitmap->ಬಫರ್ = ಬಫರ್;

    /* set pitch only, width and height are left untouched */
    ಆದರೆ ( bitmap->pitch < 0 )
      bitmap->pitch = -(ಪೂರ್ಣಾಂಕ)new_pitch;
    ಇಲ್ಲದಿದ್ದರೆ
      bitmap->pitch = (ಪೂರ್ಣಾಂಕ)new_pitch;

    ಹಿಂತಿರುಗಿ FT_Err_Ok;
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Bitmap_Embolden( FT_Library  library,
                      FT_Bitmap*  bitmap,
                      FT_Pos      xStrength,
                      FT_Pos      yStrength )
  {
    FT_Error        error;
    ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ*  p;
    FT_Int          i, x, pitch;
    FT_UInt         y;
    FT_Int          xstr, ystr;


    ಆದರೆ ( !library )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_Library_Handle );

    ಆದರೆ ( !bitmap || !bitmap->ಬಫರ್ )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

    ಆದರೆ ( ( ( FT_PIX_ಸುತ್ತು( xStrength ) >> 6 ) > FT_INT_MAX ) ||
         ( ( FT_PIX_ಸುತ್ತು( yStrength ) >> 6 ) > FT_INT_MAX ) )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

    xstr = (FT_Int)FT_PIX_ಸುತ್ತು( xStrength ) >> 6;
    ystr = (FT_Int)FT_PIX_ಸುತ್ತು( yStrength ) >> 6;

    ಆದರೆ ( xstr == 0 && ystr == 0 )
      ಹಿಂತಿರುಗಿ FT_Err_Ok;
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ( xstr < 0 || ystr < 0 )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

    ಆಯ್ಕೆ ( bitmap->pixel_ಬಹುಲಕ )
    {
    ಸ್ಥಿತಿ FT_PIXEL_ಬಹುಲಕ_GRAY2:
    ಸ್ಥಿತಿ FT_PIXEL_ಬಹುಲಕ_GRAY4:
      {
        FT_Bitmap  tmp;


        /* convert to 8bpp */
        FT_Bitmap_Init( &tmp );
        error = FT_Bitmap_Convert( library, bitmap, &tmp, 1 );
        ಆದರೆ ( error )
          ಹಿಂತಿರುಗಿ error;

        FT_Bitmap_ಮುಗಿದ( library, bitmap );
        *bitmap = tmp;
      }
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ FT_PIXEL_ಬಹುಲಕ_MONO:
      ಆದರೆ ( xstr > 8 )
        xstr = 8;
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ FT_PIXEL_ಬಹುಲಕ_LCD:
      xstr *= 3;
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ FT_PIXEL_ಬಹುಲಕ_LCD_V:
      ystr *= 3;
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ FT_PIXEL_ಬಹುಲಕ_BGRA:
      /* We don't embolden color glyphs. */
      ಹಿಂತಿರುಗಿ FT_Err_Ok;
    }

    error = ft_bitmap_assure_ಬಫರ್( library->ಸ್ಮರಣೆ, bitmap,
                                     (FT_UInt)xstr, (FT_UInt)ystr );
    ಆದರೆ ( error )
      ಹಿಂತಿರುಗಿ error;

    /* take care of bitmap flow */
    pitch = bitmap->pitch;
    ಆದರೆ ( pitch > 0 )
      p = bitmap->ಬಫರ್ + pitch * ystr;
    ಇಲ್ಲದಿದ್ದರೆ
    {
      pitch = -pitch;
      p = bitmap->ಬಫರ್ + (FT_UInt)pitch * ( bitmap->ಅಡ್ಡಸಾಲುಗಳು - 1 );
    }

    /* for each row */
    ಗಾಗಿ ( y = 0; y < bitmap->ಅಡ್ಡಸಾಲುಗಳು; y++ )
    {
      /*
       * Horizontally:
       *
       * From the last pixel on, make each pixel or'ed with the
       * `xstr' pixels before it.
       */
      ಗಾಗಿ ( x = pitch - 1; x >= 0; x-- )
      {
        ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ  tmp;


        tmp = p[x];
        ಗಾಗಿ ( i = 1; i <= xstr; i++ )
        {
          ಆದರೆ ( bitmap->pixel_ಬಹುಲಕ == FT_PIXEL_ಬಹುಲಕ_MONO )
          {
            p[x] |= tmp >> i;

            /* the maximum value of 8 for `xstr' comes from here */
            ಆದರೆ ( x > 0 )
              p[x] |= p[x - 1] << ( 8 - i );

#ಆದರೆ 0
            ಆದರೆ ( p[x] == 0xFF )
              ನಿಲ್ಲಿಸು;
#ಆದರೆ_ಅಂತ್ಯ
          }
          ಇಲ್ಲದಿದ್ದರೆ
          {
            ಆದರೆ ( x - i >= 0 )
            {
              ಆದರೆ ( p[x] + p[x - i] > bitmap->ಅಂಕೆ_grays - 1 )
              {
                p[x] = (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ)( bitmap->ಅಂಕೆ_grays - 1 );
                ನಿಲ್ಲಿಸು;
              }
              ಇಲ್ಲದಿದ್ದರೆ
              {
                p[x] = (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ)( p[x] + p[x - i] );
                ಆದರೆ ( p[x] == bitmap->ಅಂಕೆ_grays - 1 )
                  ನಿಲ್ಲಿಸು;
              }
            }
            ಇಲ್ಲದಿದ್ದರೆ
              ನಿಲ್ಲಿಸು;
          }
        }
      }

      /*
       * Vertically:
       *
       * Make the above `ystr' rows or'ed with it.
       */
      ಗಾಗಿ ( x = 1; x <= ystr; x++ )
      {
        ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ*  q;


        q = p - bitmap->pitch * x;
        ಗಾಗಿ ( i = 0; i < pitch; i++ )
          q[i] |= p[i];
      }

      p += bitmap->pitch;
    }

    bitmap->ಅಗಲ += (FT_UInt)xstr;
    bitmap->ಅಡ್ಡಸಾಲುಗಳು += (FT_UInt)ystr;

    ಹಿಂತಿರುಗಿ FT_Err_Ok;
  }


  ಸ್ಥಿರ FT_Byte
  ft_ಬೂದು_for_premultiplied_srgb_bgra( ಸ್ಥಿರಾಂಕ FT_Byte*  bgra )
  {
    FT_UInt  a = bgra[3];
    FT_UInt  l;


    /* Short-circuit transparent color to avoid division by zero. */
    ಆದರೆ ( !a )
      ಹಿಂತಿರುಗಿ 0;

    /*
     * Luminosity for sRGB is defined using ~0.2126,0.7152,0.0722
     * coefficients for RGB channels *on the linear colors*.
     * A gamma of 2.2 is fair to assume.  And then, we need to
     * undo the premultiplication too.
     *
     *   http://www.brucelindbloom.com/index.html?WorkingSpaceInfo.html#SideNotes
     *
     * We do the computation with integers only, applying a gamma of 2.0.
     * We guarantee 32-bit arithmetic to avoid overflow but the resulting
     * luminosity fits into 16 bits.
     *
     */

    l = (  4731UL /* 0.072186 * 65536 */ * bgra[0] * bgra[0] +
          46868UL /* 0.715158 * 65536 */ * bgra[1] * bgra[1] +
          13937UL /* 0.212656 * 65536 */ * bgra[2] * bgra[2] ) >> 16;

    /*
     * Final transparency can be determined as follows.
     *
     * - If alpha is zero, we want 0.
     * - If alpha is zero and luminosity is zero, we want 255.
     * - If alpha is zero and luminosity is one, we want 0.
     *
     * So the formula is a * (1 - l) = a - l * a.
     *
     * We still need to undo premultiplication by dividing l by a*a.
     *
     */

    ಹಿಂತಿರುಗಿ (FT_Byte)( a - l / a );
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Bitmap_Convert( FT_Library        library,
                     ಸ್ಥಿರಾಂಕ FT_Bitmap  *ಮೂಲ,
                     FT_Bitmap        *ಗುರಿ,
                     FT_Int            alignment )
  {
    FT_Error   error = FT_Err_Ok;
    FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ;

    FT_Byte*  s;
    FT_Byte*  t;


    ಆದರೆ ( !library )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_Library_Handle );

    ಆದರೆ ( !ಮೂಲ || !ಗುರಿ )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

    ಸ್ಮರಣೆ = library->ಸ್ಮರಣೆ;

    ಆಯ್ಕೆ ( ಮೂಲ->pixel_ಬಹುಲಕ )
    {
    ಸ್ಥಿತಿ FT_PIXEL_ಬಹುಲಕ_MONO:
    ಸ್ಥಿತಿ FT_PIXEL_ಬಹುಲಕ_ಬೂದು:
    ಸ್ಥಿತಿ FT_PIXEL_ಬಹುಲಕ_GRAY2:
    ಸ್ಥಿತಿ FT_PIXEL_ಬಹುಲಕ_GRAY4:
    ಸ್ಥಿತಿ FT_PIXEL_ಬಹುಲಕ_LCD:
    ಸ್ಥಿತಿ FT_PIXEL_ಬಹುಲಕ_LCD_V:
    ಸ್ಥಿತಿ FT_PIXEL_ಬಹುಲಕ_BGRA:
      {
        FT_Int  ಅಗಲ = (FT_Int)ಮೂಲ->ಅಗಲ;
        FT_Int  neg   = ( ಗುರಿ->pitch == 0 && ಮೂಲ->pitch < 0 ) ||
                          ಗುರಿ->pitch  < 0;


        FT_Bitmap_ಮುಗಿದ( library, ಗುರಿ );

        ಗುರಿ->pixel_ಬಹುಲಕ = FT_PIXEL_ಬಹುಲಕ_ಬೂದು;
        ಗುರಿ->ಅಡ್ಡಸಾಲುಗಳು       = ಮೂಲ->ಅಡ್ಡಸಾಲುಗಳು;
        ಗುರಿ->ಅಗಲ      = ಮೂಲ->ಅಗಲ;

        ಆದರೆ ( alignment )
        {
          FT_Int  rem = ಅಗಲ % alignment;


          ಆದರೆ ( rem )
            ಅಗಲ = alignment > 0 ? ಅಗಲ - rem + alignment
                                  : ಅಗಲ - rem - alignment;
        }

        ಆದರೆ ( FT_QALLOC_MULT( ಗುರಿ->ಬಫರ್, ಗುರಿ->ಅಡ್ಡಸಾಲುಗಳು, ಅಗಲ ) )
          ಹಿಂತಿರುಗಿ error;

        ಗುರಿ->pitch = neg ? -ಅಗಲ : ಅಗಲ;
      }
      ನಿಲ್ಲಿಸು;

    ಪೂರ್ವನಿಯೋಜಿತ:
      error = FT_THROW( Invalid_ವಾದ );
    }

    s = ಮೂಲ->ಬಫರ್;
    t = ಗುರಿ->ಬಫರ್;

    /* take care of bitmap flow */
    ಆದರೆ ( ಮೂಲ->pitch < 0 )
      s -= ಮೂಲ->pitch * (FT_Int)( ಮೂಲ->ಅಡ್ಡಸಾಲುಗಳು - 1 );
    ಆದರೆ ( ಗುರಿ->pitch < 0 )
      t -= ಗುರಿ->pitch * (FT_Int)( ಗುರಿ->ಅಡ್ಡಸಾಲುಗಳು - 1 );

    ಆಯ್ಕೆ ( ಮೂಲ->pixel_ಬಹುಲಕ )
    {
    ಸ್ಥಿತಿ FT_PIXEL_ಬಹುಲಕ_MONO:
      {
        FT_UInt  i;


        ಗುರಿ->ಅಂಕೆ_grays = 2;

        ಗಾಗಿ ( i = ಮೂಲ->ಅಡ್ಡಸಾಲುಗಳು; i > 0; i-- )
        {
          FT_Byte*  ss = s;
          FT_Byte*  tt = t;
          FT_UInt   j;


          /* get the full bytes */
          ಗಾಗಿ ( j = ಮೂಲ->ಅಗಲ >> 3; j > 0; j-- )
          {
            FT_Int  val = ss[0]; /* avoid a byte->int cast on each line */


            tt[0] = (FT_Byte)( ( val & 0x80 ) >> 7 );
            tt[1] = (FT_Byte)( ( val & 0x40 ) >> 6 );
            tt[2] = (FT_Byte)( ( val & 0x20 ) >> 5 );
            tt[3] = (FT_Byte)( ( val & 0x10 ) >> 4 );
            tt[4] = (FT_Byte)( ( val & 0x08 ) >> 3 );
            tt[5] = (FT_Byte)( ( val & 0x04 ) >> 2 );
            tt[6] = (FT_Byte)( ( val & 0x02 ) >> 1 );
            tt[7] = (FT_Byte)(   val & 0x01 );

            tt += 8;
            ss += 1;
          }

          /* get remaining pixels (if any) */
          j = ಮೂಲ->ಅಗಲ & 7;
          ಆದರೆ ( j > 0 )
          {
            FT_Int  val = *ss;


            ಗಾಗಿ ( ; j > 0; j-- )
            {
              tt[0] = (FT_Byte)( ( val & 0x80 ) >> 7);
              val <<= 1;
              tt   += 1;
            }
          }

          s += ಮೂಲ->pitch;
          t += ಗುರಿ->pitch;
        }
      }
      ನಿಲ್ಲಿಸು;


    ಸ್ಥಿತಿ FT_PIXEL_ಬಹುಲಕ_ಬೂದು:
    ಸ್ಥಿತಿ FT_PIXEL_ಬಹುಲಕ_LCD:
    ಸ್ಥಿತಿ FT_PIXEL_ಬಹುಲಕ_LCD_V:
      {
        FT_UInt  ಅಗಲ = ಮೂಲ->ಅಗಲ;
        FT_UInt  i;


        ಗುರಿ->ಅಂಕೆ_grays = 256;

        ಗಾಗಿ ( i = ಮೂಲ->ಅಡ್ಡಸಾಲುಗಳು; i > 0; i-- )
        {
          FT_ARRAY_COPY( t, s, ಅಗಲ );

          s += ಮೂಲ->pitch;
          t += ಗುರಿ->pitch;
        }
      }
      ನಿಲ್ಲಿಸು;


    ಸ್ಥಿತಿ FT_PIXEL_ಬಹುಲಕ_GRAY2:
      {
        FT_UInt  i;


        ಗುರಿ->ಅಂಕೆ_grays = 4;

        ಗಾಗಿ ( i = ಮೂಲ->ಅಡ್ಡಸಾಲುಗಳು; i > 0; i-- )
        {
          FT_Byte*  ss = s;
          FT_Byte*  tt = t;
          FT_UInt   j;


          /* get the full bytes */
          ಗಾಗಿ ( j = ಮೂಲ->ಅಗಲ >> 2; j > 0; j-- )
          {
            FT_Int  val = ss[0];


            tt[0] = (FT_Byte)( ( val & 0xC0 ) >> 6 );
            tt[1] = (FT_Byte)( ( val & 0x30 ) >> 4 );
            tt[2] = (FT_Byte)( ( val & 0x0C ) >> 2 );
            tt[3] = (FT_Byte)( ( val & 0x03 ) );

            ss += 1;
            tt += 4;
          }

          j = ಮೂಲ->ಅಗಲ & 3;
          ಆದರೆ ( j > 0 )
          {
            FT_Int  val = ss[0];


            ಗಾಗಿ ( ; j > 0; j-- )
            {
              tt[0]  = (FT_Byte)( ( val & 0xC0 ) >> 6 );
              val  <<= 2;
              tt    += 1;
            }
          }

          s += ಮೂಲ->pitch;
          t += ಗುರಿ->pitch;
        }
      }
      ನಿಲ್ಲಿಸು;


    ಸ್ಥಿತಿ FT_PIXEL_ಬಹುಲಕ_GRAY4:
      {
        FT_UInt  i;


        ಗುರಿ->ಅಂಕೆ_grays = 16;

        ಗಾಗಿ ( i = ಮೂಲ->ಅಡ್ಡಸಾಲುಗಳು; i > 0; i-- )
        {
          FT_Byte*  ss = s;
          FT_Byte*  tt = t;
          FT_UInt   j;


          /* get the full bytes */
          ಗಾಗಿ ( j = ಮೂಲ->ಅಗಲ >> 1; j > 0; j-- )
          {
            FT_Int  val = ss[0];


            tt[0] = (FT_Byte)( ( val & 0xF0 ) >> 4 );
            tt[1] = (FT_Byte)( ( val & 0x0F ) );

            ss += 1;
            tt += 2;
          }

          ಆದರೆ ( ಮೂಲ->ಅಗಲ & 1 )
            tt[0] = (FT_Byte)( ( ss[0] & 0xF0 ) >> 4 );

          s += ಮೂಲ->pitch;
          t += ಗುರಿ->pitch;
        }
      }
      ನಿಲ್ಲಿಸು;


    ಸ್ಥಿತಿ FT_PIXEL_ಬಹುಲಕ_BGRA:
      {
        FT_UInt  i;


        ಗುರಿ->ಅಂಕೆ_grays = 256;

        ಗಾಗಿ ( i = ಮೂಲ->ಅಡ್ಡಸಾಲುಗಳು; i > 0; i-- )
        {
          FT_Byte*  ss = s;
          FT_Byte*  tt = t;
          FT_UInt   j;


          ಗಾಗಿ ( j = ಮೂಲ->ಅಗಲ; j > 0; j-- )
          {
            tt[0] = ft_ಬೂದು_for_premultiplied_srgb_bgra( ss );

            ss += 4;
            tt += 1;
          }

          s += ಮೂಲ->pitch;
          t += ಗುರಿ->pitch;
        }
      }
      ನಿಲ್ಲಿಸು;

    ಪೂರ್ವನಿಯೋಜಿತ:
      ;
    }

    ಹಿಂತಿರುಗಿ error;
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Bitmap_Blend( FT_Library        library,
                   ಸ್ಥಿರಾಂಕ FT_Bitmap*  ಮೂಲ_2,
                   ಸ್ಥಿರಾಂಕ FT_ಸದಿಶ   ಮೂಲ_ಸ್ಥಾನಾಂತರ,
                   FT_Bitmap*        ಗುರಿ,
                   FT_ಸದಿಶ        *atarget_ಸ್ಥಾನಾಂತರ,
                   FT_Color          color )
  {
    FT_Error   error = FT_Err_Ok;
    FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ;

    FT_Bitmap         ಮೂಲ_bitmap;
    ಸ್ಥಿರಾಂಕ FT_Bitmap*  ಮೂಲ;

    FT_Bool  free_ಮೂಲ_bitmap          = 0;
    FT_Bool  free_ಗುರಿ_bitmap_on_error = 0;

    FT_BBox  sbx, tbx, fbx;

    ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ  final_ಅಡ್ಡಸಾಲುಗಳು, final_ಅಗಲ;


    ಆದರೆ ( !library || !ಗುರಿ || !ಮೂಲ_2 || !atarget_ಸ್ಥಾನಾಂತರ )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

    ಸ್ಮರಣೆ = library->ಸ್ಮರಣೆ;

    ಆದರೆ ( !( ಗುರಿ->pixel_ಬಹುಲಕ == FT_PIXEL_ಬಹುಲಕ_NONE     ||
            ( ಗುರಿ->pixel_ಬಹುಲಕ == FT_PIXEL_ಬಹುಲಕ_BGRA &&
              ಗುರಿ->ಬಫರ್                           ) ) )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

    ಆದರೆ ( ಮೂಲ_2->pixel_ಬಹುಲಕ == FT_PIXEL_ಬಹುಲಕ_NONE )
      ಹಿಂತಿರುಗಿ FT_Err_Ok;               /* nothing to do */

    /* pitches must have the same sign */
    ಆದರೆ ( ಗುರಿ->pixel_ಬಹುಲಕ == FT_PIXEL_ಬಹುಲಕ_BGRA &&
         ( ಮೂಲ_2->pitch ^ ಗುರಿ->pitch ) < 0   )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

    ಆದರೆ ( !( ಮೂಲ_2->ಅಗಲ && ಮೂಲ_2->ಅಡ್ಡಸಾಲುಗಳು ) )
      ಹಿಂತಿರುಗಿ FT_Err_Ok;               /* nothing to do */

    /* get source bitmap dimensions assuming integer offsets */
    sbx.xMin = ( ಮೂಲ_ಸ್ಥಾನಾಂತರ.x >> 6 );
    sbx.yMin = ( ಮೂಲ_ಸ್ಥಾನಾಂತರ.y >> 6 ) - ಮೂಲ_2->ಅಡ್ಡಸಾಲುಗಳು;
    sbx.xMax = ( ಮೂಲ_ಸ್ಥಾನಾಂತರ.x >> 6 ) + ಮೂಲ_2->ಅಗಲ;
    sbx.yMax = ( ಮೂಲ_ಸ್ಥಾನಾಂತರ.y >> 6 );

    FT_TRACE5(( "FT_Bitmap_Blend:\n" ));
    FT_TRACE5(( "  source bitmap: (%ld, %ld) -- (%ld, %ld); %u x %u\n",
      sbx.xMin, sbx.yMin, sbx.xMax, sbx.yMax,
      ಮೂಲ_2->ಅಗಲ, ಮೂಲ_2->ಅಡ್ಡಸಾಲುಗಳು ));

    /* sanity check */
    ಆದರೆ ( sbx.xMin > sbx.xMax      || sbx.yMin > sbx.yMax     ||
         ಮೂಲ_2->ಅಗಲ > 0x7FFFU || ಮೂಲ_2->ಅಡ್ಡಸಾಲುಗಳು > 0x7FFFU )
    {
      FT_TRACE5(( "FT_Bitmap_Blend: source dimension overflow\n" ));
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );
    }

    ಆದರೆ ( !ಗುರಿ->ಅಗಲ || !ಗುರಿ->ಅಡ್ಡಸಾಲುಗಳು )
    {
      FT_TRACE5(( "  target bitmap: empty\n" ));

      final_ಅಗಲ = ಮೂಲ_2->ಅಗಲ;
      final_ಅಡ್ಡಸಾಲುಗಳು  = ಮೂಲ_2->ಅಡ್ಡಸಾಲುಗಳು;

      /* create new empty bitmap */
      ಗುರಿ->ಅಗಲ      = final_ಅಗಲ;
      ಗುರಿ->ಅಡ್ಡಸಾಲುಗಳು       = final_ಅಡ್ಡಸಾಲುಗಳು;
      ಗುರಿ->pixel_ಬಹುಲಕ = FT_PIXEL_ಬಹುಲಕ_BGRA;
      ಗುರಿ->pitch      = (ಪೂರ್ಣಾಂಕ)final_ಅಗಲ * 4;
      ಗುರಿ->ಅಂಕೆ_grays  = 256;

      ಆದರೆ ( FT_ALLOC_MULT( ಗುರಿ->ಬಫರ್, ಗುರಿ->ಅಡ್ಡಸಾಲುಗಳು, ಗುರಿ->pitch ) )
        ಹಿಂತಿರುಗಿ error;

      free_ಗುರಿ_bitmap_on_error = 1;

      fbx = sbx;
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
      /* get target bitmap dimensions assuming integer offsets */
      tbx.xMin = ( atarget_ಸ್ಥಾನಾಂತರ->x >> 6 );
      tbx.yMin = ( atarget_ಸ್ಥಾನಾಂತರ->y >> 6 ) - ಗುರಿ->ಅಡ್ಡಸಾಲುಗಳು;
      tbx.xMax = ( atarget_ಸ್ಥಾನಾಂತರ->x >> 6 ) + ಗುರಿ->ಅಗಲ;
      tbx.yMax = ( atarget_ಸ್ಥಾನಾಂತರ->y >> 6 );

      FT_TRACE5(( "  target bitmap: (%ld, %ld) -- (%ld, %ld); %u x %u\n",
        tbx.xMin, tbx.yMin, tbx.xMax, tbx.yMax,
        ಗುರಿ->ಅಗಲ, ಗುರಿ->ಅಡ್ಡಸಾಲುಗಳು ));

      /* sanity check */
      ಆದರೆ ( tbx.xMin > tbx.xMax || tbx.yMin > tbx.yMax )
      {
        FT_TRACE5(( "FT_Bitmap_Blend: targget dimension overflow\n" ));
        ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );
      }

      /* compute final bitmap dimensions */
      fbx.xMin = FT_MIN( sbx.xMin, tbx.xMin );
      fbx.yMin = FT_MIN( sbx.yMin, tbx.yMin );
      fbx.xMax = FT_MAX( sbx.xMax, tbx.xMax );
      fbx.yMax = FT_MAX( sbx.yMax, tbx.yMax );

      /* sanity check */
      ಆದರೆ ( fbx.xMin < -0x10000 || fbx.xMax >= 0x10000 ||
           fbx.yMin < -0x10000 || fbx.yMax >= 0x10000 )
      {
        FT_TRACE5(( "FT_Bitmap_Blend: final dimension overflow\n" ));
        ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );
      }

      final_ಅಗಲ = fbx.xMax - fbx.xMin;
      final_ಅಡ್ಡಸಾಲುಗಳು  = fbx.yMax - fbx.yMin;

      /* adjust target bitmap to enlarged size */
      ಆದರೆ ( ಗುರಿ->ಅಗಲ < final_ಅಗಲ ||
           ಗುರಿ->ಅಡ್ಡಸಾಲುಗಳು  < final_ಅಡ್ಡಸಾಲುಗಳು  )
      {
        ಪೂರ್ಣಾಂಕ  pitch, new_pitch;

        ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ*  ಬಫರ್ = ಖಾಲಿ;


        pitch = ಗುರಿ->pitch;

        ಆದರೆ ( pitch < 0 )
          pitch = -pitch;

        new_pitch = (ಪೂರ್ಣಾಂಕ)final_ಅಗಲ * 4;

        /* TODO: provide an in-buffer solution for large bitmaps */
        /*       to avoid allocation of a new buffer             */
        ಆದರೆ ( FT_ALLOC_MULT( ಬಫರ್, final_ಅಡ್ಡಸಾಲುಗಳು, new_pitch ) )
          ಹೋಗು Error;

        /* copy data to new buffer */

        /* the bitmap flow is from top to bottom, */
        /* but y is measured from bottom to top   */
        ಆದರೆ ( ಗುರಿ->pitch < 0 )
        {
          /* XXX */
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
          FT_Pos  x = tbx.xMin - fbx.xMin;
          FT_Pos  y = tbx.yMin - fbx.yMin;

          ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ*  p =
            ಗುರಿ->ಬಫರ್;
          ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ*  q =
            ಬಫರ್ +
            ( final_ಅಡ್ಡಸಾಲುಗಳು - y - ಗುರಿ->ಅಡ್ಡಸಾಲುಗಳು ) * new_pitch +
            x * 4;
          ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ*  ಎಲ್ಲೆ_p =
            p + pitch * (ಪೂರ್ಣಾಂಕ)ಗುರಿ->ಅಡ್ಡಸಾಲುಗಳು;


          ಆಗ ( p < ಎಲ್ಲೆ_p )
          {
            FT_MEM_COPY( q, p, pitch );

            p += pitch;
            q += new_pitch;
          }
        }

        FT_FREE( ಗುರಿ->ಬಫರ್ );

        ಗುರಿ->ಅಗಲ = final_ಅಗಲ;
        ಗುರಿ->ಅಡ್ಡಸಾಲುಗಳು  = final_ಅಡ್ಡಸಾಲುಗಳು;

        ಆದರೆ ( ಗುರಿ->pitch < 0 )
          ಗುರಿ->pitch = -new_pitch;
        ಇಲ್ಲದಿದ್ದರೆ
          ಗುರಿ->pitch = new_pitch;

        ಗುರಿ->ಬಫರ್ = ಬಫರ್;
      }
    }

    FT_TRACE5(( "  final bitmap: (%ld, %ld) -- (%ld, %ld); %u x %u\n",
      fbx.xMin, fbx.yMin, fbx.xMax, fbx.yMax,
      final_ಅಗಲ, final_ಅಡ್ಡಸಾಲುಗಳು ));

    /* adjust source bitmap if necessary */
    ಆದರೆ ( ಮೂಲ_2->pixel_ಬಹುಲಕ != FT_PIXEL_ಬಹುಲಕ_ಬೂದು )
    {
      FT_Bitmap_Init( &ಮೂಲ_bitmap );
      error = FT_Bitmap_Convert( library, ಮೂಲ_2, &ಮೂಲ_bitmap, 1 );
      ಆದರೆ ( error )
        ಹೋಗು Error;

      ಮೂಲ             = &ಮೂಲ_bitmap;
      free_ಮೂಲ_bitmap = 1;
    }
    ಇಲ್ಲದಿದ್ದರೆ
      ಮೂಲ = ಮೂಲ_2;

    /* do blending; the code below returns pre-multiplied channels, */
    /* similar to what FreeType gets from `CBDT' tables             */

    /* the bitmap flow is from top to bottom, */
    /* but y is measured from bottom to top   */
    ಆದರೆ ( ಗುರಿ->pitch < 0 )
    {
      /* XXX */
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
      FT_Pos  x = sbx.xMin - fbx.xMin;
      FT_Pos  y = sbx.yMin - fbx.yMin;

      ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ*  p =
        ಮೂಲ->ಬಫರ್;
      ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ*  q =
        ಗುರಿ->ಬಫರ್ +
        ( ಗುರಿ->ಅಡ್ಡಸಾಲುಗಳು - y - ಮೂಲ->ಅಡ್ಡಸಾಲುಗಳು ) * ಗುರಿ->pitch +
        x * 4;
      ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ*  ಎಲ್ಲೆ_p =
        p + ಮೂಲ->pitch * (ಪೂರ್ಣಾಂಕ)ಮೂಲ->ಅಡ್ಡಸಾಲುಗಳು;


      ಆಗ ( p < ಎಲ್ಲೆ_p )
      {
        ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ*  r       = p;
        ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ*  s       = q;
        ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ*  ಎಲ್ಲೆ_r = r + ಮೂಲ->ಅಗಲ;


        ಆಗ ( r < ಎಲ್ಲೆ_r )
        {
          ಪೂರ್ಣಾಂಕ  aa = *r++;
          ಪೂರ್ಣಾಂಕ  fa = color.alpha * aa / 255;

          ಪೂರ್ಣಾಂಕ  fb = color.blue * fa / 255;
          ಪೂರ್ಣಾಂಕ  fg = color.green * fa / 255;
          ಪೂರ್ಣಾಂಕ  fr = color.red * fa / 255;

          ಪೂರ್ಣಾಂಕ  ba2 = 255 - fa;

          ಪೂರ್ಣಾಂಕ  bb = s[0];
          ಪೂರ್ಣಾಂಕ  bg = s[1];
          ಪೂರ್ಣಾಂಕ  br = s[2];
          ಪೂರ್ಣಾಂಕ  ba = s[3];


          *s++ = (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ)( bb * ba2 / 255 + fb );
          *s++ = (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ)( bg * ba2 / 255 + fg );
          *s++ = (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ)( br * ba2 / 255 + fr );
          *s++ = (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ)( ba * ba2 / 255 + fa );
        }

        p += ಮೂಲ->pitch;
        q += ಗುರಿ->pitch;
      }
    }

    atarget_ಸ್ಥಾನಾಂತರ->x = fbx.xMin * 64;
    atarget_ಸ್ಥಾನಾಂತರ->y = fbx.yMax * 64;

  Error:
    ಆದರೆ ( error && free_ಗುರಿ_bitmap_on_error )
      FT_Bitmap_ಮುಗಿದ( library, ಗುರಿ );

    ಆದರೆ ( free_ಮೂಲ_bitmap )
      FT_Bitmap_ಮುಗಿದ( library, &ಮೂಲ_bitmap );

    ಹಿಂತಿರುಗಿ error;
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Glyph_ಸ್ಥಾನ_Own_Bitmap( FT_Glyph_ಸ್ಥಾನ  ಸ್ಥಾನ )
  {
    ಆದರೆ ( ಸ್ಥಾನ && ಸ್ಥಾನ->format == FT_GLYPH_FORMAT_BITMAP   &&
         !( ಸ್ಥಾನ->ಆಂತರಿಕ->flags & FT_GLYPH_OWN_BITMAP ) )
    {
      FT_Bitmap  bitmap;
      FT_Error   error;


      FT_Bitmap_Init( &bitmap );
      error = FT_Bitmap_Copy( ಸ್ಥಾನ->library, &ಸ್ಥಾನ->bitmap, &bitmap );
      ಆದರೆ ( error )
        ಹಿಂತಿರುಗಿ error;

      ಸ್ಥಾನ->bitmap = bitmap;
      ಸ್ಥಾನ->ಆಂತರಿಕ->flags |= FT_GLYPH_OWN_BITMAP;
    }

    ಹಿಂತಿರುಗಿ FT_Err_Ok;
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Bitmap_ಮುಗಿದ( FT_Library  library,
                  FT_Bitmap  *bitmap )
  {
    FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ;


    ಆದರೆ ( !library )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_Library_Handle );

    ಆದರೆ ( !bitmap )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

    ಸ್ಮರಣೆ = library->ಸ್ಮರಣೆ;

    FT_FREE( bitmap->ಬಫರ್ );
    *bitmap = null_bitmap;

    ಹಿಂತಿರುಗಿ FT_Err_Ok;
  }


/*ಕೊನೆ*/
