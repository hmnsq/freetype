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


#شامل <freetype/internal/ftdebug.h>

#شامل <freetype/ftglyph.h>
#شامل <freetype/ftoutln.h>
#شامل <freetype/ftbitmap.h>
#شامل <freetype/internal/ftobjs.h>
#شامل <freetype/otsvg.h>

#شامل "ftbase.h"


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#تعریف_منسوخ  FT_COMPONENT
#تعریف_کرو FT_COMPONENT  glyph


  /*************************************************************************/
  /*************************************************************************/
  /****                                                                 ****/
  /****   FT_BitmapGlyph support                                        ****/
  /****                                                                 ****/
  /*************************************************************************/
  /*************************************************************************/

  FT_CALLBACK_DEF( FT_Error )
  ft_bitmap_glyph_init( FT_Glyph      bitmap_glyph,
                        FT_Glyph_خانہ  خانہ )
  {
    FT_BitmapGlyph  glyph   = (FT_BitmapGlyph)bitmap_glyph;
    FT_Error        error;
    FT_Library      library = FT_GLYPH( glyph )->library;


    اگر ( خانہ->format != FT_GLYPH_FORMAT_BITMAP )
    {
      error = FT_THROW( Invalid_Glyph_Format );
      جاؤ Exit;
    }

    glyph->بایاں = خانہ->bitmap_بایاں;
    glyph->چوٹی  = خانہ->bitmap_چوٹی;

    FT_Bitmap_Init( &glyph->bitmap );
    error = FT_Bitmap_Copy( library, &خانہ->bitmap, &glyph->bitmap );

  Exit:
    واپس error;
  }


  FT_CALLBACK_DEF( FT_Error )
  ft_bitmap_glyph_copy( FT_Glyph  bitmap_ماخذ,
                        FT_Glyph  bitmap_ہدف )
  {
    FT_Library      library = bitmap_ماخذ->library;
    FT_BitmapGlyph  ماخذ  = (FT_BitmapGlyph)bitmap_ماخذ;
    FT_BitmapGlyph  ہدف  = (FT_BitmapGlyph)bitmap_ہدف;


    ہدف->بایاں = ماخذ->بایاں;
    ہدف->چوٹی  = ماخذ->چوٹی;

    واپس FT_Bitmap_Copy( library, &ماخذ->bitmap, &ہدف->bitmap );
  }


  FT_CALLBACK_DEF( خالی )
  ft_bitmap_glyph_مکمل( FT_Glyph  bitmap_glyph )
  {
    FT_BitmapGlyph  glyph   = (FT_BitmapGlyph)bitmap_glyph;
    FT_Library      library = FT_GLYPH( glyph )->library;


    FT_Bitmap_مکمل( library, &glyph->bitmap );
  }


  FT_CALLBACK_DEF( خالی )
  ft_bitmap_glyph_bbox( FT_Glyph  bitmap_glyph,
                        FT_BBox*  cbox )
  {
    FT_BitmapGlyph  glyph = (FT_BitmapGlyph)bitmap_glyph;


    cbox->xMin = glyph->بایاں * 64;
    cbox->xMax = cbox->xMin + (FT_Pos)( glyph->bitmap.چوڑائی * 64 );
    cbox->yMax = glyph->چوٹی * 64;
    cbox->yMin = cbox->yMax - (FT_Pos)( glyph->bitmap.صفیں * 64 );
  }


  FT_DEFINE_GLYPH(
    ft_bitmap_glyph_class,

    حجم ( FT_BitmapGlyphRec ),
    FT_GLYPH_FORMAT_BITMAP,

    ft_bitmap_glyph_init,    /* FT_Glyph_InitFunc       glyph_init      */
    ft_bitmap_glyph_مکمل,    /* FT_Glyph_DoneFunc       glyph_done      */
    ft_bitmap_glyph_copy,    /* FT_Glyph_CopyFunc       glyph_copy      */
    تہی,                    /* FT_Glyph_TransformFunc  glyph_transform */
    ft_bitmap_glyph_bbox,    /* FT_Glyph_GetBBoxFunc    glyph_bbox      */
    تہی                     /* FT_Glyph_PrepareFunc    glyph_prepare   */
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
                         FT_Glyph_خانہ  خانہ )
  {
    FT_OutlineGlyph  glyph   = (FT_OutlineGlyph)outline_glyph;
    FT_Error         error   = FT_Err_Ok;
    FT_Library       library = FT_GLYPH( glyph )->library;
    FT_Outline*      ماخذ  = &خانہ->outline;
    FT_Outline*      ہدف  = &glyph->outline;


    /* check format in glyph slot */
    اگر ( خانہ->format != FT_GLYPH_FORMAT_OUTLINE )
    {
      error = FT_THROW( Invalid_Glyph_Format );
      جاؤ Exit;
    }

    /* allocate new outline */
    error = FT_Outline_New( library,
                            (FT_UInt)ماخذ->n_points,
                            ماخذ->n_contours,
                            &glyph->outline );
    اگر ( error )
      جاؤ Exit;

    FT_Outline_Copy( ماخذ, ہدف );

  Exit:
    واپس error;
  }


  FT_CALLBACK_DEF( خالی )
  ft_outline_glyph_مکمل( FT_Glyph  outline_glyph )
  {
    FT_OutlineGlyph  glyph = (FT_OutlineGlyph)outline_glyph;


    FT_Outline_مکمل( FT_GLYPH( glyph )->library, &glyph->outline );
  }


  FT_CALLBACK_DEF( FT_Error )
  ft_outline_glyph_copy( FT_Glyph  outline_ماخذ,
                         FT_Glyph  outline_ہدف )
  {
    FT_OutlineGlyph  ماخذ  = (FT_OutlineGlyph)outline_ماخذ;
    FT_OutlineGlyph  ہدف  = (FT_OutlineGlyph)outline_ہدف;
    FT_Error         error;
    FT_Library       library = FT_GLYPH( ماخذ )->library;


    error = FT_Outline_New( library,
                            (FT_UInt)ماخذ->outline.n_points,
                            ماخذ->outline.n_contours,
                            &ہدف->outline );
    اگر ( !error )
      FT_Outline_Copy( &ماخذ->outline, &ہدف->outline );

    واپس error;
  }


  FT_CALLBACK_DEF( خالی )
  ft_outline_glyph_transform( FT_Glyph          outline_glyph,
                              ثابت FT_میٹرکس*  میٹرکس,
                              ثابت FT_سمتیہ*  delta )
  {
    FT_OutlineGlyph  glyph = (FT_OutlineGlyph)outline_glyph;


    اگر ( میٹرکس )
      FT_Outline_Transform( &glyph->outline, میٹرکس );

    اگر ( delta )
      FT_Outline_Translate( &glyph->outline, delta->x, delta->y );
  }


  FT_CALLBACK_DEF( خالی )
  ft_outline_glyph_bbox( FT_Glyph  outline_glyph,
                         FT_BBox*  bbox )
  {
    FT_OutlineGlyph  glyph = (FT_OutlineGlyph)outline_glyph;


    FT_Outline_Get_CBox( &glyph->outline, bbox );
  }


  FT_CALLBACK_DEF( FT_Error )
  ft_outline_glyph_prepare( FT_Glyph      outline_glyph,
                            FT_Glyph_خانہ  خانہ )
  {
    FT_OutlineGlyph  glyph = (FT_OutlineGlyph)outline_glyph;


    خانہ->format         = FT_GLYPH_FORMAT_OUTLINE;
    خانہ->outline        = glyph->outline;
    خانہ->outline.flags &= ~FT_OUTLINE_OWNER;

    واپس FT_Err_Ok;
  }


  FT_DEFINE_GLYPH(
    ft_outline_glyph_class,

    حجم ( FT_OutlineGlyphRec ),
    FT_GLYPH_FORMAT_OUTLINE,

    ft_outline_glyph_init,      /* FT_Glyph_InitFunc       glyph_init      */
    ft_outline_glyph_مکمل,      /* FT_Glyph_DoneFunc       glyph_done      */
    ft_outline_glyph_copy,      /* FT_Glyph_CopyFunc       glyph_copy      */
    ft_outline_glyph_transform, /* FT_Glyph_TransformFunc  glyph_transform */
    ft_outline_glyph_bbox,      /* FT_Glyph_GetBBoxFunc    glyph_bbox      */
    ft_outline_glyph_prepare    /* FT_Glyph_PrepareFunc    glyph_prepare   */
  )


