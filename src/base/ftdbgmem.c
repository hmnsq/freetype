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


#அந்தர்பாவ <ft2build.h>
#அந்தர்பாவ FT_CONFIG_CONFIG_H
#அந்தர்பாவ <freetype/internal/ftdebug.h>
#அந்தர்பாவ <freetype/internal/ftmemory.h>
#அந்தர்பாவ <freetype/ftsystem.h>
#அந்தர்பாவ <freetype/fterrors.h>
#அந்தர்பாவ <freetype/fttypes.h>


#ifdef FT_DEBUG_MEMORY

#define  KEEPALIVE /* `Keep alive' means that freed blocks aren't released
                    * to the heap.  This is useful to detect double-frees
                    * or weird heap corruption, but it uses large amounts of
                    * memory, however.
                    */

#அந்தர்பாவ FT_CONFIG_STANDARD_LIBRARY_H

  FT_BASE_DEF( மாறிலி எழுத்து* )  ft_debug_file_   = வெற்று;
  FT_BASE_DEF( long )         ft_debug_lineno_ = 0;

  வெளி வெறுமை
  FT_DumpMemory( FT_Memory  memory );


  வகைவரை கட்டமைப்பு FT_MemSourceRec_*  FT_MemSource;
  வகைவரை கட்டமைப்பு FT_Mem_முனை_Rec*    FT_Mem_முனை;
  வகைவரை கட்டமைப்பு FT_MemTableRec_*   FT_MemTable;


