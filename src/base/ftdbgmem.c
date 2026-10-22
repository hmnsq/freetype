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


#جے_تعریف_شدہ FT_DEBUG_حافظہ

#تعریف_کرو  KEEPALIVE /* `Keep alive' means that freed blocks aren't released
                    * to the heap.  This is useful to detect double-frees
                    * or weird heap corruption, but it uses large amounts of
                    * memory, however.
                    */

#ضمن FT_CONFIG_STANDARD_LIBRARY_H

  FT_ادھار_DEF( ثابت حرف* )  ft_debug_file_   = عدم;
  FT_ادھار_DEF( لما )         ft_debug_lineno_ = 0;

  خارجي فراغ
  FT_Dump_حافظہ( FT_حافظہ  حافظہ );


  تعريفنوع بنية FT_Mem_سروت_Rec*  FT_Mem_سروت;
  تعريفنوع بنية FT_Mem_گرہ_Rec*    FT_Mem_گرہ;
  تعريفنوع بنية FT_Mem_جدول_Rec*   FT_Mem_جدول;


#تعریف_کرو FT_MEM_VAL( addr )  ( (FT_PtrDist)(FT_اشارہ_گر)( addr ) )

  /*
   * This structure holds statistics for a single allocation/release
   * site.  This is useful to know where memory operations happen the
   * most.
   */
  تعريفنوع بنية  FT_Mem_سروت_Rec
  {
    ثابت حرف*   file_name;
    لما          سطر_no;

    FT_Long       cur_بلاکاں;   /* current number of allocated blocks */
    FT_Long       max_بلاکاں;   /* max. number of allocated blocks    */
    FT_Long       all_بلاکاں;   /* total number of blocks allocated   */

    FT_Long       cur_ناپ;     /* current cumulative allocated size */
    FT_Long       max_ناپ;     /* maximum cumulative allocated size */
    FT_Long       all_ناپ;     /* total cumulative allocated size   */

    FT_Long       cur_max;      /* current maximum allocated size */

    FT_UInt32     hash;
    FT_Mem_سروت  link;

  } FT_Mem_سروت_Rec_2;


  /*
   * We don't need a resizable array for the memory sources because
   * their number is pretty limited within FreeType.
   */
#تعریف_کرو FT_MEM_سروت_BUCKETS  128

  /*
   * This structure holds information related to a single allocated
   * memory block.  If KEEPALIVE is defined, blocks that are freed by
   * FreeType are never released to the system.  Instead, their `size'
   * field is set to `-size'.  This is mainly useful to detect double
   * frees, at the price of a large memory footprint during execution.
   */
  تعريفنوع بنية  FT_Mem_گرہ_Rec
  {
    FT_Byte*      پتہ;
    FT_Long       ناپ;     /* < 0 if the block was freed */

    FT_Mem_سروت  سروت;

#جے_تعریف_شدہ KEEPALIVE
    ثابت حرف*   free_file_name;
    FT_Long       free_سطر_no;
#جے_انت

    FT_Mem_گرہ    link;

  } FT_Mem_گرہ_Rec_2;


  /*
   * The global structure, containing compound statistics and all hash
   * tables.
   */
  تعريفنوع بنية  FT_Mem_جدول_Rec
  {
    FT_Long          ناپ;
    FT_Long          nodes;
    FT_Mem_گرہ*      buckets;

    FT_Long          alloc_کل;
    FT_Long          alloc_current;
    FT_Long          alloc_max;
    FT_Long          alloc_گنتی;

    FT_Bool          bound_کل;
    FT_Long          alloc_کل_max;

    FT_Bool          bound_گنتی;
    FT_Long          alloc_گنتی_max;

    FT_Mem_سروت     sources[FT_MEM_سروت_BUCKETS];

    FT_Bool          keep_alive;

    FT_حافظہ        حافظہ;
    FT_اشارہ_گر       حافظہ_user;
    FT_Alloc_Func    alloc;
    FT_Free_Func     حرر;
    FT_Realloc_Func  realloc;

  } FT_Mem_جدول_Rec_2;


#تعریف_کرو FT_MEM_ناپ_MAX  13845163

