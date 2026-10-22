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


#समावेश <ft2build.h>
#समावेश FT_CONFIG_CONFIG_H
#समावेश <freetype/internal/ftdebug.h>
#समावेश <freetype/internal/ftmemory.h>
#समावेश <freetype/ftsystem.h>
#समावेश <freetype/fterrors.h>
#समावेश <freetype/fttypes.h>


#अगर_घोषित FT_DEBUG_MEMORY

#घोषणा  KEEPALIVE /* `Keep alive' means that freed blocks aren't released
                    * to the heap.  This is useful to detect double-frees
                    * or weird heap corruption, but it uses large amounts of
                    * memory, however.
                    */

#समावेश FT_CONFIG_STANDARD_LIBRARY_H

  FT_BASE_DEF( स्थिर अक्षर* )  ft_debug_file_   = शून्य;
  FT_BASE_DEF( दीर्घ )         ft_debug_lineno_ = 0;

  बाह्य व्योम
  FT_DumpMemory( FT_Memory  memory );


  प्रकार काष्ठा FT_MemSourceRec_*  FT_MemSource;
  प्रकार काष्ठा FT_Mem_गाँठ_Rec*    FT_Mem_गाँठ;
  प्रकार काष्ठा FT_MemTableRec_*   FT_MemTable;


#घोषणा FT_MEM_VAL( addr )  ( (FT_PtrDist)(FT_Pointer)( addr ) )

  /*
   * This structure holds statistics for a single allocation/release
   * site.  This is useful to know where memory operations happen the
   * most.
   */
  प्रकार काष्ठा  FT_MemSourceRec_
  {
    स्थिर अक्षर*   file_name;
    दीर्घ          पंक्ति_no;

    FT_Long       cur_blocks;   /* current number of allocated blocks */
    FT_Long       max_blocks;   /* max. number of allocated blocks    */
    FT_Long       all_blocks;   /* total number of blocks allocated   */

    FT_Long       cur_आकार;     /* current cumulative allocated size */
    FT_Long       max_आकार;     /* maximum cumulative allocated size */
    FT_Long       all_आकार;     /* total cumulative allocated size   */

    FT_Long       cur_max;      /* current maximum allocated size */

    FT_UInt32     hash;
    FT_MemSource  link;

  } FT_MemSourceRec;


  /*
   * We don't need a resizable array for the memory sources because
   * their number is pretty limited within FreeType.
   */
#घोषणा FT_MEM_SOURCE_BUCKETS  128

  /*
   * This structure holds information related to a single allocated
   * memory block.  If KEEPALIVE is defined, blocks that are freed by
   * FreeType are never released to the system.  Instead, their `size'
   * field is set to `-size'.  This is mainly useful to detect double
   * frees, at the price of a large memory footprint during execution.
   */
  प्रकार काष्ठा  FT_Mem_गाँठ_Rec
  {
    FT_Byte*      address;
    FT_Long       आकार;     /* < 0 if the block was freed */

    FT_MemSource  source;

#अगर_घोषित KEEPALIVE
    स्थिर अक्षर*   free_file_name;
    FT_Long       free_पंक्ति_no;
#पूर्ण_अगर

    FT_Mem_गाँठ    link;

  } FT_Mem_गाँठ_Rec_2;


  /*
   * The global structure, containing compound statistics and all hash
   * tables.
   */
  प्रकार काष्ठा  FT_MemTableRec_
  {
    FT_Long          आकार;
    FT_Long          nodes;
    FT_Mem_गाँठ*      buckets;

    FT_Long          alloc_योग;
    FT_Long          alloc_current;
    FT_Long          alloc_max;
    FT_Long          alloc_गिनती;

    FT_Bool          bound_योग;
    FT_Long          alloc_योग_max;

    FT_Bool          bound_गिनती;
    FT_Long          alloc_गिनती_max;

    FT_MemSource     sources[FT_MEM_SOURCE_BUCKETS];

    FT_Bool          keep_alive;

    FT_Memory        memory;
    FT_Pointer       memory_user;
    FT_Alloc_Func    alloc;
    FT_Free_Func     मुक्त;
    FT_Realloc_Func  पुनः_स्मृति;

  } FT_MemTableRec;


#घोषणा FT_MEM_आकार_MAX  13845163

