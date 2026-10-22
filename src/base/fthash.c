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


#ವ್ಯಾಖ್ಯಾನಿಸು INITIAL_HT_ಗಾತ್ರ  241


  ಸ್ಥಿರ FT_ULong
  hash_str_lookup( FT_Hashkey*  ಕೀಲಿ )
  {
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*  kp  = ಕೀಲಿ->str;
    FT_ULong     res = 0;


    /* Mocklisp hash function. */
    ಆಗ ( *kp )
      res = ( res << 5 ) - res + (FT_ULong)*kp++;

    ಹಿಂತಿರುಗಿ res;
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

    ಹಿಂತಿರುಗಿ res;
  }


  ಸ್ಥಿರ FT_Bool
  hash_str_compare( FT_Hashkey*  a,
                    FT_Hashkey*  b )
  {
    ಆದರೆ ( a->str[0] == b->str[0]           &&
         ft_strcmp( a->str, b->str ) == 0 )
      ಹಿಂತಿರುಗಿ 1;

    ಹಿಂತಿರುಗಿ 0;
  }


  ಸ್ಥಿರ FT_Bool
  hash_ಅಂಕೆ_compare( FT_Hashkey*  a,
                    FT_Hashkey*  b )
  {
    ಆದರೆ ( a->ಅಂಕೆ == b->ಅಂಕೆ )
      ಹಿಂತಿರುಗಿ 1;

    ಹಿಂತಿರುಗಿ 0;
  }


  ಸ್ಥಿರ FT_Hashnode*
  hash_ಪಾತ್ರೆ( FT_Hashkey  ಕೀಲಿ,
               FT_Hash     hash )
  {
    FT_ULong      res = 0;
    FT_Hashnode*  bp  = hash->ಕೋಷ್ಟಕ;
    FT_Hashnode*  ndp;


    res = (hash->lookup)( &ಕೀಲಿ );

    ndp = bp + ( res % hash->ಗಾತ್ರ_2 );
    ಆಗ ( *ndp )
    {
      ಆದರೆ ( (hash->compare)( &(*ndp)->ಕೀಲಿ, &ಕೀಲಿ ) )
        ನಿಲ್ಲಿಸು;

      ndp--;
      ಆದರೆ ( ndp < bp )
        ndp = bp + ( hash->ಗಾತ್ರ_2 - 1 );
    }

    ಹಿಂತಿರುಗಿ ndp;
  }


  ಸ್ಥಿರ FT_Error
  hash_rehash( FT_Hash    hash,
               FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ )
  {
    FT_Hashnode*  obp = hash->ಕೋಷ್ಟಕ;
    FT_Hashnode*  bp;
    FT_Hashnode*  nbp;

    FT_UInt   i, sz = hash->ಗಾತ್ರ_2;
    FT_Error  error = FT_Err_Ok;


    hash->ಗಾತ್ರ_2 <<= 1;
    hash->ಎಲ್ಲೆ  = hash->ಗಾತ್ರ_2 / 3;

    ಆದರೆ ( FT_NEW_ARRAY( hash->ಕೋಷ್ಟಕ, hash->ಗಾತ್ರ_2 ) )
      ಹೋಗು Exit;

    ಗಾಗಿ ( i = 0, bp = obp; i < sz; i++, bp++ )
    {
      ಆದರೆ ( *bp )
      {
        nbp = hash_ಪಾತ್ರೆ( (*bp)->ಕೀಲಿ, hash );
        *nbp = *bp;
      }
    }

    FT_FREE( obp );

  Exit:
    ಹಿಂತಿರುಗಿ error;
  }


  ಸ್ಥಿರ FT_Error
  hash_init( FT_Hash    hash,
             FT_Bool    is_ಅಂಕೆ,
             FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ )
  {
    FT_UInt   sz = INITIAL_HT_ಗಾತ್ರ;
    FT_Error  error;


    hash->ಗಾತ್ರ_2  = sz;
    hash->ಎಲ್ಲೆ = sz / 3;
    hash->ಬಳಸಿದ  = 0;

    ಆದರೆ ( is_ಅಂಕೆ )
    {
      hash->lookup  = hash_ಅಂಕೆ_lookup;
      hash->compare = hash_ಅಂಕೆ_compare;
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
      hash->lookup  = hash_str_lookup;
      hash->compare = hash_str_compare;
    }

    FT_MEM_NEW_ARRAY( hash->ಕೋಷ್ಟಕ, sz );

    ಹಿಂತಿರುಗಿ error;
  }


  FT_Error
  ft_hash_str_init( FT_Hash    hash,
                    FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ )
  {
    ಹಿಂತಿರುಗಿ hash_init( hash, 0, ಸ್ಮರಣೆ );
  }


  FT_Error
  ft_hash_ಅಂಕೆ_init( FT_Hash    hash,
                    FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ )
  {
    ಹಿಂತಿರುಗಿ hash_init( hash, 1, ಸ್ಮರಣೆ );
  }


  ಶೂನ್ಯ
  ft_hash_str_free( FT_Hash    hash,
                    FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ )
  {
    ಆದರೆ ( hash )
    {
      FT_UInt       sz = hash->ಗಾತ್ರ_2;
      FT_Hashnode*  bp = hash->ಕೋಷ್ಟಕ;
      FT_UInt       i;


      ಗಾಗಿ ( i = 0; i < sz; i++, bp++ )
        FT_FREE( *bp );

      FT_FREE( hash->ಕೋಷ್ಟಕ );
    }
  }


  /* `ft_hash_num_free' is the same as `ft_hash_str_free' */


  ಸ್ಥಿರ FT_Error
  hash_ಸೇರಿಸು( FT_Hashkey  ಕೀಲಿ,
               size_t      ದತ್ತಾಂಶ,
               FT_Hash     hash,
               FT_ಸ್ಮರಣೆ   ಸ್ಮರಣೆ,
               FT_Bool     overwrite )
  {
    FT_Hashnode   nn;
    FT_Hashnode*  bp    = hash_ಪಾತ್ರೆ( ಕೀಲಿ, hash );
    FT_Error      error = FT_Err_Ok;


    nn = *bp;
    ಆದರೆ ( !nn )
    {
      ಆದರೆ ( FT_QNEW( nn ) )
        ಹೋಗು Exit;
      *bp = nn;

      nn->ಕೀಲಿ  = ಕೀಲಿ;
      nn->ದತ್ತಾಂಶ = ದತ್ತಾಂಶ;

      ಆದರೆ ( hash->ಬಳಸಿದ >= hash->ಎಲ್ಲೆ )
      {
        error = hash_rehash( hash, ಸ್ಮರಣೆ );
        ಆದರೆ ( error )
          ಹೋಗು Exit;
      }

      hash->ಬಳಸಿದ++;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ( overwrite )
      nn->ದತ್ತಾಂಶ = ದತ್ತಾಂಶ;

  Exit:
    ಹಿಂತಿರುಗಿ error;
  }


  FT_Error
  ft_hash_str_ಸೇರಿಸು( ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*  ಕೀಲಿ,
                      size_t       ದತ್ತಾಂಶ,
                      FT_Hash      hash,
                      FT_ಸ್ಮರಣೆ    ಸ್ಮರಣೆ )
  {
    FT_Hashkey  hk;


    hk.str = ಕೀಲಿ;

    ಹಿಂತಿರುಗಿ hash_ಸೇರಿಸು( hk, ದತ್ತಾಂಶ, hash, ಸ್ಮರಣೆ, TRUE );
  }


  FT_Error
  ft_hash_ಅಂಕೆ_ಸೇರಿಸು( FT_Int     ಅಂಕೆ,
                      size_t     ದತ್ತಾಂಶ,
                      FT_Hash    hash,
                      FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ )
  {
    FT_Hashkey  hk;


    hk.ಅಂಕೆ = ಅಂಕೆ;

    ಹಿಂತಿರುಗಿ hash_ಸೇರಿಸು( hk, ದತ್ತಾಂಶ, hash, ಸ್ಮರಣೆ, TRUE );
  }


  FT_Error
  ft_hash_str_ಸೇರಿಸು_no_overwrite( ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*  ಕೀಲಿ,
                                   size_t       ದತ್ತಾಂಶ,
                                   FT_Hash      hash,
                                   FT_ಸ್ಮರಣೆ    ಸ್ಮರಣೆ )
  {
    FT_Hashkey  hk;


    hk.str = ಕೀಲಿ;

    ಹಿಂತಿರುಗಿ hash_ಸೇರಿಸು( hk, ದತ್ತಾಂಶ, hash, ಸ್ಮರಣೆ, FALSE );
  }


  FT_Error
  ft_hash_ಅಂಕೆ_ಸೇರಿಸು_no_overwrite( FT_Int     ಅಂಕೆ,
                                   size_t     ದತ್ತಾಂಶ,
                                   FT_Hash    hash,
                                   FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ )
  {
    FT_Hashkey  hk;


    hk.ಅಂಕೆ = ಅಂಕೆ;

    ಹಿಂತಿರುಗಿ hash_ಸೇರಿಸು( hk, ದತ್ತಾಂಶ, hash, ಸ್ಮರಣೆ, FALSE );
  }


  ಸ್ಥಿರ size_t*
  hash_lookup( FT_Hashkey  ಕೀಲಿ,
               FT_Hash     hash )
  {
    FT_Hashnode*  np = hash_ಪಾತ್ರೆ( ಕೀಲಿ, hash );


    ಹಿಂತಿರುಗಿ (*np) ? &(*np)->ದತ್ತಾಂಶ
                 : ಖಾಲಿ;
  }


  size_t*
  ft_hash_str_lookup( ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*  ಕೀಲಿ,
                      FT_Hash      hash )
  {
    FT_Hashkey  hk;


    hk.str = ಕೀಲಿ;

    ಹಿಂತಿರುಗಿ hash_lookup( hk, hash );
  }


  size_t*
  ft_hash_ಅಂಕೆ_lookup( FT_Int   ಅಂಕೆ,
                      FT_Hash  hash )
  {
    FT_Hashkey  hk;


    hk.ಅಂಕೆ = ಅಂಕೆ;

    ಹಿಂತಿರುಗಿ hash_lookup( hk, hash );
  }


  FT_Bool
  ft_hash_ಅಂಕೆ_iterator( FT_UInt  *idx,
                        FT_Int   *ಕೀಲಿ,
                        size_t   *ಮೌಲ್ಯ,
                        FT_Hash   hash )
  {
    FT_Hashnode  nn = ಖಾಲಿ;


    ಆಗ ( 1 )
    {
      ಆದರೆ ( *idx >= hash->ಗಾತ್ರ_2 )
        ಹಿಂತಿರುಗಿ 0;

      nn = hash->ಕೋಷ್ಟಕ[*idx];
      ಆದರೆ ( nn )
        ನಿಲ್ಲಿಸು;

      (*idx)++;
    }

    ಆದರೆ ( ಕೀಲಿ )
      *ಕೀಲಿ = nn->ಕೀಲಿ.ಅಂಕೆ;
    ಆದರೆ ( ಮೌಲ್ಯ )
      *ಮೌಲ್ಯ = nn->ದತ್ತಾಂಶ;

    (*idx)++;

    ಹಿಂತಿರುಗಿ 1;
  }


  FT_Bool
  ft_hash_str_iterator( FT_UInt      *idx,
                        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*  *ಕೀಲಿ,
                        size_t       *ಮೌಲ್ಯ,
                        FT_Hash       hash )
  {
    FT_Hashnode  nn = ಖಾಲಿ;


    ಆಗ ( 1 )
    {
      ಆದರೆ ( *idx >= hash->ಗಾತ್ರ_2 )
        ಹಿಂತಿರುಗಿ 0;

      nn = hash->ಕೋಷ್ಟಕ[*idx];
      ಆದರೆ ( nn )
        ನಿಲ್ಲಿಸು;

      (*idx)++;
    }

    ಆದರೆ ( ಕೀಲಿ )
      *ಕೀಲಿ = nn->ಕೀಲಿ.str;
    ಆದರೆ ( ಮೌಲ್ಯ )
      *ಮೌಲ್ಯ = nn->ದತ್ತಾಂಶ;

    (*idx)++;

    ಹಿಂತಿರುಗಿ 1;
  }


/*ಕೊನೆ*/
