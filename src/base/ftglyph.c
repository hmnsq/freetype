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


#ଅନ୍ତର୍ଭାଵ <freetype/internal/ftdebug.h>

#ଅନ୍ତର୍ଭାଵ <freetype/ftglyph.h>
#ଅନ୍ତର୍ଭାଵ <freetype/ftoutln.h>
#ଅନ୍ତର୍ଭାଵ <freetype/ftbitmap.h>
#ଅନ୍ତର୍ଭାଵ <freetype/internal/ftobjs.h>
#ଅନ୍ତର୍ଭାଵ <freetype/otsvg.h>

#ଅନ୍ତର୍ଭାଵ "ftbase.h"


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#undef  FT_COMPONENT
#define FT_COMPONENT  glyph


  /*************************************************************************/
  /*************************************************************************/
  /****                                                                 ****/
  /****   FT_BitmapGlyph support                                        ****/
  /****                                                                 ****/
  /*************************************************************************/
  /*************************************************************************/

  FT_CALLBACK_DEF( FT_Error )
  ft_bitmap_glyph_init( FT_Glyph      bitmap_glyph,
                        FT_GlyphSlot  slot )
  {
    FT_BitmapGlyph  glyph   = (FT_BitmapGlyph)bitmap_glyph;
    FT_Error        error;
    FT_Library      library = FT_GLYPH( glyph )->library;


    ଯଦି ( slot->format != FT_GLYPH_FORMAT_BITMAP )
    {
      error = FT_THROW( Invalid_Glyph_Format );
      ଯାଅ Exit;
    }

    glyph->ବାମ = slot->bitmap_ବାମ;
    glyph->ଶୀର୍ଷ  = slot->bitmap_ଶୀର୍ଷ;

    FT_Bitmap_Init( &glyph->bitmap );
    error = FT_Bitmap_Copy( library, &slot->bitmap, &glyph->bitmap );

  Exit:
    ଫେରନ୍ତୁ error;
  }


  FT_CALLBACK_DEF( FT_Error )
  ft_bitmap_glyph_copy( FT_Glyph  bitmap_source,
                        FT_Glyph  bitmap_target )
  {
    FT_Library      library = bitmap_source->library;
    FT_BitmapGlyph  source  = (FT_BitmapGlyph)bitmap_source;
    FT_BitmapGlyph  target  = (FT_BitmapGlyph)bitmap_target;


    target->ବାମ = source->ବାମ;
    target->ଶୀର୍ଷ  = source->ଶୀର୍ଷ;

    ଫେରନ୍ତୁ FT_Bitmap_Copy( library, &source->bitmap, &target->bitmap );
  }


  FT_CALLBACK_DEF( ଶୂନ୍ୟ )
  ft_bitmap_glyph_done( FT_Glyph  bitmap_glyph )
  {
    FT_BitmapGlyph  glyph   = (FT_BitmapGlyph)bitmap_glyph;
    FT_Library      library = FT_GLYPH( glyph )->library;


    FT_Bitmap_Done( library, &glyph->bitmap );
  }


  FT_CALLBACK_DEF( ଶୂନ୍ୟ )
  ft_bitmap_glyph_bbox( FT_Glyph  bitmap_glyph,
                        FT_BBox*  cbox )
  {
    FT_BitmapGlyph  glyph = (FT_BitmapGlyph)bitmap_glyph;


    cbox->xMin = glyph->ବାମ * 64;
    cbox->xMax = cbox->xMin + (FT_Pos)( glyph->bitmap.width * 64 );
    cbox->yMax = glyph->ଶୀର୍ଷ * 64;
    cbox->yMin = cbox->yMax - (FT_Pos)( glyph->bitmap.rows * 64 );
  }


  FT_DEFINE_GLYPH(
    ft_bitmap_glyph_class,

    ଆକାର ( FT_BitmapGlyphRec ),
    FT_GLYPH_FORMAT_BITMAP,

    ft_bitmap_glyph_init,    /* FT_Glyph_InitFunc       glyph_init      */
    ft_bitmap_glyph_done,    /* FT_Glyph_DoneFunc       glyph_done      */
    ft_bitmap_glyph_copy,    /* FT_Glyph_CopyFunc       glyph_copy      */
    ଖାଲି,                    /* FT_Glyph_TransformFunc  glyph_transform */
    ft_bitmap_glyph_bbox,    /* FT_Glyph_GetBBoxFunc    glyph_bbox      */
    ଖାଲି                     /* FT_Glyph_PrepareFunc    glyph_prepare   */
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
                         FT_GlyphSlot  slot )
  {
    FT_OutlineGlyph  glyph   = (FT_OutlineGlyph)outline_glyph;
    FT_Error         error   = FT_Err_Ok;
    FT_Library       library = FT_GLYPH( glyph )->library;
    FT_Outline*      source  = &slot->outline;
    FT_Outline*      target  = &glyph->outline;


    /* check format in glyph slot */
    ଯଦି ( slot->format != FT_GLYPH_FORMAT_OUTLINE )
    {
      error = FT_THROW( Invalid_Glyph_Format );
      ଯାଅ Exit;
    }

    /* allocate new outline */
    error = FT_Outline_New( library,
                            (FT_UInt)source->n_points,
                            source->n_contours,
                            &glyph->outline );
    ଯଦି ( error )
      ଯାଅ Exit;

    FT_Outline_Copy( source, target );

  Exit:
    ଫେରନ୍ତୁ error;
  }


  FT_CALLBACK_DEF( ଶୂନ୍ୟ )
  ft_outline_glyph_done( FT_Glyph  outline_glyph )
  {
    FT_OutlineGlyph  glyph = (FT_OutlineGlyph)outline_glyph;


    FT_Outline_Done( FT_GLYPH( glyph )->library, &glyph->outline );
  }


  FT_CALLBACK_DEF( FT_Error )
  ft_outline_glyph_copy( FT_Glyph  outline_source,
                         FT_Glyph  outline_target )
  {
    FT_OutlineGlyph  source  = (FT_OutlineGlyph)outline_source;
    FT_OutlineGlyph  target  = (FT_OutlineGlyph)outline_target;
    FT_Error         error;
    FT_Library       library = FT_GLYPH( source )->library;


    error = FT_Outline_New( library,
                            (FT_UInt)source->outline.n_points,
                            source->outline.n_contours,
                            &target->outline );
    ଯଦି ( !error )
      FT_Outline_Copy( &source->outline, &target->outline );

    ଫେରନ୍ତୁ error;
  }


  FT_CALLBACK_DEF( ଶୂନ୍ୟ )
  ft_outline_glyph_transform( FT_Glyph          outline_glyph,
                              ଅଚଳ FT_Matrix*  matrix,
                              ଅଚଳ FT_Vector*  delta )
  {
    FT_OutlineGlyph  glyph = (FT_OutlineGlyph)outline_glyph;


    ଯଦି ( matrix )
      FT_Outline_Transform( &glyph->outline, matrix );

    ଯଦି ( delta )
      FT_Outline_Translate( &glyph->outline, delta->x, delta->y );
  }


  FT_CALLBACK_DEF( ଶୂନ୍ୟ )
  ft_outline_glyph_bbox( FT_Glyph  outline_glyph,
                         FT_BBox*  bbox )
  {
    FT_OutlineGlyph  glyph = (FT_OutlineGlyph)outline_glyph;


    FT_Outline_Get_CBox( &glyph->outline, bbox );
  }


  FT_CALLBACK_DEF( FT_Error )
  ft_outline_glyph_prepare( FT_Glyph      outline_glyph,
                            FT_GlyphSlot  slot )
  {
    FT_OutlineGlyph  glyph = (FT_OutlineGlyph)outline_glyph;


    slot->format         = FT_GLYPH_FORMAT_OUTLINE;
    slot->outline        = glyph->outline;
    slot->outline.flags &= ~FT_OUTLINE_OWNER;

    ଫେରନ୍ତୁ FT_Err_Ok;
  }


  FT_DEFINE_GLYPH(
    ft_outline_glyph_class,

    ଆକାର ( FT_OutlineGlyphRec ),
    FT_GLYPH_FORMAT_OUTLINE,

    ft_outline_glyph_init,      /* FT_Glyph_InitFunc       glyph_init      */
    ft_outline_glyph_done,      /* FT_Glyph_DoneFunc       glyph_done      */
    ft_outline_glyph_copy,      /* FT_Glyph_CopyFunc       glyph_copy      */
    ft_outline_glyph_transform, /* FT_Glyph_TransformFunc  glyph_transform */
    ft_outline_glyph_bbox,      /* FT_Glyph_GetBBoxFunc    glyph_bbox      */
    ft_outline_glyph_prepare    /* FT_Glyph_PrepareFunc    glyph_prepare   */
  )


