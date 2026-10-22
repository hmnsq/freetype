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


#ضمن <freetype/internal/ftdebug.h>
#ضمن <freetype/internal/ftstream.h>
#ضمن <freetype/internal/ftrfork.h>

#ضمن "ftbase.h"

#وصف_رد  FT_COMPONENT
#وصف_ڪريو FT_COMPONENT  raccess


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

  FT_بنياد_DEF( FT_Error )
  FT_Raccess_Get_سرو_Info( FT_Library  library,
                             FT_وهڪرو   وهڪرو,
                             FT_Long     rfork_وٿي,
                             FT_Long    *map_وٿي,
                             FT_Long    *rdata_pos )
  {
    FT_Error       error;
    بنانشان حرف  سر[16], head2[16];
    FT_Long        map_pos, map_len, rdata_len;
    صحيح            allzeros, allmatch, i;
    FT_Long        type_list;

    FT_اڻ_استعمال_ٿيل( library );


    error = FT_وهڪرو_Seek( وهڪرو, (FT_ULong)rfork_وٿي );
    إذا ( error )
      أرجع error;

    error = FT_وهڪرو_Read( وهڪرو, (FT_Byte*)سر, 16 );
    إذا ( error )
      أرجع error;

    /* ensure positive values */
    إذا ( سر[0]  >= 0x80 ||
         سر[4]  >= 0x80 ||
         سر[8]  >= 0x80 ||
         سر[12] >= 0x80 )
      أرجع FT_THROW( Unknown_File_Format );

    *rdata_pos = ( سر[ 0] << 24 ) |
                 ( سر[ 1] << 16 ) |
                 ( سر[ 2] <<  8 ) |
                   سر[ 3];
    map_pos    = ( سر[ 4] << 24 ) |
                 ( سر[ 5] << 16 ) |
                 ( سر[ 6] <<  8 ) |
                   سر[ 7];
    rdata_len  = ( سر[ 8] << 24 ) |
                 ( سر[ 9] << 16 ) |
                 ( سر[10] <<  8 ) |
                   سر[11];
    map_len    = ( سر[12] << 24 ) |
                 ( سر[13] << 16 ) |
                 ( سر[14] <<  8 ) |
                   سر[15];

    /* the map must not be empty */
    إذا ( !map_pos )
      أرجع FT_THROW( Unknown_File_Format );

    /* check whether rdata and map overlap */
    إذا ( *rdata_pos < map_pos )
    {
      إذا ( *rdata_pos > map_pos - rdata_len )
        أرجع FT_THROW( Unknown_File_Format );
    }
    وإلا
    {
      إذا ( map_pos > *rdata_pos - map_len )
        أرجع FT_THROW( Unknown_File_Format );
    }

    /* check whether end of rdata or map exceeds stream size */
    إذا ( FT_LONG_MAX - rdata_len < *rdata_pos                               ||
         FT_LONG_MAX - map_len < map_pos                                    ||

         FT_LONG_MAX - ( *rdata_pos + rdata_len ) < rfork_وٿي            ||
         FT_LONG_MAX - ( map_pos + map_len ) < rfork_وٿي                 ||

         (FT_ULong)( rfork_وٿي + *rdata_pos + rdata_len ) > وهڪرو->سائيز ||
         (FT_ULong)( rfork_وٿي + map_pos + map_len ) > وهڪرو->سائيز      )
      أرجع FT_THROW( Unknown_File_Format );

    *rdata_pos += rfork_وٿي;
    map_pos    += rfork_وٿي;

    error = FT_وهڪرو_Seek( وهڪرو, (FT_ULong)map_pos );
    إذا ( error )
      أرجع error;

    head2[15] = (FT_Byte)( سر[15] + 1 );       /* make it be different */

    error = FT_وهڪرو_Read( وهڪرو, (FT_Byte*)head2, 16 );
    إذا ( error )
      أرجع error;

    allzeros = 1;
    allmatch = 1;
    لكل ( i = 0; i < 16; i++ )
    {
      إذا ( head2[i] != 0 )
        allzeros = 0;
      إذا ( head2[i] != سر[i] )
        allmatch = 0;
    }
    إذا ( !allzeros && !allmatch )
      أرجع FT_THROW( Unknown_File_Format );

    /* If we have reached this point then it is probably a mac resource */
    /* file.  Now, does it contain any interesting resources?           */

    (فراغ)FT_وهڪرو_SKIP( 4        /* skip handle to next resource map */
                          + 2      /* skip file resource number */
                          + 2 );   /* skip attributes */

    إذا ( FT_READ_SHORT( type_list ) )
      أرجع error;
    إذا ( type_list < 0 )
      أرجع FT_THROW( Unknown_File_Format );

    error = FT_وهڪرو_Seek( وهڪرو, (FT_ULong)( map_pos + type_list ) );
    إذا ( error )
      أرجع error;

    *map_وٿي = map_pos + type_list;
    أرجع FT_Err_Ok;
  }


  FT_COMPARE_DEF( صحيح )
  ft_raccess_ترتيب_ڏيو_ref_by_id( ثابت فراغ*  a,
                             ثابت فراغ*  b )
  {
    أرجع  ( (FT_RFork_Ref*)a )->res_id - ( (FT_RFork_Ref*)b )->res_id;
  }


  FT_بنياد_DEF( FT_Error )
  FT_Raccess_Get_معلومات_Offsets( FT_Library  library,
                              FT_وهڪرو   وهڪرو,
                              FT_Long     map_وٿي,
                              FT_Long     rdata_pos,
                              FT_Long     ٽيگ,
                              FT_Bool     ترتيب_ڏيو_by_res_id,
                              FT_Long   **offsets,
                              FT_Long    *ڳڻپ )
  {
    FT_Error      error;
    صحيح           i, j, cnt, subcnt;
    FT_Long       ٽيگ_اندروني, rpos;
    FT_يادگيري     يادگيري = library->يادگيري;
    FT_Long       عارضي;
    FT_Long       *offsets_اندروني = عدم;
    FT_RFork_Ref  *ref = عدم;


    FT_TRACE3(( "\n" ));
    error = FT_وهڪرو_Seek( وهڪرو, (FT_ULong)map_وٿي );
    إذا ( error )
      أرجع error;

    إذا ( FT_READ_SHORT( cnt ) )
      أرجع error;
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
    إذا ( cnt > 4079 )
      أرجع FT_THROW( Invalid_جدول );

    لكل ( i = 0; i < cnt; i++ )
    {
      إذا ( FT_READ_LONG( ٽيگ_اندروني ) ||
           FT_READ_SHORT( subcnt )      ||
           FT_READ_SHORT( rpos )        )
        أرجع error;

      FT_TRACE2(( "Resource tags: %c%c%c%c\n",
                  (حرف)( 0xFF & ( ٽيگ_اندروني >> 24 ) ),
                  (حرف)( 0xFF & ( ٽيگ_اندروني >> 16 ) ),
                  (حرف)( 0xFF & ( ٽيگ_اندروني >>  8 ) ),
                  (حرف)( 0xFF & ( ٽيگ_اندروني >>  0 ) ) ));
      FT_TRACE3(( "             : subcount=%d, suboffset=0x%04lx\n",
                  subcnt, rpos ));

      إذا ( ٽيگ_اندروني == ٽيگ )
      {
        *ڳڻپ = subcnt + 1;
        rpos  += map_وٿي;

        /* a zero count might be valid in the resource specification, */
        /* however, it is completely useless to us                    */
        إذا ( *ڳڻپ < 1 || *ڳڻپ > 2727 )
          أرجع FT_THROW( Invalid_جدول );

        error = FT_وهڪرو_Seek( وهڪرو, (FT_ULong)rpos );
        إذا ( error )
          أرجع error;

        إذا ( FT_QNEW_ARRAY( ref, *ڳڻپ ) )
          أرجع error;

        لكل ( j = 0; j < *ڳڻپ; j++ )
        {
          إذا ( FT_READ_SHORT( ref[j].res_id ) )
            اذهب Exit;
          إذا ( FT_وهڪرو_SKIP( 2 ) )  /* resource name offset */
            اذهب Exit;
          إذا ( FT_READ_LONG( عارضي ) ) /* attributes (8bit), offset (24bit) */
            اذهب Exit;
          إذا ( FT_وهڪرو_SKIP( 4 ) )  /* mbz */
            اذهب Exit;

          /*
           * According to Inside Macintosh: More Macintosh Toolbox,
           * "Resource IDs" (1-46), there are some reserved IDs.
           * However, FreeType2 is not a font synthesizer, no need
           * to check the acceptable resource ID or its attributes.
           */
          ref[j].وٿي = عارضي & 0xFFFFFFL;

          FT_TRACE3(( "             [%d]:"
                      " resource_id=0x%04x, offset=0x%08lx\n",
                      j, (FT_UShort)ref[j].res_id, ref[j].وٿي ));
        }

        إذا ( ترتيب_ڏيو_by_res_id )
        {
          ft_qsort( ref,
                    (size_t)*ڳڻپ,
                    حجم ( FT_RFork_Ref ),
                    ft_raccess_ترتيب_ڏيو_ref_by_id );

          FT_TRACE3(( "             -- sort resources by their ids --\n" ));

          لكل ( j = 0; j < *ڳڻپ; j++ )
            FT_TRACE3(( "             [%d]:"
                        " resource_id=0x%04x, offset=0x%08lx\n",
                        j, ref[j].res_id, ref[j].وٿي ));
        }

        إذا ( FT_QNEW_ARRAY( offsets_اندروني, *ڳڻپ ) )
          اذهب Exit;

        /* XXX: duplicated reference ID,
         *      gap between reference IDs are acceptable?
         *      further investigation on Apple implementation is needed.
         */
        لكل ( j = 0; j < *ڳڻپ; j++ )
          offsets_اندروني[j] = rdata_pos + ref[j].وٿي;

        *offsets = offsets_اندروني;
        error    = FT_Err_Ok;

      Exit:
        FT_FREE( ref );
        أرجع error;
      }
    }

    أرجع FT_THROW( Cannot_Open_Resource );
  }


