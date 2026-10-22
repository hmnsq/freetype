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


#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftdebug.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftstream.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftrfork.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "ftbase.h"

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ  FT_COMPONENT
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FT_COMPONENT  raccess


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

  FT_ਅਧਾਰ_DEF( FT_Error )
  FT_Raccess_Get_ਸਿਰਲੇਖ_Info( FT_Library  library,
                             FT_ਧਾਰਾ   ਧਾਰਾ,
                             FT_Long     rfork_ਵਿੱਥ,
                             FT_Long    *map_ਵਿੱਥ,
                             FT_Long    *rdata_pos )
  {
    FT_Error       error;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ  ਸਿਰ[16], head2[16];
    FT_Long        map_pos, map_len, rdata_len;
    ਪੂਰਨ            allzeros, allmatch, i;
    FT_Long        type_list;

    FT_ਅਣਵਰਤਿਆ( library );


    error = FT_ਧਾਰਾ_Seek( ਧਾਰਾ, (FT_ULong)rfork_ਵਿੱਥ );
    ਜੇ ( error )
      ਵਾਪਸ error;

    error = FT_ਧਾਰਾ_Read( ਧਾਰਾ, (FT_Byte*)ਸਿਰ, 16 );
    ਜੇ ( error )
      ਵਾਪਸ error;

    /* ensure positive values */
    ਜੇ ( ਸਿਰ[0]  >= 0x80 ||
         ਸਿਰ[4]  >= 0x80 ||
         ਸਿਰ[8]  >= 0x80 ||
         ਸਿਰ[12] >= 0x80 )
      ਵਾਪਸ FT_THROW( Unknown_File_Format );

    *rdata_pos = ( ਸਿਰ[ 0] << 24 ) |
                 ( ਸਿਰ[ 1] << 16 ) |
                 ( ਸਿਰ[ 2] <<  8 ) |
                   ਸਿਰ[ 3];
    map_pos    = ( ਸਿਰ[ 4] << 24 ) |
                 ( ਸਿਰ[ 5] << 16 ) |
                 ( ਸਿਰ[ 6] <<  8 ) |
                   ਸਿਰ[ 7];
    rdata_len  = ( ਸਿਰ[ 8] << 24 ) |
                 ( ਸਿਰ[ 9] << 16 ) |
                 ( ਸਿਰ[10] <<  8 ) |
                   ਸਿਰ[11];
    map_len    = ( ਸਿਰ[12] << 24 ) |
                 ( ਸਿਰ[13] << 16 ) |
                 ( ਸਿਰ[14] <<  8 ) |
                   ਸਿਰ[15];

    /* the map must not be empty */
    ਜੇ ( !map_pos )
      ਵਾਪਸ FT_THROW( Unknown_File_Format );

    /* check whether rdata and map overlap */
    ਜੇ ( *rdata_pos < map_pos )
    {
      ਜੇ ( *rdata_pos > map_pos - rdata_len )
        ਵਾਪਸ FT_THROW( Unknown_File_Format );
    }
    ਨਹੀਂਤਾਂ
    {
      ਜੇ ( map_pos > *rdata_pos - map_len )
        ਵਾਪਸ FT_THROW( Unknown_File_Format );
    }

    /* check whether end of rdata or map exceeds stream size */
    ਜੇ ( FT_LONG_MAX - rdata_len < *rdata_pos                               ||
         FT_LONG_MAX - map_len < map_pos                                    ||

         FT_LONG_MAX - ( *rdata_pos + rdata_len ) < rfork_ਵਿੱਥ            ||
         FT_LONG_MAX - ( map_pos + map_len ) < rfork_ਵਿੱਥ                 ||

         (FT_ULong)( rfork_ਵਿੱਥ + *rdata_pos + rdata_len ) > ਧਾਰਾ->ਮਾਪ ||
         (FT_ULong)( rfork_ਵਿੱਥ + map_pos + map_len ) > ਧਾਰਾ->ਮਾਪ      )
      ਵਾਪਸ FT_THROW( Unknown_File_Format );

    *rdata_pos += rfork_ਵਿੱਥ;
    map_pos    += rfork_ਵਿੱਥ;

    error = FT_ਧਾਰਾ_Seek( ਧਾਰਾ, (FT_ULong)map_pos );
    ਜੇ ( error )
      ਵਾਪਸ error;

    head2[15] = (FT_Byte)( ਸਿਰ[15] + 1 );       /* make it be different */

    error = FT_ਧਾਰਾ_Read( ਧਾਰਾ, (FT_Byte*)head2, 16 );
    ਜੇ ( error )
      ਵਾਪਸ error;

    allzeros = 1;
    allmatch = 1;
    ਲਈ ( i = 0; i < 16; i++ )
    {
      ਜੇ ( head2[i] != 0 )
        allzeros = 0;
      ਜੇ ( head2[i] != ਸਿਰ[i] )
        allmatch = 0;
    }
    ਜੇ ( !allzeros && !allmatch )
      ਵਾਪਸ FT_THROW( Unknown_File_Format );

    /* If we have reached this point then it is probably a mac resource */
    /* file.  Now, does it contain any interesting resources?           */

    (ਖਾਲੀ)FT_ਧਾਰਾ_SKIP( 4        /* skip handle to next resource map */
                          + 2      /* skip file resource number */
                          + 2 );   /* skip attributes */

    ਜੇ ( FT_READ_SHORT( type_list ) )
      ਵਾਪਸ error;
    ਜੇ ( type_list < 0 )
      ਵਾਪਸ FT_THROW( Unknown_File_Format );

    error = FT_ਧਾਰਾ_Seek( ਧਾਰਾ, (FT_ULong)( map_pos + type_list ) );
    ਜੇ ( error )
      ਵਾਪਸ error;

    *map_ਵਿੱਥ = map_pos + type_list;
    ਵਾਪਸ FT_Err_Ok;
  }


  FT_COMPARE_DEF( ਪੂਰਨ )
  ft_raccess_ਛਾਂਟੋ_ref_by_id( ਅਚਲ ਖਾਲੀ*  a,
                             ਅਚਲ ਖਾਲੀ*  b )
  {
    ਵਾਪਸ  ( (FT_RFork_Ref*)a )->res_id - ( (FT_RFork_Ref*)b )->res_id;
  }


  FT_ਅਧਾਰ_DEF( FT_Error )
  FT_Raccess_Get_ਅੰਕੜੇ_Offsets( FT_Library  library,
                              FT_ਧਾਰਾ   ਧਾਰਾ,
                              FT_Long     map_ਵਿੱਥ,
                              FT_Long     rdata_pos,
                              FT_Long     ਟੈਗ,
                              FT_Bool     ਛਾਂਟੋ_by_res_id,
                              FT_Long   **offsets,
                              FT_Long    *ਗਿਣਤੀ_2 )
  {
    FT_Error      error;
    ਪੂਰਨ           i, j, cnt, subcnt;
    FT_Long       ਟੈਗ_ਅੰਦਰੂਨੀ, rpos;
    FT_ਸਮ੍ਰਿਤੀ     ਸਮ੍ਰਿਤੀ = library->ਸਮ੍ਰਿਤੀ;
    FT_Long       ਆਰਜ਼ੀ;
    FT_Long       *offsets_ਅੰਦਰੂਨੀ = ਨਿਰਾ;
    FT_RFork_Ref  *ref = ਨਿਰਾ;


    FT_TRACE3(( "\n" ));
    error = FT_ਧਾਰਾ_Seek( ਧਾਰਾ, (FT_ULong)map_ਵਿੱਥ );
    ਜੇ ( error )
      ਵਾਪਸ error;

    ਜੇ ( FT_READ_SHORT( cnt ) )
      ਵਾਪਸ error;
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
    ਜੇ ( cnt > 4079 )
      ਵਾਪਸ FT_THROW( Invalid_ਸਾਰਣੀ );

    ਲਈ ( i = 0; i < cnt; i++ )
    {
      ਜੇ ( FT_READ_LONG( ਟੈਗ_ਅੰਦਰੂਨੀ ) ||
           FT_READ_SHORT( subcnt )      ||
           FT_READ_SHORT( rpos )        )
        ਵਾਪਸ error;

      FT_TRACE2(( "Resource tags: %c%c%c%c\n",
                  (ਅੱਖਰ)( 0xFF & ( ਟੈਗ_ਅੰਦਰੂਨੀ >> 24 ) ),
                  (ਅੱਖਰ)( 0xFF & ( ਟੈਗ_ਅੰਦਰੂਨੀ >> 16 ) ),
                  (ਅੱਖਰ)( 0xFF & ( ਟੈਗ_ਅੰਦਰੂਨੀ >>  8 ) ),
                  (ਅੱਖਰ)( 0xFF & ( ਟੈਗ_ਅੰਦਰੂਨੀ >>  0 ) ) ));
      FT_TRACE3(( "             : subcount=%d, suboffset=0x%04lx\n",
                  subcnt, rpos ));

      ਜੇ ( ਟੈਗ_ਅੰਦਰੂਨੀ == ਟੈਗ )
      {
        *ਗਿਣਤੀ_2 = subcnt + 1;
        rpos  += map_ਵਿੱਥ;

        /* a zero count might be valid in the resource specification, */
        /* however, it is completely useless to us                    */
        ਜੇ ( *ਗਿਣਤੀ_2 < 1 || *ਗਿਣਤੀ_2 > 2727 )
          ਵਾਪਸ FT_THROW( Invalid_ਸਾਰਣੀ );

        error = FT_ਧਾਰਾ_Seek( ਧਾਰਾ, (FT_ULong)rpos );
        ਜੇ ( error )
          ਵਾਪਸ error;

        ਜੇ ( FT_QNEW_ARRAY( ref, *ਗਿਣਤੀ_2 ) )
          ਵਾਪਸ error;

        ਲਈ ( j = 0; j < *ਗਿਣਤੀ_2; j++ )
        {
          ਜੇ ( FT_READ_SHORT( ref[j].res_id ) )
            ਜਾਓ Exit;
          ਜੇ ( FT_ਧਾਰਾ_SKIP( 2 ) )  /* resource name offset */
            ਜਾਓ Exit;
          ਜੇ ( FT_READ_LONG( ਆਰਜ਼ੀ ) ) /* attributes (8bit), offset (24bit) */
            ਜਾਓ Exit;
          ਜੇ ( FT_ਧਾਰਾ_SKIP( 4 ) )  /* mbz */
            ਜਾਓ Exit;

          /*
           * According to Inside Macintosh: More Macintosh Toolbox,
           * "Resource IDs" (1-46), there are some reserved IDs.
           * However, FreeType2 is not a font synthesizer, no need
           * to check the acceptable resource ID or its attributes.
           */
          ref[j].ਵਿੱਥ = ਆਰਜ਼ੀ & 0xFFFFFFL;

          FT_TRACE3(( "             [%d]:"
                      " resource_id=0x%04x, offset=0x%08lx\n",
                      j, (FT_UShort)ref[j].res_id, ref[j].ਵਿੱਥ ));
        }

        ਜੇ ( ਛਾਂਟੋ_by_res_id )
        {
          ft_qsort( ref,
                    (size_t)*ਗਿਣਤੀ_2,
                    ਆਕਾਰ ( FT_RFork_Ref ),
                    ft_raccess_ਛਾਂਟੋ_ref_by_id );

          FT_TRACE3(( "             -- sort resources by their ids --\n" ));

          ਲਈ ( j = 0; j < *ਗਿਣਤੀ_2; j++ )
            FT_TRACE3(( "             [%d]:"
                        " resource_id=0x%04x, offset=0x%08lx\n",
                        j, ref[j].res_id, ref[j].ਵਿੱਥ ));
        }

        ਜੇ ( FT_QNEW_ARRAY( offsets_ਅੰਦਰੂਨੀ, *ਗਿਣਤੀ_2 ) )
          ਜਾਓ Exit;

        /* XXX: duplicated reference ID,
         *      gap between reference IDs are acceptable?
         *      further investigation on Apple implementation is needed.
         */
        ਲਈ ( j = 0; j < *ਗਿਣਤੀ_2; j++ )
          offsets_ਅੰਦਰੂਨੀ[j] = rdata_pos + ref[j].ਵਿੱਥ;

        *offsets = offsets_ਅੰਦਰੂਨੀ;
        error    = FT_Err_Ok;

      Exit:
        FT_FREE( ref );
        ਵਾਪਸ error;
      }
    }

    ਵਾਪਸ FT_THROW( Cannot_Open_Resource );
  }


