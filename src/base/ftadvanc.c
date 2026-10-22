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


#ଅନ୍ତର୍ଭାଵ <freetype/internal/ftdebug.h>

#ଅନ୍ତର୍ଭାଵ <freetype/ftadvanc.h>
#ଅନ୍ତର୍ଭାଵ <freetype/internal/ftobjs.h>
#ଅନ୍ତର୍ଭାଵ <freetype/internal/ftcalc.h>


  ସ୍ଥିର FT_Error
  ft_face_scale_advances_( FT_Face    face,
                           FT_Fixed*  advances,
                           FT_UInt    ଗଣନା_2,
                           FT_Int32   flags )
  {
    FT_Fixed  scale;
    FT_UInt   nn;


    ଯଦି ( flags & FT_LOAD_NO_SCALE )
      ଫେରନ୍ତୁ FT_Err_Ok;

    ଯଦି ( !face->size )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Size_Handle );

    ଯଦି ( flags & FT_LOAD_VERTICAL_LAYOUT )
      scale = face->size->metrics.y_scale;
    ଅନ୍ୟଥା
      scale = face->size->metrics.x_scale;

    /* this must be the same scaling as to get linear{Hori,Vert}Advance */
    /* (see `FT_Load_Glyph' implementation in src/base/ftobjs.c)        */

    ପାଇଁ ( nn = 0; nn < ଗଣନା_2; nn++ )
      advances[nn] = FT_MulFix( 1024 * advances[nn], scale );

    ଫେରନ୍ତୁ FT_Err_Ok;
  }

  /* loading (and hinting) to calculate the advances is slow  */
  /* unless TrueType hdmx table is provided as an accelerator */
  ସ୍ଥିର FT_Error
  ft_load_advances( FT_Face   face,
                    FT_UInt   gindex,
                    FT_UInt   ଗଣନା_2,
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

    ପାଇଁ ( nn = 0; nn < ଗଣନା_2; nn++ )
    {
      error = FT_Load_Glyph( face, gindex + nn, flags );
      ଯଦି ( error )
        ଭାଙ୍ଗ;

      /* scale from 26.6 to 16.16, unless NO_SCALE was requested */
      padvances[nn] = *advance * factor;
    }

    ଫେରନ୍ତୁ error;
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


    ଯଦି ( !face )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Face_Handle );

    ଯଦି ( !padvance )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Argument );

    ଯଦି ( gindex >= (FT_UInt)face->ଅଙ୍କ_glyphs )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Glyph_Index );

    func = face->driver->clazz->get_advances;
    ଯଦି ( func && LOAD_ADVANCE_FAST_CHECK( face, flags ) )
    {
      FT_Error  error;


      error = func( face, gindex, 1, flags, padvance );
      ଯଦି ( !error )
        ଫେରନ୍ତୁ ft_face_scale_advances_( face, padvance, 1, flags );

      ଯଦି ( FT_ERR_NEQ( error, Unimplemented_Feature ) )
        ଫେରନ୍ତୁ error;
    }

    ଯଦି ( flags & FT_ADVANCE_FLAG_FAST_ONLY )
      ଫେରନ୍ତୁ FT_THROW( Unimplemented_Feature );

    ଫେରନ୍ତୁ ft_load_advances( face, gindex, 1, flags, padvance );
  }


  /* documentation is in ftadvanc.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Advances( FT_Face    face,
                   FT_UInt    start,
                   FT_UInt    ଗଣନା_2,
                   FT_Int32   flags,
                   FT_Fixed  *padvances )
  {
    FT_Face_GetAdvancesFunc  func;

    FT_UInt  ଅଙ୍କ, end;


    ଯଦି ( !face )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Face_Handle );

    ଯଦି ( !padvances )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Argument );

    ଅଙ୍କ = (FT_UInt)face->ଅଙ୍କ_glyphs;
    end = start + ଗଣନା_2;
    ଯଦି ( start >= ଅଙ୍କ || end < start || end > ଅଙ୍କ )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Glyph_Index );

    ଯଦି ( ଗଣନା_2 == 0 )
      ଫେରନ୍ତୁ FT_Err_Ok;

    func = face->driver->clazz->get_advances;
    ଯଦି ( func && LOAD_ADVANCE_FAST_CHECK( face, flags ) )
    {
      FT_Error  error;


      error = func( face, start, ଗଣନା_2, flags, padvances );
      ଯଦି ( !error )
        ଫେରନ୍ତୁ ft_face_scale_advances_( face, padvances, ଗଣନା_2, flags );

      ଯଦି ( FT_ERR_NEQ( error, Unimplemented_Feature ) )
        ଫେରନ୍ତୁ error;
    }

    ଯଦି ( flags & FT_ADVANCE_FLAG_FAST_ONLY )
      ଫେରନ୍ତୁ FT_THROW( Unimplemented_Feature );

    ଫେରନ୍ତୁ ft_load_advances( face, start, ଗଣନା_2, flags, padvances );
  }


/* END */