#ifdef FT_CONFIG_OPTION_SVG

  /*************************************************************************/
  /*************************************************************************/
  /****                                                                 ****/
  /****   FT_SvgGlyph support                                           ****/
  /****                                                                 ****/
  /*************************************************************************/
  /*************************************************************************/


  FT_CALLBACK_DEF( FT_Error )
  ft_svg_glyph_init( FT_Glyph      svg_glyph,
                     FT_GlyphSlot  slot )
  {
    FT_ULong         doc_length;
    FT_SVG_Document  document;
    FT_SvgGlyph      glyph = (FT_SvgGlyph)svg_glyph;

    FT_Error   error  = FT_Err_Ok;
    FT_Memory  memory = FT_GLYPH( glyph )->library->memory;


    ଯଦି ( slot->format != FT_GLYPH_FORMAT_SVG )
    {
      error = FT_THROW( Invalid_Glyph_Format );
      ଯାଅ Exit;
    }

    ଯଦି ( slot->other == ଖାଲି )
    {
      error = FT_THROW( Invalid_Slot_Handle );
      ଯାଅ Exit;
    }

    document = (FT_SVG_Document)slot->other;

    ଯଦି ( document->svg_document_length == 0 )
    {
      error = FT_THROW( Invalid_Slot_Handle );
      ଯାଅ Exit;
    }

    /* allocate a new document */
    doc_length = document->svg_document_length;
    ଯଦି ( FT_QALLOC( glyph->svg_document, doc_length ) )
      ଯାଅ Exit;
    glyph->svg_document_length = doc_length;

    glyph->glyph_index = slot->glyph_index;

    glyph->metrics      = document->metrics;
    glyph->units_per_EM = document->units_per_EM;

    glyph->start_glyph_id = document->start_glyph_id;
    glyph->end_glyph_id   = document->end_glyph_id;

    glyph->transform = document->transform;
    glyph->delta     = document->delta;

    /* copy the document into glyph */
    FT_MEM_COPY( glyph->svg_document, document->svg_document, doc_length );

  Exit:
    ଫେରନ୍ତୁ error;
  }


  FT_CALLBACK_DEF( ଶୂନ୍ୟ )
  ft_svg_glyph_done( FT_Glyph  svg_glyph )
  {
    FT_SvgGlyph  glyph  = (FT_SvgGlyph)svg_glyph;
    FT_Memory    memory = svg_glyph->library->memory;


    /* just free the memory */
    FT_FREE( glyph->svg_document );
  }


  FT_CALLBACK_DEF( FT_Error )
  ft_svg_glyph_copy( FT_Glyph  svg_source,
                     FT_Glyph  svg_target )
  {
    FT_SvgGlyph  source = (FT_SvgGlyph)svg_source;
    FT_SvgGlyph  target = (FT_SvgGlyph)svg_target;

    FT_Error   error  = FT_Err_Ok;
    FT_Memory  memory = FT_GLYPH( source )->library->memory;


    ଯଦି ( svg_source->format != FT_GLYPH_FORMAT_SVG )
    {
      error = FT_THROW( Invalid_Glyph_Format );
      ଯାଅ Exit;
    }

    ଯଦି ( source->svg_document_length == 0 )
    {
      error = FT_THROW( Invalid_Slot_Handle );
      ଯାଅ Exit;
    }

    target->glyph_index = source->glyph_index;

    target->svg_document_length = source->svg_document_length;

    target->metrics      = source->metrics;
    target->units_per_EM = source->units_per_EM;

    target->start_glyph_id = source->start_glyph_id;
    target->end_glyph_id   = source->end_glyph_id;

    target->transform = source->transform;
    target->delta     = source->delta;

    /* allocate space for the SVG document */
    ଯଦି ( FT_QALLOC( target->svg_document, target->svg_document_length ) )
      ଯାଅ Exit;

    /* copy the document */
    FT_MEM_COPY( target->svg_document,
                 source->svg_document,
                 target->svg_document_length );

  Exit:
    ଫେରନ୍ତୁ error;
  }


  FT_CALLBACK_DEF( ଶୂନ୍ୟ )
  ft_svg_glyph_transform( FT_Glyph          svg_glyph,
                          ଅଚଳ FT_Matrix*  _matrix,
                          ଅଚଳ FT_Vector*  _delta )
  {
    FT_SvgGlyph  glyph  = (FT_SvgGlyph)svg_glyph;
    FT_Matrix*   matrix = (FT_Matrix*)_matrix;
    FT_Vector*   delta  = (FT_Vector*)_delta;

    FT_Matrix  tmp_matrix;
    FT_Vector  tmp_delta;

    FT_Matrix  a, b;
    FT_Pos     x, y;


    ଯଦି ( !matrix )
    {
      tmp_matrix.xx = 0x10000;
      tmp_matrix.xy = 0;
      tmp_matrix.yx = 0;
      tmp_matrix.yy = 0x10000;

      matrix = &tmp_matrix;
    }

    ଯଦି ( !delta )
    {
      tmp_delta.x = 0;
      tmp_delta.y = 0;

      delta = &tmp_delta;
    }

    a = glyph->transform;
    b = *matrix;
    FT_Matrix_Multiply( &b, &a );

    x = ADD_LONG( ADD_LONG( FT_MulFix( matrix->xx, glyph->delta.x ),
                            FT_MulFix( matrix->xy, glyph->delta.y ) ),
                  delta->x );
    y = ADD_LONG( ADD_LONG( FT_MulFix( matrix->yx, glyph->delta.x ),
                            FT_MulFix( matrix->yy, glyph->delta.y ) ),
                  delta->y );

    glyph->delta.x = x;
    glyph->delta.y = y;

    glyph->transform = a;
  }


  FT_CALLBACK_DEF( FT_Error )
  ft_svg_glyph_prepare( FT_Glyph      svg_glyph,
                        FT_GlyphSlot  slot )
  {
    FT_SvgGlyph  glyph = (FT_SvgGlyph)svg_glyph;

    FT_Error   error  = FT_Err_Ok;
    FT_Memory  memory = svg_glyph->library->memory;

    FT_SVG_Document  document = ଖାଲି;


    ଯଦି ( FT_NEW( document ) )
      ଫେରନ୍ତୁ error;

    document->svg_document        = glyph->svg_document;
    document->svg_document_length = glyph->svg_document_length;

    document->metrics      = glyph->metrics;
    document->units_per_EM = glyph->units_per_EM;

    document->start_glyph_id = glyph->start_glyph_id;
    document->end_glyph_id   = glyph->end_glyph_id;

    document->transform = glyph->transform;
    document->delta     = glyph->delta;

    slot->format      = FT_GLYPH_FORMAT_SVG;
    slot->glyph_index = glyph->glyph_index;
    slot->other       = document;

    ଫେରନ୍ତୁ error;
  }


  FT_DEFINE_GLYPH(
    ft_svg_glyph_class,

    ଆକାର ( FT_SvgGlyphRec ),
    FT_GLYPH_FORMAT_SVG,

    ft_svg_glyph_init,      /* FT_Glyph_InitFunc       glyph_init      */
    ft_svg_glyph_done,      /* FT_Glyph_DoneFunc       glyph_done      */
    ft_svg_glyph_copy,      /* FT_Glyph_CopyFunc       glyph_copy      */
    ft_svg_glyph_transform, /* FT_Glyph_TransformFunc  glyph_transform */
    ଖାଲି,                   /* FT_Glyph_GetBBoxFunc    glyph_bbox      */
    ft_svg_glyph_prepare    /* FT_Glyph_PrepareFunc    glyph_prepare   */
  )

