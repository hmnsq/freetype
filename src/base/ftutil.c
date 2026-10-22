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

    जर ( !error && block && size > 0 )
      FT_MEM_ZERO( block, size );

    *p_error = error;
    परत block;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_qalloc( FT_Memory  memory,
                 FT_Long    size,
                 FT_Error  *p_error )
  {
    FT_Error    error = FT_Err_Ok;
    FT_Pointer  block = रिक्त;


    जर ( size > 0 )
    {
      block = memory->alloc( memory, size );
      जर ( !block )
        error = FT_THROW( Out_Of_Memory );
    }
    अन्यथा जर ( size < 0 )
    {
      /* may help catch/prevent security issues */
      error = FT_THROW( Invalid_Argument );
    }

    *p_error = error;
    परत block;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_realloc( FT_Memory  memory,
                  FT_Long    item_size,
                  FT_Long    cur_मोजणी,
                  FT_Long    new_मोजणी,
                  शून्य*      block,
                  FT_Error  *p_error )
  {
    FT_Error  error = FT_Err_Ok;


    block = ft_mem_qrealloc( memory, item_size,
                             cur_मोजणी, new_मोजणी, block, &error );
    जर ( !error && block && new_मोजणी > cur_मोजणी )
      FT_MEM_ZERO( (अक्षर*)block + cur_मोजणी * item_size,
                   ( new_मोजणी - cur_मोजणी ) * item_size );

    *p_error = error;
    परत block;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_qrealloc( FT_Memory  memory,
                   FT_Long    item_size,
                   FT_Long    cur_मोजणी,
                   FT_Long    new_मोजणी,
                   शून्य*      block,
                   FT_Error  *p_error )
  {
    FT_Error  error = FT_Err_Ok;


    /* Note that we now accept `item_size == 0' as a valid parameter, in
     * order to cover very weird cases where an ALLOC_MULT macro would be
     * called.
     */
    जर ( cur_मोजणी < 0 || new_मोजणी < 0 || item_size < 0 )
    {
      /* may help catch/prevent nasty security issues */
      error = FT_THROW( Invalid_Argument );
    }
    अन्यथा जर ( new_मोजणी == 0 || item_size == 0 )
    {
      ft_mem_free( memory, block );
      block = रिक्त;
    }
    अन्यथा जर ( new_मोजणी > FT_INT_MAX / item_size )
    {
      error = FT_THROW( Array_Too_Large );
    }
    अन्यथा जर ( cur_मोजणी == 0 )
    {
      FT_ASSERT( !block );

      block = memory->alloc( memory, new_मोजणी * item_size );
      जर ( block == रिक्त )
        error = FT_THROW( Out_Of_Memory );
    }
    अन्यथा
    {
      FT_Pointer  block2;
      FT_Long     cur_size = cur_मोजणी * item_size;
      FT_Long     new_size = new_मोजणी * item_size;


      block2 = memory->पुनः_स्मृति( memory, cur_size, new_size, block );
      जर ( !block2 )
        error = FT_THROW( Out_Of_Memory );
      अन्यथा
        block = block2;
    }

    *p_error = error;
    परत block;
  }


  FT_BASE_DEF( शून्य )
  ft_mem_free( FT_Memory   memory,
               स्थिरांक शून्य *P )
  {
    जर ( P )
      memory->मुक्त( memory, (शून्य*)P );
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_dup( FT_Memory    memory,
              स्थिरांक शून्य*  address,
              FT_ULong     size,
              FT_Error    *p_error )
  {
    FT_Error    error;
    FT_Pointer  p = ft_mem_qalloc( memory, (FT_Long)size, &error );


    जर ( !error && address && size > 0 )
      ft_memcpy( p, address, size );

    *p_error = error;
    परत p;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_strdup( FT_Memory    memory,
                 स्थिरांक अक्षर*  str,
                 FT_Error    *p_error )
  {
    FT_ULong  len = str ? (FT_ULong)ft_strlen( str ) + 1
                        : 0;


    परत ft_mem_dup( memory, str, len, p_error );
  }


  FT_BASE_DEF( FT_Int )
  ft_mem_strcpyn( अक्षर*        dst,
                  स्थिरांक अक्षर*  src,
                  FT_ULong     size )
  {
    पर्यंत ( size > 1 && *src != 0 )
    {
      *dst++ = *src++;
      size--;
    }

    *dst = 0;  /* always zero-terminate */

    परत *src != 0;
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

  FT_EXPORT_DEF( FT_List_गाठ )
  FT_List_Find( FT_List  list,
                शून्य*    data )
  {
    FT_List_गाठ  cur;


    जर ( !list )
      परत रिक्त;

    cur = list->head;
    पर्यंत ( cur )
    {
      जर ( cur->data == data )
        परत cur;

      cur = cur->next;
    }

    परत रिक्त;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( शून्य )
  FT_List_Add( FT_List      list,
               FT_List_गाठ  गाठ )
  {
    FT_List_गाठ  before;


    जर ( !list || !गाठ )
      परत;

    before = list->tail;

    गाठ->next = रिक्त;
    गाठ->prev = before;

    जर ( before )
      before->next = गाठ;
    अन्यथा
      list->head = गाठ;

    list->tail = गाठ;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( शून्य )
  FT_List_घाला( FT_List      list,
                  FT_List_गाठ  गाठ )
  {
    FT_List_गाठ  after;


    जर ( !list || !गाठ )
      परत;

    after = list->head;

    गाठ->next = after;
    गाठ->prev = रिक्त;

    जर ( !after )
      list->tail = गाठ;
    अन्यथा
      after->prev = गाठ;

    list->head = गाठ;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( शून्य )
  FT_List_Remove( FT_List      list,
                  FT_List_गाठ  गाठ )
  {
    FT_List_गाठ  before, after;


    जर ( !list || !गाठ )
      परत;

    before = गाठ->prev;
    after  = गाठ->next;

    जर ( before )
      before->next = after;
    अन्यथा
      list->head = after;

    जर ( after )
      after->prev = before;
    अन्यथा
      list->tail = before;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( शून्य )
  FT_List_Up( FT_List      list,
              FT_List_गाठ  गाठ )
  {
    FT_List_गाठ  before, after;


    जर ( !list || !गाठ )
      परत;

    before = गाठ->prev;
    after  = गाठ->next;

    /* check whether we are already on top of the list */
    जर ( !before )
      परत;

    before->next = after;

    जर ( after )
      after->prev = before;
    अन्यथा
      list->tail = before;

    गाठ->prev       = रिक्त;
    गाठ->next       = list->head;
    list->head->prev = गाठ;
    list->head       = गाठ;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( FT_Error )
  FT_List_Iterate( FT_List           list,
                   FT_List_Iterator  iterator,
                   शून्य*             user )
  {
    FT_List_गाठ  cur;
    FT_Error     error = FT_Err_Ok;


    जर ( !list || !iterator )
      परत FT_THROW( Invalid_Argument );

    cur = list->head;

    पर्यंत ( cur )
    {
      FT_List_गाठ  next = cur->next;


      error = iterator( cur, user );
      जर ( error )
        थांबा;

      cur = next;
    }

    परत error;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( शून्य )
  FT_List_Finalize( FT_List             list,
                    FT_List_Destructor  destroy,
                    FT_Memory           memory,
                    शून्य*               user )
  {
    FT_List_गाठ  cur;


    जर ( !list || !memory )
      परत;

    cur = list->head;
    पर्यंत ( cur )
    {
      FT_List_गाठ  next = cur->next;


      जर ( destroy )
        destroy( memory, cur->data, user );

      FT_FREE( cur );
      cur = next;
    }

    list->head = रिक्त;
    list->tail = रिक्त;
  }


/* END */
