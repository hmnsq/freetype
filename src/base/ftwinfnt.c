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
  FT_Get_WinFNT_Header( FT_Face               face,
                        FT_WinFNT_HeaderRec  *header )
  {
    FT_Service_WinFnt  service;
    FT_Error           error;


    ಯದಿ ( !face )
      ಪ್ರತ್ಯಾಗಮ FT_THROW( Invalid_Face_Handle );

    ಯದಿ ( !header )
      ಪ್ರತ್ಯಾಗಮ FT_THROW( Invalid_Argument );

    FT_FACE_LOOKUP_SERVICE( face, service, WINFNT );

    ಯದಿ ( service )
      error = service->get_header( face, header );
    ಅನ್ಯಥಾ
      error = FT_THROW( Invalid_Argument );

    ಪ್ರತ್ಯಾಗಮ error;
  }


/* END */
