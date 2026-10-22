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
#undef  FT_COMPONENT
#define FT_COMPONENT  memory


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


  FT_BASE_DEF( FT_Pointer )
  ft_mem_alloc( FT_Memory  memory,
                FT_Long    size,
                FT_Error  *p_error )
  {
    FT_Error    error;
    FT_Pointer  block = ft_mem_qalloc( memory, size, &error );

    ಯದಿ ( !error && block && size > 0 )
      FT_MEM_ZERO( block, size );

    *p_error = error;
    ಪ್ರತ್ಯಾಗಮ block;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_qalloc( FT_Memory  memory,
                 FT_Long    size,
                 FT_Error  *p_error )
  {
    FT_Error    error = FT_Err_Ok;
    FT_Pointer  block = ರಿಕ್ತ;


    ಯದಿ ( size > 0 )
    {
      block = memory->alloc( memory, size );
      ಯದಿ ( !block )
        error = FT_THROW( Out_Of_Memory );
    }
    ಅನ್ಯಥಾ ಯದಿ ( size < 0 )
    {
      /* may help catch/prevent security issues */
      error = FT_THROW( Invalid_Argument );
    }

    *p_error = error;
    ಪ್ರತ್ಯಾಗಮ block;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_realloc( FT_Memory  memory,
                  FT_Long    item_size,
                  FT_Long    cur_ಎಣಿಕೆ,
                  FT_Long    new_ಎಣಿಕೆ,
                  ಶೂನ್ಯ*      block,
                  FT_Error  *p_error )
  {
    FT_Error  error = FT_Err_Ok;


    block = ft_mem_qrealloc( memory, item_size,
                             cur_ಎಣಿಕೆ, new_ಎಣಿಕೆ, block, &error );
    ಯದಿ ( !error && block && new_ಎಣಿಕೆ > cur_ಎಣಿಕೆ )
      FT_MEM_ZERO( (ವರ್ಣ*)block + cur_ಎಣಿಕೆ * item_size,
                   ( new_ಎಣಿಕೆ - cur_ಎಣಿಕೆ ) * item_size );

    *p_error = error;
    ಪ್ರತ್ಯಾಗಮ block;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_qrealloc( FT_Memory  memory,
                   FT_Long    item_size,
                   FT_Long    cur_ಎಣಿಕೆ,
                   FT_Long    new_ಎಣಿಕೆ,
                   ಶೂನ್ಯ*      block,
                   FT_Error  *p_error )
  {
    FT_Error  error = FT_Err_Ok;


    /* Note that we now accept `item_size == 0' as a valid parameter, in
     * order to cover very weird cases where an ALLOC_MULT macro would be
     * called.
     */
    ಯದಿ ( cur_ಎಣಿಕೆ < 0 || new_ಎಣಿಕೆ < 0 || item_size < 0 )
    {
      /* may help catch/prevent nasty security issues */
      error = FT_THROW( Invalid_Argument );
    }
    ಅನ್ಯಥಾ ಯದಿ ( new_ಎಣಿಕೆ == 0 || item_size == 0 )
    {
      ft_mem_free( memory, block );
      block = ರಿಕ್ತ;
    }
    ಅನ್ಯಥಾ ಯದಿ ( new_ಎಣಿಕೆ > FT_INT_MAX / item_size )
    {
      error = FT_THROW( Array_Too_Large );
    }
    ಅನ್ಯಥಾ ಯದಿ ( cur_ಎಣಿಕೆ == 0 )
    {
      FT_ASSERT( !block );

      block = memory->alloc( memory, new_ಎಣಿಕೆ * item_size );
      ಯದಿ ( block == ರಿಕ್ತ )
        error = FT_THROW( Out_Of_Memory );
    }
    ಅನ್ಯಥಾ
    {
      FT_Pointer  block2;
      FT_Long     cur_size = cur_ಎಣಿಕೆ * item_size;
      FT_Long     new_size = new_ಎಣಿಕೆ * item_size;


      block2 = memory->realloc( memory, cur_size, new_size, block );
      ಯದಿ ( !block2 )
        error = FT_THROW( Out_Of_Memory );
      ಅನ್ಯಥಾ
        block = block2;
    }

    *p_error = error;
    ಪ್ರತ್ಯಾಗಮ block;
  }


  FT_BASE_DEF( ಶೂನ್ಯ )
  ft_mem_free( FT_Memory   memory,
               ಧ್ರುವ ಶೂನ್ಯ *P )
  {
    ಯದಿ ( P )
      memory->ಮುಕ್ತ( memory, (ಶೂನ್ಯ*)P );
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_dup( FT_Memory    memory,
              ಧ್ರುವ ಶೂನ್ಯ*  address,
              FT_ULong     size,
              FT_Error    *p_error )
  {
    FT_Error    error;
    FT_Pointer  p = ft_mem_qalloc( memory, (FT_Long)size, &error );


    ಯದಿ ( !error && address && size > 0 )
      ft_memcpy( p, address, size );

    *p_error = error;
    ಪ್ರತ್ಯಾಗಮ p;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_strdup( FT_Memory    memory,
                 ಧ್ರುವ ವರ್ಣ*  str,
                 FT_Error    *p_error )
  {
    FT_ULong  len = str ? (FT_ULong)ft_strlen( str ) + 1
                        : 0;


    ಪ್ರತ್ಯಾಗಮ ft_mem_dup( memory, str, len, p_error );
  }


  FT_BASE_DEF( FT_Int )
  ft_mem_strcpyn( ವರ್ಣ*        dst,
                  ಧ್ರುವ ವರ್ಣ*  src,
                  FT_ULong     size )
  {
    ಯಾವತ್ ( size > 1 && *src != 0 )
    {
      *dst++ = *src++;
      size--;
    }

    *dst = 0;  /* always zero-terminate */

    ಪ್ರತ್ಯಾಗಮ *src != 0;
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

#undef  FT_COMPONENT
#define FT_COMPONENT  list

  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( FT_List_ಗಂಟು )
  FT_List_Find( FT_List  list,
                ಶೂನ್ಯ*    data )
  {
    FT_List_ಗಂಟು  cur;


    ಯದಿ ( !list )
      ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

    cur = list->head;
    ಯಾವತ್ ( cur )
    {
      ಯದಿ ( cur->data == data )
        ಪ್ರತ್ಯಾಗಮ cur;

      cur = cur->next;
    }

    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( ಶೂನ್ಯ )
  FT_List_Add( FT_List      list,
               FT_List_ಗಂಟು  ಗಂಟು )
  {
    FT_List_ಗಂಟು  before;


    ಯದಿ ( !list || !ಗಂಟು )
      ಪ್ರತ್ಯಾಗಮ;

    before = list->tail;

    ಗಂಟು->next = ರಿಕ್ತ;
    ಗಂಟು->prev = before;

    ಯದಿ ( before )
      before->next = ಗಂಟು;
    ಅನ್ಯಥಾ
      list->head = ಗಂಟು;

    list->tail = ಗಂಟು;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( ಶೂನ್ಯ )
  FT_List_ಸೇರಿಸು( FT_List      list,
                  FT_List_ಗಂಟು  ಗಂಟು )
  {
    FT_List_ಗಂಟು  after;


    ಯದಿ ( !list || !ಗಂಟು )
      ಪ್ರತ್ಯಾಗಮ;

    after = list->head;

    ಗಂಟು->next = after;
    ಗಂಟು->prev = ರಿಕ್ತ;

    ಯದಿ ( !after )
      list->tail = ಗಂಟು;
    ಅನ್ಯಥಾ
      after->prev = ಗಂಟು;

    list->head = ಗಂಟು;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( ಶೂನ್ಯ )
  FT_List_Remove( FT_List      list,
                  FT_List_ಗಂಟು  ಗಂಟು )
  {
    FT_List_ಗಂಟು  before, after;


    ಯದಿ ( !list || !ಗಂಟು )
      ಪ್ರತ್ಯಾಗಮ;

    before = ಗಂಟು->prev;
    after  = ಗಂಟು->next;

    ಯದಿ ( before )
      before->next = after;
    ಅನ್ಯಥಾ
      list->head = after;

    ಯದಿ ( after )
      after->prev = before;
    ಅನ್ಯಥಾ
      list->tail = before;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( ಶೂನ್ಯ )
  FT_List_Up( FT_List      list,
              FT_List_ಗಂಟು  ಗಂಟು )
  {
    FT_List_ಗಂಟು  before, after;


    ಯದಿ ( !list || !ಗಂಟು )
      ಪ್ರತ್ಯಾಗಮ;

    before = ಗಂಟು->prev;
    after  = ಗಂಟು->next;

    /* check whether we are already on top of the list */
    ಯದಿ ( !before )
      ಪ್ರತ್ಯಾಗಮ;

    before->next = after;

    ಯದಿ ( after )
      after->prev = before;
    ಅನ್ಯಥಾ
      list->tail = before;

    ಗಂಟು->prev       = ರಿಕ್ತ;
    ಗಂಟು->next       = list->head;
    list->head->prev = ಗಂಟು;
    list->head       = ಗಂಟು;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( FT_Error )
  FT_List_Iterate( FT_List           list,
                   FT_List_Iterator  iterator,
                   ಶೂನ್ಯ*             user )
  {
    FT_List_ಗಂಟು  cur;
    FT_Error     error = FT_Err_Ok;


    ಯದಿ ( !list || !iterator )
      ಪ್ರತ್ಯಾಗಮ FT_THROW( Invalid_Argument );

    cur = list->head;

    ಯಾವತ್ ( cur )
    {
      FT_List_ಗಂಟು  next = cur->next;


      error = iterator( cur, user );
      ಯದಿ ( error )
        ಭಙ್ಗ;

      cur = next;
    }

    ಪ್ರತ್ಯಾಗಮ error;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( ಶೂನ್ಯ )
  FT_List_Finalize( FT_List             list,
                    FT_List_Destructor  destroy,
                    FT_Memory           memory,
                    ಶೂನ್ಯ*               user )
  {
    FT_List_ಗಂಟು  cur;


    ಯದಿ ( !list || !memory )
      ಪ್ರತ್ಯಾಗಮ;

    cur = list->head;
    ಯಾವತ್ ( cur )
    {
      FT_List_ಗಂಟು  next = cur->next;


      ಯದಿ ( destroy )
        destroy( memory, cur->data, user );

      FT_FREE( cur );
      cur = next;
    }

    list->head = ರಿಕ್ತ;
    list->tail = ರಿಕ್ತ;
  }


/* END */
