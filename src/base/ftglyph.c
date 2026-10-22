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


#ضمن <freetype/internal/ftdebug.h>

#ضمن <freetype/ftglyph.h>
#ضمن <freetype/ftoutln.h>
#ضمن <freetype/ftbitmap.h>
#ضمن <freetype/internal/ftobjs.h>
#ضمن <freetype/otsvg.h>

#ضمن "ftbase.h"


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#تعریف_لغوه  FT_COMPONENT
#تعریف_کړه FT_COMPONENT  glyph


  /*************************************************************************/
  /*************************************************************************/
  /****                                                                 ****/
  /****   FT_BitmapGlyph support                                        ****/
  /****                                                                 ****/
  /*************************************************************************/
  /*************************************************************************/

  FT_CALLBACK_DEF( FT_Error )
  ft_bitmap_glyph_init( FT_Glyph      bitmap_glyph,
                        FT_Glyph_خونه  خونه )
  {
    FT_BitmapGlyph  glyph   = (FT_BitmapGlyph)bitmap_glyph;
    FT_Error        error;
    FT_Library      library = FT_GLYPH( glyph )->library;


    إذا ( خونه->format != FT_GLYPH_FORMAT_BITMAP )
    {
      error = FT_THROW( Invalid_Glyph_Format );
      اذهب Exit;
    }

    glyph->کیڼ = خونه->bitmap_کیڼ;
    glyph->سر  = خونه->bitmap_سر;

    FT_Bitmap_Init( &glyph->bitmap );
    error = FT_Bitmap_Copy( library, &خونه->bitmap, &glyph->bitmap );

  Exit:
    أرجع error;
  }


  FT_CALLBACK_DEF( FT_Error )
  ft_bitmap_glyph_copy( FT_Glyph  bitmap_سرچینه,
                        FT_Glyph  bitmap_موخه )
  {
    FT_Library      library = bitmap_سرچینه->library;
    FT_BitmapGlyph  سرچینه  = (FT_BitmapGlyph)bitmap_سرچینه;
    FT_BitmapGlyph  موخه  = (FT_BitmapGlyph)bitmap_موخه;


    موخه->کیڼ = سرچینه->کیڼ;
    موخه->سر  = سرچینه->سر;

    أرجع FT_Bitmap_Copy( library, &سرچینه->bitmap, &موخه->bitmap );
  }


  FT_CALLBACK_DEF( فراغ )
  ft_bitmap_glyph_بشپړ( FT_Glyph  bitmap_glyph )
  {
    FT_BitmapGlyph  glyph   = (FT_BitmapGlyph)bitmap_glyph;
    FT_Library      library = FT_GLYPH( glyph )->library;


    FT_Bitmap_بشپړ( library, &glyph->bitmap );
  }


  FT_CALLBACK_DEF( فراغ )
  ft_bitmap_glyph_bbox( FT_Glyph  bitmap_glyph,
                        FT_BBox*  cbox )
  {
    FT_BitmapGlyph  glyph = (FT_BitmapGlyph)bitmap_glyph;


    cbox->xMin = glyph->کیڼ * 64;
    cbox->xMax = cbox->xMin + (FT_Pos)( glyph->bitmap.پلنوالی * 64 );
    cbox->yMax = glyph->سر * 64;
    cbox->yMin = cbox->yMax - (FT_Pos)( glyph->bitmap.لیکې * 64 );
  }


  FT_DEFINE_GLYPH(
    ft_bitmap_glyph_class,

    حجم ( FT_BitmapGlyphRec ),
    FT_GLYPH_FORMAT_BITMAP,

    ft_bitmap_glyph_init,    /* FT_Glyph_InitFunc       glyph_init      */
    ft_bitmap_glyph_بشپړ,    /* FT_Glyph_DoneFunc       glyph_done      */
    ft_bitmap_glyph_copy,    /* FT_Glyph_CopyFunc       glyph_copy      */
    عدم,                    /* FT_Glyph_TransformFunc  glyph_transform */
    ft_bitmap_glyph_bbox,    /* FT_Glyph_GetBBoxFunc    glyph_bbox      */
    عدم                     /* FT_Glyph_PrepareFunc    glyph_prepare   */
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
                         FT_Glyph_خونه  خونه )
  {
    FT_OutlineGlyph  glyph   = (FT_OutlineGlyph)outline_glyph;
    FT_Error         error   = FT_Err_Ok;
    FT_Library       library = FT_GLYPH( glyph )->library;
    FT_Outline*      سرچینه  = &خونه->outline;
    FT_Outline*      موخه  = &glyph->outline;


    /* check format in glyph slot */
    إذا ( خونه->format != FT_GLYPH_FORMAT_OUTLINE )
    {
      error = FT_THROW( Invalid_Glyph_Format );
      اذهب Exit;
    }

    /* allocate new outline */
    error = FT_Outline_New( library,
                            (FT_UInt)سرچینه->n_points,
                            سرچینه->n_contours,
                            &glyph->outline );
    إذا ( error )
      اذهب Exit;

    FT_Outline_Copy( سرچینه, موخه );

  Exit:
    أرجع error;
  }


  FT_CALLBACK_DEF( فراغ )
  ft_outline_glyph_بشپړ( FT_Glyph  outline_glyph )
  {
    FT_OutlineGlyph  glyph = (FT_OutlineGlyph)outline_glyph;


    FT_Outline_بشپړ( FT_GLYPH( glyph )->library, &glyph->outline );
  }


  FT_CALLBACK_DEF( FT_Error )
  ft_outline_glyph_copy( FT_Glyph  outline_سرچینه,
                         FT_Glyph  outline_موخه )
  {
    FT_OutlineGlyph  سرچینه  = (FT_OutlineGlyph)outline_سرچینه;
    FT_OutlineGlyph  موخه  = (FT_OutlineGlyph)outline_موخه;
    FT_Error         error;
    FT_Library       library = FT_GLYPH( سرچینه )->library;


    error = FT_Outline_New( library,
                            (FT_UInt)سرچینه->outline.n_points,
                            سرچینه->outline.n_contours,
                            &موخه->outline );
    إذا ( !error )
      FT_Outline_Copy( &سرچینه->outline, &موخه->outline );

    أرجع error;
  }


  FT_CALLBACK_DEF( فراغ )
  ft_outline_glyph_transform( FT_Glyph          outline_glyph,
                              ثابت FT_ماتریکس*  ماتریکس,
                              ثابت FT_ویکټور*  delta )
  {
    FT_OutlineGlyph  glyph = (FT_OutlineGlyph)outline_glyph;


    إذا ( ماتریکس )
      FT_Outline_Transform( &glyph->outline, ماتریکس );

    إذا ( delta )
      FT_Outline_Translate( &glyph->outline, delta->x, delta->y );
  }


  FT_CALLBACK_DEF( فراغ )
  ft_outline_glyph_bbox( FT_Glyph  outline_glyph,
                         FT_BBox*  bbox )
  {
    FT_OutlineGlyph  glyph = (FT_OutlineGlyph)outline_glyph;


    FT_Outline_Get_CBox( &glyph->outline, bbox );
  }


  FT_CALLBACK_DEF( FT_Error )
  ft_outline_glyph_prepare( FT_Glyph      outline_glyph,
                            FT_Glyph_خونه  خونه )
  {
    FT_OutlineGlyph  glyph = (FT_OutlineGlyph)outline_glyph;


    خونه->format         = FT_GLYPH_FORMAT_OUTLINE;
    خونه->outline        = glyph->outline;
    خونه->outline.flags &= ~FT_OUTLINE_OWNER;

    أرجع FT_Err_Ok;
  }


  FT_DEFINE_GLYPH(
    ft_outline_glyph_class,

    حجم ( FT_OutlineGlyphRec ),
    FT_GLYPH_FORMAT_OUTLINE,

    ft_outline_glyph_init,      /* FT_Glyph_InitFunc       glyph_init      */
    ft_outline_glyph_بشپړ,      /* FT_Glyph_DoneFunc       glyph_done      */
    ft_outline_glyph_copy,      /* FT_Glyph_CopyFunc       glyph_copy      */
    ft_outline_glyph_transform, /* FT_Glyph_TransformFunc  glyph_transform */
    ft_outline_glyph_bbox,      /* FT_Glyph_GetBBoxFunc    glyph_bbox      */
    ft_outline_glyph_prepare    /* FT_Glyph_PrepareFunc    glyph_prepare   */
  )


