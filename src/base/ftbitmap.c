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


#అన్తర్భావ <freetype/internal/ftdebug.h>

#అన్తర్భావ <freetype/ftbitmap.h>
#అన్తర్భావ <freetype/ftimage.h>
#అన్తర్భావ <freetype/internal/ftobjs.h>


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#నిర్వచనం_రద్దు  FT_COMPONENT
#నిర్వచించు FT_COMPONENT  bitmap


  స్థిర
  స్థిరరాశి FT_Bitmap  null_bitmap = { 0, 0, 0, ఖాళీ, 0, 0, 0, ఖాళీ };


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( శూన్య )
  FT_Bitmap_Init( FT_Bitmap  *abitmap )
  {
    ఒకవేళ ( abitmap )
      *abitmap = null_bitmap;
  }


  /* deprecated function name; retained for ABI compatibility */

  FT_EXPORT_DEF( శూన్య )
  FT_Bitmap_New( FT_Bitmap  *abitmap )
  {
    ఒకవేళ ( abitmap )
      *abitmap = null_bitmap;
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Bitmap_Copy( FT_Library        library,
                  స్థిరరాశి FT_Bitmap  *ఆధారం,
                  FT_Bitmap        *లక్ష్యం)
  {
    FT_జ్ఞాపకం  జ్ఞాపకం;
    FT_Error   error  = FT_Err_Ok;
    FT_Int     pitch;
    FT_Int     flip;


    ఒకవేళ ( !library )
      తిరిగి FT_THROW( Invalid_Library_Handle );

    ఒకవేళ ( !ఆధారం || !లక్ష్యం )
      తిరిగి FT_THROW( Invalid_వాదం );

    ఒకవేళ ( ఆధారం == లక్ష్యం )
      తిరిగి FT_Err_Ok;

    flip = ( ఆధారం->pitch < 0 && లక్ష్యం->pitch > 0 ) ||
           ( ఆధారం->pitch > 0 && లక్ష్యం->pitch < 0 );

    జ్ఞాపకం = library->జ్ఞాపకం;
    FT_FREE( లక్ష్యం->బఫరు );

    *లక్ష్యం = *ఆధారం;

    ఒకవేళ ( flip )
      లక్ష్యం->pitch = -లక్ష్యం->pitch;

    ఒకవేళ ( !ఆధారం->బఫరు )
      తిరిగి FT_Err_Ok;

    pitch  = ఆధారం->pitch;
    ఒకవేళ ( pitch < 0 )
      pitch = -pitch;

    FT_MEM_QALLOC_MULT( లక్ష్యం->బఫరు, లక్ష్యం->అడ్డువరుసలు, pitch );

    ఒకవేళ ( !error )
    {
      ఒకవేళ ( flip )
      {
        /* take care of bitmap flow */
        FT_UInt   i;
        FT_Byte*  s = ఆధారం->బఫరు;
        FT_Byte*  t = లక్ష్యం->బఫరు;


        t += (FT_ULong)pitch * ( లక్ష్యం->అడ్డువరుసలు - 1 );

        కొరకు ( i = లక్ష్యం->అడ్డువరుసలు; i > 0; i-- )
        {
          FT_ARRAY_COPY( t, s, pitch );

          s += pitch;
          t -= pitch;
        }
      }
      లేకపోతే
        FT_MEM_COPY( లక్ష్యం->బఫరు, ఆధారం->బఫరు,
                     (FT_Long)ఆధారం->అడ్డువరుసలు * pitch );
    }

    తిరిగి error;
  }


  /* Enlarge `bitmap' horizontally and vertically by `xpixels' */
  /* and `ypixels', respectively.                              */

  స్థిర FT_Error
  ft_bitmap_assure_బఫరు( FT_జ్ఞాపకం   జ్ఞాపకం,
                           FT_Bitmap*  bitmap,
                           FT_UInt     xpixels,
                           FT_UInt     ypixels )
  {
    FT_Error        error;
    గుర్తులేని పూర్ణాంకం    pitch;
    గుర్తులేని పూర్ణాంకం    new_pitch;
    FT_UInt         bpp;
    FT_UInt         వెడల్పు, ఎత్తు;
    గుర్తులేని అక్షరం*  బఫరు = ఖాళీ;


    వెడల్పు  = bitmap->వెడల్పు;
    ఎత్తు = bitmap->అడ్డువరుసలు;
    pitch  = (గుర్తులేని పూర్ణాంకం)FT_ABS( bitmap->pitch );

    ఎంపిక ( bitmap->pixel_బహుళకం )
    {
    స్థితి FT_PIXEL_బహుళకం_MONO:
      bpp       = 1;
      new_pitch = ( వెడల్పు + xpixels + 7 ) >> 3;
      ఆపు;
    స్థితి FT_PIXEL_బహుళకం_GRAY2:
      bpp       = 2;
      new_pitch = ( వెడల్పు + xpixels + 3 ) >> 2;
      ఆపు;
    స్థితి FT_PIXEL_బహుళకం_GRAY4:
      bpp       = 4;
      new_pitch = ( వెడల్పు + xpixels + 1 ) >> 1;
      ఆపు;
    స్థితి FT_PIXEL_బహుళకం_బూడిద:
    స్థితి FT_PIXEL_బహుళకం_LCD:
    స్థితి FT_PIXEL_బహుళకం_LCD_V:
      bpp       = 8;
      new_pitch = వెడల్పు + xpixels;
      ఆపు;
    అప్రమేయ:
      తిరిగి FT_THROW( Invalid_Glyph_Format );
    }

    /* if no need to allocate memory */
    ఒకవేళ ( ypixels == 0 && new_pitch <= pitch )
    {
      /* zero the padding */
      FT_UInt  బిట్టు_వెడల్పు = pitch * 8;
      FT_UInt  బిట్టు_చివరి  = ( వెడల్పు + xpixels ) * bpp;


      ఒకవేళ ( బిట్టు_చివరి < బిట్టు_వెడల్పు )
      {
        FT_Byte*  పంక్తి  = bitmap->బఫరు + ( బిట్టు_చివరి >> 3 );
        FT_Byte*  చివర   = bitmap->బఫరు + pitch;
        FT_UInt   జరుపు = బిట్టు_చివరి & 7;
        FT_UInt   ముసుగు  = 0xFF00U >> జరుపు;
        FT_UInt   లెక్క = ఎత్తు;


        కొరకు ( ; లెక్క > 0; లెక్క--, పంక్తి += pitch, చివర += pitch )
        {
          FT_Byte*  లిఖ = పంక్తి;


          ఒకవేళ ( జరుపు > 0 )
          {
            లిఖ[0] = (FT_Byte)( లిఖ[0] & ముసుగు );
            లిఖ++;
          }
          ఒకవేళ ( లిఖ < చివర )
            FT_MEM_ZERO( లిఖ, చివర - లిఖ );
        }
      }

      తిరిగి FT_Err_Ok;
    }

    /* otherwise allocate new buffer */
    ఒకవేళ ( FT_QALLOC_MULT( బఫరు, bitmap->అడ్డువరుసలు + ypixels, new_pitch ) )
      తిరిగి error;

    /* new rows get added at the top of the bitmap, */
    /* thus take care of the flow direction         */
    ఒకవేళ ( bitmap->pitch > 0 )
    {
      FT_UInt  len = ( వెడల్పు * bpp + 7 ) >> 3;

      గుర్తులేని అక్షరం*  లోపల  = bitmap->బఫరు;
      గుర్తులేని అక్షరం*  బహిర్గతం = బఫరు;

      గుర్తులేని అక్షరం*  హద్దు = bitmap->బఫరు + pitch * bitmap->అడ్డువరుసలు;
      గుర్తులేని పూర్ణాంకం    delta = new_pitch - len;


      FT_MEM_ZERO( బహిర్గతం, new_pitch * ypixels );
      బహిర్గతం += new_pitch * ypixels;

      అయితే ( లోపల < హద్దు )
      {
        FT_MEM_COPY( బహిర్గతం, లోపల, len );
        లోపల  += pitch;
        బహిర్గతం += len;

        /* we use FT_QALLOC_MULT, which doesn't zero out the buffer;      */
        /* consequently, we have to manually zero out the remaining bytes */
        FT_MEM_ZERO( బహిర్గతం, delta );
        బహిర్గతం += delta;
      }
    }
    లేకపోతే
    {
      FT_UInt  len = ( వెడల్పు * bpp + 7 ) >> 3;

      గుర్తులేని అక్షరం*  లోపల  = bitmap->బఫరు;
      గుర్తులేని అక్షరం*  బహిర్గతం = బఫరు;

      గుర్తులేని అక్షరం*  హద్దు = bitmap->బఫరు + pitch * bitmap->అడ్డువరుసలు;
      గుర్తులేని పూర్ణాంకం    delta = new_pitch - len;


      అయితే ( లోపల < హద్దు )
      {
        FT_MEM_COPY( బహిర్గతం, లోపల, len );
        లోపల  += pitch;
        బహిర్గతం += len;

        FT_MEM_ZERO( బహిర్గతం, delta );
        బహిర్గతం += delta;
      }

      FT_MEM_ZERO( బహిర్గతం, new_pitch * ypixels );
    }

    FT_FREE( bitmap->బఫరు );
    bitmap->బఫరు = బఫరు;

    /* set pitch only, width and height are left untouched */
    ఒకవేళ ( bitmap->pitch < 0 )
      bitmap->pitch = -(పూర్ణాంకం)new_pitch;
    లేకపోతే
      bitmap->pitch = (పూర్ణాంకం)new_pitch;

    తిరిగి FT_Err_Ok;
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Bitmap_Embolden( FT_Library  library,
                      FT_Bitmap*  bitmap,
                      FT_Pos      xStrength,
                      FT_Pos      yStrength )
  {
    FT_Error        error;
    గుర్తులేని అక్షరం*  p;
    FT_Int          i, x, pitch;
    FT_UInt         y;
    FT_Int          xstr, ystr;


    ఒకవేళ ( !library )
      తిరిగి FT_THROW( Invalid_Library_Handle );

    ఒకవేళ ( !bitmap || !bitmap->బఫరు )
      తిరిగి FT_THROW( Invalid_వాదం );

    ఒకవేళ ( ( ( FT_PIX_విడత( xStrength ) >> 6 ) > FT_INT_MAX ) ||
         ( ( FT_PIX_విడత( yStrength ) >> 6 ) > FT_INT_MAX ) )
      తిరిగి FT_THROW( Invalid_వాదం );

    xstr = (FT_Int)FT_PIX_విడత( xStrength ) >> 6;
    ystr = (FT_Int)FT_PIX_విడత( yStrength ) >> 6;

    ఒకవేళ ( xstr == 0 && ystr == 0 )
      తిరిగి FT_Err_Ok;
    లేకపోతే ఒకవేళ ( xstr < 0 || ystr < 0 )
      తిరిగి FT_THROW( Invalid_వాదం );

    ఎంపిక ( bitmap->pixel_బహుళకం )
    {
    స్థితి FT_PIXEL_బహుళకం_GRAY2:
    స్థితి FT_PIXEL_బహుళకం_GRAY4:
      {
        FT_Bitmap  tmp;


        /* convert to 8bpp */
        FT_Bitmap_Init( &tmp );
        error = FT_Bitmap_Convert( library, bitmap, &tmp, 1 );
        ఒకవేళ ( error )
          తిరిగి error;

        FT_Bitmap_పూర్తి( library, bitmap );
        *bitmap = tmp;
      }
      ఆపు;

    స్థితి FT_PIXEL_బహుళకం_MONO:
      ఒకవేళ ( xstr > 8 )
        xstr = 8;
      ఆపు;

    స్థితి FT_PIXEL_బహుళకం_LCD:
      xstr *= 3;
      ఆపు;

    స్థితి FT_PIXEL_బహుళకం_LCD_V:
      ystr *= 3;
      ఆపు;

    స్థితి FT_PIXEL_బహుళకం_BGRA:
      /* We don't embolden color glyphs. */
      తిరిగి FT_Err_Ok;
    }

    error = ft_bitmap_assure_బఫరు( library->జ్ఞాపకం, bitmap,
                                     (FT_UInt)xstr, (FT_UInt)ystr );
    ఒకవేళ ( error )
      తిరిగి error;

    /* take care of bitmap flow */
    pitch = bitmap->pitch;
    ఒకవేళ ( pitch > 0 )
      p = bitmap->బఫరు + pitch * ystr;
    లేకపోతే
    {
      pitch = -pitch;
      p = bitmap->బఫరు + (FT_UInt)pitch * ( bitmap->అడ్డువరుసలు - 1 );
    }

    /* for each row */
    కొరకు ( y = 0; y < bitmap->అడ్డువరుసలు; y++ )
    {
      /*
       * Horizontally:
       *
       * From the last pixel on, make each pixel or'ed with the
       * `xstr' pixels before it.
       */
      కొరకు ( x = pitch - 1; x >= 0; x-- )
      {
        గుర్తులేని అక్షరం  tmp;


        tmp = p[x];
        కొరకు ( i = 1; i <= xstr; i++ )
        {
          ఒకవేళ ( bitmap->pixel_బహుళకం == FT_PIXEL_బహుళకం_MONO )
          {
            p[x] |= tmp >> i;

            /* the maximum value of 8 for `xstr' comes from here */
            ఒకవేళ ( x > 0 )
              p[x] |= p[x - 1] << ( 8 - i );

#ఒకవేళ 0
            ఒకవేళ ( p[x] == 0xFF )
              ఆపు;
#అయితే_ముగింపు
          }
          లేకపోతే
          {
            ఒకవేళ ( x - i >= 0 )
            {
              ఒకవేళ ( p[x] + p[x - i] > bitmap->అంకె_grays - 1 )
              {
                p[x] = (గుర్తులేని అక్షరం)( bitmap->అంకె_grays - 1 );
                ఆపు;
              }
              లేకపోతే
              {
                p[x] = (గుర్తులేని అక్షరం)( p[x] + p[x - i] );
                ఒకవేళ ( p[x] == bitmap->అంకె_grays - 1 )
                  ఆపు;
              }
            }
            లేకపోతే
              ఆపు;
          }
        }
      }

      /*
       * Vertically:
       *
       * Make the above `ystr' rows or'ed with it.
       */
      కొరకు ( x = 1; x <= ystr; x++ )
      {
        గుర్తులేని అక్షరం*  q;


        q = p - bitmap->pitch * x;
        కొరకు ( i = 0; i < pitch; i++ )
          q[i] |= p[i];
      }

      p += bitmap->pitch;
    }

    bitmap->వెడల్పు += (FT_UInt)xstr;
    bitmap->అడ్డువరుసలు += (FT_UInt)ystr;

    తిరిగి FT_Err_Ok;
  }


  స్థిర FT_Byte
  ft_బూడిద_for_premultiplied_srgb_bgra( స్థిరరాశి FT_Byte*  bgra )
  {
    FT_UInt  a = bgra[3];
    FT_UInt  l;


    /* Short-circuit transparent color to avoid division by zero. */
    ఒకవేళ ( !a )
      తిరిగి 0;

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

    తిరిగి (FT_Byte)( a - l / a );
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Bitmap_Convert( FT_Library        library,
                     స్థిరరాశి FT_Bitmap  *ఆధారం,
                     FT_Bitmap        *లక్ష్యం,
                     FT_Int            alignment )
  {
    FT_Error   error = FT_Err_Ok;
    FT_జ్ఞాపకం  జ్ఞాపకం;

    FT_Byte*  s;
    FT_Byte*  t;


    ఒకవేళ ( !library )
      తిరిగి FT_THROW( Invalid_Library_Handle );

    ఒకవేళ ( !ఆధారం || !లక్ష్యం )
      తిరిగి FT_THROW( Invalid_వాదం );

    జ్ఞాపకం = library->జ్ఞాపకం;

    ఎంపిక ( ఆధారం->pixel_బహుళకం )
    {
    స్థితి FT_PIXEL_బహుళకం_MONO:
    స్థితి FT_PIXEL_బహుళకం_బూడిద:
    స్థితి FT_PIXEL_బహుళకం_GRAY2:
    స్థితి FT_PIXEL_బహుళకం_GRAY4:
    స్థితి FT_PIXEL_బహుళకం_LCD:
    స్థితి FT_PIXEL_బహుళకం_LCD_V:
    స్థితి FT_PIXEL_బహుళకం_BGRA:
      {
        FT_Int  వెడల్పు = (FT_Int)ఆధారం->వెడల్పు;
        FT_Int  neg   = ( లక్ష్యం->pitch == 0 && ఆధారం->pitch < 0 ) ||
                          లక్ష్యం->pitch  < 0;


        FT_Bitmap_పూర్తి( library, లక్ష్యం );

        లక్ష్యం->pixel_బహుళకం = FT_PIXEL_బహుళకం_బూడిద;
        లక్ష్యం->అడ్డువరుసలు       = ఆధారం->అడ్డువరుసలు;
        లక్ష్యం->వెడల్పు      = ఆధారం->వెడల్పు;

        ఒకవేళ ( alignment )
        {
          FT_Int  rem = వెడల్పు % alignment;


          ఒకవేళ ( rem )
            వెడల్పు = alignment > 0 ? వెడల్పు - rem + alignment
                                  : వెడల్పు - rem - alignment;
        }

        ఒకవేళ ( FT_QALLOC_MULT( లక్ష్యం->బఫరు, లక్ష్యం->అడ్డువరుసలు, వెడల్పు ) )
          తిరిగి error;

        లక్ష్యం->pitch = neg ? -వెడల్పు : వెడల్పు;
      }
      ఆపు;

    అప్రమేయ:
      error = FT_THROW( Invalid_వాదం );
    }

    s = ఆధారం->బఫరు;
    t = లక్ష్యం->బఫరు;

    /* take care of bitmap flow */
    ఒకవేళ ( ఆధారం->pitch < 0 )
      s -= ఆధారం->pitch * (FT_Int)( ఆధారం->అడ్డువరుసలు - 1 );
    ఒకవేళ ( లక్ష్యం->pitch < 0 )
      t -= లక్ష్యం->pitch * (FT_Int)( లక్ష్యం->అడ్డువరుసలు - 1 );

    ఎంపిక ( ఆధారం->pixel_బహుళకం )
    {
    స్థితి FT_PIXEL_బహుళకం_MONO:
      {
        FT_UInt  i;


        లక్ష్యం->అంకె_grays = 2;

        కొరకు ( i = ఆధారం->అడ్డువరుసలు; i > 0; i-- )
        {
          FT_Byte*  ss = s;
          FT_Byte*  tt = t;
          FT_UInt   j;


          /* get the full bytes */
          కొరకు ( j = ఆధారం->వెడల్పు >> 3; j > 0; j-- )
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
          j = ఆధారం->వెడల్పు & 7;
          ఒకవేళ ( j > 0 )
          {
            FT_Int  val = *ss;


            కొరకు ( ; j > 0; j-- )
            {
              tt[0] = (FT_Byte)( ( val & 0x80 ) >> 7);
              val <<= 1;
              tt   += 1;
            }
          }

          s += ఆధారం->pitch;
          t += లక్ష్యం->pitch;
        }
      }
      ఆపు;


    స్థితి FT_PIXEL_బహుళకం_బూడిద:
    స్థితి FT_PIXEL_బహుళకం_LCD:
    స్థితి FT_PIXEL_బహుళకం_LCD_V:
      {
        FT_UInt  వెడల్పు = ఆధారం->వెడల్పు;
        FT_UInt  i;


        లక్ష్యం->అంకె_grays = 256;

        కొరకు ( i = ఆధారం->అడ్డువరుసలు; i > 0; i-- )
        {
          FT_ARRAY_COPY( t, s, వెడల్పు );

          s += ఆధారం->pitch;
          t += లక్ష్యం->pitch;
        }
      }
      ఆపు;


    స్థితి FT_PIXEL_బహుళకం_GRAY2:
      {
        FT_UInt  i;


        లక్ష్యం->అంకె_grays = 4;

        కొరకు ( i = ఆధారం->అడ్డువరుసలు; i > 0; i-- )
        {
          FT_Byte*  ss = s;
          FT_Byte*  tt = t;
          FT_UInt   j;


          /* get the full bytes */
          కొరకు ( j = ఆధారం->వెడల్పు >> 2; j > 0; j-- )
          {
            FT_Int  val = ss[0];


            tt[0] = (FT_Byte)( ( val & 0xC0 ) >> 6 );
            tt[1] = (FT_Byte)( ( val & 0x30 ) >> 4 );
            tt[2] = (FT_Byte)( ( val & 0x0C ) >> 2 );
            tt[3] = (FT_Byte)( ( val & 0x03 ) );

            ss += 1;
            tt += 4;
          }

          j = ఆధారం->వెడల్పు & 3;
          ఒకవేళ ( j > 0 )
          {
            FT_Int  val = ss[0];


            కొరకు ( ; j > 0; j-- )
            {
              tt[0]  = (FT_Byte)( ( val & 0xC0 ) >> 6 );
              val  <<= 2;
              tt    += 1;
            }
          }

          s += ఆధారం->pitch;
          t += లక్ష్యం->pitch;
        }
      }
      ఆపు;


    స్థితి FT_PIXEL_బహుళకం_GRAY4:
      {
        FT_UInt  i;


        లక్ష్యం->అంకె_grays = 16;

        కొరకు ( i = ఆధారం->అడ్డువరుసలు; i > 0; i-- )
        {
          FT_Byte*  ss = s;
          FT_Byte*  tt = t;
          FT_UInt   j;


          /* get the full bytes */
          కొరకు ( j = ఆధారం->వెడల్పు >> 1; j > 0; j-- )
          {
            FT_Int  val = ss[0];


            tt[0] = (FT_Byte)( ( val & 0xF0 ) >> 4 );
            tt[1] = (FT_Byte)( ( val & 0x0F ) );

            ss += 1;
            tt += 2;
          }

          ఒకవేళ ( ఆధారం->వెడల్పు & 1 )
            tt[0] = (FT_Byte)( ( ss[0] & 0xF0 ) >> 4 );

          s += ఆధారం->pitch;
          t += లక్ష్యం->pitch;
        }
      }
      ఆపు;


    స్థితి FT_PIXEL_బహుళకం_BGRA:
      {
        FT_UInt  i;


        లక్ష్యం->అంకె_grays = 256;

        కొరకు ( i = ఆధారం->అడ్డువరుసలు; i > 0; i-- )
        {
          FT_Byte*  ss = s;
          FT_Byte*  tt = t;
          FT_UInt   j;


          కొరకు ( j = ఆధారం->వెడల్పు; j > 0; j-- )
          {
            tt[0] = ft_బూడిద_for_premultiplied_srgb_bgra( ss );

            ss += 4;
            tt += 1;
          }

          s += ఆధారం->pitch;
          t += లక్ష్యం->pitch;
        }
      }
      ఆపు;

    అప్రమేయ:
      ;
    }

    తిరిగి error;
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Bitmap_Blend( FT_Library        library,
                   స్థిరరాశి FT_Bitmap*  ఆధారం_2,
                   స్థిరరాశి FT_సదిశ   ఆధారం_స్థానభ్రంశం,
                   FT_Bitmap*        లక్ష్యం,
                   FT_సదిశ        *atarget_స్థానభ్రంశం,
                   FT_Color          color )
  {
    FT_Error   error = FT_Err_Ok;
    FT_జ్ఞాపకం  జ్ఞాపకం;

    FT_Bitmap         ఆధారం_bitmap;
    స్థిరరాశి FT_Bitmap*  ఆధారం;

    FT_Bool  free_ఆధారం_bitmap          = 0;
    FT_Bool  free_లక్ష్యం_bitmap_on_error = 0;

    FT_BBox  sbx, tbx, fbx;

    గుర్తులేని పూర్ణాంకం  final_అడ్డువరుసలు, final_వెడల్పు;


    ఒకవేళ ( !library || !లక్ష్యం || !ఆధారం_2 || !atarget_స్థానభ్రంశం )
      తిరిగి FT_THROW( Invalid_వాదం );

    జ్ఞాపకం = library->జ్ఞాపకం;

    ఒకవేళ ( !( లక్ష్యం->pixel_బహుళకం == FT_PIXEL_బహుళకం_NONE     ||
            ( లక్ష్యం->pixel_బహుళకం == FT_PIXEL_బహుళకం_BGRA &&
              లక్ష్యం->బఫరు                           ) ) )
      తిరిగి FT_THROW( Invalid_వాదం );

    ఒకవేళ ( ఆధారం_2->pixel_బహుళకం == FT_PIXEL_బహుళకం_NONE )
      తిరిగి FT_Err_Ok;               /* nothing to do */

    /* pitches must have the same sign */
    ఒకవేళ ( లక్ష్యం->pixel_బహుళకం == FT_PIXEL_బహుళకం_BGRA &&
         ( ఆధారం_2->pitch ^ లక్ష్యం->pitch ) < 0   )
      తిరిగి FT_THROW( Invalid_వాదం );

    ఒకవేళ ( !( ఆధారం_2->వెడల్పు && ఆధారం_2->అడ్డువరుసలు ) )
      తిరిగి FT_Err_Ok;               /* nothing to do */

    /* get source bitmap dimensions assuming integer offsets */
    sbx.xMin = ( ఆధారం_స్థానభ్రంశం.x >> 6 );
    sbx.yMin = ( ఆధారం_స్థానభ్రంశం.y >> 6 ) - ఆధారం_2->అడ్డువరుసలు;
    sbx.xMax = ( ఆధారం_స్థానభ్రంశం.x >> 6 ) + ఆధారం_2->వెడల్పు;
    sbx.yMax = ( ఆధారం_స్థానభ్రంశం.y >> 6 );

    FT_TRACE5(( "FT_Bitmap_Blend:\n" ));
    FT_TRACE5(( "  source bitmap: (%ld, %ld) -- (%ld, %ld); %u x %u\n",
      sbx.xMin, sbx.yMin, sbx.xMax, sbx.yMax,
      ఆధారం_2->వెడల్పు, ఆధారం_2->అడ్డువరుసలు ));

    /* sanity check */
    ఒకవేళ ( sbx.xMin > sbx.xMax      || sbx.yMin > sbx.yMax     ||
         ఆధారం_2->వెడల్పు > 0x7FFFU || ఆధారం_2->అడ్డువరుసలు > 0x7FFFU )
    {
      FT_TRACE5(( "FT_Bitmap_Blend: source dimension overflow\n" ));
      తిరిగి FT_THROW( Invalid_వాదం );
    }

    ఒకవేళ ( !లక్ష్యం->వెడల్పు || !లక్ష్యం->అడ్డువరుసలు )
    {
      FT_TRACE5(( "  target bitmap: empty\n" ));

      final_వెడల్పు = ఆధారం_2->వెడల్పు;
      final_అడ్డువరుసలు  = ఆధారం_2->అడ్డువరుసలు;

      /* create new empty bitmap */
      లక్ష్యం->వెడల్పు      = final_వెడల్పు;
      లక్ష్యం->అడ్డువరుసలు       = final_అడ్డువరుసలు;
      లక్ష్యం->pixel_బహుళకం = FT_PIXEL_బహుళకం_BGRA;
      లక్ష్యం->pitch      = (పూర్ణాంకం)final_వెడల్పు * 4;
      లక్ష్యం->అంకె_grays  = 256;

      ఒకవేళ ( FT_ALLOC_MULT( లక్ష్యం->బఫరు, లక్ష్యం->అడ్డువరుసలు, లక్ష్యం->pitch ) )
        తిరిగి error;

      free_లక్ష్యం_bitmap_on_error = 1;

      fbx = sbx;
    }
    లేకపోతే
    {
      /* get target bitmap dimensions assuming integer offsets */
      tbx.xMin = ( atarget_స్థానభ్రంశం->x >> 6 );
      tbx.yMin = ( atarget_స్థానభ్రంశం->y >> 6 ) - లక్ష్యం->అడ్డువరుసలు;
      tbx.xMax = ( atarget_స్థానభ్రంశం->x >> 6 ) + లక్ష్యం->వెడల్పు;
      tbx.yMax = ( atarget_స్థానభ్రంశం->y >> 6 );

      FT_TRACE5(( "  target bitmap: (%ld, %ld) -- (%ld, %ld); %u x %u\n",
        tbx.xMin, tbx.yMin, tbx.xMax, tbx.yMax,
        లక్ష్యం->వెడల్పు, లక్ష్యం->అడ్డువరుసలు ));

      /* sanity check */
      ఒకవేళ ( tbx.xMin > tbx.xMax || tbx.yMin > tbx.yMax )
      {
        FT_TRACE5(( "FT_Bitmap_Blend: targget dimension overflow\n" ));
        తిరిగి FT_THROW( Invalid_వాదం );
      }

      /* compute final bitmap dimensions */
      fbx.xMin = FT_MIN( sbx.xMin, tbx.xMin );
      fbx.yMin = FT_MIN( sbx.yMin, tbx.yMin );
      fbx.xMax = FT_MAX( sbx.xMax, tbx.xMax );
      fbx.yMax = FT_MAX( sbx.yMax, tbx.yMax );

      /* sanity check */
      ఒకవేళ ( fbx.xMin < -0x10000 || fbx.xMax >= 0x10000 ||
           fbx.yMin < -0x10000 || fbx.yMax >= 0x10000 )
      {
        FT_TRACE5(( "FT_Bitmap_Blend: final dimension overflow\n" ));
        తిరిగి FT_THROW( Invalid_వాదం );
      }

      final_వెడల్పు = fbx.xMax - fbx.xMin;
      final_అడ్డువరుసలు  = fbx.yMax - fbx.yMin;

      /* adjust target bitmap to enlarged size */
      ఒకవేళ ( లక్ష్యం->వెడల్పు < final_వెడల్పు ||
           లక్ష్యం->అడ్డువరుసలు  < final_అడ్డువరుసలు  )
      {
        పూర్ణాంకం  pitch, new_pitch;

        గుర్తులేని అక్షరం*  బఫరు = ఖాళీ;


        pitch = లక్ష్యం->pitch;

        ఒకవేళ ( pitch < 0 )
          pitch = -pitch;

        new_pitch = (పూర్ణాంకం)final_వెడల్పు * 4;

        /* TODO: provide an in-buffer solution for large bitmaps */
        /*       to avoid allocation of a new buffer             */
        ఒకవేళ ( FT_ALLOC_MULT( బఫరు, final_అడ్డువరుసలు, new_pitch ) )
          వెళ్ళు Error;

        /* copy data to new buffer */

        /* the bitmap flow is from top to bottom, */
        /* but y is measured from bottom to top   */
        ఒకవేళ ( లక్ష్యం->pitch < 0 )
        {
          /* XXX */
        }
        లేకపోతే
        {
          FT_Pos  x = tbx.xMin - fbx.xMin;
          FT_Pos  y = tbx.yMin - fbx.yMin;

          గుర్తులేని అక్షరం*  p =
            లక్ష్యం->బఫరు;
          గుర్తులేని అక్షరం*  q =
            బఫరు +
            ( final_అడ్డువరుసలు - y - లక్ష్యం->అడ్డువరుసలు ) * new_pitch +
            x * 4;
          గుర్తులేని అక్షరం*  హద్దు_p =
            p + pitch * (పూర్ణాంకం)లక్ష్యం->అడ్డువరుసలు;


          అయితే ( p < హద్దు_p )
          {
            FT_MEM_COPY( q, p, pitch );

            p += pitch;
            q += new_pitch;
          }
        }

        FT_FREE( లక్ష్యం->బఫరు );

        లక్ష్యం->వెడల్పు = final_వెడల్పు;
        లక్ష్యం->అడ్డువరుసలు  = final_అడ్డువరుసలు;

        ఒకవేళ ( లక్ష్యం->pitch < 0 )
          లక్ష్యం->pitch = -new_pitch;
        లేకపోతే
          లక్ష్యం->pitch = new_pitch;

        లక్ష్యం->బఫరు = బఫరు;
      }
    }

    FT_TRACE5(( "  final bitmap: (%ld, %ld) -- (%ld, %ld); %u x %u\n",
      fbx.xMin, fbx.yMin, fbx.xMax, fbx.yMax,
      final_వెడల్పు, final_అడ్డువరుసలు ));

    /* adjust source bitmap if necessary */
    ఒకవేళ ( ఆధారం_2->pixel_బహుళకం != FT_PIXEL_బహుళకం_బూడిద )
    {
      FT_Bitmap_Init( &ఆధారం_bitmap );
      error = FT_Bitmap_Convert( library, ఆధారం_2, &ఆధారం_bitmap, 1 );
      ఒకవేళ ( error )
        వెళ్ళు Error;

      ఆధారం             = &ఆధారం_bitmap;
      free_ఆధారం_bitmap = 1;
    }
    లేకపోతే
      ఆధారం = ఆధారం_2;

    /* do blending; the code below returns pre-multiplied channels, */
    /* similar to what FreeType gets from `CBDT' tables             */

    /* the bitmap flow is from top to bottom, */
    /* but y is measured from bottom to top   */
    ఒకవేళ ( లక్ష్యం->pitch < 0 )
    {
      /* XXX */
    }
    లేకపోతే
    {
      FT_Pos  x = sbx.xMin - fbx.xMin;
      FT_Pos  y = sbx.yMin - fbx.yMin;

      గుర్తులేని అక్షరం*  p =
        ఆధారం->బఫరు;
      గుర్తులేని అక్షరం*  q =
        లక్ష్యం->బఫరు +
        ( లక్ష్యం->అడ్డువరుసలు - y - ఆధారం->అడ్డువరుసలు ) * లక్ష్యం->pitch +
        x * 4;
      గుర్తులేని అక్షరం*  హద్దు_p =
        p + ఆధారం->pitch * (పూర్ణాంకం)ఆధారం->అడ్డువరుసలు;


      అయితే ( p < హద్దు_p )
      {
        గుర్తులేని అక్షరం*  r       = p;
        గుర్తులేని అక్షరం*  s       = q;
        గుర్తులేని అక్షరం*  హద్దు_r = r + ఆధారం->వెడల్పు;


        అయితే ( r < హద్దు_r )
        {
          పూర్ణాంకం  aa = *r++;
          పూర్ణాంకం  fa = color.alpha * aa / 255;

          పూర్ణాంకం  fb = color.blue * fa / 255;
          పూర్ణాంకం  fg = color.green * fa / 255;
          పూర్ణాంకం  fr = color.red * fa / 255;

          పూర్ణాంకం  ba2 = 255 - fa;

          పూర్ణాంకం  bb = s[0];
          పూర్ణాంకం  bg = s[1];
          పూర్ణాంకం  br = s[2];
          పూర్ణాంకం  ba = s[3];


          *s++ = (గుర్తులేని అక్షరం)( bb * ba2 / 255 + fb );
          *s++ = (గుర్తులేని అక్షరం)( bg * ba2 / 255 + fg );
          *s++ = (గుర్తులేని అక్షరం)( br * ba2 / 255 + fr );
          *s++ = (గుర్తులేని అక్షరం)( ba * ba2 / 255 + fa );
        }

        p += ఆధారం->pitch;
        q += లక్ష్యం->pitch;
      }
    }

    atarget_స్థానభ్రంశం->x = fbx.xMin * 64;
    atarget_స్థానభ్రంశం->y = fbx.yMax * 64;

  Error:
    ఒకవేళ ( error && free_లక్ష్యం_bitmap_on_error )
      FT_Bitmap_పూర్తి( library, లక్ష్యం );

    ఒకవేళ ( free_ఆధారం_bitmap )
      FT_Bitmap_పూర్తి( library, &ఆధారం_bitmap );

    తిరిగి error;
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Glyph_స్థానం_Own_Bitmap( FT_Glyph_స్థానం  స్థానం )
  {
    ఒకవేళ ( స్థానం && స్థానం->format == FT_GLYPH_FORMAT_BITMAP   &&
         !( స్థానం->అంతర్గత->flags & FT_GLYPH_OWN_BITMAP ) )
    {
      FT_Bitmap  bitmap;
      FT_Error   error;


      FT_Bitmap_Init( &bitmap );
      error = FT_Bitmap_Copy( స్థానం->library, &స్థానం->bitmap, &bitmap );
      ఒకవేళ ( error )
        తిరిగి error;

      స్థానం->bitmap = bitmap;
      స్థానం->అంతర్గత->flags |= FT_GLYPH_OWN_BITMAP;
    }

    తిరిగి FT_Err_Ok;
  }


  /* documentation is in ftbitmap.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Bitmap_పూర్తి( FT_Library  library,
                  FT_Bitmap  *bitmap )
  {
    FT_జ్ఞాపకం  జ్ఞాపకం;


    ఒకవేళ ( !library )
      తిరిగి FT_THROW( Invalid_Library_Handle );

    ఒకవేళ ( !bitmap )
      తిరిగి FT_THROW( Invalid_వాదం );

    జ్ఞాపకం = library->జ్ఞాపకం;

    FT_FREE( bitmap->బఫరు );
    *bitmap = null_bitmap;

    తిరిగి FT_Err_Ok;
  }


/*చివర*/
