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


#شامل <ft2build.h>
#شامل FT_CONFIG_CONFIG_H
#شامل <freetype/internal/ftdebug.h>
#شامل <freetype/internal/ftmemory.h>
#شامل <freetype/ftsystem.h>
#شامل <freetype/fterrors.h>
#شامل <freetype/fttypes.h>


#ifdef FT_DEBUG_MEMORY

#define  KEEPALIVE /* `Keep alive' means that freed blocks aren't released
                    * to the heap.  This is useful to detect double-frees
                    * or weird heap corruption, but it uses large amounts of
                    * memory, however.
                    */

#شامل FT_CONFIG_STANDARD_LIBRARY_H

  FT_BASE_DEF( ثابت نویسه* )  ft_debug_file_   = پوچ;
  FT_BASE_DEF( long )         ft_debug_lineno_ = 0;

  بیرونی تهی
  FT_DumpMemory( FT_Memory  memory );


  تعریفنوع ساختار FT_MemSourceRec_*  FT_MemSource;
  تعریفنوع ساختار FT_Mem_گره_Rec*    FT_Mem_گره;
  تعریفنوع ساختار FT_MemTableRec_*   FT_MemTable;


#define FT_MEM_VAL( addr )  ( (FT_PtrDist)(FT_Pointer)( addr ) )

  /*
   * This structure holds statistics for a single allocation/release
   * site.  This is useful to know where memory operations happen the
   * most.
   */
  تعریفنوع ساختار  FT_MemSourceRec_
  {
    ثابت نویسه*   file_name;
    long          سطر_no;

    FT_Long       cur_blocks;   /* current number of allocated blocks */
    FT_Long       max_blocks;   /* max. number of allocated blocks    */
    FT_Long       all_blocks;   /* total number of blocks allocated   */

    FT_Long       cur_size;     /* current cumulative allocated size */
    FT_Long       max_size;     /* maximum cumulative allocated size */
    FT_Long       all_size;     /* total cumulative allocated size   */

    FT_Long       cur_max;      /* current maximum allocated size */

    FT_UInt32     hash;
    FT_MemSource  link;

  } FT_MemSourceRec;


  /*
   * We don't need a resizable array for the memory sources because
   * their number is pretty limited within FreeType.
   */
#define FT_MEM_SOURCE_BUCKETS  128

  /*
   * This structure holds information related to a single allocated
   * memory block.  If KEEPALIVE is defined, blocks that are freed by
   * FreeType are never released to the system.  Instead, their `size'
   * field is set to `-size'.  This is mainly useful to detect double
   * frees, at the price of a large memory footprint during execution.
   */
  تعریفنوع ساختار  FT_Mem_گره_Rec
  {
    FT_Byte*      address;
    FT_Long       size;     /* < 0 if the block was freed */

    FT_MemSource  source;

#ifdef KEEPALIVE
    ثابت نویسه*   free_file_name;
    FT_Long       free_سطر_no;
#endif

    FT_Mem_گره    link;

  } FT_Mem_گره_Rec_2;


  /*
   * The global structure, containing compound statistics and all hash
   * tables.
   */
  تعریفنوع ساختار  FT_MemTableRec_
  {
    FT_Long          size;
    FT_Long          nodes;
    FT_Mem_گره*      buckets;

    FT_Long          alloc_total;
    FT_Long          alloc_current;
    FT_Long          alloc_max;
    FT_Long          alloc_شمار;

    FT_Bool          bound_total;
    FT_Long          alloc_total_max;

    FT_Bool          bound_شمار;
    FT_Long          alloc_شمار_max;

    FT_MemSource     sources[FT_MEM_SOURCE_BUCKETS];

    FT_Bool          keep_alive;

    FT_Memory        memory;
    FT_Pointer       memory_user;
    FT_Alloc_Func    alloc;
    FT_Free_Func     آزاد;
    FT_Realloc_Func  realloc;

  } FT_MemTableRec;


#define FT_MEM_SIZE_MAX  13845163

