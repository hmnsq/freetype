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


#అన్తర్భావ <freetype/internal/ftdebug.h>

#అన్తర్భావ <freetype/internal/ftobjs.h>
#అన్తర్భావ <freetype/internal/services/svgxval.h>


  /* documentation is in ftgxval.h */

  FT_EXPORT_DEF( FT_Error )
  FT_TrueTypeGX_Validate( FT_Face   face,
                          FT_UInt   validation_flags,
                          FT_బైట్లు  tables[FT_VALIDATE_GX_నిడివి],
                          FT_UInt   పట్టిక_నిడివి )
  {
    FT_Service_GXvalidate  service;
    FT_Error               error;


    ఒకవేళ ( !face )
    {
      error = FT_THROW( Invalid_Face_Handle );
      వెళ్ళు Exit;
    }

    ఒకవేళ ( !tables )
    {
      error = FT_THROW( Invalid_వాదం );
      వెళ్ళు Exit;
    }

    FT_FACE_కనుగొను_GLOBAL_SERVICE( face, service, GX_VALIDATE );

    ఒకవేళ ( service )
      error = service->validate( face,
                                 validation_flags,
                                 tables,
                                 పట్టిక_నిడివి );
    లేకపోతే
      error = FT_THROW( Unimplemented_Feature );

  Exit:
    తిరిగి error;
  }


  FT_EXPORT_DEF( శూన్య )
  FT_TrueTypeGX_Free( FT_Face   face,
                      FT_బైట్లు  పట్టిక )
  {
    FT_జ్ఞాపకం  జ్ఞాపకం;


    ఒకవేళ ( !face )
      తిరిగి;

    జ్ఞాపకం = FT_FACE_జ్ఞాపకం( face );

    FT_FREE( పట్టిక );
  }


  FT_EXPORT_DEF( FT_Error )
  FT_ClassicKern_Validate( FT_Face    face,
                           FT_UInt    validation_flags,
                           FT_బైట్లు  *ckern_పట్టిక )
  {
    FT_Service_CKERNvalidate  service;
    FT_Error                  error;


    ఒకవేళ ( !face )
    {
      error = FT_THROW( Invalid_Face_Handle );
      వెళ్ళు Exit;
    }

    ఒకవేళ ( !ckern_పట్టిక )
    {
      error = FT_THROW( Invalid_వాదం );
      వెళ్ళు Exit;
    }

    FT_FACE_కనుగొను_GLOBAL_SERVICE( face, service, CLASSICKERN_VALIDATE );

    ఒకవేళ ( service )
      error = service->validate( face,
                                 validation_flags,
                                 ckern_పట్టిక );
    లేకపోతే
      error = FT_THROW( Unimplemented_Feature );

  Exit:
    తిరిగి error;
  }


  FT_EXPORT_DEF( శూన్య )
  FT_ClassicKern_Free( FT_Face   face,
                       FT_బైట్లు  పట్టిక )
  {
    FT_జ్ఞాపకం  జ్ఞాపకం;


    ఒకవేళ ( !face )
      తిరిగి;

    జ్ఞాపకం = FT_FACE_జ్ఞాపకం( face );


    FT_FREE( పట్టిక );
  }


/*చివర*/
