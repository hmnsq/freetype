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


#समावेश <freetype/internal/fthash.h>
#समावेश <freetype/internal/ftmemory.h>
#समावेश <freetype/internal/ftobjs.h>


#घोषणा INITIAL_HT_आकार  241


  अटल FT_ULong
  hash_str_lookup( FT_Hashkey*  कुंजी )
  {
    स्थिर अक्षर*  kp  = कुंजी->str;
    FT_ULong     res = 0;


    /* Mocklisp hash function. */
    जबतक ( *kp )
      res = ( res << 5 ) - res + (FT_ULong)*kp++;

    वापस res;
  }


  अटल FT_ULong
  hash_अंक_lookup( FT_Hashkey*  कुंजी )
  {
    FT_ULong  अंक = (FT_ULong)कुंजी->अंक;
    FT_ULong  res;


    /* Mocklisp hash function. */
    res = अंक & 0xFF;
    res = ( res << 5 ) - res + ( ( अंक >>  8 ) & 0xFF );
    res = ( res << 5 ) - res + ( ( अंक >> 16 ) & 0xFF );
    res = ( res << 5 ) - res + ( ( अंक >> 24 ) & 0xFF );

    वापस res;
  }


  अटल FT_Bool
  hash_str_compare( FT_Hashkey*  a,
                    FT_Hashkey*  b )
  {
    अगर ( a->str[0] == b->str[0]           &&
         ft_strcmp( a->str, b->str ) == 0 )
      वापस 1;

    वापस 0;
  }


  अटल FT_Bool
  hash_अंक_compare( FT_Hashkey*  a,
                    FT_Hashkey*  b )
  {
    अगर ( a->अंक == b->अंक )
      वापस 1;

    वापस 0;
  }


  अटल FT_Hashnode*
  hash_bucket( FT_Hashkey  कुंजी,
               FT_Hash     hash )
  {
    FT_ULong      res = 0;
    FT_Hashnode*  bp  = hash->table;
    FT_Hashnode*  ndp;


    res = (hash->lookup)( &कुंजी );

    ndp = bp + ( res % hash->आकार );
    जबतक ( *ndp )
    {
      अगर ( (hash->compare)( &(*ndp)->कुंजी, &कुंजी ) )
        अवरोध;

      ndp--;
      अगर ( ndp < bp )
        ndp = bp + ( hash->आकार - 1 );
    }

    वापस ndp;
  }


  अटल FT_Error
  hash_rehash( FT_Hash    hash,
               FT_Memory  memory )
  {
    FT_Hashnode*  obp = hash->table;
    FT_Hashnode*  bp;
    FT_Hashnode*  nbp;

    FT_UInt   i, sz = hash->आकार;
    FT_Error  error = FT_Err_Ok;


    hash->आकार <<= 1;
    hash->limit  = hash->आकार / 3;

    अगर ( FT_NEW_ARRAY( hash->table, hash->आकार ) )
      जाओ Exit;

    क्रम ( i = 0, bp = obp; i < sz; i++, bp++ )
    {
      अगर ( *bp )
      {
        nbp = hash_bucket( (*bp)->कुंजी, hash );
        *nbp = *bp;
      }
    }

    FT_FREE( obp );

  Exit:
    वापस error;
  }


  अटल FT_Error
  hash_init( FT_Hash    hash,
             FT_Bool    is_अंक,
             FT_Memory  memory )
  {
    FT_UInt   sz = INITIAL_HT_आकार;
    FT_Error  error;


    hash->आकार  = sz;
    hash->limit = sz / 3;
    hash->used  = 0;

    अगर ( is_अंक )
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

    वापस error;
  }


  FT_Error
  ft_hash_str_init( FT_Hash    hash,
                    FT_Memory  memory )
  {
    वापस hash_init( hash, 0, memory );
  }


  FT_Error
  ft_hash_अंक_init( FT_Hash    hash,
                    FT_Memory  memory )
  {
    वापस hash_init( hash, 1, memory );
  }


  व्योम
  ft_hash_str_free( FT_Hash    hash,
                    FT_Memory  memory )
  {
    अगर ( hash )
    {
      FT_UInt       sz = hash->आकार;
      FT_Hashnode*  bp = hash->table;
      FT_UInt       i;


      क्रम ( i = 0; i < sz; i++, bp++ )
        FT_FREE( *bp );

      FT_FREE( hash->table );
    }
  }


  /* `ft_hash_num_free' is the same as `ft_hash_str_free' */


  अटल FT_Error
  hash_जोड़ो( FT_Hashkey  कुंजी,
               माप_प्रकार      तथ्य,
               FT_Hash     hash,
               FT_Memory   memory,
               FT_Bool     overwrite )
  {
    FT_Hashnode   nn;
    FT_Hashnode*  bp    = hash_bucket( कुंजी, hash );
    FT_Error      error = FT_Err_Ok;


    nn = *bp;
    अगर ( !nn )
    {
      अगर ( FT_QNEW( nn ) )
        जाओ Exit;
      *bp = nn;

      nn->कुंजी  = कुंजी;
      nn->तथ्य = तथ्य;

      अगर ( hash->used >= hash->limit )
      {
        error = hash_rehash( hash, memory );
        अगर ( error )
          जाओ Exit;
      }

      hash->used++;
    }
    अन्यथा अगर ( overwrite )
      nn->तथ्य = तथ्य;

  Exit:
    वापस error;
  }


  FT_Error
  ft_hash_str_जोड़ो( स्थिर अक्षर*  कुंजी,
                      माप_प्रकार       तथ्य,
                      FT_Hash      hash,
                      FT_Memory    memory )
  {
    FT_Hashkey  hk;


    hk.str = कुंजी;

    वापस hash_जोड़ो( hk, तथ्य, hash, memory, TRUE );
  }


  FT_Error
  ft_hash_अंक_जोड़ो( FT_Int     अंक,
                      माप_प्रकार     तथ्य,
                      FT_Hash    hash,
                      FT_Memory  memory )
  {
    FT_Hashkey  hk;


    hk.अंक = अंक;

    वापस hash_जोड़ो( hk, तथ्य, hash, memory, TRUE );
  }


  FT_Error
  ft_hash_str_जोड़ो_no_overwrite( स्थिर अक्षर*  कुंजी,
                                   माप_प्रकार       तथ्य,
                                   FT_Hash      hash,
                                   FT_Memory    memory )
  {
    FT_Hashkey  hk;


    hk.str = कुंजी;

    वापस hash_जोड़ो( hk, तथ्य, hash, memory, FALSE );
  }


  FT_Error
  ft_hash_अंक_जोड़ो_no_overwrite( FT_Int     अंक,
                                   माप_प्रकार     तथ्य,
                                   FT_Hash    hash,
                                   FT_Memory  memory )
  {
    FT_Hashkey  hk;


    hk.अंक = अंक;

    वापस hash_जोड़ो( hk, तथ्य, hash, memory, FALSE );
  }


  अटल माप_प्रकार*
  hash_lookup( FT_Hashkey  कुंजी,
               FT_Hash     hash )
  {
    FT_Hashnode*  np = hash_bucket( कुंजी, hash );


    वापस (*np) ? &(*np)->तथ्य
                 : शून्य;
  }


  माप_प्रकार*
  ft_hash_str_lookup( स्थिर अक्षर*  कुंजी,
                      FT_Hash      hash )
  {
    FT_Hashkey  hk;


    hk.str = कुंजी;

    वापस hash_lookup( hk, hash );
  }


  माप_प्रकार*
  ft_hash_अंक_lookup( FT_Int   अंक,
                      FT_Hash  hash )
  {
    FT_Hashkey  hk;


    hk.अंक = अंक;

    वापस hash_lookup( hk, hash );
  }


  FT_Bool
  ft_hash_अंक_iterator( FT_UInt  *idx,
                        FT_Int   *कुंजी,
                        माप_प्रकार   *मूल्य,
                        FT_Hash   hash )
  {
    FT_Hashnode  nn = शून्य;


    जबतक ( 1 )
    {
      अगर ( *idx >= hash->आकार )
        वापस 0;

      nn = hash->table[*idx];
      अगर ( nn )
        अवरोध;

      (*idx)++;
    }

    अगर ( कुंजी )
      *कुंजी = nn->कुंजी.अंक;
    अगर ( मूल्य )
      *मूल्य = nn->तथ्य;

    (*idx)++;

    वापस 1;
  }


  FT_Bool
  ft_hash_str_iterator( FT_UInt      *idx,
                        स्थिर अक्षर*  *कुंजी,
                        माप_प्रकार       *मूल्य,
                        FT_Hash       hash )
  {
    FT_Hashnode  nn = शून्य;


    जबतक ( 1 )
    {
      अगर ( *idx >= hash->आकार )
        वापस 0;

      nn = hash->table[*idx];
      अगर ( nn )
        अवरोध;

      (*idx)++;
    }

    अगर ( कुंजी )
      *कुंजी = nn->कुंजी.str;
    अगर ( मूल्य )
      *मूल्य = nn->तथ्य;

    (*idx)++;

    वापस 1;
  }


/* END */
