/****************************************************************************
 *
 * ftwinfnt.c
 *
 *   FreeType API for accessing Windows FNT specific info (body).
 *
 * Copyright (C) 2003-2026 by
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
#ಅನ್ತರ್ಭಾವ <freetype/ftwinfnt.h>
#ಅನ್ತರ್ಭಾವ <freetype/internal/ftobjs.h>
#ಅನ್ತರ್ಭಾವ <freetype/internal/services/svwinfnt.h>


  /* documentation is in ftwinfnt.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Win_FNT_ಶೀರ್ಷಿಕೆ( FT_Face               face,
                        FT_Win_FNT_ಶೀರ್ಷಿಕೆ_Rec  *ಶೀರ್ಷಿಕೆ )
  {
    FT_Service_WinFnt  service;
    FT_Error           error;


    ಆದರೆ ( !face )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_Face_Handle );

    ಆದರೆ ( !ಶೀರ್ಷಿಕೆ )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

    FT_FACE_LOOKUP_SERVICE( face, service, WINFNT );

    ಆದರೆ ( service )
      error = service->get_ಶೀರ್ಷಿಕೆ( face, ಶೀರ್ಷಿಕೆ );
    ಇಲ್ಲದಿದ್ದರೆ
      error = FT_THROW( Invalid_ವಾದ );

    ಹಿಂತಿರುಗಿ error;
  }


/*ಕೊನೆ*/
