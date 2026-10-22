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


#شامل <freetype/ftgasp.h>
#شامل <freetype/internal/tttypes.h>


  FT_EXPORT_DEF( FT_Int )
  FT_Get_Gasp( FT_Face  face,
               FT_UInt  ppem )
  {
    FT_Int  result = FT_GASP_NO_TABLE;


    اگر ( face && FT_IS_SFNT( face ) )
    {
      TT_Face  ttface = (TT_Face)face;


      اگر ( ttface->gasp.رقم_Ranges > 0 )
      {
        TT_GaspRange  range     = ttface->gasp.gaspRanges;
        TT_GaspRange  range_end = range + ttface->gasp.رقم_Ranges;


        وقتی ( ppem > range->maxPPEM )
        {
          range++;
          اگر ( range >= range_end )
            برو Exit;
        }

        result = range->gaspFlag;

        /* ensure that we don't have spurious bits */
        اگر ( ttface->gasp.version == 0 )
          result &= 3;
      }
    }
  Exit:
    بازگشت result;
  }


/* END */
