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


#ଅନ୍ତର୍ଭାଵ <freetype/internal/ftdebug.h>

#ଅନ୍ତର୍ଭାଵ <freetype/ftlcdfil.h>
#ଅନ୍ତର୍ଭାଵ <freetype/ftimage.h>
#ଅନ୍ତର୍ଭାଵ <freetype/internal/ftobjs.h>


#ifdef FT_CONFIG_OPTION_SUBPIXEL_RENDERING

  /* add padding sufficient for a 5-tap filter, */
  /* which is 2/3 of a pixel                    */
  FT_BASE_DEF( ଶୂନ୍ୟ )
  ft_lcd_padding( FT_BBox*        cbox,
                  FT_GlyphSlot    slot,
                  FT_Render_Mode  mode )
  {
    FT_UNUSED( slot );

    ଯଦି ( mode == FT_RENDER_MODE_LCD )
    {
      cbox->xMin -= 43;
      cbox->xMax += 43;
    }
    ଅନ୍ୟଥା ଯଦି ( mode == FT_RENDER_MODE_LCD_V )
    {
      cbox->yMin -= 43;
      cbox->yMax += 43;
    }
  }


  /* documentation in ftlcdfil.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Library_SetLcdFilterWeights( FT_Library      library,
                                  unsigned ଅକ୍ଷର  *weights )
  {
    ଯଦି ( !library )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Library_Handle );

    ଯଦି ( !weights )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Argument );

    ft_memcpy( library->lcd_weights, weights, FT_LCD_FILTER_FIVE_TAPS );

    ଫେରନ୍ତୁ FT_Err_Ok;
  }


  /* documentation in ftlcdfil.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Library_SetLcdFilter( FT_Library    library,
                           FT_LcdFilter  filter )
  {
    ସ୍ଥିର ଅଚଳ FT_LcdFiveTapFilter  default_weights =
                   { 0x08, 0x4d, 0x56, 0x4d, 0x08 };
    ସ୍ଥିର ଅଚଳ FT_LcdFiveTapFilter  light_weights =
                   { 0x00, 0x55, 0x56, 0x55, 0x00 };


    ଯଦି ( !library )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Library_Handle );

    ବାଛ ( filter )
    {
    ସ୍ଥିତି FT_LCD_FILTER_NONE:
      ft_memset( library->lcd_weights,
                 0,
                 FT_LCD_FILTER_FIVE_TAPS );
      ଭାଙ୍ଗ;

    ସ୍ଥିତି FT_LCD_FILTER_DEFAULT:
      ft_memcpy( library->lcd_weights,
                 default_weights,
                 FT_LCD_FILTER_FIVE_TAPS );
      ଭାଙ୍ଗ;

    ସ୍ଥିତି FT_LCD_FILTER_LIGHT:
      ft_memcpy( library->lcd_weights,
                 light_weights,
                 FT_LCD_FILTER_FIVE_TAPS );
      ଭାଙ୍ଗ;

    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      ଫେରନ୍ତୁ FT_THROW( Invalid_Argument );
    }

    ଫେରନ୍ତୁ FT_Err_Ok;
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Library_SetLcdGeometry( FT_Library  library,
                             FT_Vector   sub[3] )
  {
    FT_UNUSED( library );
    FT_UNUSED( sub );

    ଫେରନ୍ତୁ FT_THROW( Unimplemented_Feature );
  }

#ଅନ୍ୟଥା /* !FT_CONFIG_OPTION_SUBPIXEL_RENDERING */

  /* add padding to accommodate outline shifts */
  FT_BASE_DEF( ଶୂନ୍ୟ )
  ft_lcd_padding( FT_BBox*        cbox,
                  FT_GlyphSlot    slot,
                  FT_Render_Mode  mode )
  {
    FT_Vector*  sub = slot->library->lcd_geometry;

    ଯଦି ( mode == FT_RENDER_MODE_LCD )
    {
      cbox->xMin -= FT_MAX( FT_MAX( sub[0].x, sub[1].x ), sub[2].x );
      cbox->xMax -= FT_MIN( FT_MIN( sub[0].x, sub[1].x ), sub[2].x );
      cbox->yMin -= FT_MAX( FT_MAX( sub[0].y, sub[1].y ), sub[2].y );
      cbox->yMax -= FT_MIN( FT_MIN( sub[0].y, sub[1].y ), sub[2].y );
    }
    ଅନ୍ୟଥା ଯଦି ( mode == FT_RENDER_MODE_LCD_V )
    {
      cbox->xMin -= FT_MAX( FT_MAX( sub[0].y, sub[1].y ), sub[2].y );
      cbox->xMax -= FT_MIN( FT_MIN( sub[0].y, sub[1].y ), sub[2].y );
      cbox->yMin += FT_MIN( FT_MIN( sub[0].x, sub[1].x ), sub[2].x );
      cbox->yMax += FT_MAX( FT_MAX( sub[0].x, sub[1].x ), sub[2].x );
    }
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Library_SetLcdFilterWeights( FT_Library      library,
                                  unsigned ଅକ୍ଷର  *weights )
  {
    FT_UNUSED( library );
    FT_UNUSED( weights );

    ଫେରନ୍ତୁ FT_THROW( Unimplemented_Feature );
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Library_SetLcdFilter( FT_Library    library,
                           FT_LcdFilter  filter )
  {
    FT_UNUSED( library );
    FT_UNUSED( filter );

    ଫେରନ୍ତୁ FT_THROW( Unimplemented_Feature );
  }


  /* documentation in ftlcdfil.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Library_SetLcdGeometry( FT_Library  library,
                             FT_Vector   sub[3] )
  {
    ଯଦି ( !library )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Library_Handle );

    ଯଦି ( !sub )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Argument );

    ft_memcpy( library->lcd_geometry, sub, 3 * ଆକାର( FT_Vector ) );

    ଫେରନ୍ତୁ FT_Err_Ok;
  }

#endif /* !FT_CONFIG_OPTION_SUBPIXEL_RENDERING */


/* END */
