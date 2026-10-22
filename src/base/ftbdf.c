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


#ଅନ୍ତର୍ଭାଵ <freetype/internal/ftdebug.h>

#ଅନ୍ତର୍ଭାଵ <freetype/internal/ftobjs.h>
#ଅନ୍ତର୍ଭାଵ <freetype/internal/services/svbdf.h>


  /* documentation is in ftbdf.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_BDF_Charset_ID( FT_Face       face,
                         ଅଚଳ ଅକ୍ଷର*  *acharset_encoding,
                         ଅଚଳ ଅକ୍ଷର*  *acharset_registry )
  {
    FT_Error     error;
    ଅଚଳ ଅକ୍ଷର*  encoding = ଖାଲି;
    ଅଚଳ ଅକ୍ଷର*  registry = ଖାଲି;

    FT_Service_BDF  service;


    ଯଦି ( !face )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Face_Handle );

    FT_FACE_FIND_SERVICE( face, service, BDF );

    ଯଦି ( service && service->get_charset_id )
      error = service->get_charset_id( face, &encoding, &registry );
    ଅନ୍ୟଥା
      error = FT_THROW( Invalid_Argument );

    ଯଦି ( acharset_encoding )
      *acharset_encoding = encoding;

    ଯଦି ( acharset_registry )
      *acharset_registry = registry;

    ଫେରନ୍ତୁ error;
  }


  /* documentation is in ftbdf.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_BDF_Property( FT_Face           face,
                       ଅଚଳ ଅକ୍ଷର*       prop_name,
                       BDF_PropertyRec  *aproperty )
  {
    FT_Error  error;

    FT_Service_BDF  service;


    ଯଦି ( !face )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Face_Handle );

    ଯଦି ( !aproperty )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Argument );

    aproperty->type = BDF_PROPERTY_TYPE_NONE;

    FT_FACE_FIND_SERVICE( face, service, BDF );

    ଯଦି ( service && service->get_property )
      error = service->get_property( face, prop_name, aproperty );
    ଅନ୍ୟଥା
      error = FT_THROW( Invalid_Argument );

    ଫେରନ୍ତୁ error;
  }


/* END */
