/****************************************************************************
 *
 * ftglyph.c
 *
 *   FreeType convenience functions to handle glyphs (body).
 *
 * Copyright (C) 1996-2026 by
 * David Turner, Robert Wilhelm, and Werner Lemberg.
 *
 * This file is part of the FreeType project, and may only be used,
 * modified, and distributed under the terms of the FreeType project
 * license, LICENSE.TXT.  By continuing to use, modify, or distribute
 * this file you indicate that you have read the license and
 * understand and accept it fully.
 *
 */

  /**************************************************************************
   *
   * This file contains the definition of several convenience functions
   * that can be used by client applications to easily retrieve glyph
   * bitmaps and outlines from a given face.
   *
   * These functions should be optional if you are writing a font server
   * or text layout engine on top of FreeType.  However, they are pretty
   * handy for many other simple uses of the library.
   *
   */


#ಅನ್ತರ್ಭಾವ <freetype/internal/ftdebug.h>

#ಅನ್ತರ್ಭಾವ <freetype/ftglyph.h>
#ಅನ್ತರ್ಭಾವ <freetype/ftoutln.h>
#ಅನ್ತರ್ಭಾವ <freetype/ftbitmap.h>
#ಅನ್ತರ್ಭಾವ <freetype/internal/ftobjs.h>
#ಅನ್ತರ್ಭಾವ <freetype/otsvg.h>

