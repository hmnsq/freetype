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
                        FT_بائيٽون  *بنياد_جدول,
                        FT_بائيٽون  *GDEF_جدول,
                        FT_بائيٽون  *GPOS_جدول,
                        FT_بائيٽون  *GSUB_جدول,
                        FT_بائيٽون  *JSTF_جدول )
  {
    FT_Service_OTvalidate  service;
    FT_Error               error;


    إذا ( !face )
    {
      error = FT_THROW( Invalid_Face_Handle );
      اذهب Exit;
    }

    إذا ( !( بنياد_جدول &&
            GDEF_جدول &&
            GPOS_جدول &&
            GSUB_جدول &&
            JSTF_جدول ) )
    {
      error = FT_THROW( Invalid_دليل );
      اذهب Exit;
    }

    FT_FACE_ڳوليو_GLOBAL_SERVICE( face, service, OPENTYPE_VALIDATE );

    إذا ( service )
      error = service->validate( face,
                                 validation_flags,
                                 بنياد_جدول,
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
                    FT_بائيٽون  جدول )
  {
    FT_يادگيري  يادگيري;


    إذا ( !face )
      أرجع;

    يادگيري = FT_FACE_يادگيري( face );

    FT_FREE( جدول );
  }


/*پڇاڙي*/