#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ FT_CONFIG_OPTION_GUESSING_EMBEDDED_RFORK

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

  ਸਥਿਰ FT_Error
  raccess_guess_apple_double( FT_Library  library,
                              FT_ਧਾਰਾ   ਧਾਰਾ,
                              ਅੱਖਰ       *ਅਧਾਰ_file_name,
                              ਅੱਖਰ      **ਨਤੀਜਾ_file_name,
                              FT_Long    *ਨਤੀਜਾ_ਵਿੱਥ );

  ਸਥਿਰ FT_Error
  raccess_guess_apple_single( FT_Library  library,
                              FT_ਧਾਰਾ   ਧਾਰਾ,
                              ਅੱਖਰ       *ਅਧਾਰ_file_name,
                              ਅੱਖਰ      **ਨਤੀਜਾ_file_name,
                              FT_Long    *ਨਤੀਜਾ_ਵਿੱਥ );

  ਸਥਿਰ FT_Error
  raccess_guess_darwin_ufs_export( FT_Library  library,
                                   FT_ਧਾਰਾ   ਧਾਰਾ,
                                   ਅੱਖਰ       *ਅਧਾਰ_file_name,
                                   ਅੱਖਰ      **ਨਤੀਜਾ_file_name,
                                   FT_Long    *ਨਤੀਜਾ_ਵਿੱਥ );

  ਸਥਿਰ FT_Error
  raccess_guess_darwin_newvfs( FT_Library  library,
                               FT_ਧਾਰਾ   ਧਾਰਾ,
                               ਅੱਖਰ       *ਅਧਾਰ_file_name,
                               ਅੱਖਰ      **ਨਤੀਜਾ_file_name,
                               FT_Long    *ਨਤੀਜਾ_ਵਿੱਥ );

  ਸਥਿਰ FT_Error
  raccess_guess_darwin_hfsplus( FT_Library  library,
                                FT_ਧਾਰਾ   ਧਾਰਾ,
                                ਅੱਖਰ       *ਅਧਾਰ_file_name,
                                ਅੱਖਰ      **ਨਤੀਜਾ_file_name,
                                FT_Long    *ਨਤੀਜਾ_ਵਿੱਥ );

  ਸਥਿਰ FT_Error
  raccess_guess_vfat( FT_Library  library,
                      FT_ਧਾਰਾ   ਧਾਰਾ,
                      ਅੱਖਰ       *ਅਧਾਰ_file_name,
                      ਅੱਖਰ      **ਨਤੀਜਾ_file_name,
                      FT_Long    *ਨਤੀਜਾ_ਵਿੱਥ );

  ਸਥਿਰ FT_Error
  raccess_guess_linux_cap( FT_Library  library,
                           FT_ਧਾਰਾ   ਧਾਰਾ,
                           ਅੱਖਰ       *ਅਧਾਰ_file_name,
                           ਅੱਖਰ      **ਨਤੀਜਾ_file_name,
                           FT_Long    *ਨਤੀਜਾ_ਵਿੱਥ );

  ਸਥਿਰ FT_Error
  raccess_guess_linux_double( FT_Library  library,
                              FT_ਧਾਰਾ   ਧਾਰਾ,
                              ਅੱਖਰ       *ਅਧਾਰ_file_name,
                              ਅੱਖਰ      **ਨਤੀਜਾ_file_name,
                              FT_Long    *ਨਤੀਜਾ_ਵਿੱਥ );

  ਸਥਿਰ FT_Error
  raccess_guess_linux_netatalk( FT_Library  library,
                                FT_ਧਾਰਾ   ਧਾਰਾ,
                                ਅੱਖਰ       *ਅਧਾਰ_file_name,
                                ਅੱਖਰ      **ਨਤੀਜਾ_file_name,
                                FT_Long    *ਨਤੀਜਾ_ਵਿੱਥ );


  CONST_FT_RFORK_ਨਿਯਮ_ARRAY_BEGIN( ft_raccess_guess_ਸਾਰਣੀ,
                                                      ft_raccess_guess_rec )
  CONST_FT_RFORK_ਨਿਯਮ_ARRAY_ਇੰਦਰਾਜ( apple_double,      apple_double )
  CONST_FT_RFORK_ਨਿਯਮ_ARRAY_ਇੰਦਰਾਜ( apple_single,      apple_single )
  CONST_FT_RFORK_ਨਿਯਮ_ARRAY_ਇੰਦਰਾਜ( darwin_ufs_export, darwin_ufs_export )
  CONST_FT_RFORK_ਨਿਯਮ_ARRAY_ਇੰਦਰਾਜ( darwin_newvfs,     darwin_newvfs )
  CONST_FT_RFORK_ਨਿਯਮ_ARRAY_ਇੰਦਰਾਜ( darwin_hfsplus,    darwin_hfsplus )
  CONST_FT_RFORK_ਨਿਯਮ_ARRAY_ਇੰਦਰਾਜ( vfat,              vfat )
  CONST_FT_RFORK_ਨਿਯਮ_ARRAY_ਇੰਦਰਾਜ( linux_cap,         linux_cap )
  CONST_FT_RFORK_ਨਿਯਮ_ARRAY_ਇੰਦਰਾਜ( linux_double,      linux_double )
  CONST_FT_RFORK_ਨਿਯਮ_ARRAY_ਇੰਦਰਾਜ( linux_netatalk,    linux_netatalk )
  CONST_FT_RFORK_ਨਿਯਮ_ARRAY_ਸਿਰਾ


  /*************************************************************************/
  /****                                                                 ****/
  /****                       Helper functions                          ****/
  /****                                                                 ****/
  /*************************************************************************/

  ਸਥਿਰ FT_Error
  raccess_guess_apple_generic( FT_Library  library,
                               FT_ਧਾਰਾ   ਧਾਰਾ,
                               ਅੱਖਰ       *ਅਧਾਰ_file_name,
                               FT_Int32    magic,
                               FT_Long    *ਨਤੀਜਾ_ਵਿੱਥ );

  ਸਥਿਰ FT_Error
  raccess_guess_linux_double_from_file_name( FT_Library  library,
                                             ਅੱਖਰ*       file_name,
                                             FT_Long    *ਨਤੀਜਾ_ਵਿੱਥ );

  ਸਥਿਰ ਅੱਖਰ *
  raccess_make_file_name( FT_ਸਮ੍ਰਿਤੀ    ਸਮ੍ਰਿਤੀ,
                          ਅਚਲ ਅੱਖਰ  *original_name,
                          ਅਚਲ ਅੱਖਰ  *insertion );

  FT_ਅਧਾਰ_DEF( ਖਾਲੀ )
  FT_Raccess_Guess( FT_Library  library,
                    FT_ਧਾਰਾ   ਧਾਰਾ,
                    ਅੱਖਰ*       ਅਧਾਰ_name,
                    ਅੱਖਰ      **new_ਨਾਂ,
                    FT_Long    *offsets,
                    FT_Error   *errors )
  {
    FT_Int  i;


    ਲਈ ( i = 0; i < FT_RACCESS_N_ਨਿਯਮਾਂ; i++ )
    {
      new_ਨਾਂ[i] = ਨਿਰਾ;
      ਜੇ ( ਨਿਰਾ != ਧਾਰਾ )
        errors[i] = FT_ਧਾਰਾ_Seek( ਧਾਰਾ, 0 );
      ਨਹੀਂਤਾਂ
        errors[i] = FT_Err_Ok;

      ਜੇ ( errors[i] )
        ਜਾਰੀ;

      errors[i] = ft_raccess_guess_ਸਾਰਣੀ[i].func( library,
                                                  ਧਾਰਾ, ਅਧਾਰ_name,
                                                  &(new_ਨਾਂ[i]),
                                                  &(offsets[i]) );
    }

    ਵਾਪਸ;
  }


