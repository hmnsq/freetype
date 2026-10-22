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


#ಅನ್ತರ್ಭಾವ <freetype/internal/ftdebug.h>
#ಅನ್ತರ್ಭಾವ <freetype/internal/ftstream.h>
#ಅನ್ತರ್ಭಾವ <freetype/internal/ftrfork.h>

#ಅನ್ತರ್ಭಾವ "ftbase.h"

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು  FT_COMPONENT
#ವ್ಯಾಖ್ಯಾನಿಸು FT_COMPONENT  raccess


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

  FT_ತಳ_DEF( FT_Error )
  FT_Raccess_Get_ಶೀರ್ಷಿಕೆ_Info( FT_Library  library,
                             FT_ಹರಿವು   ಹರಿವು,
                             FT_Long     rfork_ಸ್ಥಾನಾಂತರ,
                             FT_Long    *map_ಸ್ಥಾನಾಂತರ,
                             FT_Long    *rdata_pos )
  {
    FT_Error       error;
    ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ  ತಲೆ[16], head2[16];
    FT_Long        map_pos, map_len, rdata_len;
    ಪೂರ್ಣಾಂಕ            allzeros, allmatch, i;
    FT_Long        type_list;

    FT_ಬಳಸದ( library );


    error = FT_ಹರಿವು_Seek( ಹರಿವು, (FT_ULong)rfork_ಸ್ಥಾನಾಂತರ );
    ಆದರೆ ( error )
      ಹಿಂತಿರುಗಿ error;

    error = FT_ಹರಿವು_Read( ಹರಿವು, (FT_Byte*)ತಲೆ, 16 );
    ಆದರೆ ( error )
      ಹಿಂತಿರುಗಿ error;

    /* ensure positive values */
    ಆದರೆ ( ತಲೆ[0]  >= 0x80 ||
         ತಲೆ[4]  >= 0x80 ||
         ತಲೆ[8]  >= 0x80 ||
         ತಲೆ[12] >= 0x80 )
      ಹಿಂತಿರುಗಿ FT_THROW( Unknown_File_Format );

    *rdata_pos = ( ತಲೆ[ 0] << 24 ) |
                 ( ತಲೆ[ 1] << 16 ) |
                 ( ತಲೆ[ 2] <<  8 ) |
                   ತಲೆ[ 3];
    map_pos    = ( ತಲೆ[ 4] << 24 ) |
                 ( ತಲೆ[ 5] << 16 ) |
                 ( ತಲೆ[ 6] <<  8 ) |
                   ತಲೆ[ 7];
    rdata_len  = ( ತಲೆ[ 8] << 24 ) |
                 ( ತಲೆ[ 9] << 16 ) |
                 ( ತಲೆ[10] <<  8 ) |
                   ತಲೆ[11];
    map_len    = ( ತಲೆ[12] << 24 ) |
                 ( ತಲೆ[13] << 16 ) |
                 ( ತಲೆ[14] <<  8 ) |
                   ತಲೆ[15];

    /* the map must not be empty */
    ಆದರೆ ( !map_pos )
      ಹಿಂತಿರುಗಿ FT_THROW( Unknown_File_Format );

    /* check whether rdata and map overlap */
    ಆದರೆ ( *rdata_pos < map_pos )
    {
      ಆದರೆ ( *rdata_pos > map_pos - rdata_len )
        ಹಿಂತಿರುಗಿ FT_THROW( Unknown_File_Format );
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
      ಆದರೆ ( map_pos > *rdata_pos - map_len )
        ಹಿಂತಿರುಗಿ FT_THROW( Unknown_File_Format );
    }

    /* check whether end of rdata or map exceeds stream size */
    ಆದರೆ ( FT_LONG_MAX - rdata_len < *rdata_pos                               ||
         FT_LONG_MAX - map_len < map_pos                                    ||

         FT_LONG_MAX - ( *rdata_pos + rdata_len ) < rfork_ಸ್ಥಾನಾಂತರ            ||
         FT_LONG_MAX - ( map_pos + map_len ) < rfork_ಸ್ಥಾನಾಂತರ                 ||

         (FT_ULong)( rfork_ಸ್ಥಾನಾಂತರ + *rdata_pos + rdata_len ) > ಹರಿವು->ಗಾತ್ರ_2 ||
         (FT_ULong)( rfork_ಸ್ಥಾನಾಂತರ + map_pos + map_len ) > ಹರಿವು->ಗಾತ್ರ_2      )
      ಹಿಂತಿರುಗಿ FT_THROW( Unknown_File_Format );

    *rdata_pos += rfork_ಸ್ಥಾನಾಂತರ;
    map_pos    += rfork_ಸ್ಥಾನಾಂತರ;

    error = FT_ಹರಿವು_Seek( ಹರಿವು, (FT_ULong)map_pos );
    ಆದರೆ ( error )
      ಹಿಂತಿರುಗಿ error;

    head2[15] = (FT_Byte)( ತಲೆ[15] + 1 );       /* make it be different */

    error = FT_ಹರಿವು_Read( ಹರಿವು, (FT_Byte*)head2, 16 );
    ಆದರೆ ( error )
      ಹಿಂತಿರುಗಿ error;

    allzeros = 1;
    allmatch = 1;
    ಗಾಗಿ ( i = 0; i < 16; i++ )
    {
      ಆದರೆ ( head2[i] != 0 )
        allzeros = 0;
      ಆದರೆ ( head2[i] != ತಲೆ[i] )
        allmatch = 0;
    }
    ಆದರೆ ( !allzeros && !allmatch )
      ಹಿಂತಿರುಗಿ FT_THROW( Unknown_File_Format );

    /* If we have reached this point then it is probably a mac resource */
    /* file.  Now, does it contain any interesting resources?           */

    (ಶೂನ್ಯ)FT_ಹರಿವು_SKIP( 4        /* skip handle to next resource map */
                          + 2      /* skip file resource number */
                          + 2 );   /* skip attributes */

    ಆದರೆ ( FT_READ_SHORT( type_list ) )
      ಹಿಂತಿರುಗಿ error;
    ಆದರೆ ( type_list < 0 )
      ಹಿಂತಿರುಗಿ FT_THROW( Unknown_File_Format );

    error = FT_ಹರಿವು_Seek( ಹರಿವು, (FT_ULong)( map_pos + type_list ) );
    ಆದರೆ ( error )
      ಹಿಂತಿರುಗಿ error;

    *map_ಸ್ಥಾನಾಂತರ = map_pos + type_list;
    ಹಿಂತಿರುಗಿ FT_Err_Ok;
  }


  FT_COMPARE_DEF( ಪೂರ್ಣಾಂಕ )
  ft_raccess_ವಿಂಗಡಿಸು_ref_by_id( ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ*  a,
                             ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ*  b )
  {
    ಹಿಂತಿರುಗಿ  ( (FT_RFork_Ref*)a )->res_id - ( (FT_RFork_Ref*)b )->res_id;
  }


  FT_ತಳ_DEF( FT_Error )
  FT_Raccess_Get_ದತ್ತಾಂಶ_Offsets( FT_Library  library,
                              FT_ಹರಿವು   ಹರಿವು,
                              FT_Long     map_ಸ್ಥಾನಾಂತರ,
                              FT_Long     rdata_pos,
                              FT_Long     ಟ್ಯಾಗ್,
                              FT_Bool     ವಿಂಗಡಿಸು_by_res_id,
                              FT_Long   **offsets,
                              FT_Long    *ಎಣಿಕೆ_2 )
  {
    FT_Error      error;
    ಪೂರ್ಣಾಂಕ           i, j, cnt, subcnt;
    FT_Long       ಟ್ಯಾಗ್_ಆಂತರಿಕ, rpos;
    FT_ಸ್ಮರಣೆ     ಸ್ಮರಣೆ = library->ಸ್ಮರಣೆ;
    FT_Long       ತಾತ್ಕಾಲಿಕ;
    FT_Long       *offsets_ಆಂತರಿಕ = ಖಾಲಿ;
    FT_RFork_Ref  *ref = ಖಾಲಿ;


    FT_TRACE3(( "\n" ));
    error = FT_ಹರಿವು_Seek( ಹರಿವು, (FT_ULong)map_ಸ್ಥಾನಾಂತರ );
    ಆದರೆ ( error )
      ಹಿಂತಿರುಗಿ error;

    ಆದರೆ ( FT_READ_SHORT( cnt ) )
      ಹಿಂತಿರುಗಿ error;
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
    ಆದರೆ ( cnt > 4079 )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ಕೋಷ್ಟಕ );

    ಗಾಗಿ ( i = 0; i < cnt; i++ )
    {
      ಆದರೆ ( FT_READ_LONG( ಟ್ಯಾಗ್_ಆಂತರಿಕ ) ||
           FT_READ_SHORT( subcnt )      ||
           FT_READ_SHORT( rpos )        )
        ಹಿಂತಿರುಗಿ error;

      FT_TRACE2(( "Resource tags: %c%c%c%c\n",
                  (ಅಕ್ಷರ)( 0xFF & ( ಟ್ಯಾಗ್_ಆಂತರಿಕ >> 24 ) ),
                  (ಅಕ್ಷರ)( 0xFF & ( ಟ್ಯಾಗ್_ಆಂತರಿಕ >> 16 ) ),
                  (ಅಕ್ಷರ)( 0xFF & ( ಟ್ಯಾಗ್_ಆಂತರಿಕ >>  8 ) ),
                  (ಅಕ್ಷರ)( 0xFF & ( ಟ್ಯಾಗ್_ಆಂತರಿಕ >>  0 ) ) ));
      FT_TRACE3(( "             : subcount=%d, suboffset=0x%04lx\n",
                  subcnt, rpos ));

      ಆದರೆ ( ಟ್ಯಾಗ್_ಆಂತರಿಕ == ಟ್ಯಾಗ್ )
      {
        *ಎಣಿಕೆ_2 = subcnt + 1;
        rpos  += map_ಸ್ಥಾನಾಂತರ;

        /* a zero count might be valid in the resource specification, */
        /* however, it is completely useless to us                    */
        ಆದರೆ ( *ಎಣಿಕೆ_2 < 1 || *ಎಣಿಕೆ_2 > 2727 )
          ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ಕೋಷ್ಟಕ );

        error = FT_ಹರಿವು_Seek( ಹರಿವು, (FT_ULong)rpos );
        ಆದರೆ ( error )
          ಹಿಂತಿರುಗಿ error;

        ಆದರೆ ( FT_QNEW_ARRAY( ref, *ಎಣಿಕೆ_2 ) )
          ಹಿಂತಿರುಗಿ error;

        ಗಾಗಿ ( j = 0; j < *ಎಣಿಕೆ_2; j++ )
        {
          ಆದರೆ ( FT_READ_SHORT( ref[j].res_id ) )
            ಹೋಗು Exit;
          ಆದರೆ ( FT_ಹರಿವು_SKIP( 2 ) )  /* resource name offset */
            ಹೋಗು Exit;
          ಆದರೆ ( FT_READ_LONG( ತಾತ್ಕಾಲಿಕ ) ) /* attributes (8bit), offset (24bit) */
            ಹೋಗು Exit;
          ಆದರೆ ( FT_ಹರಿವು_SKIP( 4 ) )  /* mbz */
            ಹೋಗು Exit;

          /*
           * According to Inside Macintosh: More Macintosh Toolbox,
           * "Resource IDs" (1-46), there are some reserved IDs.
           * However, FreeType2 is not a font synthesizer, no need
           * to check the acceptable resource ID or its attributes.
           */
          ref[j].ಸ್ಥಾನಾಂತರ = ತಾತ್ಕಾಲಿಕ & 0xFFFFFFL;

          FT_TRACE3(( "             [%d]:"
                      " resource_id=0x%04x, offset=0x%08lx\n",
                      j, (FT_UShort)ref[j].res_id, ref[j].ಸ್ಥಾನಾಂತರ ));
        }

        ಆದರೆ ( ವಿಂಗಡಿಸು_by_res_id )
        {
          ft_qsort( ref,
                    (size_t)*ಎಣಿಕೆ_2,
                    ಗಾತ್ರ ( FT_RFork_Ref ),
                    ft_raccess_ವಿಂಗಡಿಸು_ref_by_id );

          FT_TRACE3(( "             -- sort resources by their ids --\n" ));

          ಗಾಗಿ ( j = 0; j < *ಎಣಿಕೆ_2; j++ )
            FT_TRACE3(( "             [%d]:"
                        " resource_id=0x%04x, offset=0x%08lx\n",
                        j, ref[j].res_id, ref[j].ಸ್ಥಾನಾಂತರ ));
        }

        ಆದರೆ ( FT_QNEW_ARRAY( offsets_ಆಂತರಿಕ, *ಎಣಿಕೆ_2 ) )
          ಹೋಗು Exit;

        /* XXX: duplicated reference ID,
         *      gap between reference IDs are acceptable?
         *      further investigation on Apple implementation is needed.
         */
        ಗಾಗಿ ( j = 0; j < *ಎಣಿಕೆ_2; j++ )
          offsets_ಆಂತರಿಕ[j] = rdata_pos + ref[j].ಸ್ಥಾನಾಂತರ;

        *offsets = offsets_ಆಂತರಿಕ;
        error    = FT_Err_Ok;

      Exit:
        FT_FREE( ref );
        ಹಿಂತಿರುಗಿ error;
      }
    }

    ಹಿಂತಿರುಗಿ FT_THROW( Cannot_Open_Resource );
  }


