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


#ಅನ್ತರ್ಭಾವ <freetype/internal/ftdebug.h>
#ಅನ್ತರ್ಭಾವ <freetype/internal/ftgloadr.h>
#ಅನ್ತರ್ಭಾವ <freetype/internal/ftmemory.h>
#ಅನ್ತರ್ಭಾವ <freetype/internal/ftobjs.h>

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು  FT_COMPONENT
#ವ್ಯಾಖ್ಯಾನಿಸು FT_COMPONENT  gloader


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
  FT_ತಳ_DEF( FT_Error )
  FT_GlyphLoader_New( FT_ಸ್ಮರಣೆ        ಸ್ಮರಣೆ,
                      FT_GlyphLoader  *aloader )
  {
    FT_GlyphLoader  loader = ಖಾಲಿ;
    FT_Error        error;


    ಆದರೆ ( !FT_NEW( loader ) )
    {
      loader->ಸ್ಮರಣೆ = ಸ್ಮರಣೆ;
      *aloader       = loader;
    }
    ಹಿಂತಿರುಗಿ error;
  }


  /* rewind the glyph loader - reset counters to 0 */
  FT_ತಳ_DEF( ಶೂನ್ಯ )
  FT_GlyphLoader_Rewind( FT_GlyphLoader  loader )
  {
    FT_Glyph_ತುಂಬು  ತಳ    = &loader->ತಳ;
    FT_Glyph_ತುಂಬು  current = &loader->current;


    ತಳ->outline.n_points   = 0;
    ತಳ->outline.n_contours = 0;
    ತಳ->outline.flags      = 0;
    ತಳ->ಅಂಕೆ_subglyphs      = 0;

    *current = *ತಳ;
  }


  /* reset glyph loader, free all allocated tables, */
  /* and start from zero                            */
  FT_ತಳ_DEF( ಶೂನ್ಯ )
  FT_GlyphLoader_Reset( FT_GlyphLoader  loader )
  {
    FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ = loader->ಸ್ಮರಣೆ;


    FT_FREE( loader->ತಳ.outline.points );
    FT_FREE( loader->ತಳ.outline.ಟ್ಯಾಗುಗಳು );
    FT_FREE( loader->ತಳ.outline.contours );
    FT_FREE( loader->ತಳ.extra_points );
    FT_FREE( loader->ತಳ.subglyphs );

    loader->ತಳ.extra_points2 = ಖಾಲಿ;

    loader->max_points    = 0;
    loader->max_contours  = 0;
    loader->max_subglyphs = 0;

    FT_GlyphLoader_Rewind( loader );
  }


  /* delete a glyph loader */
  FT_ತಳ_DEF( ಶೂನ್ಯ )
  FT_Glyph_Loader_ಮುಗಿದ( FT_GlyphLoader  loader )
  {
    ಆದರೆ ( loader )
    {
      FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ = loader->ಸ್ಮರಣೆ;


      FT_GlyphLoader_Reset( loader );
      FT_FREE( loader );
    }
  }


  /* re-adjust the `current' outline fields */
  ಸ್ಥಿರ ಶೂನ್ಯ
  FT_GlyphLoader_Adjust_Points( FT_GlyphLoader  loader )
  {
    FT_Outline*  ತಳ    = &loader->ತಳ.outline;
    FT_Outline*  current = &loader->current.outline;


    current->points   = FT_ಸ್ಥಾನಾಂತರ( ತಳ->points,   ತಳ->n_points );
    current->ಟ್ಯಾಗುಗಳು     = FT_ಸ್ಥಾನಾಂತರ( ತಳ->ಟ್ಯಾಗುಗಳು,     ತಳ->n_points );
    current->contours = FT_ಸ್ಥಾನಾಂತರ( ತಳ->contours, ತಳ->n_contours );

    /* handle extra points table - if any */
    ಆದರೆ ( loader->use_extra )
    {
      loader->current.extra_points  = loader->ತಳ.extra_points +
                                      ತಳ->n_points;

      loader->current.extra_points2 = loader->ತಳ.extra_points2 +
                                      ತಳ->n_points;
    }
  }


  FT_ತಳ_DEF( FT_Error )
  FT_GlyphLoader_CreateExtra( FT_GlyphLoader  loader )
  {
    FT_Error   error;
    FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ = loader->ಸ್ಮರಣೆ;


    ಆದರೆ ( loader->max_points == 0           ||
         loader->ತಳ.extra_points != ಖಾಲಿ )
      ಹಿಂತಿರುಗಿ FT_Err_Ok;

    ಆದರೆ ( !FT_NEW_ARRAY( loader->ತಳ.extra_points, 2 * loader->max_points ) )
    {
      loader->use_extra          = 1;
      loader->ತಳ.extra_points2 = loader->ತಳ.extra_points +
                                   loader->max_points;

      FT_GlyphLoader_Adjust_Points( loader );
    }
    ಹಿಂತಿರುಗಿ error;
  }


  /* re-adjust the `current' subglyphs field */
  ಸ್ಥಿರ ಶೂನ್ಯ
  FT_GlyphLoader_Adjust_Subglyphs( FT_GlyphLoader  loader )
  {
    FT_Glyph_ತುಂಬು  ತಳ    = &loader->ತಳ;
    FT_Glyph_ತುಂಬು  current = &loader->current;


    current->subglyphs = FT_ಸ್ಥಾನಾಂತರ( ತಳ->subglyphs, ತಳ->ಅಂಕೆ_subglyphs );
  }


  /* Ensure that we can add `n_points' and `n_contours' to our glyph.      */
  /* This function reallocates its outline tables if necessary.  Note that */
  /* it DOESN'T change the number of points within the loader!             */
  /*                                                                       */
  FT_ತಳ_DEF( FT_Error )
  FT_GlyphLoader_CheckPoints( FT_GlyphLoader  loader,
                              FT_UInt         n_points,
                              FT_UInt         n_contours )
  {
    FT_ಸ್ಮರಣೆ    ಸ್ಮರಣೆ  = loader->ಸ್ಮರಣೆ;
    FT_Error     error   = FT_Err_Ok;
    FT_Outline*  ತಳ    = &loader->ತಳ.outline;
    FT_Outline*  current = &loader->current.outline;
    FT_Bool      adjust  = 0;

    FT_UInt  new_max, old_max, min_new_max;


    error = FT_GlyphLoader_CreateExtra( loader );
    ಆದರೆ ( error )
      ಹೋಗು Exit;

    /* check points & tags */
    new_max = (FT_UInt)ತಳ->n_points + (FT_UInt)current->n_points +
              n_points;
    old_max = loader->max_points;

    ಆದರೆ ( new_max > old_max )
    {
      ಆದರೆ ( new_max > FT_OUTLINE_POINTS_MAX )
      {
        error = FT_THROW( Array_Too_Large );
        ಹೋಗು Exit;
      }

      min_new_max = old_max + ( old_max >> 1 );
      ಆದರೆ ( new_max < min_new_max )
        new_max = min_new_max;
      new_max = FT_PAD_CEIL( new_max, 8 );
      ಆದರೆ ( new_max > FT_OUTLINE_POINTS_MAX )
        new_max = FT_OUTLINE_POINTS_MAX;

      ಆದರೆ ( FT_RENEW_ARRAY( ತಳ->points, old_max, new_max ) ||
           FT_RENEW_ARRAY( ತಳ->ಟ್ಯಾಗುಗಳು,   old_max, new_max ) )
        ಹೋಗು Exit;

      ಆದರೆ ( loader->use_extra )
      {
        ಆದರೆ ( FT_RENEW_ARRAY( loader->ತಳ.extra_points,
                             old_max * 2, new_max * 2 ) )
          ಹೋಗು Exit;

        FT_ARRAY_MOVE( loader->ತಳ.extra_points + new_max,
                       loader->ತಳ.extra_points + old_max,
                       old_max );

        loader->ತಳ.extra_points2 = loader->ತಳ.extra_points + new_max;
      }

      adjust = 1;
      loader->max_points = new_max;
    }

    error = FT_GlyphLoader_CreateExtra( loader );
    ಆದರೆ ( error )
      ಹೋಗು Exit;

    /* check contours */
    old_max = loader->max_contours;
    new_max = (FT_UInt)ತಳ->n_contours + (FT_UInt)current->n_contours +
              n_contours;
    ಆದರೆ ( new_max > old_max )
    {
      ಆದರೆ ( new_max > FT_OUTLINE_CONTOURS_MAX )
      {
        error = FT_THROW( Array_Too_Large );
        ಹೋಗು Exit;
      }

      min_new_max = old_max + ( old_max >> 1 );
      ಆದರೆ ( new_max < min_new_max )
        new_max = min_new_max;
      new_max = FT_PAD_CEIL( new_max, 4 );
      ಆದರೆ ( new_max > FT_OUTLINE_CONTOURS_MAX )
        new_max = FT_OUTLINE_CONTOURS_MAX;

      ಆದರೆ ( FT_RENEW_ARRAY( ತಳ->contours, old_max, new_max ) )
        ಹೋಗು Exit;

      adjust = 1;
      loader->max_contours = new_max;
    }

    ಆದರೆ ( adjust )
      FT_GlyphLoader_Adjust_Points( loader );

  Exit:
    ಆದರೆ ( error )
      FT_GlyphLoader_Reset( loader );

    ಹಿಂತಿರುಗಿ error;
  }


  /* Ensure that we can add `n_subglyphs' to our glyph. this function */
  /* reallocates its subglyphs table if necessary.  Note that it DOES */
  /* NOT change the number of subglyphs within the loader!            */
  /*                                                                  */
  FT_ತಳ_DEF( FT_Error )
  FT_GlyphLoader_CheckSubGlyphs( FT_GlyphLoader  loader,
                                 FT_UInt         n_subs )
  {
    FT_ಸ್ಮರಣೆ     ಸ್ಮರಣೆ = loader->ಸ್ಮರಣೆ;
    FT_Error      error  = FT_Err_Ok;
    FT_UInt       new_max, old_max;

    FT_Glyph_ತುಂಬು  ತಳ    = &loader->ತಳ;
    FT_Glyph_ತುಂಬು  current = &loader->current;


    new_max = ತಳ->ಅಂಕೆ_subglyphs + current->ಅಂಕೆ_subglyphs + n_subs;
    old_max = loader->max_subglyphs;
    ಆದರೆ ( new_max > old_max )
    {
      new_max = FT_PAD_CEIL( new_max, 2 );
      ಆದರೆ ( FT_RENEW_ARRAY( ತಳ->subglyphs, old_max, new_max ) )
        ಹೋಗು Exit;

      loader->max_subglyphs = new_max;

      FT_GlyphLoader_Adjust_Subglyphs( loader );
    }

  Exit:
    ಹಿಂತಿರುಗಿ error;
  }


  /* prepare loader for the addition of a new glyph on top of the base one */
  FT_ತಳ_DEF( ಶೂನ್ಯ )
  FT_GlyphLoader_Prepare( FT_GlyphLoader  loader )
  {
    FT_Glyph_ತುಂಬು  current = &loader->current;


    current->outline.n_points   = 0;
    current->outline.n_contours = 0;
    current->ಅಂಕೆ_subglyphs      = 0;

    FT_GlyphLoader_Adjust_Points   ( loader );
    FT_GlyphLoader_Adjust_Subglyphs( loader );
  }


  /* add current glyph to the base image -- and prepare for another */
  FT_ತಳ_DEF( ಶೂನ್ಯ )
  FT_Glyph_Loader_ಕೂಡಿಸು( FT_GlyphLoader  loader )
  {
    FT_Outline*  ತಳ;
    FT_Outline*  current;
    FT_Int       n;


    ಆದರೆ ( !loader )
      ಹಿಂತಿರುಗಿ;

    ತಳ    = &loader->ತಳ.outline;
    current = &loader->current.outline;

    /* adjust contours count in newest outline */
    ಗಾಗಿ ( n = 0; n < current->n_contours; n++ )
      current->contours[n] += ತಳ->n_points;

    ತಳ->n_points   += current->n_points;
    ತಳ->n_contours += current->n_contours;

    loader->ತಳ.ಅಂಕೆ_subglyphs += loader->current.ಅಂಕೆ_subglyphs;

    /* prepare for another new glyph image */
    FT_GlyphLoader_Prepare( loader );
  }


/*ಕೊನೆ*/
