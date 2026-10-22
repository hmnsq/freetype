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


#ಅನ್ತರ್ಭಾವ <freetype/internal/ftdebug.h>

#ಅನ್ತರ್ಭಾವ <freetype/ftsnames.h>
#ಅನ್ತರ್ಭಾವ <freetype/internal/tttypes.h>
#ಅನ್ತರ್ಭಾವ <freetype/internal/ftstream.h>


#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ TT_CONFIG_OPTION_SFNT_ಹೆಸರುಗಳು


  /* documentation is in ftsnames.h */

  FT_EXPORT_DEF( FT_UInt )
  FT_Get_Sfnt_Name_ಎಣಿಕೆ( FT_Face  face )
  {
    ಹಿಂತಿರುಗಿ ( face && FT_IS_SFNT( face ) ) ? ((TT_Face)face)->ಅಂಕೆ_ಹೆಸರುಗಳು : 0;
  }


  /* documentation is in ftsnames.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Sfnt_Name( FT_Face       face,
                    FT_UInt       idx,
                    FT_SfntName  *aname )
  {
    FT_Error  error = FT_ERR( Invalid_ವಾದ );


    ಆದರೆ ( aname && face && FT_IS_SFNT( face ) )
    {
      TT_Face  ttface = (TT_Face)face;


      ಆದರೆ ( idx < (FT_UInt)ttface->ಅಂಕೆ_ಹೆಸರುಗಳು )
      {
        TT_Name  ನಮೂದು = ttface->name_ಕೋಷ್ಟಕ.ಹೆಸರುಗಳು + idx;


        /* load name on demand */
        ಆದರೆ ( ನಮೂದು->string_ಉದ್ದಳತೆ > 0 && !ನಮೂದು->string )
        {
          FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ = face->ಸ್ಮರಣೆ;
          FT_ಹರಿವು  ಹರಿವು = face->ಹರಿವು;


          ಆದರೆ ( FT_QNEW_ARRAY ( ನಮೂದು->string, ನಮೂದು->string_ಉದ್ದಳತೆ ) ||
               FT_ಹರಿವು_SEEK( ನಮೂದು->string_ಸ್ಥಾನಾಂತರ )                ||
               FT_ಹರಿವು_READ( ನಮೂದು->string, ನಮೂದು->string_ಉದ್ದಳತೆ ) )
          {
            FT_FREE( ನಮೂದು->string );
            ನಮೂದು->string_ಉದ್ದಳತೆ = 0;
          }
        }

        aname->platform_id = ನಮೂದು->platformID;
        aname->encoding_id = ನಮೂದು->encodingID;
        aname->language_id = ನಮೂದು->languageID;
        aname->name_id     = ನಮೂದು->nameID;
        aname->string      = (FT_Byte*)ನಮೂದು->string;
        aname->string_len  = ನಮೂದು->string_ಉದ್ದಳತೆ;

        error = FT_Err_Ok;
      }
    }

    ಹಿಂತಿರುಗಿ error;
  }


  /* documentation is in ftsnames.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Sfnt_Lang_ಟ್ಯಾಗ್( FT_Face          face,
                       FT_UInt          langID,
                       FT_Sfnt_Lang_ಟ್ಯಾಗ್  *alang_ಟ್ಯಾಗ್ )
  {
    FT_Error  error = FT_ERR( Invalid_ವಾದ );


    ಆದರೆ ( alang_ಟ್ಯಾಗ್ && face && FT_IS_SFNT( face ) )
    {
      TT_Face  ttface = (TT_Face)face;


      ಆದರೆ ( ttface->name_ಕೋಷ್ಟಕ.format != 1 )
        ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ಕೋಷ್ಟಕ );

      ಆದರೆ ( langID > 0x8000U                                        &&
           langID - 0x8000U < ttface->name_ಕೋಷ್ಟಕ.ಅಂಕೆ_Lang_ಟ್ಯಾಗ್_ದಾಖಲೆಗಳು )
      {
        TT_Lang_ಟ್ಯಾಗ್  ನಮೂದು = ttface->name_ಕೋಷ್ಟಕ.lang_ಟ್ಯಾಗುಗಳು +
                            ( langID - 0x8000U );


        /* load name on demand */
        ಆದರೆ ( ನಮೂದು->string_ಉದ್ದಳತೆ > 0 && !ನಮೂದು->string )
        {
          FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ = face->ಸ್ಮರಣೆ;
          FT_ಹರಿವು  ಹರಿವು = face->ಹರಿವು;


          ಆದರೆ ( FT_QNEW_ARRAY ( ನಮೂದು->string, ನಮೂದು->string_ಉದ್ದಳತೆ ) ||
               FT_ಹರಿವು_SEEK( ನಮೂದು->string_ಸ್ಥಾನಾಂತರ )                ||
               FT_ಹರಿವು_READ( ನಮೂದು->string, ನಮೂದು->string_ಉದ್ದಳತೆ ) )
          {
            FT_FREE( ನಮೂದು->string );
            ನಮೂದು->string_ಉದ್ದಳತೆ = 0;
          }
        }

        alang_ಟ್ಯಾಗ್->string     = (FT_Byte*)ನಮೂದು->string;
        alang_ಟ್ಯಾಗ್->string_len = ನಮೂದು->string_ಉದ್ದಳತೆ;

        error = FT_Err_Ok;
      }
    }

    ಹಿಂತಿರುಗಿ error;
  }


#ಇಲ್ಲದಿದ್ದರೆ /* !TT_CONFIG_OPTION_SFNT_NAMES */


  FT_EXPORT_DEF( FT_UInt )
  FT_Get_Sfnt_Name_ಎಣಿಕೆ( FT_Face  face )
  {
    FT_ಬಳಸದ( face );

    ಹಿಂತಿರುಗಿ 0;
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Get_Sfnt_Name( FT_Face       face,
                    FT_UInt       idx,
                    FT_SfntName  *aname )
  {
    FT_ಬಳಸದ( face );
    FT_ಬಳಸದ( idx );
    FT_ಬಳಸದ( aname );

    ಹಿಂತಿರುಗಿ FT_THROW( Unimplemented_Feature );
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Get_Sfnt_Lang_ಟ್ಯಾಗ್( FT_Face          face,
                       FT_UInt          langID,
                       FT_Sfnt_Lang_ಟ್ಯಾಗ್  *alang_ಟ್ಯಾಗ್ )
  {
    FT_ಬಳಸದ( face );
    FT_ಬಳಸದ( langID );
    FT_ಬಳಸದ( alang_ಟ್ಯಾಗ್ );

    ಹಿಂತಿರುಗಿ FT_THROW( Unimplemented_Feature );
  }


#ಆದರೆ_ಅಂತ್ಯ /* !TT_CONFIG_OPTION_SFNT_NAMES */


/*ಕೊನೆ*/
