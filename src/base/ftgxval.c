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


#شامل <freetype/internal/ftdebug.h>

#شامل <freetype/internal/ftobjs.h>
#شامل <freetype/internal/services/svgxval.h>


  /* documentation is in ftgxval.h */

  FT_EXPORT_DEF( FT_Error )
  FT_TrueTypeGX_Validate( FT_Face   face,
                          FT_UInt   validation_flags,
                          FT_Bytes  tables[FT_VALIDATE_GX_LENGTH],
                          FT_UInt   table_length )
  {
    FT_Service_GXvalidate  service;
    FT_Error               error;


    اگر ( !face )
    {
      error = FT_THROW( Invalid_Face_Handle );
      برو Exit;
    }

    اگر ( !tables )
    {
      error = FT_THROW( Invalid_Argument );
      برو Exit;
    }

    FT_FACE_FIND_GLOBAL_SERVICE( face, service, GX_VALIDATE );

    اگر ( service )
      error = service->validate( face,
                                 validation_flags,
                                 tables,
                                 table_length );
    وگرنه
      error = FT_THROW( Unimplemented_Feature );

  Exit:
    بازگشت error;
  }


  FT_EXPORT_DEF( تهی )
  FT_TrueTypeGX_Free( FT_Face   face,
                      FT_Bytes  table )
  {
    FT_Memory  memory;


    اگر ( !face )
      بازگشت;

    memory = FT_FACE_MEMORY( face );

    FT_FREE( table );
  }


  FT_EXPORT_DEF( FT_Error )
  FT_ClassicKern_Validate( FT_Face    face,
                           FT_UInt    validation_flags,
                           FT_Bytes  *ckern_table )
  {
    FT_Service_CKERNvalidate  service;
    FT_Error                  error;


    اگر ( !face )
    {
      error = FT_THROW( Invalid_Face_Handle );
      برو Exit;
    }

    اگر ( !ckern_table )
    {
      error = FT_THROW( Invalid_Argument );
      برو Exit;
    }

    FT_FACE_FIND_GLOBAL_SERVICE( face, service, CLASSICKERN_VALIDATE );

    اگر ( service )
      error = service->validate( face,
                                 validation_flags,
                                 ckern_table );
    وگرنه
      error = FT_THROW( Unimplemented_Feature );

  Exit:
    بازگشت error;
  }


  FT_EXPORT_DEF( تهی )
  FT_ClassicKern_Free( FT_Face   face,
                       FT_Bytes  table )
  {
    FT_Memory  memory;


    اگر ( !face )
      بازگشت;

    memory = FT_FACE_MEMORY( face );


    FT_FREE( table );
  }


/* END */
