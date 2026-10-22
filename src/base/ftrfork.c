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

#تعریف_منسوخ  FT_COMPONENT
#تعریف_کرو FT_COMPONENT  raccess


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

  FT_بنیاد_DEF( FT_Error )
  FT_Raccess_Get_سرخی_Info( FT_Library  library,
                             FT_دھارا   دھارا,
                             FT_Long     rfork_انحراف,
                             FT_Long    *map_انحراف,
                             FT_Long    *rdata_pos )
  {
    FT_Error       error;
    غیرعلامتی حرف  سر[16], head2[16];
    FT_Long        map_pos, map_len, rdata_len;
    صحیح            allzeros, allmatch, i;
    FT_Long        type_list;

    FT_غیر_مستعمل( library );


    error = FT_دھارا_Seek( دھارا, (FT_ULong)rfork_انحراف );
    اگر ( error )
      واپس error;

    error = FT_دھارا_Read( دھارا, (FT_Byte*)سر, 16 );
    اگر ( error )
      واپس error;

    /* ensure positive values */
    اگر ( سر[0]  >= 0x80 ||
         سر[4]  >= 0x80 ||
         سر[8]  >= 0x80 ||
         سر[12] >= 0x80 )
      واپس FT_THROW( Unknown_File_Format );

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
    اگر ( !map_pos )
      واپس FT_THROW( Unknown_File_Format );

    /* check whether rdata and map overlap */
    اگر ( *rdata_pos < map_pos )
    {
      اگر ( *rdata_pos > map_pos - rdata_len )
        واپس FT_THROW( Unknown_File_Format );
    }
    ورنہ
    {
      اگر ( map_pos > *rdata_pos - map_len )
        واپس FT_THROW( Unknown_File_Format );
    }

    /* check whether end of rdata or map exceeds stream size */
    اگر ( FT_LONG_MAX - rdata_len < *rdata_pos                               ||
         FT_LONG_MAX - map_len < map_pos                                    ||

         FT_LONG_MAX - ( *rdata_pos + rdata_len ) < rfork_انحراف            ||
         FT_LONG_MAX - ( map_pos + map_len ) < rfork_انحراف                 ||

         (FT_ULong)( rfork_انحراف + *rdata_pos + rdata_len ) > دھارا->سائز ||
         (FT_ULong)( rfork_انحراف + map_pos + map_len ) > دھارا->سائز      )
      واپس FT_THROW( Unknown_File_Format );

    *rdata_pos += rfork_انحراف;
    map_pos    += rfork_انحراف;

    error = FT_دھارا_Seek( دھارا, (FT_ULong)map_pos );
    اگر ( error )
      واپس error;

    head2[15] = (FT_Byte)( سر[15] + 1 );       /* make it be different */

    error = FT_دھارا_Read( دھارا, (FT_Byte*)head2, 16 );
    اگر ( error )
      واپس error;

    allzeros = 1;
    allmatch = 1;
    برائے ( i = 0; i < 16; i++ )
    {
      اگر ( head2[i] != 0 )
        allzeros = 0;
      اگر ( head2[i] != سر[i] )
        allmatch = 0;
    }
    اگر ( !allzeros && !allmatch )
      واپس FT_THROW( Unknown_File_Format );

    /* If we have reached this point then it is probably a mac resource */
    /* file.  Now, does it contain any interesting resources?           */

    (خالی)FT_دھارا_SKIP( 4        /* skip handle to next resource map */
                          + 2      /* skip file resource number */
                          + 2 );   /* skip attributes */

    اگر ( FT_READ_SHORT( type_list ) )
      واپس error;
    اگر ( type_list < 0 )
      واپس FT_THROW( Unknown_File_Format );

    error = FT_دھارا_Seek( دھارا, (FT_ULong)( map_pos + type_list ) );
    اگر ( error )
      واپس error;

    *map_انحراف = map_pos + type_list;
    واپس FT_Err_Ok;
  }


  FT_COMPARE_DEF( صحیح )
  ft_raccess_ترتیب_دو_ref_by_id( ثابت خالی*  a,
                             ثابت خالی*  b )
  {
    واپس  ( (FT_RFork_Ref*)a )->res_id - ( (FT_RFork_Ref*)b )->res_id;
  }


  FT_بنیاد_DEF( FT_Error )
  FT_Raccess_Get_ڈیٹا_Offsets( FT_Library  library,
                              FT_دھارا   دھارا,
                              FT_Long     map_انحراف,
                              FT_Long     rdata_pos,
                              FT_Long     لیبل,
                              FT_Bool     ترتیب_دو_by_res_id,
                              FT_Long   **offsets,
                              FT_Long    *گنتی )
  {
    FT_Error      error;
    صحیح           i, j, cnt, subcnt;
    FT_Long       لیبل_اندرونی, rpos;
    FT_حافظہ     حافظہ = library->حافظہ;
    FT_Long       عارضی;
    FT_Long       *offsets_اندرونی = تہی;
    FT_RFork_Ref  *ref = تہی;


    FT_TRACE3(( "\n" ));
    error = FT_دھارا_Seek( دھارا, (FT_ULong)map_انحراف );
    اگر ( error )
      واپس error;

    اگر ( FT_READ_SHORT( cnt ) )
      واپس error;
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
      واپس FT_THROW( Invalid_جدول );

    برائے ( i = 0; i < cnt; i++ )
    {
      اگر ( FT_READ_LONG( لیبل_اندرونی ) ||
           FT_READ_SHORT( subcnt )      ||
           FT_READ_SHORT( rpos )        )
        واپس error;

      FT_TRACE2(( "Resource tags: %c%c%c%c\n",
                  (حرف)( 0xFF & ( لیبل_اندرونی >> 24 ) ),
                  (حرف)( 0xFF & ( لیبل_اندرونی >> 16 ) ),
                  (حرف)( 0xFF & ( لیبل_اندرونی >>  8 ) ),
                  (حرف)( 0xFF & ( لیبل_اندرونی >>  0 ) ) ));
      FT_TRACE3(( "             : subcount=%d, suboffset=0x%04lx\n",
                  subcnt, rpos ));

      اگر ( لیبل_اندرونی == لیبل )
      {
        *گنتی = subcnt + 1;
        rpos  += map_انحراف;

        /* a zero count might be valid in the resource specification, */
        /* however, it is completely useless to us                    */
        اگر ( *گنتی < 1 || *گنتی > 2727 )
          واپس FT_THROW( Invalid_جدول );

        error = FT_دھارا_Seek( دھارا, (FT_ULong)rpos );
        اگر ( error )
          واپس error;

        اگر ( FT_QNEW_ARRAY( ref, *گنتی ) )
          واپس error;

        برائے ( j = 0; j < *گنتی; j++ )
        {
          اگر ( FT_READ_SHORT( ref[j].res_id ) )
            جاؤ Exit;
          اگر ( FT_دھارا_SKIP( 2 ) )  /* resource name offset */
            جاؤ Exit;
          اگر ( FT_READ_LONG( عارضی ) ) /* attributes (8bit), offset (24bit) */
            جاؤ Exit;
          اگر ( FT_دھارا_SKIP( 4 ) )  /* mbz */
            جاؤ Exit;

          /*
           * According to Inside Macintosh: More Macintosh Toolbox,
           * "Resource IDs" (1-46), there are some reserved IDs.
           * However, FreeType2 is not a font synthesizer, no need
           * to check the acceptable resource ID or its attributes.
           */
          ref[j].انحراف = عارضی & 0xFFFFFFL;

          FT_TRACE3(( "             [%d]:"
                      " resource_id=0x%04x, offset=0x%08lx\n",
                      j, (FT_UShort)ref[j].res_id, ref[j].انحراف ));
        }

        اگر ( ترتیب_دو_by_res_id )
        {
          ft_qsort( ref,
                    (size_t)*گنتی,
                    حجم ( FT_RFork_Ref ),
                    ft_raccess_ترتیب_دو_ref_by_id );

          FT_TRACE3(( "             -- sort resources by their ids --\n" ));

          برائے ( j = 0; j < *گنتی; j++ )
            FT_TRACE3(( "             [%d]:"
                        " resource_id=0x%04x, offset=0x%08lx\n",
                        j, ref[j].res_id, ref[j].انحراف ));
        }

        اگر ( FT_QNEW_ARRAY( offsets_اندرونی, *گنتی ) )
          جاؤ Exit;

        /* XXX: duplicated reference ID,
         *      gap between reference IDs are acceptable?
         *      further investigation on Apple implementation is needed.
         */
        برائے ( j = 0; j < *گنتی; j++ )
          offsets_اندرونی[j] = rdata_pos + ref[j].انحراف;

        *offsets = offsets_اندرونی;
        error    = FT_Err_Ok;

      Exit:
        FT_FREE( ref );
        واپس error;
      }
    }

    واپس FT_THROW( Cannot_Open_Resource );
  }


