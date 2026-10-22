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


#అన్తర్భావ <freetype/internal/ftdebug.h>

#అన్తర్భావ <freetype/ftglyph.h>
#అన్తర్భావ <freetype/ftoutln.h>
#అన్తర్భావ <freetype/ftbitmap.h>
#అన్తర్భావ <freetype/internal/ftobjs.h>
#అన్తర్భావ <freetype/otsvg.h>

#అన్తర్భావ "ftbase.h"


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#నిర్వచనం_రద్దు  FT_COMPONENT
#నిర్వచించు FT_COMPONENT  glyph


  /*************************************************************************/
  /*************************************************************************/
  /****                                                                 ****/
  /****   FT_BitmapGlyph support                                        ****/
  /****                                                                 ****/
  /*************************************************************************/
  /*************************************************************************/

  FT_CALLBACK_DEF( FT_Error )
  ft_bitmap_glyph_init( FT_Glyph      bitmap_glyph,
                        FT_Glyph_స్థానం  స్థానం )
  {
    FT_BitmapGlyph  glyph   = (FT_BitmapGlyph)bitmap_glyph;
    FT_Error        error;
    FT_Library      library = FT_GLYPH( glyph )->library;


    ఒకవేళ ( స్థానం->format != FT_GLYPH_FORMAT_BITMAP )
    {
      error = FT_THROW( Invalid_Glyph_Format );
      వెళ్ళు Exit;
    }

    glyph->ఎడమ = స్థానం->bitmap_ఎడమ;
    glyph->శిఖరం  = స్థానం->bitmap_శిఖరం;

    FT_Bitmap_Init( &glyph->bitmap );
    error = FT_Bitmap_Copy( library, &స్థానం->bitmap, &glyph->bitmap );

  Exit:
    తిరిగి error;
  }


  FT_CALLBACK_DEF( FT_Error )
  ft_bitmap_glyph_copy( FT_Glyph  bitmap_ఆధారం,
                        FT_Glyph  bitmap_లక్ష్యం )
  {
    FT_Library      library = bitmap_ఆధారం->library;
    FT_BitmapGlyph  ఆధారం  = (FT_BitmapGlyph)bitmap_ఆధారం;
    FT_BitmapGlyph  లక్ష్యం  = (FT_BitmapGlyph)bitmap_లక్ష్యం;


    లక్ష్యం->ఎడమ = ఆధారం->ఎడమ;
    లక్ష్యం->శిఖరం  = ఆధారం->శిఖరం;

    తిరిగి FT_Bitmap_Copy( library, &ఆధారం->bitmap, &లక్ష్యం->bitmap );
  }


  FT_CALLBACK_DEF( శూన్య )
  ft_bitmap_glyph_పూర్తి( FT_Glyph  bitmap_glyph )
  {
    FT_BitmapGlyph  glyph   = (FT_BitmapGlyph)bitmap_glyph;
    FT_Library      library = FT_GLYPH( glyph )->library;


    FT_Bitmap_పూర్తి( library, &glyph->bitmap );
  }


  FT_CALLBACK_DEF( శూన్య )
  ft_bitmap_glyph_bbox( FT_Glyph  bitmap_glyph,
                        FT_BBox*  cbox )
  {
    FT_BitmapGlyph  glyph = (FT_BitmapGlyph)bitmap_glyph;


    cbox->xMin = glyph->ఎడమ * 64;
    cbox->xMax = cbox->xMin + (FT_Pos)( glyph->bitmap.వెడల్పు * 64 );
    cbox->yMax = glyph->శిఖరం * 64;
    cbox->yMin = cbox->yMax - (FT_Pos)( glyph->bitmap.అడ్డువరుసలు * 64 );
  }


  FT_DEFINE_GLYPH(
    ft_bitmap_glyph_class,

    పరిమాణం ( FT_BitmapGlyphRec ),
    FT_GLYPH_FORMAT_BITMAP,

    ft_bitmap_glyph_init,    /* FT_Glyph_InitFunc       glyph_init      */
    ft_bitmap_glyph_పూర్తి,    /* FT_Glyph_DoneFunc       glyph_done      */
    ft_bitmap_glyph_copy,    /* FT_Glyph_CopyFunc       glyph_copy      */
    ఖాళీ,                    /* FT_Glyph_TransformFunc  glyph_transform */
    ft_bitmap_glyph_bbox,    /* FT_Glyph_GetBBoxFunc    glyph_bbox      */
    ఖాళీ                     /* FT_Glyph_PrepareFunc    glyph_prepare   */
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
                         FT_Glyph_స్థానం  స్థానం )
  {
    FT_OutlineGlyph  glyph   = (FT_OutlineGlyph)outline_glyph;
    FT_Error         error   = FT_Err_Ok;
    FT_Library       library = FT_GLYPH( glyph )->library;
    FT_Outline*      ఆధారం  = &స్థానం->outline;
    FT_Outline*      లక్ష్యం  = &glyph->outline;


    /* check format in glyph slot */
    ఒకవేళ ( స్థానం->format != FT_GLYPH_FORMAT_OUTLINE )
    {
      error = FT_THROW( Invalid_Glyph_Format );
      వెళ్ళు Exit;
    }

    /* allocate new outline */
    error = FT_Outline_New( library,
                            (FT_UInt)ఆధారం->n_points,
                            ఆధారం->n_contours,
                            &glyph->outline );
    ఒకవేళ ( error )
      వెళ్ళు Exit;

    FT_Outline_Copy( ఆధారం, లక్ష్యం );

  Exit:
    తిరిగి error;
  }


  FT_CALLBACK_DEF( శూన్య )
  ft_outline_glyph_పూర్తి( FT_Glyph  outline_glyph )
  {
    FT_OutlineGlyph  glyph = (FT_OutlineGlyph)outline_glyph;


    FT_Outline_పూర్తి( FT_GLYPH( glyph )->library, &glyph->outline );
  }


  FT_CALLBACK_DEF( FT_Error )
  ft_outline_glyph_copy( FT_Glyph  outline_ఆధారం,
                         FT_Glyph  outline_లక్ష్యం )
  {
    FT_OutlineGlyph  ఆధారం  = (FT_OutlineGlyph)outline_ఆధారం;
    FT_OutlineGlyph  లక్ష్యం  = (FT_OutlineGlyph)outline_లక్ష్యం;
    FT_Error         error;
    FT_Library       library = FT_GLYPH( ఆధారం )->library;


    error = FT_Outline_New( library,
                            (FT_UInt)ఆధారం->outline.n_points,
                            ఆధారం->outline.n_contours,
                            &లక్ష్యం->outline );
    ఒకవేళ ( !error )
      FT_Outline_Copy( &ఆధారం->outline, &లక్ష్యం->outline );

    తిరిగి error;
  }


  FT_CALLBACK_DEF( శూన్య )
  ft_outline_glyph_transform( FT_Glyph          outline_glyph,
                              స్థిరరాశి FT_మాతృక*  మాతృక,
                              స్థిరరాశి FT_సదిశ*  delta )
  {
    FT_OutlineGlyph  glyph = (FT_OutlineGlyph)outline_glyph;


    ఒకవేళ ( మాతృక )
      FT_Outline_Transform( &glyph->outline, మాతృక );

    ఒకవేళ ( delta )
      FT_Outline_Translate( &glyph->outline, delta->x, delta->y );
  }


  FT_CALLBACK_DEF( శూన్య )
  ft_outline_glyph_bbox( FT_Glyph  outline_glyph,
                         FT_BBox*  bbox )
  {
    FT_OutlineGlyph  glyph = (FT_OutlineGlyph)outline_glyph;


    FT_Outline_Get_CBox( &glyph->outline, bbox );
  }


  FT_CALLBACK_DEF( FT_Error )
  ft_outline_glyph_prepare( FT_Glyph      outline_glyph,
                            FT_Glyph_స్థానం  స్థానం )
  {
    FT_OutlineGlyph  glyph = (FT_OutlineGlyph)outline_glyph;


    స్థానం->format         = FT_GLYPH_FORMAT_OUTLINE;
    స్థానం->outline        = glyph->outline;
    స్థానం->outline.flags &= ~FT_OUTLINE_OWNER;

    తిరిగి FT_Err_Ok;
  }


  FT_DEFINE_GLYPH(
    ft_outline_glyph_class,

    పరిమాణం ( FT_OutlineGlyphRec ),
    FT_GLYPH_FORMAT_OUTLINE,

    ft_outline_glyph_init,      /* FT_Glyph_InitFunc       glyph_init      */
    ft_outline_glyph_పూర్తి,      /* FT_Glyph_DoneFunc       glyph_done      */
    ft_outline_glyph_copy,      /* FT_Glyph_CopyFunc       glyph_copy      */
    ft_outline_glyph_transform, /* FT_Glyph_TransformFunc  glyph_transform */
    ft_outline_glyph_bbox,      /* FT_Glyph_GetBBoxFunc    glyph_bbox      */
    ft_outline_glyph_prepare    /* FT_Glyph_PrepareFunc    glyph_prepare   */
  )


