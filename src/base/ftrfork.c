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


#شامل <freetype/internal/ftdebug.h>
#شامل <freetype/internal/ftstream.h>
#شامل <freetype/internal/ftrfork.h>

#شامل "ftbase.h"

#undef  FT_COMPONENT
#define FT_COMPONENT  raccess


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

  FT_BASE_DEF( FT_Error )
  FT_Raccess_Get_HeaderInfo( FT_Library  library,
                             FT_Stream   stream,
                             FT_Long     rfork_offset,
                             FT_Long    *map_offset,
                             FT_Long    *rdata_pos )
  {
    FT_Error       error;
    unsigned نویسه  head[16], head2[16];
    FT_Long        map_pos, map_len, rdata_len;
    صحیح            allzeros, allmatch, i;
    FT_Long        type_list;

    FT_UNUSED( library );


    error = FT_Stream_Seek( stream, (FT_ULong)rfork_offset );
    اگر ( error )
      بازگشت error;

    error = FT_Stream_Read( stream, (FT_Byte*)head, 16 );
    اگر ( error )
      بازگشت error;

    /* ensure positive values */
    اگر ( head[0]  >= 0x80 ||
         head[4]  >= 0x80 ||
         head[8]  >= 0x80 ||
         head[12] >= 0x80 )
      بازگشت FT_THROW( Unknown_File_Format );

    *rdata_pos = ( head[ 0] << 24 ) |
                 ( head[ 1] << 16 ) |
                 ( head[ 2] <<  8 ) |
                   head[ 3];
    map_pos    = ( head[ 4] << 24 ) |
                 ( head[ 5] << 16 ) |
                 ( head[ 6] <<  8 ) |
                   head[ 7];
    rdata_len  = ( head[ 8] << 24 ) |
                 ( head[ 9] << 16 ) |
                 ( head[10] <<  8 ) |
                   head[11];
    map_len    = ( head[12] << 24 ) |
                 ( head[13] << 16 ) |
                 ( head[14] <<  8 ) |
                   head[15];

    /* the map must not be empty */
    اگر ( !map_pos )
      بازگشت FT_THROW( Unknown_File_Format );

    /* check whether rdata and map overlap */
    اگر ( *rdata_pos < map_pos )
    {
      اگر ( *rdata_pos > map_pos - rdata_len )
        بازگشت FT_THROW( Unknown_File_Format );
    }
    وگرنه
    {
      اگر ( map_pos > *rdata_pos - map_len )
        بازگشت FT_THROW( Unknown_File_Format );
    }

    /* check whether end of rdata or map exceeds stream size */
    اگر ( FT_LONG_MAX - rdata_len < *rdata_pos                               ||
         FT_LONG_MAX - map_len < map_pos                                    ||

         FT_LONG_MAX - ( *rdata_pos + rdata_len ) < rfork_offset            ||
         FT_LONG_MAX - ( map_pos + map_len ) < rfork_offset                 ||

         (FT_ULong)( rfork_offset + *rdata_pos + rdata_len ) > stream->size ||
         (FT_ULong)( rfork_offset + map_pos + map_len ) > stream->size      )
      بازگشت FT_THROW( Unknown_File_Format );

    *rdata_pos += rfork_offset;
    map_pos    += rfork_offset;

    error = FT_Stream_Seek( stream, (FT_ULong)map_pos );
    اگر ( error )
      بازگشت error;

    head2[15] = (FT_Byte)( head[15] + 1 );       /* make it be different */

    error = FT_Stream_Read( stream, (FT_Byte*)head2, 16 );
    اگر ( error )
      بازگشت error;

    allzeros = 1;
    allmatch = 1;
    برای ( i = 0; i < 16; i++ )
    {
      اگر ( head2[i] != 0 )
        allzeros = 0;
      اگر ( head2[i] != head[i] )
        allmatch = 0;
    }
    اگر ( !allzeros && !allmatch )
      بازگشت FT_THROW( Unknown_File_Format );

    /* If we have reached this point then it is probably a mac resource */
    /* file.  Now, does it contain any interesting resources?           */

    (تهی)FT_STREAM_SKIP( 4        /* skip handle to next resource map */
                          + 2      /* skip file resource number */
                          + 2 );   /* skip attributes */

    اگر ( FT_READ_SHORT( type_list ) )
      بازگشت error;
    اگر ( type_list < 0 )
      بازگشت FT_THROW( Unknown_File_Format );

    error = FT_Stream_Seek( stream, (FT_ULong)( map_pos + type_list ) );
    اگر ( error )
      بازگشت error;

    *map_offset = map_pos + type_list;
    بازگشت FT_Err_Ok;
  }


  FT_COMPARE_DEF( صحیح )
  ft_raccess_sort_ref_by_id( ثابت تهی*  a,
                             ثابت تهی*  b )
  {
    بازگشت  ( (FT_RFork_Ref*)a )->res_id - ( (FT_RFork_Ref*)b )->res_id;
  }


  FT_BASE_DEF( FT_Error )
  FT_Raccess_Get_DataOffsets( FT_Library  library,
                              FT_Stream   stream,
                              FT_Long     map_offset,
                              FT_Long     rdata_pos,
                              FT_Long     tag,
                              FT_Bool     sort_by_res_id,
                              FT_Long   **offsets,
                              FT_Long    *شمار )
  {
    FT_Error      error;
    صحیح           i, j, cnt, subcnt;
    FT_Long       tag_internal, rpos;
    FT_Memory     memory = library->memory;
    FT_Long       temp;
    FT_Long       *offsets_internal = پوچ;
    FT_RFork_Ref  *ref = پوچ;


    FT_TRACE3(( "\n" ));
    error = FT_Stream_Seek( stream, (FT_ULong)map_offset );
    اگر ( error )
      بازگشت error;

    اگر ( FT_READ_SHORT( cnt ) )
      بازگشت error;
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
    اگر ( cnt > 4079 )
      بازگشت FT_THROW( Invalid_Table );

    برای ( i = 0; i < cnt; i++ )
    {
      اگر ( FT_READ_LONG( tag_internal ) ||
           FT_READ_SHORT( subcnt )      ||
           FT_READ_SHORT( rpos )        )
        بازگشت error;

      FT_TRACE2(( "Resource tags: %c%c%c%c\n",
                  (نویسه)( 0xFF & ( tag_internal >> 24 ) ),
                  (نویسه)( 0xFF & ( tag_internal >> 16 ) ),
                  (نویسه)( 0xFF & ( tag_internal >>  8 ) ),
                  (نویسه)( 0xFF & ( tag_internal >>  0 ) ) ));
      FT_TRACE3(( "             : subcount=%d, suboffset=0x%04lx\n",
                  subcnt, rpos ));

      اگر ( tag_internal == tag )
      {
        *شمار = subcnt + 1;
        rpos  += map_offset;

        /* a zero count might be valid in the resource specification, */
        /* however, it is completely useless to us                    */
        اگر ( *شمار < 1 || *شمار > 2727 )
          بازگشت FT_THROW( Invalid_Table );

        error = FT_Stream_Seek( stream, (FT_ULong)rpos );
        اگر ( error )
          بازگشت error;

        اگر ( FT_QNEW_ARRAY( ref, *شمار ) )
          بازگشت error;

        برای ( j = 0; j < *شمار; j++ )
        {
          اگر ( FT_READ_SHORT( ref[j].res_id ) )
            برو Exit;
          اگر ( FT_STREAM_SKIP( 2 ) )  /* resource name offset */
            برو Exit;
          اگر ( FT_READ_LONG( temp ) ) /* attributes (8bit), offset (24bit) */
            برو Exit;
          اگر ( FT_STREAM_SKIP( 4 ) )  /* mbz */
            برو Exit;

          /*
           * According to Inside Macintosh: More Macintosh Toolbox,
           * "Resource IDs" (1-46), there are some reserved IDs.
           * However, FreeType2 is not a font synthesizer, no need
           * to check the acceptable resource ID or its attributes.
           */
          ref[j].offset = temp & 0xFFFFFFL;

          FT_TRACE3(( "             [%d]:"
                      " resource_id=0x%04x, offset=0x%08lx\n",
                      j, (FT_UShort)ref[j].res_id, ref[j].offset ));
        }

        اگر ( sort_by_res_id )
        {
          ft_qsort( ref,
                    (size_t)*شمار,
                    اندازه ( FT_RFork_Ref ),
                    ft_raccess_sort_ref_by_id );

          FT_TRACE3(( "             -- sort resources by their ids --\n" ));

          برای ( j = 0; j < *شمار; j++ )
            FT_TRACE3(( "             [%d]:"
                        " resource_id=0x%04x, offset=0x%08lx\n",
                        j, ref[j].res_id, ref[j].offset ));
        }

        اگر ( FT_QNEW_ARRAY( offsets_internal, *شمار ) )
          برو Exit;

        /* XXX: duplicated reference ID,
         *      gap between reference IDs are acceptable?
         *      further investigation on Apple implementation is needed.
         */
        برای ( j = 0; j < *شمار; j++ )
          offsets_internal[j] = rdata_pos + ref[j].offset;

        *offsets = offsets_internal;
        error    = FT_Err_Ok;

      Exit:
        FT_FREE( ref );
        بازگشت error;
      }
    }

    بازگشت FT_THROW( Cannot_Open_Resource );
  }


