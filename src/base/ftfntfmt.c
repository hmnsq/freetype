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


#অন্তর্ভাব <freetype/ftfntfmt.h>
#অন্তর্ভাব <freetype/internal/ftobjs.h>
#অন্তর্ভাব <freetype/internal/services/svfntfmt.h>


  /* documentation is in ftfntfmt.h */

  FT_EXPORT_DEF( ধ্রুবক অক্ষর* )
  FT_Get_Font_Format( FT_Face  face )
  {
    ধ্রুবক অক্ষর*  ফলাফল = ফাঁকা;


    যদি ( face )
      FT_FACE_FIND_SERVICE( face, ফলাফল, FONT_FORMAT );

    ফেরত ফলাফল;
  }


  /* deprecated function name; retained for ABI compatibility */

  FT_EXPORT_DEF( ধ্রুবক অক্ষর* )
  FT_Get_X11_Font_Format( FT_Face  face )
  {
    ধ্রুবক অক্ষর*  ফলাফল = ফাঁকা;


    যদি ( face )
      FT_FACE_FIND_SERVICE( face, ফলাফল, FONT_FORMAT );

    ফেরত ফলাফল;
  }


/* END */