#که_تعریف_شوی FT_CONFIG_OPTION_SVG

  /*************************************************************************/
  /*************************************************************************/
  /****                                                                 ****/
  /****   FT_SvgGlyph support                                           ****/
  /****                                                                 ****/
  /*************************************************************************/
  /*************************************************************************/


  FT_CALLBACK_DEF( FT_Error )
  ft_svg_glyph_init( FT_Glyph      svg_glyph,
                     FT_Glyph_خونه  خونه )
  {
    FT_ULong         doc_اوږدتیا;
    FT_SVG_Document  document;
    FT_SvgGlyph      glyph = (FT_SvgGlyph)svg_glyph;

    FT_Error   error  = FT_Err_Ok;
    FT_حافظه  حافظه = FT_GLYPH( glyph )->library->حافظه;


    إذا ( خونه->format != FT_GLYPH_FORMAT_SVG )
    {
      error = FT_THROW( Invalid_Glyph_Format );
      اذهب Exit;
    }

    إذا ( خونه->other == عدم )
    {
      error = FT_THROW( Invalid_خونه_Handle );
      اذهب Exit;
    }

    document = (FT_SVG_Document)خونه->other;

    إذا ( document->svg_document_اوږدتیا == 0 )
    {
      error = FT_THROW( Invalid_خونه_Handle );
      اذهب Exit;
    }

    /* allocate a new document */
    doc_اوږدتیا = document->svg_document_اوږدتیا;
    إذا ( FT_QALLOC( glyph->svg_document, doc_اوږدتیا ) )
      اذهب Exit;
    glyph->svg_document_اوږدتیا = doc_اوږدتیا;

    glyph->glyph_شاخص = خونه->glyph_شاخص;

    glyph->metrics      = document->metrics;
    glyph->units_فی_EM = document->units_فی_EM;

    glyph->پیل_glyph_id = document->پیل_glyph_id;
    glyph->پای_ټکی_glyph_id   = document->پای_ټکی_glyph_id;

    glyph->transform = document->transform;
    glyph->delta     = document->delta;

    /* copy the document into glyph */
    FT_MEM_COPY( glyph->svg_document, document->svg_document, doc_اوږدتیا );

  Exit:
    أرجع error;
  }


  FT_CALLBACK_DEF( فراغ )
  ft_svg_glyph_بشپړ( FT_Glyph  svg_glyph )
  {
    FT_SvgGlyph  glyph  = (FT_SvgGlyph)svg_glyph;
    FT_حافظه    حافظه = svg_glyph->library->حافظه;


    /* just free the memory */
    FT_FREE( glyph->svg_document );
  }


  FT_CALLBACK_DEF( FT_Error )
  ft_svg_glyph_copy( FT_Glyph  svg_سرچینه,
                     FT_Glyph  svg_موخه )
  {
    FT_SvgGlyph  سرچینه = (FT_SvgGlyph)svg_سرچینه;
    FT_SvgGlyph  موخه = (FT_SvgGlyph)svg_موخه;

    FT_Error   error  = FT_Err_Ok;
    FT_حافظه  حافظه = FT_GLYPH( سرچینه )->library->حافظه;


    إذا ( svg_سرچینه->format != FT_GLYPH_FORMAT_SVG )
    {
      error = FT_THROW( Invalid_Glyph_Format );
      اذهب Exit;
    }

    إذا ( سرچینه->svg_document_اوږدتیا == 0 )
    {
      error = FT_THROW( Invalid_خونه_Handle );
      اذهب Exit;
    }

    موخه->glyph_شاخص = سرچینه->glyph_شاخص;

    موخه->svg_document_اوږدتیا = سرچینه->svg_document_اوږدتیا;

    موخه->metrics      = سرچینه->metrics;
    موخه->units_فی_EM = سرچینه->units_فی_EM;

    موخه->پیل_glyph_id = سرچینه->پیل_glyph_id;
    موخه->پای_ټکی_glyph_id   = سرچینه->پای_ټکی_glyph_id;

    موخه->transform = سرچینه->transform;
    موخه->delta     = سرچینه->delta;

    /* allocate space for the SVG document */
    إذا ( FT_QALLOC( موخه->svg_document, موخه->svg_document_اوږدتیا ) )
      اذهب Exit;

    /* copy the document */
    FT_MEM_COPY( موخه->svg_document,
                 سرچینه->svg_document,
                 موخه->svg_document_اوږدتیا );

  Exit:
    أرجع error;
  }


  FT_CALLBACK_DEF( فراغ )
  ft_svg_glyph_transform( FT_Glyph          svg_glyph,
                          ثابت FT_ماتریکس*  ماتریکس_2,
                          ثابت FT_ویکټور*  _delta )
  {
    FT_SvgGlyph  glyph  = (FT_SvgGlyph)svg_glyph;
    FT_ماتریکس*   ماتریکس = (FT_ماتریکس*)ماتریکس_2;
    FT_ویکټور*   delta  = (FT_ویکټور*)_delta;

    FT_ماتریکس  tmp_ماتریکس;
    FT_ویکټور  tmp_delta;

    FT_ماتریکس  a, b;
    FT_Pos     x, y;


    إذا ( !ماتریکس )
    {
      tmp_ماتریکس.xx = 0x10000;
      tmp_ماتریکس.xy = 0;
      tmp_ماتریکس.yx = 0;
      tmp_ماتریکس.yy = 0x10000;

      ماتریکس = &tmp_ماتریکس;
    }

    إذا ( !delta )
    {
      tmp_delta.x = 0;
      tmp_delta.y = 0;

      delta = &tmp_delta;
    }

    a = glyph->transform;
    b = *ماتریکس;
    FT_ماتریکس_Multiply( &b, &a );

    x = جمع_کړه_LONG( جمع_کړه_LONG( FT_MulFix( ماتریکس->xx, glyph->delta.x ),
                            FT_MulFix( ماتریکس->xy, glyph->delta.y ) ),
                  delta->x );
    y = جمع_کړه_LONG( جمع_کړه_LONG( FT_MulFix( ماتریکس->yx, glyph->delta.x ),
                            FT_MulFix( ماتریکس->yy, glyph->delta.y ) ),
                  delta->y );

    glyph->delta.x = x;
    glyph->delta.y = y;

    glyph->transform = a;
  }


  FT_CALLBACK_DEF( FT_Error )
  ft_svg_glyph_prepare( FT_Glyph      svg_glyph,
                        FT_Glyph_خونه  خونه )
  {
    FT_SvgGlyph  glyph = (FT_SvgGlyph)svg_glyph;

    FT_Error   error  = FT_Err_Ok;
    FT_حافظه  حافظه = svg_glyph->library->حافظه;

    FT_SVG_Document  document = عدم;


    إذا ( FT_NEW( document ) )
      أرجع error;

    document->svg_document        = glyph->svg_document;
    document->svg_document_اوږدتیا = glyph->svg_document_اوږدتیا;

    document->metrics      = glyph->metrics;
    document->units_فی_EM = glyph->units_فی_EM;

    document->پیل_glyph_id = glyph->پیل_glyph_id;
    document->پای_ټکی_glyph_id   = glyph->پای_ټکی_glyph_id;

    document->transform = glyph->transform;
    document->delta     = glyph->delta;

    خونه->format      = FT_GLYPH_FORMAT_SVG;
    خونه->glyph_شاخص = glyph->glyph_شاخص;
    خونه->other       = document;

    أرجع error;
  }


  FT_DEFINE_GLYPH(
    ft_svg_glyph_class,

    حجم ( FT_SvgGlyphRec ),
    FT_GLYPH_FORMAT_SVG,

    ft_svg_glyph_init,      /* FT_Glyph_InitFunc       glyph_init      */
    ft_svg_glyph_بشپړ,      /* FT_Glyph_DoneFunc       glyph_done      */
    ft_svg_glyph_copy,      /* FT_Glyph_CopyFunc       glyph_copy      */
    ft_svg_glyph_transform, /* FT_Glyph_TransformFunc  glyph_transform */
    عدم,                   /* FT_Glyph_GetBBoxFunc    glyph_bbox      */
    ft_svg_glyph_prepare    /* FT_Glyph_PrepareFunc    glyph_prepare   */
  )

