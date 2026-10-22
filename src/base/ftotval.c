/****************************************************************************
 *
 * ftotval.c
 *
 *   FreeType API for validating OpenType tables (body).
 *
 * Copyright (C) 2004-2026 by
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

#ضمن <freetype/internal/ftobjs.h>
#ضمن <freetype/internal/services/svotval.h>
#ضمن <freetype/ftotval.h>


  /* documentation is in ftotval.h */

  FT_EXPORT_DEF( FT_Error )
  FT_OpenType_Validate( FT_Face    face,
                        FT_UInt    validation_flags,
                        FT_بایتها  *پایه_جدول,
                        FT_بایتها  *GDEF_جدول,
                        FT_بایتها  *GPOS_جدول,
                        FT_بایتها  *GSUB_جدول,
                        FT_بایتها  *JSTF_جدول )
  {
    FT_Service_OTvalidate  service;
    FT_Error               error;


    إذا ( !face )
    {
      error = FT_THROW( Invalid_Face_Handle );
      اذهب Exit;
    }

    إذا ( !( پایه_جدول &&
            GDEF_جدول &&
            GPOS_جدول &&
            GSUB_جدول &&
            JSTF_جدول ) )
    {
      error = FT_THROW( Invalid_آرگومان );
      اذهب Exit;
    }

    FT_FACE_پیدا_کن_GLOBAL_SERVICE( face, service, OPENTYPE_VALIDATE );

    إذا ( service )
      error = service->validate( face,
                                 validation_flags,
                                 پایه_جدول,
                                 GDEF_جدول,
                                 GPOS_جدول,
                                 GSUB_جدول,
                                 JSTF_جدول );
    وإلا
      error = FT_THROW( Unimplemented_Feature );

  Exit:
    أرجع error;
  }


  FT_EXPORT_DEF( فراغ )
  FT_OpenType_Free( FT_Face   face,
                    FT_بایتها  جدول )
  {
    FT_حافظه  حافظه;


    إذا ( !face )
      أرجع;

    حافظه = FT_FACE_حافظه( face );

    FT_FREE( جدول );
  }


/*انتها*/
