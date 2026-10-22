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
  hash_str_lookup( FT_Hashkey*  कुंजी )
  {
    ध्रुव वर्ण*  kp  = कुंजी->str;
    FT_ULong     res = 0;


    /* Mocklisp hash function. */
    यावत् ( *kp )
      res = ( res << 5 ) - res + (FT_ULong)*kp++;

    प्रत्यागम res;
  }


  स्थिर FT_ULong
  hash_अंक_lookup( FT_Hashkey*  कुंजी )
  {
    FT_ULong  अंक = (FT_ULong)कुंजी->अंक;
    FT_ULong  res;


    /* Mocklisp hash function. */
    res = अंक & 0xFF;
    res = ( res << 5 ) - res + ( ( अंक >>  8 ) & 0xFF );
    res = ( res << 5 ) - res + ( ( अंक >> 16 ) & 0xFF );
    res = ( res << 5 ) - res + ( ( अंक >> 24 ) & 0xFF );

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
  hash_अंक_compare( FT_Hashkey*  a,
                    FT_Hashkey*  b )
  {
    यदि ( a->अंक == b->अंक )
      प्रत्यागम 1;

    प्रत्यागम 0;
  }


  स्थिर FT_Hashnode*
  hash_bucket( FT_Hashkey  कुंजी,
               FT_Hash     hash )
  {
    FT_ULong      res = 0;
    FT_Hashnode*  bp  = hash->table;
    FT_Hashnode*  ndp;


    res = (hash->lookup)( &कुंजी );

    ndp = bp + ( res % hash->size );
    यावत् ( *ndp )
    {
      यदि ( (hash->compare)( &(*ndp)->कुंजी, &कुंजी ) )
        भङ्ग;

      ndp--;
      यदि ( ndp < bp )
        ndp = bp + ( hash->size - 1 );
    }

    प्रत्यागम ndp;
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

    प्रति ( i = 0, bp = obp; i < sz; i++, bp++ )
    {
      यदि ( *bp )
      {
        nbp = hash_bucket( (*bp)->कुंजी, hash );
        *nbp = *bp;
      }
    }

    FT_FREE( obp );

  Exit:
    प्रत्यागम error;
  }


  स्थिर FT_Error
  hash_init( FT_Hash    hash,
             FT_Bool    is_अंक,
             FT_Memory  memory )
  {
    FT_UInt   sz = INITIAL_HT_SIZE;
    FT_Error  error;


    hash->size  = sz;
    hash->limit = sz / 3;
    hash->used  = 0;

    यदि ( is_अंक )
    {
      hash->lookup  = hash_अंक_lookup;
      hash->compare = hash_अंक_compare;
    }
    अन्यथा
    {
      hash->lookup  = hash_str_lookup;
      hash->compare = hash_str_compare;
    }

    FT_MEM_NEW_ARRAY( hash->table, sz );

    प्रत्यागम error;
  }


  FT_Error
  ft_hash_str_init( FT_Hash    hash,
                    FT_Memory  memory )
  {
    प्रत्यागम hash_init( hash, 0, memory );
  }


  FT_Error
  ft_hash_अंक_init( FT_Hash    hash,
                    FT_Memory  memory )
  {
    प्रत्यागम hash_init( hash, 1, memory );
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


      प्रति ( i = 0; i < sz; i++, bp++ )
        FT_FREE( *bp );

      FT_FREE( hash->table );
    }
  }


  /* `ft_hash_num_free' is the same as `ft_hash_str_free' */


  स्थिर FT_Error
  hash_जोड़ो( FT_Hashkey  कुंजी,
               माप_प्रकार      data,
               FT_Hash     hash,
               FT_Memory   memory,
               FT_Bool     overwrite )
  {
    FT_Hashnode   nn;
    FT_Hashnode*  bp    = hash_bucket( कुंजी, hash );
    FT_Error      error = FT_Err_Ok;


    nn = *bp;
    यदि ( !nn )
    {
      यदि ( FT_QNEW( nn ) )
        गच्छ Exit;
      *bp = nn;

      nn->कुंजी  = कुंजी;
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
    प्रत्यागम error;
  }


  FT_Error
  ft_hash_str_जोड़ो( ध्रुव वर्ण*  कुंजी,
                      माप_प्रकार       data,
                      FT_Hash      hash,
                      FT_Memory    memory )
  {
    FT_Hashkey  hk;


    hk.str = कुंजी;

    प्रत्यागम hash_जोड़ो( hk, data, hash, memory, TRUE );
  }


  FT_Error
  ft_hash_अंक_जोड़ो( FT_Int     अंक,
                      माप_प्रकार     data,
                      FT_Hash    hash,
                      FT_Memory  memory )
  {
    FT_Hashkey  hk;


    hk.अंक = अंक;

    प्रत्यागम hash_जोड़ो( hk, data, hash, memory, TRUE );
  }


  FT_Error
  ft_hash_str_जोड़ो_no_overwrite( ध्रुव वर्ण*  कुंजी,
                                   माप_प्रकार       data,
                                   FT_Hash      hash,
                                   FT_Memory    memory )
  {
    FT_Hashkey  hk;


    hk.str = कुंजी;

    प्रत्यागम hash_जोड़ो( hk, data, hash, memory, FALSE );
  }


  FT_Error
  ft_hash_अंक_जोड़ो_no_overwrite( FT_Int     अंक,
                                   माप_प्रकार     data,
                                   FT_Hash    hash,
                                   FT_Memory  memory )
  {
    FT_Hashkey  hk;


    hk.अंक = अंक;

    प्रत्यागम hash_जोड़ो( hk, data, hash, memory, FALSE );
  }


  स्थिर माप_प्रकार*
  hash_lookup( FT_Hashkey  कुंजी,
               FT_Hash     hash )
  {
    FT_Hashnode*  np = hash_bucket( कुंजी, hash );


    प्रत्यागम (*np) ? &(*np)->data
                 : रिक्त;
  }


  माप_प्रकार*
  ft_hash_str_lookup( ध्रुव वर्ण*  कुंजी,
                      FT_Hash      hash )
  {
    FT_Hashkey  hk;


    hk.str = कुंजी;

    प्रत्यागम hash_lookup( hk, hash );
  }


  माप_प्रकार*
  ft_hash_अंक_lookup( FT_Int   अंक,
                      FT_Hash  hash )
  {
    FT_Hashkey  hk;


    hk.अंक = अंक;

    प्रत्यागम hash_lookup( hk, hash );
  }


  FT_Bool
  ft_hash_अंक_iterator( FT_UInt  *idx,
                        FT_Int   *कुंजी,
                        माप_प्रकार   *मूल्य,
                        FT_Hash   hash )
  {
    FT_Hashnode  nn = रिक्त;


    यावत् ( 1 )
    {
      यदि ( *idx >= hash->size )
        प्रत्यागम 0;

      nn = hash->table[*idx];
      यदि ( nn )
        भङ्ग;

      (*idx)++;
    }

    यदि ( कुंजी )
      *कुंजी = nn->कुंजी.अंक;
    यदि ( मूल्य )
      *मूल्य = nn->data;

    (*idx)++;

    प्रत्यागम 1;
  }


  FT_Bool
  ft_hash_str_iterator( FT_UInt      *idx,
                        ध्रुव वर्ण*  *कुंजी,
                        माप_प्रकार       *मूल्य,
                        FT_Hash       hash )
  {
    FT_Hashnode  nn = रिक्त;


    यावत् ( 1 )
    {
      यदि ( *idx >= hash->size )
        प्रत्यागम 0;

      nn = hash->table[*idx];
      यदि ( nn )
        भङ्ग;

      (*idx)++;
    }

    यदि ( कुंजी )
      *कुंजी = nn->कुंजी.str;
    यदि ( मूल्य )
      *मूल्य = nn->data;

    (*idx)++;

    प्रत्यागम 1;
  }


/* END */
