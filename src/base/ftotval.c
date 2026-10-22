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
                        FT_بائٹیں  *بنیاد_جدول,
                        FT_بائٹیں  *GDEF_جدول,
                        FT_بائٹیں  *GPOS_جدول,
                        FT_بائٹیں  *GSUB_جدول,
                        FT_بائٹیں  *JSTF_جدول )
  {
    FT_Service_OTvalidate  service;
    FT_Error               error;


    اگر ( !face )
    {
      error = FT_THROW( Invalid_Face_Handle );
      جاؤ Exit;
    }

    اگر ( !( بنیاد_جدول &&
            GDEF_جدول &&
            GPOS_جدول &&
            GSUB_جدول &&
            JSTF_جدول ) )
    {
      error = FT_THROW( Invalid_دلیل );
      جاؤ Exit;
    }

    FT_FACE_ڈھونڈو_GLOBAL_SERVICE( face, service, OPENTYPE_VALIDATE );

    اگر ( service )
      error = service->validate( face,
                                 validation_flags,
                                 بنیاد_جدول,
                                 GDEF_جدول,
                                 GPOS_جدول,
                                 GSUB_جدول,
                                 JSTF_جدول );
    ورنہ
      error = FT_THROW( Unimplemented_Feature );

  Exit:
    واپس error;
  }


  FT_EXPORT_DEF( خالی )
  FT_OpenType_Free( FT_Face   face,
                    FT_بائٹیں  جدول )
  {
    FT_حافظہ  حافظہ;


    اگر ( !face )
      واپس;

    حافظہ = FT_FACE_حافظہ( face );

    FT_FREE( جدول );
  }


/*انتہا*/
