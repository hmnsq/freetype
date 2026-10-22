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


#অন্তর্ভাৱ <ft2build.h>
#অন্তর্ভাৱ FT_CONFIG_CONFIG_H
#অন্তর্ভাৱ <freetype/internal/ftdebug.h>
#অন্তর্ভাৱ <freetype/internal/ftmemory.h>
#অন্তর্ভাৱ <freetype/ftsystem.h>
#অন্তর্ভাৱ <freetype/fterrors.h>
#অন্তর্ভাৱ <freetype/fttypes.h>


#ifdef FT_DEBUG_MEMORY

#define  KEEPALIVE /* `Keep alive' means that freed blocks aren't released
                    * to the heap.  This is useful to detect double-frees
                    * or weird heap corruption, but it uses large amounts of
                    * memory, however.
                    */

#অন্তর্ভাৱ FT_CONFIG_STANDARD_LIBRARY_H

  FT_BASE_DEF( ধ্রুৱ ৱর্ণ* )  ft_debug_file_   = রিক্ত;
  FT_BASE_DEF( long )         ft_debug_lineno_ = 0;

  বাহ্য শূন্য
  FT_DumpMemory( FT_Memory  memory );


  প্রকারনির্দেশ সংরচনা FT_MemSourceRec_*  FT_MemSource;
  প্রকারনির্দেশ সংরচনা FT_Mem_গ্ৰন্থি_Rec*    FT_Mem_গ্ৰন্থি;
  প্রকারনির্দেশ সংরচনা FT_MemTableRec_*   FT_MemTable;


