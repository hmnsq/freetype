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


#अन्तर्भाव <ft2build.h>
#अन्तर्भाव FT_CONFIG_CONFIG_H
#अन्तर्भाव <freetype/internal/ftdebug.h>
#अन्तर्भाव <freetype/internal/ftmemory.h>
#अन्तर्भाव <freetype/ftsystem.h>
#अन्तर्भाव <freetype/fterrors.h>
#अन्तर्भाव <freetype/fttypes.h>


#ifdef FT_DEBUG_MEMORY

#define  KEEPALIVE /* `Keep alive' means that freed blocks aren't released
                    * to the heap.  This is useful to detect double-frees
                    * or weird heap corruption, but it uses large amounts of
                    * memory, however.
                    */

#अन्तर्भाव FT_CONFIG_STANDARD_LIBRARY_H

  FT_BASE_DEF( ध्रुव वर्ण* )  ft_debug_file_   = रिक्त;
  FT_BASE_DEF( long )         ft_debug_lineno_ = 0;

  बाह्य शून्य
  FT_DumpMemory( FT_Memory  memory );


  प्रकारनिर्देश संरचना FT_MemSourceRec_*  FT_MemSource;
  प्रकारनिर्देश संरचना FT_Mem_ग्रन्थिः_Rec*    FT_Mem_ग्रन्थिः;
  प्रकारनिर्देश संरचना FT_MemTableRec_*   FT_MemTable;


