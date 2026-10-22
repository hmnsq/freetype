/****************************************************************************
 *
 * ftcid.c
 *
 *   FreeType API for accessing CID font information.
 *
 * Copyright (C) 2007-2026 by
 * Derek Clegg and Michael Toftdal.
 *
 * This file is part of the FreeType project, and may only be used,
 * modified, and distributed under the terms of the FreeType project
 * license, LICENSE.TXT.  By continuing to use, modify, or distribute
 * this file you indicate that you have read the license and
 * understand and accept it fully.
 *
 */


#అన్తర్భావ <freetype/ftcid.h>
#అన్తర్భావ <freetype/internal/ftobjs.h>
#అన్తర్భావ <freetype/internal/services/svcid.h>


  /* documentation is in ftcid.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_CID_Registry_Ordering_Supplement( FT_Face       face,
                                           స్థిరరాశి అక్షరం*  *registry,
                                           స్థిరరాశి అక్షరం*  *ordering,
                                           FT_Int       *supplement)
  {
    FT_Error     error;
    స్థిరరాశి అక్షరం*  r = ఖాళీ;
    స్థిరరాశి అక్షరం*  o = ఖాళీ;
    FT_Int       s = 0;


    error = FT_ERR( Invalid_వాదం );

    ఒకవేళ ( face )
    {
      FT_Service_CID  service;


      FT_FACE_కనుగొను_SERVICE( face, service, CID );

      ఒకవేళ ( service && service->get_ros )
        error = service->get_ros( face, &r, &o, &s );
    }

    ఒకవేళ ( registry )
      *registry = r;

    ఒకవేళ ( ordering )
      *ordering = o;

    ఒకవేళ ( supplement )
      *supplement = s;

    తిరిగి error;
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Get_CID_Is_Internally_CID_Keyed( FT_Face   face,
                                      FT_Bool  *is_cid )
  {
    FT_Error  error = FT_ERR( Invalid_వాదం );
    FT_Bool   ic = 0;


    ఒకవేళ ( face )
    {
      FT_Service_CID  service;


      FT_FACE_కనుగొను_SERVICE( face, service, CID );

      ఒకవేళ ( service && service->get_is_cid )
        error = service->get_is_cid( face, &ic);
    }

    ఒకవేళ ( is_cid )
      *is_cid = ic;

    తిరిగి error;
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Get_CID_From_Glyph_సూచీ( FT_Face   face,
                               FT_UInt   glyph_సూచీ,
                               FT_UInt  *cid )
  {
    FT_Error  error = FT_ERR( Invalid_వాదం );
    FT_UInt   c = 0;


    ఒకవేళ ( face )
    {
      FT_Service_CID  service;


      FT_FACE_కనుగొను_SERVICE( face, service, CID );

      ఒకవేళ ( service && service->get_cid_from_glyph_సూచీ )
        error = service->get_cid_from_glyph_సూచీ( face, glyph_సూచీ, &c);
    }

    ఒకవేళ ( cid )
      *cid = c;

    తిరిగి error;
  }


/*చివర*/
