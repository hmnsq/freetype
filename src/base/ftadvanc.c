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
                           FT_UInt    ಎಣಿಕೆ_2,
                           FT_Int32   flags )
  {
    FT_Fixed  scale;
    FT_UInt   nn;


    ಆದರೆ ( flags & FT_ತುಂಬು_NO_SCALE )
      ಹಿಂತಿರುಗಿ FT_Err_Ok;

    ಆದರೆ ( !face->ಗಾತ್ರ_2 )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ಗಾತ್ರ_Handle );

    ಆದರೆ ( flags & FT_ತುಂಬು_VERTICAL_LAYOUT )
      scale = face->ಗಾತ್ರ_2->metrics.y_scale;
    ಇಲ್ಲದಿದ್ದರೆ
      scale = face->ಗಾತ್ರ_2->metrics.x_scale;

    /* this must be the same scaling as to get linear{Hori,Vert}Advance */
    /* (see `FT_Load_Glyph' implementation in src/base/ftobjs.c)        */

    ಗಾಗಿ ( nn = 0; nn < ಎಣಿಕೆ_2; nn++ )
      advances[nn] = FT_MulFix( 1024 * advances[nn], scale );

    ಹಿಂತಿರುಗಿ FT_Err_Ok;
  }

  /* loading (and hinting) to calculate the advances is slow  */
  /* unless TrueType hdmx table is provided as an accelerator */
  ಸ್ಥಿರ FT_Error
  ft_ತುಂಬು_advances( FT_Face   face,
                    FT_UInt   gindex,
                    FT_UInt   ಎಣಿಕೆ_2,
                    FT_Int32  flags,
                    FT_Fixed  *padvances )
  {
    FT_UInt   nn;
    FT_Error  error   = FT_Err_Ok;
    FT_Pos    ಗುಣಕ  = flags & FT_ತುಂಬು_NO_SCALE ? 1 : 1024;
    FT_Pos*   advance = flags & FT_ತುಂಬು_VERTICAL_LAYOUT
                          ? &face->glyph->advance.y
                          : &face->glyph->advance.x;


    flags |= (FT_UInt32)FT_ತುಂಬು_ADVANCE_ONLY;

    ಗಾಗಿ ( nn = 0; nn < ಎಣಿಕೆ_2; nn++ )
    {
      error = FT_ತುಂಬು_Glyph( face, gindex + nn, flags );
      ಆದರೆ ( error )
        ನಿಲ್ಲಿಸು;

      /* scale from 26.6 to 16.16, unless NO_SCALE was requested */
      padvances[nn] = *advance * ಗುಣಕ;
    }

    ಹಿಂತಿರುಗಿ error;
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

#ವ್ಯಾಖ್ಯಾನಿಸು ತುಂಬು_ADVANCE_FAST_CHECK( face, flags )                      \
          ( flags & ( FT_ತುಂಬು_NO_SCALE | FT_ತುಂಬು_NO_HINTING )    || \
            FT_ತುಂಬು_ಗುರಿ_ಬಹುಲಕ( flags ) == FT_RENDER_ಬಹುಲಕ_LIGHT )


  /* documentation is in ftadvanc.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Advance( FT_Face    face,
                  FT_UInt    gindex,
                  FT_Int32   flags,
                  FT_Fixed  *padvance )
  {
    FT_Face_GetAdvancesFunc  func;


    ಆದರೆ ( !face )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_Face_Handle );

    ಆದರೆ ( !padvance )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

    ಆದರೆ ( gindex >= (FT_UInt)face->ಅಂಕೆ_glyphs )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_Glyph_ಸೂಚ್ಯಂಕ );

    func = face->driver->clazz->get_advances;
    ಆದರೆ ( func && ತುಂಬು_ADVANCE_FAST_CHECK( face, flags ) )
    {
      FT_Error  error;


      error = func( face, gindex, 1, flags, padvance );
      ಆದರೆ ( !error )
        ಹಿಂತಿರುಗಿ ft_face_scale_advances_( face, padvance, 1, flags );

      ಆದರೆ ( FT_ERR_NEQ( error, Unimplemented_Feature ) )
        ಹಿಂತಿರುಗಿ error;
    }

    ಆದರೆ ( flags & FT_ADVANCE_FLAG_FAST_ONLY )
      ಹಿಂತಿರುಗಿ FT_THROW( Unimplemented_Feature );

    ಹಿಂತಿರುಗಿ ft_ತುಂಬು_advances( face, gindex, 1, flags, padvance );
  }


  /* documentation is in ftadvanc.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Advances( FT_Face    face,
                   FT_UInt    ಆರಂಭ,
                   FT_UInt    ಎಣಿಕೆ_2,
                   FT_Int32   flags,
                   FT_Fixed  *padvances )
  {
    FT_Face_GetAdvancesFunc  func;

    FT_UInt  ಅಂಕೆ, ಕೊನೆ;


    ಆದರೆ ( !face )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_Face_Handle );

    ಆದರೆ ( !padvances )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

    ಅಂಕೆ = (FT_UInt)face->ಅಂಕೆ_glyphs;
    ಕೊನೆ = ಆರಂಭ + ಎಣಿಕೆ_2;
    ಆದರೆ ( ಆರಂಭ >= ಅಂಕೆ || ಕೊನೆ < ಆರಂಭ || ಕೊನೆ > ಅಂಕೆ )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_Glyph_ಸೂಚ್ಯಂಕ );

    ಆದರೆ ( ಎಣಿಕೆ_2 == 0 )
      ಹಿಂತಿರುಗಿ FT_Err_Ok;

    func = face->driver->clazz->get_advances;
    ಆದರೆ ( func && ತುಂಬು_ADVANCE_FAST_CHECK( face, flags ) )
    {
      FT_Error  error;


      error = func( face, ಆರಂಭ, ಎಣಿಕೆ_2, flags, padvances );
      ಆದರೆ ( !error )
        ಹಿಂತಿರುಗಿ ft_face_scale_advances_( face, padvances, ಎಣಿಕೆ_2, flags );

      ಆದರೆ ( FT_ERR_NEQ( error, Unimplemented_Feature ) )
        ಹಿಂತಿರುಗಿ error;
    }

    ಆದರೆ ( flags & FT_ADVANCE_FLAG_FAST_ONLY )
      ಹಿಂತಿರುಗಿ FT_THROW( Unimplemented_Feature );

    ಹಿಂತಿರುಗಿ ft_ತುಂಬು_advances( face, ಆರಂಭ, ಎಣಿಕೆ_2, flags, padvances );
  }


/*ಕೊನೆ*/
