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
#وصف_رد  FT_COMPONENT
#وصف_ڪريو FT_COMPONENT  bitmap


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
                  ثابت FT_Bitmap  *ذريعو,
                  FT_Bitmap        *هدف)
  {
    FT_يادگيري  يادگيري;
    FT_Error   error  = FT_Err_Ok;
    FT_Int     pitch;
    FT_Int     flip;


    إذا ( !library )
      أرجع FT_THROW( Invalid_Library_Handle );

    إذا ( !ذريعو || !هدف )
      أرجع FT_THROW( Invalid_دليل );

    إذا ( ذريعو == هدف )
      أرجع FT_Err_Ok;

    flip = ( ذريعو->pitch < 0 && هدف->pitch > 0 ) ||
           ( ذريعو->pitch > 0 && هدف->pitch < 0 );

    يادگيري = library->يادگيري;
    FT_FREE( هدف->بفر );

    *هدف = *ذريعو;

    إذا ( flip )
      هدف->pitch = -هدف->pitch;

    إذا ( !ذريعو->بفر )
      أرجع FT_Err_Ok;

    pitch  = ذريعو->pitch;
    إذا ( pitch < 0 )
      pitch = -pitch;

    FT_MEM_QALLOC_MULT( هدف->بفر, هدف->صفون, pitch );

    إذا ( !error )
    {
      إذا ( flip )
      {
        /* take care of bitmap flow */
        FT_UInt   i;
        FT_Byte*  s = ذريعو->بفر;
        FT_Byte*  t = هدف->بفر;


        t += (FT_ULong)pitch * ( هدف->صفون - 1 );

        لكل ( i = هدف->صفون; i > 0; i-- )
        {
          FT_ARRAY_COPY( t, s, pitch );

          s += pitch;
          t -= pitch;
        }
      }
      وإلا
        FT_MEM_COPY( هدف->بفر, ذريعو->بفر,
                     (FT_Long)ذريعو->صفون * pitch );
    }

    أرجع error;
  }


  /* Enlarge `bitmap' horizontally and vertically by `xpixels' */
  /* and `ypixels', respectively.                              */

  ساكن FT_Error
  ft_bitmap_assure_بفر( FT_يادگيري   يادگيري,
                           FT_Bitmap*  bitmap,
                           FT_UInt     xpixels,
                           FT_UInt     ypixels )
  {
    FT_Error        error;
    بنانشان صحيح    pitch;
    بنانشان صحيح    new_pitch;
    FT_UInt         bpp;
    FT_UInt         ويڪر, اوچائي;
    بنانشان حرف*  بفر = عدم;


    ويڪر  = bitmap->ويڪر;
    اوچائي = bitmap->صفون;
    pitch  = (بنانشان صحيح)FT_ABS( bitmap->pitch );

    بدل ( bitmap->pixel_گهڻائي )
    {
    حالة FT_PIXEL_گهڻائي_MONO:
      bpp       = 1;
      new_pitch = ( ويڪر + xpixels + 7 ) >> 3;
      اكسر;
    حالة FT_PIXEL_گهڻائي_GRAY2:
      bpp       = 2;
      new_pitch = ( ويڪر + xpixels + 3 ) >> 2;
      اكسر;
    حالة FT_PIXEL_گهڻائي_GRAY4:
      bpp       = 4;
      new_pitch = ( ويڪر + xpixels + 1 ) >> 1;
      اكسر;
    حالة FT_PIXEL_گهڻائي_ناسي:
    حالة FT_PIXEL_گهڻائي_LCD:
    حالة FT_PIXEL_گهڻائي_LCD_V:
      bpp       = 8;
      new_pitch = ويڪر + xpixels;
      اكسر;
    افتراضي:
      أرجع FT_THROW( Invalid_Glyph_Format );
    }

    /* if no need to allocate memory */
    إذا ( ypixels == 0 && new_pitch <= pitch )
    {
      /* zero the padding */
      FT_UInt  ٻٽو_انگ_ويڪر = pitch * 8;
      FT_UInt  ٻٽو_انگ_آخري  = ( ويڪر + xpixels ) * bpp;


      إذا ( ٻٽو_انگ_آخري < ٻٽو_انگ_ويڪر )
      {
        FT_Byte*  سٽ  = bitmap->بفر + ( ٻٽو_انگ_آخري >> 3 );
        FT_Byte*  پڇاڙي   = bitmap->بفر + pitch;
        FT_UInt   سرڪايو = ٻٽو_انگ_آخري & 7;
        FT_UInt   نقاب  = 0xFF00U >> سرڪايو;
        FT_UInt   ڳڻپ = اوچائي;


        لكل ( ; ڳڻپ > 0; ڳڻپ--, سٽ += pitch, پڇاڙي += pitch )
        {
          FT_Byte*  اكتب = سٽ;


          إذا ( سرڪايو > 0 )
          {
            اكتب[0] = (FT_Byte)( اكتب[0] & نقاب );
            اكتب++;
          }
          إذا ( اكتب < پڇاڙي )
            FT_MEM_ZERO( اكتب, پڇاڙي - اكتب );
        }
      }

      أرجع FT_Err_Ok;
    }

    /* otherwise allocate new buffer */
    إذا ( FT_QALLOC_MULT( بفر, bitmap->صفون + ypixels, new_pitch ) )
      أرجع error;

    /* new rows get added at the top of the bitmap, */
    /* thus take care of the flow direction         */
    إذا ( bitmap->pitch > 0 )
    {
      FT_UInt  len = ( ويڪر * bpp + 7 ) >> 3;

      بنانشان حرف*  اندريون  = bitmap->بفر;
      بنانشان حرف*  ٻاهريون = بفر;

      بنانشان حرف*  حد_بندي = bitmap->بفر + pitch * bitmap->صفون;
      بنانشان صحيح    delta = new_pitch - len;


      FT_MEM_ZERO( ٻاهريون, new_pitch * ypixels );
      ٻاهريون += new_pitch * ypixels;

      طالما ( اندريون < حد_بندي )
      {
        FT_MEM_COPY( ٻاهريون, اندريون, len );
        اندريون  += pitch;
        ٻاهريون += len;

        /* we use FT_QALLOC_MULT, which doesn't zero out the buffer;      */
        /* consequently, we have to manually zero out the remaining bytes */
        FT_MEM_ZERO( ٻاهريون, delta );
        ٻاهريون += delta;
      }
    }
    وإلا
    {
      FT_UInt  len = ( ويڪر * bpp + 7 ) >> 3;

      بنانشان حرف*  اندريون  = bitmap->بفر;
      بنانشان حرف*  ٻاهريون = بفر;

      بنانشان حرف*  حد_بندي = bitmap->بفر + pitch * bitmap->صفون;
      بنانشان صحيح    delta = new_pitch - len;


      طالما ( اندريون < حد_بندي )
      {
        FT_MEM_COPY( ٻاهريون, اندريون, len );
        اندريون  += pitch;
        ٻاهريون += len;

        FT_MEM_ZERO( ٻاهريون, delta );
        ٻاهريون += delta;
      }

      FT_MEM_ZERO( ٻاهريون, new_pitch * ypixels );
    }

    FT_FREE( bitmap->بفر );
    bitmap->بفر = بفر;

    /* set pitch only, width and height are left untouched */
    إذا ( bitmap->pitch < 0 )
      bitmap->pitch = -(صحيح)new_pitch;
    وإلا
      bitmap->pitch = (صحيح)new_pitch;

    أرجع FT_Err_Ok;
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Bitmap_Embolden( FT_Library  library,
                      FT_Bitmap*  bitmap,
                      FT_Pos      xStrength,
                      FT_Pos      yStrength )
  {
    FT_Error        error;
    بنانشان حرف*  p;
    FT_Int          i, x, pitch;
    FT_UInt         y;
    FT_Int          xstr, ystr;


    إذا ( !library )
      أرجع FT_THROW( Invalid_Library_Handle );

    إذا ( !bitmap || !bitmap->بفر )
      أرجع FT_THROW( Invalid_دليل );

    إذا ( ( ( FT_PIX_ڦيرو( xStrength ) >> 6 ) > FT_INT_MAX ) ||
         ( ( FT_PIX_ڦيرو( yStrength ) >> 6 ) > FT_INT_MAX ) )
      أرجع FT_THROW( Invalid_دليل );

    xstr = (FT_Int)FT_PIX_ڦيرو( xStrength ) >> 6;
    ystr = (FT_Int)FT_PIX_ڦيرو( yStrength ) >> 6;

    إذا ( xstr == 0 && ystr == 0 )
      أرجع FT_Err_Ok;
    وإلا إذا ( xstr < 0 || ystr < 0 )
      أرجع FT_THROW( Invalid_دليل );

    بدل ( bitmap->pixel_گهڻائي )
    {
    حالة FT_PIXEL_گهڻائي_GRAY2:
    حالة FT_PIXEL_گهڻائي_GRAY4:
      {
        FT_Bitmap  tmp;


        /* convert to 8bpp */
        FT_Bitmap_Init( &tmp );
        error = FT_Bitmap_Convert( library, bitmap, &tmp, 1 );
        إذا ( error )
          أرجع error;

        FT_Bitmap_پورو( library, bitmap );
        *bitmap = tmp;
      }
      اكسر;

    حالة FT_PIXEL_گهڻائي_MONO:
      إذا ( xstr > 8 )
        xstr = 8;
      اكسر;

    حالة FT_PIXEL_گهڻائي_LCD:
      xstr *= 3;
      اكسر;

    حالة FT_PIXEL_گهڻائي_LCD_V:
      ystr *= 3;
      اكسر;

    حالة FT_PIXEL_گهڻائي_BGRA:
      /* We don't embolden color glyphs. */
      أرجع FT_Err_Ok;
    }

    error = ft_bitmap_assure_بفر( library->يادگيري, bitmap,
                                     (FT_UInt)xstr, (FT_UInt)ystr );
    إذا ( error )
      أرجع error;

    /* take care of bitmap flow */
    pitch = bitmap->pitch;
    إذا ( pitch > 0 )
      p = bitmap->بفر + pitch * ystr;
    وإلا
    {
      pitch = -pitch;
      p = bitmap->بفر + (FT_UInt)pitch * ( bitmap->صفون - 1 );
    }

    /* for each row */
    لكل ( y = 0; y < bitmap->صفون; y++ )
    {
      /*
       * Horizontally:
       *
       * From the last pixel on, make each pixel or'ed with the
       * `xstr' pixels before it.
       */
      لكل ( x = pitch - 1; x >= 0; x-- )
      {
        بنانشان حرف  tmp;


        tmp = p[x];
        لكل ( i = 1; i <= xstr; i++ )
        {
          إذا ( bitmap->pixel_گهڻائي == FT_PIXEL_گهڻائي_MONO )
          {
            p[x] |= tmp >> i;

            /* the maximum value of 8 for `xstr' comes from here */
            إذا ( x > 0 )
              p[x] |= p[x - 1] << ( 8 - i );

#إذا 0
            إذا ( p[x] == 0xFF )
              اكسر;
#جيڪڏهن_پورو
          }
          وإلا
          {
            إذا ( x - i >= 0 )
            {
              إذا ( p[x] + p[x - i] > bitmap->انگ_grays - 1 )
              {
                p[x] = (بنانشان حرف)( bitmap->انگ_grays - 1 );
                اكسر;
              }
              وإلا
              {
                p[x] = (بنانشان حرف)( p[x] + p[x - i] );
                إذا ( p[x] == bitmap->انگ_grays - 1 )
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
        بنانشان حرف*  q;


        q = p - bitmap->pitch * x;
        لكل ( i = 0; i < pitch; i++ )
          q[i] |= p[i];
      }

      p += bitmap->pitch;
    }

    bitmap->ويڪر += (FT_UInt)xstr;
    bitmap->صفون += (FT_UInt)ystr;

    أرجع FT_Err_Ok;
  }


  ساكن FT_Byte
  ft_ناسي_for_premultiplied_srgb_bgra( ثابت FT_Byte*  bgra )
  {
    FT_UInt  a = bgra[3];
    FT_UInt  l;


    /* Short-circuit transparent color to avoid division by zero. */
    إذا ( !a )
      أرجع 0;

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

    أرجع (FT_Byte)( a - l / a );
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Bitmap_Convert( FT_Library        library,
                     ثابت FT_Bitmap  *ذريعو,
                     FT_Bitmap        *هدف,
                     FT_Int            alignment )
  {
    FT_Error   error = FT_Err_Ok;
    FT_يادگيري  يادگيري;

    FT_Byte*  s;
    FT_Byte*  t;


    إذا ( !library )
      أرجع FT_THROW( Invalid_Library_Handle );

    إذا ( !ذريعو || !هدف )
      أرجع FT_THROW( Invalid_دليل );

    يادگيري = library->يادگيري;

    بدل ( ذريعو->pixel_گهڻائي )
    {
    حالة FT_PIXEL_گهڻائي_MONO:
    حالة FT_PIXEL_گهڻائي_ناسي:
    حالة FT_PIXEL_گهڻائي_GRAY2:
    حالة FT_PIXEL_گهڻائي_GRAY4:
    حالة FT_PIXEL_گهڻائي_LCD:
    حالة FT_PIXEL_گهڻائي_LCD_V:
    حالة FT_PIXEL_گهڻائي_BGRA:
      {
        FT_Int  ويڪر = (FT_Int)ذريعو->ويڪر;
        FT_Int  neg   = ( هدف->pitch == 0 && ذريعو->pitch < 0 ) ||
                          هدف->pitch  < 0;


        FT_Bitmap_پورو( library, هدف );

        هدف->pixel_گهڻائي = FT_PIXEL_گهڻائي_ناسي;
        هدف->صفون       = ذريعو->صفون;
        هدف->ويڪر      = ذريعو->ويڪر;

        إذا ( alignment )
        {
          FT_Int  rem = ويڪر % alignment;


          إذا ( rem )
            ويڪر = alignment > 0 ? ويڪر - rem + alignment
                                  : ويڪر - rem - alignment;
        }

        إذا ( FT_QALLOC_MULT( هدف->بفر, هدف->صفون, ويڪر ) )
          أرجع error;

        هدف->pitch = neg ? -ويڪر : ويڪر;
      }
      اكسر;

    افتراضي:
      error = FT_THROW( Invalid_دليل );
    }

    s = ذريعو->بفر;
    t = هدف->بفر;

    /* take care of bitmap flow */
    إذا ( ذريعو->pitch < 0 )
      s -= ذريعو->pitch * (FT_Int)( ذريعو->صفون - 1 );
    إذا ( هدف->pitch < 0 )
      t -= هدف->pitch * (FT_Int)( هدف->صفون - 1 );

    بدل ( ذريعو->pixel_گهڻائي )
    {
    حالة FT_PIXEL_گهڻائي_MONO:
      {
        FT_UInt  i;


        هدف->انگ_grays = 2;

        لكل ( i = ذريعو->صفون; i > 0; i-- )
        {
          FT_Byte*  ss = s;
          FT_Byte*  tt = t;
          FT_UInt   j;


          /* get the full bytes */
          لكل ( j = ذريعو->ويڪر >> 3; j > 0; j-- )
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
          j = ذريعو->ويڪر & 7;
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

          s += ذريعو->pitch;
          t += هدف->pitch;
        }
      }
      اكسر;


    حالة FT_PIXEL_گهڻائي_ناسي:
    حالة FT_PIXEL_گهڻائي_LCD:
    حالة FT_PIXEL_گهڻائي_LCD_V:
      {
        FT_UInt  ويڪر = ذريعو->ويڪر;
        FT_UInt  i;


        هدف->انگ_grays = 256;

        لكل ( i = ذريعو->صفون; i > 0; i-- )
        {
          FT_ARRAY_COPY( t, s, ويڪر );

          s += ذريعو->pitch;
          t += هدف->pitch;
        }
      }
      اكسر;


    حالة FT_PIXEL_گهڻائي_GRAY2:
      {
        FT_UInt  i;


        هدف->انگ_grays = 4;

        لكل ( i = ذريعو->صفون; i > 0; i-- )
        {
          FT_Byte*  ss = s;
          FT_Byte*  tt = t;
          FT_UInt   j;


          /* get the full bytes */
          لكل ( j = ذريعو->ويڪر >> 2; j > 0; j-- )
          {
            FT_Int  val = ss[0];


            tt[0] = (FT_Byte)( ( val & 0xC0 ) >> 6 );
            tt[1] = (FT_Byte)( ( val & 0x30 ) >> 4 );
            tt[2] = (FT_Byte)( ( val & 0x0C ) >> 2 );
            tt[3] = (FT_Byte)( ( val & 0x03 ) );

            ss += 1;
            tt += 4;
          }

          j = ذريعو->ويڪر & 3;
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

          s += ذريعو->pitch;
          t += هدف->pitch;
        }
      }
      اكسر;


    حالة FT_PIXEL_گهڻائي_GRAY4:
      {
        FT_UInt  i;


        هدف->انگ_grays = 16;

        لكل ( i = ذريعو->صفون; i > 0; i-- )
        {
          FT_Byte*  ss = s;
          FT_Byte*  tt = t;
          FT_UInt   j;


          /* get the full bytes */
          لكل ( j = ذريعو->ويڪر >> 1; j > 0; j-- )
          {
            FT_Int  val = ss[0];


            tt[0] = (FT_Byte)( ( val & 0xF0 ) >> 4 );
            tt[1] = (FT_Byte)( ( val & 0x0F ) );

            ss += 1;
            tt += 2;
          }

          إذا ( ذريعو->ويڪر & 1 )
            tt[0] = (FT_Byte)( ( ss[0] & 0xF0 ) >> 4 );

          s += ذريعو->pitch;
          t += هدف->pitch;
        }
      }
      اكسر;


    حالة FT_PIXEL_گهڻائي_BGRA:
      {
        FT_UInt  i;


        هدف->انگ_grays = 256;

        لكل ( i = ذريعو->صفون; i > 0; i-- )
        {
          FT_Byte*  ss = s;
          FT_Byte*  tt = t;
          FT_UInt   j;


          لكل ( j = ذريعو->ويڪر; j > 0; j-- )
          {
            tt[0] = ft_ناسي_for_premultiplied_srgb_bgra( ss );

            ss += 4;
            tt += 1;
          }

          s += ذريعو->pitch;
          t += هدف->pitch;
        }
      }
      اكسر;

    افتراضي:
      ;
    }

    أرجع error;
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Bitmap_Blend( FT_Library        library,
                   ثابت FT_Bitmap*  ذريعو_2,
                   ثابت FT_ويڪٽر   ذريعو_وٿي,
                   FT_Bitmap*        هدف,
                   FT_ويڪٽر        *atarget_وٿي,
                   FT_Color          color )
  {
    FT_Error   error = FT_Err_Ok;
    FT_يادگيري  يادگيري;

    FT_Bitmap         ذريعو_bitmap;
    ثابت FT_Bitmap*  ذريعو;

    FT_Bool  free_ذريعو_bitmap          = 0;
    FT_Bool  free_هدف_bitmap_on_error = 0;

    FT_BBox  sbx, tbx, fbx;

    بنانشان صحيح  final_صفون, final_ويڪر;


    إذا ( !library || !هدف || !ذريعو_2 || !atarget_وٿي )
      أرجع FT_THROW( Invalid_دليل );

    يادگيري = library->يادگيري;

    إذا ( !( هدف->pixel_گهڻائي == FT_PIXEL_گهڻائي_NONE     ||
            ( هدف->pixel_گهڻائي == FT_PIXEL_گهڻائي_BGRA &&
              هدف->بفر                           ) ) )
      أرجع FT_THROW( Invalid_دليل );

    إذا ( ذريعو_2->pixel_گهڻائي == FT_PIXEL_گهڻائي_NONE )
      أرجع FT_Err_Ok;               /* nothing to do */

    /* pitches must have the same sign */
    إذا ( هدف->pixel_گهڻائي == FT_PIXEL_گهڻائي_BGRA &&
         ( ذريعو_2->pitch ^ هدف->pitch ) < 0   )
      أرجع FT_THROW( Invalid_دليل );

    إذا ( !( ذريعو_2->ويڪر && ذريعو_2->صفون ) )
      أرجع FT_Err_Ok;               /* nothing to do */

    /* get source bitmap dimensions assuming integer offsets */
    sbx.xMin = ( ذريعو_وٿي.x >> 6 );
    sbx.yMin = ( ذريعو_وٿي.y >> 6 ) - ذريعو_2->صفون;
    sbx.xMax = ( ذريعو_وٿي.x >> 6 ) + ذريعو_2->ويڪر;
    sbx.yMax = ( ذريعو_وٿي.y >> 6 );

    FT_TRACE5(( "FT_Bitmap_Blend:\n" ));
    FT_TRACE5(( "  source bitmap: (%ld, %ld) -- (%ld, %ld); %u x %u\n",
      sbx.xMin, sbx.yMin, sbx.xMax, sbx.yMax,
      ذريعو_2->ويڪر, ذريعو_2->صفون ));

    /* sanity check */
    إذا ( sbx.xMin > sbx.xMax      || sbx.yMin > sbx.yMax     ||
         ذريعو_2->ويڪر > 0x7FFFU || ذريعو_2->صفون > 0x7FFFU )
    {
      FT_TRACE5(( "FT_Bitmap_Blend: source dimension overflow\n" ));
      أرجع FT_THROW( Invalid_دليل );
    }

    إذا ( !هدف->ويڪر || !هدف->صفون )
    {
      FT_TRACE5(( "  target bitmap: empty\n" ));

      final_ويڪر = ذريعو_2->ويڪر;
      final_صفون  = ذريعو_2->صفون;

      /* create new empty bitmap */
      هدف->ويڪر      = final_ويڪر;
      هدف->صفون       = final_صفون;
      هدف->pixel_گهڻائي = FT_PIXEL_گهڻائي_BGRA;
      هدف->pitch      = (صحيح)final_ويڪر * 4;
      هدف->انگ_grays  = 256;

      إذا ( FT_ALLOC_MULT( هدف->بفر, هدف->صفون, هدف->pitch ) )
        أرجع error;

      free_هدف_bitmap_on_error = 1;

      fbx = sbx;
    }
    وإلا
    {
      /* get target bitmap dimensions assuming integer offsets */
      tbx.xMin = ( atarget_وٿي->x >> 6 );
      tbx.yMin = ( atarget_وٿي->y >> 6 ) - هدف->صفون;
      tbx.xMax = ( atarget_وٿي->x >> 6 ) + هدف->ويڪر;
      tbx.yMax = ( atarget_وٿي->y >> 6 );

      FT_TRACE5(( "  target bitmap: (%ld, %ld) -- (%ld, %ld); %u x %u\n",
        tbx.xMin, tbx.yMin, tbx.xMax, tbx.yMax,
        هدف->ويڪر, هدف->صفون ));

      /* sanity check */
      إذا ( tbx.xMin > tbx.xMax || tbx.yMin > tbx.yMax )
      {
        FT_TRACE5(( "FT_Bitmap_Blend: targget dimension overflow\n" ));
        أرجع FT_THROW( Invalid_دليل );
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
        أرجع FT_THROW( Invalid_دليل );
      }

      final_ويڪر = fbx.xMax - fbx.xMin;
      final_صفون  = fbx.yMax - fbx.yMin;

      /* adjust target bitmap to enlarged size */
      إذا ( هدف->ويڪر < final_ويڪر ||
           هدف->صفون  < final_صفون  )
      {
        صحيح  pitch, new_pitch;

        بنانشان حرف*  بفر = عدم;


        pitch = هدف->pitch;

        إذا ( pitch < 0 )
          pitch = -pitch;

        new_pitch = (صحيح)final_ويڪر * 4;

        /* TODO: provide an in-buffer solution for large bitmaps */
        /*       to avoid allocation of a new buffer             */
        إذا ( FT_ALLOC_MULT( بفر, final_صفون, new_pitch ) )
          اذهب Error;

        /* copy data to new buffer */

        /* the bitmap flow is from top to bottom, */
        /* but y is measured from bottom to top   */
        إذا ( هدف->pitch < 0 )
        {
          /* XXX */
        }
        وإلا
        {
          FT_Pos  x = tbx.xMin - fbx.xMin;
          FT_Pos  y = tbx.yMin - fbx.yMin;

          بنانشان حرف*  p =
            هدف->بفر;
          بنانشان حرف*  q =
            بفر +
            ( final_صفون - y - هدف->صفون ) * new_pitch +
            x * 4;
          بنانشان حرف*  حد_بندي_p =
            p + pitch * (صحيح)هدف->صفون;


          طالما ( p < حد_بندي_p )
          {
            FT_MEM_COPY( q, p, pitch );

            p += pitch;
            q += new_pitch;
          }
        }

        FT_FREE( هدف->بفر );

        هدف->ويڪر = final_ويڪر;
        هدف->صفون  = final_صفون;

        إذا ( هدف->pitch < 0 )
          هدف->pitch = -new_pitch;
        وإلا
          هدف->pitch = new_pitch;

        هدف->بفر = بفر;
      }
    }

    FT_TRACE5(( "  final bitmap: (%ld, %ld) -- (%ld, %ld); %u x %u\n",
      fbx.xMin, fbx.yMin, fbx.xMax, fbx.yMax,
      final_ويڪر, final_صفون ));

    /* adjust source bitmap if necessary */
    إذا ( ذريعو_2->pixel_گهڻائي != FT_PIXEL_گهڻائي_ناسي )
    {
      FT_Bitmap_Init( &ذريعو_bitmap );
      error = FT_Bitmap_Convert( library, ذريعو_2, &ذريعو_bitmap, 1 );
      إذا ( error )
        اذهب Error;

      ذريعو             = &ذريعو_bitmap;
      free_ذريعو_bitmap = 1;
    }
    وإلا
      ذريعو = ذريعو_2;

    /* do blending; the code below returns pre-multiplied channels, */
    /* similar to what FreeType gets from `CBDT' tables             */

    /* the bitmap flow is from top to bottom, */
    /* but y is measured from bottom to top   */
    إذا ( هدف->pitch < 0 )
    {
      /* XXX */
    }
    وإلا
    {
      FT_Pos  x = sbx.xMin - fbx.xMin;
      FT_Pos  y = sbx.yMin - fbx.yMin;

      بنانشان حرف*  p =
        ذريعو->بفر;
      بنانشان حرف*  q =
        هدف->بفر +
        ( هدف->صفون - y - ذريعو->صفون ) * هدف->pitch +
        x * 4;
      بنانشان حرف*  حد_بندي_p =
        p + ذريعو->pitch * (صحيح)ذريعو->صفون;


      طالما ( p < حد_بندي_p )
      {
        بنانشان حرف*  r       = p;
        بنانشان حرف*  s       = q;
        بنانشان حرف*  حد_بندي_r = r + ذريعو->ويڪر;


        طالما ( r < حد_بندي_r )
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


          *s++ = (بنانشان حرف)( bb * ba2 / 255 + fb );
          *s++ = (بنانشان حرف)( bg * ba2 / 255 + fg );
          *s++ = (بنانشان حرف)( br * ba2 / 255 + fr );
          *s++ = (بنانشان حرف)( ba * ba2 / 255 + fa );
        }

        p += ذريعو->pitch;
        q += هدف->pitch;
      }
    }

    atarget_وٿي->x = fbx.xMin * 64;
    atarget_وٿي->y = fbx.yMax * 64;

  Error:
    إذا ( error && free_هدف_bitmap_on_error )
      FT_Bitmap_پورو( library, هدف );

    إذا ( free_ذريعو_bitmap )
      FT_Bitmap_پورو( library, &ذريعو_bitmap );

    أرجع error;
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Glyph_جڳهه_Own_Bitmap( FT_Glyph_جڳهه  جڳهه )
  {
    إذا ( جڳهه && جڳهه->format == FT_GLYPH_FORMAT_BITMAP   &&
         !( جڳهه->اندروني->flags & FT_GLYPH_OWN_BITMAP ) )
    {
      FT_Bitmap  bitmap;
      FT_Error   error;


      FT_Bitmap_Init( &bitmap );
      error = FT_Bitmap_Copy( جڳهه->library, &جڳهه->bitmap, &bitmap );
      إذا ( error )
        أرجع error;

      جڳهه->bitmap = bitmap;
      جڳهه->اندروني->flags |= FT_GLYPH_OWN_BITMAP;
    }

    أرجع FT_Err_Ok;
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Bitmap_پورو( FT_Library  library,
                  FT_Bitmap  *bitmap )
  {
    FT_يادگيري  يادگيري;


    إذا ( !library )
      أرجع FT_THROW( Invalid_Library_Handle );

    إذا ( !bitmap )
      أرجع FT_THROW( Invalid_دليل );

    يادگيري = library->يادگيري;

    FT_FREE( bitmap->بفر );
    *bitmap = null_bitmap;

    أرجع FT_Err_Ok;
  }


/*پڇاڙي*/