#ifdef FT_CONFIG_OPTION_GUESSING_EMBEDDED_RFORK

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

  ایستا FT_Error
  raccess_guess_apple_double( FT_Library  library,
                              FT_Stream   stream,
                              نویسه       *base_file_name,
                              نویسه      **result_file_name,
                              FT_Long    *result_offset );

  ایستا FT_Error
  raccess_guess_apple_single( FT_Library  library,
                              FT_Stream   stream,
                              نویسه       *base_file_name,
                              نویسه      **result_file_name,
                              FT_Long    *result_offset );

  ایستا FT_Error
  raccess_guess_darwin_ufs_export( FT_Library  library,
                                   FT_Stream   stream,
                                   نویسه       *base_file_name,
                                   نویسه      **result_file_name,
                                   FT_Long    *result_offset );

  ایستا FT_Error
  raccess_guess_darwin_newvfs( FT_Library  library,
                               FT_Stream   stream,
                               نویسه       *base_file_name,
                               نویسه      **result_file_name,
                               FT_Long    *result_offset );

  ایستا FT_Error
  raccess_guess_darwin_hfsplus( FT_Library  library,
                                FT_Stream   stream,
                                نویسه       *base_file_name,
                                نویسه      **result_file_name,
                                FT_Long    *result_offset );

  ایستا FT_Error
  raccess_guess_vfat( FT_Library  library,
                      FT_Stream   stream,
                      نویسه       *base_file_name,
                      نویسه      **result_file_name,
                      FT_Long    *result_offset );

  ایستا FT_Error
  raccess_guess_linux_cap( FT_Library  library,
                           FT_Stream   stream,
                           نویسه       *base_file_name,
                           نویسه      **result_file_name,
                           FT_Long    *result_offset );

  ایستا FT_Error
  raccess_guess_linux_double( FT_Library  library,
                              FT_Stream   stream,
                              نویسه       *base_file_name,
                              نویسه      **result_file_name,
                              FT_Long    *result_offset );

  ایستا FT_Error
  raccess_guess_linux_netatalk( FT_Library  library,
                                FT_Stream   stream,
                                نویسه       *base_file_name,
                                نویسه      **result_file_name,
                                FT_Long    *result_offset );


  CONST_FT_RFORK_RULE_ARRAY_BEGIN( ft_raccess_guess_table,
                                                      ft_raccess_guess_rec )
  CONST_FT_RFORK_RULE_ARRAY_ENTRY( apple_double,      apple_double )
  CONST_FT_RFORK_RULE_ARRAY_ENTRY( apple_single,      apple_single )
  CONST_FT_RFORK_RULE_ARRAY_ENTRY( darwin_ufs_export, darwin_ufs_export )
  CONST_FT_RFORK_RULE_ARRAY_ENTRY( darwin_newvfs,     darwin_newvfs )
  CONST_FT_RFORK_RULE_ARRAY_ENTRY( darwin_hfsplus,    darwin_hfsplus )
  CONST_FT_RFORK_RULE_ARRAY_ENTRY( vfat,              vfat )
  CONST_FT_RFORK_RULE_ARRAY_ENTRY( linux_cap,         linux_cap )
  CONST_FT_RFORK_RULE_ARRAY_ENTRY( linux_double,      linux_double )
  CONST_FT_RFORK_RULE_ARRAY_ENTRY( linux_netatalk,    linux_netatalk )
  CONST_FT_RFORK_RULE_ARRAY_END


  /*************************************************************************/
  /****                                                                 ****/
  /****                       Helper functions                          ****/
  /****                                                                 ****/
  /*************************************************************************/

  ایستا FT_Error
  raccess_guess_apple_generic( FT_Library  library,
                               FT_Stream   stream,
                               نویسه       *base_file_name,
                               FT_Int32    magic,
                               FT_Long    *result_offset );

  ایستا FT_Error
  raccess_guess_linux_double_from_file_name( FT_Library  library,
                                             نویسه*       file_name,
                                             FT_Long    *result_offset );

  ایستا نویسه *
  raccess_make_file_name( FT_Memory    memory,
                          ثابت نویسه  *original_name,
                          ثابت نویسه  *insertion );

  FT_BASE_DEF( تهی )
  FT_Raccess_Guess( FT_Library  library,
                    FT_Stream   stream,
                    نویسه*       base_name,
                    نویسه      **new_names,
                    FT_Long    *offsets,
                    FT_Error   *errors )
  {
    FT_Int  i;


    برای ( i = 0; i < FT_RACCESS_N_RULES; i++ )
    {
      new_names[i] = پوچ;
      اگر ( پوچ != stream )
        errors[i] = FT_Stream_Seek( stream, 0 );
      وگرنه
        errors[i] = FT_Err_Ok;

      اگر ( errors[i] )
        ادامه;

      errors[i] = ft_raccess_guess_table[i].func( library,
                                                  stream, base_name,
                                                  &(new_names[i]),
                                                  &(offsets[i]) );
    }

    بازگشت;
  }


