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


#ಅನ್ತರ್ಭಾವ <freetype/internal/ftdebug.h>

#ಅನ್ತರ್ಭಾವ <freetype/internal/ftobjs.h>
#ಅನ್ತರ್ಭಾವ <freetype/internal/services/svgxval.h>


  /* documentation is in ftgxval.h */

  FT_EXPORT_DEF( FT_Error )
  FT_TrueTypeGX_Validate( FT_Face   face,
                          FT_UInt   validation_flags,
                          FT_ಬೈಟುಗಳು  tables[FT_VALIDATE_GX_ಉದ್ದಳತೆ],
                          FT_UInt   ಕೋಷ್ಟಕ_ಉದ್ದಳತೆ )
  {
    FT_Service_GXvalidate  service;
    FT_Error               error;


    ಆದರೆ ( !face )
    {
      error = FT_THROW( Invalid_Face_Handle );
      ಹೋಗು Exit;
    }

    ಆದರೆ ( !tables )
    {
      error = FT_THROW( Invalid_ವಾದ );
      ಹೋಗು Exit;
    }

    FT_FACE_ಕಂಡುಹಿಡಿ_GLOBAL_SERVICE( face, service, GX_VALIDATE );

    ಆದರೆ ( service )
      error = service->validate( face,
                                 validation_flags,
                                 tables,
                                 ಕೋಷ್ಟಕ_ಉದ್ದಳತೆ );
    ಇಲ್ಲದಿದ್ದರೆ
      error = FT_THROW( Unimplemented_Feature );

  Exit:
    ಹಿಂತಿರುಗಿ error;
  }


  FT_EXPORT_DEF( ಶೂನ್ಯ )
  FT_TrueTypeGX_Free( FT_Face   face,
                      FT_ಬೈಟುಗಳು  ಕೋಷ್ಟಕ )
  {
    FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ;


    ಆದರೆ ( !face )
      ಹಿಂತಿರುಗಿ;

    ಸ್ಮರಣೆ = FT_FACE_ಸ್ಮರಣೆ( face );

    FT_FREE( ಕೋಷ್ಟಕ );
  }


  FT_EXPORT_DEF( FT_Error )
  FT_ClassicKern_Validate( FT_Face    face,
                           FT_UInt    validation_flags,
                           FT_ಬೈಟುಗಳು  *ckern_ಕೋಷ್ಟಕ )
  {
    FT_Service_CKERNvalidate  service;
    FT_Error                  error;


    ಆದರೆ ( !face )
    {
      error = FT_THROW( Invalid_Face_Handle );
      ಹೋಗು Exit;
    }

    ಆದರೆ ( !ckern_ಕೋಷ್ಟಕ )
    {
      error = FT_THROW( Invalid_ವಾದ );
      ಹೋಗು Exit;
    }

    FT_FACE_ಕಂಡುಹಿಡಿ_GLOBAL_SERVICE( face, service, CLASSICKERN_VALIDATE );

    ಆದರೆ ( service )
      error = service->validate( face,
                                 validation_flags,
                                 ckern_ಕೋಷ್ಟಕ );
    ಇಲ್ಲದಿದ್ದರೆ
      error = FT_THROW( Unimplemented_Feature );

  Exit:
    ಹಿಂತಿರುಗಿ error;
  }


  FT_EXPORT_DEF( ಶೂನ್ಯ )
  FT_ClassicKern_Free( FT_Face   face,
                       FT_ಬೈಟುಗಳು  ಕೋಷ್ಟಕ )
  {
    FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ;


    ಆದರೆ ( !face )
      ಹಿಂತಿರುಗಿ;

    ಸ್ಮರಣೆ = FT_FACE_ಸ್ಮರಣೆ( face );


    FT_FREE( ಕೋಷ್ಟಕ );
  }


/*ಕೊನೆ*/