#ਜੇ defined( FT_CONFIG_OPTION_MAC_FONTS ) && !defined( FT_MACINTOSH )
  ਸਥਿਰ FT_RFork_ਨਿਯਮ
  raccess_get_ਨਿਯਮ_type_from_ਨਿਯਮ_ਸੂਚਕਾਂਕ( FT_Library  library,
                                         FT_UInt     ਨਿਯਮ_ਸੂਚਕਾਂਕ )
  {
    FT_ਅਣਵਰਤਿਆ( library );

    ਜੇ ( ਨਿਯਮ_ਸੂਚਕਾਂਕ >= FT_RACCESS_N_ਨਿਯਮਾਂ )
      ਵਾਪਸ FT_RFork_ਨਿਯਮ_invalid;

    ਵਾਪਸ ft_raccess_guess_ਸਾਰਣੀ[ਨਿਯਮ_ਸੂਚਕਾਂਕ].type;
  }


  /*
   * For this function, refer ftbase.h.
   */
  FT_LOCAL_DEF( FT_Bool )
  ft_raccess_ਨਿਯਮ_by_darwin_vfs( FT_Library  library,
                                 FT_UInt     ਨਿਯਮ_ਸੂਚਕਾਂਕ )
  {
    ਚੋਣ( raccess_get_ਨਿਯਮ_type_from_ਨਿਯਮ_ਸੂਚਕਾਂਕ( library, ਨਿਯਮ_ਸੂਚਕਾਂਕ ) )
    {
      ਹਾਲਤ FT_RFork_ਨਿਯਮ_darwin_newvfs:
      ਹਾਲਤ FT_RFork_ਨਿਯਮ_darwin_hfsplus:
        ਵਾਪਸ TRUE;

      ਮੂਲ:
        ਵਾਪਸ FALSE;
    }
  }