#اگر_تعریف_شدہ FT_CONFIG_OPTION_SVG

  /*************************************************************************/
  /*************************************************************************/
  /****                                                                 ****/
  /****   FT_SvgGlyph support                                           ****/
  /****                                                                 ****/
  /*************************************************************************/
  /*************************************************************************/


  FT_CALLBACK_DEF( FT_Error )
  ft_svg_glyph_init( FT_Glyph      svg_glyph,
                     FT_Glyph_خانہ  خانہ )
  {
    FT_ULong         doc_لمبائی;
    FT_SVG_Document  document;
    FT_SvgGlyph      glyph = (FT_SvgGlyph)svg_glyph;

    FT_Error   error  = FT_Err_Ok;
    FT_حافظہ  حافظہ = FT_GLYPH( glyph )->library->حافظہ;


    اگر ( خانہ->format != FT_GLYPH_FORMAT_SVG )
    {
      error = FT_THROW( Invalid_Glyph_Format );
      جاؤ Exit;
    }

    اگر ( خانہ->other == تہی )
    {
      error = FT_THROW( Invalid_خانہ_Handle );
      جاؤ Exit;
    }

    document = (FT_SVG_Document)خانہ->other;

    اگر ( document->svg_document_لمبائی == 0 )
    {
      error = FT_THROW( Invalid_خانہ_Handle );
      جاؤ Exit;
    }

    /* allocate a new document */
    doc_لمبائی = document->svg_document_لمبائی;
    اگر ( FT_QALLOC( glyph->svg_document, doc_لمبائی ) )
      جاؤ Exit;
    glyph->svg_document_لمبائی = doc_لمبائی;

    glyph->glyph_اشاریہ = خانہ->glyph_اشاریہ;

    glyph->metrics      = document->metrics;
    glyph->units_فی_EM = document->units_فی_EM;

    glyph->آغاز_glyph_id = document->آغاز_glyph_id;
    glyph->انتہا_glyph_id   = document->انتہا_glyph_id;

    glyph->transform = document->transform;
    glyph->delta     = document->delta;

    /* copy the document into glyph */
    FT_MEM_COPY( glyph->svg_document, document->svg_document, doc_لمبائی );

  Exit:
    واپس error;
  }


  FT_CALLBACK_DEF( خالی )
  ft_svg_glyph_مکمل( FT_Glyph  svg_glyph )
  {
    FT_SvgGlyph  glyph  = (FT_SvgGlyph)svg_glyph;
    FT_حافظہ    حافظہ = svg_glyph->library->حافظہ;


    /* just free the memory */
    FT_FREE( glyph->svg_document );
  }


  FT_CALLBACK_DEF( FT_Error )
  ft_svg_glyph_copy( FT_Glyph  svg_ماخذ,
                     FT_Glyph  svg_ہدف )
  {
    FT_SvgGlyph  ماخذ = (FT_SvgGlyph)svg_ماخذ;
    FT_SvgGlyph  ہدف = (FT_SvgGlyph)svg_ہدف;

    FT_Error   error  = FT_Err_Ok;
    FT_حافظہ  حافظہ = FT_GLYPH( ماخذ )->library->حافظہ;


    اگر ( svg_ماخذ->format != FT_GLYPH_FORMAT_SVG )
    {
      error = FT_THROW( Invalid_Glyph_Format );
      جاؤ Exit;
    }

    اگر ( ماخذ->svg_document_لمبائی == 0 )
    {
      error = FT_THROW( Invalid_خانہ_Handle );
      جاؤ Exit;
    }

    ہدف->glyph_اشاریہ = ماخذ->glyph_اشاریہ;

    ہدف->svg_document_لمبائی = ماخذ->svg_document_لمبائی;

    ہدف->metrics      = ماخذ->metrics;
    ہدف->units_فی_EM = ماخذ->units_فی_EM;

    ہدف->آغاز_glyph_id = ماخذ->آغاز_glyph_id;
    ہدف->انتہا_glyph_id   = ماخذ->انتہا_glyph_id;

    ہدف->transform = ماخذ->transform;
    ہدف->delta     = ماخذ->delta;

    /* allocate space for the SVG document */
    اگر ( FT_QALLOC( ہدف->svg_document, ہدف->svg_document_لمبائی ) )
      جاؤ Exit;

    /* copy the document */
    FT_MEM_COPY( ہدف->svg_document,
                 ماخذ->svg_document,
                 ہدف->svg_document_لمبائی );

  Exit:
    واپس error;
  }


  FT_CALLBACK_DEF( خالی )
  ft_svg_glyph_transform( FT_Glyph          svg_glyph,
                          ثابت FT_میٹرکس*  میٹرکس_2,
                          ثابت FT_سمتیہ*  _delta )
  {
    FT_SvgGlyph  glyph  = (FT_SvgGlyph)svg_glyph;
    FT_میٹرکس*   میٹرکس = (FT_میٹرکس*)میٹرکس_2;
    FT_سمتیہ*   delta  = (FT_سمتیہ*)_delta;

    FT_میٹرکس  tmp_میٹرکس;
    FT_سمتیہ  tmp_delta;

    FT_میٹرکس  a, b;
    FT_Pos     x, y;


    اگر ( !میٹرکس )
    {
      tmp_میٹرکس.xx = 0x10000;
      tmp_میٹرکس.xy = 0;
      tmp_میٹرکس.yx = 0;
      tmp_میٹرکس.yy = 0x10000;

      میٹرکس = &tmp_میٹرکس;
    }

    اگر ( !delta )
    {
      tmp_delta.x = 0;
      tmp_delta.y = 0;

      delta = &tmp_delta;
    }

    a = glyph->transform;
    b = *میٹرکس;
    FT_میٹرکس_Multiply( &b, &a );

    x = جوڑو_LONG( جوڑو_LONG( FT_MulFix( میٹرکس->xx, glyph->delta.x ),
                            FT_MulFix( میٹرکس->xy, glyph->delta.y ) ),
                  delta->x );
    y = جوڑو_LONG( جوڑو_LONG( FT_MulFix( میٹرکس->yx, glyph->delta.x ),
                            FT_MulFix( میٹرکس->yy, glyph->delta.y ) ),
                  delta->y );

    glyph->delta.x = x;
    glyph->delta.y = y;

    glyph->transform = a;
  }


  FT_CALLBACK_DEF( FT_Error )
  ft_svg_glyph_prepare( FT_Glyph      svg_glyph,
                        FT_Glyph_خانہ  خانہ )
  {
    FT_SvgGlyph  glyph = (FT_SvgGlyph)svg_glyph;

    FT_Error   error  = FT_Err_Ok;
    FT_حافظہ  حافظہ = svg_glyph->library->حافظہ;

    FT_SVG_Document  document = تہی;


    اگر ( FT_NEW( document ) )
      واپس error;

    document->svg_document        = glyph->svg_document;
    document->svg_document_لمبائی = glyph->svg_document_لمبائی;

    document->metrics      = glyph->metrics;
    document->units_فی_EM = glyph->units_فی_EM;

    document->آغاز_glyph_id = glyph->آغاز_glyph_id;
    document->انتہا_glyph_id   = glyph->انتہا_glyph_id;

    document->transform = glyph->transform;
    document->delta     = glyph->delta;

    خانہ->format      = FT_GLYPH_FORMAT_SVG;
    خانہ->glyph_اشاریہ = glyph->glyph_اشاریہ;
    خانہ->other       = document;

    واپس error;
  }


  FT_DEFINE_GLYPH(
    ft_svg_glyph_class,

    حجم ( FT_SvgGlyphRec ),
    FT_GLYPH_FORMAT_SVG,

    ft_svg_glyph_init,      /* FT_Glyph_InitFunc       glyph_init      */
    ft_svg_glyph_مکمل,      /* FT_Glyph_DoneFunc       glyph_done      */
    ft_svg_glyph_copy,      /* FT_Glyph_CopyFunc       glyph_copy      */
    ft_svg_glyph_transform, /* FT_Glyph_TransformFunc  glyph_transform */
    تہی,                   /* FT_Glyph_GetBBoxFunc    glyph_bbox      */
    ft_svg_glyph_prepare    /* FT_Glyph_PrepareFunc    glyph_prepare   */
  )

