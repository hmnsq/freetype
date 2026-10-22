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


#অন্তর্ভাব <freetype/internal/ftdebug.h>
#অন্তর্ভাব <freetype/internal/sfnt.h>
#অন্তর্ভাব <freetype/internal/tttypes.h>
#অন্তর্ভাব <freetype/ftcolor.h>


#ifdef TT_CONFIG_OPTION_COLOR_LAYERS

  স্থির
  ধ্রুবক FT_Palette_তথ্য  null_palette_তথ্য = { 0, ফাঁকা, ফাঁকা, 0, ফাঁকা };


  /* documentation is in ftcolor.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Palette_তথ্য_Get( FT_Face           face,
                       FT_Palette_তথ্য  *apalette_তথ্য )
  {
    যদি ( !face )
      ফেরত FT_THROW( Invalid_Face_Handle );
    যদি ( !apalette_তথ্য)
      ফেরত FT_THROW( Invalid_Argument );

    যদি ( FT_IS_SFNT( face ) )
      *apalette_তথ্য = ( (TT_Face)face )->palette_তথ্য;
    অন্যথা
      *apalette_তথ্য = null_palette_তথ্য;

    ফেরত FT_Err_Ok;
  }


  /* documentation is in ftcolor.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Palette_Select( FT_Face     face,
                     FT_UShort   palette_সূচক,
                     FT_Color*  *apalette )
  {
    FT_Error  error;
    TT_Face   ttface = (TT_Face)face;


    যদি ( !face )
      ফেরত FT_THROW( Invalid_Face_Handle );

    যদি ( !FT_IS_SFNT( face ) )
    {
      যদি ( apalette )
        *apalette = ফাঁকা;

      ফেরত FT_Err_Ok;
    }

    যদি ( palette_সূচক != ttface->palette_সূচক )
    {
      SFNT_Service  sfnt = (SFNT_Service)ttface->sfnt;


      error = sfnt->set_palette( ttface, palette_সূচক );
      যদি ( error )
        ফেরত error;

      ttface->palette_সূচক = palette_সূচক;
    }

    যদি ( apalette )
      *apalette = ttface->palette;

    ফেরত FT_Err_Ok;
  }


  /* documentation is in ftcolor.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Palette_Set_Foreground_Color( FT_Face   face,
                                   FT_Color  foreground_color )
  {
    TT_Face  ttface;


    যদি ( !face )
      ফেরত FT_THROW( Invalid_Face_Handle );

    যদি ( !FT_IS_SFNT( face ) )
      ফেরত FT_Err_Ok;

    ttface = (TT_Face)face;

    ttface->foreground_color      = foreground_color;
    ttface->have_foreground_color = 1;

    ফেরত FT_Err_Ok;
  }

#অন্যথা /* !TT_CONFIG_OPTION_COLOR_LAYERS */

  FT_EXPORT_DEF( FT_Error )
  FT_Palette_তথ্য_Get( FT_Face           face,
                       FT_Palette_তথ্য  *apalette_তথ্য )
  {
    FT_UNUSED( face );
    FT_UNUSED( apalette_তথ্য );


    ফেরত FT_THROW( Unimplemented_Feature );
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Palette_Select( FT_Face     face,
                     FT_UShort   palette_সূচক,
                     FT_Color*  *apalette )
  {
    FT_UNUSED( face );
    FT_UNUSED( palette_সূচক );
    FT_UNUSED( apalette );


    ফেরত FT_THROW( Unimplemented_Feature );
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Palette_Set_Foreground_Color( FT_Face   face,
                                   FT_Color  foreground_color )
  {
    FT_UNUSED( face );
    FT_UNUSED( foreground_color );


    ফেরত FT_THROW( Unimplemented_Feature );
  }

#endif /* !TT_CONFIG_OPTION_COLOR_LAYERS */


/* END */
