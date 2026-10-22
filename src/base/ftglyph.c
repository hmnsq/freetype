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
#لغو_تعریف  FT_COMPONENT
#تعریف_کن FT_COMPONENT  glyph


  /*************************************************************************/
  /*************************************************************************/
  /****                                                                 ****/
  /****   FT_BitmapGlyph support                                        ****/
  /****                                                                 ****/
  /*************************************************************************/
  /*************************************************************************/

  FT_CALLBACK_DEF( FT_Error )
  ft_bitmap_glyph_init( FT_Glyph      bitmap_glyph,
                        FT_Glyph_جایگاه  جایگاه )
  {
    FT_BitmapGlyph  glyph   = (FT_BitmapGlyph)bitmap_glyph;
    FT_Error        error;
    FT_Library      library = FT_GLYPH( glyph )->library;


    إذا ( جایگاه->format != FT_GLYPH_FORMAT_BITMAP )
    {
      error = FT_THROW( Invalid_Glyph_Format );
      اذهب Exit;
    }

    glyph->چپ = جایگاه->bitmap_چپ;
    glyph->بالاترین  = جایگاه->bitmap_بالاترین;

    FT_Bitmap_Init( &glyph->bitmap );
    error = FT_Bitmap_Copy( library, &جایگاه->bitmap, &glyph->bitmap );

  Exit:
    أرجع error;
  }


  FT_CALLBACK_DEF( FT_Error )
  ft_bitmap_glyph_copy( FT_Glyph  bitmap_منبع,
                        FT_Glyph  bitmap_هدف )
  {
    FT_Library      library = bitmap_منبع->library;
    FT_BitmapGlyph  منبع  = (FT_BitmapGlyph)bitmap_منبع;
    FT_BitmapGlyph  هدف  = (FT_BitmapGlyph)bitmap_هدف;


    هدف->چپ = منبع->چپ;
    هدف->بالاترین  = منبع->بالاترین;

    أرجع FT_Bitmap_Copy( library, &منبع->bitmap, &هدف->bitmap );
  }


  FT_CALLBACK_DEF( فراغ )
  ft_bitmap_glyph_انجامشده( FT_Glyph  bitmap_glyph )
  {
    FT_BitmapGlyph  glyph   = (FT_BitmapGlyph)bitmap_glyph;
    FT_Library      library = FT_GLYPH( glyph )->library;


    FT_Bitmap_انجامشده( library, &glyph->bitmap );
  }


  FT_CALLBACK_DEF( فراغ )
  ft_bitmap_glyph_bbox( FT_Glyph  bitmap_glyph,
                        FT_BBox*  cbox )
  {
    FT_BitmapGlyph  glyph = (FT_BitmapGlyph)bitmap_glyph;


    cbox->xMin = glyph->چپ * 64;
    cbox->xMax = cbox->xMin + (FT_Pos)( glyph->bitmap.عرض * 64 );
    cbox->yMax = glyph->بالاترین * 64;
    cbox->yMin = cbox->yMax - (FT_Pos)( glyph->bitmap.ردیفها * 64 );
  }


  FT_DEFINE_GLYPH(
    ft_bitmap_glyph_class,

    حجم ( FT_BitmapGlyphRec ),
    FT_GLYPH_FORMAT_BITMAP,

    ft_bitmap_glyph_init,    /* FT_Glyph_InitFunc       glyph_init      */
    ft_bitmap_glyph_انجامشده,    /* FT_Glyph_DoneFunc       glyph_done      */
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
                         FT_Glyph_جایگاه  جایگاه )
  {
    FT_OutlineGlyph  glyph   = (FT_OutlineGlyph)outline_glyph;
    FT_Error         error   = FT_Err_Ok;
    FT_Library       library = FT_GLYPH( glyph )->library;
    FT_Outline*      منبع  = &جایگاه->outline;
    FT_Outline*      هدف  = &glyph->outline;


    /* check format in glyph slot */
    إذا ( جایگاه->format != FT_GLYPH_FORMAT_OUTLINE )
    {
      error = FT_THROW( Invalid_Glyph_Format );
      اذهب Exit;
    }

    /* allocate new outline */
    error = FT_Outline_New( library,
                            (FT_UInt)منبع->n_points,
                            منبع->n_contours,
                            &glyph->outline );
    إذا ( error )
      اذهب Exit;

    FT_Outline_Copy( منبع, هدف );

  Exit:
    أرجع error;
  }


  FT_CALLBACK_DEF( فراغ )
  ft_outline_glyph_انجامشده( FT_Glyph  outline_glyph )
  {
    FT_OutlineGlyph  glyph = (FT_OutlineGlyph)outline_glyph;


    FT_Outline_انجامشده( FT_GLYPH( glyph )->library, &glyph->outline );
  }


  FT_CALLBACK_DEF( FT_Error )
  ft_outline_glyph_copy( FT_Glyph  outline_منبع,
                         FT_Glyph  outline_هدف )
  {
    FT_OutlineGlyph  منبع  = (FT_OutlineGlyph)outline_منبع;
    FT_OutlineGlyph  هدف  = (FT_OutlineGlyph)outline_هدف;
    FT_Error         error;
    FT_Library       library = FT_GLYPH( منبع )->library;


    error = FT_Outline_New( library,
                            (FT_UInt)منبع->outline.n_points,
                            منبع->outline.n_contours,
                            &هدف->outline );
    إذا ( !error )
      FT_Outline_Copy( &منبع->outline, &هدف->outline );

    أرجع error;
  }


  FT_CALLBACK_DEF( فراغ )
  ft_outline_glyph_transform( FT_Glyph          outline_glyph,
                              ثابت FT_ماتریس*  ماتریس,
                              ثابت FT_وکتور*  delta )
  {
    FT_OutlineGlyph  glyph = (FT_OutlineGlyph)outline_glyph;


    إذا ( ماتریس )
      FT_Outline_Transform( &glyph->outline, ماتریس );

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
                            FT_Glyph_جایگاه  جایگاه )
  {
    FT_OutlineGlyph  glyph = (FT_OutlineGlyph)outline_glyph;


    جایگاه->format         = FT_GLYPH_FORMAT_OUTLINE;
    جایگاه->outline        = glyph->outline;
    جایگاه->outline.flags &= ~FT_OUTLINE_OWNER;

    أرجع FT_Err_Ok;
  }


  FT_DEFINE_GLYPH(
    ft_outline_glyph_class,

    حجم ( FT_OutlineGlyphRec ),
    FT_GLYPH_FORMAT_OUTLINE,

    ft_outline_glyph_init,      /* FT_Glyph_InitFunc       glyph_init      */
    ft_outline_glyph_انجامشده,      /* FT_Glyph_DoneFunc       glyph_done      */
    ft_outline_glyph_copy,      /* FT_Glyph_CopyFunc       glyph_copy      */
    ft_outline_glyph_transform, /* FT_Glyph_TransformFunc  glyph_transform */
    ft_outline_glyph_bbox,      /* FT_Glyph_GetBBoxFunc    glyph_bbox      */
    ft_outline_glyph_prepare    /* FT_Glyph_PrepareFunc    glyph_prepare   */
  )