#اگر_ختم /* FT_CONFIG_OPTION_SVG */


  /*************************************************************************/
  /*************************************************************************/
  /****                                                                 ****/
  /****   FT_Glyph class and API                                        ****/
  /****                                                                 ****/
  /*************************************************************************/
  /*************************************************************************/

   ساکن FT_Error
   ft_new_glyph( FT_Library             library,
                 ثابت FT_Glyph_Class*  clazz,
                 FT_Glyph*              aglyph )
   {
     FT_حافظہ  حافظہ = library->حافظہ;
     FT_Error   error;
     FT_Glyph   glyph  = تہی;


     *aglyph = تہی;

     اگر ( !FT_ALLOC( glyph, clazz->glyph_سائز ) )
     {
       glyph->library = library;
       glyph->clazz   = clazz;
       glyph->format  = clazz->glyph_format;

       *aglyph = glyph;
     }

     واپس error;
   }


  /* documentation is in ftglyph.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Glyph_Copy( FT_Glyph   ماخذ,
                 FT_Glyph  *ہدف )
  {
    FT_Glyph               copy;
    FT_Error               error;
    ثابت FT_Glyph_Class*  clazz;


    /* check arguments */
    اگر ( !ہدف || !ماخذ || !ماخذ->clazz )
    {
      error = FT_THROW( Invalid_دلیل );
      جاؤ Exit;
    }

    *ہدف = تہی;

    اگر ( !ماخذ || !ماخذ->clazz )
    {
      error = FT_THROW( Invalid_دلیل );
      جاؤ Exit;
    }

    clazz = ماخذ->clazz;
    error = ft_new_glyph( ماخذ->library, clazz, &copy );
    اگر ( error )
      جاؤ Exit;

    copy->advance = ماخذ->advance;
    copy->format  = ماخذ->format;

    اگر ( clazz->glyph_copy )
      error = clazz->glyph_copy( ماخذ, copy );

    اگر ( error )
      FT_مکمل_Glyph( copy );
    ورنہ
      *ہدف = copy;

  Exit:
    واپس error;
  }


  /* documentation is in ftglyph.h */

  FT_EXPORT( FT_Error )
  FT_New_Glyph( FT_Library       library,
                FT_Glyph_Format  format,
                FT_Glyph        *aglyph )
  {
    ثابت FT_Glyph_Class*  clazz = تہی;

    اگر ( !library || !aglyph )
      واپس FT_THROW( Invalid_دلیل );

    /* if it is a bitmap, that's easy :-) */
    اگر ( format == FT_GLYPH_FORMAT_BITMAP )
      clazz = &ft_bitmap_glyph_class;

    /* if it is an outline */
    ورنہ اگر ( format == FT_GLYPH_FORMAT_OUTLINE )
      clazz = &ft_outline_glyph_class;

#اگر_تعریف_شدہ FT_CONFIG_OPTION_SVG
    /* if it is an SVG glyph */
    ورنہ اگر ( format == FT_GLYPH_FORMAT_SVG )
      clazz = &ft_svg_glyph_class;
#اگر_ختم

    ورنہ
    {
      /* try to find a renderer that supports the glyph image format */
      FT_Renderer  render = FT_Lookup_Renderer( library, format, 0 );


      اگر ( render )
        clazz = &render->glyph_class;
    }

    اگر ( !clazz )
      واپس FT_THROW( Invalid_Glyph_Format );

    /* create FT_Glyph object */
    واپس ft_new_glyph( library, clazz, aglyph );
  }


  /* documentation is in ftglyph.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Glyph( FT_Glyph_خانہ  خانہ,
                FT_Glyph     *aglyph )
  {
    FT_Error  error;
    FT_Glyph  glyph;


    اگر ( !خانہ )
      واپس FT_THROW( Invalid_خانہ_Handle );

    اگر ( !aglyph )
      واپس FT_THROW( Invalid_دلیل );

    /* create FT_Glyph object */
    error = FT_New_Glyph( خانہ->library, خانہ->format, &glyph );
    اگر ( error )
      جاؤ Exit;

    /* copy advance while converting 26.6 to 16.16 format */
    اگر ( خانہ->advance.x >=  0x8000L * 64 ||
         خانہ->advance.x <= -0x8000L * 64 )
    {
      FT_ERROR(( "FT_Get_Glyph: advance width too large\n" ));
      error = FT_THROW( Invalid_دلیل );
      جاؤ Exit2;
    }
    اگر ( خانہ->advance.y >=  0x8000L * 64 ||
         خانہ->advance.y <= -0x8000L * 64 )
    {
      FT_ERROR(( "FT_Get_Glyph: advance height too large\n" ));
      error = FT_THROW( Invalid_دلیل );
      جاؤ Exit2;
    }

    glyph->advance.x = خانہ->advance.x * 1024;
    glyph->advance.y = خانہ->advance.y * 1024;

    /* now import the image from the glyph slot */
    error = glyph->clazz->glyph_init( glyph, خانہ );

  Exit2:
    /* if an error occurred, destroy the glyph */
    اگر ( error )
    {
      FT_مکمل_Glyph( glyph );
      *aglyph = تہی;
    }
    ورنہ
      *aglyph = glyph;

  Exit:
    واپس error;
  }


  /* documentation is in ftglyph.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Glyph_Transform( FT_Glyph          glyph,
                      ثابت FT_میٹرکس*  میٹرکس,
                      ثابت FT_سمتیہ*  delta )
  {
    FT_Error  error = FT_Err_Ok;


    اگر ( !glyph || !glyph->clazz )
      error = FT_THROW( Invalid_دلیل );
    ورنہ
    {
      ثابت FT_Glyph_Class*  clazz = glyph->clazz;


      اگر ( clazz->glyph_transform )
      {
        /* transform glyph image */
        clazz->glyph_transform( glyph, میٹرکس, delta );

        /* transform advance vector */
        اگر ( میٹرکس )
          FT_سمتیہ_Transform( &glyph->advance, میٹرکس );
      }
      ورنہ
        error = FT_THROW( Invalid_Glyph_Format );
    }
    واپس error;
  }


  /* documentation is in ftglyph.h */

  FT_EXPORT_DEF( خالی )
  FT_Glyph_Get_CBox( FT_Glyph  glyph,
                     FT_UInt   bbox_منوال,
                     FT_BBox  *acbox )
  {
    ثابت FT_Glyph_Class*  clazz;


    اگر ( !acbox )
      واپس;

    acbox->xMin = acbox->yMin = acbox->xMax = acbox->yMax = 0;

    اگر ( !glyph || !glyph->clazz )
      واپس;

    clazz = glyph->clazz;
    اگر ( !clazz->glyph_bbox )
      واپس;

    /* retrieve bbox in 26.6 coordinates */
    clazz->glyph_bbox( glyph, acbox );

    /* perform grid fitting if needed */
    اگر ( bbox_منوال == FT_GLYPH_BBOX_GRIDFIT ||
         bbox_منوال == FT_GLYPH_BBOX_PIXELS  )
    {
      acbox->xMin = FT_PIX_FLOOR( acbox->xMin );
      acbox->yMin = FT_PIX_FLOOR( acbox->yMin );
      acbox->xMax = FT_PIX_CEIL_LONG( acbox->xMax );
      acbox->yMax = FT_PIX_CEIL_LONG( acbox->yMax );
    }

    /* convert to integer pixels if needed */
    اگر ( bbox_منوال == FT_GLYPH_BBOX_TRUNCATE ||
         bbox_منوال == FT_GLYPH_BBOX_PIXELS   )
    {
      acbox->xMin >>= 6;
      acbox->yMin >>= 6;
      acbox->xMax >>= 6;
      acbox->yMax >>= 6;
    }
  }


  /* documentation is in ftglyph.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Glyph_مقصد_Bitmap( FT_Glyph*         the_glyph,
                      FT_Render_منوال    render_منوال,
                      ثابت FT_سمتیہ*  origin,
                      FT_Bool           destroy )
  {
    FT_Glyph_خانہ_Rec           dummy;
    FT_Glyph_خانہ_اندرونی_Rec  dummy_اندرونی;
    FT_Error                  error = FT_Err_Ok;
    FT_Glyph                  b, glyph;
    FT_BitmapGlyph            bitmap = تہی;
    ثابت FT_Glyph_Class*     clazz;

    FT_Library                library;


    /* check argument */
    اگر ( !the_glyph )
      جاؤ Bad;
    glyph = *the_glyph;
    اگر ( !glyph )
      جاؤ Bad;

    clazz   = glyph->clazz;
    library = glyph->library;
    اگر ( !library || !clazz )
      جاؤ Bad;

    /* when called with a bitmap glyph, do nothing and return successfully */
    اگر ( clazz == &ft_bitmap_glyph_class )
      جاؤ Exit;

    اگر ( !clazz->glyph_prepare )
      جاؤ Bad;

    /* we render the glyph into a glyph bitmap using a `dummy' glyph slot */
    /* then calling FT_Render_Glyph_Internal()                            */

    FT_ZERO( &dummy );
    FT_ZERO( &dummy_اندرونی );
    dummy.اندرونی_2 = &dummy_اندرونی;
    dummy.library  = library;
    dummy.format   = clazz->glyph_format;

    /* create result bitmap glyph */
    error = ft_new_glyph( library, &ft_bitmap_glyph_class, &b );
    اگر ( error )
      جاؤ Exit;
    bitmap = (FT_BitmapGlyph)b;

