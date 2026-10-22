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
                         अचर वर्ण*  *acharset_encoding,
                         अचर वर्ण*  *acharset_registry )
  {
    FT_Error     error;
    अचर वर्ण*  encoding = रिक्त;
    अचर वर्ण*  registry = रिक्त;

    FT_Service_BDF  service;


    यदि ( !face )
      फर्काऊ FT_THROW( Invalid_Face_Handle );

    FT_FACE_FIND_SERVICE( face, service, BDF );

    यदि ( service && service->get_charset_id )
      error = service->get_charset_id( face, &encoding, &registry );
    अन्यथा
      error = FT_THROW( Invalid_Argument );

    यदि ( acharset_encoding )
      *acharset_encoding = encoding;

    यदि ( acharset_registry )
      *acharset_registry = registry;

    फर्काऊ error;
  }


  /* documentation is in ftbdf.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_BDF_Property( FT_Face           face,
                       अचर वर्ण*       prop_name,
                       BDF_PropertyRec  *aproperty )
  {
    FT_Error  error;

    FT_Service_BDF  service;


    यदि ( !face )
      फर्काऊ FT_THROW( Invalid_Face_Handle );

    यदि ( !aproperty )
      फर्काऊ FT_THROW( Invalid_Argument );

    aproperty->type = BDF_PROPERTY_TYPE_NONE;

    FT_FACE_FIND_SERVICE( face, service, BDF );

    यदि ( service && service->get_property )
      error = service->get_property( face, prop_name, aproperty );
    अन्यथा
      error = FT_THROW( Invalid_Argument );

    फर्काऊ error;
  }


/* END */