#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ FT_CONFIG_OPTION_GUESSING_EMBEDDED_RFORK

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

  ಸ್ಥಿರ FT_Error
  raccess_guess_apple_double( FT_Library  library,
                              FT_ಹರಿವು   ಹರಿವು,
                              ಅಕ್ಷರ       *ತಳ_file_name,
                              ಅಕ್ಷರ      **ಫಲಿತಾಂಶ_file_name,
                              FT_Long    *ಫಲಿತಾಂಶ_ಸ್ಥಾನಾಂತರ );

  ಸ್ಥಿರ FT_Error
  raccess_guess_apple_single( FT_Library  library,
                              FT_ಹರಿವು   ಹರಿವು,
                              ಅಕ್ಷರ       *ತಳ_file_name,
                              ಅಕ್ಷರ      **ಫಲಿತಾಂಶ_file_name,
                              FT_Long    *ಫಲಿತಾಂಶ_ಸ್ಥಾನಾಂತರ );

  ಸ್ಥಿರ FT_Error
  raccess_guess_darwin_ufs_export( FT_Library  library,
                                   FT_ಹರಿವು   ಹರಿವು,
                                   ಅಕ್ಷರ       *ತಳ_file_name,
                                   ಅಕ್ಷರ      **ಫಲಿತಾಂಶ_file_name,
                                   FT_Long    *ಫಲಿತಾಂಶ_ಸ್ಥಾನಾಂತರ );

  ಸ್ಥಿರ FT_Error
  raccess_guess_darwin_newvfs( FT_Library  library,
                               FT_ಹರಿವು   ಹರಿವು,
                               ಅಕ್ಷರ       *ತಳ_file_name,
                               ಅಕ್ಷರ      **ಫಲಿತಾಂಶ_file_name,
                               FT_Long    *ಫಲಿತಾಂಶ_ಸ್ಥಾನಾಂತರ );

  ಸ್ಥಿರ FT_Error
  raccess_guess_darwin_hfsplus( FT_Library  library,
                                FT_ಹರಿವು   ಹರಿವು,
                                ಅಕ್ಷರ       *ತಳ_file_name,
                                ಅಕ್ಷರ      **ಫಲಿತಾಂಶ_file_name,
                                FT_Long    *ಫಲಿತಾಂಶ_ಸ್ಥಾನಾಂತರ );

  ಸ್ಥಿರ FT_Error
  raccess_guess_vfat( FT_Library  library,
                      FT_ಹರಿವು   ಹರಿವು,
                      ಅಕ್ಷರ       *ತಳ_file_name,
                      ಅಕ್ಷರ      **ಫಲಿತಾಂಶ_file_name,
                      FT_Long    *ಫಲಿತಾಂಶ_ಸ್ಥಾನಾಂತರ );

  ಸ್ಥಿರ FT_Error
  raccess_guess_linux_cap( FT_Library  library,
                           FT_ಹರಿವು   ಹರಿವು,
                           ಅಕ್ಷರ       *ತಳ_file_name,
                           ಅಕ್ಷರ      **ಫಲಿತಾಂಶ_file_name,
                           FT_Long    *ಫಲಿತಾಂಶ_ಸ್ಥಾನಾಂತರ );

  ಸ್ಥಿರ FT_Error
  raccess_guess_linux_double( FT_Library  library,
                              FT_ಹರಿವು   ಹರಿವು,
                              ಅಕ್ಷರ       *ತಳ_file_name,
                              ಅಕ್ಷರ      **ಫಲಿತಾಂಶ_file_name,
                              FT_Long    *ಫಲಿತಾಂಶ_ಸ್ಥಾನಾಂತರ );

  ಸ್ಥಿರ FT_Error
  raccess_guess_linux_netatalk( FT_Library  library,
                                FT_ಹರಿವು   ಹರಿವು,
                                ಅಕ್ಷರ       *ತಳ_file_name,
                                ಅಕ್ಷರ      **ಫಲಿತಾಂಶ_file_name,
                                FT_Long    *ಫಲಿತಾಂಶ_ಸ್ಥಾನಾಂತರ );


  CONST_FT_RFORK_ನಿಯಮ_ARRAY_BEGIN( ft_raccess_guess_ಕೋಷ್ಟಕ,
                                                      ft_raccess_guess_rec )
  CONST_FT_RFORK_ನಿಯಮ_ARRAY_ನಮೂದು( apple_double,      apple_double )
  CONST_FT_RFORK_ನಿಯಮ_ARRAY_ನಮೂದು( apple_single,      apple_single )
  CONST_FT_RFORK_ನಿಯಮ_ARRAY_ನಮೂದು( darwin_ufs_export, darwin_ufs_export )
  CONST_FT_RFORK_ನಿಯಮ_ARRAY_ನಮೂದು( darwin_newvfs,     darwin_newvfs )
  CONST_FT_RFORK_ನಿಯಮ_ARRAY_ನಮೂದು( darwin_hfsplus,    darwin_hfsplus )
  CONST_FT_RFORK_ನಿಯಮ_ARRAY_ನಮೂದು( vfat,              vfat )
  CONST_FT_RFORK_ನಿಯಮ_ARRAY_ನಮೂದು( linux_cap,         linux_cap )
  CONST_FT_RFORK_ನಿಯಮ_ARRAY_ನಮೂದು( linux_double,      linux_double )
  CONST_FT_RFORK_ನಿಯಮ_ARRAY_ನಮೂದು( linux_netatalk,    linux_netatalk )
  CONST_FT_RFORK_ನಿಯಮ_ARRAY_ಕೊನೆ


  /*************************************************************************/
  /****                                                                 ****/
  /****                       Helper functions                          ****/
  /****                                                                 ****/
  /*************************************************************************/

  ಸ್ಥಿರ FT_Error
  raccess_guess_apple_generic( FT_Library  library,
                               FT_ಹರಿವು   ಹರಿವು,
                               ಅಕ್ಷರ       *ತಳ_file_name,
                               FT_Int32    magic,
                               FT_Long    *ಫಲಿತಾಂಶ_ಸ್ಥಾನಾಂತರ );

  ಸ್ಥಿರ FT_Error
  raccess_guess_linux_double_from_file_name( FT_Library  library,
                                             ಅಕ್ಷರ*       file_name,
                                             FT_Long    *ಫಲಿತಾಂಶ_ಸ್ಥಾನಾಂತರ );

  ಸ್ಥಿರ ಅಕ್ಷರ *
  raccess_make_file_name( FT_ಸ್ಮರಣೆ    ಸ್ಮರಣೆ,
                          ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ  *original_name,
                          ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ  *insertion );

  FT_ತಳ_DEF( ಶೂನ್ಯ )
  FT_Raccess_Guess( FT_Library  library,
                    FT_ಹರಿವು   ಹರಿವು,
                    ಅಕ್ಷರ*       ತಳ_name,
                    ಅಕ್ಷರ      **new_ಹೆಸರುಗಳು,
                    FT_Long    *offsets,
                    FT_Error   *errors )
  {
    FT_Int  i;


    ಗಾಗಿ ( i = 0; i < FT_RACCESS_N_ನಿಯಮಗಳು; i++ )
    {
      new_ಹೆಸರುಗಳು[i] = ಖಾಲಿ;
      ಆದರೆ ( ಖಾಲಿ != ಹರಿವು )
        errors[i] = FT_ಹರಿವು_Seek( ಹರಿವು, 0 );
      ಇಲ್ಲದಿದ್ದರೆ
        errors[i] = FT_Err_Ok;

      ಆದರೆ ( errors[i] )
        ಮುಂದುವರಿ;

      errors[i] = ft_raccess_guess_ಕೋಷ್ಟಕ[i].func( library,
                                                  ಹರಿವು, ತಳ_name,
                                                  &(new_ಹೆಸರುಗಳು[i]),
                                                  &(offsets[i]) );
    }

    ಹಿಂತಿರುಗಿ;
  }