#اگر 1
    /* if `origin' is set, translate the glyph image */
    اگر ( origin )
      FT_Glyph_Transform( glyph, تہی, origin );
#ورنہ
    FT_غیر_مستعمل( origin );
#اگر_ختم

    /* prepare dummy slot for rendering */
    error = clazz->glyph_prepare( glyph, &dummy );
    اگر ( !error )
      error = FT_Render_Glyph_اندرونی( glyph->library, &dummy, render_منوال );

#اگر_تعریف_شدہ FT_CONFIG_OPTION_SVG
    اگر ( clazz == &ft_svg_glyph_class )
    {
      FT_حافظہ  حافظہ = library->حافظہ;


      FT_FREE( dummy.other );
    }
#اگر_ختم

#اگر 1
    اگر ( !destroy && origin )
    {
      FT_سمتیہ  v;


      v.x = -origin->x;
      v.y = -origin->y;
      FT_Glyph_Transform( glyph, تہی, &v );
    }
#اگر_ختم

    اگر ( error )
      جاؤ Exit;

    /* in case of success, copy the bitmap to the glyph bitmap */
    bitmap->بایاں   = dummy.bitmap_بایاں;
    bitmap->چوٹی    = dummy.bitmap_چوٹی;
    bitmap->bitmap = dummy.bitmap;

    /* copy advance */
    bitmap->جڑ.advance = glyph->advance;

    اگر ( destroy )
      FT_مکمل_Glyph( glyph );

    *the_glyph = FT_GLYPH( bitmap );

  Exit:
    اگر ( error && bitmap )
      FT_مکمل_Glyph( FT_GLYPH( bitmap ) );

    واپس error;

  Bad:
    error = FT_THROW( Invalid_دلیل );
    جاؤ Exit;
  }


  /* documentation is in ftglyph.h */

  FT_EXPORT_DEF( خالی )
  FT_مکمل_Glyph( FT_Glyph  glyph )
  {
    اگر ( glyph )
    {
      FT_حافظہ              حافظہ = glyph->library->حافظہ;
      ثابت FT_Glyph_Class*  clazz  = glyph->clazz;


      اگر ( clazz->glyph_مکمل )
        clazz->glyph_مکمل( glyph );

      FT_FREE( glyph );
    }
  }


/*انتہا*/
