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


#అన్తర్భావ <freetype/internal/ftdebug.h>
#అన్తర్భావ <freetype/internal/ftstream.h>
#అన్తర్భావ <freetype/internal/ftrfork.h>

#అన్తర్భావ "ftbase.h"

#నిర్వచనం_రద్దు  FT_COMPONENT
#నిర్వచించు FT_COMPONENT  raccess


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

  FT_పునాది_DEF( FT_Error )
  FT_Raccess_Get_శీర్షిక_Info( FT_Library  library,
                             FT_ప్రవాహం   ప్రవాహం,
                             FT_Long     rfork_స్థానభ్రంశం,
                             FT_Long    *map_స్థానభ్రంశం,
                             FT_Long    *rdata_pos )
  {
    FT_Error       error;
    గుర్తులేని అక్షరం  తల[16], head2[16];
    FT_Long        map_pos, map_len, rdata_len;
    పూర్ణాంకం            allzeros, allmatch, i;
    FT_Long        type_list;

    FT_వాడని( library );


    error = FT_ప్రవాహం_Seek( ప్రవాహం, (FT_ULong)rfork_స్థానభ్రంశం );
    ఒకవేళ ( error )
      తిరిగి error;

    error = FT_ప్రవాహం_Read( ప్రవాహం, (FT_Byte*)తల, 16 );
    ఒకవేళ ( error )
      తిరిగి error;

    /* ensure positive values */
    ఒకవేళ ( తల[0]  >= 0x80 ||
         తల[4]  >= 0x80 ||
         తల[8]  >= 0x80 ||
         తల[12] >= 0x80 )
      తిరిగి FT_THROW( Unknown_File_Format );

    *rdata_pos = ( తల[ 0] << 24 ) |
                 ( తల[ 1] << 16 ) |
                 ( తల[ 2] <<  8 ) |
                   తల[ 3];
    map_pos    = ( తల[ 4] << 24 ) |
                 ( తల[ 5] << 16 ) |
                 ( తల[ 6] <<  8 ) |
                   తల[ 7];
    rdata_len  = ( తల[ 8] << 24 ) |
                 ( తల[ 9] << 16 ) |
                 ( తల[10] <<  8 ) |
                   తల[11];
    map_len    = ( తల[12] << 24 ) |
                 ( తల[13] << 16 ) |
                 ( తల[14] <<  8 ) |
                   తల[15];

    /* the map must not be empty */
    ఒకవేళ ( !map_pos )
      తిరిగి FT_THROW( Unknown_File_Format );

    /* check whether rdata and map overlap */
    ఒకవేళ ( *rdata_pos < map_pos )
    {
      ఒకవేళ ( *rdata_pos > map_pos - rdata_len )
        తిరిగి FT_THROW( Unknown_File_Format );
    }
    లేకపోతే
    {
      ఒకవేళ ( map_pos > *rdata_pos - map_len )
        తిరిగి FT_THROW( Unknown_File_Format );
    }

    /* check whether end of rdata or map exceeds stream size */
    ఒకవేళ ( FT_LONG_MAX - rdata_len < *rdata_pos                               ||
         FT_LONG_MAX - map_len < map_pos                                    ||

         FT_LONG_MAX - ( *rdata_pos + rdata_len ) < rfork_స్థానభ్రంశం            ||
         FT_LONG_MAX - ( map_pos + map_len ) < rfork_స్థానభ్రంశం                 ||

         (FT_ULong)( rfork_స్థానభ్రంశం + *rdata_pos + rdata_len ) > ప్రవాహం->కొలత ||
         (FT_ULong)( rfork_స్థానభ్రంశం + map_pos + map_len ) > ప్రవాహం->కొలత      )
      తిరిగి FT_THROW( Unknown_File_Format );

    *rdata_pos += rfork_స్థానభ్రంశం;
    map_pos    += rfork_స్థానభ్రంశం;

    error = FT_ప్రవాహం_Seek( ప్రవాహం, (FT_ULong)map_pos );
    ఒకవేళ ( error )
      తిరిగి error;

    head2[15] = (FT_Byte)( తల[15] + 1 );       /* make it be different */

    error = FT_ప్రవాహం_Read( ప్రవాహం, (FT_Byte*)head2, 16 );
    ఒకవేళ ( error )
      తిరిగి error;

    allzeros = 1;
    allmatch = 1;
    కొరకు ( i = 0; i < 16; i++ )
    {
      ఒకవేళ ( head2[i] != 0 )
        allzeros = 0;
      ఒకవేళ ( head2[i] != తల[i] )
        allmatch = 0;
    }
    ఒకవేళ ( !allzeros && !allmatch )
      తిరిగి FT_THROW( Unknown_File_Format );

    /* If we have reached this point then it is probably a mac resource */
    /* file.  Now, does it contain any interesting resources?           */

    (శూన్య)FT_ప్రవాహం_SKIP( 4        /* skip handle to next resource map */
                          + 2      /* skip file resource number */
                          + 2 );   /* skip attributes */

    ఒకవేళ ( FT_READ_SHORT( type_list ) )
      తిరిగి error;
    ఒకవేళ ( type_list < 0 )
      తిరిగి FT_THROW( Unknown_File_Format );

    error = FT_ప్రవాహం_Seek( ప్రవాహం, (FT_ULong)( map_pos + type_list ) );
    ఒకవేళ ( error )
      తిరిగి error;

    *map_స్థానభ్రంశం = map_pos + type_list;
    తిరిగి FT_Err_Ok;
  }


  FT_COMPARE_DEF( పూర్ణాంకం )
  ft_raccess_క్రమీకరించు_ref_by_id( స్థిరరాశి శూన్య*  a,
                             స్థిరరాశి శూన్య*  b )
  {
    తిరిగి  ( (FT_RFork_Ref*)a )->res_id - ( (FT_RFork_Ref*)b )->res_id;
  }


  FT_పునాది_DEF( FT_Error )
  FT_Raccess_Get_దత్తాంశం_Offsets( FT_Library  library,
                              FT_ప్రవాహం   ప్రవాహం,
                              FT_Long     map_స్థానభ్రంశం,
                              FT_Long     rdata_pos,
                              FT_Long     ట్యాగ్,
                              FT_Bool     క్రమీకరించు_by_res_id,
                              FT_Long   **offsets,
                              FT_Long    *లెక్క )
  {
    FT_Error      error;
    పూర్ణాంకం           i, j, cnt, subcnt;
    FT_Long       ట్యాగ్_అంతర్గత, rpos;
    FT_జ్ఞాపకం     జ్ఞాపకం = library->జ్ఞాపకం;
    FT_Long       తాత్కాలిక;
    FT_Long       *offsets_అంతర్గత = ఖాళీ;
    FT_RFork_Ref  *ref = ఖాళీ;


    FT_TRACE3(( "\n" ));
    error = FT_ప్రవాహం_Seek( ప్రవాహం, (FT_ULong)map_స్థానభ్రంశం );
    ఒకవేళ ( error )
      తిరిగి error;

    ఒకవేళ ( FT_READ_SHORT( cnt ) )
      తిరిగి error;
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
    ఒకవేళ ( cnt > 4079 )
      తిరిగి FT_THROW( Invalid_పట్టిక );

    కొరకు ( i = 0; i < cnt; i++ )
    {
      ఒకవేళ ( FT_READ_LONG( ట్యాగ్_అంతర్గత ) ||
           FT_READ_SHORT( subcnt )      ||
           FT_READ_SHORT( rpos )        )
        తిరిగి error;

      FT_TRACE2(( "Resource tags: %c%c%c%c\n",
                  (అక్షరం)( 0xFF & ( ట్యాగ్_అంతర్గత >> 24 ) ),
                  (అక్షరం)( 0xFF & ( ట్యాగ్_అంతర్గత >> 16 ) ),
                  (అక్షరం)( 0xFF & ( ట్యాగ్_అంతర్గత >>  8 ) ),
                  (అక్షరం)( 0xFF & ( ట్యాగ్_అంతర్గత >>  0 ) ) ));
      FT_TRACE3(( "             : subcount=%d, suboffset=0x%04lx\n",
                  subcnt, rpos ));

      ఒకవేళ ( ట్యాగ్_అంతర్గత == ట్యాగ్ )
      {
        *లెక్క = subcnt + 1;
        rpos  += map_స్థానభ్రంశం;

        /* a zero count might be valid in the resource specification, */
        /* however, it is completely useless to us                    */
        ఒకవేళ ( *లెక్క < 1 || *లెక్క > 2727 )
          తిరిగి FT_THROW( Invalid_పట్టిక );

        error = FT_ప్రవాహం_Seek( ప్రవాహం, (FT_ULong)rpos );
        ఒకవేళ ( error )
          తిరిగి error;

        ఒకవేళ ( FT_QNEW_ARRAY( ref, *లెక్క ) )
          తిరిగి error;

        కొరకు ( j = 0; j < *లెక్క; j++ )
        {
          ఒకవేళ ( FT_READ_SHORT( ref[j].res_id ) )
            వెళ్ళు Exit;
          ఒకవేళ ( FT_ప్రవాహం_SKIP( 2 ) )  /* resource name offset */
            వెళ్ళు Exit;
          ఒకవేళ ( FT_READ_LONG( తాత్కాలిక ) ) /* attributes (8bit), offset (24bit) */
            వెళ్ళు Exit;
          ఒకవేళ ( FT_ప్రవాహం_SKIP( 4 ) )  /* mbz */
            వెళ్ళు Exit;

          /*
           * According to Inside Macintosh: More Macintosh Toolbox,
           * "Resource IDs" (1-46), there are some reserved IDs.
           * However, FreeType2 is not a font synthesizer, no need
           * to check the acceptable resource ID or its attributes.
           */
          ref[j].స్థానభ్రంశం = తాత్కాలిక & 0xFFFFFFL;

          FT_TRACE3(( "             [%d]:"
                      " resource_id=0x%04x, offset=0x%08lx\n",
                      j, (FT_UShort)ref[j].res_id, ref[j].స్థానభ్రంశం ));
        }

        ఒకవేళ ( క్రమీకరించు_by_res_id )
        {
          ft_qsort( ref,
                    (size_t)*లెక్క,
                    పరిమాణం ( FT_RFork_Ref ),
                    ft_raccess_క్రమీకరించు_ref_by_id );

          FT_TRACE3(( "             -- sort resources by their ids --\n" ));

          కొరకు ( j = 0; j < *లెక్క; j++ )
            FT_TRACE3(( "             [%d]:"
                        " resource_id=0x%04x, offset=0x%08lx\n",
                        j, ref[j].res_id, ref[j].స్థానభ్రంశం ));
        }

        ఒకవేళ ( FT_QNEW_ARRAY( offsets_అంతర్గత, *లెక్క ) )
          వెళ్ళు Exit;

        /* XXX: duplicated reference ID,
         *      gap between reference IDs are acceptable?
         *      further investigation on Apple implementation is needed.
         */
        కొరకు ( j = 0; j < *లెక్క; j++ )
          offsets_అంతర్గత[j] = rdata_pos + ref[j].స్థానభ్రంశం;

        *offsets = offsets_అంతర్గత;
        error    = FT_Err_Ok;

      Exit:
        FT_FREE( ref );
        తిరిగి error;
      }
    }

    తిరిగి FT_THROW( Cannot_Open_Resource );
  }


