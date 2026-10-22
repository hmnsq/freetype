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


#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftdebug.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/ftglyph.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/ftoutln.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/ftbitmap.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftobjs.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/otsvg.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "ftbase.h"


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ  FT_COMPONENT
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FT_COMPONENT  glyph


  /*************************************************************************/
  /*************************************************************************/
  /****                                                                 ****/
  /****   FT_BitmapGlyph support                                        ****/
  /****                                                                 ****/
  /*************************************************************************/
  /*************************************************************************/

  FT_CALLBACK_DEF( FT_Error )
  ft_bitmap_glyph_init( FT_Glyph      bitmap_glyph,
                        FT_Glyph_ਥਾਂ  ਥਾਂ )
  {
    FT_BitmapGlyph  glyph   = (FT_BitmapGlyph)bitmap_glyph;
    FT_Error        error;
    FT_Library      library = FT_GLYPH( glyph )->library;


    ਜੇ ( ਥਾਂ->format != FT_GLYPH_FORMAT_BITMAP )
    {
      error = FT_THROW( Invalid_Glyph_Format );
      ਜਾਓ Exit;
    }

    glyph->ਖੱਬਾ = ਥਾਂ->bitmap_ਖੱਬਾ;
    glyph->ਸਿਖਰ  = ਥਾਂ->bitmap_ਸਿਖਰ;

    FT_Bitmap_Init( &glyph->bitmap );
    error = FT_Bitmap_Copy( library, &ਥਾਂ->bitmap, &glyph->bitmap );

  Exit:
    ਵਾਪਸ error;
  }


  FT_CALLBACK_DEF( FT_Error )
  ft_bitmap_glyph_copy( FT_Glyph  bitmap_ਸਰੋਤ,
                        FT_Glyph  bitmap_ਨਿਸ਼ਾਨਾ )
  {
    FT_Library      library = bitmap_ਸਰੋਤ->library;
    FT_BitmapGlyph  ਸਰੋਤ  = (FT_BitmapGlyph)bitmap_ਸਰੋਤ;
    FT_BitmapGlyph  ਨਿਸ਼ਾਨਾ  = (FT_BitmapGlyph)bitmap_ਨਿਸ਼ਾਨਾ;


    ਨਿਸ਼ਾਨਾ->ਖੱਬਾ = ਸਰੋਤ->ਖੱਬਾ;
    ਨਿਸ਼ਾਨਾ->ਸਿਖਰ  = ਸਰੋਤ->ਸਿਖਰ;

    ਵਾਪਸ FT_Bitmap_Copy( library, &ਸਰੋਤ->bitmap, &ਨਿਸ਼ਾਨਾ->bitmap );
  }


  FT_CALLBACK_DEF( ਖਾਲੀ )
  ft_bitmap_glyph_ਪੂਰਾ( FT_Glyph  bitmap_glyph )
  {
    FT_BitmapGlyph  glyph   = (FT_BitmapGlyph)bitmap_glyph;
    FT_Library      library = FT_GLYPH( glyph )->library;


    FT_Bitmap_ਪੂਰਾ( library, &glyph->bitmap );
  }


  FT_CALLBACK_DEF( ਖਾਲੀ )
  ft_bitmap_glyph_bbox( FT_Glyph  bitmap_glyph,
                        FT_BBox*  cbox )
  {
    FT_BitmapGlyph  glyph = (FT_BitmapGlyph)bitmap_glyph;


    cbox->xMin = glyph->ਖੱਬਾ * 64;
    cbox->xMax = cbox->xMin + (FT_Pos)( glyph->bitmap.ਚੌੜਾਈ * 64 );
    cbox->yMax = glyph->ਸਿਖਰ * 64;
    cbox->yMin = cbox->yMax - (FT_Pos)( glyph->bitmap.ਪੰਕਤੀਆਂ * 64 );
  }


  FT_DEFINE_GLYPH(
    ft_bitmap_glyph_class,

    ਆਕਾਰ ( FT_BitmapGlyphRec ),
    FT_GLYPH_FORMAT_BITMAP,

    ft_bitmap_glyph_init,    /* FT_Glyph_InitFunc       glyph_init      */
    ft_bitmap_glyph_ਪੂਰਾ,    /* FT_Glyph_DoneFunc       glyph_done      */
    ft_bitmap_glyph_copy,    /* FT_Glyph_CopyFunc       glyph_copy      */
    ਨਿਰਾ,                    /* FT_Glyph_TransformFunc  glyph_transform */
    ft_bitmap_glyph_bbox,    /* FT_Glyph_GetBBoxFunc    glyph_bbox      */
    ਨਿਰਾ                     /* FT_Glyph_PrepareFunc    glyph_prepare   */
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
                         FT_Glyph_ਥਾਂ  ਥਾਂ )
  {
    FT_OutlineGlyph  glyph   = (FT_OutlineGlyph)outline_glyph;
    FT_Error         error   = FT_Err_Ok;
    FT_Library       library = FT_GLYPH( glyph )->library;
    FT_Outline*      ਸਰੋਤ  = &ਥਾਂ->outline;
    FT_Outline*      ਨਿਸ਼ਾਨਾ  = &glyph->outline;


    /* check format in glyph slot */
    ਜੇ ( ਥਾਂ->format != FT_GLYPH_FORMAT_OUTLINE )
    {
      error = FT_THROW( Invalid_Glyph_Format );
      ਜਾਓ Exit;
    }

    /* allocate new outline */
    error = FT_Outline_New( library,
                            (FT_UInt)ਸਰੋਤ->n_points,
                            ਸਰੋਤ->n_contours,
                            &glyph->outline );
    ਜੇ ( error )
      ਜਾਓ Exit;

    FT_Outline_Copy( ਸਰੋਤ, ਨਿਸ਼ਾਨਾ );

  Exit:
    ਵਾਪਸ error;
  }


  FT_CALLBACK_DEF( ਖਾਲੀ )
  ft_outline_glyph_ਪੂਰਾ( FT_Glyph  outline_glyph )
  {
    FT_OutlineGlyph  glyph = (FT_OutlineGlyph)outline_glyph;


    FT_Outline_ਪੂਰਾ( FT_GLYPH( glyph )->library, &glyph->outline );
  }


  FT_CALLBACK_DEF( FT_Error )
  ft_outline_glyph_copy( FT_Glyph  outline_ਸਰੋਤ,
                         FT_Glyph  outline_ਨਿਸ਼ਾਨਾ )
  {
    FT_OutlineGlyph  ਸਰੋਤ  = (FT_OutlineGlyph)outline_ਸਰੋਤ;
    FT_OutlineGlyph  ਨਿਸ਼ਾਨਾ  = (FT_OutlineGlyph)outline_ਨਿਸ਼ਾਨਾ;
    FT_Error         error;
    FT_Library       library = FT_GLYPH( ਸਰੋਤ )->library;


    error = FT_Outline_New( library,
                            (FT_UInt)ਸਰੋਤ->outline.n_points,
                            ਸਰੋਤ->outline.n_contours,
                            &ਨਿਸ਼ਾਨਾ->outline );
    ਜੇ ( !error )
      FT_Outline_Copy( &ਸਰੋਤ->outline, &ਨਿਸ਼ਾਨਾ->outline );

    ਵਾਪਸ error;
  }


  FT_CALLBACK_DEF( ਖਾਲੀ )
  ft_outline_glyph_transform( FT_Glyph          outline_glyph,
                              ਅਚਲ FT_ਮੈਟ੍ਰਿਕਸ*  ਮੈਟ੍ਰਿਕਸ,
                              ਅਚਲ FT_ਸਦਿਸ਼*  delta )
  {
    FT_OutlineGlyph  glyph = (FT_OutlineGlyph)outline_glyph;


    ਜੇ ( ਮੈਟ੍ਰਿਕਸ )
      FT_Outline_Transform( &glyph->outline, ਮੈਟ੍ਰਿਕਸ );

    ਜੇ ( delta )
      FT_Outline_Translate( &glyph->outline, delta->x, delta->y );
  }


  FT_CALLBACK_DEF( ਖਾਲੀ )
  ft_outline_glyph_bbox( FT_Glyph  outline_glyph,
                         FT_BBox*  bbox )
  {
    FT_OutlineGlyph  glyph = (FT_OutlineGlyph)outline_glyph;


    FT_Outline_Get_CBox( &glyph->outline, bbox );
  }


  FT_CALLBACK_DEF( FT_Error )
  ft_outline_glyph_prepare( FT_Glyph      outline_glyph,
                            FT_Glyph_ਥਾਂ  ਥਾਂ )
  {
    FT_OutlineGlyph  glyph = (FT_OutlineGlyph)outline_glyph;


    ਥਾਂ->format         = FT_GLYPH_FORMAT_OUTLINE;
    ਥਾਂ->outline        = glyph->outline;
    ਥਾਂ->outline.flags &= ~FT_OUTLINE_OWNER;

    ਵਾਪਸ FT_Err_Ok;
  }


  FT_DEFINE_GLYPH(
    ft_outline_glyph_class,

    ਆਕਾਰ ( FT_OutlineGlyphRec ),
    FT_GLYPH_FORMAT_OUTLINE,

    ft_outline_glyph_init,      /* FT_Glyph_InitFunc       glyph_init      */
    ft_outline_glyph_ਪੂਰਾ,      /* FT_Glyph_DoneFunc       glyph_done      */
    ft_outline_glyph_copy,      /* FT_Glyph_CopyFunc       glyph_copy      */
    ft_outline_glyph_transform, /* FT_Glyph_TransformFunc  glyph_transform */
    ft_outline_glyph_bbox,      /* FT_Glyph_GetBBoxFunc    glyph_bbox      */
    ft_outline_glyph_prepare    /* FT_Glyph_PrepareFunc    glyph_prepare   */
  )


