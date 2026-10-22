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


#અન્તર્ભાવ <freetype/internal/fthash.h>
#અન્તર્ભાવ <freetype/internal/ftmemory.h>
#અન્તર્ભાવ <freetype/internal/ftobjs.h>


#define INITIAL_HT_SIZE  241


  સ્થિર FT_ULong
  hash_str_lookup( FT_Hashkey*  ચાવી )
  {
    અચલ અક્ષર*  kp  = ચાવી->str;
    FT_ULong     res = 0;


    /* Mocklisp hash function. */
    જ્યારે ( *kp )
      res = ( res << 5 ) - res + (FT_ULong)*kp++;

    પાછા res;
  }


  સ્થિર FT_ULong
  hash_અંક_lookup( FT_Hashkey*  ચાવી )
  {
    FT_ULong  અંક = (FT_ULong)ચાવી->અંક;
    FT_ULong  res;


    /* Mocklisp hash function. */
    res = અંક & 0xFF;
    res = ( res << 5 ) - res + ( ( અંક >>  8 ) & 0xFF );
    res = ( res << 5 ) - res + ( ( અંક >> 16 ) & 0xFF );
    res = ( res << 5 ) - res + ( ( અંક >> 24 ) & 0xFF );

    પાછા res;
  }


  સ્થિર FT_Bool
  hash_str_compare( FT_Hashkey*  a,
                    FT_Hashkey*  b )
  {
    જો ( a->str[0] == b->str[0]           &&
         ft_strcmp( a->str, b->str ) == 0 )
      પાછા 1;

    પાછા 0;
  }


  સ્થિર FT_Bool
  hash_અંક_compare( FT_Hashkey*  a,
                    FT_Hashkey*  b )
  {
    જો ( a->અંક == b->અંક )
      પાછા 1;

    પાછા 0;
  }


  સ્થિર FT_Hashnode*
  hash_bucket( FT_Hashkey  ચાવી,
               FT_Hash     hash )
  {
    FT_ULong      res = 0;
    FT_Hashnode*  bp  = hash->table;
    FT_Hashnode*  ndp;


    res = (hash->lookup)( &ચાવી );

    ndp = bp + ( res % hash->size );
    જ્યારે ( *ndp )
    {
      જો ( (hash->compare)( &(*ndp)->ચાવી, &ચાવી ) )
        તોડો;

      ndp--;
      જો ( ndp < bp )
        ndp = bp + ( hash->size - 1 );
    }

    પાછા ndp;
  }


  સ્થિર FT_Error
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

    જો ( FT_NEW_ARRAY( hash->table, hash->size ) )
      જાઓ Exit;

    માટે ( i = 0, bp = obp; i < sz; i++, bp++ )
    {
      જો ( *bp )
      {
        nbp = hash_bucket( (*bp)->ચાવી, hash );
        *nbp = *bp;
      }
    }

    FT_FREE( obp );

  Exit:
    પાછા error;
  }


  સ્થિર FT_Error
  hash_init( FT_Hash    hash,
             FT_Bool    is_અંક,
             FT_Memory  memory )
  {
    FT_UInt   sz = INITIAL_HT_SIZE;
    FT_Error  error;


    hash->size  = sz;
    hash->limit = sz / 3;
    hash->used  = 0;

    જો ( is_અંક )
    {
      hash->lookup  = hash_અંક_lookup;
      hash->compare = hash_અંક_compare;
    }
    નહીંતર
    {
      hash->lookup  = hash_str_lookup;
      hash->compare = hash_str_compare;
    }

    FT_MEM_NEW_ARRAY( hash->table, sz );

    પાછા error;
  }


  FT_Error
  ft_hash_str_init( FT_Hash    hash,
                    FT_Memory  memory )
  {
    પાછા hash_init( hash, 0, memory );
  }


  FT_Error
  ft_hash_અંક_init( FT_Hash    hash,
                    FT_Memory  memory )
  {
    પાછા hash_init( hash, 1, memory );
  }


  શૂન્ય
  ft_hash_str_free( FT_Hash    hash,
                    FT_Memory  memory )
  {
    જો ( hash )
    {
      FT_UInt       sz = hash->size;
      FT_Hashnode*  bp = hash->table;
      FT_UInt       i;


      માટે ( i = 0; i < sz; i++, bp++ )
        FT_FREE( *bp );

      FT_FREE( hash->table );
    }
  }


  /* `ft_hash_num_free' is the same as `ft_hash_str_free' */


  સ્થિર FT_Error
  hash_ઉમેરો( FT_Hashkey  ચાવી,
               size_t      data,
               FT_Hash     hash,
               FT_Memory   memory,
               FT_Bool     overwrite )
  {
    FT_Hashnode   nn;
    FT_Hashnode*  bp    = hash_bucket( ચાવી, hash );
    FT_Error      error = FT_Err_Ok;


    nn = *bp;
    જો ( !nn )
    {
      જો ( FT_QNEW( nn ) )
        જાઓ Exit;
      *bp = nn;

      nn->ચાવી  = ચાવી;
      nn->data = data;

      જો ( hash->used >= hash->limit )
      {
        error = hash_rehash( hash, memory );
        જો ( error )
          જાઓ Exit;
      }

      hash->used++;
    }
    નહીંતર જો ( overwrite )
      nn->data = data;

  Exit:
    પાછા error;
  }


  FT_Error
  ft_hash_str_ઉમેરો( અચલ અક્ષર*  ચાવી,
                      size_t       data,
                      FT_Hash      hash,
                      FT_Memory    memory )
  {
    FT_Hashkey  hk;


    hk.str = ચાવી;

    પાછા hash_ઉમેરો( hk, data, hash, memory, TRUE );
  }


  FT_Error
  ft_hash_અંક_ઉમેરો( FT_Int     અંક,
                      size_t     data,
                      FT_Hash    hash,
                      FT_Memory  memory )
  {
    FT_Hashkey  hk;


    hk.અંક = અંક;

    પાછા hash_ઉમેરો( hk, data, hash, memory, TRUE );
  }


  FT_Error
  ft_hash_str_ઉમેરો_no_overwrite( અચલ અક્ષર*  ચાવી,
                                   size_t       data,
                                   FT_Hash      hash,
                                   FT_Memory    memory )
  {
    FT_Hashkey  hk;


    hk.str = ચાવી;

    પાછા hash_ઉમેરો( hk, data, hash, memory, FALSE );
  }


  FT_Error
  ft_hash_અંક_ઉમેરો_no_overwrite( FT_Int     અંક,
                                   size_t     data,
                                   FT_Hash    hash,
                                   FT_Memory  memory )
  {
    FT_Hashkey  hk;


    hk.અંક = અંક;

    પાછા hash_ઉમેરો( hk, data, hash, memory, FALSE );
  }


  સ્થિર size_t*
  hash_lookup( FT_Hashkey  ચાવી,
               FT_Hash     hash )
  {
    FT_Hashnode*  np = hash_bucket( ચાવી, hash );


    પાછા (*np) ? &(*np)->data
                 : ખાલી;
  }


  size_t*
  ft_hash_str_lookup( અચલ અક્ષર*  ચાવી,
                      FT_Hash      hash )
  {
    FT_Hashkey  hk;


    hk.str = ચાવી;

    પાછા hash_lookup( hk, hash );
  }


  size_t*
  ft_hash_અંક_lookup( FT_Int   અંક,
                      FT_Hash  hash )
  {
    FT_Hashkey  hk;


    hk.અંક = અંક;

    પાછા hash_lookup( hk, hash );
  }


  FT_Bool
  ft_hash_અંક_iterator( FT_UInt  *idx,
                        FT_Int   *ચાવી,
                        size_t   *કિંમત,
                        FT_Hash   hash )
  {
    FT_Hashnode  nn = ખાલી;


    જ્યારે ( 1 )
    {
      જો ( *idx >= hash->size )
        પાછા 0;

      nn = hash->table[*idx];
      જો ( nn )
        તોડો;

      (*idx)++;
    }

    જો ( ચાવી )
      *ચાવી = nn->ચાવી.અંક;
    જો ( કિંમત )
      *કિંમત = nn->data;

    (*idx)++;

    પાછા 1;
  }


  FT_Bool
  ft_hash_str_iterator( FT_UInt      *idx,
                        અચલ અક્ષર*  *ચાવી,
                        size_t       *કિંમત,
                        FT_Hash       hash )
  {
    FT_Hashnode  nn = ખાલી;


    જ્યારે ( 1 )
    {
      જો ( *idx >= hash->size )
        પાછા 0;

      nn = hash->table[*idx];
      જો ( nn )
        તોડો;

      (*idx)++;
    }

    જો ( ચાવી )
      *ચાવી = nn->ચાવી.str;
    જો ( કિંમત )
      *કિંમત = nn->data;

    (*idx)++;

    પાછા 1;
  }


/* END */
