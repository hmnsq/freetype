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
#لغو_تعریف  FT_COMPONENT
#تعریف_کن FT_COMPONENT  حافظه


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


  FT_پایه_DEF( FT_اشاره_گر )
  ft_mem_alloc( FT_حافظه  حافظه,
                FT_Long    اندازه,
                FT_Error  *p_error )
  {
    FT_Error    error;
    FT_اشاره_گر  بلوک = ft_mem_qalloc( حافظه, اندازه, &error );

    إذا ( !error && بلوک && اندازه > 0 )
      FT_MEM_ZERO( بلوک, اندازه );

    *p_error = error;
    أرجع بلوک;
  }


  FT_پایه_DEF( FT_اشاره_گر )
  ft_mem_qalloc( FT_حافظه  حافظه,
                 FT_Long    اندازه,
                 FT_Error  *p_error )
  {
    FT_Error    error = FT_Err_Ok;
    FT_اشاره_گر  بلوک = عدم;


    إذا ( اندازه > 0 )
    {
      بلوک = حافظه->alloc( حافظه, اندازه );
      إذا ( !بلوک )
        error = FT_THROW( بیرون_از_حافظه );
    }
    وإلا إذا ( اندازه < 0 )
    {
      /* may help catch/prevent security issues */
      error = FT_THROW( Invalid_آرگومان );
    }

    *p_error = error;
    أرجع بلوک;
  }


  FT_پایه_DEF( FT_اشاره_گر )
  ft_mem_realloc( FT_حافظه  حافظه,
                  FT_Long    قلم_اندازه,
                  FT_Long    cur_شمار,
                  FT_Long    new_شمار,
                  فراغ*      بلوک,
                  FT_Error  *p_error )
  {
    FT_Error  error = FT_Err_Ok;


    بلوک = ft_mem_qrealloc( حافظه, قلم_اندازه,
                             cur_شمار, new_شمار, بلوک, &error );
    إذا ( !error && بلوک && new_شمار > cur_شمار )
      FT_MEM_ZERO( (حرف*)بلوک + cur_شمار * قلم_اندازه,
                   ( new_شمار - cur_شمار ) * قلم_اندازه );

    *p_error = error;
    أرجع بلوک;
  }


  FT_پایه_DEF( FT_اشاره_گر )
  ft_mem_qrealloc( FT_حافظه  حافظه,
                   FT_Long    قلم_اندازه,
                   FT_Long    cur_شمار,
                   FT_Long    new_شمار,
                   فراغ*      بلوک,
                   FT_Error  *p_error )
  {
    FT_Error  error = FT_Err_Ok;


    /* Note that we now accept `item_size == 0' as a valid parameter, in
     * order to cover very weird cases where an ALLOC_MULT macro would be
     * called.
     */
    إذا ( cur_شمار < 0 || new_شمار < 0 || قلم_اندازه < 0 )
    {
      /* may help catch/prevent nasty security issues */
      error = FT_THROW( Invalid_آرگومان );
    }
    وإلا إذا ( new_شمار == 0 || قلم_اندازه == 0 )
    {
      ft_mem_free( حافظه, بلوک );
      بلوک = عدم;
    }
    وإلا إذا ( new_شمار > FT_INT_MAX / قلم_اندازه )
    {
      error = FT_THROW( Array_Too_Large );
    }
    وإلا إذا ( cur_شمار == 0 )
    {
      FT_ASSERT( !بلوک );

      بلوک = حافظه->alloc( حافظه, new_شمار * قلم_اندازه );
      إذا ( بلوک == عدم )
        error = FT_THROW( بیرون_از_حافظه );
    }
    وإلا
    {
      FT_اشاره_گر  block2;
      FT_Long     cur_اندازه = cur_شمار * قلم_اندازه;
      FT_Long     new_اندازه = new_شمار * قلم_اندازه;


      block2 = حافظه->realloc( حافظه, cur_اندازه, new_اندازه, بلوک );
      إذا ( !block2 )
        error = FT_THROW( بیرون_از_حافظه );
      وإلا
        بلوک = block2;
    }

    *p_error = error;
    أرجع بلوک;
  }


  FT_پایه_DEF( فراغ )
  ft_mem_free( FT_حافظه   حافظه,
               ثابت فراغ *P )
  {
    إذا ( P )
      حافظه->حرر( حافظه, (فراغ*)P );
  }


  FT_پایه_DEF( FT_اشاره_گر )
  ft_mem_dup( FT_حافظه    حافظه,
              ثابت فراغ*  آدرس,
              FT_ULong     اندازه,
              FT_Error    *p_error )
  {
    FT_Error    error;
    FT_اشاره_گر  p = ft_mem_qalloc( حافظه, (FT_Long)اندازه, &error );


    إذا ( !error && آدرس && اندازه > 0 )
      ft_memcpy( p, آدرس, اندازه );

    *p_error = error;
    أرجع p;
  }


  FT_پایه_DEF( FT_اشاره_گر )
  ft_mem_strdup( FT_حافظه    حافظه,
                 ثابت حرف*  str,
                 FT_Error    *p_error )
  {
    FT_ULong  len = str ? (FT_ULong)ft_strlen( str ) + 1
                        : 0;


    أرجع ft_mem_dup( حافظه, str, len, p_error );
  }


  FT_پایه_DEF( FT_Int )
  ft_mem_strcpyn( حرف*        dst,
                  ثابت حرف*  src,
                  FT_ULong     اندازه )
  {
    طالما ( اندازه > 1 && *src != 0 )
    {
      *dst++ = *src++;
      اندازه--;
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

#لغو_تعریف  FT_COMPONENT
#تعریف_کن FT_COMPONENT  list

  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( FT_List_گره )
  FT_List_پیدا_کن( FT_List  list,
                فراغ*    داده )
  {
    FT_List_گره  cur;


    إذا ( !list )
      أرجع عدم;

    cur = list->سر;
    طالما ( cur )
    {
      إذا ( cur->داده == داده )
        أرجع cur;

      cur = cur->بعدی;
    }

    أرجع عدم;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( فراغ )
  FT_List_جمع_کن( FT_List      list,
               FT_List_گره  گره )
  {
    FT_List_گره  before;


    إذا ( !list || !گره )
      أرجع;

    before = list->دم;

    گره->بعدی = عدم;
    گره->prev = before;

    إذا ( before )
      before->بعدی = گره;
    وإلا
      list->سر = گره;

    list->دم = گره;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( فراغ )
  FT_List_درج( FT_List      list,
                  FT_List_گره  گره )
  {
    FT_List_گره  after;


    إذا ( !list || !گره )
      أرجع;

    after = list->سر;

    گره->بعدی = after;
    گره->prev = عدم;

    إذا ( !after )
      list->دم = گره;
    وإلا
      after->prev = گره;

    list->سر = گره;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( فراغ )
  FT_List_Remove( FT_List      list,
                  FT_List_گره  گره )
  {
    FT_List_گره  before, after;


    إذا ( !list || !گره )
      أرجع;

    before = گره->prev;
    after  = گره->بعدی;

    إذا ( before )
      before->بعدی = after;
    وإلا
      list->سر = after;

    إذا ( after )
      after->prev = before;
    وإلا
      list->دم = before;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( فراغ )
  FT_List_به_بالا( FT_List      list,
              FT_List_گره  گره )
  {
    FT_List_گره  before, after;


    إذا ( !list || !گره )
      أرجع;

    before = گره->prev;
    after  = گره->بعدی;

    /* check whether we are already on top of the list */
    إذا ( !before )
      أرجع;

    before->بعدی = after;

    إذا ( after )
      after->prev = before;
    وإلا
      list->دم = before;

    گره->prev       = عدم;
    گره->بعدی       = list->سر;
    list->سر->prev = گره;
    list->سر       = گره;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( FT_Error )
  FT_List_Iterate( FT_List           list,
                   FT_List_Iterator  iterator,
                   فراغ*             user )
  {
    FT_List_گره  cur;
    FT_Error     error = FT_Err_Ok;


    إذا ( !list || !iterator )
      أرجع FT_THROW( Invalid_آرگومان );

    cur = list->سر;

    طالما ( cur )
    {
      FT_List_گره  بعدی = cur->بعدی;


      error = iterator( cur, user );
      إذا ( error )
        اكسر;

      cur = بعدی;
    }

    أرجع error;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( فراغ )
  FT_List_Finalize( FT_List             list,
                    FT_List_Destructor  destroy,
                    FT_حافظه           حافظه,
                    فراغ*               user )
  {
    FT_List_گره  cur;


    إذا ( !list || !حافظه )
      أرجع;

    cur = list->سر;
    طالما ( cur )
    {
      FT_List_گره  بعدی = cur->بعدی;


      إذا ( destroy )
        destroy( حافظه, cur->داده, user );

      FT_FREE( cur );
      cur = بعدی;
    }

    list->سر = عدم;
    list->دم = عدم;
  }


/*انتها*/
