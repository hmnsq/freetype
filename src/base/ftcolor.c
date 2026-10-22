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


#यदि_परिभाषितम् TT_CONFIG_OPTION_COLOR_LAYERS

  स्थिर
  ध्रुव FT_Palette_दत्त  null_palette_दत्त = { 0, रिक्त, रिक्त, 0, रिक्त };


  /* documentation is in ftcolor.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Palette_दत्त_Get( FT_Face           face,
                       FT_Palette_दत्त  *apalette_दत्त )
  {
    यदि ( !face )
      प्रत्यागम FT_THROW( Invalid_Face_Handle );
    यदि ( !apalette_दत्त)
      प्रत्यागम FT_THROW( Invalid_तर्कः );

    यदि ( FT_IS_SFNT( face ) )
      *apalette_दत्त = ( (TT_Face)face )->palette_दत्त;
    अन्यथा
      *apalette_दत्त = null_palette_दत्त;

    प्रत्यागम FT_Err_Ok;
  }


  /* documentation is in ftcolor.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Palette_Select( FT_Face     face,
                     FT_UShort   palette_सूचकांक,
                     FT_Color*  *apalette )
  {
    FT_Error  error;
    TT_Face   ttface = (TT_Face)face;


    यदि ( !face )
      प्रत्यागम FT_THROW( Invalid_Face_Handle );

    यदि ( !FT_IS_SFNT( face ) )
    {
      यदि ( apalette )
        *apalette = रिक्त;

      प्रत्यागम FT_Err_Ok;
    }

    यदि ( palette_सूचकांक != ttface->palette_सूचकांक )
    {
      SFNT_Service  sfnt = (SFNT_Service)ttface->sfnt;


      error = sfnt->समुच्चयः_palette( ttface, palette_सूचकांक );
      यदि ( error )
        प्रत्यागम error;

      ttface->palette_सूचकांक = palette_सूचकांक;
    }

    यदि ( apalette )
      *apalette = ttface->palette;

    प्रत्यागम FT_Err_Ok;
  }


  /* documentation is in ftcolor.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Palette_समुच्चयः_Foreground_Color( FT_Face   face,
                                   FT_Color  foreground_color )
  {
    TT_Face  ttface;


    यदि ( !face )
      प्रत्यागम FT_THROW( Invalid_Face_Handle );

    यदि ( !FT_IS_SFNT( face ) )
      प्रत्यागम FT_Err_Ok;

    ttface = (TT_Face)face;

    ttface->foreground_color      = foreground_color;
    ttface->have_foreground_color = 1;

    प्रत्यागम FT_Err_Ok;
  }

#अन्यथा /* !TT_CONFIG_OPTION_COLOR_LAYERS */

  FT_EXPORT_DEF( FT_Error )
  FT_Palette_दत्त_Get( FT_Face           face,
                       FT_Palette_दत्त  *apalette_दत्त )
  {
    FT_अप्रयुक्तम्( face );
    FT_अप्रयुक्तम्( apalette_दत्त );


    प्रत्यागम FT_THROW( Unimplemented_Feature );
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Palette_Select( FT_Face     face,
                     FT_UShort   palette_सूचकांक,
                     FT_Color*  *apalette )
  {
    FT_अप्रयुक्तम्( face );
    FT_अप्रयुक्तम्( palette_सूचकांक );
    FT_अप्रयुक्तम्( apalette );


    प्रत्यागम FT_THROW( Unimplemented_Feature );
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Palette_समुच्चयः_Foreground_Color( FT_Face   face,
                                   FT_Color  foreground_color )
  {
    FT_अप्रयुक्तम्( face );
    FT_अप्रयुक्तम्( foreground_color );


    प्रत्यागम FT_THROW( Unimplemented_Feature );
  }

#यदि_अन्तः /* !TT_CONFIG_OPTION_COLOR_LAYERS */


/*अन्तः*/