#د_که_پای /* FT_CONFIG_OPTION_SVG */


  /*************************************************************************/
  /*************************************************************************/
  /****                                                                 ****/
  /****   FT_Glyph class and API                                        ****/
  /****                                                                 ****/
  /*************************************************************************/
  /*************************************************************************/

   ساكن FT_Error
   ft_new_glyph( FT_Library             library,
                 ثابت FT_Glyph_Class*  clazz,
                 FT_Glyph*              aglyph )
   {
     FT_حافظه  حافظه = library->حافظه;
     FT_Error   error;
     FT_Glyph   glyph  = عدم;


     *aglyph = عدم;

     إذا ( !FT_ALLOC( glyph, clazz->glyph_اندازه ) )
     {
       glyph->library = library;
       glyph->clazz   = clazz;
       glyph->format  = clazz->glyph_format;

       *aglyph = glyph;
     }

     أرجع error;
   }


  /* documentation is in ftglyph.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Glyph_Copy( FT_Glyph   سرچینه,
                 FT_Glyph  *موخه )
  {
    FT_Glyph               copy;
    FT_Error               error;
    ثابت FT_Glyph_Class*  clazz;


    /* check arguments */
    إذا ( !موخه || !سرچینه || !سرچینه->clazz )
    {
      error = FT_THROW( Invalid_دلیل );
      اذهب Exit;
    }

    *موخه = عدم;

    إذا ( !سرچینه || !سرچینه->clazz )
    {
      error = FT_THROW( Invalid_دلیل );
      اذهب Exit;
    }

    clazz = سرچینه->clazz;
    error = ft_new_glyph( سرچینه->library, clazz, &copy );
    إذا ( error )
      اذهب Exit;

    copy->advance = سرچینه->advance;
    copy->format  = سرچینه->format;

    إذا ( clazz->glyph_copy )
      error = clazz->glyph_copy( سرچینه, copy );

    إذا ( error )
      FT_بشپړ_Glyph( copy );
    وإلا
      *موخه = copy;

  Exit:
    أرجع error;
  }


  /* documentation is in ftglyph.h */

  FT_EXPORT( FT_Error )
  FT_New_Glyph( FT_Library       library,
                FT_Glyph_Format  format,
                FT_Glyph        *aglyph )
  {
    ثابت FT_Glyph_Class*  clazz = عدم;

    إذا ( !library || !aglyph )
      أرجع FT_THROW( Invalid_دلیل );

    /* if it is a bitmap, that's easy :-) */
    إذا ( format == FT_GLYPH_FORMAT_BITMAP )
      clazz = &ft_bitmap_glyph_class;

    /* if it is an outline */
    وإلا إذا ( format == FT_GLYPH_FORMAT_OUTLINE )
      clazz = &ft_outline_glyph_class;

#که_تعریف_شوی FT_CONFIG_OPTION_SVG
    /* if it is an SVG glyph */
    وإلا إذا ( format == FT_GLYPH_FORMAT_SVG )
      clazz = &ft_svg_glyph_class;
#د_که_پای

    وإلا
    {
      /* try to find a renderer that supports the glyph image format */
      FT_Renderer  render = FT_Lookup_Renderer( library, format, 0 );


      إذا ( render )
        clazz = &render->glyph_class;
    }

    إذا ( !clazz )
      أرجع FT_THROW( Invalid_Glyph_Format );

    /* create FT_Glyph object */
    أرجع ft_new_glyph( library, clazz, aglyph );
  }


  /* documentation is in ftglyph.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Glyph( FT_Glyph_خونه  خونه,
                FT_Glyph     *aglyph )
  {
    FT_Error  error;
    FT_Glyph  glyph;


    إذا ( !خونه )
      أرجع FT_THROW( Invalid_خونه_Handle );

    إذا ( !aglyph )
      أرجع FT_THROW( Invalid_دلیل );

    /* create FT_Glyph object */
    error = FT_New_Glyph( خونه->library, خونه->format, &glyph );
    إذا ( error )
      اذهب Exit;

    /* copy advance while converting 26.6 to 16.16 format */
    إذا ( خونه->advance.x >=  0x8000L * 64 ||
         خونه->advance.x <= -0x8000L * 64 )
    {
      FT_ERROR(( "FT_Get_Glyph: advance width too large\n" ));
      error = FT_THROW( Invalid_دلیل );
      اذهب Exit2;
    }
    إذا ( خونه->advance.y >=  0x8000L * 64 ||
         خونه->advance.y <= -0x8000L * 64 )
    {
      FT_ERROR(( "FT_Get_Glyph: advance height too large\n" ));
      error = FT_THROW( Invalid_دلیل );
      اذهب Exit2;
    }

    glyph->advance.x = خونه->advance.x * 1024;
    glyph->advance.y = خونه->advance.y * 1024;

    /* now import the image from the glyph slot */
    error = glyph->clazz->glyph_init( glyph, خونه );

  Exit2:
    /* if an error occurred, destroy the glyph */
    إذا ( error )
    {
      FT_بشپړ_Glyph( glyph );
      *aglyph = عدم;
    }
    وإلا
      *aglyph = glyph;

  Exit:
    أرجع error;
  }


  /* documentation is in ftglyph.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Glyph_Transform( FT_Glyph          glyph,
                      ثابت FT_ماتریکس*  ماتریکس,
                      ثابت FT_ویکټور*  delta )
  {
    FT_Error  error = FT_Err_Ok;


    إذا ( !glyph || !glyph->clazz )
      error = FT_THROW( Invalid_دلیل );
    وإلا
    {
      ثابت FT_Glyph_Class*  clazz = glyph->clazz;


      إذا ( clazz->glyph_transform )
      {
        /* transform glyph image */
        clazz->glyph_transform( glyph, ماتریکس, delta );

        /* transform advance vector */
        إذا ( ماتریکس )
          FT_ویکټور_Transform( &glyph->advance, ماتریکس );
      }
      وإلا
        error = FT_THROW( Invalid_Glyph_Format );
    }
    أرجع error;
  }


  /* documentation is in ftglyph.h */

  FT_EXPORT_DEF( فراغ )
  FT_Glyph_Get_CBox( FT_Glyph  glyph,
                     FT_UInt   bbox_ډېرپېښ,
                     FT_BBox  *acbox )
  {
    ثابت FT_Glyph_Class*  clazz;


    إذا ( !acbox )
      أرجع;

    acbox->xMin = acbox->yMin = acbox->xMax = acbox->yMax = 0;

    إذا ( !glyph || !glyph->clazz )
      أرجع;

    clazz = glyph->clazz;
    إذا ( !clazz->glyph_bbox )
      أرجع;

    /* retrieve bbox in 26.6 coordinates */
    clazz->glyph_bbox( glyph, acbox );

    /* perform grid fitting if needed */
    إذا ( bbox_ډېرپېښ == FT_GLYPH_BBOX_GRIDFIT ||
         bbox_ډېرپېښ == FT_GLYPH_BBOX_PIXELS  )
    {
      acbox->xMin = FT_PIX_FLOOR( acbox->xMin );
      acbox->yMin = FT_PIX_FLOOR( acbox->yMin );
      acbox->xMax = FT_PIX_CEIL_LONG( acbox->xMax );
      acbox->yMax = FT_PIX_CEIL_LONG( acbox->yMax );
    }

    /* convert to integer pixels if needed */
    إذا ( bbox_ډېرپېښ == FT_GLYPH_BBOX_TRUNCATE ||
         bbox_ډېرپېښ == FT_GLYPH_BBOX_PIXELS   )
    {
      acbox->xMin >>= 6;
      acbox->yMin >>= 6;
      acbox->xMax >>= 6;
      acbox->yMax >>= 6;
    }
  }


  /* documentation is in ftglyph.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Glyph_منزل_Bitmap( FT_Glyph*         the_glyph,
                      FT_Render_ډېرپېښ    render_ډېرپېښ,
                      ثابت FT_ویکټور*  origin,
                      FT_Bool           destroy )
  {
    FT_Glyph_خونه_Rec           dummy;
    FT_Glyph_خونه_دننی_Rec  dummy_دننی;
    FT_Error                  error = FT_Err_Ok;
    FT_Glyph                  b, glyph;
    FT_BitmapGlyph            bitmap = عدم;
    ثابت FT_Glyph_Class*     clazz;

    FT_Library                library;


    /* check argument */
    إذا ( !the_glyph )
      اذهب Bad;
    glyph = *the_glyph;
    إذا ( !glyph )
      اذهب Bad;

    clazz   = glyph->clazz;
    library = glyph->library;
    إذا ( !library || !clazz )
      اذهب Bad;

    /* when called with a bitmap glyph, do nothing and return successfully */
    إذا ( clazz == &ft_bitmap_glyph_class )
      اذهب Exit;

    إذا ( !clazz->glyph_prepare )
      اذهب Bad;

    /* we render the glyph into a glyph bitmap using a `dummy' glyph slot */
    /* then calling FT_Render_Glyph_Internal()                            */

    FT_ZERO( &dummy );
    FT_ZERO( &dummy_دننی );
    dummy.دننی = &dummy_دننی;
    dummy.library  = library;
    dummy.format   = clazz->glyph_format;

    /* create result bitmap glyph */
    error = ft_new_glyph( library, &ft_bitmap_glyph_class, &b );
    إذا ( error )
      اذهب Exit;
    bitmap = (FT_BitmapGlyph)b;

