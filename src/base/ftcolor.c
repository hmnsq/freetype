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


#ضمن <freetype/internal/ftdebug.h>
#ضمن <freetype/internal/sfnt.h>
#ضمن <freetype/internal/tttypes.h>
#ضمن <freetype/ftcolor.h>


#جيڪڏهن_وصف_ٿيل TT_CONFIG_OPTION_COLOR_LAYERS

  ساكن
  ثابت FT_Palette_معلومات  null_palette_معلومات = { 0, عدم, عدم, 0, عدم };


  /* documentation is in ftcolor.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Palette_معلومات_Get( FT_Face           face,
                       FT_Palette_معلومات  *apalette_معلومات )
  {
    إذا ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );
    إذا ( !apalette_معلومات)
      أرجع FT_THROW( Invalid_دليل );

    إذا ( FT_IS_SFNT( face ) )
      *apalette_معلومات = ( (TT_Face)face )->palette_معلومات;
    وإلا
      *apalette_معلومات = null_palette_معلومات;

    أرجع FT_Err_Ok;
  }


  /* documentation is in ftcolor.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Palette_Select( FT_Face     face,
                     FT_UShort   palette_اشاريو,
                     FT_Color*  *apalette )
  {
    FT_Error  error;
    TT_Face   ttface = (TT_Face)face;


    إذا ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );

    إذا ( !FT_IS_SFNT( face ) )
    {
      إذا ( apalette )
        *apalette = عدم;

      أرجع FT_Err_Ok;
    }

    إذا ( palette_اشاريو != ttface->palette_اشاريو )
    {
      SFNT_Service  sfnt = (SFNT_Service)ttface->sfnt;


      error = sfnt->سيٽ_palette( ttface, palette_اشاريو );
      إذا ( error )
        أرجع error;

      ttface->palette_اشاريو = palette_اشاريو;
    }

    إذا ( apalette )
      *apalette = ttface->palette;

    أرجع FT_Err_Ok;
  }


  /* documentation is in ftcolor.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Palette_سيٽ_Foreground_Color( FT_Face   face,
                                   FT_Color  foreground_color )
  {
    TT_Face  ttface;


    إذا ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );

    إذا ( !FT_IS_SFNT( face ) )
      أرجع FT_Err_Ok;

    ttface = (TT_Face)face;

    ttface->foreground_color      = foreground_color;
    ttface->have_foreground_color = 1;

    أرجع FT_Err_Ok;
  }

#وإلا /* !TT_CONFIG_OPTION_COLOR_LAYERS */

  FT_EXPORT_DEF( FT_Error )
  FT_Palette_معلومات_Get( FT_Face           face,
                       FT_Palette_معلومات  *apalette_معلومات )
  {
    FT_اڻ_استعمال_ٿيل( face );
    FT_اڻ_استعمال_ٿيل( apalette_معلومات );


    أرجع FT_THROW( Unimplemented_Feature );
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Palette_Select( FT_Face     face,
                     FT_UShort   palette_اشاريو,
                     FT_Color*  *apalette )
  {
    FT_اڻ_استعمال_ٿيل( face );
    FT_اڻ_استعمال_ٿيل( palette_اشاريو );
    FT_اڻ_استعمال_ٿيل( apalette );


    أرجع FT_THROW( Unimplemented_Feature );
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Palette_سيٽ_Foreground_Color( FT_Face   face,
                                   FT_Color  foreground_color )
  {
    FT_اڻ_استعمال_ٿيل( face );
    FT_اڻ_استعمال_ٿيل( foreground_color );


    أرجع FT_THROW( Unimplemented_Feature );
  }

#جيڪڏهن_پورو /* !TT_CONFIG_OPTION_COLOR_LAYERS */


/*پڇاڙي*/