#ಅನ್ತರ್ಭಾವ "ftbase.h"


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು  FT_COMPONENT
#ವ್ಯಾಖ್ಯಾನಿಸು FT_COMPONENT  glyph


  /*************************************************************************/
  /*************************************************************************/
  /****                                                                 ****/
  /****   FT_BitmapGlyph support                                        ****/
  /****                                                                 ****/
  /*************************************************************************/
  /*************************************************************************/

  FT_CALLBACK_DEF( FT_Error )
  ft_bitmap_glyph_init( FT_Glyph      bitmap_glyph,
                        FT_Glyph_ಸ್ಥಾನ  ಸ್ಥಾನ )
  {
    FT_BitmapGlyph  glyph   = (FT_BitmapGlyph)bitmap_glyph;
    FT_Error        error;
    FT_Library      library = FT_GLYPH( glyph )->library;


    ಆದರೆ ( ಸ್ಥಾನ->format != FT_GLYPH_FORMAT_BITMAP )
    {
      error = FT_THROW( Invalid_Glyph_Format );
      ಹೋಗು Exit;
    }

    glyph->ಎಡ = ಸ್ಥಾನ->bitmap_ಎಡ;
    glyph->ತುದಿ  = ಸ್ಥಾನ->bitmap_ತುದಿ;

    FT_Bitmap_Init( &glyph->bitmap );
    error = FT_Bitmap_Copy( library, &ಸ್ಥಾನ->bitmap, &glyph->bitmap );

  Exit:
    ಹಿಂತಿರುಗಿ error;
  }


  FT_CALLBACK_DEF( FT_Error )
  ft_bitmap_glyph_copy( FT_Glyph  bitmap_ಮೂಲ,
                        FT_Glyph  bitmap_ಗುರಿ )
  {
    FT_Library      library = bitmap_ಮೂಲ->library;
    FT_BitmapGlyph  ಮೂಲ  = (FT_BitmapGlyph)bitmap_ಮೂಲ;
    FT_BitmapGlyph  ಗುರಿ  = (FT_BitmapGlyph)bitmap_ಗುರಿ;


    ಗುರಿ->ಎಡ = ಮೂಲ->ಎಡ;
    ಗುರಿ->ತುದಿ  = ಮೂಲ->ತುದಿ;

    ಹಿಂತಿರುಗಿ FT_Bitmap_Copy( library, &ಮೂಲ->bitmap, &ಗುರಿ->bitmap );
  }


  FT_CALLBACK_DEF( ಶೂನ್ಯ )
  ft_bitmap_glyph_ಮುಗಿದ( FT_Glyph  bitmap_glyph )
  {
    FT_BitmapGlyph  glyph   = (FT_BitmapGlyph)bitmap_glyph;
    FT_Library      library = FT_GLYPH( glyph )->library;


    FT_Bitmap_ಮುಗಿದ( library, &glyph->bitmap );
  }


  FT_CALLBACK_DEF( ಶೂನ್ಯ )
  ft_bitmap_glyph_bbox( FT_Glyph  bitmap_glyph,
                        FT_BBox*  cbox )
  {
    FT_BitmapGlyph  glyph = (FT_BitmapGlyph)bitmap_glyph;


    cbox->xMin = glyph->ಎಡ * 64;
    cbox->xMax = cbox->xMin + (FT_Pos)( glyph->bitmap.ಅಗಲ * 64 );
    cbox->yMax = glyph->ತುದಿ * 64;
    cbox->yMin = cbox->yMax - (FT_Pos)( glyph->bitmap.ಅಡ್ಡಸಾಲುಗಳು * 64 );
  }


  FT_DEFINE_GLYPH(
    ft_bitmap_glyph_class,

    ಗಾತ್ರ ( FT_BitmapGlyphRec ),
    FT_GLYPH_FORMAT_BITMAP,

    ft_bitmap_glyph_init,    /* FT_Glyph_InitFunc       glyph_init      */
    ft_bitmap_glyph_ಮುಗಿದ,    /* FT_Glyph_DoneFunc       glyph_done      */
    ft_bitmap_glyph_copy,    /* FT_Glyph_CopyFunc       glyph_copy      */
    ಖಾಲಿ,                    /* FT_Glyph_TransformFunc  glyph_transform */
    ft_bitmap_glyph_bbox,    /* FT_Glyph_GetBBoxFunc    glyph_bbox      */
    ಖಾಲಿ                     /* FT_Glyph_PrepareFunc    glyph_prepare   */
  )


  /*************************************************************************/
  /*************************************************************************/
  /****                                                                 ****/
  /****   FT_OutlineGlyph support                                       ****/
  /****                                                                 ****/
  /*************************************************************************/
  /*************************************************************************/


  FT_CALLBACK_DEF( FT_Error )
  ft_outline_glyph_init( FT_Glyph      outline_glyph,
                         FT_Glyph_ಸ್ಥಾನ  ಸ್ಥಾನ )
  {
    FT_OutlineGlyph  glyph   = (FT_OutlineGlyph)outline_glyph;
    FT_Error         error   = FT_Err_Ok;
    FT_Library       library = FT_GLYPH( glyph )->library;
    FT_Outline*      ಮೂಲ  = &ಸ್ಥಾನ->outline;
    FT_Outline*      ಗುರಿ  = &glyph->outline;


    /* check format in glyph slot */
    ಆದರೆ ( ಸ್ಥಾನ->format != FT_GLYPH_FORMAT_OUTLINE )
    {
      error = FT_THROW( Invalid_Glyph_Format );
      ಹೋಗು Exit;
    }

    /* allocate new outline */
    error = FT_Outline_New( library,
                            (FT_UInt)ಮೂಲ->n_points,
                            ಮೂಲ->n_contours,
                            &glyph->outline );
    ಆದರೆ ( error )
      ಹೋಗು Exit;

    FT_Outline_Copy( ಮೂಲ, ಗುರಿ );

  Exit:
    ಹಿಂತಿರುಗಿ error;
  }


  FT_CALLBACK_DEF( ಶೂನ್ಯ )
  ft_outline_glyph_ಮುಗಿದ( FT_Glyph  outline_glyph )
  {
    FT_OutlineGlyph  glyph = (FT_OutlineGlyph)outline_glyph;


    FT_Outline_ಮುಗಿದ( FT_GLYPH( glyph )->library, &glyph->outline );
  }


  FT_CALLBACK_DEF( FT_Error )
  ft_outline_glyph_copy( FT_Glyph  outline_ಮೂಲ,
                         FT_Glyph  outline_ಗುರಿ )
  {
    FT_OutlineGlyph  ಮೂಲ  = (FT_OutlineGlyph)outline_ಮೂಲ;
    FT_OutlineGlyph  ಗುರಿ  = (FT_OutlineGlyph)outline_ಗುರಿ;
    FT_Error         error;
    FT_Library       library = FT_GLYPH( ಮೂಲ )->library;


    error = FT_Outline_New( library,
                            (FT_UInt)ಮೂಲ->outline.n_points,
                            ಮೂಲ->outline.n_contours,
                            &ಗುರಿ->outline );
    ಆದರೆ ( !error )
      FT_Outline_Copy( &ಮೂಲ->outline, &ಗುರಿ->outline );

    ಹಿಂತಿರುಗಿ error;
  }


  FT_CALLBACK_DEF( ಶೂನ್ಯ )
  ft_outline_glyph_transform( FT_Glyph          outline_glyph,
                              ಸ್ಥಿರಾಂಕ FT_ಮಾತೃಕೆ*  ಮಾತೃಕೆ,
                              ಸ್ಥಿರಾಂಕ FT_ಸದಿಶ*  delta )
  {
    FT_OutlineGlyph  glyph = (FT_OutlineGlyph)outline_glyph;


    ಆದರೆ ( ಮಾತೃಕೆ )
      FT_Outline_Transform( &glyph->outline, ಮಾತೃಕೆ );

    ಆದರೆ ( delta )
      FT_Outline_Translate( &glyph->outline, delta->x, delta->y );
  }


  FT_CALLBACK_DEF( ಶೂನ್ಯ )
  ft_outline_glyph_bbox( FT_Glyph  outline_glyph,
                         FT_BBox*  bbox )
  {
    FT_OutlineGlyph  glyph = (FT_OutlineGlyph)outline_glyph;


    FT_Outline_Get_CBox( &glyph->outline, bbox );
  }


  FT_CALLBACK_DEF( FT_Error )
  ft_outline_glyph_prepare( FT_Glyph      outline_glyph,
                            FT_Glyph_ಸ್ಥಾನ  ಸ್ಥಾನ )
  {
    FT_OutlineGlyph  glyph = (FT_OutlineGlyph)outline_glyph;


    ಸ್ಥಾನ->format         = FT_GLYPH_FORMAT_OUTLINE;
    ಸ್ಥಾನ->outline        = glyph->outline;
    ಸ್ಥಾನ->outline.flags &= ~FT_OUTLINE_OWNER;

    ಹಿಂತಿರುಗಿ FT_Err_Ok;
  }


  FT_DEFINE_GLYPH(
    ft_outline_glyph_class,

    ಗಾತ್ರ ( FT_OutlineGlyphRec ),
    FT_GLYPH_FORMAT_OUTLINE,

    ft_outline_glyph_init,      /* FT_Glyph_InitFunc       glyph_init      */
    ft_outline_glyph_ಮುಗಿದ,      /* FT_Glyph_DoneFunc       glyph_done      */
    ft_outline_glyph_copy,      /* FT_Glyph_CopyFunc       glyph_copy      */
    ft_outline_glyph_transform, /* FT_Glyph_TransformFunc  glyph_transform */
    ft_outline_glyph_bbox,      /* FT_Glyph_GetBBoxFunc    glyph_bbox      */
    ft_outline_glyph_prepare    /* FT_Glyph_PrepareFunc    glyph_prepare   */
  )


