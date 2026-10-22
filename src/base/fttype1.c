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


    जइ ( !face )
      प्रत्यागम FT_THROW( Invalid_Face_Handle );

    जइ ( !afont_info )
      प्रत्यागम FT_THROW( Invalid_Argument );

    FT_FACE_FIND_SERVICE( face, service, POSTSCRIPT_INFO );

    जइ ( service && service->ps_get_font_info )
      error = service->ps_get_font_info( face, afont_info );
    अण्णहा
      error = FT_THROW( Invalid_Argument );

    प्रत्यागम error;
  }


  /* documentation is in t1tables.h */

  FT_EXPORT_DEF( FT_Int )
  FT_Has_PS_Glyph_Names( FT_Face  face )
  {
    FT_Int             result = 0;
    FT_Service_PsInfo  service;


    जइ ( face )
    {
      FT_FACE_FIND_SERVICE( face, service, POSTSCRIPT_INFO );

      जइ ( service && service->ps_has_glyph_names )
        result = service->ps_has_glyph_names( face );
    }

    प्रत्यागम result;
  }


  /* documentation is in t1tables.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_PS_Font_Private( FT_Face         face,
                          PS_PrivateRec*  afont_private )
  {
    FT_Error           error;
    FT_Service_PsInfo  service;


    जइ ( !face )
      प्रत्यागम FT_THROW( Invalid_Face_Handle );

    जइ ( !afont_private )
      प्रत्यागम FT_THROW( Invalid_Argument );

    FT_FACE_FIND_SERVICE( face, service, POSTSCRIPT_INFO );

    जइ ( service && service->ps_get_font_private )
      error = service->ps_get_font_private( face, afont_private );
    अण्णहा
      error = FT_THROW( Invalid_Argument );

    प्रत्यागम error;
  }


  /* documentation is in t1tables.h */

  FT_EXPORT_DEF( FT_Long )
  FT_Get_PS_Font_मानम्( FT_Face       face,
                        PS_Dict_कुञ्चिकाः  कुञ्चिका,
                        FT_UInt       idx,
                        शून्य         *मानम्,
                        FT_Long       मानम्_len )
  {
    FT_Int             result  = 0;
    FT_Service_PsInfo  service = रिक्त;


    जइ ( face )
    {
      FT_FACE_FIND_SERVICE( face, service, POSTSCRIPT_INFO );

      जइ ( service && service->ps_get_font_मानम् )
        result = service->ps_get_font_मानम्( face, कुञ्चिका, idx,
                                             मानम्, मानम्_len );
    }

    प्रत्यागम result;
  }


/* END */
