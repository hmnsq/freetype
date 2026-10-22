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

#అన్తర్భావ <freetype/internal/ftdebug.h>

#అన్తర్భావ <freetype/internal/ftobjs.h>
#అన్తర్భావ <freetype/internal/services/svotval.h>
#అన్తర్భావ <freetype/ftotval.h>


  /* documentation is in ftotval.h */

  FT_EXPORT_DEF( FT_Error )
  FT_OpenType_Validate( FT_Face    face,
                        FT_UInt    validation_flags,
                        FT_బైట్లు  *పునాది_పట్టిక,
                        FT_బైట్లు  *GDEF_పట్టిక,
                        FT_బైట్లు  *GPOS_పట్టిక,
                        FT_బైట్లు  *GSUB_పట్టిక,
                        FT_బైట్లు  *JSTF_పట్టిక )
  {
    FT_Service_OTvalidate  service;
    FT_Error               error;


    ఒకవేళ ( !face )
    {
      error = FT_THROW( Invalid_Face_Handle );
      వెళ్ళు Exit;
    }

    ఒకవేళ ( !( పునాది_పట్టిక &&
            GDEF_పట్టిక &&
            GPOS_పట్టిక &&
            GSUB_పట్టిక &&
            JSTF_పట్టిక ) )
    {
      error = FT_THROW( Invalid_వాదం );
      వెళ్ళు Exit;
    }

    FT_FACE_కనుగొను_GLOBAL_SERVICE( face, service, OPENTYPE_VALIDATE );

    ఒకవేళ ( service )
      error = service->validate( face,
                                 validation_flags,
                                 పునాది_పట్టిక,
                                 GDEF_పట్టిక,
                                 GPOS_పట్టిక,
                                 GSUB_పట్టిక,
                                 JSTF_పట్టిక );
    లేకపోతే
      error = FT_THROW( Unimplemented_Feature );

  Exit:
    తిరిగి error;
  }


  FT_EXPORT_DEF( శూన్య )
  FT_OpenType_Free( FT_Face   face,
                    FT_బైట్లు  పట్టిక )
  {
    FT_జ్ఞాపకం  జ్ఞాపకం;


    ఒకవేళ ( !face )
      తిరిగి;

    జ్ఞాపకం = FT_FACE_జ్ఞాపకం( face );

    FT_FREE( పట్టిక );
  }


/*చివర*/
