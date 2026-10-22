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


#ಅನ್ತರ್ಭಾವ <freetype/internal/ftdebug.h>

#ಅನ್ತರ್ಭಾವ <freetype/internal/ftobjs.h>
#ಅನ್ತರ್ಭಾವ <freetype/internal/services/svbdf.h>


  /* documentation is in ftbdf.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_BDF_Charset_ID( FT_Face       face,
                         ಧ್ರುವ ವರ್ಣ*  *acharset_encoding,
                         ಧ್ರುವ ವರ್ಣ*  *acharset_registry )
  {
    FT_Error     error;
    ಧ್ರುವ ವರ್ಣ*  encoding = ರಿಕ್ತ;
    ಧ್ರುವ ವರ್ಣ*  registry = ರಿಕ್ತ;

    FT_Service_BDF  service;


    ಯದಿ ( !face )
      ಪ್ರತ್ಯಾಗಮ FT_THROW( Invalid_Face_Handle );

    FT_FACE_FIND_SERVICE( face, service, BDF );

    ಯದಿ ( service && service->get_charset_id )
      error = service->get_charset_id( face, &encoding, &registry );
    ಅನ್ಯಥಾ
      error = FT_THROW( Invalid_Argument );

    ಯದಿ ( acharset_encoding )
      *acharset_encoding = encoding;

    ಯದಿ ( acharset_registry )
      *acharset_registry = registry;

    ಪ್ರತ್ಯಾಗಮ error;
  }


  /* documentation is in ftbdf.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_BDF_Property( FT_Face           face,
                       ಧ್ರುವ ವರ್ಣ*       prop_name,
                       BDF_PropertyRec  *aproperty )
  {
    FT_Error  error;

    FT_Service_BDF  service;


    ಯದಿ ( !face )
      ಪ್ರತ್ಯಾಗಮ FT_THROW( Invalid_Face_Handle );

    ಯದಿ ( !aproperty )
      ಪ್ರತ್ಯಾಗಮ FT_THROW( Invalid_Argument );

    aproperty->type = BDF_PROPERTY_TYPE_NONE;

    FT_FACE_FIND_SERVICE( face, service, BDF );

    ಯದಿ ( service && service->get_property )
      error = service->get_property( face, prop_name, aproperty );
    ಅನ್ಯಥಾ
      error = FT_THROW( Invalid_Argument );

    ಪ್ರತ್ಯಾಗಮ error;
  }


/* END */
