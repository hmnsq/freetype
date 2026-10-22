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


#ضمن <freetype/internal/ftdebug.h>
#ضمن <freetype/internal/ftobjs.h>
#ضمن <freetype/internal/ftserv.h>
#ضمن <freetype/internal/services/svpsinfo.h>


  /* documentation is in t1tables.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_PS_Font_Info( FT_Face          face,
                       PS_FontInfoRec*  afont_info )
  {
    FT_Error           error;
    FT_Service_PsInfo  service;


    إذا ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );

    إذا ( !afont_info )
      أرجع FT_THROW( Invalid_Argument );

    FT_FACE_FIND_SERVICE( face, service, POSTSCRIPT_INFO );

    إذا ( service && service->ps_get_font_info )
      error = service->ps_get_font_info( face, afont_info );
    وإلا
      error = FT_THROW( Invalid_Argument );

    أرجع error;
  }


  /* documentation is in t1tables.h */

  FT_EXPORT_DEF( FT_Int )
  FT_Has_PS_Glyph_Names( FT_Face  face )
  {
    FT_Int             result = 0;
    FT_Service_PsInfo  service;


    إذا ( face )
    {
      FT_FACE_FIND_SERVICE( face, service, POSTSCRIPT_INFO );

      إذا ( service && service->ps_has_glyph_names )
        result = service->ps_has_glyph_names( face );
    }

    أرجع result;
  }


  /* documentation is in t1tables.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_PS_Font_Private( FT_Face         face,
                          PS_PrivateRec*  afont_private )
  {
    FT_Error           error;
    FT_Service_PsInfo  service;


    إذا ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );

    إذا ( !afont_private )
      أرجع FT_THROW( Invalid_Argument );

    FT_FACE_FIND_SERVICE( face, service, POSTSCRIPT_INFO );

    إذا ( service && service->ps_get_font_private )
      error = service->ps_get_font_private( face, afont_private );
    وإلا
      error = FT_THROW( Invalid_Argument );

    أرجع error;
  }


  /* documentation is in t1tables.h */

  FT_EXPORT_DEF( FT_Long )
  FT_Get_PS_Font_قيمة( FT_Face       face,
                        PS_Dict_مفاتيح  مفتاح,
                        FT_UInt       idx,
                        فراغ         *قيمة,
                        FT_Long       قيمة_len )
  {
    FT_Int             result  = 0;
    FT_Service_PsInfo  service = عدم;


    إذا ( face )
    {
      FT_FACE_FIND_SERVICE( face, service, POSTSCRIPT_INFO );

      إذا ( service && service->ps_get_font_قيمة )
        result = service->ps_get_font_قيمة( face, مفتاح, idx,
                                             قيمة, قيمة_len );
    }

    أرجع result;
  }


/* END */
