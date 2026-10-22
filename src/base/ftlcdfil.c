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


#شامل <freetype/internal/ftdebug.h>

#شامل <freetype/ftlcdfil.h>
#شامل <freetype/ftimage.h>
#شامل <freetype/internal/ftobjs.h>


#اگر_تعریف_شدہ FT_CONFIG_OPTION_SUBPIXEL_RENDERING

  /* add padding sufficient for a 5-tap filter, */
  /* which is 2/3 of a pixel                    */
  FT_بنیاد_DEF( خالی )
  ft_lcd_padding( FT_BBox*        cbox,
                  FT_Glyph_خانہ    خانہ,
                  FT_Render_منوال  منوال )
  {
    FT_غیر_مستعمل( خانہ );

    اگر ( منوال == FT_RENDER_منوال_LCD )
    {
      cbox->xMin -= 43;
      cbox->xMax += 43;
    }
    ورنہ اگر ( منوال == FT_RENDER_منوال_LCD_V )
    {
      cbox->yMin -= 43;
      cbox->yMax += 43;
    }
  }


  /* documentation in ftlcdfil.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Library_سیٹ_Lcd_Filter_Weights( FT_Library      library,
                                  غیرعلامتی حرف  *weights )
  {
    اگر ( !library )
      واپس FT_THROW( Invalid_Library_Handle );

    اگر ( !weights )
      واپس FT_THROW( Invalid_دلیل );

    ft_memcpy( library->lcd_weights, weights, FT_LCD_FILTER_FIVE_TAPS );

    واپس FT_Err_Ok;
  }


  /* documentation in ftlcdfil.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Library_سیٹ_Lcd_Filter( FT_Library    library,
                           FT_LcdFilter  filter )
  {
    ساکن ثابت FT_LcdFiveTapFilter  default_weights =
                   { 0x08, 0x4d, 0x56, 0x4d, 0x08 };
    ساکن ثابت FT_LcdFiveTapFilter  light_weights =
                   { 0x00, 0x55, 0x56, 0x55, 0x00 };


    اگر ( !library )
      واپس FT_THROW( Invalid_Library_Handle );

    انتخاب ( filter )
    {
    حالت FT_LCD_FILTER_NONE:
      ft_memset( library->lcd_weights,
                 0,
                 FT_LCD_FILTER_FIVE_TAPS );
      توڑو;

    حالت FT_LCD_FILTER_DEFAULT:
      ft_memcpy( library->lcd_weights,
                 default_weights,
                 FT_LCD_FILTER_FIVE_TAPS );
      توڑو;

    حالت FT_LCD_FILTER_LIGHT:
      ft_memcpy( library->lcd_weights,
                 light_weights,
                 FT_LCD_FILTER_FIVE_TAPS );
      توڑو;

    طےشدہ:
      واپس FT_THROW( Invalid_دلیل );
    }

    واپس FT_Err_Ok;
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Library_سیٹ_Lcd_Geometry( FT_Library  library,
                             FT_سمتیہ   sub[3] )
  {
    FT_غیر_مستعمل( library );
    FT_غیر_مستعمل( sub );

    واپس FT_THROW( Unimplemented_Feature );
  }

#ورنہ /* !FT_CONFIG_OPTION_SUBPIXEL_RENDERING */

  /* add padding to accommodate outline shifts */
  FT_بنیاد_DEF( خالی )
  ft_lcd_padding( FT_BBox*        cbox,
                  FT_Glyph_خانہ    خانہ,
                  FT_Render_منوال  منوال )
  {
    FT_سمتیہ*  sub = خانہ->library->lcd_geometry;

    اگر ( منوال == FT_RENDER_منوال_LCD )
    {
      cbox->xMin -= FT_MAX( FT_MAX( sub[0].x, sub[1].x ), sub[2].x );
      cbox->xMax -= FT_MIN( FT_MIN( sub[0].x, sub[1].x ), sub[2].x );
      cbox->yMin -= FT_MAX( FT_MAX( sub[0].y, sub[1].y ), sub[2].y );
      cbox->yMax -= FT_MIN( FT_MIN( sub[0].y, sub[1].y ), sub[2].y );
    }
    ورنہ اگر ( منوال == FT_RENDER_منوال_LCD_V )
    {
      cbox->xMin -= FT_MAX( FT_MAX( sub[0].y, sub[1].y ), sub[2].y );
      cbox->xMax -= FT_MIN( FT_MIN( sub[0].y, sub[1].y ), sub[2].y );
      cbox->yMin += FT_MIN( FT_MIN( sub[0].x, sub[1].x ), sub[2].x );
      cbox->yMax += FT_MAX( FT_MAX( sub[0].x, sub[1].x ), sub[2].x );
    }
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Library_سیٹ_Lcd_Filter_Weights( FT_Library      library,
                                  غیرعلامتی حرف  *weights )
  {
    FT_غیر_مستعمل( library );
    FT_غیر_مستعمل( weights );

    واپس FT_THROW( Unimplemented_Feature );
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Library_سیٹ_Lcd_Filter( FT_Library    library,
                           FT_LcdFilter  filter )
  {
    FT_غیر_مستعمل( library );
    FT_غیر_مستعمل( filter );

    واپس FT_THROW( Unimplemented_Feature );
  }


  /* documentation in ftlcdfil.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Library_سیٹ_Lcd_Geometry( FT_Library  library,
                             FT_سمتیہ   sub[3] )
  {
    اگر ( !library )
      واپس FT_THROW( Invalid_Library_Handle );

    اگر ( !sub )
      واپس FT_THROW( Invalid_دلیل );

    ft_memcpy( library->lcd_geometry, sub, 3 * حجم( FT_سمتیہ ) );

    واپس FT_Err_Ok;
  }

#اگر_ختم /* !FT_CONFIG_OPTION_SUBPIXEL_RENDERING */


/*انتہا*/
