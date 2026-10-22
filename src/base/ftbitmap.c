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
#لغو_تعریف  FT_COMPONENT
#تعریف_کن FT_COMPONENT  bitmap


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
                  ثابت FT_Bitmap  *منبع,
                  FT_Bitmap        *هدف)
  {
    FT_حافظه  حافظه;
    FT_Error   error  = FT_Err_Ok;
    FT_Int     pitch;
    FT_Int     flip;


    إذا ( !library )
      أرجع FT_THROW( Invalid_Library_Handle );

    إذا ( !منبع || !هدف )
      أرجع FT_THROW( Invalid_آرگومان );

    إذا ( منبع == هدف )
      أرجع FT_Err_Ok;

    flip = ( منبع->pitch < 0 && هدف->pitch > 0 ) ||
           ( منبع->pitch > 0 && هدف->pitch < 0 );

    حافظه = library->حافظه;
    FT_FREE( هدف->میانگیر );

    *هدف = *منبع;

    إذا ( flip )
      هدف->pitch = -هدف->pitch;

    إذا ( !منبع->میانگیر )
      أرجع FT_Err_Ok;

    pitch  = منبع->pitch;
    إذا ( pitch < 0 )
      pitch = -pitch;

    FT_MEM_QALLOC_MULT( هدف->میانگیر, هدف->ردیفها, pitch );

    إذا ( !error )
    {
      إذا ( flip )
      {
        /* take care of bitmap flow */
        FT_UInt   i;
        FT_Byte*  s = منبع->میانگیر;
        FT_Byte*  t = هدف->میانگیر;


        t += (FT_ULong)pitch * ( هدف->ردیفها - 1 );

        لكل ( i = هدف->ردیفها; i > 0; i-- )
        {
          FT_ARRAY_COPY( t, s, pitch );

          s += pitch;
          t -= pitch;
        }
      }
      وإلا
        FT_MEM_COPY( هدف->میانگیر, منبع->میانگیر,
                     (FT_Long)منبع->ردیفها * pitch );
    }

    أرجع error;
  }


  /* Enlarge `bitmap' horizontally and vertically by `xpixels' */
  /* and `ypixels', respectively.                              */

  ساكن FT_Error
  ft_bitmap_assure_میانگیر( FT_حافظه   حافظه,
                           FT_Bitmap*  bitmap,
                           FT_UInt     xpixels,
                           FT_UInt     ypixels )
  {
    FT_Error        error;
    بیعلامت صحيح    pitch;
    بیعلامت صحيح    new_pitch;
    FT_UInt         bpp;
    FT_UInt         عرض, ارتفاع;
    بیعلامت حرف*  میانگیر = عدم;


    عرض  = bitmap->عرض;
    ارتفاع = bitmap->ردیفها;
    pitch  = (بیعلامت صحيح)FT_ABS( bitmap->pitch );

    بدل ( bitmap->pixel_نما )
    {
    حالة FT_PIXEL_نما_MONO:
      bpp       = 1;
      new_pitch = ( عرض + xpixels + 7 ) >> 3;
      اكسر;
    حالة FT_PIXEL_نما_GRAY2:
      bpp       = 2;
      new_pitch = ( عرض + xpixels + 3 ) >> 2;
      اكسر;
    حالة FT_PIXEL_نما_GRAY4:
      bpp       = 4;
      new_pitch = ( عرض + xpixels + 1 ) >> 1;
      اكسر;
    حالة FT_PIXEL_نما_خاکستری:
    حالة FT_PIXEL_نما_LCD:
    حالة FT_PIXEL_نما_LCD_V:
      bpp       = 8;
      new_pitch = عرض + xpixels;
      اكسر;
    افتراضي:
      أرجع FT_THROW( Invalid_Glyph_Format );
    }

    /* if no need to allocate memory */
    إذا ( ypixels == 0 && new_pitch <= pitch )
    {
      /* zero the padding */
      FT_UInt  بیت_عرض = pitch * 8;
      FT_UInt  بیت_آخرین  = ( عرض + xpixels ) * bpp;


      إذا ( بیت_آخرین < بیت_عرض )
      {
        FT_Byte*  سطر  = bitmap->میانگیر + ( بیت_آخرین >> 3 );
        FT_Byte*  انتها   = bitmap->میانگیر + pitch;
        FT_UInt   لغزش = بیت_آخرین & 7;
        FT_UInt   ماسک  = 0xFF00U >> لغزش;
        FT_UInt   شمار = ارتفاع;


        لكل ( ; شمار > 0; شمار--, سطر += pitch, انتها += pitch )
        {
          FT_Byte*  اكتب = سطر;


          إذا ( لغزش > 0 )
          {
            اكتب[0] = (FT_Byte)( اكتب[0] & ماسک );
            اكتب++;
          }
          إذا ( اكتب < انتها )
            FT_MEM_ZERO( اكتب, انتها - اكتب );
        }
      }

      أرجع FT_Err_Ok;
    }

    /* otherwise allocate new buffer */
    إذا ( FT_QALLOC_MULT( میانگیر, bitmap->ردیفها + ypixels, new_pitch ) )
      أرجع error;

    /* new rows get added at the top of the bitmap, */
    /* thus take care of the flow direction         */
    إذا ( bitmap->pitch > 0 )
    {
      FT_UInt  len = ( عرض * bpp + 7 ) >> 3;

      بیعلامت حرف*  درون  = bitmap->میانگیر;
      بیعلامت حرف*  بیرون = میانگیر;

      بیعلامت حرف*  حد = bitmap->میانگیر + pitch * bitmap->ردیفها;
      بیعلامت صحيح    delta = new_pitch - len;


      FT_MEM_ZERO( بیرون, new_pitch * ypixels );
      بیرون += new_pitch * ypixels;

      طالما ( درون < حد )
      {
        FT_MEM_COPY( بیرون, درون, len );
        درون  += pitch;
        بیرون += len;

        /* we use FT_QALLOC_MULT, which doesn't zero out the buffer;      */
        /* consequently, we have to manually zero out the remaining bytes */
        FT_MEM_ZERO( بیرون, delta );
        بیرون += delta;
      }
    }
    وإلا
    {
      FT_UInt  len = ( عرض * bpp + 7 ) >> 3;

      بیعلامت حرف*  درون  = bitmap->میانگیر;
      بیعلامت حرف*  بیرون = میانگیر;

      بیعلامت حرف*  حد = bitmap->میانگیر + pitch * bitmap->ردیفها;
      بیعلامت صحيح    delta = new_pitch - len;


      طالما ( درون < حد )
      {
        FT_MEM_COPY( بیرون, درون, len );
        درون  += pitch;
        بیرون += len;

        FT_MEM_ZERO( بیرون, delta );
        بیرون += delta;
      }

      FT_MEM_ZERO( بیرون, new_pitch * ypixels );
    }

    FT_FREE( bitmap->میانگیر );
    bitmap->میانگیر = میانگیر;

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
    بیعلامت حرف*  p;
    FT_Int          i, x, pitch;
    FT_UInt         y;
    FT_Int          xstr, ystr;


    إذا ( !library )
      أرجع FT_THROW( Invalid_Library_Handle );

    إذا ( !bitmap || !bitmap->میانگیر )
      أرجع FT_THROW( Invalid_آرگومان );

    إذا ( ( ( FT_PIX_دور( xStrength ) >> 6 ) > FT_INT_MAX ) ||
         ( ( FT_PIX_دور( yStrength ) >> 6 ) > FT_INT_MAX ) )
      أرجع FT_THROW( Invalid_آرگومان );

    xstr = (FT_Int)FT_PIX_دور( xStrength ) >> 6;
    ystr = (FT_Int)FT_PIX_دور( yStrength ) >> 6;

    إذا ( xstr == 0 && ystr == 0 )
      أرجع FT_Err_Ok;
    وإلا إذا ( xstr < 0 || ystr < 0 )
      أرجع FT_THROW( Invalid_آرگومان );

    بدل ( bitmap->pixel_نما )
    {
    حالة FT_PIXEL_نما_GRAY2:
    حالة FT_PIXEL_نما_GRAY4:
      {
        FT_Bitmap  tmp;


        /* convert to 8bpp */
        FT_Bitmap_Init( &tmp );
        error = FT_Bitmap_Convert( library, bitmap, &tmp, 1 );
        إذا ( error )
          أرجع error;

        FT_Bitmap_انجامشده( library, bitmap );
        *bitmap = tmp;
      }
      اكسر;

    حالة FT_PIXEL_نما_MONO:
      إذا ( xstr > 8 )
        xstr = 8;
      اكسر;

    حالة FT_PIXEL_نما_LCD:
      xstr *= 3;
      اكسر;

    حالة FT_PIXEL_نما_LCD_V:
      ystr *= 3;
      اكسر;

    حالة FT_PIXEL_نما_BGRA:
      /* We don't embolden color glyphs. */
      أرجع FT_Err_Ok;
    }

    error = ft_bitmap_assure_میانگیر( library->حافظه, bitmap,
                                     (FT_UInt)xstr, (FT_UInt)ystr );
    إذا ( error )
      أرجع error;

    /* take care of bitmap flow */
    pitch = bitmap->pitch;
    إذا ( pitch > 0 )
      p = bitmap->میانگیر + pitch * ystr;
    وإلا
    {
      pitch = -pitch;
      p = bitmap->میانگیر + (FT_UInt)pitch * ( bitmap->ردیفها - 1 );
    }

    /* for each row */
    لكل ( y = 0; y < bitmap->ردیفها; y++ )
    {
      /*
       * Horizontally:
       *
       * From the last pixel on, make each pixel or'ed with the
       * `xstr' pixels before it.
       */
      لكل ( x = pitch - 1; x >= 0; x-- )
      {
        بیعلامت حرف  tmp;


        tmp = p[x];
        لكل ( i = 1; i <= xstr; i++ )
        {
          إذا ( bitmap->pixel_نما == FT_PIXEL_نما_MONO )
          {
            p[x] |= tmp >> i;

            /* the maximum value of 8 for `xstr' comes from here */
            إذا ( x > 0 )
              p[x] |= p[x - 1] << ( 8 - i );

#إذا 0
            إذا ( p[x] == 0xFF )
              اكسر;
#پایان_اگر
          }
          وإلا
          {
            إذا ( x - i >= 0 )
            {
              إذا ( p[x] + p[x - i] > bitmap->رقم_grays - 1 )
              {
                p[x] = (بیعلامت حرف)( bitmap->رقم_grays - 1 );
                اكسر;
              }
              وإلا
              {
                p[x] = (بیعلامت حرف)( p[x] + p[x - i] );
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
        بیعلامت حرف*  q;


        q = p - bitmap->pitch * x;
        لكل ( i = 0; i < pitch; i++ )
          q[i] |= p[i];
      }

      p += bitmap->pitch;
    }

    bitmap->عرض += (FT_UInt)xstr;
    bitmap->ردیفها += (FT_UInt)ystr;

    أرجع FT_Err_Ok;
  }


  ساكن FT_Byte
  ft_خاکستری_for_premultiplied_srgb_bgra( ثابت FT_Byte*  bgra )
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
                     ثابت FT_Bitmap  *منبع,
                     FT_Bitmap        *هدف,
                     FT_Int            alignment )
  {
    FT_Error   error = FT_Err_Ok;
    FT_حافظه  حافظه;

    FT_Byte*  s;
    FT_Byte*  t;


    إذا ( !library )
      أرجع FT_THROW( Invalid_Library_Handle );

    إذا ( !منبع || !هدف )
      أرجع FT_THROW( Invalid_آرگومان );

    حافظه = library->حافظه;

    بدل ( منبع->pixel_نما )
    {
    حالة FT_PIXEL_نما_MONO:
    حالة FT_PIXEL_نما_خاکستری:
    حالة FT_PIXEL_نما_GRAY2:
    حالة FT_PIXEL_نما_GRAY4:
    حالة FT_PIXEL_نما_LCD:
    حالة FT_PIXEL_نما_LCD_V:
    حالة FT_PIXEL_نما_BGRA:
      {
        FT_Int  عرض = (FT_Int)منبع->عرض;
        FT_Int  neg   = ( هدف->pitch == 0 && منبع->pitch < 0 ) ||
                          هدف->pitch  < 0;


        FT_Bitmap_انجامشده( library, هدف );

        هدف->pixel_نما = FT_PIXEL_نما_خاکستری;
        هدف->ردیفها       = منبع->ردیفها;
        هدف->عرض      = منبع->عرض;

        إذا ( alignment )
        {
          FT_Int  rem = عرض % alignment;


          إذا ( rem )
            عرض = alignment > 0 ? عرض - rem + alignment
                                  : عرض - rem - alignment;
        }

        إذا ( FT_QALLOC_MULT( هدف->میانگیر, هدف->ردیفها, عرض ) )
          أرجع error;

        هدف->pitch = neg ? -عرض : عرض;
      }
      اكسر;

    افتراضي:
      error = FT_THROW( Invalid_آرگومان );
    }

    s = منبع->میانگیر;
    t = هدف->میانگیر;

    /* take care of bitmap flow */
    إذا ( منبع->pitch < 0 )
      s -= منبع->pitch * (FT_Int)( منبع->ردیفها - 1 );
    إذا ( هدف->pitch < 0 )
      t -= هدف->pitch * (FT_Int)( هدف->ردیفها - 1 );

    بدل ( منبع->pixel_نما )
    {
    حالة FT_PIXEL_نما_MONO:
      {
        FT_UInt  i;


        هدف->رقم_grays = 2;

        لكل ( i = منبع->ردیفها; i > 0; i-- )
        {
          FT_Byte*  ss = s;
          FT_Byte*  tt = t;
          FT_UInt   j;


          /* get the full bytes */
          لكل ( j = منبع->عرض >> 3; j > 0; j-- )
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
          j = منبع->عرض & 7;
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

          s += منبع->pitch;
          t += هدف->pitch;
        }
      }
      اكسر;


    حالة FT_PIXEL_نما_خاکستری:
    حالة FT_PIXEL_نما_LCD:
    حالة FT_PIXEL_نما_LCD_V:
      {
        FT_UInt  عرض = منبع->عرض;
        FT_UInt  i;


        هدف->رقم_grays = 256;

        لكل ( i = منبع->ردیفها; i > 0; i-- )
        {
          FT_ARRAY_COPY( t, s, عرض );

          s += منبع->pitch;
          t += هدف->pitch;
        }
      }
      اكسر;


    حالة FT_PIXEL_نما_GRAY2:
      {
        FT_UInt  i;


        هدف->رقم_grays = 4;

        لكل ( i = منبع->ردیفها; i > 0; i-- )
        {
          FT_Byte*  ss = s;
          FT_Byte*  tt = t;
          FT_UInt   j;


          /* get the full bytes */
          لكل ( j = منبع->عرض >> 2; j > 0; j-- )
          {
            FT_Int  val = ss[0];


            tt[0] = (FT_Byte)( ( val & 0xC0 ) >> 6 );
            tt[1] = (FT_Byte)( ( val & 0x30 ) >> 4 );
            tt[2] = (FT_Byte)( ( val & 0x0C ) >> 2 );
            tt[3] = (FT_Byte)( ( val & 0x03 ) );

            ss += 1;
            tt += 4;
          }

          j = منبع->عرض & 3;
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

          s += منبع->pitch;
          t += هدف->pitch;
        }
      }
      اكسر;


    حالة FT_PIXEL_نما_GRAY4:
      {
        FT_UInt  i;


        هدف->رقم_grays = 16;

        لكل ( i = منبع->ردیفها; i > 0; i-- )
        {
          FT_Byte*  ss = s;
          FT_Byte*  tt = t;
          FT_UInt   j;


          /* get the full bytes */
          لكل ( j = منبع->عرض >> 1; j > 0; j-- )
          {
            FT_Int  val = ss[0];


            tt[0] = (FT_Byte)( ( val & 0xF0 ) >> 4 );
            tt[1] = (FT_Byte)( ( val & 0x0F ) );

            ss += 1;
            tt += 2;
          }

          إذا ( منبع->عرض & 1 )
            tt[0] = (FT_Byte)( ( ss[0] & 0xF0 ) >> 4 );

          s += منبع->pitch;
          t += هدف->pitch;
        }
      }
      اكسر;


    حالة FT_PIXEL_نما_BGRA:
      {
        FT_UInt  i;


        هدف->رقم_grays = 256;

        لكل ( i = منبع->ردیفها; i > 0; i-- )
        {
          FT_Byte*  ss = s;
          FT_Byte*  tt = t;
          FT_UInt   j;


          لكل ( j = منبع->عرض; j > 0; j-- )
          {
            tt[0] = ft_خاکستری_for_premultiplied_srgb_bgra( ss );

            ss += 4;
            tt += 1;
          }

          s += منبع->pitch;
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
                   ثابت FT_Bitmap*  منبع_2,
                   ثابت FT_وکتور   منبع_جابه_جایی,
                   FT_Bitmap*        هدف,
                   FT_وکتور        *atarget_جابه_جایی,
                   FT_Color          color )
  {
    FT_Error   error = FT_Err_Ok;
    FT_حافظه  حافظه;

    FT_Bitmap         منبع_bitmap;
    ثابت FT_Bitmap*  منبع;

    FT_Bool  free_منبع_bitmap          = 0;
    FT_Bool  free_هدف_bitmap_on_error = 0;

    FT_BBox  sbx, tbx, fbx;

    بیعلامت صحيح  final_ردیفها, final_عرض;


    إذا ( !library || !هدف || !منبع_2 || !atarget_جابه_جایی )
      أرجع FT_THROW( Invalid_آرگومان );

    حافظه = library->حافظه;

    إذا ( !( هدف->pixel_نما == FT_PIXEL_نما_NONE     ||
            ( هدف->pixel_نما == FT_PIXEL_نما_BGRA &&
              هدف->میانگیر                           ) ) )
      أرجع FT_THROW( Invalid_آرگومان );

    إذا ( منبع_2->pixel_نما == FT_PIXEL_نما_NONE )
      أرجع FT_Err_Ok;               /* nothing to do */

    /* pitches must have the same sign */
    إذا ( هدف->pixel_نما == FT_PIXEL_نما_BGRA &&
         ( منبع_2->pitch ^ هدف->pitch ) < 0   )
      أرجع FT_THROW( Invalid_آرگومان );

    إذا ( !( منبع_2->عرض && منبع_2->ردیفها ) )
      أرجع FT_Err_Ok;               /* nothing to do */

    /* get source bitmap dimensions assuming integer offsets */
    sbx.xMin = ( منبع_جابه_جایی.x >> 6 );
    sbx.yMin = ( منبع_جابه_جایی.y >> 6 ) - منبع_2->ردیفها;
    sbx.xMax = ( منبع_جابه_جایی.x >> 6 ) + منبع_2->عرض;
    sbx.yMax = ( منبع_جابه_جایی.y >> 6 );

    FT_TRACE5(( "FT_Bitmap_Blend:\n" ));
    FT_TRACE5(( "  source bitmap: (%ld, %ld) -- (%ld, %ld); %u x %u\n",
      sbx.xMin, sbx.yMin, sbx.xMax, sbx.yMax,
      منبع_2->عرض, منبع_2->ردیفها ));

    /* sanity check */
    إذا ( sbx.xMin > sbx.xMax      || sbx.yMin > sbx.yMax     ||
         منبع_2->عرض > 0x7FFFU || منبع_2->ردیفها > 0x7FFFU )
    {
      FT_TRACE5(( "FT_Bitmap_Blend: source dimension overflow\n" ));
      أرجع FT_THROW( Invalid_آرگومان );
    }

    إذا ( !هدف->عرض || !هدف->ردیفها )
    {
      FT_TRACE5(( "  target bitmap: empty\n" ));

      final_عرض = منبع_2->عرض;
      final_ردیفها  = منبع_2->ردیفها;

      /* create new empty bitmap */
      هدف->عرض      = final_عرض;
      هدف->ردیفها       = final_ردیفها;
      هدف->pixel_نما = FT_PIXEL_نما_BGRA;
      هدف->pitch      = (صحيح)final_عرض * 4;
      هدف->رقم_grays  = 256;

      إذا ( FT_ALLOC_MULT( هدف->میانگیر, هدف->ردیفها, هدف->pitch ) )
        أرجع error;

      free_هدف_bitmap_on_error = 1;

      fbx = sbx;
    }
    وإلا
    {
      /* get target bitmap dimensions assuming integer offsets */
      tbx.xMin = ( atarget_جابه_جایی->x >> 6 );
      tbx.yMin = ( atarget_جابه_جایی->y >> 6 ) - هدف->ردیفها;
      tbx.xMax = ( atarget_جابه_جایی->x >> 6 ) + هدف->عرض;
      tbx.yMax = ( atarget_جابه_جایی->y >> 6 );

      FT_TRACE5(( "  target bitmap: (%ld, %ld) -- (%ld, %ld); %u x %u\n",
        tbx.xMin, tbx.yMin, tbx.xMax, tbx.yMax,
        هدف->عرض, هدف->ردیفها ));

      /* sanity check */
      إذا ( tbx.xMin > tbx.xMax || tbx.yMin > tbx.yMax )
      {
        FT_TRACE5(( "FT_Bitmap_Blend: targget dimension overflow\n" ));
        أرجع FT_THROW( Invalid_آرگومان );
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
        أرجع FT_THROW( Invalid_آرگومان );
      }

      final_عرض = fbx.xMax - fbx.xMin;
      final_ردیفها  = fbx.yMax - fbx.yMin;

      /* adjust target bitmap to enlarged size */
      إذا ( هدف->عرض < final_عرض ||
           هدف->ردیفها  < final_ردیفها  )
      {
        صحيح  pitch, new_pitch;

        بیعلامت حرف*  میانگیر = عدم;


        pitch = هدف->pitch;

        إذا ( pitch < 0 )
          pitch = -pitch;

        new_pitch = (صحيح)final_عرض * 4;

        /* TODO: provide an in-buffer solution for large bitmaps */
        /*       to avoid allocation of a new buffer             */
        إذا ( FT_ALLOC_MULT( میانگیر, final_ردیفها, new_pitch ) )
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

          بیعلامت حرف*  p =
            هدف->میانگیر;
          بیعلامت حرف*  q =
            میانگیر +
            ( final_ردیفها - y - هدف->ردیفها ) * new_pitch +
            x * 4;
          بیعلامت حرف*  حد_p =
            p + pitch * (صحيح)هدف->ردیفها;


          طالما ( p < حد_p )
          {
            FT_MEM_COPY( q, p, pitch );

            p += pitch;
            q += new_pitch;
          }
        }

        FT_FREE( هدف->میانگیر );

        هدف->عرض = final_عرض;
        هدف->ردیفها  = final_ردیفها;

        إذا ( هدف->pitch < 0 )
          هدف->pitch = -new_pitch;
        وإلا
          هدف->pitch = new_pitch;

        هدف->میانگیر = میانگیر;
      }
    }

    FT_TRACE5(( "  final bitmap: (%ld, %ld) -- (%ld, %ld); %u x %u\n",
      fbx.xMin, fbx.yMin, fbx.xMax, fbx.yMax,
      final_عرض, final_ردیفها ));

    /* adjust source bitmap if necessary */
    إذا ( منبع_2->pixel_نما != FT_PIXEL_نما_خاکستری )
    {
      FT_Bitmap_Init( &منبع_bitmap );
      error = FT_Bitmap_Convert( library, منبع_2, &منبع_bitmap, 1 );
      إذا ( error )
        اذهب Error;

      منبع             = &منبع_bitmap;
      free_منبع_bitmap = 1;
    }
    وإلا
      منبع = منبع_2;

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

      بیعلامت حرف*  p =
        منبع->میانگیر;
      بیعلامت حرف*  q =
        هدف->میانگیر +
        ( هدف->ردیفها - y - منبع->ردیفها ) * هدف->pitch +
        x * 4;
      بیعلامت حرف*  حد_p =
        p + منبع->pitch * (صحيح)منبع->ردیفها;


      طالما ( p < حد_p )
      {
        بیعلامت حرف*  r       = p;
        بیعلامت حرف*  s       = q;
        بیعلامت حرف*  حد_r = r + منبع->عرض;


        طالما ( r < حد_r )
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


          *s++ = (بیعلامت حرف)( bb * ba2 / 255 + fb );
          *s++ = (بیعلامت حرف)( bg * ba2 / 255 + fg );
          *s++ = (بیعلامت حرف)( br * ba2 / 255 + fr );
          *s++ = (بیعلامت حرف)( ba * ba2 / 255 + fa );
        }

        p += منبع->pitch;
        q += هدف->pitch;
      }
    }

    atarget_جابه_جایی->x = fbx.xMin * 64;
    atarget_جابه_جایی->y = fbx.yMax * 64;

  Error:
    إذا ( error && free_هدف_bitmap_on_error )
      FT_Bitmap_انجامشده( library, هدف );

    إذا ( free_منبع_bitmap )
      FT_Bitmap_انجامشده( library, &منبع_bitmap );

    أرجع error;
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Glyph_جایگاه_Own_Bitmap( FT_Glyph_جایگاه  جایگاه )
  {
    إذا ( جایگاه && جایگاه->format == FT_GLYPH_FORMAT_BITMAP   &&
         !( جایگاه->داخلی->flags & FT_GLYPH_OWN_BITMAP ) )
    {
      FT_Bitmap  bitmap;
      FT_Error   error;


      FT_Bitmap_Init( &bitmap );
      error = FT_Bitmap_Copy( جایگاه->library, &جایگاه->bitmap, &bitmap );
      إذا ( error )
        أرجع error;

      جایگاه->bitmap = bitmap;
      جایگاه->داخلی->flags |= FT_GLYPH_OWN_BITMAP;
    }

    أرجع FT_Err_Ok;
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Bitmap_انجامشده( FT_Library  library,
                  FT_Bitmap  *bitmap )
  {
    FT_حافظه  حافظه;


    إذا ( !library )
      أرجع FT_THROW( Invalid_Library_Handle );

    إذا ( !bitmap )
      أرجع FT_THROW( Invalid_آرگومان );

    حافظه = library->حافظه;

    FT_FREE( bitmap->میانگیر );
    *bitmap = null_bitmap;

    أرجع FT_Err_Ok;
  }


/*انتها*/
