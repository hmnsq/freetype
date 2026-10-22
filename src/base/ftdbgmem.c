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

  FT_BASE_DEF( स्थिरांक अक्षर* )  ft_debug_file_   = रिक्त;
  FT_BASE_DEF( दीर्घ )         ft_debug_lineno_ = 0;

  बाह्य शून्य
  FT_DumpMemory( FT_Memory  memory );


  प्रकारव्याख्या संरचना FT_MemSourceRec_*  FT_MemSource;
  प्रकारव्याख्या संरचना FT_Mem_गाठ_Rec*    FT_Mem_गाठ;
  प्रकारव्याख्या संरचना FT_MemTableRec_*   FT_MemTable;


#define FT_MEM_VAL( addr )  ( (FT_PtrDist)(FT_Pointer)( addr ) )

  /*
   * This structure holds statistics for a single allocation/release
   * site.  This is useful to know where memory operations happen the
   * most.
   */
  प्रकारव्याख्या संरचना  FT_MemSourceRec_
  {
    स्थिरांक अक्षर*   file_name;
    दीर्घ          ओळ_no;

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
  प्रकारव्याख्या संरचना  FT_Mem_गाठ_Rec
  {
    FT_Byte*      address;
    FT_Long       size;     /* < 0 if the block was freed */

    FT_MemSource  source;

#ifdef KEEPALIVE
    स्थिरांक अक्षर*   free_file_name;
    FT_Long       free_ओळ_no;
#endif

    FT_Mem_गाठ    link;

  } FT_Mem_गाठ_Rec_2;


  /*
   * The global structure, containing compound statistics and all hash
   * tables.
   */
  प्रकारव्याख्या संरचना  FT_MemTableRec_
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
    FT_Realloc_Func  पुनः_स्मृति;

  } FT_MemTableRec;


#define FT_MEM_SIZE_MAX  13845163

