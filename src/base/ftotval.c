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

#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftdebug.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftobjs.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/services/svotval.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/ftotval.h>


  /* documentation is in ftotval.h */

  FT_EXPORT_DEF( FT_Error )
  FT_OpenType_Validate( FT_Face    face,
                        FT_UInt    validation_flags,
                        FT_ਬਾਈਟਾਂ  *ਅਧਾਰ_ਸਾਰਣੀ,
                        FT_ਬਾਈਟਾਂ  *GDEF_ਸਾਰਣੀ,
                        FT_ਬਾਈਟਾਂ  *GPOS_ਸਾਰਣੀ,
                        FT_ਬਾਈਟਾਂ  *GSUB_ਸਾਰਣੀ,
                        FT_ਬਾਈਟਾਂ  *JSTF_ਸਾਰਣੀ )
  {
    FT_Service_OTvalidate  service;
    FT_Error               error;


    ਜੇ ( !face )
    {
      error = FT_THROW( Invalid_Face_Handle );
      ਜਾਓ Exit;
    }

    ਜੇ ( !( ਅਧਾਰ_ਸਾਰਣੀ &&
            GDEF_ਸਾਰਣੀ &&
            GPOS_ਸਾਰਣੀ &&
            GSUB_ਸਾਰਣੀ &&
            JSTF_ਸਾਰਣੀ ) )
    {
      error = FT_THROW( Invalid_ਦਲੀਲ );
      ਜਾਓ Exit;
    }

    FT_FACE_ਲੱਭੋ_GLOBAL_SERVICE( face, service, OPENTYPE_VALIDATE );

    ਜੇ ( service )
      error = service->validate( face,
                                 validation_flags,
                                 ਅਧਾਰ_ਸਾਰਣੀ,
                                 GDEF_ਸਾਰਣੀ,
                                 GPOS_ਸਾਰਣੀ,
                                 GSUB_ਸਾਰਣੀ,
                                 JSTF_ਸਾਰਣੀ );
    ਨਹੀਂਤਾਂ
      error = FT_THROW( Unimplemented_Feature );

  Exit:
    ਵਾਪਸ error;
  }


  FT_EXPORT_DEF( ਖਾਲੀ )
  FT_OpenType_Free( FT_Face   face,
                    FT_ਬਾਈਟਾਂ  ਸਾਰਣੀ )
  {
    FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ;


    ਜੇ ( !face )
      ਵਾਪਸ;

    ਸਮ੍ਰਿਤੀ = FT_FACE_ਸਮ੍ਰਿਤੀ( face );

    FT_FREE( ਸਾਰਣੀ );
  }


/*ਸਿਰਾ*/
