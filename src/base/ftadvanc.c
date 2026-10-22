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
                           FT_UInt    मोजणी,
                           FT_Int32   flags )
  {
    FT_Fixed  scale;
    FT_UInt   nn;


    जर ( flags & FT_LOAD_NO_SCALE )
      परत FT_Err_Ok;

    जर ( !face->size )
      परत FT_THROW( Invalid_Size_Handle );

    जर ( flags & FT_LOAD_VERTICAL_LAYOUT )
      scale = face->size->metrics.y_scale;
    अन्यथा
      scale = face->size->metrics.x_scale;

    /* this must be the same scaling as to get linear{Hori,Vert}Advance */
    /* (see `FT_Load_Glyph' implementation in src/base/ftobjs.c)        */

    साठी ( nn = 0; nn < मोजणी; nn++ )
      advances[nn] = FT_MulFix( 1024 * advances[nn], scale );

    परत FT_Err_Ok;
  }

  /* loading (and hinting) to calculate the advances is slow  */
  /* unless TrueType hdmx table is provided as an accelerator */
  स्थिर FT_Error
  ft_load_advances( FT_Face   face,
                    FT_UInt   gindex,
                    FT_UInt   मोजणी,
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

    साठी ( nn = 0; nn < मोजणी; nn++ )
    {
      error = FT_Load_Glyph( face, gindex + nn, flags );
      जर ( error )
        थांबा;

      /* scale from 26.6 to 16.16, unless NO_SCALE was requested */
      padvances[nn] = *advance * factor;
    }

    परत error;
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


    जर ( !face )
      परत FT_THROW( Invalid_Face_Handle );

    जर ( !padvance )
      परत FT_THROW( Invalid_Argument );

    जर ( gindex >= (FT_UInt)face->अंक_glyphs )
      परत FT_THROW( Invalid_Glyph_Index );

    func = face->driver->clazz->get_advances;
    जर ( func && LOAD_ADVANCE_FAST_CHECK( face, flags ) )
    {
      FT_Error  error;


      error = func( face, gindex, 1, flags, padvance );
      जर ( !error )
        परत ft_face_scale_advances_( face, padvance, 1, flags );

      जर ( FT_ERR_NEQ( error, Unimplemented_Feature ) )
        परत error;
    }

    जर ( flags & FT_ADVANCE_FLAG_FAST_ONLY )
      परत FT_THROW( Unimplemented_Feature );

    परत ft_load_advances( face, gindex, 1, flags, padvance );
  }


  /* documentation is in ftadvanc.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Advances( FT_Face    face,
                   FT_UInt    start,
                   FT_UInt    मोजणी,
                   FT_Int32   flags,
                   FT_Fixed  *padvances )
  {
    FT_Face_GetAdvancesFunc  func;

    FT_UInt  अंक, end;


    जर ( !face )
      परत FT_THROW( Invalid_Face_Handle );

    जर ( !padvances )
      परत FT_THROW( Invalid_Argument );

    अंक = (FT_UInt)face->अंक_glyphs;
    end = start + मोजणी;
    जर ( start >= अंक || end < start || end > अंक )
      परत FT_THROW( Invalid_Glyph_Index );

    जर ( मोजणी == 0 )
      परत FT_Err_Ok;

    func = face->driver->clazz->get_advances;
    जर ( func && LOAD_ADVANCE_FAST_CHECK( face, flags ) )
    {
      FT_Error  error;


      error = func( face, start, मोजणी, flags, padvances );
      जर ( !error )
        परत ft_face_scale_advances_( face, padvances, मोजणी, flags );

      जर ( FT_ERR_NEQ( error, Unimplemented_Feature ) )
        परत error;
    }

    जर ( flags & FT_ADVANCE_FLAG_FAST_ONLY )
      परत FT_THROW( Unimplemented_Feature );

    परत ft_load_advances( face, start, मोजणी, flags, padvances );
  }


/* END */
