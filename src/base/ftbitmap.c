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


#شامل <freetype/internal/ftdebug.h>

#شامل <freetype/ftbitmap.h>
#شامل <freetype/ftimage.h>
#شامل <freetype/internal/ftobjs.h>


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#تعریف_منسوخ  FT_COMPONENT
#تعریف_کرو FT_COMPONENT  bitmap


  ساکن
  ثابت FT_Bitmap  null_bitmap = { 0, 0, 0, تہی, 0, 0, 0, تہی };


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( خالی )
  FT_Bitmap_Init( FT_Bitmap  *abitmap )
  {
    اگر ( abitmap )
      *abitmap = null_bitmap;
  }


  /* deprecated function name; retained for ABI compatibility */

  FT_EXPORT_DEF( خالی )
  FT_Bitmap_New( FT_Bitmap  *abitmap )
  {
    اگر ( abitmap )
      *abitmap = null_bitmap;
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Bitmap_Copy( FT_Library        library,
                  ثابت FT_Bitmap  *ماخذ,
                  FT_Bitmap        *ہدف)
  {
    FT_حافظہ  حافظہ;
    FT_Error   error  = FT_Err_Ok;
    FT_Int     pitch;
    FT_Int     flip;


    اگر ( !library )
      واپس FT_THROW( Invalid_Library_Handle );

    اگر ( !ماخذ || !ہدف )
      واپس FT_THROW( Invalid_دلیل );

    اگر ( ماخذ == ہدف )
      واپس FT_Err_Ok;

    flip = ( ماخذ->pitch < 0 && ہدف->pitch > 0 ) ||
           ( ماخذ->pitch > 0 && ہدف->pitch < 0 );

    حافظہ = library->حافظہ;
    FT_FREE( ہدف->بفر );

    *ہدف = *ماخذ;

    اگر ( flip )
      ہدف->pitch = -ہدف->pitch;

    اگر ( !ماخذ->بفر )
      واپس FT_Err_Ok;

    pitch  = ماخذ->pitch;
    اگر ( pitch < 0 )
      pitch = -pitch;

    FT_MEM_QALLOC_MULT( ہدف->بفر, ہدف->صفیں, pitch );

    اگر ( !error )
    {
      اگر ( flip )
      {
        /* take care of bitmap flow */
        FT_UInt   i;
        FT_Byte*  s = ماخذ->بفر;
        FT_Byte*  t = ہدف->بفر;


        t += (FT_ULong)pitch * ( ہدف->صفیں - 1 );

        برائے ( i = ہدف->صفیں; i > 0; i-- )
        {
          FT_ARRAY_COPY( t, s, pitch );

          s += pitch;
          t -= pitch;
        }
      }
      ورنہ
        FT_MEM_COPY( ہدف->بفر, ماخذ->بفر,
                     (FT_Long)ماخذ->صفیں * pitch );
    }

    واپس error;
  }


  /* Enlarge `bitmap' horizontally and vertically by `xpixels' */
  /* and `ypixels', respectively.                              */

  ساکن FT_Error
  ft_bitmap_assure_بفر( FT_حافظہ   حافظہ,
                           FT_Bitmap*  bitmap,
                           FT_UInt     xpixels,
                           FT_UInt     ypixels )
  {
    FT_Error        error;
    غیرعلامتی صحیح    pitch;
    غیرعلامتی صحیح    new_pitch;
    FT_UInt         bpp;
    FT_UInt         چوڑائی, اونچائی;
    غیرعلامتی حرف*  بفر = تہی;


    چوڑائی  = bitmap->چوڑائی;
    اونچائی = bitmap->صفیں;
    pitch  = (غیرعلامتی صحیح)FT_ABS( bitmap->pitch );

    انتخاب ( bitmap->pixel_منوال )
    {
    حالت FT_PIXEL_منوال_MONO:
      bpp       = 1;
      new_pitch = ( چوڑائی + xpixels + 7 ) >> 3;
      توڑو;
    حالت FT_PIXEL_منوال_GRAY2:
      bpp       = 2;
      new_pitch = ( چوڑائی + xpixels + 3 ) >> 2;
      توڑو;
    حالت FT_PIXEL_منوال_GRAY4:
      bpp       = 4;
      new_pitch = ( چوڑائی + xpixels + 1 ) >> 1;
      توڑو;
    حالت FT_PIXEL_منوال_سرمئی:
    حالت FT_PIXEL_منوال_LCD:
    حالت FT_PIXEL_منوال_LCD_V:
      bpp       = 8;
      new_pitch = چوڑائی + xpixels;
      توڑو;
    طےشدہ:
      واپس FT_THROW( Invalid_Glyph_Format );
    }

    /* if no need to allocate memory */
    اگر ( ypixels == 0 && new_pitch <= pitch )
    {
      /* zero the padding */
      FT_UInt  ثنائی_ہندسہ_چوڑائی = pitch * 8;
      FT_UInt  ثنائی_ہندسہ_آخری  = ( چوڑائی + xpixels ) * bpp;


      اگر ( ثنائی_ہندسہ_آخری < ثنائی_ہندسہ_چوڑائی )
      {
        FT_Byte*  سطر  = bitmap->بفر + ( ثنائی_ہندسہ_آخری >> 3 );
        FT_Byte*  انتہا   = bitmap->بفر + pitch;
        FT_UInt   سرکاؤ = ثنائی_ہندسہ_آخری & 7;
        FT_UInt   نقاب  = 0xFF00U >> سرکاؤ;
        FT_UInt   گنتی = اونچائی;


        برائے ( ; گنتی > 0; گنتی--, سطر += pitch, انتہا += pitch )
        {
          FT_Byte*  اكتب = سطر;


          اگر ( سرکاؤ > 0 )
          {
            اكتب[0] = (FT_Byte)( اكتب[0] & نقاب );
            اكتب++;
          }
          اگر ( اكتب < انتہا )
            FT_MEM_ZERO( اكتب, انتہا - اكتب );
        }
      }

      واپس FT_Err_Ok;
    }

    /* otherwise allocate new buffer */
    اگر ( FT_QALLOC_MULT( بفر, bitmap->صفیں + ypixels, new_pitch ) )
      واپس error;

    /* new rows get added at the top of the bitmap, */
    /* thus take care of the flow direction         */
    اگر ( bitmap->pitch > 0 )
    {
      FT_UInt  len = ( چوڑائی * bpp + 7 ) >> 3;

      غیرعلامتی حرف*  اندر  = bitmap->بفر;
      غیرعلامتی حرف*  برآمد = بفر;

      غیرعلامتی حرف*  حد_بندی = bitmap->بفر + pitch * bitmap->صفیں;
      غیرعلامتی صحیح    delta = new_pitch - len;


      FT_MEM_ZERO( برآمد, new_pitch * ypixels );
      برآمد += new_pitch * ypixels;

      جب ( اندر < حد_بندی )
      {
        FT_MEM_COPY( برآمد, اندر, len );
        اندر  += pitch;
        برآمد += len;

        /* we use FT_QALLOC_MULT, which doesn't zero out the buffer;      */
        /* consequently, we have to manually zero out the remaining bytes */
        FT_MEM_ZERO( برآمد, delta );
        برآمد += delta;
      }
    }
    ورنہ
    {
      FT_UInt  len = ( چوڑائی * bpp + 7 ) >> 3;

      غیرعلامتی حرف*  اندر  = bitmap->بفر;
      غیرعلامتی حرف*  برآمد = بفر;

      غیرعلامتی حرف*  حد_بندی = bitmap->بفر + pitch * bitmap->صفیں;
      غیرعلامتی صحیح    delta = new_pitch - len;


      جب ( اندر < حد_بندی )
      {
        FT_MEM_COPY( برآمد, اندر, len );
        اندر  += pitch;
        برآمد += len;

        FT_MEM_ZERO( برآمد, delta );
        برآمد += delta;
      }

      FT_MEM_ZERO( برآمد, new_pitch * ypixels );
    }

    FT_FREE( bitmap->بفر );
    bitmap->بفر = بفر;

    /* set pitch only, width and height are left untouched */
    اگر ( bitmap->pitch < 0 )
      bitmap->pitch = -(صحیح)new_pitch;
    ورنہ
      bitmap->pitch = (صحیح)new_pitch;

    واپس FT_Err_Ok;
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Bitmap_Embolden( FT_Library  library,
                      FT_Bitmap*  bitmap,
                      FT_Pos      xStrength,
                      FT_Pos      yStrength )
  {
    FT_Error        error;
    غیرعلامتی حرف*  p;
    FT_Int          i, x, pitch;
    FT_UInt         y;
    FT_Int          xstr, ystr;


    اگر ( !library )
      واپس FT_THROW( Invalid_Library_Handle );

    اگر ( !bitmap || !bitmap->بفر )
      واپس FT_THROW( Invalid_دلیل );

    اگر ( ( ( FT_PIX_باری( xStrength ) >> 6 ) > FT_INT_MAX ) ||
         ( ( FT_PIX_باری( yStrength ) >> 6 ) > FT_INT_MAX ) )
      واپس FT_THROW( Invalid_دلیل );

    xstr = (FT_Int)FT_PIX_باری( xStrength ) >> 6;
    ystr = (FT_Int)FT_PIX_باری( yStrength ) >> 6;

    اگر ( xstr == 0 && ystr == 0 )
      واپس FT_Err_Ok;
    ورنہ اگر ( xstr < 0 || ystr < 0 )
      واپس FT_THROW( Invalid_دلیل );

    انتخاب ( bitmap->pixel_منوال )
    {
    حالت FT_PIXEL_منوال_GRAY2:
    حالت FT_PIXEL_منوال_GRAY4:
      {
        FT_Bitmap  tmp;


        /* convert to 8bpp */
        FT_Bitmap_Init( &tmp );
        error = FT_Bitmap_Convert( library, bitmap, &tmp, 1 );
        اگر ( error )
          واپس error;

        FT_Bitmap_مکمل( library, bitmap );
        *bitmap = tmp;
      }
      توڑو;

    حالت FT_PIXEL_منوال_MONO:
      اگر ( xstr > 8 )
        xstr = 8;
      توڑو;

    حالت FT_PIXEL_منوال_LCD:
      xstr *= 3;
      توڑو;

    حالت FT_PIXEL_منوال_LCD_V:
      ystr *= 3;
      توڑو;

    حالت FT_PIXEL_منوال_BGRA:
      /* We don't embolden color glyphs. */
      واپس FT_Err_Ok;
    }

    error = ft_bitmap_assure_بفر( library->حافظہ, bitmap,
                                     (FT_UInt)xstr, (FT_UInt)ystr );
    اگر ( error )
      واپس error;

    /* take care of bitmap flow */
    pitch = bitmap->pitch;
    اگر ( pitch > 0 )
      p = bitmap->بفر + pitch * ystr;
    ورنہ
    {
      pitch = -pitch;
      p = bitmap->بفر + (FT_UInt)pitch * ( bitmap->صفیں - 1 );
    }

    /* for each row */
    برائے ( y = 0; y < bitmap->صفیں; y++ )
    {
      /*
       * Horizontally:
       *
       * From the last pixel on, make each pixel or'ed with the
       * `xstr' pixels before it.
       */
      برائے ( x = pitch - 1; x >= 0; x-- )
      {
        غیرعلامتی حرف  tmp;


        tmp = p[x];
        برائے ( i = 1; i <= xstr; i++ )
        {
          اگر ( bitmap->pixel_منوال == FT_PIXEL_منوال_MONO )
          {
            p[x] |= tmp >> i;

            /* the maximum value of 8 for `xstr' comes from here */
            اگر ( x > 0 )
              p[x] |= p[x - 1] << ( 8 - i );

#اگر 0
            اگر ( p[x] == 0xFF )
              توڑو;
#اگر_ختم
          }
          ورنہ
          {
            اگر ( x - i >= 0 )
            {
              اگر ( p[x] + p[x - i] > bitmap->عدد_grays - 1 )
              {
                p[x] = (غیرعلامتی حرف)( bitmap->عدد_grays - 1 );
                توڑو;
              }
              ورنہ
              {
                p[x] = (غیرعلامتی حرف)( p[x] + p[x - i] );
                اگر ( p[x] == bitmap->عدد_grays - 1 )
                  توڑو;
              }
            }
            ورنہ
              توڑو;
          }
        }
      }

      /*
       * Vertically:
       *
       * Make the above `ystr' rows or'ed with it.
       */
      برائے ( x = 1; x <= ystr; x++ )
      {
        غیرعلامتی حرف*  q;


        q = p - bitmap->pitch * x;
        برائے ( i = 0; i < pitch; i++ )
          q[i] |= p[i];
      }

      p += bitmap->pitch;
    }

    bitmap->چوڑائی += (FT_UInt)xstr;
    bitmap->صفیں += (FT_UInt)ystr;

    واپس FT_Err_Ok;
  }


  ساکن FT_Byte
  ft_سرمئی_for_premultiplied_srgb_bgra( ثابت FT_Byte*  bgra )
  {
    FT_UInt  a = bgra[3];
    FT_UInt  l;


    /* Short-circuit transparent color to avoid division by zero. */
    اگر ( !a )
      واپس 0;

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

    واپس (FT_Byte)( a - l / a );
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Bitmap_Convert( FT_Library        library,
                     ثابت FT_Bitmap  *ماخذ,
                     FT_Bitmap        *ہدف,
                     FT_Int            alignment )
  {
    FT_Error   error = FT_Err_Ok;
    FT_حافظہ  حافظہ;

    FT_Byte*  s;
    FT_Byte*  t;


    اگر ( !library )
      واپس FT_THROW( Invalid_Library_Handle );

    اگر ( !ماخذ || !ہدف )
      واپس FT_THROW( Invalid_دلیل );

    حافظہ = library->حافظہ;

    انتخاب ( ماخذ->pixel_منوال )
    {
    حالت FT_PIXEL_منوال_MONO:
    حالت FT_PIXEL_منوال_سرمئی:
    حالت FT_PIXEL_منوال_GRAY2:
    حالت FT_PIXEL_منوال_GRAY4:
    حالت FT_PIXEL_منوال_LCD:
    حالت FT_PIXEL_منوال_LCD_V:
    حالت FT_PIXEL_منوال_BGRA:
      {
        FT_Int  چوڑائی = (FT_Int)ماخذ->چوڑائی;
        FT_Int  neg   = ( ہدف->pitch == 0 && ماخذ->pitch < 0 ) ||
                          ہدف->pitch  < 0;


        FT_Bitmap_مکمل( library, ہدف );

        ہدف->pixel_منوال = FT_PIXEL_منوال_سرمئی;
        ہدف->صفیں       = ماخذ->صفیں;
        ہدف->چوڑائی      = ماخذ->چوڑائی;

        اگر ( alignment )
        {
          FT_Int  rem = چوڑائی % alignment;


          اگر ( rem )
            چوڑائی = alignment > 0 ? چوڑائی - rem + alignment
                                  : چوڑائی - rem - alignment;
        }

        اگر ( FT_QALLOC_MULT( ہدف->بفر, ہدف->صفیں, چوڑائی ) )
          واپس error;

        ہدف->pitch = neg ? -چوڑائی : چوڑائی;
      }
      توڑو;

    طےشدہ:
      error = FT_THROW( Invalid_دلیل );
    }

    s = ماخذ->بفر;
    t = ہدف->بفر;

    /* take care of bitmap flow */
    اگر ( ماخذ->pitch < 0 )
      s -= ماخذ->pitch * (FT_Int)( ماخذ->صفیں - 1 );
    اگر ( ہدف->pitch < 0 )
      t -= ہدف->pitch * (FT_Int)( ہدف->صفیں - 1 );

    انتخاب ( ماخذ->pixel_منوال )
    {
    حالت FT_PIXEL_منوال_MONO:
      {
        FT_UInt  i;


        ہدف->عدد_grays = 2;

        برائے ( i = ماخذ->صفیں; i > 0; i-- )
        {
          FT_Byte*  ss = s;
          FT_Byte*  tt = t;
          FT_UInt   j;


          /* get the full bytes */
          برائے ( j = ماخذ->چوڑائی >> 3; j > 0; j-- )
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
          j = ماخذ->چوڑائی & 7;
          اگر ( j > 0 )
          {
            FT_Int  val = *ss;


            برائے ( ; j > 0; j-- )
            {
              tt[0] = (FT_Byte)( ( val & 0x80 ) >> 7);
              val <<= 1;
              tt   += 1;
            }
          }

          s += ماخذ->pitch;
          t += ہدف->pitch;
        }
      }
      توڑو;


    حالت FT_PIXEL_منوال_سرمئی:
    حالت FT_PIXEL_منوال_LCD:
    حالت FT_PIXEL_منوال_LCD_V:
      {
        FT_UInt  چوڑائی = ماخذ->چوڑائی;
        FT_UInt  i;


        ہدف->عدد_grays = 256;

        برائے ( i = ماخذ->صفیں; i > 0; i-- )
        {
          FT_ARRAY_COPY( t, s, چوڑائی );

          s += ماخذ->pitch;
          t += ہدف->pitch;
        }
      }
      توڑو;


    حالت FT_PIXEL_منوال_GRAY2:
      {
        FT_UInt  i;


        ہدف->عدد_grays = 4;

        برائے ( i = ماخذ->صفیں; i > 0; i-- )
        {
          FT_Byte*  ss = s;
          FT_Byte*  tt = t;
          FT_UInt   j;


          /* get the full bytes */
          برائے ( j = ماخذ->چوڑائی >> 2; j > 0; j-- )
          {
            FT_Int  val = ss[0];


            tt[0] = (FT_Byte)( ( val & 0xC0 ) >> 6 );
            tt[1] = (FT_Byte)( ( val & 0x30 ) >> 4 );
            tt[2] = (FT_Byte)( ( val & 0x0C ) >> 2 );
            tt[3] = (FT_Byte)( ( val & 0x03 ) );

            ss += 1;
            tt += 4;
          }

          j = ماخذ->چوڑائی & 3;
          اگر ( j > 0 )
          {
            FT_Int  val = ss[0];


            برائے ( ; j > 0; j-- )
            {
              tt[0]  = (FT_Byte)( ( val & 0xC0 ) >> 6 );
              val  <<= 2;
              tt    += 1;
            }
          }

          s += ماخذ->pitch;
          t += ہدف->pitch;
        }
      }
      توڑو;


    حالت FT_PIXEL_منوال_GRAY4:
      {
        FT_UInt  i;


        ہدف->عدد_grays = 16;

        برائے ( i = ماخذ->صفیں; i > 0; i-- )
        {
          FT_Byte*  ss = s;
          FT_Byte*  tt = t;
          FT_UInt   j;


          /* get the full bytes */
          برائے ( j = ماخذ->چوڑائی >> 1; j > 0; j-- )
          {
            FT_Int  val = ss[0];


            tt[0] = (FT_Byte)( ( val & 0xF0 ) >> 4 );
            tt[1] = (FT_Byte)( ( val & 0x0F ) );

            ss += 1;
            tt += 2;
          }

          اگر ( ماخذ->چوڑائی & 1 )
            tt[0] = (FT_Byte)( ( ss[0] & 0xF0 ) >> 4 );

          s += ماخذ->pitch;
          t += ہدف->pitch;
        }
      }
      توڑو;


    حالت FT_PIXEL_منوال_BGRA:
      {
        FT_UInt  i;


        ہدف->عدد_grays = 256;

        برائے ( i = ماخذ->صفیں; i > 0; i-- )
        {
          FT_Byte*  ss = s;
          FT_Byte*  tt = t;
          FT_UInt   j;


          برائے ( j = ماخذ->چوڑائی; j > 0; j-- )
          {
            tt[0] = ft_سرمئی_for_premultiplied_srgb_bgra( ss );

            ss += 4;
            tt += 1;
          }

          s += ماخذ->pitch;
          t += ہدف->pitch;
        }
      }
      توڑو;

    طےشدہ:
      ;
    }

    واپس error;
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Bitmap_Blend( FT_Library        library,
                   ثابت FT_Bitmap*  ماخذ_2,
                   ثابت FT_سمتیہ   ماخذ_انحراف,
                   FT_Bitmap*        ہدف,
                   FT_سمتیہ        *atarget_انحراف,
                   FT_Color          color )
  {
    FT_Error   error = FT_Err_Ok;
    FT_حافظہ  حافظہ;

    FT_Bitmap         ماخذ_bitmap;
    ثابت FT_Bitmap*  ماخذ;

    FT_Bool  free_ماخذ_bitmap          = 0;
    FT_Bool  free_ہدف_bitmap_on_error = 0;

    FT_BBox  sbx, tbx, fbx;

    غیرعلامتی صحیح  final_صفیں, final_چوڑائی;


    اگر ( !library || !ہدف || !ماخذ_2 || !atarget_انحراف )
      واپس FT_THROW( Invalid_دلیل );

    حافظہ = library->حافظہ;

    اگر ( !( ہدف->pixel_منوال == FT_PIXEL_منوال_NONE     ||
            ( ہدف->pixel_منوال == FT_PIXEL_منوال_BGRA &&
              ہدف->بفر                           ) ) )
      واپس FT_THROW( Invalid_دلیل );

    اگر ( ماخذ_2->pixel_منوال == FT_PIXEL_منوال_NONE )
      واپس FT_Err_Ok;               /* nothing to do */

    /* pitches must have the same sign */
    اگر ( ہدف->pixel_منوال == FT_PIXEL_منوال_BGRA &&
         ( ماخذ_2->pitch ^ ہدف->pitch ) < 0   )
      واپس FT_THROW( Invalid_دلیل );

    اگر ( !( ماخذ_2->چوڑائی && ماخذ_2->صفیں ) )
      واپس FT_Err_Ok;               /* nothing to do */

    /* get source bitmap dimensions assuming integer offsets */
    sbx.xMin = ( ماخذ_انحراف.x >> 6 );
    sbx.yMin = ( ماخذ_انحراف.y >> 6 ) - ماخذ_2->صفیں;
    sbx.xMax = ( ماخذ_انحراف.x >> 6 ) + ماخذ_2->چوڑائی;
    sbx.yMax = ( ماخذ_انحراف.y >> 6 );

    FT_TRACE5(( "FT_Bitmap_Blend:\n" ));
    FT_TRACE5(( "  source bitmap: (%ld, %ld) -- (%ld, %ld); %u x %u\n",
      sbx.xMin, sbx.yMin, sbx.xMax, sbx.yMax,
      ماخذ_2->چوڑائی, ماخذ_2->صفیں ));

    /* sanity check */
    اگر ( sbx.xMin > sbx.xMax      || sbx.yMin > sbx.yMax     ||
         ماخذ_2->چوڑائی > 0x7FFFU || ماخذ_2->صفیں > 0x7FFFU )
    {
      FT_TRACE5(( "FT_Bitmap_Blend: source dimension overflow\n" ));
      واپس FT_THROW( Invalid_دلیل );
    }

    اگر ( !ہدف->چوڑائی || !ہدف->صفیں )
    {
      FT_TRACE5(( "  target bitmap: empty\n" ));

      final_چوڑائی = ماخذ_2->چوڑائی;
      final_صفیں  = ماخذ_2->صفیں;

      /* create new empty bitmap */
      ہدف->چوڑائی      = final_چوڑائی;
      ہدف->صفیں       = final_صفیں;
      ہدف->pixel_منوال = FT_PIXEL_منوال_BGRA;
      ہدف->pitch      = (صحیح)final_چوڑائی * 4;
      ہدف->عدد_grays  = 256;

      اگر ( FT_ALLOC_MULT( ہدف->بفر, ہدف->صفیں, ہدف->pitch ) )
        واپس error;

      free_ہدف_bitmap_on_error = 1;

      fbx = sbx;
    }
    ورنہ
    {
      /* get target bitmap dimensions assuming integer offsets */
      tbx.xMin = ( atarget_انحراف->x >> 6 );
      tbx.yMin = ( atarget_انحراف->y >> 6 ) - ہدف->صفیں;
      tbx.xMax = ( atarget_انحراف->x >> 6 ) + ہدف->چوڑائی;
      tbx.yMax = ( atarget_انحراف->y >> 6 );

      FT_TRACE5(( "  target bitmap: (%ld, %ld) -- (%ld, %ld); %u x %u\n",
        tbx.xMin, tbx.yMin, tbx.xMax, tbx.yMax,
        ہدف->چوڑائی, ہدف->صفیں ));

      /* sanity check */
      اگر ( tbx.xMin > tbx.xMax || tbx.yMin > tbx.yMax )
      {
        FT_TRACE5(( "FT_Bitmap_Blend: targget dimension overflow\n" ));
        واپس FT_THROW( Invalid_دلیل );
      }

      /* compute final bitmap dimensions */
      fbx.xMin = FT_MIN( sbx.xMin, tbx.xMin );
      fbx.yMin = FT_MIN( sbx.yMin, tbx.yMin );
      fbx.xMax = FT_MAX( sbx.xMax, tbx.xMax );
      fbx.yMax = FT_MAX( sbx.yMax, tbx.yMax );

      /* sanity check */
      اگر ( fbx.xMin < -0x10000 || fbx.xMax >= 0x10000 ||
           fbx.yMin < -0x10000 || fbx.yMax >= 0x10000 )
      {
        FT_TRACE5(( "FT_Bitmap_Blend: final dimension overflow\n" ));
        واپس FT_THROW( Invalid_دلیل );
      }

      final_چوڑائی = fbx.xMax - fbx.xMin;
      final_صفیں  = fbx.yMax - fbx.yMin;

      /* adjust target bitmap to enlarged size */
      اگر ( ہدف->چوڑائی < final_چوڑائی ||
           ہدف->صفیں  < final_صفیں  )
      {
        صحیح  pitch, new_pitch;

        غیرعلامتی حرف*  بفر = تہی;


        pitch = ہدف->pitch;

        اگر ( pitch < 0 )
          pitch = -pitch;

        new_pitch = (صحیح)final_چوڑائی * 4;

        /* TODO: provide an in-buffer solution for large bitmaps */
        /*       to avoid allocation of a new buffer             */
        اگر ( FT_ALLOC_MULT( بفر, final_صفیں, new_pitch ) )
          جاؤ Error;

        /* copy data to new buffer */

        /* the bitmap flow is from top to bottom, */
        /* but y is measured from bottom to top   */
        اگر ( ہدف->pitch < 0 )
        {
          /* XXX */
        }
        ورنہ
        {
          FT_Pos  x = tbx.xMin - fbx.xMin;
          FT_Pos  y = tbx.yMin - fbx.yMin;

          غیرعلامتی حرف*  p =
            ہدف->بفر;
          غیرعلامتی حرف*  q =
            بفر +
            ( final_صفیں - y - ہدف->صفیں ) * new_pitch +
            x * 4;
          غیرعلامتی حرف*  حد_بندی_p =
            p + pitch * (صحیح)ہدف->صفیں;


          جب ( p < حد_بندی_p )
          {
            FT_MEM_COPY( q, p, pitch );

            p += pitch;
            q += new_pitch;
          }
        }

        FT_FREE( ہدف->بفر );

        ہدف->چوڑائی = final_چوڑائی;
        ہدف->صفیں  = final_صفیں;

        اگر ( ہدف->pitch < 0 )
          ہدف->pitch = -new_pitch;
        ورنہ
          ہدف->pitch = new_pitch;

        ہدف->بفر = بفر;
      }
    }

    FT_TRACE5(( "  final bitmap: (%ld, %ld) -- (%ld, %ld); %u x %u\n",
      fbx.xMin, fbx.yMin, fbx.xMax, fbx.yMax,
      final_چوڑائی, final_صفیں ));

    /* adjust source bitmap if necessary */
    اگر ( ماخذ_2->pixel_منوال != FT_PIXEL_منوال_سرمئی )
    {
      FT_Bitmap_Init( &ماخذ_bitmap );
      error = FT_Bitmap_Convert( library, ماخذ_2, &ماخذ_bitmap, 1 );
      اگر ( error )
        جاؤ Error;

      ماخذ             = &ماخذ_bitmap;
      free_ماخذ_bitmap = 1;
    }
    ورنہ
      ماخذ = ماخذ_2;

    /* do blending; the code below returns pre-multiplied channels, */
    /* similar to what FreeType gets from `CBDT' tables             */

    /* the bitmap flow is from top to bottom, */
    /* but y is measured from bottom to top   */
    اگر ( ہدف->pitch < 0 )
    {
      /* XXX */
    }
    ورنہ
    {
      FT_Pos  x = sbx.xMin - fbx.xMin;
      FT_Pos  y = sbx.yMin - fbx.yMin;

      غیرعلامتی حرف*  p =
        ماخذ->بفر;
      غیرعلامتی حرف*  q =
        ہدف->بفر +
        ( ہدف->صفیں - y - ماخذ->صفیں ) * ہدف->pitch +
        x * 4;
      غیرعلامتی حرف*  حد_بندی_p =
        p + ماخذ->pitch * (صحیح)ماخذ->صفیں;


      جب ( p < حد_بندی_p )
      {
        غیرعلامتی حرف*  r       = p;
        غیرعلامتی حرف*  s       = q;
        غیرعلامتی حرف*  حد_بندی_r = r + ماخذ->چوڑائی;


        جب ( r < حد_بندی_r )
        {
          صحیح  aa = *r++;
          صحیح  fa = color.alpha * aa / 255;

          صحیح  fb = color.blue * fa / 255;
          صحیح  fg = color.green * fa / 255;
          صحیح  fr = color.red * fa / 255;

          صحیح  ba2 = 255 - fa;

          صحیح  bb = s[0];
          صحیح  bg = s[1];
          صحیح  br = s[2];
          صحیح  ba = s[3];


          *s++ = (غیرعلامتی حرف)( bb * ba2 / 255 + fb );
          *s++ = (غیرعلامتی حرف)( bg * ba2 / 255 + fg );
          *s++ = (غیرعلامتی حرف)( br * ba2 / 255 + fr );
          *s++ = (غیرعلامتی حرف)( ba * ba2 / 255 + fa );
        }

        p += ماخذ->pitch;
        q += ہدف->pitch;
      }
    }

    atarget_انحراف->x = fbx.xMin * 64;
    atarget_انحراف->y = fbx.yMax * 64;

  Error:
    اگر ( error && free_ہدف_bitmap_on_error )
      FT_Bitmap_مکمل( library, ہدف );

    اگر ( free_ماخذ_bitmap )
      FT_Bitmap_مکمل( library, &ماخذ_bitmap );

    واپس error;
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Glyph_خانہ_Own_Bitmap( FT_Glyph_خانہ  خانہ )
  {
    اگر ( خانہ && خانہ->format == FT_GLYPH_FORMAT_BITMAP   &&
         !( خانہ->اندرونی_2->flags & FT_GLYPH_OWN_BITMAP ) )
    {
      FT_Bitmap  bitmap;
      FT_Error   error;


      FT_Bitmap_Init( &bitmap );
      error = FT_Bitmap_Copy( خانہ->library, &خانہ->bitmap, &bitmap );
      اگر ( error )
        واپس error;

      خانہ->bitmap = bitmap;
      خانہ->اندرونی_2->flags |= FT_GLYPH_OWN_BITMAP;
    }

    واپس FT_Err_Ok;
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Bitmap_مکمل( FT_Library  library,
                  FT_Bitmap  *bitmap )
  {
    FT_حافظہ  حافظہ;


    اگر ( !library )
      واپس FT_THROW( Invalid_Library_Handle );

    اگر ( !bitmap )
      واپس FT_THROW( Invalid_دلیل );

    حافظہ = library->حافظہ;

    FT_FREE( bitmap->بفر );
    *bitmap = null_bitmap;

    واپس FT_Err_Ok;
  }


/*انتہا*/
