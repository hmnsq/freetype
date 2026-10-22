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


#അന്തര്ഭാവ <freetype/internal/ftdebug.h>
#അന്തര്ഭാവ <freetype/internal/ftobjs.h>
#അന്തര്ഭാവ <freetype/internal/ftserv.h>
#അന്തര്ഭാവ <freetype/internal/services/svpsinfo.h>


  /* documentation is in t1tables.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_PS_Font_Info( FT_Face          face,
                       PS_FontInfoRec*  afont_info )
  {
    FT_Error           error;
    FT_Service_PsInfo  service;


    എങ്കിൽ ( !face )
      തിരികെ FT_THROW( Invalid_Face_Handle );

    എങ്കിൽ ( !afont_info )
      തിരികെ FT_THROW( Invalid_Argument );

    FT_FACE_FIND_SERVICE( face, service, POSTSCRIPT_INFO );

    എങ്കിൽ ( service && service->ps_get_font_info )
      error = service->ps_get_font_info( face, afont_info );
    അല്ലെങ്കിൽ
      error = FT_THROW( Invalid_Argument );

    തിരികെ error;
  }


  /* documentation is in t1tables.h */

  FT_EXPORT_DEF( FT_Int )
  FT_Has_PS_Glyph_Names( FT_Face  face )
  {
    FT_Int             result = 0;
    FT_Service_PsInfo  service;


    എങ്കിൽ ( face )
    {
      FT_FACE_FIND_SERVICE( face, service, POSTSCRIPT_INFO );

      എങ്കിൽ ( service && service->ps_has_glyph_names )
        result = service->ps_has_glyph_names( face );
    }

    തിരികെ result;
  }


  /* documentation is in t1tables.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_PS_Font_Private( FT_Face         face,
                          PS_PrivateRec*  afont_private )
  {
    FT_Error           error;
    FT_Service_PsInfo  service;


    എങ്കിൽ ( !face )
      തിരികെ FT_THROW( Invalid_Face_Handle );

    എങ്കിൽ ( !afont_private )
      തിരികെ FT_THROW( Invalid_Argument );

    FT_FACE_FIND_SERVICE( face, service, POSTSCRIPT_INFO );

    എങ്കിൽ ( service && service->ps_get_font_private )
      error = service->ps_get_font_private( face, afont_private );
    അല്ലെങ്കിൽ
      error = FT_THROW( Invalid_Argument );

    തിരികെ error;
  }


  /* documentation is in t1tables.h */

  FT_EXPORT_DEF( FT_Long )
  FT_Get_PS_Font_മൂല്യം( FT_Face       face,
                        PS_Dict_താക്കോലുകൾ  താക്കോൽ,
                        FT_UInt       idx,
                        ശൂന്യം         *മൂല്യം,
                        FT_Long       മൂല്യം_len )
  {
    FT_Int             result  = 0;
    FT_Service_PsInfo  service = ശൂന്യ;


    എങ്കിൽ ( face )
    {
      FT_FACE_FIND_SERVICE( face, service, POSTSCRIPT_INFO );

      എങ്കിൽ ( service && service->ps_get_font_മൂല്യം )
        result = service->ps_get_font_മൂല്യം( face, താക്കോൽ, idx,
                                             മൂല്യം, മൂല്യം_len );
    }

    തിരികെ result;
  }


/* END */
