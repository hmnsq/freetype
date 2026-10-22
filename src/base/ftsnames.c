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


#شامل <freetype/internal/ftdebug.h>

#شامل <freetype/ftsnames.h>
#شامل <freetype/internal/tttypes.h>
#شامل <freetype/internal/ftstream.h>


#اگر_تعریف_شدہ TT_CONFIG_OPTION_SFNT_نام


  /* documentation is in ftsnames.h */

  FT_EXPORT_DEF( FT_UInt )
  FT_Get_Sfnt_Name_گنتی( FT_Face  face )
  {
    واپس ( face && FT_IS_SFNT( face ) ) ? ((TT_Face)face)->عدد_نام : 0;
  }


  /* documentation is in ftsnames.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Sfnt_Name( FT_Face       face,
                    FT_UInt       idx,
                    FT_SfntName  *aname )
  {
    FT_Error  error = FT_ERR( Invalid_دلیل );


    اگر ( aname && face && FT_IS_SFNT( face ) )
    {
      TT_Face  ttface = (TT_Face)face;


      اگر ( idx < (FT_UInt)ttface->عدد_نام )
      {
        TT_Name  اندراج_2 = ttface->name_جدول.نام + idx;


        /* load name on demand */
        اگر ( اندراج_2->string_لمبائی > 0 && !اندراج_2->string )
        {
          FT_حافظہ  حافظہ = face->حافظہ;
          FT_دھارا  دھارا = face->دھارا;


          اگر ( FT_QNEW_ARRAY ( اندراج_2->string, اندراج_2->string_لمبائی ) ||
               FT_دھارا_SEEK( اندراج_2->string_انحراف )                ||
               FT_دھارا_READ( اندراج_2->string, اندراج_2->string_لمبائی ) )
          {
            FT_FREE( اندراج_2->string );
            اندراج_2->string_لمبائی = 0;
          }
        }

        aname->platform_id = اندراج_2->platformID;
        aname->encoding_id = اندراج_2->encodingID;
        aname->language_id = اندراج_2->languageID;
        aname->name_id     = اندراج_2->nameID;
        aname->string      = (FT_Byte*)اندراج_2->string;
        aname->string_len  = اندراج_2->string_لمبائی;

        error = FT_Err_Ok;
      }
    }

    واپس error;
  }


  /* documentation is in ftsnames.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Sfnt_Lang_لیبل( FT_Face          face,
                       FT_UInt          langID,
                       FT_Sfnt_Lang_لیبل  *alang_لیبل )
  {
    FT_Error  error = FT_ERR( Invalid_دلیل );


    اگر ( alang_لیبل && face && FT_IS_SFNT( face ) )
    {
      TT_Face  ttface = (TT_Face)face;


      اگر ( ttface->name_جدول.format != 1 )
        واپس FT_THROW( Invalid_جدول );

      اگر ( langID > 0x8000U                                        &&
           langID - 0x8000U < ttface->name_جدول.عدد_Lang_لیبل_اندراجات )
      {
        TT_Lang_لیبل  اندراج_2 = ttface->name_جدول.lang_لیبل_فہرست +
                            ( langID - 0x8000U );


        /* load name on demand */
        اگر ( اندراج_2->string_لمبائی > 0 && !اندراج_2->string )
        {
          FT_حافظہ  حافظہ = face->حافظہ;
          FT_دھارا  دھارا = face->دھارا;


          اگر ( FT_QNEW_ARRAY ( اندراج_2->string, اندراج_2->string_لمبائی ) ||
               FT_دھارا_SEEK( اندراج_2->string_انحراف )                ||
               FT_دھارا_READ( اندراج_2->string, اندراج_2->string_لمبائی ) )
          {
            FT_FREE( اندراج_2->string );
            اندراج_2->string_لمبائی = 0;
          }
        }

        alang_لیبل->string     = (FT_Byte*)اندراج_2->string;
        alang_لیبل->string_len = اندراج_2->string_لمبائی;

        error = FT_Err_Ok;
      }
    }

    واپس error;
  }


#ورنہ /* !TT_CONFIG_OPTION_SFNT_NAMES */


  FT_EXPORT_DEF( FT_UInt )
  FT_Get_Sfnt_Name_گنتی( FT_Face  face )
  {
    FT_غیر_مستعمل( face );

    واپس 0;
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Get_Sfnt_Name( FT_Face       face,
                    FT_UInt       idx,
                    FT_SfntName  *aname )
  {
    FT_غیر_مستعمل( face );
    FT_غیر_مستعمل( idx );
    FT_غیر_مستعمل( aname );

    واپس FT_THROW( Unimplemented_Feature );
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Get_Sfnt_Lang_لیبل( FT_Face          face,
                       FT_UInt          langID,
                       FT_Sfnt_Lang_لیبل  *alang_لیبل )
  {
    FT_غیر_مستعمل( face );
    FT_غیر_مستعمل( langID );
    FT_غیر_مستعمل( alang_لیبل );

    واپس FT_THROW( Unimplemented_Feature );
  }


#اگر_ختم /* !TT_CONFIG_OPTION_SFNT_NAMES */


/*انتہا*/
