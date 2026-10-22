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


#ضمن <freetype/internal/fthash.h>
#ضمن <freetype/internal/ftmemory.h>
#ضمن <freetype/internal/ftobjs.h>


#وصف_ڪريو INITIAL_HT_سائيز  241


  ساكن FT_ULong
  hash_str_lookup( FT_Hashkey*  ڪنجي )
  {
    ثابت حرف*  kp  = ڪنجي->str;
    FT_ULong     res = 0;


    /* Mocklisp hash function. */
    طالما ( *kp )
      res = ( res << 5 ) - res + (FT_ULong)*kp++;

    أرجع res;
  }


  ساكن FT_ULong
  hash_انگ_lookup( FT_Hashkey*  ڪنجي )
  {
    FT_ULong  انگ = (FT_ULong)ڪنجي->انگ;
    FT_ULong  res;


    /* Mocklisp hash function. */
    res = انگ & 0xFF;
    res = ( res << 5 ) - res + ( ( انگ >>  8 ) & 0xFF );
    res = ( res << 5 ) - res + ( ( انگ >> 16 ) & 0xFF );
    res = ( res << 5 ) - res + ( ( انگ >> 24 ) & 0xFF );

    أرجع res;
  }


  ساكن FT_Bool
  hash_str_compare( FT_Hashkey*  a,
                    FT_Hashkey*  b )
  {
    إذا ( a->str[0] == b->str[0]           &&
         ft_strcmp( a->str, b->str ) == 0 )
      أرجع 1;

    أرجع 0;
  }


  ساكن FT_Bool
  hash_انگ_compare( FT_Hashkey*  a,
                    FT_Hashkey*  b )
  {
    إذا ( a->انگ == b->انگ )
      أرجع 1;

    أرجع 0;
  }


  ساكن FT_Hashnode*
  hash_ٻالٽي( FT_Hashkey  ڪنجي,
               FT_Hash     hash )
  {
    FT_ULong      res = 0;
    FT_Hashnode*  bp  = hash->جدول;
    FT_Hashnode*  ndp;


    res = (hash->lookup)( &ڪنجي );

    ndp = bp + ( res % hash->سائيز );
    طالما ( *ndp )
    {
      إذا ( (hash->compare)( &(*ndp)->ڪنجي, &ڪنجي ) )
        اكسر;

      ndp--;
      إذا ( ndp < bp )
        ndp = bp + ( hash->سائيز - 1 );
    }

    أرجع ndp;
  }


  ساكن FT_Error
  hash_rehash( FT_Hash    hash,
               FT_يادگيري  يادگيري )
  {
    FT_Hashnode*  obp = hash->جدول;
    FT_Hashnode*  bp;
    FT_Hashnode*  nbp;

    FT_UInt   i, sz = hash->سائيز;
    FT_Error  error = FT_Err_Ok;


    hash->سائيز <<= 1;
    hash->حد_بندي  = hash->سائيز / 3;

    إذا ( FT_NEW_ARRAY( hash->جدول, hash->سائيز ) )
      اذهب Exit;

    لكل ( i = 0, bp = obp; i < sz; i++, bp++ )
    {
      إذا ( *bp )
      {
        nbp = hash_ٻالٽي( (*bp)->ڪنجي, hash );
        *nbp = *bp;
      }
    }

    FT_FREE( obp );

  Exit:
    أرجع error;
  }


  ساكن FT_Error
  hash_init( FT_Hash    hash,
             FT_Bool    is_انگ,
             FT_يادگيري  يادگيري )
  {
    FT_UInt   sz = INITIAL_HT_سائيز;
    FT_Error  error;


    hash->سائيز  = sz;
    hash->حد_بندي = sz / 3;
    hash->استعمال_ٿيل  = 0;

    إذا ( is_انگ )
    {
      hash->lookup  = hash_انگ_lookup;
      hash->compare = hash_انگ_compare;
    }
    وإلا
    {
      hash->lookup  = hash_str_lookup;
      hash->compare = hash_str_compare;
    }

    FT_MEM_NEW_ARRAY( hash->جدول, sz );

    أرجع error;
  }


  FT_Error
  ft_hash_str_init( FT_Hash    hash,
                    FT_يادگيري  يادگيري )
  {
    أرجع hash_init( hash, 0, يادگيري );
  }


  FT_Error
  ft_hash_انگ_init( FT_Hash    hash,
                    FT_يادگيري  يادگيري )
  {
    أرجع hash_init( hash, 1, يادگيري );
  }


  فراغ
  ft_hash_str_free( FT_Hash    hash,
                    FT_يادگيري  يادگيري )
  {
    إذا ( hash )
    {
      FT_UInt       sz = hash->سائيز;
      FT_Hashnode*  bp = hash->جدول;
      FT_UInt       i;


      لكل ( i = 0; i < sz; i++, bp++ )
        FT_FREE( *bp );

      FT_FREE( hash->جدول );
    }
  }


  /* `ft_hash_num_free' is the same as `ft_hash_str_free' */


  ساكن FT_Error
  hash_وجهو( FT_Hashkey  ڪنجي,
               size_t      معلومات,
               FT_Hash     hash,
               FT_يادگيري   يادگيري,
               FT_Bool     overwrite )
  {
    FT_Hashnode   nn;
    FT_Hashnode*  bp    = hash_ٻالٽي( ڪنجي, hash );
    FT_Error      error = FT_Err_Ok;


    nn = *bp;
    إذا ( !nn )
    {
      إذا ( FT_QNEW( nn ) )
        اذهب Exit;
      *bp = nn;

      nn->ڪنجي  = ڪنجي;
      nn->معلومات = معلومات;

      إذا ( hash->استعمال_ٿيل >= hash->حد_بندي )
      {
        error = hash_rehash( hash, يادگيري );
        إذا ( error )
          اذهب Exit;
      }

      hash->استعمال_ٿيل++;
    }
    وإلا إذا ( overwrite )
      nn->معلومات = معلومات;

  Exit:
    أرجع error;
  }


  FT_Error
  ft_hash_str_وجهو( ثابت حرف*  ڪنجي,
                      size_t       معلومات,
                      FT_Hash      hash,
                      FT_يادگيري    يادگيري )
  {
    FT_Hashkey  hk;


    hk.str = ڪنجي;

    أرجع hash_وجهو( hk, معلومات, hash, يادگيري, TRUE );
  }


  FT_Error
  ft_hash_انگ_وجهو( FT_Int     انگ,
                      size_t     معلومات,
                      FT_Hash    hash,
                      FT_يادگيري  يادگيري )
  {
    FT_Hashkey  hk;


    hk.انگ = انگ;

    أرجع hash_وجهو( hk, معلومات, hash, يادگيري, TRUE );
  }


  FT_Error
  ft_hash_str_وجهو_no_overwrite( ثابت حرف*  ڪنجي,
                                   size_t       معلومات,
                                   FT_Hash      hash,
                                   FT_يادگيري    يادگيري )
  {
    FT_Hashkey  hk;


    hk.str = ڪنجي;

    أرجع hash_وجهو( hk, معلومات, hash, يادگيري, FALSE );
  }


  FT_Error
  ft_hash_انگ_وجهو_no_overwrite( FT_Int     انگ,
                                   size_t     معلومات,
                                   FT_Hash    hash,
                                   FT_يادگيري  يادگيري )
  {
    FT_Hashkey  hk;


    hk.انگ = انگ;

    أرجع hash_وجهو( hk, معلومات, hash, يادگيري, FALSE );
  }


  ساكن size_t*
  hash_lookup( FT_Hashkey  ڪنجي,
               FT_Hash     hash )
  {
    FT_Hashnode*  np = hash_ٻالٽي( ڪنجي, hash );


    أرجع (*np) ? &(*np)->معلومات
                 : عدم;
  }


  size_t*
  ft_hash_str_lookup( ثابت حرف*  ڪنجي,
                      FT_Hash      hash )
  {
    FT_Hashkey  hk;


    hk.str = ڪنجي;

    أرجع hash_lookup( hk, hash );
  }


  size_t*
  ft_hash_انگ_lookup( FT_Int   انگ,
                      FT_Hash  hash )
  {
    FT_Hashkey  hk;


    hk.انگ = انگ;

    أرجع hash_lookup( hk, hash );
  }


  FT_Bool
  ft_hash_انگ_iterator( FT_UInt  *idx,
                        FT_Int   *ڪنجي,
                        size_t   *قدر,
                        FT_Hash   hash )
  {
    FT_Hashnode  nn = عدم;


    طالما ( 1 )
    {
      إذا ( *idx >= hash->سائيز )
        أرجع 0;

      nn = hash->جدول[*idx];
      إذا ( nn )
        اكسر;

      (*idx)++;
    }

    إذا ( ڪنجي )
      *ڪنجي = nn->ڪنجي.انگ;
    إذا ( قدر )
      *قدر = nn->معلومات;

    (*idx)++;

    أرجع 1;
  }


  FT_Bool
  ft_hash_str_iterator( FT_UInt      *idx,
                        ثابت حرف*  *ڪنجي,
                        size_t       *قدر,
                        FT_Hash       hash )
  {
    FT_Hashnode  nn = عدم;


    طالما ( 1 )
    {
      إذا ( *idx >= hash->سائيز )
        أرجع 0;

      nn = hash->جدول[*idx];
      إذا ( nn )
        اكسر;

      (*idx)++;
    }

    إذا ( ڪنجي )
      *ڪنجي = nn->ڪنجي.str;
    إذا ( قدر )
      *قدر = nn->معلومات;

    (*idx)++;

    أرجع 1;
  }


/*پڇاڙي*/