#إذا 1
    /* if `origin' is set, translate the glyph image */
    إذا ( origin )
      FT_Glyph_Transform( glyph, عدم, origin );
#وإلا
    FT_ناکارول_شوی( origin );
#د_که_پای

    /* prepare dummy slot for rendering */
    error = clazz->glyph_prepare( glyph, &dummy );
    إذا ( !error )
      error = FT_Render_Glyph_دننی( glyph->library, &dummy, render_ډېرپېښ );

#که_تعریف_شوی FT_CONFIG_OPTION_SVG
    إذا ( clazz == &ft_svg_glyph_class )
    {
      FT_حافظه  حافظه = library->حافظه;


      FT_FREE( dummy.other );
    }
#د_که_پای

#إذا 1
    إذا ( !destroy && origin )
    {
      FT_ویکټور  v;


      v.x = -origin->x;
      v.y = -origin->y;
      FT_Glyph_Transform( glyph, عدم, &v );
    }
#د_که_پای

    إذا ( error )
      اذهب Exit;

    /* in case of success, copy the bitmap to the glyph bitmap */
    bitmap->کیڼ   = dummy.bitmap_کیڼ;
    bitmap->سر    = dummy.bitmap_سر;
    bitmap->bitmap = dummy.bitmap;

    /* copy advance */
    bitmap->ریښه.advance = glyph->advance;

    إذا ( destroy )
      FT_بشپړ_Glyph( glyph );

    *the_glyph = FT_GLYPH( bitmap );

  Exit:
    إذا ( error && bitmap )
      FT_بشپړ_Glyph( FT_GLYPH( bitmap ) );

    أرجع error;

  Bad:
    error = FT_THROW( Invalid_دلیل );
    اذهب Exit;
  }


  /* documentation is in ftglyph.h */

  FT_EXPORT_DEF( فراغ )
  FT_بشپړ_Glyph( FT_Glyph  glyph )
  {
    إذا ( glyph )
    {
      FT_حافظه              حافظه = glyph->library->حافظه;
      ثابت FT_Glyph_Class*  clazz  = glyph->clazz;


      إذا ( clazz->glyph_بشپړ )
        clazz->glyph_بشپړ( glyph );

      FT_FREE( glyph );
    }
  }


/*پای_ټکی*/
