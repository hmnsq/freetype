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


#شامل <freetype/internal/ftdebug.h>
#شامل <freetype/internal/ftgloadr.h>
#شامل <freetype/internal/ftmemory.h>
#شامل <freetype/internal/ftobjs.h>

#تعریف_منسوخ  FT_COMPONENT
#تعریف_کرو FT_COMPONENT  gloader


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
  FT_بنیاد_DEF( FT_Error )
  FT_GlyphLoader_New( FT_حافظہ        حافظہ,
                      FT_GlyphLoader  *aloader )
  {
    FT_GlyphLoader  loader = تہی;
    FT_Error        error;


    اگر ( !FT_NEW( loader ) )
    {
      loader->حافظہ = حافظہ;
      *aloader       = loader;
    }
    واپس error;
  }


  /* rewind the glyph loader - reset counters to 0 */
  FT_بنیاد_DEF( خالی )
  FT_GlyphLoader_Rewind( FT_GlyphLoader  loader )
  {
    FT_Glyph_بھرو  بنیاد    = &loader->بنیاد;
    FT_Glyph_بھرو  current = &loader->current;


    بنیاد->outline.n_points   = 0;
    بنیاد->outline.n_contours = 0;
    بنیاد->outline.flags      = 0;
    بنیاد->عدد_subglyphs      = 0;

    *current = *بنیاد;
  }


  /* reset glyph loader, free all allocated tables, */
  /* and start from zero                            */
  FT_بنیاد_DEF( خالی )
  FT_GlyphLoader_Reset( FT_GlyphLoader  loader )
  {
    FT_حافظہ  حافظہ = loader->حافظہ;


    FT_FREE( loader->بنیاد.outline.points );
    FT_FREE( loader->بنیاد.outline.لیبل_فہرست );
    FT_FREE( loader->بنیاد.outline.contours );
    FT_FREE( loader->بنیاد.extra_points );
    FT_FREE( loader->بنیاد.subglyphs );

    loader->بنیاد.extra_points2 = تہی;

    loader->max_points    = 0;
    loader->max_contours  = 0;
    loader->max_subglyphs = 0;

    FT_GlyphLoader_Rewind( loader );
  }


  /* delete a glyph loader */
  FT_بنیاد_DEF( خالی )
  FT_Glyph_Loader_مکمل( FT_GlyphLoader  loader )
  {
    اگر ( loader )
    {
      FT_حافظہ  حافظہ = loader->حافظہ;


      FT_GlyphLoader_Reset( loader );
      FT_FREE( loader );
    }
  }


  /* re-adjust the `current' outline fields */
  ساکن خالی
  FT_GlyphLoader_Adjust_Points( FT_GlyphLoader  loader )
  {
    FT_Outline*  بنیاد    = &loader->بنیاد.outline;
    FT_Outline*  current = &loader->current.outline;


    current->points   = FT_انحراف( بنیاد->points,   بنیاد->n_points );
    current->لیبل_فہرست     = FT_انحراف( بنیاد->لیبل_فہرست,     بنیاد->n_points );
    current->contours = FT_انحراف( بنیاد->contours, بنیاد->n_contours );

    /* handle extra points table - if any */
    اگر ( loader->use_extra )
    {
      loader->current.extra_points  = loader->بنیاد.extra_points +
                                      بنیاد->n_points;

      loader->current.extra_points2 = loader->بنیاد.extra_points2 +
                                      بنیاد->n_points;
    }
  }


  FT_بنیاد_DEF( FT_Error )
  FT_GlyphLoader_CreateExtra( FT_GlyphLoader  loader )
  {
    FT_Error   error;
    FT_حافظہ  حافظہ = loader->حافظہ;


    اگر ( loader->max_points == 0           ||
         loader->بنیاد.extra_points != تہی )
      واپس FT_Err_Ok;

    اگر ( !FT_NEW_ARRAY( loader->بنیاد.extra_points, 2 * loader->max_points ) )
    {
      loader->use_extra          = 1;
      loader->بنیاد.extra_points2 = loader->بنیاد.extra_points +
                                   loader->max_points;

      FT_GlyphLoader_Adjust_Points( loader );
    }
    واپس error;
  }


  /* re-adjust the `current' subglyphs field */
  ساکن خالی
  FT_GlyphLoader_Adjust_Subglyphs( FT_GlyphLoader  loader )
  {
    FT_Glyph_بھرو  بنیاد    = &loader->بنیاد;
    FT_Glyph_بھرو  current = &loader->current;


    current->subglyphs = FT_انحراف( بنیاد->subglyphs, بنیاد->عدد_subglyphs );
  }


  /* Ensure that we can add `n_points' and `n_contours' to our glyph.      */
  /* This function reallocates its outline tables if necessary.  Note that */
  /* it DOESN'T change the number of points within the loader!             */
  /*                                                                       */
  FT_بنیاد_DEF( FT_Error )
  FT_GlyphLoader_CheckPoints( FT_GlyphLoader  loader,
                              FT_UInt         n_points,
                              FT_UInt         n_contours )
  {
    FT_حافظہ    حافظہ  = loader->حافظہ;
    FT_Error     error   = FT_Err_Ok;
    FT_Outline*  بنیاد    = &loader->بنیاد.outline;
    FT_Outline*  current = &loader->current.outline;
    FT_Bool      adjust  = 0;

    FT_UInt  new_max, old_max, min_new_max;


    error = FT_GlyphLoader_CreateExtra( loader );
    اگر ( error )
      جاؤ Exit;

    /* check points & tags */
    new_max = (FT_UInt)بنیاد->n_points + (FT_UInt)current->n_points +
              n_points;
    old_max = loader->max_points;

    اگر ( new_max > old_max )
    {
      اگر ( new_max > FT_OUTLINE_POINTS_MAX )
      {
        error = FT_THROW( Array_Too_Large );
        جاؤ Exit;
      }

      min_new_max = old_max + ( old_max >> 1 );
      اگر ( new_max < min_new_max )
        new_max = min_new_max;
      new_max = FT_PAD_CEIL( new_max, 8 );
      اگر ( new_max > FT_OUTLINE_POINTS_MAX )
        new_max = FT_OUTLINE_POINTS_MAX;

      اگر ( FT_RENEW_ARRAY( بنیاد->points, old_max, new_max ) ||
           FT_RENEW_ARRAY( بنیاد->لیبل_فہرست,   old_max, new_max ) )
        جاؤ Exit;

      اگر ( loader->use_extra )
      {
        اگر ( FT_RENEW_ARRAY( loader->بنیاد.extra_points,
                             old_max * 2, new_max * 2 ) )
          جاؤ Exit;

        FT_ARRAY_MOVE( loader->بنیاد.extra_points + new_max,
                       loader->بنیاد.extra_points + old_max,
                       old_max );

        loader->بنیاد.extra_points2 = loader->بنیاد.extra_points + new_max;
      }

      adjust = 1;
      loader->max_points = new_max;
    }

    error = FT_GlyphLoader_CreateExtra( loader );
    اگر ( error )
      جاؤ Exit;

    /* check contours */
    old_max = loader->max_contours;
    new_max = (FT_UInt)بنیاد->n_contours + (FT_UInt)current->n_contours +
              n_contours;
    اگر ( new_max > old_max )
    {
      اگر ( new_max > FT_OUTLINE_CONTOURS_MAX )
      {
        error = FT_THROW( Array_Too_Large );
        جاؤ Exit;
      }

      min_new_max = old_max + ( old_max >> 1 );
      اگر ( new_max < min_new_max )
        new_max = min_new_max;
      new_max = FT_PAD_CEIL( new_max, 4 );
      اگر ( new_max > FT_OUTLINE_CONTOURS_MAX )
        new_max = FT_OUTLINE_CONTOURS_MAX;

      اگر ( FT_RENEW_ARRAY( بنیاد->contours, old_max, new_max ) )
        جاؤ Exit;

      adjust = 1;
      loader->max_contours = new_max;
    }

    اگر ( adjust )
      FT_GlyphLoader_Adjust_Points( loader );

  Exit:
    اگر ( error )
      FT_GlyphLoader_Reset( loader );

    واپس error;
  }


  /* Ensure that we can add `n_subglyphs' to our glyph. this function */
  /* reallocates its subglyphs table if necessary.  Note that it DOES */
  /* NOT change the number of subglyphs within the loader!            */
  /*                                                                  */
  FT_بنیاد_DEF( FT_Error )
  FT_GlyphLoader_CheckSubGlyphs( FT_GlyphLoader  loader,
                                 FT_UInt         n_subs )
  {
    FT_حافظہ     حافظہ = loader->حافظہ;
    FT_Error      error  = FT_Err_Ok;
    FT_UInt       new_max, old_max;

    FT_Glyph_بھرو  بنیاد    = &loader->بنیاد;
    FT_Glyph_بھرو  current = &loader->current;


    new_max = بنیاد->عدد_subglyphs + current->عدد_subglyphs + n_subs;
    old_max = loader->max_subglyphs;
    اگر ( new_max > old_max )
    {
      new_max = FT_PAD_CEIL( new_max, 2 );
      اگر ( FT_RENEW_ARRAY( بنیاد->subglyphs, old_max, new_max ) )
        جاؤ Exit;

      loader->max_subglyphs = new_max;

      FT_GlyphLoader_Adjust_Subglyphs( loader );
    }

  Exit:
    واپس error;
  }


  /* prepare loader for the addition of a new glyph on top of the base one */
  FT_بنیاد_DEF( خالی )
  FT_GlyphLoader_Prepare( FT_GlyphLoader  loader )
  {
    FT_Glyph_بھرو  current = &loader->current;


    current->outline.n_points   = 0;
    current->outline.n_contours = 0;
    current->عدد_subglyphs      = 0;

    FT_GlyphLoader_Adjust_Points   ( loader );
    FT_GlyphLoader_Adjust_Subglyphs( loader );
  }


  /* add current glyph to the base image -- and prepare for another */
  FT_بنیاد_DEF( خالی )
  FT_Glyph_Loader_جوڑو( FT_GlyphLoader  loader )
  {
    FT_Outline*  بنیاد;
    FT_Outline*  current;
    FT_Int       n;


    اگر ( !loader )
      واپس;

    بنیاد    = &loader->بنیاد.outline;
    current = &loader->current.outline;

    /* adjust contours count in newest outline */
    برائے ( n = 0; n < current->n_contours; n++ )
      current->contours[n] += بنیاد->n_points;

    بنیاد->n_points   += current->n_points;
    بنیاد->n_contours += current->n_contours;

    loader->بنیاد.عدد_subglyphs += loader->current.عدد_subglyphs;

    /* prepare for another new glyph image */
    FT_GlyphLoader_Prepare( loader );
  }


/*انتہا*/
