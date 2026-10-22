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


#ಅನ್ತರ್ಭಾವ <ft2build.h>
#ಅನ್ತರ್ಭಾವ FT_CONFIG_CONFIG_H
#ಅನ್ತರ್ಭಾವ <freetype/internal/ftdebug.h>
#ಅನ್ತರ್ಭಾವ <freetype/internal/ftmemory.h>
#ಅನ್ತರ್ಭಾವ <freetype/ftsystem.h>
#ಅನ್ತರ್ಭಾವ <freetype/fterrors.h>
#ಅನ್ತರ್ಭಾವ <freetype/fttypes.h>


#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ FT_DEBUG_ಸ್ಮರಣೆ

#ವ್ಯಾಖ್ಯಾನಿಸು  KEEPALIVE /* `Keep alive' means that freed blocks aren't released
                    * to the heap.  This is useful to detect double-frees
                    * or weird heap corruption, but it uses large amounts of
                    * memory, however.
                    */

#ಅನ್ತರ್ಭಾವ FT_CONFIG_STANDARD_LIBRARY_H

  FT_ತಳ_DEF( ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* )  ft_debug_file_   = ಖಾಲಿ;
  FT_ತಳ_DEF( ದೀರ್ಘ )         ft_debug_lineno_ = 0;

  ಬಾಹ್ಯ ಶೂನ್ಯ
  FT_Dump_ಸ್ಮರಣೆ( FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ );


  ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ FT_Mem_ಮೂಲ_Rec*  FT_Mem_ಮೂಲ;
  ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ FT_Mem_ಗಂಟು_Rec*    FT_Mem_ಗಂಟು;
  ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ FT_Mem_ಕೋಷ್ಟಕ_Rec*   FT_Mem_ಕೋಷ್ಟಕ;


#ವ್ಯಾಖ್ಯಾನಿಸು FT_MEM_VAL( addr )  ( (FT_PtrDist)(FT_ಸೂಚಕ)( addr ) )

  /*
   * This structure holds statistics for a single allocation/release
   * site.  This is useful to know where memory operations happen the
   * most.
   */
  ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ  FT_Mem_ಮೂಲ_Rec
  {
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*   file_name;
    ದೀರ್ಘ          ಸಾಲು_no;

    FT_Long       cur_ಖಂಡಗಳು;   /* current number of allocated blocks */
    FT_Long       max_ಖಂಡಗಳು;   /* max. number of allocated blocks    */
    FT_Long       all_ಖಂಡಗಳು;   /* total number of blocks allocated   */

    FT_Long       cur_ಗಾತ್ರ;     /* current cumulative allocated size */
    FT_Long       max_ಗಾತ್ರ;     /* maximum cumulative allocated size */
    FT_Long       all_ಗಾತ್ರ;     /* total cumulative allocated size   */

    FT_Long       cur_max;      /* current maximum allocated size */

    FT_UInt32     hash;
    FT_Mem_ಮೂಲ  link;

  } FT_Mem_ಮೂಲ_Rec_2;


  /*
   * We don't need a resizable array for the memory sources because
   * their number is pretty limited within FreeType.
   */
#ವ್ಯಾಖ್ಯಾನಿಸು FT_MEM_ಮೂಲ_BUCKETS  128

  /*
   * This structure holds information related to a single allocated
   * memory block.  If KEEPALIVE is defined, blocks that are freed by
   * FreeType are never released to the system.  Instead, their `size'
   * field is set to `-size'.  This is mainly useful to detect double
   * frees, at the price of a large memory footprint during execution.
   */
  ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ  FT_Mem_ಗಂಟು_Rec
  {
    FT_Byte*      ವಿಳಾಸ;
    FT_Long       ಗಾತ್ರ_2;     /* < 0 if the block was freed */

    FT_Mem_ಮೂಲ  ಮೂಲ;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ KEEPALIVE
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*   free_file_name;
    FT_Long       free_ಸಾಲು_no;
#ಆದರೆ_ಅಂತ್ಯ

    FT_Mem_ಗಂಟು    link;

  } FT_Mem_ಗಂಟು_Rec_2;


  /*
   * The global structure, containing compound statistics and all hash
   * tables.
   */
  ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ  FT_Mem_ಕೋಷ್ಟಕ_Rec
  {
    FT_Long          ಗಾತ್ರ_2;
    FT_Long          nodes;
    FT_Mem_ಗಂಟು*      buckets;

    FT_Long          alloc_ಒಟ್ಟು;
    FT_Long          alloc_current;
    FT_Long          alloc_max;
    FT_Long          alloc_ಎಣಿಕೆ;

    FT_Bool          bound_ಒಟ್ಟು;
    FT_Long          alloc_ಒಟ್ಟು_max;

    FT_Bool          bound_ಎಣಿಕೆ;
    FT_Long          alloc_ಎಣಿಕೆ_max;

    FT_Mem_ಮೂಲ     sources[FT_MEM_ಮೂಲ_BUCKETS];

    FT_Bool          keep_alive;

    FT_ಸ್ಮರಣೆ        ಸ್ಮರಣೆ;
    FT_ಸೂಚಕ       ಸ್ಮರಣೆ_user;
    FT_Alloc_Func    alloc;
    FT_Free_Func     ಬಿಡುಗಡೆ;
    FT_Realloc_Func  realloc;

  } FT_Mem_ಕೋಷ್ಟಕ_Rec_2;


#ವ್ಯಾಖ್ಯಾನಿಸು FT_MEM_ಗಾತ್ರ_MAX  13845163