#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ FT_CONFIG_OPTION_SVG

  /*************************************************************************/
  /*************************************************************************/
  /****                                                                 ****/
  /****   FT_SvgGlyph support                                           ****/
  /****                                                                 ****/
  /*************************************************************************/
  /*************************************************************************/


  FT_CALLBACK_DEF( FT_Error )
  ft_svg_glyph_init( FT_Glyph      svg_glyph,
                     FT_Glyph_ਥਾਂ  ਥਾਂ )
  {
    FT_ULong         doc_ਲੰਮਾਈ;
    FT_SVG_Document  document;
    FT_SvgGlyph      glyph = (FT_SvgGlyph)svg_glyph;

    FT_Error   error  = FT_Err_Ok;
    FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ = FT_GLYPH( glyph )->library->ਸਮ੍ਰਿਤੀ;


    ਜੇ ( ਥਾਂ->format != FT_GLYPH_FORMAT_SVG )
    {
      error = FT_THROW( Invalid_Glyph_Format );
      ਜਾਓ Exit;
    }

    ਜੇ ( ਥਾਂ->other == ਨਿਰਾ )
    {
      error = FT_THROW( Invalid_ਥਾਂ_Handle );
      ਜਾਓ Exit;
    }

    document = (FT_SVG_Document)ਥਾਂ->other;

    ਜੇ ( document->svg_document_ਲੰਮਾਈ == 0 )
    {
      error = FT_THROW( Invalid_ਥਾਂ_Handle );
      ਜਾਓ Exit;
    }

    /* allocate a new document */
    doc_ਲੰਮਾਈ = document->svg_document_ਲੰਮਾਈ;
    ਜੇ ( FT_QALLOC( glyph->svg_document, doc_ਲੰਮਾਈ ) )
      ਜਾਓ Exit;
    glyph->svg_document_ਲੰਮਾਈ = doc_ਲੰਮਾਈ;

    glyph->glyph_ਸੂਚਕਾਂਕ = ਥਾਂ->glyph_ਸੂਚਕਾਂਕ;

    glyph->metrics      = document->metrics;
    glyph->units_ਪ੍ਰਤੀ_EM = document->units_ਪ੍ਰਤੀ_EM;

    glyph->ਸ਼ੁਰੂ_glyph_id = document->ਸ਼ੁਰੂ_glyph_id;
    glyph->ਸਿਰਾ_glyph_id   = document->ਸਿਰਾ_glyph_id;

    glyph->transform = document->transform;
    glyph->delta     = document->delta;

    /* copy the document into glyph */
    FT_MEM_COPY( glyph->svg_document, document->svg_document, doc_ਲੰਮਾਈ );

  Exit:
    ਵਾਪਸ error;
  }


  FT_CALLBACK_DEF( ਖਾਲੀ )
  ft_svg_glyph_ਪੂਰਾ( FT_Glyph  svg_glyph )
  {
    FT_SvgGlyph  glyph  = (FT_SvgGlyph)svg_glyph;
    FT_ਸਮ੍ਰਿਤੀ    ਸਮ੍ਰਿਤੀ = svg_glyph->library->ਸਮ੍ਰਿਤੀ;


    /* just free the memory */
    FT_FREE( glyph->svg_document );
  }


  FT_CALLBACK_DEF( FT_Error )
  ft_svg_glyph_copy( FT_Glyph  svg_ਸਰੋਤ,
                     FT_Glyph  svg_ਨਿਸ਼ਾਨਾ )
  {
    FT_SvgGlyph  ਸਰੋਤ = (FT_SvgGlyph)svg_ਸਰੋਤ;
    FT_SvgGlyph  ਨਿਸ਼ਾਨਾ = (FT_SvgGlyph)svg_ਨਿਸ਼ਾਨਾ;

    FT_Error   error  = FT_Err_Ok;
    FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ = FT_GLYPH( ਸਰੋਤ )->library->ਸਮ੍ਰਿਤੀ;


    ਜੇ ( svg_ਸਰੋਤ->format != FT_GLYPH_FORMAT_SVG )
    {
      error = FT_THROW( Invalid_Glyph_Format );
      ਜਾਓ Exit;
    }

    ਜੇ ( ਸਰੋਤ->svg_document_ਲੰਮਾਈ == 0 )
    {
      error = FT_THROW( Invalid_ਥਾਂ_Handle );
      ਜਾਓ Exit;
    }

    ਨਿਸ਼ਾਨਾ->glyph_ਸੂਚਕਾਂਕ = ਸਰੋਤ->glyph_ਸੂਚਕਾਂਕ;

    ਨਿਸ਼ਾਨਾ->svg_document_ਲੰਮਾਈ = ਸਰੋਤ->svg_document_ਲੰਮਾਈ;

    ਨਿਸ਼ਾਨਾ->metrics      = ਸਰੋਤ->metrics;
    ਨਿਸ਼ਾਨਾ->units_ਪ੍ਰਤੀ_EM = ਸਰੋਤ->units_ਪ੍ਰਤੀ_EM;

    ਨਿਸ਼ਾਨਾ->ਸ਼ੁਰੂ_glyph_id = ਸਰੋਤ->ਸ਼ੁਰੂ_glyph_id;
    ਨਿਸ਼ਾਨਾ->ਸਿਰਾ_glyph_id   = ਸਰੋਤ->ਸਿਰਾ_glyph_id;

    ਨਿਸ਼ਾਨਾ->transform = ਸਰੋਤ->transform;
    ਨਿਸ਼ਾਨਾ->delta     = ਸਰੋਤ->delta;

    /* allocate space for the SVG document */
    ਜੇ ( FT_QALLOC( ਨਿਸ਼ਾਨਾ->svg_document, ਨਿਸ਼ਾਨਾ->svg_document_ਲੰਮਾਈ ) )
      ਜਾਓ Exit;

    /* copy the document */
    FT_MEM_COPY( ਨਿਸ਼ਾਨਾ->svg_document,
                 ਸਰੋਤ->svg_document,
                 ਨਿਸ਼ਾਨਾ->svg_document_ਲੰਮਾਈ );

  Exit:
    ਵਾਪਸ error;
  }


  FT_CALLBACK_DEF( ਖਾਲੀ )
  ft_svg_glyph_transform( FT_Glyph          svg_glyph,
                          ਅਚਲ FT_ਮੈਟ੍ਰਿਕਸ*  ਮੈਟ੍ਰਿਕਸ_2,
                          ਅਚਲ FT_ਸਦਿਸ਼*  _delta )
  {
    FT_SvgGlyph  glyph  = (FT_SvgGlyph)svg_glyph;
    FT_ਮੈਟ੍ਰਿਕਸ*   ਮੈਟ੍ਰਿਕਸ = (FT_ਮੈਟ੍ਰਿਕਸ*)ਮੈਟ੍ਰਿਕਸ_2;
    FT_ਸਦਿਸ਼*   delta  = (FT_ਸਦਿਸ਼*)_delta;

    FT_ਮੈਟ੍ਰਿਕਸ  tmp_ਮੈਟ੍ਰਿਕਸ;
    FT_ਸਦਿਸ਼  tmp_delta;

    FT_ਮੈਟ੍ਰਿਕਸ  a, b;
    FT_Pos     x, y;


    ਜੇ ( !ਮੈਟ੍ਰਿਕਸ )
    {
      tmp_ਮੈਟ੍ਰਿਕਸ.xx = 0x10000;
      tmp_ਮੈਟ੍ਰਿਕਸ.xy = 0;
      tmp_ਮੈਟ੍ਰਿਕਸ.yx = 0;
      tmp_ਮੈਟ੍ਰਿਕਸ.yy = 0x10000;

      ਮੈਟ੍ਰਿਕਸ = &tmp_ਮੈਟ੍ਰਿਕਸ;
    }

    ਜੇ ( !delta )
    {
      tmp_delta.x = 0;
      tmp_delta.y = 0;

      delta = &tmp_delta;
    }

    a = glyph->transform;
    b = *ਮੈਟ੍ਰਿਕਸ;
    FT_ਮੈਟ੍ਰਿਕਸ_Multiply( &b, &a );

    x = ਜੋੜੋ_LONG( ਜੋੜੋ_LONG( FT_MulFix( ਮੈਟ੍ਰਿਕਸ->xx, glyph->delta.x ),
                            FT_MulFix( ਮੈਟ੍ਰਿਕਸ->xy, glyph->delta.y ) ),
                  delta->x );
    y = ਜੋੜੋ_LONG( ਜੋੜੋ_LONG( FT_MulFix( ਮੈਟ੍ਰਿਕਸ->yx, glyph->delta.x ),
                            FT_MulFix( ਮੈਟ੍ਰਿਕਸ->yy, glyph->delta.y ) ),
                  delta->y );

    glyph->delta.x = x;
    glyph->delta.y = y;

    glyph->transform = a;
  }


  FT_CALLBACK_DEF( FT_Error )
  ft_svg_glyph_prepare( FT_Glyph      svg_glyph,
                        FT_Glyph_ਥਾਂ  ਥਾਂ )
  {
    FT_SvgGlyph  glyph = (FT_SvgGlyph)svg_glyph;

    FT_Error   error  = FT_Err_Ok;
    FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ = svg_glyph->library->ਸਮ੍ਰਿਤੀ;

    FT_SVG_Document  document = ਨਿਰਾ;


    ਜੇ ( FT_NEW( document ) )
      ਵਾਪਸ error;

    document->svg_document        = glyph->svg_document;
    document->svg_document_ਲੰਮਾਈ = glyph->svg_document_ਲੰਮਾਈ;

    document->metrics      = glyph->metrics;
    document->units_ਪ੍ਰਤੀ_EM = glyph->units_ਪ੍ਰਤੀ_EM;

    document->ਸ਼ੁਰੂ_glyph_id = glyph->ਸ਼ੁਰੂ_glyph_id;
    document->ਸਿਰਾ_glyph_id   = glyph->ਸਿਰਾ_glyph_id;

    document->transform = glyph->transform;
    document->delta     = glyph->delta;

    ਥਾਂ->format      = FT_GLYPH_FORMAT_SVG;
    ਥਾਂ->glyph_ਸੂਚਕਾਂਕ = glyph->glyph_ਸੂਚਕਾਂਕ;
    ਥਾਂ->other       = document;

    ਵਾਪਸ error;
  }


  FT_DEFINE_GLYPH(
    ft_svg_glyph_class,

    ਆਕਾਰ ( FT_SvgGlyphRec ),
    FT_GLYPH_FORMAT_SVG,

    ft_svg_glyph_init,      /* FT_Glyph_InitFunc       glyph_init      */
    ft_svg_glyph_ਪੂਰਾ,      /* FT_Glyph_DoneFunc       glyph_done      */
    ft_svg_glyph_copy,      /* FT_Glyph_CopyFunc       glyph_copy      */
    ft_svg_glyph_transform, /* FT_Glyph_TransformFunc  glyph_transform */
    ਨਿਰਾ,                   /* FT_Glyph_GetBBoxFunc    glyph_bbox      */
    ft_svg_glyph_prepare    /* FT_Glyph_PrepareFunc    glyph_prepare   */
  )

