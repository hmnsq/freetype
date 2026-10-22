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


#अन्तर्भाव <freetype/internal/ftdebug.h>

#अन्तर्भाव <freetype/ftsnames.h>
#अन्तर्भाव <freetype/internal/tttypes.h>
#अन्तर्भाव <freetype/internal/ftstream.h>


#यदि_परिभाषितम् TT_CONFIG_OPTION_SFNT_नामानि


  /* documentation is in ftsnames.h */

  FT_EXPORT_DEF( FT_UInt )
  FT_Get_Sfnt_Name_गणनम्( FT_Face  face )
  {
    प्रत्यागम ( face && FT_IS_SFNT( face ) ) ? ((TT_Face)face)->अङ्कः_नामानि : 0;
  }


  /* documentation is in ftsnames.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Sfnt_Name( FT_Face       face,
                    FT_UInt       idx,
                    FT_SfntName  *aname )
  {
    FT_Error  error = FT_ERR( Invalid_तर्कः );


    यदि ( aname && face && FT_IS_SFNT( face ) )
    {
      TT_Face  ttface = (TT_Face)face;


      यदि ( idx < (FT_UInt)ttface->अङ्कः_नामानि )
      {
        TT_Name  प्रविष्टिः = ttface->name_सारणी.नामानि + idx;


        /* load name on demand */
        यदि ( प्रविष्टिः->string_दीर्घता > 0 && !प्रविष्टिः->string )
        {
          FT_स्मृतिः  स्मृतिः = face->स्मृतिः;
          FT_प्रवाहः  प्रवाहः = face->प्रवाहः;


          यदि ( FT_QNEW_ARRAY ( प्रविष्टिः->string, प्रविष्टिः->string_दीर्घता ) ||
               FT_प्रवाहः_SEEK( प्रविष्टिः->string_विस्थापनम् )                ||
               FT_प्रवाहः_READ( प्रविष्टिः->string, प्रविष्टिः->string_दीर्घता ) )
          {
            FT_FREE( प्रविष्टिः->string );
            प्रविष्टिः->string_दीर्घता = 0;
          }
        }

        aname->platform_id = प्रविष्टिः->platformID;
        aname->encoding_id = प्रविष्टिः->encodingID;
        aname->language_id = प्रविष्टिः->languageID;
        aname->name_id     = प्रविष्टिः->nameID;
        aname->string      = (FT_Byte*)प्रविष्टिः->string;
        aname->string_len  = प्रविष्टिः->string_दीर्घता;

        error = FT_Err_Ok;
      }
    }

    प्रत्यागम error;
  }


  /* documentation is in ftsnames.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Sfnt_Lang_चिह्नकम्( FT_Face          face,
                       FT_UInt          langID,
                       FT_Sfnt_Lang_चिह्नकम्  *alang_चिह्नकम् )
  {
    FT_Error  error = FT_ERR( Invalid_तर्कः );


    यदि ( alang_चिह्नकम् && face && FT_IS_SFNT( face ) )
    {
      TT_Face  ttface = (TT_Face)face;


      यदि ( ttface->name_सारणी.format != 1 )
        प्रत्यागम FT_THROW( Invalid_सारणी );

      यदि ( langID > 0x8000U                                        &&
           langID - 0x8000U < ttface->name_सारणी.अङ्कः_Lang_चिह्नकम्_लेखाः )
      {
        TT_Lang_चिह्नकम्  प्रविष्टिः = ttface->name_सारणी.lang_चिह्नकानि +
                            ( langID - 0x8000U );


        /* load name on demand */
        यदि ( प्रविष्टिः->string_दीर्घता > 0 && !प्रविष्टिः->string )
        {
          FT_स्मृतिः  स्मृतिः = face->स्मृतिः;
          FT_प्रवाहः  प्रवाहः = face->प्रवाहः;


          यदि ( FT_QNEW_ARRAY ( प्रविष्टिः->string, प्रविष्टिः->string_दीर्घता ) ||
               FT_प्रवाहः_SEEK( प्रविष्टिः->string_विस्थापनम् )                ||
               FT_प्रवाहः_READ( प्रविष्टिः->string, प्रविष्टिः->string_दीर्घता ) )
          {
            FT_FREE( प्रविष्टिः->string );
            प्रविष्टिः->string_दीर्घता = 0;
          }
        }

        alang_चिह्नकम्->string     = (FT_Byte*)प्रविष्टिः->string;
        alang_चिह्नकम्->string_len = प्रविष्टिः->string_दीर्घता;

        error = FT_Err_Ok;
      }
    }

    प्रत्यागम error;
  }


#अन्यथा /* !TT_CONFIG_OPTION_SFNT_NAMES */


  FT_EXPORT_DEF( FT_UInt )
  FT_Get_Sfnt_Name_गणनम्( FT_Face  face )
  {
    FT_अप्रयुक्तम्( face );

    प्रत्यागम 0;
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Get_Sfnt_Name( FT_Face       face,
                    FT_UInt       idx,
                    FT_SfntName  *aname )
  {
    FT_अप्रयुक्तम्( face );
    FT_अप्रयुक्तम्( idx );
    FT_अप्रयुक्तम्( aname );

    प्रत्यागम FT_THROW( Unimplemented_Feature );
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Get_Sfnt_Lang_चिह्नकम्( FT_Face          face,
                       FT_UInt          langID,
                       FT_Sfnt_Lang_चिह्नकम्  *alang_चिह्नकम् )
  {
    FT_अप्रयुक्तम्( face );
    FT_अप्रयुक्तम्( langID );
    FT_अप्रयुक्तम्( alang_चिह्नकम् );

    प्रत्यागम FT_THROW( Unimplemented_Feature );
  }


#यदि_अन्तः /* !TT_CONFIG_OPTION_SFNT_NAMES */


/*अन्तः*/
