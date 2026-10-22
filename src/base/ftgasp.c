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


#अन्तर्भाव <freetype/ftgasp.h>
#अन्तर्भाव <freetype/internal/tttypes.h>


  FT_EXPORT_DEF( FT_Int )
  FT_Get_Gasp( FT_Face  face,
               FT_UInt  ppem )
  {
    FT_Int  result = FT_GASP_NO_TABLE;


    यदि ( face && FT_IS_SFNT( face ) )
    {
      TT_Face  ttface = (TT_Face)face;


      यदि ( ttface->gasp.अङ्क_Ranges > 0 )
      {
        TT_GaspRange  range     = ttface->gasp.gaspRanges;
        TT_GaspRange  range_end = range + ttface->gasp.अङ्क_Ranges;


        जबसम्म ( ppem > range->maxPPEM )
        {
          range++;
          यदि ( range >= range_end )
            गच्छ Exit;
        }

        result = range->gaspFlag;

        /* ensure that we don't have spurious bits */
        यदि ( ttface->gasp.version == 0 )
          result &= 3;
      }
    }
  Exit:
    फर्काऊ result;
  }


/* END */
