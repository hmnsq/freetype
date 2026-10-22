/****************************************************************************
 *
 * ftgasp.c
 *
 *   Access of TrueType's `gasp' table (body).
 *
 * Copyright (C) 2007-2026 by
 * David Turner, Robert Wilhelm, and Werner Lemberg.
 *
 * This file is part of the FreeType project, and may only be used,
 * modified, and distributed under the terms of the FreeType project
 * license, LICENSE.TXT.  By continuing to use, modify, or distribute
 * this file you indicate that you have read the license and
 * understand and accept it fully.
 *
 */


#అన్తర్భావ <freetype/ftgasp.h>
#అన్తర్భావ <freetype/internal/tttypes.h>


  FT_EXPORT_DEF( FT_Int )
  FT_Get_Gasp( FT_Face  face,
               FT_UInt  ppem )
  {
    FT_Int  ఫలితం = FT_GASP_NO_పట్టిక;


    ఒకవేళ ( face && FT_IS_SFNT( face ) )
    {
      TT_Face  ttface = (TT_Face)face;


      ఒకవేళ ( ttface->gasp.అంకె_Ranges > 0 )
      {
        TT_GaspRange  range     = ttface->gasp.gaspRanges;
        TT_GaspRange  range_చివర = range + ttface->gasp.అంకె_Ranges;


        అయితే ( ppem > range->maxPPEM )
        {
          range++;
          ఒకవేళ ( range >= range_చివర )
            వెళ్ళు Exit;
        }

        ఫలితం = range->gaspFlag;

        /* ensure that we don't have spurious bits */
        ఒకవేళ ( ttface->gasp.సంస్కరణ == 0 )
          ఫలితం &= 3;
      }
    }
  Exit:
    తిరిగి ఫలితం;
  }


/*చివర*/
