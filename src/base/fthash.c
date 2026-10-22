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


#അന്തര്ഭാവ <freetype/internal/fthash.h>
#അന്തര്ഭാവ <freetype/internal/ftmemory.h>
#അന്തര്ഭാവ <freetype/internal/ftobjs.h>


#define INITIAL_HT_SIZE  241


  സ്ഥിതം FT_ULong
  hash_str_lookup( FT_Hashkey*  താക്കോൽ )
  {
    സ്ഥിരം അക്ഷരം*  kp  = താക്കോൽ->str;
    FT_ULong     res = 0;


    /* Mocklisp hash function. */
    ആകുമ്പോൾ ( *kp )
      res = ( res << 5 ) - res + (FT_ULong)*kp++;

    തിരികെ res;
  }


  സ്ഥിതം FT_ULong
  hash_അക്കം_lookup( FT_Hashkey*  താക്കോൽ )
  {
    FT_ULong  അക്കം = (FT_ULong)താക്കോൽ->അക്കം;
    FT_ULong  res;


    /* Mocklisp hash function. */
    res = അക്കം & 0xFF;
    res = ( res << 5 ) - res + ( ( അക്കം >>  8 ) & 0xFF );
    res = ( res << 5 ) - res + ( ( അക്കം >> 16 ) & 0xFF );
    res = ( res << 5 ) - res + ( ( അക്കം >> 24 ) & 0xFF );

    തിരികെ res;
  }


  സ്ഥിതം FT_Bool
  hash_str_compare( FT_Hashkey*  a,
                    FT_Hashkey*  b )
  {
    എങ്കിൽ ( a->str[0] == b->str[0]           &&
         ft_strcmp( a->str, b->str ) == 0 )
      തിരികെ 1;

    തിരികെ 0;
  }


  സ്ഥിതം FT_Bool
  hash_അക്കം_compare( FT_Hashkey*  a,
                    FT_Hashkey*  b )
  {
    എങ്കിൽ ( a->അക്കം == b->അക്കം )
      തിരികെ 1;

    തിരികെ 0;
  }


  സ്ഥിതം FT_Hashnode*
  hash_bucket( FT_Hashkey  താക്കോൽ,
               FT_Hash     hash )
  {
    FT_ULong      res = 0;
    FT_Hashnode*  bp  = hash->table;
    FT_Hashnode*  ndp;


    res = (hash->lookup)( &താക്കോൽ );

    ndp = bp + ( res % hash->size );
    ആകുമ്പോൾ ( *ndp )
    {
      എങ്കിൽ ( (hash->compare)( &(*ndp)->താക്കോൽ, &താക്കോൽ ) )
        നിർത്തുക;

      ndp--;
      എങ്കിൽ ( ndp < bp )
        ndp = bp + ( hash->size - 1 );
    }

    തിരികെ ndp;
  }


  സ്ഥിതം FT_Error
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

    എങ്കിൽ ( FT_NEW_ARRAY( hash->table, hash->size ) )
      പോകുക Exit;

    വേണ്ടി ( i = 0, bp = obp; i < sz; i++, bp++ )
    {
      എങ്കിൽ ( *bp )
      {
        nbp = hash_bucket( (*bp)->താക്കോൽ, hash );
        *nbp = *bp;
      }
    }

    FT_FREE( obp );

  Exit:
    തിരികെ error;
  }


  സ്ഥിതം FT_Error
  hash_init( FT_Hash    hash,
             FT_Bool    is_അക്കം,
             FT_Memory  memory )
  {
    FT_UInt   sz = INITIAL_HT_SIZE;
    FT_Error  error;


    hash->size  = sz;
    hash->limit = sz / 3;
    hash->used  = 0;

    എങ്കിൽ ( is_അക്കം )
    {
      hash->lookup  = hash_അക്കം_lookup;
      hash->compare = hash_അക്കം_compare;
    }
    അല്ലെങ്കിൽ
    {
      hash->lookup  = hash_str_lookup;
      hash->compare = hash_str_compare;
    }

    FT_MEM_NEW_ARRAY( hash->table, sz );

    തിരികെ error;
  }


  FT_Error
  ft_hash_str_init( FT_Hash    hash,
                    FT_Memory  memory )
  {
    തിരികെ hash_init( hash, 0, memory );
  }


  FT_Error
  ft_hash_അക്കം_init( FT_Hash    hash,
                    FT_Memory  memory )
  {
    തിരികെ hash_init( hash, 1, memory );
  }


  ശൂന്യം
  ft_hash_str_free( FT_Hash    hash,
                    FT_Memory  memory )
  {
    എങ്കിൽ ( hash )
    {
      FT_UInt       sz = hash->size;
      FT_Hashnode*  bp = hash->table;
      FT_UInt       i;


      വേണ്ടി ( i = 0; i < sz; i++, bp++ )
        FT_FREE( *bp );

      FT_FREE( hash->table );
    }
  }


  /* `ft_hash_num_free' is the same as `ft_hash_str_free' */


  സ്ഥിതം FT_Error
  hash_തിരുകുക( FT_Hashkey  താക്കോൽ,
               size_t      data,
               FT_Hash     hash,
               FT_Memory   memory,
               FT_Bool     overwrite )
  {
    FT_Hashnode   nn;
    FT_Hashnode*  bp    = hash_bucket( താക്കോൽ, hash );
    FT_Error      error = FT_Err_Ok;


    nn = *bp;
    എങ്കിൽ ( !nn )
    {
      എങ്കിൽ ( FT_QNEW( nn ) )
        പോകുക Exit;
      *bp = nn;

      nn->താക്കോൽ  = താക്കോൽ;
      nn->data = data;

      എങ്കിൽ ( hash->used >= hash->limit )
      {
        error = hash_rehash( hash, memory );
        എങ്കിൽ ( error )
          പോകുക Exit;
      }

      hash->used++;
    }
    അല്ലെങ്കിൽ എങ്കിൽ ( overwrite )
      nn->data = data;

  Exit:
    തിരികെ error;
  }


  FT_Error
  ft_hash_str_തിരുകുക( സ്ഥിരം അക്ഷരം*  താക്കോൽ,
                      size_t       data,
                      FT_Hash      hash,
                      FT_Memory    memory )
  {
    FT_Hashkey  hk;


    hk.str = താക്കോൽ;

    തിരികെ hash_തിരുകുക( hk, data, hash, memory, TRUE );
  }


  FT_Error
  ft_hash_അക്കം_തിരുകുക( FT_Int     അക്കം,
                      size_t     data,
                      FT_Hash    hash,
                      FT_Memory  memory )
  {
    FT_Hashkey  hk;


    hk.അക്കം = അക്കം;

    തിരികെ hash_തിരുകുക( hk, data, hash, memory, TRUE );
  }


  FT_Error
  ft_hash_str_തിരുകുക_no_overwrite( സ്ഥിരം അക്ഷരം*  താക്കോൽ,
                                   size_t       data,
                                   FT_Hash      hash,
                                   FT_Memory    memory )
  {
    FT_Hashkey  hk;


    hk.str = താക്കോൽ;

    തിരികെ hash_തിരുകുക( hk, data, hash, memory, FALSE );
  }


  FT_Error
  ft_hash_അക്കം_തിരുകുക_no_overwrite( FT_Int     അക്കം,
                                   size_t     data,
                                   FT_Hash    hash,
                                   FT_Memory  memory )
  {
    FT_Hashkey  hk;


    hk.അക്കം = അക്കം;

    തിരികെ hash_തിരുകുക( hk, data, hash, memory, FALSE );
  }


  സ്ഥിതം size_t*
  hash_lookup( FT_Hashkey  താക്കോൽ,
               FT_Hash     hash )
  {
    FT_Hashnode*  np = hash_bucket( താക്കോൽ, hash );


    തിരികെ (*np) ? &(*np)->data
                 : ശൂന്യ;
  }


  size_t*
  ft_hash_str_lookup( സ്ഥിരം അക്ഷരം*  താക്കോൽ,
                      FT_Hash      hash )
  {
    FT_Hashkey  hk;


    hk.str = താക്കോൽ;

    തിരികെ hash_lookup( hk, hash );
  }


  size_t*
  ft_hash_അക്കം_lookup( FT_Int   അക്കം,
                      FT_Hash  hash )
  {
    FT_Hashkey  hk;


    hk.അക്കം = അക്കം;

    തിരികെ hash_lookup( hk, hash );
  }


  FT_Bool
  ft_hash_അക്കം_iterator( FT_UInt  *idx,
                        FT_Int   *താക്കോൽ,
                        size_t   *മൂല്യം,
                        FT_Hash   hash )
  {
    FT_Hashnode  nn = ശൂന്യ;


    ആകുമ്പോൾ ( 1 )
    {
      എങ്കിൽ ( *idx >= hash->size )
        തിരികെ 0;

      nn = hash->table[*idx];
      എങ്കിൽ ( nn )
        നിർത്തുക;

      (*idx)++;
    }

    എങ്കിൽ ( താക്കോൽ )
      *താക്കോൽ = nn->താക്കോൽ.അക്കം;
    എങ്കിൽ ( മൂല്യം )
      *മൂല്യം = nn->data;

    (*idx)++;

    തിരികെ 1;
  }


  FT_Bool
  ft_hash_str_iterator( FT_UInt      *idx,
                        സ്ഥിരം അക്ഷരം*  *താക്കോൽ,
                        size_t       *മൂല്യം,
                        FT_Hash       hash )
  {
    FT_Hashnode  nn = ശൂന്യ;


    ആകുമ്പോൾ ( 1 )
    {
      എങ്കിൽ ( *idx >= hash->size )
        തിരികെ 0;

      nn = hash->table[*idx];
      എങ്കിൽ ( nn )
        നിർത്തുക;

      (*idx)++;
    }

    എങ്കിൽ ( താക്കോൽ )
      *താക്കോൽ = nn->താക്കോൽ.str;
    എങ്കിൽ ( മൂല്യം )
      *മൂല്യം = nn->data;

    (*idx)++;

    തിരികെ 1;
  }


/* END */
