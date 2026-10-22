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


#समावेश <freetype/ftcid.h>
#समावेश <freetype/internal/ftobjs.h>
#समावेश <freetype/internal/services/svcid.h>


  /* documentation is in ftcid.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_CID_Registry_Ordering_Supplement( FT_Face       face,
                                           स्थिर अक्षर*  *registry,
                                           स्थिर अक्षर*  *ordering,
                                           FT_Int       *supplement)
  {
    FT_Error     error;
    स्थिर अक्षर*  r = शून्य;
    स्थिर अक्षर*  o = शून्य;
    FT_Int       s = 0;


    error = FT_ERR( Invalid_Argument );

    अगर ( face )
    {
      FT_Service_CID  service;


      FT_FACE_FIND_SERVICE( face, service, CID );

      अगर ( service && service->get_ros )
        error = service->get_ros( face, &r, &o, &s );
    }

    अगर ( registry )
      *registry = r;

    अगर ( ordering )
      *ordering = o;

    अगर ( supplement )
      *supplement = s;

    वापस error;
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Get_CID_Is_Internally_CID_Keyed( FT_Face   face,
                                      FT_Bool  *is_cid )
  {
    FT_Error  error = FT_ERR( Invalid_Argument );
    FT_Bool   ic = 0;


    अगर ( face )
    {
      FT_Service_CID  service;


      FT_FACE_FIND_SERVICE( face, service, CID );

      अगर ( service && service->get_is_cid )
        error = service->get_is_cid( face, &ic);
    }

    अगर ( is_cid )
      *is_cid = ic;

    वापस error;
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Get_CID_From_Glyph_सूचकांक( FT_Face   face,
                               FT_UInt   glyph_सूचकांक,
                               FT_UInt  *cid )
  {
    FT_Error  error = FT_ERR( Invalid_Argument );
    FT_UInt   c = 0;


    अगर ( face )
    {
      FT_Service_CID  service;


      FT_FACE_FIND_SERVICE( face, service, CID );

      अगर ( service && service->get_cid_from_glyph_सूचकांक )
        error = service->get_cid_from_glyph_सूचकांक( face, glyph_सूचकांक, &c);
    }

    अगर ( cid )
      *cid = c;

    वापस error;
  }


/* END */
