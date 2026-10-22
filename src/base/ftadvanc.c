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
                           FT_UInt    count,
                           FT_Int32   flags )
  {
    FT_Fixed  scale;
    FT_UInt   nn;


    𐤀𐤌 ( flags & FT_LOAD_NO_SCALE )
      أرجع FT_Err_Ok;

    𐤀𐤌 ( !face->size )
      أرجع FT_THROW( Invalid_Size_Handle );

    𐤀𐤌 ( flags & FT_LOAD_VERTICAL_LAYOUT )
      scale = face->size->metrics.y_scale;
    وإلا
      scale = face->size->metrics.x_scale;

    /* this must be the same scaling as to get linear{Hori,Vert}Advance */
    /* (see `FT_Load_Glyph' implementation in src/base/ftobjs.c)        */

    لكل ( nn = 0; nn < count; nn++ )
      advances[nn] = FT_MulFix( 1024 * advances[nn], scale );

    أرجع FT_Err_Ok;
  }

  /* loading (and hinting) to calculate the advances is slow  */
  /* unless TrueType hdmx table is provided as an accelerator */
  ساكن FT_Error
  ft_load_advances( FT_Face   face,
                    FT_UInt   gindex,
                    FT_UInt   count,
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

    لكل ( nn = 0; nn < count; nn++ )
    {
      error = FT_Load_Glyph( face, gindex + nn, flags );
      𐤀𐤌 ( error )
        اكسر;

      /* scale from 26.6 to 16.16, unless NO_SCALE was requested */
      padvances[nn] = *advance * factor;
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


    𐤀𐤌 ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );

    𐤀𐤌 ( !padvance )
      أرجع FT_THROW( Invalid_Argument );

    𐤀𐤌 ( gindex >= (FT_UInt)face->num_glyphs )
      أرجع FT_THROW( Invalid_Glyph_Index );

    func = face->driver->clazz->get_advances;
    𐤀𐤌 ( func && LOAD_ADVANCE_FAST_CHECK( face, flags ) )
    {
      FT_Error  error;


      error = func( face, gindex, 1, flags, padvance );
      𐤀𐤌 ( !error )
        أرجع ft_face_scale_advances_( face, padvance, 1, flags );

      𐤀𐤌 ( FT_ERR_NEQ( error, Unimplemented_Feature ) )
        أرجع error;
    }

    𐤀𐤌 ( flags & FT_ADVANCE_FLAG_FAST_ONLY )
      أرجع FT_THROW( Unimplemented_Feature );

    أرجع ft_load_advances( face, gindex, 1, flags, padvance );
  }


  /* documentation is in ftadvanc.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Advances( FT_Face    face,
                   FT_UInt    start,
                   FT_UInt    count,
                   FT_Int32   flags,
                   FT_Fixed  *padvances )
  {
    FT_Face_GetAdvancesFunc  func;

    FT_UInt  num, end;


    𐤀𐤌 ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );

    𐤀𐤌 ( !padvances )
      أرجع FT_THROW( Invalid_Argument );

    num = (FT_UInt)face->num_glyphs;
    end = start + count;
    𐤀𐤌 ( start >= num || end < start || end > num )
      أرجع FT_THROW( Invalid_Glyph_Index );

    𐤀𐤌 ( count == 0 )
      أرجع FT_Err_Ok;

    func = face->driver->clazz->get_advances;
    𐤀𐤌 ( func && LOAD_ADVANCE_FAST_CHECK( face, flags ) )
    {
      FT_Error  error;


      error = func( face, start, count, flags, padvances );
      𐤀𐤌 ( !error )
        أرجع ft_face_scale_advances_( face, padvances, count, flags );

      𐤀𐤌 ( FT_ERR_NEQ( error, Unimplemented_Feature ) )
        أرجع error;
    }

    𐤀𐤌 ( flags & FT_ADVANCE_FLAG_FAST_ONLY )
      أرجع FT_THROW( Unimplemented_Feature );

    أرجع ft_load_advances( face, start, count, flags, padvances );
  }


/* END */
