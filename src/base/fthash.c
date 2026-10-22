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


#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/fthash.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftmemory.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftobjs.h>


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ INITIAL_HT_ਮਾਪ  241


  ਸਥਿਰ FT_ULong
  hash_str_lookup( FT_Hashkey*  ਕੁੰਜੀ )
  {
    ਅਚਲ ਅੱਖਰ*  kp  = ਕੁੰਜੀ->str;
    FT_ULong     res = 0;


    /* Mocklisp hash function. */
    ਜਦੋਂ ( *kp )
      res = ( res << 5 ) - res + (FT_ULong)*kp++;

    ਵਾਪਸ res;
  }


  ਸਥਿਰ FT_ULong
  hash_ਅੰਕ_lookup( FT_Hashkey*  ਕੁੰਜੀ )
  {
    FT_ULong  ਅੰਕ = (FT_ULong)ਕੁੰਜੀ->ਅੰਕ;
    FT_ULong  res;


    /* Mocklisp hash function. */
    res = ਅੰਕ & 0xFF;
    res = ( res << 5 ) - res + ( ( ਅੰਕ >>  8 ) & 0xFF );
    res = ( res << 5 ) - res + ( ( ਅੰਕ >> 16 ) & 0xFF );
    res = ( res << 5 ) - res + ( ( ਅੰਕ >> 24 ) & 0xFF );

    ਵਾਪਸ res;
  }


  ਸਥਿਰ FT_Bool
  hash_str_compare( FT_Hashkey*  a,
                    FT_Hashkey*  b )
  {
    ਜੇ ( a->str[0] == b->str[0]           &&
         ft_strcmp( a->str, b->str ) == 0 )
      ਵਾਪਸ 1;

    ਵਾਪਸ 0;
  }


  ਸਥਿਰ FT_Bool
  hash_ਅੰਕ_compare( FT_Hashkey*  a,
                    FT_Hashkey*  b )
  {
    ਜੇ ( a->ਅੰਕ == b->ਅੰਕ )
      ਵਾਪਸ 1;

    ਵਾਪਸ 0;
  }


  ਸਥਿਰ FT_Hashnode*
  hash_ਬਾਲਟੀ( FT_Hashkey  ਕੁੰਜੀ,
               FT_Hash     hash )
  {
    FT_ULong      res = 0;
    FT_Hashnode*  bp  = hash->ਸਾਰਣੀ;
    FT_Hashnode*  ndp;


    res = (hash->lookup)( &ਕੁੰਜੀ );

    ndp = bp + ( res % hash->ਮਾਪ );
    ਜਦੋਂ ( *ndp )
    {
      ਜੇ ( (hash->compare)( &(*ndp)->ਕੁੰਜੀ, &ਕੁੰਜੀ ) )
        ਰੋਕੋ;

      ndp--;
      ਜੇ ( ndp < bp )
        ndp = bp + ( hash->ਮਾਪ - 1 );
    }

    ਵਾਪਸ ndp;
  }


  ਸਥਿਰ FT_Error
  hash_rehash( FT_Hash    hash,
               FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ )
  {
    FT_Hashnode*  obp = hash->ਸਾਰਣੀ;
    FT_Hashnode*  bp;
    FT_Hashnode*  nbp;

    FT_UInt   i, sz = hash->ਮਾਪ;
    FT_Error  error = FT_Err_Ok;


    hash->ਮਾਪ <<= 1;
    hash->ਹੱਦਬੰਦੀ  = hash->ਮਾਪ / 3;

    ਜੇ ( FT_NEW_ARRAY( hash->ਸਾਰਣੀ, hash->ਮਾਪ ) )
      ਜਾਓ Exit;

    ਲਈ ( i = 0, bp = obp; i < sz; i++, bp++ )
    {
      ਜੇ ( *bp )
      {
        nbp = hash_ਬਾਲਟੀ( (*bp)->ਕੁੰਜੀ, hash );
        *nbp = *bp;
      }
    }

    FT_FREE( obp );

  Exit:
    ਵਾਪਸ error;
  }


  ਸਥਿਰ FT_Error
  hash_init( FT_Hash    hash,
             FT_Bool    is_ਅੰਕ,
             FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ )
  {
    FT_UInt   sz = INITIAL_HT_ਮਾਪ;
    FT_Error  error;


    hash->ਮਾਪ  = sz;
    hash->ਹੱਦਬੰਦੀ = sz / 3;
    hash->ਵਰਤਿਆ  = 0;

    ਜੇ ( is_ਅੰਕ )
    {
      hash->lookup  = hash_ਅੰਕ_lookup;
      hash->compare = hash_ਅੰਕ_compare;
    }
    ਨਹੀਂਤਾਂ
    {
      hash->lookup  = hash_str_lookup;
      hash->compare = hash_str_compare;
    }

    FT_MEM_NEW_ARRAY( hash->ਸਾਰਣੀ, sz );

    ਵਾਪਸ error;
  }


  FT_Error
  ft_hash_str_init( FT_Hash    hash,
                    FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ )
  {
    ਵਾਪਸ hash_init( hash, 0, ਸਮ੍ਰਿਤੀ );
  }


  FT_Error
  ft_hash_ਅੰਕ_init( FT_Hash    hash,
                    FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ )
  {
    ਵਾਪਸ hash_init( hash, 1, ਸਮ੍ਰਿਤੀ );
  }


  ਖਾਲੀ
  ft_hash_str_free( FT_Hash    hash,
                    FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ )
  {
    ਜੇ ( hash )
    {
      FT_UInt       sz = hash->ਮਾਪ;
      FT_Hashnode*  bp = hash->ਸਾਰਣੀ;
      FT_UInt       i;


      ਲਈ ( i = 0; i < sz; i++, bp++ )
        FT_FREE( *bp );

      FT_FREE( hash->ਸਾਰਣੀ );
    }
  }


  /* `ft_hash_num_free' is the same as `ft_hash_str_free' */


  ਸਥਿਰ FT_Error
  hash_ਪਾਓ( FT_Hashkey  ਕੁੰਜੀ,
               size_t      ਅੰਕੜੇ,
               FT_Hash     hash,
               FT_ਸਮ੍ਰਿਤੀ   ਸਮ੍ਰਿਤੀ,
               FT_Bool     overwrite )
  {
    FT_Hashnode   nn;
    FT_Hashnode*  bp    = hash_ਬਾਲਟੀ( ਕੁੰਜੀ, hash );
    FT_Error      error = FT_Err_Ok;


    nn = *bp;
    ਜੇ ( !nn )
    {
      ਜੇ ( FT_QNEW( nn ) )
        ਜਾਓ Exit;
      *bp = nn;

      nn->ਕੁੰਜੀ  = ਕੁੰਜੀ;
      nn->ਅੰਕੜੇ = ਅੰਕੜੇ;

      ਜੇ ( hash->ਵਰਤਿਆ >= hash->ਹੱਦਬੰਦੀ )
      {
        error = hash_rehash( hash, ਸਮ੍ਰਿਤੀ );
        ਜੇ ( error )
          ਜਾਓ Exit;
      }

      hash->ਵਰਤਿਆ++;
    }
    ਨਹੀਂਤਾਂ ਜੇ ( overwrite )
      nn->ਅੰਕੜੇ = ਅੰਕੜੇ;

  Exit:
    ਵਾਪਸ error;
  }


  FT_Error
  ft_hash_str_ਪਾਓ( ਅਚਲ ਅੱਖਰ*  ਕੁੰਜੀ,
                      size_t       ਅੰਕੜੇ,
                      FT_Hash      hash,
                      FT_ਸਮ੍ਰਿਤੀ    ਸਮ੍ਰਿਤੀ )
  {
    FT_Hashkey  hk;


    hk.str = ਕੁੰਜੀ;

    ਵਾਪਸ hash_ਪਾਓ( hk, ਅੰਕੜੇ, hash, ਸਮ੍ਰਿਤੀ, TRUE );
  }


  FT_Error
  ft_hash_ਅੰਕ_ਪਾਓ( FT_Int     ਅੰਕ,
                      size_t     ਅੰਕੜੇ,
                      FT_Hash    hash,
                      FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ )
  {
    FT_Hashkey  hk;


    hk.ਅੰਕ = ਅੰਕ;

    ਵਾਪਸ hash_ਪਾਓ( hk, ਅੰਕੜੇ, hash, ਸਮ੍ਰਿਤੀ, TRUE );
  }


  FT_Error
  ft_hash_str_ਪਾਓ_no_overwrite( ਅਚਲ ਅੱਖਰ*  ਕੁੰਜੀ,
                                   size_t       ਅੰਕੜੇ,
                                   FT_Hash      hash,
                                   FT_ਸਮ੍ਰਿਤੀ    ਸਮ੍ਰਿਤੀ )
  {
    FT_Hashkey  hk;


    hk.str = ਕੁੰਜੀ;

    ਵਾਪਸ hash_ਪਾਓ( hk, ਅੰਕੜੇ, hash, ਸਮ੍ਰਿਤੀ, FALSE );
  }


  FT_Error
  ft_hash_ਅੰਕ_ਪਾਓ_no_overwrite( FT_Int     ਅੰਕ,
                                   size_t     ਅੰਕੜੇ,
                                   FT_Hash    hash,
                                   FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ )
  {
    FT_Hashkey  hk;


    hk.ਅੰਕ = ਅੰਕ;

    ਵਾਪਸ hash_ਪਾਓ( hk, ਅੰਕੜੇ, hash, ਸਮ੍ਰਿਤੀ, FALSE );
  }


  ਸਥਿਰ size_t*
  hash_lookup( FT_Hashkey  ਕੁੰਜੀ,
               FT_Hash     hash )
  {
    FT_Hashnode*  np = hash_ਬਾਲਟੀ( ਕੁੰਜੀ, hash );


    ਵਾਪਸ (*np) ? &(*np)->ਅੰਕੜੇ
                 : ਨਿਰਾ;
  }


  size_t*
  ft_hash_str_lookup( ਅਚਲ ਅੱਖਰ*  ਕੁੰਜੀ,
                      FT_Hash      hash )
  {
    FT_Hashkey  hk;


    hk.str = ਕੁੰਜੀ;

    ਵਾਪਸ hash_lookup( hk, hash );
  }


  size_t*
  ft_hash_ਅੰਕ_lookup( FT_Int   ਅੰਕ,
                      FT_Hash  hash )
  {
    FT_Hashkey  hk;


    hk.ਅੰਕ = ਅੰਕ;

    ਵਾਪਸ hash_lookup( hk, hash );
  }


  FT_Bool
  ft_hash_ਅੰਕ_iterator( FT_UInt  *idx,
                        FT_Int   *ਕੁੰਜੀ,
                        size_t   *ਮੁੱਲ,
                        FT_Hash   hash )
  {
    FT_Hashnode  nn = ਨਿਰਾ;


    ਜਦੋਂ ( 1 )
    {
      ਜੇ ( *idx >= hash->ਮਾਪ )
        ਵਾਪਸ 0;

      nn = hash->ਸਾਰਣੀ[*idx];
      ਜੇ ( nn )
        ਰੋਕੋ;

      (*idx)++;
    }

    ਜੇ ( ਕੁੰਜੀ )
      *ਕੁੰਜੀ = nn->ਕੁੰਜੀ.ਅੰਕ;
    ਜੇ ( ਮੁੱਲ )
      *ਮੁੱਲ = nn->ਅੰਕੜੇ;

    (*idx)++;

    ਵਾਪਸ 1;
  }


  FT_Bool
  ft_hash_str_iterator( FT_UInt      *idx,
                        ਅਚਲ ਅੱਖਰ*  *ਕੁੰਜੀ,
                        size_t       *ਮੁੱਲ,
                        FT_Hash       hash )
  {
    FT_Hashnode  nn = ਨਿਰਾ;


    ਜਦੋਂ ( 1 )
    {
      ਜੇ ( *idx >= hash->ਮਾਪ )
        ਵਾਪਸ 0;

      nn = hash->ਸਾਰਣੀ[*idx];
      ਜੇ ( nn )
        ਰੋਕੋ;

      (*idx)++;
    }

    ਜੇ ( ਕੁੰਜੀ )
      *ਕੁੰਜੀ = nn->ਕੁੰਜੀ.str;
    ਜੇ ( ਮੁੱਲ )
      *ਮੁੱਲ = nn->ਅੰਕੜੇ;

    (*idx)++;

    ਵਾਪਸ 1;
  }


/*ਸਿਰਾ*/