#اگر defined( FT_CONFIG_OPTION_MAC_FONTS ) && !defined( FT_MACINTOSH )
  ایستا FT_RFork_Rule
  raccess_get_rule_type_from_rule_index( FT_Library  library,
                                         FT_UInt     rule_index )
  {
    FT_UNUSED( library );

    اگر ( rule_index >= FT_RACCESS_N_RULES )
      بازگشت FT_RFork_Rule_invalid;

    بازگشت ft_raccess_guess_table[rule_index].type;
  }


  /*
   * For this function, refer ftbase.h.
   */
  FT_LOCAL_DEF( FT_Bool )
  ft_raccess_rule_by_darwin_vfs( FT_Library  library,
                                 FT_UInt     rule_index )
  {
    انتخاب( raccess_get_rule_type_from_rule_index( library, rule_index ) )
    {
      حالت FT_RFork_Rule_darwin_newvfs:
      حالت FT_RFork_Rule_darwin_hfsplus:
        بازگشت TRUE;

      پیشفرض:
        بازگشت FALSE;
    }
  }
#endif


  ایستا FT_Error
  raccess_guess_apple_double( FT_Library  library,
                              FT_Stream   stream,
                              نویسه       *base_file_name,
                              نویسه      **result_file_name,
                              FT_Long    *result_offset )
  {
    FT_Int32  magic = ( 0x00 << 24 ) |
                      ( 0x05 << 16 ) |
                      ( 0x16 <<  8 ) |
                        0x07;


    *result_file_name = پوچ;
    اگر ( پوچ == stream )
      بازگشت FT_THROW( Cannot_Open_Stream );

    بازگشت raccess_guess_apple_generic( library, stream, base_file_name,
                                        magic, result_offset );
  }


  ایستا FT_Error
  raccess_guess_apple_single( FT_Library  library,
                              FT_Stream   stream,
                              نویسه       *base_file_name,
                              نویسه      **result_file_name,
                              FT_Long    *result_offset )
  {
    FT_Int32  magic = ( 0x00 << 24 ) |
                      ( 0x05 << 16 ) |
                      ( 0x16 <<  8 ) |
                        0x00;


    *result_file_name = پوچ;
    اگر ( پوچ == stream )
      بازگشت FT_THROW( Cannot_Open_Stream );

    بازگشت raccess_guess_apple_generic( library, stream, base_file_name,
                                        magic, result_offset );
  }


  ایستا FT_Error
  raccess_guess_darwin_ufs_export( FT_Library  library,
                                   FT_Stream   stream,
                                   نویسه       *base_file_name,
                                   نویسه      **result_file_name,
                                   FT_Long    *result_offset )
  {
    نویسه*      newpath;
    FT_Error   error;
    FT_Memory  memory;

    FT_UNUSED( stream );


    memory  = library->memory;
    newpath = raccess_make_file_name( memory, base_file_name, "._" );
    اگر ( !newpath )
      بازگشت FT_THROW( Out_Of_Memory );

    error = raccess_guess_linux_double_from_file_name( library, newpath,
                                                       result_offset );
    اگر ( !error )
      *result_file_name = newpath;
    وگرنه
      FT_FREE( newpath );

    بازگشت error;
  }


  ایستا FT_Error
  raccess_guess_darwin_hfsplus( FT_Library  library,
                                FT_Stream   stream,
                                نویسه       *base_file_name,
                                نویسه      **result_file_name,
                                FT_Long    *result_offset )
  {
    /*
      Only meaningful on systems with hfs+ drivers (or Macs).
     */
    FT_Error   error;
    نویسه*      newpath = پوچ;
    FT_Memory  memory;
    FT_Long    base_file_len = (FT_Long)ft_strlen( base_file_name );

    FT_UNUSED( stream );


    memory = library->memory;

    اگر ( base_file_len + 6 > FT_INT_MAX )
      بازگشت FT_THROW( Array_Too_Large );

    اگر ( FT_QALLOC( newpath, base_file_len + 6 ) )
      بازگشت error;

    FT_MEM_COPY( newpath, base_file_name, base_file_len );
    FT_MEM_COPY( newpath + base_file_len, "/rsrc", 6 );

    *result_file_name = newpath;
    *result_offset    = 0;

    بازگشت FT_Err_Ok;
  }


  ایستا FT_Error
  raccess_guess_darwin_newvfs( FT_Library  library,
                               FT_Stream   stream,
                               نویسه       *base_file_name,
                               نویسه      **result_file_name,
                               FT_Long    *result_offset )
  {
    /*
      Only meaningful on systems with Mac OS X (> 10.1).
     */
    FT_Error   error;
    نویسه*      newpath = پوچ;
    FT_Memory  memory;
    FT_Long    base_file_len = (FT_Long)ft_strlen( base_file_name );

    FT_UNUSED( stream );


    memory = library->memory;

    اگر ( base_file_len + 18 > FT_INT_MAX )
      بازگشت FT_THROW( Array_Too_Large );

    اگر ( FT_QALLOC( newpath, base_file_len + 18 ) )
      بازگشت error;

    FT_MEM_COPY( newpath, base_file_name, base_file_len );
    FT_MEM_COPY( newpath + base_file_len, "/..namedfork/rsrc", 18 );

    *result_file_name = newpath;
    *result_offset    = 0;

    بازگشت FT_Err_Ok;
  }


  ایستا FT_Error
  raccess_guess_vfat( FT_Library  library,
                      FT_Stream   stream,
                      نویسه       *base_file_name,
                      نویسه      **result_file_name,
                      FT_Long    *result_offset )
  {
    نویسه*      newpath;
    FT_Memory  memory;

    FT_UNUSED( stream );


    memory = library->memory;

    newpath = raccess_make_file_name( memory, base_file_name,
                                      "resource.frk/" );
    اگر ( !newpath )
      بازگشت FT_THROW( Out_Of_Memory );

    *result_file_name = newpath;
    *result_offset    = 0;

    بازگشت FT_Err_Ok;
  }


  ایستا FT_Error
  raccess_guess_linux_cap( FT_Library  library,
                           FT_Stream   stream,
                           نویسه       *base_file_name,
                           نویسه      **result_file_name,
                           FT_Long    *result_offset )
  {
    نویسه*      newpath;
    FT_Memory  memory;

    FT_UNUSED( stream );


    memory = library->memory;

    newpath = raccess_make_file_name( memory, base_file_name, ".resource/" );
    اگر ( !newpath )
      بازگشت FT_THROW( Out_Of_Memory );

    *result_file_name = newpath;
    *result_offset    = 0;

    بازگشت FT_Err_Ok;
  }


  ایستا FT_Error
  raccess_guess_linux_double( FT_Library  library,
                              FT_Stream   stream,
                              نویسه       *base_file_name,
                              نویسه      **result_file_name,
                              FT_Long    *result_offset )
  {
    نویسه*      newpath;
    FT_Error   error;
    FT_Memory  memory;

    FT_UNUSED( stream );


    memory = library->memory;

    newpath = raccess_make_file_name( memory, base_file_name, "%" );
    اگر ( !newpath )
      بازگشت FT_THROW( Out_Of_Memory );

    error = raccess_guess_linux_double_from_file_name( library, newpath,
                                                       result_offset );
    اگر ( !error )
      *result_file_name = newpath;
    وگرنه
      FT_FREE( newpath );

    بازگشت error;
  }


  ایستا FT_Error
  raccess_guess_linux_netatalk( FT_Library  library,
                                FT_Stream   stream,
                                نویسه       *base_file_name,
                                نویسه      **result_file_name,
                                FT_Long    *result_offset )
  {
    نویسه*      newpath;
    FT_Error   error;
    FT_Memory  memory;

    FT_UNUSED( stream );


    memory = library->memory;

    newpath = raccess_make_file_name( memory, base_file_name,
                                      ".AppleDouble/" );
    اگر ( !newpath )
      بازگشت FT_THROW( Out_Of_Memory );

    error = raccess_guess_linux_double_from_file_name( library, newpath,
                                                       result_offset );
    اگر ( !error )
      *result_file_name = newpath;
    وگرنه
      FT_FREE( newpath );

    بازگشت error;
  }


  ایستا FT_Error
  raccess_guess_apple_generic( FT_Library  library,
                               FT_Stream   stream,
                               نویسه       *base_file_name,
                               FT_Int32    magic,
                               FT_Long    *result_offset )
  {
    FT_Int32   magic_from_stream;
    FT_Error   error;
    FT_Int32   version_عدد = 0;
    FT_UShort  n_of_entries;

    صحیح        i;
    FT_Int32   entry_id, entry_offset, entry_length = 0;

    ثابت FT_Int32  resource_fork_entry_id = 0x2;

    FT_UNUSED( library );
    FT_UNUSED( base_file_name );
    FT_UNUSED( version_عدد );
    FT_UNUSED( entry_length   );


    اگر ( FT_READ_LONG( magic_from_stream ) )
      بازگشت error;
    اگر ( magic_from_stream != magic )
      بازگشت FT_THROW( Unknown_File_Format );

    اگر ( FT_READ_LONG( version_عدد ) )
      بازگشت error;

    /* filler */
    error = FT_Stream_Skip( stream, 16 );
    اگر ( error )
      بازگشت error;

    اگر ( FT_READ_USHORT( n_of_entries ) )
      بازگشت error;
    اگر ( n_of_entries == 0 )
      بازگشت FT_THROW( Unknown_File_Format );

    برای ( i = 0; i < n_of_entries; i++ )
    {
      اگر ( FT_READ_LONG( entry_id ) )
        بازگشت error;
      اگر ( entry_id == resource_fork_entry_id )
      {
        اگر ( FT_READ_LONG( entry_offset ) ||
             FT_READ_LONG( entry_length ) )
          ادامه;
        *result_offset = entry_offset;

        بازگشت FT_Err_Ok;
      }
      وگرنه
      {
        error = FT_Stream_Skip( stream, 4 + 4 );    /* offset + length */
        اگر ( error )
          بازگشت error;
      }
    }

    بازگشت FT_THROW( Unknown_File_Format );
  }


  ایستا FT_Error
  raccess_guess_linux_double_from_file_name( FT_Library  library,
                                             نویسه       *file_name,
                                             FT_Long    *result_offset )
  {
    FT_Open_Args  args2;
    FT_Stream     stream2;
    نویسه*         nouse = پوچ;
    FT_Error      error;


    args2.flags    = FT_OPEN_PATHNAME;
    args2.pathname = file_name;
    error = FT_Stream_New( library, &args2, &stream2 );
    اگر ( error )
      بازگشت error;

    error = raccess_guess_apple_double( library, stream2, file_name,
                                        &nouse, result_offset );

    FT_Stream_Free( stream2, 0 );

    بازگشت error;
  }


  ایستا نویسه*
  raccess_make_file_name( FT_Memory    memory,
                          ثابت نویسه  *original_name,
                          ثابت نویسه  *insertion )
  {
    نویسه*        new_name = پوچ;
    ثابت نویسه*  tmp;
    ثابت نویسه*  slash;
    size_t       new_length;
    FT_Error     error;


    new_length = ft_strlen( original_name ) + ft_strlen( insertion );
    اگر ( FT_QALLOC( new_name, new_length + 1 ) )
      بازگشت پوچ;

    tmp = ft_strrchr( original_name, '/' );
    اگر ( tmp )
    {
      ft_strncpy( new_name,
                  original_name,
                  (size_t)( tmp - original_name + 1 ) );
      new_name[tmp - original_name + 1] = '\0';
      slash = tmp + 1;
    }
    وگرنه
    {
      slash       = original_name;
      new_name[0] = '\0';
    }

    ft_strcat( new_name, insertion );
    ft_strcat( new_name, slash );

    بازگشت new_name;
  }


#وگرنه   /* !FT_CONFIG_OPTION_GUESSING_EMBEDDED_RFORK */


  /**************************************************************************
   *                 Dummy function; just sets errors
   */

  FT_BASE_DEF( تهی )
  FT_Raccess_Guess( FT_Library  library,
                    FT_Stream   stream,
                    نویسه       *base_name,
                    نویسه      **new_names,
                    FT_Long    *offsets,
                    FT_Error   *errors )
  {
    FT_Int  i;

    FT_UNUSED( library );
    FT_UNUSED( stream );
    FT_UNUSED( base_name );


    برای ( i = 0; i < FT_RACCESS_N_RULES; i++ )
    {
      new_names[i] = پوچ;
      offsets[i]   = 0;
      errors[i]    = FT_ERR( Unimplemented_Feature );
    }
  }


#endif  /* !FT_CONFIG_OPTION_GUESSING_EMBEDDED_RFORK */


/* END */
