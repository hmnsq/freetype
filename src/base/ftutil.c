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


#include <freetype/internal/ftdebug.h>
#include <freetype/internal/ftmemory.h>
#include <freetype/internal/ftobjs.h>
#include <freetype/ftlist.h>


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

    ប្រសិនបើ ( !error && block && size > 0 )
      FT_MEM_ZERO( block, size );

    *p_error = error;
    ត្រឡប់ block;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_qalloc( FT_Memory  memory,
                 FT_Long    size,
                 FT_Error  *p_error )
  {
    FT_Error    error = FT_Err_Ok;
    FT_Pointer  block = NULL;


    ប្រសិនបើ ( size > 0 )
    {
      block = memory->alloc( memory, size );
      ប្រសិនបើ ( !block )
        error = FT_THROW( Out_Of_Memory );
    }
    បើមិនដូច្នេះ ប្រសិនបើ ( size < 0 )
    {
      /* may help catch/prevent security issues */
      error = FT_THROW( Invalid_Argument );
    }

    *p_error = error;
    ត្រឡប់ block;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_realloc( FT_Memory  memory,
                  FT_Long    item_size,
                  FT_Long    cur_count,
                  FT_Long    new_count,
                  void*      block,
                  FT_Error  *p_error )
  {
    FT_Error  error = FT_Err_Ok;


    block = ft_mem_qrealloc( memory, item_size,
                             cur_count, new_count, block, &error );
    ប្រសិនបើ ( !error && block && new_count > cur_count )
      FT_MEM_ZERO( (var_na*)block + cur_count * item_size,
                   ( new_count - cur_count ) * item_size );

    *p_error = error;
    ត្រឡប់ block;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_qrealloc( FT_Memory  memory,
                   FT_Long    item_size,
                   FT_Long    cur_count,
                   FT_Long    new_count,
                   void*      block,
                   FT_Error  *p_error )
  {
    FT_Error  error = FT_Err_Ok;


    /* Note that we now accept `item_size == 0' as a valid parameter, in
     * order to cover very weird cases where an ALLOC_MULT macro would be
     * called.
     */
    ប្រសិនបើ ( cur_count < 0 || new_count < 0 || item_size < 0 )
    {
      /* may help catch/prevent nasty security issues */
      error = FT_THROW( Invalid_Argument );
    }
    បើមិនដូច្នេះ ប្រសិនបើ ( new_count == 0 || item_size == 0 )
    {
      ft_mem_free( memory, block );
      block = NULL;
    }
    បើមិនដូច្នេះ ប្រសិនបើ ( new_count > FT_INT_MAX / item_size )
    {
      error = FT_THROW( Array_Too_Large );
    }
    បើមិនដូច្នេះ ប្រសិនបើ ( cur_count == 0 )
    {
      FT_ASSERT( !block );

      block = memory->alloc( memory, new_count * item_size );
      ប្រសិនបើ ( block == NULL )
        error = FT_THROW( Out_Of_Memory );
    }
    បើមិនដូច្នេះ
    {
      FT_Pointer  block2;
      FT_Long     cur_size = cur_count * item_size;
      FT_Long     new_size = new_count * item_size;


      block2 = memory->realloc( memory, cur_size, new_size, block );
      ប្រសិនបើ ( !block2 )
        error = FT_THROW( Out_Of_Memory );
      បើមិនដូច្នេះ
        block = block2;
    }

    *p_error = error;
    ត្រឡប់ block;
  }


  FT_BASE_DEF( void )
  ft_mem_free( FT_Memory   memory,
               const void *P )
  {
    ប្រសិនបើ ( P )
      memory->free( memory, (void*)P );
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_dup( FT_Memory    memory,
              const void*  address,
              FT_ULong     size,
              FT_Error    *p_error )
  {
    FT_Error    error;
    FT_Pointer  p = ft_mem_qalloc( memory, (FT_Long)size, &error );


    ប្រសិនបើ ( !error && address && size > 0 )
      ft_memcpy( p, address, size );

    *p_error = error;
    ត្រឡប់ p;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_strdup( FT_Memory    memory,
                 const var_na*  str,
                 FT_Error    *p_error )
  {
    FT_ULong  len = str ? (FT_ULong)ft_strlen( str ) + 1
                        : 0;


    ត្រឡប់ ft_mem_dup( memory, str, len, p_error );
  }


  FT_BASE_DEF( FT_Int )
  ft_mem_strcpyn( var_na*        dst,
                  const var_na*  src,
                  FT_ULong     size )
  {
    ខណៈពេល ( size > 1 && *src != 0 )
    {
      *dst++ = *src++;
      size--;
    }

    *dst = 0;  /* always zero-terminate */

    ត្រឡប់ *src != 0;
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
                void*    data )
  {
    FT_ListNode  cur;


    ប្រសិនបើ ( !list )
      ត្រឡប់ NULL;

    cur = list->head;
    ខណៈពេល ( cur )
    {
      ប្រសិនបើ ( cur->data == data )
        ត្រឡប់ cur;

      cur = cur->next;
    }

    ត្រឡប់ NULL;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( void )
  FT_List_Add( FT_List      list,
               FT_ListNode  node )
  {
    FT_ListNode  before;


    ប្រសិនបើ ( !list || !node )
      ត្រឡប់;

    before = list->tail;

    node->next = NULL;
    node->prev = before;

    ប្រសិនបើ ( before )
      before->next = node;
    បើមិនដូច្នេះ
      list->head = node;

    list->tail = node;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( void )
  FT_List_Insert( FT_List      list,
                  FT_ListNode  node )
  {
    FT_ListNode  after;


    ប្រសិនបើ ( !list || !node )
      ត្រឡប់;

    after = list->head;

    node->next = after;
    node->prev = NULL;

    ប្រសិនបើ ( !after )
      list->tail = node;
    បើមិនដូច្នេះ
      after->prev = node;

    list->head = node;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( void )
  FT_List_Remove( FT_List      list,
                  FT_ListNode  node )
  {
    FT_ListNode  before, after;


    ប្រសិនបើ ( !list || !node )
      ត្រឡប់;

    before = node->prev;
    after  = node->next;

    ប្រសិនបើ ( before )
      before->next = after;
    បើមិនដូច្នេះ
      list->head = after;

    ប្រសិនបើ ( after )
      after->prev = before;
    បើមិនដូច្នេះ
      list->tail = before;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( void )
  FT_List_Up( FT_List      list,
              FT_ListNode  node )
  {
    FT_ListNode  before, after;


    ប្រសិនបើ ( !list || !node )
      ត្រឡប់;

    before = node->prev;
    after  = node->next;

    /* check whether we are already on top of the list */
    ប្រសិនបើ ( !before )
      ត្រឡប់;

    before->next = after;

    ប្រសិនបើ ( after )
      after->prev = before;
    បើមិនដូច្នេះ
      list->tail = before;

    node->prev       = NULL;
    node->next       = list->head;
    list->head->prev = node;
    list->head       = node;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( FT_Error )
  FT_List_Iterate( FT_List           list,
                   FT_List_Iterator  iterator,
                   void*             user )
  {
    FT_ListNode  cur;
    FT_Error     error = FT_Err_Ok;


    ប្រសិនបើ ( !list || !iterator )
      ត្រឡប់ FT_THROW( Invalid_Argument );

    cur = list->head;

    ខណៈពេល ( cur )
    {
      FT_ListNode  next = cur->next;


      error = iterator( cur, user );
      ប្រសិនបើ ( error )
        bha_nga;

      cur = next;
    }

    ត្រឡប់ error;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( void )
  FT_List_Finalize( FT_List             list,
                    FT_List_Destructor  destroy,
                    FT_Memory           memory,
                    void*               user )
  {
    FT_ListNode  cur;


    ប្រសិនបើ ( !list || !memory )
      ត្រឡប់;

    cur = list->head;
    ខណៈពេល ( cur )
    {
      FT_ListNode  next = cur->next;


      ប្រសិនបើ ( destroy )
        destroy( memory, cur->data, user );

      FT_FREE( cur );
      cur = next;
    }

    list->head = NULL;
    list->tail = NULL;
  }


/* END */
