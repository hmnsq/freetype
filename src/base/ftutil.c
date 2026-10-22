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


#অন্তর্ভাৱ <freetype/internal/ftdebug.h>
#অন্তর্ভাৱ <freetype/internal/ftmemory.h>
#অন্তর্ভাৱ <freetype/internal/ftobjs.h>
#অন্তর্ভাৱ <freetype/ftlist.h>


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

    যদি ( !error && block && size > 0 )
      FT_MEM_ZERO( block, size );

    *p_error = error;
    ঘূৰাই block;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_qalloc( FT_Memory  memory,
                 FT_Long    size,
                 FT_Error  *p_error )
  {
    FT_Error    error = FT_Err_Ok;
    FT_Pointer  block = রিক্ত;


    যদি ( size > 0 )
    {
      block = memory->alloc( memory, size );
      যদি ( !block )
        error = FT_THROW( Out_Of_Memory );
    }
    অন্যথা যদি ( size < 0 )
    {
      /* may help catch/prevent security issues */
      error = FT_THROW( Invalid_Argument );
    }

    *p_error = error;
    ঘূৰাই block;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_realloc( FT_Memory  memory,
                  FT_Long    item_size,
                  FT_Long    cur_গুনতি,
                  FT_Long    new_গুনতি,
                  শূন্য*      block,
                  FT_Error  *p_error )
  {
    FT_Error  error = FT_Err_Ok;


    block = ft_mem_qrealloc( memory, item_size,
                             cur_গুনতি, new_গুনতি, block, &error );
    যদি ( !error && block && new_গুনতি > cur_গুনতি )
      FT_MEM_ZERO( (ৱর্ণ*)block + cur_গুনতি * item_size,
                   ( new_গুনতি - cur_গুনতি ) * item_size );

    *p_error = error;
    ঘূৰাই block;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_qrealloc( FT_Memory  memory,
                   FT_Long    item_size,
                   FT_Long    cur_গুনতি,
                   FT_Long    new_গুনতি,
                   শূন্য*      block,
                   FT_Error  *p_error )
  {
    FT_Error  error = FT_Err_Ok;


    /* Note that we now accept `item_size == 0' as a valid parameter, in
     * order to cover very weird cases where an ALLOC_MULT macro would be
     * called.
     */
    যদি ( cur_গুনতি < 0 || new_গুনতি < 0 || item_size < 0 )
    {
      /* may help catch/prevent nasty security issues */
      error = FT_THROW( Invalid_Argument );
    }
    অন্যথা যদি ( new_গুনতি == 0 || item_size == 0 )
    {
      ft_mem_free( memory, block );
      block = রিক্ত;
    }
    অন্যথা যদি ( new_গুনতি > FT_INT_MAX / item_size )
    {
      error = FT_THROW( Array_Too_Large );
    }
    অন্যথা যদি ( cur_গুনতি == 0 )
    {
      FT_ASSERT( !block );

      block = memory->alloc( memory, new_গুনতি * item_size );
      যদি ( block == রিক্ত )
        error = FT_THROW( Out_Of_Memory );
    }
    অন্যথা
    {
      FT_Pointer  block2;
      FT_Long     cur_size = cur_গুনতি * item_size;
      FT_Long     new_size = new_গুনতি * item_size;


      block2 = memory->realloc( memory, cur_size, new_size, block );
      যদি ( !block2 )
        error = FT_THROW( Out_Of_Memory );
      অন্যথা
        block = block2;
    }

    *p_error = error;
    ঘূৰাই block;
  }


  FT_BASE_DEF( শূন্য )
  ft_mem_free( FT_Memory   memory,
               ধ্রুৱ শূন্য *P )
  {
    যদি ( P )
      memory->মুক্ত( memory, (শূন্য*)P );
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_dup( FT_Memory    memory,
              ধ্রুৱ শূন্য*  address,
              FT_ULong     size,
              FT_Error    *p_error )
  {
    FT_Error    error;
    FT_Pointer  p = ft_mem_qalloc( memory, (FT_Long)size, &error );


    যদি ( !error && address && size > 0 )
      ft_memcpy( p, address, size );

    *p_error = error;
    ঘূৰাই p;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_strdup( FT_Memory    memory,
                 ধ্রুৱ ৱর্ণ*  str,
                 FT_Error    *p_error )
  {
    FT_ULong  len = str ? (FT_ULong)ft_strlen( str ) + 1
                        : 0;


    ঘূৰাই ft_mem_dup( memory, str, len, p_error );
  }


  FT_BASE_DEF( FT_Int )
  ft_mem_strcpyn( ৱর্ণ*        dst,
                  ধ্রুৱ ৱর্ণ*  src,
                  FT_ULong     size )
  {
    যেতিয়ালৈ ( size > 1 && *src != 0 )
    {
      *dst++ = *src++;
      size--;
    }

    *dst = 0;  /* always zero-terminate */

    ঘূৰাই *src != 0;
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

  FT_EXPORT_DEF( FT_List_গ্ৰন্থি )
  FT_List_Find( FT_List  list,
                শূন্য*    data )
  {
    FT_List_গ্ৰন্থি  cur;


    যদি ( !list )
      ঘূৰাই রিক্ত;

    cur = list->head;
    যেতিয়ালৈ ( cur )
    {
      যদি ( cur->data == data )
        ঘূৰাই cur;

      cur = cur->next;
    }

    ঘূৰাই রিক্ত;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( শূন্য )
  FT_List_Add( FT_List      list,
               FT_List_গ্ৰন্থি  গ্ৰন্থি )
  {
    FT_List_গ্ৰন্থি  before;


    যদি ( !list || !গ্ৰন্থি )
      ঘূৰাই;

    before = list->tail;

    গ্ৰন্থি->next = রিক্ত;
    গ্ৰন্থি->prev = before;

    যদি ( before )
      before->next = গ্ৰন্থি;
    অন্যথা
      list->head = গ্ৰন্থি;

    list->tail = গ্ৰন্থি;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( শূন্য )
  FT_List_ঢোকাও( FT_List      list,
                  FT_List_গ্ৰন্থি  গ্ৰন্থি )
  {
    FT_List_গ্ৰন্থি  after;


    যদি ( !list || !গ্ৰন্থি )
      ঘূৰাই;

    after = list->head;

    গ্ৰন্থি->next = after;
    গ্ৰন্থি->prev = রিক্ত;

    যদি ( !after )
      list->tail = গ্ৰন্থি;
    অন্যথা
      after->prev = গ্ৰন্থি;

    list->head = গ্ৰন্থি;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( শূন্য )
  FT_List_Remove( FT_List      list,
                  FT_List_গ্ৰন্থি  গ্ৰন্থি )
  {
    FT_List_গ্ৰন্থি  before, after;


    যদি ( !list || !গ্ৰন্থি )
      ঘূৰাই;

    before = গ্ৰন্থি->prev;
    after  = গ্ৰন্থি->next;

    যদি ( before )
      before->next = after;
    অন্যথা
      list->head = after;

    যদি ( after )
      after->prev = before;
    অন্যথা
      list->tail = before;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( শূন্য )
  FT_List_Up( FT_List      list,
              FT_List_গ্ৰন্থি  গ্ৰন্থি )
  {
    FT_List_গ্ৰন্থি  before, after;


    যদি ( !list || !গ্ৰন্থি )
      ঘূৰাই;

    before = গ্ৰন্থি->prev;
    after  = গ্ৰন্থি->next;

    /* check whether we are already on top of the list */
    যদি ( !before )
      ঘূৰাই;

    before->next = after;

    যদি ( after )
      after->prev = before;
    অন্যথা
      list->tail = before;

    গ্ৰন্থি->prev       = রিক্ত;
    গ্ৰন্থি->next       = list->head;
    list->head->prev = গ্ৰন্থি;
    list->head       = গ্ৰন্থি;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( FT_Error )
  FT_List_Iterate( FT_List           list,
                   FT_List_Iterator  iterator,
                   শূন্য*             user )
  {
    FT_List_গ্ৰন্থি  cur;
    FT_Error     error = FT_Err_Ok;


    যদি ( !list || !iterator )
      ঘূৰাই FT_THROW( Invalid_Argument );

    cur = list->head;

    যেতিয়ালৈ ( cur )
    {
      FT_List_গ্ৰন্থি  next = cur->next;


      error = iterator( cur, user );
      যদি ( error )
        ভঙ্গ;

      cur = next;
    }

    ঘূৰাই error;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( শূন্য )
  FT_List_Finalize( FT_List             list,
                    FT_List_Destructor  destroy,
                    FT_Memory           memory,
                    শূন্য*               user )
  {
    FT_List_গ্ৰন্থি  cur;


    যদি ( !list || !memory )
      ঘূৰাই;

    cur = list->head;
    যেতিয়ালৈ ( cur )
    {
      FT_List_গ্ৰন্থি  next = cur->next;


      যদি ( destroy )
        destroy( memory, cur->data, user );

      FT_FREE( cur );
      cur = next;
    }

    list->head = রিক্ত;
    list->tail = রিক্ত;
  }


/* END */
