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
#وصف_رد  FT_COMPONENT
#وصف_ڪريو FT_COMPONENT  glyph


  /*************************************************************************/
  /*************************************************************************/
  /****                                                                 ****/
  /****   FT_BitmapGlyph support                                        ****/
  /****                                                                 ****/
  /*************************************************************************/
  /*************************************************************************/

  FT_CALLBACK_DEF( FT_Error )
  ft_bitmap_glyph_init( FT_Glyph      bitmap_glyph,
                        FT_Glyph_جڳهه  جڳهه )
  {
    FT_BitmapGlyph  glyph   = (FT_BitmapGlyph)bitmap_glyph;
    FT_Error        error;
    FT_Library      library = FT_GLYPH( glyph )->library;


    إذا ( جڳهه->format != FT_GLYPH_FORMAT_BITMAP )
    {
      error = FT_THROW( Invalid_Glyph_Format );
      اذهب Exit;
    }

    glyph->کاٻو = جڳهه->bitmap_کاٻو;
    glyph->چوٽي  = جڳهه->bitmap_چوٽي;

    FT_Bitmap_Init( &glyph->bitmap );
    error = FT_Bitmap_Copy( library, &جڳهه->bitmap, &glyph->bitmap );

  Exit:
    أرجع error;
  }


  FT_CALLBACK_DEF( FT_Error )
  ft_bitmap_glyph_copy( FT_Glyph  bitmap_ذريعو,
                        FT_Glyph  bitmap_هدف )
  {
    FT_Library      library = bitmap_ذريعو->library;
    FT_BitmapGlyph  ذريعو  = (FT_BitmapGlyph)bitmap_ذريعو;
    FT_BitmapGlyph  هدف  = (FT_BitmapGlyph)bitmap_هدف;


    هدف->کاٻو = ذريعو->کاٻو;
    هدف->چوٽي  = ذريعو->چوٽي;

    أرجع FT_Bitmap_Copy( library, &ذريعو->bitmap, &هدف->bitmap );
  }


  FT_CALLBACK_DEF( فراغ )
  ft_bitmap_glyph_پورو( FT_Glyph  bitmap_glyph )
  {
    FT_BitmapGlyph  glyph   = (FT_BitmapGlyph)bitmap_glyph;
    FT_Library      library = FT_GLYPH( glyph )->library;


    FT_Bitmap_پورو( library, &glyph->bitmap );
  }


  FT_CALLBACK_DEF( فراغ )
  ft_bitmap_glyph_bbox( FT_Glyph  bitmap_glyph,
                        FT_BBox*  cbox )
  {
    FT_BitmapGlyph  glyph = (FT_BitmapGlyph)bitmap_glyph;


    cbox->xMin = glyph->کاٻو * 64;
    cbox->xMax = cbox->xMin + (FT_Pos)( glyph->bitmap.ويڪر * 64 );
    cbox->yMax = glyph->چوٽي * 64;
    cbox->yMin = cbox->yMax - (FT_Pos)( glyph->bitmap.صفون * 64 );
  }


  FT_DEFINE_GLYPH(
    ft_bitmap_glyph_class,

    حجم ( FT_BitmapGlyphRec ),
    FT_GLYPH_FORMAT_BITMAP,

    ft_bitmap_glyph_init,    /* FT_Glyph_InitFunc       glyph_init      */
    ft_bitmap_glyph_پورو,    /* FT_Glyph_DoneFunc       glyph_done      */
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
                         FT_Glyph_جڳهه  جڳهه )
  {
    FT_OutlineGlyph  glyph   = (FT_OutlineGlyph)outline_glyph;
    FT_Error         error   = FT_Err_Ok;
    FT_Library       library = FT_GLYPH( glyph )->library;
    FT_Outline*      ذريعو  = &جڳهه->outline;
    FT_Outline*      هدف  = &glyph->outline;


    /* check format in glyph slot */
    إذا ( جڳهه->format != FT_GLYPH_FORMAT_OUTLINE )
    {
      error = FT_THROW( Invalid_Glyph_Format );
      اذهب Exit;
    }

    /* allocate new outline */
    error = FT_Outline_New( library,
                            (FT_UInt)ذريعو->n_points,
                            ذريعو->n_contours,
                            &glyph->outline );
    إذا ( error )
      اذهب Exit;

    FT_Outline_Copy( ذريعو, هدف );

  Exit:
    أرجع error;
  }


  FT_CALLBACK_DEF( فراغ )
  ft_outline_glyph_پورو( FT_Glyph  outline_glyph )
  {
    FT_OutlineGlyph  glyph = (FT_OutlineGlyph)outline_glyph;


    FT_Outline_پورو( FT_GLYPH( glyph )->library, &glyph->outline );
  }


  FT_CALLBACK_DEF( FT_Error )
  ft_outline_glyph_copy( FT_Glyph  outline_ذريعو,
                         FT_Glyph  outline_هدف )
  {
    FT_OutlineGlyph  ذريعو  = (FT_OutlineGlyph)outline_ذريعو;
    FT_OutlineGlyph  هدف  = (FT_OutlineGlyph)outline_هدف;
    FT_Error         error;
    FT_Library       library = FT_GLYPH( ذريعو )->library;


    error = FT_Outline_New( library,
                            (FT_UInt)ذريعو->outline.n_points,
                            ذريعو->outline.n_contours,
                            &هدف->outline );
    إذا ( !error )
      FT_Outline_Copy( &ذريعو->outline, &هدف->outline );

    أرجع error;
  }


  FT_CALLBACK_DEF( فراغ )
  ft_outline_glyph_transform( FT_Glyph          outline_glyph,
                              ثابت FT_ميٽرڪس*  ميٽرڪس,
                              ثابت FT_ويڪٽر*  delta )
  {
    FT_OutlineGlyph  glyph = (FT_OutlineGlyph)outline_glyph;


    إذا ( ميٽرڪس )
      FT_Outline_Transform( &glyph->outline, ميٽرڪس );

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
                            FT_Glyph_جڳهه  جڳهه )
  {
    FT_OutlineGlyph  glyph = (FT_OutlineGlyph)outline_glyph;


    جڳهه->format         = FT_GLYPH_FORMAT_OUTLINE;
    جڳهه->outline        = glyph->outline;
    جڳهه->outline.flags &= ~FT_OUTLINE_OWNER;

    أرجع FT_Err_Ok;
  }


  FT_DEFINE_GLYPH(
    ft_outline_glyph_class,

    حجم ( FT_OutlineGlyphRec ),
    FT_GLYPH_FORMAT_OUTLINE,

    ft_outline_glyph_init,      /* FT_Glyph_InitFunc       glyph_init      */
    ft_outline_glyph_پورو,      /* FT_Glyph_DoneFunc       glyph_done      */
    ft_outline_glyph_copy,      /* FT_Glyph_CopyFunc       glyph_copy      */
    ft_outline_glyph_transform, /* FT_Glyph_TransformFunc  glyph_transform */
    ft_outline_glyph_bbox,      /* FT_Glyph_GetBBoxFunc    glyph_bbox      */
    ft_outline_glyph_prepare    /* FT_Glyph_PrepareFunc    glyph_prepare   */
  )