#నిర్వచితమైతే FT_CONFIG_OPTION_SVG

  /*************************************************************************/
  /*************************************************************************/
  /****                                                                 ****/
  /****   FT_SvgGlyph support                                           ****/
  /****                                                                 ****/
  /*************************************************************************/
  /*************************************************************************/


  FT_CALLBACK_DEF( FT_Error )
  ft_svg_glyph_init( FT_Glyph      svg_glyph,
                     FT_Glyph_స్థానం  స్థానం )
  {
    FT_ULong         doc_నిడివి;
    FT_SVG_Document  document;
    FT_SvgGlyph      glyph = (FT_SvgGlyph)svg_glyph;

    FT_Error   error  = FT_Err_Ok;
    FT_జ్ఞాపకం  జ్ఞాపకం = FT_GLYPH( glyph )->library->జ్ఞాపకం;


    ఒకవేళ ( స్థానం->format != FT_GLYPH_FORMAT_SVG )
    {
      error = FT_THROW( Invalid_Glyph_Format );
      వెళ్ళు Exit;
    }

    ఒకవేళ ( స్థానం->other == ఖాళీ )
    {
      error = FT_THROW( Invalid_స్థానం_Handle );
      వెళ్ళు Exit;
    }

    document = (FT_SVG_Document)స్థానం->other;

    ఒకవేళ ( document->svg_document_నిడివి == 0 )
    {
      error = FT_THROW( Invalid_స్థానం_Handle );
      వెళ్ళు Exit;
    }

    /* allocate a new document */
    doc_నిడివి = document->svg_document_నిడివి;
    ఒకవేళ ( FT_QALLOC( glyph->svg_document, doc_నిడివి ) )
      వెళ్ళు Exit;
    glyph->svg_document_నిడివి = doc_నిడివి;

    glyph->glyph_సూచీ = స్థానం->glyph_సూచీ;

    glyph->metrics      = document->metrics;
    glyph->units_చొప్పున_EM = document->units_చొప్పున_EM;

    glyph->ప్రారంభం_glyph_id = document->ప్రారంభం_glyph_id;
    glyph->చివర_glyph_id   = document->చివర_glyph_id;

    glyph->transform = document->transform;
    glyph->delta     = document->delta;

    /* copy the document into glyph */
    FT_MEM_COPY( glyph->svg_document, document->svg_document, doc_నిడివి );

  Exit:
    తిరిగి error;
  }


  FT_CALLBACK_DEF( శూన్య )
  ft_svg_glyph_పూర్తి( FT_Glyph  svg_glyph )
  {
    FT_SvgGlyph  glyph  = (FT_SvgGlyph)svg_glyph;
    FT_జ్ఞాపకం    జ్ఞాపకం = svg_glyph->library->జ్ఞాపకం;


    /* just free the memory */
    FT_FREE( glyph->svg_document );
  }


  FT_CALLBACK_DEF( FT_Error )
  ft_svg_glyph_copy( FT_Glyph  svg_ఆధారం,
                     FT_Glyph  svg_లక్ష్యం )
  {
    FT_SvgGlyph  ఆధారం = (FT_SvgGlyph)svg_ఆధారం;
    FT_SvgGlyph  లక్ష్యం = (FT_SvgGlyph)svg_లక్ష్యం;

    FT_Error   error  = FT_Err_Ok;
    FT_జ్ఞాపకం  జ్ఞాపకం = FT_GLYPH( ఆధారం )->library->జ్ఞాపకం;


    ఒకవేళ ( svg_ఆధారం->format != FT_GLYPH_FORMAT_SVG )
    {
      error = FT_THROW( Invalid_Glyph_Format );
      వెళ్ళు Exit;
    }

    ఒకవేళ ( ఆధారం->svg_document_నిడివి == 0 )
    {
      error = FT_THROW( Invalid_స్థానం_Handle );
      వెళ్ళు Exit;
    }

    లక్ష్యం->glyph_సూచీ = ఆధారం->glyph_సూచీ;

    లక్ష్యం->svg_document_నిడివి = ఆధారం->svg_document_నిడివి;

    లక్ష్యం->metrics      = ఆధారం->metrics;
    లక్ష్యం->units_చొప్పున_EM = ఆధారం->units_చొప్పున_EM;

    లక్ష్యం->ప్రారంభం_glyph_id = ఆధారం->ప్రారంభం_glyph_id;
    లక్ష్యం->చివర_glyph_id   = ఆధారం->చివర_glyph_id;

    లక్ష్యం->transform = ఆధారం->transform;
    లక్ష్యం->delta     = ఆధారం->delta;

    /* allocate space for the SVG document */
    ఒకవేళ ( FT_QALLOC( లక్ష్యం->svg_document, లక్ష్యం->svg_document_నిడివి ) )
      వెళ్ళు Exit;

    /* copy the document */
    FT_MEM_COPY( లక్ష్యం->svg_document,
                 ఆధారం->svg_document,
                 లక్ష్యం->svg_document_నిడివి );

  Exit:
    తిరిగి error;
  }


  FT_CALLBACK_DEF( శూన్య )
  ft_svg_glyph_transform( FT_Glyph          svg_glyph,
                          స్థిరరాశి FT_మాతృక*  మాతృక_2,
                          స్థిరరాశి FT_సదిశ*  _delta )
  {
    FT_SvgGlyph  glyph  = (FT_SvgGlyph)svg_glyph;
    FT_మాతృక*   మాతృక = (FT_మాతృక*)మాతృక_2;
    FT_సదిశ*   delta  = (FT_సదిశ*)_delta;

    FT_మాతృక  tmp_మాతృక;
    FT_సదిశ  tmp_delta;

    FT_మాతృక  a, b;
    FT_Pos     x, y;


    ఒకవేళ ( !మాతృక )
    {
      tmp_మాతృక.xx = 0x10000;
      tmp_మాతృక.xy = 0;
      tmp_మాతృక.yx = 0;
      tmp_మాతృక.yy = 0x10000;

      మాతృక = &tmp_మాతృక;
    }

    ఒకవేళ ( !delta )
    {
      tmp_delta.x = 0;
      tmp_delta.y = 0;

      delta = &tmp_delta;
    }

    a = glyph->transform;
    b = *మాతృక;
    FT_మాతృక_Multiply( &b, &a );

    x = కూడు_LONG( కూడు_LONG( FT_MulFix( మాతృక->xx, glyph->delta.x ),
                            FT_MulFix( మాతృక->xy, glyph->delta.y ) ),
                  delta->x );
    y = కూడు_LONG( కూడు_LONG( FT_MulFix( మాతృక->yx, glyph->delta.x ),
                            FT_MulFix( మాతృక->yy, glyph->delta.y ) ),
                  delta->y );

    glyph->delta.x = x;
    glyph->delta.y = y;

    glyph->transform = a;
  }


  FT_CALLBACK_DEF( FT_Error )
  ft_svg_glyph_prepare( FT_Glyph      svg_glyph,
                        FT_Glyph_స్థానం  స్థానం )
  {
    FT_SvgGlyph  glyph = (FT_SvgGlyph)svg_glyph;

    FT_Error   error  = FT_Err_Ok;
    FT_జ్ఞాపకం  జ్ఞాపకం = svg_glyph->library->జ్ఞాపకం;

    FT_SVG_Document  document = ఖాళీ;


    ఒకవేళ ( FT_NEW( document ) )
      తిరిగి error;

    document->svg_document        = glyph->svg_document;
    document->svg_document_నిడివి = glyph->svg_document_నిడివి;

    document->metrics      = glyph->metrics;
    document->units_చొప్పున_EM = glyph->units_చొప్పున_EM;

    document->ప్రారంభం_glyph_id = glyph->ప్రారంభం_glyph_id;
    document->చివర_glyph_id   = glyph->చివర_glyph_id;

    document->transform = glyph->transform;
    document->delta     = glyph->delta;

    స్థానం->format      = FT_GLYPH_FORMAT_SVG;
    స్థానం->glyph_సూచీ = glyph->glyph_సూచీ;
    స్థానం->other       = document;

    తిరిగి error;
  }


  FT_DEFINE_GLYPH(
    ft_svg_glyph_class,

    పరిమాణం ( FT_SvgGlyphRec ),
    FT_GLYPH_FORMAT_SVG,

    ft_svg_glyph_init,      /* FT_Glyph_InitFunc       glyph_init      */
    ft_svg_glyph_పూర్తి,      /* FT_Glyph_DoneFunc       glyph_done      */
    ft_svg_glyph_copy,      /* FT_Glyph_CopyFunc       glyph_copy      */
    ft_svg_glyph_transform, /* FT_Glyph_TransformFunc  glyph_transform */
    ఖాళీ,                   /* FT_Glyph_GetBBoxFunc    glyph_bbox      */
    ft_svg_glyph_prepare    /* FT_Glyph_PrepareFunc    glyph_prepare   */
  )

