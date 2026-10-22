/****************************************************************************
 *
 * ftmac.c
 *
 *   Mac FOND support.  Written by just@letterror.com.
 * Heavily modified by mpsuzuki, George Williams, and Sean McBride.
 *
 * This file is for Mac OS X only; see builds/mac/ftoldmac.c for
 * classic platforms built by MPW.
 *
 * Copyright (C) 1996-2026 by
 * Just van Rossum, David Turner, Robert Wilhelm, and Werner Lemberg.
 *
 * This file is part of the FreeType project, and may only be used,
 * modified, and distributed under the terms of the FreeType project
 * license, LICENSE.TXT.  By continuing to use, modify, or distribute
 * this file you indicate that you have read the license and
 * understand and accept it fully.
 *
 */


  /*
    Notes

    Mac suitcase files can (and often do!) contain multiple fonts.  To
    support this I use the face_index argument of FT_(Open|New)_Face()
    functions, and pretend the suitcase file is a collection.

    Warning: fbit and NFNT bitmap resources are not supported yet.  In old
    sfnt fonts, bitmap glyph data for each size is stored in each `NFNT'
    resources instead of the `bdat' table in the sfnt resource.  Therefore,
    face->num_fixed_sizes is set to 0, because bitmap data in `NFNT'
    resource is unavailable at present.

    The Mac FOND support works roughly like this:

    - Check whether the offered stream points to a Mac suitcase file.  This
      is done by checking the file type: it has to be 'FFIL' or 'tfil'.  The
      stream that gets passed to our init_face() routine is a stdio stream,
      which isn't usable for us, since the FOND resources live in the
      resource fork.  So we just grab the stream->pathname field.

    - Read the FOND resource into memory, then check whether there is a
      TrueType font and/or(!) a Type 1 font available.

    - If there is a Type 1 font available (as a separate `LWFN' file), read
      its data into memory, massage it slightly so it becomes PFB data, wrap
      it into a memory stream, load the Type 1 driver and delegate the rest
      of the work to it by calling FT_Open_Face().  (XXX TODO: after this
      has been done, the kerning data from the FOND resource should be
      appended to the face: On the Mac there are usually no AFM files
      available.  However, this is tricky since we need to map Mac char
      codes to ps glyph names to glyph ID's...)

    - If there is a TrueType font (an `sfnt' resource), read it into memory,
      wrap it into a memory stream, load the TrueType driver and delegate
      the rest of the work to it, by calling FT_Open_Face().

    - Some suitcase fonts (notably Onyx) might point the `LWFN' file to
      itself, even though it doesn't contains `POST' resources.  To handle
      this special case without opening the file an extra time, we just
      ignore errors from the `LWFN' and fallback to the `sfnt' if both are
      available.
  */


#شامل <freetype/freetype.h>
#شامل <freetype/tttags.h>
#شامل <freetype/internal/ftdebug.h>
#شامل <freetype/internal/ftstream.h>
#شامل "ftbase.h"


#ifdef FT_MACINTOSH

  /* This is for Mac OS X.  Without redefinition, OS_INLINE */
  /* expands to `static inline' which doesn't survive the   */
  /* -ansi compilation flag of GCC.                         */
#اگر !HAVE_ANSI_OS_INLINE
#undef  OS_INLINE
#define OS_INLINE  ایستا __inline__
#endif

  /* `configure' checks the availability of `ResourceIndex' strictly */
  /* and sets HAVE_TYPE_RESOURCE_INDEX 1 or 0 always.  If it is      */
  /* not set (e.g., a build without `configure'), the availability   */
  /* is guessed from the SDK version.                                */
#ifndef HAVE_TYPE_RESOURCE_INDEX
#اگر !defined( MAC_OS_X_VERSION_10_5 ) || \
    ( MAC_OS_X_VERSION_MAX_ALLOWED < MAC_OS_X_VERSION_10_5 )
#define HAVE_TYPE_RESOURCE_INDEX 0
#وگرنه
#define HAVE_TYPE_RESOURCE_INDEX 1
#endif
#endif /* !HAVE_TYPE_RESOURCE_INDEX */

#اگر ( HAVE_TYPE_RESOURCE_INDEX == 0 )
  تعریفنوع short  ResourceIndex;
#endif

#شامل <CoreServices/CoreServices.h>
#شامل <ApplicationServices/ApplicationServices.h>
#شامل <sys/syslimits.h> /* PATH_MAX */

  /* Don't want warnings about our own use of deprecated functions. */
#define FT_DEPRECATED_ATTRIBUTE

