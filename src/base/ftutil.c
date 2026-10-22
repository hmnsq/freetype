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


#شامل <freetype/internal/ftdebug.h>
#شامل <freetype/internal/ftmemory.h>
#شامل <freetype/internal/ftobjs.h>
#شامل <freetype/ftlist.h>


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

    اگر ( !error && block && size > 0 )
      FT_MEM_ZERO( block, size );

    *p_error = error;
    بازگشت block;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_qalloc( FT_Memory  memory,
                 FT_Long    size,
                 FT_Error  *p_error )
  {
    FT_Error    error = FT_Err_Ok;
    FT_Pointer  block = پوچ;


    اگر ( size > 0 )
    {
      block = memory->alloc( memory, size );
      اگر ( !block )
        error = FT_THROW( Out_Of_Memory );
    }
    وگرنه اگر ( size < 0 )
    {
      /* may help catch/prevent security issues */
      error = FT_THROW( Invalid_Argument );
    }

    *p_error = error;
    بازگشت block;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_realloc( FT_Memory  memory,
                  FT_Long    item_size,
                  FT_Long    cur_شمار,
                  FT_Long    new_شمار,
                  تهی*      block,
                  FT_Error  *p_error )
  {
    FT_Error  error = FT_Err_Ok;


    block = ft_mem_qrealloc( memory, item_size,
                             cur_شمار, new_شمار, block, &error );
    اگر ( !error && block && new_شمار > cur_شمار )
      FT_MEM_ZERO( (نویسه*)block + cur_شمار * item_size,
                   ( new_شمار - cur_شمار ) * item_size );

    *p_error = error;
    بازگشت block;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_qrealloc( FT_Memory  memory,
                   FT_Long    item_size,
                   FT_Long    cur_شمار,
                   FT_Long    new_شمار,
                   تهی*      block,
                   FT_Error  *p_error )
  {
    FT_Error  error = FT_Err_Ok;


    /* Note that we now accept `item_size == 0' as a valid parameter, in
     * order to cover very weird cases where an ALLOC_MULT macro would be
     * called.
     */
    اگر ( cur_شمار < 0 || new_شمار < 0 || item_size < 0 )
    {
      /* may help catch/prevent nasty security issues */
      error = FT_THROW( Invalid_Argument );
    }
    وگرنه اگر ( new_شمار == 0 || item_size == 0 )
    {
      ft_mem_free( memory, block );
      block = پوچ;
    }
    وگرنه اگر ( new_شمار > FT_INT_MAX / item_size )
    {
      error = FT_THROW( Array_Too_Large );
    }
    وگرنه اگر ( cur_شمار == 0 )
    {
      FT_ASSERT( !block );

      block = memory->alloc( memory, new_شمار * item_size );
      اگر ( block == پوچ )
        error = FT_THROW( Out_Of_Memory );
    }
    وگرنه
    {
      FT_Pointer  block2;
      FT_Long     cur_size = cur_شمار * item_size;
      FT_Long     new_size = new_شمار * item_size;


      block2 = memory->realloc( memory, cur_size, new_size, block );
      اگر ( !block2 )
        error = FT_THROW( Out_Of_Memory );
      وگرنه
        block = block2;
    }

    *p_error = error;
    بازگشت block;
  }


  FT_BASE_DEF( تهی )
  ft_mem_free( FT_Memory   memory,
               ثابت تهی *P )
  {
    اگر ( P )
      memory->آزاد( memory, (تهی*)P );
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_dup( FT_Memory    memory,
              ثابت تهی*  address,
              FT_ULong     size,
              FT_Error    *p_error )
  {
    FT_Error    error;
    FT_Pointer  p = ft_mem_qalloc( memory, (FT_Long)size, &error );


    اگر ( !error && address && size > 0 )
      ft_memcpy( p, address, size );

    *p_error = error;
    بازگشت p;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_strdup( FT_Memory    memory,
                 ثابت نویسه*  str,
                 FT_Error    *p_error )
  {
    FT_ULong  len = str ? (FT_ULong)ft_strlen( str ) + 1
                        : 0;


    بازگشت ft_mem_dup( memory, str, len, p_error );
  }


  FT_BASE_DEF( FT_Int )
  ft_mem_strcpyn( نویسه*        dst,
                  ثابت نویسه*  src,
                  FT_ULong     size )
  {
    وقتی ( size > 1 && *src != 0 )
    {
      *dst++ = *src++;
      size--;
    }

    *dst = 0;  /* always zero-terminate */

    بازگشت *src != 0;
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

  FT_EXPORT_DEF( FT_List_گره )
  FT_List_Find( FT_List  list,
                تهی*    data )
  {
    FT_List_گره  cur;


    اگر ( !list )
      بازگشت پوچ;

    cur = list->head;
    وقتی ( cur )
    {
      اگر ( cur->data == data )
        بازگشت cur;

      cur = cur->next;
    }

    بازگشت پوچ;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( تهی )
  FT_List_Add( FT_List      list,
               FT_List_گره  گره )
  {
    FT_List_گره  before;


    اگر ( !list || !گره )
      بازگشت;

    before = list->tail;

    گره->next = پوچ;
    گره->prev = before;

    اگر ( before )
      before->next = گره;
    وگرنه
      list->head = گره;

    list->tail = گره;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( تهی )
  FT_List_درج( FT_List      list,
                  FT_List_گره  گره )
  {
    FT_List_گره  after;


    اگر ( !list || !گره )
      بازگشت;

    after = list->head;

    گره->next = after;
    گره->prev = پوچ;

    اگر ( !after )
      list->tail = گره;
    وگرنه
      after->prev = گره;

    list->head = گره;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( تهی )
  FT_List_Remove( FT_List      list,
                  FT_List_گره  گره )
  {
    FT_List_گره  before, after;


    اگر ( !list || !گره )
      بازگشت;

    before = گره->prev;
    after  = گره->next;

    اگر ( before )
      before->next = after;
    وگرنه
      list->head = after;

    اگر ( after )
      after->prev = before;
    وگرنه
      list->tail = before;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( تهی )
  FT_List_Up( FT_List      list,
              FT_List_گره  گره )
  {
    FT_List_گره  before, after;


    اگر ( !list || !گره )
      بازگشت;

    before = گره->prev;
    after  = گره->next;

    /* check whether we are already on top of the list */
    اگر ( !before )
      بازگشت;

    before->next = after;

    اگر ( after )
      after->prev = before;
    وگرنه
      list->tail = before;

    گره->prev       = پوچ;
    گره->next       = list->head;
    list->head->prev = گره;
    list->head       = گره;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( FT_Error )
  FT_List_Iterate( FT_List           list,
                   FT_List_Iterator  iterator,
                   تهی*             user )
  {
    FT_List_گره  cur;
    FT_Error     error = FT_Err_Ok;


    اگر ( !list || !iterator )
      بازگشت FT_THROW( Invalid_Argument );

    cur = list->head;

    وقتی ( cur )
    {
      FT_List_گره  next = cur->next;


      error = iterator( cur, user );
      اگر ( error )
        بشکن;

      cur = next;
    }

    بازگشت error;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( تهی )
  FT_List_Finalize( FT_List             list,
                    FT_List_Destructor  destroy,
                    FT_Memory           memory,
                    تهی*               user )
  {
    FT_List_گره  cur;


    اگر ( !list || !memory )
      بازگشت;

    cur = list->head;
    وقتی ( cur )
    {
      FT_List_گره  next = cur->next;


      اگر ( destroy )
        destroy( memory, cur->data, user );

      FT_FREE( cur );
      cur = next;
    }

    list->head = پوچ;
    list->tail = پوچ;
  }


/* END */