#define FT_MEM_VAL( addr )  ( (FT_PtrDist)(FT_Pointer)( addr ) )

  /*
   * This structure holds statistics for a single allocation/release
   * site.  This is useful to know where memory operations happen the
   * most.
   */
  প্রকারনির্দেশ সংরচনা  FT_MemSourceRec_
  {
    ধ্রুৱ ৱর্ণ*   file_name;
    long          সাৰি_no;

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
  প্রকারনির্দেশ সংরচনা  FT_Mem_গ্ৰন্থি_Rec
  {
    FT_Byte*      address;
    FT_Long       size;     /* < 0 if the block was freed */

    FT_MemSource  source;

#ifdef KEEPALIVE
    ধ্রুৱ ৱর্ণ*   free_file_name;
    FT_Long       free_সাৰি_no;
#endif

    FT_Mem_গ্ৰন্থি    link;

  } FT_Mem_গ্ৰন্থি_Rec_2;


  /*
   * The global structure, containing compound statistics and all hash
   * tables.
   */
  প্রকারনির্দেশ সংরচনা  FT_MemTableRec_
  {
    FT_Long          size;
    FT_Long          nodes;
    FT_Mem_গ্ৰন্থি*      buckets;

    FT_Long          alloc_total;
    FT_Long          alloc_current;
    FT_Long          alloc_max;
    FT_Long          alloc_গুনতি;

    FT_Bool          bound_total;
    FT_Long          alloc_total_max;

    FT_Bool          bound_গুনতি;
    FT_Long          alloc_গুনতি_max;

    FT_MemSource     sources[FT_MEM_SOURCE_BUCKETS];

    FT_Bool          keep_alive;

    FT_Memory        memory;
    FT_Pointer       memory_user;
    FT_Alloc_Func    alloc;
    FT_Free_Func     মুক্ত;
    FT_Realloc_Func  realloc;

  } FT_MemTableRec;


#define FT_MEM_SIZE_MAX  13845163

#define FT_FILENAME( x )  ( (x) ? (x) : "unknown file" )


  /*
   * Prime numbers are ugly to handle.  It would be better to implement
   * L-Hashing, which is 10% faster and doesn't require divisions.
   */
  স্থির ধ্রুৱ FT_Int  ft_mem_primes[] =
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


  স্থির FT_Long
  ft_mem_closest_মৌলিক( FT_Long  অঙ্ক )
  {
    size_t  i;


    বাবে ( i = 0;
          i < পরিমাণ ( ft_mem_primes ) / পরিমাণ ( ft_mem_primes[0] ); i++ )
      যদি ( ft_mem_primes[i] > অঙ্ক )
        ঘূৰাই ft_mem_primes[i];

    ঘূৰাই FT_MEM_SIZE_MAX;
  }


  স্থির শূন্য
  ft_mem_debug_panic( ধ্রুৱ ৱর্ণ*  fmt,
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


  স্থির FT_Pointer
  ft_mem_table_alloc( FT_MemTable  table,
                      FT_Long      size )
  {
    FT_Memory   memory = table->memory;
    FT_Pointer  block;


    memory->user = table->memory_user;
    block = table->alloc( memory, size );
    memory->user = table;

    ঘূৰাই block;
  }


  স্থির শূন্য
  ft_mem_table_free( FT_MemTable  table,
                     FT_Pointer   block )
  {
    FT_Memory  memory = table->memory;


    memory->user = table->memory_user;
    table->মুক্ত( memory, block );
    memory->user = table;
  }


  স্থির শূন্য
  ft_mem_table_resize( FT_MemTable  table )
  {
    FT_Long  new_size;


    new_size = ft_mem_closest_মৌলিক( table->nodes );
    যদি ( new_size != table->size )
    {
      FT_Mem_গ্ৰন্থি*  new_buckets;
      FT_Long      i;


      new_buckets = (FT_Mem_গ্ৰন্থি *)
                      ft_mem_table_alloc(
                        table,
                        new_size * (FT_Long)পরিমাণ ( FT_Mem_গ্ৰন্থি ) );
      যদি ( !new_buckets )
        ঘূৰাই;

      FT_ARRAY_ZERO( new_buckets, new_size );

      বাবে ( i = 0; i < table->size; i++ )
      {
        FT_Mem_গ্ৰন্থি  গ্ৰন্থি, next, *pnode;
        FT_PtrDist  hash;


        গ্ৰন্থি = table->buckets[i];
        যেতিয়ালৈ ( গ্ৰন্থি )
        {
          next  = গ্ৰন্থি->link;
          hash  = FT_MEM_VAL( গ্ৰন্থি->address ) % (FT_PtrDist)new_size;
          pnode = new_buckets + hash;

          গ্ৰন্থি->link = pnode[0];
          pnode[0]   = গ্ৰন্থি;

          গ্ৰন্থি = next;
        }
      }

      যদি ( table->buckets )
        ft_mem_table_free( table, table->buckets );

      table->buckets = new_buckets;
      table->size    = new_size;
    }
  }


  স্থির শূন্য
  ft_mem_table_destroy( FT_MemTable  table )
  {
    FT_Long  i;
    FT_Long  leak_গুনতি = 0;
    FT_Long  leaks      = 0;


    /* remove all blocks from the table, revealing leaked ones */
    বাবে ( i = 0; i < table->size; i++ )
    {
      FT_Mem_গ্ৰন্থি  *pnode = table->buckets + i, next, গ্ৰন্থি = *pnode;


      যেতিয়ালৈ ( গ্ৰন্থি )
      {
        next       = গ্ৰন্থি->link;
        গ্ৰন্থি->link = রিক্ত;

        যদি ( গ্ৰন্থি->size > 0 )
        {
          printf(
            "leaked memory block at address %p, size %8ld in (%s:%ld)\n",
            (শূন্য*)গ্ৰন্থি->address,
            গ্ৰন্থি->size,
            FT_FILENAME( গ্ৰন্থি->source->file_name ),
            গ্ৰন্থি->source->সাৰি_no );

          leak_গুনতি++;
          leaks += গ্ৰন্থি->size;

          ft_mem_table_free( table, গ্ৰন্থি->address );
        }

        গ্ৰন্থি->address = রিক্ত;
        গ্ৰন্থি->size    = 0;

        ft_mem_table_free( table, গ্ৰন্থি );
        গ্ৰন্থি = next;
      }
      table->buckets[i] = রিক্ত;
    }

    ft_mem_table_free( table, table->buckets );
    table->buckets = রিক্ত;

    table->size  = 0;
    table->nodes = 0;

    /* remove all sources */
    বাবে ( i = 0; i < FT_MEM_SOURCE_BUCKETS; i++ )
    {
      FT_MemSource  source, next;


      বাবে ( source = table->sources[i]; source != রিক্ত; source = next )
      {
        next = source->link;
        ft_mem_table_free( table, source );
      }

      table->sources[i] = রিক্ত;
    }

    printf( "FreeType: total memory allocations = %ld\n",
            table->alloc_total );
    printf( "FreeType: maximum memory footprint = %ld\n",
            table->alloc_max );

    যদি ( leak_গুনতি > 0 )
      ft_mem_debug_panic(
        "FreeType: %ld bytes of memory leaked in %ld blocks\n",
        leaks, leak_গুনতি );

    printf( "FreeType: no memory leaks detected\n" );
  }


  স্থির FT_Mem_গ্ৰন্থি*
  ft_mem_table_get_nodep( FT_MemTable  table,
                          FT_Byte*     address )
  {
    FT_PtrDist   hash;
    FT_Mem_গ্ৰন্থি  *pnode, গ্ৰন্থি;


    hash  = FT_MEM_VAL( address );
    pnode = table->buckets + ( hash % (FT_PtrDist)table->size );

    বাবে (;;)
    {
      গ্ৰন্থি = pnode[0];
      যদি ( !গ্ৰন্থি )
        ভঙ্গ;

      যদি ( গ্ৰন্থি->address == address )
        ভঙ্গ;

      pnode = &গ্ৰন্থি->link;
    }
    ঘূৰাই pnode;
  }


  স্থির FT_MemSource
  ft_mem_table_get_source( FT_MemTable  table )
  {
    FT_UInt32     hash;
    FT_MemSource  গ্ৰন্থি, *pnode;


    /* cast to FT_PtrDist first since void* can be larger */
    /* than FT_UInt32 and GCC 4.1.1 emits a warning       */
    hash  = (FT_UInt32)(FT_PtrDist)(শূন্য*)ft_debug_file_ +
              (FT_UInt32)( 5 * ft_debug_lineno_ );
    pnode = &table->sources[hash % FT_MEM_SOURCE_BUCKETS];

    বাবে (;;)
    {
      গ্ৰন্থি = *pnode;
      যদি ( !গ্ৰন্থি )
        ভঙ্গ;

      যদি ( গ্ৰন্থি->file_name == ft_debug_file_   &&
           গ্ৰন্থি->সাৰি_no   == ft_debug_lineno_ )
        গচ্ছ Exit;

      pnode = &গ্ৰন্থি->link;
    }

    গ্ৰন্থি = (FT_MemSource)ft_mem_table_alloc( table, পরিমাণ ( *গ্ৰন্থি ) );
    যদি ( !গ্ৰন্থি )
      ft_mem_debug_panic(
        "not enough memory to perform memory debugging\n" );

    গ্ৰন্থি->file_name = ft_debug_file_;
    গ্ৰন্থি->সাৰি_no   = ft_debug_lineno_;

    গ্ৰন্থি->cur_blocks = 0;
    গ্ৰন্থি->max_blocks = 0;
    গ্ৰন্থি->all_blocks = 0;

    গ্ৰন্থি->cur_size = 0;
    গ্ৰন্থি->max_size = 0;
    গ্ৰন্থি->all_size = 0;

    গ্ৰন্থি->cur_max = 0;

    গ্ৰন্থি->link = রিক্ত;
    গ্ৰন্থি->hash = hash;
    *pnode     = গ্ৰন্থি;

  Exit:
    ঘূৰাই গ্ৰন্থি;
  }


  স্থির শূন্য
  ft_mem_table_set( FT_MemTable  table,
                    FT_Byte*     address,
                    FT_Long      size,
                    FT_Long      delta )
  {
    FT_Mem_গ্ৰন্থি  *pnode, গ্ৰন্থি;


    যদি ( table )
    {
      FT_MemSource  source;


      pnode = ft_mem_table_get_nodep( table, address );
      গ্ৰন্থি  = *pnode;
      যদি ( গ্ৰন্থি )
      {
        যদি ( গ্ৰন্থি->size < 0 )
        {
          /* This block was already freed.  Our memory is now completely */
          /* corrupted!                                                  */
          /* This can only happen in keep-alive mode.                    */
          ft_mem_debug_panic(
            "memory heap corrupted (allocating freed block)" );
        }
        অন্যথা
        {
          /* This block was already allocated.  This means that our memory */
          /* is also corrupted!                                            */
          ft_mem_debug_panic(
            "memory heap corrupted (re-allocating allocated block at"
            " %p, of size %ld)\n"
            "org=%s:%d new=%s:%d\n",
            গ্ৰন্থি->address, গ্ৰন্থি->size,
            FT_FILENAME( গ্ৰন্থি->source->file_name ), গ্ৰন্থি->source->সাৰি_no,
            FT_FILENAME( ft_debug_file_ ), ft_debug_lineno_ );
        }
      }

      /* we need to create a new node in this table */
      গ্ৰন্থি = (FT_Mem_গ্ৰন্থি)ft_mem_table_alloc( table, পরিমাণ ( *গ্ৰন্থি ) );
      যদি ( !গ্ৰন্থি )
        ft_mem_debug_panic( "not enough memory to run memory tests" );

      গ্ৰন্থি->address = address;
      গ্ৰন্থি->size    = size;
      গ্ৰন্থি->source  = source = ft_mem_table_get_source( table );

      যদি ( delta == 0 )
      {
        /* this is an allocation */
        source->all_blocks++;
        source->cur_blocks++;
        যদি ( source->cur_blocks > source->max_blocks )
          source->max_blocks = source->cur_blocks;
      }

      যদি ( size > source->cur_max )
        source->cur_max = size;

      যদি ( delta != 0 )
      {
        /* we are growing or shrinking a reallocated block */
        source->cur_size     += delta;
        table->alloc_current += delta;
      }
      অন্যথা
      {
        /* we are allocating a new block */
        source->cur_size     += size;
        table->alloc_current += size;
      }

      source->all_size += size;

      যদি ( source->cur_size > source->max_size )
        source->max_size = source->cur_size;

      গ্ৰন্থি->free_file_name = রিক্ত;
      গ্ৰন্থি->free_সাৰি_no   = 0;

      গ্ৰন্থি->link = pnode[0];

      pnode[0] = গ্ৰন্থি;
      table->nodes++;

      table->alloc_total += size;

      যদি ( table->alloc_current > table->alloc_max )
        table->alloc_max = table->alloc_current;

      যদি ( table->nodes * 3 < table->size  ||
           table->size  * 3 < table->nodes )
        ft_mem_table_resize( table );
    }
  }


  স্থির শূন্য
  ft_mem_table_remove( FT_MemTable  table,
                       FT_Byte*     address,
                       FT_Long      delta )
  {
    যদি ( table )
    {
      FT_Mem_গ্ৰন্থি  *pnode, গ্ৰন্থি;


      pnode = ft_mem_table_get_nodep( table, address );
      গ্ৰন্থি  = *pnode;
      যদি ( গ্ৰন্থি )
      {
        FT_MemSource  source;


        যদি ( গ্ৰন্থি->size < 0 )
          ft_mem_debug_panic(
            "freeing memory block at %p more than once\n"
            "  at (%s:%ld)!\n"
            "  Block was allocated at (%s:%ld)\n"
            "  and released at (%s:%ld).",
            address,
            FT_FILENAME( ft_debug_file_ ), ft_debug_lineno_,
            FT_FILENAME( গ্ৰন্থি->source->file_name ), গ্ৰন্থি->source->সাৰি_no,
            FT_FILENAME( গ্ৰন্থি->free_file_name ), গ্ৰন্থি->free_সাৰি_no );

        /* scramble the node's content for additional safety */
        FT_MEM_SET( address, 0xF3, গ্ৰন্থি->size );

        যদি ( delta == 0 )
        {
          source = গ্ৰন্থি->source;

          source->cur_blocks--;
          source->cur_size -= গ্ৰন্থি->size;

          table->alloc_current -= গ্ৰন্থি->size;
        }

        যদি ( table->keep_alive )
        {
          /* we simply invert the node's size to indicate that the node */
          /* was freed.                                                 */
          গ্ৰন্থি->size           = -গ্ৰন্থি->size;
          গ্ৰন্থি->free_file_name = ft_debug_file_;
          গ্ৰন্থি->free_সাৰি_no   = ft_debug_lineno_;
        }
        অন্যথা
        {
          table->nodes--;

          *pnode = গ্ৰন্থি->link;

          গ্ৰন্থি->size   = 0;
          গ্ৰন্থি->source = রিক্ত;

          ft_mem_table_free( table, গ্ৰন্থি );

          যদি ( table->nodes * 3 < table->size  ||
               table->size  * 3 < table->nodes )
            ft_mem_table_resize( table );
        }
      }
      অন্যথা
        ft_mem_debug_panic(
          "trying to free unknown block at %p in (%s:%ld)\n",
          address,
          FT_FILENAME( ft_debug_file_ ), ft_debug_lineno_ );
    }
  }


  স্থির FT_Pointer
  ft_mem_debug_alloc( FT_Memory  memory,
                      FT_Long    size )
  {
    FT_MemTable  table = (FT_MemTable)memory->user;
    FT_Byte*     block;


    যদি ( size <= 0 )
      ft_mem_debug_panic( "negative block size allocation (%ld)", size );

    /* return NULL if the maximum number of allocations was reached */
    যদি ( table->bound_গুনতি                           &&
         table->alloc_গুনতি >= table->alloc_গুনতি_max )
      ঘূৰাই রিক্ত;

    /* return NULL if this allocation would overflow the maximum heap size */
    যদি ( table->bound_total                                   &&
         table->alloc_total_max - table->alloc_current > size )
      ঘূৰাই রিক্ত;

    block = (FT_Byte *)ft_mem_table_alloc( table, size );
    যদি ( block )
    {
      ft_mem_table_set( table, block, size, 0 );

      table->alloc_গুনতি++;
    }

    ft_debug_file_   = "<unknown>";
    ft_debug_lineno_ = 0;

    ঘূৰাই (FT_Pointer)block;
  }


  স্থির শূন্য
  ft_mem_debug_free( FT_Memory   memory,
                     FT_Pointer  block )
  {
    FT_MemTable  table = (FT_MemTable)memory->user;


    যদি ( !block )
      ft_mem_debug_panic( "trying to free NULL in (%s:%ld)",
                          FT_FILENAME( ft_debug_file_ ),
                          ft_debug_lineno_ );

    ft_mem_table_remove( table, (FT_Byte*)block, 0 );

    যদি ( !table->keep_alive )
      ft_mem_table_free( table, block );

    table->alloc_গুনতি--;

    ft_debug_file_   = "<unknown>";
    ft_debug_lineno_ = 0;
  }


  স্থির FT_Pointer
  ft_mem_debug_realloc( FT_Memory   memory,
                        FT_Long     cur_size,
                        FT_Long     new_size,
                        FT_Pointer  block )
  {
    FT_MemTable  table = (FT_MemTable)memory->user;
    FT_Mem_গ্ৰন্থি   গ্ৰন্থি, *pnode;
    FT_Pointer   new_block;
    FT_Long      delta;

    ধ্রুৱ ৱর্ণ*  file_name = FT_FILENAME( ft_debug_file_ );
    FT_Long      সাৰি_no   = ft_debug_lineno_;


    /* unlikely, but possible */
    যদি ( new_size == cur_size )
      ঘূৰাই block;

    /* the following is valid according to ANSI C */
#যদি 0
    যদি ( !block || !cur_size )
      ft_mem_debug_panic( "trying to reallocate NULL in (%s:%ld)",
                          file_name, সাৰি_no );
#endif

    /* while the following is allowed in ANSI C also, we abort since */
    /* such case should be handled by FreeType.                      */
    যদি ( new_size <= 0 )
      ft_mem_debug_panic(
        "trying to reallocate %p to size 0 (current is %ld) in (%s:%ld)",
        block, cur_size, file_name, সাৰি_no );

    /* check `cur_size' value */
    pnode = ft_mem_table_get_nodep( table, (FT_Byte*)block );
    গ্ৰন্থি  = *pnode;
    যদি ( !গ্ৰন্থি )
      ft_mem_debug_panic(
        "trying to reallocate unknown block at %p in (%s:%ld)",
        block, file_name, সাৰি_no );

    যদি ( গ্ৰন্থি->size <= 0 )
      ft_mem_debug_panic(
        "trying to reallocate freed block at %p in (%s:%ld)",
        block, file_name, সাৰি_no );

    যদি ( গ্ৰন্থি->size != cur_size )
      ft_mem_debug_panic( "invalid ft_realloc request for %p. cur_size is "
                          "%ld instead of %ld in (%s:%ld)",
                          block, cur_size, গ্ৰন্থি->size, file_name, সাৰি_no );

    /* return NULL if the maximum number of allocations was reached */
    যদি ( table->bound_গুনতি                           &&
         table->alloc_গুনতি >= table->alloc_গুনতি_max )
      ঘূৰাই রিক্ত;

    delta = new_size - cur_size;

    /* return NULL if this allocation would overflow the maximum heap size */
    যদি ( delta > 0                                             &&
         table->bound_total                                    &&
         table->alloc_current + delta > table->alloc_total_max )
      ঘূৰাই রিক্ত;

    new_block = (FT_Pointer)ft_mem_table_alloc( table, new_size );
    যদি ( !new_block )
      ঘূৰাই রিক্ত;

    ft_mem_table_set( table, (FT_Byte*)new_block, new_size, delta );

    ft_memcpy( new_block, block, cur_size < new_size ? (size_t)cur_size
                                                     : (size_t)new_size );

    ft_mem_table_remove( table, (FT_Byte*)block, delta );

    ft_debug_file_   = "<unknown>";
    ft_debug_lineno_ = 0;

    যদি ( !table->keep_alive )
      ft_mem_table_free( table, block );

    ঘূৰাই new_block;
  }


  বাহ্য শূন্য
  ft_mem_debug_init( FT_Memory  memory )
  {
    FT_MemTable  table;


    যদি ( !ft_getenv( "FT2_DEBUG_MEMORY" ) )
      ঘূৰাই;

    table = (FT_MemTable)memory->alloc( memory, পরিমাণ ( *table ) );

    যদি ( table )
    {
      FT_ZERO( table );

      table->memory      = memory;
      table->memory_user = memory->user;
      table->alloc       = memory->alloc;
      table->realloc     = memory->realloc;
      table->মুক্ত        = memory->মুক্ত;

      ft_mem_table_resize( table );

      যদি ( table->size )
      {
        ধ্রুৱ ৱর্ণ*  p;


        memory->user    = table;
        memory->alloc   = ft_mem_debug_alloc;
        memory->realloc = ft_mem_debug_realloc;
        memory->মুক্ত    = ft_mem_debug_free;

        p = ft_getenv( "FT2_ALLOC_TOTAL_MAX" );
        যদি ( p )
        {
          FT_Long  total_max = ft_strtol( p, রিক্ত, 10 );


          যদি ( total_max > 0 )
          {
            table->bound_total     = 1;
            table->alloc_total_max = total_max;
          }
        }

        p = ft_getenv( "FT2_ALLOC_COUNT_MAX" );
        যদি ( p )
        {
          FT_Long  total_গুনতি = ft_strtol( p, রিক্ত, 10 );


          যদি ( total_গুনতি > 0 )
          {
            table->bound_গুনতি     = 1;
            table->alloc_গুনতি_max = total_গুনতি;
          }
        }

        p = ft_getenv( "FT2_KEEP_ALIVE" );
        যদি ( p )
        {
          FT_Long  keep_alive = ft_strtol( p, রিক্ত, 10 );


          যদি ( keep_alive > 0 )
            table->keep_alive = 1;
        }
      }
      অন্যথা
        memory->মুক্ত( memory, table );
    }
  }


  বাহ্য শূন্য
  ft_mem_debug_done( FT_Memory  memory )
  {
    যদি ( memory->মুক্ত == ft_mem_debug_free )
    {
      FT_MemTable  table = (FT_MemTable)memory->user;


      FT_DumpMemory( memory );

      ft_mem_table_destroy( table );

      memory->মুক্ত    = table->মুক্ত;
      memory->realloc = table->realloc;
      memory->alloc   = table->alloc;
      memory->user    = table->memory_user;

      memory->মুক্ত( memory, table );
    }
  }


  FT_COMPARE_DEF( পূর্ণাঙ্ক )
  ft_mem_source_compare( ধ্রুৱ শূন্য*  p1,
                         ধ্রুৱ শূন্য*  p2 )
  {
    FT_MemSource  s1 = *(FT_MemSource*)p1;
    FT_MemSource  s2 = *(FT_MemSource*)p2;


    যদি ( s2->max_size > s1->max_size )
      ঘূৰাই 1;
    অন্যথা যদি ( s2->max_size < s1->max_size )
      ঘূৰাই -1;
    অন্যথা
      ঘূৰাই 0;
  }


  বাহ্য শূন্য
  FT_DumpMemory( FT_Memory  memory )
  {
    যদি ( memory->মুক্ত == ft_mem_debug_free )
    {
      FT_MemTable    table = (FT_MemTable)memory->user;
      FT_MemSource*  bucket = table->sources;
      FT_MemSource*  limit  = bucket + FT_MEM_SOURCE_BUCKETS;
      FT_MemSource*  sources;
      FT_Int         nn, গুনতি;
      ধ্রুৱ ৱর্ণ*    fmt;


      গুনতি = 0;
      বাবে ( ; bucket < limit; bucket++ )
      {
        FT_MemSource  source = *bucket;


        বাবে ( ; source; source = source->link )
          গুনতি++;
      }

      sources = (FT_MemSource*)
                  ft_mem_table_alloc(
                    table, গুনতি * (FT_Long)পরিমাণ ( *sources ) );

      গুনতি = 0;
      বাবে ( bucket = table->sources; bucket < limit; bucket++ )
      {
        FT_MemSource  source = *bucket;


        বাবে ( ; source; source = source->link )
          sources[গুনতি++] = source;
      }

      ft_qsort( sources,
                (size_t)গুনতি,
                পরিমাণ ( *sources ),
                ft_mem_source_compare );

      printf( "FreeType Memory Dump: "
              "current=%ld max=%ld total=%ld count=%ld\n",
              table->alloc_current, table->alloc_max,
              table->alloc_total, table->alloc_গুনতি );
      printf( " block  block    sizes    sizes    sizes   source\n" );
      printf( " count   high      sum  highsum      max   location\n" );
      printf( "-------------------------------------------------\n" );

      fmt = "%6ld %6ld %8ld %8ld %8ld %s:%d\n";

      বাবে ( nn = 0; nn < গুনতি; nn++ )
      {
        FT_MemSource  source = sources[nn];


        printf( fmt,
                source->cur_blocks, source->max_blocks,
                source->cur_size, source->max_size, source->cur_max,
                FT_FILENAME( source->file_name ),
                source->সাৰি_no );
      }
      printf( "------------------------------------------------\n" );

      ft_mem_table_free( table, sources );
    }
  }

#অন্যথা  /* !FT_DEBUG_MEMORY */

  /* ANSI C doesn't like empty source files */
  প্রকারনির্দেশ পূর্ণাঙ্ক  debug_mem_dummy_;

#endif /* !FT_DEBUG_MEMORY */


/* END */
