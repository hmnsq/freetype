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


#ضمن <freetype/internal/ftdebug.h>
#ضمن <freetype/internal/sfnt.h>
#ضمن <freetype/internal/tttypes.h>
#ضمن <freetype/ftcolor.h>


#اگر_تعریف_شده TT_CONFIG_OPTION_COLOR_LAYERS

  ساكن
  ثابت FT_Palette_داده  null_palette_داده = { 0, عدم, عدم, 0, عدم };


  /* documentation is in ftcolor.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Palette_داده_Get( FT_Face           face,
                       FT_Palette_داده  *apalette_داده )
  {
    إذا ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );
    إذا ( !apalette_داده)
      أرجع FT_THROW( Invalid_آرگومان );

    إذا ( FT_IS_SFNT( face ) )
      *apalette_داده = ( (TT_Face)face )->palette_داده;
    وإلا
      *apalette_داده = null_palette_داده;

    أرجع FT_Err_Ok;
  }


  /* documentation is in ftcolor.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Palette_Select( FT_Face     face,
                     FT_UShort   palette_شاخص,
                     FT_Color*  *apalette )
  {
    FT_Error  error;
    TT_Face   ttface = (TT_Face)face;


    إذا ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );

    إذا ( !FT_IS_SFNT( face ) )
    {
      إذا ( apalette )
        *apalette = عدم;

      أرجع FT_Err_Ok;
    }

    إذا ( palette_شاخص != ttface->palette_شاخص )
    {
      SFNT_Service  sfnt = (SFNT_Service)ttface->sfnt;


      error = sfnt->مجموعه_palette( ttface, palette_شاخص );
      إذا ( error )
        أرجع error;

      ttface->palette_شاخص = palette_شاخص;
    }

    إذا ( apalette )
      *apalette = ttface->palette;

    أرجع FT_Err_Ok;
  }


  /* documentation is in ftcolor.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Palette_مجموعه_Foreground_Color( FT_Face   face,
                                   FT_Color  foreground_color )
  {
    TT_Face  ttface;


    إذا ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );

    إذا ( !FT_IS_SFNT( face ) )
      أرجع FT_Err_Ok;

    ttface = (TT_Face)face;

    ttface->foreground_color      = foreground_color;
    ttface->have_foreground_color = 1;

    أرجع FT_Err_Ok;
  }

#وإلا /* !TT_CONFIG_OPTION_COLOR_LAYERS */

  FT_EXPORT_DEF( FT_Error )
  FT_Palette_داده_Get( FT_Face           face,
                       FT_Palette_داده  *apalette_داده )
  {
    FT_استفادهنشده( face );
    FT_استفادهنشده( apalette_داده );


    أرجع FT_THROW( Unimplemented_Feature );
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Palette_Select( FT_Face     face,
                     FT_UShort   palette_شاخص,
                     FT_Color*  *apalette )
  {
    FT_استفادهنشده( face );
    FT_استفادهنشده( palette_شاخص );
    FT_استفادهنشده( apalette );


    أرجع FT_THROW( Unimplemented_Feature );
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Palette_مجموعه_Foreground_Color( FT_Face   face,
                                   FT_Color  foreground_color )
  {
    FT_استفادهنشده( face );
    FT_استفادهنشده( foreground_color );


    أرجع FT_THROW( Unimplemented_Feature );
  }

#پایان_اگر /* !TT_CONFIG_OPTION_COLOR_LAYERS */


/*انتها*/