#define FT_FILENAME( x )  ( (x) ? (x) : "unknown file" )


  /*
   * Prime numbers are ugly to handle.  It would be better to implement
   * L-Hashing, which is 10% faster and doesn't require divisions.
   */
  ایستا ثابت FT_Int  ft_mem_primes[] =
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


  ایستا FT_Long
  ft_mem_closest_اول( FT_Long  رقم )
  {
    size_t  i;


    برای ( i = 0;
          i < اندازه ( ft_mem_primes ) / اندازه ( ft_mem_primes[0] ); i++ )
      اگر ( ft_mem_primes[i] > رقم )
        بازگشت ft_mem_primes[i];

    بازگشت FT_MEM_SIZE_MAX;
  }


  ایستا تهی
  ft_mem_debug_panic( ثابت نویسه*  fmt,
                      ... )
  {
    va_list  ap;


    printf( "FreeType.Debug: " );

    va_start( ap, fmt );
    vprintf( fmt, ap );
    va_end( ap );

    printf( "\n" );
    exit( EXIT_FAILURE );
  }


  ایستا FT_Pointer
  ft_mem_table_alloc( FT_MemTable  table,
                      FT_Long      size )
  {
    FT_Memory   memory = table->memory;
    FT_Pointer  block;


    memory->user = table->memory_user;
    block = table->alloc( memory, size );
    memory->user = table;

    بازگشت block;
  }


  ایستا تهی
  ft_mem_table_free( FT_MemTable  table,
                     FT_Pointer   block )
  {
    FT_Memory  memory = table->memory;


    memory->user = table->memory_user;
    table->آزاد( memory, block );
    memory->user = table;
  }


  ایستا تهی
  ft_mem_table_resize( FT_MemTable  table )
  {
    FT_Long  new_size;


    new_size = ft_mem_closest_اول( table->nodes );
    اگر ( new_size != table->size )
    {
      FT_Mem_گره*  new_buckets;
      FT_Long      i;


      new_buckets = (FT_Mem_گره *)
                      ft_mem_table_alloc(
                        table,
                        new_size * (FT_Long)اندازه ( FT_Mem_گره ) );
      اگر ( !new_buckets )
        بازگشت;

      FT_ARRAY_ZERO( new_buckets, new_size );

      برای ( i = 0; i < table->size; i++ )
      {
        FT_Mem_گره  گره, next, *pnode;
        FT_PtrDist  hash;


        گره = table->buckets[i];
        وقتی ( گره )
        {
          next  = گره->link;
          hash  = FT_MEM_VAL( گره->address ) % (FT_PtrDist)new_size;
          pnode = new_buckets + hash;

          گره->link = pnode[0];
          pnode[0]   = گره;

          گره = next;
        }
      }

      اگر ( table->buckets )
        ft_mem_table_free( table, table->buckets );

      table->buckets = new_buckets;
      table->size    = new_size;
    }
  }


  ایستا تهی
  ft_mem_table_destroy( FT_MemTable  table )
  {
    FT_Long  i;
    FT_Long  leak_شمار = 0;
    FT_Long  leaks      = 0;


    /* remove all blocks from the table, revealing leaked ones */
    برای ( i = 0; i < table->size; i++ )
    {
      FT_Mem_گره  *pnode = table->buckets + i, next, گره = *pnode;


      وقتی ( گره )
      {
        next       = گره->link;
        گره->link = پوچ;

        اگر ( گره->size > 0 )
        {
          printf(
            "leaked memory block at address %p, size %8ld in (%s:%ld)\n",
            (تهی*)گره->address,
            گره->size,
            FT_FILENAME( گره->source->file_name ),
            گره->source->سطر_no );

          leak_شمار++;
          leaks += گره->size;

          ft_mem_table_free( table, گره->address );
        }

        گره->address = پوچ;
        گره->size    = 0;

        ft_mem_table_free( table, گره );
        گره = next;
      }
      table->buckets[i] = پوچ;
    }

    ft_mem_table_free( table, table->buckets );
    table->buckets = پوچ;

    table->size  = 0;
    table->nodes = 0;

    /* remove all sources */
    برای ( i = 0; i < FT_MEM_SOURCE_BUCKETS; i++ )
    {
      FT_MemSource  source, next;


      برای ( source = table->sources[i]; source != پوچ; source = next )
      {
        next = source->link;
        ft_mem_table_free( table, source );
      }

      table->sources[i] = پوچ;
    }

    printf( "FreeType: total memory allocations = %ld\n",
            table->alloc_total );
    printf( "FreeType: maximum memory footprint = %ld\n",
            table->alloc_max );

    اگر ( leak_شمار > 0 )
      ft_mem_debug_panic(
        "FreeType: %ld bytes of memory leaked in %ld blocks\n",
        leaks, leak_شمار );

    printf( "FreeType: no memory leaks detected\n" );
  }


  ایستا FT_Mem_گره*
  ft_mem_table_get_nodep( FT_MemTable  table,
                          FT_Byte*     address )
  {
    FT_PtrDist   hash;
    FT_Mem_گره  *pnode, گره;


    hash  = FT_MEM_VAL( address );
    pnode = table->buckets + ( hash % (FT_PtrDist)table->size );

    برای (;;)
    {
      گره = pnode[0];
      اگر ( !گره )
        بشکن;

      اگر ( گره->address == address )
        بشکن;

      pnode = &گره->link;
    }
    بازگشت pnode;
  }


  ایستا FT_MemSource
  ft_mem_table_get_source( FT_MemTable  table )
  {
    FT_UInt32     hash;
    FT_MemSource  گره, *pnode;


    /* cast to FT_PtrDist first since void* can be larger */
    /* than FT_UInt32 and GCC 4.1.1 emits a warning       */
    hash  = (FT_UInt32)(FT_PtrDist)(تهی*)ft_debug_file_ +
              (FT_UInt32)( 5 * ft_debug_lineno_ );
    pnode = &table->sources[hash % FT_MEM_SOURCE_BUCKETS];

    برای (;;)
    {
      گره = *pnode;
      اگر ( !گره )
        بشکن;

      اگر ( گره->file_name == ft_debug_file_   &&
           گره->سطر_no   == ft_debug_lineno_ )
        برو Exit;

      pnode = &گره->link;
    }

    گره = (FT_MemSource)ft_mem_table_alloc( table, اندازه ( *گره ) );
    اگر ( !گره )
      ft_mem_debug_panic(
        "not enough memory to perform memory debugging\n" );

    گره->file_name = ft_debug_file_;
    گره->سطر_no   = ft_debug_lineno_;

    گره->cur_blocks = 0;
    گره->max_blocks = 0;
    گره->all_blocks = 0;

    گره->cur_size = 0;
    گره->max_size = 0;
    گره->all_size = 0;

    گره->cur_max = 0;

    گره->link = پوچ;
    گره->hash = hash;
    *pnode     = گره;

  Exit:
    بازگشت گره;
  }


  ایستا تهی
  ft_mem_table_set( FT_MemTable  table,
                    FT_Byte*     address,
                    FT_Long      size,
                    FT_Long      delta )
  {
    FT_Mem_گره  *pnode, گره;


    اگر ( table )
    {
      FT_MemSource  source;


      pnode = ft_mem_table_get_nodep( table, address );
      گره  = *pnode;
      اگر ( گره )
      {
        اگر ( گره->size < 0 )
        {
          /* This block was already freed.  Our memory is now completely */
          /* corrupted!                                                  */
          /* This can only happen in keep-alive mode.                    */
          ft_mem_debug_panic(
            "memory heap corrupted (allocating freed block)" );
        }
        وگرنه
        {
          /* This block was already allocated.  This means that our memory */
          /* is also corrupted!                                            */
          ft_mem_debug_panic(
            "memory heap corrupted (re-allocating allocated block at"
            " %p, of size %ld)\n"
            "org=%s:%d new=%s:%d\n",
            گره->address, گره->size,
            FT_FILENAME( گره->source->file_name ), گره->source->سطر_no,
            FT_FILENAME( ft_debug_file_ ), ft_debug_lineno_ );
        }
      }

      /* we need to create a new node in this table */
      گره = (FT_Mem_گره)ft_mem_table_alloc( table, اندازه ( *گره ) );
      اگر ( !گره )
        ft_mem_debug_panic( "not enough memory to run memory tests" );

      گره->address = address;
      گره->size    = size;
      گره->source  = source = ft_mem_table_get_source( table );

      اگر ( delta == 0 )
      {
        /* this is an allocation */
        source->all_blocks++;
        source->cur_blocks++;
        اگر ( source->cur_blocks > source->max_blocks )
          source->max_blocks = source->cur_blocks;
      }

      اگر ( size > source->cur_max )
        source->cur_max = size;

      اگر ( delta != 0 )
      {
        /* we are growing or shrinking a reallocated block */
        source->cur_size     += delta;
        table->alloc_current += delta;
      }
      وگرنه
      {
        /* we are allocating a new block */
        source->cur_size     += size;
        table->alloc_current += size;
      }

      source->all_size += size;

      اگر ( source->cur_size > source->max_size )
        source->max_size = source->cur_size;

      گره->free_file_name = پوچ;
      گره->free_سطر_no   = 0;

      گره->link = pnode[0];

      pnode[0] = گره;
      table->nodes++;

      table->alloc_total += size;

      اگر ( table->alloc_current > table->alloc_max )
        table->alloc_max = table->alloc_current;

      اگر ( table->nodes * 3 < table->size  ||
           table->size  * 3 < table->nodes )
        ft_mem_table_resize( table );
    }
  }


  ایستا تهی
  ft_mem_table_remove( FT_MemTable  table,
                       FT_Byte*     address,
                       FT_Long      delta )
  {
    اگر ( table )
    {
      FT_Mem_گره  *pnode, گره;


      pnode = ft_mem_table_get_nodep( table, address );
      گره  = *pnode;
      اگر ( گره )
      {
        FT_MemSource  source;


        اگر ( گره->size < 0 )
          ft_mem_debug_panic(
            "freeing memory block at %p more than once\n"
            "  at (%s:%ld)!\n"
            "  Block was allocated at (%s:%ld)\n"
            "  and released at (%s:%ld).",
            address,
            FT_FILENAME( ft_debug_file_ ), ft_debug_lineno_,
            FT_FILENAME( گره->source->file_name ), گره->source->سطر_no,
            FT_FILENAME( گره->free_file_name ), گره->free_سطر_no );

        /* scramble the node's content for additional safety */
        FT_MEM_SET( address, 0xF3, گره->size );

        اگر ( delta == 0 )
        {
          source = گره->source;

          source->cur_blocks--;
          source->cur_size -= گره->size;

          table->alloc_current -= گره->size;
        }

        اگر ( table->keep_alive )
        {
          /* we simply invert the node's size to indicate that the node */
          /* was freed.                                                 */
          گره->size           = -گره->size;
          گره->free_file_name = ft_debug_file_;
          گره->free_سطر_no   = ft_debug_lineno_;
        }
        وگرنه
        {
          table->nodes--;

          *pnode = گره->link;

          گره->size   = 0;
          گره->source = پوچ;

          ft_mem_table_free( table, گره );

          اگر ( table->nodes * 3 < table->size  ||
               table->size  * 3 < table->nodes )
            ft_mem_table_resize( table );
        }
      }
      وگرنه
        ft_mem_debug_panic(
          "trying to free unknown block at %p in (%s:%ld)\n",
          address,
          FT_FILENAME( ft_debug_file_ ), ft_debug_lineno_ );
    }
  }


  ایستا FT_Pointer
  ft_mem_debug_alloc( FT_Memory  memory,
                      FT_Long    size )
  {
    FT_MemTable  table = (FT_MemTable)memory->user;
    FT_Byte*     block;


    اگر ( size <= 0 )
      ft_mem_debug_panic( "negative block size allocation (%ld)", size );

    /* return NULL if the maximum number of allocations was reached */
    اگر ( table->bound_شمار                           &&
         table->alloc_شمار >= table->alloc_شمار_max )
      بازگشت پوچ;

    /* return NULL if this allocation would overflow the maximum heap size */
    اگر ( table->bound_total                                   &&
         table->alloc_total_max - table->alloc_current > size )
      بازگشت پوچ;

    block = (FT_Byte *)ft_mem_table_alloc( table, size );
    اگر ( block )
    {
      ft_mem_table_set( table, block, size, 0 );

      table->alloc_شمار++;
    }

    ft_debug_file_   = "<unknown>";
    ft_debug_lineno_ = 0;

    بازگشت (FT_Pointer)block;
  }


  ایستا تهی
  ft_mem_debug_free( FT_Memory   memory,
                     FT_Pointer  block )
  {
    FT_MemTable  table = (FT_MemTable)memory->user;


    اگر ( !block )
      ft_mem_debug_panic( "trying to free NULL in (%s:%ld)",
                          FT_FILENAME( ft_debug_file_ ),
                          ft_debug_lineno_ );

    ft_mem_table_remove( table, (FT_Byte*)block, 0 );

    اگر ( !table->keep_alive )
      ft_mem_table_free( table, block );

    table->alloc_شمار--;

    ft_debug_file_   = "<unknown>";
    ft_debug_lineno_ = 0;
  }


  ایستا FT_Pointer
  ft_mem_debug_realloc( FT_Memory   memory,
                        FT_Long     cur_size,
                        FT_Long     new_size,
                        FT_Pointer  block )
  {
    FT_MemTable  table = (FT_MemTable)memory->user;
    FT_Mem_گره   گره, *pnode;
    FT_Pointer   new_block;
    FT_Long      delta;

    ثابت نویسه*  file_name = FT_FILENAME( ft_debug_file_ );
    FT_Long      سطر_no   = ft_debug_lineno_;


    /* unlikely, but possible */
    اگر ( new_size == cur_size )
      بازگشت block;

    /* the following is valid according to ANSI C */
#اگر 0
    اگر ( !block || !cur_size )
      ft_mem_debug_panic( "trying to reallocate NULL in (%s:%ld)",
                          file_name, سطر_no );
#endif

    /* while the following is allowed in ANSI C also, we abort since */
    /* such case should be handled by FreeType.                      */
    اگر ( new_size <= 0 )
      ft_mem_debug_panic(
        "trying to reallocate %p to size 0 (current is %ld) in (%s:%ld)",
        block, cur_size, file_name, سطر_no );

    /* check `cur_size' value */
    pnode = ft_mem_table_get_nodep( table, (FT_Byte*)block );
    گره  = *pnode;
    اگر ( !گره )
      ft_mem_debug_panic(
        "trying to reallocate unknown block at %p in (%s:%ld)",
        block, file_name, سطر_no );

    اگر ( گره->size <= 0 )
      ft_mem_debug_panic(
        "trying to reallocate freed block at %p in (%s:%ld)",
        block, file_name, سطر_no );

    اگر ( گره->size != cur_size )
      ft_mem_debug_panic( "invalid ft_realloc request for %p. cur_size is "
                          "%ld instead of %ld in (%s:%ld)",
                          block, cur_size, گره->size, file_name, سطر_no );

    /* return NULL if the maximum number of allocations was reached */
    اگر ( table->bound_شمار                           &&
         table->alloc_شمار >= table->alloc_شمار_max )
      بازگشت پوچ;

    delta = new_size - cur_size;

    /* return NULL if this allocation would overflow the maximum heap size */
    اگر ( delta > 0                                             &&
         table->bound_total                                    &&
         table->alloc_current + delta > table->alloc_total_max )
      بازگشت پوچ;

    new_block = (FT_Pointer)ft_mem_table_alloc( table, new_size );
    اگر ( !new_block )
      بازگشت پوچ;

    ft_mem_table_set( table, (FT_Byte*)new_block, new_size, delta );

    ft_memcpy( new_block, block, cur_size < new_size ? (size_t)cur_size
                                                     : (size_t)new_size );

    ft_mem_table_remove( table, (FT_Byte*)block, delta );

    ft_debug_file_   = "<unknown>";
    ft_debug_lineno_ = 0;

    اگر ( !table->keep_alive )
      ft_mem_table_free( table, block );

    بازگشت new_block;
  }


  بیرونی تهی
  ft_mem_debug_init( FT_Memory  memory )
  {
    FT_MemTable  table;


    اگر ( !ft_getenv( "FT2_DEBUG_MEMORY" ) )
      بازگشت;

    table = (FT_MemTable)memory->alloc( memory, اندازه ( *table ) );

    اگر ( table )
    {
      FT_ZERO( table );

      table->memory      = memory;
      table->memory_user = memory->user;
      table->alloc       = memory->alloc;
      table->realloc     = memory->realloc;
      table->آزاد        = memory->آزاد;

      ft_mem_table_resize( table );

      اگر ( table->size )
      {
        ثابت نویسه*  p;


        memory->user    = table;
        memory->alloc   = ft_mem_debug_alloc;
        memory->realloc = ft_mem_debug_realloc;
        memory->آزاد    = ft_mem_debug_free;

        p = ft_getenv( "FT2_ALLOC_TOTAL_MAX" );
        اگر ( p )
        {
          FT_Long  total_max = ft_strtol( p, پوچ, 10 );


          اگر ( total_max > 0 )
          {
            table->bound_total     = 1;
            table->alloc_total_max = total_max;
          }
        }

        p = ft_getenv( "FT2_ALLOC_COUNT_MAX" );
        اگر ( p )
        {
          FT_Long  total_شمار = ft_strtol( p, پوچ, 10 );


          اگر ( total_شمار > 0 )
          {
            table->bound_شمار     = 1;
            table->alloc_شمار_max = total_شمار;
          }
        }

        p = ft_getenv( "FT2_KEEP_ALIVE" );
        اگر ( p )
        {
          FT_Long  keep_alive = ft_strtol( p, پوچ, 10 );


          اگر ( keep_alive > 0 )
            table->keep_alive = 1;
        }
      }
      وگرنه
        memory->آزاد( memory, table );
    }
  }


  بیرونی تهی
  ft_mem_debug_done( FT_Memory  memory )
  {
    اگر ( memory->آزاد == ft_mem_debug_free )
    {
      FT_MemTable  table = (FT_MemTable)memory->user;


      FT_DumpMemory( memory );

      ft_mem_table_destroy( table );

      memory->آزاد    = table->آزاد;
      memory->realloc = table->realloc;
      memory->alloc   = table->alloc;
      memory->user    = table->memory_user;

      memory->آزاد( memory, table );
    }
  }


  FT_COMPARE_DEF( صحیح )
  ft_mem_source_compare( ثابت تهی*  p1,
                         ثابت تهی*  p2 )
  {
    FT_MemSource  s1 = *(FT_MemSource*)p1;
    FT_MemSource  s2 = *(FT_MemSource*)p2;


    اگر ( s2->max_size > s1->max_size )
      بازگشت 1;
    وگرنه اگر ( s2->max_size < s1->max_size )
      بازگشت -1;
    وگرنه
      بازگشت 0;
  }


  بیرونی تهی
  FT_DumpMemory( FT_Memory  memory )
  {
    اگر ( memory->آزاد == ft_mem_debug_free )
    {
      FT_MemTable    table = (FT_MemTable)memory->user;
      FT_MemSource*  bucket = table->sources;
      FT_MemSource*  limit  = bucket + FT_MEM_SOURCE_BUCKETS;
      FT_MemSource*  sources;
      FT_Int         nn, شمار;
      ثابت نویسه*    fmt;


      شمار = 0;
      برای ( ; bucket < limit; bucket++ )
      {
        FT_MemSource  source = *bucket;


        برای ( ; source; source = source->link )
          شمار++;
      }

      sources = (FT_MemSource*)
                  ft_mem_table_alloc(
                    table, شمار * (FT_Long)اندازه ( *sources ) );

      شمار = 0;
      برای ( bucket = table->sources; bucket < limit; bucket++ )
      {
        FT_MemSource  source = *bucket;


        برای ( ; source; source = source->link )
          sources[شمار++] = source;
      }

      ft_qsort( sources,
                (size_t)شمار,
                اندازه ( *sources ),
                ft_mem_source_compare );

      printf( "FreeType Memory Dump: "
              "current=%ld max=%ld total=%ld count=%ld\n",
              table->alloc_current, table->alloc_max,
              table->alloc_total, table->alloc_شمار );
      printf( " block  block    sizes    sizes    sizes   source\n" );
      printf( " count   high      sum  highsum      max   location\n" );
      printf( "-------------------------------------------------\n" );

      fmt = "%6ld %6ld %8ld %8ld %8ld %s:%d\n";

      برای ( nn = 0; nn < شمار; nn++ )
      {
        FT_MemSource  source = sources[nn];


        printf( fmt,
                source->cur_blocks, source->max_blocks,
                source->cur_size, source->max_size, source->cur_max,
                FT_FILENAME( source->file_name ),
                source->سطر_no );
      }
      printf( "------------------------------------------------\n" );

      ft_mem_table_free( table, sources );
    }
  }

#وگرنه  /* !FT_DEBUG_MEMORY */

  /* ANSI C doesn't like empty source files */
  تعریفنوع صحیح  debug_mem_dummy_;

#endif /* !FT_DEBUG_MEMORY */


/* END */
