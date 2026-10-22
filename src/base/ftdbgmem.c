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


#અન્તર્ભાવ <ft2build.h>
#અન્તર્ભાવ FT_CONFIG_CONFIG_H
#અન્તર્ભાવ <freetype/internal/ftdebug.h>
#અન્તર્ભાવ <freetype/internal/ftmemory.h>
#અન્તર્ભાવ <freetype/ftsystem.h>
#અન્તર્ભાવ <freetype/fterrors.h>
#અન્તર્ભાવ <freetype/fttypes.h>


#ifdef FT_DEBUG_MEMORY

#define  KEEPALIVE /* `Keep alive' means that freed blocks aren't released
                    * to the heap.  This is useful to detect double-frees
                    * or weird heap corruption, but it uses large amounts of
                    * memory, however.
                    */

#અન્તર્ભાવ FT_CONFIG_STANDARD_LIBRARY_H

  FT_BASE_DEF( અચલ અક્ષર* )  ft_debug_file_   = ખાલી;
  FT_BASE_DEF( long )         ft_debug_lineno_ = 0;

  બાહ્ય શૂન્ય
  FT_DumpMemory( FT_Memory  memory );


  પ્રકારવ્યાખ્યા સંરચના FT_MemSourceRec_*  FT_MemSource;
  પ્રકારવ્યાખ્યા સંરચના FT_Mem_ગાંઠ_Rec*    FT_Mem_ગાંઠ;
  પ્રકારવ્યાખ્યા સંરચના FT_MemTableRec_*   FT_MemTable;


