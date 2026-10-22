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


#अन्तर्भाव <freetype/internal/ftdebug.h>
#अन्तर्भाव <freetype/ftwinfnt.h>
#अन्तर्भाव <freetype/internal/ftobjs.h>
#अन्तर्भाव <freetype/internal/services/svwinfnt.h>


  /* documentation is in ftwinfnt.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Win_FNT_शीर्षकम्( FT_Face               face,
                        FT_Win_FNT_शीर्षकम्_Rec  *शीर्षकम् )
  {
    FT_Service_WinFnt  service;
    FT_Error           error;


    यदि ( !face )
      प्रत्यागम FT_THROW( Invalid_Face_Handle );

    यदि ( !शीर्षकम् )
      प्रत्यागम FT_THROW( Invalid_तर्कः );

    FT_FACE_LOOKUP_SERVICE( face, service, WINFNT );

    यदि ( service )
      error = service->get_शीर्षकम्( face, शीर्षकम् );
    अन्यथा
      error = FT_THROW( Invalid_तर्कः );

    प्रत्यागम error;
  }


/*अन्तः*/