#ਜੇ_ਅੰਤ


  ਸਥਿਰ FT_Error
  raccess_guess_apple_double( FT_Library  library,
                              FT_ਧਾਰਾ   ਧਾਰਾ,
                              ਅੱਖਰ       *ਅਧਾਰ_file_name,
                              ਅੱਖਰ      **ਨਤੀਜਾ_file_name,
                              FT_Long    *ਨਤੀਜਾ_ਵਿੱਥ )
  {
    FT_Int32  magic = ( 0x00 << 24 ) |
                      ( 0x05 << 16 ) |
                      ( 0x16 <<  8 ) |
                        0x07;


    *ਨਤੀਜਾ_file_name = ਨਿਰਾ;
    ਜੇ ( ਨਿਰਾ == ਧਾਰਾ )
      ਵਾਪਸ FT_THROW( Cannot_Open_ਧਾਰਾ );

    ਵਾਪਸ raccess_guess_apple_generic( library, ਧਾਰਾ, ਅਧਾਰ_file_name,
                                        magic, ਨਤੀਜਾ_ਵਿੱਥ );
  }


  ਸਥਿਰ FT_Error
  raccess_guess_apple_single( FT_Library  library,
                              FT_ਧਾਰਾ   ਧਾਰਾ,
                              ਅੱਖਰ       *ਅਧਾਰ_file_name,
                              ਅੱਖਰ      **ਨਤੀਜਾ_file_name,
                              FT_Long    *ਨਤੀਜਾ_ਵਿੱਥ )
  {
    FT_Int32  magic = ( 0x00 << 24 ) |
                      ( 0x05 << 16 ) |
                      ( 0x16 <<  8 ) |
                        0x00;


    *ਨਤੀਜਾ_file_name = ਨਿਰਾ;
    ਜੇ ( ਨਿਰਾ == ਧਾਰਾ )
      ਵਾਪਸ FT_THROW( Cannot_Open_ਧਾਰਾ );

    ਵਾਪਸ raccess_guess_apple_generic( library, ਧਾਰਾ, ਅਧਾਰ_file_name,
                                        magic, ਨਤੀਜਾ_ਵਿੱਥ );
  }


  ਸਥਿਰ FT_Error
  raccess_guess_darwin_ufs_export( FT_Library  library,
                                   FT_ਧਾਰਾ   ਧਾਰਾ,
                                   ਅੱਖਰ       *ਅਧਾਰ_file_name,
                                   ਅੱਖਰ      **ਨਤੀਜਾ_file_name,
                                   FT_Long    *ਨਤੀਜਾ_ਵਿੱਥ )
  {
    ਅੱਖਰ*      newpath;
    FT_Error   error;
    FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ;

    FT_ਅਣਵਰਤਿਆ( ਧਾਰਾ );


    ਸਮ੍ਰਿਤੀ  = library->ਸਮ੍ਰਿਤੀ;
    newpath = raccess_make_file_name( ਸਮ੍ਰਿਤੀ, ਅਧਾਰ_file_name, "._" );
    ਜੇ ( !newpath )
      ਵਾਪਸ FT_THROW( ਬਾਹਰਲਾ_ਦਾ_ਸਮ੍ਰਿਤੀ );

    error = raccess_guess_linux_double_from_file_name( library, newpath,
                                                       ਨਤੀਜਾ_ਵਿੱਥ );
    ਜੇ ( !error )
      *ਨਤੀਜਾ_file_name = newpath;
    ਨਹੀਂਤਾਂ
      FT_FREE( newpath );

    ਵਾਪਸ error;
  }


  ਸਥਿਰ FT_Error
  raccess_guess_darwin_hfsplus( FT_Library  library,
                                FT_ਧਾਰਾ   ਧਾਰਾ,
                                ਅੱਖਰ       *ਅਧਾਰ_file_name,
                                ਅੱਖਰ      **ਨਤੀਜਾ_file_name,
                                FT_Long    *ਨਤੀਜਾ_ਵਿੱਥ )
  {
    /*
      Only meaningful on systems with hfs+ drivers (or Macs).
     */
    FT_Error   error;
    ਅੱਖਰ*      newpath = ਨਿਰਾ;
    FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ;
    FT_Long    ਅਧਾਰ_file_len = (FT_Long)ft_strlen( ਅਧਾਰ_file_name );

    FT_ਅਣਵਰਤਿਆ( ਧਾਰਾ );


    ਸਮ੍ਰਿਤੀ = library->ਸਮ੍ਰਿਤੀ;

    ਜੇ ( ਅਧਾਰ_file_len + 6 > FT_INT_MAX )
      ਵਾਪਸ FT_THROW( Array_Too_Large );

    ਜੇ ( FT_QALLOC( newpath, ਅਧਾਰ_file_len + 6 ) )
      ਵਾਪਸ error;

    FT_MEM_COPY( newpath, ਅਧਾਰ_file_name, ਅਧਾਰ_file_len );
    FT_MEM_COPY( newpath + ਅਧਾਰ_file_len, "/rsrc", 6 );

    *ਨਤੀਜਾ_file_name = newpath;
    *ਨਤੀਜਾ_ਵਿੱਥ    = 0;

    ਵਾਪਸ FT_Err_Ok;
  }


  ਸਥਿਰ FT_Error
  raccess_guess_darwin_newvfs( FT_Library  library,
                               FT_ਧਾਰਾ   ਧਾਰਾ,
                               ਅੱਖਰ       *ਅਧਾਰ_file_name,
                               ਅੱਖਰ      **ਨਤੀਜਾ_file_name,
                               FT_Long    *ਨਤੀਜਾ_ਵਿੱਥ )
  {
    /*
      Only meaningful on systems with Mac OS X (> 10.1).
     */
    FT_Error   error;
    ਅੱਖਰ*      newpath = ਨਿਰਾ;
    FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ;
    FT_Long    ਅਧਾਰ_file_len = (FT_Long)ft_strlen( ਅਧਾਰ_file_name );

    FT_ਅਣਵਰਤਿਆ( ਧਾਰਾ );


    ਸਮ੍ਰਿਤੀ = library->ਸਮ੍ਰਿਤੀ;

    ਜੇ ( ਅਧਾਰ_file_len + 18 > FT_INT_MAX )
      ਵਾਪਸ FT_THROW( Array_Too_Large );

    ਜੇ ( FT_QALLOC( newpath, ਅਧਾਰ_file_len + 18 ) )
      ਵਾਪਸ error;

    FT_MEM_COPY( newpath, ਅਧਾਰ_file_name, ਅਧਾਰ_file_len );
    FT_MEM_COPY( newpath + ਅਧਾਰ_file_len, "/..namedfork/rsrc", 18 );

    *ਨਤੀਜਾ_file_name = newpath;
    *ਨਤੀਜਾ_ਵਿੱਥ    = 0;

    ਵਾਪਸ FT_Err_Ok;
  }


  ਸਥਿਰ FT_Error
  raccess_guess_vfat( FT_Library  library,
                      FT_ਧਾਰਾ   ਧਾਰਾ,
                      ਅੱਖਰ       *ਅਧਾਰ_file_name,
                      ਅੱਖਰ      **ਨਤੀਜਾ_file_name,
                      FT_Long    *ਨਤੀਜਾ_ਵਿੱਥ )
  {
    ਅੱਖਰ*      newpath;
    FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ;

    FT_ਅਣਵਰਤਿਆ( ਧਾਰਾ );


    ਸਮ੍ਰਿਤੀ = library->ਸਮ੍ਰਿਤੀ;

    newpath = raccess_make_file_name( ਸਮ੍ਰਿਤੀ, ਅਧਾਰ_file_name,
                                      "resource.frk/" );
    ਜੇ ( !newpath )
      ਵਾਪਸ FT_THROW( ਬਾਹਰਲਾ_ਦਾ_ਸਮ੍ਰਿਤੀ );

    *ਨਤੀਜਾ_file_name = newpath;
    *ਨਤੀਜਾ_ਵਿੱਥ    = 0;

    ਵਾਪਸ FT_Err_Ok;
  }


  ਸਥਿਰ FT_Error
  raccess_guess_linux_cap( FT_Library  library,
                           FT_ਧਾਰਾ   ਧਾਰਾ,
                           ਅੱਖਰ       *ਅਧਾਰ_file_name,
                           ਅੱਖਰ      **ਨਤੀਜਾ_file_name,
                           FT_Long    *ਨਤੀਜਾ_ਵਿੱਥ )
  {
    ਅੱਖਰ*      newpath;
    FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ;

    FT_ਅਣਵਰਤਿਆ( ਧਾਰਾ );


    ਸਮ੍ਰਿਤੀ = library->ਸਮ੍ਰਿਤੀ;

    newpath = raccess_make_file_name( ਸਮ੍ਰਿਤੀ, ਅਧਾਰ_file_name, ".resource/" );
    ਜੇ ( !newpath )
      ਵਾਪਸ FT_THROW( ਬਾਹਰਲਾ_ਦਾ_ਸਮ੍ਰਿਤੀ );

    *ਨਤੀਜਾ_file_name = newpath;
    *ਨਤੀਜਾ_ਵਿੱਥ    = 0;

    ਵਾਪਸ FT_Err_Ok;
  }


  ਸਥਿਰ FT_Error
  raccess_guess_linux_double( FT_Library  library,
                              FT_ਧਾਰਾ   ਧਾਰਾ,
                              ਅੱਖਰ       *ਅਧਾਰ_file_name,
                              ਅੱਖਰ      **ਨਤੀਜਾ_file_name,
                              FT_Long    *ਨਤੀਜਾ_ਵਿੱਥ )
  {
    ਅੱਖਰ*      newpath;
    FT_Error   error;
    FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ;

    FT_ਅਣਵਰਤਿਆ( ਧਾਰਾ );


    ਸਮ੍ਰਿਤੀ = library->ਸਮ੍ਰਿਤੀ;

    newpath = raccess_make_file_name( ਸਮ੍ਰਿਤੀ, ਅਧਾਰ_file_name, "%" );
    ਜੇ ( !newpath )
      ਵਾਪਸ FT_THROW( ਬਾਹਰਲਾ_ਦਾ_ਸਮ੍ਰਿਤੀ );

    error = raccess_guess_linux_double_from_file_name( library, newpath,
                                                       ਨਤੀਜਾ_ਵਿੱਥ );
    ਜੇ ( !error )
      *ਨਤੀਜਾ_file_name = newpath;
    ਨਹੀਂਤਾਂ
      FT_FREE( newpath );

    ਵਾਪਸ error;
  }


  ਸਥਿਰ FT_Error
  raccess_guess_linux_netatalk( FT_Library  library,
                                FT_ਧਾਰਾ   ਧਾਰਾ,
                                ਅੱਖਰ       *ਅਧਾਰ_file_name,
                                ਅੱਖਰ      **ਨਤੀਜਾ_file_name,
                                FT_Long    *ਨਤੀਜਾ_ਵਿੱਥ )
  {
    ਅੱਖਰ*      newpath;
    FT_Error   error;
    FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ;

    FT_ਅਣਵਰਤਿਆ( ਧਾਰਾ );


    ਸਮ੍ਰਿਤੀ = library->ਸਮ੍ਰਿਤੀ;

    newpath = raccess_make_file_name( ਸਮ੍ਰਿਤੀ, ਅਧਾਰ_file_name,
                                      ".AppleDouble/" );
    ਜੇ ( !newpath )
      ਵਾਪਸ FT_THROW( ਬਾਹਰਲਾ_ਦਾ_ਸਮ੍ਰਿਤੀ );

    error = raccess_guess_linux_double_from_file_name( library, newpath,
                                                       ਨਤੀਜਾ_ਵਿੱਥ );
    ਜੇ ( !error )
      *ਨਤੀਜਾ_file_name = newpath;
    ਨਹੀਂਤਾਂ
      FT_FREE( newpath );

    ਵਾਪਸ error;
  }


  ਸਥਿਰ FT_Error
  raccess_guess_apple_generic( FT_Library  library,
                               FT_ਧਾਰਾ   ਧਾਰਾ,
                               ਅੱਖਰ       *ਅਧਾਰ_file_name,
                               FT_Int32    magic,
                               FT_Long    *ਨਤੀਜਾ_ਵਿੱਥ )
  {
    FT_Int32   magic_from_ਧਾਰਾ;
    FT_Error   error;
    FT_Int32   ਸੰਸਕਰਣ_ਸੰਖਿਆ = 0;
    FT_UShort  n_ਦਾ_entries;

    ਪੂਰਨ        i;
    FT_Int32   ਇੰਦਰਾਜ_id, ਇੰਦਰਾਜ_ਵਿੱਥ, ਇੰਦਰਾਜ_ਲੰਮਾਈ = 0;

    ਅਚਲ FT_Int32  resource_fork_ਇੰਦਰਾਜ_id = 0x2;

    FT_ਅਣਵਰਤਿਆ( library );
    FT_ਅਣਵਰਤਿਆ( ਅਧਾਰ_file_name );
    FT_ਅਣਵਰਤਿਆ( ਸੰਸਕਰਣ_ਸੰਖਿਆ );
    FT_ਅਣਵਰਤਿਆ( ਇੰਦਰਾਜ_ਲੰਮਾਈ   );


    ਜੇ ( FT_READ_LONG( magic_from_ਧਾਰਾ ) )
      ਵਾਪਸ error;
    ਜੇ ( magic_from_ਧਾਰਾ != magic )
      ਵਾਪਸ FT_THROW( Unknown_File_Format );

    ਜੇ ( FT_READ_LONG( ਸੰਸਕਰਣ_ਸੰਖਿਆ ) )
      ਵਾਪਸ error;

    /* filler */
    error = FT_ਧਾਰਾ_Skip( ਧਾਰਾ, 16 );
    ਜੇ ( error )
      ਵਾਪਸ error;

    ਜੇ ( FT_READ_USHORT( n_ਦਾ_entries ) )
      ਵਾਪਸ error;
    ਜੇ ( n_ਦਾ_entries == 0 )
      ਵਾਪਸ FT_THROW( Unknown_File_Format );

    ਲਈ ( i = 0; i < n_ਦਾ_entries; i++ )
    {
      ਜੇ ( FT_READ_LONG( ਇੰਦਰਾਜ_id ) )
        ਵਾਪਸ error;
      ਜੇ ( ਇੰਦਰਾਜ_id == resource_fork_ਇੰਦਰਾਜ_id )
      {
        ਜੇ ( FT_READ_LONG( ਇੰਦਰਾਜ_ਵਿੱਥ ) ||
             FT_READ_LONG( ਇੰਦਰਾਜ_ਲੰਮਾਈ ) )
          ਜਾਰੀ;
        *ਨਤੀਜਾ_ਵਿੱਥ = ਇੰਦਰਾਜ_ਵਿੱਥ;

        ਵਾਪਸ FT_Err_Ok;
      }
      ਨਹੀਂਤਾਂ
      {
        error = FT_ਧਾਰਾ_Skip( ਧਾਰਾ, 4 + 4 );    /* offset + length */
        ਜੇ ( error )
          ਵਾਪਸ error;
      }
    }

    ਵਾਪਸ FT_THROW( Unknown_File_Format );
  }


  ਸਥਿਰ FT_Error
  raccess_guess_linux_double_from_file_name( FT_Library  library,
                                             ਅੱਖਰ       *file_name,
                                             FT_Long    *ਨਤੀਜਾ_ਵਿੱਥ )
  {
    FT_Open_ਦਲੀਲਾਂ  args2;
    FT_ਧਾਰਾ     stream2;
    ਅੱਖਰ*         nouse = ਨਿਰਾ;
    FT_Error      error;


    args2.flags    = FT_OPEN_PATHNAME;
    args2.pathname = file_name;
    error = FT_ਧਾਰਾ_New( library, &args2, &stream2 );
    ਜੇ ( error )
      ਵਾਪਸ error;

    error = raccess_guess_apple_double( library, stream2, file_name,
                                        &nouse, ਨਤੀਜਾ_ਵਿੱਥ );

    FT_ਧਾਰਾ_Free( stream2, 0 );

    ਵਾਪਸ error;
  }


  ਸਥਿਰ ਅੱਖਰ*
  raccess_make_file_name( FT_ਸਮ੍ਰਿਤੀ    ਸਮ੍ਰਿਤੀ,
                          ਅਚਲ ਅੱਖਰ  *original_name,
                          ਅਚਲ ਅੱਖਰ  *insertion )
  {
    ਅੱਖਰ*        new_name = ਨਿਰਾ;
    ਅਚਲ ਅੱਖਰ*  tmp;
    ਅਚਲ ਅੱਖਰ*  slash;
    size_t       new_ਲੰਮਾਈ;
    FT_Error     error;


    new_ਲੰਮਾਈ = ft_strlen( original_name ) + ft_strlen( insertion );
    ਜੇ ( FT_QALLOC( new_name, new_ਲੰਮਾਈ + 1 ) )
      ਵਾਪਸ ਨਿਰਾ;

    tmp = ft_strrchr( original_name, '/' );
    ਜੇ ( tmp )
    {
      ft_strncpy( new_name,
                  original_name,
                  (size_t)( tmp - original_name + 1 ) );
      new_name[tmp - original_name + 1] = '\0';
      slash = tmp + 1;
    }
    ਨਹੀਂਤਾਂ
    {
      slash       = original_name;
      new_name[0] = '\0';
    }

    ft_strcat( new_name, insertion );
    ft_strcat( new_name, slash );

    ਵਾਪਸ new_name;
  }


