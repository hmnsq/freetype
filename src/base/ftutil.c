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
#وصف_رد  FT_COMPONENT
#وصف_ڪريو FT_COMPONENT  يادگيري


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


  FT_بنياد_DEF( FT_اشارو )
  ft_mem_alloc( FT_يادگيري  يادگيري,
                FT_Long    سائيز,
                FT_Error  *p_error )
  {
    FT_Error    error;
    FT_اشارو  بلاڪ = ft_mem_qalloc( يادگيري, سائيز, &error );

    إذا ( !error && بلاڪ && سائيز > 0 )
      FT_MEM_ZERO( بلاڪ, سائيز );

    *p_error = error;
    أرجع بلاڪ;
  }


  FT_بنياد_DEF( FT_اشارو )
  ft_mem_qalloc( FT_يادگيري  يادگيري,
                 FT_Long    سائيز,
                 FT_Error  *p_error )
  {
    FT_Error    error = FT_Err_Ok;
    FT_اشارو  بلاڪ = عدم;


    إذا ( سائيز > 0 )
    {
      بلاڪ = يادگيري->alloc( يادگيري, سائيز );
      إذا ( !بلاڪ )
        error = FT_THROW( ٻاهريون_جو_يادگيري );
    }
    وإلا إذا ( سائيز < 0 )
    {
      /* may help catch/prevent security issues */
      error = FT_THROW( Invalid_دليل );
    }

    *p_error = error;
    أرجع بلاڪ;
  }


  FT_بنياد_DEF( FT_اشارو )
  ft_mem_realloc( FT_يادگيري  يادگيري,
                  FT_Long    شي_سائيز,
                  FT_Long    cur_ڳڻپ,
                  FT_Long    new_ڳڻپ,
                  فراغ*      بلاڪ,
                  FT_Error  *p_error )
  {
    FT_Error  error = FT_Err_Ok;


    بلاڪ = ft_mem_qrealloc( يادگيري, شي_سائيز,
                             cur_ڳڻپ, new_ڳڻپ, بلاڪ, &error );
    إذا ( !error && بلاڪ && new_ڳڻپ > cur_ڳڻپ )
      FT_MEM_ZERO( (حرف*)بلاڪ + cur_ڳڻپ * شي_سائيز,
                   ( new_ڳڻپ - cur_ڳڻپ ) * شي_سائيز );

    *p_error = error;
    أرجع بلاڪ;
  }


  FT_بنياد_DEF( FT_اشارو )
  ft_mem_qrealloc( FT_يادگيري  يادگيري,
                   FT_Long    شي_سائيز,
                   FT_Long    cur_ڳڻپ,
                   FT_Long    new_ڳڻپ,
                   فراغ*      بلاڪ,
                   FT_Error  *p_error )
  {
    FT_Error  error = FT_Err_Ok;


    /* Note that we now accept `item_size == 0' as a valid parameter, in
     * order to cover very weird cases where an ALLOC_MULT macro would be
     * called.
     */
    إذا ( cur_ڳڻپ < 0 || new_ڳڻپ < 0 || شي_سائيز < 0 )
    {
      /* may help catch/prevent nasty security issues */
      error = FT_THROW( Invalid_دليل );
    }
    وإلا إذا ( new_ڳڻپ == 0 || شي_سائيز == 0 )
    {
      ft_mem_free( يادگيري, بلاڪ );
      بلاڪ = عدم;
    }
    وإلا إذا ( new_ڳڻپ > FT_INT_MAX / شي_سائيز )
    {
      error = FT_THROW( Array_Too_Large );
    }
    وإلا إذا ( cur_ڳڻپ == 0 )
    {
      FT_ASSERT( !بلاڪ );

      بلاڪ = يادگيري->alloc( يادگيري, new_ڳڻپ * شي_سائيز );
      إذا ( بلاڪ == عدم )
        error = FT_THROW( ٻاهريون_جو_يادگيري );
    }
    وإلا
    {
      FT_اشارو  block2;
      FT_Long     cur_سائيز = cur_ڳڻپ * شي_سائيز;
      FT_Long     new_سائيز = new_ڳڻپ * شي_سائيز;


      block2 = يادگيري->realloc( يادگيري, cur_سائيز, new_سائيز, بلاڪ );
      إذا ( !block2 )
        error = FT_THROW( ٻاهريون_جو_يادگيري );
      وإلا
        بلاڪ = block2;
    }

    *p_error = error;
    أرجع بلاڪ;
  }


  FT_بنياد_DEF( فراغ )
  ft_mem_free( FT_يادگيري   يادگيري,
               ثابت فراغ *P )
  {
    إذا ( P )
      يادگيري->حرر( يادگيري, (فراغ*)P );
  }


  FT_بنياد_DEF( FT_اشارو )
  ft_mem_dup( FT_يادگيري    يادگيري,
              ثابت فراغ*  پتو,
              FT_ULong     سائيز,
              FT_Error    *p_error )
  {
    FT_Error    error;
    FT_اشارو  p = ft_mem_qalloc( يادگيري, (FT_Long)سائيز, &error );


    إذا ( !error && پتو && سائيز > 0 )
      ft_memcpy( p, پتو, سائيز );

    *p_error = error;
    أرجع p;
  }


  FT_بنياد_DEF( FT_اشارو )
  ft_mem_strdup( FT_يادگيري    يادگيري,
                 ثابت حرف*  str,
                 FT_Error    *p_error )
  {
    FT_ULong  len = str ? (FT_ULong)ft_strlen( str ) + 1
                        : 0;


    أرجع ft_mem_dup( يادگيري, str, len, p_error );
  }


  FT_بنياد_DEF( FT_Int )
  ft_mem_strcpyn( حرف*        dst,
                  ثابت حرف*  src,
                  FT_ULong     سائيز )
  {
    طالما ( سائيز > 1 && *src != 0 )
    {
      *dst++ = *src++;
      سائيز--;
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

#وصف_رد  FT_COMPONENT
#وصف_ڪريو FT_COMPONENT  list

  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( FT_List_ڳنڍ )
  FT_List_ڳوليو( FT_List  list,
                فراغ*    معلومات )
  {
    FT_List_ڳنڍ  cur;


    إذا ( !list )
      أرجع عدم;

    cur = list->سر;
    طالما ( cur )
    {
      إذا ( cur->معلومات == معلومات )
        أرجع cur;

      cur = cur->ايندڙ;
    }

    أرجع عدم;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( فراغ )
  FT_List_جوڙيو( FT_List      list,
               FT_List_ڳنڍ  ڳنڍ )
  {
    FT_List_ڳنڍ  before;


    إذا ( !list || !ڳنڍ )
      أرجع;

    before = list->پڇ;

    ڳنڍ->ايندڙ = عدم;
    ڳنڍ->prev = before;

    إذا ( before )
      before->ايندڙ = ڳنڍ;
    وإلا
      list->سر = ڳنڍ;

    list->پڇ = ڳنڍ;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( فراغ )
  FT_List_وجهو( FT_List      list,
                  FT_List_ڳنڍ  ڳنڍ )
  {
    FT_List_ڳنڍ  after;


    إذا ( !list || !ڳنڍ )
      أرجع;

    after = list->سر;

    ڳنڍ->ايندڙ = after;
    ڳنڍ->prev = عدم;

    إذا ( !after )
      list->پڇ = ڳنڍ;
    وإلا
      after->prev = ڳنڍ;

    list->سر = ڳنڍ;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( فراغ )
  FT_List_Remove( FT_List      list,
                  FT_List_ڳنڍ  ڳنڍ )
  {
    FT_List_ڳنڍ  before, after;


    إذا ( !list || !ڳنڍ )
      أرجع;

    before = ڳنڍ->prev;
    after  = ڳنڍ->ايندڙ;

    إذا ( before )
      before->ايندڙ = after;
    وإلا
      list->سر = after;

    إذا ( after )
      after->prev = before;
    وإلا
      list->پڇ = before;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( فراغ )
  FT_List_مٿي( FT_List      list,
              FT_List_ڳنڍ  ڳنڍ )
  {
    FT_List_ڳنڍ  before, after;


    إذا ( !list || !ڳنڍ )
      أرجع;

    before = ڳنڍ->prev;
    after  = ڳنڍ->ايندڙ;

    /* check whether we are already on top of the list */
    إذا ( !before )
      أرجع;

    before->ايندڙ = after;

    إذا ( after )
      after->prev = before;
    وإلا
      list->پڇ = before;

    ڳنڍ->prev       = عدم;
    ڳنڍ->ايندڙ       = list->سر;
    list->سر->prev = ڳنڍ;
    list->سر       = ڳنڍ;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( FT_Error )
  FT_List_Iterate( FT_List           list,
                   FT_List_Iterator  iterator,
                   فراغ*             user )
  {
    FT_List_ڳنڍ  cur;
    FT_Error     error = FT_Err_Ok;


    إذا ( !list || !iterator )
      أرجع FT_THROW( Invalid_دليل );

    cur = list->سر;

    طالما ( cur )
    {
      FT_List_ڳنڍ  ايندڙ = cur->ايندڙ;


      error = iterator( cur, user );
      إذا ( error )
        اكسر;

      cur = ايندڙ;
    }

    أرجع error;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( فراغ )
  FT_List_Finalize( FT_List             list,
                    FT_List_Destructor  destroy,
                    FT_يادگيري           يادگيري,
                    فراغ*               user )
  {
    FT_List_ڳنڍ  cur;


    إذا ( !list || !يادگيري )
      أرجع;

    cur = list->سر;
    طالما ( cur )
    {
      FT_List_ڳنڍ  ايندڙ = cur->ايندڙ;


      إذا ( destroy )
        destroy( يادگيري, cur->معلومات, user );

      FT_FREE( cur );
      cur = ايندڙ;
    }

    list->سر = عدم;
    list->پڇ = عدم;
  }


/*پڇاڙي*/