#اگر_تعریف_شده FT_CONFIG_OPTION_SVG

  /*************************************************************************/
  /*************************************************************************/
  /****                                                                 ****/
  /****   FT_SvgGlyph support                                           ****/
  /****                                                                 ****/
  /*************************************************************************/
  /*************************************************************************/


  FT_CALLBACK_DEF( FT_Error )
  ft_svg_glyph_init( FT_Glyph      svg_glyph,
                     FT_Glyph_جایگاه  جایگاه )
  {
    FT_ULong         doc_درازی;
    FT_SVG_Document  document;
    FT_SvgGlyph      glyph = (FT_SvgGlyph)svg_glyph;

    FT_Error   error  = FT_Err_Ok;
    FT_حافظه  حافظه = FT_GLYPH( glyph )->library->حافظه;


    إذا ( جایگاه->format != FT_GLYPH_FORMAT_SVG )
    {
      error = FT_THROW( Invalid_Glyph_Format );
      اذهب Exit;
    }

    إذا ( جایگاه->other == عدم )
    {
      error = FT_THROW( Invalid_جایگاه_Handle );
      اذهب Exit;
    }

    document = (FT_SVG_Document)جایگاه->other;

    إذا ( document->svg_document_درازی == 0 )
    {
      error = FT_THROW( Invalid_جایگاه_Handle );
      اذهب Exit;
    }

    /* allocate a new document */
    doc_درازی = document->svg_document_درازی;
    إذا ( FT_QALLOC( glyph->svg_document, doc_درازی ) )
      اذهب Exit;
    glyph->svg_document_درازی = doc_درازی;

    glyph->glyph_شاخص = جایگاه->glyph_شاخص;

    glyph->metrics      = document->metrics;
    glyph->units_در_هر_EM = document->units_در_هر_EM;

    glyph->شروع_glyph_id = document->شروع_glyph_id;
    glyph->انتها_glyph_id   = document->انتها_glyph_id;

    glyph->transform = document->transform;
    glyph->delta     = document->delta;

    /* copy the document into glyph */
    FT_MEM_COPY( glyph->svg_document, document->svg_document, doc_درازی );

  Exit:
    أرجع error;
  }


  FT_CALLBACK_DEF( فراغ )
  ft_svg_glyph_انجامشده( FT_Glyph  svg_glyph )
  {
    FT_SvgGlyph  glyph  = (FT_SvgGlyph)svg_glyph;
    FT_حافظه    حافظه = svg_glyph->library->حافظه;


    /* just free the memory */
    FT_FREE( glyph->svg_document );
  }


  FT_CALLBACK_DEF( FT_Error )
  ft_svg_glyph_copy( FT_Glyph  svg_منبع,
                     FT_Glyph  svg_هدف )
  {
    FT_SvgGlyph  منبع = (FT_SvgGlyph)svg_منبع;
    FT_SvgGlyph  هدف = (FT_SvgGlyph)svg_هدف;

    FT_Error   error  = FT_Err_Ok;
    FT_حافظه  حافظه = FT_GLYPH( منبع )->library->حافظه;


    إذا ( svg_منبع->format != FT_GLYPH_FORMAT_SVG )
    {
      error = FT_THROW( Invalid_Glyph_Format );
      اذهب Exit;
    }

    إذا ( منبع->svg_document_درازی == 0 )
    {
      error = FT_THROW( Invalid_جایگاه_Handle );
      اذهب Exit;
    }

    هدف->glyph_شاخص = منبع->glyph_شاخص;

    هدف->svg_document_درازی = منبع->svg_document_درازی;

    هدف->metrics      = منبع->metrics;
    هدف->units_در_هر_EM = منبع->units_در_هر_EM;

    هدف->شروع_glyph_id = منبع->شروع_glyph_id;
    هدف->انتها_glyph_id   = منبع->انتها_glyph_id;

    هدف->transform = منبع->transform;
    هدف->delta     = منبع->delta;

    /* allocate space for the SVG document */
    إذا ( FT_QALLOC( هدف->svg_document, هدف->svg_document_درازی ) )
      اذهب Exit;

    /* copy the document */
    FT_MEM_COPY( هدف->svg_document,
                 منبع->svg_document,
                 هدف->svg_document_درازی );

  Exit:
    أرجع error;
  }


  FT_CALLBACK_DEF( فراغ )
  ft_svg_glyph_transform( FT_Glyph          svg_glyph,
                          ثابت FT_ماتریس*  ماتریس_2,
                          ثابت FT_وکتور*  _delta )
  {
    FT_SvgGlyph  glyph  = (FT_SvgGlyph)svg_glyph;
    FT_ماتریس*   ماتریس = (FT_ماتریس*)ماتریس_2;
    FT_وکتور*   delta  = (FT_وکتور*)_delta;

    FT_ماتریس  tmp_ماتریس;
    FT_وکتور  tmp_delta;

    FT_ماتریس  a, b;
    FT_Pos     x, y;


    إذا ( !ماتریس )
    {
      tmp_ماتریس.xx = 0x10000;
      tmp_ماتریس.xy = 0;
      tmp_ماتریس.yx = 0;
      tmp_ماتریس.yy = 0x10000;

      ماتریس = &tmp_ماتریس;
    }

    إذا ( !delta )
    {
      tmp_delta.x = 0;
      tmp_delta.y = 0;

      delta = &tmp_delta;
    }

    a = glyph->transform;
    b = *ماتریس;
    FT_ماتریس_Multiply( &b, &a );

    x = جمع_کن_LONG( جمع_کن_LONG( FT_MulFix( ماتریس->xx, glyph->delta.x ),
                            FT_MulFix( ماتریس->xy, glyph->delta.y ) ),
                  delta->x );
    y = جمع_کن_LONG( جمع_کن_LONG( FT_MulFix( ماتریس->yx, glyph->delta.x ),
                            FT_MulFix( ماتریس->yy, glyph->delta.y ) ),
                  delta->y );

    glyph->delta.x = x;
    glyph->delta.y = y;

    glyph->transform = a;
  }


  FT_CALLBACK_DEF( FT_Error )
  ft_svg_glyph_prepare( FT_Glyph      svg_glyph,
                        FT_Glyph_جایگاه  جایگاه )
  {
    FT_SvgGlyph  glyph = (FT_SvgGlyph)svg_glyph;

    FT_Error   error  = FT_Err_Ok;
    FT_حافظه  حافظه = svg_glyph->library->حافظه;

    FT_SVG_Document  document = عدم;


    إذا ( FT_NEW( document ) )
      أرجع error;

    document->svg_document        = glyph->svg_document;
    document->svg_document_درازی = glyph->svg_document_درازی;

    document->metrics      = glyph->metrics;
    document->units_در_هر_EM = glyph->units_در_هر_EM;

    document->شروع_glyph_id = glyph->شروع_glyph_id;
    document->انتها_glyph_id   = glyph->انتها_glyph_id;

    document->transform = glyph->transform;
    document->delta     = glyph->delta;

    جایگاه->format      = FT_GLYPH_FORMAT_SVG;
    جایگاه->glyph_شاخص = glyph->glyph_شاخص;
    جایگاه->other       = document;

    أرجع error;
  }


  FT_DEFINE_GLYPH(
    ft_svg_glyph_class,

    حجم ( FT_SvgGlyphRec ),
    FT_GLYPH_FORMAT_SVG,

    ft_svg_glyph_init,      /* FT_Glyph_InitFunc       glyph_init      */
    ft_svg_glyph_انجامشده,      /* FT_Glyph_DoneFunc       glyph_done      */
    ft_svg_glyph_copy,      /* FT_Glyph_CopyFunc       glyph_copy      */
    ft_svg_glyph_transform, /* FT_Glyph_TransformFunc  glyph_transform */
    عدم,                   /* FT_Glyph_GetBBoxFunc    glyph_bbox      */
    ft_svg_glyph_prepare    /* FT_Glyph_PrepareFunc    glyph_prepare   */
  )

