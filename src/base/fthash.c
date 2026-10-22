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


#অন্তর্ভাৱ <freetype/internal/fthash.h>
#অন্তর্ভাৱ <freetype/internal/ftmemory.h>
#অন্তর্ভাৱ <freetype/internal/ftobjs.h>


#define INITIAL_HT_SIZE  241


  স্থির FT_ULong
  hash_str_lookup( FT_Hashkey*  চাবি )
  {
    ধ্রুৱ ৱর্ণ*  kp  = চাবি->str;
    FT_ULong     res = 0;


    /* Mocklisp hash function. */
    যেতিয়ালৈ ( *kp )
      res = ( res << 5 ) - res + (FT_ULong)*kp++;

    ঘূৰাই res;
  }


  স্থির FT_ULong
  hash_অঙ্ক_lookup( FT_Hashkey*  চাবি )
  {
    FT_ULong  অঙ্ক = (FT_ULong)চাবি->অঙ্ক;
    FT_ULong  res;


    /* Mocklisp hash function. */
    res = অঙ্ক & 0xFF;
    res = ( res << 5 ) - res + ( ( অঙ্ক >>  8 ) & 0xFF );
    res = ( res << 5 ) - res + ( ( অঙ্ক >> 16 ) & 0xFF );
    res = ( res << 5 ) - res + ( ( অঙ্ক >> 24 ) & 0xFF );

    ঘূৰাই res;
  }


  স্থির FT_Bool
  hash_str_compare( FT_Hashkey*  a,
                    FT_Hashkey*  b )
  {
    যদি ( a->str[0] == b->str[0]           &&
         ft_strcmp( a->str, b->str ) == 0 )
      ঘূৰাই 1;

    ঘূৰাই 0;
  }


  স্থির FT_Bool
  hash_অঙ্ক_compare( FT_Hashkey*  a,
                    FT_Hashkey*  b )
  {
    যদি ( a->অঙ্ক == b->অঙ্ক )
      ঘূৰাই 1;

    ঘূৰাই 0;
  }


  স্থির FT_Hashnode*
  hash_bucket( FT_Hashkey  চাবি,
               FT_Hash     hash )
  {
    FT_ULong      res = 0;
    FT_Hashnode*  bp  = hash->table;
    FT_Hashnode*  ndp;


    res = (hash->lookup)( &চাবি );

    ndp = bp + ( res % hash->size );
    যেতিয়ালৈ ( *ndp )
    {
      যদি ( (hash->compare)( &(*ndp)->চাবি, &চাবি ) )
        ভঙ্গ;

      ndp--;
      যদি ( ndp < bp )
        ndp = bp + ( hash->size - 1 );
    }

    ঘূৰাই ndp;
  }


  স্থির FT_Error
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

    যদি ( FT_NEW_ARRAY( hash->table, hash->size ) )
      গচ্ছ Exit;

    বাবে ( i = 0, bp = obp; i < sz; i++, bp++ )
    {
      যদি ( *bp )
      {
        nbp = hash_bucket( (*bp)->চাবি, hash );
        *nbp = *bp;
      }
    }

    FT_FREE( obp );

  Exit:
    ঘূৰাই error;
  }


  স্থির FT_Error
  hash_init( FT_Hash    hash,
             FT_Bool    is_অঙ্ক,
             FT_Memory  memory )
  {
    FT_UInt   sz = INITIAL_HT_SIZE;
    FT_Error  error;


    hash->size  = sz;
    hash->limit = sz / 3;
    hash->used  = 0;

    যদি ( is_অঙ্ক )
    {
      hash->lookup  = hash_অঙ্ক_lookup;
      hash->compare = hash_অঙ্ক_compare;
    }
    অন্যথা
    {
      hash->lookup  = hash_str_lookup;
      hash->compare = hash_str_compare;
    }

    FT_MEM_NEW_ARRAY( hash->table, sz );

    ঘূৰাই error;
  }


  FT_Error
  ft_hash_str_init( FT_Hash    hash,
                    FT_Memory  memory )
  {
    ঘূৰাই hash_init( hash, 0, memory );
  }


  FT_Error
  ft_hash_অঙ্ক_init( FT_Hash    hash,
                    FT_Memory  memory )
  {
    ঘূৰাই hash_init( hash, 1, memory );
  }


  শূন্য
  ft_hash_str_free( FT_Hash    hash,
                    FT_Memory  memory )
  {
    যদি ( hash )
    {
      FT_UInt       sz = hash->size;
      FT_Hashnode*  bp = hash->table;
      FT_UInt       i;


      বাবে ( i = 0; i < sz; i++, bp++ )
        FT_FREE( *bp );

      FT_FREE( hash->table );
    }
  }


  /* `ft_hash_num_free' is the same as `ft_hash_str_free' */


  স্থির FT_Error
  hash_ঢোকাও( FT_Hashkey  চাবি,
               size_t      data,
               FT_Hash     hash,
               FT_Memory   memory,
               FT_Bool     overwrite )
  {
    FT_Hashnode   nn;
    FT_Hashnode*  bp    = hash_bucket( চাবি, hash );
    FT_Error      error = FT_Err_Ok;


    nn = *bp;
    যদি ( !nn )
    {
      যদি ( FT_QNEW( nn ) )
        গচ্ছ Exit;
      *bp = nn;

      nn->চাবি  = চাবি;
      nn->data = data;

      যদি ( hash->used >= hash->limit )
      {
        error = hash_rehash( hash, memory );
        যদি ( error )
          গচ্ছ Exit;
      }

      hash->used++;
    }
    অন্যথা যদি ( overwrite )
      nn->data = data;

  Exit:
    ঘূৰাই error;
  }


  FT_Error
  ft_hash_str_ঢোকাও( ধ্রুৱ ৱর্ণ*  চাবি,
                      size_t       data,
                      FT_Hash      hash,
                      FT_Memory    memory )
  {
    FT_Hashkey  hk;


    hk.str = চাবি;

    ঘূৰাই hash_ঢোকাও( hk, data, hash, memory, TRUE );
  }


  FT_Error
  ft_hash_অঙ্ক_ঢোকাও( FT_Int     অঙ্ক,
                      size_t     data,
                      FT_Hash    hash,
                      FT_Memory  memory )
  {
    FT_Hashkey  hk;


    hk.অঙ্ক = অঙ্ক;

    ঘূৰাই hash_ঢোকাও( hk, data, hash, memory, TRUE );
  }


  FT_Error
  ft_hash_str_ঢোকাও_no_overwrite( ধ্রুৱ ৱর্ণ*  চাবি,
                                   size_t       data,
                                   FT_Hash      hash,
                                   FT_Memory    memory )
  {
    FT_Hashkey  hk;


    hk.str = চাবি;

    ঘূৰাই hash_ঢোকাও( hk, data, hash, memory, FALSE );
  }


  FT_Error
  ft_hash_অঙ্ক_ঢোকাও_no_overwrite( FT_Int     অঙ্ক,
                                   size_t     data,
                                   FT_Hash    hash,
                                   FT_Memory  memory )
  {
    FT_Hashkey  hk;


    hk.অঙ্ক = অঙ্ক;

    ঘূৰাই hash_ঢোকাও( hk, data, hash, memory, FALSE );
  }


  স্থির size_t*
  hash_lookup( FT_Hashkey  চাবি,
               FT_Hash     hash )
  {
    FT_Hashnode*  np = hash_bucket( চাবি, hash );


    ঘূৰাই (*np) ? &(*np)->data
                 : রিক্ত;
  }


  size_t*
  ft_hash_str_lookup( ধ্রুৱ ৱর্ণ*  চাবি,
                      FT_Hash      hash )
  {
    FT_Hashkey  hk;


    hk.str = চাবি;

    ঘূৰাই hash_lookup( hk, hash );
  }


  size_t*
  ft_hash_অঙ্ক_lookup( FT_Int   অঙ্ক,
                      FT_Hash  hash )
  {
    FT_Hashkey  hk;


    hk.অঙ্ক = অঙ্ক;

    ঘূৰাই hash_lookup( hk, hash );
  }


  FT_Bool
  ft_hash_অঙ্ক_iterator( FT_UInt  *idx,
                        FT_Int   *চাবি,
                        size_t   *মান,
                        FT_Hash   hash )
  {
    FT_Hashnode  nn = রিক্ত;


    যেতিয়ালৈ ( 1 )
    {
      যদি ( *idx >= hash->size )
        ঘূৰাই 0;

      nn = hash->table[*idx];
      যদি ( nn )
        ভঙ্গ;

      (*idx)++;
    }

    যদি ( চাবি )
      *চাবি = nn->চাবি.অঙ্ক;
    যদি ( মান )
      *মান = nn->data;

    (*idx)++;

    ঘূৰাই 1;
  }


  FT_Bool
  ft_hash_str_iterator( FT_UInt      *idx,
                        ধ্রুৱ ৱর্ণ*  *চাবি,
                        size_t       *মান,
                        FT_Hash       hash )
  {
    FT_Hashnode  nn = রিক্ত;


    যেতিয়ালৈ ( 1 )
    {
      যদি ( *idx >= hash->size )
        ঘূৰাই 0;

      nn = hash->table[*idx];
      যদি ( nn )
        ভঙ্গ;

      (*idx)++;
    }

    যদি ( চাবি )
      *চাবি = nn->চাবি.str;
    যদি ( মান )
      *মান = nn->data;

    (*idx)++;

    ঘূৰাই 1;
  }


/* END */
