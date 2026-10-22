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


#অন্তর্ভাব <freetype/freetype.h>
#অন্তর্ভাব <freetype/tttags.h>
#অন্তর্ভাব <freetype/internal/ftdebug.h>
#অন্তর্ভাব <freetype/internal/ftstream.h>
#অন্তর্ভাব "ftbase.h"


#ifdef FT_MACINTOSH

  /* This is for Mac OS X.  Without redefinition, OS_INLINE */
  /* expands to `static inline' which doesn't survive the   */
  /* -ansi compilation flag of GCC.                         */
#যদি !HAVE_ANSI_OS_INLINE
#undef  OS_INLINE
#define OS_INLINE  স্থির __inline__
#endif

  /* `configure' checks the availability of `ResourceIndex' strictly */
  /* and sets HAVE_TYPE_RESOURCE_INDEX 1 or 0 always.  If it is      */
  /* not set (e.g., a build without `configure'), the availability   */
  /* is guessed from the SDK version.                                */
#ifndef HAVE_TYPE_RESOURCE_সূচক
#যদি !defined( MAC_OS_X_VERSION_10_5 ) || \
    ( MAC_OS_X_VERSION_MAX_ALLOWED < MAC_OS_X_VERSION_10_5 )
#define HAVE_TYPE_RESOURCE_সূচক 0
#অন্যথা
#define HAVE_TYPE_RESOURCE_সূচক 1
#endif
#endif /* !HAVE_TYPE_RESOURCE_INDEX */

#যদি ( HAVE_TYPE_RESOURCE_সূচক == 0 )
  প্রকারলেখ short  Resource_সূচক;
#endif

#অন্তর্ভাব <CoreServices/CoreServices.h>
#অন্তর্ভাব <ApplicationServices/ApplicationServices.h>
#অন্তর্ভাব <sys/syslimits.h> /* PATH_MAX */

  /* Don't want warnings about our own use of deprecated functions. */
#define FT_DEPRECATED_ATTRIBUTE

