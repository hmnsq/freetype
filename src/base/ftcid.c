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
                                           ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*  *registry,
                                           ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*  *ordering,
                                           FT_Int       *supplement)
  {
    FT_Error     error;
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*  r = ಖಾಲಿ;
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*  o = ಖಾಲಿ;
    FT_Int       s = 0;


    error = FT_ERR( Invalid_ವಾದ );

    ಆದರೆ ( face )
    {
      FT_Service_CID  service;


      FT_FACE_ಕಂಡುಹಿಡಿ_SERVICE( face, service, CID );

      ಆದರೆ ( service && service->get_ros )
        error = service->get_ros( face, &r, &o, &s );
    }

    ಆದರೆ ( registry )
      *registry = r;

    ಆದರೆ ( ordering )
      *ordering = o;

    ಆದರೆ ( supplement )
      *supplement = s;

    ಹಿಂತಿರುಗಿ error;
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Get_CID_Is_Internally_CID_Keyed( FT_Face   face,
                                      FT_Bool  *is_cid )
  {
    FT_Error  error = FT_ERR( Invalid_ವಾದ );
    FT_Bool   ic = 0;


    ಆದರೆ ( face )
    {
      FT_Service_CID  service;


      FT_FACE_ಕಂಡುಹಿಡಿ_SERVICE( face, service, CID );

      ಆದರೆ ( service && service->get_is_cid )
        error = service->get_is_cid( face, &ic);
    }

    ಆದರೆ ( is_cid )
      *is_cid = ic;

    ಹಿಂತಿರುಗಿ error;
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Get_CID_From_Glyph_ಸೂಚ್ಯಂಕ( FT_Face   face,
                               FT_UInt   glyph_ಸೂಚ್ಯಂಕ,
                               FT_UInt  *cid )
  {
    FT_Error  error = FT_ERR( Invalid_ವಾದ );
    FT_UInt   c = 0;


    ಆದರೆ ( face )
    {
      FT_Service_CID  service;


      FT_FACE_ಕಂಡುಹಿಡಿ_SERVICE( face, service, CID );

      ಆದರೆ ( service && service->get_cid_from_glyph_ಸೂಚ್ಯಂಕ )
        error = service->get_cid_from_glyph_ಸೂಚ್ಯಂಕ( face, glyph_ಸೂಚ್ಯಂಕ, &c);
    }

    ಆದರೆ ( cid )
      *cid = c;

    ಹಿಂತಿರುಗಿ error;
  }


/*ಕೊನೆ*/