#endif /* FT_CONFIG_OPTION_SVG */


  /*************************************************************************/
  /*************************************************************************/
  /****                                                                 ****/
  /****   FT_Glyph class and API                                        ****/
  /****                                                                 ****/
  /*************************************************************************/
  /*************************************************************************/

   ସ୍ଥିର FT_Error
   ft_new_glyph( FT_Library             library,
                 ଅଚଳ FT_Glyph_Class*  clazz,
                 FT_Glyph*              aglyph )
   {
     FT_Memory  memory = library->memory;
     FT_Error   error;
     FT_Glyph   glyph  = ଖାଲି;


     *aglyph = ଖାଲି;

     ଯଦି ( !FT_ALLOC( glyph, clazz->glyph_size ) )
     {
       glyph->library = library;
       glyph->clazz   = clazz;
       glyph->format  = clazz->glyph_format;

       *aglyph = glyph;
     }

     ଫେରନ୍ତୁ error;
   }


  /* documentation is in ftglyph.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Glyph_Copy( FT_Glyph   source,
                 FT_Glyph  *target )
  {
    FT_Glyph               copy;
    FT_Error               error;
    ଅଚଳ FT_Glyph_Class*  clazz;


    /* check arguments */
    ଯଦି ( !target || !source || !source->clazz )
    {
      error = FT_THROW( Invalid_Argument );
      ଯାଅ Exit;
    }

    *target = ଖାଲି;

    ଯଦି ( !source || !source->clazz )
    {
      error = FT_THROW( Invalid_Argument );
      ଯାଅ Exit;
    }

    clazz = source->clazz;
    error = ft_new_glyph( source->library, clazz, &copy );
    ଯଦି ( error )
      ଯାଅ Exit;

    copy->advance = source->advance;
    copy->format  = source->format;

    ଯଦି ( clazz->glyph_copy )
      error = clazz->glyph_copy( source, copy );

    ଯଦି ( error )
      FT_Done_Glyph( copy );
    ଅନ୍ୟଥା
      *target = copy;

  Exit:
    ଫେରନ୍ତୁ error;
  }


  /* documentation is in ftglyph.h */

  FT_EXPORT( FT_Error )
  FT_New_Glyph( FT_Library       library,
                FT_Glyph_Format  format,
                FT_Glyph        *aglyph )
  {
    ଅଚଳ FT_Glyph_Class*  clazz = ଖାଲି;

    ଯଦି ( !library || !aglyph )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Argument );

    /* if it is a bitmap, that's easy :-) */
    ଯଦି ( format == FT_GLYPH_FORMAT_BITMAP )
      clazz = &ft_bitmap_glyph_class;

    /* if it is an outline */
    ଅନ୍ୟଥା ଯଦି ( format == FT_GLYPH_FORMAT_OUTLINE )
      clazz = &ft_outline_glyph_class;