#define FT_MEM_VAL( addr )  ( (FT_PtrDist)(FT_Pointer)( addr ) )

  /*
   * This structure holds statistics for a single allocation/release
   * site.  This is useful to know where memory operations happen the
   * most.
   */
  வகைவரை கட்டமைப்பு  FT_MemSourceRec_
  {
    மாறிலி எழுத்து*   file_name;
    long          வரி_no;

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
  வகைவரை கட்டமைப்பு  FT_Mem_முனை_Rec
  {
    FT_Byte*      address;
    FT_Long       size;     /* < 0 if the block was freed */

    FT_MemSource  source;

#ifdef KEEPALIVE
    மாறிலி எழுத்து*   free_file_name;
    FT_Long       free_வரி_no;
#endif

    FT_Mem_முனை    link;

  } FT_Mem_முனை_Rec_2;


  /*
   * The global structure, containing compound statistics and all hash
   * tables.
   */
  வகைவரை கட்டமைப்பு  FT_MemTableRec_
  {
    FT_Long          size;
    FT_Long          nodes;
    FT_Mem_முனை*      buckets;

    FT_Long          alloc_total;
    FT_Long          alloc_current;
    FT_Long          alloc_max;
    FT_Long          alloc_மொத்தம்;

    FT_Bool          bound_total;
    FT_Long          alloc_total_max;

    FT_Bool          bound_மொத்தம்;
    FT_Long          alloc_மொத்தம்_max;

    FT_MemSource     sources[FT_MEM_SOURCE_BUCKETS];

    FT_Bool          keep_alive;

    FT_Memory        memory;
    FT_Pointer       memory_user;
    FT_Alloc_Func    alloc;
    FT_Free_Func     விடுவி;
    FT_Realloc_Func  realloc;

  } FT_MemTableRec;


#define FT_MEM_SIZE_MAX  13845163

#define FT_FILENAME( x )  ( (x) ? (x) : "unknown file" )


  /*
   * Prime numbers are ugly to handle.  It would be better to implement
   * L-Hashing, which is 10% faster and doesn't require divisions.
   */
  நிலையான மாறிலி FT_Int  ft_mem_primes[] =
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


  நிலையான FT_Long
  ft_mem_closest_பகாஎண்( FT_Long  இலக்கம் )
  {
    size_t  i;


    க்கு ( i = 0;
          i < அளவு ( ft_mem_primes ) / அளவு ( ft_mem_primes[0] ); i++ )
      எனில் ( ft_mem_primes[i] > இலக்கம் )
        திரும்பு ft_mem_primes[i];

    திரும்பு FT_MEM_SIZE_MAX;
  }


  நிலையான வெறுமை
  ft_mem_debug_panic( மாறிலி எழுத்து*  fmt,
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


  நிலையான FT_Pointer
  ft_mem_table_alloc( FT_MemTable  table,
                      FT_Long      size )
  {
    FT_Memory   memory = table->memory;
    FT_Pointer  block;


    memory->user = table->memory_user;
    block = table->alloc( memory, size );
    memory->user = table;

    திரும்பு block;
  }


  நிலையான வெறுமை
  ft_mem_table_free( FT_MemTable  table,
                     FT_Pointer   block )
  {
    FT_Memory  memory = table->memory;


    memory->user = table->memory_user;
    table->விடுவி( memory, block );
    memory->user = table;
  }


  நிலையான வெறுமை
  ft_mem_table_resize( FT_MemTable  table )
  {
    FT_Long  new_size;


    new_size = ft_mem_closest_பகாஎண்( table->nodes );
    எனில் ( new_size != table->size )
    {
      FT_Mem_முனை*  new_buckets;
      FT_Long      i;


      new_buckets = (FT_Mem_முனை *)
                      ft_mem_table_alloc(
                        table,
                        new_size * (FT_Long)அளவு ( FT_Mem_முனை ) );
      எனில் ( !new_buckets )
        திரும்பு;

      FT_ARRAY_ZERO( new_buckets, new_size );

      க்கு ( i = 0; i < table->size; i++ )
      {
        FT_Mem_முனை  முனை, next, *pnode;
        FT_PtrDist  hash;


        முனை = table->buckets[i];
        போது ( முனை )
        {
          next  = முனை->link;
          hash  = FT_MEM_VAL( முனை->address ) % (FT_PtrDist)new_size;
          pnode = new_buckets + hash;

          முனை->link = pnode[0];
          pnode[0]   = முனை;

          முனை = next;
        }
      }

      எனில் ( table->buckets )
        ft_mem_table_free( table, table->buckets );

      table->buckets = new_buckets;
      table->size    = new_size;
    }
  }


  நிலையான வெறுமை
  ft_mem_table_destroy( FT_MemTable  table )
  {
    FT_Long  i;
    FT_Long  leak_மொத்தம் = 0;
    FT_Long  leaks      = 0;


    /* remove all blocks from the table, revealing leaked ones */
    க்கு ( i = 0; i < table->size; i++ )
    {
      FT_Mem_முனை  *pnode = table->buckets + i, next, முனை = *pnode;


      போது ( முனை )
      {
        next       = முனை->link;
        முனை->link = வெற்று;

        எனில் ( முனை->size > 0 )
        {
          printf(
            "leaked memory block at address %p, size %8ld in (%s:%ld)\n",
            (வெறுமை*)முனை->address,
            முனை->size,
            FT_FILENAME( முனை->source->file_name ),
            முனை->source->வரி_no );

          leak_மொத்தம்++;
          leaks += முனை->size;

          ft_mem_table_free( table, முனை->address );
        }

        முனை->address = வெற்று;
        முனை->size    = 0;

        ft_mem_table_free( table, முனை );
        முனை = next;
      }
      table->buckets[i] = வெற்று;
    }

    ft_mem_table_free( table, table->buckets );
    table->buckets = வெற்று;

    table->size  = 0;
    table->nodes = 0;

    /* remove all sources */
    க்கு ( i = 0; i < FT_MEM_SOURCE_BUCKETS; i++ )
    {
      FT_MemSource  source, next;


      க்கு ( source = table->sources[i]; source != வெற்று; source = next )
      {
        next = source->link;
        ft_mem_table_free( table, source );
      }

      table->sources[i] = வெற்று;
    }

    printf( "FreeType: total memory allocations = %ld\n",
            table->alloc_total );
    printf( "FreeType: maximum memory footprint = %ld\n",
            table->alloc_max );

    எனில் ( leak_மொத்தம் > 0 )
      ft_mem_debug_panic(
        "FreeType: %ld bytes of memory leaked in %ld blocks\n",
        leaks, leak_மொத்தம் );

    printf( "FreeType: no memory leaks detected\n" );
  }


  நிலையான FT_Mem_முனை*
  ft_mem_table_get_nodep( FT_MemTable  table,
                          FT_Byte*     address )
  {
    FT_PtrDist   hash;
    FT_Mem_முனை  *pnode, முனை;


    hash  = FT_MEM_VAL( address );
    pnode = table->buckets + ( hash % (FT_PtrDist)table->size );

    க்கு (;;)
    {
      முனை = pnode[0];
      எனில் ( !முனை )
        நிறுத்து;

      எனில் ( முனை->address == address )
        நிறுத்து;

      pnode = &முனை->link;
    }
    திரும்பு pnode;
  }


  நிலையான FT_MemSource
  ft_mem_table_get_source( FT_MemTable  table )
  {
    FT_UInt32     hash;
    FT_MemSource  முனை, *pnode;


    /* cast to FT_PtrDist first since void* can be larger */
    /* than FT_UInt32 and GCC 4.1.1 emits a warning       */
    hash  = (FT_UInt32)(FT_PtrDist)(வெறுமை*)ft_debug_file_ +
              (FT_UInt32)( 5 * ft_debug_lineno_ );
    pnode = &table->sources[hash % FT_MEM_SOURCE_BUCKETS];

    க்கு (;;)
    {
      முனை = *pnode;
      எனில் ( !முனை )
        நிறுத்து;

      எனில் ( முனை->file_name == ft_debug_file_   &&
           முனை->வரி_no   == ft_debug_lineno_ )
        செல் Exit;

      pnode = &முனை->link;
    }

    முனை = (FT_MemSource)ft_mem_table_alloc( table, அளவு ( *முனை ) );
    எனில் ( !முனை )
      ft_mem_debug_panic(
        "not enough memory to perform memory debugging\n" );

    முனை->file_name = ft_debug_file_;
    முனை->வரி_no   = ft_debug_lineno_;

    முனை->cur_blocks = 0;
    முனை->max_blocks = 0;
    முனை->all_blocks = 0;

    முனை->cur_size = 0;
    முனை->max_size = 0;
    முனை->all_size = 0;

    முனை->cur_max = 0;

    முனை->link = வெற்று;
    முனை->hash = hash;
    *pnode     = முனை;

  Exit:
    திரும்பு முனை;
  }


  நிலையான வெறுமை
  ft_mem_table_set( FT_MemTable  table,
                    FT_Byte*     address,
                    FT_Long      size,
                    FT_Long      delta )
  {
    FT_Mem_முனை  *pnode, முனை;


    எனில் ( table )
    {
      FT_MemSource  source;


      pnode = ft_mem_table_get_nodep( table, address );
      முனை  = *pnode;
      எனில் ( முனை )
      {
        எனில் ( முனை->size < 0 )
        {
          /* This block was already freed.  Our memory is now completely */
          /* corrupted!                                                  */
          /* This can only happen in keep-alive mode.                    */
          ft_mem_debug_panic(
            "memory heap corrupted (allocating freed block)" );
        }
        இல்லையெனில்
        {
          /* This block was already allocated.  This means that our memory */
          /* is also corrupted!                                            */
          ft_mem_debug_panic(
            "memory heap corrupted (re-allocating allocated block at"
            " %p, of size %ld)\n"
            "org=%s:%d new=%s:%d\n",
            முனை->address, முனை->size,
            FT_FILENAME( முனை->source->file_name ), முனை->source->வரி_no,
            FT_FILENAME( ft_debug_file_ ), ft_debug_lineno_ );
        }
      }

      /* we need to create a new node in this table */
      முனை = (FT_Mem_முனை)ft_mem_table_alloc( table, அளவு ( *முனை ) );
      எனில் ( !முனை )
        ft_mem_debug_panic( "not enough memory to run memory tests" );

      முனை->address = address;
      முனை->size    = size;
      முனை->source  = source = ft_mem_table_get_source( table );

      எனில் ( delta == 0 )
      {
        /* this is an allocation */
        source->all_blocks++;
        source->cur_blocks++;
        எனில் ( source->cur_blocks > source->max_blocks )
          source->max_blocks = source->cur_blocks;
      }

      எனில் ( size > source->cur_max )
        source->cur_max = size;

      எனில் ( delta != 0 )
      {
        /* we are growing or shrinking a reallocated block */
        source->cur_size     += delta;
        table->alloc_current += delta;
      }
      இல்லையெனில்
      {
        /* we are allocating a new block */
        source->cur_size     += size;
        table->alloc_current += size;
      }

      source->all_size += size;

      எனில் ( source->cur_size > source->max_size )
        source->max_size = source->cur_size;

      முனை->free_file_name = வெற்று;
      முனை->free_வரி_no   = 0;

      முனை->link = pnode[0];

      pnode[0] = முனை;
      table->nodes++;

      table->alloc_total += size;

      எனில் ( table->alloc_current > table->alloc_max )
        table->alloc_max = table->alloc_current;

      எனில் ( table->nodes * 3 < table->size  ||
           table->size  * 3 < table->nodes )
        ft_mem_table_resize( table );
    }
  }


  நிலையான வெறுமை
  ft_mem_table_remove( FT_MemTable  table,
                       FT_Byte*     address,
                       FT_Long      delta )
  {
    எனில் ( table )
    {
      FT_Mem_முனை  *pnode, முனை;


      pnode = ft_mem_table_get_nodep( table, address );
      முனை  = *pnode;
      எனில் ( முனை )
      {
        FT_MemSource  source;


        எனில் ( முனை->size < 0 )
          ft_mem_debug_panic(
            "freeing memory block at %p more than once\n"
            "  at (%s:%ld)!\n"
            "  Block was allocated at (%s:%ld)\n"
            "  and released at (%s:%ld).",
            address,
            FT_FILENAME( ft_debug_file_ ), ft_debug_lineno_,
            FT_FILENAME( முனை->source->file_name ), முனை->source->வரி_no,
            FT_FILENAME( முனை->free_file_name ), முனை->free_வரி_no );

        /* scramble the node's content for additional safety */
        FT_MEM_SET( address, 0xF3, முனை->size );

        எனில் ( delta == 0 )
        {
          source = முனை->source;

          source->cur_blocks--;
          source->cur_size -= முனை->size;

          table->alloc_current -= முனை->size;
        }

        எனில் ( table->keep_alive )
        {
          /* we simply invert the node's size to indicate that the node */
          /* was freed.                                                 */
          முனை->size           = -முனை->size;
          முனை->free_file_name = ft_debug_file_;
          முனை->free_வரி_no   = ft_debug_lineno_;
        }
        இல்லையெனில்
        {
          table->nodes--;

          *pnode = முனை->link;

          முனை->size   = 0;
          முனை->source = வெற்று;

          ft_mem_table_free( table, முனை );

          எனில் ( table->nodes * 3 < table->size  ||
               table->size  * 3 < table->nodes )
            ft_mem_table_resize( table );
        }
      }
      இல்லையெனில்
        ft_mem_debug_panic(
          "trying to free unknown block at %p in (%s:%ld)\n",
          address,
          FT_FILENAME( ft_debug_file_ ), ft_debug_lineno_ );
    }
  }


  நிலையான FT_Pointer
  ft_mem_debug_alloc( FT_Memory  memory,
                      FT_Long    size )
  {
    FT_MemTable  table = (FT_MemTable)memory->user;
    FT_Byte*     block;


    எனில் ( size <= 0 )
      ft_mem_debug_panic( "negative block size allocation (%ld)", size );

    /* return NULL if the maximum number of allocations was reached */
    எனில் ( table->bound_மொத்தம்                           &&
         table->alloc_மொத்தம் >= table->alloc_மொத்தம்_max )
      திரும்பு வெற்று;

    /* return NULL if this allocation would overflow the maximum heap size */
    எனில் ( table->bound_total                                   &&
         table->alloc_total_max - table->alloc_current > size )
      திரும்பு வெற்று;

    block = (FT_Byte *)ft_mem_table_alloc( table, size );
    எனில் ( block )
    {
      ft_mem_table_set( table, block, size, 0 );

      table->alloc_மொத்தம்++;
    }

    ft_debug_file_   = "<unknown>";
    ft_debug_lineno_ = 0;

    திரும்பு (FT_Pointer)block;
  }


  நிலையான வெறுமை
  ft_mem_debug_free( FT_Memory   memory,
                     FT_Pointer  block )
  {
    FT_MemTable  table = (FT_MemTable)memory->user;


    எனில் ( !block )
      ft_mem_debug_panic( "trying to free NULL in (%s:%ld)",
                          FT_FILENAME( ft_debug_file_ ),
                          ft_debug_lineno_ );

    ft_mem_table_remove( table, (FT_Byte*)block, 0 );

    எனில் ( !table->keep_alive )
      ft_mem_table_free( table, block );

    table->alloc_மொத்தம்--;

    ft_debug_file_   = "<unknown>";
    ft_debug_lineno_ = 0;
  }


  நிலையான FT_Pointer
  ft_mem_debug_realloc( FT_Memory   memory,
                        FT_Long     cur_size,
                        FT_Long     new_size,
                        FT_Pointer  block )
  {
    FT_MemTable  table = (FT_MemTable)memory->user;
    FT_Mem_முனை   முனை, *pnode;
    FT_Pointer   new_block;
    FT_Long      delta;

    மாறிலி எழுத்து*  file_name = FT_FILENAME( ft_debug_file_ );
    FT_Long      வரி_no   = ft_debug_lineno_;


    /* unlikely, but possible */
    எனில் ( new_size == cur_size )
      திரும்பு block;

    /* the following is valid according to ANSI C */
#எனில் 0
    எனில் ( !block || !cur_size )
      ft_mem_debug_panic( "trying to reallocate NULL in (%s:%ld)",
                          file_name, வரி_no );
#endif

    /* while the following is allowed in ANSI C also, we abort since */
    /* such case should be handled by FreeType.                      */
    எனில் ( new_size <= 0 )
      ft_mem_debug_panic(
        "trying to reallocate %p to size 0 (current is %ld) in (%s:%ld)",
        block, cur_size, file_name, வரி_no );

    /* check `cur_size' value */
    pnode = ft_mem_table_get_nodep( table, (FT_Byte*)block );
    முனை  = *pnode;
    எனில் ( !முனை )
      ft_mem_debug_panic(
        "trying to reallocate unknown block at %p in (%s:%ld)",
        block, file_name, வரி_no );

    எனில் ( முனை->size <= 0 )
      ft_mem_debug_panic(
        "trying to reallocate freed block at %p in (%s:%ld)",
        block, file_name, வரி_no );

    எனில் ( முனை->size != cur_size )
      ft_mem_debug_panic( "invalid ft_realloc request for %p. cur_size is "
                          "%ld instead of %ld in (%s:%ld)",
                          block, cur_size, முனை->size, file_name, வரி_no );

    /* return NULL if the maximum number of allocations was reached */
    எனில் ( table->bound_மொத்தம்                           &&
         table->alloc_மொத்தம் >= table->alloc_மொத்தம்_max )
      திரும்பு வெற்று;

    delta = new_size - cur_size;

    /* return NULL if this allocation would overflow the maximum heap size */
    எனில் ( delta > 0                                             &&
         table->bound_total                                    &&
         table->alloc_current + delta > table->alloc_total_max )
      திரும்பு வெற்று;

    new_block = (FT_Pointer)ft_mem_table_alloc( table, new_size );
    எனில் ( !new_block )
      திரும்பு வெற்று;

    ft_mem_table_set( table, (FT_Byte*)new_block, new_size, delta );

    ft_memcpy( new_block, block, cur_size < new_size ? (size_t)cur_size
                                                     : (size_t)new_size );

    ft_mem_table_remove( table, (FT_Byte*)block, delta );

    ft_debug_file_   = "<unknown>";
    ft_debug_lineno_ = 0;

    எனில் ( !table->keep_alive )
      ft_mem_table_free( table, block );

    திரும்பு new_block;
  }


  வெளி வெறுமை
  ft_mem_debug_init( FT_Memory  memory )
  {
    FT_MemTable  table;


    எனில் ( !ft_getenv( "FT2_DEBUG_MEMORY" ) )
      திரும்பு;

    table = (FT_MemTable)memory->alloc( memory, அளவு ( *table ) );

    எனில் ( table )
    {
      FT_ZERO( table );

      table->memory      = memory;
      table->memory_user = memory->user;
      table->alloc       = memory->alloc;
      table->realloc     = memory->realloc;
      table->விடுவி        = memory->விடுவி;

      ft_mem_table_resize( table );

      எனில் ( table->size )
      {
        மாறிலி எழுத்து*  p;


        memory->user    = table;
        memory->alloc   = ft_mem_debug_alloc;
        memory->realloc = ft_mem_debug_realloc;
        memory->விடுவி    = ft_mem_debug_free;

        p = ft_getenv( "FT2_ALLOC_TOTAL_MAX" );
        எனில் ( p )
        {
          FT_Long  total_max = ft_strtol( p, வெற்று, 10 );


          எனில் ( total_max > 0 )
          {
            table->bound_total     = 1;
            table->alloc_total_max = total_max;
          }
        }

        p = ft_getenv( "FT2_ALLOC_COUNT_MAX" );
        எனில் ( p )
        {
          FT_Long  total_மொத்தம் = ft_strtol( p, வெற்று, 10 );


          எனில் ( total_மொத்தம் > 0 )
          {
            table->bound_மொத்தம்     = 1;
            table->alloc_மொத்தம்_max = total_மொத்தம்;
          }
        }

        p = ft_getenv( "FT2_KEEP_ALIVE" );
        எனில் ( p )
        {
          FT_Long  keep_alive = ft_strtol( p, வெற்று, 10 );


          எனில் ( keep_alive > 0 )
            table->keep_alive = 1;
        }
      }
      இல்லையெனில்
        memory->விடுவி( memory, table );
    }
  }


  வெளி வெறுமை
  ft_mem_debug_done( FT_Memory  memory )
  {
    எனில் ( memory->விடுவி == ft_mem_debug_free )
    {
      FT_MemTable  table = (FT_MemTable)memory->user;


      FT_DumpMemory( memory );

      ft_mem_table_destroy( table );

      memory->விடுவி    = table->விடுவி;
      memory->realloc = table->realloc;
      memory->alloc   = table->alloc;
      memory->user    = table->memory_user;

      memory->விடுவி( memory, table );
    }
  }


  FT_COMPARE_DEF( முழு )
  ft_mem_source_compare( மாறிலி வெறுமை*  p1,
                         மாறிலி வெறுமை*  p2 )
  {
    FT_MemSource  s1 = *(FT_MemSource*)p1;
    FT_MemSource  s2 = *(FT_MemSource*)p2;


    எனில் ( s2->max_size > s1->max_size )
      திரும்பு 1;
    இல்லையெனில் எனில் ( s2->max_size < s1->max_size )
      திரும்பு -1;
    இல்லையெனில்
      திரும்பு 0;
  }


  வெளி வெறுமை
  FT_DumpMemory( FT_Memory  memory )
  {
    எனில் ( memory->விடுவி == ft_mem_debug_free )
    {
      FT_MemTable    table = (FT_MemTable)memory->user;
      FT_MemSource*  bucket = table->sources;
      FT_MemSource*  limit  = bucket + FT_MEM_SOURCE_BUCKETS;
      FT_MemSource*  sources;
      FT_Int         nn, மொத்தம்;
      மாறிலி எழுத்து*    fmt;


      மொத்தம் = 0;
      க்கு ( ; bucket < limit; bucket++ )
      {
        FT_MemSource  source = *bucket;


        க்கு ( ; source; source = source->link )
          மொத்தம்++;
      }

      sources = (FT_MemSource*)
                  ft_mem_table_alloc(
                    table, மொத்தம் * (FT_Long)அளவு ( *sources ) );

      மொத்தம் = 0;
      க்கு ( bucket = table->sources; bucket < limit; bucket++ )
      {
        FT_MemSource  source = *bucket;


        க்கு ( ; source; source = source->link )
          sources[மொத்தம்++] = source;
      }

      ft_qsort( sources,
                (size_t)மொத்தம்,
                அளவு ( *sources ),
                ft_mem_source_compare );

      printf( "FreeType Memory Dump: "
              "current=%ld max=%ld total=%ld count=%ld\n",
              table->alloc_current, table->alloc_max,
              table->alloc_total, table->alloc_மொத்தம் );
      printf( " block  block    sizes    sizes    sizes   source\n" );
      printf( " count   high      sum  highsum      max   location\n" );
      printf( "-------------------------------------------------\n" );

      fmt = "%6ld %6ld %8ld %8ld %8ld %s:%d\n";

      க்கு ( nn = 0; nn < மொத்தம்; nn++ )
      {
        FT_MemSource  source = sources[nn];


        printf( fmt,
                source->cur_blocks, source->max_blocks,
                source->cur_size, source->max_size, source->cur_max,
                FT_FILENAME( source->file_name ),
                source->வரி_no );
      }
      printf( "------------------------------------------------\n" );

      ft_mem_table_free( table, sources );
    }
  }

#இல்லையெனில்  /* !FT_DEBUG_MEMORY */

  /* ANSI C doesn't like empty source files */
  வகைவரை முழு  debug_mem_dummy_;

#endif /* !FT_DEBUG_MEMORY */


/* END */
