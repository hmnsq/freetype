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

    यदि ( !error && block && size > 0 )
      FT_MEM_ZERO( block, size );

    *p_error = error;
    फर्काऊ block;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_qalloc( FT_Memory  memory,
                 FT_Long    size,
                 FT_Error  *p_error )
  {
    FT_Error    error = FT_Err_Ok;
    FT_Pointer  block = रिक्त;


    यदि ( size > 0 )
    {
      block = memory->alloc( memory, size );
      यदि ( !block )
        error = FT_THROW( Out_Of_Memory );
    }
    अन्यथा यदि ( size < 0 )
    {
      /* may help catch/prevent security issues */
      error = FT_THROW( Invalid_Argument );
    }

    *p_error = error;
    फर्काऊ block;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_realloc( FT_Memory  memory,
                  FT_Long    item_size,
                  FT_Long    cur_गन्ती,
                  FT_Long    new_गन्ती,
                  शून्य*      block,
                  FT_Error  *p_error )
  {
    FT_Error  error = FT_Err_Ok;


    block = ft_mem_qrealloc( memory, item_size,
                             cur_गन्ती, new_गन्ती, block, &error );
    यदि ( !error && block && new_गन्ती > cur_गन्ती )
      FT_MEM_ZERO( (वर्ण*)block + cur_गन्ती * item_size,
                   ( new_गन्ती - cur_गन्ती ) * item_size );

    *p_error = error;
    फर्काऊ block;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_qrealloc( FT_Memory  memory,
                   FT_Long    item_size,
                   FT_Long    cur_गन्ती,
                   FT_Long    new_गन्ती,
                   शून्य*      block,
                   FT_Error  *p_error )
  {
    FT_Error  error = FT_Err_Ok;


    /* Note that we now accept `item_size == 0' as a valid parameter, in
     * order to cover very weird cases where an ALLOC_MULT macro would be
     * called.
     */
    यदि ( cur_गन्ती < 0 || new_गन्ती < 0 || item_size < 0 )
    {
      /* may help catch/prevent nasty security issues */
      error = FT_THROW( Invalid_Argument );
    }
    अन्यथा यदि ( new_गन्ती == 0 || item_size == 0 )
    {
      ft_mem_free( memory, block );
      block = रिक्त;
    }
    अन्यथा यदि ( new_गन्ती > FT_INT_MAX / item_size )
    {
      error = FT_THROW( Array_Too_Large );
    }
    अन्यथा यदि ( cur_गन्ती == 0 )
    {
      FT_ASSERT( !block );

      block = memory->alloc( memory, new_गन्ती * item_size );
      यदि ( block == रिक्त )
        error = FT_THROW( Out_Of_Memory );
    }
    अन्यथा
    {
      FT_Pointer  block2;
      FT_Long     cur_size = cur_गन्ती * item_size;
      FT_Long     new_size = new_गन्ती * item_size;


      block2 = memory->पुनः_स्मृति( memory, cur_size, new_size, block );
      यदि ( !block2 )
        error = FT_THROW( Out_Of_Memory );
      अन्यथा
        block = block2;
    }

    *p_error = error;
    फर्काऊ block;
  }


  FT_BASE_DEF( शून्य )
  ft_mem_free( FT_Memory   memory,
               अचर शून्य *P )
  {
    यदि ( P )
      memory->मुक्त( memory, (शून्य*)P );
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_dup( FT_Memory    memory,
              अचर शून्य*  address,
              FT_ULong     size,
              FT_Error    *p_error )
  {
    FT_Error    error;
    FT_Pointer  p = ft_mem_qalloc( memory, (FT_Long)size, &error );


    यदि ( !error && address && size > 0 )
      ft_memcpy( p, address, size );

    *p_error = error;
    फर्काऊ p;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_strdup( FT_Memory    memory,
                 अचर वर्ण*  str,
                 FT_Error    *p_error )
  {
    FT_ULong  len = str ? (FT_ULong)ft_strlen( str ) + 1
                        : 0;


    फर्काऊ ft_mem_dup( memory, str, len, p_error );
  }


  FT_BASE_DEF( FT_Int )
  ft_mem_strcpyn( वर्ण*        dst,
                  अचर वर्ण*  src,
                  FT_ULong     size )
  {
    जबसम्म ( size > 1 && *src != 0 )
    {
      *dst++ = *src++;
      size--;
    }

    *dst = 0;  /* always zero-terminate */

    फर्काऊ *src != 0;
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

  FT_EXPORT_DEF( FT_List_गाँठो )
  FT_List_Find( FT_List  list,
                शून्य*    data )
  {
    FT_List_गाँठो  cur;


    यदि ( !list )
      फर्काऊ रिक्त;

    cur = list->head;
    जबसम्म ( cur )
    {
      यदि ( cur->data == data )
        फर्काऊ cur;

      cur = cur->next;
    }

    फर्काऊ रिक्त;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( शून्य )
  FT_List_Add( FT_List      list,
               FT_List_गाँठो  गाँठो )
  {
    FT_List_गाँठो  before;


    यदि ( !list || !गाँठो )
      फर्काऊ;

    before = list->tail;

    गाँठो->next = रिक्त;
    गाँठो->prev = before;

    यदि ( before )
      before->next = गाँठो;
    अन्यथा
      list->head = गाँठो;

    list->tail = गाँठो;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( शून्य )
  FT_List_घुसाउ( FT_List      list,
                  FT_List_गाँठो  गाँठो )
  {
    FT_List_गाँठो  after;


    यदि ( !list || !गाँठो )
      फर्काऊ;

    after = list->head;

    गाँठो->next = after;
    गाँठो->prev = रिक्त;

    यदि ( !after )
      list->tail = गाँठो;
    अन्यथा
      after->prev = गाँठो;

    list->head = गाँठो;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( शून्य )
  FT_List_Remove( FT_List      list,
                  FT_List_गाँठो  गाँठो )
  {
    FT_List_गाँठो  before, after;


    यदि ( !list || !गाँठो )
      फर्काऊ;

    before = गाँठो->prev;
    after  = गाँठो->next;

    यदि ( before )
      before->next = after;
    अन्यथा
      list->head = after;

    यदि ( after )
      after->prev = before;
    अन्यथा
      list->tail = before;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( शून्य )
  FT_List_Up( FT_List      list,
              FT_List_गाँठो  गाँठो )
  {
    FT_List_गाँठो  before, after;


    यदि ( !list || !गाँठो )
      फर्काऊ;

    before = गाँठो->prev;
    after  = गाँठो->next;

    /* check whether we are already on top of the list */
    यदि ( !before )
      फर्काऊ;

    before->next = after;

    यदि ( after )
      after->prev = before;
    अन्यथा
      list->tail = before;

    गाँठो->prev       = रिक्त;
    गाँठो->next       = list->head;
    list->head->prev = गाँठो;
    list->head       = गाँठो;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( FT_Error )
  FT_List_Iterate( FT_List           list,
                   FT_List_Iterator  iterator,
                   शून्य*             user )
  {
    FT_List_गाँठो  cur;
    FT_Error     error = FT_Err_Ok;


    यदि ( !list || !iterator )
      फर्काऊ FT_THROW( Invalid_Argument );

    cur = list->head;

    जबसम्म ( cur )
    {
      FT_List_गाँठो  next = cur->next;


      error = iterator( cur, user );
      यदि ( error )
        रोक;

      cur = next;
    }

    फर्काऊ error;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( शून्य )
  FT_List_Finalize( FT_List             list,
                    FT_List_Destructor  destroy,
                    FT_Memory           memory,
                    शून्य*               user )
  {
    FT_List_गाँठो  cur;


    यदि ( !list || !memory )
      फर्काऊ;

    cur = list->head;
    जबसम्म ( cur )
    {
      FT_List_गाँठो  next = cur->next;


      यदि ( destroy )
        destroy( memory, cur->data, user );

      FT_FREE( cur );
      cur = next;
    }

    list->head = रिक्त;
    list->tail = रिक्त;
  }


/* END */