#ಆದರೆ defined( FT_CONFIG_OPTION_MAC_FONTS ) && !defined( FT_MACINTOSH )
  ಸ್ಥಿರ FT_RFork_ನಿಯಮ
  raccess_get_ನಿಯಮ_type_from_ನಿಯಮ_ಸೂಚ್ಯಂಕ( FT_Library  library,
                                         FT_UInt     ನಿಯಮ_ಸೂಚ್ಯಂಕ )
  {
    FT_ಬಳಸದ( library );

    ಆದರೆ ( ನಿಯಮ_ಸೂಚ್ಯಂಕ >= FT_RACCESS_N_ನಿಯಮಗಳು )
      ಹಿಂತಿರುಗಿ FT_RFork_ನಿಯಮ_invalid;

    ಹಿಂತಿರುಗಿ ft_raccess_guess_ಕೋಷ್ಟಕ[ನಿಯಮ_ಸೂಚ್ಯಂಕ].type;
  }


  /*
   * For this function, refer ftbase.h.
   */
  FT_LOCAL_DEF( FT_Bool )
  ft_raccess_ನಿಯಮ_by_darwin_vfs( FT_Library  library,
                                 FT_UInt     ನಿಯಮ_ಸೂಚ್ಯಂಕ )
  {
    ಆಯ್ಕೆ( raccess_get_ನಿಯಮ_type_from_ನಿಯಮ_ಸೂಚ್ಯಂಕ( library, ನಿಯಮ_ಸೂಚ್ಯಂಕ ) )
    {
      ಸ್ಥಿತಿ FT_RFork_ನಿಯಮ_darwin_newvfs:
      ಸ್ಥಿತಿ FT_RFork_ನಿಯಮ_darwin_hfsplus:
        ಹಿಂತಿರುಗಿ TRUE;

      ಪೂರ್ವನಿಯೋಜಿತ:
        ಹಿಂತಿರುಗಿ FALSE;
    }
  }
