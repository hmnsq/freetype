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

#ಅನ್ತರ್ಭಾವ <freetype/internal/ftdebug.h>

#ಅನ್ತರ್ಭಾವ <freetype/internal/ftobjs.h>
#ಅನ್ತರ್ಭಾವ <freetype/internal/services/svotval.h>
#ಅನ್ತರ್ಭಾವ <freetype/ftotval.h>


  /* documentation is in ftotval.h */

  FT_EXPORT_DEF( FT_Error )
  FT_OpenType_Validate( FT_Face    face,
                        FT_UInt    validation_flags,
                        FT_ಬೈಟುಗಳು  *ತಳ_ಕೋಷ್ಟಕ,
                        FT_ಬೈಟುಗಳು  *GDEF_ಕೋಷ್ಟಕ,
                        FT_ಬೈಟುಗಳು  *GPOS_ಕೋಷ್ಟಕ,
                        FT_ಬೈಟುಗಳು  *GSUB_ಕೋಷ್ಟಕ,
                        FT_ಬೈಟುಗಳು  *JSTF_ಕೋಷ್ಟಕ )
  {
    FT_Service_OTvalidate  service;
    FT_Error               error;


    ಆದರೆ ( !face )
    {
      error = FT_THROW( Invalid_Face_Handle );
      ಹೋಗು Exit;
    }

    ಆದರೆ ( !( ತಳ_ಕೋಷ್ಟಕ &&
            GDEF_ಕೋಷ್ಟಕ &&
            GPOS_ಕೋಷ್ಟಕ &&
            GSUB_ಕೋಷ್ಟಕ &&
            JSTF_ಕೋಷ್ಟಕ ) )
    {
      error = FT_THROW( Invalid_ವಾದ );
      ಹೋಗು Exit;
    }

    FT_FACE_ಕಂಡುಹಿಡಿ_GLOBAL_SERVICE( face, service, OPENTYPE_VALIDATE );

    ಆದರೆ ( service )
      error = service->validate( face,
                                 validation_flags,
                                 ತಳ_ಕೋಷ್ಟಕ,
                                 GDEF_ಕೋಷ್ಟಕ,
                                 GPOS_ಕೋಷ್ಟಕ,
                                 GSUB_ಕೋಷ್ಟಕ,
                                 JSTF_ಕೋಷ್ಟಕ );
    ಇಲ್ಲದಿದ್ದರೆ
      error = FT_THROW( Unimplemented_Feature );

  Exit:
    ಹಿಂತಿರುಗಿ error;
  }


  FT_EXPORT_DEF( ಶೂನ್ಯ )
  FT_OpenType_Free( FT_Face   face,
                    FT_ಬೈಟುಗಳು  ಕೋಷ್ಟಕ )
  {
    FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ;


    ಆದರೆ ( !face )
      ಹಿಂತಿರುಗಿ;

    ಸ್ಮರಣೆ = FT_FACE_ಸ್ಮರಣೆ( face );

    FT_FREE( ಕೋಷ್ಟಕ );
  }


/*ಕೊನೆ*/