#جيڪڏهن_وصف_ٿيل FT_CONFIG_OPTION_GUESSING_EMBEDDED_RFORK

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

  ساكن FT_Error
  raccess_guess_apple_double( FT_Library  library,
                              FT_وهڪرو   وهڪرو,
                              حرف       *بنياد_file_name,
                              حرف      **نتيجو_file_name,
                              FT_Long    *نتيجو_وٿي );

  ساكن FT_Error
  raccess_guess_apple_single( FT_Library  library,
                              FT_وهڪرو   وهڪرو,
                              حرف       *بنياد_file_name,
                              حرف      **نتيجو_file_name,
                              FT_Long    *نتيجو_وٿي );

  ساكن FT_Error
  raccess_guess_darwin_ufs_export( FT_Library  library,
                                   FT_وهڪرو   وهڪرو,
                                   حرف       *بنياد_file_name,
                                   حرف      **نتيجو_file_name,
                                   FT_Long    *نتيجو_وٿي );

  ساكن FT_Error
  raccess_guess_darwin_newvfs( FT_Library  library,
                               FT_وهڪرو   وهڪرو,
                               حرف       *بنياد_file_name,
                               حرف      **نتيجو_file_name,
                               FT_Long    *نتيجو_وٿي );

  ساكن FT_Error
  raccess_guess_darwin_hfsplus( FT_Library  library,
                                FT_وهڪرو   وهڪرو,
                                حرف       *بنياد_file_name,
                                حرف      **نتيجو_file_name,
                                FT_Long    *نتيجو_وٿي );

  ساكن FT_Error
  raccess_guess_vfat( FT_Library  library,
                      FT_وهڪرو   وهڪرو,
                      حرف       *بنياد_file_name,
                      حرف      **نتيجو_file_name,
                      FT_Long    *نتيجو_وٿي );

  ساكن FT_Error
  raccess_guess_linux_cap( FT_Library  library,
                           FT_وهڪرو   وهڪرو,
                           حرف       *بنياد_file_name,
                           حرف      **نتيجو_file_name,
                           FT_Long    *نتيجو_وٿي );

  ساكن FT_Error
  raccess_guess_linux_double( FT_Library  library,
                              FT_وهڪرو   وهڪرو,
                              حرف       *بنياد_file_name,
                              حرف      **نتيجو_file_name,
                              FT_Long    *نتيجو_وٿي );

  ساكن FT_Error
  raccess_guess_linux_netatalk( FT_Library  library,
                                FT_وهڪرو   وهڪرو,
                                حرف       *بنياد_file_name,
                                حرف      **نتيجو_file_name,
                                FT_Long    *نتيجو_وٿي );


  CONST_FT_RFORK_قاعدو_ARRAY_BEGIN( ft_raccess_guess_جدول,
                                                      ft_raccess_guess_rec )
  CONST_FT_RFORK_قاعدو_ARRAY_داخلا( apple_double,      apple_double )
  CONST_FT_RFORK_قاعدو_ARRAY_داخلا( apple_single,      apple_single )
  CONST_FT_RFORK_قاعدو_ARRAY_داخلا( darwin_ufs_export, darwin_ufs_export )
  CONST_FT_RFORK_قاعدو_ARRAY_داخلا( darwin_newvfs,     darwin_newvfs )
  CONST_FT_RFORK_قاعدو_ARRAY_داخلا( darwin_hfsplus,    darwin_hfsplus )
  CONST_FT_RFORK_قاعدو_ARRAY_داخلا( vfat,              vfat )
  CONST_FT_RFORK_قاعدو_ARRAY_داخلا( linux_cap,         linux_cap )
  CONST_FT_RFORK_قاعدو_ARRAY_داخلا( linux_double,      linux_double )
  CONST_FT_RFORK_قاعدو_ARRAY_داخلا( linux_netatalk,    linux_netatalk )
  CONST_FT_RFORK_قاعدو_ARRAY_پڇاڙي


  /*************************************************************************/
  /****                                                                 ****/
  /****                       Helper functions                          ****/
  /****                                                                 ****/
  /*************************************************************************/

  ساكن FT_Error
  raccess_guess_apple_generic( FT_Library  library,
                               FT_وهڪرو   وهڪرو,
                               حرف       *بنياد_file_name,
                               FT_Int32    magic,
                               FT_Long    *نتيجو_وٿي );

  ساكن FT_Error
  raccess_guess_linux_double_from_file_name( FT_Library  library,
                                             حرف*       file_name,
                                             FT_Long    *نتيجو_وٿي );

  ساكن حرف *
  raccess_make_file_name( FT_يادگيري    يادگيري,
                          ثابت حرف  *original_name,
                          ثابت حرف  *insertion );

  FT_بنياد_DEF( فراغ )
  FT_Raccess_Guess( FT_Library  library,
                    FT_وهڪرو   وهڪرو,
                    حرف*       بنياد_name,
                    حرف      **new_نالا,
                    FT_Long    *offsets,
                    FT_Error   *errors )
  {
    FT_Int  i;


    لكل ( i = 0; i < FT_RACCESS_N_قاعدا; i++ )
    {
      new_نالا[i] = عدم;
      إذا ( عدم != وهڪرو )
        errors[i] = FT_وهڪرو_Seek( وهڪرو, 0 );
      وإلا
        errors[i] = FT_Err_Ok;

      إذا ( errors[i] )
        استمر;

      errors[i] = ft_raccess_guess_جدول[i].func( library,
                                                  وهڪرو, بنياد_name,
                                                  &(new_نالا[i]),
                                                  &(offsets[i]) );
    }

    أرجع;
  }


