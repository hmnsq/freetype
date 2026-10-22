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


#அந்தர்பாவ <freetype/ftgasp.h>
#அந்தர்பாவ <freetype/internal/tttypes.h>


  FT_EXPORT_DEF( FT_Int )
  FT_Get_Gasp( FT_Face  face,
               FT_UInt  ppem )
  {
    FT_Int  result = FT_GASP_NO_TABLE;


    எனில் ( face && FT_IS_SFNT( face ) )
    {
      TT_Face  ttface = (TT_Face)face;


      எனில் ( ttface->gasp.இலக்கம்_Ranges > 0 )
      {
        TT_GaspRange  range     = ttface->gasp.gaspRanges;
        TT_GaspRange  range_end = range + ttface->gasp.இலக்கம்_Ranges;


        போது ( ppem > range->maxPPEM )
        {
          range++;
          எனில் ( range >= range_end )
            செல் Exit;
        }

        result = range->gaspFlag;

        /* ensure that we don't have spurious bits */
        எனில் ( ttface->gasp.version == 0 )
          result &= 3;
      }
    }
  Exit:
    திரும்பு result;
  }


/* END */
