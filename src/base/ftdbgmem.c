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


#ਅਨ੍ਤਰ੍ਭਾਵ <ft2build.h>
#ਅਨ੍ਤਰ੍ਭਾਵ FT_CONFIG_CONFIG_H
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftdebug.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftmemory.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/ftsystem.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/fterrors.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/fttypes.h>


#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ FT_DEBUG_ਸਮ੍ਰਿਤੀ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ  KEEPALIVE /* `Keep alive' means that freed blocks aren't released
                    * to the heap.  This is useful to detect double-frees
                    * or weird heap corruption, but it uses large amounts of
                    * memory, however.
                    */

#ਅਨ੍ਤਰ੍ਭਾਵ FT_CONFIG_STANDARD_LIBRARY_H

  FT_ਅਧਾਰ_DEF( ਅਚਲ ਅੱਖਰ* )  ft_debug_file_   = ਨਿਰਾ;
  FT_ਅਧਾਰ_DEF( ਲੰਮਾ )         ft_debug_lineno_ = 0;

  ਬਾਹਰੀ ਖਾਲੀ
  FT_Dump_ਸਮ੍ਰਿਤੀ( FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ );


  ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ FT_Mem_ਸਰੋਤ_Rec*  FT_Mem_ਸਰੋਤ;
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ FT_Mem_ਗੰਢ_Rec*    FT_Mem_ਗੰਢ;
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ FT_Mem_ਸਾਰਣੀ_Rec*   FT_Mem_ਸਾਰਣੀ;


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FT_MEM_VAL( addr )  ( (FT_PtrDist)(FT_ਸੂਚਕ)( addr ) )

  /*
   * This structure holds statistics for a single allocation/release
   * site.  This is useful to know where memory operations happen the
   * most.
   */
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ  FT_Mem_ਸਰੋਤ_Rec
  {
    ਅਚਲ ਅੱਖਰ*   file_name;
    ਲੰਮਾ          ਸਤਰ_no;

    FT_Long       cur_ਖੰਡਾਂ;   /* current number of allocated blocks */
    FT_Long       max_ਖੰਡਾਂ;   /* max. number of allocated blocks    */
    FT_Long       all_ਖੰਡਾਂ;   /* total number of blocks allocated   */

    FT_Long       cur_ਮਾਪ;     /* current cumulative allocated size */
    FT_Long       max_ਮਾਪ;     /* maximum cumulative allocated size */
    FT_Long       all_ਮਾਪ;     /* total cumulative allocated size   */

    FT_Long       cur_max;      /* current maximum allocated size */

    FT_UInt32     hash;
    FT_Mem_ਸਰੋਤ  link;

  } FT_Mem_ਸਰੋਤ_Rec_2;


  /*
   * We don't need a resizable array for the memory sources because
   * their number is pretty limited within FreeType.
   */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FT_MEM_ਸਰੋਤ_BUCKETS  128

  /*
   * This structure holds information related to a single allocated
   * memory block.  If KEEPALIVE is defined, blocks that are freed by
   * FreeType are never released to the system.  Instead, their `size'
   * field is set to `-size'.  This is mainly useful to detect double
   * frees, at the price of a large memory footprint during execution.
   */
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ  FT_Mem_ਗੰਢ_Rec
  {
    FT_Byte*      ਪਤਾ;
    FT_Long       ਮਾਪ;     /* < 0 if the block was freed */

    FT_Mem_ਸਰੋਤ  ਸਰੋਤ;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ KEEPALIVE
    ਅਚਲ ਅੱਖਰ*   free_file_name;
    FT_Long       free_ਸਤਰ_no;
#ਜੇ_ਅੰਤ

    FT_Mem_ਗੰਢ    link;

  } FT_Mem_ਗੰਢ_Rec_2;


  /*
   * The global structure, containing compound statistics and all hash
   * tables.
   */
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ  FT_Mem_ਸਾਰਣੀ_Rec
  {
    FT_Long          ਮਾਪ;
    FT_Long          nodes;
    FT_Mem_ਗੰਢ*      buckets;

    FT_Long          alloc_ਕੁੱਲ;
    FT_Long          alloc_current;
    FT_Long          alloc_max;
    FT_Long          alloc_ਗਿਣਤੀ;

    FT_Bool          bound_ਕੁੱਲ;
    FT_Long          alloc_ਕੁੱਲ_max;

    FT_Bool          bound_ਗਿਣਤੀ;
    FT_Long          alloc_ਗਿਣਤੀ_max;

    FT_Mem_ਸਰੋਤ     sources[FT_MEM_ਸਰੋਤ_BUCKETS];

    FT_Bool          keep_alive;

    FT_ਸਮ੍ਰਿਤੀ        ਸਮ੍ਰਿਤੀ;
    FT_ਸੂਚਕ       ਸਮ੍ਰਿਤੀ_user;
    FT_Alloc_Func    alloc;
    FT_Free_Func     ਮੁਕਤ;
    FT_Realloc_Func  realloc;

  } FT_Mem_ਸਾਰਣੀ_Rec_2;


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FT_MEM_ਮਾਪ_MAX  13845163

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FT_FILENAME( x )  ( (x) ? (x) : "unknown file" )


  /*
   * Prime numbers are ugly to handle.  It would be better to implement
   * L-Hashing, which is 10% faster and doesn't require divisions.
   */
  ਸਥਿਰ ਅਚਲ FT_Int  ft_mem_ਅਭਾਜ_ਸੰਖਿਆਵਾਂ[] =
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


  ਸਥਿਰ FT_Long
  ft_mem_closest_ਅਭਾਜ( FT_Long  ਅੰਕ )
  {
    size_t  i;


    ਲਈ ( i = 0;
          i < ਆਕਾਰ ( ft_mem_ਅਭਾਜ_ਸੰਖਿਆਵਾਂ ) / ਆਕਾਰ ( ft_mem_ਅਭਾਜ_ਸੰਖਿਆਵਾਂ[0] ); i++ )
      ਜੇ ( ft_mem_ਅਭਾਜ_ਸੰਖਿਆਵਾਂ[i] > ਅੰਕ )
        ਵਾਪਸ ft_mem_ਅਭਾਜ_ਸੰਖਿਆਵਾਂ[i];

    ਵਾਪਸ FT_MEM_ਮਾਪ_MAX;
  }


  ਸਥਿਰ ਖਾਲੀ
  ft_mem_debug_panic( ਅਚਲ ਅੱਖਰ*  fmt,
                      ... )
  {
    va_list  ap;


    ਲਿਖੋ( "FreeType.Debug: " );

    va_ਸ਼ੁਰੂ( ap, fmt );
    vprintf( fmt, ap );
    va_ਸਿਰਾ( ap );

    ਲਿਖੋ( "\n" );
    ਬਾਹਰ_ਨਿਕਲੋ( EXIT_FAILURE );
  }


  ਸਥਿਰ FT_ਸੂਚਕ
  ft_mem_ਸਾਰਣੀ_alloc( FT_Mem_ਸਾਰਣੀ  ਸਾਰਣੀ,
                      FT_Long      ਮਾਪ )
  {
    FT_ਸਮ੍ਰਿਤੀ   ਸਮ੍ਰਿਤੀ = ਸਾਰਣੀ->ਸਮ੍ਰਿਤੀ;
    FT_ਸੂਚਕ  ਖੰਡ;


    ਸਮ੍ਰਿਤੀ->user = ਸਾਰਣੀ->ਸਮ੍ਰਿਤੀ_user;
    ਖੰਡ = ਸਾਰਣੀ->alloc( ਸਮ੍ਰਿਤੀ, ਮਾਪ );
    ਸਮ੍ਰਿਤੀ->user = ਸਾਰਣੀ;

    ਵਾਪਸ ਖੰਡ;
  }


  ਸਥਿਰ ਖਾਲੀ
  ft_mem_ਸਾਰਣੀ_free( FT_Mem_ਸਾਰਣੀ  ਸਾਰਣੀ,
                     FT_ਸੂਚਕ   ਖੰਡ )
  {
    FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ = ਸਾਰਣੀ->ਸਮ੍ਰਿਤੀ;


    ਸਮ੍ਰਿਤੀ->user = ਸਾਰਣੀ->ਸਮ੍ਰਿਤੀ_user;
    ਸਾਰਣੀ->ਮੁਕਤ( ਸਮ੍ਰਿਤੀ, ਖੰਡ );
    ਸਮ੍ਰਿਤੀ->user = ਸਾਰਣੀ;
  }


  ਸਥਿਰ ਖਾਲੀ
  ft_mem_ਸਾਰਣੀ_resize( FT_Mem_ਸਾਰਣੀ  ਸਾਰਣੀ )
  {
    FT_Long  new_ਮਾਪ;


    new_ਮਾਪ = ft_mem_closest_ਅਭਾਜ( ਸਾਰਣੀ->nodes );
    ਜੇ ( new_ਮਾਪ != ਸਾਰਣੀ->ਮਾਪ )
    {
      FT_Mem_ਗੰਢ*  new_buckets;
      FT_Long      i;


      new_buckets = (FT_Mem_ਗੰਢ *)
                      ft_mem_ਸਾਰਣੀ_alloc(
                        ਸਾਰਣੀ,
                        new_ਮਾਪ * (FT_Long)ਆਕਾਰ ( FT_Mem_ਗੰਢ ) );
      ਜੇ ( !new_buckets )
        ਵਾਪਸ;

      FT_ARRAY_ZERO( new_buckets, new_ਮਾਪ );

      ਲਈ ( i = 0; i < ਸਾਰਣੀ->ਮਾਪ; i++ )
      {
        FT_Mem_ਗੰਢ  ਗੰਢ, ਅਗਲਾ, *pnode;
        FT_PtrDist  hash;


        ਗੰਢ = ਸਾਰਣੀ->buckets[i];
        ਜਦੋਂ ( ਗੰਢ )
        {
          ਅਗਲਾ  = ਗੰਢ->link;
          hash  = FT_MEM_VAL( ਗੰਢ->ਪਤਾ ) % (FT_PtrDist)new_ਮਾਪ;
          pnode = new_buckets + hash;

          ਗੰਢ->link = pnode[0];
          pnode[0]   = ਗੰਢ;

          ਗੰਢ = ਅਗਲਾ;
        }
      }

      ਜੇ ( ਸਾਰਣੀ->buckets )
        ft_mem_ਸਾਰਣੀ_free( ਸਾਰਣੀ, ਸਾਰਣੀ->buckets );

      ਸਾਰਣੀ->buckets = new_buckets;
      ਸਾਰਣੀ->ਮਾਪ    = new_ਮਾਪ;
    }
  }


  ਸਥਿਰ ਖਾਲੀ
  ft_mem_ਸਾਰਣੀ_destroy( FT_Mem_ਸਾਰਣੀ  ਸਾਰਣੀ )
  {
    FT_Long  i;
    FT_Long  leak_ਗਿਣਤੀ = 0;
    FT_Long  leaks      = 0;


    /* remove all blocks from the table, revealing leaked ones */
    ਲਈ ( i = 0; i < ਸਾਰਣੀ->ਮਾਪ; i++ )
    {
      FT_Mem_ਗੰਢ  *pnode = ਸਾਰਣੀ->buckets + i, ਅਗਲਾ, ਗੰਢ = *pnode;


      ਜਦੋਂ ( ਗੰਢ )
      {
        ਅਗਲਾ       = ਗੰਢ->link;
        ਗੰਢ->link = ਨਿਰਾ;

        ਜੇ ( ਗੰਢ->ਮਾਪ > 0 )
        {
          ਲਿਖੋ(
            "leaked memory block at address %p, size %8ld in (%s:%ld)\n",
            (ਖਾਲੀ*)ਗੰਢ->ਪਤਾ,
            ਗੰਢ->ਮਾਪ,
            FT_FILENAME( ਗੰਢ->ਸਰੋਤ->file_name ),
            ਗੰਢ->ਸਰੋਤ->ਸਤਰ_no );

          leak_ਗਿਣਤੀ++;
          leaks += ਗੰਢ->ਮਾਪ;

          ft_mem_ਸਾਰਣੀ_free( ਸਾਰਣੀ, ਗੰਢ->ਪਤਾ );
        }

        ਗੰਢ->ਪਤਾ = ਨਿਰਾ;
        ਗੰਢ->ਮਾਪ    = 0;

        ft_mem_ਸਾਰਣੀ_free( ਸਾਰਣੀ, ਗੰਢ );
        ਗੰਢ = ਅਗਲਾ;
      }
      ਸਾਰਣੀ->buckets[i] = ਨਿਰਾ;
    }

    ft_mem_ਸਾਰਣੀ_free( ਸਾਰਣੀ, ਸਾਰਣੀ->buckets );
    ਸਾਰਣੀ->buckets = ਨਿਰਾ;

    ਸਾਰਣੀ->ਮਾਪ  = 0;
    ਸਾਰਣੀ->nodes = 0;

    /* remove all sources */
    ਲਈ ( i = 0; i < FT_MEM_ਸਰੋਤ_BUCKETS; i++ )
    {
      FT_Mem_ਸਰੋਤ  ਸਰੋਤ, ਅਗਲਾ;


      ਲਈ ( ਸਰੋਤ = ਸਾਰਣੀ->sources[i]; ਸਰੋਤ != ਨਿਰਾ; ਸਰੋਤ = ਅਗਲਾ )
      {
        ਅਗਲਾ = ਸਰੋਤ->link;
        ft_mem_ਸਾਰਣੀ_free( ਸਾਰਣੀ, ਸਰੋਤ );
      }

      ਸਾਰਣੀ->sources[i] = ਨਿਰਾ;
    }

    ਲਿਖੋ( "FreeType: total memory allocations = %ld\n",
            ਸਾਰਣੀ->alloc_ਕੁੱਲ );
    ਲਿਖੋ( "FreeType: maximum memory footprint = %ld\n",
            ਸਾਰਣੀ->alloc_max );

    ਜੇ ( leak_ਗਿਣਤੀ > 0 )
      ft_mem_debug_panic(
        "FreeType: %ld bytes of memory leaked in %ld blocks\n",
        leaks, leak_ਗਿਣਤੀ );

    ਲਿਖੋ( "FreeType: no memory leaks detected\n" );
  }


  ਸਥਿਰ FT_Mem_ਗੰਢ*
  ft_mem_ਸਾਰਣੀ_get_nodep( FT_Mem_ਸਾਰਣੀ  ਸਾਰਣੀ,
                          FT_Byte*     ਪਤਾ )
  {
    FT_PtrDist   hash;
    FT_Mem_ਗੰਢ  *pnode, ਗੰਢ;


    hash  = FT_MEM_VAL( ਪਤਾ );
    pnode = ਸਾਰਣੀ->buckets + ( hash % (FT_PtrDist)ਸਾਰਣੀ->ਮਾਪ );

    ਲਈ (;;)
    {
      ਗੰਢ = pnode[0];
      ਜੇ ( !ਗੰਢ )
        ਰੋਕੋ;

      ਜੇ ( ਗੰਢ->ਪਤਾ == ਪਤਾ )
        ਰੋਕੋ;

      pnode = &ਗੰਢ->link;
    }
    ਵਾਪਸ pnode;
  }


  ਸਥਿਰ FT_Mem_ਸਰੋਤ
  ft_mem_ਸਾਰਣੀ_get_ਸਰੋਤ( FT_Mem_ਸਾਰਣੀ  ਸਾਰਣੀ )
  {
    FT_UInt32     hash;
    FT_Mem_ਸਰੋਤ  ਗੰਢ, *pnode;


    /* cast to FT_PtrDist first since void* can be larger */
    /* than FT_UInt32 and GCC 4.1.1 emits a warning       */
    hash  = (FT_UInt32)(FT_PtrDist)(ਖਾਲੀ*)ft_debug_file_ +
              (FT_UInt32)( 5 * ft_debug_lineno_ );
    pnode = &ਸਾਰਣੀ->sources[hash % FT_MEM_ਸਰੋਤ_BUCKETS];

    ਲਈ (;;)
    {
      ਗੰਢ = *pnode;
      ਜੇ ( !ਗੰਢ )
        ਰੋਕੋ;

      ਜੇ ( ਗੰਢ->file_name == ft_debug_file_   &&
           ਗੰਢ->ਸਤਰ_no   == ft_debug_lineno_ )
        ਜਾਓ Exit;

      pnode = &ਗੰਢ->link;
    }

    ਗੰਢ = (FT_Mem_ਸਰੋਤ)ft_mem_ਸਾਰਣੀ_alloc( ਸਾਰਣੀ, ਆਕਾਰ ( *ਗੰਢ ) );
    ਜੇ ( !ਗੰਢ )
      ft_mem_debug_panic(
        "not enough memory to perform memory debugging\n" );

    ਗੰਢ->file_name = ft_debug_file_;
    ਗੰਢ->ਸਤਰ_no   = ft_debug_lineno_;

    ਗੰਢ->cur_ਖੰਡਾਂ = 0;
    ਗੰਢ->max_ਖੰਡਾਂ = 0;
    ਗੰਢ->all_ਖੰਡਾਂ = 0;

    ਗੰਢ->cur_ਮਾਪ = 0;
    ਗੰਢ->max_ਮਾਪ = 0;
    ਗੰਢ->all_ਮਾਪ = 0;

    ਗੰਢ->cur_max = 0;

    ਗੰਢ->link = ਨਿਰਾ;
    ਗੰਢ->hash = hash;
    *pnode     = ਗੰਢ;

  Exit:
    ਵਾਪਸ ਗੰਢ;
  }


  ਸਥਿਰ ਖਾਲੀ
  ft_mem_ਸਾਰਣੀ_ਸੈੱਟ( FT_Mem_ਸਾਰਣੀ  ਸਾਰਣੀ,
                    FT_Byte*     ਪਤਾ,
                    FT_Long      ਮਾਪ,
                    FT_Long      delta )
  {
    FT_Mem_ਗੰਢ  *pnode, ਗੰਢ;


    ਜੇ ( ਸਾਰਣੀ )
    {
      FT_Mem_ਸਰੋਤ  ਸਰੋਤ;


      pnode = ft_mem_ਸਾਰਣੀ_get_nodep( ਸਾਰਣੀ, ਪਤਾ );
      ਗੰਢ  = *pnode;
      ਜੇ ( ਗੰਢ )
      {
        ਜੇ ( ਗੰਢ->ਮਾਪ < 0 )
        {
          /* This block was already freed.  Our memory is now completely */
          /* corrupted!                                                  */
          /* This can only happen in keep-alive mode.                    */
          ft_mem_debug_panic(
            "memory heap corrupted (allocating freed block)" );
        }
        ਨਹੀਂਤਾਂ
        {
          /* This block was already allocated.  This means that our memory */
          /* is also corrupted!                                            */
          ft_mem_debug_panic(
            "memory heap corrupted (re-allocating allocated block at"
            " %p, of size %ld)\n"
            "org=%s:%d new=%s:%d\n",
            ਗੰਢ->ਪਤਾ, ਗੰਢ->ਮਾਪ,
            FT_FILENAME( ਗੰਢ->ਸਰੋਤ->file_name ), ਗੰਢ->ਸਰੋਤ->ਸਤਰ_no,
            FT_FILENAME( ft_debug_file_ ), ft_debug_lineno_ );
        }
      }

      /* we need to create a new node in this table */
      ਗੰਢ = (FT_Mem_ਗੰਢ)ft_mem_ਸਾਰਣੀ_alloc( ਸਾਰਣੀ, ਆਕਾਰ ( *ਗੰਢ ) );
      ਜੇ ( !ਗੰਢ )
        ft_mem_debug_panic( "not enough memory to run memory tests" );

      ਗੰਢ->ਪਤਾ = ਪਤਾ;
      ਗੰਢ->ਮਾਪ    = ਮਾਪ;
      ਗੰਢ->ਸਰੋਤ  = ਸਰੋਤ = ft_mem_ਸਾਰਣੀ_get_ਸਰੋਤ( ਸਾਰਣੀ );

      ਜੇ ( delta == 0 )
      {
        /* this is an allocation */
        ਸਰੋਤ->all_ਖੰਡਾਂ++;
        ਸਰੋਤ->cur_ਖੰਡਾਂ++;
        ਜੇ ( ਸਰੋਤ->cur_ਖੰਡਾਂ > ਸਰੋਤ->max_ਖੰਡਾਂ )
          ਸਰੋਤ->max_ਖੰਡਾਂ = ਸਰੋਤ->cur_ਖੰਡਾਂ;
      }

      ਜੇ ( ਮਾਪ > ਸਰੋਤ->cur_max )
        ਸਰੋਤ->cur_max = ਮਾਪ;

      ਜੇ ( delta != 0 )
      {
        /* we are growing or shrinking a reallocated block */
        ਸਰੋਤ->cur_ਮਾਪ     += delta;
        ਸਾਰਣੀ->alloc_current += delta;
      }
      ਨਹੀਂਤਾਂ
      {
        /* we are allocating a new block */
        ਸਰੋਤ->cur_ਮਾਪ     += ਮਾਪ;
        ਸਾਰਣੀ->alloc_current += ਮਾਪ;
      }

      ਸਰੋਤ->all_ਮਾਪ += ਮਾਪ;

      ਜੇ ( ਸਰੋਤ->cur_ਮਾਪ > ਸਰੋਤ->max_ਮਾਪ )
        ਸਰੋਤ->max_ਮਾਪ = ਸਰੋਤ->cur_ਮਾਪ;

      ਗੰਢ->free_file_name = ਨਿਰਾ;
      ਗੰਢ->free_ਸਤਰ_no   = 0;

      ਗੰਢ->link = pnode[0];

      pnode[0] = ਗੰਢ;
      ਸਾਰਣੀ->nodes++;

      ਸਾਰਣੀ->alloc_ਕੁੱਲ += ਮਾਪ;

      ਜੇ ( ਸਾਰਣੀ->alloc_current > ਸਾਰਣੀ->alloc_max )
        ਸਾਰਣੀ->alloc_max = ਸਾਰਣੀ->alloc_current;

      ਜੇ ( ਸਾਰਣੀ->nodes * 3 < ਸਾਰਣੀ->ਮਾਪ  ||
           ਸਾਰਣੀ->ਮਾਪ  * 3 < ਸਾਰਣੀ->nodes )
        ft_mem_ਸਾਰਣੀ_resize( ਸਾਰਣੀ );
    }
  }


  ਸਥਿਰ ਖਾਲੀ
  ft_mem_ਸਾਰਣੀ_remove( FT_Mem_ਸਾਰਣੀ  ਸਾਰਣੀ,
                       FT_Byte*     ਪਤਾ,
                       FT_Long      delta )
  {
    ਜੇ ( ਸਾਰਣੀ )
    {
      FT_Mem_ਗੰਢ  *pnode, ਗੰਢ;


      pnode = ft_mem_ਸਾਰਣੀ_get_nodep( ਸਾਰਣੀ, ਪਤਾ );
      ਗੰਢ  = *pnode;
      ਜੇ ( ਗੰਢ )
      {
        FT_Mem_ਸਰੋਤ  ਸਰੋਤ;


        ਜੇ ( ਗੰਢ->ਮਾਪ < 0 )
          ft_mem_debug_panic(
            "freeing memory block at %p more than once\n"
            "  at (%s:%ld)!\n"
            "  Block was allocated at (%s:%ld)\n"
            "  and released at (%s:%ld).",
            ਪਤਾ,
            FT_FILENAME( ft_debug_file_ ), ft_debug_lineno_,
            FT_FILENAME( ਗੰਢ->ਸਰੋਤ->file_name ), ਗੰਢ->ਸਰੋਤ->ਸਤਰ_no,
            FT_FILENAME( ਗੰਢ->free_file_name ), ਗੰਢ->free_ਸਤਰ_no );

        /* scramble the node's content for additional safety */
        FT_MEM_ਸੈੱਟ( ਪਤਾ, 0xF3, ਗੰਢ->ਮਾਪ );

        ਜੇ ( delta == 0 )
        {
          ਸਰੋਤ = ਗੰਢ->ਸਰੋਤ;

          ਸਰੋਤ->cur_ਖੰਡਾਂ--;
          ਸਰੋਤ->cur_ਮਾਪ -= ਗੰਢ->ਮਾਪ;

          ਸਾਰਣੀ->alloc_current -= ਗੰਢ->ਮਾਪ;
        }

        ਜੇ ( ਸਾਰਣੀ->keep_alive )
        {
          /* we simply invert the node's size to indicate that the node */
          /* was freed.                                                 */
          ਗੰਢ->ਮਾਪ           = -ਗੰਢ->ਮਾਪ;
          ਗੰਢ->free_file_name = ft_debug_file_;
          ਗੰਢ->free_ਸਤਰ_no   = ft_debug_lineno_;
        }
        ਨਹੀਂਤਾਂ
        {
          ਸਾਰਣੀ->nodes--;

          *pnode = ਗੰਢ->link;

          ਗੰਢ->ਮਾਪ   = 0;
          ਗੰਢ->ਸਰੋਤ = ਨਿਰਾ;

          ft_mem_ਸਾਰਣੀ_free( ਸਾਰਣੀ, ਗੰਢ );

          ਜੇ ( ਸਾਰਣੀ->nodes * 3 < ਸਾਰਣੀ->ਮਾਪ  ||
               ਸਾਰਣੀ->ਮਾਪ  * 3 < ਸਾਰਣੀ->nodes )
            ft_mem_ਸਾਰਣੀ_resize( ਸਾਰਣੀ );
        }
      }
      ਨਹੀਂਤਾਂ
        ft_mem_debug_panic(
          "trying to free unknown block at %p in (%s:%ld)\n",
          ਪਤਾ,
          FT_FILENAME( ft_debug_file_ ), ft_debug_lineno_ );
    }
  }


  ਸਥਿਰ FT_ਸੂਚਕ
  ft_mem_debug_alloc( FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ,
                      FT_Long    ਮਾਪ )
  {
    FT_Mem_ਸਾਰਣੀ  ਸਾਰਣੀ = (FT_Mem_ਸਾਰਣੀ)ਸਮ੍ਰਿਤੀ->user;
    FT_Byte*     ਖੰਡ;


    ਜੇ ( ਮਾਪ <= 0 )
      ft_mem_debug_panic( "negative block size allocation (%ld)", ਮਾਪ );

    /* return NULL if the maximum number of allocations was reached */
    ਜੇ ( ਸਾਰਣੀ->bound_ਗਿਣਤੀ                           &&
         ਸਾਰਣੀ->alloc_ਗਿਣਤੀ >= ਸਾਰਣੀ->alloc_ਗਿਣਤੀ_max )
      ਵਾਪਸ ਨਿਰਾ;

    /* return NULL if this allocation would overflow the maximum heap size */
    ਜੇ ( ਸਾਰਣੀ->bound_ਕੁੱਲ                                   &&
         ਸਾਰਣੀ->alloc_ਕੁੱਲ_max - ਸਾਰਣੀ->alloc_current > ਮਾਪ )
      ਵਾਪਸ ਨਿਰਾ;

    ਖੰਡ = (FT_Byte *)ft_mem_ਸਾਰਣੀ_alloc( ਸਾਰਣੀ, ਮਾਪ );
    ਜੇ ( ਖੰਡ )
    {
      ft_mem_ਸਾਰਣੀ_ਸੈੱਟ( ਸਾਰਣੀ, ਖੰਡ, ਮਾਪ, 0 );

      ਸਾਰਣੀ->alloc_ਗਿਣਤੀ++;
    }

    ft_debug_file_   = "<unknown>";
    ft_debug_lineno_ = 0;

    ਵਾਪਸ (FT_ਸੂਚਕ)ਖੰਡ;
  }


  ਸਥਿਰ ਖਾਲੀ
  ft_mem_debug_free( FT_ਸਮ੍ਰਿਤੀ   ਸਮ੍ਰਿਤੀ,
                     FT_ਸੂਚਕ  ਖੰਡ )
  {
    FT_Mem_ਸਾਰਣੀ  ਸਾਰਣੀ = (FT_Mem_ਸਾਰਣੀ)ਸਮ੍ਰਿਤੀ->user;


    ਜੇ ( !ਖੰਡ )
      ft_mem_debug_panic( "trying to free NULL in (%s:%ld)",
                          FT_FILENAME( ft_debug_file_ ),
                          ft_debug_lineno_ );

    ft_mem_ਸਾਰਣੀ_remove( ਸਾਰਣੀ, (FT_Byte*)ਖੰਡ, 0 );

    ਜੇ ( !ਸਾਰਣੀ->keep_alive )
      ft_mem_ਸਾਰਣੀ_free( ਸਾਰਣੀ, ਖੰਡ );

    ਸਾਰਣੀ->alloc_ਗਿਣਤੀ--;

    ft_debug_file_   = "<unknown>";
    ft_debug_lineno_ = 0;
  }


  ਸਥਿਰ FT_ਸੂਚਕ
  ft_mem_debug_realloc( FT_ਸਮ੍ਰਿਤੀ   ਸਮ੍ਰਿਤੀ,
                        FT_Long     cur_ਮਾਪ,
                        FT_Long     new_ਮਾਪ,
                        FT_ਸੂਚਕ  ਖੰਡ )
  {
    FT_Mem_ਸਾਰਣੀ  ਸਾਰਣੀ = (FT_Mem_ਸਾਰਣੀ)ਸਮ੍ਰਿਤੀ->user;
    FT_Mem_ਗੰਢ   ਗੰਢ, *pnode;
    FT_ਸੂਚਕ   new_ਖੰਡ;
    FT_Long      delta;

    ਅਚਲ ਅੱਖਰ*  file_name = FT_FILENAME( ft_debug_file_ );
    FT_Long      ਸਤਰ_no   = ft_debug_lineno_;


    /* unlikely, but possible */
    ਜੇ ( new_ਮਾਪ == cur_ਮਾਪ )
      ਵਾਪਸ ਖੰਡ;

    /* the following is valid according to ANSI C */
#ਜੇ 0
    ਜੇ ( !ਖੰਡ || !cur_ਮਾਪ )
      ft_mem_debug_panic( "trying to reallocate NULL in (%s:%ld)",
                          file_name, ਸਤਰ_no );
#ਜੇ_ਅੰਤ

    /* while the following is allowed in ANSI C also, we abort since */
    /* such case should be handled by FreeType.                      */
    ਜੇ ( new_ਮਾਪ <= 0 )
      ft_mem_debug_panic(
        "trying to reallocate %p to size 0 (current is %ld) in (%s:%ld)",
        ਖੰਡ, cur_ਮਾਪ, file_name, ਸਤਰ_no );

    /* check `cur_size' value */
    pnode = ft_mem_ਸਾਰਣੀ_get_nodep( ਸਾਰਣੀ, (FT_Byte*)ਖੰਡ );
    ਗੰਢ  = *pnode;
    ਜੇ ( !ਗੰਢ )
      ft_mem_debug_panic(
        "trying to reallocate unknown block at %p in (%s:%ld)",
        ਖੰਡ, file_name, ਸਤਰ_no );

    ਜੇ ( ਗੰਢ->ਮਾਪ <= 0 )
      ft_mem_debug_panic(
        "trying to reallocate freed block at %p in (%s:%ld)",
        ਖੰਡ, file_name, ਸਤਰ_no );

    ਜੇ ( ਗੰਢ->ਮਾਪ != cur_ਮਾਪ )
      ft_mem_debug_panic( "invalid ft_realloc request for %p. cur_size is "
                          "%ld instead of %ld in (%s:%ld)",
                          ਖੰਡ, cur_ਮਾਪ, ਗੰਢ->ਮਾਪ, file_name, ਸਤਰ_no );

    /* return NULL if the maximum number of allocations was reached */
    ਜੇ ( ਸਾਰਣੀ->bound_ਗਿਣਤੀ                           &&
         ਸਾਰਣੀ->alloc_ਗਿਣਤੀ >= ਸਾਰਣੀ->alloc_ਗਿਣਤੀ_max )
      ਵਾਪਸ ਨਿਰਾ;

    delta = new_ਮਾਪ - cur_ਮਾਪ;

    /* return NULL if this allocation would overflow the maximum heap size */
    ਜੇ ( delta > 0                                             &&
         ਸਾਰਣੀ->bound_ਕੁੱਲ                                    &&
         ਸਾਰਣੀ->alloc_current + delta > ਸਾਰਣੀ->alloc_ਕੁੱਲ_max )
      ਵਾਪਸ ਨਿਰਾ;

    new_ਖੰਡ = (FT_ਸੂਚਕ)ft_mem_ਸਾਰਣੀ_alloc( ਸਾਰਣੀ, new_ਮਾਪ );
    ਜੇ ( !new_ਖੰਡ )
      ਵਾਪਸ ਨਿਰਾ;

    ft_mem_ਸਾਰਣੀ_ਸੈੱਟ( ਸਾਰਣੀ, (FT_Byte*)new_ਖੰਡ, new_ਮਾਪ, delta );

    ft_memcpy( new_ਖੰਡ, ਖੰਡ, cur_ਮਾਪ < new_ਮਾਪ ? (size_t)cur_ਮਾਪ
                                                     : (size_t)new_ਮਾਪ );

    ft_mem_ਸਾਰਣੀ_remove( ਸਾਰਣੀ, (FT_Byte*)ਖੰਡ, delta );

    ft_debug_file_   = "<unknown>";
    ft_debug_lineno_ = 0;

    ਜੇ ( !ਸਾਰਣੀ->keep_alive )
      ft_mem_ਸਾਰਣੀ_free( ਸਾਰਣੀ, ਖੰਡ );

    ਵਾਪਸ new_ਖੰਡ;
  }


  ਬਾਹਰੀ ਖਾਲੀ
  ft_mem_debug_init( FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ )
  {
    FT_Mem_ਸਾਰਣੀ  ਸਾਰਣੀ;


    ਜੇ ( !ft_getenv( "FT2_DEBUG_MEMORY" ) )
      ਵਾਪਸ;

    ਸਾਰਣੀ = (FT_Mem_ਸਾਰਣੀ)ਸਮ੍ਰਿਤੀ->alloc( ਸਮ੍ਰਿਤੀ, ਆਕਾਰ ( *ਸਾਰਣੀ ) );

    ਜੇ ( ਸਾਰਣੀ )
    {
      FT_ZERO( ਸਾਰਣੀ );

      ਸਾਰਣੀ->ਸਮ੍ਰਿਤੀ      = ਸਮ੍ਰਿਤੀ;
      ਸਾਰਣੀ->ਸਮ੍ਰਿਤੀ_user = ਸਮ੍ਰਿਤੀ->user;
      ਸਾਰਣੀ->alloc       = ਸਮ੍ਰਿਤੀ->alloc;
      ਸਾਰਣੀ->realloc     = ਸਮ੍ਰਿਤੀ->realloc;
      ਸਾਰਣੀ->ਮੁਕਤ        = ਸਮ੍ਰਿਤੀ->ਮੁਕਤ;

      ft_mem_ਸਾਰਣੀ_resize( ਸਾਰਣੀ );

      ਜੇ ( ਸਾਰਣੀ->ਮਾਪ )
      {
        ਅਚਲ ਅੱਖਰ*  p;


        ਸਮ੍ਰਿਤੀ->user    = ਸਾਰਣੀ;
        ਸਮ੍ਰਿਤੀ->alloc   = ft_mem_debug_alloc;
        ਸਮ੍ਰਿਤੀ->realloc = ft_mem_debug_realloc;
        ਸਮ੍ਰਿਤੀ->ਮੁਕਤ    = ft_mem_debug_free;

        p = ft_getenv( "FT2_ALLOC_TOTAL_MAX" );
        ਜੇ ( p )
        {
          FT_Long  ਕੁੱਲ_max = ft_strtol( p, ਨਿਰਾ, 10 );


          ਜੇ ( ਕੁੱਲ_max > 0 )
          {
            ਸਾਰਣੀ->bound_ਕੁੱਲ     = 1;
            ਸਾਰਣੀ->alloc_ਕੁੱਲ_max = ਕੁੱਲ_max;
          }
        }

        p = ft_getenv( "FT2_ALLOC_COUNT_MAX" );
        ਜੇ ( p )
        {
          FT_Long  ਕੁੱਲ_ਗਿਣਤੀ = ft_strtol( p, ਨਿਰਾ, 10 );


          ਜੇ ( ਕੁੱਲ_ਗਿਣਤੀ > 0 )
          {
            ਸਾਰਣੀ->bound_ਗਿਣਤੀ     = 1;
            ਸਾਰਣੀ->alloc_ਗਿਣਤੀ_max = ਕੁੱਲ_ਗਿਣਤੀ;
          }
        }

        p = ft_getenv( "FT2_KEEP_ALIVE" );
        ਜੇ ( p )
        {
          FT_Long  keep_alive = ft_strtol( p, ਨਿਰਾ, 10 );


          ਜੇ ( keep_alive > 0 )
            ਸਾਰਣੀ->keep_alive = 1;
        }
      }
      ਨਹੀਂਤਾਂ
        ਸਮ੍ਰਿਤੀ->ਮੁਕਤ( ਸਮ੍ਰਿਤੀ, ਸਾਰਣੀ );
    }
  }


  ਬਾਹਰੀ ਖਾਲੀ
  ft_mem_debug_ਪੂਰਾ( FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ )
  {
    ਜੇ ( ਸਮ੍ਰਿਤੀ->ਮੁਕਤ == ft_mem_debug_free )
    {
      FT_Mem_ਸਾਰਣੀ  ਸਾਰਣੀ = (FT_Mem_ਸਾਰਣੀ)ਸਮ੍ਰਿਤੀ->user;


      FT_Dump_ਸਮ੍ਰਿਤੀ( ਸਮ੍ਰਿਤੀ );

      ft_mem_ਸਾਰਣੀ_destroy( ਸਾਰਣੀ );

      ਸਮ੍ਰਿਤੀ->ਮੁਕਤ    = ਸਾਰਣੀ->ਮੁਕਤ;
      ਸਮ੍ਰਿਤੀ->realloc = ਸਾਰਣੀ->realloc;
      ਸਮ੍ਰਿਤੀ->alloc   = ਸਾਰਣੀ->alloc;
      ਸਮ੍ਰਿਤੀ->user    = ਸਾਰਣੀ->ਸਮ੍ਰਿਤੀ_user;

      ਸਮ੍ਰਿਤੀ->ਮੁਕਤ( ਸਮ੍ਰਿਤੀ, ਸਾਰਣੀ );
    }
  }


  FT_COMPARE_DEF( ਪੂਰਨ )
  ft_mem_ਸਰੋਤ_compare( ਅਚਲ ਖਾਲੀ*  p1,
                         ਅਚਲ ਖਾਲੀ*  p2 )
  {
    FT_Mem_ਸਰੋਤ  s1 = *(FT_Mem_ਸਰੋਤ*)p1;
    FT_Mem_ਸਰੋਤ  s2 = *(FT_Mem_ਸਰੋਤ*)p2;


    ਜੇ ( s2->max_ਮਾਪ > s1->max_ਮਾਪ )
      ਵਾਪਸ 1;
    ਨਹੀਂਤਾਂ ਜੇ ( s2->max_ਮਾਪ < s1->max_ਮਾਪ )
      ਵਾਪਸ -1;
    ਨਹੀਂਤਾਂ
      ਵਾਪਸ 0;
  }


  ਬਾਹਰੀ ਖਾਲੀ
  FT_Dump_ਸਮ੍ਰਿਤੀ( FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ )
  {
    ਜੇ ( ਸਮ੍ਰਿਤੀ->ਮੁਕਤ == ft_mem_debug_free )
    {
      FT_Mem_ਸਾਰਣੀ    ਸਾਰਣੀ = (FT_Mem_ਸਾਰਣੀ)ਸਮ੍ਰਿਤੀ->user;
      FT_Mem_ਸਰੋਤ*  ਬਾਲਟੀ = ਸਾਰਣੀ->sources;
      FT_Mem_ਸਰੋਤ*  ਹੱਦਬੰਦੀ  = ਬਾਲਟੀ + FT_MEM_ਸਰੋਤ_BUCKETS;
      FT_Mem_ਸਰੋਤ*  sources;
      FT_Int         nn, ਗਿਣਤੀ_2;
      ਅਚਲ ਅੱਖਰ*    fmt;


      ਗਿਣਤੀ_2 = 0;
      ਲਈ ( ; ਬਾਲਟੀ < ਹੱਦਬੰਦੀ; ਬਾਲਟੀ++ )
      {
        FT_Mem_ਸਰੋਤ  ਸਰੋਤ = *ਬਾਲਟੀ;


        ਲਈ ( ; ਸਰੋਤ; ਸਰੋਤ = ਸਰੋਤ->link )
          ਗਿਣਤੀ_2++;
      }

      sources = (FT_Mem_ਸਰੋਤ*)
                  ft_mem_ਸਾਰਣੀ_alloc(
                    ਸਾਰਣੀ, ਗਿਣਤੀ_2 * (FT_Long)ਆਕਾਰ ( *sources ) );

      ਗਿਣਤੀ_2 = 0;
      ਲਈ ( ਬਾਲਟੀ = ਸਾਰਣੀ->sources; ਬਾਲਟੀ < ਹੱਦਬੰਦੀ; ਬਾਲਟੀ++ )
      {
        FT_Mem_ਸਰੋਤ  ਸਰੋਤ = *ਬਾਲਟੀ;


        ਲਈ ( ; ਸਰੋਤ; ਸਰੋਤ = ਸਰੋਤ->link )
          sources[ਗਿਣਤੀ_2++] = ਸਰੋਤ;
      }

      ft_qsort( sources,
                (size_t)ਗਿਣਤੀ_2,
                ਆਕਾਰ ( *sources ),
                ft_mem_ਸਰੋਤ_compare );

      ਲਿਖੋ( "FreeType Memory Dump: "
              "current=%ld max=%ld total=%ld count=%ld\n",
              ਸਾਰਣੀ->alloc_current, ਸਾਰਣੀ->alloc_max,
              ਸਾਰਣੀ->alloc_ਕੁੱਲ, ਸਾਰਣੀ->alloc_ਗਿਣਤੀ );
      ਲਿਖੋ( " block  block    sizes    sizes    sizes   source\n" );
      ਲਿਖੋ( " count   high      sum  highsum      max   location\n" );
      ਲਿਖੋ( "-------------------------------------------------\n" );

      fmt = "%6ld %6ld %8ld %8ld %8ld %s:%d\n";

      ਲਈ ( nn = 0; nn < ਗਿਣਤੀ_2; nn++ )
      {
        FT_Mem_ਸਰੋਤ  ਸਰੋਤ = sources[nn];


        ਲਿਖੋ( fmt,
                ਸਰੋਤ->cur_ਖੰਡਾਂ, ਸਰੋਤ->max_ਖੰਡਾਂ,
                ਸਰੋਤ->cur_ਮਾਪ, ਸਰੋਤ->max_ਮਾਪ, ਸਰੋਤ->cur_max,
                FT_FILENAME( ਸਰੋਤ->file_name ),
                ਸਰੋਤ->ਸਤਰ_no );
      }
      ਲਿਖੋ( "------------------------------------------------\n" );

      ft_mem_ਸਾਰਣੀ_free( ਸਾਰਣੀ, sources );
    }
  }

#ਨਹੀਂਤਾਂ  /* !FT_DEBUG_MEMORY */

  /* ANSI C doesn't like empty source files */
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਪੂਰਨ  debug_mem_dummy_;

#ਜੇ_ਅੰਤ /* !FT_DEBUG_MEMORY */


/*ਸਿਰਾ*/