#অন্তর্ভাব <freetype/ftmac.h>

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
  FT_GetFile_From_Mac_Name( ধ্রুবক অক্ষর*  fontName,
                            FSSpec*      pathSpec,
                            FT_Long*     face_সূচক )
  {
    FT_UNUSED( fontName );
    FT_UNUSED( pathSpec );
    FT_UNUSED( face_সূচক );

    ফেরত FT_THROW( Unimplemented_Feature );
  }


  /* Private function.                                         */
  /* The FSSpec type has been discouraged for a long time,     */
  /* unfortunately an FSRef replacement API for                */
  /* ATSFontGetFileSpecification() is only available in        */
  /* Mac OS X 10.5 and later.                                  */
  স্থির OSStatus
  FT_ATSFontGetFileReference( ATSFontRef  ats_font_id,
                              FSRef*      ats_font_ref )
  {
#যদি defined( MAC_OS_X_VERSION_10_5 ) && \
    ( MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_5 )

    OSStatus  err;

    err = ATSFontGetFileReference( ats_font_id, ats_font_ref );

    ফেরত err;
#নতুবা_যদি __LP64__ /* No 64bit Carbon API on legacy platforms */
    FT_UNUSED( ats_font_id );
    FT_UNUSED( ats_font_ref );


    ফেরত fnfErr;
#অন্যথা /* 32bit Carbon API on legacy platforms */
    OSStatus  err;
    FSSpec    spec;


    err = ATSFontGetFileSpecification( ats_font_id, &spec );
    যদি ( noErr == err )
      err = FSpMakeFSRef( &spec, ats_font_ref );

    ফেরত err;
#endif
  }


  স্থির FT_Error
  FT_GetFileRef_From_Mac_ATS_Name( ধ্রুবক অক্ষর*  fontName,
                                   FSRef*       ats_font_ref,
                                   FT_Long*     face_সূচক )
  {
    CFStringRef  cf_fontName;
    ATSFontRef   ats_font_id;


    *face_সূচক = 0;

    cf_fontName = CFStringCreateWithCString( ফাঁকা, fontName,
                                             kCFStringEncodingMacRoman );
    ats_font_id = ATSFontFindFromName( cf_fontName,
                                       kATSOptionFlagsUnRestrictedScope );
    CFRelease( cf_fontName );

    যদি ( ats_font_id == 0 || ats_font_id == 0xFFFFFFFFUL )
      ফেরত FT_THROW( Unknown_File_Format );

    যদি ( noErr != FT_ATSFontGetFileReference( ats_font_id, ats_font_ref ) )
      ফেরত FT_THROW( Unknown_File_Format );

    /* face_index calculation by searching preceding fontIDs */
    /* with same FSRef                                       */
    {
      ATSFontRef  id2 = ats_font_id - 1;
      FSRef       ref2;


      যখন ( id2 > 0 )
      {
        যদি ( noErr != FT_ATSFontGetFileReference( id2, &ref2 ) )
          বিরতি;
        যদি ( noErr != FSCompareFSRefs( ats_font_ref, &ref2 ) )
          বিরতি;

        id2 --;
      }
      *face_সূচক = ats_font_id - ( id2 + 1 );
    }

    ফেরত FT_Err_Ok;
  }


  FT_EXPORT_DEF( FT_Error )
  FT_GetFilePath_From_Mac_ATS_Name( ধ্রুবক অক্ষর*  fontName,
                                    UInt8*       path,
                                    UInt32       max_Path_আকার,
                                    FT_Long*     face_সূচক )
  {
    FSRef     ref;
    FT_Error  err;


    যদি ( !fontName || !face_সূচক )
      ফেরত FT_THROW( Invalid_Argument);

    err = FT_GetFileRef_From_Mac_ATS_Name( fontName, &ref, face_সূচক );
    যদি ( err )
      ফেরত err;

    যদি ( noErr != FSRefMakePath( &ref, path, max_Path_আকার ) )
      ফেরত FT_THROW( Unknown_File_Format );

    ফেরত FT_Err_Ok;
  }


  /* This function is deprecated because FSSpec is deprecated in Mac OS X  */
  FT_EXPORT_DEF( FT_Error )
  FT_GetFile_From_Mac_ATS_Name( ধ্রুবক অক্ষর*  fontName,
                                FSSpec*      pathSpec,
                                FT_Long*     face_সূচক )
  {
#যদি ( __LP64__ ) || ( defined( MAC_OS_X_VERSION_10_5 ) && \
      ( MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_5 ) )
    FT_UNUSED( fontName );
    FT_UNUSED( pathSpec );
    FT_UNUSED( face_সূচক );

    ফেরত FT_THROW( Unimplemented_Feature );
#অন্যথা
    FSRef     ref;
    FT_Error  err;


    যদি ( !fontName || !face_সূচক )
      ফেরত FT_THROW( Invalid_Argument );

    err = FT_GetFileRef_From_Mac_ATS_Name( fontName, &ref, face_সূচক );
    যদি ( err )
      ফেরত err;

    যদি ( noErr != FSGetCatalogInfo( &ref, kFSCatInfoNone, ফাঁকা, ফাঁকা,
                                    pathSpec, ফাঁকা ) )
      ফেরত FT_THROW( Unknown_File_Format );

    ফেরত FT_Err_Ok;
