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
                         ثابت نویسه*  *acharset_encoding,
                         ثابت نویسه*  *acharset_registry )
  {
    FT_Error     error;
    ثابت نویسه*  encoding = پوچ;
    ثابت نویسه*  registry = پوچ;

    FT_Service_BDF  service;


    اگر ( !face )
      بازگشت FT_THROW( Invalid_Face_Handle );

    FT_FACE_FIND_SERVICE( face, service, BDF );

    اگر ( service && service->get_charset_id )
      error = service->get_charset_id( face, &encoding, &registry );
    وگرنه
      error = FT_THROW( Invalid_Argument );

    اگر ( acharset_encoding )
      *acharset_encoding = encoding;

    اگر ( acharset_registry )
      *acharset_registry = registry;

    بازگشت error;
  }


  /* documentation is in ftbdf.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_BDF_Property( FT_Face           face,
                       ثابت نویسه*       prop_name,
                       BDF_PropertyRec  *aproperty )
  {
    FT_Error  error;

    FT_Service_BDF  service;


    اگر ( !face )
      بازگشت FT_THROW( Invalid_Face_Handle );

    اگر ( !aproperty )
      بازگشت FT_THROW( Invalid_Argument );

    aproperty->type = BDF_PROPERTY_TYPE_NONE;

    FT_FACE_FIND_SERVICE( face, service, BDF );

    اگر ( service && service->get_property )
      error = service->get_property( face, prop_name, aproperty );
    وگرنه
      error = FT_THROW( Invalid_Argument );

    بازگشت error;
  }


/* END */
