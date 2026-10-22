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


#अन्तर्भाव <freetype/internal/ftdebug.h>

#अन्तर्भाव <freetype/internal/ftobjs.h>
#अन्तर्भाव <freetype/internal/services/svgxval.h>


  /* documentation is in ftgxval.h */

  FT_EXPORT_DEF( FT_Error )
  FT_TrueTypeGX_Validate( FT_Face   face,
                          FT_UInt   validation_flags,
                          FT_अष्टकानि  tables[FT_VALIDATE_GX_दीर्घता],
                          FT_UInt   सारणी_दीर्घता )
  {
    FT_Service_GXvalidate  service;
    FT_Error               error;


    यदि ( !face )
    {
      error = FT_THROW( Invalid_Face_Handle );
      गच्छ Exit;
    }

    यदि ( !tables )
    {
      error = FT_THROW( Invalid_तर्कः );
      गच्छ Exit;
    }

    FT_FACE_FIND_GLOBAL_SERVICE( face, service, GX_VALIDATE );

    यदि ( service )
      error = service->validate( face,
                                 validation_flags,
                                 tables,
                                 सारणी_दीर्घता );
    अन्यथा
      error = FT_THROW( Unimplemented_Feature );

  Exit:
    प्रत्यागम error;
  }


  FT_EXPORT_DEF( शून्य )
  FT_TrueTypeGX_Free( FT_Face   face,
                      FT_अष्टकानि  सारणी )
  {
    FT_स्मृतिः  स्मृतिः;


    यदि ( !face )
      प्रत्यागम;

    स्मृतिः = FT_FACE_स्मृतिः( face );

    FT_FREE( सारणी );
  }


  FT_EXPORT_DEF( FT_Error )
  FT_ClassicKern_Validate( FT_Face    face,
                           FT_UInt    validation_flags,
                           FT_अष्टकानि  *ckern_सारणी )
  {
    FT_Service_CKERNvalidate  service;
    FT_Error                  error;


    यदि ( !face )
    {
      error = FT_THROW( Invalid_Face_Handle );
      गच्छ Exit;
    }

    यदि ( !ckern_सारणी )
    {
      error = FT_THROW( Invalid_तर्कः );
      गच्छ Exit;
    }

    FT_FACE_FIND_GLOBAL_SERVICE( face, service, CLASSICKERN_VALIDATE );

    यदि ( service )
      error = service->validate( face,
                                 validation_flags,
                                 ckern_सारणी );
    अन्यथा
      error = FT_THROW( Unimplemented_Feature );

  Exit:
    प्रत्यागम error;
  }


  FT_EXPORT_DEF( शून्य )
  FT_ClassicKern_Free( FT_Face   face,
                       FT_अष्टकानि  सारणी )
  {
    FT_स्मृतिः  स्मृतिः;


    यदि ( !face )
      प्रत्यागम;

    स्मृतिः = FT_FACE_स्मृतिः( face );


    FT_FREE( सारणी );
  }


/*अन्तः*/
