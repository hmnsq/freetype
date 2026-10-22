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
      أرجع FT_THROW( Invalid_دلیل );

    FT_FACE_پیدا_کړه_SERVICE( face, service, POSTSCRIPT_INFO );

    إذا ( service && service->ps_get_font_info )
      error = service->ps_get_font_info( face, afont_info );
    وإلا
      error = FT_THROW( Invalid_دلیل );

    أرجع error;
  }


  /* documentation is in t1tables.h */

  FT_EXPORT_DEF( FT_Int )
  FT_Has_PS_Glyph_نومونه( FT_Face  face )
  {
    FT_Int             پایله = 0;
    FT_Service_PsInfo  service;


    إذا ( face )
    {
      FT_FACE_پیدا_کړه_SERVICE( face, service, POSTSCRIPT_INFO );

      إذا ( service && service->ps_has_glyph_نومونه )
        پایله = service->ps_has_glyph_نومونه( face );
    }

    أرجع پایله;
  }


  /* documentation is in t1tables.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_PS_Font_شخصي( FT_Face         face,
                          PS_شخصي_Rec*  afont_شخصي )
  {
    FT_Error           error;
    FT_Service_PsInfo  service;


    إذا ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );

    إذا ( !afont_شخصي )
      أرجع FT_THROW( Invalid_دلیل );

    FT_FACE_پیدا_کړه_SERVICE( face, service, POSTSCRIPT_INFO );

    إذا ( service && service->ps_get_font_شخصي )
      error = service->ps_get_font_شخصي( face, afont_شخصي );
    وإلا
      error = FT_THROW( Invalid_دلیل );

    أرجع error;
  }


  /* documentation is in t1tables.h */

  FT_EXPORT_DEF( FT_Long )
  FT_Get_PS_Font_ارزښت( FT_Face       face,
                        PS_Dict_کلیګانې  کلی,
                        FT_UInt       idx,
                        فراغ         *ارزښت,
                        FT_Long       ارزښت_len )
  {
    FT_Int             پایله  = 0;
    FT_Service_PsInfo  service = عدم;


    إذا ( face )
    {
      FT_FACE_پیدا_کړه_SERVICE( face, service, POSTSCRIPT_INFO );

      إذا ( service && service->ps_get_font_ارزښت )
        پایله = service->ps_get_font_ارزښت( face, کلی, idx,
                                             ارزښت, ارزښت_len );
    }

    أرجع پایله;
  }


/*پای_ټکی*/
