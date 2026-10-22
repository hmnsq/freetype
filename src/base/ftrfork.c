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

#لغو_تعریف  FT_COMPONENT
#تعریف_کن FT_COMPONENT  raccess


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

  FT_پایه_DEF( FT_Error )
  FT_Raccess_Get_سرآیند_Info( FT_Library  library,
                             FT_جریان   جریان,
                             FT_Long     rfork_جابه_جایی,
                             FT_Long    *map_جابه_جایی,
                             FT_Long    *rdata_pos )
  {
    FT_Error       error;
    بیعلامت حرف  سر[16], head2[16];
    FT_Long        map_pos, map_len, rdata_len;
    صحيح            allzeros, allmatch, i;
    FT_Long        type_list;

    FT_استفادهنشده( library );


    error = FT_جریان_Seek( جریان, (FT_ULong)rfork_جابه_جایی );
    إذا ( error )
      أرجع error;

    error = FT_جریان_Read( جریان, (FT_Byte*)سر, 16 );
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

         FT_LONG_MAX - ( *rdata_pos + rdata_len ) < rfork_جابه_جایی            ||
         FT_LONG_MAX - ( map_pos + map_len ) < rfork_جابه_جایی                 ||

         (FT_ULong)( rfork_جابه_جایی + *rdata_pos + rdata_len ) > جریان->اندازه ||
         (FT_ULong)( rfork_جابه_جایی + map_pos + map_len ) > جریان->اندازه      )
      أرجع FT_THROW( Unknown_File_Format );

    *rdata_pos += rfork_جابه_جایی;
    map_pos    += rfork_جابه_جایی;

    error = FT_جریان_Seek( جریان, (FT_ULong)map_pos );
    إذا ( error )
      أرجع error;

    head2[15] = (FT_Byte)( سر[15] + 1 );       /* make it be different */

    error = FT_جریان_Read( جریان, (FT_Byte*)head2, 16 );
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

    (فراغ)FT_جریان_SKIP( 4        /* skip handle to next resource map */
                          + 2      /* skip file resource number */
                          + 2 );   /* skip attributes */

    إذا ( FT_READ_SHORT( type_list ) )
      أرجع error;
    إذا ( type_list < 0 )
      أرجع FT_THROW( Unknown_File_Format );

    error = FT_جریان_Seek( جریان, (FT_ULong)( map_pos + type_list ) );
    إذا ( error )
      أرجع error;

    *map_جابه_جایی = map_pos + type_list;
    أرجع FT_Err_Ok;
  }


  FT_COMPARE_DEF( صحيح )
  ft_raccess_مرتب_سازی_ref_by_id( ثابت فراغ*  a,
                             ثابت فراغ*  b )
  {
    أرجع  ( (FT_RFork_Ref*)a )->res_id - ( (FT_RFork_Ref*)b )->res_id;
  }


  FT_پایه_DEF( FT_Error )
  FT_Raccess_Get_داده_Offsets( FT_Library  library,
                              FT_جریان   جریان,
                              FT_Long     map_جابه_جایی,
                              FT_Long     rdata_pos,
                              FT_Long     برچسب,
                              FT_Bool     مرتب_سازی_by_res_id,
                              FT_Long   **offsets,
                              FT_Long    *شمار )
  {
    FT_Error      error;
    صحيح           i, j, cnt, subcnt;
    FT_Long       برچسب_داخلی, rpos;
    FT_حافظه     حافظه = library->حافظه;
    FT_Long       موقت;
    FT_Long       *offsets_داخلی = عدم;
    FT_RFork_Ref  *ref = عدم;


    FT_TRACE3(( "\n" ));
    error = FT_جریان_Seek( جریان, (FT_ULong)map_جابه_جایی );
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
      إذا ( FT_READ_LONG( برچسب_داخلی ) ||
           FT_READ_SHORT( subcnt )      ||
           FT_READ_SHORT( rpos )        )
        أرجع error;

      FT_TRACE2(( "Resource tags: %c%c%c%c\n",
                  (حرف)( 0xFF & ( برچسب_داخلی >> 24 ) ),
                  (حرف)( 0xFF & ( برچسب_داخلی >> 16 ) ),
                  (حرف)( 0xFF & ( برچسب_داخلی >>  8 ) ),
                  (حرف)( 0xFF & ( برچسب_داخلی >>  0 ) ) ));
      FT_TRACE3(( "             : subcount=%d, suboffset=0x%04lx\n",
                  subcnt, rpos ));

      إذا ( برچسب_داخلی == برچسب )
      {
        *شمار = subcnt + 1;
        rpos  += map_جابه_جایی;

        /* a zero count might be valid in the resource specification, */
        /* however, it is completely useless to us                    */
        إذا ( *شمار < 1 || *شمار > 2727 )
          أرجع FT_THROW( Invalid_جدول );

        error = FT_جریان_Seek( جریان, (FT_ULong)rpos );
        إذا ( error )
          أرجع error;

        إذا ( FT_QNEW_ARRAY( ref, *شمار ) )
          أرجع error;

        لكل ( j = 0; j < *شمار; j++ )
        {
          إذا ( FT_READ_SHORT( ref[j].res_id ) )
            اذهب Exit;
          إذا ( FT_جریان_SKIP( 2 ) )  /* resource name offset */
            اذهب Exit;
          إذا ( FT_READ_LONG( موقت ) ) /* attributes (8bit), offset (24bit) */
            اذهب Exit;
          إذا ( FT_جریان_SKIP( 4 ) )  /* mbz */
            اذهب Exit;

          /*
           * According to Inside Macintosh: More Macintosh Toolbox,
           * "Resource IDs" (1-46), there are some reserved IDs.
           * However, FreeType2 is not a font synthesizer, no need
           * to check the acceptable resource ID or its attributes.
           */
          ref[j].جابه_جایی = موقت & 0xFFFFFFL;

          FT_TRACE3(( "             [%d]:"
                      " resource_id=0x%04x, offset=0x%08lx\n",
                      j, (FT_UShort)ref[j].res_id, ref[j].جابه_جایی ));
        }

        إذا ( مرتب_سازی_by_res_id )
        {
          ft_qsort( ref,
                    (size_t)*شمار,
                    حجم ( FT_RFork_Ref ),
                    ft_raccess_مرتب_سازی_ref_by_id );

          FT_TRACE3(( "             -- sort resources by their ids --\n" ));

          لكل ( j = 0; j < *شمار; j++ )
            FT_TRACE3(( "             [%d]:"
                        " resource_id=0x%04x, offset=0x%08lx\n",
                        j, ref[j].res_id, ref[j].جابه_جایی ));
        }

        إذا ( FT_QNEW_ARRAY( offsets_داخلی, *شمار ) )
          اذهب Exit;

        /* XXX: duplicated reference ID,
         *      gap between reference IDs are acceptable?
         *      further investigation on Apple implementation is needed.
         */
        لكل ( j = 0; j < *شمار; j++ )
          offsets_داخلی[j] = rdata_pos + ref[j].جابه_جایی;

        *offsets = offsets_داخلی;
        error    = FT_Err_Ok;

      Exit:
        FT_FREE( ref );
        أرجع error;
      }
    }

    أرجع FT_THROW( Cannot_Open_Resource );
  }


