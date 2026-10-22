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


#अन्तर्भाव <freetype/internal/ftdebug.h>

#अन्तर्भाव <freetype/ftlcdfil.h>
#अन्तर्भाव <freetype/ftimage.h>
#अन्तर्भाव <freetype/internal/ftobjs.h>


#यदि_परिभाषितम् FT_CONFIG_OPTION_SUBPIXEL_RENDERING

  /* add padding sufficient for a 5-tap filter, */
  /* which is 2/3 of a pixel                    */
  FT_आधारः_DEF( शून्य )
  ft_lcd_padding( FT_BBox*        cbox,
                  FT_GlyphSlot    slot,
                  FT_Render_बहुलकम्  बहुलकम् )
  {
    FT_अप्रयुक्तम्( slot );

    यदि ( बहुलकम् == FT_RENDER_बहुलकम्_LCD )
    {
      cbox->xMin -= 43;
      cbox->xMax += 43;
    }
    अन्यथा यदि ( बहुलकम् == FT_RENDER_बहुलकम्_LCD_V )
    {
      cbox->yMin -= 43;
      cbox->yMax += 43;
    }
  }


  /* documentation in ftlcdfil.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Library_समुच्चयः_Lcd_Filter_Weights( FT_Library      library,
                                  अचिह्नित वर्ण  *weights )
  {
    यदि ( !library )
      प्रत्यागम FT_THROW( Invalid_Library_Handle );

    यदि ( !weights )
      प्रत्यागम FT_THROW( Invalid_तर्कः );

    ft_memcpy( library->lcd_weights, weights, FT_LCD_FILTER_FIVE_TAPS );

    प्रत्यागम FT_Err_Ok;
  }


  /* documentation in ftlcdfil.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Library_समुच्चयः_Lcd_Filter( FT_Library    library,
                           FT_LcdFilter  filter )
  {
    स्थिर ध्रुव FT_LcdFiveTapFilter  default_weights =
                   { 0x08, 0x4d, 0x56, 0x4d, 0x08 };
    स्थिर ध्रुव FT_LcdFiveTapFilter  light_weights =
                   { 0x00, 0x55, 0x56, 0x55, 0x00 };


    यदि ( !library )
      प्रत्यागम FT_THROW( Invalid_Library_Handle );

    विकल्प ( filter )
    {
    पक्ष FT_LCD_FILTER_NONE:
      ft_memset( library->lcd_weights,
                 0,
                 FT_LCD_FILTER_FIVE_TAPS );
      भङ्ग;

    पक्ष FT_LCD_FILTER_DEFAULT:
      ft_memcpy( library->lcd_weights,
                 default_weights,
                 FT_LCD_FILTER_FIVE_TAPS );
      भङ्ग;

    पक्ष FT_LCD_FILTER_LIGHT:
      ft_memcpy( library->lcd_weights,
                 light_weights,
                 FT_LCD_FILTER_FIVE_TAPS );
      भङ्ग;

    स्वभाव:
      प्रत्यागम FT_THROW( Invalid_तर्कः );
    }

    प्रत्यागम FT_Err_Ok;
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Library_समुच्चयः_Lcd_Geometry( FT_Library  library,
                             FT_सदिशम्   sub[3] )
  {
    FT_अप्रयुक्तम्( library );
    FT_अप्रयुक्तम्( sub );

    प्रत्यागम FT_THROW( Unimplemented_Feature );
  }

#अन्यथा /* !FT_CONFIG_OPTION_SUBPIXEL_RENDERING */

  /* add padding to accommodate outline shifts */
  FT_आधारः_DEF( शून्य )
  ft_lcd_padding( FT_BBox*        cbox,
                  FT_GlyphSlot    slot,
                  FT_Render_बहुलकम्  बहुलकम् )
  {
    FT_सदिशम्*  sub = slot->library->lcd_geometry;

    यदि ( बहुलकम् == FT_RENDER_बहुलकम्_LCD )
    {
      cbox->xMin -= FT_MAX( FT_MAX( sub[0].x, sub[1].x ), sub[2].x );
      cbox->xMax -= FT_MIN( FT_MIN( sub[0].x, sub[1].x ), sub[2].x );
      cbox->yMin -= FT_MAX( FT_MAX( sub[0].y, sub[1].y ), sub[2].y );
      cbox->yMax -= FT_MIN( FT_MIN( sub[0].y, sub[1].y ), sub[2].y );
    }
    अन्यथा यदि ( बहुलकम् == FT_RENDER_बहुलकम्_LCD_V )
    {
      cbox->xMin -= FT_MAX( FT_MAX( sub[0].y, sub[1].y ), sub[2].y );
      cbox->xMax -= FT_MIN( FT_MIN( sub[0].y, sub[1].y ), sub[2].y );
      cbox->yMin += FT_MIN( FT_MIN( sub[0].x, sub[1].x ), sub[2].x );
      cbox->yMax += FT_MAX( FT_MAX( sub[0].x, sub[1].x ), sub[2].x );
    }
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Library_समुच्चयः_Lcd_Filter_Weights( FT_Library      library,
                                  अचिह्नित वर्ण  *weights )
  {
    FT_अप्रयुक्तम्( library );
    FT_अप्रयुक्तम्( weights );

    प्रत्यागम FT_THROW( Unimplemented_Feature );
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Library_समुच्चयः_Lcd_Filter( FT_Library    library,
                           FT_LcdFilter  filter )
  {
    FT_अप्रयुक्तम्( library );
    FT_अप्रयुक्तम्( filter );

    प्रत्यागम FT_THROW( Unimplemented_Feature );
  }


  /* documentation in ftlcdfil.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Library_समुच्चयः_Lcd_Geometry( FT_Library  library,
                             FT_सदिशम्   sub[3] )
  {
    यदि ( !library )
      प्रत्यागम FT_THROW( Invalid_Library_Handle );

    यदि ( !sub )
      प्रत्यागम FT_THROW( Invalid_तर्कः );

    ft_memcpy( library->lcd_geometry, sub, 3 * परिमाण( FT_सदिशम् ) );

    प्रत्यागम FT_Err_Ok;
  }

#यदि_अन्तः /* !FT_CONFIG_OPTION_SUBPIXEL_RENDERING */


/*अन्तः*/