#ਜੇ_ਅੰਤ /* FT_CONFIG_OPTION_SVG */


  /*************************************************************************/
  /*************************************************************************/
  /****                                                                 ****/
  /****   FT_Glyph class and API                                        ****/
  /****                                                                 ****/
  /*************************************************************************/
  /*************************************************************************/

   ਸਥਿਰ FT_Error
   ft_new_glyph( FT_Library             library,
                 ਅਚਲ FT_Glyph_Class*  clazz,
                 FT_Glyph*              aglyph )
   {
     FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ = library->ਸਮ੍ਰਿਤੀ;
     FT_Error   error;
     FT_Glyph   glyph  = ਨਿਰਾ;


     *aglyph = ਨਿਰਾ;

     ਜੇ ( !FT_ALLOC( glyph, clazz->glyph_ਮਾਪ ) )
     {
       glyph->library = library;
       glyph->clazz   = clazz;
       glyph->format  = clazz->glyph_format;

       *aglyph = glyph;
     }

     ਵਾਪਸ error;
   }


  /* documentation is in ftglyph.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Glyph_Copy( FT_Glyph   ਸਰੋਤ,
                 FT_Glyph  *ਨਿਸ਼ਾਨਾ )
  {
    FT_Glyph               copy;
    FT_Error               error;
    ਅਚਲ FT_Glyph_Class*  clazz;


    /* check arguments */
    ਜੇ ( !ਨਿਸ਼ਾਨਾ || !ਸਰੋਤ || !ਸਰੋਤ->clazz )
    {
      error = FT_THROW( Invalid_ਦਲੀਲ );
      ਜਾਓ Exit;
    }

    *ਨਿਸ਼ਾਨਾ = ਨਿਰਾ;

    ਜੇ ( !ਸਰੋਤ || !ਸਰੋਤ->clazz )
    {
      error = FT_THROW( Invalid_ਦਲੀਲ );
      ਜਾਓ Exit;
    }

    clazz = ਸਰੋਤ->clazz;
    error = ft_new_glyph( ਸਰੋਤ->library, clazz, &copy );
    ਜੇ ( error )
      ਜਾਓ Exit;

    copy->advance = ਸਰੋਤ->advance;
    copy->format  = ਸਰੋਤ->format;

    ਜੇ ( clazz->glyph_copy )
      error = clazz->glyph_copy( ਸਰੋਤ, copy );

    ਜੇ ( error )
      FT_ਪੂਰਾ_Glyph( copy );
    ਨਹੀਂਤਾਂ
      *ਨਿਸ਼ਾਨਾ = copy;

  Exit:
    ਵਾਪਸ error;
  }


  /* documentation is in ftglyph.h */

  FT_EXPORT( FT_Error )
  FT_New_Glyph( FT_Library       library,
                FT_Glyph_Format  format,
                FT_Glyph        *aglyph )
  {
    ਅਚਲ FT_Glyph_Class*  clazz = ਨਿਰਾ;

    ਜੇ ( !library || !aglyph )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    /* if it is a bitmap, that's easy :-) */
    ਜੇ ( format == FT_GLYPH_FORMAT_BITMAP )
      clazz = &ft_bitmap_glyph_class;

    /* if it is an outline */
    ਨਹੀਂਤਾਂ ਜੇ ( format == FT_GLYPH_FORMAT_OUTLINE )
      clazz = &ft_outline_glyph_class;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ FT_CONFIG_OPTION_SVG
    /* if it is an SVG glyph */
    ਨਹੀਂਤਾਂ ਜੇ ( format == FT_GLYPH_FORMAT_SVG )
      clazz = &ft_svg_glyph_class;
#ਜੇ_ਅੰਤ

    ਨਹੀਂਤਾਂ
    {
      /* try to find a renderer that supports the glyph image format */
      FT_Renderer  render = FT_Lookup_Renderer( library, format, 0 );


      ਜੇ ( render )
        clazz = &render->glyph_class;
    }

    ਜੇ ( !clazz )
      ਵਾਪਸ FT_THROW( Invalid_Glyph_Format );

    /* create FT_Glyph object */
    ਵਾਪਸ ft_new_glyph( library, clazz, aglyph );
  }


  /* documentation is in ftglyph.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Glyph( FT_Glyph_ਥਾਂ  ਥਾਂ,
                FT_Glyph     *aglyph )
  {
    FT_Error  error;
    FT_Glyph  glyph;


    ਜੇ ( !ਥਾਂ )
      ਵਾਪਸ FT_THROW( Invalid_ਥਾਂ_Handle );

    ਜੇ ( !aglyph )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    /* create FT_Glyph object */
    error = FT_New_Glyph( ਥਾਂ->library, ਥਾਂ->format, &glyph );
    ਜੇ ( error )
      ਜਾਓ Exit;

    /* copy advance while converting 26.6 to 16.16 format */
    ਜੇ ( ਥਾਂ->advance.x >=  0x8000L * 64 ||
         ਥਾਂ->advance.x <= -0x8000L * 64 )
    {
      FT_ERROR(( "FT_Get_Glyph: advance width too large\n" ));
      error = FT_THROW( Invalid_ਦਲੀਲ );
      ਜਾਓ Exit2;
    }
    ਜੇ ( ਥਾਂ->advance.y >=  0x8000L * 64 ||
         ਥਾਂ->advance.y <= -0x8000L * 64 )
    {
      FT_ERROR(( "FT_Get_Glyph: advance height too large\n" ));
      error = FT_THROW( Invalid_ਦਲੀਲ );
      ਜਾਓ Exit2;
    }

    glyph->advance.x = ਥਾਂ->advance.x * 1024;
    glyph->advance.y = ਥਾਂ->advance.y * 1024;

    /* now import the image from the glyph slot */
    error = glyph->clazz->glyph_init( glyph, ਥਾਂ );

  Exit2:
    /* if an error occurred, destroy the glyph */
    ਜੇ ( error )
    {
      FT_ਪੂਰਾ_Glyph( glyph );
      *aglyph = ਨਿਰਾ;
    }
    ਨਹੀਂਤਾਂ
      *aglyph = glyph;

  Exit:
    ਵਾਪਸ error;
  }


  /* documentation is in ftglyph.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Glyph_Transform( FT_Glyph          glyph,
                      ਅਚਲ FT_ਮੈਟ੍ਰਿਕਸ*  ਮੈਟ੍ਰਿਕਸ,
                      ਅਚਲ FT_ਸਦਿਸ਼*  delta )
  {
    FT_Error  error = FT_Err_Ok;


    ਜੇ ( !glyph || !glyph->clazz )
      error = FT_THROW( Invalid_ਦਲੀਲ );
    ਨਹੀਂਤਾਂ
    {
      ਅਚਲ FT_Glyph_Class*  clazz = glyph->clazz;


      ਜੇ ( clazz->glyph_transform )
      {
        /* transform glyph image */
        clazz->glyph_transform( glyph, ਮੈਟ੍ਰਿਕਸ, delta );

        /* transform advance vector */
        ਜੇ ( ਮੈਟ੍ਰਿਕਸ )
          FT_ਸਦਿਸ਼_Transform( &glyph->advance, ਮੈਟ੍ਰਿਕਸ );
      }
      ਨਹੀਂਤਾਂ
        error = FT_THROW( Invalid_Glyph_Format );
    }
    ਵਾਪਸ error;
  }


  /* documentation is in ftglyph.h */

  FT_EXPORT_DEF( ਖਾਲੀ )
  FT_Glyph_Get_CBox( FT_Glyph  glyph,
                     FT_UInt   bbox_ਬਹੁਲਕ,
                     FT_BBox  *acbox )
  {
    ਅਚਲ FT_Glyph_Class*  clazz;


    ਜੇ ( !acbox )
      ਵਾਪਸ;

    acbox->xMin = acbox->yMin = acbox->xMax = acbox->yMax = 0;

    ਜੇ ( !glyph || !glyph->clazz )
      ਵਾਪਸ;

    clazz = glyph->clazz;
    ਜੇ ( !clazz->glyph_bbox )
      ਵਾਪਸ;

    /* retrieve bbox in 26.6 coordinates */
    clazz->glyph_bbox( glyph, acbox );

    /* perform grid fitting if needed */
    ਜੇ ( bbox_ਬਹੁਲਕ == FT_GLYPH_BBOX_GRIDFIT ||
         bbox_ਬਹੁਲਕ == FT_GLYPH_BBOX_PIXELS  )
    {
      acbox->xMin = FT_PIX_FLOOR( acbox->xMin );
      acbox->yMin = FT_PIX_FLOOR( acbox->yMin );
      acbox->xMax = FT_PIX_CEIL_LONG( acbox->xMax );
      acbox->yMax = FT_PIX_CEIL_LONG( acbox->yMax );
    }

    /* convert to integer pixels if needed */
    ਜੇ ( bbox_ਬਹੁਲਕ == FT_GLYPH_BBOX_TRUNCATE ||
         bbox_ਬਹੁਲਕ == FT_GLYPH_BBOX_PIXELS   )
    {
      acbox->xMin >>= 6;
      acbox->yMin >>= 6;
      acbox->xMax >>= 6;
      acbox->yMax >>= 6;
    }
  }


  /* documentation is in ftglyph.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Glyph_ਮੰਜ਼ਿਲ_Bitmap( FT_Glyph*         the_glyph,
                      FT_Render_ਬਹੁਲਕ    render_ਬਹੁਲਕ,
                      ਅਚਲ FT_ਸਦਿਸ਼*  origin,
                      FT_Bool           destroy )
  {
    FT_Glyph_ਥਾਂ_Rec           dummy;
    FT_Glyph_ਥਾਂ_ਅੰਦਰੂਨੀ_Rec  dummy_ਅੰਦਰੂਨੀ;
    FT_Error                  error = FT_Err_Ok;
    FT_Glyph                  b, glyph;
    FT_BitmapGlyph            bitmap = ਨਿਰਾ;
    ਅਚਲ FT_Glyph_Class*     clazz;

    FT_Library                library;


    /* check argument */
    ਜੇ ( !the_glyph )
      ਜਾਓ Bad;
    glyph = *the_glyph;
    ਜੇ ( !glyph )
      ਜਾਓ Bad;

    clazz   = glyph->clazz;
    library = glyph->library;
    ਜੇ ( !library || !clazz )
      ਜਾਓ Bad;

    /* when called with a bitmap glyph, do nothing and return successfully */
    ਜੇ ( clazz == &ft_bitmap_glyph_class )
      ਜਾਓ Exit;

    ਜੇ ( !clazz->glyph_prepare )
      ਜਾਓ Bad;

    /* we render the glyph into a glyph bitmap using a `dummy' glyph slot */
    /* then calling FT_Render_Glyph_Internal()                            */

    FT_ZERO( &dummy );
    FT_ZERO( &dummy_ਅੰਦਰੂਨੀ );
    dummy.ਅੰਦਰੂਨੀ = &dummy_ਅੰਦਰੂਨੀ;
    dummy.library  = library;
    dummy.format   = clazz->glyph_format;

    /* create result bitmap glyph */
    error = ft_new_glyph( library, &ft_bitmap_glyph_class, &b );
    ਜੇ ( error )
      ਜਾਓ Exit;
    bitmap = (FT_BitmapGlyph)b;

