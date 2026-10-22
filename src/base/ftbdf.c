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


#include <freetype/internal/ftdebug.h>

#include <freetype/internal/ftobjs.h>
#include <freetype/internal/services/svbdf.h>


  /* documentation is in ftbdf.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_BDF_Charset_ID( FT_Face       face,
                         const var_na*  *acharset_encoding,
                         const var_na*  *acharset_registry )
  {
    FT_Error     error;
    const var_na*  encoding = NULL;
    const var_na*  registry = NULL;

    FT_Service_BDF  service;


    အကယ်၍ ( !face )
      ပြန်ပေး FT_THROW( Invalid_Face_Handle );

    FT_FACE_FIND_SERVICE( face, service, BDF );

    အကယ်၍ ( service && service->get_charset_id )
      error = service->get_charset_id( face, &encoding, &registry );
    မဟုတ်ပါက
      error = FT_THROW( Invalid_Argument );

    အကယ်၍ ( acharset_encoding )
      *acharset_encoding = encoding;

    အကယ်၍ ( acharset_registry )
      *acharset_registry = registry;

    ပြန်ပေး error;
  }


  /* documentation is in ftbdf.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_BDF_Property( FT_Face           face,
                       const var_na*       prop_name,
                       BDF_PropertyRec  *aproperty )
  {
    FT_Error  error;

    FT_Service_BDF  service;


    အကယ်၍ ( !face )
      ပြန်ပေး FT_THROW( Invalid_Face_Handle );

    အကယ်၍ ( !aproperty )
      ပြန်ပေး FT_THROW( Invalid_Argument );

    aproperty->type = BDF_PROPERTY_TYPE_NONE;

    FT_FACE_FIND_SERVICE( face, service, BDF );

    အကယ်၍ ( service && service->get_property )
      error = service->get_property( face, prop_name, aproperty );
    မဟုတ်ပါက
      error = FT_THROW( Invalid_Argument );

    ပြန်ပေး error;
  }


/* END */
