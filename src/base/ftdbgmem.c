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


#यदि_परिभाषितम् FT_DEBUG_स्मृतिः

#परिभाषय  KEEPALIVE /* `Keep alive' means that freed blocks aren't released
                    * to the heap.  This is useful to detect double-frees
                    * or weird heap corruption, but it uses large amounts of
                    * memory, however.
                    */

#अन्तर्भाव FT_CONFIG_STANDARD_LIBRARY_H

  FT_आधारः_DEF( ध्रुव वर्ण* )  ft_debug_file_   = रिक्त;
  FT_आधारः_DEF( दीर्घ )         ft_debug_lineno_ = 0;

  बाह्य शून्य
  FT_Dump_स्मृतिः( FT_स्मृतिः  स्मृतिः );


  प्रकारनिर्देश संरचना FT_MemSourceRec_*  FT_MemSource;
  प्रकारनिर्देश संरचना FT_Mem_ग्रन्थिः_Rec*    FT_Mem_ग्रन्थिः;
  प्रकारनिर्देश संरचना FT_Mem_सारणी_Rec*   FT_Mem_सारणी;


#परिभाषय FT_MEM_VAL( addr )  ( (FT_PtrDist)(FT_Pointer)( addr ) )

  /*
   * This structure holds statistics for a single allocation/release
   * site.  This is useful to know where memory operations happen the
   * most.
   */
  प्रकारनिर्देश संरचना  FT_MemSourceRec_
  {
    ध्रुव वर्ण*   file_name;
    दीर्घ          पङ्क्तिः_no;

    FT_Long       cur_खण्डानि;   /* current number of allocated blocks */
    FT_Long       max_खण्डानि;   /* max. number of allocated blocks    */
    FT_Long       all_खण्डानि;   /* total number of blocks allocated   */

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
#परिभाषय FT_MEM_SOURCE_BUCKETS  128

  /*
   * This structure holds information related to a single allocated
   * memory block.  If KEEPALIVE is defined, blocks that are freed by
   * FreeType are never released to the system.  Instead, their `size'
   * field is set to `-size'.  This is mainly useful to detect double
   * frees, at the price of a large memory footprint during execution.
   */
  प्रकारनिर्देश संरचना  FT_Mem_ग्रन्थिः_Rec
  {
    FT_Byte*      स्थानाङ्कः;
    FT_Long       आकार;     /* < 0 if the block was freed */

    FT_MemSource  source;

#यदि_परिभाषितम् KEEPALIVE
    ध्रुव वर्ण*   free_file_name;
    FT_Long       free_पङ्क्तिः_no;
#यदि_अन्तः

    FT_Mem_ग्रन्थिः    link;

  } FT_Mem_ग्रन्थिः_Rec_2;


  /*
   * The global structure, containing compound statistics and all hash
   * tables.
   */
  प्रकारनिर्देश संरचना  FT_Mem_सारणी_Rec
  {
    FT_Long          आकार;
    FT_Long          nodes;
    FT_Mem_ग्रन्थिः*      buckets;

    FT_Long          alloc_योग;
    FT_Long          alloc_current;
    FT_Long          alloc_max;
    FT_Long          alloc_गणनम्;

    FT_Bool          bound_योग;
    FT_Long          alloc_योग_max;

    FT_Bool          bound_गणनम्;
    FT_Long          alloc_गणनम्_max;

    FT_MemSource     sources[FT_MEM_SOURCE_BUCKETS];

    FT_Bool          keep_alive;

    FT_स्मृतिः        स्मृतिः;
    FT_Pointer       स्मृतिः_user;
    FT_Alloc_Func    alloc;
    FT_Free_Func     मुक्त;
    FT_Realloc_Func  realloc;

  } FT_Mem_सारणी_Rec_2;


#परिभाषय FT_MEM_आकार_MAX  13845163

#परिभाषय FT_FILENAME( x )  ( (x) ? (x) : "unknown file" )


  /*
   * Prime numbers are ugly to handle.  It would be better to implement
   * L-Hashing, which is 10% faster and doesn't require divisions.
   */
  स्थिर ध्रुव FT_Int  ft_mem_अभाज्याः[] =
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
          i < परिमाण ( ft_mem_अभाज्याः ) / परिमाण ( ft_mem_अभाज्याः[0] ); i++ )
      यदि ( ft_mem_अभाज्याः[i] > अङ्कः )
        प्रत्यागम ft_mem_अभाज्याः[i];

    प्रत्यागम FT_MEM_आकार_MAX;
  }


  स्थिर शून्य
  ft_mem_debug_panic( ध्रुव वर्ण*  fmt,
                      ... )
  {
    va_list  ap;


    प्रारूप_लिख( "FreeType.Debug: " );

    va_start( ap, fmt );
    vprintf( fmt, ap );
    va_अन्तः( ap );

    प्रारूप_लिख( "\n" );
    निर्गच्छ( EXIT_FAILURE );
  }


  स्थिर FT_Pointer
  ft_mem_सारणी_alloc( FT_Mem_सारणी  सारणी,
                      FT_Long      आकार )
  {
    FT_स्मृतिः   स्मृतिः = सारणी->स्मृतिः;
    FT_Pointer  खण्डम्;


    स्मृतिः->user = सारणी->स्मृतिः_user;
    खण्डम् = सारणी->alloc( स्मृतिः, आकार );
    स्मृतिः->user = सारणी;

    प्रत्यागम खण्डम्;
  }


  स्थिर शून्य
  ft_mem_सारणी_free( FT_Mem_सारणी  सारणी,
                     FT_Pointer   खण्डम् )
  {
    FT_स्मृतिः  स्मृतिः = सारणी->स्मृतिः;


    स्मृतिः->user = सारणी->स्मृतिः_user;
    सारणी->मुक्त( स्मृतिः, खण्डम् );
    स्मृतिः->user = सारणी;
  }


  स्थिर शून्य
  ft_mem_सारणी_resize( FT_Mem_सारणी  सारणी )
  {
    FT_Long  new_आकार;


    new_आकार = ft_mem_closest_अविभाज्यः( सारणी->nodes );
    यदि ( new_आकार != सारणी->आकार )
    {
      FT_Mem_ग्रन्थिः*  new_buckets;
      FT_Long      i;


      new_buckets = (FT_Mem_ग्रन्थिः *)
                      ft_mem_सारणी_alloc(
                        सारणी,
                        new_आकार * (FT_Long)परिमाण ( FT_Mem_ग्रन्थिः ) );
      यदि ( !new_buckets )
        प्रत्यागम;

      FT_ARRAY_ZERO( new_buckets, new_आकार );

      प्रति ( i = 0; i < सारणी->आकार; i++ )
      {
        FT_Mem_ग्रन्थिः  ग्रन्थिः, अग्र, *pnode;
        FT_PtrDist  hash;


        ग्रन्थिः = सारणी->buckets[i];
        यावत् ( ग्रन्थिः )
        {
          अग्र  = ग्रन्थिः->link;
          hash  = FT_MEM_VAL( ग्रन्थिः->स्थानाङ्कः ) % (FT_PtrDist)new_आकार;
          pnode = new_buckets + hash;

          ग्रन्थिः->link = pnode[0];
          pnode[0]   = ग्रन्थिः;

          ग्रन्थिः = अग्र;
        }
      }

      यदि ( सारणी->buckets )
        ft_mem_सारणी_free( सारणी, सारणी->buckets );

      सारणी->buckets = new_buckets;
      सारणी->आकार    = new_आकार;
    }
  }


  स्थिर शून्य
  ft_mem_सारणी_destroy( FT_Mem_सारणी  सारणी )
  {
    FT_Long  i;
    FT_Long  leak_गणनम् = 0;
    FT_Long  leaks      = 0;


    /* remove all blocks from the table, revealing leaked ones */
    प्रति ( i = 0; i < सारणी->आकार; i++ )
    {
      FT_Mem_ग्रन्थिः  *pnode = सारणी->buckets + i, अग्र, ग्रन्थिः = *pnode;


      यावत् ( ग्रन्थिः )
      {
        अग्र       = ग्रन्थिः->link;
        ग्रन्थिः->link = रिक्त;

        यदि ( ग्रन्थिः->आकार > 0 )
        {
          प्रारूप_लिख(
            "leaked memory block at address %p, size %8ld in (%s:%ld)\n",
            (शून्य*)ग्रन्थिः->स्थानाङ्कः,
            ग्रन्थिः->आकार,
            FT_FILENAME( ग्रन्थिः->source->file_name ),
            ग्रन्थिः->source->पङ्क्तिः_no );

          leak_गणनम्++;
          leaks += ग्रन्थिः->आकार;

          ft_mem_सारणी_free( सारणी, ग्रन्थिः->स्थानाङ्कः );
        }

        ग्रन्थिः->स्थानाङ्कः = रिक्त;
        ग्रन्थिः->आकार    = 0;

        ft_mem_सारणी_free( सारणी, ग्रन्थिः );
        ग्रन्थिः = अग्र;
      }
      सारणी->buckets[i] = रिक्त;
    }

    ft_mem_सारणी_free( सारणी, सारणी->buckets );
    सारणी->buckets = रिक्त;

    सारणी->आकार  = 0;
    सारणी->nodes = 0;

    /* remove all sources */
    प्रति ( i = 0; i < FT_MEM_SOURCE_BUCKETS; i++ )
    {
      FT_MemSource  source, अग्र;


      प्रति ( source = सारणी->sources[i]; source != रिक्त; source = अग्र )
      {
        अग्र = source->link;
        ft_mem_सारणी_free( सारणी, source );
      }

      सारणी->sources[i] = रिक्त;
    }

    प्रारूप_लिख( "FreeType: total memory allocations = %ld\n",
            सारणी->alloc_योग );
    प्रारूप_लिख( "FreeType: maximum memory footprint = %ld\n",
            सारणी->alloc_max );

    यदि ( leak_गणनम् > 0 )
      ft_mem_debug_panic(
        "FreeType: %ld bytes of memory leaked in %ld blocks\n",
        leaks, leak_गणनम् );

    प्रारूप_लिख( "FreeType: no memory leaks detected\n" );
  }


  स्थिर FT_Mem_ग्रन्थिः*
  ft_mem_सारणी_get_nodep( FT_Mem_सारणी  सारणी,
                          FT_Byte*     स्थानाङ्कः )
  {
    FT_PtrDist   hash;
    FT_Mem_ग्रन्थिः  *pnode, ग्रन्थिः;


    hash  = FT_MEM_VAL( स्थानाङ्कः );
    pnode = सारणी->buckets + ( hash % (FT_PtrDist)सारणी->आकार );

    प्रति (;;)
    {
      ग्रन्थिः = pnode[0];
      यदि ( !ग्रन्थिः )
        भङ्ग;

      यदि ( ग्रन्थिः->स्थानाङ्कः == स्थानाङ्कः )
        भङ्ग;

      pnode = &ग्रन्थिः->link;
    }
    प्रत्यागम pnode;
  }


  स्थिर FT_MemSource
  ft_mem_सारणी_get_source( FT_Mem_सारणी  सारणी )
  {
    FT_UInt32     hash;
    FT_MemSource  ग्रन्थिः, *pnode;


    /* cast to FT_PtrDist first since void* can be larger */
    /* than FT_UInt32 and GCC 4.1.1 emits a warning       */
    hash  = (FT_UInt32)(FT_PtrDist)(शून्य*)ft_debug_file_ +
              (FT_UInt32)( 5 * ft_debug_lineno_ );
    pnode = &सारणी->sources[hash % FT_MEM_SOURCE_BUCKETS];

    प्रति (;;)
    {
      ग्रन्थिः = *pnode;
      यदि ( !ग्रन्थिः )
        भङ्ग;

      यदि ( ग्रन्थिः->file_name == ft_debug_file_   &&
           ग्रन्थिः->पङ्क्तिः_no   == ft_debug_lineno_ )
        गच्छ Exit;

      pnode = &ग्रन्थिः->link;
    }

    ग्रन्थिः = (FT_MemSource)ft_mem_सारणी_alloc( सारणी, परिमाण ( *ग्रन्थिः ) );
    यदि ( !ग्रन्थिः )
      ft_mem_debug_panic(
        "not enough memory to perform memory debugging\n" );

    ग्रन्थिः->file_name = ft_debug_file_;
    ग्रन्थिः->पङ्क्तिः_no   = ft_debug_lineno_;

    ग्रन्थिः->cur_खण्डानि = 0;
    ग्रन्थिः->max_खण्डानि = 0;
    ग्रन्थिः->all_खण्डानि = 0;

    ग्रन्थिः->cur_आकार = 0;
    ग्रन्थिः->max_आकार = 0;
    ग्रन्थिः->all_आकार = 0;

    ग्रन्थिः->cur_max = 0;

    ग्रन्थिः->link = रिक्त;
    ग्रन्थिः->hash = hash;
    *pnode     = ग्रन्थिः;

  Exit:
    प्रत्यागम ग्रन्थिः;
  }


  स्थिर शून्य
  ft_mem_सारणी_समुच्चयः( FT_Mem_सारणी  सारणी,
                    FT_Byte*     स्थानाङ्कः,
                    FT_Long      आकार,
                    FT_Long      delta )
  {
    FT_Mem_ग्रन्थिः  *pnode, ग्रन्थिः;


    यदि ( सारणी )
    {
      FT_MemSource  source;


      pnode = ft_mem_सारणी_get_nodep( सारणी, स्थानाङ्कः );
      ग्रन्थिः  = *pnode;
      यदि ( ग्रन्थिः )
      {
        यदि ( ग्रन्थिः->आकार < 0 )
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
            ग्रन्थिः->स्थानाङ्कः, ग्रन्थिः->आकार,
            FT_FILENAME( ग्रन्थिः->source->file_name ), ग्रन्थिः->source->पङ्क्तिः_no,
            FT_FILENAME( ft_debug_file_ ), ft_debug_lineno_ );
        }
      }

      /* we need to create a new node in this table */
      ग्रन्थिः = (FT_Mem_ग्रन्थिः)ft_mem_सारणी_alloc( सारणी, परिमाण ( *ग्रन्थिः ) );
      यदि ( !ग्रन्थिः )
        ft_mem_debug_panic( "not enough memory to run memory tests" );

      ग्रन्थिः->स्थानाङ्कः = स्थानाङ्कः;
      ग्रन्थिः->आकार    = आकार;
      ग्रन्थिः->source  = source = ft_mem_सारणी_get_source( सारणी );

      यदि ( delta == 0 )
      {
        /* this is an allocation */
        source->all_खण्डानि++;
        source->cur_खण्डानि++;
        यदि ( source->cur_खण्डानि > source->max_खण्डानि )
          source->max_खण्डानि = source->cur_खण्डानि;
      }

      यदि ( आकार > source->cur_max )
        source->cur_max = आकार;

      यदि ( delta != 0 )
      {
        /* we are growing or shrinking a reallocated block */
        source->cur_आकार     += delta;
        सारणी->alloc_current += delta;
      }
      अन्यथा
      {
        /* we are allocating a new block */
        source->cur_आकार     += आकार;
        सारणी->alloc_current += आकार;
      }

      source->all_आकार += आकार;

      यदि ( source->cur_आकार > source->max_आकार )
        source->max_आकार = source->cur_आकार;

      ग्रन्थिः->free_file_name = रिक्त;
      ग्रन्थिः->free_पङ्क्तिः_no   = 0;

      ग्रन्थिः->link = pnode[0];

      pnode[0] = ग्रन्थिः;
      सारणी->nodes++;

      सारणी->alloc_योग += आकार;

      यदि ( सारणी->alloc_current > सारणी->alloc_max )
        सारणी->alloc_max = सारणी->alloc_current;

      यदि ( सारणी->nodes * 3 < सारणी->आकार  ||
           सारणी->आकार  * 3 < सारणी->nodes )
        ft_mem_सारणी_resize( सारणी );
    }
  }


  स्थिर शून्य
  ft_mem_सारणी_remove( FT_Mem_सारणी  सारणी,
                       FT_Byte*     स्थानाङ्कः,
                       FT_Long      delta )
  {
    यदि ( सारणी )
    {
      FT_Mem_ग्रन्थिः  *pnode, ग्रन्थिः;


      pnode = ft_mem_सारणी_get_nodep( सारणी, स्थानाङ्कः );
      ग्रन्थिः  = *pnode;
      यदि ( ग्रन्थिः )
      {
        FT_MemSource  source;


        यदि ( ग्रन्थिः->आकार < 0 )
          ft_mem_debug_panic(
            "freeing memory block at %p more than once\n"
            "  at (%s:%ld)!\n"
            "  Block was allocated at (%s:%ld)\n"
            "  and released at (%s:%ld).",
            स्थानाङ्कः,
            FT_FILENAME( ft_debug_file_ ), ft_debug_lineno_,
            FT_FILENAME( ग्रन्थिः->source->file_name ), ग्रन्थिः->source->पङ्क्तिः_no,
            FT_FILENAME( ग्रन्थिः->free_file_name ), ग्रन्थिः->free_पङ्क्तिः_no );

        /* scramble the node's content for additional safety */
        FT_MEM_समुच्चयः( स्थानाङ्कः, 0xF3, ग्रन्थिः->आकार );

        यदि ( delta == 0 )
        {
          source = ग्रन्थिः->source;

          source->cur_खण्डानि--;
          source->cur_आकार -= ग्रन्थिः->आकार;

          सारणी->alloc_current -= ग्रन्थिः->आकार;
        }

        यदि ( सारणी->keep_alive )
        {
          /* we simply invert the node's size to indicate that the node */
          /* was freed.                                                 */
          ग्रन्थिः->आकार           = -ग्रन्थिः->आकार;
          ग्रन्थिः->free_file_name = ft_debug_file_;
          ग्रन्थिः->free_पङ्क्तिः_no   = ft_debug_lineno_;
        }
        अन्यथा
        {
          सारणी->nodes--;

          *pnode = ग्रन्थिः->link;

          ग्रन्थिः->आकार   = 0;
          ग्रन्थिः->source = रिक्त;

          ft_mem_सारणी_free( सारणी, ग्रन्थिः );

          यदि ( सारणी->nodes * 3 < सारणी->आकार  ||
               सारणी->आकार  * 3 < सारणी->nodes )
            ft_mem_सारणी_resize( सारणी );
        }
      }
      अन्यथा
        ft_mem_debug_panic(
          "trying to free unknown block at %p in (%s:%ld)\n",
          स्थानाङ्कः,
          FT_FILENAME( ft_debug_file_ ), ft_debug_lineno_ );
    }
  }


  स्थिर FT_Pointer
  ft_mem_debug_alloc( FT_स्मृतिः  स्मृतिः,
                      FT_Long    आकार )
  {
    FT_Mem_सारणी  सारणी = (FT_Mem_सारणी)स्मृतिः->user;
    FT_Byte*     खण्डम्;


    यदि ( आकार <= 0 )
      ft_mem_debug_panic( "negative block size allocation (%ld)", आकार );

    /* return NULL if the maximum number of allocations was reached */
    यदि ( सारणी->bound_गणनम्                           &&
         सारणी->alloc_गणनम् >= सारणी->alloc_गणनम्_max )
      प्रत्यागम रिक्त;

    /* return NULL if this allocation would overflow the maximum heap size */
    यदि ( सारणी->bound_योग                                   &&
         सारणी->alloc_योग_max - सारणी->alloc_current > आकार )
      प्रत्यागम रिक्त;

    खण्डम् = (FT_Byte *)ft_mem_सारणी_alloc( सारणी, आकार );
    यदि ( खण्डम् )
    {
      ft_mem_सारणी_समुच्चयः( सारणी, खण्डम्, आकार, 0 );

      सारणी->alloc_गणनम्++;
    }

    ft_debug_file_   = "<unknown>";
    ft_debug_lineno_ = 0;

    प्रत्यागम (FT_Pointer)खण्डम्;
  }


  स्थिर शून्य
  ft_mem_debug_free( FT_स्मृतिः   स्मृतिः,
                     FT_Pointer  खण्डम् )
  {
    FT_Mem_सारणी  सारणी = (FT_Mem_सारणी)स्मृतिः->user;


    यदि ( !खण्डम् )
      ft_mem_debug_panic( "trying to free NULL in (%s:%ld)",
                          FT_FILENAME( ft_debug_file_ ),
                          ft_debug_lineno_ );

    ft_mem_सारणी_remove( सारणी, (FT_Byte*)खण्डम्, 0 );

    यदि ( !सारणी->keep_alive )
      ft_mem_सारणी_free( सारणी, खण्डम् );

    सारणी->alloc_गणनम्--;

    ft_debug_file_   = "<unknown>";
    ft_debug_lineno_ = 0;
  }


  स्थिर FT_Pointer
  ft_mem_debug_realloc( FT_स्मृतिः   स्मृतिः,
                        FT_Long     cur_आकार,
                        FT_Long     new_आकार,
                        FT_Pointer  खण्डम् )
  {
    FT_Mem_सारणी  सारणी = (FT_Mem_सारणी)स्मृतिः->user;
    FT_Mem_ग्रन्थिः   ग्रन्थिः, *pnode;
    FT_Pointer   new_खण्डम्;
    FT_Long      delta;

    ध्रुव वर्ण*  file_name = FT_FILENAME( ft_debug_file_ );
    FT_Long      पङ्क्तिः_no   = ft_debug_lineno_;


    /* unlikely, but possible */
    यदि ( new_आकार == cur_आकार )
      प्रत्यागम खण्डम्;

    /* the following is valid according to ANSI C */
#यदि 0
    यदि ( !खण्डम् || !cur_आकार )
      ft_mem_debug_panic( "trying to reallocate NULL in (%s:%ld)",
                          file_name, पङ्क्तिः_no );
#यदि_अन्तः

    /* while the following is allowed in ANSI C also, we abort since */
    /* such case should be handled by FreeType.                      */
    यदि ( new_आकार <= 0 )
      ft_mem_debug_panic(
        "trying to reallocate %p to size 0 (current is %ld) in (%s:%ld)",
        खण्डम्, cur_आकार, file_name, पङ्क्तिः_no );

    /* check `cur_size' value */
    pnode = ft_mem_सारणी_get_nodep( सारणी, (FT_Byte*)खण्डम् );
    ग्रन्थिः  = *pnode;
    यदि ( !ग्रन्थिः )
      ft_mem_debug_panic(
        "trying to reallocate unknown block at %p in (%s:%ld)",
        खण्डम्, file_name, पङ्क्तिः_no );

    यदि ( ग्रन्थिः->आकार <= 0 )
      ft_mem_debug_panic(
        "trying to reallocate freed block at %p in (%s:%ld)",
        खण्डम्, file_name, पङ्क्तिः_no );

    यदि ( ग्रन्थिः->आकार != cur_आकार )
      ft_mem_debug_panic( "invalid ft_realloc request for %p. cur_size is "
                          "%ld instead of %ld in (%s:%ld)",
                          खण्डम्, cur_आकार, ग्रन्थिः->आकार, file_name, पङ्क्तिः_no );

    /* return NULL if the maximum number of allocations was reached */
    यदि ( सारणी->bound_गणनम्                           &&
         सारणी->alloc_गणनम् >= सारणी->alloc_गणनम्_max )
      प्रत्यागम रिक्त;

    delta = new_आकार - cur_आकार;

    /* return NULL if this allocation would overflow the maximum heap size */
    यदि ( delta > 0                                             &&
         सारणी->bound_योग                                    &&
         सारणी->alloc_current + delta > सारणी->alloc_योग_max )
      प्रत्यागम रिक्त;

    new_खण्डम् = (FT_Pointer)ft_mem_सारणी_alloc( सारणी, new_आकार );
    यदि ( !new_खण्डम् )
      प्रत्यागम रिक्त;

    ft_mem_सारणी_समुच्चयः( सारणी, (FT_Byte*)new_खण्डम्, new_आकार, delta );

    ft_memcpy( new_खण्डम्, खण्डम्, cur_आकार < new_आकार ? (size_t)cur_आकार
                                                     : (size_t)new_आकार );

    ft_mem_सारणी_remove( सारणी, (FT_Byte*)खण्डम्, delta );

    ft_debug_file_   = "<unknown>";
    ft_debug_lineno_ = 0;

    यदि ( !सारणी->keep_alive )
      ft_mem_सारणी_free( सारणी, खण्डम् );

    प्रत्यागम new_खण्डम्;
  }


  बाह्य शून्य
  ft_mem_debug_init( FT_स्मृतिः  स्मृतिः )
  {
    FT_Mem_सारणी  सारणी;


    यदि ( !ft_getenv( "FT2_DEBUG_MEMORY" ) )
      प्रत्यागम;

    सारणी = (FT_Mem_सारणी)स्मृतिः->alloc( स्मृतिः, परिमाण ( *सारणी ) );

    यदि ( सारणी )
    {
      FT_ZERO( सारणी );

      सारणी->स्मृतिः      = स्मृतिः;
      सारणी->स्मृतिः_user = स्मृतिः->user;
      सारणी->alloc       = स्मृतिः->alloc;
      सारणी->realloc     = स्मृतिः->realloc;
      सारणी->मुक्त        = स्मृतिः->मुक्त;

      ft_mem_सारणी_resize( सारणी );

      यदि ( सारणी->आकार )
      {
        ध्रुव वर्ण*  p;


        स्मृतिः->user    = सारणी;
        स्मृतिः->alloc   = ft_mem_debug_alloc;
        स्मृतिः->realloc = ft_mem_debug_realloc;
        स्मृतिः->मुक्त    = ft_mem_debug_free;

        p = ft_getenv( "FT2_ALLOC_TOTAL_MAX" );
        यदि ( p )
        {
          FT_Long  योग_max = ft_strtol( p, रिक्त, 10 );


          यदि ( योग_max > 0 )
          {
            सारणी->bound_योग     = 1;
            सारणी->alloc_योग_max = योग_max;
          }
        }

        p = ft_getenv( "FT2_ALLOC_COUNT_MAX" );
        यदि ( p )
        {
          FT_Long  योग_गणनम् = ft_strtol( p, रिक्त, 10 );


          यदि ( योग_गणनम् > 0 )
          {
            सारणी->bound_गणनम्     = 1;
            सारणी->alloc_गणनम्_max = योग_गणनम्;
          }
        }

        p = ft_getenv( "FT2_KEEP_ALIVE" );
        यदि ( p )
        {
          FT_Long  keep_alive = ft_strtol( p, रिक्त, 10 );


          यदि ( keep_alive > 0 )
            सारणी->keep_alive = 1;
        }
      }
      अन्यथा
        स्मृतिः->मुक्त( स्मृतिः, सारणी );
    }
  }


  बाह्य शून्य
  ft_mem_debug_समाप्तम्( FT_स्मृतिः  स्मृतिः )
  {
    यदि ( स्मृतिः->मुक्त == ft_mem_debug_free )
    {
      FT_Mem_सारणी  सारणी = (FT_Mem_सारणी)स्मृतिः->user;


      FT_Dump_स्मृतिः( स्मृतिः );

      ft_mem_सारणी_destroy( सारणी );

      स्मृतिः->मुक्त    = सारणी->मुक्त;
      स्मृतिः->realloc = सारणी->realloc;
      स्मृतिः->alloc   = सारणी->alloc;
      स्मृतिः->user    = सारणी->स्मृतिः_user;

      स्मृतिः->मुक्त( स्मृतिः, सारणी );
    }
  }


  FT_COMPARE_DEF( पूर्णाङ्क )
  ft_mem_source_compare( ध्रुव शून्य*  p1,
                         ध्रुव शून्य*  p2 )
  {
    FT_MemSource  s1 = *(FT_MemSource*)p1;
    FT_MemSource  s2 = *(FT_MemSource*)p2;


    यदि ( s2->max_आकार > s1->max_आकार )
      प्रत्यागम 1;
    अन्यथा यदि ( s2->max_आकार < s1->max_आकार )
      प्रत्यागम -1;
    अन्यथा
      प्रत्यागम 0;
  }


  बाह्य शून्य
  FT_Dump_स्मृतिः( FT_स्मृतिः  स्मृतिः )
  {
    यदि ( स्मृतिः->मुक्त == ft_mem_debug_free )
    {
      FT_Mem_सारणी    सारणी = (FT_Mem_सारणी)स्मृतिः->user;
      FT_MemSource*  bucket = सारणी->sources;
      FT_MemSource*  सीमा  = bucket + FT_MEM_SOURCE_BUCKETS;
      FT_MemSource*  sources;
      FT_Int         nn, गणनम्;
      ध्रुव वर्ण*    fmt;


      गणनम् = 0;
      प्रति ( ; bucket < सीमा; bucket++ )
      {
        FT_MemSource  source = *bucket;


        प्रति ( ; source; source = source->link )
          गणनम्++;
      }

      sources = (FT_MemSource*)
                  ft_mem_सारणी_alloc(
                    सारणी, गणनम् * (FT_Long)परिमाण ( *sources ) );

      गणनम् = 0;
      प्रति ( bucket = सारणी->sources; bucket < सीमा; bucket++ )
      {
        FT_MemSource  source = *bucket;


        प्रति ( ; source; source = source->link )
          sources[गणनम्++] = source;
      }

      ft_qsort( sources,
                (size_t)गणनम्,
                परिमाण ( *sources ),
                ft_mem_source_compare );

      प्रारूप_लिख( "FreeType Memory Dump: "
              "current=%ld max=%ld total=%ld count=%ld\n",
              सारणी->alloc_current, सारणी->alloc_max,
              सारणी->alloc_योग, सारणी->alloc_गणनम् );
      प्रारूप_लिख( " block  block    sizes    sizes    sizes   source\n" );
      प्रारूप_लिख( " count   high      sum  highsum      max   location\n" );
      प्रारूप_लिख( "-------------------------------------------------\n" );

      fmt = "%6ld %6ld %8ld %8ld %8ld %s:%d\n";

      प्रति ( nn = 0; nn < गणनम्; nn++ )
      {
        FT_MemSource  source = sources[nn];


        प्रारूप_लिख( fmt,
                source->cur_खण्डानि, source->max_खण्डानि,
                source->cur_आकार, source->max_आकार, source->cur_max,
                FT_FILENAME( source->file_name ),
                source->पङ्क्तिः_no );
      }
      प्रारूप_लिख( "------------------------------------------------\n" );

      ft_mem_सारणी_free( सारणी, sources );
    }
  }

#अन्यथा  /* !FT_DEBUG_MEMORY */

  /* ANSI C doesn't like empty source files */
  प्रकारनिर्देश पूर्णाङ्क  debug_mem_dummy_;

#यदि_अन्तः /* !FT_DEBUG_MEMORY */


/*अन्तः*/
