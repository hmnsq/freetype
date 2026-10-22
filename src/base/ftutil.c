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
#تعریف_رد  FT_COMPONENT
#تعریف_کرو FT_COMPONENT  حافظہ


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


  FT_ادھار_DEF( FT_اشارہ_گر )
  ft_mem_alloc( FT_حافظہ  حافظہ,
                FT_Long    ناپ,
                FT_Error  *p_error )
  {
    FT_Error    error;
    FT_اشارہ_گر  بلاک = ft_mem_qalloc( حافظہ, ناپ, &error );

    إذا ( !error && بلاک && ناپ > 0 )
      FT_MEM_ZERO( بلاک, ناپ );

    *p_error = error;
    أرجع بلاک;
  }


  FT_ادھار_DEF( FT_اشارہ_گر )
  ft_mem_qalloc( FT_حافظہ  حافظہ,
                 FT_Long    ناپ,
                 FT_Error  *p_error )
  {
    FT_Error    error = FT_Err_Ok;
    FT_اشارہ_گر  بلاک = عدم;


    إذا ( ناپ > 0 )
    {
      بلاک = حافظہ->alloc( حافظہ, ناپ );
      إذا ( !بلاک )
        error = FT_THROW( باہرلا_دا_حافظہ );
    }
    وإلا إذا ( ناپ < 0 )
    {
      /* may help catch/prevent security issues */
      error = FT_THROW( Invalid_دلیل );
    }

    *p_error = error;
    أرجع بلاک;
  }


  FT_ادھار_DEF( FT_اشارہ_گر )
  ft_mem_realloc( FT_حافظہ  حافظہ,
                  FT_Long    شے_ناپ,
                  FT_Long    cur_گنتی,
                  FT_Long    new_گنتی,
                  فراغ*      بلاک,
                  FT_Error  *p_error )
  {
    FT_Error  error = FT_Err_Ok;


    بلاک = ft_mem_qrealloc( حافظہ, شے_ناپ,
                             cur_گنتی, new_گنتی, بلاک, &error );
    إذا ( !error && بلاک && new_گنتی > cur_گنتی )
      FT_MEM_ZERO( (حرف*)بلاک + cur_گنتی * شے_ناپ,
                   ( new_گنتی - cur_گنتی ) * شے_ناپ );

    *p_error = error;
    أرجع بلاک;
  }


  FT_ادھار_DEF( FT_اشارہ_گر )
  ft_mem_qrealloc( FT_حافظہ  حافظہ,
                   FT_Long    شے_ناپ,
                   FT_Long    cur_گنتی,
                   FT_Long    new_گنتی,
                   فراغ*      بلاک,
                   FT_Error  *p_error )
  {
    FT_Error  error = FT_Err_Ok;


    /* Note that we now accept `item_size == 0' as a valid parameter, in
     * order to cover very weird cases where an ALLOC_MULT macro would be
     * called.
     */
    إذا ( cur_گنتی < 0 || new_گنتی < 0 || شے_ناپ < 0 )
    {
      /* may help catch/prevent nasty security issues */
      error = FT_THROW( Invalid_دلیل );
    }
    وإلا إذا ( new_گنتی == 0 || شے_ناپ == 0 )
    {
      ft_mem_free( حافظہ, بلاک );
      بلاک = عدم;
    }
    وإلا إذا ( new_گنتی > FT_INT_MAX / شے_ناپ )
    {
      error = FT_THROW( Array_Too_Large );
    }
    وإلا إذا ( cur_گنتی == 0 )
    {
      FT_ASSERT( !بلاک );

      بلاک = حافظہ->alloc( حافظہ, new_گنتی * شے_ناپ );
      إذا ( بلاک == عدم )
        error = FT_THROW( باہرلا_دا_حافظہ );
    }
    وإلا
    {
      FT_اشارہ_گر  block2;
      FT_Long     cur_ناپ = cur_گنتی * شے_ناپ;
      FT_Long     new_ناپ = new_گنتی * شے_ناپ;


      block2 = حافظہ->realloc( حافظہ, cur_ناپ, new_ناپ, بلاک );
      إذا ( !block2 )
        error = FT_THROW( باہرلا_دا_حافظہ );
      وإلا
        بلاک = block2;
    }

    *p_error = error;
    أرجع بلاک;
  }


  FT_ادھار_DEF( فراغ )
  ft_mem_free( FT_حافظہ   حافظہ,
               ثابت فراغ *P )
  {
    إذا ( P )
      حافظہ->حرر( حافظہ, (فراغ*)P );
  }


  FT_ادھار_DEF( FT_اشارہ_گر )
  ft_mem_dup( FT_حافظہ    حافظہ,
              ثابت فراغ*  پتہ,
              FT_ULong     ناپ,
              FT_Error    *p_error )
  {
    FT_Error    error;
    FT_اشارہ_گر  p = ft_mem_qalloc( حافظہ, (FT_Long)ناپ, &error );


    إذا ( !error && پتہ && ناپ > 0 )
      ft_memcpy( p, پتہ, ناپ );

    *p_error = error;
    أرجع p;
  }


  FT_ادھار_DEF( FT_اشارہ_گر )
  ft_mem_strdup( FT_حافظہ    حافظہ,
                 ثابت حرف*  str,
                 FT_Error    *p_error )
  {
    FT_ULong  len = str ? (FT_ULong)ft_strlen( str ) + 1
                        : 0;


    أرجع ft_mem_dup( حافظہ, str, len, p_error );
  }


  FT_ادھار_DEF( FT_Int )
  ft_mem_strcpyn( حرف*        dst,
                  ثابت حرف*  src,
                  FT_ULong     ناپ )
  {
    طالما ( ناپ > 1 && *src != 0 )
    {
      *dst++ = *src++;
      ناپ--;
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

#تعریف_رد  FT_COMPONENT
#تعریف_کرو FT_COMPONENT  list

  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( FT_List_گرہ )
  FT_List_لبھو( FT_List  list,
                فراغ*    انکڑے )
  {
    FT_List_گرہ  cur;


    إذا ( !list )
      أرجع عدم;

    cur = list->سر;
    طالما ( cur )
    {
      إذا ( cur->انکڑے == انکڑے )
        أرجع cur;

      cur = cur->اگلا;
    }

    أرجع عدم;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( فراغ )
  FT_List_جوڑو( FT_List      list,
               FT_List_گرہ  گرہ )
  {
    FT_List_گرہ  before;


    إذا ( !list || !گرہ )
      أرجع;

    before = list->پوچھ;

    گرہ->اگلا = عدم;
    گرہ->prev = before;

    إذا ( before )
      before->اگلا = گرہ;
    وإلا
      list->سر = گرہ;

    list->پوچھ = گرہ;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( فراغ )
  FT_List_داخل_کرو( FT_List      list,
                  FT_List_گرہ  گرہ )
  {
    FT_List_گرہ  after;


    إذا ( !list || !گرہ )
      أرجع;

    after = list->سر;

    گرہ->اگلا = after;
    گرہ->prev = عدم;

    إذا ( !after )
      list->پوچھ = گرہ;
    وإلا
      after->prev = گرہ;

    list->سر = گرہ;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( فراغ )
  FT_List_Remove( FT_List      list,
                  FT_List_گرہ  گرہ )
  {
    FT_List_گرہ  before, after;


    إذا ( !list || !گرہ )
      أرجع;

    before = گرہ->prev;
    after  = گرہ->اگلا;

    إذا ( before )
      before->اگلا = after;
    وإلا
      list->سر = after;

    إذا ( after )
      after->prev = before;
    وإلا
      list->پوچھ = before;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( فراغ )
  FT_List_اتے( FT_List      list,
              FT_List_گرہ  گرہ )
  {
    FT_List_گرہ  before, after;


    إذا ( !list || !گرہ )
      أرجع;

    before = گرہ->prev;
    after  = گرہ->اگلا;

    /* check whether we are already on top of the list */
    إذا ( !before )
      أرجع;

    before->اگلا = after;

    إذا ( after )
      after->prev = before;
    وإلا
      list->پوچھ = before;

    گرہ->prev       = عدم;
    گرہ->اگلا       = list->سر;
    list->سر->prev = گرہ;
    list->سر       = گرہ;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( FT_Error )
  FT_List_Iterate( FT_List           list,
                   FT_List_Iterator  iterator,
                   فراغ*             user )
  {
    FT_List_گرہ  cur;
    FT_Error     error = FT_Err_Ok;


    إذا ( !list || !iterator )
      أرجع FT_THROW( Invalid_دلیل );

    cur = list->سر;

    طالما ( cur )
    {
      FT_List_گرہ  اگلا = cur->اگلا;


      error = iterator( cur, user );
      إذا ( error )
        اكسر;

      cur = اگلا;
    }

    أرجع error;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( فراغ )
  FT_List_Finalize( FT_List             list,
                    FT_List_Destructor  destroy,
                    FT_حافظہ           حافظہ,
                    فراغ*               user )
  {
    FT_List_گرہ  cur;


    إذا ( !list || !حافظہ )
      أرجع;

    cur = list->سر;
    طالما ( cur )
    {
      FT_List_گرہ  اگلا = cur->اگلا;


      إذا ( destroy )
        destroy( حافظہ, cur->انکڑے, user );

      FT_FREE( cur );
      cur = اگلا;
    }

    list->سر = عدم;
    list->پوچھ = عدم;
  }


/*سرا*/