#endif
  }


  স্থির OSErr
  FT_FSPathMakeRes( ধ্রুবক UInt8*    pathname,
                    Res_File_Ref_অঙ্ক*  res )
  {
    OSErr  err;
    FSRef  ref;


    যদি ( noErr != FSPathMakeRef( pathname, &ref, FALSE ) )
      ফেরত FT_THROW( Cannot_Open_Resource );

    /* at present, no support for dfont format */
    err = FSOpenResourceFile( &ref, 0, ফাঁকা, fsRdPerm, res );
    যদি ( noErr == err )
      ফেরত err;

    /* fallback to original resource-fork font */
    *res = FSOpenResFile( &ref, fsRdPerm );
    err  = ResError();

    ফেরত err;
  }


  /* Return the file type for given pathname */
  স্থির OSType
  get_file_type_from_path( ধ্রুবক UInt8*  pathname )
  {
    FSRef          ref;
    FSCatalogInfo  info;


    যদি ( noErr != FSPathMakeRef( pathname, &ref, FALSE ) )
      ফেরত ( OSType ) 0;

    যদি ( noErr != FSGetCatalogInfo( &ref, kFSCatInfoFinderInfo, &info,
                                    ফাঁকা, ফাঁকা, ফাঁকা ) )
      ফেরত ( OSType ) 0;

    ফেরত ( (FInfo *)( info.finderInfo ) )->fdType;
  }


  /* Given a PostScript font name, create the Macintosh LWFN file name. */
  স্থির শূন্য
  create_lwfn_name( অক্ষর*   ps_name,
                    Str255  lwfn_file_name )
  {
    পূর্ণসংখ্যা       max = 5, গুনতি = 0;
    FT_Byte*  p = lwfn_file_name;
    FT_Byte*  q = (FT_Byte*)ps_name;


    lwfn_file_name[0] = 0;

    যখন ( *q )
    {
      যদি ( ft_isupper( *q ) )
      {
        যদি ( গুনতি )
          max = 3;
        গুনতি = 0;
      }
      যদি ( গুনতি < max && ( ft_isalnum( *q ) || *q == '_' ) )
      {
        *++p = *q;
        lwfn_file_name[0]++;
        গুনতি++;
      }
      q++;
    }
  }


  স্থির short
  গুনতি_faces_sfnt( অক্ষর*  fond_তথ্য )
  {
    /* The count is 1 greater than the value in the FOND.  */
    /* Isn't that cute? :-)                                */

    ফেরত EndianS16_BtoN( *( (short*)( fond_তথ্য +
                                        আকার ( FamRec ) ) ) ) + 1;
  }


  স্থির short
  গুনতি_faces_scalable( অক্ষর*  fond_তথ্য )
  {
    AsscEntry*  assoc;
    short       i, face, face_all;


    face_all = EndianS16_BtoN( *( (short *)( fond_তথ্য +
                                             আকার ( FamRec ) ) ) ) + 1;
    assoc    = (AsscEntry*)( fond_তথ্য + আকার ( FamRec ) + 2 );
    face     = 0;

    জন্য ( i = 0; i < face_all; i++ )
    {
      যদি ( 0 == EndianS16_BtoN( assoc[i].font_আকার ) )
        face++;
    }
    ফেরত face;
  }


  /* Look inside the FOND data, answer whether there should be an SFNT
     resource, and answer the name of a possible LWFN Type 1 file.

     Thanks to Paul Miller (paulm@profoundeffects.com) for the fix
     to load a face OTHER than the first one in the FOND!
  */


  স্থির শূন্য
  parse_fond( অক্ষর*   fond_তথ্য,
              short*  have_sfnt,
              ResID*  sfnt_id,
              Str255  lwfn_file_name,
              short   face_সূচক )
  {
    AsscEntry*  assoc;
    AsscEntry*  base_assoc;
    FamRec*     fond;


    *sfnt_id          = 0;
    *have_sfnt        = 0;
    lwfn_file_name[0] = 0;

    fond       = (FamRec*)fond_তথ্য;
    assoc      = (AsscEntry*)( fond_তথ্য + আকার ( FamRec ) + 2 );
    base_assoc = assoc;

    /* the maximum faces in a FOND is 48, size of StyleTable.indexes[] */
    যদি ( 47 < face_সূচক )
      ফেরত;

    /* Let's do a little range checking before we get too excited here */
    যদি ( face_সূচক < গুনতি_faces_sfnt( fond_তথ্য ) )
    {
      assoc += face_সূচক;        /* add on the face_index! */

      /* if the face at this index is not scalable,
         fall back to the first one (old behavior) */
      যদি ( EndianS16_BtoN( assoc->font_আকার ) == 0 )
      {
        *have_sfnt = 1;
        *sfnt_id   = EndianS16_BtoN( assoc->fontID );
      }
      অন্যথা যদি ( base_assoc->font_আকার == 0 )
      {
        *have_sfnt = 1;
        *sfnt_id   = EndianS16_BtoN( base_assoc->fontID );
      }
    }

    যদি ( EndianS32_BtoN( fond->ffStylOff ) )
    {
      unsigned অক্ষর*  p = (unsigned অক্ষর*)fond_তথ্য;
      StyleTable*     style;
      unsigned short  string_গুনতি;
      অক্ষর            ps_name[256];
      unsigned অক্ষর*  names[64];
      পূর্ণসংখ্যা             i;


      p += EndianS32_BtoN( fond->ffStylOff );
      style = (StyleTable*)p;
      p += আকার ( StyleTable );
      string_গুনতি = EndianS16_BtoN( *(short*)(p) );
      string_গুনতি = FT_MIN( 64, string_গুনতি );
      p += আকার ( short );

      জন্য ( i = 0; i < string_গুনতি; i++ )
      {
        names[i] = p;
        p       += names[i][0];
        p++;
      }

      {
        size_t  ps_name_len = (size_t)names[0][0];


        যদি ( ps_name_len != 0 )
        {
          ft_memcpy( ps_name, names[0] + 1, ps_name_len );
          ps_name[ps_name_len] = 0;
        }
        যদি ( style->indexes[face_সূচক] > 1 &&
             style->indexes[face_সূচক] <= string_গুনতি )
        {
          unsigned অক্ষর*  suffixes = names[style->indexes[face_সূচক] - 1];


          জন্য ( i = 1; i <= suffixes[0]; i++ )
          {
            unsigned অক্ষর*  s;
            size_t          j = suffixes[i] - 1;


            যদি ( j < string_গুনতি && ( s = names[j] ) != ফাঁকা )
            {
              size_t  s_len = (size_t)s[0];


              যদি ( s_len != 0 && ps_name_len + s_len < আকার ( ps_name ) )
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


  স্থির  FT_Error
  lookup_lwfn_by_fond( ধ্রুবক UInt8*      path_fond,
                       ConstStr255Param  base_lwfn,
                       UInt8*            path_lwfn,
                       size_t            path_আকার )
  {
    FSRef   ref, par_ref;
    size_t  dirname_len;


    /* Pathname for FSRef can be in various formats: HFS, HFS+, and POSIX. */
    /* We should not extract parent directory by string manipulation.      */

    যদি ( noErr != FSPathMakeRef( path_fond, &ref, FALSE ) )
      ফেরত FT_THROW( Invalid_Argument );

    যদি ( noErr != FSGetCatalogInfo( &ref, kFSCatInfoNone,
                                    ফাঁকা, ফাঁকা, ফাঁকা, &par_ref ) )
      ফেরত FT_THROW( Invalid_Argument );

    যদি ( noErr != FSRefMakePath( &par_ref, path_lwfn, path_আকার ) )
      ফেরত FT_THROW( Invalid_Argument );

    যদি ( ft_strlen( (অক্ষর *)path_lwfn ) + 1 + base_lwfn[0] > path_আকার )
      ফেরত FT_THROW( Invalid_Argument );

    /* now we have absolute dirname in path_lwfn */
    ft_strcat( (অক্ষর *)path_lwfn, "/" );
    dirname_len = ft_strlen( (অক্ষর *)path_lwfn );
    ft_strcat( (অক্ষর *)path_lwfn, (অক্ষর *)base_lwfn + 1 );
    path_lwfn[dirname_len + base_lwfn[0]] = '\0';

    যদি ( noErr != FSPathMakeRef( path_lwfn, &ref, FALSE ) )
      ফেরত FT_THROW( Cannot_Open_Resource );

    যদি ( noErr != FSGetCatalogInfo( &ref, kFSCatInfoNone,
                                    ফাঁকা, ফাঁকা, ফাঁকা, ফাঁকা ) )
      ফেরত FT_THROW( Cannot_Open_Resource );

    ফেরত FT_Err_Ok;
  }


  স্থির short
  গুনতি_faces( Handle        fond,
               ধ্রুবক UInt8*  pathname )
  {
    ResID     sfnt_id;
    short     have_sfnt, have_lwfn;
    Str255    lwfn_file_name;
    UInt8     buff[PATH_MAX];
    FT_Error  err;
    short     অঙ্ক_faces;


    have_sfnt = have_lwfn = 0;

    parse_fond( *fond, &have_sfnt, &sfnt_id, lwfn_file_name, 0 );

    যদি ( lwfn_file_name[0] )
    {
      err = lookup_lwfn_by_fond( pathname, lwfn_file_name,
                                 buff, আকার ( buff ) );
      যদি ( !err )
        have_lwfn = 1;
    }

    যদি ( have_lwfn && ( !have_sfnt || PREFER_LWFN ) )
      অঙ্ক_faces = 1;
    অন্যথা
      অঙ্ক_faces = গুনতি_faces_scalable( *fond );

    ফেরত অঙ্ক_faces;
  }


  /* Read Type 1 data from the POST resources inside the LWFN file,
     return a PFB buffer.  This is somewhat convoluted because the FT2
     PFB parser wants the ASCII header as one chunk, and the LWFN
     chunks are often not organized that way, so we glue chunks
     of the same type together. */
  স্থির FT_Error
  read_lwfn( FT_Memory      memory,
             Res_File_Ref_অঙ্ক  res,
             FT_Byte**      pfb_তথ্য,
             FT_ULong*      আকার_2 )
  {
    FT_Error       error = FT_Err_Ok;
    ResID          res_id;
    unsigned অক্ষর  *বাফার, *p, *আকার_p = ফাঁকা;
    FT_ULong       যোগ_আকার = 0;
    FT_ULong       old_যোগ_আকার = 0;
    FT_ULong       post_আকার, pfb_chunk_আকার;
    Handle         post_তথ্য;
    অক্ষর           code, last_code;


    UseResFile( res );

    /* First pass: load all POST resources, and determine the size of */
    /* the output buffer.                                             */
    res_id    = 501;
    last_code = -1;

    জন্য (;;)
    {
      post_তথ্য = Get1Resource( TTAG_POST, res_id++ );
      যদি ( !post_তথ্য )
        বিরতি;  /* we are done */

      code = (*post_তথ্য)[0];

      যদি ( code != last_code )
      {
        যদি ( code == 5 )
          যোগ_আকার += 2; /* just the end code */
        অন্যথা
          যোগ_আকার += 6; /* code + 4 bytes chunk length */
      }

      যোগ_আকার += (FT_ULong)Get_Handle_আকার( post_তথ্য ) - 2;
      last_code = code;

      /* detect resource fork overflow */
      যদি ( FT_MAC_RFORK_MAX_LEN < যোগ_আকার )
      {
        error = FT_THROW( Array_Too_Large );
        যাও Error;
      }

      old_যোগ_আকার = যোগ_আকার;
    }

    যদি ( FT_QALLOC( বাফার, (FT_Long)যোগ_আকার ) )
      যাও Error;

    /* Second pass: append all POST data to the buffer, add PFB fields. */
    /* Glue all consecutive chunks of the same type together.           */
    p              = বাফার;
    res_id         = 501;
    last_code      = -1;
    pfb_chunk_আকার = 0;

    জন্য (;;)
    {
      post_তথ্য = Get1Resource( TTAG_POST, res_id++ );
      যদি ( !post_তথ্য )
        বিরতি;  /* we are done */

      post_আকার = (FT_ULong)Get_Handle_আকার( post_তথ্য ) - 2;
      code = (*post_তথ্য)[0];

      যদি ( code != last_code )
      {
        যদি ( last_code != -1 )
        {
          /* we are done adding a chunk, fill in the size field */
          যদি ( আকার_p )
          {
            *আকার_p++ = (FT_Byte)(   pfb_chunk_আকার         & 0xFF );
            *আকার_p++ = (FT_Byte)( ( pfb_chunk_আকার >> 8  ) & 0xFF );
            *আকার_p++ = (FT_Byte)( ( pfb_chunk_আকার >> 16 ) & 0xFF );
            *আকার_p++ = (FT_Byte)( ( pfb_chunk_আকার >> 24 ) & 0xFF );
          }
          pfb_chunk_আকার = 0;
        }

        *p++ = 0x80;
        যদি ( code == 5 )
          *p++ = 0x03;  /* the end */
        অন্যথা যদি ( code == 2 )
          *p++ = 0x02;  /* binary segment */
        অন্যথা
          *p++ = 0x01;  /* ASCII segment */

        যদি ( code != 5 )
        {
          আকার_p = p;   /* save for later */
          p += 4;       /* make space for size field */
        }
      }

      ft_memcpy( p, *post_তথ্য + 2, post_আকার );
      pfb_chunk_আকার += post_আকার;
      p += post_আকার;
      last_code = code;
    }

    *pfb_তথ্য = বাফার;
    *আকার_2 = যোগ_আকার;

  Error:
    CloseResFile( res );
    ফেরত error;
  }


  /* Create a new FT_Face from a file path to an LWFN file. */
  স্থির FT_Error
  FT_New_Face_From_LWFN( FT_Library    library,
                         ধ্রুবক UInt8*  pathname,
                         FT_Long       face_সূচক,
                         FT_Face*      aface )
  {
    FT_Byte*       pfb_তথ্য;
    FT_ULong       pfb_আকার;
    FT_Error       error;
    Res_File_Ref_অঙ্ক  res;


    যদি ( noErr != FT_FSPathMakeRes( pathname, &res ) )
      ফেরত FT_THROW( Cannot_Open_Resource );

    pfb_তথ্য = ফাঁকা;
    pfb_আকার = 0;
    error = read_lwfn( library->memory, res, &pfb_তথ্য, &pfb_আকার );
    CloseResFile( res ); /* PFB is already loaded, useless anymore */
    যদি ( error )
      ফেরত error;

    ফেরত open_face_from_বাফার( library,
                                  pfb_তথ্য,
                                  pfb_আকার,
                                  face_সূচক,
                                  "type1",
                                  aface );
  }


  /* Create a new FT_Face from an SFNT resource, specified by res ID. */
  স্থির FT_Error
  FT_New_Face_From_SFNT( FT_Library  library,
                         ResID       sfnt_id,
                         FT_Long     face_সূচক,
                         FT_Face*    aface )
  {
    Handle     sfnt = ফাঁকা;
    FT_Byte*   sfnt_তথ্য;
    size_t     sfnt_আকার;
    FT_Error   error  = FT_Err_Ok;
    FT_Memory  memory = library->memory;
    পূর্ণসংখ্যা        is_cff, is_sfnt_ps;


    sfnt = GetResource( TTAG_sfnt, sfnt_id );
    যদি ( !sfnt )
      ফেরত FT_THROW( Invalid_Handle );

    sfnt_আকার = (FT_ULong)Get_Handle_আকার( sfnt );

    /* detect resource fork overflow */
    যদি ( FT_MAC_RFORK_MAX_LEN < sfnt_আকার )
      ফেরত FT_THROW( Array_Too_Large );

    যদি ( FT_QALLOC( sfnt_তথ্য, (FT_Long)sfnt_আকার ) )
    {
      ReleaseResource( sfnt );
      ফেরত error;
    }

    ft_memcpy( sfnt_তথ্য, *sfnt, sfnt_আকার );
    ReleaseResource( sfnt );

    is_cff     = sfnt_আকার > 4 && !ft_memcmp( sfnt_তথ্য, "OTTO", 4 );
    is_sfnt_ps = sfnt_আকার > 4 && !ft_memcmp( sfnt_তথ্য, "typ1", 4 );

    যদি ( is_sfnt_ps )
    {
      FT_Stream  stream;


      যদি ( FT_NEW( stream ) )
        যাও Try_OpenType;

      FT_Stream_OpenMemory( stream, sfnt_তথ্য, sfnt_আকার );
      যদি ( !open_face_PS_from_sfnt_stream( library,
                                           stream,
                                           face_সূচক,
                                           0, ফাঁকা,
                                           aface ) )
      {
        FT_Stream_Close( stream );
        FT_FREE( stream );
        FT_FREE( sfnt_তথ্য );
        যাও Exit;
      }

      FT_FREE( stream );
    }
  Try_OpenType:
    error = open_face_from_বাফার( library,
                                   sfnt_তথ্য,
                                   sfnt_আকার,
                                   face_সূচক,
                                   is_cff ? "cff" : "truetype",
                                   aface );
  Exit:
    ফেরত error;
  }


  /* Create a new FT_Face from a file path to a suitcase file. */
  স্থির FT_Error
  FT_New_Face_From_Suitcase( FT_Library    library,
                             ধ্রুবক UInt8*  pathname,
                             FT_Long       face_সূচক,
                             FT_Face*      aface )
  {
    FT_Error       error = FT_ERR( Cannot_Open_Resource );
    Res_File_Ref_অঙ্ক  res_ref;
    Resource_সূচক  res_সূচক;
    Handle         fond;
    short          অঙ্ক_faces_in_res;
    FT_Long        গুনতি;


    যদি ( noErr != FT_FSPathMakeRes( pathname, &res_ref ) )
      ফেরত FT_THROW( Cannot_Open_Resource );

    UseResFile( res_ref );
    যদি ( ResError() )
      ফেরত FT_THROW( Cannot_Open_Resource );

    res_সূচক        = 1;
    অঙ্ক_faces_in_res = 0;
    গুনতি            = face_সূচক;
    যখন ( গুনতি >= 0 )
    {
      short  অঙ্ক_faces_in_fond;


      fond = Get1IndResource( TTAG_FOND, res_সূচক );
      যদি ( ResError() )
        বিরতি;

      অঙ্ক_faces_in_fond  = গুনতি_faces( fond, pathname );
      অঙ্ক_faces_in_res  += অঙ্ক_faces_in_fond;

      যদি ( গুনতি < অঙ্ক_faces_in_fond )
        error = FT_New_Face_From_FOND( library, fond, গুনতি, aface );

      res_সূচক++;
      গুনতি -= অঙ্ক_faces_in_fond;
    }

    CloseResFile( res_ref );

    যদি ( !error && aface && *aface )
    {
      (*aface)->অঙ্ক_faces  = অঙ্ক_faces_in_res;
      (*aface)->face_সূচক = face_সূচক;
    }

    ফেরত error;
  }


  /* documentation is in ftmac.h */

  FT_EXPORT_DEF( FT_Error )
  FT_New_Face_From_FOND( FT_Library  library,
                         Handle      fond,
                         FT_Long     face_সূচক,
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
    যদি ( ResError() != noErr || fond_type != TTAG_FOND )
      ফেরত FT_THROW( Invalid_File_Format );

    parse_fond( *fond, &have_sfnt, &sfnt_id, lwfn_file_name, face_সূচক );

    যদি ( lwfn_file_name[0] )
    {
      Res_File_Ref_অঙ্ক  res;


      res = HomeResFile( fond );
      যদি ( noErr != ResError() )
        যাও found_no_lwfn_file;

      {
        UInt8  path_fond[PATH_MAX];
        FSRef  ref;


        err = FSGetForkCBInfo( res, k_FSInvalid_Volume_Ref_অঙ্ক,
                               ফাঁকা, ফাঁকা, ফাঁকা, &ref, ফাঁকা );
        যদি ( noErr != err )
          যাও found_no_lwfn_file;

        err = FSRefMakePath( &ref, path_fond, আকার ( path_fond ) );
        যদি ( noErr != err )
          যাও found_no_lwfn_file;

        error = lookup_lwfn_by_fond( path_fond, lwfn_file_name,
                                     path_lwfn, আকার ( path_lwfn ) );
        যদি ( !error )
          have_lwfn = 1;
      }
    }

    যদি ( have_lwfn && ( !have_sfnt || PREFER_LWFN ) )
      error = FT_New_Face_From_LWFN( library,
                                     path_lwfn,
                                     face_সূচক,
                                     aface );
    অন্যথা
      error = FT_THROW( Unknown_File_Format );

  found_no_lwfn_file:
    যদি ( have_sfnt && error )
      error = FT_New_Face_From_SFNT( library,
                                     sfnt_id,
                                     face_সূচক,
                                     aface );

    ফেরত error;
  }


  /* Common function to load a new FT_Face from a resource file. */
  স্থির FT_Error
  FT_New_Face_From_Resource( FT_Library    library,
                             ধ্রুবক UInt8*  pathname,
                             FT_Long       face_সূচক,
                             FT_Face*      aface )
  {
    OSType    file_type;
    FT_Error  error;


    /* LWFN is a (very) specific file format, check for it explicitly */
    file_type = get_file_type_from_path( pathname );
    যদি ( file_type == TTAG_LWFN )
      ফেরত FT_New_Face_From_LWFN( library, pathname, face_সূচক, aface );

    /* Otherwise the file type doesn't matter (there are more than  */
    /* `FFIL' and `tfil').  Just try opening it as a font suitcase; */
    /* if it works, fine.                                           */

    error = FT_New_Face_From_Suitcase( library, pathname, face_সূচক, aface );
    যদি ( error )
    {
      /* let it fall through to normal loader (.ttf, .otf, etc.); */
      /* we signal this by returning no error and no FT_Face      */
      *aface = ফাঁকা;
    }

    ফেরত FT_Err_Ok;
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
               ধ্রুবক অক্ষর*  pathname,
               FT_Long      face_সূচক,
               FT_Face*     aface )
  {
    FT_Open_Args  args;
    FT_Error      error;


    /* test for valid `library' and `aface' delayed to FT_Open_Face() */
    যদি ( !pathname )
      ফেরত FT_THROW( Invalid_Argument );

    *aface = ফাঁকা;

    /* try resourcefork based font: LWFN, FFIL */
    error = FT_New_Face_From_Resource( library, (UInt8 *)pathname,
                                       face_সূচক, aface );
    যদি ( error || *aface )
      ফেরত error;

    /* let it fall through to normal loader (.ttf, .otf, etc.) */
    args.flags    = FT_OPEN_PATHNAME;
    args.pathname = (অক্ষর*)pathname;

    ফেরত FT_Open_Face( library, &args, face_সূচক, aface );
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
                          ধ্রুবক FSRef*  ref,
                          FT_Long       face_সূচক,
                          FT_Face*      aface )
  {
    FT_Error      error;
    FT_Open_Args  args;

    OSErr  err;
    UInt8  pathname[PATH_MAX];


    /* check of `library' and `aface' delayed to */
    /* `FT_New_Face_From_Resource'               */

    যদি ( !ref )
      ফেরত FT_THROW( Invalid_Argument );

    err = FSRefMakePath( ref, pathname, আকার ( pathname ) );
    যদি ( err )
      error = FT_THROW( Cannot_Open_Resource );

    error = FT_New_Face_From_Resource( library, pathname, face_সূচক, aface );
    যদি ( error || *aface )
      ফেরত error;

    /* fallback to datafork font */
    args.flags    = FT_OPEN_PATHNAME;
    args.pathname = (অক্ষর*)pathname;
    ফেরত FT_Open_Face( library, &args, face_সূচক, aface );
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
                           ধ্রুবক FSSpec*  spec,
                           FT_Long        face_সূচক,
                           FT_Face*       aface )
  {
#যদি ( __LP64__ ) || ( defined( MAC_OS_X_VERSION_10_5 ) && \
      ( MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_5 ) )
    FT_UNUSED( library );
    FT_UNUSED( spec );
    FT_UNUSED( face_সূচক );
    FT_UNUSED( aface );

    ফেরত FT_THROW( Unimplemented_Feature );
#অন্যথা
    FSRef  ref;


    /* check of `library' and `aface' delayed to `FT_New_Face_From_FSRef' */

    যদি ( !spec || FSpMakeFSRef( spec, &ref ) != noErr )
      ফেরত FT_THROW( Invalid_Argument );
    অন্যথা
      ফেরত FT_New_Face_From_FSRef( library, &ref, face_সূচক, aface );
#endif
  }

#অন্যথা /* !FT_MACINTOSH */

  /* ANSI C doesn't like empty source files */
  প্রকারলেখ পূর্ণসংখ্যা  ft_mac_dummy_;

#endif /* !FT_MACINTOSH */


/* END */
