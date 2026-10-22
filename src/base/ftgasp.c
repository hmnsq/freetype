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


#અન્તર્ભાવ <freetype/ftgasp.h>
#અન્તર્ભાવ <freetype/internal/tttypes.h>


  FT_EXPORT_DEF( FT_Int )
  FT_Get_Gasp( FT_Face  face,
               FT_UInt  ppem )
  {
    FT_Int  result = FT_GASP_NO_TABLE;


    જો ( face && FT_IS_SFNT( face ) )
    {
      TT_Face  ttface = (TT_Face)face;


      જો ( ttface->gasp.અંક_Ranges > 0 )
      {
        TT_GaspRange  range     = ttface->gasp.gaspRanges;
        TT_GaspRange  range_end = range + ttface->gasp.અંક_Ranges;


        જ્યારે ( ppem > range->maxPPEM )
        {
          range++;
          જો ( range >= range_end )
            જાઓ Exit;
        }

        result = range->gaspFlag;

        /* ensure that we don't have spurious bits */
        જો ( ttface->gasp.version == 0 )
          result &= 3;
      }
    }
  Exit:
    પાછા result;
  }


/* END */
