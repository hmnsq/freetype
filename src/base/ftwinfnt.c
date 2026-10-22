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


#شامل <freetype/internal/ftdebug.h>
#شامل <freetype/ftwinfnt.h>
#شامل <freetype/internal/ftobjs.h>
#شامل <freetype/internal/services/svwinfnt.h>


  /* documentation is in ftwinfnt.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Win_FNT_سرخی( FT_Face               face,
                        FT_Win_FNT_سرخی_Rec  *سرخی )
  {
    FT_Service_WinFnt  service;
    FT_Error           error;


    اگر ( !face )
      واپس FT_THROW( Invalid_Face_Handle );

    اگر ( !سرخی )
      واپس FT_THROW( Invalid_دلیل );

    FT_FACE_LOOKUP_SERVICE( face, service, WINFNT );

    اگر ( service )
      error = service->get_سرخی( face, سرخی );
    ورنہ
      error = FT_THROW( Invalid_دلیل );

    واپس error;
  }


/*انتہا*/
