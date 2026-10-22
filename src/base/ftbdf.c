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


#ضمن <freetype/internal/ftdebug.h>

#ضمن <freetype/internal/ftobjs.h>
#ضمن <freetype/internal/services/svbdf.h>


  /* documentation is in ftbdf.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_BDF_Charset_ID( FT_Face       face,
                         ثابت حرف*  *acharset_encoding,
                         ثابت حرف*  *acharset_registry )
  {
    FT_Error     error;
    ثابت حرف*  encoding = عدم;
    ثابت حرف*  registry = عدم;

    FT_Service_BDF  service;


    ܐܢ ( !face )
      ܦܢܐ FT_THROW( Invalid_Face_Handle );

    FT_FACE_FIND_SERVICE( face, service, BDF );

    ܐܢ ( service && service->get_charset_id )
      error = service->get_charset_id( face, &encoding, &registry );
    ܐܚܪܢܐ
      error = FT_THROW( Invalid_Argument );

    ܐܢ ( acharset_encoding )
      *acharset_encoding = encoding;

    ܐܢ ( acharset_registry )
      *acharset_registry = registry;

    ܦܢܐ error;
  }


  /* documentation is in ftbdf.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_BDF_Property( FT_Face           face,
                       ثابت حرف*       prop_name,
                       BDF_PropertyRec  *aproperty )
  {
    FT_Error  error;

    FT_Service_BDF  service;


    ܐܢ ( !face )
      ܦܢܐ FT_THROW( Invalid_Face_Handle );

    ܐܢ ( !aproperty )
      ܦܢܐ FT_THROW( Invalid_Argument );

    aproperty->type = BDF_PROPERTY_TYPE_NONE;

    FT_FACE_FIND_SERVICE( face, service, BDF );

    ܐܢ ( service && service->get_property )
      error = service->get_property( face, prop_name, aproperty );
    ܐܚܪܢܐ
      error = FT_THROW( Invalid_Argument );

    ܦܢܐ error;
  }


/* END */
