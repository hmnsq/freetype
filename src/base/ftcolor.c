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


#അന്തര്ഭാവ <freetype/internal/ftdebug.h>
#അന്തര്ഭാവ <freetype/internal/sfnt.h>
#അന്തര്ഭാവ <freetype/internal/tttypes.h>
#അന്തര്ഭാവ <freetype/ftcolor.h>


#ifdef TT_CONFIG_OPTION_COLOR_LAYERS

  സ്ഥിതം
  സ്ഥിരം FT_Palette_Data  null_palette_data = { 0, ശൂന്യ, ശൂന്യ, 0, ശൂന്യ };


  /* documentation is in ftcolor.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Palette_Data_Get( FT_Face           face,
                       FT_Palette_Data  *apalette_data )
  {
    എങ്കിൽ ( !face )
      തിരികെ FT_THROW( Invalid_Face_Handle );
    എങ്കിൽ ( !apalette_data)
      തിരികെ FT_THROW( Invalid_Argument );

    എങ്കിൽ ( FT_IS_SFNT( face ) )
      *apalette_data = ( (TT_Face)face )->palette_data;
    അല്ലെങ്കിൽ
      *apalette_data = null_palette_data;

    തിരികെ FT_Err_Ok;
  }


  /* documentation is in ftcolor.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Palette_Select( FT_Face     face,
                     FT_UShort   palette_index,
                     FT_Color*  *apalette )
  {
    FT_Error  error;
    TT_Face   ttface = (TT_Face)face;


    എങ്കിൽ ( !face )
      തിരികെ FT_THROW( Invalid_Face_Handle );

    എങ്കിൽ ( !FT_IS_SFNT( face ) )
    {
      എങ്കിൽ ( apalette )
        *apalette = ശൂന്യ;

      തിരികെ FT_Err_Ok;
    }

    എങ്കിൽ ( palette_index != ttface->palette_index )
    {
      SFNT_Service  sfnt = (SFNT_Service)ttface->sfnt;


      error = sfnt->set_palette( ttface, palette_index );
      എങ്കിൽ ( error )
        തിരികെ error;

      ttface->palette_index = palette_index;
    }

    എങ്കിൽ ( apalette )
      *apalette = ttface->palette;

    തിരികെ FT_Err_Ok;
  }


  /* documentation is in ftcolor.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Palette_Set_Foreground_Color( FT_Face   face,
                                   FT_Color  foreground_color )
  {
    TT_Face  ttface;


    എങ്കിൽ ( !face )
      തിരികെ FT_THROW( Invalid_Face_Handle );

    എങ്കിൽ ( !FT_IS_SFNT( face ) )
      തിരികെ FT_Err_Ok;

    ttface = (TT_Face)face;

    ttface->foreground_color      = foreground_color;
    ttface->have_foreground_color = 1;

    തിരികെ FT_Err_Ok;
  }

#അല്ലെങ്കിൽ /* !TT_CONFIG_OPTION_COLOR_LAYERS */

  FT_EXPORT_DEF( FT_Error )
  FT_Palette_Data_Get( FT_Face           face,
                       FT_Palette_Data  *apalette_data )
  {
    FT_UNUSED( face );
    FT_UNUSED( apalette_data );


    തിരികെ FT_THROW( Unimplemented_Feature );
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Palette_Select( FT_Face     face,
                     FT_UShort   palette_index,
                     FT_Color*  *apalette )
  {
    FT_UNUSED( face );
    FT_UNUSED( palette_index );
    FT_UNUSED( apalette );


    തിരികെ FT_THROW( Unimplemented_Feature );
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Palette_Set_Foreground_Color( FT_Face   face,
                                   FT_Color  foreground_color )
  {
    FT_UNUSED( face );
    FT_UNUSED( foreground_color );


    തിരികെ FT_THROW( Unimplemented_Feature );
  }

#endif /* !TT_CONFIG_OPTION_COLOR_LAYERS */


/* END */
