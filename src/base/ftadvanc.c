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


#अन्तर्भाव <freetype/internal/ftdebug.h>

#अन्तर्भाव <freetype/ftadvanc.h>
#अन्तर्भाव <freetype/internal/ftobjs.h>
#अन्तर्भाव <freetype/internal/ftcalc.h>


  स्थिर FT_Error
  ft_face_scale_advances_( FT_Face    face,
                           FT_Fixed*  advances,
                           FT_UInt    गणनम्,
                           FT_Int32   flags )
  {
    FT_Fixed  scale;
    FT_UInt   nn;


    यदि ( flags & FT_आरोपय_NO_SCALE )
      प्रत्यागम FT_Err_Ok;

    यदि ( !face->आकार )
      प्रत्यागम FT_THROW( Invalid_आकार_Handle );

    यदि ( flags & FT_आरोपय_VERTICAL_LAYOUT )
      scale = face->आकार->metrics.y_scale;
    अन्यथा
      scale = face->आकार->metrics.x_scale;

    /* this must be the same scaling as to get linear{Hori,Vert}Advance */
    /* (see `FT_Load_Glyph' implementation in src/base/ftobjs.c)        */

    प्रति ( nn = 0; nn < गणनम्; nn++ )
      advances[nn] = FT_MulFix( 1024 * advances[nn], scale );

    प्रत्यागम FT_Err_Ok;
  }

  /* loading (and hinting) to calculate the advances is slow  */
  /* unless TrueType hdmx table is provided as an accelerator */
  स्थिर FT_Error
  ft_आरोपय_advances( FT_Face   face,
                    FT_UInt   gindex,
                    FT_UInt   गणनम्,
                    FT_Int32  flags,
                    FT_Fixed  *padvances )
  {
    FT_UInt   nn;
    FT_Error  error   = FT_Err_Ok;
    FT_Pos    गुणकः  = flags & FT_आरोपय_NO_SCALE ? 1 : 1024;
    FT_Pos*   advance = flags & FT_आरोपय_VERTICAL_LAYOUT
                          ? &face->glyph->advance.y
                          : &face->glyph->advance.x;


    flags |= (FT_UInt32)FT_आरोपय_ADVANCE_ONLY;

    प्रति ( nn = 0; nn < गणनम्; nn++ )
    {
      error = FT_आरोपय_Glyph( face, gindex + nn, flags );
      यदि ( error )
        भङ्ग;

      /* scale from 26.6 to 16.16, unless NO_SCALE was requested */
      padvances[nn] = *advance * गुणकः;
    }

    प्रत्यागम error;
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

#परिभाषय आरोपय_ADVANCE_FAST_CHECK( face, flags )                      \
          ( flags & ( FT_आरोपय_NO_SCALE | FT_आरोपय_NO_HINTING )    || \
            FT_आरोपय_TARGET_बहुलकम्( flags ) == FT_RENDER_बहुलकम्_LIGHT )


  /* documentation is in ftadvanc.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Advance( FT_Face    face,
                  FT_UInt    gindex,
                  FT_Int32   flags,
                  FT_Fixed  *padvance )
  {
    FT_Face_GetAdvancesFunc  func;


    यदि ( !face )
      प्रत्यागम FT_THROW( Invalid_Face_Handle );

    यदि ( !padvance )
      प्रत्यागम FT_THROW( Invalid_तर्कः );

    यदि ( gindex >= (FT_UInt)face->अङ्कः_glyphs )
      प्रत्यागम FT_THROW( Invalid_Glyph_सूचकांक );

    func = face->driver->clazz->get_advances;
    यदि ( func && आरोपय_ADVANCE_FAST_CHECK( face, flags ) )
    {
      FT_Error  error;


      error = func( face, gindex, 1, flags, padvance );
      यदि ( !error )
        प्रत्यागम ft_face_scale_advances_( face, padvance, 1, flags );

      यदि ( FT_ERR_NEQ( error, Unimplemented_Feature ) )
        प्रत्यागम error;
    }

    यदि ( flags & FT_ADVANCE_FLAG_FAST_ONLY )
      प्रत्यागम FT_THROW( Unimplemented_Feature );

    प्रत्यागम ft_आरोपय_advances( face, gindex, 1, flags, padvance );
  }


  /* documentation is in ftadvanc.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Advances( FT_Face    face,
                   FT_UInt    start,
                   FT_UInt    गणनम्,
                   FT_Int32   flags,
                   FT_Fixed  *padvances )
  {
    FT_Face_GetAdvancesFunc  func;

    FT_UInt  अङ्कः, अन्तः;


    यदि ( !face )
      प्रत्यागम FT_THROW( Invalid_Face_Handle );

    यदि ( !padvances )
      प्रत्यागम FT_THROW( Invalid_तर्कः );

    अङ्कः = (FT_UInt)face->अङ्कः_glyphs;
    अन्तः = start + गणनम्;
    यदि ( start >= अङ्कः || अन्तः < start || अन्तः > अङ्कः )
      प्रत्यागम FT_THROW( Invalid_Glyph_सूचकांक );

    यदि ( गणनम् == 0 )
      प्रत्यागम FT_Err_Ok;

    func = face->driver->clazz->get_advances;
    यदि ( func && आरोपय_ADVANCE_FAST_CHECK( face, flags ) )
    {
      FT_Error  error;


      error = func( face, start, गणनम्, flags, padvances );
      यदि ( !error )
        प्रत्यागम ft_face_scale_advances_( face, padvances, गणनम्, flags );

      यदि ( FT_ERR_NEQ( error, Unimplemented_Feature ) )
        प्रत्यागम error;
    }

    यदि ( flags & FT_ADVANCE_FLAG_FAST_ONLY )
      प्रत्यागम FT_THROW( Unimplemented_Feature );

    प्रत्यागम ft_आरोपय_advances( face, start, गणनम्, flags, padvances );
  }


/*अन्तः*/
