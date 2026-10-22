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

    ຖ້າ ( !error && block && size > 0 )
      FT_MEM_ZERO( block, size );

    *p_error = error;
    ສົ່ງຄືນ block;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_qalloc( FT_Memory  memory,
                 FT_Long    size,
                 FT_Error  *p_error )
  {
    FT_Error    error = FT_Err_Ok;
    FT_Pointer  block = NULL;


    ຖ້າ ( size > 0 )
    {
      block = memory->alloc( memory, size );
      ຖ້າ ( !block )
        error = FT_THROW( Out_Of_Memory );
    }
    ບໍ່ດັ່ງນັ້ນ ຖ້າ ( size < 0 )
    {
      /* may help catch/prevent security issues */
      error = FT_THROW( Invalid_Argument );
    }

    *p_error = error;
    ສົ່ງຄືນ block;
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
    ຖ້າ ( !error && block && new_count > cur_count )
      FT_MEM_ZERO( (var_na*)block + cur_count * item_size,
                   ( new_count - cur_count ) * item_size );

    *p_error = error;
    ສົ່ງຄືນ block;
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
    ຖ້າ ( cur_count < 0 || new_count < 0 || item_size < 0 )
    {
      /* may help catch/prevent nasty security issues */
      error = FT_THROW( Invalid_Argument );
    }
    ບໍ່ດັ່ງນັ້ນ ຖ້າ ( new_count == 0 || item_size == 0 )
    {
      ft_mem_free( memory, block );
      block = NULL;
    }
    ບໍ່ດັ່ງນັ້ນ ຖ້າ ( new_count > FT_INT_MAX / item_size )
    {
      error = FT_THROW( Array_Too_Large );
    }
    ບໍ່ດັ່ງນັ້ນ ຖ້າ ( cur_count == 0 )
    {
      FT_ASSERT( !block );

      block = memory->alloc( memory, new_count * item_size );
      ຖ້າ ( block == NULL )
        error = FT_THROW( Out_Of_Memory );
    }
    ບໍ່ດັ່ງນັ້ນ
    {
      FT_Pointer  block2;
      FT_Long     cur_size = cur_count * item_size;
      FT_Long     new_size = new_count * item_size;


      block2 = memory->realloc( memory, cur_size, new_size, block );
      ຖ້າ ( !block2 )
        error = FT_THROW( Out_Of_Memory );
      ບໍ່ດັ່ງນັ້ນ
        block = block2;
    }

    *p_error = error;
    ສົ່ງຄືນ block;
  }


  FT_BASE_DEF( void )
  ft_mem_free( FT_Memory   memory,
               const void *P )
  {
    ຖ້າ ( P )
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


    ຖ້າ ( !error && address && size > 0 )
      ft_memcpy( p, address, size );

    *p_error = error;
    ສົ່ງຄືນ p;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_strdup( FT_Memory    memory,
                 const var_na*  str,
                 FT_Error    *p_error )
  {
    FT_ULong  len = str ? (FT_ULong)ft_strlen( str ) + 1
                        : 0;


    ສົ່ງຄືນ ft_mem_dup( memory, str, len, p_error );
  }


  FT_BASE_DEF( FT_Int )
  ft_mem_strcpyn( var_na*        dst,
                  const var_na*  src,
                  FT_ULong     size )
  {
    ໃນຂະນະທີ່ ( size > 1 && *src != 0 )
    {
      *dst++ = *src++;
      size--;
    }

    *dst = 0;  /* always zero-terminate */

    ສົ່ງຄືນ *src != 0;
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


    ຖ້າ ( !list )
      ສົ່ງຄືນ NULL;

    cur = list->head;
    ໃນຂະນະທີ່ ( cur )
    {
      ຖ້າ ( cur->data == data )
        ສົ່ງຄືນ cur;

      cur = cur->next;
    }

    ສົ່ງຄືນ NULL;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( void )
  FT_List_Add( FT_List      list,
               FT_ListNode  node )
  {
    FT_ListNode  before;


    ຖ້າ ( !list || !node )
      ສົ່ງຄືນ;

    before = list->tail;

    node->next = NULL;
    node->prev = before;

    ຖ້າ ( before )
      before->next = node;
    ບໍ່ດັ່ງນັ້ນ
      list->head = node;

    list->tail = node;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( void )
  FT_List_Insert( FT_List      list,
                  FT_ListNode  node )
  {
    FT_ListNode  after;


    ຖ້າ ( !list || !node )
      ສົ່ງຄືນ;

    after = list->head;

    node->next = after;
    node->prev = NULL;

    ຖ້າ ( !after )
      list->tail = node;
    ບໍ່ດັ່ງນັ້ນ
      after->prev = node;

    list->head = node;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( void )
  FT_List_Remove( FT_List      list,
                  FT_ListNode  node )
  {
    FT_ListNode  before, after;


    ຖ້າ ( !list || !node )
      ສົ່ງຄືນ;

    before = node->prev;
    after  = node->next;

    ຖ້າ ( before )
      before->next = after;
    ບໍ່ດັ່ງນັ້ນ
      list->head = after;

    ຖ້າ ( after )
      after->prev = before;
    ບໍ່ດັ່ງນັ້ນ
      list->tail = before;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( void )
  FT_List_Up( FT_List      list,
              FT_ListNode  node )
  {
    FT_ListNode  before, after;


    ຖ້າ ( !list || !node )
      ສົ່ງຄືນ;

    before = node->prev;
    after  = node->next;

    /* check whether we are already on top of the list */
    ຖ້າ ( !before )
      ສົ່ງຄືນ;

    before->next = after;

    ຖ້າ ( after )
      after->prev = before;
    ບໍ່ດັ່ງນັ້ນ
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


    ຖ້າ ( !list || !iterator )
      ສົ່ງຄືນ FT_THROW( Invalid_Argument );

    cur = list->head;

    ໃນຂະນະທີ່ ( cur )
    {
      FT_ListNode  next = cur->next;


      error = iterator( cur, user );
      ຖ້າ ( error )
        bha_nga;

      cur = next;
    }

    ສົ່ງຄືນ error;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( void )
  FT_List_Finalize( FT_List             list,
                    FT_List_Destructor  destroy,
                    FT_Memory           memory,
                    void*               user )
  {
    FT_ListNode  cur;


    ຖ້າ ( !list || !memory )
      ສົ່ງຄືນ;

    cur = list->head;
    ໃນຂະນະທີ່ ( cur )
    {
      FT_ListNode  next = cur->next;


      ຖ້າ ( destroy )
        destroy( memory, cur->data, user );

      FT_FREE( cur );
      cur = next;
    }

    list->head = NULL;
    list->tail = NULL;
  }


/* END */
