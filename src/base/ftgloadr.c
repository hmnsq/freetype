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


#অন্তর্ভাব <freetype/internal/ftdebug.h>
#অন্তর্ভাব <freetype/internal/ftgloadr.h>
#অন্তর্ভাব <freetype/internal/ftmemory.h>
#অন্তর্ভাব <freetype/internal/ftobjs.h>

#undef  FT_COMPONENT
#define FT_COMPONENT  gloader


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
  FT_BASE_DEF( FT_Error )
  FT_GlyphLoader_New( FT_Memory        memory,
                      FT_GlyphLoader  *aloader )
  {
    FT_GlyphLoader  loader = রিক্ত;
    FT_Error        error;


    যদি ( !FT_NEW( loader ) )
    {
      loader->memory = memory;
      *aloader       = loader;
    }
    প্রত্যাগম error;
  }


  /* rewind the glyph loader - reset counters to 0 */
  FT_BASE_DEF( শূন্য )
  FT_GlyphLoader_Rewind( FT_GlyphLoader  loader )
  {
    FT_GlyphLoad  base    = &loader->base;
    FT_GlyphLoad  current = &loader->current;


    base->outline.n_points   = 0;
    base->outline.n_contours = 0;
    base->outline.flags      = 0;
    base->অঙ্ক_subglyphs      = 0;

    *current = *base;
  }


  /* reset glyph loader, free all allocated tables, */
  /* and start from zero                            */
  FT_BASE_DEF( শূন্য )
  FT_GlyphLoader_Reset( FT_GlyphLoader  loader )
  {
    FT_Memory  memory = loader->memory;


    FT_FREE( loader->base.outline.points );
    FT_FREE( loader->base.outline.tags );
    FT_FREE( loader->base.outline.contours );
    FT_FREE( loader->base.extra_points );
    FT_FREE( loader->base.subglyphs );

    loader->base.extra_points2 = রিক্ত;

    loader->max_points    = 0;
    loader->max_contours  = 0;
    loader->max_subglyphs = 0;

    FT_GlyphLoader_Rewind( loader );
  }


  /* delete a glyph loader */
  FT_BASE_DEF( শূন্য )
  FT_GlyphLoader_Done( FT_GlyphLoader  loader )
  {
    যদি ( loader )
    {
      FT_Memory  memory = loader->memory;


      FT_GlyphLoader_Reset( loader );
      FT_FREE( loader );
    }
  }


  /* re-adjust the `current' outline fields */
  স্থির শূন্য
  FT_GlyphLoader_Adjust_Points( FT_GlyphLoader  loader )
  {
    FT_Outline*  base    = &loader->base.outline;
    FT_Outline*  current = &loader->current.outline;


    current->points   = FT_OFFSET( base->points,   base->n_points );
    current->tags     = FT_OFFSET( base->tags,     base->n_points );
    current->contours = FT_OFFSET( base->contours, base->n_contours );

    /* handle extra points table - if any */
    যদি ( loader->use_extra )
    {
      loader->current.extra_points  = loader->base.extra_points +
                                      base->n_points;

      loader->current.extra_points2 = loader->base.extra_points2 +
                                      base->n_points;
    }
  }


  FT_BASE_DEF( FT_Error )
  FT_GlyphLoader_CreateExtra( FT_GlyphLoader  loader )
  {
    FT_Error   error;
    FT_Memory  memory = loader->memory;


    যদি ( loader->max_points == 0           ||
         loader->base.extra_points != রিক্ত )
      প্রত্যাগম FT_Err_Ok;

    যদি ( !FT_NEW_ARRAY( loader->base.extra_points, 2 * loader->max_points ) )
    {
      loader->use_extra          = 1;
      loader->base.extra_points2 = loader->base.extra_points +
                                   loader->max_points;

      FT_GlyphLoader_Adjust_Points( loader );
    }
    প্রত্যাগম error;
  }


  /* re-adjust the `current' subglyphs field */
  স্থির শূন্য
  FT_GlyphLoader_Adjust_Subglyphs( FT_GlyphLoader  loader )
  {
    FT_GlyphLoad  base    = &loader->base;
    FT_GlyphLoad  current = &loader->current;


    current->subglyphs = FT_OFFSET( base->subglyphs, base->অঙ্ক_subglyphs );
  }


  /* Ensure that we can add `n_points' and `n_contours' to our glyph.      */
  /* This function reallocates its outline tables if necessary.  Note that */
  /* it DOESN'T change the number of points within the loader!             */
  /*                                                                       */
  FT_BASE_DEF( FT_Error )
  FT_GlyphLoader_CheckPoints( FT_GlyphLoader  loader,
                              FT_UInt         n_points,
                              FT_UInt         n_contours )
  {
    FT_Memory    memory  = loader->memory;
    FT_Error     error   = FT_Err_Ok;
    FT_Outline*  base    = &loader->base.outline;
    FT_Outline*  current = &loader->current.outline;
    FT_Bool      adjust  = 0;

    FT_UInt  new_max, old_max, min_new_max;


    error = FT_GlyphLoader_CreateExtra( loader );
    যদি ( error )
      গচ্ছ Exit;

    /* check points & tags */
    new_max = (FT_UInt)base->n_points + (FT_UInt)current->n_points +
              n_points;
    old_max = loader->max_points;

    যদি ( new_max > old_max )
    {
      যদি ( new_max > FT_OUTLINE_POINTS_MAX )
      {
        error = FT_THROW( Array_Too_Large );
        গচ্ছ Exit;
      }

      min_new_max = old_max + ( old_max >> 1 );
      যদি ( new_max < min_new_max )
        new_max = min_new_max;
      new_max = FT_PAD_CEIL( new_max, 8 );
      যদি ( new_max > FT_OUTLINE_POINTS_MAX )
        new_max = FT_OUTLINE_POINTS_MAX;

      যদি ( FT_RENEW_ARRAY( base->points, old_max, new_max ) ||
           FT_RENEW_ARRAY( base->tags,   old_max, new_max ) )
        গচ্ছ Exit;

      যদি ( loader->use_extra )
      {
        যদি ( FT_RENEW_ARRAY( loader->base.extra_points,
                             old_max * 2, new_max * 2 ) )
          গচ্ছ Exit;

        FT_ARRAY_MOVE( loader->base.extra_points + new_max,
                       loader->base.extra_points + old_max,
                       old_max );

        loader->base.extra_points2 = loader->base.extra_points + new_max;
      }

      adjust = 1;
      loader->max_points = new_max;
    }

    error = FT_GlyphLoader_CreateExtra( loader );
    যদি ( error )
      গচ্ছ Exit;

    /* check contours */
    old_max = loader->max_contours;
    new_max = (FT_UInt)base->n_contours + (FT_UInt)current->n_contours +
              n_contours;
    যদি ( new_max > old_max )
    {
      যদি ( new_max > FT_OUTLINE_CONTOURS_MAX )
      {
        error = FT_THROW( Array_Too_Large );
        গচ্ছ Exit;
      }

      min_new_max = old_max + ( old_max >> 1 );
      যদি ( new_max < min_new_max )
        new_max = min_new_max;
      new_max = FT_PAD_CEIL( new_max, 4 );
      যদি ( new_max > FT_OUTLINE_CONTOURS_MAX )
        new_max = FT_OUTLINE_CONTOURS_MAX;

      যদি ( FT_RENEW_ARRAY( base->contours, old_max, new_max ) )
        গচ্ছ Exit;

      adjust = 1;
      loader->max_contours = new_max;
    }

    যদি ( adjust )
      FT_GlyphLoader_Adjust_Points( loader );

  Exit:
    যদি ( error )
      FT_GlyphLoader_Reset( loader );

    প্রত্যাগম error;
  }


  /* Ensure that we can add `n_subglyphs' to our glyph. this function */
  /* reallocates its subglyphs table if necessary.  Note that it DOES */
  /* NOT change the number of subglyphs within the loader!            */
  /*                                                                  */
  FT_BASE_DEF( FT_Error )
  FT_GlyphLoader_CheckSubGlyphs( FT_GlyphLoader  loader,
                                 FT_UInt         n_subs )
  {
    FT_Memory     memory = loader->memory;
    FT_Error      error  = FT_Err_Ok;
    FT_UInt       new_max, old_max;

    FT_GlyphLoad  base    = &loader->base;
    FT_GlyphLoad  current = &loader->current;


    new_max = base->অঙ্ক_subglyphs + current->অঙ্ক_subglyphs + n_subs;
    old_max = loader->max_subglyphs;
    যদি ( new_max > old_max )
    {
      new_max = FT_PAD_CEIL( new_max, 2 );
      যদি ( FT_RENEW_ARRAY( base->subglyphs, old_max, new_max ) )
        গচ্ছ Exit;

      loader->max_subglyphs = new_max;

      FT_GlyphLoader_Adjust_Subglyphs( loader );
    }

  Exit:
    প্রত্যাগম error;
  }


  /* prepare loader for the addition of a new glyph on top of the base one */
  FT_BASE_DEF( শূন্য )
  FT_GlyphLoader_Prepare( FT_GlyphLoader  loader )
  {
    FT_GlyphLoad  current = &loader->current;


    current->outline.n_points   = 0;
    current->outline.n_contours = 0;
    current->অঙ্ক_subglyphs      = 0;

    FT_GlyphLoader_Adjust_Points   ( loader );
    FT_GlyphLoader_Adjust_Subglyphs( loader );
  }


  /* add current glyph to the base image -- and prepare for another */
  FT_BASE_DEF( শূন্য )
  FT_GlyphLoader_Add( FT_GlyphLoader  loader )
  {
    FT_Outline*  base;
    FT_Outline*  current;
    FT_Int       n;


    যদি ( !loader )
      প্রত্যাগম;

    base    = &loader->base.outline;
    current = &loader->current.outline;

    /* adjust contours count in newest outline */
    প্রতি ( n = 0; n < current->n_contours; n++ )
      current->contours[n] += base->n_points;

    base->n_points   += current->n_points;
    base->n_contours += current->n_contours;

    loader->base.অঙ্ক_subglyphs += loader->current.অঙ্ক_subglyphs;

    /* prepare for another new glyph image */
    FT_GlyphLoader_Prepare( loader );
  }


/* END */
