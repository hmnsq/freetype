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


#समावेश <freetype/internal/ftdebug.h>

#समावेश <freetype/ftglyph.h>
#समावेश <freetype/ftoutln.h>
#समावेश <freetype/ftbitmap.h>
#समावेश <freetype/internal/ftobjs.h>
#समावेश <freetype/otsvg.h>

#समावेश "ftbase.h"


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#अघोषित  FT_COMPONENT
#घोषणा FT_COMPONENT  glyph


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


    अगर ( slot->format != FT_GLYPH_FORMAT_BITMAP )
    {
      error = FT_THROW( Invalid_Glyph_Format );
      जाओ Exit;
    }

    glyph->बायाँ = slot->bitmap_बायाँ;
    glyph->शीर्ष  = slot->bitmap_शीर्ष;

    FT_Bitmap_Init( &glyph->bitmap );
    error = FT_Bitmap_Copy( library, &slot->bitmap, &glyph->bitmap );

  Exit:
    वापस error;
  }


  FT_CALLBACK_DEF( FT_Error )
  ft_bitmap_glyph_copy( FT_Glyph  bitmap_source,
                        FT_Glyph  bitmap_target )
  {
    FT_Library      library = bitmap_source->library;
    FT_BitmapGlyph  source  = (FT_BitmapGlyph)bitmap_source;
    FT_BitmapGlyph  target  = (FT_BitmapGlyph)bitmap_target;


    target->बायाँ = source->बायाँ;
    target->शीर्ष  = source->शीर्ष;

    वापस FT_Bitmap_Copy( library, &source->bitmap, &target->bitmap );
  }


  FT_CALLBACK_DEF( व्योम )
  ft_bitmap_glyph_done( FT_Glyph  bitmap_glyph )
  {
    FT_BitmapGlyph  glyph   = (FT_BitmapGlyph)bitmap_glyph;
    FT_Library      library = FT_GLYPH( glyph )->library;


    FT_Bitmap_Done( library, &glyph->bitmap );
  }


  FT_CALLBACK_DEF( व्योम )
  ft_bitmap_glyph_bbox( FT_Glyph  bitmap_glyph,
                        FT_BBox*  cbox )
  {
    FT_BitmapGlyph  glyph = (FT_BitmapGlyph)bitmap_glyph;


    cbox->xMin = glyph->बायाँ * 64;
    cbox->xMax = cbox->xMin + (FT_Pos)( glyph->bitmap.width * 64 );
    cbox->yMax = glyph->शीर्ष * 64;
    cbox->yMin = cbox->yMax - (FT_Pos)( glyph->bitmap.rows * 64 );
  }


  FT_DEFINE_GLYPH(
    ft_bitmap_glyph_class,

    माप ( FT_BitmapGlyphRec ),
    FT_GLYPH_FORMAT_BITMAP,

    ft_bitmap_glyph_init,    /* FT_Glyph_InitFunc       glyph_init      */
    ft_bitmap_glyph_done,    /* FT_Glyph_DoneFunc       glyph_done      */
    ft_bitmap_glyph_copy,    /* FT_Glyph_CopyFunc       glyph_copy      */
    शून्य,                    /* FT_Glyph_TransformFunc  glyph_transform */
    ft_bitmap_glyph_bbox,    /* FT_Glyph_GetBBoxFunc    glyph_bbox      */
    शून्य                     /* FT_Glyph_PrepareFunc    glyph_prepare   */
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
    अगर ( slot->format != FT_GLYPH_FORMAT_OUTLINE )
    {
      error = FT_THROW( Invalid_Glyph_Format );
      जाओ Exit;
    }

    /* allocate new outline */
    error = FT_Outline_New( library,
                            (FT_UInt)source->n_points,
                            source->n_contours,
                            &glyph->outline );
    अगर ( error )
      जाओ Exit;

    FT_Outline_Copy( source, target );

  Exit:
    वापस error;
  }


  FT_CALLBACK_DEF( व्योम )
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
    अगर ( !error )
      FT_Outline_Copy( &source->outline, &target->outline );

    वापस error;
  }


  FT_CALLBACK_DEF( व्योम )
  ft_outline_glyph_transform( FT_Glyph          outline_glyph,
                              स्थिर FT_Matrix*  matrix,
                              स्थिर FT_Vector*  delta )
  {
    FT_OutlineGlyph  glyph = (FT_OutlineGlyph)outline_glyph;


    अगर ( matrix )
      FT_Outline_Transform( &glyph->outline, matrix );

    अगर ( delta )
      FT_Outline_Translate( &glyph->outline, delta->x, delta->y );
  }


  FT_CALLBACK_DEF( व्योम )
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

    वापस FT_Err_Ok;
  }


  FT_DEFINE_GLYPH(
    ft_outline_glyph_class,

    माप ( FT_OutlineGlyphRec ),
    FT_GLYPH_FORMAT_OUTLINE,

    ft_outline_glyph_init,      /* FT_Glyph_InitFunc       glyph_init      */
    ft_outline_glyph_done,      /* FT_Glyph_DoneFunc       glyph_done      */
    ft_outline_glyph_copy,      /* FT_Glyph_CopyFunc       glyph_copy      */
    ft_outline_glyph_transform, /* FT_Glyph_TransformFunc  glyph_transform */
    ft_outline_glyph_bbox,      /* FT_Glyph_GetBBoxFunc    glyph_bbox      */
    ft_outline_glyph_prepare    /* FT_Glyph_PrepareFunc    glyph_prepare   */
  )


