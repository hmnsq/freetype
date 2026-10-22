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


#ifdef TT_CONFIG_OPTION_SFNT_NAMES


  /* documentation is in ftsnames.h */

  FT_EXPORT_DEF( FT_UInt )
  FT_Get_Sfnt_Name_गणनम्( FT_Face  face )
  {
    प्रत्यागम ( face && FT_IS_SFNT( face ) ) ? ((TT_Face)face)->अङ्कः_names : 0;
  }


  /* documentation is in ftsnames.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Sfnt_Name( FT_Face       face,
                    FT_UInt       idx,
                    FT_SfntName  *aname )
  {
    FT_Error  error = FT_ERR( Invalid_Argument );


    जइ ( aname && face && FT_IS_SFNT( face ) )
    {
      TT_Face  ttface = (TT_Face)face;


      जइ ( idx < (FT_UInt)ttface->अङ्कः_names )
      {
        TT_Name  entry = ttface->name_table.names + idx;


        /* load name on demand */
        जइ ( entry->stringLength > 0 && !entry->string )
        {
          FT_Memory  memory = face->memory;
          FT_Stream  stream = face->stream;


          जइ ( FT_QNEW_ARRAY ( entry->string, entry->stringLength ) ||
               FT_STREAM_SEEK( entry->stringOffset )                ||
               FT_STREAM_READ( entry->string, entry->stringLength ) )
          {
            FT_FREE( entry->string );
            entry->stringLength = 0;
          }
        }

        aname->platform_id = entry->platformID;
        aname->encoding_id = entry->encodingID;
        aname->language_id = entry->languageID;
        aname->name_id     = entry->nameID;
        aname->string      = (FT_Byte*)entry->string;
        aname->string_len  = entry->stringLength;

        error = FT_Err_Ok;
      }
    }

    प्रत्यागम error;
  }


  /* documentation is in ftsnames.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Sfnt_LangTag( FT_Face          face,
                       FT_UInt          langID,
                       FT_SfntLangTag  *alangTag )
  {
    FT_Error  error = FT_ERR( Invalid_Argument );


    जइ ( alangTag && face && FT_IS_SFNT( face ) )
    {
      TT_Face  ttface = (TT_Face)face;


      जइ ( ttface->name_table.format != 1 )
        प्रत्यागम FT_THROW( Invalid_Table );

      जइ ( langID > 0x8000U                                        &&
           langID - 0x8000U < ttface->name_table.अङ्कः_Lang_Tag_Records )
      {
        TT_LangTag  entry = ttface->name_table.langTags +
                            ( langID - 0x8000U );


        /* load name on demand */
        जइ ( entry->stringLength > 0 && !entry->string )
        {
          FT_Memory  memory = face->memory;
          FT_Stream  stream = face->stream;


          जइ ( FT_QNEW_ARRAY ( entry->string, entry->stringLength ) ||
               FT_STREAM_SEEK( entry->stringOffset )                ||
               FT_STREAM_READ( entry->string, entry->stringLength ) )
          {
            FT_FREE( entry->string );
            entry->stringLength = 0;
          }
        }

        alangTag->string     = (FT_Byte*)entry->string;
        alangTag->string_len = entry->stringLength;

        error = FT_Err_Ok;
      }
    }

    प्रत्यागम error;
  }


#अण्णहा /* !TT_CONFIG_OPTION_SFNT_NAMES */


  FT_EXPORT_DEF( FT_UInt )
  FT_Get_Sfnt_Name_गणनम्( FT_Face  face )
  {
    FT_UNUSED( face );

    प्रत्यागम 0;
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Get_Sfnt_Name( FT_Face       face,
                    FT_UInt       idx,
                    FT_SfntName  *aname )
  {
    FT_UNUSED( face );
    FT_UNUSED( idx );
    FT_UNUSED( aname );

    प्रत्यागम FT_THROW( Unimplemented_Feature );
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Get_Sfnt_LangTag( FT_Face          face,
                       FT_UInt          langID,
                       FT_SfntLangTag  *alangTag )
  {
    FT_UNUSED( face );
    FT_UNUSED( langID );
    FT_UNUSED( alangTag );

    प्रत्यागम FT_THROW( Unimplemented_Feature );
  }


#endif /* !TT_CONFIG_OPTION_SFNT_NAMES */


/* END */
