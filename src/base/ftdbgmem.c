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


#شامل <ft2build.h>
#شامل FT_CONFIG_CONFIG_H
#شامل <freetype/internal/ftdebug.h>
#شامل <freetype/internal/ftmemory.h>
#شامل <freetype/ftsystem.h>
#شامل <freetype/fterrors.h>
#شامل <freetype/fttypes.h>


#اگر_تعریف_شدہ FT_DEBUG_حافظہ

#تعریف_کرو  KEEPALIVE /* `Keep alive' means that freed blocks aren't released
                    * to the heap.  This is useful to detect double-frees
                    * or weird heap corruption, but it uses large amounts of
                    * memory, however.
                    */

#شامل FT_CONFIG_STANDARD_LIBRARY_H

  FT_بنیاد_DEF( ثابت حرف* )  ft_debug_file_   = تہی;
  FT_بنیاد_DEF( طویل )         ft_debug_lineno_ = 0;

  خارجی خالی
  FT_Dump_حافظہ( FT_حافظہ  حافظہ );


  قسمتعریف ساخت FT_Mem_ماخذ_Rec*  FT_Mem_ماخذ;
  قسمتعریف ساخت FT_Mem_گرہ_Rec*    FT_Mem_گرہ;
  قسمتعریف ساخت FT_Mem_جدول_Rec*   FT_Mem_جدول;


#تعریف_کرو FT_MEM_VAL( addr )  ( (FT_PtrDist)(FT_اشارہ_گر)( addr ) )

  /*
   * This structure holds statistics for a single allocation/release
   * site.  This is useful to know where memory operations happen the
   * most.
   */
  قسمتعریف ساخت  FT_Mem_ماخذ_Rec
  {
    ثابت حرف*   file_name;
    طویل          سطر_no;

    FT_Long       cur_بلاکیں;   /* current number of allocated blocks */
    FT_Long       max_بلاکیں;   /* max. number of allocated blocks    */
    FT_Long       all_بلاکیں;   /* total number of blocks allocated   */

    FT_Long       cur_سائز;     /* current cumulative allocated size */
    FT_Long       max_سائز;     /* maximum cumulative allocated size */
    FT_Long       all_سائز;     /* total cumulative allocated size   */

    FT_Long       cur_max;      /* current maximum allocated size */

    FT_UInt32     hash;
    FT_Mem_ماخذ  link;

  } FT_Mem_ماخذ_Rec_2;


  /*
   * We don't need a resizable array for the memory sources because
   * their number is pretty limited within FreeType.
   */
#تعریف_کرو FT_MEM_ماخذ_BUCKETS  128

  /*
   * This structure holds information related to a single allocated
   * memory block.  If KEEPALIVE is defined, blocks that are freed by
   * FreeType are never released to the system.  Instead, their `size'
   * field is set to `-size'.  This is mainly useful to detect double
   * frees, at the price of a large memory footprint during execution.
   */
  قسمتعریف ساخت  FT_Mem_گرہ_Rec
  {
    FT_Byte*      پتہ;
    FT_Long       سائز;     /* < 0 if the block was freed */

    FT_Mem_ماخذ  ماخذ;

#اگر_تعریف_شدہ KEEPALIVE
    ثابت حرف*   free_file_name;
    FT_Long       free_سطر_no;
#اگر_ختم

    FT_Mem_گرہ    link;

  } FT_Mem_گرہ_Rec_2;


  /*
   * The global structure, containing compound statistics and all hash
   * tables.
   */
  قسمتعریف ساخت  FT_Mem_جدول_Rec
  {
    FT_Long          سائز;
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

    FT_Mem_ماخذ     sources[FT_MEM_ماخذ_BUCKETS];

    FT_Bool          keep_alive;

    FT_حافظہ        حافظہ;
    FT_اشارہ_گر       حافظہ_user;
    FT_Alloc_Func    alloc;
    FT_Free_Func     آزاد;
    FT_Realloc_Func  realloc;

  } FT_Mem_جدول_Rec_2;


#تعریف_کرو FT_MEM_سائز_MAX  13845163

