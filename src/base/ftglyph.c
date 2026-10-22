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
#تعریف_رد  FT_COMPONENT
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
                        FT_Glyph_تھاں  تھاں )
  {
    FT_BitmapGlyph  glyph   = (FT_BitmapGlyph)bitmap_glyph;
    FT_Error        error;
    FT_Library      library = FT_GLYPH( glyph )->library;


    إذا ( تھاں->format != FT_GLYPH_FORMAT_BITMAP )
    {
      error = FT_THROW( Invalid_Glyph_Format );
      اذهب Exit;
    }

    glyph->بایاں = تھاں->bitmap_بایاں;
    glyph->چوٹی  = تھاں->bitmap_چوٹی;

    FT_Bitmap_Init( &glyph->bitmap );
    error = FT_Bitmap_Copy( library, &تھاں->bitmap, &glyph->bitmap );

  Exit:
    أرجع error;
  }


  FT_CALLBACK_DEF( FT_Error )
  ft_bitmap_glyph_copy( FT_Glyph  bitmap_سروت,
                        FT_Glyph  bitmap_نشانہ )
  {
    FT_Library      library = bitmap_سروت->library;
    FT_BitmapGlyph  سروت  = (FT_BitmapGlyph)bitmap_سروت;
    FT_BitmapGlyph  نشانہ  = (FT_BitmapGlyph)bitmap_نشانہ;


    نشانہ->بایاں = سروت->بایاں;
    نشانہ->چوٹی  = سروت->چوٹی;

    أرجع FT_Bitmap_Copy( library, &سروت->bitmap, &نشانہ->bitmap );
  }


  FT_CALLBACK_DEF( فراغ )
  ft_bitmap_glyph_پورا( FT_Glyph  bitmap_glyph )
  {
    FT_BitmapGlyph  glyph   = (FT_BitmapGlyph)bitmap_glyph;
    FT_Library      library = FT_GLYPH( glyph )->library;


    FT_Bitmap_پورا( library, &glyph->bitmap );
  }


  FT_CALLBACK_DEF( فراغ )
  ft_bitmap_glyph_bbox( FT_Glyph  bitmap_glyph,
                        FT_BBox*  cbox )
  {
    FT_BitmapGlyph  glyph = (FT_BitmapGlyph)bitmap_glyph;


    cbox->xMin = glyph->بایاں * 64;
    cbox->xMax = cbox->xMin + (FT_Pos)( glyph->bitmap.چوڑائی * 64 );
    cbox->yMax = glyph->چوٹی * 64;
    cbox->yMin = cbox->yMax - (FT_Pos)( glyph->bitmap.صفاں * 64 );
  }


  FT_DEFINE_GLYPH(
    ft_bitmap_glyph_class,

    حجم ( FT_BitmapGlyphRec ),
    FT_GLYPH_FORMAT_BITMAP,

    ft_bitmap_glyph_init,    /* FT_Glyph_InitFunc       glyph_init      */
    ft_bitmap_glyph_پورا,    /* FT_Glyph_DoneFunc       glyph_done      */
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
                         FT_Glyph_تھاں  تھاں )
  {
    FT_OutlineGlyph  glyph   = (FT_OutlineGlyph)outline_glyph;
    FT_Error         error   = FT_Err_Ok;
    FT_Library       library = FT_GLYPH( glyph )->library;
    FT_Outline*      سروت  = &تھاں->outline;
    FT_Outline*      نشانہ  = &glyph->outline;


    /* check format in glyph slot */
    إذا ( تھاں->format != FT_GLYPH_FORMAT_OUTLINE )
    {
      error = FT_THROW( Invalid_Glyph_Format );
      اذهب Exit;
    }

    /* allocate new outline */
    error = FT_Outline_New( library,
                            (FT_UInt)سروت->n_points,
                            سروت->n_contours,
                            &glyph->outline );
    إذا ( error )
      اذهب Exit;

    FT_Outline_Copy( سروت, نشانہ );

  Exit:
    أرجع error;
  }


  FT_CALLBACK_DEF( فراغ )
  ft_outline_glyph_پورا( FT_Glyph  outline_glyph )
  {
    FT_OutlineGlyph  glyph = (FT_OutlineGlyph)outline_glyph;


    FT_Outline_پورا( FT_GLYPH( glyph )->library, &glyph->outline );
  }


  FT_CALLBACK_DEF( FT_Error )
  ft_outline_glyph_copy( FT_Glyph  outline_سروت,
                         FT_Glyph  outline_نشانہ )
  {
    FT_OutlineGlyph  سروت  = (FT_OutlineGlyph)outline_سروت;
    FT_OutlineGlyph  نشانہ  = (FT_OutlineGlyph)outline_نشانہ;
    FT_Error         error;
    FT_Library       library = FT_GLYPH( سروت )->library;


    error = FT_Outline_New( library,
                            (FT_UInt)سروت->outline.n_points,
                            سروت->outline.n_contours,
                            &نشانہ->outline );
    إذا ( !error )
      FT_Outline_Copy( &سروت->outline, &نشانہ->outline );

    أرجع error;
  }


  FT_CALLBACK_DEF( فراغ )
  ft_outline_glyph_transform( FT_Glyph          outline_glyph,
                              ثابت FT_میٹرکس*  میٹرکس,
                              ثابت FT_سمتیہ*  delta )
  {
    FT_OutlineGlyph  glyph = (FT_OutlineGlyph)outline_glyph;


    إذا ( میٹرکس )
      FT_Outline_Transform( &glyph->outline, میٹرکس );

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
                            FT_Glyph_تھاں  تھاں )
  {
    FT_OutlineGlyph  glyph = (FT_OutlineGlyph)outline_glyph;


    تھاں->format         = FT_GLYPH_FORMAT_OUTLINE;
    تھاں->outline        = glyph->outline;
    تھاں->outline.flags &= ~FT_OUTLINE_OWNER;

    أرجع FT_Err_Ok;
  }


  FT_DEFINE_GLYPH(
    ft_outline_glyph_class,

    حجم ( FT_OutlineGlyphRec ),
    FT_GLYPH_FORMAT_OUTLINE,

    ft_outline_glyph_init,      /* FT_Glyph_InitFunc       glyph_init      */
    ft_outline_glyph_پورا,      /* FT_Glyph_DoneFunc       glyph_done      */
    ft_outline_glyph_copy,      /* FT_Glyph_CopyFunc       glyph_copy      */
    ft_outline_glyph_transform, /* FT_Glyph_TransformFunc  glyph_transform */
    ft_outline_glyph_bbox,      /* FT_Glyph_GetBBoxFunc    glyph_bbox      */
    ft_outline_glyph_prepare    /* FT_Glyph_PrepareFunc    glyph_prepare   */
  )


