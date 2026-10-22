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

    if ( !error && block && size > 0 )
      FT_MEM_ZERO( block, size );

    *p_error = error;
    return block;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_qalloc( FT_Memory  memory,
                 FT_Long    size,
                 FT_Error  *p_error )
  {
    FT_Error    error = FT_Err_Ok;
    FT_Pointer  block = NULL;


    if ( size > 0 )
    {
      block = memory->alloc( memory, size );
      if ( !block )
        error = FT_THROW( Out_Of_Memory );
    }
    else if ( size < 0 )
    {
      /* may help catch/prevent security issues */
      error = FT_THROW( Invalid_Argument );
    }

    *p_error = error;
    return block;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_realloc( FT_Memory  memory,
                  FT_Long    item_size,
                  FT_Long    cur_बिलांग,
                  FT_Long    new_बिलांग,
                  void*      block,
                  FT_Error  *p_error )
  {
    FT_Error  error = FT_Err_Ok;


    block = ft_mem_qrealloc( memory, item_size,
                             cur_बिलांग, new_बिलांग, block, &error );
    if ( !error && block && new_बिलांग > cur_बिलांग )
      FT_MEM_ZERO( (char*)block + cur_बिलांग * item_size,
                   ( new_बिलांग - cur_बिलांग ) * item_size );

    *p_error = error;
    return block;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_qrealloc( FT_Memory  memory,
                   FT_Long    item_size,
                   FT_Long    cur_बिलांग,
                   FT_Long    new_बिलांग,
                   void*      block,
                   FT_Error  *p_error )
  {
    FT_Error  error = FT_Err_Ok;


    /* Note that we now accept `item_size == 0' as a valid parameter, in
     * order to cover very weird cases where an ALLOC_MULT macro would be
     * called.
     */
    if ( cur_बिलांग < 0 || new_बिलांग < 0 || item_size < 0 )
    {
      /* may help catch/prevent nasty security issues */
      error = FT_THROW( Invalid_Argument );
    }
    else if ( new_बिलांग == 0 || item_size == 0 )
    {
      ft_mem_free( memory, block );
      block = NULL;
    }
    else if ( new_बिलांग > FT_INT_MAX / item_size )
    {
      error = FT_THROW( Array_Too_Large );
    }
    else if ( cur_बिलांग == 0 )
    {
      FT_ASSERT( !block );

      block = memory->alloc( memory, new_बिलांग * item_size );
      if ( block == NULL )
        error = FT_THROW( Out_Of_Memory );
    }
    else
    {
      FT_Pointer  block2;
      FT_Long     cur_size = cur_बिलांग * item_size;
      FT_Long     new_size = new_बिलांग * item_size;


      block2 = memory->realloc( memory, cur_size, new_size, block );
      if ( !block2 )
        error = FT_THROW( Out_Of_Memory );
      else
        block = block2;
    }

    *p_error = error;
    return block;
  }


  FT_BASE_DEF( void )
  ft_mem_free( FT_Memory   memory,
               const void *P )
  {
    if ( P )
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


    if ( !error && address && size > 0 )
      ft_memcpy( p, address, size );

    *p_error = error;
    return p;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_strdup( FT_Memory    memory,
                 const char*  str,
                 FT_Error    *p_error )
  {
    FT_ULong  len = str ? (FT_ULong)ft_strlen( str ) + 1
                        : 0;


    return ft_mem_dup( memory, str, len, p_error );
  }


  FT_BASE_DEF( FT_Int )
  ft_mem_strcpyn( char*        dst,
                  const char*  src,
                  FT_ULong     size )
  {
    while ( size > 1 && *src != 0 )
    {
      *dst++ = *src++;
      size--;
    }

    *dst = 0;  /* always zero-terminate */

    return *src != 0;
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

  FT_EXPORT_DEF( FT_List_बुहोल )
  FT_List_Find( FT_List  list,
                void*    data )
  {
    FT_List_बुहोल  cur;


    if ( !list )
      return NULL;

    cur = list->head;
    while ( cur )
    {
      if ( cur->data == data )
        return cur;

      cur = cur->next;
    }

    return NULL;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( void )
  FT_List_Add( FT_List      list,
               FT_List_बुहोल  बुहोल )
  {
    FT_List_बुहोल  before;


    if ( !list || !बुहोल )
      return;

    before = list->tail;

    बुहोल->next = NULL;
    बुहोल->prev = before;

    if ( before )
      before->next = बुहोल;
    else
      list->head = बुहोल;

    list->tail = बुहोल;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( void )
  FT_List_इसिंगित( FT_List      list,
                  FT_List_बुहोल  बुहोल )
  {
    FT_List_बुहोल  after;


    if ( !list || !बुहोल )
      return;

    after = list->head;

    बुहोल->next = after;
    बुहोल->prev = NULL;

    if ( !after )
      list->tail = बुहोल;
    else
      after->prev = बुहोल;

    list->head = बुहोल;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( void )
  FT_List_Remove( FT_List      list,
                  FT_List_बुहोल  बुहोल )
  {
    FT_List_बुहोल  before, after;


    if ( !list || !बुहोल )
      return;

    before = बुहोल->prev;
    after  = बुहोल->next;

    if ( before )
      before->next = after;
    else
      list->head = after;

    if ( after )
      after->prev = before;
    else
      list->tail = before;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( void )
  FT_List_Up( FT_List      list,
              FT_List_बुहोल  बुहोल )
  {
    FT_List_बुहोल  before, after;


    if ( !list || !बुहोल )
      return;

    before = बुहोल->prev;
    after  = बुहोल->next;

    /* check whether we are already on top of the list */
    if ( !before )
      return;

    before->next = after;

    if ( after )
      after->prev = before;
    else
      list->tail = before;

    बुहोल->prev       = NULL;
    बुहोल->next       = list->head;
    list->head->prev = बुहोल;
    list->head       = बुहोल;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( FT_Error )
  FT_List_Iterate( FT_List           list,
                   FT_List_Iterator  iterator,
                   void*             user )
  {
    FT_List_बुहोल  cur;
    FT_Error     error = FT_Err_Ok;


    if ( !list || !iterator )
      return FT_THROW( Invalid_Argument );

    cur = list->head;

    while ( cur )
    {
      FT_List_बुहोल  next = cur->next;


      error = iterator( cur, user );
      if ( error )
        break;

      cur = next;
    }

    return error;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( void )
  FT_List_Finalize( FT_List             list,
                    FT_List_Destructor  destroy,
                    FT_Memory           memory,
                    void*               user )
  {
    FT_List_बुहोल  cur;


    if ( !list || !memory )
      return;

    cur = list->head;
    while ( cur )
    {
      FT_List_बुहोल  next = cur->next;


      if ( destroy )
        destroy( memory, cur->data, user );

      FT_FREE( cur );
      cur = next;
    }

    list->head = NULL;
    list->tail = NULL;
  }


/* END */
