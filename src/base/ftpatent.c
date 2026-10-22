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

#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/freetype.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/tttags.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftobjs.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftstream.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/services/svsfnt.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/services/svttglyf.h>


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Face_CheckTrueTypePatents( FT_Face  face )
  {
    FT_ਅਣਵਰਤਿਆ( face );

    ਵਾਪਸ FALSE;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Face_ਸੈੱਟ_Unpatented_Hinting( FT_Face  face,
                                FT_Bool  ਮੁੱਲ )
  {
    FT_ਅਣਵਰਤਿਆ( face );
    FT_ਅਣਵਰਤਿਆ( ਮੁੱਲ );

    ਵਾਪਸ FALSE;
  }

/*ਸਿਰਾ*/
