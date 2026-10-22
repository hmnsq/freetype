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
                           FT_UInt    गन्ती,
                           FT_Int32   flags )
  {
    FT_Fixed  scale;
    FT_UInt   nn;


    यदि ( flags & FT_LOAD_NO_SCALE )
      फर्काऊ FT_Err_Ok;

    यदि ( !face->size )
      फर्काऊ FT_THROW( Invalid_Size_Handle );

    यदि ( flags & FT_LOAD_VERTICAL_LAYOUT )
      scale = face->size->metrics.y_scale;
    अन्यथा
      scale = face->size->metrics.x_scale;

    /* this must be the same scaling as to get linear{Hori,Vert}Advance */
    /* (see `FT_Load_Glyph' implementation in src/base/ftobjs.c)        */

    लागि ( nn = 0; nn < गन्ती; nn++ )
      advances[nn] = FT_MulFix( 1024 * advances[nn], scale );

    फर्काऊ FT_Err_Ok;
  }

  /* loading (and hinting) to calculate the advances is slow  */
  /* unless TrueType hdmx table is provided as an accelerator */
  स्थिर FT_Error
  ft_load_advances( FT_Face   face,
                    FT_UInt   gindex,
                    FT_UInt   गन्ती,
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

    लागि ( nn = 0; nn < गन्ती; nn++ )
    {
      error = FT_Load_Glyph( face, gindex + nn, flags );
      यदि ( error )
        रोक;

      /* scale from 26.6 to 16.16, unless NO_SCALE was requested */
      padvances[nn] = *advance * factor;
    }

    फर्काऊ error;
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


    यदि ( !face )
      फर्काऊ FT_THROW( Invalid_Face_Handle );

    यदि ( !padvance )
      फर्काऊ FT_THROW( Invalid_Argument );

    यदि ( gindex >= (FT_UInt)face->अङ्क_glyphs )
      फर्काऊ FT_THROW( Invalid_Glyph_Index );

    func = face->driver->clazz->get_advances;
    यदि ( func && LOAD_ADVANCE_FAST_CHECK( face, flags ) )
    {
      FT_Error  error;


      error = func( face, gindex, 1, flags, padvance );
      यदि ( !error )
        फर्काऊ ft_face_scale_advances_( face, padvance, 1, flags );

      यदि ( FT_ERR_NEQ( error, Unimplemented_Feature ) )
        फर्काऊ error;
    }

    यदि ( flags & FT_ADVANCE_FLAG_FAST_ONLY )
      फर्काऊ FT_THROW( Unimplemented_Feature );

    फर्काऊ ft_load_advances( face, gindex, 1, flags, padvance );
  }


  /* documentation is in ftadvanc.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Advances( FT_Face    face,
                   FT_UInt    start,
                   FT_UInt    गन्ती,
                   FT_Int32   flags,
                   FT_Fixed  *padvances )
  {
    FT_Face_GetAdvancesFunc  func;

    FT_UInt  अङ्क, end;


    यदि ( !face )
      फर्काऊ FT_THROW( Invalid_Face_Handle );

    यदि ( !padvances )
      फर्काऊ FT_THROW( Invalid_Argument );

    अङ्क = (FT_UInt)face->अङ्क_glyphs;
    end = start + गन्ती;
    यदि ( start >= अङ्क || end < start || end > अङ्क )
      फर्काऊ FT_THROW( Invalid_Glyph_Index );

    यदि ( गन्ती == 0 )
      फर्काऊ FT_Err_Ok;

    func = face->driver->clazz->get_advances;
    यदि ( func && LOAD_ADVANCE_FAST_CHECK( face, flags ) )
    {
      FT_Error  error;


      error = func( face, start, गन्ती, flags, padvances );
      यदि ( !error )
        फर्काऊ ft_face_scale_advances_( face, padvances, गन्ती, flags );

      यदि ( FT_ERR_NEQ( error, Unimplemented_Feature ) )
        फर्काऊ error;
    }

    यदि ( flags & FT_ADVANCE_FLAG_FAST_ONLY )
      फर्काऊ FT_THROW( Unimplemented_Feature );

    फर्काऊ ft_load_advances( face, start, गन्ती, flags, padvances );
  }


/* END */