#ਜੇ 1
    /* if `origin' is set, translate the glyph image */
    ਜੇ ( origin )
      FT_Glyph_Transform( glyph, ਨਿਰਾ, origin );
#ਨਹੀਂਤਾਂ
    FT_ਅਣਵਰਤਿਆ( origin );
#ਜੇ_ਅੰਤ

    /* prepare dummy slot for rendering */
    error = clazz->glyph_prepare( glyph, &dummy );
    ਜੇ ( !error )
      error = FT_Render_Glyph_ਅੰਦਰੂਨੀ( glyph->library, &dummy, render_ਬਹੁਲਕ );

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ FT_CONFIG_OPTION_SVG
    ਜੇ ( clazz == &ft_svg_glyph_class )
    {
      FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ = library->ਸਮ੍ਰਿਤੀ;


      FT_FREE( dummy.other );
    }
#ਜੇ_ਅੰਤ

#ਜੇ 1
    ਜੇ ( !destroy && origin )
    {
      FT_ਸਦਿਸ਼  v;


      v.x = -origin->x;
      v.y = -origin->y;
      FT_Glyph_Transform( glyph, ਨਿਰਾ, &v );
    }
#ਜੇ_ਅੰਤ

    ਜੇ ( error )
      ਜਾਓ Exit;

    /* in case of success, copy the bitmap to the glyph bitmap */
    bitmap->ਖੱਬਾ   = dummy.bitmap_ਖੱਬਾ;
    bitmap->ਸਿਖਰ    = dummy.bitmap_ਸਿਖਰ;
    bitmap->bitmap = dummy.bitmap;

    /* copy advance */
    bitmap->ਜੜ੍ਹ.advance = glyph->advance;

    ਜੇ ( destroy )
      FT_ਪੂਰਾ_Glyph( glyph );

    *the_glyph = FT_GLYPH( bitmap );

  Exit:
    ਜੇ ( error && bitmap )
      FT_ਪੂਰਾ_Glyph( FT_GLYPH( bitmap ) );

    ਵਾਪਸ error;

  Bad:
    error = FT_THROW( Invalid_ਦਲੀਲ );
    ਜਾਓ Exit;
  }


  /* documentation is in ftglyph.h */

  FT_EXPORT_DEF( ਖਾਲੀ )
  FT_ਪੂਰਾ_Glyph( FT_Glyph  glyph )
  {
    ਜੇ ( glyph )
    {
      FT_ਸਮ੍ਰਿਤੀ              ਸਮ੍ਰਿਤੀ = glyph->library->ਸਮ੍ਰਿਤੀ;
      ਅਚਲ FT_Glyph_Class*  clazz  = glyph->clazz;


      ਜੇ ( clazz->glyph_ਪੂਰਾ )
        clazz->glyph_ਪੂਰਾ( glyph );

      FT_FREE( glyph );
    }
  }


/*ਸਿਰਾ*/
