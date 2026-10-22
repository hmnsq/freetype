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

#شامل <freetype/t1tables.h>
#شامل <freetype/tttables.h>
#شامل <freetype/internal/ftserv.h>
#شامل <freetype/internal/services/svpsinfo.h>


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UShort )
  FT_Get_FSType_Flags( FT_Face  face )
  {
    TT_OS2*  os2;


    /* first, try to get the fs_type directly from the font */
    اگر ( face )
    {
      FT_Service_PsInfo  service = تہی;


      FT_FACE_ڈھونڈو_SERVICE( face, service, POSTSCRIPT_INFO );

      اگر ( service && service->ps_get_font_extra )
      {
        PS_FontExtraRec  extra;


        اگر ( !service->ps_get_font_extra( face, &extra ) &&
             extra.fs_type != 0                          )
          واپس extra.fs_type;
      }
    }

    /* look at FSType before fsType for Type42 */

    اگر ( ( os2 = (TT_OS2*)FT_Get_Sfnt_جدول( face, FT_SFNT_OS2 ) ) != تہی &&
         os2->نسخہ != 0xFFFFU                                           )
      واپس os2->fsType;

    واپس 0;
  }


/*انتہا*/
