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
                         ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*  *acharset_encoding,
                         ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*  *acharset_registry )
  {
    FT_Error     error;
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*  encoding = ಖಾಲಿ;
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*  registry = ಖಾಲಿ;

    FT_Service_BDF  service;


    ಆದರೆ ( !face )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_Face_Handle );

    FT_FACE_ಕಂಡುಹಿಡಿ_SERVICE( face, service, BDF );

    ಆದರೆ ( service && service->get_charset_id )
      error = service->get_charset_id( face, &encoding, &registry );
    ಇಲ್ಲದಿದ್ದರೆ
      error = FT_THROW( Invalid_ವಾದ );

    ಆದರೆ ( acharset_encoding )
      *acharset_encoding = encoding;

    ಆದರೆ ( acharset_registry )
      *acharset_registry = registry;

    ಹಿಂತಿರುಗಿ error;
  }


  /* documentation is in ftbdf.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_BDF_Property( FT_Face           face,
                       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*       prop_name,
                       BDF_PropertyRec  *aproperty )
  {
    FT_Error  error;

    FT_Service_BDF  service;


    ಆದರೆ ( !face )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_Face_Handle );

    ಆದರೆ ( !aproperty )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

    aproperty->type = BDF_PROPERTY_TYPE_NONE;

    FT_FACE_ಕಂಡುಹಿಡಿ_SERVICE( face, service, BDF );

    ಆದರೆ ( service && service->get_property )
      error = service->get_property( face, prop_name, aproperty );
    ಇಲ್ಲದಿದ್ದರೆ
      error = FT_THROW( Invalid_ವಾದ );

    ಹಿಂತಿರುಗಿ error;
  }


/*ಕೊನೆ*/
