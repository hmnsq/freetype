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


#समावेश <freetype/internal/ftdebug.h>
#समावेश <freetype/internal/sfnt.h>
#समावेश <freetype/internal/tttypes.h>
#समावेश <freetype/ftcolor.h>


#अगर_घोषित TT_CONFIG_OPTION_COLOR_LAYERS

  अटल
  स्थिर FT_Palette_तथ्य  null_palette_तथ्य = { 0, शून्य, शून्य, 0, शून्य };


  /* documentation is in ftcolor.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Palette_तथ्य_Get( FT_Face           face,
                       FT_Palette_तथ्य  *apalette_तथ्य )
  {
    अगर ( !face )
      वापस FT_THROW( Invalid_Face_Handle );
    अगर ( !apalette_तथ्य)
      वापस FT_THROW( Invalid_Argument );

    अगर ( FT_IS_SFNT( face ) )
      *apalette_तथ्य = ( (TT_Face)face )->palette_तथ्य;
    अन्यथा
      *apalette_तथ्य = null_palette_तथ्य;

    वापस FT_Err_Ok;
  }


  /* documentation is in ftcolor.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Palette_Select( FT_Face     face,
                     FT_UShort   palette_सूचकांक,
                     FT_Color*  *apalette )
  {
    FT_Error  error;
    TT_Face   ttface = (TT_Face)face;


    अगर ( !face )
      वापस FT_THROW( Invalid_Face_Handle );

    अगर ( !FT_IS_SFNT( face ) )
    {
      अगर ( apalette )
        *apalette = शून्य;

      वापस FT_Err_Ok;
    }

    अगर ( palette_सूचकांक != ttface->palette_सूचकांक )
    {
      SFNT_Service  sfnt = (SFNT_Service)ttface->sfnt;


      error = sfnt->set_palette( ttface, palette_सूचकांक );
      अगर ( error )
        वापस error;

      ttface->palette_सूचकांक = palette_सूचकांक;
    }

    अगर ( apalette )
      *apalette = ttface->palette;

    वापस FT_Err_Ok;
  }


  /* documentation is in ftcolor.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Palette_Set_Foreground_Color( FT_Face   face,
                                   FT_Color  foreground_color )
  {
    TT_Face  ttface;


    अगर ( !face )
      वापस FT_THROW( Invalid_Face_Handle );

    अगर ( !FT_IS_SFNT( face ) )
      वापस FT_Err_Ok;

    ttface = (TT_Face)face;

    ttface->foreground_color      = foreground_color;
    ttface->have_foreground_color = 1;

    वापस FT_Err_Ok;
  }

#अन्यथा /* !TT_CONFIG_OPTION_COLOR_LAYERS */

  FT_EXPORT_DEF( FT_Error )
  FT_Palette_तथ्य_Get( FT_Face           face,
                       FT_Palette_तथ्य  *apalette_तथ्य )
  {
    FT_UNUSED( face );
    FT_UNUSED( apalette_तथ्य );


    वापस FT_THROW( Unimplemented_Feature );
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Palette_Select( FT_Face     face,
                     FT_UShort   palette_सूचकांक,
                     FT_Color*  *apalette )
  {
    FT_UNUSED( face );
    FT_UNUSED( palette_सूचकांक );
    FT_UNUSED( apalette );


    वापस FT_THROW( Unimplemented_Feature );
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Palette_Set_Foreground_Color( FT_Face   face,
                                   FT_Color  foreground_color )
  {
    FT_UNUSED( face );
    FT_UNUSED( foreground_color );


    वापस FT_THROW( Unimplemented_Feature );
  }

#पूर्ण_अगर /* !TT_CONFIG_OPTION_COLOR_LAYERS */


/* END */
