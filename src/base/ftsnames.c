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


#که_تعریف_شوی TT_CONFIG_OPTION_SFNT_نومونه


  /* documentation is in ftsnames.h */

  FT_EXPORT_DEF( FT_UInt )
  FT_Get_Sfnt_Name_شمېر( FT_Face  face )
  {
    أرجع ( face && FT_IS_SFNT( face ) ) ? ((TT_Face)face)->عدد_نومونه : 0;
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


      إذا ( idx < (FT_UInt)ttface->عدد_نومونه )
      {
        TT_Name  ثبت = ttface->name_جدول.نومونه + idx;


        /* load name on demand */
        إذا ( ثبت->string_اوږدتیا > 0 && !ثبت->string )
        {
          FT_حافظه  حافظه = face->حافظه;
          FT_بهیر  بهیر = face->بهیر;


          إذا ( FT_QNEW_ARRAY ( ثبت->string, ثبت->string_اوږدتیا ) ||
               FT_بهیر_SEEK( ثبت->string_دننه_واټن )                ||
               FT_بهیر_READ( ثبت->string, ثبت->string_اوږدتیا ) )
          {
            FT_FREE( ثبت->string );
            ثبت->string_اوږدتیا = 0;
          }
        }

        aname->platform_id = ثبت->platformID;
        aname->encoding_id = ثبت->encodingID;
        aname->language_id = ثبت->languageID;
        aname->name_id     = ثبت->nameID;
        aname->string      = (FT_Byte*)ثبت->string;
        aname->string_len  = ثبت->string_اوږدتیا;

        error = FT_Err_Ok;
      }
    }

    أرجع error;
  }


  /* documentation is in ftsnames.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Sfnt_Lang_نښان( FT_Face          face,
                       FT_UInt          langID,
                       FT_Sfnt_Lang_نښان  *alang_نښان )
  {
    FT_Error  error = FT_ERR( Invalid_دلیل );


    إذا ( alang_نښان && face && FT_IS_SFNT( face ) )
    {
      TT_Face  ttface = (TT_Face)face;


      إذا ( ttface->name_جدول.format != 1 )
        أرجع FT_THROW( Invalid_جدول );

      إذا ( langID > 0x8000U                                        &&
           langID - 0x8000U < ttface->name_جدول.عدد_Lang_نښان_ریکارډونه )
      {
        TT_Lang_نښان  ثبت = ttface->name_جدول.lang_نښانونه +
                            ( langID - 0x8000U );


        /* load name on demand */
        إذا ( ثبت->string_اوږدتیا > 0 && !ثبت->string )
        {
          FT_حافظه  حافظه = face->حافظه;
          FT_بهیر  بهیر = face->بهیر;


          إذا ( FT_QNEW_ARRAY ( ثبت->string, ثبت->string_اوږدتیا ) ||
               FT_بهیر_SEEK( ثبت->string_دننه_واټن )                ||
               FT_بهیر_READ( ثبت->string, ثبت->string_اوږدتیا ) )
          {
            FT_FREE( ثبت->string );
            ثبت->string_اوږدتیا = 0;
          }
        }

        alang_نښان->string     = (FT_Byte*)ثبت->string;
        alang_نښان->string_len = ثبت->string_اوږدتیا;

        error = FT_Err_Ok;
      }
    }

    أرجع error;
  }


#وإلا /* !TT_CONFIG_OPTION_SFNT_NAMES */


  FT_EXPORT_DEF( FT_UInt )
  FT_Get_Sfnt_Name_شمېر( FT_Face  face )
  {
    FT_ناکارول_شوی( face );

    أرجع 0;
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Get_Sfnt_Name( FT_Face       face,
                    FT_UInt       idx,
                    FT_SfntName  *aname )
  {
    FT_ناکارول_شوی( face );
    FT_ناکارول_شوی( idx );
    FT_ناکارول_شوی( aname );

    أرجع FT_THROW( Unimplemented_Feature );
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Get_Sfnt_Lang_نښان( FT_Face          face,
                       FT_UInt          langID,
                       FT_Sfnt_Lang_نښان  *alang_نښان )
  {
    FT_ناکارول_شوی( face );
    FT_ناکارول_شوی( langID );
    FT_ناکارول_شوی( alang_نښان );

    أرجع FT_THROW( Unimplemented_Feature );
  }


#د_که_پای /* !TT_CONFIG_OPTION_SFNT_NAMES */


/*پای_ټکی*/
