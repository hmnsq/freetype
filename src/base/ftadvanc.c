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


#అన్తర్భావ <freetype/internal/ftdebug.h>

#అన్తర్భావ <freetype/ftadvanc.h>
#అన్తర్భావ <freetype/internal/ftobjs.h>
#అన్తర్భావ <freetype/internal/ftcalc.h>


  స్థిర FT_Error
  ft_face_scale_advances_( FT_Face    face,
                           FT_Fixed*  advances,
                           FT_UInt    లెక్క,
                           FT_Int32   flags )
  {
    FT_Fixed  scale;
    FT_UInt   nn;


    ఒకవేళ ( flags & FT_ఎక్కించు_NO_SCALE )
      తిరిగి FT_Err_Ok;

    ఒకవేళ ( !face->కొలత )
      తిరిగి FT_THROW( Invalid_కొలత_Handle );

    ఒకవేళ ( flags & FT_ఎక్కించు_VERTICAL_LAYOUT )
      scale = face->కొలత->metrics.y_scale;
    లేకపోతే
      scale = face->కొలత->metrics.x_scale;

    /* this must be the same scaling as to get linear{Hori,Vert}Advance */
    /* (see `FT_Load_Glyph' implementation in src/base/ftobjs.c)        */

    కొరకు ( nn = 0; nn < లెక్క; nn++ )
      advances[nn] = FT_MulFix( 1024 * advances[nn], scale );

    తిరిగి FT_Err_Ok;
  }

  /* loading (and hinting) to calculate the advances is slow  */
  /* unless TrueType hdmx table is provided as an accelerator */
  స్థిర FT_Error
  ft_ఎక్కించు_advances( FT_Face   face,
                    FT_UInt   gindex,
                    FT_UInt   లెక్క,
                    FT_Int32  flags,
                    FT_Fixed  *padvances )
  {
    FT_UInt   nn;
    FT_Error  error   = FT_Err_Ok;
    FT_Pos    గుణకం  = flags & FT_ఎక్కించు_NO_SCALE ? 1 : 1024;
    FT_Pos*   advance = flags & FT_ఎక్కించు_VERTICAL_LAYOUT
                          ? &face->glyph->advance.y
                          : &face->glyph->advance.x;


    flags |= (FT_UInt32)FT_ఎక్కించు_ADVANCE_ONLY;

    కొరకు ( nn = 0; nn < లెక్క; nn++ )
    {
      error = FT_ఎక్కించు_Glyph( face, gindex + nn, flags );
      ఒకవేళ ( error )
        ఆపు;

      /* scale from 26.6 to 16.16, unless NO_SCALE was requested */
      padvances[nn] = *advance * గుణకం;
    }

    తిరిగి error;
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

#నిర్వచించు ఎక్కించు_ADVANCE_FAST_CHECK( face, flags )                      \
          ( flags & ( FT_ఎక్కించు_NO_SCALE | FT_ఎక్కించు_NO_HINTING )    || \
            FT_ఎక్కించు_లక్ష్యం_బహుళకం( flags ) == FT_RENDER_బహుళకం_LIGHT )


  /* documentation is in ftadvanc.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Advance( FT_Face    face,
                  FT_UInt    gindex,
                  FT_Int32   flags,
                  FT_Fixed  *padvance )
  {
    FT_Face_GetAdvancesFunc  func;


    ఒకవేళ ( !face )
      తిరిగి FT_THROW( Invalid_Face_Handle );

    ఒకవేళ ( !padvance )
      తిరిగి FT_THROW( Invalid_వాదం );

    ఒకవేళ ( gindex >= (FT_UInt)face->అంకె_glyphs )
      తిరిగి FT_THROW( Invalid_Glyph_సూచీ );

    func = face->driver->clazz->get_advances;
    ఒకవేళ ( func && ఎక్కించు_ADVANCE_FAST_CHECK( face, flags ) )
    {
      FT_Error  error;


      error = func( face, gindex, 1, flags, padvance );
      ఒకవేళ ( !error )
        తిరిగి ft_face_scale_advances_( face, padvance, 1, flags );

      ఒకవేళ ( FT_ERR_NEQ( error, Unimplemented_Feature ) )
        తిరిగి error;
    }

    ఒకవేళ ( flags & FT_ADVANCE_FLAG_FAST_ONLY )
      తిరిగి FT_THROW( Unimplemented_Feature );

    తిరిగి ft_ఎక్కించు_advances( face, gindex, 1, flags, padvance );
  }


  /* documentation is in ftadvanc.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Advances( FT_Face    face,
                   FT_UInt    ప్రారంభం,
                   FT_UInt    లెక్క,
                   FT_Int32   flags,
                   FT_Fixed  *padvances )
  {
    FT_Face_GetAdvancesFunc  func;

    FT_UInt  అంకె, చివర;


    ఒకవేళ ( !face )
      తిరిగి FT_THROW( Invalid_Face_Handle );

    ఒకవేళ ( !padvances )
      తిరిగి FT_THROW( Invalid_వాదం );

    అంకె = (FT_UInt)face->అంకె_glyphs;
    చివర = ప్రారంభం + లెక్క;
    ఒకవేళ ( ప్రారంభం >= అంకె || చివర < ప్రారంభం || చివర > అంకె )
      తిరిగి FT_THROW( Invalid_Glyph_సూచీ );

    ఒకవేళ ( లెక్క == 0 )
      తిరిగి FT_Err_Ok;

    func = face->driver->clazz->get_advances;
    ఒకవేళ ( func && ఎక్కించు_ADVANCE_FAST_CHECK( face, flags ) )
    {
      FT_Error  error;


      error = func( face, ప్రారంభం, లెక్క, flags, padvances );
      ఒకవేళ ( !error )
        తిరిగి ft_face_scale_advances_( face, padvances, లెక్క, flags );

      ఒకవేళ ( FT_ERR_NEQ( error, Unimplemented_Feature ) )
        తిరిగి error;
    }

    ఒకవేళ ( flags & FT_ADVANCE_FLAG_FAST_ONLY )
      తిరిగి FT_THROW( Unimplemented_Feature );

    తిరిగి ft_ఎక్కించు_advances( face, ప్రారంభం, లెక్క, flags, padvances );
  }


/*చివర*/
