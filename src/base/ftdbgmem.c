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


#اگر_تعریف_شده FT_DEBUG_حافظه

#تعریف_کن  KEEPALIVE /* `Keep alive' means that freed blocks aren't released
                    * to the heap.  This is useful to detect double-frees
                    * or weird heap corruption, but it uses large amounts of
                    * memory, however.
                    */

#ضمن FT_CONFIG_STANDARD_LIBRARY_H

  FT_پایه_DEF( ثابت حرف* )  ft_debug_file_   = عدم;
  FT_پایه_DEF( دراز )         ft_debug_lineno_ = 0;

  خارجي فراغ
  FT_Dump_حافظه( FT_حافظه  حافظه );


  تعريفنوع بنية FT_Mem_منبع_Rec*  FT_Mem_منبع;
  تعريفنوع بنية FT_Mem_گره_Rec*    FT_Mem_گره;
  تعريفنوع بنية FT_Mem_جدول_Rec*   FT_Mem_جدول;


#تعریف_کن FT_MEM_VAL( addr )  ( (FT_PtrDist)(FT_اشاره_گر)( addr ) )

  /*
   * This structure holds statistics for a single allocation/release
   * site.  This is useful to know where memory operations happen the
   * most.
   */
  تعريفنوع بنية  FT_Mem_منبع_Rec
  {
    ثابت حرف*   file_name;
    دراز          سطر_no;

    FT_Long       cur_بلوکها;   /* current number of allocated blocks */
    FT_Long       max_بلوکها;   /* max. number of allocated blocks    */
    FT_Long       all_بلوکها;   /* total number of blocks allocated   */

    FT_Long       cur_اندازه;     /* current cumulative allocated size */
    FT_Long       max_اندازه;     /* maximum cumulative allocated size */
    FT_Long       all_اندازه;     /* total cumulative allocated size   */

    FT_Long       cur_max;      /* current maximum allocated size */

    FT_UInt32     hash;
    FT_Mem_منبع  link;

  } FT_Mem_منبع_Rec_2;


  /*
   * We don't need a resizable array for the memory sources because
   * their number is pretty limited within FreeType.
   */
#تعریف_کن FT_MEM_منبع_BUCKETS  128

  /*
   * This structure holds information related to a single allocated
   * memory block.  If KEEPALIVE is defined, blocks that are freed by
   * FreeType are never released to the system.  Instead, their `size'
   * field is set to `-size'.  This is mainly useful to detect double
   * frees, at the price of a large memory footprint during execution.
   */
  تعريفنوع بنية  FT_Mem_گره_Rec
  {
    FT_Byte*      آدرس;
    FT_Long       اندازه;     /* < 0 if the block was freed */

    FT_Mem_منبع  منبع;

#اگر_تعریف_شده KEEPALIVE
    ثابت حرف*   free_file_name;
    FT_Long       free_سطر_no;
#پایان_اگر

    FT_Mem_گره    link;

  } FT_Mem_گره_Rec_2;


  /*
   * The global structure, containing compound statistics and all hash
   * tables.
   */
  تعريفنوع بنية  FT_Mem_جدول_Rec
  {
    FT_Long          اندازه;
    FT_Long          nodes;
    FT_Mem_گره*      buckets;

    FT_Long          alloc_مجموع;
    FT_Long          alloc_current;
    FT_Long          alloc_max;
    FT_Long          alloc_شمار;

    FT_Bool          bound_مجموع;
    FT_Long          alloc_مجموع_max;

    FT_Bool          bound_شمار;
    FT_Long          alloc_شمار_max;

    FT_Mem_منبع     sources[FT_MEM_منبع_BUCKETS];

    FT_Bool          keep_alive;

    FT_حافظه        حافظه;
    FT_اشاره_گر       حافظه_user;
    FT_Alloc_Func    alloc;
    FT_Free_Func     حرر;
    FT_Realloc_Func  realloc;

  } FT_Mem_جدول_Rec_2;


#تعریف_کن FT_MEM_اندازه_MAX  13845163

