/****************************************************************************
 *
 * ftadvanc.c
 *
 *   Quick computation of advance widths (body).
 *
 * Copyright (C) 2008-2026 by
 * David Turner, Robert Wilhelm, and Werner Lemberg.
 *
 * This file is part of the FreeType project, and may only be used,
 * modified, and distributed under the terms of the FreeType project
 * license, LICENSE.TXT.  By continuing to use, modify, or distribute
 * this file you indicate that you have read the license and
 * understand and accept it fully.
 *
 */


#شامل <freetype/internal/ftdebug.h>

#شامل <freetype/ftadvanc.h>
#شامل <freetype/internal/ftobjs.h>
#شامل <freetype/internal/ftcalc.h>


  ساکن FT_Error
  ft_face_scale_advances_( FT_Face    face,
                           FT_Fixed*  advances,
                           FT_UInt    گنتی,
                           FT_Int32   flags )
  {
    FT_Fixed  scale;
    FT_UInt   nn;


    اگر ( flags & FT_بھرو_NO_SCALE )
      واپس FT_Err_Ok;

    اگر ( !face->سائز )
      واپس FT_THROW( Invalid_سائز_Handle );

    اگر ( flags & FT_بھرو_VERTICAL_LAYOUT )
      scale = face->سائز->metrics.y_scale;
    ورنہ
      scale = face->سائز->metrics.x_scale;

    /* this must be the same scaling as to get linear{Hori,Vert}Advance */
    /* (see `FT_Load_Glyph' implementation in src/base/ftobjs.c)        */

    برائے ( nn = 0; nn < گنتی; nn++ )
      advances[nn] = FT_MulFix( 1024 * advances[nn], scale );

    واپس FT_Err_Ok;
  }

  /* loading (and hinting) to calculate the advances is slow  */
  /* unless TrueType hdmx table is provided as an accelerator */
  ساکن FT_Error
  ft_بھرو_advances( FT_Face   face,
                    FT_UInt   gindex,
                    FT_UInt   گنتی,
                    FT_Int32  flags,
                    FT_Fixed  *padvances )
  {
    FT_UInt   nn;
    FT_Error  error   = FT_Err_Ok;
    FT_Pos    ضربی_عدد  = flags & FT_بھرو_NO_SCALE ? 1 : 1024;
    FT_Pos*   advance = flags & FT_بھرو_VERTICAL_LAYOUT
                          ? &face->glyph->advance.y
                          : &face->glyph->advance.x;


    flags |= (FT_UInt32)FT_بھرو_ADVANCE_ONLY;

    برائے ( nn = 0; nn < گنتی; nn++ )
    {
      error = FT_بھرو_Glyph( face, gindex + nn, flags );
      اگر ( error )
        توڑو;

      /* scale from 26.6 to 16.16, unless NO_SCALE was requested */
      padvances[nn] = *advance * ضربی_عدد;
    }

    واپس error;
  }


   /* at the moment, we can perform fast advance retrieval only in */
   /* the following cases:                                         */
   /*                                                              */
   /*  - unscaled load                                             */
   /*  - unhinted load                                             */
   /*  - light-hinted load                                         */
   /*  - if a variations font, it must have an `HVAR' or `VVAR'    */
   /*    table (thus the old MM or GX fonts don't qualify; this    */
   /*    gets checked by the driver-specific functions)            */

#تعریف_کرو بھرو_ADVANCE_FAST_CHECK( face, flags )                      \
          ( flags & ( FT_بھرو_NO_SCALE | FT_بھرو_NO_HINTING )    || \
            FT_بھرو_ہدف_منوال( flags ) == FT_RENDER_منوال_LIGHT )


  /* documentation is in ftadvanc.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Advance( FT_Face    face,
                  FT_UInt    gindex,
                  FT_Int32   flags,
                  FT_Fixed  *padvance )
  {
    FT_Face_GetAdvancesFunc  func;


    اگر ( !face )
      واپس FT_THROW( Invalid_Face_Handle );

    اگر ( !padvance )
      واپس FT_THROW( Invalid_دلیل );

    اگر ( gindex >= (FT_UInt)face->عدد_glyphs )
      واپس FT_THROW( Invalid_Glyph_اشاریہ );

    func = face->driver->clazz->get_advances;
    اگر ( func && بھرو_ADVANCE_FAST_CHECK( face, flags ) )
    {
      FT_Error  error;


      error = func( face, gindex, 1, flags, padvance );
      اگر ( !error )
        واپس ft_face_scale_advances_( face, padvance, 1, flags );

      اگر ( FT_ERR_NEQ( error, Unimplemented_Feature ) )
        واپس error;
    }

    اگر ( flags & FT_ADVANCE_FLAG_FAST_ONLY )
      واپس FT_THROW( Unimplemented_Feature );

    واپس ft_بھرو_advances( face, gindex, 1, flags, padvance );
  }


  /* documentation is in ftadvanc.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Advances( FT_Face    face,
                   FT_UInt    آغاز,
                   FT_UInt    گنتی,
                   FT_Int32   flags,
                   FT_Fixed  *padvances )
  {
    FT_Face_GetAdvancesFunc  func;

    FT_UInt  عدد, انتہا;


    اگر ( !face )
      واپس FT_THROW( Invalid_Face_Handle );

    اگر ( !padvances )
      واپس FT_THROW( Invalid_دلیل );

    عدد = (FT_UInt)face->عدد_glyphs;
    انتہا = آغاز + گنتی;
    اگر ( آغاز >= عدد || انتہا < آغاز || انتہا > عدد )
      واپس FT_THROW( Invalid_Glyph_اشاریہ );

    اگر ( گنتی == 0 )
      واپس FT_Err_Ok;

    func = face->driver->clazz->get_advances;
    اگر ( func && بھرو_ADVANCE_FAST_CHECK( face, flags ) )
    {
      FT_Error  error;


      error = func( face, آغاز, گنتی, flags, padvances );
      اگر ( !error )
        واپس ft_face_scale_advances_( face, padvances, گنتی, flags );

      اگر ( FT_ERR_NEQ( error, Unimplemented_Feature ) )
        واپس error;
    }

    اگر ( flags & FT_ADVANCE_FLAG_FAST_ONLY )
      واپس FT_THROW( Unimplemented_Feature );

    واپس ft_بھرو_advances( face, آغاز, گنتی, flags, padvances );
  }


/*انتہا*/
