/****************************************************************************
 *
 * fttype1.c
 *
 *   FreeType utility file for PS names support (body).
 *
 * Copyright (C) 2002-2026 by
 * David Turner, Robert Wilhelm, and Werner Lemberg.
 *
 * This file is part of the FreeType project, and may only be used,
 * modified, and distributed under the terms of the FreeType project
 * license, LICENSE.TXT.  By continuing to use, modify, or distribute
 * this file you indicate that you have read the license and
 * understand and accept it fully.
 *
 */


#ଅନ୍ତର୍ଭାଵ <freetype/internal/ftdebug.h>
#ଅନ୍ତର୍ଭାଵ <freetype/internal/ftobjs.h>
#ଅନ୍ତର୍ଭାଵ <freetype/internal/ftserv.h>
#ଅନ୍ତର୍ଭାଵ <freetype/internal/services/svpsinfo.h>


  /* documentation is in t1tables.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_PS_Font_Info( FT_Face          face,
                       PS_FontInfoRec*  afont_info )
  {
    FT_Error           error;
    FT_Service_PsInfo  service;


    ଯଦି ( !face )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Face_Handle );

    ଯଦି ( !afont_info )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Argument );

    FT_FACE_FIND_SERVICE( face, service, POSTSCRIPT_INFO );

    ଯଦି ( service && service->ps_get_font_info )
      error = service->ps_get_font_info( face, afont_info );
    ଅନ୍ୟଥା
      error = FT_THROW( Invalid_Argument );

    ଫେରନ୍ତୁ error;
  }


  /* documentation is in t1tables.h */

  FT_EXPORT_DEF( FT_Int )
  FT_Has_PS_Glyph_Names( FT_Face  face )
  {
    FT_Int             result = 0;
    FT_Service_PsInfo  service;


    ଯଦି ( face )
    {
      FT_FACE_FIND_SERVICE( face, service, POSTSCRIPT_INFO );

      ଯଦି ( service && service->ps_has_glyph_names )
        result = service->ps_has_glyph_names( face );
    }

    ଫେରନ୍ତୁ result;
  }


  /* documentation is in t1tables.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_PS_Font_Private( FT_Face         face,
                          PS_PrivateRec*  afont_private )
  {
    FT_Error           error;
    FT_Service_PsInfo  service;


    ଯଦି ( !face )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Face_Handle );

    ଯଦି ( !afont_private )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Argument );

    FT_FACE_FIND_SERVICE( face, service, POSTSCRIPT_INFO );

    ଯଦି ( service && service->ps_get_font_private )
      error = service->ps_get_font_private( face, afont_private );
    ଅନ୍ୟଥା
      error = FT_THROW( Invalid_Argument );

    ଫେରନ୍ତୁ error;
  }


  /* documentation is in t1tables.h */

  FT_EXPORT_DEF( FT_Long )
  FT_Get_PS_Font_ମୂଲ୍ୟ( FT_Face       face,
                        PS_Dict_ଚାବିଗୁଡ଼ିକ  ଚାବି,
                        FT_UInt       idx,
                        ଶୂନ୍ୟ         *ମୂଲ୍ୟ,
                        FT_Long       ମୂଲ୍ୟ_len )
  {
    FT_Int             result  = 0;
    FT_Service_PsInfo  service = ଖାଲି;


    ଯଦି ( face )
    {
      FT_FACE_FIND_SERVICE( face, service, POSTSCRIPT_INFO );

      ଯଦି ( service && service->ps_get_font_ମୂଲ୍ୟ )
        result = service->ps_get_font_ମୂଲ୍ୟ( face, ଚାବି, idx,
                                             ମୂଲ୍ୟ, ମୂଲ୍ୟ_len );
    }

    ଫେରନ୍ତୁ result;
  }


/* END */
