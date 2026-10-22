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

#ಅನ್ತರ್ಭಾವ <freetype/freetype.h>
#ಅನ್ತರ್ಭಾವ <freetype/tttags.h>
#ಅನ್ತರ್ಭಾವ <freetype/internal/ftobjs.h>
#ಅನ್ತರ್ಭಾವ <freetype/internal/ftstream.h>
#ಅನ್ತರ್ಭಾವ <freetype/internal/services/svsfnt.h>
#ಅನ್ತರ್ಭಾವ <freetype/internal/services/svttglyf.h>


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Face_CheckTrueTypePatents( FT_Face  face )
  {
    FT_ಬಳಸದ( face );

    ಹಿಂತಿರುಗಿ FALSE;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Face_ಗಣ_Unpatented_Hinting( FT_Face  face,
                                FT_Bool  ಮೌಲ್ಯ )
  {
    FT_ಬಳಸದ( face );
    FT_ಬಳಸದ( ಮೌಲ್ಯ );

    ಹಿಂತಿರುಗಿ FALSE;
  }

/*ಕೊನೆ*/
