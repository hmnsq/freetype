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
#अपरिभाषय  FT_COMPONENT
#परिभाषय FT_COMPONENT  स्मृतिः


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


  FT_आधारः_DEF( FT_Pointer )
  ft_mem_alloc( FT_स्मृतिः  स्मृतिः,
                FT_Long    आकार,
                FT_Error  *p_error )
  {
    FT_Error    error;
    FT_Pointer  खण्डम् = ft_mem_qalloc( स्मृतिः, आकार, &error );

    यदि ( !error && खण्डम् && आकार > 0 )
      FT_MEM_ZERO( खण्डम्, आकार );

    *p_error = error;
    प्रत्यागम खण्डम्;
  }


  FT_आधारः_DEF( FT_Pointer )
  ft_mem_qalloc( FT_स्मृतिः  स्मृतिः,
                 FT_Long    आकार,
                 FT_Error  *p_error )
  {
    FT_Error    error = FT_Err_Ok;
    FT_Pointer  खण्डम् = रिक्त;


    यदि ( आकार > 0 )
    {
      खण्डम् = स्मृतिः->alloc( स्मृतिः, आकार );
      यदि ( !खण्डम् )
        error = FT_THROW( बहिर्गमनम्_सम्बन्धि_स्मृतिः );
    }
    अन्यथा यदि ( आकार < 0 )
    {
      /* may help catch/prevent security issues */
      error = FT_THROW( Invalid_तर्कः );
    }

    *p_error = error;
    प्रत्यागम खण्डम्;
  }


  FT_आधारः_DEF( FT_Pointer )
  ft_mem_realloc( FT_स्मृतिः  स्मृतिः,
                  FT_Long    item_आकार,
                  FT_Long    cur_गणनम्,
                  FT_Long    new_गणनम्,
                  शून्य*      खण्डम्,
                  FT_Error  *p_error )
  {
    FT_Error  error = FT_Err_Ok;


    खण्डम् = ft_mem_qrealloc( स्मृतिः, item_आकार,
                             cur_गणनम्, new_गणनम्, खण्डम्, &error );
    यदि ( !error && खण्डम् && new_गणनम् > cur_गणनम् )
      FT_MEM_ZERO( (वर्ण*)खण्डम् + cur_गणनम् * item_आकार,
                   ( new_गणनम् - cur_गणनम् ) * item_आकार );

    *p_error = error;
    प्रत्यागम खण्डम्;
  }


  FT_आधारः_DEF( FT_Pointer )
  ft_mem_qrealloc( FT_स्मृतिः  स्मृतिः,
                   FT_Long    item_आकार,
                   FT_Long    cur_गणनम्,
                   FT_Long    new_गणनम्,
                   शून्य*      खण्डम्,
                   FT_Error  *p_error )
  {
    FT_Error  error = FT_Err_Ok;


    /* Note that we now accept `item_size == 0' as a valid parameter, in
     * order to cover very weird cases where an ALLOC_MULT macro would be
     * called.
     */
    यदि ( cur_गणनम् < 0 || new_गणनम् < 0 || item_आकार < 0 )
    {
      /* may help catch/prevent nasty security issues */
      error = FT_THROW( Invalid_तर्कः );
    }
    अन्यथा यदि ( new_गणनम् == 0 || item_आकार == 0 )
    {
      ft_mem_free( स्मृतिः, खण्डम् );
      खण्डम् = रिक्त;
    }
    अन्यथा यदि ( new_गणनम् > FT_INT_MAX / item_आकार )
    {
      error = FT_THROW( Array_Too_Large );
    }
    अन्यथा यदि ( cur_गणनम् == 0 )
    {
      FT_ASSERT( !खण्डम् );

      खण्डम् = स्मृतिः->alloc( स्मृतिः, new_गणनम् * item_आकार );
      यदि ( खण्डम् == रिक्त )
        error = FT_THROW( बहिर्गमनम्_सम्बन्धि_स्मृतिः );
    }
    अन्यथा
    {
      FT_Pointer  block2;
      FT_Long     cur_आकार = cur_गणनम् * item_आकार;
      FT_Long     new_आकार = new_गणनम् * item_आकार;


      block2 = स्मृतिः->realloc( स्मृतिः, cur_आकार, new_आकार, खण्डम् );
      यदि ( !block2 )
        error = FT_THROW( बहिर्गमनम्_सम्बन्धि_स्मृतिः );
      अन्यथा
        खण्डम् = block2;
    }

    *p_error = error;
    प्रत्यागम खण्डम्;
  }


  FT_आधारः_DEF( शून्य )
  ft_mem_free( FT_स्मृतिः   स्मृतिः,
               ध्रुव शून्य *P )
  {
    यदि ( P )
      स्मृतिः->मुक्त( स्मृतिः, (शून्य*)P );
  }


  FT_आधारः_DEF( FT_Pointer )
  ft_mem_dup( FT_स्मृतिः    स्मृतिः,
              ध्रुव शून्य*  स्थानाङ्कः,
              FT_ULong     आकार,
              FT_Error    *p_error )
  {
    FT_Error    error;
    FT_Pointer  p = ft_mem_qalloc( स्मृतिः, (FT_Long)आकार, &error );


    यदि ( !error && स्थानाङ्कः && आकार > 0 )
      ft_memcpy( p, स्थानाङ्कः, आकार );

    *p_error = error;
    प्रत्यागम p;
  }


  FT_आधारः_DEF( FT_Pointer )
  ft_mem_strdup( FT_स्मृतिः    स्मृतिः,
                 ध्रुव वर्ण*  str,
                 FT_Error    *p_error )
  {
    FT_ULong  len = str ? (FT_ULong)ft_strlen( str ) + 1
                        : 0;


    प्रत्यागम ft_mem_dup( स्मृतिः, str, len, p_error );
  }


  FT_आधारः_DEF( FT_Int )
  ft_mem_strcpyn( वर्ण*        dst,
                  ध्रुव वर्ण*  src,
                  FT_ULong     आकार )
  {
    यावत् ( आकार > 1 && *src != 0 )
    {
      *dst++ = *src++;
      आकार--;
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

#अपरिभाषय  FT_COMPONENT
#परिभाषय FT_COMPONENT  list

  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( FT_List_ग्रन्थिः )
  FT_List_Find( FT_List  list,
                शून्य*    दत्त )
  {
    FT_List_ग्रन्थिः  cur;


    यदि ( !list )
      प्रत्यागम रिक्त;

    cur = list->शीर्ष;
    यावत् ( cur )
    {
      यदि ( cur->दत्त == दत्त )
        प्रत्यागम cur;

      cur = cur->अग्र;
    }

    प्रत्यागम रिक्त;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( शून्य )
  FT_List_सङ्कलय( FT_List      list,
               FT_List_ग्रन्थिः  ग्रन्थिः )
  {
    FT_List_ग्रन्थिः  before;


    यदि ( !list || !ग्रन्थिः )
      प्रत्यागम;

    before = list->पुच्छ;

    ग्रन्थिः->अग्र = रिक्त;
    ग्रन्थिः->पूर्व = before;

    यदि ( before )
      before->अग्र = ग्रन्थिः;
    अन्यथा
      list->शीर्ष = ग्रन्थिः;

    list->पुच्छ = ग्रन्थिः;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( शून्य )
  FT_List_निवेशय( FT_List      list,
                  FT_List_ग्रन्थिः  ग्रन्थिः )
  {
    FT_List_ग्रन्थिः  after;


    यदि ( !list || !ग्रन्थिः )
      प्रत्यागम;

    after = list->शीर्ष;

    ग्रन्थिः->अग्र = after;
    ग्रन्थिः->पूर्व = रिक्त;

    यदि ( !after )
      list->पुच्छ = ग्रन्थिः;
    अन्यथा
      after->पूर्व = ग्रन्थिः;

    list->शीर्ष = ग्रन्थिः;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( शून्य )
  FT_List_Remove( FT_List      list,
                  FT_List_ग्रन्थिः  ग्रन्थिः )
  {
    FT_List_ग्रन्थिः  before, after;


    यदि ( !list || !ग्रन्थिः )
      प्रत्यागम;

    before = ग्रन्थिः->पूर्व;
    after  = ग्रन्थिः->अग्र;

    यदि ( before )
      before->अग्र = after;
    अन्यथा
      list->शीर्ष = after;

    यदि ( after )
      after->पूर्व = before;
    अन्यथा
      list->पुच्छ = before;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( शून्य )
  FT_List_ऊर्ध्वम्( FT_List      list,
              FT_List_ग्रन्थिः  ग्रन्थिः )
  {
    FT_List_ग्रन्थिः  before, after;


    यदि ( !list || !ग्रन्थिः )
      प्रत्यागम;

    before = ग्रन्थिः->पूर्व;
    after  = ग्रन्थिः->अग्र;

    /* check whether we are already on top of the list */
    यदि ( !before )
      प्रत्यागम;

    before->अग्र = after;

    यदि ( after )
      after->पूर्व = before;
    अन्यथा
      list->पुच्छ = before;

    ग्रन्थिः->पूर्व       = रिक्त;
    ग्रन्थिः->अग्र       = list->शीर्ष;
    list->शीर्ष->पूर्व = ग्रन्थिः;
    list->शीर्ष       = ग्रन्थिः;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( FT_Error )
  FT_List_Iterate( FT_List           list,
                   FT_List_Iterator  iterator,
                   शून्य*             user )
  {
    FT_List_ग्रन्थिः  cur;
    FT_Error     error = FT_Err_Ok;


    यदि ( !list || !iterator )
      प्रत्यागम FT_THROW( Invalid_तर्कः );

    cur = list->शीर्ष;

    यावत् ( cur )
    {
      FT_List_ग्रन्थिः  अग्र = cur->अग्र;


      error = iterator( cur, user );
      यदि ( error )
        भङ्ग;

      cur = अग्र;
    }

    प्रत्यागम error;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( शून्य )
  FT_List_Finalize( FT_List             list,
                    FT_List_Destructor  destroy,
                    FT_स्मृतिः           स्मृतिः,
                    शून्य*               user )
  {
    FT_List_ग्रन्थिः  cur;


    यदि ( !list || !स्मृतिः )
      प्रत्यागम;

    cur = list->शीर्ष;
    यावत् ( cur )
    {
      FT_List_ग्रन्थिः  अग्र = cur->अग्र;


      यदि ( destroy )
        destroy( स्मृतिः, cur->दत्त, user );

      FT_FREE( cur );
      cur = अग्र;
    }

    list->शीर्ष = रिक्त;
    list->पुच्छ = रिक्त;
  }


/*अन्तः*/
