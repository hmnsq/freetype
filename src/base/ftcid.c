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


#অন্তর্ভাব <freetype/ftcid.h>
#অন্তর্ভাব <freetype/internal/ftobjs.h>
#অন্তর্ভাব <freetype/internal/services/svcid.h>


  /* documentation is in ftcid.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_CID_Registry_Ordering_Supplement( FT_Face       face,
                                           ধ্রুব বর্ণ*  *registry,
                                           ধ্রুব বর্ণ*  *ordering,
                                           FT_Int       *supplement)
  {
    FT_Error     error;
    ধ্রুব বর্ণ*  r = রিক্ত;
    ধ্রুব বর্ণ*  o = রিক্ত;
    FT_Int       s = 0;


    error = FT_ERR( Invalid_Argument );

    যদি ( face )
    {
      FT_Service_CID  service;


      FT_FACE_FIND_SERVICE( face, service, CID );

      যদি ( service && service->get_ros )
        error = service->get_ros( face, &r, &o, &s );
    }

    যদি ( registry )
      *registry = r;

    যদি ( ordering )
      *ordering = o;

    যদি ( supplement )
      *supplement = s;

    প্রত্যাগম error;
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Get_CID_Is_Internally_CID_Keyed( FT_Face   face,
                                      FT_Bool  *is_cid )
  {
    FT_Error  error = FT_ERR( Invalid_Argument );
    FT_Bool   ic = 0;


    যদি ( face )
    {
      FT_Service_CID  service;


      FT_FACE_FIND_SERVICE( face, service, CID );

      যদি ( service && service->get_is_cid )
        error = service->get_is_cid( face, &ic);
    }

    যদি ( is_cid )
      *is_cid = ic;

    প্রত্যাগম error;
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Get_CID_From_Glyph_Index( FT_Face   face,
                               FT_UInt   glyph_index,
                               FT_UInt  *cid )
  {
    FT_Error  error = FT_ERR( Invalid_Argument );
    FT_UInt   c = 0;


    যদি ( face )
    {
      FT_Service_CID  service;


      FT_FACE_FIND_SERVICE( face, service, CID );

      যদি ( service && service->get_cid_from_glyph_index )
        error = service->get_cid_from_glyph_index( face, glyph_index, &c);
    }

    যদি ( cid )
      *cid = c;

    প্রত্যাগম error;
  }


/* END */