#ਨਹੀਂਤਾਂ   /* !FT_CONFIG_OPTION_GUESSING_EMBEDDED_RFORK */


  /**************************************************************************
   *                 Dummy function; just sets errors
   */

  FT_ਅਧਾਰ_DEF( ਖਾਲੀ )
  FT_Raccess_Guess( FT_Library  library,
                    FT_ਧਾਰਾ   ਧਾਰਾ,
                    ਅੱਖਰ       *ਅਧਾਰ_name,
                    ਅੱਖਰ      **new_ਨਾਂ,
                    FT_Long    *offsets,
                    FT_Error   *errors )
  {
    FT_Int  i;

    FT_ਅਣਵਰਤਿਆ( library );
    FT_ਅਣਵਰਤਿਆ( ਧਾਰਾ );
    FT_ਅਣਵਰਤਿਆ( ਅਧਾਰ_name );


    ਲਈ ( i = 0; i < FT_RACCESS_N_ਨਿਯਮਾਂ; i++ )
    {
      new_ਨਾਂ[i] = ਨਿਰਾ;
      offsets[i]   = 0;
      errors[i]    = FT_ERR( Unimplemented_Feature );
    }
  }


#ਜੇ_ਅੰਤ  /* !FT_CONFIG_OPTION_GUESSING_EMBEDDED_RFORK */


/*ਸਿਰਾ*/
