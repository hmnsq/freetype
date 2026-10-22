/****************************************************************************
 *
 * ftrfork.c
 *
 *   Embedded resource forks accessor (body).
 *
 * Copyright (C) 2004-2026 by
 * Masatake YAMATO and Redhat K.K.
 *
 * FT_Raccess_Get_HeaderInfo() and raccess_guess_darwin_hfsplus() are
 * derived from ftobjs.c.
 *
 * This file is part of the FreeType project, and may only be used,
 * modified, and distributed under the terms of the FreeType project
 * license, LICENSE.TXT.  By continuing to use, modify, or distribute
 * this file you indicate that you have read the license and
 * understand and accept it fully.
 *
 */

/****************************************************************************
 * Development of the code in this file is support of
 * Information-technology Promotion Agency, Japan.
 */


#अन्तर्भाव <freetype/internal/ftdebug.h>
#अन्तर्भाव <freetype/internal/ftstream.h>
#अन्तर्भाव <freetype/internal/ftrfork.h>

#अन्तर्भाव "ftbase.h"

#अपरिभाषय  FT_COMPONENT
#परिभाषय FT_COMPONENT  raccess


  /*************************************************************************/
  /*************************************************************************/
  /*************************************************************************/
  /****                                                                 ****/
  /****                                                                 ****/
  /****               Resource fork directory access                    ****/
  /****                                                                 ****/
  /****                                                                 ****/
  /*************************************************************************/
  /*************************************************************************/
  /*************************************************************************/

  FT_आधारः_DEF( FT_Error )
  FT_Raccess_Get_शीर्षकम्_Info( FT_Library  library,
                             FT_प्रवाहः   प्रवाहः,
                             FT_Long     rfork_विस्थापनम्,
                             FT_Long    *map_विस्थापनम्,
                             FT_Long    *rdata_pos )
  {
    FT_Error       error;
    अचिह्नित वर्ण  शीर्ष[16], head2[16];
    FT_Long        map_pos, map_len, rdata_len;
    पूर्णाङ्क            allzeros, allmatch, i;
    FT_Long        type_list;

    FT_अप्रयुक्तम्( library );


    error = FT_प्रवाहः_Seek( प्रवाहः, (FT_ULong)rfork_विस्थापनम् );
    यदि ( error )
      प्रत्यागम error;

    error = FT_प्रवाहः_Read( प्रवाहः, (FT_Byte*)शीर्ष, 16 );
    यदि ( error )
      प्रत्यागम error;

    /* ensure positive values */
    यदि ( शीर्ष[0]  >= 0x80 ||
         शीर्ष[4]  >= 0x80 ||
         शीर्ष[8]  >= 0x80 ||
         शीर्ष[12] >= 0x80 )
      प्रत्यागम FT_THROW( Unknown_File_Format );

    *rdata_pos = ( शीर्ष[ 0] << 24 ) |
                 ( शीर्ष[ 1] << 16 ) |
                 ( शीर्ष[ 2] <<  8 ) |
                   शीर्ष[ 3];
    map_pos    = ( शीर्ष[ 4] << 24 ) |
                 ( शीर्ष[ 5] << 16 ) |
                 ( शीर्ष[ 6] <<  8 ) |
                   शीर्ष[ 7];
    rdata_len  = ( शीर्ष[ 8] << 24 ) |
                 ( शीर्ष[ 9] << 16 ) |
                 ( शीर्ष[10] <<  8 ) |
                   शीर्ष[11];
    map_len    = ( शीर्ष[12] << 24 ) |
                 ( शीर्ष[13] << 16 ) |
                 ( शीर्ष[14] <<  8 ) |
                   शीर्ष[15];

    /* the map must not be empty */
    यदि ( !map_pos )
      प्रत्यागम FT_THROW( Unknown_File_Format );

    /* check whether rdata and map overlap */
    यदि ( *rdata_pos < map_pos )
    {
      यदि ( *rdata_pos > map_pos - rdata_len )
        प्रत्यागम FT_THROW( Unknown_File_Format );
    }
    अन्यथा
    {
      यदि ( map_pos > *rdata_pos - map_len )
        प्रत्यागम FT_THROW( Unknown_File_Format );
    }

    /* check whether end of rdata or map exceeds stream size */
    यदि ( FT_LONG_MAX - rdata_len < *rdata_pos                               ||
         FT_LONG_MAX - map_len < map_pos                                    ||

         FT_LONG_MAX - ( *rdata_pos + rdata_len ) < rfork_विस्थापनम्            ||
         FT_LONG_MAX - ( map_pos + map_len ) < rfork_विस्थापनम्                 ||

         (FT_ULong)( rfork_विस्थापनम् + *rdata_pos + rdata_len ) > प्रवाहः->आकार ||
         (FT_ULong)( rfork_विस्थापनम् + map_pos + map_len ) > प्रवाहः->आकार      )
      प्रत्यागम FT_THROW( Unknown_File_Format );

    *rdata_pos += rfork_विस्थापनम्;
    map_pos    += rfork_विस्थापनम्;

    error = FT_प्रवाहः_Seek( प्रवाहः, (FT_ULong)map_pos );
    यदि ( error )
      प्रत्यागम error;

    head2[15] = (FT_Byte)( शीर्ष[15] + 1 );       /* make it be different */

    error = FT_प्रवाहः_Read( प्रवाहः, (FT_Byte*)head2, 16 );
    यदि ( error )
      प्रत्यागम error;

    allzeros = 1;
    allmatch = 1;
    प्रति ( i = 0; i < 16; i++ )
    {
      यदि ( head2[i] != 0 )
        allzeros = 0;
      यदि ( head2[i] != शीर्ष[i] )
        allmatch = 0;
    }
    यदि ( !allzeros && !allmatch )
      प्रत्यागम FT_THROW( Unknown_File_Format );

    /* If we have reached this point then it is probably a mac resource */
    /* file.  Now, does it contain any interesting resources?           */

    (शून्य)FT_प्रवाहः_SKIP( 4        /* skip handle to next resource map */
                          + 2      /* skip file resource number */
                          + 2 );   /* skip attributes */

    यदि ( FT_READ_SHORT( type_list ) )
      प्रत्यागम error;
    यदि ( type_list < 0 )
      प्रत्यागम FT_THROW( Unknown_File_Format );

    error = FT_प्रवाहः_Seek( प्रवाहः, (FT_ULong)( map_pos + type_list ) );
    यदि ( error )
      प्रत्यागम error;

    *map_विस्थापनम् = map_pos + type_list;
    प्रत्यागम FT_Err_Ok;
  }


  FT_COMPARE_DEF( पूर्णाङ्क )
  ft_raccess_क्रमय_ref_by_id( ध्रुव शून्य*  a,
                             ध्रुव शून्य*  b )
  {
    प्रत्यागम  ( (FT_RFork_Ref*)a )->res_id - ( (FT_RFork_Ref*)b )->res_id;
  }


  FT_आधारः_DEF( FT_Error )
  FT_Raccess_Get_दत्त_Offsets( FT_Library  library,
                              FT_प्रवाहः   प्रवाहः,
                              FT_Long     map_विस्थापनम्,
                              FT_Long     rdata_pos,
                              FT_Long     चिह्नकम्,
                              FT_Bool     क्रमय_by_res_id,
                              FT_Long   **offsets,
                              FT_Long    *गणनम् )
  {
    FT_Error      error;
    पूर्णाङ्क           i, j, cnt, subcnt;
    FT_Long       चिह्नकम्_आन्तरिकम्, rpos;
    FT_स्मृतिः     स्मृतिः = library->स्मृतिः;
    FT_Long       अस्थायी;
    FT_Long       *offsets_आन्तरिकम् = रिक्त;
    FT_RFork_Ref  *ref = रिक्त;


    FT_TRACE3(( "\n" ));
    error = FT_प्रवाहः_Seek( प्रवाहः, (FT_ULong)map_विस्थापनम् );
    यदि ( error )
      प्रत्यागम error;

    यदि ( FT_READ_SHORT( cnt ) )
      प्रत्यागम error;
    cnt++;

    /* `rpos' is a signed 16bit integer offset to resource records; the    */
    /* size of a resource record is 12 bytes.  The map header is 28 bytes, */
    /* and a type list needs 10 bytes or more.  If we assume that the name */
    /* list is empty and we have only a single entry in the type list,     */
    /* there can be at most                                                */
    /*                                                                     */
    /*   (32768 - 28 - 10) / 12 = 2727                                     */
    /*                                                                     */
    /* resources.                                                          */
    /*                                                                     */
    /* A type list starts with a two-byte counter, followed by 10-byte     */
    /* type records.  Assuming that there are no resources, the number of  */
    /* type records can be at most                                         */
    /*                                                                     */
    /*   (32768 - 28 - 2) / 8 = 4079                                       */
    /*                                                                     */
    यदि ( cnt > 4079 )
      प्रत्यागम FT_THROW( Invalid_सारणी );

    प्रति ( i = 0; i < cnt; i++ )
    {
      यदि ( FT_READ_LONG( चिह्नकम्_आन्तरिकम् ) ||
           FT_READ_SHORT( subcnt )      ||
           FT_READ_SHORT( rpos )        )
        प्रत्यागम error;

      FT_TRACE2(( "Resource tags: %c%c%c%c\n",
                  (वर्ण)( 0xFF & ( चिह्नकम्_आन्तरिकम् >> 24 ) ),
                  (वर्ण)( 0xFF & ( चिह्नकम्_आन्तरिकम् >> 16 ) ),
                  (वर्ण)( 0xFF & ( चिह्नकम्_आन्तरिकम् >>  8 ) ),
                  (वर्ण)( 0xFF & ( चिह्नकम्_आन्तरिकम् >>  0 ) ) ));
      FT_TRACE3(( "             : subcount=%d, suboffset=0x%04lx\n",
                  subcnt, rpos ));

      यदि ( चिह्नकम्_आन्तरिकम् == चिह्नकम् )
      {
        *गणनम् = subcnt + 1;
        rpos  += map_विस्थापनम्;

        /* a zero count might be valid in the resource specification, */
        /* however, it is completely useless to us                    */
        यदि ( *गणनम् < 1 || *गणनम् > 2727 )
          प्रत्यागम FT_THROW( Invalid_सारणी );

        error = FT_प्रवाहः_Seek( प्रवाहः, (FT_ULong)rpos );
        यदि ( error )
          प्रत्यागम error;

        यदि ( FT_QNEW_ARRAY( ref, *गणनम् ) )
          प्रत्यागम error;

        प्रति ( j = 0; j < *गणनम्; j++ )
        {
          यदि ( FT_READ_SHORT( ref[j].res_id ) )
            गच्छ Exit;
          यदि ( FT_प्रवाहः_SKIP( 2 ) )  /* resource name offset */
            गच्छ Exit;
          यदि ( FT_READ_LONG( अस्थायी ) ) /* attributes (8bit), offset (24bit) */
            गच्छ Exit;
          यदि ( FT_प्रवाहः_SKIP( 4 ) )  /* mbz */
            गच्छ Exit;

          /*
           * According to Inside Macintosh: More Macintosh Toolbox,
           * "Resource IDs" (1-46), there are some reserved IDs.
           * However, FreeType2 is not a font synthesizer, no need
           * to check the acceptable resource ID or its attributes.
           */
          ref[j].विस्थापनम् = अस्थायी & 0xFFFFFFL;

          FT_TRACE3(( "             [%d]:"
                      " resource_id=0x%04x, offset=0x%08lx\n",
                      j, (FT_UShort)ref[j].res_id, ref[j].विस्थापनम् ));
        }

        यदि ( क्रमय_by_res_id )
        {
          ft_qsort( ref,
                    (size_t)*गणनम्,
                    परिमाण ( FT_RFork_Ref ),
                    ft_raccess_क्रमय_ref_by_id );

          FT_TRACE3(( "             -- sort resources by their ids --\n" ));

          प्रति ( j = 0; j < *गणनम्; j++ )
            FT_TRACE3(( "             [%d]:"
                        " resource_id=0x%04x, offset=0x%08lx\n",
                        j, ref[j].res_id, ref[j].विस्थापनम् ));
        }

        यदि ( FT_QNEW_ARRAY( offsets_आन्तरिकम्, *गणनम् ) )
          गच्छ Exit;

        /* XXX: duplicated reference ID,
         *      gap between reference IDs are acceptable?
         *      further investigation on Apple implementation is needed.
         */
        प्रति ( j = 0; j < *गणनम्; j++ )
          offsets_आन्तरिकम्[j] = rdata_pos + ref[j].विस्थापनम्;

        *offsets = offsets_आन्तरिकम्;
        error    = FT_Err_Ok;

      Exit:
        FT_FREE( ref );
        प्रत्यागम error;
      }
    }

    प्रत्यागम FT_THROW( Cannot_Open_Resource );
  }