#جيڪڏهن_وصف_ٿيل FT_CONFIG_OPTION_SVG

  /*************************************************************************/
  /*************************************************************************/
  /****                                                                 ****/
  /****   FT_SvgGlyph support                                           ****/
  /****                                                                 ****/
  /*************************************************************************/
  /*************************************************************************/


  FT_CALLBACK_DEF( FT_Error )
  ft_svg_glyph_init( FT_Glyph      svg_glyph,
                     FT_Glyph_جڳهه  جڳهه )
  {
    FT_ULong         doc_ڊيگهائي;
    FT_SVG_Document  document;
    FT_SvgGlyph      glyph = (FT_SvgGlyph)svg_glyph;

    FT_Error   error  = FT_Err_Ok;
    FT_يادگيري  يادگيري = FT_GLYPH( glyph )->library->يادگيري;


    إذا ( جڳهه->format != FT_GLYPH_FORMAT_SVG )
    {
      error = FT_THROW( Invalid_Glyph_Format );
      اذهب Exit;
    }

    إذا ( جڳهه->other == عدم )
    {
      error = FT_THROW( Invalid_جڳهه_Handle );
      اذهب Exit;
    }

    document = (FT_SVG_Document)جڳهه->other;

    إذا ( document->svg_document_ڊيگهائي == 0 )
    {
      error = FT_THROW( Invalid_جڳهه_Handle );
      اذهب Exit;
    }

    /* allocate a new document */
    doc_ڊيگهائي = document->svg_document_ڊيگهائي;
    إذا ( FT_QALLOC( glyph->svg_document, doc_ڊيگهائي ) )
      اذهب Exit;
    glyph->svg_document_ڊيگهائي = doc_ڊيگهائي;

    glyph->glyph_اشاريو = جڳهه->glyph_اشاريو;

    glyph->metrics      = document->metrics;
    glyph->units_في_EM = document->units_في_EM;

    glyph->شروعات_glyph_id = document->شروعات_glyph_id;
    glyph->پڇاڙي_glyph_id   = document->پڇاڙي_glyph_id;

    glyph->transform = document->transform;
    glyph->delta     = document->delta;

    /* copy the document into glyph */
    FT_MEM_COPY( glyph->svg_document, document->svg_document, doc_ڊيگهائي );

  Exit:
    أرجع error;
  }


  FT_CALLBACK_DEF( فراغ )
  ft_svg_glyph_پورو( FT_Glyph  svg_glyph )
  {
    FT_SvgGlyph  glyph  = (FT_SvgGlyph)svg_glyph;
    FT_يادگيري    يادگيري = svg_glyph->library->يادگيري;


    /* just free the memory */
    FT_FREE( glyph->svg_document );
  }


  FT_CALLBACK_DEF( FT_Error )
  ft_svg_glyph_copy( FT_Glyph  svg_ذريعو,
                     FT_Glyph  svg_هدف )
  {
    FT_SvgGlyph  ذريعو = (FT_SvgGlyph)svg_ذريعو;
    FT_SvgGlyph  هدف = (FT_SvgGlyph)svg_هدف;

    FT_Error   error  = FT_Err_Ok;
    FT_يادگيري  يادگيري = FT_GLYPH( ذريعو )->library->يادگيري;


    إذا ( svg_ذريعو->format != FT_GLYPH_FORMAT_SVG )
    {
      error = FT_THROW( Invalid_Glyph_Format );
      اذهب Exit;
    }

    إذا ( ذريعو->svg_document_ڊيگهائي == 0 )
    {
      error = FT_THROW( Invalid_جڳهه_Handle );
      اذهب Exit;
    }

    هدف->glyph_اشاريو = ذريعو->glyph_اشاريو;

    هدف->svg_document_ڊيگهائي = ذريعو->svg_document_ڊيگهائي;

    هدف->metrics      = ذريعو->metrics;
    هدف->units_في_EM = ذريعو->units_في_EM;

    هدف->شروعات_glyph_id = ذريعو->شروعات_glyph_id;
    هدف->پڇاڙي_glyph_id   = ذريعو->پڇاڙي_glyph_id;

    هدف->transform = ذريعو->transform;
    هدف->delta     = ذريعو->delta;

    /* allocate space for the SVG document */
    إذا ( FT_QALLOC( هدف->svg_document, هدف->svg_document_ڊيگهائي ) )
      اذهب Exit;

    /* copy the document */
    FT_MEM_COPY( هدف->svg_document,
                 ذريعو->svg_document,
                 هدف->svg_document_ڊيگهائي );

  Exit:
    أرجع error;
  }


  FT_CALLBACK_DEF( فراغ )
  ft_svg_glyph_transform( FT_Glyph          svg_glyph,
                          ثابت FT_ميٽرڪس*  ميٽرڪس_2,
                          ثابت FT_ويڪٽر*  _delta )
  {
    FT_SvgGlyph  glyph  = (FT_SvgGlyph)svg_glyph;
    FT_ميٽرڪس*   ميٽرڪس = (FT_ميٽرڪس*)ميٽرڪس_2;
    FT_ويڪٽر*   delta  = (FT_ويڪٽر*)_delta;

    FT_ميٽرڪس  tmp_ميٽرڪس;
    FT_ويڪٽر  tmp_delta;

    FT_ميٽرڪس  a, b;
    FT_Pos     x, y;


    إذا ( !ميٽرڪس )
    {
      tmp_ميٽرڪس.xx = 0x10000;
      tmp_ميٽرڪس.xy = 0;
      tmp_ميٽرڪس.yx = 0;
      tmp_ميٽرڪس.yy = 0x10000;

      ميٽرڪس = &tmp_ميٽرڪس;
    }

    إذا ( !delta )
    {
      tmp_delta.x = 0;
      tmp_delta.y = 0;

      delta = &tmp_delta;
    }

    a = glyph->transform;
    b = *ميٽرڪس;
    FT_ميٽرڪس_Multiply( &b, &a );

    x = جوڙيو_LONG( جوڙيو_LONG( FT_MulFix( ميٽرڪس->xx, glyph->delta.x ),
                            FT_MulFix( ميٽرڪس->xy, glyph->delta.y ) ),
                  delta->x );
    y = جوڙيو_LONG( جوڙيو_LONG( FT_MulFix( ميٽرڪس->yx, glyph->delta.x ),
                            FT_MulFix( ميٽرڪس->yy, glyph->delta.y ) ),
                  delta->y );

    glyph->delta.x = x;
    glyph->delta.y = y;

    glyph->transform = a;
  }


  FT_CALLBACK_DEF( FT_Error )
  ft_svg_glyph_prepare( FT_Glyph      svg_glyph,
                        FT_Glyph_جڳهه  جڳهه )
  {
    FT_SvgGlyph  glyph = (FT_SvgGlyph)svg_glyph;

    FT_Error   error  = FT_Err_Ok;
    FT_يادگيري  يادگيري = svg_glyph->library->يادگيري;

    FT_SVG_Document  document = عدم;


    إذا ( FT_NEW( document ) )
      أرجع error;

    document->svg_document        = glyph->svg_document;
    document->svg_document_ڊيگهائي = glyph->svg_document_ڊيگهائي;

    document->metrics      = glyph->metrics;
    document->units_في_EM = glyph->units_في_EM;

    document->شروعات_glyph_id = glyph->شروعات_glyph_id;
    document->پڇاڙي_glyph_id   = glyph->پڇاڙي_glyph_id;

    document->transform = glyph->transform;
    document->delta     = glyph->delta;

    جڳهه->format      = FT_GLYPH_FORMAT_SVG;
    جڳهه->glyph_اشاريو = glyph->glyph_اشاريو;
    جڳهه->other       = document;

    أرجع error;
  }


  FT_DEFINE_GLYPH(
    ft_svg_glyph_class,

    حجم ( FT_SvgGlyphRec ),
    FT_GLYPH_FORMAT_SVG,

    ft_svg_glyph_init,      /* FT_Glyph_InitFunc       glyph_init      */
    ft_svg_glyph_پورو,      /* FT_Glyph_DoneFunc       glyph_done      */
    ft_svg_glyph_copy,      /* FT_Glyph_CopyFunc       glyph_copy      */
    ft_svg_glyph_transform, /* FT_Glyph_TransformFunc  glyph_transform */
    عدم,                   /* FT_Glyph_GetBBoxFunc    glyph_bbox      */
    ft_svg_glyph_prepare    /* FT_Glyph_PrepareFunc    glyph_prepare   */
  )

