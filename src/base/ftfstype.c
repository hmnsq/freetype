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

#ଅନ୍ତର୍ଭାଵ <freetype/t1tables.h>
#ଅନ୍ତର୍ଭାଵ <freetype/tttables.h>
#ଅନ୍ତର୍ଭାଵ <freetype/internal/ftserv.h>
#ଅନ୍ତର୍ଭାଵ <freetype/internal/services/svpsinfo.h>


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UShort )
  FT_Get_FSType_Flags( FT_Face  face )
  {
    TT_OS2*  os2;


    /* first, try to get the fs_type directly from the font */
    ଯଦି ( face )
    {
      FT_Service_PsInfo  service = ଖାଲି;


      FT_FACE_FIND_SERVICE( face, service, POSTSCRIPT_INFO );

      ଯଦି ( service && service->ps_get_font_extra )
      {
        PS_FontExtraRec  extra;


        ଯଦି ( !service->ps_get_font_extra( face, &extra ) &&
             extra.fs_type != 0                          )
          ଫେରନ୍ତୁ extra.fs_type;
      }
    }

    /* look at FSType before fsType for Type42 */

    ଯଦି ( ( os2 = (TT_OS2*)FT_Get_Sfnt_Table( face, FT_SFNT_OS2 ) ) != ଖାଲି &&
         os2->version != 0xFFFFU                                           )
      ଫେରନ୍ତୁ os2->fsType;

    ଫେରନ୍ତୁ 0;
  }


/* END */
