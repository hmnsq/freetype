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


#അന്തര്ഭാവ <ft2build.h>
#അന്തര്ഭാവ FT_CONFIG_CONFIG_H
#അന്തര്ഭാവ <freetype/internal/ftdebug.h>
#അന്തര്ഭാവ <freetype/internal/ftmemory.h>
#അന്തര്ഭാവ <freetype/ftsystem.h>
#അന്തര്ഭാവ <freetype/fterrors.h>
#അന്തര്ഭാവ <freetype/fttypes.h>


#ifdef FT_DEBUG_MEMORY

#define  KEEPALIVE /* `Keep alive' means that freed blocks aren't released
                    * to the heap.  This is useful to detect double-frees
                    * or weird heap corruption, but it uses large amounts of
                    * memory, however.
                    */

#അന്തര്ഭാവ FT_CONFIG_STANDARD_LIBRARY_H

  FT_BASE_DEF( സ്ഥിരം അക്ഷരം* )  ft_debug_file_   = ശൂന്യ;
  FT_BASE_DEF( long )         ft_debug_lineno_ = 0;

  ബാഹ്യ ശൂന്യം
  FT_DumpMemory( FT_Memory  memory );


  തരനിർവചനം ഘടന FT_MemSourceRec_*  FT_MemSource;
  തരനിർവചനം ഘടന FT_Mem_കണ്ണി_Rec*    FT_Mem_കണ്ണി;
  തരനിർവചനം ഘടന FT_MemTableRec_*   FT_MemTable;