#define FT_MEM_VAL( addr )  ( (FT_PtrDist)(FT_Pointer)( addr ) )

  /*
   * This structure holds statistics for a single allocation/release
   * site.  This is useful to know where memory operations happen the
   * most.
   */
  પ્રકારવ્યાખ્યા સંરચના  FT_MemSourceRec_
  {
    અચલ અક્ષર*   file_name;
    long          લીટી_no;

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
  પ્રકારવ્યાખ્યા સંરચના  FT_Mem_ગાંઠ_Rec
  {
    FT_Byte*      address;
    FT_Long       size;     /* < 0 if the block was freed */

    FT_MemSource  source;

#ifdef KEEPALIVE
    અચલ અક્ષર*   free_file_name;
    FT_Long       free_લીટી_no;
#endif

    FT_Mem_ગાંઠ    link;

  } FT_Mem_ગાંઠ_Rec_2;


  /*
   * The global structure, containing compound statistics and all hash
   * tables.
   */
  પ્રકારવ્યાખ્યા સંરચના  FT_MemTableRec_
  {
    FT_Long          size;
    FT_Long          nodes;
    FT_Mem_ગાંઠ*      buckets;

    FT_Long          alloc_total;
    FT_Long          alloc_current;
    FT_Long          alloc_max;
    FT_Long          alloc_ગણતરી;

    FT_Bool          bound_total;
    FT_Long          alloc_total_max;

    FT_Bool          bound_ગણતરી;
    FT_Long          alloc_ગણતરી_max;

    FT_MemSource     sources[FT_MEM_SOURCE_BUCKETS];

    FT_Bool          keep_alive;

    FT_Memory        memory;
    FT_Pointer       memory_user;
    FT_Alloc_Func    alloc;
    FT_Free_Func     મુક્ત;
    FT_Realloc_Func  realloc;

  } FT_MemTableRec;


#define FT_MEM_SIZE_MAX  13845163

#define FT_FILENAME( x )  ( (x) ? (x) : "unknown file" )


  /*
   * Prime numbers are ugly to handle.  It would be better to implement
   * L-Hashing, which is 10% faster and doesn't require divisions.
   */
  સ્થિર અચલ FT_Int  ft_mem_primes[] =
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


  સ્થિર FT_Long
  ft_mem_closest_અવિભાજ્ય( FT_Long  અંક )
  {
    size_t  i;


    માટે ( i = 0;
          i < કદ ( ft_mem_primes ) / કદ ( ft_mem_primes[0] ); i++ )
      જો ( ft_mem_primes[i] > અંક )
        પાછા ft_mem_primes[i];

    પાછા FT_MEM_SIZE_MAX;
  }


  સ્થિર શૂન્ય
  ft_mem_debug_panic( અચલ અક્ષર*  fmt,
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


  સ્થિર FT_Pointer
  ft_mem_table_alloc( FT_MemTable  table,
                      FT_Long      size )
  {
    FT_Memory   memory = table->memory;
    FT_Pointer  block;


    memory->user = table->memory_user;
    block = table->alloc( memory, size );
    memory->user = table;

    પાછા block;
  }


  સ્થિર શૂન્ય
  ft_mem_table_free( FT_MemTable  table,
                     FT_Pointer   block )
  {
    FT_Memory  memory = table->memory;


    memory->user = table->memory_user;
    table->મુક્ત( memory, block );
    memory->user = table;
  }


  સ્થિર શૂન્ય
  ft_mem_table_resize( FT_MemTable  table )
  {
    FT_Long  new_size;


    new_size = ft_mem_closest_અવિભાજ્ય( table->nodes );
    જો ( new_size != table->size )
    {
      FT_Mem_ગાંઠ*  new_buckets;
      FT_Long      i;


      new_buckets = (FT_Mem_ગાંઠ *)
                      ft_mem_table_alloc(
                        table,
                        new_size * (FT_Long)કદ ( FT_Mem_ગાંઠ ) );
      જો ( !new_buckets )
        પાછા;

      FT_ARRAY_ZERO( new_buckets, new_size );

      માટે ( i = 0; i < table->size; i++ )
      {
        FT_Mem_ગાંઠ  ગાંઠ, next, *pnode;
        FT_PtrDist  hash;


        ગાંઠ = table->buckets[i];
        જ્યારે ( ગાંઠ )
        {
          next  = ગાંઠ->link;
          hash  = FT_MEM_VAL( ગાંઠ->address ) % (FT_PtrDist)new_size;
          pnode = new_buckets + hash;

          ગાંઠ->link = pnode[0];
          pnode[0]   = ગાંઠ;

          ગાંઠ = next;
        }
      }

      જો ( table->buckets )
        ft_mem_table_free( table, table->buckets );

      table->buckets = new_buckets;
      table->size    = new_size;
    }
  }


  સ્થિર શૂન્ય
  ft_mem_table_destroy( FT_MemTable  table )
  {
    FT_Long  i;
    FT_Long  leak_ગણતરી = 0;
    FT_Long  leaks      = 0;


    /* remove all blocks from the table, revealing leaked ones */
    માટે ( i = 0; i < table->size; i++ )
    {
      FT_Mem_ગાંઠ  *pnode = table->buckets + i, next, ગાંઠ = *pnode;


      જ્યારે ( ગાંઠ )
      {
        next       = ગાંઠ->link;
        ગાંઠ->link = ખાલી;

        જો ( ગાંઠ->size > 0 )
        {
          printf(
            "leaked memory block at address %p, size %8ld in (%s:%ld)\n",
            (શૂન્ય*)ગાંઠ->address,
            ગાંઠ->size,
            FT_FILENAME( ગાંઠ->source->file_name ),
            ગાંઠ->source->લીટી_no );

          leak_ગણતરી++;
          leaks += ગાંઠ->size;

          ft_mem_table_free( table, ગાંઠ->address );
        }

        ગાંઠ->address = ખાલી;
        ગાંઠ->size    = 0;

        ft_mem_table_free( table, ગાંઠ );
        ગાંઠ = next;
      }
      table->buckets[i] = ખાલી;
    }

    ft_mem_table_free( table, table->buckets );
    table->buckets = ખાલી;

    table->size  = 0;
    table->nodes = 0;

    /* remove all sources */
    માટે ( i = 0; i < FT_MEM_SOURCE_BUCKETS; i++ )
    {
      FT_MemSource  source, next;


      માટે ( source = table->sources[i]; source != ખાલી; source = next )
      {
        next = source->link;
        ft_mem_table_free( table, source );
      }

      table->sources[i] = ખાલી;
    }

    printf( "FreeType: total memory allocations = %ld\n",
            table->alloc_total );
    printf( "FreeType: maximum memory footprint = %ld\n",
            table->alloc_max );

    જો ( leak_ગણતરી > 0 )
      ft_mem_debug_panic(
        "FreeType: %ld bytes of memory leaked in %ld blocks\n",
        leaks, leak_ગણતરી );

    printf( "FreeType: no memory leaks detected\n" );
  }


  સ્થિર FT_Mem_ગાંઠ*
  ft_mem_table_get_nodep( FT_MemTable  table,
                          FT_Byte*     address )
  {
    FT_PtrDist   hash;
    FT_Mem_ગાંઠ  *pnode, ગાંઠ;


    hash  = FT_MEM_VAL( address );
    pnode = table->buckets + ( hash % (FT_PtrDist)table->size );

    માટે (;;)
    {
      ગાંઠ = pnode[0];
      જો ( !ગાંઠ )
        તોડો;

      જો ( ગાંઠ->address == address )
        તોડો;

      pnode = &ગાંઠ->link;
    }
    પાછા pnode;
  }


  સ્થિર FT_MemSource
  ft_mem_table_get_source( FT_MemTable  table )
  {
    FT_UInt32     hash;
    FT_MemSource  ગાંઠ, *pnode;


    /* cast to FT_PtrDist first since void* can be larger */
    /* than FT_UInt32 and GCC 4.1.1 emits a warning       */
    hash  = (FT_UInt32)(FT_PtrDist)(શૂન્ય*)ft_debug_file_ +
              (FT_UInt32)( 5 * ft_debug_lineno_ );
    pnode = &table->sources[hash % FT_MEM_SOURCE_BUCKETS];

    માટે (;;)
    {
      ગાંઠ = *pnode;
      જો ( !ગાંઠ )
        તોડો;

      જો ( ગાંઠ->file_name == ft_debug_file_   &&
           ગાંઠ->લીટી_no   == ft_debug_lineno_ )
        જાઓ Exit;

      pnode = &ગાંઠ->link;
    }

    ગાંઠ = (FT_MemSource)ft_mem_table_alloc( table, કદ ( *ગાંઠ ) );
    જો ( !ગાંઠ )
      ft_mem_debug_panic(
        "not enough memory to perform memory debugging\n" );

    ગાંઠ->file_name = ft_debug_file_;
    ગાંઠ->લીટી_no   = ft_debug_lineno_;

    ગાંઠ->cur_blocks = 0;
    ગાંઠ->max_blocks = 0;
    ગાંઠ->all_blocks = 0;

    ગાંઠ->cur_size = 0;
    ગાંઠ->max_size = 0;
    ગાંઠ->all_size = 0;

    ગાંઠ->cur_max = 0;

    ગાંઠ->link = ખાલી;
    ગાંઠ->hash = hash;
    *pnode     = ગાંઠ;

  Exit:
    પાછા ગાંઠ;
  }


  સ્થિર શૂન્ય
  ft_mem_table_set( FT_MemTable  table,
                    FT_Byte*     address,
                    FT_Long      size,
                    FT_Long      delta )
  {
    FT_Mem_ગાંઠ  *pnode, ગાંઠ;


    જો ( table )
    {
      FT_MemSource  source;


      pnode = ft_mem_table_get_nodep( table, address );
      ગાંઠ  = *pnode;
      જો ( ગાંઠ )
      {
        જો ( ગાંઠ->size < 0 )
        {
          /* This block was already freed.  Our memory is now completely */
          /* corrupted!                                                  */
          /* This can only happen in keep-alive mode.                    */
          ft_mem_debug_panic(
            "memory heap corrupted (allocating freed block)" );
        }
        નહીંતર
        {
          /* This block was already allocated.  This means that our memory */
          /* is also corrupted!                                            */
          ft_mem_debug_panic(
            "memory heap corrupted (re-allocating allocated block at"
            " %p, of size %ld)\n"
            "org=%s:%d new=%s:%d\n",
            ગાંઠ->address, ગાંઠ->size,
            FT_FILENAME( ગાંઠ->source->file_name ), ગાંઠ->source->લીટી_no,
            FT_FILENAME( ft_debug_file_ ), ft_debug_lineno_ );
        }
      }

      /* we need to create a new node in this table */
      ગાંઠ = (FT_Mem_ગાંઠ)ft_mem_table_alloc( table, કદ ( *ગાંઠ ) );
      જો ( !ગાંઠ )
        ft_mem_debug_panic( "not enough memory to run memory tests" );

      ગાંઠ->address = address;
      ગાંઠ->size    = size;
      ગાંઠ->source  = source = ft_mem_table_get_source( table );

      જો ( delta == 0 )
      {
        /* this is an allocation */
        source->all_blocks++;
        source->cur_blocks++;
        જો ( source->cur_blocks > source->max_blocks )
          source->max_blocks = source->cur_blocks;
      }

      જો ( size > source->cur_max )
        source->cur_max = size;

      જો ( delta != 0 )
      {
        /* we are growing or shrinking a reallocated block */
        source->cur_size     += delta;
        table->alloc_current += delta;
      }
      નહીંતર
      {
        /* we are allocating a new block */
        source->cur_size     += size;
        table->alloc_current += size;
      }

      source->all_size += size;

      જો ( source->cur_size > source->max_size )
        source->max_size = source->cur_size;

      ગાંઠ->free_file_name = ખાલી;
      ગાંઠ->free_લીટી_no   = 0;

      ગાંઠ->link = pnode[0];

      pnode[0] = ગાંઠ;
      table->nodes++;

      table->alloc_total += size;

      જો ( table->alloc_current > table->alloc_max )
        table->alloc_max = table->alloc_current;

      જો ( table->nodes * 3 < table->size  ||
           table->size  * 3 < table->nodes )
        ft_mem_table_resize( table );
    }
  }


  સ્થિર શૂન્ય
  ft_mem_table_remove( FT_MemTable  table,
                       FT_Byte*     address,
                       FT_Long      delta )
  {
    જો ( table )
    {
      FT_Mem_ગાંઠ  *pnode, ગાંઠ;


      pnode = ft_mem_table_get_nodep( table, address );
      ગાંઠ  = *pnode;
      જો ( ગાંઠ )
      {
        FT_MemSource  source;


        જો ( ગાંઠ->size < 0 )
          ft_mem_debug_panic(
            "freeing memory block at %p more than once\n"
            "  at (%s:%ld)!\n"
            "  Block was allocated at (%s:%ld)\n"
            "  and released at (%s:%ld).",
            address,
            FT_FILENAME( ft_debug_file_ ), ft_debug_lineno_,
            FT_FILENAME( ગાંઠ->source->file_name ), ગાંઠ->source->લીટી_no,
            FT_FILENAME( ગાંઠ->free_file_name ), ગાંઠ->free_લીટી_no );

        /* scramble the node's content for additional safety */
        FT_MEM_SET( address, 0xF3, ગાંઠ->size );

        જો ( delta == 0 )
        {
          source = ગાંઠ->source;

          source->cur_blocks--;
          source->cur_size -= ગાંઠ->size;

          table->alloc_current -= ગાંઠ->size;
        }

        જો ( table->keep_alive )
        {
          /* we simply invert the node's size to indicate that the node */
          /* was freed.                                                 */
          ગાંઠ->size           = -ગાંઠ->size;
          ગાંઠ->free_file_name = ft_debug_file_;
          ગાંઠ->free_લીટી_no   = ft_debug_lineno_;
        }
        નહીંતર
        {
          table->nodes--;

          *pnode = ગાંઠ->link;

          ગાંઠ->size   = 0;
          ગાંઠ->source = ખાલી;

          ft_mem_table_free( table, ગાંઠ );

          જો ( table->nodes * 3 < table->size  ||
               table->size  * 3 < table->nodes )
            ft_mem_table_resize( table );
        }
      }
      નહીંતર
        ft_mem_debug_panic(
          "trying to free unknown block at %p in (%s:%ld)\n",
          address,
          FT_FILENAME( ft_debug_file_ ), ft_debug_lineno_ );
    }
  }


  સ્થિર FT_Pointer
  ft_mem_debug_alloc( FT_Memory  memory,
                      FT_Long    size )
  {
    FT_MemTable  table = (FT_MemTable)memory->user;
    FT_Byte*     block;


    જો ( size <= 0 )
      ft_mem_debug_panic( "negative block size allocation (%ld)", size );

    /* return NULL if the maximum number of allocations was reached */
    જો ( table->bound_ગણતરી                           &&
         table->alloc_ગણતરી >= table->alloc_ગણતરી_max )
      પાછા ખાલી;

    /* return NULL if this allocation would overflow the maximum heap size */
    જો ( table->bound_total                                   &&
         table->alloc_total_max - table->alloc_current > size )
      પાછા ખાલી;

    block = (FT_Byte *)ft_mem_table_alloc( table, size );
    જો ( block )
    {
      ft_mem_table_set( table, block, size, 0 );

      table->alloc_ગણતરી++;
    }

    ft_debug_file_   = "<unknown>";
    ft_debug_lineno_ = 0;

    પાછા (FT_Pointer)block;
  }


  સ્થિર શૂન્ય
  ft_mem_debug_free( FT_Memory   memory,
                     FT_Pointer  block )
  {
    FT_MemTable  table = (FT_MemTable)memory->user;


    જો ( !block )
      ft_mem_debug_panic( "trying to free NULL in (%s:%ld)",
                          FT_FILENAME( ft_debug_file_ ),
                          ft_debug_lineno_ );

    ft_mem_table_remove( table, (FT_Byte*)block, 0 );

    જો ( !table->keep_alive )
      ft_mem_table_free( table, block );

    table->alloc_ગણતરી--;

    ft_debug_file_   = "<unknown>";
    ft_debug_lineno_ = 0;
  }


  સ્થિર FT_Pointer
  ft_mem_debug_realloc( FT_Memory   memory,
                        FT_Long     cur_size,
                        FT_Long     new_size,
                        FT_Pointer  block )
  {
    FT_MemTable  table = (FT_MemTable)memory->user;
    FT_Mem_ગાંઠ   ગાંઠ, *pnode;
    FT_Pointer   new_block;
    FT_Long      delta;

    અચલ અક્ષર*  file_name = FT_FILENAME( ft_debug_file_ );
    FT_Long      લીટી_no   = ft_debug_lineno_;


    /* unlikely, but possible */
    જો ( new_size == cur_size )
      પાછા block;

    /* the following is valid according to ANSI C */
#જો 0
    જો ( !block || !cur_size )
      ft_mem_debug_panic( "trying to reallocate NULL in (%s:%ld)",
                          file_name, લીટી_no );
#endif

    /* while the following is allowed in ANSI C also, we abort since */
    /* such case should be handled by FreeType.                      */
    જો ( new_size <= 0 )
      ft_mem_debug_panic(
        "trying to reallocate %p to size 0 (current is %ld) in (%s:%ld)",
        block, cur_size, file_name, લીટી_no );

    /* check `cur_size' value */
    pnode = ft_mem_table_get_nodep( table, (FT_Byte*)block );
    ગાંઠ  = *pnode;
    જો ( !ગાંઠ )
      ft_mem_debug_panic(
        "trying to reallocate unknown block at %p in (%s:%ld)",
        block, file_name, લીટી_no );

    જો ( ગાંઠ->size <= 0 )
      ft_mem_debug_panic(
        "trying to reallocate freed block at %p in (%s:%ld)",
        block, file_name, લીટી_no );

    જો ( ગાંઠ->size != cur_size )
      ft_mem_debug_panic( "invalid ft_realloc request for %p. cur_size is "
                          "%ld instead of %ld in (%s:%ld)",
                          block, cur_size, ગાંઠ->size, file_name, લીટી_no );

    /* return NULL if the maximum number of allocations was reached */
    જો ( table->bound_ગણતરી                           &&
         table->alloc_ગણતરી >= table->alloc_ગણતરી_max )
      પાછા ખાલી;

    delta = new_size - cur_size;

    /* return NULL if this allocation would overflow the maximum heap size */
    જો ( delta > 0                                             &&
         table->bound_total                                    &&
         table->alloc_current + delta > table->alloc_total_max )
      પાછા ખાલી;

    new_block = (FT_Pointer)ft_mem_table_alloc( table, new_size );
    જો ( !new_block )
      પાછા ખાલી;

    ft_mem_table_set( table, (FT_Byte*)new_block, new_size, delta );

    ft_memcpy( new_block, block, cur_size < new_size ? (size_t)cur_size
                                                     : (size_t)new_size );

    ft_mem_table_remove( table, (FT_Byte*)block, delta );

    ft_debug_file_   = "<unknown>";
    ft_debug_lineno_ = 0;

    જો ( !table->keep_alive )
      ft_mem_table_free( table, block );

    પાછા new_block;
  }


  બાહ્ય શૂન્ય
  ft_mem_debug_init( FT_Memory  memory )
  {
    FT_MemTable  table;


    જો ( !ft_getenv( "FT2_DEBUG_MEMORY" ) )
      પાછા;

    table = (FT_MemTable)memory->alloc( memory, કદ ( *table ) );

    જો ( table )
    {
      FT_ZERO( table );

      table->memory      = memory;
      table->memory_user = memory->user;
      table->alloc       = memory->alloc;
      table->realloc     = memory->realloc;
      table->મુક્ત        = memory->મુક્ત;

      ft_mem_table_resize( table );

      જો ( table->size )
      {
        અચલ અક્ષર*  p;


        memory->user    = table;
        memory->alloc   = ft_mem_debug_alloc;
        memory->realloc = ft_mem_debug_realloc;
        memory->મુક્ત    = ft_mem_debug_free;

        p = ft_getenv( "FT2_ALLOC_TOTAL_MAX" );
        જો ( p )
        {
          FT_Long  total_max = ft_strtol( p, ખાલી, 10 );


          જો ( total_max > 0 )
          {
            table->bound_total     = 1;
            table->alloc_total_max = total_max;
          }
        }

        p = ft_getenv( "FT2_ALLOC_COUNT_MAX" );
        જો ( p )
        {
          FT_Long  total_ગણતરી = ft_strtol( p, ખાલી, 10 );


          જો ( total_ગણતરી > 0 )
          {
            table->bound_ગણતરી     = 1;
            table->alloc_ગણતરી_max = total_ગણતરી;
          }
        }

        p = ft_getenv( "FT2_KEEP_ALIVE" );
        જો ( p )
        {
          FT_Long  keep_alive = ft_strtol( p, ખાલી, 10 );


          જો ( keep_alive > 0 )
            table->keep_alive = 1;
        }
      }
      નહીંતર
        memory->મુક્ત( memory, table );
    }
  }


  બાહ્ય શૂન્ય
  ft_mem_debug_done( FT_Memory  memory )
  {
    જો ( memory->મુક્ત == ft_mem_debug_free )
    {
      FT_MemTable  table = (FT_MemTable)memory->user;


      FT_DumpMemory( memory );

      ft_mem_table_destroy( table );

      memory->મુક્ત    = table->મુક્ત;
      memory->realloc = table->realloc;
      memory->alloc   = table->alloc;
      memory->user    = table->memory_user;

      memory->મુક્ત( memory, table );
    }
  }


  FT_COMPARE_DEF( પૂર્ણાંક )
  ft_mem_source_compare( અચલ શૂન્ય*  p1,
                         અચલ શૂન્ય*  p2 )
  {
    FT_MemSource  s1 = *(FT_MemSource*)p1;
    FT_MemSource  s2 = *(FT_MemSource*)p2;


    જો ( s2->max_size > s1->max_size )
      પાછા 1;
    નહીંતર જો ( s2->max_size < s1->max_size )
      પાછા -1;
    નહીંતર
      પાછા 0;
  }


  બાહ્ય શૂન્ય
  FT_DumpMemory( FT_Memory  memory )
  {
    જો ( memory->મુક્ત == ft_mem_debug_free )
    {
      FT_MemTable    table = (FT_MemTable)memory->user;
      FT_MemSource*  bucket = table->sources;
      FT_MemSource*  limit  = bucket + FT_MEM_SOURCE_BUCKETS;
      FT_MemSource*  sources;
      FT_Int         nn, ગણતરી;
      અચલ અક્ષર*    fmt;


      ગણતરી = 0;
      માટે ( ; bucket < limit; bucket++ )
      {
        FT_MemSource  source = *bucket;


        માટે ( ; source; source = source->link )
          ગણતરી++;
      }

      sources = (FT_MemSource*)
                  ft_mem_table_alloc(
                    table, ગણતરી * (FT_Long)કદ ( *sources ) );

      ગણતરી = 0;
      માટે ( bucket = table->sources; bucket < limit; bucket++ )
      {
        FT_MemSource  source = *bucket;


        માટે ( ; source; source = source->link )
          sources[ગણતરી++] = source;
      }

      ft_qsort( sources,
                (size_t)ગણતરી,
                કદ ( *sources ),
                ft_mem_source_compare );

      printf( "FreeType Memory Dump: "
              "current=%ld max=%ld total=%ld count=%ld\n",
              table->alloc_current, table->alloc_max,
              table->alloc_total, table->alloc_ગણતરી );
      printf( " block  block    sizes    sizes    sizes   source\n" );
      printf( " count   high      sum  highsum      max   location\n" );
      printf( "-------------------------------------------------\n" );

      fmt = "%6ld %6ld %8ld %8ld %8ld %s:%d\n";

      માટે ( nn = 0; nn < ગણતરી; nn++ )
      {
        FT_MemSource  source = sources[nn];


        printf( fmt,
                source->cur_blocks, source->max_blocks,
                source->cur_size, source->max_size, source->cur_max,
                FT_FILENAME( source->file_name ),
                source->લીટી_no );
      }
      printf( "------------------------------------------------\n" );

      ft_mem_table_free( table, sources );
    }
  }

#નહીંતર  /* !FT_DEBUG_MEMORY */

  /* ANSI C doesn't like empty source files */
  પ્રકારવ્યાખ્યા પૂર્ણાંક  debug_mem_dummy_;

#endif /* !FT_DEBUG_MEMORY */


/* END */
