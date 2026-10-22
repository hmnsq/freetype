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

    ᮚᮦᮔ᮪ ( !error && block && size > 0 )
      FT_MEM_ZERO( block, size );

    *p_error = error;
    ᮝᮜᮤ block;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_qalloc( FT_Memory  memory,
                 FT_Long    size,
                 FT_Error  *p_error )
  {
    FT_Error    error = FT_Err_Ok;
    FT_Pointer  block = NULL;


    ᮚᮦᮔ᮪ ( size > 0 )
    {
      block = memory->alloc( memory, size );
      ᮚᮦᮔ᮪ ( !block )
        error = FT_THROW( Out_Of_Memory );
    }
    ᮜᮤᮚ ᮚᮦᮔ᮪ ( size < 0 )
    {
      /* may help catch/prevent security issues */
      error = FT_THROW( Invalid_Argument );
    }

    *p_error = error;
    ᮝᮜᮤ block;
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
    ᮚᮦᮔ᮪ ( !error && block && new_count > cur_count )
      FT_MEM_ZERO( (var_na*)block + cur_count * item_size,
                   ( new_count - cur_count ) * item_size );

    *p_error = error;
    ᮝᮜᮤ block;
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
    ᮚᮦᮔ᮪ ( cur_count < 0 || new_count < 0 || item_size < 0 )
    {
      /* may help catch/prevent nasty security issues */
      error = FT_THROW( Invalid_Argument );
    }
    ᮜᮤᮚ ᮚᮦᮔ᮪ ( new_count == 0 || item_size == 0 )
    {
      ft_mem_free( memory, block );
      block = NULL;
    }
    ᮜᮤᮚ ᮚᮦᮔ᮪ ( new_count > FT_INT_MAX / item_size )
    {
      error = FT_THROW( Array_Too_Large );
    }
    ᮜᮤᮚ ᮚᮦᮔ᮪ ( cur_count == 0 )
    {
      FT_ASSERT( !block );

      block = memory->alloc( memory, new_count * item_size );
      ᮚᮦᮔ᮪ ( block == NULL )
        error = FT_THROW( Out_Of_Memory );
    }
    ᮜᮤᮚ
    {
      FT_Pointer  block2;
      FT_Long     cur_size = cur_count * item_size;
      FT_Long     new_size = new_count * item_size;


      block2 = memory->realloc( memory, cur_size, new_size, block );
      ᮚᮦᮔ᮪ ( !block2 )
        error = FT_THROW( Out_Of_Memory );
      ᮜᮤᮚ
        block = block2;
    }

    *p_error = error;
    ᮝᮜᮤ block;
  }


  FT_BASE_DEF( void )
  ft_mem_free( FT_Memory   memory,
               const void *P )
  {
    ᮚᮦᮔ᮪ ( P )
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


    ᮚᮦᮔ᮪ ( !error && address && size > 0 )
      ft_memcpy( p, address, size );

    *p_error = error;
    ᮝᮜᮤ p;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_strdup( FT_Memory    memory,
                 const var_na*  str,
                 FT_Error    *p_error )
  {
    FT_ULong  len = str ? (FT_ULong)ft_strlen( str ) + 1
                        : 0;


    ᮝᮜᮤ ft_mem_dup( memory, str, len, p_error );
  }


  FT_BASE_DEF( FT_Int )
  ft_mem_strcpyn( var_na*        dst,
                  const var_na*  src,
                  FT_ULong     size )
  {
    ᮞᮨᮓᮨᮜ ( size > 1 && *src != 0 )
    {
      *dst++ = *src++;
      size--;
    }

    *dst = 0;  /* always zero-terminate */

    ᮝᮜᮤ *src != 0;
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


    ᮚᮦᮔ᮪ ( !list )
      ᮝᮜᮤ NULL;

    cur = list->head;
    ᮞᮨᮓᮨᮜ ( cur )
    {
      ᮚᮦᮔ᮪ ( cur->data == data )
        ᮝᮜᮤ cur;

      cur = cur->next;
    }

    ᮝᮜᮤ NULL;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( void )
  FT_List_Add( FT_List      list,
               FT_ListNode  node )
  {
    FT_ListNode  before;


    ᮚᮦᮔ᮪ ( !list || !node )
      ᮝᮜᮤ;

    before = list->tail;

    node->next = NULL;
    node->prev = before;

    ᮚᮦᮔ᮪ ( before )
      before->next = node;
    ᮜᮤᮚ
      list->head = node;

    list->tail = node;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( void )
  FT_List_Insert( FT_List      list,
                  FT_ListNode  node )
  {
    FT_ListNode  after;


    ᮚᮦᮔ᮪ ( !list || !node )
      ᮝᮜᮤ;

    after = list->head;

    node->next = after;
    node->prev = NULL;

    ᮚᮦᮔ᮪ ( !after )
      list->tail = node;
    ᮜᮤᮚ
      after->prev = node;

    list->head = node;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( void )
  FT_List_Remove( FT_List      list,
                  FT_ListNode  node )
  {
    FT_ListNode  before, after;


    ᮚᮦᮔ᮪ ( !list || !node )
      ᮝᮜᮤ;

    before = node->prev;
    after  = node->next;

    ᮚᮦᮔ᮪ ( before )
      before->next = after;
    ᮜᮤᮚ
      list->head = after;

    ᮚᮦᮔ᮪ ( after )
      after->prev = before;
    ᮜᮤᮚ
      list->tail = before;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( void )
  FT_List_Up( FT_List      list,
              FT_ListNode  node )
  {
    FT_ListNode  before, after;


    ᮚᮦᮔ᮪ ( !list || !node )
      ᮝᮜᮤ;

    before = node->prev;
    after  = node->next;

    /* check whether we are already on top of the list */
    ᮚᮦᮔ᮪ ( !before )
      ᮝᮜᮤ;

    before->next = after;

    ᮚᮦᮔ᮪ ( after )
      after->prev = before;
    ᮜᮤᮚ
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


    ᮚᮦᮔ᮪ ( !list || !iterator )
      ᮝᮜᮤ FT_THROW( Invalid_Argument );

    cur = list->head;

    ᮞᮨᮓᮨᮜ ( cur )
    {
      FT_ListNode  next = cur->next;


      error = iterator( cur, user );
      ᮚᮦᮔ᮪ ( error )
        bha_nga;

      cur = next;
    }

    ᮝᮜᮤ error;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( void )
  FT_List_Finalize( FT_List             list,
                    FT_List_Destructor  destroy,
                    FT_Memory           memory,
                    void*               user )
  {
    FT_ListNode  cur;


    ᮚᮦᮔ᮪ ( !list || !memory )
      ᮝᮜᮤ;

    cur = list->head;
    ᮞᮨᮓᮨᮜ ( cur )
    {
      FT_ListNode  next = cur->next;


      ᮚᮦᮔ᮪ ( destroy )
        destroy( memory, cur->data, user );

      FT_FREE( cur );
      cur = next;
    }

    list->head = NULL;
    list->tail = NULL;
  }


/* END */
