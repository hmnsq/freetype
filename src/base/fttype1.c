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


#include <freetype/internal/ftdebug.h>
#include <freetype/internal/ftobjs.h>
#include <freetype/internal/ftserv.h>
#include <freetype/internal/services/svpsinfo.h>


  /* documentation is in t1tables.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_PS_Font_Info( FT_Face          face,
                       PS_FontInfoRec*  afont_info )
  {
    FT_Error           error;
    FT_Service_PsInfo  service;


    如果 ( !face )
      返回 FT_THROW( Invalid_Face_Handle );

    如果 ( !afont_info )
      返回 FT_THROW( Invalid_Argument );

    FT_FACE_FIND_SERVICE( face, service, POSTSCRIPT_INFO );

    如果 ( service && service->ps_get_font_info )
      error = service->ps_get_font_info( face, afont_info );
    否则
      error = FT_THROW( Invalid_Argument );

    返回 error;
  }


  /* documentation is in t1tables.h */

  FT_EXPORT_DEF( FT_Int )
  FT_Has_PS_Glyph_Names( FT_Face  face )
  {
    FT_Int             result = 0;
    FT_Service_PsInfo  service;


    如果 ( face )
    {
      FT_FACE_FIND_SERVICE( face, service, POSTSCRIPT_INFO );

      如果 ( service && service->ps_has_glyph_names )
        result = service->ps_has_glyph_names( face );
    }

    返回 result;
  }


  /* documentation is in t1tables.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_PS_Font_Private( FT_Face         face,
                          PS_PrivateRec*  afont_private )
  {
    FT_Error           error;
    FT_Service_PsInfo  service;


    如果 ( !face )
      返回 FT_THROW( Invalid_Face_Handle );

    如果 ( !afont_private )
      返回 FT_THROW( Invalid_Argument );

    FT_FACE_FIND_SERVICE( face, service, POSTSCRIPT_INFO );

    如果 ( service && service->ps_get_font_private )
      error = service->ps_get_font_private( face, afont_private );
    否则
      error = FT_THROW( Invalid_Argument );

    返回 error;
  }


  /* documentation is in t1tables.h */

  FT_EXPORT_DEF( FT_Long )
  FT_Get_PS_Font_Value( FT_Face       face,
                        PS_Dict_Keys  key,
                        FT_UInt       idx,
                        void         *value,
                        FT_Long       value_len )
  {
    FT_Int             result  = 0;
    FT_Service_PsInfo  service = NULL;


    如果 ( face )
    {
      FT_FACE_FIND_SERVICE( face, service, POSTSCRIPT_INFO );

      如果 ( service && service->ps_get_font_value )
        result = service->ps_get_font_value( face, key, idx,
                                             value, value_len );
    }

    返回 result;
  }


/* END */
