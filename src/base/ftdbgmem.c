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


#ضمن <ft2build.h>
#ضمن FT_CONFIG_CONFIG_H
#ضمن <freetype/internal/ftdebug.h>
#ضمن <freetype/internal/ftmemory.h>
#ضمن <freetype/ftsystem.h>
#ضمن <freetype/fterrors.h>
#ضمن <freetype/fttypes.h>


#جيڪڏهن_وصف_ٿيل FT_DEBUG_يادگيري

#وصف_ڪريو  KEEPALIVE /* `Keep alive' means that freed blocks aren't released
                    * to the heap.  This is useful to detect double-frees
                    * or weird heap corruption, but it uses large amounts of
                    * memory, however.
                    */

#ضمن FT_CONFIG_STANDARD_LIBRARY_H

  FT_بنياد_DEF( ثابت حرف* )  ft_debug_file_   = عدم;
  FT_بنياد_DEF( ڊگهو )         ft_debug_lineno_ = 0;

  خارجي فراغ
  FT_Dump_يادگيري( FT_يادگيري  يادگيري );


  تعريفنوع بنية FT_Mem_ذريعو_Rec*  FT_Mem_ذريعو;
  تعريفنوع بنية FT_Mem_ڳنڍ_Rec*    FT_Mem_ڳنڍ;
  تعريفنوع بنية FT_Mem_جدول_Rec*   FT_Mem_جدول;


#وصف_ڪريو FT_MEM_VAL( addr )  ( (FT_PtrDist)(FT_اشارو)( addr ) )

  /*
   * This structure holds statistics for a single allocation/release
   * site.  This is useful to know where memory operations happen the
   * most.
   */
  تعريفنوع بنية  FT_Mem_ذريعو_Rec
  {
    ثابت حرف*   file_name;
    ڊگهو          سٽ_no;

    FT_Long       cur_بلاڪون;   /* current number of allocated blocks */
    FT_Long       max_بلاڪون;   /* max. number of allocated blocks    */
    FT_Long       all_بلاڪون;   /* total number of blocks allocated   */

    FT_Long       cur_سائيز;     /* current cumulative allocated size */
    FT_Long       max_سائيز;     /* maximum cumulative allocated size */
    FT_Long       all_سائيز;     /* total cumulative allocated size   */

    FT_Long       cur_max;      /* current maximum allocated size */

    FT_UInt32     hash;
    FT_Mem_ذريعو  link;

  } FT_Mem_ذريعو_Rec_2;


  /*
   * We don't need a resizable array for the memory sources because
   * their number is pretty limited within FreeType.
   */
#وصف_ڪريو FT_MEM_ذريعو_BUCKETS  128

  /*
   * This structure holds information related to a single allocated
   * memory block.  If KEEPALIVE is defined, blocks that are freed by
   * FreeType are never released to the system.  Instead, their `size'
   * field is set to `-size'.  This is mainly useful to detect double
   * frees, at the price of a large memory footprint during execution.
   */
  تعريفنوع بنية  FT_Mem_ڳنڍ_Rec
  {
    FT_Byte*      پتو;
    FT_Long       سائيز;     /* < 0 if the block was freed */

    FT_Mem_ذريعو  ذريعو;

#جيڪڏهن_وصف_ٿيل KEEPALIVE
    ثابت حرف*   free_file_name;
    FT_Long       free_سٽ_no;
#جيڪڏهن_پورو

    FT_Mem_ڳنڍ    link;

  } FT_Mem_ڳنڍ_Rec_2;


  /*
   * The global structure, containing compound statistics and all hash
   * tables.
   */
  تعريفنوع بنية  FT_Mem_جدول_Rec
  {
    FT_Long          سائيز;
    FT_Long          nodes;
    FT_Mem_ڳنڍ*      buckets;

    FT_Long          alloc_ڪل;
    FT_Long          alloc_current;
    FT_Long          alloc_max;
    FT_Long          alloc_ڳڻپ;

    FT_Bool          bound_ڪل;
    FT_Long          alloc_ڪل_max;

    FT_Bool          bound_ڳڻپ;
    FT_Long          alloc_ڳڻپ_max;

    FT_Mem_ذريعو     sources[FT_MEM_ذريعو_BUCKETS];

    FT_Bool          keep_alive;

    FT_يادگيري        يادگيري;
    FT_اشارو       يادگيري_user;
    FT_Alloc_Func    alloc;
    FT_Free_Func     حرر;
    FT_Realloc_Func  realloc;

  } FT_Mem_جدول_Rec_2;


