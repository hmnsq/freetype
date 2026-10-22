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


#ಅನ್ತರ್ಭಾವ <freetype/internal/ftdebug.h>
#ಅನ್ತರ್ಭಾವ <freetype/internal/sfnt.h>
#ಅನ್ತರ್ಭಾವ <freetype/internal/tttypes.h>
#ಅನ್ತರ್ಭಾವ <freetype/ftcolor.h>


#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ TT_CONFIG_OPTION_COLOR_LAYERS

  ಸ್ಥಿರ
  ಸ್ಥಿರಾಂಕ FT_Palette_ದತ್ತಾಂಶ  null_palette_ದತ್ತಾಂಶ = { 0, ಖಾಲಿ, ಖಾಲಿ, 0, ಖಾಲಿ };


  /* documentation is in ftcolor.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Palette_ದತ್ತಾಂಶ_Get( FT_Face           face,
                       FT_Palette_ದತ್ತಾಂಶ  *apalette_ದತ್ತಾಂಶ )
  {
    ಆದರೆ ( !face )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_Face_Handle );
    ಆದರೆ ( !apalette_ದತ್ತಾಂಶ)
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

    ಆದರೆ ( FT_IS_SFNT( face ) )
      *apalette_ದತ್ತಾಂಶ = ( (TT_Face)face )->palette_ದತ್ತಾಂಶ;
    ಇಲ್ಲದಿದ್ದರೆ
      *apalette_ದತ್ತಾಂಶ = null_palette_ದತ್ತಾಂಶ;

    ಹಿಂತಿರುಗಿ FT_Err_Ok;
  }


  /* documentation is in ftcolor.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Palette_Select( FT_Face     face,
                     FT_UShort   palette_ಸೂಚ್ಯಂಕ,
                     FT_Color*  *apalette )
  {
    FT_Error  error;
    TT_Face   ttface = (TT_Face)face;


    ಆದರೆ ( !face )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_Face_Handle );

    ಆದರೆ ( !FT_IS_SFNT( face ) )
    {
      ಆದರೆ ( apalette )
        *apalette = ಖಾಲಿ;

      ಹಿಂತಿರುಗಿ FT_Err_Ok;
    }

    ಆದರೆ ( palette_ಸೂಚ್ಯಂಕ != ttface->palette_ಸೂಚ್ಯಂಕ )
    {
      SFNT_Service  sfnt = (SFNT_Service)ttface->sfnt;


      error = sfnt->ಗಣ_palette( ttface, palette_ಸೂಚ್ಯಂಕ );
      ಆದರೆ ( error )
        ಹಿಂತಿರುಗಿ error;

      ttface->palette_ಸೂಚ್ಯಂಕ = palette_ಸೂಚ್ಯಂಕ;
    }

    ಆದರೆ ( apalette )
      *apalette = ttface->palette;

    ಹಿಂತಿರುಗಿ FT_Err_Ok;
  }


  /* documentation is in ftcolor.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Palette_ಗಣ_Foreground_Color( FT_Face   face,
                                   FT_Color  foreground_color )
  {
    TT_Face  ttface;


    ಆದರೆ ( !face )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_Face_Handle );

    ಆದರೆ ( !FT_IS_SFNT( face ) )
      ಹಿಂತಿರುಗಿ FT_Err_Ok;

    ttface = (TT_Face)face;

    ttface->foreground_color      = foreground_color;
    ttface->have_foreground_color = 1;

    ಹಿಂತಿರುಗಿ FT_Err_Ok;
  }

#ಇಲ್ಲದಿದ್ದರೆ /* !TT_CONFIG_OPTION_COLOR_LAYERS */

  FT_EXPORT_DEF( FT_Error )
  FT_Palette_ದತ್ತಾಂಶ_Get( FT_Face           face,
                       FT_Palette_ದತ್ತಾಂಶ  *apalette_ದತ್ತಾಂಶ )
  {
    FT_ಬಳಸದ( face );
    FT_ಬಳಸದ( apalette_ದತ್ತಾಂಶ );


    ಹಿಂತಿರುಗಿ FT_THROW( Unimplemented_Feature );
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Palette_Select( FT_Face     face,
                     FT_UShort   palette_ಸೂಚ್ಯಂಕ,
                     FT_Color*  *apalette )
  {
    FT_ಬಳಸದ( face );
    FT_ಬಳಸದ( palette_ಸೂಚ್ಯಂಕ );
    FT_ಬಳಸದ( apalette );


    ಹಿಂತಿರುಗಿ FT_THROW( Unimplemented_Feature );
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Palette_ಗಣ_Foreground_Color( FT_Face   face,
                                   FT_Color  foreground_color )
  {
    FT_ಬಳಸದ( face );
    FT_ಬಳಸದ( foreground_color );


    ಹಿಂತಿರುಗಿ FT_THROW( Unimplemented_Feature );
  }

#ಆದರೆ_ಅಂತ್ಯ /* !TT_CONFIG_OPTION_COLOR_LAYERS */


/*ಕೊನೆ*/
