/****************************************************************************
 *
 * ftdbgmem.c
 *
 *   Memory debugger (body).
 *
 * Copyright (C) 2001-2026 by
 * David Turner, Robert Wilhelm, and Werner Lemberg.
 *
 * This file is part of the FreeType project, and may only be used,
 * modified, and distributed under the terms of the FreeType project
 * license, LICENSE.TXT.  By continuing to use, modify, or distribute
 * this file you indicate that you have read the license and
 * understand and accept it fully.
 *
 */


#అన్తర్భావ <ft2build.h>
#అన్తర్భావ FT_CONFIG_CONFIG_H
#అన్తర్భావ <freetype/internal/ftdebug.h>
#అన్తర్భావ <freetype/internal/ftmemory.h>
#అన్తర్భావ <freetype/ftsystem.h>
#అన్తర్భావ <freetype/fterrors.h>
#అన్తర్భావ <freetype/fttypes.h>


#నిర్వచితమైతే FT_DEBUG_జ్ఞాపకం

#నిర్వచించు  KEEPALIVE /* `Keep alive' means that freed blocks aren't released
                    * to the heap.  This is useful to detect double-frees
                    * or weird heap corruption, but it uses large amounts of
                    * memory, however.
                    */

#అన్తర్భావ FT_CONFIG_STANDARD_LIBRARY_H

  FT_పునాది_DEF( స్థిరరాశి అక్షరం* )  ft_debug_file_   = ఖాళీ;
  FT_పునాది_DEF( దీర్ఘ )         ft_debug_lineno_ = 0;

  బాహ్య శూన్య
  FT_Dump_జ్ఞాపకం( FT_జ్ఞాపకం  జ్ఞాపకం );


  రకనిర్వచనం నిర్మాణం FT_Mem_ఆధారం_Rec*  FT_Mem_ఆధారం;
  రకనిర్వచనం నిర్మాణం FT_Mem_కణుపు_Rec*    FT_Mem_కణుపు;
  రకనిర్వచనం నిర్మాణం FT_Mem_పట్టిక_Rec*   FT_Mem_పట్టిక;


#నిర్వచించు FT_MEM_VAL( addr )  ( (FT_PtrDist)(FT_సూచిక)( addr ) )

  /*
   * This structure holds statistics for a single allocation/release
   * site.  This is useful to know where memory operations happen the
   * most.
   */
  రకనిర్వచనం నిర్మాణం  FT_Mem_ఆధారం_Rec
  {
    స్థిరరాశి అక్షరం*   file_name;
    దీర్ఘ          పంక్తి_no;

    FT_Long       cur_ఖండాలు;   /* current number of allocated blocks */
    FT_Long       max_ఖండాలు;   /* max. number of allocated blocks    */
    FT_Long       all_ఖండాలు;   /* total number of blocks allocated   */

    FT_Long       cur_కొలత;     /* current cumulative allocated size */
    FT_Long       max_కొలత;     /* maximum cumulative allocated size */
    FT_Long       all_కొలత;     /* total cumulative allocated size   */

    FT_Long       cur_max;      /* current maximum allocated size */

    FT_UInt32     hash;
    FT_Mem_ఆధారం  link;

  } FT_Mem_ఆధారం_Rec_2;


  /*
   * We don't need a resizable array for the memory sources because
   * their number is pretty limited within FreeType.
   */
#నిర్వచించు FT_MEM_ఆధారం_BUCKETS  128

  /*
   * This structure holds information related to a single allocated
   * memory block.  If KEEPALIVE is defined, blocks that are freed by
   * FreeType are never released to the system.  Instead, their `size'
   * field is set to `-size'.  This is mainly useful to detect double
   * frees, at the price of a large memory footprint during execution.
   */
  రకనిర్వచనం నిర్మాణం  FT_Mem_కణుపు_Rec
  {
    FT_Byte*      చిరునామా;
    FT_Long       కొలత;     /* < 0 if the block was freed */

    FT_Mem_ఆధారం  ఆధారం;

#నిర్వచితమైతే KEEPALIVE
    స్థిరరాశి అక్షరం*   free_file_name;
    FT_Long       free_పంక్తి_no;
#అయితే_ముగింపు

    FT_Mem_కణుపు    link;

  } FT_Mem_కణుపు_Rec_2;


  /*
   * The global structure, containing compound statistics and all hash
   * tables.
   */
  రకనిర్వచనం నిర్మాణం  FT_Mem_పట్టిక_Rec
  {
    FT_Long          కొలత;
    FT_Long          nodes;
    FT_Mem_కణుపు*      buckets;

    FT_Long          alloc_మొత్తం;
    FT_Long          alloc_current;
    FT_Long          alloc_max;
    FT_Long          alloc_లెక్క;

    FT_Bool          bound_మొత్తం;
    FT_Long          alloc_మొత్తం_max;

    FT_Bool          bound_లెక్క;
    FT_Long          alloc_లెక్క_max;

    FT_Mem_ఆధారం     sources[FT_MEM_ఆధారం_BUCKETS];

    FT_Bool          keep_alive;

    FT_జ్ఞాపకం        జ్ఞాపకం;
    FT_సూచిక       జ్ఞాపకం_user;
    FT_Alloc_Func    alloc;
    FT_Free_Func     విడుదల;
    FT_Realloc_Func  realloc;

  } FT_Mem_పట్టిక_Rec_2;


