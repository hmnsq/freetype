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


#ଅନ୍ତର୍ଭାଵ <freetype/internal/fthash.h>
#ଅନ୍ତର୍ଭାଵ <freetype/internal/ftmemory.h>
#ଅନ୍ତର୍ଭାଵ <freetype/internal/ftobjs.h>


#define INITIAL_HT_SIZE  241


  ସ୍ଥିର FT_ULong
  hash_str_lookup( FT_Hashkey*  ଚାବି )
  {
    ଅଚଳ ଅକ୍ଷର*  kp  = ଚାବି->str;
    FT_ULong     res = 0;


    /* Mocklisp hash function. */
    ଯେତେବେଳେ ( *kp )
      res = ( res << 5 ) - res + (FT_ULong)*kp++;

    ଫେରନ୍ତୁ res;
  }


  ସ୍ଥିର FT_ULong
  hash_ଅଙ୍କ_lookup( FT_Hashkey*  ଚାବି )
  {
    FT_ULong  ଅଙ୍କ = (FT_ULong)ଚାବି->ଅଙ୍କ;
    FT_ULong  res;


    /* Mocklisp hash function. */
    res = ଅଙ୍କ & 0xFF;
    res = ( res << 5 ) - res + ( ( ଅଙ୍କ >>  8 ) & 0xFF );
    res = ( res << 5 ) - res + ( ( ଅଙ୍କ >> 16 ) & 0xFF );
    res = ( res << 5 ) - res + ( ( ଅଙ୍କ >> 24 ) & 0xFF );

    ଫେରନ୍ତୁ res;
  }


  ସ୍ଥିର FT_Bool
  hash_str_compare( FT_Hashkey*  a,
                    FT_Hashkey*  b )
  {
    ଯଦି ( a->str[0] == b->str[0]           &&
         ft_strcmp( a->str, b->str ) == 0 )
      ଫେରନ୍ତୁ 1;

    ଫେରନ୍ତୁ 0;
  }


  ସ୍ଥିର FT_Bool
  hash_ଅଙ୍କ_compare( FT_Hashkey*  a,
                    FT_Hashkey*  b )
  {
    ଯଦି ( a->ଅଙ୍କ == b->ଅଙ୍କ )
      ଫେରନ୍ତୁ 1;

    ଫେରନ୍ତୁ 0;
  }


  ସ୍ଥିର FT_Hashnode*
  hash_bucket( FT_Hashkey  ଚାବି,
               FT_Hash     hash )
  {
    FT_ULong      res = 0;
    FT_Hashnode*  bp  = hash->table;
    FT_Hashnode*  ndp;


    res = (hash->lookup)( &ଚାବି );

    ndp = bp + ( res % hash->size );
    ଯେତେବେଳେ ( *ndp )
    {
      ଯଦି ( (hash->compare)( &(*ndp)->ଚାବି, &ଚାବି ) )
        ଭାଙ୍ଗ;

      ndp--;
      ଯଦି ( ndp < bp )
        ndp = bp + ( hash->size - 1 );
    }

    ଫେରନ୍ତୁ ndp;
  }


  ସ୍ଥିର FT_Error
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

    ଯଦି ( FT_NEW_ARRAY( hash->table, hash->size ) )
      ଯାଅ Exit;

    ପାଇଁ ( i = 0, bp = obp; i < sz; i++, bp++ )
    {
      ଯଦି ( *bp )
      {
        nbp = hash_bucket( (*bp)->ଚାବି, hash );
        *nbp = *bp;
      }
    }

    FT_FREE( obp );

  Exit:
    ଫେରନ୍ତୁ error;
  }


  ସ୍ଥିର FT_Error
  hash_init( FT_Hash    hash,
             FT_Bool    is_ଅଙ୍କ,
             FT_Memory  memory )
  {
    FT_UInt   sz = INITIAL_HT_SIZE;
    FT_Error  error;


    hash->size  = sz;
    hash->limit = sz / 3;
    hash->used  = 0;

    ଯଦି ( is_ଅଙ୍କ )
    {
      hash->lookup  = hash_ଅଙ୍କ_lookup;
      hash->compare = hash_ଅଙ୍କ_compare;
    }
    ଅନ୍ୟଥା
    {
      hash->lookup  = hash_str_lookup;
      hash->compare = hash_str_compare;
    }

    FT_MEM_NEW_ARRAY( hash->table, sz );

    ଫେରନ୍ତୁ error;
  }


  FT_Error
  ft_hash_str_init( FT_Hash    hash,
                    FT_Memory  memory )
  {
    ଫେରନ୍ତୁ hash_init( hash, 0, memory );
  }


  FT_Error
  ft_hash_ଅଙ୍କ_init( FT_Hash    hash,
                    FT_Memory  memory )
  {
    ଫେରନ୍ତୁ hash_init( hash, 1, memory );
  }


  ଶୂନ୍ୟ
  ft_hash_str_free( FT_Hash    hash,
                    FT_Memory  memory )
  {
    ଯଦି ( hash )
    {
      FT_UInt       sz = hash->size;
      FT_Hashnode*  bp = hash->table;
      FT_UInt       i;


      ପାଇଁ ( i = 0; i < sz; i++, bp++ )
        FT_FREE( *bp );

      FT_FREE( hash->table );
    }
  }


  /* `ft_hash_num_free' is the same as `ft_hash_str_free' */


  ସ୍ଥିର FT_Error
  hash_ଭର୍ତ୍ତି( FT_Hashkey  ଚାବି,
               size_t      data,
               FT_Hash     hash,
               FT_Memory   memory,
               FT_Bool     overwrite )
  {
    FT_Hashnode   nn;
    FT_Hashnode*  bp    = hash_bucket( ଚାବି, hash );
    FT_Error      error = FT_Err_Ok;


    nn = *bp;
    ଯଦି ( !nn )
    {
      ଯଦି ( FT_QNEW( nn ) )
        ଯାଅ Exit;
      *bp = nn;

      nn->ଚାବି  = ଚାବି;
      nn->data = data;

      ଯଦି ( hash->used >= hash->limit )
      {
        error = hash_rehash( hash, memory );
        ଯଦି ( error )
          ଯାଅ Exit;
      }

      hash->used++;
    }
    ଅନ୍ୟଥା ଯଦି ( overwrite )
      nn->data = data;

  Exit:
    ଫେରନ୍ତୁ error;
  }


  FT_Error
  ft_hash_str_ଭର୍ତ୍ତି( ଅଚଳ ଅକ୍ଷର*  ଚାବି,
                      size_t       data,
                      FT_Hash      hash,
                      FT_Memory    memory )
  {
    FT_Hashkey  hk;


    hk.str = ଚାବି;

    ଫେରନ୍ତୁ hash_ଭର୍ତ୍ତି( hk, data, hash, memory, TRUE );
  }


  FT_Error
  ft_hash_ଅଙ୍କ_ଭର୍ତ୍ତି( FT_Int     ଅଙ୍କ,
                      size_t     data,
                      FT_Hash    hash,
                      FT_Memory  memory )
  {
    FT_Hashkey  hk;


    hk.ଅଙ୍କ = ଅଙ୍କ;

    ଫେରନ୍ତୁ hash_ଭର୍ତ୍ତି( hk, data, hash, memory, TRUE );
  }


  FT_Error
  ft_hash_str_ଭର୍ତ୍ତି_no_overwrite( ଅଚଳ ଅକ୍ଷର*  ଚାବି,
                                   size_t       data,
                                   FT_Hash      hash,
                                   FT_Memory    memory )
  {
    FT_Hashkey  hk;


    hk.str = ଚାବି;

    ଫେରନ୍ତୁ hash_ଭର୍ତ୍ତି( hk, data, hash, memory, FALSE );
  }


  FT_Error
  ft_hash_ଅଙ୍କ_ଭର୍ତ୍ତି_no_overwrite( FT_Int     ଅଙ୍କ,
                                   size_t     data,
                                   FT_Hash    hash,
                                   FT_Memory  memory )
  {
    FT_Hashkey  hk;


    hk.ଅଙ୍କ = ଅଙ୍କ;

    ଫେରନ୍ତୁ hash_ଭର୍ତ୍ତି( hk, data, hash, memory, FALSE );
  }


  ସ୍ଥିର size_t*
  hash_lookup( FT_Hashkey  ଚାବି,
               FT_Hash     hash )
  {
    FT_Hashnode*  np = hash_bucket( ଚାବି, hash );


    ଫେରନ୍ତୁ (*np) ? &(*np)->data
                 : ଖାଲି;
  }


  size_t*
  ft_hash_str_lookup( ଅଚଳ ଅକ୍ଷର*  ଚାବି,
                      FT_Hash      hash )
  {
    FT_Hashkey  hk;


    hk.str = ଚାବି;

    ଫେରନ୍ତୁ hash_lookup( hk, hash );
  }


  size_t*
  ft_hash_ଅଙ୍କ_lookup( FT_Int   ଅଙ୍କ,
                      FT_Hash  hash )
  {
    FT_Hashkey  hk;


    hk.ଅଙ୍କ = ଅଙ୍କ;

    ଫେରନ୍ତୁ hash_lookup( hk, hash );
  }


  FT_Bool
  ft_hash_ଅଙ୍କ_iterator( FT_UInt  *idx,
                        FT_Int   *ଚାବି,
                        size_t   *ମୂଲ୍ୟ,
                        FT_Hash   hash )
  {
    FT_Hashnode  nn = ଖାଲି;


    ଯେତେବେଳେ ( 1 )
    {
      ଯଦି ( *idx >= hash->size )
        ଫେରନ୍ତୁ 0;

      nn = hash->table[*idx];
      ଯଦି ( nn )
        ଭାଙ୍ଗ;

      (*idx)++;
    }

    ଯଦି ( ଚାବି )
      *ଚାବି = nn->ଚାବି.ଅଙ୍କ;
    ଯଦି ( ମୂଲ୍ୟ )
      *ମୂଲ୍ୟ = nn->data;

    (*idx)++;

    ଫେରନ୍ତୁ 1;
  }


  FT_Bool
  ft_hash_str_iterator( FT_UInt      *idx,
                        ଅଚଳ ଅକ୍ଷର*  *ଚାବି,
                        size_t       *ମୂଲ୍ୟ,
                        FT_Hash       hash )
  {
    FT_Hashnode  nn = ଖାଲି;


    ଯେତେବେଳେ ( 1 )
    {
      ଯଦି ( *idx >= hash->size )
        ଫେରନ୍ତୁ 0;

      nn = hash->table[*idx];
      ଯଦି ( nn )
        ଭାଙ୍ଗ;

      (*idx)++;
    }

    ଯଦି ( ଚାବି )
      *ଚାବି = nn->ଚାବି.str;
    ଯଦି ( ମୂଲ୍ୟ )
      *ମୂଲ୍ୟ = nn->data;

    (*idx)++;

    ଫେରନ୍ତୁ 1;
  }


/* END */
