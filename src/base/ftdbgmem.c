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


#ಅನ್ತರ್ಭಾವ <ft2build.h>
#ಅನ್ತರ್ಭಾವ FT_CONFIG_CONFIG_H
#ಅನ್ತರ್ಭಾವ <freetype/internal/ftdebug.h>
#ಅನ್ತರ್ಭಾವ <freetype/internal/ftmemory.h>
#ಅನ್ತರ್ಭಾವ <freetype/ftsystem.h>
#ಅನ್ತರ್ಭಾವ <freetype/fterrors.h>
#ಅನ್ತರ್ಭಾವ <freetype/fttypes.h>


#ifdef FT_DEBUG_MEMORY

#define  KEEPALIVE /* `Keep alive' means that freed blocks aren't released
                    * to the heap.  This is useful to detect double-frees
                    * or weird heap corruption, but it uses large amounts of
                    * memory, however.
                    */

#ಅನ್ತರ್ಭಾವ FT_CONFIG_STANDARD_LIBRARY_H

  FT_BASE_DEF( ಧ್ರುವ ವರ್ಣ* )  ft_debug_file_   = ರಿಕ್ತ;
  FT_BASE_DEF( long )         ft_debug_lineno_ = 0;

  ಬಾಹ್ಯ ಶೂನ್ಯ
  FT_DumpMemory( FT_Memory  memory );


  ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ FT_MemSourceRec_*  FT_MemSource;
  ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ FT_Mem_ಗಂಟು_Rec*    FT_Mem_ಗಂಟು;
  ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ FT_MemTableRec_*   FT_MemTable;


