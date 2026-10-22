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


#اگر_تعریف_شده TT_CONFIG_OPTION_SFNT_نامها


  /* documentation is in ftsnames.h */

  FT_EXPORT_DEF( FT_UInt )
  FT_Get_Sfnt_Name_شمار( FT_Face  face )
  {
    أرجع ( face && FT_IS_SFNT( face ) ) ? ((TT_Face)face)->رقم_نامها : 0;
  }


  /* documentation is in ftsnames.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Sfnt_Name( FT_Face       face,
                    FT_UInt       idx,
                    FT_SfntName  *aname )
  {
    FT_Error  error = FT_ERR( Invalid_آرگومان );


    إذا ( aname && face && FT_IS_SFNT( face ) )
    {
      TT_Face  ttface = (TT_Face)face;


      إذا ( idx < (FT_UInt)ttface->رقم_نامها )
      {
        TT_Name  مدخل = ttface->name_جدول.نامها + idx;


        /* load name on demand */
        إذا ( مدخل->string_درازی > 0 && !مدخل->string )
        {
          FT_حافظه  حافظه = face->حافظه;
          FT_جریان  جریان = face->جریان;


          إذا ( FT_QNEW_ARRAY ( مدخل->string, مدخل->string_درازی ) ||
               FT_جریان_SEEK( مدخل->string_جابه_جایی )                ||
               FT_جریان_READ( مدخل->string, مدخل->string_درازی ) )
          {
            FT_FREE( مدخل->string );
            مدخل->string_درازی = 0;
          }
        }

        aname->platform_id = مدخل->platformID;
        aname->encoding_id = مدخل->encodingID;
        aname->language_id = مدخل->languageID;
        aname->name_id     = مدخل->nameID;
        aname->string      = (FT_Byte*)مدخل->string;
        aname->string_len  = مدخل->string_درازی;

        error = FT_Err_Ok;
      }
    }

    أرجع error;
  }


  /* documentation is in ftsnames.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Sfnt_Lang_برچسب( FT_Face          face,
                       FT_UInt          langID,
                       FT_Sfnt_Lang_برچسب  *alang_برچسب )
  {
    FT_Error  error = FT_ERR( Invalid_آرگومان );


    إذا ( alang_برچسب && face && FT_IS_SFNT( face ) )
    {
      TT_Face  ttface = (TT_Face)face;


      إذا ( ttface->name_جدول.format != 1 )
        أرجع FT_THROW( Invalid_جدول );

      إذا ( langID > 0x8000U                                        &&
           langID - 0x8000U < ttface->name_جدول.رقم_Lang_برچسب_رکوردها )
      {
        TT_Lang_برچسب  مدخل = ttface->name_جدول.lang_برچسبها +
                            ( langID - 0x8000U );


        /* load name on demand */
        إذا ( مدخل->string_درازی > 0 && !مدخل->string )
        {
          FT_حافظه  حافظه = face->حافظه;
          FT_جریان  جریان = face->جریان;


          إذا ( FT_QNEW_ARRAY ( مدخل->string, مدخل->string_درازی ) ||
               FT_جریان_SEEK( مدخل->string_جابه_جایی )                ||
               FT_جریان_READ( مدخل->string, مدخل->string_درازی ) )
          {
            FT_FREE( مدخل->string );
            مدخل->string_درازی = 0;
          }
        }

        alang_برچسب->string     = (FT_Byte*)مدخل->string;
        alang_برچسب->string_len = مدخل->string_درازی;

        error = FT_Err_Ok;
      }
    }

    أرجع error;
  }


#وإلا /* !TT_CONFIG_OPTION_SFNT_NAMES */


  FT_EXPORT_DEF( FT_UInt )
  FT_Get_Sfnt_Name_شمار( FT_Face  face )
  {
    FT_استفادهنشده( face );

    أرجع 0;
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Get_Sfnt_Name( FT_Face       face,
                    FT_UInt       idx,
                    FT_SfntName  *aname )
  {
    FT_استفادهنشده( face );
    FT_استفادهنشده( idx );
    FT_استفادهنشده( aname );

    أرجع FT_THROW( Unimplemented_Feature );
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Get_Sfnt_Lang_برچسب( FT_Face          face,
                       FT_UInt          langID,
                       FT_Sfnt_Lang_برچسب  *alang_برچسب )
  {
    FT_استفادهنشده( face );
    FT_استفادهنشده( langID );
    FT_استفادهنشده( alang_برچسب );

    أرجع FT_THROW( Unimplemented_Feature );
  }


#پایان_اگر /* !TT_CONFIG_OPTION_SFNT_NAMES */


/*انتها*/
