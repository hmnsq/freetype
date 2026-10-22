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


#अन्तर्भाव <freetype/ftfntfmt.h>
#अन्तर्भाव <freetype/internal/ftobjs.h>
#अन्तर्भाव <freetype/internal/services/svfntfmt.h>


  /* documentation is in ftfntfmt.h */

  FT_EXPORT_DEF( ध्रुव वर्ण* )
  FT_Get_Font_Format( FT_Face  face )
  {
    ध्रुव वर्ण*  परिणाम = रिक्त;


    यदि ( face )
      FT_FACE_FIND_SERVICE( face, परिणाम, FONT_FORMAT );

    प्रत्यागम परिणाम;
  }


  /* deprecated function name; retained for ABI compatibility */

  FT_EXPORT_DEF( ध्रुव वर्ण* )
  FT_Get_X11_Font_Format( FT_Face  face )
  {
    ध्रुव वर्ण*  परिणाम = रिक्त;


    यदि ( face )
      FT_FACE_FIND_SERVICE( face, परिणाम, FONT_FORMAT );

    प्रत्यागम परिणाम;
  }


/*अन्तः*/
