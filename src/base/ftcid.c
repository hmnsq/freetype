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


#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/ftcid.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftobjs.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/services/svcid.h>


  /* documentation is in ftcid.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_CID_Registry_Ordering_Supplement( FT_Face       face,
                                           ਅਚਲ ਅੱਖਰ*  *registry,
                                           ਅਚਲ ਅੱਖਰ*  *ordering,
                                           FT_Int       *supplement)
  {
    FT_Error     error;
    ਅਚਲ ਅੱਖਰ*  r = ਨਿਰਾ;
    ਅਚਲ ਅੱਖਰ*  o = ਨਿਰਾ;
    FT_Int       s = 0;


    error = FT_ERR( Invalid_ਦਲੀਲ );

    ਜੇ ( face )
    {
      FT_Service_CID  service;


      FT_FACE_ਲੱਭੋ_SERVICE( face, service, CID );

      ਜੇ ( service && service->get_ros )
        error = service->get_ros( face, &r, &o, &s );
    }

    ਜੇ ( registry )
      *registry = r;

    ਜੇ ( ordering )
      *ordering = o;

    ਜੇ ( supplement )
      *supplement = s;

    ਵਾਪਸ error;
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Get_CID_Is_Internally_CID_Keyed( FT_Face   face,
                                      FT_Bool  *is_cid )
  {
    FT_Error  error = FT_ERR( Invalid_ਦਲੀਲ );
    FT_Bool   ic = 0;


    ਜੇ ( face )
    {
      FT_Service_CID  service;


      FT_FACE_ਲੱਭੋ_SERVICE( face, service, CID );

      ਜੇ ( service && service->get_is_cid )
        error = service->get_is_cid( face, &ic);
    }

    ਜੇ ( is_cid )
      *is_cid = ic;

    ਵਾਪਸ error;
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Get_CID_From_Glyph_ਸੂਚਕਾਂਕ( FT_Face   face,
                               FT_UInt   glyph_ਸੂਚਕਾਂਕ,
                               FT_UInt  *cid )
  {
    FT_Error  error = FT_ERR( Invalid_ਦਲੀਲ );
    FT_UInt   c = 0;


    ਜੇ ( face )
    {
      FT_Service_CID  service;


      FT_FACE_ਲੱਭੋ_SERVICE( face, service, CID );

      ਜੇ ( service && service->get_cid_from_glyph_ਸੂਚਕਾਂਕ )
        error = service->get_cid_from_glyph_ਸੂਚਕਾਂਕ( face, glyph_ਸੂਚਕਾਂਕ, &c);
    }

    ਜੇ ( cid )
      *cid = c;

    ਵਾਪਸ error;
  }


/*ਸਿਰਾ*/
