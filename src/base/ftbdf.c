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


#अन्तर्भाव <freetype/internal/ftdebug.h>

#अन्तर्भाव <freetype/internal/ftobjs.h>
#अन्तर्भाव <freetype/internal/services/svbdf.h>


  /* documentation is in ftbdf.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_BDF_Charset_ID( FT_Face       face,
                         स्थिरांक अक्षर*  *acharset_encoding,
                         स्थिरांक अक्षर*  *acharset_registry )
  {
    FT_Error     error;
    स्थिरांक अक्षर*  encoding = रिक्त;
    स्थिरांक अक्षर*  registry = रिक्त;

    FT_Service_BDF  service;


    जर ( !face )
      परत FT_THROW( Invalid_Face_Handle );

    FT_FACE_FIND_SERVICE( face, service, BDF );

    जर ( service && service->get_charset_id )
      error = service->get_charset_id( face, &encoding, &registry );
    अन्यथा
      error = FT_THROW( Invalid_Argument );

    जर ( acharset_encoding )
      *acharset_encoding = encoding;

    जर ( acharset_registry )
      *acharset_registry = registry;

    परत error;
  }


  /* documentation is in ftbdf.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_BDF_Property( FT_Face           face,
                       स्थिरांक अक्षर*       prop_name,
                       BDF_PropertyRec  *aproperty )
  {
    FT_Error  error;

    FT_Service_BDF  service;


    जर ( !face )
      परत FT_THROW( Invalid_Face_Handle );

    जर ( !aproperty )
      परत FT_THROW( Invalid_Argument );

    aproperty->type = BDF_PROPERTY_TYPE_NONE;

    FT_FACE_FIND_SERVICE( face, service, BDF );

    जर ( service && service->get_property )
      error = service->get_property( face, prop_name, aproperty );
    अन्यथा
      error = FT_THROW( Invalid_Argument );

    परत error;
  }


/* END */
