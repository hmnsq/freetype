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


#అన్తర్భావ <freetype/ftfntfmt.h>
#అన్తర్భావ <freetype/internal/ftobjs.h>
#అన్తర్భావ <freetype/internal/services/svfntfmt.h>


  /* documentation is in ftfntfmt.h */

  FT_EXPORT_DEF( స్థిరరాశి అక్షరం* )
  FT_Get_Font_Format( FT_Face  face )
  {
    స్థిరరాశి అక్షరం*  ఫలితం = ఖాళీ;


    ఒకవేళ ( face )
      FT_FACE_కనుగొను_SERVICE( face, ఫలితం, FONT_FORMAT );

    తిరిగి ఫలితం;
  }


  /* deprecated function name; retained for ABI compatibility */

  FT_EXPORT_DEF( స్థిరరాశి అక్షరం* )
  FT_Get_X11_Font_Format( FT_Face  face )
  {
    స్థిరరాశి అక్షరం*  ఫలితం = ఖాళీ;


    ఒకవేళ ( face )
      FT_FACE_కనుగొను_SERVICE( face, ఫలితం, FONT_FORMAT );

    తిరిగి ఫలితం;
  }


/*చివర*/