#اگر_تعریف_شده FT_CONFIG_OPTION_GUESSING_EMBEDDED_RFORK

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
                              FT_جریان   جریان,
                              حرف       *پایه_file_name,
                              حرف      **نتیجه_file_name,
                              FT_Long    *نتیجه_جابه_جایی );

  ساكن FT_Error
  raccess_guess_apple_single( FT_Library  library,
                              FT_جریان   جریان,
                              حرف       *پایه_file_name,
                              حرف      **نتیجه_file_name,
                              FT_Long    *نتیجه_جابه_جایی );

  ساكن FT_Error
  raccess_guess_darwin_ufs_export( FT_Library  library,
                                   FT_جریان   جریان,
                                   حرف       *پایه_file_name,
                                   حرف      **نتیجه_file_name,
                                   FT_Long    *نتیجه_جابه_جایی );

  ساكن FT_Error
  raccess_guess_darwin_newvfs( FT_Library  library,
                               FT_جریان   جریان,
                               حرف       *پایه_file_name,
                               حرف      **نتیجه_file_name,
                               FT_Long    *نتیجه_جابه_جایی );

  ساكن FT_Error
  raccess_guess_darwin_hfsplus( FT_Library  library,
                                FT_جریان   جریان,
                                حرف       *پایه_file_name,
                                حرف      **نتیجه_file_name,
                                FT_Long    *نتیجه_جابه_جایی );

  ساكن FT_Error
  raccess_guess_vfat( FT_Library  library,
                      FT_جریان   جریان,
                      حرف       *پایه_file_name,
                      حرف      **نتیجه_file_name,
                      FT_Long    *نتیجه_جابه_جایی );

  ساكن FT_Error
  raccess_guess_linux_cap( FT_Library  library,
                           FT_جریان   جریان,
                           حرف       *پایه_file_name,
                           حرف      **نتیجه_file_name,
                           FT_Long    *نتیجه_جابه_جایی );

  ساكن FT_Error
  raccess_guess_linux_double( FT_Library  library,
                              FT_جریان   جریان,
                              حرف       *پایه_file_name,
                              حرف      **نتیجه_file_name,
                              FT_Long    *نتیجه_جابه_جایی );

  ساكن FT_Error
  raccess_guess_linux_netatalk( FT_Library  library,
                                FT_جریان   جریان,
                                حرف       *پایه_file_name,
                                حرف      **نتیجه_file_name,
                                FT_Long    *نتیجه_جابه_جایی );


  CONST_FT_RFORK_قاعده_ARRAY_BEGIN( ft_raccess_guess_جدول,
                                                      ft_raccess_guess_rec )
  CONST_FT_RFORK_قاعده_ARRAY_مدخل( apple_double,      apple_double )
  CONST_FT_RFORK_قاعده_ARRAY_مدخل( apple_single,      apple_single )
  CONST_FT_RFORK_قاعده_ARRAY_مدخل( darwin_ufs_export, darwin_ufs_export )
  CONST_FT_RFORK_قاعده_ARRAY_مدخل( darwin_newvfs,     darwin_newvfs )
  CONST_FT_RFORK_قاعده_ARRAY_مدخل( darwin_hfsplus,    darwin_hfsplus )
  CONST_FT_RFORK_قاعده_ARRAY_مدخل( vfat,              vfat )
  CONST_FT_RFORK_قاعده_ARRAY_مدخل( linux_cap,         linux_cap )
  CONST_FT_RFORK_قاعده_ARRAY_مدخل( linux_double,      linux_double )
  CONST_FT_RFORK_قاعده_ARRAY_مدخل( linux_netatalk,    linux_netatalk )
  CONST_FT_RFORK_قاعده_ARRAY_انتها


  /*************************************************************************/
  /****                                                                 ****/
  /****                       Helper functions                          ****/
  /****                                                                 ****/
  /*************************************************************************/

  ساكن FT_Error
  raccess_guess_apple_generic( FT_Library  library,
                               FT_جریان   جریان,
                               حرف       *پایه_file_name,
                               FT_Int32    magic,
                               FT_Long    *نتیجه_جابه_جایی );

  ساكن FT_Error
  raccess_guess_linux_double_from_file_name( FT_Library  library,
                                             حرف*       file_name,
                                             FT_Long    *نتیجه_جابه_جایی );

  ساكن حرف *
  raccess_make_file_name( FT_حافظه    حافظه,
                          ثابت حرف  *original_name,
                          ثابت حرف  *insertion );

  FT_پایه_DEF( فراغ )
  FT_Raccess_Guess( FT_Library  library,
                    FT_جریان   جریان,
                    حرف*       پایه_name,
                    حرف      **new_نامها,
                    FT_Long    *offsets,
                    FT_Error   *errors )
  {
    FT_Int  i;


    لكل ( i = 0; i < FT_RACCESS_N_قواعد; i++ )
    {
      new_نامها[i] = عدم;
      إذا ( عدم != جریان )
        errors[i] = FT_جریان_Seek( جریان, 0 );
      وإلا
        errors[i] = FT_Err_Ok;

      إذا ( errors[i] )
        استمر;

      errors[i] = ft_raccess_guess_جدول[i].func( library,
                                                  جریان, پایه_name,
                                                  &(new_نامها[i]),
                                                  &(offsets[i]) );
    }

    أرجع;
  }


