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
#تعریف_لغوه  FT_COMPONENT
#تعریف_کړه FT_COMPONENT  حافظه


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


  FT_بنسټ_DEF( FT_اشاره_ګر )
  ft_mem_alloc( FT_حافظه  حافظه,
                FT_Long    اندازه,
                FT_Error  *p_error )
  {
    FT_Error    error;
    FT_اشاره_ګر  بلاک = ft_mem_qalloc( حافظه, اندازه, &error );

    إذا ( !error && بلاک && اندازه > 0 )
      FT_MEM_ZERO( بلاک, اندازه );

    *p_error = error;
    أرجع بلاک;
  }


  FT_بنسټ_DEF( FT_اشاره_ګر )
  ft_mem_qalloc( FT_حافظه  حافظه,
                 FT_Long    اندازه,
                 FT_Error  *p_error )
  {
    FT_Error    error = FT_Err_Ok;
    FT_اشاره_ګر  بلاک = عدم;


    إذا ( اندازه > 0 )
    {
      بلاک = حافظه->alloc( حافظه, اندازه );
      إذا ( !بلاک )
        error = FT_THROW( بهر_د_حافظه );
    }
    وإلا إذا ( اندازه < 0 )
    {
      /* may help catch/prevent security issues */
      error = FT_THROW( Invalid_دلیل );
    }

    *p_error = error;
    أرجع بلاک;
  }


  FT_بنسټ_DEF( FT_اشاره_ګر )
  ft_mem_realloc( FT_حافظه  حافظه,
                  FT_Long    توکی_اندازه,
                  FT_Long    cur_شمېر,
                  FT_Long    new_شمېر,
                  فراغ*      بلاک,
                  FT_Error  *p_error )
  {
    FT_Error  error = FT_Err_Ok;


    بلاک = ft_mem_qrealloc( حافظه, توکی_اندازه,
                             cur_شمېر, new_شمېر, بلاک, &error );
    إذا ( !error && بلاک && new_شمېر > cur_شمېر )
      FT_MEM_ZERO( (حرف*)بلاک + cur_شمېر * توکی_اندازه,
                   ( new_شمېر - cur_شمېر ) * توکی_اندازه );

    *p_error = error;
    أرجع بلاک;
  }


  FT_بنسټ_DEF( FT_اشاره_ګر )
  ft_mem_qrealloc( FT_حافظه  حافظه,
                   FT_Long    توکی_اندازه,
                   FT_Long    cur_شمېر,
                   FT_Long    new_شمېر,
                   فراغ*      بلاک,
                   FT_Error  *p_error )
  {
    FT_Error  error = FT_Err_Ok;


    /* Note that we now accept `item_size == 0' as a valid parameter, in
     * order to cover very weird cases where an ALLOC_MULT macro would be
     * called.
     */
    إذا ( cur_شمېر < 0 || new_شمېر < 0 || توکی_اندازه < 0 )
    {
      /* may help catch/prevent nasty security issues */
      error = FT_THROW( Invalid_دلیل );
    }
    وإلا إذا ( new_شمېر == 0 || توکی_اندازه == 0 )
    {
      ft_mem_free( حافظه, بلاک );
      بلاک = عدم;
    }
    وإلا إذا ( new_شمېر > FT_INT_MAX / توکی_اندازه )
    {
      error = FT_THROW( Array_Too_Large );
    }
    وإلا إذا ( cur_شمېر == 0 )
    {
      FT_ASSERT( !بلاک );

      بلاک = حافظه->alloc( حافظه, new_شمېر * توکی_اندازه );
      إذا ( بلاک == عدم )
        error = FT_THROW( بهر_د_حافظه );
    }
    وإلا
    {
      FT_اشاره_ګر  block2;
      FT_Long     cur_اندازه = cur_شمېر * توکی_اندازه;
      FT_Long     new_اندازه = new_شمېر * توکی_اندازه;


      block2 = حافظه->realloc( حافظه, cur_اندازه, new_اندازه, بلاک );
      إذا ( !block2 )
        error = FT_THROW( بهر_د_حافظه );
      وإلا
        بلاک = block2;
    }

    *p_error = error;
    أرجع بلاک;
  }


  FT_بنسټ_DEF( فراغ )
  ft_mem_free( FT_حافظه   حافظه,
               ثابت فراغ *P )
  {
    إذا ( P )
      حافظه->حرر( حافظه, (فراغ*)P );
  }


  FT_بنسټ_DEF( FT_اشاره_ګر )
  ft_mem_dup( FT_حافظه    حافظه,
              ثابت فراغ*  پته,
              FT_ULong     اندازه,
              FT_Error    *p_error )
  {
    FT_Error    error;
    FT_اشاره_ګر  p = ft_mem_qalloc( حافظه, (FT_Long)اندازه, &error );


    إذا ( !error && پته && اندازه > 0 )
      ft_memcpy( p, پته, اندازه );

    *p_error = error;
    أرجع p;
  }


  FT_بنسټ_DEF( FT_اشاره_ګر )
  ft_mem_strdup( FT_حافظه    حافظه,
                 ثابت حرف*  str,
                 FT_Error    *p_error )
  {
    FT_ULong  len = str ? (FT_ULong)ft_strlen( str ) + 1
                        : 0;


    أرجع ft_mem_dup( حافظه, str, len, p_error );
  }


  FT_بنسټ_DEF( FT_Int )
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