#پایان_اگر /* FT_CONFIG_OPTION_SVG */


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
  FT_Glyph_Copy( FT_Glyph   منبع,
                 FT_Glyph  *هدف )
  {
    FT_Glyph               copy;
    FT_Error               error;
    ثابت FT_Glyph_Class*  clazz;


    /* check arguments */
    إذا ( !هدف || !منبع || !منبع->clazz )
    {
      error = FT_THROW( Invalid_آرگومان );
      اذهب Exit;
    }

    *هدف = عدم;

    إذا ( !منبع || !منبع->clazz )
    {
      error = FT_THROW( Invalid_آرگومان );
      اذهب Exit;
    }

    clazz = منبع->clazz;
    error = ft_new_glyph( منبع->library, clazz, &copy );
    إذا ( error )
      اذهب Exit;

    copy->advance = منبع->advance;
    copy->format  = منبع->format;

    إذا ( clazz->glyph_copy )
      error = clazz->glyph_copy( منبع, copy );

    إذا ( error )
      FT_انجامشده_Glyph( copy );
    وإلا
      *هدف = copy;

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
      أرجع FT_THROW( Invalid_آرگومان );

    /* if it is a bitmap, that's easy :-) */
    إذا ( format == FT_GLYPH_FORMAT_BITMAP )
      clazz = &ft_bitmap_glyph_class;

    /* if it is an outline */
    وإلا إذا ( format == FT_GLYPH_FORMAT_OUTLINE )
      clazz = &ft_outline_glyph_class;

#اگر_تعریف_شده FT_CONFIG_OPTION_SVG
    /* if it is an SVG glyph */
    وإلا إذا ( format == FT_GLYPH_FORMAT_SVG )
      clazz = &ft_svg_glyph_class;
#پایان_اگر

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
  FT_Get_Glyph( FT_Glyph_جایگاه  جایگاه,
                FT_Glyph     *aglyph )
  {
    FT_Error  error;
    FT_Glyph  glyph;


    إذا ( !جایگاه )
      أرجع FT_THROW( Invalid_جایگاه_Handle );

    إذا ( !aglyph )
      أرجع FT_THROW( Invalid_آرگومان );

    /* create FT_Glyph object */
    error = FT_New_Glyph( جایگاه->library, جایگاه->format, &glyph );
    إذا ( error )
      اذهب Exit;

    /* copy advance while converting 26.6 to 16.16 format */
    إذا ( جایگاه->advance.x >=  0x8000L * 64 ||
         جایگاه->advance.x <= -0x8000L * 64 )
    {
      FT_ERROR(( "FT_Get_Glyph: advance width too large\n" ));
      error = FT_THROW( Invalid_آرگومان );
      اذهب Exit2;
    }
    إذا ( جایگاه->advance.y >=  0x8000L * 64 ||
         جایگاه->advance.y <= -0x8000L * 64 )
    {
      FT_ERROR(( "FT_Get_Glyph: advance height too large\n" ));
      error = FT_THROW( Invalid_آرگومان );
      اذهب Exit2;
    }

    glyph->advance.x = جایگاه->advance.x * 1024;
    glyph->advance.y = جایگاه->advance.y * 1024;

    /* now import the image from the glyph slot */
    error = glyph->clazz->glyph_init( glyph, جایگاه );

  Exit2:
    /* if an error occurred, destroy the glyph */
    إذا ( error )
    {
      FT_انجامشده_Glyph( glyph );
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
                      ثابت FT_ماتریس*  ماتریس,
                      ثابت FT_وکتور*  delta )
  {
    FT_Error  error = FT_Err_Ok;


    إذا ( !glyph || !glyph->clazz )
      error = FT_THROW( Invalid_آرگومان );
    وإلا
    {
      ثابت FT_Glyph_Class*  clazz = glyph->clazz;


      إذا ( clazz->glyph_transform )
      {
        /* transform glyph image */
        clazz->glyph_transform( glyph, ماتریس, delta );

        /* transform advance vector */
        إذا ( ماتریس )
          FT_وکتور_Transform( &glyph->advance, ماتریس );
      }
      وإلا
        error = FT_THROW( Invalid_Glyph_Format );
    }
    أرجع error;
  }


  /* documentation is in ftglyph.h */

  FT_EXPORT_DEF( فراغ )
  FT_Glyph_Get_CBox( FT_Glyph  glyph,
                     FT_UInt   bbox_نما,
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
    إذا ( bbox_نما == FT_GLYPH_BBOX_GRIDFIT ||
         bbox_نما == FT_GLYPH_BBOX_PIXELS  )
    {
      acbox->xMin = FT_PIX_FLOOR( acbox->xMin );
      acbox->yMin = FT_PIX_FLOOR( acbox->yMin );
      acbox->xMax = FT_PIX_CEIL_LONG( acbox->xMax );
      acbox->yMax = FT_PIX_CEIL_LONG( acbox->yMax );
    }

    /* convert to integer pixels if needed */
    إذا ( bbox_نما == FT_GLYPH_BBOX_TRUNCATE ||
         bbox_نما == FT_GLYPH_BBOX_PIXELS   )
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
                      FT_Render_نما    render_نما,
                      ثابت FT_وکتور*  origin,
                      FT_Bool           destroy )
  {
    FT_Glyph_جایگاه_Rec           dummy;
    FT_Glyph_جایگاه_داخلی_Rec  dummy_داخلی;
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
    FT_ZERO( &dummy_داخلی );
    dummy.داخلی = &dummy_داخلی;
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
    FT_استفادهنشده( origin );
#پایان_اگر

    /* prepare dummy slot for rendering */
    error = clazz->glyph_prepare( glyph, &dummy );
    إذا ( !error )
      error = FT_Render_Glyph_داخلی( glyph->library, &dummy, render_نما );

#اگر_تعریف_شده FT_CONFIG_OPTION_SVG
    إذا ( clazz == &ft_svg_glyph_class )
    {
      FT_حافظه  حافظه = library->حافظه;


      FT_FREE( dummy.other );
    }
#پایان_اگر

#إذا 1
    إذا ( !destroy && origin )
    {
      FT_وکتور  v;


      v.x = -origin->x;
      v.y = -origin->y;
      FT_Glyph_Transform( glyph, عدم, &v );
    }
#پایان_اگر

    إذا ( error )
      اذهب Exit;

    /* in case of success, copy the bitmap to the glyph bitmap */
    bitmap->چپ   = dummy.bitmap_چپ;
    bitmap->بالاترین    = dummy.bitmap_بالاترین;
    bitmap->bitmap = dummy.bitmap;

    /* copy advance */
    bitmap->ریشه.advance = glyph->advance;

    إذا ( destroy )
      FT_انجامشده_Glyph( glyph );

    *the_glyph = FT_GLYPH( bitmap );

  Exit:
    إذا ( error && bitmap )
      FT_انجامشده_Glyph( FT_GLYPH( bitmap ) );

    أرجع error;

  Bad:
    error = FT_THROW( Invalid_آرگومان );
    اذهب Exit;
  }


  /* documentation is in ftglyph.h */

  FT_EXPORT_DEF( فراغ )
  FT_انجامشده_Glyph( FT_Glyph  glyph )
  {
    إذا ( glyph )
    {
      FT_حافظه              حافظه = glyph->library->حافظه;
      ثابت FT_Glyph_Class*  clazz  = glyph->clazz;


      إذا ( clazz->glyph_انجامشده )
        clazz->glyph_انجامشده( glyph );

      FT_FREE( glyph );
    }
  }


/*انتها*/