#यदि_परिभाषितम् FT_CONFIG_OPTION_GUESSING_EMBEDDED_RFORK

  /*************************************************************************/
  /*************************************************************************/
  /*************************************************************************/
  /****                                                                 ****/
  /****                                                                 ****/
  /****                     Guessing functions                          ****/
  /****                                                                 ****/
  /****            When you add a new guessing function,                ****/
  /****           update FT_RACCESS_N_RULES in ftrfork.h.               ****/
  /****                                                                 ****/
  /*************************************************************************/
  /*************************************************************************/
  /*************************************************************************/

  स्थिर FT_Error
  raccess_guess_apple_double( FT_Library  library,
                              FT_प्रवाहः   प्रवाहः,
                              वर्ण       *आधारः_file_name,
                              वर्ण      **परिणाम_file_name,
                              FT_Long    *परिणाम_विस्थापनम् );

  स्थिर FT_Error
  raccess_guess_apple_single( FT_Library  library,
                              FT_प्रवाहः   प्रवाहः,
                              वर्ण       *आधारः_file_name,
                              वर्ण      **परिणाम_file_name,
                              FT_Long    *परिणाम_विस्थापनम् );

  स्थिर FT_Error
  raccess_guess_darwin_ufs_export( FT_Library  library,
                                   FT_प्रवाहः   प्रवाहः,
                                   वर्ण       *आधारः_file_name,
                                   वर्ण      **परिणाम_file_name,
                                   FT_Long    *परिणाम_विस्थापनम् );

  स्थिर FT_Error
  raccess_guess_darwin_newvfs( FT_Library  library,
                               FT_प्रवाहः   प्रवाहः,
                               वर्ण       *आधारः_file_name,
                               वर्ण      **परिणाम_file_name,
                               FT_Long    *परिणाम_विस्थापनम् );

  स्थिर FT_Error
  raccess_guess_darwin_hfsplus( FT_Library  library,
                                FT_प्रवाहः   प्रवाहः,
                                वर्ण       *आधारः_file_name,
                                वर्ण      **परिणाम_file_name,
                                FT_Long    *परिणाम_विस्थापनम् );

  स्थिर FT_Error
  raccess_guess_vfat( FT_Library  library,
                      FT_प्रवाहः   प्रवाहः,
                      वर्ण       *आधारः_file_name,
                      वर्ण      **परिणाम_file_name,
                      FT_Long    *परिणाम_विस्थापनम् );

  स्थिर FT_Error
  raccess_guess_linux_cap( FT_Library  library,
                           FT_प्रवाहः   प्रवाहः,
                           वर्ण       *आधारः_file_name,
                           वर्ण      **परिणाम_file_name,
                           FT_Long    *परिणाम_विस्थापनम् );

  स्थिर FT_Error
  raccess_guess_linux_double( FT_Library  library,
                              FT_प्रवाहः   प्रवाहः,
                              वर्ण       *आधारः_file_name,
                              वर्ण      **परिणाम_file_name,
                              FT_Long    *परिणाम_विस्थापनम् );

  स्थिर FT_Error
  raccess_guess_linux_netatalk( FT_Library  library,
                                FT_प्रवाहः   प्रवाहः,
                                वर्ण       *आधारः_file_name,
                                वर्ण      **परिणाम_file_name,
                                FT_Long    *परिणाम_विस्थापनम् );


  CONST_FT_RFORK_नियमः_ARRAY_BEGIN( ft_raccess_guess_सारणी,
                                                      ft_raccess_guess_rec )
  CONST_FT_RFORK_नियमः_ARRAY_प्रविष्टिः( apple_double,      apple_double )
  CONST_FT_RFORK_नियमः_ARRAY_प्रविष्टिः( apple_single,      apple_single )
  CONST_FT_RFORK_नियमः_ARRAY_प्रविष्टिः( darwin_ufs_export, darwin_ufs_export )
  CONST_FT_RFORK_नियमः_ARRAY_प्रविष्टिः( darwin_newvfs,     darwin_newvfs )
  CONST_FT_RFORK_नियमः_ARRAY_प्रविष्टिः( darwin_hfsplus,    darwin_hfsplus )
  CONST_FT_RFORK_नियमः_ARRAY_प्रविष्टिः( vfat,              vfat )
  CONST_FT_RFORK_नियमः_ARRAY_प्रविष्टिः( linux_cap,         linux_cap )
  CONST_FT_RFORK_नियमः_ARRAY_प्रविष्टिः( linux_double,      linux_double )
  CONST_FT_RFORK_नियमः_ARRAY_प्रविष्टिः( linux_netatalk,    linux_netatalk )
  CONST_FT_RFORK_नियमः_ARRAY_अन्तः


  /*************************************************************************/
  /****                                                                 ****/
  /****                       Helper functions                          ****/
  /****                                                                 ****/
  /*************************************************************************/

  स्थिर FT_Error
  raccess_guess_apple_generic( FT_Library  library,
                               FT_प्रवाहः   प्रवाहः,
                               वर्ण       *आधारः_file_name,
                               FT_Int32    magic,
                               FT_Long    *परिणाम_विस्थापनम् );

  स्थिर FT_Error
  raccess_guess_linux_double_from_file_name( FT_Library  library,
                                             वर्ण*       file_name,
                                             FT_Long    *परिणाम_विस्थापनम् );

  स्थिर वर्ण *
  raccess_make_file_name( FT_स्मृतिः    स्मृतिः,
                          ध्रुव वर्ण  *original_name,
                          ध्रुव वर्ण  *insertion );

  FT_आधारः_DEF( शून्य )
  FT_Raccess_Guess( FT_Library  library,
                    FT_प्रवाहः   प्रवाहः,
                    वर्ण*       आधारः_name,
                    वर्ण      **new_नामानि,
                    FT_Long    *offsets,
                    FT_Error   *errors )
  {
    FT_Int  i;


    प्रति ( i = 0; i < FT_RACCESS_N_नियमाः; i++ )
    {
      new_नामानि[i] = रिक्त;
      यदि ( रिक्त != प्रवाहः )
        errors[i] = FT_प्रवाहः_Seek( प्रवाहः, 0 );
      अन्यथा
        errors[i] = FT_Err_Ok;

      यदि ( errors[i] )
        अनुवर्त;

      errors[i] = ft_raccess_guess_सारणी[i].func( library,
                                                  प्रवाहः, आधारः_name,
                                                  &(new_नामानि[i]),
                                                  &(offsets[i]) );
    }

    प्रत्यागम;
  }


