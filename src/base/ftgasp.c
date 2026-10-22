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


#অন্তর্ভাব <freetype/ftgasp.h>
#অন্তর্ভাব <freetype/internal/tttypes.h>


  FT_EXPORT_DEF( FT_Int )
  FT_Get_Gasp( FT_Face  face,
               FT_UInt  ppem )
  {
    FT_Int  result = FT_GASP_NO_TABLE;


    যদি ( face && FT_IS_SFNT( face ) )
    {
      TT_Face  ttface = (TT_Face)face;


      যদি ( ttface->gasp.অঙ্ক_Ranges > 0 )
      {
        TT_GaspRange  range     = ttface->gasp.gaspRanges;
        TT_GaspRange  range_end = range + ttface->gasp.অঙ্ক_Ranges;


        যাবত্ ( ppem > range->maxPPEM )
        {
          range++;
          যদি ( range >= range_end )
            গচ্ছ Exit;
        }

        result = range->gaspFlag;

        /* ensure that we don't have spurious bits */
        যদি ( ttface->gasp.version == 0 )
          result &= 3;
      }
    }
  Exit:
    প্রত্যাগম result;
  }


/* END */