#నిర్వచించు FT_MEM_కొలత_MAX  13845163

#నిర్వచించు FT_FILENAME( x )  ( (x) ? (x) : "unknown file" )


  /*
   * Prime numbers are ugly to handle.  It would be better to implement
   * L-Hashing, which is 10% faster and doesn't require divisions.
   */
  స్థిర స్థిరరాశి FT_Int  ft_mem_ప్రధానసంఖ్యలు[] =
  {
    7,
    11,
    19,
    37,
    73,
    109,
    163,
    251,
    367,
    557,
    823,
    1237,
    1861,
    2777,
    4177,
    6247,
    9371,
    14057,
    21089,
    31627,
    47431,
    71143,
    106721,
    160073,
    240101,
    360163,
    540217,
    810343,
    1215497,
    1823231,
    2734867,
    4102283,
    6153409,
    9230113,
    13845163,
  };


  స్థిర FT_Long
  ft_mem_closest_ప్రధానసంఖ్య( FT_Long  అంకె )
  {
    size_t  i;


    కొరకు ( i = 0;
          i < పరిమాణం ( ft_mem_ప్రధానసంఖ్యలు ) / పరిమాణం ( ft_mem_ప్రధానసంఖ్యలు[0] ); i++ )
      ఒకవేళ ( ft_mem_ప్రధానసంఖ్యలు[i] > అంకె )
        తిరిగి ft_mem_ప్రధానసంఖ్యలు[i];

    తిరిగి FT_MEM_కొలత_MAX;
  }


  స్థిర శూన్య
  ft_mem_debug_panic( స్థిరరాశి అక్షరం*  fmt,
                      ... )
  {
    va_list  ap;


    రాయి( "FreeType.Debug: " );

    va_ప్రారంభం( ap, fmt );
    vprintf( fmt, ap );
    va_చివర( ap );

    రాయి( "\n" );
    నిష్క్రమించు( EXIT_FAILURE );
  }


  స్థిర FT_సూచిక
  ft_mem_పట్టిక_alloc( FT_Mem_పట్టిక  పట్టిక,
                      FT_Long      కొలత )
  {
    FT_జ్ఞాపకం   జ్ఞాపకం = పట్టిక->జ్ఞాపకం;
    FT_సూచిక  ఖండం;


    జ్ఞాపకం->user = పట్టిక->జ్ఞాపకం_user;
    ఖండం = పట్టిక->alloc( జ్ఞాపకం, కొలత );
    జ్ఞాపకం->user = పట్టిక;

    తిరిగి ఖండం;
  }


  స్థిర శూన్య
  ft_mem_పట్టిక_free( FT_Mem_పట్టిక  పట్టిక,
                     FT_సూచిక   ఖండం )
  {
    FT_జ్ఞాపకం  జ్ఞాపకం = పట్టిక->జ్ఞాపకం;


    జ్ఞాపకం->user = పట్టిక->జ్ఞాపకం_user;
    పట్టిక->విడుదల( జ్ఞాపకం, ఖండం );
    జ్ఞాపకం->user = పట్టిక;
  }


  స్థిర శూన్య
  ft_mem_పట్టిక_resize( FT_Mem_పట్టిక  పట్టిక )
  {
    FT_Long  new_కొలత;


    new_కొలత = ft_mem_closest_ప్రధానసంఖ్య( పట్టిక->nodes );
    ఒకవేళ ( new_కొలత != పట్టిక->కొలత )
    {
      FT_Mem_కణుపు*  new_buckets;
      FT_Long      i;


      new_buckets = (FT_Mem_కణుపు *)
                      ft_mem_పట్టిక_alloc(
                        పట్టిక,
                        new_కొలత * (FT_Long)పరిమాణం ( FT_Mem_కణుపు ) );
      ఒకవేళ ( !new_buckets )
        తిరిగి;

      FT_ARRAY_ZERO( new_buckets, new_కొలత );

      కొరకు ( i = 0; i < పట్టిక->కొలత; i++ )
      {
        FT_Mem_కణుపు  కణుపు, తదుపరి, *pnode;
        FT_PtrDist  hash;


        కణుపు = పట్టిక->buckets[i];
        అయితే ( కణుపు )
        {
          తదుపరి  = కణుపు->link;
          hash  = FT_MEM_VAL( కణుపు->చిరునామా ) % (FT_PtrDist)new_కొలత;
          pnode = new_buckets + hash;

          కణుపు->link = pnode[0];
          pnode[0]   = కణుపు;

          కణుపు = తదుపరి;
        }
      }

      ఒకవేళ ( పట్టిక->buckets )
        ft_mem_పట్టిక_free( పట్టిక, పట్టిక->buckets );

      పట్టిక->buckets = new_buckets;
      పట్టిక->కొలత    = new_కొలత;
    }
  }


  స్థిర శూన్య
  ft_mem_పట్టిక_destroy( FT_Mem_పట్టిక  పట్టిక )
  {
    FT_Long  i;
    FT_Long  leak_లెక్క = 0;
    FT_Long  leaks      = 0;


    /* remove all blocks from the table, revealing leaked ones */
    కొరకు ( i = 0; i < పట్టిక->కొలత; i++ )
    {
      FT_Mem_కణుపు  *pnode = పట్టిక->buckets + i, తదుపరి, కణుపు = *pnode;


      అయితే ( కణుపు )
      {
        తదుపరి       = కణుపు->link;
        కణుపు->link = ఖాళీ;

        ఒకవేళ ( కణుపు->కొలత > 0 )
        {
          రాయి(
            "leaked memory block at address %p, size %8ld in (%s:%ld)\n",
            (శూన్య*)కణుపు->చిరునామా,
            కణుపు->కొలత,
            FT_FILENAME( కణుపు->ఆధారం->file_name ),
            కణుపు->ఆధారం->పంక్తి_no );

          leak_లెక్క++;
          leaks += కణుపు->కొలత;

          ft_mem_పట్టిక_free( పట్టిక, కణుపు->చిరునామా );
        }

        కణుపు->చిరునామా = ఖాళీ;
        కణుపు->కొలత    = 0;

        ft_mem_పట్టిక_free( పట్టిక, కణుపు );
        కణుపు = తదుపరి;
      }
      పట్టిక->buckets[i] = ఖాళీ;
    }

    ft_mem_పట్టిక_free( పట్టిక, పట్టిక->buckets );
    పట్టిక->buckets = ఖాళీ;

    పట్టిక->కొలత  = 0;
    పట్టిక->nodes = 0;

    /* remove all sources */
    కొరకు ( i = 0; i < FT_MEM_ఆధారం_BUCKETS; i++ )
    {
      FT_Mem_ఆధారం  ఆధారం, తదుపరి;


      కొరకు ( ఆధారం = పట్టిక->sources[i]; ఆధారం != ఖాళీ; ఆధారం = తదుపరి )
      {
        తదుపరి = ఆధారం->link;
        ft_mem_పట్టిక_free( పట్టిక, ఆధారం );
      }

      పట్టిక->sources[i] = ఖాళీ;
    }

    రాయి( "FreeType: total memory allocations = %ld\n",
            పట్టిక->alloc_మొత్తం );
    రాయి( "FreeType: maximum memory footprint = %ld\n",
            పట్టిక->alloc_max );

    ఒకవేళ ( leak_లెక్క > 0 )
      ft_mem_debug_panic(
        "FreeType: %ld bytes of memory leaked in %ld blocks\n",
        leaks, leak_లెక్క );

    రాయి( "FreeType: no memory leaks detected\n" );
  }


  స్థిర FT_Mem_కణుపు*
  ft_mem_పట్టిక_get_nodep( FT_Mem_పట్టిక  పట్టిక,
                          FT_Byte*     చిరునామా )
  {
    FT_PtrDist   hash;
    FT_Mem_కణుపు  *pnode, కణుపు;


    hash  = FT_MEM_VAL( చిరునామా );
    pnode = పట్టిక->buckets + ( hash % (FT_PtrDist)పట్టిక->కొలత );

    కొరకు (;;)
    {
      కణుపు = pnode[0];
      ఒకవేళ ( !కణుపు )
        ఆపు;

      ఒకవేళ ( కణుపు->చిరునామా == చిరునామా )
        ఆపు;

      pnode = &కణుపు->link;
    }
    తిరిగి pnode;
  }


  స్థిర FT_Mem_ఆధారం
  ft_mem_పట్టిక_get_ఆధారం( FT_Mem_పట్టిక  పట్టిక )
  {
    FT_UInt32     hash;
    FT_Mem_ఆధారం  కణుపు, *pnode;


    /* cast to FT_PtrDist first since void* can be larger */
    /* than FT_UInt32 and GCC 4.1.1 emits a warning       */
    hash  = (FT_UInt32)(FT_PtrDist)(శూన్య*)ft_debug_file_ +
              (FT_UInt32)( 5 * ft_debug_lineno_ );
    pnode = &పట్టిక->sources[hash % FT_MEM_ఆధారం_BUCKETS];

    కొరకు (;;)
    {
      కణుపు = *pnode;
      ఒకవేళ ( !కణుపు )
        ఆపు;

      ఒకవేళ ( కణుపు->file_name == ft_debug_file_   &&
           కణుపు->పంక్తి_no   == ft_debug_lineno_ )
        వెళ్ళు Exit;

      pnode = &కణుపు->link;
    }

    కణుపు = (FT_Mem_ఆధారం)ft_mem_పట్టిక_alloc( పట్టిక, పరిమాణం ( *కణుపు ) );
    ఒకవేళ ( !కణుపు )
      ft_mem_debug_panic(
        "not enough memory to perform memory debugging\n" );

    కణుపు->file_name = ft_debug_file_;
    కణుపు->పంక్తి_no   = ft_debug_lineno_;

    కణుపు->cur_ఖండాలు = 0;
    కణుపు->max_ఖండాలు = 0;
    కణుపు->all_ఖండాలు = 0;

    కణుపు->cur_కొలత = 0;
    కణుపు->max_కొలత = 0;
    కణుపు->all_కొలత = 0;

    కణుపు->cur_max = 0;

    కణుపు->link = ఖాళీ;
    కణుపు->hash = hash;
    *pnode     = కణుపు;

  Exit:
    తిరిగి కణుపు;
  }


  స్థిర శూన్య
  ft_mem_పట్టిక_సమితి( FT_Mem_పట్టిక  పట్టిక,
                    FT_Byte*     చిరునామా,
                    FT_Long      కొలత,
                    FT_Long      delta )
  {
    FT_Mem_కణుపు  *pnode, కణుపు;


    ఒకవేళ ( పట్టిక )
    {
      FT_Mem_ఆధారం  ఆధారం;


      pnode = ft_mem_పట్టిక_get_nodep( పట్టిక, చిరునామా );
      కణుపు  = *pnode;
      ఒకవేళ ( కణుపు )
      {
        ఒకవేళ ( కణుపు->కొలత < 0 )
        {
          /* This block was already freed.  Our memory is now completely */
          /* corrupted!                                                  */
          /* This can only happen in keep-alive mode.                    */
          ft_mem_debug_panic(
            "memory heap corrupted (allocating freed block)" );
        }
        లేకపోతే
        {
          /* This block was already allocated.  This means that our memory */
          /* is also corrupted!                                            */
          ft_mem_debug_panic(
            "memory heap corrupted (re-allocating allocated block at"
            " %p, of size %ld)\n"
            "org=%s:%d new=%s:%d\n",
            కణుపు->చిరునామా, కణుపు->కొలత,
            FT_FILENAME( కణుపు->ఆధారం->file_name ), కణుపు->ఆధారం->పంక్తి_no,
            FT_FILENAME( ft_debug_file_ ), ft_debug_lineno_ );
        }
      }

      /* we need to create a new node in this table */
      కణుపు = (FT_Mem_కణుపు)ft_mem_పట్టిక_alloc( పట్టిక, పరిమాణం ( *కణుపు ) );
      ఒకవేళ ( !కణుపు )
        ft_mem_debug_panic( "not enough memory to run memory tests" );

      కణుపు->చిరునామా = చిరునామా;
      కణుపు->కొలత    = కొలత;
      కణుపు->ఆధారం  = ఆధారం = ft_mem_పట్టిక_get_ఆధారం( పట్టిక );

      ఒకవేళ ( delta == 0 )
      {
        /* this is an allocation */
        ఆధారం->all_ఖండాలు++;
        ఆధారం->cur_ఖండాలు++;
        ఒకవేళ ( ఆధారం->cur_ఖండాలు > ఆధారం->max_ఖండాలు )
          ఆధారం->max_ఖండాలు = ఆధారం->cur_ఖండాలు;
      }

      ఒకవేళ ( కొలత > ఆధారం->cur_max )
        ఆధారం->cur_max = కొలత;

      ఒకవేళ ( delta != 0 )
      {
        /* we are growing or shrinking a reallocated block */
        ఆధారం->cur_కొలత     += delta;
        పట్టిక->alloc_current += delta;
      }
      లేకపోతే
      {
        /* we are allocating a new block */
        ఆధారం->cur_కొలత     += కొలత;
        పట్టిక->alloc_current += కొలత;
      }

      ఆధారం->all_కొలత += కొలత;

      ఒకవేళ ( ఆధారం->cur_కొలత > ఆధారం->max_కొలత )
        ఆధారం->max_కొలత = ఆధారం->cur_కొలత;

      కణుపు->free_file_name = ఖాళీ;
      కణుపు->free_పంక్తి_no   = 0;

      కణుపు->link = pnode[0];

      pnode[0] = కణుపు;
      పట్టిక->nodes++;

      పట్టిక->alloc_మొత్తం += కొలత;

      ఒకవేళ ( పట్టిక->alloc_current > పట్టిక->alloc_max )
        పట్టిక->alloc_max = పట్టిక->alloc_current;

      ఒకవేళ ( పట్టిక->nodes * 3 < పట్టిక->కొలత  ||
           పట్టిక->కొలత  * 3 < పట్టిక->nodes )
        ft_mem_పట్టిక_resize( పట్టిక );
    }
  }


  స్థిర శూన్య
  ft_mem_పట్టిక_remove( FT_Mem_పట్టిక  పట్టిక,
                       FT_Byte*     చిరునామా,
                       FT_Long      delta )
  {
    ఒకవేళ ( పట్టిక )
    {
      FT_Mem_కణుపు  *pnode, కణుపు;


      pnode = ft_mem_పట్టిక_get_nodep( పట్టిక, చిరునామా );
      కణుపు  = *pnode;
      ఒకవేళ ( కణుపు )
      {
        FT_Mem_ఆధారం  ఆధారం;


        ఒకవేళ ( కణుపు->కొలత < 0 )
          ft_mem_debug_panic(
            "freeing memory block at %p more than once\n"
            "  at (%s:%ld)!\n"
            "  Block was allocated at (%s:%ld)\n"
            "  and released at (%s:%ld).",
            చిరునామా,
            FT_FILENAME( ft_debug_file_ ), ft_debug_lineno_,
            FT_FILENAME( కణుపు->ఆధారం->file_name ), కణుపు->ఆధారం->పంక్తి_no,
            FT_FILENAME( కణుపు->free_file_name ), కణుపు->free_పంక్తి_no );

        /* scramble the node's content for additional safety */
        FT_MEM_సమితి( చిరునామా, 0xF3, కణుపు->కొలత );

        ఒకవేళ ( delta == 0 )
        {
          ఆధారం = కణుపు->ఆధారం;

          ఆధారం->cur_ఖండాలు--;
          ఆధారం->cur_కొలత -= కణుపు->కొలత;

          పట్టిక->alloc_current -= కణుపు->కొలత;
        }

        ఒకవేళ ( పట్టిక->keep_alive )
        {
          /* we simply invert the node's size to indicate that the node */
          /* was freed.                                                 */
          కణుపు->కొలత           = -కణుపు->కొలత;
          కణుపు->free_file_name = ft_debug_file_;
          కణుపు->free_పంక్తి_no   = ft_debug_lineno_;
        }
        లేకపోతే
        {
          పట్టిక->nodes--;

          *pnode = కణుపు->link;

          కణుపు->కొలత   = 0;
          కణుపు->ఆధారం = ఖాళీ;

          ft_mem_పట్టిక_free( పట్టిక, కణుపు );

          ఒకవేళ ( పట్టిక->nodes * 3 < పట్టిక->కొలత  ||
               పట్టిక->కొలత  * 3 < పట్టిక->nodes )
            ft_mem_పట్టిక_resize( పట్టిక );
        }
      }
      లేకపోతే
        ft_mem_debug_panic(
          "trying to free unknown block at %p in (%s:%ld)\n",
          చిరునామా,
          FT_FILENAME( ft_debug_file_ ), ft_debug_lineno_ );
    }
  }


  స్థిర FT_సూచిక
  ft_mem_debug_alloc( FT_జ్ఞాపకం  జ్ఞాపకం,
                      FT_Long    కొలత )
  {
    FT_Mem_పట్టిక  పట్టిక = (FT_Mem_పట్టిక)జ్ఞాపకం->user;
    FT_Byte*     ఖండం;


    ఒకవేళ ( కొలత <= 0 )
      ft_mem_debug_panic( "negative block size allocation (%ld)", కొలత );

    /* return NULL if the maximum number of allocations was reached */
    ఒకవేళ ( పట్టిక->bound_లెక్క                           &&
         పట్టిక->alloc_లెక్క >= పట్టిక->alloc_లెక్క_max )
      తిరిగి ఖాళీ;

    /* return NULL if this allocation would overflow the maximum heap size */
    ఒకవేళ ( పట్టిక->bound_మొత్తం                                   &&
         పట్టిక->alloc_మొత్తం_max - పట్టిక->alloc_current > కొలత )
      తిరిగి ఖాళీ;

    ఖండం = (FT_Byte *)ft_mem_పట్టిక_alloc( పట్టిక, కొలత );
    ఒకవేళ ( ఖండం )
    {
      ft_mem_పట్టిక_సమితి( పట్టిక, ఖండం, కొలత, 0 );

      పట్టిక->alloc_లెక్క++;
    }

    ft_debug_file_   = "<unknown>";
    ft_debug_lineno_ = 0;

    తిరిగి (FT_సూచిక)ఖండం;
  }


  స్థిర శూన్య
  ft_mem_debug_free( FT_జ్ఞాపకం   జ్ఞాపకం,
                     FT_సూచిక  ఖండం )
  {
    FT_Mem_పట్టిక  పట్టిక = (FT_Mem_పట్టిక)జ్ఞాపకం->user;


    ఒకవేళ ( !ఖండం )
      ft_mem_debug_panic( "trying to free NULL in (%s:%ld)",
                          FT_FILENAME( ft_debug_file_ ),
                          ft_debug_lineno_ );

    ft_mem_పట్టిక_remove( పట్టిక, (FT_Byte*)ఖండం, 0 );

    ఒకవేళ ( !పట్టిక->keep_alive )
      ft_mem_పట్టిక_free( పట్టిక, ఖండం );

    పట్టిక->alloc_లెక్క--;

    ft_debug_file_   = "<unknown>";
    ft_debug_lineno_ = 0;
  }


  స్థిర FT_సూచిక
  ft_mem_debug_realloc( FT_జ్ఞాపకం   జ్ఞాపకం,
                        FT_Long     cur_కొలత,
                        FT_Long     new_కొలత,
                        FT_సూచిక  ఖండం )
  {
    FT_Mem_పట్టిక  పట్టిక = (FT_Mem_పట్టిక)జ్ఞాపకం->user;
    FT_Mem_కణుపు   కణుపు, *pnode;
    FT_సూచిక   new_ఖండం;
    FT_Long      delta;

    స్థిరరాశి అక్షరం*  file_name = FT_FILENAME( ft_debug_file_ );
    FT_Long      పంక్తి_no   = ft_debug_lineno_;


    /* unlikely, but possible */
    ఒకవేళ ( new_కొలత == cur_కొలత )
      తిరిగి ఖండం;

    /* the following is valid according to ANSI C */
#ఒకవేళ 0
    ఒకవేళ ( !ఖండం || !cur_కొలత )
      ft_mem_debug_panic( "trying to reallocate NULL in (%s:%ld)",
                          file_name, పంక్తి_no );
#అయితే_ముగింపు

    /* while the following is allowed in ANSI C also, we abort since */
    /* such case should be handled by FreeType.                      */
    ఒకవేళ ( new_కొలత <= 0 )
      ft_mem_debug_panic(
        "trying to reallocate %p to size 0 (current is %ld) in (%s:%ld)",
        ఖండం, cur_కొలత, file_name, పంక్తి_no );

    /* check `cur_size' value */
    pnode = ft_mem_పట్టిక_get_nodep( పట్టిక, (FT_Byte*)ఖండం );
    కణుపు  = *pnode;
    ఒకవేళ ( !కణుపు )
      ft_mem_debug_panic(
        "trying to reallocate unknown block at %p in (%s:%ld)",
        ఖండం, file_name, పంక్తి_no );

    ఒకవేళ ( కణుపు->కొలత <= 0 )
      ft_mem_debug_panic(
        "trying to reallocate freed block at %p in (%s:%ld)",
        ఖండం, file_name, పంక్తి_no );

    ఒకవేళ ( కణుపు->కొలత != cur_కొలత )
      ft_mem_debug_panic( "invalid ft_realloc request for %p. cur_size is "
                          "%ld instead of %ld in (%s:%ld)",
                          ఖండం, cur_కొలత, కణుపు->కొలత, file_name, పంక్తి_no );

    /* return NULL if the maximum number of allocations was reached */
    ఒకవేళ ( పట్టిక->bound_లెక్క                           &&
         పట్టిక->alloc_లెక్క >= పట్టిక->alloc_లెక్క_max )
      తిరిగి ఖాళీ;

    delta = new_కొలత - cur_కొలత;

    /* return NULL if this allocation would overflow the maximum heap size */
    ఒకవేళ ( delta > 0                                             &&
         పట్టిక->bound_మొత్తం                                    &&
         పట్టిక->alloc_current + delta > పట్టిక->alloc_మొత్తం_max )
      తిరిగి ఖాళీ;

    new_ఖండం = (FT_సూచిక)ft_mem_పట్టిక_alloc( పట్టిక, new_కొలత );
    ఒకవేళ ( !new_ఖండం )
      తిరిగి ఖాళీ;

    ft_mem_పట్టిక_సమితి( పట్టిక, (FT_Byte*)new_ఖండం, new_కొలత, delta );

    ft_memcpy( new_ఖండం, ఖండం, cur_కొలత < new_కొలత ? (size_t)cur_కొలత
                                                     : (size_t)new_కొలత );

    ft_mem_పట్టిక_remove( పట్టిక, (FT_Byte*)ఖండం, delta );

    ft_debug_file_   = "<unknown>";
    ft_debug_lineno_ = 0;

    ఒకవేళ ( !పట్టిక->keep_alive )
      ft_mem_పట్టిక_free( పట్టిక, ఖండం );

    తిరిగి new_ఖండం;
  }


  బాహ్య శూన్య
  ft_mem_debug_init( FT_జ్ఞాపకం  జ్ఞాపకం )
  {
    FT_Mem_పట్టిక  పట్టిక;


    ఒకవేళ ( !ft_getenv( "FT2_DEBUG_MEMORY" ) )
      తిరిగి;

    పట్టిక = (FT_Mem_పట్టిక)జ్ఞాపకం->alloc( జ్ఞాపకం, పరిమాణం ( *పట్టిక ) );

    ఒకవేళ ( పట్టిక )
    {
      FT_ZERO( పట్టిక );

      పట్టిక->జ్ఞాపకం      = జ్ఞాపకం;
      పట్టిక->జ్ఞాపకం_user = జ్ఞాపకం->user;
      పట్టిక->alloc       = జ్ఞాపకం->alloc;
      పట్టిక->realloc     = జ్ఞాపకం->realloc;
      పట్టిక->విడుదల        = జ్ఞాపకం->విడుదల;

      ft_mem_పట్టిక_resize( పట్టిక );

      ఒకవేళ ( పట్టిక->కొలత )
      {
        స్థిరరాశి అక్షరం*  p;


        జ్ఞాపకం->user    = పట్టిక;
        జ్ఞాపకం->alloc   = ft_mem_debug_alloc;
        జ్ఞాపకం->realloc = ft_mem_debug_realloc;
        జ్ఞాపకం->విడుదల    = ft_mem_debug_free;

        p = ft_getenv( "FT2_ALLOC_TOTAL_MAX" );
        ఒకవేళ ( p )
        {
          FT_Long  మొత్తం_max = ft_strtol( p, ఖాళీ, 10 );


          ఒకవేళ ( మొత్తం_max > 0 )
          {
            పట్టిక->bound_మొత్తం     = 1;
            పట్టిక->alloc_మొత్తం_max = మొత్తం_max;
          }
        }

        p = ft_getenv( "FT2_ALLOC_COUNT_MAX" );
        ఒకవేళ ( p )
        {
          FT_Long  మొత్తం_లెక్క = ft_strtol( p, ఖాళీ, 10 );


          ఒకవేళ ( మొత్తం_లెక్క > 0 )
          {
            పట్టిక->bound_లెక్క     = 1;
            పట్టిక->alloc_లెక్క_max = మొత్తం_లెక్క;
          }
        }

        p = ft_getenv( "FT2_KEEP_ALIVE" );
        ఒకవేళ ( p )
        {
          FT_Long  keep_alive = ft_strtol( p, ఖాళీ, 10 );


          ఒకవేళ ( keep_alive > 0 )
            పట్టిక->keep_alive = 1;
        }
      }
      లేకపోతే
        జ్ఞాపకం->విడుదల( జ్ఞాపకం, పట్టిక );
    }
  }


  బాహ్య శూన్య
  ft_mem_debug_పూర్తి( FT_జ్ఞాపకం  జ్ఞాపకం )
  {
    ఒకవేళ ( జ్ఞాపకం->విడుదల == ft_mem_debug_free )
    {
      FT_Mem_పట్టిక  పట్టిక = (FT_Mem_పట్టిక)జ్ఞాపకం->user;


      FT_Dump_జ్ఞాపకం( జ్ఞాపకం );

      ft_mem_పట్టిక_destroy( పట్టిక );

      జ్ఞాపకం->విడుదల    = పట్టిక->విడుదల;
      జ్ఞాపకం->realloc = పట్టిక->realloc;
      జ్ఞాపకం->alloc   = పట్టిక->alloc;
      జ్ఞాపకం->user    = పట్టిక->జ్ఞాపకం_user;

      జ్ఞాపకం->విడుదల( జ్ఞాపకం, పట్టిక );
    }
  }


  FT_COMPARE_DEF( పూర్ణాంకం )
  ft_mem_ఆధారం_compare( స్థిరరాశి శూన్య*  p1,
                         స్థిరరాశి శూన్య*  p2 )
  {
    FT_Mem_ఆధారం  s1 = *(FT_Mem_ఆధారం*)p1;
    FT_Mem_ఆధారం  s2 = *(FT_Mem_ఆధారం*)p2;


    ఒకవేళ ( s2->max_కొలత > s1->max_కొలత )
      తిరిగి 1;
    లేకపోతే ఒకవేళ ( s2->max_కొలత < s1->max_కొలత )
      తిరిగి -1;
    లేకపోతే
      తిరిగి 0;
  }


  బాహ్య శూన్య
  FT_Dump_జ్ఞాపకం( FT_జ్ఞాపకం  జ్ఞాపకం )
  {
    ఒకవేళ ( జ్ఞాపకం->విడుదల == ft_mem_debug_free )
    {
      FT_Mem_పట్టిక    పట్టిక = (FT_Mem_పట్టిక)జ్ఞాపకం->user;
      FT_Mem_ఆధారం*  పాత్ర = పట్టిక->sources;
      FT_Mem_ఆధారం*  హద్దు  = పాత్ర + FT_MEM_ఆధారం_BUCKETS;
      FT_Mem_ఆధారం*  sources;
      FT_Int         nn, లెక్క;
      స్థిరరాశి అక్షరం*    fmt;


      లెక్క = 0;
      కొరకు ( ; పాత్ర < హద్దు; పాత్ర++ )
      {
        FT_Mem_ఆధారం  ఆధారం = *పాత్ర;


        కొరకు ( ; ఆధారం; ఆధారం = ఆధారం->link )
          లెక్క++;
      }

      sources = (FT_Mem_ఆధారం*)
                  ft_mem_పట్టిక_alloc(
                    పట్టిక, లెక్క * (FT_Long)పరిమాణం ( *sources ) );

      లెక్క = 0;
      కొరకు ( పాత్ర = పట్టిక->sources; పాత్ర < హద్దు; పాత్ర++ )
      {
        FT_Mem_ఆధారం  ఆధారం = *పాత్ర;


        కొరకు ( ; ఆధారం; ఆధారం = ఆధారం->link )
          sources[లెక్క++] = ఆధారం;
      }

      ft_qsort( sources,
                (size_t)లెక్క,
                పరిమాణం ( *sources ),
                ft_mem_ఆధారం_compare );

      రాయి( "FreeType Memory Dump: "
              "current=%ld max=%ld total=%ld count=%ld\n",
              పట్టిక->alloc_current, పట్టిక->alloc_max,
              పట్టిక->alloc_మొత్తం, పట్టిక->alloc_లెక్క );
      రాయి( " block  block    sizes    sizes    sizes   source\n" );
      రాయి( " count   high      sum  highsum      max   location\n" );
      రాయి( "-------------------------------------------------\n" );

      fmt = "%6ld %6ld %8ld %8ld %8ld %s:%d\n";

      కొరకు ( nn = 0; nn < లెక్క; nn++ )
      {
        FT_Mem_ఆధారం  ఆధారం = sources[nn];


        రాయి( fmt,
                ఆధారం->cur_ఖండాలు, ఆధారం->max_ఖండాలు,
                ఆధారం->cur_కొలత, ఆధారం->max_కొలత, ఆధారం->cur_max,
                FT_FILENAME( ఆధారం->file_name ),
                ఆధారం->పంక్తి_no );
      }
      రాయి( "------------------------------------------------\n" );

      ft_mem_పట్టిక_free( పట్టిక, sources );
    }
  }

#లేకపోతే  /* !FT_DEBUG_MEMORY */

  /* ANSI C doesn't like empty source files */
  రకనిర్వచనం పూర్ణాంకం  debug_mem_dummy_;

#అయితే_ముగింపు /* !FT_DEBUG_MEMORY */


/*చివర*/