#నిర్వచితమైతే FT_CONFIG_OPTION_GUESSING_EMBEDDED_RFORK

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

  స్థిర FT_Error
  raccess_guess_apple_double( FT_Library  library,
                              FT_ప్రవాహం   ప్రవాహం,
                              అక్షరం       *పునాది_file_name,
                              అక్షరం      **ఫలితం_file_name,
                              FT_Long    *ఫలితం_స్థానభ్రంశం );

  స్థిర FT_Error
  raccess_guess_apple_single( FT_Library  library,
                              FT_ప్రవాహం   ప్రవాహం,
                              అక్షరం       *పునాది_file_name,
                              అక్షరం      **ఫలితం_file_name,
                              FT_Long    *ఫలితం_స్థానభ్రంశం );

  స్థిర FT_Error
  raccess_guess_darwin_ufs_export( FT_Library  library,
                                   FT_ప్రవాహం   ప్రవాహం,
                                   అక్షరం       *పునాది_file_name,
                                   అక్షరం      **ఫలితం_file_name,
                                   FT_Long    *ఫలితం_స్థానభ్రంశం );

  స్థిర FT_Error
  raccess_guess_darwin_newvfs( FT_Library  library,
                               FT_ప్రవాహం   ప్రవాహం,
                               అక్షరం       *పునాది_file_name,
                               అక్షరం      **ఫలితం_file_name,
                               FT_Long    *ఫలితం_స్థానభ్రంశం );

  స్థిర FT_Error
  raccess_guess_darwin_hfsplus( FT_Library  library,
                                FT_ప్రవాహం   ప్రవాహం,
                                అక్షరం       *పునాది_file_name,
                                అక్షరం      **ఫలితం_file_name,
                                FT_Long    *ఫలితం_స్థానభ్రంశం );

  స్థిర FT_Error
  raccess_guess_vfat( FT_Library  library,
                      FT_ప్రవాహం   ప్రవాహం,
                      అక్షరం       *పునాది_file_name,
                      అక్షరం      **ఫలితం_file_name,
                      FT_Long    *ఫలితం_స్థానభ్రంశం );

  స్థిర FT_Error
  raccess_guess_linux_cap( FT_Library  library,
                           FT_ప్రవాహం   ప్రవాహం,
                           అక్షరం       *పునాది_file_name,
                           అక్షరం      **ఫలితం_file_name,
                           FT_Long    *ఫలితం_స్థానభ్రంశం );

  స్థిర FT_Error
  raccess_guess_linux_double( FT_Library  library,
                              FT_ప్రవాహం   ప్రవాహం,
                              అక్షరం       *పునాది_file_name,
                              అక్షరం      **ఫలితం_file_name,
                              FT_Long    *ఫలితం_స్థానభ్రంశం );

  స్థిర FT_Error
  raccess_guess_linux_netatalk( FT_Library  library,
                                FT_ప్రవాహం   ప్రవాహం,
                                అక్షరం       *పునాది_file_name,
                                అక్షరం      **ఫలితం_file_name,
                                FT_Long    *ఫలితం_స్థానభ్రంశం );


  CONST_FT_RFORK_నియమం_ARRAY_BEGIN( ft_raccess_guess_పట్టిక,
                                                      ft_raccess_guess_rec )
  CONST_FT_RFORK_నియమం_ARRAY_నమోదు( apple_double,      apple_double )
  CONST_FT_RFORK_నియమం_ARRAY_నమోదు( apple_single,      apple_single )
  CONST_FT_RFORK_నియమం_ARRAY_నమోదు( darwin_ufs_export, darwin_ufs_export )
  CONST_FT_RFORK_నియమం_ARRAY_నమోదు( darwin_newvfs,     darwin_newvfs )
  CONST_FT_RFORK_నియమం_ARRAY_నమోదు( darwin_hfsplus,    darwin_hfsplus )
  CONST_FT_RFORK_నియమం_ARRAY_నమోదు( vfat,              vfat )
  CONST_FT_RFORK_నియమం_ARRAY_నమోదు( linux_cap,         linux_cap )
  CONST_FT_RFORK_నియమం_ARRAY_నమోదు( linux_double,      linux_double )
  CONST_FT_RFORK_నియమం_ARRAY_నమోదు( linux_netatalk,    linux_netatalk )
  CONST_FT_RFORK_నియమం_ARRAY_చివర


  /*************************************************************************/
  /****                                                                 ****/
  /****                       Helper functions                          ****/
  /****                                                                 ****/
  /*************************************************************************/

  స్థిర FT_Error
  raccess_guess_apple_generic( FT_Library  library,
                               FT_ప్రవాహం   ప్రవాహం,
                               అక్షరం       *పునాది_file_name,
                               FT_Int32    magic,
                               FT_Long    *ఫలితం_స్థానభ్రంశం );

  స్థిర FT_Error
  raccess_guess_linux_double_from_file_name( FT_Library  library,
                                             అక్షరం*       file_name,
                                             FT_Long    *ఫలితం_స్థానభ్రంశం );

  స్థిర అక్షరం *
  raccess_make_file_name( FT_జ్ఞాపకం    జ్ఞాపకం,
                          స్థిరరాశి అక్షరం  *original_name,
                          స్థిరరాశి అక్షరం  *insertion );

  FT_పునాది_DEF( శూన్య )
  FT_Raccess_Guess( FT_Library  library,
                    FT_ప్రవాహం   ప్రవాహం,
                    అక్షరం*       పునాది_name,
                    అక్షరం      **new_పేర్లు,
                    FT_Long    *offsets,
                    FT_Error   *errors )
  {
    FT_Int  i;


    కొరకు ( i = 0; i < FT_RACCESS_N_నియమాలు; i++ )
    {
      new_పేర్లు[i] = ఖాళీ;
      ఒకవేళ ( ఖాళీ != ప్రవాహం )
        errors[i] = FT_ప్రవాహం_Seek( ప్రవాహం, 0 );
      లేకపోతే
        errors[i] = FT_Err_Ok;

      ఒకవేళ ( errors[i] )
        కొనసాగు;

      errors[i] = ft_raccess_guess_పట్టిక[i].func( library,
                                                  ప్రవాహం, పునాది_name,
                                                  &(new_పేర్లు[i]),
                                                  &(offsets[i]) );
    }

    తిరిగి;
  }


