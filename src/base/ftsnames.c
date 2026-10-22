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


#అన్తర్భావ <freetype/internal/ftdebug.h>

#అన్తర్భావ <freetype/ftsnames.h>
#అన్తర్భావ <freetype/internal/tttypes.h>
#అన్తర్భావ <freetype/internal/ftstream.h>


#నిర్వచితమైతే TT_CONFIG_OPTION_SFNT_పేర్లు


  /* documentation is in ftsnames.h */

  FT_EXPORT_DEF( FT_UInt )
  FT_Get_Sfnt_Name_లెక్క( FT_Face  face )
  {
    తిరిగి ( face && FT_IS_SFNT( face ) ) ? ((TT_Face)face)->అంకె_పేర్లు : 0;
  }


  /* documentation is in ftsnames.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Sfnt_Name( FT_Face       face,
                    FT_UInt       idx,
                    FT_SfntName  *aname )
  {
    FT_Error  error = FT_ERR( Invalid_వాదం );


    ఒకవేళ ( aname && face && FT_IS_SFNT( face ) )
    {
      TT_Face  ttface = (TT_Face)face;


      ఒకవేళ ( idx < (FT_UInt)ttface->అంకె_పేర్లు )
      {
        TT_Name  నమోదు_2 = ttface->name_పట్టిక.పేర్లు + idx;


        /* load name on demand */
        ఒకవేళ ( నమోదు_2->string_నిడివి > 0 && !నమోదు_2->string )
        {
          FT_జ్ఞాపకం  జ్ఞాపకం = face->జ్ఞాపకం;
          FT_ప్రవాహం  ప్రవాహం = face->ప్రవాహం;


          ఒకవేళ ( FT_QNEW_ARRAY ( నమోదు_2->string, నమోదు_2->string_నిడివి ) ||
               FT_ప్రవాహం_SEEK( నమోదు_2->string_స్థానభ్రంశం )                ||
               FT_ప్రవాహం_READ( నమోదు_2->string, నమోదు_2->string_నిడివి ) )
          {
            FT_FREE( నమోదు_2->string );
            నమోదు_2->string_నిడివి = 0;
          }
        }

        aname->platform_id = నమోదు_2->platformID;
        aname->encoding_id = నమోదు_2->encodingID;
        aname->language_id = నమోదు_2->languageID;
        aname->name_id     = నమోదు_2->nameID;
        aname->string      = (FT_Byte*)నమోదు_2->string;
        aname->string_len  = నమోదు_2->string_నిడివి;

        error = FT_Err_Ok;
      }
    }

    తిరిగి error;
  }


  /* documentation is in ftsnames.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Sfnt_Lang_ట్యాగ్( FT_Face          face,
                       FT_UInt          langID,
                       FT_Sfnt_Lang_ట్యాగ్  *alang_ట్యాగ్ )
  {
    FT_Error  error = FT_ERR( Invalid_వాదం );


    ఒకవేళ ( alang_ట్యాగ్ && face && FT_IS_SFNT( face ) )
    {
      TT_Face  ttface = (TT_Face)face;


      ఒకవేళ ( ttface->name_పట్టిక.format != 1 )
        తిరిగి FT_THROW( Invalid_పట్టిక );

      ఒకవేళ ( langID > 0x8000U                                        &&
           langID - 0x8000U < ttface->name_పట్టిక.అంకె_Lang_ట్యాగ్_రికార్డులు )
      {
        TT_Lang_ట్యాగ్  నమోదు_2 = ttface->name_పట్టిక.lang_ట్యాగులు +
                            ( langID - 0x8000U );


        /* load name on demand */
        ఒకవేళ ( నమోదు_2->string_నిడివి > 0 && !నమోదు_2->string )
        {
          FT_జ్ఞాపకం  జ్ఞాపకం = face->జ్ఞాపకం;
          FT_ప్రవాహం  ప్రవాహం = face->ప్రవాహం;


          ఒకవేళ ( FT_QNEW_ARRAY ( నమోదు_2->string, నమోదు_2->string_నిడివి ) ||
               FT_ప్రవాహం_SEEK( నమోదు_2->string_స్థానభ్రంశం )                ||
               FT_ప్రవాహం_READ( నమోదు_2->string, నమోదు_2->string_నిడివి ) )
          {
            FT_FREE( నమోదు_2->string );
            నమోదు_2->string_నిడివి = 0;
          }
        }

        alang_ట్యాగ్->string     = (FT_Byte*)నమోదు_2->string;
        alang_ట్యాగ్->string_len = నమోదు_2->string_నిడివి;

        error = FT_Err_Ok;
      }
    }

    తిరిగి error;
  }


#లేకపోతే /* !TT_CONFIG_OPTION_SFNT_NAMES */


  FT_EXPORT_DEF( FT_UInt )
  FT_Get_Sfnt_Name_లెక్క( FT_Face  face )
  {
    FT_వాడని( face );

    తిరిగి 0;
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Get_Sfnt_Name( FT_Face       face,
                    FT_UInt       idx,
                    FT_SfntName  *aname )
  {
    FT_వాడని( face );
    FT_వాడని( idx );
    FT_వాడని( aname );

    తిరిగి FT_THROW( Unimplemented_Feature );
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Get_Sfnt_Lang_ట్యాగ్( FT_Face          face,
                       FT_UInt          langID,
                       FT_Sfnt_Lang_ట్యాగ్  *alang_ట్యాగ్ )
  {
    FT_వాడని( face );
    FT_వాడని( langID );
    FT_వాడని( alang_ట్యాగ్ );

    తిరిగి FT_THROW( Unimplemented_Feature );
  }


#అయితే_ముగింపు /* !TT_CONFIG_OPTION_SFNT_NAMES */


/*చివర*/
