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

#अन्तर्भाव <freetype/internal/ftdebug.h>

#अन्तर्भाव <freetype/internal/ftobjs.h>
#अन्तर्भाव <freetype/internal/services/svotval.h>
#अन्तर्भाव <freetype/ftotval.h>


  /* documentation is in ftotval.h */

  FT_EXPORT_DEF( FT_Error )
  FT_OpenType_Validate( FT_Face    face,
                        FT_UInt    validation_flags,
                        FT_अष्टकानि  *आधारः_सारणी,
                        FT_अष्टकानि  *GDEF_सारणी,
                        FT_अष्टकानि  *GPOS_सारणी,
                        FT_अष्टकानि  *GSUB_सारणी,
                        FT_अष्टकानि  *JSTF_सारणी )
  {
    FT_Service_OTvalidate  service;
    FT_Error               error;


    यदि ( !face )
    {
      error = FT_THROW( Invalid_Face_Handle );
      गच्छ Exit;
    }

    यदि ( !( आधारः_सारणी &&
            GDEF_सारणी &&
            GPOS_सारणी &&
            GSUB_सारणी &&
            JSTF_सारणी ) )
    {
      error = FT_THROW( Invalid_तर्कः );
      गच्छ Exit;
    }

    FT_FACE_FIND_GLOBAL_SERVICE( face, service, OPENTYPE_VALIDATE );

    यदि ( service )
      error = service->validate( face,
                                 validation_flags,
                                 आधारः_सारणी,
                                 GDEF_सारणी,
                                 GPOS_सारणी,
                                 GSUB_सारणी,
                                 JSTF_सारणी );
    अन्यथा
      error = FT_THROW( Unimplemented_Feature );

  Exit:
    प्रत्यागम error;
  }


  FT_EXPORT_DEF( शून्य )
  FT_OpenType_Free( FT_Face   face,
                    FT_अष्टकानि  सारणी )
  {
    FT_स्मृतिः  स्मृतिः;


    यदि ( !face )
      प्रत्यागम;

    स्मृतिः = FT_FACE_स्मृतिः( face );

    FT_FREE( सारणी );
  }


/*अन्तः*/
