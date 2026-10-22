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

#لغو_تعریف  FT_COMPONENT
#تعریف_کن FT_COMPONENT  gloader


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
  FT_پایه_DEF( FT_Error )
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
  FT_پایه_DEF( فراغ )
  FT_GlyphLoader_Rewind( FT_GlyphLoader  loader )
  {
    FT_Glyph_بارگیری  پایه    = &loader->پایه;
    FT_Glyph_بارگیری  current = &loader->current;


    پایه->outline.n_points   = 0;
    پایه->outline.n_contours = 0;
    پایه->outline.flags      = 0;
    پایه->رقم_subglyphs      = 0;

    *current = *پایه;
  }


  /* reset glyph loader, free all allocated tables, */
  /* and start from zero                            */
  FT_پایه_DEF( فراغ )
  FT_GlyphLoader_Reset( FT_GlyphLoader  loader )
  {
    FT_حافظه  حافظه = loader->حافظه;


    FT_FREE( loader->پایه.outline.points );
    FT_FREE( loader->پایه.outline.برچسبها );
    FT_FREE( loader->پایه.outline.contours );
    FT_FREE( loader->پایه.extra_points );
    FT_FREE( loader->پایه.subglyphs );

    loader->پایه.extra_points2 = عدم;

    loader->max_points    = 0;
    loader->max_contours  = 0;
    loader->max_subglyphs = 0;

    FT_GlyphLoader_Rewind( loader );
  }


  /* delete a glyph loader */
  FT_پایه_DEF( فراغ )
  FT_Glyph_Loader_انجامشده( FT_GlyphLoader  loader )
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
    FT_Outline*  پایه    = &loader->پایه.outline;
    FT_Outline*  current = &loader->current.outline;


    current->points   = FT_جابه_جایی( پایه->points,   پایه->n_points );
    current->برچسبها     = FT_جابه_جایی( پایه->برچسبها,     پایه->n_points );
    current->contours = FT_جابه_جایی( پایه->contours, پایه->n_contours );

    /* handle extra points table - if any */
    إذا ( loader->use_extra )
    {
      loader->current.extra_points  = loader->پایه.extra_points +
                                      پایه->n_points;

      loader->current.extra_points2 = loader->پایه.extra_points2 +
                                      پایه->n_points;
    }
  }


  FT_پایه_DEF( FT_Error )
  FT_GlyphLoader_CreateExtra( FT_GlyphLoader  loader )
  {
    FT_Error   error;
    FT_حافظه  حافظه = loader->حافظه;


    إذا ( loader->max_points == 0           ||
         loader->پایه.extra_points != عدم )
      أرجع FT_Err_Ok;

    إذا ( !FT_NEW_ARRAY( loader->پایه.extra_points, 2 * loader->max_points ) )
    {
      loader->use_extra          = 1;
      loader->پایه.extra_points2 = loader->پایه.extra_points +
                                   loader->max_points;

      FT_GlyphLoader_Adjust_Points( loader );
    }
    أرجع error;
  }


  /* re-adjust the `current' subglyphs field */
  ساكن فراغ
  FT_GlyphLoader_Adjust_Subglyphs( FT_GlyphLoader  loader )
  {
    FT_Glyph_بارگیری  پایه    = &loader->پایه;
    FT_Glyph_بارگیری  current = &loader->current;


    current->subglyphs = FT_جابه_جایی( پایه->subglyphs, پایه->رقم_subglyphs );
  }


  /* Ensure that we can add `n_points' and `n_contours' to our glyph.      */
  /* This function reallocates its outline tables if necessary.  Note that */
  /* it DOESN'T change the number of points within the loader!             */
  /*                                                                       */
  FT_پایه_DEF( FT_Error )
  FT_GlyphLoader_CheckPoints( FT_GlyphLoader  loader,
                              FT_UInt         n_points,
                              FT_UInt         n_contours )
  {
    FT_حافظه    حافظه  = loader->حافظه;
    FT_Error     error   = FT_Err_Ok;
    FT_Outline*  پایه    = &loader->پایه.outline;
    FT_Outline*  current = &loader->current.outline;
    FT_Bool      adjust  = 0;

    FT_UInt  new_max, old_max, min_new_max;


    error = FT_GlyphLoader_CreateExtra( loader );
    إذا ( error )
      اذهب Exit;

    /* check points & tags */
    new_max = (FT_UInt)پایه->n_points + (FT_UInt)current->n_points +
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

      إذا ( FT_RENEW_ARRAY( پایه->points, old_max, new_max ) ||
           FT_RENEW_ARRAY( پایه->برچسبها,   old_max, new_max ) )
        اذهب Exit;

      إذا ( loader->use_extra )
      {
        إذا ( FT_RENEW_ARRAY( loader->پایه.extra_points,
                             old_max * 2, new_max * 2 ) )
          اذهب Exit;

        FT_ARRAY_MOVE( loader->پایه.extra_points + new_max,
                       loader->پایه.extra_points + old_max,
                       old_max );

        loader->پایه.extra_points2 = loader->پایه.extra_points + new_max;
      }

      adjust = 1;
      loader->max_points = new_max;
    }

    error = FT_GlyphLoader_CreateExtra( loader );
    إذا ( error )
      اذهب Exit;

    /* check contours */
    old_max = loader->max_contours;
    new_max = (FT_UInt)پایه->n_contours + (FT_UInt)current->n_contours +
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

      إذا ( FT_RENEW_ARRAY( پایه->contours, old_max, new_max ) )
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
  FT_پایه_DEF( FT_Error )
  FT_GlyphLoader_CheckSubGlyphs( FT_GlyphLoader  loader,
                                 FT_UInt         n_subs )
  {
    FT_حافظه     حافظه = loader->حافظه;
    FT_Error      error  = FT_Err_Ok;
    FT_UInt       new_max, old_max;

    FT_Glyph_بارگیری  پایه    = &loader->پایه;
    FT_Glyph_بارگیری  current = &loader->current;


    new_max = پایه->رقم_subglyphs + current->رقم_subglyphs + n_subs;
    old_max = loader->max_subglyphs;
    إذا ( new_max > old_max )
    {
      new_max = FT_PAD_CEIL( new_max, 2 );
      إذا ( FT_RENEW_ARRAY( پایه->subglyphs, old_max, new_max ) )
        اذهب Exit;

      loader->max_subglyphs = new_max;

      FT_GlyphLoader_Adjust_Subglyphs( loader );
    }

  Exit:
    أرجع error;
  }


  /* prepare loader for the addition of a new glyph on top of the base one */
  FT_پایه_DEF( فراغ )
  FT_GlyphLoader_Prepare( FT_GlyphLoader  loader )
  {
    FT_Glyph_بارگیری  current = &loader->current;


    current->outline.n_points   = 0;
    current->outline.n_contours = 0;
    current->رقم_subglyphs      = 0;

    FT_GlyphLoader_Adjust_Points   ( loader );
    FT_GlyphLoader_Adjust_Subglyphs( loader );
  }


  /* add current glyph to the base image -- and prepare for another */
  FT_پایه_DEF( فراغ )
  FT_Glyph_Loader_جمع_کن( FT_GlyphLoader  loader )
  {
    FT_Outline*  پایه;
    FT_Outline*  current;
    FT_Int       n;


    إذا ( !loader )
      أرجع;

    پایه    = &loader->پایه.outline;
    current = &loader->current.outline;

    /* adjust contours count in newest outline */
    لكل ( n = 0; n < current->n_contours; n++ )
      current->contours[n] += پایه->n_points;

    پایه->n_points   += current->n_points;
    پایه->n_contours += current->n_contours;

    loader->پایه.رقم_subglyphs += loader->current.رقم_subglyphs;

    /* prepare for another new glyph image */
    FT_GlyphLoader_Prepare( loader );
  }


/*انتها*/
