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


#ಅನ್ತರ್ಭಾವ <freetype/ftfntfmt.h>
#ಅನ್ತರ್ಭಾವ <freetype/internal/ftobjs.h>
#ಅನ್ತರ್ಭಾವ <freetype/internal/services/svfntfmt.h>


  /* documentation is in ftfntfmt.h */

  FT_EXPORT_DEF( ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* )
  FT_Get_Font_Format( FT_Face  face )
  {
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*  ಫಲಿತಾಂಶ = ಖಾಲಿ;


    ಆದರೆ ( face )
      FT_FACE_ಕಂಡುಹಿಡಿ_SERVICE( face, ಫಲಿತಾಂಶ, FONT_FORMAT );

    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
  }


  /* deprecated function name; retained for ABI compatibility */

  FT_EXPORT_DEF( ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* )
  FT_Get_X11_Font_Format( FT_Face  face )
  {
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*  ಫಲಿತಾಂಶ = ಖಾಲಿ;


    ಆದರೆ ( face )
      FT_FACE_ಕಂಡುಹಿಡಿ_SERVICE( face, ಫಲಿತಾಂಶ, FONT_FORMAT );

    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
  }


/*ಕೊನೆ*/
