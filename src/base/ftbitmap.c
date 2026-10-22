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


#അന്തര്ഭാവ <freetype/internal/ftdebug.h>

#അന്തര്ഭാവ <freetype/ftbitmap.h>
#അന്തര്ഭാവ <freetype/ftimage.h>
#അന്തര്ഭാവ <freetype/internal/ftobjs.h>


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#undef  FT_COMPONENT
#define FT_COMPONENT  bitmap


  സ്ഥിതം
  സ്ഥിരം FT_Bitmap  null_bitmap = { 0, 0, 0, ശൂന്യ, 0, 0, 0, ശൂന്യ };


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( ശൂന്യം )
  FT_Bitmap_Init( FT_Bitmap  *abitmap )
  {
    എങ്കിൽ ( abitmap )
      *abitmap = null_bitmap;
  }


  /* deprecated function name; retained for ABI compatibility */

  FT_EXPORT_DEF( ശൂന്യം )
  FT_Bitmap_New( FT_Bitmap  *abitmap )
  {
    എങ്കിൽ ( abitmap )
      *abitmap = null_bitmap;
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Bitmap_Copy( FT_Library        library,
                  സ്ഥിരം FT_Bitmap  *source,
                  FT_Bitmap        *target)
  {
    FT_Memory  memory;
    FT_Error   error  = FT_Err_Ok;
    FT_Int     pitch;
    FT_Int     flip;


    എങ്കിൽ ( !library )
      തിരികെ FT_THROW( Invalid_Library_Handle );

    എങ്കിൽ ( !source || !target )
      തിരികെ FT_THROW( Invalid_Argument );

    എങ്കിൽ ( source == target )
      തിരികെ FT_Err_Ok;

    flip = ( source->pitch < 0 && target->pitch > 0 ) ||
           ( source->pitch > 0 && target->pitch < 0 );

    memory = library->memory;
    FT_FREE( target->buffer );

    *target = *source;

    എങ്കിൽ ( flip )
      target->pitch = -target->pitch;

    എങ്കിൽ ( !source->buffer )
      തിരികെ FT_Err_Ok;

    pitch  = source->pitch;
    എങ്കിൽ ( pitch < 0 )
      pitch = -pitch;

    FT_MEM_QALLOC_MULT( target->buffer, target->rows, pitch );

    എങ്കിൽ ( !error )
    {
      എങ്കിൽ ( flip )
      {
        /* take care of bitmap flow */
        FT_UInt   i;
        FT_Byte*  s = source->buffer;
        FT_Byte*  t = target->buffer;


        t += (FT_ULong)pitch * ( target->rows - 1 );

        വേണ്ടി ( i = target->rows; i > 0; i-- )
        {
          FT_ARRAY_COPY( t, s, pitch );

          s += pitch;
          t -= pitch;
        }
      }
      അല്ലെങ്കിൽ
        FT_MEM_COPY( target->buffer, source->buffer,
                     (FT_Long)source->rows * pitch );
    }

    തിരികെ error;
  }


  /* Enlarge `bitmap' horizontally and vertically by `xpixels' */
  /* and `ypixels', respectively.                              */

  സ്ഥിതം FT_Error
  ft_bitmap_assure_buffer( FT_Memory   memory,
                           FT_Bitmap*  bitmap,
                           FT_UInt     xpixels,
                           FT_UInt     ypixels )
  {
    FT_Error        error;
    unsigned പൂർണ്ണസംഖ്യ    pitch;
    unsigned പൂർണ്ണസംഖ്യ    new_pitch;
    FT_UInt         bpp;
    FT_UInt         width, height;
    unsigned അക്ഷരം*  buffer = ശൂന്യ;


    width  = bitmap->width;
    height = bitmap->rows;
    pitch  = (unsigned പൂർണ്ണസംഖ്യ)FT_ABS( bitmap->pitch );

    തിരഞ്ഞെടുപ്പ് ( bitmap->pixel_mode )
    {
    അവസ്ഥ FT_PIXEL_MODE_MONO:
      bpp       = 1;
      new_pitch = ( width + xpixels + 7 ) >> 3;
      നിർത്തുക;
    അവസ്ഥ FT_PIXEL_MODE_GRAY2:
      bpp       = 2;
      new_pitch = ( width + xpixels + 3 ) >> 2;
      നിർത്തുക;
    അവസ്ഥ FT_PIXEL_MODE_GRAY4:
      bpp       = 4;
      new_pitch = ( width + xpixels + 1 ) >> 1;
      നിർത്തുക;
    അവസ്ഥ FT_PIXEL_MODE_GRAY:
    അവസ്ഥ FT_PIXEL_MODE_LCD:
    അവസ്ഥ FT_PIXEL_MODE_LCD_V:
      bpp       = 8;
      new_pitch = width + xpixels;
      നിർത്തുക;
    സ്ഥിരസ്ഥിതി:
      തിരികെ FT_THROW( Invalid_Glyph_Format );
    }

    /* if no need to allocate memory */
    എങ്കിൽ ( ypixels == 0 && new_pitch <= pitch )
    {
      /* zero the padding */
      FT_UInt  bit_width = pitch * 8;
      FT_UInt  bit_last  = ( width + xpixels ) * bpp;


      എങ്കിൽ ( bit_last < bit_width )
      {
        FT_Byte*  വരി  = bitmap->buffer + ( bit_last >> 3 );
        FT_Byte*  end   = bitmap->buffer + pitch;
        FT_UInt   shift = bit_last & 7;
        FT_UInt   mask  = 0xFF00U >> shift;
        FT_UInt   എണ്ണം = height;


        വേണ്ടി ( ; എണ്ണം > 0; എണ്ണം--, വരി += pitch, end += pitch )
        {
          FT_Byte*  ലിഖ = വരി;


          എങ്കിൽ ( shift > 0 )
          {
            ലിഖ[0] = (FT_Byte)( ലിഖ[0] & mask );
            ലിഖ++;
          }
          എങ്കിൽ ( ലിഖ < end )
            FT_MEM_ZERO( ലിഖ, end - ലിഖ );
        }
      }

      തിരികെ FT_Err_Ok;
    }

    /* otherwise allocate new buffer */
    എങ്കിൽ ( FT_QALLOC_MULT( buffer, bitmap->rows + ypixels, new_pitch ) )
      തിരികെ error;

    /* new rows get added at the top of the bitmap, */
    /* thus take care of the flow direction         */
    എങ്കിൽ ( bitmap->pitch > 0 )
    {
      FT_UInt  len = ( width * bpp + 7 ) >> 3;

      unsigned അക്ഷരം*  in  = bitmap->buffer;
      unsigned അക്ഷരം*  out = buffer;

      unsigned അക്ഷരം*  limit = bitmap->buffer + pitch * bitmap->rows;
      unsigned പൂർണ്ണസംഖ്യ    delta = new_pitch - len;


      FT_MEM_ZERO( out, new_pitch * ypixels );
      out += new_pitch * ypixels;

      ആകുമ്പോൾ ( in < limit )
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
    അല്ലെങ്കിൽ
    {
      FT_UInt  len = ( width * bpp + 7 ) >> 3;

      unsigned അക്ഷരം*  in  = bitmap->buffer;
      unsigned അക്ഷരം*  out = buffer;

      unsigned അക്ഷരം*  limit = bitmap->buffer + pitch * bitmap->rows;
      unsigned പൂർണ്ണസംഖ്യ    delta = new_pitch - len;


      ആകുമ്പോൾ ( in < limit )
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
    എങ്കിൽ ( bitmap->pitch < 0 )
      bitmap->pitch = -(പൂർണ്ണസംഖ്യ)new_pitch;
    അല്ലെങ്കിൽ
      bitmap->pitch = (പൂർണ്ണസംഖ്യ)new_pitch;

    തിരികെ FT_Err_Ok;
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Bitmap_Embolden( FT_Library  library,
                      FT_Bitmap*  bitmap,
                      FT_Pos      xStrength,
                      FT_Pos      yStrength )
  {
    FT_Error        error;
    unsigned അക്ഷരം*  p;
    FT_Int          i, x, pitch;
    FT_UInt         y;
    FT_Int          xstr, ystr;


    എങ്കിൽ ( !library )
      തിരികെ FT_THROW( Invalid_Library_Handle );

    എങ്കിൽ ( !bitmap || !bitmap->buffer )
      തിരികെ FT_THROW( Invalid_Argument );

    എങ്കിൽ ( ( ( FT_PIX_ROUND( xStrength ) >> 6 ) > FT_INT_MAX ) ||
         ( ( FT_PIX_ROUND( yStrength ) >> 6 ) > FT_INT_MAX ) )
      തിരികെ FT_THROW( Invalid_Argument );

    xstr = (FT_Int)FT_PIX_ROUND( xStrength ) >> 6;
    ystr = (FT_Int)FT_PIX_ROUND( yStrength ) >> 6;

    എങ്കിൽ ( xstr == 0 && ystr == 0 )
      തിരികെ FT_Err_Ok;
    അല്ലെങ്കിൽ എങ്കിൽ ( xstr < 0 || ystr < 0 )
      തിരികെ FT_THROW( Invalid_Argument );

    തിരഞ്ഞെടുപ്പ് ( bitmap->pixel_mode )
    {
    അവസ്ഥ FT_PIXEL_MODE_GRAY2:
    അവസ്ഥ FT_PIXEL_MODE_GRAY4:
      {
        FT_Bitmap  tmp;


        /* convert to 8bpp */
        FT_Bitmap_Init( &tmp );
        error = FT_Bitmap_Convert( library, bitmap, &tmp, 1 );
        എങ്കിൽ ( error )
          തിരികെ error;

        FT_Bitmap_Done( library, bitmap );
        *bitmap = tmp;
      }
      നിർത്തുക;

    അവസ്ഥ FT_PIXEL_MODE_MONO:
      എങ്കിൽ ( xstr > 8 )
        xstr = 8;
      നിർത്തുക;

    അവസ്ഥ FT_PIXEL_MODE_LCD:
      xstr *= 3;
      നിർത്തുക;

    അവസ്ഥ FT_PIXEL_MODE_LCD_V:
      ystr *= 3;
      നിർത്തുക;

    അവസ്ഥ FT_PIXEL_MODE_BGRA:
      /* We don't embolden color glyphs. */
      തിരികെ FT_Err_Ok;
    }

    error = ft_bitmap_assure_buffer( library->memory, bitmap,
                                     (FT_UInt)xstr, (FT_UInt)ystr );
    എങ്കിൽ ( error )
      തിരികെ error;

    /* take care of bitmap flow */
    pitch = bitmap->pitch;
    എങ്കിൽ ( pitch > 0 )
      p = bitmap->buffer + pitch * ystr;
    അല്ലെങ്കിൽ
    {
      pitch = -pitch;
      p = bitmap->buffer + (FT_UInt)pitch * ( bitmap->rows - 1 );
    }

    /* for each row */
    വേണ്ടി ( y = 0; y < bitmap->rows; y++ )
    {
      /*
       * Horizontally:
       *
       * From the last pixel on, make each pixel or'ed with the
       * `xstr' pixels before it.
       */
      വേണ്ടി ( x = pitch - 1; x >= 0; x-- )
      {
        unsigned അക്ഷരം  tmp;


        tmp = p[x];
        വേണ്ടി ( i = 1; i <= xstr; i++ )
        {
          എങ്കിൽ ( bitmap->pixel_mode == FT_PIXEL_MODE_MONO )
          {
            p[x] |= tmp >> i;

            /* the maximum value of 8 for `xstr' comes from here */
            എങ്കിൽ ( x > 0 )
              p[x] |= p[x - 1] << ( 8 - i );

#എങ്കിൽ 0
            എങ്കിൽ ( p[x] == 0xFF )
              നിർത്തുക;
#endif
          }
          അല്ലെങ്കിൽ
          {
            എങ്കിൽ ( x - i >= 0 )
            {
              എങ്കിൽ ( p[x] + p[x - i] > bitmap->അക്കം_grays - 1 )
              {
                p[x] = (unsigned അക്ഷരം)( bitmap->അക്കം_grays - 1 );
                നിർത്തുക;
              }
              അല്ലെങ്കിൽ
              {
                p[x] = (unsigned അക്ഷരം)( p[x] + p[x - i] );
                എങ്കിൽ ( p[x] == bitmap->അക്കം_grays - 1 )
                  നിർത്തുക;
              }
            }
            അല്ലെങ്കിൽ
              നിർത്തുക;
          }
        }
      }

      /*
       * Vertically:
       *
       * Make the above `ystr' rows or'ed with it.
       */
      വേണ്ടി ( x = 1; x <= ystr; x++ )
      {
        unsigned അക്ഷരം*  q;


        q = p - bitmap->pitch * x;
        വേണ്ടി ( i = 0; i < pitch; i++ )
          q[i] |= p[i];
      }

      p += bitmap->pitch;
    }

    bitmap->width += (FT_UInt)xstr;
    bitmap->rows += (FT_UInt)ystr;

    തിരികെ FT_Err_Ok;
  }


  സ്ഥിതം FT_Byte
  ft_gray_for_premultiplied_srgb_bgra( സ്ഥിരം FT_Byte*  bgra )
  {
    FT_UInt  a = bgra[3];
    FT_UInt  l;


    /* Short-circuit transparent color to avoid division by zero. */
    എങ്കിൽ ( !a )
      തിരികെ 0;

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

    തിരികെ (FT_Byte)( a - l / a );
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Bitmap_Convert( FT_Library        library,
                     സ്ഥിരം FT_Bitmap  *source,
                     FT_Bitmap        *target,
                     FT_Int            alignment )
  {
    FT_Error   error = FT_Err_Ok;
    FT_Memory  memory;

    FT_Byte*  s;
    FT_Byte*  t;


    എങ്കിൽ ( !library )
      തിരികെ FT_THROW( Invalid_Library_Handle );

    എങ്കിൽ ( !source || !target )
      തിരികെ FT_THROW( Invalid_Argument );

    memory = library->memory;

    തിരഞ്ഞെടുപ്പ് ( source->pixel_mode )
    {
    അവസ്ഥ FT_PIXEL_MODE_MONO:
    അവസ്ഥ FT_PIXEL_MODE_GRAY:
    അവസ്ഥ FT_PIXEL_MODE_GRAY2:
    അവസ്ഥ FT_PIXEL_MODE_GRAY4:
    അവസ്ഥ FT_PIXEL_MODE_LCD:
    അവസ്ഥ FT_PIXEL_MODE_LCD_V:
    അവസ്ഥ FT_PIXEL_MODE_BGRA:
      {
        FT_Int  width = (FT_Int)source->width;
        FT_Int  neg   = ( target->pitch == 0 && source->pitch < 0 ) ||
                          target->pitch  < 0;


        FT_Bitmap_Done( library, target );

        target->pixel_mode = FT_PIXEL_MODE_GRAY;
        target->rows       = source->rows;
        target->width      = source->width;

        എങ്കിൽ ( alignment )
        {
          FT_Int  rem = width % alignment;


          എങ്കിൽ ( rem )
            width = alignment > 0 ? width - rem + alignment
                                  : width - rem - alignment;
        }

        എങ്കിൽ ( FT_QALLOC_MULT( target->buffer, target->rows, width ) )
          തിരികെ error;

        target->pitch = neg ? -width : width;
      }
      നിർത്തുക;

    സ്ഥിരസ്ഥിതി:
      error = FT_THROW( Invalid_Argument );
    }

    s = source->buffer;
    t = target->buffer;

    /* take care of bitmap flow */
    എങ്കിൽ ( source->pitch < 0 )
      s -= source->pitch * (FT_Int)( source->rows - 1 );
    എങ്കിൽ ( target->pitch < 0 )
      t -= target->pitch * (FT_Int)( target->rows - 1 );

    തിരഞ്ഞെടുപ്പ് ( source->pixel_mode )
    {
    അവസ്ഥ FT_PIXEL_MODE_MONO:
      {
        FT_UInt  i;


        target->അക്കം_grays = 2;

        വേണ്ടി ( i = source->rows; i > 0; i-- )
        {
          FT_Byte*  ss = s;
          FT_Byte*  tt = t;
          FT_UInt   j;


          /* get the full bytes */
          വേണ്ടി ( j = source->width >> 3; j > 0; j-- )
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
          എങ്കിൽ ( j > 0 )
          {
            FT_Int  val = *ss;


            വേണ്ടി ( ; j > 0; j-- )
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
      നിർത്തുക;


    അവസ്ഥ FT_PIXEL_MODE_GRAY:
    അവസ്ഥ FT_PIXEL_MODE_LCD:
    അവസ്ഥ FT_PIXEL_MODE_LCD_V:
      {
        FT_UInt  width = source->width;
        FT_UInt  i;


        target->അക്കം_grays = 256;

        വേണ്ടി ( i = source->rows; i > 0; i-- )
        {
          FT_ARRAY_COPY( t, s, width );

          s += source->pitch;
          t += target->pitch;
        }
      }
      നിർത്തുക;


    അവസ്ഥ FT_PIXEL_MODE_GRAY2:
      {
        FT_UInt  i;


        target->അക്കം_grays = 4;

        വേണ്ടി ( i = source->rows; i > 0; i-- )
        {
          FT_Byte*  ss = s;
          FT_Byte*  tt = t;
          FT_UInt   j;


          /* get the full bytes */
          വേണ്ടി ( j = source->width >> 2; j > 0; j-- )
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
          എങ്കിൽ ( j > 0 )
          {
            FT_Int  val = ss[0];


            വേണ്ടി ( ; j > 0; j-- )
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
      നിർത്തുക;


    അവസ്ഥ FT_PIXEL_MODE_GRAY4:
      {
        FT_UInt  i;


        target->അക്കം_grays = 16;

        വേണ്ടി ( i = source->rows; i > 0; i-- )
        {
          FT_Byte*  ss = s;
          FT_Byte*  tt = t;
          FT_UInt   j;


          /* get the full bytes */
          വേണ്ടി ( j = source->width >> 1; j > 0; j-- )
          {
            FT_Int  val = ss[0];


            tt[0] = (FT_Byte)( ( val & 0xF0 ) >> 4 );
            tt[1] = (FT_Byte)( ( val & 0x0F ) );

            ss += 1;
            tt += 2;
          }

          എങ്കിൽ ( source->width & 1 )
            tt[0] = (FT_Byte)( ( ss[0] & 0xF0 ) >> 4 );

          s += source->pitch;
          t += target->pitch;
        }
      }
      നിർത്തുക;


    അവസ്ഥ FT_PIXEL_MODE_BGRA:
      {
        FT_UInt  i;


        target->അക്കം_grays = 256;

        വേണ്ടി ( i = source->rows; i > 0; i-- )
        {
          FT_Byte*  ss = s;
          FT_Byte*  tt = t;
          FT_UInt   j;


          വേണ്ടി ( j = source->width; j > 0; j-- )
          {
            tt[0] = ft_gray_for_premultiplied_srgb_bgra( ss );

            ss += 4;
            tt += 1;
          }

          s += source->pitch;
          t += target->pitch;
        }
      }
      നിർത്തുക;

    സ്ഥിരസ്ഥിതി:
      ;
    }

    തിരികെ error;
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Bitmap_Blend( FT_Library        library,
                   സ്ഥിരം FT_Bitmap*  source_,
                   സ്ഥിരം FT_Vector   source_offset_,
                   FT_Bitmap*        target,
                   FT_Vector        *atarget_offset,
                   FT_Color          color )
  {
    FT_Error   error = FT_Err_Ok;
    FT_Memory  memory;

    FT_Bitmap         source_bitmap;
    സ്ഥിരം FT_Bitmap*  source;

    FT_Bool  free_source_bitmap          = 0;
    FT_Bool  free_target_bitmap_on_error = 0;

    FT_BBox  sbx, tbx, fbx;

    unsigned പൂർണ്ണസംഖ്യ  final_rows, final_width;


    എങ്കിൽ ( !library || !target || !source_ || !atarget_offset )
      തിരികെ FT_THROW( Invalid_Argument );

    memory = library->memory;

    എങ്കിൽ ( !( target->pixel_mode == FT_PIXEL_MODE_NONE     ||
            ( target->pixel_mode == FT_PIXEL_MODE_BGRA &&
              target->buffer                           ) ) )
      തിരികെ FT_THROW( Invalid_Argument );

    എങ്കിൽ ( source_->pixel_mode == FT_PIXEL_MODE_NONE )
      തിരികെ FT_Err_Ok;               /* nothing to do */

    /* pitches must have the same sign */
    എങ്കിൽ ( target->pixel_mode == FT_PIXEL_MODE_BGRA &&
         ( source_->pitch ^ target->pitch ) < 0   )
      തിരികെ FT_THROW( Invalid_Argument );

    എങ്കിൽ ( !( source_->width && source_->rows ) )
      തിരികെ FT_Err_Ok;               /* nothing to do */

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
    എങ്കിൽ ( sbx.xMin > sbx.xMax      || sbx.yMin > sbx.yMax     ||
         source_->width > 0x7FFFU || source_->rows > 0x7FFFU )
    {
      FT_TRACE5(( "FT_Bitmap_Blend: source dimension overflow\n" ));
      തിരികെ FT_THROW( Invalid_Argument );
    }

    എങ്കിൽ ( !target->width || !target->rows )
    {
      FT_TRACE5(( "  target bitmap: empty\n" ));

      final_width = source_->width;
      final_rows  = source_->rows;

      /* create new empty bitmap */
      target->width      = final_width;
      target->rows       = final_rows;
      target->pixel_mode = FT_PIXEL_MODE_BGRA;
      target->pitch      = (പൂർണ്ണസംഖ്യ)final_width * 4;
      target->അക്കം_grays  = 256;

      എങ്കിൽ ( FT_ALLOC_MULT( target->buffer, target->rows, target->pitch ) )
        തിരികെ error;

      free_target_bitmap_on_error = 1;

      fbx = sbx;
    }
    അല്ലെങ്കിൽ
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
      എങ്കിൽ ( tbx.xMin > tbx.xMax || tbx.yMin > tbx.yMax )
      {
        FT_TRACE5(( "FT_Bitmap_Blend: targget dimension overflow\n" ));
        തിരികെ FT_THROW( Invalid_Argument );
      }

      /* compute final bitmap dimensions */
      fbx.xMin = FT_MIN( sbx.xMin, tbx.xMin );
      fbx.yMin = FT_MIN( sbx.yMin, tbx.yMin );
      fbx.xMax = FT_MAX( sbx.xMax, tbx.xMax );
      fbx.yMax = FT_MAX( sbx.yMax, tbx.yMax );

      /* sanity check */
      എങ്കിൽ ( fbx.xMin < -0x10000 || fbx.xMax >= 0x10000 ||
           fbx.yMin < -0x10000 || fbx.yMax >= 0x10000 )
      {
        FT_TRACE5(( "FT_Bitmap_Blend: final dimension overflow\n" ));
        തിരികെ FT_THROW( Invalid_Argument );
      }

      final_width = fbx.xMax - fbx.xMin;
      final_rows  = fbx.yMax - fbx.yMin;

      /* adjust target bitmap to enlarged size */
      എങ്കിൽ ( target->width < final_width ||
           target->rows  < final_rows  )
      {
        പൂർണ്ണസംഖ്യ  pitch, new_pitch;

        unsigned അക്ഷരം*  buffer = ശൂന്യ;


        pitch = target->pitch;

        എങ്കിൽ ( pitch < 0 )
          pitch = -pitch;

        new_pitch = (പൂർണ്ണസംഖ്യ)final_width * 4;

        /* TODO: provide an in-buffer solution for large bitmaps */
        /*       to avoid allocation of a new buffer             */
        എങ്കിൽ ( FT_ALLOC_MULT( buffer, final_rows, new_pitch ) )
          പോകുക Error;

        /* copy data to new buffer */

        /* the bitmap flow is from top to bottom, */
        /* but y is measured from bottom to top   */
        എങ്കിൽ ( target->pitch < 0 )
        {
          /* XXX */
        }
        അല്ലെങ്കിൽ
        {
          FT_Pos  x = tbx.xMin - fbx.xMin;
          FT_Pos  y = tbx.yMin - fbx.yMin;

          unsigned അക്ഷരം*  p =
            target->buffer;
          unsigned അക്ഷരം*  q =
            buffer +
            ( final_rows - y - target->rows ) * new_pitch +
            x * 4;
          unsigned അക്ഷരം*  limit_p =
            p + pitch * (പൂർണ്ണസംഖ്യ)target->rows;


          ആകുമ്പോൾ ( p < limit_p )
          {
            FT_MEM_COPY( q, p, pitch );

            p += pitch;
            q += new_pitch;
          }
        }

        FT_FREE( target->buffer );

        target->width = final_width;
        target->rows  = final_rows;

        എങ്കിൽ ( target->pitch < 0 )
          target->pitch = -new_pitch;
        അല്ലെങ്കിൽ
          target->pitch = new_pitch;

        target->buffer = buffer;
      }
    }

    FT_TRACE5(( "  final bitmap: (%ld, %ld) -- (%ld, %ld); %u x %u\n",
      fbx.xMin, fbx.yMin, fbx.xMax, fbx.yMax,
      final_width, final_rows ));

    /* adjust source bitmap if necessary */
    എങ്കിൽ ( source_->pixel_mode != FT_PIXEL_MODE_GRAY )
    {
      FT_Bitmap_Init( &source_bitmap );
      error = FT_Bitmap_Convert( library, source_, &source_bitmap, 1 );
      എങ്കിൽ ( error )
        പോകുക Error;

      source             = &source_bitmap;
      free_source_bitmap = 1;
    }
    അല്ലെങ്കിൽ
      source = source_;

    /* do blending; the code below returns pre-multiplied channels, */
    /* similar to what FreeType gets from `CBDT' tables             */

    /* the bitmap flow is from top to bottom, */
    /* but y is measured from bottom to top   */
    എങ്കിൽ ( target->pitch < 0 )
    {
      /* XXX */
    }
    അല്ലെങ്കിൽ
    {
      FT_Pos  x = sbx.xMin - fbx.xMin;
      FT_Pos  y = sbx.yMin - fbx.yMin;

      unsigned അക്ഷരം*  p =
        source->buffer;
      unsigned അക്ഷരം*  q =
        target->buffer +
        ( target->rows - y - source->rows ) * target->pitch +
        x * 4;
      unsigned അക്ഷരം*  limit_p =
        p + source->pitch * (പൂർണ്ണസംഖ്യ)source->rows;


      ആകുമ്പോൾ ( p < limit_p )
      {
        unsigned അക്ഷരം*  r       = p;
        unsigned അക്ഷരം*  s       = q;
        unsigned അക്ഷരം*  limit_r = r + source->width;


        ആകുമ്പോൾ ( r < limit_r )
        {
          പൂർണ്ണസംഖ്യ  aa = *r++;
          പൂർണ്ണസംഖ്യ  fa = color.alpha * aa / 255;

          പൂർണ്ണസംഖ്യ  fb = color.blue * fa / 255;
          പൂർണ്ണസംഖ്യ  fg = color.green * fa / 255;
          പൂർണ്ണസംഖ്യ  fr = color.red * fa / 255;

          പൂർണ്ണസംഖ്യ  ba2 = 255 - fa;

          പൂർണ്ണസംഖ്യ  bb = s[0];
          പൂർണ്ണസംഖ്യ  bg = s[1];
          പൂർണ്ണസംഖ്യ  br = s[2];
          പൂർണ്ണസംഖ്യ  ba = s[3];


          *s++ = (unsigned അക്ഷരം)( bb * ba2 / 255 + fb );
          *s++ = (unsigned അക്ഷരം)( bg * ba2 / 255 + fg );
          *s++ = (unsigned അക്ഷരം)( br * ba2 / 255 + fr );
          *s++ = (unsigned അക്ഷരം)( ba * ba2 / 255 + fa );
        }

        p += source->pitch;
        q += target->pitch;
      }
    }

    atarget_offset->x = fbx.xMin * 64;
    atarget_offset->y = fbx.yMax * 64;

  Error:
    എങ്കിൽ ( error && free_target_bitmap_on_error )
      FT_Bitmap_Done( library, target );

    എങ്കിൽ ( free_source_bitmap )
      FT_Bitmap_Done( library, &source_bitmap );

    തിരികെ error;
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_GlyphSlot_Own_Bitmap( FT_GlyphSlot  slot )
  {
    എങ്കിൽ ( slot && slot->format == FT_GLYPH_FORMAT_BITMAP   &&
         !( slot->internal->flags & FT_GLYPH_OWN_BITMAP ) )
    {
      FT_Bitmap  bitmap;
      FT_Error   error;


      FT_Bitmap_Init( &bitmap );
      error = FT_Bitmap_Copy( slot->library, &slot->bitmap, &bitmap );
      എങ്കിൽ ( error )
        തിരികെ error;

      slot->bitmap = bitmap;
      slot->internal->flags |= FT_GLYPH_OWN_BITMAP;
    }

    തിരികെ FT_Err_Ok;
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Bitmap_Done( FT_Library  library,
                  FT_Bitmap  *bitmap )
  {
    FT_Memory  memory;


    എങ്കിൽ ( !library )
      തിരികെ FT_THROW( Invalid_Library_Handle );

    എങ്കിൽ ( !bitmap )
      തിരികെ FT_THROW( Invalid_Argument );

    memory = library->memory;

    FT_FREE( bitmap->buffer );
    *bitmap = null_bitmap;

    തിരികെ FT_Err_Ok;
  }


/* END */
