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


#تعریف_کړه INITIAL_HT_اندازه  241


  ساكن FT_ULong
  hash_str_lookup( FT_Hashkey*  کلی )
  {
    ثابت حرف*  kp  = کلی->str;
    FT_ULong     res = 0;


    /* Mocklisp hash function. */
    طالما ( *kp )
      res = ( res << 5 ) - res + (FT_ULong)*kp++;

    أرجع res;
  }


  ساكن FT_ULong
  hash_عدد_lookup( FT_Hashkey*  کلی )
  {
    FT_ULong  عدد = (FT_ULong)کلی->عدد;
    FT_ULong  res;


    /* Mocklisp hash function. */
    res = عدد & 0xFF;
    res = ( res << 5 ) - res + ( ( عدد >>  8 ) & 0xFF );
    res = ( res << 5 ) - res + ( ( عدد >> 16 ) & 0xFF );
    res = ( res << 5 ) - res + ( ( عدد >> 24 ) & 0xFF );

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
  hash_عدد_compare( FT_Hashkey*  a,
                    FT_Hashkey*  b )
  {
    إذا ( a->عدد == b->عدد )
      أرجع 1;

    أرجع 0;
  }


  ساكن FT_Hashnode*
  hash_لوښی( FT_Hashkey  کلی,
               FT_Hash     hash )
  {
    FT_ULong      res = 0;
    FT_Hashnode*  bp  = hash->جدول;
    FT_Hashnode*  ndp;


    res = (hash->lookup)( &کلی );

    ndp = bp + ( res % hash->اندازه );
    طالما ( *ndp )
    {
      إذا ( (hash->compare)( &(*ndp)->کلی, &کلی ) )
        اكسر;

      ndp--;
      إذا ( ndp < bp )
        ndp = bp + ( hash->اندازه - 1 );
    }

    أرجع ndp;
  }


  ساكن FT_Error
  hash_rehash( FT_Hash    hash,
               FT_حافظه  حافظه )
  {
    FT_Hashnode*  obp = hash->جدول;
    FT_Hashnode*  bp;
    FT_Hashnode*  nbp;

    FT_UInt   i, sz = hash->اندازه;
    FT_Error  error = FT_Err_Ok;


    hash->اندازه <<= 1;
    hash->حد  = hash->اندازه / 3;

    إذا ( FT_NEW_ARRAY( hash->جدول, hash->اندازه ) )
      اذهب Exit;

    لكل ( i = 0, bp = obp; i < sz; i++, bp++ )
    {
      إذا ( *bp )
      {
        nbp = hash_لوښی( (*bp)->کلی, hash );
        *nbp = *bp;
      }
    }

    FT_FREE( obp );

  Exit:
    أرجع error;
  }


  ساكن FT_Error
  hash_init( FT_Hash    hash,
             FT_Bool    is_عدد,
             FT_حافظه  حافظه )
  {
    FT_UInt   sz = INITIAL_HT_اندازه;
    FT_Error  error;


    hash->اندازه  = sz;
    hash->حد = sz / 3;
    hash->کارول_شوی  = 0;

    إذا ( is_عدد )
    {
      hash->lookup  = hash_عدد_lookup;
      hash->compare = hash_عدد_compare;
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
                    FT_حافظه  حافظه )
  {
    أرجع hash_init( hash, 0, حافظه );
  }


  FT_Error
  ft_hash_عدد_init( FT_Hash    hash,
                    FT_حافظه  حافظه )
  {
    أرجع hash_init( hash, 1, حافظه );
  }


  فراغ
  ft_hash_str_free( FT_Hash    hash,
                    FT_حافظه  حافظه )
  {
    إذا ( hash )
    {
      FT_UInt       sz = hash->اندازه;
      FT_Hashnode*  bp = hash->جدول;
      FT_UInt       i;


      لكل ( i = 0; i < sz; i++, bp++ )
        FT_FREE( *bp );

      FT_FREE( hash->جدول );
    }
  }


  /* `ft_hash_num_free' is the same as `ft_hash_str_free' */


  ساكن FT_Error
  hash_ورننباسه( FT_Hashkey  کلی,
               size_t      معلومات,
               FT_Hash     hash,
               FT_حافظه   حافظه,
               FT_Bool     overwrite )
  {
    FT_Hashnode   nn;
    FT_Hashnode*  bp    = hash_لوښی( کلی, hash );
    FT_Error      error = FT_Err_Ok;


    nn = *bp;
    إذا ( !nn )
    {
      إذا ( FT_QNEW( nn ) )
        اذهب Exit;
      *bp = nn;

      nn->کلی  = کلی;
      nn->معلومات = معلومات;

      إذا ( hash->کارول_شوی >= hash->حد )
      {
        error = hash_rehash( hash, حافظه );
        إذا ( error )
          اذهب Exit;
      }

      hash->کارول_شوی++;
    }
    وإلا إذا ( overwrite )
      nn->معلومات = معلومات;

  Exit:
    أرجع error;
  }


  FT_Error
  ft_hash_str_ورننباسه( ثابت حرف*  کلی,
                      size_t       معلومات,
                      FT_Hash      hash,
                      FT_حافظه    حافظه )
  {
    FT_Hashkey  hk;


    hk.str = کلی;

    أرجع hash_ورننباسه( hk, معلومات, hash, حافظه, TRUE );
  }


  FT_Error
  ft_hash_عدد_ورننباسه( FT_Int     عدد,
                      size_t     معلومات,
                      FT_Hash    hash,
                      FT_حافظه  حافظه )
  {
    FT_Hashkey  hk;


    hk.عدد = عدد;

    أرجع hash_ورننباسه( hk, معلومات, hash, حافظه, TRUE );
  }


  FT_Error
  ft_hash_str_ورننباسه_no_overwrite( ثابت حرف*  کلی,
                                   size_t       معلومات,
                                   FT_Hash      hash,
                                   FT_حافظه    حافظه )
  {
    FT_Hashkey  hk;


    hk.str = کلی;

    أرجع hash_ورننباسه( hk, معلومات, hash, حافظه, FALSE );
  }


  FT_Error
  ft_hash_عدد_ورننباسه_no_overwrite( FT_Int     عدد,
                                   size_t     معلومات,
                                   FT_Hash    hash,
                                   FT_حافظه  حافظه )
  {
    FT_Hashkey  hk;


    hk.عدد = عدد;

    أرجع hash_ورننباسه( hk, معلومات, hash, حافظه, FALSE );
  }


  ساكن size_t*
  hash_lookup( FT_Hashkey  کلی,
               FT_Hash     hash )
  {
    FT_Hashnode*  np = hash_لوښی( کلی, hash );


    أرجع (*np) ? &(*np)->معلومات
                 : عدم;
  }


  size_t*
  ft_hash_str_lookup( ثابت حرف*  کلی,
                      FT_Hash      hash )
  {
    FT_Hashkey  hk;


    hk.str = کلی;

    أرجع hash_lookup( hk, hash );
  }


  size_t*
  ft_hash_عدد_lookup( FT_Int   عدد,
                      FT_Hash  hash )
  {
    FT_Hashkey  hk;


    hk.عدد = عدد;

    أرجع hash_lookup( hk, hash );
  }


  FT_Bool
  ft_hash_عدد_iterator( FT_UInt  *idx,
                        FT_Int   *کلی,
                        size_t   *ارزښت,
                        FT_Hash   hash )
  {
    FT_Hashnode  nn = عدم;


    طالما ( 1 )
    {
      إذا ( *idx >= hash->اندازه )
        أرجع 0;

      nn = hash->جدول[*idx];
      إذا ( nn )
        اكسر;

      (*idx)++;
    }

    إذا ( کلی )
      *کلی = nn->کلی.عدد;
    إذا ( ارزښت )
      *ارزښت = nn->معلومات;

    (*idx)++;

    أرجع 1;
  }


  FT_Bool
  ft_hash_str_iterator( FT_UInt      *idx,
                        ثابت حرف*  *کلی,
                        size_t       *ارزښت,
                        FT_Hash       hash )
  {
    FT_Hashnode  nn = عدم;


    طالما ( 1 )
    {
      إذا ( *idx >= hash->اندازه )
        أرجع 0;

      nn = hash->جدول[*idx];
      إذا ( nn )
        اكسر;

      (*idx)++;
    }

    إذا ( کلی )
      *کلی = nn->کلی.str;
    إذا ( ارزښت )
      *ارزښت = nn->معلومات;

    (*idx)++;

    أرجع 1;
  }


/*پای_ټکی*/
