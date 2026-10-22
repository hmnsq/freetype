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
#undef  FT_COMPONENT
#define FT_COMPONENT  bitmap


  ಸ್ಥಿರ
  ಧ್ರುವ FT_Bitmap  null_bitmap = { 0, 0, 0, ರಿಕ್ತ, 0, 0, 0, ರಿಕ್ತ };


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( ಶೂನ್ಯ )
  FT_Bitmap_Init( FT_Bitmap  *abitmap )
  {
    ಯದಿ ( abitmap )
      *abitmap = null_bitmap;
  }


  /* deprecated function name; retained for ABI compatibility */

  FT_EXPORT_DEF( ಶೂನ್ಯ )
  FT_Bitmap_New( FT_Bitmap  *abitmap )
  {
    ಯದಿ ( abitmap )
      *abitmap = null_bitmap;
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Bitmap_Copy( FT_Library        library,
                  ಧ್ರುವ FT_Bitmap  *source,
                  FT_Bitmap        *target)
  {
    FT_Memory  memory;
    FT_Error   error  = FT_Err_Ok;
    FT_Int     pitch;
    FT_Int     flip;


    ಯದಿ ( !library )
      ಪ್ರತ್ಯಾಗಮ FT_THROW( Invalid_Library_Handle );

    ಯದಿ ( !source || !target )
      ಪ್ರತ್ಯಾಗಮ FT_THROW( Invalid_Argument );

    ಯದಿ ( source == target )
      ಪ್ರತ್ಯಾಗಮ FT_Err_Ok;

    flip = ( source->pitch < 0 && target->pitch > 0 ) ||
           ( source->pitch > 0 && target->pitch < 0 );

    memory = library->memory;
    FT_FREE( target->buffer );

    *target = *source;

    ಯದಿ ( flip )
      target->pitch = -target->pitch;

    ಯದಿ ( !source->buffer )
      ಪ್ರತ್ಯಾಗಮ FT_Err_Ok;

    pitch  = source->pitch;
    ಯದಿ ( pitch < 0 )
      pitch = -pitch;

    FT_MEM_QALLOC_MULT( target->buffer, target->rows, pitch );

    ಯದಿ ( !error )
    {
      ಯದಿ ( flip )
      {
        /* take care of bitmap flow */
        FT_UInt   i;
        FT_Byte*  s = source->buffer;
        FT_Byte*  t = target->buffer;


        t += (FT_ULong)pitch * ( target->rows - 1 );

        ಪ್ರತಿ ( i = target->rows; i > 0; i-- )
        {
          FT_ARRAY_COPY( t, s, pitch );

          s += pitch;
          t -= pitch;
        }
      }
      ಅನ್ಯಥಾ
        FT_MEM_COPY( target->buffer, source->buffer,
                     (FT_Long)source->rows * pitch );
    }

    ಪ್ರತ್ಯಾಗಮ error;
  }


  /* Enlarge `bitmap' horizontally and vertically by `xpixels' */
  /* and `ypixels', respectively.                              */

  ಸ್ಥಿರ FT_Error
  ft_bitmap_assure_buffer( FT_Memory   memory,
                           FT_Bitmap*  bitmap,
                           FT_UInt     xpixels,
                           FT_UInt     ypixels )
  {
    FT_Error        error;
    unsigned ಪೂರ್ಣಾಙ್ಕ    pitch;
    unsigned ಪೂರ್ಣಾಙ್ಕ    new_pitch;
    FT_UInt         bpp;
    FT_UInt         width, height;
    unsigned ವರ್ಣ*  buffer = ರಿಕ್ತ;


    width  = bitmap->width;
    height = bitmap->rows;
    pitch  = (unsigned ಪೂರ್ಣಾಙ್ಕ)FT_ABS( bitmap->pitch );

    ವಿಕಲ್ಪ ( bitmap->pixel_mode )
    {
    ಪಕ್ಷ FT_PIXEL_MODE_MONO:
      bpp       = 1;
      new_pitch = ( width + xpixels + 7 ) >> 3;
      ಭಙ್ಗ;
    ಪಕ್ಷ FT_PIXEL_MODE_GRAY2:
      bpp       = 2;
      new_pitch = ( width + xpixels + 3 ) >> 2;
      ಭಙ್ಗ;
    ಪಕ್ಷ FT_PIXEL_MODE_GRAY4:
      bpp       = 4;
      new_pitch = ( width + xpixels + 1 ) >> 1;
      ಭಙ್ಗ;
    ಪಕ್ಷ FT_PIXEL_MODE_GRAY:
    ಪಕ್ಷ FT_PIXEL_MODE_LCD:
    ಪಕ್ಷ FT_PIXEL_MODE_LCD_V:
      bpp       = 8;
      new_pitch = width + xpixels;
      ಭಙ್ಗ;
    ಸ್ವಭಾವ:
      ಪ್ರತ್ಯಾಗಮ FT_THROW( Invalid_Glyph_Format );
    }

    /* if no need to allocate memory */
    ಯದಿ ( ypixels == 0 && new_pitch <= pitch )
    {
      /* zero the padding */
      FT_UInt  bit_width = pitch * 8;
      FT_UInt  bit_last  = ( width + xpixels ) * bpp;


      ಯದಿ ( bit_last < bit_width )
      {
        FT_Byte*  ಸಾಲು  = bitmap->buffer + ( bit_last >> 3 );
        FT_Byte*  end   = bitmap->buffer + pitch;
        FT_UInt   shift = bit_last & 7;
        FT_UInt   mask  = 0xFF00U >> shift;
        FT_UInt   ಎಣಿಕೆ = height;


        ಪ್ರತಿ ( ; ಎಣಿಕೆ > 0; ಎಣಿಕೆ--, ಸಾಲು += pitch, end += pitch )
        {
          FT_Byte*  ಲಿಖ = ಸಾಲು;


          ಯದಿ ( shift > 0 )
          {
            ಲಿಖ[0] = (FT_Byte)( ಲಿಖ[0] & mask );
            ಲಿಖ++;
          }
          ಯದಿ ( ಲಿಖ < end )
            FT_MEM_ZERO( ಲಿಖ, end - ಲಿಖ );
        }
      }

      ಪ್ರತ್ಯಾಗಮ FT_Err_Ok;
    }

    /* otherwise allocate new buffer */
    ಯದಿ ( FT_QALLOC_MULT( buffer, bitmap->rows + ypixels, new_pitch ) )
      ಪ್ರತ್ಯಾಗಮ error;

    /* new rows get added at the top of the bitmap, */
    /* thus take care of the flow direction         */
    ಯದಿ ( bitmap->pitch > 0 )
    {
      FT_UInt  len = ( width * bpp + 7 ) >> 3;

      unsigned ವರ್ಣ*  in  = bitmap->buffer;
      unsigned ವರ್ಣ*  out = buffer;

      unsigned ವರ್ಣ*  limit = bitmap->buffer + pitch * bitmap->rows;
      unsigned ಪೂರ್ಣಾಙ್ಕ    delta = new_pitch - len;


      FT_MEM_ZERO( out, new_pitch * ypixels );
      out += new_pitch * ypixels;

      ಯಾವತ್ ( in < limit )
      {
        FT_MEM_COPY( out, in, len );
        in  += pitch;
        out += len;

        /* we use FT_QALLOC_MULT, which doesn't zero out the buffer;      */
        /* consequently, we have to manually zero out the remaining bytes */
        FT_MEM_ZERO( out, delta );
        out += delta;
      }
    }
    ಅನ್ಯಥಾ
    {
      FT_UInt  len = ( width * bpp + 7 ) >> 3;

      unsigned ವರ್ಣ*  in  = bitmap->buffer;
      unsigned ವರ್ಣ*  out = buffer;

      unsigned ವರ್ಣ*  limit = bitmap->buffer + pitch * bitmap->rows;
      unsigned ಪೂರ್ಣಾಙ್ಕ    delta = new_pitch - len;


      ಯಾವತ್ ( in < limit )
      {
        FT_MEM_COPY( out, in, len );
        in  += pitch;
        out += len;

        FT_MEM_ZERO( out, delta );
        out += delta;
      }

      FT_MEM_ZERO( out, new_pitch * ypixels );
    }

    FT_FREE( bitmap->buffer );
    bitmap->buffer = buffer;

    /* set pitch only, width and height are left untouched */
    ಯದಿ ( bitmap->pitch < 0 )
      bitmap->pitch = -(ಪೂರ್ಣಾಙ್ಕ)new_pitch;
    ಅನ್ಯಥಾ
      bitmap->pitch = (ಪೂರ್ಣಾಙ್ಕ)new_pitch;

    ಪ್ರತ್ಯಾಗಮ FT_Err_Ok;
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Bitmap_Embolden( FT_Library  library,
                      FT_Bitmap*  bitmap,
                      FT_Pos      xStrength,
                      FT_Pos      yStrength )
  {
    FT_Error        error;
    unsigned ವರ್ಣ*  p;
    FT_Int          i, x, pitch;
    FT_UInt         y;
    FT_Int          xstr, ystr;


    ಯದಿ ( !library )
      ಪ್ರತ್ಯಾಗಮ FT_THROW( Invalid_Library_Handle );

    ಯದಿ ( !bitmap || !bitmap->buffer )
      ಪ್ರತ್ಯಾಗಮ FT_THROW( Invalid_Argument );

    ಯದಿ ( ( ( FT_PIX_ROUND( xStrength ) >> 6 ) > FT_INT_MAX ) ||
         ( ( FT_PIX_ROUND( yStrength ) >> 6 ) > FT_INT_MAX ) )
      ಪ್ರತ್ಯಾಗಮ FT_THROW( Invalid_Argument );

    xstr = (FT_Int)FT_PIX_ROUND( xStrength ) >> 6;
    ystr = (FT_Int)FT_PIX_ROUND( yStrength ) >> 6;

    ಯದಿ ( xstr == 0 && ystr == 0 )
      ಪ್ರತ್ಯಾಗಮ FT_Err_Ok;
    ಅನ್ಯಥಾ ಯದಿ ( xstr < 0 || ystr < 0 )
      ಪ್ರತ್ಯಾಗಮ FT_THROW( Invalid_Argument );

    ವಿಕಲ್ಪ ( bitmap->pixel_mode )
    {
    ಪಕ್ಷ FT_PIXEL_MODE_GRAY2:
    ಪಕ್ಷ FT_PIXEL_MODE_GRAY4:
      {
        FT_Bitmap  tmp;


        /* convert to 8bpp */
        FT_Bitmap_Init( &tmp );
        error = FT_Bitmap_Convert( library, bitmap, &tmp, 1 );
        ಯದಿ ( error )
          ಪ್ರತ್ಯಾಗಮ error;

        FT_Bitmap_Done( library, bitmap );
        *bitmap = tmp;
      }
      ಭಙ್ಗ;

    ಪಕ್ಷ FT_PIXEL_MODE_MONO:
      ಯದಿ ( xstr > 8 )
        xstr = 8;
      ಭಙ್ಗ;

    ಪಕ್ಷ FT_PIXEL_MODE_LCD:
      xstr *= 3;
      ಭಙ್ಗ;

    ಪಕ್ಷ FT_PIXEL_MODE_LCD_V:
      ystr *= 3;
      ಭಙ್ಗ;

    ಪಕ್ಷ FT_PIXEL_MODE_BGRA:
      /* We don't embolden color glyphs. */
      ಪ್ರತ್ಯಾಗಮ FT_Err_Ok;
    }

    error = ft_bitmap_assure_buffer( library->memory, bitmap,
                                     (FT_UInt)xstr, (FT_UInt)ystr );
    ಯದಿ ( error )
      ಪ್ರತ್ಯಾಗಮ error;

    /* take care of bitmap flow */
    pitch = bitmap->pitch;
    ಯದಿ ( pitch > 0 )
      p = bitmap->buffer + pitch * ystr;
    ಅನ್ಯಥಾ
    {
      pitch = -pitch;
      p = bitmap->buffer + (FT_UInt)pitch * ( bitmap->rows - 1 );
    }

    /* for each row */
    ಪ್ರತಿ ( y = 0; y < bitmap->rows; y++ )
    {
      /*
       * Horizontally:
       *
       * From the last pixel on, make each pixel or'ed with the
       * `xstr' pixels before it.
       */
      ಪ್ರತಿ ( x = pitch - 1; x >= 0; x-- )
      {
        unsigned ವರ್ಣ  tmp;


        tmp = p[x];
        ಪ್ರತಿ ( i = 1; i <= xstr; i++ )
        {
          ಯದಿ ( bitmap->pixel_mode == FT_PIXEL_MODE_MONO )
          {
            p[x] |= tmp >> i;

            /* the maximum value of 8 for `xstr' comes from here */
            ಯದಿ ( x > 0 )
              p[x] |= p[x - 1] << ( 8 - i );

#ಯದಿ 0
            ಯದಿ ( p[x] == 0xFF )
              ಭಙ್ಗ;
#endif
          }
          ಅನ್ಯಥಾ
          {
            ಯದಿ ( x - i >= 0 )
            {
              ಯದಿ ( p[x] + p[x - i] > bitmap->ಅಂಕೆ_grays - 1 )
              {
                p[x] = (unsigned ವರ್ಣ)( bitmap->ಅಂಕೆ_grays - 1 );
                ಭಙ್ಗ;
              }
              ಅನ್ಯಥಾ
              {
                p[x] = (unsigned ವರ್ಣ)( p[x] + p[x - i] );
                ಯದಿ ( p[x] == bitmap->ಅಂಕೆ_grays - 1 )
                  ಭಙ್ಗ;
              }
            }
            ಅನ್ಯಥಾ
              ಭಙ್ಗ;
          }
        }
      }

      /*
       * Vertically:
       *
       * Make the above `ystr' rows or'ed with it.
       */
      ಪ್ರತಿ ( x = 1; x <= ystr; x++ )
      {
        unsigned ವರ್ಣ*  q;


        q = p - bitmap->pitch * x;
        ಪ್ರತಿ ( i = 0; i < pitch; i++ )
          q[i] |= p[i];
      }

      p += bitmap->pitch;
    }

    bitmap->width += (FT_UInt)xstr;
    bitmap->rows += (FT_UInt)ystr;

    ಪ್ರತ್ಯಾಗಮ FT_Err_Ok;
  }


  ಸ್ಥಿರ FT_Byte
  ft_gray_for_premultiplied_srgb_bgra( ಧ್ರುವ FT_Byte*  bgra )
  {
    FT_UInt  a = bgra[3];
    FT_UInt  l;


    /* Short-circuit transparent color to avoid division by zero. */
    ಯದಿ ( !a )
      ಪ್ರತ್ಯಾಗಮ 0;

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

    ಪ್ರತ್ಯಾಗಮ (FT_Byte)( a - l / a );
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Bitmap_Convert( FT_Library        library,
                     ಧ್ರುವ FT_Bitmap  *source,
                     FT_Bitmap        *target,
                     FT_Int            alignment )
  {
    FT_Error   error = FT_Err_Ok;
    FT_Memory  memory;

    FT_Byte*  s;
    FT_Byte*  t;


    ಯದಿ ( !library )
      ಪ್ರತ್ಯಾಗಮ FT_THROW( Invalid_Library_Handle );

    ಯದಿ ( !source || !target )
      ಪ್ರತ್ಯಾಗಮ FT_THROW( Invalid_Argument );

    memory = library->memory;

    ವಿಕಲ್ಪ ( source->pixel_mode )
    {
    ಪಕ್ಷ FT_PIXEL_MODE_MONO:
    ಪಕ್ಷ FT_PIXEL_MODE_GRAY:
    ಪಕ್ಷ FT_PIXEL_MODE_GRAY2:
    ಪಕ್ಷ FT_PIXEL_MODE_GRAY4:
    ಪಕ್ಷ FT_PIXEL_MODE_LCD:
    ಪಕ್ಷ FT_PIXEL_MODE_LCD_V:
    ಪಕ್ಷ FT_PIXEL_MODE_BGRA:
      {
        FT_Int  width = (FT_Int)source->width;
        FT_Int  neg   = ( target->pitch == 0 && source->pitch < 0 ) ||
                          target->pitch  < 0;


        FT_Bitmap_Done( library, target );

        target->pixel_mode = FT_PIXEL_MODE_GRAY;
        target->rows       = source->rows;
        target->width      = source->width;

        ಯದಿ ( alignment )
        {
          FT_Int  rem = width % alignment;


          ಯದಿ ( rem )
            width = alignment > 0 ? width - rem + alignment
                                  : width - rem - alignment;
        }

        ಯದಿ ( FT_QALLOC_MULT( target->buffer, target->rows, width ) )
          ಪ್ರತ್ಯಾಗಮ error;

        target->pitch = neg ? -width : width;
      }
      ಭಙ್ಗ;

    ಸ್ವಭಾವ:
      error = FT_THROW( Invalid_Argument );
    }

    s = source->buffer;
    t = target->buffer;

    /* take care of bitmap flow */
    ಯದಿ ( source->pitch < 0 )
      s -= source->pitch * (FT_Int)( source->rows - 1 );
    ಯದಿ ( target->pitch < 0 )
      t -= target->pitch * (FT_Int)( target->rows - 1 );

    ವಿಕಲ್ಪ ( source->pixel_mode )
    {
    ಪಕ್ಷ FT_PIXEL_MODE_MONO:
      {
        FT_UInt  i;


        target->ಅಂಕೆ_grays = 2;

        ಪ್ರತಿ ( i = source->rows; i > 0; i-- )
        {
          FT_Byte*  ss = s;
          FT_Byte*  tt = t;
          FT_UInt   j;


          /* get the full bytes */
          ಪ್ರತಿ ( j = source->width >> 3; j > 0; j-- )
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
          j = source->width & 7;
          ಯದಿ ( j > 0 )
          {
            FT_Int  val = *ss;


            ಪ್ರತಿ ( ; j > 0; j-- )
            {
              tt[0] = (FT_Byte)( ( val & 0x80 ) >> 7);
              val <<= 1;
              tt   += 1;
            }
          }

          s += source->pitch;
          t += target->pitch;
        }
      }
      ಭಙ್ಗ;


    ಪಕ್ಷ FT_PIXEL_MODE_GRAY:
    ಪಕ್ಷ FT_PIXEL_MODE_LCD:
    ಪಕ್ಷ FT_PIXEL_MODE_LCD_V:
      {
        FT_UInt  width = source->width;
        FT_UInt  i;


        target->ಅಂಕೆ_grays = 256;

        ಪ್ರತಿ ( i = source->rows; i > 0; i-- )
        {
          FT_ARRAY_COPY( t, s, width );

          s += source->pitch;
          t += target->pitch;
        }
      }
      ಭಙ್ಗ;


    ಪಕ್ಷ FT_PIXEL_MODE_GRAY2:
      {
        FT_UInt  i;


        target->ಅಂಕೆ_grays = 4;

        ಪ್ರತಿ ( i = source->rows; i > 0; i-- )
        {
          FT_Byte*  ss = s;
          FT_Byte*  tt = t;
          FT_UInt   j;


          /* get the full bytes */
          ಪ್ರತಿ ( j = source->width >> 2; j > 0; j-- )
          {
            FT_Int  val = ss[0];


            tt[0] = (FT_Byte)( ( val & 0xC0 ) >> 6 );
            tt[1] = (FT_Byte)( ( val & 0x30 ) >> 4 );
            tt[2] = (FT_Byte)( ( val & 0x0C ) >> 2 );
            tt[3] = (FT_Byte)( ( val & 0x03 ) );

            ss += 1;
            tt += 4;
          }

          j = source->width & 3;
          ಯದಿ ( j > 0 )
          {
            FT_Int  val = ss[0];


            ಪ್ರತಿ ( ; j > 0; j-- )
            {
              tt[0]  = (FT_Byte)( ( val & 0xC0 ) >> 6 );
              val  <<= 2;
              tt    += 1;
            }
          }

          s += source->pitch;
          t += target->pitch;
        }
      }
      ಭಙ್ಗ;


    ಪಕ್ಷ FT_PIXEL_MODE_GRAY4:
      {
        FT_UInt  i;


        target->ಅಂಕೆ_grays = 16;

        ಪ್ರತಿ ( i = source->rows; i > 0; i-- )
        {
          FT_Byte*  ss = s;
          FT_Byte*  tt = t;
          FT_UInt   j;


          /* get the full bytes */
          ಪ್ರತಿ ( j = source->width >> 1; j > 0; j-- )
          {
            FT_Int  val = ss[0];


            tt[0] = (FT_Byte)( ( val & 0xF0 ) >> 4 );
            tt[1] = (FT_Byte)( ( val & 0x0F ) );

            ss += 1;
            tt += 2;
          }

          ಯದಿ ( source->width & 1 )
            tt[0] = (FT_Byte)( ( ss[0] & 0xF0 ) >> 4 );

          s += source->pitch;
          t += target->pitch;
        }
      }
      ಭಙ್ಗ;


    ಪಕ್ಷ FT_PIXEL_MODE_BGRA:
      {
        FT_UInt  i;


        target->ಅಂಕೆ_grays = 256;

        ಪ್ರತಿ ( i = source->rows; i > 0; i-- )
        {
          FT_Byte*  ss = s;
          FT_Byte*  tt = t;
          FT_UInt   j;


          ಪ್ರತಿ ( j = source->width; j > 0; j-- )
          {
            tt[0] = ft_gray_for_premultiplied_srgb_bgra( ss );

            ss += 4;
            tt += 1;
          }

          s += source->pitch;
          t += target->pitch;
        }
      }
      ಭಙ್ಗ;

    ಸ್ವಭಾವ:
      ;
    }

    ಪ್ರತ್ಯಾಗಮ error;
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Bitmap_Blend( FT_Library        library,
                   ಧ್ರುವ FT_Bitmap*  source_,
                   ಧ್ರುವ FT_Vector   source_offset_,
                   FT_Bitmap*        target,
                   FT_Vector        *atarget_offset,
                   FT_Color          color )
  {
    FT_Error   error = FT_Err_Ok;
    FT_Memory  memory;

    FT_Bitmap         source_bitmap;
    ಧ್ರುವ FT_Bitmap*  source;

    FT_Bool  free_source_bitmap          = 0;
    FT_Bool  free_target_bitmap_on_error = 0;

    FT_BBox  sbx, tbx, fbx;

    unsigned ಪೂರ್ಣಾಙ್ಕ  final_rows, final_width;


    ಯದಿ ( !library || !target || !source_ || !atarget_offset )
      ಪ್ರತ್ಯಾಗಮ FT_THROW( Invalid_Argument );

    memory = library->memory;

    ಯದಿ ( !( target->pixel_mode == FT_PIXEL_MODE_NONE     ||
            ( target->pixel_mode == FT_PIXEL_MODE_BGRA &&
              target->buffer                           ) ) )
      ಪ್ರತ್ಯಾಗಮ FT_THROW( Invalid_Argument );

    ಯದಿ ( source_->pixel_mode == FT_PIXEL_MODE_NONE )
      ಪ್ರತ್ಯಾಗಮ FT_Err_Ok;               /* nothing to do */

    /* pitches must have the same sign */
    ಯದಿ ( target->pixel_mode == FT_PIXEL_MODE_BGRA &&
         ( source_->pitch ^ target->pitch ) < 0   )
      ಪ್ರತ್ಯಾಗಮ FT_THROW( Invalid_Argument );

    ಯದಿ ( !( source_->width && source_->rows ) )
      ಪ್ರತ್ಯಾಗಮ FT_Err_Ok;               /* nothing to do */

    /* get source bitmap dimensions assuming integer offsets */
    sbx.xMin = ( source_offset_.x >> 6 );
    sbx.yMin = ( source_offset_.y >> 6 ) - source_->rows;
    sbx.xMax = ( source_offset_.x >> 6 ) + source_->width;
    sbx.yMax = ( source_offset_.y >> 6 );

    FT_TRACE5(( "FT_Bitmap_Blend:\n" ));
    FT_TRACE5(( "  source bitmap: (%ld, %ld) -- (%ld, %ld); %u x %u\n",
      sbx.xMin, sbx.yMin, sbx.xMax, sbx.yMax,
      source_->width, source_->rows ));

    /* sanity check */
    ಯದಿ ( sbx.xMin > sbx.xMax      || sbx.yMin > sbx.yMax     ||
         source_->width > 0x7FFFU || source_->rows > 0x7FFFU )
    {
      FT_TRACE5(( "FT_Bitmap_Blend: source dimension overflow\n" ));
      ಪ್ರತ್ಯಾಗಮ FT_THROW( Invalid_Argument );
    }

    ಯದಿ ( !target->width || !target->rows )
    {
      FT_TRACE5(( "  target bitmap: empty\n" ));

      final_width = source_->width;
      final_rows  = source_->rows;

      /* create new empty bitmap */
      target->width      = final_width;
      target->rows       = final_rows;
      target->pixel_mode = FT_PIXEL_MODE_BGRA;
      target->pitch      = (ಪೂರ್ಣಾಙ್ಕ)final_width * 4;
      target->ಅಂಕೆ_grays  = 256;

      ಯದಿ ( FT_ALLOC_MULT( target->buffer, target->rows, target->pitch ) )
        ಪ್ರತ್ಯಾಗಮ error;

      free_target_bitmap_on_error = 1;

      fbx = sbx;
    }
    ಅನ್ಯಥಾ
    {
      /* get target bitmap dimensions assuming integer offsets */
      tbx.xMin = ( atarget_offset->x >> 6 );
      tbx.yMin = ( atarget_offset->y >> 6 ) - target->rows;
      tbx.xMax = ( atarget_offset->x >> 6 ) + target->width;
      tbx.yMax = ( atarget_offset->y >> 6 );

      FT_TRACE5(( "  target bitmap: (%ld, %ld) -- (%ld, %ld); %u x %u\n",
        tbx.xMin, tbx.yMin, tbx.xMax, tbx.yMax,
        target->width, target->rows ));

      /* sanity check */
      ಯದಿ ( tbx.xMin > tbx.xMax || tbx.yMin > tbx.yMax )
      {
        FT_TRACE5(( "FT_Bitmap_Blend: targget dimension overflow\n" ));
        ಪ್ರತ್ಯಾಗಮ FT_THROW( Invalid_Argument );
      }

      /* compute final bitmap dimensions */
      fbx.xMin = FT_MIN( sbx.xMin, tbx.xMin );
      fbx.yMin = FT_MIN( sbx.yMin, tbx.yMin );
      fbx.xMax = FT_MAX( sbx.xMax, tbx.xMax );
      fbx.yMax = FT_MAX( sbx.yMax, tbx.yMax );

      /* sanity check */
      ಯದಿ ( fbx.xMin < -0x10000 || fbx.xMax >= 0x10000 ||
           fbx.yMin < -0x10000 || fbx.yMax >= 0x10000 )
      {
        FT_TRACE5(( "FT_Bitmap_Blend: final dimension overflow\n" ));
        ಪ್ರತ್ಯಾಗಮ FT_THROW( Invalid_Argument );
      }

      final_width = fbx.xMax - fbx.xMin;
      final_rows  = fbx.yMax - fbx.yMin;

      /* adjust target bitmap to enlarged size */
      ಯದಿ ( target->width < final_width ||
           target->rows  < final_rows  )
      {
        ಪೂರ್ಣಾಙ್ಕ  pitch, new_pitch;

        unsigned ವರ್ಣ*  buffer = ರಿಕ್ತ;


        pitch = target->pitch;

        ಯದಿ ( pitch < 0 )
          pitch = -pitch;

        new_pitch = (ಪೂರ್ಣಾಙ್ಕ)final_width * 4;

        /* TODO: provide an in-buffer solution for large bitmaps */
        /*       to avoid allocation of a new buffer             */
        ಯದಿ ( FT_ALLOC_MULT( buffer, final_rows, new_pitch ) )
          ಗಚ್ಛ Error;

        /* copy data to new buffer */

        /* the bitmap flow is from top to bottom, */
        /* but y is measured from bottom to top   */
        ಯದಿ ( target->pitch < 0 )
        {
          /* XXX */
        }
        ಅನ್ಯಥಾ
        {
          FT_Pos  x = tbx.xMin - fbx.xMin;
          FT_Pos  y = tbx.yMin - fbx.yMin;

          unsigned ವರ್ಣ*  p =
            target->buffer;
          unsigned ವರ್ಣ*  q =
            buffer +
            ( final_rows - y - target->rows ) * new_pitch +
            x * 4;
          unsigned ವರ್ಣ*  limit_p =
            p + pitch * (ಪೂರ್ಣಾಙ್ಕ)target->rows;


          ಯಾವತ್ ( p < limit_p )
          {
            FT_MEM_COPY( q, p, pitch );

            p += pitch;
            q += new_pitch;
          }
        }

        FT_FREE( target->buffer );

        target->width = final_width;
        target->rows  = final_rows;

        ಯದಿ ( target->pitch < 0 )
          target->pitch = -new_pitch;
        ಅನ್ಯಥಾ
          target->pitch = new_pitch;

        target->buffer = buffer;
      }
    }

    FT_TRACE5(( "  final bitmap: (%ld, %ld) -- (%ld, %ld); %u x %u\n",
      fbx.xMin, fbx.yMin, fbx.xMax, fbx.yMax,
      final_width, final_rows ));

    /* adjust source bitmap if necessary */
    ಯದಿ ( source_->pixel_mode != FT_PIXEL_MODE_GRAY )
    {
      FT_Bitmap_Init( &source_bitmap );
      error = FT_Bitmap_Convert( library, source_, &source_bitmap, 1 );
      ಯದಿ ( error )
        ಗಚ್ಛ Error;

      source             = &source_bitmap;
      free_source_bitmap = 1;
    }
    ಅನ್ಯಥಾ
      source = source_;

    /* do blending; the code below returns pre-multiplied channels, */
    /* similar to what FreeType gets from `CBDT' tables             */

    /* the bitmap flow is from top to bottom, */
    /* but y is measured from bottom to top   */
    ಯದಿ ( target->pitch < 0 )
    {
      /* XXX */
    }
    ಅನ್ಯಥಾ
    {
      FT_Pos  x = sbx.xMin - fbx.xMin;
      FT_Pos  y = sbx.yMin - fbx.yMin;

      unsigned ವರ್ಣ*  p =
        source->buffer;
      unsigned ವರ್ಣ*  q =
        target->buffer +
        ( target->rows - y - source->rows ) * target->pitch +
        x * 4;
      unsigned ವರ್ಣ*  limit_p =
        p + source->pitch * (ಪೂರ್ಣಾಙ್ಕ)source->rows;


      ಯಾವತ್ ( p < limit_p )
      {
        unsigned ವರ್ಣ*  r       = p;
        unsigned ವರ್ಣ*  s       = q;
        unsigned ವರ್ಣ*  limit_r = r + source->width;


        ಯಾವತ್ ( r < limit_r )
        {
          ಪೂರ್ಣಾಙ್ಕ  aa = *r++;
          ಪೂರ್ಣಾಙ್ಕ  fa = color.alpha * aa / 255;

          ಪೂರ್ಣಾಙ್ಕ  fb = color.blue * fa / 255;
          ಪೂರ್ಣಾಙ್ಕ  fg = color.green * fa / 255;
          ಪೂರ್ಣಾಙ್ಕ  fr = color.red * fa / 255;

          ಪೂರ್ಣಾಙ್ಕ  ba2 = 255 - fa;

          ಪೂರ್ಣಾಙ್ಕ  bb = s[0];
          ಪೂರ್ಣಾಙ್ಕ  bg = s[1];
          ಪೂರ್ಣಾಙ್ಕ  br = s[2];
          ಪೂರ್ಣಾಙ್ಕ  ba = s[3];


          *s++ = (unsigned ವರ್ಣ)( bb * ba2 / 255 + fb );
          *s++ = (unsigned ವರ್ಣ)( bg * ba2 / 255 + fg );
          *s++ = (unsigned ವರ್ಣ)( br * ba2 / 255 + fr );
          *s++ = (unsigned ವರ್ಣ)( ba * ba2 / 255 + fa );
        }

        p += source->pitch;
        q += target->pitch;
      }
    }

    atarget_offset->x = fbx.xMin * 64;
    atarget_offset->y = fbx.yMax * 64;

  Error:
    ಯದಿ ( error && free_target_bitmap_on_error )
      FT_Bitmap_Done( library, target );

    ಯದಿ ( free_source_bitmap )
      FT_Bitmap_Done( library, &source_bitmap );

    ಪ್ರತ್ಯಾಗಮ error;
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_GlyphSlot_Own_Bitmap( FT_GlyphSlot  slot )
  {
    ಯದಿ ( slot && slot->format == FT_GLYPH_FORMAT_BITMAP   &&
         !( slot->internal->flags & FT_GLYPH_OWN_BITMAP ) )
    {
      FT_Bitmap  bitmap;
      FT_Error   error;


      FT_Bitmap_Init( &bitmap );
      error = FT_Bitmap_Copy( slot->library, &slot->bitmap, &bitmap );
      ಯದಿ ( error )
        ಪ್ರತ್ಯಾಗಮ error;

      slot->bitmap = bitmap;
      slot->internal->flags |= FT_GLYPH_OWN_BITMAP;
    }

    ಪ್ರತ್ಯಾಗಮ FT_Err_Ok;
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Bitmap_Done( FT_Library  library,
                  FT_Bitmap  *bitmap )
  {
    FT_Memory  memory;


    ಯದಿ ( !library )
      ಪ್ರತ್ಯಾಗಮ FT_THROW( Invalid_Library_Handle );

    ಯದಿ ( !bitmap )
      ಪ್ರತ್ಯಾಗಮ FT_THROW( Invalid_Argument );

    memory = library->memory;

    FT_FREE( bitmap->buffer );
    *bitmap = null_bitmap;

    ಪ್ರತ್ಯಾಗಮ FT_Err_Ok;
  }


/* END */
