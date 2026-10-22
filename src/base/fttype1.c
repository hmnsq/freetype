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


#அந்தர்பாவ <freetype/internal/ftdebug.h>
#அந்தர்பாவ <freetype/internal/ftobjs.h>
#அந்தர்பாவ <freetype/internal/ftserv.h>
#அந்தர்பாவ <freetype/internal/services/svpsinfo.h>


  /* documentation is in t1tables.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_PS_Font_Info( FT_Face          face,
                       PS_FontInfoRec*  afont_info )
  {
    FT_Error           error;
    FT_Service_PsInfo  service;


    எனில் ( !face )
      திரும்பு FT_THROW( Invalid_Face_Handle );

    எனில் ( !afont_info )
      திரும்பு FT_THROW( Invalid_Argument );

    FT_FACE_FIND_SERVICE( face, service, POSTSCRIPT_INFO );

    எனில் ( service && service->ps_get_font_info )
      error = service->ps_get_font_info( face, afont_info );
    இல்லையெனில்
      error = FT_THROW( Invalid_Argument );

    திரும்பு error;
  }


  /* documentation is in t1tables.h */

  FT_EXPORT_DEF( FT_Int )
  FT_Has_PS_Glyph_Names( FT_Face  face )
  {
    FT_Int             result = 0;
    FT_Service_PsInfo  service;


    எனில் ( face )
    {
      FT_FACE_FIND_SERVICE( face, service, POSTSCRIPT_INFO );

      எனில் ( service && service->ps_has_glyph_names )
        result = service->ps_has_glyph_names( face );
    }

    திரும்பு result;
  }


  /* documentation is in t1tables.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_PS_Font_Private( FT_Face         face,
                          PS_PrivateRec*  afont_private )
  {
    FT_Error           error;
    FT_Service_PsInfo  service;


    எனில் ( !face )
      திரும்பு FT_THROW( Invalid_Face_Handle );

    எனில் ( !afont_private )
      திரும்பு FT_THROW( Invalid_Argument );

    FT_FACE_FIND_SERVICE( face, service, POSTSCRIPT_INFO );

    எனில் ( service && service->ps_get_font_private )
      error = service->ps_get_font_private( face, afont_private );
    இல்லையெனில்
      error = FT_THROW( Invalid_Argument );

    திரும்பு error;
  }


  /* documentation is in t1tables.h */

  FT_EXPORT_DEF( FT_Long )
  FT_Get_PS_Font_மதிப்பு( FT_Face       face,
                        PS_Dict_சாவிகள்  சாவி,
                        FT_UInt       idx,
                        வெறுமை         *மதிப்பு,
                        FT_Long       மதிப்பு_len )
  {
    FT_Int             result  = 0;
    FT_Service_PsInfo  service = வெற்று;


    எனில் ( face )
    {
      FT_FACE_FIND_SERVICE( face, service, POSTSCRIPT_INFO );

      எனில் ( service && service->ps_get_font_மதிப்பு )
        result = service->ps_get_font_மதிப்பு( face, சாவி, idx,
                                             மதிப்பு, மதிப்பு_len );
    }

    திரும்பு result;
  }


/* END */
