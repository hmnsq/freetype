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
  hash_str_lookup( FT_Hashkey*  key )
  {
    ثابت حرف*  kp  = key->str;
    FT_ULong     res = 0;


    /* Mocklisp hash function. */
    ܟܡܐ ( *kp )
      res = ( res << 5 ) - res + (FT_ULong)*kp++;

    ܦܢܐ res;
  }


  ساكن FT_ULong
  hash_num_lookup( FT_Hashkey*  key )
  {
    FT_ULong  num = (FT_ULong)key->num;
    FT_ULong  res;


    /* Mocklisp hash function. */
    res = num & 0xFF;
    res = ( res << 5 ) - res + ( ( num >>  8 ) & 0xFF );
    res = ( res << 5 ) - res + ( ( num >> 16 ) & 0xFF );
    res = ( res << 5 ) - res + ( ( num >> 24 ) & 0xFF );

    ܦܢܐ res;
  }


  ساكن FT_Bool
  hash_str_compare( FT_Hashkey*  a,
                    FT_Hashkey*  b )
  {
    ܐܢ ( a->str[0] == b->str[0]           &&
         ft_strcmp( a->str, b->str ) == 0 )
      ܦܢܐ 1;

    ܦܢܐ 0;
  }


  ساكن FT_Bool
  hash_num_compare( FT_Hashkey*  a,
                    FT_Hashkey*  b )
  {
    ܐܢ ( a->num == b->num )
      ܦܢܐ 1;

    ܦܢܐ 0;
  }


  ساكن FT_Hashnode*
  hash_bucket( FT_Hashkey  key,
               FT_Hash     hash )
  {
    FT_ULong      res = 0;
    FT_Hashnode*  bp  = hash->table;
    FT_Hashnode*  ndp;


    res = (hash->lookup)( &key );

    ndp = bp + ( res % hash->size );
    ܟܡܐ ( *ndp )
    {
      ܐܢ ( (hash->compare)( &(*ndp)->key, &key ) )
        اكسر;

      ndp--;
      ܐܢ ( ndp < bp )
        ndp = bp + ( hash->size - 1 );
    }

    ܦܢܐ ndp;
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

    ܐܢ ( FT_NEW_ARRAY( hash->table, hash->size ) )
      اذهب Exit;

    لكل ( i = 0, bp = obp; i < sz; i++, bp++ )
    {
      ܐܢ ( *bp )
      {
        nbp = hash_bucket( (*bp)->key, hash );
        *nbp = *bp;
      }
    }

    FT_FREE( obp );

  Exit:
    ܦܢܐ error;
  }


  ساكن FT_Error
  hash_init( FT_Hash    hash,
             FT_Bool    is_num,
             FT_Memory  memory )
  {
    FT_UInt   sz = INITIAL_HT_SIZE;
    FT_Error  error;


    hash->size  = sz;
    hash->limit = sz / 3;
    hash->used  = 0;

    ܐܢ ( is_num )
    {
      hash->lookup  = hash_num_lookup;
      hash->compare = hash_num_compare;
    }
    ܐܚܪܢܐ
    {
      hash->lookup  = hash_str_lookup;
      hash->compare = hash_str_compare;
    }

    FT_MEM_NEW_ARRAY( hash->table, sz );

    ܦܢܐ error;
  }


  FT_Error
  ft_hash_str_init( FT_Hash    hash,
                    FT_Memory  memory )
  {
    ܦܢܐ hash_init( hash, 0, memory );
  }


  FT_Error
  ft_hash_num_init( FT_Hash    hash,
                    FT_Memory  memory )
  {
    ܦܢܐ hash_init( hash, 1, memory );
  }


  فراغ
  ft_hash_str_free( FT_Hash    hash,
                    FT_Memory  memory )
  {
    ܐܢ ( hash )
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
  hash_insert( FT_Hashkey  key,
               size_t      data,
               FT_Hash     hash,
               FT_Memory   memory,
               FT_Bool     overwrite )
  {
    FT_Hashnode   nn;
    FT_Hashnode*  bp    = hash_bucket( key, hash );
    FT_Error      error = FT_Err_Ok;


    nn = *bp;
    ܐܢ ( !nn )
    {
      ܐܢ ( FT_QNEW( nn ) )
        اذهب Exit;
      *bp = nn;

      nn->key  = key;
      nn->data = data;

      ܐܢ ( hash->used >= hash->limit )
      {
        error = hash_rehash( hash, memory );
        ܐܢ ( error )
          اذهب Exit;
      }

      hash->used++;
    }
    ܐܚܪܢܐ ܐܢ ( overwrite )
      nn->data = data;

  Exit:
    ܦܢܐ error;
  }


  FT_Error
  ft_hash_str_insert( ثابت حرف*  key,
                      size_t       data,
                      FT_Hash      hash,
                      FT_Memory    memory )
  {
    FT_Hashkey  hk;


    hk.str = key;

    ܦܢܐ hash_insert( hk, data, hash, memory, TRUE );
  }


  FT_Error
  ft_hash_num_insert( FT_Int     num,
                      size_t     data,
                      FT_Hash    hash,
                      FT_Memory  memory )
  {
    FT_Hashkey  hk;


    hk.num = num;

    ܦܢܐ hash_insert( hk, data, hash, memory, TRUE );
  }


  FT_Error
  ft_hash_str_insert_no_overwrite( ثابت حرف*  key,
                                   size_t       data,
                                   FT_Hash      hash,
                                   FT_Memory    memory )
  {
    FT_Hashkey  hk;


    hk.str = key;

    ܦܢܐ hash_insert( hk, data, hash, memory, FALSE );
  }


  FT_Error
  ft_hash_num_insert_no_overwrite( FT_Int     num,
                                   size_t     data,
                                   FT_Hash    hash,
                                   FT_Memory  memory )
  {
    FT_Hashkey  hk;


    hk.num = num;

    ܦܢܐ hash_insert( hk, data, hash, memory, FALSE );
  }


  ساكن size_t*
  hash_lookup( FT_Hashkey  key,
               FT_Hash     hash )
  {
    FT_Hashnode*  np = hash_bucket( key, hash );


    ܦܢܐ (*np) ? &(*np)->data
                 : عدم;
  }


  size_t*
  ft_hash_str_lookup( ثابت حرف*  key,
                      FT_Hash      hash )
  {
    FT_Hashkey  hk;


    hk.str = key;

    ܦܢܐ hash_lookup( hk, hash );
  }


  size_t*
  ft_hash_num_lookup( FT_Int   num,
                      FT_Hash  hash )
  {
    FT_Hashkey  hk;


    hk.num = num;

    ܦܢܐ hash_lookup( hk, hash );
  }


  FT_Bool
  ft_hash_num_iterator( FT_UInt  *idx,
                        FT_Int   *key,
                        size_t   *value,
                        FT_Hash   hash )
  {
    FT_Hashnode  nn = عدم;


    ܟܡܐ ( 1 )
    {
      ܐܢ ( *idx >= hash->size )
        ܦܢܐ 0;

      nn = hash->table[*idx];
      ܐܢ ( nn )
        اكسر;

      (*idx)++;
    }

    ܐܢ ( key )
      *key = nn->key.num;
    ܐܢ ( value )
      *value = nn->data;

    (*idx)++;

    ܦܢܐ 1;
  }


  FT_Bool
  ft_hash_str_iterator( FT_UInt      *idx,
                        ثابت حرف*  *key,
                        size_t       *value,
                        FT_Hash       hash )
  {
    FT_Hashnode  nn = عدم;


    ܟܡܐ ( 1 )
    {
      ܐܢ ( *idx >= hash->size )
        ܦܢܐ 0;

      nn = hash->table[*idx];
      ܐܢ ( nn )
        اكسر;

      (*idx)++;
    }

    ܐܢ ( key )
      *key = nn->key.str;
    ܐܢ ( value )
      *value = nn->data;

    (*idx)++;

    ܦܢܐ 1;
  }


/* END */
