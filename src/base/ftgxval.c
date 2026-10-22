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


#അന്തര്ഭാവ <freetype/internal/ftdebug.h>

#അന്തര്ഭാവ <freetype/internal/ftobjs.h>
#അന്തര്ഭാവ <freetype/internal/services/svgxval.h>


  /* documentation is in ftgxval.h */

  FT_EXPORT_DEF( FT_Error )
  FT_TrueTypeGX_Validate( FT_Face   face,
                          FT_UInt   validation_flags,
                          FT_Bytes  tables[FT_VALIDATE_GX_LENGTH],
                          FT_UInt   table_length )
  {
    FT_Service_GXvalidate  service;
    FT_Error               error;


    എങ്കിൽ ( !face )
    {
      error = FT_THROW( Invalid_Face_Handle );
      പോകുക Exit;
    }

    എങ്കിൽ ( !tables )
    {
      error = FT_THROW( Invalid_Argument );
      പോകുക Exit;
    }

    FT_FACE_FIND_GLOBAL_SERVICE( face, service, GX_VALIDATE );

    എങ്കിൽ ( service )
      error = service->validate( face,
                                 validation_flags,
                                 tables,
                                 table_length );
    അല്ലെങ്കിൽ
      error = FT_THROW( Unimplemented_Feature );

  Exit:
    തിരികെ error;
  }


  FT_EXPORT_DEF( ശൂന്യം )
  FT_TrueTypeGX_Free( FT_Face   face,
                      FT_Bytes  table )
  {
    FT_Memory  memory;


    എങ്കിൽ ( !face )
      തിരികെ;

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


    എങ്കിൽ ( !face )
    {
      error = FT_THROW( Invalid_Face_Handle );
      പോകുക Exit;
    }

    എങ്കിൽ ( !ckern_table )
    {
      error = FT_THROW( Invalid_Argument );
      പോകുക Exit;
    }

    FT_FACE_FIND_GLOBAL_SERVICE( face, service, CLASSICKERN_VALIDATE );

    എങ്കിൽ ( service )
      error = service->validate( face,
                                 validation_flags,
                                 ckern_table );
    അല്ലെങ്കിൽ
      error = FT_THROW( Unimplemented_Feature );

  Exit:
    തിരികെ error;
  }


  FT_EXPORT_DEF( ശൂന്യം )
  FT_ClassicKern_Free( FT_Face   face,
                       FT_Bytes  table )
  {
    FT_Memory  memory;


    എങ്കിൽ ( !face )
      തിരികെ;

    memory = FT_FACE_MEMORY( face );


    FT_FREE( table );
  }


/* END */
