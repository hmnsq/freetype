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
#تعریف_لغوه  FT_COMPONENT
#تعریف_کړه FT_COMPONENT  bitmap


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
                  ثابت FT_Bitmap  *سرچینه,
                  FT_Bitmap        *موخه)
  {
    FT_حافظه  حافظه;
    FT_Error   error  = FT_Err_Ok;
    FT_Int     pitch;
    FT_Int     flip;


    إذا ( !library )
      أرجع FT_THROW( Invalid_Library_Handle );

    إذا ( !سرچینه || !موخه )
      أرجع FT_THROW( Invalid_دلیل );

    إذا ( سرچینه == موخه )
      أرجع FT_Err_Ok;

    flip = ( سرچینه->pitch < 0 && موخه->pitch > 0 ) ||
           ( سرچینه->pitch > 0 && موخه->pitch < 0 );

    حافظه = library->حافظه;
    FT_FREE( موخه->بفر );

    *موخه = *سرچینه;

    إذا ( flip )
      موخه->pitch = -موخه->pitch;

    إذا ( !سرچینه->بفر )
      أرجع FT_Err_Ok;

    pitch  = سرچینه->pitch;
    إذا ( pitch < 0 )
      pitch = -pitch;

    FT_MEM_QALLOC_MULT( موخه->بفر, موخه->لیکې, pitch );

    إذا ( !error )
    {
      إذا ( flip )
      {
        /* take care of bitmap flow */
        FT_UInt   i;
        FT_Byte*  s = سرچینه->بفر;
        FT_Byte*  t = موخه->بفر;


        t += (FT_ULong)pitch * ( موخه->لیکې - 1 );

        لكل ( i = موخه->لیکې; i > 0; i-- )
        {
          FT_ARRAY_COPY( t, s, pitch );

          s += pitch;
          t -= pitch;
        }
      }
      وإلا
        FT_MEM_COPY( موخه->بفر, سرچینه->بفر,
                     (FT_Long)سرچینه->لیکې * pitch );
    }

    أرجع error;
  }


  /* Enlarge `bitmap' horizontally and vertically by `xpixels' */
  /* and `ypixels', respectively.                              */

  ساكن FT_Error
  ft_bitmap_assure_بفر( FT_حافظه   حافظه,
                           FT_Bitmap*  bitmap,
                           FT_UInt     xpixels,
                           FT_UInt     ypixels )
  {
    FT_Error        error;
    بینښه صحيح    pitch;
    بینښه صحيح    new_pitch;
    FT_UInt         bpp;
    FT_UInt         پلنوالی, لوړوالی;
    بینښه حرف*  بفر = عدم;


    پلنوالی  = bitmap->پلنوالی;
    لوړوالی = bitmap->لیکې;
    pitch  = (بینښه صحيح)FT_ABS( bitmap->pitch );

    بدل ( bitmap->pixel_ډېرپېښ )
    {
    حالة FT_PIXEL_ډېرپېښ_MONO:
      bpp       = 1;
      new_pitch = ( پلنوالی + xpixels + 7 ) >> 3;
      اكسر;
    حالة FT_PIXEL_ډېرپېښ_GRAY2:
      bpp       = 2;
      new_pitch = ( پلنوالی + xpixels + 3 ) >> 2;
      اكسر;
    حالة FT_PIXEL_ډېرپېښ_GRAY4:
      bpp       = 4;
      new_pitch = ( پلنوالی + xpixels + 1 ) >> 1;
      اكسر;
    حالة FT_PIXEL_ډېرپېښ_خړ:
    حالة FT_PIXEL_ډېرپېښ_LCD:
    حالة FT_PIXEL_ډېرپېښ_LCD_V:
      bpp       = 8;
      new_pitch = پلنوالی + xpixels;
      اكسر;
    افتراضي:
      أرجع FT_THROW( Invalid_Glyph_Format );
    }

    /* if no need to allocate memory */
    إذا ( ypixels == 0 && new_pitch <= pitch )
    {
      /* zero the padding */
      FT_UInt  بټ_پلنوالی = pitch * 8;
      FT_UInt  بټ_وروستی  = ( پلنوالی + xpixels ) * bpp;


      إذا ( بټ_وروستی < بټ_پلنوالی )
      {
        FT_Byte*  کرښه  = bitmap->بفر + ( بټ_وروستی >> 3 );
        FT_Byte*  پای_ټکی   = bitmap->بفر + pitch;
        FT_UInt   ښويدنه = بټ_وروستی & 7;
        FT_UInt   ماسک  = 0xFF00U >> ښويدنه;
        FT_UInt   شمېر = لوړوالی;


        لكل ( ; شمېر > 0; شمېر--, کرښه += pitch, پای_ټکی += pitch )
        {
          FT_Byte*  اكتب = کرښه;


          إذا ( ښويدنه > 0 )
          {
            اكتب[0] = (FT_Byte)( اكتب[0] & ماسک );
            اكتب++;
          }
          إذا ( اكتب < پای_ټکی )
            FT_MEM_ZERO( اكتب, پای_ټکی - اكتب );
        }
      }

      أرجع FT_Err_Ok;
    }

    /* otherwise allocate new buffer */
    إذا ( FT_QALLOC_MULT( بفر, bitmap->لیکې + ypixels, new_pitch ) )
      أرجع error;

    /* new rows get added at the top of the bitmap, */
    /* thus take care of the flow direction         */
    إذا ( bitmap->pitch > 0 )
    {
      FT_UInt  len = ( پلنوالی * bpp + 7 ) >> 3;

      بینښه حرف*  دننه  = bitmap->بفر;
      بینښه حرف*  بهر = بفر;

      بینښه حرف*  حد = bitmap->بفر + pitch * bitmap->لیکې;
      بینښه صحيح    delta = new_pitch - len;


      FT_MEM_ZERO( بهر, new_pitch * ypixels );
      بهر += new_pitch * ypixels;

      طالما ( دننه < حد )
      {
        FT_MEM_COPY( بهر, دننه, len );
        دننه  += pitch;
        بهر += len;

        /* we use FT_QALLOC_MULT, which doesn't zero out the buffer;      */
        /* consequently, we have to manually zero out the remaining bytes */
        FT_MEM_ZERO( بهر, delta );
        بهر += delta;
      }
    }
    وإلا
    {
      FT_UInt  len = ( پلنوالی * bpp + 7 ) >> 3;

      بینښه حرف*  دننه  = bitmap->بفر;
      بینښه حرف*  بهر = بفر;

      بینښه حرف*  حد = bitmap->بفر + pitch * bitmap->لیکې;
      بینښه صحيح    delta = new_pitch - len;


      طالما ( دننه < حد )
      {
        FT_MEM_COPY( بهر, دننه, len );
        دننه  += pitch;
        بهر += len;

        FT_MEM_ZERO( بهر, delta );
        بهر += delta;
      }

      FT_MEM_ZERO( بهر, new_pitch * ypixels );
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
    بینښه حرف*  p;
    FT_Int          i, x, pitch;
    FT_UInt         y;
    FT_Int          xstr, ystr;


    إذا ( !library )
      أرجع FT_THROW( Invalid_Library_Handle );

    إذا ( !bitmap || !bitmap->بفر )
      أرجع FT_THROW( Invalid_دلیل );

    إذا ( ( ( FT_PIX_پړاو( xStrength ) >> 6 ) > FT_INT_MAX ) ||
         ( ( FT_PIX_پړاو( yStrength ) >> 6 ) > FT_INT_MAX ) )
      أرجع FT_THROW( Invalid_دلیل );

    xstr = (FT_Int)FT_PIX_پړاو( xStrength ) >> 6;
    ystr = (FT_Int)FT_PIX_پړاو( yStrength ) >> 6;

    إذا ( xstr == 0 && ystr == 0 )
      أرجع FT_Err_Ok;
    وإلا إذا ( xstr < 0 || ystr < 0 )
      أرجع FT_THROW( Invalid_دلیل );

    بدل ( bitmap->pixel_ډېرپېښ )
    {
    حالة FT_PIXEL_ډېرپېښ_GRAY2:
    حالة FT_PIXEL_ډېرپېښ_GRAY4:
      {
        FT_Bitmap  tmp;


        /* convert to 8bpp */
        FT_Bitmap_Init( &tmp );
        error = FT_Bitmap_Convert( library, bitmap, &tmp, 1 );
        إذا ( error )
          أرجع error;

        FT_Bitmap_بشپړ( library, bitmap );
        *bitmap = tmp;
      }
      اكسر;

    حالة FT_PIXEL_ډېرپېښ_MONO:
      إذا ( xstr > 8 )
        xstr = 8;
      اكسر;

    حالة FT_PIXEL_ډېرپېښ_LCD:
      xstr *= 3;
      اكسر;

    حالة FT_PIXEL_ډېرپېښ_LCD_V:
      ystr *= 3;
      اكسر;

    حالة FT_PIXEL_ډېرپېښ_BGRA:
      /* We don't embolden color glyphs. */
      أرجع FT_Err_Ok;
    }

    error = ft_bitmap_assure_بفر( library->حافظه, bitmap,
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
      p = bitmap->بفر + (FT_UInt)pitch * ( bitmap->لیکې - 1 );
    }

    /* for each row */
    لكل ( y = 0; y < bitmap->لیکې; y++ )
    {
      /*
       * Horizontally:
       *
       * From the last pixel on, make each pixel or'ed with the
       * `xstr' pixels before it.
       */
      لكل ( x = pitch - 1; x >= 0; x-- )
      {
        بینښه حرف  tmp;


        tmp = p[x];
        لكل ( i = 1; i <= xstr; i++ )
        {
          إذا ( bitmap->pixel_ډېرپېښ == FT_PIXEL_ډېرپېښ_MONO )
          {
            p[x] |= tmp >> i;

            /* the maximum value of 8 for `xstr' comes from here */
            إذا ( x > 0 )
              p[x] |= p[x - 1] << ( 8 - i );

#إذا 0
            إذا ( p[x] == 0xFF )
              اكسر;
#د_که_پای
          }
          وإلا
          {
            إذا ( x - i >= 0 )
            {
              إذا ( p[x] + p[x - i] > bitmap->عدد_grays - 1 )
              {
                p[x] = (بینښه حرف)( bitmap->عدد_grays - 1 );
                اكسر;
              }
              وإلا
              {
                p[x] = (بینښه حرف)( p[x] + p[x - i] );
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
        بینښه حرف*  q;


        q = p - bitmap->pitch * x;
        لكل ( i = 0; i < pitch; i++ )
          q[i] |= p[i];
      }

      p += bitmap->pitch;
    }

    bitmap->پلنوالی += (FT_UInt)xstr;
    bitmap->لیکې += (FT_UInt)ystr;

    أرجع FT_Err_Ok;
  }


  ساكن FT_Byte
  ft_خړ_for_premultiplied_srgb_bgra( ثابت FT_Byte*  bgra )
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
                     ثابت FT_Bitmap  *سرچینه,
                     FT_Bitmap        *موخه,
                     FT_Int            alignment )
  {
    FT_Error   error = FT_Err_Ok;
    FT_حافظه  حافظه;

    FT_Byte*  s;
    FT_Byte*  t;


    إذا ( !library )
      أرجع FT_THROW( Invalid_Library_Handle );

    إذا ( !سرچینه || !موخه )
      أرجع FT_THROW( Invalid_دلیل );

    حافظه = library->حافظه;

    بدل ( سرچینه->pixel_ډېرپېښ )
    {
    حالة FT_PIXEL_ډېرپېښ_MONO:
    حالة FT_PIXEL_ډېرپېښ_خړ:
    حالة FT_PIXEL_ډېرپېښ_GRAY2:
    حالة FT_PIXEL_ډېرپېښ_GRAY4:
    حالة FT_PIXEL_ډېرپېښ_LCD:
    حالة FT_PIXEL_ډېرپېښ_LCD_V:
    حالة FT_PIXEL_ډېرپېښ_BGRA:
      {
        FT_Int  پلنوالی = (FT_Int)سرچینه->پلنوالی;
        FT_Int  neg   = ( موخه->pitch == 0 && سرچینه->pitch < 0 ) ||
                          موخه->pitch  < 0;


        FT_Bitmap_بشپړ( library, موخه );

        موخه->pixel_ډېرپېښ = FT_PIXEL_ډېرپېښ_خړ;
        موخه->لیکې       = سرچینه->لیکې;
        موخه->پلنوالی      = سرچینه->پلنوالی;

        إذا ( alignment )
        {
          FT_Int  rem = پلنوالی % alignment;


          إذا ( rem )
            پلنوالی = alignment > 0 ? پلنوالی - rem + alignment
                                  : پلنوالی - rem - alignment;
        }

        إذا ( FT_QALLOC_MULT( موخه->بفر, موخه->لیکې, پلنوالی ) )
          أرجع error;

        موخه->pitch = neg ? -پلنوالی : پلنوالی;
      }
      اكسر;

    افتراضي:
      error = FT_THROW( Invalid_دلیل );
    }

    s = سرچینه->بفر;
    t = موخه->بفر;

    /* take care of bitmap flow */
    إذا ( سرچینه->pitch < 0 )
      s -= سرچینه->pitch * (FT_Int)( سرچینه->لیکې - 1 );
    إذا ( موخه->pitch < 0 )
      t -= موخه->pitch * (FT_Int)( موخه->لیکې - 1 );

    بدل ( سرچینه->pixel_ډېرپېښ )
    {
    حالة FT_PIXEL_ډېرپېښ_MONO:
      {
        FT_UInt  i;


        موخه->عدد_grays = 2;

        لكل ( i = سرچینه->لیکې; i > 0; i-- )
        {
          FT_Byte*  ss = s;
          FT_Byte*  tt = t;
          FT_UInt   j;


          /* get the full bytes */
          لكل ( j = سرچینه->پلنوالی >> 3; j > 0; j-- )
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
          j = سرچینه->پلنوالی & 7;
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

          s += سرچینه->pitch;
          t += موخه->pitch;
        }
      }
      اكسر;


    حالة FT_PIXEL_ډېرپېښ_خړ:
    حالة FT_PIXEL_ډېرپېښ_LCD:
    حالة FT_PIXEL_ډېرپېښ_LCD_V:
      {
        FT_UInt  پلنوالی = سرچینه->پلنوالی;
        FT_UInt  i;


        موخه->عدد_grays = 256;

        لكل ( i = سرچینه->لیکې; i > 0; i-- )
        {
          FT_ARRAY_COPY( t, s, پلنوالی );

          s += سرچینه->pitch;
          t += موخه->pitch;
        }
      }
      اكسر;


    حالة FT_PIXEL_ډېرپېښ_GRAY2:
      {
        FT_UInt  i;


        موخه->عدد_grays = 4;

        لكل ( i = سرچینه->لیکې; i > 0; i-- )
        {
          FT_Byte*  ss = s;
          FT_Byte*  tt = t;
          FT_UInt   j;


          /* get the full bytes */
          لكل ( j = سرچینه->پلنوالی >> 2; j > 0; j-- )
          {
            FT_Int  val = ss[0];


            tt[0] = (FT_Byte)( ( val & 0xC0 ) >> 6 );
            tt[1] = (FT_Byte)( ( val & 0x30 ) >> 4 );
            tt[2] = (FT_Byte)( ( val & 0x0C ) >> 2 );
            tt[3] = (FT_Byte)( ( val & 0x03 ) );

            ss += 1;
            tt += 4;
          }

          j = سرچینه->پلنوالی & 3;
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

          s += سرچینه->pitch;
          t += موخه->pitch;
        }
      }
      اكسر;


    حالة FT_PIXEL_ډېرپېښ_GRAY4:
      {
        FT_UInt  i;


        موخه->عدد_grays = 16;

        لكل ( i = سرچینه->لیکې; i > 0; i-- )
        {
          FT_Byte*  ss = s;
          FT_Byte*  tt = t;
          FT_UInt   j;


          /* get the full bytes */
          لكل ( j = سرچینه->پلنوالی >> 1; j > 0; j-- )
          {
            FT_Int  val = ss[0];


            tt[0] = (FT_Byte)( ( val & 0xF0 ) >> 4 );
            tt[1] = (FT_Byte)( ( val & 0x0F ) );

            ss += 1;
            tt += 2;
          }

          إذا ( سرچینه->پلنوالی & 1 )
            tt[0] = (FT_Byte)( ( ss[0] & 0xF0 ) >> 4 );

          s += سرچینه->pitch;
          t += موخه->pitch;
        }
      }
      اكسر;


    حالة FT_PIXEL_ډېرپېښ_BGRA:
      {
        FT_UInt  i;


        موخه->عدد_grays = 256;

        لكل ( i = سرچینه->لیکې; i > 0; i-- )
        {
          FT_Byte*  ss = s;
          FT_Byte*  tt = t;
          FT_UInt   j;


          لكل ( j = سرچینه->پلنوالی; j > 0; j-- )
          {
            tt[0] = ft_خړ_for_premultiplied_srgb_bgra( ss );

            ss += 4;
            tt += 1;
          }

          s += سرچینه->pitch;
          t += موخه->pitch;
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
                   ثابت FT_Bitmap*  سرچینه_2,
                   ثابت FT_ویکټور   سرچینه_دننه_واټن,
                   FT_Bitmap*        موخه,
                   FT_ویکټور        *atarget_دننه_واټن,
                   FT_Color          color )
  {
    FT_Error   error = FT_Err_Ok;
    FT_حافظه  حافظه;

    FT_Bitmap         سرچینه_bitmap;
    ثابت FT_Bitmap*  سرچینه;

    FT_Bool  free_سرچینه_bitmap          = 0;
    FT_Bool  free_موخه_bitmap_on_error = 0;

    FT_BBox  sbx, tbx, fbx;

    بینښه صحيح  final_لیکې, final_پلنوالی;


    إذا ( !library || !موخه || !سرچینه_2 || !atarget_دننه_واټن )
      أرجع FT_THROW( Invalid_دلیل );

    حافظه = library->حافظه;

    إذا ( !( موخه->pixel_ډېرپېښ == FT_PIXEL_ډېرپېښ_NONE     ||
            ( موخه->pixel_ډېرپېښ == FT_PIXEL_ډېرپېښ_BGRA &&
              موخه->بفر                           ) ) )
      أرجع FT_THROW( Invalid_دلیل );

    إذا ( سرچینه_2->pixel_ډېرپېښ == FT_PIXEL_ډېرپېښ_NONE )
      أرجع FT_Err_Ok;               /* nothing to do */

    /* pitches must have the same sign */
    إذا ( موخه->pixel_ډېرپېښ == FT_PIXEL_ډېرپېښ_BGRA &&
         ( سرچینه_2->pitch ^ موخه->pitch ) < 0   )
      أرجع FT_THROW( Invalid_دلیل );

    إذا ( !( سرچینه_2->پلنوالی && سرچینه_2->لیکې ) )
      أرجع FT_Err_Ok;               /* nothing to do */

    /* get source bitmap dimensions assuming integer offsets */
    sbx.xMin = ( سرچینه_دننه_واټن.x >> 6 );
    sbx.yMin = ( سرچینه_دننه_واټن.y >> 6 ) - سرچینه_2->لیکې;
    sbx.xMax = ( سرچینه_دننه_واټن.x >> 6 ) + سرچینه_2->پلنوالی;
    sbx.yMax = ( سرچینه_دننه_واټن.y >> 6 );

    FT_TRACE5(( "FT_Bitmap_Blend:\n" ));
    FT_TRACE5(( "  source bitmap: (%ld, %ld) -- (%ld, %ld); %u x %u\n",
      sbx.xMin, sbx.yMin, sbx.xMax, sbx.yMax,
      سرچینه_2->پلنوالی, سرچینه_2->لیکې ));

    /* sanity check */
    إذا ( sbx.xMin > sbx.xMax      || sbx.yMin > sbx.yMax     ||
         سرچینه_2->پلنوالی > 0x7FFFU || سرچینه_2->لیکې > 0x7FFFU )
    {
      FT_TRACE5(( "FT_Bitmap_Blend: source dimension overflow\n" ));
      أرجع FT_THROW( Invalid_دلیل );
    }

    إذا ( !موخه->پلنوالی || !موخه->لیکې )
    {
      FT_TRACE5(( "  target bitmap: empty\n" ));

      final_پلنوالی = سرچینه_2->پلنوالی;
      final_لیکې  = سرچینه_2->لیکې;

      /* create new empty bitmap */
      موخه->پلنوالی      = final_پلنوالی;
      موخه->لیکې       = final_لیکې;
      موخه->pixel_ډېرپېښ = FT_PIXEL_ډېرپېښ_BGRA;
      موخه->pitch      = (صحيح)final_پلنوالی * 4;
      موخه->عدد_grays  = 256;

      إذا ( FT_ALLOC_MULT( موخه->بفر, موخه->لیکې, موخه->pitch ) )
        أرجع error;

      free_موخه_bitmap_on_error = 1;

      fbx = sbx;
    }
    وإلا
    {
      /* get target bitmap dimensions assuming integer offsets */
      tbx.xMin = ( atarget_دننه_واټن->x >> 6 );
      tbx.yMin = ( atarget_دننه_واټن->y >> 6 ) - موخه->لیکې;
      tbx.xMax = ( atarget_دننه_واټن->x >> 6 ) + موخه->پلنوالی;
      tbx.yMax = ( atarget_دننه_واټن->y >> 6 );

      FT_TRACE5(( "  target bitmap: (%ld, %ld) -- (%ld, %ld); %u x %u\n",
        tbx.xMin, tbx.yMin, tbx.xMax, tbx.yMax,
        موخه->پلنوالی, موخه->لیکې ));

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

      final_پلنوالی = fbx.xMax - fbx.xMin;
      final_لیکې  = fbx.yMax - fbx.yMin;

      /* adjust target bitmap to enlarged size */
      إذا ( موخه->پلنوالی < final_پلنوالی ||
           موخه->لیکې  < final_لیکې  )
      {
        صحيح  pitch, new_pitch;

        بینښه حرف*  بفر = عدم;


        pitch = موخه->pitch;

        إذا ( pitch < 0 )
          pitch = -pitch;

        new_pitch = (صحيح)final_پلنوالی * 4;

        /* TODO: provide an in-buffer solution for large bitmaps */
        /*       to avoid allocation of a new buffer             */
        إذا ( FT_ALLOC_MULT( بفر, final_لیکې, new_pitch ) )
          اذهب Error;

        /* copy data to new buffer */

        /* the bitmap flow is from top to bottom, */
        /* but y is measured from bottom to top   */
        إذا ( موخه->pitch < 0 )
        {
          /* XXX */
        }
        وإلا
        {
          FT_Pos  x = tbx.xMin - fbx.xMin;
          FT_Pos  y = tbx.yMin - fbx.yMin;

          بینښه حرف*  p =
            موخه->بفر;
          بینښه حرف*  q =
            بفر +
            ( final_لیکې - y - موخه->لیکې ) * new_pitch +
            x * 4;
          بینښه حرف*  حد_p =
            p + pitch * (صحيح)موخه->لیکې;


          طالما ( p < حد_p )
          {
            FT_MEM_COPY( q, p, pitch );

            p += pitch;
            q += new_pitch;
          }
        }

        FT_FREE( موخه->بفر );

        موخه->پلنوالی = final_پلنوالی;
        موخه->لیکې  = final_لیکې;

        إذا ( موخه->pitch < 0 )
          موخه->pitch = -new_pitch;
        وإلا
          موخه->pitch = new_pitch;

        موخه->بفر = بفر;
      }
    }

    FT_TRACE5(( "  final bitmap: (%ld, %ld) -- (%ld, %ld); %u x %u\n",
      fbx.xMin, fbx.yMin, fbx.xMax, fbx.yMax,
      final_پلنوالی, final_لیکې ));

    /* adjust source bitmap if necessary */
    إذا ( سرچینه_2->pixel_ډېرپېښ != FT_PIXEL_ډېرپېښ_خړ )
    {
      FT_Bitmap_Init( &سرچینه_bitmap );
      error = FT_Bitmap_Convert( library, سرچینه_2, &سرچینه_bitmap, 1 );
      إذا ( error )
        اذهب Error;

      سرچینه             = &سرچینه_bitmap;
      free_سرچینه_bitmap = 1;
    }
    وإلا
      سرچینه = سرچینه_2;

    /* do blending; the code below returns pre-multiplied channels, */
    /* similar to what FreeType gets from `CBDT' tables             */

    /* the bitmap flow is from top to bottom, */
    /* but y is measured from bottom to top   */
    إذا ( موخه->pitch < 0 )
    {
      /* XXX */
    }
    وإلا
    {
      FT_Pos  x = sbx.xMin - fbx.xMin;
      FT_Pos  y = sbx.yMin - fbx.yMin;

      بینښه حرف*  p =
        سرچینه->بفر;
      بینښه حرف*  q =
        موخه->بفر +
        ( موخه->لیکې - y - سرچینه->لیکې ) * موخه->pitch +
        x * 4;
      بینښه حرف*  حد_p =
        p + سرچینه->pitch * (صحيح)سرچینه->لیکې;


      طالما ( p < حد_p )
      {
        بینښه حرف*  r       = p;
        بینښه حرف*  s       = q;
        بینښه حرف*  حد_r = r + سرچینه->پلنوالی;


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


          *s++ = (بینښه حرف)( bb * ba2 / 255 + fb );
          *s++ = (بینښه حرف)( bg * ba2 / 255 + fg );
          *s++ = (بینښه حرف)( br * ba2 / 255 + fr );
          *s++ = (بینښه حرف)( ba * ba2 / 255 + fa );
        }

        p += سرچینه->pitch;
        q += موخه->pitch;
      }
    }

    atarget_دننه_واټن->x = fbx.xMin * 64;
    atarget_دننه_واټن->y = fbx.yMax * 64;

  Error:
    إذا ( error && free_موخه_bitmap_on_error )
      FT_Bitmap_بشپړ( library, موخه );

    إذا ( free_سرچینه_bitmap )
      FT_Bitmap_بشپړ( library, &سرچینه_bitmap );

    أرجع error;
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Glyph_خونه_Own_Bitmap( FT_Glyph_خونه  خونه )
  {
    إذا ( خونه && خونه->format == FT_GLYPH_FORMAT_BITMAP   &&
         !( خونه->دننی->flags & FT_GLYPH_OWN_BITMAP ) )
    {
      FT_Bitmap  bitmap;
      FT_Error   error;


      FT_Bitmap_Init( &bitmap );
      error = FT_Bitmap_Copy( خونه->library, &خونه->bitmap, &bitmap );
      إذا ( error )
        أرجع error;

      خونه->bitmap = bitmap;
      خونه->دننی->flags |= FT_GLYPH_OWN_BITMAP;
    }

    أرجع FT_Err_Ok;
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Bitmap_بشپړ( FT_Library  library,
                  FT_Bitmap  *bitmap )
  {
    FT_حافظه  حافظه;


    إذا ( !library )
      أرجع FT_THROW( Invalid_Library_Handle );

    إذا ( !bitmap )
      أرجع FT_THROW( Invalid_دلیل );

    حافظه = library->حافظه;

    FT_FREE( bitmap->بفر );
    *bitmap = null_bitmap;

    أرجع FT_Err_Ok;
  }


/*پای_ټکی*/
