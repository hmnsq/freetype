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


#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftdebug.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftobjs.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftserv.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/services/svpsinfo.h>


  /* documentation is in t1tables.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_PS_Font_Info( FT_Face          face,
                       PS_FontInfoRec*  afont_info )
  {
    FT_Error           error;
    FT_Service_PsInfo  service;


    ਜੇ ( !face )
      ਵਾਪਸ FT_THROW( Invalid_Face_Handle );

    ਜੇ ( !afont_info )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    FT_FACE_ਲੱਭੋ_SERVICE( face, service, POSTSCRIPT_INFO );

    ਜੇ ( service && service->ps_get_font_info )
      error = service->ps_get_font_info( face, afont_info );
    ਨਹੀਂਤਾਂ
      error = FT_THROW( Invalid_ਦਲੀਲ );

    ਵਾਪਸ error;
  }


  /* documentation is in t1tables.h */

  FT_EXPORT_DEF( FT_Int )
  FT_Has_PS_Glyph_ਨਾਂ( FT_Face  face )
  {
    FT_Int             ਨਤੀਜਾ = 0;
    FT_Service_PsInfo  service;


    ਜੇ ( face )
    {
      FT_FACE_ਲੱਭੋ_SERVICE( face, service, POSTSCRIPT_INFO );

      ਜੇ ( service && service->ps_has_glyph_ਨਾਂ )
        ਨਤੀਜਾ = service->ps_has_glyph_ਨਾਂ( face );
    }

    ਵਾਪਸ ਨਤੀਜਾ;
  }


  /* documentation is in t1tables.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_PS_Font_ਨਿੱਜੀ( FT_Face         face,
                          PS_ਨਿੱਜੀ_Rec*  afont_ਨਿੱਜੀ )
  {
    FT_Error           error;
    FT_Service_PsInfo  service;


    ਜੇ ( !face )
      ਵਾਪਸ FT_THROW( Invalid_Face_Handle );

    ਜੇ ( !afont_ਨਿੱਜੀ )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    FT_FACE_ਲੱਭੋ_SERVICE( face, service, POSTSCRIPT_INFO );

    ਜੇ ( service && service->ps_get_font_ਨਿੱਜੀ )
      error = service->ps_get_font_ਨਿੱਜੀ( face, afont_ਨਿੱਜੀ );
    ਨਹੀਂਤਾਂ
      error = FT_THROW( Invalid_ਦਲੀਲ );

    ਵਾਪਸ error;
  }


  /* documentation is in t1tables.h */

  FT_EXPORT_DEF( FT_Long )
  FT_Get_PS_Font_ਮੁੱਲ( FT_Face       face,
                        PS_Dict_ਕੁੰਜੀਆਂ  ਕੁੰਜੀ,
                        FT_UInt       idx,
                        ਖਾਲੀ         *ਮੁੱਲ,
                        FT_Long       ਮੁੱਲ_len )
  {
    FT_Int             ਨਤੀਜਾ  = 0;
    FT_Service_PsInfo  service = ਨਿਰਾ;


    ਜੇ ( face )
    {
      FT_FACE_ਲੱਭੋ_SERVICE( face, service, POSTSCRIPT_INFO );

      ਜੇ ( service && service->ps_get_font_ਮੁੱਲ )
        ਨਤੀਜਾ = service->ps_get_font_ਮੁੱਲ( face, ਕੁੰਜੀ, idx,
                                             ਮੁੱਲ, ਮੁੱਲ_len );
    }

    ਵਾਪਸ ਨਤੀਜਾ;
  }


/*ਸਿਰਾ*/
