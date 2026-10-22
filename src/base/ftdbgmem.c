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


#অন্তর্ভাব <ft2build.h>
#অন্তর্ভাব FT_CONFIG_CONFIG_H
#অন্তর্ভাব <freetype/internal/ftdebug.h>
#অন্তর্ভাব <freetype/internal/ftmemory.h>
#অন্তর্ভাব <freetype/ftsystem.h>
#অন্তর্ভাব <freetype/fterrors.h>
#অন্তর্ভাব <freetype/fttypes.h>


#ifdef FT_DEBUG_MEMORY

#define  KEEPALIVE /* `Keep alive' means that freed blocks aren't released
                    * to the heap.  This is useful to detect double-frees
                    * or weird heap corruption, but it uses large amounts of
                    * memory, however.
                    */

#অন্তর্ভাব FT_CONFIG_STANDARD_LIBRARY_H

  FT_BASE_DEF( ধ্রুবক অক্ষর* )  ft_debug_file_   = ফাঁকা;
  FT_BASE_DEF( long )         ft_debug_lineno_ = 0;

  বাহ্যিক শূন্য
  FT_DumpMemory( FT_Memory  memory );


  প্রকারলেখ কাঠামো FT_MemSourceRec_*  FT_MemSource;
  প্রকারলেখ কাঠামো FT_Mem_গ্রন্থি_Rec*    FT_Mem_গ্রন্থি;
  প্রকারলেখ কাঠামো FT_MemTableRec_*   FT_MemTable;


