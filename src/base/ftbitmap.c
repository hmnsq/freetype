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


#समावेश <freetype/internal/ftdebug.h>

#समावेश <freetype/ftbitmap.h>
#समावेश <freetype/ftimage.h>
#समावेश <freetype/internal/ftobjs.h>


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#अघोषित  FT_COMPONENT
#घोषणा FT_COMPONENT  bitmap


  अटल
  स्थिर FT_Bitmap  null_bitmap = { 0, 0, 0, शून्य, 0, 0, 0, शून्य };


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( व्योम )
  FT_Bitmap_Init( FT_Bitmap  *abitmap )
  {
    अगर ( abitmap )
      *abitmap = null_bitmap;
  }


  /* deprecated function name; retained for ABI compatibility */

  FT_EXPORT_DEF( व्योम )
  FT_Bitmap_New( FT_Bitmap  *abitmap )
  {
    अगर ( abitmap )
      *abitmap = null_bitmap;
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Bitmap_Copy( FT_Library        library,
                  स्थिर FT_Bitmap  *source,
                  FT_Bitmap        *target)
  {
    FT_Memory  memory;
    FT_Error   error  = FT_Err_Ok;
    FT_Int     pitch;
    FT_Int     flip;


    अगर ( !library )
      वापस FT_THROW( Invalid_Library_Handle );

    अगर ( !source || !target )
      वापस FT_THROW( Invalid_Argument );

    अगर ( source == target )
      वापस FT_Err_Ok;

    flip = ( source->pitch < 0 && target->pitch > 0 ) ||
           ( source->pitch > 0 && target->pitch < 0 );

    memory = library->memory;
    FT_FREE( target->बफर );

    *target = *source;

    अगर ( flip )
      target->pitch = -target->pitch;

    अगर ( !source->बफर )
      वापस FT_Err_Ok;

    pitch  = source->pitch;
    अगर ( pitch < 0 )
      pitch = -pitch;

    FT_MEM_QALLOC_MULT( target->बफर, target->rows, pitch );

    अगर ( !error )
    {
      अगर ( flip )
      {
        /* take care of bitmap flow */
        FT_UInt   i;
        FT_Byte*  s = source->बफर;
        FT_Byte*  t = target->बफर;


        t += (FT_ULong)pitch * ( target->rows - 1 );

        क्रम ( i = target->rows; i > 0; i-- )
        {
          FT_ARRAY_COPY( t, s, pitch );

          s += pitch;
          t -= pitch;
        }
      }
      अन्यथा
        FT_MEM_COPY( target->बफर, source->बफर,
                     (FT_Long)source->rows * pitch );
    }

    वापस error;
  }


  /* Enlarge `bitmap' horizontally and vertically by `xpixels' */
  /* and `ypixels', respectively.                              */

  अटल FT_Error
  ft_bitmap_assure_बफर( FT_Memory   memory,
                           FT_Bitmap*  bitmap,
                           FT_UInt     xpixels,
                           FT_UInt     ypixels )
  {
    FT_Error        error;
    अचिन्हित पूर्णांक    pitch;
    अचिन्हित पूर्णांक    new_pitch;
    FT_UInt         bpp;
    FT_UInt         width, height;
    अचिन्हित अक्षर*  बफर = शून्य;


    width  = bitmap->width;
    height = bitmap->rows;
    pitch  = (अचिन्हित पूर्णांक)FT_ABS( bitmap->pitch );

    चयन ( bitmap->pixel_mode )
    {
    हाल FT_PIXEL_MODE_MONO:
      bpp       = 1;
      new_pitch = ( width + xpixels + 7 ) >> 3;
      अवरोध;
    हाल FT_PIXEL_MODE_GRAY2:
      bpp       = 2;
      new_pitch = ( width + xpixels + 3 ) >> 2;
      अवरोध;
    हाल FT_PIXEL_MODE_GRAY4:
      bpp       = 4;
      new_pitch = ( width + xpixels + 1 ) >> 1;
      अवरोध;
    हाल FT_PIXEL_MODE_GRAY:
    हाल FT_PIXEL_MODE_LCD:
    हाल FT_PIXEL_MODE_LCD_V:
      bpp       = 8;
      new_pitch = width + xpixels;
      अवरोध;
    शेष:
      वापस FT_THROW( Invalid_Glyph_Format );
    }

    /* if no need to allocate memory */
    अगर ( ypixels == 0 && new_pitch <= pitch )
    {
      /* zero the padding */
      FT_UInt  bit_width = pitch * 8;
      FT_UInt  bit_last  = ( width + xpixels ) * bpp;


      अगर ( bit_last < bit_width )
      {
        FT_Byte*  पंक्ति  = bitmap->बफर + ( bit_last >> 3 );
        FT_Byte*  end   = bitmap->बफर + pitch;
        FT_UInt   shift = bit_last & 7;
        FT_UInt   mask  = 0xFF00U >> shift;
        FT_UInt   गिनती = height;


        क्रम ( ; गिनती > 0; गिनती--, पंक्ति += pitch, end += pitch )
        {
          FT_Byte*  ग_लिखो = पंक्ति;


          अगर ( shift > 0 )
          {
            ग_लिखो[0] = (FT_Byte)( ग_लिखो[0] & mask );
            ग_लिखो++;
          }
          अगर ( ग_लिखो < end )
            FT_MEM_ZERO( ग_लिखो, end - ग_लिखो );
        }
      }

      वापस FT_Err_Ok;
    }

    /* otherwise allocate new buffer */
    अगर ( FT_QALLOC_MULT( बफर, bitmap->rows + ypixels, new_pitch ) )
      वापस error;

    /* new rows get added at the top of the bitmap, */
    /* thus take care of the flow direction         */
    अगर ( bitmap->pitch > 0 )
    {
      FT_UInt  len = ( width * bpp + 7 ) >> 3;

      अचिन्हित अक्षर*  in  = bitmap->बफर;
      अचिन्हित अक्षर*  out = बफर;

      अचिन्हित अक्षर*  limit = bitmap->बफर + pitch * bitmap->rows;
      अचिन्हित पूर्णांक    delta = new_pitch - len;


      FT_MEM_ZERO( out, new_pitch * ypixels );
      out += new_pitch * ypixels;

      जबतक ( in < limit )
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

      अचिन्हित अक्षर*  in  = bitmap->बफर;
      अचिन्हित अक्षर*  out = बफर;

      अचिन्हित अक्षर*  limit = bitmap->बफर + pitch * bitmap->rows;
      अचिन्हित पूर्णांक    delta = new_pitch - len;


      जबतक ( in < limit )
      {
        FT_MEM_COPY( out, in, len );
        in  += pitch;
        out += len;

        FT_MEM_ZERO( out, delta );
        out += delta;
      }

      FT_MEM_ZERO( out, new_pitch * ypixels );
    }

    FT_FREE( bitmap->बफर );
    bitmap->बफर = बफर;

    /* set pitch only, width and height are left untouched */
    अगर ( bitmap->pitch < 0 )
      bitmap->pitch = -(पूर्णांक)new_pitch;
    अन्यथा
      bitmap->pitch = (पूर्णांक)new_pitch;

    वापस FT_Err_Ok;
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


    अगर ( !library )
      वापस FT_THROW( Invalid_Library_Handle );

    अगर ( !bitmap || !bitmap->बफर )
      वापस FT_THROW( Invalid_Argument );

    अगर ( ( ( FT_PIX_ROUND( xStrength ) >> 6 ) > FT_INT_MAX ) ||
         ( ( FT_PIX_ROUND( yStrength ) >> 6 ) > FT_INT_MAX ) )
      वापस FT_THROW( Invalid_Argument );

    xstr = (FT_Int)FT_PIX_ROUND( xStrength ) >> 6;
    ystr = (FT_Int)FT_PIX_ROUND( yStrength ) >> 6;

    अगर ( xstr == 0 && ystr == 0 )
      वापस FT_Err_Ok;
    अन्यथा अगर ( xstr < 0 || ystr < 0 )
      वापस FT_THROW( Invalid_Argument );

    चयन ( bitmap->pixel_mode )
    {
    हाल FT_PIXEL_MODE_GRAY2:
    हाल FT_PIXEL_MODE_GRAY4:
      {
        FT_Bitmap  tmp;


        /* convert to 8bpp */
        FT_Bitmap_Init( &tmp );
        error = FT_Bitmap_Convert( library, bitmap, &tmp, 1 );
        अगर ( error )
          वापस error;

        FT_Bitmap_Done( library, bitmap );
        *bitmap = tmp;
      }
      अवरोध;

    हाल FT_PIXEL_MODE_MONO:
      अगर ( xstr > 8 )
        xstr = 8;
      अवरोध;

    हाल FT_PIXEL_MODE_LCD:
      xstr *= 3;
      अवरोध;

    हाल FT_PIXEL_MODE_LCD_V:
      ystr *= 3;
      अवरोध;

    हाल FT_PIXEL_MODE_BGRA:
      /* We don't embolden color glyphs. */
      वापस FT_Err_Ok;
    }

    error = ft_bitmap_assure_बफर( library->memory, bitmap,
                                     (FT_UInt)xstr, (FT_UInt)ystr );
    अगर ( error )
      वापस error;

    /* take care of bitmap flow */
    pitch = bitmap->pitch;
    अगर ( pitch > 0 )
      p = bitmap->बफर + pitch * ystr;
    अन्यथा
    {
      pitch = -pitch;
      p = bitmap->बफर + (FT_UInt)pitch * ( bitmap->rows - 1 );
    }

    /* for each row */
    क्रम ( y = 0; y < bitmap->rows; y++ )
    {
      /*
       * Horizontally:
       *
       * From the last pixel on, make each pixel or'ed with the
       * `xstr' pixels before it.
       */
      क्रम ( x = pitch - 1; x >= 0; x-- )
      {
        अचिन्हित अक्षर  tmp;


        tmp = p[x];
        क्रम ( i = 1; i <= xstr; i++ )
        {
          अगर ( bitmap->pixel_mode == FT_PIXEL_MODE_MONO )
          {
            p[x] |= tmp >> i;

            /* the maximum value of 8 for `xstr' comes from here */
            अगर ( x > 0 )
              p[x] |= p[x - 1] << ( 8 - i );

#अगर 0
            अगर ( p[x] == 0xFF )
              अवरोध;
#पूर्ण_अगर
          }
          अन्यथा
          {
            अगर ( x - i >= 0 )
            {
              अगर ( p[x] + p[x - i] > bitmap->अंक_grays - 1 )
              {
                p[x] = (अचिन्हित अक्षर)( bitmap->अंक_grays - 1 );
                अवरोध;
              }
              अन्यथा
              {
                p[x] = (अचिन्हित अक्षर)( p[x] + p[x - i] );
                अगर ( p[x] == bitmap->अंक_grays - 1 )
                  अवरोध;
              }
            }
            अन्यथा
              अवरोध;
          }
        }
      }

      /*
       * Vertically:
       *
       * Make the above `ystr' rows or'ed with it.
       */
      क्रम ( x = 1; x <= ystr; x++ )
      {
        अचिन्हित अक्षर*  q;


        q = p - bitmap->pitch * x;
        क्रम ( i = 0; i < pitch; i++ )
          q[i] |= p[i];
      }

      p += bitmap->pitch;
    }

    bitmap->width += (FT_UInt)xstr;
    bitmap->rows += (FT_UInt)ystr;

    वापस FT_Err_Ok;
  }


  अटल FT_Byte
  ft_gray_for_premultiplied_srgb_bgra( स्थिर FT_Byte*  bgra )
  {
    FT_UInt  a = bgra[3];
    FT_UInt  l;


    /* Short-circuit transparent color to avoid division by zero. */
    अगर ( !a )
      वापस 0;

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

    वापस (FT_Byte)( a - l / a );
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Bitmap_Convert( FT_Library        library,
                     स्थिर FT_Bitmap  *source,
                     FT_Bitmap        *target,
                     FT_Int            alignment )
  {
    FT_Error   error = FT_Err_Ok;
    FT_Memory  memory;

    FT_Byte*  s;
    FT_Byte*  t;


    अगर ( !library )
      वापस FT_THROW( Invalid_Library_Handle );

    अगर ( !source || !target )
      वापस FT_THROW( Invalid_Argument );

    memory = library->memory;

    चयन ( source->pixel_mode )
    {
    हाल FT_PIXEL_MODE_MONO:
    हाल FT_PIXEL_MODE_GRAY:
    हाल FT_PIXEL_MODE_GRAY2:
    हाल FT_PIXEL_MODE_GRAY4:
    हाल FT_PIXEL_MODE_LCD:
    हाल FT_PIXEL_MODE_LCD_V:
    हाल FT_PIXEL_MODE_BGRA:
      {
        FT_Int  width = (FT_Int)source->width;
        FT_Int  neg   = ( target->pitch == 0 && source->pitch < 0 ) ||
                          target->pitch  < 0;


        FT_Bitmap_Done( library, target );

        target->pixel_mode = FT_PIXEL_MODE_GRAY;
        target->rows       = source->rows;
        target->width      = source->width;

        अगर ( alignment )
        {
          FT_Int  rem = width % alignment;


          अगर ( rem )
            width = alignment > 0 ? width - rem + alignment
                                  : width - rem - alignment;
        }

        अगर ( FT_QALLOC_MULT( target->बफर, target->rows, width ) )
          वापस error;

        target->pitch = neg ? -width : width;
      }
      अवरोध;

    शेष:
      error = FT_THROW( Invalid_Argument );
    }

    s = source->बफर;
    t = target->बफर;

    /* take care of bitmap flow */
    अगर ( source->pitch < 0 )
      s -= source->pitch * (FT_Int)( source->rows - 1 );
    अगर ( target->pitch < 0 )
      t -= target->pitch * (FT_Int)( target->rows - 1 );

    चयन ( source->pixel_mode )
    {
    हाल FT_PIXEL_MODE_MONO:
      {
        FT_UInt  i;


        target->अंक_grays = 2;

        क्रम ( i = source->rows; i > 0; i-- )
        {
          FT_Byte*  ss = s;
          FT_Byte*  tt = t;
          FT_UInt   j;


          /* get the full bytes */
          क्रम ( j = source->width >> 3; j > 0; j-- )
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
          अगर ( j > 0 )
          {
            FT_Int  val = *ss;


            क्रम ( ; j > 0; j-- )
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
      अवरोध;


    हाल FT_PIXEL_MODE_GRAY:
    हाल FT_PIXEL_MODE_LCD:
    हाल FT_PIXEL_MODE_LCD_V:
      {
        FT_UInt  width = source->width;
        FT_UInt  i;


        target->अंक_grays = 256;

        क्रम ( i = source->rows; i > 0; i-- )
        {
          FT_ARRAY_COPY( t, s, width );

          s += source->pitch;
          t += target->pitch;
        }
      }
      अवरोध;


    हाल FT_PIXEL_MODE_GRAY2:
      {
        FT_UInt  i;


        target->अंक_grays = 4;

        क्रम ( i = source->rows; i > 0; i-- )
        {
          FT_Byte*  ss = s;
          FT_Byte*  tt = t;
          FT_UInt   j;


          /* get the full bytes */
          क्रम ( j = source->width >> 2; j > 0; j-- )
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
          अगर ( j > 0 )
          {
            FT_Int  val = ss[0];


            क्रम ( ; j > 0; j-- )
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
      अवरोध;


    हाल FT_PIXEL_MODE_GRAY4:
      {
        FT_UInt  i;


        target->अंक_grays = 16;

        क्रम ( i = source->rows; i > 0; i-- )
        {
          FT_Byte*  ss = s;
          FT_Byte*  tt = t;
          FT_UInt   j;


          /* get the full bytes */
          क्रम ( j = source->width >> 1; j > 0; j-- )
          {
            FT_Int  val = ss[0];


            tt[0] = (FT_Byte)( ( val & 0xF0 ) >> 4 );
            tt[1] = (FT_Byte)( ( val & 0x0F ) );

            ss += 1;
            tt += 2;
          }

          अगर ( source->width & 1 )
            tt[0] = (FT_Byte)( ( ss[0] & 0xF0 ) >> 4 );

          s += source->pitch;
          t += target->pitch;
        }
      }
      अवरोध;


    हाल FT_PIXEL_MODE_BGRA:
      {
        FT_UInt  i;


        target->अंक_grays = 256;

        क्रम ( i = source->rows; i > 0; i-- )
        {
          FT_Byte*  ss = s;
          FT_Byte*  tt = t;
          FT_UInt   j;


          क्रम ( j = source->width; j > 0; j-- )
          {
            tt[0] = ft_gray_for_premultiplied_srgb_bgra( ss );

            ss += 4;
            tt += 1;
          }

          s += source->pitch;
          t += target->pitch;
        }
      }
      अवरोध;

    शेष:
      ;
    }

    वापस error;
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Bitmap_Blend( FT_Library        library,
                   स्थिर FT_Bitmap*  source_,
                   स्थिर FT_Vector   source_offset_,
                   FT_Bitmap*        target,
                   FT_Vector        *atarget_offset,
                   FT_Color          color )
  {
    FT_Error   error = FT_Err_Ok;
    FT_Memory  memory;

    FT_Bitmap         source_bitmap;
    स्थिर FT_Bitmap*  source;

    FT_Bool  free_source_bitmap          = 0;
    FT_Bool  free_target_bitmap_on_error = 0;

    FT_BBox  sbx, tbx, fbx;

    अचिन्हित पूर्णांक  final_rows, final_width;


    अगर ( !library || !target || !source_ || !atarget_offset )
      वापस FT_THROW( Invalid_Argument );

    memory = library->memory;

    अगर ( !( target->pixel_mode == FT_PIXEL_MODE_NONE     ||
            ( target->pixel_mode == FT_PIXEL_MODE_BGRA &&
              target->बफर                           ) ) )
      वापस FT_THROW( Invalid_Argument );

    अगर ( source_->pixel_mode == FT_PIXEL_MODE_NONE )
      वापस FT_Err_Ok;               /* nothing to do */

    /* pitches must have the same sign */
    अगर ( target->pixel_mode == FT_PIXEL_MODE_BGRA &&
         ( source_->pitch ^ target->pitch ) < 0   )
      वापस FT_THROW( Invalid_Argument );

    अगर ( !( source_->width && source_->rows ) )
      वापस FT_Err_Ok;               /* nothing to do */

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
    अगर ( sbx.xMin > sbx.xMax      || sbx.yMin > sbx.yMax     ||
         source_->width > 0x7FFFU || source_->rows > 0x7FFFU )
    {
      FT_TRACE5(( "FT_Bitmap_Blend: source dimension overflow\n" ));
      वापस FT_THROW( Invalid_Argument );
    }

    अगर ( !target->width || !target->rows )
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

      अगर ( FT_ALLOC_MULT( target->बफर, target->rows, target->pitch ) )
        वापस error;

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
      अगर ( tbx.xMin > tbx.xMax || tbx.yMin > tbx.yMax )
      {
        FT_TRACE5(( "FT_Bitmap_Blend: targget dimension overflow\n" ));
        वापस FT_THROW( Invalid_Argument );
      }

      /* compute final bitmap dimensions */
      fbx.xMin = FT_MIN( sbx.xMin, tbx.xMin );
      fbx.yMin = FT_MIN( sbx.yMin, tbx.yMin );
      fbx.xMax = FT_MAX( sbx.xMax, tbx.xMax );
      fbx.yMax = FT_MAX( sbx.yMax, tbx.yMax );

      /* sanity check */
      अगर ( fbx.xMin < -0x10000 || fbx.xMax >= 0x10000 ||
           fbx.yMin < -0x10000 || fbx.yMax >= 0x10000 )
      {
        FT_TRACE5(( "FT_Bitmap_Blend: final dimension overflow\n" ));
        वापस FT_THROW( Invalid_Argument );
      }

      final_width = fbx.xMax - fbx.xMin;
      final_rows  = fbx.yMax - fbx.yMin;

      /* adjust target bitmap to enlarged size */
      अगर ( target->width < final_width ||
           target->rows  < final_rows  )
      {
        पूर्णांक  pitch, new_pitch;

        अचिन्हित अक्षर*  बफर = शून्य;


        pitch = target->pitch;

        अगर ( pitch < 0 )
          pitch = -pitch;

        new_pitch = (पूर्णांक)final_width * 4;

        /* TODO: provide an in-buffer solution for large bitmaps */
        /*       to avoid allocation of a new buffer             */
        अगर ( FT_ALLOC_MULT( बफर, final_rows, new_pitch ) )
          जाओ Error;

        /* copy data to new buffer */

        /* the bitmap flow is from top to bottom, */
        /* but y is measured from bottom to top   */
        अगर ( target->pitch < 0 )
        {
          /* XXX */
        }
        अन्यथा
        {
          FT_Pos  x = tbx.xMin - fbx.xMin;
          FT_Pos  y = tbx.yMin - fbx.yMin;

          अचिन्हित अक्षर*  p =
            target->बफर;
          अचिन्हित अक्षर*  q =
            बफर +
            ( final_rows - y - target->rows ) * new_pitch +
            x * 4;
          अचिन्हित अक्षर*  limit_p =
            p + pitch * (पूर्णांक)target->rows;


          जबतक ( p < limit_p )
          {
            FT_MEM_COPY( q, p, pitch );

            p += pitch;
            q += new_pitch;
          }
        }

        FT_FREE( target->बफर );

        target->width = final_width;
        target->rows  = final_rows;

        अगर ( target->pitch < 0 )
          target->pitch = -new_pitch;
        अन्यथा
          target->pitch = new_pitch;

        target->बफर = बफर;
      }
    }

    FT_TRACE5(( "  final bitmap: (%ld, %ld) -- (%ld, %ld); %u x %u\n",
      fbx.xMin, fbx.yMin, fbx.xMax, fbx.yMax,
      final_width, final_rows ));

    /* adjust source bitmap if necessary */
    अगर ( source_->pixel_mode != FT_PIXEL_MODE_GRAY )
    {
      FT_Bitmap_Init( &source_bitmap );
      error = FT_Bitmap_Convert( library, source_, &source_bitmap, 1 );
      अगर ( error )
        जाओ Error;

      source             = &source_bitmap;
      free_source_bitmap = 1;
    }
    अन्यथा
      source = source_;

    /* do blending; the code below returns pre-multiplied channels, */
    /* similar to what FreeType gets from `CBDT' tables             */

    /* the bitmap flow is from top to bottom, */
    /* but y is measured from bottom to top   */
    अगर ( target->pitch < 0 )
    {
      /* XXX */
    }
    अन्यथा
    {
      FT_Pos  x = sbx.xMin - fbx.xMin;
      FT_Pos  y = sbx.yMin - fbx.yMin;

      अचिन्हित अक्षर*  p =
        source->बफर;
      अचिन्हित अक्षर*  q =
        target->बफर +
        ( target->rows - y - source->rows ) * target->pitch +
        x * 4;
      अचिन्हित अक्षर*  limit_p =
        p + source->pitch * (पूर्णांक)source->rows;


      जबतक ( p < limit_p )
      {
        अचिन्हित अक्षर*  r       = p;
        अचिन्हित अक्षर*  s       = q;
        अचिन्हित अक्षर*  limit_r = r + source->width;


        जबतक ( r < limit_r )
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
    अगर ( error && free_target_bitmap_on_error )
      FT_Bitmap_Done( library, target );

    अगर ( free_source_bitmap )
      FT_Bitmap_Done( library, &source_bitmap );

    वापस error;
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_GlyphSlot_Own_Bitmap( FT_GlyphSlot  slot )
  {
    अगर ( slot && slot->format == FT_GLYPH_FORMAT_BITMAP   &&
         !( slot->internal->flags & FT_GLYPH_OWN_BITMAP ) )
    {
      FT_Bitmap  bitmap;
      FT_Error   error;


      FT_Bitmap_Init( &bitmap );
      error = FT_Bitmap_Copy( slot->library, &slot->bitmap, &bitmap );
      अगर ( error )
        वापस error;

      slot->bitmap = bitmap;
      slot->internal->flags |= FT_GLYPH_OWN_BITMAP;
    }

    वापस FT_Err_Ok;
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Bitmap_Done( FT_Library  library,
                  FT_Bitmap  *bitmap )
  {
    FT_Memory  memory;


    अगर ( !library )
      वापस FT_THROW( Invalid_Library_Handle );

    अगर ( !bitmap )
      वापस FT_THROW( Invalid_Argument );

    memory = library->memory;

    FT_FREE( bitmap->बफर );
    *bitmap = null_bitmap;

    वापस FT_Err_Ok;
  }


/* END */
