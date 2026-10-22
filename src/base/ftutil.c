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
    إرجاع block;
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
    إرجاع block;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_realloc( FT_Memory  memory,
                  FT_Long    item_size,
                  FT_Long    cur_تعداد,
                  FT_Long    new_تعداد,
                  فراغ*      block,
                  FT_Error  *p_error )
  {
    FT_Error  error = FT_Err_Ok;


    block = ft_mem_qrealloc( memory, item_size,
                             cur_تعداد, new_تعداد, block, &error );
    إذا ( !error && block && new_تعداد > cur_تعداد )
      FT_MEM_ZERO( (حرف*)block + cur_تعداد * item_size,
                   ( new_تعداد - cur_تعداد ) * item_size );

    *p_error = error;
    إرجاع block;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_qrealloc( FT_Memory  memory,
                   FT_Long    item_size,
                   FT_Long    cur_تعداد,
                   FT_Long    new_تعداد,
                   فراغ*      block,
                   FT_Error  *p_error )
  {
    FT_Error  error = FT_Err_Ok;


    /* Note that we now accept `item_size == 0' as a valid parameter, in
     * order to cover very weird cases where an ALLOC_MULT macro would be
     * called.
     */
    إذا ( cur_تعداد < 0 || new_تعداد < 0 || item_size < 0 )
    {
      /* may help catch/prevent nasty security issues */
      error = FT_THROW( Invalid_Argument );
    }
    وإلا إذا ( new_تعداد == 0 || item_size == 0 )
    {
      ft_mem_free( memory, block );
      block = عدم;
    }
    وإلا إذا ( new_تعداد > FT_INT_MAX / item_size )
    {
      error = FT_THROW( Array_Too_Large );
    }
    وإلا إذا ( cur_تعداد == 0 )
    {
      FT_ASSERT( !block );

      block = memory->alloc( memory, new_تعداد * item_size );
      إذا ( block == عدم )
        error = FT_THROW( Out_Of_Memory );
    }
    وإلا
    {
      FT_Pointer  block2;
      FT_Long     cur_size = cur_تعداد * item_size;
      FT_Long     new_size = new_تعداد * item_size;


      block2 = memory->realloc( memory, cur_size, new_size, block );
      إذا ( !block2 )
        error = FT_THROW( Out_Of_Memory );
      وإلا
        block = block2;
    }

    *p_error = error;
    إرجاع block;
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
    إرجاع p;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_strdup( FT_Memory    memory,
                 ثابت حرف*  str,
                 FT_Error    *p_error )
  {
    FT_ULong  len = str ? (FT_ULong)ft_strlen( str ) + 1
                        : 0;


    إرجاع ft_mem_dup( memory, str, len, p_error );
  }


  FT_BASE_DEF( FT_Int )
  ft_mem_strcpyn( حرف*        dst,
                  ثابت حرف*  src,
                  FT_ULong     size )
  {
    بينما ( size > 1 && *src != 0 )
    {
      *dst++ = *src++;
      size--;
    }

    *dst = 0;  /* always zero-terminate */

    إرجاع *src != 0;
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

  FT_EXPORT_DEF( FT_List_عقدة )
  FT_List_Find( FT_List  list,
                فراغ*    data )
  {
    FT_List_عقدة  cur;


    إذا ( !list )
      إرجاع عدم;

    cur = list->head;
    بينما ( cur )
    {
      إذا ( cur->data == data )
        إرجاع cur;

      cur = cur->next;
    }

    إرجاع عدم;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( فراغ )
  FT_List_Add( FT_List      list,
               FT_List_عقدة  عقدة )
  {
    FT_List_عقدة  before;


    إذا ( !list || !عقدة )
      إرجاع;

    before = list->tail;

    عقدة->next = عدم;
    عقدة->prev = before;

    إذا ( before )
      before->next = عقدة;
    وإلا
      list->head = عقدة;

    list->tail = عقدة;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( فراغ )
  FT_List_أدرج( FT_List      list,
                  FT_List_عقدة  عقدة )
  {
    FT_List_عقدة  after;


    إذا ( !list || !عقدة )
      إرجاع;

    after = list->head;

    عقدة->next = after;
    عقدة->prev = عدم;

    إذا ( !after )
      list->tail = عقدة;
    وإلا
      after->prev = عقدة;

    list->head = عقدة;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( فراغ )
  FT_List_Remove( FT_List      list,
                  FT_List_عقدة  عقدة )
  {
    FT_List_عقدة  before, after;


    إذا ( !list || !عقدة )
      إرجاع;

    before = عقدة->prev;
    after  = عقدة->next;

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
              FT_List_عقدة  عقدة )
  {
    FT_List_عقدة  before, after;


    إذا ( !list || !عقدة )
      إرجاع;

    before = عقدة->prev;
    after  = عقدة->next;

    /* check whether we are already on top of the list */
    إذا ( !before )
      إرجاع;

    before->next = after;

    إذا ( after )
      after->prev = before;
    وإلا
      list->tail = before;

    عقدة->prev       = عدم;
    عقدة->next       = list->head;
    list->head->prev = عقدة;
    list->head       = عقدة;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( FT_Error )
  FT_List_Iterate( FT_List           list,
                   FT_List_Iterator  iterator,
                   فراغ*             user )
  {
    FT_List_عقدة  cur;
    FT_Error     error = FT_Err_Ok;


    إذا ( !list || !iterator )
      إرجاع FT_THROW( Invalid_Argument );

    cur = list->head;

    بينما ( cur )
    {
      FT_List_عقدة  next = cur->next;


      error = iterator( cur, user );
      إذا ( error )
        اكسر;

      cur = next;
    }

    إرجاع error;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( فراغ )
  FT_List_Finalize( FT_List             list,
                    FT_List_Destructor  destroy,
                    FT_Memory           memory,
                    فراغ*               user )
  {
    FT_List_عقدة  cur;


    إذا ( !list || !memory )
      إرجاع;

    cur = list->head;
    بينما ( cur )
    {
      FT_List_عقدة  next = cur->next;


      إذا ( destroy )
        destroy( memory, cur->data, user );

      FT_FREE( cur );
      cur = next;
    }

    list->head = عدم;
    list->tail = عدم;
  }


/* END */
