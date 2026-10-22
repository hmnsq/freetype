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


#ಅನ್ತರ್ಭಾವ <freetype/ftcid.h>
#ಅನ್ತರ್ಭಾವ <freetype/internal/ftobjs.h>
#ಅನ್ತರ್ಭಾವ <freetype/internal/services/svcid.h>


  /* documentation is in ftcid.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_CID_Registry_Ordering_Supplement( FT_Face       face,
                                           ಧ್ರುವ ವರ್ಣ*  *registry,
                                           ಧ್ರುವ ವರ್ಣ*  *ordering,
                                           FT_Int       *supplement)
  {
    FT_Error     error;
    ಧ್ರುವ ವರ್ಣ*  r = ರಿಕ್ತ;
    ಧ್ರುವ ವರ್ಣ*  o = ರಿಕ್ತ;
    FT_Int       s = 0;


    error = FT_ERR( Invalid_Argument );

    ಯದಿ ( face )
    {
      FT_Service_CID  service;


      FT_FACE_FIND_SERVICE( face, service, CID );

      ಯದಿ ( service && service->get_ros )
        error = service->get_ros( face, &r, &o, &s );
    }

    ಯದಿ ( registry )
      *registry = r;

    ಯದಿ ( ordering )
      *ordering = o;

    ಯದಿ ( supplement )
      *supplement = s;

    ಪ್ರತ್ಯಾಗಮ error;
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Get_CID_Is_Internally_CID_Keyed( FT_Face   face,
                                      FT_Bool  *is_cid )
  {
    FT_Error  error = FT_ERR( Invalid_Argument );
    FT_Bool   ic = 0;


    ಯದಿ ( face )
    {
      FT_Service_CID  service;


      FT_FACE_FIND_SERVICE( face, service, CID );

      ಯದಿ ( service && service->get_is_cid )
        error = service->get_is_cid( face, &ic);
    }

    ಯದಿ ( is_cid )
      *is_cid = ic;

    ಪ್ರತ್ಯಾಗಮ error;
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Get_CID_From_Glyph_Index( FT_Face   face,
                               FT_UInt   glyph_index,
                               FT_UInt  *cid )
  {
    FT_Error  error = FT_ERR( Invalid_Argument );
    FT_UInt   c = 0;


    ಯದಿ ( face )
    {
      FT_Service_CID  service;


      FT_FACE_FIND_SERVICE( face, service, CID );

      ಯದಿ ( service && service->get_cid_from_glyph_index )
        error = service->get_cid_from_glyph_index( face, glyph_index, &c);
    }

    ಯದಿ ( cid )
      *cid = c;

    ಪ್ರತ್ಯಾಗಮ error;
  }


/* END */
