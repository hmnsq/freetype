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


    सचे ( flags & FT_LOAD_NO_SCALE )
      पटिनिवत्त FT_Err_Ok;

    सचे ( !face->size )
      पटिनिवत्त FT_THROW( Invalid_Size_Handle );

    सचे ( flags & FT_LOAD_VERTICAL_LAYOUT )
      scale = face->size->metrics.y_scale;
    अञ्ञथा
      scale = face->size->metrics.x_scale;

    /* this must be the same scaling as to get linear{Hori,Vert}Advance */
    /* (see `FT_Load_Glyph' implementation in src/base/ftobjs.c)        */

    अत्थाय ( nn = 0; nn < गणनम्; nn++ )
      advances[nn] = FT_MulFix( 1024 * advances[nn], scale );

    पटिनिवत्त FT_Err_Ok;
  }

  /* loading (and hinting) to calculate the advances is slow  */
  /* unless TrueType hdmx table is provided as an accelerator */
  स्थिर FT_Error
  ft_load_advances( FT_Face   face,
                    FT_UInt   gindex,
                    FT_UInt   गणनम्,
                    FT_Int32  flags,
                    FT_Fixed  *padvances )
  {
    FT_UInt   nn;
    FT_Error  error   = FT_Err_Ok;
    FT_Pos    factor  = flags & FT_LOAD_NO_SCALE ? 1 : 1024;
    FT_Pos*   advance = flags & FT_LOAD_VERTICAL_LAYOUT
                          ? &face->glyph->advance.y
                          : &face->glyph->advance.x;


    flags |= (FT_UInt32)FT_LOAD_ADVANCE_ONLY;

    अत्थाय ( nn = 0; nn < गणनम्; nn++ )
    {
      error = FT_Load_Glyph( face, gindex + nn, flags );
      सचे ( error )
        भङ्ग;

      /* scale from 26.6 to 16.16, unless NO_SCALE was requested */
      padvances[nn] = *advance * factor;
    }

    पटिनिवत्त error;
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

#define LOAD_ADVANCE_FAST_CHECK( face, flags )                      \
          ( flags & ( FT_LOAD_NO_SCALE | FT_LOAD_NO_HINTING )    || \
            FT_LOAD_TARGET_MODE( flags ) == FT_RENDER_MODE_LIGHT )


  /* documentation is in ftadvanc.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Advance( FT_Face    face,
                  FT_UInt    gindex,
                  FT_Int32   flags,
                  FT_Fixed  *padvance )
  {
    FT_Face_GetAdvancesFunc  func;


    सचे ( !face )
      पटिनिवत्त FT_THROW( Invalid_Face_Handle );

    सचे ( !padvance )
      पटिनिवत्त FT_THROW( Invalid_Argument );

    सचे ( gindex >= (FT_UInt)face->अङ्कः_glyphs )
      पटिनिवत्त FT_THROW( Invalid_Glyph_Index );

    func = face->driver->clazz->get_advances;
    सचे ( func && LOAD_ADVANCE_FAST_CHECK( face, flags ) )
    {
      FT_Error  error;


      error = func( face, gindex, 1, flags, padvance );
      सचे ( !error )
        पटिनिवत्त ft_face_scale_advances_( face, padvance, 1, flags );

      सचे ( FT_ERR_NEQ( error, Unimplemented_Feature ) )
        पटिनिवत्त error;
    }

    सचे ( flags & FT_ADVANCE_FLAG_FAST_ONLY )
      पटिनिवत्त FT_THROW( Unimplemented_Feature );

    पटिनिवत्त ft_load_advances( face, gindex, 1, flags, padvance );
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

    FT_UInt  अङ्कः, end;


    सचे ( !face )
      पटिनिवत्त FT_THROW( Invalid_Face_Handle );

    सचे ( !padvances )
      पटिनिवत्त FT_THROW( Invalid_Argument );

    अङ्कः = (FT_UInt)face->अङ्कः_glyphs;
    end = start + गणनम्;
    सचे ( start >= अङ्कः || end < start || end > अङ्कः )
      पटिनिवत्त FT_THROW( Invalid_Glyph_Index );

    सचे ( गणनम् == 0 )
      पटिनिवत्त FT_Err_Ok;

    func = face->driver->clazz->get_advances;
    सचे ( func && LOAD_ADVANCE_FAST_CHECK( face, flags ) )
    {
      FT_Error  error;


      error = func( face, start, गणनम्, flags, padvances );
      सचे ( !error )
        पटिनिवत्त ft_face_scale_advances_( face, padvances, गणनम्, flags );

      सचे ( FT_ERR_NEQ( error, Unimplemented_Feature ) )
        पटिनिवत्त error;
    }

    सचे ( flags & FT_ADVANCE_FLAG_FAST_ONLY )
      पटिनिवत्त FT_THROW( Unimplemented_Feature );

    पटिनिवत्त ft_load_advances( face, start, गणनम्, flags, padvances );
  }


/* END */
