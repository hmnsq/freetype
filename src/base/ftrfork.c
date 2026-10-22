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


#समावेश <freetype/internal/ftdebug.h>
#समावेश <freetype/internal/ftstream.h>
#समावेश <freetype/internal/ftrfork.h>

#समावेश "ftbase.h"

#अघोषित  FT_COMPONENT
#घोषणा FT_COMPONENT  raccess


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
    अचिन्हित अक्षर  शीर्ष_2[16], head2[16];
    FT_Long        map_pos, map_len, rdata_len;
    पूर्णांक            allzeros, allmatch, i;
    FT_Long        type_list;

    FT_UNUSED( library );


    error = FT_Stream_Seek( stream, (FT_ULong)rfork_offset );
    अगर ( error )
      वापस error;

    error = FT_Stream_Read( stream, (FT_Byte*)शीर्ष_2, 16 );
    अगर ( error )
      वापस error;

    /* ensure positive values */
    अगर ( शीर्ष_2[0]  >= 0x80 ||
         शीर्ष_2[4]  >= 0x80 ||
         शीर्ष_2[8]  >= 0x80 ||
         शीर्ष_2[12] >= 0x80 )
      वापस FT_THROW( Unknown_File_Format );

    *rdata_pos = ( शीर्ष_2[ 0] << 24 ) |
                 ( शीर्ष_2[ 1] << 16 ) |
                 ( शीर्ष_2[ 2] <<  8 ) |
                   शीर्ष_2[ 3];
    map_pos    = ( शीर्ष_2[ 4] << 24 ) |
                 ( शीर्ष_2[ 5] << 16 ) |
                 ( शीर्ष_2[ 6] <<  8 ) |
                   शीर्ष_2[ 7];
    rdata_len  = ( शीर्ष_2[ 8] << 24 ) |
                 ( शीर्ष_2[ 9] << 16 ) |
                 ( शीर्ष_2[10] <<  8 ) |
                   शीर्ष_2[11];
    map_len    = ( शीर्ष_2[12] << 24 ) |
                 ( शीर्ष_2[13] << 16 ) |
                 ( शीर्ष_2[14] <<  8 ) |
                   शीर्ष_2[15];

    /* the map must not be empty */
    अगर ( !map_pos )
      वापस FT_THROW( Unknown_File_Format );

    /* check whether rdata and map overlap */
    अगर ( *rdata_pos < map_pos )
    {
      अगर ( *rdata_pos > map_pos - rdata_len )
        वापस FT_THROW( Unknown_File_Format );
    }
    अन्यथा
    {
      अगर ( map_pos > *rdata_pos - map_len )
        वापस FT_THROW( Unknown_File_Format );
    }

    /* check whether end of rdata or map exceeds stream size */
    अगर ( FT_LONG_MAX - rdata_len < *rdata_pos                               ||
         FT_LONG_MAX - map_len < map_pos                                    ||

         FT_LONG_MAX - ( *rdata_pos + rdata_len ) < rfork_offset            ||
         FT_LONG_MAX - ( map_pos + map_len ) < rfork_offset                 ||

         (FT_ULong)( rfork_offset + *rdata_pos + rdata_len ) > stream->आकार ||
         (FT_ULong)( rfork_offset + map_pos + map_len ) > stream->आकार      )
      वापस FT_THROW( Unknown_File_Format );

    *rdata_pos += rfork_offset;
    map_pos    += rfork_offset;

    error = FT_Stream_Seek( stream, (FT_ULong)map_pos );
    अगर ( error )
      वापस error;

    head2[15] = (FT_Byte)( शीर्ष_2[15] + 1 );       /* make it be different */

    error = FT_Stream_Read( stream, (FT_Byte*)head2, 16 );
    अगर ( error )
      वापस error;

    allzeros = 1;
    allmatch = 1;
    क्रम ( i = 0; i < 16; i++ )
    {
      अगर ( head2[i] != 0 )
        allzeros = 0;
      अगर ( head2[i] != शीर्ष_2[i] )
        allmatch = 0;
    }
    अगर ( !allzeros && !allmatch )
      वापस FT_THROW( Unknown_File_Format );

    /* If we have reached this point then it is probably a mac resource */
    /* file.  Now, does it contain any interesting resources?           */

    (व्योम)FT_STREAM_SKIP( 4        /* skip handle to next resource map */
                          + 2      /* skip file resource number */
                          + 2 );   /* skip attributes */

    अगर ( FT_READ_SHORT( type_list ) )
      वापस error;
    अगर ( type_list < 0 )
      वापस FT_THROW( Unknown_File_Format );

    error = FT_Stream_Seek( stream, (FT_ULong)( map_pos + type_list ) );
    अगर ( error )
      वापस error;

    *map_offset = map_pos + type_list;
    वापस FT_Err_Ok;
  }


  FT_COMPARE_DEF( पूर्णांक )
  ft_raccess_sort_ref_by_id( स्थिर व्योम*  a,
                             स्थिर व्योम*  b )
  {
    वापस  ( (FT_RFork_Ref*)a )->res_id - ( (FT_RFork_Ref*)b )->res_id;
  }


  FT_BASE_DEF( FT_Error )
  FT_Raccess_Get_तथ्य_Offsets( FT_Library  library,
                              FT_Stream   stream,
                              FT_Long     map_offset,
                              FT_Long     rdata_pos,
                              FT_Long     tag,
                              FT_Bool     sort_by_res_id,
                              FT_Long   **offsets,
                              FT_Long    *गिनती )
  {
    FT_Error      error;
    पूर्णांक           i, j, cnt, subcnt;
    FT_Long       tag_internal, rpos;
    FT_Memory     memory = library->memory;
    FT_Long       अस्थायी;
    FT_Long       *offsets_internal = शून्य;
    FT_RFork_Ref  *ref = शून्य;


    FT_TRACE3(( "\n" ));
    error = FT_Stream_Seek( stream, (FT_ULong)map_offset );
    अगर ( error )
      वापस error;

    अगर ( FT_READ_SHORT( cnt ) )
      वापस error;
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
    अगर ( cnt > 4079 )
      वापस FT_THROW( Invalid_Table );

    क्रम ( i = 0; i < cnt; i++ )
    {
      अगर ( FT_READ_LONG( tag_internal ) ||
           FT_READ_SHORT( subcnt )      ||
           FT_READ_SHORT( rpos )        )
        वापस error;

      FT_TRACE2(( "Resource tags: %c%c%c%c\n",
                  (अक्षर)( 0xFF & ( tag_internal >> 24 ) ),
                  (अक्षर)( 0xFF & ( tag_internal >> 16 ) ),
                  (अक्षर)( 0xFF & ( tag_internal >>  8 ) ),
                  (अक्षर)( 0xFF & ( tag_internal >>  0 ) ) ));
      FT_TRACE3(( "             : subcount=%d, suboffset=0x%04lx\n",
                  subcnt, rpos ));

      अगर ( tag_internal == tag )
      {
        *गिनती = subcnt + 1;
        rpos  += map_offset;

        /* a zero count might be valid in the resource specification, */
        /* however, it is completely useless to us                    */
        अगर ( *गिनती < 1 || *गिनती > 2727 )
          वापस FT_THROW( Invalid_Table );

        error = FT_Stream_Seek( stream, (FT_ULong)rpos );
        अगर ( error )
          वापस error;

        अगर ( FT_QNEW_ARRAY( ref, *गिनती ) )
          वापस error;

        क्रम ( j = 0; j < *गिनती; j++ )
        {
          अगर ( FT_READ_SHORT( ref[j].res_id ) )
            जाओ Exit;
          अगर ( FT_STREAM_SKIP( 2 ) )  /* resource name offset */
            जाओ Exit;
          अगर ( FT_READ_LONG( अस्थायी ) ) /* attributes (8bit), offset (24bit) */
            जाओ Exit;
          अगर ( FT_STREAM_SKIP( 4 ) )  /* mbz */
            जाओ Exit;

          /*
           * According to Inside Macintosh: More Macintosh Toolbox,
           * "Resource IDs" (1-46), there are some reserved IDs.
           * However, FreeType2 is not a font synthesizer, no need
           * to check the acceptable resource ID or its attributes.
           */
          ref[j].offset = अस्थायी & 0xFFFFFFL;

          FT_TRACE3(( "             [%d]:"
                      " resource_id=0x%04x, offset=0x%08lx\n",
                      j, (FT_UShort)ref[j].res_id, ref[j].offset ));
        }

        अगर ( sort_by_res_id )
        {
          ft_qsort( ref,
                    (माप_प्रकार)*गिनती,
                    माप ( FT_RFork_Ref ),
                    ft_raccess_sort_ref_by_id );

          FT_TRACE3(( "             -- sort resources by their ids --\n" ));

          क्रम ( j = 0; j < *गिनती; j++ )
            FT_TRACE3(( "             [%d]:"
                        " resource_id=0x%04x, offset=0x%08lx\n",
                        j, ref[j].res_id, ref[j].offset ));
        }

        अगर ( FT_QNEW_ARRAY( offsets_internal, *गिनती ) )
          जाओ Exit;

        /* XXX: duplicated reference ID,
         *      gap between reference IDs are acceptable?
         *      further investigation on Apple implementation is needed.
         */
        क्रम ( j = 0; j < *गिनती; j++ )
          offsets_internal[j] = rdata_pos + ref[j].offset;

        *offsets = offsets_internal;
        error    = FT_Err_Ok;

      Exit:
        FT_FREE( ref );
        वापस error;
      }
    }

    वापस FT_THROW( Cannot_Open_Resource );
  }


