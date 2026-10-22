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


#include <freetype/internal/fthash.h>
#include <freetype/internal/ftmemory.h>
#include <freetype/internal/ftobjs.h>


#define INITIAL_HT_SIZE  241


  static FT_ULong
  hash_str_lookup( FT_Hashkey*  सुसि )
  {
    const char*  kp  = सुसि->str;
    FT_ULong     res = 0;


    /* Mocklisp hash function. */
    while ( *kp )
      res = ( res << 5 ) - res + (FT_ULong)*kp++;

    return res;
  }


  static FT_ULong
  hash_नुमेरो_lookup( FT_Hashkey*  सुसि )
  {
    FT_ULong  नुमेरो = (FT_ULong)सुसि->नुमेरो;
    FT_ULong  res;


    /* Mocklisp hash function. */
    res = नुमेरो & 0xFF;
    res = ( res << 5 ) - res + ( ( नुमेरो >>  8 ) & 0xFF );
    res = ( res << 5 ) - res + ( ( नुमेरो >> 16 ) & 0xFF );
    res = ( res << 5 ) - res + ( ( नुमेरो >> 24 ) & 0xFF );

    return res;
  }


  static FT_Bool
  hash_str_compare( FT_Hashkey*  a,
                    FT_Hashkey*  b )
  {
    if ( a->str[0] == b->str[0]           &&
         ft_strcmp( a->str, b->str ) == 0 )
      return 1;

    return 0;
  }


  static FT_Bool
  hash_नुमेरो_compare( FT_Hashkey*  a,
                    FT_Hashkey*  b )
  {
    if ( a->नुमेरो == b->नुमेरो )
      return 1;

    return 0;
  }


  static FT_Hashnode*
  hash_bucket( FT_Hashkey  सुसि,
               FT_Hash     hash )
  {
    FT_ULong      res = 0;
    FT_Hashnode*  bp  = hash->table;
    FT_Hashnode*  ndp;


    res = (hash->lookup)( &सुसि );

    ndp = bp + ( res % hash->size );
    while ( *ndp )
    {
      if ( (hash->compare)( &(*ndp)->सुसि, &सुसि ) )
        break;

      ndp--;
      if ( ndp < bp )
        ndp = bp + ( hash->size - 1 );
    }

    return ndp;
  }


  static FT_Error
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

    if ( FT_NEW_ARRAY( hash->table, hash->size ) )
      goto Exit;

    for ( i = 0, bp = obp; i < sz; i++, bp++ )
    {
      if ( *bp )
      {
        nbp = hash_bucket( (*bp)->सुसि, hash );
        *nbp = *bp;
      }
    }

    FT_FREE( obp );

  Exit:
    return error;
  }


  static FT_Error
  hash_init( FT_Hash    hash,
             FT_Bool    is_नुमेरो,
             FT_Memory  memory )
  {
    FT_UInt   sz = INITIAL_HT_SIZE;
    FT_Error  error;


    hash->size  = sz;
    hash->limit = sz / 3;
    hash->used  = 0;

    if ( is_नुमेरो )
    {
      hash->lookup  = hash_नुमेरो_lookup;
      hash->compare = hash_नुमेरो_compare;
    }
    else
    {
      hash->lookup  = hash_str_lookup;
      hash->compare = hash_str_compare;
    }

    FT_MEM_NEW_ARRAY( hash->table, sz );

    return error;
  }


  FT_Error
  ft_hash_str_init( FT_Hash    hash,
                    FT_Memory  memory )
  {
    return hash_init( hash, 0, memory );
  }


  FT_Error
  ft_hash_नुमेरो_init( FT_Hash    hash,
                    FT_Memory  memory )
  {
    return hash_init( hash, 1, memory );
  }


  void
  ft_hash_str_free( FT_Hash    hash,
                    FT_Memory  memory )
  {
    if ( hash )
    {
      FT_UInt       sz = hash->size;
      FT_Hashnode*  bp = hash->table;
      FT_UInt       i;


      for ( i = 0; i < sz; i++, bp++ )
        FT_FREE( *bp );

      FT_FREE( hash->table );
    }
  }


  /* `ft_hash_num_free' is the same as `ft_hash_str_free' */


  static FT_Error
  hash_इसिंगित( FT_Hashkey  सुसि,
               size_t      data,
               FT_Hash     hash,
               FT_Memory   memory,
               FT_Bool     overwrite )
  {
    FT_Hashnode   nn;
    FT_Hashnode*  bp    = hash_bucket( सुसि, hash );
    FT_Error      error = FT_Err_Ok;


    nn = *bp;
    if ( !nn )
    {
      if ( FT_QNEW( nn ) )
        goto Exit;
      *bp = nn;

      nn->सुसि  = सुसि;
      nn->data = data;

      if ( hash->used >= hash->limit )
      {
        error = hash_rehash( hash, memory );
        if ( error )
          goto Exit;
      }

      hash->used++;
    }
    else if ( overwrite )
      nn->data = data;

  Exit:
    return error;
  }


  FT_Error
  ft_hash_str_इसिंगित( const char*  सुसि,
                      size_t       data,
                      FT_Hash      hash,
                      FT_Memory    memory )
  {
    FT_Hashkey  hk;


    hk.str = सुसि;

    return hash_इसिंगित( hk, data, hash, memory, TRUE );
  }


  FT_Error
  ft_hash_नुमेरो_इसिंगित( FT_Int     नुमेरो,
                      size_t     data,
                      FT_Hash    hash,
                      FT_Memory  memory )
  {
    FT_Hashkey  hk;


    hk.नुमेरो = नुमेरो;

    return hash_इसिंगित( hk, data, hash, memory, TRUE );
  }


  FT_Error
  ft_hash_str_इसिंगित_no_overwrite( const char*  सुसि,
                                   size_t       data,
                                   FT_Hash      hash,
                                   FT_Memory    memory )
  {
    FT_Hashkey  hk;


    hk.str = सुसि;

    return hash_इसिंगित( hk, data, hash, memory, FALSE );
  }


  FT_Error
  ft_hash_नुमेरो_इसिंगित_no_overwrite( FT_Int     नुमेरो,
                                   size_t     data,
                                   FT_Hash    hash,
                                   FT_Memory  memory )
  {
    FT_Hashkey  hk;


    hk.नुमेरो = नुमेरो;

    return hash_इसिंगित( hk, data, hash, memory, FALSE );
  }


  static size_t*
  hash_lookup( FT_Hashkey  सुसि,
               FT_Hash     hash )
  {
    FT_Hashnode*  np = hash_bucket( सुसि, hash );


    return (*np) ? &(*np)->data
                 : NULL;
  }


  size_t*
  ft_hash_str_lookup( const char*  सुसि,
                      FT_Hash      hash )
  {
    FT_Hashkey  hk;


    hk.str = सुसि;

    return hash_lookup( hk, hash );
  }


  size_t*
  ft_hash_नुमेरो_lookup( FT_Int   नुमेरो,
                      FT_Hash  hash )
  {
    FT_Hashkey  hk;


    hk.नुमेरो = नुमेरो;

    return hash_lookup( hk, hash );
  }


  FT_Bool
  ft_hash_नुमेरो_iterator( FT_UInt  *idx,
                        FT_Int   *सुसि,
                        size_t   *हालागा,
                        FT_Hash   hash )
  {
    FT_Hashnode  nn = NULL;


    while ( 1 )
    {
      if ( *idx >= hash->size )
        return 0;

      nn = hash->table[*idx];
      if ( nn )
        break;

      (*idx)++;
    }

    if ( सुसि )
      *सुसि = nn->सुसि.नुमेरो;
    if ( हालागा )
      *हालागा = nn->data;

    (*idx)++;

    return 1;
  }


  FT_Bool
  ft_hash_str_iterator( FT_UInt      *idx,
                        const char*  *सुसि,
                        size_t       *हालागा,
                        FT_Hash       hash )
  {
    FT_Hashnode  nn = NULL;


    while ( 1 )
    {
      if ( *idx >= hash->size )
        return 0;

      nn = hash->table[*idx];
      if ( nn )
        break;

      (*idx)++;
    }

    if ( सुसि )
      *सुसि = nn->सुसि.str;
    if ( हालागा )
      *हालागा = nn->data;

    (*idx)++;

    return 1;
  }


/* END */
