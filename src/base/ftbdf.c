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


#समावेश <freetype/internal/ftdebug.h>

#समावेश <freetype/internal/ftobjs.h>
#समावेश <freetype/internal/services/svbdf.h>


  /* documentation is in ftbdf.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_BDF_Charset_ID( FT_Face       face,
                         स्थिर अक्षर*  *acharset_encoding,
                         स्थिर अक्षर*  *acharset_registry )
  {
    FT_Error     error;
    स्थिर अक्षर*  encoding = शून्य;
    स्थिर अक्षर*  registry = शून्य;

    FT_Service_BDF  service;


    अगर ( !face )
      वापस FT_THROW( Invalid_Face_Handle );

    FT_FACE_FIND_SERVICE( face, service, BDF );

    अगर ( service && service->get_charset_id )
      error = service->get_charset_id( face, &encoding, &registry );
    अन्यथा
      error = FT_THROW( Invalid_Argument );

    अगर ( acharset_encoding )
      *acharset_encoding = encoding;

    अगर ( acharset_registry )
      *acharset_registry = registry;

    वापस error;
  }


  /* documentation is in ftbdf.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_BDF_Property( FT_Face           face,
                       स्थिर अक्षर*       prop_name,
                       BDF_PropertyRec  *aproperty )
  {
    FT_Error  error;

    FT_Service_BDF  service;


    अगर ( !face )
      वापस FT_THROW( Invalid_Face_Handle );

    अगर ( !aproperty )
      वापस FT_THROW( Invalid_Argument );

    aproperty->type = BDF_PROPERTY_TYPE_NONE;

    FT_FACE_FIND_SERVICE( face, service, BDF );

    अगर ( service && service->get_property )
      error = service->get_property( face, prop_name, aproperty );
    अन्यथा
      error = FT_THROW( Invalid_Argument );

    वापस error;
  }


/* END */
