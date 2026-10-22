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


#அந்தர்பாவ <freetype/internal/ftdebug.h>

#அந்தர்பாவ <freetype/ftbitmap.h>
#அந்தர்பாவ <freetype/ftimage.h>
#அந்தர்பாவ <freetype/internal/ftobjs.h>


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#undef  FT_COMPONENT
#define FT_COMPONENT  bitmap


  நிலையான
  மாறிலி FT_Bitmap  null_bitmap = { 0, 0, 0, வெற்று, 0, 0, 0, வெற்று };


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( வெறுமை )
  FT_Bitmap_Init( FT_Bitmap  *abitmap )
  {
    எனில் ( abitmap )
      *abitmap = null_bitmap;
  }


  /* deprecated function name; retained for ABI compatibility */

  FT_EXPORT_DEF( வெறுமை )
  FT_Bitmap_New( FT_Bitmap  *abitmap )
  {
    எனில் ( abitmap )
      *abitmap = null_bitmap;
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Bitmap_Copy( FT_Library        library,
                  மாறிலி FT_Bitmap  *source,
                  FT_Bitmap        *target)
  {
    FT_Memory  memory;
    FT_Error   error  = FT_Err_Ok;
    FT_Int     pitch;
    FT_Int     flip;


    எனில் ( !library )
      திரும்பு FT_THROW( Invalid_Library_Handle );

    எனில் ( !source || !target )
      திரும்பு FT_THROW( Invalid_Argument );

    எனில் ( source == target )
      திரும்பு FT_Err_Ok;

    flip = ( source->pitch < 0 && target->pitch > 0 ) ||
           ( source->pitch > 0 && target->pitch < 0 );

    memory = library->memory;
    FT_FREE( target->buffer );

    *target = *source;

    எனில் ( flip )
      target->pitch = -target->pitch;

    எனில் ( !source->buffer )
      திரும்பு FT_Err_Ok;

    pitch  = source->pitch;
    எனில் ( pitch < 0 )
      pitch = -pitch;

    FT_MEM_QALLOC_MULT( target->buffer, target->rows, pitch );

    எனில் ( !error )
    {
      எனில் ( flip )
      {
        /* take care of bitmap flow */
        FT_UInt   i;
        FT_Byte*  s = source->buffer;
        FT_Byte*  t = target->buffer;


        t += (FT_ULong)pitch * ( target->rows - 1 );

        க்கு ( i = target->rows; i > 0; i-- )
        {
          FT_ARRAY_COPY( t, s, pitch );

          s += pitch;
          t -= pitch;
        }
      }
      இல்லையெனில்
        FT_MEM_COPY( target->buffer, source->buffer,
                     (FT_Long)source->rows * pitch );
    }

    திரும்பு error;
  }


  /* Enlarge `bitmap' horizontally and vertically by `xpixels' */
  /* and `ypixels', respectively.                              */

  நிலையான FT_Error
  ft_bitmap_assure_buffer( FT_Memory   memory,
                           FT_Bitmap*  bitmap,
                           FT_UInt     xpixels,
                           FT_UInt     ypixels )
  {
    FT_Error        error;
    unsigned முழு    pitch;
    unsigned முழு    new_pitch;
    FT_UInt         bpp;
    FT_UInt         width, height;
    unsigned எழுத்து*  buffer = வெற்று;


    width  = bitmap->width;
    height = bitmap->rows;
    pitch  = (unsigned முழு)FT_ABS( bitmap->pitch );

    தேர்வு ( bitmap->pixel_mode )
    {
    நிலை FT_PIXEL_MODE_MONO:
      bpp       = 1;
      new_pitch = ( width + xpixels + 7 ) >> 3;
      நிறுத்து;
    நிலை FT_PIXEL_MODE_GRAY2:
      bpp       = 2;
      new_pitch = ( width + xpixels + 3 ) >> 2;
      நிறுத்து;
    நிலை FT_PIXEL_MODE_GRAY4:
      bpp       = 4;
      new_pitch = ( width + xpixels + 1 ) >> 1;
      நிறுத்து;
    நிலை FT_PIXEL_MODE_GRAY:
    நிலை FT_PIXEL_MODE_LCD:
    நிலை FT_PIXEL_MODE_LCD_V:
      bpp       = 8;
      new_pitch = width + xpixels;
      நிறுத்து;
    இயல்பு:
      திரும்பு FT_THROW( Invalid_Glyph_Format );
    }

    /* if no need to allocate memory */
    எனில் ( ypixels == 0 && new_pitch <= pitch )
    {
      /* zero the padding */
      FT_UInt  bit_width = pitch * 8;
      FT_UInt  bit_last  = ( width + xpixels ) * bpp;


      எனில் ( bit_last < bit_width )
      {
        FT_Byte*  வரி  = bitmap->buffer + ( bit_last >> 3 );
        FT_Byte*  end   = bitmap->buffer + pitch;
        FT_UInt   shift = bit_last & 7;
        FT_UInt   mask  = 0xFF00U >> shift;
        FT_UInt   மொத்தம் = height;


        க்கு ( ; மொத்தம் > 0; மொத்தம்--, வரி += pitch, end += pitch )
        {
          FT_Byte*  லிக = வரி;


          எனில் ( shift > 0 )
          {
            லிக[0] = (FT_Byte)( லிக[0] & mask );
            லிக++;
          }
          எனில் ( லிக < end )
            FT_MEM_ZERO( லிக, end - லிக );
        }
      }

      திரும்பு FT_Err_Ok;
    }

    /* otherwise allocate new buffer */
    எனில் ( FT_QALLOC_MULT( buffer, bitmap->rows + ypixels, new_pitch ) )
      திரும்பு error;

    /* new rows get added at the top of the bitmap, */
    /* thus take care of the flow direction         */
    எனில் ( bitmap->pitch > 0 )
    {
      FT_UInt  len = ( width * bpp + 7 ) >> 3;

      unsigned எழுத்து*  in  = bitmap->buffer;
      unsigned எழுத்து*  out = buffer;

      unsigned எழுத்து*  limit = bitmap->buffer + pitch * bitmap->rows;
      unsigned முழு    delta = new_pitch - len;


      FT_MEM_ZERO( out, new_pitch * ypixels );
      out += new_pitch * ypixels;

      போது ( in < limit )
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
    இல்லையெனில்
    {
      FT_UInt  len = ( width * bpp + 7 ) >> 3;

      unsigned எழுத்து*  in  = bitmap->buffer;
      unsigned எழுத்து*  out = buffer;

      unsigned எழுத்து*  limit = bitmap->buffer + pitch * bitmap->rows;
      unsigned முழு    delta = new_pitch - len;


      போது ( in < limit )
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
    எனில் ( bitmap->pitch < 0 )
      bitmap->pitch = -(முழு)new_pitch;
    இல்லையெனில்
      bitmap->pitch = (முழு)new_pitch;

    திரும்பு FT_Err_Ok;
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Bitmap_Embolden( FT_Library  library,
                      FT_Bitmap*  bitmap,
                      FT_Pos      xStrength,
                      FT_Pos      yStrength )
  {
    FT_Error        error;
    unsigned எழுத்து*  p;
    FT_Int          i, x, pitch;
    FT_UInt         y;
    FT_Int          xstr, ystr;


    எனில் ( !library )
      திரும்பு FT_THROW( Invalid_Library_Handle );

    எனில் ( !bitmap || !bitmap->buffer )
      திரும்பு FT_THROW( Invalid_Argument );

    எனில் ( ( ( FT_PIX_ROUND( xStrength ) >> 6 ) > FT_INT_MAX ) ||
         ( ( FT_PIX_ROUND( yStrength ) >> 6 ) > FT_INT_MAX ) )
      திரும்பு FT_THROW( Invalid_Argument );

    xstr = (FT_Int)FT_PIX_ROUND( xStrength ) >> 6;
    ystr = (FT_Int)FT_PIX_ROUND( yStrength ) >> 6;

    எனில் ( xstr == 0 && ystr == 0 )
      திரும்பு FT_Err_Ok;
    இல்லையெனில் எனில் ( xstr < 0 || ystr < 0 )
      திரும்பு FT_THROW( Invalid_Argument );

    தேர்வு ( bitmap->pixel_mode )
    {
    நிலை FT_PIXEL_MODE_GRAY2:
    நிலை FT_PIXEL_MODE_GRAY4:
      {
        FT_Bitmap  tmp;


        /* convert to 8bpp */
        FT_Bitmap_Init( &tmp );
        error = FT_Bitmap_Convert( library, bitmap, &tmp, 1 );
        எனில் ( error )
          திரும்பு error;

        FT_Bitmap_Done( library, bitmap );
        *bitmap = tmp;
      }
      நிறுத்து;

    நிலை FT_PIXEL_MODE_MONO:
      எனில் ( xstr > 8 )
        xstr = 8;
      நிறுத்து;

    நிலை FT_PIXEL_MODE_LCD:
      xstr *= 3;
      நிறுத்து;

    நிலை FT_PIXEL_MODE_LCD_V:
      ystr *= 3;
      நிறுத்து;

    நிலை FT_PIXEL_MODE_BGRA:
      /* We don't embolden color glyphs. */
      திரும்பு FT_Err_Ok;
    }

    error = ft_bitmap_assure_buffer( library->memory, bitmap,
                                     (FT_UInt)xstr, (FT_UInt)ystr );
    எனில் ( error )
      திரும்பு error;

    /* take care of bitmap flow */
    pitch = bitmap->pitch;
    எனில் ( pitch > 0 )
      p = bitmap->buffer + pitch * ystr;
    இல்லையெனில்
    {
      pitch = -pitch;
      p = bitmap->buffer + (FT_UInt)pitch * ( bitmap->rows - 1 );
    }

    /* for each row */
    க்கு ( y = 0; y < bitmap->rows; y++ )
    {
      /*
       * Horizontally:
       *
       * From the last pixel on, make each pixel or'ed with the
       * `xstr' pixels before it.
       */
      க்கு ( x = pitch - 1; x >= 0; x-- )
      {
        unsigned எழுத்து  tmp;


        tmp = p[x];
        க்கு ( i = 1; i <= xstr; i++ )
        {
          எனில் ( bitmap->pixel_mode == FT_PIXEL_MODE_MONO )
          {
            p[x] |= tmp >> i;

            /* the maximum value of 8 for `xstr' comes from here */
            எனில் ( x > 0 )
              p[x] |= p[x - 1] << ( 8 - i );

#எனில் 0
            எனில் ( p[x] == 0xFF )
              நிறுத்து;
#endif
          }
          இல்லையெனில்
          {
            எனில் ( x - i >= 0 )
            {
              எனில் ( p[x] + p[x - i] > bitmap->இலக்கம்_grays - 1 )
              {
                p[x] = (unsigned எழுத்து)( bitmap->இலக்கம்_grays - 1 );
                நிறுத்து;
              }
              இல்லையெனில்
              {
                p[x] = (unsigned எழுத்து)( p[x] + p[x - i] );
                எனில் ( p[x] == bitmap->இலக்கம்_grays - 1 )
                  நிறுத்து;
              }
            }
            இல்லையெனில்
              நிறுத்து;
          }
        }
      }

      /*
       * Vertically:
       *
       * Make the above `ystr' rows or'ed with it.
       */
      க்கு ( x = 1; x <= ystr; x++ )
      {
        unsigned எழுத்து*  q;


        q = p - bitmap->pitch * x;
        க்கு ( i = 0; i < pitch; i++ )
          q[i] |= p[i];
      }

      p += bitmap->pitch;
    }

    bitmap->width += (FT_UInt)xstr;
    bitmap->rows += (FT_UInt)ystr;

    திரும்பு FT_Err_Ok;
  }


  நிலையான FT_Byte
  ft_gray_for_premultiplied_srgb_bgra( மாறிலி FT_Byte*  bgra )
  {
    FT_UInt  a = bgra[3];
    FT_UInt  l;


    /* Short-circuit transparent color to avoid division by zero. */
    எனில் ( !a )
      திரும்பு 0;

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

    திரும்பு (FT_Byte)( a - l / a );
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Bitmap_Convert( FT_Library        library,
                     மாறிலி FT_Bitmap  *source,
                     FT_Bitmap        *target,
                     FT_Int            alignment )
  {
    FT_Error   error = FT_Err_Ok;
    FT_Memory  memory;

    FT_Byte*  s;
    FT_Byte*  t;


    எனில் ( !library )
      திரும்பு FT_THROW( Invalid_Library_Handle );

    எனில் ( !source || !target )
      திரும்பு FT_THROW( Invalid_Argument );

    memory = library->memory;

    தேர்வு ( source->pixel_mode )
    {
    நிலை FT_PIXEL_MODE_MONO:
    நிலை FT_PIXEL_MODE_GRAY:
    நிலை FT_PIXEL_MODE_GRAY2:
    நிலை FT_PIXEL_MODE_GRAY4:
    நிலை FT_PIXEL_MODE_LCD:
    நிலை FT_PIXEL_MODE_LCD_V:
    நிலை FT_PIXEL_MODE_BGRA:
      {
        FT_Int  width = (FT_Int)source->width;
        FT_Int  neg   = ( target->pitch == 0 && source->pitch < 0 ) ||
                          target->pitch  < 0;


        FT_Bitmap_Done( library, target );

        target->pixel_mode = FT_PIXEL_MODE_GRAY;
        target->rows       = source->rows;
        target->width      = source->width;

        எனில் ( alignment )
        {
          FT_Int  rem = width % alignment;


          எனில் ( rem )
            width = alignment > 0 ? width - rem + alignment
                                  : width - rem - alignment;
        }

        எனில் ( FT_QALLOC_MULT( target->buffer, target->rows, width ) )
          திரும்பு error;

        target->pitch = neg ? -width : width;
      }
      நிறுத்து;

    இயல்பு:
      error = FT_THROW( Invalid_Argument );
    }

    s = source->buffer;
    t = target->buffer;

    /* take care of bitmap flow */
    எனில் ( source->pitch < 0 )
      s -= source->pitch * (FT_Int)( source->rows - 1 );
    எனில் ( target->pitch < 0 )
      t -= target->pitch * (FT_Int)( target->rows - 1 );

    தேர்வு ( source->pixel_mode )
    {
    நிலை FT_PIXEL_MODE_MONO:
      {
        FT_UInt  i;


        target->இலக்கம்_grays = 2;

        க்கு ( i = source->rows; i > 0; i-- )
        {
          FT_Byte*  ss = s;
          FT_Byte*  tt = t;
          FT_UInt   j;


          /* get the full bytes */
          க்கு ( j = source->width >> 3; j > 0; j-- )
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
          எனில் ( j > 0 )
          {
            FT_Int  val = *ss;


            க்கு ( ; j > 0; j-- )
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
      நிறுத்து;


    நிலை FT_PIXEL_MODE_GRAY:
    நிலை FT_PIXEL_MODE_LCD:
    நிலை FT_PIXEL_MODE_LCD_V:
      {
        FT_UInt  width = source->width;
        FT_UInt  i;


        target->இலக்கம்_grays = 256;

        க்கு ( i = source->rows; i > 0; i-- )
        {
          FT_ARRAY_COPY( t, s, width );

          s += source->pitch;
          t += target->pitch;
        }
      }
      நிறுத்து;


    நிலை FT_PIXEL_MODE_GRAY2:
      {
        FT_UInt  i;


        target->இலக்கம்_grays = 4;

        க்கு ( i = source->rows; i > 0; i-- )
        {
          FT_Byte*  ss = s;
          FT_Byte*  tt = t;
          FT_UInt   j;


          /* get the full bytes */
          க்கு ( j = source->width >> 2; j > 0; j-- )
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
          எனில் ( j > 0 )
          {
            FT_Int  val = ss[0];


            க்கு ( ; j > 0; j-- )
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
      நிறுத்து;


    நிலை FT_PIXEL_MODE_GRAY4:
      {
        FT_UInt  i;


        target->இலக்கம்_grays = 16;

        க்கு ( i = source->rows; i > 0; i-- )
        {
          FT_Byte*  ss = s;
          FT_Byte*  tt = t;
          FT_UInt   j;


          /* get the full bytes */
          க்கு ( j = source->width >> 1; j > 0; j-- )
          {
            FT_Int  val = ss[0];


            tt[0] = (FT_Byte)( ( val & 0xF0 ) >> 4 );
            tt[1] = (FT_Byte)( ( val & 0x0F ) );

            ss += 1;
            tt += 2;
          }

          எனில் ( source->width & 1 )
            tt[0] = (FT_Byte)( ( ss[0] & 0xF0 ) >> 4 );

          s += source->pitch;
          t += target->pitch;
        }
      }
      நிறுத்து;


    நிலை FT_PIXEL_MODE_BGRA:
      {
        FT_UInt  i;


        target->இலக்கம்_grays = 256;

        க்கு ( i = source->rows; i > 0; i-- )
        {
          FT_Byte*  ss = s;
          FT_Byte*  tt = t;
          FT_UInt   j;


          க்கு ( j = source->width; j > 0; j-- )
          {
            tt[0] = ft_gray_for_premultiplied_srgb_bgra( ss );

            ss += 4;
            tt += 1;
          }

          s += source->pitch;
          t += target->pitch;
        }
      }
      நிறுத்து;

    இயல்பு:
      ;
    }

    திரும்பு error;
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Bitmap_Blend( FT_Library        library,
                   மாறிலி FT_Bitmap*  source_,
                   மாறிலி FT_Vector   source_offset_,
                   FT_Bitmap*        target,
                   FT_Vector        *atarget_offset,
                   FT_Color          color )
  {
    FT_Error   error = FT_Err_Ok;
    FT_Memory  memory;

    FT_Bitmap         source_bitmap;
    மாறிலி FT_Bitmap*  source;

    FT_Bool  free_source_bitmap          = 0;
    FT_Bool  free_target_bitmap_on_error = 0;

    FT_BBox  sbx, tbx, fbx;

    unsigned முழு  final_rows, final_width;


    எனில் ( !library || !target || !source_ || !atarget_offset )
      திரும்பு FT_THROW( Invalid_Argument );

    memory = library->memory;

    எனில் ( !( target->pixel_mode == FT_PIXEL_MODE_NONE     ||
            ( target->pixel_mode == FT_PIXEL_MODE_BGRA &&
              target->buffer                           ) ) )
      திரும்பு FT_THROW( Invalid_Argument );

    எனில் ( source_->pixel_mode == FT_PIXEL_MODE_NONE )
      திரும்பு FT_Err_Ok;               /* nothing to do */

    /* pitches must have the same sign */
    எனில் ( target->pixel_mode == FT_PIXEL_MODE_BGRA &&
         ( source_->pitch ^ target->pitch ) < 0   )
      திரும்பு FT_THROW( Invalid_Argument );

    எனில் ( !( source_->width && source_->rows ) )
      திரும்பு FT_Err_Ok;               /* nothing to do */

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
    எனில் ( sbx.xMin > sbx.xMax      || sbx.yMin > sbx.yMax     ||
         source_->width > 0x7FFFU || source_->rows > 0x7FFFU )
    {
      FT_TRACE5(( "FT_Bitmap_Blend: source dimension overflow\n" ));
      திரும்பு FT_THROW( Invalid_Argument );
    }

    எனில் ( !target->width || !target->rows )
    {
      FT_TRACE5(( "  target bitmap: empty\n" ));

      final_width = source_->width;
      final_rows  = source_->rows;

      /* create new empty bitmap */
      target->width      = final_width;
      target->rows       = final_rows;
      target->pixel_mode = FT_PIXEL_MODE_BGRA;
      target->pitch      = (முழு)final_width * 4;
      target->இலக்கம்_grays  = 256;

      எனில் ( FT_ALLOC_MULT( target->buffer, target->rows, target->pitch ) )
        திரும்பு error;

      free_target_bitmap_on_error = 1;

      fbx = sbx;
    }
    இல்லையெனில்
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
      எனில் ( tbx.xMin > tbx.xMax || tbx.yMin > tbx.yMax )
      {
        FT_TRACE5(( "FT_Bitmap_Blend: targget dimension overflow\n" ));
        திரும்பு FT_THROW( Invalid_Argument );
      }

      /* compute final bitmap dimensions */
      fbx.xMin = FT_MIN( sbx.xMin, tbx.xMin );
      fbx.yMin = FT_MIN( sbx.yMin, tbx.yMin );
      fbx.xMax = FT_MAX( sbx.xMax, tbx.xMax );
      fbx.yMax = FT_MAX( sbx.yMax, tbx.yMax );

      /* sanity check */
      எனில் ( fbx.xMin < -0x10000 || fbx.xMax >= 0x10000 ||
           fbx.yMin < -0x10000 || fbx.yMax >= 0x10000 )
      {
        FT_TRACE5(( "FT_Bitmap_Blend: final dimension overflow\n" ));
        திரும்பு FT_THROW( Invalid_Argument );
      }

      final_width = fbx.xMax - fbx.xMin;
      final_rows  = fbx.yMax - fbx.yMin;

      /* adjust target bitmap to enlarged size */
      எனில் ( target->width < final_width ||
           target->rows  < final_rows  )
      {
        முழு  pitch, new_pitch;

        unsigned எழுத்து*  buffer = வெற்று;


        pitch = target->pitch;

        எனில் ( pitch < 0 )
          pitch = -pitch;

        new_pitch = (முழு)final_width * 4;

        /* TODO: provide an in-buffer solution for large bitmaps */
        /*       to avoid allocation of a new buffer             */
        எனில் ( FT_ALLOC_MULT( buffer, final_rows, new_pitch ) )
          செல் Error;

        /* copy data to new buffer */

        /* the bitmap flow is from top to bottom, */
        /* but y is measured from bottom to top   */
        எனில் ( target->pitch < 0 )
        {
          /* XXX */
        }
        இல்லையெனில்
        {
          FT_Pos  x = tbx.xMin - fbx.xMin;
          FT_Pos  y = tbx.yMin - fbx.yMin;

          unsigned எழுத்து*  p =
            target->buffer;
          unsigned எழுத்து*  q =
            buffer +
            ( final_rows - y - target->rows ) * new_pitch +
            x * 4;
          unsigned எழுத்து*  limit_p =
            p + pitch * (முழு)target->rows;


          போது ( p < limit_p )
          {
            FT_MEM_COPY( q, p, pitch );

            p += pitch;
            q += new_pitch;
          }
        }

        FT_FREE( target->buffer );

        target->width = final_width;
        target->rows  = final_rows;

        எனில் ( target->pitch < 0 )
          target->pitch = -new_pitch;
        இல்லையெனில்
          target->pitch = new_pitch;

        target->buffer = buffer;
      }
    }

    FT_TRACE5(( "  final bitmap: (%ld, %ld) -- (%ld, %ld); %u x %u\n",
      fbx.xMin, fbx.yMin, fbx.xMax, fbx.yMax,
      final_width, final_rows ));

    /* adjust source bitmap if necessary */
    எனில் ( source_->pixel_mode != FT_PIXEL_MODE_GRAY )
    {
      FT_Bitmap_Init( &source_bitmap );
      error = FT_Bitmap_Convert( library, source_, &source_bitmap, 1 );
      எனில் ( error )
        செல் Error;

      source             = &source_bitmap;
      free_source_bitmap = 1;
    }
    இல்லையெனில்
      source = source_;

    /* do blending; the code below returns pre-multiplied channels, */
    /* similar to what FreeType gets from `CBDT' tables             */

    /* the bitmap flow is from top to bottom, */
    /* but y is measured from bottom to top   */
    எனில் ( target->pitch < 0 )
    {
      /* XXX */
    }
    இல்லையெனில்
    {
      FT_Pos  x = sbx.xMin - fbx.xMin;
      FT_Pos  y = sbx.yMin - fbx.yMin;

      unsigned எழுத்து*  p =
        source->buffer;
      unsigned எழுத்து*  q =
        target->buffer +
        ( target->rows - y - source->rows ) * target->pitch +
        x * 4;
      unsigned எழுத்து*  limit_p =
        p + source->pitch * (முழு)source->rows;


      போது ( p < limit_p )
      {
        unsigned எழுத்து*  r       = p;
        unsigned எழுத்து*  s       = q;
        unsigned எழுத்து*  limit_r = r + source->width;


        போது ( r < limit_r )
        {
          முழு  aa = *r++;
          முழு  fa = color.alpha * aa / 255;

          முழு  fb = color.blue * fa / 255;
          முழு  fg = color.green * fa / 255;
          முழு  fr = color.red * fa / 255;

          முழு  ba2 = 255 - fa;

          முழு  bb = s[0];
          முழு  bg = s[1];
          முழு  br = s[2];
          முழு  ba = s[3];


          *s++ = (unsigned எழுத்து)( bb * ba2 / 255 + fb );
          *s++ = (unsigned எழுத்து)( bg * ba2 / 255 + fg );
          *s++ = (unsigned எழுத்து)( br * ba2 / 255 + fr );
          *s++ = (unsigned எழுத்து)( ba * ba2 / 255 + fa );
        }

        p += source->pitch;
        q += target->pitch;
      }
    }

    atarget_offset->x = fbx.xMin * 64;
    atarget_offset->y = fbx.yMax * 64;

  Error:
    எனில் ( error && free_target_bitmap_on_error )
      FT_Bitmap_Done( library, target );

    எனில் ( free_source_bitmap )
      FT_Bitmap_Done( library, &source_bitmap );

    திரும்பு error;
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_GlyphSlot_Own_Bitmap( FT_GlyphSlot  slot )
  {
    எனில் ( slot && slot->format == FT_GLYPH_FORMAT_BITMAP   &&
         !( slot->internal->flags & FT_GLYPH_OWN_BITMAP ) )
    {
      FT_Bitmap  bitmap;
      FT_Error   error;


      FT_Bitmap_Init( &bitmap );
      error = FT_Bitmap_Copy( slot->library, &slot->bitmap, &bitmap );
      எனில் ( error )
        திரும்பு error;

      slot->bitmap = bitmap;
      slot->internal->flags |= FT_GLYPH_OWN_BITMAP;
    }

    திரும்பு FT_Err_Ok;
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Bitmap_Done( FT_Library  library,
                  FT_Bitmap  *bitmap )
  {
    FT_Memory  memory;


    எனில் ( !library )
      திரும்பு FT_THROW( Invalid_Library_Handle );

    எனில் ( !bitmap )
      திரும்பு FT_THROW( Invalid_Argument );

    memory = library->memory;

    FT_FREE( bitmap->buffer );
    *bitmap = null_bitmap;

    திரும்பு FT_Err_Ok;
  }


/* END */