#تعریف_کن FT_FILENAME( x )  ( (x) ? (x) : "unknown file" )


  /*
   * Prime numbers are ugly to handle.  It would be better to implement
   * L-Hashing, which is 10% faster and doesn't require divisions.
   */
  ساكن ثابت FT_Int  ft_mem_اعداد_اول[] =
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
  ft_mem_closest_اول( FT_Long  رقم )
  {
    size_t  i;


    لكل ( i = 0;
          i < حجم ( ft_mem_اعداد_اول ) / حجم ( ft_mem_اعداد_اول[0] ); i++ )
      إذا ( ft_mem_اعداد_اول[i] > رقم )
        أرجع ft_mem_اعداد_اول[i];

    أرجع FT_MEM_اندازه_MAX;
  }


  ساكن فراغ
  ft_mem_debug_panic( ثابت حرف*  fmt,
                      ... )
  {
    va_list  ap;


    چاپ_کن( "FreeType.Debug: " );

    va_شروع( ap, fmt );
    vprintf( fmt, ap );
    va_انتها( ap );

    چاپ_کن( "\n" );
    خروج( EXIT_FAILURE );
  }


  ساكن FT_اشاره_گر
  ft_mem_جدول_alloc( FT_Mem_جدول  جدول,
                      FT_Long      اندازه )
  {
    FT_حافظه   حافظه = جدول->حافظه;
    FT_اشاره_گر  بلوک;


    حافظه->user = جدول->حافظه_user;
    بلوک = جدول->alloc( حافظه, اندازه );
    حافظه->user = جدول;

    أرجع بلوک;
  }


  ساكن فراغ
  ft_mem_جدول_free( FT_Mem_جدول  جدول,
                     FT_اشاره_گر   بلوک )
  {
    FT_حافظه  حافظه = جدول->حافظه;


    حافظه->user = جدول->حافظه_user;
    جدول->حرر( حافظه, بلوک );
    حافظه->user = جدول;
  }


  ساكن فراغ
  ft_mem_جدول_resize( FT_Mem_جدول  جدول )
  {
    FT_Long  new_اندازه;


    new_اندازه = ft_mem_closest_اول( جدول->nodes );
    إذا ( new_اندازه != جدول->اندازه )
    {
      FT_Mem_گره*  new_buckets;
      FT_Long      i;


      new_buckets = (FT_Mem_گره *)
                      ft_mem_جدول_alloc(
                        جدول,
                        new_اندازه * (FT_Long)حجم ( FT_Mem_گره ) );
      إذا ( !new_buckets )
        أرجع;

      FT_ARRAY_ZERO( new_buckets, new_اندازه );

      لكل ( i = 0; i < جدول->اندازه; i++ )
      {
        FT_Mem_گره  گره, بعدی, *pnode;
        FT_PtrDist  hash;


        گره = جدول->buckets[i];
        طالما ( گره )
        {
          بعدی  = گره->link;
          hash  = FT_MEM_VAL( گره->آدرس ) % (FT_PtrDist)new_اندازه;
          pnode = new_buckets + hash;

          گره->link = pnode[0];
          pnode[0]   = گره;

          گره = بعدی;
        }
      }

      إذا ( جدول->buckets )
        ft_mem_جدول_free( جدول, جدول->buckets );

      جدول->buckets = new_buckets;
      جدول->اندازه    = new_اندازه;
    }
  }


  ساكن فراغ
  ft_mem_جدول_destroy( FT_Mem_جدول  جدول )
  {
    FT_Long  i;
    FT_Long  leak_شمار = 0;
    FT_Long  leaks      = 0;


    /* remove all blocks from the table, revealing leaked ones */
    لكل ( i = 0; i < جدول->اندازه; i++ )
    {
      FT_Mem_گره  *pnode = جدول->buckets + i, بعدی, گره = *pnode;


      طالما ( گره )
      {
        بعدی       = گره->link;
        گره->link = عدم;

        إذا ( گره->اندازه > 0 )
        {
          چاپ_کن(
            "leaked memory block at address %p, size %8ld in (%s:%ld)\n",
            (فراغ*)گره->آدرس,
            گره->اندازه,
            FT_FILENAME( گره->منبع->file_name ),
            گره->منبع->سطر_no );

          leak_شمار++;
          leaks += گره->اندازه;

          ft_mem_جدول_free( جدول, گره->آدرس );
        }

        گره->آدرس = عدم;
        گره->اندازه    = 0;

        ft_mem_جدول_free( جدول, گره );
        گره = بعدی;
      }
      جدول->buckets[i] = عدم;
    }

    ft_mem_جدول_free( جدول, جدول->buckets );
    جدول->buckets = عدم;

    جدول->اندازه  = 0;
    جدول->nodes = 0;

    /* remove all sources */
    لكل ( i = 0; i < FT_MEM_منبع_BUCKETS; i++ )
    {
      FT_Mem_منبع  منبع, بعدی;


      لكل ( منبع = جدول->sources[i]; منبع != عدم; منبع = بعدی )
      {
        بعدی = منبع->link;
        ft_mem_جدول_free( جدول, منبع );
      }

      جدول->sources[i] = عدم;
    }

    چاپ_کن( "FreeType: total memory allocations = %ld\n",
            جدول->alloc_مجموع );
    چاپ_کن( "FreeType: maximum memory footprint = %ld\n",
            جدول->alloc_max );

    إذا ( leak_شمار > 0 )
      ft_mem_debug_panic(
        "FreeType: %ld bytes of memory leaked in %ld blocks\n",
        leaks, leak_شمار );

    چاپ_کن( "FreeType: no memory leaks detected\n" );
  }


  ساكن FT_Mem_گره*
  ft_mem_جدول_get_nodep( FT_Mem_جدول  جدول,
                          FT_Byte*     آدرس )
  {
    FT_PtrDist   hash;
    FT_Mem_گره  *pnode, گره;


    hash  = FT_MEM_VAL( آدرس );
    pnode = جدول->buckets + ( hash % (FT_PtrDist)جدول->اندازه );

    لكل (;;)
    {
      گره = pnode[0];
      إذا ( !گره )
        اكسر;

      إذا ( گره->آدرس == آدرس )
        اكسر;

      pnode = &گره->link;
    }
    أرجع pnode;
  }


  ساكن FT_Mem_منبع
  ft_mem_جدول_get_منبع( FT_Mem_جدول  جدول )
  {
    FT_UInt32     hash;
    FT_Mem_منبع  گره, *pnode;


    /* cast to FT_PtrDist first since void* can be larger */
    /* than FT_UInt32 and GCC 4.1.1 emits a warning       */
    hash  = (FT_UInt32)(FT_PtrDist)(فراغ*)ft_debug_file_ +
              (FT_UInt32)( 5 * ft_debug_lineno_ );
    pnode = &جدول->sources[hash % FT_MEM_منبع_BUCKETS];

    لكل (;;)
    {
      گره = *pnode;
      إذا ( !گره )
        اكسر;

      إذا ( گره->file_name == ft_debug_file_   &&
           گره->سطر_no   == ft_debug_lineno_ )
        اذهب Exit;

      pnode = &گره->link;
    }

    گره = (FT_Mem_منبع)ft_mem_جدول_alloc( جدول, حجم ( *گره ) );
    إذا ( !گره )
      ft_mem_debug_panic(
        "not enough memory to perform memory debugging\n" );

    گره->file_name = ft_debug_file_;
    گره->سطر_no   = ft_debug_lineno_;

    گره->cur_بلوکها = 0;
    گره->max_بلوکها = 0;
    گره->all_بلوکها = 0;

    گره->cur_اندازه = 0;
    گره->max_اندازه = 0;
    گره->all_اندازه = 0;

    گره->cur_max = 0;

    گره->link = عدم;
    گره->hash = hash;
    *pnode     = گره;

  Exit:
    أرجع گره;
  }


  ساكن فراغ
  ft_mem_جدول_مجموعه( FT_Mem_جدول  جدول,
                    FT_Byte*     آدرس,
                    FT_Long      اندازه,
                    FT_Long      delta )
  {
    FT_Mem_گره  *pnode, گره;


    إذا ( جدول )
    {
      FT_Mem_منبع  منبع;


      pnode = ft_mem_جدول_get_nodep( جدول, آدرس );
      گره  = *pnode;
      إذا ( گره )
      {
        إذا ( گره->اندازه < 0 )
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
            گره->آدرس, گره->اندازه,
            FT_FILENAME( گره->منبع->file_name ), گره->منبع->سطر_no,
            FT_FILENAME( ft_debug_file_ ), ft_debug_lineno_ );
        }
      }

      /* we need to create a new node in this table */
      گره = (FT_Mem_گره)ft_mem_جدول_alloc( جدول, حجم ( *گره ) );
      إذا ( !گره )
        ft_mem_debug_panic( "not enough memory to run memory tests" );

      گره->آدرس = آدرس;
      گره->اندازه    = اندازه;
      گره->منبع  = منبع = ft_mem_جدول_get_منبع( جدول );

      إذا ( delta == 0 )
      {
        /* this is an allocation */
        منبع->all_بلوکها++;
        منبع->cur_بلوکها++;
        إذا ( منبع->cur_بلوکها > منبع->max_بلوکها )
          منبع->max_بلوکها = منبع->cur_بلوکها;
      }

      إذا ( اندازه > منبع->cur_max )
        منبع->cur_max = اندازه;

      إذا ( delta != 0 )
      {
        /* we are growing or shrinking a reallocated block */
        منبع->cur_اندازه     += delta;
        جدول->alloc_current += delta;
      }
      وإلا
      {
        /* we are allocating a new block */
        منبع->cur_اندازه     += اندازه;
        جدول->alloc_current += اندازه;
      }

      منبع->all_اندازه += اندازه;

      إذا ( منبع->cur_اندازه > منبع->max_اندازه )
        منبع->max_اندازه = منبع->cur_اندازه;

      گره->free_file_name = عدم;
      گره->free_سطر_no   = 0;

      گره->link = pnode[0];

      pnode[0] = گره;
      جدول->nodes++;

      جدول->alloc_مجموع += اندازه;

      إذا ( جدول->alloc_current > جدول->alloc_max )
        جدول->alloc_max = جدول->alloc_current;

      إذا ( جدول->nodes * 3 < جدول->اندازه  ||
           جدول->اندازه  * 3 < جدول->nodes )
        ft_mem_جدول_resize( جدول );
    }
  }


  ساكن فراغ
  ft_mem_جدول_remove( FT_Mem_جدول  جدول,
                       FT_Byte*     آدرس,
                       FT_Long      delta )
  {
    إذا ( جدول )
    {
      FT_Mem_گره  *pnode, گره;


      pnode = ft_mem_جدول_get_nodep( جدول, آدرس );
      گره  = *pnode;
      إذا ( گره )
      {
        FT_Mem_منبع  منبع;


        إذا ( گره->اندازه < 0 )
          ft_mem_debug_panic(
            "freeing memory block at %p more than once\n"
            "  at (%s:%ld)!\n"
            "  Block was allocated at (%s:%ld)\n"
            "  and released at (%s:%ld).",
            آدرس,
            FT_FILENAME( ft_debug_file_ ), ft_debug_lineno_,
            FT_FILENAME( گره->منبع->file_name ), گره->منبع->سطر_no,
            FT_FILENAME( گره->free_file_name ), گره->free_سطر_no );

        /* scramble the node's content for additional safety */
        FT_MEM_مجموعه( آدرس, 0xF3, گره->اندازه );

        إذا ( delta == 0 )
        {
          منبع = گره->منبع;

          منبع->cur_بلوکها--;
          منبع->cur_اندازه -= گره->اندازه;

          جدول->alloc_current -= گره->اندازه;
        }

        إذا ( جدول->keep_alive )
        {
          /* we simply invert the node's size to indicate that the node */
          /* was freed.                                                 */
          گره->اندازه           = -گره->اندازه;
          گره->free_file_name = ft_debug_file_;
          گره->free_سطر_no   = ft_debug_lineno_;
        }
        وإلا
        {
          جدول->nodes--;

          *pnode = گره->link;

          گره->اندازه   = 0;
          گره->منبع = عدم;

          ft_mem_جدول_free( جدول, گره );

          إذا ( جدول->nodes * 3 < جدول->اندازه  ||
               جدول->اندازه  * 3 < جدول->nodes )
            ft_mem_جدول_resize( جدول );
        }
      }
      وإلا
        ft_mem_debug_panic(
          "trying to free unknown block at %p in (%s:%ld)\n",
          آدرس,
          FT_FILENAME( ft_debug_file_ ), ft_debug_lineno_ );
    }
  }


  ساكن FT_اشاره_گر
  ft_mem_debug_alloc( FT_حافظه  حافظه,
                      FT_Long    اندازه )
  {
    FT_Mem_جدول  جدول = (FT_Mem_جدول)حافظه->user;
    FT_Byte*     بلوک;


    إذا ( اندازه <= 0 )
      ft_mem_debug_panic( "negative block size allocation (%ld)", اندازه );

    /* return NULL if the maximum number of allocations was reached */
    إذا ( جدول->bound_شمار                           &&
         جدول->alloc_شمار >= جدول->alloc_شمار_max )
      أرجع عدم;

    /* return NULL if this allocation would overflow the maximum heap size */
    إذا ( جدول->bound_مجموع                                   &&
         جدول->alloc_مجموع_max - جدول->alloc_current > اندازه )
      أرجع عدم;

    بلوک = (FT_Byte *)ft_mem_جدول_alloc( جدول, اندازه );
    إذا ( بلوک )
    {
      ft_mem_جدول_مجموعه( جدول, بلوک, اندازه, 0 );

      جدول->alloc_شمار++;
    }

    ft_debug_file_   = "<unknown>";
    ft_debug_lineno_ = 0;

    أرجع (FT_اشاره_گر)بلوک;
  }


  ساكن فراغ
  ft_mem_debug_free( FT_حافظه   حافظه,
                     FT_اشاره_گر  بلوک )
  {
    FT_Mem_جدول  جدول = (FT_Mem_جدول)حافظه->user;


    إذا ( !بلوک )
      ft_mem_debug_panic( "trying to free NULL in (%s:%ld)",
                          FT_FILENAME( ft_debug_file_ ),
                          ft_debug_lineno_ );

    ft_mem_جدول_remove( جدول, (FT_Byte*)بلوک, 0 );

    إذا ( !جدول->keep_alive )
      ft_mem_جدول_free( جدول, بلوک );

    جدول->alloc_شمار--;

    ft_debug_file_   = "<unknown>";
    ft_debug_lineno_ = 0;
  }


  ساكن FT_اشاره_گر
  ft_mem_debug_realloc( FT_حافظه   حافظه,
                        FT_Long     cur_اندازه,
                        FT_Long     new_اندازه,
                        FT_اشاره_گر  بلوک )
  {
    FT_Mem_جدول  جدول = (FT_Mem_جدول)حافظه->user;
    FT_Mem_گره   گره, *pnode;
    FT_اشاره_گر   new_بلوک;
    FT_Long      delta;

    ثابت حرف*  file_name = FT_FILENAME( ft_debug_file_ );
    FT_Long      سطر_no   = ft_debug_lineno_;


    /* unlikely, but possible */
    إذا ( new_اندازه == cur_اندازه )
      أرجع بلوک;

    /* the following is valid according to ANSI C */
#إذا 0
    إذا ( !بلوک || !cur_اندازه )
      ft_mem_debug_panic( "trying to reallocate NULL in (%s:%ld)",
                          file_name, سطر_no );
#پایان_اگر

    /* while the following is allowed in ANSI C also, we abort since */
    /* such case should be handled by FreeType.                      */
    إذا ( new_اندازه <= 0 )
      ft_mem_debug_panic(
        "trying to reallocate %p to size 0 (current is %ld) in (%s:%ld)",
        بلوک, cur_اندازه, file_name, سطر_no );

    /* check `cur_size' value */
    pnode = ft_mem_جدول_get_nodep( جدول, (FT_Byte*)بلوک );
    گره  = *pnode;
    إذا ( !گره )
      ft_mem_debug_panic(
        "trying to reallocate unknown block at %p in (%s:%ld)",
        بلوک, file_name, سطر_no );

    إذا ( گره->اندازه <= 0 )
      ft_mem_debug_panic(
        "trying to reallocate freed block at %p in (%s:%ld)",
        بلوک, file_name, سطر_no );

    إذا ( گره->اندازه != cur_اندازه )
      ft_mem_debug_panic( "invalid ft_realloc request for %p. cur_size is "
                          "%ld instead of %ld in (%s:%ld)",
                          بلوک, cur_اندازه, گره->اندازه, file_name, سطر_no );

    /* return NULL if the maximum number of allocations was reached */
    إذا ( جدول->bound_شمار                           &&
         جدول->alloc_شمار >= جدول->alloc_شمار_max )
      أرجع عدم;

    delta = new_اندازه - cur_اندازه;

    /* return NULL if this allocation would overflow the maximum heap size */
    إذا ( delta > 0                                             &&
         جدول->bound_مجموع                                    &&
         جدول->alloc_current + delta > جدول->alloc_مجموع_max )
      أرجع عدم;

    new_بلوک = (FT_اشاره_گر)ft_mem_جدول_alloc( جدول, new_اندازه );
    إذا ( !new_بلوک )
      أرجع عدم;

    ft_mem_جدول_مجموعه( جدول, (FT_Byte*)new_بلوک, new_اندازه, delta );

    ft_memcpy( new_بلوک, بلوک, cur_اندازه < new_اندازه ? (size_t)cur_اندازه
                                                     : (size_t)new_اندازه );

    ft_mem_جدول_remove( جدول, (FT_Byte*)بلوک, delta );

    ft_debug_file_   = "<unknown>";
    ft_debug_lineno_ = 0;

    إذا ( !جدول->keep_alive )
      ft_mem_جدول_free( جدول, بلوک );

    أرجع new_بلوک;
  }


  خارجي فراغ
  ft_mem_debug_init( FT_حافظه  حافظه )
  {
    FT_Mem_جدول  جدول;


    إذا ( !ft_getenv( "FT2_DEBUG_MEMORY" ) )
      أرجع;

    جدول = (FT_Mem_جدول)حافظه->alloc( حافظه, حجم ( *جدول ) );

    إذا ( جدول )
    {
      FT_ZERO( جدول );

      جدول->حافظه      = حافظه;
      جدول->حافظه_user = حافظه->user;
      جدول->alloc       = حافظه->alloc;
      جدول->realloc     = حافظه->realloc;
      جدول->حرر        = حافظه->حرر;

      ft_mem_جدول_resize( جدول );

      إذا ( جدول->اندازه )
      {
        ثابت حرف*  p;


        حافظه->user    = جدول;
        حافظه->alloc   = ft_mem_debug_alloc;
        حافظه->realloc = ft_mem_debug_realloc;
        حافظه->حرر    = ft_mem_debug_free;

        p = ft_getenv( "FT2_ALLOC_TOTAL_MAX" );
        إذا ( p )
        {
          FT_Long  مجموع_max = ft_strtol( p, عدم, 10 );


          إذا ( مجموع_max > 0 )
          {
            جدول->bound_مجموع     = 1;
            جدول->alloc_مجموع_max = مجموع_max;
          }
        }

        p = ft_getenv( "FT2_ALLOC_COUNT_MAX" );
        إذا ( p )
        {
          FT_Long  مجموع_شمار = ft_strtol( p, عدم, 10 );


          إذا ( مجموع_شمار > 0 )
          {
            جدول->bound_شمار     = 1;
            جدول->alloc_شمار_max = مجموع_شمار;
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
        حافظه->حرر( حافظه, جدول );
    }
  }


  خارجي فراغ
  ft_mem_debug_انجامشده( FT_حافظه  حافظه )
  {
    إذا ( حافظه->حرر == ft_mem_debug_free )
    {
      FT_Mem_جدول  جدول = (FT_Mem_جدول)حافظه->user;


      FT_Dump_حافظه( حافظه );

      ft_mem_جدول_destroy( جدول );

      حافظه->حرر    = جدول->حرر;
      حافظه->realloc = جدول->realloc;
      حافظه->alloc   = جدول->alloc;
      حافظه->user    = جدول->حافظه_user;

      حافظه->حرر( حافظه, جدول );
    }
  }


  FT_COMPARE_DEF( صحيح )
  ft_mem_منبع_compare( ثابت فراغ*  p1,
                         ثابت فراغ*  p2 )
  {
    FT_Mem_منبع  s1 = *(FT_Mem_منبع*)p1;
    FT_Mem_منبع  s2 = *(FT_Mem_منبع*)p2;


    إذا ( s2->max_اندازه > s1->max_اندازه )
      أرجع 1;
    وإلا إذا ( s2->max_اندازه < s1->max_اندازه )
      أرجع -1;
    وإلا
      أرجع 0;
  }


  خارجي فراغ
  FT_Dump_حافظه( FT_حافظه  حافظه )
  {
    إذا ( حافظه->حرر == ft_mem_debug_free )
    {
      FT_Mem_جدول    جدول = (FT_Mem_جدول)حافظه->user;
      FT_Mem_منبع*  سطل = جدول->sources;
      FT_Mem_منبع*  حد  = سطل + FT_MEM_منبع_BUCKETS;
      FT_Mem_منبع*  sources;
      FT_Int         nn, شمار;
      ثابت حرف*    fmt;


      شمار = 0;
      لكل ( ; سطل < حد; سطل++ )
      {
        FT_Mem_منبع  منبع = *سطل;


        لكل ( ; منبع; منبع = منبع->link )
          شمار++;
      }

      sources = (FT_Mem_منبع*)
                  ft_mem_جدول_alloc(
                    جدول, شمار * (FT_Long)حجم ( *sources ) );

      شمار = 0;
      لكل ( سطل = جدول->sources; سطل < حد; سطل++ )
      {
        FT_Mem_منبع  منبع = *سطل;


        لكل ( ; منبع; منبع = منبع->link )
          sources[شمار++] = منبع;
      }

      ft_qsort( sources,
                (size_t)شمار,
                حجم ( *sources ),
                ft_mem_منبع_compare );

      چاپ_کن( "FreeType Memory Dump: "
              "current=%ld max=%ld total=%ld count=%ld\n",
              جدول->alloc_current, جدول->alloc_max,
              جدول->alloc_مجموع, جدول->alloc_شمار );
      چاپ_کن( " block  block    sizes    sizes    sizes   source\n" );
      چاپ_کن( " count   high      sum  highsum      max   location\n" );
      چاپ_کن( "-------------------------------------------------\n" );

      fmt = "%6ld %6ld %8ld %8ld %8ld %s:%d\n";

      لكل ( nn = 0; nn < شمار; nn++ )
      {
        FT_Mem_منبع  منبع = sources[nn];


        چاپ_کن( fmt,
                منبع->cur_بلوکها, منبع->max_بلوکها,
                منبع->cur_اندازه, منبع->max_اندازه, منبع->cur_max,
                FT_FILENAME( منبع->file_name ),
                منبع->سطر_no );
      }
      چاپ_کن( "------------------------------------------------\n" );

      ft_mem_جدول_free( جدول, sources );
    }
  }

#وإلا  /* !FT_DEBUG_MEMORY */

  /* ANSI C doesn't like empty source files */
  تعريفنوع صحيح  debug_mem_dummy_;

#پایان_اگر /* !FT_DEBUG_MEMORY */


/*انتها*/