#अगर_घोषित FT_CONFIG_OPTION_SVG

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
    FT_ULong         doc_लंबाई;
    FT_SVG_Document  document;
    FT_SvgGlyph      glyph = (FT_SvgGlyph)svg_glyph;

    FT_Error   error  = FT_Err_Ok;
    FT_Memory  memory = FT_GLYPH( glyph )->library->memory;


    अगर ( slot->format != FT_GLYPH_FORMAT_SVG )
    {
      error = FT_THROW( Invalid_Glyph_Format );
      जाओ Exit;
    }

    अगर ( slot->other == शून्य )
    {
      error = FT_THROW( Invalid_Slot_Handle );
      जाओ Exit;
    }

    document = (FT_SVG_Document)slot->other;

    अगर ( document->svg_document_लंबाई == 0 )
    {
      error = FT_THROW( Invalid_Slot_Handle );
      जाओ Exit;
    }

    /* allocate a new document */
    doc_लंबाई = document->svg_document_लंबाई;
    अगर ( FT_QALLOC( glyph->svg_document, doc_लंबाई ) )
      जाओ Exit;
    glyph->svg_document_लंबाई = doc_लंबाई;

    glyph->glyph_सूचकांक = slot->glyph_सूचकांक;

    glyph->metrics      = document->metrics;
    glyph->units_per_EM = document->units_per_EM;

    glyph->start_glyph_id = document->start_glyph_id;
    glyph->end_glyph_id   = document->end_glyph_id;

    glyph->transform = document->transform;
    glyph->delta     = document->delta;

    /* copy the document into glyph */
    FT_MEM_COPY( glyph->svg_document, document->svg_document, doc_लंबाई );

  Exit:
    वापस error;
  }


  FT_CALLBACK_DEF( व्योम )
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


    अगर ( svg_source->format != FT_GLYPH_FORMAT_SVG )
    {
      error = FT_THROW( Invalid_Glyph_Format );
      जाओ Exit;
    }

    अगर ( source->svg_document_लंबाई == 0 )
    {
      error = FT_THROW( Invalid_Slot_Handle );
      जाओ Exit;
    }

    target->glyph_सूचकांक = source->glyph_सूचकांक;

    target->svg_document_लंबाई = source->svg_document_लंबाई;

    target->metrics      = source->metrics;
    target->units_per_EM = source->units_per_EM;

    target->start_glyph_id = source->start_glyph_id;
    target->end_glyph_id   = source->end_glyph_id;

    target->transform = source->transform;
    target->delta     = source->delta;

    /* allocate space for the SVG document */
    अगर ( FT_QALLOC( target->svg_document, target->svg_document_लंबाई ) )
      जाओ Exit;

    /* copy the document */
    FT_MEM_COPY( target->svg_document,
                 source->svg_document,
                 target->svg_document_लंबाई );

  Exit:
    वापस error;
  }


  FT_CALLBACK_DEF( व्योम )
  ft_svg_glyph_transform( FT_Glyph          svg_glyph,
                          स्थिर FT_Matrix*  _matrix,
                          स्थिर FT_Vector*  _delta )
  {
    FT_SvgGlyph  glyph  = (FT_SvgGlyph)svg_glyph;
    FT_Matrix*   matrix = (FT_Matrix*)_matrix;
    FT_Vector*   delta  = (FT_Vector*)_delta;

    FT_Matrix  tmp_matrix;
    FT_Vector  tmp_delta;

    FT_Matrix  a, b;
    FT_Pos     x, y;


    अगर ( !matrix )
    {
      tmp_matrix.xx = 0x10000;
      tmp_matrix.xy = 0;
      tmp_matrix.yx = 0;
      tmp_matrix.yy = 0x10000;

      matrix = &tmp_matrix;
    }

    अगर ( !delta )
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

    FT_SVG_Document  document = शून्य;


    अगर ( FT_NEW( document ) )
      वापस error;

    document->svg_document        = glyph->svg_document;
    document->svg_document_लंबाई = glyph->svg_document_लंबाई;

    document->metrics      = glyph->metrics;
    document->units_per_EM = glyph->units_per_EM;

    document->start_glyph_id = glyph->start_glyph_id;
    document->end_glyph_id   = glyph->end_glyph_id;

    document->transform = glyph->transform;
    document->delta     = glyph->delta;

    slot->format      = FT_GLYPH_FORMAT_SVG;
    slot->glyph_सूचकांक = glyph->glyph_सूचकांक;
    slot->other       = document;

    वापस error;
  }


  FT_DEFINE_GLYPH(
    ft_svg_glyph_class,

    माप ( FT_SvgGlyphRec ),
    FT_GLYPH_FORMAT_SVG,

    ft_svg_glyph_init,      /* FT_Glyph_InitFunc       glyph_init      */
    ft_svg_glyph_done,      /* FT_Glyph_DoneFunc       glyph_done      */
    ft_svg_glyph_copy,      /* FT_Glyph_CopyFunc       glyph_copy      */
    ft_svg_glyph_transform, /* FT_Glyph_TransformFunc  glyph_transform */
    शून्य,                   /* FT_Glyph_GetBBoxFunc    glyph_bbox      */
    ft_svg_glyph_prepare    /* FT_Glyph_PrepareFunc    glyph_prepare   */
  )

