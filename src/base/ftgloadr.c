/****************************************************************************
 *
 * ftgloadr.c
 *
 *   The FreeType glyph loader (body).
 *
 * Copyright (C) 2002-2026 by
 * David Turner, Robert Wilhelm, and Werner Lemberg
 *
 * This file is part of the FreeType project, and may only be used,
 * modified, and distributed under the terms of the FreeType project
 * license, LICENSE.TXT.  By continuing to use, modify, or distribute
 * this file you indicate that you have read the license and
 * understand and accept it fully.
 *
 */


#అన్తర్భావ <freetype/internal/ftdebug.h>
#అన్తర్భావ <freetype/internal/ftgloadr.h>
#అన్తర్భావ <freetype/internal/ftmemory.h>
#అన్తర్భావ <freetype/internal/ftobjs.h>

#నిర్వచనం_రద్దు  FT_COMPONENT
#నిర్వచించు FT_COMPONENT  gloader


  /*************************************************************************/
  /*************************************************************************/
  /*************************************************************************/
  /*****                                                               *****/
  /*****                                                               *****/
  /*****                    G L Y P H   L O A D E R                    *****/
  /*****                                                               *****/
  /*****                                                               *****/
  /*************************************************************************/
  /*************************************************************************/
  /*************************************************************************/

  /**************************************************************************
   *
   * The glyph loader is a simple object which is used to load a set of
   * glyphs easily.  It is critical for the correct loading of composites.
   *
   * Ideally, one can see it as a stack of abstract `glyph' objects.
   *
   *   loader.base     Is really the bottom of the stack.  It describes a
   *                   single glyph image made of the juxtaposition of
   *                   several glyphs (those `in the stack').
   *
   *   loader.current  Describes the top of the stack, on which a new
   *                   glyph can be loaded.
   *
   *   Rewind          Clears the stack.
   *   Prepare         Set up `loader.current' for addition of a new glyph
   *                   image.
   *   Add             Add the `current' glyph image to the `base' one,
   *                   and prepare for another one.
   *
   * The glyph loader is now a base object.  Each driver used to
   * re-implement it in one way or the other, which wasted code and
   * energy.
   *
   */


  /* create a new glyph loader */
  FT_పునాది_DEF( FT_Error )
  FT_GlyphLoader_New( FT_జ్ఞాపకం        జ్ఞాపకం,
                      FT_GlyphLoader  *aloader )
  {
    FT_GlyphLoader  loader = ఖాళీ;
    FT_Error        error;


    ఒకవేళ ( !FT_NEW( loader ) )
    {
      loader->జ్ఞాపకం = జ్ఞాపకం;
      *aloader       = loader;
    }
    తిరిగి error;
  }


  /* rewind the glyph loader - reset counters to 0 */
  FT_పునాది_DEF( శూన్య )
  FT_GlyphLoader_Rewind( FT_GlyphLoader  loader )
  {
    FT_Glyph_ఎక్కించు  పునాది    = &loader->పునాది;
    FT_Glyph_ఎక్కించు  current = &loader->current;


    పునాది->outline.n_points   = 0;
    పునాది->outline.n_contours = 0;
    పునాది->outline.flags      = 0;
    పునాది->అంకె_subglyphs      = 0;

    *current = *పునాది;
  }


  /* reset glyph loader, free all allocated tables, */
  /* and start from zero                            */
  FT_పునాది_DEF( శూన్య )
  FT_GlyphLoader_Reset( FT_GlyphLoader  loader )
  {
    FT_జ్ఞాపకం  జ్ఞాపకం = loader->జ్ఞాపకం;


    FT_FREE( loader->పునాది.outline.points );
    FT_FREE( loader->పునాది.outline.ట్యాగులు );
    FT_FREE( loader->పునాది.outline.contours );
    FT_FREE( loader->పునాది.extra_points );
    FT_FREE( loader->పునాది.subglyphs );

    loader->పునాది.extra_points2 = ఖాళీ;

    loader->max_points    = 0;
    loader->max_contours  = 0;
    loader->max_subglyphs = 0;

    FT_GlyphLoader_Rewind( loader );
  }


  /* delete a glyph loader */
  FT_పునాది_DEF( శూన్య )
  FT_Glyph_Loader_పూర్తి( FT_GlyphLoader  loader )
  {
    ఒకవేళ ( loader )
    {
      FT_జ్ఞాపకం  జ్ఞాపకం = loader->జ్ఞాపకం;


      FT_GlyphLoader_Reset( loader );
      FT_FREE( loader );
    }
  }


  /* re-adjust the `current' outline fields */
  స్థిర శూన్య
  FT_GlyphLoader_Adjust_Points( FT_GlyphLoader  loader )
  {
    FT_Outline*  పునాది    = &loader->పునాది.outline;
    FT_Outline*  current = &loader->current.outline;


    current->points   = FT_స్థానభ్రంశం( పునాది->points,   పునాది->n_points );
    current->ట్యాగులు     = FT_స్థానభ్రంశం( పునాది->ట్యాగులు,     పునాది->n_points );
    current->contours = FT_స్థానభ్రంశం( పునాది->contours, పునాది->n_contours );

    /* handle extra points table - if any */
    ఒకవేళ ( loader->use_extra )
    {
      loader->current.extra_points  = loader->పునాది.extra_points +
                                      పునాది->n_points;

      loader->current.extra_points2 = loader->పునాది.extra_points2 +
                                      పునాది->n_points;
    }
  }


  FT_పునాది_DEF( FT_Error )
  FT_GlyphLoader_CreateExtra( FT_GlyphLoader  loader )
  {
    FT_Error   error;
    FT_జ్ఞాపకం  జ్ఞాపకం = loader->జ్ఞాపకం;


    ఒకవేళ ( loader->max_points == 0           ||
         loader->పునాది.extra_points != ఖాళీ )
      తిరిగి FT_Err_Ok;

    ఒకవేళ ( !FT_NEW_ARRAY( loader->పునాది.extra_points, 2 * loader->max_points ) )
    {
      loader->use_extra          = 1;
      loader->పునాది.extra_points2 = loader->పునాది.extra_points +
                                   loader->max_points;

      FT_GlyphLoader_Adjust_Points( loader );
    }
    తిరిగి error;
  }


  /* re-adjust the `current' subglyphs field */
  స్థిర శూన్య
  FT_GlyphLoader_Adjust_Subglyphs( FT_GlyphLoader  loader )
  {
    FT_Glyph_ఎక్కించు  పునాది    = &loader->పునాది;
    FT_Glyph_ఎక్కించు  current = &loader->current;


    current->subglyphs = FT_స్థానభ్రంశం( పునాది->subglyphs, పునాది->అంకె_subglyphs );
  }


  /* Ensure that we can add `n_points' and `n_contours' to our glyph.      */
  /* This function reallocates its outline tables if necessary.  Note that */
  /* it DOESN'T change the number of points within the loader!             */
  /*                                                                       */
  FT_పునాది_DEF( FT_Error )
  FT_GlyphLoader_CheckPoints( FT_GlyphLoader  loader,
                              FT_UInt         n_points,
                              FT_UInt         n_contours )
  {
    FT_జ్ఞాపకం    జ్ఞాపకం  = loader->జ్ఞాపకం;
    FT_Error     error   = FT_Err_Ok;
    FT_Outline*  పునాది    = &loader->పునాది.outline;
    FT_Outline*  current = &loader->current.outline;
    FT_Bool      adjust  = 0;

    FT_UInt  new_max, old_max, min_new_max;


    error = FT_GlyphLoader_CreateExtra( loader );
    ఒకవేళ ( error )
      వెళ్ళు Exit;

    /* check points & tags */
    new_max = (FT_UInt)పునాది->n_points + (FT_UInt)current->n_points +
              n_points;
    old_max = loader->max_points;

    ఒకవేళ ( new_max > old_max )
    {
      ఒకవేళ ( new_max > FT_OUTLINE_POINTS_MAX )
      {
        error = FT_THROW( Array_Too_Large );
        వెళ్ళు Exit;
      }

      min_new_max = old_max + ( old_max >> 1 );
      ఒకవేళ ( new_max < min_new_max )
        new_max = min_new_max;
      new_max = FT_PAD_CEIL( new_max, 8 );
      ఒకవేళ ( new_max > FT_OUTLINE_POINTS_MAX )
        new_max = FT_OUTLINE_POINTS_MAX;

      ఒకవేళ ( FT_RENEW_ARRAY( పునాది->points, old_max, new_max ) ||
           FT_RENEW_ARRAY( పునాది->ట్యాగులు,   old_max, new_max ) )
        వెళ్ళు Exit;

      ఒకవేళ ( loader->use_extra )
      {
        ఒకవేళ ( FT_RENEW_ARRAY( loader->పునాది.extra_points,
                             old_max * 2, new_max * 2 ) )
          వెళ్ళు Exit;

        FT_ARRAY_MOVE( loader->పునాది.extra_points + new_max,
                       loader->పునాది.extra_points + old_max,
                       old_max );

        loader->పునాది.extra_points2 = loader->పునాది.extra_points + new_max;
      }

      adjust = 1;
      loader->max_points = new_max;
    }

    error = FT_GlyphLoader_CreateExtra( loader );
    ఒకవేళ ( error )
      వెళ్ళు Exit;

    /* check contours */
    old_max = loader->max_contours;
    new_max = (FT_UInt)పునాది->n_contours + (FT_UInt)current->n_contours +
              n_contours;
    ఒకవేళ ( new_max > old_max )
    {
      ఒకవేళ ( new_max > FT_OUTLINE_CONTOURS_MAX )
      {
        error = FT_THROW( Array_Too_Large );
        వెళ్ళు Exit;
      }

      min_new_max = old_max + ( old_max >> 1 );
      ఒకవేళ ( new_max < min_new_max )
        new_max = min_new_max;
      new_max = FT_PAD_CEIL( new_max, 4 );
      ఒకవేళ ( new_max > FT_OUTLINE_CONTOURS_MAX )
        new_max = FT_OUTLINE_CONTOURS_MAX;

      ఒకవేళ ( FT_RENEW_ARRAY( పునాది->contours, old_max, new_max ) )
        వెళ్ళు Exit;

      adjust = 1;
      loader->max_contours = new_max;
    }

    ఒకవేళ ( adjust )
      FT_GlyphLoader_Adjust_Points( loader );

  Exit:
    ఒకవేళ ( error )
      FT_GlyphLoader_Reset( loader );

    తిరిగి error;
  }


  /* Ensure that we can add `n_subglyphs' to our glyph. this function */
  /* reallocates its subglyphs table if necessary.  Note that it DOES */
  /* NOT change the number of subglyphs within the loader!            */
  /*                                                                  */
  FT_పునాది_DEF( FT_Error )
  FT_GlyphLoader_CheckSubGlyphs( FT_GlyphLoader  loader,
                                 FT_UInt         n_subs )
  {
    FT_జ్ఞాపకం     జ్ఞాపకం = loader->జ్ఞాపకం;
    FT_Error      error  = FT_Err_Ok;
    FT_UInt       new_max, old_max;

    FT_Glyph_ఎక్కించు  పునాది    = &loader->పునాది;
    FT_Glyph_ఎక్కించు  current = &loader->current;


    new_max = పునాది->అంకె_subglyphs + current->అంకె_subglyphs + n_subs;
    old_max = loader->max_subglyphs;
    ఒకవేళ ( new_max > old_max )
    {
      new_max = FT_PAD_CEIL( new_max, 2 );
      ఒకవేళ ( FT_RENEW_ARRAY( పునాది->subglyphs, old_max, new_max ) )
        వెళ్ళు Exit;

      loader->max_subglyphs = new_max;

      FT_GlyphLoader_Adjust_Subglyphs( loader );
    }

  Exit:
    తిరిగి error;
  }


  /* prepare loader for the addition of a new glyph on top of the base one */
  FT_పునాది_DEF( శూన్య )
  FT_GlyphLoader_Prepare( FT_GlyphLoader  loader )
  {
    FT_Glyph_ఎక్కించు  current = &loader->current;


    current->outline.n_points   = 0;
    current->outline.n_contours = 0;
    current->అంకె_subglyphs      = 0;

    FT_GlyphLoader_Adjust_Points   ( loader );
    FT_GlyphLoader_Adjust_Subglyphs( loader );
  }


  /* add current glyph to the base image -- and prepare for another */
  FT_పునాది_DEF( శూన్య )
  FT_Glyph_Loader_కూడు( FT_GlyphLoader  loader )
  {
    FT_Outline*  పునాది;
    FT_Outline*  current;
    FT_Int       n;


    ఒకవేళ ( !loader )
      తిరిగి;

    పునాది    = &loader->పునాది.outline;
    current = &loader->current.outline;

    /* adjust contours count in newest outline */
    కొరకు ( n = 0; n < current->n_contours; n++ )
      current->contours[n] += పునాది->n_points;

    పునాది->n_points   += current->n_points;
    పునాది->n_contours += current->n_contours;

    loader->పునాది.అంకె_subglyphs += loader->current.అంకె_subglyphs;

    /* prepare for another new glyph image */
    FT_GlyphLoader_Prepare( loader );
  }


/*చివర*/
