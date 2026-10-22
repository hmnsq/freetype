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


#अन्तर्भाव <freetype/internal/ftdebug.h>
#अन्तर्भाव <freetype/internal/sfnt.h>
#अन्तर्भाव <freetype/internal/tttypes.h>
#अन्तर्भाव <freetype/ftcolor.h>


#ifdef TT_CONFIG_OPTION_COLOR_LAYERS

  स्थिर
  ध्रुव FT_Palette_Data  null_palette_data = { 0, रिक्त, रिक्त, 0, रिक्त };


  /* documentation is in ftcolor.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Palette_Data_Get( FT_Face           face,
                       FT_Palette_Data  *apalette_data )
  {
    जइ ( !face )
      प्रत्यागम FT_THROW( Invalid_Face_Handle );
    जइ ( !apalette_data)
      प्रत्यागम FT_THROW( Invalid_Argument );

    जइ ( FT_IS_SFNT( face ) )
      *apalette_data = ( (TT_Face)face )->palette_data;
    अण्णहा
      *apalette_data = null_palette_data;

    प्रत्यागम FT_Err_Ok;
  }


  /* documentation is in ftcolor.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Palette_Select( FT_Face     face,
                     FT_UShort   palette_index,
                     FT_Color*  *apalette )
  {
    FT_Error  error;
    TT_Face   ttface = (TT_Face)face;


    जइ ( !face )
      प्रत्यागम FT_THROW( Invalid_Face_Handle );

    जइ ( !FT_IS_SFNT( face ) )
    {
      जइ ( apalette )
        *apalette = रिक्त;

      प्रत्यागम FT_Err_Ok;
    }

    जइ ( palette_index != ttface->palette_index )
    {
      SFNT_Service  sfnt = (SFNT_Service)ttface->sfnt;


      error = sfnt->set_palette( ttface, palette_index );
      जइ ( error )
        प्रत्यागम error;

      ttface->palette_index = palette_index;
    }

    जइ ( apalette )
      *apalette = ttface->palette;

    प्रत्यागम FT_Err_Ok;
  }


  /* documentation is in ftcolor.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Palette_Set_Foreground_Color( FT_Face   face,
                                   FT_Color  foreground_color )
  {
    TT_Face  ttface;


    जइ ( !face )
      प्रत्यागम FT_THROW( Invalid_Face_Handle );

    जइ ( !FT_IS_SFNT( face ) )
      प्रत्यागम FT_Err_Ok;

    ttface = (TT_Face)face;

    ttface->foreground_color      = foreground_color;
    ttface->have_foreground_color = 1;

    प्रत्यागम FT_Err_Ok;
  }

#अण्णहा /* !TT_CONFIG_OPTION_COLOR_LAYERS */

  FT_EXPORT_DEF( FT_Error )
  FT_Palette_Data_Get( FT_Face           face,
                       FT_Palette_Data  *apalette_data )
  {
    FT_UNUSED( face );
    FT_UNUSED( apalette_data );


    प्रत्यागम FT_THROW( Unimplemented_Feature );
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Palette_Select( FT_Face     face,
                     FT_UShort   palette_index,
                     FT_Color*  *apalette )
  {
    FT_UNUSED( face );
    FT_UNUSED( palette_index );
    FT_UNUSED( apalette );


    प्रत्यागम FT_THROW( Unimplemented_Feature );
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Palette_Set_Foreground_Color( FT_Face   face,
                                   FT_Color  foreground_color )
  {
    FT_UNUSED( face );
    FT_UNUSED( foreground_color );


    प्रत्यागम FT_THROW( Unimplemented_Feature );
  }

#endif /* !TT_CONFIG_OPTION_COLOR_LAYERS */


/* END */
