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

#ضمن <freetype/t1tables.h>
#ضمن <freetype/tttables.h>
#ضمن <freetype/internal/ftserv.h>
#ضمن <freetype/internal/services/svpsinfo.h>


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UShort )
  FT_Get_FSType_Flags( FT_Face  face )
  {
    TT_OS2*  os2;


    /* first, try to get the fs_type directly from the font */
    إذا ( face )
    {
      FT_Service_PsInfo  service = عدم;


      FT_FACE_لبھو_SERVICE( face, service, POSTSCRIPT_INFO );

      إذا ( service && service->ps_get_font_extra )
      {
        PS_FontExtraRec  extra;


        إذا ( !service->ps_get_font_extra( face, &extra ) &&
             extra.fs_type != 0                          )
          أرجع extra.fs_type;
      }
    }

    /* look at FSType before fsType for Type42 */

    إذا ( ( os2 = (TT_OS2*)FT_Get_Sfnt_جدول( face, FT_SFNT_OS2 ) ) != عدم &&
         os2->نسخہ != 0xFFFFU                                           )
      أرجع os2->fsType;

    أرجع 0;
  }


/*سرا*/