#جے_تعریف_شدہ FT_CONFIG_OPTION_SVG

  /*************************************************************************/
  /*************************************************************************/
  /****                                                                 ****/
  /****   FT_SvgGlyph support                                           ****/
  /****                                                                 ****/
  /*************************************************************************/
  /*************************************************************************/


  FT_CALLBACK_DEF( FT_Error )
  ft_svg_glyph_init( FT_Glyph      svg_glyph,
                     FT_Glyph_تھاں  تھاں )
  {
    FT_ULong         doc_لمائی;
    FT_SVG_Document  document;
    FT_SvgGlyph      glyph = (FT_SvgGlyph)svg_glyph;

    FT_Error   error  = FT_Err_Ok;
    FT_حافظہ  حافظہ = FT_GLYPH( glyph )->library->حافظہ;


    إذا ( تھاں->format != FT_GLYPH_FORMAT_SVG )
    {
      error = FT_THROW( Invalid_Glyph_Format );
      اذهب Exit;
    }

    إذا ( تھاں->other == عدم )
    {
      error = FT_THROW( Invalid_تھاں_Handle );
      اذهب Exit;
    }

    document = (FT_SVG_Document)تھاں->other;

    إذا ( document->svg_document_لمائی == 0 )
    {
      error = FT_THROW( Invalid_تھاں_Handle );
      اذهب Exit;
    }

    /* allocate a new document */
    doc_لمائی = document->svg_document_لمائی;
    إذا ( FT_QALLOC( glyph->svg_document, doc_لمائی ) )
      اذهب Exit;
    glyph->svg_document_لمائی = doc_لمائی;

    glyph->glyph_اشاریہ = تھاں->glyph_اشاریہ;

    glyph->metrics      = document->metrics;
    glyph->units_فی_EM = document->units_فی_EM;

    glyph->شروع_glyph_id = document->شروع_glyph_id;
    glyph->سرا_glyph_id   = document->سرا_glyph_id;

    glyph->transform = document->transform;
    glyph->delta     = document->delta;

    /* copy the document into glyph */
    FT_MEM_COPY( glyph->svg_document, document->svg_document, doc_لمائی );

  Exit:
    أرجع error;
  }


  FT_CALLBACK_DEF( فراغ )
  ft_svg_glyph_پورا( FT_Glyph  svg_glyph )
  {
    FT_SvgGlyph  glyph  = (FT_SvgGlyph)svg_glyph;
    FT_حافظہ    حافظہ = svg_glyph->library->حافظہ;


    /* just free the memory */
    FT_FREE( glyph->svg_document );
  }


  FT_CALLBACK_DEF( FT_Error )
  ft_svg_glyph_copy( FT_Glyph  svg_سروت,
                     FT_Glyph  svg_نشانہ )
  {
    FT_SvgGlyph  سروت = (FT_SvgGlyph)svg_سروت;
    FT_SvgGlyph  نشانہ = (FT_SvgGlyph)svg_نشانہ;

    FT_Error   error  = FT_Err_Ok;
    FT_حافظہ  حافظہ = FT_GLYPH( سروت )->library->حافظہ;


    إذا ( svg_سروت->format != FT_GLYPH_FORMAT_SVG )
    {
      error = FT_THROW( Invalid_Glyph_Format );
      اذهب Exit;
    }

    إذا ( سروت->svg_document_لمائی == 0 )
    {
      error = FT_THROW( Invalid_تھاں_Handle );
      اذهب Exit;
    }

    نشانہ->glyph_اشاریہ = سروت->glyph_اشاریہ;

    نشانہ->svg_document_لمائی = سروت->svg_document_لمائی;

    نشانہ->metrics      = سروت->metrics;
    نشانہ->units_فی_EM = سروت->units_فی_EM;

    نشانہ->شروع_glyph_id = سروت->شروع_glyph_id;
    نشانہ->سرا_glyph_id   = سروت->سرا_glyph_id;

    نشانہ->transform = سروت->transform;
    نشانہ->delta     = سروت->delta;

    /* allocate space for the SVG document */
    إذا ( FT_QALLOC( نشانہ->svg_document, نشانہ->svg_document_لمائی ) )
      اذهب Exit;

    /* copy the document */
    FT_MEM_COPY( نشانہ->svg_document,
                 سروت->svg_document,
                 نشانہ->svg_document_لمائی );

  Exit:
    أرجع error;
  }


  FT_CALLBACK_DEF( فراغ )
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


    إذا ( !میٹرکس )
    {
      tmp_میٹرکس.xx = 0x10000;
      tmp_میٹرکس.xy = 0;
      tmp_میٹرکس.yx = 0;
      tmp_میٹرکس.yy = 0x10000;

      میٹرکس = &tmp_میٹرکس;
    }

    إذا ( !delta )
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
                        FT_Glyph_تھاں  تھاں )
  {
    FT_SvgGlyph  glyph = (FT_SvgGlyph)svg_glyph;

    FT_Error   error  = FT_Err_Ok;
    FT_حافظہ  حافظہ = svg_glyph->library->حافظہ;

    FT_SVG_Document  document = عدم;


    إذا ( FT_NEW( document ) )
      أرجع error;

    document->svg_document        = glyph->svg_document;
    document->svg_document_لمائی = glyph->svg_document_لمائی;

    document->metrics      = glyph->metrics;
    document->units_فی_EM = glyph->units_فی_EM;

    document->شروع_glyph_id = glyph->شروع_glyph_id;
    document->سرا_glyph_id   = glyph->سرا_glyph_id;

    document->transform = glyph->transform;
    document->delta     = glyph->delta;

    تھاں->format      = FT_GLYPH_FORMAT_SVG;
    تھاں->glyph_اشاریہ = glyph->glyph_اشاریہ;
    تھاں->other       = document;

    أرجع error;
  }


  FT_DEFINE_GLYPH(
    ft_svg_glyph_class,

    حجم ( FT_SvgGlyphRec ),
    FT_GLYPH_FORMAT_SVG,

    ft_svg_glyph_init,      /* FT_Glyph_InitFunc       glyph_init      */
    ft_svg_glyph_پورا,      /* FT_Glyph_DoneFunc       glyph_done      */
    ft_svg_glyph_copy,      /* FT_Glyph_CopyFunc       glyph_copy      */
    ft_svg_glyph_transform, /* FT_Glyph_TransformFunc  glyph_transform */
    عدم,                   /* FT_Glyph_GetBBoxFunc    glyph_bbox      */
    ft_svg_glyph_prepare    /* FT_Glyph_PrepareFunc    glyph_prepare   */
  )

