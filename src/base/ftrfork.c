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


#অন্তর্ভাব <freetype/internal/ftdebug.h>
#অন্তর্ভাব <freetype/internal/ftstream.h>
#অন্তর্ভাব <freetype/internal/ftrfork.h>

#অন্তর্ভাব "ftbase.h"

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
    unsigned অক্ষর  শীর্ষ_2[16], head2[16];
    FT_Long        map_pos, map_len, rdata_len;
    পূর্ণসংখ্যা            allzeros, allmatch, i;
    FT_Long        type_list;

    FT_UNUSED( library );


    error = FT_Stream_Seek( stream, (FT_ULong)rfork_offset );
    যদি ( error )
      ফেরত error;

    error = FT_Stream_Read( stream, (FT_Byte*)শীর্ষ_2, 16 );
    যদি ( error )
      ফেরত error;

    /* ensure positive values */
    যদি ( শীর্ষ_2[0]  >= 0x80 ||
         শীর্ষ_2[4]  >= 0x80 ||
         শীর্ষ_2[8]  >= 0x80 ||
         শীর্ষ_2[12] >= 0x80 )
      ফেরত FT_THROW( Unknown_File_Format );

    *rdata_pos = ( শীর্ষ_2[ 0] << 24 ) |
                 ( শীর্ষ_2[ 1] << 16 ) |
                 ( শীর্ষ_2[ 2] <<  8 ) |
                   শীর্ষ_2[ 3];
    map_pos    = ( শীর্ষ_2[ 4] << 24 ) |
                 ( শীর্ষ_2[ 5] << 16 ) |
                 ( শীর্ষ_2[ 6] <<  8 ) |
                   শীর্ষ_2[ 7];
    rdata_len  = ( শীর্ষ_2[ 8] << 24 ) |
                 ( শীর্ষ_2[ 9] << 16 ) |
                 ( শীর্ষ_2[10] <<  8 ) |
                   শীর্ষ_2[11];
    map_len    = ( শীর্ষ_2[12] << 24 ) |
                 ( শীর্ষ_2[13] << 16 ) |
                 ( শীর্ষ_2[14] <<  8 ) |
                   শীর্ষ_2[15];

    /* the map must not be empty */
    যদি ( !map_pos )
      ফেরত FT_THROW( Unknown_File_Format );

    /* check whether rdata and map overlap */
    যদি ( *rdata_pos < map_pos )
    {
      যদি ( *rdata_pos > map_pos - rdata_len )
        ফেরত FT_THROW( Unknown_File_Format );
    }
    অন্যথা
    {
      যদি ( map_pos > *rdata_pos - map_len )
        ফেরত FT_THROW( Unknown_File_Format );
    }

    /* check whether end of rdata or map exceeds stream size */
    যদি ( FT_LONG_MAX - rdata_len < *rdata_pos                               ||
         FT_LONG_MAX - map_len < map_pos                                    ||

         FT_LONG_MAX - ( *rdata_pos + rdata_len ) < rfork_offset            ||
         FT_LONG_MAX - ( map_pos + map_len ) < rfork_offset                 ||

         (FT_ULong)( rfork_offset + *rdata_pos + rdata_len ) > stream->আকার_2 ||
         (FT_ULong)( rfork_offset + map_pos + map_len ) > stream->আকার_2      )
      ফেরত FT_THROW( Unknown_File_Format );

    *rdata_pos += rfork_offset;
    map_pos    += rfork_offset;

    error = FT_Stream_Seek( stream, (FT_ULong)map_pos );
    যদি ( error )
      ফেরত error;

    head2[15] = (FT_Byte)( শীর্ষ_2[15] + 1 );       /* make it be different */

    error = FT_Stream_Read( stream, (FT_Byte*)head2, 16 );
    যদি ( error )
      ফেরত error;

    allzeros = 1;
    allmatch = 1;
    জন্য ( i = 0; i < 16; i++ )
    {
      যদি ( head2[i] != 0 )
        allzeros = 0;
      যদি ( head2[i] != শীর্ষ_2[i] )
        allmatch = 0;
    }
    যদি ( !allzeros && !allmatch )
      ফেরত FT_THROW( Unknown_File_Format );

    /* If we have reached this point then it is probably a mac resource */
    /* file.  Now, does it contain any interesting resources?           */

    (শূন্য)FT_STREAM_SKIP( 4        /* skip handle to next resource map */
                          + 2      /* skip file resource number */
                          + 2 );   /* skip attributes */

    যদি ( FT_READ_SHORT( type_list ) )
      ফেরত error;
    যদি ( type_list < 0 )
      ফেরত FT_THROW( Unknown_File_Format );

    error = FT_Stream_Seek( stream, (FT_ULong)( map_pos + type_list ) );
    যদি ( error )
      ফেরত error;

    *map_offset = map_pos + type_list;
    ফেরত FT_Err_Ok;
  }


  FT_COMPARE_DEF( পূর্ণসংখ্যা )
  ft_raccess_sort_ref_by_id( ধ্রুবক শূন্য*  a,
                             ধ্রুবক শূন্য*  b )
  {
    ফেরত  ( (FT_RFork_Ref*)a )->res_id - ( (FT_RFork_Ref*)b )->res_id;
  }


  FT_BASE_DEF( FT_Error )
  FT_Raccess_Get_তথ্য_Offsets( FT_Library  library,
                              FT_Stream   stream,
                              FT_Long     map_offset,
                              FT_Long     rdata_pos,
                              FT_Long     tag,
                              FT_Bool     sort_by_res_id,
                              FT_Long   **offsets,
                              FT_Long    *গুনতি )
  {
    FT_Error      error;
    পূর্ণসংখ্যা           i, j, cnt, subcnt;
    FT_Long       tag_internal, rpos;
    FT_Memory     memory = library->memory;
    FT_Long       অস্থায়ী;
    FT_Long       *offsets_internal = ফাঁকা;
    FT_RFork_Ref  *ref = ফাঁকা;


    FT_TRACE3(( "\n" ));
    error = FT_Stream_Seek( stream, (FT_ULong)map_offset );
    যদি ( error )
      ফেরত error;

    যদি ( FT_READ_SHORT( cnt ) )
      ফেরত error;
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
    যদি ( cnt > 4079 )
      ফেরত FT_THROW( Invalid_Table );

    জন্য ( i = 0; i < cnt; i++ )
    {
      যদি ( FT_READ_LONG( tag_internal ) ||
           FT_READ_SHORT( subcnt )      ||
           FT_READ_SHORT( rpos )        )
        ফেরত error;

      FT_TRACE2(( "Resource tags: %c%c%c%c\n",
                  (অক্ষর)( 0xFF & ( tag_internal >> 24 ) ),
                  (অক্ষর)( 0xFF & ( tag_internal >> 16 ) ),
                  (অক্ষর)( 0xFF & ( tag_internal >>  8 ) ),
                  (অক্ষর)( 0xFF & ( tag_internal >>  0 ) ) ));
      FT_TRACE3(( "             : subcount=%d, suboffset=0x%04lx\n",
                  subcnt, rpos ));

      যদি ( tag_internal == tag )
      {
        *গুনতি = subcnt + 1;
        rpos  += map_offset;

        /* a zero count might be valid in the resource specification, */
        /* however, it is completely useless to us                    */
        যদি ( *গুনতি < 1 || *গুনতি > 2727 )
          ফেরত FT_THROW( Invalid_Table );

        error = FT_Stream_Seek( stream, (FT_ULong)rpos );
        যদি ( error )
          ফেরত error;

        যদি ( FT_QNEW_ARRAY( ref, *গুনতি ) )
          ফেরত error;

        জন্য ( j = 0; j < *গুনতি; j++ )
        {
          যদি ( FT_READ_SHORT( ref[j].res_id ) )
            যাও Exit;
          যদি ( FT_STREAM_SKIP( 2 ) )  /* resource name offset */
            যাও Exit;
          যদি ( FT_READ_LONG( অস্থায়ী ) ) /* attributes (8bit), offset (24bit) */
            যাও Exit;
          যদি ( FT_STREAM_SKIP( 4 ) )  /* mbz */
            যাও Exit;

          /*
           * According to Inside Macintosh: More Macintosh Toolbox,
           * "Resource IDs" (1-46), there are some reserved IDs.
           * However, FreeType2 is not a font synthesizer, no need
           * to check the acceptable resource ID or its attributes.
           */
          ref[j].offset = অস্থায়ী & 0xFFFFFFL;

          FT_TRACE3(( "             [%d]:"
                      " resource_id=0x%04x, offset=0x%08lx\n",
                      j, (FT_UShort)ref[j].res_id, ref[j].offset ));
        }

        যদি ( sort_by_res_id )
        {
          ft_qsort( ref,
                    (size_t)*গুনতি,
                    আকার ( FT_RFork_Ref ),
                    ft_raccess_sort_ref_by_id );

          FT_TRACE3(( "             -- sort resources by their ids --\n" ));

          জন্য ( j = 0; j < *গুনতি; j++ )
            FT_TRACE3(( "             [%d]:"
                        " resource_id=0x%04x, offset=0x%08lx\n",
                        j, ref[j].res_id, ref[j].offset ));
        }

        যদি ( FT_QNEW_ARRAY( offsets_internal, *গুনতি ) )
          যাও Exit;

        /* XXX: duplicated reference ID,
         *      gap between reference IDs are acceptable?
         *      further investigation on Apple implementation is needed.
         */
        জন্য ( j = 0; j < *গুনতি; j++ )
          offsets_internal[j] = rdata_pos + ref[j].offset;

        *offsets = offsets_internal;
        error    = FT_Err_Ok;

      Exit:
        FT_FREE( ref );
        ফেরত error;
      }
    }

    ফেরত FT_THROW( Cannot_Open_Resource );
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

  স্থির FT_Error
  raccess_guess_apple_double( FT_Library  library,
                              FT_Stream   stream,
                              অক্ষর       *base_file_name,
                              অক্ষর      **ফলাফল_file_name,
                              FT_Long    *ফলাফল_offset );

  স্থির FT_Error
  raccess_guess_apple_single( FT_Library  library,
                              FT_Stream   stream,
                              অক্ষর       *base_file_name,
                              অক্ষর      **ফলাফল_file_name,
                              FT_Long    *ফলাফল_offset );

  স্থির FT_Error
  raccess_guess_darwin_ufs_export( FT_Library  library,
                                   FT_Stream   stream,
                                   অক্ষর       *base_file_name,
                                   অক্ষর      **ফলাফল_file_name,
                                   FT_Long    *ফলাফল_offset );

  স্থির FT_Error
  raccess_guess_darwin_newvfs( FT_Library  library,
                               FT_Stream   stream,
                               অক্ষর       *base_file_name,
                               অক্ষর      **ফলাফল_file_name,
                               FT_Long    *ফলাফল_offset );

  স্থির FT_Error
  raccess_guess_darwin_hfsplus( FT_Library  library,
                                FT_Stream   stream,
                                অক্ষর       *base_file_name,
                                অক্ষর      **ফলাফল_file_name,
                                FT_Long    *ফলাফল_offset );

  স্থির FT_Error
  raccess_guess_vfat( FT_Library  library,
                      FT_Stream   stream,
                      অক্ষর       *base_file_name,
                      অক্ষর      **ফলাফল_file_name,
                      FT_Long    *ফলাফল_offset );

  স্থির FT_Error
  raccess_guess_linux_cap( FT_Library  library,
                           FT_Stream   stream,
                           অক্ষর       *base_file_name,
                           অক্ষর      **ফলাফল_file_name,
                           FT_Long    *ফলাফল_offset );

  স্থির FT_Error
  raccess_guess_linux_double( FT_Library  library,
                              FT_Stream   stream,
                              অক্ষর       *base_file_name,
                              অক্ষর      **ফলাফল_file_name,
                              FT_Long    *ফলাফল_offset );

  স্থির FT_Error
  raccess_guess_linux_netatalk( FT_Library  library,
                                FT_Stream   stream,
                                অক্ষর       *base_file_name,
                                অক্ষর      **ফলাফল_file_name,
                                FT_Long    *ফলাফল_offset );


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

  স্থির FT_Error
  raccess_guess_apple_generic( FT_Library  library,
                               FT_Stream   stream,
                               অক্ষর       *base_file_name,
                               FT_Int32    magic,
                               FT_Long    *ফলাফল_offset );

  স্থির FT_Error
  raccess_guess_linux_double_from_file_name( FT_Library  library,
                                             অক্ষর*       file_name,
                                             FT_Long    *ফলাফল_offset );

  স্থির অক্ষর *
  raccess_make_file_name( FT_Memory    memory,
                          ধ্রুবক অক্ষর  *original_name,
                          ধ্রুবক অক্ষর  *insertion );

  FT_BASE_DEF( শূন্য )
  FT_Raccess_Guess( FT_Library  library,
                    FT_Stream   stream,
                    অক্ষর*       base_name,
                    অক্ষর      **new_names,
                    FT_Long    *offsets,
                    FT_Error   *errors )
  {
    FT_Int  i;


    জন্য ( i = 0; i < FT_RACCESS_N_RULES; i++ )
    {
      new_names[i] = ফাঁকা;
      যদি ( ফাঁকা != stream )
        errors[i] = FT_Stream_Seek( stream, 0 );
      অন্যথা
        errors[i] = FT_Err_Ok;

      যদি ( errors[i] )
        চালু;

      errors[i] = ft_raccess_guess_table[i].func( library,
                                                  stream, base_name,
                                                  &(new_names[i]),
                                                  &(offsets[i]) );
    }

    ফেরত;
  }


#যদি defined( FT_CONFIG_OPTION_MAC_FONTS ) && !defined( FT_MACINTOSH )
  স্থির FT_RFork_Rule
  raccess_get_rule_type_from_rule_সূচক( FT_Library  library,
                                         FT_UInt     rule_সূচক )
  {
    FT_UNUSED( library );

    যদি ( rule_সূচক >= FT_RACCESS_N_RULES )
      ফেরত FT_RFork_Rule_invalid;

    ফেরত ft_raccess_guess_table[rule_সূচক].type;
  }


  /*
   * For this function, refer ftbase.h.
   */
  FT_LOCAL_DEF( FT_Bool )
  ft_raccess_rule_by_darwin_vfs( FT_Library  library,
                                 FT_UInt     rule_সূচক )
  {
    নির্বাচন( raccess_get_rule_type_from_rule_সূচক( library, rule_সূচক ) )
    {
      ক্ষেত্র FT_RFork_Rule_darwin_newvfs:
      ক্ষেত্র FT_RFork_Rule_darwin_hfsplus:
        ফেরত TRUE;

      পূর্বনির্ধারিত:
        ফেরত FALSE;
    }
  }
#endif


  স্থির FT_Error
  raccess_guess_apple_double( FT_Library  library,
                              FT_Stream   stream,
                              অক্ষর       *base_file_name,
                              অক্ষর      **ফলাফল_file_name,
                              FT_Long    *ফলাফল_offset )
  {
    FT_Int32  magic = ( 0x00 << 24 ) |
                      ( 0x05 << 16 ) |
                      ( 0x16 <<  8 ) |
                        0x07;


    *ফলাফল_file_name = ফাঁকা;
    যদি ( ফাঁকা == stream )
      ফেরত FT_THROW( Cannot_Open_Stream );

    ফেরত raccess_guess_apple_generic( library, stream, base_file_name,
                                        magic, ফলাফল_offset );
  }


  স্থির FT_Error
  raccess_guess_apple_single( FT_Library  library,
                              FT_Stream   stream,
                              অক্ষর       *base_file_name,
                              অক্ষর      **ফলাফল_file_name,
                              FT_Long    *ফলাফল_offset )
  {
    FT_Int32  magic = ( 0x00 << 24 ) |
                      ( 0x05 << 16 ) |
                      ( 0x16 <<  8 ) |
                        0x00;


    *ফলাফল_file_name = ফাঁকা;
    যদি ( ফাঁকা == stream )
      ফেরত FT_THROW( Cannot_Open_Stream );

    ফেরত raccess_guess_apple_generic( library, stream, base_file_name,
                                        magic, ফলাফল_offset );
  }


  স্থির FT_Error
  raccess_guess_darwin_ufs_export( FT_Library  library,
                                   FT_Stream   stream,
                                   অক্ষর       *base_file_name,
                                   অক্ষর      **ফলাফল_file_name,
                                   FT_Long    *ফলাফল_offset )
  {
    অক্ষর*      newpath;
    FT_Error   error;
    FT_Memory  memory;

    FT_UNUSED( stream );


    memory  = library->memory;
    newpath = raccess_make_file_name( memory, base_file_name, "._" );
    যদি ( !newpath )
      ফেরত FT_THROW( Out_Of_Memory );

    error = raccess_guess_linux_double_from_file_name( library, newpath,
                                                       ফলাফল_offset );
    যদি ( !error )
      *ফলাফল_file_name = newpath;
    অন্যথা
      FT_FREE( newpath );

    ফেরত error;
  }


  স্থির FT_Error
  raccess_guess_darwin_hfsplus( FT_Library  library,
                                FT_Stream   stream,
                                অক্ষর       *base_file_name,
                                অক্ষর      **ফলাফল_file_name,
                                FT_Long    *ফলাফল_offset )
  {
    /*
      Only meaningful on systems with hfs+ drivers (or Macs).
     */
    FT_Error   error;
    অক্ষর*      newpath = ফাঁকা;
    FT_Memory  memory;
    FT_Long    base_file_len = (FT_Long)ft_strlen( base_file_name );

    FT_UNUSED( stream );


    memory = library->memory;

    যদি ( base_file_len + 6 > FT_INT_MAX )
      ফেরত FT_THROW( Array_Too_Large );

    যদি ( FT_QALLOC( newpath, base_file_len + 6 ) )
      ফেরত error;

    FT_MEM_COPY( newpath, base_file_name, base_file_len );
    FT_MEM_COPY( newpath + base_file_len, "/rsrc", 6 );

    *ফলাফল_file_name = newpath;
    *ফলাফল_offset    = 0;

    ফেরত FT_Err_Ok;
  }


  স্থির FT_Error
  raccess_guess_darwin_newvfs( FT_Library  library,
                               FT_Stream   stream,
                               অক্ষর       *base_file_name,
                               অক্ষর      **ফলাফল_file_name,
                               FT_Long    *ফলাফল_offset )
  {
    /*
      Only meaningful on systems with Mac OS X (> 10.1).
     */
    FT_Error   error;
    অক্ষর*      newpath = ফাঁকা;
    FT_Memory  memory;
    FT_Long    base_file_len = (FT_Long)ft_strlen( base_file_name );

    FT_UNUSED( stream );


    memory = library->memory;

    যদি ( base_file_len + 18 > FT_INT_MAX )
      ফেরত FT_THROW( Array_Too_Large );

    যদি ( FT_QALLOC( newpath, base_file_len + 18 ) )
      ফেরত error;

    FT_MEM_COPY( newpath, base_file_name, base_file_len );
    FT_MEM_COPY( newpath + base_file_len, "/..namedfork/rsrc", 18 );

    *ফলাফল_file_name = newpath;
    *ফলাফল_offset    = 0;

    ফেরত FT_Err_Ok;
  }


  স্থির FT_Error
  raccess_guess_vfat( FT_Library  library,
                      FT_Stream   stream,
                      অক্ষর       *base_file_name,
                      অক্ষর      **ফলাফল_file_name,
                      FT_Long    *ফলাফল_offset )
  {
    অক্ষর*      newpath;
    FT_Memory  memory;

    FT_UNUSED( stream );


    memory = library->memory;

    newpath = raccess_make_file_name( memory, base_file_name,
                                      "resource.frk/" );
    যদি ( !newpath )
      ফেরত FT_THROW( Out_Of_Memory );

    *ফলাফল_file_name = newpath;
    *ফলাফল_offset    = 0;

    ফেরত FT_Err_Ok;
  }


  স্থির FT_Error
  raccess_guess_linux_cap( FT_Library  library,
                           FT_Stream   stream,
                           অক্ষর       *base_file_name,
                           অক্ষর      **ফলাফল_file_name,
                           FT_Long    *ফলাফল_offset )
  {
    অক্ষর*      newpath;
    FT_Memory  memory;

    FT_UNUSED( stream );


    memory = library->memory;

    newpath = raccess_make_file_name( memory, base_file_name, ".resource/" );
    যদি ( !newpath )
      ফেরত FT_THROW( Out_Of_Memory );

    *ফলাফল_file_name = newpath;
    *ফলাফল_offset    = 0;

    ফেরত FT_Err_Ok;
  }


  স্থির FT_Error
  raccess_guess_linux_double( FT_Library  library,
                              FT_Stream   stream,
                              অক্ষর       *base_file_name,
                              অক্ষর      **ফলাফল_file_name,
                              FT_Long    *ফলাফল_offset )
  {
    অক্ষর*      newpath;
    FT_Error   error;
    FT_Memory  memory;

    FT_UNUSED( stream );


    memory = library->memory;

    newpath = raccess_make_file_name( memory, base_file_name, "%" );
    যদি ( !newpath )
      ফেরত FT_THROW( Out_Of_Memory );

    error = raccess_guess_linux_double_from_file_name( library, newpath,
                                                       ফলাফল_offset );
    যদি ( !error )
      *ফলাফল_file_name = newpath;
    অন্যথা
      FT_FREE( newpath );

    ফেরত error;
  }


  স্থির FT_Error
  raccess_guess_linux_netatalk( FT_Library  library,
                                FT_Stream   stream,
                                অক্ষর       *base_file_name,
                                অক্ষর      **ফলাফল_file_name,
                                FT_Long    *ফলাফল_offset )
  {
    অক্ষর*      newpath;
    FT_Error   error;
    FT_Memory  memory;

    FT_UNUSED( stream );


    memory = library->memory;

    newpath = raccess_make_file_name( memory, base_file_name,
                                      ".AppleDouble/" );
    যদি ( !newpath )
      ফেরত FT_THROW( Out_Of_Memory );

    error = raccess_guess_linux_double_from_file_name( library, newpath,
                                                       ফলাফল_offset );
    যদি ( !error )
      *ফলাফল_file_name = newpath;
    অন্যথা
      FT_FREE( newpath );

    ফেরত error;
  }


  স্থির FT_Error
  raccess_guess_apple_generic( FT_Library  library,
                               FT_Stream   stream,
                               অক্ষর       *base_file_name,
                               FT_Int32    magic,
                               FT_Long    *ফলাফল_offset )
  {
    FT_Int32   magic_from_stream;
    FT_Error   error;
    FT_Int32   version_সংখ্যা = 0;
    FT_UShort  n_of_entries;

    পূর্ণসংখ্যা        i;
    FT_Int32   entry_id, entry_offset, entry_দৈর্ঘ্য = 0;

    ধ্রুবক FT_Int32  resource_fork_entry_id = 0x2;

    FT_UNUSED( library );
    FT_UNUSED( base_file_name );
    FT_UNUSED( version_সংখ্যা );
    FT_UNUSED( entry_দৈর্ঘ্য   );


    যদি ( FT_READ_LONG( magic_from_stream ) )
      ফেরত error;
    যদি ( magic_from_stream != magic )
      ফেরত FT_THROW( Unknown_File_Format );

    যদি ( FT_READ_LONG( version_সংখ্যা ) )
      ফেরত error;

    /* filler */
    error = FT_Stream_Skip( stream, 16 );
    যদি ( error )
      ফেরত error;

    যদি ( FT_READ_USHORT( n_of_entries ) )
      ফেরত error;
    যদি ( n_of_entries == 0 )
      ফেরত FT_THROW( Unknown_File_Format );

    জন্য ( i = 0; i < n_of_entries; i++ )
    {
      যদি ( FT_READ_LONG( entry_id ) )
        ফেরত error;
      যদি ( entry_id == resource_fork_entry_id )
      {
        যদি ( FT_READ_LONG( entry_offset ) ||
             FT_READ_LONG( entry_দৈর্ঘ্য ) )
          চালু;
        *ফলাফল_offset = entry_offset;

        ফেরত FT_Err_Ok;
      }
      অন্যথা
      {
        error = FT_Stream_Skip( stream, 4 + 4 );    /* offset + length */
        যদি ( error )
          ফেরত error;
      }
    }

    ফেরত FT_THROW( Unknown_File_Format );
  }


  স্থির FT_Error
  raccess_guess_linux_double_from_file_name( FT_Library  library,
                                             অক্ষর       *file_name,
                                             FT_Long    *ফলাফল_offset )
  {
    FT_Open_Args  args2;
    FT_Stream     stream2;
    অক্ষর*         nouse = ফাঁকা;
    FT_Error      error;


    args2.flags    = FT_OPEN_PATHNAME;
    args2.pathname = file_name;
    error = FT_Stream_New( library, &args2, &stream2 );
    যদি ( error )
      ফেরত error;

    error = raccess_guess_apple_double( library, stream2, file_name,
                                        &nouse, ফলাফল_offset );

    FT_Stream_Free( stream2, 0 );

    ফেরত error;
  }


  স্থির অক্ষর*
  raccess_make_file_name( FT_Memory    memory,
                          ধ্রুবক অক্ষর  *original_name,
                          ধ্রুবক অক্ষর  *insertion )
  {
    অক্ষর*        new_name = ফাঁকা;
    ধ্রুবক অক্ষর*  tmp;
    ধ্রুবক অক্ষর*  slash;
    size_t       new_দৈর্ঘ্য;
    FT_Error     error;


    new_দৈর্ঘ্য = ft_strlen( original_name ) + ft_strlen( insertion );
    যদি ( FT_QALLOC( new_name, new_দৈর্ঘ্য + 1 ) )
      ফেরত ফাঁকা;

    tmp = ft_strrchr( original_name, '/' );
    যদি ( tmp )
    {
      ft_strncpy( new_name,
                  original_name,
                  (size_t)( tmp - original_name + 1 ) );
      new_name[tmp - original_name + 1] = '\0';
      slash = tmp + 1;
    }
    অন্যথা
    {
      slash       = original_name;
      new_name[0] = '\0';
    }

    ft_strcat( new_name, insertion );
    ft_strcat( new_name, slash );

    ফেরত new_name;
  }


#অন্যথা   /* !FT_CONFIG_OPTION_GUESSING_EMBEDDED_RFORK */


  /**************************************************************************
   *                 Dummy function; just sets errors
   */

  FT_BASE_DEF( শূন্য )
  FT_Raccess_Guess( FT_Library  library,
                    FT_Stream   stream,
                    অক্ষর       *base_name,
                    অক্ষর      **new_names,
                    FT_Long    *offsets,
                    FT_Error   *errors )
  {
    FT_Int  i;

    FT_UNUSED( library );
    FT_UNUSED( stream );
    FT_UNUSED( base_name );


    জন্য ( i = 0; i < FT_RACCESS_N_RULES; i++ )
    {
      new_names[i] = ফাঁকা;
      offsets[i]   = 0;
      errors[i]    = FT_ERR( Unimplemented_Feature );
    }
  }


#endif  /* !FT_CONFIG_OPTION_GUESSING_EMBEDDED_RFORK */


/* END */
