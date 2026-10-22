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


#ضمن <freetype/internal/ftdebug.h>

#ضمن <freetype/ftsnames.h>
#ضمن <freetype/internal/tttypes.h>
#ضمن <freetype/internal/ftstream.h>


#جے_تعریف_شدہ TT_CONFIG_OPTION_SFNT_ناں


  /* documentation is in ftsnames.h */

  FT_EXPORT_DEF( FT_UInt )
  FT_Get_Sfnt_Name_گنتی( FT_Face  face )
  {
    أرجع ( face && FT_IS_SFNT( face ) ) ? ((TT_Face)face)->عدد_ناں : 0;
  }


  /* documentation is in ftsnames.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Sfnt_Name( FT_Face       face,
                    FT_UInt       idx,
                    FT_SfntName  *aname )
  {
    FT_Error  error = FT_ERR( Invalid_دلیل );


    إذا ( aname && face && FT_IS_SFNT( face ) )
    {
      TT_Face  ttface = (TT_Face)face;


      إذا ( idx < (FT_UInt)ttface->عدد_ناں )
      {
        TT_Name  اندراج = ttface->name_جدول.ناں + idx;


        /* load name on demand */
        إذا ( اندراج->string_لمائی > 0 && !اندراج->string )
        {
          FT_حافظہ  حافظہ = face->حافظہ;
          FT_دھارا  دھارا = face->دھارا;


          إذا ( FT_QNEW_ARRAY ( اندراج->string, اندراج->string_لمائی ) ||
               FT_دھارا_SEEK( اندراج->string_وتھ )                ||
               FT_دھارا_READ( اندراج->string, اندراج->string_لمائی ) )
          {
            FT_FREE( اندراج->string );
            اندراج->string_لمائی = 0;
          }
        }

        aname->platform_id = اندراج->platformID;
        aname->encoding_id = اندراج->encodingID;
        aname->language_id = اندراج->languageID;
        aname->name_id     = اندراج->nameID;
        aname->string      = (FT_Byte*)اندراج->string;
        aname->string_len  = اندراج->string_لمائی;

        error = FT_Err_Ok;
      }
    }

    أرجع error;
  }


  /* documentation is in ftsnames.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Sfnt_Lang_ٹیگ( FT_Face          face,
                       FT_UInt          langID,
                       FT_Sfnt_Lang_ٹیگ  *alang_ٹیگ )
  {
    FT_Error  error = FT_ERR( Invalid_دلیل );


    إذا ( alang_ٹیگ && face && FT_IS_SFNT( face ) )
    {
      TT_Face  ttface = (TT_Face)face;


      إذا ( ttface->name_جدول.format != 1 )
        أرجع FT_THROW( Invalid_جدول );

      إذا ( langID > 0x8000U                                        &&
           langID - 0x8000U < ttface->name_جدول.عدد_Lang_ٹیگ_ریکارڈاں )
      {
        TT_Lang_ٹیگ  اندراج = ttface->name_جدول.lang_ٹیگ_سوچی +
                            ( langID - 0x8000U );


        /* load name on demand */
        إذا ( اندراج->string_لمائی > 0 && !اندراج->string )
        {
          FT_حافظہ  حافظہ = face->حافظہ;
          FT_دھارا  دھارا = face->دھارا;


          إذا ( FT_QNEW_ARRAY ( اندراج->string, اندراج->string_لمائی ) ||
               FT_دھارا_SEEK( اندراج->string_وتھ )                ||
               FT_دھارا_READ( اندراج->string, اندراج->string_لمائی ) )
          {
            FT_FREE( اندراج->string );
            اندراج->string_لمائی = 0;
          }
        }

        alang_ٹیگ->string     = (FT_Byte*)اندراج->string;
        alang_ٹیگ->string_len = اندراج->string_لمائی;

        error = FT_Err_Ok;
      }
    }

    أرجع error;
  }


#وإلا /* !TT_CONFIG_OPTION_SFNT_NAMES */


  FT_EXPORT_DEF( FT_UInt )
  FT_Get_Sfnt_Name_گنتی( FT_Face  face )
  {
    FT_بن_ورتیا( face );

    أرجع 0;
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Get_Sfnt_Name( FT_Face       face,
                    FT_UInt       idx,
                    FT_SfntName  *aname )
  {
    FT_بن_ورتیا( face );
    FT_بن_ورتیا( idx );
    FT_بن_ورتیا( aname );

    أرجع FT_THROW( Unimplemented_Feature );
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Get_Sfnt_Lang_ٹیگ( FT_Face          face,
                       FT_UInt          langID,
                       FT_Sfnt_Lang_ٹیگ  *alang_ٹیگ )
  {
    FT_بن_ورتیا( face );
    FT_بن_ورتیا( langID );
    FT_بن_ورتیا( alang_ٹیگ );

    أرجع FT_THROW( Unimplemented_Feature );
  }


#جے_انت /* !TT_CONFIG_OPTION_SFNT_NAMES */


/*سرا*/
