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


#அந்தர்பாவ <freetype/internal/ftdebug.h>
#அந்தர்பாவ <freetype/internal/ftmemory.h>
#அந்தர்பாவ <freetype/internal/ftobjs.h>
#அந்தர்பாவ <freetype/ftlist.h>


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

    எனில் ( !error && block && size > 0 )
      FT_MEM_ZERO( block, size );

    *p_error = error;
    திரும்பு block;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_qalloc( FT_Memory  memory,
                 FT_Long    size,
                 FT_Error  *p_error )
  {
    FT_Error    error = FT_Err_Ok;
    FT_Pointer  block = வெற்று;


    எனில் ( size > 0 )
    {
      block = memory->alloc( memory, size );
      எனில் ( !block )
        error = FT_THROW( Out_Of_Memory );
    }
    இல்லையெனில் எனில் ( size < 0 )
    {
      /* may help catch/prevent security issues */
      error = FT_THROW( Invalid_Argument );
    }

    *p_error = error;
    திரும்பு block;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_realloc( FT_Memory  memory,
                  FT_Long    item_size,
                  FT_Long    cur_மொத்தம்,
                  FT_Long    new_மொத்தம்,
                  வெறுமை*      block,
                  FT_Error  *p_error )
  {
    FT_Error  error = FT_Err_Ok;


    block = ft_mem_qrealloc( memory, item_size,
                             cur_மொத்தம், new_மொத்தம், block, &error );
    எனில் ( !error && block && new_மொத்தம் > cur_மொத்தம் )
      FT_MEM_ZERO( (எழுத்து*)block + cur_மொத்தம் * item_size,
                   ( new_மொத்தம் - cur_மொத்தம் ) * item_size );

    *p_error = error;
    திரும்பு block;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_qrealloc( FT_Memory  memory,
                   FT_Long    item_size,
                   FT_Long    cur_மொத்தம்,
                   FT_Long    new_மொத்தம்,
                   வெறுமை*      block,
                   FT_Error  *p_error )
  {
    FT_Error  error = FT_Err_Ok;


    /* Note that we now accept `item_size == 0' as a valid parameter, in
     * order to cover very weird cases where an ALLOC_MULT macro would be
     * called.
     */
    எனில் ( cur_மொத்தம் < 0 || new_மொத்தம் < 0 || item_size < 0 )
    {
      /* may help catch/prevent nasty security issues */
      error = FT_THROW( Invalid_Argument );
    }
    இல்லையெனில் எனில் ( new_மொத்தம் == 0 || item_size == 0 )
    {
      ft_mem_free( memory, block );
      block = வெற்று;
    }
    இல்லையெனில் எனில் ( new_மொத்தம் > FT_INT_MAX / item_size )
    {
      error = FT_THROW( Array_Too_Large );
    }
    இல்லையெனில் எனில் ( cur_மொத்தம் == 0 )
    {
      FT_ASSERT( !block );

      block = memory->alloc( memory, new_மொத்தம் * item_size );
      எனில் ( block == வெற்று )
        error = FT_THROW( Out_Of_Memory );
    }
    இல்லையெனில்
    {
      FT_Pointer  block2;
      FT_Long     cur_size = cur_மொத்தம் * item_size;
      FT_Long     new_size = new_மொத்தம் * item_size;


      block2 = memory->realloc( memory, cur_size, new_size, block );
      எனில் ( !block2 )
        error = FT_THROW( Out_Of_Memory );
      இல்லையெனில்
        block = block2;
    }

    *p_error = error;
    திரும்பு block;
  }


  FT_BASE_DEF( வெறுமை )
  ft_mem_free( FT_Memory   memory,
               மாறிலி வெறுமை *P )
  {
    எனில் ( P )
      memory->விடுவி( memory, (வெறுமை*)P );
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_dup( FT_Memory    memory,
              மாறிலி வெறுமை*  address,
              FT_ULong     size,
              FT_Error    *p_error )
  {
    FT_Error    error;
    FT_Pointer  p = ft_mem_qalloc( memory, (FT_Long)size, &error );


    எனில் ( !error && address && size > 0 )
      ft_memcpy( p, address, size );

    *p_error = error;
    திரும்பு p;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_strdup( FT_Memory    memory,
                 மாறிலி எழுத்து*  str,
                 FT_Error    *p_error )
  {
    FT_ULong  len = str ? (FT_ULong)ft_strlen( str ) + 1
                        : 0;


    திரும்பு ft_mem_dup( memory, str, len, p_error );
  }


  FT_BASE_DEF( FT_Int )
  ft_mem_strcpyn( எழுத்து*        dst,
                  மாறிலி எழுத்து*  src,
                  FT_ULong     size )
  {
    போது ( size > 1 && *src != 0 )
    {
      *dst++ = *src++;
      size--;
    }

    *dst = 0;  /* always zero-terminate */

    திரும்பு *src != 0;
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

  FT_EXPORT_DEF( FT_List_முனை )
  FT_List_Find( FT_List  list,
                வெறுமை*    data )
  {
    FT_List_முனை  cur;


    எனில் ( !list )
      திரும்பு வெற்று;

    cur = list->head;
    போது ( cur )
    {
      எனில் ( cur->data == data )
        திரும்பு cur;

      cur = cur->next;
    }

    திரும்பு வெற்று;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( வெறுமை )
  FT_List_Add( FT_List      list,
               FT_List_முனை  முனை )
  {
    FT_List_முனை  before;


    எனில் ( !list || !முனை )
      திரும்பு;

    before = list->tail;

    முனை->next = வெற்று;
    முனை->prev = before;

    எனில் ( before )
      before->next = முனை;
    இல்லையெனில்
      list->head = முனை;

    list->tail = முனை;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( வெறுமை )
  FT_List_செருகு( FT_List      list,
                  FT_List_முனை  முனை )
  {
    FT_List_முனை  after;


    எனில் ( !list || !முனை )
      திரும்பு;

    after = list->head;

    முனை->next = after;
    முனை->prev = வெற்று;

    எனில் ( !after )
      list->tail = முனை;
    இல்லையெனில்
      after->prev = முனை;

    list->head = முனை;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( வெறுமை )
  FT_List_Remove( FT_List      list,
                  FT_List_முனை  முனை )
  {
    FT_List_முனை  before, after;


    எனில் ( !list || !முனை )
      திரும்பு;

    before = முனை->prev;
    after  = முனை->next;

    எனில் ( before )
      before->next = after;
    இல்லையெனில்
      list->head = after;

    எனில் ( after )
      after->prev = before;
    இல்லையெனில்
      list->tail = before;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( வெறுமை )
  FT_List_Up( FT_List      list,
              FT_List_முனை  முனை )
  {
    FT_List_முனை  before, after;


    எனில் ( !list || !முனை )
      திரும்பு;

    before = முனை->prev;
    after  = முனை->next;

    /* check whether we are already on top of the list */
    எனில் ( !before )
      திரும்பு;

    before->next = after;

    எனில் ( after )
      after->prev = before;
    இல்லையெனில்
      list->tail = before;

    முனை->prev       = வெற்று;
    முனை->next       = list->head;
    list->head->prev = முனை;
    list->head       = முனை;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( FT_Error )
  FT_List_Iterate( FT_List           list,
                   FT_List_Iterator  iterator,
                   வெறுமை*             user )
  {
    FT_List_முனை  cur;
    FT_Error     error = FT_Err_Ok;


    எனில் ( !list || !iterator )
      திரும்பு FT_THROW( Invalid_Argument );

    cur = list->head;

    போது ( cur )
    {
      FT_List_முனை  next = cur->next;


      error = iterator( cur, user );
      எனில் ( error )
        நிறுத்து;

      cur = next;
    }

    திரும்பு error;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( வெறுமை )
  FT_List_Finalize( FT_List             list,
                    FT_List_Destructor  destroy,
                    FT_Memory           memory,
                    வெறுமை*               user )
  {
    FT_List_முனை  cur;


    எனில் ( !list || !memory )
      திரும்பு;

    cur = list->head;
    போது ( cur )
    {
      FT_List_முனை  next = cur->next;


      எனில் ( destroy )
        destroy( memory, cur->data, user );

      FT_FREE( cur );
      cur = next;
    }

    list->head = வெற்று;
    list->tail = வெற்று;
  }


/* END */