#define FT_MEM_VAL( addr )  ( (FT_PtrDist)(FT_Pointer)( addr ) )

  /*
   * This structure holds statistics for a single allocation/release
   * site.  This is useful to know where memory operations happen the
   * most.
   */
  ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ  FT_MemSourceRec_
  {
    ಧ್ರುವ ವರ್ಣ*   file_name;
    long          ಸಾಲು_no;

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
  ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ  FT_Mem_ಗಂಟು_Rec
  {
    FT_Byte*      address;
    FT_Long       size;     /* < 0 if the block was freed */

    FT_MemSource  source;

#ifdef KEEPALIVE
    ಧ್ರುವ ವರ್ಣ*   free_file_name;
    FT_Long       free_ಸಾಲು_no;
#endif

    FT_Mem_ಗಂಟು    link;

  } FT_Mem_ಗಂಟು_Rec_2;


  /*
   * The global structure, containing compound statistics and all hash
   * tables.
   */
  ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ  FT_MemTableRec_
  {
    FT_Long          size;
    FT_Long          nodes;
    FT_Mem_ಗಂಟು*      buckets;

    FT_Long          alloc_total;
    FT_Long          alloc_current;
    FT_Long          alloc_max;
    FT_Long          alloc_ಎಣಿಕೆ;

    FT_Bool          bound_total;
    FT_Long          alloc_total_max;

    FT_Bool          bound_ಎಣಿಕೆ;
    FT_Long          alloc_ಎಣಿಕೆ_max;

    FT_MemSource     sources[FT_MEM_SOURCE_BUCKETS];

    FT_Bool          keep_alive;

    FT_Memory        memory;
    FT_Pointer       memory_user;
    FT_Alloc_Func    alloc;
    FT_Free_Func     ಮುಕ್ತ;
    FT_Realloc_Func  realloc;

  } FT_MemTableRec;


#define FT_MEM_SIZE_MAX  13845163

#define FT_FILENAME( x )  ( (x) ? (x) : "unknown file" )


  /*
   * Prime numbers are ugly to handle.  It would be better to implement
   * L-Hashing, which is 10% faster and doesn't require divisions.
   */
  ಸ್ಥಿರ ಧ್ರುವ FT_Int  ft_mem_primes[] =
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


  ಸ್ಥಿರ FT_Long
  ft_mem_closest_ಅವಿಭಾಜ್ಯ( FT_Long  ಅಂಕೆ )
  {
    size_t  i;


    ಪ್ರತಿ ( i = 0;
          i < ಪರಿಮಾಣ ( ft_mem_primes ) / ಪರಿಮಾಣ ( ft_mem_primes[0] ); i++ )
      ಯದಿ ( ft_mem_primes[i] > ಅಂಕೆ )
        ಪ್ರತ್ಯಾಗಮ ft_mem_primes[i];

    ಪ್ರತ್ಯಾಗಮ FT_MEM_SIZE_MAX;
  }


  ಸ್ಥಿರ ಶೂನ್ಯ
  ft_mem_debug_panic( ಧ್ರುವ ವರ್ಣ*  fmt,
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


  ಸ್ಥಿರ FT_Pointer
  ft_mem_table_alloc( FT_MemTable  table,
                      FT_Long      size )
  {
    FT_Memory   memory = table->memory;
    FT_Pointer  block;


    memory->user = table->memory_user;
    block = table->alloc( memory, size );
    memory->user = table;

    ಪ್ರತ್ಯಾಗಮ block;
  }


  ಸ್ಥಿರ ಶೂನ್ಯ
  ft_mem_table_free( FT_MemTable  table,
                     FT_Pointer   block )
  {
    FT_Memory  memory = table->memory;


    memory->user = table->memory_user;
    table->ಮುಕ್ತ( memory, block );
    memory->user = table;
  }


  ಸ್ಥಿರ ಶೂನ್ಯ
  ft_mem_table_resize( FT_MemTable  table )
  {
    FT_Long  new_size;


    new_size = ft_mem_closest_ಅವಿಭಾಜ್ಯ( table->nodes );
    ಯದಿ ( new_size != table->size )
    {
      FT_Mem_ಗಂಟು*  new_buckets;
      FT_Long      i;


      new_buckets = (FT_Mem_ಗಂಟು *)
                      ft_mem_table_alloc(
                        table,
                        new_size * (FT_Long)ಪರಿಮಾಣ ( FT_Mem_ಗಂಟು ) );
      ಯದಿ ( !new_buckets )
        ಪ್ರತ್ಯಾಗಮ;

      FT_ARRAY_ZERO( new_buckets, new_size );

      ಪ್ರತಿ ( i = 0; i < table->size; i++ )
      {
        FT_Mem_ಗಂಟು  ಗಂಟು, next, *pnode;
        FT_PtrDist  hash;


        ಗಂಟು = table->buckets[i];
        ಯಾವತ್ ( ಗಂಟು )
        {
          next  = ಗಂಟು->link;
          hash  = FT_MEM_VAL( ಗಂಟು->address ) % (FT_PtrDist)new_size;
          pnode = new_buckets + hash;

          ಗಂಟು->link = pnode[0];
          pnode[0]   = ಗಂಟು;

          ಗಂಟು = next;
        }
      }

      ಯದಿ ( table->buckets )
        ft_mem_table_free( table, table->buckets );

      table->buckets = new_buckets;
      table->size    = new_size;
    }
  }


  ಸ್ಥಿರ ಶೂನ್ಯ
  ft_mem_table_destroy( FT_MemTable  table )
  {
    FT_Long  i;
    FT_Long  leak_ಎಣಿಕೆ = 0;
    FT_Long  leaks      = 0;


    /* remove all blocks from the table, revealing leaked ones */
    ಪ್ರತಿ ( i = 0; i < table->size; i++ )
    {
      FT_Mem_ಗಂಟು  *pnode = table->buckets + i, next, ಗಂಟು = *pnode;


      ಯಾವತ್ ( ಗಂಟು )
      {
        next       = ಗಂಟು->link;
        ಗಂಟು->link = ರಿಕ್ತ;

        ಯದಿ ( ಗಂಟು->size > 0 )
        {
          printf(
            "leaked memory block at address %p, size %8ld in (%s:%ld)\n",
            (ಶೂನ್ಯ*)ಗಂಟು->address,
            ಗಂಟು->size,
            FT_FILENAME( ಗಂಟು->source->file_name ),
            ಗಂಟು->source->ಸಾಲು_no );

          leak_ಎಣಿಕೆ++;
          leaks += ಗಂಟು->size;

          ft_mem_table_free( table, ಗಂಟು->address );
        }

        ಗಂಟು->address = ರಿಕ್ತ;
        ಗಂಟು->size    = 0;

        ft_mem_table_free( table, ಗಂಟು );
        ಗಂಟು = next;
      }
      table->buckets[i] = ರಿಕ್ತ;
    }

    ft_mem_table_free( table, table->buckets );
    table->buckets = ರಿಕ್ತ;

    table->size  = 0;
    table->nodes = 0;

    /* remove all sources */
    ಪ್ರತಿ ( i = 0; i < FT_MEM_SOURCE_BUCKETS; i++ )
    {
      FT_MemSource  source, next;


      ಪ್ರತಿ ( source = table->sources[i]; source != ರಿಕ್ತ; source = next )
      {
        next = source->link;
        ft_mem_table_free( table, source );
      }

      table->sources[i] = ರಿಕ್ತ;
    }

    printf( "FreeType: total memory allocations = %ld\n",
            table->alloc_total );
    printf( "FreeType: maximum memory footprint = %ld\n",
            table->alloc_max );

    ಯದಿ ( leak_ಎಣಿಕೆ > 0 )
      ft_mem_debug_panic(
        "FreeType: %ld bytes of memory leaked in %ld blocks\n",
        leaks, leak_ಎಣಿಕೆ );

    printf( "FreeType: no memory leaks detected\n" );
  }


  ಸ್ಥಿರ FT_Mem_ಗಂಟು*
  ft_mem_table_get_nodep( FT_MemTable  table,
                          FT_Byte*     address )
  {
    FT_PtrDist   hash;
    FT_Mem_ಗಂಟು  *pnode, ಗಂಟು;


    hash  = FT_MEM_VAL( address );
    pnode = table->buckets + ( hash % (FT_PtrDist)table->size );

    ಪ್ರತಿ (;;)
    {
      ಗಂಟು = pnode[0];
      ಯದಿ ( !ಗಂಟು )
        ಭಙ್ಗ;

      ಯದಿ ( ಗಂಟು->address == address )
        ಭಙ್ಗ;

      pnode = &ಗಂಟು->link;
    }
    ಪ್ರತ್ಯಾಗಮ pnode;
  }


  ಸ್ಥಿರ FT_MemSource
  ft_mem_table_get_source( FT_MemTable  table )
  {
    FT_UInt32     hash;
    FT_MemSource  ಗಂಟು, *pnode;


    /* cast to FT_PtrDist first since void* can be larger */
    /* than FT_UInt32 and GCC 4.1.1 emits a warning       */
    hash  = (FT_UInt32)(FT_PtrDist)(ಶೂನ್ಯ*)ft_debug_file_ +
              (FT_UInt32)( 5 * ft_debug_lineno_ );
    pnode = &table->sources[hash % FT_MEM_SOURCE_BUCKETS];

    ಪ್ರತಿ (;;)
    {
      ಗಂಟು = *pnode;
      ಯದಿ ( !ಗಂಟು )
        ಭಙ್ಗ;

      ಯದಿ ( ಗಂಟು->file_name == ft_debug_file_   &&
           ಗಂಟು->ಸಾಲು_no   == ft_debug_lineno_ )
        ಗಚ್ಛ Exit;

      pnode = &ಗಂಟು->link;
    }

    ಗಂಟು = (FT_MemSource)ft_mem_table_alloc( table, ಪರಿಮಾಣ ( *ಗಂಟು ) );
    ಯದಿ ( !ಗಂಟು )
      ft_mem_debug_panic(
        "not enough memory to perform memory debugging\n" );

    ಗಂಟು->file_name = ft_debug_file_;
    ಗಂಟು->ಸಾಲು_no   = ft_debug_lineno_;

    ಗಂಟು->cur_blocks = 0;
    ಗಂಟು->max_blocks = 0;
    ಗಂಟು->all_blocks = 0;

    ಗಂಟು->cur_size = 0;
    ಗಂಟು->max_size = 0;
    ಗಂಟು->all_size = 0;

    ಗಂಟು->cur_max = 0;

    ಗಂಟು->link = ರಿಕ್ತ;
    ಗಂಟು->hash = hash;
    *pnode     = ಗಂಟು;

  Exit:
    ಪ್ರತ್ಯಾಗಮ ಗಂಟು;
  }


  ಸ್ಥಿರ ಶೂನ್ಯ
  ft_mem_table_set( FT_MemTable  table,
                    FT_Byte*     address,
                    FT_Long      size,
                    FT_Long      delta )
  {
    FT_Mem_ಗಂಟು  *pnode, ಗಂಟು;


    ಯದಿ ( table )
    {
      FT_MemSource  source;


      pnode = ft_mem_table_get_nodep( table, address );
      ಗಂಟು  = *pnode;
      ಯದಿ ( ಗಂಟು )
      {
        ಯದಿ ( ಗಂಟು->size < 0 )
        {
          /* This block was already freed.  Our memory is now completely */
          /* corrupted!                                                  */
          /* This can only happen in keep-alive mode.                    */
          ft_mem_debug_panic(
            "memory heap corrupted (allocating freed block)" );
        }
        ಅನ್ಯಥಾ
        {
          /* This block was already allocated.  This means that our memory */
          /* is also corrupted!                                            */
          ft_mem_debug_panic(
            "memory heap corrupted (re-allocating allocated block at"
            " %p, of size %ld)\n"
            "org=%s:%d new=%s:%d\n",
            ಗಂಟು->address, ಗಂಟು->size,
            FT_FILENAME( ಗಂಟು->source->file_name ), ಗಂಟು->source->ಸಾಲು_no,
            FT_FILENAME( ft_debug_file_ ), ft_debug_lineno_ );
        }
      }

      /* we need to create a new node in this table */
      ಗಂಟು = (FT_Mem_ಗಂಟು)ft_mem_table_alloc( table, ಪರಿಮಾಣ ( *ಗಂಟು ) );
      ಯದಿ ( !ಗಂಟು )
        ft_mem_debug_panic( "not enough memory to run memory tests" );

      ಗಂಟು->address = address;
      ಗಂಟು->size    = size;
      ಗಂಟು->source  = source = ft_mem_table_get_source( table );

      ಯದಿ ( delta == 0 )
      {
        /* this is an allocation */
        source->all_blocks++;
        source->cur_blocks++;
        ಯದಿ ( source->cur_blocks > source->max_blocks )
          source->max_blocks = source->cur_blocks;
      }

      ಯದಿ ( size > source->cur_max )
        source->cur_max = size;

      ಯದಿ ( delta != 0 )
      {
        /* we are growing or shrinking a reallocated block */
        source->cur_size     += delta;
        table->alloc_current += delta;
      }
      ಅನ್ಯಥಾ
      {
        /* we are allocating a new block */
        source->cur_size     += size;
        table->alloc_current += size;
      }

      source->all_size += size;

      ಯದಿ ( source->cur_size > source->max_size )
        source->max_size = source->cur_size;

      ಗಂಟು->free_file_name = ರಿಕ್ತ;
      ಗಂಟು->free_ಸಾಲು_no   = 0;

      ಗಂಟು->link = pnode[0];

      pnode[0] = ಗಂಟು;
      table->nodes++;

      table->alloc_total += size;

      ಯದಿ ( table->alloc_current > table->alloc_max )
        table->alloc_max = table->alloc_current;

      ಯದಿ ( table->nodes * 3 < table->size  ||
           table->size  * 3 < table->nodes )
        ft_mem_table_resize( table );
    }
  }


  ಸ್ಥಿರ ಶೂನ್ಯ
  ft_mem_table_remove( FT_MemTable  table,
                       FT_Byte*     address,
                       FT_Long      delta )
  {
    ಯದಿ ( table )
    {
      FT_Mem_ಗಂಟು  *pnode, ಗಂಟು;


      pnode = ft_mem_table_get_nodep( table, address );
      ಗಂಟು  = *pnode;
      ಯದಿ ( ಗಂಟು )
      {
        FT_MemSource  source;


        ಯದಿ ( ಗಂಟು->size < 0 )
          ft_mem_debug_panic(
            "freeing memory block at %p more than once\n"
            "  at (%s:%ld)!\n"
            "  Block was allocated at (%s:%ld)\n"
            "  and released at (%s:%ld).",
            address,
            FT_FILENAME( ft_debug_file_ ), ft_debug_lineno_,
            FT_FILENAME( ಗಂಟು->source->file_name ), ಗಂಟು->source->ಸಾಲು_no,
            FT_FILENAME( ಗಂಟು->free_file_name ), ಗಂಟು->free_ಸಾಲು_no );

        /* scramble the node's content for additional safety */
        FT_MEM_SET( address, 0xF3, ಗಂಟು->size );

        ಯದಿ ( delta == 0 )
        {
          source = ಗಂಟು->source;

          source->cur_blocks--;
          source->cur_size -= ಗಂಟು->size;

          table->alloc_current -= ಗಂಟು->size;
        }

        ಯದಿ ( table->keep_alive )
        {
          /* we simply invert the node's size to indicate that the node */
          /* was freed.                                                 */
          ಗಂಟು->size           = -ಗಂಟು->size;
          ಗಂಟು->free_file_name = ft_debug_file_;
          ಗಂಟು->free_ಸಾಲು_no   = ft_debug_lineno_;
        }
        ಅನ್ಯಥಾ
        {
          table->nodes--;

          *pnode = ಗಂಟು->link;

          ಗಂಟು->size   = 0;
          ಗಂಟು->source = ರಿಕ್ತ;

          ft_mem_table_free( table, ಗಂಟು );

          ಯದಿ ( table->nodes * 3 < table->size  ||
               table->size  * 3 < table->nodes )
            ft_mem_table_resize( table );
        }
      }
      ಅನ್ಯಥಾ
        ft_mem_debug_panic(
          "trying to free unknown block at %p in (%s:%ld)\n",
          address,
          FT_FILENAME( ft_debug_file_ ), ft_debug_lineno_ );
    }
  }


  ಸ್ಥಿರ FT_Pointer
  ft_mem_debug_alloc( FT_Memory  memory,
                      FT_Long    size )
  {
    FT_MemTable  table = (FT_MemTable)memory->user;
    FT_Byte*     block;


    ಯದಿ ( size <= 0 )
      ft_mem_debug_panic( "negative block size allocation (%ld)", size );

    /* return NULL if the maximum number of allocations was reached */
    ಯದಿ ( table->bound_ಎಣಿಕೆ                           &&
         table->alloc_ಎಣಿಕೆ >= table->alloc_ಎಣಿಕೆ_max )
      ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

    /* return NULL if this allocation would overflow the maximum heap size */
    ಯದಿ ( table->bound_total                                   &&
         table->alloc_total_max - table->alloc_current > size )
      ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

    block = (FT_Byte *)ft_mem_table_alloc( table, size );
    ಯದಿ ( block )
    {
      ft_mem_table_set( table, block, size, 0 );

      table->alloc_ಎಣಿಕೆ++;
    }

    ft_debug_file_   = "<unknown>";
    ft_debug_lineno_ = 0;

    ಪ್ರತ್ಯಾಗಮ (FT_Pointer)block;
  }


  ಸ್ಥಿರ ಶೂನ್ಯ
  ft_mem_debug_free( FT_Memory   memory,
                     FT_Pointer  block )
  {
    FT_MemTable  table = (FT_MemTable)memory->user;


    ಯದಿ ( !block )
      ft_mem_debug_panic( "trying to free NULL in (%s:%ld)",
                          FT_FILENAME( ft_debug_file_ ),
                          ft_debug_lineno_ );

    ft_mem_table_remove( table, (FT_Byte*)block, 0 );

    ಯದಿ ( !table->keep_alive )
      ft_mem_table_free( table, block );

    table->alloc_ಎಣಿಕೆ--;

    ft_debug_file_   = "<unknown>";
    ft_debug_lineno_ = 0;
  }


  ಸ್ಥಿರ FT_Pointer
  ft_mem_debug_realloc( FT_Memory   memory,
                        FT_Long     cur_size,
                        FT_Long     new_size,
                        FT_Pointer  block )
  {
    FT_MemTable  table = (FT_MemTable)memory->user;
    FT_Mem_ಗಂಟು   ಗಂಟು, *pnode;
    FT_Pointer   new_block;
    FT_Long      delta;

    ಧ್ರುವ ವರ್ಣ*  file_name = FT_FILENAME( ft_debug_file_ );
    FT_Long      ಸಾಲು_no   = ft_debug_lineno_;


    /* unlikely, but possible */
    ಯದಿ ( new_size == cur_size )
      ಪ್ರತ್ಯಾಗಮ block;

    /* the following is valid according to ANSI C */
#ಯದಿ 0
    ಯದಿ ( !block || !cur_size )
      ft_mem_debug_panic( "trying to reallocate NULL in (%s:%ld)",
                          file_name, ಸಾಲು_no );
#endif

    /* while the following is allowed in ANSI C also, we abort since */
    /* such case should be handled by FreeType.                      */
    ಯದಿ ( new_size <= 0 )
      ft_mem_debug_panic(
        "trying to reallocate %p to size 0 (current is %ld) in (%s:%ld)",
        block, cur_size, file_name, ಸಾಲು_no );

    /* check `cur_size' value */
    pnode = ft_mem_table_get_nodep( table, (FT_Byte*)block );
    ಗಂಟು  = *pnode;
    ಯದಿ ( !ಗಂಟು )
      ft_mem_debug_panic(
        "trying to reallocate unknown block at %p in (%s:%ld)",
        block, file_name, ಸಾಲು_no );

    ಯದಿ ( ಗಂಟು->size <= 0 )
      ft_mem_debug_panic(
        "trying to reallocate freed block at %p in (%s:%ld)",
        block, file_name, ಸಾಲು_no );

    ಯದಿ ( ಗಂಟು->size != cur_size )
      ft_mem_debug_panic( "invalid ft_realloc request for %p. cur_size is "
                          "%ld instead of %ld in (%s:%ld)",
                          block, cur_size, ಗಂಟು->size, file_name, ಸಾಲು_no );

    /* return NULL if the maximum number of allocations was reached */
    ಯದಿ ( table->bound_ಎಣಿಕೆ                           &&
         table->alloc_ಎಣಿಕೆ >= table->alloc_ಎಣಿಕೆ_max )
      ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

    delta = new_size - cur_size;

    /* return NULL if this allocation would overflow the maximum heap size */
    ಯದಿ ( delta > 0                                             &&
         table->bound_total                                    &&
         table->alloc_current + delta > table->alloc_total_max )
      ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

    new_block = (FT_Pointer)ft_mem_table_alloc( table, new_size );
    ಯದಿ ( !new_block )
      ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

    ft_mem_table_set( table, (FT_Byte*)new_block, new_size, delta );

    ft_memcpy( new_block, block, cur_size < new_size ? (size_t)cur_size
                                                     : (size_t)new_size );

    ft_mem_table_remove( table, (FT_Byte*)block, delta );

    ft_debug_file_   = "<unknown>";
    ft_debug_lineno_ = 0;

    ಯದಿ ( !table->keep_alive )
      ft_mem_table_free( table, block );

    ಪ್ರತ್ಯಾಗಮ new_block;
  }


  ಬಾಹ್ಯ ಶೂನ್ಯ
  ft_mem_debug_init( FT_Memory  memory )
  {
    FT_MemTable  table;


    ಯದಿ ( !ft_getenv( "FT2_DEBUG_MEMORY" ) )
      ಪ್ರತ್ಯಾಗಮ;

    table = (FT_MemTable)memory->alloc( memory, ಪರಿಮಾಣ ( *table ) );

    ಯದಿ ( table )
    {
      FT_ZERO( table );

      table->memory      = memory;
      table->memory_user = memory->user;
      table->alloc       = memory->alloc;
      table->realloc     = memory->realloc;
      table->ಮುಕ್ತ        = memory->ಮುಕ್ತ;

      ft_mem_table_resize( table );

      ಯದಿ ( table->size )
      {
        ಧ್ರುವ ವರ್ಣ*  p;


        memory->user    = table;
        memory->alloc   = ft_mem_debug_alloc;
        memory->realloc = ft_mem_debug_realloc;
        memory->ಮುಕ್ತ    = ft_mem_debug_free;

        p = ft_getenv( "FT2_ALLOC_TOTAL_MAX" );
        ಯದಿ ( p )
        {
          FT_Long  total_max = ft_strtol( p, ರಿಕ್ತ, 10 );


          ಯದಿ ( total_max > 0 )
          {
            table->bound_total     = 1;
            table->alloc_total_max = total_max;
          }
        }

        p = ft_getenv( "FT2_ALLOC_COUNT_MAX" );
        ಯದಿ ( p )
        {
          FT_Long  total_ಎಣಿಕೆ = ft_strtol( p, ರಿಕ್ತ, 10 );


          ಯದಿ ( total_ಎಣಿಕೆ > 0 )
          {
            table->bound_ಎಣಿಕೆ     = 1;
            table->alloc_ಎಣಿಕೆ_max = total_ಎಣಿಕೆ;
          }
        }

        p = ft_getenv( "FT2_KEEP_ALIVE" );
        ಯದಿ ( p )
        {
          FT_Long  keep_alive = ft_strtol( p, ರಿಕ್ತ, 10 );


          ಯದಿ ( keep_alive > 0 )
            table->keep_alive = 1;
        }
      }
      ಅನ್ಯಥಾ
        memory->ಮುಕ್ತ( memory, table );
    }
  }


  ಬಾಹ್ಯ ಶೂನ್ಯ
  ft_mem_debug_done( FT_Memory  memory )
  {
    ಯದಿ ( memory->ಮುಕ್ತ == ft_mem_debug_free )
    {
      FT_MemTable  table = (FT_MemTable)memory->user;


      FT_DumpMemory( memory );

      ft_mem_table_destroy( table );

      memory->ಮುಕ್ತ    = table->ಮುಕ್ತ;
      memory->realloc = table->realloc;
      memory->alloc   = table->alloc;
      memory->user    = table->memory_user;

      memory->ಮುಕ್ತ( memory, table );
    }
  }


  FT_COMPARE_DEF( ಪೂರ್ಣಾಙ್ಕ )
  ft_mem_source_compare( ಧ್ರುವ ಶೂನ್ಯ*  p1,
                         ಧ್ರುವ ಶೂನ್ಯ*  p2 )
  {
    FT_MemSource  s1 = *(FT_MemSource*)p1;
    FT_MemSource  s2 = *(FT_MemSource*)p2;


    ಯದಿ ( s2->max_size > s1->max_size )
      ಪ್ರತ್ಯಾಗಮ 1;
    ಅನ್ಯಥಾ ಯದಿ ( s2->max_size < s1->max_size )
      ಪ್ರತ್ಯಾಗಮ -1;
    ಅನ್ಯಥಾ
      ಪ್ರತ್ಯಾಗಮ 0;
  }


  ಬಾಹ್ಯ ಶೂನ್ಯ
  FT_DumpMemory( FT_Memory  memory )
  {
    ಯದಿ ( memory->ಮುಕ್ತ == ft_mem_debug_free )
    {
      FT_MemTable    table = (FT_MemTable)memory->user;
      FT_MemSource*  bucket = table->sources;
      FT_MemSource*  limit  = bucket + FT_MEM_SOURCE_BUCKETS;
      FT_MemSource*  sources;
      FT_Int         nn, ಎಣಿಕೆ;
      ಧ್ರುವ ವರ್ಣ*    fmt;


      ಎಣಿಕೆ = 0;
      ಪ್ರತಿ ( ; bucket < limit; bucket++ )
      {
        FT_MemSource  source = *bucket;


        ಪ್ರತಿ ( ; source; source = source->link )
          ಎಣಿಕೆ++;
      }

      sources = (FT_MemSource*)
                  ft_mem_table_alloc(
                    table, ಎಣಿಕೆ * (FT_Long)ಪರಿಮಾಣ ( *sources ) );

      ಎಣಿಕೆ = 0;
      ಪ್ರತಿ ( bucket = table->sources; bucket < limit; bucket++ )
      {
        FT_MemSource  source = *bucket;


        ಪ್ರತಿ ( ; source; source = source->link )
          sources[ಎಣಿಕೆ++] = source;
      }

      ft_qsort( sources,
                (size_t)ಎಣಿಕೆ,
                ಪರಿಮಾಣ ( *sources ),
                ft_mem_source_compare );

      printf( "FreeType Memory Dump: "
              "current=%ld max=%ld total=%ld count=%ld\n",
              table->alloc_current, table->alloc_max,
              table->alloc_total, table->alloc_ಎಣಿಕೆ );
      printf( " block  block    sizes    sizes    sizes   source\n" );
      printf( " count   high      sum  highsum      max   location\n" );
      printf( "-------------------------------------------------\n" );

      fmt = "%6ld %6ld %8ld %8ld %8ld %s:%d\n";

      ಪ್ರತಿ ( nn = 0; nn < ಎಣಿಕೆ; nn++ )
      {
        FT_MemSource  source = sources[nn];


        printf( fmt,
                source->cur_blocks, source->max_blocks,
                source->cur_size, source->max_size, source->cur_max,
                FT_FILENAME( source->file_name ),
                source->ಸಾಲು_no );
      }
      printf( "------------------------------------------------\n" );

      ft_mem_table_free( table, sources );
    }
  }

#ಅನ್ಯಥಾ  /* !FT_DEBUG_MEMORY */

  /* ANSI C doesn't like empty source files */
  ಪ್ರಕಾರನಿರ್ದೇಶ ಪೂರ್ಣಾಙ್ಕ  debug_mem_dummy_;

#endif /* !FT_DEBUG_MEMORY */


/* END */
