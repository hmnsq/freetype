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


#अन्तर्भाव <freetype/internal/ftdebug.h>
#अन्तर्भाव <freetype/internal/ftgloadr.h>
#अन्तर्भाव <freetype/internal/ftmemory.h>
#अन्तर्भाव <freetype/internal/ftobjs.h>

#अपरिभाषय  FT_COMPONENT
#परिभाषय FT_COMPONENT  gloader


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
  FT_आधारः_DEF( FT_Error )
  FT_GlyphLoader_New( FT_स्मृतिः        स्मृतिः,
                      FT_GlyphLoader  *aloader )
  {
    FT_GlyphLoader  loader = रिक्त;
    FT_Error        error;


    यदि ( !FT_NEW( loader ) )
    {
      loader->स्मृतिः = स्मृतिः;
      *aloader       = loader;
    }
    प्रत्यागम error;
  }


  /* rewind the glyph loader - reset counters to 0 */
  FT_आधारः_DEF( शून्य )
  FT_GlyphLoader_Rewind( FT_GlyphLoader  loader )
  {
    FT_Glyph_आरोपय  आधारः    = &loader->आधारः;
    FT_Glyph_आरोपय  current = &loader->current;


    आधारः->outline.n_points   = 0;
    आधारः->outline.n_contours = 0;
    आधारः->outline.flags      = 0;
    आधारः->अङ्कः_subglyphs      = 0;

    *current = *आधारः;
  }


  /* reset glyph loader, free all allocated tables, */
  /* and start from zero                            */
  FT_आधारः_DEF( शून्य )
  FT_GlyphLoader_Reset( FT_GlyphLoader  loader )
  {
    FT_स्मृतिः  स्मृतिः = loader->स्मृतिः;


    FT_FREE( loader->आधारः.outline.points );
    FT_FREE( loader->आधारः.outline.चिह्नकानि );
    FT_FREE( loader->आधारः.outline.contours );
    FT_FREE( loader->आधारः.extra_points );
    FT_FREE( loader->आधारः.subglyphs );

    loader->आधारः.extra_points2 = रिक्त;

    loader->max_points    = 0;
    loader->max_contours  = 0;
    loader->max_subglyphs = 0;

    FT_GlyphLoader_Rewind( loader );
  }


  /* delete a glyph loader */
  FT_आधारः_DEF( शून्य )
  FT_Glyph_Loader_समाप्तम्( FT_GlyphLoader  loader )
  {
    यदि ( loader )
    {
      FT_स्मृतिः  स्मृतिः = loader->स्मृतिः;


      FT_GlyphLoader_Reset( loader );
      FT_FREE( loader );
    }
  }


  /* re-adjust the `current' outline fields */
  स्थिर शून्य
  FT_GlyphLoader_Adjust_Points( FT_GlyphLoader  loader )
  {
    FT_Outline*  आधारः    = &loader->आधारः.outline;
    FT_Outline*  current = &loader->current.outline;


    current->points   = FT_विस्थापनम्( आधारः->points,   आधारः->n_points );
    current->चिह्नकानि     = FT_विस्थापनम्( आधारः->चिह्नकानि,     आधारः->n_points );
    current->contours = FT_विस्थापनम्( आधारः->contours, आधारः->n_contours );

    /* handle extra points table - if any */
    यदि ( loader->use_extra )
    {
      loader->current.extra_points  = loader->आधारः.extra_points +
                                      आधारः->n_points;

      loader->current.extra_points2 = loader->आधारः.extra_points2 +
                                      आधारः->n_points;
    }
  }


  FT_आधारः_DEF( FT_Error )
  FT_GlyphLoader_CreateExtra( FT_GlyphLoader  loader )
  {
    FT_Error   error;
    FT_स्मृतिः  स्मृतिः = loader->स्मृतिः;


    यदि ( loader->max_points == 0           ||
         loader->आधारः.extra_points != रिक्त )
      प्रत्यागम FT_Err_Ok;

    यदि ( !FT_NEW_ARRAY( loader->आधारः.extra_points, 2 * loader->max_points ) )
    {
      loader->use_extra          = 1;
      loader->आधारः.extra_points2 = loader->आधारः.extra_points +
                                   loader->max_points;

      FT_GlyphLoader_Adjust_Points( loader );
    }
    प्रत्यागम error;
  }


  /* re-adjust the `current' subglyphs field */
  स्थिर शून्य
  FT_GlyphLoader_Adjust_Subglyphs( FT_GlyphLoader  loader )
  {
    FT_Glyph_आरोपय  आधारः    = &loader->आधारः;
    FT_Glyph_आरोपय  current = &loader->current;


    current->subglyphs = FT_विस्थापनम्( आधारः->subglyphs, आधारः->अङ्कः_subglyphs );
  }


  /* Ensure that we can add `n_points' and `n_contours' to our glyph.      */
  /* This function reallocates its outline tables if necessary.  Note that */
  /* it DOESN'T change the number of points within the loader!             */
  /*                                                                       */
  FT_आधारः_DEF( FT_Error )
  FT_GlyphLoader_CheckPoints( FT_GlyphLoader  loader,
                              FT_UInt         n_points,
                              FT_UInt         n_contours )
  {
    FT_स्मृतिः    स्मृतिः  = loader->स्मृतिः;
    FT_Error     error   = FT_Err_Ok;
    FT_Outline*  आधारः    = &loader->आधारः.outline;
    FT_Outline*  current = &loader->current.outline;
    FT_Bool      adjust  = 0;

    FT_UInt  new_max, old_max, min_new_max;


    error = FT_GlyphLoader_CreateExtra( loader );
    यदि ( error )
      गच्छ Exit;

    /* check points & tags */
    new_max = (FT_UInt)आधारः->n_points + (FT_UInt)current->n_points +
              n_points;
    old_max = loader->max_points;

    यदि ( new_max > old_max )
    {
      यदि ( new_max > FT_OUTLINE_POINTS_MAX )
      {
        error = FT_THROW( Array_Too_Large );
        गच्छ Exit;
      }

      min_new_max = old_max + ( old_max >> 1 );
      यदि ( new_max < min_new_max )
        new_max = min_new_max;
      new_max = FT_PAD_CEIL( new_max, 8 );
      यदि ( new_max > FT_OUTLINE_POINTS_MAX )
        new_max = FT_OUTLINE_POINTS_MAX;

      यदि ( FT_RENEW_ARRAY( आधारः->points, old_max, new_max ) ||
           FT_RENEW_ARRAY( आधारः->चिह्नकानि,   old_max, new_max ) )
        गच्छ Exit;

      यदि ( loader->use_extra )
      {
        यदि ( FT_RENEW_ARRAY( loader->आधारः.extra_points,
                             old_max * 2, new_max * 2 ) )
          गच्छ Exit;

        FT_ARRAY_MOVE( loader->आधारः.extra_points + new_max,
                       loader->आधारः.extra_points + old_max,
                       old_max );

        loader->आधारः.extra_points2 = loader->आधारः.extra_points + new_max;
      }

      adjust = 1;
      loader->max_points = new_max;
    }

    error = FT_GlyphLoader_CreateExtra( loader );
    यदि ( error )
      गच्छ Exit;

    /* check contours */
    old_max = loader->max_contours;
    new_max = (FT_UInt)आधारः->n_contours + (FT_UInt)current->n_contours +
              n_contours;
    यदि ( new_max > old_max )
    {
      यदि ( new_max > FT_OUTLINE_CONTOURS_MAX )
      {
        error = FT_THROW( Array_Too_Large );
        गच्छ Exit;
      }

      min_new_max = old_max + ( old_max >> 1 );
      यदि ( new_max < min_new_max )
        new_max = min_new_max;
      new_max = FT_PAD_CEIL( new_max, 4 );
      यदि ( new_max > FT_OUTLINE_CONTOURS_MAX )
        new_max = FT_OUTLINE_CONTOURS_MAX;

      यदि ( FT_RENEW_ARRAY( आधारः->contours, old_max, new_max ) )
        गच्छ Exit;

      adjust = 1;
      loader->max_contours = new_max;
    }

    यदि ( adjust )
      FT_GlyphLoader_Adjust_Points( loader );

  Exit:
    यदि ( error )
      FT_GlyphLoader_Reset( loader );

    प्रत्यागम error;
  }


  /* Ensure that we can add `n_subglyphs' to our glyph. this function */
  /* reallocates its subglyphs table if necessary.  Note that it DOES */
  /* NOT change the number of subglyphs within the loader!            */
  /*                                                                  */
  FT_आधारः_DEF( FT_Error )
  FT_GlyphLoader_CheckSubGlyphs( FT_GlyphLoader  loader,
                                 FT_UInt         n_subs )
  {
    FT_स्मृतिः     स्मृतिः = loader->स्मृतिः;
    FT_Error      error  = FT_Err_Ok;
    FT_UInt       new_max, old_max;

    FT_Glyph_आरोपय  आधारः    = &loader->आधारः;
    FT_Glyph_आरोपय  current = &loader->current;


    new_max = आधारः->अङ्कः_subglyphs + current->अङ्कः_subglyphs + n_subs;
    old_max = loader->max_subglyphs;
    यदि ( new_max > old_max )
    {
      new_max = FT_PAD_CEIL( new_max, 2 );
      यदि ( FT_RENEW_ARRAY( आधारः->subglyphs, old_max, new_max ) )
        गच्छ Exit;

      loader->max_subglyphs = new_max;

      FT_GlyphLoader_Adjust_Subglyphs( loader );
    }

  Exit:
    प्रत्यागम error;
  }


  /* prepare loader for the addition of a new glyph on top of the base one */
  FT_आधारः_DEF( शून्य )
  FT_GlyphLoader_Prepare( FT_GlyphLoader  loader )
  {
    FT_Glyph_आरोपय  current = &loader->current;


    current->outline.n_points   = 0;
    current->outline.n_contours = 0;
    current->अङ्कः_subglyphs      = 0;

    FT_GlyphLoader_Adjust_Points   ( loader );
    FT_GlyphLoader_Adjust_Subglyphs( loader );
  }


  /* add current glyph to the base image -- and prepare for another */
  FT_आधारः_DEF( शून्य )
  FT_Glyph_Loader_सङ्कलय( FT_GlyphLoader  loader )
  {
    FT_Outline*  आधारः;
    FT_Outline*  current;
    FT_Int       n;


    यदि ( !loader )
      प्रत्यागम;

    आधारः    = &loader->आधारः.outline;
    current = &loader->current.outline;

    /* adjust contours count in newest outline */
    प्रति ( n = 0; n < current->n_contours; n++ )
      current->contours[n] += आधारः->n_points;

    आधारः->n_points   += current->n_points;
    आधारः->n_contours += current->n_contours;

    loader->आधारः.अङ्कः_subglyphs += loader->current.अङ्कः_subglyphs;

    /* prepare for another new glyph image */
    FT_GlyphLoader_Prepare( loader );
  }


/*अन्तः*/
