/****************************************************************************
 *
 * ftutil.c
 *
 *   FreeType utility file for memory and list management (body).
 *
 * Copyright (C) 2002-2026 by
 * David Turner, Robert Wilhelm, and Werner Lemberg.
 *
 * This file is part of the FreeType project, and may only be used,
 * modified, and distributed under the terms of the FreeType project
 * license, LICENSE.TXT.  By continuing to use, modify, or distribute
 * this file you indicate that you have read the license and
 * understand and accept it fully.
 *
 */


#ಅನ್ತರ್ಭಾವ <freetype/internal/ftdebug.h>
#ಅನ್ತರ್ಭಾವ <freetype/internal/ftmemory.h>
#ಅನ್ತರ್ಭಾವ <freetype/internal/ftobjs.h>
#ಅನ್ತರ್ಭಾವ <freetype/ftlist.h>


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು  FT_COMPONENT
#ವ್ಯಾಖ್ಯಾನಿಸು FT_COMPONENT  ಸ್ಮರಣೆ


  /*************************************************************************/
  /*************************************************************************/
  /*************************************************************************/
  /*****                                                               *****/
  /*****                                                               *****/
  /*****               M E M O R Y   M A N A G E M E N T               *****/
  /*****                                                               *****/
  /*****                                                               *****/
  /*************************************************************************/
  /*************************************************************************/
  /*************************************************************************/


  FT_ತಳ_DEF( FT_ಸೂಚಕ )
  ft_mem_alloc( FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ,
                FT_Long    ಗಾತ್ರ_2,
                FT_Error  *p_error )
  {
    FT_Error    error;
    FT_ಸೂಚಕ  ಖಂಡ = ft_mem_qalloc( ಸ್ಮರಣೆ, ಗಾತ್ರ_2, &error );

    ಆದರೆ ( !error && ಖಂಡ && ಗಾತ್ರ_2 > 0 )
      FT_MEM_ZERO( ಖಂಡ, ಗಾತ್ರ_2 );

    *p_error = error;
    ಹಿಂತಿರುಗಿ ಖಂಡ;
  }


  FT_ತಳ_DEF( FT_ಸೂಚಕ )
  ft_mem_qalloc( FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ,
                 FT_Long    ಗಾತ್ರ_2,
                 FT_Error  *p_error )
  {
    FT_Error    error = FT_Err_Ok;
    FT_ಸೂಚಕ  ಖಂಡ = ಖಾಲಿ;


    ಆದರೆ ( ಗಾತ್ರ_2 > 0 )
    {
      ಖಂಡ = ಸ್ಮರಣೆ->alloc( ಸ್ಮರಣೆ, ಗಾತ್ರ_2 );
      ಆದರೆ ( !ಖಂಡ )
        error = FT_THROW( ಹೊರಹೋಗು_ದ_ಸ್ಮರಣೆ );
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ( ಗಾತ್ರ_2 < 0 )
    {
      /* may help catch/prevent security issues */
      error = FT_THROW( Invalid_ವಾದ );
    }

    *p_error = error;
    ಹಿಂತಿರುಗಿ ಖಂಡ;
  }


  FT_ತಳ_DEF( FT_ಸೂಚಕ )
  ft_mem_realloc( FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ,
                  FT_Long    ಅಂಶ_ಗಾತ್ರ,
                  FT_Long    cur_ಎಣಿಕೆ,
                  FT_Long    new_ಎಣಿಕೆ,
                  ಶೂನ್ಯ*      ಖಂಡ,
                  FT_Error  *p_error )
  {
    FT_Error  error = FT_Err_Ok;


    ಖಂಡ = ft_mem_qrealloc( ಸ್ಮರಣೆ, ಅಂಶ_ಗಾತ್ರ,
                             cur_ಎಣಿಕೆ, new_ಎಣಿಕೆ, ಖಂಡ, &error );
    ಆದರೆ ( !error && ಖಂಡ && new_ಎಣಿಕೆ > cur_ಎಣಿಕೆ )
      FT_MEM_ZERO( (ಅಕ್ಷರ*)ಖಂಡ + cur_ಎಣಿಕೆ * ಅಂಶ_ಗಾತ್ರ,
                   ( new_ಎಣಿಕೆ - cur_ಎಣಿಕೆ ) * ಅಂಶ_ಗಾತ್ರ );

    *p_error = error;
    ಹಿಂತಿರುಗಿ ಖಂಡ;
  }


  FT_ತಳ_DEF( FT_ಸೂಚಕ )
  ft_mem_qrealloc( FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ,
                   FT_Long    ಅಂಶ_ಗಾತ್ರ,
                   FT_Long    cur_ಎಣಿಕೆ,
                   FT_Long    new_ಎಣಿಕೆ,
                   ಶೂನ್ಯ*      ಖಂಡ,
                   FT_Error  *p_error )
  {
    FT_Error  error = FT_Err_Ok;


    /* Note that we now accept `item_size == 0' as a valid parameter, in
     * order to cover very weird cases where an ALLOC_MULT macro would be
     * called.
     */
    ಆದರೆ ( cur_ಎಣಿಕೆ < 0 || new_ಎಣಿಕೆ < 0 || ಅಂಶ_ಗಾತ್ರ < 0 )
    {
      /* may help catch/prevent nasty security issues */
      error = FT_THROW( Invalid_ವಾದ );
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ( new_ಎಣಿಕೆ == 0 || ಅಂಶ_ಗಾತ್ರ == 0 )
    {
      ft_mem_free( ಸ್ಮರಣೆ, ಖಂಡ );
      ಖಂಡ = ಖಾಲಿ;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ( new_ಎಣಿಕೆ > FT_INT_MAX / ಅಂಶ_ಗಾತ್ರ )
    {
      error = FT_THROW( Array_Too_Large );
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ( cur_ಎಣಿಕೆ == 0 )
    {
      FT_ASSERT( !ಖಂಡ );

      ಖಂಡ = ಸ್ಮರಣೆ->alloc( ಸ್ಮರಣೆ, new_ಎಣಿಕೆ * ಅಂಶ_ಗಾತ್ರ );
      ಆದರೆ ( ಖಂಡ == ಖಾಲಿ )
        error = FT_THROW( ಹೊರಹೋಗು_ದ_ಸ್ಮರಣೆ );
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
      FT_ಸೂಚಕ  block2;
      FT_Long     cur_ಗಾತ್ರ = cur_ಎಣಿಕೆ * ಅಂಶ_ಗಾತ್ರ;
      FT_Long     new_ಗಾತ್ರ = new_ಎಣಿಕೆ * ಅಂಶ_ಗಾತ್ರ;


      block2 = ಸ್ಮರಣೆ->realloc( ಸ್ಮರಣೆ, cur_ಗಾತ್ರ, new_ಗಾತ್ರ, ಖಂಡ );
      ಆದರೆ ( !block2 )
        error = FT_THROW( ಹೊರಹೋಗು_ದ_ಸ್ಮರಣೆ );
      ಇಲ್ಲದಿದ್ದರೆ
        ಖಂಡ = block2;
    }

    *p_error = error;
    ಹಿಂತಿರುಗಿ ಖಂಡ;
  }


  FT_ತಳ_DEF( ಶೂನ್ಯ )
  ft_mem_free( FT_ಸ್ಮರಣೆ   ಸ್ಮರಣೆ,
               ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *P )
  {
    ಆದರೆ ( P )
      ಸ್ಮರಣೆ->ಬಿಡುಗಡೆ( ಸ್ಮರಣೆ, (ಶೂನ್ಯ*)P );
  }


  FT_ತಳ_DEF( FT_ಸೂಚಕ )
  ft_mem_dup( FT_ಸ್ಮರಣೆ    ಸ್ಮರಣೆ,
              ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ*  ವಿಳಾಸ,
              FT_ULong     ಗಾತ್ರ_2,
              FT_Error    *p_error )
  {
    FT_Error    error;
    FT_ಸೂಚಕ  p = ft_mem_qalloc( ಸ್ಮರಣೆ, (FT_Long)ಗಾತ್ರ_2, &error );


    ಆದರೆ ( !error && ವಿಳಾಸ && ಗಾತ್ರ_2 > 0 )
      ft_memcpy( p, ವಿಳಾಸ, ಗಾತ್ರ_2 );

    *p_error = error;
    ಹಿಂತಿರುಗಿ p;
  }


  FT_ತಳ_DEF( FT_ಸೂಚಕ )
  ft_mem_strdup( FT_ಸ್ಮರಣೆ    ಸ್ಮರಣೆ,
                 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*  str,
                 FT_Error    *p_error )
  {
    FT_ULong  len = str ? (FT_ULong)ft_strlen( str ) + 1
                        : 0;


    ಹಿಂತಿರುಗಿ ft_mem_dup( ಸ್ಮರಣೆ, str, len, p_error );
  }


  FT_ತಳ_DEF( FT_Int )
  ft_mem_strcpyn( ಅಕ್ಷರ*        dst,
                  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*  src,
                  FT_ULong     ಗಾತ್ರ_2 )
  {
    ಆಗ ( ಗಾತ್ರ_2 > 1 && *src != 0 )
    {
      *dst++ = *src++;
      ಗಾತ್ರ_2--;
    }

    *dst = 0;  /* always zero-terminate */

    ಹಿಂತಿರುಗಿ *src != 0;
  }


  /*************************************************************************/
  /*************************************************************************/
  /*************************************************************************/
  /*****                                                               *****/
  /*****                                                               *****/
  /*****            D O U B L Y   L I N K E D   L I S T S              *****/
  /*****                                                               *****/
  /*****                                                               *****/
  /*************************************************************************/
  /*************************************************************************/
  /*************************************************************************/

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು  FT_COMPONENT
#ವ್ಯಾಖ್ಯಾನಿಸು FT_COMPONENT  list

  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( FT_List_ಗಂಟು )
  FT_List_ಕಂಡುಹಿಡಿ( FT_List  list,
                ಶೂನ್ಯ*    ದತ್ತಾಂಶ )
  {
    FT_List_ಗಂಟು  cur;


    ಆದರೆ ( !list )
      ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    cur = list->ತಲೆ;
    ಆಗ ( cur )
    {
      ಆದರೆ ( cur->ದತ್ತಾಂಶ == ದತ್ತಾಂಶ )
        ಹಿಂತಿರುಗಿ cur;

      cur = cur->ಮುಂದಿನ;
    }

    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( ಶೂನ್ಯ )
  FT_List_ಕೂಡಿಸು( FT_List      list,
               FT_List_ಗಂಟು  ಗಂಟು )
  {
    FT_List_ಗಂಟು  before;


    ಆದರೆ ( !list || !ಗಂಟು )
      ಹಿಂತಿರುಗಿ;

    before = list->ಬಾಲ;

    ಗಂಟು->ಮುಂದಿನ = ಖಾಲಿ;
    ಗಂಟು->prev = before;

    ಆದರೆ ( before )
      before->ಮುಂದಿನ = ಗಂಟು;
    ಇಲ್ಲದಿದ್ದರೆ
      list->ತಲೆ = ಗಂಟು;

    list->ಬಾಲ = ಗಂಟು;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( ಶೂನ್ಯ )
  FT_List_ಸೇರಿಸು( FT_List      list,
                  FT_List_ಗಂಟು  ಗಂಟು )
  {
    FT_List_ಗಂಟು  after;


    ಆದರೆ ( !list || !ಗಂಟು )
      ಹಿಂತಿರುಗಿ;

    after = list->ತಲೆ;

    ಗಂಟು->ಮುಂದಿನ = after;
    ಗಂಟು->prev = ಖಾಲಿ;

    ಆದರೆ ( !after )
      list->ಬಾಲ = ಗಂಟು;
    ಇಲ್ಲದಿದ್ದರೆ
      after->prev = ಗಂಟು;

    list->ತಲೆ = ಗಂಟು;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( ಶೂನ್ಯ )
  FT_List_Remove( FT_List      list,
                  FT_List_ಗಂಟು  ಗಂಟು )
  {
    FT_List_ಗಂಟು  before, after;


    ಆದರೆ ( !list || !ಗಂಟು )
      ಹಿಂತಿರುಗಿ;

    before = ಗಂಟು->prev;
    after  = ಗಂಟು->ಮುಂದಿನ;

    ಆದರೆ ( before )
      before->ಮುಂದಿನ = after;
    ಇಲ್ಲದಿದ್ದರೆ
      list->ತಲೆ = after;

    ಆದರೆ ( after )
      after->prev = before;
    ಇಲ್ಲದಿದ್ದರೆ
      list->ಬಾಲ = before;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( ಶೂನ್ಯ )
  FT_List_ಮೇಲೆ( FT_List      list,
              FT_List_ಗಂಟು  ಗಂಟು )
  {
    FT_List_ಗಂಟು  before, after;


    ಆದರೆ ( !list || !ಗಂಟು )
      ಹಿಂತಿರುಗಿ;

    before = ಗಂಟು->prev;
    after  = ಗಂಟು->ಮುಂದಿನ;

    /* check whether we are already on top of the list */
    ಆದರೆ ( !before )
      ಹಿಂತಿರುಗಿ;

    before->ಮುಂದಿನ = after;

    ಆದರೆ ( after )
      after->prev = before;
    ಇಲ್ಲದಿದ್ದರೆ
      list->ಬಾಲ = before;

    ಗಂಟು->prev       = ಖಾಲಿ;
    ಗಂಟು->ಮುಂದಿನ       = list->ತಲೆ;
    list->ತಲೆ->prev = ಗಂಟು;
    list->ತಲೆ       = ಗಂಟು;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( FT_Error )
  FT_List_Iterate( FT_List           list,
                   FT_List_Iterator  iterator,
                   ಶೂನ್ಯ*             user )
  {
    FT_List_ಗಂಟು  cur;
    FT_Error     error = FT_Err_Ok;


    ಆದರೆ ( !list || !iterator )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

    cur = list->ತಲೆ;

    ಆಗ ( cur )
    {
      FT_List_ಗಂಟು  ಮುಂದಿನ = cur->ಮುಂದಿನ;


      error = iterator( cur, user );
      ಆದರೆ ( error )
        ನಿಲ್ಲಿಸು;

      cur = ಮುಂದಿನ;
    }

    ಹಿಂತಿರುಗಿ error;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( ಶೂನ್ಯ )
  FT_List_Finalize( FT_List             list,
                    FT_List_Destructor  destroy,
                    FT_ಸ್ಮರಣೆ           ಸ್ಮರಣೆ,
                    ಶೂನ್ಯ*               user )
  {
    FT_List_ಗಂಟು  cur;


    ಆದರೆ ( !list || !ಸ್ಮರಣೆ )
      ಹಿಂತಿರುಗಿ;

    cur = list->ತಲೆ;
    ಆಗ ( cur )
    {
      FT_List_ಗಂಟು  ಮುಂದಿನ = cur->ಮುಂದಿನ;


      ಆದರೆ ( destroy )
        destroy( ಸ್ಮರಣೆ, cur->ದತ್ತಾಂಶ, user );

      FT_FREE( cur );
      cur = ಮುಂದಿನ;
    }

    list->ತಲೆ = ಖಾಲಿ;
    list->ಬಾಲ = ಖಾಲಿ;
  }


/*ಕೊನೆ*/
