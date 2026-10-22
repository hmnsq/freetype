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


#અન્તર્ભાવ <freetype/internal/ftdebug.h>

#અન્તર્ભાવ <freetype/internal/ftobjs.h>
#અન્તર્ભાવ <freetype/internal/services/svbdf.h>


  /* documentation is in ftbdf.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_BDF_Charset_ID( FT_Face       face,
                         અચલ અક્ષર*  *acharset_encoding,
                         અચલ અક્ષર*  *acharset_registry )
  {
    FT_Error     error;
    અચલ અક્ષર*  encoding = ખાલી;
    અચલ અક્ષર*  registry = ખાલી;

    FT_Service_BDF  service;


    જો ( !face )
      પાછા FT_THROW( Invalid_Face_Handle );

    FT_FACE_FIND_SERVICE( face, service, BDF );

    જો ( service && service->get_charset_id )
      error = service->get_charset_id( face, &encoding, &registry );
    નહીંતર
      error = FT_THROW( Invalid_Argument );

    જો ( acharset_encoding )
      *acharset_encoding = encoding;

    જો ( acharset_registry )
      *acharset_registry = registry;

    પાછા error;
  }


  /* documentation is in ftbdf.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_BDF_Property( FT_Face           face,
                       અચલ અક્ષર*       prop_name,
                       BDF_PropertyRec  *aproperty )
  {
    FT_Error  error;

    FT_Service_BDF  service;


    જો ( !face )
      પાછા FT_THROW( Invalid_Face_Handle );

    જો ( !aproperty )
      પાછા FT_THROW( Invalid_Argument );

    aproperty->type = BDF_PROPERTY_TYPE_NONE;

    FT_FACE_FIND_SERVICE( face, service, BDF );

    જો ( service && service->get_property )
      error = service->get_property( face, prop_name, aproperty );
    નહીંતર
      error = FT_THROW( Invalid_Argument );

    પાછા error;
  }


/* END */