#define FT_MEM_VAL( addr )  ( (FT_PtrDist)(FT_Pointer)( addr ) )

  /*
   * This structure holds statistics for a single allocation/release
   * site.  This is useful to know where memory operations happen the
   * most.
   */
  प्रकारनिर्देश संरचना  FT_MemSourceRec_
  {
    ध्रुव वर्ण*   file_name;
    long          पङ्क्तिः_no;

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
  प्रकारनिर्देश संरचना  FT_Mem_ग्रन्थिः_Rec
  {
    FT_Byte*      address;
    FT_Long       size;     /* < 0 if the block was freed */

    FT_MemSource  source;

#ifdef KEEPALIVE
    ध्रुव वर्ण*   free_file_name;
    FT_Long       free_पङ्क्तिः_no;
#endif

    FT_Mem_ग्रन्थिः    link;

  } FT_Mem_ग्रन्थिः_Rec_2;


  /*
   * The global structure, containing compound statistics and all hash
   * tables.
   */
  प्रकारनिर्देश संरचना  FT_MemTableRec_
  {
    FT_Long          size;
    FT_Long          nodes;
    FT_Mem_ग्रन्थिः*      buckets;

    FT_Long          alloc_total;
    FT_Long          alloc_current;
    FT_Long          alloc_max;
    FT_Long          alloc_गणनम्;

    FT_Bool          bound_total;
    FT_Long          alloc_total_max;

    FT_Bool          bound_गणनम्;
    FT_Long          alloc_गणनम्_max;

    FT_MemSource     sources[FT_MEM_SOURCE_BUCKETS];

    FT_Bool          keep_alive;

    FT_Memory        memory;
    FT_Pointer       memory_user;
    FT_Alloc_Func    alloc;
    FT_Free_Func     मुक्त;
    FT_Realloc_Func  realloc;

  } FT_MemTableRec;


#define FT_MEM_SIZE_MAX  13845163

#define FT_FILENAME( x )  ( (x) ? (x) : "unknown file" )


  /*
   * Prime numbers are ugly to handle.  It would be better to implement
   * L-Hashing, which is 10% faster and doesn't require divisions.
   */
  स्थिर ध्रुव FT_Int  ft_mem_primes[] =
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


  स्थिर FT_Long
  ft_mem_closest_अविभाज्यः( FT_Long  अङ्कः )
  {
    size_t  i;


    प्रति ( i = 0;
          i < परिमाण ( ft_mem_primes ) / परिमाण ( ft_mem_primes[0] ); i++ )
      जइ ( ft_mem_primes[i] > अङ्कः )
        प्रत्यागम ft_mem_primes[i];

    प्रत्यागम FT_MEM_SIZE_MAX;
  }


  स्थिर शून्य
  ft_mem_debug_panic( ध्रुव वर्ण*  fmt,
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


  स्थिर FT_Pointer
  ft_mem_table_alloc( FT_MemTable  table,
                      FT_Long      size )
  {
    FT_Memory   memory = table->memory;
    FT_Pointer  block;


    memory->user = table->memory_user;
    block = table->alloc( memory, size );
    memory->user = table;

    प्रत्यागम block;
  }


  स्थिर शून्य
  ft_mem_table_free( FT_MemTable  table,
                     FT_Pointer   block )
  {
    FT_Memory  memory = table->memory;


    memory->user = table->memory_user;
    table->मुक्त( memory, block );
    memory->user = table;
  }


  स्थिर शून्य
  ft_mem_table_resize( FT_MemTable  table )
  {
    FT_Long  new_size;


    new_size = ft_mem_closest_अविभाज्यः( table->nodes );
    जइ ( new_size != table->size )
    {
      FT_Mem_ग्रन्थिः*  new_buckets;
      FT_Long      i;


      new_buckets = (FT_Mem_ग्रन्थिः *)
                      ft_mem_table_alloc(
                        table,
                        new_size * (FT_Long)परिमाण ( FT_Mem_ग्रन्थिः ) );
      जइ ( !new_buckets )
        प्रत्यागम;

      FT_ARRAY_ZERO( new_buckets, new_size );

      प्रति ( i = 0; i < table->size; i++ )
      {
        FT_Mem_ग्रन्थिः  ग्रन्थिः, next, *pnode;
        FT_PtrDist  hash;


        ग्रन्थिः = table->buckets[i];
        जाव ( ग्रन्थिः )
        {
          next  = ग्रन्थिः->link;
          hash  = FT_MEM_VAL( ग्रन्थिः->address ) % (FT_PtrDist)new_size;
          pnode = new_buckets + hash;

          ग्रन्थिः->link = pnode[0];
          pnode[0]   = ग्रन्थिः;

          ग्रन्थिः = next;
        }
      }

      जइ ( table->buckets )
        ft_mem_table_free( table, table->buckets );

      table->buckets = new_buckets;
      table->size    = new_size;
    }
  }


  स्थिर शून्य
  ft_mem_table_destroy( FT_MemTable  table )
  {
    FT_Long  i;
    FT_Long  leak_गणनम् = 0;
    FT_Long  leaks      = 0;


    /* remove all blocks from the table, revealing leaked ones */
    प्रति ( i = 0; i < table->size; i++ )
    {
      FT_Mem_ग्रन्थिः  *pnode = table->buckets + i, next, ग्रन्थिः = *pnode;


      जाव ( ग्रन्थिः )
      {
        next       = ग्रन्थिः->link;
        ग्रन्थिः->link = रिक्त;

        जइ ( ग्रन्थिः->size > 0 )
        {
          printf(
            "leaked memory block at address %p, size %8ld in (%s:%ld)\n",
            (शून्य*)ग्रन्थिः->address,
            ग्रन्थिः->size,
            FT_FILENAME( ग्रन्थिः->source->file_name ),
            ग्रन्थिः->source->पङ्क्तिः_no );

          leak_गणनम्++;
          leaks += ग्रन्थिः->size;

          ft_mem_table_free( table, ग्रन्थिः->address );
        }

        ग्रन्थिः->address = रिक्त;
        ग्रन्थिः->size    = 0;

        ft_mem_table_free( table, ग्रन्थिः );
        ग्रन्थिः = next;
      }
      table->buckets[i] = रिक्त;
    }

    ft_mem_table_free( table, table->buckets );
    table->buckets = रिक्त;

    table->size  = 0;
    table->nodes = 0;

    /* remove all sources */
    प्रति ( i = 0; i < FT_MEM_SOURCE_BUCKETS; i++ )
    {
      FT_MemSource  source, next;


      प्रति ( source = table->sources[i]; source != रिक्त; source = next )
      {
        next = source->link;
        ft_mem_table_free( table, source );
      }

      table->sources[i] = रिक्त;
    }

    printf( "FreeType: total memory allocations = %ld\n",
            table->alloc_total );
    printf( "FreeType: maximum memory footprint = %ld\n",
            table->alloc_max );

    जइ ( leak_गणनम् > 0 )
      ft_mem_debug_panic(
        "FreeType: %ld bytes of memory leaked in %ld blocks\n",
        leaks, leak_गणनम् );

    printf( "FreeType: no memory leaks detected\n" );
  }


  स्थिर FT_Mem_ग्रन्थिः*
  ft_mem_table_get_nodep( FT_MemTable  table,
                          FT_Byte*     address )
  {
    FT_PtrDist   hash;
    FT_Mem_ग्रन्थिः  *pnode, ग्रन्थिः;


    hash  = FT_MEM_VAL( address );
    pnode = table->buckets + ( hash % (FT_PtrDist)table->size );

    प्रति (;;)
    {
      ग्रन्थिः = pnode[0];
      जइ ( !ग्रन्थिः )
        भङ्ग;

      जइ ( ग्रन्थिः->address == address )
        भङ्ग;

      pnode = &ग्रन्थिः->link;
    }
    प्रत्यागम pnode;
  }


  स्थिर FT_MemSource
  ft_mem_table_get_source( FT_MemTable  table )
  {
    FT_UInt32     hash;
    FT_MemSource  ग्रन्थिः, *pnode;


    /* cast to FT_PtrDist first since void* can be larger */
    /* than FT_UInt32 and GCC 4.1.1 emits a warning       */
    hash  = (FT_UInt32)(FT_PtrDist)(शून्य*)ft_debug_file_ +
              (FT_UInt32)( 5 * ft_debug_lineno_ );
    pnode = &table->sources[hash % FT_MEM_SOURCE_BUCKETS];

    प्रति (;;)
    {
      ग्रन्थिः = *pnode;
      जइ ( !ग्रन्थिः )
        भङ्ग;

      जइ ( ग्रन्थिः->file_name == ft_debug_file_   &&
           ग्रन्थिः->पङ्क्तिः_no   == ft_debug_lineno_ )
        गच्छ Exit;

      pnode = &ग्रन्थिः->link;
    }

    ग्रन्थिः = (FT_MemSource)ft_mem_table_alloc( table, परिमाण ( *ग्रन्थिः ) );
    जइ ( !ग्रन्थिः )
      ft_mem_debug_panic(
        "not enough memory to perform memory debugging\n" );

    ग्रन्थिः->file_name = ft_debug_file_;
    ग्रन्थिः->पङ्क्तिः_no   = ft_debug_lineno_;

    ग्रन्थिः->cur_blocks = 0;
    ग्रन्थिः->max_blocks = 0;
    ग्रन्थिः->all_blocks = 0;

    ग्रन्थिः->cur_size = 0;
    ग्रन्थिः->max_size = 0;
    ग्रन्थिः->all_size = 0;

    ग्रन्थिः->cur_max = 0;

    ग्रन्थिः->link = रिक्त;
    ग्रन्थिः->hash = hash;
    *pnode     = ग्रन्थिः;

  Exit:
    प्रत्यागम ग्रन्थिः;
  }


  स्थिर शून्य
  ft_mem_table_set( FT_MemTable  table,
                    FT_Byte*     address,
                    FT_Long      size,
                    FT_Long      delta )
  {
    FT_Mem_ग्रन्थिः  *pnode, ग्रन्थिः;


    जइ ( table )
    {
      FT_MemSource  source;


      pnode = ft_mem_table_get_nodep( table, address );
      ग्रन्थिः  = *pnode;
      जइ ( ग्रन्थिः )
      {
        जइ ( ग्रन्थिः->size < 0 )
        {
          /* This block was already freed.  Our memory is now completely */
          /* corrupted!                                                  */
          /* This can only happen in keep-alive mode.                    */
          ft_mem_debug_panic(
            "memory heap corrupted (allocating freed block)" );
        }
        अण्णहा
        {
          /* This block was already allocated.  This means that our memory */
          /* is also corrupted!                                            */
          ft_mem_debug_panic(
            "memory heap corrupted (re-allocating allocated block at"
            " %p, of size %ld)\n"
            "org=%s:%d new=%s:%d\n",
            ग्रन्थिः->address, ग्रन्थिः->size,
            FT_FILENAME( ग्रन्थिः->source->file_name ), ग्रन्थिः->source->पङ्क्तिः_no,
            FT_FILENAME( ft_debug_file_ ), ft_debug_lineno_ );
        }
      }

      /* we need to create a new node in this table */
      ग्रन्थिः = (FT_Mem_ग्रन्थिः)ft_mem_table_alloc( table, परिमाण ( *ग्रन्थिः ) );
      जइ ( !ग्रन्थिः )
        ft_mem_debug_panic( "not enough memory to run memory tests" );

      ग्रन्थिः->address = address;
      ग्रन्थिः->size    = size;
      ग्रन्थिः->source  = source = ft_mem_table_get_source( table );

      जइ ( delta == 0 )
      {
        /* this is an allocation */
        source->all_blocks++;
        source->cur_blocks++;
        जइ ( source->cur_blocks > source->max_blocks )
          source->max_blocks = source->cur_blocks;
      }

      जइ ( size > source->cur_max )
        source->cur_max = size;

      जइ ( delta != 0 )
      {
        /* we are growing or shrinking a reallocated block */
        source->cur_size     += delta;
        table->alloc_current += delta;
      }
      अण्णहा
      {
        /* we are allocating a new block */
        source->cur_size     += size;
        table->alloc_current += size;
      }

      source->all_size += size;

      जइ ( source->cur_size > source->max_size )
        source->max_size = source->cur_size;

      ग्रन्थिः->free_file_name = रिक्त;
      ग्रन्थिः->free_पङ्क्तिः_no   = 0;

      ग्रन्थिः->link = pnode[0];

      pnode[0] = ग्रन्थिः;
      table->nodes++;

      table->alloc_total += size;

      जइ ( table->alloc_current > table->alloc_max )
        table->alloc_max = table->alloc_current;

      जइ ( table->nodes * 3 < table->size  ||
           table->size  * 3 < table->nodes )
        ft_mem_table_resize( table );
    }
  }


  स्थिर शून्य
  ft_mem_table_remove( FT_MemTable  table,
                       FT_Byte*     address,
                       FT_Long      delta )
  {
    जइ ( table )
    {
      FT_Mem_ग्रन्थिः  *pnode, ग्रन्थिः;


      pnode = ft_mem_table_get_nodep( table, address );
      ग्रन्थिः  = *pnode;
      जइ ( ग्रन्थिः )
      {
        FT_MemSource  source;


        जइ ( ग्रन्थिः->size < 0 )
          ft_mem_debug_panic(
            "freeing memory block at %p more than once\n"
            "  at (%s:%ld)!\n"
            "  Block was allocated at (%s:%ld)\n"
            "  and released at (%s:%ld).",
            address,
            FT_FILENAME( ft_debug_file_ ), ft_debug_lineno_,
            FT_FILENAME( ग्रन्थिः->source->file_name ), ग्रन्थिः->source->पङ्क्तिः_no,
            FT_FILENAME( ग्रन्थिः->free_file_name ), ग्रन्थिः->free_पङ्क्तिः_no );

        /* scramble the node's content for additional safety */
        FT_MEM_SET( address, 0xF3, ग्रन्थिः->size );

        जइ ( delta == 0 )
        {
          source = ग्रन्थिः->source;

          source->cur_blocks--;
          source->cur_size -= ग्रन्थिः->size;

          table->alloc_current -= ग्रन्थिः->size;
        }

        जइ ( table->keep_alive )
        {
          /* we simply invert the node's size to indicate that the node */
          /* was freed.                                                 */
          ग्रन्थिः->size           = -ग्रन्थिः->size;
          ग्रन्थिः->free_file_name = ft_debug_file_;
          ग्रन्थिः->free_पङ्क्तिः_no   = ft_debug_lineno_;
        }
        अण्णहा
        {
          table->nodes--;

          *pnode = ग्रन्थिः->link;

          ग्रन्थिः->size   = 0;
          ग्रन्थिः->source = रिक्त;

          ft_mem_table_free( table, ग्रन्थिः );

          जइ ( table->nodes * 3 < table->size  ||
               table->size  * 3 < table->nodes )
            ft_mem_table_resize( table );
        }
      }
      अण्णहा
        ft_mem_debug_panic(
          "trying to free unknown block at %p in (%s:%ld)\n",
          address,
          FT_FILENAME( ft_debug_file_ ), ft_debug_lineno_ );
    }
  }


  स्थिर FT_Pointer
  ft_mem_debug_alloc( FT_Memory  memory,
                      FT_Long    size )
  {
    FT_MemTable  table = (FT_MemTable)memory->user;
    FT_Byte*     block;


    जइ ( size <= 0 )
      ft_mem_debug_panic( "negative block size allocation (%ld)", size );

    /* return NULL if the maximum number of allocations was reached */
    जइ ( table->bound_गणनम्                           &&
         table->alloc_गणनम् >= table->alloc_गणनम्_max )
      प्रत्यागम रिक्त;

    /* return NULL if this allocation would overflow the maximum heap size */
    जइ ( table->bound_total                                   &&
         table->alloc_total_max - table->alloc_current > size )
      प्रत्यागम रिक्त;

    block = (FT_Byte *)ft_mem_table_alloc( table, size );
    जइ ( block )
    {
      ft_mem_table_set( table, block, size, 0 );

      table->alloc_गणनम्++;
    }

    ft_debug_file_   = "<unknown>";
    ft_debug_lineno_ = 0;

    प्रत्यागम (FT_Pointer)block;
  }


  स्थिर शून्य
  ft_mem_debug_free( FT_Memory   memory,
                     FT_Pointer  block )
  {
    FT_MemTable  table = (FT_MemTable)memory->user;


    जइ ( !block )
      ft_mem_debug_panic( "trying to free NULL in (%s:%ld)",
                          FT_FILENAME( ft_debug_file_ ),
                          ft_debug_lineno_ );

    ft_mem_table_remove( table, (FT_Byte*)block, 0 );

    जइ ( !table->keep_alive )
      ft_mem_table_free( table, block );

    table->alloc_गणनम्--;

    ft_debug_file_   = "<unknown>";
    ft_debug_lineno_ = 0;
  }


  स्थिर FT_Pointer
  ft_mem_debug_realloc( FT_Memory   memory,
                        FT_Long     cur_size,
                        FT_Long     new_size,
                        FT_Pointer  block )
  {
    FT_MemTable  table = (FT_MemTable)memory->user;
    FT_Mem_ग्रन्थिः   ग्रन्थिः, *pnode;
    FT_Pointer   new_block;
    FT_Long      delta;

    ध्रुव वर्ण*  file_name = FT_FILENAME( ft_debug_file_ );
    FT_Long      पङ्क्तिः_no   = ft_debug_lineno_;


    /* unlikely, but possible */
    जइ ( new_size == cur_size )
      प्रत्यागम block;

    /* the following is valid according to ANSI C */
#जइ 0
    जइ ( !block || !cur_size )
      ft_mem_debug_panic( "trying to reallocate NULL in (%s:%ld)",
                          file_name, पङ्क्तिः_no );
#endif

    /* while the following is allowed in ANSI C also, we abort since */
    /* such case should be handled by FreeType.                      */
    जइ ( new_size <= 0 )
      ft_mem_debug_panic(
        "trying to reallocate %p to size 0 (current is %ld) in (%s:%ld)",
        block, cur_size, file_name, पङ्क्तिः_no );

    /* check `cur_size' value */
    pnode = ft_mem_table_get_nodep( table, (FT_Byte*)block );
    ग्रन्थिः  = *pnode;
    जइ ( !ग्रन्थिः )
      ft_mem_debug_panic(
        "trying to reallocate unknown block at %p in (%s:%ld)",
        block, file_name, पङ्क्तिः_no );

    जइ ( ग्रन्थिः->size <= 0 )
      ft_mem_debug_panic(
        "trying to reallocate freed block at %p in (%s:%ld)",
        block, file_name, पङ्क्तिः_no );

    जइ ( ग्रन्थिः->size != cur_size )
      ft_mem_debug_panic( "invalid ft_realloc request for %p. cur_size is "
                          "%ld instead of %ld in (%s:%ld)",
                          block, cur_size, ग्रन्थिः->size, file_name, पङ्क्तिः_no );

    /* return NULL if the maximum number of allocations was reached */
    जइ ( table->bound_गणनम्                           &&
         table->alloc_गणनम् >= table->alloc_गणनम्_max )
      प्रत्यागम रिक्त;

    delta = new_size - cur_size;

    /* return NULL if this allocation would overflow the maximum heap size */
    जइ ( delta > 0                                             &&
         table->bound_total                                    &&
         table->alloc_current + delta > table->alloc_total_max )
      प्रत्यागम रिक्त;

    new_block = (FT_Pointer)ft_mem_table_alloc( table, new_size );
    जइ ( !new_block )
      प्रत्यागम रिक्त;

    ft_mem_table_set( table, (FT_Byte*)new_block, new_size, delta );

    ft_memcpy( new_block, block, cur_size < new_size ? (size_t)cur_size
                                                     : (size_t)new_size );

    ft_mem_table_remove( table, (FT_Byte*)block, delta );

    ft_debug_file_   = "<unknown>";
    ft_debug_lineno_ = 0;

    जइ ( !table->keep_alive )
      ft_mem_table_free( table, block );

    प्रत्यागम new_block;
  }


  बाह्य शून्य
  ft_mem_debug_init( FT_Memory  memory )
  {
    FT_MemTable  table;


    जइ ( !ft_getenv( "FT2_DEBUG_MEMORY" ) )
      प्रत्यागम;

    table = (FT_MemTable)memory->alloc( memory, परिमाण ( *table ) );

    जइ ( table )
    {
      FT_ZERO( table );

      table->memory      = memory;
      table->memory_user = memory->user;
      table->alloc       = memory->alloc;
      table->realloc     = memory->realloc;
      table->मुक्त        = memory->मुक्त;

      ft_mem_table_resize( table );

      जइ ( table->size )
      {
        ध्रुव वर्ण*  p;


        memory->user    = table;
        memory->alloc   = ft_mem_debug_alloc;
        memory->realloc = ft_mem_debug_realloc;
        memory->मुक्त    = ft_mem_debug_free;

        p = ft_getenv( "FT2_ALLOC_TOTAL_MAX" );
        जइ ( p )
        {
          FT_Long  total_max = ft_strtol( p, रिक्त, 10 );


          जइ ( total_max > 0 )
          {
            table->bound_total     = 1;
            table->alloc_total_max = total_max;
          }
        }

        p = ft_getenv( "FT2_ALLOC_COUNT_MAX" );
        जइ ( p )
        {
          FT_Long  total_गणनम् = ft_strtol( p, रिक्त, 10 );


          जइ ( total_गणनम् > 0 )
          {
            table->bound_गणनम्     = 1;
            table->alloc_गणनम्_max = total_गणनम्;
          }
        }

        p = ft_getenv( "FT2_KEEP_ALIVE" );
        जइ ( p )
        {
          FT_Long  keep_alive = ft_strtol( p, रिक्त, 10 );


          जइ ( keep_alive > 0 )
            table->keep_alive = 1;
        }
      }
      अण्णहा
        memory->मुक्त( memory, table );
    }
  }


  बाह्य शून्य
  ft_mem_debug_done( FT_Memory  memory )
  {
    जइ ( memory->मुक्त == ft_mem_debug_free )
    {
      FT_MemTable  table = (FT_MemTable)memory->user;


      FT_DumpMemory( memory );

      ft_mem_table_destroy( table );

      memory->मुक्त    = table->मुक्त;
      memory->realloc = table->realloc;
      memory->alloc   = table->alloc;
      memory->user    = table->memory_user;

      memory->मुक्त( memory, table );
    }
  }


  FT_COMPARE_DEF( पूर्णाङ्क )
  ft_mem_source_compare( ध्रुव शून्य*  p1,
                         ध्रुव शून्य*  p2 )
  {
    FT_MemSource  s1 = *(FT_MemSource*)p1;
    FT_MemSource  s2 = *(FT_MemSource*)p2;


    जइ ( s2->max_size > s1->max_size )
      प्रत्यागम 1;
    अण्णहा जइ ( s2->max_size < s1->max_size )
      प्रत्यागम -1;
    अण्णहा
      प्रत्यागम 0;
  }


  बाह्य शून्य
  FT_DumpMemory( FT_Memory  memory )
  {
    जइ ( memory->मुक्त == ft_mem_debug_free )
    {
      FT_MemTable    table = (FT_MemTable)memory->user;
      FT_MemSource*  bucket = table->sources;
      FT_MemSource*  limit  = bucket + FT_MEM_SOURCE_BUCKETS;
      FT_MemSource*  sources;
      FT_Int         nn, गणनम्;
      ध्रुव वर्ण*    fmt;


      गणनम् = 0;
      प्रति ( ; bucket < limit; bucket++ )
      {
        FT_MemSource  source = *bucket;


        प्रति ( ; source; source = source->link )
          गणनम्++;
      }

      sources = (FT_MemSource*)
                  ft_mem_table_alloc(
                    table, गणनम् * (FT_Long)परिमाण ( *sources ) );

      गणनम् = 0;
      प्रति ( bucket = table->sources; bucket < limit; bucket++ )
      {
        FT_MemSource  source = *bucket;


        प्रति ( ; source; source = source->link )
          sources[गणनम्++] = source;
      }

      ft_qsort( sources,
                (size_t)गणनम्,
                परिमाण ( *sources ),
                ft_mem_source_compare );

      printf( "FreeType Memory Dump: "
              "current=%ld max=%ld total=%ld count=%ld\n",
              table->alloc_current, table->alloc_max,
              table->alloc_total, table->alloc_गणनम् );
      printf( " block  block    sizes    sizes    sizes   source\n" );
      printf( " count   high      sum  highsum      max   location\n" );
      printf( "-------------------------------------------------\n" );

      fmt = "%6ld %6ld %8ld %8ld %8ld %s:%d\n";

      प्रति ( nn = 0; nn < गणनम्; nn++ )
      {
        FT_MemSource  source = sources[nn];


        printf( fmt,
                source->cur_blocks, source->max_blocks,
                source->cur_size, source->max_size, source->cur_max,
                FT_FILENAME( source->file_name ),
                source->पङ्क्तिः_no );
      }
      printf( "------------------------------------------------\n" );

      ft_mem_table_free( table, sources );
    }
  }

#अण्णहा  /* !FT_DEBUG_MEMORY */

  /* ANSI C doesn't like empty source files */
  प्रकारनिर्देश पूर्णाङ्क  debug_mem_dummy_;

#endif /* !FT_DEBUG_MEMORY */


/* END */
