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


#ଅନ୍ତର୍ଭାଵ <ft2build.h>
#ଅନ୍ତର୍ଭାଵ FT_CONFIG_CONFIG_H
#ଅନ୍ତର୍ଭାଵ <freetype/internal/ftdebug.h>
#ଅନ୍ତର୍ଭାଵ <freetype/internal/ftmemory.h>
#ଅନ୍ତର୍ଭାଵ <freetype/ftsystem.h>
#ଅନ୍ତର୍ଭାଵ <freetype/fterrors.h>
#ଅନ୍ତର୍ଭାଵ <freetype/fttypes.h>


#ifdef FT_DEBUG_MEMORY

#define  KEEPALIVE /* `Keep alive' means that freed blocks aren't released
                    * to the heap.  This is useful to detect double-frees
                    * or weird heap corruption, but it uses large amounts of
                    * memory, however.
                    */

#ଅନ୍ତର୍ଭାଵ FT_CONFIG_STANDARD_LIBRARY_H

  FT_BASE_DEF( ଅଚଳ ଅକ୍ଷର* )  ft_debug_file_   = ଖାଲି;
  FT_BASE_DEF( long )         ft_debug_lineno_ = 0;

  ବାହ୍ୟ ଶୂନ୍ୟ
  FT_DumpMemory( FT_Memory  memory );


  ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା FT_MemSourceRec_*  FT_MemSource;
  ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା FT_Mem_ଗଣ୍ଠି_Rec*    FT_Mem_ଗଣ୍ଠି;
  ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା FT_MemTableRec_*   FT_MemTable;