#अगर_घोषित FT_CONFIG_OPTION_GUESSING_EMBEDDED_RFORK

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

  अटल FT_Error
  raccess_guess_apple_double( FT_Library  library,
                              FT_Stream   stream,
                              अक्षर       *base_file_name,
                              अक्षर      **परिणाम_file_name,
                              FT_Long    *परिणाम_offset );

  अटल FT_Error
  raccess_guess_apple_single( FT_Library  library,
                              FT_Stream   stream,
                              अक्षर       *base_file_name,
                              अक्षर      **परिणाम_file_name,
                              FT_Long    *परिणाम_offset );

  अटल FT_Error
  raccess_guess_darwin_ufs_export( FT_Library  library,
                                   FT_Stream   stream,
                                   अक्षर       *base_file_name,
                                   अक्षर      **परिणाम_file_name,
                                   FT_Long    *परिणाम_offset );

  अटल FT_Error
  raccess_guess_darwin_newvfs( FT_Library  library,
                               FT_Stream   stream,
                               अक्षर       *base_file_name,
                               अक्षर      **परिणाम_file_name,
                               FT_Long    *परिणाम_offset );

  अटल FT_Error
  raccess_guess_darwin_hfsplus( FT_Library  library,
                                FT_Stream   stream,
                                अक्षर       *base_file_name,
                                अक्षर      **परिणाम_file_name,
                                FT_Long    *परिणाम_offset );

  अटल FT_Error
  raccess_guess_vfat( FT_Library  library,
                      FT_Stream   stream,
                      अक्षर       *base_file_name,
                      अक्षर      **परिणाम_file_name,
                      FT_Long    *परिणाम_offset );

  अटल FT_Error
  raccess_guess_linux_cap( FT_Library  library,
                           FT_Stream   stream,
                           अक्षर       *base_file_name,
                           अक्षर      **परिणाम_file_name,
                           FT_Long    *परिणाम_offset );

  अटल FT_Error
  raccess_guess_linux_double( FT_Library  library,
                              FT_Stream   stream,
                              अक्षर       *base_file_name,
                              अक्षर      **परिणाम_file_name,
                              FT_Long    *परिणाम_offset );

  अटल FT_Error
  raccess_guess_linux_netatalk( FT_Library  library,
                                FT_Stream   stream,
                                अक्षर       *base_file_name,
                                अक्षर      **परिणाम_file_name,
                                FT_Long    *परिणाम_offset );


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

  अटल FT_Error
  raccess_guess_apple_generic( FT_Library  library,
                               FT_Stream   stream,
                               अक्षर       *base_file_name,
                               FT_Int32    magic,
                               FT_Long    *परिणाम_offset );

  अटल FT_Error
  raccess_guess_linux_double_from_file_name( FT_Library  library,
                                             अक्षर*       file_name,
                                             FT_Long    *परिणाम_offset );

  अटल अक्षर *
  raccess_make_file_name( FT_Memory    memory,
                          स्थिर अक्षर  *original_name,
                          स्थिर अक्षर  *insertion );

  FT_BASE_DEF( व्योम )
  FT_Raccess_Guess( FT_Library  library,
                    FT_Stream   stream,
                    अक्षर*       base_name,
                    अक्षर      **new_names,
                    FT_Long    *offsets,
                    FT_Error   *errors )
  {
    FT_Int  i;


    क्रम ( i = 0; i < FT_RACCESS_N_RULES; i++ )
    {
      new_names[i] = शून्य;
      अगर ( शून्य != stream )
        errors[i] = FT_Stream_Seek( stream, 0 );
      अन्यथा
        errors[i] = FT_Err_Ok;

      अगर ( errors[i] )
        जारी;

      errors[i] = ft_raccess_guess_table[i].func( library,
                                                  stream, base_name,
                                                  &(new_names[i]),
                                                  &(offsets[i]) );
    }

    वापस;
  }


