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

#అన్తర్భావ <freetype/freetype.h>
#అన్తర్భావ <freetype/tttags.h>
#అన్తర్భావ <freetype/internal/ftobjs.h>
#అన్తర్భావ <freetype/internal/ftstream.h>
#అన్తర్భావ <freetype/internal/services/svsfnt.h>
#అన్తర్భావ <freetype/internal/services/svttglyf.h>


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Face_CheckTrueTypePatents( FT_Face  face )
  {
    FT_వాడని( face );

    తిరిగి FALSE;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Face_సమితి_Unpatented_Hinting( FT_Face  face,
                                FT_Bool  విలువ )
  {
    FT_వాడని( face );
    FT_వాడని( విలువ );

    తిరిగి FALSE;
  }

/*చివర*/
