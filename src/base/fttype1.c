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


#अन्तर्भाव <freetype/internal/ftdebug.h>
#अन्तर्भाव <freetype/internal/ftobjs.h>
#अन्तर्भाव <freetype/internal/ftserv.h>
#अन्तर्भाव <freetype/internal/services/svpsinfo.h>


  /* documentation is in t1tables.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_PS_Font_Info( FT_Face          face,
                       PS_FontInfoRec*  afont_info )
  {
    FT_Error           error;
    FT_Service_PsInfo  service;


    यदि ( !face )
      फर्काऊ FT_THROW( Invalid_Face_Handle );

    यदि ( !afont_info )
      फर्काऊ FT_THROW( Invalid_Argument );

    FT_FACE_FIND_SERVICE( face, service, POSTSCRIPT_INFO );

    यदि ( service && service->ps_get_font_info )
      error = service->ps_get_font_info( face, afont_info );
    अन्यथा
      error = FT_THROW( Invalid_Argument );

    फर्काऊ error;
  }


  /* documentation is in t1tables.h */

  FT_EXPORT_DEF( FT_Int )
  FT_Has_PS_Glyph_Names( FT_Face  face )
  {
    FT_Int             result = 0;
    FT_Service_PsInfo  service;


    यदि ( face )
    {
      FT_FACE_FIND_SERVICE( face, service, POSTSCRIPT_INFO );

      यदि ( service && service->ps_has_glyph_names )
        result = service->ps_has_glyph_names( face );
    }

    फर्काऊ result;
  }


  /* documentation is in t1tables.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_PS_Font_Private( FT_Face         face,
                          PS_PrivateRec*  afont_private )
  {
    FT_Error           error;
    FT_Service_PsInfo  service;


    यदि ( !face )
      फर्काऊ FT_THROW( Invalid_Face_Handle );

    यदि ( !afont_private )
      फर्काऊ FT_THROW( Invalid_Argument );

    FT_FACE_FIND_SERVICE( face, service, POSTSCRIPT_INFO );

    यदि ( service && service->ps_get_font_private )
      error = service->ps_get_font_private( face, afont_private );
    अन्यथा
      error = FT_THROW( Invalid_Argument );

    फर्काऊ error;
  }


  /* documentation is in t1tables.h */

  FT_EXPORT_DEF( FT_Long )
  FT_Get_PS_Font_मान( FT_Face       face,
                        PS_Dict_साँचोहरू  साँचो,
                        FT_UInt       idx,
                        शून्य         *मान,
                        FT_Long       मान_len )
  {
    FT_Int             result  = 0;
    FT_Service_PsInfo  service = रिक्त;


    यदि ( face )
    {
      FT_FACE_FIND_SERVICE( face, service, POSTSCRIPT_INFO );

      यदि ( service && service->ps_get_font_मान )
        result = service->ps_get_font_मान( face, साँचो, idx,
                                             मान, मान_len );
    }

    फर्काऊ result;
  }


/* END */