#شامل <freetype/ftmac.h>

#ifndef kATSOptionFlagsUnRestrictedScope /* since Mac OS X 10.1 */
#define kATSOptionFlagsUnRestrictedScope kATSOptionFlagsDefault
#endif


  /* Set PREFER_LWFN to 1 if LWFN (Type 1) is preferred over
     TrueType in case *both* are available (this is not common,
     but it *is* possible). */
#ifndef PREFER_LWFN
#define PREFER_LWFN  1
#endif


  /* This function is deprecated because FSSpec is deprecated in Mac OS X  */
  FT_EXPORT_DEF( FT_Error )
  FT_GetFile_From_Mac_Name( ثابت نویسه*  fontName,
                            FSSpec*      pathSpec,
                            FT_Long*     face_index )
  {
    FT_UNUSED( fontName );
    FT_UNUSED( pathSpec );
    FT_UNUSED( face_index );

    بازگشت FT_THROW( Unimplemented_Feature );
  }


  /* Private function.                                         */
  /* The FSSpec type has been discouraged for a long time,     */
  /* unfortunately an FSRef replacement API for                */
  /* ATSFontGetFileSpecification() is only available in        */
  /* Mac OS X 10.5 and later.                                  */
  ایستا OSStatus
  FT_ATSFontGetFileReference( ATSFontRef  ats_font_id,
                              FSRef*      ats_font_ref )
  {
#اگر defined( MAC_OS_X_VERSION_10_5 ) && \
    ( MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_5 )

    OSStatus  err;

    err = ATSFontGetFileReference( ats_font_id, ats_font_ref );

    بازگشت err;
#وگرنهاگر __LP64__ /* No 64bit Carbon API on legacy platforms */
    FT_UNUSED( ats_font_id );
    FT_UNUSED( ats_font_ref );


    بازگشت fnfErr;
#وگرنه /* 32bit Carbon API on legacy platforms */
    OSStatus  err;
    FSSpec    spec;


    err = ATSFontGetFileSpecification( ats_font_id, &spec );
    اگر ( noErr == err )
      err = FSpMakeFSRef( &spec, ats_font_ref );

    بازگشت err;
#endif
  }


  ایستا FT_Error
  FT_GetFileRef_From_Mac_ATS_Name( ثابت نویسه*  fontName,
                                   FSRef*       ats_font_ref,
                                   FT_Long*     face_index )
  {
    CFStringRef  cf_fontName;
    ATSFontRef   ats_font_id;


    *face_index = 0;

    cf_fontName = CFStringCreateWithCString( پوچ, fontName,
                                             kCFStringEncodingMacRoman );
    ats_font_id = ATSFontFindFromName( cf_fontName,
                                       kATSOptionFlagsUnRestrictedScope );
    CFRelease( cf_fontName );

    اگر ( ats_font_id == 0 || ats_font_id == 0xFFFFFFFFUL )
      بازگشت FT_THROW( Unknown_File_Format );

    اگر ( noErr != FT_ATSFontGetFileReference( ats_font_id, ats_font_ref ) )
      بازگشت FT_THROW( Unknown_File_Format );

    /* face_index calculation by searching preceding fontIDs */
    /* with same FSRef                                       */
    {
      ATSFontRef  id2 = ats_font_id - 1;
      FSRef       ref2;


      وقتی ( id2 > 0 )
      {
        اگر ( noErr != FT_ATSFontGetFileReference( id2, &ref2 ) )
          بشکن;
        اگر ( noErr != FSCompareFSRefs( ats_font_ref, &ref2 ) )
          بشکن;

        id2 --;
      }
      *face_index = ats_font_id - ( id2 + 1 );
    }

    بازگشت FT_Err_Ok;
  }


  FT_EXPORT_DEF( FT_Error )
  FT_GetFilePath_From_Mac_ATS_Name( ثابت نویسه*  fontName,
                                    UInt8*       path,
                                    UInt32       maxPathSize,
                                    FT_Long*     face_index )
  {
    FSRef     ref;
    FT_Error  err;


    اگر ( !fontName || !face_index )
      بازگشت FT_THROW( Invalid_Argument);

    err = FT_GetFileRef_From_Mac_ATS_Name( fontName, &ref, face_index );
    اگر ( err )
      بازگشت err;

    اگر ( noErr != FSRefMakePath( &ref, path, maxPathSize ) )
      بازگشت FT_THROW( Unknown_File_Format );

    بازگشت FT_Err_Ok;
  }


  /* This function is deprecated because FSSpec is deprecated in Mac OS X  */
  FT_EXPORT_DEF( FT_Error )
  FT_GetFile_From_Mac_ATS_Name( ثابت نویسه*  fontName,
                                FSSpec*      pathSpec,
                                FT_Long*     face_index )
  {
#اگر ( __LP64__ ) || ( defined( MAC_OS_X_VERSION_10_5 ) && \
      ( MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_5 ) )
    FT_UNUSED( fontName );
    FT_UNUSED( pathSpec );
    FT_UNUSED( face_index );

    بازگشت FT_THROW( Unimplemented_Feature );