#ಆದರೆ_ಅಂತ್ಯ


  ಸ್ಥಿರ FT_Error
  raccess_guess_apple_double( FT_Library  library,
                              FT_ಹರಿವು   ಹರಿವು,
                              ಅಕ್ಷರ       *ತಳ_file_name,
                              ಅಕ್ಷರ      **ಫಲಿತಾಂಶ_file_name,
                              FT_Long    *ಫಲಿತಾಂಶ_ಸ್ಥಾನಾಂತರ )
  {
    FT_Int32  magic = ( 0x00 << 24 ) |
                      ( 0x05 << 16 ) |
                      ( 0x16 <<  8 ) |
                        0x07;


    *ಫಲಿತಾಂಶ_file_name = ಖಾಲಿ;
    ಆದರೆ ( ಖಾಲಿ == ಹರಿವು )
      ಹಿಂತಿರುಗಿ FT_THROW( Cannot_Open_ಹರಿವು );

    ಹಿಂತಿರುಗಿ raccess_guess_apple_generic( library, ಹರಿವು, ತಳ_file_name,
                                        magic, ಫಲಿತಾಂಶ_ಸ್ಥಾನಾಂತರ );
  }


  ಸ್ಥಿರ FT_Error
  raccess_guess_apple_single( FT_Library  library,
                              FT_ಹರಿವು   ಹರಿವು,
                              ಅಕ್ಷರ       *ತಳ_file_name,
                              ಅಕ್ಷರ      **ಫಲಿತಾಂಶ_file_name,
                              FT_Long    *ಫಲಿತಾಂಶ_ಸ್ಥಾನಾಂತರ )
  {
    FT_Int32  magic = ( 0x00 << 24 ) |
                      ( 0x05 << 16 ) |
                      ( 0x16 <<  8 ) |
                        0x00;


    *ಫಲಿತಾಂಶ_file_name = ಖಾಲಿ;
    ಆದರೆ ( ಖಾಲಿ == ಹರಿವು )
      ಹಿಂತಿರುಗಿ FT_THROW( Cannot_Open_ಹರಿವು );

    ಹಿಂತಿರುಗಿ raccess_guess_apple_generic( library, ಹರಿವು, ತಳ_file_name,
                                        magic, ಫಲಿತಾಂಶ_ಸ್ಥಾನಾಂತರ );
  }


  ಸ್ಥಿರ FT_Error
  raccess_guess_darwin_ufs_export( FT_Library  library,
                                   FT_ಹರಿವು   ಹರಿವು,
                                   ಅಕ್ಷರ       *ತಳ_file_name,
                                   ಅಕ್ಷರ      **ಫಲಿತಾಂಶ_file_name,
                                   FT_Long    *ಫಲಿತಾಂಶ_ಸ್ಥಾನಾಂತರ )
  {
    ಅಕ್ಷರ*      newpath;
    FT_Error   error;
    FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ;

    FT_ಬಳಸದ( ಹರಿವು );


    ಸ್ಮರಣೆ  = library->ಸ್ಮರಣೆ;
    newpath = raccess_make_file_name( ಸ್ಮರಣೆ, ತಳ_file_name, "._" );
    ಆದರೆ ( !newpath )
      ಹಿಂತಿರುಗಿ FT_THROW( ಹೊರಹೋಗು_ದ_ಸ್ಮರಣೆ );

    error = raccess_guess_linux_double_from_file_name( library, newpath,
                                                       ಫಲಿತಾಂಶ_ಸ್ಥಾನಾಂತರ );
    ಆದರೆ ( !error )
      *ಫಲಿತಾಂಶ_file_name = newpath;
    ಇಲ್ಲದಿದ್ದರೆ
      FT_FREE( newpath );

    ಹಿಂತಿರುಗಿ error;
  }


  ಸ್ಥಿರ FT_Error
  raccess_guess_darwin_hfsplus( FT_Library  library,
                                FT_ಹರಿವು   ಹರಿವು,
                                ಅಕ್ಷರ       *ತಳ_file_name,
                                ಅಕ್ಷರ      **ಫಲಿತಾಂಶ_file_name,
                                FT_Long    *ಫಲಿತಾಂಶ_ಸ್ಥಾನಾಂತರ )
  {
    /*
      Only meaningful on systems with hfs+ drivers (or Macs).
     */
    FT_Error   error;
    ಅಕ್ಷರ*      newpath = ಖಾಲಿ;
    FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ;
    FT_Long    ತಳ_file_len = (FT_Long)ft_strlen( ತಳ_file_name );

    FT_ಬಳಸದ( ಹರಿವು );


    ಸ್ಮರಣೆ = library->ಸ್ಮರಣೆ;

    ಆದರೆ ( ತಳ_file_len + 6 > FT_INT_MAX )
      ಹಿಂತಿರುಗಿ FT_THROW( Array_Too_Large );

    ಆದರೆ ( FT_QALLOC( newpath, ತಳ_file_len + 6 ) )
      ಹಿಂತಿರುಗಿ error;

    FT_MEM_COPY( newpath, ತಳ_file_name, ತಳ_file_len );
    FT_MEM_COPY( newpath + ತಳ_file_len, "/rsrc", 6 );

    *ಫಲಿತಾಂಶ_file_name = newpath;
    *ಫಲಿತಾಂಶ_ಸ್ಥಾನಾಂತರ    = 0;

    ಹಿಂತಿರುಗಿ FT_Err_Ok;
  }


  ಸ್ಥಿರ FT_Error
  raccess_guess_darwin_newvfs( FT_Library  library,
                               FT_ಹರಿವು   ಹರಿವು,
                               ಅಕ್ಷರ       *ತಳ_file_name,
                               ಅಕ್ಷರ      **ಫಲಿತಾಂಶ_file_name,
                               FT_Long    *ಫಲಿತಾಂಶ_ಸ್ಥಾನಾಂತರ )
  {
    /*
      Only meaningful on systems with Mac OS X (> 10.1).
     */
    FT_Error   error;
    ಅಕ್ಷರ*      newpath = ಖಾಲಿ;
    FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ;
    FT_Long    ತಳ_file_len = (FT_Long)ft_strlen( ತಳ_file_name );

    FT_ಬಳಸದ( ಹರಿವು );


    ಸ್ಮರಣೆ = library->ಸ್ಮರಣೆ;

    ಆದರೆ ( ತಳ_file_len + 18 > FT_INT_MAX )
      ಹಿಂತಿರುಗಿ FT_THROW( Array_Too_Large );

    ಆದರೆ ( FT_QALLOC( newpath, ತಳ_file_len + 18 ) )
      ಹಿಂತಿರುಗಿ error;

    FT_MEM_COPY( newpath, ತಳ_file_name, ತಳ_file_len );
    FT_MEM_COPY( newpath + ತಳ_file_len, "/..namedfork/rsrc", 18 );

    *ಫಲಿತಾಂಶ_file_name = newpath;
    *ಫಲಿತಾಂಶ_ಸ್ಥಾನಾಂತರ    = 0;

    ಹಿಂತಿರುಗಿ FT_Err_Ok;
  }


  ಸ್ಥಿರ FT_Error
  raccess_guess_vfat( FT_Library  library,
                      FT_ಹರಿವು   ಹರಿವು,
                      ಅಕ್ಷರ       *ತಳ_file_name,
                      ಅಕ್ಷರ      **ಫಲಿತಾಂಶ_file_name,
                      FT_Long    *ಫಲಿತಾಂಶ_ಸ್ಥಾನಾಂತರ )
  {
    ಅಕ್ಷರ*      newpath;
    FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ;

    FT_ಬಳಸದ( ಹರಿವು );


    ಸ್ಮರಣೆ = library->ಸ್ಮರಣೆ;

    newpath = raccess_make_file_name( ಸ್ಮರಣೆ, ತಳ_file_name,
                                      "resource.frk/" );
    ಆದರೆ ( !newpath )
      ಹಿಂತಿರುಗಿ FT_THROW( ಹೊರಹೋಗು_ದ_ಸ್ಮರಣೆ );

    *ಫಲಿತಾಂಶ_file_name = newpath;
    *ಫಲಿತಾಂಶ_ಸ್ಥಾನಾಂತರ    = 0;

    ಹಿಂತಿರುಗಿ FT_Err_Ok;
  }


  ಸ್ಥಿರ FT_Error
  raccess_guess_linux_cap( FT_Library  library,
                           FT_ಹರಿವು   ಹರಿವು,
                           ಅಕ್ಷರ       *ತಳ_file_name,
                           ಅಕ್ಷರ      **ಫಲಿತಾಂಶ_file_name,
                           FT_Long    *ಫಲಿತಾಂಶ_ಸ್ಥಾನಾಂತರ )
  {
    ಅಕ್ಷರ*      newpath;
    FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ;

    FT_ಬಳಸದ( ಹರಿವು );


    ಸ್ಮರಣೆ = library->ಸ್ಮರಣೆ;

    newpath = raccess_make_file_name( ಸ್ಮರಣೆ, ತಳ_file_name, ".resource/" );
    ಆದರೆ ( !newpath )
      ಹಿಂತಿರುಗಿ FT_THROW( ಹೊರಹೋಗು_ದ_ಸ್ಮರಣೆ );

    *ಫಲಿತಾಂಶ_file_name = newpath;
    *ಫಲಿತಾಂಶ_ಸ್ಥಾನಾಂತರ    = 0;

    ಹಿಂತಿರುಗಿ FT_Err_Ok;
  }


  ಸ್ಥಿರ FT_Error
  raccess_guess_linux_double( FT_Library  library,
                              FT_ಹರಿವು   ಹರಿವು,
                              ಅಕ್ಷರ       *ತಳ_file_name,
                              ಅಕ್ಷರ      **ಫಲಿತಾಂಶ_file_name,
                              FT_Long    *ಫಲಿತಾಂಶ_ಸ್ಥಾನಾಂತರ )
  {
    ಅಕ್ಷರ*      newpath;
    FT_Error   error;
    FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ;

    FT_ಬಳಸದ( ಹರಿವು );


    ಸ್ಮರಣೆ = library->ಸ್ಮರಣೆ;

    newpath = raccess_make_file_name( ಸ್ಮರಣೆ, ತಳ_file_name, "%" );
    ಆದರೆ ( !newpath )
      ಹಿಂತಿರುಗಿ FT_THROW( ಹೊರಹೋಗು_ದ_ಸ್ಮರಣೆ );

    error = raccess_guess_linux_double_from_file_name( library, newpath,
                                                       ಫಲಿತಾಂಶ_ಸ್ಥಾನಾಂತರ );
    ಆದರೆ ( !error )
      *ಫಲಿತಾಂಶ_file_name = newpath;
    ಇಲ್ಲದಿದ್ದರೆ
      FT_FREE( newpath );

    ಹಿಂತಿರುಗಿ error;
  }


  ಸ್ಥಿರ FT_Error
  raccess_guess_linux_netatalk( FT_Library  library,
                                FT_ಹರಿವು   ಹರಿವು,
                                ಅಕ್ಷರ       *ತಳ_file_name,
                                ಅಕ್ಷರ      **ಫಲಿತಾಂಶ_file_name,
                                FT_Long    *ಫಲಿತಾಂಶ_ಸ್ಥಾನಾಂತರ )
  {
    ಅಕ್ಷರ*      newpath;
    FT_Error   error;
    FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ;

    FT_ಬಳಸದ( ಹರಿವು );


    ಸ್ಮರಣೆ = library->ಸ್ಮರಣೆ;

    newpath = raccess_make_file_name( ಸ್ಮರಣೆ, ತಳ_file_name,
                                      ".AppleDouble/" );
    ಆದರೆ ( !newpath )
      ಹಿಂತಿರುಗಿ FT_THROW( ಹೊರಹೋಗು_ದ_ಸ್ಮರಣೆ );

    error = raccess_guess_linux_double_from_file_name( library, newpath,
                                                       ಫಲಿತಾಂಶ_ಸ್ಥಾನಾಂತರ );
    ಆದರೆ ( !error )
      *ಫಲಿತಾಂಶ_file_name = newpath;
    ಇಲ್ಲದಿದ್ದರೆ
      FT_FREE( newpath );

    ಹಿಂತಿರುಗಿ error;
  }


  ಸ್ಥಿರ FT_Error
  raccess_guess_apple_generic( FT_Library  library,
                               FT_ಹರಿವು   ಹರಿವು,
                               ಅಕ್ಷರ       *ತಳ_file_name,
                               FT_Int32    magic,
                               FT_Long    *ಫಲಿತಾಂಶ_ಸ್ಥಾನಾಂತರ )
  {
    FT_Int32   magic_from_ಹರಿವು;
    FT_Error   error;
    FT_Int32   ಆವೃತ್ತಿ_ಸಂಖ್ಯೆ = 0;
    FT_UShort  n_ದ_entries;

    ಪೂರ್ಣಾಂಕ        i;
    FT_Int32   ನಮೂದು_id, ನಮೂದು_ಸ್ಥಾನಾಂತರ, ನಮೂದು_ಉದ್ದಳತೆ = 0;

    ಸ್ಥಿರಾಂಕ FT_Int32  resource_fork_ನಮೂದು_id = 0x2;

    FT_ಬಳಸದ( library );
    FT_ಬಳಸದ( ತಳ_file_name );
    FT_ಬಳಸದ( ಆವೃತ್ತಿ_ಸಂಖ್ಯೆ );
    FT_ಬಳಸದ( ನಮೂದು_ಉದ್ದಳತೆ   );


    ಆದರೆ ( FT_READ_LONG( magic_from_ಹರಿವು ) )
      ಹಿಂತಿರುಗಿ error;
    ಆದರೆ ( magic_from_ಹರಿವು != magic )
      ಹಿಂತಿರುಗಿ FT_THROW( Unknown_File_Format );

    ಆದರೆ ( FT_READ_LONG( ಆವೃತ್ತಿ_ಸಂಖ್ಯೆ ) )
      ಹಿಂತಿರುಗಿ error;

    /* filler */
    error = FT_ಹರಿವು_Skip( ಹರಿವು, 16 );
    ಆದರೆ ( error )
      ಹಿಂತಿರುಗಿ error;

    ಆದರೆ ( FT_READ_USHORT( n_ದ_entries ) )
      ಹಿಂತಿರುಗಿ error;
    ಆದರೆ ( n_ದ_entries == 0 )
      ಹಿಂತಿರುಗಿ FT_THROW( Unknown_File_Format );

    ಗಾಗಿ ( i = 0; i < n_ದ_entries; i++ )
    {
      ಆದರೆ ( FT_READ_LONG( ನಮೂದು_id ) )
        ಹಿಂತಿರುಗಿ error;
      ಆದರೆ ( ನಮೂದು_id == resource_fork_ನಮೂದು_id )
      {
        ಆದರೆ ( FT_READ_LONG( ನಮೂದು_ಸ್ಥಾನಾಂತರ ) ||
             FT_READ_LONG( ನಮೂದು_ಉದ್ದಳತೆ ) )
          ಮುಂದುವರಿ;
        *ಫಲಿತಾಂಶ_ಸ್ಥಾನಾಂತರ = ನಮೂದು_ಸ್ಥಾನಾಂತರ;

        ಹಿಂತಿರುಗಿ FT_Err_Ok;
      }
      ಇಲ್ಲದಿದ್ದರೆ
      {
        error = FT_ಹರಿವು_Skip( ಹರಿವು, 4 + 4 );    /* offset + length */
        ಆದರೆ ( error )
          ಹಿಂತಿರುಗಿ error;
      }
    }

    ಹಿಂತಿರುಗಿ FT_THROW( Unknown_File_Format );
  }


  ಸ್ಥಿರ FT_Error
  raccess_guess_linux_double_from_file_name( FT_Library  library,
                                             ಅಕ್ಷರ       *file_name,
                                             FT_Long    *ಫಲಿತಾಂಶ_ಸ್ಥಾನಾಂತರ )
  {
    FT_Open_ವಾದಗಳು  args2;
    FT_ಹರಿವು     stream2;
    ಅಕ್ಷರ*         nouse = ಖಾಲಿ;
    FT_Error      error;


    args2.flags    = FT_OPEN_PATHNAME;
    args2.pathname = file_name;
    error = FT_ಹರಿವು_New( library, &args2, &stream2 );
    ಆದರೆ ( error )
      ಹಿಂತಿರುಗಿ error;

    error = raccess_guess_apple_double( library, stream2, file_name,
                                        &nouse, ಫಲಿತಾಂಶ_ಸ್ಥಾನಾಂತರ );

    FT_ಹರಿವು_Free( stream2, 0 );

    ಹಿಂತಿರುಗಿ error;
  }


  ಸ್ಥಿರ ಅಕ್ಷರ*
  raccess_make_file_name( FT_ಸ್ಮರಣೆ    ಸ್ಮರಣೆ,
                          ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ  *original_name,
                          ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ  *insertion )
  {
    ಅಕ್ಷರ*        new_name = ಖಾಲಿ;
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*  tmp;
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*  slash;
    size_t       new_ಉದ್ದಳತೆ;
    FT_Error     error;


    new_ಉದ್ದಳತೆ = ft_strlen( original_name ) + ft_strlen( insertion );
    ಆದರೆ ( FT_QALLOC( new_name, new_ಉದ್ದಳತೆ + 1 ) )
      ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    tmp = ft_strrchr( original_name, '/' );
    ಆದರೆ ( tmp )
    {
      ft_strncpy( new_name,
                  original_name,
                  (size_t)( tmp - original_name + 1 ) );
      new_name[tmp - original_name + 1] = '\0';
      slash = tmp + 1;
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
      slash       = original_name;
      new_name[0] = '\0';
    }

    ft_strcat( new_name, insertion );
    ft_strcat( new_name, slash );

    ಹಿಂತಿರುಗಿ new_name;
  }


