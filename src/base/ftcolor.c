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


#ifdef TT_CONFIG_OPTION_COLOR_LAYERS

  ಸ್ಥಿರ
  ಧ್ರುವ FT_Palette_Data  null_palette_data = { 0, ರಿಕ್ತ, ರಿಕ್ತ, 0, ರಿಕ್ತ };


  /* documentation is in ftcolor.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Palette_Data_Get( FT_Face           face,
                       FT_Palette_Data  *apalette_data )
  {
    ಯದಿ ( !face )
      ಪ್ರತ್ಯಾಗಮ FT_THROW( Invalid_Face_Handle );
    ಯದಿ ( !apalette_data)
      ಪ್ರತ್ಯಾಗಮ FT_THROW( Invalid_Argument );

    ಯದಿ ( FT_IS_SFNT( face ) )
      *apalette_data = ( (TT_Face)face )->palette_data;
    ಅನ್ಯಥಾ
      *apalette_data = null_palette_data;

    ಪ್ರತ್ಯಾಗಮ FT_Err_Ok;
  }


  /* documentation is in ftcolor.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Palette_Select( FT_Face     face,
                     FT_UShort   palette_index,
                     FT_Color*  *apalette )
  {
    FT_Error  error;
    TT_Face   ttface = (TT_Face)face;


    ಯದಿ ( !face )
      ಪ್ರತ್ಯಾಗಮ FT_THROW( Invalid_Face_Handle );

    ಯದಿ ( !FT_IS_SFNT( face ) )
    {
      ಯದಿ ( apalette )
        *apalette = ರಿಕ್ತ;

      ಪ್ರತ್ಯಾಗಮ FT_Err_Ok;
    }

    ಯದಿ ( palette_index != ttface->palette_index )
    {
      SFNT_Service  sfnt = (SFNT_Service)ttface->sfnt;


      error = sfnt->set_palette( ttface, palette_index );
      ಯದಿ ( error )
        ಪ್ರತ್ಯಾಗಮ error;

      ttface->palette_index = palette_index;
    }

    ಯದಿ ( apalette )
      *apalette = ttface->palette;

    ಪ್ರತ್ಯಾಗಮ FT_Err_Ok;
  }


  /* documentation is in ftcolor.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Palette_Set_Foreground_Color( FT_Face   face,
                                   FT_Color  foreground_color )
  {
    TT_Face  ttface;


    ಯದಿ ( !face )
      ಪ್ರತ್ಯಾಗಮ FT_THROW( Invalid_Face_Handle );

    ಯದಿ ( !FT_IS_SFNT( face ) )
      ಪ್ರತ್ಯಾಗಮ FT_Err_Ok;

    ttface = (TT_Face)face;

    ttface->foreground_color      = foreground_color;
    ttface->have_foreground_color = 1;

    ಪ್ರತ್ಯಾಗಮ FT_Err_Ok;
  }

#ಅನ್ಯಥಾ /* !TT_CONFIG_OPTION_COLOR_LAYERS */

  FT_EXPORT_DEF( FT_Error )
  FT_Palette_Data_Get( FT_Face           face,
                       FT_Palette_Data  *apalette_data )
  {
    FT_UNUSED( face );
    FT_UNUSED( apalette_data );


    ಪ್ರತ್ಯಾಗಮ FT_THROW( Unimplemented_Feature );
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Palette_Select( FT_Face     face,
                     FT_UShort   palette_index,
                     FT_Color*  *apalette )
  {
    FT_UNUSED( face );
    FT_UNUSED( palette_index );
    FT_UNUSED( apalette );


    ಪ್ರತ್ಯಾಗಮ FT_THROW( Unimplemented_Feature );
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Palette_Set_Foreground_Color( FT_Face   face,
                                   FT_Color  foreground_color )
  {
    FT_UNUSED( face );
    FT_UNUSED( foreground_color );


    ಪ್ರತ್ಯಾಗಮ FT_THROW( Unimplemented_Feature );
  }

#endif /* !TT_CONFIG_OPTION_COLOR_LAYERS */


/* END */
