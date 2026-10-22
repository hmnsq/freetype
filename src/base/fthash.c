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


#define INITIAL_HT_SIZE  241


  स्थिर FT_ULong
  hash_str_lookup( FT_Hashkey*  कुञ्चिका )
  {
    ध्रुव वर्ण*  kp  = कुञ्चिका->str;
    FT_ULong     res = 0;


    /* Mocklisp hash function. */
    याव ( *kp )
      res = ( res << 5 ) - res + (FT_ULong)*kp++;

    पटिनिवत्त res;
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

    पटिनिवत्त res;
  }


  स्थिर FT_Bool
  hash_str_compare( FT_Hashkey*  a,
                    FT_Hashkey*  b )
  {
    सचे ( a->str[0] == b->str[0]           &&
         ft_strcmp( a->str, b->str ) == 0 )
      पटिनिवत्त 1;

    पटिनिवत्त 0;
  }


  स्थिर FT_Bool
  hash_अङ्कः_compare( FT_Hashkey*  a,
                    FT_Hashkey*  b )
  {
    सचे ( a->अङ्कः == b->अङ्कः )
      पटिनिवत्त 1;

    पटिनिवत्त 0;
  }


  स्थिर FT_Hashnode*
  hash_bucket( FT_Hashkey  कुञ्चिका,
               FT_Hash     hash )
  {
    FT_ULong      res = 0;
    FT_Hashnode*  bp  = hash->table;
    FT_Hashnode*  ndp;


    res = (hash->lookup)( &कुञ्चिका );

    ndp = bp + ( res % hash->size );
    याव ( *ndp )
    {
      सचे ( (hash->compare)( &(*ndp)->कुञ्चिका, &कुञ्चिका ) )
        भङ्ग;

      ndp--;
      सचे ( ndp < bp )
        ndp = bp + ( hash->size - 1 );
    }

    पटिनिवत्त ndp;
  }


  स्थिर FT_Error
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

    सचे ( FT_NEW_ARRAY( hash->table, hash->size ) )
      गच्छ Exit;

    अत्थाय ( i = 0, bp = obp; i < sz; i++, bp++ )
    {
      सचे ( *bp )
      {
        nbp = hash_bucket( (*bp)->कुञ्चिका, hash );
        *nbp = *bp;
      }
    }

    FT_FREE( obp );

  Exit:
    पटिनिवत्त error;
  }


  स्थिर FT_Error
  hash_init( FT_Hash    hash,
             FT_Bool    is_अङ्कः,
             FT_Memory  memory )
  {
    FT_UInt   sz = INITIAL_HT_SIZE;
    FT_Error  error;


    hash->size  = sz;
    hash->limit = sz / 3;
    hash->used  = 0;

    सचे ( is_अङ्कः )
    {
      hash->lookup  = hash_अङ्कः_lookup;
      hash->compare = hash_अङ्कः_compare;
    }
    अञ्ञथा
    {
      hash->lookup  = hash_str_lookup;
      hash->compare = hash_str_compare;
    }

    FT_MEM_NEW_ARRAY( hash->table, sz );

    पटिनिवत्त error;
  }


  FT_Error
  ft_hash_str_init( FT_Hash    hash,
                    FT_Memory  memory )
  {
    पटिनिवत्त hash_init( hash, 0, memory );
  }


  FT_Error
  ft_hash_अङ्कः_init( FT_Hash    hash,
                    FT_Memory  memory )
  {
    पटिनिवत्त hash_init( hash, 1, memory );
  }


  शून्य
  ft_hash_str_free( FT_Hash    hash,
                    FT_Memory  memory )
  {
    सचे ( hash )
    {
      FT_UInt       sz = hash->size;
      FT_Hashnode*  bp = hash->table;
      FT_UInt       i;


      अत्थाय ( i = 0; i < sz; i++, bp++ )
        FT_FREE( *bp );

      FT_FREE( hash->table );
    }
  }


  /* `ft_hash_num_free' is the same as `ft_hash_str_free' */


  स्थिर FT_Error
  hash_निवेशय( FT_Hashkey  कुञ्चिका,
               size_t      data,
               FT_Hash     hash,
               FT_Memory   memory,
               FT_Bool     overwrite )
  {
    FT_Hashnode   nn;
    FT_Hashnode*  bp    = hash_bucket( कुञ्चिका, hash );
    FT_Error      error = FT_Err_Ok;


    nn = *bp;
    सचे ( !nn )
    {
      सचे ( FT_QNEW( nn ) )
        गच्छ Exit;
      *bp = nn;

      nn->कुञ्चिका  = कुञ्चिका;
      nn->data = data;

      सचे ( hash->used >= hash->limit )
      {
        error = hash_rehash( hash, memory );
        सचे ( error )
          गच्छ Exit;
      }

      hash->used++;
    }
    अञ्ञथा सचे ( overwrite )
      nn->data = data;

  Exit:
    पटिनिवत्त error;
  }


  FT_Error
  ft_hash_str_निवेशय( ध्रुव वर्ण*  कुञ्चिका,
                      size_t       data,
                      FT_Hash      hash,
                      FT_Memory    memory )
  {
    FT_Hashkey  hk;


    hk.str = कुञ्चिका;

    पटिनिवत्त hash_निवेशय( hk, data, hash, memory, TRUE );
  }


  FT_Error
  ft_hash_अङ्कः_निवेशय( FT_Int     अङ्कः,
                      size_t     data,
                      FT_Hash    hash,
                      FT_Memory  memory )
  {
    FT_Hashkey  hk;


    hk.अङ्कः = अङ्कः;

    पटिनिवत्त hash_निवेशय( hk, data, hash, memory, TRUE );
  }


  FT_Error
  ft_hash_str_निवेशय_no_overwrite( ध्रुव वर्ण*  कुञ्चिका,
                                   size_t       data,
                                   FT_Hash      hash,
                                   FT_Memory    memory )
  {
    FT_Hashkey  hk;


    hk.str = कुञ्चिका;

    पटिनिवत्त hash_निवेशय( hk, data, hash, memory, FALSE );
  }


  FT_Error
  ft_hash_अङ्कः_निवेशय_no_overwrite( FT_Int     अङ्कः,
                                   size_t     data,
                                   FT_Hash    hash,
                                   FT_Memory  memory )
  {
    FT_Hashkey  hk;


    hk.अङ्कः = अङ्कः;

    पटिनिवत्त hash_निवेशय( hk, data, hash, memory, FALSE );
  }


  स्थिर size_t*
  hash_lookup( FT_Hashkey  कुञ्चिका,
               FT_Hash     hash )
  {
    FT_Hashnode*  np = hash_bucket( कुञ्चिका, hash );


    पटिनिवत्त (*np) ? &(*np)->data
                 : रिक्त;
  }


  size_t*
  ft_hash_str_lookup( ध्रुव वर्ण*  कुञ्चिका,
                      FT_Hash      hash )
  {
    FT_Hashkey  hk;


    hk.str = कुञ्चिका;

    पटिनिवत्त hash_lookup( hk, hash );
  }


  size_t*
  ft_hash_अङ्कः_lookup( FT_Int   अङ्कः,
                      FT_Hash  hash )
  {
    FT_Hashkey  hk;


    hk.अङ्कः = अङ्कः;

    पटिनिवत्त hash_lookup( hk, hash );
  }


  FT_Bool
  ft_hash_अङ्कः_iterator( FT_UInt  *idx,
                        FT_Int   *कुञ्चिका,
                        size_t   *मानम्,
                        FT_Hash   hash )
  {
    FT_Hashnode  nn = रिक्त;


    याव ( 1 )
    {
      सचे ( *idx >= hash->size )
        पटिनिवत्त 0;

      nn = hash->table[*idx];
      सचे ( nn )
        भङ्ग;

      (*idx)++;
    }

    सचे ( कुञ्चिका )
      *कुञ्चिका = nn->कुञ्चिका.अङ्कः;
    सचे ( मानम् )
      *मानम् = nn->data;

    (*idx)++;

    पटिनिवत्त 1;
  }


  FT_Bool
  ft_hash_str_iterator( FT_UInt      *idx,
                        ध्रुव वर्ण*  *कुञ्चिका,
                        size_t       *मानम्,
                        FT_Hash       hash )
  {
    FT_Hashnode  nn = रिक्त;


    याव ( 1 )
    {
      सचे ( *idx >= hash->size )
        पटिनिवत्त 0;

      nn = hash->table[*idx];
      सचे ( nn )
        भङ्ग;

      (*idx)++;
    }

    सचे ( कुञ्चिका )
      *कुञ्चिका = nn->कुञ्चिका.str;
    सचे ( मानम् )
      *मानम् = nn->data;

    (*idx)++;

    पटिनिवत्त 1;
  }


/* END */
