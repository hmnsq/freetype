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


#अन्तर्भाव <freetype/internal/ftdebug.h>
#अन्तर्भाव <freetype/internal/ftmemory.h>
#अन्तर्भाव <freetype/internal/ftobjs.h>
#अन्तर्भाव <freetype/ftlist.h>


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

    जइ ( !error && block && size > 0 )
      FT_MEM_ZERO( block, size );

    *p_error = error;
    प्रत्यागम block;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_qalloc( FT_Memory  memory,
                 FT_Long    size,
                 FT_Error  *p_error )
  {
    FT_Error    error = FT_Err_Ok;
    FT_Pointer  block = रिक्त;


    जइ ( size > 0 )
    {
      block = memory->alloc( memory, size );
      जइ ( !block )
        error = FT_THROW( Out_Of_Memory );
    }
    अण्णहा जइ ( size < 0 )
    {
      /* may help catch/prevent security issues */
      error = FT_THROW( Invalid_Argument );
    }

    *p_error = error;
    प्रत्यागम block;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_realloc( FT_Memory  memory,
                  FT_Long    item_size,
                  FT_Long    cur_गणनम्,
                  FT_Long    new_गणनम्,
                  शून्य*      block,
                  FT_Error  *p_error )
  {
    FT_Error  error = FT_Err_Ok;


    block = ft_mem_qrealloc( memory, item_size,
                             cur_गणनम्, new_गणनम्, block, &error );
    जइ ( !error && block && new_गणनम् > cur_गणनम् )
      FT_MEM_ZERO( (वर्ण*)block + cur_गणनम् * item_size,
                   ( new_गणनम् - cur_गणनम् ) * item_size );

    *p_error = error;
    प्रत्यागम block;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_qrealloc( FT_Memory  memory,
                   FT_Long    item_size,
                   FT_Long    cur_गणनम्,
                   FT_Long    new_गणनम्,
                   शून्य*      block,
                   FT_Error  *p_error )
  {
    FT_Error  error = FT_Err_Ok;


    /* Note that we now accept `item_size == 0' as a valid parameter, in
     * order to cover very weird cases where an ALLOC_MULT macro would be
     * called.
     */
    जइ ( cur_गणनम् < 0 || new_गणनम् < 0 || item_size < 0 )
    {
      /* may help catch/prevent nasty security issues */
      error = FT_THROW( Invalid_Argument );
    }
    अण्णहा जइ ( new_गणनम् == 0 || item_size == 0 )
    {
      ft_mem_free( memory, block );
      block = रिक्त;
    }
    अण्णहा जइ ( new_गणनम् > FT_INT_MAX / item_size )
    {
      error = FT_THROW( Array_Too_Large );
    }
    अण्णहा जइ ( cur_गणनम् == 0 )
    {
      FT_ASSERT( !block );

      block = memory->alloc( memory, new_गणनम् * item_size );
      जइ ( block == रिक्त )
        error = FT_THROW( Out_Of_Memory );
    }
    अण्णहा
    {
      FT_Pointer  block2;
      FT_Long     cur_size = cur_गणनम् * item_size;
      FT_Long     new_size = new_गणनम् * item_size;


      block2 = memory->realloc( memory, cur_size, new_size, block );
      जइ ( !block2 )
        error = FT_THROW( Out_Of_Memory );
      अण्णहा
        block = block2;
    }

    *p_error = error;
    प्रत्यागम block;
  }


  FT_BASE_DEF( शून्य )
  ft_mem_free( FT_Memory   memory,
               ध्रुव शून्य *P )
  {
    जइ ( P )
      memory->मुक्त( memory, (शून्य*)P );
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_dup( FT_Memory    memory,
              ध्रुव शून्य*  address,
              FT_ULong     size,
              FT_Error    *p_error )
  {
    FT_Error    error;
    FT_Pointer  p = ft_mem_qalloc( memory, (FT_Long)size, &error );


    जइ ( !error && address && size > 0 )
      ft_memcpy( p, address, size );

    *p_error = error;
    प्रत्यागम p;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_strdup( FT_Memory    memory,
                 ध्रुव वर्ण*  str,
                 FT_Error    *p_error )
  {
    FT_ULong  len = str ? (FT_ULong)ft_strlen( str ) + 1
                        : 0;


    प्रत्यागम ft_mem_dup( memory, str, len, p_error );
  }


  FT_BASE_DEF( FT_Int )
  ft_mem_strcpyn( वर्ण*        dst,
                  ध्रुव वर्ण*  src,
                  FT_ULong     size )
  {
    जाव ( size > 1 && *src != 0 )
    {
      *dst++ = *src++;
      size--;
    }

    *dst = 0;  /* always zero-terminate */

    प्रत्यागम *src != 0;
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

  FT_EXPORT_DEF( FT_List_ग्रन्थिः )
  FT_List_Find( FT_List  list,
                शून्य*    data )
  {
    FT_List_ग्रन्थिः  cur;


    जइ ( !list )
      प्रत्यागम रिक्त;

    cur = list->head;
    जाव ( cur )
    {
      जइ ( cur->data == data )
        प्रत्यागम cur;

      cur = cur->next;
    }

    प्रत्यागम रिक्त;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( शून्य )
  FT_List_Add( FT_List      list,
               FT_List_ग्रन्थिः  ग्रन्थिः )
  {
    FT_List_ग्रन्थिः  before;


    जइ ( !list || !ग्रन्थिः )
      प्रत्यागम;

    before = list->tail;

    ग्रन्थिः->next = रिक्त;
    ग्रन्थिः->prev = before;

    जइ ( before )
      before->next = ग्रन्थिः;
    अण्णहा
      list->head = ग्रन्थिः;

    list->tail = ग्रन्थिः;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( शून्य )
  FT_List_निवेशय( FT_List      list,
                  FT_List_ग्रन्थिः  ग्रन्थिः )
  {
    FT_List_ग्रन्थिः  after;


    जइ ( !list || !ग्रन्थिः )
      प्रत्यागम;

    after = list->head;

    ग्रन्थिः->next = after;
    ग्रन्थिः->prev = रिक्त;

    जइ ( !after )
      list->tail = ग्रन्थिः;
    अण्णहा
      after->prev = ग्रन्थिः;

    list->head = ग्रन्थिः;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( शून्य )
  FT_List_Remove( FT_List      list,
                  FT_List_ग्रन्थिः  ग्रन्थिः )
  {
    FT_List_ग्रन्थिः  before, after;


    जइ ( !list || !ग्रन्थिः )
      प्रत्यागम;

    before = ग्रन्थिः->prev;
    after  = ग्रन्थिः->next;

    जइ ( before )
      before->next = after;
    अण्णहा
      list->head = after;

    जइ ( after )
      after->prev = before;
    अण्णहा
      list->tail = before;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( शून्य )
  FT_List_Up( FT_List      list,
              FT_List_ग्रन्थिः  ग्रन्थिः )
  {
    FT_List_ग्रन्थिः  before, after;


    जइ ( !list || !ग्रन्थिः )
      प्रत्यागम;

    before = ग्रन्थिः->prev;
    after  = ग्रन्थिः->next;

    /* check whether we are already on top of the list */
    जइ ( !before )
      प्रत्यागम;

    before->next = after;

    जइ ( after )
      after->prev = before;
    अण्णहा
      list->tail = before;

    ग्रन्थिः->prev       = रिक्त;
    ग्रन्थिः->next       = list->head;
    list->head->prev = ग्रन्थिः;
    list->head       = ग्रन्थिः;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( FT_Error )
  FT_List_Iterate( FT_List           list,
                   FT_List_Iterator  iterator,
                   शून्य*             user )
  {
    FT_List_ग्रन्थिः  cur;
    FT_Error     error = FT_Err_Ok;


    जइ ( !list || !iterator )
      प्रत्यागम FT_THROW( Invalid_Argument );

    cur = list->head;

    जाव ( cur )
    {
      FT_List_ग्रन्थिः  next = cur->next;


      error = iterator( cur, user );
      जइ ( error )
        भङ्ग;

      cur = next;
    }

    प्रत्यागम error;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( शून्य )
  FT_List_Finalize( FT_List             list,
                    FT_List_Destructor  destroy,
                    FT_Memory           memory,
                    शून्य*               user )
  {
    FT_List_ग्रन्थिः  cur;


    जइ ( !list || !memory )
      प्रत्यागम;

    cur = list->head;
    जाव ( cur )
    {
      FT_List_ग्रन्थिः  next = cur->next;


      जइ ( destroy )
        destroy( memory, cur->data, user );

      FT_FREE( cur );
      cur = next;
    }

    list->head = रिक्त;
    list->tail = रिक्त;
  }


/* END */
