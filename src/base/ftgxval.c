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


#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftdebug.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftobjs.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/services/svgxval.h>


  /* documentation is in ftgxval.h */

  FT_EXPORT_DEF( FT_Error )
  FT_TrueTypeGX_Validate( FT_Face   face,
                          FT_UInt   validation_flags,
                          FT_ਬਾਈਟਾਂ  tables[FT_VALIDATE_GX_ਲੰਮਾਈ],
                          FT_UInt   ਸਾਰਣੀ_ਲੰਮਾਈ )
  {
    FT_Service_GXvalidate  service;
    FT_Error               error;


    ਜੇ ( !face )
    {
      error = FT_THROW( Invalid_Face_Handle );
      ਜਾਓ Exit;
    }

    ਜੇ ( !tables )
    {
      error = FT_THROW( Invalid_ਦਲੀਲ );
      ਜਾਓ Exit;
    }

    FT_FACE_ਲੱਭੋ_GLOBAL_SERVICE( face, service, GX_VALIDATE );

    ਜੇ ( service )
      error = service->validate( face,
                                 validation_flags,
                                 tables,
                                 ਸਾਰਣੀ_ਲੰਮਾਈ );
    ਨਹੀਂਤਾਂ
      error = FT_THROW( Unimplemented_Feature );

  Exit:
    ਵਾਪਸ error;
  }


  FT_EXPORT_DEF( ਖਾਲੀ )
  FT_TrueTypeGX_Free( FT_Face   face,
                      FT_ਬਾਈਟਾਂ  ਸਾਰਣੀ )
  {
    FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ;


    ਜੇ ( !face )
      ਵਾਪਸ;

    ਸਮ੍ਰਿਤੀ = FT_FACE_ਸਮ੍ਰਿਤੀ( face );

    FT_FREE( ਸਾਰਣੀ );
  }


  FT_EXPORT_DEF( FT_Error )
  FT_ClassicKern_Validate( FT_Face    face,
                           FT_UInt    validation_flags,
                           FT_ਬਾਈਟਾਂ  *ckern_ਸਾਰਣੀ )
  {
    FT_Service_CKERNvalidate  service;
    FT_Error                  error;


    ਜੇ ( !face )
    {
      error = FT_THROW( Invalid_Face_Handle );
      ਜਾਓ Exit;
    }

    ਜੇ ( !ckern_ਸਾਰਣੀ )
    {
      error = FT_THROW( Invalid_ਦਲੀਲ );
      ਜਾਓ Exit;
    }

    FT_FACE_ਲੱਭੋ_GLOBAL_SERVICE( face, service, CLASSICKERN_VALIDATE );

    ਜੇ ( service )
      error = service->validate( face,
                                 validation_flags,
                                 ckern_ਸਾਰਣੀ );
    ਨਹੀਂਤਾਂ
      error = FT_THROW( Unimplemented_Feature );

  Exit:
    ਵਾਪਸ error;
  }


  FT_EXPORT_DEF( ਖਾਲੀ )
  FT_ClassicKern_Free( FT_Face   face,
                       FT_ਬਾਈਟਾਂ  ਸਾਰਣੀ )
  {
    FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ;


    ਜੇ ( !face )
      ਵਾਪਸ;

    ਸਮ੍ਰਿਤੀ = FT_FACE_ਸਮ੍ਰਿਤੀ( face );


    FT_FREE( ਸਾਰਣੀ );
  }


/*ਸਿਰਾ*/