#تعریف_کرو FT_FILENAME( x )  ( (x) ? (x) : "unknown file" )


  /*
   * Prime numbers are ugly to handle.  It would be better to implement
   * L-Hashing, which is 10% faster and doesn't require divisions.
   */
  ساكن ثابت FT_Int  ft_mem_مفرد_عدد[] =
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
  ft_mem_closest_مفرد( FT_Long  عدد )
  {
    size_t  i;


    لكل ( i = 0;
          i < حجم ( ft_mem_مفرد_عدد ) / حجم ( ft_mem_مفرد_عدد[0] ); i++ )
      إذا ( ft_mem_مفرد_عدد[i] > عدد )
        أرجع ft_mem_مفرد_عدد[i];

    أرجع FT_MEM_ناپ_MAX;
  }


  ساكن فراغ
  ft_mem_debug_panic( ثابت حرف*  fmt,
                      ... )
  {
    va_list  ap;


    لکھو( "FreeType.Debug: " );

    va_شروع( ap, fmt );
    vprintf( fmt, ap );
    va_سرا( ap );

    لکھو( "\n" );
    باہر_نکلو( EXIT_FAILURE );
  }


  ساكن FT_اشارہ_گر
  ft_mem_جدول_alloc( FT_Mem_جدول  جدول,
                      FT_Long      ناپ )
  {
    FT_حافظہ   حافظہ = جدول->حافظہ;
    FT_اشارہ_گر  بلاک;


    حافظہ->user = جدول->حافظہ_user;
    بلاک = جدول->alloc( حافظہ, ناپ );
    حافظہ->user = جدول;

    أرجع بلاک;
  }


  ساكن فراغ
  ft_mem_جدول_free( FT_Mem_جدول  جدول,
                     FT_اشارہ_گر   بلاک )
  {
    FT_حافظہ  حافظہ = جدول->حافظہ;


    حافظہ->user = جدول->حافظہ_user;
    جدول->حرر( حافظہ, بلاک );
    حافظہ->user = جدول;
  }


  ساكن فراغ
  ft_mem_جدول_resize( FT_Mem_جدول  جدول )
  {
    FT_Long  new_ناپ;


    new_ناپ = ft_mem_closest_مفرد( جدول->nodes );
    إذا ( new_ناپ != جدول->ناپ )
    {
      FT_Mem_گرہ*  new_buckets;
      FT_Long      i;


      new_buckets = (FT_Mem_گرہ *)
                      ft_mem_جدول_alloc(
                        جدول,
                        new_ناپ * (FT_Long)حجم ( FT_Mem_گرہ ) );
      إذا ( !new_buckets )
        أرجع;

      FT_ARRAY_ZERO( new_buckets, new_ناپ );

      لكل ( i = 0; i < جدول->ناپ; i++ )
      {
        FT_Mem_گرہ  گرہ, اگلا, *pnode;
        FT_PtrDist  hash;


        گرہ = جدول->buckets[i];
        طالما ( گرہ )
        {
          اگلا  = گرہ->link;
          hash  = FT_MEM_VAL( گرہ->پتہ ) % (FT_PtrDist)new_ناپ;
          pnode = new_buckets + hash;

          گرہ->link = pnode[0];
          pnode[0]   = گرہ;

          گرہ = اگلا;
        }
      }

      إذا ( جدول->buckets )
        ft_mem_جدول_free( جدول, جدول->buckets );

      جدول->buckets = new_buckets;
      جدول->ناپ    = new_ناپ;
    }
  }


  ساكن فراغ
  ft_mem_جدول_destroy( FT_Mem_جدول  جدول )
  {
    FT_Long  i;
    FT_Long  leak_گنتی = 0;
    FT_Long  leaks      = 0;


    /* remove all blocks from the table, revealing leaked ones */
    لكل ( i = 0; i < جدول->ناپ; i++ )
    {
      FT_Mem_گرہ  *pnode = جدول->buckets + i, اگلا, گرہ = *pnode;


      طالما ( گرہ )
      {
        اگلا       = گرہ->link;
        گرہ->link = عدم;

        إذا ( گرہ->ناپ > 0 )
        {
          لکھو(
            "leaked memory block at address %p, size %8ld in (%s:%ld)\n",
            (فراغ*)گرہ->پتہ,
            گرہ->ناپ,
            FT_FILENAME( گرہ->سروت->file_name ),
            گرہ->سروت->سطر_no );

          leak_گنتی++;
          leaks += گرہ->ناپ;

          ft_mem_جدول_free( جدول, گرہ->پتہ );
        }

        گرہ->پتہ = عدم;
        گرہ->ناپ    = 0;

        ft_mem_جدول_free( جدول, گرہ );
        گرہ = اگلا;
      }
      جدول->buckets[i] = عدم;
    }

    ft_mem_جدول_free( جدول, جدول->buckets );
    جدول->buckets = عدم;

    جدول->ناپ  = 0;
    جدول->nodes = 0;

    /* remove all sources */
    لكل ( i = 0; i < FT_MEM_سروت_BUCKETS; i++ )
    {
      FT_Mem_سروت  سروت, اگلا;


      لكل ( سروت = جدول->sources[i]; سروت != عدم; سروت = اگلا )
      {
        اگلا = سروت->link;
        ft_mem_جدول_free( جدول, سروت );
      }

      جدول->sources[i] = عدم;
    }

    لکھو( "FreeType: total memory allocations = %ld\n",
            جدول->alloc_کل );
    لکھو( "FreeType: maximum memory footprint = %ld\n",
            جدول->alloc_max );

    إذا ( leak_گنتی > 0 )
      ft_mem_debug_panic(
        "FreeType: %ld bytes of memory leaked in %ld blocks\n",
        leaks, leak_گنتی );

    لکھو( "FreeType: no memory leaks detected\n" );
  }


  ساكن FT_Mem_گرہ*
  ft_mem_جدول_get_nodep( FT_Mem_جدول  جدول,
                          FT_Byte*     پتہ )
  {
    FT_PtrDist   hash;
    FT_Mem_گرہ  *pnode, گرہ;


    hash  = FT_MEM_VAL( پتہ );
    pnode = جدول->buckets + ( hash % (FT_PtrDist)جدول->ناپ );

    لكل (;;)
    {
      گرہ = pnode[0];
      إذا ( !گرہ )
        اكسر;

      إذا ( گرہ->پتہ == پتہ )
        اكسر;

      pnode = &گرہ->link;
    }
    أرجع pnode;
  }


  ساكن FT_Mem_سروت
  ft_mem_جدول_get_سروت( FT_Mem_جدول  جدول )
  {
    FT_UInt32     hash;
    FT_Mem_سروت  گرہ, *pnode;


    /* cast to FT_PtrDist first since void* can be larger */
    /* than FT_UInt32 and GCC 4.1.1 emits a warning       */
    hash  = (FT_UInt32)(FT_PtrDist)(فراغ*)ft_debug_file_ +
              (FT_UInt32)( 5 * ft_debug_lineno_ );
    pnode = &جدول->sources[hash % FT_MEM_سروت_BUCKETS];

    لكل (;;)
    {
      گرہ = *pnode;
      إذا ( !گرہ )
        اكسر;

      إذا ( گرہ->file_name == ft_debug_file_   &&
           گرہ->سطر_no   == ft_debug_lineno_ )
        اذهب Exit;

      pnode = &گرہ->link;
    }

    گرہ = (FT_Mem_سروت)ft_mem_جدول_alloc( جدول, حجم ( *گرہ ) );
    إذا ( !گرہ )
      ft_mem_debug_panic(
        "not enough memory to perform memory debugging\n" );

    گرہ->file_name = ft_debug_file_;
    گرہ->سطر_no   = ft_debug_lineno_;

    گرہ->cur_بلاکاں = 0;
    گرہ->max_بلاکاں = 0;
    گرہ->all_بلاکاں = 0;

    گرہ->cur_ناپ = 0;
    گرہ->max_ناپ = 0;
    گرہ->all_ناپ = 0;

    گرہ->cur_max = 0;

    گرہ->link = عدم;
    گرہ->hash = hash;
    *pnode     = گرہ;

  Exit:
    أرجع گرہ;
  }


  ساكن فراغ
  ft_mem_جدول_سیٹ( FT_Mem_جدول  جدول,
                    FT_Byte*     پتہ,
                    FT_Long      ناپ,
                    FT_Long      delta )
  {
    FT_Mem_گرہ  *pnode, گرہ;


    إذا ( جدول )
    {
      FT_Mem_سروت  سروت;


      pnode = ft_mem_جدول_get_nodep( جدول, پتہ );
      گرہ  = *pnode;
      إذا ( گرہ )
      {
        إذا ( گرہ->ناپ < 0 )
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
            گرہ->پتہ, گرہ->ناپ,
            FT_FILENAME( گرہ->سروت->file_name ), گرہ->سروت->سطر_no,
            FT_FILENAME( ft_debug_file_ ), ft_debug_lineno_ );
        }
      }

      /* we need to create a new node in this table */
      گرہ = (FT_Mem_گرہ)ft_mem_جدول_alloc( جدول, حجم ( *گرہ ) );
      إذا ( !گرہ )
        ft_mem_debug_panic( "not enough memory to run memory tests" );

      گرہ->پتہ = پتہ;
      گرہ->ناپ    = ناپ;
      گرہ->سروت  = سروت = ft_mem_جدول_get_سروت( جدول );

      إذا ( delta == 0 )
      {
        /* this is an allocation */
        سروت->all_بلاکاں++;
        سروت->cur_بلاکاں++;
        إذا ( سروت->cur_بلاکاں > سروت->max_بلاکاں )
          سروت->max_بلاکاں = سروت->cur_بلاکاں;
      }

      إذا ( ناپ > سروت->cur_max )
        سروت->cur_max = ناپ;

      إذا ( delta != 0 )
      {
        /* we are growing or shrinking a reallocated block */
        سروت->cur_ناپ     += delta;
        جدول->alloc_current += delta;
      }
      وإلا
      {
        /* we are allocating a new block */
        سروت->cur_ناپ     += ناپ;
        جدول->alloc_current += ناپ;
      }

      سروت->all_ناپ += ناپ;

      إذا ( سروت->cur_ناپ > سروت->max_ناپ )
        سروت->max_ناپ = سروت->cur_ناپ;

      گرہ->free_file_name = عدم;
      گرہ->free_سطر_no   = 0;

      گرہ->link = pnode[0];

      pnode[0] = گرہ;
      جدول->nodes++;

      جدول->alloc_کل += ناپ;

      إذا ( جدول->alloc_current > جدول->alloc_max )
        جدول->alloc_max = جدول->alloc_current;

      إذا ( جدول->nodes * 3 < جدول->ناپ  ||
           جدول->ناپ  * 3 < جدول->nodes )
        ft_mem_جدول_resize( جدول );
    }
  }


  ساكن فراغ
  ft_mem_جدول_remove( FT_Mem_جدول  جدول,
                       FT_Byte*     پتہ,
                       FT_Long      delta )
  {
    إذا ( جدول )
    {
      FT_Mem_گرہ  *pnode, گرہ;


      pnode = ft_mem_جدول_get_nodep( جدول, پتہ );
      گرہ  = *pnode;
      إذا ( گرہ )
      {
        FT_Mem_سروت  سروت;


        إذا ( گرہ->ناپ < 0 )
          ft_mem_debug_panic(
            "freeing memory block at %p more than once\n"
            "  at (%s:%ld)!\n"
            "  Block was allocated at (%s:%ld)\n"
            "  and released at (%s:%ld).",
            پتہ,
            FT_FILENAME( ft_debug_file_ ), ft_debug_lineno_,
            FT_FILENAME( گرہ->سروت->file_name ), گرہ->سروت->سطر_no,
            FT_FILENAME( گرہ->free_file_name ), گرہ->free_سطر_no );

        /* scramble the node's content for additional safety */
        FT_MEM_سیٹ( پتہ, 0xF3, گرہ->ناپ );

        إذا ( delta == 0 )
        {
          سروت = گرہ->سروت;

          سروت->cur_بلاکاں--;
          سروت->cur_ناپ -= گرہ->ناپ;

          جدول->alloc_current -= گرہ->ناپ;
        }

        إذا ( جدول->keep_alive )
        {
          /* we simply invert the node's size to indicate that the node */
          /* was freed.                                                 */
          گرہ->ناپ           = -گرہ->ناپ;
          گرہ->free_file_name = ft_debug_file_;
          گرہ->free_سطر_no   = ft_debug_lineno_;
        }
        وإلا
        {
          جدول->nodes--;

          *pnode = گرہ->link;

          گرہ->ناپ   = 0;
          گرہ->سروت = عدم;

          ft_mem_جدول_free( جدول, گرہ );

          إذا ( جدول->nodes * 3 < جدول->ناپ  ||
               جدول->ناپ  * 3 < جدول->nodes )
            ft_mem_جدول_resize( جدول );
        }
      }
      وإلا
        ft_mem_debug_panic(
          "trying to free unknown block at %p in (%s:%ld)\n",
          پتہ,
          FT_FILENAME( ft_debug_file_ ), ft_debug_lineno_ );
    }
  }


  ساكن FT_اشارہ_گر
  ft_mem_debug_alloc( FT_حافظہ  حافظہ,
                      FT_Long    ناپ )
  {
    FT_Mem_جدول  جدول = (FT_Mem_جدول)حافظہ->user;
    FT_Byte*     بلاک;


    إذا ( ناپ <= 0 )
      ft_mem_debug_panic( "negative block size allocation (%ld)", ناپ );

    /* return NULL if the maximum number of allocations was reached */
    إذا ( جدول->bound_گنتی                           &&
         جدول->alloc_گنتی >= جدول->alloc_گنتی_max )
      أرجع عدم;

    /* return NULL if this allocation would overflow the maximum heap size */
    إذا ( جدول->bound_کل                                   &&
         جدول->alloc_کل_max - جدول->alloc_current > ناپ )
      أرجع عدم;

    بلاک = (FT_Byte *)ft_mem_جدول_alloc( جدول, ناپ );
    إذا ( بلاک )
    {
      ft_mem_جدول_سیٹ( جدول, بلاک, ناپ, 0 );

      جدول->alloc_گنتی++;
    }

    ft_debug_file_   = "<unknown>";
    ft_debug_lineno_ = 0;

    أرجع (FT_اشارہ_گر)بلاک;
  }


  ساكن فراغ
  ft_mem_debug_free( FT_حافظہ   حافظہ,
                     FT_اشارہ_گر  بلاک )
  {
    FT_Mem_جدول  جدول = (FT_Mem_جدول)حافظہ->user;


    إذا ( !بلاک )
      ft_mem_debug_panic( "trying to free NULL in (%s:%ld)",
                          FT_FILENAME( ft_debug_file_ ),
                          ft_debug_lineno_ );

    ft_mem_جدول_remove( جدول, (FT_Byte*)بلاک, 0 );

    إذا ( !جدول->keep_alive )
      ft_mem_جدول_free( جدول, بلاک );

    جدول->alloc_گنتی--;

    ft_debug_file_   = "<unknown>";
    ft_debug_lineno_ = 0;
  }


  ساكن FT_اشارہ_گر
  ft_mem_debug_realloc( FT_حافظہ   حافظہ,
                        FT_Long     cur_ناپ,
                        FT_Long     new_ناپ,
                        FT_اشارہ_گر  بلاک )
  {
    FT_Mem_جدول  جدول = (FT_Mem_جدول)حافظہ->user;
    FT_Mem_گرہ   گرہ, *pnode;
    FT_اشارہ_گر   new_بلاک;
    FT_Long      delta;

    ثابت حرف*  file_name = FT_FILENAME( ft_debug_file_ );
    FT_Long      سطر_no   = ft_debug_lineno_;


    /* unlikely, but possible */
    إذا ( new_ناپ == cur_ناپ )
      أرجع بلاک;

    /* the following is valid according to ANSI C */
#إذا 0
    إذا ( !بلاک || !cur_ناپ )
      ft_mem_debug_panic( "trying to reallocate NULL in (%s:%ld)",
                          file_name, سطر_no );
#جے_انت

    /* while the following is allowed in ANSI C also, we abort since */
    /* such case should be handled by FreeType.                      */
    إذا ( new_ناپ <= 0 )
      ft_mem_debug_panic(
        "trying to reallocate %p to size 0 (current is %ld) in (%s:%ld)",
        بلاک, cur_ناپ, file_name, سطر_no );

    /* check `cur_size' value */
    pnode = ft_mem_جدول_get_nodep( جدول, (FT_Byte*)بلاک );
    گرہ  = *pnode;
    إذا ( !گرہ )
      ft_mem_debug_panic(
        "trying to reallocate unknown block at %p in (%s:%ld)",
        بلاک, file_name, سطر_no );

    إذا ( گرہ->ناپ <= 0 )
      ft_mem_debug_panic(
        "trying to reallocate freed block at %p in (%s:%ld)",
        بلاک, file_name, سطر_no );

    إذا ( گرہ->ناپ != cur_ناپ )
      ft_mem_debug_panic( "invalid ft_realloc request for %p. cur_size is "
                          "%ld instead of %ld in (%s:%ld)",
                          بلاک, cur_ناپ, گرہ->ناپ, file_name, سطر_no );

    /* return NULL if the maximum number of allocations was reached */
    إذا ( جدول->bound_گنتی                           &&
         جدول->alloc_گنتی >= جدول->alloc_گنتی_max )
      أرجع عدم;

    delta = new_ناپ - cur_ناپ;

    /* return NULL if this allocation would overflow the maximum heap size */
    إذا ( delta > 0                                             &&
         جدول->bound_کل                                    &&
         جدول->alloc_current + delta > جدول->alloc_کل_max )
      أرجع عدم;

    new_بلاک = (FT_اشارہ_گر)ft_mem_جدول_alloc( جدول, new_ناپ );
    إذا ( !new_بلاک )
      أرجع عدم;

    ft_mem_جدول_سیٹ( جدول, (FT_Byte*)new_بلاک, new_ناپ, delta );

    ft_memcpy( new_بلاک, بلاک, cur_ناپ < new_ناپ ? (size_t)cur_ناپ
                                                     : (size_t)new_ناپ );

    ft_mem_جدول_remove( جدول, (FT_Byte*)بلاک, delta );

    ft_debug_file_   = "<unknown>";
    ft_debug_lineno_ = 0;

    إذا ( !جدول->keep_alive )
      ft_mem_جدول_free( جدول, بلاک );

    أرجع new_بلاک;
  }


  خارجي فراغ
  ft_mem_debug_init( FT_حافظہ  حافظہ )
  {
    FT_Mem_جدول  جدول;


    إذا ( !ft_getenv( "FT2_DEBUG_MEMORY" ) )
      أرجع;

    جدول = (FT_Mem_جدول)حافظہ->alloc( حافظہ, حجم ( *جدول ) );

    إذا ( جدول )
    {
      FT_ZERO( جدول );

      جدول->حافظہ      = حافظہ;
      جدول->حافظہ_user = حافظہ->user;
      جدول->alloc       = حافظہ->alloc;
      جدول->realloc     = حافظہ->realloc;
      جدول->حرر        = حافظہ->حرر;

      ft_mem_جدول_resize( جدول );

      إذا ( جدول->ناپ )
      {
        ثابت حرف*  p;


        حافظہ->user    = جدول;
        حافظہ->alloc   = ft_mem_debug_alloc;
        حافظہ->realloc = ft_mem_debug_realloc;
        حافظہ->حرر    = ft_mem_debug_free;

        p = ft_getenv( "FT2_ALLOC_TOTAL_MAX" );
        إذا ( p )
        {
          FT_Long  کل_max = ft_strtol( p, عدم, 10 );


          إذا ( کل_max > 0 )
          {
            جدول->bound_کل     = 1;
            جدول->alloc_کل_max = کل_max;
          }
        }

        p = ft_getenv( "FT2_ALLOC_COUNT_MAX" );
        إذا ( p )
        {
          FT_Long  کل_گنتی = ft_strtol( p, عدم, 10 );


          إذا ( کل_گنتی > 0 )
          {
            جدول->bound_گنتی     = 1;
            جدول->alloc_گنتی_max = کل_گنتی;
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
        حافظہ->حرر( حافظہ, جدول );
    }
  }


  خارجي فراغ
  ft_mem_debug_پورا( FT_حافظہ  حافظہ )
  {
    إذا ( حافظہ->حرر == ft_mem_debug_free )
    {
      FT_Mem_جدول  جدول = (FT_Mem_جدول)حافظہ->user;


      FT_Dump_حافظہ( حافظہ );

      ft_mem_جدول_destroy( جدول );

      حافظہ->حرر    = جدول->حرر;
      حافظہ->realloc = جدول->realloc;
      حافظہ->alloc   = جدول->alloc;
      حافظہ->user    = جدول->حافظہ_user;

      حافظہ->حرر( حافظہ, جدول );
    }
  }


  FT_COMPARE_DEF( صحيح )
  ft_mem_سروت_compare( ثابت فراغ*  p1,
                         ثابت فراغ*  p2 )
  {
    FT_Mem_سروت  s1 = *(FT_Mem_سروت*)p1;
    FT_Mem_سروت  s2 = *(FT_Mem_سروت*)p2;


    إذا ( s2->max_ناپ > s1->max_ناپ )
      أرجع 1;
    وإلا إذا ( s2->max_ناپ < s1->max_ناپ )
      أرجع -1;
    وإلا
      أرجع 0;
  }


  خارجي فراغ
  FT_Dump_حافظہ( FT_حافظہ  حافظہ )
  {
    إذا ( حافظہ->حرر == ft_mem_debug_free )
    {
      FT_Mem_جدول    جدول = (FT_Mem_جدول)حافظہ->user;
      FT_Mem_سروت*  بالٹی = جدول->sources;
      FT_Mem_سروت*  حد_بندی  = بالٹی + FT_MEM_سروت_BUCKETS;
      FT_Mem_سروت*  sources;
      FT_Int         nn, گنتی;
      ثابت حرف*    fmt;


      گنتی = 0;
      لكل ( ; بالٹی < حد_بندی; بالٹی++ )
      {
        FT_Mem_سروت  سروت = *بالٹی;


        لكل ( ; سروت; سروت = سروت->link )
          گنتی++;
      }

      sources = (FT_Mem_سروت*)
                  ft_mem_جدول_alloc(
                    جدول, گنتی * (FT_Long)حجم ( *sources ) );

      گنتی = 0;
      لكل ( بالٹی = جدول->sources; بالٹی < حد_بندی; بالٹی++ )
      {
        FT_Mem_سروت  سروت = *بالٹی;


        لكل ( ; سروت; سروت = سروت->link )
          sources[گنتی++] = سروت;
      }

      ft_qsort( sources,
                (size_t)گنتی,
                حجم ( *sources ),
                ft_mem_سروت_compare );

      لکھو( "FreeType Memory Dump: "
              "current=%ld max=%ld total=%ld count=%ld\n",
              جدول->alloc_current, جدول->alloc_max,
              جدول->alloc_کل, جدول->alloc_گنتی );
      لکھو( " block  block    sizes    sizes    sizes   source\n" );
      لکھو( " count   high      sum  highsum      max   location\n" );
      لکھو( "-------------------------------------------------\n" );

      fmt = "%6ld %6ld %8ld %8ld %8ld %s:%d\n";

      لكل ( nn = 0; nn < گنتی; nn++ )
      {
        FT_Mem_سروت  سروت = sources[nn];


        لکھو( fmt,
                سروت->cur_بلاکاں, سروت->max_بلاکاں,
                سروت->cur_ناپ, سروت->max_ناپ, سروت->cur_max,
                FT_FILENAME( سروت->file_name ),
                سروت->سطر_no );
      }
      لکھو( "------------------------------------------------\n" );

      ft_mem_جدول_free( جدول, sources );
    }
  }

#وإلا  /* !FT_DEBUG_MEMORY */

  /* ANSI C doesn't like empty source files */
  تعريفنوع صحيح  debug_mem_dummy_;

#جے_انت /* !FT_DEBUG_MEMORY */


/*سرا*/