#घोषणा FT_FILENAME( x )  ( (x) ? (x) : "unknown file" )


  /*
   * Prime numbers are ugly to handle.  It would be better to implement
   * L-Hashing, which is 10% faster and doesn't require divisions.
   */
  अटल स्थिर FT_Int  ft_mem_primes[] =
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


  अटल FT_Long
  ft_mem_closest_अभाज्य( FT_Long  अंक )
  {
    माप_प्रकार  i;


    क्रम ( i = 0;
          i < माप ( ft_mem_primes ) / माप ( ft_mem_primes[0] ); i++ )
      अगर ( ft_mem_primes[i] > अंक )
        वापस ft_mem_primes[i];

    वापस FT_MEM_आकार_MAX;
  }


  अटल व्योम
  ft_mem_debug_panic( स्थिर अक्षर*  fmt,
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


  अटल FT_Pointer
  ft_mem_table_alloc( FT_MemTable  table,
                      FT_Long      आकार )
  {
    FT_Memory   memory = table->memory;
    FT_Pointer  block;


    memory->user = table->memory_user;
    block = table->alloc( memory, आकार );
    memory->user = table;

    वापस block;
  }


  अटल व्योम
  ft_mem_table_free( FT_MemTable  table,
                     FT_Pointer   block )
  {
    FT_Memory  memory = table->memory;


    memory->user = table->memory_user;
    table->मुक्त( memory, block );
    memory->user = table;
  }


  अटल व्योम
  ft_mem_table_resize( FT_MemTable  table )
  {
    FT_Long  new_आकार;


    new_आकार = ft_mem_closest_अभाज्य( table->nodes );
    अगर ( new_आकार != table->आकार )
    {
      FT_Mem_गाँठ*  new_buckets;
      FT_Long      i;


      new_buckets = (FT_Mem_गाँठ *)
                      ft_mem_table_alloc(
                        table,
                        new_आकार * (FT_Long)माप ( FT_Mem_गाँठ ) );
      अगर ( !new_buckets )
        वापस;

      FT_ARRAY_ZERO( new_buckets, new_आकार );

      क्रम ( i = 0; i < table->आकार; i++ )
      {
        FT_Mem_गाँठ  गाँठ, अग्र, *pnode;
        FT_PtrDist  hash;


        गाँठ = table->buckets[i];
        जबतक ( गाँठ )
        {
          अग्र  = गाँठ->link;
          hash  = FT_MEM_VAL( गाँठ->address ) % (FT_PtrDist)new_आकार;
          pnode = new_buckets + hash;

          गाँठ->link = pnode[0];
          pnode[0]   = गाँठ;

          गाँठ = अग्र;
        }
      }

      अगर ( table->buckets )
        ft_mem_table_free( table, table->buckets );

      table->buckets = new_buckets;
      table->आकार    = new_आकार;
    }
  }


  अटल व्योम
  ft_mem_table_destroy( FT_MemTable  table )
  {
    FT_Long  i;
    FT_Long  leak_गिनती = 0;
    FT_Long  leaks      = 0;


    /* remove all blocks from the table, revealing leaked ones */
    क्रम ( i = 0; i < table->आकार; i++ )
    {
      FT_Mem_गाँठ  *pnode = table->buckets + i, अग्र, गाँठ = *pnode;


      जबतक ( गाँठ )
      {
        अग्र       = गाँठ->link;
        गाँठ->link = शून्य;

        अगर ( गाँठ->आकार > 0 )
        {
          म_लिखो(
            "leaked memory block at address %p, size %8ld in (%s:%ld)\n",
            (व्योम*)गाँठ->address,
            गाँठ->आकार,
            FT_FILENAME( गाँठ->source->file_name ),
            गाँठ->source->पंक्ति_no );

          leak_गिनती++;
          leaks += गाँठ->आकार;

          ft_mem_table_free( table, गाँठ->address );
        }

        गाँठ->address = शून्य;
        गाँठ->आकार    = 0;

        ft_mem_table_free( table, गाँठ );
        गाँठ = अग्र;
      }
      table->buckets[i] = शून्य;
    }

    ft_mem_table_free( table, table->buckets );
    table->buckets = शून्य;

    table->आकार  = 0;
    table->nodes = 0;

    /* remove all sources */
    क्रम ( i = 0; i < FT_MEM_SOURCE_BUCKETS; i++ )
    {
      FT_MemSource  source, अग्र;


      क्रम ( source = table->sources[i]; source != शून्य; source = अग्र )
      {
        अग्र = source->link;
        ft_mem_table_free( table, source );
      }

      table->sources[i] = शून्य;
    }

    म_लिखो( "FreeType: total memory allocations = %ld\n",
            table->alloc_योग );
    म_लिखो( "FreeType: maximum memory footprint = %ld\n",
            table->alloc_max );

    अगर ( leak_गिनती > 0 )
      ft_mem_debug_panic(
        "FreeType: %ld bytes of memory leaked in %ld blocks\n",
        leaks, leak_गिनती );

    म_लिखो( "FreeType: no memory leaks detected\n" );
  }


  अटल FT_Mem_गाँठ*
  ft_mem_table_get_nodep( FT_MemTable  table,
                          FT_Byte*     address )
  {
    FT_PtrDist   hash;
    FT_Mem_गाँठ  *pnode, गाँठ;


    hash  = FT_MEM_VAL( address );
    pnode = table->buckets + ( hash % (FT_PtrDist)table->आकार );

    क्रम (;;)
    {
      गाँठ = pnode[0];
      अगर ( !गाँठ )
        अवरोध;

      अगर ( गाँठ->address == address )
        अवरोध;

      pnode = &गाँठ->link;
    }
    वापस pnode;
  }


  अटल FT_MemSource
  ft_mem_table_get_source( FT_MemTable  table )
  {
    FT_UInt32     hash;
    FT_MemSource  गाँठ, *pnode;


    /* cast to FT_PtrDist first since void* can be larger */
    /* than FT_UInt32 and GCC 4.1.1 emits a warning       */
    hash  = (FT_UInt32)(FT_PtrDist)(व्योम*)ft_debug_file_ +
              (FT_UInt32)( 5 * ft_debug_lineno_ );
    pnode = &table->sources[hash % FT_MEM_SOURCE_BUCKETS];

    क्रम (;;)
    {
      गाँठ = *pnode;
      अगर ( !गाँठ )
        अवरोध;

      अगर ( गाँठ->file_name == ft_debug_file_   &&
           गाँठ->पंक्ति_no   == ft_debug_lineno_ )
        जाओ Exit;

      pnode = &गाँठ->link;
    }

    गाँठ = (FT_MemSource)ft_mem_table_alloc( table, माप ( *गाँठ ) );
    अगर ( !गाँठ )
      ft_mem_debug_panic(
        "not enough memory to perform memory debugging\n" );

    गाँठ->file_name = ft_debug_file_;
    गाँठ->पंक्ति_no   = ft_debug_lineno_;

    गाँठ->cur_blocks = 0;
    गाँठ->max_blocks = 0;
    गाँठ->all_blocks = 0;

    गाँठ->cur_आकार = 0;
    गाँठ->max_आकार = 0;
    गाँठ->all_आकार = 0;

    गाँठ->cur_max = 0;

    गाँठ->link = शून्य;
    गाँठ->hash = hash;
    *pnode     = गाँठ;

  Exit:
    वापस गाँठ;
  }


  अटल व्योम
  ft_mem_table_set( FT_MemTable  table,
                    FT_Byte*     address,
                    FT_Long      आकार,
                    FT_Long      delta )
  {
    FT_Mem_गाँठ  *pnode, गाँठ;


    अगर ( table )
    {
      FT_MemSource  source;


      pnode = ft_mem_table_get_nodep( table, address );
      गाँठ  = *pnode;
      अगर ( गाँठ )
      {
        अगर ( गाँठ->आकार < 0 )
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
            गाँठ->address, गाँठ->आकार,
            FT_FILENAME( गाँठ->source->file_name ), गाँठ->source->पंक्ति_no,
            FT_FILENAME( ft_debug_file_ ), ft_debug_lineno_ );
        }
      }

      /* we need to create a new node in this table */
      गाँठ = (FT_Mem_गाँठ)ft_mem_table_alloc( table, माप ( *गाँठ ) );
      अगर ( !गाँठ )
        ft_mem_debug_panic( "not enough memory to run memory tests" );

      गाँठ->address = address;
      गाँठ->आकार    = आकार;
      गाँठ->source  = source = ft_mem_table_get_source( table );

      अगर ( delta == 0 )
      {
        /* this is an allocation */
        source->all_blocks++;
        source->cur_blocks++;
        अगर ( source->cur_blocks > source->max_blocks )
          source->max_blocks = source->cur_blocks;
      }

      अगर ( आकार > source->cur_max )
        source->cur_max = आकार;

      अगर ( delta != 0 )
      {
        /* we are growing or shrinking a reallocated block */
        source->cur_आकार     += delta;
        table->alloc_current += delta;
      }
      अन्यथा
      {
        /* we are allocating a new block */
        source->cur_आकार     += आकार;
        table->alloc_current += आकार;
      }

      source->all_आकार += आकार;

      अगर ( source->cur_आकार > source->max_आकार )
        source->max_आकार = source->cur_आकार;

      गाँठ->free_file_name = शून्य;
      गाँठ->free_पंक्ति_no   = 0;

      गाँठ->link = pnode[0];

      pnode[0] = गाँठ;
      table->nodes++;

      table->alloc_योग += आकार;

      अगर ( table->alloc_current > table->alloc_max )
        table->alloc_max = table->alloc_current;

      अगर ( table->nodes * 3 < table->आकार  ||
           table->आकार  * 3 < table->nodes )
        ft_mem_table_resize( table );
    }
  }


  अटल व्योम
  ft_mem_table_remove( FT_MemTable  table,
                       FT_Byte*     address,
                       FT_Long      delta )
  {
    अगर ( table )
    {
      FT_Mem_गाँठ  *pnode, गाँठ;


      pnode = ft_mem_table_get_nodep( table, address );
      गाँठ  = *pnode;
      अगर ( गाँठ )
      {
        FT_MemSource  source;


        अगर ( गाँठ->आकार < 0 )
          ft_mem_debug_panic(
            "freeing memory block at %p more than once\n"
            "  at (%s:%ld)!\n"
            "  Block was allocated at (%s:%ld)\n"
            "  and released at (%s:%ld).",
            address,
            FT_FILENAME( ft_debug_file_ ), ft_debug_lineno_,
            FT_FILENAME( गाँठ->source->file_name ), गाँठ->source->पंक्ति_no,
            FT_FILENAME( गाँठ->free_file_name ), गाँठ->free_पंक्ति_no );

        /* scramble the node's content for additional safety */
        FT_MEM_SET( address, 0xF3, गाँठ->आकार );

        अगर ( delta == 0 )
        {
          source = गाँठ->source;

          source->cur_blocks--;
          source->cur_आकार -= गाँठ->आकार;

          table->alloc_current -= गाँठ->आकार;
        }

        अगर ( table->keep_alive )
        {
          /* we simply invert the node's size to indicate that the node */
          /* was freed.                                                 */
          गाँठ->आकार           = -गाँठ->आकार;
          गाँठ->free_file_name = ft_debug_file_;
          गाँठ->free_पंक्ति_no   = ft_debug_lineno_;
        }
        अन्यथा
        {
          table->nodes--;

          *pnode = गाँठ->link;

          गाँठ->आकार   = 0;
          गाँठ->source = शून्य;

          ft_mem_table_free( table, गाँठ );

          अगर ( table->nodes * 3 < table->आकार  ||
               table->आकार  * 3 < table->nodes )
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


  अटल FT_Pointer
  ft_mem_debug_alloc( FT_Memory  memory,
                      FT_Long    आकार )
  {
    FT_MemTable  table = (FT_MemTable)memory->user;
    FT_Byte*     block;


    अगर ( आकार <= 0 )
      ft_mem_debug_panic( "negative block size allocation (%ld)", आकार );

    /* return NULL if the maximum number of allocations was reached */
    अगर ( table->bound_गिनती                           &&
         table->alloc_गिनती >= table->alloc_गिनती_max )
      वापस शून्य;

    /* return NULL if this allocation would overflow the maximum heap size */
    अगर ( table->bound_योग                                   &&
         table->alloc_योग_max - table->alloc_current > आकार )
      वापस शून्य;

    block = (FT_Byte *)ft_mem_table_alloc( table, आकार );
    अगर ( block )
    {
      ft_mem_table_set( table, block, आकार, 0 );

      table->alloc_गिनती++;
    }

    ft_debug_file_   = "<unknown>";
    ft_debug_lineno_ = 0;

    वापस (FT_Pointer)block;
  }


  अटल व्योम
  ft_mem_debug_free( FT_Memory   memory,
                     FT_Pointer  block )
  {
    FT_MemTable  table = (FT_MemTable)memory->user;


    अगर ( !block )
      ft_mem_debug_panic( "trying to free NULL in (%s:%ld)",
                          FT_FILENAME( ft_debug_file_ ),
                          ft_debug_lineno_ );

    ft_mem_table_remove( table, (FT_Byte*)block, 0 );

    अगर ( !table->keep_alive )
      ft_mem_table_free( table, block );

    table->alloc_गिनती--;

    ft_debug_file_   = "<unknown>";
    ft_debug_lineno_ = 0;
  }


  अटल FT_Pointer
  ft_mem_debug_realloc( FT_Memory   memory,
                        FT_Long     cur_आकार,
                        FT_Long     new_आकार,
                        FT_Pointer  block )
  {
    FT_MemTable  table = (FT_MemTable)memory->user;
    FT_Mem_गाँठ   गाँठ, *pnode;
    FT_Pointer   new_block;
    FT_Long      delta;

    स्थिर अक्षर*  file_name = FT_FILENAME( ft_debug_file_ );
    FT_Long      पंक्ति_no   = ft_debug_lineno_;


    /* unlikely, but possible */
    अगर ( new_आकार == cur_आकार )
      वापस block;

    /* the following is valid according to ANSI C */
#अगर 0
    अगर ( !block || !cur_आकार )
      ft_mem_debug_panic( "trying to reallocate NULL in (%s:%ld)",
                          file_name, पंक्ति_no );
#पूर्ण_अगर

    /* while the following is allowed in ANSI C also, we abort since */
    /* such case should be handled by FreeType.                      */
    अगर ( new_आकार <= 0 )
      ft_mem_debug_panic(
        "trying to reallocate %p to size 0 (current is %ld) in (%s:%ld)",
        block, cur_आकार, file_name, पंक्ति_no );

    /* check `cur_size' value */
    pnode = ft_mem_table_get_nodep( table, (FT_Byte*)block );
    गाँठ  = *pnode;
    अगर ( !गाँठ )
      ft_mem_debug_panic(
        "trying to reallocate unknown block at %p in (%s:%ld)",
        block, file_name, पंक्ति_no );

    अगर ( गाँठ->आकार <= 0 )
      ft_mem_debug_panic(
        "trying to reallocate freed block at %p in (%s:%ld)",
        block, file_name, पंक्ति_no );

    अगर ( गाँठ->आकार != cur_आकार )
      ft_mem_debug_panic( "invalid ft_realloc request for %p. cur_size is "
                          "%ld instead of %ld in (%s:%ld)",
                          block, cur_आकार, गाँठ->आकार, file_name, पंक्ति_no );

    /* return NULL if the maximum number of allocations was reached */
    अगर ( table->bound_गिनती                           &&
         table->alloc_गिनती >= table->alloc_गिनती_max )
      वापस शून्य;

    delta = new_आकार - cur_आकार;

    /* return NULL if this allocation would overflow the maximum heap size */
    अगर ( delta > 0                                             &&
         table->bound_योग                                    &&
         table->alloc_current + delta > table->alloc_योग_max )
      वापस शून्य;

    new_block = (FT_Pointer)ft_mem_table_alloc( table, new_आकार );
    अगर ( !new_block )
      वापस शून्य;

    ft_mem_table_set( table, (FT_Byte*)new_block, new_आकार, delta );

    ft_memcpy( new_block, block, cur_आकार < new_आकार ? (माप_प्रकार)cur_आकार
                                                     : (माप_प्रकार)new_आकार );

    ft_mem_table_remove( table, (FT_Byte*)block, delta );

    ft_debug_file_   = "<unknown>";
    ft_debug_lineno_ = 0;

    अगर ( !table->keep_alive )
      ft_mem_table_free( table, block );

    वापस new_block;
  }


  बाह्य व्योम
  ft_mem_debug_init( FT_Memory  memory )
  {
    FT_MemTable  table;


    अगर ( !ft_getenv( "FT2_DEBUG_MEMORY" ) )
      वापस;

    table = (FT_MemTable)memory->alloc( memory, माप ( *table ) );

    अगर ( table )
    {
      FT_ZERO( table );

      table->memory      = memory;
      table->memory_user = memory->user;
      table->alloc       = memory->alloc;
      table->पुनः_स्मृति     = memory->पुनः_स्मृति;
      table->मुक्त        = memory->मुक्त;

      ft_mem_table_resize( table );

      अगर ( table->आकार )
      {
        स्थिर अक्षर*  p;


        memory->user    = table;
        memory->alloc   = ft_mem_debug_alloc;
        memory->पुनः_स्मृति = ft_mem_debug_realloc;
        memory->मुक्त    = ft_mem_debug_free;

        p = ft_getenv( "FT2_ALLOC_TOTAL_MAX" );
        अगर ( p )
        {
          FT_Long  योग_max = ft_strtol( p, शून्य, 10 );


          अगर ( योग_max > 0 )
          {
            table->bound_योग     = 1;
            table->alloc_योग_max = योग_max;
          }
        }

        p = ft_getenv( "FT2_ALLOC_COUNT_MAX" );
        अगर ( p )
        {
          FT_Long  योग_गिनती = ft_strtol( p, शून्य, 10 );


          अगर ( योग_गिनती > 0 )
          {
            table->bound_गिनती     = 1;
            table->alloc_गिनती_max = योग_गिनती;
          }
        }

        p = ft_getenv( "FT2_KEEP_ALIVE" );
        अगर ( p )
        {
          FT_Long  keep_alive = ft_strtol( p, शून्य, 10 );


          अगर ( keep_alive > 0 )
            table->keep_alive = 1;
        }
      }
      अन्यथा
        memory->मुक्त( memory, table );
    }
  }


  बाह्य व्योम
  ft_mem_debug_done( FT_Memory  memory )
  {
    अगर ( memory->मुक्त == ft_mem_debug_free )
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
  ft_mem_source_compare( स्थिर व्योम*  p1,
                         स्थिर व्योम*  p2 )
  {
    FT_MemSource  s1 = *(FT_MemSource*)p1;
    FT_MemSource  s2 = *(FT_MemSource*)p2;


    अगर ( s2->max_आकार > s1->max_आकार )
      वापस 1;
    अन्यथा अगर ( s2->max_आकार < s1->max_आकार )
      वापस -1;
    अन्यथा
      वापस 0;
  }


  बाह्य व्योम
  FT_DumpMemory( FT_Memory  memory )
  {
    अगर ( memory->मुक्त == ft_mem_debug_free )
    {
      FT_MemTable    table = (FT_MemTable)memory->user;
      FT_MemSource*  bucket = table->sources;
      FT_MemSource*  limit  = bucket + FT_MEM_SOURCE_BUCKETS;
      FT_MemSource*  sources;
      FT_Int         nn, गिनती;
      स्थिर अक्षर*    fmt;


      गिनती = 0;
      क्रम ( ; bucket < limit; bucket++ )
      {
        FT_MemSource  source = *bucket;


        क्रम ( ; source; source = source->link )
          गिनती++;
      }

      sources = (FT_MemSource*)
                  ft_mem_table_alloc(
                    table, गिनती * (FT_Long)माप ( *sources ) );

      गिनती = 0;
      क्रम ( bucket = table->sources; bucket < limit; bucket++ )
      {
        FT_MemSource  source = *bucket;


        क्रम ( ; source; source = source->link )
          sources[गिनती++] = source;
      }

      ft_qsort( sources,
                (माप_प्रकार)गिनती,
                माप ( *sources ),
                ft_mem_source_compare );

      म_लिखो( "FreeType Memory Dump: "
              "current=%ld max=%ld total=%ld count=%ld\n",
              table->alloc_current, table->alloc_max,
              table->alloc_योग, table->alloc_गिनती );
      म_लिखो( " block  block    sizes    sizes    sizes   source\n" );
      म_लिखो( " count   high      sum  highsum      max   location\n" );
      म_लिखो( "-------------------------------------------------\n" );

      fmt = "%6ld %6ld %8ld %8ld %8ld %s:%d\n";

      क्रम ( nn = 0; nn < गिनती; nn++ )
      {
        FT_MemSource  source = sources[nn];


        म_लिखो( fmt,
                source->cur_blocks, source->max_blocks,
                source->cur_आकार, source->max_आकार, source->cur_max,
                FT_FILENAME( source->file_name ),
                source->पंक्ति_no );
      }
      म_लिखो( "------------------------------------------------\n" );

      ft_mem_table_free( table, sources );
    }
  }

#अन्यथा  /* !FT_DEBUG_MEMORY */

  /* ANSI C doesn't like empty source files */
  प्रकार पूर्णांक  debug_mem_dummy_;

#पूर्ण_अगर /* !FT_DEBUG_MEMORY */


/* END */
