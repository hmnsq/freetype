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


#অন্তর্ভাব <freetype/internal/ftdebug.h>

#অন্তর্ভাব <freetype/internal/ftobjs.h>
#অন্তর্ভাব <freetype/internal/services/svbdf.h>


  /* documentation is in ftbdf.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_BDF_Charset_ID( FT_Face       face,
                         ধ্রুব বর্ণ*  *acharset_encoding,
                         ধ্রুব বর্ণ*  *acharset_registry )
  {
    FT_Error     error;
    ধ্রুব বর্ণ*  encoding = রিক্ত;
    ধ্রুব বর্ণ*  registry = রিক্ত;

    FT_Service_BDF  service;


    যদি ( !face )
      প্রত্যাগম FT_THROW( Invalid_Face_Handle );

    FT_FACE_FIND_SERVICE( face, service, BDF );

    যদি ( service && service->get_charset_id )
      error = service->get_charset_id( face, &encoding, &registry );
    অন্যথা
      error = FT_THROW( Invalid_Argument );

    যদি ( acharset_encoding )
      *acharset_encoding = encoding;

    যদি ( acharset_registry )
      *acharset_registry = registry;

    প্রত্যাগম error;
  }


  /* documentation is in ftbdf.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_BDF_Property( FT_Face           face,
                       ধ্রুব বর্ণ*       prop_name,
                       BDF_PropertyRec  *aproperty )
  {
    FT_Error  error;

    FT_Service_BDF  service;


    যদি ( !face )
      প্রত্যাগম FT_THROW( Invalid_Face_Handle );

    যদি ( !aproperty )
      প্রত্যাগম FT_THROW( Invalid_Argument );

    aproperty->type = BDF_PROPERTY_TYPE_NONE;

    FT_FACE_FIND_SERVICE( face, service, BDF );

    যদি ( service && service->get_property )
      error = service->get_property( face, prop_name, aproperty );
    অন্যথা
      error = FT_THROW( Invalid_Argument );

    প্রত্যাগম error;
  }


/* END */
