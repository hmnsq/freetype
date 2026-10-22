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


#ضمن <freetype/internal/ftdebug.h>
#ضمن <freetype/internal/ftmemory.h>
#ضمن <freetype/internal/ftobjs.h>
#ضمن <freetype/ftlist.h>


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

    ܐܢ ( !error && block && size > 0 )
      FT_MEM_ZERO( block, size );

    *p_error = error;
    ܦܢܐ block;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_qalloc( FT_Memory  memory,
                 FT_Long    size,
                 FT_Error  *p_error )
  {
    FT_Error    error = FT_Err_Ok;
    FT_Pointer  block = عدم;


    ܐܢ ( size > 0 )
    {
      block = memory->alloc( memory, size );
      ܐܢ ( !block )
        error = FT_THROW( Out_Of_Memory );
    }
    ܐܚܪܢܐ ܐܢ ( size < 0 )
    {
      /* may help catch/prevent security issues */
      error = FT_THROW( Invalid_Argument );
    }

    *p_error = error;
    ܦܢܐ block;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_realloc( FT_Memory  memory,
                  FT_Long    item_size,
                  FT_Long    cur_count,
                  FT_Long    new_count,
                  فراغ*      block,
                  FT_Error  *p_error )
  {
    FT_Error  error = FT_Err_Ok;


    block = ft_mem_qrealloc( memory, item_size,
                             cur_count, new_count, block, &error );
    ܐܢ ( !error && block && new_count > cur_count )
      FT_MEM_ZERO( (حرف*)block + cur_count * item_size,
                   ( new_count - cur_count ) * item_size );

    *p_error = error;
    ܦܢܐ block;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_qrealloc( FT_Memory  memory,
                   FT_Long    item_size,
                   FT_Long    cur_count,
                   FT_Long    new_count,
                   فراغ*      block,
                   FT_Error  *p_error )
  {
    FT_Error  error = FT_Err_Ok;


    /* Note that we now accept `item_size == 0' as a valid parameter, in
     * order to cover very weird cases where an ALLOC_MULT macro would be
     * called.
     */
    ܐܢ ( cur_count < 0 || new_count < 0 || item_size < 0 )
    {
      /* may help catch/prevent nasty security issues */
      error = FT_THROW( Invalid_Argument );
    }
    ܐܚܪܢܐ ܐܢ ( new_count == 0 || item_size == 0 )
    {
      ft_mem_free( memory, block );
      block = عدم;
    }
    ܐܚܪܢܐ ܐܢ ( new_count > FT_INT_MAX / item_size )
    {
      error = FT_THROW( Array_Too_Large );
    }
    ܐܚܪܢܐ ܐܢ ( cur_count == 0 )
    {
      FT_ASSERT( !block );

      block = memory->alloc( memory, new_count * item_size );
      ܐܢ ( block == عدم )
        error = FT_THROW( Out_Of_Memory );
    }
    ܐܚܪܢܐ
    {
      FT_Pointer  block2;
      FT_Long     cur_size = cur_count * item_size;
      FT_Long     new_size = new_count * item_size;


      block2 = memory->realloc( memory, cur_size, new_size, block );
      ܐܢ ( !block2 )
        error = FT_THROW( Out_Of_Memory );
      ܐܚܪܢܐ
        block = block2;
    }

    *p_error = error;
    ܦܢܐ block;
  }


  FT_BASE_DEF( فراغ )
  ft_mem_free( FT_Memory   memory,
               ثابت فراغ *P )
  {
    ܐܢ ( P )
      memory->حرر( memory, (فراغ*)P );
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_dup( FT_Memory    memory,
              ثابت فراغ*  address,
              FT_ULong     size,
              FT_Error    *p_error )
  {
    FT_Error    error;
    FT_Pointer  p = ft_mem_qalloc( memory, (FT_Long)size, &error );


    ܐܢ ( !error && address && size > 0 )
      ft_memcpy( p, address, size );

    *p_error = error;
    ܦܢܐ p;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_strdup( FT_Memory    memory,
                 ثابت حرف*  str,
                 FT_Error    *p_error )
  {
    FT_ULong  len = str ? (FT_ULong)ft_strlen( str ) + 1
                        : 0;


    ܦܢܐ ft_mem_dup( memory, str, len, p_error );
  }


  FT_BASE_DEF( FT_Int )
  ft_mem_strcpyn( حرف*        dst,
                  ثابت حرف*  src,
                  FT_ULong     size )
  {
    ܟܡܐ ( size > 1 && *src != 0 )
    {
      *dst++ = *src++;
      size--;
    }

    *dst = 0;  /* always zero-terminate */

    ܦܢܐ *src != 0;
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

  FT_EXPORT_DEF( FT_ListNode )
  FT_List_Find( FT_List  list,
                فراغ*    data )
  {
    FT_ListNode  cur;


    ܐܢ ( !list )
      ܦܢܐ عدم;

    cur = list->head;
    ܟܡܐ ( cur )
    {
      ܐܢ ( cur->data == data )
        ܦܢܐ cur;

      cur = cur->next;
    }

    ܦܢܐ عدم;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( فراغ )
  FT_List_Add( FT_List      list,
               FT_ListNode  node )
  {
    FT_ListNode  before;


    ܐܢ ( !list || !node )
      ܦܢܐ;

    before = list->tail;

    node->next = عدم;
    node->prev = before;

    ܐܢ ( before )
      before->next = node;
    ܐܚܪܢܐ
      list->head = node;

    list->tail = node;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( فراغ )
  FT_List_Insert( FT_List      list,
                  FT_ListNode  node )
  {
    FT_ListNode  after;


    ܐܢ ( !list || !node )
      ܦܢܐ;

    after = list->head;

    node->next = after;
    node->prev = عدم;

    ܐܢ ( !after )
      list->tail = node;
    ܐܚܪܢܐ
      after->prev = node;

    list->head = node;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( فراغ )
  FT_List_Remove( FT_List      list,
                  FT_ListNode  node )
  {
    FT_ListNode  before, after;


    ܐܢ ( !list || !node )
      ܦܢܐ;

    before = node->prev;
    after  = node->next;

    ܐܢ ( before )
      before->next = after;
    ܐܚܪܢܐ
      list->head = after;

    ܐܢ ( after )
      after->prev = before;
    ܐܚܪܢܐ
      list->tail = before;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( فراغ )
  FT_List_Up( FT_List      list,
              FT_ListNode  node )
  {
    FT_ListNode  before, after;


    ܐܢ ( !list || !node )
      ܦܢܐ;

    before = node->prev;
    after  = node->next;

    /* check whether we are already on top of the list */
    ܐܢ ( !before )
      ܦܢܐ;

    before->next = after;

    ܐܢ ( after )
      after->prev = before;
    ܐܚܪܢܐ
      list->tail = before;

    node->prev       = عدم;
    node->next       = list->head;
    list->head->prev = node;
    list->head       = node;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( FT_Error )
  FT_List_Iterate( FT_List           list,
                   FT_List_Iterator  iterator,
                   فراغ*             user )
  {
    FT_ListNode  cur;
    FT_Error     error = FT_Err_Ok;


    ܐܢ ( !list || !iterator )
      ܦܢܐ FT_THROW( Invalid_Argument );

    cur = list->head;

    ܟܡܐ ( cur )
    {
      FT_ListNode  next = cur->next;


      error = iterator( cur, user );
      ܐܢ ( error )
        اكسر;

      cur = next;
    }

    ܦܢܐ error;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( فراغ )
  FT_List_Finalize( FT_List             list,
                    FT_List_Destructor  destroy,
                    FT_Memory           memory,
                    فراغ*               user )
  {
    FT_ListNode  cur;


    ܐܢ ( !list || !memory )
      ܦܢܐ;

    cur = list->head;
    ܟܡܐ ( cur )
    {
      FT_ListNode  next = cur->next;


      ܐܢ ( destroy )
        destroy( memory, cur->data, user );

      FT_FREE( cur );
      cur = next;
    }

    list->head = عدم;
    list->tail = عدم;
  }


/* END */
