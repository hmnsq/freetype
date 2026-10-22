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


#అన్తర్భావ <freetype/internal/fthash.h>
#అన్తర్భావ <freetype/internal/ftmemory.h>
#అన్తర్భావ <freetype/internal/ftobjs.h>


#నిర్వచించు INITIAL_HT_కొలత  241


  స్థిర FT_ULong
  hash_str_lookup( FT_Hashkey*  కీలకం )
  {
    స్థిరరాశి అక్షరం*  kp  = కీలకం->str;
    FT_ULong     res = 0;


    /* Mocklisp hash function. */
    అయితే ( *kp )
      res = ( res << 5 ) - res + (FT_ULong)*kp++;

    తిరిగి res;
  }


  స్థిర FT_ULong
  hash_అంకె_lookup( FT_Hashkey*  కీలకం )
  {
    FT_ULong  అంకె = (FT_ULong)కీలకం->అంకె;
    FT_ULong  res;


    /* Mocklisp hash function. */
    res = అంకె & 0xFF;
    res = ( res << 5 ) - res + ( ( అంకె >>  8 ) & 0xFF );
    res = ( res << 5 ) - res + ( ( అంకె >> 16 ) & 0xFF );
    res = ( res << 5 ) - res + ( ( అంకె >> 24 ) & 0xFF );

    తిరిగి res;
  }


  స్థిర FT_Bool
  hash_str_compare( FT_Hashkey*  a,
                    FT_Hashkey*  b )
  {
    ఒకవేళ ( a->str[0] == b->str[0]           &&
         ft_strcmp( a->str, b->str ) == 0 )
      తిరిగి 1;

    తిరిగి 0;
  }


  స్థిర FT_Bool
  hash_అంకె_compare( FT_Hashkey*  a,
                    FT_Hashkey*  b )
  {
    ఒకవేళ ( a->అంకె == b->అంకె )
      తిరిగి 1;

    తిరిగి 0;
  }


  స్థిర FT_Hashnode*
  hash_పాత్ర( FT_Hashkey  కీలకం,
               FT_Hash     hash )
  {
    FT_ULong      res = 0;
    FT_Hashnode*  bp  = hash->పట్టిక;
    FT_Hashnode*  ndp;


    res = (hash->lookup)( &కీలకం );

    ndp = bp + ( res % hash->కొలత );
    అయితే ( *ndp )
    {
      ఒకవేళ ( (hash->compare)( &(*ndp)->కీలకం, &కీలకం ) )
        ఆపు;

      ndp--;
      ఒకవేళ ( ndp < bp )
        ndp = bp + ( hash->కొలత - 1 );
    }

    తిరిగి ndp;
  }


  స్థిర FT_Error
  hash_rehash( FT_Hash    hash,
               FT_జ్ఞాపకం  జ్ఞాపకం )
  {
    FT_Hashnode*  obp = hash->పట్టిక;
    FT_Hashnode*  bp;
    FT_Hashnode*  nbp;

    FT_UInt   i, sz = hash->కొలత;
    FT_Error  error = FT_Err_Ok;


    hash->కొలత <<= 1;
    hash->హద్దు  = hash->కొలత / 3;

    ఒకవేళ ( FT_NEW_ARRAY( hash->పట్టిక, hash->కొలత ) )
      వెళ్ళు Exit;

    కొరకు ( i = 0, bp = obp; i < sz; i++, bp++ )
    {
      ఒకవేళ ( *bp )
      {
        nbp = hash_పాత్ర( (*bp)->కీలకం, hash );
        *nbp = *bp;
      }
    }

    FT_FREE( obp );

  Exit:
    తిరిగి error;
  }


  స్థిర FT_Error
  hash_init( FT_Hash    hash,
             FT_Bool    is_అంకె,
             FT_జ్ఞాపకం  జ్ఞాపకం )
  {
    FT_UInt   sz = INITIAL_HT_కొలత;
    FT_Error  error;


    hash->కొలత  = sz;
    hash->హద్దు = sz / 3;
    hash->వాడిన  = 0;

    ఒకవేళ ( is_అంకె )
    {
      hash->lookup  = hash_అంకె_lookup;
      hash->compare = hash_అంకె_compare;
    }
    లేకపోతే
    {
      hash->lookup  = hash_str_lookup;
      hash->compare = hash_str_compare;
    }

    FT_MEM_NEW_ARRAY( hash->పట్టిక, sz );

    తిరిగి error;
  }


  FT_Error
  ft_hash_str_init( FT_Hash    hash,
                    FT_జ్ఞాపకం  జ్ఞాపకం )
  {
    తిరిగి hash_init( hash, 0, జ్ఞాపకం );
  }


  FT_Error
  ft_hash_అంకె_init( FT_Hash    hash,
                    FT_జ్ఞాపకం  జ్ఞాపకం )
  {
    తిరిగి hash_init( hash, 1, జ్ఞాపకం );
  }


  శూన్య
  ft_hash_str_free( FT_Hash    hash,
                    FT_జ్ఞాపకం  జ్ఞాపకం )
  {
    ఒకవేళ ( hash )
    {
      FT_UInt       sz = hash->కొలత;
      FT_Hashnode*  bp = hash->పట్టిక;
      FT_UInt       i;


      కొరకు ( i = 0; i < sz; i++, bp++ )
        FT_FREE( *bp );

      FT_FREE( hash->పట్టిక );
    }
  }


  /* `ft_hash_num_free' is the same as `ft_hash_str_free' */


  స్థిర FT_Error
  hash_చొప్పించు( FT_Hashkey  కీలకం,
               size_t      దత్తాంశం,
               FT_Hash     hash,
               FT_జ్ఞాపకం   జ్ఞాపకం,
               FT_Bool     overwrite )
  {
    FT_Hashnode   nn;
    FT_Hashnode*  bp    = hash_పాత్ర( కీలకం, hash );
    FT_Error      error = FT_Err_Ok;


    nn = *bp;
    ఒకవేళ ( !nn )
    {
      ఒకవేళ ( FT_QNEW( nn ) )
        వెళ్ళు Exit;
      *bp = nn;

      nn->కీలకం  = కీలకం;
      nn->దత్తాంశం = దత్తాంశం;

      ఒకవేళ ( hash->వాడిన >= hash->హద్దు )
      {
        error = hash_rehash( hash, జ్ఞాపకం );
        ఒకవేళ ( error )
          వెళ్ళు Exit;
      }

      hash->వాడిన++;
    }
    లేకపోతే ఒకవేళ ( overwrite )
      nn->దత్తాంశం = దత్తాంశం;

  Exit:
    తిరిగి error;
  }


  FT_Error
  ft_hash_str_చొప్పించు( స్థిరరాశి అక్షరం*  కీలకం,
                      size_t       దత్తాంశం,
                      FT_Hash      hash,
                      FT_జ్ఞాపకం    జ్ఞాపకం )
  {
    FT_Hashkey  hk;


    hk.str = కీలకం;

    తిరిగి hash_చొప్పించు( hk, దత్తాంశం, hash, జ్ఞాపకం, TRUE );
  }


  FT_Error
  ft_hash_అంకె_చొప్పించు( FT_Int     అంకె,
                      size_t     దత్తాంశం,
                      FT_Hash    hash,
                      FT_జ్ఞాపకం  జ్ఞాపకం )
  {
    FT_Hashkey  hk;


    hk.అంకె = అంకె;

    తిరిగి hash_చొప్పించు( hk, దత్తాంశం, hash, జ్ఞాపకం, TRUE );
  }


  FT_Error
  ft_hash_str_చొప్పించు_no_overwrite( స్థిరరాశి అక్షరం*  కీలకం,
                                   size_t       దత్తాంశం,
                                   FT_Hash      hash,
                                   FT_జ్ఞాపకం    జ్ఞాపకం )
  {
    FT_Hashkey  hk;


    hk.str = కీలకం;

    తిరిగి hash_చొప్పించు( hk, దత్తాంశం, hash, జ్ఞాపకం, FALSE );
  }


  FT_Error
  ft_hash_అంకె_చొప్పించు_no_overwrite( FT_Int     అంకె,
                                   size_t     దత్తాంశం,
                                   FT_Hash    hash,
                                   FT_జ్ఞాపకం  జ్ఞాపకం )
  {
    FT_Hashkey  hk;


    hk.అంకె = అంకె;

    తిరిగి hash_చొప్పించు( hk, దత్తాంశం, hash, జ్ఞాపకం, FALSE );
  }


  స్థిర size_t*
  hash_lookup( FT_Hashkey  కీలకం,
               FT_Hash     hash )
  {
    FT_Hashnode*  np = hash_పాత్ర( కీలకం, hash );


    తిరిగి (*np) ? &(*np)->దత్తాంశం
                 : ఖాళీ;
  }


  size_t*
  ft_hash_str_lookup( స్థిరరాశి అక్షరం*  కీలకం,
                      FT_Hash      hash )
  {
    FT_Hashkey  hk;


    hk.str = కీలకం;

    తిరిగి hash_lookup( hk, hash );
  }


  size_t*
  ft_hash_అంకె_lookup( FT_Int   అంకె,
                      FT_Hash  hash )
  {
    FT_Hashkey  hk;


    hk.అంకె = అంకె;

    తిరిగి hash_lookup( hk, hash );
  }


  FT_Bool
  ft_hash_అంకె_iterator( FT_UInt  *idx,
                        FT_Int   *కీలకం,
                        size_t   *విలువ,
                        FT_Hash   hash )
  {
    FT_Hashnode  nn = ఖాళీ;


    అయితే ( 1 )
    {
      ఒకవేళ ( *idx >= hash->కొలత )
        తిరిగి 0;

      nn = hash->పట్టిక[*idx];
      ఒకవేళ ( nn )
        ఆపు;

      (*idx)++;
    }

    ఒకవేళ ( కీలకం )
      *కీలకం = nn->కీలకం.అంకె;
    ఒకవేళ ( విలువ )
      *విలువ = nn->దత్తాంశం;

    (*idx)++;

    తిరిగి 1;
  }


  FT_Bool
  ft_hash_str_iterator( FT_UInt      *idx,
                        స్థిరరాశి అక్షరం*  *కీలకం,
                        size_t       *విలువ,
                        FT_Hash       hash )
  {
    FT_Hashnode  nn = ఖాళీ;


    అయితే ( 1 )
    {
      ఒకవేళ ( *idx >= hash->కొలత )
        తిరిగి 0;

      nn = hash->పట్టిక[*idx];
      ఒకవేళ ( nn )
        ఆపు;

      (*idx)++;
    }

    ఒకవేళ ( కీలకం )
      *కీలకం = nn->కీలకం.str;
    ఒకవేళ ( విలువ )
      *విలువ = nn->దత్తాంశం;

    (*idx)++;

    తిరిగి 1;
  }


/*చివర*/
