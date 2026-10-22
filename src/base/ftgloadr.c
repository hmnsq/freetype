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

#وصف_رد  FT_COMPONENT
#وصف_ڪريو FT_COMPONENT  gloader


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
  FT_بنياد_DEF( FT_Error )
  FT_GlyphLoader_New( FT_يادگيري        يادگيري,
                      FT_GlyphLoader  *aloader )
  {
    FT_GlyphLoader  loader = عدم;
    FT_Error        error;


    إذا ( !FT_NEW( loader ) )
    {
      loader->يادگيري = يادگيري;
      *aloader       = loader;
    }
    أرجع error;
  }


  /* rewind the glyph loader - reset counters to 0 */
  FT_بنياد_DEF( فراغ )
  FT_GlyphLoader_Rewind( FT_GlyphLoader  loader )
  {
    FT_Glyph_ڀريو  بنياد    = &loader->بنياد;
    FT_Glyph_ڀريو  current = &loader->current;


    بنياد->outline.n_points   = 0;
    بنياد->outline.n_contours = 0;
    بنياد->outline.flags      = 0;
    بنياد->انگ_subglyphs      = 0;

    *current = *بنياد;
  }


  /* reset glyph loader, free all allocated tables, */
  /* and start from zero                            */
  FT_بنياد_DEF( فراغ )
  FT_GlyphLoader_Reset( FT_GlyphLoader  loader )
  {
    FT_يادگيري  يادگيري = loader->يادگيري;


    FT_FREE( loader->بنياد.outline.points );
    FT_FREE( loader->بنياد.outline.ٽيگ_فهرست );
    FT_FREE( loader->بنياد.outline.contours );
    FT_FREE( loader->بنياد.extra_points );
    FT_FREE( loader->بنياد.subglyphs );

    loader->بنياد.extra_points2 = عدم;

    loader->max_points    = 0;
    loader->max_contours  = 0;
    loader->max_subglyphs = 0;

    FT_GlyphLoader_Rewind( loader );
  }


  /* delete a glyph loader */
  FT_بنياد_DEF( فراغ )
  FT_Glyph_Loader_پورو( FT_GlyphLoader  loader )
  {
    إذا ( loader )
    {
      FT_يادگيري  يادگيري = loader->يادگيري;


      FT_GlyphLoader_Reset( loader );
      FT_FREE( loader );
    }
  }


  /* re-adjust the `current' outline fields */
  ساكن فراغ
  FT_GlyphLoader_Adjust_Points( FT_GlyphLoader  loader )
  {
    FT_Outline*  بنياد    = &loader->بنياد.outline;
    FT_Outline*  current = &loader->current.outline;


    current->points   = FT_وٿي( بنياد->points,   بنياد->n_points );
    current->ٽيگ_فهرست     = FT_وٿي( بنياد->ٽيگ_فهرست,     بنياد->n_points );
    current->contours = FT_وٿي( بنياد->contours, بنياد->n_contours );

    /* handle extra points table - if any */
    إذا ( loader->use_extra )
    {
      loader->current.extra_points  = loader->بنياد.extra_points +
                                      بنياد->n_points;

      loader->current.extra_points2 = loader->بنياد.extra_points2 +
                                      بنياد->n_points;
    }
  }


  FT_بنياد_DEF( FT_Error )
  FT_GlyphLoader_CreateExtra( FT_GlyphLoader  loader )
  {
    FT_Error   error;
    FT_يادگيري  يادگيري = loader->يادگيري;


    إذا ( loader->max_points == 0           ||
         loader->بنياد.extra_points != عدم )
      أرجع FT_Err_Ok;

    إذا ( !FT_NEW_ARRAY( loader->بنياد.extra_points, 2 * loader->max_points ) )
    {
      loader->use_extra          = 1;
      loader->بنياد.extra_points2 = loader->بنياد.extra_points +
                                   loader->max_points;

      FT_GlyphLoader_Adjust_Points( loader );
    }
    أرجع error;
  }


  /* re-adjust the `current' subglyphs field */
  ساكن فراغ
  FT_GlyphLoader_Adjust_Subglyphs( FT_GlyphLoader  loader )
  {
    FT_Glyph_ڀريو  بنياد    = &loader->بنياد;
    FT_Glyph_ڀريو  current = &loader->current;


    current->subglyphs = FT_وٿي( بنياد->subglyphs, بنياد->انگ_subglyphs );
  }


  /* Ensure that we can add `n_points' and `n_contours' to our glyph.      */
  /* This function reallocates its outline tables if necessary.  Note that */
  /* it DOESN'T change the number of points within the loader!             */
  /*                                                                       */
  FT_بنياد_DEF( FT_Error )
  FT_GlyphLoader_CheckPoints( FT_GlyphLoader  loader,
                              FT_UInt         n_points,
                              FT_UInt         n_contours )
  {
    FT_يادگيري    يادگيري  = loader->يادگيري;
    FT_Error     error   = FT_Err_Ok;
    FT_Outline*  بنياد    = &loader->بنياد.outline;
    FT_Outline*  current = &loader->current.outline;
    FT_Bool      adjust  = 0;

    FT_UInt  new_max, old_max, min_new_max;


    error = FT_GlyphLoader_CreateExtra( loader );
    إذا ( error )
      اذهب Exit;

    /* check points & tags */
    new_max = (FT_UInt)بنياد->n_points + (FT_UInt)current->n_points +
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

      إذا ( FT_RENEW_ARRAY( بنياد->points, old_max, new_max ) ||
           FT_RENEW_ARRAY( بنياد->ٽيگ_فهرست,   old_max, new_max ) )
        اذهب Exit;

      إذا ( loader->use_extra )
      {
        إذا ( FT_RENEW_ARRAY( loader->بنياد.extra_points,
                             old_max * 2, new_max * 2 ) )
          اذهب Exit;

        FT_ARRAY_MOVE( loader->بنياد.extra_points + new_max,
                       loader->بنياد.extra_points + old_max,
                       old_max );

        loader->بنياد.extra_points2 = loader->بنياد.extra_points + new_max;
      }

      adjust = 1;
      loader->max_points = new_max;
    }

    error = FT_GlyphLoader_CreateExtra( loader );
    إذا ( error )
      اذهب Exit;

    /* check contours */
    old_max = loader->max_contours;
    new_max = (FT_UInt)بنياد->n_contours + (FT_UInt)current->n_contours +
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

      إذا ( FT_RENEW_ARRAY( بنياد->contours, old_max, new_max ) )
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
  FT_بنياد_DEF( FT_Error )
  FT_GlyphLoader_CheckSubGlyphs( FT_GlyphLoader  loader,
                                 FT_UInt         n_subs )
  {
    FT_يادگيري     يادگيري = loader->يادگيري;
    FT_Error      error  = FT_Err_Ok;
    FT_UInt       new_max, old_max;

    FT_Glyph_ڀريو  بنياد    = &loader->بنياد;
    FT_Glyph_ڀريو  current = &loader->current;


    new_max = بنياد->انگ_subglyphs + current->انگ_subglyphs + n_subs;
    old_max = loader->max_subglyphs;
    إذا ( new_max > old_max )
    {
      new_max = FT_PAD_CEIL( new_max, 2 );
      إذا ( FT_RENEW_ARRAY( بنياد->subglyphs, old_max, new_max ) )
        اذهب Exit;

      loader->max_subglyphs = new_max;

      FT_GlyphLoader_Adjust_Subglyphs( loader );
    }

  Exit:
    أرجع error;
  }


  /* prepare loader for the addition of a new glyph on top of the base one */
  FT_بنياد_DEF( فراغ )
  FT_GlyphLoader_Prepare( FT_GlyphLoader  loader )
  {
    FT_Glyph_ڀريو  current = &loader->current;


    current->outline.n_points   = 0;
    current->outline.n_contours = 0;
    current->انگ_subglyphs      = 0;

    FT_GlyphLoader_Adjust_Points   ( loader );
    FT_GlyphLoader_Adjust_Subglyphs( loader );
  }


  /* add current glyph to the base image -- and prepare for another */
  FT_بنياد_DEF( فراغ )
  FT_Glyph_Loader_جوڙيو( FT_GlyphLoader  loader )
  {
    FT_Outline*  بنياد;
    FT_Outline*  current;
    FT_Int       n;


    إذا ( !loader )
      أرجع;

    بنياد    = &loader->بنياد.outline;
    current = &loader->current.outline;

    /* adjust contours count in newest outline */
    لكل ( n = 0; n < current->n_contours; n++ )
      current->contours[n] += بنياد->n_points;

    بنياد->n_points   += current->n_points;
    بنياد->n_contours += current->n_contours;

    loader->بنياد.انگ_subglyphs += loader->current.انگ_subglyphs;

    /* prepare for another new glyph image */
    FT_GlyphLoader_Prepare( loader );
  }


/*پڇاڙي*/
