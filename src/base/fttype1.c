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


    ಆದರೆ ( !face )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_Face_Handle );

    ಆದರೆ ( !afont_info )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

    FT_FACE_ಕಂಡುಹಿಡಿ_SERVICE( face, service, POSTSCRIPT_INFO );

    ಆದರೆ ( service && service->ps_get_font_info )
      error = service->ps_get_font_info( face, afont_info );
    ಇಲ್ಲದಿದ್ದರೆ
      error = FT_THROW( Invalid_ವಾದ );

    ಹಿಂತಿರುಗಿ error;
  }


  /* documentation is in t1tables.h */

  FT_EXPORT_DEF( FT_Int )
  FT_Has_PS_Glyph_ಹೆಸರುಗಳು( FT_Face  face )
  {
    FT_Int             ಫಲಿತಾಂಶ = 0;
    FT_Service_PsInfo  service;


    ಆದರೆ ( face )
    {
      FT_FACE_ಕಂಡುಹಿಡಿ_SERVICE( face, service, POSTSCRIPT_INFO );

      ಆದರೆ ( service && service->ps_has_glyph_ಹೆಸರುಗಳು )
        ಫಲಿತಾಂಶ = service->ps_has_glyph_ಹೆಸರುಗಳು( face );
    }

    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
  }


  /* documentation is in t1tables.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_PS_Font_ಖಾಸಗಿ( FT_Face         face,
                          PS_ಖಾಸಗಿ_Rec*  afont_ಖಾಸಗಿ )
  {
    FT_Error           error;
    FT_Service_PsInfo  service;


    ಆದರೆ ( !face )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_Face_Handle );

    ಆದರೆ ( !afont_ಖಾಸಗಿ )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

    FT_FACE_ಕಂಡುಹಿಡಿ_SERVICE( face, service, POSTSCRIPT_INFO );

    ಆದರೆ ( service && service->ps_get_font_ಖಾಸಗಿ )
      error = service->ps_get_font_ಖಾಸಗಿ( face, afont_ಖಾಸಗಿ );
    ಇಲ್ಲದಿದ್ದರೆ
      error = FT_THROW( Invalid_ವಾದ );

    ಹಿಂತಿರುಗಿ error;
  }


  /* documentation is in t1tables.h */

  FT_EXPORT_DEF( FT_Long )
  FT_Get_PS_Font_ಮೌಲ್ಯ( FT_Face       face,
                        PS_Dict_ಕೀಲಿಗಳು  ಕೀಲಿ,
                        FT_UInt       idx,
                        ಶೂನ್ಯ         *ಮೌಲ್ಯ,
                        FT_Long       ಮೌಲ್ಯ_len )
  {
    FT_Int             ಫಲಿತಾಂಶ  = 0;
    FT_Service_PsInfo  service = ಖಾಲಿ;


    ಆದರೆ ( face )
    {
      FT_FACE_ಕಂಡುಹಿಡಿ_SERVICE( face, service, POSTSCRIPT_INFO );

      ಆದರೆ ( service && service->ps_get_font_ಮೌಲ್ಯ )
        ಫಲಿತಾಂಶ = service->ps_get_font_ಮೌಲ್ಯ( face, ಕೀಲಿ, idx,
                                             ಮೌಲ್ಯ, ಮೌಲ್ಯ_len );
    }

    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
  }


/*ಕೊನೆ*/
