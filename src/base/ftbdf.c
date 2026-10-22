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


#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftdebug.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftobjs.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/services/svbdf.h>


  /* documentation is in ftbdf.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_BDF_Charset_ID( FT_Face       face,
                         ਅਚਲ ਅੱਖਰ*  *acharset_encoding,
                         ਅਚਲ ਅੱਖਰ*  *acharset_registry )
  {
    FT_Error     error;
    ਅਚਲ ਅੱਖਰ*  encoding = ਨਿਰਾ;
    ਅਚਲ ਅੱਖਰ*  registry = ਨਿਰਾ;

    FT_Service_BDF  service;


    ਜੇ ( !face )
      ਵਾਪਸ FT_THROW( Invalid_Face_Handle );

    FT_FACE_ਲੱਭੋ_SERVICE( face, service, BDF );

    ਜੇ ( service && service->get_charset_id )
      error = service->get_charset_id( face, &encoding, &registry );
    ਨਹੀਂਤਾਂ
      error = FT_THROW( Invalid_ਦਲੀਲ );

    ਜੇ ( acharset_encoding )
      *acharset_encoding = encoding;

    ਜੇ ( acharset_registry )
      *acharset_registry = registry;

    ਵਾਪਸ error;
  }


  /* documentation is in ftbdf.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_BDF_Property( FT_Face           face,
                       ਅਚਲ ਅੱਖਰ*       prop_name,
                       BDF_PropertyRec  *aproperty )
  {
    FT_Error  error;

    FT_Service_BDF  service;


    ਜੇ ( !face )
      ਵਾਪਸ FT_THROW( Invalid_Face_Handle );

    ਜੇ ( !aproperty )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    aproperty->type = BDF_PROPERTY_TYPE_NONE;

    FT_FACE_ਲੱਭੋ_SERVICE( face, service, BDF );

    ਜੇ ( service && service->get_property )
      error = service->get_property( face, prop_name, aproperty );
    ਨਹੀਂਤਾਂ
      error = FT_THROW( Invalid_ਦਲੀਲ );

    ਵਾਪਸ error;
  }


/*ਸਿਰਾ*/
