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
                           FT_UInt    شمار,
                           FT_Int32   flags )
  {
    FT_Fixed  scale;
    FT_UInt   nn;


    إذا ( flags & FT_بارگیری_NO_SCALE )
      أرجع FT_Err_Ok;

    إذا ( !face->اندازه )
      أرجع FT_THROW( Invalid_اندازه_Handle );

    إذا ( flags & FT_بارگیری_VERTICAL_LAYOUT )
      scale = face->اندازه->metrics.y_scale;
    وإلا
      scale = face->اندازه->metrics.x_scale;

    /* this must be the same scaling as to get linear{Hori,Vert}Advance */
    /* (see `FT_Load_Glyph' implementation in src/base/ftobjs.c)        */

    لكل ( nn = 0; nn < شمار; nn++ )
      advances[nn] = FT_MulFix( 1024 * advances[nn], scale );

    أرجع FT_Err_Ok;
  }

  /* loading (and hinting) to calculate the advances is slow  */
  /* unless TrueType hdmx table is provided as an accelerator */
  ساكن FT_Error
  ft_بارگیری_advances( FT_Face   face,
                    FT_UInt   gindex,
                    FT_UInt   شمار,
                    FT_Int32  flags,
                    FT_Fixed  *padvances )
  {
    FT_UInt   nn;
    FT_Error  error   = FT_Err_Ok;
    FT_Pos    ضریب  = flags & FT_بارگیری_NO_SCALE ? 1 : 1024;
    FT_Pos*   advance = flags & FT_بارگیری_VERTICAL_LAYOUT
                          ? &face->glyph->advance.y
                          : &face->glyph->advance.x;


    flags |= (FT_UInt32)FT_بارگیری_ADVANCE_ONLY;

    لكل ( nn = 0; nn < شمار; nn++ )
    {
      error = FT_بارگیری_Glyph( face, gindex + nn, flags );
      إذا ( error )
        اكسر;

      /* scale from 26.6 to 16.16, unless NO_SCALE was requested */
      padvances[nn] = *advance * ضریب;
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

#تعریف_کن بارگیری_ADVANCE_FAST_CHECK( face, flags )                      \
          ( flags & ( FT_بارگیری_NO_SCALE | FT_بارگیری_NO_HINTING )    || \
            FT_بارگیری_هدف_نما( flags ) == FT_RENDER_نما_LIGHT )


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
      أرجع FT_THROW( Invalid_آرگومان );

    إذا ( gindex >= (FT_UInt)face->رقم_glyphs )
      أرجع FT_THROW( Invalid_Glyph_شاخص );

    func = face->driver->clazz->get_advances;
    إذا ( func && بارگیری_ADVANCE_FAST_CHECK( face, flags ) )
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

    أرجع ft_بارگیری_advances( face, gindex, 1, flags, padvance );
  }


  /* documentation is in ftadvanc.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Advances( FT_Face    face,
                   FT_UInt    شروع,
                   FT_UInt    شمار,
                   FT_Int32   flags,
                   FT_Fixed  *padvances )
  {
    FT_Face_GetAdvancesFunc  func;

    FT_UInt  رقم, انتها;


    إذا ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );

    إذا ( !padvances )
      أرجع FT_THROW( Invalid_آرگومان );

    رقم = (FT_UInt)face->رقم_glyphs;
    انتها = شروع + شمار;
    إذا ( شروع >= رقم || انتها < شروع || انتها > رقم )
      أرجع FT_THROW( Invalid_Glyph_شاخص );

    إذا ( شمار == 0 )
      أرجع FT_Err_Ok;

    func = face->driver->clazz->get_advances;
    إذا ( func && بارگیری_ADVANCE_FAST_CHECK( face, flags ) )
    {
      FT_Error  error;


      error = func( face, شروع, شمار, flags, padvances );
      إذا ( !error )
        أرجع ft_face_scale_advances_( face, padvances, شمار, flags );

      إذا ( FT_ERR_NEQ( error, Unimplemented_Feature ) )
        أرجع error;
    }

    إذا ( flags & FT_ADVANCE_FLAG_FAST_ONLY )
      أرجع FT_THROW( Unimplemented_Feature );

    أرجع ft_بارگیری_advances( face, شروع, شمار, flags, padvances );
  }


/*انتها*/