#ఒకవేళ defined( FT_CONFIG_OPTION_MAC_FONTS ) && !defined( FT_MACINTOSH )
  స్థిర FT_RFork_నియమం
  raccess_get_నియమం_type_from_నియమం_సూచీ( FT_Library  library,
                                         FT_UInt     నియమం_సూచీ )
  {
    FT_వాడని( library );

    ఒకవేళ ( నియమం_సూచీ >= FT_RACCESS_N_నియమాలు )
      తిరిగి FT_RFork_నియమం_invalid;

    తిరిగి ft_raccess_guess_పట్టిక[నియమం_సూచీ].type;
  }


  /*
   * For this function, refer ftbase.h.
   */
  FT_LOCAL_DEF( FT_Bool )
  ft_raccess_నియమం_by_darwin_vfs( FT_Library  library,
                                 FT_UInt     నియమం_సూచీ )
  {
    ఎంపిక( raccess_get_నియమం_type_from_నియమం_సూచీ( library, నియమం_సూచీ ) )
    {
      స్థితి FT_RFork_నియమం_darwin_newvfs:
      స్థితి FT_RFork_నియమం_darwin_hfsplus:
        తిరిగి TRUE;

      అప్రమేయ:
        తిరిగి FALSE;
    }
  }
#అయితే_ముగింపు


  స్థిర FT_Error
  raccess_guess_apple_double( FT_Library  library,
                              FT_ప్రవాహం   ప్రవాహం,
                              అక్షరం       *పునాది_file_name,
                              అక్షరం      **ఫలితం_file_name,
                              FT_Long    *ఫలితం_స్థానభ్రంశం )
  {
    FT_Int32  magic = ( 0x00 << 24 ) |
                      ( 0x05 << 16 ) |
                      ( 0x16 <<  8 ) |
                        0x07;


    *ఫలితం_file_name = ఖాళీ;
    ఒకవేళ ( ఖాళీ == ప్రవాహం )
      తిరిగి FT_THROW( Cannot_Open_ప్రవాహం );

    తిరిగి raccess_guess_apple_generic( library, ప్రవాహం, పునాది_file_name,
                                        magic, ఫలితం_స్థానభ్రంశం );
  }


  స్థిర FT_Error
  raccess_guess_apple_single( FT_Library  library,
                              FT_ప్రవాహం   ప్రవాహం,
                              అక్షరం       *పునాది_file_name,
                              అక్షరం      **ఫలితం_file_name,
                              FT_Long    *ఫలితం_స్థానభ్రంశం )
  {
    FT_Int32  magic = ( 0x00 << 24 ) |
                      ( 0x05 << 16 ) |
                      ( 0x16 <<  8 ) |
                        0x00;


    *ఫలితం_file_name = ఖాళీ;
    ఒకవేళ ( ఖాళీ == ప్రవాహం )
      తిరిగి FT_THROW( Cannot_Open_ప్రవాహం );

    తిరిగి raccess_guess_apple_generic( library, ప్రవాహం, పునాది_file_name,
                                        magic, ఫలితం_స్థానభ్రంశం );
  }


  స్థిర FT_Error
  raccess_guess_darwin_ufs_export( FT_Library  library,
                                   FT_ప్రవాహం   ప్రవాహం,
                                   అక్షరం       *పునాది_file_name,
                                   అక్షరం      **ఫలితం_file_name,
                                   FT_Long    *ఫలితం_స్థానభ్రంశం )
  {
    అక్షరం*      newpath;
    FT_Error   error;
    FT_జ్ఞాపకం  జ్ఞాపకం;

    FT_వాడని( ప్రవాహం );


    జ్ఞాపకం  = library->జ్ఞాపకం;
    newpath = raccess_make_file_name( జ్ఞాపకం, పునాది_file_name, "._" );
    ఒకవేళ ( !newpath )
      తిరిగి FT_THROW( బహిర్గతం_యొక్క_జ్ఞాపకం );

    error = raccess_guess_linux_double_from_file_name( library, newpath,
                                                       ఫలితం_స్థానభ్రంశం );
    ఒకవేళ ( !error )
      *ఫలితం_file_name = newpath;
    లేకపోతే
      FT_FREE( newpath );

    తిరిగి error;
  }


  స్థిర FT_Error
  raccess_guess_darwin_hfsplus( FT_Library  library,
                                FT_ప్రవాహం   ప్రవాహం,
                                అక్షరం       *పునాది_file_name,
                                అక్షరం      **ఫలితం_file_name,
                                FT_Long    *ఫలితం_స్థానభ్రంశం )
  {
    /*
      Only meaningful on systems with hfs+ drivers (or Macs).
     */
    FT_Error   error;
    అక్షరం*      newpath = ఖాళీ;
    FT_జ్ఞాపకం  జ్ఞాపకం;
    FT_Long    పునాది_file_len = (FT_Long)ft_strlen( పునాది_file_name );

    FT_వాడని( ప్రవాహం );


    జ్ఞాపకం = library->జ్ఞాపకం;

    ఒకవేళ ( పునాది_file_len + 6 > FT_INT_MAX )
      తిరిగి FT_THROW( Array_Too_Large );

    ఒకవేళ ( FT_QALLOC( newpath, పునాది_file_len + 6 ) )
      తిరిగి error;

    FT_MEM_COPY( newpath, పునాది_file_name, పునాది_file_len );
    FT_MEM_COPY( newpath + పునాది_file_len, "/rsrc", 6 );

    *ఫలితం_file_name = newpath;
    *ఫలితం_స్థానభ్రంశం    = 0;

    తిరిగి FT_Err_Ok;
  }


  స్థిర FT_Error
  raccess_guess_darwin_newvfs( FT_Library  library,
                               FT_ప్రవాహం   ప్రవాహం,
                               అక్షరం       *పునాది_file_name,
                               అక్షరం      **ఫలితం_file_name,
                               FT_Long    *ఫలితం_స్థానభ్రంశం )
  {
    /*
      Only meaningful on systems with Mac OS X (> 10.1).
     */
    FT_Error   error;
    అక్షరం*      newpath = ఖాళీ;
    FT_జ్ఞాపకం  జ్ఞాపకం;
    FT_Long    పునాది_file_len = (FT_Long)ft_strlen( పునాది_file_name );

    FT_వాడని( ప్రవాహం );


    జ్ఞాపకం = library->జ్ఞాపకం;

    ఒకవేళ ( పునాది_file_len + 18 > FT_INT_MAX )
      తిరిగి FT_THROW( Array_Too_Large );

    ఒకవేళ ( FT_QALLOC( newpath, పునాది_file_len + 18 ) )
      తిరిగి error;

    FT_MEM_COPY( newpath, పునాది_file_name, పునాది_file_len );
    FT_MEM_COPY( newpath + పునాది_file_len, "/..namedfork/rsrc", 18 );

    *ఫలితం_file_name = newpath;
    *ఫలితం_స్థానభ్రంశం    = 0;

    తిరిగి FT_Err_Ok;
  }


  స్థిర FT_Error
  raccess_guess_vfat( FT_Library  library,
                      FT_ప్రవాహం   ప్రవాహం,
                      అక్షరం       *పునాది_file_name,
                      అక్షరం      **ఫలితం_file_name,
                      FT_Long    *ఫలితం_స్థానభ్రంశం )
  {
    అక్షరం*      newpath;
    FT_జ్ఞాపకం  జ్ఞాపకం;

    FT_వాడని( ప్రవాహం );


    జ్ఞాపకం = library->జ్ఞాపకం;

    newpath = raccess_make_file_name( జ్ఞాపకం, పునాది_file_name,
                                      "resource.frk/" );
    ఒకవేళ ( !newpath )
      తిరిగి FT_THROW( బహిర్గతం_యొక్క_జ్ఞాపకం );

    *ఫలితం_file_name = newpath;
    *ఫలితం_స్థానభ్రంశం    = 0;

    తిరిగి FT_Err_Ok;
  }


  స్థిర FT_Error
  raccess_guess_linux_cap( FT_Library  library,
                           FT_ప్రవాహం   ప్రవాహం,
                           అక్షరం       *పునాది_file_name,
                           అక్షరం      **ఫలితం_file_name,
                           FT_Long    *ఫలితం_స్థానభ్రంశం )
  {
    అక్షరం*      newpath;
    FT_జ్ఞాపకం  జ్ఞాపకం;

    FT_వాడని( ప్రవాహం );


    జ్ఞాపకం = library->జ్ఞాపకం;

    newpath = raccess_make_file_name( జ్ఞాపకం, పునాది_file_name, ".resource/" );
    ఒకవేళ ( !newpath )
      తిరిగి FT_THROW( బహిర్గతం_యొక్క_జ్ఞాపకం );

    *ఫలితం_file_name = newpath;
    *ఫలితం_స్థానభ్రంశం    = 0;

    తిరిగి FT_Err_Ok;
  }


  స్థిర FT_Error
  raccess_guess_linux_double( FT_Library  library,
                              FT_ప్రవాహం   ప్రవాహం,
                              అక్షరం       *పునాది_file_name,
                              అక్షరం      **ఫలితం_file_name,
                              FT_Long    *ఫలితం_స్థానభ్రంశం )
  {
    అక్షరం*      newpath;
    FT_Error   error;
    FT_జ్ఞాపకం  జ్ఞాపకం;

    FT_వాడని( ప్రవాహం );


    జ్ఞాపకం = library->జ్ఞాపకం;

    newpath = raccess_make_file_name( జ్ఞాపకం, పునాది_file_name, "%" );
    ఒకవేళ ( !newpath )
      తిరిగి FT_THROW( బహిర్గతం_యొక్క_జ్ఞాపకం );

    error = raccess_guess_linux_double_from_file_name( library, newpath,
                                                       ఫలితం_స్థానభ్రంశం );
    ఒకవేళ ( !error )
      *ఫలితం_file_name = newpath;
    లేకపోతే
      FT_FREE( newpath );

    తిరిగి error;
  }


  స్థిర FT_Error
  raccess_guess_linux_netatalk( FT_Library  library,
                                FT_ప్రవాహం   ప్రవాహం,
                                అక్షరం       *పునాది_file_name,
                                అక్షరం      **ఫలితం_file_name,
                                FT_Long    *ఫలితం_స్థానభ్రంశం )
  {
    అక్షరం*      newpath;
    FT_Error   error;
    FT_జ్ఞాపకం  జ్ఞాపకం;

    FT_వాడని( ప్రవాహం );


    జ్ఞాపకం = library->జ్ఞాపకం;

    newpath = raccess_make_file_name( జ్ఞాపకం, పునాది_file_name,
                                      ".AppleDouble/" );
    ఒకవేళ ( !newpath )
      తిరిగి FT_THROW( బహిర్గతం_యొక్క_జ్ఞాపకం );

    error = raccess_guess_linux_double_from_file_name( library, newpath,
                                                       ఫలితం_స్థానభ్రంశం );
    ఒకవేళ ( !error )
      *ఫలితం_file_name = newpath;
    లేకపోతే
      FT_FREE( newpath );

    తిరిగి error;
  }


  స్థిర FT_Error
  raccess_guess_apple_generic( FT_Library  library,
                               FT_ప్రవాహం   ప్రవాహం,
                               అక్షరం       *పునాది_file_name,
                               FT_Int32    magic,
                               FT_Long    *ఫలితం_స్థానభ్రంశం )
  {
    FT_Int32   magic_from_ప్రవాహం;
    FT_Error   error;
    FT_Int32   సంస్కరణ_సంఖ్య = 0;
    FT_UShort  n_యొక్క_entries;

    పూర్ణాంకం        i;
    FT_Int32   నమోదు_id, నమోదు_స్థానభ్రంశం, నమోదు_నిడివి = 0;

    స్థిరరాశి FT_Int32  resource_fork_నమోదు_id = 0x2;

    FT_వాడని( library );
    FT_వాడని( పునాది_file_name );
    FT_వాడని( సంస్కరణ_సంఖ్య );
    FT_వాడని( నమోదు_నిడివి   );


    ఒకవేళ ( FT_READ_LONG( magic_from_ప్రవాహం ) )
      తిరిగి error;
    ఒకవేళ ( magic_from_ప్రవాహం != magic )
      తిరిగి FT_THROW( Unknown_File_Format );

    ఒకవేళ ( FT_READ_LONG( సంస్కరణ_సంఖ్య ) )
      తిరిగి error;

    /* filler */
    error = FT_ప్రవాహం_Skip( ప్రవాహం, 16 );
    ఒకవేళ ( error )
      తిరిగి error;

    ఒకవేళ ( FT_READ_USHORT( n_యొక్క_entries ) )
      తిరిగి error;
    ఒకవేళ ( n_యొక్క_entries == 0 )
      తిరిగి FT_THROW( Unknown_File_Format );

    కొరకు ( i = 0; i < n_యొక్క_entries; i++ )
    {
      ఒకవేళ ( FT_READ_LONG( నమోదు_id ) )
        తిరిగి error;
      ఒకవేళ ( నమోదు_id == resource_fork_నమోదు_id )
      {
        ఒకవేళ ( FT_READ_LONG( నమోదు_స్థానభ్రంశం ) ||
             FT_READ_LONG( నమోదు_నిడివి ) )
          కొనసాగు;
        *ఫలితం_స్థానభ్రంశం = నమోదు_స్థానభ్రంశం;

        తిరిగి FT_Err_Ok;
      }
      లేకపోతే
      {
        error = FT_ప్రవాహం_Skip( ప్రవాహం, 4 + 4 );    /* offset + length */
        ఒకవేళ ( error )
          తిరిగి error;
      }
    }

    తిరిగి FT_THROW( Unknown_File_Format );
  }


  స్థిర FT_Error
  raccess_guess_linux_double_from_file_name( FT_Library  library,
                                             అక్షరం       *file_name,
                                             FT_Long    *ఫలితం_స్థానభ్రంశం )
  {
    FT_Open_వాదనలు  args2;
    FT_ప్రవాహం     stream2;
    అక్షరం*         nouse = ఖాళీ;
    FT_Error      error;


    args2.flags    = FT_OPEN_PATHNAME;
    args2.pathname = file_name;
    error = FT_ప్రవాహం_New( library, &args2, &stream2 );
    ఒకవేళ ( error )
      తిరిగి error;

    error = raccess_guess_apple_double( library, stream2, file_name,
                                        &nouse, ఫలితం_స్థానభ్రంశం );

    FT_ప్రవాహం_Free( stream2, 0 );

    తిరిగి error;
  }


  స్థిర అక్షరం*
  raccess_make_file_name( FT_జ్ఞాపకం    జ్ఞాపకం,
                          స్థిరరాశి అక్షరం  *original_name,
                          స్థిరరాశి అక్షరం  *insertion )
  {
    అక్షరం*        new_name = ఖాళీ;
    స్థిరరాశి అక్షరం*  tmp;
    స్థిరరాశి అక్షరం*  slash;
    size_t       new_నిడివి;
    FT_Error     error;


    new_నిడివి = ft_strlen( original_name ) + ft_strlen( insertion );
    ఒకవేళ ( FT_QALLOC( new_name, new_నిడివి + 1 ) )
      తిరిగి ఖాళీ;

    tmp = ft_strrchr( original_name, '/' );
    ఒకవేళ ( tmp )
    {
      ft_strncpy( new_name,
                  original_name,
                  (size_t)( tmp - original_name + 1 ) );
      new_name[tmp - original_name + 1] = '\0';
      slash = tmp + 1;
    }
    లేకపోతే
    {
      slash       = original_name;
      new_name[0] = '\0';
    }

    ft_strcat( new_name, insertion );
    ft_strcat( new_name, slash );

    తిరిగి new_name;
  }


#లేకపోతే   /* !FT_CONFIG_OPTION_GUESSING_EMBEDDED_RFORK */


  /**************************************************************************
   *                 Dummy function; just sets errors
   */

  FT_పునాది_DEF( శూన్య )
  FT_Raccess_Guess( FT_Library  library,
                    FT_ప్రవాహం   ప్రవాహం,
                    అక్షరం       *పునాది_name,
                    అక్షరం      **new_పేర్లు,
                    FT_Long    *offsets,
                    FT_Error   *errors )
  {
    FT_Int  i;

    FT_వాడని( library );
    FT_వాడని( ప్రవాహం );
    FT_వాడని( పునాది_name );


    కొరకు ( i = 0; i < FT_RACCESS_N_నియమాలు; i++ )
    {
      new_పేర్లు[i] = ఖాళీ;
      offsets[i]   = 0;
      errors[i]    = FT_ERR( Unimplemented_Feature );
    }
  }


#అయితే_ముగింపు  /* !FT_CONFIG_OPTION_GUESSING_EMBEDDED_RFORK */


/*చివర*/
