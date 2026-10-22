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


#অন্তর্ভাব <freetype/internal/ftdebug.h>
#অন্তর্ভাব <freetype/internal/ftmemory.h>
#অন্তর্ভাব <freetype/internal/ftobjs.h>
#অন্তর্ভাব <freetype/ftlist.h>


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
                FT_Long    আকার_2,
                FT_Error  *p_error )
  {
    FT_Error    error;
    FT_Pointer  block = ft_mem_qalloc( memory, আকার_2, &error );

    যদি ( !error && block && আকার_2 > 0 )
      FT_MEM_ZERO( block, আকার_2 );

    *p_error = error;
    ফেরত block;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_qalloc( FT_Memory  memory,
                 FT_Long    আকার_2,
                 FT_Error  *p_error )
  {
    FT_Error    error = FT_Err_Ok;
    FT_Pointer  block = ফাঁকা;


    যদি ( আকার_2 > 0 )
    {
      block = memory->alloc( memory, আকার_2 );
      যদি ( !block )
        error = FT_THROW( Out_Of_Memory );
    }
    অন্যথা যদি ( আকার_2 < 0 )
    {
      /* may help catch/prevent security issues */
      error = FT_THROW( Invalid_Argument );
    }

    *p_error = error;
    ফেরত block;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_realloc( FT_Memory  memory,
                  FT_Long    item_আকার,
                  FT_Long    cur_গুনতি,
                  FT_Long    new_গুনতি,
                  শূন্য*      block,
                  FT_Error  *p_error )
  {
    FT_Error  error = FT_Err_Ok;


    block = ft_mem_qrealloc( memory, item_আকার,
                             cur_গুনতি, new_গুনতি, block, &error );
    যদি ( !error && block && new_গুনতি > cur_গুনতি )
      FT_MEM_ZERO( (অক্ষর*)block + cur_গুনতি * item_আকার,
                   ( new_গুনতি - cur_গুনতি ) * item_আকার );

    *p_error = error;
    ফেরত block;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_qrealloc( FT_Memory  memory,
                   FT_Long    item_আকার,
                   FT_Long    cur_গুনতি,
                   FT_Long    new_গুনতি,
                   শূন্য*      block,
                   FT_Error  *p_error )
  {
    FT_Error  error = FT_Err_Ok;


    /* Note that we now accept `item_size == 0' as a valid parameter, in
     * order to cover very weird cases where an ALLOC_MULT macro would be
     * called.
     */
    যদি ( cur_গুনতি < 0 || new_গুনতি < 0 || item_আকার < 0 )
    {
      /* may help catch/prevent nasty security issues */
      error = FT_THROW( Invalid_Argument );
    }
    অন্যথা যদি ( new_গুনতি == 0 || item_আকার == 0 )
    {
      ft_mem_free( memory, block );
      block = ফাঁকা;
    }
    অন্যথা যদি ( new_গুনতি > FT_INT_MAX / item_আকার )
    {
      error = FT_THROW( Array_Too_Large );
    }
    অন্যথা যদি ( cur_গুনতি == 0 )
    {
      FT_ASSERT( !block );

      block = memory->alloc( memory, new_গুনতি * item_আকার );
      যদি ( block == ফাঁকা )
        error = FT_THROW( Out_Of_Memory );
    }
    অন্যথা
    {
      FT_Pointer  block2;
      FT_Long     cur_আকার = cur_গুনতি * item_আকার;
      FT_Long     new_আকার = new_গুনতি * item_আকার;


      block2 = memory->realloc( memory, cur_আকার, new_আকার, block );
      যদি ( !block2 )
        error = FT_THROW( Out_Of_Memory );
      অন্যথা
        block = block2;
    }

    *p_error = error;
    ফেরত block;
  }


  FT_BASE_DEF( শূন্য )
  ft_mem_free( FT_Memory   memory,
               ধ্রুবক শূন্য *P )
  {
    যদি ( P )
      memory->মুক্ত( memory, (শূন্য*)P );
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_dup( FT_Memory    memory,
              ধ্রুবক শূন্য*  address,
              FT_ULong     আকার_2,
              FT_Error    *p_error )
  {
    FT_Error    error;
    FT_Pointer  p = ft_mem_qalloc( memory, (FT_Long)আকার_2, &error );


    যদি ( !error && address && আকার_2 > 0 )
      ft_memcpy( p, address, আকার_2 );

    *p_error = error;
    ফেরত p;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_strdup( FT_Memory    memory,
                 ধ্রুবক অক্ষর*  str,
                 FT_Error    *p_error )
  {
    FT_ULong  len = str ? (FT_ULong)ft_strlen( str ) + 1
                        : 0;


    ফেরত ft_mem_dup( memory, str, len, p_error );
  }


  FT_BASE_DEF( FT_Int )
  ft_mem_strcpyn( অক্ষর*        dst,
                  ধ্রুবক অক্ষর*  src,
                  FT_ULong     আকার_2 )
  {
    যখন ( আকার_2 > 1 && *src != 0 )
    {
      *dst++ = *src++;
      আকার_2--;
    }

    *dst = 0;  /* always zero-terminate */

    ফেরত *src != 0;
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

  FT_EXPORT_DEF( FT_List_গ্রন্থি )
  FT_List_Find( FT_List  list,
                শূন্য*    তথ্য )
  {
    FT_List_গ্রন্থি  cur;


    যদি ( !list )
      ফেরত ফাঁকা;

    cur = list->শীর্ষ_2;
    যখন ( cur )
    {
      যদি ( cur->তথ্য == তথ্য )
        ফেরত cur;

      cur = cur->পরবর্তী;
    }

    ফেরত ফাঁকা;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( শূন্য )
  FT_List_Add( FT_List      list,
               FT_List_গ্রন্থি  গ্রন্থি )
  {
    FT_List_গ্রন্থি  before;


    যদি ( !list || !গ্রন্থি )
      ফেরত;

    before = list->লেজ;

    গ্রন্থি->পরবর্তী = ফাঁকা;
    গ্রন্থি->পূর্ব = before;

    যদি ( before )
      before->পরবর্তী = গ্রন্থি;
    অন্যথা
      list->শীর্ষ_2 = গ্রন্থি;

    list->লেজ = গ্রন্থি;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( শূন্য )
  FT_List_ঢোকাও( FT_List      list,
                  FT_List_গ্রন্থি  গ্রন্থি )
  {
    FT_List_গ্রন্থি  after;


    যদি ( !list || !গ্রন্থি )
      ফেরত;

    after = list->শীর্ষ_2;

    গ্রন্থি->পরবর্তী = after;
    গ্রন্থি->পূর্ব = ফাঁকা;

    যদি ( !after )
      list->লেজ = গ্রন্থি;
    অন্যথা
      after->পূর্ব = গ্রন্থি;

    list->শীর্ষ_2 = গ্রন্থি;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( শূন্য )
  FT_List_Remove( FT_List      list,
                  FT_List_গ্রন্থি  গ্রন্থি )
  {
    FT_List_গ্রন্থি  before, after;


    যদি ( !list || !গ্রন্থি )
      ফেরত;

    before = গ্রন্থি->পূর্ব;
    after  = গ্রন্থি->পরবর্তী;

    যদি ( before )
      before->পরবর্তী = after;
    অন্যথা
      list->শীর্ষ_2 = after;

    যদি ( after )
      after->পূর্ব = before;
    অন্যথা
      list->লেজ = before;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( শূন্য )
  FT_List_Up( FT_List      list,
              FT_List_গ্রন্থি  গ্রন্থি )
  {
    FT_List_গ্রন্থি  before, after;


    যদি ( !list || !গ্রন্থি )
      ফেরত;

    before = গ্রন্থি->পূর্ব;
    after  = গ্রন্থি->পরবর্তী;

    /* check whether we are already on top of the list */
    যদি ( !before )
      ফেরত;

    before->পরবর্তী = after;

    যদি ( after )
      after->পূর্ব = before;
    অন্যথা
      list->লেজ = before;

    গ্রন্থি->পূর্ব       = ফাঁকা;
    গ্রন্থি->পরবর্তী       = list->শীর্ষ_2;
    list->শীর্ষ_2->পূর্ব = গ্রন্থি;
    list->শীর্ষ_2       = গ্রন্থি;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( FT_Error )
  FT_List_Iterate( FT_List           list,
                   FT_List_Iterator  iterator,
                   শূন্য*             user )
  {
    FT_List_গ্রন্থি  cur;
    FT_Error     error = FT_Err_Ok;


    যদি ( !list || !iterator )
      ফেরত FT_THROW( Invalid_Argument );

    cur = list->শীর্ষ_2;

    যখন ( cur )
    {
      FT_List_গ্রন্থি  পরবর্তী = cur->পরবর্তী;


      error = iterator( cur, user );
      যদি ( error )
        বিরতি;

      cur = পরবর্তী;
    }

    ফেরত error;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( শূন্য )
  FT_List_Finalize( FT_List             list,
                    FT_List_Destructor  destroy,
                    FT_Memory           memory,
                    শূন্য*               user )
  {
    FT_List_গ্রন্থি  cur;


    যদি ( !list || !memory )
      ফেরত;

    cur = list->শীর্ষ_2;
    যখন ( cur )
    {
      FT_List_গ্রন্থি  পরবর্তী = cur->পরবর্তী;


      যদি ( destroy )
        destroy( memory, cur->তথ্য, user );

      FT_FREE( cur );
      cur = পরবর্তী;
    }

    list->শীর্ষ_2 = ফাঁকা;
    list->লেজ = ফাঁকা;
  }


/* END */
