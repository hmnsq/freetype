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

    إذا ( !error && block && size > 0 )
      FT_MEM_ZERO( block, size );

    *p_error = error;
    أرجع block;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_qalloc( FT_Memory  memory,
                 FT_Long    size,
                 FT_Error  *p_error )
  {
    FT_Error    error = FT_Err_Ok;
    FT_Pointer  block = عدم;


    إذا ( size > 0 )
    {
      block = memory->alloc( memory, size );
      إذا ( !block )
        error = FT_THROW( Out_Of_Memory );
    }
    وإلا إذا ( size < 0 )
    {
      /* may help catch/prevent security issues */
      error = FT_THROW( Invalid_Argument );
    }

    *p_error = error;
    أرجع block;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_realloc( FT_Memory  memory,
                  FT_Long    item_size,
                  FT_Long    cur_ژماردن,
                  FT_Long    new_ژماردن,
                  فراغ*      block,
                  FT_Error  *p_error )
  {
    FT_Error  error = FT_Err_Ok;


    block = ft_mem_qrealloc( memory, item_size,
                             cur_ژماردن, new_ژماردن, block, &error );
    إذا ( !error && block && new_ژماردن > cur_ژماردن )
      FT_MEM_ZERO( (حرف*)block + cur_ژماردن * item_size,
                   ( new_ژماردن - cur_ژماردن ) * item_size );

    *p_error = error;
    أرجع block;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_qrealloc( FT_Memory  memory,
                   FT_Long    item_size,
                   FT_Long    cur_ژماردن,
                   FT_Long    new_ژماردن,
                   فراغ*      block,
                   FT_Error  *p_error )
  {
    FT_Error  error = FT_Err_Ok;


    /* Note that we now accept `item_size == 0' as a valid parameter, in
     * order to cover very weird cases where an ALLOC_MULT macro would be
     * called.
     */
    إذا ( cur_ژماردن < 0 || new_ژماردن < 0 || item_size < 0 )
    {
      /* may help catch/prevent nasty security issues */
      error = FT_THROW( Invalid_Argument );
    }
    وإلا إذا ( new_ژماردن == 0 || item_size == 0 )
    {
      ft_mem_free( memory, block );
      block = عدم;
    }
    وإلا إذا ( new_ژماردن > FT_INT_MAX / item_size )
    {
      error = FT_THROW( Array_Too_Large );
    }
    وإلا إذا ( cur_ژماردن == 0 )
    {
      FT_ASSERT( !block );

      block = memory->alloc( memory, new_ژماردن * item_size );
      إذا ( block == عدم )
        error = FT_THROW( Out_Of_Memory );
    }
    وإلا
    {
      FT_Pointer  block2;
      FT_Long     cur_size = cur_ژماردن * item_size;
      FT_Long     new_size = new_ژماردن * item_size;


      block2 = memory->realloc( memory, cur_size, new_size, block );
      إذا ( !block2 )
        error = FT_THROW( Out_Of_Memory );
      وإلا
        block = block2;
    }

    *p_error = error;
    أرجع block;
  }


  FT_BASE_DEF( فراغ )
  ft_mem_free( FT_Memory   memory,
               ثابت فراغ *P )
  {
    إذا ( P )
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


    إذا ( !error && address && size > 0 )
      ft_memcpy( p, address, size );

    *p_error = error;
    أرجع p;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_strdup( FT_Memory    memory,
                 ثابت حرف*  str,
                 FT_Error    *p_error )
  {
    FT_ULong  len = str ? (FT_ULong)ft_strlen( str ) + 1
                        : 0;


    أرجع ft_mem_dup( memory, str, len, p_error );
  }


  FT_BASE_DEF( FT_Int )
  ft_mem_strcpyn( حرف*        dst,
                  ثابت حرف*  src,
                  FT_ULong     size )
  {
    طالما ( size > 1 && *src != 0 )
    {
      *dst++ = *src++;
      size--;
    }

    *dst = 0;  /* always zero-terminate */

    أرجع *src != 0;
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

  FT_EXPORT_DEF( FT_List_گرێ )
  FT_List_Find( FT_List  list,
                فراغ*    data )
  {
    FT_List_گرێ  cur;


    إذا ( !list )
      أرجع عدم;

    cur = list->head;
    طالما ( cur )
    {
      إذا ( cur->data == data )
        أرجع cur;

      cur = cur->next;
    }

    أرجع عدم;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( فراغ )
  FT_List_Add( FT_List      list,
               FT_List_گرێ  گرێ )
  {
    FT_List_گرێ  before;


    إذا ( !list || !گرێ )
      أرجع;

    before = list->tail;

    گرێ->next = عدم;
    گرێ->prev = before;

    إذا ( before )
      before->next = گرێ;
    وإلا
      list->head = گرێ;

    list->tail = گرێ;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( فراغ )
  FT_List_زیادکردن( FT_List      list,
                  FT_List_گرێ  گرێ )
  {
    FT_List_گرێ  after;


    إذا ( !list || !گرێ )
      أرجع;

    after = list->head;

    گرێ->next = after;
    گرێ->prev = عدم;

    إذا ( !after )
      list->tail = گرێ;
    وإلا
      after->prev = گرێ;

    list->head = گرێ;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( فراغ )
  FT_List_Remove( FT_List      list,
                  FT_List_گرێ  گرێ )
  {
    FT_List_گرێ  before, after;


    إذا ( !list || !گرێ )
      أرجع;

    before = گرێ->prev;
    after  = گرێ->next;

    إذا ( before )
      before->next = after;
    وإلا
      list->head = after;

    إذا ( after )
      after->prev = before;
    وإلا
      list->tail = before;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( فراغ )
  FT_List_Up( FT_List      list,
              FT_List_گرێ  گرێ )
  {
    FT_List_گرێ  before, after;


    إذا ( !list || !گرێ )
      أرجع;

    before = گرێ->prev;
    after  = گرێ->next;

    /* check whether we are already on top of the list */
    إذا ( !before )
      أرجع;

    before->next = after;

    إذا ( after )
      after->prev = before;
    وإلا
      list->tail = before;

    گرێ->prev       = عدم;
    گرێ->next       = list->head;
    list->head->prev = گرێ;
    list->head       = گرێ;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( FT_Error )
  FT_List_Iterate( FT_List           list,
                   FT_List_Iterator  iterator,
                   فراغ*             user )
  {
    FT_List_گرێ  cur;
    FT_Error     error = FT_Err_Ok;


    إذا ( !list || !iterator )
      أرجع FT_THROW( Invalid_Argument );

    cur = list->head;

    طالما ( cur )
    {
      FT_List_گرێ  next = cur->next;


      error = iterator( cur, user );
      إذا ( error )
        اكسر;

      cur = next;
    }

    أرجع error;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( فراغ )
  FT_List_Finalize( FT_List             list,
                    FT_List_Destructor  destroy,
                    FT_Memory           memory,
                    فراغ*               user )
  {
    FT_List_گرێ  cur;


    إذا ( !list || !memory )
      أرجع;

    cur = list->head;
    طالما ( cur )
    {
      FT_List_گرێ  next = cur->next;


      إذا ( destroy )
        destroy( memory, cur->data, user );

      FT_FREE( cur );
      cur = next;
    }

    list->head = عدم;
    list->tail = عدم;
  }


/* END */
