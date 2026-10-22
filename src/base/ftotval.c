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

#অন্তর্ভাৱ <freetype/internal/ftdebug.h>

#অন্তর্ভাৱ <freetype/internal/ftobjs.h>
#অন্তর্ভাৱ <freetype/internal/services/svotval.h>
#অন্তর্ভাৱ <freetype/ftotval.h>


  /* documentation is in ftotval.h */

  FT_EXPORT_DEF( FT_Error )
  FT_OpenType_Validate( FT_Face    face,
                        FT_UInt    validation_flags,
                        FT_Bytes  *BASE_table,
                        FT_Bytes  *GDEF_table,
                        FT_Bytes  *GPOS_table,
                        FT_Bytes  *GSUB_table,
                        FT_Bytes  *JSTF_table )
  {
    FT_Service_OTvalidate  service;
    FT_Error               error;


    যদি ( !face )
    {
      error = FT_THROW( Invalid_Face_Handle );
      গচ্ছ Exit;
    }

    যদি ( !( BASE_table &&
            GDEF_table &&
            GPOS_table &&
            GSUB_table &&
            JSTF_table ) )
    {
      error = FT_THROW( Invalid_Argument );
      গচ্ছ Exit;
    }

    FT_FACE_FIND_GLOBAL_SERVICE( face, service, OPENTYPE_VALIDATE );

    যদি ( service )
      error = service->validate( face,
                                 validation_flags,
                                 BASE_table,
                                 GDEF_table,
                                 GPOS_table,
                                 GSUB_table,
                                 JSTF_table );
    অন্যথা
      error = FT_THROW( Unimplemented_Feature );

  Exit:
    ঘূৰাই error;
  }


  FT_EXPORT_DEF( শূন্য )
  FT_OpenType_Free( FT_Face   face,
                    FT_Bytes  table )
  {
    FT_Memory  memory;


    যদি ( !face )
      ঘূৰাই;

    memory = FT_FACE_MEMORY( face );

    FT_FREE( table );
  }


/* END */