#جے_انت /* FT_CONFIG_OPTION_SVG */


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
     FT_حافظہ  حافظہ = library->حافظہ;
     FT_Error   error;
     FT_Glyph   glyph  = عدم;


     *aglyph = عدم;

     إذا ( !FT_ALLOC( glyph, clazz->glyph_ناپ ) )
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
  FT_Glyph_Copy( FT_Glyph   سروت,
                 FT_Glyph  *نشانہ )
  {
    FT_Glyph               copy;
    FT_Error               error;
    ثابت FT_Glyph_Class*  clazz;


    /* check arguments */
    إذا ( !نشانہ || !سروت || !سروت->clazz )
    {
      error = FT_THROW( Invalid_دلیل );
      اذهب Exit;
    }

    *نشانہ = عدم;

    إذا ( !سروت || !سروت->clazz )
    {
      error = FT_THROW( Invalid_دلیل );
      اذهب Exit;
    }

    clazz = سروت->clazz;
    error = ft_new_glyph( سروت->library, clazz, &copy );
    إذا ( error )
      اذهب Exit;

    copy->advance = سروت->advance;
    copy->format  = سروت->format;

    إذا ( clazz->glyph_copy )
      error = clazz->glyph_copy( سروت, copy );

    إذا ( error )
      FT_پورا_Glyph( copy );
    وإلا
      *نشانہ = copy;

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

#جے_تعریف_شدہ FT_CONFIG_OPTION_SVG
    /* if it is an SVG glyph */
    وإلا إذا ( format == FT_GLYPH_FORMAT_SVG )
      clazz = &ft_svg_glyph_class;
#جے_انت

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
  FT_Get_Glyph( FT_Glyph_تھاں  تھاں,
                FT_Glyph     *aglyph )
  {
    FT_Error  error;
    FT_Glyph  glyph;


    إذا ( !تھاں )
      أرجع FT_THROW( Invalid_تھاں_Handle );

    إذا ( !aglyph )
      أرجع FT_THROW( Invalid_دلیل );

    /* create FT_Glyph object */
    error = FT_New_Glyph( تھاں->library, تھاں->format, &glyph );
    إذا ( error )
      اذهب Exit;

    /* copy advance while converting 26.6 to 16.16 format */
    إذا ( تھاں->advance.x >=  0x8000L * 64 ||
         تھاں->advance.x <= -0x8000L * 64 )
    {
      FT_ERROR(( "FT_Get_Glyph: advance width too large\n" ));
      error = FT_THROW( Invalid_دلیل );
      اذهب Exit2;
    }
    إذا ( تھاں->advance.y >=  0x8000L * 64 ||
         تھاں->advance.y <= -0x8000L * 64 )
    {
      FT_ERROR(( "FT_Get_Glyph: advance height too large\n" ));
      error = FT_THROW( Invalid_دلیل );
      اذهب Exit2;
    }

    glyph->advance.x = تھاں->advance.x * 1024;
    glyph->advance.y = تھاں->advance.y * 1024;

    /* now import the image from the glyph slot */
    error = glyph->clazz->glyph_init( glyph, تھاں );

  Exit2:
    /* if an error occurred, destroy the glyph */
    إذا ( error )
    {
      FT_پورا_Glyph( glyph );
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
                      ثابت FT_میٹرکس*  میٹرکس,
                      ثابت FT_سمتیہ*  delta )
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
        clazz->glyph_transform( glyph, میٹرکس, delta );

        /* transform advance vector */
        إذا ( میٹرکس )
          FT_سمتیہ_Transform( &glyph->advance, میٹرکس );
      }
      وإلا
        error = FT_THROW( Invalid_Glyph_Format );
    }
    أرجع error;
  }


  /* documentation is in ftglyph.h */

  FT_EXPORT_DEF( فراغ )
  FT_Glyph_Get_CBox( FT_Glyph  glyph,
                     FT_UInt   bbox_منوال,
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
    إذا ( bbox_منوال == FT_GLYPH_BBOX_GRIDFIT ||
         bbox_منوال == FT_GLYPH_BBOX_PIXELS  )
    {
      acbox->xMin = FT_PIX_FLOOR( acbox->xMin );
      acbox->yMin = FT_PIX_FLOOR( acbox->yMin );
      acbox->xMax = FT_PIX_CEIL_LONG( acbox->xMax );
      acbox->yMax = FT_PIX_CEIL_LONG( acbox->yMax );
    }

    /* convert to integer pixels if needed */
    إذا ( bbox_منوال == FT_GLYPH_BBOX_TRUNCATE ||
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
  FT_Glyph_منزل_Bitmap( FT_Glyph*         the_glyph,
                      FT_Render_منوال    render_منوال,
                      ثابت FT_سمتیہ*  origin,
                      FT_Bool           destroy )
  {
    FT_Glyph_تھاں_Rec           dummy;
    FT_Glyph_تھاں_اندرونی_Rec  dummy_اندرونی;
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
    FT_ZERO( &dummy_اندرونی );
    dummy.اندرونی = &dummy_اندرونی;
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
    FT_بن_ورتیا( origin );
#جے_انت

    /* prepare dummy slot for rendering */
    error = clazz->glyph_prepare( glyph, &dummy );
    إذا ( !error )
      error = FT_Render_Glyph_اندرونی( glyph->library, &dummy, render_منوال );

#جے_تعریف_شدہ FT_CONFIG_OPTION_SVG
    إذا ( clazz == &ft_svg_glyph_class )
    {
      FT_حافظہ  حافظہ = library->حافظہ;


      FT_FREE( dummy.other );
    }
#جے_انت

#إذا 1
    إذا ( !destroy && origin )
    {
      FT_سمتیہ  v;


      v.x = -origin->x;
      v.y = -origin->y;
      FT_Glyph_Transform( glyph, عدم, &v );
    }
#جے_انت

    إذا ( error )
      اذهب Exit;

    /* in case of success, copy the bitmap to the glyph bitmap */
    bitmap->بایاں   = dummy.bitmap_بایاں;
    bitmap->چوٹی    = dummy.bitmap_چوٹی;
    bitmap->bitmap = dummy.bitmap;

    /* copy advance */
    bitmap->جڑ.advance = glyph->advance;

    إذا ( destroy )
      FT_پورا_Glyph( glyph );

    *the_glyph = FT_GLYPH( bitmap );

  Exit:
    إذا ( error && bitmap )
      FT_پورا_Glyph( FT_GLYPH( bitmap ) );

    أرجع error;

  Bad:
    error = FT_THROW( Invalid_دلیل );
    اذهب Exit;
  }


  /* documentation is in ftglyph.h */

  FT_EXPORT_DEF( فراغ )
  FT_پورا_Glyph( FT_Glyph  glyph )
  {
    إذا ( glyph )
    {
      FT_حافظہ              حافظہ = glyph->library->حافظہ;
      ثابت FT_Glyph_Class*  clazz  = glyph->clazz;


      إذا ( clazz->glyph_پورا )
        clazz->glyph_پورا( glyph );

      FT_FREE( glyph );
    }
  }


/*سرا*/