#पूर्ण_अगर /* FT_CONFIG_OPTION_SVG */


  /*************************************************************************/
  /*************************************************************************/
  /****                                                                 ****/
  /****   FT_Glyph class and API                                        ****/
  /****                                                                 ****/
  /*************************************************************************/
  /*************************************************************************/

   अटल FT_Error
   ft_new_glyph( FT_Library             library,
                 स्थिर FT_Glyph_Class*  clazz,
                 FT_Glyph*              aglyph )
   {
     FT_Memory  memory = library->memory;
     FT_Error   error;
     FT_Glyph   glyph  = शून्य;


     *aglyph = शून्य;

     अगर ( !FT_ALLOC( glyph, clazz->glyph_आकार ) )
     {
       glyph->library = library;
       glyph->clazz   = clazz;
       glyph->format  = clazz->glyph_format;

       *aglyph = glyph;
     }

     वापस error;
   }


  /* documentation is in ftglyph.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Glyph_Copy( FT_Glyph   source,
                 FT_Glyph  *target )
  {
    FT_Glyph               copy;
    FT_Error               error;
    स्थिर FT_Glyph_Class*  clazz;


    /* check arguments */
    अगर ( !target || !source || !source->clazz )
    {
      error = FT_THROW( Invalid_Argument );
      जाओ Exit;
    }

    *target = शून्य;

    अगर ( !source || !source->clazz )
    {
      error = FT_THROW( Invalid_Argument );
      जाओ Exit;
    }

    clazz = source->clazz;
    error = ft_new_glyph( source->library, clazz, &copy );
    अगर ( error )
      जाओ Exit;

    copy->advance = source->advance;
    copy->format  = source->format;

    अगर ( clazz->glyph_copy )
      error = clazz->glyph_copy( source, copy );

    अगर ( error )
      FT_Done_Glyph( copy );
    अन्यथा
      *target = copy;

  Exit:
    वापस error;
  }


  /* documentation is in ftglyph.h */

  FT_EXPORT( FT_Error )
  FT_New_Glyph( FT_Library       library,
                FT_Glyph_Format  format,
                FT_Glyph        *aglyph )
  {
    स्थिर FT_Glyph_Class*  clazz = शून्य;

    अगर ( !library || !aglyph )
      वापस FT_THROW( Invalid_Argument );

    /* if it is a bitmap, that's easy :-) */
    अगर ( format == FT_GLYPH_FORMAT_BITMAP )
      clazz = &ft_bitmap_glyph_class;

    /* if it is an outline */
    अन्यथा अगर ( format == FT_GLYPH_FORMAT_OUTLINE )
      clazz = &ft_outline_glyph_class;

#अगर_घोषित FT_CONFIG_OPTION_SVG
    /* if it is an SVG glyph */
    अन्यथा अगर ( format == FT_GLYPH_FORMAT_SVG )
      clazz = &ft_svg_glyph_class;
#पूर्ण_अगर

    अन्यथा
    {
      /* try to find a renderer that supports the glyph image format */
      FT_Renderer  render = FT_Lookup_Renderer( library, format, 0 );


      अगर ( render )
        clazz = &render->glyph_class;
    }

    अगर ( !clazz )
      वापस FT_THROW( Invalid_Glyph_Format );

    /* create FT_Glyph object */
    वापस ft_new_glyph( library, clazz, aglyph );
  }


  /* documentation is in ftglyph.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Glyph( FT_GlyphSlot  slot,
                FT_Glyph     *aglyph )
  {
    FT_Error  error;
    FT_Glyph  glyph;


    अगर ( !slot )
      वापस FT_THROW( Invalid_Slot_Handle );

    अगर ( !aglyph )
      वापस FT_THROW( Invalid_Argument );

    /* create FT_Glyph object */
    error = FT_New_Glyph( slot->library, slot->format, &glyph );
    अगर ( error )
      जाओ Exit;

    /* copy advance while converting 26.6 to 16.16 format */
    अगर ( slot->advance.x >=  0x8000L * 64 ||
         slot->advance.x <= -0x8000L * 64 )
    {
      FT_ERROR(( "FT_Get_Glyph: advance width too large\n" ));
      error = FT_THROW( Invalid_Argument );
      जाओ Exit2;
    }
    अगर ( slot->advance.y >=  0x8000L * 64 ||
         slot->advance.y <= -0x8000L * 64 )
    {
      FT_ERROR(( "FT_Get_Glyph: advance height too large\n" ));
      error = FT_THROW( Invalid_Argument );
      जाओ Exit2;
    }

    glyph->advance.x = slot->advance.x * 1024;
    glyph->advance.y = slot->advance.y * 1024;

    /* now import the image from the glyph slot */
    error = glyph->clazz->glyph_init( glyph, slot );

  Exit2:
    /* if an error occurred, destroy the glyph */
    अगर ( error )
    {
      FT_Done_Glyph( glyph );
      *aglyph = शून्य;
    }
    अन्यथा
      *aglyph = glyph;

  Exit:
    वापस error;
  }


  /* documentation is in ftglyph.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Glyph_Transform( FT_Glyph          glyph,
                      स्थिर FT_Matrix*  matrix,
                      स्थिर FT_Vector*  delta )
  {
    FT_Error  error = FT_Err_Ok;


    अगर ( !glyph || !glyph->clazz )
      error = FT_THROW( Invalid_Argument );
    अन्यथा
    {
      स्थिर FT_Glyph_Class*  clazz = glyph->clazz;


      अगर ( clazz->glyph_transform )
      {
        /* transform glyph image */
        clazz->glyph_transform( glyph, matrix, delta );

        /* transform advance vector */
        अगर ( matrix )
          FT_Vector_Transform( &glyph->advance, matrix );
      }
      अन्यथा
        error = FT_THROW( Invalid_Glyph_Format );
    }
    वापस error;
  }


  /* documentation is in ftglyph.h */

  FT_EXPORT_DEF( व्योम )
  FT_Glyph_Get_CBox( FT_Glyph  glyph,
                     FT_UInt   bbox_mode,
                     FT_BBox  *acbox )
  {
    स्थिर FT_Glyph_Class*  clazz;


    अगर ( !acbox )
      वापस;

    acbox->xMin = acbox->yMin = acbox->xMax = acbox->yMax = 0;

    अगर ( !glyph || !glyph->clazz )
      वापस;

    clazz = glyph->clazz;
    अगर ( !clazz->glyph_bbox )
      वापस;

    /* retrieve bbox in 26.6 coordinates */
    clazz->glyph_bbox( glyph, acbox );

    /* perform grid fitting if needed */
    अगर ( bbox_mode == FT_GLYPH_BBOX_GRIDFIT ||
         bbox_mode == FT_GLYPH_BBOX_PIXELS  )
    {
      acbox->xMin = FT_PIX_FLOOR( acbox->xMin );
      acbox->yMin = FT_PIX_FLOOR( acbox->yMin );
      acbox->xMax = FT_PIX_CEIL_LONG( acbox->xMax );
      acbox->yMax = FT_PIX_CEIL_LONG( acbox->yMax );
    }

    /* convert to integer pixels if needed */
    अगर ( bbox_mode == FT_GLYPH_BBOX_TRUNCATE ||
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
                      स्थिर FT_Vector*  origin,
                      FT_Bool           destroy )
  {
    FT_GlyphSlotRec           dummy;
    FT_GlyphSlot_InternalRec  dummy_internal;
    FT_Error                  error = FT_Err_Ok;
    FT_Glyph                  b, glyph;
    FT_BitmapGlyph            bitmap = शून्य;
    स्थिर FT_Glyph_Class*     clazz;

    FT_Library                library;


    /* check argument */
    अगर ( !the_glyph )
      जाओ Bad;
    glyph = *the_glyph;
    अगर ( !glyph )
      जाओ Bad;

    clazz   = glyph->clazz;
    library = glyph->library;
    अगर ( !library || !clazz )
      जाओ Bad;

    /* when called with a bitmap glyph, do nothing and return successfully */
    अगर ( clazz == &ft_bitmap_glyph_class )
      जाओ Exit;

    अगर ( !clazz->glyph_prepare )
      जाओ Bad;

    /* we render the glyph into a glyph bitmap using a `dummy' glyph slot */
    /* then calling FT_Render_Glyph_Internal()                            */

    FT_ZERO( &dummy );
    FT_ZERO( &dummy_internal );
    dummy.internal = &dummy_internal;
    dummy.library  = library;
    dummy.format   = clazz->glyph_format;

    /* create result bitmap glyph */
    error = ft_new_glyph( library, &ft_bitmap_glyph_class, &b );
    अगर ( error )
      जाओ Exit;
    bitmap = (FT_BitmapGlyph)b;

#अगर 1
    /* if `origin' is set, translate the glyph image */
    अगर ( origin )
      FT_Glyph_Transform( glyph, शून्य, origin );
#अन्यथा
    FT_UNUSED( origin );
#पूर्ण_अगर

    /* prepare dummy slot for rendering */
    error = clazz->glyph_prepare( glyph, &dummy );
    अगर ( !error )
      error = FT_Render_Glyph_Internal( glyph->library, &dummy, render_mode );

#अगर_घोषित FT_CONFIG_OPTION_SVG
    अगर ( clazz == &ft_svg_glyph_class )
    {
      FT_Memory  memory = library->memory;


      FT_FREE( dummy.other );
    }
#पूर्ण_अगर

#अगर 1
    अगर ( !destroy && origin )
    {
      FT_Vector  v;


      v.x = -origin->x;
      v.y = -origin->y;
      FT_Glyph_Transform( glyph, शून्य, &v );
    }
#पूर्ण_अगर

    अगर ( error )
      जाओ Exit;

    /* in case of success, copy the bitmap to the glyph bitmap */
    bitmap->बायाँ   = dummy.bitmap_बायाँ;
    bitmap->शीर्ष    = dummy.bitmap_शीर्ष;
    bitmap->bitmap = dummy.bitmap;

    /* copy advance */
    bitmap->मूल.advance = glyph->advance;

    अगर ( destroy )
      FT_Done_Glyph( glyph );

    *the_glyph = FT_GLYPH( bitmap );

  Exit:
    अगर ( error && bitmap )
      FT_Done_Glyph( FT_GLYPH( bitmap ) );

    वापस error;

  Bad:
    error = FT_THROW( Invalid_Argument );
    जाओ Exit;
  }


  /* documentation is in ftglyph.h */

  FT_EXPORT_DEF( व्योम )
  FT_Done_Glyph( FT_Glyph  glyph )
  {
    अगर ( glyph )
    {
      FT_Memory              memory = glyph->library->memory;
      स्थिर FT_Glyph_Class*  clazz  = glyph->clazz;


      अगर ( clazz->glyph_done )
        clazz->glyph_done( glyph );

      FT_FREE( glyph );
    }
  }


/* END */
