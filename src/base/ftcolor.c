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


#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftdebug.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/sfnt.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/tttypes.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/ftcolor.h>


#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ TT_CONFIG_OPTION_COLOR_LAYERS

  ਸਥਿਰ
  ਅਚਲ FT_Palette_ਅੰਕੜੇ  null_palette_ਅੰਕੜੇ = { 0, ਨਿਰਾ, ਨਿਰਾ, 0, ਨਿਰਾ };


  /* documentation is in ftcolor.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Palette_ਅੰਕੜੇ_Get( FT_Face           face,
                       FT_Palette_ਅੰਕੜੇ  *apalette_ਅੰਕੜੇ )
  {
    ਜੇ ( !face )
      ਵਾਪਸ FT_THROW( Invalid_Face_Handle );
    ਜੇ ( !apalette_ਅੰਕੜੇ)
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    ਜੇ ( FT_IS_SFNT( face ) )
      *apalette_ਅੰਕੜੇ = ( (TT_Face)face )->palette_ਅੰਕੜੇ;
    ਨਹੀਂਤਾਂ
      *apalette_ਅੰਕੜੇ = null_palette_ਅੰਕੜੇ;

    ਵਾਪਸ FT_Err_Ok;
  }


  /* documentation is in ftcolor.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Palette_Select( FT_Face     face,
                     FT_UShort   palette_ਸੂਚਕਾਂਕ,
                     FT_Color*  *apalette )
  {
    FT_Error  error;
    TT_Face   ttface = (TT_Face)face;


    ਜੇ ( !face )
      ਵਾਪਸ FT_THROW( Invalid_Face_Handle );

    ਜੇ ( !FT_IS_SFNT( face ) )
    {
      ਜੇ ( apalette )
        *apalette = ਨਿਰਾ;

      ਵਾਪਸ FT_Err_Ok;
    }

    ਜੇ ( palette_ਸੂਚਕਾਂਕ != ttface->palette_ਸੂਚਕਾਂਕ )
    {
      SFNT_Service  sfnt = (SFNT_Service)ttface->sfnt;


      error = sfnt->ਸੈੱਟ_palette( ttface, palette_ਸੂਚਕਾਂਕ );
      ਜੇ ( error )
        ਵਾਪਸ error;

      ttface->palette_ਸੂਚਕਾਂਕ = palette_ਸੂਚਕਾਂਕ;
    }

    ਜੇ ( apalette )
      *apalette = ttface->palette;

    ਵਾਪਸ FT_Err_Ok;
  }


  /* documentation is in ftcolor.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Palette_ਸੈੱਟ_Foreground_Color( FT_Face   face,
                                   FT_Color  foreground_color )
  {
    TT_Face  ttface;


    ਜੇ ( !face )
      ਵਾਪਸ FT_THROW( Invalid_Face_Handle );

    ਜੇ ( !FT_IS_SFNT( face ) )
      ਵਾਪਸ FT_Err_Ok;

    ttface = (TT_Face)face;

    ttface->foreground_color      = foreground_color;
    ttface->have_foreground_color = 1;

    ਵਾਪਸ FT_Err_Ok;
  }

#ਨਹੀਂਤਾਂ /* !TT_CONFIG_OPTION_COLOR_LAYERS */

  FT_EXPORT_DEF( FT_Error )
  FT_Palette_ਅੰਕੜੇ_Get( FT_Face           face,
                       FT_Palette_ਅੰਕੜੇ  *apalette_ਅੰਕੜੇ )
  {
    FT_ਅਣਵਰਤਿਆ( face );
    FT_ਅਣਵਰਤਿਆ( apalette_ਅੰਕੜੇ );


    ਵਾਪਸ FT_THROW( Unimplemented_Feature );
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Palette_Select( FT_Face     face,
                     FT_UShort   palette_ਸੂਚਕਾਂਕ,
                     FT_Color*  *apalette )
  {
    FT_ਅਣਵਰਤਿਆ( face );
    FT_ਅਣਵਰਤਿਆ( palette_ਸੂਚਕਾਂਕ );
    FT_ਅਣਵਰਤਿਆ( apalette );


    ਵਾਪਸ FT_THROW( Unimplemented_Feature );
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Palette_ਸੈੱਟ_Foreground_Color( FT_Face   face,
                                   FT_Color  foreground_color )
  {
    FT_ਅਣਵਰਤਿਆ( face );
    FT_ਅਣਵਰਤਿਆ( foreground_color );


    ਵਾਪਸ FT_THROW( Unimplemented_Feature );
  }

#ਜੇ_ਅੰਤ /* !TT_CONFIG_OPTION_COLOR_LAYERS */


/*ਸਿਰਾ*/
