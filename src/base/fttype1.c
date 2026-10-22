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


#समावेश <freetype/internal/ftdebug.h>
#समावेश <freetype/internal/ftobjs.h>
#समावेश <freetype/internal/ftserv.h>
#समावेश <freetype/internal/services/svpsinfo.h>


  /* documentation is in t1tables.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_PS_Font_Info( FT_Face          face,
                       PS_FontInfoRec*  afont_info )
  {
    FT_Error           error;
    FT_Service_PsInfo  service;


    अगर ( !face )
      वापस FT_THROW( Invalid_Face_Handle );

    अगर ( !afont_info )
      वापस FT_THROW( Invalid_Argument );

    FT_FACE_FIND_SERVICE( face, service, POSTSCRIPT_INFO );

    अगर ( service && service->ps_get_font_info )
      error = service->ps_get_font_info( face, afont_info );
    अन्यथा
      error = FT_THROW( Invalid_Argument );

    वापस error;
  }


  /* documentation is in t1tables.h */

  FT_EXPORT_DEF( FT_Int )
  FT_Has_PS_Glyph_Names( FT_Face  face )
  {
    FT_Int             परिणाम = 0;
    FT_Service_PsInfo  service;


    अगर ( face )
    {
      FT_FACE_FIND_SERVICE( face, service, POSTSCRIPT_INFO );

      अगर ( service && service->ps_has_glyph_names )
        परिणाम = service->ps_has_glyph_names( face );
    }

    वापस परिणाम;
  }


  /* documentation is in t1tables.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_PS_Font_Private( FT_Face         face,
                          PS_PrivateRec*  afont_private )
  {
    FT_Error           error;
    FT_Service_PsInfo  service;


    अगर ( !face )
      वापस FT_THROW( Invalid_Face_Handle );

    अगर ( !afont_private )
      वापस FT_THROW( Invalid_Argument );

    FT_FACE_FIND_SERVICE( face, service, POSTSCRIPT_INFO );

    अगर ( service && service->ps_get_font_private )
      error = service->ps_get_font_private( face, afont_private );
    अन्यथा
      error = FT_THROW( Invalid_Argument );

    वापस error;
  }


  /* documentation is in t1tables.h */

  FT_EXPORT_DEF( FT_Long )
  FT_Get_PS_Font_मूल्य( FT_Face       face,
                        PS_Dict_कुंजियाँ  कुंजी,
                        FT_UInt       idx,
                        व्योम         *मूल्य,
                        FT_Long       मूल्य_len )
  {
    FT_Int             परिणाम  = 0;
    FT_Service_PsInfo  service = शून्य;


    अगर ( face )
    {
      FT_FACE_FIND_SERVICE( face, service, POSTSCRIPT_INFO );

      अगर ( service && service->ps_get_font_मूल्य )
        परिणाम = service->ps_get_font_मूल्य( face, कुंजी, idx,
                                             मूल्य, मूल्य_len );
    }

    वापस परिणाम;
  }


/* END */
