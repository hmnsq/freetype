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


#అన్తర్భావ <freetype/internal/ftdebug.h>
#అన్తర్భావ <freetype/ftwinfnt.h>
#అన్తర్భావ <freetype/internal/ftobjs.h>
#అన్తర్భావ <freetype/internal/services/svwinfnt.h>


  /* documentation is in ftwinfnt.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Win_FNT_శీర్షిక( FT_Face               face,
                        FT_Win_FNT_శీర్షిక_Rec  *శీర్షిక )
  {
    FT_Service_WinFnt  service;
    FT_Error           error;


    ఒకవేళ ( !face )
      తిరిగి FT_THROW( Invalid_Face_Handle );

    ఒకవేళ ( !శీర్షిక )
      తిరిగి FT_THROW( Invalid_వాదం );

    FT_FACE_LOOKUP_SERVICE( face, service, WINFNT );

    ఒకవేళ ( service )
      error = service->get_శీర్షిక( face, శీర్షిక );
    లేకపోతే
      error = FT_THROW( Invalid_వాదం );

    తిరిగి error;
  }


/*చివర*/