#यदि defined( FT_CONFIG_OPTION_MAC_FONTS ) && !defined( FT_MACINTOSH )
  स्थिर FT_RFork_नियमः
  raccess_get_नियमः_type_from_नियमः_सूचकांक( FT_Library  library,
                                         FT_UInt     नियमः_सूचकांक )
  {
    FT_अप्रयुक्तम्( library );

    यदि ( नियमः_सूचकांक >= FT_RACCESS_N_नियमाः )
      प्रत्यागम FT_RFork_नियमः_invalid;

    प्रत्यागम ft_raccess_guess_सारणी[नियमः_सूचकांक].type;
  }


  /*
   * For this function, refer ftbase.h.
   */
  FT_LOCAL_DEF( FT_Bool )
  ft_raccess_नियमः_by_darwin_vfs( FT_Library  library,
                                 FT_UInt     नियमः_सूचकांक )
  {
    विकल्प( raccess_get_नियमः_type_from_नियमः_सूचकांक( library, नियमः_सूचकांक ) )
    {
      पक्ष FT_RFork_नियमः_darwin_newvfs:
      पक्ष FT_RFork_नियमः_darwin_hfsplus:
        प्रत्यागम TRUE;

      स्वभाव:
        प्रत्यागम FALSE;
    }
  }
