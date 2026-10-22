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
#تعریف_رد  FT_COMPONENT
#تعریف_کرو FT_COMPONENT  bitmap


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
                  ثابت FT_Bitmap  *سروت,
                  FT_Bitmap        *نشانہ)
  {
    FT_حافظہ  حافظہ;
    FT_Error   error  = FT_Err_Ok;
    FT_Int     pitch;
    FT_Int     flip;


    إذا ( !library )
      أرجع FT_THROW( Invalid_Library_Handle );

    إذا ( !سروت || !نشانہ )
      أرجع FT_THROW( Invalid_دلیل );

    إذا ( سروت == نشانہ )
      أرجع FT_Err_Ok;

    flip = ( سروت->pitch < 0 && نشانہ->pitch > 0 ) ||
           ( سروت->pitch > 0 && نشانہ->pitch < 0 );

    حافظہ = library->حافظہ;
    FT_FREE( نشانہ->بفر );

    *نشانہ = *سروت;

    إذا ( flip )
      نشانہ->pitch = -نشانہ->pitch;

    إذا ( !سروت->بفر )
      أرجع FT_Err_Ok;

    pitch  = سروت->pitch;
    إذا ( pitch < 0 )
      pitch = -pitch;

    FT_MEM_QALLOC_MULT( نشانہ->بفر, نشانہ->صفاں, pitch );

    إذا ( !error )
    {
      إذا ( flip )
      {
        /* take care of bitmap flow */
        FT_UInt   i;
        FT_Byte*  s = سروت->بفر;
        FT_Byte*  t = نشانہ->بفر;


        t += (FT_ULong)pitch * ( نشانہ->صفاں - 1 );

        لكل ( i = نشانہ->صفاں; i > 0; i-- )
        {
          FT_ARRAY_COPY( t, s, pitch );

          s += pitch;
          t -= pitch;
        }
      }
      وإلا
        FT_MEM_COPY( نشانہ->بفر, سروت->بفر,
                     (FT_Long)سروت->صفاں * pitch );
    }

    أرجع error;
  }


  /* Enlarge `bitmap' horizontally and vertically by `xpixels' */
  /* and `ypixels', respectively.                              */

  ساكن FT_Error
  ft_bitmap_assure_بفر( FT_حافظہ   حافظہ,
                           FT_Bitmap*  bitmap,
                           FT_UInt     xpixels,
                           FT_UInt     ypixels )
  {
    FT_Error        error;
    بنانشانی صحيح    pitch;
    بنانشانی صحيح    new_pitch;
    FT_UInt         bpp;
    FT_UInt         چوڑائی, اچائی;
    بنانشانی حرف*  بفر = عدم;


    چوڑائی  = bitmap->چوڑائی;
    اچائی = bitmap->صفاں;
    pitch  = (بنانشانی صحيح)FT_ABS( bitmap->pitch );

    بدل ( bitmap->pixel_منوال )
    {
    حالة FT_PIXEL_منوال_MONO:
      bpp       = 1;
      new_pitch = ( چوڑائی + xpixels + 7 ) >> 3;
      اكسر;
    حالة FT_PIXEL_منوال_GRAY2:
      bpp       = 2;
      new_pitch = ( چوڑائی + xpixels + 3 ) >> 2;
      اكسر;
    حالة FT_PIXEL_منوال_GRAY4:
      bpp       = 4;
      new_pitch = ( چوڑائی + xpixels + 1 ) >> 1;
      اكسر;
    حالة FT_PIXEL_منوال_سلیٹی:
    حالة FT_PIXEL_منوال_LCD:
    حالة FT_PIXEL_منوال_LCD_V:
      bpp       = 8;
      new_pitch = چوڑائی + xpixels;
      اكسر;
    افتراضي:
      أرجع FT_THROW( Invalid_Glyph_Format );
    }

    /* if no need to allocate memory */
    إذا ( ypixels == 0 && new_pitch <= pitch )
    {
      /* zero the padding */
      FT_UInt  ثنائی_ہندسہ_چوڑائی = pitch * 8;
      FT_UInt  ثنائی_ہندسہ_آخری  = ( چوڑائی + xpixels ) * bpp;


      إذا ( ثنائی_ہندسہ_آخری < ثنائی_ہندسہ_چوڑائی )
      {
        FT_Byte*  سطر  = bitmap->بفر + ( ثنائی_ہندسہ_آخری >> 3 );
        FT_Byte*  سرا   = bitmap->بفر + pitch;
        FT_UInt   کھسکاؤ = ثنائی_ہندسہ_آخری & 7;
        FT_UInt   نقاب  = 0xFF00U >> کھسکاؤ;
        FT_UInt   گنتی = اچائی;


        لكل ( ; گنتی > 0; گنتی--, سطر += pitch, سرا += pitch )
        {
          FT_Byte*  اكتب = سطر;


          إذا ( کھسکاؤ > 0 )
          {
            اكتب[0] = (FT_Byte)( اكتب[0] & نقاب );
            اكتب++;
          }
          إذا ( اكتب < سرا )
            FT_MEM_ZERO( اكتب, سرا - اكتب );
        }
      }

      أرجع FT_Err_Ok;
    }

    /* otherwise allocate new buffer */
    إذا ( FT_QALLOC_MULT( بفر, bitmap->صفاں + ypixels, new_pitch ) )
      أرجع error;

    /* new rows get added at the top of the bitmap, */
    /* thus take care of the flow direction         */
    إذا ( bitmap->pitch > 0 )
    {
      FT_UInt  len = ( چوڑائی * bpp + 7 ) >> 3;

      بنانشانی حرف*  وچ  = bitmap->بفر;
      بنانشانی حرف*  باہرلا = بفر;

      بنانشانی حرف*  حد_بندی = bitmap->بفر + pitch * bitmap->صفاں;
      بنانشانی صحيح    delta = new_pitch - len;


      FT_MEM_ZERO( باہرلا, new_pitch * ypixels );
      باہرلا += new_pitch * ypixels;

      طالما ( وچ < حد_بندی )
      {
        FT_MEM_COPY( باہرلا, وچ, len );
        وچ  += pitch;
        باہرلا += len;

        /* we use FT_QALLOC_MULT, which doesn't zero out the buffer;      */
        /* consequently, we have to manually zero out the remaining bytes */
        FT_MEM_ZERO( باہرلا, delta );
        باہرلا += delta;
      }
    }
    وإلا
    {
      FT_UInt  len = ( چوڑائی * bpp + 7 ) >> 3;

      بنانشانی حرف*  وچ  = bitmap->بفر;
      بنانشانی حرف*  باہرلا = بفر;

      بنانشانی حرف*  حد_بندی = bitmap->بفر + pitch * bitmap->صفاں;
      بنانشانی صحيح    delta = new_pitch - len;


      طالما ( وچ < حد_بندی )
      {
        FT_MEM_COPY( باہرلا, وچ, len );
        وچ  += pitch;
        باہرلا += len;

        FT_MEM_ZERO( باہرلا, delta );
        باہرلا += delta;
      }

      FT_MEM_ZERO( باہرلا, new_pitch * ypixels );
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
    بنانشانی حرف*  p;
    FT_Int          i, x, pitch;
    FT_UInt         y;
    FT_Int          xstr, ystr;


    إذا ( !library )
      أرجع FT_THROW( Invalid_Library_Handle );

    إذا ( !bitmap || !bitmap->بفر )
      أرجع FT_THROW( Invalid_دلیل );

    إذا ( ( ( FT_PIX_گیڑا( xStrength ) >> 6 ) > FT_INT_MAX ) ||
         ( ( FT_PIX_گیڑا( yStrength ) >> 6 ) > FT_INT_MAX ) )
      أرجع FT_THROW( Invalid_دلیل );

    xstr = (FT_Int)FT_PIX_گیڑا( xStrength ) >> 6;
    ystr = (FT_Int)FT_PIX_گیڑا( yStrength ) >> 6;

    إذا ( xstr == 0 && ystr == 0 )
      أرجع FT_Err_Ok;
    وإلا إذا ( xstr < 0 || ystr < 0 )
      أرجع FT_THROW( Invalid_دلیل );

    بدل ( bitmap->pixel_منوال )
    {
    حالة FT_PIXEL_منوال_GRAY2:
    حالة FT_PIXEL_منوال_GRAY4:
      {
        FT_Bitmap  tmp;


        /* convert to 8bpp */
        FT_Bitmap_Init( &tmp );
        error = FT_Bitmap_Convert( library, bitmap, &tmp, 1 );
        إذا ( error )
          أرجع error;

        FT_Bitmap_پورا( library, bitmap );
        *bitmap = tmp;
      }
      اكسر;

    حالة FT_PIXEL_منوال_MONO:
      إذا ( xstr > 8 )
        xstr = 8;
      اكسر;

    حالة FT_PIXEL_منوال_LCD:
      xstr *= 3;
      اكسر;

    حالة FT_PIXEL_منوال_LCD_V:
      ystr *= 3;
      اكسر;

    حالة FT_PIXEL_منوال_BGRA:
      /* We don't embolden color glyphs. */
      أرجع FT_Err_Ok;
    }

    error = ft_bitmap_assure_بفر( library->حافظہ, bitmap,
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
      p = bitmap->بفر + (FT_UInt)pitch * ( bitmap->صفاں - 1 );
    }

    /* for each row */
    لكل ( y = 0; y < bitmap->صفاں; y++ )
    {
      /*
       * Horizontally:
       *
       * From the last pixel on, make each pixel or'ed with the
       * `xstr' pixels before it.
       */
      لكل ( x = pitch - 1; x >= 0; x-- )
      {
        بنانشانی حرف  tmp;


        tmp = p[x];
        لكل ( i = 1; i <= xstr; i++ )
        {
          إذا ( bitmap->pixel_منوال == FT_PIXEL_منوال_MONO )
          {
            p[x] |= tmp >> i;

            /* the maximum value of 8 for `xstr' comes from here */
            إذا ( x > 0 )
              p[x] |= p[x - 1] << ( 8 - i );

#إذا 0
            إذا ( p[x] == 0xFF )
              اكسر;
#جے_انت
          }
          وإلا
          {
            إذا ( x - i >= 0 )
            {
              إذا ( p[x] + p[x - i] > bitmap->عدد_grays - 1 )
              {
                p[x] = (بنانشانی حرف)( bitmap->عدد_grays - 1 );
                اكسر;
              }
              وإلا
              {
                p[x] = (بنانشانی حرف)( p[x] + p[x - i] );
                إذا ( p[x] == bitmap->عدد_grays - 1 )
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
        بنانشانی حرف*  q;


        q = p - bitmap->pitch * x;
        لكل ( i = 0; i < pitch; i++ )
          q[i] |= p[i];
      }

      p += bitmap->pitch;
    }

    bitmap->چوڑائی += (FT_UInt)xstr;
    bitmap->صفاں += (FT_UInt)ystr;

    أرجع FT_Err_Ok;
  }


  ساكن FT_Byte
  ft_سلیٹی_for_premultiplied_srgb_bgra( ثابت FT_Byte*  bgra )
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
                     ثابت FT_Bitmap  *سروت,
                     FT_Bitmap        *نشانہ,
                     FT_Int            alignment )
  {
    FT_Error   error = FT_Err_Ok;
    FT_حافظہ  حافظہ;

    FT_Byte*  s;
    FT_Byte*  t;


    إذا ( !library )
      أرجع FT_THROW( Invalid_Library_Handle );

    إذا ( !سروت || !نشانہ )
      أرجع FT_THROW( Invalid_دلیل );

    حافظہ = library->حافظہ;

    بدل ( سروت->pixel_منوال )
    {
    حالة FT_PIXEL_منوال_MONO:
    حالة FT_PIXEL_منوال_سلیٹی:
    حالة FT_PIXEL_منوال_GRAY2:
    حالة FT_PIXEL_منوال_GRAY4:
    حالة FT_PIXEL_منوال_LCD:
    حالة FT_PIXEL_منوال_LCD_V:
    حالة FT_PIXEL_منوال_BGRA:
      {
        FT_Int  چوڑائی = (FT_Int)سروت->چوڑائی;
        FT_Int  neg   = ( نشانہ->pitch == 0 && سروت->pitch < 0 ) ||
                          نشانہ->pitch  < 0;


        FT_Bitmap_پورا( library, نشانہ );

        نشانہ->pixel_منوال = FT_PIXEL_منوال_سلیٹی;
        نشانہ->صفاں       = سروت->صفاں;
        نشانہ->چوڑائی      = سروت->چوڑائی;

        إذا ( alignment )
        {
          FT_Int  rem = چوڑائی % alignment;


          إذا ( rem )
            چوڑائی = alignment > 0 ? چوڑائی - rem + alignment
                                  : چوڑائی - rem - alignment;
        }

        إذا ( FT_QALLOC_MULT( نشانہ->بفر, نشانہ->صفاں, چوڑائی ) )
          أرجع error;

        نشانہ->pitch = neg ? -چوڑائی : چوڑائی;
      }
      اكسر;

    افتراضي:
      error = FT_THROW( Invalid_دلیل );
    }

    s = سروت->بفر;
    t = نشانہ->بفر;

    /* take care of bitmap flow */
    إذا ( سروت->pitch < 0 )
      s -= سروت->pitch * (FT_Int)( سروت->صفاں - 1 );
    إذا ( نشانہ->pitch < 0 )
      t -= نشانہ->pitch * (FT_Int)( نشانہ->صفاں - 1 );

    بدل ( سروت->pixel_منوال )
    {
    حالة FT_PIXEL_منوال_MONO:
      {
        FT_UInt  i;


        نشانہ->عدد_grays = 2;

        لكل ( i = سروت->صفاں; i > 0; i-- )
        {
          FT_Byte*  ss = s;
          FT_Byte*  tt = t;
          FT_UInt   j;


          /* get the full bytes */
          لكل ( j = سروت->چوڑائی >> 3; j > 0; j-- )
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
          j = سروت->چوڑائی & 7;
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

          s += سروت->pitch;
          t += نشانہ->pitch;
        }
      }
      اكسر;


    حالة FT_PIXEL_منوال_سلیٹی:
    حالة FT_PIXEL_منوال_LCD:
    حالة FT_PIXEL_منوال_LCD_V:
      {
        FT_UInt  چوڑائی = سروت->چوڑائی;
        FT_UInt  i;


        نشانہ->عدد_grays = 256;

        لكل ( i = سروت->صفاں; i > 0; i-- )
        {
          FT_ARRAY_COPY( t, s, چوڑائی );

          s += سروت->pitch;
          t += نشانہ->pitch;
        }
      }
      اكسر;


    حالة FT_PIXEL_منوال_GRAY2:
      {
        FT_UInt  i;


        نشانہ->عدد_grays = 4;

        لكل ( i = سروت->صفاں; i > 0; i-- )
        {
          FT_Byte*  ss = s;
          FT_Byte*  tt = t;
          FT_UInt   j;


          /* get the full bytes */
          لكل ( j = سروت->چوڑائی >> 2; j > 0; j-- )
          {
            FT_Int  val = ss[0];


            tt[0] = (FT_Byte)( ( val & 0xC0 ) >> 6 );
            tt[1] = (FT_Byte)( ( val & 0x30 ) >> 4 );
            tt[2] = (FT_Byte)( ( val & 0x0C ) >> 2 );
            tt[3] = (FT_Byte)( ( val & 0x03 ) );

            ss += 1;
            tt += 4;
          }

          j = سروت->چوڑائی & 3;
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

          s += سروت->pitch;
          t += نشانہ->pitch;
        }
      }
      اكسر;


    حالة FT_PIXEL_منوال_GRAY4:
      {
        FT_UInt  i;


        نشانہ->عدد_grays = 16;

        لكل ( i = سروت->صفاں; i > 0; i-- )
        {
          FT_Byte*  ss = s;
          FT_Byte*  tt = t;
          FT_UInt   j;


          /* get the full bytes */
          لكل ( j = سروت->چوڑائی >> 1; j > 0; j-- )
          {
            FT_Int  val = ss[0];


            tt[0] = (FT_Byte)( ( val & 0xF0 ) >> 4 );
            tt[1] = (FT_Byte)( ( val & 0x0F ) );

            ss += 1;
            tt += 2;
          }

          إذا ( سروت->چوڑائی & 1 )
            tt[0] = (FT_Byte)( ( ss[0] & 0xF0 ) >> 4 );

          s += سروت->pitch;
          t += نشانہ->pitch;
        }
      }
      اكسر;


    حالة FT_PIXEL_منوال_BGRA:
      {
        FT_UInt  i;


        نشانہ->عدد_grays = 256;

        لكل ( i = سروت->صفاں; i > 0; i-- )
        {
          FT_Byte*  ss = s;
          FT_Byte*  tt = t;
          FT_UInt   j;


          لكل ( j = سروت->چوڑائی; j > 0; j-- )
          {
            tt[0] = ft_سلیٹی_for_premultiplied_srgb_bgra( ss );

            ss += 4;
            tt += 1;
          }

          s += سروت->pitch;
          t += نشانہ->pitch;
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
                   ثابت FT_Bitmap*  سروت_2,
                   ثابت FT_سمتیہ   سروت_وتھ,
                   FT_Bitmap*        نشانہ,
                   FT_سمتیہ        *atarget_وتھ,
                   FT_Color          color )
  {
    FT_Error   error = FT_Err_Ok;
    FT_حافظہ  حافظہ;

    FT_Bitmap         سروت_bitmap;
    ثابت FT_Bitmap*  سروت;

    FT_Bool  free_سروت_bitmap          = 0;
    FT_Bool  free_نشانہ_bitmap_on_error = 0;

    FT_BBox  sbx, tbx, fbx;

    بنانشانی صحيح  final_صفاں, final_چوڑائی;


    إذا ( !library || !نشانہ || !سروت_2 || !atarget_وتھ )
      أرجع FT_THROW( Invalid_دلیل );

    حافظہ = library->حافظہ;

    إذا ( !( نشانہ->pixel_منوال == FT_PIXEL_منوال_NONE     ||
            ( نشانہ->pixel_منوال == FT_PIXEL_منوال_BGRA &&
              نشانہ->بفر                           ) ) )
      أرجع FT_THROW( Invalid_دلیل );

    إذا ( سروت_2->pixel_منوال == FT_PIXEL_منوال_NONE )
      أرجع FT_Err_Ok;               /* nothing to do */

    /* pitches must have the same sign */
    إذا ( نشانہ->pixel_منوال == FT_PIXEL_منوال_BGRA &&
         ( سروت_2->pitch ^ نشانہ->pitch ) < 0   )
      أرجع FT_THROW( Invalid_دلیل );

    إذا ( !( سروت_2->چوڑائی && سروت_2->صفاں ) )
      أرجع FT_Err_Ok;               /* nothing to do */

    /* get source bitmap dimensions assuming integer offsets */
    sbx.xMin = ( سروت_وتھ.x >> 6 );
    sbx.yMin = ( سروت_وتھ.y >> 6 ) - سروت_2->صفاں;
    sbx.xMax = ( سروت_وتھ.x >> 6 ) + سروت_2->چوڑائی;
    sbx.yMax = ( سروت_وتھ.y >> 6 );

    FT_TRACE5(( "FT_Bitmap_Blend:\n" ));
    FT_TRACE5(( "  source bitmap: (%ld, %ld) -- (%ld, %ld); %u x %u\n",
      sbx.xMin, sbx.yMin, sbx.xMax, sbx.yMax,
      سروت_2->چوڑائی, سروت_2->صفاں ));

    /* sanity check */
    إذا ( sbx.xMin > sbx.xMax      || sbx.yMin > sbx.yMax     ||
         سروت_2->چوڑائی > 0x7FFFU || سروت_2->صفاں > 0x7FFFU )
    {
      FT_TRACE5(( "FT_Bitmap_Blend: source dimension overflow\n" ));
      أرجع FT_THROW( Invalid_دلیل );
    }

    إذا ( !نشانہ->چوڑائی || !نشانہ->صفاں )
    {
      FT_TRACE5(( "  target bitmap: empty\n" ));

      final_چوڑائی = سروت_2->چوڑائی;
      final_صفاں  = سروت_2->صفاں;

      /* create new empty bitmap */
      نشانہ->چوڑائی      = final_چوڑائی;
      نشانہ->صفاں       = final_صفاں;
      نشانہ->pixel_منوال = FT_PIXEL_منوال_BGRA;
      نشانہ->pitch      = (صحيح)final_چوڑائی * 4;
      نشانہ->عدد_grays  = 256;

      إذا ( FT_ALLOC_MULT( نشانہ->بفر, نشانہ->صفاں, نشانہ->pitch ) )
        أرجع error;

      free_نشانہ_bitmap_on_error = 1;

      fbx = sbx;
    }
    وإلا
    {
      /* get target bitmap dimensions assuming integer offsets */
      tbx.xMin = ( atarget_وتھ->x >> 6 );
      tbx.yMin = ( atarget_وتھ->y >> 6 ) - نشانہ->صفاں;
      tbx.xMax = ( atarget_وتھ->x >> 6 ) + نشانہ->چوڑائی;
      tbx.yMax = ( atarget_وتھ->y >> 6 );

      FT_TRACE5(( "  target bitmap: (%ld, %ld) -- (%ld, %ld); %u x %u\n",
        tbx.xMin, tbx.yMin, tbx.xMax, tbx.yMax,
        نشانہ->چوڑائی, نشانہ->صفاں ));

      /* sanity check */
      إذا ( tbx.xMin > tbx.xMax || tbx.yMin > tbx.yMax )
      {
        FT_TRACE5(( "FT_Bitmap_Blend: targget dimension overflow\n" ));
        أرجع FT_THROW( Invalid_دلیل );
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
        أرجع FT_THROW( Invalid_دلیل );
      }

      final_چوڑائی = fbx.xMax - fbx.xMin;
      final_صفاں  = fbx.yMax - fbx.yMin;

      /* adjust target bitmap to enlarged size */
      إذا ( نشانہ->چوڑائی < final_چوڑائی ||
           نشانہ->صفاں  < final_صفاں  )
      {
        صحيح  pitch, new_pitch;

        بنانشانی حرف*  بفر = عدم;


        pitch = نشانہ->pitch;

        إذا ( pitch < 0 )
          pitch = -pitch;

        new_pitch = (صحيح)final_چوڑائی * 4;

        /* TODO: provide an in-buffer solution for large bitmaps */
        /*       to avoid allocation of a new buffer             */
        إذا ( FT_ALLOC_MULT( بفر, final_صفاں, new_pitch ) )
          اذهب Error;

        /* copy data to new buffer */

        /* the bitmap flow is from top to bottom, */
        /* but y is measured from bottom to top   */
        إذا ( نشانہ->pitch < 0 )
        {
          /* XXX */
        }
        وإلا
        {
          FT_Pos  x = tbx.xMin - fbx.xMin;
          FT_Pos  y = tbx.yMin - fbx.yMin;

          بنانشانی حرف*  p =
            نشانہ->بفر;
          بنانشانی حرف*  q =
            بفر +
            ( final_صفاں - y - نشانہ->صفاں ) * new_pitch +
            x * 4;
          بنانشانی حرف*  حد_بندی_p =
            p + pitch * (صحيح)نشانہ->صفاں;


          طالما ( p < حد_بندی_p )
          {
            FT_MEM_COPY( q, p, pitch );

            p += pitch;
            q += new_pitch;
          }
        }

        FT_FREE( نشانہ->بفر );

        نشانہ->چوڑائی = final_چوڑائی;
        نشانہ->صفاں  = final_صفاں;

        إذا ( نشانہ->pitch < 0 )
          نشانہ->pitch = -new_pitch;
        وإلا
          نشانہ->pitch = new_pitch;

        نشانہ->بفر = بفر;
      }
    }

    FT_TRACE5(( "  final bitmap: (%ld, %ld) -- (%ld, %ld); %u x %u\n",
      fbx.xMin, fbx.yMin, fbx.xMax, fbx.yMax,
      final_چوڑائی, final_صفاں ));

    /* adjust source bitmap if necessary */
    إذا ( سروت_2->pixel_منوال != FT_PIXEL_منوال_سلیٹی )
    {
      FT_Bitmap_Init( &سروت_bitmap );
      error = FT_Bitmap_Convert( library, سروت_2, &سروت_bitmap, 1 );
      إذا ( error )
        اذهب Error;

      سروت             = &سروت_bitmap;
      free_سروت_bitmap = 1;
    }
    وإلا
      سروت = سروت_2;

    /* do blending; the code below returns pre-multiplied channels, */
    /* similar to what FreeType gets from `CBDT' tables             */

    /* the bitmap flow is from top to bottom, */
    /* but y is measured from bottom to top   */
    إذا ( نشانہ->pitch < 0 )
    {
      /* XXX */
    }
    وإلا
    {
      FT_Pos  x = sbx.xMin - fbx.xMin;
      FT_Pos  y = sbx.yMin - fbx.yMin;

      بنانشانی حرف*  p =
        سروت->بفر;
      بنانشانی حرف*  q =
        نشانہ->بفر +
        ( نشانہ->صفاں - y - سروت->صفاں ) * نشانہ->pitch +
        x * 4;
      بنانشانی حرف*  حد_بندی_p =
        p + سروت->pitch * (صحيح)سروت->صفاں;


      طالما ( p < حد_بندی_p )
      {
        بنانشانی حرف*  r       = p;
        بنانشانی حرف*  s       = q;
        بنانشانی حرف*  حد_بندی_r = r + سروت->چوڑائی;


        طالما ( r < حد_بندی_r )
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


          *s++ = (بنانشانی حرف)( bb * ba2 / 255 + fb );
          *s++ = (بنانشانی حرف)( bg * ba2 / 255 + fg );
          *s++ = (بنانشانی حرف)( br * ba2 / 255 + fr );
          *s++ = (بنانشانی حرف)( ba * ba2 / 255 + fa );
        }

        p += سروت->pitch;
        q += نشانہ->pitch;
      }
    }

    atarget_وتھ->x = fbx.xMin * 64;
    atarget_وتھ->y = fbx.yMax * 64;

  Error:
    إذا ( error && free_نشانہ_bitmap_on_error )
      FT_Bitmap_پورا( library, نشانہ );

    إذا ( free_سروت_bitmap )
      FT_Bitmap_پورا( library, &سروت_bitmap );

    أرجع error;
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Glyph_تھاں_Own_Bitmap( FT_Glyph_تھاں  تھاں )
  {
    إذا ( تھاں && تھاں->format == FT_GLYPH_FORMAT_BITMAP   &&
         !( تھاں->اندرونی->flags & FT_GLYPH_OWN_BITMAP ) )
    {
      FT_Bitmap  bitmap;
      FT_Error   error;


      FT_Bitmap_Init( &bitmap );
      error = FT_Bitmap_Copy( تھاں->library, &تھاں->bitmap, &bitmap );
      إذا ( error )
        أرجع error;

      تھاں->bitmap = bitmap;
      تھاں->اندرونی->flags |= FT_GLYPH_OWN_BITMAP;
    }

    أرجع FT_Err_Ok;
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Bitmap_پورا( FT_Library  library,
                  FT_Bitmap  *bitmap )
  {
    FT_حافظہ  حافظہ;


    إذا ( !library )
      أرجع FT_THROW( Invalid_Library_Handle );

    إذا ( !bitmap )
      أرجع FT_THROW( Invalid_دلیل );

    حافظہ = library->حافظہ;

    FT_FREE( bitmap->بفر );
    *bitmap = null_bitmap;

    أرجع FT_Err_Ok;
  }


/*سرا*/
