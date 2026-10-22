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


#تعریف_کرو INITIAL_HT_ناپ  241


  ساكن FT_ULong
  hash_str_lookup( FT_Hashkey*  کلید )
  {
    ثابت حرف*  kp  = کلید->str;
    FT_ULong     res = 0;


    /* Mocklisp hash function. */
    طالما ( *kp )
      res = ( res << 5 ) - res + (FT_ULong)*kp++;

    أرجع res;
  }


  ساكن FT_ULong
  hash_عدد_lookup( FT_Hashkey*  کلید )
  {
    FT_ULong  عدد = (FT_ULong)کلید->عدد;
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
  hash_بالٹی( FT_Hashkey  کلید,
               FT_Hash     hash )
  {
    FT_ULong      res = 0;
    FT_Hashnode*  bp  = hash->جدول;
    FT_Hashnode*  ndp;


    res = (hash->lookup)( &کلید );

    ndp = bp + ( res % hash->ناپ );
    طالما ( *ndp )
    {
      إذا ( (hash->compare)( &(*ndp)->کلید, &کلید ) )
        اكسر;

      ndp--;
      إذا ( ndp < bp )
        ndp = bp + ( hash->ناپ - 1 );
    }

    أرجع ndp;
  }


  ساكن FT_Error
  hash_rehash( FT_Hash    hash,
               FT_حافظہ  حافظہ )
  {
    FT_Hashnode*  obp = hash->جدول;
    FT_Hashnode*  bp;
    FT_Hashnode*  nbp;

    FT_UInt   i, sz = hash->ناپ;
    FT_Error  error = FT_Err_Ok;


    hash->ناپ <<= 1;
    hash->حد_بندی  = hash->ناپ / 3;

    إذا ( FT_NEW_ARRAY( hash->جدول, hash->ناپ ) )
      اذهب Exit;

    لكل ( i = 0, bp = obp; i < sz; i++, bp++ )
    {
      إذا ( *bp )
      {
        nbp = hash_بالٹی( (*bp)->کلید, hash );
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
             FT_حافظہ  حافظہ )
  {
    FT_UInt   sz = INITIAL_HT_ناپ;
    FT_Error  error;


    hash->ناپ  = sz;
    hash->حد_بندی = sz / 3;
    hash->ورتیا  = 0;

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
                    FT_حافظہ  حافظہ )
  {
    أرجع hash_init( hash, 0, حافظہ );
  }


  FT_Error
  ft_hash_عدد_init( FT_Hash    hash,
                    FT_حافظہ  حافظہ )
  {
    أرجع hash_init( hash, 1, حافظہ );
  }


  فراغ
  ft_hash_str_free( FT_Hash    hash,
                    FT_حافظہ  حافظہ )
  {
    إذا ( hash )
    {
      FT_UInt       sz = hash->ناپ;
      FT_Hashnode*  bp = hash->جدول;
      FT_UInt       i;


      لكل ( i = 0; i < sz; i++, bp++ )
        FT_FREE( *bp );

      FT_FREE( hash->جدول );
    }
  }


  /* `ft_hash_num_free' is the same as `ft_hash_str_free' */


  ساكن FT_Error
  hash_داخل_کرو( FT_Hashkey  کلید,
               size_t      انکڑے,
               FT_Hash     hash,
               FT_حافظہ   حافظہ,
               FT_Bool     overwrite )
  {
    FT_Hashnode   nn;
    FT_Hashnode*  bp    = hash_بالٹی( کلید, hash );
    FT_Error      error = FT_Err_Ok;


    nn = *bp;
    إذا ( !nn )
    {
      إذا ( FT_QNEW( nn ) )
        اذهب Exit;
      *bp = nn;

      nn->کلید  = کلید;
      nn->انکڑے = انکڑے;

      إذا ( hash->ورتیا >= hash->حد_بندی )
      {
        error = hash_rehash( hash, حافظہ );
        إذا ( error )
          اذهب Exit;
      }

      hash->ورتیا++;
    }
    وإلا إذا ( overwrite )
      nn->انکڑے = انکڑے;

  Exit:
    أرجع error;
  }


  FT_Error
  ft_hash_str_داخل_کرو( ثابت حرف*  کلید,
                      size_t       انکڑے,
                      FT_Hash      hash,
                      FT_حافظہ    حافظہ )
  {
    FT_Hashkey  hk;


    hk.str = کلید;

    أرجع hash_داخل_کرو( hk, انکڑے, hash, حافظہ, TRUE );
  }


  FT_Error
  ft_hash_عدد_داخل_کرو( FT_Int     عدد,
                      size_t     انکڑے,
                      FT_Hash    hash,
                      FT_حافظہ  حافظہ )
  {
    FT_Hashkey  hk;


    hk.عدد = عدد;

    أرجع hash_داخل_کرو( hk, انکڑے, hash, حافظہ, TRUE );
  }


  FT_Error
  ft_hash_str_داخل_کرو_no_overwrite( ثابت حرف*  کلید,
                                   size_t       انکڑے,
                                   FT_Hash      hash,
                                   FT_حافظہ    حافظہ )
  {
    FT_Hashkey  hk;


    hk.str = کلید;

    أرجع hash_داخل_کرو( hk, انکڑے, hash, حافظہ, FALSE );
  }


  FT_Error
  ft_hash_عدد_داخل_کرو_no_overwrite( FT_Int     عدد,
                                   size_t     انکڑے,
                                   FT_Hash    hash,
                                   FT_حافظہ  حافظہ )
  {
    FT_Hashkey  hk;


    hk.عدد = عدد;

    أرجع hash_داخل_کرو( hk, انکڑے, hash, حافظہ, FALSE );
  }


  ساكن size_t*
  hash_lookup( FT_Hashkey  کلید,
               FT_Hash     hash )
  {
    FT_Hashnode*  np = hash_بالٹی( کلید, hash );


    أرجع (*np) ? &(*np)->انکڑے
                 : عدم;
  }


  size_t*
  ft_hash_str_lookup( ثابت حرف*  کلید,
                      FT_Hash      hash )
  {
    FT_Hashkey  hk;


    hk.str = کلید;

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
                        FT_Int   *کلید,
                        size_t   *قدر,
                        FT_Hash   hash )
  {
    FT_Hashnode  nn = عدم;


    طالما ( 1 )
    {
      إذا ( *idx >= hash->ناپ )
        أرجع 0;

      nn = hash->جدول[*idx];
      إذا ( nn )
        اكسر;

      (*idx)++;
    }

    إذا ( کلید )
      *کلید = nn->کلید.عدد;
    إذا ( قدر )
      *قدر = nn->انکڑے;

    (*idx)++;

    أرجع 1;
  }


  FT_Bool
  ft_hash_str_iterator( FT_UInt      *idx,
                        ثابت حرف*  *کلید,
                        size_t       *قدر,
                        FT_Hash       hash )
  {
    FT_Hashnode  nn = عدم;


    طالما ( 1 )
    {
      إذا ( *idx >= hash->ناپ )
        أرجع 0;

      nn = hash->جدول[*idx];
      إذا ( nn )
        اكسر;

      (*idx)++;
    }

    إذا ( کلید )
      *کلید = nn->کلید.str;
    إذا ( قدر )
      *قدر = nn->انکڑے;

    (*idx)++;

    أرجع 1;
  }


/*سرا*/
