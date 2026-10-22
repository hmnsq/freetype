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


#جيڪڏهن_وصف_ٿيل TT_CONFIG_OPTION_SFNT_نالا


  /* documentation is in ftsnames.h */

  FT_EXPORT_DEF( FT_UInt )
  FT_Get_Sfnt_Name_ڳڻپ( FT_Face  face )
  {
    أرجع ( face && FT_IS_SFNT( face ) ) ? ((TT_Face)face)->انگ_نالا : 0;
  }


  /* documentation is in ftsnames.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Sfnt_Name( FT_Face       face,
                    FT_UInt       idx,
                    FT_SfntName  *aname )
  {
    FT_Error  error = FT_ERR( Invalid_دليل );


    إذا ( aname && face && FT_IS_SFNT( face ) )
    {
      TT_Face  ttface = (TT_Face)face;


      إذا ( idx < (FT_UInt)ttface->انگ_نالا )
      {
        TT_Name  داخلا = ttface->name_جدول.نالا + idx;


        /* load name on demand */
        إذا ( داخلا->string_ڊيگهائي > 0 && !داخلا->string )
        {
          FT_يادگيري  يادگيري = face->يادگيري;
          FT_وهڪرو  وهڪرو = face->وهڪرو;


          إذا ( FT_QNEW_ARRAY ( داخلا->string, داخلا->string_ڊيگهائي ) ||
               FT_وهڪرو_SEEK( داخلا->string_وٿي )                ||
               FT_وهڪرو_READ( داخلا->string, داخلا->string_ڊيگهائي ) )
          {
            FT_FREE( داخلا->string );
            داخلا->string_ڊيگهائي = 0;
          }
        }

        aname->platform_id = داخلا->platformID;
        aname->encoding_id = داخلا->encodingID;
        aname->language_id = داخلا->languageID;
        aname->name_id     = داخلا->nameID;
        aname->string      = (FT_Byte*)داخلا->string;
        aname->string_len  = داخلا->string_ڊيگهائي;

        error = FT_Err_Ok;
      }
    }

    أرجع error;
  }


  /* documentation is in ftsnames.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Sfnt_Lang_ٽيگ( FT_Face          face,
                       FT_UInt          langID,
                       FT_Sfnt_Lang_ٽيگ  *alang_ٽيگ )
  {
    FT_Error  error = FT_ERR( Invalid_دليل );


    إذا ( alang_ٽيگ && face && FT_IS_SFNT( face ) )
    {
      TT_Face  ttface = (TT_Face)face;


      إذا ( ttface->name_جدول.format != 1 )
        أرجع FT_THROW( Invalid_جدول );

      إذا ( langID > 0x8000U                                        &&
           langID - 0x8000U < ttface->name_جدول.انگ_Lang_ٽيگ_رڪارڊون )
      {
        TT_Lang_ٽيگ  داخلا = ttface->name_جدول.lang_ٽيگ_فهرست +
                            ( langID - 0x8000U );


        /* load name on demand */
        إذا ( داخلا->string_ڊيگهائي > 0 && !داخلا->string )
        {
          FT_يادگيري  يادگيري = face->يادگيري;
          FT_وهڪرو  وهڪرو = face->وهڪرو;


          إذا ( FT_QNEW_ARRAY ( داخلا->string, داخلا->string_ڊيگهائي ) ||
               FT_وهڪرو_SEEK( داخلا->string_وٿي )                ||
               FT_وهڪرو_READ( داخلا->string, داخلا->string_ڊيگهائي ) )
          {
            FT_FREE( داخلا->string );
            داخلا->string_ڊيگهائي = 0;
          }
        }

        alang_ٽيگ->string     = (FT_Byte*)داخلا->string;
        alang_ٽيگ->string_len = داخلا->string_ڊيگهائي;

        error = FT_Err_Ok;
      }
    }

    أرجع error;
  }


#وإلا /* !TT_CONFIG_OPTION_SFNT_NAMES */


  FT_EXPORT_DEF( FT_UInt )
  FT_Get_Sfnt_Name_ڳڻپ( FT_Face  face )
  {
    FT_اڻ_استعمال_ٿيل( face );

    أرجع 0;
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Get_Sfnt_Name( FT_Face       face,
                    FT_UInt       idx,
                    FT_SfntName  *aname )
  {
    FT_اڻ_استعمال_ٿيل( face );
    FT_اڻ_استعمال_ٿيل( idx );
    FT_اڻ_استعمال_ٿيل( aname );

    أرجع FT_THROW( Unimplemented_Feature );
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Get_Sfnt_Lang_ٽيگ( FT_Face          face,
                       FT_UInt          langID,
                       FT_Sfnt_Lang_ٽيگ  *alang_ٽيگ )
  {
    FT_اڻ_استعمال_ٿيل( face );
    FT_اڻ_استعمال_ٿيل( langID );
    FT_اڻ_استعمال_ٿيل( alang_ٽيگ );

    أرجع FT_THROW( Unimplemented_Feature );
  }


#جيڪڏهن_پورو /* !TT_CONFIG_OPTION_SFNT_NAMES */


/*پڇاڙي*/
