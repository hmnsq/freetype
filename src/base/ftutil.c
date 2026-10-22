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


#അന്തര്ഭാവ <freetype/internal/ftdebug.h>
#അന്തര്ഭാവ <freetype/internal/ftmemory.h>
#അന്തര്ഭാവ <freetype/internal/ftobjs.h>
#അന്തര്ഭാവ <freetype/ftlist.h>


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

    എങ്കിൽ ( !error && block && size > 0 )
      FT_MEM_ZERO( block, size );

    *p_error = error;
    തിരികെ block;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_qalloc( FT_Memory  memory,
                 FT_Long    size,
                 FT_Error  *p_error )
  {
    FT_Error    error = FT_Err_Ok;
    FT_Pointer  block = ശൂന്യ;


    എങ്കിൽ ( size > 0 )
    {
      block = memory->alloc( memory, size );
      എങ്കിൽ ( !block )
        error = FT_THROW( Out_Of_Memory );
    }
    അല്ലെങ്കിൽ എങ്കിൽ ( size < 0 )
    {
      /* may help catch/prevent security issues */
      error = FT_THROW( Invalid_Argument );
    }

    *p_error = error;
    തിരികെ block;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_realloc( FT_Memory  memory,
                  FT_Long    item_size,
                  FT_Long    cur_എണ്ണം,
                  FT_Long    new_എണ്ണം,
                  ശൂന്യം*      block,
                  FT_Error  *p_error )
  {
    FT_Error  error = FT_Err_Ok;


    block = ft_mem_qrealloc( memory, item_size,
                             cur_എണ്ണം, new_എണ്ണം, block, &error );
    എങ്കിൽ ( !error && block && new_എണ്ണം > cur_എണ്ണം )
      FT_MEM_ZERO( (അക്ഷരം*)block + cur_എണ്ണം * item_size,
                   ( new_എണ്ണം - cur_എണ്ണം ) * item_size );

    *p_error = error;
    തിരികെ block;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_qrealloc( FT_Memory  memory,
                   FT_Long    item_size,
                   FT_Long    cur_എണ്ണം,
                   FT_Long    new_എണ്ണം,
                   ശൂന്യം*      block,
                   FT_Error  *p_error )
  {
    FT_Error  error = FT_Err_Ok;


    /* Note that we now accept `item_size == 0' as a valid parameter, in
     * order to cover very weird cases where an ALLOC_MULT macro would be
     * called.
     */
    എങ്കിൽ ( cur_എണ്ണം < 0 || new_എണ്ണം < 0 || item_size < 0 )
    {
      /* may help catch/prevent nasty security issues */
      error = FT_THROW( Invalid_Argument );
    }
    അല്ലെങ്കിൽ എങ്കിൽ ( new_എണ്ണം == 0 || item_size == 0 )
    {
      ft_mem_free( memory, block );
      block = ശൂന്യ;
    }
    അല്ലെങ്കിൽ എങ്കിൽ ( new_എണ്ണം > FT_INT_MAX / item_size )
    {
      error = FT_THROW( Array_Too_Large );
    }
    അല്ലെങ്കിൽ എങ്കിൽ ( cur_എണ്ണം == 0 )
    {
      FT_ASSERT( !block );

      block = memory->alloc( memory, new_എണ്ണം * item_size );
      എങ്കിൽ ( block == ശൂന്യ )
        error = FT_THROW( Out_Of_Memory );
    }
    അല്ലെങ്കിൽ
    {
      FT_Pointer  block2;
      FT_Long     cur_size = cur_എണ്ണം * item_size;
      FT_Long     new_size = new_എണ്ണം * item_size;


      block2 = memory->realloc( memory, cur_size, new_size, block );
      എങ്കിൽ ( !block2 )
        error = FT_THROW( Out_Of_Memory );
      അല്ലെങ്കിൽ
        block = block2;
    }

    *p_error = error;
    തിരികെ block;
  }


  FT_BASE_DEF( ശൂന്യം )
  ft_mem_free( FT_Memory   memory,
               സ്ഥിരം ശൂന്യം *P )
  {
    എങ്കിൽ ( P )
      memory->സ്വതന്ത്രം( memory, (ശൂന്യം*)P );
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_dup( FT_Memory    memory,
              സ്ഥിരം ശൂന്യം*  address,
              FT_ULong     size,
              FT_Error    *p_error )
  {
    FT_Error    error;
    FT_Pointer  p = ft_mem_qalloc( memory, (FT_Long)size, &error );


    എങ്കിൽ ( !error && address && size > 0 )
      ft_memcpy( p, address, size );

    *p_error = error;
    തിരികെ p;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_strdup( FT_Memory    memory,
                 സ്ഥിരം അക്ഷരം*  str,
                 FT_Error    *p_error )
  {
    FT_ULong  len = str ? (FT_ULong)ft_strlen( str ) + 1
                        : 0;


    തിരികെ ft_mem_dup( memory, str, len, p_error );
  }


  FT_BASE_DEF( FT_Int )
  ft_mem_strcpyn( അക്ഷരം*        dst,
                  സ്ഥിരം അക്ഷരം*  src,
                  FT_ULong     size )
  {
    ആകുമ്പോൾ ( size > 1 && *src != 0 )
    {
      *dst++ = *src++;
      size--;
    }

    *dst = 0;  /* always zero-terminate */

    തിരികെ *src != 0;
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

  FT_EXPORT_DEF( FT_List_കണ്ണി )
  FT_List_Find( FT_List  list,
                ശൂന്യം*    data )
  {
    FT_List_കണ്ണി  cur;


    എങ്കിൽ ( !list )
      തിരികെ ശൂന്യ;

    cur = list->head;
    ആകുമ്പോൾ ( cur )
    {
      എങ്കിൽ ( cur->data == data )
        തിരികെ cur;

      cur = cur->next;
    }

    തിരികെ ശൂന്യ;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( ശൂന്യം )
  FT_List_Add( FT_List      list,
               FT_List_കണ്ണി  കണ്ണി )
  {
    FT_List_കണ്ണി  before;


    എങ്കിൽ ( !list || !കണ്ണി )
      തിരികെ;

    before = list->tail;

    കണ്ണി->next = ശൂന്യ;
    കണ്ണി->prev = before;

    എങ്കിൽ ( before )
      before->next = കണ്ണി;
    അല്ലെങ്കിൽ
      list->head = കണ്ണി;

    list->tail = കണ്ണി;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( ശൂന്യം )
  FT_List_തിരുകുക( FT_List      list,
                  FT_List_കണ്ണി  കണ്ണി )
  {
    FT_List_കണ്ണി  after;


    എങ്കിൽ ( !list || !കണ്ണി )
      തിരികെ;

    after = list->head;

    കണ്ണി->next = after;
    കണ്ണി->prev = ശൂന്യ;

    എങ്കിൽ ( !after )
      list->tail = കണ്ണി;
    അല്ലെങ്കിൽ
      after->prev = കണ്ണി;

    list->head = കണ്ണി;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( ശൂന്യം )
  FT_List_Remove( FT_List      list,
                  FT_List_കണ്ണി  കണ്ണി )
  {
    FT_List_കണ്ണി  before, after;


    എങ്കിൽ ( !list || !കണ്ണി )
      തിരികെ;

    before = കണ്ണി->prev;
    after  = കണ്ണി->next;

    എങ്കിൽ ( before )
      before->next = after;
    അല്ലെങ്കിൽ
      list->head = after;

    എങ്കിൽ ( after )
      after->prev = before;
    അല്ലെങ്കിൽ
      list->tail = before;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( ശൂന്യം )
  FT_List_Up( FT_List      list,
              FT_List_കണ്ണി  കണ്ണി )
  {
    FT_List_കണ്ണി  before, after;


    എങ്കിൽ ( !list || !കണ്ണി )
      തിരികെ;

    before = കണ്ണി->prev;
    after  = കണ്ണി->next;

    /* check whether we are already on top of the list */
    എങ്കിൽ ( !before )
      തിരികെ;

    before->next = after;

    എങ്കിൽ ( after )
      after->prev = before;
    അല്ലെങ്കിൽ
      list->tail = before;

    കണ്ണി->prev       = ശൂന്യ;
    കണ്ണി->next       = list->head;
    list->head->prev = കണ്ണി;
    list->head       = കണ്ണി;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( FT_Error )
  FT_List_Iterate( FT_List           list,
                   FT_List_Iterator  iterator,
                   ശൂന്യം*             user )
  {
    FT_List_കണ്ണി  cur;
    FT_Error     error = FT_Err_Ok;


    എങ്കിൽ ( !list || !iterator )
      തിരികെ FT_THROW( Invalid_Argument );

    cur = list->head;

    ആകുമ്പോൾ ( cur )
    {
      FT_List_കണ്ണി  next = cur->next;


      error = iterator( cur, user );
      എങ്കിൽ ( error )
        നിർത്തുക;

      cur = next;
    }

    തിരികെ error;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( ശൂന്യം )
  FT_List_Finalize( FT_List             list,
                    FT_List_Destructor  destroy,
                    FT_Memory           memory,
                    ശൂന്യം*               user )
  {
    FT_List_കണ്ണി  cur;


    എങ്കിൽ ( !list || !memory )
      തിരികെ;

    cur = list->head;
    ആകുമ്പോൾ ( cur )
    {
      FT_List_കണ്ണി  next = cur->next;


      എങ്കിൽ ( destroy )
        destroy( memory, cur->data, user );

      FT_FREE( cur );
      cur = next;
    }

    list->head = ശൂന്യ;
    list->tail = ശൂന്യ;
  }


/* END */
