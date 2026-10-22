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


#अन्तर्भाव <freetype/internal/fthash.h>
#अन्तर्भाव <freetype/internal/ftmemory.h>
#अन्तर्भाव <freetype/internal/ftobjs.h>


#परिभाषय INITIAL_HT_आकार  241


  स्थिर FT_ULong
  hash_str_lookup( FT_Hashkey*  कुञ्चिका )
  {
    ध्रुव वर्ण*  kp  = कुञ्चिका->str;
    FT_ULong     res = 0;


    /* Mocklisp hash function. */
    यावत् ( *kp )
      res = ( res << 5 ) - res + (FT_ULong)*kp++;

    प्रत्यागम res;
  }


  स्थिर FT_ULong
  hash_अङ्कः_lookup( FT_Hashkey*  कुञ्चिका )
  {
    FT_ULong  अङ्कः = (FT_ULong)कुञ्चिका->अङ्कः;
    FT_ULong  res;


    /* Mocklisp hash function. */
    res = अङ्कः & 0xFF;
    res = ( res << 5 ) - res + ( ( अङ्कः >>  8 ) & 0xFF );
    res = ( res << 5 ) - res + ( ( अङ्कः >> 16 ) & 0xFF );
    res = ( res << 5 ) - res + ( ( अङ्कः >> 24 ) & 0xFF );

    प्रत्यागम res;
  }


  स्थिर FT_Bool
  hash_str_compare( FT_Hashkey*  a,
                    FT_Hashkey*  b )
  {
    यदि ( a->str[0] == b->str[0]           &&
         ft_strcmp( a->str, b->str ) == 0 )
      प्रत्यागम 1;

    प्रत्यागम 0;
  }


  स्थिर FT_Bool
  hash_अङ्कः_compare( FT_Hashkey*  a,
                    FT_Hashkey*  b )
  {
    यदि ( a->अङ्कः == b->अङ्कः )
      प्रत्यागम 1;

    प्रत्यागम 0;
  }


  स्थिर FT_Hashnode*
  hash_bucket( FT_Hashkey  कुञ्चिका,
               FT_Hash     hash )
  {
    FT_ULong      res = 0;
    FT_Hashnode*  bp  = hash->सारणी;
    FT_Hashnode*  ndp;


    res = (hash->lookup)( &कुञ्चिका );

    ndp = bp + ( res % hash->आकार );
    यावत् ( *ndp )
    {
      यदि ( (hash->compare)( &(*ndp)->कुञ्चिका, &कुञ्चिका ) )
        भङ्ग;

      ndp--;
      यदि ( ndp < bp )
        ndp = bp + ( hash->आकार - 1 );
    }

    प्रत्यागम ndp;
  }


  स्थिर FT_Error
  hash_rehash( FT_Hash    hash,
               FT_स्मृतिः  स्मृतिः )
  {
    FT_Hashnode*  obp = hash->सारणी;
    FT_Hashnode*  bp;
    FT_Hashnode*  nbp;

    FT_UInt   i, sz = hash->आकार;
    FT_Error  error = FT_Err_Ok;


    hash->आकार <<= 1;
    hash->सीमा  = hash->आकार / 3;

    यदि ( FT_NEW_ARRAY( hash->सारणी, hash->आकार ) )
      गच्छ Exit;

    प्रति ( i = 0, bp = obp; i < sz; i++, bp++ )
    {
      यदि ( *bp )
      {
        nbp = hash_bucket( (*bp)->कुञ्चिका, hash );
        *nbp = *bp;
      }
    }

    FT_FREE( obp );

  Exit:
    प्रत्यागम error;
  }


  स्थिर FT_Error
  hash_init( FT_Hash    hash,
             FT_Bool    is_अङ्कः,
             FT_स्मृतिः  स्मृतिः )
  {
    FT_UInt   sz = INITIAL_HT_आकार;
    FT_Error  error;


    hash->आकार  = sz;
    hash->सीमा = sz / 3;
    hash->प्रयुक्तम्  = 0;

    यदि ( is_अङ्कः )
    {
      hash->lookup  = hash_अङ्कः_lookup;
      hash->compare = hash_अङ्कः_compare;
    }
    अन्यथा
    {
      hash->lookup  = hash_str_lookup;
      hash->compare = hash_str_compare;
    }

    FT_MEM_NEW_ARRAY( hash->सारणी, sz );

    प्रत्यागम error;
  }


  FT_Error
  ft_hash_str_init( FT_Hash    hash,
                    FT_स्मृतिः  स्मृतिः )
  {
    प्रत्यागम hash_init( hash, 0, स्मृतिः );
  }


  FT_Error
  ft_hash_अङ्कः_init( FT_Hash    hash,
                    FT_स्मृतिः  स्मृतिः )
  {
    प्रत्यागम hash_init( hash, 1, स्मृतिः );
  }


  शून्य
  ft_hash_str_free( FT_Hash    hash,
                    FT_स्मृतिः  स्मृतिः )
  {
    यदि ( hash )
    {
      FT_UInt       sz = hash->आकार;
      FT_Hashnode*  bp = hash->सारणी;
      FT_UInt       i;


      प्रति ( i = 0; i < sz; i++, bp++ )
        FT_FREE( *bp );

      FT_FREE( hash->सारणी );
    }
  }


  /* `ft_hash_num_free' is the same as `ft_hash_str_free' */


  स्थिर FT_Error
  hash_निवेशय( FT_Hashkey  कुञ्चिका,
               size_t      दत्त,
               FT_Hash     hash,
               FT_स्मृतिः   स्मृतिः,
               FT_Bool     overwrite )
  {
    FT_Hashnode   nn;
    FT_Hashnode*  bp    = hash_bucket( कुञ्चिका, hash );
    FT_Error      error = FT_Err_Ok;


    nn = *bp;
    यदि ( !nn )
    {
      यदि ( FT_QNEW( nn ) )
        गच्छ Exit;
      *bp = nn;

      nn->कुञ्चिका  = कुञ्चिका;
      nn->दत्त = दत्त;

      यदि ( hash->प्रयुक्तम् >= hash->सीमा )
      {
        error = hash_rehash( hash, स्मृतिः );
        यदि ( error )
          गच्छ Exit;
      }

      hash->प्रयुक्तम्++;
    }
    अन्यथा यदि ( overwrite )
      nn->दत्त = दत्त;

  Exit:
    प्रत्यागम error;
  }


  FT_Error
  ft_hash_str_निवेशय( ध्रुव वर्ण*  कुञ्चिका,
                      size_t       दत्त,
                      FT_Hash      hash,
                      FT_स्मृतिः    स्मृतिः )
  {
    FT_Hashkey  hk;


    hk.str = कुञ्चिका;

    प्रत्यागम hash_निवेशय( hk, दत्त, hash, स्मृतिः, TRUE );
  }


  FT_Error
  ft_hash_अङ्कः_निवेशय( FT_Int     अङ्कः,
                      size_t     दत्त,
                      FT_Hash    hash,
                      FT_स्मृतिः  स्मृतिः )
  {
    FT_Hashkey  hk;


    hk.अङ्कः = अङ्कः;

    प्रत्यागम hash_निवेशय( hk, दत्त, hash, स्मृतिः, TRUE );
  }


  FT_Error
  ft_hash_str_निवेशय_no_overwrite( ध्रुव वर्ण*  कुञ्चिका,
                                   size_t       दत्त,
                                   FT_Hash      hash,
                                   FT_स्मृतिः    स्मृतिः )
  {
    FT_Hashkey  hk;


    hk.str = कुञ्चिका;

    प्रत्यागम hash_निवेशय( hk, दत्त, hash, स्मृतिः, FALSE );
  }


  FT_Error
  ft_hash_अङ्कः_निवेशय_no_overwrite( FT_Int     अङ्कः,
                                   size_t     दत्त,
                                   FT_Hash    hash,
                                   FT_स्मृतिः  स्मृतिः )
  {
    FT_Hashkey  hk;


    hk.अङ्कः = अङ्कः;

    प्रत्यागम hash_निवेशय( hk, दत्त, hash, स्मृतिः, FALSE );
  }


  स्थिर size_t*
  hash_lookup( FT_Hashkey  कुञ्चिका,
               FT_Hash     hash )
  {
    FT_Hashnode*  np = hash_bucket( कुञ्चिका, hash );


    प्रत्यागम (*np) ? &(*np)->दत्त
                 : रिक्त;
  }


  size_t*
  ft_hash_str_lookup( ध्रुव वर्ण*  कुञ्चिका,
                      FT_Hash      hash )
  {
    FT_Hashkey  hk;


    hk.str = कुञ्चिका;

    प्रत्यागम hash_lookup( hk, hash );
  }


  size_t*
  ft_hash_अङ्कः_lookup( FT_Int   अङ्कः,
                      FT_Hash  hash )
  {
    FT_Hashkey  hk;


    hk.अङ्कः = अङ्कः;

    प्रत्यागम hash_lookup( hk, hash );
  }


  FT_Bool
  ft_hash_अङ्कः_iterator( FT_UInt  *idx,
                        FT_Int   *कुञ्चिका,
                        size_t   *मानम्,
                        FT_Hash   hash )
  {
    FT_Hashnode  nn = रिक्त;


    यावत् ( 1 )
    {
      यदि ( *idx >= hash->आकार )
        प्रत्यागम 0;

      nn = hash->सारणी[*idx];
      यदि ( nn )
        भङ्ग;

      (*idx)++;
    }

    यदि ( कुञ्चिका )
      *कुञ्चिका = nn->कुञ्चिका.अङ्कः;
    यदि ( मानम् )
      *मानम् = nn->दत्त;

    (*idx)++;

    प्रत्यागम 1;
  }


  FT_Bool
  ft_hash_str_iterator( FT_UInt      *idx,
                        ध्रुव वर्ण*  *कुञ्चिका,
                        size_t       *मानम्,
                        FT_Hash       hash )
  {
    FT_Hashnode  nn = रिक्त;


    यावत् ( 1 )
    {
      यदि ( *idx >= hash->आकार )
        प्रत्यागम 0;

      nn = hash->सारणी[*idx];
      यदि ( nn )
        भङ्ग;

      (*idx)++;
    }

    यदि ( कुञ्चिका )
      *कुञ्चिका = nn->कुञ्चिका.str;
    यदि ( मानम् )
      *मानम् = nn->दत्त;

    (*idx)++;

    प्रत्यागम 1;
  }


/*अन्तः*/