#అయితే_ముగింపు /* FT_CONFIG_OPTION_SVG */


  /*************************************************************************/
  /*************************************************************************/
  /****                                                                 ****/
  /****   FT_Glyph class and API                                        ****/
  /****                                                                 ****/
  /*************************************************************************/
  /*************************************************************************/

   స్థిర FT_Error
   ft_new_glyph( FT_Library             library,
                 స్థిరరాశి FT_Glyph_Class*  clazz,
                 FT_Glyph*              aglyph )
   {
     FT_జ్ఞాపకం  జ్ఞాపకం = library->జ్ఞాపకం;
     FT_Error   error;
     FT_Glyph   glyph  = ఖాళీ;


     *aglyph = ఖాళీ;

     ఒకవేళ ( !FT_ALLOC( glyph, clazz->glyph_కొలత ) )
     {
       glyph->library = library;
       glyph->clazz   = clazz;
       glyph->format  = clazz->glyph_format;

       *aglyph = glyph;
     }

     తిరిగి error;
   }


  /* documentation is in ftglyph.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Glyph_Copy( FT_Glyph   ఆధారం,
                 FT_Glyph  *లక్ష్యం )
  {
    FT_Glyph               copy;
    FT_Error               error;
    స్థిరరాశి FT_Glyph_Class*  clazz;


    /* check arguments */
    ఒకవేళ ( !లక్ష్యం || !ఆధారం || !ఆధారం->clazz )
    {
      error = FT_THROW( Invalid_వాదం );
      వెళ్ళు Exit;
    }

    *లక్ష్యం = ఖాళీ;

    ఒకవేళ ( !ఆధారం || !ఆధారం->clazz )
    {
      error = FT_THROW( Invalid_వాదం );
      వెళ్ళు Exit;
    }

    clazz = ఆధారం->clazz;
    error = ft_new_glyph( ఆధారం->library, clazz, &copy );
    ఒకవేళ ( error )
      వెళ్ళు Exit;

    copy->advance = ఆధారం->advance;
    copy->format  = ఆధారం->format;

    ఒకవేళ ( clazz->glyph_copy )
      error = clazz->glyph_copy( ఆధారం, copy );

    ఒకవేళ ( error )
      FT_పూర్తి_Glyph( copy );
    లేకపోతే
      *లక్ష్యం = copy;

  Exit:
    తిరిగి error;
  }


  /* documentation is in ftglyph.h */

  FT_EXPORT( FT_Error )
  FT_New_Glyph( FT_Library       library,
                FT_Glyph_Format  format,
                FT_Glyph        *aglyph )
  {
    స్థిరరాశి FT_Glyph_Class*  clazz = ఖాళీ;

    ఒకవేళ ( !library || !aglyph )
      తిరిగి FT_THROW( Invalid_వాదం );

    /* if it is a bitmap, that's easy :-) */
    ఒకవేళ ( format == FT_GLYPH_FORMAT_BITMAP )
      clazz = &ft_bitmap_glyph_class;

    /* if it is an outline */
    లేకపోతే ఒకవేళ ( format == FT_GLYPH_FORMAT_OUTLINE )
      clazz = &ft_outline_glyph_class;

#నిర్వచితమైతే FT_CONFIG_OPTION_SVG
    /* if it is an SVG glyph */
    లేకపోతే ఒకవేళ ( format == FT_GLYPH_FORMAT_SVG )
      clazz = &ft_svg_glyph_class;
#అయితే_ముగింపు

    లేకపోతే
    {
      /* try to find a renderer that supports the glyph image format */
      FT_Renderer  render = FT_Lookup_Renderer( library, format, 0 );


      ఒకవేళ ( render )
        clazz = &render->glyph_class;
    }

    ఒకవేళ ( !clazz )
      తిరిగి FT_THROW( Invalid_Glyph_Format );

    /* create FT_Glyph object */
    తిరిగి ft_new_glyph( library, clazz, aglyph );
  }


  /* documentation is in ftglyph.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Glyph( FT_Glyph_స్థానం  స్థానం,
                FT_Glyph     *aglyph )
  {
    FT_Error  error;
    FT_Glyph  glyph;


    ఒకవేళ ( !స్థానం )
      తిరిగి FT_THROW( Invalid_స్థానం_Handle );

    ఒకవేళ ( !aglyph )
      తిరిగి FT_THROW( Invalid_వాదం );

    /* create FT_Glyph object */
    error = FT_New_Glyph( స్థానం->library, స్థానం->format, &glyph );
    ఒకవేళ ( error )
      వెళ్ళు Exit;

    /* copy advance while converting 26.6 to 16.16 format */
    ఒకవేళ ( స్థానం->advance.x >=  0x8000L * 64 ||
         స్థానం->advance.x <= -0x8000L * 64 )
    {
      FT_ERROR(( "FT_Get_Glyph: advance width too large\n" ));
      error = FT_THROW( Invalid_వాదం );
      వెళ్ళు Exit2;
    }
    ఒకవేళ ( స్థానం->advance.y >=  0x8000L * 64 ||
         స్థానం->advance.y <= -0x8000L * 64 )
    {
      FT_ERROR(( "FT_Get_Glyph: advance height too large\n" ));
      error = FT_THROW( Invalid_వాదం );
      వెళ్ళు Exit2;
    }

    glyph->advance.x = స్థానం->advance.x * 1024;
    glyph->advance.y = స్థానం->advance.y * 1024;

    /* now import the image from the glyph slot */
    error = glyph->clazz->glyph_init( glyph, స్థానం );

  Exit2:
    /* if an error occurred, destroy the glyph */
    ఒకవేళ ( error )
    {
      FT_పూర్తి_Glyph( glyph );
      *aglyph = ఖాళీ;
    }
    లేకపోతే
      *aglyph = glyph;

  Exit:
    తిరిగి error;
  }


  /* documentation is in ftglyph.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Glyph_Transform( FT_Glyph          glyph,
                      స్థిరరాశి FT_మాతృక*  మాతృక,
                      స్థిరరాశి FT_సదిశ*  delta )
  {
    FT_Error  error = FT_Err_Ok;


    ఒకవేళ ( !glyph || !glyph->clazz )
      error = FT_THROW( Invalid_వాదం );
    లేకపోతే
    {
      స్థిరరాశి FT_Glyph_Class*  clazz = glyph->clazz;


      ఒకవేళ ( clazz->glyph_transform )
      {
        /* transform glyph image */
        clazz->glyph_transform( glyph, మాతృక, delta );

        /* transform advance vector */
        ఒకవేళ ( మాతృక )
          FT_సదిశ_Transform( &glyph->advance, మాతృక );
      }
      లేకపోతే
        error = FT_THROW( Invalid_Glyph_Format );
    }
    తిరిగి error;
  }


  /* documentation is in ftglyph.h */

  FT_EXPORT_DEF( శూన్య )
  FT_Glyph_Get_CBox( FT_Glyph  glyph,
                     FT_UInt   bbox_బహుళకం,
                     FT_BBox  *acbox )
  {
    స్థిరరాశి FT_Glyph_Class*  clazz;


    ఒకవేళ ( !acbox )
      తిరిగి;

    acbox->xMin = acbox->yMin = acbox->xMax = acbox->yMax = 0;

    ఒకవేళ ( !glyph || !glyph->clazz )
      తిరిగి;

    clazz = glyph->clazz;
    ఒకవేళ ( !clazz->glyph_bbox )
      తిరిగి;

    /* retrieve bbox in 26.6 coordinates */
    clazz->glyph_bbox( glyph, acbox );

    /* perform grid fitting if needed */
    ఒకవేళ ( bbox_బహుళకం == FT_GLYPH_BBOX_GRIDFIT ||
         bbox_బహుళకం == FT_GLYPH_BBOX_PIXELS  )
    {
      acbox->xMin = FT_PIX_FLOOR( acbox->xMin );
      acbox->yMin = FT_PIX_FLOOR( acbox->yMin );
      acbox->xMax = FT_PIX_CEIL_LONG( acbox->xMax );
      acbox->yMax = FT_PIX_CEIL_LONG( acbox->yMax );
    }

    /* convert to integer pixels if needed */
    ఒకవేళ ( bbox_బహుళకం == FT_GLYPH_BBOX_TRUNCATE ||
         bbox_బహుళకం == FT_GLYPH_BBOX_PIXELS   )
    {
      acbox->xMin >>= 6;
      acbox->yMin >>= 6;
      acbox->xMax >>= 6;
      acbox->yMax >>= 6;
    }
  }


  /* documentation is in ftglyph.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Glyph_గమ్యం_Bitmap( FT_Glyph*         the_glyph,
                      FT_Render_బహుళకం    render_బహుళకం,
                      స్థిరరాశి FT_సదిశ*  origin,
                      FT_Bool           destroy )
  {
    FT_Glyph_స్థానం_Rec           dummy;
    FT_Glyph_స్థానం_అంతర్గత_Rec  dummy_అంతర్గత;
    FT_Error                  error = FT_Err_Ok;
    FT_Glyph                  b, glyph;
    FT_BitmapGlyph            bitmap = ఖాళీ;
    స్థిరరాశి FT_Glyph_Class*     clazz;

    FT_Library                library;


    /* check argument */
    ఒకవేళ ( !the_glyph )
      వెళ్ళు Bad;
    glyph = *the_glyph;
    ఒకవేళ ( !glyph )
      వెళ్ళు Bad;

    clazz   = glyph->clazz;
    library = glyph->library;
    ఒకవేళ ( !library || !clazz )
      వెళ్ళు Bad;

    /* when called with a bitmap glyph, do nothing and return successfully */
    ఒకవేళ ( clazz == &ft_bitmap_glyph_class )
      వెళ్ళు Exit;

    ఒకవేళ ( !clazz->glyph_prepare )
      వెళ్ళు Bad;

    /* we render the glyph into a glyph bitmap using a `dummy' glyph slot */
    /* then calling FT_Render_Glyph_Internal()                            */

    FT_ZERO( &dummy );
    FT_ZERO( &dummy_అంతర్గత );
    dummy.అంతర్గత = &dummy_అంతర్గత;
    dummy.library  = library;
    dummy.format   = clazz->glyph_format;

    /* create result bitmap glyph */
    error = ft_new_glyph( library, &ft_bitmap_glyph_class, &b );
    ఒకవేళ ( error )
      వెళ్ళు Exit;
    bitmap = (FT_BitmapGlyph)b;

