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


#అన్తర్భావ <freetype/internal/ftdebug.h>
#అన్తర్భావ <freetype/internal/sfnt.h>
#అన్తర్భావ <freetype/internal/tttypes.h>
#అన్తర్భావ <freetype/ftcolor.h>


#నిర్వచితమైతే TT_CONFIG_OPTION_COLOR_LAYERS

  స్థిర
  స్థిరరాశి FT_Palette_దత్తాంశం  null_palette_దత్తాంశం = { 0, ఖాళీ, ఖాళీ, 0, ఖాళీ };


  /* documentation is in ftcolor.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Palette_దత్తాంశం_Get( FT_Face           face,
                       FT_Palette_దత్తాంశం  *apalette_దత్తాంశం )
  {
    ఒకవేళ ( !face )
      తిరిగి FT_THROW( Invalid_Face_Handle );
    ఒకవేళ ( !apalette_దత్తాంశం)
      తిరిగి FT_THROW( Invalid_వాదం );

    ఒకవేళ ( FT_IS_SFNT( face ) )
      *apalette_దత్తాంశం = ( (TT_Face)face )->palette_దత్తాంశం;
    లేకపోతే
      *apalette_దత్తాంశం = null_palette_దత్తాంశం;

    తిరిగి FT_Err_Ok;
  }


  /* documentation is in ftcolor.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Palette_Select( FT_Face     face,
                     FT_UShort   palette_సూచీ,
                     FT_Color*  *apalette )
  {
    FT_Error  error;
    TT_Face   ttface = (TT_Face)face;


    ఒకవేళ ( !face )
      తిరిగి FT_THROW( Invalid_Face_Handle );

    ఒకవేళ ( !FT_IS_SFNT( face ) )
    {
      ఒకవేళ ( apalette )
        *apalette = ఖాళీ;

      తిరిగి FT_Err_Ok;
    }

    ఒకవేళ ( palette_సూచీ != ttface->palette_సూచీ )
    {
      SFNT_Service  sfnt = (SFNT_Service)ttface->sfnt;


      error = sfnt->సమితి_palette( ttface, palette_సూచీ );
      ఒకవేళ ( error )
        తిరిగి error;

      ttface->palette_సూచీ = palette_సూచీ;
    }

    ఒకవేళ ( apalette )
      *apalette = ttface->palette;

    తిరిగి FT_Err_Ok;
  }


  /* documentation is in ftcolor.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Palette_సమితి_Foreground_Color( FT_Face   face,
                                   FT_Color  foreground_color )
  {
    TT_Face  ttface;


    ఒకవేళ ( !face )
      తిరిగి FT_THROW( Invalid_Face_Handle );

    ఒకవేళ ( !FT_IS_SFNT( face ) )
      తిరిగి FT_Err_Ok;

    ttface = (TT_Face)face;

    ttface->foreground_color      = foreground_color;
    ttface->have_foreground_color = 1;

    తిరిగి FT_Err_Ok;
  }

#లేకపోతే /* !TT_CONFIG_OPTION_COLOR_LAYERS */

  FT_EXPORT_DEF( FT_Error )
  FT_Palette_దత్తాంశం_Get( FT_Face           face,
                       FT_Palette_దత్తాంశం  *apalette_దత్తాంశం )
  {
    FT_వాడని( face );
    FT_వాడని( apalette_దత్తాంశం );


    తిరిగి FT_THROW( Unimplemented_Feature );
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Palette_Select( FT_Face     face,
                     FT_UShort   palette_సూచీ,
                     FT_Color*  *apalette )
  {
    FT_వాడని( face );
    FT_వాడని( palette_సూచీ );
    FT_వాడని( apalette );


    తిరిగి FT_THROW( Unimplemented_Feature );
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Palette_సమితి_Foreground_Color( FT_Face   face,
                                   FT_Color  foreground_color )
  {
    FT_వాడని( face );
    FT_వాడని( foreground_color );


    తిరిగి FT_THROW( Unimplemented_Feature );
  }

#అయితే_ముగింపు /* !TT_CONFIG_OPTION_COLOR_LAYERS */


/*చివర*/
