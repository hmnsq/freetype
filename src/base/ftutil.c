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


#અન્તર્ભાવ <freetype/internal/ftdebug.h>
#અન્તર્ભાવ <freetype/internal/ftmemory.h>
#અન્તર્ભાવ <freetype/internal/ftobjs.h>
#અન્તર્ભાવ <freetype/ftlist.h>


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

    જો ( !error && block && size > 0 )
      FT_MEM_ZERO( block, size );

    *p_error = error;
    પાછા block;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_qalloc( FT_Memory  memory,
                 FT_Long    size,
                 FT_Error  *p_error )
  {
    FT_Error    error = FT_Err_Ok;
    FT_Pointer  block = ખાલી;


    જો ( size > 0 )
    {
      block = memory->alloc( memory, size );
      જો ( !block )
        error = FT_THROW( Out_Of_Memory );
    }
    નહીંતર જો ( size < 0 )
    {
      /* may help catch/prevent security issues */
      error = FT_THROW( Invalid_Argument );
    }

    *p_error = error;
    પાછા block;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_realloc( FT_Memory  memory,
                  FT_Long    item_size,
                  FT_Long    cur_ગણતરી,
                  FT_Long    new_ગણતરી,
                  શૂન્ય*      block,
                  FT_Error  *p_error )
  {
    FT_Error  error = FT_Err_Ok;


    block = ft_mem_qrealloc( memory, item_size,
                             cur_ગણતરી, new_ગણતરી, block, &error );
    જો ( !error && block && new_ગણતરી > cur_ગણતરી )
      FT_MEM_ZERO( (અક્ષર*)block + cur_ગણતરી * item_size,
                   ( new_ગણતરી - cur_ગણતરી ) * item_size );

    *p_error = error;
    પાછા block;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_qrealloc( FT_Memory  memory,
                   FT_Long    item_size,
                   FT_Long    cur_ગણતરી,
                   FT_Long    new_ગણતરી,
                   શૂન્ય*      block,
                   FT_Error  *p_error )
  {
    FT_Error  error = FT_Err_Ok;


    /* Note that we now accept `item_size == 0' as a valid parameter, in
     * order to cover very weird cases where an ALLOC_MULT macro would be
     * called.
     */
    જો ( cur_ગણતરી < 0 || new_ગણતરી < 0 || item_size < 0 )
    {
      /* may help catch/prevent nasty security issues */
      error = FT_THROW( Invalid_Argument );
    }
    નહીંતર જો ( new_ગણતરી == 0 || item_size == 0 )
    {
      ft_mem_free( memory, block );
      block = ખાલી;
    }
    નહીંતર જો ( new_ગણતરી > FT_INT_MAX / item_size )
    {
      error = FT_THROW( Array_Too_Large );
    }
    નહીંતર જો ( cur_ગણતરી == 0 )
    {
      FT_ASSERT( !block );

      block = memory->alloc( memory, new_ગણતરી * item_size );
      જો ( block == ખાલી )
        error = FT_THROW( Out_Of_Memory );
    }
    નહીંતર
    {
      FT_Pointer  block2;
      FT_Long     cur_size = cur_ગણતરી * item_size;
      FT_Long     new_size = new_ગણતરી * item_size;


      block2 = memory->realloc( memory, cur_size, new_size, block );
      જો ( !block2 )
        error = FT_THROW( Out_Of_Memory );
      નહીંતર
        block = block2;
    }

    *p_error = error;
    પાછા block;
  }


  FT_BASE_DEF( શૂન્ય )
  ft_mem_free( FT_Memory   memory,
               અચલ શૂન્ય *P )
  {
    જો ( P )
      memory->મુક્ત( memory, (શૂન્ય*)P );
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_dup( FT_Memory    memory,
              અચલ શૂન્ય*  address,
              FT_ULong     size,
              FT_Error    *p_error )
  {
    FT_Error    error;
    FT_Pointer  p = ft_mem_qalloc( memory, (FT_Long)size, &error );


    જો ( !error && address && size > 0 )
      ft_memcpy( p, address, size );

    *p_error = error;
    પાછા p;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_strdup( FT_Memory    memory,
                 અચલ અક્ષર*  str,
                 FT_Error    *p_error )
  {
    FT_ULong  len = str ? (FT_ULong)ft_strlen( str ) + 1
                        : 0;


    પાછા ft_mem_dup( memory, str, len, p_error );
  }


  FT_BASE_DEF( FT_Int )
  ft_mem_strcpyn( અક્ષર*        dst,
                  અચલ અક્ષર*  src,
                  FT_ULong     size )
  {
    જ્યારે ( size > 1 && *src != 0 )
    {
      *dst++ = *src++;
      size--;
    }

    *dst = 0;  /* always zero-terminate */

    પાછા *src != 0;
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

  FT_EXPORT_DEF( FT_List_ગાંઠ )
  FT_List_Find( FT_List  list,
                શૂન્ય*    data )
  {
    FT_List_ગાંઠ  cur;


    જો ( !list )
      પાછા ખાલી;

    cur = list->head;
    જ્યારે ( cur )
    {
      જો ( cur->data == data )
        પાછા cur;

      cur = cur->next;
    }

    પાછા ખાલી;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( શૂન્ય )
  FT_List_Add( FT_List      list,
               FT_List_ગાંઠ  ગાંઠ )
  {
    FT_List_ગાંઠ  before;


    જો ( !list || !ગાંઠ )
      પાછા;

    before = list->tail;

    ગાંઠ->next = ખાલી;
    ગાંઠ->prev = before;

    જો ( before )
      before->next = ગાંઠ;
    નહીંતર
      list->head = ગાંઠ;

    list->tail = ગાંઠ;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( શૂન્ય )
  FT_List_ઉમેરો( FT_List      list,
                  FT_List_ગાંઠ  ગાંઠ )
  {
    FT_List_ગાંઠ  after;


    જો ( !list || !ગાંઠ )
      પાછા;

    after = list->head;

    ગાંઠ->next = after;
    ગાંઠ->prev = ખાલી;

    જો ( !after )
      list->tail = ગાંઠ;
    નહીંતર
      after->prev = ગાંઠ;

    list->head = ગાંઠ;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( શૂન્ય )
  FT_List_Remove( FT_List      list,
                  FT_List_ગાંઠ  ગાંઠ )
  {
    FT_List_ગાંઠ  before, after;


    જો ( !list || !ગાંઠ )
      પાછા;

    before = ગાંઠ->prev;
    after  = ગાંઠ->next;

    જો ( before )
      before->next = after;
    નહીંતર
      list->head = after;

    જો ( after )
      after->prev = before;
    નહીંતર
      list->tail = before;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( શૂન્ય )
  FT_List_Up( FT_List      list,
              FT_List_ગાંઠ  ગાંઠ )
  {
    FT_List_ગાંઠ  before, after;


    જો ( !list || !ગાંઠ )
      પાછા;

    before = ગાંઠ->prev;
    after  = ગાંઠ->next;

    /* check whether we are already on top of the list */
    જો ( !before )
      પાછા;

    before->next = after;

    જો ( after )
      after->prev = before;
    નહીંતર
      list->tail = before;

    ગાંઠ->prev       = ખાલી;
    ગાંઠ->next       = list->head;
    list->head->prev = ગાંઠ;
    list->head       = ગાંઠ;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( FT_Error )
  FT_List_Iterate( FT_List           list,
                   FT_List_Iterator  iterator,
                   શૂન્ય*             user )
  {
    FT_List_ગાંઠ  cur;
    FT_Error     error = FT_Err_Ok;


    જો ( !list || !iterator )
      પાછા FT_THROW( Invalid_Argument );

    cur = list->head;

    જ્યારે ( cur )
    {
      FT_List_ગાંઠ  next = cur->next;


      error = iterator( cur, user );
      જો ( error )
        તોડો;

      cur = next;
    }

    પાછા error;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( શૂન્ય )
  FT_List_Finalize( FT_List             list,
                    FT_List_Destructor  destroy,
                    FT_Memory           memory,
                    શૂન્ય*               user )
  {
    FT_List_ગાંઠ  cur;


    જો ( !list || !memory )
      પાછા;

    cur = list->head;
    જ્યારે ( cur )
    {
      FT_List_ગાંઠ  next = cur->next;


      જો ( destroy )
        destroy( memory, cur->data, user );

      FT_FREE( cur );
      cur = next;
    }

    list->head = ખાલી;
    list->tail = ખાલી;
  }


/* END */
