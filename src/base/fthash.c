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


#அந்தர்பாவ <freetype/internal/fthash.h>
#அந்தர்பாவ <freetype/internal/ftmemory.h>
#அந்தர்பாவ <freetype/internal/ftobjs.h>


#define INITIAL_HT_SIZE  241


  நிலையான FT_ULong
  hash_str_lookup( FT_Hashkey*  சாவி )
  {
    மாறிலி எழுத்து*  kp  = சாவி->str;
    FT_ULong     res = 0;


    /* Mocklisp hash function. */
    போது ( *kp )
      res = ( res << 5 ) - res + (FT_ULong)*kp++;

    திரும்பு res;
  }


  நிலையான FT_ULong
  hash_இலக்கம்_lookup( FT_Hashkey*  சாவி )
  {
    FT_ULong  இலக்கம் = (FT_ULong)சாவி->இலக்கம்;
    FT_ULong  res;


    /* Mocklisp hash function. */
    res = இலக்கம் & 0xFF;
    res = ( res << 5 ) - res + ( ( இலக்கம் >>  8 ) & 0xFF );
    res = ( res << 5 ) - res + ( ( இலக்கம் >> 16 ) & 0xFF );
    res = ( res << 5 ) - res + ( ( இலக்கம் >> 24 ) & 0xFF );

    திரும்பு res;
  }


  நிலையான FT_Bool
  hash_str_compare( FT_Hashkey*  a,
                    FT_Hashkey*  b )
  {
    எனில் ( a->str[0] == b->str[0]           &&
         ft_strcmp( a->str, b->str ) == 0 )
      திரும்பு 1;

    திரும்பு 0;
  }


  நிலையான FT_Bool
  hash_இலக்கம்_compare( FT_Hashkey*  a,
                    FT_Hashkey*  b )
  {
    எனில் ( a->இலக்கம் == b->இலக்கம் )
      திரும்பு 1;

    திரும்பு 0;
  }


  நிலையான FT_Hashnode*
  hash_bucket( FT_Hashkey  சாவி,
               FT_Hash     hash )
  {
    FT_ULong      res = 0;
    FT_Hashnode*  bp  = hash->table;
    FT_Hashnode*  ndp;


    res = (hash->lookup)( &சாவி );

    ndp = bp + ( res % hash->size );
    போது ( *ndp )
    {
      எனில் ( (hash->compare)( &(*ndp)->சாவி, &சாவி ) )
        நிறுத்து;

      ndp--;
      எனில் ( ndp < bp )
        ndp = bp + ( hash->size - 1 );
    }

    திரும்பு ndp;
  }


  நிலையான FT_Error
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

    எனில் ( FT_NEW_ARRAY( hash->table, hash->size ) )
      செல் Exit;

    க்கு ( i = 0, bp = obp; i < sz; i++, bp++ )
    {
      எனில் ( *bp )
      {
        nbp = hash_bucket( (*bp)->சாவி, hash );
        *nbp = *bp;
      }
    }

    FT_FREE( obp );

  Exit:
    திரும்பு error;
  }


  நிலையான FT_Error
  hash_init( FT_Hash    hash,
             FT_Bool    is_இலக்கம்,
             FT_Memory  memory )
  {
    FT_UInt   sz = INITIAL_HT_SIZE;
    FT_Error  error;


    hash->size  = sz;
    hash->limit = sz / 3;
    hash->used  = 0;

    எனில் ( is_இலக்கம் )
    {
      hash->lookup  = hash_இலக்கம்_lookup;
      hash->compare = hash_இலக்கம்_compare;
    }
    இல்லையெனில்
    {
      hash->lookup  = hash_str_lookup;
      hash->compare = hash_str_compare;
    }

    FT_MEM_NEW_ARRAY( hash->table, sz );

    திரும்பு error;
  }


  FT_Error
  ft_hash_str_init( FT_Hash    hash,
                    FT_Memory  memory )
  {
    திரும்பு hash_init( hash, 0, memory );
  }


  FT_Error
  ft_hash_இலக்கம்_init( FT_Hash    hash,
                    FT_Memory  memory )
  {
    திரும்பு hash_init( hash, 1, memory );
  }


  வெறுமை
  ft_hash_str_free( FT_Hash    hash,
                    FT_Memory  memory )
  {
    எனில் ( hash )
    {
      FT_UInt       sz = hash->size;
      FT_Hashnode*  bp = hash->table;
      FT_UInt       i;


      க்கு ( i = 0; i < sz; i++, bp++ )
        FT_FREE( *bp );

      FT_FREE( hash->table );
    }
  }


  /* `ft_hash_num_free' is the same as `ft_hash_str_free' */


  நிலையான FT_Error
  hash_செருகு( FT_Hashkey  சாவி,
               size_t      data,
               FT_Hash     hash,
               FT_Memory   memory,
               FT_Bool     overwrite )
  {
    FT_Hashnode   nn;
    FT_Hashnode*  bp    = hash_bucket( சாவி, hash );
    FT_Error      error = FT_Err_Ok;


    nn = *bp;
    எனில் ( !nn )
    {
      எனில் ( FT_QNEW( nn ) )
        செல் Exit;
      *bp = nn;

      nn->சாவி  = சாவி;
      nn->data = data;

      எனில் ( hash->used >= hash->limit )
      {
        error = hash_rehash( hash, memory );
        எனில் ( error )
          செல் Exit;
      }

      hash->used++;
    }
    இல்லையெனில் எனில் ( overwrite )
      nn->data = data;

  Exit:
    திரும்பு error;
  }


  FT_Error
  ft_hash_str_செருகு( மாறிலி எழுத்து*  சாவி,
                      size_t       data,
                      FT_Hash      hash,
                      FT_Memory    memory )
  {
    FT_Hashkey  hk;


    hk.str = சாவி;

    திரும்பு hash_செருகு( hk, data, hash, memory, TRUE );
  }


  FT_Error
  ft_hash_இலக்கம்_செருகு( FT_Int     இலக்கம்,
                      size_t     data,
                      FT_Hash    hash,
                      FT_Memory  memory )
  {
    FT_Hashkey  hk;


    hk.இலக்கம் = இலக்கம்;

    திரும்பு hash_செருகு( hk, data, hash, memory, TRUE );
  }


  FT_Error
  ft_hash_str_செருகு_no_overwrite( மாறிலி எழுத்து*  சாவி,
                                   size_t       data,
                                   FT_Hash      hash,
                                   FT_Memory    memory )
  {
    FT_Hashkey  hk;


    hk.str = சாவி;

    திரும்பு hash_செருகு( hk, data, hash, memory, FALSE );
  }


  FT_Error
  ft_hash_இலக்கம்_செருகு_no_overwrite( FT_Int     இலக்கம்,
                                   size_t     data,
                                   FT_Hash    hash,
                                   FT_Memory  memory )
  {
    FT_Hashkey  hk;


    hk.இலக்கம் = இலக்கம்;

    திரும்பு hash_செருகு( hk, data, hash, memory, FALSE );
  }


  நிலையான size_t*
  hash_lookup( FT_Hashkey  சாவி,
               FT_Hash     hash )
  {
    FT_Hashnode*  np = hash_bucket( சாவி, hash );


    திரும்பு (*np) ? &(*np)->data
                 : வெற்று;
  }


  size_t*
  ft_hash_str_lookup( மாறிலி எழுத்து*  சாவி,
                      FT_Hash      hash )
  {
    FT_Hashkey  hk;


    hk.str = சாவி;

    திரும்பு hash_lookup( hk, hash );
  }


  size_t*
  ft_hash_இலக்கம்_lookup( FT_Int   இலக்கம்,
                      FT_Hash  hash )
  {
    FT_Hashkey  hk;


    hk.இலக்கம் = இலக்கம்;

    திரும்பு hash_lookup( hk, hash );
  }


  FT_Bool
  ft_hash_இலக்கம்_iterator( FT_UInt  *idx,
                        FT_Int   *சாவி,
                        size_t   *மதிப்பு,
                        FT_Hash   hash )
  {
    FT_Hashnode  nn = வெற்று;


    போது ( 1 )
    {
      எனில் ( *idx >= hash->size )
        திரும்பு 0;

      nn = hash->table[*idx];
      எனில் ( nn )
        நிறுத்து;

      (*idx)++;
    }

    எனில் ( சாவி )
      *சாவி = nn->சாவி.இலக்கம்;
    எனில் ( மதிப்பு )
      *மதிப்பு = nn->data;

    (*idx)++;

    திரும்பு 1;
  }


  FT_Bool
  ft_hash_str_iterator( FT_UInt      *idx,
                        மாறிலி எழுத்து*  *சாவி,
                        size_t       *மதிப்பு,
                        FT_Hash       hash )
  {
    FT_Hashnode  nn = வெற்று;


    போது ( 1 )
    {
      எனில் ( *idx >= hash->size )
        திரும்பு 0;

      nn = hash->table[*idx];
      எனில் ( nn )
        நிறுத்து;

      (*idx)++;
    }

    எனில் ( சாவி )
      *சாவி = nn->சாவி.str;
    எனில் ( மதிப்பு )
      *மதிப்பு = nn->data;

    (*idx)++;

    திரும்பு 1;
  }


/* END */
