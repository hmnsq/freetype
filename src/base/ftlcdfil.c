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


#అన్తర్భావ <freetype/internal/ftdebug.h>

#అన్తర్భావ <freetype/ftlcdfil.h>
#అన్తర్భావ <freetype/ftimage.h>
#అన్తర్భావ <freetype/internal/ftobjs.h>


#నిర్వచితమైతే FT_CONFIG_OPTION_SUBPIXEL_RENDERING

  /* add padding sufficient for a 5-tap filter, */
  /* which is 2/3 of a pixel                    */
  FT_పునాది_DEF( శూన్య )
  ft_lcd_padding( FT_BBox*        cbox,
                  FT_Glyph_స్థానం    స్థానం,
                  FT_Render_బహుళకం  బహుళకం )
  {
    FT_వాడని( స్థానం );

    ఒకవేళ ( బహుళకం == FT_RENDER_బహుళకం_LCD )
    {
      cbox->xMin -= 43;
      cbox->xMax += 43;
    }
    లేకపోతే ఒకవేళ ( బహుళకం == FT_RENDER_బహుళకం_LCD_V )
    {
      cbox->yMin -= 43;
      cbox->yMax += 43;
    }
  }


  /* documentation in ftlcdfil.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Library_సమితి_Lcd_Filter_Weights( FT_Library      library,
                                  గుర్తులేని అక్షరం  *weights )
  {
    ఒకవేళ ( !library )
      తిరిగి FT_THROW( Invalid_Library_Handle );

    ఒకవేళ ( !weights )
      తిరిగి FT_THROW( Invalid_వాదం );

    ft_memcpy( library->lcd_weights, weights, FT_LCD_FILTER_FIVE_TAPS );

    తిరిగి FT_Err_Ok;
  }


  /* documentation in ftlcdfil.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Library_సమితి_Lcd_Filter( FT_Library    library,
                           FT_LcdFilter  filter )
  {
    స్థిర స్థిరరాశి FT_LcdFiveTapFilter  default_weights =
                   { 0x08, 0x4d, 0x56, 0x4d, 0x08 };
    స్థిర స్థిరరాశి FT_LcdFiveTapFilter  light_weights =
                   { 0x00, 0x55, 0x56, 0x55, 0x00 };


    ఒకవేళ ( !library )
      తిరిగి FT_THROW( Invalid_Library_Handle );

    ఎంపిక ( filter )
    {
    స్థితి FT_LCD_FILTER_NONE:
      ft_memset( library->lcd_weights,
                 0,
                 FT_LCD_FILTER_FIVE_TAPS );
      ఆపు;

    స్థితి FT_LCD_FILTER_DEFAULT:
      ft_memcpy( library->lcd_weights,
                 default_weights,
                 FT_LCD_FILTER_FIVE_TAPS );
      ఆపు;

    స్థితి FT_LCD_FILTER_LIGHT:
      ft_memcpy( library->lcd_weights,
                 light_weights,
                 FT_LCD_FILTER_FIVE_TAPS );
      ఆపు;

    అప్రమేయ:
      తిరిగి FT_THROW( Invalid_వాదం );
    }

    తిరిగి FT_Err_Ok;
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Library_సమితి_Lcd_Geometry( FT_Library  library,
                             FT_సదిశ   sub[3] )
  {
    FT_వాడని( library );
    FT_వాడని( sub );

    తిరిగి FT_THROW( Unimplemented_Feature );
  }

#లేకపోతే /* !FT_CONFIG_OPTION_SUBPIXEL_RENDERING */

  /* add padding to accommodate outline shifts */
  FT_పునాది_DEF( శూన్య )
  ft_lcd_padding( FT_BBox*        cbox,
                  FT_Glyph_స్థానం    స్థానం,
                  FT_Render_బహుళకం  బహుళకం )
  {
    FT_సదిశ*  sub = స్థానం->library->lcd_geometry;

    ఒకవేళ ( బహుళకం == FT_RENDER_బహుళకం_LCD )
    {
      cbox->xMin -= FT_MAX( FT_MAX( sub[0].x, sub[1].x ), sub[2].x );
      cbox->xMax -= FT_MIN( FT_MIN( sub[0].x, sub[1].x ), sub[2].x );
      cbox->yMin -= FT_MAX( FT_MAX( sub[0].y, sub[1].y ), sub[2].y );
      cbox->yMax -= FT_MIN( FT_MIN( sub[0].y, sub[1].y ), sub[2].y );
    }
    లేకపోతే ఒకవేళ ( బహుళకం == FT_RENDER_బహుళకం_LCD_V )
    {
      cbox->xMin -= FT_MAX( FT_MAX( sub[0].y, sub[1].y ), sub[2].y );
      cbox->xMax -= FT_MIN( FT_MIN( sub[0].y, sub[1].y ), sub[2].y );
      cbox->yMin += FT_MIN( FT_MIN( sub[0].x, sub[1].x ), sub[2].x );
      cbox->yMax += FT_MAX( FT_MAX( sub[0].x, sub[1].x ), sub[2].x );
    }
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Library_సమితి_Lcd_Filter_Weights( FT_Library      library,
                                  గుర్తులేని అక్షరం  *weights )
  {
    FT_వాడని( library );
    FT_వాడని( weights );

    తిరిగి FT_THROW( Unimplemented_Feature );
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Library_సమితి_Lcd_Filter( FT_Library    library,
                           FT_LcdFilter  filter )
  {
    FT_వాడని( library );
    FT_వాడని( filter );

    తిరిగి FT_THROW( Unimplemented_Feature );
  }


  /* documentation in ftlcdfil.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Library_సమితి_Lcd_Geometry( FT_Library  library,
                             FT_సదిశ   sub[3] )
  {
    ఒకవేళ ( !library )
      తిరిగి FT_THROW( Invalid_Library_Handle );

    ఒకవేళ ( !sub )
      తిరిగి FT_THROW( Invalid_వాదం );

    ft_memcpy( library->lcd_geometry, sub, 3 * పరిమాణం( FT_సదిశ ) );

    తిరిగి FT_Err_Ok;
  }

#అయితే_ముగింపు /* !FT_CONFIG_OPTION_SUBPIXEL_RENDERING */


/*చివర*/
