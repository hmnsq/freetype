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
#अपरिभाषय  FT_COMPONENT
#परिभाषय FT_COMPONENT  bitmap


  स्थिर
  ध्रुव FT_Bitmap  null_bitmap = { 0, 0, 0, रिक्त, 0, 0, 0, रिक्त };


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( शून्य )
  FT_Bitmap_Init( FT_Bitmap  *abitmap )
  {
    यदि ( abitmap )
      *abitmap = null_bitmap;
  }


  /* deprecated function name; retained for ABI compatibility */

  FT_EXPORT_DEF( शून्य )
  FT_Bitmap_New( FT_Bitmap  *abitmap )
  {
    यदि ( abitmap )
      *abitmap = null_bitmap;
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Bitmap_Copy( FT_Library        library,
                  ध्रुव FT_Bitmap  *source,
                  FT_Bitmap        *target)
  {
    FT_स्मृतिः  स्मृतिः;
    FT_Error   error  = FT_Err_Ok;
    FT_Int     pitch;
    FT_Int     flip;


    यदि ( !library )
      प्रत्यागम FT_THROW( Invalid_Library_Handle );

    यदि ( !source || !target )
      प्रत्यागम FT_THROW( Invalid_तर्कः );

    यदि ( source == target )
      प्रत्यागम FT_Err_Ok;

    flip = ( source->pitch < 0 && target->pitch > 0 ) ||
           ( source->pitch > 0 && target->pitch < 0 );

    स्मृतिः = library->स्मृतिः;
    FT_FREE( target->बफर );

    *target = *source;

    यदि ( flip )
      target->pitch = -target->pitch;

    यदि ( !source->बफर )
      प्रत्यागम FT_Err_Ok;

    pitch  = source->pitch;
    यदि ( pitch < 0 )
      pitch = -pitch;

    FT_MEM_QALLOC_MULT( target->बफर, target->पङ्क्तिकाः, pitch );

    यदि ( !error )
    {
      यदि ( flip )
      {
        /* take care of bitmap flow */
        FT_UInt   i;
        FT_Byte*  s = source->बफर;
        FT_Byte*  t = target->बफर;


        t += (FT_ULong)pitch * ( target->पङ्क्तिकाः - 1 );

        प्रति ( i = target->पङ्क्तिकाः; i > 0; i-- )
        {
          FT_ARRAY_COPY( t, s, pitch );

          s += pitch;
          t -= pitch;
        }
      }
      अन्यथा
        FT_MEM_COPY( target->बफर, source->बफर,
                     (FT_Long)source->पङ्क्तिकाः * pitch );
    }

    प्रत्यागम error;
  }


  /* Enlarge `bitmap' horizontally and vertically by `xpixels' */
  /* and `ypixels', respectively.                              */

  स्थिर FT_Error
  ft_bitmap_assure_बफर( FT_स्मृतिः   स्मृतिः,
                           FT_Bitmap*  bitmap,
                           FT_UInt     xpixels,
                           FT_UInt     ypixels )
  {
    FT_Error        error;
    अचिह्नित पूर्णाङ्क    pitch;
    अचिह्नित पूर्णाङ्क    new_pitch;
    FT_UInt         bpp;
    FT_UInt         विस्तारः, height;
    अचिह्नित वर्ण*  बफर = रिक्त;


    विस्तारः  = bitmap->विस्तारः;
    height = bitmap->पङ्क्तिकाः;
    pitch  = (अचिह्नित पूर्णाङ्क)FT_ABS( bitmap->pitch );

    विकल्प ( bitmap->pixel_बहुलकम् )
    {
    पक्ष FT_PIXEL_बहुलकम्_MONO:
      bpp       = 1;
      new_pitch = ( विस्तारः + xpixels + 7 ) >> 3;
      भङ्ग;
    पक्ष FT_PIXEL_बहुलकम्_GRAY2:
      bpp       = 2;
      new_pitch = ( विस्तारः + xpixels + 3 ) >> 2;
      भङ्ग;
    पक्ष FT_PIXEL_बहुलकम्_GRAY4:
      bpp       = 4;
      new_pitch = ( विस्तारः + xpixels + 1 ) >> 1;
      भङ्ग;
    पक्ष FT_PIXEL_बहुलकम्_GRAY:
    पक्ष FT_PIXEL_बहुलकम्_LCD:
    पक्ष FT_PIXEL_बहुलकम्_LCD_V:
      bpp       = 8;
      new_pitch = विस्तारः + xpixels;
      भङ्ग;
    स्वभाव:
      प्रत्यागम FT_THROW( Invalid_Glyph_Format );
    }

    /* if no need to allocate memory */
    यदि ( ypixels == 0 && new_pitch <= pitch )
    {
      /* zero the padding */
      FT_UInt  bit_विस्तारः = pitch * 8;
      FT_UInt  bit_चरमः  = ( विस्तारः + xpixels ) * bpp;


      यदि ( bit_चरमः < bit_विस्तारः )
      {
        FT_Byte*  पङ्क्तिः  = bitmap->बफर + ( bit_चरमः >> 3 );
        FT_Byte*  अन्तः   = bitmap->बफर + pitch;
        FT_UInt   सरणम् = bit_चरमः & 7;
        FT_UInt   आवरणम्  = 0xFF00U >> सरणम्;
        FT_UInt   गणनम् = height;


        प्रति ( ; गणनम् > 0; गणनम्--, पङ्क्तिः += pitch, अन्तः += pitch )
        {
          FT_Byte*  लिख = पङ्क्तिः;


          यदि ( सरणम् > 0 )
          {
            लिख[0] = (FT_Byte)( लिख[0] & आवरणम् );
            लिख++;
          }
          यदि ( लिख < अन्तः )
            FT_MEM_ZERO( लिख, अन्तः - लिख );
        }
      }

      प्रत्यागम FT_Err_Ok;
    }

    /* otherwise allocate new buffer */
    यदि ( FT_QALLOC_MULT( बफर, bitmap->पङ्क्तिकाः + ypixels, new_pitch ) )
      प्रत्यागम error;

    /* new rows get added at the top of the bitmap, */
    /* thus take care of the flow direction         */
    यदि ( bitmap->pitch > 0 )
    {
      FT_UInt  len = ( विस्तारः * bpp + 7 ) >> 3;

      अचिह्नित वर्ण*  अन्तर्  = bitmap->बफर;
      अचिह्नित वर्ण*  बहिर्गमनम् = बफर;

      अचिह्नित वर्ण*  सीमा = bitmap->बफर + pitch * bitmap->पङ्क्तिकाः;
      अचिह्नित पूर्णाङ्क    delta = new_pitch - len;


      FT_MEM_ZERO( बहिर्गमनम्, new_pitch * ypixels );
      बहिर्गमनम् += new_pitch * ypixels;

      यावत् ( अन्तर् < सीमा )
      {
        FT_MEM_COPY( बहिर्गमनम्, अन्तर्, len );
        अन्तर्  += pitch;
        बहिर्गमनम् += len;

        /* we use FT_QALLOC_MULT, which doesn't zero out the buffer;      */
        /* consequently, we have to manually zero out the remaining bytes */
        FT_MEM_ZERO( बहिर्गमनम्, delta );
        बहिर्गमनम् += delta;
      }
    }
    अन्यथा
    {
      FT_UInt  len = ( विस्तारः * bpp + 7 ) >> 3;

      अचिह्नित वर्ण*  अन्तर्  = bitmap->बफर;
      अचिह्नित वर्ण*  बहिर्गमनम् = बफर;

      अचिह्नित वर्ण*  सीमा = bitmap->बफर + pitch * bitmap->पङ्क्तिकाः;
      अचिह्नित पूर्णाङ्क    delta = new_pitch - len;


      यावत् ( अन्तर् < सीमा )
      {
        FT_MEM_COPY( बहिर्गमनम्, अन्तर्, len );
        अन्तर्  += pitch;
        बहिर्गमनम् += len;

        FT_MEM_ZERO( बहिर्गमनम्, delta );
        बहिर्गमनम् += delta;
      }

      FT_MEM_ZERO( बहिर्गमनम्, new_pitch * ypixels );
    }

    FT_FREE( bitmap->बफर );
    bitmap->बफर = बफर;

    /* set pitch only, width and height are left untouched */
    यदि ( bitmap->pitch < 0 )
      bitmap->pitch = -(पूर्णाङ्क)new_pitch;
    अन्यथा
      bitmap->pitch = (पूर्णाङ्क)new_pitch;

    प्रत्यागम FT_Err_Ok;
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Bitmap_Embolden( FT_Library  library,
                      FT_Bitmap*  bitmap,
                      FT_Pos      xStrength,
                      FT_Pos      yStrength )
  {
    FT_Error        error;
    अचिह्नित वर्ण*  p;
    FT_Int          i, x, pitch;
    FT_UInt         y;
    FT_Int          xstr, ystr;


    यदि ( !library )
      प्रत्यागम FT_THROW( Invalid_Library_Handle );

    यदि ( !bitmap || !bitmap->बफर )
      प्रत्यागम FT_THROW( Invalid_तर्कः );

    यदि ( ( ( FT_PIX_आवर्तनम्( xStrength ) >> 6 ) > FT_INT_MAX ) ||
         ( ( FT_PIX_आवर्तनम्( yStrength ) >> 6 ) > FT_INT_MAX ) )
      प्रत्यागम FT_THROW( Invalid_तर्कः );

    xstr = (FT_Int)FT_PIX_आवर्तनम्( xStrength ) >> 6;
    ystr = (FT_Int)FT_PIX_आवर्तनम्( yStrength ) >> 6;

    यदि ( xstr == 0 && ystr == 0 )
      प्रत्यागम FT_Err_Ok;
    अन्यथा यदि ( xstr < 0 || ystr < 0 )
      प्रत्यागम FT_THROW( Invalid_तर्कः );

    विकल्प ( bitmap->pixel_बहुलकम् )
    {
    पक्ष FT_PIXEL_बहुलकम्_GRAY2:
    पक्ष FT_PIXEL_बहुलकम्_GRAY4:
      {
        FT_Bitmap  tmp;


        /* convert to 8bpp */
        FT_Bitmap_Init( &tmp );
        error = FT_Bitmap_Convert( library, bitmap, &tmp, 1 );
        यदि ( error )
          प्रत्यागम error;

        FT_Bitmap_समाप्तम्( library, bitmap );
        *bitmap = tmp;
      }
      भङ्ग;

    पक्ष FT_PIXEL_बहुलकम्_MONO:
      यदि ( xstr > 8 )
        xstr = 8;
      भङ्ग;

    पक्ष FT_PIXEL_बहुलकम्_LCD:
      xstr *= 3;
      भङ्ग;

    पक्ष FT_PIXEL_बहुलकम्_LCD_V:
      ystr *= 3;
      भङ्ग;

    पक्ष FT_PIXEL_बहुलकम्_BGRA:
      /* We don't embolden color glyphs. */
      प्रत्यागम FT_Err_Ok;
    }

    error = ft_bitmap_assure_बफर( library->स्मृतिः, bitmap,
                                     (FT_UInt)xstr, (FT_UInt)ystr );
    यदि ( error )
      प्रत्यागम error;

    /* take care of bitmap flow */
    pitch = bitmap->pitch;
    यदि ( pitch > 0 )
      p = bitmap->बफर + pitch * ystr;
    अन्यथा
    {
      pitch = -pitch;
      p = bitmap->बफर + (FT_UInt)pitch * ( bitmap->पङ्क्तिकाः - 1 );
    }

    /* for each row */
    प्रति ( y = 0; y < bitmap->पङ्क्तिकाः; y++ )
    {
      /*
       * Horizontally:
       *
       * From the last pixel on, make each pixel or'ed with the
       * `xstr' pixels before it.
       */
      प्रति ( x = pitch - 1; x >= 0; x-- )
      {
        अचिह्नित वर्ण  tmp;


        tmp = p[x];
        प्रति ( i = 1; i <= xstr; i++ )
        {
          यदि ( bitmap->pixel_बहुलकम् == FT_PIXEL_बहुलकम्_MONO )
          {
            p[x] |= tmp >> i;

            /* the maximum value of 8 for `xstr' comes from here */
            यदि ( x > 0 )
              p[x] |= p[x - 1] << ( 8 - i );

#यदि 0
            यदि ( p[x] == 0xFF )
              भङ्ग;
#यदि_अन्तः
          }
          अन्यथा
          {
            यदि ( x - i >= 0 )
            {
              यदि ( p[x] + p[x - i] > bitmap->अङ्कः_grays - 1 )
              {
                p[x] = (अचिह्नित वर्ण)( bitmap->अङ्कः_grays - 1 );
                भङ्ग;
              }
              अन्यथा
              {
                p[x] = (अचिह्नित वर्ण)( p[x] + p[x - i] );
                यदि ( p[x] == bitmap->अङ्कः_grays - 1 )
                  भङ्ग;
              }
            }
            अन्यथा
              भङ्ग;
          }
        }
      }

      /*
       * Vertically:
       *
       * Make the above `ystr' rows or'ed with it.
       */
      प्रति ( x = 1; x <= ystr; x++ )
      {
        अचिह्नित वर्ण*  q;


        q = p - bitmap->pitch * x;
        प्रति ( i = 0; i < pitch; i++ )
          q[i] |= p[i];
      }

      p += bitmap->pitch;
    }

    bitmap->विस्तारः += (FT_UInt)xstr;
    bitmap->पङ्क्तिकाः += (FT_UInt)ystr;

    प्रत्यागम FT_Err_Ok;
  }


  स्थिर FT_Byte
  ft_gray_for_premultiplied_srgb_bgra( ध्रुव FT_Byte*  bgra )
  {
    FT_UInt  a = bgra[3];
    FT_UInt  l;


    /* Short-circuit transparent color to avoid division by zero. */
    यदि ( !a )
      प्रत्यागम 0;

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

    प्रत्यागम (FT_Byte)( a - l / a );
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Bitmap_Convert( FT_Library        library,
                     ध्रुव FT_Bitmap  *source,
                     FT_Bitmap        *target,
                     FT_Int            alignment )
  {
    FT_Error   error = FT_Err_Ok;
    FT_स्मृतिः  स्मृतिः;

    FT_Byte*  s;
    FT_Byte*  t;


    यदि ( !library )
      प्रत्यागम FT_THROW( Invalid_Library_Handle );

    यदि ( !source || !target )
      प्रत्यागम FT_THROW( Invalid_तर्कः );

    स्मृतिः = library->स्मृतिः;

    विकल्प ( source->pixel_बहुलकम् )
    {
    पक्ष FT_PIXEL_बहुलकम्_MONO:
    पक्ष FT_PIXEL_बहुलकम्_GRAY:
    पक्ष FT_PIXEL_बहुलकम्_GRAY2:
    पक्ष FT_PIXEL_बहुलकम्_GRAY4:
    पक्ष FT_PIXEL_बहुलकम्_LCD:
    पक्ष FT_PIXEL_बहुलकम्_LCD_V:
    पक्ष FT_PIXEL_बहुलकम्_BGRA:
      {
        FT_Int  विस्तारः = (FT_Int)source->विस्तारः;
        FT_Int  neg   = ( target->pitch == 0 && source->pitch < 0 ) ||
                          target->pitch  < 0;


        FT_Bitmap_समाप्तम्( library, target );

        target->pixel_बहुलकम् = FT_PIXEL_बहुलकम्_GRAY;
        target->पङ्क्तिकाः       = source->पङ्क्तिकाः;
        target->विस्तारः      = source->विस्तारः;

        यदि ( alignment )
        {
          FT_Int  rem = विस्तारः % alignment;


          यदि ( rem )
            विस्तारः = alignment > 0 ? विस्तारः - rem + alignment
                                  : विस्तारः - rem - alignment;
        }

        यदि ( FT_QALLOC_MULT( target->बफर, target->पङ्क्तिकाः, विस्तारः ) )
          प्रत्यागम error;

        target->pitch = neg ? -विस्तारः : विस्तारः;
      }
      भङ्ग;

    स्वभाव:
      error = FT_THROW( Invalid_तर्कः );
    }

    s = source->बफर;
    t = target->बफर;

    /* take care of bitmap flow */
    यदि ( source->pitch < 0 )
      s -= source->pitch * (FT_Int)( source->पङ्क्तिकाः - 1 );
    यदि ( target->pitch < 0 )
      t -= target->pitch * (FT_Int)( target->पङ्क्तिकाः - 1 );

    विकल्प ( source->pixel_बहुलकम् )
    {
    पक्ष FT_PIXEL_बहुलकम्_MONO:
      {
        FT_UInt  i;


        target->अङ्कः_grays = 2;

        प्रति ( i = source->पङ्क्तिकाः; i > 0; i-- )
        {
          FT_Byte*  ss = s;
          FT_Byte*  tt = t;
          FT_UInt   j;


          /* get the full bytes */
          प्रति ( j = source->विस्तारः >> 3; j > 0; j-- )
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
          j = source->विस्तारः & 7;
          यदि ( j > 0 )
          {
            FT_Int  val = *ss;


            प्रति ( ; j > 0; j-- )
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
      भङ्ग;


    पक्ष FT_PIXEL_बहुलकम्_GRAY:
    पक्ष FT_PIXEL_बहुलकम्_LCD:
    पक्ष FT_PIXEL_बहुलकम्_LCD_V:
      {
        FT_UInt  विस्तारः = source->विस्तारः;
        FT_UInt  i;


        target->अङ्कः_grays = 256;

        प्रति ( i = source->पङ्क्तिकाः; i > 0; i-- )
        {
          FT_ARRAY_COPY( t, s, विस्तारः );

          s += source->pitch;
          t += target->pitch;
        }
      }
      भङ्ग;


    पक्ष FT_PIXEL_बहुलकम्_GRAY2:
      {
        FT_UInt  i;


        target->अङ्कः_grays = 4;

        प्रति ( i = source->पङ्क्तिकाः; i > 0; i-- )
        {
          FT_Byte*  ss = s;
          FT_Byte*  tt = t;
          FT_UInt   j;


          /* get the full bytes */
          प्रति ( j = source->विस्तारः >> 2; j > 0; j-- )
          {
            FT_Int  val = ss[0];


            tt[0] = (FT_Byte)( ( val & 0xC0 ) >> 6 );
            tt[1] = (FT_Byte)( ( val & 0x30 ) >> 4 );
            tt[2] = (FT_Byte)( ( val & 0x0C ) >> 2 );
            tt[3] = (FT_Byte)( ( val & 0x03 ) );

            ss += 1;
            tt += 4;
          }

          j = source->विस्तारः & 3;
          यदि ( j > 0 )
          {
            FT_Int  val = ss[0];


            प्रति ( ; j > 0; j-- )
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
      भङ्ग;


    पक्ष FT_PIXEL_बहुलकम्_GRAY4:
      {
        FT_UInt  i;


        target->अङ्कः_grays = 16;

        प्रति ( i = source->पङ्क्तिकाः; i > 0; i-- )
        {
          FT_Byte*  ss = s;
          FT_Byte*  tt = t;
          FT_UInt   j;


          /* get the full bytes */
          प्रति ( j = source->विस्तारः >> 1; j > 0; j-- )
          {
            FT_Int  val = ss[0];


            tt[0] = (FT_Byte)( ( val & 0xF0 ) >> 4 );
            tt[1] = (FT_Byte)( ( val & 0x0F ) );

            ss += 1;
            tt += 2;
          }

          यदि ( source->विस्तारः & 1 )
            tt[0] = (FT_Byte)( ( ss[0] & 0xF0 ) >> 4 );

          s += source->pitch;
          t += target->pitch;
        }
      }
      भङ्ग;


    पक्ष FT_PIXEL_बहुलकम्_BGRA:
      {
        FT_UInt  i;


        target->अङ्कः_grays = 256;

        प्रति ( i = source->पङ्क्तिकाः; i > 0; i-- )
        {
          FT_Byte*  ss = s;
          FT_Byte*  tt = t;
          FT_UInt   j;


          प्रति ( j = source->विस्तारः; j > 0; j-- )
          {
            tt[0] = ft_gray_for_premultiplied_srgb_bgra( ss );

            ss += 4;
            tt += 1;
          }

          s += source->pitch;
          t += target->pitch;
        }
      }
      भङ्ग;

    स्वभाव:
      ;
    }

    प्रत्यागम error;
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Bitmap_Blend( FT_Library        library,
                   ध्रुव FT_Bitmap*  source_,
                   ध्रुव FT_सदिशम्   source_विस्थापनम्,
                   FT_Bitmap*        target,
                   FT_सदिशम्        *atarget_विस्थापनम्,
                   FT_Color          color )
  {
    FT_Error   error = FT_Err_Ok;
    FT_स्मृतिः  स्मृतिः;

    FT_Bitmap         source_bitmap;
    ध्रुव FT_Bitmap*  source;

    FT_Bool  free_source_bitmap          = 0;
    FT_Bool  free_target_bitmap_on_error = 0;

    FT_BBox  sbx, tbx, fbx;

    अचिह्नित पूर्णाङ्क  final_पङ्क्तिकाः, final_विस्तारः;


    यदि ( !library || !target || !source_ || !atarget_विस्थापनम् )
      प्रत्यागम FT_THROW( Invalid_तर्कः );

    स्मृतिः = library->स्मृतिः;

    यदि ( !( target->pixel_बहुलकम् == FT_PIXEL_बहुलकम्_NONE     ||
            ( target->pixel_बहुलकम् == FT_PIXEL_बहुलकम्_BGRA &&
              target->बफर                           ) ) )
      प्रत्यागम FT_THROW( Invalid_तर्कः );

    यदि ( source_->pixel_बहुलकम् == FT_PIXEL_बहुलकम्_NONE )
      प्रत्यागम FT_Err_Ok;               /* nothing to do */

    /* pitches must have the same sign */
    यदि ( target->pixel_बहुलकम् == FT_PIXEL_बहुलकम्_BGRA &&
         ( source_->pitch ^ target->pitch ) < 0   )
      प्रत्यागम FT_THROW( Invalid_तर्कः );

    यदि ( !( source_->विस्तारः && source_->पङ्क्तिकाः ) )
      प्रत्यागम FT_Err_Ok;               /* nothing to do */

    /* get source bitmap dimensions assuming integer offsets */
    sbx.xMin = ( source_विस्थापनम्.x >> 6 );
    sbx.yMin = ( source_विस्थापनम्.y >> 6 ) - source_->पङ्क्तिकाः;
    sbx.xMax = ( source_विस्थापनम्.x >> 6 ) + source_->विस्तारः;
    sbx.yMax = ( source_विस्थापनम्.y >> 6 );

    FT_TRACE5(( "FT_Bitmap_Blend:\n" ));
    FT_TRACE5(( "  source bitmap: (%ld, %ld) -- (%ld, %ld); %u x %u\n",
      sbx.xMin, sbx.yMin, sbx.xMax, sbx.yMax,
      source_->विस्तारः, source_->पङ्क्तिकाः ));

    /* sanity check */
    यदि ( sbx.xMin > sbx.xMax      || sbx.yMin > sbx.yMax     ||
         source_->विस्तारः > 0x7FFFU || source_->पङ्क्तिकाः > 0x7FFFU )
    {
      FT_TRACE5(( "FT_Bitmap_Blend: source dimension overflow\n" ));
      प्रत्यागम FT_THROW( Invalid_तर्कः );
    }

    यदि ( !target->विस्तारः || !target->पङ्क्तिकाः )
    {
      FT_TRACE5(( "  target bitmap: empty\n" ));

      final_विस्तारः = source_->विस्तारः;
      final_पङ्क्तिकाः  = source_->पङ्क्तिकाः;

      /* create new empty bitmap */
      target->विस्तारः      = final_विस्तारः;
      target->पङ्क्तिकाः       = final_पङ्क्तिकाः;
      target->pixel_बहुलकम् = FT_PIXEL_बहुलकम्_BGRA;
      target->pitch      = (पूर्णाङ्क)final_विस्तारः * 4;
      target->अङ्कः_grays  = 256;

      यदि ( FT_ALLOC_MULT( target->बफर, target->पङ्क्तिकाः, target->pitch ) )
        प्रत्यागम error;

      free_target_bitmap_on_error = 1;

      fbx = sbx;
    }
    अन्यथा
    {
      /* get target bitmap dimensions assuming integer offsets */
      tbx.xMin = ( atarget_विस्थापनम्->x >> 6 );
      tbx.yMin = ( atarget_विस्थापनम्->y >> 6 ) - target->पङ्क्तिकाः;
      tbx.xMax = ( atarget_विस्थापनम्->x >> 6 ) + target->विस्तारः;
      tbx.yMax = ( atarget_विस्थापनम्->y >> 6 );

      FT_TRACE5(( "  target bitmap: (%ld, %ld) -- (%ld, %ld); %u x %u\n",
        tbx.xMin, tbx.yMin, tbx.xMax, tbx.yMax,
        target->विस्तारः, target->पङ्क्तिकाः ));

      /* sanity check */
      यदि ( tbx.xMin > tbx.xMax || tbx.yMin > tbx.yMax )
      {
        FT_TRACE5(( "FT_Bitmap_Blend: targget dimension overflow\n" ));
        प्रत्यागम FT_THROW( Invalid_तर्कः );
      }

      /* compute final bitmap dimensions */
      fbx.xMin = FT_MIN( sbx.xMin, tbx.xMin );
      fbx.yMin = FT_MIN( sbx.yMin, tbx.yMin );
      fbx.xMax = FT_MAX( sbx.xMax, tbx.xMax );
      fbx.yMax = FT_MAX( sbx.yMax, tbx.yMax );

      /* sanity check */
      यदि ( fbx.xMin < -0x10000 || fbx.xMax >= 0x10000 ||
           fbx.yMin < -0x10000 || fbx.yMax >= 0x10000 )
      {
        FT_TRACE5(( "FT_Bitmap_Blend: final dimension overflow\n" ));
        प्रत्यागम FT_THROW( Invalid_तर्कः );
      }

      final_विस्तारः = fbx.xMax - fbx.xMin;
      final_पङ्क्तिकाः  = fbx.yMax - fbx.yMin;

      /* adjust target bitmap to enlarged size */
      यदि ( target->विस्तारः < final_विस्तारः ||
           target->पङ्क्तिकाः  < final_पङ्क्तिकाः  )
      {
        पूर्णाङ्क  pitch, new_pitch;

        अचिह्नित वर्ण*  बफर = रिक्त;


        pitch = target->pitch;

        यदि ( pitch < 0 )
          pitch = -pitch;

        new_pitch = (पूर्णाङ्क)final_विस्तारः * 4;

        /* TODO: provide an in-buffer solution for large bitmaps */
        /*       to avoid allocation of a new buffer             */
        यदि ( FT_ALLOC_MULT( बफर, final_पङ्क्तिकाः, new_pitch ) )
          गच्छ Error;

        /* copy data to new buffer */

        /* the bitmap flow is from top to bottom, */
        /* but y is measured from bottom to top   */
        यदि ( target->pitch < 0 )
        {
          /* XXX */
        }
        अन्यथा
        {
          FT_Pos  x = tbx.xMin - fbx.xMin;
          FT_Pos  y = tbx.yMin - fbx.yMin;

          अचिह्नित वर्ण*  p =
            target->बफर;
          अचिह्नित वर्ण*  q =
            बफर +
            ( final_पङ्क्तिकाः - y - target->पङ्क्तिकाः ) * new_pitch +
            x * 4;
          अचिह्नित वर्ण*  सीमा_p =
            p + pitch * (पूर्णाङ्क)target->पङ्क्तिकाः;


          यावत् ( p < सीमा_p )
          {
            FT_MEM_COPY( q, p, pitch );

            p += pitch;
            q += new_pitch;
          }
        }

        FT_FREE( target->बफर );

        target->विस्तारः = final_विस्तारः;
        target->पङ्क्तिकाः  = final_पङ्क्तिकाः;

        यदि ( target->pitch < 0 )
          target->pitch = -new_pitch;
        अन्यथा
          target->pitch = new_pitch;

        target->बफर = बफर;
      }
    }

    FT_TRACE5(( "  final bitmap: (%ld, %ld) -- (%ld, %ld); %u x %u\n",
      fbx.xMin, fbx.yMin, fbx.xMax, fbx.yMax,
      final_विस्तारः, final_पङ्क्तिकाः ));

    /* adjust source bitmap if necessary */
    यदि ( source_->pixel_बहुलकम् != FT_PIXEL_बहुलकम्_GRAY )
    {
      FT_Bitmap_Init( &source_bitmap );
      error = FT_Bitmap_Convert( library, source_, &source_bitmap, 1 );
      यदि ( error )
        गच्छ Error;

      source             = &source_bitmap;
      free_source_bitmap = 1;
    }
    अन्यथा
      source = source_;

    /* do blending; the code below returns pre-multiplied channels, */
    /* similar to what FreeType gets from `CBDT' tables             */

    /* the bitmap flow is from top to bottom, */
    /* but y is measured from bottom to top   */
    यदि ( target->pitch < 0 )
    {
      /* XXX */
    }
    अन्यथा
    {
      FT_Pos  x = sbx.xMin - fbx.xMin;
      FT_Pos  y = sbx.yMin - fbx.yMin;

      अचिह्नित वर्ण*  p =
        source->बफर;
      अचिह्नित वर्ण*  q =
        target->बफर +
        ( target->पङ्क्तिकाः - y - source->पङ्क्तिकाः ) * target->pitch +
        x * 4;
      अचिह्नित वर्ण*  सीमा_p =
        p + source->pitch * (पूर्णाङ्क)source->पङ्क्तिकाः;


      यावत् ( p < सीमा_p )
      {
        अचिह्नित वर्ण*  r       = p;
        अचिह्नित वर्ण*  s       = q;
        अचिह्नित वर्ण*  सीमा_r = r + source->विस्तारः;


        यावत् ( r < सीमा_r )
        {
          पूर्णाङ्क  aa = *r++;
          पूर्णाङ्क  fa = color.alpha * aa / 255;

          पूर्णाङ्क  fb = color.blue * fa / 255;
          पूर्णाङ्क  fg = color.green * fa / 255;
          पूर्णाङ्क  fr = color.red * fa / 255;

          पूर्णाङ्क  ba2 = 255 - fa;

          पूर्णाङ्क  bb = s[0];
          पूर्णाङ्क  bg = s[1];
          पूर्णाङ्क  br = s[2];
          पूर्णाङ्क  ba = s[3];


          *s++ = (अचिह्नित वर्ण)( bb * ba2 / 255 + fb );
          *s++ = (अचिह्नित वर्ण)( bg * ba2 / 255 + fg );
          *s++ = (अचिह्नित वर्ण)( br * ba2 / 255 + fr );
          *s++ = (अचिह्नित वर्ण)( ba * ba2 / 255 + fa );
        }

        p += source->pitch;
        q += target->pitch;
      }
    }

    atarget_विस्थापनम्->x = fbx.xMin * 64;
    atarget_विस्थापनम्->y = fbx.yMax * 64;

  Error:
    यदि ( error && free_target_bitmap_on_error )
      FT_Bitmap_समाप्तम्( library, target );

    यदि ( free_source_bitmap )
      FT_Bitmap_समाप्तम्( library, &source_bitmap );

    प्रत्यागम error;
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_GlyphSlot_Own_Bitmap( FT_GlyphSlot  slot )
  {
    यदि ( slot && slot->format == FT_GLYPH_FORMAT_BITMAP   &&
         !( slot->आन्तरिकम्->flags & FT_GLYPH_OWN_BITMAP ) )
    {
      FT_Bitmap  bitmap;
      FT_Error   error;


      FT_Bitmap_Init( &bitmap );
      error = FT_Bitmap_Copy( slot->library, &slot->bitmap, &bitmap );
      यदि ( error )
        प्रत्यागम error;

      slot->bitmap = bitmap;
      slot->आन्तरिकम्->flags |= FT_GLYPH_OWN_BITMAP;
    }

    प्रत्यागम FT_Err_Ok;
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Bitmap_समाप्तम्( FT_Library  library,
                  FT_Bitmap  *bitmap )
  {
    FT_स्मृतिः  स्मृतिः;


    यदि ( !library )
      प्रत्यागम FT_THROW( Invalid_Library_Handle );

    यदि ( !bitmap )
      प्रत्यागम FT_THROW( Invalid_तर्कः );

    स्मृतिः = library->स्मृतिः;

    FT_FREE( bitmap->बफर );
    *bitmap = null_bitmap;

    प्रत्यागम FT_Err_Ok;
  }


/*अन्तः*/
