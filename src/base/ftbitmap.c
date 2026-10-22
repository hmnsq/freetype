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


#ضمن <freetype/internal/ftdebug.h>

#ضمن <freetype/ftbitmap.h>
#ضمن <freetype/ftimage.h>
#ضمن <freetype/internal/ftobjs.h>


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#undef  FT_COMPONENT
#define FT_COMPONENT  bitmap


  ساكن
  ثابت FT_Bitmap  null_bitmap = { 0, 0, 0, عدم, 0, 0, 0, عدم };


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( فراغ )
  FT_Bitmap_Init( FT_Bitmap  *abitmap )
  {
    إذا ( abitmap )
      *abitmap = null_bitmap;
  }


  /* deprecated function name; retained for ABI compatibility */

  FT_EXPORT_DEF( فراغ )
  FT_Bitmap_New( FT_Bitmap  *abitmap )
  {
    إذا ( abitmap )
      *abitmap = null_bitmap;
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Bitmap_Copy( FT_Library        library,
                  ثابت FT_Bitmap  *source,
                  FT_Bitmap        *target)
  {
    FT_Memory  memory;
    FT_Error   error  = FT_Err_Ok;
    FT_Int     pitch;
    FT_Int     flip;


    إذا ( !library )
      إرجاع FT_THROW( Invalid_Library_Handle );

    إذا ( !source || !target )
      إرجاع FT_THROW( Invalid_Argument );

    إذا ( source == target )
      إرجاع FT_Err_Ok;

    flip = ( source->pitch < 0 && target->pitch > 0 ) ||
           ( source->pitch > 0 && target->pitch < 0 );

    memory = library->memory;
    FT_FREE( target->buffer );

    *target = *source;

    إذا ( flip )
      target->pitch = -target->pitch;

    إذا ( !source->buffer )
      إرجاع FT_Err_Ok;

    pitch  = source->pitch;
    إذا ( pitch < 0 )
      pitch = -pitch;

    FT_MEM_QALLOC_MULT( target->buffer, target->rows, pitch );

    إذا ( !error )
    {
      إذا ( flip )
      {
        /* take care of bitmap flow */
        FT_UInt   i;
        FT_Byte*  s = source->buffer;
        FT_Byte*  t = target->buffer;


        t += (FT_ULong)pitch * ( target->rows - 1 );

        لكل ( i = target->rows; i > 0; i-- )
        {
          FT_ARRAY_COPY( t, s, pitch );

          s += pitch;
          t -= pitch;
        }
      }
      وإلا
        FT_MEM_COPY( target->buffer, source->buffer,
                     (FT_Long)source->rows * pitch );
    }

    إرجاع error;
  }


  /* Enlarge `bitmap' horizontally and vertically by `xpixels' */
  /* and `ypixels', respectively.                              */

  ساكن FT_Error
  ft_bitmap_assure_buffer( FT_Memory   memory,
                           FT_Bitmap*  bitmap,
                           FT_UInt     xpixels,
                           FT_UInt     ypixels )
  {
    FT_Error        error;
    unsigned صحيح    pitch;
    unsigned صحيح    new_pitch;
    FT_UInt         bpp;
    FT_UInt         width, height;
    unsigned حرف*  buffer = عدم;


    width  = bitmap->width;
    height = bitmap->rows;
    pitch  = (unsigned صحيح)FT_ABS( bitmap->pitch );

    بدل ( bitmap->pixel_mode )
    {
    حالة FT_PIXEL_MODE_MONO:
      bpp       = 1;
      new_pitch = ( width + xpixels + 7 ) >> 3;
      اكسر;
    حالة FT_PIXEL_MODE_GRAY2:
      bpp       = 2;
      new_pitch = ( width + xpixels + 3 ) >> 2;
      اكسر;
    حالة FT_PIXEL_MODE_GRAY4:
      bpp       = 4;
      new_pitch = ( width + xpixels + 1 ) >> 1;
      اكسر;
    حالة FT_PIXEL_MODE_GRAY:
    حالة FT_PIXEL_MODE_LCD:
    حالة FT_PIXEL_MODE_LCD_V:
      bpp       = 8;
      new_pitch = width + xpixels;
      اكسر;
    افتراضي:
      إرجاع FT_THROW( Invalid_Glyph_Format );
    }

    /* if no need to allocate memory */
    إذا ( ypixels == 0 && new_pitch <= pitch )
    {
      /* zero the padding */
      FT_UInt  bit_width = pitch * 8;
      FT_UInt  bit_last  = ( width + xpixels ) * bpp;


      إذا ( bit_last < bit_width )
      {
        FT_Byte*  سطر  = bitmap->buffer + ( bit_last >> 3 );
        FT_Byte*  end   = bitmap->buffer + pitch;
        FT_UInt   shift = bit_last & 7;
        FT_UInt   mask  = 0xFF00U >> shift;
        FT_UInt   تعداد_2 = height;


        لكل ( ; تعداد_2 > 0; تعداد_2--, سطر += pitch, end += pitch )
        {
          FT_Byte*  اكتب = سطر;


          إذا ( shift > 0 )
          {
            اكتب[0] = (FT_Byte)( اكتب[0] & mask );
            اكتب++;
          }
          إذا ( اكتب < end )
            FT_MEM_ZERO( اكتب, end - اكتب );
        }
      }

      إرجاع FT_Err_Ok;
    }

    /* otherwise allocate new buffer */
    إذا ( FT_QALLOC_MULT( buffer, bitmap->rows + ypixels, new_pitch ) )
      إرجاع error;

    /* new rows get added at the top of the bitmap, */
    /* thus take care of the flow direction         */
    إذا ( bitmap->pitch > 0 )
    {
      FT_UInt  len = ( width * bpp + 7 ) >> 3;

      unsigned حرف*  in  = bitmap->buffer;
      unsigned حرف*  out = buffer;

      unsigned حرف*  limit = bitmap->buffer + pitch * bitmap->rows;
      unsigned صحيح    delta = new_pitch - len;


      FT_MEM_ZERO( out, new_pitch * ypixels );
      out += new_pitch * ypixels;

      بينما ( in < limit )
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
    وإلا
    {
      FT_UInt  len = ( width * bpp + 7 ) >> 3;

      unsigned حرف*  in  = bitmap->buffer;
      unsigned حرف*  out = buffer;

      unsigned حرف*  limit = bitmap->buffer + pitch * bitmap->rows;
      unsigned صحيح    delta = new_pitch - len;


      بينما ( in < limit )
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
    إذا ( bitmap->pitch < 0 )
      bitmap->pitch = -(صحيح)new_pitch;
    وإلا
      bitmap->pitch = (صحيح)new_pitch;

    إرجاع FT_Err_Ok;
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Bitmap_Embolden( FT_Library  library,
                      FT_Bitmap*  bitmap,
                      FT_Pos      xStrength,
                      FT_Pos      yStrength )
  {
    FT_Error        error;
    unsigned حرف*  p;
    FT_Int          i, x, pitch;
    FT_UInt         y;
    FT_Int          xstr, ystr;


    إذا ( !library )
      إرجاع FT_THROW( Invalid_Library_Handle );

    إذا ( !bitmap || !bitmap->buffer )
      إرجاع FT_THROW( Invalid_Argument );

    إذا ( ( ( FT_PIX_ROUND( xStrength ) >> 6 ) > FT_INT_MAX ) ||
         ( ( FT_PIX_ROUND( yStrength ) >> 6 ) > FT_INT_MAX ) )
      إرجاع FT_THROW( Invalid_Argument );

    xstr = (FT_Int)FT_PIX_ROUND( xStrength ) >> 6;
    ystr = (FT_Int)FT_PIX_ROUND( yStrength ) >> 6;

    إذا ( xstr == 0 && ystr == 0 )
      إرجاع FT_Err_Ok;
    وإلا إذا ( xstr < 0 || ystr < 0 )
      إرجاع FT_THROW( Invalid_Argument );

    بدل ( bitmap->pixel_mode )
    {
    حالة FT_PIXEL_MODE_GRAY2:
    حالة FT_PIXEL_MODE_GRAY4:
      {
        FT_Bitmap  tmp;


        /* convert to 8bpp */
        FT_Bitmap_Init( &tmp );
        error = FT_Bitmap_Convert( library, bitmap, &tmp, 1 );
        إذا ( error )
          إرجاع error;

        FT_Bitmap_Done( library, bitmap );
        *bitmap = tmp;
      }
      اكسر;

    حالة FT_PIXEL_MODE_MONO:
      إذا ( xstr > 8 )
        xstr = 8;
      اكسر;

    حالة FT_PIXEL_MODE_LCD:
      xstr *= 3;
      اكسر;

    حالة FT_PIXEL_MODE_LCD_V:
      ystr *= 3;
      اكسر;

    حالة FT_PIXEL_MODE_BGRA:
      /* We don't embolden color glyphs. */
      إرجاع FT_Err_Ok;
    }

    error = ft_bitmap_assure_buffer( library->memory, bitmap,
                                     (FT_UInt)xstr, (FT_UInt)ystr );
    إذا ( error )
      إرجاع error;

    /* take care of bitmap flow */
    pitch = bitmap->pitch;
    إذا ( pitch > 0 )
      p = bitmap->buffer + pitch * ystr;
    وإلا
    {
      pitch = -pitch;
      p = bitmap->buffer + (FT_UInt)pitch * ( bitmap->rows - 1 );
    }

    /* for each row */
    لكل ( y = 0; y < bitmap->rows; y++ )
    {
      /*
       * Horizontally:
       *
       * From the last pixel on, make each pixel or'ed with the
       * `xstr' pixels before it.
       */
      لكل ( x = pitch - 1; x >= 0; x-- )
      {
        unsigned حرف  tmp;


        tmp = p[x];
        لكل ( i = 1; i <= xstr; i++ )
        {
          إذا ( bitmap->pixel_mode == FT_PIXEL_MODE_MONO )
          {
            p[x] |= tmp >> i;

            /* the maximum value of 8 for `xstr' comes from here */
            إذا ( x > 0 )
              p[x] |= p[x - 1] << ( 8 - i );

#إذا 0
            إذا ( p[x] == 0xFF )
              اكسر;
#endif
          }
          وإلا
          {
            إذا ( x - i >= 0 )
            {
              إذا ( p[x] + p[x - i] > bitmap->رقم_grays - 1 )
              {
                p[x] = (unsigned حرف)( bitmap->رقم_grays - 1 );
                اكسر;
              }
              وإلا
              {
                p[x] = (unsigned حرف)( p[x] + p[x - i] );
                إذا ( p[x] == bitmap->رقم_grays - 1 )
                  اكسر;
              }
            }
            وإلا
              اكسر;
          }
        }
      }

      /*
       * Vertically:
       *
       * Make the above `ystr' rows or'ed with it.
       */
      لكل ( x = 1; x <= ystr; x++ )
      {
        unsigned حرف*  q;


        q = p - bitmap->pitch * x;
        لكل ( i = 0; i < pitch; i++ )
          q[i] |= p[i];
      }

      p += bitmap->pitch;
    }

    bitmap->width += (FT_UInt)xstr;
    bitmap->rows += (FT_UInt)ystr;

    إرجاع FT_Err_Ok;
  }


  ساكن FT_Byte
  ft_gray_for_premultiplied_srgb_bgra( ثابت FT_Byte*  bgra )
  {
    FT_UInt  a = bgra[3];
    FT_UInt  l;


    /* Short-circuit transparent color to avoid division by zero. */
    إذا ( !a )
      إرجاع 0;

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

    إرجاع (FT_Byte)( a - l / a );
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Bitmap_Convert( FT_Library        library,
                     ثابت FT_Bitmap  *source,
                     FT_Bitmap        *target,
                     FT_Int            alignment )
  {
    FT_Error   error = FT_Err_Ok;
    FT_Memory  memory;

    FT_Byte*  s;
    FT_Byte*  t;


    إذا ( !library )
      إرجاع FT_THROW( Invalid_Library_Handle );

    إذا ( !source || !target )
      إرجاع FT_THROW( Invalid_Argument );

    memory = library->memory;

    بدل ( source->pixel_mode )
    {
    حالة FT_PIXEL_MODE_MONO:
    حالة FT_PIXEL_MODE_GRAY:
    حالة FT_PIXEL_MODE_GRAY2:
    حالة FT_PIXEL_MODE_GRAY4:
    حالة FT_PIXEL_MODE_LCD:
    حالة FT_PIXEL_MODE_LCD_V:
    حالة FT_PIXEL_MODE_BGRA:
      {
        FT_Int  width = (FT_Int)source->width;
        FT_Int  neg   = ( target->pitch == 0 && source->pitch < 0 ) ||
                          target->pitch  < 0;


        FT_Bitmap_Done( library, target );

        target->pixel_mode = FT_PIXEL_MODE_GRAY;
        target->rows       = source->rows;
        target->width      = source->width;

        إذا ( alignment )
        {
          FT_Int  rem = width % alignment;


          إذا ( rem )
            width = alignment > 0 ? width - rem + alignment
                                  : width - rem - alignment;
        }

        إذا ( FT_QALLOC_MULT( target->buffer, target->rows, width ) )
          إرجاع error;

        target->pitch = neg ? -width : width;
      }
      اكسر;

    افتراضي:
      error = FT_THROW( Invalid_Argument );
    }

    s = source->buffer;
    t = target->buffer;

    /* take care of bitmap flow */
    إذا ( source->pitch < 0 )
      s -= source->pitch * (FT_Int)( source->rows - 1 );
    إذا ( target->pitch < 0 )
      t -= target->pitch * (FT_Int)( target->rows - 1 );

    بدل ( source->pixel_mode )
    {
    حالة FT_PIXEL_MODE_MONO:
      {
        FT_UInt  i;


        target->رقم_grays = 2;

        لكل ( i = source->rows; i > 0; i-- )
        {
          FT_Byte*  ss = s;
          FT_Byte*  tt = t;
          FT_UInt   j;


          /* get the full bytes */
          لكل ( j = source->width >> 3; j > 0; j-- )
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
          إذا ( j > 0 )
          {
            FT_Int  val = *ss;


            لكل ( ; j > 0; j-- )
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
      اكسر;


    حالة FT_PIXEL_MODE_GRAY:
    حالة FT_PIXEL_MODE_LCD:
    حالة FT_PIXEL_MODE_LCD_V:
      {
        FT_UInt  width = source->width;
        FT_UInt  i;


        target->رقم_grays = 256;

        لكل ( i = source->rows; i > 0; i-- )
        {
          FT_ARRAY_COPY( t, s, width );

          s += source->pitch;
          t += target->pitch;
        }
      }
      اكسر;


    حالة FT_PIXEL_MODE_GRAY2:
      {
        FT_UInt  i;


        target->رقم_grays = 4;

        لكل ( i = source->rows; i > 0; i-- )
        {
          FT_Byte*  ss = s;
          FT_Byte*  tt = t;
          FT_UInt   j;


          /* get the full bytes */
          لكل ( j = source->width >> 2; j > 0; j-- )
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
          إذا ( j > 0 )
          {
            FT_Int  val = ss[0];


            لكل ( ; j > 0; j-- )
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
      اكسر;


    حالة FT_PIXEL_MODE_GRAY4:
      {
        FT_UInt  i;


        target->رقم_grays = 16;

        لكل ( i = source->rows; i > 0; i-- )
        {
          FT_Byte*  ss = s;
          FT_Byte*  tt = t;
          FT_UInt   j;


          /* get the full bytes */
          لكل ( j = source->width >> 1; j > 0; j-- )
          {
            FT_Int  val = ss[0];


            tt[0] = (FT_Byte)( ( val & 0xF0 ) >> 4 );
            tt[1] = (FT_Byte)( ( val & 0x0F ) );

            ss += 1;
            tt += 2;
          }

          إذا ( source->width & 1 )
            tt[0] = (FT_Byte)( ( ss[0] & 0xF0 ) >> 4 );

          s += source->pitch;
          t += target->pitch;
        }
      }
      اكسر;


    حالة FT_PIXEL_MODE_BGRA:
      {
        FT_UInt  i;


        target->رقم_grays = 256;

        لكل ( i = source->rows; i > 0; i-- )
        {
          FT_Byte*  ss = s;
          FT_Byte*  tt = t;
          FT_UInt   j;


          لكل ( j = source->width; j > 0; j-- )
          {
            tt[0] = ft_gray_for_premultiplied_srgb_bgra( ss );

            ss += 4;
            tt += 1;
          }

          s += source->pitch;
          t += target->pitch;
        }
      }
      اكسر;

    افتراضي:
      ;
    }

    إرجاع error;
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Bitmap_Blend( FT_Library        library,
                   ثابت FT_Bitmap*  source_,
                   ثابت FT_Vector   source_offset_,
                   FT_Bitmap*        target,
                   FT_Vector        *atarget_offset,
                   FT_Color          color )
  {
    FT_Error   error = FT_Err_Ok;
    FT_Memory  memory;

    FT_Bitmap         source_bitmap;
    ثابت FT_Bitmap*  source;

    FT_Bool  free_source_bitmap          = 0;
    FT_Bool  free_target_bitmap_on_error = 0;

    FT_BBox  sbx, tbx, fbx;

    unsigned صحيح  final_rows, final_width;


    إذا ( !library || !target || !source_ || !atarget_offset )
      إرجاع FT_THROW( Invalid_Argument );

    memory = library->memory;

    إذا ( !( target->pixel_mode == FT_PIXEL_MODE_NONE     ||
            ( target->pixel_mode == FT_PIXEL_MODE_BGRA &&
              target->buffer                           ) ) )
      إرجاع FT_THROW( Invalid_Argument );

    إذا ( source_->pixel_mode == FT_PIXEL_MODE_NONE )
      إرجاع FT_Err_Ok;               /* nothing to do */

    /* pitches must have the same sign */
    إذا ( target->pixel_mode == FT_PIXEL_MODE_BGRA &&
         ( source_->pitch ^ target->pitch ) < 0   )
      إرجاع FT_THROW( Invalid_Argument );

    إذا ( !( source_->width && source_->rows ) )
      إرجاع FT_Err_Ok;               /* nothing to do */

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
    إذا ( sbx.xMin > sbx.xMax      || sbx.yMin > sbx.yMax     ||
         source_->width > 0x7FFFU || source_->rows > 0x7FFFU )
    {
      FT_TRACE5(( "FT_Bitmap_Blend: source dimension overflow\n" ));
      إرجاع FT_THROW( Invalid_Argument );
    }

    إذا ( !target->width || !target->rows )
    {
      FT_TRACE5(( "  target bitmap: empty\n" ));

      final_width = source_->width;
      final_rows  = source_->rows;

      /* create new empty bitmap */
      target->width      = final_width;
      target->rows       = final_rows;
      target->pixel_mode = FT_PIXEL_MODE_BGRA;
      target->pitch      = (صحيح)final_width * 4;
      target->رقم_grays  = 256;

      إذا ( FT_ALLOC_MULT( target->buffer, target->rows, target->pitch ) )
        إرجاع error;

      free_target_bitmap_on_error = 1;

      fbx = sbx;
    }
    وإلا
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
      إذا ( tbx.xMin > tbx.xMax || tbx.yMin > tbx.yMax )
      {
        FT_TRACE5(( "FT_Bitmap_Blend: targget dimension overflow\n" ));
        إرجاع FT_THROW( Invalid_Argument );
      }

      /* compute final bitmap dimensions */
      fbx.xMin = FT_MIN( sbx.xMin, tbx.xMin );
      fbx.yMin = FT_MIN( sbx.yMin, tbx.yMin );
      fbx.xMax = FT_MAX( sbx.xMax, tbx.xMax );
      fbx.yMax = FT_MAX( sbx.yMax, tbx.yMax );

      /* sanity check */
      إذا ( fbx.xMin < -0x10000 || fbx.xMax >= 0x10000 ||
           fbx.yMin < -0x10000 || fbx.yMax >= 0x10000 )
      {
        FT_TRACE5(( "FT_Bitmap_Blend: final dimension overflow\n" ));
        إرجاع FT_THROW( Invalid_Argument );
      }

      final_width = fbx.xMax - fbx.xMin;
      final_rows  = fbx.yMax - fbx.yMin;

      /* adjust target bitmap to enlarged size */
      إذا ( target->width < final_width ||
           target->rows  < final_rows  )
      {
        صحيح  pitch, new_pitch;

        unsigned حرف*  buffer = عدم;


        pitch = target->pitch;

        إذا ( pitch < 0 )
          pitch = -pitch;

        new_pitch = (صحيح)final_width * 4;

        /* TODO: provide an in-buffer solution for large bitmaps */
        /*       to avoid allocation of a new buffer             */
        إذا ( FT_ALLOC_MULT( buffer, final_rows, new_pitch ) )
          اذهب Error;

        /* copy data to new buffer */

        /* the bitmap flow is from top to bottom, */
        /* but y is measured from bottom to top   */
        إذا ( target->pitch < 0 )
        {
          /* XXX */
        }
        وإلا
        {
          FT_Pos  x = tbx.xMin - fbx.xMin;
          FT_Pos  y = tbx.yMin - fbx.yMin;

          unsigned حرف*  p =
            target->buffer;
          unsigned حرف*  q =
            buffer +
            ( final_rows - y - target->rows ) * new_pitch +
            x * 4;
          unsigned حرف*  limit_p =
            p + pitch * (صحيح)target->rows;


          بينما ( p < limit_p )
          {
            FT_MEM_COPY( q, p, pitch );

            p += pitch;
            q += new_pitch;
          }
        }

        FT_FREE( target->buffer );

        target->width = final_width;
        target->rows  = final_rows;

        إذا ( target->pitch < 0 )
          target->pitch = -new_pitch;
        وإلا
          target->pitch = new_pitch;

        target->buffer = buffer;
      }
    }

    FT_TRACE5(( "  final bitmap: (%ld, %ld) -- (%ld, %ld); %u x %u\n",
      fbx.xMin, fbx.yMin, fbx.xMax, fbx.yMax,
      final_width, final_rows ));

    /* adjust source bitmap if necessary */
    إذا ( source_->pixel_mode != FT_PIXEL_MODE_GRAY )
    {
      FT_Bitmap_Init( &source_bitmap );
      error = FT_Bitmap_Convert( library, source_, &source_bitmap, 1 );
      إذا ( error )
        اذهب Error;

      source             = &source_bitmap;
      free_source_bitmap = 1;
    }
    وإلا
      source = source_;

    /* do blending; the code below returns pre-multiplied channels, */
    /* similar to what FreeType gets from `CBDT' tables             */

    /* the bitmap flow is from top to bottom, */
    /* but y is measured from bottom to top   */
    إذا ( target->pitch < 0 )
    {
      /* XXX */
    }
    وإلا
    {
      FT_Pos  x = sbx.xMin - fbx.xMin;
      FT_Pos  y = sbx.yMin - fbx.yMin;

      unsigned حرف*  p =
        source->buffer;
      unsigned حرف*  q =
        target->buffer +
        ( target->rows - y - source->rows ) * target->pitch +
        x * 4;
      unsigned حرف*  limit_p =
        p + source->pitch * (صحيح)source->rows;


      بينما ( p < limit_p )
      {
        unsigned حرف*  r       = p;
        unsigned حرف*  s       = q;
        unsigned حرف*  limit_r = r + source->width;


        بينما ( r < limit_r )
        {
          صحيح  aa = *r++;
          صحيح  fa = color.alpha * aa / 255;

          صحيح  fb = color.blue * fa / 255;
          صحيح  fg = color.green * fa / 255;
          صحيح  fr = color.red * fa / 255;

          صحيح  ba2 = 255 - fa;

          صحيح  bb = s[0];
          صحيح  bg = s[1];
          صحيح  br = s[2];
          صحيح  ba = s[3];


          *s++ = (unsigned حرف)( bb * ba2 / 255 + fb );
          *s++ = (unsigned حرف)( bg * ba2 / 255 + fg );
          *s++ = (unsigned حرف)( br * ba2 / 255 + fr );
          *s++ = (unsigned حرف)( ba * ba2 / 255 + fa );
        }

        p += source->pitch;
        q += target->pitch;
      }
    }

    atarget_offset->x = fbx.xMin * 64;
    atarget_offset->y = fbx.yMax * 64;

  Error:
    إذا ( error && free_target_bitmap_on_error )
      FT_Bitmap_Done( library, target );

    إذا ( free_source_bitmap )
      FT_Bitmap_Done( library, &source_bitmap );

    إرجاع error;
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_GlyphSlot_Own_Bitmap( FT_GlyphSlot  slot )
  {
    إذا ( slot && slot->format == FT_GLYPH_FORMAT_BITMAP   &&
         !( slot->internal->flags & FT_GLYPH_OWN_BITMAP ) )
    {
      FT_Bitmap  bitmap;
      FT_Error   error;


      FT_Bitmap_Init( &bitmap );
      error = FT_Bitmap_Copy( slot->library, &slot->bitmap, &bitmap );
      إذا ( error )
        إرجاع error;

      slot->bitmap = bitmap;
      slot->internal->flags |= FT_GLYPH_OWN_BITMAP;
    }

    إرجاع FT_Err_Ok;
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Bitmap_Done( FT_Library  library,
                  FT_Bitmap  *bitmap )
  {
    FT_Memory  memory;


    إذا ( !library )
      إرجاع FT_THROW( Invalid_Library_Handle );

    إذا ( !bitmap )
      إرجاع FT_THROW( Invalid_Argument );

    memory = library->memory;

    FT_FREE( bitmap->buffer );
    *bitmap = null_bitmap;

    إرجاع FT_Err_Ok;
  }


/* END */
