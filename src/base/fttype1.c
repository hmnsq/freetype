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
      प्रत्यागम FT_THROW( Invalid_Face_Handle );

    यदि ( !afont_info )
      प्रत्यागम FT_THROW( Invalid_तर्कः );

    FT_FACE_FIND_SERVICE( face, service, POSTSCRIPT_INFO );

    यदि ( service && service->ps_get_font_info )
      error = service->ps_get_font_info( face, afont_info );
    अन्यथा
      error = FT_THROW( Invalid_तर्कः );

    प्रत्यागम error;
  }


  /* documentation is in t1tables.h */

  FT_EXPORT_DEF( FT_Int )
  FT_Has_PS_Glyph_नामानि( FT_Face  face )
  {
    FT_Int             परिणाम = 0;
    FT_Service_PsInfo  service;


    यदि ( face )
    {
      FT_FACE_FIND_SERVICE( face, service, POSTSCRIPT_INFO );

      यदि ( service && service->ps_has_glyph_नामानि )
        परिणाम = service->ps_has_glyph_नामानि( face );
    }

    प्रत्यागम परिणाम;
  }


  /* documentation is in t1tables.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_PS_Font_वैयक्तिकम्( FT_Face         face,
                          PS_वैयक्तिकम्_Rec*  afont_वैयक्तिकम् )
  {
    FT_Error           error;
    FT_Service_PsInfo  service;


    यदि ( !face )
      प्रत्यागम FT_THROW( Invalid_Face_Handle );

    यदि ( !afont_वैयक्तिकम् )
      प्रत्यागम FT_THROW( Invalid_तर्कः );

    FT_FACE_FIND_SERVICE( face, service, POSTSCRIPT_INFO );

    यदि ( service && service->ps_get_font_वैयक्तिकम् )
      error = service->ps_get_font_वैयक्तिकम्( face, afont_वैयक्तिकम् );
    अन्यथा
      error = FT_THROW( Invalid_तर्कः );

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
    FT_Int             परिणाम  = 0;
    FT_Service_PsInfo  service = रिक्त;


    यदि ( face )
    {
      FT_FACE_FIND_SERVICE( face, service, POSTSCRIPT_INFO );

      यदि ( service && service->ps_get_font_मानम् )
        परिणाम = service->ps_get_font_मानम्( face, कुञ्चिका, idx,
                                             मानम्, मानम्_len );
    }

    प्रत्यागम परिणाम;
  }


/*अन्तः*/
