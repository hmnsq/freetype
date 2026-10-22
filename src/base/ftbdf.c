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


#అన్తర్భావ <freetype/internal/ftdebug.h>

#అన్తర్భావ <freetype/internal/ftobjs.h>
#అన్తర్భావ <freetype/internal/services/svbdf.h>


  /* documentation is in ftbdf.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_BDF_Charset_ID( FT_Face       face,
                         స్థిరరాశి అక్షరం*  *acharset_encoding,
                         స్థిరరాశి అక్షరం*  *acharset_registry )
  {
    FT_Error     error;
    స్థిరరాశి అక్షరం*  encoding = ఖాళీ;
    స్థిరరాశి అక్షరం*  registry = ఖాళీ;

    FT_Service_BDF  service;


    ఒకవేళ ( !face )
      తిరిగి FT_THROW( Invalid_Face_Handle );

    FT_FACE_కనుగొను_SERVICE( face, service, BDF );

    ఒకవేళ ( service && service->get_charset_id )
      error = service->get_charset_id( face, &encoding, &registry );
    లేకపోతే
      error = FT_THROW( Invalid_వాదం );

    ఒకవేళ ( acharset_encoding )
      *acharset_encoding = encoding;

    ఒకవేళ ( acharset_registry )
      *acharset_registry = registry;

    తిరిగి error;
  }


  /* documentation is in ftbdf.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_BDF_Property( FT_Face           face,
                       స్థిరరాశి అక్షరం*       prop_name,
                       BDF_PropertyRec  *aproperty )
  {
    FT_Error  error;

    FT_Service_BDF  service;


    ఒకవేళ ( !face )
      తిరిగి FT_THROW( Invalid_Face_Handle );

    ఒకవేళ ( !aproperty )
      తిరిగి FT_THROW( Invalid_వాదం );

    aproperty->type = BDF_PROPERTY_TYPE_NONE;

    FT_FACE_కనుగొను_SERVICE( face, service, BDF );

    ఒకవేళ ( service && service->get_property )
      error = service->get_property( face, prop_name, aproperty );
    లేకపోతే
      error = FT_THROW( Invalid_వాదం );

    తిరిగి error;
  }


/*చివర*/