#define FT_MEM_VAL( addr )  ( (FT_PtrDist)(FT_Pointer)( addr ) )

  /*
   * This structure holds statistics for a single allocation/release
   * site.  This is useful to know where memory operations happen the
   * most.
   */
  ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା  FT_MemSourceRec_
  {
    ଅଚଳ ଅକ୍ଷର*   file_name;
    long          ଧାଡ଼ି_no;

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
  ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା  FT_Mem_ଗଣ୍ଠି_Rec
  {
    FT_Byte*      address;
    FT_Long       size;     /* < 0 if the block was freed */

    FT_MemSource  source;

#ifdef KEEPALIVE
    ଅଚଳ ଅକ୍ଷର*   free_file_name;
    FT_Long       free_ଧାଡ଼ି_no;
#endif

    FT_Mem_ଗଣ୍ଠି    link;

  } FT_Mem_ଗଣ୍ଠି_Rec_2;


  /*
   * The global structure, containing compound statistics and all hash
   * tables.
   */
  ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା  FT_MemTableRec_
  {
    FT_Long          size;
    FT_Long          nodes;
    FT_Mem_ଗଣ୍ଠି*      buckets;

    FT_Long          alloc_total;
    FT_Long          alloc_current;
    FT_Long          alloc_max;
    FT_Long          alloc_ଗଣନା;

    FT_Bool          bound_total;
    FT_Long          alloc_total_max;

    FT_Bool          bound_ଗଣନା;
    FT_Long          alloc_ଗଣନା_max;

    FT_MemSource     sources[FT_MEM_SOURCE_BUCKETS];

    FT_Bool          keep_alive;

    FT_Memory        memory;
    FT_Pointer       memory_user;
    FT_Alloc_Func    alloc;
    FT_Free_Func     ମୁକ୍ତ;
    FT_Realloc_Func  realloc;

  } FT_MemTableRec;


#define FT_MEM_SIZE_MAX  13845163

#define FT_FILENAME( x )  ( (x) ? (x) : "unknown file" )


  /*
   * Prime numbers are ugly to handle.  It would be better to implement
   * L-Hashing, which is 10% faster and doesn't require divisions.
   */
  ସ୍ଥିର ଅଚଳ FT_Int  ft_mem_primes[] =
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


  ସ୍ଥିର FT_Long
  ft_mem_closest_ମୌଳିକ( FT_Long  ଅଙ୍କ )
  {
    size_t  i;


    ପାଇଁ ( i = 0;
          i < ଆକାର ( ft_mem_primes ) / ଆକାର ( ft_mem_primes[0] ); i++ )
      ଯଦି ( ft_mem_primes[i] > ଅଙ୍କ )
        ଫେରନ୍ତୁ ft_mem_primes[i];

    ଫେରନ୍ତୁ FT_MEM_SIZE_MAX;
  }


  ସ୍ଥିର ଶୂନ୍ୟ
  ft_mem_debug_panic( ଅଚଳ ଅକ୍ଷର*  fmt,
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


  ସ୍ଥିର FT_Pointer
  ft_mem_table_alloc( FT_MemTable  table,
                      FT_Long      size )
  {
    FT_Memory   memory = table->memory;
    FT_Pointer  block;


    memory->user = table->memory_user;
    block = table->alloc( memory, size );
    memory->user = table;

    ଫେରନ୍ତୁ block;
  }


  ସ୍ଥିର ଶୂନ୍ୟ
  ft_mem_table_free( FT_MemTable  table,
                     FT_Pointer   block )
  {
    FT_Memory  memory = table->memory;


    memory->user = table->memory_user;
    table->ମୁକ୍ତ( memory, block );
    memory->user = table;
  }


  ସ୍ଥିର ଶୂନ୍ୟ
  ft_mem_table_resize( FT_MemTable  table )
  {
    FT_Long  new_size;


    new_size = ft_mem_closest_ମୌଳିକ( table->nodes );
    ଯଦି ( new_size != table->size )
    {
      FT_Mem_ଗଣ୍ଠି*  new_buckets;
      FT_Long      i;


      new_buckets = (FT_Mem_ଗଣ୍ଠି *)
                      ft_mem_table_alloc(
                        table,
                        new_size * (FT_Long)ଆକାର ( FT_Mem_ଗଣ୍ଠି ) );
      ଯଦି ( !new_buckets )
        ଫେରନ୍ତୁ;

      FT_ARRAY_ZERO( new_buckets, new_size );

      ପାଇଁ ( i = 0; i < table->size; i++ )
      {
        FT_Mem_ଗଣ୍ଠି  ଗଣ୍ଠି, next, *pnode;
        FT_PtrDist  hash;


        ଗଣ୍ଠି = table->buckets[i];
        ଯେତେବେଳେ ( ଗଣ୍ଠି )
        {
          next  = ଗଣ୍ଠି->link;
          hash  = FT_MEM_VAL( ଗଣ୍ଠି->address ) % (FT_PtrDist)new_size;
          pnode = new_buckets + hash;

          ଗଣ୍ଠି->link = pnode[0];
          pnode[0]   = ଗଣ୍ଠି;

          ଗଣ୍ଠି = next;
        }
      }

      ଯଦି ( table->buckets )
        ft_mem_table_free( table, table->buckets );

      table->buckets = new_buckets;
      table->size    = new_size;
    }
  }


  ସ୍ଥିର ଶୂନ୍ୟ
  ft_mem_table_destroy( FT_MemTable  table )
  {
    FT_Long  i;
    FT_Long  leak_ଗଣନା = 0;
    FT_Long  leaks      = 0;


    /* remove all blocks from the table, revealing leaked ones */
    ପାଇଁ ( i = 0; i < table->size; i++ )
    {
      FT_Mem_ଗଣ୍ଠି  *pnode = table->buckets + i, next, ଗଣ୍ଠି = *pnode;


      ଯେତେବେଳେ ( ଗଣ୍ଠି )
      {
        next       = ଗଣ୍ଠି->link;
        ଗଣ୍ଠି->link = ଖାଲି;

        ଯଦି ( ଗଣ୍ଠି->size > 0 )
        {
          printf(
            "leaked memory block at address %p, size %8ld in (%s:%ld)\n",
            (ଶୂନ୍ୟ*)ଗଣ୍ଠି->address,
            ଗଣ୍ଠି->size,
            FT_FILENAME( ଗଣ୍ଠି->source->file_name ),
            ଗଣ୍ଠି->source->ଧାଡ଼ି_no );

          leak_ଗଣନା++;
          leaks += ଗଣ୍ଠି->size;

          ft_mem_table_free( table, ଗଣ୍ଠି->address );
        }

        ଗଣ୍ଠି->address = ଖାଲି;
        ଗଣ୍ଠି->size    = 0;

        ft_mem_table_free( table, ଗଣ୍ଠି );
        ଗଣ୍ଠି = next;
      }
      table->buckets[i] = ଖାଲି;
    }

    ft_mem_table_free( table, table->buckets );
    table->buckets = ଖାଲି;

    table->size  = 0;
    table->nodes = 0;

    /* remove all sources */
    ପାଇଁ ( i = 0; i < FT_MEM_SOURCE_BUCKETS; i++ )
    {
      FT_MemSource  source, next;


      ପାଇଁ ( source = table->sources[i]; source != ଖାଲି; source = next )
      {
        next = source->link;
        ft_mem_table_free( table, source );
      }

      table->sources[i] = ଖାଲି;
    }

    printf( "FreeType: total memory allocations = %ld\n",
            table->alloc_total );
    printf( "FreeType: maximum memory footprint = %ld\n",
            table->alloc_max );

    ଯଦି ( leak_ଗଣନା > 0 )
      ft_mem_debug_panic(
        "FreeType: %ld bytes of memory leaked in %ld blocks\n",
        leaks, leak_ଗଣନା );

    printf( "FreeType: no memory leaks detected\n" );
  }


  ସ୍ଥିର FT_Mem_ଗଣ୍ଠି*
  ft_mem_table_get_nodep( FT_MemTable  table,
                          FT_Byte*     address )
  {
    FT_PtrDist   hash;
    FT_Mem_ଗଣ୍ଠି  *pnode, ଗଣ୍ଠି;


    hash  = FT_MEM_VAL( address );
    pnode = table->buckets + ( hash % (FT_PtrDist)table->size );

    ପାଇଁ (;;)
    {
      ଗଣ୍ଠି = pnode[0];
      ଯଦି ( !ଗଣ୍ଠି )
        ଭାଙ୍ଗ;

      ଯଦି ( ଗଣ୍ଠି->address == address )
        ଭାଙ୍ଗ;

      pnode = &ଗଣ୍ଠି->link;
    }
    ଫେରନ୍ତୁ pnode;
  }


  ସ୍ଥିର FT_MemSource
  ft_mem_table_get_source( FT_MemTable  table )
  {
    FT_UInt32     hash;
    FT_MemSource  ଗଣ୍ଠି, *pnode;


    /* cast to FT_PtrDist first since void* can be larger */
    /* than FT_UInt32 and GCC 4.1.1 emits a warning       */
    hash  = (FT_UInt32)(FT_PtrDist)(ଶୂନ୍ୟ*)ft_debug_file_ +
              (FT_UInt32)( 5 * ft_debug_lineno_ );
    pnode = &table->sources[hash % FT_MEM_SOURCE_BUCKETS];

    ପାଇଁ (;;)
    {
      ଗଣ୍ଠି = *pnode;
      ଯଦି ( !ଗଣ୍ଠି )
        ଭାଙ୍ଗ;

      ଯଦି ( ଗଣ୍ଠି->file_name == ft_debug_file_   &&
           ଗଣ୍ଠି->ଧାଡ଼ି_no   == ft_debug_lineno_ )
        ଯାଅ Exit;

      pnode = &ଗଣ୍ଠି->link;
    }

    ଗଣ୍ଠି = (FT_MemSource)ft_mem_table_alloc( table, ଆକାର ( *ଗଣ୍ଠି ) );
    ଯଦି ( !ଗଣ୍ଠି )
      ft_mem_debug_panic(
        "not enough memory to perform memory debugging\n" );

    ଗଣ୍ଠି->file_name = ft_debug_file_;
    ଗଣ୍ଠି->ଧାଡ଼ି_no   = ft_debug_lineno_;

    ଗଣ୍ଠି->cur_blocks = 0;
    ଗଣ୍ଠି->max_blocks = 0;
    ଗଣ୍ଠି->all_blocks = 0;

    ଗଣ୍ଠି->cur_size = 0;
    ଗଣ୍ଠି->max_size = 0;
    ଗଣ୍ଠି->all_size = 0;

    ଗଣ୍ଠି->cur_max = 0;

    ଗଣ୍ଠି->link = ଖାଲି;
    ଗଣ୍ଠି->hash = hash;
    *pnode     = ଗଣ୍ଠି;

  Exit:
    ଫେରନ୍ତୁ ଗଣ୍ଠି;
  }


  ସ୍ଥିର ଶୂନ୍ୟ
  ft_mem_table_set( FT_MemTable  table,
                    FT_Byte*     address,
                    FT_Long      size,
                    FT_Long      delta )
  {
    FT_Mem_ଗଣ୍ଠି  *pnode, ଗଣ୍ଠି;


    ଯଦି ( table )
    {
      FT_MemSource  source;


      pnode = ft_mem_table_get_nodep( table, address );
      ଗଣ୍ଠି  = *pnode;
      ଯଦି ( ଗଣ୍ଠି )
      {
        ଯଦି ( ଗଣ୍ଠି->size < 0 )
        {
          /* This block was already freed.  Our memory is now completely */
          /* corrupted!                                                  */
          /* This can only happen in keep-alive mode.                    */
          ft_mem_debug_panic(
            "memory heap corrupted (allocating freed block)" );
        }
        ଅନ୍ୟଥା
        {
          /* This block was already allocated.  This means that our memory */
          /* is also corrupted!                                            */
          ft_mem_debug_panic(
            "memory heap corrupted (re-allocating allocated block at"
            " %p, of size %ld)\n"
            "org=%s:%d new=%s:%d\n",
            ଗଣ୍ଠି->address, ଗଣ୍ଠି->size,
            FT_FILENAME( ଗଣ୍ଠି->source->file_name ), ଗଣ୍ଠି->source->ଧାଡ଼ି_no,
            FT_FILENAME( ft_debug_file_ ), ft_debug_lineno_ );
        }
      }

      /* we need to create a new node in this table */
      ଗଣ୍ଠି = (FT_Mem_ଗଣ୍ଠି)ft_mem_table_alloc( table, ଆକାର ( *ଗଣ୍ଠି ) );
      ଯଦି ( !ଗଣ୍ଠି )
        ft_mem_debug_panic( "not enough memory to run memory tests" );

      ଗଣ୍ଠି->address = address;
      ଗଣ୍ଠି->size    = size;
      ଗଣ୍ଠି->source  = source = ft_mem_table_get_source( table );

      ଯଦି ( delta == 0 )
      {
        /* this is an allocation */
        source->all_blocks++;
        source->cur_blocks++;
        ଯଦି ( source->cur_blocks > source->max_blocks )
          source->max_blocks = source->cur_blocks;
      }

      ଯଦି ( size > source->cur_max )
        source->cur_max = size;

      ଯଦି ( delta != 0 )
      {
        /* we are growing or shrinking a reallocated block */
        source->cur_size     += delta;
        table->alloc_current += delta;
      }
      ଅନ୍ୟଥା
      {
        /* we are allocating a new block */
        source->cur_size     += size;
        table->alloc_current += size;
      }

      source->all_size += size;

      ଯଦି ( source->cur_size > source->max_size )
        source->max_size = source->cur_size;

      ଗଣ୍ଠି->free_file_name = ଖାଲି;
      ଗଣ୍ଠି->free_ଧାଡ଼ି_no   = 0;

      ଗଣ୍ଠି->link = pnode[0];

      pnode[0] = ଗଣ୍ଠି;
      table->nodes++;

      table->alloc_total += size;

      ଯଦି ( table->alloc_current > table->alloc_max )
        table->alloc_max = table->alloc_current;

      ଯଦି ( table->nodes * 3 < table->size  ||
           table->size  * 3 < table->nodes )
        ft_mem_table_resize( table );
    }
  }


  ସ୍ଥିର ଶୂନ୍ୟ
  ft_mem_table_remove( FT_MemTable  table,
                       FT_Byte*     address,
                       FT_Long      delta )
  {
    ଯଦି ( table )
    {
      FT_Mem_ଗଣ୍ଠି  *pnode, ଗଣ୍ଠି;


      pnode = ft_mem_table_get_nodep( table, address );
      ଗଣ୍ଠି  = *pnode;
      ଯଦି ( ଗଣ୍ଠି )
      {
        FT_MemSource  source;


        ଯଦି ( ଗଣ୍ଠି->size < 0 )
          ft_mem_debug_panic(
            "freeing memory block at %p more than once\n"
            "  at (%s:%ld)!\n"
            "  Block was allocated at (%s:%ld)\n"
            "  and released at (%s:%ld).",
            address,
            FT_FILENAME( ft_debug_file_ ), ft_debug_lineno_,
            FT_FILENAME( ଗଣ୍ଠି->source->file_name ), ଗଣ୍ଠି->source->ଧାଡ଼ି_no,
            FT_FILENAME( ଗଣ୍ଠି->free_file_name ), ଗଣ୍ଠି->free_ଧାଡ଼ି_no );

        /* scramble the node's content for additional safety */
        FT_MEM_SET( address, 0xF3, ଗଣ୍ଠି->size );

        ଯଦି ( delta == 0 )
        {
          source = ଗଣ୍ଠି->source;

          source->cur_blocks--;
          source->cur_size -= ଗଣ୍ଠି->size;

          table->alloc_current -= ଗଣ୍ଠି->size;
        }

        ଯଦି ( table->keep_alive )
        {
          /* we simply invert the node's size to indicate that the node */
          /* was freed.                                                 */
          ଗଣ୍ଠି->size           = -ଗଣ୍ଠି->size;
          ଗଣ୍ଠି->free_file_name = ft_debug_file_;
          ଗଣ୍ଠି->free_ଧାଡ଼ି_no   = ft_debug_lineno_;
        }
        ଅନ୍ୟଥା
        {
          table->nodes--;

          *pnode = ଗଣ୍ଠି->link;

          ଗଣ୍ଠି->size   = 0;
          ଗଣ୍ଠି->source = ଖାଲି;

          ft_mem_table_free( table, ଗଣ୍ଠି );

          ଯଦି ( table->nodes * 3 < table->size  ||
               table->size  * 3 < table->nodes )
            ft_mem_table_resize( table );
        }
      }
      ଅନ୍ୟଥା
        ft_mem_debug_panic(
          "trying to free unknown block at %p in (%s:%ld)\n",
          address,
          FT_FILENAME( ft_debug_file_ ), ft_debug_lineno_ );
    }
  }


  ସ୍ଥିର FT_Pointer
  ft_mem_debug_alloc( FT_Memory  memory,
                      FT_Long    size )
  {
    FT_MemTable  table = (FT_MemTable)memory->user;
    FT_Byte*     block;


    ଯଦି ( size <= 0 )
      ft_mem_debug_panic( "negative block size allocation (%ld)", size );

    /* return NULL if the maximum number of allocations was reached */
    ଯଦି ( table->bound_ଗଣନା                           &&
         table->alloc_ଗଣନା >= table->alloc_ଗଣନା_max )
      ଫେରନ୍ତୁ ଖାଲି;

    /* return NULL if this allocation would overflow the maximum heap size */
    ଯଦି ( table->bound_total                                   &&
         table->alloc_total_max - table->alloc_current > size )
      ଫେରନ୍ତୁ ଖାଲି;

    block = (FT_Byte *)ft_mem_table_alloc( table, size );
    ଯଦି ( block )
    {
      ft_mem_table_set( table, block, size, 0 );

      table->alloc_ଗଣନା++;
    }

    ft_debug_file_   = "<unknown>";
    ft_debug_lineno_ = 0;

    ଫେରନ୍ତୁ (FT_Pointer)block;
  }


  ସ୍ଥିର ଶୂନ୍ୟ
  ft_mem_debug_free( FT_Memory   memory,
                     FT_Pointer  block )
  {
    FT_MemTable  table = (FT_MemTable)memory->user;


    ଯଦି ( !block )
      ft_mem_debug_panic( "trying to free NULL in (%s:%ld)",
                          FT_FILENAME( ft_debug_file_ ),
                          ft_debug_lineno_ );

    ft_mem_table_remove( table, (FT_Byte*)block, 0 );

    ଯଦି ( !table->keep_alive )
      ft_mem_table_free( table, block );

    table->alloc_ଗଣନା--;

    ft_debug_file_   = "<unknown>";
    ft_debug_lineno_ = 0;
  }


  ସ୍ଥିର FT_Pointer
  ft_mem_debug_realloc( FT_Memory   memory,
                        FT_Long     cur_size,
                        FT_Long     new_size,
                        FT_Pointer  block )
  {
    FT_MemTable  table = (FT_MemTable)memory->user;
    FT_Mem_ଗଣ୍ଠି   ଗଣ୍ଠି, *pnode;
    FT_Pointer   new_block;
    FT_Long      delta;

    ଅଚଳ ଅକ୍ଷର*  file_name = FT_FILENAME( ft_debug_file_ );
    FT_Long      ଧାଡ଼ି_no   = ft_debug_lineno_;


    /* unlikely, but possible */
    ଯଦି ( new_size == cur_size )
      ଫେରନ୍ତୁ block;

    /* the following is valid according to ANSI C */
#ଯଦି 0
    ଯଦି ( !block || !cur_size )
      ft_mem_debug_panic( "trying to reallocate NULL in (%s:%ld)",
                          file_name, ଧାଡ଼ି_no );
#endif

    /* while the following is allowed in ANSI C also, we abort since */
    /* such case should be handled by FreeType.                      */
    ଯଦି ( new_size <= 0 )
      ft_mem_debug_panic(
        "trying to reallocate %p to size 0 (current is %ld) in (%s:%ld)",
        block, cur_size, file_name, ଧାଡ଼ି_no );

    /* check `cur_size' value */
    pnode = ft_mem_table_get_nodep( table, (FT_Byte*)block );
    ଗଣ୍ଠି  = *pnode;
    ଯଦି ( !ଗଣ୍ଠି )
      ft_mem_debug_panic(
        "trying to reallocate unknown block at %p in (%s:%ld)",
        block, file_name, ଧାଡ଼ି_no );

    ଯଦି ( ଗଣ୍ଠି->size <= 0 )
      ft_mem_debug_panic(
        "trying to reallocate freed block at %p in (%s:%ld)",
        block, file_name, ଧାଡ଼ି_no );

    ଯଦି ( ଗଣ୍ଠି->size != cur_size )
      ft_mem_debug_panic( "invalid ft_realloc request for %p. cur_size is "
                          "%ld instead of %ld in (%s:%ld)",
                          block, cur_size, ଗଣ୍ଠି->size, file_name, ଧାଡ଼ି_no );

    /* return NULL if the maximum number of allocations was reached */
    ଯଦି ( table->bound_ଗଣନା                           &&
         table->alloc_ଗଣନା >= table->alloc_ଗଣନା_max )
      ଫେରନ୍ତୁ ଖାଲି;

    delta = new_size - cur_size;

    /* return NULL if this allocation would overflow the maximum heap size */
    ଯଦି ( delta > 0                                             &&
         table->bound_total                                    &&
         table->alloc_current + delta > table->alloc_total_max )
      ଫେରନ୍ତୁ ଖାଲି;

    new_block = (FT_Pointer)ft_mem_table_alloc( table, new_size );
    ଯଦି ( !new_block )
      ଫେରନ୍ତୁ ଖାଲି;

    ft_mem_table_set( table, (FT_Byte*)new_block, new_size, delta );

    ft_memcpy( new_block, block, cur_size < new_size ? (size_t)cur_size
                                                     : (size_t)new_size );

    ft_mem_table_remove( table, (FT_Byte*)block, delta );

    ft_debug_file_   = "<unknown>";
    ft_debug_lineno_ = 0;

    ଯଦି ( !table->keep_alive )
      ft_mem_table_free( table, block );

    ଫେରନ୍ତୁ new_block;
  }


  ବାହ୍ୟ ଶୂନ୍ୟ
  ft_mem_debug_init( FT_Memory  memory )
  {
    FT_MemTable  table;


    ଯଦି ( !ft_getenv( "FT2_DEBUG_MEMORY" ) )
      ଫେରନ୍ତୁ;

    table = (FT_MemTable)memory->alloc( memory, ଆକାର ( *table ) );

    ଯଦି ( table )
    {
      FT_ZERO( table );

      table->memory      = memory;
      table->memory_user = memory->user;
      table->alloc       = memory->alloc;
      table->realloc     = memory->realloc;
      table->ମୁକ୍ତ        = memory->ମୁକ୍ତ;

      ft_mem_table_resize( table );

      ଯଦି ( table->size )
      {
        ଅଚଳ ଅକ୍ଷର*  p;


        memory->user    = table;
        memory->alloc   = ft_mem_debug_alloc;
        memory->realloc = ft_mem_debug_realloc;
        memory->ମୁକ୍ତ    = ft_mem_debug_free;

        p = ft_getenv( "FT2_ALLOC_TOTAL_MAX" );
        ଯଦି ( p )
        {
          FT_Long  total_max = ft_strtol( p, ଖାଲି, 10 );


          ଯଦି ( total_max > 0 )
          {
            table->bound_total     = 1;
            table->alloc_total_max = total_max;
          }
        }

        p = ft_getenv( "FT2_ALLOC_COUNT_MAX" );
        ଯଦି ( p )
        {
          FT_Long  total_ଗଣନା = ft_strtol( p, ଖାଲି, 10 );


          ଯଦି ( total_ଗଣନା > 0 )
          {
            table->bound_ଗଣନା     = 1;
            table->alloc_ଗଣନା_max = total_ଗଣନା;
          }
        }

        p = ft_getenv( "FT2_KEEP_ALIVE" );
        ଯଦି ( p )
        {
          FT_Long  keep_alive = ft_strtol( p, ଖାଲି, 10 );


          ଯଦି ( keep_alive > 0 )
            table->keep_alive = 1;
        }
      }
      ଅନ୍ୟଥା
        memory->ମୁକ୍ତ( memory, table );
    }
  }


  ବାହ୍ୟ ଶୂନ୍ୟ
  ft_mem_debug_done( FT_Memory  memory )
  {
    ଯଦି ( memory->ମୁକ୍ତ == ft_mem_debug_free )
    {
      FT_MemTable  table = (FT_MemTable)memory->user;


      FT_DumpMemory( memory );

      ft_mem_table_destroy( table );

      memory->ମୁକ୍ତ    = table->ମୁକ୍ତ;
      memory->realloc = table->realloc;
      memory->alloc   = table->alloc;
      memory->user    = table->memory_user;

      memory->ମୁକ୍ତ( memory, table );
    }
  }


  FT_COMPARE_DEF( ପୂର୍ଣ୍ଣସଂଖ୍ୟା )
  ft_mem_source_compare( ଅଚଳ ଶୂନ୍ୟ*  p1,
                         ଅଚଳ ଶୂନ୍ୟ*  p2 )
  {
    FT_MemSource  s1 = *(FT_MemSource*)p1;
    FT_MemSource  s2 = *(FT_MemSource*)p2;


    ଯଦି ( s2->max_size > s1->max_size )
      ଫେରନ୍ତୁ 1;
    ଅନ୍ୟଥା ଯଦି ( s2->max_size < s1->max_size )
      ଫେରନ୍ତୁ -1;
    ଅନ୍ୟଥା
      ଫେରନ୍ତୁ 0;
  }


  ବାହ୍ୟ ଶୂନ୍ୟ
  FT_DumpMemory( FT_Memory  memory )
  {
    ଯଦି ( memory->ମୁକ୍ତ == ft_mem_debug_free )
    {
      FT_MemTable    table = (FT_MemTable)memory->user;
      FT_MemSource*  bucket = table->sources;
      FT_MemSource*  limit  = bucket + FT_MEM_SOURCE_BUCKETS;
      FT_MemSource*  sources;
      FT_Int         nn, ଗଣନା_2;
      ଅଚଳ ଅକ୍ଷର*    fmt;


      ଗଣନା_2 = 0;
      ପାଇଁ ( ; bucket < limit; bucket++ )
      {
        FT_MemSource  source = *bucket;


        ପାଇଁ ( ; source; source = source->link )
          ଗଣନା_2++;
      }

      sources = (FT_MemSource*)
                  ft_mem_table_alloc(
                    table, ଗଣନା_2 * (FT_Long)ଆକାର ( *sources ) );

      ଗଣନା_2 = 0;
      ପାଇଁ ( bucket = table->sources; bucket < limit; bucket++ )
      {
        FT_MemSource  source = *bucket;


        ପାଇଁ ( ; source; source = source->link )
          sources[ଗଣନା_2++] = source;
      }

      ft_qsort( sources,
                (size_t)ଗଣନା_2,
                ଆକାର ( *sources ),
                ft_mem_source_compare );

      printf( "FreeType Memory Dump: "
              "current=%ld max=%ld total=%ld count=%ld\n",
              table->alloc_current, table->alloc_max,
              table->alloc_total, table->alloc_ଗଣନା );
      printf( " block  block    sizes    sizes    sizes   source\n" );
      printf( " count   high      sum  highsum      max   location\n" );
      printf( "-------------------------------------------------\n" );

      fmt = "%6ld %6ld %8ld %8ld %8ld %s:%d\n";

      ପାଇଁ ( nn = 0; nn < ଗଣନା_2; nn++ )
      {
        FT_MemSource  source = sources[nn];


        printf( fmt,
                source->cur_blocks, source->max_blocks,
                source->cur_size, source->max_size, source->cur_max,
                FT_FILENAME( source->file_name ),
                source->ଧାଡ଼ି_no );
      }
      printf( "------------------------------------------------\n" );

      ft_mem_table_free( table, sources );
    }
  }

#ଅନ୍ୟଥା  /* !FT_DEBUG_MEMORY */

  /* ANSI C doesn't like empty source files */
  ପ୍ରକାରବ୍ୟାଖ୍ୟା ପୂର୍ଣ୍ଣସଂଖ୍ୟା  debug_mem_dummy_;

#endif /* !FT_DEBUG_MEMORY */


/* END */