#define FT_FILENAME( x )  ( (x) ? (x) : "unknown file" )


  /*
   * Prime numbers are ugly to handle.  It would be better to implement
   * L-Hashing, which is 10% faster and doesn't require divisions.
   */
  स्थिर स्थिरांक FT_Int  ft_mem_primes[] =
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
    माप_प्रकार  i;


    साठी ( i = 0;
          i < आकार ( ft_mem_primes ) / आकार ( ft_mem_primes[0] ); i++ )
      जर ( ft_mem_primes[i] > अंक )
        परत ft_mem_primes[i];

    परत FT_MEM_SIZE_MAX;
  }


  स्थिर शून्य
  ft_mem_debug_panic( स्थिरांक अक्षर*  fmt,
                      ... )
  {
    बहु_सूची  ap;


    म_लिखो( "FreeType.Debug: " );

    बहु_शुरू( ap, fmt );
    भ_लिखो( fmt, ap );
    बहु_पूर्ण( ap );

    म_लिखो( "\n" );
    निकास( निकास_त्रुटि );
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

    परत block;
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
    जर ( new_size != table->size )
    {
      FT_Mem_गाठ*  new_buckets;
      FT_Long      i;


      new_buckets = (FT_Mem_गाठ *)
                      ft_mem_table_alloc(
                        table,
                        new_size * (FT_Long)आकार ( FT_Mem_गाठ ) );
      जर ( !new_buckets )
        परत;

      FT_ARRAY_ZERO( new_buckets, new_size );

      साठी ( i = 0; i < table->size; i++ )
      {
        FT_Mem_गाठ  गाठ, next, *pnode;
        FT_PtrDist  hash;


        गाठ = table->buckets[i];
        पर्यंत ( गाठ )
        {
          next  = गाठ->link;
          hash  = FT_MEM_VAL( गाठ->address ) % (FT_PtrDist)new_size;
          pnode = new_buckets + hash;

          गाठ->link = pnode[0];
          pnode[0]   = गाठ;

          गाठ = next;
        }
      }

      जर ( table->buckets )
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
    साठी ( i = 0; i < table->size; i++ )
    {
      FT_Mem_गाठ  *pnode = table->buckets + i, next, गाठ = *pnode;


      पर्यंत ( गाठ )
      {
        next       = गाठ->link;
        गाठ->link = रिक्त;

        जर ( गाठ->size > 0 )
        {
          म_लिखो(
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
    साठी ( i = 0; i < FT_MEM_SOURCE_BUCKETS; i++ )
    {
      FT_MemSource  source, next;


      साठी ( source = table->sources[i]; source != रिक्त; source = next )
      {
        next = source->link;
        ft_mem_table_free( table, source );
      }

      table->sources[i] = रिक्त;
    }

    म_लिखो( "FreeType: total memory allocations = %ld\n",
            table->alloc_total );
    म_लिखो( "FreeType: maximum memory footprint = %ld\n",
            table->alloc_max );

    जर ( leak_मोजणी > 0 )
      ft_mem_debug_panic(
        "FreeType: %ld bytes of memory leaked in %ld blocks\n",
        leaks, leak_मोजणी );

    म_लिखो( "FreeType: no memory leaks detected\n" );
  }


  स्थिर FT_Mem_गाठ*
  ft_mem_table_get_nodep( FT_MemTable  table,
                          FT_Byte*     address )
  {
    FT_PtrDist   hash;
    FT_Mem_गाठ  *pnode, गाठ;


    hash  = FT_MEM_VAL( address );
    pnode = table->buckets + ( hash % (FT_PtrDist)table->size );

    साठी (;;)
    {
      गाठ = pnode[0];
      जर ( !गाठ )
        थांबा;

      जर ( गाठ->address == address )
        थांबा;

      pnode = &गाठ->link;
    }
    परत pnode;
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

    साठी (;;)
    {
      गाठ = *pnode;
      जर ( !गाठ )
        थांबा;

      जर ( गाठ->file_name == ft_debug_file_   &&
           गाठ->ओळ_no   == ft_debug_lineno_ )
        जा Exit;

      pnode = &गाठ->link;
    }

    गाठ = (FT_MemSource)ft_mem_table_alloc( table, आकार ( *गाठ ) );
    जर ( !गाठ )
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
    परत गाठ;
  }


  स्थिर शून्य
  ft_mem_table_set( FT_MemTable  table,
                    FT_Byte*     address,
                    FT_Long      size,
                    FT_Long      delta )
  {
    FT_Mem_गाठ  *pnode, गाठ;


    जर ( table )
    {
      FT_MemSource  source;


      pnode = ft_mem_table_get_nodep( table, address );
      गाठ  = *pnode;
      जर ( गाठ )
      {
        जर ( गाठ->size < 0 )
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
      गाठ = (FT_Mem_गाठ)ft_mem_table_alloc( table, आकार ( *गाठ ) );
      जर ( !गाठ )
        ft_mem_debug_panic( "not enough memory to run memory tests" );

      गाठ->address = address;
      गाठ->size    = size;
      गाठ->source  = source = ft_mem_table_get_source( table );

      जर ( delta == 0 )
      {
        /* this is an allocation */
        source->all_blocks++;
        source->cur_blocks++;
        जर ( source->cur_blocks > source->max_blocks )
          source->max_blocks = source->cur_blocks;
      }

      जर ( size > source->cur_max )
        source->cur_max = size;

      जर ( delta != 0 )
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

      जर ( source->cur_size > source->max_size )
        source->max_size = source->cur_size;

      गाठ->free_file_name = रिक्त;
      गाठ->free_ओळ_no   = 0;

      गाठ->link = pnode[0];

      pnode[0] = गाठ;
      table->nodes++;

      table->alloc_total += size;

      जर ( table->alloc_current > table->alloc_max )
        table->alloc_max = table->alloc_current;

      जर ( table->nodes * 3 < table->size  ||
           table->size  * 3 < table->nodes )
        ft_mem_table_resize( table );
    }
  }


  स्थिर शून्य
  ft_mem_table_remove( FT_MemTable  table,
                       FT_Byte*     address,
                       FT_Long      delta )
  {
    जर ( table )
    {
      FT_Mem_गाठ  *pnode, गाठ;


      pnode = ft_mem_table_get_nodep( table, address );
      गाठ  = *pnode;
      जर ( गाठ )
      {
        FT_MemSource  source;


        जर ( गाठ->size < 0 )
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

        जर ( delta == 0 )
        {
          source = गाठ->source;

          source->cur_blocks--;
          source->cur_size -= गाठ->size;

          table->alloc_current -= गाठ->size;
        }

        जर ( table->keep_alive )
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

          जर ( table->nodes * 3 < table->size  ||
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


    जर ( size <= 0 )
      ft_mem_debug_panic( "negative block size allocation (%ld)", size );

    /* return NULL if the maximum number of allocations was reached */
    जर ( table->bound_मोजणी                           &&
         table->alloc_मोजणी >= table->alloc_मोजणी_max )
      परत रिक्त;

    /* return NULL if this allocation would overflow the maximum heap size */
    जर ( table->bound_total                                   &&
         table->alloc_total_max - table->alloc_current > size )
      परत रिक्त;

    block = (FT_Byte *)ft_mem_table_alloc( table, size );
    जर ( block )
    {
      ft_mem_table_set( table, block, size, 0 );

      table->alloc_मोजणी++;
    }

    ft_debug_file_   = "<unknown>";
    ft_debug_lineno_ = 0;

    परत (FT_Pointer)block;
  }


  स्थिर शून्य
  ft_mem_debug_free( FT_Memory   memory,
                     FT_Pointer  block )
  {
    FT_MemTable  table = (FT_MemTable)memory->user;


    जर ( !block )
      ft_mem_debug_panic( "trying to free NULL in (%s:%ld)",
                          FT_FILENAME( ft_debug_file_ ),
                          ft_debug_lineno_ );

    ft_mem_table_remove( table, (FT_Byte*)block, 0 );

    जर ( !table->keep_alive )
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

    स्थिरांक अक्षर*  file_name = FT_FILENAME( ft_debug_file_ );
    FT_Long      ओळ_no   = ft_debug_lineno_;


    /* unlikely, but possible */
    जर ( new_size == cur_size )
      परत block;

    /* the following is valid according to ANSI C */
#जर 0
    जर ( !block || !cur_size )
      ft_mem_debug_panic( "trying to reallocate NULL in (%s:%ld)",
                          file_name, ओळ_no );
#endif

    /* while the following is allowed in ANSI C also, we abort since */
    /* such case should be handled by FreeType.                      */
    जर ( new_size <= 0 )
      ft_mem_debug_panic(
        "trying to reallocate %p to size 0 (current is %ld) in (%s:%ld)",
        block, cur_size, file_name, ओळ_no );

    /* check `cur_size' value */
    pnode = ft_mem_table_get_nodep( table, (FT_Byte*)block );
    गाठ  = *pnode;
    जर ( !गाठ )
      ft_mem_debug_panic(
        "trying to reallocate unknown block at %p in (%s:%ld)",
        block, file_name, ओळ_no );

    जर ( गाठ->size <= 0 )
      ft_mem_debug_panic(
        "trying to reallocate freed block at %p in (%s:%ld)",
        block, file_name, ओळ_no );

    जर ( गाठ->size != cur_size )
      ft_mem_debug_panic( "invalid ft_realloc request for %p. cur_size is "
                          "%ld instead of %ld in (%s:%ld)",
                          block, cur_size, गाठ->size, file_name, ओळ_no );

    /* return NULL if the maximum number of allocations was reached */
    जर ( table->bound_मोजणी                           &&
         table->alloc_मोजणी >= table->alloc_मोजणी_max )
      परत रिक्त;

    delta = new_size - cur_size;

    /* return NULL if this allocation would overflow the maximum heap size */
    जर ( delta > 0                                             &&
         table->bound_total                                    &&
         table->alloc_current + delta > table->alloc_total_max )
      परत रिक्त;

    new_block = (FT_Pointer)ft_mem_table_alloc( table, new_size );
    जर ( !new_block )
      परत रिक्त;

    ft_mem_table_set( table, (FT_Byte*)new_block, new_size, delta );

    ft_memcpy( new_block, block, cur_size < new_size ? (माप_प्रकार)cur_size
                                                     : (माप_प्रकार)new_size );

    ft_mem_table_remove( table, (FT_Byte*)block, delta );

    ft_debug_file_   = "<unknown>";
    ft_debug_lineno_ = 0;

    जर ( !table->keep_alive )
      ft_mem_table_free( table, block );

    परत new_block;
  }


  बाह्य शून्य
  ft_mem_debug_init( FT_Memory  memory )
  {
    FT_MemTable  table;


    जर ( !ft_getenv( "FT2_DEBUG_MEMORY" ) )
      परत;

    table = (FT_MemTable)memory->alloc( memory, आकार ( *table ) );

    जर ( table )
    {
      FT_ZERO( table );

      table->memory      = memory;
      table->memory_user = memory->user;
      table->alloc       = memory->alloc;
      table->पुनः_स्मृति     = memory->पुनः_स्मृति;
      table->मुक्त        = memory->मुक्त;

      ft_mem_table_resize( table );

      जर ( table->size )
      {
        स्थिरांक अक्षर*  p;


        memory->user    = table;
        memory->alloc   = ft_mem_debug_alloc;
        memory->पुनः_स्मृति = ft_mem_debug_realloc;
        memory->मुक्त    = ft_mem_debug_free;

        p = ft_getenv( "FT2_ALLOC_TOTAL_MAX" );
        जर ( p )
        {
          FT_Long  total_max = ft_strtol( p, रिक्त, 10 );


          जर ( total_max > 0 )
          {
            table->bound_total     = 1;
            table->alloc_total_max = total_max;
          }
        }

        p = ft_getenv( "FT2_ALLOC_COUNT_MAX" );
        जर ( p )
        {
          FT_Long  total_मोजणी = ft_strtol( p, रिक्त, 10 );


          जर ( total_मोजणी > 0 )
          {
            table->bound_मोजणी     = 1;
            table->alloc_मोजणी_max = total_मोजणी;
          }
        }

        p = ft_getenv( "FT2_KEEP_ALIVE" );
        जर ( p )
        {
          FT_Long  keep_alive = ft_strtol( p, रिक्त, 10 );


          जर ( keep_alive > 0 )
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
    जर ( memory->मुक्त == ft_mem_debug_free )
    {
      FT_MemTable  table = (FT_MemTable)memory->user;


      FT_DumpMemory( memory );

      ft_mem_table_destroy( table );

      memory->मुक्त    = table->मुक्त;
      memory->पुनः_स्मृति = table->पुनः_स्मृति;
      memory->alloc   = table->alloc;
      memory->user    = table->memory_user;

      memory->मुक्त( memory, table );
    }
  }


  FT_COMPARE_DEF( पूर्णांक )
  ft_mem_source_compare( स्थिरांक शून्य*  p1,
                         स्थिरांक शून्य*  p2 )
  {
    FT_MemSource  s1 = *(FT_MemSource*)p1;
    FT_MemSource  s2 = *(FT_MemSource*)p2;


    जर ( s2->max_size > s1->max_size )
      परत 1;
    अन्यथा जर ( s2->max_size < s1->max_size )
      परत -1;
    अन्यथा
      परत 0;
  }


  बाह्य शून्य
  FT_DumpMemory( FT_Memory  memory )
  {
    जर ( memory->मुक्त == ft_mem_debug_free )
    {
      FT_MemTable    table = (FT_MemTable)memory->user;
      FT_MemSource*  bucket = table->sources;
      FT_MemSource*  limit  = bucket + FT_MEM_SOURCE_BUCKETS;
      FT_MemSource*  sources;
      FT_Int         nn, मोजणी;
      स्थिरांक अक्षर*    fmt;


      मोजणी = 0;
      साठी ( ; bucket < limit; bucket++ )
      {
        FT_MemSource  source = *bucket;


        साठी ( ; source; source = source->link )
          मोजणी++;
      }

      sources = (FT_MemSource*)
                  ft_mem_table_alloc(
                    table, मोजणी * (FT_Long)आकार ( *sources ) );

      मोजणी = 0;
      साठी ( bucket = table->sources; bucket < limit; bucket++ )
      {
        FT_MemSource  source = *bucket;


        साठी ( ; source; source = source->link )
          sources[मोजणी++] = source;
      }

      ft_qsort( sources,
                (माप_प्रकार)मोजणी,
                आकार ( *sources ),
                ft_mem_source_compare );

      म_लिखो( "FreeType Memory Dump: "
              "current=%ld max=%ld total=%ld count=%ld\n",
              table->alloc_current, table->alloc_max,
              table->alloc_total, table->alloc_मोजणी );
      म_लिखो( " block  block    sizes    sizes    sizes   source\n" );
      म_लिखो( " count   high      sum  highsum      max   location\n" );
      म_लिखो( "-------------------------------------------------\n" );

      fmt = "%6ld %6ld %8ld %8ld %8ld %s:%d\n";

      साठी ( nn = 0; nn < मोजणी; nn++ )
      {
        FT_MemSource  source = sources[nn];


        म_लिखो( fmt,
                source->cur_blocks, source->max_blocks,
                source->cur_size, source->max_size, source->cur_max,
                FT_FILENAME( source->file_name ),
                source->ओळ_no );
      }
      म_लिखो( "------------------------------------------------\n" );

      ft_mem_table_free( table, sources );
    }
  }

#अन्यथा  /* !FT_DEBUG_MEMORY */

  /* ANSI C doesn't like empty source files */
  प्रकारव्याख्या पूर्णांक  debug_mem_dummy_;

#endif /* !FT_DEBUG_MEMORY */


/* END */
