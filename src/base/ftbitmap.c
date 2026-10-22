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


#अन्तर्भाव <freetype/internal/ftdebug.h>

#अन्तर्भाव <freetype/ftbitmap.h>
#अन्तर्भाव <freetype/ftimage.h>
#अन्तर्भाव <freetype/internal/ftobjs.h>


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#undef  FT_COMPONENT
#define FT_COMPONENT  bitmap


  स्थिर
  स्थिरांक FT_Bitmap  null_bitmap = { 0, 0, 0, रिक्त, 0, 0, 0, रिक्त };


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( शून्य )
  FT_Bitmap_Init( FT_Bitmap  *abitmap )
  {
    जर ( abitmap )
      *abitmap = null_bitmap;
  }


  /* deprecated function name; retained for ABI compatibility */

  FT_EXPORT_DEF( शून्य )
  FT_Bitmap_New( FT_Bitmap  *abitmap )
  {
    जर ( abitmap )
      *abitmap = null_bitmap;
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Bitmap_Copy( FT_Library        library,
                  स्थिरांक FT_Bitmap  *source,
                  FT_Bitmap        *target)
  {
    FT_Memory  memory;
    FT_Error   error  = FT_Err_Ok;
    FT_Int     pitch;
    FT_Int     flip;


    जर ( !library )
      परत FT_THROW( Invalid_Library_Handle );

    जर ( !source || !target )
      परत FT_THROW( Invalid_Argument );

    जर ( source == target )
      परत FT_Err_Ok;

    flip = ( source->pitch < 0 && target->pitch > 0 ) ||
           ( source->pitch > 0 && target->pitch < 0 );

    memory = library->memory;
    FT_FREE( target->buffer );

    *target = *source;

    जर ( flip )
      target->pitch = -target->pitch;

    जर ( !source->buffer )
      परत FT_Err_Ok;

    pitch  = source->pitch;
    जर ( pitch < 0 )
      pitch = -pitch;

    FT_MEM_QALLOC_MULT( target->buffer, target->rows, pitch );

    जर ( !error )
    {
      जर ( flip )
      {
        /* take care of bitmap flow */
        FT_UInt   i;
        FT_Byte*  s = source->buffer;
        FT_Byte*  t = target->buffer;


        t += (FT_ULong)pitch * ( target->rows - 1 );

        साठी ( i = target->rows; i > 0; i-- )
        {
          FT_ARRAY_COPY( t, s, pitch );

          s += pitch;
          t -= pitch;
        }
      }
      अन्यथा
        FT_MEM_COPY( target->buffer, source->buffer,
                     (FT_Long)source->rows * pitch );
    }

    परत error;
  }


  /* Enlarge `bitmap' horizontally and vertically by `xpixels' */
  /* and `ypixels', respectively.                              */

  स्थिर FT_Error
  ft_bitmap_assure_buffer( FT_Memory   memory,
                           FT_Bitmap*  bitmap,
                           FT_UInt     xpixels,
                           FT_UInt     ypixels )
  {
    FT_Error        error;
    अचिन्हित पूर्णांक    pitch;
    अचिन्हित पूर्णांक    new_pitch;
    FT_UInt         bpp;
    FT_UInt         width, height;
    अचिन्हित अक्षर*  buffer = रिक्त;


    width  = bitmap->width;
    height = bitmap->rows;
    pitch  = (अचिन्हित पूर्णांक)FT_ABS( bitmap->pitch );

    निवड ( bitmap->pixel_mode )
    {
    स्थिती FT_PIXEL_MODE_MONO:
      bpp       = 1;
      new_pitch = ( width + xpixels + 7 ) >> 3;
      थांबा;
    स्थिती FT_PIXEL_MODE_GRAY2:
      bpp       = 2;
      new_pitch = ( width + xpixels + 3 ) >> 2;
      थांबा;
    स्थिती FT_PIXEL_MODE_GRAY4:
      bpp       = 4;
      new_pitch = ( width + xpixels + 1 ) >> 1;
      थांबा;
    स्थिती FT_PIXEL_MODE_GRAY:
    स्थिती FT_PIXEL_MODE_LCD:
    स्थिती FT_PIXEL_MODE_LCD_V:
      bpp       = 8;
      new_pitch = width + xpixels;
      थांबा;
    पूर्वनिर्धारित:
      परत FT_THROW( Invalid_Glyph_Format );
    }

    /* if no need to allocate memory */
    जर ( ypixels == 0 && new_pitch <= pitch )
    {
      /* zero the padding */
      FT_UInt  bit_width = pitch * 8;
      FT_UInt  bit_last  = ( width + xpixels ) * bpp;


      जर ( bit_last < bit_width )
      {
        FT_Byte*  ओळ  = bitmap->buffer + ( bit_last >> 3 );
        FT_Byte*  end   = bitmap->buffer + pitch;
        FT_UInt   shift = bit_last & 7;
        FT_UInt   mask  = 0xFF00U >> shift;
        FT_UInt   मोजणी = height;


        साठी ( ; मोजणी > 0; मोजणी--, ओळ += pitch, end += pitch )
        {
          FT_Byte*  लिख = ओळ;


          जर ( shift > 0 )
          {
            लिख[0] = (FT_Byte)( लिख[0] & mask );
            लिख++;
          }
          जर ( लिख < end )
            FT_MEM_ZERO( लिख, end - लिख );
        }
      }

      परत FT_Err_Ok;
    }

    /* otherwise allocate new buffer */
    जर ( FT_QALLOC_MULT( buffer, bitmap->rows + ypixels, new_pitch ) )
      परत error;

    /* new rows get added at the top of the bitmap, */
    /* thus take care of the flow direction         */
    जर ( bitmap->pitch > 0 )
    {
      FT_UInt  len = ( width * bpp + 7 ) >> 3;

      अचिन्हित अक्षर*  in  = bitmap->buffer;
      अचिन्हित अक्षर*  out = buffer;

      अचिन्हित अक्षर*  limit = bitmap->buffer + pitch * bitmap->rows;
      अचिन्हित पूर्णांक    delta = new_pitch - len;


      FT_MEM_ZERO( out, new_pitch * ypixels );
      out += new_pitch * ypixels;

      पर्यंत ( in < limit )
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
    अन्यथा
    {
      FT_UInt  len = ( width * bpp + 7 ) >> 3;

      अचिन्हित अक्षर*  in  = bitmap->buffer;
      अचिन्हित अक्षर*  out = buffer;

      अचिन्हित अक्षर*  limit = bitmap->buffer + pitch * bitmap->rows;
      अचिन्हित पूर्णांक    delta = new_pitch - len;


      पर्यंत ( in < limit )
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
    जर ( bitmap->pitch < 0 )
      bitmap->pitch = -(पूर्णांक)new_pitch;
    अन्यथा
      bitmap->pitch = (पूर्णांक)new_pitch;

    परत FT_Err_Ok;
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Bitmap_Embolden( FT_Library  library,
                      FT_Bitmap*  bitmap,
                      FT_Pos      xStrength,
                      FT_Pos      yStrength )
  {
    FT_Error        error;
    अचिन्हित अक्षर*  p;
    FT_Int          i, x, pitch;
    FT_UInt         y;
    FT_Int          xstr, ystr;


    जर ( !library )
      परत FT_THROW( Invalid_Library_Handle );

    जर ( !bitmap || !bitmap->buffer )
      परत FT_THROW( Invalid_Argument );

    जर ( ( ( FT_PIX_ROUND( xStrength ) >> 6 ) > FT_INT_MAX ) ||
         ( ( FT_PIX_ROUND( yStrength ) >> 6 ) > FT_INT_MAX ) )
      परत FT_THROW( Invalid_Argument );

    xstr = (FT_Int)FT_PIX_ROUND( xStrength ) >> 6;
    ystr = (FT_Int)FT_PIX_ROUND( yStrength ) >> 6;

    जर ( xstr == 0 && ystr == 0 )
      परत FT_Err_Ok;
    अन्यथा जर ( xstr < 0 || ystr < 0 )
      परत FT_THROW( Invalid_Argument );

    निवड ( bitmap->pixel_mode )
    {
    स्थिती FT_PIXEL_MODE_GRAY2:
    स्थिती FT_PIXEL_MODE_GRAY4:
      {
        FT_Bitmap  tmp;


        /* convert to 8bpp */
        FT_Bitmap_Init( &tmp );
        error = FT_Bitmap_Convert( library, bitmap, &tmp, 1 );
        जर ( error )
          परत error;

        FT_Bitmap_Done( library, bitmap );
        *bitmap = tmp;
      }
      थांबा;

    स्थिती FT_PIXEL_MODE_MONO:
      जर ( xstr > 8 )
        xstr = 8;
      थांबा;

    स्थिती FT_PIXEL_MODE_LCD:
      xstr *= 3;
      थांबा;

    स्थिती FT_PIXEL_MODE_LCD_V:
      ystr *= 3;
      थांबा;

    स्थिती FT_PIXEL_MODE_BGRA:
      /* We don't embolden color glyphs. */
      परत FT_Err_Ok;
    }

    error = ft_bitmap_assure_buffer( library->memory, bitmap,
                                     (FT_UInt)xstr, (FT_UInt)ystr );
    जर ( error )
      परत error;

    /* take care of bitmap flow */
    pitch = bitmap->pitch;
    जर ( pitch > 0 )
      p = bitmap->buffer + pitch * ystr;
    अन्यथा
    {
      pitch = -pitch;
      p = bitmap->buffer + (FT_UInt)pitch * ( bitmap->rows - 1 );
    }

    /* for each row */
    साठी ( y = 0; y < bitmap->rows; y++ )
    {
      /*
       * Horizontally:
       *
       * From the last pixel on, make each pixel or'ed with the
       * `xstr' pixels before it.
       */
      साठी ( x = pitch - 1; x >= 0; x-- )
      {
        अचिन्हित अक्षर  tmp;


        tmp = p[x];
        साठी ( i = 1; i <= xstr; i++ )
        {
          जर ( bitmap->pixel_mode == FT_PIXEL_MODE_MONO )
          {
            p[x] |= tmp >> i;

            /* the maximum value of 8 for `xstr' comes from here */
            जर ( x > 0 )
              p[x] |= p[x - 1] << ( 8 - i );

#जर 0
            जर ( p[x] == 0xFF )
              थांबा;
#endif
          }
          अन्यथा
          {
            जर ( x - i >= 0 )
            {
              जर ( p[x] + p[x - i] > bitmap->अंक_grays - 1 )
              {
                p[x] = (अचिन्हित अक्षर)( bitmap->अंक_grays - 1 );
                थांबा;
              }
              अन्यथा
              {
                p[x] = (अचिन्हित अक्षर)( p[x] + p[x - i] );
                जर ( p[x] == bitmap->अंक_grays - 1 )
                  थांबा;
              }
            }
            अन्यथा
              थांबा;
          }
        }
      }

      /*
       * Vertically:
       *
       * Make the above `ystr' rows or'ed with it.
       */
      साठी ( x = 1; x <= ystr; x++ )
      {
        अचिन्हित अक्षर*  q;


        q = p - bitmap->pitch * x;
        साठी ( i = 0; i < pitch; i++ )
          q[i] |= p[i];
      }

      p += bitmap->pitch;
    }

    bitmap->width += (FT_UInt)xstr;
    bitmap->rows += (FT_UInt)ystr;

    परत FT_Err_Ok;
  }


  स्थिर FT_Byte
  ft_gray_for_premultiplied_srgb_bgra( स्थिरांक FT_Byte*  bgra )
  {
    FT_UInt  a = bgra[3];
    FT_UInt  l;


    /* Short-circuit transparent color to avoid division by zero. */
    जर ( !a )
      परत 0;

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

    परत (FT_Byte)( a - l / a );
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Bitmap_Convert( FT_Library        library,
                     स्थिरांक FT_Bitmap  *source,
                     FT_Bitmap        *target,
                     FT_Int            alignment )
  {
    FT_Error   error = FT_Err_Ok;
    FT_Memory  memory;

    FT_Byte*  s;
    FT_Byte*  t;


    जर ( !library )
      परत FT_THROW( Invalid_Library_Handle );

    जर ( !source || !target )
      परत FT_THROW( Invalid_Argument );

    memory = library->memory;

    निवड ( source->pixel_mode )
    {
    स्थिती FT_PIXEL_MODE_MONO:
    स्थिती FT_PIXEL_MODE_GRAY:
    स्थिती FT_PIXEL_MODE_GRAY2:
    स्थिती FT_PIXEL_MODE_GRAY4:
    स्थिती FT_PIXEL_MODE_LCD:
    स्थिती FT_PIXEL_MODE_LCD_V:
    स्थिती FT_PIXEL_MODE_BGRA:
      {
        FT_Int  width = (FT_Int)source->width;
        FT_Int  neg   = ( target->pitch == 0 && source->pitch < 0 ) ||
                          target->pitch  < 0;


        FT_Bitmap_Done( library, target );

        target->pixel_mode = FT_PIXEL_MODE_GRAY;
        target->rows       = source->rows;
        target->width      = source->width;

        जर ( alignment )
        {
          FT_Int  rem = width % alignment;


          जर ( rem )
            width = alignment > 0 ? width - rem + alignment
                                  : width - rem - alignment;
        }

        जर ( FT_QALLOC_MULT( target->buffer, target->rows, width ) )
          परत error;

        target->pitch = neg ? -width : width;
      }
      थांबा;

    पूर्वनिर्धारित:
      error = FT_THROW( Invalid_Argument );
    }

    s = source->buffer;
    t = target->buffer;

    /* take care of bitmap flow */
    जर ( source->pitch < 0 )
      s -= source->pitch * (FT_Int)( source->rows - 1 );
    जर ( target->pitch < 0 )
      t -= target->pitch * (FT_Int)( target->rows - 1 );

    निवड ( source->pixel_mode )
    {
    स्थिती FT_PIXEL_MODE_MONO:
      {
        FT_UInt  i;


        target->अंक_grays = 2;

        साठी ( i = source->rows; i > 0; i-- )
        {
          FT_Byte*  ss = s;
          FT_Byte*  tt = t;
          FT_UInt   j;


          /* get the full bytes */
          साठी ( j = source->width >> 3; j > 0; j-- )
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
          जर ( j > 0 )
          {
            FT_Int  val = *ss;


            साठी ( ; j > 0; j-- )
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
      थांबा;


    स्थिती FT_PIXEL_MODE_GRAY:
    स्थिती FT_PIXEL_MODE_LCD:
    स्थिती FT_PIXEL_MODE_LCD_V:
      {
        FT_UInt  width = source->width;
        FT_UInt  i;


        target->अंक_grays = 256;

        साठी ( i = source->rows; i > 0; i-- )
        {
          FT_ARRAY_COPY( t, s, width );

          s += source->pitch;
          t += target->pitch;
        }
      }
      थांबा;


    स्थिती FT_PIXEL_MODE_GRAY2:
      {
        FT_UInt  i;


        target->अंक_grays = 4;

        साठी ( i = source->rows; i > 0; i-- )
        {
          FT_Byte*  ss = s;
          FT_Byte*  tt = t;
          FT_UInt   j;


          /* get the full bytes */
          साठी ( j = source->width >> 2; j > 0; j-- )
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
          जर ( j > 0 )
          {
            FT_Int  val = ss[0];


            साठी ( ; j > 0; j-- )
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
      थांबा;


    स्थिती FT_PIXEL_MODE_GRAY4:
      {
        FT_UInt  i;


        target->अंक_grays = 16;

        साठी ( i = source->rows; i > 0; i-- )
        {
          FT_Byte*  ss = s;
          FT_Byte*  tt = t;
          FT_UInt   j;


          /* get the full bytes */
          साठी ( j = source->width >> 1; j > 0; j-- )
          {
            FT_Int  val = ss[0];


            tt[0] = (FT_Byte)( ( val & 0xF0 ) >> 4 );
            tt[1] = (FT_Byte)( ( val & 0x0F ) );

            ss += 1;
            tt += 2;
          }

          जर ( source->width & 1 )
            tt[0] = (FT_Byte)( ( ss[0] & 0xF0 ) >> 4 );

          s += source->pitch;
          t += target->pitch;
        }
      }
      थांबा;


    स्थिती FT_PIXEL_MODE_BGRA:
      {
        FT_UInt  i;


        target->अंक_grays = 256;

        साठी ( i = source->rows; i > 0; i-- )
        {
          FT_Byte*  ss = s;
          FT_Byte*  tt = t;
          FT_UInt   j;


          साठी ( j = source->width; j > 0; j-- )
          {
            tt[0] = ft_gray_for_premultiplied_srgb_bgra( ss );

            ss += 4;
            tt += 1;
          }

          s += source->pitch;
          t += target->pitch;
        }
      }
      थांबा;

    पूर्वनिर्धारित:
      ;
    }

    परत error;
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Bitmap_Blend( FT_Library        library,
                   स्थिरांक FT_Bitmap*  source_,
                   स्थिरांक FT_Vector   source_offset_,
                   FT_Bitmap*        target,
                   FT_Vector        *atarget_offset,
                   FT_Color          color )
  {
    FT_Error   error = FT_Err_Ok;
    FT_Memory  memory;

    FT_Bitmap         source_bitmap;
    स्थिरांक FT_Bitmap*  source;

    FT_Bool  free_source_bitmap          = 0;
    FT_Bool  free_target_bitmap_on_error = 0;

    FT_BBox  sbx, tbx, fbx;

    अचिन्हित पूर्णांक  final_rows, final_width;


    जर ( !library || !target || !source_ || !atarget_offset )
      परत FT_THROW( Invalid_Argument );

    memory = library->memory;

    जर ( !( target->pixel_mode == FT_PIXEL_MODE_NONE     ||
            ( target->pixel_mode == FT_PIXEL_MODE_BGRA &&
              target->buffer                           ) ) )
      परत FT_THROW( Invalid_Argument );

    जर ( source_->pixel_mode == FT_PIXEL_MODE_NONE )
      परत FT_Err_Ok;               /* nothing to do */

    /* pitches must have the same sign */
    जर ( target->pixel_mode == FT_PIXEL_MODE_BGRA &&
         ( source_->pitch ^ target->pitch ) < 0   )
      परत FT_THROW( Invalid_Argument );

    जर ( !( source_->width && source_->rows ) )
      परत FT_Err_Ok;               /* nothing to do */

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
    जर ( sbx.xMin > sbx.xMax      || sbx.yMin > sbx.yMax     ||
         source_->width > 0x7FFFU || source_->rows > 0x7FFFU )
    {
      FT_TRACE5(( "FT_Bitmap_Blend: source dimension overflow\n" ));
      परत FT_THROW( Invalid_Argument );
    }

    जर ( !target->width || !target->rows )
    {
      FT_TRACE5(( "  target bitmap: empty\n" ));

      final_width = source_->width;
      final_rows  = source_->rows;

      /* create new empty bitmap */
      target->width      = final_width;
      target->rows       = final_rows;
      target->pixel_mode = FT_PIXEL_MODE_BGRA;
      target->pitch      = (पूर्णांक)final_width * 4;
      target->अंक_grays  = 256;

      जर ( FT_ALLOC_MULT( target->buffer, target->rows, target->pitch ) )
        परत error;

      free_target_bitmap_on_error = 1;

      fbx = sbx;
    }
    अन्यथा
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
      जर ( tbx.xMin > tbx.xMax || tbx.yMin > tbx.yMax )
      {
        FT_TRACE5(( "FT_Bitmap_Blend: targget dimension overflow\n" ));
        परत FT_THROW( Invalid_Argument );
      }

      /* compute final bitmap dimensions */
      fbx.xMin = FT_MIN( sbx.xMin, tbx.xMin );
      fbx.yMin = FT_MIN( sbx.yMin, tbx.yMin );
      fbx.xMax = FT_MAX( sbx.xMax, tbx.xMax );
      fbx.yMax = FT_MAX( sbx.yMax, tbx.yMax );

      /* sanity check */
      जर ( fbx.xMin < -0x10000 || fbx.xMax >= 0x10000 ||
           fbx.yMin < -0x10000 || fbx.yMax >= 0x10000 )
      {
        FT_TRACE5(( "FT_Bitmap_Blend: final dimension overflow\n" ));
        परत FT_THROW( Invalid_Argument );
      }

      final_width = fbx.xMax - fbx.xMin;
      final_rows  = fbx.yMax - fbx.yMin;

      /* adjust target bitmap to enlarged size */
      जर ( target->width < final_width ||
           target->rows  < final_rows  )
      {
        पूर्णांक  pitch, new_pitch;

        अचिन्हित अक्षर*  buffer = रिक्त;


        pitch = target->pitch;

        जर ( pitch < 0 )
          pitch = -pitch;

        new_pitch = (पूर्णांक)final_width * 4;

        /* TODO: provide an in-buffer solution for large bitmaps */
        /*       to avoid allocation of a new buffer             */
        जर ( FT_ALLOC_MULT( buffer, final_rows, new_pitch ) )
          जा Error;

        /* copy data to new buffer */

        /* the bitmap flow is from top to bottom, */
        /* but y is measured from bottom to top   */
        जर ( target->pitch < 0 )
        {
          /* XXX */
        }
        अन्यथा
        {
          FT_Pos  x = tbx.xMin - fbx.xMin;
          FT_Pos  y = tbx.yMin - fbx.yMin;

          अचिन्हित अक्षर*  p =
            target->buffer;
          अचिन्हित अक्षर*  q =
            buffer +
            ( final_rows - y - target->rows ) * new_pitch +
            x * 4;
          अचिन्हित अक्षर*  limit_p =
            p + pitch * (पूर्णांक)target->rows;


          पर्यंत ( p < limit_p )
          {
            FT_MEM_COPY( q, p, pitch );

            p += pitch;
            q += new_pitch;
          }
        }

        FT_FREE( target->buffer );

        target->width = final_width;
        target->rows  = final_rows;

        जर ( target->pitch < 0 )
          target->pitch = -new_pitch;
        अन्यथा
          target->pitch = new_pitch;

        target->buffer = buffer;
      }
    }

    FT_TRACE5(( "  final bitmap: (%ld, %ld) -- (%ld, %ld); %u x %u\n",
      fbx.xMin, fbx.yMin, fbx.xMax, fbx.yMax,
      final_width, final_rows ));

    /* adjust source bitmap if necessary */
    जर ( source_->pixel_mode != FT_PIXEL_MODE_GRAY )
    {
      FT_Bitmap_Init( &source_bitmap );
      error = FT_Bitmap_Convert( library, source_, &source_bitmap, 1 );
      जर ( error )
        जा Error;

      source             = &source_bitmap;
      free_source_bitmap = 1;
    }
    अन्यथा
      source = source_;

    /* do blending; the code below returns pre-multiplied channels, */
    /* similar to what FreeType gets from `CBDT' tables             */

    /* the bitmap flow is from top to bottom, */
    /* but y is measured from bottom to top   */
    जर ( target->pitch < 0 )
    {
      /* XXX */
    }
    अन्यथा
    {
      FT_Pos  x = sbx.xMin - fbx.xMin;
      FT_Pos  y = sbx.yMin - fbx.yMin;

      अचिन्हित अक्षर*  p =
        source->buffer;
      अचिन्हित अक्षर*  q =
        target->buffer +
        ( target->rows - y - source->rows ) * target->pitch +
        x * 4;
      अचिन्हित अक्षर*  limit_p =
        p + source->pitch * (पूर्णांक)source->rows;


      पर्यंत ( p < limit_p )
      {
        अचिन्हित अक्षर*  r       = p;
        अचिन्हित अक्षर*  s       = q;
        अचिन्हित अक्षर*  limit_r = r + source->width;


        पर्यंत ( r < limit_r )
        {
          पूर्णांक  aa = *r++;
          पूर्णांक  fa = color.alpha * aa / 255;

          पूर्णांक  fb = color.blue * fa / 255;
          पूर्णांक  fg = color.green * fa / 255;
          पूर्णांक  fr = color.red * fa / 255;

          पूर्णांक  ba2 = 255 - fa;

          पूर्णांक  bb = s[0];
          पूर्णांक  bg = s[1];
          पूर्णांक  br = s[2];
          पूर्णांक  ba = s[3];


          *s++ = (अचिन्हित अक्षर)( bb * ba2 / 255 + fb );
          *s++ = (अचिन्हित अक्षर)( bg * ba2 / 255 + fg );
          *s++ = (अचिन्हित अक्षर)( br * ba2 / 255 + fr );
          *s++ = (अचिन्हित अक्षर)( ba * ba2 / 255 + fa );
        }

        p += source->pitch;
        q += target->pitch;
      }
    }

    atarget_offset->x = fbx.xMin * 64;
    atarget_offset->y = fbx.yMax * 64;

  Error:
    जर ( error && free_target_bitmap_on_error )
      FT_Bitmap_Done( library, target );

    जर ( free_source_bitmap )
      FT_Bitmap_Done( library, &source_bitmap );

    परत error;
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_GlyphSlot_Own_Bitmap( FT_GlyphSlot  slot )
  {
    जर ( slot && slot->format == FT_GLYPH_FORMAT_BITMAP   &&
         !( slot->internal->flags & FT_GLYPH_OWN_BITMAP ) )
    {
      FT_Bitmap  bitmap;
      FT_Error   error;


      FT_Bitmap_Init( &bitmap );
      error = FT_Bitmap_Copy( slot->library, &slot->bitmap, &bitmap );
      जर ( error )
        परत error;

      slot->bitmap = bitmap;
      slot->internal->flags |= FT_GLYPH_OWN_BITMAP;
    }

    परत FT_Err_Ok;
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Bitmap_Done( FT_Library  library,
                  FT_Bitmap  *bitmap )
  {
    FT_Memory  memory;


    जर ( !library )
      परत FT_THROW( Invalid_Library_Handle );

    जर ( !bitmap )
      परत FT_THROW( Invalid_Argument );

    memory = library->memory;

    FT_FREE( bitmap->buffer );
    *bitmap = null_bitmap;

    परत FT_Err_Ok;
  }


/* END */