#إذا defined( FT_CONFIG_OPTION_MAC_FONTS ) && !defined( FT_MACINTOSH )
  ساكن FT_RFork_قاعده
  raccess_get_قاعده_type_from_قاعده_شاخص( FT_Library  library,
                                         FT_UInt     قاعده_شاخص )
  {
    FT_استفادهنشده( library );

    إذا ( قاعده_شاخص >= FT_RACCESS_N_قواعد )
      أرجع FT_RFork_قاعده_invalid;

    أرجع ft_raccess_guess_جدول[قاعده_شاخص].type;
  }


  /*
   * For this function, refer ftbase.h.
   */
  FT_LOCAL_DEF( FT_Bool )
  ft_raccess_قاعده_by_darwin_vfs( FT_Library  library,
                                 FT_UInt     قاعده_شاخص )
  {
    بدل( raccess_get_قاعده_type_from_قاعده_شاخص( library, قاعده_شاخص ) )
    {
      حالة FT_RFork_قاعده_darwin_newvfs:
      حالة FT_RFork_قاعده_darwin_hfsplus:
        أرجع TRUE;

      افتراضي:
        أرجع FALSE;
    }
  }
#پایان_اگر


  ساكن FT_Error
  raccess_guess_apple_double( FT_Library  library,
                              FT_جریان   جریان,
                              حرف       *پایه_file_name,
                              حرف      **نتیجه_file_name,
                              FT_Long    *نتیجه_جابه_جایی )
  {
    FT_Int32  magic = ( 0x00 << 24 ) |
                      ( 0x05 << 16 ) |
                      ( 0x16 <<  8 ) |
                        0x07;


    *نتیجه_file_name = عدم;
    إذا ( عدم == جریان )
      أرجع FT_THROW( Cannot_Open_جریان );

    أرجع raccess_guess_apple_generic( library, جریان, پایه_file_name,
                                        magic, نتیجه_جابه_جایی );
  }


  ساكن FT_Error
  raccess_guess_apple_single( FT_Library  library,
                              FT_جریان   جریان,
                              حرف       *پایه_file_name,
                              حرف      **نتیجه_file_name,
                              FT_Long    *نتیجه_جابه_جایی )
  {
    FT_Int32  magic = ( 0x00 << 24 ) |
                      ( 0x05 << 16 ) |
                      ( 0x16 <<  8 ) |
                        0x00;


    *نتیجه_file_name = عدم;
    إذا ( عدم == جریان )
      أرجع FT_THROW( Cannot_Open_جریان );

    أرجع raccess_guess_apple_generic( library, جریان, پایه_file_name,
                                        magic, نتیجه_جابه_جایی );
  }


  ساكن FT_Error
  raccess_guess_darwin_ufs_export( FT_Library  library,
                                   FT_جریان   جریان,
                                   حرف       *پایه_file_name,
                                   حرف      **نتیجه_file_name,
                                   FT_Long    *نتیجه_جابه_جایی )
  {
    حرف*      newpath;
    FT_Error   error;
    FT_حافظه  حافظه;

    FT_استفادهنشده( جریان );


    حافظه  = library->حافظه;
    newpath = raccess_make_file_name( حافظه, پایه_file_name, "._" );
    إذا ( !newpath )
      أرجع FT_THROW( بیرون_از_حافظه );

    error = raccess_guess_linux_double_from_file_name( library, newpath,
                                                       نتیجه_جابه_جایی );
    إذا ( !error )
      *نتیجه_file_name = newpath;
    وإلا
      FT_FREE( newpath );

    أرجع error;
  }


  ساكن FT_Error
  raccess_guess_darwin_hfsplus( FT_Library  library,
                                FT_جریان   جریان,
                                حرف       *پایه_file_name,
                                حرف      **نتیجه_file_name,
                                FT_Long    *نتیجه_جابه_جایی )
  {
    /*
      Only meaningful on systems with hfs+ drivers (or Macs).
     */
    FT_Error   error;
    حرف*      newpath = عدم;
    FT_حافظه  حافظه;
    FT_Long    پایه_file_len = (FT_Long)ft_strlen( پایه_file_name );

    FT_استفادهنشده( جریان );


    حافظه = library->حافظه;

    إذا ( پایه_file_len + 6 > FT_INT_MAX )
      أرجع FT_THROW( Array_Too_Large );

    إذا ( FT_QALLOC( newpath, پایه_file_len + 6 ) )
      أرجع error;

    FT_MEM_COPY( newpath, پایه_file_name, پایه_file_len );
    FT_MEM_COPY( newpath + پایه_file_len, "/rsrc", 6 );

    *نتیجه_file_name = newpath;
    *نتیجه_جابه_جایی    = 0;

    أرجع FT_Err_Ok;
  }


  ساكن FT_Error
  raccess_guess_darwin_newvfs( FT_Library  library,
                               FT_جریان   جریان,
                               حرف       *پایه_file_name,
                               حرف      **نتیجه_file_name,
                               FT_Long    *نتیجه_جابه_جایی )
  {
    /*
      Only meaningful on systems with Mac OS X (> 10.1).
     */
    FT_Error   error;
    حرف*      newpath = عدم;
    FT_حافظه  حافظه;
    FT_Long    پایه_file_len = (FT_Long)ft_strlen( پایه_file_name );

    FT_استفادهنشده( جریان );


    حافظه = library->حافظه;

    إذا ( پایه_file_len + 18 > FT_INT_MAX )
      أرجع FT_THROW( Array_Too_Large );

    إذا ( FT_QALLOC( newpath, پایه_file_len + 18 ) )
      أرجع error;

    FT_MEM_COPY( newpath, پایه_file_name, پایه_file_len );
    FT_MEM_COPY( newpath + پایه_file_len, "/..namedfork/rsrc", 18 );

    *نتیجه_file_name = newpath;
    *نتیجه_جابه_جایی    = 0;

    أرجع FT_Err_Ok;
  }


  ساكن FT_Error
  raccess_guess_vfat( FT_Library  library,
                      FT_جریان   جریان,
                      حرف       *پایه_file_name,
                      حرف      **نتیجه_file_name,
                      FT_Long    *نتیجه_جابه_جایی )
  {
    حرف*      newpath;
    FT_حافظه  حافظه;

    FT_استفادهنشده( جریان );


    حافظه = library->حافظه;

    newpath = raccess_make_file_name( حافظه, پایه_file_name,
                                      "resource.frk/" );
    إذا ( !newpath )
      أرجع FT_THROW( بیرون_از_حافظه );

    *نتیجه_file_name = newpath;
    *نتیجه_جابه_جایی    = 0;

    أرجع FT_Err_Ok;
  }


  ساكن FT_Error
  raccess_guess_linux_cap( FT_Library  library,
                           FT_جریان   جریان,
                           حرف       *پایه_file_name,
                           حرف      **نتیجه_file_name,
                           FT_Long    *نتیجه_جابه_جایی )
  {
    حرف*      newpath;
    FT_حافظه  حافظه;

    FT_استفادهنشده( جریان );


    حافظه = library->حافظه;

    newpath = raccess_make_file_name( حافظه, پایه_file_name, ".resource/" );
    إذا ( !newpath )
      أرجع FT_THROW( بیرون_از_حافظه );

    *نتیجه_file_name = newpath;
    *نتیجه_جابه_جایی    = 0;

    أرجع FT_Err_Ok;
  }


  ساكن FT_Error
  raccess_guess_linux_double( FT_Library  library,
                              FT_جریان   جریان,
                              حرف       *پایه_file_name,
                              حرف      **نتیجه_file_name,
                              FT_Long    *نتیجه_جابه_جایی )
  {
    حرف*      newpath;
    FT_Error   error;
    FT_حافظه  حافظه;

    FT_استفادهنشده( جریان );


    حافظه = library->حافظه;

    newpath = raccess_make_file_name( حافظه, پایه_file_name, "%" );
    إذا ( !newpath )
      أرجع FT_THROW( بیرون_از_حافظه );

    error = raccess_guess_linux_double_from_file_name( library, newpath,
                                                       نتیجه_جابه_جایی );
    إذا ( !error )
      *نتیجه_file_name = newpath;
    وإلا
      FT_FREE( newpath );

    أرجع error;
  }


  ساكن FT_Error
  raccess_guess_linux_netatalk( FT_Library  library,
                                FT_جریان   جریان,
                                حرف       *پایه_file_name,
                                حرف      **نتیجه_file_name,
                                FT_Long    *نتیجه_جابه_جایی )
  {
    حرف*      newpath;
    FT_Error   error;
    FT_حافظه  حافظه;

    FT_استفادهنشده( جریان );


    حافظه = library->حافظه;

    newpath = raccess_make_file_name( حافظه, پایه_file_name,
                                      ".AppleDouble/" );
    إذا ( !newpath )
      أرجع FT_THROW( بیرون_از_حافظه );

    error = raccess_guess_linux_double_from_file_name( library, newpath,
                                                       نتیجه_جابه_جایی );
    إذا ( !error )
      *نتیجه_file_name = newpath;
    وإلا
      FT_FREE( newpath );

    أرجع error;
  }


  ساكن FT_Error
  raccess_guess_apple_generic( FT_Library  library,
                               FT_جریان   جریان,
                               حرف       *پایه_file_name,
                               FT_Int32    magic,
                               FT_Long    *نتیجه_جابه_جایی )
  {
    FT_Int32   magic_from_جریان;
    FT_Error   error;
    FT_Int32   نسخه_عدد = 0;
    FT_UShort  n_از_entries;

    صحيح        i;
    FT_Int32   مدخل_id, مدخل_جابه_جایی, مدخل_درازی = 0;

    ثابت FT_Int32  resource_fork_مدخل_id = 0x2;

    FT_استفادهنشده( library );
    FT_استفادهنشده( پایه_file_name );
    FT_استفادهنشده( نسخه_عدد );
    FT_استفادهنشده( مدخل_درازی   );


    إذا ( FT_READ_LONG( magic_from_جریان ) )
      أرجع error;
    إذا ( magic_from_جریان != magic )
      أرجع FT_THROW( Unknown_File_Format );

    إذا ( FT_READ_LONG( نسخه_عدد ) )
      أرجع error;

    /* filler */
    error = FT_جریان_Skip( جریان, 16 );
    إذا ( error )
      أرجع error;

    إذا ( FT_READ_USHORT( n_از_entries ) )
      أرجع error;
    إذا ( n_از_entries == 0 )
      أرجع FT_THROW( Unknown_File_Format );

    لكل ( i = 0; i < n_از_entries; i++ )
    {
      إذا ( FT_READ_LONG( مدخل_id ) )
        أرجع error;
      إذا ( مدخل_id == resource_fork_مدخل_id )
      {
        إذا ( FT_READ_LONG( مدخل_جابه_جایی ) ||
             FT_READ_LONG( مدخل_درازی ) )
          استمر;
        *نتیجه_جابه_جایی = مدخل_جابه_جایی;

        أرجع FT_Err_Ok;
      }
      وإلا
      {
        error = FT_جریان_Skip( جریان, 4 + 4 );    /* offset + length */
        إذا ( error )
          أرجع error;
      }
    }

    أرجع FT_THROW( Unknown_File_Format );
  }


  ساكن FT_Error
  raccess_guess_linux_double_from_file_name( FT_Library  library,
                                             حرف       *file_name,
                                             FT_Long    *نتیجه_جابه_جایی )
  {
    FT_Open_آرگومانها  args2;
    FT_جریان     stream2;
    حرف*         nouse = عدم;
    FT_Error      error;


    args2.flags    = FT_OPEN_PATHNAME;
    args2.pathname = file_name;
    error = FT_جریان_New( library, &args2, &stream2 );
    إذا ( error )
      أرجع error;

    error = raccess_guess_apple_double( library, stream2, file_name,
                                        &nouse, نتیجه_جابه_جایی );

    FT_جریان_Free( stream2, 0 );

    أرجع error;
  }


  ساكن حرف*
  raccess_make_file_name( FT_حافظه    حافظه,
                          ثابت حرف  *original_name,
                          ثابت حرف  *insertion )
  {
    حرف*        new_name = عدم;
    ثابت حرف*  tmp;
    ثابت حرف*  slash;
    size_t       new_درازی;
    FT_Error     error;


    new_درازی = ft_strlen( original_name ) + ft_strlen( insertion );
    إذا ( FT_QALLOC( new_name, new_درازی + 1 ) )
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

  FT_پایه_DEF( فراغ )
  FT_Raccess_Guess( FT_Library  library,
                    FT_جریان   جریان,
                    حرف       *پایه_name,
                    حرف      **new_نامها,
                    FT_Long    *offsets,
                    FT_Error   *errors )
  {
    FT_Int  i;

    FT_استفادهنشده( library );
    FT_استفادهنشده( جریان );
    FT_استفادهنشده( پایه_name );


    لكل ( i = 0; i < FT_RACCESS_N_قواعد; i++ )
    {
      new_نامها[i] = عدم;
      offsets[i]   = 0;
      errors[i]    = FT_ERR( Unimplemented_Feature );
    }
  }


#پایان_اگر  /* !FT_CONFIG_OPTION_GUESSING_EMBEDDED_RFORK */


/*انتها*/