#define FT_MEM_VAL( addr )  ( (FT_PtrDist)(FT_Pointer)( addr ) )

  /*
   * This structure holds statistics for a single allocation/release
   * site.  This is useful to know where memory operations happen the
   * most.
   */
  প্রকারলেখ কাঠামো  FT_MemSourceRec_
  {
    ধ্রুবক অক্ষর*   file_name;
    long          সারি_no;

    FT_Long       cur_blocks;   /* current number of allocated blocks */
    FT_Long       max_blocks;   /* max. number of allocated blocks    */
    FT_Long       all_blocks;   /* total number of blocks allocated   */

    FT_Long       cur_আকার;     /* current cumulative allocated size */
    FT_Long       max_আকার;     /* maximum cumulative allocated size */
    FT_Long       all_আকার;     /* total cumulative allocated size   */

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
  প্রকারলেখ কাঠামো  FT_Mem_গ্রন্থি_Rec
  {
    FT_Byte*      address;
    FT_Long       আকার_2;     /* < 0 if the block was freed */

    FT_MemSource  source;

#ifdef KEEPALIVE
    ধ্রুবক অক্ষর*   free_file_name;
    FT_Long       free_সারি_no;
#endif

    FT_Mem_গ্রন্থি    link;

  } FT_Mem_গ্রন্থি_Rec_2;


  /*
   * The global structure, containing compound statistics and all hash
   * tables.
   */
  প্রকারলেখ কাঠামো  FT_MemTableRec_
  {
    FT_Long          আকার_2;
    FT_Long          nodes;
    FT_Mem_গ্রন্থি*      buckets;

    FT_Long          alloc_যোগ;
    FT_Long          alloc_current;
    FT_Long          alloc_max;
    FT_Long          alloc_গুনতি;

    FT_Bool          bound_যোগ;
    FT_Long          alloc_যোগ_max;

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


#define FT_MEM_আকার_MAX  13845163

#define FT_FILENAME( x )  ( (x) ? (x) : "unknown file" )


  /*
   * Prime numbers are ugly to handle.  It would be better to implement
   * L-Hashing, which is 10% faster and doesn't require divisions.
   */
  স্থির ধ্রুবক FT_Int  ft_mem_primes[] =
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


    জন্য ( i = 0;
          i < আকার ( ft_mem_primes ) / আকার ( ft_mem_primes[0] ); i++ )
      যদি ( ft_mem_primes[i] > অঙ্ক )
        ফেরত ft_mem_primes[i];

    ফেরত FT_MEM_আকার_MAX;
  }


  স্থির শূন্য
  ft_mem_debug_panic( ধ্রুবক অক্ষর*  fmt,
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
                      FT_Long      আকার_2 )
  {
    FT_Memory   memory = table->memory;
    FT_Pointer  block;


    memory->user = table->memory_user;
    block = table->alloc( memory, আকার_2 );
    memory->user = table;

    ফেরত block;
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
    FT_Long  new_আকার;


    new_আকার = ft_mem_closest_মৌলিক( table->nodes );
    যদি ( new_আকার != table->আকার_2 )
    {
      FT_Mem_গ্রন্থি*  new_buckets;
      FT_Long      i;


      new_buckets = (FT_Mem_গ্রন্থি *)
                      ft_mem_table_alloc(
                        table,
                        new_আকার * (FT_Long)আকার ( FT_Mem_গ্রন্থি ) );
      যদি ( !new_buckets )
        ফেরত;

      FT_ARRAY_ZERO( new_buckets, new_আকার );

      জন্য ( i = 0; i < table->আকার_2; i++ )
      {
        FT_Mem_গ্রন্থি  গ্রন্থি, পরবর্তী, *pnode;
        FT_PtrDist  hash;


        গ্রন্থি = table->buckets[i];
        যখন ( গ্রন্থি )
        {
          পরবর্তী  = গ্রন্থি->link;
          hash  = FT_MEM_VAL( গ্রন্থি->address ) % (FT_PtrDist)new_আকার;
          pnode = new_buckets + hash;

          গ্রন্থি->link = pnode[0];
          pnode[0]   = গ্রন্থি;

          গ্রন্থি = পরবর্তী;
        }
      }

      যদি ( table->buckets )
        ft_mem_table_free( table, table->buckets );

      table->buckets = new_buckets;
      table->আকার_2    = new_আকার;
    }
  }


  স্থির শূন্য
  ft_mem_table_destroy( FT_MemTable  table )
  {
    FT_Long  i;
    FT_Long  leak_গুনতি = 0;
    FT_Long  leaks      = 0;


    /* remove all blocks from the table, revealing leaked ones */
    জন্য ( i = 0; i < table->আকার_2; i++ )
    {
      FT_Mem_গ্রন্থি  *pnode = table->buckets + i, পরবর্তী, গ্রন্থি = *pnode;


      যখন ( গ্রন্থি )
      {
        পরবর্তী       = গ্রন্থি->link;
        গ্রন্থি->link = ফাঁকা;

        যদি ( গ্রন্থি->আকার_2 > 0 )
        {
          printf(
            "leaked memory block at address %p, size %8ld in (%s:%ld)\n",
            (শূন্য*)গ্রন্থি->address,
            গ্রন্থি->আকার_2,
            FT_FILENAME( গ্রন্থি->source->file_name ),
            গ্রন্থি->source->সারি_no );

          leak_গুনতি++;
          leaks += গ্রন্থি->আকার_2;

          ft_mem_table_free( table, গ্রন্থি->address );
        }

        গ্রন্থি->address = ফাঁকা;
        গ্রন্থি->আকার_2    = 0;

        ft_mem_table_free( table, গ্রন্থি );
        গ্রন্থি = পরবর্তী;
      }
      table->buckets[i] = ফাঁকা;
    }

    ft_mem_table_free( table, table->buckets );
    table->buckets = ফাঁকা;

    table->আকার_2  = 0;
    table->nodes = 0;

    /* remove all sources */
    জন্য ( i = 0; i < FT_MEM_SOURCE_BUCKETS; i++ )
    {
      FT_MemSource  source, পরবর্তী;


      জন্য ( source = table->sources[i]; source != ফাঁকা; source = পরবর্তী )
      {
        পরবর্তী = source->link;
        ft_mem_table_free( table, source );
      }

      table->sources[i] = ফাঁকা;
    }

    printf( "FreeType: total memory allocations = %ld\n",
            table->alloc_যোগ );
    printf( "FreeType: maximum memory footprint = %ld\n",
            table->alloc_max );

    যদি ( leak_গুনতি > 0 )
      ft_mem_debug_panic(
        "FreeType: %ld bytes of memory leaked in %ld blocks\n",
        leaks, leak_গুনতি );

    printf( "FreeType: no memory leaks detected\n" );
  }


  স্থির FT_Mem_গ্রন্থি*
  ft_mem_table_get_nodep( FT_MemTable  table,
                          FT_Byte*     address )
  {
    FT_PtrDist   hash;
    FT_Mem_গ্রন্থি  *pnode, গ্রন্থি;


    hash  = FT_MEM_VAL( address );
    pnode = table->buckets + ( hash % (FT_PtrDist)table->আকার_2 );

    জন্য (;;)
    {
      গ্রন্থি = pnode[0];
      যদি ( !গ্রন্থি )
        বিরতি;

      যদি ( গ্রন্থি->address == address )
        বিরতি;

      pnode = &গ্রন্থি->link;
    }
    ফেরত pnode;
  }


  স্থির FT_MemSource
  ft_mem_table_get_source( FT_MemTable  table )
  {
    FT_UInt32     hash;
    FT_MemSource  গ্রন্থি, *pnode;


    /* cast to FT_PtrDist first since void* can be larger */
    /* than FT_UInt32 and GCC 4.1.1 emits a warning       */
    hash  = (FT_UInt32)(FT_PtrDist)(শূন্য*)ft_debug_file_ +
              (FT_UInt32)( 5 * ft_debug_lineno_ );
    pnode = &table->sources[hash % FT_MEM_SOURCE_BUCKETS];

    জন্য (;;)
    {
      গ্রন্থি = *pnode;
      যদি ( !গ্রন্থি )
        বিরতি;

      যদি ( গ্রন্থি->file_name == ft_debug_file_   &&
           গ্রন্থি->সারি_no   == ft_debug_lineno_ )
        যাও Exit;

      pnode = &গ্রন্থি->link;
    }

    গ্রন্থি = (FT_MemSource)ft_mem_table_alloc( table, আকার ( *গ্রন্থি ) );
    যদি ( !গ্রন্থি )
      ft_mem_debug_panic(
        "not enough memory to perform memory debugging\n" );

    গ্রন্থি->file_name = ft_debug_file_;
    গ্রন্থি->সারি_no   = ft_debug_lineno_;

    গ্রন্থি->cur_blocks = 0;
    গ্রন্থি->max_blocks = 0;
    গ্রন্থি->all_blocks = 0;

    গ্রন্থি->cur_আকার = 0;
    গ্রন্থি->max_আকার = 0;
    গ্রন্থি->all_আকার = 0;

    গ্রন্থি->cur_max = 0;

    গ্রন্থি->link = ফাঁকা;
    গ্রন্থি->hash = hash;
    *pnode     = গ্রন্থি;

  Exit:
    ফেরত গ্রন্থি;
  }


  স্থির শূন্য
  ft_mem_table_set( FT_MemTable  table,
                    FT_Byte*     address,
                    FT_Long      আকার_2,
                    FT_Long      delta )
  {
    FT_Mem_গ্রন্থি  *pnode, গ্রন্থি;


    যদি ( table )
    {
      FT_MemSource  source;


      pnode = ft_mem_table_get_nodep( table, address );
      গ্রন্থি  = *pnode;
      যদি ( গ্রন্থি )
      {
        যদি ( গ্রন্থি->আকার_2 < 0 )
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
            গ্রন্থি->address, গ্রন্থি->আকার_2,
            FT_FILENAME( গ্রন্থি->source->file_name ), গ্রন্থি->source->সারি_no,
            FT_FILENAME( ft_debug_file_ ), ft_debug_lineno_ );
        }
      }

      /* we need to create a new node in this table */
      গ্রন্থি = (FT_Mem_গ্রন্থি)ft_mem_table_alloc( table, আকার ( *গ্রন্থি ) );
      যদি ( !গ্রন্থি )
        ft_mem_debug_panic( "not enough memory to run memory tests" );

      গ্রন্থি->address = address;
      গ্রন্থি->আকার_2    = আকার_2;
      গ্রন্থি->source  = source = ft_mem_table_get_source( table );

      যদি ( delta == 0 )
      {
        /* this is an allocation */
        source->all_blocks++;
        source->cur_blocks++;
        যদি ( source->cur_blocks > source->max_blocks )
          source->max_blocks = source->cur_blocks;
      }

      যদি ( আকার_2 > source->cur_max )
        source->cur_max = আকার_2;

      যদি ( delta != 0 )
      {
        /* we are growing or shrinking a reallocated block */
        source->cur_আকার     += delta;
        table->alloc_current += delta;
      }
      অন্যথা
      {
        /* we are allocating a new block */
        source->cur_আকার     += আকার_2;
        table->alloc_current += আকার_2;
      }

      source->all_আকার += আকার_2;

      যদি ( source->cur_আকার > source->max_আকার )
        source->max_আকার = source->cur_আকার;

      গ্রন্থি->free_file_name = ফাঁকা;
      গ্রন্থি->free_সারি_no   = 0;

      গ্রন্থি->link = pnode[0];

      pnode[0] = গ্রন্থি;
      table->nodes++;

      table->alloc_যোগ += আকার_2;

      যদি ( table->alloc_current > table->alloc_max )
        table->alloc_max = table->alloc_current;

      যদি ( table->nodes * 3 < table->আকার_2  ||
           table->আকার_2  * 3 < table->nodes )
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
      FT_Mem_গ্রন্থি  *pnode, গ্রন্থি;


      pnode = ft_mem_table_get_nodep( table, address );
      গ্রন্থি  = *pnode;
      যদি ( গ্রন্থি )
      {
        FT_MemSource  source;


        যদি ( গ্রন্থি->আকার_2 < 0 )
          ft_mem_debug_panic(
            "freeing memory block at %p more than once\n"
            "  at (%s:%ld)!\n"
            "  Block was allocated at (%s:%ld)\n"
            "  and released at (%s:%ld).",
            address,
            FT_FILENAME( ft_debug_file_ ), ft_debug_lineno_,
            FT_FILENAME( গ্রন্থি->source->file_name ), গ্রন্থি->source->সারি_no,
            FT_FILENAME( গ্রন্থি->free_file_name ), গ্রন্থি->free_সারি_no );

        /* scramble the node's content for additional safety */
        FT_MEM_SET( address, 0xF3, গ্রন্থি->আকার_2 );

        যদি ( delta == 0 )
        {
          source = গ্রন্থি->source;

          source->cur_blocks--;
          source->cur_আকার -= গ্রন্থি->আকার_2;

          table->alloc_current -= গ্রন্থি->আকার_2;
        }

        যদি ( table->keep_alive )
        {
          /* we simply invert the node's size to indicate that the node */
          /* was freed.                                                 */
          গ্রন্থি->আকার_2           = -গ্রন্থি->আকার_2;
          গ্রন্থি->free_file_name = ft_debug_file_;
          গ্রন্থি->free_সারি_no   = ft_debug_lineno_;
        }
        অন্যথা
        {
          table->nodes--;

          *pnode = গ্রন্থি->link;

          গ্রন্থি->আকার_2   = 0;
          গ্রন্থি->source = ফাঁকা;

          ft_mem_table_free( table, গ্রন্থি );

          যদি ( table->nodes * 3 < table->আকার_2  ||
               table->আকার_2  * 3 < table->nodes )
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
                      FT_Long    আকার_2 )
  {
    FT_MemTable  table = (FT_MemTable)memory->user;
    FT_Byte*     block;


    যদি ( আকার_2 <= 0 )
      ft_mem_debug_panic( "negative block size allocation (%ld)", আকার_2 );

    /* return NULL if the maximum number of allocations was reached */
    যদি ( table->bound_গুনতি                           &&
         table->alloc_গুনতি >= table->alloc_গুনতি_max )
      ফেরত ফাঁকা;

    /* return NULL if this allocation would overflow the maximum heap size */
    যদি ( table->bound_যোগ                                   &&
         table->alloc_যোগ_max - table->alloc_current > আকার_2 )
      ফেরত ফাঁকা;

    block = (FT_Byte *)ft_mem_table_alloc( table, আকার_2 );
    যদি ( block )
    {
      ft_mem_table_set( table, block, আকার_2, 0 );

      table->alloc_গুনতি++;
    }

    ft_debug_file_   = "<unknown>";
    ft_debug_lineno_ = 0;

    ফেরত (FT_Pointer)block;
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
                        FT_Long     cur_আকার,
                        FT_Long     new_আকার,
                        FT_Pointer  block )
  {
    FT_MemTable  table = (FT_MemTable)memory->user;
    FT_Mem_গ্রন্থি   গ্রন্থি, *pnode;
    FT_Pointer   new_block;
    FT_Long      delta;

    ধ্রুবক অক্ষর*  file_name = FT_FILENAME( ft_debug_file_ );
    FT_Long      সারি_no   = ft_debug_lineno_;


    /* unlikely, but possible */
    যদি ( new_আকার == cur_আকার )
      ফেরত block;

    /* the following is valid according to ANSI C */
#যদি 0
    যদি ( !block || !cur_আকার )
      ft_mem_debug_panic( "trying to reallocate NULL in (%s:%ld)",
                          file_name, সারি_no );
#endif

    /* while the following is allowed in ANSI C also, we abort since */
    /* such case should be handled by FreeType.                      */
    যদি ( new_আকার <= 0 )
      ft_mem_debug_panic(
        "trying to reallocate %p to size 0 (current is %ld) in (%s:%ld)",
        block, cur_আকার, file_name, সারি_no );

    /* check `cur_size' value */
    pnode = ft_mem_table_get_nodep( table, (FT_Byte*)block );
    গ্রন্থি  = *pnode;
    যদি ( !গ্রন্থি )
      ft_mem_debug_panic(
        "trying to reallocate unknown block at %p in (%s:%ld)",
        block, file_name, সারি_no );

    যদি ( গ্রন্থি->আকার_2 <= 0 )
      ft_mem_debug_panic(
        "trying to reallocate freed block at %p in (%s:%ld)",
        block, file_name, সারি_no );

    যদি ( গ্রন্থি->আকার_2 != cur_আকার )
      ft_mem_debug_panic( "invalid ft_realloc request for %p. cur_size is "
                          "%ld instead of %ld in (%s:%ld)",
                          block, cur_আকার, গ্রন্থি->আকার_2, file_name, সারি_no );

    /* return NULL if the maximum number of allocations was reached */
    যদি ( table->bound_গুনতি                           &&
         table->alloc_গুনতি >= table->alloc_গুনতি_max )
      ফেরত ফাঁকা;

    delta = new_আকার - cur_আকার;

    /* return NULL if this allocation would overflow the maximum heap size */
    যদি ( delta > 0                                             &&
         table->bound_যোগ                                    &&
         table->alloc_current + delta > table->alloc_যোগ_max )
      ফেরত ফাঁকা;

    new_block = (FT_Pointer)ft_mem_table_alloc( table, new_আকার );
    যদি ( !new_block )
      ফেরত ফাঁকা;

    ft_mem_table_set( table, (FT_Byte*)new_block, new_আকার, delta );

    ft_memcpy( new_block, block, cur_আকার < new_আকার ? (size_t)cur_আকার
                                                     : (size_t)new_আকার );

    ft_mem_table_remove( table, (FT_Byte*)block, delta );

    ft_debug_file_   = "<unknown>";
    ft_debug_lineno_ = 0;

    যদি ( !table->keep_alive )
      ft_mem_table_free( table, block );

    ফেরত new_block;
  }


  বাহ্যিক শূন্য
  ft_mem_debug_init( FT_Memory  memory )
  {
    FT_MemTable  table;


    যদি ( !ft_getenv( "FT2_DEBUG_MEMORY" ) )
      ফেরত;

    table = (FT_MemTable)memory->alloc( memory, আকার ( *table ) );

    যদি ( table )
    {
      FT_ZERO( table );

      table->memory      = memory;
      table->memory_user = memory->user;
      table->alloc       = memory->alloc;
      table->realloc     = memory->realloc;
      table->মুক্ত        = memory->মুক্ত;

      ft_mem_table_resize( table );

      যদি ( table->আকার_2 )
      {
        ধ্রুবক অক্ষর*  p;


        memory->user    = table;
        memory->alloc   = ft_mem_debug_alloc;
        memory->realloc = ft_mem_debug_realloc;
        memory->মুক্ত    = ft_mem_debug_free;

        p = ft_getenv( "FT2_ALLOC_TOTAL_MAX" );
        যদি ( p )
        {
          FT_Long  যোগ_max = ft_strtol( p, ফাঁকা, 10 );


          যদি ( যোগ_max > 0 )
          {
            table->bound_যোগ     = 1;
            table->alloc_যোগ_max = যোগ_max;
          }
        }

        p = ft_getenv( "FT2_ALLOC_COUNT_MAX" );
        যদি ( p )
        {
          FT_Long  যোগ_গুনতি = ft_strtol( p, ফাঁকা, 10 );


          যদি ( যোগ_গুনতি > 0 )
          {
            table->bound_গুনতি     = 1;
            table->alloc_গুনতি_max = যোগ_গুনতি;
          }
        }

        p = ft_getenv( "FT2_KEEP_ALIVE" );
        যদি ( p )
        {
          FT_Long  keep_alive = ft_strtol( p, ফাঁকা, 10 );


          যদি ( keep_alive > 0 )
            table->keep_alive = 1;
        }
      }
      অন্যথা
        memory->মুক্ত( memory, table );
    }
  }


  বাহ্যিক শূন্য
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


  FT_COMPARE_DEF( পূর্ণসংখ্যা )
  ft_mem_source_compare( ধ্রুবক শূন্য*  p1,
                         ধ্রুবক শূন্য*  p2 )
  {
    FT_MemSource  s1 = *(FT_MemSource*)p1;
    FT_MemSource  s2 = *(FT_MemSource*)p2;


    যদি ( s2->max_আকার > s1->max_আকার )
      ফেরত 1;
    অন্যথা যদি ( s2->max_আকার < s1->max_আকার )
      ফেরত -1;
    অন্যথা
      ফেরত 0;
  }


  বাহ্যিক শূন্য
  FT_DumpMemory( FT_Memory  memory )
  {
    যদি ( memory->মুক্ত == ft_mem_debug_free )
    {
      FT_MemTable    table = (FT_MemTable)memory->user;
      FT_MemSource*  bucket = table->sources;
      FT_MemSource*  limit  = bucket + FT_MEM_SOURCE_BUCKETS;
      FT_MemSource*  sources;
      FT_Int         nn, গুনতি;
      ধ্রুবক অক্ষর*    fmt;


      গুনতি = 0;
      জন্য ( ; bucket < limit; bucket++ )
      {
        FT_MemSource  source = *bucket;


        জন্য ( ; source; source = source->link )
          গুনতি++;
      }

      sources = (FT_MemSource*)
                  ft_mem_table_alloc(
                    table, গুনতি * (FT_Long)আকার ( *sources ) );

      গুনতি = 0;
      জন্য ( bucket = table->sources; bucket < limit; bucket++ )
      {
        FT_MemSource  source = *bucket;


        জন্য ( ; source; source = source->link )
          sources[গুনতি++] = source;
      }

      ft_qsort( sources,
                (size_t)গুনতি,
                আকার ( *sources ),
                ft_mem_source_compare );

      printf( "FreeType Memory Dump: "
              "current=%ld max=%ld total=%ld count=%ld\n",
              table->alloc_current, table->alloc_max,
              table->alloc_যোগ, table->alloc_গুনতি );
      printf( " block  block    sizes    sizes    sizes   source\n" );
      printf( " count   high      sum  highsum      max   location\n" );
      printf( "-------------------------------------------------\n" );

      fmt = "%6ld %6ld %8ld %8ld %8ld %s:%d\n";

      জন্য ( nn = 0; nn < গুনতি; nn++ )
      {
        FT_MemSource  source = sources[nn];


        printf( fmt,
                source->cur_blocks, source->max_blocks,
                source->cur_আকার, source->max_আকার, source->cur_max,
                FT_FILENAME( source->file_name ),
                source->সারি_no );
      }
      printf( "------------------------------------------------\n" );

      ft_mem_table_free( table, sources );
    }
  }

#অন্যথা  /* !FT_DEBUG_MEMORY */

  /* ANSI C doesn't like empty source files */
  প্রকারলেখ পূর্ণসংখ্যা  debug_mem_dummy_;

#endif /* !FT_DEBUG_MEMORY */


/* END */
