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


#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftdebug.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftgloadr.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftmemory.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftobjs.h>

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ  FT_COMPONENT
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FT_COMPONENT  gloader


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
  FT_ਅਧਾਰ_DEF( FT_Error )
  FT_GlyphLoader_New( FT_ਸਮ੍ਰਿਤੀ        ਸਮ੍ਰਿਤੀ,
                      FT_GlyphLoader  *aloader )
  {
    FT_GlyphLoader  loader = ਨਿਰਾ;
    FT_Error        error;


    ਜੇ ( !FT_NEW( loader ) )
    {
      loader->ਸਮ੍ਰਿਤੀ = ਸਮ੍ਰਿਤੀ;
      *aloader       = loader;
    }
    ਵਾਪਸ error;
  }


  /* rewind the glyph loader - reset counters to 0 */
  FT_ਅਧਾਰ_DEF( ਖਾਲੀ )
  FT_GlyphLoader_Rewind( FT_GlyphLoader  loader )
  {
    FT_Glyph_ਲੱਦੋ  ਅਧਾਰ    = &loader->ਅਧਾਰ;
    FT_Glyph_ਲੱਦੋ  current = &loader->current;


    ਅਧਾਰ->outline.n_points   = 0;
    ਅਧਾਰ->outline.n_contours = 0;
    ਅਧਾਰ->outline.flags      = 0;
    ਅਧਾਰ->ਅੰਕ_subglyphs      = 0;

    *current = *ਅਧਾਰ;
  }


  /* reset glyph loader, free all allocated tables, */
  /* and start from zero                            */
  FT_ਅਧਾਰ_DEF( ਖਾਲੀ )
  FT_GlyphLoader_Reset( FT_GlyphLoader  loader )
  {
    FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ = loader->ਸਮ੍ਰਿਤੀ;


    FT_FREE( loader->ਅਧਾਰ.outline.points );
    FT_FREE( loader->ਅਧਾਰ.outline.ਟੈਗ_ਸੂਚੀ );
    FT_FREE( loader->ਅਧਾਰ.outline.contours );
    FT_FREE( loader->ਅਧਾਰ.extra_points );
    FT_FREE( loader->ਅਧਾਰ.subglyphs );

    loader->ਅਧਾਰ.extra_points2 = ਨਿਰਾ;

    loader->max_points    = 0;
    loader->max_contours  = 0;
    loader->max_subglyphs = 0;

    FT_GlyphLoader_Rewind( loader );
  }


  /* delete a glyph loader */
  FT_ਅਧਾਰ_DEF( ਖਾਲੀ )
  FT_Glyph_Loader_ਪੂਰਾ( FT_GlyphLoader  loader )
  {
    ਜੇ ( loader )
    {
      FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ = loader->ਸਮ੍ਰਿਤੀ;


      FT_GlyphLoader_Reset( loader );
      FT_FREE( loader );
    }
  }


  /* re-adjust the `current' outline fields */
  ਸਥਿਰ ਖਾਲੀ
  FT_GlyphLoader_Adjust_Points( FT_GlyphLoader  loader )
  {
    FT_Outline*  ਅਧਾਰ    = &loader->ਅਧਾਰ.outline;
    FT_Outline*  current = &loader->current.outline;


    current->points   = FT_ਵਿੱਥ( ਅਧਾਰ->points,   ਅਧਾਰ->n_points );
    current->ਟੈਗ_ਸੂਚੀ     = FT_ਵਿੱਥ( ਅਧਾਰ->ਟੈਗ_ਸੂਚੀ,     ਅਧਾਰ->n_points );
    current->contours = FT_ਵਿੱਥ( ਅਧਾਰ->contours, ਅਧਾਰ->n_contours );

    /* handle extra points table - if any */
    ਜੇ ( loader->use_extra )
    {
      loader->current.extra_points  = loader->ਅਧਾਰ.extra_points +
                                      ਅਧਾਰ->n_points;

      loader->current.extra_points2 = loader->ਅਧਾਰ.extra_points2 +
                                      ਅਧਾਰ->n_points;
    }
  }


  FT_ਅਧਾਰ_DEF( FT_Error )
  FT_GlyphLoader_CreateExtra( FT_GlyphLoader  loader )
  {
    FT_Error   error;
    FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ = loader->ਸਮ੍ਰਿਤੀ;


    ਜੇ ( loader->max_points == 0           ||
         loader->ਅਧਾਰ.extra_points != ਨਿਰਾ )
      ਵਾਪਸ FT_Err_Ok;

    ਜੇ ( !FT_NEW_ARRAY( loader->ਅਧਾਰ.extra_points, 2 * loader->max_points ) )
    {
      loader->use_extra          = 1;
      loader->ਅਧਾਰ.extra_points2 = loader->ਅਧਾਰ.extra_points +
                                   loader->max_points;

      FT_GlyphLoader_Adjust_Points( loader );
    }
    ਵਾਪਸ error;
  }


  /* re-adjust the `current' subglyphs field */
  ਸਥਿਰ ਖਾਲੀ
  FT_GlyphLoader_Adjust_Subglyphs( FT_GlyphLoader  loader )
  {
    FT_Glyph_ਲੱਦੋ  ਅਧਾਰ    = &loader->ਅਧਾਰ;
    FT_Glyph_ਲੱਦੋ  current = &loader->current;


    current->subglyphs = FT_ਵਿੱਥ( ਅਧਾਰ->subglyphs, ਅਧਾਰ->ਅੰਕ_subglyphs );
  }


  /* Ensure that we can add `n_points' and `n_contours' to our glyph.      */
  /* This function reallocates its outline tables if necessary.  Note that */
  /* it DOESN'T change the number of points within the loader!             */
  /*                                                                       */
  FT_ਅਧਾਰ_DEF( FT_Error )
  FT_GlyphLoader_CheckPoints( FT_GlyphLoader  loader,
                              FT_UInt         n_points,
                              FT_UInt         n_contours )
  {
    FT_ਸਮ੍ਰਿਤੀ    ਸਮ੍ਰਿਤੀ  = loader->ਸਮ੍ਰਿਤੀ;
    FT_Error     error   = FT_Err_Ok;
    FT_Outline*  ਅਧਾਰ    = &loader->ਅਧਾਰ.outline;
    FT_Outline*  current = &loader->current.outline;
    FT_Bool      adjust  = 0;

    FT_UInt  new_max, old_max, min_new_max;


    error = FT_GlyphLoader_CreateExtra( loader );
    ਜੇ ( error )
      ਜਾਓ Exit;

    /* check points & tags */
    new_max = (FT_UInt)ਅਧਾਰ->n_points + (FT_UInt)current->n_points +
              n_points;
    old_max = loader->max_points;

    ਜੇ ( new_max > old_max )
    {
      ਜੇ ( new_max > FT_OUTLINE_POINTS_MAX )
      {
        error = FT_THROW( Array_Too_Large );
        ਜਾਓ Exit;
      }

      min_new_max = old_max + ( old_max >> 1 );
      ਜੇ ( new_max < min_new_max )
        new_max = min_new_max;
      new_max = FT_PAD_CEIL( new_max, 8 );
      ਜੇ ( new_max > FT_OUTLINE_POINTS_MAX )
        new_max = FT_OUTLINE_POINTS_MAX;

      ਜੇ ( FT_RENEW_ARRAY( ਅਧਾਰ->points, old_max, new_max ) ||
           FT_RENEW_ARRAY( ਅਧਾਰ->ਟੈਗ_ਸੂਚੀ,   old_max, new_max ) )
        ਜਾਓ Exit;

      ਜੇ ( loader->use_extra )
      {
        ਜੇ ( FT_RENEW_ARRAY( loader->ਅਧਾਰ.extra_points,
                             old_max * 2, new_max * 2 ) )
          ਜਾਓ Exit;

        FT_ARRAY_MOVE( loader->ਅਧਾਰ.extra_points + new_max,
                       loader->ਅਧਾਰ.extra_points + old_max,
                       old_max );

        loader->ਅਧਾਰ.extra_points2 = loader->ਅਧਾਰ.extra_points + new_max;
      }

      adjust = 1;
      loader->max_points = new_max;
    }

    error = FT_GlyphLoader_CreateExtra( loader );
    ਜੇ ( error )
      ਜਾਓ Exit;

    /* check contours */
    old_max = loader->max_contours;
    new_max = (FT_UInt)ਅਧਾਰ->n_contours + (FT_UInt)current->n_contours +
              n_contours;
    ਜੇ ( new_max > old_max )
    {
      ਜੇ ( new_max > FT_OUTLINE_CONTOURS_MAX )
      {
        error = FT_THROW( Array_Too_Large );
        ਜਾਓ Exit;
      }

      min_new_max = old_max + ( old_max >> 1 );
      ਜੇ ( new_max < min_new_max )
        new_max = min_new_max;
      new_max = FT_PAD_CEIL( new_max, 4 );
      ਜੇ ( new_max > FT_OUTLINE_CONTOURS_MAX )
        new_max = FT_OUTLINE_CONTOURS_MAX;

      ਜੇ ( FT_RENEW_ARRAY( ਅਧਾਰ->contours, old_max, new_max ) )
        ਜਾਓ Exit;

      adjust = 1;
      loader->max_contours = new_max;
    }

    ਜੇ ( adjust )
      FT_GlyphLoader_Adjust_Points( loader );

  Exit:
    ਜੇ ( error )
      FT_GlyphLoader_Reset( loader );

    ਵਾਪਸ error;
  }


  /* Ensure that we can add `n_subglyphs' to our glyph. this function */
  /* reallocates its subglyphs table if necessary.  Note that it DOES */
  /* NOT change the number of subglyphs within the loader!            */
  /*                                                                  */
  FT_ਅਧਾਰ_DEF( FT_Error )
  FT_GlyphLoader_CheckSubGlyphs( FT_GlyphLoader  loader,
                                 FT_UInt         n_subs )
  {
    FT_ਸਮ੍ਰਿਤੀ     ਸਮ੍ਰਿਤੀ = loader->ਸਮ੍ਰਿਤੀ;
    FT_Error      error  = FT_Err_Ok;
    FT_UInt       new_max, old_max;

    FT_Glyph_ਲੱਦੋ  ਅਧਾਰ    = &loader->ਅਧਾਰ;
    FT_Glyph_ਲੱਦੋ  current = &loader->current;


    new_max = ਅਧਾਰ->ਅੰਕ_subglyphs + current->ਅੰਕ_subglyphs + n_subs;
    old_max = loader->max_subglyphs;
    ਜੇ ( new_max > old_max )
    {
      new_max = FT_PAD_CEIL( new_max, 2 );
      ਜੇ ( FT_RENEW_ARRAY( ਅਧਾਰ->subglyphs, old_max, new_max ) )
        ਜਾਓ Exit;

      loader->max_subglyphs = new_max;

      FT_GlyphLoader_Adjust_Subglyphs( loader );
    }

  Exit:
    ਵਾਪਸ error;
  }


  /* prepare loader for the addition of a new glyph on top of the base one */
  FT_ਅਧਾਰ_DEF( ਖਾਲੀ )
  FT_GlyphLoader_Prepare( FT_GlyphLoader  loader )
  {
    FT_Glyph_ਲੱਦੋ  current = &loader->current;


    current->outline.n_points   = 0;
    current->outline.n_contours = 0;
    current->ਅੰਕ_subglyphs      = 0;

    FT_GlyphLoader_Adjust_Points   ( loader );
    FT_GlyphLoader_Adjust_Subglyphs( loader );
  }


  /* add current glyph to the base image -- and prepare for another */
  FT_ਅਧਾਰ_DEF( ਖਾਲੀ )
  FT_Glyph_Loader_ਜੋੜੋ( FT_GlyphLoader  loader )
  {
    FT_Outline*  ਅਧਾਰ;
    FT_Outline*  current;
    FT_Int       n;


    ਜੇ ( !loader )
      ਵਾਪਸ;

    ਅਧਾਰ    = &loader->ਅਧਾਰ.outline;
    current = &loader->current.outline;

    /* adjust contours count in newest outline */
    ਲਈ ( n = 0; n < current->n_contours; n++ )
      current->contours[n] += ਅਧਾਰ->n_points;

    ਅਧਾਰ->n_points   += current->n_points;
    ਅਧਾਰ->n_contours += current->n_contours;

    loader->ਅਧਾਰ.ਅੰਕ_subglyphs += loader->current.ਅੰਕ_subglyphs;

    /* prepare for another new glyph image */
    FT_GlyphLoader_Prepare( loader );
  }


/*ਸਿਰਾ*/
