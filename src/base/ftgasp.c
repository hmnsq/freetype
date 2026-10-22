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


#ضمن <freetype/ftgasp.h>
#ضمن <freetype/internal/tttypes.h>


  FT_EXPORT_DEF( FT_Int )
  FT_Get_Gasp( FT_Face  face,
               FT_UInt  ppem )
  {
    FT_Int  نتیجه = FT_GASP_NO_جدول;


    إذا ( face && FT_IS_SFNT( face ) )
    {
      TT_Face  ttface = (TT_Face)face;


      إذا ( ttface->gasp.رقم_Ranges > 0 )
      {
        TT_GaspRange  range     = ttface->gasp.gaspRanges;
        TT_GaspRange  range_انتها = range + ttface->gasp.رقم_Ranges;


        طالما ( ppem > range->maxPPEM )
        {
          range++;
          إذا ( range >= range_انتها )
            اذهب Exit;
        }

        نتیجه = range->gaspFlag;

        /* ensure that we don't have spurious bits */
        إذا ( ttface->gasp.نسخه == 0 )
          نتیجه &= 3;
      }
    }
  Exit:
    أرجع نتیجه;
  }


/*انتها*/
