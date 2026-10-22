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


#অন্তর্ভাব <freetype/internal/ftdebug.h>
#অন্তর্ভাব <freetype/internal/ftobjs.h>
#অন্তর্ভাব <freetype/internal/ftserv.h>
#অন্তর্ভাব <freetype/internal/services/svpsinfo.h>


  /* documentation is in t1tables.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_PS_Font_Info( FT_Face          face,
                       PS_FontInfoRec*  afont_info )
  {
    FT_Error           error;
    FT_Service_PsInfo  service;


    যদি ( !face )
      ফেরত FT_THROW( Invalid_Face_Handle );

    যদি ( !afont_info )
      ফেরত FT_THROW( Invalid_Argument );

    FT_FACE_FIND_SERVICE( face, service, POSTSCRIPT_INFO );

    যদি ( service && service->ps_get_font_info )
      error = service->ps_get_font_info( face, afont_info );
    অন্যথা
      error = FT_THROW( Invalid_Argument );

    ফেরত error;
  }


  /* documentation is in t1tables.h */

  FT_EXPORT_DEF( FT_Int )
  FT_Has_PS_Glyph_Names( FT_Face  face )
  {
    FT_Int             ফলাফল = 0;
    FT_Service_PsInfo  service;


    যদি ( face )
    {
      FT_FACE_FIND_SERVICE( face, service, POSTSCRIPT_INFO );

      যদি ( service && service->ps_has_glyph_names )
        ফলাফল = service->ps_has_glyph_names( face );
    }

    ফেরত ফলাফল;
  }


  /* documentation is in t1tables.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_PS_Font_Private( FT_Face         face,
                          PS_PrivateRec*  afont_private )
  {
    FT_Error           error;
    FT_Service_PsInfo  service;


    যদি ( !face )
      ফেরত FT_THROW( Invalid_Face_Handle );

    যদি ( !afont_private )
      ফেরত FT_THROW( Invalid_Argument );

    FT_FACE_FIND_SERVICE( face, service, POSTSCRIPT_INFO );

    যদি ( service && service->ps_get_font_private )
      error = service->ps_get_font_private( face, afont_private );
    অন্যথা
      error = FT_THROW( Invalid_Argument );

    ফেরত error;
  }


  /* documentation is in t1tables.h */

  FT_EXPORT_DEF( FT_Long )
  FT_Get_PS_Font_মান( FT_Face       face,
                        PS_Dict_চাবিগুলি  চাবি,
                        FT_UInt       idx,
                        শূন্য         *মান,
                        FT_Long       মান_len )
  {
    FT_Int             ফলাফল  = 0;
    FT_Service_PsInfo  service = ফাঁকা;


    যদি ( face )
    {
      FT_FACE_FIND_SERVICE( face, service, POSTSCRIPT_INFO );

      যদি ( service && service->ps_get_font_মান )
        ফলাফল = service->ps_get_font_মান( face, চাবি, idx,
                                             মান, মান_len );
    }

    ফেরত ফলাফল;
  }


/* END */
