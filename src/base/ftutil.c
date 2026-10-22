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


#شامل <freetype/internal/ftdebug.h>
#شامل <freetype/internal/ftmemory.h>
#شامل <freetype/internal/ftobjs.h>
#شامل <freetype/ftlist.h>


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#تعریف_منسوخ  FT_COMPONENT
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


  FT_بنیاد_DEF( FT_اشارہ_گر )
  ft_mem_alloc( FT_حافظہ  حافظہ,
                FT_Long    سائز,
                FT_Error  *p_error )
  {
    FT_Error    error;
    FT_اشارہ_گر  بلاک = ft_mem_qalloc( حافظہ, سائز, &error );

    اگر ( !error && بلاک && سائز > 0 )
      FT_MEM_ZERO( بلاک, سائز );

    *p_error = error;
    واپس بلاک;
  }


  FT_بنیاد_DEF( FT_اشارہ_گر )
  ft_mem_qalloc( FT_حافظہ  حافظہ,
                 FT_Long    سائز,
                 FT_Error  *p_error )
  {
    FT_Error    error = FT_Err_Ok;
    FT_اشارہ_گر  بلاک = تہی;


    اگر ( سائز > 0 )
    {
      بلاک = حافظہ->alloc( حافظہ, سائز );
      اگر ( !بلاک )
        error = FT_THROW( برآمد_کا_حافظہ );
    }
    ورنہ اگر ( سائز < 0 )
    {
      /* may help catch/prevent security issues */
      error = FT_THROW( Invalid_دلیل );
    }

    *p_error = error;
    واپس بلاک;
  }


  FT_بنیاد_DEF( FT_اشارہ_گر )
  ft_mem_realloc( FT_حافظہ  حافظہ,
                  FT_Long    شے_سائز,
                  FT_Long    cur_گنتی,
                  FT_Long    new_گنتی,
                  خالی*      بلاک,
                  FT_Error  *p_error )
  {
    FT_Error  error = FT_Err_Ok;


    بلاک = ft_mem_qrealloc( حافظہ, شے_سائز,
                             cur_گنتی, new_گنتی, بلاک, &error );
    اگر ( !error && بلاک && new_گنتی > cur_گنتی )
      FT_MEM_ZERO( (حرف*)بلاک + cur_گنتی * شے_سائز,
                   ( new_گنتی - cur_گنتی ) * شے_سائز );

    *p_error = error;
    واپس بلاک;
  }


  FT_بنیاد_DEF( FT_اشارہ_گر )
  ft_mem_qrealloc( FT_حافظہ  حافظہ,
                   FT_Long    شے_سائز,
                   FT_Long    cur_گنتی,
                   FT_Long    new_گنتی,
                   خالی*      بلاک,
                   FT_Error  *p_error )
  {
    FT_Error  error = FT_Err_Ok;


    /* Note that we now accept `item_size == 0' as a valid parameter, in
     * order to cover very weird cases where an ALLOC_MULT macro would be
     * called.
     */
    اگر ( cur_گنتی < 0 || new_گنتی < 0 || شے_سائز < 0 )
    {
      /* may help catch/prevent nasty security issues */
      error = FT_THROW( Invalid_دلیل );
    }
    ورنہ اگر ( new_گنتی == 0 || شے_سائز == 0 )
    {
      ft_mem_free( حافظہ, بلاک );
      بلاک = تہی;
    }
    ورنہ اگر ( new_گنتی > FT_INT_MAX / شے_سائز )
    {
      error = FT_THROW( Array_Too_Large );
    }
    ورنہ اگر ( cur_گنتی == 0 )
    {
      FT_ASSERT( !بلاک );

      بلاک = حافظہ->alloc( حافظہ, new_گنتی * شے_سائز );
      اگر ( بلاک == تہی )
        error = FT_THROW( برآمد_کا_حافظہ );
    }
    ورنہ
    {
      FT_اشارہ_گر  block2;
      FT_Long     cur_سائز = cur_گنتی * شے_سائز;
      FT_Long     new_سائز = new_گنتی * شے_سائز;


      block2 = حافظہ->realloc( حافظہ, cur_سائز, new_سائز, بلاک );
      اگر ( !block2 )
        error = FT_THROW( برآمد_کا_حافظہ );
      ورنہ
        بلاک = block2;
    }

    *p_error = error;
    واپس بلاک;
  }


  FT_بنیاد_DEF( خالی )
  ft_mem_free( FT_حافظہ   حافظہ,
               ثابت خالی *P )
  {
    اگر ( P )
      حافظہ->آزاد( حافظہ, (خالی*)P );
  }


  FT_بنیاد_DEF( FT_اشارہ_گر )
  ft_mem_dup( FT_حافظہ    حافظہ,
              ثابت خالی*  پتہ,
              FT_ULong     سائز,
              FT_Error    *p_error )
  {
    FT_Error    error;
    FT_اشارہ_گر  p = ft_mem_qalloc( حافظہ, (FT_Long)سائز, &error );


    اگر ( !error && پتہ && سائز > 0 )
      ft_memcpy( p, پتہ, سائز );

    *p_error = error;
    واپس p;
  }


  FT_بنیاد_DEF( FT_اشارہ_گر )
  ft_mem_strdup( FT_حافظہ    حافظہ,
                 ثابت حرف*  str,
                 FT_Error    *p_error )
  {
    FT_ULong  len = str ? (FT_ULong)ft_strlen( str ) + 1
                        : 0;


    واپس ft_mem_dup( حافظہ, str, len, p_error );
  }


  FT_بنیاد_DEF( FT_Int )
  ft_mem_strcpyn( حرف*        dst,
                  ثابت حرف*  src,
                  FT_ULong     سائز )
  {
    جب ( سائز > 1 && *src != 0 )
    {
      *dst++ = *src++;
      سائز--;
    }

    *dst = 0;  /* always zero-terminate */

    واپس *src != 0;
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

#تعریف_منسوخ  FT_COMPONENT
#تعریف_کرو FT_COMPONENT  list

  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( FT_List_گرہ )
  FT_List_ڈھونڈو( FT_List  list,
                خالی*    ڈیٹا )
  {
    FT_List_گرہ  cur;


    اگر ( !list )
      واپس تہی;

    cur = list->سر;
    جب ( cur )
    {
      اگر ( cur->ڈیٹا == ڈیٹا )
        واپس cur;

      cur = cur->اگلا;
    }

    واپس تہی;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( خالی )
  FT_List_جوڑو( FT_List      list,
               FT_List_گرہ  گرہ )
  {
    FT_List_گرہ  before;


    اگر ( !list || !گرہ )
      واپس;

    before = list->دم;

    گرہ->اگلا = تہی;
    گرہ->پچھلا = before;

    اگر ( before )
      before->اگلا = گرہ;
    ورنہ
      list->سر = گرہ;

    list->دم = گرہ;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( خالی )
  FT_List_داخل_کرو( FT_List      list,
                  FT_List_گرہ  گرہ )
  {
    FT_List_گرہ  after;


    اگر ( !list || !گرہ )
      واپس;

    after = list->سر;

    گرہ->اگلا = after;
    گرہ->پچھلا = تہی;

    اگر ( !after )
      list->دم = گرہ;
    ورنہ
      after->پچھلا = گرہ;

    list->سر = گرہ;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( خالی )
  FT_List_Remove( FT_List      list,
                  FT_List_گرہ  گرہ )
  {
    FT_List_گرہ  before, after;


    اگر ( !list || !گرہ )
      واپس;

    before = گرہ->پچھلا;
    after  = گرہ->اگلا;

    اگر ( before )
      before->اگلا = after;
    ورنہ
      list->سر = after;

    اگر ( after )
      after->پچھلا = before;
    ورنہ
      list->دم = before;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( خالی )
  FT_List_اوپر( FT_List      list,
              FT_List_گرہ  گرہ )
  {
    FT_List_گرہ  before, after;


    اگر ( !list || !گرہ )
      واپس;

    before = گرہ->پچھلا;
    after  = گرہ->اگلا;

    /* check whether we are already on top of the list */
    اگر ( !before )
      واپس;

    before->اگلا = after;

    اگر ( after )
      after->پچھلا = before;
    ورنہ
      list->دم = before;

    گرہ->پچھلا       = تہی;
    گرہ->اگلا       = list->سر;
    list->سر->پچھلا = گرہ;
    list->سر       = گرہ;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( FT_Error )
  FT_List_Iterate( FT_List           list,
                   FT_List_Iterator  iterator,
                   خالی*             user )
  {
    FT_List_گرہ  cur;
    FT_Error     error = FT_Err_Ok;


    اگر ( !list || !iterator )
      واپس FT_THROW( Invalid_دلیل );

    cur = list->سر;

    جب ( cur )
    {
      FT_List_گرہ  اگلا = cur->اگلا;


      error = iterator( cur, user );
      اگر ( error )
        توڑو;

      cur = اگلا;
    }

    واپس error;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( خالی )
  FT_List_Finalize( FT_List             list,
                    FT_List_Destructor  destroy,
                    FT_حافظہ           حافظہ,
                    خالی*               user )
  {
    FT_List_گرہ  cur;


    اگر ( !list || !حافظہ )
      واپس;

    cur = list->سر;
    جب ( cur )
    {
      FT_List_گرہ  اگلا = cur->اگلا;


      اگر ( destroy )
        destroy( حافظہ, cur->ڈیٹا, user );

      FT_FREE( cur );
      cur = اگلا;
    }

    list->سر = تہی;
    list->دم = تہی;
  }


/*انتہا*/
