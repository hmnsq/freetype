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


#அந்தர்பாவ <freetype/internal/ftdebug.h>

#அந்தர்பாவ <freetype/internal/ftobjs.h>
#அந்தர்பாவ <freetype/internal/services/svbdf.h>


  /* documentation is in ftbdf.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_BDF_Charset_ID( FT_Face       face,
                         மாறிலி எழுத்து*  *acharset_encoding,
                         மாறிலி எழுத்து*  *acharset_registry )
  {
    FT_Error     error;
    மாறிலி எழுத்து*  encoding = வெற்று;
    மாறிலி எழுத்து*  registry = வெற்று;

    FT_Service_BDF  service;


    எனில் ( !face )
      திரும்பு FT_THROW( Invalid_Face_Handle );

    FT_FACE_FIND_SERVICE( face, service, BDF );

    எனில் ( service && service->get_charset_id )
      error = service->get_charset_id( face, &encoding, &registry );
    இல்லையெனில்
      error = FT_THROW( Invalid_Argument );

    எனில் ( acharset_encoding )
      *acharset_encoding = encoding;

    எனில் ( acharset_registry )
      *acharset_registry = registry;

    திரும்பு error;
  }


  /* documentation is in ftbdf.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_BDF_Property( FT_Face           face,
                       மாறிலி எழுத்து*       prop_name,
                       BDF_PropertyRec  *aproperty )
  {
    FT_Error  error;

    FT_Service_BDF  service;


    எனில் ( !face )
      திரும்பு FT_THROW( Invalid_Face_Handle );

    எனில் ( !aproperty )
      திரும்பு FT_THROW( Invalid_Argument );

    aproperty->type = BDF_PROPERTY_TYPE_NONE;

    FT_FACE_FIND_SERVICE( face, service, BDF );

    எனில் ( service && service->get_property )
      error = service->get_property( face, prop_name, aproperty );
    இல்லையெனில்
      error = FT_THROW( Invalid_Argument );

    திரும்பு error;
  }


/* END */