#اگر_تعریف_شدہ FT_CONFIG_OPTION_GUESSING_EMBEDDED_RFORK

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

  ساکن FT_Error
  raccess_guess_apple_double( FT_Library  library,
                              FT_دھارا   دھارا,
                              حرف       *بنیاد_file_name,
                              حرف      **نتیجہ_file_name,
                              FT_Long    *نتیجہ_انحراف );

  ساکن FT_Error
  raccess_guess_apple_single( FT_Library  library,
                              FT_دھارا   دھارا,
                              حرف       *بنیاد_file_name,
                              حرف      **نتیجہ_file_name,
                              FT_Long    *نتیجہ_انحراف );

  ساکن FT_Error
  raccess_guess_darwin_ufs_export( FT_Library  library,
                                   FT_دھارا   دھارا,
                                   حرف       *بنیاد_file_name,
                                   حرف      **نتیجہ_file_name,
                                   FT_Long    *نتیجہ_انحراف );

  ساکن FT_Error
  raccess_guess_darwin_newvfs( FT_Library  library,
                               FT_دھارا   دھارا,
                               حرف       *بنیاد_file_name,
                               حرف      **نتیجہ_file_name,
                               FT_Long    *نتیجہ_انحراف );

  ساکن FT_Error
  raccess_guess_darwin_hfsplus( FT_Library  library,
                                FT_دھارا   دھارا,
                                حرف       *بنیاد_file_name,
                                حرف      **نتیجہ_file_name,
                                FT_Long    *نتیجہ_انحراف );

  ساکن FT_Error
  raccess_guess_vfat( FT_Library  library,
                      FT_دھارا   دھارا,
                      حرف       *بنیاد_file_name,
                      حرف      **نتیجہ_file_name,
                      FT_Long    *نتیجہ_انحراف );

  ساکن FT_Error
  raccess_guess_linux_cap( FT_Library  library,
                           FT_دھارا   دھارا,
                           حرف       *بنیاد_file_name,
                           حرف      **نتیجہ_file_name,
                           FT_Long    *نتیجہ_انحراف );

  ساکن FT_Error
  raccess_guess_linux_double( FT_Library  library,
                              FT_دھارا   دھارا,
                              حرف       *بنیاد_file_name,
                              حرف      **نتیجہ_file_name,
                              FT_Long    *نتیجہ_انحراف );

  ساکن FT_Error
  raccess_guess_linux_netatalk( FT_Library  library,
                                FT_دھارا   دھارا,
                                حرف       *بنیاد_file_name,
                                حرف      **نتیجہ_file_name,
                                FT_Long    *نتیجہ_انحراف );


  CONST_FT_RFORK_قاعدہ_ARRAY_BEGIN( ft_raccess_guess_جدول,
                                                      ft_raccess_guess_rec )
  CONST_FT_RFORK_قاعدہ_ARRAY_اندراج( apple_double,      apple_double )
  CONST_FT_RFORK_قاعدہ_ARRAY_اندراج( apple_single,      apple_single )
  CONST_FT_RFORK_قاعدہ_ARRAY_اندراج( darwin_ufs_export, darwin_ufs_export )
  CONST_FT_RFORK_قاعدہ_ARRAY_اندراج( darwin_newvfs,     darwin_newvfs )
  CONST_FT_RFORK_قاعدہ_ARRAY_اندراج( darwin_hfsplus,    darwin_hfsplus )
  CONST_FT_RFORK_قاعدہ_ARRAY_اندراج( vfat,              vfat )
  CONST_FT_RFORK_قاعدہ_ARRAY_اندراج( linux_cap,         linux_cap )
  CONST_FT_RFORK_قاعدہ_ARRAY_اندراج( linux_double,      linux_double )
  CONST_FT_RFORK_قاعدہ_ARRAY_اندراج( linux_netatalk,    linux_netatalk )
  CONST_FT_RFORK_قاعدہ_ARRAY_انتہا


  /*************************************************************************/
  /****                                                                 ****/
  /****                       Helper functions                          ****/
  /****                                                                 ****/
  /*************************************************************************/

  ساکن FT_Error
  raccess_guess_apple_generic( FT_Library  library,
                               FT_دھارا   دھارا,
                               حرف       *بنیاد_file_name,
                               FT_Int32    magic,
                               FT_Long    *نتیجہ_انحراف );

  ساکن FT_Error
  raccess_guess_linux_double_from_file_name( FT_Library  library,
                                             حرف*       file_name,
                                             FT_Long    *نتیجہ_انحراف );

  ساکن حرف *
  raccess_make_file_name( FT_حافظہ    حافظہ,
                          ثابت حرف  *original_name,
                          ثابت حرف  *insertion );

  FT_بنیاد_DEF( خالی )
  FT_Raccess_Guess( FT_Library  library,
                    FT_دھارا   دھارا,
                    حرف*       بنیاد_name,
                    حرف      **new_نام,
                    FT_Long    *offsets,
                    FT_Error   *errors )
  {
    FT_Int  i;


    برائے ( i = 0; i < FT_RACCESS_N_قواعد; i++ )
    {
      new_نام[i] = تہی;
      اگر ( تہی != دھارا )
        errors[i] = FT_دھارا_Seek( دھارا, 0 );
      ورنہ
        errors[i] = FT_Err_Ok;

      اگر ( errors[i] )
        جاری;

      errors[i] = ft_raccess_guess_جدول[i].func( library,
                                                  دھارا, بنیاد_name,
                                                  &(new_نام[i]),
                                                  &(offsets[i]) );
    }

    واپس;
  }