#إذا defined( FT_CONFIG_OPTION_MAC_FONTS ) && !defined( FT_MACINTOSH )
  ساكن FT_RFork_قاعدو
  raccess_get_قاعدو_type_from_قاعدو_اشاريو( FT_Library  library,
                                         FT_UInt     قاعدو_اشاريو )
  {
    FT_اڻ_استعمال_ٿيل( library );

    إذا ( قاعدو_اشاريو >= FT_RACCESS_N_قاعدا )
      أرجع FT_RFork_قاعدو_invalid;

    أرجع ft_raccess_guess_جدول[قاعدو_اشاريو].type;
  }


  /*
   * For this function, refer ftbase.h.
   */
  FT_LOCAL_DEF( FT_Bool )
  ft_raccess_قاعدو_by_darwin_vfs( FT_Library  library,
                                 FT_UInt     قاعدو_اشاريو )
  {
    بدل( raccess_get_قاعدو_type_from_قاعدو_اشاريو( library, قاعدو_اشاريو ) )
    {
      حالة FT_RFork_قاعدو_darwin_newvfs:
      حالة FT_RFork_قاعدو_darwin_hfsplus:
        أرجع TRUE;

      افتراضي:
        أرجع FALSE;
    }
  }
#جيڪڏهن_پورو


  ساكن FT_Error
  raccess_guess_apple_double( FT_Library  library,
                              FT_وهڪرو   وهڪرو,
                              حرف       *بنياد_file_name,
                              حرف      **نتيجو_file_name,
                              FT_Long    *نتيجو_وٿي )
  {
    FT_Int32  magic = ( 0x00 << 24 ) |
                      ( 0x05 << 16 ) |
                      ( 0x16 <<  8 ) |
                        0x07;


    *نتيجو_file_name = عدم;
    إذا ( عدم == وهڪرو )
      أرجع FT_THROW( Cannot_Open_وهڪرو );

    أرجع raccess_guess_apple_generic( library, وهڪرو, بنياد_file_name,
                                        magic, نتيجو_وٿي );
  }


  ساكن FT_Error
  raccess_guess_apple_single( FT_Library  library,
                              FT_وهڪرو   وهڪرو,
                              حرف       *بنياد_file_name,
                              حرف      **نتيجو_file_name,
                              FT_Long    *نتيجو_وٿي )
  {
    FT_Int32  magic = ( 0x00 << 24 ) |
                      ( 0x05 << 16 ) |
                      ( 0x16 <<  8 ) |
                        0x00;


    *نتيجو_file_name = عدم;
    إذا ( عدم == وهڪرو )
      أرجع FT_THROW( Cannot_Open_وهڪرو );

    أرجع raccess_guess_apple_generic( library, وهڪرو, بنياد_file_name,
                                        magic, نتيجو_وٿي );
  }


  ساكن FT_Error
  raccess_guess_darwin_ufs_export( FT_Library  library,
                                   FT_وهڪرو   وهڪرو,
                                   حرف       *بنياد_file_name,
                                   حرف      **نتيجو_file_name,
                                   FT_Long    *نتيجو_وٿي )
  {
    حرف*      newpath;
    FT_Error   error;
    FT_يادگيري  يادگيري;

    FT_اڻ_استعمال_ٿيل( وهڪرو );


    يادگيري  = library->يادگيري;
    newpath = raccess_make_file_name( يادگيري, بنياد_file_name, "._" );
    إذا ( !newpath )
      أرجع FT_THROW( ٻاهريون_جو_يادگيري );

    error = raccess_guess_linux_double_from_file_name( library, newpath,
                                                       نتيجو_وٿي );
    إذا ( !error )
      *نتيجو_file_name = newpath;
    وإلا
      FT_FREE( newpath );

    أرجع error;
  }


  ساكن FT_Error
  raccess_guess_darwin_hfsplus( FT_Library  library,
                                FT_وهڪرو   وهڪرو,
                                حرف       *بنياد_file_name,
                                حرف      **نتيجو_file_name,
                                FT_Long    *نتيجو_وٿي )
  {
    /*
      Only meaningful on systems with hfs+ drivers (or Macs).
     */
    FT_Error   error;
    حرف*      newpath = عدم;
    FT_يادگيري  يادگيري;
    FT_Long    بنياد_file_len = (FT_Long)ft_strlen( بنياد_file_name );

    FT_اڻ_استعمال_ٿيل( وهڪرو );


    يادگيري = library->يادگيري;

    إذا ( بنياد_file_len + 6 > FT_INT_MAX )
      أرجع FT_THROW( Array_Too_Large );

    إذا ( FT_QALLOC( newpath, بنياد_file_len + 6 ) )
      أرجع error;

    FT_MEM_COPY( newpath, بنياد_file_name, بنياد_file_len );
    FT_MEM_COPY( newpath + بنياد_file_len, "/rsrc", 6 );

    *نتيجو_file_name = newpath;
    *نتيجو_وٿي    = 0;

    أرجع FT_Err_Ok;
  }


  ساكن FT_Error
  raccess_guess_darwin_newvfs( FT_Library  library,
                               FT_وهڪرو   وهڪرو,
                               حرف       *بنياد_file_name,
                               حرف      **نتيجو_file_name,
                               FT_Long    *نتيجو_وٿي )
  {
    /*
      Only meaningful on systems with Mac OS X (> 10.1).
     */
    FT_Error   error;
    حرف*      newpath = عدم;
    FT_يادگيري  يادگيري;
    FT_Long    بنياد_file_len = (FT_Long)ft_strlen( بنياد_file_name );

    FT_اڻ_استعمال_ٿيل( وهڪرو );


    يادگيري = library->يادگيري;

    إذا ( بنياد_file_len + 18 > FT_INT_MAX )
      أرجع FT_THROW( Array_Too_Large );

    إذا ( FT_QALLOC( newpath, بنياد_file_len + 18 ) )
      أرجع error;

    FT_MEM_COPY( newpath, بنياد_file_name, بنياد_file_len );
    FT_MEM_COPY( newpath + بنياد_file_len, "/..namedfork/rsrc", 18 );

    *نتيجو_file_name = newpath;
    *نتيجو_وٿي    = 0;

    أرجع FT_Err_Ok;
  }


  ساكن FT_Error
  raccess_guess_vfat( FT_Library  library,
                      FT_وهڪرو   وهڪرو,
                      حرف       *بنياد_file_name,
                      حرف      **نتيجو_file_name,
                      FT_Long    *نتيجو_وٿي )
  {
    حرف*      newpath;
    FT_يادگيري  يادگيري;

    FT_اڻ_استعمال_ٿيل( وهڪرو );


    يادگيري = library->يادگيري;

    newpath = raccess_make_file_name( يادگيري, بنياد_file_name,
                                      "resource.frk/" );
    إذا ( !newpath )
      أرجع FT_THROW( ٻاهريون_جو_يادگيري );

    *نتيجو_file_name = newpath;
    *نتيجو_وٿي    = 0;

    أرجع FT_Err_Ok;
  }


  ساكن FT_Error
  raccess_guess_linux_cap( FT_Library  library,
                           FT_وهڪرو   وهڪرو,
                           حرف       *بنياد_file_name,
                           حرف      **نتيجو_file_name,
                           FT_Long    *نتيجو_وٿي )
  {
    حرف*      newpath;
    FT_يادگيري  يادگيري;

    FT_اڻ_استعمال_ٿيل( وهڪرو );


    يادگيري = library->يادگيري;

    newpath = raccess_make_file_name( يادگيري, بنياد_file_name, ".resource/" );
    إذا ( !newpath )
      أرجع FT_THROW( ٻاهريون_جو_يادگيري );

    *نتيجو_file_name = newpath;
    *نتيجو_وٿي    = 0;

    أرجع FT_Err_Ok;
  }


  ساكن FT_Error
  raccess_guess_linux_double( FT_Library  library,
                              FT_وهڪرو   وهڪرو,
                              حرف       *بنياد_file_name,
                              حرف      **نتيجو_file_name,
                              FT_Long    *نتيجو_وٿي )
  {
    حرف*      newpath;
    FT_Error   error;
    FT_يادگيري  يادگيري;

    FT_اڻ_استعمال_ٿيل( وهڪرو );


    يادگيري = library->يادگيري;

    newpath = raccess_make_file_name( يادگيري, بنياد_file_name, "%" );
    إذا ( !newpath )
      أرجع FT_THROW( ٻاهريون_جو_يادگيري );

    error = raccess_guess_linux_double_from_file_name( library, newpath,
                                                       نتيجو_وٿي );
    إذا ( !error )
      *نتيجو_file_name = newpath;
    وإلا
      FT_FREE( newpath );

    أرجع error;
  }


  ساكن FT_Error
  raccess_guess_linux_netatalk( FT_Library  library,
                                FT_وهڪرو   وهڪرو,
                                حرف       *بنياد_file_name,
                                حرف      **نتيجو_file_name,
                                FT_Long    *نتيجو_وٿي )
  {
    حرف*      newpath;
    FT_Error   error;
    FT_يادگيري  يادگيري;

    FT_اڻ_استعمال_ٿيل( وهڪرو );


    يادگيري = library->يادگيري;

    newpath = raccess_make_file_name( يادگيري, بنياد_file_name,
                                      ".AppleDouble/" );
    إذا ( !newpath )
      أرجع FT_THROW( ٻاهريون_جو_يادگيري );

    error = raccess_guess_linux_double_from_file_name( library, newpath,
                                                       نتيجو_وٿي );
    إذا ( !error )
      *نتيجو_file_name = newpath;
    وإلا
      FT_FREE( newpath );

    أرجع error;
  }


  ساكن FT_Error
  raccess_guess_apple_generic( FT_Library  library,
                               FT_وهڪرو   وهڪرو,
                               حرف       *بنياد_file_name,
                               FT_Int32    magic,
                               FT_Long    *نتيجو_وٿي )
  {
    FT_Int32   magic_from_وهڪرو;
    FT_Error   error;
    FT_Int32   نسخو_عدد = 0;
    FT_UShort  n_جو_entries;

    صحيح        i;
    FT_Int32   داخلا_id, داخلا_وٿي, داخلا_ڊيگهائي = 0;

    ثابت FT_Int32  resource_fork_داخلا_id = 0x2;

    FT_اڻ_استعمال_ٿيل( library );
    FT_اڻ_استعمال_ٿيل( بنياد_file_name );
    FT_اڻ_استعمال_ٿيل( نسخو_عدد );
    FT_اڻ_استعمال_ٿيل( داخلا_ڊيگهائي   );


    إذا ( FT_READ_LONG( magic_from_وهڪرو ) )
      أرجع error;
    إذا ( magic_from_وهڪرو != magic )
      أرجع FT_THROW( Unknown_File_Format );

    إذا ( FT_READ_LONG( نسخو_عدد ) )
      أرجع error;

    /* filler */
    error = FT_وهڪرو_Skip( وهڪرو, 16 );
    إذا ( error )
      أرجع error;

    إذا ( FT_READ_USHORT( n_جو_entries ) )
      أرجع error;
    إذا ( n_جو_entries == 0 )
      أرجع FT_THROW( Unknown_File_Format );

    لكل ( i = 0; i < n_جو_entries; i++ )
    {
      إذا ( FT_READ_LONG( داخلا_id ) )
        أرجع error;
      إذا ( داخلا_id == resource_fork_داخلا_id )
      {
        إذا ( FT_READ_LONG( داخلا_وٿي ) ||
             FT_READ_LONG( داخلا_ڊيگهائي ) )
          استمر;
        *نتيجو_وٿي = داخلا_وٿي;

        أرجع FT_Err_Ok;
      }
      وإلا
      {
        error = FT_وهڪرو_Skip( وهڪرو, 4 + 4 );    /* offset + length */
        إذا ( error )
          أرجع error;
      }
    }

    أرجع FT_THROW( Unknown_File_Format );
  }


  ساكن FT_Error
  raccess_guess_linux_double_from_file_name( FT_Library  library,
                                             حرف       *file_name,
                                             FT_Long    *نتيجو_وٿي )
  {
    FT_Open_دليلون  args2;
    FT_وهڪرو     stream2;
    حرف*         nouse = عدم;
    FT_Error      error;


    args2.flags    = FT_OPEN_PATHNAME;
    args2.pathname = file_name;
    error = FT_وهڪرو_New( library, &args2, &stream2 );
    إذا ( error )
      أرجع error;

    error = raccess_guess_apple_double( library, stream2, file_name,
                                        &nouse, نتيجو_وٿي );

    FT_وهڪرو_Free( stream2, 0 );

    أرجع error;
  }


  ساكن حرف*
  raccess_make_file_name( FT_يادگيري    يادگيري,
                          ثابت حرف  *original_name,
                          ثابت حرف  *insertion )
  {
    حرف*        new_name = عدم;
    ثابت حرف*  tmp;
    ثابت حرف*  slash;
    size_t       new_ڊيگهائي;
    FT_Error     error;


    new_ڊيگهائي = ft_strlen( original_name ) + ft_strlen( insertion );
    إذا ( FT_QALLOC( new_name, new_ڊيگهائي + 1 ) )
      أرجع عدم;

    tmp = ft_strrchr( original_name, '/' );
    إذا ( tmp )
    {
      ft_strncpy( new_name,
                  original_name,
                  (size_t)( tmp - original_name + 1 ) );
      new_name[tmp - original_name + 1] = '\0';
      slash = tmp + 1;
    }
    وإلا
    {
      slash       = original_name;
      new_name[0] = '\0';
    }

    ft_strcat( new_name, insertion );
    ft_strcat( new_name, slash );

    أرجع new_name;
  }


#وإلا   /* !FT_CONFIG_OPTION_GUESSING_EMBEDDED_RFORK */


  /**************************************************************************
   *                 Dummy function; just sets errors
   */

  FT_بنياد_DEF( فراغ )
  FT_Raccess_Guess( FT_Library  library,
                    FT_وهڪرو   وهڪرو,
                    حرف       *بنياد_name,
                    حرف      **new_نالا,
                    FT_Long    *offsets,
                    FT_Error   *errors )
  {
    FT_Int  i;

    FT_اڻ_استعمال_ٿيل( library );
    FT_اڻ_استعمال_ٿيل( وهڪرو );
    FT_اڻ_استعمال_ٿيل( بنياد_name );


    لكل ( i = 0; i < FT_RACCESS_N_قاعدا; i++ )
    {
      new_نالا[i] = عدم;
      offsets[i]   = 0;
      errors[i]    = FT_ERR( Unimplemented_Feature );
    }
  }


#جيڪڏهن_پورو  /* !FT_CONFIG_OPTION_GUESSING_EMBEDDED_RFORK */


/*پڇاڙي*/
