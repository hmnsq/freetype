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


#ಅನ್ತರ್ಭಾವ <freetype/internal/fthash.h>
#ಅನ್ತರ್ಭಾವ <freetype/internal/ftmemory.h>
#ಅನ್ತರ್ಭಾವ <freetype/internal/ftobjs.h>


#define INITIAL_HT_SIZE  241


  ಸ್ಥಿರ FT_ULong
  hash_str_lookup( FT_Hashkey*  ಕೀಲಿ )
  {
    ಧ್ರುವ ವರ್ಣ*  kp  = ಕೀಲಿ->str;
    FT_ULong     res = 0;


    /* Mocklisp hash function. */
    ಯಾವತ್ ( *kp )
      res = ( res << 5 ) - res + (FT_ULong)*kp++;

    ಪ್ರತ್ಯಾಗಮ res;
  }


  ಸ್ಥಿರ FT_ULong
  hash_ಅಂಕೆ_lookup( FT_Hashkey*  ಕೀಲಿ )
  {
    FT_ULong  ಅಂಕೆ = (FT_ULong)ಕೀಲಿ->ಅಂಕೆ;
    FT_ULong  res;


    /* Mocklisp hash function. */
    res = ಅಂಕೆ & 0xFF;
    res = ( res << 5 ) - res + ( ( ಅಂಕೆ >>  8 ) & 0xFF );
    res = ( res << 5 ) - res + ( ( ಅಂಕೆ >> 16 ) & 0xFF );
    res = ( res << 5 ) - res + ( ( ಅಂಕೆ >> 24 ) & 0xFF );

    ಪ್ರತ್ಯಾಗಮ res;
  }


  ಸ್ಥಿರ FT_Bool
  hash_str_compare( FT_Hashkey*  a,
                    FT_Hashkey*  b )
  {
    ಯದಿ ( a->str[0] == b->str[0]           &&
         ft_strcmp( a->str, b->str ) == 0 )
      ಪ್ರತ್ಯಾಗಮ 1;

    ಪ್ರತ್ಯಾಗಮ 0;
  }


  ಸ್ಥಿರ FT_Bool
  hash_ಅಂಕೆ_compare( FT_Hashkey*  a,
                    FT_Hashkey*  b )
  {
    ಯದಿ ( a->ಅಂಕೆ == b->ಅಂಕೆ )
      ಪ್ರತ್ಯಾಗಮ 1;

    ಪ್ರತ್ಯಾಗಮ 0;
  }


  ಸ್ಥಿರ FT_Hashnode*
  hash_bucket( FT_Hashkey  ಕೀಲಿ,
               FT_Hash     hash )
  {
    FT_ULong      res = 0;
    FT_Hashnode*  bp  = hash->table;
    FT_Hashnode*  ndp;


    res = (hash->lookup)( &ಕೀಲಿ );

    ndp = bp + ( res % hash->size );
    ಯಾವತ್ ( *ndp )
    {
      ಯದಿ ( (hash->compare)( &(*ndp)->ಕೀಲಿ, &ಕೀಲಿ ) )
        ಭಙ್ಗ;

      ndp--;
      ಯದಿ ( ndp < bp )
        ndp = bp + ( hash->size - 1 );
    }

    ಪ್ರತ್ಯಾಗಮ ndp;
  }


  ಸ್ಥಿರ FT_Error
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

    ಯದಿ ( FT_NEW_ARRAY( hash->table, hash->size ) )
      ಗಚ್ಛ Exit;

    ಪ್ರತಿ ( i = 0, bp = obp; i < sz; i++, bp++ )
    {
      ಯದಿ ( *bp )
      {
        nbp = hash_bucket( (*bp)->ಕೀಲಿ, hash );
        *nbp = *bp;
      }
    }

    FT_FREE( obp );

  Exit:
    ಪ್ರತ್ಯಾಗಮ error;
  }


  ಸ್ಥಿರ FT_Error
  hash_init( FT_Hash    hash,
             FT_Bool    is_ಅಂಕೆ,
             FT_Memory  memory )
  {
    FT_UInt   sz = INITIAL_HT_SIZE;
    FT_Error  error;


    hash->size  = sz;
    hash->limit = sz / 3;
    hash->used  = 0;

    ಯದಿ ( is_ಅಂಕೆ )
    {
      hash->lookup  = hash_ಅಂಕೆ_lookup;
      hash->compare = hash_ಅಂಕೆ_compare;
    }
    ಅನ್ಯಥಾ
    {
      hash->lookup  = hash_str_lookup;
      hash->compare = hash_str_compare;
    }

    FT_MEM_NEW_ARRAY( hash->table, sz );

    ಪ್ರತ್ಯಾಗಮ error;
  }


  FT_Error
  ft_hash_str_init( FT_Hash    hash,
                    FT_Memory  memory )
  {
    ಪ್ರತ್ಯಾಗಮ hash_init( hash, 0, memory );
  }


  FT_Error
  ft_hash_ಅಂಕೆ_init( FT_Hash    hash,
                    FT_Memory  memory )
  {
    ಪ್ರತ್ಯಾಗಮ hash_init( hash, 1, memory );
  }


  ಶೂನ್ಯ
  ft_hash_str_free( FT_Hash    hash,
                    FT_Memory  memory )
  {
    ಯದಿ ( hash )
    {
      FT_UInt       sz = hash->size;
      FT_Hashnode*  bp = hash->table;
      FT_UInt       i;


      ಪ್ರತಿ ( i = 0; i < sz; i++, bp++ )
        FT_FREE( *bp );

      FT_FREE( hash->table );
    }
  }


  /* `ft_hash_num_free' is the same as `ft_hash_str_free' */


  ಸ್ಥಿರ FT_Error
  hash_ಸೇರಿಸು( FT_Hashkey  ಕೀಲಿ,
               size_t      data,
               FT_Hash     hash,
               FT_Memory   memory,
               FT_Bool     overwrite )
  {
    FT_Hashnode   nn;
    FT_Hashnode*  bp    = hash_bucket( ಕೀಲಿ, hash );
    FT_Error      error = FT_Err_Ok;


    nn = *bp;
    ಯದಿ ( !nn )
    {
      ಯದಿ ( FT_QNEW( nn ) )
        ಗಚ್ಛ Exit;
      *bp = nn;

      nn->ಕೀಲಿ  = ಕೀಲಿ;
      nn->data = data;

      ಯದಿ ( hash->used >= hash->limit )
      {
        error = hash_rehash( hash, memory );
        ಯದಿ ( error )
          ಗಚ್ಛ Exit;
      }

      hash->used++;
    }
    ಅನ್ಯಥಾ ಯದಿ ( overwrite )
      nn->data = data;

  Exit:
    ಪ್ರತ್ಯಾಗಮ error;
  }


  FT_Error
  ft_hash_str_ಸೇರಿಸು( ಧ್ರುವ ವರ್ಣ*  ಕೀಲಿ,
                      size_t       data,
                      FT_Hash      hash,
                      FT_Memory    memory )
  {
    FT_Hashkey  hk;


    hk.str = ಕೀಲಿ;

    ಪ್ರತ್ಯಾಗಮ hash_ಸೇರಿಸು( hk, data, hash, memory, TRUE );
  }


  FT_Error
  ft_hash_ಅಂಕೆ_ಸೇರಿಸು( FT_Int     ಅಂಕೆ,
                      size_t     data,
                      FT_Hash    hash,
                      FT_Memory  memory )
  {
    FT_Hashkey  hk;


    hk.ಅಂಕೆ = ಅಂಕೆ;

    ಪ್ರತ್ಯಾಗಮ hash_ಸೇರಿಸು( hk, data, hash, memory, TRUE );
  }


  FT_Error
  ft_hash_str_ಸೇರಿಸು_no_overwrite( ಧ್ರುವ ವರ್ಣ*  ಕೀಲಿ,
                                   size_t       data,
                                   FT_Hash      hash,
                                   FT_Memory    memory )
  {
    FT_Hashkey  hk;


    hk.str = ಕೀಲಿ;

    ಪ್ರತ್ಯಾಗಮ hash_ಸೇರಿಸು( hk, data, hash, memory, FALSE );
  }


  FT_Error
  ft_hash_ಅಂಕೆ_ಸೇರಿಸು_no_overwrite( FT_Int     ಅಂಕೆ,
                                   size_t     data,
                                   FT_Hash    hash,
                                   FT_Memory  memory )
  {
    FT_Hashkey  hk;


    hk.ಅಂಕೆ = ಅಂಕೆ;

    ಪ್ರತ್ಯಾಗಮ hash_ಸೇರಿಸು( hk, data, hash, memory, FALSE );
  }


  ಸ್ಥಿರ size_t*
  hash_lookup( FT_Hashkey  ಕೀಲಿ,
               FT_Hash     hash )
  {
    FT_Hashnode*  np = hash_bucket( ಕೀಲಿ, hash );


    ಪ್ರತ್ಯಾಗಮ (*np) ? &(*np)->data
                 : ರಿಕ್ತ;
  }


  size_t*
  ft_hash_str_lookup( ಧ್ರುವ ವರ್ಣ*  ಕೀಲಿ,
                      FT_Hash      hash )
  {
    FT_Hashkey  hk;


    hk.str = ಕೀಲಿ;

    ಪ್ರತ್ಯಾಗಮ hash_lookup( hk, hash );
  }


  size_t*
  ft_hash_ಅಂಕೆ_lookup( FT_Int   ಅಂಕೆ,
                      FT_Hash  hash )
  {
    FT_Hashkey  hk;


    hk.ಅಂಕೆ = ಅಂಕೆ;

    ಪ್ರತ್ಯಾಗಮ hash_lookup( hk, hash );
  }


  FT_Bool
  ft_hash_ಅಂಕೆ_iterator( FT_UInt  *idx,
                        FT_Int   *ಕೀಲಿ,
                        size_t   *ಮೌಲ್ಯ,
                        FT_Hash   hash )
  {
    FT_Hashnode  nn = ರಿಕ್ತ;


    ಯಾವತ್ ( 1 )
    {
      ಯದಿ ( *idx >= hash->size )
        ಪ್ರತ್ಯಾಗಮ 0;

      nn = hash->table[*idx];
      ಯದಿ ( nn )
        ಭಙ್ಗ;

      (*idx)++;
    }

    ಯದಿ ( ಕೀಲಿ )
      *ಕೀಲಿ = nn->ಕೀಲಿ.ಅಂಕೆ;
    ಯದಿ ( ಮೌಲ್ಯ )
      *ಮೌಲ್ಯ = nn->data;

    (*idx)++;

    ಪ್ರತ್ಯಾಗಮ 1;
  }


  FT_Bool
  ft_hash_str_iterator( FT_UInt      *idx,
                        ಧ್ರುವ ವರ್ಣ*  *ಕೀಲಿ,
                        size_t       *ಮೌಲ್ಯ,
                        FT_Hash       hash )
  {
    FT_Hashnode  nn = ರಿಕ್ತ;


    ಯಾವತ್ ( 1 )
    {
      ಯದಿ ( *idx >= hash->size )
        ಪ್ರತ್ಯಾಗಮ 0;

      nn = hash->table[*idx];
      ಯದಿ ( nn )
        ಭಙ್ಗ;

      (*idx)++;
    }

    ಯದಿ ( ಕೀಲಿ )
      *ಕೀಲಿ = nn->ಕೀಲಿ.str;
    ಯದಿ ( ಮೌಲ್ಯ )
      *ಮೌಲ್ಯ = nn->data;

    (*idx)++;

    ಪ್ರತ್ಯಾಗಮ 1;
  }


/* END */
