/****************************************************************************
 *
 * fthash.c
 *
 *   Hashing functions (body).
 *
 */

/*
 * Copyright 2000 Computing Research Labs, New Mexico State University
 * Copyright 2001-2015
 *   Francesco Zappa Nardelli
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COMPUTING RESEARCH LAB OR NEW MEXICO STATE UNIVERSITY BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT
 * OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR
 * THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

  /**************************************************************************
   *
   * This file is based on code from bdf.c,v 1.22 2000/03/16 20:08:50
   *
   * taken from Mark Leisher's xmbdfed package
   *
   */


#ضمن <freetype/internal/fthash.h>
#ضمن <freetype/internal/ftmemory.h>
#ضمن <freetype/internal/ftobjs.h>


#define INITIAL_HT_SIZE  241


  ساكن FT_ULong
  hash_str_lookup( FT_Hashkey*  مفتاح )
  {
    ثابت حرف*  kp  = مفتاح->str;
    FT_ULong     res = 0;


    /* Mocklisp hash function. */
    بينما ( *kp )
      res = ( res << 5 ) - res + (FT_ULong)*kp++;

    إرجاع res;
  }


  ساكن FT_ULong
  hash_رقم_lookup( FT_Hashkey*  مفتاح )
  {
    FT_ULong  رقم = (FT_ULong)مفتاح->رقم;
    FT_ULong  res;


    /* Mocklisp hash function. */
    res = رقم & 0xFF;
    res = ( res << 5 ) - res + ( ( رقم >>  8 ) & 0xFF );
    res = ( res << 5 ) - res + ( ( رقم >> 16 ) & 0xFF );
    res = ( res << 5 ) - res + ( ( رقم >> 24 ) & 0xFF );

    إرجاع res;
  }


  ساكن FT_Bool
  hash_str_compare( FT_Hashkey*  a,
                    FT_Hashkey*  b )
  {
    إذا ( a->str[0] == b->str[0]           &&
         ft_strcmp( a->str, b->str ) == 0 )
      إرجاع 1;

    إرجاع 0;
  }


  ساكن FT_Bool
  hash_رقم_compare( FT_Hashkey*  a,
                    FT_Hashkey*  b )
  {
    إذا ( a->رقم == b->رقم )
      إرجاع 1;

    إرجاع 0;
  }


  ساكن FT_Hashnode*
  hash_bucket( FT_Hashkey  مفتاح,
               FT_Hash     hash )
  {
    FT_ULong      res = 0;
    FT_Hashnode*  bp  = hash->table;
    FT_Hashnode*  ndp;


    res = (hash->lookup)( &مفتاح );

    ndp = bp + ( res % hash->size );
    بينما ( *ndp )
    {
      إذا ( (hash->compare)( &(*ndp)->مفتاح, &مفتاح ) )
        اكسر;

      ndp--;
      إذا ( ndp < bp )
        ndp = bp + ( hash->size - 1 );
    }

    إرجاع ndp;
  }


  ساكن FT_Error
  hash_rehash( FT_Hash    hash,
               FT_Memory  memory )
  {
    FT_Hashnode*  obp = hash->table;
    FT_Hashnode*  bp;
    FT_Hashnode*  nbp;

    FT_UInt   i, sz = hash->size;
    FT_Error  error = FT_Err_Ok;


    hash->size <<= 1;
    hash->limit  = hash->size / 3;

    إذا ( FT_NEW_ARRAY( hash->table, hash->size ) )
      اذهب Exit;

    لكل ( i = 0, bp = obp; i < sz; i++, bp++ )
    {
      إذا ( *bp )
      {
        nbp = hash_bucket( (*bp)->مفتاح, hash );
        *nbp = *bp;
      }
    }

    FT_FREE( obp );

  Exit:
    إرجاع error;
  }


  ساكن FT_Error
  hash_init( FT_Hash    hash,
             FT_Bool    is_رقم,
             FT_Memory  memory )
  {
    FT_UInt   sz = INITIAL_HT_SIZE;
    FT_Error  error;


    hash->size  = sz;
    hash->limit = sz / 3;
    hash->used  = 0;

    إذا ( is_رقم )
    {
      hash->lookup  = hash_رقم_lookup;
      hash->compare = hash_رقم_compare;
    }
    وإلا
    {
      hash->lookup  = hash_str_lookup;
      hash->compare = hash_str_compare;
    }

    FT_MEM_NEW_ARRAY( hash->table, sz );

    إرجاع error;
  }


  FT_Error
  ft_hash_str_init( FT_Hash    hash,
                    FT_Memory  memory )
  {
    إرجاع hash_init( hash, 0, memory );
  }


  FT_Error
  ft_hash_رقم_init( FT_Hash    hash,
                    FT_Memory  memory )
  {
    إرجاع hash_init( hash, 1, memory );
  }


  فراغ
  ft_hash_str_free( FT_Hash    hash,
                    FT_Memory  memory )
  {
    إذا ( hash )
    {
      FT_UInt       sz = hash->size;
      FT_Hashnode*  bp = hash->table;
      FT_UInt       i;


      لكل ( i = 0; i < sz; i++, bp++ )
        FT_FREE( *bp );

      FT_FREE( hash->table );
    }
  }


  /* `ft_hash_num_free' is the same as `ft_hash_str_free' */


  ساكن FT_Error
  hash_أدرج( FT_Hashkey  مفتاح,
               size_t      data,
               FT_Hash     hash,
               FT_Memory   memory,
               FT_Bool     overwrite )
  {
    FT_Hashnode   nn;
    FT_Hashnode*  bp    = hash_bucket( مفتاح, hash );
    FT_Error      error = FT_Err_Ok;


    nn = *bp;
    إذا ( !nn )
    {
      إذا ( FT_QNEW( nn ) )
        اذهب Exit;
      *bp = nn;

      nn->مفتاح  = مفتاح;
      nn->data = data;

      إذا ( hash->used >= hash->limit )
      {
        error = hash_rehash( hash, memory );
        إذا ( error )
          اذهب Exit;
      }

      hash->used++;
    }
    وإلا إذا ( overwrite )
      nn->data = data;

  Exit:
    إرجاع error;
  }


  FT_Error
  ft_hash_str_أدرج( ثابت حرف*  مفتاح,
                      size_t       data,
                      FT_Hash      hash,
                      FT_Memory    memory )
  {
    FT_Hashkey  hk;


    hk.str = مفتاح;

    إرجاع hash_أدرج( hk, data, hash, memory, TRUE );
  }


  FT_Error
  ft_hash_رقم_أدرج( FT_Int     رقم,
                      size_t     data,
                      FT_Hash    hash,
                      FT_Memory  memory )
  {
    FT_Hashkey  hk;


    hk.رقم = رقم;

    إرجاع hash_أدرج( hk, data, hash, memory, TRUE );
  }


  FT_Error
  ft_hash_str_أدرج_no_overwrite( ثابت حرف*  مفتاح,
                                   size_t       data,
                                   FT_Hash      hash,
                                   FT_Memory    memory )
  {
    FT_Hashkey  hk;


    hk.str = مفتاح;

    إرجاع hash_أدرج( hk, data, hash, memory, FALSE );
  }


  FT_Error
  ft_hash_رقم_أدرج_no_overwrite( FT_Int     رقم,
                                   size_t     data,
                                   FT_Hash    hash,
                                   FT_Memory  memory )
  {
    FT_Hashkey  hk;


    hk.رقم = رقم;

    إرجاع hash_أدرج( hk, data, hash, memory, FALSE );
  }


  ساكن size_t*
  hash_lookup( FT_Hashkey  مفتاح,
               FT_Hash     hash )
  {
    FT_Hashnode*  np = hash_bucket( مفتاح, hash );


    إرجاع (*np) ? &(*np)->data
                 : عدم;
  }


  size_t*
  ft_hash_str_lookup( ثابت حرف*  مفتاح,
                      FT_Hash      hash )
  {
    FT_Hashkey  hk;


    hk.str = مفتاح;

    إرجاع hash_lookup( hk, hash );
  }


  size_t*
  ft_hash_رقم_lookup( FT_Int   رقم,
                      FT_Hash  hash )
  {
    FT_Hashkey  hk;


    hk.رقم = رقم;

    إرجاع hash_lookup( hk, hash );
  }


  FT_Bool
  ft_hash_رقم_iterator( FT_UInt  *idx,
                        FT_Int   *مفتاح,
                        size_t   *قيمة,
                        FT_Hash   hash )
  {
    FT_Hashnode  nn = عدم;


    بينما ( 1 )
    {
      إذا ( *idx >= hash->size )
        إرجاع 0;

      nn = hash->table[*idx];
      إذا ( nn )
        اكسر;

      (*idx)++;
    }

    إذا ( مفتاح )
      *مفتاح = nn->مفتاح.رقم;
    إذا ( قيمة )
      *قيمة = nn->data;

    (*idx)++;

    إرجاع 1;
  }


  FT_Bool
  ft_hash_str_iterator( FT_UInt      *idx,
                        ثابت حرف*  *مفتاح,
                        size_t       *قيمة,
                        FT_Hash       hash )
  {
    FT_Hashnode  nn = عدم;


    بينما ( 1 )
    {
      إذا ( *idx >= hash->size )
        إرجاع 0;

      nn = hash->table[*idx];
      إذا ( nn )
        اكسر;

      (*idx)++;
    }

    إذا ( مفتاح )
      *مفتاح = nn->مفتاح.str;
    إذا ( قيمة )
      *قيمة = nn->data;

    (*idx)++;

    إرجاع 1;
  }


/* END */