#define FT_MEM_VAL( addr )  ( (FT_PtrDist)(FT_Pointer)( addr ) )

  /*
   * This structure holds statistics for a single allocation/release
   * site.  This is useful to know where memory operations happen the
   * most.
   */
  തരനിർവചനം ഘടന  FT_MemSourceRec_
  {
    സ്ഥിരം അക്ഷരം*   file_name;
    long          വരി_no;

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
  തരനിർവചനം ഘടന  FT_Mem_കണ്ണി_Rec
  {
    FT_Byte*      address;
    FT_Long       size;     /* < 0 if the block was freed */

    FT_MemSource  source;

#ifdef KEEPALIVE
    സ്ഥിരം അക്ഷരം*   free_file_name;
    FT_Long       free_വരി_no;
#endif

    FT_Mem_കണ്ണി    link;

  } FT_Mem_കണ്ണി_Rec_2;


  /*
   * The global structure, containing compound statistics and all hash
   * tables.
   */
  തരനിർവചനം ഘടന  FT_MemTableRec_
  {
    FT_Long          size;
    FT_Long          nodes;
    FT_Mem_കണ്ണി*      buckets;

    FT_Long          alloc_total;
    FT_Long          alloc_current;
    FT_Long          alloc_max;
    FT_Long          alloc_എണ്ണം;

    FT_Bool          bound_total;
    FT_Long          alloc_total_max;

    FT_Bool          bound_എണ്ണം;
    FT_Long          alloc_എണ്ണം_max;

    FT_MemSource     sources[FT_MEM_SOURCE_BUCKETS];

    FT_Bool          keep_alive;

    FT_Memory        memory;
    FT_Pointer       memory_user;
    FT_Alloc_Func    alloc;
    FT_Free_Func     സ്വതന്ത്രം;
    FT_Realloc_Func  realloc;

  } FT_MemTableRec;


#define FT_MEM_SIZE_MAX  13845163

#define FT_FILENAME( x )  ( (x) ? (x) : "unknown file" )


  /*
   * Prime numbers are ugly to handle.  It would be better to implement
   * L-Hashing, which is 10% faster and doesn't require divisions.
   */
  സ്ഥിതം സ്ഥിരം FT_Int  ft_mem_primes[] =
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


  സ്ഥിതം FT_Long
  ft_mem_closest_അഭാജ്യം( FT_Long  അക്കം )
  {
    size_t  i;


    വേണ്ടി ( i = 0;
          i < വലുപ്പം ( ft_mem_primes ) / വലുപ്പം ( ft_mem_primes[0] ); i++ )
      എങ്കിൽ ( ft_mem_primes[i] > അക്കം )
        തിരികെ ft_mem_primes[i];

    തിരികെ FT_MEM_SIZE_MAX;
  }


  സ്ഥിതം ശൂന്യം
  ft_mem_debug_panic( സ്ഥിരം അക്ഷരം*  fmt,
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


  സ്ഥിതം FT_Pointer
  ft_mem_table_alloc( FT_MemTable  table,
                      FT_Long      size )
  {
    FT_Memory   memory = table->memory;
    FT_Pointer  block;


    memory->user = table->memory_user;
    block = table->alloc( memory, size );
    memory->user = table;

    തിരികെ block;
  }


  സ്ഥിതം ശൂന്യം
  ft_mem_table_free( FT_MemTable  table,
                     FT_Pointer   block )
  {
    FT_Memory  memory = table->memory;


    memory->user = table->memory_user;
    table->സ്വതന്ത്രം( memory, block );
    memory->user = table;
  }


  സ്ഥിതം ശൂന്യം
  ft_mem_table_resize( FT_MemTable  table )
  {
    FT_Long  new_size;


    new_size = ft_mem_closest_അഭാജ്യം( table->nodes );
    എങ്കിൽ ( new_size != table->size )
    {
      FT_Mem_കണ്ണി*  new_buckets;
      FT_Long      i;


      new_buckets = (FT_Mem_കണ്ണി *)
                      ft_mem_table_alloc(
                        table,
                        new_size * (FT_Long)വലുപ്പം ( FT_Mem_കണ്ണി ) );
      എങ്കിൽ ( !new_buckets )
        തിരികെ;

      FT_ARRAY_ZERO( new_buckets, new_size );

      വേണ്ടി ( i = 0; i < table->size; i++ )
      {
        FT_Mem_കണ്ണി  കണ്ണി, next, *pnode;
        FT_PtrDist  hash;


        കണ്ണി = table->buckets[i];
        ആകുമ്പോൾ ( കണ്ണി )
        {
          next  = കണ്ണി->link;
          hash  = FT_MEM_VAL( കണ്ണി->address ) % (FT_PtrDist)new_size;
          pnode = new_buckets + hash;

          കണ്ണി->link = pnode[0];
          pnode[0]   = കണ്ണി;

          കണ്ണി = next;
        }
      }

      എങ്കിൽ ( table->buckets )
        ft_mem_table_free( table, table->buckets );

      table->buckets = new_buckets;
      table->size    = new_size;
    }
  }


  സ്ഥിതം ശൂന്യം
  ft_mem_table_destroy( FT_MemTable  table )
  {
    FT_Long  i;
    FT_Long  leak_എണ്ണം = 0;
    FT_Long  leaks      = 0;


    /* remove all blocks from the table, revealing leaked ones */
    വേണ്ടി ( i = 0; i < table->size; i++ )
    {
      FT_Mem_കണ്ണി  *pnode = table->buckets + i, next, കണ്ണി = *pnode;


      ആകുമ്പോൾ ( കണ്ണി )
      {
        next       = കണ്ണി->link;
        കണ്ണി->link = ശൂന്യ;

        എങ്കിൽ ( കണ്ണി->size > 0 )
        {
          printf(
            "leaked memory block at address %p, size %8ld in (%s:%ld)\n",
            (ശൂന്യം*)കണ്ണി->address,
            കണ്ണി->size,
            FT_FILENAME( കണ്ണി->source->file_name ),
            കണ്ണി->source->വരി_no );

          leak_എണ്ണം++;
          leaks += കണ്ണി->size;

          ft_mem_table_free( table, കണ്ണി->address );
        }

        കണ്ണി->address = ശൂന്യ;
        കണ്ണി->size    = 0;

        ft_mem_table_free( table, കണ്ണി );
        കണ്ണി = next;
      }
      table->buckets[i] = ശൂന്യ;
    }

    ft_mem_table_free( table, table->buckets );
    table->buckets = ശൂന്യ;

    table->size  = 0;
    table->nodes = 0;

    /* remove all sources */
    വേണ്ടി ( i = 0; i < FT_MEM_SOURCE_BUCKETS; i++ )
    {
      FT_MemSource  source, next;


      വേണ്ടി ( source = table->sources[i]; source != ശൂന്യ; source = next )
      {
        next = source->link;
        ft_mem_table_free( table, source );
      }

      table->sources[i] = ശൂന്യ;
    }

    printf( "FreeType: total memory allocations = %ld\n",
            table->alloc_total );
    printf( "FreeType: maximum memory footprint = %ld\n",
            table->alloc_max );

    എങ്കിൽ ( leak_എണ്ണം > 0 )
      ft_mem_debug_panic(
        "FreeType: %ld bytes of memory leaked in %ld blocks\n",
        leaks, leak_എണ്ണം );

    printf( "FreeType: no memory leaks detected\n" );
  }


  സ്ഥിതം FT_Mem_കണ്ണി*
  ft_mem_table_get_nodep( FT_MemTable  table,
                          FT_Byte*     address )
  {
    FT_PtrDist   hash;
    FT_Mem_കണ്ണി  *pnode, കണ്ണി;


    hash  = FT_MEM_VAL( address );
    pnode = table->buckets + ( hash % (FT_PtrDist)table->size );

    വേണ്ടി (;;)
    {
      കണ്ണി = pnode[0];
      എങ്കിൽ ( !കണ്ണി )
        നിർത്തുക;

      എങ്കിൽ ( കണ്ണി->address == address )
        നിർത്തുക;

      pnode = &കണ്ണി->link;
    }
    തിരികെ pnode;
  }


  സ്ഥിതം FT_MemSource
  ft_mem_table_get_source( FT_MemTable  table )
  {
    FT_UInt32     hash;
    FT_MemSource  കണ്ണി, *pnode;


    /* cast to FT_PtrDist first since void* can be larger */
    /* than FT_UInt32 and GCC 4.1.1 emits a warning       */
    hash  = (FT_UInt32)(FT_PtrDist)(ശൂന്യം*)ft_debug_file_ +
              (FT_UInt32)( 5 * ft_debug_lineno_ );
    pnode = &table->sources[hash % FT_MEM_SOURCE_BUCKETS];

    വേണ്ടി (;;)
    {
      കണ്ണി = *pnode;
      എങ്കിൽ ( !കണ്ണി )
        നിർത്തുക;

      എങ്കിൽ ( കണ്ണി->file_name == ft_debug_file_   &&
           കണ്ണി->വരി_no   == ft_debug_lineno_ )
        പോകുക Exit;

      pnode = &കണ്ണി->link;
    }

    കണ്ണി = (FT_MemSource)ft_mem_table_alloc( table, വലുപ്പം ( *കണ്ണി ) );
    എങ്കിൽ ( !കണ്ണി )
      ft_mem_debug_panic(
        "not enough memory to perform memory debugging\n" );

    കണ്ണി->file_name = ft_debug_file_;
    കണ്ണി->വരി_no   = ft_debug_lineno_;

    കണ്ണി->cur_blocks = 0;
    കണ്ണി->max_blocks = 0;
    കണ്ണി->all_blocks = 0;

    കണ്ണി->cur_size = 0;
    കണ്ണി->max_size = 0;
    കണ്ണി->all_size = 0;

    കണ്ണി->cur_max = 0;

    കണ്ണി->link = ശൂന്യ;
    കണ്ണി->hash = hash;
    *pnode     = കണ്ണി;

  Exit:
    തിരികെ കണ്ണി;
  }


  സ്ഥിതം ശൂന്യം
  ft_mem_table_set( FT_MemTable  table,
                    FT_Byte*     address,
                    FT_Long      size,
                    FT_Long      delta )
  {
    FT_Mem_കണ്ണി  *pnode, കണ്ണി;


    എങ്കിൽ ( table )
    {
      FT_MemSource  source;


      pnode = ft_mem_table_get_nodep( table, address );
      കണ്ണി  = *pnode;
      എങ്കിൽ ( കണ്ണി )
      {
        എങ്കിൽ ( കണ്ണി->size < 0 )
        {
          /* This block was already freed.  Our memory is now completely */
          /* corrupted!                                                  */
          /* This can only happen in keep-alive mode.                    */
          ft_mem_debug_panic(
            "memory heap corrupted (allocating freed block)" );
        }
        അല്ലെങ്കിൽ
        {
          /* This block was already allocated.  This means that our memory */
          /* is also corrupted!                                            */
          ft_mem_debug_panic(
            "memory heap corrupted (re-allocating allocated block at"
            " %p, of size %ld)\n"
            "org=%s:%d new=%s:%d\n",
            കണ്ണി->address, കണ്ണി->size,
            FT_FILENAME( കണ്ണി->source->file_name ), കണ്ണി->source->വരി_no,
            FT_FILENAME( ft_debug_file_ ), ft_debug_lineno_ );
        }
      }

      /* we need to create a new node in this table */
      കണ്ണി = (FT_Mem_കണ്ണി)ft_mem_table_alloc( table, വലുപ്പം ( *കണ്ണി ) );
      എങ്കിൽ ( !കണ്ണി )
        ft_mem_debug_panic( "not enough memory to run memory tests" );

      കണ്ണി->address = address;
      കണ്ണി->size    = size;
      കണ്ണി->source  = source = ft_mem_table_get_source( table );

      എങ്കിൽ ( delta == 0 )
      {
        /* this is an allocation */
        source->all_blocks++;
        source->cur_blocks++;
        എങ്കിൽ ( source->cur_blocks > source->max_blocks )
          source->max_blocks = source->cur_blocks;
      }

      എങ്കിൽ ( size > source->cur_max )
        source->cur_max = size;

      എങ്കിൽ ( delta != 0 )
      {
        /* we are growing or shrinking a reallocated block */
        source->cur_size     += delta;
        table->alloc_current += delta;
      }
      അല്ലെങ്കിൽ
      {
        /* we are allocating a new block */
        source->cur_size     += size;
        table->alloc_current += size;
      }

      source->all_size += size;

      എങ്കിൽ ( source->cur_size > source->max_size )
        source->max_size = source->cur_size;

      കണ്ണി->free_file_name = ശൂന്യ;
      കണ്ണി->free_വരി_no   = 0;

      കണ്ണി->link = pnode[0];

      pnode[0] = കണ്ണി;
      table->nodes++;

      table->alloc_total += size;

      എങ്കിൽ ( table->alloc_current > table->alloc_max )
        table->alloc_max = table->alloc_current;

      എങ്കിൽ ( table->nodes * 3 < table->size  ||
           table->size  * 3 < table->nodes )
        ft_mem_table_resize( table );
    }
  }


  സ്ഥിതം ശൂന്യം
  ft_mem_table_remove( FT_MemTable  table,
                       FT_Byte*     address,
                       FT_Long      delta )
  {
    എങ്കിൽ ( table )
    {
      FT_Mem_കണ്ണി  *pnode, കണ്ണി;


      pnode = ft_mem_table_get_nodep( table, address );
      കണ്ണി  = *pnode;
      എങ്കിൽ ( കണ്ണി )
      {
        FT_MemSource  source;


        എങ്കിൽ ( കണ്ണി->size < 0 )
          ft_mem_debug_panic(
            "freeing memory block at %p more than once\n"
            "  at (%s:%ld)!\n"
            "  Block was allocated at (%s:%ld)\n"
            "  and released at (%s:%ld).",
            address,
            FT_FILENAME( ft_debug_file_ ), ft_debug_lineno_,
            FT_FILENAME( കണ്ണി->source->file_name ), കണ്ണി->source->വരി_no,
            FT_FILENAME( കണ്ണി->free_file_name ), കണ്ണി->free_വരി_no );

        /* scramble the node's content for additional safety */
        FT_MEM_SET( address, 0xF3, കണ്ണി->size );

        എങ്കിൽ ( delta == 0 )
        {
          source = കണ്ണി->source;

          source->cur_blocks--;
          source->cur_size -= കണ്ണി->size;

          table->alloc_current -= കണ്ണി->size;
        }

        എങ്കിൽ ( table->keep_alive )
        {
          /* we simply invert the node's size to indicate that the node */
          /* was freed.                                                 */
          കണ്ണി->size           = -കണ്ണി->size;
          കണ്ണി->free_file_name = ft_debug_file_;
          കണ്ണി->free_വരി_no   = ft_debug_lineno_;
        }
        അല്ലെങ്കിൽ
        {
          table->nodes--;

          *pnode = കണ്ണി->link;

          കണ്ണി->size   = 0;
          കണ്ണി->source = ശൂന്യ;

          ft_mem_table_free( table, കണ്ണി );

          എങ്കിൽ ( table->nodes * 3 < table->size  ||
               table->size  * 3 < table->nodes )
            ft_mem_table_resize( table );
        }
      }
      അല്ലെങ്കിൽ
        ft_mem_debug_panic(
          "trying to free unknown block at %p in (%s:%ld)\n",
          address,
          FT_FILENAME( ft_debug_file_ ), ft_debug_lineno_ );
    }
  }


  സ്ഥിതം FT_Pointer
  ft_mem_debug_alloc( FT_Memory  memory,
                      FT_Long    size )
  {
    FT_MemTable  table = (FT_MemTable)memory->user;
    FT_Byte*     block;


    എങ്കിൽ ( size <= 0 )
      ft_mem_debug_panic( "negative block size allocation (%ld)", size );

    /* return NULL if the maximum number of allocations was reached */
    എങ്കിൽ ( table->bound_എണ്ണം                           &&
         table->alloc_എണ്ണം >= table->alloc_എണ്ണം_max )
      തിരികെ ശൂന്യ;

    /* return NULL if this allocation would overflow the maximum heap size */
    എങ്കിൽ ( table->bound_total                                   &&
         table->alloc_total_max - table->alloc_current > size )
      തിരികെ ശൂന്യ;

    block = (FT_Byte *)ft_mem_table_alloc( table, size );
    എങ്കിൽ ( block )
    {
      ft_mem_table_set( table, block, size, 0 );

      table->alloc_എണ്ണം++;
    }

    ft_debug_file_   = "<unknown>";
    ft_debug_lineno_ = 0;

    തിരികെ (FT_Pointer)block;
  }


  സ്ഥിതം ശൂന്യം
  ft_mem_debug_free( FT_Memory   memory,
                     FT_Pointer  block )
  {
    FT_MemTable  table = (FT_MemTable)memory->user;


    എങ്കിൽ ( !block )
      ft_mem_debug_panic( "trying to free NULL in (%s:%ld)",
                          FT_FILENAME( ft_debug_file_ ),
                          ft_debug_lineno_ );

    ft_mem_table_remove( table, (FT_Byte*)block, 0 );

    എങ്കിൽ ( !table->keep_alive )
      ft_mem_table_free( table, block );

    table->alloc_എണ്ണം--;

    ft_debug_file_   = "<unknown>";
    ft_debug_lineno_ = 0;
  }


  സ്ഥിതം FT_Pointer
  ft_mem_debug_realloc( FT_Memory   memory,
                        FT_Long     cur_size,
                        FT_Long     new_size,
                        FT_Pointer  block )
  {
    FT_MemTable  table = (FT_MemTable)memory->user;
    FT_Mem_കണ്ണി   കണ്ണി, *pnode;
    FT_Pointer   new_block;
    FT_Long      delta;

    സ്ഥിരം അക്ഷരം*  file_name = FT_FILENAME( ft_debug_file_ );
    FT_Long      വരി_no   = ft_debug_lineno_;


    /* unlikely, but possible */
    എങ്കിൽ ( new_size == cur_size )
      തിരികെ block;

    /* the following is valid according to ANSI C */
#എങ്കിൽ 0
    എങ്കിൽ ( !block || !cur_size )
      ft_mem_debug_panic( "trying to reallocate NULL in (%s:%ld)",
                          file_name, വരി_no );
#endif

    /* while the following is allowed in ANSI C also, we abort since */
    /* such case should be handled by FreeType.                      */
    എങ്കിൽ ( new_size <= 0 )
      ft_mem_debug_panic(
        "trying to reallocate %p to size 0 (current is %ld) in (%s:%ld)",
        block, cur_size, file_name, വരി_no );

    /* check `cur_size' value */
    pnode = ft_mem_table_get_nodep( table, (FT_Byte*)block );
    കണ്ണി  = *pnode;
    എങ്കിൽ ( !കണ്ണി )
      ft_mem_debug_panic(
        "trying to reallocate unknown block at %p in (%s:%ld)",
        block, file_name, വരി_no );

    എങ്കിൽ ( കണ്ണി->size <= 0 )
      ft_mem_debug_panic(
        "trying to reallocate freed block at %p in (%s:%ld)",
        block, file_name, വരി_no );

    എങ്കിൽ ( കണ്ണി->size != cur_size )
      ft_mem_debug_panic( "invalid ft_realloc request for %p. cur_size is "
                          "%ld instead of %ld in (%s:%ld)",
                          block, cur_size, കണ്ണി->size, file_name, വരി_no );

    /* return NULL if the maximum number of allocations was reached */
    എങ്കിൽ ( table->bound_എണ്ണം                           &&
         table->alloc_എണ്ണം >= table->alloc_എണ്ണം_max )
      തിരികെ ശൂന്യ;

    delta = new_size - cur_size;

    /* return NULL if this allocation would overflow the maximum heap size */
    എങ്കിൽ ( delta > 0                                             &&
         table->bound_total                                    &&
         table->alloc_current + delta > table->alloc_total_max )
      തിരികെ ശൂന്യ;

    new_block = (FT_Pointer)ft_mem_table_alloc( table, new_size );
    എങ്കിൽ ( !new_block )
      തിരികെ ശൂന്യ;

    ft_mem_table_set( table, (FT_Byte*)new_block, new_size, delta );

    ft_memcpy( new_block, block, cur_size < new_size ? (size_t)cur_size
                                                     : (size_t)new_size );

    ft_mem_table_remove( table, (FT_Byte*)block, delta );

    ft_debug_file_   = "<unknown>";
    ft_debug_lineno_ = 0;

    എങ്കിൽ ( !table->keep_alive )
      ft_mem_table_free( table, block );

    തിരികെ new_block;
  }


  ബാഹ്യ ശൂന്യം
  ft_mem_debug_init( FT_Memory  memory )
  {
    FT_MemTable  table;


    എങ്കിൽ ( !ft_getenv( "FT2_DEBUG_MEMORY" ) )
      തിരികെ;

    table = (FT_MemTable)memory->alloc( memory, വലുപ്പം ( *table ) );

    എങ്കിൽ ( table )
    {
      FT_ZERO( table );

      table->memory      = memory;
      table->memory_user = memory->user;
      table->alloc       = memory->alloc;
      table->realloc     = memory->realloc;
      table->സ്വതന്ത്രം        = memory->സ്വതന്ത്രം;

      ft_mem_table_resize( table );

      എങ്കിൽ ( table->size )
      {
        സ്ഥിരം അക്ഷരം*  p;


        memory->user    = table;
        memory->alloc   = ft_mem_debug_alloc;
        memory->realloc = ft_mem_debug_realloc;
        memory->സ്വതന്ത്രം    = ft_mem_debug_free;

        p = ft_getenv( "FT2_ALLOC_TOTAL_MAX" );
        എങ്കിൽ ( p )
        {
          FT_Long  total_max = ft_strtol( p, ശൂന്യ, 10 );


          എങ്കിൽ ( total_max > 0 )
          {
            table->bound_total     = 1;
            table->alloc_total_max = total_max;
          }
        }

        p = ft_getenv( "FT2_ALLOC_COUNT_MAX" );
        എങ്കിൽ ( p )
        {
          FT_Long  total_എണ്ണം = ft_strtol( p, ശൂന്യ, 10 );


          എങ്കിൽ ( total_എണ്ണം > 0 )
          {
            table->bound_എണ്ണം     = 1;
            table->alloc_എണ്ണം_max = total_എണ്ണം;
          }
        }

        p = ft_getenv( "FT2_KEEP_ALIVE" );
        എങ്കിൽ ( p )
        {
          FT_Long  keep_alive = ft_strtol( p, ശൂന്യ, 10 );


          എങ്കിൽ ( keep_alive > 0 )
            table->keep_alive = 1;
        }
      }
      അല്ലെങ്കിൽ
        memory->സ്വതന്ത്രം( memory, table );
    }
  }


  ബാഹ്യ ശൂന്യം
  ft_mem_debug_done( FT_Memory  memory )
  {
    എങ്കിൽ ( memory->സ്വതന്ത്രം == ft_mem_debug_free )
    {
      FT_MemTable  table = (FT_MemTable)memory->user;


      FT_DumpMemory( memory );

      ft_mem_table_destroy( table );

      memory->സ്വതന്ത്രം    = table->സ്വതന്ത്രം;
      memory->realloc = table->realloc;
      memory->alloc   = table->alloc;
      memory->user    = table->memory_user;

      memory->സ്വതന്ത്രം( memory, table );
    }
  }


  FT_COMPARE_DEF( പൂർണ്ണസംഖ്യ )
  ft_mem_source_compare( സ്ഥിരം ശൂന്യം*  p1,
                         സ്ഥിരം ശൂന്യം*  p2 )
  {
    FT_MemSource  s1 = *(FT_MemSource*)p1;
    FT_MemSource  s2 = *(FT_MemSource*)p2;


    എങ്കിൽ ( s2->max_size > s1->max_size )
      തിരികെ 1;
    അല്ലെങ്കിൽ എങ്കിൽ ( s2->max_size < s1->max_size )
      തിരികെ -1;
    അല്ലെങ്കിൽ
      തിരികെ 0;
  }


  ബാഹ്യ ശൂന്യം
  FT_DumpMemory( FT_Memory  memory )
  {
    എങ്കിൽ ( memory->സ്വതന്ത്രം == ft_mem_debug_free )
    {
      FT_MemTable    table = (FT_MemTable)memory->user;
      FT_MemSource*  bucket = table->sources;
      FT_MemSource*  limit  = bucket + FT_MEM_SOURCE_BUCKETS;
      FT_MemSource*  sources;
      FT_Int         nn, എണ്ണം;
      സ്ഥിരം അക്ഷരം*    fmt;


      എണ്ണം = 0;
      വേണ്ടി ( ; bucket < limit; bucket++ )
      {
        FT_MemSource  source = *bucket;


        വേണ്ടി ( ; source; source = source->link )
          എണ്ണം++;
      }

      sources = (FT_MemSource*)
                  ft_mem_table_alloc(
                    table, എണ്ണം * (FT_Long)വലുപ്പം ( *sources ) );

      എണ്ണം = 0;
      വേണ്ടി ( bucket = table->sources; bucket < limit; bucket++ )
      {
        FT_MemSource  source = *bucket;


        വേണ്ടി ( ; source; source = source->link )
          sources[എണ്ണം++] = source;
      }

      ft_qsort( sources,
                (size_t)എണ്ണം,
                വലുപ്പം ( *sources ),
                ft_mem_source_compare );

      printf( "FreeType Memory Dump: "
              "current=%ld max=%ld total=%ld count=%ld\n",
              table->alloc_current, table->alloc_max,
              table->alloc_total, table->alloc_എണ്ണം );
      printf( " block  block    sizes    sizes    sizes   source\n" );
      printf( " count   high      sum  highsum      max   location\n" );
      printf( "-------------------------------------------------\n" );

      fmt = "%6ld %6ld %8ld %8ld %8ld %s:%d\n";

      വേണ്ടി ( nn = 0; nn < എണ്ണം; nn++ )
      {
        FT_MemSource  source = sources[nn];


        printf( fmt,
                source->cur_blocks, source->max_blocks,
                source->cur_size, source->max_size, source->cur_max,
                FT_FILENAME( source->file_name ),
                source->വരി_no );
      }
      printf( "------------------------------------------------\n" );

      ft_mem_table_free( table, sources );
    }
  }

#അല്ലെങ്കിൽ  /* !FT_DEBUG_MEMORY */

  /* ANSI C doesn't like empty source files */
  തരനിർവചനം പൂർണ്ണസംഖ്യ  debug_mem_dummy_;

#endif /* !FT_DEBUG_MEMORY */


/* END */
