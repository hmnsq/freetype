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


#include <freetype/internal/ftdebug.h>
#include <freetype/internal/sfnt.h>
#include <freetype/internal/tttypes.h>
#include <freetype/ftcolor.h>


#ifdef TT_CONFIG_OPTION_COLOR_LAYERS

  static
  const FT_Palette_Data  null_palette_data = { 0, NULL, NULL, 0, NULL };


  /* documentation is in ftcolor.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Palette_Data_Get( FT_Face           face,
                       FT_Palette_Data  *apalette_data )
  {
    если ( !face )
      вернуть FT_THROW( Invalid_Face_Handle );
    если ( !apalette_data)
      вернуть FT_THROW( Invalid_Argument );

    если ( FT_IS_SFNT( face ) )
      *apalette_data = ( (TT_Face)face )->palette_data;
    иначе
      *apalette_data = null_palette_data;

    вернуть FT_Err_Ok;
  }


  /* documentation is in ftcolor.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Palette_Select( FT_Face     face,
                     FT_UShort   palette_index,
                     FT_Color*  *apalette )
  {
    FT_Error  error;
    TT_Face   ttface = (TT_Face)face;


    если ( !face )
      вернуть FT_THROW( Invalid_Face_Handle );

    если ( !FT_IS_SFNT( face ) )
    {
      если ( apalette )
        *apalette = NULL;

      вернуть FT_Err_Ok;
    }

    если ( palette_index != ttface->palette_index )
    {
      SFNT_Service  sfnt = (SFNT_Service)ttface->sfnt;


      error = sfnt->set_palette( ttface, palette_index );
      если ( error )
        вернуть error;

      ttface->palette_index = palette_index;
    }

    если ( apalette )
      *apalette = ttface->palette;

    вернуть FT_Err_Ok;
  }


  /* documentation is in ftcolor.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Palette_Set_Foreground_Color( FT_Face   face,
                                   FT_Color  foreground_color )
  {
    TT_Face  ttface;


    если ( !face )
      вернуть FT_THROW( Invalid_Face_Handle );

    если ( !FT_IS_SFNT( face ) )
      вернуть FT_Err_Ok;

    ttface = (TT_Face)face;

    ttface->foreground_color      = foreground_color;
    ttface->have_foreground_color = 1;

    вернуть FT_Err_Ok;
  }

#иначе /* !TT_CONFIG_OPTION_COLOR_LAYERS */

  FT_EXPORT_DEF( FT_Error )
  FT_Palette_Data_Get( FT_Face           face,
                       FT_Palette_Data  *apalette_data )
  {
    FT_UNUSED( face );
    FT_UNUSED( apalette_data );


    вернуть FT_THROW( Unimplemented_Feature );
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Palette_Select( FT_Face     face,
                     FT_UShort   palette_index,
                     FT_Color*  *apalette )
  {
    FT_UNUSED( face );
    FT_UNUSED( palette_index );
    FT_UNUSED( apalette );


    вернуть FT_THROW( Unimplemented_Feature );
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Palette_Set_Foreground_Color( FT_Face   face,
                                   FT_Color  foreground_color )
  {
    FT_UNUSED( face );
    FT_UNUSED( foreground_color );


    вернуть FT_THROW( Unimplemented_Feature );
  }

#endif /* !TT_CONFIG_OPTION_COLOR_LAYERS */


/* END */
