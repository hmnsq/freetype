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


#که_تعریف_شوی FT_DEBUG_حافظه

#تعریف_کړه  KEEPALIVE /* `Keep alive' means that freed blocks aren't released
                    * to the heap.  This is useful to detect double-frees
                    * or weird heap corruption, but it uses large amounts of
                    * memory, however.
                    */

#ضمن FT_CONFIG_STANDARD_LIBRARY_H

  FT_بنسټ_DEF( ثابت حرف* )  ft_debug_file_   = عدم;
  FT_بنسټ_DEF( اوږد )         ft_debug_lineno_ = 0;

  خارجي فراغ
  FT_Dump_حافظه( FT_حافظه  حافظه );


  تعريفنوع بنية FT_Mem_سرچینه_Rec*  FT_Mem_سرچینه;
  تعريفنوع بنية FT_Mem_غوټه_Rec*    FT_Mem_غوټه;
  تعريفنوع بنية FT_Mem_جدول_Rec*   FT_Mem_جدول;


#تعریف_کړه FT_MEM_VAL( addr )  ( (FT_PtrDist)(FT_اشاره_ګر)( addr ) )

  /*
   * This structure holds statistics for a single allocation/release
   * site.  This is useful to know where memory operations happen the
   * most.
   */
  تعريفنوع بنية  FT_Mem_سرچینه_Rec
  {
    ثابت حرف*   file_name;
    اوږد          کرښه_no;

    FT_Long       cur_بلاکونه;   /* current number of allocated blocks */
    FT_Long       max_بلاکونه;   /* max. number of allocated blocks    */
    FT_Long       all_بلاکونه;   /* total number of blocks allocated   */

    FT_Long       cur_اندازه;     /* current cumulative allocated size */
    FT_Long       max_اندازه;     /* maximum cumulative allocated size */
    FT_Long       all_اندازه;     /* total cumulative allocated size   */

    FT_Long       cur_max;      /* current maximum allocated size */

    FT_UInt32     hash;
    FT_Mem_سرچینه  link;

  } FT_Mem_سرچینه_Rec_2;


  /*
   * We don't need a resizable array for the memory sources because
   * their number is pretty limited within FreeType.
   */
#تعریف_کړه FT_MEM_سرچینه_BUCKETS  128

  /*
   * This structure holds information related to a single allocated
   * memory block.  If KEEPALIVE is defined, blocks that are freed by
   * FreeType are never released to the system.  Instead, their `size'
   * field is set to `-size'.  This is mainly useful to detect double
   * frees, at the price of a large memory footprint during execution.
   */
  تعريفنوع بنية  FT_Mem_غوټه_Rec
  {
    FT_Byte*      پته;
    FT_Long       اندازه;     /* < 0 if the block was freed */

    FT_Mem_سرچینه  سرچینه;

#که_تعریف_شوی KEEPALIVE
    ثابت حرف*   free_file_name;
    FT_Long       free_کرښه_no;
#د_که_پای

    FT_Mem_غوټه    link;

  } FT_Mem_غوټه_Rec_2;


  /*
   * The global structure, containing compound statistics and all hash
   * tables.
   */
  تعريفنوع بنية  FT_Mem_جدول_Rec
  {
    FT_Long          اندازه;
    FT_Long          nodes;
    FT_Mem_غوټه*      buckets;

    FT_Long          alloc_ټول;
    FT_Long          alloc_current;
    FT_Long          alloc_max;
    FT_Long          alloc_شمېر;

    FT_Bool          bound_ټول;
    FT_Long          alloc_ټول_max;

    FT_Bool          bound_شمېر;
    FT_Long          alloc_شمېر_max;

    FT_Mem_سرچینه     sources[FT_MEM_سرچینه_BUCKETS];

    FT_Bool          keep_alive;

    FT_حافظه        حافظه;
    FT_اشاره_ګر       حافظه_user;
    FT_Alloc_Func    alloc;
    FT_Free_Func     حرر;
    FT_Realloc_Func  realloc;

  } FT_Mem_جدول_Rec_2;


#تعریف_کړه FT_MEM_اندازه_MAX  13845163

