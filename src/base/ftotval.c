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

#شامل <freetype/internal/ftdebug.h>

#شامل <freetype/internal/ftobjs.h>
#شامل <freetype/internal/services/svotval.h>
#شامل <freetype/ftotval.h>


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


    اگر ( !face )
    {
      error = FT_THROW( Invalid_Face_Handle );
      برو Exit;
    }

    اگر ( !( BASE_table &&
            GDEF_table &&
            GPOS_table &&
            GSUB_table &&
            JSTF_table ) )
    {
      error = FT_THROW( Invalid_Argument );
      برو Exit;
    }

    FT_FACE_FIND_GLOBAL_SERVICE( face, service, OPENTYPE_VALIDATE );

    اگر ( service )
      error = service->validate( face,
                                 validation_flags,
                                 BASE_table,
                                 GDEF_table,
                                 GPOS_table,
                                 GSUB_table,
                                 JSTF_table );
    وگرنه
      error = FT_THROW( Unimplemented_Feature );

  Exit:
    بازگشت error;
  }


  FT_EXPORT_DEF( تهی )
  FT_OpenType_Free( FT_Face   face,
                    FT_Bytes  table )
  {
    FT_Memory  memory;


    اگر ( !face )
      بازگشت;

    memory = FT_FACE_MEMORY( face );

    FT_FREE( table );
  }


/* END */