#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ FT_CONFIG_OPTION_SVG

  /*************************************************************************/
  /*************************************************************************/
  /****                                                                 ****/
  /****   FT_SvgGlyph support                                           ****/
  /****                                                                 ****/
  /*************************************************************************/
  /*************************************************************************/


  FT_CALLBACK_DEF( FT_Error )
  ft_svg_glyph_init( FT_Glyph      svg_glyph,
                     FT_Glyph_ಸ್ಥಾನ  ಸ್ಥಾನ )
  {
    FT_ULong         doc_ಉದ್ದಳತೆ;
    FT_SVG_Document  document;
    FT_SvgGlyph      glyph = (FT_SvgGlyph)svg_glyph;

    FT_Error   error  = FT_Err_Ok;
    FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ = FT_GLYPH( glyph )->library->ಸ್ಮರಣೆ;


    ಆದರೆ ( ಸ್ಥಾನ->format != FT_GLYPH_FORMAT_SVG )
    {
      error = FT_THROW( Invalid_Glyph_Format );
      ಹೋಗು Exit;
    }

    ಆದರೆ ( ಸ್ಥಾನ->other == ಖಾಲಿ )
    {
      error = FT_THROW( Invalid_ಸ್ಥಾನ_Handle );
      ಹೋಗು Exit;
    }

    document = (FT_SVG_Document)ಸ್ಥಾನ->other;

    ಆದರೆ ( document->svg_document_ಉದ್ದಳತೆ == 0 )
    {
      error = FT_THROW( Invalid_ಸ್ಥಾನ_Handle );
      ಹೋಗು Exit;
    }

    /* allocate a new document */
    doc_ಉದ್ದಳತೆ = document->svg_document_ಉದ್ದಳತೆ;
    ಆದರೆ ( FT_QALLOC( glyph->svg_document, doc_ಉದ್ದಳತೆ ) )
      ಹೋಗು Exit;
    glyph->svg_document_ಉದ್ದಳತೆ = doc_ಉದ್ದಳತೆ;

    glyph->glyph_ಸೂಚ್ಯಂಕ = ಸ್ಥಾನ->glyph_ಸೂಚ್ಯಂಕ;

    glyph->metrics      = document->metrics;
    glyph->units_ಪ್ರತಿ_EM = document->units_ಪ್ರತಿ_EM;

    glyph->ಆರಂಭ_glyph_id = document->ಆರಂಭ_glyph_id;
    glyph->ಕೊನೆ_glyph_id   = document->ಕೊನೆ_glyph_id;

    glyph->transform = document->transform;
    glyph->delta     = document->delta;

    /* copy the document into glyph */
    FT_MEM_COPY( glyph->svg_document, document->svg_document, doc_ಉದ್ದಳತೆ );

  Exit:
    ಹಿಂತಿರುಗಿ error;
  }


  FT_CALLBACK_DEF( ಶೂನ್ಯ )
  ft_svg_glyph_ಮುಗಿದ( FT_Glyph  svg_glyph )
  {
    FT_SvgGlyph  glyph  = (FT_SvgGlyph)svg_glyph;
    FT_ಸ್ಮರಣೆ    ಸ್ಮರಣೆ = svg_glyph->library->ಸ್ಮರಣೆ;


    /* just free the memory */
    FT_FREE( glyph->svg_document );
  }


  FT_CALLBACK_DEF( FT_Error )
  ft_svg_glyph_copy( FT_Glyph  svg_ಮೂಲ,
                     FT_Glyph  svg_ಗುರಿ )
  {
    FT_SvgGlyph  ಮೂಲ = (FT_SvgGlyph)svg_ಮೂಲ;
    FT_SvgGlyph  ಗುರಿ = (FT_SvgGlyph)svg_ಗುರಿ;

    FT_Error   error  = FT_Err_Ok;
    FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ = FT_GLYPH( ಮೂಲ )->library->ಸ್ಮರಣೆ;


    ಆದರೆ ( svg_ಮೂಲ->format != FT_GLYPH_FORMAT_SVG )
    {
      error = FT_THROW( Invalid_Glyph_Format );
      ಹೋಗು Exit;
    }

    ಆದರೆ ( ಮೂಲ->svg_document_ಉದ್ದಳತೆ == 0 )
    {
      error = FT_THROW( Invalid_ಸ್ಥಾನ_Handle );
      ಹೋಗು Exit;
    }

    ಗುರಿ->glyph_ಸೂಚ್ಯಂಕ = ಮೂಲ->glyph_ಸೂಚ್ಯಂಕ;

    ಗುರಿ->svg_document_ಉದ್ದಳತೆ = ಮೂಲ->svg_document_ಉದ್ದಳತೆ;

    ಗುರಿ->metrics      = ಮೂಲ->metrics;
    ಗುರಿ->units_ಪ್ರತಿ_EM = ಮೂಲ->units_ಪ್ರತಿ_EM;

    ಗುರಿ->ಆರಂಭ_glyph_id = ಮೂಲ->ಆರಂಭ_glyph_id;
    ಗುರಿ->ಕೊನೆ_glyph_id   = ಮೂಲ->ಕೊನೆ_glyph_id;

    ಗುರಿ->transform = ಮೂಲ->transform;
    ಗುರಿ->delta     = ಮೂಲ->delta;

    /* allocate space for the SVG document */
    ಆದರೆ ( FT_QALLOC( ಗುರಿ->svg_document, ಗುರಿ->svg_document_ಉದ್ದಳತೆ ) )
      ಹೋಗು Exit;

    /* copy the document */
    FT_MEM_COPY( ಗುರಿ->svg_document,
                 ಮೂಲ->svg_document,
                 ಗುರಿ->svg_document_ಉದ್ದಳತೆ );

  Exit:
    ಹಿಂತಿರುಗಿ error;
  }


  FT_CALLBACK_DEF( ಶೂನ್ಯ )
  ft_svg_glyph_transform( FT_Glyph          svg_glyph,
                          ಸ್ಥಿರಾಂಕ FT_ಮಾತೃಕೆ*  ಮಾತೃಕೆ_2,
                          ಸ್ಥಿರಾಂಕ FT_ಸದಿಶ*  _delta )
  {
    FT_SvgGlyph  glyph  = (FT_SvgGlyph)svg_glyph;
    FT_ಮಾತೃಕೆ*   ಮಾತೃಕೆ = (FT_ಮಾತೃಕೆ*)ಮಾತೃಕೆ_2;
    FT_ಸದಿಶ*   delta  = (FT_ಸದಿಶ*)_delta;

    FT_ಮಾತೃಕೆ  tmp_ಮಾತೃಕೆ;
    FT_ಸದಿಶ  tmp_delta;

    FT_ಮಾತೃಕೆ  a, b;
    FT_Pos     x, y;


    ಆದರೆ ( !ಮಾತೃಕೆ )
    {
      tmp_ಮಾತೃಕೆ.xx = 0x10000;
      tmp_ಮಾತೃಕೆ.xy = 0;
      tmp_ಮಾತೃಕೆ.yx = 0;
      tmp_ಮಾತೃಕೆ.yy = 0x10000;

      ಮಾತೃಕೆ = &tmp_ಮಾತೃಕೆ;
    }

    ಆದರೆ ( !delta )
    {
      tmp_delta.x = 0;
      tmp_delta.y = 0;

      delta = &tmp_delta;
    }

    a = glyph->transform;
    b = *ಮಾತೃಕೆ;
    FT_ಮಾತೃಕೆ_Multiply( &b, &a );

    x = ಕೂಡಿಸು_LONG( ಕೂಡಿಸು_LONG( FT_MulFix( ಮಾತೃಕೆ->xx, glyph->delta.x ),
                            FT_MulFix( ಮಾತೃಕೆ->xy, glyph->delta.y ) ),
                  delta->x );
    y = ಕೂಡಿಸು_LONG( ಕೂಡಿಸು_LONG( FT_MulFix( ಮಾತೃಕೆ->yx, glyph->delta.x ),
                            FT_MulFix( ಮಾತೃಕೆ->yy, glyph->delta.y ) ),
                  delta->y );

    glyph->delta.x = x;
    glyph->delta.y = y;

    glyph->transform = a;
  }


  FT_CALLBACK_DEF( FT_Error )
  ft_svg_glyph_prepare( FT_Glyph      svg_glyph,
                        FT_Glyph_ಸ್ಥಾನ  ಸ್ಥಾನ )
  {
    FT_SvgGlyph  glyph = (FT_SvgGlyph)svg_glyph;

    FT_Error   error  = FT_Err_Ok;
    FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ = svg_glyph->library->ಸ್ಮರಣೆ;

    FT_SVG_Document  document = ಖಾಲಿ;


    ಆದರೆ ( FT_NEW( document ) )
      ಹಿಂತಿರುಗಿ error;

    document->svg_document        = glyph->svg_document;
    document->svg_document_ಉದ್ದಳತೆ = glyph->svg_document_ಉದ್ದಳತೆ;

    document->metrics      = glyph->metrics;
    document->units_ಪ್ರತಿ_EM = glyph->units_ಪ್ರತಿ_EM;

    document->ಆರಂಭ_glyph_id = glyph->ಆರಂಭ_glyph_id;
    document->ಕೊನೆ_glyph_id   = glyph->ಕೊನೆ_glyph_id;

    document->transform = glyph->transform;
    document->delta     = glyph->delta;

    ಸ್ಥಾನ->format      = FT_GLYPH_FORMAT_SVG;
    ಸ್ಥಾನ->glyph_ಸೂಚ್ಯಂಕ = glyph->glyph_ಸೂಚ್ಯಂಕ;
    ಸ್ಥಾನ->other       = document;

    ಹಿಂತಿರುಗಿ error;
  }


  FT_DEFINE_GLYPH(
    ft_svg_glyph_class,

    ಗಾತ್ರ ( FT_SvgGlyphRec ),
    FT_GLYPH_FORMAT_SVG,

    ft_svg_glyph_init,      /* FT_Glyph_InitFunc       glyph_init      */
    ft_svg_glyph_ಮುಗಿದ,      /* FT_Glyph_DoneFunc       glyph_done      */
    ft_svg_glyph_copy,      /* FT_Glyph_CopyFunc       glyph_copy      */
    ft_svg_glyph_transform, /* FT_Glyph_TransformFunc  glyph_transform */
    ಖಾಲಿ,                   /* FT_Glyph_GetBBoxFunc    glyph_bbox      */
    ft_svg_glyph_prepare    /* FT_Glyph_PrepareFunc    glyph_prepare   */
  )