#تعریف_کړه FT_FILENAME( x )  ( (x) ? (x) : "unknown file" )


  /*
   * Prime numbers are ugly to handle.  It would be better to implement
   * L-Hashing, which is 10% faster and doesn't require divisions.
   */
  ساكن ثابت FT_Int  ft_mem_لومړني_عددونه[] =
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
  ft_mem_closest_اولي( FT_Long  عدد )
  {
    size_t  i;


    لكل ( i = 0;
          i < حجم ( ft_mem_لومړني_عددونه ) / حجم ( ft_mem_لومړني_عددونه[0] ); i++ )
      إذا ( ft_mem_لومړني_عددونه[i] > عدد )
        أرجع ft_mem_لومړني_عددونه[i];

    أرجع FT_MEM_اندازه_MAX;
  }


  ساكن فراغ
  ft_mem_debug_panic( ثابت حرف*  fmt,
                      ... )
  {
    va_list  ap;


    چاپ_کړه( "FreeType.Debug: " );

    va_پیل( ap, fmt );
    vprintf( fmt, ap );
    va_پای_ټکی( ap );

    چاپ_کړه( "\n" );
    وتل( EXIT_FAILURE );
  }


  ساكن FT_اشاره_ګر
  ft_mem_جدول_alloc( FT_Mem_جدول  جدول,
                      FT_Long      اندازه )
  {
    FT_حافظه   حافظه = جدول->حافظه;
    FT_اشاره_ګر  بلاک;


    حافظه->user = جدول->حافظه_user;
    بلاک = جدول->alloc( حافظه, اندازه );
    حافظه->user = جدول;

    أرجع بلاک;
  }


  ساكن فراغ
  ft_mem_جدول_free( FT_Mem_جدول  جدول,
                     FT_اشاره_ګر   بلاک )
  {
    FT_حافظه  حافظه = جدول->حافظه;


    حافظه->user = جدول->حافظه_user;
    جدول->حرر( حافظه, بلاک );
    حافظه->user = جدول;
  }


  ساكن فراغ
  ft_mem_جدول_resize( FT_Mem_جدول  جدول )
  {
    FT_Long  new_اندازه;


    new_اندازه = ft_mem_closest_اولي( جدول->nodes );
    إذا ( new_اندازه != جدول->اندازه )
    {
      FT_Mem_غوټه*  new_buckets;
      FT_Long      i;


      new_buckets = (FT_Mem_غوټه *)
                      ft_mem_جدول_alloc(
                        جدول,
                        new_اندازه * (FT_Long)حجم ( FT_Mem_غوټه ) );
      إذا ( !new_buckets )
        أرجع;

      FT_ARRAY_ZERO( new_buckets, new_اندازه );

      لكل ( i = 0; i < جدول->اندازه; i++ )
      {
        FT_Mem_غوټه  غوټه, بل, *pnode;
        FT_PtrDist  hash;


        غوټه = جدول->buckets[i];
        طالما ( غوټه )
        {
          بل  = غوټه->link;
          hash  = FT_MEM_VAL( غوټه->پته ) % (FT_PtrDist)new_اندازه;
          pnode = new_buckets + hash;

          غوټه->link = pnode[0];
          pnode[0]   = غوټه;

          غوټه = بل;
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
    FT_Long  leak_شمېر = 0;
    FT_Long  leaks      = 0;


    /* remove all blocks from the table, revealing leaked ones */
    لكل ( i = 0; i < جدول->اندازه; i++ )
    {
      FT_Mem_غوټه  *pnode = جدول->buckets + i, بل, غوټه = *pnode;


      طالما ( غوټه )
      {
        بل       = غوټه->link;
        غوټه->link = عدم;

        إذا ( غوټه->اندازه > 0 )
        {
          چاپ_کړه(
            "leaked memory block at address %p, size %8ld in (%s:%ld)\n",
            (فراغ*)غوټه->پته,
            غوټه->اندازه,
            FT_FILENAME( غوټه->سرچینه->file_name ),
            غوټه->سرچینه->کرښه_no );

          leak_شمېر++;
          leaks += غوټه->اندازه;

          ft_mem_جدول_free( جدول, غوټه->پته );
        }

        غوټه->پته = عدم;
        غوټه->اندازه    = 0;

        ft_mem_جدول_free( جدول, غوټه );
        غوټه = بل;
      }
      جدول->buckets[i] = عدم;
    }

    ft_mem_جدول_free( جدول, جدول->buckets );
    جدول->buckets = عدم;

    جدول->اندازه  = 0;
    جدول->nodes = 0;

    /* remove all sources */
    لكل ( i = 0; i < FT_MEM_سرچینه_BUCKETS; i++ )
    {
      FT_Mem_سرچینه  سرچینه, بل;


      لكل ( سرچینه = جدول->sources[i]; سرچینه != عدم; سرچینه = بل )
      {
        بل = سرچینه->link;
        ft_mem_جدول_free( جدول, سرچینه );
      }

      جدول->sources[i] = عدم;
    }

    چاپ_کړه( "FreeType: total memory allocations = %ld\n",
            جدول->alloc_ټول );
    چاپ_کړه( "FreeType: maximum memory footprint = %ld\n",
            جدول->alloc_max );

    إذا ( leak_شمېر > 0 )
      ft_mem_debug_panic(
        "FreeType: %ld bytes of memory leaked in %ld blocks\n",
        leaks, leak_شمېر );

    چاپ_کړه( "FreeType: no memory leaks detected\n" );
  }


  ساكن FT_Mem_غوټه*
  ft_mem_جدول_get_nodep( FT_Mem_جدول  جدول,
                          FT_Byte*     پته )
  {
    FT_PtrDist   hash;
    FT_Mem_غوټه  *pnode, غوټه;


    hash  = FT_MEM_VAL( پته );
    pnode = جدول->buckets + ( hash % (FT_PtrDist)جدول->اندازه );

    لكل (;;)
    {
      غوټه = pnode[0];
      إذا ( !غوټه )
        اكسر;

      إذا ( غوټه->پته == پته )
        اكسر;

      pnode = &غوټه->link;
    }
    أرجع pnode;
  }


  ساكن FT_Mem_سرچینه
  ft_mem_جدول_get_سرچینه( FT_Mem_جدول  جدول )
  {
    FT_UInt32     hash;
    FT_Mem_سرچینه  غوټه, *pnode;


    /* cast to FT_PtrDist first since void* can be larger */
    /* than FT_UInt32 and GCC 4.1.1 emits a warning       */
    hash  = (FT_UInt32)(FT_PtrDist)(فراغ*)ft_debug_file_ +
              (FT_UInt32)( 5 * ft_debug_lineno_ );
    pnode = &جدول->sources[hash % FT_MEM_سرچینه_BUCKETS];

    لكل (;;)
    {
      غوټه = *pnode;
      إذا ( !غوټه )
        اكسر;

      إذا ( غوټه->file_name == ft_debug_file_   &&
           غوټه->کرښه_no   == ft_debug_lineno_ )
        اذهب Exit;

      pnode = &غوټه->link;
    }

    غوټه = (FT_Mem_سرچینه)ft_mem_جدول_alloc( جدول, حجم ( *غوټه ) );
    إذا ( !غوټه )
      ft_mem_debug_panic(
        "not enough memory to perform memory debugging\n" );

    غوټه->file_name = ft_debug_file_;
    غوټه->کرښه_no   = ft_debug_lineno_;

    غوټه->cur_بلاکونه = 0;
    غوټه->max_بلاکونه = 0;
    غوټه->all_بلاکونه = 0;

    غوټه->cur_اندازه = 0;
    غوټه->max_اندازه = 0;
    غوټه->all_اندازه = 0;

    غوټه->cur_max = 0;

    غوټه->link = عدم;
    غوټه->hash = hash;
    *pnode     = غوټه;

  Exit:
    أرجع غوټه;
  }


  ساكن فراغ
  ft_mem_جدول_سېټ( FT_Mem_جدول  جدول,
                    FT_Byte*     پته,
                    FT_Long      اندازه,
                    FT_Long      delta )
  {
    FT_Mem_غوټه  *pnode, غوټه;


    إذا ( جدول )
    {
      FT_Mem_سرچینه  سرچینه;


      pnode = ft_mem_جدول_get_nodep( جدول, پته );
      غوټه  = *pnode;
      إذا ( غوټه )
      {
        إذا ( غوټه->اندازه < 0 )
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
            غوټه->پته, غوټه->اندازه,
            FT_FILENAME( غوټه->سرچینه->file_name ), غوټه->سرچینه->کرښه_no,
            FT_FILENAME( ft_debug_file_ ), ft_debug_lineno_ );
        }
      }

      /* we need to create a new node in this table */
      غوټه = (FT_Mem_غوټه)ft_mem_جدول_alloc( جدول, حجم ( *غوټه ) );
      إذا ( !غوټه )
        ft_mem_debug_panic( "not enough memory to run memory tests" );

      غوټه->پته = پته;
      غوټه->اندازه    = اندازه;
      غوټه->سرچینه  = سرچینه = ft_mem_جدول_get_سرچینه( جدول );

      إذا ( delta == 0 )
      {
        /* this is an allocation */
        سرچینه->all_بلاکونه++;
        سرچینه->cur_بلاکونه++;
        إذا ( سرچینه->cur_بلاکونه > سرچینه->max_بلاکونه )
          سرچینه->max_بلاکونه = سرچینه->cur_بلاکونه;
      }

      إذا ( اندازه > سرچینه->cur_max )
        سرچینه->cur_max = اندازه;

      إذا ( delta != 0 )
      {
        /* we are growing or shrinking a reallocated block */
        سرچینه->cur_اندازه     += delta;
        جدول->alloc_current += delta;
      }
      وإلا
      {
        /* we are allocating a new block */
        سرچینه->cur_اندازه     += اندازه;
        جدول->alloc_current += اندازه;
      }

      سرچینه->all_اندازه += اندازه;

      إذا ( سرچینه->cur_اندازه > سرچینه->max_اندازه )
        سرچینه->max_اندازه = سرچینه->cur_اندازه;

      غوټه->free_file_name = عدم;
      غوټه->free_کرښه_no   = 0;

      غوټه->link = pnode[0];

      pnode[0] = غوټه;
      جدول->nodes++;

      جدول->alloc_ټول += اندازه;

      إذا ( جدول->alloc_current > جدول->alloc_max )
        جدول->alloc_max = جدول->alloc_current;

      إذا ( جدول->nodes * 3 < جدول->اندازه  ||
           جدول->اندازه  * 3 < جدول->nodes )
        ft_mem_جدول_resize( جدول );
    }
  }


  ساكن فراغ
  ft_mem_جدول_remove( FT_Mem_جدول  جدول,
                       FT_Byte*     پته,
                       FT_Long      delta )
  {
    إذا ( جدول )
    {
      FT_Mem_غوټه  *pnode, غوټه;


      pnode = ft_mem_جدول_get_nodep( جدول, پته );
      غوټه  = *pnode;
      إذا ( غوټه )
      {
        FT_Mem_سرچینه  سرچینه;


        إذا ( غوټه->اندازه < 0 )
          ft_mem_debug_panic(
            "freeing memory block at %p more than once\n"
            "  at (%s:%ld)!\n"
            "  Block was allocated at (%s:%ld)\n"
            "  and released at (%s:%ld).",
            پته,
            FT_FILENAME( ft_debug_file_ ), ft_debug_lineno_,
            FT_FILENAME( غوټه->سرچینه->file_name ), غوټه->سرچینه->کرښه_no,
            FT_FILENAME( غوټه->free_file_name ), غوټه->free_کرښه_no );

        /* scramble the node's content for additional safety */
        FT_MEM_سېټ( پته, 0xF3, غوټه->اندازه );

        إذا ( delta == 0 )
        {
          سرچینه = غوټه->سرچینه;

          سرچینه->cur_بلاکونه--;
          سرچینه->cur_اندازه -= غوټه->اندازه;

          جدول->alloc_current -= غوټه->اندازه;
        }

        إذا ( جدول->keep_alive )
        {
          /* we simply invert the node's size to indicate that the node */
          /* was freed.                                                 */
          غوټه->اندازه           = -غوټه->اندازه;
          غوټه->free_file_name = ft_debug_file_;
          غوټه->free_کرښه_no   = ft_debug_lineno_;
        }
        وإلا
        {
          جدول->nodes--;

          *pnode = غوټه->link;

          غوټه->اندازه   = 0;
          غوټه->سرچینه = عدم;

          ft_mem_جدول_free( جدول, غوټه );

          إذا ( جدول->nodes * 3 < جدول->اندازه  ||
               جدول->اندازه  * 3 < جدول->nodes )
            ft_mem_جدول_resize( جدول );
        }
      }
      وإلا
        ft_mem_debug_panic(
          "trying to free unknown block at %p in (%s:%ld)\n",
          پته,
          FT_FILENAME( ft_debug_file_ ), ft_debug_lineno_ );
    }
  }


  ساكن FT_اشاره_ګر
  ft_mem_debug_alloc( FT_حافظه  حافظه,
                      FT_Long    اندازه )
  {
    FT_Mem_جدول  جدول = (FT_Mem_جدول)حافظه->user;
    FT_Byte*     بلاک;


    إذا ( اندازه <= 0 )
      ft_mem_debug_panic( "negative block size allocation (%ld)", اندازه );

    /* return NULL if the maximum number of allocations was reached */
    إذا ( جدول->bound_شمېر                           &&
         جدول->alloc_شمېر >= جدول->alloc_شمېر_max )
      أرجع عدم;

    /* return NULL if this allocation would overflow the maximum heap size */
    إذا ( جدول->bound_ټول                                   &&
         جدول->alloc_ټول_max - جدول->alloc_current > اندازه )
      أرجع عدم;

    بلاک = (FT_Byte *)ft_mem_جدول_alloc( جدول, اندازه );
    إذا ( بلاک )
    {
      ft_mem_جدول_سېټ( جدول, بلاک, اندازه, 0 );

      جدول->alloc_شمېر++;
    }

    ft_debug_file_   = "<unknown>";
    ft_debug_lineno_ = 0;

    أرجع (FT_اشاره_ګر)بلاک;
  }


  ساكن فراغ
  ft_mem_debug_free( FT_حافظه   حافظه,
                     FT_اشاره_ګر  بلاک )
  {
    FT_Mem_جدول  جدول = (FT_Mem_جدول)حافظه->user;


    إذا ( !بلاک )
      ft_mem_debug_panic( "trying to free NULL in (%s:%ld)",
                          FT_FILENAME( ft_debug_file_ ),
                          ft_debug_lineno_ );

    ft_mem_جدول_remove( جدول, (FT_Byte*)بلاک, 0 );

    إذا ( !جدول->keep_alive )
      ft_mem_جدول_free( جدول, بلاک );

    جدول->alloc_شمېر--;

    ft_debug_file_   = "<unknown>";
    ft_debug_lineno_ = 0;
  }


  ساكن FT_اشاره_ګر
  ft_mem_debug_realloc( FT_حافظه   حافظه,
                        FT_Long     cur_اندازه,
                        FT_Long     new_اندازه,
                        FT_اشاره_ګر  بلاک )
  {
    FT_Mem_جدول  جدول = (FT_Mem_جدول)حافظه->user;
    FT_Mem_غوټه   غوټه, *pnode;
    FT_اشاره_ګر   new_بلاک;
    FT_Long      delta;

    ثابت حرف*  file_name = FT_FILENAME( ft_debug_file_ );
    FT_Long      کرښه_no   = ft_debug_lineno_;


    /* unlikely, but possible */
    إذا ( new_اندازه == cur_اندازه )
      أرجع بلاک;

    /* the following is valid according to ANSI C */
#إذا 0
    إذا ( !بلاک || !cur_اندازه )
      ft_mem_debug_panic( "trying to reallocate NULL in (%s:%ld)",
                          file_name, کرښه_no );
#د_که_پای

    /* while the following is allowed in ANSI C also, we abort since */
    /* such case should be handled by FreeType.                      */
    إذا ( new_اندازه <= 0 )
      ft_mem_debug_panic(
        "trying to reallocate %p to size 0 (current is %ld) in (%s:%ld)",
        بلاک, cur_اندازه, file_name, کرښه_no );

    /* check `cur_size' value */
    pnode = ft_mem_جدول_get_nodep( جدول, (FT_Byte*)بلاک );
    غوټه  = *pnode;
    إذا ( !غوټه )
      ft_mem_debug_panic(
        "trying to reallocate unknown block at %p in (%s:%ld)",
        بلاک, file_name, کرښه_no );

    إذا ( غوټه->اندازه <= 0 )
      ft_mem_debug_panic(
        "trying to reallocate freed block at %p in (%s:%ld)",
        بلاک, file_name, کرښه_no );

    إذا ( غوټه->اندازه != cur_اندازه )
      ft_mem_debug_panic( "invalid ft_realloc request for %p. cur_size is "
                          "%ld instead of %ld in (%s:%ld)",
                          بلاک, cur_اندازه, غوټه->اندازه, file_name, کرښه_no );

    /* return NULL if the maximum number of allocations was reached */
    إذا ( جدول->bound_شمېر                           &&
         جدول->alloc_شمېر >= جدول->alloc_شمېر_max )
      أرجع عدم;

    delta = new_اندازه - cur_اندازه;

    /* return NULL if this allocation would overflow the maximum heap size */
    إذا ( delta > 0                                             &&
         جدول->bound_ټول                                    &&
         جدول->alloc_current + delta > جدول->alloc_ټول_max )
      أرجع عدم;

    new_بلاک = (FT_اشاره_ګر)ft_mem_جدول_alloc( جدول, new_اندازه );
    إذا ( !new_بلاک )
      أرجع عدم;

    ft_mem_جدول_سېټ( جدول, (FT_Byte*)new_بلاک, new_اندازه, delta );

    ft_memcpy( new_بلاک, بلاک, cur_اندازه < new_اندازه ? (size_t)cur_اندازه
                                                     : (size_t)new_اندازه );

    ft_mem_جدول_remove( جدول, (FT_Byte*)بلاک, delta );

    ft_debug_file_   = "<unknown>";
    ft_debug_lineno_ = 0;

    إذا ( !جدول->keep_alive )
      ft_mem_جدول_free( جدول, بلاک );

    أرجع new_بلاک;
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
          FT_Long  ټول_max = ft_strtol( p, عدم, 10 );


          إذا ( ټول_max > 0 )
          {
            جدول->bound_ټول     = 1;
            جدول->alloc_ټول_max = ټول_max;
          }
        }

        p = ft_getenv( "FT2_ALLOC_COUNT_MAX" );
        إذا ( p )
        {
          FT_Long  ټول_شمېر = ft_strtol( p, عدم, 10 );


          إذا ( ټول_شمېر > 0 )
          {
            جدول->bound_شمېر     = 1;
            جدول->alloc_شمېر_max = ټول_شمېر;
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
  ft_mem_debug_بشپړ( FT_حافظه  حافظه )
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
  ft_mem_سرچینه_compare( ثابت فراغ*  p1,
                         ثابت فراغ*  p2 )
  {
    FT_Mem_سرچینه  s1 = *(FT_Mem_سرچینه*)p1;
    FT_Mem_سرچینه  s2 = *(FT_Mem_سرچینه*)p2;


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
      FT_Mem_سرچینه*  لوښی = جدول->sources;
      FT_Mem_سرچینه*  حد  = لوښی + FT_MEM_سرچینه_BUCKETS;
      FT_Mem_سرچینه*  sources;
      FT_Int         nn, شمېر;
      ثابت حرف*    fmt;


      شمېر = 0;
      لكل ( ; لوښی < حد; لوښی++ )
      {
        FT_Mem_سرچینه  سرچینه = *لوښی;


        لكل ( ; سرچینه; سرچینه = سرچینه->link )
          شمېر++;
      }

      sources = (FT_Mem_سرچینه*)
                  ft_mem_جدول_alloc(
                    جدول, شمېر * (FT_Long)حجم ( *sources ) );

      شمېر = 0;
      لكل ( لوښی = جدول->sources; لوښی < حد; لوښی++ )
      {
        FT_Mem_سرچینه  سرچینه = *لوښی;


        لكل ( ; سرچینه; سرچینه = سرچینه->link )
          sources[شمېر++] = سرچینه;
      }

      ft_qsort( sources,
                (size_t)شمېر,
                حجم ( *sources ),
                ft_mem_سرچینه_compare );

      چاپ_کړه( "FreeType Memory Dump: "
              "current=%ld max=%ld total=%ld count=%ld\n",
              جدول->alloc_current, جدول->alloc_max,
              جدول->alloc_ټول, جدول->alloc_شمېر );
      چاپ_کړه( " block  block    sizes    sizes    sizes   source\n" );
      چاپ_کړه( " count   high      sum  highsum      max   location\n" );
      چاپ_کړه( "-------------------------------------------------\n" );

      fmt = "%6ld %6ld %8ld %8ld %8ld %s:%d\n";

      لكل ( nn = 0; nn < شمېر; nn++ )
      {
        FT_Mem_سرچینه  سرچینه = sources[nn];


        چاپ_کړه( fmt,
                سرچینه->cur_بلاکونه, سرچینه->max_بلاکونه,
                سرچینه->cur_اندازه, سرچینه->max_اندازه, سرچینه->cur_max,
                FT_FILENAME( سرچینه->file_name ),
                سرچینه->کرښه_no );
      }
      چاپ_کړه( "------------------------------------------------\n" );

      ft_mem_جدول_free( جدول, sources );
    }
  }

#وإلا  /* !FT_DEBUG_MEMORY */

  /* ANSI C doesn't like empty source files */
  تعريفنوع صحيح  debug_mem_dummy_;

#د_که_پای /* !FT_DEBUG_MEMORY */


/*پای_ټکی*/
