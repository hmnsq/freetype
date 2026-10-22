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


#അന്തര്ഭാവ <freetype/internal/ftdebug.h>

#അന്തര്ഭാവ <freetype/internal/ftobjs.h>
#അന്തര്ഭാവ <freetype/internal/services/svbdf.h>


  /* documentation is in ftbdf.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_BDF_Charset_ID( FT_Face       face,
                         സ്ഥിരം അക്ഷരം*  *acharset_encoding,
                         സ്ഥിരം അക്ഷരം*  *acharset_registry )
  {
    FT_Error     error;
    സ്ഥിരം അക്ഷരം*  encoding = ശൂന്യ;
    സ്ഥിരം അക്ഷരം*  registry = ശൂന്യ;

    FT_Service_BDF  service;


    എങ്കിൽ ( !face )
      തിരികെ FT_THROW( Invalid_Face_Handle );

    FT_FACE_FIND_SERVICE( face, service, BDF );

    എങ്കിൽ ( service && service->get_charset_id )
      error = service->get_charset_id( face, &encoding, &registry );
    അല്ലെങ്കിൽ
      error = FT_THROW( Invalid_Argument );

    എങ്കിൽ ( acharset_encoding )
      *acharset_encoding = encoding;

    എങ്കിൽ ( acharset_registry )
      *acharset_registry = registry;

    തിരികെ error;
  }


  /* documentation is in ftbdf.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_BDF_Property( FT_Face           face,
                       സ്ഥിരം അക്ഷരം*       prop_name,
                       BDF_PropertyRec  *aproperty )
  {
    FT_Error  error;

    FT_Service_BDF  service;


    എങ്കിൽ ( !face )
      തിരികെ FT_THROW( Invalid_Face_Handle );

    എങ്കിൽ ( !aproperty )
      തിരികെ FT_THROW( Invalid_Argument );

    aproperty->type = BDF_PROPERTY_TYPE_NONE;

    FT_FACE_FIND_SERVICE( face, service, BDF );

    എങ്കിൽ ( service && service->get_property )
      error = service->get_property( face, prop_name, aproperty );
    അല്ലെങ്കിൽ
      error = FT_THROW( Invalid_Argument );

    തിരികെ error;
  }


/* END */
