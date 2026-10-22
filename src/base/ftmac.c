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


#अन्तर्भाव <freetype/freetype.h>
#अन्तर्भाव <freetype/tttags.h>
#अन्तर्भाव <freetype/internal/ftdebug.h>
#अन्तर्भाव <freetype/internal/ftstream.h>
#अन्तर्भाव "ftbase.h"


#ifdef FT_MACINTOSH

  /* This is for Mac OS X.  Without redefinition, OS_INLINE */
  /* expands to `static inline' which doesn't survive the   */
  /* -ansi compilation flag of GCC.                         */
#जइ !HAVE_ANSI_OS_INLINE
#undef  OS_INLINE
#define OS_INLINE  स्थिर __inline__
#endif

  /* `configure' checks the availability of `ResourceIndex' strictly */
  /* and sets HAVE_TYPE_RESOURCE_INDEX 1 or 0 always.  If it is      */
  /* not set (e.g., a build without `configure'), the availability   */
  /* is guessed from the SDK version.                                */
#ifndef HAVE_TYPE_RESOURCE_INDEX
#जइ !defined( MAC_OS_X_VERSION_10_5 ) || \
    ( MAC_OS_X_VERSION_MAX_ALLOWED < MAC_OS_X_VERSION_10_5 )
#define HAVE_TYPE_RESOURCE_INDEX 0
#अण्णहा
#define HAVE_TYPE_RESOURCE_INDEX 1
#endif
#endif /* !HAVE_TYPE_RESOURCE_INDEX */

#जइ ( HAVE_TYPE_RESOURCE_INDEX == 0 )
  प्रकारनिर्देश short  ResourceIndex;
#endif

#अन्तर्भाव <CoreServices/CoreServices.h>
#अन्तर्भाव <ApplicationServices/ApplicationServices.h>
#अन्तर्भाव <sys/syslimits.h> /* PATH_MAX */

  /* Don't want warnings about our own use of deprecated functions. */
#define FT_DEPRECATED_ATTRIBUTE

#अन्तर्भाव <freetype/ftmac.h>

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
  FT_GetFile_From_Mac_Name( ध्रुव वर्ण*  fontName,
                            FSSpec*      pathSpec,
                            FT_Long*     face_index )
  {
    FT_UNUSED( fontName );
    FT_UNUSED( pathSpec );
    FT_UNUSED( face_index );

    प्रत्यागम FT_THROW( Unimplemented_Feature );
  }


  /* Private function.                                         */
  /* The FSSpec type has been discouraged for a long time,     */
  /* unfortunately an FSRef replacement API for                */
  /* ATSFontGetFileSpecification() is only available in        */
  /* Mac OS X 10.5 and later.                                  */
  स्थिर OSStatus
  FT_ATSFontGetFileReference( ATSFontRef  ats_font_id,
                              FSRef*      ats_font_ref )
  {
#जइ defined( MAC_OS_X_VERSION_10_5 ) && \
    ( MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_5 )

    OSStatus  err;

    err = ATSFontGetFileReference( ats_font_id, ats_font_ref );

    प्रत्यागम err;
#नोचेद्यदि __LP64__ /* No 64bit Carbon API on legacy platforms */
    FT_UNUSED( ats_font_id );
    FT_UNUSED( ats_font_ref );


    प्रत्यागम fnfErr;
#अण्णहा /* 32bit Carbon API on legacy platforms */
    OSStatus  err;
    FSSpec    spec;


    err = ATSFontGetFileSpecification( ats_font_id, &spec );
    जइ ( noErr == err )
      err = FSpMakeFSRef( &spec, ats_font_ref );

    प्रत्यागम err;
#endif
  }


  स्थिर FT_Error
  FT_GetFileRef_From_Mac_ATS_Name( ध्रुव वर्ण*  fontName,
                                   FSRef*       ats_font_ref,
                                   FT_Long*     face_index )
  {
    CFStringRef  cf_fontName;
    ATSFontRef   ats_font_id;


    *face_index = 0;

    cf_fontName = CFStringCreateWithCString( रिक्त, fontName,
                                             kCFStringEncodingMacRoman );
    ats_font_id = ATSFontFindFromName( cf_fontName,
                                       kATSOptionFlagsUnRestrictedScope );
    CFRelease( cf_fontName );

    जइ ( ats_font_id == 0 || ats_font_id == 0xFFFFFFFFUL )
      प्रत्यागम FT_THROW( Unknown_File_Format );

    जइ ( noErr != FT_ATSFontGetFileReference( ats_font_id, ats_font_ref ) )
      प्रत्यागम FT_THROW( Unknown_File_Format );

    /* face_index calculation by searching preceding fontIDs */
    /* with same FSRef                                       */
    {
      ATSFontRef  id2 = ats_font_id - 1;
      FSRef       ref2;


      जाव ( id2 > 0 )
      {
        जइ ( noErr != FT_ATSFontGetFileReference( id2, &ref2 ) )
          भङ्ग;
        जइ ( noErr != FSCompareFSRefs( ats_font_ref, &ref2 ) )
          भङ्ग;

        id2 --;
      }
      *face_index = ats_font_id - ( id2 + 1 );
    }

    प्रत्यागम FT_Err_Ok;
  }


  FT_EXPORT_DEF( FT_Error )
  FT_GetFilePath_From_Mac_ATS_Name( ध्रुव वर्ण*  fontName,
                                    UInt8*       path,
                                    UInt32       maxPathSize,
                                    FT_Long*     face_index )
  {
    FSRef     ref;
    FT_Error  err;


    जइ ( !fontName || !face_index )
      प्रत्यागम FT_THROW( Invalid_Argument);

    err = FT_GetFileRef_From_Mac_ATS_Name( fontName, &ref, face_index );
    जइ ( err )
      प्रत्यागम err;

    जइ ( noErr != FSRefMakePath( &ref, path, maxPathSize ) )
      प्रत्यागम FT_THROW( Unknown_File_Format );

    प्रत्यागम FT_Err_Ok;
  }


  /* This function is deprecated because FSSpec is deprecated in Mac OS X  */
  FT_EXPORT_DEF( FT_Error )
  FT_GetFile_From_Mac_ATS_Name( ध्रुव वर्ण*  fontName,
                                FSSpec*      pathSpec,
                                FT_Long*     face_index )
  {
#जइ ( __LP64__ ) || ( defined( MAC_OS_X_VERSION_10_5 ) && \
      ( MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_5 ) )
    FT_UNUSED( fontName );
    FT_UNUSED( pathSpec );
    FT_UNUSED( face_index );

    प्रत्यागम FT_THROW( Unimplemented_Feature );
