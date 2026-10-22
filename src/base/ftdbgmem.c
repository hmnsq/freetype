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
  प्रकारनिर्देश संरचना FT_Mem_गाठ_Rec*    FT_Mem_गाठ;
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
    long          ओळ_no;

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
  प्रकारनिर्देश संरचना  FT_Mem_गाठ_Rec
  {
    FT_Byte*      address;
    FT_Long       size;     /* < 0 if the block was freed */

    FT_MemSource  source;

#ifdef KEEPALIVE
    ध्रुव वर्ण*   free_file_name;
    FT_Long       free_ओळ_no;
#endif

    FT_Mem_गाठ    link;

  } FT_Mem_गाठ_Rec_2;


  /*
   * The global structure, containing compound statistics and all hash
   * tables.
   */
  प्रकारनिर्देश संरचना  FT_MemTableRec_
  {
    FT_Long          size;
    FT_Long          nodes;
    FT_Mem_गाठ*      buckets;

    FT_Long          alloc_total;
    FT_Long          alloc_current;
    FT_Long          alloc_max;
    FT_Long          alloc_मोजणी;

    FT_Bool          bound_total;
    FT_Long          alloc_total_max;

    FT_Bool          bound_मोजणी;
    FT_Long          alloc_मोजणी_max;

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
  ft_mem_closest_मूळसंख्या( FT_Long  अंक )
  {
    size_t  i;


    प्रति ( i = 0;
          i < परिमाण ( ft_mem_primes ) / परिमाण ( ft_mem_primes[0] ); i++ )
      यदि ( ft_mem_primes[i] > अंक )
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


    new_size = ft_mem_closest_मूळसंख्या( table->nodes );
    यदि ( new_size != table->size )
    {
      FT_Mem_गाठ*  new_buckets;
      FT_Long      i;


      new_buckets = (FT_Mem_गाठ *)
                      ft_mem_table_alloc(
                        table,
                        new_size * (FT_Long)परिमाण ( FT_Mem_गाठ ) );
      यदि ( !new_buckets )
        प्रत्यागम;

      FT_ARRAY_ZERO( new_buckets, new_size );

      प्रति ( i = 0; i < table->size; i++ )
      {
        FT_Mem_गाठ  गाठ, next, *pnode;
        FT_PtrDist  hash;


        गाठ = table->buckets[i];
        यावत् ( गाठ )
        {
          next  = गाठ->link;
          hash  = FT_MEM_VAL( गाठ->address ) % (FT_PtrDist)new_size;
          pnode = new_buckets + hash;

          गाठ->link = pnode[0];
          pnode[0]   = गाठ;

          गाठ = next;
        }
      }

      यदि ( table->buckets )
        ft_mem_table_free( table, table->buckets );

      table->buckets = new_buckets;
      table->size    = new_size;
    }
  }


  स्थिर शून्य
  ft_mem_table_destroy( FT_MemTable  table )
  {
    FT_Long  i;
    FT_Long  leak_मोजणी = 0;
    FT_Long  leaks      = 0;


    /* remove all blocks from the table, revealing leaked ones */
    प्रति ( i = 0; i < table->size; i++ )
    {
      FT_Mem_गाठ  *pnode = table->buckets + i, next, गाठ = *pnode;


      यावत् ( गाठ )
      {
        next       = गाठ->link;
        गाठ->link = रिक्त;

        यदि ( गाठ->size > 0 )
        {
          printf(
            "leaked memory block at address %p, size %8ld in (%s:%ld)\n",
            (शून्य*)गाठ->address,
            गाठ->size,
            FT_FILENAME( गाठ->source->file_name ),
            गाठ->source->ओळ_no );

          leak_मोजणी++;
          leaks += गाठ->size;

          ft_mem_table_free( table, गाठ->address );
        }

        गाठ->address = रिक्त;
        गाठ->size    = 0;

        ft_mem_table_free( table, गाठ );
        गाठ = next;
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

    यदि ( leak_मोजणी > 0 )
      ft_mem_debug_panic(
        "FreeType: %ld bytes of memory leaked in %ld blocks\n",
        leaks, leak_मोजणी );

    printf( "FreeType: no memory leaks detected\n" );
  }


  स्थिर FT_Mem_गाठ*
  ft_mem_table_get_nodep( FT_MemTable  table,
                          FT_Byte*     address )
  {
    FT_PtrDist   hash;
    FT_Mem_गाठ  *pnode, गाठ;


    hash  = FT_MEM_VAL( address );
    pnode = table->buckets + ( hash % (FT_PtrDist)table->size );

    प्रति (;;)
    {
      गाठ = pnode[0];
      यदि ( !गाठ )
        भङ्ग;

      यदि ( गाठ->address == address )
        भङ्ग;

      pnode = &गाठ->link;
    }
    प्रत्यागम pnode;
  }


  स्थिर FT_MemSource
  ft_mem_table_get_source( FT_MemTable  table )
  {
    FT_UInt32     hash;
    FT_MemSource  गाठ, *pnode;


    /* cast to FT_PtrDist first since void* can be larger */
    /* than FT_UInt32 and GCC 4.1.1 emits a warning       */
    hash  = (FT_UInt32)(FT_PtrDist)(शून्य*)ft_debug_file_ +
              (FT_UInt32)( 5 * ft_debug_lineno_ );
    pnode = &table->sources[hash % FT_MEM_SOURCE_BUCKETS];

    प्रति (;;)
    {
      गाठ = *pnode;
      यदि ( !गाठ )
        भङ्ग;

      यदि ( गाठ->file_name == ft_debug_file_   &&
           गाठ->ओळ_no   == ft_debug_lineno_ )
        गच्छ Exit;

      pnode = &गाठ->link;
    }

    गाठ = (FT_MemSource)ft_mem_table_alloc( table, परिमाण ( *गाठ ) );
    यदि ( !गाठ )
      ft_mem_debug_panic(
        "not enough memory to perform memory debugging\n" );

    गाठ->file_name = ft_debug_file_;
    गाठ->ओळ_no   = ft_debug_lineno_;

    गाठ->cur_blocks = 0;
    गाठ->max_blocks = 0;
    गाठ->all_blocks = 0;

    गाठ->cur_size = 0;
    गाठ->max_size = 0;
    गाठ->all_size = 0;

    गाठ->cur_max = 0;

    गाठ->link = रिक्त;
    गाठ->hash = hash;
    *pnode     = गाठ;

  Exit:
    प्रत्यागम गाठ;
  }


  स्थिर शून्य
  ft_mem_table_set( FT_MemTable  table,
                    FT_Byte*     address,
                    FT_Long      size,
                    FT_Long      delta )
  {
    FT_Mem_गाठ  *pnode, गाठ;


    यदि ( table )
    {
      FT_MemSource  source;


      pnode = ft_mem_table_get_nodep( table, address );
      गाठ  = *pnode;
      यदि ( गाठ )
      {
        यदि ( गाठ->size < 0 )
        {
          /* This block was already freed.  Our memory is now completely */
          /* corrupted!                                                  */
          /* This can only happen in keep-alive mode.                    */
          ft_mem_debug_panic(
            "memory heap corrupted (allocating freed block)" );
        }
        अन्यथा
        {
          /* This block was already allocated.  This means that our memory */
          /* is also corrupted!                                            */
          ft_mem_debug_panic(
            "memory heap corrupted (re-allocating allocated block at"
            " %p, of size %ld)\n"
            "org=%s:%d new=%s:%d\n",
            गाठ->address, गाठ->size,
            FT_FILENAME( गाठ->source->file_name ), गाठ->source->ओळ_no,
            FT_FILENAME( ft_debug_file_ ), ft_debug_lineno_ );
        }
      }

      /* we need to create a new node in this table */
      गाठ = (FT_Mem_गाठ)ft_mem_table_alloc( table, परिमाण ( *गाठ ) );
      यदि ( !गाठ )
        ft_mem_debug_panic( "not enough memory to run memory tests" );

      गाठ->address = address;
      गाठ->size    = size;
      गाठ->source  = source = ft_mem_table_get_source( table );

      यदि ( delta == 0 )
      {
        /* this is an allocation */
        source->all_blocks++;
        source->cur_blocks++;
        यदि ( source->cur_blocks > source->max_blocks )
          source->max_blocks = source->cur_blocks;
      }

      यदि ( size > source->cur_max )
        source->cur_max = size;

      यदि ( delta != 0 )
      {
        /* we are growing or shrinking a reallocated block */
        source->cur_size     += delta;
        table->alloc_current += delta;
      }
      अन्यथा
      {
        /* we are allocating a new block */
        source->cur_size     += size;
        table->alloc_current += size;
      }

      source->all_size += size;

      यदि ( source->cur_size > source->max_size )
        source->max_size = source->cur_size;

      गाठ->free_file_name = रिक्त;
      गाठ->free_ओळ_no   = 0;

      गाठ->link = pnode[0];

      pnode[0] = गाठ;
      table->nodes++;

      table->alloc_total += size;

      यदि ( table->alloc_current > table->alloc_max )
        table->alloc_max = table->alloc_current;

      यदि ( table->nodes * 3 < table->size  ||
           table->size  * 3 < table->nodes )
        ft_mem_table_resize( table );
    }
  }


  स्थिर शून्य
  ft_mem_table_remove( FT_MemTable  table,
                       FT_Byte*     address,
                       FT_Long      delta )
  {
    यदि ( table )
    {
      FT_Mem_गाठ  *pnode, गाठ;


      pnode = ft_mem_table_get_nodep( table, address );
      गाठ  = *pnode;
      यदि ( गाठ )
      {
        FT_MemSource  source;


        यदि ( गाठ->size < 0 )
          ft_mem_debug_panic(
            "freeing memory block at %p more than once\n"
            "  at (%s:%ld)!\n"
            "  Block was allocated at (%s:%ld)\n"
            "  and released at (%s:%ld).",
            address,
            FT_FILENAME( ft_debug_file_ ), ft_debug_lineno_,
            FT_FILENAME( गाठ->source->file_name ), गाठ->source->ओळ_no,
            FT_FILENAME( गाठ->free_file_name ), गाठ->free_ओळ_no );

        /* scramble the node's content for additional safety */
        FT_MEM_SET( address, 0xF3, गाठ->size );

        यदि ( delta == 0 )
        {
          source = गाठ->source;

          source->cur_blocks--;
          source->cur_size -= गाठ->size;

          table->alloc_current -= गाठ->size;
        }

        यदि ( table->keep_alive )
        {
          /* we simply invert the node's size to indicate that the node */
          /* was freed.                                                 */
          गाठ->size           = -गाठ->size;
          गाठ->free_file_name = ft_debug_file_;
          गाठ->free_ओळ_no   = ft_debug_lineno_;
        }
        अन्यथा
        {
          table->nodes--;

          *pnode = गाठ->link;

          गाठ->size   = 0;
          गाठ->source = रिक्त;

          ft_mem_table_free( table, गाठ );

          यदि ( table->nodes * 3 < table->size  ||
               table->size  * 3 < table->nodes )
            ft_mem_table_resize( table );
        }
      }
      अन्यथा
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


    यदि ( size <= 0 )
      ft_mem_debug_panic( "negative block size allocation (%ld)", size );

    /* return NULL if the maximum number of allocations was reached */
    यदि ( table->bound_मोजणी                           &&
         table->alloc_मोजणी >= table->alloc_मोजणी_max )
      प्रत्यागम रिक्त;

    /* return NULL if this allocation would overflow the maximum heap size */
    यदि ( table->bound_total                                   &&
         table->alloc_total_max - table->alloc_current > size )
      प्रत्यागम रिक्त;

    block = (FT_Byte *)ft_mem_table_alloc( table, size );
    यदि ( block )
    {
      ft_mem_table_set( table, block, size, 0 );

      table->alloc_मोजणी++;
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


    यदि ( !block )
      ft_mem_debug_panic( "trying to free NULL in (%s:%ld)",
                          FT_FILENAME( ft_debug_file_ ),
                          ft_debug_lineno_ );

    ft_mem_table_remove( table, (FT_Byte*)block, 0 );

    यदि ( !table->keep_alive )
      ft_mem_table_free( table, block );

    table->alloc_मोजणी--;

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
    FT_Mem_गाठ   गाठ, *pnode;
    FT_Pointer   new_block;
    FT_Long      delta;

    ध्रुव वर्ण*  file_name = FT_FILENAME( ft_debug_file_ );
    FT_Long      ओळ_no   = ft_debug_lineno_;


    /* unlikely, but possible */
    यदि ( new_size == cur_size )
      प्रत्यागम block;

    /* the following is valid according to ANSI C */
#यदि 0
    यदि ( !block || !cur_size )
      ft_mem_debug_panic( "trying to reallocate NULL in (%s:%ld)",
                          file_name, ओळ_no );
#endif

    /* while the following is allowed in ANSI C also, we abort since */
    /* such case should be handled by FreeType.                      */
    यदि ( new_size <= 0 )
      ft_mem_debug_panic(
        "trying to reallocate %p to size 0 (current is %ld) in (%s:%ld)",
        block, cur_size, file_name, ओळ_no );

    /* check `cur_size' value */
    pnode = ft_mem_table_get_nodep( table, (FT_Byte*)block );
    गाठ  = *pnode;
    यदि ( !गाठ )
      ft_mem_debug_panic(
        "trying to reallocate unknown block at %p in (%s:%ld)",
        block, file_name, ओळ_no );

    यदि ( गाठ->size <= 0 )
      ft_mem_debug_panic(
        "trying to reallocate freed block at %p in (%s:%ld)",
        block, file_name, ओळ_no );

    यदि ( गाठ->size != cur_size )
      ft_mem_debug_panic( "invalid ft_realloc request for %p. cur_size is "
                          "%ld instead of %ld in (%s:%ld)",
                          block, cur_size, गाठ->size, file_name, ओळ_no );

    /* return NULL if the maximum number of allocations was reached */
    यदि ( table->bound_मोजणी                           &&
         table->alloc_मोजणी >= table->alloc_मोजणी_max )
      प्रत्यागम रिक्त;

    delta = new_size - cur_size;

    /* return NULL if this allocation would overflow the maximum heap size */
    यदि ( delta > 0                                             &&
         table->bound_total                                    &&
         table->alloc_current + delta > table->alloc_total_max )
      प्रत्यागम रिक्त;

    new_block = (FT_Pointer)ft_mem_table_alloc( table, new_size );
    यदि ( !new_block )
      प्रत्यागम रिक्त;

    ft_mem_table_set( table, (FT_Byte*)new_block, new_size, delta );

    ft_memcpy( new_block, block, cur_size < new_size ? (size_t)cur_size
                                                     : (size_t)new_size );

    ft_mem_table_remove( table, (FT_Byte*)block, delta );

    ft_debug_file_   = "<unknown>";
    ft_debug_lineno_ = 0;

    यदि ( !table->keep_alive )
      ft_mem_table_free( table, block );

    प्रत्यागम new_block;
  }


  बाह्य शून्य
  ft_mem_debug_init( FT_Memory  memory )
  {
    FT_MemTable  table;


    यदि ( !ft_getenv( "FT2_DEBUG_MEMORY" ) )
      प्रत्यागम;

    table = (FT_MemTable)memory->alloc( memory, परिमाण ( *table ) );

    यदि ( table )
    {
      FT_ZERO( table );

      table->memory      = memory;
      table->memory_user = memory->user;
      table->alloc       = memory->alloc;
      table->realloc     = memory->realloc;
      table->मुक्त        = memory->मुक्त;

      ft_mem_table_resize( table );

      यदि ( table->size )
      {
        ध्रुव वर्ण*  p;


        memory->user    = table;
        memory->alloc   = ft_mem_debug_alloc;
        memory->realloc = ft_mem_debug_realloc;
        memory->मुक्त    = ft_mem_debug_free;

        p = ft_getenv( "FT2_ALLOC_TOTAL_MAX" );
        यदि ( p )
        {
          FT_Long  total_max = ft_strtol( p, रिक्त, 10 );


          यदि ( total_max > 0 )
          {
            table->bound_total     = 1;
            table->alloc_total_max = total_max;
          }
        }

        p = ft_getenv( "FT2_ALLOC_COUNT_MAX" );
        यदि ( p )
        {
          FT_Long  total_मोजणी = ft_strtol( p, रिक्त, 10 );


          यदि ( total_मोजणी > 0 )
          {
            table->bound_मोजणी     = 1;
            table->alloc_मोजणी_max = total_मोजणी;
          }
        }

        p = ft_getenv( "FT2_KEEP_ALIVE" );
        यदि ( p )
        {
          FT_Long  keep_alive = ft_strtol( p, रिक्त, 10 );


          यदि ( keep_alive > 0 )
            table->keep_alive = 1;
        }
      }
      अन्यथा
        memory->मुक्त( memory, table );
    }
  }


  बाह्य शून्य
  ft_mem_debug_done( FT_Memory  memory )
  {
    यदि ( memory->मुक्त == ft_mem_debug_free )
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


    यदि ( s2->max_size > s1->max_size )
      प्रत्यागम 1;
    अन्यथा यदि ( s2->max_size < s1->max_size )
      प्रत्यागम -1;
    अन्यथा
      प्रत्यागम 0;
  }


  बाह्य शून्य
  FT_DumpMemory( FT_Memory  memory )
  {
    यदि ( memory->मुक्त == ft_mem_debug_free )
    {
      FT_MemTable    table = (FT_MemTable)memory->user;
      FT_MemSource*  bucket = table->sources;
      FT_MemSource*  limit  = bucket + FT_MEM_SOURCE_BUCKETS;
      FT_MemSource*  sources;
      FT_Int         nn, मोजणी;
      ध्रुव वर्ण*    fmt;


      मोजणी = 0;
      प्रति ( ; bucket < limit; bucket++ )
      {
        FT_MemSource  source = *bucket;


        प्रति ( ; source; source = source->link )
          मोजणी++;
      }

      sources = (FT_MemSource*)
                  ft_mem_table_alloc(
                    table, मोजणी * (FT_Long)परिमाण ( *sources ) );

      मोजणी = 0;
      प्रति ( bucket = table->sources; bucket < limit; bucket++ )
      {
        FT_MemSource  source = *bucket;


        प्रति ( ; source; source = source->link )
          sources[मोजणी++] = source;
      }

      ft_qsort( sources,
                (size_t)मोजणी,
                परिमाण ( *sources ),
                ft_mem_source_compare );

      printf( "FreeType Memory Dump: "
              "current=%ld max=%ld total=%ld count=%ld\n",
              table->alloc_current, table->alloc_max,
              table->alloc_total, table->alloc_मोजणी );
      printf( " block  block    sizes    sizes    sizes   source\n" );
      printf( " count   high      sum  highsum      max   location\n" );
      printf( "-------------------------------------------------\n" );

      fmt = "%6ld %6ld %8ld %8ld %8ld %s:%d\n";

      प्रति ( nn = 0; nn < मोजणी; nn++ )
      {
        FT_MemSource  source = sources[nn];


        printf( fmt,
                source->cur_blocks, source->max_blocks,
                source->cur_size, source->max_size, source->cur_max,
                FT_FILENAME( source->file_name ),
                source->ओळ_no );
      }
      printf( "------------------------------------------------\n" );

      ft_mem_table_free( table, sources );
    }
  }

#अन्यथा  /* !FT_DEBUG_MEMORY */

  /* ANSI C doesn't like empty source files */
  प्रकारनिर्देश पूर्णाङ्क  debug_mem_dummy_;

#endif /* !FT_DEBUG_MEMORY */


/* END */
