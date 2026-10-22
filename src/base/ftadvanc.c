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


#அந்தர்பாவ <freetype/internal/ftdebug.h>

#அந்தர்பாவ <freetype/ftadvanc.h>
#அந்தர்பாவ <freetype/internal/ftobjs.h>
#அந்தர்பாவ <freetype/internal/ftcalc.h>


  நிலையான FT_Error
  ft_face_scale_advances_( FT_Face    face,
                           FT_Fixed*  advances,
                           FT_UInt    மொத்தம்,
                           FT_Int32   flags )
  {
    FT_Fixed  scale;
    FT_UInt   nn;


    எனில் ( flags & FT_LOAD_NO_SCALE )
      திரும்பு FT_Err_Ok;

    எனில் ( !face->size )
      திரும்பு FT_THROW( Invalid_Size_Handle );

    எனில் ( flags & FT_LOAD_VERTICAL_LAYOUT )
      scale = face->size->metrics.y_scale;
    இல்லையெனில்
      scale = face->size->metrics.x_scale;

    /* this must be the same scaling as to get linear{Hori,Vert}Advance */
    /* (see `FT_Load_Glyph' implementation in src/base/ftobjs.c)        */

    க்கு ( nn = 0; nn < மொத்தம்; nn++ )
      advances[nn] = FT_MulFix( 1024 * advances[nn], scale );

    திரும்பு FT_Err_Ok;
  }

  /* loading (and hinting) to calculate the advances is slow  */
  /* unless TrueType hdmx table is provided as an accelerator */
  நிலையான FT_Error
  ft_load_advances( FT_Face   face,
                    FT_UInt   gindex,
                    FT_UInt   மொத்தம்,
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

    க்கு ( nn = 0; nn < மொத்தம்; nn++ )
    {
      error = FT_Load_Glyph( face, gindex + nn, flags );
      எனில் ( error )
        நிறுத்து;

      /* scale from 26.6 to 16.16, unless NO_SCALE was requested */
      padvances[nn] = *advance * factor;
    }

    திரும்பு error;
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


    எனில் ( !face )
      திரும்பு FT_THROW( Invalid_Face_Handle );

    எனில் ( !padvance )
      திரும்பு FT_THROW( Invalid_Argument );

    எனில் ( gindex >= (FT_UInt)face->இலக்கம்_glyphs )
      திரும்பு FT_THROW( Invalid_Glyph_Index );

    func = face->driver->clazz->get_advances;
    எனில் ( func && LOAD_ADVANCE_FAST_CHECK( face, flags ) )
    {
      FT_Error  error;


      error = func( face, gindex, 1, flags, padvance );
      எனில் ( !error )
        திரும்பு ft_face_scale_advances_( face, padvance, 1, flags );

      எனில் ( FT_ERR_NEQ( error, Unimplemented_Feature ) )
        திரும்பு error;
    }

    எனில் ( flags & FT_ADVANCE_FLAG_FAST_ONLY )
      திரும்பு FT_THROW( Unimplemented_Feature );

    திரும்பு ft_load_advances( face, gindex, 1, flags, padvance );
  }


  /* documentation is in ftadvanc.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Advances( FT_Face    face,
                   FT_UInt    start,
                   FT_UInt    மொத்தம்,
                   FT_Int32   flags,
                   FT_Fixed  *padvances )
  {
    FT_Face_GetAdvancesFunc  func;

    FT_UInt  இலக்கம், end;


    எனில் ( !face )
      திரும்பு FT_THROW( Invalid_Face_Handle );

    எனில் ( !padvances )
      திரும்பு FT_THROW( Invalid_Argument );

    இலக்கம் = (FT_UInt)face->இலக்கம்_glyphs;
    end = start + மொத்தம்;
    எனில் ( start >= இலக்கம் || end < start || end > இலக்கம் )
      திரும்பு FT_THROW( Invalid_Glyph_Index );

    எனில் ( மொத்தம் == 0 )
      திரும்பு FT_Err_Ok;

    func = face->driver->clazz->get_advances;
    எனில் ( func && LOAD_ADVANCE_FAST_CHECK( face, flags ) )
    {
      FT_Error  error;


      error = func( face, start, மொத்தம், flags, padvances );
      எனில் ( !error )
        திரும்பு ft_face_scale_advances_( face, padvances, மொத்தம், flags );

      எனில் ( FT_ERR_NEQ( error, Unimplemented_Feature ) )
        திரும்பு error;
    }

    எனில் ( flags & FT_ADVANCE_FLAG_FAST_ONLY )
      திரும்பு FT_THROW( Unimplemented_Feature );

    திரும்பு ft_load_advances( face, start, மொத்தம், flags, padvances );
  }


/* END */
