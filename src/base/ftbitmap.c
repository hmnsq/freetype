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


#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftdebug.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/ftbitmap.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/ftimage.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftobjs.h>


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ  FT_COMPONENT
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FT_COMPONENT  bitmap


  ਸਥਿਰ
  ਅਚਲ FT_Bitmap  null_bitmap = { 0, 0, 0, ਨਿਰਾ, 0, 0, 0, ਨਿਰਾ };


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( ਖਾਲੀ )
  FT_Bitmap_Init( FT_Bitmap  *abitmap )
  {
    ਜੇ ( abitmap )
      *abitmap = null_bitmap;
  }


  /* deprecated function name; retained for ABI compatibility */

  FT_EXPORT_DEF( ਖਾਲੀ )
  FT_Bitmap_New( FT_Bitmap  *abitmap )
  {
    ਜੇ ( abitmap )
      *abitmap = null_bitmap;
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Bitmap_Copy( FT_Library        library,
                  ਅਚਲ FT_Bitmap  *ਸਰੋਤ,
                  FT_Bitmap        *ਨਿਸ਼ਾਨਾ)
  {
    FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ;
    FT_Error   error  = FT_Err_Ok;
    FT_Int     pitch;
    FT_Int     flip;


    ਜੇ ( !library )
      ਵਾਪਸ FT_THROW( Invalid_Library_Handle );

    ਜੇ ( !ਸਰੋਤ || !ਨਿਸ਼ਾਨਾ )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    ਜੇ ( ਸਰੋਤ == ਨਿਸ਼ਾਨਾ )
      ਵਾਪਸ FT_Err_Ok;

    flip = ( ਸਰੋਤ->pitch < 0 && ਨਿਸ਼ਾਨਾ->pitch > 0 ) ||
           ( ਸਰੋਤ->pitch > 0 && ਨਿਸ਼ਾਨਾ->pitch < 0 );

    ਸਮ੍ਰਿਤੀ = library->ਸਮ੍ਰਿਤੀ;
    FT_FREE( ਨਿਸ਼ਾਨਾ->ਬਫਰ );

    *ਨਿਸ਼ਾਨਾ = *ਸਰੋਤ;

    ਜੇ ( flip )
      ਨਿਸ਼ਾਨਾ->pitch = -ਨਿਸ਼ਾਨਾ->pitch;

    ਜੇ ( !ਸਰੋਤ->ਬਫਰ )
      ਵਾਪਸ FT_Err_Ok;

    pitch  = ਸਰੋਤ->pitch;
    ਜੇ ( pitch < 0 )
      pitch = -pitch;

    FT_MEM_QALLOC_MULT( ਨਿਸ਼ਾਨਾ->ਬਫਰ, ਨਿਸ਼ਾਨਾ->ਪੰਕਤੀਆਂ, pitch );

    ਜੇ ( !error )
    {
      ਜੇ ( flip )
      {
        /* take care of bitmap flow */
        FT_UInt   i;
        FT_Byte*  s = ਸਰੋਤ->ਬਫਰ;
        FT_Byte*  t = ਨਿਸ਼ਾਨਾ->ਬਫਰ;


        t += (FT_ULong)pitch * ( ਨਿਸ਼ਾਨਾ->ਪੰਕਤੀਆਂ - 1 );

        ਲਈ ( i = ਨਿਸ਼ਾਨਾ->ਪੰਕਤੀਆਂ; i > 0; i-- )
        {
          FT_ARRAY_COPY( t, s, pitch );

          s += pitch;
          t -= pitch;
        }
      }
      ਨਹੀਂਤਾਂ
        FT_MEM_COPY( ਨਿਸ਼ਾਨਾ->ਬਫਰ, ਸਰੋਤ->ਬਫਰ,
                     (FT_Long)ਸਰੋਤ->ਪੰਕਤੀਆਂ * pitch );
    }

    ਵਾਪਸ error;
  }


  /* Enlarge `bitmap' horizontally and vertically by `xpixels' */
  /* and `ypixels', respectively.                              */

  ਸਥਿਰ FT_Error
  ft_bitmap_assure_ਬਫਰ( FT_ਸਮ੍ਰਿਤੀ   ਸਮ੍ਰਿਤੀ,
                           FT_Bitmap*  bitmap,
                           FT_UInt     xpixels,
                           FT_UInt     ypixels )
  {
    FT_Error        error;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    pitch;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    new_pitch;
    FT_UInt         bpp;
    FT_UInt         ਚੌੜਾਈ, ਉਚਾਈ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ*  ਬਫਰ = ਨਿਰਾ;


    ਚੌੜਾਈ  = bitmap->ਚੌੜਾਈ;
    ਉਚਾਈ = bitmap->ਪੰਕਤੀਆਂ;
    pitch  = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)FT_ABS( bitmap->pitch );

    ਚੋਣ ( bitmap->pixel_ਬਹੁਲਕ )
    {
    ਹਾਲਤ FT_PIXEL_ਬਹੁਲਕ_MONO:
      bpp       = 1;
      new_pitch = ( ਚੌੜਾਈ + xpixels + 7 ) >> 3;
      ਰੋਕੋ;
    ਹਾਲਤ FT_PIXEL_ਬਹੁਲਕ_GRAY2:
      bpp       = 2;
      new_pitch = ( ਚੌੜਾਈ + xpixels + 3 ) >> 2;
      ਰੋਕੋ;
    ਹਾਲਤ FT_PIXEL_ਬਹੁਲਕ_GRAY4:
      bpp       = 4;
      new_pitch = ( ਚੌੜਾਈ + xpixels + 1 ) >> 1;
      ਰੋਕੋ;
    ਹਾਲਤ FT_PIXEL_ਬਹੁਲਕ_ਸਲੇਟੀ:
    ਹਾਲਤ FT_PIXEL_ਬਹੁਲਕ_LCD:
    ਹਾਲਤ FT_PIXEL_ਬਹੁਲਕ_LCD_V:
      bpp       = 8;
      new_pitch = ਚੌੜਾਈ + xpixels;
      ਰੋਕੋ;
    ਮੂਲ:
      ਵਾਪਸ FT_THROW( Invalid_Glyph_Format );
    }

    /* if no need to allocate memory */
    ਜੇ ( ypixels == 0 && new_pitch <= pitch )
    {
      /* zero the padding */
      FT_UInt  ਦੋਆਧਾਰੀ_ਅੰਕ_ਚੌੜਾਈ = pitch * 8;
      FT_UInt  ਦੋਆਧਾਰੀ_ਅੰਕ_ਆਖਰੀ  = ( ਚੌੜਾਈ + xpixels ) * bpp;


      ਜੇ ( ਦੋਆਧਾਰੀ_ਅੰਕ_ਆਖਰੀ < ਦੋਆਧਾਰੀ_ਅੰਕ_ਚੌੜਾਈ )
      {
        FT_Byte*  ਸਤਰ  = bitmap->ਬਫਰ + ( ਦੋਆਧਾਰੀ_ਅੰਕ_ਆਖਰੀ >> 3 );
        FT_Byte*  ਸਿਰਾ   = bitmap->ਬਫਰ + pitch;
        FT_UInt   ਖਿਸਕਾਓ = ਦੋਆਧਾਰੀ_ਅੰਕ_ਆਖਰੀ & 7;
        FT_UInt   ਨਕਾਬ  = 0xFF00U >> ਖਿਸਕਾਓ;
        FT_UInt   ਗਿਣਤੀ_2 = ਉਚਾਈ;


        ਲਈ ( ; ਗਿਣਤੀ_2 > 0; ਗਿਣਤੀ_2--, ਸਤਰ += pitch, ਸਿਰਾ += pitch )
        {
          FT_Byte*  ਲਿਖ = ਸਤਰ;


          ਜੇ ( ਖਿਸਕਾਓ > 0 )
          {
            ਲਿਖ[0] = (FT_Byte)( ਲਿਖ[0] & ਨਕਾਬ );
            ਲਿਖ++;
          }
          ਜੇ ( ਲਿਖ < ਸਿਰਾ )
            FT_MEM_ZERO( ਲਿਖ, ਸਿਰਾ - ਲਿਖ );
        }
      }

      ਵਾਪਸ FT_Err_Ok;
    }

    /* otherwise allocate new buffer */
    ਜੇ ( FT_QALLOC_MULT( ਬਫਰ, bitmap->ਪੰਕਤੀਆਂ + ypixels, new_pitch ) )
      ਵਾਪਸ error;

    /* new rows get added at the top of the bitmap, */
    /* thus take care of the flow direction         */
    ਜੇ ( bitmap->pitch > 0 )
    {
      FT_UInt  len = ( ਚੌੜਾਈ * bpp + 7 ) >> 3;

      ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ*  ਵਿੱਚ  = bitmap->ਬਫਰ;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ*  ਬਾਹਰਲਾ = ਬਫਰ;

      ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ*  ਹੱਦਬੰਦੀ = bitmap->ਬਫਰ + pitch * bitmap->ਪੰਕਤੀਆਂ;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    delta = new_pitch - len;


      FT_MEM_ZERO( ਬਾਹਰਲਾ, new_pitch * ypixels );
      ਬਾਹਰਲਾ += new_pitch * ypixels;

      ਜਦੋਂ ( ਵਿੱਚ < ਹੱਦਬੰਦੀ )
      {
        FT_MEM_COPY( ਬਾਹਰਲਾ, ਵਿੱਚ, len );
        ਵਿੱਚ  += pitch;
        ਬਾਹਰਲਾ += len;

        /* we use FT_QALLOC_MULT, which doesn't zero out the buffer;      */
        /* consequently, we have to manually zero out the remaining bytes */
        FT_MEM_ZERO( ਬਾਹਰਲਾ, delta );
        ਬਾਹਰਲਾ += delta;
      }
    }
    ਨਹੀਂਤਾਂ
    {
      FT_UInt  len = ( ਚੌੜਾਈ * bpp + 7 ) >> 3;

      ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ*  ਵਿੱਚ  = bitmap->ਬਫਰ;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ*  ਬਾਹਰਲਾ = ਬਫਰ;

      ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ*  ਹੱਦਬੰਦੀ = bitmap->ਬਫਰ + pitch * bitmap->ਪੰਕਤੀਆਂ;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    delta = new_pitch - len;


      ਜਦੋਂ ( ਵਿੱਚ < ਹੱਦਬੰਦੀ )
      {
        FT_MEM_COPY( ਬਾਹਰਲਾ, ਵਿੱਚ, len );
        ਵਿੱਚ  += pitch;
        ਬਾਹਰਲਾ += len;

        FT_MEM_ZERO( ਬਾਹਰਲਾ, delta );
        ਬਾਹਰਲਾ += delta;
      }

      FT_MEM_ZERO( ਬਾਹਰਲਾ, new_pitch * ypixels );
    }

    FT_FREE( bitmap->ਬਫਰ );
    bitmap->ਬਫਰ = ਬਫਰ;

    /* set pitch only, width and height are left untouched */
    ਜੇ ( bitmap->pitch < 0 )
      bitmap->pitch = -(ਪੂਰਨ)new_pitch;
    ਨਹੀਂਤਾਂ
      bitmap->pitch = (ਪੂਰਨ)new_pitch;

    ਵਾਪਸ FT_Err_Ok;
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Bitmap_Embolden( FT_Library  library,
                      FT_Bitmap*  bitmap,
                      FT_Pos      xStrength,
                      FT_Pos      yStrength )
  {
    FT_Error        error;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ*  p;
    FT_Int          i, x, pitch;
    FT_UInt         y;
    FT_Int          xstr, ystr;


    ਜੇ ( !library )
      ਵਾਪਸ FT_THROW( Invalid_Library_Handle );

    ਜੇ ( !bitmap || !bitmap->ਬਫਰ )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    ਜੇ ( ( ( FT_PIX_ਗੇੜਾ( xStrength ) >> 6 ) > FT_INT_MAX ) ||
         ( ( FT_PIX_ਗੇੜਾ( yStrength ) >> 6 ) > FT_INT_MAX ) )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    xstr = (FT_Int)FT_PIX_ਗੇੜਾ( xStrength ) >> 6;
    ystr = (FT_Int)FT_PIX_ਗੇੜਾ( yStrength ) >> 6;

    ਜੇ ( xstr == 0 && ystr == 0 )
      ਵਾਪਸ FT_Err_Ok;
    ਨਹੀਂਤਾਂ ਜੇ ( xstr < 0 || ystr < 0 )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    ਚੋਣ ( bitmap->pixel_ਬਹੁਲਕ )
    {
    ਹਾਲਤ FT_PIXEL_ਬਹੁਲਕ_GRAY2:
    ਹਾਲਤ FT_PIXEL_ਬਹੁਲਕ_GRAY4:
      {
        FT_Bitmap  tmp;


        /* convert to 8bpp */
        FT_Bitmap_Init( &tmp );
        error = FT_Bitmap_Convert( library, bitmap, &tmp, 1 );
        ਜੇ ( error )
          ਵਾਪਸ error;

        FT_Bitmap_ਪੂਰਾ( library, bitmap );
        *bitmap = tmp;
      }
      ਰੋਕੋ;

    ਹਾਲਤ FT_PIXEL_ਬਹੁਲਕ_MONO:
      ਜੇ ( xstr > 8 )
        xstr = 8;
      ਰੋਕੋ;

    ਹਾਲਤ FT_PIXEL_ਬਹੁਲਕ_LCD:
      xstr *= 3;
      ਰੋਕੋ;

    ਹਾਲਤ FT_PIXEL_ਬਹੁਲਕ_LCD_V:
      ystr *= 3;
      ਰੋਕੋ;

    ਹਾਲਤ FT_PIXEL_ਬਹੁਲਕ_BGRA:
      /* We don't embolden color glyphs. */
      ਵਾਪਸ FT_Err_Ok;
    }

    error = ft_bitmap_assure_ਬਫਰ( library->ਸਮ੍ਰਿਤੀ, bitmap,
                                     (FT_UInt)xstr, (FT_UInt)ystr );
    ਜੇ ( error )
      ਵਾਪਸ error;

    /* take care of bitmap flow */
    pitch = bitmap->pitch;
    ਜੇ ( pitch > 0 )
      p = bitmap->ਬਫਰ + pitch * ystr;
    ਨਹੀਂਤਾਂ
    {
      pitch = -pitch;
      p = bitmap->ਬਫਰ + (FT_UInt)pitch * ( bitmap->ਪੰਕਤੀਆਂ - 1 );
    }

    /* for each row */
    ਲਈ ( y = 0; y < bitmap->ਪੰਕਤੀਆਂ; y++ )
    {
      /*
       * Horizontally:
       *
       * From the last pixel on, make each pixel or'ed with the
       * `xstr' pixels before it.
       */
      ਲਈ ( x = pitch - 1; x >= 0; x-- )
      {
        ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ  tmp;


        tmp = p[x];
        ਲਈ ( i = 1; i <= xstr; i++ )
        {
          ਜੇ ( bitmap->pixel_ਬਹੁਲਕ == FT_PIXEL_ਬਹੁਲਕ_MONO )
          {
            p[x] |= tmp >> i;

            /* the maximum value of 8 for `xstr' comes from here */
            ਜੇ ( x > 0 )
              p[x] |= p[x - 1] << ( 8 - i );

#ਜੇ 0
            ਜੇ ( p[x] == 0xFF )
              ਰੋਕੋ;
#ਜੇ_ਅੰਤ
          }
          ਨਹੀਂਤਾਂ
          {
            ਜੇ ( x - i >= 0 )
            {
              ਜੇ ( p[x] + p[x - i] > bitmap->ਅੰਕ_grays - 1 )
              {
                p[x] = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ)( bitmap->ਅੰਕ_grays - 1 );
                ਰੋਕੋ;
              }
              ਨਹੀਂਤਾਂ
              {
                p[x] = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ)( p[x] + p[x - i] );
                ਜੇ ( p[x] == bitmap->ਅੰਕ_grays - 1 )
                  ਰੋਕੋ;
              }
            }
            ਨਹੀਂਤਾਂ
              ਰੋਕੋ;
          }
        }
      }

      /*
       * Vertically:
       *
       * Make the above `ystr' rows or'ed with it.
       */
      ਲਈ ( x = 1; x <= ystr; x++ )
      {
        ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ*  q;


        q = p - bitmap->pitch * x;
        ਲਈ ( i = 0; i < pitch; i++ )
          q[i] |= p[i];
      }

      p += bitmap->pitch;
    }

    bitmap->ਚੌੜਾਈ += (FT_UInt)xstr;
    bitmap->ਪੰਕਤੀਆਂ += (FT_UInt)ystr;

    ਵਾਪਸ FT_Err_Ok;
  }


  ਸਥਿਰ FT_Byte
  ft_ਸਲੇਟੀ_for_premultiplied_srgb_bgra( ਅਚਲ FT_Byte*  bgra )
  {
    FT_UInt  a = bgra[3];
    FT_UInt  l;


    /* Short-circuit transparent color to avoid division by zero. */
    ਜੇ ( !a )
      ਵਾਪਸ 0;

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

    ਵਾਪਸ (FT_Byte)( a - l / a );
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Bitmap_Convert( FT_Library        library,
                     ਅਚਲ FT_Bitmap  *ਸਰੋਤ,
                     FT_Bitmap        *ਨਿਸ਼ਾਨਾ,
                     FT_Int            alignment )
  {
    FT_Error   error = FT_Err_Ok;
    FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ;

    FT_Byte*  s;
    FT_Byte*  t;


    ਜੇ ( !library )
      ਵਾਪਸ FT_THROW( Invalid_Library_Handle );

    ਜੇ ( !ਸਰੋਤ || !ਨਿਸ਼ਾਨਾ )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    ਸਮ੍ਰਿਤੀ = library->ਸਮ੍ਰਿਤੀ;

    ਚੋਣ ( ਸਰੋਤ->pixel_ਬਹੁਲਕ )
    {
    ਹਾਲਤ FT_PIXEL_ਬਹੁਲਕ_MONO:
    ਹਾਲਤ FT_PIXEL_ਬਹੁਲਕ_ਸਲੇਟੀ:
    ਹਾਲਤ FT_PIXEL_ਬਹੁਲਕ_GRAY2:
    ਹਾਲਤ FT_PIXEL_ਬਹੁਲਕ_GRAY4:
    ਹਾਲਤ FT_PIXEL_ਬਹੁਲਕ_LCD:
    ਹਾਲਤ FT_PIXEL_ਬਹੁਲਕ_LCD_V:
    ਹਾਲਤ FT_PIXEL_ਬਹੁਲਕ_BGRA:
      {
        FT_Int  ਚੌੜਾਈ = (FT_Int)ਸਰੋਤ->ਚੌੜਾਈ;
        FT_Int  neg   = ( ਨਿਸ਼ਾਨਾ->pitch == 0 && ਸਰੋਤ->pitch < 0 ) ||
                          ਨਿਸ਼ਾਨਾ->pitch  < 0;


        FT_Bitmap_ਪੂਰਾ( library, ਨਿਸ਼ਾਨਾ );

        ਨਿਸ਼ਾਨਾ->pixel_ਬਹੁਲਕ = FT_PIXEL_ਬਹੁਲਕ_ਸਲੇਟੀ;
        ਨਿਸ਼ਾਨਾ->ਪੰਕਤੀਆਂ       = ਸਰੋਤ->ਪੰਕਤੀਆਂ;
        ਨਿਸ਼ਾਨਾ->ਚੌੜਾਈ      = ਸਰੋਤ->ਚੌੜਾਈ;

        ਜੇ ( alignment )
        {
          FT_Int  rem = ਚੌੜਾਈ % alignment;


          ਜੇ ( rem )
            ਚੌੜਾਈ = alignment > 0 ? ਚੌੜਾਈ - rem + alignment
                                  : ਚੌੜਾਈ - rem - alignment;
        }

        ਜੇ ( FT_QALLOC_MULT( ਨਿਸ਼ਾਨਾ->ਬਫਰ, ਨਿਸ਼ਾਨਾ->ਪੰਕਤੀਆਂ, ਚੌੜਾਈ ) )
          ਵਾਪਸ error;

        ਨਿਸ਼ਾਨਾ->pitch = neg ? -ਚੌੜਾਈ : ਚੌੜਾਈ;
      }
      ਰੋਕੋ;

    ਮੂਲ:
      error = FT_THROW( Invalid_ਦਲੀਲ );
    }

    s = ਸਰੋਤ->ਬਫਰ;
    t = ਨਿਸ਼ਾਨਾ->ਬਫਰ;

    /* take care of bitmap flow */
    ਜੇ ( ਸਰੋਤ->pitch < 0 )
      s -= ਸਰੋਤ->pitch * (FT_Int)( ਸਰੋਤ->ਪੰਕਤੀਆਂ - 1 );
    ਜੇ ( ਨਿਸ਼ਾਨਾ->pitch < 0 )
      t -= ਨਿਸ਼ਾਨਾ->pitch * (FT_Int)( ਨਿਸ਼ਾਨਾ->ਪੰਕਤੀਆਂ - 1 );

    ਚੋਣ ( ਸਰੋਤ->pixel_ਬਹੁਲਕ )
    {
    ਹਾਲਤ FT_PIXEL_ਬਹੁਲਕ_MONO:
      {
        FT_UInt  i;


        ਨਿਸ਼ਾਨਾ->ਅੰਕ_grays = 2;

        ਲਈ ( i = ਸਰੋਤ->ਪੰਕਤੀਆਂ; i > 0; i-- )
        {
          FT_Byte*  ss = s;
          FT_Byte*  tt = t;
          FT_UInt   j;


          /* get the full bytes */
          ਲਈ ( j = ਸਰੋਤ->ਚੌੜਾਈ >> 3; j > 0; j-- )
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
          j = ਸਰੋਤ->ਚੌੜਾਈ & 7;
          ਜੇ ( j > 0 )
          {
            FT_Int  val = *ss;


            ਲਈ ( ; j > 0; j-- )
            {
              tt[0] = (FT_Byte)( ( val & 0x80 ) >> 7);
              val <<= 1;
              tt   += 1;
            }
          }

          s += ਸਰੋਤ->pitch;
          t += ਨਿਸ਼ਾਨਾ->pitch;
        }
      }
      ਰੋਕੋ;


    ਹਾਲਤ FT_PIXEL_ਬਹੁਲਕ_ਸਲੇਟੀ:
    ਹਾਲਤ FT_PIXEL_ਬਹੁਲਕ_LCD:
    ਹਾਲਤ FT_PIXEL_ਬਹੁਲਕ_LCD_V:
      {
        FT_UInt  ਚੌੜਾਈ = ਸਰੋਤ->ਚੌੜਾਈ;
        FT_UInt  i;


        ਨਿਸ਼ਾਨਾ->ਅੰਕ_grays = 256;

        ਲਈ ( i = ਸਰੋਤ->ਪੰਕਤੀਆਂ; i > 0; i-- )
        {
          FT_ARRAY_COPY( t, s, ਚੌੜਾਈ );

          s += ਸਰੋਤ->pitch;
          t += ਨਿਸ਼ਾਨਾ->pitch;
        }
      }
      ਰੋਕੋ;


    ਹਾਲਤ FT_PIXEL_ਬਹੁਲਕ_GRAY2:
      {
        FT_UInt  i;


        ਨਿਸ਼ਾਨਾ->ਅੰਕ_grays = 4;

        ਲਈ ( i = ਸਰੋਤ->ਪੰਕਤੀਆਂ; i > 0; i-- )
        {
          FT_Byte*  ss = s;
          FT_Byte*  tt = t;
          FT_UInt   j;


          /* get the full bytes */
          ਲਈ ( j = ਸਰੋਤ->ਚੌੜਾਈ >> 2; j > 0; j-- )
          {
            FT_Int  val = ss[0];


            tt[0] = (FT_Byte)( ( val & 0xC0 ) >> 6 );
            tt[1] = (FT_Byte)( ( val & 0x30 ) >> 4 );
            tt[2] = (FT_Byte)( ( val & 0x0C ) >> 2 );
            tt[3] = (FT_Byte)( ( val & 0x03 ) );

            ss += 1;
            tt += 4;
          }

          j = ਸਰੋਤ->ਚੌੜਾਈ & 3;
          ਜੇ ( j > 0 )
          {
            FT_Int  val = ss[0];


            ਲਈ ( ; j > 0; j-- )
            {
              tt[0]  = (FT_Byte)( ( val & 0xC0 ) >> 6 );
              val  <<= 2;
              tt    += 1;
            }
          }

          s += ਸਰੋਤ->pitch;
          t += ਨਿਸ਼ਾਨਾ->pitch;
        }
      }
      ਰੋਕੋ;


    ਹਾਲਤ FT_PIXEL_ਬਹੁਲਕ_GRAY4:
      {
        FT_UInt  i;


        ਨਿਸ਼ਾਨਾ->ਅੰਕ_grays = 16;

        ਲਈ ( i = ਸਰੋਤ->ਪੰਕਤੀਆਂ; i > 0; i-- )
        {
          FT_Byte*  ss = s;
          FT_Byte*  tt = t;
          FT_UInt   j;


          /* get the full bytes */
          ਲਈ ( j = ਸਰੋਤ->ਚੌੜਾਈ >> 1; j > 0; j-- )
          {
            FT_Int  val = ss[0];


            tt[0] = (FT_Byte)( ( val & 0xF0 ) >> 4 );
            tt[1] = (FT_Byte)( ( val & 0x0F ) );

            ss += 1;
            tt += 2;
          }

          ਜੇ ( ਸਰੋਤ->ਚੌੜਾਈ & 1 )
            tt[0] = (FT_Byte)( ( ss[0] & 0xF0 ) >> 4 );

          s += ਸਰੋਤ->pitch;
          t += ਨਿਸ਼ਾਨਾ->pitch;
        }
      }
      ਰੋਕੋ;


    ਹਾਲਤ FT_PIXEL_ਬਹੁਲਕ_BGRA:
      {
        FT_UInt  i;


        ਨਿਸ਼ਾਨਾ->ਅੰਕ_grays = 256;

        ਲਈ ( i = ਸਰੋਤ->ਪੰਕਤੀਆਂ; i > 0; i-- )
        {
          FT_Byte*  ss = s;
          FT_Byte*  tt = t;
          FT_UInt   j;


          ਲਈ ( j = ਸਰੋਤ->ਚੌੜਾਈ; j > 0; j-- )
          {
            tt[0] = ft_ਸਲੇਟੀ_for_premultiplied_srgb_bgra( ss );

            ss += 4;
            tt += 1;
          }

          s += ਸਰੋਤ->pitch;
          t += ਨਿਸ਼ਾਨਾ->pitch;
        }
      }
      ਰੋਕੋ;

    ਮੂਲ:
      ;
    }

    ਵਾਪਸ error;
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Bitmap_Blend( FT_Library        library,
                   ਅਚਲ FT_Bitmap*  ਸਰੋਤ_2,
                   ਅਚਲ FT_ਸਦਿਸ਼   ਸਰੋਤ_ਵਿੱਥ,
                   FT_Bitmap*        ਨਿਸ਼ਾਨਾ,
                   FT_ਸਦਿਸ਼        *atarget_ਵਿੱਥ,
                   FT_Color          color )
  {
    FT_Error   error = FT_Err_Ok;
    FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ;

    FT_Bitmap         ਸਰੋਤ_bitmap;
    ਅਚਲ FT_Bitmap*  ਸਰੋਤ;

    FT_Bool  free_ਸਰੋਤ_bitmap          = 0;
    FT_Bool  free_ਨਿਸ਼ਾਨਾ_bitmap_on_error = 0;

    FT_BBox  sbx, tbx, fbx;

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  final_ਪੰਕਤੀਆਂ, final_ਚੌੜਾਈ;


    ਜੇ ( !library || !ਨਿਸ਼ਾਨਾ || !ਸਰੋਤ_2 || !atarget_ਵਿੱਥ )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    ਸਮ੍ਰਿਤੀ = library->ਸਮ੍ਰਿਤੀ;

    ਜੇ ( !( ਨਿਸ਼ਾਨਾ->pixel_ਬਹੁਲਕ == FT_PIXEL_ਬਹੁਲਕ_NONE     ||
            ( ਨਿਸ਼ਾਨਾ->pixel_ਬਹੁਲਕ == FT_PIXEL_ਬਹੁਲਕ_BGRA &&
              ਨਿਸ਼ਾਨਾ->ਬਫਰ                           ) ) )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    ਜੇ ( ਸਰੋਤ_2->pixel_ਬਹੁਲਕ == FT_PIXEL_ਬਹੁਲਕ_NONE )
      ਵਾਪਸ FT_Err_Ok;               /* nothing to do */

    /* pitches must have the same sign */
    ਜੇ ( ਨਿਸ਼ਾਨਾ->pixel_ਬਹੁਲਕ == FT_PIXEL_ਬਹੁਲਕ_BGRA &&
         ( ਸਰੋਤ_2->pitch ^ ਨਿਸ਼ਾਨਾ->pitch ) < 0   )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    ਜੇ ( !( ਸਰੋਤ_2->ਚੌੜਾਈ && ਸਰੋਤ_2->ਪੰਕਤੀਆਂ ) )
      ਵਾਪਸ FT_Err_Ok;               /* nothing to do */

    /* get source bitmap dimensions assuming integer offsets */
    sbx.xMin = ( ਸਰੋਤ_ਵਿੱਥ.x >> 6 );
    sbx.yMin = ( ਸਰੋਤ_ਵਿੱਥ.y >> 6 ) - ਸਰੋਤ_2->ਪੰਕਤੀਆਂ;
    sbx.xMax = ( ਸਰੋਤ_ਵਿੱਥ.x >> 6 ) + ਸਰੋਤ_2->ਚੌੜਾਈ;
    sbx.yMax = ( ਸਰੋਤ_ਵਿੱਥ.y >> 6 );

    FT_TRACE5(( "FT_Bitmap_Blend:\n" ));
    FT_TRACE5(( "  source bitmap: (%ld, %ld) -- (%ld, %ld); %u x %u\n",
      sbx.xMin, sbx.yMin, sbx.xMax, sbx.yMax,
      ਸਰੋਤ_2->ਚੌੜਾਈ, ਸਰੋਤ_2->ਪੰਕਤੀਆਂ ));

    /* sanity check */
    ਜੇ ( sbx.xMin > sbx.xMax      || sbx.yMin > sbx.yMax     ||
         ਸਰੋਤ_2->ਚੌੜਾਈ > 0x7FFFU || ਸਰੋਤ_2->ਪੰਕਤੀਆਂ > 0x7FFFU )
    {
      FT_TRACE5(( "FT_Bitmap_Blend: source dimension overflow\n" ));
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );
    }

    ਜੇ ( !ਨਿਸ਼ਾਨਾ->ਚੌੜਾਈ || !ਨਿਸ਼ਾਨਾ->ਪੰਕਤੀਆਂ )
    {
      FT_TRACE5(( "  target bitmap: empty\n" ));

      final_ਚੌੜਾਈ = ਸਰੋਤ_2->ਚੌੜਾਈ;
      final_ਪੰਕਤੀਆਂ  = ਸਰੋਤ_2->ਪੰਕਤੀਆਂ;

      /* create new empty bitmap */
      ਨਿਸ਼ਾਨਾ->ਚੌੜਾਈ      = final_ਚੌੜਾਈ;
      ਨਿਸ਼ਾਨਾ->ਪੰਕਤੀਆਂ       = final_ਪੰਕਤੀਆਂ;
      ਨਿਸ਼ਾਨਾ->pixel_ਬਹੁਲਕ = FT_PIXEL_ਬਹੁਲਕ_BGRA;
      ਨਿਸ਼ਾਨਾ->pitch      = (ਪੂਰਨ)final_ਚੌੜਾਈ * 4;
      ਨਿਸ਼ਾਨਾ->ਅੰਕ_grays  = 256;

      ਜੇ ( FT_ALLOC_MULT( ਨਿਸ਼ਾਨਾ->ਬਫਰ, ਨਿਸ਼ਾਨਾ->ਪੰਕਤੀਆਂ, ਨਿਸ਼ਾਨਾ->pitch ) )
        ਵਾਪਸ error;

      free_ਨਿਸ਼ਾਨਾ_bitmap_on_error = 1;

      fbx = sbx;
    }
    ਨਹੀਂਤਾਂ
    {
      /* get target bitmap dimensions assuming integer offsets */
      tbx.xMin = ( atarget_ਵਿੱਥ->x >> 6 );
      tbx.yMin = ( atarget_ਵਿੱਥ->y >> 6 ) - ਨਿਸ਼ਾਨਾ->ਪੰਕਤੀਆਂ;
      tbx.xMax = ( atarget_ਵਿੱਥ->x >> 6 ) + ਨਿਸ਼ਾਨਾ->ਚੌੜਾਈ;
      tbx.yMax = ( atarget_ਵਿੱਥ->y >> 6 );

      FT_TRACE5(( "  target bitmap: (%ld, %ld) -- (%ld, %ld); %u x %u\n",
        tbx.xMin, tbx.yMin, tbx.xMax, tbx.yMax,
        ਨਿਸ਼ਾਨਾ->ਚੌੜਾਈ, ਨਿਸ਼ਾਨਾ->ਪੰਕਤੀਆਂ ));

      /* sanity check */
      ਜੇ ( tbx.xMin > tbx.xMax || tbx.yMin > tbx.yMax )
      {
        FT_TRACE5(( "FT_Bitmap_Blend: targget dimension overflow\n" ));
        ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );
      }

      /* compute final bitmap dimensions */
      fbx.xMin = FT_MIN( sbx.xMin, tbx.xMin );
      fbx.yMin = FT_MIN( sbx.yMin, tbx.yMin );
      fbx.xMax = FT_MAX( sbx.xMax, tbx.xMax );
      fbx.yMax = FT_MAX( sbx.yMax, tbx.yMax );

      /* sanity check */
      ਜੇ ( fbx.xMin < -0x10000 || fbx.xMax >= 0x10000 ||
           fbx.yMin < -0x10000 || fbx.yMax >= 0x10000 )
      {
        FT_TRACE5(( "FT_Bitmap_Blend: final dimension overflow\n" ));
        ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );
      }

      final_ਚੌੜਾਈ = fbx.xMax - fbx.xMin;
      final_ਪੰਕਤੀਆਂ  = fbx.yMax - fbx.yMin;

      /* adjust target bitmap to enlarged size */
      ਜੇ ( ਨਿਸ਼ਾਨਾ->ਚੌੜਾਈ < final_ਚੌੜਾਈ ||
           ਨਿਸ਼ਾਨਾ->ਪੰਕਤੀਆਂ  < final_ਪੰਕਤੀਆਂ  )
      {
        ਪੂਰਨ  pitch, new_pitch;

        ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ*  ਬਫਰ = ਨਿਰਾ;


        pitch = ਨਿਸ਼ਾਨਾ->pitch;

        ਜੇ ( pitch < 0 )
          pitch = -pitch;

        new_pitch = (ਪੂਰਨ)final_ਚੌੜਾਈ * 4;

        /* TODO: provide an in-buffer solution for large bitmaps */
        /*       to avoid allocation of a new buffer             */
        ਜੇ ( FT_ALLOC_MULT( ਬਫਰ, final_ਪੰਕਤੀਆਂ, new_pitch ) )
          ਜਾਓ Error;

        /* copy data to new buffer */

        /* the bitmap flow is from top to bottom, */
        /* but y is measured from bottom to top   */
        ਜੇ ( ਨਿਸ਼ਾਨਾ->pitch < 0 )
        {
          /* XXX */
        }
        ਨਹੀਂਤਾਂ
        {
          FT_Pos  x = tbx.xMin - fbx.xMin;
          FT_Pos  y = tbx.yMin - fbx.yMin;

          ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ*  p =
            ਨਿਸ਼ਾਨਾ->ਬਫਰ;
          ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ*  q =
            ਬਫਰ +
            ( final_ਪੰਕਤੀਆਂ - y - ਨਿਸ਼ਾਨਾ->ਪੰਕਤੀਆਂ ) * new_pitch +
            x * 4;
          ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ*  ਹੱਦਬੰਦੀ_p =
            p + pitch * (ਪੂਰਨ)ਨਿਸ਼ਾਨਾ->ਪੰਕਤੀਆਂ;


          ਜਦੋਂ ( p < ਹੱਦਬੰਦੀ_p )
          {
            FT_MEM_COPY( q, p, pitch );

            p += pitch;
            q += new_pitch;
          }
        }

        FT_FREE( ਨਿਸ਼ਾਨਾ->ਬਫਰ );

        ਨਿਸ਼ਾਨਾ->ਚੌੜਾਈ = final_ਚੌੜਾਈ;
        ਨਿਸ਼ਾਨਾ->ਪੰਕਤੀਆਂ  = final_ਪੰਕਤੀਆਂ;

        ਜੇ ( ਨਿਸ਼ਾਨਾ->pitch < 0 )
          ਨਿਸ਼ਾਨਾ->pitch = -new_pitch;
        ਨਹੀਂਤਾਂ
          ਨਿਸ਼ਾਨਾ->pitch = new_pitch;

        ਨਿਸ਼ਾਨਾ->ਬਫਰ = ਬਫਰ;
      }
    }

    FT_TRACE5(( "  final bitmap: (%ld, %ld) -- (%ld, %ld); %u x %u\n",
      fbx.xMin, fbx.yMin, fbx.xMax, fbx.yMax,
      final_ਚੌੜਾਈ, final_ਪੰਕਤੀਆਂ ));

    /* adjust source bitmap if necessary */
    ਜੇ ( ਸਰੋਤ_2->pixel_ਬਹੁਲਕ != FT_PIXEL_ਬਹੁਲਕ_ਸਲੇਟੀ )
    {
      FT_Bitmap_Init( &ਸਰੋਤ_bitmap );
      error = FT_Bitmap_Convert( library, ਸਰੋਤ_2, &ਸਰੋਤ_bitmap, 1 );
      ਜੇ ( error )
        ਜਾਓ Error;

      ਸਰੋਤ             = &ਸਰੋਤ_bitmap;
      free_ਸਰੋਤ_bitmap = 1;
    }
    ਨਹੀਂਤਾਂ
      ਸਰੋਤ = ਸਰੋਤ_2;

    /* do blending; the code below returns pre-multiplied channels, */
    /* similar to what FreeType gets from `CBDT' tables             */

    /* the bitmap flow is from top to bottom, */
    /* but y is measured from bottom to top   */
    ਜੇ ( ਨਿਸ਼ਾਨਾ->pitch < 0 )
    {
      /* XXX */
    }
    ਨਹੀਂਤਾਂ
    {
      FT_Pos  x = sbx.xMin - fbx.xMin;
      FT_Pos  y = sbx.yMin - fbx.yMin;

      ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ*  p =
        ਸਰੋਤ->ਬਫਰ;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ*  q =
        ਨਿਸ਼ਾਨਾ->ਬਫਰ +
        ( ਨਿਸ਼ਾਨਾ->ਪੰਕਤੀਆਂ - y - ਸਰੋਤ->ਪੰਕਤੀਆਂ ) * ਨਿਸ਼ਾਨਾ->pitch +
        x * 4;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ*  ਹੱਦਬੰਦੀ_p =
        p + ਸਰੋਤ->pitch * (ਪੂਰਨ)ਸਰੋਤ->ਪੰਕਤੀਆਂ;


      ਜਦੋਂ ( p < ਹੱਦਬੰਦੀ_p )
      {
        ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ*  r       = p;
        ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ*  s       = q;
        ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ*  ਹੱਦਬੰਦੀ_r = r + ਸਰੋਤ->ਚੌੜਾਈ;


        ਜਦੋਂ ( r < ਹੱਦਬੰਦੀ_r )
        {
          ਪੂਰਨ  aa = *r++;
          ਪੂਰਨ  fa = color.alpha * aa / 255;

          ਪੂਰਨ  fb = color.blue * fa / 255;
          ਪੂਰਨ  fg = color.green * fa / 255;
          ਪੂਰਨ  fr = color.red * fa / 255;

          ਪੂਰਨ  ba2 = 255 - fa;

          ਪੂਰਨ  bb = s[0];
          ਪੂਰਨ  bg = s[1];
          ਪੂਰਨ  br = s[2];
          ਪੂਰਨ  ba = s[3];


          *s++ = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ)( bb * ba2 / 255 + fb );
          *s++ = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ)( bg * ba2 / 255 + fg );
          *s++ = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ)( br * ba2 / 255 + fr );
          *s++ = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ)( ba * ba2 / 255 + fa );
        }

        p += ਸਰੋਤ->pitch;
        q += ਨਿਸ਼ਾਨਾ->pitch;
      }
    }

    atarget_ਵਿੱਥ->x = fbx.xMin * 64;
    atarget_ਵਿੱਥ->y = fbx.yMax * 64;

  Error:
    ਜੇ ( error && free_ਨਿਸ਼ਾਨਾ_bitmap_on_error )
      FT_Bitmap_ਪੂਰਾ( library, ਨਿਸ਼ਾਨਾ );

    ਜੇ ( free_ਸਰੋਤ_bitmap )
      FT_Bitmap_ਪੂਰਾ( library, &ਸਰੋਤ_bitmap );

    ਵਾਪਸ error;
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Glyph_ਥਾਂ_Own_Bitmap( FT_Glyph_ਥਾਂ  ਥਾਂ )
  {
    ਜੇ ( ਥਾਂ && ਥਾਂ->format == FT_GLYPH_FORMAT_BITMAP   &&
         !( ਥਾਂ->ਅੰਦਰੂਨੀ->flags & FT_GLYPH_OWN_BITMAP ) )
    {
      FT_Bitmap  bitmap;
      FT_Error   error;


      FT_Bitmap_Init( &bitmap );
      error = FT_Bitmap_Copy( ਥਾਂ->library, &ਥਾਂ->bitmap, &bitmap );
      ਜੇ ( error )
        ਵਾਪਸ error;

      ਥਾਂ->bitmap = bitmap;
      ਥਾਂ->ਅੰਦਰੂਨੀ->flags |= FT_GLYPH_OWN_BITMAP;
    }

    ਵਾਪਸ FT_Err_Ok;
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Bitmap_ਪੂਰਾ( FT_Library  library,
                  FT_Bitmap  *bitmap )
  {
    FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ;


    ਜੇ ( !library )
      ਵਾਪਸ FT_THROW( Invalid_Library_Handle );

    ਜੇ ( !bitmap )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    ਸਮ੍ਰਿਤੀ = library->ਸਮ੍ਰਿਤੀ;

    FT_FREE( bitmap->ਬਫਰ );
    *bitmap = null_bitmap;

    ਵਾਪਸ FT_Err_Ok;
  }


/*ਸਿਰਾ*/
