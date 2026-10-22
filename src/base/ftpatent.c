/****************************************************************************
 *
 * ftpatent.c
 *
 *   FreeType API for checking patented TrueType bytecode instructions
 *   (body).  Obsolete, retained for backward compatibility.
 *
 * Copyright (C) 2007-2026 by
 * David Turner.
 *
 * This file is part of the FreeType project, and may only be used,
 * modified, and distributed under the terms of the FreeType project
 * license, LICENSE.TXT.  By continuing to use, modify, or distribute
 * this file you indicate that you have read the license and
 * understand and accept it fully.
 *
 */

#അന്തര്ഭാവ <freetype/freetype.h>
#അന്തര്ഭാവ <freetype/tttags.h>
#അന്തര്ഭാവ <freetype/internal/ftobjs.h>
#അന്തര്ഭാവ <freetype/internal/ftstream.h>
#അന്തര്ഭാവ <freetype/internal/services/svsfnt.h>
#അന്തര്ഭാവ <freetype/internal/services/svttglyf.h>


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Face_CheckTrueTypePatents( FT_Face  face )
  {
    FT_UNUSED( face );

    തിരികെ FALSE;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Face_SetUnpatentedHinting( FT_Face  face,
                                FT_Bool  മൂല്യം )
  {
    FT_UNUSED( face );
    FT_UNUSED( മൂല്യം );

    തിരികെ FALSE;
  }

/* END */
