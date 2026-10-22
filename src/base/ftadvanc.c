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


#ضمن <freetype/internal/ftdebug.h>

#ضمن <freetype/ftadvanc.h>
#ضمن <freetype/internal/ftobjs.h>
#ضمن <freetype/internal/ftcalc.h>


  ساكن FT_Error
  ft_face_scale_advances_( FT_Face    face,
                           FT_Fixed*  advances,
                           FT_UInt    ڳڻپ,
                           FT_Int32   flags )
  {
    FT_Fixed  scale;
    FT_UInt   nn;


    إذا ( flags & FT_ڀريو_NO_SCALE )
      أرجع FT_Err_Ok;

    إذا ( !face->سائيز )
      أرجع FT_THROW( Invalid_سائيز_Handle );

    إذا ( flags & FT_ڀريو_VERTICAL_LAYOUT )
      scale = face->سائيز->metrics.y_scale;
    وإلا
      scale = face->سائيز->metrics.x_scale;

    /* this must be the same scaling as to get linear{Hori,Vert}Advance */
    /* (see `FT_Load_Glyph' implementation in src/base/ftobjs.c)        */

    لكل ( nn = 0; nn < ڳڻپ; nn++ )
      advances[nn] = FT_MulFix( 1024 * advances[nn], scale );

    أرجع FT_Err_Ok;
  }

  /* loading (and hinting) to calculate the advances is slow  */
  /* unless TrueType hdmx table is provided as an accelerator */
  ساكن FT_Error
  ft_ڀريو_advances( FT_Face   face,
                    FT_UInt   gindex,
                    FT_UInt   ڳڻپ,
                    FT_Int32  flags,
                    FT_Fixed  *padvances )
  {
    FT_UInt   nn;
    FT_Error  error   = FT_Err_Ok;
    FT_Pos    ضرب_ڪندڙ  = flags & FT_ڀريو_NO_SCALE ? 1 : 1024;
    FT_Pos*   advance = flags & FT_ڀريو_VERTICAL_LAYOUT
                          ? &face->glyph->advance.y
                          : &face->glyph->advance.x;


    flags |= (FT_UInt32)FT_ڀريو_ADVANCE_ONLY;

    لكل ( nn = 0; nn < ڳڻپ; nn++ )
    {
      error = FT_ڀريو_Glyph( face, gindex + nn, flags );
      إذا ( error )
        اكسر;

      /* scale from 26.6 to 16.16, unless NO_SCALE was requested */
      padvances[nn] = *advance * ضرب_ڪندڙ;
    }

    أرجع error;
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

#وصف_ڪريو ڀريو_ADVANCE_FAST_CHECK( face, flags )                      \
          ( flags & ( FT_ڀريو_NO_SCALE | FT_ڀريو_NO_HINTING )    || \
            FT_ڀريو_هدف_گهڻائي( flags ) == FT_RENDER_گهڻائي_LIGHT )


  /* documentation is in ftadvanc.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Advance( FT_Face    face,
                  FT_UInt    gindex,
                  FT_Int32   flags,
                  FT_Fixed  *padvance )
  {
    FT_Face_GetAdvancesFunc  func;


    إذا ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );

    إذا ( !padvance )
      أرجع FT_THROW( Invalid_دليل );

    إذا ( gindex >= (FT_UInt)face->انگ_glyphs )
      أرجع FT_THROW( Invalid_Glyph_اشاريو );

    func = face->driver->clazz->get_advances;
    إذا ( func && ڀريو_ADVANCE_FAST_CHECK( face, flags ) )
    {
      FT_Error  error;


      error = func( face, gindex, 1, flags, padvance );
      إذا ( !error )
        أرجع ft_face_scale_advances_( face, padvance, 1, flags );

      إذا ( FT_ERR_NEQ( error, Unimplemented_Feature ) )
        أرجع error;
    }

    إذا ( flags & FT_ADVANCE_FLAG_FAST_ONLY )
      أرجع FT_THROW( Unimplemented_Feature );

    أرجع ft_ڀريو_advances( face, gindex, 1, flags, padvance );
  }


  /* documentation is in ftadvanc.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Advances( FT_Face    face,
                   FT_UInt    شروعات,
                   FT_UInt    ڳڻپ,
                   FT_Int32   flags,
                   FT_Fixed  *padvances )
  {
    FT_Face_GetAdvancesFunc  func;

    FT_UInt  انگ, پڇاڙي;


    إذا ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );

    إذا ( !padvances )
      أرجع FT_THROW( Invalid_دليل );

    انگ = (FT_UInt)face->انگ_glyphs;
    پڇاڙي = شروعات + ڳڻپ;
    إذا ( شروعات >= انگ || پڇاڙي < شروعات || پڇاڙي > انگ )
      أرجع FT_THROW( Invalid_Glyph_اشاريو );

    إذا ( ڳڻپ == 0 )
      أرجع FT_Err_Ok;

    func = face->driver->clazz->get_advances;
    إذا ( func && ڀريو_ADVANCE_FAST_CHECK( face, flags ) )
    {
      FT_Error  error;


      error = func( face, شروعات, ڳڻپ, flags, padvances );
      إذا ( !error )
        أرجع ft_face_scale_advances_( face, padvances, ڳڻپ, flags );

      إذا ( FT_ERR_NEQ( error, Unimplemented_Feature ) )
        أرجع error;
    }

    إذا ( flags & FT_ADVANCE_FLAG_FAST_ONLY )
      أرجع FT_THROW( Unimplemented_Feature );

    أرجع ft_ڀريو_advances( face, شروعات, ڳڻپ, flags, padvances );
  }


/*پڇاڙي*/