#ifdef FT_CONFIG_OPTION_SVG
    /* if it is an SVG glyph */
    ଅନ୍ୟଥା ଯଦି ( format == FT_GLYPH_FORMAT_SVG )
      clazz = &ft_svg_glyph_class;
#endif

    ଅନ୍ୟଥା
    {
      /* try to find a renderer that supports the glyph image format */
      FT_Renderer  render = FT_Lookup_Renderer( library, format, 0 );


      ଯଦି ( render )
        clazz = &render->glyph_class;
    }

    ଯଦି ( !clazz )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Glyph_Format );

    /* create FT_Glyph object */
    ଫେରନ୍ତୁ ft_new_glyph( library, clazz, aglyph );
  }


  /* documentation is in ftglyph.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Glyph( FT_GlyphSlot  slot,
                FT_Glyph     *aglyph )
  {
    FT_Error  error;
    FT_Glyph  glyph;


    ଯଦି ( !slot )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Slot_Handle );

    ଯଦି ( !aglyph )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Argument );

    /* create FT_Glyph object */
    error = FT_New_Glyph( slot->library, slot->format, &glyph );
    ଯଦି ( error )
      ଯାଅ Exit;

    /* copy advance while converting 26.6 to 16.16 format */
    ଯଦି ( slot->advance.x >=  0x8000L * 64 ||
         slot->advance.x <= -0x8000L * 64 )
    {
      FT_ERROR(( "FT_Get_Glyph: advance width too large\n" ));
      error = FT_THROW( Invalid_Argument );
      ଯାଅ Exit2;
    }
    ଯଦି ( slot->advance.y >=  0x8000L * 64 ||
         slot->advance.y <= -0x8000L * 64 )
    {
      FT_ERROR(( "FT_Get_Glyph: advance height too large\n" ));
      error = FT_THROW( Invalid_Argument );
      ଯାଅ Exit2;
    }

    glyph->advance.x = slot->advance.x * 1024;
    glyph->advance.y = slot->advance.y * 1024;

    /* now import the image from the glyph slot */
    error = glyph->clazz->glyph_init( glyph, slot );

  Exit2:
    /* if an error occurred, destroy the glyph */
    ଯଦି ( error )
    {
      FT_Done_Glyph( glyph );
      *aglyph = ଖାଲି;
    }
    ଅନ୍ୟଥା
      *aglyph = glyph;

  Exit:
    ଫେରନ୍ତୁ error;
  }


  /* documentation is in ftglyph.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Glyph_Transform( FT_Glyph          glyph,
                      ଅଚଳ FT_Matrix*  matrix,
                      ଅଚଳ FT_Vector*  delta )
  {
    FT_Error  error = FT_Err_Ok;


    ଯଦି ( !glyph || !glyph->clazz )
      error = FT_THROW( Invalid_Argument );
    ଅନ୍ୟଥା
    {
      ଅଚଳ FT_Glyph_Class*  clazz = glyph->clazz;


      ଯଦି ( clazz->glyph_transform )
      {
        /* transform glyph image */
        clazz->glyph_transform( glyph, matrix, delta );

        /* transform advance vector */
        ଯଦି ( matrix )
          FT_Vector_Transform( &glyph->advance, matrix );
      }
      ଅନ୍ୟଥା
        error = FT_THROW( Invalid_Glyph_Format );
    }
    ଫେରନ୍ତୁ error;
  }


  /* documentation is in ftglyph.h */

  FT_EXPORT_DEF( ଶୂନ୍ୟ )
  FT_Glyph_Get_CBox( FT_Glyph  glyph,
                     FT_UInt   bbox_mode,
                     FT_BBox  *acbox )
  {
    ଅଚଳ FT_Glyph_Class*  clazz;


    ଯଦି ( !acbox )
      ଫେରନ୍ତୁ;

    acbox->xMin = acbox->yMin = acbox->xMax = acbox->yMax = 0;

    ଯଦି ( !glyph || !glyph->clazz )
      ଫେରନ୍ତୁ;

    clazz = glyph->clazz;
    ଯଦି ( !clazz->glyph_bbox )
      ଫେରନ୍ତୁ;

    /* retrieve bbox in 26.6 coordinates */
    clazz->glyph_bbox( glyph, acbox );

    /* perform grid fitting if needed */
    ଯଦି ( bbox_mode == FT_GLYPH_BBOX_GRIDFIT ||
         bbox_mode == FT_GLYPH_BBOX_PIXELS  )
    {
      acbox->xMin = FT_PIX_FLOOR( acbox->xMin );
      acbox->yMin = FT_PIX_FLOOR( acbox->yMin );
      acbox->xMax = FT_PIX_CEIL_LONG( acbox->xMax );
      acbox->yMax = FT_PIX_CEIL_LONG( acbox->yMax );
    }

    /* convert to integer pixels if needed */
    ଯଦି ( bbox_mode == FT_GLYPH_BBOX_TRUNCATE ||
         bbox_mode == FT_GLYPH_BBOX_PIXELS   )
    {
      acbox->xMin >>= 6;
      acbox->yMin >>= 6;
      acbox->xMax >>= 6;
      acbox->yMax >>= 6;
    }
  }


  /* documentation is in ftglyph.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Glyph_To_Bitmap( FT_Glyph*         the_glyph,
                      FT_Render_Mode    render_mode,
                      ଅଚଳ FT_Vector*  origin,
                      FT_Bool           destroy )
  {
    FT_GlyphSlotRec           dummy;
    FT_GlyphSlot_InternalRec  dummy_internal;
    FT_Error                  error = FT_Err_Ok;
    FT_Glyph                  b, glyph;
    FT_BitmapGlyph            bitmap = ଖାଲି;
    ଅଚଳ FT_Glyph_Class*     clazz;

    FT_Library                library;


    /* check argument */
    ଯଦି ( !the_glyph )
      ଯାଅ Bad;
    glyph = *the_glyph;
    ଯଦି ( !glyph )
      ଯାଅ Bad;

    clazz   = glyph->clazz;
    library = glyph->library;
    ଯଦି ( !library || !clazz )
      ଯାଅ Bad;

    /* when called with a bitmap glyph, do nothing and return successfully */
    ଯଦି ( clazz == &ft_bitmap_glyph_class )
      ଯାଅ Exit;

    ଯଦି ( !clazz->glyph_prepare )
      ଯାଅ Bad;

    /* we render the glyph into a glyph bitmap using a `dummy' glyph slot */
    /* then calling FT_Render_Glyph_Internal()                            */

    FT_ZERO( &dummy );
    FT_ZERO( &dummy_internal );
    dummy.internal = &dummy_internal;
    dummy.library  = library;
    dummy.format   = clazz->glyph_format;

    /* create result bitmap glyph */
    error = ft_new_glyph( library, &ft_bitmap_glyph_class, &b );
    ଯଦି ( error )
      ଯାଅ Exit;
    bitmap = (FT_BitmapGlyph)b;

