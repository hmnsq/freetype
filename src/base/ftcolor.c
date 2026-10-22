/****************************************************************************
 *
 * ftcolor.c
 *
 *   FreeType's glyph color management (body).
 *
 * Copyright (C) 2018-2026 by
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
#شامل <freetype/internal/sfnt.h>
#شامل <freetype/internal/tttypes.h>
#شامل <freetype/ftcolor.h>


#اگر_تعریف_شدہ TT_CONFIG_OPTION_COLOR_LAYERS

  ساکن
  ثابت FT_Palette_ڈیٹا  null_palette_ڈیٹا = { 0, تہی, تہی, 0, تہی };


  /* documentation is in ftcolor.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Palette_ڈیٹا_Get( FT_Face           face,
                       FT_Palette_ڈیٹا  *apalette_ڈیٹا )
  {
    اگر ( !face )
      واپس FT_THROW( Invalid_Face_Handle );
    اگر ( !apalette_ڈیٹا)
      واپس FT_THROW( Invalid_دلیل );

    اگر ( FT_IS_SFNT( face ) )
      *apalette_ڈیٹا = ( (TT_Face)face )->palette_ڈیٹا;
    ورنہ
      *apalette_ڈیٹا = null_palette_ڈیٹا;

    واپس FT_Err_Ok;
  }


  /* documentation is in ftcolor.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Palette_Select( FT_Face     face,
                     FT_UShort   palette_اشاریہ,
                     FT_Color*  *apalette )
  {
    FT_Error  error;
    TT_Face   ttface = (TT_Face)face;


    اگر ( !face )
      واپس FT_THROW( Invalid_Face_Handle );

    اگر ( !FT_IS_SFNT( face ) )
    {
      اگر ( apalette )
        *apalette = تہی;

      واپس FT_Err_Ok;
    }

    اگر ( palette_اشاریہ != ttface->palette_اشاریہ )
    {
      SFNT_Service  sfnt = (SFNT_Service)ttface->sfnt;


      error = sfnt->سیٹ_palette( ttface, palette_اشاریہ );
      اگر ( error )
        واپس error;

      ttface->palette_اشاریہ = palette_اشاریہ;
    }

    اگر ( apalette )
      *apalette = ttface->palette;

    واپس FT_Err_Ok;
  }


  /* documentation is in ftcolor.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Palette_سیٹ_Foreground_Color( FT_Face   face,
                                   FT_Color  foreground_color )
  {
    TT_Face  ttface;


    اگر ( !face )
      واپس FT_THROW( Invalid_Face_Handle );

    اگر ( !FT_IS_SFNT( face ) )
      واپس FT_Err_Ok;

    ttface = (TT_Face)face;

    ttface->foreground_color      = foreground_color;
    ttface->have_foreground_color = 1;

    واپس FT_Err_Ok;
  }

#ورنہ /* !TT_CONFIG_OPTION_COLOR_LAYERS */

  FT_EXPORT_DEF( FT_Error )
  FT_Palette_ڈیٹا_Get( FT_Face           face,
                       FT_Palette_ڈیٹا  *apalette_ڈیٹا )
  {
    FT_غیر_مستعمل( face );
    FT_غیر_مستعمل( apalette_ڈیٹا );


    واپس FT_THROW( Unimplemented_Feature );
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Palette_Select( FT_Face     face,
                     FT_UShort   palette_اشاریہ,
                     FT_Color*  *apalette )
  {
    FT_غیر_مستعمل( face );
    FT_غیر_مستعمل( palette_اشاریہ );
    FT_غیر_مستعمل( apalette );


    واپس FT_THROW( Unimplemented_Feature );
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Palette_سیٹ_Foreground_Color( FT_Face   face,
                                   FT_Color  foreground_color )
  {
    FT_غیر_مستعمل( face );
    FT_غیر_مستعمل( foreground_color );


    واپس FT_THROW( Unimplemented_Feature );
  }

#اگر_ختم /* !TT_CONFIG_OPTION_COLOR_LAYERS */


/*انتہا*/
