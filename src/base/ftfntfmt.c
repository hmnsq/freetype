/****************************************************************************
 *
 * ftfntfmt.c
 *
 *   FreeType utility file for font formats (body).
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


#অন্তর্ভাৱ <freetype/ftfntfmt.h>
#অন্তর্ভাৱ <freetype/internal/ftobjs.h>
#অন্তর্ভাৱ <freetype/internal/services/svfntfmt.h>


  /* documentation is in ftfntfmt.h */

  FT_EXPORT_DEF( ধ্রুৱ ৱর্ণ* )
  FT_Get_Font_Format( FT_Face  face )
  {
    ধ্রুৱ ৱর্ণ*  result = রিক্ত;


    যদি ( face )
      FT_FACE_FIND_SERVICE( face, result, FONT_FORMAT );

    ঘূৰাই result;
  }


  /* deprecated function name; retained for ABI compatibility */

  FT_EXPORT_DEF( ধ্রুৱ ৱর্ণ* )
  FT_Get_X11_Font_Format( FT_Face  face )
  {
    ধ্রুৱ ৱর্ণ*  result = রিক্ত;


    যদি ( face )
      FT_FACE_FIND_SERVICE( face, result, FONT_FORMAT );

    ঘূৰাই result;
  }


/* END */