#اگر defined( FT_CONFIG_OPTION_MAC_FONTS ) && !defined( FT_MACINTOSH )
  ساکن FT_RFork_قاعدہ
  raccess_get_قاعدہ_type_from_قاعدہ_اشاریہ( FT_Library  library,
                                         FT_UInt     قاعدہ_اشاریہ )
  {
    FT_غیر_مستعمل( library );

    اگر ( قاعدہ_اشاریہ >= FT_RACCESS_N_قواعد )
      واپس FT_RFork_قاعدہ_invalid;

    واپس ft_raccess_guess_جدول[قاعدہ_اشاریہ].type;
  }


  /*
   * For this function, refer ftbase.h.
   */
  FT_LOCAL_DEF( FT_Bool )
  ft_raccess_قاعدہ_by_darwin_vfs( FT_Library  library,
                                 FT_UInt     قاعدہ_اشاریہ )
  {
    انتخاب( raccess_get_قاعدہ_type_from_قاعدہ_اشاریہ( library, قاعدہ_اشاریہ ) )
    {
      حالت FT_RFork_قاعدہ_darwin_newvfs:
      حالت FT_RFork_قاعدہ_darwin_hfsplus:
        واپس TRUE;

      طےشدہ:
        واپس FALSE;
    }
  }
#اگر_ختم


  ساکن FT_Error
  raccess_guess_apple_double( FT_Library  library,
                              FT_دھارا   دھارا,
                              حرف       *بنیاد_file_name,
                              حرف      **نتیجہ_file_name,
                              FT_Long    *نتیجہ_انحراف )
  {
    FT_Int32  magic = ( 0x00 << 24 ) |
                      ( 0x05 << 16 ) |
                      ( 0x16 <<  8 ) |
                        0x07;


    *نتیجہ_file_name = تہی;
    اگر ( تہی == دھارا )
      واپس FT_THROW( Cannot_Open_دھارا );

    واپس raccess_guess_apple_generic( library, دھارا, بنیاد_file_name,
                                        magic, نتیجہ_انحراف );
  }


  ساکن FT_Error
  raccess_guess_apple_single( FT_Library  library,
                              FT_دھارا   دھارا,
                              حرف       *بنیاد_file_name,
                              حرف      **نتیجہ_file_name,
                              FT_Long    *نتیجہ_انحراف )
  {
    FT_Int32  magic = ( 0x00 << 24 ) |
                      ( 0x05 << 16 ) |
                      ( 0x16 <<  8 ) |
                        0x00;


    *نتیجہ_file_name = تہی;
    اگر ( تہی == دھارا )
      واپس FT_THROW( Cannot_Open_دھارا );

    واپس raccess_guess_apple_generic( library, دھارا, بنیاد_file_name,
                                        magic, نتیجہ_انحراف );
  }


  ساکن FT_Error
  raccess_guess_darwin_ufs_export( FT_Library  library,
                                   FT_دھارا   دھارا,
                                   حرف       *بنیاد_file_name,
                                   حرف      **نتیجہ_file_name,
                                   FT_Long    *نتیجہ_انحراف )
  {
    حرف*      newpath;
    FT_Error   error;
    FT_حافظہ  حافظہ;

    FT_غیر_مستعمل( دھارا );


    حافظہ  = library->حافظہ;
    newpath = raccess_make_file_name( حافظہ, بنیاد_file_name, "._" );
    اگر ( !newpath )
      واپس FT_THROW( برآمد_کا_حافظہ );

    error = raccess_guess_linux_double_from_file_name( library, newpath,
                                                       نتیجہ_انحراف );
    اگر ( !error )
      *نتیجہ_file_name = newpath;
    ورنہ
      FT_FREE( newpath );

    واپس error;
  }


  ساکن FT_Error
  raccess_guess_darwin_hfsplus( FT_Library  library,
                                FT_دھارا   دھارا,
                                حرف       *بنیاد_file_name,
                                حرف      **نتیجہ_file_name,
                                FT_Long    *نتیجہ_انحراف )
  {
    /*
      Only meaningful on systems with hfs+ drivers (or Macs).
     */
    FT_Error   error;
    حرف*      newpath = تہی;
    FT_حافظہ  حافظہ;
    FT_Long    بنیاد_file_len = (FT_Long)ft_strlen( بنیاد_file_name );

    FT_غیر_مستعمل( دھارا );


    حافظہ = library->حافظہ;

    اگر ( بنیاد_file_len + 6 > FT_INT_MAX )
      واپس FT_THROW( Array_Too_Large );

    اگر ( FT_QALLOC( newpath, بنیاد_file_len + 6 ) )
      واپس error;

    FT_MEM_COPY( newpath, بنیاد_file_name, بنیاد_file_len );
    FT_MEM_COPY( newpath + بنیاد_file_len, "/rsrc", 6 );

    *نتیجہ_file_name = newpath;
    *نتیجہ_انحراف    = 0;

    واپس FT_Err_Ok;
  }


  ساکن FT_Error
  raccess_guess_darwin_newvfs( FT_Library  library,
                               FT_دھارا   دھارا,
                               حرف       *بنیاد_file_name,
                               حرف      **نتیجہ_file_name,
                               FT_Long    *نتیجہ_انحراف )
  {
    /*
      Only meaningful on systems with Mac OS X (> 10.1).
     */
    FT_Error   error;
    حرف*      newpath = تہی;
    FT_حافظہ  حافظہ;
    FT_Long    بنیاد_file_len = (FT_Long)ft_strlen( بنیاد_file_name );

    FT_غیر_مستعمل( دھارا );


    حافظہ = library->حافظہ;

    اگر ( بنیاد_file_len + 18 > FT_INT_MAX )
      واپس FT_THROW( Array_Too_Large );

    اگر ( FT_QALLOC( newpath, بنیاد_file_len + 18 ) )
      واپس error;

    FT_MEM_COPY( newpath, بنیاد_file_name, بنیاد_file_len );
    FT_MEM_COPY( newpath + بنیاد_file_len, "/..namedfork/rsrc", 18 );

    *نتیجہ_file_name = newpath;
    *نتیجہ_انحراف    = 0;

    واپس FT_Err_Ok;
  }


  ساکن FT_Error
  raccess_guess_vfat( FT_Library  library,
                      FT_دھارا   دھارا,
                      حرف       *بنیاد_file_name,
                      حرف      **نتیجہ_file_name,
                      FT_Long    *نتیجہ_انحراف )
  {
    حرف*      newpath;
    FT_حافظہ  حافظہ;

    FT_غیر_مستعمل( دھارا );


    حافظہ = library->حافظہ;

    newpath = raccess_make_file_name( حافظہ, بنیاد_file_name,
                                      "resource.frk/" );
    اگر ( !newpath )
      واپس FT_THROW( برآمد_کا_حافظہ );

    *نتیجہ_file_name = newpath;
    *نتیجہ_انحراف    = 0;

    واپس FT_Err_Ok;
  }


  ساکن FT_Error
  raccess_guess_linux_cap( FT_Library  library,
                           FT_دھارا   دھارا,
                           حرف       *بنیاد_file_name,
                           حرف      **نتیجہ_file_name,
                           FT_Long    *نتیجہ_انحراف )
  {
    حرف*      newpath;
    FT_حافظہ  حافظہ;

    FT_غیر_مستعمل( دھارا );


    حافظہ = library->حافظہ;

    newpath = raccess_make_file_name( حافظہ, بنیاد_file_name, ".resource/" );
    اگر ( !newpath )
      واپس FT_THROW( برآمد_کا_حافظہ );

    *نتیجہ_file_name = newpath;
    *نتیجہ_انحراف    = 0;

    واپس FT_Err_Ok;
  }


  ساکن FT_Error
  raccess_guess_linux_double( FT_Library  library,
                              FT_دھارا   دھارا,
                              حرف       *بنیاد_file_name,
                              حرف      **نتیجہ_file_name,
                              FT_Long    *نتیجہ_انحراف )
  {
    حرف*      newpath;
    FT_Error   error;
    FT_حافظہ  حافظہ;

    FT_غیر_مستعمل( دھارا );


    حافظہ = library->حافظہ;

    newpath = raccess_make_file_name( حافظہ, بنیاد_file_name, "%" );
    اگر ( !newpath )
      واپس FT_THROW( برآمد_کا_حافظہ );

    error = raccess_guess_linux_double_from_file_name( library, newpath,
                                                       نتیجہ_انحراف );
    اگر ( !error )
      *نتیجہ_file_name = newpath;
    ورنہ
      FT_FREE( newpath );

    واپس error;
  }


  ساکن FT_Error
  raccess_guess_linux_netatalk( FT_Library  library,
                                FT_دھارا   دھارا,
                                حرف       *بنیاد_file_name,
                                حرف      **نتیجہ_file_name,
                                FT_Long    *نتیجہ_انحراف )
  {
    حرف*      newpath;
    FT_Error   error;
    FT_حافظہ  حافظہ;

    FT_غیر_مستعمل( دھارا );


    حافظہ = library->حافظہ;

    newpath = raccess_make_file_name( حافظہ, بنیاد_file_name,
                                      ".AppleDouble/" );
    اگر ( !newpath )
      واپس FT_THROW( برآمد_کا_حافظہ );

    error = raccess_guess_linux_double_from_file_name( library, newpath,
                                                       نتیجہ_انحراف );
    اگر ( !error )
      *نتیجہ_file_name = newpath;
    ورنہ
      FT_FREE( newpath );

    واپس error;
  }


  ساکن FT_Error
  raccess_guess_apple_generic( FT_Library  library,
                               FT_دھارا   دھارا,
                               حرف       *بنیاد_file_name,
                               FT_Int32    magic,
                               FT_Long    *نتیجہ_انحراف )
  {
    FT_Int32   magic_from_دھارا;
    FT_Error   error;
    FT_Int32   نسخہ_نمبر = 0;
    FT_UShort  n_کا_entries;

    صحیح        i;
    FT_Int32   اندراج_id, اندراج_انحراف, اندراج_لمبائی = 0;

    ثابت FT_Int32  resource_fork_اندراج_id = 0x2;

    FT_غیر_مستعمل( library );
    FT_غیر_مستعمل( بنیاد_file_name );
    FT_غیر_مستعمل( نسخہ_نمبر );
    FT_غیر_مستعمل( اندراج_لمبائی   );


    اگر ( FT_READ_LONG( magic_from_دھارا ) )
      واپس error;
    اگر ( magic_from_دھارا != magic )
      واپس FT_THROW( Unknown_File_Format );

    اگر ( FT_READ_LONG( نسخہ_نمبر ) )
      واپس error;

    /* filler */
    error = FT_دھارا_Skip( دھارا, 16 );
    اگر ( error )
      واپس error;

    اگر ( FT_READ_USHORT( n_کا_entries ) )
      واپس error;
    اگر ( n_کا_entries == 0 )
      واپس FT_THROW( Unknown_File_Format );

    برائے ( i = 0; i < n_کا_entries; i++ )
    {
      اگر ( FT_READ_LONG( اندراج_id ) )
        واپس error;
      اگر ( اندراج_id == resource_fork_اندراج_id )
      {
        اگر ( FT_READ_LONG( اندراج_انحراف ) ||
             FT_READ_LONG( اندراج_لمبائی ) )
          جاری;
        *نتیجہ_انحراف = اندراج_انحراف;

        واپس FT_Err_Ok;
      }
      ورنہ
      {
        error = FT_دھارا_Skip( دھارا, 4 + 4 );    /* offset + length */
        اگر ( error )
          واپس error;
      }
    }

    واپس FT_THROW( Unknown_File_Format );
  }


  ساکن FT_Error
  raccess_guess_linux_double_from_file_name( FT_Library  library,
                                             حرف       *file_name,
                                             FT_Long    *نتیجہ_انحراف )
  {
    FT_Open_دلائل  args2;
    FT_دھارا     stream2;
    حرف*         nouse = تہی;
    FT_Error      error;


    args2.flags    = FT_OPEN_PATHNAME;
    args2.pathname = file_name;
    error = FT_دھارا_New( library, &args2, &stream2 );
    اگر ( error )
      واپس error;

    error = raccess_guess_apple_double( library, stream2, file_name,
                                        &nouse, نتیجہ_انحراف );

    FT_دھارا_Free( stream2, 0 );

    واپس error;
  }


  ساکن حرف*
  raccess_make_file_name( FT_حافظہ    حافظہ,
                          ثابت حرف  *original_name,
                          ثابت حرف  *insertion )
  {
    حرف*        new_name = تہی;
    ثابت حرف*  tmp;
    ثابت حرف*  slash;
    size_t       new_لمبائی;
    FT_Error     error;


    new_لمبائی = ft_strlen( original_name ) + ft_strlen( insertion );
    اگر ( FT_QALLOC( new_name, new_لمبائی + 1 ) )
      واپس تہی;

    tmp = ft_strrchr( original_name, '/' );
    اگر ( tmp )
    {
      ft_strncpy( new_name,
                  original_name,
                  (size_t)( tmp - original_name + 1 ) );
      new_name[tmp - original_name + 1] = '\0';
      slash = tmp + 1;
    }
    ورنہ
    {
      slash       = original_name;
      new_name[0] = '\0';
    }

    ft_strcat( new_name, insertion );
    ft_strcat( new_name, slash );

    واپس new_name;
  }


#ورنہ   /* !FT_CONFIG_OPTION_GUESSING_EMBEDDED_RFORK */


  /**************************************************************************
   *                 Dummy function; just sets errors
   */

  FT_بنیاد_DEF( خالی )
  FT_Raccess_Guess( FT_Library  library,
                    FT_دھارا   دھارا,
                    حرف       *بنیاد_name,
                    حرف      **new_نام,
                    FT_Long    *offsets,
                    FT_Error   *errors )
  {
    FT_Int  i;

    FT_غیر_مستعمل( library );
    FT_غیر_مستعمل( دھارا );
    FT_غیر_مستعمل( بنیاد_name );


    برائے ( i = 0; i < FT_RACCESS_N_قواعد; i++ )
    {
      new_نام[i] = تہی;
      offsets[i]   = 0;
      errors[i]    = FT_ERR( Unimplemented_Feature );
    }
  }


#اگر_ختم  /* !FT_CONFIG_OPTION_GUESSING_EMBEDDED_RFORK */


/*انتہا*/
