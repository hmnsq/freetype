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


#ضمن <freetype/internal/ftdebug.h>

#ضمن <freetype/ftlcdfil.h>
#ضمن <freetype/ftimage.h>
#ضمن <freetype/internal/ftobjs.h>


#جيڪڏهن_وصف_ٿيل FT_CONFIG_OPTION_SUBPIXEL_RENDERING

  /* add padding sufficient for a 5-tap filter, */
  /* which is 2/3 of a pixel                    */
  FT_بنياد_DEF( فراغ )
  ft_lcd_padding( FT_BBox*        cbox,
                  FT_Glyph_جڳهه    جڳهه,
                  FT_Render_گهڻائي  گهڻائي )
  {
    FT_اڻ_استعمال_ٿيل( جڳهه );

    إذا ( گهڻائي == FT_RENDER_گهڻائي_LCD )
    {
      cbox->xMin -= 43;
      cbox->xMax += 43;
    }
    وإلا إذا ( گهڻائي == FT_RENDER_گهڻائي_LCD_V )
    {
      cbox->yMin -= 43;
      cbox->yMax += 43;
    }
  }


  /* documentation in ftlcdfil.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Library_سيٽ_Lcd_Filter_Weights( FT_Library      library,
                                  بنانشان حرف  *weights )
  {
    إذا ( !library )
      أرجع FT_THROW( Invalid_Library_Handle );

    إذا ( !weights )
      أرجع FT_THROW( Invalid_دليل );

    ft_memcpy( library->lcd_weights, weights, FT_LCD_FILTER_FIVE_TAPS );

    أرجع FT_Err_Ok;
  }


  /* documentation in ftlcdfil.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Library_سيٽ_Lcd_Filter( FT_Library    library,
                           FT_LcdFilter  filter )
  {
    ساكن ثابت FT_LcdFiveTapFilter  default_weights =
                   { 0x08, 0x4d, 0x56, 0x4d, 0x08 };
    ساكن ثابت FT_LcdFiveTapFilter  light_weights =
                   { 0x00, 0x55, 0x56, 0x55, 0x00 };


    إذا ( !library )
      أرجع FT_THROW( Invalid_Library_Handle );

    بدل ( filter )
    {
    حالة FT_LCD_FILTER_NONE:
      ft_memset( library->lcd_weights,
                 0,
                 FT_LCD_FILTER_FIVE_TAPS );
      اكسر;

    حالة FT_LCD_FILTER_DEFAULT:
      ft_memcpy( library->lcd_weights,
                 default_weights,
                 FT_LCD_FILTER_FIVE_TAPS );
      اكسر;

    حالة FT_LCD_FILTER_LIGHT:
      ft_memcpy( library->lcd_weights,
                 light_weights,
                 FT_LCD_FILTER_FIVE_TAPS );
      اكسر;

    افتراضي:
      أرجع FT_THROW( Invalid_دليل );
    }

    أرجع FT_Err_Ok;
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Library_سيٽ_Lcd_Geometry( FT_Library  library,
                             FT_ويڪٽر   sub[3] )
  {
    FT_اڻ_استعمال_ٿيل( library );
    FT_اڻ_استعمال_ٿيل( sub );

    أرجع FT_THROW( Unimplemented_Feature );
  }

#وإلا /* !FT_CONFIG_OPTION_SUBPIXEL_RENDERING */

  /* add padding to accommodate outline shifts */
  FT_بنياد_DEF( فراغ )
  ft_lcd_padding( FT_BBox*        cbox,
                  FT_Glyph_جڳهه    جڳهه,
                  FT_Render_گهڻائي  گهڻائي )
  {
    FT_ويڪٽر*  sub = جڳهه->library->lcd_geometry;

    إذا ( گهڻائي == FT_RENDER_گهڻائي_LCD )
    {
      cbox->xMin -= FT_MAX( FT_MAX( sub[0].x, sub[1].x ), sub[2].x );
      cbox->xMax -= FT_MIN( FT_MIN( sub[0].x, sub[1].x ), sub[2].x );
      cbox->yMin -= FT_MAX( FT_MAX( sub[0].y, sub[1].y ), sub[2].y );
      cbox->yMax -= FT_MIN( FT_MIN( sub[0].y, sub[1].y ), sub[2].y );
    }
    وإلا إذا ( گهڻائي == FT_RENDER_گهڻائي_LCD_V )
    {
      cbox->xMin -= FT_MAX( FT_MAX( sub[0].y, sub[1].y ), sub[2].y );
      cbox->xMax -= FT_MIN( FT_MIN( sub[0].y, sub[1].y ), sub[2].y );
      cbox->yMin += FT_MIN( FT_MIN( sub[0].x, sub[1].x ), sub[2].x );
      cbox->yMax += FT_MAX( FT_MAX( sub[0].x, sub[1].x ), sub[2].x );
    }
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Library_سيٽ_Lcd_Filter_Weights( FT_Library      library,
                                  بنانشان حرف  *weights )
  {
    FT_اڻ_استعمال_ٿيل( library );
    FT_اڻ_استعمال_ٿيل( weights );

    أرجع FT_THROW( Unimplemented_Feature );
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Library_سيٽ_Lcd_Filter( FT_Library    library,
                           FT_LcdFilter  filter )
  {
    FT_اڻ_استعمال_ٿيل( library );
    FT_اڻ_استعمال_ٿيل( filter );

    أرجع FT_THROW( Unimplemented_Feature );
  }


  /* documentation in ftlcdfil.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Library_سيٽ_Lcd_Geometry( FT_Library  library,
                             FT_ويڪٽر   sub[3] )
  {
    إذا ( !library )
      أرجع FT_THROW( Invalid_Library_Handle );

    إذا ( !sub )
      أرجع FT_THROW( Invalid_دليل );

    ft_memcpy( library->lcd_geometry, sub, 3 * حجم( FT_ويڪٽر ) );

    أرجع FT_Err_Ok;
  }

#جيڪڏهن_پورو /* !FT_CONFIG_OPTION_SUBPIXEL_RENDERING */


/*پڇاڙي*/