#ವ್ಯಾಖ್ಯಾನಿಸು FT_FILENAME( x )  ( (x) ? (x) : "unknown file" )


  /*
   * Prime numbers are ugly to handle.  It would be better to implement
   * L-Hashing, which is 10% faster and doesn't require divisions.
   */
  ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ FT_Int  ft_mem_ಅವಿಭಾಜ್ಯಗಳು[] =
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


  ಸ್ಥಿರ FT_Long
  ft_mem_closest_ಅವಿಭಾಜ್ಯ( FT_Long  ಅಂಕೆ )
  {
    size_t  i;


    ಗಾಗಿ ( i = 0;
          i < ಗಾತ್ರ ( ft_mem_ಅವಿಭಾಜ್ಯಗಳು ) / ಗಾತ್ರ ( ft_mem_ಅವಿಭಾಜ್ಯಗಳು[0] ); i++ )
      ಆದರೆ ( ft_mem_ಅವಿಭಾಜ್ಯಗಳು[i] > ಅಂಕೆ )
        ಹಿಂತಿರುಗಿ ft_mem_ಅವಿಭಾಜ್ಯಗಳು[i];

    ಹಿಂತಿರುಗಿ FT_MEM_ಗಾತ್ರ_MAX;
  }


  ಸ್ಥಿರ ಶೂನ್ಯ
  ft_mem_debug_panic( ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*  fmt,
                      ... )
  {
    va_list  ap;


    ಬರೆ( "FreeType.Debug: " );

    va_ಆರಂಭ( ap, fmt );
    vprintf( fmt, ap );
    va_ಕೊನೆ( ap );

    ಬರೆ( "\n" );
    ನಿರ್ಗಮಿಸು( EXIT_FAILURE );
  }


  ಸ್ಥಿರ FT_ಸೂಚಕ
  ft_mem_ಕೋಷ್ಟಕ_alloc( FT_Mem_ಕೋಷ್ಟಕ  ಕೋಷ್ಟಕ,
                      FT_Long      ಗಾತ್ರ_2 )
  {
    FT_ಸ್ಮರಣೆ   ಸ್ಮರಣೆ = ಕೋಷ್ಟಕ->ಸ್ಮರಣೆ;
    FT_ಸೂಚಕ  ಖಂಡ;


    ಸ್ಮರಣೆ->user = ಕೋಷ್ಟಕ->ಸ್ಮರಣೆ_user;
    ಖಂಡ = ಕೋಷ್ಟಕ->alloc( ಸ್ಮರಣೆ, ಗಾತ್ರ_2 );
    ಸ್ಮರಣೆ->user = ಕೋಷ್ಟಕ;

    ಹಿಂತಿರುಗಿ ಖಂಡ;
  }


  ಸ್ಥಿರ ಶೂನ್ಯ
  ft_mem_ಕೋಷ್ಟಕ_free( FT_Mem_ಕೋಷ್ಟಕ  ಕೋಷ್ಟಕ,
                     FT_ಸೂಚಕ   ಖಂಡ )
  {
    FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ = ಕೋಷ್ಟಕ->ಸ್ಮರಣೆ;


    ಸ್ಮರಣೆ->user = ಕೋಷ್ಟಕ->ಸ್ಮರಣೆ_user;
    ಕೋಷ್ಟಕ->ಬಿಡುಗಡೆ( ಸ್ಮರಣೆ, ಖಂಡ );
    ಸ್ಮರಣೆ->user = ಕೋಷ್ಟಕ;
  }


  ಸ್ಥಿರ ಶೂನ್ಯ
  ft_mem_ಕೋಷ್ಟಕ_resize( FT_Mem_ಕೋಷ್ಟಕ  ಕೋಷ್ಟಕ )
  {
    FT_Long  new_ಗಾತ್ರ;


    new_ಗಾತ್ರ = ft_mem_closest_ಅವಿಭಾಜ್ಯ( ಕೋಷ್ಟಕ->nodes );
    ಆದರೆ ( new_ಗಾತ್ರ != ಕೋಷ್ಟಕ->ಗಾತ್ರ_2 )
    {
      FT_Mem_ಗಂಟು*  new_buckets;
      FT_Long      i;


      new_buckets = (FT_Mem_ಗಂಟು *)
                      ft_mem_ಕೋಷ್ಟಕ_alloc(
                        ಕೋಷ್ಟಕ,
                        new_ಗಾತ್ರ * (FT_Long)ಗಾತ್ರ ( FT_Mem_ಗಂಟು ) );
      ಆದರೆ ( !new_buckets )
        ಹಿಂತಿರುಗಿ;

      FT_ARRAY_ZERO( new_buckets, new_ಗಾತ್ರ );

      ಗಾಗಿ ( i = 0; i < ಕೋಷ್ಟಕ->ಗಾತ್ರ_2; i++ )
      {
        FT_Mem_ಗಂಟು  ಗಂಟು, ಮುಂದಿನ, *pnode;
        FT_PtrDist  hash;


        ಗಂಟು = ಕೋಷ್ಟಕ->buckets[i];
        ಆಗ ( ಗಂಟು )
        {
          ಮುಂದಿನ  = ಗಂಟು->link;
          hash  = FT_MEM_VAL( ಗಂಟು->ವಿಳಾಸ ) % (FT_PtrDist)new_ಗಾತ್ರ;
          pnode = new_buckets + hash;

          ಗಂಟು->link = pnode[0];
          pnode[0]   = ಗಂಟು;

          ಗಂಟು = ಮುಂದಿನ;
        }
      }

      ಆದರೆ ( ಕೋಷ್ಟಕ->buckets )
        ft_mem_ಕೋಷ್ಟಕ_free( ಕೋಷ್ಟಕ, ಕೋಷ್ಟಕ->buckets );

      ಕೋಷ್ಟಕ->buckets = new_buckets;
      ಕೋಷ್ಟಕ->ಗಾತ್ರ_2    = new_ಗಾತ್ರ;
    }
  }


  ಸ್ಥಿರ ಶೂನ್ಯ
  ft_mem_ಕೋಷ್ಟಕ_destroy( FT_Mem_ಕೋಷ್ಟಕ  ಕೋಷ್ಟಕ )
  {
    FT_Long  i;
    FT_Long  leak_ಎಣಿಕೆ = 0;
    FT_Long  leaks      = 0;


    /* remove all blocks from the table, revealing leaked ones */
    ಗಾಗಿ ( i = 0; i < ಕೋಷ್ಟಕ->ಗಾತ್ರ_2; i++ )
    {
      FT_Mem_ಗಂಟು  *pnode = ಕೋಷ್ಟಕ->buckets + i, ಮುಂದಿನ, ಗಂಟು = *pnode;


      ಆಗ ( ಗಂಟು )
      {
        ಮುಂದಿನ       = ಗಂಟು->link;
        ಗಂಟು->link = ಖಾಲಿ;

        ಆದರೆ ( ಗಂಟು->ಗಾತ್ರ_2 > 0 )
        {
          ಬರೆ(
            "leaked memory block at address %p, size %8ld in (%s:%ld)\n",
            (ಶೂನ್ಯ*)ಗಂಟು->ವಿಳಾಸ,
            ಗಂಟು->ಗಾತ್ರ_2,
            FT_FILENAME( ಗಂಟು->ಮೂಲ->file_name ),
            ಗಂಟು->ಮೂಲ->ಸಾಲು_no );

          leak_ಎಣಿಕೆ++;
          leaks += ಗಂಟು->ಗಾತ್ರ_2;

          ft_mem_ಕೋಷ್ಟಕ_free( ಕೋಷ್ಟಕ, ಗಂಟು->ವಿಳಾಸ );
        }

        ಗಂಟು->ವಿಳಾಸ = ಖಾಲಿ;
        ಗಂಟು->ಗಾತ್ರ_2    = 0;

        ft_mem_ಕೋಷ್ಟಕ_free( ಕೋಷ್ಟಕ, ಗಂಟು );
        ಗಂಟು = ಮುಂದಿನ;
      }
      ಕೋಷ್ಟಕ->buckets[i] = ಖಾಲಿ;
    }

    ft_mem_ಕೋಷ್ಟಕ_free( ಕೋಷ್ಟಕ, ಕೋಷ್ಟಕ->buckets );
    ಕೋಷ್ಟಕ->buckets = ಖಾಲಿ;

    ಕೋಷ್ಟಕ->ಗಾತ್ರ_2  = 0;
    ಕೋಷ್ಟಕ->nodes = 0;

    /* remove all sources */
    ಗಾಗಿ ( i = 0; i < FT_MEM_ಮೂಲ_BUCKETS; i++ )
    {
      FT_Mem_ಮೂಲ  ಮೂಲ, ಮುಂದಿನ;


      ಗಾಗಿ ( ಮೂಲ = ಕೋಷ್ಟಕ->sources[i]; ಮೂಲ != ಖಾಲಿ; ಮೂಲ = ಮುಂದಿನ )
      {
        ಮುಂದಿನ = ಮೂಲ->link;
        ft_mem_ಕೋಷ್ಟಕ_free( ಕೋಷ್ಟಕ, ಮೂಲ );
      }

      ಕೋಷ್ಟಕ->sources[i] = ಖಾಲಿ;
    }

    ಬರೆ( "FreeType: total memory allocations = %ld\n",
            ಕೋಷ್ಟಕ->alloc_ಒಟ್ಟು );
    ಬರೆ( "FreeType: maximum memory footprint = %ld\n",
            ಕೋಷ್ಟಕ->alloc_max );

    ಆದರೆ ( leak_ಎಣಿಕೆ > 0 )
      ft_mem_debug_panic(
        "FreeType: %ld bytes of memory leaked in %ld blocks\n",
        leaks, leak_ಎಣಿಕೆ );

    ಬರೆ( "FreeType: no memory leaks detected\n" );
  }


  ಸ್ಥಿರ FT_Mem_ಗಂಟು*
  ft_mem_ಕೋಷ್ಟಕ_get_nodep( FT_Mem_ಕೋಷ್ಟಕ  ಕೋಷ್ಟಕ,
                          FT_Byte*     ವಿಳಾಸ )
  {
    FT_PtrDist   hash;
    FT_Mem_ಗಂಟು  *pnode, ಗಂಟು;


    hash  = FT_MEM_VAL( ವಿಳಾಸ );
    pnode = ಕೋಷ್ಟಕ->buckets + ( hash % (FT_PtrDist)ಕೋಷ್ಟಕ->ಗಾತ್ರ_2 );

    ಗಾಗಿ (;;)
    {
      ಗಂಟು = pnode[0];
      ಆದರೆ ( !ಗಂಟು )
        ನಿಲ್ಲಿಸು;

      ಆದರೆ ( ಗಂಟು->ವಿಳಾಸ == ವಿಳಾಸ )
        ನಿಲ್ಲಿಸು;

      pnode = &ಗಂಟು->link;
    }
    ಹಿಂತಿರುಗಿ pnode;
  }


  ಸ್ಥಿರ FT_Mem_ಮೂಲ
  ft_mem_ಕೋಷ್ಟಕ_get_ಮೂಲ( FT_Mem_ಕೋಷ್ಟಕ  ಕೋಷ್ಟಕ )
  {
    FT_UInt32     hash;
    FT_Mem_ಮೂಲ  ಗಂಟು, *pnode;


    /* cast to FT_PtrDist first since void* can be larger */
    /* than FT_UInt32 and GCC 4.1.1 emits a warning       */
    hash  = (FT_UInt32)(FT_PtrDist)(ಶೂನ್ಯ*)ft_debug_file_ +
              (FT_UInt32)( 5 * ft_debug_lineno_ );
    pnode = &ಕೋಷ್ಟಕ->sources[hash % FT_MEM_ಮೂಲ_BUCKETS];

    ಗಾಗಿ (;;)
    {
      ಗಂಟು = *pnode;
      ಆದರೆ ( !ಗಂಟು )
        ನಿಲ್ಲಿಸು;

      ಆದರೆ ( ಗಂಟು->file_name == ft_debug_file_   &&
           ಗಂಟು->ಸಾಲು_no   == ft_debug_lineno_ )
        ಹೋಗು Exit;

      pnode = &ಗಂಟು->link;
    }

    ಗಂಟು = (FT_Mem_ಮೂಲ)ft_mem_ಕೋಷ್ಟಕ_alloc( ಕೋಷ್ಟಕ, ಗಾತ್ರ ( *ಗಂಟು ) );
    ಆದರೆ ( !ಗಂಟು )
      ft_mem_debug_panic(
        "not enough memory to perform memory debugging\n" );

    ಗಂಟು->file_name = ft_debug_file_;
    ಗಂಟು->ಸಾಲು_no   = ft_debug_lineno_;

    ಗಂಟು->cur_ಖಂಡಗಳು = 0;
    ಗಂಟು->max_ಖಂಡಗಳು = 0;
    ಗಂಟು->all_ಖಂಡಗಳು = 0;

    ಗಂಟು->cur_ಗಾತ್ರ = 0;
    ಗಂಟು->max_ಗಾತ್ರ = 0;
    ಗಂಟು->all_ಗಾತ್ರ = 0;

    ಗಂಟು->cur_max = 0;

    ಗಂಟು->link = ಖಾಲಿ;
    ಗಂಟು->hash = hash;
    *pnode     = ಗಂಟು;

  Exit:
    ಹಿಂತಿರುಗಿ ಗಂಟು;
  }


  ಸ್ಥಿರ ಶೂನ್ಯ
  ft_mem_ಕೋಷ್ಟಕ_ಗಣ( FT_Mem_ಕೋಷ್ಟಕ  ಕೋಷ್ಟಕ,
                    FT_Byte*     ವಿಳಾಸ,
                    FT_Long      ಗಾತ್ರ_2,
                    FT_Long      delta )
  {
    FT_Mem_ಗಂಟು  *pnode, ಗಂಟು;


    ಆದರೆ ( ಕೋಷ್ಟಕ )
    {
      FT_Mem_ಮೂಲ  ಮೂಲ;


      pnode = ft_mem_ಕೋಷ್ಟಕ_get_nodep( ಕೋಷ್ಟಕ, ವಿಳಾಸ );
      ಗಂಟು  = *pnode;
      ಆದರೆ ( ಗಂಟು )
      {
        ಆದರೆ ( ಗಂಟು->ಗಾತ್ರ_2 < 0 )
        {
          /* This block was already freed.  Our memory is now completely */
          /* corrupted!                                                  */
          /* This can only happen in keep-alive mode.                    */
          ft_mem_debug_panic(
            "memory heap corrupted (allocating freed block)" );
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
          /* This block was already allocated.  This means that our memory */
          /* is also corrupted!                                            */
          ft_mem_debug_panic(
            "memory heap corrupted (re-allocating allocated block at"
            " %p, of size %ld)\n"
            "org=%s:%d new=%s:%d\n",
            ಗಂಟು->ವಿಳಾಸ, ಗಂಟು->ಗಾತ್ರ_2,
            FT_FILENAME( ಗಂಟು->ಮೂಲ->file_name ), ಗಂಟು->ಮೂಲ->ಸಾಲು_no,
            FT_FILENAME( ft_debug_file_ ), ft_debug_lineno_ );
        }
      }

      /* we need to create a new node in this table */
      ಗಂಟು = (FT_Mem_ಗಂಟು)ft_mem_ಕೋಷ್ಟಕ_alloc( ಕೋಷ್ಟಕ, ಗಾತ್ರ ( *ಗಂಟು ) );
      ಆದರೆ ( !ಗಂಟು )
        ft_mem_debug_panic( "not enough memory to run memory tests" );

      ಗಂಟು->ವಿಳಾಸ = ವಿಳಾಸ;
      ಗಂಟು->ಗಾತ್ರ_2    = ಗಾತ್ರ_2;
      ಗಂಟು->ಮೂಲ  = ಮೂಲ = ft_mem_ಕೋಷ್ಟಕ_get_ಮೂಲ( ಕೋಷ್ಟಕ );

      ಆದರೆ ( delta == 0 )
      {
        /* this is an allocation */
        ಮೂಲ->all_ಖಂಡಗಳು++;
        ಮೂಲ->cur_ಖಂಡಗಳು++;
        ಆದರೆ ( ಮೂಲ->cur_ಖಂಡಗಳು > ಮೂಲ->max_ಖಂಡಗಳು )
          ಮೂಲ->max_ಖಂಡಗಳು = ಮೂಲ->cur_ಖಂಡಗಳು;
      }

      ಆದರೆ ( ಗಾತ್ರ_2 > ಮೂಲ->cur_max )
        ಮೂಲ->cur_max = ಗಾತ್ರ_2;

      ಆದರೆ ( delta != 0 )
      {
        /* we are growing or shrinking a reallocated block */
        ಮೂಲ->cur_ಗಾತ್ರ     += delta;
        ಕೋಷ್ಟಕ->alloc_current += delta;
      }
      ಇಲ್ಲದಿದ್ದರೆ
      {
        /* we are allocating a new block */
        ಮೂಲ->cur_ಗಾತ್ರ     += ಗಾತ್ರ_2;
        ಕೋಷ್ಟಕ->alloc_current += ಗಾತ್ರ_2;
      }

      ಮೂಲ->all_ಗಾತ್ರ += ಗಾತ್ರ_2;

      ಆದರೆ ( ಮೂಲ->cur_ಗಾತ್ರ > ಮೂಲ->max_ಗಾತ್ರ )
        ಮೂಲ->max_ಗಾತ್ರ = ಮೂಲ->cur_ಗಾತ್ರ;

      ಗಂಟು->free_file_name = ಖಾಲಿ;
      ಗಂಟು->free_ಸಾಲು_no   = 0;

      ಗಂಟು->link = pnode[0];

      pnode[0] = ಗಂಟು;
      ಕೋಷ್ಟಕ->nodes++;

      ಕೋಷ್ಟಕ->alloc_ಒಟ್ಟು += ಗಾತ್ರ_2;

      ಆದರೆ ( ಕೋಷ್ಟಕ->alloc_current > ಕೋಷ್ಟಕ->alloc_max )
        ಕೋಷ್ಟಕ->alloc_max = ಕೋಷ್ಟಕ->alloc_current;

      ಆದರೆ ( ಕೋಷ್ಟಕ->nodes * 3 < ಕೋಷ್ಟಕ->ಗಾತ್ರ_2  ||
           ಕೋಷ್ಟಕ->ಗಾತ್ರ_2  * 3 < ಕೋಷ್ಟಕ->nodes )
        ft_mem_ಕೋಷ್ಟಕ_resize( ಕೋಷ್ಟಕ );
    }
  }


  ಸ್ಥಿರ ಶೂನ್ಯ
  ft_mem_ಕೋಷ್ಟಕ_remove( FT_Mem_ಕೋಷ್ಟಕ  ಕೋಷ್ಟಕ,
                       FT_Byte*     ವಿಳಾಸ,
                       FT_Long      delta )
  {
    ಆದರೆ ( ಕೋಷ್ಟಕ )
    {
      FT_Mem_ಗಂಟು  *pnode, ಗಂಟು;


      pnode = ft_mem_ಕೋಷ್ಟಕ_get_nodep( ಕೋಷ್ಟಕ, ವಿಳಾಸ );
      ಗಂಟು  = *pnode;
      ಆದರೆ ( ಗಂಟು )
      {
        FT_Mem_ಮೂಲ  ಮೂಲ;


        ಆದರೆ ( ಗಂಟು->ಗಾತ್ರ_2 < 0 )
          ft_mem_debug_panic(
            "freeing memory block at %p more than once\n"
            "  at (%s:%ld)!\n"
            "  Block was allocated at (%s:%ld)\n"
            "  and released at (%s:%ld).",
            ವಿಳಾಸ,
            FT_FILENAME( ft_debug_file_ ), ft_debug_lineno_,
            FT_FILENAME( ಗಂಟು->ಮೂಲ->file_name ), ಗಂಟು->ಮೂಲ->ಸಾಲು_no,
            FT_FILENAME( ಗಂಟು->free_file_name ), ಗಂಟು->free_ಸಾಲು_no );

        /* scramble the node's content for additional safety */
        FT_MEM_ಗಣ( ವಿಳಾಸ, 0xF3, ಗಂಟು->ಗಾತ್ರ_2 );

        ಆದರೆ ( delta == 0 )
        {
          ಮೂಲ = ಗಂಟು->ಮೂಲ;

          ಮೂಲ->cur_ಖಂಡಗಳು--;
          ಮೂಲ->cur_ಗಾತ್ರ -= ಗಂಟು->ಗಾತ್ರ_2;

          ಕೋಷ್ಟಕ->alloc_current -= ಗಂಟು->ಗಾತ್ರ_2;
        }

        ಆದರೆ ( ಕೋಷ್ಟಕ->keep_alive )
        {
          /* we simply invert the node's size to indicate that the node */
          /* was freed.                                                 */
          ಗಂಟು->ಗಾತ್ರ_2           = -ಗಂಟು->ಗಾತ್ರ_2;
          ಗಂಟು->free_file_name = ft_debug_file_;
          ಗಂಟು->free_ಸಾಲು_no   = ft_debug_lineno_;
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
          ಕೋಷ್ಟಕ->nodes--;

          *pnode = ಗಂಟು->link;

          ಗಂಟು->ಗಾತ್ರ_2   = 0;
          ಗಂಟು->ಮೂಲ = ಖಾಲಿ;

          ft_mem_ಕೋಷ್ಟಕ_free( ಕೋಷ್ಟಕ, ಗಂಟು );

          ಆದರೆ ( ಕೋಷ್ಟಕ->nodes * 3 < ಕೋಷ್ಟಕ->ಗಾತ್ರ_2  ||
               ಕೋಷ್ಟಕ->ಗಾತ್ರ_2  * 3 < ಕೋಷ್ಟಕ->nodes )
            ft_mem_ಕೋಷ್ಟಕ_resize( ಕೋಷ್ಟಕ );
        }
      }
      ಇಲ್ಲದಿದ್ದರೆ
        ft_mem_debug_panic(
          "trying to free unknown block at %p in (%s:%ld)\n",
          ವಿಳಾಸ,
          FT_FILENAME( ft_debug_file_ ), ft_debug_lineno_ );
    }
  }


  ಸ್ಥಿರ FT_ಸೂಚಕ
  ft_mem_debug_alloc( FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ,
                      FT_Long    ಗಾತ್ರ_2 )
  {
    FT_Mem_ಕೋಷ್ಟಕ  ಕೋಷ್ಟಕ = (FT_Mem_ಕೋಷ್ಟಕ)ಸ್ಮರಣೆ->user;
    FT_Byte*     ಖಂಡ;


    ಆದರೆ ( ಗಾತ್ರ_2 <= 0 )
      ft_mem_debug_panic( "negative block size allocation (%ld)", ಗಾತ್ರ_2 );

    /* return NULL if the maximum number of allocations was reached */
    ಆದರೆ ( ಕೋಷ್ಟಕ->bound_ಎಣಿಕೆ                           &&
         ಕೋಷ್ಟಕ->alloc_ಎಣಿಕೆ >= ಕೋಷ್ಟಕ->alloc_ಎಣಿಕೆ_max )
      ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    /* return NULL if this allocation would overflow the maximum heap size */
    ಆದರೆ ( ಕೋಷ್ಟಕ->bound_ಒಟ್ಟು                                   &&
         ಕೋಷ್ಟಕ->alloc_ಒಟ್ಟು_max - ಕೋಷ್ಟಕ->alloc_current > ಗಾತ್ರ_2 )
      ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    ಖಂಡ = (FT_Byte *)ft_mem_ಕೋಷ್ಟಕ_alloc( ಕೋಷ್ಟಕ, ಗಾತ್ರ_2 );
    ಆದರೆ ( ಖಂಡ )
    {
      ft_mem_ಕೋಷ್ಟಕ_ಗಣ( ಕೋಷ್ಟಕ, ಖಂಡ, ಗಾತ್ರ_2, 0 );

      ಕೋಷ್ಟಕ->alloc_ಎಣಿಕೆ++;
    }

    ft_debug_file_   = "<unknown>";
    ft_debug_lineno_ = 0;

    ಹಿಂತಿರುಗಿ (FT_ಸೂಚಕ)ಖಂಡ;
  }


  ಸ್ಥಿರ ಶೂನ್ಯ
  ft_mem_debug_free( FT_ಸ್ಮರಣೆ   ಸ್ಮರಣೆ,
                     FT_ಸೂಚಕ  ಖಂಡ )
  {
    FT_Mem_ಕೋಷ್ಟಕ  ಕೋಷ್ಟಕ = (FT_Mem_ಕೋಷ್ಟಕ)ಸ್ಮರಣೆ->user;


    ಆದರೆ ( !ಖಂಡ )
      ft_mem_debug_panic( "trying to free NULL in (%s:%ld)",
                          FT_FILENAME( ft_debug_file_ ),
                          ft_debug_lineno_ );

    ft_mem_ಕೋಷ್ಟಕ_remove( ಕೋಷ್ಟಕ, (FT_Byte*)ಖಂಡ, 0 );

    ಆದರೆ ( !ಕೋಷ್ಟಕ->keep_alive )
      ft_mem_ಕೋಷ್ಟಕ_free( ಕೋಷ್ಟಕ, ಖಂಡ );

    ಕೋಷ್ಟಕ->alloc_ಎಣಿಕೆ--;

    ft_debug_file_   = "<unknown>";
    ft_debug_lineno_ = 0;
  }


  ಸ್ಥಿರ FT_ಸೂಚಕ
  ft_mem_debug_realloc( FT_ಸ್ಮರಣೆ   ಸ್ಮರಣೆ,
                        FT_Long     cur_ಗಾತ್ರ,
                        FT_Long     new_ಗಾತ್ರ,
                        FT_ಸೂಚಕ  ಖಂಡ )
  {
    FT_Mem_ಕೋಷ್ಟಕ  ಕೋಷ್ಟಕ = (FT_Mem_ಕೋಷ್ಟಕ)ಸ್ಮರಣೆ->user;
    FT_Mem_ಗಂಟು   ಗಂಟು, *pnode;
    FT_ಸೂಚಕ   new_ಖಂಡ;
    FT_Long      delta;

    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*  file_name = FT_FILENAME( ft_debug_file_ );
    FT_Long      ಸಾಲು_no   = ft_debug_lineno_;


    /* unlikely, but possible */
    ಆದರೆ ( new_ಗಾತ್ರ == cur_ಗಾತ್ರ )
      ಹಿಂತಿರುಗಿ ಖಂಡ;

    /* the following is valid according to ANSI C */
#ಆದರೆ 0
    ಆದರೆ ( !ಖಂಡ || !cur_ಗಾತ್ರ )
      ft_mem_debug_panic( "trying to reallocate NULL in (%s:%ld)",
                          file_name, ಸಾಲು_no );
#ಆದರೆ_ಅಂತ್ಯ

    /* while the following is allowed in ANSI C also, we abort since */
    /* such case should be handled by FreeType.                      */
    ಆದರೆ ( new_ಗಾತ್ರ <= 0 )
      ft_mem_debug_panic(
        "trying to reallocate %p to size 0 (current is %ld) in (%s:%ld)",
        ಖಂಡ, cur_ಗಾತ್ರ, file_name, ಸಾಲು_no );

    /* check `cur_size' value */
    pnode = ft_mem_ಕೋಷ್ಟಕ_get_nodep( ಕೋಷ್ಟಕ, (FT_Byte*)ಖಂಡ );
    ಗಂಟು  = *pnode;
    ಆದರೆ ( !ಗಂಟು )
      ft_mem_debug_panic(
        "trying to reallocate unknown block at %p in (%s:%ld)",
        ಖಂಡ, file_name, ಸಾಲು_no );

    ಆದರೆ ( ಗಂಟು->ಗಾತ್ರ_2 <= 0 )
      ft_mem_debug_panic(
        "trying to reallocate freed block at %p in (%s:%ld)",
        ಖಂಡ, file_name, ಸಾಲು_no );

    ಆದರೆ ( ಗಂಟು->ಗಾತ್ರ_2 != cur_ಗಾತ್ರ )
      ft_mem_debug_panic( "invalid ft_realloc request for %p. cur_size is "
                          "%ld instead of %ld in (%s:%ld)",
                          ಖಂಡ, cur_ಗಾತ್ರ, ಗಂಟು->ಗಾತ್ರ_2, file_name, ಸಾಲು_no );

    /* return NULL if the maximum number of allocations was reached */
    ಆದರೆ ( ಕೋಷ್ಟಕ->bound_ಎಣಿಕೆ                           &&
         ಕೋಷ್ಟಕ->alloc_ಎಣಿಕೆ >= ಕೋಷ್ಟಕ->alloc_ಎಣಿಕೆ_max )
      ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    delta = new_ಗಾತ್ರ - cur_ಗಾತ್ರ;

    /* return NULL if this allocation would overflow the maximum heap size */
    ಆದರೆ ( delta > 0                                             &&
         ಕೋಷ್ಟಕ->bound_ಒಟ್ಟು                                    &&
         ಕೋಷ್ಟಕ->alloc_current + delta > ಕೋಷ್ಟಕ->alloc_ಒಟ್ಟು_max )
      ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    new_ಖಂಡ = (FT_ಸೂಚಕ)ft_mem_ಕೋಷ್ಟಕ_alloc( ಕೋಷ್ಟಕ, new_ಗಾತ್ರ );
    ಆದರೆ ( !new_ಖಂಡ )
      ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    ft_mem_ಕೋಷ್ಟಕ_ಗಣ( ಕೋಷ್ಟಕ, (FT_Byte*)new_ಖಂಡ, new_ಗಾತ್ರ, delta );

    ft_memcpy( new_ಖಂಡ, ಖಂಡ, cur_ಗಾತ್ರ < new_ಗಾತ್ರ ? (size_t)cur_ಗಾತ್ರ
                                                     : (size_t)new_ಗಾತ್ರ );

    ft_mem_ಕೋಷ್ಟಕ_remove( ಕೋಷ್ಟಕ, (FT_Byte*)ಖಂಡ, delta );

    ft_debug_file_   = "<unknown>";
    ft_debug_lineno_ = 0;

    ಆದರೆ ( !ಕೋಷ್ಟಕ->keep_alive )
      ft_mem_ಕೋಷ್ಟಕ_free( ಕೋಷ್ಟಕ, ಖಂಡ );

    ಹಿಂತಿರುಗಿ new_ಖಂಡ;
  }


  ಬಾಹ್ಯ ಶೂನ್ಯ
  ft_mem_debug_init( FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ )
  {
    FT_Mem_ಕೋಷ್ಟಕ  ಕೋಷ್ಟಕ;


    ಆದರೆ ( !ft_getenv( "FT2_DEBUG_MEMORY" ) )
      ಹಿಂತಿರುಗಿ;

    ಕೋಷ್ಟಕ = (FT_Mem_ಕೋಷ್ಟಕ)ಸ್ಮರಣೆ->alloc( ಸ್ಮರಣೆ, ಗಾತ್ರ ( *ಕೋಷ್ಟಕ ) );

    ಆದರೆ ( ಕೋಷ್ಟಕ )
    {
      FT_ZERO( ಕೋಷ್ಟಕ );

      ಕೋಷ್ಟಕ->ಸ್ಮರಣೆ      = ಸ್ಮರಣೆ;
      ಕೋಷ್ಟಕ->ಸ್ಮರಣೆ_user = ಸ್ಮರಣೆ->user;
      ಕೋಷ್ಟಕ->alloc       = ಸ್ಮರಣೆ->alloc;
      ಕೋಷ್ಟಕ->realloc     = ಸ್ಮರಣೆ->realloc;
      ಕೋಷ್ಟಕ->ಬಿಡುಗಡೆ        = ಸ್ಮರಣೆ->ಬಿಡುಗಡೆ;

      ft_mem_ಕೋಷ್ಟಕ_resize( ಕೋಷ್ಟಕ );

      ಆದರೆ ( ಕೋಷ್ಟಕ->ಗಾತ್ರ_2 )
      {
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*  p;


        ಸ್ಮರಣೆ->user    = ಕೋಷ್ಟಕ;
        ಸ್ಮರಣೆ->alloc   = ft_mem_debug_alloc;
        ಸ್ಮರಣೆ->realloc = ft_mem_debug_realloc;
        ಸ್ಮರಣೆ->ಬಿಡುಗಡೆ    = ft_mem_debug_free;

        p = ft_getenv( "FT2_ALLOC_TOTAL_MAX" );
        ಆದರೆ ( p )
        {
          FT_Long  ಒಟ್ಟು_max = ft_strtol( p, ಖಾಲಿ, 10 );


          ಆದರೆ ( ಒಟ್ಟು_max > 0 )
          {
            ಕೋಷ್ಟಕ->bound_ಒಟ್ಟು     = 1;
            ಕೋಷ್ಟಕ->alloc_ಒಟ್ಟು_max = ಒಟ್ಟು_max;
          }
        }

        p = ft_getenv( "FT2_ALLOC_COUNT_MAX" );
        ಆದರೆ ( p )
        {
          FT_Long  ಒಟ್ಟು_ಎಣಿಕೆ = ft_strtol( p, ಖಾಲಿ, 10 );


          ಆದರೆ ( ಒಟ್ಟು_ಎಣಿಕೆ > 0 )
          {
            ಕೋಷ್ಟಕ->bound_ಎಣಿಕೆ     = 1;
            ಕೋಷ್ಟಕ->alloc_ಎಣಿಕೆ_max = ಒಟ್ಟು_ಎಣಿಕೆ;
          }
        }

        p = ft_getenv( "FT2_KEEP_ALIVE" );
        ಆದರೆ ( p )
        {
          FT_Long  keep_alive = ft_strtol( p, ಖಾಲಿ, 10 );


          ಆದರೆ ( keep_alive > 0 )
            ಕೋಷ್ಟಕ->keep_alive = 1;
        }
      }
      ಇಲ್ಲದಿದ್ದರೆ
        ಸ್ಮರಣೆ->ಬಿಡುಗಡೆ( ಸ್ಮರಣೆ, ಕೋಷ್ಟಕ );
    }
  }


  ಬಾಹ್ಯ ಶೂನ್ಯ
  ft_mem_debug_ಮುಗಿದ( FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ )
  {
    ಆದರೆ ( ಸ್ಮರಣೆ->ಬಿಡುಗಡೆ == ft_mem_debug_free )
    {
      FT_Mem_ಕೋಷ್ಟಕ  ಕೋಷ್ಟಕ = (FT_Mem_ಕೋಷ್ಟಕ)ಸ್ಮರಣೆ->user;


      FT_Dump_ಸ್ಮರಣೆ( ಸ್ಮರಣೆ );

      ft_mem_ಕೋಷ್ಟಕ_destroy( ಕೋಷ್ಟಕ );

      ಸ್ಮರಣೆ->ಬಿಡುಗಡೆ    = ಕೋಷ್ಟಕ->ಬಿಡುಗಡೆ;
      ಸ್ಮರಣೆ->realloc = ಕೋಷ್ಟಕ->realloc;
      ಸ್ಮರಣೆ->alloc   = ಕೋಷ್ಟಕ->alloc;
      ಸ್ಮರಣೆ->user    = ಕೋಷ್ಟಕ->ಸ್ಮರಣೆ_user;

      ಸ್ಮರಣೆ->ಬಿಡುಗಡೆ( ಸ್ಮರಣೆ, ಕೋಷ್ಟಕ );
    }
  }


  FT_COMPARE_DEF( ಪೂರ್ಣಾಂಕ )
  ft_mem_ಮೂಲ_compare( ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ*  p1,
                         ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ*  p2 )
  {
    FT_Mem_ಮೂಲ  s1 = *(FT_Mem_ಮೂಲ*)p1;
    FT_Mem_ಮೂಲ  s2 = *(FT_Mem_ಮೂಲ*)p2;


    ಆದರೆ ( s2->max_ಗಾತ್ರ > s1->max_ಗಾತ್ರ )
      ಹಿಂತಿರುಗಿ 1;
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ( s2->max_ಗಾತ್ರ < s1->max_ಗಾತ್ರ )
      ಹಿಂತಿರುಗಿ -1;
    ಇಲ್ಲದಿದ್ದರೆ
      ಹಿಂತಿರುಗಿ 0;
  }


  ಬಾಹ್ಯ ಶೂನ್ಯ
  FT_Dump_ಸ್ಮರಣೆ( FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ )
  {
    ಆದರೆ ( ಸ್ಮರಣೆ->ಬಿಡುಗಡೆ == ft_mem_debug_free )
    {
      FT_Mem_ಕೋಷ್ಟಕ    ಕೋಷ್ಟಕ = (FT_Mem_ಕೋಷ್ಟಕ)ಸ್ಮರಣೆ->user;
      FT_Mem_ಮೂಲ*  ಪಾತ್ರೆ = ಕೋಷ್ಟಕ->sources;
      FT_Mem_ಮೂಲ*  ಎಲ್ಲೆ  = ಪಾತ್ರೆ + FT_MEM_ಮೂಲ_BUCKETS;
      FT_Mem_ಮೂಲ*  sources;
      FT_Int         nn, ಎಣಿಕೆ_2;
      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*    fmt;


      ಎಣಿಕೆ_2 = 0;
      ಗಾಗಿ ( ; ಪಾತ್ರೆ < ಎಲ್ಲೆ; ಪಾತ್ರೆ++ )
      {
        FT_Mem_ಮೂಲ  ಮೂಲ = *ಪಾತ್ರೆ;


        ಗಾಗಿ ( ; ಮೂಲ; ಮೂಲ = ಮೂಲ->link )
          ಎಣಿಕೆ_2++;
      }

      sources = (FT_Mem_ಮೂಲ*)
                  ft_mem_ಕೋಷ್ಟಕ_alloc(
                    ಕೋಷ್ಟಕ, ಎಣಿಕೆ_2 * (FT_Long)ಗಾತ್ರ ( *sources ) );

      ಎಣಿಕೆ_2 = 0;
      ಗಾಗಿ ( ಪಾತ್ರೆ = ಕೋಷ್ಟಕ->sources; ಪಾತ್ರೆ < ಎಲ್ಲೆ; ಪಾತ್ರೆ++ )
      {
        FT_Mem_ಮೂಲ  ಮೂಲ = *ಪಾತ್ರೆ;


        ಗಾಗಿ ( ; ಮೂಲ; ಮೂಲ = ಮೂಲ->link )
          sources[ಎಣಿಕೆ_2++] = ಮೂಲ;
      }

      ft_qsort( sources,
                (size_t)ಎಣಿಕೆ_2,
                ಗಾತ್ರ ( *sources ),
                ft_mem_ಮೂಲ_compare );

      ಬರೆ( "FreeType Memory Dump: "
              "current=%ld max=%ld total=%ld count=%ld\n",
              ಕೋಷ್ಟಕ->alloc_current, ಕೋಷ್ಟಕ->alloc_max,
              ಕೋಷ್ಟಕ->alloc_ಒಟ್ಟು, ಕೋಷ್ಟಕ->alloc_ಎಣಿಕೆ );
      ಬರೆ( " block  block    sizes    sizes    sizes   source\n" );
      ಬರೆ( " count   high      sum  highsum      max   location\n" );
      ಬರೆ( "-------------------------------------------------\n" );

      fmt = "%6ld %6ld %8ld %8ld %8ld %s:%d\n";

      ಗಾಗಿ ( nn = 0; nn < ಎಣಿಕೆ_2; nn++ )
      {
        FT_Mem_ಮೂಲ  ಮೂಲ = sources[nn];


        ಬರೆ( fmt,
                ಮೂಲ->cur_ಖಂಡಗಳು, ಮೂಲ->max_ಖಂಡಗಳು,
                ಮೂಲ->cur_ಗಾತ್ರ, ಮೂಲ->max_ಗಾತ್ರ, ಮೂಲ->cur_max,
                FT_FILENAME( ಮೂಲ->file_name ),
                ಮೂಲ->ಸಾಲು_no );
      }
      ಬರೆ( "------------------------------------------------\n" );

      ft_mem_ಕೋಷ್ಟಕ_free( ಕೋಷ್ಟಕ, sources );
    }
  }

#ಇಲ್ಲದಿದ್ದರೆ  /* !FT_DEBUG_MEMORY */

  /* ANSI C doesn't like empty source files */
  ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಪೂರ್ಣಾಂಕ  debug_mem_dummy_;

#ಆದರೆ_ಅಂತ್ಯ /* !FT_DEBUG_MEMORY */


/*ಕೊನೆ*/