#ఒకవేళ 1
    /* if `origin' is set, translate the glyph image */
    ఒకవేళ ( origin )
      FT_Glyph_Transform( glyph, ఖాళీ, origin );
#లేకపోతే
    FT_వాడని( origin );
#అయితే_ముగింపు

    /* prepare dummy slot for rendering */
    error = clazz->glyph_prepare( glyph, &dummy );
    ఒకవేళ ( !error )
      error = FT_Render_Glyph_అంతర్గత( glyph->library, &dummy, render_బహుళకం );

#నిర్వచితమైతే FT_CONFIG_OPTION_SVG
    ఒకవేళ ( clazz == &ft_svg_glyph_class )
    {
      FT_జ్ఞాపకం  జ్ఞాపకం = library->జ్ఞాపకం;


      FT_FREE( dummy.other );
    }
#అయితే_ముగింపు

#ఒకవేళ 1
    ఒకవేళ ( !destroy && origin )
    {
      FT_సదిశ  v;


      v.x = -origin->x;
      v.y = -origin->y;
      FT_Glyph_Transform( glyph, ఖాళీ, &v );
    }
#అయితే_ముగింపు

    ఒకవేళ ( error )
      వెళ్ళు Exit;

    /* in case of success, copy the bitmap to the glyph bitmap */
    bitmap->ఎడమ   = dummy.bitmap_ఎడమ;
    bitmap->శిఖరం    = dummy.bitmap_శిఖరం;
    bitmap->bitmap = dummy.bitmap;

    /* copy advance */
    bitmap->మూలం.advance = glyph->advance;

    ఒకవేళ ( destroy )
      FT_పూర్తి_Glyph( glyph );

    *the_glyph = FT_GLYPH( bitmap );

  Exit:
    ఒకవేళ ( error && bitmap )
      FT_పూర్తి_Glyph( FT_GLYPH( bitmap ) );

    తిరిగి error;

  Bad:
    error = FT_THROW( Invalid_వాదం );
    వెళ్ళు Exit;
  }


  /* documentation is in ftglyph.h */

  FT_EXPORT_DEF( శూన్య )
  FT_పూర్తి_Glyph( FT_Glyph  glyph )
  {
    ఒకవేళ ( glyph )
    {
      FT_జ్ఞాపకం              జ్ఞాపకం = glyph->library->జ్ఞాపకం;
      స్థిరరాశి FT_Glyph_Class*  clazz  = glyph->clazz;


      ఒకవేళ ( clazz->glyph_పూర్తి )
        clazz->glyph_పూర్తి( glyph );

      FT_FREE( glyph );
    }
  }


/*చివర*/