#अगर defined( FT_CONFIG_OPTION_MAC_FONTS ) && !defined( FT_MACINTOSH )
  अटल FT_RFork_Rule
  raccess_get_rule_type_from_rule_सूचकांक( FT_Library  library,
                                         FT_UInt     rule_सूचकांक )
  {
    FT_UNUSED( library );

    अगर ( rule_सूचकांक >= FT_RACCESS_N_RULES )
      वापस FT_RFork_Rule_invalid;

    वापस ft_raccess_guess_table[rule_सूचकांक].type;
  }


  /*
   * For this function, refer ftbase.h.
   */
  FT_LOCAL_DEF( FT_Bool )
  ft_raccess_rule_by_darwin_vfs( FT_Library  library,
                                 FT_UInt     rule_सूचकांक )
  {
    चयन( raccess_get_rule_type_from_rule_सूचकांक( library, rule_सूचकांक ) )
    {
      हाल FT_RFork_Rule_darwin_newvfs:
      हाल FT_RFork_Rule_darwin_hfsplus:
        वापस TRUE;

      शेष:
        वापस FALSE;
    }
  }
#पूर्ण_अगर


  अटल FT_Error
  raccess_guess_apple_double( FT_Library  library,
                              FT_Stream   stream,
                              अक्षर       *base_file_name,
                              अक्षर      **परिणाम_file_name,
                              FT_Long    *परिणाम_offset )
  {
    FT_Int32  magic = ( 0x00 << 24 ) |
                      ( 0x05 << 16 ) |
                      ( 0x16 <<  8 ) |
                        0x07;


    *परिणाम_file_name = शून्य;
    अगर ( शून्य == stream )
      वापस FT_THROW( Cannot_Open_Stream );

    वापस raccess_guess_apple_generic( library, stream, base_file_name,
                                        magic, परिणाम_offset );
  }


  अटल FT_Error
  raccess_guess_apple_single( FT_Library  library,
                              FT_Stream   stream,
                              अक्षर       *base_file_name,
                              अक्षर      **परिणाम_file_name,
                              FT_Long    *परिणाम_offset )
  {
    FT_Int32  magic = ( 0x00 << 24 ) |
                      ( 0x05 << 16 ) |
                      ( 0x16 <<  8 ) |
                        0x00;


    *परिणाम_file_name = शून्य;
    अगर ( शून्य == stream )
      वापस FT_THROW( Cannot_Open_Stream );

    वापस raccess_guess_apple_generic( library, stream, base_file_name,
                                        magic, परिणाम_offset );
  }


  अटल FT_Error
  raccess_guess_darwin_ufs_export( FT_Library  library,
                                   FT_Stream   stream,
                                   अक्षर       *base_file_name,
                                   अक्षर      **परिणाम_file_name,
                                   FT_Long    *परिणाम_offset )
  {
    अक्षर*      newpath;
    FT_Error   error;
    FT_Memory  memory;

    FT_UNUSED( stream );


    memory  = library->memory;
    newpath = raccess_make_file_name( memory, base_file_name, "._" );
    अगर ( !newpath )
      वापस FT_THROW( Out_Of_Memory );

    error = raccess_guess_linux_double_from_file_name( library, newpath,
                                                       परिणाम_offset );
    अगर ( !error )
      *परिणाम_file_name = newpath;
    अन्यथा
      FT_FREE( newpath );

    वापस error;
  }


  अटल FT_Error
  raccess_guess_darwin_hfsplus( FT_Library  library,
                                FT_Stream   stream,
                                अक्षर       *base_file_name,
                                अक्षर      **परिणाम_file_name,
                                FT_Long    *परिणाम_offset )
  {
    /*
      Only meaningful on systems with hfs+ drivers (or Macs).
     */
    FT_Error   error;
    अक्षर*      newpath = शून्य;
    FT_Memory  memory;
    FT_Long    base_file_len = (FT_Long)ft_strlen( base_file_name );

    FT_UNUSED( stream );


    memory = library->memory;

    अगर ( base_file_len + 6 > FT_INT_MAX )
      वापस FT_THROW( Array_Too_Large );

    अगर ( FT_QALLOC( newpath, base_file_len + 6 ) )
      वापस error;

    FT_MEM_COPY( newpath, base_file_name, base_file_len );
    FT_MEM_COPY( newpath + base_file_len, "/rsrc", 6 );

    *परिणाम_file_name = newpath;
    *परिणाम_offset    = 0;

    वापस FT_Err_Ok;
  }


  अटल FT_Error
  raccess_guess_darwin_newvfs( FT_Library  library,
                               FT_Stream   stream,
                               अक्षर       *base_file_name,
                               अक्षर      **परिणाम_file_name,
                               FT_Long    *परिणाम_offset )
  {
    /*
      Only meaningful on systems with Mac OS X (> 10.1).
     */
    FT_Error   error;
    अक्षर*      newpath = शून्य;
    FT_Memory  memory;
    FT_Long    base_file_len = (FT_Long)ft_strlen( base_file_name );

    FT_UNUSED( stream );


    memory = library->memory;

    अगर ( base_file_len + 18 > FT_INT_MAX )
      वापस FT_THROW( Array_Too_Large );

    अगर ( FT_QALLOC( newpath, base_file_len + 18 ) )
      वापस error;

    FT_MEM_COPY( newpath, base_file_name, base_file_len );
    FT_MEM_COPY( newpath + base_file_len, "/..namedfork/rsrc", 18 );

    *परिणाम_file_name = newpath;
    *परिणाम_offset    = 0;

    वापस FT_Err_Ok;
  }


  अटल FT_Error
  raccess_guess_vfat( FT_Library  library,
                      FT_Stream   stream,
                      अक्षर       *base_file_name,
                      अक्षर      **परिणाम_file_name,
                      FT_Long    *परिणाम_offset )
  {
    अक्षर*      newpath;
    FT_Memory  memory;

    FT_UNUSED( stream );


    memory = library->memory;

    newpath = raccess_make_file_name( memory, base_file_name,
                                      "resource.frk/" );
    अगर ( !newpath )
      वापस FT_THROW( Out_Of_Memory );

    *परिणाम_file_name = newpath;
    *परिणाम_offset    = 0;

    वापस FT_Err_Ok;
  }


  अटल FT_Error
  raccess_guess_linux_cap( FT_Library  library,
                           FT_Stream   stream,
                           अक्षर       *base_file_name,
                           अक्षर      **परिणाम_file_name,
                           FT_Long    *परिणाम_offset )
  {
    अक्षर*      newpath;
    FT_Memory  memory;

    FT_UNUSED( stream );


    memory = library->memory;

    newpath = raccess_make_file_name( memory, base_file_name, ".resource/" );
    अगर ( !newpath )
      वापस FT_THROW( Out_Of_Memory );

    *परिणाम_file_name = newpath;
    *परिणाम_offset    = 0;

    वापस FT_Err_Ok;
  }


  अटल FT_Error
  raccess_guess_linux_double( FT_Library  library,
                              FT_Stream   stream,
                              अक्षर       *base_file_name,
                              अक्षर      **परिणाम_file_name,
                              FT_Long    *परिणाम_offset )
  {
    अक्षर*      newpath;
    FT_Error   error;
    FT_Memory  memory;

    FT_UNUSED( stream );


    memory = library->memory;

    newpath = raccess_make_file_name( memory, base_file_name, "%" );
    अगर ( !newpath )
      वापस FT_THROW( Out_Of_Memory );

    error = raccess_guess_linux_double_from_file_name( library, newpath,
                                                       परिणाम_offset );
    अगर ( !error )
      *परिणाम_file_name = newpath;
    अन्यथा
      FT_FREE( newpath );

    वापस error;
  }


  अटल FT_Error
  raccess_guess_linux_netatalk( FT_Library  library,
                                FT_Stream   stream,
                                अक्षर       *base_file_name,
                                अक्षर      **परिणाम_file_name,
                                FT_Long    *परिणाम_offset )
  {
    अक्षर*      newpath;
    FT_Error   error;
    FT_Memory  memory;

    FT_UNUSED( stream );


    memory = library->memory;

    newpath = raccess_make_file_name( memory, base_file_name,
                                      ".AppleDouble/" );
    अगर ( !newpath )
      वापस FT_THROW( Out_Of_Memory );

    error = raccess_guess_linux_double_from_file_name( library, newpath,
                                                       परिणाम_offset );
    अगर ( !error )
      *परिणाम_file_name = newpath;
    अन्यथा
      FT_FREE( newpath );

    वापस error;
  }


  अटल FT_Error
  raccess_guess_apple_generic( FT_Library  library,
                               FT_Stream   stream,
                               अक्षर       *base_file_name,
                               FT_Int32    magic,
                               FT_Long    *परिणाम_offset )
  {
    FT_Int32   magic_from_stream;
    FT_Error   error;
    FT_Int32   version_संख्या = 0;
    FT_UShort  n_of_entries;

    पूर्णांक        i;
    FT_Int32   entry_id, entry_offset, entry_लंबाई = 0;

    स्थिर FT_Int32  resource_fork_entry_id = 0x2;

    FT_UNUSED( library );
    FT_UNUSED( base_file_name );
    FT_UNUSED( version_संख्या );
    FT_UNUSED( entry_लंबाई   );


    अगर ( FT_READ_LONG( magic_from_stream ) )
      वापस error;
    अगर ( magic_from_stream != magic )
      वापस FT_THROW( Unknown_File_Format );

    अगर ( FT_READ_LONG( version_संख्या ) )
      वापस error;

    /* filler */
    error = FT_Stream_Skip( stream, 16 );
    अगर ( error )
      वापस error;

    अगर ( FT_READ_USHORT( n_of_entries ) )
      वापस error;
    अगर ( n_of_entries == 0 )
      वापस FT_THROW( Unknown_File_Format );

    क्रम ( i = 0; i < n_of_entries; i++ )
    {
      अगर ( FT_READ_LONG( entry_id ) )
        वापस error;
      अगर ( entry_id == resource_fork_entry_id )
      {
        अगर ( FT_READ_LONG( entry_offset ) ||
             FT_READ_LONG( entry_लंबाई ) )
          जारी;
        *परिणाम_offset = entry_offset;

        वापस FT_Err_Ok;
      }
      अन्यथा
      {
        error = FT_Stream_Skip( stream, 4 + 4 );    /* offset + length */
        अगर ( error )
          वापस error;
      }
    }

    वापस FT_THROW( Unknown_File_Format );
  }


  अटल FT_Error
  raccess_guess_linux_double_from_file_name( FT_Library  library,
                                             अक्षर       *file_name,
                                             FT_Long    *परिणाम_offset )
  {
    FT_Open_Args  args2;
    FT_Stream     stream2;
    अक्षर*         nouse = शून्य;
    FT_Error      error;


    args2.flags    = FT_OPEN_PATHNAME;
    args2.pathname = file_name;
    error = FT_Stream_New( library, &args2, &stream2 );
    अगर ( error )
      वापस error;

    error = raccess_guess_apple_double( library, stream2, file_name,
                                        &nouse, परिणाम_offset );

    FT_Stream_Free( stream2, 0 );

    वापस error;
  }


  अटल अक्षर*
  raccess_make_file_name( FT_Memory    memory,
                          स्थिर अक्षर  *original_name,
                          स्थिर अक्षर  *insertion )
  {
    अक्षर*        new_name = शून्य;
    स्थिर अक्षर*  tmp;
    स्थिर अक्षर*  slash;
    माप_प्रकार       new_लंबाई;
    FT_Error     error;


    new_लंबाई = ft_strlen( original_name ) + ft_strlen( insertion );
    अगर ( FT_QALLOC( new_name, new_लंबाई + 1 ) )
      वापस शून्य;

    tmp = ft_strrchr( original_name, '/' );
    अगर ( tmp )
    {
      ft_strncpy( new_name,
                  original_name,
                  (माप_प्रकार)( tmp - original_name + 1 ) );
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

    वापस new_name;
  }


#अन्यथा   /* !FT_CONFIG_OPTION_GUESSING_EMBEDDED_RFORK */


  /**************************************************************************
   *                 Dummy function; just sets errors
   */

  FT_BASE_DEF( व्योम )
  FT_Raccess_Guess( FT_Library  library,
                    FT_Stream   stream,
                    अक्षर       *base_name,
                    अक्षर      **new_names,
                    FT_Long    *offsets,
                    FT_Error   *errors )
  {
    FT_Int  i;

    FT_UNUSED( library );
    FT_UNUSED( stream );
    FT_UNUSED( base_name );


    क्रम ( i = 0; i < FT_RACCESS_N_RULES; i++ )
    {
      new_names[i] = शून्य;
      offsets[i]   = 0;
      errors[i]    = FT_ERR( Unimplemented_Feature );
    }
  }


#पूर्ण_अगर  /* !FT_CONFIG_OPTION_GUESSING_EMBEDDED_RFORK */


/* END */
