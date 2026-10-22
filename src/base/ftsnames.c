/****************************************************************************
 *
 * ftsnames.c
 *
 *   Simple interface to access SFNT name tables (which are used
 *   to hold font names, copyright info, notices, etc.) (body).
 *
 *   This is _not_ used to retrieve glyph names!
 *
 * Copyright (C) 1996-2026 by
 * David Turner, Robert Wilhelm, and Werner Lemberg.
 *
 * This file is part of the FreeType project, and may only be used,
 * modified, and distributed under the terms of the FreeType project
 * license, LICENSE.TXT.  By continuing to use, modify, or distribute
 * this file you indicate that you have read the license and
 * understand and accept it fully.
 *
 */


#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftdebug.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/ftsnames.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/tttypes.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftstream.h>


#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ TT_CONFIG_OPTION_SFNT_ਨਾਂ


  /* documentation is in ftsnames.h */

  FT_EXPORT_DEF( FT_UInt )
  FT_Get_Sfnt_Name_ਗਿਣਤੀ( FT_Face  face )
  {
    ਵਾਪਸ ( face && FT_IS_SFNT( face ) ) ? ((TT_Face)face)->ਅੰਕ_ਨਾਂ : 0;
  }


  /* documentation is in ftsnames.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Sfnt_Name( FT_Face       face,
                    FT_UInt       idx,
                    FT_SfntName  *aname )
  {
    FT_Error  error = FT_ERR( Invalid_ਦਲੀਲ );


    ਜੇ ( aname && face && FT_IS_SFNT( face ) )
    {
      TT_Face  ttface = (TT_Face)face;


      ਜੇ ( idx < (FT_UInt)ttface->ਅੰਕ_ਨਾਂ )
      {
        TT_Name  ਇੰਦਰਾਜ = ttface->name_ਸਾਰਣੀ.ਨਾਂ + idx;


        /* load name on demand */
        ਜੇ ( ਇੰਦਰਾਜ->string_ਲੰਮਾਈ > 0 && !ਇੰਦਰਾਜ->string )
        {
          FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ = face->ਸਮ੍ਰਿਤੀ;
          FT_ਧਾਰਾ  ਧਾਰਾ = face->ਧਾਰਾ;


          ਜੇ ( FT_QNEW_ARRAY ( ਇੰਦਰਾਜ->string, ਇੰਦਰਾਜ->string_ਲੰਮਾਈ ) ||
               FT_ਧਾਰਾ_SEEK( ਇੰਦਰਾਜ->string_ਵਿੱਥ )                ||
               FT_ਧਾਰਾ_READ( ਇੰਦਰਾਜ->string, ਇੰਦਰਾਜ->string_ਲੰਮਾਈ ) )
          {
            FT_FREE( ਇੰਦਰਾਜ->string );
            ਇੰਦਰਾਜ->string_ਲੰਮਾਈ = 0;
          }
        }

        aname->platform_id = ਇੰਦਰਾਜ->platformID;
        aname->encoding_id = ਇੰਦਰਾਜ->encodingID;
        aname->language_id = ਇੰਦਰਾਜ->languageID;
        aname->name_id     = ਇੰਦਰਾਜ->nameID;
        aname->string      = (FT_Byte*)ਇੰਦਰਾਜ->string;
        aname->string_len  = ਇੰਦਰਾਜ->string_ਲੰਮਾਈ;

        error = FT_Err_Ok;
      }
    }

    ਵਾਪਸ error;
  }


  /* documentation is in ftsnames.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Sfnt_Lang_ਟੈਗ( FT_Face          face,
                       FT_UInt          langID,
                       FT_Sfnt_Lang_ਟੈਗ  *alang_ਟੈਗ )
  {
    FT_Error  error = FT_ERR( Invalid_ਦਲੀਲ );


    ਜੇ ( alang_ਟੈਗ && face && FT_IS_SFNT( face ) )
    {
      TT_Face  ttface = (TT_Face)face;


      ਜੇ ( ttface->name_ਸਾਰਣੀ.format != 1 )
        ਵਾਪਸ FT_THROW( Invalid_ਸਾਰਣੀ );

      ਜੇ ( langID > 0x8000U                                        &&
           langID - 0x8000U < ttface->name_ਸਾਰਣੀ.ਅੰਕ_Lang_ਟੈਗ_ਰਿਕਾਰਡਾਂ )
      {
        TT_Lang_ਟੈਗ  ਇੰਦਰਾਜ = ttface->name_ਸਾਰਣੀ.lang_ਟੈਗ_ਸੂਚੀ +
                            ( langID - 0x8000U );


        /* load name on demand */
        ਜੇ ( ਇੰਦਰਾਜ->string_ਲੰਮਾਈ > 0 && !ਇੰਦਰਾਜ->string )
        {
          FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ = face->ਸਮ੍ਰਿਤੀ;
          FT_ਧਾਰਾ  ਧਾਰਾ = face->ਧਾਰਾ;


          ਜੇ ( FT_QNEW_ARRAY ( ਇੰਦਰਾਜ->string, ਇੰਦਰਾਜ->string_ਲੰਮਾਈ ) ||
               FT_ਧਾਰਾ_SEEK( ਇੰਦਰਾਜ->string_ਵਿੱਥ )                ||
               FT_ਧਾਰਾ_READ( ਇੰਦਰਾਜ->string, ਇੰਦਰਾਜ->string_ਲੰਮਾਈ ) )
          {
            FT_FREE( ਇੰਦਰਾਜ->string );
            ਇੰਦਰਾਜ->string_ਲੰਮਾਈ = 0;
          }
        }

        alang_ਟੈਗ->string     = (FT_Byte*)ਇੰਦਰਾਜ->string;
        alang_ਟੈਗ->string_len = ਇੰਦਰਾਜ->string_ਲੰਮਾਈ;

        error = FT_Err_Ok;
      }
    }

    ਵਾਪਸ error;
  }


#ਨਹੀਂਤਾਂ /* !TT_CONFIG_OPTION_SFNT_NAMES */


  FT_EXPORT_DEF( FT_UInt )
  FT_Get_Sfnt_Name_ਗਿਣਤੀ( FT_Face  face )
  {
    FT_ਅਣਵਰਤਿਆ( face );

    ਵਾਪਸ 0;
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Get_Sfnt_Name( FT_Face       face,
                    FT_UInt       idx,
                    FT_SfntName  *aname )
  {
    FT_ਅਣਵਰਤਿਆ( face );
    FT_ਅਣਵਰਤਿਆ( idx );
    FT_ਅਣਵਰਤਿਆ( aname );

    ਵਾਪਸ FT_THROW( Unimplemented_Feature );
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Get_Sfnt_Lang_ਟੈਗ( FT_Face          face,
                       FT_UInt          langID,
                       FT_Sfnt_Lang_ਟੈਗ  *alang_ਟੈਗ )
  {
    FT_ਅਣਵਰਤਿਆ( face );
    FT_ਅਣਵਰਤਿਆ( langID );
    FT_ਅਣਵਰਤਿਆ( alang_ਟੈਗ );

    ਵਾਪਸ FT_THROW( Unimplemented_Feature );
  }


#ਜੇ_ਅੰਤ /* !TT_CONFIG_OPTION_SFNT_NAMES */


/*ਸਿਰਾ*/
