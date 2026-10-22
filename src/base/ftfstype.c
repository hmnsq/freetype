/****************************************************************************
 *
 * ftfstype.c
 *
 *   FreeType utility file to access FSType data (body).
 *
 * Copyright (C) 2008-2026 by
 * David Turner, Robert Wilhelm, and Werner Lemberg.
 *
 * This file is part of the FreeType project, and may only be used,
 * modified, and distributed under the terms of the FreeType project
 * license, LICENSE.TXT.  By continuing to use, modify, or distribute
 * this file you indicate that you have read the license and
 * understand and accept it fully.
 *
 */

#ಅನ್ತರ್ಭಾವ <freetype/t1tables.h>
#ಅನ್ತರ್ಭಾವ <freetype/tttables.h>
#ಅನ್ತರ್ಭಾವ <freetype/internal/ftserv.h>
#ಅನ್ತರ್ಭಾವ <freetype/internal/services/svpsinfo.h>


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UShort )
  FT_Get_FSType_Flags( FT_Face  face )
  {
    TT_OS2*  os2;


    /* first, try to get the fs_type directly from the font */
    ಆದರೆ ( face )
    {
      FT_Service_PsInfo  service = ಖಾಲಿ;


      FT_FACE_ಕಂಡುಹಿಡಿ_SERVICE( face, service, POSTSCRIPT_INFO );

      ಆದರೆ ( service && service->ps_get_font_extra )
      {
        PS_FontExtraRec  extra;


        ಆದರೆ ( !service->ps_get_font_extra( face, &extra ) &&
             extra.fs_type != 0                          )
          ಹಿಂತಿರುಗಿ extra.fs_type;
      }
    }

    /* look at FSType before fsType for Type42 */

    ಆದರೆ ( ( os2 = (TT_OS2*)FT_Get_Sfnt_ಕೋಷ್ಟಕ( face, FT_SFNT_OS2 ) ) != ಖಾಲಿ &&
         os2->ಆವೃತ್ತಿ != 0xFFFFU                                           )
      ಹಿಂತಿರುಗಿ os2->fsType;

    ಹಿಂತಿರುಗಿ 0;
  }


/*ಕೊನೆ*/