#ಆದರೆ_ಅಂತ್ಯ /* FT_CONFIG_OPTION_SVG */


  /*************************************************************************/
  /*************************************************************************/
  /****                                                                 ****/
  /****   FT_Glyph class and API                                        ****/
  /****                                                                 ****/
  /*************************************************************************/
  /*************************************************************************/

   ಸ್ಥಿರ FT_Error
   ft_new_glyph( FT_Library             library,
                 ಸ್ಥಿರಾಂಕ FT_Glyph_Class*  clazz,
                 FT_Glyph*              aglyph )
   {
     FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ = library->ಸ್ಮರಣೆ;
     FT_Error   error;
     FT_Glyph   glyph  = ಖಾಲಿ;


     *aglyph = ಖಾಲಿ;

     ಆದರೆ ( !FT_ALLOC( glyph, clazz->glyph_ಗಾತ್ರ ) )
     {
       glyph->library = library;
       glyph->clazz   = clazz;
       glyph->format  = clazz->glyph_format;

       *aglyph = glyph;
     }

     ಹಿಂತಿರುಗಿ error;
   }


  /* documentation is in ftglyph.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Glyph_Copy( FT_Glyph   ಮೂಲ,
                 FT_Glyph  *ಗುರಿ )
  {
    FT_Glyph               copy;
    FT_Error               error;
    ಸ್ಥಿರಾಂಕ FT_Glyph_Class*  clazz;


    /* check arguments */
    ಆದರೆ ( !ಗುರಿ || !ಮೂಲ || !ಮೂಲ->clazz )
    {
      error = FT_THROW( Invalid_ವಾದ );
      ಹೋಗು Exit;
    }

    *ಗುರಿ = ಖಾಲಿ;

    ಆದರೆ ( !ಮೂಲ || !ಮೂಲ->clazz )
    {
      error = FT_THROW( Invalid_ವಾದ );
      ಹೋಗು Exit;
    }

    clazz = ಮೂಲ->clazz;
    error = ft_new_glyph( ಮೂಲ->library, clazz, &copy );
    ಆದರೆ ( error )
      ಹೋಗು Exit;

    copy->advance = ಮೂಲ->advance;
    copy->format  = ಮೂಲ->format;

    ಆದರೆ ( clazz->glyph_copy )
      error = clazz->glyph_copy( ಮೂಲ, copy );

    ಆದರೆ ( error )
      FT_ಮುಗಿದ_Glyph( copy );
    ಇಲ್ಲದಿದ್ದರೆ
      *ಗುರಿ = copy;

  Exit:
    ಹಿಂತಿರುಗಿ error;
  }


  /* documentation is in ftglyph.h */

  FT_EXPORT( FT_Error )
  FT_New_Glyph( FT_Library       library,
                FT_Glyph_Format  format,
                FT_Glyph        *aglyph )
  {
    ಸ್ಥಿರಾಂಕ FT_Glyph_Class*  clazz = ಖಾಲಿ;

    ಆದರೆ ( !library || !aglyph )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

    /* if it is a bitmap, that's easy :-) */
    ಆದರೆ ( format == FT_GLYPH_FORMAT_BITMAP )
      clazz = &ft_bitmap_glyph_class;

    /* if it is an outline */
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ( format == FT_GLYPH_FORMAT_OUTLINE )
      clazz = &ft_outline_glyph_class;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ FT_CONFIG_OPTION_SVG
    /* if it is an SVG glyph */
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ( format == FT_GLYPH_FORMAT_SVG )
      clazz = &ft_svg_glyph_class;
#ಆದರೆ_ಅಂತ್ಯ

    ಇಲ್ಲದಿದ್ದರೆ
    {
      /* try to find a renderer that supports the glyph image format */
      FT_Renderer  render = FT_Lookup_Renderer( library, format, 0 );


      ಆದರೆ ( render )
        clazz = &render->glyph_class;
    }

    ಆದರೆ ( !clazz )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_Glyph_Format );

    /* create FT_Glyph object */
    ಹಿಂತಿರುಗಿ ft_new_glyph( library, clazz, aglyph );
  }


  /* documentation is in ftglyph.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Glyph( FT_Glyph_ಸ್ಥಾನ  ಸ್ಥಾನ,
                FT_Glyph     *aglyph )
  {
    FT_Error  error;
    FT_Glyph  glyph;


    ಆದರೆ ( !ಸ್ಥಾನ )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ಸ್ಥಾನ_Handle );

    ಆದರೆ ( !aglyph )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

    /* create FT_Glyph object */
    error = FT_New_Glyph( ಸ್ಥಾನ->library, ಸ್ಥಾನ->format, &glyph );
    ಆದರೆ ( error )
      ಹೋಗು Exit;

    /* copy advance while converting 26.6 to 16.16 format */
    ಆದರೆ ( ಸ್ಥಾನ->advance.x >=  0x8000L * 64 ||
         ಸ್ಥಾನ->advance.x <= -0x8000L * 64 )
    {
      FT_ERROR(( "FT_Get_Glyph: advance width too large\n" ));
      error = FT_THROW( Invalid_ವಾದ );
      ಹೋಗು Exit2;
    }
    ಆದರೆ ( ಸ್ಥಾನ->advance.y >=  0x8000L * 64 ||
         ಸ್ಥಾನ->advance.y <= -0x8000L * 64 )
    {
      FT_ERROR(( "FT_Get_Glyph: advance height too large\n" ));
      error = FT_THROW( Invalid_ವಾದ );
      ಹೋಗು Exit2;
    }

    glyph->advance.x = ಸ್ಥಾನ->advance.x * 1024;
    glyph->advance.y = ಸ್ಥಾನ->advance.y * 1024;

    /* now import the image from the glyph slot */
    error = glyph->clazz->glyph_init( glyph, ಸ್ಥಾನ );

  Exit2:
    /* if an error occurred, destroy the glyph */
    ಆದರೆ ( error )
    {
      FT_ಮುಗಿದ_Glyph( glyph );
      *aglyph = ಖಾಲಿ;
    }
    ಇಲ್ಲದಿದ್ದರೆ
      *aglyph = glyph;

  Exit:
    ಹಿಂತಿರುಗಿ error;
  }


  /* documentation is in ftglyph.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Glyph_Transform( FT_Glyph          glyph,
                      ಸ್ಥಿರಾಂಕ FT_ಮಾತೃಕೆ*  ಮಾತೃಕೆ,
                      ಸ್ಥಿರಾಂಕ FT_ಸದಿಶ*  delta )
  {
    FT_Error  error = FT_Err_Ok;


    ಆದರೆ ( !glyph || !glyph->clazz )
      error = FT_THROW( Invalid_ವಾದ );
    ಇಲ್ಲದಿದ್ದರೆ
    {
      ಸ್ಥಿರಾಂಕ FT_Glyph_Class*  clazz = glyph->clazz;


      ಆದರೆ ( clazz->glyph_transform )
      {
        /* transform glyph image */
        clazz->glyph_transform( glyph, ಮಾತೃಕೆ, delta );

        /* transform advance vector */
        ಆದರೆ ( ಮಾತೃಕೆ )
          FT_ಸದಿಶ_Transform( &glyph->advance, ಮಾತೃಕೆ );
      }
      ಇಲ್ಲದಿದ್ದರೆ
        error = FT_THROW( Invalid_Glyph_Format );
    }
    ಹಿಂತಿರುಗಿ error;
  }


  /* documentation is in ftglyph.h */

  FT_EXPORT_DEF( ಶೂನ್ಯ )
  FT_Glyph_Get_CBox( FT_Glyph  glyph,
                     FT_UInt   bbox_ಬಹುಲಕ,
                     FT_BBox  *acbox )
  {
    ಸ್ಥಿರಾಂಕ FT_Glyph_Class*  clazz;


    ಆದರೆ ( !acbox )
      ಹಿಂತಿರುಗಿ;

    acbox->xMin = acbox->yMin = acbox->xMax = acbox->yMax = 0;

    ಆದರೆ ( !glyph || !glyph->clazz )
      ಹಿಂತಿರುಗಿ;

    clazz = glyph->clazz;
    ಆದರೆ ( !clazz->glyph_bbox )
      ಹಿಂತಿರುಗಿ;

    /* retrieve bbox in 26.6 coordinates */
    clazz->glyph_bbox( glyph, acbox );

    /* perform grid fitting if needed */
    ಆದರೆ ( bbox_ಬಹುಲಕ == FT_GLYPH_BBOX_GRIDFIT ||
         bbox_ಬಹುಲಕ == FT_GLYPH_BBOX_PIXELS  )
    {
      acbox->xMin = FT_PIX_FLOOR( acbox->xMin );
      acbox->yMin = FT_PIX_FLOOR( acbox->yMin );
      acbox->xMax = FT_PIX_CEIL_LONG( acbox->xMax );
      acbox->yMax = FT_PIX_CEIL_LONG( acbox->yMax );
    }

    /* convert to integer pixels if needed */
    ಆದರೆ ( bbox_ಬಹುಲಕ == FT_GLYPH_BBOX_TRUNCATE ||
         bbox_ಬಹುಲಕ == FT_GLYPH_BBOX_PIXELS   )
    {
      acbox->xMin >>= 6;
      acbox->yMin >>= 6;
      acbox->xMax >>= 6;
      acbox->yMax >>= 6;
    }
  }


  /* documentation is in ftglyph.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Glyph_ಗಮ್ಯ_Bitmap( FT_Glyph*         the_glyph,
                      FT_Render_ಬಹುಲಕ    render_ಬಹುಲಕ,
                      ಸ್ಥಿರಾಂಕ FT_ಸದಿಶ*  origin,
                      FT_Bool           destroy )
  {
    FT_Glyph_ಸ್ಥಾನ_Rec           dummy;
    FT_Glyph_ಸ್ಥಾನ_ಆಂತರಿಕ_Rec  dummy_ಆಂತರಿಕ;
    FT_Error                  error = FT_Err_Ok;
    FT_Glyph                  b, glyph;
    FT_BitmapGlyph            bitmap = ಖಾಲಿ;
    ಸ್ಥಿರಾಂಕ FT_Glyph_Class*     clazz;

    FT_Library                library;


    /* check argument */
    ಆದರೆ ( !the_glyph )
      ಹೋಗು Bad;
    glyph = *the_glyph;
    ಆದರೆ ( !glyph )
      ಹೋಗು Bad;

    clazz   = glyph->clazz;
    library = glyph->library;
    ಆದರೆ ( !library || !clazz )
      ಹೋಗು Bad;

    /* when called with a bitmap glyph, do nothing and return successfully */
    ಆದರೆ ( clazz == &ft_bitmap_glyph_class )
      ಹೋಗು Exit;

    ಆದರೆ ( !clazz->glyph_prepare )
      ಹೋಗು Bad;

    /* we render the glyph into a glyph bitmap using a `dummy' glyph slot */
    /* then calling FT_Render_Glyph_Internal()                            */

    FT_ZERO( &dummy );
    FT_ZERO( &dummy_ಆಂತರಿಕ );
    dummy.ಆಂತರಿಕ = &dummy_ಆಂತರಿಕ;
    dummy.library  = library;
    dummy.format   = clazz->glyph_format;

    /* create result bitmap glyph */
    error = ft_new_glyph( library, &ft_bitmap_glyph_class, &b );
    ಆದರೆ ( error )
      ಹೋಗು Exit;
    bitmap = (FT_BitmapGlyph)b;