#جيڪڏهن_پورو /* FT_CONFIG_OPTION_SVG */


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
     FT_يادگيري  يادگيري = library->يادگيري;
     FT_Error   error;
     FT_Glyph   glyph  = عدم;


     *aglyph = عدم;

     إذا ( !FT_ALLOC( glyph, clazz->glyph_سائيز ) )
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
  FT_Glyph_Copy( FT_Glyph   ذريعو,
                 FT_Glyph  *هدف )
  {
    FT_Glyph               copy;
    FT_Error               error;
    ثابت FT_Glyph_Class*  clazz;


    /* check arguments */
    إذا ( !هدف || !ذريعو || !ذريعو->clazz )
    {
      error = FT_THROW( Invalid_دليل );
      اذهب Exit;
    }

    *هدف = عدم;

    إذا ( !ذريعو || !ذريعو->clazz )
    {
      error = FT_THROW( Invalid_دليل );
      اذهب Exit;
    }

    clazz = ذريعو->clazz;
    error = ft_new_glyph( ذريعو->library, clazz, &copy );
    إذا ( error )
      اذهب Exit;

    copy->advance = ذريعو->advance;
    copy->format  = ذريعو->format;

    إذا ( clazz->glyph_copy )
      error = clazz->glyph_copy( ذريعو, copy );

    إذا ( error )
      FT_پورو_Glyph( copy );
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
      أرجع FT_THROW( Invalid_دليل );

    /* if it is a bitmap, that's easy :-) */
    إذا ( format == FT_GLYPH_FORMAT_BITMAP )
      clazz = &ft_bitmap_glyph_class;

    /* if it is an outline */
    وإلا إذا ( format == FT_GLYPH_FORMAT_OUTLINE )
      clazz = &ft_outline_glyph_class;

#جيڪڏهن_وصف_ٿيل FT_CONFIG_OPTION_SVG
    /* if it is an SVG glyph */
    وإلا إذا ( format == FT_GLYPH_FORMAT_SVG )
      clazz = &ft_svg_glyph_class;
#جيڪڏهن_پورو

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
  FT_Get_Glyph( FT_Glyph_جڳهه  جڳهه,
                FT_Glyph     *aglyph )
  {
    FT_Error  error;
    FT_Glyph  glyph;


    إذا ( !جڳهه )
      أرجع FT_THROW( Invalid_جڳهه_Handle );

    إذا ( !aglyph )
      أرجع FT_THROW( Invalid_دليل );

    /* create FT_Glyph object */
    error = FT_New_Glyph( جڳهه->library, جڳهه->format, &glyph );
    إذا ( error )
      اذهب Exit;

    /* copy advance while converting 26.6 to 16.16 format */
    إذا ( جڳهه->advance.x >=  0x8000L * 64 ||
         جڳهه->advance.x <= -0x8000L * 64 )
    {
      FT_ERROR(( "FT_Get_Glyph: advance width too large\n" ));
      error = FT_THROW( Invalid_دليل );
      اذهب Exit2;
    }
    إذا ( جڳهه->advance.y >=  0x8000L * 64 ||
         جڳهه->advance.y <= -0x8000L * 64 )
    {
      FT_ERROR(( "FT_Get_Glyph: advance height too large\n" ));
      error = FT_THROW( Invalid_دليل );
      اذهب Exit2;
    }

    glyph->advance.x = جڳهه->advance.x * 1024;
    glyph->advance.y = جڳهه->advance.y * 1024;

    /* now import the image from the glyph slot */
    error = glyph->clazz->glyph_init( glyph, جڳهه );

  Exit2:
    /* if an error occurred, destroy the glyph */
    إذا ( error )
    {
      FT_پورو_Glyph( glyph );
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
                      ثابت FT_ميٽرڪس*  ميٽرڪس,
                      ثابت FT_ويڪٽر*  delta )
  {
    FT_Error  error = FT_Err_Ok;


    إذا ( !glyph || !glyph->clazz )
      error = FT_THROW( Invalid_دليل );
    وإلا
    {
      ثابت FT_Glyph_Class*  clazz = glyph->clazz;


      إذا ( clazz->glyph_transform )
      {
        /* transform glyph image */
        clazz->glyph_transform( glyph, ميٽرڪس, delta );

        /* transform advance vector */
        إذا ( ميٽرڪس )
          FT_ويڪٽر_Transform( &glyph->advance, ميٽرڪس );
      }
      وإلا
        error = FT_THROW( Invalid_Glyph_Format );
    }
    أرجع error;
  }


  /* documentation is in ftglyph.h */

  FT_EXPORT_DEF( فراغ )
  FT_Glyph_Get_CBox( FT_Glyph  glyph,
                     FT_UInt   bbox_گهڻائي,
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
    إذا ( bbox_گهڻائي == FT_GLYPH_BBOX_GRIDFIT ||
         bbox_گهڻائي == FT_GLYPH_BBOX_PIXELS  )
    {
      acbox->xMin = FT_PIX_FLOOR( acbox->xMin );
      acbox->yMin = FT_PIX_FLOOR( acbox->yMin );
      acbox->xMax = FT_PIX_CEIL_LONG( acbox->xMax );
      acbox->yMax = FT_PIX_CEIL_LONG( acbox->yMax );
    }

    /* convert to integer pixels if needed */
    إذا ( bbox_گهڻائي == FT_GLYPH_BBOX_TRUNCATE ||
         bbox_گهڻائي == FT_GLYPH_BBOX_PIXELS   )
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
                      FT_Render_گهڻائي    render_گهڻائي,
                      ثابت FT_ويڪٽر*  origin,
                      FT_Bool           destroy )
  {
    FT_Glyph_جڳهه_Rec           dummy;
    FT_Glyph_جڳهه_اندروني_Rec  dummy_اندروني;
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
    FT_ZERO( &dummy_اندروني );
    dummy.اندروني = &dummy_اندروني;
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
    FT_اڻ_استعمال_ٿيل( origin );
#جيڪڏهن_پورو

    /* prepare dummy slot for rendering */
    error = clazz->glyph_prepare( glyph, &dummy );
    إذا ( !error )
      error = FT_Render_Glyph_اندروني( glyph->library, &dummy, render_گهڻائي );

#جيڪڏهن_وصف_ٿيل FT_CONFIG_OPTION_SVG
    إذا ( clazz == &ft_svg_glyph_class )
    {
      FT_يادگيري  يادگيري = library->يادگيري;


      FT_FREE( dummy.other );
    }
#جيڪڏهن_پورو

#إذا 1
    إذا ( !destroy && origin )
    {
      FT_ويڪٽر  v;


      v.x = -origin->x;
      v.y = -origin->y;
      FT_Glyph_Transform( glyph, عدم, &v );
    }
#جيڪڏهن_پورو

    إذا ( error )
      اذهب Exit;

    /* in case of success, copy the bitmap to the glyph bitmap */
    bitmap->کاٻو   = dummy.bitmap_کاٻو;
    bitmap->چوٽي    = dummy.bitmap_چوٽي;
    bitmap->bitmap = dummy.bitmap;

    /* copy advance */
    bitmap->پاڙ.advance = glyph->advance;

    إذا ( destroy )
      FT_پورو_Glyph( glyph );

    *the_glyph = FT_GLYPH( bitmap );

  Exit:
    إذا ( error && bitmap )
      FT_پورو_Glyph( FT_GLYPH( bitmap ) );

    أرجع error;

  Bad:
    error = FT_THROW( Invalid_دليل );
    اذهب Exit;
  }


  /* documentation is in ftglyph.h */

  FT_EXPORT_DEF( فراغ )
  FT_پورو_Glyph( FT_Glyph  glyph )
  {
    إذا ( glyph )
    {
      FT_يادگيري              يادگيري = glyph->library->يادگيري;
      ثابت FT_Glyph_Class*  clazz  = glyph->clazz;


      إذا ( clazz->glyph_پورو )
        clazz->glyph_پورو( glyph );

      FT_FREE( glyph );
    }
  }


/*پڇاڙي*/