#تعریف_لغوه  FT_COMPONENT
#تعریف_کړه FT_COMPONENT  list

  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( FT_List_غوټه )
  FT_List_پیدا_کړه( FT_List  list,
                فراغ*    معلومات )
  {
    FT_List_غوټه  cur;


    إذا ( !list )
      أرجع عدم;

    cur = list->سر_2;
    طالما ( cur )
    {
      إذا ( cur->معلومات == معلومات )
        أرجع cur;

      cur = cur->بل;
    }

    أرجع عدم;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( فراغ )
  FT_List_جمع_کړه( FT_List      list,
               FT_List_غوټه  غوټه )
  {
    FT_List_غوټه  before;


    إذا ( !list || !غوټه )
      أرجع;

    before = list->لکۍ;

    غوټه->بل = عدم;
    غوټه->prev = before;

    إذا ( before )
      before->بل = غوټه;
    وإلا
      list->سر_2 = غوټه;

    list->لکۍ = غوټه;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( فراغ )
  FT_List_ورننباسه( FT_List      list,
                  FT_List_غوټه  غوټه )
  {
    FT_List_غوټه  after;


    إذا ( !list || !غوټه )
      أرجع;

    after = list->سر_2;

    غوټه->بل = after;
    غوټه->prev = عدم;

    إذا ( !after )
      list->لکۍ = غوټه;
    وإلا
      after->prev = غوټه;

    list->سر_2 = غوټه;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( فراغ )
  FT_List_Remove( FT_List      list,
                  FT_List_غوټه  غوټه )
  {
    FT_List_غوټه  before, after;


    إذا ( !list || !غوټه )
      أرجع;

    before = غوټه->prev;
    after  = غوټه->بل;

    إذا ( before )
      before->بل = after;
    وإلا
      list->سر_2 = after;

    إذا ( after )
      after->prev = before;
    وإلا
      list->لکۍ = before;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( فراغ )
  FT_List_پورته( FT_List      list,
              FT_List_غوټه  غوټه )
  {
    FT_List_غوټه  before, after;


    إذا ( !list || !غوټه )
      أرجع;

    before = غوټه->prev;
    after  = غوټه->بل;

    /* check whether we are already on top of the list */
    إذا ( !before )
      أرجع;

    before->بل = after;

    إذا ( after )
      after->prev = before;
    وإلا
      list->لکۍ = before;

    غوټه->prev       = عدم;
    غوټه->بل       = list->سر_2;
    list->سر_2->prev = غوټه;
    list->سر_2       = غوټه;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( FT_Error )
  FT_List_Iterate( FT_List           list,
                   FT_List_Iterator  iterator,
                   فراغ*             user )
  {
    FT_List_غوټه  cur;
    FT_Error     error = FT_Err_Ok;


    إذا ( !list || !iterator )
      أرجع FT_THROW( Invalid_دلیل );

    cur = list->سر_2;

    طالما ( cur )
    {
      FT_List_غوټه  بل = cur->بل;


      error = iterator( cur, user );
      إذا ( error )
        اكسر;

      cur = بل;
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
    FT_List_غوټه  cur;


    إذا ( !list || !حافظه )
      أرجع;

    cur = list->سر_2;
    طالما ( cur )
    {
      FT_List_غوټه  بل = cur->بل;


      إذا ( destroy )
        destroy( حافظه, cur->معلومات, user );

      FT_FREE( cur );
      cur = بل;
    }

    list->سر_2 = عدم;
    list->لکۍ = عدم;
  }


/*پای_ټکی*/
