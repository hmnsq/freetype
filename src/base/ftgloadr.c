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


#ضمن <freetype/internal/ftdebug.h>
#ضمن <freetype/internal/ftgloadr.h>
#ضمن <freetype/internal/ftmemory.h>
#ضمن <freetype/internal/ftobjs.h>

#تعریف_لغوه  FT_COMPONENT
#تعریف_کړه FT_COMPONENT  gloader


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
  FT_بنسټ_DEF( FT_Error )
  FT_GlyphLoader_New( FT_حافظه        حافظه,
                      FT_GlyphLoader  *aloader )
  {
    FT_GlyphLoader  loader = عدم;
    FT_Error        error;


    إذا ( !FT_NEW( loader ) )
    {
      loader->حافظه = حافظه;
      *aloader       = loader;
    }
    أرجع error;
  }


  /* rewind the glyph loader - reset counters to 0 */
  FT_بنسټ_DEF( فراغ )
  FT_GlyphLoader_Rewind( FT_GlyphLoader  loader )
  {
    FT_Glyph_بارول  بنسټ    = &loader->بنسټ;
    FT_Glyph_بارول  current = &loader->current;


    بنسټ->outline.n_points   = 0;
    بنسټ->outline.n_contours = 0;
    بنسټ->outline.flags      = 0;
    بنسټ->عدد_subglyphs      = 0;

    *current = *بنسټ;
  }


  /* reset glyph loader, free all allocated tables, */
  /* and start from zero                            */
  FT_بنسټ_DEF( فراغ )
  FT_GlyphLoader_Reset( FT_GlyphLoader  loader )
  {
    FT_حافظه  حافظه = loader->حافظه;


    FT_FREE( loader->بنسټ.outline.points );
    FT_FREE( loader->بنسټ.outline.نښانونه );
    FT_FREE( loader->بنسټ.outline.contours );
    FT_FREE( loader->بنسټ.extra_points );
    FT_FREE( loader->بنسټ.subglyphs );

    loader->بنسټ.extra_points2 = عدم;

    loader->max_points    = 0;
    loader->max_contours  = 0;
    loader->max_subglyphs = 0;

    FT_GlyphLoader_Rewind( loader );
  }


  /* delete a glyph loader */
  FT_بنسټ_DEF( فراغ )
  FT_Glyph_Loader_بشپړ( FT_GlyphLoader  loader )
  {
    إذا ( loader )
    {
      FT_حافظه  حافظه = loader->حافظه;


      FT_GlyphLoader_Reset( loader );
      FT_FREE( loader );
    }
  }


  /* re-adjust the `current' outline fields */
  ساكن فراغ
  FT_GlyphLoader_Adjust_Points( FT_GlyphLoader  loader )
  {
    FT_Outline*  بنسټ    = &loader->بنسټ.outline;
    FT_Outline*  current = &loader->current.outline;


    current->points   = FT_دننه_واټن( بنسټ->points,   بنسټ->n_points );
    current->نښانونه     = FT_دننه_واټن( بنسټ->نښانونه,     بنسټ->n_points );
    current->contours = FT_دننه_واټن( بنسټ->contours, بنسټ->n_contours );

    /* handle extra points table - if any */
    إذا ( loader->use_extra )
    {
      loader->current.extra_points  = loader->بنسټ.extra_points +
                                      بنسټ->n_points;

      loader->current.extra_points2 = loader->بنسټ.extra_points2 +
                                      بنسټ->n_points;
    }
  }


  FT_بنسټ_DEF( FT_Error )
  FT_GlyphLoader_CreateExtra( FT_GlyphLoader  loader )
  {
    FT_Error   error;
    FT_حافظه  حافظه = loader->حافظه;


    إذا ( loader->max_points == 0           ||
         loader->بنسټ.extra_points != عدم )
      أرجع FT_Err_Ok;

    إذا ( !FT_NEW_ARRAY( loader->بنسټ.extra_points, 2 * loader->max_points ) )
    {
      loader->use_extra          = 1;
      loader->بنسټ.extra_points2 = loader->بنسټ.extra_points +
                                   loader->max_points;

      FT_GlyphLoader_Adjust_Points( loader );
    }
    أرجع error;
  }


  /* re-adjust the `current' subglyphs field */
  ساكن فراغ
  FT_GlyphLoader_Adjust_Subglyphs( FT_GlyphLoader  loader )
  {
    FT_Glyph_بارول  بنسټ    = &loader->بنسټ;
    FT_Glyph_بارول  current = &loader->current;


    current->subglyphs = FT_دننه_واټن( بنسټ->subglyphs, بنسټ->عدد_subglyphs );
  }


  /* Ensure that we can add `n_points' and `n_contours' to our glyph.      */
  /* This function reallocates its outline tables if necessary.  Note that */
  /* it DOESN'T change the number of points within the loader!             */
  /*                                                                       */
  FT_بنسټ_DEF( FT_Error )
  FT_GlyphLoader_CheckPoints( FT_GlyphLoader  loader,
                              FT_UInt         n_points,
                              FT_UInt         n_contours )
  {
    FT_حافظه    حافظه  = loader->حافظه;
    FT_Error     error   = FT_Err_Ok;
    FT_Outline*  بنسټ    = &loader->بنسټ.outline;
    FT_Outline*  current = &loader->current.outline;
    FT_Bool      adjust  = 0;

    FT_UInt  new_max, old_max, min_new_max;


    error = FT_GlyphLoader_CreateExtra( loader );
    إذا ( error )
      اذهب Exit;

    /* check points & tags */
    new_max = (FT_UInt)بنسټ->n_points + (FT_UInt)current->n_points +
              n_points;
    old_max = loader->max_points;

    إذا ( new_max > old_max )
    {
      إذا ( new_max > FT_OUTLINE_POINTS_MAX )
      {
        error = FT_THROW( Array_Too_Large );
        اذهب Exit;
      }

      min_new_max = old_max + ( old_max >> 1 );
      إذا ( new_max < min_new_max )
        new_max = min_new_max;
      new_max = FT_PAD_CEIL( new_max, 8 );
      إذا ( new_max > FT_OUTLINE_POINTS_MAX )
        new_max = FT_OUTLINE_POINTS_MAX;

      إذا ( FT_RENEW_ARRAY( بنسټ->points, old_max, new_max ) ||
           FT_RENEW_ARRAY( بنسټ->نښانونه,   old_max, new_max ) )
        اذهب Exit;

      إذا ( loader->use_extra )
      {
        إذا ( FT_RENEW_ARRAY( loader->بنسټ.extra_points,
                             old_max * 2, new_max * 2 ) )
          اذهب Exit;

        FT_ARRAY_MOVE( loader->بنسټ.extra_points + new_max,
                       loader->بنسټ.extra_points + old_max,
                       old_max );

        loader->بنسټ.extra_points2 = loader->بنسټ.extra_points + new_max;
      }

      adjust = 1;
      loader->max_points = new_max;
    }

    error = FT_GlyphLoader_CreateExtra( loader );
    إذا ( error )
      اذهب Exit;

    /* check contours */
    old_max = loader->max_contours;
    new_max = (FT_UInt)بنسټ->n_contours + (FT_UInt)current->n_contours +
              n_contours;
    إذا ( new_max > old_max )
    {
      إذا ( new_max > FT_OUTLINE_CONTOURS_MAX )
      {
        error = FT_THROW( Array_Too_Large );
        اذهب Exit;
      }

      min_new_max = old_max + ( old_max >> 1 );
      إذا ( new_max < min_new_max )
        new_max = min_new_max;
      new_max = FT_PAD_CEIL( new_max, 4 );
      إذا ( new_max > FT_OUTLINE_CONTOURS_MAX )
        new_max = FT_OUTLINE_CONTOURS_MAX;

      إذا ( FT_RENEW_ARRAY( بنسټ->contours, old_max, new_max ) )
        اذهب Exit;

      adjust = 1;
      loader->max_contours = new_max;
    }

    إذا ( adjust )
      FT_GlyphLoader_Adjust_Points( loader );

  Exit:
    إذا ( error )
      FT_GlyphLoader_Reset( loader );

    أرجع error;
  }


  /* Ensure that we can add `n_subglyphs' to our glyph. this function */
  /* reallocates its subglyphs table if necessary.  Note that it DOES */
  /* NOT change the number of subglyphs within the loader!            */
  /*                                                                  */
  FT_بنسټ_DEF( FT_Error )
  FT_GlyphLoader_CheckSubGlyphs( FT_GlyphLoader  loader,
                                 FT_UInt         n_subs )
  {
    FT_حافظه     حافظه = loader->حافظه;
    FT_Error      error  = FT_Err_Ok;
    FT_UInt       new_max, old_max;

    FT_Glyph_بارول  بنسټ    = &loader->بنسټ;
    FT_Glyph_بارول  current = &loader->current;


    new_max = بنسټ->عدد_subglyphs + current->عدد_subglyphs + n_subs;
    old_max = loader->max_subglyphs;
    إذا ( new_max > old_max )
    {
      new_max = FT_PAD_CEIL( new_max, 2 );
      إذا ( FT_RENEW_ARRAY( بنسټ->subglyphs, old_max, new_max ) )
        اذهب Exit;

      loader->max_subglyphs = new_max;

      FT_GlyphLoader_Adjust_Subglyphs( loader );
    }

  Exit:
    أرجع error;
  }


  /* prepare loader for the addition of a new glyph on top of the base one */
  FT_بنسټ_DEF( فراغ )
  FT_GlyphLoader_Prepare( FT_GlyphLoader  loader )
  {
    FT_Glyph_بارول  current = &loader->current;


    current->outline.n_points   = 0;
    current->outline.n_contours = 0;
    current->عدد_subglyphs      = 0;

    FT_GlyphLoader_Adjust_Points   ( loader );
    FT_GlyphLoader_Adjust_Subglyphs( loader );
  }


  /* add current glyph to the base image -- and prepare for another */
  FT_بنسټ_DEF( فراغ )
  FT_Glyph_Loader_جمع_کړه( FT_GlyphLoader  loader )
  {
    FT_Outline*  بنسټ;
    FT_Outline*  current;
    FT_Int       n;


    إذا ( !loader )
      أرجع;

    بنسټ    = &loader->بنسټ.outline;
    current = &loader->current.outline;

    /* adjust contours count in newest outline */
    لكل ( n = 0; n < current->n_contours; n++ )
      current->contours[n] += بنسټ->n_points;

    بنسټ->n_points   += current->n_points;
    بنسټ->n_contours += current->n_contours;

    loader->بنسټ.عدد_subglyphs += loader->current.عدد_subglyphs;

    /* prepare for another new glyph image */
    FT_GlyphLoader_Prepare( loader );
  }


/*پای_ټکی*/
