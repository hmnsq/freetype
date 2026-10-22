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


#ضمن <freetype/internal/ftdebug.h>
#ضمن <freetype/ftwinfnt.h>
#ضمن <freetype/internal/ftobjs.h>
#ضمن <freetype/internal/services/svwinfnt.h>


  /* documentation is in ftwinfnt.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Win_FNT_سرو( FT_Face               face,
                        FT_Win_FNT_سرو_Rec  *سرو )
  {
    FT_Service_WinFnt  service;
    FT_Error           error;


    إذا ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );

    إذا ( !سرو )
      أرجع FT_THROW( Invalid_دليل );

    FT_FACE_LOOKUP_SERVICE( face, service, WINFNT );

    إذا ( service )
      error = service->get_سرو( face, سرو );
    وإلا
      error = FT_THROW( Invalid_دليل );

    أرجع error;
  }


/*پڇاڙي*/
