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


#அந்தர்பாவ <freetype/internal/ftdebug.h>
#அந்தர்பாவ <freetype/internal/sfnt.h>
#அந்தர்பாவ <freetype/internal/tttypes.h>
#அந்தர்பாவ <freetype/ftcolor.h>


#ifdef TT_CONFIG_OPTION_COLOR_LAYERS

  நிலையான
  மாறிலி FT_Palette_Data  null_palette_data = { 0, வெற்று, வெற்று, 0, வெற்று };


  /* documentation is in ftcolor.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Palette_Data_Get( FT_Face           face,
                       FT_Palette_Data  *apalette_data )
  {
    எனில் ( !face )
      திரும்பு FT_THROW( Invalid_Face_Handle );
    எனில் ( !apalette_data)
      திரும்பு FT_THROW( Invalid_Argument );

    எனில் ( FT_IS_SFNT( face ) )
      *apalette_data = ( (TT_Face)face )->palette_data;
    இல்லையெனில்
      *apalette_data = null_palette_data;

    திரும்பு FT_Err_Ok;
  }


  /* documentation is in ftcolor.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Palette_Select( FT_Face     face,
                     FT_UShort   palette_index,
                     FT_Color*  *apalette )
  {
    FT_Error  error;
    TT_Face   ttface = (TT_Face)face;


    எனில் ( !face )
      திரும்பு FT_THROW( Invalid_Face_Handle );

    எனில் ( !FT_IS_SFNT( face ) )
    {
      எனில் ( apalette )
        *apalette = வெற்று;

      திரும்பு FT_Err_Ok;
    }

    எனில் ( palette_index != ttface->palette_index )
    {
      SFNT_Service  sfnt = (SFNT_Service)ttface->sfnt;


      error = sfnt->set_palette( ttface, palette_index );
      எனில் ( error )
        திரும்பு error;

      ttface->palette_index = palette_index;
    }

    எனில் ( apalette )
      *apalette = ttface->palette;

    திரும்பு FT_Err_Ok;
  }


  /* documentation is in ftcolor.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Palette_Set_Foreground_Color( FT_Face   face,
                                   FT_Color  foreground_color )
  {
    TT_Face  ttface;


    எனில் ( !face )
      திரும்பு FT_THROW( Invalid_Face_Handle );

    எனில் ( !FT_IS_SFNT( face ) )
      திரும்பு FT_Err_Ok;

    ttface = (TT_Face)face;

    ttface->foreground_color      = foreground_color;
    ttface->have_foreground_color = 1;

    திரும்பு FT_Err_Ok;
  }

#இல்லையெனில் /* !TT_CONFIG_OPTION_COLOR_LAYERS */

  FT_EXPORT_DEF( FT_Error )
  FT_Palette_Data_Get( FT_Face           face,
                       FT_Palette_Data  *apalette_data )
  {
    FT_UNUSED( face );
    FT_UNUSED( apalette_data );


    திரும்பு FT_THROW( Unimplemented_Feature );
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Palette_Select( FT_Face     face,
                     FT_UShort   palette_index,
                     FT_Color*  *apalette )
  {
    FT_UNUSED( face );
    FT_UNUSED( palette_index );
    FT_UNUSED( apalette );


    திரும்பு FT_THROW( Unimplemented_Feature );
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Palette_Set_Foreground_Color( FT_Face   face,
                                   FT_Color  foreground_color )
  {
    FT_UNUSED( face );
    FT_UNUSED( foreground_color );


    திரும்பு FT_THROW( Unimplemented_Feature );
  }

#endif /* !TT_CONFIG_OPTION_COLOR_LAYERS */


/* END */
