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


#అన్తర్భావ <freetype/internal/ftdebug.h>
#అన్తర్భావ <freetype/internal/ftmemory.h>
#అన్తర్భావ <freetype/internal/ftobjs.h>
#అన్తర్భావ <freetype/ftlist.h>


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#నిర్వచనం_రద్దు  FT_COMPONENT
#నిర్వచించు FT_COMPONENT  జ్ఞాపకం


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


  FT_పునాది_DEF( FT_సూచిక )
  ft_mem_alloc( FT_జ్ఞాపకం  జ్ఞాపకం,
                FT_Long    కొలత,
                FT_Error  *p_error )
  {
    FT_Error    error;
    FT_సూచిక  ఖండం = ft_mem_qalloc( జ్ఞాపకం, కొలత, &error );

    ఒకవేళ ( !error && ఖండం && కొలత > 0 )
      FT_MEM_ZERO( ఖండం, కొలత );

    *p_error = error;
    తిరిగి ఖండం;
  }


  FT_పునాది_DEF( FT_సూచిక )
  ft_mem_qalloc( FT_జ్ఞాపకం  జ్ఞాపకం,
                 FT_Long    కొలత,
                 FT_Error  *p_error )
  {
    FT_Error    error = FT_Err_Ok;
    FT_సూచిక  ఖండం = ఖాళీ;


    ఒకవేళ ( కొలత > 0 )
    {
      ఖండం = జ్ఞాపకం->alloc( జ్ఞాపకం, కొలత );
      ఒకవేళ ( !ఖండం )
        error = FT_THROW( బహిర్గతం_యొక్క_జ్ఞాపకం );
    }
    లేకపోతే ఒకవేళ ( కొలత < 0 )
    {
      /* may help catch/prevent security issues */
      error = FT_THROW( Invalid_వాదం );
    }

    *p_error = error;
    తిరిగి ఖండం;
  }


  FT_పునాది_DEF( FT_సూచిక )
  ft_mem_realloc( FT_జ్ఞాపకం  జ్ఞాపకం,
                  FT_Long    అంశం_కొలత,
                  FT_Long    cur_లెక్క,
                  FT_Long    new_లెక్క,
                  శూన్య*      ఖండం,
                  FT_Error  *p_error )
  {
    FT_Error  error = FT_Err_Ok;


    ఖండం = ft_mem_qrealloc( జ్ఞాపకం, అంశం_కొలత,
                             cur_లెక్క, new_లెక్క, ఖండం, &error );
    ఒకవేళ ( !error && ఖండం && new_లెక్క > cur_లెక్క )
      FT_MEM_ZERO( (అక్షరం*)ఖండం + cur_లెక్క * అంశం_కొలత,
                   ( new_లెక్క - cur_లెక్క ) * అంశం_కొలత );

    *p_error = error;
    తిరిగి ఖండం;
  }


  FT_పునాది_DEF( FT_సూచిక )
  ft_mem_qrealloc( FT_జ్ఞాపకం  జ్ఞాపకం,
                   FT_Long    అంశం_కొలత,
                   FT_Long    cur_లెక్క,
                   FT_Long    new_లెక్క,
                   శూన్య*      ఖండం,
                   FT_Error  *p_error )
  {
    FT_Error  error = FT_Err_Ok;


    /* Note that we now accept `item_size == 0' as a valid parameter, in
     * order to cover very weird cases where an ALLOC_MULT macro would be
     * called.
     */
    ఒకవేళ ( cur_లెక్క < 0 || new_లెక్క < 0 || అంశం_కొలత < 0 )
    {
      /* may help catch/prevent nasty security issues */
      error = FT_THROW( Invalid_వాదం );
    }
    లేకపోతే ఒకవేళ ( new_లెక్క == 0 || అంశం_కొలత == 0 )
    {
      ft_mem_free( జ్ఞాపకం, ఖండం );
      ఖండం = ఖాళీ;
    }
    లేకపోతే ఒకవేళ ( new_లెక్క > FT_INT_MAX / అంశం_కొలత )
    {
      error = FT_THROW( Array_Too_Large );
    }
    లేకపోతే ఒకవేళ ( cur_లెక్క == 0 )
    {
      FT_ASSERT( !ఖండం );

      ఖండం = జ్ఞాపకం->alloc( జ్ఞాపకం, new_లెక్క * అంశం_కొలత );
      ఒకవేళ ( ఖండం == ఖాళీ )
        error = FT_THROW( బహిర్గతం_యొక్క_జ్ఞాపకం );
    }
    లేకపోతే
    {
      FT_సూచిక  block2;
      FT_Long     cur_కొలత = cur_లెక్క * అంశం_కొలత;
      FT_Long     new_కొలత = new_లెక్క * అంశం_కొలత;


      block2 = జ్ఞాపకం->realloc( జ్ఞాపకం, cur_కొలత, new_కొలత, ఖండం );
      ఒకవేళ ( !block2 )
        error = FT_THROW( బహిర్గతం_యొక్క_జ్ఞాపకం );
      లేకపోతే
        ఖండం = block2;
    }

    *p_error = error;
    తిరిగి ఖండం;
  }


  FT_పునాది_DEF( శూన్య )
  ft_mem_free( FT_జ్ఞాపకం   జ్ఞాపకం,
               స్థిరరాశి శూన్య *P )
  {
    ఒకవేళ ( P )
      జ్ఞాపకం->విడుదల( జ్ఞాపకం, (శూన్య*)P );
  }


  FT_పునాది_DEF( FT_సూచిక )
  ft_mem_dup( FT_జ్ఞాపకం    జ్ఞాపకం,
              స్థిరరాశి శూన్య*  చిరునామా,
              FT_ULong     కొలత,
              FT_Error    *p_error )
  {
    FT_Error    error;
    FT_సూచిక  p = ft_mem_qalloc( జ్ఞాపకం, (FT_Long)కొలత, &error );


    ఒకవేళ ( !error && చిరునామా && కొలత > 0 )
      ft_memcpy( p, చిరునామా, కొలత );

    *p_error = error;
    తిరిగి p;
  }


  FT_పునాది_DEF( FT_సూచిక )
  ft_mem_strdup( FT_జ్ఞాపకం    జ్ఞాపకం,
                 స్థిరరాశి అక్షరం*  str,
                 FT_Error    *p_error )
  {
    FT_ULong  len = str ? (FT_ULong)ft_strlen( str ) + 1
                        : 0;


    తిరిగి ft_mem_dup( జ్ఞాపకం, str, len, p_error );
  }


  FT_పునాది_DEF( FT_Int )
  ft_mem_strcpyn( అక్షరం*        dst,
                  స్థిరరాశి అక్షరం*  src,
                  FT_ULong     కొలత )
  {
    అయితే ( కొలత > 1 && *src != 0 )
    {
      *dst++ = *src++;
      కొలత--;
    }

    *dst = 0;  /* always zero-terminate */

    తిరిగి *src != 0;
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

#నిర్వచనం_రద్దు  FT_COMPONENT
#నిర్వచించు FT_COMPONENT  list

  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( FT_List_కణుపు )
  FT_List_కనుగొను( FT_List  list,
                శూన్య*    దత్తాంశం )
  {
    FT_List_కణుపు  cur;


    ఒకవేళ ( !list )
      తిరిగి ఖాళీ;

    cur = list->తల;
    అయితే ( cur )
    {
      ఒకవేళ ( cur->దత్తాంశం == దత్తాంశం )
        తిరిగి cur;

      cur = cur->తదుపరి;
    }

    తిరిగి ఖాళీ;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( శూన్య )
  FT_List_కూడు( FT_List      list,
               FT_List_కణుపు  కణుపు )
  {
    FT_List_కణుపు  before;


    ఒకవేళ ( !list || !కణుపు )
      తిరిగి;

    before = list->తోక;

    కణుపు->తదుపరి = ఖాళీ;
    కణుపు->prev = before;

    ఒకవేళ ( before )
      before->తదుపరి = కణుపు;
    లేకపోతే
      list->తల = కణుపు;

    list->తోక = కణుపు;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( శూన్య )
  FT_List_చొప్పించు( FT_List      list,
                  FT_List_కణుపు  కణుపు )
  {
    FT_List_కణుపు  after;


    ఒకవేళ ( !list || !కణుపు )
      తిరిగి;

    after = list->తల;

    కణుపు->తదుపరి = after;
    కణుపు->prev = ఖాళీ;

    ఒకవేళ ( !after )
      list->తోక = కణుపు;
    లేకపోతే
      after->prev = కణుపు;

    list->తల = కణుపు;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( శూన్య )
  FT_List_Remove( FT_List      list,
                  FT_List_కణుపు  కణుపు )
  {
    FT_List_కణుపు  before, after;


    ఒకవేళ ( !list || !కణుపు )
      తిరిగి;

    before = కణుపు->prev;
    after  = కణుపు->తదుపరి;

    ఒకవేళ ( before )
      before->తదుపరి = after;
    లేకపోతే
      list->తల = after;

    ఒకవేళ ( after )
      after->prev = before;
    లేకపోతే
      list->తోక = before;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( శూన్య )
  FT_List_పైకి( FT_List      list,
              FT_List_కణుపు  కణుపు )
  {
    FT_List_కణుపు  before, after;


    ఒకవేళ ( !list || !కణుపు )
      తిరిగి;

    before = కణుపు->prev;
    after  = కణుపు->తదుపరి;

    /* check whether we are already on top of the list */
    ఒకవేళ ( !before )
      తిరిగి;

    before->తదుపరి = after;

    ఒకవేళ ( after )
      after->prev = before;
    లేకపోతే
      list->తోక = before;

    కణుపు->prev       = ఖాళీ;
    కణుపు->తదుపరి       = list->తల;
    list->తల->prev = కణుపు;
    list->తల       = కణుపు;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( FT_Error )
  FT_List_Iterate( FT_List           list,
                   FT_List_Iterator  iterator,
                   శూన్య*             user )
  {
    FT_List_కణుపు  cur;
    FT_Error     error = FT_Err_Ok;


    ఒకవేళ ( !list || !iterator )
      తిరిగి FT_THROW( Invalid_వాదం );

    cur = list->తల;

    అయితే ( cur )
    {
      FT_List_కణుపు  తదుపరి = cur->తదుపరి;


      error = iterator( cur, user );
      ఒకవేళ ( error )
        ఆపు;

      cur = తదుపరి;
    }

    తిరిగి error;
  }


  /* documentation is in ftlist.h */

  FT_EXPORT_DEF( శూన్య )
  FT_List_Finalize( FT_List             list,
                    FT_List_Destructor  destroy,
                    FT_జ్ఞాపకం           జ్ఞాపకం,
                    శూన్య*               user )
  {
    FT_List_కణుపు  cur;


    ఒకవేళ ( !list || !జ్ఞాపకం )
      తిరిగి;

    cur = list->తల;
    అయితే ( cur )
    {
      FT_List_కణుపు  తదుపరి = cur->తదుపరి;


      ఒకవేళ ( destroy )
        destroy( జ్ఞాపకం, cur->దత్తాంశం, user );

      FT_FREE( cur );
      cur = తదుపరి;
    }

    list->తల = ఖాళీ;
    list->తోక = ఖాళీ;
  }


/*చివర*/
