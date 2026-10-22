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


#অন্তর্ভাৱ <freetype/internal/ftdebug.h>
#অন্তর্ভাৱ <freetype/internal/sfnt.h>
#অন্তর্ভাৱ <freetype/internal/tttypes.h>
#অন্তর্ভাৱ <freetype/ftcolor.h>


#ifdef TT_CONFIG_OPTION_COLOR_LAYERS

  স্থির
  ধ্রুৱ FT_Palette_Data  null_palette_data = { 0, রিক্ত, রিক্ত, 0, রিক্ত };


  /* documentation is in ftcolor.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Palette_Data_Get( FT_Face           face,
                       FT_Palette_Data  *apalette_data )
  {
    যদি ( !face )
      ঘূৰাই FT_THROW( Invalid_Face_Handle );
    যদি ( !apalette_data)
      ঘূৰাই FT_THROW( Invalid_Argument );

    যদি ( FT_IS_SFNT( face ) )
      *apalette_data = ( (TT_Face)face )->palette_data;
    অন্যথা
      *apalette_data = null_palette_data;

    ঘূৰাই FT_Err_Ok;
  }


  /* documentation is in ftcolor.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Palette_Select( FT_Face     face,
                     FT_UShort   palette_index,
                     FT_Color*  *apalette )
  {
    FT_Error  error;
    TT_Face   ttface = (TT_Face)face;


    যদি ( !face )
      ঘূৰাই FT_THROW( Invalid_Face_Handle );

    যদি ( !FT_IS_SFNT( face ) )
    {
      যদি ( apalette )
        *apalette = রিক্ত;

      ঘূৰাই FT_Err_Ok;
    }

    যদি ( palette_index != ttface->palette_index )
    {
      SFNT_Service  sfnt = (SFNT_Service)ttface->sfnt;


      error = sfnt->set_palette( ttface, palette_index );
      যদি ( error )
        ঘূৰাই error;

      ttface->palette_index = palette_index;
    }

    যদি ( apalette )
      *apalette = ttface->palette;

    ঘূৰাই FT_Err_Ok;
  }


  /* documentation is in ftcolor.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Palette_Set_Foreground_Color( FT_Face   face,
                                   FT_Color  foreground_color )
  {
    TT_Face  ttface;


    যদি ( !face )
      ঘূৰাই FT_THROW( Invalid_Face_Handle );

    যদি ( !FT_IS_SFNT( face ) )
      ঘূৰাই FT_Err_Ok;

    ttface = (TT_Face)face;

    ttface->foreground_color      = foreground_color;
    ttface->have_foreground_color = 1;

    ঘূৰাই FT_Err_Ok;
  }

#অন্যথা /* !TT_CONFIG_OPTION_COLOR_LAYERS */

  FT_EXPORT_DEF( FT_Error )
  FT_Palette_Data_Get( FT_Face           face,
                       FT_Palette_Data  *apalette_data )
  {
    FT_UNUSED( face );
    FT_UNUSED( apalette_data );


    ঘূৰাই FT_THROW( Unimplemented_Feature );
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Palette_Select( FT_Face     face,
                     FT_UShort   palette_index,
                     FT_Color*  *apalette )
  {
    FT_UNUSED( face );
    FT_UNUSED( palette_index );
    FT_UNUSED( apalette );


    ঘূৰাই FT_THROW( Unimplemented_Feature );
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Palette_Set_Foreground_Color( FT_Face   face,
                                   FT_Color  foreground_color )
  {
    FT_UNUSED( face );
    FT_UNUSED( foreground_color );


    ঘূৰাই FT_THROW( Unimplemented_Feature );
  }

#endif /* !TT_CONFIG_OPTION_COLOR_LAYERS */


/* END */
