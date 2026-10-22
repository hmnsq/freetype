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
                         ध्रुव वर्ण*  *acharset_encoding,
                         ध्रुव वर्ण*  *acharset_registry )
  {
    FT_Error     error;
    ध्रुव वर्ण*  encoding = रिक्त;
    ध्रुव वर्ण*  registry = रिक्त;

    FT_Service_BDF  service;


    सचे ( !face )
      पटिनिवत्त FT_THROW( Invalid_Face_Handle );

    FT_FACE_FIND_SERVICE( face, service, BDF );

    सचे ( service && service->get_charset_id )
      error = service->get_charset_id( face, &encoding, &registry );
    अञ्ञथा
      error = FT_THROW( Invalid_Argument );

    सचे ( acharset_encoding )
      *acharset_encoding = encoding;

    सचे ( acharset_registry )
      *acharset_registry = registry;

    पटिनिवत्त error;
  }


  /* documentation is in ftbdf.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_BDF_Property( FT_Face           face,
                       ध्रुव वर्ण*       prop_name,
                       BDF_PropertyRec  *aproperty )
  {
    FT_Error  error;

    FT_Service_BDF  service;


    सचे ( !face )
      पटिनिवत्त FT_THROW( Invalid_Face_Handle );

    सचे ( !aproperty )
      पटिनिवत्त FT_THROW( Invalid_Argument );

    aproperty->type = BDF_PROPERTY_TYPE_NONE;

    FT_FACE_FIND_SERVICE( face, service, BDF );

    सचे ( service && service->get_property )
      error = service->get_property( face, prop_name, aproperty );
    अञ्ञथा
      error = FT_THROW( Invalid_Argument );

    पटिनिवत्त error;
  }


/* END */