#وصف_ڪريو FT_MEM_سائيز_MAX  13845163

#وصف_ڪريو FT_FILENAME( x )  ( (x) ? (x) : "unknown file" )


  /*
   * Prime numbers are ugly to handle.  It would be better to implement
   * L-Hashing, which is 10% faster and doesn't require divisions.
   */
  ساكن ثابت FT_Int  ft_mem_مفرد_انگ[] =
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


  ساكن FT_Long
  ft_mem_closest_مفرد( FT_Long  انگ )
  {
    size_t  i;


    لكل ( i = 0;
          i < حجم ( ft_mem_مفرد_انگ ) / حجم ( ft_mem_مفرد_انگ[0] ); i++ )
      إذا ( ft_mem_مفرد_انگ[i] > انگ )
        أرجع ft_mem_مفرد_انگ[i];

    أرجع FT_MEM_سائيز_MAX;
  }


  ساكن فراغ
  ft_mem_debug_panic( ثابت حرف*  fmt,
                      ... )
  {
    va_list  ap;


    لکو( "FreeType.Debug: " );

    va_شروعات( ap, fmt );
    vprintf( fmt, ap );
    va_پڇاڙي( ap );

    لکو( "\n" );
    ٻاهر_نڪرو( EXIT_FAILURE );
  }


  ساكن FT_اشارو
  ft_mem_جدول_alloc( FT_Mem_جدول  جدول,
                      FT_Long      سائيز )
  {
    FT_يادگيري   يادگيري = جدول->يادگيري;
    FT_اشارو  بلاڪ;


    يادگيري->user = جدول->يادگيري_user;
    بلاڪ = جدول->alloc( يادگيري, سائيز );
    يادگيري->user = جدول;

    أرجع بلاڪ;
  }


  ساكن فراغ
  ft_mem_جدول_free( FT_Mem_جدول  جدول,
                     FT_اشارو   بلاڪ )
  {
    FT_يادگيري  يادگيري = جدول->يادگيري;


    يادگيري->user = جدول->يادگيري_user;
    جدول->حرر( يادگيري, بلاڪ );
    يادگيري->user = جدول;
  }


  ساكن فراغ
  ft_mem_جدول_resize( FT_Mem_جدول  جدول )
  {
    FT_Long  new_سائيز;


    new_سائيز = ft_mem_closest_مفرد( جدول->nodes );
    إذا ( new_سائيز != جدول->سائيز )
    {
      FT_Mem_ڳنڍ*  new_buckets;
      FT_Long      i;


      new_buckets = (FT_Mem_ڳنڍ *)
                      ft_mem_جدول_alloc(
                        جدول,
                        new_سائيز * (FT_Long)حجم ( FT_Mem_ڳنڍ ) );
      إذا ( !new_buckets )
        أرجع;

      FT_ARRAY_ZERO( new_buckets, new_سائيز );

      لكل ( i = 0; i < جدول->سائيز; i++ )
      {
        FT_Mem_ڳنڍ  ڳنڍ, ايندڙ, *pnode;
        FT_PtrDist  hash;


        ڳنڍ = جدول->buckets[i];
        طالما ( ڳنڍ )
        {
          ايندڙ  = ڳنڍ->link;
          hash  = FT_MEM_VAL( ڳنڍ->پتو ) % (FT_PtrDist)new_سائيز;
          pnode = new_buckets + hash;

          ڳنڍ->link = pnode[0];
          pnode[0]   = ڳنڍ;

          ڳنڍ = ايندڙ;
        }
      }

      إذا ( جدول->buckets )
        ft_mem_جدول_free( جدول, جدول->buckets );

      جدول->buckets = new_buckets;
      جدول->سائيز    = new_سائيز;
    }
  }


  ساكن فراغ
  ft_mem_جدول_destroy( FT_Mem_جدول  جدول )
  {
    FT_Long  i;
    FT_Long  leak_ڳڻپ = 0;
    FT_Long  leaks      = 0;


    /* remove all blocks from the table, revealing leaked ones */
    لكل ( i = 0; i < جدول->سائيز; i++ )
    {
      FT_Mem_ڳنڍ  *pnode = جدول->buckets + i, ايندڙ, ڳنڍ = *pnode;


      طالما ( ڳنڍ )
      {
        ايندڙ       = ڳنڍ->link;
        ڳنڍ->link = عدم;

        إذا ( ڳنڍ->سائيز > 0 )
        {
          لکو(
            "leaked memory block at address %p, size %8ld in (%s:%ld)\n",
            (فراغ*)ڳنڍ->پتو,
            ڳنڍ->سائيز,
            FT_FILENAME( ڳنڍ->ذريعو->file_name ),
            ڳنڍ->ذريعو->سٽ_no );

          leak_ڳڻپ++;
          leaks += ڳنڍ->سائيز;

          ft_mem_جدول_free( جدول, ڳنڍ->پتو );
        }

        ڳنڍ->پتو = عدم;
        ڳنڍ->سائيز    = 0;

        ft_mem_جدول_free( جدول, ڳنڍ );
        ڳنڍ = ايندڙ;
      }
      جدول->buckets[i] = عدم;
    }

    ft_mem_جدول_free( جدول, جدول->buckets );
    جدول->buckets = عدم;

    جدول->سائيز  = 0;
    جدول->nodes = 0;

    /* remove all sources */
    لكل ( i = 0; i < FT_MEM_ذريعو_BUCKETS; i++ )
    {
      FT_Mem_ذريعو  ذريعو, ايندڙ;


      لكل ( ذريعو = جدول->sources[i]; ذريعو != عدم; ذريعو = ايندڙ )
      {
        ايندڙ = ذريعو->link;
        ft_mem_جدول_free( جدول, ذريعو );
      }

      جدول->sources[i] = عدم;
    }

    لکو( "FreeType: total memory allocations = %ld\n",
            جدول->alloc_ڪل );
    لکو( "FreeType: maximum memory footprint = %ld\n",
            جدول->alloc_max );

    إذا ( leak_ڳڻپ > 0 )
      ft_mem_debug_panic(
        "FreeType: %ld bytes of memory leaked in %ld blocks\n",
        leaks, leak_ڳڻپ );

    لکو( "FreeType: no memory leaks detected\n" );
  }


  ساكن FT_Mem_ڳنڍ*
  ft_mem_جدول_get_nodep( FT_Mem_جدول  جدول,
                          FT_Byte*     پتو )
  {
    FT_PtrDist   hash;
    FT_Mem_ڳنڍ  *pnode, ڳنڍ;


    hash  = FT_MEM_VAL( پتو );
    pnode = جدول->buckets + ( hash % (FT_PtrDist)جدول->سائيز );

    لكل (;;)
    {
      ڳنڍ = pnode[0];
      إذا ( !ڳنڍ )
        اكسر;

      إذا ( ڳنڍ->پتو == پتو )
        اكسر;

      pnode = &ڳنڍ->link;
    }
    أرجع pnode;
  }


  ساكن FT_Mem_ذريعو
  ft_mem_جدول_get_ذريعو( FT_Mem_جدول  جدول )
  {
    FT_UInt32     hash;
    FT_Mem_ذريعو  ڳنڍ, *pnode;


    /* cast to FT_PtrDist first since void* can be larger */
    /* than FT_UInt32 and GCC 4.1.1 emits a warning       */
    hash  = (FT_UInt32)(FT_PtrDist)(فراغ*)ft_debug_file_ +
              (FT_UInt32)( 5 * ft_debug_lineno_ );
    pnode = &جدول->sources[hash % FT_MEM_ذريعو_BUCKETS];

    لكل (;;)
    {
      ڳنڍ = *pnode;
      إذا ( !ڳنڍ )
        اكسر;

      إذا ( ڳنڍ->file_name == ft_debug_file_   &&
           ڳنڍ->سٽ_no   == ft_debug_lineno_ )
        اذهب Exit;

      pnode = &ڳنڍ->link;
    }

    ڳنڍ = (FT_Mem_ذريعو)ft_mem_جدول_alloc( جدول, حجم ( *ڳنڍ ) );
    إذا ( !ڳنڍ )
      ft_mem_debug_panic(
        "not enough memory to perform memory debugging\n" );

    ڳنڍ->file_name = ft_debug_file_;
    ڳنڍ->سٽ_no   = ft_debug_lineno_;

    ڳنڍ->cur_بلاڪون = 0;
    ڳنڍ->max_بلاڪون = 0;
    ڳنڍ->all_بلاڪون = 0;

    ڳنڍ->cur_سائيز = 0;
    ڳنڍ->max_سائيز = 0;
    ڳنڍ->all_سائيز = 0;

    ڳنڍ->cur_max = 0;

    ڳنڍ->link = عدم;
    ڳنڍ->hash = hash;
    *pnode     = ڳنڍ;

  Exit:
    أرجع ڳنڍ;
  }


  ساكن فراغ
  ft_mem_جدول_سيٽ( FT_Mem_جدول  جدول,
                    FT_Byte*     پتو,
                    FT_Long      سائيز,
                    FT_Long      delta )
  {
    FT_Mem_ڳنڍ  *pnode, ڳنڍ;


    إذا ( جدول )
    {
      FT_Mem_ذريعو  ذريعو;


      pnode = ft_mem_جدول_get_nodep( جدول, پتو );
      ڳنڍ  = *pnode;
      إذا ( ڳنڍ )
      {
        إذا ( ڳنڍ->سائيز < 0 )
        {
          /* This block was already freed.  Our memory is now completely */
          /* corrupted!                                                  */
          /* This can only happen in keep-alive mode.                    */
          ft_mem_debug_panic(
            "memory heap corrupted (allocating freed block)" );
        }
        وإلا
        {
          /* This block was already allocated.  This means that our memory */
          /* is also corrupted!                                            */
          ft_mem_debug_panic(
            "memory heap corrupted (re-allocating allocated block at"
            " %p, of size %ld)\n"
            "org=%s:%d new=%s:%d\n",
            ڳنڍ->پتو, ڳنڍ->سائيز,
            FT_FILENAME( ڳنڍ->ذريعو->file_name ), ڳنڍ->ذريعو->سٽ_no,
            FT_FILENAME( ft_debug_file_ ), ft_debug_lineno_ );
        }
      }

      /* we need to create a new node in this table */
      ڳنڍ = (FT_Mem_ڳنڍ)ft_mem_جدول_alloc( جدول, حجم ( *ڳنڍ ) );
      إذا ( !ڳنڍ )
        ft_mem_debug_panic( "not enough memory to run memory tests" );

      ڳنڍ->پتو = پتو;
      ڳنڍ->سائيز    = سائيز;
      ڳنڍ->ذريعو  = ذريعو = ft_mem_جدول_get_ذريعو( جدول );

      إذا ( delta == 0 )
      {
        /* this is an allocation */
        ذريعو->all_بلاڪون++;
        ذريعو->cur_بلاڪون++;
        إذا ( ذريعو->cur_بلاڪون > ذريعو->max_بلاڪون )
          ذريعو->max_بلاڪون = ذريعو->cur_بلاڪون;
      }

      إذا ( سائيز > ذريعو->cur_max )
        ذريعو->cur_max = سائيز;

      إذا ( delta != 0 )
      {
        /* we are growing or shrinking a reallocated block */
        ذريعو->cur_سائيز     += delta;
        جدول->alloc_current += delta;
      }
      وإلا
      {
        /* we are allocating a new block */
        ذريعو->cur_سائيز     += سائيز;
        جدول->alloc_current += سائيز;
      }

      ذريعو->all_سائيز += سائيز;

      إذا ( ذريعو->cur_سائيز > ذريعو->max_سائيز )
        ذريعو->max_سائيز = ذريعو->cur_سائيز;

      ڳنڍ->free_file_name = عدم;
      ڳنڍ->free_سٽ_no   = 0;

      ڳنڍ->link = pnode[0];

      pnode[0] = ڳنڍ;
      جدول->nodes++;

      جدول->alloc_ڪل += سائيز;

      إذا ( جدول->alloc_current > جدول->alloc_max )
        جدول->alloc_max = جدول->alloc_current;

      إذا ( جدول->nodes * 3 < جدول->سائيز  ||
           جدول->سائيز  * 3 < جدول->nodes )
        ft_mem_جدول_resize( جدول );
    }
  }


  ساكن فراغ
  ft_mem_جدول_remove( FT_Mem_جدول  جدول,
                       FT_Byte*     پتو,
                       FT_Long      delta )
  {
    إذا ( جدول )
    {
      FT_Mem_ڳنڍ  *pnode, ڳنڍ;


      pnode = ft_mem_جدول_get_nodep( جدول, پتو );
      ڳنڍ  = *pnode;
      إذا ( ڳنڍ )
      {
        FT_Mem_ذريعو  ذريعو;


        إذا ( ڳنڍ->سائيز < 0 )
          ft_mem_debug_panic(
            "freeing memory block at %p more than once\n"
            "  at (%s:%ld)!\n"
            "  Block was allocated at (%s:%ld)\n"
            "  and released at (%s:%ld).",
            پتو,
            FT_FILENAME( ft_debug_file_ ), ft_debug_lineno_,
            FT_FILENAME( ڳنڍ->ذريعو->file_name ), ڳنڍ->ذريعو->سٽ_no,
            FT_FILENAME( ڳنڍ->free_file_name ), ڳنڍ->free_سٽ_no );

        /* scramble the node's content for additional safety */
        FT_MEM_سيٽ( پتو, 0xF3, ڳنڍ->سائيز );

        إذا ( delta == 0 )
        {
          ذريعو = ڳنڍ->ذريعو;

          ذريعو->cur_بلاڪون--;
          ذريعو->cur_سائيز -= ڳنڍ->سائيز;

          جدول->alloc_current -= ڳنڍ->سائيز;
        }

        إذا ( جدول->keep_alive )
        {
          /* we simply invert the node's size to indicate that the node */
          /* was freed.                                                 */
          ڳنڍ->سائيز           = -ڳنڍ->سائيز;
          ڳنڍ->free_file_name = ft_debug_file_;
          ڳنڍ->free_سٽ_no   = ft_debug_lineno_;
        }
        وإلا
        {
          جدول->nodes--;

          *pnode = ڳنڍ->link;

          ڳنڍ->سائيز   = 0;
          ڳنڍ->ذريعو = عدم;

          ft_mem_جدول_free( جدول, ڳنڍ );

          إذا ( جدول->nodes * 3 < جدول->سائيز  ||
               جدول->سائيز  * 3 < جدول->nodes )
            ft_mem_جدول_resize( جدول );
        }
      }
      وإلا
        ft_mem_debug_panic(
          "trying to free unknown block at %p in (%s:%ld)\n",
          پتو,
          FT_FILENAME( ft_debug_file_ ), ft_debug_lineno_ );
    }
  }


  ساكن FT_اشارو
  ft_mem_debug_alloc( FT_يادگيري  يادگيري,
                      FT_Long    سائيز )
  {
    FT_Mem_جدول  جدول = (FT_Mem_جدول)يادگيري->user;
    FT_Byte*     بلاڪ;


    إذا ( سائيز <= 0 )
      ft_mem_debug_panic( "negative block size allocation (%ld)", سائيز );

    /* return NULL if the maximum number of allocations was reached */
    إذا ( جدول->bound_ڳڻپ                           &&
         جدول->alloc_ڳڻپ >= جدول->alloc_ڳڻپ_max )
      أرجع عدم;

    /* return NULL if this allocation would overflow the maximum heap size */
    إذا ( جدول->bound_ڪل                                   &&
         جدول->alloc_ڪل_max - جدول->alloc_current > سائيز )
      أرجع عدم;

    بلاڪ = (FT_Byte *)ft_mem_جدول_alloc( جدول, سائيز );
    إذا ( بلاڪ )
    {
      ft_mem_جدول_سيٽ( جدول, بلاڪ, سائيز, 0 );

      جدول->alloc_ڳڻپ++;
    }

    ft_debug_file_   = "<unknown>";
    ft_debug_lineno_ = 0;

    أرجع (FT_اشارو)بلاڪ;
  }


  ساكن فراغ
  ft_mem_debug_free( FT_يادگيري   يادگيري,
                     FT_اشارو  بلاڪ )
  {
    FT_Mem_جدول  جدول = (FT_Mem_جدول)يادگيري->user;


    إذا ( !بلاڪ )
      ft_mem_debug_panic( "trying to free NULL in (%s:%ld)",
                          FT_FILENAME( ft_debug_file_ ),
                          ft_debug_lineno_ );

    ft_mem_جدول_remove( جدول, (FT_Byte*)بلاڪ, 0 );

    إذا ( !جدول->keep_alive )
      ft_mem_جدول_free( جدول, بلاڪ );

    جدول->alloc_ڳڻپ--;

    ft_debug_file_   = "<unknown>";
    ft_debug_lineno_ = 0;
  }


  ساكن FT_اشارو
  ft_mem_debug_realloc( FT_يادگيري   يادگيري,
                        FT_Long     cur_سائيز,
                        FT_Long     new_سائيز,
                        FT_اشارو  بلاڪ )
  {
    FT_Mem_جدول  جدول = (FT_Mem_جدول)يادگيري->user;
    FT_Mem_ڳنڍ   ڳنڍ, *pnode;
    FT_اشارو   new_بلاڪ;
    FT_Long      delta;

    ثابت حرف*  file_name = FT_FILENAME( ft_debug_file_ );
    FT_Long      سٽ_no   = ft_debug_lineno_;


    /* unlikely, but possible */
    إذا ( new_سائيز == cur_سائيز )
      أرجع بلاڪ;

    /* the following is valid according to ANSI C */
#إذا 0
    إذا ( !بلاڪ || !cur_سائيز )
      ft_mem_debug_panic( "trying to reallocate NULL in (%s:%ld)",
                          file_name, سٽ_no );
#جيڪڏهن_پورو

    /* while the following is allowed in ANSI C also, we abort since */
    /* such case should be handled by FreeType.                      */
    إذا ( new_سائيز <= 0 )
      ft_mem_debug_panic(
        "trying to reallocate %p to size 0 (current is %ld) in (%s:%ld)",
        بلاڪ, cur_سائيز, file_name, سٽ_no );

    /* check `cur_size' value */
    pnode = ft_mem_جدول_get_nodep( جدول, (FT_Byte*)بلاڪ );
    ڳنڍ  = *pnode;
    إذا ( !ڳنڍ )
      ft_mem_debug_panic(
        "trying to reallocate unknown block at %p in (%s:%ld)",
        بلاڪ, file_name, سٽ_no );

    إذا ( ڳنڍ->سائيز <= 0 )
      ft_mem_debug_panic(
        "trying to reallocate freed block at %p in (%s:%ld)",
        بلاڪ, file_name, سٽ_no );

    إذا ( ڳنڍ->سائيز != cur_سائيز )
      ft_mem_debug_panic( "invalid ft_realloc request for %p. cur_size is "
                          "%ld instead of %ld in (%s:%ld)",
                          بلاڪ, cur_سائيز, ڳنڍ->سائيز, file_name, سٽ_no );

    /* return NULL if the maximum number of allocations was reached */
    إذا ( جدول->bound_ڳڻپ                           &&
         جدول->alloc_ڳڻپ >= جدول->alloc_ڳڻپ_max )
      أرجع عدم;

    delta = new_سائيز - cur_سائيز;

    /* return NULL if this allocation would overflow the maximum heap size */
    إذا ( delta > 0                                             &&
         جدول->bound_ڪل                                    &&
         جدول->alloc_current + delta > جدول->alloc_ڪل_max )
      أرجع عدم;

    new_بلاڪ = (FT_اشارو)ft_mem_جدول_alloc( جدول, new_سائيز );
    إذا ( !new_بلاڪ )
      أرجع عدم;

    ft_mem_جدول_سيٽ( جدول, (FT_Byte*)new_بلاڪ, new_سائيز, delta );

    ft_memcpy( new_بلاڪ, بلاڪ, cur_سائيز < new_سائيز ? (size_t)cur_سائيز
                                                     : (size_t)new_سائيز );

    ft_mem_جدول_remove( جدول, (FT_Byte*)بلاڪ, delta );

    ft_debug_file_   = "<unknown>";
    ft_debug_lineno_ = 0;

    إذا ( !جدول->keep_alive )
      ft_mem_جدول_free( جدول, بلاڪ );

    أرجع new_بلاڪ;
  }


  خارجي فراغ
  ft_mem_debug_init( FT_يادگيري  يادگيري )
  {
    FT_Mem_جدول  جدول;


    إذا ( !ft_getenv( "FT2_DEBUG_MEMORY" ) )
      أرجع;

    جدول = (FT_Mem_جدول)يادگيري->alloc( يادگيري, حجم ( *جدول ) );

    إذا ( جدول )
    {
      FT_ZERO( جدول );

      جدول->يادگيري      = يادگيري;
      جدول->يادگيري_user = يادگيري->user;
      جدول->alloc       = يادگيري->alloc;
      جدول->realloc     = يادگيري->realloc;
      جدول->حرر        = يادگيري->حرر;

      ft_mem_جدول_resize( جدول );

      إذا ( جدول->سائيز )
      {
        ثابت حرف*  p;


        يادگيري->user    = جدول;
        يادگيري->alloc   = ft_mem_debug_alloc;
        يادگيري->realloc = ft_mem_debug_realloc;
        يادگيري->حرر    = ft_mem_debug_free;

        p = ft_getenv( "FT2_ALLOC_TOTAL_MAX" );
        إذا ( p )
        {
          FT_Long  ڪل_max = ft_strtol( p, عدم, 10 );


          إذا ( ڪل_max > 0 )
          {
            جدول->bound_ڪل     = 1;
            جدول->alloc_ڪل_max = ڪل_max;
          }
        }

        p = ft_getenv( "FT2_ALLOC_COUNT_MAX" );
        إذا ( p )
        {
          FT_Long  ڪل_ڳڻپ = ft_strtol( p, عدم, 10 );


          إذا ( ڪل_ڳڻپ > 0 )
          {
            جدول->bound_ڳڻپ     = 1;
            جدول->alloc_ڳڻپ_max = ڪل_ڳڻپ;
          }
        }

        p = ft_getenv( "FT2_KEEP_ALIVE" );
        إذا ( p )
        {
          FT_Long  keep_alive = ft_strtol( p, عدم, 10 );


          إذا ( keep_alive > 0 )
            جدول->keep_alive = 1;
        }
      }
      وإلا
        يادگيري->حرر( يادگيري, جدول );
    }
  }


  خارجي فراغ
  ft_mem_debug_پورو( FT_يادگيري  يادگيري )
  {
    إذا ( يادگيري->حرر == ft_mem_debug_free )
    {
      FT_Mem_جدول  جدول = (FT_Mem_جدول)يادگيري->user;


      FT_Dump_يادگيري( يادگيري );

      ft_mem_جدول_destroy( جدول );

      يادگيري->حرر    = جدول->حرر;
      يادگيري->realloc = جدول->realloc;
      يادگيري->alloc   = جدول->alloc;
      يادگيري->user    = جدول->يادگيري_user;

      يادگيري->حرر( يادگيري, جدول );
    }
  }


  FT_COMPARE_DEF( صحيح )
  ft_mem_ذريعو_compare( ثابت فراغ*  p1,
                         ثابت فراغ*  p2 )
  {
    FT_Mem_ذريعو  s1 = *(FT_Mem_ذريعو*)p1;
    FT_Mem_ذريعو  s2 = *(FT_Mem_ذريعو*)p2;


    إذا ( s2->max_سائيز > s1->max_سائيز )
      أرجع 1;
    وإلا إذا ( s2->max_سائيز < s1->max_سائيز )
      أرجع -1;
    وإلا
      أرجع 0;
  }


  خارجي فراغ
  FT_Dump_يادگيري( FT_يادگيري  يادگيري )
  {
    إذا ( يادگيري->حرر == ft_mem_debug_free )
    {
      FT_Mem_جدول    جدول = (FT_Mem_جدول)يادگيري->user;
      FT_Mem_ذريعو*  ٻالٽي = جدول->sources;
      FT_Mem_ذريعو*  حد_بندي  = ٻالٽي + FT_MEM_ذريعو_BUCKETS;
      FT_Mem_ذريعو*  sources;
      FT_Int         nn, ڳڻپ;
      ثابت حرف*    fmt;


      ڳڻپ = 0;
      لكل ( ; ٻالٽي < حد_بندي; ٻالٽي++ )
      {
        FT_Mem_ذريعو  ذريعو = *ٻالٽي;


        لكل ( ; ذريعو; ذريعو = ذريعو->link )
          ڳڻپ++;
      }

      sources = (FT_Mem_ذريعو*)
                  ft_mem_جدول_alloc(
                    جدول, ڳڻپ * (FT_Long)حجم ( *sources ) );

      ڳڻپ = 0;
      لكل ( ٻالٽي = جدول->sources; ٻالٽي < حد_بندي; ٻالٽي++ )
      {
        FT_Mem_ذريعو  ذريعو = *ٻالٽي;


        لكل ( ; ذريعو; ذريعو = ذريعو->link )
          sources[ڳڻپ++] = ذريعو;
      }

      ft_qsort( sources,
                (size_t)ڳڻپ,
                حجم ( *sources ),
                ft_mem_ذريعو_compare );

      لکو( "FreeType Memory Dump: "
              "current=%ld max=%ld total=%ld count=%ld\n",
              جدول->alloc_current, جدول->alloc_max,
              جدول->alloc_ڪل, جدول->alloc_ڳڻپ );
      لکو( " block  block    sizes    sizes    sizes   source\n" );
      لکو( " count   high      sum  highsum      max   location\n" );
      لکو( "-------------------------------------------------\n" );

      fmt = "%6ld %6ld %8ld %8ld %8ld %s:%d\n";

      لكل ( nn = 0; nn < ڳڻپ; nn++ )
      {
        FT_Mem_ذريعو  ذريعو = sources[nn];


        لکو( fmt,
                ذريعو->cur_بلاڪون, ذريعو->max_بلاڪون,
                ذريعو->cur_سائيز, ذريعو->max_سائيز, ذريعو->cur_max,
                FT_FILENAME( ذريعو->file_name ),
                ذريعو->سٽ_no );
      }
      لکو( "------------------------------------------------\n" );

      ft_mem_جدول_free( جدول, sources );
    }
  }

#وإلا  /* !FT_DEBUG_MEMORY */

  /* ANSI C doesn't like empty source files */
  تعريفنوع صحيح  debug_mem_dummy_;

#جيڪڏهن_پورو /* !FT_DEBUG_MEMORY */


/*پڇاڙي*/
