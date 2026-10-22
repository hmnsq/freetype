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


#ಅನ್ತರ್ಭಾವ <freetype/internal/ftdebug.h>

#ಅನ್ತರ್ಭಾವ <freetype/ftadvanc.h>
#ಅನ್ತರ್ಭಾವ <freetype/internal/ftobjs.h>
#ಅನ್ತರ್ಭಾವ <freetype/internal/ftcalc.h>


  ಸ್ಥಿರ FT_Error
  ft_face_scale_advances_( FT_Face    face,
                           FT_Fixed*  advances,
                           FT_UInt    ಎಣಿಕೆ,
                           FT_Int32   flags )
  {
    FT_Fixed  scale;
    FT_UInt   nn;


    ಯದಿ ( flags & FT_LOAD_NO_SCALE )
      ಪ್ರತ್ಯಾಗಮ FT_Err_Ok;

    ಯದಿ ( !face->size )
      ಪ್ರತ್ಯಾಗಮ FT_THROW( Invalid_Size_Handle );

    ಯದಿ ( flags & FT_LOAD_VERTICAL_LAYOUT )
      scale = face->size->metrics.y_scale;
    ಅನ್ಯಥಾ
      scale = face->size->metrics.x_scale;

    /* this must be the same scaling as to get linear{Hori,Vert}Advance */
    /* (see `FT_Load_Glyph' implementation in src/base/ftobjs.c)        */

    ಪ್ರತಿ ( nn = 0; nn < ಎಣಿಕೆ; nn++ )
      advances[nn] = FT_MulFix( 1024 * advances[nn], scale );

    ಪ್ರತ್ಯಾಗಮ FT_Err_Ok;
  }

  /* loading (and hinting) to calculate the advances is slow  */
  /* unless TrueType hdmx table is provided as an accelerator */
  ಸ್ಥಿರ FT_Error
  ft_load_advances( FT_Face   face,
                    FT_UInt   gindex,
                    FT_UInt   ಎಣಿಕೆ,
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

    ಪ್ರತಿ ( nn = 0; nn < ಎಣಿಕೆ; nn++ )
    {
      error = FT_Load_Glyph( face, gindex + nn, flags );
      ಯದಿ ( error )
        ಭಙ್ಗ;

      /* scale from 26.6 to 16.16, unless NO_SCALE was requested */
      padvances[nn] = *advance * factor;
    }

    ಪ್ರತ್ಯಾಗಮ error;
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


    ಯದಿ ( !face )
      ಪ್ರತ್ಯಾಗಮ FT_THROW( Invalid_Face_Handle );

    ಯದಿ ( !padvance )
      ಪ್ರತ್ಯಾಗಮ FT_THROW( Invalid_Argument );

    ಯದಿ ( gindex >= (FT_UInt)face->ಅಂಕೆ_glyphs )
      ಪ್ರತ್ಯಾಗಮ FT_THROW( Invalid_Glyph_Index );

    func = face->driver->clazz->get_advances;
    ಯದಿ ( func && LOAD_ADVANCE_FAST_CHECK( face, flags ) )
    {
      FT_Error  error;


      error = func( face, gindex, 1, flags, padvance );
      ಯದಿ ( !error )
        ಪ್ರತ್ಯಾಗಮ ft_face_scale_advances_( face, padvance, 1, flags );

      ಯದಿ ( FT_ERR_NEQ( error, Unimplemented_Feature ) )
        ಪ್ರತ್ಯಾಗಮ error;
    }

    ಯದಿ ( flags & FT_ADVANCE_FLAG_FAST_ONLY )
      ಪ್ರತ್ಯಾಗಮ FT_THROW( Unimplemented_Feature );

    ಪ್ರತ್ಯಾಗಮ ft_load_advances( face, gindex, 1, flags, padvance );
  }


  /* documentation is in ftadvanc.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Advances( FT_Face    face,
                   FT_UInt    start,
                   FT_UInt    ಎಣಿಕೆ,
                   FT_Int32   flags,
                   FT_Fixed  *padvances )
  {
    FT_Face_GetAdvancesFunc  func;

    FT_UInt  ಅಂಕೆ, end;


    ಯದಿ ( !face )
      ಪ್ರತ್ಯಾಗಮ FT_THROW( Invalid_Face_Handle );

    ಯದಿ ( !padvances )
      ಪ್ರತ್ಯಾಗಮ FT_THROW( Invalid_Argument );

    ಅಂಕೆ = (FT_UInt)face->ಅಂಕೆ_glyphs;
    end = start + ಎಣಿಕೆ;
    ಯದಿ ( start >= ಅಂಕೆ || end < start || end > ಅಂಕೆ )
      ಪ್ರತ್ಯಾಗಮ FT_THROW( Invalid_Glyph_Index );

    ಯದಿ ( ಎಣಿಕೆ == 0 )
      ಪ್ರತ್ಯಾಗಮ FT_Err_Ok;

    func = face->driver->clazz->get_advances;
    ಯದಿ ( func && LOAD_ADVANCE_FAST_CHECK( face, flags ) )
    {
      FT_Error  error;


      error = func( face, start, ಎಣಿಕೆ, flags, padvances );
      ಯದಿ ( !error )
        ಪ್ರತ್ಯಾಗಮ ft_face_scale_advances_( face, padvances, ಎಣಿಕೆ, flags );

      ಯದಿ ( FT_ERR_NEQ( error, Unimplemented_Feature ) )
        ಪ್ರತ್ಯಾಗಮ error;
    }

    ಯದಿ ( flags & FT_ADVANCE_FLAG_FAST_ONLY )
      ಪ್ರತ್ಯಾಗಮ FT_THROW( Unimplemented_Feature );

    ಪ್ರತ್ಯಾಗಮ ft_load_advances( face, start, ಎಣಿಕೆ, flags, padvances );
  }


/* END */