#ଯଦି 1
    /* if `origin' is set, translate the glyph image */
    ଯଦି ( origin )
      FT_Glyph_Transform( glyph, ଖାଲି, origin );
#ଅନ୍ୟଥା
    FT_UNUSED( origin );
#endif

    /* prepare dummy slot for rendering */
    error = clazz->glyph_prepare( glyph, &dummy );
    ଯଦି ( !error )
      error = FT_Render_Glyph_Internal( glyph->library, &dummy, render_mode );

#ifdef FT_CONFIG_OPTION_SVG
    ଯଦି ( clazz == &ft_svg_glyph_class )
    {
      FT_Memory  memory = library->memory;


      FT_FREE( dummy.other );
    }
#endif

#ଯଦି 1
    ଯଦି ( !destroy && origin )
    {
      FT_Vector  v;


      v.x = -origin->x;
      v.y = -origin->y;
      FT_Glyph_Transform( glyph, ଖାଲି, &v );
    }
#endif

    ଯଦି ( error )
      ଯାଅ Exit;

    /* in case of success, copy the bitmap to the glyph bitmap */
    bitmap->ବାମ   = dummy.bitmap_ବାମ;
    bitmap->ଶୀର୍ଷ    = dummy.bitmap_ଶୀର୍ଷ;
    bitmap->bitmap = dummy.bitmap;

    /* copy advance */
    bitmap->ମୂଳ.advance = glyph->advance;

    ଯଦି ( destroy )
      FT_Done_Glyph( glyph );

    *the_glyph = FT_GLYPH( bitmap );

  Exit:
    ଯଦି ( error && bitmap )
      FT_Done_Glyph( FT_GLYPH( bitmap ) );

    ଫେରନ୍ତୁ error;

  Bad:
    error = FT_THROW( Invalid_Argument );
    ଯାଅ Exit;
  }


  /* documentation is in ftglyph.h */

  FT_EXPORT_DEF( ଶୂନ୍ୟ )
  FT_Done_Glyph( FT_Glyph  glyph )
  {
    ଯଦି ( glyph )
    {
      FT_Memory              memory = glyph->library->memory;
      ଅଚଳ FT_Glyph_Class*  clazz  = glyph->clazz;


      ଯଦି ( clazz->glyph_done )
        clazz->glyph_done( glyph );

      FT_FREE( glyph );
    }
  }


/* END */
