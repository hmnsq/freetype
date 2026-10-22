/****************************************************************************
 *
 * ftlcdfil.c
 *
 *   FreeType API for color filtering of subpixel bitmap glyphs (body).
 *
 * Copyright (C) 2006-2026 by
 * David Turner, Robert Wilhelm, and Werner Lemberg.
 *
 * This file is part of the FreeType project, and may only be used,
 * modified, and distributed under the terms of the FreeType project
 * license, LICENSE.TXT.  By continuing to use, modify, or distribute
 * this file you indicate that you have read the license and
 * understand and accept it fully.
 *
 */


#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftdebug.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/ftlcdfil.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/ftimage.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftobjs.h>


#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ FT_CONFIG_OPTION_SUBPIXEL_RENDERING

  /* add padding sufficient for a 5-tap filter, */
  /* which is 2/3 of a pixel                    */
  FT_ਅਧਾਰ_DEF( ਖਾਲੀ )
  ft_lcd_padding( FT_BBox*        cbox,
                  FT_Glyph_ਥਾਂ    ਥਾਂ,
                  FT_Render_ਬਹੁਲਕ  ਬਹੁਲਕ )
  {
    FT_ਅਣਵਰਤਿਆ( ਥਾਂ );

    ਜੇ ( ਬਹੁਲਕ == FT_RENDER_ਬਹੁਲਕ_LCD )
    {
      cbox->xMin -= 43;
      cbox->xMax += 43;
    }
    ਨਹੀਂਤਾਂ ਜੇ ( ਬਹੁਲਕ == FT_RENDER_ਬਹੁਲਕ_LCD_V )
    {
      cbox->yMin -= 43;
      cbox->yMax += 43;
    }
  }


  /* documentation in ftlcdfil.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Library_ਸੈੱਟ_Lcd_Filter_Weights( FT_Library      library,
                                  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ  *weights )
  {
    ਜੇ ( !library )
      ਵਾਪਸ FT_THROW( Invalid_Library_Handle );

    ਜੇ ( !weights )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    ft_memcpy( library->lcd_weights, weights, FT_LCD_FILTER_FIVE_TAPS );

    ਵਾਪਸ FT_Err_Ok;
  }


  /* documentation in ftlcdfil.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Library_ਸੈੱਟ_Lcd_Filter( FT_Library    library,
                           FT_LcdFilter  filter )
  {
    ਸਥਿਰ ਅਚਲ FT_LcdFiveTapFilter  default_weights =
                   { 0x08, 0x4d, 0x56, 0x4d, 0x08 };
    ਸਥਿਰ ਅਚਲ FT_LcdFiveTapFilter  light_weights =
                   { 0x00, 0x55, 0x56, 0x55, 0x00 };


    ਜੇ ( !library )
      ਵਾਪਸ FT_THROW( Invalid_Library_Handle );

    ਚੋਣ ( filter )
    {
    ਹਾਲਤ FT_LCD_FILTER_NONE:
      ft_memset( library->lcd_weights,
                 0,
                 FT_LCD_FILTER_FIVE_TAPS );
      ਰੋਕੋ;

    ਹਾਲਤ FT_LCD_FILTER_DEFAULT:
      ft_memcpy( library->lcd_weights,
                 default_weights,
                 FT_LCD_FILTER_FIVE_TAPS );
      ਰੋਕੋ;

    ਹਾਲਤ FT_LCD_FILTER_LIGHT:
      ft_memcpy( library->lcd_weights,
                 light_weights,
                 FT_LCD_FILTER_FIVE_TAPS );
      ਰੋਕੋ;

    ਮੂਲ:
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );
    }

    ਵਾਪਸ FT_Err_Ok;
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Library_ਸੈੱਟ_Lcd_Geometry( FT_Library  library,
                             FT_ਸਦਿਸ਼   sub[3] )
  {
    FT_ਅਣਵਰਤਿਆ( library );
    FT_ਅਣਵਰਤਿਆ( sub );

    ਵਾਪਸ FT_THROW( Unimplemented_Feature );
  }

#ਨਹੀਂਤਾਂ /* !FT_CONFIG_OPTION_SUBPIXEL_RENDERING */

  /* add padding to accommodate outline shifts */
  FT_ਅਧਾਰ_DEF( ਖਾਲੀ )
  ft_lcd_padding( FT_BBox*        cbox,
                  FT_Glyph_ਥਾਂ    ਥਾਂ,
                  FT_Render_ਬਹੁਲਕ  ਬਹੁਲਕ )
  {
    FT_ਸਦਿਸ਼*  sub = ਥਾਂ->library->lcd_geometry;

    ਜੇ ( ਬਹੁਲਕ == FT_RENDER_ਬਹੁਲਕ_LCD )
    {
      cbox->xMin -= FT_MAX( FT_MAX( sub[0].x, sub[1].x ), sub[2].x );
      cbox->xMax -= FT_MIN( FT_MIN( sub[0].x, sub[1].x ), sub[2].x );
      cbox->yMin -= FT_MAX( FT_MAX( sub[0].y, sub[1].y ), sub[2].y );
      cbox->yMax -= FT_MIN( FT_MIN( sub[0].y, sub[1].y ), sub[2].y );
    }
    ਨਹੀਂਤਾਂ ਜੇ ( ਬਹੁਲਕ == FT_RENDER_ਬਹੁਲਕ_LCD_V )
    {
      cbox->xMin -= FT_MAX( FT_MAX( sub[0].y, sub[1].y ), sub[2].y );
      cbox->xMax -= FT_MIN( FT_MIN( sub[0].y, sub[1].y ), sub[2].y );
      cbox->yMin += FT_MIN( FT_MIN( sub[0].x, sub[1].x ), sub[2].x );
      cbox->yMax += FT_MAX( FT_MAX( sub[0].x, sub[1].x ), sub[2].x );
    }
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Library_ਸੈੱਟ_Lcd_Filter_Weights( FT_Library      library,
                                  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ  *weights )
  {
    FT_ਅਣਵਰਤਿਆ( library );
    FT_ਅਣਵਰਤਿਆ( weights );

    ਵਾਪਸ FT_THROW( Unimplemented_Feature );
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Library_ਸੈੱਟ_Lcd_Filter( FT_Library    library,
                           FT_LcdFilter  filter )
  {
    FT_ਅਣਵਰਤਿਆ( library );
    FT_ਅਣਵਰਤਿਆ( filter );

    ਵਾਪਸ FT_THROW( Unimplemented_Feature );
  }


  /* documentation in ftlcdfil.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Library_ਸੈੱਟ_Lcd_Geometry( FT_Library  library,
                             FT_ਸਦਿਸ਼   sub[3] )
  {
    ਜੇ ( !library )
      ਵਾਪਸ FT_THROW( Invalid_Library_Handle );

    ਜੇ ( !sub )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    ft_memcpy( library->lcd_geometry, sub, 3 * ਆਕਾਰ( FT_ਸਦਿਸ਼ ) );

    ਵਾਪਸ FT_Err_Ok;
  }

#ਜੇ_ਅੰਤ /* !FT_CONFIG_OPTION_SUBPIXEL_RENDERING */


/*ਸਿਰਾ*/
