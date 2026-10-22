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


#ಅನ್ತರ್ಭಾವ <freetype/internal/ftdebug.h>
#ಅನ್ತರ್ಭಾವ <freetype/internal/ftobjs.h>
#ಅನ್ತರ್ಭಾವ <freetype/internal/ftserv.h>
#ಅನ್ತರ್ಭಾವ <freetype/internal/services/svpsinfo.h>


  /* documentation is in t1tables.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_PS_Font_Info( FT_Face          face,
                       PS_FontInfoRec*  afont_info )
  {
    FT_Error           error;
    FT_Service_PsInfo  service;


    ಯದಿ ( !face )
      ಪ್ರತ್ಯಾಗಮ FT_THROW( Invalid_Face_Handle );

    ಯದಿ ( !afont_info )
      ಪ್ರತ್ಯಾಗಮ FT_THROW( Invalid_Argument );

    FT_FACE_FIND_SERVICE( face, service, POSTSCRIPT_INFO );

    ಯದಿ ( service && service->ps_get_font_info )
      error = service->ps_get_font_info( face, afont_info );
    ಅನ್ಯಥಾ
      error = FT_THROW( Invalid_Argument );

    ಪ್ರತ್ಯಾಗಮ error;
  }


  /* documentation is in t1tables.h */

  FT_EXPORT_DEF( FT_Int )
  FT_Has_PS_Glyph_Names( FT_Face  face )
  {
    FT_Int             result = 0;
    FT_Service_PsInfo  service;


    ಯದಿ ( face )
    {
      FT_FACE_FIND_SERVICE( face, service, POSTSCRIPT_INFO );

      ಯದಿ ( service && service->ps_has_glyph_names )
        result = service->ps_has_glyph_names( face );
    }

    ಪ್ರತ್ಯಾಗಮ result;
  }


  /* documentation is in t1tables.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_PS_Font_Private( FT_Face         face,
                          PS_PrivateRec*  afont_private )
  {
    FT_Error           error;
    FT_Service_PsInfo  service;


    ಯದಿ ( !face )
      ಪ್ರತ್ಯಾಗಮ FT_THROW( Invalid_Face_Handle );

    ಯದಿ ( !afont_private )
      ಪ್ರತ್ಯಾಗಮ FT_THROW( Invalid_Argument );

    FT_FACE_FIND_SERVICE( face, service, POSTSCRIPT_INFO );

    ಯದಿ ( service && service->ps_get_font_private )
      error = service->ps_get_font_private( face, afont_private );
    ಅನ್ಯಥಾ
      error = FT_THROW( Invalid_Argument );

    ಪ್ರತ್ಯಾಗಮ error;
  }


  /* documentation is in t1tables.h */

  FT_EXPORT_DEF( FT_Long )
  FT_Get_PS_Font_ಮೌಲ್ಯ( FT_Face       face,
                        PS_Dict_ಕೀಲಿಗಳು  ಕೀಲಿ,
                        FT_UInt       idx,
                        ಶೂನ್ಯ         *ಮೌಲ್ಯ,
                        FT_Long       ಮೌಲ್ಯ_len )
  {
    FT_Int             result  = 0;
    FT_Service_PsInfo  service = ರಿಕ್ತ;


    ಯದಿ ( face )
    {
      FT_FACE_FIND_SERVICE( face, service, POSTSCRIPT_INFO );

      ಯದಿ ( service && service->ps_get_font_ಮೌಲ್ಯ )
        result = service->ps_get_font_ಮೌಲ್ಯ( face, ಕೀಲಿ, idx,
                                             ಮೌಲ್ಯ, ಮೌಲ್ಯ_len );
    }

    ಪ್ರತ್ಯಾಗಮ result;
  }


/* END */
