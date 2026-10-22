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


#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftdebug.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftmemory.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftobjs.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/ftlist.h>


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ  FT_COMPONENT
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FT_COMPONENT  ਸਮ੍ਰਿਤੀ


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


  FT_ਅਧਾਰ_DEF( FT_ਸੂਚਕ )
  ft_mem_alloc( FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ,
                FT_Long    ਮਾਪ,
                FT_Error  *p_error )
  {
    FT_Error    error;
    FT_ਸੂਚਕ  ਖੰਡ = ft_mem_qalloc( ਸਮ੍ਰਿਤੀ, ਮਾਪ, &error );

    ਜੇ ( !error && ਖੰਡ && ਮਾਪ > 0 )
      FT_MEM_ZERO( ਖੰਡ, ਮਾਪ );

    *p_error = error;
    ਵਾਪਸ ਖੰਡ;
  }


  FT_ਅਧਾਰ_DEF( FT_ਸੂਚਕ )
  ft_mem_qalloc( FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ,
                 FT_Long    ਮਾਪ,
                 FT_Error  *p_error )
  {
    FT_Error    error = FT_Err_Ok;
    FT_ਸੂਚਕ  ਖੰਡ = ਨਿਰਾ;


    ਜੇ ( ਮਾਪ > 0 )
    {
      ਖੰਡ = ਸਮ੍ਰਿਤੀ->alloc( ਸਮ੍ਰਿਤੀ, ਮਾਪ );
      ਜੇ ( !ਖੰਡ )
        error = FT_THROW( ਬਾਹਰਲਾ_ਦਾ_ਸਮ੍ਰਿਤੀ );
    }
    ਨਹੀਂਤਾਂ ਜੇ ( ਮਾਪ < 0 )
    {
      /* may help catch/prevent security issues */
      error = FT_THROW( Invalid_ਦਲੀਲ );
    }

    *p_error = error;
    ਵਾਪਸ ਖੰਡ;
  }


  FT_ਅਧਾਰ_DEF( FT_ਸੂਚਕ )
  ft_mem_realloc( FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ,
                  FT_Long    ਵਸਤੂ_ਮਾਪ,
                  FT_Long    cur_ਗਿਣਤੀ,
                  FT_Long    new_ਗਿਣਤੀ,
                  ਖਾਲੀ*      ਖੰਡ,
                  FT_Error  *p_error )
  {
    FT_Error  error = FT_Err_Ok;


    ਖੰਡ = ft_mem_qrealloc( ਸਮ੍ਰਿਤੀ, ਵਸਤੂ_ਮਾਪ,
                             cur_ਗਿਣਤੀ, new_ਗਿਣਤੀ, ਖੰਡ, &error );
    ਜੇ ( !error && ਖੰਡ && new_ਗਿਣਤੀ > cur_ਗਿਣਤੀ )
      FT_MEM_ZERO( (ਅੱਖਰ*)ਖੰਡ + cur_ਗਿਣਤੀ * ਵਸਤੂ_ਮਾਪ,
                   ( new_ਗਿਣਤੀ - cur_ਗਿਣਤੀ ) * ਵਸਤੂ_ਮਾਪ );

    *p_error = error;
    ਵਾਪਸ ਖੰਡ;
  }


  FT_ਅਧਾਰ_DEF( FT_ਸੂਚਕ )
  ft_mem_qrealloc( FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ,
                   FT_Long    ਵਸਤੂ_ਮਾਪ,
                   FT_Long    cur_ਗਿਣਤੀ,
                   FT_Long    new_ਗਿਣਤੀ,
                   ਖਾਲੀ*      ਖੰਡ,
                   FT_Error  *p_error )
  {
    FT_Error  error = FT_Err_Ok;


    /* Note that we now accept `item_size == 0' as a valid parameter, in
     * order to cover very weird cases where an ALLOC_MULT macro would be
     * called.
     */
    ਜੇ ( cur_ਗਿਣਤੀ < 0 || new_ਗਿਣਤੀ < 0 || ਵਸਤੂ_ਮਾਪ < 0 )
    {
      /* may help catch/prevent nasty security issues */
      error = FT_THROW( Invalid_ਦਲੀਲ );
    }
    ਨਹੀਂਤਾਂ ਜੇ ( new_ਗਿਣਤੀ == 0 || ਵਸਤੂ_ਮਾਪ == 0 )
    {
      ft_mem_free( ਸਮ੍ਰਿਤੀ, ਖੰਡ );
      ਖੰਡ = ਨਿਰਾ;
    }
    ਨਹੀਂਤਾਂ ਜੇ ( new_ਗਿਣਤੀ > FT_INT_MAX / ਵਸਤੂ_ਮਾਪ )
    {
      error = FT_THROW( Array_Too_Large );
    }
    ਨਹੀਂਤਾਂ ਜੇ ( cur_ਗਿਣਤੀ == 0 )
    {
      FT_ASSERT( !ਖੰਡ );

      ਖੰਡ = ਸਮ੍ਰਿਤੀ->alloc( ਸਮ੍ਰਿਤੀ, new_ਗਿਣਤੀ * ਵਸਤੂ_ਮਾਪ );
      ਜੇ ( ਖੰਡ == ਨਿਰਾ )
        error = FT_THROW( ਬਾਹਰਲਾ_ਦਾ_ਸਮ੍ਰਿਤੀ );
    }
    ਨਹੀਂਤਾਂ
    {
      FT_ਸੂਚਕ  block2;
      FT_Long     cur_ਮਾਪ = cur_ਗਿਣਤੀ * ਵਸਤੂ_ਮਾਪ;
      FT_Long     new_ਮਾਪ = new_ਗਿਣਤੀ * ਵਸਤੂ_ਮਾਪ;


      block2 = ਸਮ੍ਰਿਤੀ->realloc( ਸਮ੍ਰਿਤੀ, cur_ਮਾਪ, new_ਮਾਪ, ਖੰਡ );
      ਜੇ ( !block2 )
        error = FT_THROW( ਬਾਹਰਲਾ_ਦਾ_ਸਮ੍ਰਿਤੀ );
      ਨਹੀਂਤਾਂ
        ਖੰਡ = block2;
    }

    *p_error = error;
    ਵਾਪਸ ਖੰਡ;
  }


  FT_ਅਧਾਰ_DEF( ਖਾਲੀ )
  ft_mem_free( FT_ਸਮ੍ਰਿਤੀ   ਸਮ੍ਰਿਤੀ,
               ਅਚਲ ਖਾਲੀ *P )
  {
    ਜੇ ( P )
      ਸਮ੍ਰਿਤੀ->ਮੁਕਤ( ਸਮ੍ਰਿਤੀ, (ਖਾਲੀ*)P );
  }


  FT_ਅਧਾਰ_DEF( FT_ਸੂਚਕ )
  ft_mem_dup( FT_ਸਮ੍ਰਿਤੀ    ਸਮ੍ਰਿਤੀ,
              ਅਚਲ ਖਾਲੀ*  ਪਤਾ,
              FT_ULong     ਮਾਪ,
              FT_Error    *p_error )
  {
    FT_Error    error;
    FT_ਸੂਚਕ  p = ft_mem_qalloc( ਸਮ੍ਰਿਤੀ, (FT_Long)ਮਾਪ, &error );


    ਜੇ ( !error && ਪਤਾ && ਮਾਪ > 0 )
      ft_memcpy( p, ਪਤਾ, ਮਾਪ );

    *p_error = error;
    ਵਾਪਸ p;
  }


  FT_ਅਧਾਰ_DEF( FT_ਸੂਚਕ )
  ft_mem_strdup( FT_ਸਮ੍ਰਿਤੀ    ਸਮ੍ਰਿਤੀ,
                 ਅਚਲ ਅੱਖਰ*  str,
                 FT_Error    *p_error )
  {
    FT_ULong  len = str ? (FT_ULong)ft_strlen( str ) + 1
                        : 0;


    ਵਾਪਸ ft_mem_dup( ਸਮ੍ਰਿਤੀ, str, len, p_error );
  }


  FT_ਅਧਾਰ_DEF( FT_Int )
  ft_mem_strcpyn( ਅੱਖਰ*        dst,
                  ਅਚਲ ਅੱਖਰ*  src,
                  FT_ULong     ਮਾਪ )
  {
    ਜਦੋਂ ( ਮਾਪ > 1 && *src != 0 )
    {
      *dst++ = *src++;
      ਮਾਪ--;
    }

    *dst = 0;  /* always zero-terminate */

    ਵਾਪਸ *src != 0;
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

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ  FT_COMPONENT
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FT_COMPONENT  list

  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( FT_List_ਗੰਢ )
  FT_List_ਲੱਭੋ( FT_List  list,
                ਖਾਲੀ*    ਅੰਕੜੇ )
  {
    FT_List_ਗੰਢ  cur;


    ਜੇ ( !list )
      ਵਾਪਸ ਨਿਰਾ;

    cur = list->ਸਿਰ;
    ਜਦੋਂ ( cur )
    {
      ਜੇ ( cur->ਅੰਕੜੇ == ਅੰਕੜੇ )
        ਵਾਪਸ cur;

      cur = cur->ਅਗਲਾ;
    }

    ਵਾਪਸ ਨਿਰਾ;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( ਖਾਲੀ )
  FT_List_ਜੋੜੋ( FT_List      list,
               FT_List_ਗੰਢ  ਗੰਢ )
  {
    FT_List_ਗੰਢ  before;


    ਜੇ ( !list || !ਗੰਢ )
      ਵਾਪਸ;

    before = list->ਪੂਛ;

    ਗੰਢ->ਅਗਲਾ = ਨਿਰਾ;
    ਗੰਢ->prev = before;

    ਜੇ ( before )
      before->ਅਗਲਾ = ਗੰਢ;
    ਨਹੀਂਤਾਂ
      list->ਸਿਰ = ਗੰਢ;

    list->ਪੂਛ = ਗੰਢ;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( ਖਾਲੀ )
  FT_List_ਪਾਓ( FT_List      list,
                  FT_List_ਗੰਢ  ਗੰਢ )
  {
    FT_List_ਗੰਢ  after;


    ਜੇ ( !list || !ਗੰਢ )
      ਵਾਪਸ;

    after = list->ਸਿਰ;

    ਗੰਢ->ਅਗਲਾ = after;
    ਗੰਢ->prev = ਨਿਰਾ;

    ਜੇ ( !after )
      list->ਪੂਛ = ਗੰਢ;
    ਨਹੀਂਤਾਂ
      after->prev = ਗੰਢ;

    list->ਸਿਰ = ਗੰਢ;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( ਖਾਲੀ )
  FT_List_Remove( FT_List      list,
                  FT_List_ਗੰਢ  ਗੰਢ )
  {
    FT_List_ਗੰਢ  before, after;


    ਜੇ ( !list || !ਗੰਢ )
      ਵਾਪਸ;

    before = ਗੰਢ->prev;
    after  = ਗੰਢ->ਅਗਲਾ;

    ਜੇ ( before )
      before->ਅਗਲਾ = after;
    ਨਹੀਂਤਾਂ
      list->ਸਿਰ = after;

    ਜੇ ( after )
      after->prev = before;
    ਨਹੀਂਤਾਂ
      list->ਪੂਛ = before;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( ਖਾਲੀ )
  FT_List_ਉੱਪਰ( FT_List      list,
              FT_List_ਗੰਢ  ਗੰਢ )
  {
    FT_List_ਗੰਢ  before, after;


    ਜੇ ( !list || !ਗੰਢ )
      ਵਾਪਸ;

    before = ਗੰਢ->prev;
    after  = ਗੰਢ->ਅਗਲਾ;

    /* check whether we are already on top of the list */
    ਜੇ ( !before )
      ਵਾਪਸ;

    before->ਅਗਲਾ = after;

    ਜੇ ( after )
      after->prev = before;
    ਨਹੀਂਤਾਂ
      list->ਪੂਛ = before;

    ਗੰਢ->prev       = ਨਿਰਾ;
    ਗੰਢ->ਅਗਲਾ       = list->ਸਿਰ;
    list->ਸਿਰ->prev = ਗੰਢ;
    list->ਸਿਰ       = ਗੰਢ;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( FT_Error )
  FT_List_Iterate( FT_List           list,
                   FT_List_Iterator  iterator,
                   ਖਾਲੀ*             user )
  {
    FT_List_ਗੰਢ  cur;
    FT_Error     error = FT_Err_Ok;


    ਜੇ ( !list || !iterator )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    cur = list->ਸਿਰ;

    ਜਦੋਂ ( cur )
    {
      FT_List_ਗੰਢ  ਅਗਲਾ = cur->ਅਗਲਾ;


      error = iterator( cur, user );
      ਜੇ ( error )
        ਰੋਕੋ;

      cur = ਅਗਲਾ;
    }

    ਵਾਪਸ error;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( ਖਾਲੀ )
  FT_List_Finalize( FT_List             list,
                    FT_List_Destructor  destroy,
                    FT_ਸਮ੍ਰਿਤੀ           ਸਮ੍ਰਿਤੀ,
                    ਖਾਲੀ*               user )
  {
    FT_List_ਗੰਢ  cur;


    ਜੇ ( !list || !ਸਮ੍ਰਿਤੀ )
      ਵਾਪਸ;

    cur = list->ਸਿਰ;
    ਜਦੋਂ ( cur )
    {
      FT_List_ਗੰਢ  ਅਗਲਾ = cur->ਅਗਲਾ;


      ਜੇ ( destroy )
        destroy( ਸਮ੍ਰਿਤੀ, cur->ਅੰਕੜੇ, user );

      FT_FREE( cur );
      cur = ਅਗਲਾ;
    }

    list->ਸਿਰ = ਨਿਰਾ;
    list->ਪੂਛ = ਨਿਰਾ;
  }


/*ਸਿਰਾ*/
