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


#അന്തര്ഭാവ <freetype/internal/ftdebug.h>

#അന്തര്ഭാവ <freetype/ftadvanc.h>
#അന്തര്ഭാവ <freetype/internal/ftobjs.h>
#അന്തര്ഭാവ <freetype/internal/ftcalc.h>


  സ്ഥിതം FT_Error
  ft_face_scale_advances_( FT_Face    face,
                           FT_Fixed*  advances,
                           FT_UInt    എണ്ണം,
                           FT_Int32   flags )
  {
    FT_Fixed  scale;
    FT_UInt   nn;


    എങ്കിൽ ( flags & FT_LOAD_NO_SCALE )
      തിരികെ FT_Err_Ok;

    എങ്കിൽ ( !face->size )
      തിരികെ FT_THROW( Invalid_Size_Handle );

    എങ്കിൽ ( flags & FT_LOAD_VERTICAL_LAYOUT )
      scale = face->size->metrics.y_scale;
    അല്ലെങ്കിൽ
      scale = face->size->metrics.x_scale;

    /* this must be the same scaling as to get linear{Hori,Vert}Advance */
    /* (see `FT_Load_Glyph' implementation in src/base/ftobjs.c)        */

    വേണ്ടി ( nn = 0; nn < എണ്ണം; nn++ )
      advances[nn] = FT_MulFix( 1024 * advances[nn], scale );

    തിരികെ FT_Err_Ok;
  }

  /* loading (and hinting) to calculate the advances is slow  */
  /* unless TrueType hdmx table is provided as an accelerator */
  സ്ഥിതം FT_Error
  ft_load_advances( FT_Face   face,
                    FT_UInt   gindex,
                    FT_UInt   എണ്ണം,
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

    വേണ്ടി ( nn = 0; nn < എണ്ണം; nn++ )
    {
      error = FT_Load_Glyph( face, gindex + nn, flags );
      എങ്കിൽ ( error )
        നിർത്തുക;

      /* scale from 26.6 to 16.16, unless NO_SCALE was requested */
      padvances[nn] = *advance * factor;
    }

    തിരികെ error;
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


    എങ്കിൽ ( !face )
      തിരികെ FT_THROW( Invalid_Face_Handle );

    എങ്കിൽ ( !padvance )
      തിരികെ FT_THROW( Invalid_Argument );

    എങ്കിൽ ( gindex >= (FT_UInt)face->അക്കം_glyphs )
      തിരികെ FT_THROW( Invalid_Glyph_Index );

    func = face->driver->clazz->get_advances;
    എങ്കിൽ ( func && LOAD_ADVANCE_FAST_CHECK( face, flags ) )
    {
      FT_Error  error;


      error = func( face, gindex, 1, flags, padvance );
      എങ്കിൽ ( !error )
        തിരികെ ft_face_scale_advances_( face, padvance, 1, flags );

      എങ്കിൽ ( FT_ERR_NEQ( error, Unimplemented_Feature ) )
        തിരികെ error;
    }

    എങ്കിൽ ( flags & FT_ADVANCE_FLAG_FAST_ONLY )
      തിരികെ FT_THROW( Unimplemented_Feature );

    തിരികെ ft_load_advances( face, gindex, 1, flags, padvance );
  }


  /* documentation is in ftadvanc.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Advances( FT_Face    face,
                   FT_UInt    start,
                   FT_UInt    എണ്ണം,
                   FT_Int32   flags,
                   FT_Fixed  *padvances )
  {
    FT_Face_GetAdvancesFunc  func;

    FT_UInt  അക്കം, end;


    എങ്കിൽ ( !face )
      തിരികെ FT_THROW( Invalid_Face_Handle );

    എങ്കിൽ ( !padvances )
      തിരികെ FT_THROW( Invalid_Argument );

    അക്കം = (FT_UInt)face->അക്കം_glyphs;
    end = start + എണ്ണം;
    എങ്കിൽ ( start >= അക്കം || end < start || end > അക്കം )
      തിരികെ FT_THROW( Invalid_Glyph_Index );

    എങ്കിൽ ( എണ്ണം == 0 )
      തിരികെ FT_Err_Ok;

    func = face->driver->clazz->get_advances;
    എങ്കിൽ ( func && LOAD_ADVANCE_FAST_CHECK( face, flags ) )
    {
      FT_Error  error;


      error = func( face, start, എണ്ണം, flags, padvances );
      എങ്കിൽ ( !error )
        തിരികെ ft_face_scale_advances_( face, padvances, എണ്ണം, flags );

      എങ്കിൽ ( FT_ERR_NEQ( error, Unimplemented_Feature ) )
        തിരികെ error;
    }

    എങ്കിൽ ( flags & FT_ADVANCE_FLAG_FAST_ONLY )
      തിരികെ FT_THROW( Unimplemented_Feature );

    തിരികെ ft_load_advances( face, start, എണ്ണം, flags, padvances );
  }


/* END */
