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


#অন্তর্ভাব <freetype/internal/ftdebug.h>

#অন্তর্ভাব <freetype/ftadvanc.h>
#অন্তর্ভাব <freetype/internal/ftobjs.h>
#অন্তর্ভাব <freetype/internal/ftcalc.h>


  স্থির FT_Error
  ft_face_scale_advances_( FT_Face    face,
                           FT_Fixed*  advances,
                           FT_UInt    গুনতি,
                           FT_Int32   flags )
  {
    FT_Fixed  scale;
    FT_UInt   nn;


    যদি ( flags & FT_LOAD_NO_SCALE )
      প্রত্যাগম FT_Err_Ok;

    যদি ( !face->size )
      প্রত্যাগম FT_THROW( Invalid_Size_Handle );

    যদি ( flags & FT_LOAD_VERTICAL_LAYOUT )
      scale = face->size->metrics.y_scale;
    অন্যথা
      scale = face->size->metrics.x_scale;

    /* this must be the same scaling as to get linear{Hori,Vert}Advance */
    /* (see `FT_Load_Glyph' implementation in src/base/ftobjs.c)        */

    প্রতি ( nn = 0; nn < গুনতি; nn++ )
      advances[nn] = FT_MulFix( 1024 * advances[nn], scale );

    প্রত্যাগম FT_Err_Ok;
  }

  /* loading (and hinting) to calculate the advances is slow  */
  /* unless TrueType hdmx table is provided as an accelerator */
  স্থির FT_Error
  ft_load_advances( FT_Face   face,
                    FT_UInt   gindex,
                    FT_UInt   গুনতি,
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

    প্রতি ( nn = 0; nn < গুনতি; nn++ )
    {
      error = FT_Load_Glyph( face, gindex + nn, flags );
      যদি ( error )
        ভঙ্গ;

      /* scale from 26.6 to 16.16, unless NO_SCALE was requested */
      padvances[nn] = *advance * factor;
    }

    প্রত্যাগম error;
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


    যদি ( !face )
      প্রত্যাগম FT_THROW( Invalid_Face_Handle );

    যদি ( !padvance )
      প্রত্যাগম FT_THROW( Invalid_Argument );

    যদি ( gindex >= (FT_UInt)face->অঙ্ক_glyphs )
      প্রত্যাগম FT_THROW( Invalid_Glyph_Index );

    func = face->driver->clazz->get_advances;
    যদি ( func && LOAD_ADVANCE_FAST_CHECK( face, flags ) )
    {
      FT_Error  error;


      error = func( face, gindex, 1, flags, padvance );
      যদি ( !error )
        প্রত্যাগম ft_face_scale_advances_( face, padvance, 1, flags );

      যদি ( FT_ERR_NEQ( error, Unimplemented_Feature ) )
        প্রত্যাগম error;
    }

    যদি ( flags & FT_ADVANCE_FLAG_FAST_ONLY )
      প্রত্যাগম FT_THROW( Unimplemented_Feature );

    প্রত্যাগম ft_load_advances( face, gindex, 1, flags, padvance );
  }


  /* documentation is in ftadvanc.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Advances( FT_Face    face,
                   FT_UInt    start,
                   FT_UInt    গুনতি,
                   FT_Int32   flags,
                   FT_Fixed  *padvances )
  {
    FT_Face_GetAdvancesFunc  func;

    FT_UInt  অঙ্ক, end;


    যদি ( !face )
      প্রত্যাগম FT_THROW( Invalid_Face_Handle );

    যদি ( !padvances )
      প্রত্যাগম FT_THROW( Invalid_Argument );

    অঙ্ক = (FT_UInt)face->অঙ্ক_glyphs;
    end = start + গুনতি;
    যদি ( start >= অঙ্ক || end < start || end > অঙ্ক )
      প্রত্যাগম FT_THROW( Invalid_Glyph_Index );

    যদি ( গুনতি == 0 )
      প্রত্যাগম FT_Err_Ok;

    func = face->driver->clazz->get_advances;
    যদি ( func && LOAD_ADVANCE_FAST_CHECK( face, flags ) )
    {
      FT_Error  error;


      error = func( face, start, গুনতি, flags, padvances );
      যদি ( !error )
        প্রত্যাগম ft_face_scale_advances_( face, padvances, গুনতি, flags );

      যদি ( FT_ERR_NEQ( error, Unimplemented_Feature ) )
        প্রত্যাগম error;
    }

    যদি ( flags & FT_ADVANCE_FLAG_FAST_ONLY )
      প্রত্যাগম FT_THROW( Unimplemented_Feature );

    প্রত্যাগম ft_load_advances( face, start, গুনতি, flags, padvances );
  }


/* END */