#وگرنه
    FSRef     ref;
    FT_Error  err;


    اگر ( !fontName || !face_index )
      بازگشت FT_THROW( Invalid_Argument );

    err = FT_GetFileRef_From_Mac_ATS_Name( fontName, &ref, face_index );
    اگر ( err )
      بازگشت err;

    اگر ( noErr != FSGetCatalogInfo( &ref, kFSCatInfoNone, پوچ, پوچ,
                                    pathSpec, پوچ ) )
      بازگشت FT_THROW( Unknown_File_Format );

    بازگشت FT_Err_Ok;
#endif
  }


  ایستا OSErr
  FT_FSPathMakeRes( ثابت UInt8*    pathname,
                    Res_File_Ref_رقم*  res )
  {
    OSErr  err;
    FSRef  ref;


    اگر ( noErr != FSPathMakeRef( pathname, &ref, FALSE ) )
      بازگشت FT_THROW( Cannot_Open_Resource );

    /* at present, no support for dfont format */
    err = FSOpenResourceFile( &ref, 0, پوچ, fsRdPerm, res );
    اگر ( noErr == err )
      بازگشت err;

    /* fallback to original resource-fork font */
    *res = FSOpenResFile( &ref, fsRdPerm );
    err  = ResError();

    بازگشت err;
  }


  /* Return the file type for given pathname */
  ایستا OSType
  get_file_type_from_path( ثابت UInt8*  pathname )
  {
    FSRef          ref;
    FSCatalogInfo  info;


    اگر ( noErr != FSPathMakeRef( pathname, &ref, FALSE ) )
      بازگشت ( OSType ) 0;

    اگر ( noErr != FSGetCatalogInfo( &ref, kFSCatInfoFinderInfo, &info,
                                    پوچ, پوچ, پوچ ) )
      بازگشت ( OSType ) 0;

    بازگشت ( (FInfo *)( info.finderInfo ) )->fdType;
  }


  /* Given a PostScript font name, create the Macintosh LWFN file name. */
  ایستا تهی
  create_lwfn_name( نویسه*   ps_name,
                    Str255  lwfn_file_name )
  {
    صحیح       max = 5, شمار = 0;
    FT_Byte*  p = lwfn_file_name;
    FT_Byte*  q = (FT_Byte*)ps_name;


    lwfn_file_name[0] = 0;

    وقتی ( *q )
    {
      اگر ( ft_isupper( *q ) )
      {
        اگر ( شمار )
          max = 3;
        شمار = 0;
      }
      اگر ( شمار < max && ( ft_isalnum( *q ) || *q == '_' ) )
      {
        *++p = *q;
        lwfn_file_name[0]++;
        شمار++;
      }
      q++;
    }
  }


  ایستا short
  شمار_faces_sfnt( نویسه*  fond_data )
  {
    /* The count is 1 greater than the value in the FOND.  */
    /* Isn't that cute? :-)                                */

    بازگشت EndianS16_BtoN( *( (short*)( fond_data +
                                        اندازه ( FamRec ) ) ) ) + 1;
  }


  ایستا short
  شمار_faces_scalable( نویسه*  fond_data )
  {
    AsscEntry*  assoc;
    short       i, face, face_all;


    face_all = EndianS16_BtoN( *( (short *)( fond_data +
                                             اندازه ( FamRec ) ) ) ) + 1;
    assoc    = (AsscEntry*)( fond_data + اندازه ( FamRec ) + 2 );
    face     = 0;

    برای ( i = 0; i < face_all; i++ )
    {
      اگر ( 0 == EndianS16_BtoN( assoc[i].fontSize ) )
        face++;
    }
    بازگشت face;
  }


  /* Look inside the FOND data, answer whether there should be an SFNT
     resource, and answer the name of a possible LWFN Type 1 file.

     Thanks to Paul Miller (paulm@profoundeffects.com) for the fix
     to load a face OTHER than the first one in the FOND!
  */


  ایستا تهی
  parse_fond( نویسه*   fond_data,
              short*  have_sfnt,
              ResID*  sfnt_id,
              Str255  lwfn_file_name,
              short   face_index )
  {
    AsscEntry*  assoc;
    AsscEntry*  base_assoc;
    FamRec*     fond;


    *sfnt_id          = 0;
    *have_sfnt        = 0;
    lwfn_file_name[0] = 0;

    fond       = (FamRec*)fond_data;
    assoc      = (AsscEntry*)( fond_data + اندازه ( FamRec ) + 2 );
    base_assoc = assoc;

    /* the maximum faces in a FOND is 48, size of StyleTable.indexes[] */
    اگر ( 47 < face_index )
      بازگشت;

    /* Let's do a little range checking before we get too excited here */
    اگر ( face_index < شمار_faces_sfnt( fond_data ) )
    {
      assoc += face_index;        /* add on the face_index! */

      /* if the face at this index is not scalable,
         fall back to the first one (old behavior) */
      اگر ( EndianS16_BtoN( assoc->fontSize ) == 0 )
      {
        *have_sfnt = 1;
        *sfnt_id   = EndianS16_BtoN( assoc->fontID );
      }
      وگرنه اگر ( base_assoc->fontSize == 0 )
      {
        *have_sfnt = 1;
        *sfnt_id   = EndianS16_BtoN( base_assoc->fontID );
      }
    }

    اگر ( EndianS32_BtoN( fond->ffStylOff ) )
    {
      unsigned نویسه*  p = (unsigned نویسه*)fond_data;
      StyleTable*     style;
      unsigned short  string_شمار;
      نویسه            ps_name[256];
      unsigned نویسه*  names[64];
      صحیح             i;


      p += EndianS32_BtoN( fond->ffStylOff );
      style = (StyleTable*)p;
      p += اندازه ( StyleTable );
      string_شمار = EndianS16_BtoN( *(short*)(p) );
      string_شمار = FT_MIN( 64, string_شمار );
      p += اندازه ( short );

      برای ( i = 0; i < string_شمار; i++ )
      {
        names[i] = p;
        p       += names[i][0];
        p++;
      }

      {
        size_t  ps_name_len = (size_t)names[0][0];


        اگر ( ps_name_len != 0 )
        {
          ft_memcpy( ps_name, names[0] + 1, ps_name_len );
          ps_name[ps_name_len] = 0;
        }
        اگر ( style->indexes[face_index] > 1 &&
             style->indexes[face_index] <= string_شمار )
        {
          unsigned نویسه*  suffixes = names[style->indexes[face_index] - 1];


          برای ( i = 1; i <= suffixes[0]; i++ )
          {
            unsigned نویسه*  s;
            size_t          j = suffixes[i] - 1;


            اگر ( j < string_شمار && ( s = names[j] ) != پوچ )
            {
              size_t  s_len = (size_t)s[0];


              اگر ( s_len != 0 && ps_name_len + s_len < اندازه ( ps_name ) )
              {
                ft_memcpy( ps_name + ps_name_len, s + 1, s_len );
                ps_name_len += s_len;
                ps_name[ps_name_len] = 0;
              }
            }
          }
        }
      }

      create_lwfn_name( ps_name, lwfn_file_name );
    }
  }


  ایستا  FT_Error
  lookup_lwfn_by_fond( ثابت UInt8*      path_fond,
                       ConstStr255Param  base_lwfn,
                       UInt8*            path_lwfn,
                       size_t            path_size )
  {
    FSRef   ref, par_ref;
    size_t  dirname_len;


    /* Pathname for FSRef can be in various formats: HFS, HFS+, and POSIX. */
    /* We should not extract parent directory by string manipulation.      */

    اگر ( noErr != FSPathMakeRef( path_fond, &ref, FALSE ) )
      بازگشت FT_THROW( Invalid_Argument );

    اگر ( noErr != FSGetCatalogInfo( &ref, kFSCatInfoNone,
                                    پوچ, پوچ, پوچ, &par_ref ) )
      بازگشت FT_THROW( Invalid_Argument );

    اگر ( noErr != FSRefMakePath( &par_ref, path_lwfn, path_size ) )
      بازگشت FT_THROW( Invalid_Argument );

    اگر ( ft_strlen( (نویسه *)path_lwfn ) + 1 + base_lwfn[0] > path_size )
      بازگشت FT_THROW( Invalid_Argument );

    /* now we have absolute dirname in path_lwfn */
    ft_strcat( (نویسه *)path_lwfn, "/" );
    dirname_len = ft_strlen( (نویسه *)path_lwfn );
    ft_strcat( (نویسه *)path_lwfn, (نویسه *)base_lwfn + 1 );
    path_lwfn[dirname_len + base_lwfn[0]] = '\0';

    اگر ( noErr != FSPathMakeRef( path_lwfn, &ref, FALSE ) )
      بازگشت FT_THROW( Cannot_Open_Resource );

    اگر ( noErr != FSGetCatalogInfo( &ref, kFSCatInfoNone,
                                    پوچ, پوچ, پوچ, پوچ ) )
      بازگشت FT_THROW( Cannot_Open_Resource );

    بازگشت FT_Err_Ok;
  }


  ایستا short
  شمار_faces( Handle        fond,
               ثابت UInt8*  pathname )
  {
    ResID     sfnt_id;
    short     have_sfnt, have_lwfn;
    Str255    lwfn_file_name;
    UInt8     buff[PATH_MAX];
    FT_Error  err;
    short     رقم_faces;


    have_sfnt = have_lwfn = 0;

    parse_fond( *fond, &have_sfnt, &sfnt_id, lwfn_file_name, 0 );

    اگر ( lwfn_file_name[0] )
    {
      err = lookup_lwfn_by_fond( pathname, lwfn_file_name,
                                 buff, اندازه ( buff ) );
      اگر ( !err )
        have_lwfn = 1;
    }

    اگر ( have_lwfn && ( !have_sfnt || PREFER_LWFN ) )
      رقم_faces = 1;
    وگرنه
      رقم_faces = شمار_faces_scalable( *fond );

    بازگشت رقم_faces;
  }


  /* Read Type 1 data from the POST resources inside the LWFN file,
     return a PFB buffer.  This is somewhat convoluted because the FT2
     PFB parser wants the ASCII header as one chunk, and the LWFN
     chunks are often not organized that way, so we glue chunks
     of the same type together. */
  ایستا FT_Error
  read_lwfn( FT_Memory      memory,
             Res_File_Ref_رقم  res,
             FT_Byte**      pfb_data,
             FT_ULong*      size )
  {
    FT_Error       error = FT_Err_Ok;
    ResID          res_id;
    unsigned نویسه  *buffer, *p, *size_p = پوچ;
    FT_ULong       total_size = 0;
    FT_ULong       old_total_size = 0;
    FT_ULong       post_size, pfb_chunk_size;
    Handle         post_data;
    نویسه           code, last_code;


    UseResFile( res );

    /* First pass: load all POST resources, and determine the size of */
    /* the output buffer.                                             */
    res_id    = 501;
    last_code = -1;

    برای (;;)
    {
      post_data = Get1Resource( TTAG_POST, res_id++ );
      اگر ( !post_data )
        بشکن;  /* we are done */

      code = (*post_data)[0];

      اگر ( code != last_code )
      {
        اگر ( code == 5 )
          total_size += 2; /* just the end code */
        وگرنه
          total_size += 6; /* code + 4 bytes chunk length */
      }

      total_size += (FT_ULong)GetHandleSize( post_data ) - 2;
      last_code = code;

      /* detect resource fork overflow */
      اگر ( FT_MAC_RFORK_MAX_LEN < total_size )
      {
        error = FT_THROW( Array_Too_Large );
        برو Error;
      }

      old_total_size = total_size;
    }

    اگر ( FT_QALLOC( buffer, (FT_Long)total_size ) )
      برو Error;

    /* Second pass: append all POST data to the buffer, add PFB fields. */
    /* Glue all consecutive chunks of the same type together.           */
    p              = buffer;
    res_id         = 501;
    last_code      = -1;
    pfb_chunk_size = 0;

    برای (;;)
    {
      post_data = Get1Resource( TTAG_POST, res_id++ );
      اگر ( !post_data )
        بشکن;  /* we are done */

      post_size = (FT_ULong)GetHandleSize( post_data ) - 2;
      code = (*post_data)[0];

      اگر ( code != last_code )
      {
        اگر ( last_code != -1 )
        {
          /* we are done adding a chunk, fill in the size field */
          اگر ( size_p )
          {
            *size_p++ = (FT_Byte)(   pfb_chunk_size         & 0xFF );
            *size_p++ = (FT_Byte)( ( pfb_chunk_size >> 8  ) & 0xFF );
            *size_p++ = (FT_Byte)( ( pfb_chunk_size >> 16 ) & 0xFF );
            *size_p++ = (FT_Byte)( ( pfb_chunk_size >> 24 ) & 0xFF );
          }
          pfb_chunk_size = 0;
        }

        *p++ = 0x80;
        اگر ( code == 5 )
          *p++ = 0x03;  /* the end */
        وگرنه اگر ( code == 2 )
          *p++ = 0x02;  /* binary segment */
        وگرنه
          *p++ = 0x01;  /* ASCII segment */

        اگر ( code != 5 )
        {
          size_p = p;   /* save for later */
          p += 4;       /* make space for size field */
        }
      }

      ft_memcpy( p, *post_data + 2, post_size );
      pfb_chunk_size += post_size;
      p += post_size;
      last_code = code;
    }

    *pfb_data = buffer;
    *size = total_size;

  Error:
    CloseResFile( res );
    بازگشت error;
  }


  /* Create a new FT_Face from a file path to an LWFN file. */
  ایستا FT_Error
  FT_New_Face_From_LWFN( FT_Library    library,
                         ثابت UInt8*  pathname,
                         FT_Long       face_index,
                         FT_Face*      aface )
  {
    FT_Byte*       pfb_data;
    FT_ULong       pfb_size;
    FT_Error       error;
    Res_File_Ref_رقم  res;


    اگر ( noErr != FT_FSPathMakeRes( pathname, &res ) )
      بازگشت FT_THROW( Cannot_Open_Resource );

    pfb_data = پوچ;
    pfb_size = 0;
    error = read_lwfn( library->memory, res, &pfb_data, &pfb_size );
    CloseResFile( res ); /* PFB is already loaded, useless anymore */
    اگر ( error )
      بازگشت error;

    بازگشت open_face_from_buffer( library,
                                  pfb_data,
                                  pfb_size,
                                  face_index,
                                  "type1",
                                  aface );
  }


  /* Create a new FT_Face from an SFNT resource, specified by res ID. */
  ایستا FT_Error
  FT_New_Face_From_SFNT( FT_Library  library,
                         ResID       sfnt_id,
                         FT_Long     face_index,
                         FT_Face*    aface )
  {
    Handle     sfnt = پوچ;
    FT_Byte*   sfnt_data;
    size_t     sfnt_size;
    FT_Error   error  = FT_Err_Ok;
    FT_Memory  memory = library->memory;
    صحیح        is_cff, is_sfnt_ps;


    sfnt = GetResource( TTAG_sfnt, sfnt_id );
    اگر ( !sfnt )
      بازگشت FT_THROW( Invalid_Handle );

    sfnt_size = (FT_ULong)GetHandleSize( sfnt );

    /* detect resource fork overflow */
    اگر ( FT_MAC_RFORK_MAX_LEN < sfnt_size )
      بازگشت FT_THROW( Array_Too_Large );

    اگر ( FT_QALLOC( sfnt_data, (FT_Long)sfnt_size ) )
    {
      ReleaseResource( sfnt );
      بازگشت error;
    }

    ft_memcpy( sfnt_data, *sfnt, sfnt_size );
    ReleaseResource( sfnt );

    is_cff     = sfnt_size > 4 && !ft_memcmp( sfnt_data, "OTTO", 4 );
    is_sfnt_ps = sfnt_size > 4 && !ft_memcmp( sfnt_data, "typ1", 4 );

    اگر ( is_sfnt_ps )
    {
      FT_Stream  stream;


      اگر ( FT_NEW( stream ) )
        برو Try_OpenType;

      FT_Stream_OpenMemory( stream, sfnt_data, sfnt_size );
      اگر ( !open_face_PS_from_sfnt_stream( library,
                                           stream,
                                           face_index,
                                           0, پوچ,
                                           aface ) )
      {
        FT_Stream_Close( stream );
        FT_FREE( stream );
        FT_FREE( sfnt_data );
        برو Exit;
      }

      FT_FREE( stream );
    }
  Try_OpenType:
    error = open_face_from_buffer( library,
                                   sfnt_data,
                                   sfnt_size,
                                   face_index,
                                   is_cff ? "cff" : "truetype",
                                   aface );
  Exit:
    بازگشت error;
  }


  /* Create a new FT_Face from a file path to a suitcase file. */
  ایستا FT_Error
  FT_New_Face_From_Suitcase( FT_Library    library,
                             ثابت UInt8*  pathname,
                             FT_Long       face_index,
                             FT_Face*      aface )
  {
    FT_Error       error = FT_ERR( Cannot_Open_Resource );
    Res_File_Ref_رقم  res_ref;
    ResourceIndex  res_index;
    Handle         fond;
    short          رقم_faces_in_res;
    FT_Long        شمار;


    اگر ( noErr != FT_FSPathMakeRes( pathname, &res_ref ) )
      بازگشت FT_THROW( Cannot_Open_Resource );

    UseResFile( res_ref );
    اگر ( ResError() )
      بازگشت FT_THROW( Cannot_Open_Resource );

    res_index        = 1;
    رقم_faces_in_res = 0;
    شمار            = face_index;
    وقتی ( شمار >= 0 )
    {
      short  رقم_faces_in_fond;


      fond = Get1IndResource( TTAG_FOND, res_index );
      اگر ( ResError() )
        بشکن;

      رقم_faces_in_fond  = شمار_faces( fond, pathname );
      رقم_faces_in_res  += رقم_faces_in_fond;

      اگر ( شمار < رقم_faces_in_fond )
        error = FT_New_Face_From_FOND( library, fond, شمار, aface );

      res_index++;
      شمار -= رقم_faces_in_fond;
    }

    CloseResFile( res_ref );

    اگر ( !error && aface && *aface )
    {
      (*aface)->رقم_faces  = رقم_faces_in_res;
      (*aface)->face_index = face_index;
    }

    بازگشت error;
  }


  /* documentation is in ftmac.h */

  FT_EXPORT_DEF( FT_Error )
  FT_New_Face_From_FOND( FT_Library  library,
                         Handle      fond,
                         FT_Long     face_index,
                         FT_Face*    aface )
  {
    short     have_sfnt, have_lwfn = 0;
    ResID     sfnt_id, fond_id;
    OSType    fond_type;
    Str255    fond_name;
    Str255    lwfn_file_name;
    UInt8     path_lwfn[PATH_MAX];
    OSErr     err;
    FT_Error  error = FT_Err_Ok;


    /* check of `library' and `aface' delayed to `FT_New_Face_From_XXX' */

    GetResInfo( fond, &fond_id, &fond_type, fond_name );
    اگر ( ResError() != noErr || fond_type != TTAG_FOND )
      بازگشت FT_THROW( Invalid_File_Format );

    parse_fond( *fond, &have_sfnt, &sfnt_id, lwfn_file_name, face_index );

    اگر ( lwfn_file_name[0] )
    {
      Res_File_Ref_رقم  res;


      res = HomeResFile( fond );
      اگر ( noErr != ResError() )
        برو found_no_lwfn_file;

      {
        UInt8  path_fond[PATH_MAX];
        FSRef  ref;


        err = FSGetForkCBInfo( res, k_FSInvalid_Volume_Ref_رقم,
                               پوچ, پوچ, پوچ, &ref, پوچ );
        اگر ( noErr != err )
          برو found_no_lwfn_file;

        err = FSRefMakePath( &ref, path_fond, اندازه ( path_fond ) );
        اگر ( noErr != err )
          برو found_no_lwfn_file;

        error = lookup_lwfn_by_fond( path_fond, lwfn_file_name,
                                     path_lwfn, اندازه ( path_lwfn ) );
        اگر ( !error )
          have_lwfn = 1;
      }
    }

    اگر ( have_lwfn && ( !have_sfnt || PREFER_LWFN ) )
      error = FT_New_Face_From_LWFN( library,
                                     path_lwfn,
                                     face_index,
                                     aface );
    وگرنه
      error = FT_THROW( Unknown_File_Format );

  found_no_lwfn_file:
    اگر ( have_sfnt && error )
      error = FT_New_Face_From_SFNT( library,
                                     sfnt_id,
                                     face_index,
                                     aface );

    بازگشت error;
  }


  /* Common function to load a new FT_Face from a resource file. */
  ایستا FT_Error
  FT_New_Face_From_Resource( FT_Library    library,
                             ثابت UInt8*  pathname,
                             FT_Long       face_index,
                             FT_Face*      aface )
  {
    OSType    file_type;
    FT_Error  error;


    /* LWFN is a (very) specific file format, check for it explicitly */
    file_type = get_file_type_from_path( pathname );
    اگر ( file_type == TTAG_LWFN )
      بازگشت FT_New_Face_From_LWFN( library, pathname, face_index, aface );

    /* Otherwise the file type doesn't matter (there are more than  */
    /* `FFIL' and `tfil').  Just try opening it as a font suitcase; */
    /* if it works, fine.                                           */

    error = FT_New_Face_From_Suitcase( library, pathname, face_index, aface );
    اگر ( error )
    {
      /* let it fall through to normal loader (.ttf, .otf, etc.); */
      /* we signal this by returning no error and no FT_Face      */
      *aface = پوچ;
    }

    بازگشت FT_Err_Ok;
  }


  /**************************************************************************
   *
   * @Function:
   *   FT_New_Face
   *
   * @Description:
   *   This is the Mac-specific implementation of FT_New_Face.  In
   *   addition to the standard FT_New_Face() functionality, it also
   *   accepts pathnames to Mac suitcase files.  For further
   *   documentation see the original FT_New_Face() in freetype.h.
   */
  FT_EXPORT_DEF( FT_Error )
  FT_New_Face( FT_Library   library,
               ثابت نویسه*  pathname,
               FT_Long      face_index,
               FT_Face*     aface )
  {
    FT_Open_Args  args;
    FT_Error      error;


    /* test for valid `library' and `aface' delayed to FT_Open_Face() */
    اگر ( !pathname )
      بازگشت FT_THROW( Invalid_Argument );

    *aface = پوچ;

    /* try resourcefork based font: LWFN, FFIL */
    error = FT_New_Face_From_Resource( library, (UInt8 *)pathname,
                                       face_index, aface );
    اگر ( error || *aface )
      بازگشت error;

    /* let it fall through to normal loader (.ttf, .otf, etc.) */
    args.flags    = FT_OPEN_PATHNAME;
    args.pathname = (نویسه*)pathname;

    بازگشت FT_Open_Face( library, &args, face_index, aface );
  }


  /**************************************************************************
   *
   * @Function:
   *   FT_New_Face_From_FSRef
   *
   * @Description:
   *   FT_New_Face_From_FSRef is identical to FT_New_Face except it
   *   accepts an FSRef instead of a path.
   *
   * This function is deprecated because Carbon data types (FSRef)
   * are not cross-platform, and thus not suitable for the FreeType API.
   */
  FT_EXPORT_DEF( FT_Error )
  FT_New_Face_From_FSRef( FT_Library    library,
                          ثابت FSRef*  ref,
                          FT_Long       face_index,
                          FT_Face*      aface )
  {
    FT_Error      error;
    FT_Open_Args  args;

    OSErr  err;
    UInt8  pathname[PATH_MAX];


    /* check of `library' and `aface' delayed to */
    /* `FT_New_Face_From_Resource'               */

    اگر ( !ref )
      بازگشت FT_THROW( Invalid_Argument );

    err = FSRefMakePath( ref, pathname, اندازه ( pathname ) );
    اگر ( err )
      error = FT_THROW( Cannot_Open_Resource );

    error = FT_New_Face_From_Resource( library, pathname, face_index, aface );
    اگر ( error || *aface )
      بازگشت error;

    /* fallback to datafork font */
    args.flags    = FT_OPEN_PATHNAME;
    args.pathname = (نویسه*)pathname;
    بازگشت FT_Open_Face( library, &args, face_index, aface );
  }


  /**************************************************************************
   *
   * @Function:
   *   FT_New_Face_From_FSSpec
   *
   * @Description:
   *   FT_New_Face_From_FSSpec is identical to FT_New_Face except it
   *   accepts an FSSpec instead of a path.
   *
   * This function is deprecated because FSSpec is deprecated in Mac OS X
   */
  FT_EXPORT_DEF( FT_Error )
  FT_New_Face_From_FSSpec( FT_Library     library,
                           ثابت FSSpec*  spec,
                           FT_Long        face_index,
                           FT_Face*       aface )
  {
#اگر ( __LP64__ ) || ( defined( MAC_OS_X_VERSION_10_5 ) && \
      ( MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_5 ) )
    FT_UNUSED( library );
    FT_UNUSED( spec );
    FT_UNUSED( face_index );
    FT_UNUSED( aface );

    بازگشت FT_THROW( Unimplemented_Feature );
#وگرنه
    FSRef  ref;


    /* check of `library' and `aface' delayed to `FT_New_Face_From_FSRef' */

    اگر ( !spec || FSpMakeFSRef( spec, &ref ) != noErr )
      بازگشت FT_THROW( Invalid_Argument );
    وگرنه
      بازگشت FT_New_Face_From_FSRef( library, &ref, face_index, aface );
#endif
  }

#وگرنه /* !FT_MACINTOSH */

  /* ANSI C doesn't like empty source files */
  تعریفنوع صحیح  ft_mac_dummy_;

#endif /* !FT_MACINTOSH */


/* END */
