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
    FT_Int  परिणाम = FT_GASP_NO_सारणी;


    यदि ( face && FT_IS_SFNT( face ) )
    {
      TT_Face  ttface = (TT_Face)face;


      यदि ( ttface->gasp.अङ्कः_Ranges > 0 )
      {
        TT_GaspRange  range     = ttface->gasp.gaspRanges;
        TT_GaspRange  range_अन्तः = range + ttface->gasp.अङ्कः_Ranges;


        यावत् ( ppem > range->maxPPEM )
        {
          range++;
          यदि ( range >= range_अन्तः )
            गच्छ Exit;
        }

        परिणाम = range->gaspFlag;

        /* ensure that we don't have spurious bits */
        यदि ( ttface->gasp.संस्करणम् == 0 )
          परिणाम &= 3;
      }
    }
  Exit:
    प्रत्यागम परिणाम;
  }


/*अन्तः*/