#تعریف_کرو FT_FILENAME( x )  ( (x) ? (x) : "unknown file" )


  /*
   * Prime numbers are ugly to handle.  It would be better to implement
   * L-Hashing, which is 10% faster and doesn't require divisions.
   */
  ساکن ثابت FT_Int  ft_mem_مفرد_اعداد[] =
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


  ساکن FT_Long
  ft_mem_closest_مفرد( FT_Long  عدد )
  {
    size_t  i;


    برائے ( i = 0;
          i < حجم ( ft_mem_مفرد_اعداد ) / حجم ( ft_mem_مفرد_اعداد[0] ); i++ )
      اگر ( ft_mem_مفرد_اعداد[i] > عدد )
        واپس ft_mem_مفرد_اعداد[i];

    واپس FT_MEM_سائز_MAX;
  }


  ساکن خالی
  ft_mem_debug_panic( ثابت حرف*  fmt,
                      ... )
  {
    va_list  ap;


    لکھو( "FreeType.Debug: " );

    va_آغاز( ap, fmt );
    vprintf( fmt, ap );
    va_انتہا( ap );

    لکھو( "\n" );
    باہر_نکلو( EXIT_FAILURE );
  }


  ساکن FT_اشارہ_گر
  ft_mem_جدول_alloc( FT_Mem_جدول  جدول,
                      FT_Long      سائز )
  {
    FT_حافظہ   حافظہ = جدول->حافظہ;
    FT_اشارہ_گر  بلاک;


    حافظہ->user = جدول->حافظہ_user;
    بلاک = جدول->alloc( حافظہ, سائز );
    حافظہ->user = جدول;

    واپس بلاک;
  }


  ساکن خالی
  ft_mem_جدول_free( FT_Mem_جدول  جدول,
                     FT_اشارہ_گر   بلاک )
  {
    FT_حافظہ  حافظہ = جدول->حافظہ;


    حافظہ->user = جدول->حافظہ_user;
    جدول->آزاد( حافظہ, بلاک );
    حافظہ->user = جدول;
  }


  ساکن خالی
  ft_mem_جدول_resize( FT_Mem_جدول  جدول )
  {
    FT_Long  new_سائز;


    new_سائز = ft_mem_closest_مفرد( جدول->nodes );
    اگر ( new_سائز != جدول->سائز )
    {
      FT_Mem_گرہ*  new_buckets;
      FT_Long      i;


      new_buckets = (FT_Mem_گرہ *)
                      ft_mem_جدول_alloc(
                        جدول,
                        new_سائز * (FT_Long)حجم ( FT_Mem_گرہ ) );
      اگر ( !new_buckets )
        واپس;

      FT_ARRAY_ZERO( new_buckets, new_سائز );

      برائے ( i = 0; i < جدول->سائز; i++ )
      {
        FT_Mem_گرہ  گرہ, اگلا, *pnode;
        FT_PtrDist  hash;


        گرہ = جدول->buckets[i];
        جب ( گرہ )
        {
          اگلا  = گرہ->link;
          hash  = FT_MEM_VAL( گرہ->پتہ ) % (FT_PtrDist)new_سائز;
          pnode = new_buckets + hash;

          گرہ->link = pnode[0];
          pnode[0]   = گرہ;

          گرہ = اگلا;
        }
      }

      اگر ( جدول->buckets )
        ft_mem_جدول_free( جدول, جدول->buckets );

      جدول->buckets = new_buckets;
      جدول->سائز    = new_سائز;
    }
  }


  ساکن خالی
  ft_mem_جدول_destroy( FT_Mem_جدول  جدول )
  {
    FT_Long  i;
    FT_Long  leak_گنتی = 0;
    FT_Long  leaks      = 0;


    /* remove all blocks from the table, revealing leaked ones */
    برائے ( i = 0; i < جدول->سائز; i++ )
    {
      FT_Mem_گرہ  *pnode = جدول->buckets + i, اگلا, گرہ = *pnode;


      جب ( گرہ )
      {
        اگلا       = گرہ->link;
        گرہ->link = تہی;

        اگر ( گرہ->سائز > 0 )
        {
          لکھو(
            "leaked memory block at address %p, size %8ld in (%s:%ld)\n",
            (خالی*)گرہ->پتہ,
            گرہ->سائز,
            FT_FILENAME( گرہ->ماخذ->file_name ),
            گرہ->ماخذ->سطر_no );

          leak_گنتی++;
          leaks += گرہ->سائز;

          ft_mem_جدول_free( جدول, گرہ->پتہ );
        }

        گرہ->پتہ = تہی;
        گرہ->سائز    = 0;

        ft_mem_جدول_free( جدول, گرہ );
        گرہ = اگلا;
      }
      جدول->buckets[i] = تہی;
    }

    ft_mem_جدول_free( جدول, جدول->buckets );
    جدول->buckets = تہی;

    جدول->سائز  = 0;
    جدول->nodes = 0;

    /* remove all sources */
    برائے ( i = 0; i < FT_MEM_ماخذ_BUCKETS; i++ )
    {
      FT_Mem_ماخذ  ماخذ, اگلا;


      برائے ( ماخذ = جدول->sources[i]; ماخذ != تہی; ماخذ = اگلا )
      {
        اگلا = ماخذ->link;
        ft_mem_جدول_free( جدول, ماخذ );
      }

      جدول->sources[i] = تہی;
    }

    لکھو( "FreeType: total memory allocations = %ld\n",
            جدول->alloc_کل );
    لکھو( "FreeType: maximum memory footprint = %ld\n",
            جدول->alloc_max );

    اگر ( leak_گنتی > 0 )
      ft_mem_debug_panic(
        "FreeType: %ld bytes of memory leaked in %ld blocks\n",
        leaks, leak_گنتی );

    لکھو( "FreeType: no memory leaks detected\n" );
  }


  ساکن FT_Mem_گرہ*
  ft_mem_جدول_get_nodep( FT_Mem_جدول  جدول,
                          FT_Byte*     پتہ )
  {
    FT_PtrDist   hash;
    FT_Mem_گرہ  *pnode, گرہ;


    hash  = FT_MEM_VAL( پتہ );
    pnode = جدول->buckets + ( hash % (FT_PtrDist)جدول->سائز );

    برائے (;;)
    {
      گرہ = pnode[0];
      اگر ( !گرہ )
        توڑو;

      اگر ( گرہ->پتہ == پتہ )
        توڑو;

      pnode = &گرہ->link;
    }
    واپس pnode;
  }


  ساکن FT_Mem_ماخذ
  ft_mem_جدول_get_ماخذ( FT_Mem_جدول  جدول )
  {
    FT_UInt32     hash;
    FT_Mem_ماخذ  گرہ, *pnode;


    /* cast to FT_PtrDist first since void* can be larger */
    /* than FT_UInt32 and GCC 4.1.1 emits a warning       */
    hash  = (FT_UInt32)(FT_PtrDist)(خالی*)ft_debug_file_ +
              (FT_UInt32)( 5 * ft_debug_lineno_ );
    pnode = &جدول->sources[hash % FT_MEM_ماخذ_BUCKETS];

    برائے (;;)
    {
      گرہ = *pnode;
      اگر ( !گرہ )
        توڑو;

      اگر ( گرہ->file_name == ft_debug_file_   &&
           گرہ->سطر_no   == ft_debug_lineno_ )
        جاؤ Exit;

      pnode = &گرہ->link;
    }

    گرہ = (FT_Mem_ماخذ)ft_mem_جدول_alloc( جدول, حجم ( *گرہ ) );
    اگر ( !گرہ )
      ft_mem_debug_panic(
        "not enough memory to perform memory debugging\n" );

    گرہ->file_name = ft_debug_file_;
    گرہ->سطر_no   = ft_debug_lineno_;

    گرہ->cur_بلاکیں = 0;
    گرہ->max_بلاکیں = 0;
    گرہ->all_بلاکیں = 0;

    گرہ->cur_سائز = 0;
    گرہ->max_سائز = 0;
    گرہ->all_سائز = 0;

    گرہ->cur_max = 0;

    گرہ->link = تہی;
    گرہ->hash = hash;
    *pnode     = گرہ;

  Exit:
    واپس گرہ;
  }


  ساکن خالی
  ft_mem_جدول_سیٹ( FT_Mem_جدول  جدول,
                    FT_Byte*     پتہ,
                    FT_Long      سائز,
                    FT_Long      delta )
  {
    FT_Mem_گرہ  *pnode, گرہ;


    اگر ( جدول )
    {
      FT_Mem_ماخذ  ماخذ;


      pnode = ft_mem_جدول_get_nodep( جدول, پتہ );
      گرہ  = *pnode;
      اگر ( گرہ )
      {
        اگر ( گرہ->سائز < 0 )
        {
          /* This block was already freed.  Our memory is now completely */
          /* corrupted!                                                  */
          /* This can only happen in keep-alive mode.                    */
          ft_mem_debug_panic(
            "memory heap corrupted (allocating freed block)" );
        }
        ورنہ
        {
          /* This block was already allocated.  This means that our memory */
          /* is also corrupted!                                            */
          ft_mem_debug_panic(
            "memory heap corrupted (re-allocating allocated block at"
            " %p, of size %ld)\n"
            "org=%s:%d new=%s:%d\n",
            گرہ->پتہ, گرہ->سائز,
            FT_FILENAME( گرہ->ماخذ->file_name ), گرہ->ماخذ->سطر_no,
            FT_FILENAME( ft_debug_file_ ), ft_debug_lineno_ );
        }
      }

      /* we need to create a new node in this table */
      گرہ = (FT_Mem_گرہ)ft_mem_جدول_alloc( جدول, حجم ( *گرہ ) );
      اگر ( !گرہ )
        ft_mem_debug_panic( "not enough memory to run memory tests" );

      گرہ->پتہ = پتہ;
      گرہ->سائز    = سائز;
      گرہ->ماخذ  = ماخذ = ft_mem_جدول_get_ماخذ( جدول );

      اگر ( delta == 0 )
      {
        /* this is an allocation */
        ماخذ->all_بلاکیں++;
        ماخذ->cur_بلاکیں++;
        اگر ( ماخذ->cur_بلاکیں > ماخذ->max_بلاکیں )
          ماخذ->max_بلاکیں = ماخذ->cur_بلاکیں;
      }

      اگر ( سائز > ماخذ->cur_max )
        ماخذ->cur_max = سائز;

      اگر ( delta != 0 )
      {
        /* we are growing or shrinking a reallocated block */
        ماخذ->cur_سائز     += delta;
        جدول->alloc_current += delta;
      }
      ورنہ
      {
        /* we are allocating a new block */
        ماخذ->cur_سائز     += سائز;
        جدول->alloc_current += سائز;
      }

      ماخذ->all_سائز += سائز;

      اگر ( ماخذ->cur_سائز > ماخذ->max_سائز )
        ماخذ->max_سائز = ماخذ->cur_سائز;

      گرہ->free_file_name = تہی;
      گرہ->free_سطر_no   = 0;

      گرہ->link = pnode[0];

      pnode[0] = گرہ;
      جدول->nodes++;

      جدول->alloc_کل += سائز;

      اگر ( جدول->alloc_current > جدول->alloc_max )
        جدول->alloc_max = جدول->alloc_current;

      اگر ( جدول->nodes * 3 < جدول->سائز  ||
           جدول->سائز  * 3 < جدول->nodes )
        ft_mem_جدول_resize( جدول );
    }
  }


  ساکن خالی
  ft_mem_جدول_remove( FT_Mem_جدول  جدول,
                       FT_Byte*     پتہ,
                       FT_Long      delta )
  {
    اگر ( جدول )
    {
      FT_Mem_گرہ  *pnode, گرہ;


      pnode = ft_mem_جدول_get_nodep( جدول, پتہ );
      گرہ  = *pnode;
      اگر ( گرہ )
      {
        FT_Mem_ماخذ  ماخذ;


        اگر ( گرہ->سائز < 0 )
          ft_mem_debug_panic(
            "freeing memory block at %p more than once\n"
            "  at (%s:%ld)!\n"
            "  Block was allocated at (%s:%ld)\n"
            "  and released at (%s:%ld).",
            پتہ,
            FT_FILENAME( ft_debug_file_ ), ft_debug_lineno_,
            FT_FILENAME( گرہ->ماخذ->file_name ), گرہ->ماخذ->سطر_no,
            FT_FILENAME( گرہ->free_file_name ), گرہ->free_سطر_no );

        /* scramble the node's content for additional safety */
        FT_MEM_سیٹ( پتہ, 0xF3, گرہ->سائز );

        اگر ( delta == 0 )
        {
          ماخذ = گرہ->ماخذ;

          ماخذ->cur_بلاکیں--;
          ماخذ->cur_سائز -= گرہ->سائز;

          جدول->alloc_current -= گرہ->سائز;
        }

        اگر ( جدول->keep_alive )
        {
          /* we simply invert the node's size to indicate that the node */
          /* was freed.                                                 */
          گرہ->سائز           = -گرہ->سائز;
          گرہ->free_file_name = ft_debug_file_;
          گرہ->free_سطر_no   = ft_debug_lineno_;
        }
        ورنہ
        {
          جدول->nodes--;

          *pnode = گرہ->link;

          گرہ->سائز   = 0;
          گرہ->ماخذ = تہی;

          ft_mem_جدول_free( جدول, گرہ );

          اگر ( جدول->nodes * 3 < جدول->سائز  ||
               جدول->سائز  * 3 < جدول->nodes )
            ft_mem_جدول_resize( جدول );
        }
      }
      ورنہ
        ft_mem_debug_panic(
          "trying to free unknown block at %p in (%s:%ld)\n",
          پتہ,
          FT_FILENAME( ft_debug_file_ ), ft_debug_lineno_ );
    }
  }


  ساکن FT_اشارہ_گر
  ft_mem_debug_alloc( FT_حافظہ  حافظہ,
                      FT_Long    سائز )
  {
    FT_Mem_جدول  جدول = (FT_Mem_جدول)حافظہ->user;
    FT_Byte*     بلاک;


    اگر ( سائز <= 0 )
      ft_mem_debug_panic( "negative block size allocation (%ld)", سائز );

    /* return NULL if the maximum number of allocations was reached */
    اگر ( جدول->bound_گنتی                           &&
         جدول->alloc_گنتی >= جدول->alloc_گنتی_max )
      واپس تہی;

    /* return NULL if this allocation would overflow the maximum heap size */
    اگر ( جدول->bound_کل                                   &&
         جدول->alloc_کل_max - جدول->alloc_current > سائز )
      واپس تہی;

    بلاک = (FT_Byte *)ft_mem_جدول_alloc( جدول, سائز );
    اگر ( بلاک )
    {
      ft_mem_جدول_سیٹ( جدول, بلاک, سائز, 0 );

      جدول->alloc_گنتی++;
    }

    ft_debug_file_   = "<unknown>";
    ft_debug_lineno_ = 0;

    واپس (FT_اشارہ_گر)بلاک;
  }


  ساکن خالی
  ft_mem_debug_free( FT_حافظہ   حافظہ,
                     FT_اشارہ_گر  بلاک )
  {
    FT_Mem_جدول  جدول = (FT_Mem_جدول)حافظہ->user;


    اگر ( !بلاک )
      ft_mem_debug_panic( "trying to free NULL in (%s:%ld)",
                          FT_FILENAME( ft_debug_file_ ),
                          ft_debug_lineno_ );

    ft_mem_جدول_remove( جدول, (FT_Byte*)بلاک, 0 );

    اگر ( !جدول->keep_alive )
      ft_mem_جدول_free( جدول, بلاک );

    جدول->alloc_گنتی--;

    ft_debug_file_   = "<unknown>";
    ft_debug_lineno_ = 0;
  }


  ساکن FT_اشارہ_گر
  ft_mem_debug_realloc( FT_حافظہ   حافظہ,
                        FT_Long     cur_سائز,
                        FT_Long     new_سائز,
                        FT_اشارہ_گر  بلاک )
  {
    FT_Mem_جدول  جدول = (FT_Mem_جدول)حافظہ->user;
    FT_Mem_گرہ   گرہ, *pnode;
    FT_اشارہ_گر   new_بلاک;
    FT_Long      delta;

    ثابت حرف*  file_name = FT_FILENAME( ft_debug_file_ );
    FT_Long      سطر_no   = ft_debug_lineno_;


    /* unlikely, but possible */
    اگر ( new_سائز == cur_سائز )
      واپس بلاک;

    /* the following is valid according to ANSI C */
#اگر 0
    اگر ( !بلاک || !cur_سائز )
      ft_mem_debug_panic( "trying to reallocate NULL in (%s:%ld)",
                          file_name, سطر_no );
#اگر_ختم

    /* while the following is allowed in ANSI C also, we abort since */
    /* such case should be handled by FreeType.                      */
    اگر ( new_سائز <= 0 )
      ft_mem_debug_panic(
        "trying to reallocate %p to size 0 (current is %ld) in (%s:%ld)",
        بلاک, cur_سائز, file_name, سطر_no );

    /* check `cur_size' value */
    pnode = ft_mem_جدول_get_nodep( جدول, (FT_Byte*)بلاک );
    گرہ  = *pnode;
    اگر ( !گرہ )
      ft_mem_debug_panic(
        "trying to reallocate unknown block at %p in (%s:%ld)",
        بلاک, file_name, سطر_no );

    اگر ( گرہ->سائز <= 0 )
      ft_mem_debug_panic(
        "trying to reallocate freed block at %p in (%s:%ld)",
        بلاک, file_name, سطر_no );

    اگر ( گرہ->سائز != cur_سائز )
      ft_mem_debug_panic( "invalid ft_realloc request for %p. cur_size is "
                          "%ld instead of %ld in (%s:%ld)",
                          بلاک, cur_سائز, گرہ->سائز, file_name, سطر_no );

    /* return NULL if the maximum number of allocations was reached */
    اگر ( جدول->bound_گنتی                           &&
         جدول->alloc_گنتی >= جدول->alloc_گنتی_max )
      واپس تہی;

    delta = new_سائز - cur_سائز;

    /* return NULL if this allocation would overflow the maximum heap size */
    اگر ( delta > 0                                             &&
         جدول->bound_کل                                    &&
         جدول->alloc_current + delta > جدول->alloc_کل_max )
      واپس تہی;

    new_بلاک = (FT_اشارہ_گر)ft_mem_جدول_alloc( جدول, new_سائز );
    اگر ( !new_بلاک )
      واپس تہی;

    ft_mem_جدول_سیٹ( جدول, (FT_Byte*)new_بلاک, new_سائز, delta );

    ft_memcpy( new_بلاک, بلاک, cur_سائز < new_سائز ? (size_t)cur_سائز
                                                     : (size_t)new_سائز );

    ft_mem_جدول_remove( جدول, (FT_Byte*)بلاک, delta );

    ft_debug_file_   = "<unknown>";
    ft_debug_lineno_ = 0;

    اگر ( !جدول->keep_alive )
      ft_mem_جدول_free( جدول, بلاک );

    واپس new_بلاک;
  }


  خارجی خالی
  ft_mem_debug_init( FT_حافظہ  حافظہ )
  {
    FT_Mem_جدول  جدول;


    اگر ( !ft_getenv( "FT2_DEBUG_MEMORY" ) )
      واپس;

    جدول = (FT_Mem_جدول)حافظہ->alloc( حافظہ, حجم ( *جدول ) );

    اگر ( جدول )
    {
      FT_ZERO( جدول );

      جدول->حافظہ      = حافظہ;
      جدول->حافظہ_user = حافظہ->user;
      جدول->alloc       = حافظہ->alloc;
      جدول->realloc     = حافظہ->realloc;
      جدول->آزاد        = حافظہ->آزاد;

      ft_mem_جدول_resize( جدول );

      اگر ( جدول->سائز )
      {
        ثابت حرف*  p;


        حافظہ->user    = جدول;
        حافظہ->alloc   = ft_mem_debug_alloc;
        حافظہ->realloc = ft_mem_debug_realloc;
        حافظہ->آزاد    = ft_mem_debug_free;

        p = ft_getenv( "FT2_ALLOC_TOTAL_MAX" );
        اگر ( p )
        {
          FT_Long  کل_max = ft_strtol( p, تہی, 10 );


          اگر ( کل_max > 0 )
          {
            جدول->bound_کل     = 1;
            جدول->alloc_کل_max = کل_max;
          }
        }

        p = ft_getenv( "FT2_ALLOC_COUNT_MAX" );
        اگر ( p )
        {
          FT_Long  کل_گنتی = ft_strtol( p, تہی, 10 );


          اگر ( کل_گنتی > 0 )
          {
            جدول->bound_گنتی     = 1;
            جدول->alloc_گنتی_max = کل_گنتی;
          }
        }

        p = ft_getenv( "FT2_KEEP_ALIVE" );
        اگر ( p )
        {
          FT_Long  keep_alive = ft_strtol( p, تہی, 10 );


          اگر ( keep_alive > 0 )
            جدول->keep_alive = 1;
        }
      }
      ورنہ
        حافظہ->آزاد( حافظہ, جدول );
    }
  }


  خارجی خالی
  ft_mem_debug_مکمل( FT_حافظہ  حافظہ )
  {
    اگر ( حافظہ->آزاد == ft_mem_debug_free )
    {
      FT_Mem_جدول  جدول = (FT_Mem_جدول)حافظہ->user;


      FT_Dump_حافظہ( حافظہ );

      ft_mem_جدول_destroy( جدول );

      حافظہ->آزاد    = جدول->آزاد;
      حافظہ->realloc = جدول->realloc;
      حافظہ->alloc   = جدول->alloc;
      حافظہ->user    = جدول->حافظہ_user;

      حافظہ->آزاد( حافظہ, جدول );
    }
  }


  FT_COMPARE_DEF( صحیح )
  ft_mem_ماخذ_compare( ثابت خالی*  p1,
                         ثابت خالی*  p2 )
  {
    FT_Mem_ماخذ  s1 = *(FT_Mem_ماخذ*)p1;
    FT_Mem_ماخذ  s2 = *(FT_Mem_ماخذ*)p2;


    اگر ( s2->max_سائز > s1->max_سائز )
      واپس 1;
    ورنہ اگر ( s2->max_سائز < s1->max_سائز )
      واپس -1;
    ورنہ
      واپس 0;
  }


  خارجی خالی
  FT_Dump_حافظہ( FT_حافظہ  حافظہ )
  {
    اگر ( حافظہ->آزاد == ft_mem_debug_free )
    {
      FT_Mem_جدول    جدول = (FT_Mem_جدول)حافظہ->user;
      FT_Mem_ماخذ*  بالٹی = جدول->sources;
      FT_Mem_ماخذ*  حد_بندی  = بالٹی + FT_MEM_ماخذ_BUCKETS;
      FT_Mem_ماخذ*  sources;
      FT_Int         nn, گنتی;
      ثابت حرف*    fmt;


      گنتی = 0;
      برائے ( ; بالٹی < حد_بندی; بالٹی++ )
      {
        FT_Mem_ماخذ  ماخذ = *بالٹی;


        برائے ( ; ماخذ; ماخذ = ماخذ->link )
          گنتی++;
      }

      sources = (FT_Mem_ماخذ*)
                  ft_mem_جدول_alloc(
                    جدول, گنتی * (FT_Long)حجم ( *sources ) );

      گنتی = 0;
      برائے ( بالٹی = جدول->sources; بالٹی < حد_بندی; بالٹی++ )
      {
        FT_Mem_ماخذ  ماخذ = *بالٹی;


        برائے ( ; ماخذ; ماخذ = ماخذ->link )
          sources[گنتی++] = ماخذ;
      }

      ft_qsort( sources,
                (size_t)گنتی,
                حجم ( *sources ),
                ft_mem_ماخذ_compare );

      لکھو( "FreeType Memory Dump: "
              "current=%ld max=%ld total=%ld count=%ld\n",
              جدول->alloc_current, جدول->alloc_max,
              جدول->alloc_کل, جدول->alloc_گنتی );
      لکھو( " block  block    sizes    sizes    sizes   source\n" );
      لکھو( " count   high      sum  highsum      max   location\n" );
      لکھو( "-------------------------------------------------\n" );

      fmt = "%6ld %6ld %8ld %8ld %8ld %s:%d\n";

      برائے ( nn = 0; nn < گنتی; nn++ )
      {
        FT_Mem_ماخذ  ماخذ = sources[nn];


        لکھو( fmt,
                ماخذ->cur_بلاکیں, ماخذ->max_بلاکیں,
                ماخذ->cur_سائز, ماخذ->max_سائز, ماخذ->cur_max,
                FT_FILENAME( ماخذ->file_name ),
                ماخذ->سطر_no );
      }
      لکھو( "------------------------------------------------\n" );

      ft_mem_جدول_free( جدول, sources );
    }
  }

#ورنہ  /* !FT_DEBUG_MEMORY */

  /* ANSI C doesn't like empty source files */
  قسمتعریف صحیح  debug_mem_dummy_;

#اگر_ختم /* !FT_DEBUG_MEMORY */


/*انتہا*/
