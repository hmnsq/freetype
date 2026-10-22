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


#समावेश <freetype/internal/ftdebug.h>
#समावेश <freetype/internal/ftmemory.h>
#समावेश <freetype/internal/ftobjs.h>
#समावेश <freetype/ftlist.h>


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#अघोषित  FT_COMPONENT
#घोषणा FT_COMPONENT  memory


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
                FT_Long    आकार,
                FT_Error  *p_error )
  {
    FT_Error    error;
    FT_Pointer  block = ft_mem_qalloc( memory, आकार, &error );

    अगर ( !error && block && आकार > 0 )
      FT_MEM_ZERO( block, आकार );

    *p_error = error;
    वापस block;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_qalloc( FT_Memory  memory,
                 FT_Long    आकार,
                 FT_Error  *p_error )
  {
    FT_Error    error = FT_Err_Ok;
    FT_Pointer  block = शून्य;


    अगर ( आकार > 0 )
    {
      block = memory->alloc( memory, आकार );
      अगर ( !block )
        error = FT_THROW( Out_Of_Memory );
    }
    अन्यथा अगर ( आकार < 0 )
    {
      /* may help catch/prevent security issues */
      error = FT_THROW( Invalid_Argument );
    }

    *p_error = error;
    वापस block;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_realloc( FT_Memory  memory,
                  FT_Long    item_आकार,
                  FT_Long    cur_गिनती,
                  FT_Long    new_गिनती,
                  व्योम*      block,
                  FT_Error  *p_error )
  {
    FT_Error  error = FT_Err_Ok;


    block = ft_mem_qrealloc( memory, item_आकार,
                             cur_गिनती, new_गिनती, block, &error );
    अगर ( !error && block && new_गिनती > cur_गिनती )
      FT_MEM_ZERO( (अक्षर*)block + cur_गिनती * item_आकार,
                   ( new_गिनती - cur_गिनती ) * item_आकार );

    *p_error = error;
    वापस block;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_qrealloc( FT_Memory  memory,
                   FT_Long    item_आकार,
                   FT_Long    cur_गिनती,
                   FT_Long    new_गिनती,
                   व्योम*      block,
                   FT_Error  *p_error )
  {
    FT_Error  error = FT_Err_Ok;


    /* Note that we now accept `item_size == 0' as a valid parameter, in
     * order to cover very weird cases where an ALLOC_MULT macro would be
     * called.
     */
    अगर ( cur_गिनती < 0 || new_गिनती < 0 || item_आकार < 0 )
    {
      /* may help catch/prevent nasty security issues */
      error = FT_THROW( Invalid_Argument );
    }
    अन्यथा अगर ( new_गिनती == 0 || item_आकार == 0 )
    {
      ft_mem_free( memory, block );
      block = शून्य;
    }
    अन्यथा अगर ( new_गिनती > FT_INT_MAX / item_आकार )
    {
      error = FT_THROW( Array_Too_Large );
    }
    अन्यथा अगर ( cur_गिनती == 0 )
    {
      FT_ASSERT( !block );

      block = memory->alloc( memory, new_गिनती * item_आकार );
      अगर ( block == शून्य )
        error = FT_THROW( Out_Of_Memory );
    }
    अन्यथा
    {
      FT_Pointer  block2;
      FT_Long     cur_आकार = cur_गिनती * item_आकार;
      FT_Long     new_आकार = new_गिनती * item_आकार;


      block2 = memory->पुनः_स्मृति( memory, cur_आकार, new_आकार, block );
      अगर ( !block2 )
        error = FT_THROW( Out_Of_Memory );
      अन्यथा
        block = block2;
    }

    *p_error = error;
    वापस block;
  }


  FT_BASE_DEF( व्योम )
  ft_mem_free( FT_Memory   memory,
               स्थिर व्योम *P )
  {
    अगर ( P )
      memory->मुक्त( memory, (व्योम*)P );
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_dup( FT_Memory    memory,
              स्थिर व्योम*  address,
              FT_ULong     आकार,
              FT_Error    *p_error )
  {
    FT_Error    error;
    FT_Pointer  p = ft_mem_qalloc( memory, (FT_Long)आकार, &error );


    अगर ( !error && address && आकार > 0 )
      ft_memcpy( p, address, आकार );

    *p_error = error;
    वापस p;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_mem_strdup( FT_Memory    memory,
                 स्थिर अक्षर*  str,
                 FT_Error    *p_error )
  {
    FT_ULong  len = str ? (FT_ULong)ft_strlen( str ) + 1
                        : 0;


    वापस ft_mem_dup( memory, str, len, p_error );
  }


  FT_BASE_DEF( FT_Int )
  ft_mem_strcpyn( अक्षर*        dst,
                  स्थिर अक्षर*  src,
                  FT_ULong     आकार )
  {
    जबतक ( आकार > 1 && *src != 0 )
    {
      *dst++ = *src++;
      आकार--;
    }

    *dst = 0;  /* always zero-terminate */

    वापस *src != 0;
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

#अघोषित  FT_COMPONENT
#घोषणा FT_COMPONENT  list

  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( FT_List_गाँठ )
  FT_List_Find( FT_List  list,
                व्योम*    तथ्य )
  {
    FT_List_गाँठ  cur;


    अगर ( !list )
      वापस शून्य;

    cur = list->शीर्ष_2;
    जबतक ( cur )
    {
      अगर ( cur->तथ्य == तथ्य )
        वापस cur;

      cur = cur->अग्र;
    }

    वापस शून्य;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( व्योम )
  FT_List_Add( FT_List      list,
               FT_List_गाँठ  गाँठ )
  {
    FT_List_गाँठ  before;


    अगर ( !list || !गाँठ )
      वापस;

    before = list->पुच्छ;

    गाँठ->अग्र = शून्य;
    गाँठ->पूर्व = before;

    अगर ( before )
      before->अग्र = गाँठ;
    अन्यथा
      list->शीर्ष_2 = गाँठ;

    list->पुच्छ = गाँठ;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( व्योम )
  FT_List_जोड़ो( FT_List      list,
                  FT_List_गाँठ  गाँठ )
  {
    FT_List_गाँठ  after;


    अगर ( !list || !गाँठ )
      वापस;

    after = list->शीर्ष_2;

    गाँठ->अग्र = after;
    गाँठ->पूर्व = शून्य;

    अगर ( !after )
      list->पुच्छ = गाँठ;
    अन्यथा
      after->पूर्व = गाँठ;

    list->शीर्ष_2 = गाँठ;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( व्योम )
  FT_List_Remove( FT_List      list,
                  FT_List_गाँठ  गाँठ )
  {
    FT_List_गाँठ  before, after;


    अगर ( !list || !गाँठ )
      वापस;

    before = गाँठ->पूर्व;
    after  = गाँठ->अग्र;

    अगर ( before )
      before->अग्र = after;
    अन्यथा
      list->शीर्ष_2 = after;

    अगर ( after )
      after->पूर्व = before;
    अन्यथा
      list->पुच्छ = before;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( व्योम )
  FT_List_Up( FT_List      list,
              FT_List_गाँठ  गाँठ )
  {
    FT_List_गाँठ  before, after;


    अगर ( !list || !गाँठ )
      वापस;

    before = गाँठ->पूर्व;
    after  = गाँठ->अग्र;

    /* check whether we are already on top of the list */
    अगर ( !before )
      वापस;

    before->अग्र = after;

    अगर ( after )
      after->पूर्व = before;
    अन्यथा
      list->पुच्छ = before;

    गाँठ->पूर्व       = शून्य;
    गाँठ->अग्र       = list->शीर्ष_2;
    list->शीर्ष_2->पूर्व = गाँठ;
    list->शीर्ष_2       = गाँठ;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( FT_Error )
  FT_List_Iterate( FT_List           list,
                   FT_List_Iterator  iterator,
                   व्योम*             user )
  {
    FT_List_गाँठ  cur;
    FT_Error     error = FT_Err_Ok;


    अगर ( !list || !iterator )
      वापस FT_THROW( Invalid_Argument );

    cur = list->शीर्ष_2;

    जबतक ( cur )
    {
      FT_List_गाँठ  अग्र = cur->अग्र;


      error = iterator( cur, user );
      अगर ( error )
        अवरोध;

      cur = अग्र;
    }

    वापस error;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( व्योम )
  FT_List_Finalize( FT_List             list,
                    FT_List_Destructor  destroy,
                    FT_Memory           memory,
                    व्योम*               user )
  {
    FT_List_गाँठ  cur;


    अगर ( !list || !memory )
      वापस;

    cur = list->शीर्ष_2;
    जबतक ( cur )
    {
      FT_List_गाँठ  अग्र = cur->अग्र;


      अगर ( destroy )
        destroy( memory, cur->तथ्य, user );

      FT_FREE( cur );
      cur = अग्र;
    }

    list->शीर्ष_2 = शून्य;
    list->पुच्छ = शून्य;
  }


/* END */