#यदि_अन्तः


  स्थिर FT_Error
  raccess_guess_apple_double( FT_Library  library,
                              FT_प्रवाहः   प्रवाहः,
                              वर्ण       *आधारः_file_name,
                              वर्ण      **परिणाम_file_name,
                              FT_Long    *परिणाम_विस्थापनम् )
  {
    FT_Int32  magic = ( 0x00 << 24 ) |
                      ( 0x05 << 16 ) |
                      ( 0x16 <<  8 ) |
                        0x07;


    *परिणाम_file_name = रिक्त;
    यदि ( रिक्त == प्रवाहः )
      प्रत्यागम FT_THROW( Cannot_Open_प्रवाहः );

    प्रत्यागम raccess_guess_apple_generic( library, प्रवाहः, आधारः_file_name,
                                        magic, परिणाम_विस्थापनम् );
  }


  स्थिर FT_Error
  raccess_guess_apple_single( FT_Library  library,
                              FT_प्रवाहः   प्रवाहः,
                              वर्ण       *आधारः_file_name,
                              वर्ण      **परिणाम_file_name,
                              FT_Long    *परिणाम_विस्थापनम् )
  {
    FT_Int32  magic = ( 0x00 << 24 ) |
                      ( 0x05 << 16 ) |
                      ( 0x16 <<  8 ) |
                        0x00;


    *परिणाम_file_name = रिक्त;
    यदि ( रिक्त == प्रवाहः )
      प्रत्यागम FT_THROW( Cannot_Open_प्रवाहः );

    प्रत्यागम raccess_guess_apple_generic( library, प्रवाहः, आधारः_file_name,
                                        magic, परिणाम_विस्थापनम् );
  }


  स्थिर FT_Error
  raccess_guess_darwin_ufs_export( FT_Library  library,
                                   FT_प्रवाहः   प्रवाहः,
                                   वर्ण       *आधारः_file_name,
                                   वर्ण      **परिणाम_file_name,
                                   FT_Long    *परिणाम_विस्थापनम् )
  {
    वर्ण*      newpath;
    FT_Error   error;
    FT_स्मृतिः  स्मृतिः;

    FT_अप्रयुक्तम्( प्रवाहः );


    स्मृतिः  = library->स्मृतिः;
    newpath = raccess_make_file_name( स्मृतिः, आधारः_file_name, "._" );
    यदि ( !newpath )
      प्रत्यागम FT_THROW( बहिर्गमनम्_सम्बन्धि_स्मृतिः );

    error = raccess_guess_linux_double_from_file_name( library, newpath,
                                                       परिणाम_विस्थापनम् );
    यदि ( !error )
      *परिणाम_file_name = newpath;
    अन्यथा
      FT_FREE( newpath );

    प्रत्यागम error;
  }


  स्थिर FT_Error
  raccess_guess_darwin_hfsplus( FT_Library  library,
                                FT_प्रवाहः   प्रवाहः,
                                वर्ण       *आधारः_file_name,
                                वर्ण      **परिणाम_file_name,
                                FT_Long    *परिणाम_विस्थापनम् )
  {
    /*
      Only meaningful on systems with hfs+ drivers (or Macs).
     */
    FT_Error   error;
    वर्ण*      newpath = रिक्त;
    FT_स्मृतिः  स्मृतिः;
    FT_Long    आधारः_file_len = (FT_Long)ft_strlen( आधारः_file_name );

    FT_अप्रयुक्तम्( प्रवाहः );


    स्मृतिः = library->स्मृतिः;

    यदि ( आधारः_file_len + 6 > FT_INT_MAX )
      प्रत्यागम FT_THROW( Array_Too_Large );

    यदि ( FT_QALLOC( newpath, आधारः_file_len + 6 ) )
      प्रत्यागम error;

    FT_MEM_COPY( newpath, आधारः_file_name, आधारः_file_len );
    FT_MEM_COPY( newpath + आधारः_file_len, "/rsrc", 6 );

    *परिणाम_file_name = newpath;
    *परिणाम_विस्थापनम्    = 0;

    प्रत्यागम FT_Err_Ok;
  }


  स्थिर FT_Error
  raccess_guess_darwin_newvfs( FT_Library  library,
                               FT_प्रवाहः   प्रवाहः,
                               वर्ण       *आधारः_file_name,
                               वर्ण      **परिणाम_file_name,
                               FT_Long    *परिणाम_विस्थापनम् )
  {
    /*
      Only meaningful on systems with Mac OS X (> 10.1).
     */
    FT_Error   error;
    वर्ण*      newpath = रिक्त;
    FT_स्मृतिः  स्मृतिः;
    FT_Long    आधारः_file_len = (FT_Long)ft_strlen( आधारः_file_name );

    FT_अप्रयुक्तम्( प्रवाहः );


    स्मृतिः = library->स्मृतिः;

    यदि ( आधारः_file_len + 18 > FT_INT_MAX )
      प्रत्यागम FT_THROW( Array_Too_Large );

    यदि ( FT_QALLOC( newpath, आधारः_file_len + 18 ) )
      प्रत्यागम error;

    FT_MEM_COPY( newpath, आधारः_file_name, आधारः_file_len );
    FT_MEM_COPY( newpath + आधारः_file_len, "/..namedfork/rsrc", 18 );

    *परिणाम_file_name = newpath;
    *परिणाम_विस्थापनम्    = 0;

    प्रत्यागम FT_Err_Ok;
  }


  स्थिर FT_Error
  raccess_guess_vfat( FT_Library  library,
                      FT_प्रवाहः   प्रवाहः,
                      वर्ण       *आधारः_file_name,
                      वर्ण      **परिणाम_file_name,
                      FT_Long    *परिणाम_विस्थापनम् )
  {
    वर्ण*      newpath;
    FT_स्मृतिः  स्मृतिः;

    FT_अप्रयुक्तम्( प्रवाहः );


    स्मृतिः = library->स्मृतिः;

    newpath = raccess_make_file_name( स्मृतिः, आधारः_file_name,
                                      "resource.frk/" );
    यदि ( !newpath )
      प्रत्यागम FT_THROW( बहिर्गमनम्_सम्बन्धि_स्मृतिः );

    *परिणाम_file_name = newpath;
    *परिणाम_विस्थापनम्    = 0;

    प्रत्यागम FT_Err_Ok;
  }


  स्थिर FT_Error
  raccess_guess_linux_cap( FT_Library  library,
                           FT_प्रवाहः   प्रवाहः,
                           वर्ण       *आधारः_file_name,
                           वर्ण      **परिणाम_file_name,
                           FT_Long    *परिणाम_विस्थापनम् )
  {
    वर्ण*      newpath;
    FT_स्मृतिः  स्मृतिः;

    FT_अप्रयुक्तम्( प्रवाहः );


    स्मृतिः = library->स्मृतिः;

    newpath = raccess_make_file_name( स्मृतिः, आधारः_file_name, ".resource/" );
    यदि ( !newpath )
      प्रत्यागम FT_THROW( बहिर्गमनम्_सम्बन्धि_स्मृतिः );

    *परिणाम_file_name = newpath;
    *परिणाम_विस्थापनम्    = 0;

    प्रत्यागम FT_Err_Ok;
  }


  स्थिर FT_Error
  raccess_guess_linux_double( FT_Library  library,
                              FT_प्रवाहः   प्रवाहः,
                              वर्ण       *आधारः_file_name,
                              वर्ण      **परिणाम_file_name,
                              FT_Long    *परिणाम_विस्थापनम् )
  {
    वर्ण*      newpath;
    FT_Error   error;
    FT_स्मृतिः  स्मृतिः;

    FT_अप्रयुक्तम्( प्रवाहः );


    स्मृतिः = library->स्मृतिः;

    newpath = raccess_make_file_name( स्मृतिः, आधारः_file_name, "%" );
    यदि ( !newpath )
      प्रत्यागम FT_THROW( बहिर्गमनम्_सम्बन्धि_स्मृतिः );

    error = raccess_guess_linux_double_from_file_name( library, newpath,
                                                       परिणाम_विस्थापनम् );
    यदि ( !error )
      *परिणाम_file_name = newpath;
    अन्यथा
      FT_FREE( newpath );

    प्रत्यागम error;
  }


  स्थिर FT_Error
  raccess_guess_linux_netatalk( FT_Library  library,
                                FT_प्रवाहः   प्रवाहः,
                                वर्ण       *आधारः_file_name,
                                वर्ण      **परिणाम_file_name,
                                FT_Long    *परिणाम_विस्थापनम् )
  {
    वर्ण*      newpath;
    FT_Error   error;
    FT_स्मृतिः  स्मृतिः;

    FT_अप्रयुक्तम्( प्रवाहः );


    स्मृतिः = library->स्मृतिः;

    newpath = raccess_make_file_name( स्मृतिः, आधारः_file_name,
                                      ".AppleDouble/" );
    यदि ( !newpath )
      प्रत्यागम FT_THROW( बहिर्गमनम्_सम्बन्धि_स्मृतिः );

    error = raccess_guess_linux_double_from_file_name( library, newpath,
                                                       परिणाम_विस्थापनम् );
    यदि ( !error )
      *परिणाम_file_name = newpath;
    अन्यथा
      FT_FREE( newpath );

    प्रत्यागम error;
  }


  स्थिर FT_Error
  raccess_guess_apple_generic( FT_Library  library,
                               FT_प्रवाहः   प्रवाहः,
                               वर्ण       *आधारः_file_name,
                               FT_Int32    magic,
                               FT_Long    *परिणाम_विस्थापनम् )
  {
    FT_Int32   magic_from_प्रवाहः;
    FT_Error   error;
    FT_Int32   संस्करणम्_सङ्ख्या = 0;
    FT_UShort  n_सम्बन्धि_entries;

    पूर्णाङ्क        i;
    FT_Int32   प्रविष्टिः_id, प्रविष्टिः_विस्थापनम्, प्रविष्टिः_दीर्घता = 0;

    ध्रुव FT_Int32  resource_fork_प्रविष्टिः_id = 0x2;

    FT_अप्रयुक्तम्( library );
    FT_अप्रयुक्तम्( आधारः_file_name );
    FT_अप्रयुक्तम्( संस्करणम्_सङ्ख्या );
    FT_अप्रयुक्तम्( प्रविष्टिः_दीर्घता   );


    यदि ( FT_READ_LONG( magic_from_प्रवाहः ) )
      प्रत्यागम error;
    यदि ( magic_from_प्रवाहः != magic )
      प्रत्यागम FT_THROW( Unknown_File_Format );

    यदि ( FT_READ_LONG( संस्करणम्_सङ्ख्या ) )
      प्रत्यागम error;

    /* filler */
    error = FT_प्रवाहः_Skip( प्रवाहः, 16 );
    यदि ( error )
      प्रत्यागम error;

    यदि ( FT_READ_USHORT( n_सम्बन्धि_entries ) )
      प्रत्यागम error;
    यदि ( n_सम्बन्धि_entries == 0 )
      प्रत्यागम FT_THROW( Unknown_File_Format );

    प्रति ( i = 0; i < n_सम्बन्धि_entries; i++ )
    {
      यदि ( FT_READ_LONG( प्रविष्टिः_id ) )
        प्रत्यागम error;
      यदि ( प्रविष्टिः_id == resource_fork_प्रविष्टिः_id )
      {
        यदि ( FT_READ_LONG( प्रविष्टिः_विस्थापनम् ) ||
             FT_READ_LONG( प्रविष्टिः_दीर्घता ) )
          अनुवर्त;
        *परिणाम_विस्थापनम् = प्रविष्टिः_विस्थापनम्;

        प्रत्यागम FT_Err_Ok;
      }
      अन्यथा
      {
        error = FT_प्रवाहः_Skip( प्रवाहः, 4 + 4 );    /* offset + length */
        यदि ( error )
          प्रत्यागम error;
      }
    }

    प्रत्यागम FT_THROW( Unknown_File_Format );
  }


  स्थिर FT_Error
  raccess_guess_linux_double_from_file_name( FT_Library  library,
                                             वर्ण       *file_name,
                                             FT_Long    *परिणाम_विस्थापनम् )
  {
    FT_Open_तर्काः  args2;
    FT_प्रवाहः     stream2;
    वर्ण*         nouse = रिक्त;
    FT_Error      error;


    args2.flags    = FT_OPEN_PATHNAME;
    args2.pathname = file_name;
    error = FT_प्रवाहः_New( library, &args2, &stream2 );
    यदि ( error )
      प्रत्यागम error;

    error = raccess_guess_apple_double( library, stream2, file_name,
                                        &nouse, परिणाम_विस्थापनम् );

    FT_प्रवाहः_Free( stream2, 0 );

    प्रत्यागम error;
  }


  स्थिर वर्ण*
  raccess_make_file_name( FT_स्मृतिः    स्मृतिः,
                          ध्रुव वर्ण  *original_name,
                          ध्रुव वर्ण  *insertion )
  {
    वर्ण*        new_name = रिक्त;
    ध्रुव वर्ण*  tmp;
    ध्रुव वर्ण*  slash;
    size_t       new_दीर्घता;
    FT_Error     error;


    new_दीर्घता = ft_strlen( original_name ) + ft_strlen( insertion );
    यदि ( FT_QALLOC( new_name, new_दीर्घता + 1 ) )
      प्रत्यागम रिक्त;

    tmp = ft_strrchr( original_name, '/' );
    यदि ( tmp )
    {
      ft_strncpy( new_name,
                  original_name,
                  (size_t)( tmp - original_name + 1 ) );
      new_name[tmp - original_name + 1] = '\0';
      slash = tmp + 1;
    }
    अन्यथा
    {
      slash       = original_name;
      new_name[0] = '\0';
    }

    ft_strcat( new_name, insertion );
    ft_strcat( new_name, slash );

    प्रत्यागम new_name;
  }


#अन्यथा   /* !FT_CONFIG_OPTION_GUESSING_EMBEDDED_RFORK */


  /**************************************************************************
   *                 Dummy function; just sets errors
   */

  FT_आधारः_DEF( शून्य )
  FT_Raccess_Guess( FT_Library  library,
                    FT_प्रवाहः   प्रवाहः,
                    वर्ण       *आधारः_name,
                    वर्ण      **new_नामानि,
                    FT_Long    *offsets,
                    FT_Error   *errors )
  {
    FT_Int  i;

    FT_अप्रयुक्तम्( library );
    FT_अप्रयुक्तम्( प्रवाहः );
    FT_अप्रयुक्तम्( आधारः_name );


    प्रति ( i = 0; i < FT_RACCESS_N_नियमाः; i++ )
    {
      new_नामानि[i] = रिक्त;
      offsets[i]   = 0;
      errors[i]    = FT_ERR( Unimplemented_Feature );
    }
  }


#यदि_अन्तः  /* !FT_CONFIG_OPTION_GUESSING_EMBEDDED_RFORK */


/*अन्तः*/
