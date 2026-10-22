/****************************************************************************
 *
 * ftbdf.c
 *
 *   FreeType API for accessing BDF-specific strings (body).
 *
 * Copyright (C) 2002-2026 by
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
#شامل <freetype/internal/services/svbdf.h>


  /* documentation is in ftbdf.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_BDF_Charset_ID( FT_Face       face,
                         ثابت حرف*  *acharset_encoding,
                         ثابت حرف*  *acharset_registry )
  {
    FT_Error     error;
    ثابت حرف*  encoding = تہی;
    ثابت حرف*  registry = تہی;

    FT_Service_BDF  service;


    اگر ( !face )
      واپس FT_THROW( Invalid_Face_Handle );

    FT_FACE_ڈھونڈو_SERVICE( face, service, BDF );

    اگر ( service && service->get_charset_id )
      error = service->get_charset_id( face, &encoding, &registry );
    ورنہ
      error = FT_THROW( Invalid_دلیل );

    اگر ( acharset_encoding )
      *acharset_encoding = encoding;

    اگر ( acharset_registry )
      *acharset_registry = registry;

    واپس error;
  }


  /* documentation is in ftbdf.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_BDF_Property( FT_Face           face,
                       ثابت حرف*       prop_name,
                       BDF_PropertyRec  *aproperty )
  {
    FT_Error  error;

    FT_Service_BDF  service;


    اگر ( !face )
      واپس FT_THROW( Invalid_Face_Handle );

    اگر ( !aproperty )
      واپس FT_THROW( Invalid_دلیل );

    aproperty->type = BDF_PROPERTY_TYPE_NONE;

    FT_FACE_ڈھونڈو_SERVICE( face, service, BDF );

    اگر ( service && service->get_property )
      error = service->get_property( face, prop_name, aproperty );
    ورنہ
      error = FT_THROW( Invalid_دلیل );

    واپس error;
  }


/*انتہا*/
