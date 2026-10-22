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
  hash_str_lookup( FT_Hashkey*  साँचो )
  {
    अचर वर्ण*  kp  = साँचो->str;
    FT_ULong     res = 0;


    /* Mocklisp hash function. */
    जबसम्म ( *kp )
      res = ( res << 5 ) - res + (FT_ULong)*kp++;

    फर्काऊ res;
  }


  स्थिर FT_ULong
  hash_अङ्क_lookup( FT_Hashkey*  साँचो )
  {
    FT_ULong  अङ्क = (FT_ULong)साँचो->अङ्क;
    FT_ULong  res;


    /* Mocklisp hash function. */
    res = अङ्क & 0xFF;
    res = ( res << 5 ) - res + ( ( अङ्क >>  8 ) & 0xFF );
    res = ( res << 5 ) - res + ( ( अङ्क >> 16 ) & 0xFF );
    res = ( res << 5 ) - res + ( ( अङ्क >> 24 ) & 0xFF );

    फर्काऊ res;
  }


  स्थिर FT_Bool
  hash_str_compare( FT_Hashkey*  a,
                    FT_Hashkey*  b )
  {
    यदि ( a->str[0] == b->str[0]           &&
         ft_strcmp( a->str, b->str ) == 0 )
      फर्काऊ 1;

    फर्काऊ 0;
  }


  स्थिर FT_Bool
  hash_अङ्क_compare( FT_Hashkey*  a,
                    FT_Hashkey*  b )
  {
    यदि ( a->अङ्क == b->अङ्क )
      फर्काऊ 1;

    फर्काऊ 0;
  }


  स्थिर FT_Hashnode*
  hash_bucket( FT_Hashkey  साँचो,
               FT_Hash     hash )
  {
    FT_ULong      res = 0;
    FT_Hashnode*  bp  = hash->table;
    FT_Hashnode*  ndp;


    res = (hash->lookup)( &साँचो );

    ndp = bp + ( res % hash->size );
    जबसम्म ( *ndp )
    {
      यदि ( (hash->compare)( &(*ndp)->साँचो, &साँचो ) )
        रोक;

      ndp--;
      यदि ( ndp < bp )
        ndp = bp + ( hash->size - 1 );
    }

    फर्काऊ ndp;
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

    यदि ( FT_NEW_ARRAY( hash->table, hash->size ) )
      गच्छ Exit;

    लागि ( i = 0, bp = obp; i < sz; i++, bp++ )
    {
      यदि ( *bp )
      {
        nbp = hash_bucket( (*bp)->साँचो, hash );
        *nbp = *bp;
      }
    }

    FT_FREE( obp );

  Exit:
    फर्काऊ error;
  }


  स्थिर FT_Error
  hash_init( FT_Hash    hash,
             FT_Bool    is_अङ्क,
             FT_Memory  memory )
  {
    FT_UInt   sz = INITIAL_HT_SIZE;
    FT_Error  error;


    hash->size  = sz;
    hash->limit = sz / 3;
    hash->used  = 0;

    यदि ( is_अङ्क )
    {
      hash->lookup  = hash_अङ्क_lookup;
      hash->compare = hash_अङ्क_compare;
    }
    अन्यथा
    {
      hash->lookup  = hash_str_lookup;
      hash->compare = hash_str_compare;
    }

    FT_MEM_NEW_ARRAY( hash->table, sz );

    फर्काऊ error;
  }


  FT_Error
  ft_hash_str_init( FT_Hash    hash,
                    FT_Memory  memory )
  {
    फर्काऊ hash_init( hash, 0, memory );
  }


  FT_Error
  ft_hash_अङ्क_init( FT_Hash    hash,
                    FT_Memory  memory )
  {
    फर्काऊ hash_init( hash, 1, memory );
  }


  शून्य
  ft_hash_str_free( FT_Hash    hash,
                    FT_Memory  memory )
  {
    यदि ( hash )
    {
      FT_UInt       sz = hash->size;
      FT_Hashnode*  bp = hash->table;
      FT_UInt       i;


      लागि ( i = 0; i < sz; i++, bp++ )
        FT_FREE( *bp );

      FT_FREE( hash->table );
    }
  }


  /* `ft_hash_num_free' is the same as `ft_hash_str_free' */


  स्थिर FT_Error
  hash_घुसाउ( FT_Hashkey  साँचो,
               माप_प्रकार      data,
               FT_Hash     hash,
               FT_Memory   memory,
               FT_Bool     overwrite )
  {
    FT_Hashnode   nn;
    FT_Hashnode*  bp    = hash_bucket( साँचो, hash );
    FT_Error      error = FT_Err_Ok;


    nn = *bp;
    यदि ( !nn )
    {
      यदि ( FT_QNEW( nn ) )
        गच्छ Exit;
      *bp = nn;

      nn->साँचो  = साँचो;
      nn->data = data;

      यदि ( hash->used >= hash->limit )
      {
        error = hash_rehash( hash, memory );
        यदि ( error )
          गच्छ Exit;
      }

      hash->used++;
    }
    अन्यथा यदि ( overwrite )
      nn->data = data;

  Exit:
    फर्काऊ error;
  }


  FT_Error
  ft_hash_str_घुसाउ( अचर वर्ण*  साँचो,
                      माप_प्रकार       data,
                      FT_Hash      hash,
                      FT_Memory    memory )
  {
    FT_Hashkey  hk;


    hk.str = साँचो;

    फर्काऊ hash_घुसाउ( hk, data, hash, memory, TRUE );
  }


  FT_Error
  ft_hash_अङ्क_घुसाउ( FT_Int     अङ्क,
                      माप_प्रकार     data,
                      FT_Hash    hash,
                      FT_Memory  memory )
  {
    FT_Hashkey  hk;


    hk.अङ्क = अङ्क;

    फर्काऊ hash_घुसाउ( hk, data, hash, memory, TRUE );
  }


  FT_Error
  ft_hash_str_घुसाउ_no_overwrite( अचर वर्ण*  साँचो,
                                   माप_प्रकार       data,
                                   FT_Hash      hash,
                                   FT_Memory    memory )
  {
    FT_Hashkey  hk;


    hk.str = साँचो;

    फर्काऊ hash_घुसाउ( hk, data, hash, memory, FALSE );
  }


  FT_Error
  ft_hash_अङ्क_घुसाउ_no_overwrite( FT_Int     अङ्क,
                                   माप_प्रकार     data,
                                   FT_Hash    hash,
                                   FT_Memory  memory )
  {
    FT_Hashkey  hk;


    hk.अङ्क = अङ्क;

    फर्काऊ hash_घुसाउ( hk, data, hash, memory, FALSE );
  }


  स्थिर माप_प्रकार*
  hash_lookup( FT_Hashkey  साँचो,
               FT_Hash     hash )
  {
    FT_Hashnode*  np = hash_bucket( साँचो, hash );


    फर्काऊ (*np) ? &(*np)->data
                 : रिक्त;
  }


  माप_प्रकार*
  ft_hash_str_lookup( अचर वर्ण*  साँचो,
                      FT_Hash      hash )
  {
    FT_Hashkey  hk;


    hk.str = साँचो;

    फर्काऊ hash_lookup( hk, hash );
  }


  माप_प्रकार*
  ft_hash_अङ्क_lookup( FT_Int   अङ्क,
                      FT_Hash  hash )
  {
    FT_Hashkey  hk;


    hk.अङ्क = अङ्क;

    फर्काऊ hash_lookup( hk, hash );
  }


  FT_Bool
  ft_hash_अङ्क_iterator( FT_UInt  *idx,
                        FT_Int   *साँचो,
                        माप_प्रकार   *मान,
                        FT_Hash   hash )
  {
    FT_Hashnode  nn = रिक्त;


    जबसम्म ( 1 )
    {
      यदि ( *idx >= hash->size )
        फर्काऊ 0;

      nn = hash->table[*idx];
      यदि ( nn )
        रोक;

      (*idx)++;
    }

    यदि ( साँचो )
      *साँचो = nn->साँचो.अङ्क;
    यदि ( मान )
      *मान = nn->data;

    (*idx)++;

    फर्काऊ 1;
  }


  FT_Bool
  ft_hash_str_iterator( FT_UInt      *idx,
                        अचर वर्ण*  *साँचो,
                        माप_प्रकार       *मान,
                        FT_Hash       hash )
  {
    FT_Hashnode  nn = रिक्त;


    जबसम्म ( 1 )
    {
      यदि ( *idx >= hash->size )
        फर्काऊ 0;

      nn = hash->table[*idx];
      यदि ( nn )
        रोक;

      (*idx)++;
    }

    यदि ( साँचो )
      *साँचो = nn->साँचो.str;
    यदि ( मान )
      *मान = nn->data;

    (*idx)++;

    फर्काऊ 1;
  }


/* END */
