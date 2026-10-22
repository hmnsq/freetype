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


#شامل <freetype/ftcid.h>
#شامل <freetype/internal/ftobjs.h>
#شامل <freetype/internal/services/svcid.h>


  /* documentation is in ftcid.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_CID_Registry_Ordering_Supplement( FT_Face       face,
                                           ثابت حرف*  *registry,
                                           ثابت حرف*  *ordering,
                                           FT_Int       *supplement)
  {
    FT_Error     error;
    ثابت حرف*  r = تہی;
    ثابت حرف*  o = تہی;
    FT_Int       s = 0;


    error = FT_ERR( Invalid_دلیل );

    اگر ( face )
    {
      FT_Service_CID  service;


      FT_FACE_ڈھونڈو_SERVICE( face, service, CID );

      اگر ( service && service->get_ros )
        error = service->get_ros( face, &r, &o, &s );
    }

    اگر ( registry )
      *registry = r;

    اگر ( ordering )
      *ordering = o;

    اگر ( supplement )
      *supplement = s;

    واپس error;
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Get_CID_Is_Internally_CID_Keyed( FT_Face   face,
                                      FT_Bool  *is_cid )
  {
    FT_Error  error = FT_ERR( Invalid_دلیل );
    FT_Bool   ic = 0;


    اگر ( face )
    {
      FT_Service_CID  service;


      FT_FACE_ڈھونڈو_SERVICE( face, service, CID );

      اگر ( service && service->get_is_cid )
        error = service->get_is_cid( face, &ic);
    }

    اگر ( is_cid )
      *is_cid = ic;

    واپس error;
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Get_CID_From_Glyph_اشاریہ( FT_Face   face,
                               FT_UInt   glyph_اشاریہ,
                               FT_UInt  *cid )
  {
    FT_Error  error = FT_ERR( Invalid_دلیل );
    FT_UInt   c = 0;


    اگر ( face )
    {
      FT_Service_CID  service;


      FT_FACE_ڈھونڈو_SERVICE( face, service, CID );

      اگر ( service && service->get_cid_from_glyph_اشاریہ )
        error = service->get_cid_from_glyph_اشاریہ( face, glyph_اشاریہ, &c);
    }

    اگر ( cid )
      *cid = c;

    واپس error;
  }


/*انتہا*/
