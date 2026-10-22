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
      واپس FT_THROW( Invalid_Face_Handle );

    اگر ( !afont_info )
      واپس FT_THROW( Invalid_دلیل );

    FT_FACE_ڈھونڈو_SERVICE( face, service, POSTSCRIPT_INFO );

    اگر ( service && service->ps_get_font_info )
      error = service->ps_get_font_info( face, afont_info );
    ورنہ
      error = FT_THROW( Invalid_دلیل );

    واپس error;
  }


  /* documentation is in t1tables.h */

  FT_EXPORT_DEF( FT_Int )
  FT_Has_PS_Glyph_نام( FT_Face  face )
  {
    FT_Int             نتیجہ = 0;
    FT_Service_PsInfo  service;


    اگر ( face )
    {
      FT_FACE_ڈھونڈو_SERVICE( face, service, POSTSCRIPT_INFO );

      اگر ( service && service->ps_has_glyph_نام )
        نتیجہ = service->ps_has_glyph_نام( face );
    }

    واپس نتیجہ;
  }


  /* documentation is in t1tables.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_PS_Font_نجی( FT_Face         face,
                          PS_نجی_Rec*  afont_نجی )
  {
    FT_Error           error;
    FT_Service_PsInfo  service;


    اگر ( !face )
      واپس FT_THROW( Invalid_Face_Handle );

    اگر ( !afont_نجی )
      واپس FT_THROW( Invalid_دلیل );

    FT_FACE_ڈھونڈو_SERVICE( face, service, POSTSCRIPT_INFO );

    اگر ( service && service->ps_get_font_نجی )
      error = service->ps_get_font_نجی( face, afont_نجی );
    ورنہ
      error = FT_THROW( Invalid_دلیل );

    واپس error;
  }


  /* documentation is in t1tables.h */

  FT_EXPORT_DEF( FT_Long )
  FT_Get_PS_Font_قدر( FT_Face       face,
                        PS_Dict_کلیدیں  کلید,
                        FT_UInt       idx,
                        خالی         *قدر,
                        FT_Long       قدر_len )
  {
    FT_Int             نتیجہ  = 0;
    FT_Service_PsInfo  service = تہی;


    اگر ( face )
    {
      FT_FACE_ڈھونڈو_SERVICE( face, service, POSTSCRIPT_INFO );

      اگر ( service && service->ps_get_font_قدر )
        نتیجہ = service->ps_get_font_قدر( face, کلید, idx,
                                             قدر, قدر_len );
    }

    واپس نتیجہ;
  }


/*انتہا*/
