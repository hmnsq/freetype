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


#ضمن <ft2build.h>
#ضمن FT_CONFIG_CONFIG_H
#ضمن <freetype/internal/ftdebug.h>
#ضمن <freetype/internal/ftmemory.h>
#ضمن <freetype/ftsystem.h>
#ضمن <freetype/fterrors.h>
#ضمن <freetype/fttypes.h>


#ifdef FT_DEBUG_MEMORY

#define  KEEPALIVE /* `Keep alive' means that freed blocks aren't released
                    * to the heap.  This is useful to detect double-frees
                    * or weird heap corruption, but it uses large amounts of
                    * memory, however.
                    */

#ضمن FT_CONFIG_STANDARD_LIBRARY_H

  FT_BASE_DEF( ثابت حرف* )  ft_debug_file_   = عدم;
  FT_BASE_DEF( long )         ft_debug_lineno_ = 0;

  خارجي فراغ
  FT_DumpMemory( FT_Memory  memory );


  تعريفنوع بنية FT_MemSourceRec_*  FT_MemSource;
  تعريفنوع بنية FT_Mem_عقدة_Rec*    FT_Mem_عقدة;
  تعريفنوع بنية FT_MemTableRec_*   FT_MemTable;


#define FT_MEM_VAL( addr )  ( (FT_PtrDist)(FT_Pointer)( addr ) )

  /*
   * This structure holds statistics for a single allocation/release
   * site.  This is useful to know where memory operations happen the
   * most.
   */
  تعريفنوع بنية  FT_MemSourceRec_
  {
    ثابت حرف*   file_name;
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
  تعريفنوع بنية  FT_Mem_عقدة_Rec
  {
    FT_Byte*      address;
    FT_Long       size;     /* < 0 if the block was freed */

    FT_MemSource  source;

#ifdef KEEPALIVE
    ثابت حرف*   free_file_name;
    FT_Long       free_سطر_no;
#endif

    FT_Mem_عقدة    link;

  } FT_Mem_عقدة_Rec_2;


  /*
   * The global structure, containing compound statistics and all hash
   * tables.
   */
  تعريفنوع بنية  FT_MemTableRec_
  {
    FT_Long          size;
    FT_Long          nodes;
    FT_Mem_عقدة*      buckets;

    FT_Long          alloc_total;
    FT_Long          alloc_current;
    FT_Long          alloc_max;
    FT_Long          alloc_تعداد;

    FT_Bool          bound_total;
    FT_Long          alloc_total_max;

    FT_Bool          bound_تعداد;
    FT_Long          alloc_تعداد_max;

    FT_MemSource     sources[FT_MEM_SOURCE_BUCKETS];

    FT_Bool          keep_alive;

    FT_Memory        memory;
    FT_Pointer       memory_user;
    FT_Alloc_Func    alloc;
    FT_Free_Func     حرر;
    FT_Realloc_Func  realloc;

  } FT_MemTableRec;


#define FT_MEM_SIZE_MAX  13845163

#define FT_FILENAME( x )  ( (x) ? (x) : "unknown file" )


  /*
   * Prime numbers are ugly to handle.  It would be better to implement
   * L-Hashing, which is 10% faster and doesn't require divisions.
   */
  ساكن ثابت FT_Int  ft_mem_primes[] =
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


  ساكن FT_Long
  ft_mem_closest_أولي( FT_Long  رقم )
  {
    size_t  i;


    لكل ( i = 0;
          i < حجم ( ft_mem_primes ) / حجم ( ft_mem_primes[0] ); i++ )
      إذا ( ft_mem_primes[i] > رقم )
        إرجاع ft_mem_primes[i];

    إرجاع FT_MEM_SIZE_MAX;
  }


  ساكن فراغ
  ft_mem_debug_panic( ثابت حرف*  fmt,
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


  ساكن FT_Pointer
  ft_mem_table_alloc( FT_MemTable  table,
                      FT_Long      size )
  {
    FT_Memory   memory = table->memory;
    FT_Pointer  block;


    memory->user = table->memory_user;
    block = table->alloc( memory, size );
    memory->user = table;

    إرجاع block;
  }


  ساكن فراغ
  ft_mem_table_free( FT_MemTable  table,
                     FT_Pointer   block )
  {
    FT_Memory  memory = table->memory;


    memory->user = table->memory_user;
    table->حرر( memory, block );
    memory->user = table;
  }


  ساكن فراغ
  ft_mem_table_resize( FT_MemTable  table )
  {
    FT_Long  new_size;


    new_size = ft_mem_closest_أولي( table->nodes );
    إذا ( new_size != table->size )
    {
      FT_Mem_عقدة*  new_buckets;
      FT_Long      i;


      new_buckets = (FT_Mem_عقدة *)
                      ft_mem_table_alloc(
                        table,
                        new_size * (FT_Long)حجم ( FT_Mem_عقدة ) );
      إذا ( !new_buckets )
        إرجاع;

      FT_ARRAY_ZERO( new_buckets, new_size );

      لكل ( i = 0; i < table->size; i++ )
      {
        FT_Mem_عقدة  عقدة, next, *pnode;
        FT_PtrDist  hash;


        عقدة = table->buckets[i];
        بينما ( عقدة )
        {
          next  = عقدة->link;
          hash  = FT_MEM_VAL( عقدة->address ) % (FT_PtrDist)new_size;
          pnode = new_buckets + hash;

          عقدة->link = pnode[0];
          pnode[0]   = عقدة;

          عقدة = next;
        }
      }

      إذا ( table->buckets )
        ft_mem_table_free( table, table->buckets );

      table->buckets = new_buckets;
      table->size    = new_size;
    }
  }


  ساكن فراغ
  ft_mem_table_destroy( FT_MemTable  table )
  {
    FT_Long  i;
    FT_Long  leak_تعداد = 0;
    FT_Long  leaks      = 0;


    /* remove all blocks from the table, revealing leaked ones */
    لكل ( i = 0; i < table->size; i++ )
    {
      FT_Mem_عقدة  *pnode = table->buckets + i, next, عقدة = *pnode;


      بينما ( عقدة )
      {
        next       = عقدة->link;
        عقدة->link = عدم;

        إذا ( عقدة->size > 0 )
        {
          printf(
            "leaked memory block at address %p, size %8ld in (%s:%ld)\n",
            (فراغ*)عقدة->address,
            عقدة->size,
            FT_FILENAME( عقدة->source->file_name ),
            عقدة->source->سطر_no );

          leak_تعداد++;
          leaks += عقدة->size;

          ft_mem_table_free( table, عقدة->address );
        }

        عقدة->address = عدم;
        عقدة->size    = 0;

        ft_mem_table_free( table, عقدة );
        عقدة = next;
      }
      table->buckets[i] = عدم;
    }

    ft_mem_table_free( table, table->buckets );
    table->buckets = عدم;

    table->size  = 0;
    table->nodes = 0;

    /* remove all sources */
    لكل ( i = 0; i < FT_MEM_SOURCE_BUCKETS; i++ )
    {
      FT_MemSource  source, next;


      لكل ( source = table->sources[i]; source != عدم; source = next )
      {
        next = source->link;
        ft_mem_table_free( table, source );
      }

      table->sources[i] = عدم;
    }

    printf( "FreeType: total memory allocations = %ld\n",
            table->alloc_total );
    printf( "FreeType: maximum memory footprint = %ld\n",
            table->alloc_max );

    إذا ( leak_تعداد > 0 )
      ft_mem_debug_panic(
        "FreeType: %ld bytes of memory leaked in %ld blocks\n",
        leaks, leak_تعداد );

    printf( "FreeType: no memory leaks detected\n" );
  }


  ساكن FT_Mem_عقدة*
  ft_mem_table_get_nodep( FT_MemTable  table,
                          FT_Byte*     address )
  {
    FT_PtrDist   hash;
    FT_Mem_عقدة  *pnode, عقدة;


    hash  = FT_MEM_VAL( address );
    pnode = table->buckets + ( hash % (FT_PtrDist)table->size );

    لكل (;;)
    {
      عقدة = pnode[0];
      إذا ( !عقدة )
        اكسر;

      إذا ( عقدة->address == address )
        اكسر;

      pnode = &عقدة->link;
    }
    إرجاع pnode;
  }


  ساكن FT_MemSource
  ft_mem_table_get_source( FT_MemTable  table )
  {
    FT_UInt32     hash;
    FT_MemSource  عقدة, *pnode;


    /* cast to FT_PtrDist first since void* can be larger */
    /* than FT_UInt32 and GCC 4.1.1 emits a warning       */
    hash  = (FT_UInt32)(FT_PtrDist)(فراغ*)ft_debug_file_ +
              (FT_UInt32)( 5 * ft_debug_lineno_ );
    pnode = &table->sources[hash % FT_MEM_SOURCE_BUCKETS];

    لكل (;;)
    {
      عقدة = *pnode;
      إذا ( !عقدة )
        اكسر;

      إذا ( عقدة->file_name == ft_debug_file_   &&
           عقدة->سطر_no   == ft_debug_lineno_ )
        اذهب Exit;

      pnode = &عقدة->link;
    }

    عقدة = (FT_MemSource)ft_mem_table_alloc( table, حجم ( *عقدة ) );
    إذا ( !عقدة )
      ft_mem_debug_panic(
        "not enough memory to perform memory debugging\n" );

    عقدة->file_name = ft_debug_file_;
    عقدة->سطر_no   = ft_debug_lineno_;

    عقدة->cur_blocks = 0;
    عقدة->max_blocks = 0;
    عقدة->all_blocks = 0;

    عقدة->cur_size = 0;
    عقدة->max_size = 0;
    عقدة->all_size = 0;

    عقدة->cur_max = 0;

    عقدة->link = عدم;
    عقدة->hash = hash;
    *pnode     = عقدة;

  Exit:
    إرجاع عقدة;
  }


  ساكن فراغ
  ft_mem_table_set( FT_MemTable  table,
                    FT_Byte*     address,
                    FT_Long      size,
                    FT_Long      delta )
  {
    FT_Mem_عقدة  *pnode, عقدة;


    إذا ( table )
    {
      FT_MemSource  source;


      pnode = ft_mem_table_get_nodep( table, address );
      عقدة  = *pnode;
      إذا ( عقدة )
      {
        إذا ( عقدة->size < 0 )
        {
          /* This block was already freed.  Our memory is now completely */
          /* corrupted!                                                  */
          /* This can only happen in keep-alive mode.                    */
          ft_mem_debug_panic(
            "memory heap corrupted (allocating freed block)" );
        }
        وإلا
        {
          /* This block was already allocated.  This means that our memory */
          /* is also corrupted!                                            */
          ft_mem_debug_panic(
            "memory heap corrupted (re-allocating allocated block at"
            " %p, of size %ld)\n"
            "org=%s:%d new=%s:%d\n",
            عقدة->address, عقدة->size,
            FT_FILENAME( عقدة->source->file_name ), عقدة->source->سطر_no,
            FT_FILENAME( ft_debug_file_ ), ft_debug_lineno_ );
        }
      }

      /* we need to create a new node in this table */
      عقدة = (FT_Mem_عقدة)ft_mem_table_alloc( table, حجم ( *عقدة ) );
      إذا ( !عقدة )
        ft_mem_debug_panic( "not enough memory to run memory tests" );

      عقدة->address = address;
      عقدة->size    = size;
      عقدة->source  = source = ft_mem_table_get_source( table );

      إذا ( delta == 0 )
      {
        /* this is an allocation */
        source->all_blocks++;
        source->cur_blocks++;
        إذا ( source->cur_blocks > source->max_blocks )
          source->max_blocks = source->cur_blocks;
      }

      إذا ( size > source->cur_max )
        source->cur_max = size;

      إذا ( delta != 0 )
      {
        /* we are growing or shrinking a reallocated block */
        source->cur_size     += delta;
        table->alloc_current += delta;
      }
      وإلا
      {
        /* we are allocating a new block */
        source->cur_size     += size;
        table->alloc_current += size;
      }

      source->all_size += size;

      إذا ( source->cur_size > source->max_size )
        source->max_size = source->cur_size;

      عقدة->free_file_name = عدم;
      عقدة->free_سطر_no   = 0;

      عقدة->link = pnode[0];

      pnode[0] = عقدة;
      table->nodes++;

      table->alloc_total += size;

      إذا ( table->alloc_current > table->alloc_max )
        table->alloc_max = table->alloc_current;

      إذا ( table->nodes * 3 < table->size  ||
           table->size  * 3 < table->nodes )
        ft_mem_table_resize( table );
    }
  }


  ساكن فراغ
  ft_mem_table_remove( FT_MemTable  table,
                       FT_Byte*     address,
                       FT_Long      delta )
  {
    إذا ( table )
    {
      FT_Mem_عقدة  *pnode, عقدة;


      pnode = ft_mem_table_get_nodep( table, address );
      عقدة  = *pnode;
      إذا ( عقدة )
      {
        FT_MemSource  source;


        إذا ( عقدة->size < 0 )
          ft_mem_debug_panic(
            "freeing memory block at %p more than once\n"
            "  at (%s:%ld)!\n"
            "  Block was allocated at (%s:%ld)\n"
            "  and released at (%s:%ld).",
            address,
            FT_FILENAME( ft_debug_file_ ), ft_debug_lineno_,
            FT_FILENAME( عقدة->source->file_name ), عقدة->source->سطر_no,
            FT_FILENAME( عقدة->free_file_name ), عقدة->free_سطر_no );

        /* scramble the node's content for additional safety */
        FT_MEM_SET( address, 0xF3, عقدة->size );

        إذا ( delta == 0 )
        {
          source = عقدة->source;

          source->cur_blocks--;
          source->cur_size -= عقدة->size;

          table->alloc_current -= عقدة->size;
        }

        إذا ( table->keep_alive )
        {
          /* we simply invert the node's size to indicate that the node */
          /* was freed.                                                 */
          عقدة->size           = -عقدة->size;
          عقدة->free_file_name = ft_debug_file_;
          عقدة->free_سطر_no   = ft_debug_lineno_;
        }
        وإلا
        {
          table->nodes--;

          *pnode = عقدة->link;

          عقدة->size   = 0;
          عقدة->source = عدم;

          ft_mem_table_free( table, عقدة );

          إذا ( table->nodes * 3 < table->size  ||
               table->size  * 3 < table->nodes )
            ft_mem_table_resize( table );
        }
      }
      وإلا
        ft_mem_debug_panic(
          "trying to free unknown block at %p in (%s:%ld)\n",
          address,
          FT_FILENAME( ft_debug_file_ ), ft_debug_lineno_ );
    }
  }


  ساكن FT_Pointer
  ft_mem_debug_alloc( FT_Memory  memory,
                      FT_Long    size )
  {
    FT_MemTable  table = (FT_MemTable)memory->user;
    FT_Byte*     block;


    إذا ( size <= 0 )
      ft_mem_debug_panic( "negative block size allocation (%ld)", size );

    /* return NULL if the maximum number of allocations was reached */
    إذا ( table->bound_تعداد                           &&
         table->alloc_تعداد >= table->alloc_تعداد_max )
      إرجاع عدم;

    /* return NULL if this allocation would overflow the maximum heap size */
    إذا ( table->bound_total                                   &&
         table->alloc_total_max - table->alloc_current > size )
      إرجاع عدم;

    block = (FT_Byte *)ft_mem_table_alloc( table, size );
    إذا ( block )
    {
      ft_mem_table_set( table, block, size, 0 );

      table->alloc_تعداد++;
    }

    ft_debug_file_   = "<unknown>";
    ft_debug_lineno_ = 0;

    إرجاع (FT_Pointer)block;
  }


  ساكن فراغ
  ft_mem_debug_free( FT_Memory   memory,
                     FT_Pointer  block )
  {
    FT_MemTable  table = (FT_MemTable)memory->user;


    إذا ( !block )
      ft_mem_debug_panic( "trying to free NULL in (%s:%ld)",
                          FT_FILENAME( ft_debug_file_ ),
                          ft_debug_lineno_ );

    ft_mem_table_remove( table, (FT_Byte*)block, 0 );

    إذا ( !table->keep_alive )
      ft_mem_table_free( table, block );

    table->alloc_تعداد--;

    ft_debug_file_   = "<unknown>";
    ft_debug_lineno_ = 0;
  }


  ساكن FT_Pointer
  ft_mem_debug_realloc( FT_Memory   memory,
                        FT_Long     cur_size,
                        FT_Long     new_size,
                        FT_Pointer  block )
  {
    FT_MemTable  table = (FT_MemTable)memory->user;
    FT_Mem_عقدة   عقدة, *pnode;
    FT_Pointer   new_block;
    FT_Long      delta;

    ثابت حرف*  file_name = FT_FILENAME( ft_debug_file_ );
    FT_Long      سطر_no   = ft_debug_lineno_;


    /* unlikely, but possible */
    إذا ( new_size == cur_size )
      إرجاع block;

    /* the following is valid according to ANSI C */
#إذا 0
    إذا ( !block || !cur_size )
      ft_mem_debug_panic( "trying to reallocate NULL in (%s:%ld)",
                          file_name, سطر_no );
#endif

    /* while the following is allowed in ANSI C also, we abort since */
    /* such case should be handled by FreeType.                      */
    إذا ( new_size <= 0 )
      ft_mem_debug_panic(
        "trying to reallocate %p to size 0 (current is %ld) in (%s:%ld)",
        block, cur_size, file_name, سطر_no );

    /* check `cur_size' value */
    pnode = ft_mem_table_get_nodep( table, (FT_Byte*)block );
    عقدة  = *pnode;
    إذا ( !عقدة )
      ft_mem_debug_panic(
        "trying to reallocate unknown block at %p in (%s:%ld)",
        block, file_name, سطر_no );

    إذا ( عقدة->size <= 0 )
      ft_mem_debug_panic(
        "trying to reallocate freed block at %p in (%s:%ld)",
        block, file_name, سطر_no );

    إذا ( عقدة->size != cur_size )
      ft_mem_debug_panic( "invalid ft_realloc request for %p. cur_size is "
                          "%ld instead of %ld in (%s:%ld)",
                          block, cur_size, عقدة->size, file_name, سطر_no );

    /* return NULL if the maximum number of allocations was reached */
    إذا ( table->bound_تعداد                           &&
         table->alloc_تعداد >= table->alloc_تعداد_max )
      إرجاع عدم;

    delta = new_size - cur_size;

    /* return NULL if this allocation would overflow the maximum heap size */
    إذا ( delta > 0                                             &&
         table->bound_total                                    &&
         table->alloc_current + delta > table->alloc_total_max )
      إرجاع عدم;

    new_block = (FT_Pointer)ft_mem_table_alloc( table, new_size );
    إذا ( !new_block )
      إرجاع عدم;

    ft_mem_table_set( table, (FT_Byte*)new_block, new_size, delta );

    ft_memcpy( new_block, block, cur_size < new_size ? (size_t)cur_size
                                                     : (size_t)new_size );

    ft_mem_table_remove( table, (FT_Byte*)block, delta );

    ft_debug_file_   = "<unknown>";
    ft_debug_lineno_ = 0;

    إذا ( !table->keep_alive )
      ft_mem_table_free( table, block );

    إرجاع new_block;
  }


  خارجي فراغ
  ft_mem_debug_init( FT_Memory  memory )
  {
    FT_MemTable  table;


    إذا ( !ft_getenv( "FT2_DEBUG_MEMORY" ) )
      إرجاع;

    table = (FT_MemTable)memory->alloc( memory, حجم ( *table ) );

    إذا ( table )
    {
      FT_ZERO( table );

      table->memory      = memory;
      table->memory_user = memory->user;
      table->alloc       = memory->alloc;
      table->realloc     = memory->realloc;
      table->حرر        = memory->حرر;

      ft_mem_table_resize( table );

      إذا ( table->size )
      {
        ثابت حرف*  p;


        memory->user    = table;
        memory->alloc   = ft_mem_debug_alloc;
        memory->realloc = ft_mem_debug_realloc;
        memory->حرر    = ft_mem_debug_free;

        p = ft_getenv( "FT2_ALLOC_TOTAL_MAX" );
        إذا ( p )
        {
          FT_Long  total_max = ft_strtol( p, عدم, 10 );


          إذا ( total_max > 0 )
          {
            table->bound_total     = 1;
            table->alloc_total_max = total_max;
          }
        }

        p = ft_getenv( "FT2_ALLOC_COUNT_MAX" );
        إذا ( p )
        {
          FT_Long  total_تعداد = ft_strtol( p, عدم, 10 );


          إذا ( total_تعداد > 0 )
          {
            table->bound_تعداد     = 1;
            table->alloc_تعداد_max = total_تعداد;
          }
        }

        p = ft_getenv( "FT2_KEEP_ALIVE" );
        إذا ( p )
        {
          FT_Long  keep_alive = ft_strtol( p, عدم, 10 );


          إذا ( keep_alive > 0 )
            table->keep_alive = 1;
        }
      }
      وإلا
        memory->حرر( memory, table );
    }
  }


  خارجي فراغ
  ft_mem_debug_done( FT_Memory  memory )
  {
    إذا ( memory->حرر == ft_mem_debug_free )
    {
      FT_MemTable  table = (FT_MemTable)memory->user;


      FT_DumpMemory( memory );

      ft_mem_table_destroy( table );

      memory->حرر    = table->حرر;
      memory->realloc = table->realloc;
      memory->alloc   = table->alloc;
      memory->user    = table->memory_user;

      memory->حرر( memory, table );
    }
  }


  FT_COMPARE_DEF( صحيح )
  ft_mem_source_compare( ثابت فراغ*  p1,
                         ثابت فراغ*  p2 )
  {
    FT_MemSource  s1 = *(FT_MemSource*)p1;
    FT_MemSource  s2 = *(FT_MemSource*)p2;


    إذا ( s2->max_size > s1->max_size )
      إرجاع 1;
    وإلا إذا ( s2->max_size < s1->max_size )
      إرجاع -1;
    وإلا
      إرجاع 0;
  }


  خارجي فراغ
  FT_DumpMemory( FT_Memory  memory )
  {
    إذا ( memory->حرر == ft_mem_debug_free )
    {
      FT_MemTable    table = (FT_MemTable)memory->user;
      FT_MemSource*  bucket = table->sources;
      FT_MemSource*  limit  = bucket + FT_MEM_SOURCE_BUCKETS;
      FT_MemSource*  sources;
      FT_Int         nn, تعداد_2;
      ثابت حرف*    fmt;


      تعداد_2 = 0;
      لكل ( ; bucket < limit; bucket++ )
      {
        FT_MemSource  source = *bucket;


        لكل ( ; source; source = source->link )
          تعداد_2++;
      }

      sources = (FT_MemSource*)
                  ft_mem_table_alloc(
                    table, تعداد_2 * (FT_Long)حجم ( *sources ) );

      تعداد_2 = 0;
      لكل ( bucket = table->sources; bucket < limit; bucket++ )
      {
        FT_MemSource  source = *bucket;


        لكل ( ; source; source = source->link )
          sources[تعداد_2++] = source;
      }

      ft_qsort( sources,
                (size_t)تعداد_2,
                حجم ( *sources ),
                ft_mem_source_compare );

      printf( "FreeType Memory Dump: "
              "current=%ld max=%ld total=%ld count=%ld\n",
              table->alloc_current, table->alloc_max,
              table->alloc_total, table->alloc_تعداد );
      printf( " block  block    sizes    sizes    sizes   source\n" );
      printf( " count   high      sum  highsum      max   location\n" );
      printf( "-------------------------------------------------\n" );

      fmt = "%6ld %6ld %8ld %8ld %8ld %s:%d\n";

      لكل ( nn = 0; nn < تعداد_2; nn++ )
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

#وإلا  /* !FT_DEBUG_MEMORY */

  /* ANSI C doesn't like empty source files */
  تعريفنوع صحيح  debug_mem_dummy_;

#endif /* !FT_DEBUG_MEMORY */


/* END */
