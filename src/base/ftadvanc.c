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


#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftdebug.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/ftadvanc.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftobjs.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftcalc.h>


  ਸਥਿਰ FT_Error
  ft_face_scale_advances_( FT_Face    face,
                           FT_Fixed*  advances,
                           FT_UInt    ਗਿਣਤੀ_2,
                           FT_Int32   flags )
  {
    FT_Fixed  scale;
    FT_UInt   nn;


    ਜੇ ( flags & FT_ਲੱਦੋ_NO_SCALE )
      ਵਾਪਸ FT_Err_Ok;

    ਜੇ ( !face->ਮਾਪ )
      ਵਾਪਸ FT_THROW( Invalid_ਮਾਪ_Handle );

    ਜੇ ( flags & FT_ਲੱਦੋ_VERTICAL_LAYOUT )
      scale = face->ਮਾਪ->metrics.y_scale;
    ਨਹੀਂਤਾਂ
      scale = face->ਮਾਪ->metrics.x_scale;

    /* this must be the same scaling as to get linear{Hori,Vert}Advance */
    /* (see `FT_Load_Glyph' implementation in src/base/ftobjs.c)        */

    ਲਈ ( nn = 0; nn < ਗਿਣਤੀ_2; nn++ )
      advances[nn] = FT_MulFix( 1024 * advances[nn], scale );

    ਵਾਪਸ FT_Err_Ok;
  }

  /* loading (and hinting) to calculate the advances is slow  */
  /* unless TrueType hdmx table is provided as an accelerator */
  ਸਥਿਰ FT_Error
  ft_ਲੱਦੋ_advances( FT_Face   face,
                    FT_UInt   gindex,
                    FT_UInt   ਗਿਣਤੀ_2,
                    FT_Int32  flags,
                    FT_Fixed  *padvances )
  {
    FT_UInt   nn;
    FT_Error  error   = FT_Err_Ok;
    FT_Pos    ਗੁਣਕ  = flags & FT_ਲੱਦੋ_NO_SCALE ? 1 : 1024;
    FT_Pos*   advance = flags & FT_ਲੱਦੋ_VERTICAL_LAYOUT
                          ? &face->glyph->advance.y
                          : &face->glyph->advance.x;


    flags |= (FT_UInt32)FT_ਲੱਦੋ_ADVANCE_ONLY;

    ਲਈ ( nn = 0; nn < ਗਿਣਤੀ_2; nn++ )
    {
      error = FT_ਲੱਦੋ_Glyph( face, gindex + nn, flags );
      ਜੇ ( error )
        ਰੋਕੋ;

      /* scale from 26.6 to 16.16, unless NO_SCALE was requested */
      padvances[nn] = *advance * ਗੁਣਕ;
    }

    ਵਾਪਸ error;
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

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਲੱਦੋ_ADVANCE_FAST_CHECK( face, flags )                      \
          ( flags & ( FT_ਲੱਦੋ_NO_SCALE | FT_ਲੱਦੋ_NO_HINTING )    || \
            FT_ਲੱਦੋ_ਨਿਸ਼ਾਨਾ_ਬਹੁਲਕ( flags ) == FT_RENDER_ਬਹੁਲਕ_LIGHT )


  /* documentation is in ftadvanc.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Advance( FT_Face    face,
                  FT_UInt    gindex,
                  FT_Int32   flags,
                  FT_Fixed  *padvance )
  {
    FT_Face_GetAdvancesFunc  func;


    ਜੇ ( !face )
      ਵਾਪਸ FT_THROW( Invalid_Face_Handle );

    ਜੇ ( !padvance )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    ਜੇ ( gindex >= (FT_UInt)face->ਅੰਕ_glyphs )
      ਵਾਪਸ FT_THROW( Invalid_Glyph_ਸੂਚਕਾਂਕ );

    func = face->driver->clazz->get_advances;
    ਜੇ ( func && ਲੱਦੋ_ADVANCE_FAST_CHECK( face, flags ) )
    {
      FT_Error  error;


      error = func( face, gindex, 1, flags, padvance );
      ਜੇ ( !error )
        ਵਾਪਸ ft_face_scale_advances_( face, padvance, 1, flags );

      ਜੇ ( FT_ERR_NEQ( error, Unimplemented_Feature ) )
        ਵਾਪਸ error;
    }

    ਜੇ ( flags & FT_ADVANCE_FLAG_FAST_ONLY )
      ਵਾਪਸ FT_THROW( Unimplemented_Feature );

    ਵਾਪਸ ft_ਲੱਦੋ_advances( face, gindex, 1, flags, padvance );
  }


  /* documentation is in ftadvanc.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Advances( FT_Face    face,
                   FT_UInt    ਸ਼ੁਰੂ,
                   FT_UInt    ਗਿਣਤੀ_2,
                   FT_Int32   flags,
                   FT_Fixed  *padvances )
  {
    FT_Face_GetAdvancesFunc  func;

    FT_UInt  ਅੰਕ, ਸਿਰਾ;


    ਜੇ ( !face )
      ਵਾਪਸ FT_THROW( Invalid_Face_Handle );

    ਜੇ ( !padvances )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    ਅੰਕ = (FT_UInt)face->ਅੰਕ_glyphs;
    ਸਿਰਾ = ਸ਼ੁਰੂ + ਗਿਣਤੀ_2;
    ਜੇ ( ਸ਼ੁਰੂ >= ਅੰਕ || ਸਿਰਾ < ਸ਼ੁਰੂ || ਸਿਰਾ > ਅੰਕ )
      ਵਾਪਸ FT_THROW( Invalid_Glyph_ਸੂਚਕਾਂਕ );

    ਜੇ ( ਗਿਣਤੀ_2 == 0 )
      ਵਾਪਸ FT_Err_Ok;

    func = face->driver->clazz->get_advances;
    ਜੇ ( func && ਲੱਦੋ_ADVANCE_FAST_CHECK( face, flags ) )
    {
      FT_Error  error;


      error = func( face, ਸ਼ੁਰੂ, ਗਿਣਤੀ_2, flags, padvances );
      ਜੇ ( !error )
        ਵਾਪਸ ft_face_scale_advances_( face, padvances, ਗਿਣਤੀ_2, flags );

      ਜੇ ( FT_ERR_NEQ( error, Unimplemented_Feature ) )
        ਵਾਪਸ error;
    }

    ਜੇ ( flags & FT_ADVANCE_FLAG_FAST_ONLY )
      ਵਾਪਸ FT_THROW( Unimplemented_Feature );

    ਵਾਪਸ ft_ਲੱਦੋ_advances( face, ਸ਼ੁਰੂ, ਗਿਣਤੀ_2, flags, padvances );
  }


/*ਸਿਰਾ*/
