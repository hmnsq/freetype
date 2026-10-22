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


#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftdebug.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/ftwinfnt.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftobjs.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/services/svwinfnt.h>


  /* documentation is in ftwinfnt.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Win_FNT_ਸਿਰਲੇਖ( FT_Face               face,
                        FT_Win_FNT_ਸਿਰਲੇਖ_Rec  *ਸਿਰਲੇਖ )
  {
    FT_Service_WinFnt  service;
    FT_Error           error;


    ਜੇ ( !face )
      ਵਾਪਸ FT_THROW( Invalid_Face_Handle );

    ਜੇ ( !ਸਿਰਲੇਖ )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    FT_FACE_LOOKUP_SERVICE( face, service, WINFNT );

    ਜੇ ( service )
      error = service->get_ਸਿਰਲੇਖ( face, ਸਿਰਲੇਖ );
    ਨਹੀਂਤਾਂ
      error = FT_THROW( Invalid_ਦਲੀਲ );

    ਵਾਪਸ error;
  }


/*ਸਿਰਾ*/
