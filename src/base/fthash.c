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
  hash_str_lookup( FT_Hashkey*  مبودي )
  {
    ثابت حرف*  kp  = مبودي->str;
    FT_ULong     res = 0;


    /* Mocklisp hash function. */
    طالما ( *kp )
      res = ( res << 5 ) - res + (FT_ULong)*kp++;

    أرجع res;
  }


  ساكن FT_ULong
  hash_لمب_lookup( FT_Hashkey*  مبودي )
  {
    FT_ULong  لمب = (FT_ULong)مبودي->لمب;
    FT_ULong  res;


    /* Mocklisp hash function. */
    res = لمب & 0xFF;
    res = ( res << 5 ) - res + ( ( لمب >>  8 ) & 0xFF );
    res = ( res << 5 ) - res + ( ( لمب >> 16 ) & 0xFF );
    res = ( res << 5 ) - res + ( ( لمب >> 24 ) & 0xFF );

    أرجع res;
  }


  ساكن FT_Bool
  hash_str_compare( FT_Hashkey*  a,
                    FT_Hashkey*  b )
  {
    إذا ( a->str[0] == b->str[0]           &&
         ft_strcmp( a->str, b->str ) == 0 )
      أرجع 1;

    أرجع 0;
  }


  ساكن FT_Bool
  hash_لمب_compare( FT_Hashkey*  a,
                    FT_Hashkey*  b )
  {
    إذا ( a->لمب == b->لمب )
      أرجع 1;

    أرجع 0;
  }


  ساكن FT_Hashnode*
  hash_bucket( FT_Hashkey  مبودي,
               FT_Hash     hash )
  {
    FT_ULong      res = 0;
    FT_Hashnode*  bp  = hash->table;
    FT_Hashnode*  ndp;


    res = (hash->lookup)( &مبودي );

    ndp = bp + ( res % hash->size );
    طالما ( *ndp )
    {
      إذا ( (hash->compare)( &(*ndp)->مبودي, &مبودي ) )
        اكسر;

      ndp--;
      إذا ( ndp < bp )
        ndp = bp + ( hash->size - 1 );
    }

    أرجع ndp;
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
        nbp = hash_bucket( (*bp)->مبودي, hash );
        *nbp = *bp;
      }
    }

    FT_FREE( obp );

  Exit:
    أرجع error;
  }


  ساكن FT_Error
  hash_init( FT_Hash    hash,
             FT_Bool    is_لمب,
             FT_Memory  memory )
  {
    FT_UInt   sz = INITIAL_HT_SIZE;
    FT_Error  error;


    hash->size  = sz;
    hash->limit = sz / 3;
    hash->used  = 0;

    إذا ( is_لمب )
    {
      hash->lookup  = hash_لمب_lookup;
      hash->compare = hash_لمب_compare;
    }
    وإلا
    {
      hash->lookup  = hash_str_lookup;
      hash->compare = hash_str_compare;
    }

    FT_MEM_NEW_ARRAY( hash->table, sz );

    أرجع error;
  }


  FT_Error
  ft_hash_str_init( FT_Hash    hash,
                    FT_Memory  memory )
  {
    أرجع hash_init( hash, 0, memory );
  }


  FT_Error
  ft_hash_لمب_init( FT_Hash    hash,
                    FT_Memory  memory )
  {
    أرجع hash_init( hash, 1, memory );
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
  hash_سك( FT_Hashkey  مبودي,
               size_t      data,
               FT_Hash     hash,
               FT_Memory   memory,
               FT_Bool     overwrite )
  {
    FT_Hashnode   nn;
    FT_Hashnode*  bp    = hash_bucket( مبودي, hash );
    FT_Error      error = FT_Err_Ok;


    nn = *bp;
    إذا ( !nn )
    {
      إذا ( FT_QNEW( nn ) )
        اذهب Exit;
      *bp = nn;

      nn->مبودي  = مبودي;
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
    أرجع error;
  }


  FT_Error
  ft_hash_str_سك( ثابت حرف*  مبودي,
                      size_t       data,
                      FT_Hash      hash,
                      FT_Memory    memory )
  {
    FT_Hashkey  hk;


    hk.str = مبودي;

    أرجع hash_سك( hk, data, hash, memory, TRUE );
  }


  FT_Error
  ft_hash_لمب_سك( FT_Int     لمب,
                      size_t     data,
                      FT_Hash    hash,
                      FT_Memory  memory )
  {
    FT_Hashkey  hk;


    hk.لمب = لمب;

    أرجع hash_سك( hk, data, hash, memory, TRUE );
  }


  FT_Error
  ft_hash_str_سك_no_overwrite( ثابت حرف*  مبودي,
                                   size_t       data,
                                   FT_Hash      hash,
                                   FT_Memory    memory )
  {
    FT_Hashkey  hk;


    hk.str = مبودي;

    أرجع hash_سك( hk, data, hash, memory, FALSE );
  }


  FT_Error
  ft_hash_لمب_سك_no_overwrite( FT_Int     لمب,
                                   size_t     data,
                                   FT_Hash    hash,
                                   FT_Memory  memory )
  {
    FT_Hashkey  hk;


    hk.لمب = لمب;

    أرجع hash_سك( hk, data, hash, memory, FALSE );
  }


  ساكن size_t*
  hash_lookup( FT_Hashkey  مبودي,
               FT_Hash     hash )
  {
    FT_Hashnode*  np = hash_bucket( مبودي, hash );


    أرجع (*np) ? &(*np)->data
                 : عدم;
  }


  size_t*
  ft_hash_str_lookup( ثابت حرف*  مبودي,
                      FT_Hash      hash )
  {
    FT_Hashkey  hk;


    hk.str = مبودي;

    أرجع hash_lookup( hk, hash );
  }


  size_t*
  ft_hash_لمب_lookup( FT_Int   لمب,
                      FT_Hash  hash )
  {
    FT_Hashkey  hk;


    hk.لمب = لمب;

    أرجع hash_lookup( hk, hash );
  }


  FT_Bool
  ft_hash_لمب_iterator( FT_UInt  *idx,
                        FT_Int   *مبودي,
                        size_t   *درج,
                        FT_Hash   hash )
  {
    FT_Hashnode  nn = عدم;


    طالما ( 1 )
    {
      إذا ( *idx >= hash->size )
        أرجع 0;

      nn = hash->table[*idx];
      إذا ( nn )
        اكسر;

      (*idx)++;
    }

    إذا ( مبودي )
      *مبودي = nn->مبودي.لمب;
    إذا ( درج )
      *درج = nn->data;

    (*idx)++;

    أرجع 1;
  }


  FT_Bool
  ft_hash_str_iterator( FT_UInt      *idx,
                        ثابت حرف*  *مبودي,
                        size_t       *درج,
                        FT_Hash       hash )
  {
    FT_Hashnode  nn = عدم;


    طالما ( 1 )
    {
      إذا ( *idx >= hash->size )
        أرجع 0;

      nn = hash->table[*idx];
      إذا ( nn )
        اكسر;

      (*idx)++;
    }

    إذا ( مبودي )
      *مبودي = nn->مبودي.str;
    إذا ( درج )
      *درج = nn->data;

    (*idx)++;

    أرجع 1;
  }


/* END */
