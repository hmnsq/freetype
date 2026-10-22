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


#شامل <freetype/internal/fthash.h>
#شامل <freetype/internal/ftmemory.h>
#شامل <freetype/internal/ftobjs.h>


#define INITIAL_HT_SIZE  241


  ایستا FT_ULong
  hash_str_lookup( FT_Hashkey*  کلید )
  {
    ثابت نویسه*  kp  = کلید->str;
    FT_ULong     res = 0;


    /* Mocklisp hash function. */
    وقتی ( *kp )
      res = ( res << 5 ) - res + (FT_ULong)*kp++;

    بازگشت res;
  }


  ایستا FT_ULong
  hash_رقم_lookup( FT_Hashkey*  کلید )
  {
    FT_ULong  رقم = (FT_ULong)کلید->رقم;
    FT_ULong  res;


    /* Mocklisp hash function. */
    res = رقم & 0xFF;
    res = ( res << 5 ) - res + ( ( رقم >>  8 ) & 0xFF );
    res = ( res << 5 ) - res + ( ( رقم >> 16 ) & 0xFF );
    res = ( res << 5 ) - res + ( ( رقم >> 24 ) & 0xFF );

    بازگشت res;
  }


  ایستا FT_Bool
  hash_str_compare( FT_Hashkey*  a,
                    FT_Hashkey*  b )
  {
    اگر ( a->str[0] == b->str[0]           &&
         ft_strcmp( a->str, b->str ) == 0 )
      بازگشت 1;

    بازگشت 0;
  }


  ایستا FT_Bool
  hash_رقم_compare( FT_Hashkey*  a,
                    FT_Hashkey*  b )
  {
    اگر ( a->رقم == b->رقم )
      بازگشت 1;

    بازگشت 0;
  }


  ایستا FT_Hashnode*
  hash_bucket( FT_Hashkey  کلید,
               FT_Hash     hash )
  {
    FT_ULong      res = 0;
    FT_Hashnode*  bp  = hash->table;
    FT_Hashnode*  ndp;


    res = (hash->lookup)( &کلید );

    ndp = bp + ( res % hash->size );
    وقتی ( *ndp )
    {
      اگر ( (hash->compare)( &(*ndp)->کلید, &کلید ) )
        بشکن;

      ndp--;
      اگر ( ndp < bp )
        ndp = bp + ( hash->size - 1 );
    }

    بازگشت ndp;
  }


  ایستا FT_Error
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

    اگر ( FT_NEW_ARRAY( hash->table, hash->size ) )
      برو Exit;

    برای ( i = 0, bp = obp; i < sz; i++, bp++ )
    {
      اگر ( *bp )
      {
        nbp = hash_bucket( (*bp)->کلید, hash );
        *nbp = *bp;
      }
    }

    FT_FREE( obp );

  Exit:
    بازگشت error;
  }


  ایستا FT_Error
  hash_init( FT_Hash    hash,
             FT_Bool    is_رقم,
             FT_Memory  memory )
  {
    FT_UInt   sz = INITIAL_HT_SIZE;
    FT_Error  error;


    hash->size  = sz;
    hash->limit = sz / 3;
    hash->used  = 0;

    اگر ( is_رقم )
    {
      hash->lookup  = hash_رقم_lookup;
      hash->compare = hash_رقم_compare;
    }
    وگرنه
    {
      hash->lookup  = hash_str_lookup;
      hash->compare = hash_str_compare;
    }

    FT_MEM_NEW_ARRAY( hash->table, sz );

    بازگشت error;
  }


  FT_Error
  ft_hash_str_init( FT_Hash    hash,
                    FT_Memory  memory )
  {
    بازگشت hash_init( hash, 0, memory );
  }


  FT_Error
  ft_hash_رقم_init( FT_Hash    hash,
                    FT_Memory  memory )
  {
    بازگشت hash_init( hash, 1, memory );
  }


  تهی
  ft_hash_str_free( FT_Hash    hash,
                    FT_Memory  memory )
  {
    اگر ( hash )
    {
      FT_UInt       sz = hash->size;
      FT_Hashnode*  bp = hash->table;
      FT_UInt       i;


      برای ( i = 0; i < sz; i++, bp++ )
        FT_FREE( *bp );

      FT_FREE( hash->table );
    }
  }


  /* `ft_hash_num_free' is the same as `ft_hash_str_free' */


  ایستا FT_Error
  hash_درج( FT_Hashkey  کلید,
               size_t      data,
               FT_Hash     hash,
               FT_Memory   memory,
               FT_Bool     overwrite )
  {
    FT_Hashnode   nn;
    FT_Hashnode*  bp    = hash_bucket( کلید, hash );
    FT_Error      error = FT_Err_Ok;


    nn = *bp;
    اگر ( !nn )
    {
      اگر ( FT_QNEW( nn ) )
        برو Exit;
      *bp = nn;

      nn->کلید  = کلید;
      nn->data = data;

      اگر ( hash->used >= hash->limit )
      {
        error = hash_rehash( hash, memory );
        اگر ( error )
          برو Exit;
      }

      hash->used++;
    }
    وگرنه اگر ( overwrite )
      nn->data = data;

  Exit:
    بازگشت error;
  }


  FT_Error
  ft_hash_str_درج( ثابت نویسه*  کلید,
                      size_t       data,
                      FT_Hash      hash,
                      FT_Memory    memory )
  {
    FT_Hashkey  hk;


    hk.str = کلید;

    بازگشت hash_درج( hk, data, hash, memory, TRUE );
  }


  FT_Error
  ft_hash_رقم_درج( FT_Int     رقم,
                      size_t     data,
                      FT_Hash    hash,
                      FT_Memory  memory )
  {
    FT_Hashkey  hk;


    hk.رقم = رقم;

    بازگشت hash_درج( hk, data, hash, memory, TRUE );
  }


  FT_Error
  ft_hash_str_درج_no_overwrite( ثابت نویسه*  کلید,
                                   size_t       data,
                                   FT_Hash      hash,
                                   FT_Memory    memory )
  {
    FT_Hashkey  hk;


    hk.str = کلید;

    بازگشت hash_درج( hk, data, hash, memory, FALSE );
  }


  FT_Error
  ft_hash_رقم_درج_no_overwrite( FT_Int     رقم,
                                   size_t     data,
                                   FT_Hash    hash,
                                   FT_Memory  memory )
  {
    FT_Hashkey  hk;


    hk.رقم = رقم;

    بازگشت hash_درج( hk, data, hash, memory, FALSE );
  }


  ایستا size_t*
  hash_lookup( FT_Hashkey  کلید,
               FT_Hash     hash )
  {
    FT_Hashnode*  np = hash_bucket( کلید, hash );


    بازگشت (*np) ? &(*np)->data
                 : پوچ;
  }


  size_t*
  ft_hash_str_lookup( ثابت نویسه*  کلید,
                      FT_Hash      hash )
  {
    FT_Hashkey  hk;


    hk.str = کلید;

    بازگشت hash_lookup( hk, hash );
  }


  size_t*
  ft_hash_رقم_lookup( FT_Int   رقم,
                      FT_Hash  hash )
  {
    FT_Hashkey  hk;


    hk.رقم = رقم;

    بازگشت hash_lookup( hk, hash );
  }


  FT_Bool
  ft_hash_رقم_iterator( FT_UInt  *idx,
                        FT_Int   *کلید,
                        size_t   *مقدار,
                        FT_Hash   hash )
  {
    FT_Hashnode  nn = پوچ;


    وقتی ( 1 )
    {
      اگر ( *idx >= hash->size )
        بازگشت 0;

      nn = hash->table[*idx];
      اگر ( nn )
        بشکن;

      (*idx)++;
    }

    اگر ( کلید )
      *کلید = nn->کلید.رقم;
    اگر ( مقدار )
      *مقدار = nn->data;

    (*idx)++;

    بازگشت 1;
  }


  FT_Bool
  ft_hash_str_iterator( FT_UInt      *idx,
                        ثابت نویسه*  *کلید,
                        size_t       *مقدار,
                        FT_Hash       hash )
  {
    FT_Hashnode  nn = پوچ;


    وقتی ( 1 )
    {
      اگر ( *idx >= hash->size )
        بازگشت 0;

      nn = hash->table[*idx];
      اگر ( nn )
        بشکن;

      (*idx)++;
    }

    اگر ( کلید )
      *کلید = nn->کلید.str;
    اگر ( مقدار )
      *مقدار = nn->data;

    (*idx)++;

    بازگشت 1;
  }


/* END */