#ಇಲ್ಲದಿದ್ದರೆ   /* !FT_CONFIG_OPTION_GUESSING_EMBEDDED_RFORK */


  /**************************************************************************
   *                 Dummy function; just sets errors
   */

  FT_ತಳ_DEF( ಶೂನ್ಯ )
  FT_Raccess_Guess( FT_Library  library,
                    FT_ಹರಿವು   ಹರಿವು,
                    ಅಕ್ಷರ       *ತಳ_name,
                    ಅಕ್ಷರ      **new_ಹೆಸರುಗಳು,
                    FT_Long    *offsets,
                    FT_Error   *errors )
  {
    FT_Int  i;

    FT_ಬಳಸದ( library );
    FT_ಬಳಸದ( ಹರಿವು );
    FT_ಬಳಸದ( ತಳ_name );


    ಗಾಗಿ ( i = 0; i < FT_RACCESS_N_ನಿಯಮಗಳು; i++ )
    {
      new_ಹೆಸರುಗಳು[i] = ಖಾಲಿ;
      offsets[i]   = 0;
      errors[i]    = FT_ERR( Unimplemented_Feature );
    }
  }


#ಆದರೆ_ಅಂತ್ಯ  /* !FT_CONFIG_OPTION_GUESSING_EMBEDDED_RFORK */


/*ಕೊನೆ*/
