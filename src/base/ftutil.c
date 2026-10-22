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


#ଅନ୍ତର୍ଭାଵ <freetype/internal/ftdebug.h>
#ଅନ୍ତର୍ଭାଵ <freetype/internal/ftmemory.h>
#ଅନ୍ତର୍ଭାଵ <freetype/internal/ftobjs.h>
#ଅନ୍ତର୍ଭାଵ <freetype/ftlist.h>


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

    ଯଦି ( !error && block && size > 0 )
      FT_MEM_ZERO( block, size );

    *p_error = error;
    ଫେରନ୍ତୁ block;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_qalloc( FT_Memory  memory,
                 FT_Long    size,
                 FT_Error  *p_error )
  {
    FT_Error    error = FT_Err_Ok;
    FT_Pointer  block = ଖାଲି;


    ଯଦି ( size > 0 )
    {
      block = memory->alloc( memory, size );
      ଯଦି ( !block )
        error = FT_THROW( Out_Of_Memory );
    }
    ଅନ୍ୟଥା ଯଦି ( size < 0 )
    {
      /* may help catch/prevent security issues */
      error = FT_THROW( Invalid_Argument );
    }

    *p_error = error;
    ଫେରନ୍ତୁ block;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_realloc( FT_Memory  memory,
                  FT_Long    item_size,
                  FT_Long    cur_ଗଣନା,
                  FT_Long    new_ଗଣନା,
                  ଶୂନ୍ୟ*      block,
                  FT_Error  *p_error )
  {
    FT_Error  error = FT_Err_Ok;


    block = ft_mem_qrealloc( memory, item_size,
                             cur_ଗଣନା, new_ଗଣନା, block, &error );
    ଯଦି ( !error && block && new_ଗଣନା > cur_ଗଣନା )
      FT_MEM_ZERO( (ଅକ୍ଷର*)block + cur_ଗଣନା * item_size,
                   ( new_ଗଣନା - cur_ଗଣନା ) * item_size );

    *p_error = error;
    ଫେରନ୍ତୁ block;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_qrealloc( FT_Memory  memory,
                   FT_Long    item_size,
                   FT_Long    cur_ଗଣନା,
                   FT_Long    new_ଗଣନା,
                   ଶୂନ୍ୟ*      block,
                   FT_Error  *p_error )
  {
    FT_Error  error = FT_Err_Ok;


    /* Note that we now accept `item_size == 0' as a valid parameter, in
     * order to cover very weird cases where an ALLOC_MULT macro would be
     * called.
     */
    ଯଦି ( cur_ଗଣନା < 0 || new_ଗଣନା < 0 || item_size < 0 )
    {
      /* may help catch/prevent nasty security issues */
      error = FT_THROW( Invalid_Argument );
    }
    ଅନ୍ୟଥା ଯଦି ( new_ଗଣନା == 0 || item_size == 0 )
    {
      ft_mem_free( memory, block );
      block = ଖାଲି;
    }
    ଅନ୍ୟଥା ଯଦି ( new_ଗଣନା > FT_INT_MAX / item_size )
    {
      error = FT_THROW( Array_Too_Large );
    }
    ଅନ୍ୟଥା ଯଦି ( cur_ଗଣନା == 0 )
    {
      FT_ASSERT( !block );

      block = memory->alloc( memory, new_ଗଣନା * item_size );
      ଯଦି ( block == ଖାଲି )
        error = FT_THROW( Out_Of_Memory );
    }
    ଅନ୍ୟଥା
    {
      FT_Pointer  block2;
      FT_Long     cur_size = cur_ଗଣନା * item_size;
      FT_Long     new_size = new_ଗଣନା * item_size;


      block2 = memory->realloc( memory, cur_size, new_size, block );
      ଯଦି ( !block2 )
        error = FT_THROW( Out_Of_Memory );
      ଅନ୍ୟଥା
        block = block2;
    }

    *p_error = error;
    ଫେରନ୍ତୁ block;
  }


  FT_BASE_DEF( ଶୂନ୍ୟ )
  ft_mem_free( FT_Memory   memory,
               ଅଚଳ ଶୂନ୍ୟ *P )
  {
    ଯଦି ( P )
      memory->ମୁକ୍ତ( memory, (ଶୂନ୍ୟ*)P );
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_dup( FT_Memory    memory,
              ଅଚଳ ଶୂନ୍ୟ*  address,
              FT_ULong     size,
              FT_Error    *p_error )
  {
    FT_Error    error;
    FT_Pointer  p = ft_mem_qalloc( memory, (FT_Long)size, &error );


    ଯଦି ( !error && address && size > 0 )
      ft_memcpy( p, address, size );

    *p_error = error;
    ଫେରନ୍ତୁ p;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_strdup( FT_Memory    memory,
                 ଅଚଳ ଅକ୍ଷର*  str,
                 FT_Error    *p_error )
  {
    FT_ULong  len = str ? (FT_ULong)ft_strlen( str ) + 1
                        : 0;


    ଫେରନ୍ତୁ ft_mem_dup( memory, str, len, p_error );
  }


  FT_BASE_DEF( FT_Int )
  ft_mem_strcpyn( ଅକ୍ଷର*        dst,
                  ଅଚଳ ଅକ୍ଷର*  src,
                  FT_ULong     size )
  {
    ଯେତେବେଳେ ( size > 1 && *src != 0 )
    {
      *dst++ = *src++;
      size--;
    }

    *dst = 0;  /* always zero-terminate */

    ଫେରନ୍ତୁ *src != 0;
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

  FT_EXPORT_DEF( FT_List_ଗଣ୍ଠି )
  FT_List_Find( FT_List  list,
                ଶୂନ୍ୟ*    data )
  {
    FT_List_ଗଣ୍ଠି  cur;


    ଯଦି ( !list )
      ଫେରନ୍ତୁ ଖାଲି;

    cur = list->head;
    ଯେତେବେଳେ ( cur )
    {
      ଯଦି ( cur->data == data )
        ଫେରନ୍ତୁ cur;

      cur = cur->next;
    }

    ଫେରନ୍ତୁ ଖାଲି;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( ଶୂନ୍ୟ )
  FT_List_Add( FT_List      list,
               FT_List_ଗଣ୍ଠି  ଗଣ୍ଠି )
  {
    FT_List_ଗଣ୍ଠି  before;


    ଯଦି ( !list || !ଗଣ୍ଠି )
      ଫେରନ୍ତୁ;

    before = list->tail;

    ଗଣ୍ଠି->next = ଖାଲି;
    ଗଣ୍ଠି->prev = before;

    ଯଦି ( before )
      before->next = ଗଣ୍ଠି;
    ଅନ୍ୟଥା
      list->head = ଗଣ୍ଠି;

    list->tail = ଗଣ୍ଠି;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( ଶୂନ୍ୟ )
  FT_List_ଭର୍ତ୍ତି( FT_List      list,
                  FT_List_ଗଣ୍ଠି  ଗଣ୍ଠି )
  {
    FT_List_ଗଣ୍ଠି  after;


    ଯଦି ( !list || !ଗଣ୍ଠି )
      ଫେରନ୍ତୁ;

    after = list->head;

    ଗଣ୍ଠି->next = after;
    ଗଣ୍ଠି->prev = ଖାଲି;

    ଯଦି ( !after )
      list->tail = ଗଣ୍ଠି;
    ଅନ୍ୟଥା
      after->prev = ଗଣ୍ଠି;

    list->head = ଗଣ୍ଠି;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( ଶୂନ୍ୟ )
  FT_List_Remove( FT_List      list,
                  FT_List_ଗଣ୍ଠି  ଗଣ୍ଠି )
  {
    FT_List_ଗଣ୍ଠି  before, after;


    ଯଦି ( !list || !ଗଣ୍ଠି )
      ଫେରନ୍ତୁ;

    before = ଗଣ୍ଠି->prev;
    after  = ଗଣ୍ଠି->next;

    ଯଦି ( before )
      before->next = after;
    ଅନ୍ୟଥା
      list->head = after;

    ଯଦି ( after )
      after->prev = before;
    ଅନ୍ୟଥା
      list->tail = before;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( ଶୂନ୍ୟ )
  FT_List_Up( FT_List      list,
              FT_List_ଗଣ୍ଠି  ଗଣ୍ଠି )
  {
    FT_List_ଗଣ୍ଠି  before, after;


    ଯଦି ( !list || !ଗଣ୍ଠି )
      ଫେରନ୍ତୁ;

    before = ଗଣ୍ଠି->prev;
    after  = ଗଣ୍ଠି->next;

    /* check whether we are already on top of the list */
    ଯଦି ( !before )
      ଫେରନ୍ତୁ;

    before->next = after;

    ଯଦି ( after )
      after->prev = before;
    ଅନ୍ୟଥା
      list->tail = before;

    ଗଣ୍ଠି->prev       = ଖାଲି;
    ଗଣ୍ଠି->next       = list->head;
    list->head->prev = ଗଣ୍ଠି;
    list->head       = ଗଣ୍ଠି;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( FT_Error )
  FT_List_Iterate( FT_List           list,
                   FT_List_Iterator  iterator,
                   ଶୂନ୍ୟ*             user )
  {
    FT_List_ଗଣ୍ଠି  cur;
    FT_Error     error = FT_Err_Ok;


    ଯଦି ( !list || !iterator )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Argument );

    cur = list->head;

    ଯେତେବେଳେ ( cur )
    {
      FT_List_ଗଣ୍ଠି  next = cur->next;


      error = iterator( cur, user );
      ଯଦି ( error )
        ଭାଙ୍ଗ;

      cur = next;
    }

    ଫେରନ୍ତୁ error;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( ଶୂନ୍ୟ )
  FT_List_Finalize( FT_List             list,
                    FT_List_Destructor  destroy,
                    FT_Memory           memory,
                    ଶୂନ୍ୟ*               user )
  {
    FT_List_ଗଣ୍ଠି  cur;


    ଯଦି ( !list || !memory )
      ଫେରନ୍ତୁ;

    cur = list->head;
    ଯେତେବେଳେ ( cur )
    {
      FT_List_ଗଣ୍ଠି  next = cur->next;


      ଯଦି ( destroy )
        destroy( memory, cur->data, user );

      FT_FREE( cur );
      cur = next;
    }

    list->head = ଖାଲି;
    list->tail = ଖାଲି;
  }


/* END */
