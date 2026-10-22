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

#অন্তর্ভাব <freetype/t1tables.h>
#অন্তর্ভাব <freetype/tttables.h>
#অন্তর্ভাব <freetype/internal/ftserv.h>
#অন্তর্ভাব <freetype/internal/services/svpsinfo.h>


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UShort )
  FT_Get_FSType_Flags( FT_Face  face )
  {
    TT_OS2*  os2;


    /* first, try to get the fs_type directly from the font */
    যদি ( face )
    {
      FT_Service_PsInfo  service = ফাঁকা;


      FT_FACE_FIND_SERVICE( face, service, POSTSCRIPT_INFO );

      যদি ( service && service->ps_get_font_extra )
      {
        PS_FontExtraRec  extra;


        যদি ( !service->ps_get_font_extra( face, &extra ) &&
             extra.fs_type != 0                          )
          ফেরত extra.fs_type;
      }
    }

    /* look at FSType before fsType for Type42 */

    যদি ( ( os2 = (TT_OS2*)FT_Get_Sfnt_Table( face, FT_SFNT_OS2 ) ) != ফাঁকা &&
         os2->version != 0xFFFFU                                           )
      ফেরত os2->fsType;

    ফেরত 0;
  }


/* END */
