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


#અન્તર્ભાવ <freetype/ftcid.h>
#અન્તર્ભાવ <freetype/internal/ftobjs.h>
#અન્તર્ભાવ <freetype/internal/services/svcid.h>


  /* documentation is in ftcid.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_CID_Registry_Ordering_Supplement( FT_Face       face,
                                           અચલ અક્ષર*  *registry,
                                           અચલ અક્ષર*  *ordering,
                                           FT_Int       *supplement)
  {
    FT_Error     error;
    અચલ અક્ષર*  r = ખાલી;
    અચલ અક્ષર*  o = ખાલી;
    FT_Int       s = 0;


    error = FT_ERR( Invalid_Argument );

    જો ( face )
    {
      FT_Service_CID  service;


      FT_FACE_FIND_SERVICE( face, service, CID );

      જો ( service && service->get_ros )
        error = service->get_ros( face, &r, &o, &s );
    }

    જો ( registry )
      *registry = r;

    જો ( ordering )
      *ordering = o;

    જો ( supplement )
      *supplement = s;

    પાછા error;
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Get_CID_Is_Internally_CID_Keyed( FT_Face   face,
                                      FT_Bool  *is_cid )
  {
    FT_Error  error = FT_ERR( Invalid_Argument );
    FT_Bool   ic = 0;


    જો ( face )
    {
      FT_Service_CID  service;


      FT_FACE_FIND_SERVICE( face, service, CID );

      જો ( service && service->get_is_cid )
        error = service->get_is_cid( face, &ic);
    }

    જો ( is_cid )
      *is_cid = ic;

    પાછા error;
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Get_CID_From_Glyph_Index( FT_Face   face,
                               FT_UInt   glyph_index,
                               FT_UInt  *cid )
  {
    FT_Error  error = FT_ERR( Invalid_Argument );
    FT_UInt   c = 0;


    જો ( face )
    {
      FT_Service_CID  service;


      FT_FACE_FIND_SERVICE( face, service, CID );

      જો ( service && service->get_cid_from_glyph_index )
        error = service->get_cid_from_glyph_index( face, glyph_index, &c);
    }

    જો ( cid )
      *cid = c;

    પાછા error;
  }


/* END */
