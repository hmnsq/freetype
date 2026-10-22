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


#include <freetype/internal/ftdebug.h>
#include <freetype/internal/ftstream.h>
#include <freetype/internal/ftrfork.h>

#include "ftbase.h"

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
    unsigned var_na  head[16], head2[16];
    FT_Long        map_pos, map_len, rdata_len;
    ཧྲིལ            allzeros, allmatch, i;
    FT_Long        type_list;

    FT_UNUSED( library );


    error = FT_Stream_Seek( stream, (FT_ULong)rfork_offset );
    གལ་སྲིད ( error )
      ལོག error;

    error = FT_Stream_Read( stream, (FT_Byte*)head, 16 );
    གལ་སྲིད ( error )
      ལོག error;

    /* ensure positive values */
    གལ་སྲིད ( head[0]  >= 0x80 ||
         head[4]  >= 0x80 ||
         head[8]  >= 0x80 ||
         head[12] >= 0x80 )
      ལོག FT_THROW( Unknown_File_Format );

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
    གལ་སྲིད ( !map_pos )
      ལོག FT_THROW( Unknown_File_Format );

    /* check whether rdata and map overlap */
    གལ་སྲིད ( *rdata_pos < map_pos )
    {
      གལ་སྲིད ( *rdata_pos > map_pos - rdata_len )
        ལོག FT_THROW( Unknown_File_Format );
    }
    དེ་མིན
    {
      གལ་སྲིད ( map_pos > *rdata_pos - map_len )
        ལོག FT_THROW( Unknown_File_Format );
    }

    /* check whether end of rdata or map exceeds stream size */
    གལ་སྲིད ( FT_LONG_MAX - rdata_len < *rdata_pos                               ||
         FT_LONG_MAX - map_len < map_pos                                    ||

         FT_LONG_MAX - ( *rdata_pos + rdata_len ) < rfork_offset            ||
         FT_LONG_MAX - ( map_pos + map_len ) < rfork_offset                 ||

         (FT_ULong)( rfork_offset + *rdata_pos + rdata_len ) > stream->size ||
         (FT_ULong)( rfork_offset + map_pos + map_len ) > stream->size      )
      ལོག FT_THROW( Unknown_File_Format );

    *rdata_pos += rfork_offset;
    map_pos    += rfork_offset;

    error = FT_Stream_Seek( stream, (FT_ULong)map_pos );
    གལ་སྲིད ( error )
      ལོག error;

    head2[15] = (FT_Byte)( head[15] + 1 );       /* make it be different */

    error = FT_Stream_Read( stream, (FT_Byte*)head2, 16 );
    གལ་སྲིད ( error )
      ལོག error;

    allzeros = 1;
    allmatch = 1;
    དོན་ལུ ( i = 0; i < 16; i++ )
    {
      གལ་སྲིད ( head2[i] != 0 )
        allzeros = 0;
      གལ་སྲིད ( head2[i] != head[i] )
        allmatch = 0;
    }
    གལ་སྲིད ( !allzeros && !allmatch )
      ལོག FT_THROW( Unknown_File_Format );

    /* If we have reached this point then it is probably a mac resource */
    /* file.  Now, does it contain any interesting resources?           */

    (void)FT_STREAM_SKIP( 4        /* skip handle to next resource map */
                          + 2      /* skip file resource number */
                          + 2 );   /* skip attributes */

    གལ་སྲིད ( FT_READ_SHORT( type_list ) )
      ལོག error;
    གལ་སྲིད ( type_list < 0 )
      ལོག FT_THROW( Unknown_File_Format );

    error = FT_Stream_Seek( stream, (FT_ULong)( map_pos + type_list ) );
    གལ་སྲིད ( error )
      ལོག error;

    *map_offset = map_pos + type_list;
    ལོག FT_Err_Ok;
  }


  FT_COMPARE_DEF( ཧྲིལ )
  ft_raccess_sort_ref_by_id( const void*  a,
                             const void*  b )
  {
    ལོག  ( (FT_RFork_Ref*)a )->res_id - ( (FT_RFork_Ref*)b )->res_id;
  }


  FT_BASE_DEF( FT_Error )
  FT_Raccess_Get_DataOffsets( FT_Library  library,
                              FT_Stream   stream,
                              FT_Long     map_offset,
                              FT_Long     rdata_pos,
                              FT_Long     tag,
                              FT_Bool     sort_by_res_id,
                              FT_Long   **offsets,
                              FT_Long    *count )
  {
    FT_Error      error;
    ཧྲིལ           i, j, cnt, subcnt;
    FT_Long       tag_internal, rpos;
    FT_Memory     memory = library->memory;
    FT_Long       temp;
    FT_Long       *offsets_internal = NULL;
    FT_RFork_Ref  *ref = NULL;


    FT_TRACE3(( "\n" ));
    error = FT_Stream_Seek( stream, (FT_ULong)map_offset );
    གལ་སྲིད ( error )
      ལོག error;

    གལ་སྲིད ( FT_READ_SHORT( cnt ) )
      ལོག error;
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
    གལ་སྲིད ( cnt > 4079 )
      ལོག FT_THROW( Invalid_Table );

    དོན་ལུ ( i = 0; i < cnt; i++ )
    {
      གལ་སྲིད ( FT_READ_LONG( tag_internal ) ||
           FT_READ_SHORT( subcnt )      ||
           FT_READ_SHORT( rpos )        )
        ལོག error;

      FT_TRACE2(( "Resource tags: %c%c%c%c\n",
                  (var_na)( 0xFF & ( tag_internal >> 24 ) ),
                  (var_na)( 0xFF & ( tag_internal >> 16 ) ),
                  (var_na)( 0xFF & ( tag_internal >>  8 ) ),
                  (var_na)( 0xFF & ( tag_internal >>  0 ) ) ));
      FT_TRACE3(( "             : subcount=%d, suboffset=0x%04lx\n",
                  subcnt, rpos ));

      གལ་སྲིད ( tag_internal == tag )
      {
        *count = subcnt + 1;
        rpos  += map_offset;

        /* a zero count might be valid in the resource specification, */
        /* however, it is completely useless to us                    */
        གལ་སྲིད ( *count < 1 || *count > 2727 )
          ལོག FT_THROW( Invalid_Table );

        error = FT_Stream_Seek( stream, (FT_ULong)rpos );
        གལ་སྲིད ( error )
          ལོག error;

        གལ་སྲིད ( FT_QNEW_ARRAY( ref, *count ) )
          ལོག error;

        དོན་ལུ ( j = 0; j < *count; j++ )
        {
          གལ་སྲིད ( FT_READ_SHORT( ref[j].res_id ) )
            goto Exit;
          གལ་སྲིད ( FT_STREAM_SKIP( 2 ) )  /* resource name offset */
            goto Exit;
          གལ་སྲིད ( FT_READ_LONG( temp ) ) /* attributes (8bit), offset (24bit) */
            goto Exit;
          གལ་སྲིད ( FT_STREAM_SKIP( 4 ) )  /* mbz */
            goto Exit;

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

        གལ་སྲིད ( sort_by_res_id )
        {
          ft_qsort( ref,
                    (size_t)*count,
                    parimaa_na ( FT_RFork_Ref ),
                    ft_raccess_sort_ref_by_id );

          FT_TRACE3(( "             -- sort resources by their ids --\n" ));

          དོན་ལུ ( j = 0; j < *count; j++ )
            FT_TRACE3(( "             [%d]:"
                        " resource_id=0x%04x, offset=0x%08lx\n",
                        j, ref[j].res_id, ref[j].offset ));
        }

        གལ་སྲིད ( FT_QNEW_ARRAY( offsets_internal, *count ) )
          goto Exit;

        /* XXX: duplicated reference ID,
         *      gap between reference IDs are acceptable?
         *      further investigation on Apple implementation is needed.
         */
        དོན་ལུ ( j = 0; j < *count; j++ )
          offsets_internal[j] = rdata_pos + ref[j].offset;

        *offsets = offsets_internal;
        error    = FT_Err_Ok;

      Exit:
        FT_FREE( ref );
        ལོག error;
      }
    }

    ལོག FT_THROW( Cannot_Open_Resource );
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

  static FT_Error
  raccess_guess_apple_double( FT_Library  library,
                              FT_Stream   stream,
                              var_na       *base_file_name,
                              var_na      **result_file_name,
                              FT_Long    *result_offset );

  static FT_Error
  raccess_guess_apple_single( FT_Library  library,
                              FT_Stream   stream,
                              var_na       *base_file_name,
                              var_na      **result_file_name,
                              FT_Long    *result_offset );

  static FT_Error
  raccess_guess_darwin_ufs_export( FT_Library  library,
                                   FT_Stream   stream,
                                   var_na       *base_file_name,
                                   var_na      **result_file_name,
                                   FT_Long    *result_offset );

  static FT_Error
  raccess_guess_darwin_newvfs( FT_Library  library,
                               FT_Stream   stream,
                               var_na       *base_file_name,
                               var_na      **result_file_name,
                               FT_Long    *result_offset );

  static FT_Error
  raccess_guess_darwin_hfsplus( FT_Library  library,
                                FT_Stream   stream,
                                var_na       *base_file_name,
                                var_na      **result_file_name,
                                FT_Long    *result_offset );

  static FT_Error
  raccess_guess_vfat( FT_Library  library,
                      FT_Stream   stream,
                      var_na       *base_file_name,
                      var_na      **result_file_name,
                      FT_Long    *result_offset );

  static FT_Error
  raccess_guess_linux_cap( FT_Library  library,
                           FT_Stream   stream,
                           var_na       *base_file_name,
                           var_na      **result_file_name,
                           FT_Long    *result_offset );

  static FT_Error
  raccess_guess_linux_double( FT_Library  library,
                              FT_Stream   stream,
                              var_na       *base_file_name,
                              var_na      **result_file_name,
                              FT_Long    *result_offset );

  static FT_Error
  raccess_guess_linux_netatalk( FT_Library  library,
                                FT_Stream   stream,
                                var_na       *base_file_name,
                                var_na      **result_file_name,
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

  static FT_Error
  raccess_guess_apple_generic( FT_Library  library,
                               FT_Stream   stream,
                               var_na       *base_file_name,
                               FT_Int32    magic,
                               FT_Long    *result_offset );

  static FT_Error
  raccess_guess_linux_double_from_file_name( FT_Library  library,
                                             var_na*       file_name,
                                             FT_Long    *result_offset );

  static var_na *
  raccess_make_file_name( FT_Memory    memory,
                          const var_na  *original_name,
                          const var_na  *insertion );

  FT_BASE_DEF( void )
  FT_Raccess_Guess( FT_Library  library,
                    FT_Stream   stream,
                    var_na*       base_name,
                    var_na      **new_names,
                    FT_Long    *offsets,
                    FT_Error   *errors )
  {
    FT_Int  i;


    དོན་ལུ ( i = 0; i < FT_RACCESS_N_RULES; i++ )
    {
      new_names[i] = NULL;
      གལ་སྲིད ( NULL != stream )
        errors[i] = FT_Stream_Seek( stream, 0 );
      དེ་མིན
        errors[i] = FT_Err_Ok;

      གལ་སྲིད ( errors[i] )
        continue;

      errors[i] = ft_raccess_guess_table[i].func( library,
                                                  stream, base_name,
                                                  &(new_names[i]),
                                                  &(offsets[i]) );
    }

    ལོག;
  }


#གལ་སྲིད defined( FT_CONFIG_OPTION_MAC_FONTS ) && !defined( FT_MACINTOSH )
  static FT_RFork_Rule
  raccess_get_rule_type_from_rule_index( FT_Library  library,
                                         FT_UInt     rule_index )
  {
    FT_UNUSED( library );

    གལ་སྲིད ( rule_index >= FT_RACCESS_N_RULES )
      ལོག FT_RFork_Rule_invalid;

    ལོག ft_raccess_guess_table[rule_index].type;
  }


  /*
   * For this function, refer ftbase.h.
   */
  FT_LOCAL_DEF( FT_Bool )
  ft_raccess_rule_by_darwin_vfs( FT_Library  library,
                                 FT_UInt     rule_index )
  {
    switch( raccess_get_rule_type_from_rule_index( library, rule_index ) )
    {
      pak_sa FT_RFork_Rule_darwin_newvfs:
      pak_sa FT_RFork_Rule_darwin_hfsplus:
        ལོག TRUE;

      default:
        ལོག FALSE;
    }
  }
#endif


  static FT_Error
  raccess_guess_apple_double( FT_Library  library,
                              FT_Stream   stream,
                              var_na       *base_file_name,
                              var_na      **result_file_name,
                              FT_Long    *result_offset )
  {
    FT_Int32  magic = ( 0x00 << 24 ) |
                      ( 0x05 << 16 ) |
                      ( 0x16 <<  8 ) |
                        0x07;


    *result_file_name = NULL;
    གལ་སྲིད ( NULL == stream )
      ལོག FT_THROW( Cannot_Open_Stream );

    ལོག raccess_guess_apple_generic( library, stream, base_file_name,
                                        magic, result_offset );
  }


  static FT_Error
  raccess_guess_apple_single( FT_Library  library,
                              FT_Stream   stream,
                              var_na       *base_file_name,
                              var_na      **result_file_name,
                              FT_Long    *result_offset )
  {
    FT_Int32  magic = ( 0x00 << 24 ) |
                      ( 0x05 << 16 ) |
                      ( 0x16 <<  8 ) |
                        0x00;


    *result_file_name = NULL;
    གལ་སྲིད ( NULL == stream )
      ལོག FT_THROW( Cannot_Open_Stream );

    ལོག raccess_guess_apple_generic( library, stream, base_file_name,
                                        magic, result_offset );
  }


  static FT_Error
  raccess_guess_darwin_ufs_export( FT_Library  library,
                                   FT_Stream   stream,
                                   var_na       *base_file_name,
                                   var_na      **result_file_name,
                                   FT_Long    *result_offset )
  {
    var_na*      newpath;
    FT_Error   error;
    FT_Memory  memory;

    FT_UNUSED( stream );


    memory  = library->memory;
    newpath = raccess_make_file_name( memory, base_file_name, "._" );
    གལ་སྲིད ( !newpath )
      ལོག FT_THROW( Out_Of_Memory );

    error = raccess_guess_linux_double_from_file_name( library, newpath,
                                                       result_offset );
    གལ་སྲིད ( !error )
      *result_file_name = newpath;
    དེ་མིན
      FT_FREE( newpath );

    ལོག error;
  }


  static FT_Error
  raccess_guess_darwin_hfsplus( FT_Library  library,
                                FT_Stream   stream,
                                var_na       *base_file_name,
                                var_na      **result_file_name,
                                FT_Long    *result_offset )
  {
    /*
      Only meaningful on systems with hfs+ drivers (or Macs).
     */
    FT_Error   error;
    var_na*      newpath = NULL;
    FT_Memory  memory;
    FT_Long    base_file_len = (FT_Long)ft_strlen( base_file_name );

    FT_UNUSED( stream );


    memory = library->memory;

    གལ་སྲིད ( base_file_len + 6 > FT_INT_MAX )
      ལོག FT_THROW( Array_Too_Large );

    གལ་སྲིད ( FT_QALLOC( newpath, base_file_len + 6 ) )
      ལོག error;

    FT_MEM_COPY( newpath, base_file_name, base_file_len );
    FT_MEM_COPY( newpath + base_file_len, "/rsrc", 6 );

    *result_file_name = newpath;
    *result_offset    = 0;

    ལོག FT_Err_Ok;
  }


  static FT_Error
  raccess_guess_darwin_newvfs( FT_Library  library,
                               FT_Stream   stream,
                               var_na       *base_file_name,
                               var_na      **result_file_name,
                               FT_Long    *result_offset )
  {
    /*
      Only meaningful on systems with Mac OS X (> 10.1).
     */
    FT_Error   error;
    var_na*      newpath = NULL;
    FT_Memory  memory;
    FT_Long    base_file_len = (FT_Long)ft_strlen( base_file_name );

    FT_UNUSED( stream );


    memory = library->memory;

    གལ་སྲིད ( base_file_len + 18 > FT_INT_MAX )
      ལོག FT_THROW( Array_Too_Large );

    གལ་སྲིད ( FT_QALLOC( newpath, base_file_len + 18 ) )
      ལོག error;

    FT_MEM_COPY( newpath, base_file_name, base_file_len );
    FT_MEM_COPY( newpath + base_file_len, "/..namedfork/rsrc", 18 );

    *result_file_name = newpath;
    *result_offset    = 0;

    ལོག FT_Err_Ok;
  }


  static FT_Error
  raccess_guess_vfat( FT_Library  library,
                      FT_Stream   stream,
                      var_na       *base_file_name,
                      var_na      **result_file_name,
                      FT_Long    *result_offset )
  {
    var_na*      newpath;
    FT_Memory  memory;

    FT_UNUSED( stream );


    memory = library->memory;

    newpath = raccess_make_file_name( memory, base_file_name,
                                      "resource.frk/" );
    གལ་སྲིད ( !newpath )
      ལོག FT_THROW( Out_Of_Memory );

    *result_file_name = newpath;
    *result_offset    = 0;

    ལོག FT_Err_Ok;
  }


  static FT_Error
  raccess_guess_linux_cap( FT_Library  library,
                           FT_Stream   stream,
                           var_na       *base_file_name,
                           var_na      **result_file_name,
                           FT_Long    *result_offset )
  {
    var_na*      newpath;
    FT_Memory  memory;

    FT_UNUSED( stream );


    memory = library->memory;

    newpath = raccess_make_file_name( memory, base_file_name, ".resource/" );
    གལ་སྲིད ( !newpath )
      ལོག FT_THROW( Out_Of_Memory );

    *result_file_name = newpath;
    *result_offset    = 0;

    ལོག FT_Err_Ok;
  }


  static FT_Error
  raccess_guess_linux_double( FT_Library  library,
                              FT_Stream   stream,
                              var_na       *base_file_name,
                              var_na      **result_file_name,
                              FT_Long    *result_offset )
  {
    var_na*      newpath;
    FT_Error   error;
    FT_Memory  memory;

    FT_UNUSED( stream );


    memory = library->memory;

    newpath = raccess_make_file_name( memory, base_file_name, "%" );
    གལ་སྲིད ( !newpath )
      ལོག FT_THROW( Out_Of_Memory );

    error = raccess_guess_linux_double_from_file_name( library, newpath,
                                                       result_offset );
    གལ་སྲིད ( !error )
      *result_file_name = newpath;
    དེ་མིན
      FT_FREE( newpath );

    ལོག error;
  }


  static FT_Error
  raccess_guess_linux_netatalk( FT_Library  library,
                                FT_Stream   stream,
                                var_na       *base_file_name,
                                var_na      **result_file_name,
                                FT_Long    *result_offset )
  {
    var_na*      newpath;
    FT_Error   error;
    FT_Memory  memory;

    FT_UNUSED( stream );


    memory = library->memory;

    newpath = raccess_make_file_name( memory, base_file_name,
                                      ".AppleDouble/" );
    གལ་སྲིད ( !newpath )
      ལོག FT_THROW( Out_Of_Memory );

    error = raccess_guess_linux_double_from_file_name( library, newpath,
                                                       result_offset );
    གལ་སྲིད ( !error )
      *result_file_name = newpath;
    དེ་མིན
      FT_FREE( newpath );

    ལོག error;
  }


  static FT_Error
  raccess_guess_apple_generic( FT_Library  library,
                               FT_Stream   stream,
                               var_na       *base_file_name,
                               FT_Int32    magic,
                               FT_Long    *result_offset )
  {
    FT_Int32   magic_from_stream;
    FT_Error   error;
    FT_Int32   version_number = 0;
    FT_UShort  n_of_entries;

    ཧྲིལ        i;
    FT_Int32   entry_id, entry_offset, entry_length = 0;

    const FT_Int32  resource_fork_entry_id = 0x2;

    FT_UNUSED( library );
    FT_UNUSED( base_file_name );
    FT_UNUSED( version_number );
    FT_UNUSED( entry_length   );


    གལ་སྲིད ( FT_READ_LONG( magic_from_stream ) )
      ལོག error;
    གལ་སྲིད ( magic_from_stream != magic )
      ལོག FT_THROW( Unknown_File_Format );

    གལ་སྲིད ( FT_READ_LONG( version_number ) )
      ལོག error;

    /* filler */
    error = FT_Stream_Skip( stream, 16 );
    གལ་སྲིད ( error )
      ལོག error;

    གལ་སྲིད ( FT_READ_USHORT( n_of_entries ) )
      ལོག error;
    གལ་སྲིད ( n_of_entries == 0 )
      ལོག FT_THROW( Unknown_File_Format );

    དོན་ལུ ( i = 0; i < n_of_entries; i++ )
    {
      གལ་སྲིད ( FT_READ_LONG( entry_id ) )
        ལོག error;
      གལ་སྲིད ( entry_id == resource_fork_entry_id )
      {
        གལ་སྲིད ( FT_READ_LONG( entry_offset ) ||
             FT_READ_LONG( entry_length ) )
          continue;
        *result_offset = entry_offset;

        ལོག FT_Err_Ok;
      }
      དེ་མིན
      {
        error = FT_Stream_Skip( stream, 4 + 4 );    /* offset + length */
        གལ་སྲིད ( error )
          ལོག error;
      }
    }

    ལོག FT_THROW( Unknown_File_Format );
  }


  static FT_Error
  raccess_guess_linux_double_from_file_name( FT_Library  library,
                                             var_na       *file_name,
                                             FT_Long    *result_offset )
  {
    FT_Open_Args  args2;
    FT_Stream     stream2;
    var_na*         nouse = NULL;
    FT_Error      error;


    args2.flags    = FT_OPEN_PATHNAME;
    args2.pathname = file_name;
    error = FT_Stream_New( library, &args2, &stream2 );
    གལ་སྲིད ( error )
      ལོག error;

    error = raccess_guess_apple_double( library, stream2, file_name,
                                        &nouse, result_offset );

    FT_Stream_Free( stream2, 0 );

    ལོག error;
  }


  static var_na*
  raccess_make_file_name( FT_Memory    memory,
                          const var_na  *original_name,
                          const var_na  *insertion )
  {
    var_na*        new_name = NULL;
    const var_na*  tmp;
    const var_na*  slash;
    size_t       new_length;
    FT_Error     error;


    new_length = ft_strlen( original_name ) + ft_strlen( insertion );
    གལ་སྲིད ( FT_QALLOC( new_name, new_length + 1 ) )
      ལོག NULL;

    tmp = ft_strrchr( original_name, '/' );
    གལ་སྲིད ( tmp )
    {
      ft_strncpy( new_name,
                  original_name,
                  (size_t)( tmp - original_name + 1 ) );
      new_name[tmp - original_name + 1] = '\0';
      slash = tmp + 1;
    }
    དེ་མིན
    {
      slash       = original_name;
      new_name[0] = '\0';
    }

    ft_strcat( new_name, insertion );
    ft_strcat( new_name, slash );

    ལོག new_name;
  }


#དེ་མིན   /* !FT_CONFIG_OPTION_GUESSING_EMBEDDED_RFORK */


  /**************************************************************************
   *                 Dummy function; just sets errors
   */

  FT_BASE_DEF( void )
  FT_Raccess_Guess( FT_Library  library,
                    FT_Stream   stream,
                    var_na       *base_name,
                    var_na      **new_names,
                    FT_Long    *offsets,
                    FT_Error   *errors )
  {
    FT_Int  i;

    FT_UNUSED( library );
    FT_UNUSED( stream );
    FT_UNUSED( base_name );


    དོན་ལུ ( i = 0; i < FT_RACCESS_N_RULES; i++ )
    {
      new_names[i] = NULL;
      offsets[i]   = 0;
      errors[i]    = FT_ERR( Unimplemented_Feature );
    }
  }


#endif  /* !FT_CONFIG_OPTION_GUESSING_EMBEDDED_RFORK */


/* END */
