/****************************************************************************
 *
 * ftgxval.c
 *
 *   FreeType API for validating TrueTypeGX/AAT tables (body).
 *
 * Copyright (C) 2004-2026 by
 * Masatake YAMATO, Redhat K.K,
 * David Turner, Robert Wilhelm, and Werner Lemberg.
 *
 * This file is part of the FreeType project, and may only be used,
 * modified, and distributed under the terms of the FreeType project
 * license, LICENSE.TXT.  By continuing to use, modify, or distribute
 * this file you indicate that you have read the license and
 * understand and accept it fully.
 *
 */

/****************************************************************************
 *
 * gxvalid is derived from both gxlayout module and otvalid module.
 * Development of gxlayout is supported by the Information-technology
 * Promotion Agency(IPA), Japan.
 *
 */


#ضمن <freetype/internal/ftdebug.h>

#ضمن <freetype/internal/ftobjs.h>
#ضمن <freetype/internal/services/svgxval.h>


  /* documentation is in ftgxval.h */

  FT_EXPORT_DEF( FT_Error )
  FT_TrueTypeGX_Validate( FT_Face   face,
                          FT_UInt   validation_flags,
                          FT_بائيٽون  tables[FT_VALIDATE_GX_ڊيگهائي],
                          FT_UInt   جدول_ڊيگهائي )
  {
    FT_Service_GXvalidate  service;
    FT_Error               error;


    إذا ( !face )
    {
      error = FT_THROW( Invalid_Face_Handle );
      اذهب Exit;
    }

    إذا ( !tables )
    {
      error = FT_THROW( Invalid_دليل );
      اذهب Exit;
    }

    FT_FACE_ڳوليو_GLOBAL_SERVICE( face, service, GX_VALIDATE );

    إذا ( service )
      error = service->validate( face,
                                 validation_flags,
                                 tables,
                                 جدول_ڊيگهائي );
    وإلا
      error = FT_THROW( Unimplemented_Feature );

  Exit:
    أرجع error;
  }


  FT_EXPORT_DEF( فراغ )
  FT_TrueTypeGX_Free( FT_Face   face,
                      FT_بائيٽون  جدول )
  {
    FT_يادگيري  يادگيري;


    إذا ( !face )
      أرجع;

    يادگيري = FT_FACE_يادگيري( face );

    FT_FREE( جدول );
  }


  FT_EXPORT_DEF( FT_Error )
  FT_ClassicKern_Validate( FT_Face    face,
                           FT_UInt    validation_flags,
                           FT_بائيٽون  *ckern_جدول )
  {
    FT_Service_CKERNvalidate  service;
    FT_Error                  error;


    إذا ( !face )
    {
      error = FT_THROW( Invalid_Face_Handle );
      اذهب Exit;
    }

    إذا ( !ckern_جدول )
    {
      error = FT_THROW( Invalid_دليل );
      اذهب Exit;
    }

    FT_FACE_ڳوليو_GLOBAL_SERVICE( face, service, CLASSICKERN_VALIDATE );

    إذا ( service )
      error = service->validate( face,
                                 validation_flags,
                                 ckern_جدول );
    وإلا
      error = FT_THROW( Unimplemented_Feature );

  Exit:
    أرجع error;
  }


  FT_EXPORT_DEF( فراغ )
  FT_ClassicKern_Free( FT_Face   face,
                       FT_بائيٽون  جدول )
  {
    FT_يادگيري  يادگيري;


    إذا ( !face )
      أرجع;

    يادگيري = FT_FACE_يادگيري( face );


    FT_FREE( جدول );
  }


/*پڇاڙي*/
