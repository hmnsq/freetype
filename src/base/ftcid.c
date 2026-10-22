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


#അന്തര്ഭാവ <freetype/ftcid.h>
#അന്തര്ഭാവ <freetype/internal/ftobjs.h>
#അന്തര്ഭാവ <freetype/internal/services/svcid.h>


  /* documentation is in ftcid.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_CID_Registry_Ordering_Supplement( FT_Face       face,
                                           സ്ഥിരം അക്ഷരം*  *registry,
                                           സ്ഥിരം അക്ഷരം*  *ordering,
                                           FT_Int       *supplement)
  {
    FT_Error     error;
    സ്ഥിരം അക്ഷരം*  r = ശൂന്യ;
    സ്ഥിരം അക്ഷരം*  o = ശൂന്യ;
    FT_Int       s = 0;


    error = FT_ERR( Invalid_Argument );

    എങ്കിൽ ( face )
    {
      FT_Service_CID  service;


      FT_FACE_FIND_SERVICE( face, service, CID );

      എങ്കിൽ ( service && service->get_ros )
        error = service->get_ros( face, &r, &o, &s );
    }

    എങ്കിൽ ( registry )
      *registry = r;

    എങ്കിൽ ( ordering )
      *ordering = o;

    എങ്കിൽ ( supplement )
      *supplement = s;

    തിരികെ error;
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Get_CID_Is_Internally_CID_Keyed( FT_Face   face,
                                      FT_Bool  *is_cid )
  {
    FT_Error  error = FT_ERR( Invalid_Argument );
    FT_Bool   ic = 0;


    എങ്കിൽ ( face )
    {
      FT_Service_CID  service;


      FT_FACE_FIND_SERVICE( face, service, CID );

      എങ്കിൽ ( service && service->get_is_cid )
        error = service->get_is_cid( face, &ic);
    }

    എങ്കിൽ ( is_cid )
      *is_cid = ic;

    തിരികെ error;
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Get_CID_From_Glyph_Index( FT_Face   face,
                               FT_UInt   glyph_index,
                               FT_UInt  *cid )
  {
    FT_Error  error = FT_ERR( Invalid_Argument );
    FT_UInt   c = 0;


    എങ്കിൽ ( face )
    {
      FT_Service_CID  service;


      FT_FACE_FIND_SERVICE( face, service, CID );

      എങ്കിൽ ( service && service->get_cid_from_glyph_index )
        error = service->get_cid_from_glyph_index( face, glyph_index, &c);
    }

    എങ്കിൽ ( cid )
      *cid = c;

    തിരികെ error;
  }


/* END */
