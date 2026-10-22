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


#شامل <freetype/internal/ftdebug.h>
#شامل <freetype/internal/ftobjs.h>
#شامل <freetype/internal/ftserv.h>
#شامل <freetype/internal/services/svpsinfo.h>


  /* documentation is in t1tables.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_PS_Font_Info( FT_Face          face,
                       PS_FontInfoRec*  afont_info )
  {
    FT_Error           error;
    FT_Service_PsInfo  service;


    اگر ( !face )
      بازگشت FT_THROW( Invalid_Face_Handle );

    اگر ( !afont_info )
      بازگشت FT_THROW( Invalid_Argument );

    FT_FACE_FIND_SERVICE( face, service, POSTSCRIPT_INFO );

    اگر ( service && service->ps_get_font_info )
      error = service->ps_get_font_info( face, afont_info );
    وگرنه
      error = FT_THROW( Invalid_Argument );

    بازگشت error;
  }


  /* documentation is in t1tables.h */

  FT_EXPORT_DEF( FT_Int )
  FT_Has_PS_Glyph_Names( FT_Face  face )
  {
    FT_Int             result = 0;
    FT_Service_PsInfo  service;


    اگر ( face )
    {
      FT_FACE_FIND_SERVICE( face, service, POSTSCRIPT_INFO );

      اگر ( service && service->ps_has_glyph_names )
        result = service->ps_has_glyph_names( face );
    }

    بازگشت result;
  }


  /* documentation is in t1tables.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_PS_Font_Private( FT_Face         face,
                          PS_PrivateRec*  afont_private )
  {
    FT_Error           error;
    FT_Service_PsInfo  service;


    اگر ( !face )
      بازگشت FT_THROW( Invalid_Face_Handle );

    اگر ( !afont_private )
      بازگشت FT_THROW( Invalid_Argument );

    FT_FACE_FIND_SERVICE( face, service, POSTSCRIPT_INFO );

    اگر ( service && service->ps_get_font_private )
      error = service->ps_get_font_private( face, afont_private );
    وگرنه
      error = FT_THROW( Invalid_Argument );

    بازگشت error;
  }


  /* documentation is in t1tables.h */

  FT_EXPORT_DEF( FT_Long )
  FT_Get_PS_Font_مقدار( FT_Face       face,
                        PS_Dict_کلیدها  کلید,
                        FT_UInt       idx,
                        تهی         *مقدار,
                        FT_Long       مقدار_len )
  {
    FT_Int             result  = 0;
    FT_Service_PsInfo  service = پوچ;


    اگر ( face )
    {
      FT_FACE_FIND_SERVICE( face, service, POSTSCRIPT_INFO );

      اگر ( service && service->ps_get_font_مقدار )
        result = service->ps_get_font_مقدار( face, کلید, idx,
                                             مقدار, مقدار_len );
    }

    بازگشت result;
  }


/* END */
