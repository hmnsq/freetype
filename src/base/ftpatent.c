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

#समावेश <freetype/freetype.h>
#समावेश <freetype/tttags.h>
#समावेश <freetype/internal/ftobjs.h>
#समावेश <freetype/internal/ftstream.h>
#समावेश <freetype/internal/services/svsfnt.h>
#समावेश <freetype/internal/services/svttglyf.h>


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Face_CheckTrueTypePatents( FT_Face  face )
  {
    FT_UNUSED( face );

    वापस FALSE;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Face_SetUnpatentedHinting( FT_Face  face,
                                FT_Bool  मूल्य )
  {
    FT_UNUSED( face );
    FT_UNUSED( मूल्य );

    वापस FALSE;
  }

/* END */