#अण्णहा
    FSRef     ref;
    FT_Error  err;


    जइ ( !fontName || !face_index )
      प्रत्यागम FT_THROW( Invalid_Argument );

    err = FT_GetFileRef_From_Mac_ATS_Name( fontName, &ref, face_index );
    जइ ( err )
      प्रत्यागम err;

    जइ ( noErr != FSGetCatalogInfo( &ref, kFSCatInfoNone, रिक्त, रिक्त,
                                    pathSpec, रिक्त ) )
      प्रत्यागम FT_THROW( Unknown_File_Format );

    प्रत्यागम FT_Err_Ok;
#endif
  }


  स्थिर OSErr
  FT_FSPathMakeRes( ध्रुव UInt8*    pathname,
                    Res_File_Ref_अङ्कः*  res )
  {
    OSErr  err;
    FSRef  ref;


    जइ ( noErr != FSPathMakeRef( pathname, &ref, FALSE ) )
      प्रत्यागम FT_THROW( Cannot_Open_Resource );

    /* at present, no support for dfont format */
    err = FSOpenResourceFile( &ref, 0, रिक्त, fsRdPerm, res );
    जइ ( noErr == err )
      प्रत्यागम err;

    /* fallback to original resource-fork font */
    *res = FSOpenResFile( &ref, fsRdPerm );
    err  = ResError();

    प्रत्यागम err;
  }


  /* Return the file type for given pathname */
  स्थिर OSType
  get_file_type_from_path( ध्रुव UInt8*  pathname )
  {
    FSRef          ref;
    FSCatalogInfo  info;


    जइ ( noErr != FSPathMakeRef( pathname, &ref, FALSE ) )
      प्रत्यागम ( OSType ) 0;

    जइ ( noErr != FSGetCatalogInfo( &ref, kFSCatInfoFinderInfo, &info,
                                    रिक्त, रिक्त, रिक्त ) )
      प्रत्यागम ( OSType ) 0;

    प्रत्यागम ( (FInfo *)( info.finderInfo ) )->fdType;
  }


  /* Given a PostScript font name, create the Macintosh LWFN file name. */
  स्थिर शून्य
  create_lwfn_name( वर्ण*   ps_name,
                    Str255  lwfn_file_name )
  {
    पूर्णाङ्क       max = 5, गणनम् = 0;
    FT_Byte*  p = lwfn_file_name;
    FT_Byte*  q = (FT_Byte*)ps_name;


    lwfn_file_name[0] = 0;

    जाव ( *q )
    {
      जइ ( ft_isupper( *q ) )
      {
        जइ ( गणनम् )
          max = 3;
        गणनम् = 0;
      }
      जइ ( गणनम् < max && ( ft_isalnum( *q ) || *q == '_' ) )
      {
        *++p = *q;
        lwfn_file_name[0]++;
        गणनम्++;
      }
      q++;
    }
  }


  स्थिर short
  गणनम्_faces_sfnt( वर्ण*  fond_data )
  {
    /* The count is 1 greater than the value in the FOND.  */
    /* Isn't that cute? :-)                                */

    प्रत्यागम EndianS16_BtoN( *( (short*)( fond_data +
                                        परिमाण ( FamRec ) ) ) ) + 1;
  }


  स्थिर short
  गणनम्_faces_scalable( वर्ण*  fond_data )
  {
    AsscEntry*  assoc;
    short       i, face, face_all;


    face_all = EndianS16_BtoN( *( (short *)( fond_data +
                                             परिमाण ( FamRec ) ) ) ) + 1;
    assoc    = (AsscEntry*)( fond_data + परिमाण ( FamRec ) + 2 );
    face     = 0;

    प्रति ( i = 0; i < face_all; i++ )
    {
      जइ ( 0 == EndianS16_BtoN( assoc[i].fontSize ) )
        face++;
    }
    प्रत्यागम face;
  }


  /* Look inside the FOND data, answer whether there should be an SFNT
     resource, and answer the name of a possible LWFN Type 1 file.

     Thanks to Paul Miller (paulm@profoundeffects.com) for the fix
     to load a face OTHER than the first one in the FOND!
  */


  स्थिर शून्य
  parse_fond( वर्ण*   fond_data,
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
    assoc      = (AsscEntry*)( fond_data + परिमाण ( FamRec ) + 2 );
    base_assoc = assoc;

    /* the maximum faces in a FOND is 48, size of StyleTable.indexes[] */
    जइ ( 47 < face_index )
      प्रत्यागम;

    /* Let's do a little range checking before we get too excited here */
    जइ ( face_index < गणनम्_faces_sfnt( fond_data ) )
    {
      assoc += face_index;        /* add on the face_index! */

      /* if the face at this index is not scalable,
         fall back to the first one (old behavior) */
      जइ ( EndianS16_BtoN( assoc->fontSize ) == 0 )
      {
        *have_sfnt = 1;
        *sfnt_id   = EndianS16_BtoN( assoc->fontID );
      }
      अण्णहा जइ ( base_assoc->fontSize == 0 )
      {
        *have_sfnt = 1;
        *sfnt_id   = EndianS16_BtoN( base_assoc->fontID );
      }
    }

    जइ ( EndianS32_BtoN( fond->ffStylOff ) )
    {
      unsigned वर्ण*  p = (unsigned वर्ण*)fond_data;
      StyleTable*     style;
      unsigned short  string_गणनम्;
      वर्ण            ps_name[256];
      unsigned वर्ण*  names[64];
      पूर्णाङ्क             i;


      p += EndianS32_BtoN( fond->ffStylOff );
      style = (StyleTable*)p;
      p += परिमाण ( StyleTable );
      string_गणनम् = EndianS16_BtoN( *(short*)(p) );
      string_गणनम् = FT_MIN( 64, string_गणनम् );
      p += परिमाण ( short );

      प्रति ( i = 0; i < string_गणनम्; i++ )
      {
        names[i] = p;
        p       += names[i][0];
        p++;
      }

      {
        size_t  ps_name_len = (size_t)names[0][0];


        जइ ( ps_name_len != 0 )
        {
          ft_memcpy( ps_name, names[0] + 1, ps_name_len );
          ps_name[ps_name_len] = 0;
        }
        जइ ( style->indexes[face_index] > 1 &&
             style->indexes[face_index] <= string_गणनम् )
        {
          unsigned वर्ण*  suffixes = names[style->indexes[face_index] - 1];


          प्रति ( i = 1; i <= suffixes[0]; i++ )
          {
            unsigned वर्ण*  s;
            size_t          j = suffixes[i] - 1;


            जइ ( j < string_गणनम् && ( s = names[j] ) != रिक्त )
            {
              size_t  s_len = (size_t)s[0];


              जइ ( s_len != 0 && ps_name_len + s_len < परिमाण ( ps_name ) )
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


  स्थिर  FT_Error
  lookup_lwfn_by_fond( ध्रुव UInt8*      path_fond,
                       ConstStr255Param  base_lwfn,
                       UInt8*            path_lwfn,
                       size_t            path_size )
  {
    FSRef   ref, par_ref;
    size_t  dirname_len;


    /* Pathname for FSRef can be in various formats: HFS, HFS+, and POSIX. */
    /* We should not extract parent directory by string manipulation.      */

    जइ ( noErr != FSPathMakeRef( path_fond, &ref, FALSE ) )
      प्रत्यागम FT_THROW( Invalid_Argument );

    जइ ( noErr != FSGetCatalogInfo( &ref, kFSCatInfoNone,
                                    रिक्त, रिक्त, रिक्त, &par_ref ) )
      प्रत्यागम FT_THROW( Invalid_Argument );

    जइ ( noErr != FSRefMakePath( &par_ref, path_lwfn, path_size ) )
      प्रत्यागम FT_THROW( Invalid_Argument );

    जइ ( ft_strlen( (वर्ण *)path_lwfn ) + 1 + base_lwfn[0] > path_size )
      प्रत्यागम FT_THROW( Invalid_Argument );

    /* now we have absolute dirname in path_lwfn */
    ft_strcat( (वर्ण *)path_lwfn, "/" );
    dirname_len = ft_strlen( (वर्ण *)path_lwfn );
    ft_strcat( (वर्ण *)path_lwfn, (वर्ण *)base_lwfn + 1 );
    path_lwfn[dirname_len + base_lwfn[0]] = '\0';

    जइ ( noErr != FSPathMakeRef( path_lwfn, &ref, FALSE ) )
      प्रत्यागम FT_THROW( Cannot_Open_Resource );

    जइ ( noErr != FSGetCatalogInfo( &ref, kFSCatInfoNone,
                                    रिक्त, रिक्त, रिक्त, रिक्त ) )
      प्रत्यागम FT_THROW( Cannot_Open_Resource );

    प्रत्यागम FT_Err_Ok;
  }


  स्थिर short
  गणनम्_faces( Handle        fond,
               ध्रुव UInt8*  pathname )
  {
    ResID     sfnt_id;
    short     have_sfnt, have_lwfn;
    Str255    lwfn_file_name;
    UInt8     buff[PATH_MAX];
    FT_Error  err;
    short     अङ्कः_faces;


    have_sfnt = have_lwfn = 0;

    parse_fond( *fond, &have_sfnt, &sfnt_id, lwfn_file_name, 0 );

    जइ ( lwfn_file_name[0] )
    {
      err = lookup_lwfn_by_fond( pathname, lwfn_file_name,
                                 buff, परिमाण ( buff ) );
      जइ ( !err )
        have_lwfn = 1;
    }

    जइ ( have_lwfn && ( !have_sfnt || PREFER_LWFN ) )
      अङ्कः_faces = 1;
    अण्णहा
      अङ्कः_faces = गणनम्_faces_scalable( *fond );

    प्रत्यागम अङ्कः_faces;
  }


  /* Read Type 1 data from the POST resources inside the LWFN file,
     return a PFB buffer.  This is somewhat convoluted because the FT2
     PFB parser wants the ASCII header as one chunk, and the LWFN
     chunks are often not organized that way, so we glue chunks
     of the same type together. */
  स्थिर FT_Error
  read_lwfn( FT_Memory      memory,
             Res_File_Ref_अङ्कः  res,
             FT_Byte**      pfb_data,
             FT_ULong*      size )
  {
    FT_Error       error = FT_Err_Ok;
    ResID          res_id;
    unsigned वर्ण  *buffer, *p, *size_p = रिक्त;
    FT_ULong       total_size = 0;
    FT_ULong       old_total_size = 0;
    FT_ULong       post_size, pfb_chunk_size;
    Handle         post_data;
    वर्ण           code, last_code;


    UseResFile( res );

    /* First pass: load all POST resources, and determine the size of */
    /* the output buffer.                                             */
    res_id    = 501;
    last_code = -1;

    प्रति (;;)
    {
      post_data = Get1Resource( TTAG_POST, res_id++ );
      जइ ( !post_data )
        भङ्ग;  /* we are done */

      code = (*post_data)[0];

      जइ ( code != last_code )
      {
        जइ ( code == 5 )
          total_size += 2; /* just the end code */
        अण्णहा
          total_size += 6; /* code + 4 bytes chunk length */
      }

      total_size += (FT_ULong)GetHandleSize( post_data ) - 2;
      last_code = code;

      /* detect resource fork overflow */
      जइ ( FT_MAC_RFORK_MAX_LEN < total_size )
      {
        error = FT_THROW( Array_Too_Large );
        गच्छ Error;
      }

      old_total_size = total_size;
    }

    जइ ( FT_QALLOC( buffer, (FT_Long)total_size ) )
      गच्छ Error;

    /* Second pass: append all POST data to the buffer, add PFB fields. */
    /* Glue all consecutive chunks of the same type together.           */
    p              = buffer;
    res_id         = 501;
    last_code      = -1;
    pfb_chunk_size = 0;

    प्रति (;;)
    {
      post_data = Get1Resource( TTAG_POST, res_id++ );
      जइ ( !post_data )
        भङ्ग;  /* we are done */

      post_size = (FT_ULong)GetHandleSize( post_data ) - 2;
      code = (*post_data)[0];

      जइ ( code != last_code )
      {
        जइ ( last_code != -1 )
        {
          /* we are done adding a chunk, fill in the size field */
          जइ ( size_p )
          {
            *size_p++ = (FT_Byte)(   pfb_chunk_size         & 0xFF );
            *size_p++ = (FT_Byte)( ( pfb_chunk_size >> 8  ) & 0xFF );
            *size_p++ = (FT_Byte)( ( pfb_chunk_size >> 16 ) & 0xFF );
            *size_p++ = (FT_Byte)( ( pfb_chunk_size >> 24 ) & 0xFF );
          }
          pfb_chunk_size = 0;
        }

        *p++ = 0x80;
        जइ ( code == 5 )
          *p++ = 0x03;  /* the end */
        अण्णहा जइ ( code == 2 )
          *p++ = 0x02;  /* binary segment */
        अण्णहा
          *p++ = 0x01;  /* ASCII segment */

        जइ ( code != 5 )
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
    प्रत्यागम error;
  }


  /* Create a new FT_Face from a file path to an LWFN file. */
  स्थिर FT_Error
  FT_New_Face_From_LWFN( FT_Library    library,
                         ध्रुव UInt8*  pathname,
                         FT_Long       face_index,
                         FT_Face*      aface )
  {
    FT_Byte*       pfb_data;
    FT_ULong       pfb_size;
    FT_Error       error;
    Res_File_Ref_अङ्कः  res;


    जइ ( noErr != FT_FSPathMakeRes( pathname, &res ) )
      प्रत्यागम FT_THROW( Cannot_Open_Resource );

    pfb_data = रिक्त;
    pfb_size = 0;
    error = read_lwfn( library->memory, res, &pfb_data, &pfb_size );
    CloseResFile( res ); /* PFB is already loaded, useless anymore */
    जइ ( error )
      प्रत्यागम error;

    प्रत्यागम open_face_from_buffer( library,
                                  pfb_data,
                                  pfb_size,
                                  face_index,
                                  "type1",
                                  aface );
  }


  /* Create a new FT_Face from an SFNT resource, specified by res ID. */
  स्थिर FT_Error
  FT_New_Face_From_SFNT( FT_Library  library,
                         ResID       sfnt_id,
                         FT_Long     face_index,
                         FT_Face*    aface )
  {
    Handle     sfnt = रिक्त;
    FT_Byte*   sfnt_data;
    size_t     sfnt_size;
    FT_Error   error  = FT_Err_Ok;
    FT_Memory  memory = library->memory;
    पूर्णाङ्क        is_cff, is_sfnt_ps;


    sfnt = GetResource( TTAG_sfnt, sfnt_id );
    जइ ( !sfnt )
      प्रत्यागम FT_THROW( Invalid_Handle );

    sfnt_size = (FT_ULong)GetHandleSize( sfnt );

    /* detect resource fork overflow */
    जइ ( FT_MAC_RFORK_MAX_LEN < sfnt_size )
      प्रत्यागम FT_THROW( Array_Too_Large );

    जइ ( FT_QALLOC( sfnt_data, (FT_Long)sfnt_size ) )
    {
      ReleaseResource( sfnt );
      प्रत्यागम error;
    }

    ft_memcpy( sfnt_data, *sfnt, sfnt_size );
    ReleaseResource( sfnt );

    is_cff     = sfnt_size > 4 && !ft_memcmp( sfnt_data, "OTTO", 4 );
    is_sfnt_ps = sfnt_size > 4 && !ft_memcmp( sfnt_data, "typ1", 4 );

    जइ ( is_sfnt_ps )
    {
      FT_Stream  stream;


      जइ ( FT_NEW( stream ) )
        गच्छ Try_OpenType;

      FT_Stream_OpenMemory( stream, sfnt_data, sfnt_size );
      जइ ( !open_face_PS_from_sfnt_stream( library,
                                           stream,
                                           face_index,
                                           0, रिक्त,
                                           aface ) )
      {
        FT_Stream_Close( stream );
        FT_FREE( stream );
        FT_FREE( sfnt_data );
        गच्छ Exit;
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
    प्रत्यागम error;
  }


  /* Create a new FT_Face from a file path to a suitcase file. */
  स्थिर FT_Error
  FT_New_Face_From_Suitcase( FT_Library    library,
                             ध्रुव UInt8*  pathname,
                             FT_Long       face_index,
                             FT_Face*      aface )
  {
    FT_Error       error = FT_ERR( Cannot_Open_Resource );
    Res_File_Ref_अङ्कः  res_ref;
    ResourceIndex  res_index;
    Handle         fond;
    short          अङ्कः_faces_in_res;
    FT_Long        गणनम्;


    जइ ( noErr != FT_FSPathMakeRes( pathname, &res_ref ) )
      प्रत्यागम FT_THROW( Cannot_Open_Resource );

    UseResFile( res_ref );
    जइ ( ResError() )
      प्रत्यागम FT_THROW( Cannot_Open_Resource );

    res_index        = 1;
    अङ्कः_faces_in_res = 0;
    गणनम्            = face_index;
    जाव ( गणनम् >= 0 )
    {
      short  अङ्कः_faces_in_fond;


      fond = Get1IndResource( TTAG_FOND, res_index );
      जइ ( ResError() )
        भङ्ग;

      अङ्कः_faces_in_fond  = गणनम्_faces( fond, pathname );
      अङ्कः_faces_in_res  += अङ्कः_faces_in_fond;

      जइ ( गणनम् < अङ्कः_faces_in_fond )
        error = FT_New_Face_From_FOND( library, fond, गणनम्, aface );

      res_index++;
      गणनम् -= अङ्कः_faces_in_fond;
    }

    CloseResFile( res_ref );

    जइ ( !error && aface && *aface )
    {
      (*aface)->अङ्कः_faces  = अङ्कः_faces_in_res;
      (*aface)->face_index = face_index;
    }

    प्रत्यागम error;
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
    जइ ( ResError() != noErr || fond_type != TTAG_FOND )
      प्रत्यागम FT_THROW( Invalid_File_Format );

    parse_fond( *fond, &have_sfnt, &sfnt_id, lwfn_file_name, face_index );

    जइ ( lwfn_file_name[0] )
    {
      Res_File_Ref_अङ्कः  res;


      res = HomeResFile( fond );
      जइ ( noErr != ResError() )
        गच्छ found_no_lwfn_file;

      {
        UInt8  path_fond[PATH_MAX];
        FSRef  ref;


        err = FSGetForkCBInfo( res, k_FSInvalid_Volume_Ref_अङ्कः,
                               रिक्त, रिक्त, रिक्त, &ref, रिक्त );
        जइ ( noErr != err )
          गच्छ found_no_lwfn_file;

        err = FSRefMakePath( &ref, path_fond, परिमाण ( path_fond ) );
        जइ ( noErr != err )
          गच्छ found_no_lwfn_file;

        error = lookup_lwfn_by_fond( path_fond, lwfn_file_name,
                                     path_lwfn, परिमाण ( path_lwfn ) );
        जइ ( !error )
          have_lwfn = 1;
      }
    }

    जइ ( have_lwfn && ( !have_sfnt || PREFER_LWFN ) )
      error = FT_New_Face_From_LWFN( library,
                                     path_lwfn,
                                     face_index,
                                     aface );
    अण्णहा
      error = FT_THROW( Unknown_File_Format );

  found_no_lwfn_file:
    जइ ( have_sfnt && error )
      error = FT_New_Face_From_SFNT( library,
                                     sfnt_id,
                                     face_index,
                                     aface );

    प्रत्यागम error;
  }


  /* Common function to load a new FT_Face from a resource file. */
  स्थिर FT_Error
  FT_New_Face_From_Resource( FT_Library    library,
                             ध्रुव UInt8*  pathname,
                             FT_Long       face_index,
                             FT_Face*      aface )
  {
    OSType    file_type;
    FT_Error  error;


    /* LWFN is a (very) specific file format, check for it explicitly */
    file_type = get_file_type_from_path( pathname );
    जइ ( file_type == TTAG_LWFN )
      प्रत्यागम FT_New_Face_From_LWFN( library, pathname, face_index, aface );

    /* Otherwise the file type doesn't matter (there are more than  */
    /* `FFIL' and `tfil').  Just try opening it as a font suitcase; */
    /* if it works, fine.                                           */

    error = FT_New_Face_From_Suitcase( library, pathname, face_index, aface );
    जइ ( error )
    {
      /* let it fall through to normal loader (.ttf, .otf, etc.); */
      /* we signal this by returning no error and no FT_Face      */
      *aface = रिक्त;
    }

    प्रत्यागम FT_Err_Ok;
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
               ध्रुव वर्ण*  pathname,
               FT_Long      face_index,
               FT_Face*     aface )
  {
    FT_Open_Args  args;
    FT_Error      error;


    /* test for valid `library' and `aface' delayed to FT_Open_Face() */
    जइ ( !pathname )
      प्रत्यागम FT_THROW( Invalid_Argument );

    *aface = रिक्त;

    /* try resourcefork based font: LWFN, FFIL */
    error = FT_New_Face_From_Resource( library, (UInt8 *)pathname,
                                       face_index, aface );
    जइ ( error || *aface )
      प्रत्यागम error;

    /* let it fall through to normal loader (.ttf, .otf, etc.) */
    args.flags    = FT_OPEN_PATHNAME;
    args.pathname = (वर्ण*)pathname;

    प्रत्यागम FT_Open_Face( library, &args, face_index, aface );
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
                          ध्रुव FSRef*  ref,
                          FT_Long       face_index,
                          FT_Face*      aface )
  {
    FT_Error      error;
    FT_Open_Args  args;

    OSErr  err;
    UInt8  pathname[PATH_MAX];


    /* check of `library' and `aface' delayed to */
    /* `FT_New_Face_From_Resource'               */

    जइ ( !ref )
      प्रत्यागम FT_THROW( Invalid_Argument );

    err = FSRefMakePath( ref, pathname, परिमाण ( pathname ) );
    जइ ( err )
      error = FT_THROW( Cannot_Open_Resource );

    error = FT_New_Face_From_Resource( library, pathname, face_index, aface );
    जइ ( error || *aface )
      प्रत्यागम error;

    /* fallback to datafork font */
    args.flags    = FT_OPEN_PATHNAME;
    args.pathname = (वर्ण*)pathname;
    प्रत्यागम FT_Open_Face( library, &args, face_index, aface );
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
                           ध्रुव FSSpec*  spec,
                           FT_Long        face_index,
                           FT_Face*       aface )
  {
#जइ ( __LP64__ ) || ( defined( MAC_OS_X_VERSION_10_5 ) && \
      ( MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_5 ) )
    FT_UNUSED( library );
    FT_UNUSED( spec );
    FT_UNUSED( face_index );
    FT_UNUSED( aface );

    प्रत्यागम FT_THROW( Unimplemented_Feature );
#अण्णहा
    FSRef  ref;


    /* check of `library' and `aface' delayed to `FT_New_Face_From_FSRef' */

    जइ ( !spec || FSpMakeFSRef( spec, &ref ) != noErr )
      प्रत्यागम FT_THROW( Invalid_Argument );
    अण्णहा
      प्रत्यागम FT_New_Face_From_FSRef( library, &ref, face_index, aface );
#endif
  }

#अण्णहा /* !FT_MACINTOSH */

  /* ANSI C doesn't like empty source files */
  प्रकारनिर्देश पूर्णाङ्क  ft_mac_dummy_;

#endif /* !FT_MACINTOSH */


/* END */