#ಆದರೆ 1
    /* if `origin' is set, translate the glyph image */
    ಆದರೆ ( origin )
      FT_Glyph_Transform( glyph, ಖಾಲಿ, origin );
#ಇಲ್ಲದಿದ್ದರೆ
    FT_ಬಳಸದ( origin );
#ಆದರೆ_ಅಂತ್ಯ

    /* prepare dummy slot for rendering */
    error = clazz->glyph_prepare( glyph, &dummy );
    ಆದರೆ ( !error )
      error = FT_Render_Glyph_ಆಂತರಿಕ( glyph->library, &dummy, render_ಬಹುಲಕ );

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ FT_CONFIG_OPTION_SVG
    ಆದರೆ ( clazz == &ft_svg_glyph_class )
    {
      FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ = library->ಸ್ಮರಣೆ;


      FT_FREE( dummy.other );
    }
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ 1
    ಆದರೆ ( !destroy && origin )
    {
      FT_ಸದಿಶ  v;


      v.x = -origin->x;
      v.y = -origin->y;
      FT_Glyph_Transform( glyph, ಖಾಲಿ, &v );
    }
#ಆದರೆ_ಅಂತ್ಯ

    ಆದರೆ ( error )
      ಹೋಗು Exit;

    /* in case of success, copy the bitmap to the glyph bitmap */
    bitmap->ಎಡ   = dummy.bitmap_ಎಡ;
    bitmap->ತುದಿ    = dummy.bitmap_ತುದಿ;
    bitmap->bitmap = dummy.bitmap;

    /* copy advance */
    bitmap->ಬೇರು.advance = glyph->advance;

    ಆದರೆ ( destroy )
      FT_ಮುಗಿದ_Glyph( glyph );

    *the_glyph = FT_GLYPH( bitmap );

  Exit:
    ಆದರೆ ( error && bitmap )
      FT_ಮುಗಿದ_Glyph( FT_GLYPH( bitmap ) );

    ಹಿಂತಿರುಗಿ error;

  Bad:
    error = FT_THROW( Invalid_ವಾದ );
    ಹೋಗು Exit;
  }


  /* documentation is in ftglyph.h */

  FT_EXPORT_DEF( ಶೂನ್ಯ )
  FT_ಮುಗಿದ_Glyph( FT_Glyph  glyph )
  {
    ಆದರೆ ( glyph )
    {
      FT_ಸ್ಮರಣೆ              ಸ್ಮರಣೆ = glyph->library->ಸ್ಮರಣೆ;
      ಸ್ಥಿರಾಂಕ FT_Glyph_Class*  clazz  = glyph->clazz;


      ಆದರೆ ( clazz->glyph_ಮುಗಿದ )
        clazz->glyph_ಮುಗಿದ( glyph );

      FT_FREE( glyph );
    }
  }


/*ಕೊನೆ*/
