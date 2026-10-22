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


#ಅನ್ತರ್ಭಾವ <freetype/ftgasp.h>
#ಅನ್ತರ್ಭಾವ <freetype/internal/tttypes.h>


  FT_EXPORT_DEF( FT_Int )
  FT_Get_Gasp( FT_Face  face,
               FT_UInt  ppem )
  {
    FT_Int  ಫಲಿತಾಂಶ = FT_GASP_NO_ಕೋಷ್ಟಕ;


    ಆದರೆ ( face && FT_IS_SFNT( face ) )
    {
      TT_Face  ttface = (TT_Face)face;


      ಆದರೆ ( ttface->gasp.ಅಂಕೆ_Ranges > 0 )
      {
        TT_GaspRange  range     = ttface->gasp.gaspRanges;
        TT_GaspRange  range_ಕೊನೆ = range + ttface->gasp.ಅಂಕೆ_Ranges;


        ಆಗ ( ppem > range->maxPPEM )
        {
          range++;
          ಆದರೆ ( range >= range_ಕೊನೆ )
            ಹೋಗು Exit;
        }

        ಫಲಿತಾಂಶ = range->gaspFlag;

        /* ensure that we don't have spurious bits */
        ಆದರೆ ( ttface->gasp.ಆವೃತ್ತಿ == 0 )
          ಫಲಿತಾಂಶ &= 3;
      }
    }
  Exit:
    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
  }


/*ಕೊನೆ*/
