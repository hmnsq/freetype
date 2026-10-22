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


#यदि_परिभाषितम् FT_MACINTOSH

  /* This is for Mac OS X.  Without redefinition, OS_INLINE */
  /* expands to `static inline' which doesn't survive the   */
  /* -ansi compilation flag of GCC.                         */
#यदि !HAVE_ANSI_OS_INLINE
#अपरिभाषय  OS_INLINE
#परिभाषय OS_INLINE  स्थिर __inline__
#यदि_अन्तः

  /* `configure' checks the availability of `ResourceIndex' strictly */
  /* and sets HAVE_TYPE_RESOURCE_INDEX 1 or 0 always.  If it is      */
  /* not set (e.g., a build without `configure'), the availability   */
  /* is guessed from the SDK version.                                */
#यदि_अपरिभाषितम् HAVE_TYPE_RESOURCE_सूचकांक
#यदि !defined( MAC_OS_X_संस्करणम्_10_5 ) || \
    ( MAC_OS_X_संस्करणम्_MAX_ALLOWED < MAC_OS_X_संस्करणम्_10_5 )
#परिभाषय HAVE_TYPE_RESOURCE_सूचकांक 0
#अन्यथा
#परिभाषय HAVE_TYPE_RESOURCE_सूचकांक 1
#यदि_अन्तः
#यदि_अन्तः /* !HAVE_TYPE_RESOURCE_INDEX */

#यदि ( HAVE_TYPE_RESOURCE_सूचकांक == 0 )
  प्रकारनिर्देश ह्रस्व  Resource_सूचकांक;
#यदि_अन्तः

#अन्तर्भाव <CoreServices/CoreServices.h>
#अन्तर्भाव <ApplicationServices/ApplicationServices.h>
#अन्तर्भाव <sys/syslimits.h> /* PATH_MAX */

  /* Don't want warnings about our own use of deprecated functions. */
#परिभाषय FT_DEPRECATED_ATTRIBUTE

#अन्तर्भाव <freetype/ftmac.h>

#यदि_अपरिभाषितम् kATSOptionFlagsUnRestrictedScope /* since Mac OS X 10.1 */
#परिभाषय kATSOptionFlagsUnRestrictedScope kATSOptionFlagsDefault
#यदि_अन्तः


  /* Set PREFER_LWFN to 1 if LWFN (Type 1) is preferred over
     TrueType in case *both* are available (this is not common,
     but it *is* possible). */
#यदि_अपरिभाषितम् PREFER_LWFN
#परिभाषय PREFER_LWFN  1
#यदि_अन्तः


  /* This function is deprecated because FSSpec is deprecated in Mac OS X  */
  FT_EXPORT_DEF( FT_Error )
  FT_GetFile_From_Mac_Name( ध्रुव वर्ण*  fontName,
                            FSSpec*      मार्गः_Spec,
                            FT_Long*     face_सूचकांक )
  {
    FT_अप्रयुक्तम्( fontName );
    FT_अप्रयुक्तम्( मार्गः_Spec );
    FT_अप्रयुक्तम्( face_सूचकांक );

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
#यदि defined( MAC_OS_X_संस्करणम्_10_5 ) && \
    ( MAC_OS_X_संस्करणम्_MIN_REQUIRED >= MAC_OS_X_संस्करणम्_10_5 )

    OSStatus  err;

    err = ATSFontGetFileReference( ats_font_id, ats_font_ref );

    प्रत्यागम err;
#नोचेद्यदि __LP64__ /* No 64bit Carbon API on legacy platforms */
    FT_अप्रयुक्तम्( ats_font_id );
    FT_अप्रयुक्तम्( ats_font_ref );


    प्रत्यागम fnfErr;
#अन्यथा /* 32bit Carbon API on legacy platforms */
    OSStatus  err;
    FSSpec    spec;


    err = ATSFontGetFileSpecification( ats_font_id, &spec );
    यदि ( noErr == err )
      err = FSpMakeFSRef( &spec, ats_font_ref );

    प्रत्यागम err;
#यदि_अन्तः
  }


  स्थिर FT_Error
  FT_GetFileRef_From_Mac_ATS_Name( ध्रुव वर्ण*  fontName,
                                   FSRef*       ats_font_ref,
                                   FT_Long*     face_सूचकांक )
  {
    CFStringRef  cf_fontName;
    ATSFontRef   ats_font_id;


    *face_सूचकांक = 0;

    cf_fontName = CFStringCreateWithCString( रिक्त, fontName,
                                             kCFStringEncodingMacRoman );
    ats_font_id = ATSFontFindFromName( cf_fontName,
                                       kATSOptionFlagsUnRestrictedScope );
    CFRelease( cf_fontName );

    यदि ( ats_font_id == 0 || ats_font_id == 0xFFFFFFFFUL )
      प्रत्यागम FT_THROW( Unknown_File_Format );

    यदि ( noErr != FT_ATSFontGetFileReference( ats_font_id, ats_font_ref ) )
      प्रत्यागम FT_THROW( Unknown_File_Format );

    /* face_index calculation by searching preceding fontIDs */
    /* with same FSRef                                       */
    {
      ATSFontRef  id2 = ats_font_id - 1;
      FSRef       ref2;


      यावत् ( id2 > 0 )
      {
        यदि ( noErr != FT_ATSFontGetFileReference( id2, &ref2 ) )
          भङ्ग;
        यदि ( noErr != FSCompareFSRefs( ats_font_ref, &ref2 ) )
          भङ्ग;

        id2 --;
      }
      *face_सूचकांक = ats_font_id - ( id2 + 1 );
    }

    प्रत्यागम FT_Err_Ok;
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Get_File_मार्गः_From_Mac_ATS_Name( ध्रुव वर्ण*  fontName,
                                    UInt8*       मार्गः,
                                    UInt32       max_मार्गः_आकार,
                                    FT_Long*     face_सूचकांक )
  {
    FSRef     ref;
    FT_Error  err;


    यदि ( !fontName || !face_सूचकांक )
      प्रत्यागम FT_THROW( Invalid_तर्कः);

    err = FT_GetFileRef_From_Mac_ATS_Name( fontName, &ref, face_सूचकांक );
    यदि ( err )
      प्रत्यागम err;

    यदि ( noErr != FSRef_Make_मार्गः( &ref, मार्गः, max_मार्गः_आकार ) )
      प्रत्यागम FT_THROW( Unknown_File_Format );

    प्रत्यागम FT_Err_Ok;
  }


  /* This function is deprecated because FSSpec is deprecated in Mac OS X  */
  FT_EXPORT_DEF( FT_Error )
  FT_GetFile_From_Mac_ATS_Name( ध्रुव वर्ण*  fontName,
                                FSSpec*      मार्गः_Spec,
                                FT_Long*     face_सूचकांक )
  {
#यदि ( __LP64__ ) || ( defined( MAC_OS_X_संस्करणम्_10_5 ) && \
      ( MAC_OS_X_संस्करणम्_MIN_REQUIRED >= MAC_OS_X_संस्करणम्_10_5 ) )
    FT_अप्रयुक्तम्( fontName );
    FT_अप्रयुक्तम्( मार्गः_Spec );
    FT_अप्रयुक्तम्( face_सूचकांक );

    प्रत्यागम FT_THROW( Unimplemented_Feature );
#अन्यथा
    FSRef     ref;
    FT_Error  err;


    यदि ( !fontName || !face_सूचकांक )
      प्रत्यागम FT_THROW( Invalid_तर्कः );

    err = FT_GetFileRef_From_Mac_ATS_Name( fontName, &ref, face_सूचकांक );
    यदि ( err )
      प्रत्यागम err;

    यदि ( noErr != FSGetCatalogInfo( &ref, kFSCatInfoNone, रिक्त, रिक्त,
                                    मार्गः_Spec, रिक्त ) )
      प्रत्यागम FT_THROW( Unknown_File_Format );

    प्रत्यागम FT_Err_Ok;
#यदि_अन्तः
  }


  स्थिर OSErr
  FT_FSPathMakeRes( ध्रुव UInt8*    pathname,
                    Res_File_Ref_अङ्कः*  res )
  {
    OSErr  err;
    FSRef  ref;


    यदि ( noErr != FSPathMakeRef( pathname, &ref, FALSE ) )
      प्रत्यागम FT_THROW( Cannot_Open_Resource );

    /* at present, no support for dfont format */
    err = FSOpenResourceFile( &ref, 0, रिक्त, fsRdPerm, res );
    यदि ( noErr == err )
      प्रत्यागम err;

    /* fallback to original resource-fork font */
    *res = FSOpenResFile( &ref, fsRdPerm );
    err  = ResError();

    प्रत्यागम err;
  }


  /* Return the file type for given pathname */
  स्थिर OSType
  get_file_type_from_मार्गः( ध्रुव UInt8*  pathname )
  {
    FSRef          ref;
    FSCatalogInfo  info;


    यदि ( noErr != FSPathMakeRef( pathname, &ref, FALSE ) )
      प्रत्यागम ( OSType ) 0;

    यदि ( noErr != FSGetCatalogInfo( &ref, kFSCatInfoFinderInfo, &info,
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

    यावत् ( *q )
    {
      यदि ( ft_isupper( *q ) )
      {
        यदि ( गणनम् )
          max = 3;
        गणनम् = 0;
      }
      यदि ( गणनम् < max && ( ft_isalnum( *q ) || *q == '_' ) )
      {
        *++p = *q;
        lwfn_file_name[0]++;
        गणनम्++;
      }
      q++;
    }
  }


  स्थिर ह्रस्व
  गणनम्_faces_sfnt( वर्ण*  fond_दत्त )
  {
    /* The count is 1 greater than the value in the FOND.  */
    /* Isn't that cute? :-)                                */

    प्रत्यागम EndianS16_BtoN( *( (ह्रस्व*)( fond_दत्त +
                                        परिमाण ( FamRec ) ) ) ) + 1;
  }


  स्थिर ह्रस्व
  गणनम्_faces_scalable( वर्ण*  fond_दत्त )
  {
    Assc_प्रविष्टिः*  assoc;
    ह्रस्व       i, face, face_all;


    face_all = EndianS16_BtoN( *( (ह्रस्व *)( fond_दत्त +
                                             परिमाण ( FamRec ) ) ) ) + 1;
    assoc    = (Assc_प्रविष्टिः*)( fond_दत्त + परिमाण ( FamRec ) + 2 );
    face     = 0;

    प्रति ( i = 0; i < face_all; i++ )
    {
      यदि ( 0 == EndianS16_BtoN( assoc[i].font_आकार ) )
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
  parse_fond( वर्ण*   fond_दत्त,
              ह्रस्व*  have_sfnt,
              ResID*  sfnt_id,
              Str255  lwfn_file_name,
              ह्रस्व   face_सूचकांक )
  {
    Assc_प्रविष्टिः*  assoc;
    Assc_प्रविष्टिः*  आधारः_assoc;
    FamRec*     fond;


    *sfnt_id          = 0;
    *have_sfnt        = 0;
    lwfn_file_name[0] = 0;

    fond       = (FamRec*)fond_दत्त;
    assoc      = (Assc_प्रविष्टिः*)( fond_दत्त + परिमाण ( FamRec ) + 2 );
    आधारः_assoc = assoc;

    /* the maximum faces in a FOND is 48, size of StyleTable.indexes[] */
    यदि ( 47 < face_सूचकांक )
      प्रत्यागम;

    /* Let's do a little range checking before we get too excited here */
    यदि ( face_सूचकांक < गणनम्_faces_sfnt( fond_दत्त ) )
    {
      assoc += face_सूचकांक;        /* add on the face_index! */

      /* if the face at this index is not scalable,
         fall back to the first one (old behavior) */
      यदि ( EndianS16_BtoN( assoc->font_आकार ) == 0 )
      {
        *have_sfnt = 1;
        *sfnt_id   = EndianS16_BtoN( assoc->fontID );
      }
      अन्यथा यदि ( आधारः_assoc->font_आकार == 0 )
      {
        *have_sfnt = 1;
        *sfnt_id   = EndianS16_BtoN( आधारः_assoc->fontID );
      }
    }

    यदि ( EndianS32_BtoN( fond->ffStylOff ) )
    {
      अचिह्नित वर्ण*  p = (अचिह्नित वर्ण*)fond_दत्त;
      Style_सारणी*     style;
      अचिह्नित ह्रस्व  string_गणनम्;
      वर्ण            ps_name[256];
      अचिह्नित वर्ण*  नामानि[64];
      पूर्णाङ्क             i;


      p += EndianS32_BtoN( fond->ffStylOff );
      style = (Style_सारणी*)p;
      p += परिमाण ( Style_सारणी );
      string_गणनम् = EndianS16_BtoN( *(ह्रस्व*)(p) );
      string_गणनम् = FT_MIN( 64, string_गणनम् );
      p += परिमाण ( ह्रस्व );

      प्रति ( i = 0; i < string_गणनम्; i++ )
      {
        नामानि[i] = p;
        p       += नामानि[i][0];
        p++;
      }

      {
        size_t  ps_name_len = (size_t)नामानि[0][0];


        यदि ( ps_name_len != 0 )
        {
          ft_memcpy( ps_name, नामानि[0] + 1, ps_name_len );
          ps_name[ps_name_len] = 0;
        }
        यदि ( style->indexes[face_सूचकांक] > 1 &&
             style->indexes[face_सूचकांक] <= string_गणनम् )
        {
          अचिह्नित वर्ण*  suffixes = नामानि[style->indexes[face_सूचकांक] - 1];


          प्रति ( i = 1; i <= suffixes[0]; i++ )
          {
            अचिह्नित वर्ण*  s;
            size_t          j = suffixes[i] - 1;


            यदि ( j < string_गणनम् && ( s = नामानि[j] ) != रिक्त )
            {
              size_t  s_len = (size_t)s[0];


              यदि ( s_len != 0 && ps_name_len + s_len < परिमाण ( ps_name ) )
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
  lookup_lwfn_by_fond( ध्रुव UInt8*      मार्गः_fond,
                       ConstStr255Param  आधारः_lwfn,
                       UInt8*            मार्गः_lwfn,
                       size_t            मार्गः_आकार )
  {
    FSRef   ref, par_ref;
    size_t  dirname_len;


    /* Pathname for FSRef can be in various formats: HFS, HFS+, and POSIX. */
    /* We should not extract parent directory by string manipulation.      */

    यदि ( noErr != FSPathMakeRef( मार्गः_fond, &ref, FALSE ) )
      प्रत्यागम FT_THROW( Invalid_तर्कः );

    यदि ( noErr != FSGetCatalogInfo( &ref, kFSCatInfoNone,
                                    रिक्त, रिक्त, रिक्त, &par_ref ) )
      प्रत्यागम FT_THROW( Invalid_तर्कः );

    यदि ( noErr != FSRef_Make_मार्गः( &par_ref, मार्गः_lwfn, मार्गः_आकार ) )
      प्रत्यागम FT_THROW( Invalid_तर्कः );

    यदि ( ft_strlen( (वर्ण *)मार्गः_lwfn ) + 1 + आधारः_lwfn[0] > मार्गः_आकार )
      प्रत्यागम FT_THROW( Invalid_तर्कः );

    /* now we have absolute dirname in path_lwfn */
    ft_strcat( (वर्ण *)मार्गः_lwfn, "/" );
    dirname_len = ft_strlen( (वर्ण *)मार्गः_lwfn );
    ft_strcat( (वर्ण *)मार्गः_lwfn, (वर्ण *)आधारः_lwfn + 1 );
    मार्गः_lwfn[dirname_len + आधारः_lwfn[0]] = '\0';

    यदि ( noErr != FSPathMakeRef( मार्गः_lwfn, &ref, FALSE ) )
      प्रत्यागम FT_THROW( Cannot_Open_Resource );

    यदि ( noErr != FSGetCatalogInfo( &ref, kFSCatInfoNone,
                                    रिक्त, रिक्त, रिक्त, रिक्त ) )
      प्रत्यागम FT_THROW( Cannot_Open_Resource );

    प्रत्यागम FT_Err_Ok;
  }


  स्थिर ह्रस्व
  गणनम्_faces( Handle        fond,
               ध्रुव UInt8*  pathname )
  {
    ResID     sfnt_id;
    ह्रस्व     have_sfnt, have_lwfn;
    Str255    lwfn_file_name;
    UInt8     buff[मार्गः_MAX];
    FT_Error  err;
    ह्रस्व     अङ्कः_faces;


    have_sfnt = have_lwfn = 0;

    parse_fond( *fond, &have_sfnt, &sfnt_id, lwfn_file_name, 0 );

    यदि ( lwfn_file_name[0] )
    {
      err = lookup_lwfn_by_fond( pathname, lwfn_file_name,
                                 buff, परिमाण ( buff ) );
      यदि ( !err )
        have_lwfn = 1;
    }

    यदि ( have_lwfn && ( !have_sfnt || PREFER_LWFN ) )
      अङ्कः_faces = 1;
    अन्यथा
      अङ्कः_faces = गणनम्_faces_scalable( *fond );

    प्रत्यागम अङ्कः_faces;
  }


  /* Read Type 1 data from the POST resources inside the LWFN file,
     return a PFB buffer.  This is somewhat convoluted because the FT2
     PFB parser wants the ASCII header as one chunk, and the LWFN
     chunks are often not organized that way, so we glue chunks
     of the same type together. */
  स्थिर FT_Error
  read_lwfn( FT_स्मृतिः      स्मृतिः,
             Res_File_Ref_अङ्कः  res,
             FT_Byte**      pfb_दत्त,
             FT_ULong*      आकार )
  {
    FT_Error       error = FT_Err_Ok;
    ResID          res_id;
    अचिह्नित वर्ण  *बफर, *p, *आकार_p = रिक्त;
    FT_ULong       योग_आकार = 0;
    FT_ULong       old_योग_आकार = 0;
    FT_ULong       post_आकार, pfb_chunk_आकार;
    Handle         post_दत्त;
    वर्ण           संहिता, चरमः_संहिता;


    UseResFile( res );

    /* First pass: load all POST resources, and determine the size of */
    /* the output buffer.                                             */
    res_id    = 501;
    चरमः_संहिता = -1;

    प्रति (;;)
    {
      post_दत्त = Get1Resource( TTAG_POST, res_id++ );
      यदि ( !post_दत्त )
        भङ्ग;  /* we are done */

      संहिता = (*post_दत्त)[0];

      यदि ( संहिता != चरमः_संहिता )
      {
        यदि ( संहिता == 5 )
          योग_आकार += 2; /* just the end code */
        अन्यथा
          योग_आकार += 6; /* code + 4 bytes chunk length */
      }

      योग_आकार += (FT_ULong)Get_Handle_आकार( post_दत्त ) - 2;
      चरमः_संहिता = संहिता;

      /* detect resource fork overflow */
      यदि ( FT_MAC_RFORK_MAX_LEN < योग_आकार )
      {
        error = FT_THROW( Array_Too_Large );
        गच्छ Error;
      }

      old_योग_आकार = योग_आकार;
    }

    यदि ( FT_QALLOC( बफर, (FT_Long)योग_आकार ) )
      गच्छ Error;

    /* Second pass: append all POST data to the buffer, add PFB fields. */
    /* Glue all consecutive chunks of the same type together.           */
    p              = बफर;
    res_id         = 501;
    चरमः_संहिता      = -1;
    pfb_chunk_आकार = 0;

    प्रति (;;)
    {
      post_दत्त = Get1Resource( TTAG_POST, res_id++ );
      यदि ( !post_दत्त )
        भङ्ग;  /* we are done */

      post_आकार = (FT_ULong)Get_Handle_आकार( post_दत्त ) - 2;
      संहिता = (*post_दत्त)[0];

      यदि ( संहिता != चरमः_संहिता )
      {
        यदि ( चरमः_संहिता != -1 )
        {
          /* we are done adding a chunk, fill in the size field */
          यदि ( आकार_p )
          {
            *आकार_p++ = (FT_Byte)(   pfb_chunk_आकार         & 0xFF );
            *आकार_p++ = (FT_Byte)( ( pfb_chunk_आकार >> 8  ) & 0xFF );
            *आकार_p++ = (FT_Byte)( ( pfb_chunk_आकार >> 16 ) & 0xFF );
            *आकार_p++ = (FT_Byte)( ( pfb_chunk_आकार >> 24 ) & 0xFF );
          }
          pfb_chunk_आकार = 0;
        }

        *p++ = 0x80;
        यदि ( संहिता == 5 )
          *p++ = 0x03;  /* the end */
        अन्यथा यदि ( संहिता == 2 )
          *p++ = 0x02;  /* binary segment */
        अन्यथा
          *p++ = 0x01;  /* ASCII segment */

        यदि ( संहिता != 5 )
        {
          आकार_p = p;   /* save for later */
          p += 4;       /* make space for size field */
        }
      }

      ft_memcpy( p, *post_दत्त + 2, post_आकार );
      pfb_chunk_आकार += post_आकार;
      p += post_आकार;
      चरमः_संहिता = संहिता;
    }

    *pfb_दत्त = बफर;
    *आकार = योग_आकार;

  Error:
    CloseResFile( res );
    प्रत्यागम error;
  }


  /* Create a new FT_Face from a file path to an LWFN file. */
  स्थिर FT_Error
  FT_New_Face_From_LWFN( FT_Library    library,
                         ध्रुव UInt8*  pathname,
                         FT_Long       face_सूचकांक,
                         FT_Face*      aface )
  {
    FT_Byte*       pfb_दत्त;
    FT_ULong       pfb_आकार;
    FT_Error       error;
    Res_File_Ref_अङ्कः  res;


    यदि ( noErr != FT_FSPathMakeRes( pathname, &res ) )
      प्रत्यागम FT_THROW( Cannot_Open_Resource );

    pfb_दत्त = रिक्त;
    pfb_आकार = 0;
    error = read_lwfn( library->स्मृतिः, res, &pfb_दत्त, &pfb_आकार );
    CloseResFile( res ); /* PFB is already loaded, useless anymore */
    यदि ( error )
      प्रत्यागम error;

    प्रत्यागम open_face_from_बफर( library,
                                  pfb_दत्त,
                                  pfb_आकार,
                                  face_सूचकांक,
                                  "type1",
                                  aface );
  }


  /* Create a new FT_Face from an SFNT resource, specified by res ID. */
  स्थिर FT_Error
  FT_New_Face_From_SFNT( FT_Library  library,
                         ResID       sfnt_id,
                         FT_Long     face_सूचकांक,
                         FT_Face*    aface )
  {
    Handle     sfnt = रिक्त;
    FT_Byte*   sfnt_दत्त;
    size_t     sfnt_आकार;
    FT_Error   error  = FT_Err_Ok;
    FT_स्मृतिः  स्मृतिः = library->स्मृतिः;
    पूर्णाङ्क        is_cff, is_sfnt_ps;


    sfnt = GetResource( TTAG_sfnt, sfnt_id );
    यदि ( !sfnt )
      प्रत्यागम FT_THROW( Invalid_Handle );

    sfnt_आकार = (FT_ULong)Get_Handle_आकार( sfnt );

    /* detect resource fork overflow */
    यदि ( FT_MAC_RFORK_MAX_LEN < sfnt_आकार )
      प्रत्यागम FT_THROW( Array_Too_Large );

    यदि ( FT_QALLOC( sfnt_दत्त, (FT_Long)sfnt_आकार ) )
    {
      ReleaseResource( sfnt );
      प्रत्यागम error;
    }

    ft_memcpy( sfnt_दत्त, *sfnt, sfnt_आकार );
    ReleaseResource( sfnt );

    is_cff     = sfnt_आकार > 4 && !ft_memcmp( sfnt_दत्त, "OTTO", 4 );
    is_sfnt_ps = sfnt_आकार > 4 && !ft_memcmp( sfnt_दत्त, "typ1", 4 );

    यदि ( is_sfnt_ps )
    {
      FT_प्रवाहः  प्रवाहः;


      यदि ( FT_NEW( प्रवाहः ) )
        गच्छ Try_OpenType;

      FT_प्रवाहः_Open_स्मृतिः( प्रवाहः, sfnt_दत्त, sfnt_आकार );
      यदि ( !open_face_PS_from_sfnt_प्रवाहः( library,
                                           प्रवाहः,
                                           face_सूचकांक,
                                           0, रिक्त,
                                           aface ) )
      {
        FT_प्रवाहः_Close( प्रवाहः );
        FT_FREE( प्रवाहः );
        FT_FREE( sfnt_दत्त );
        गच्छ Exit;
      }

      FT_FREE( प्रवाहः );
    }
  Try_OpenType:
    error = open_face_from_बफर( library,
                                   sfnt_दत्त,
                                   sfnt_आकार,
                                   face_सूचकांक,
                                   is_cff ? "cff" : "truetype",
                                   aface );
  Exit:
    प्रत्यागम error;
  }


  /* Create a new FT_Face from a file path to a suitcase file. */
  स्थिर FT_Error
  FT_New_Face_From_Suitcase( FT_Library    library,
                             ध्रुव UInt8*  pathname,
                             FT_Long       face_सूचकांक,
                             FT_Face*      aface )
  {
    FT_Error       error = FT_ERR( Cannot_Open_Resource );
    Res_File_Ref_अङ्कः  res_ref;
    Resource_सूचकांक  res_सूचकांक;
    Handle         fond;
    ह्रस्व          अङ्कः_faces_अन्तर्_res;
    FT_Long        गणनम्;


    यदि ( noErr != FT_FSPathMakeRes( pathname, &res_ref ) )
      प्रत्यागम FT_THROW( Cannot_Open_Resource );

    UseResFile( res_ref );
    यदि ( ResError() )
      प्रत्यागम FT_THROW( Cannot_Open_Resource );

    res_सूचकांक        = 1;
    अङ्कः_faces_अन्तर्_res = 0;
    गणनम्            = face_सूचकांक;
    यावत् ( गणनम् >= 0 )
    {
      ह्रस्व  अङ्कः_faces_अन्तर्_fond;


      fond = Get1IndResource( TTAG_FOND, res_सूचकांक );
      यदि ( ResError() )
        भङ्ग;

      अङ्कः_faces_अन्तर्_fond  = गणनम्_faces( fond, pathname );
      अङ्कः_faces_अन्तर्_res  += अङ्कः_faces_अन्तर्_fond;

      यदि ( गणनम् < अङ्कः_faces_अन्तर्_fond )
        error = FT_New_Face_From_FOND( library, fond, गणनम्, aface );

      res_सूचकांक++;
      गणनम् -= अङ्कः_faces_अन्तर्_fond;
    }

    CloseResFile( res_ref );

    यदि ( !error && aface && *aface )
    {
      (*aface)->अङ्कः_faces  = अङ्कः_faces_अन्तर्_res;
      (*aface)->face_सूचकांक = face_सूचकांक;
    }

    प्रत्यागम error;
  }


  /* documentation is in ftmac.h */

  FT_EXPORT_DEF( FT_Error )
  FT_New_Face_From_FOND( FT_Library  library,
                         Handle      fond,
                         FT_Long     face_सूचकांक,
                         FT_Face*    aface )
  {
    ह्रस्व     have_sfnt, have_lwfn = 0;
    ResID     sfnt_id, fond_id;
    OSType    fond_type;
    Str255    fond_name;
    Str255    lwfn_file_name;
    UInt8     मार्गः_lwfn[मार्गः_MAX];
    OSErr     err;
    FT_Error  error = FT_Err_Ok;


    /* check of `library' and `aface' delayed to `FT_New_Face_From_XXX' */

    GetResInfo( fond, &fond_id, &fond_type, fond_name );
    यदि ( ResError() != noErr || fond_type != TTAG_FOND )
      प्रत्यागम FT_THROW( Invalid_File_Format );

    parse_fond( *fond, &have_sfnt, &sfnt_id, lwfn_file_name, face_सूचकांक );

    यदि ( lwfn_file_name[0] )
    {
      Res_File_Ref_अङ्कः  res;


      res = HomeResFile( fond );
      यदि ( noErr != ResError() )
        गच्छ प्राप्तम्_no_lwfn_file;

      {
        UInt8  मार्गः_fond[मार्गः_MAX];
        FSRef  ref;


        err = FSGetForkCBInfo( res, k_FSInvalid_Volume_Ref_अङ्कः,
                               रिक्त, रिक्त, रिक्त, &ref, रिक्त );
        यदि ( noErr != err )
          गच्छ प्राप्तम्_no_lwfn_file;

        err = FSRef_Make_मार्गः( &ref, मार्गः_fond, परिमाण ( मार्गः_fond ) );
        यदि ( noErr != err )
          गच्छ प्राप्तम्_no_lwfn_file;

        error = lookup_lwfn_by_fond( मार्गः_fond, lwfn_file_name,
                                     मार्गः_lwfn, परिमाण ( मार्गः_lwfn ) );
        यदि ( !error )
          have_lwfn = 1;
      }
    }

    यदि ( have_lwfn && ( !have_sfnt || PREFER_LWFN ) )
      error = FT_New_Face_From_LWFN( library,
                                     मार्गः_lwfn,
                                     face_सूचकांक,
                                     aface );
    अन्यथा
      error = FT_THROW( Unknown_File_Format );

  प्राप्तम्_no_lwfn_file:
    यदि ( have_sfnt && error )
      error = FT_New_Face_From_SFNT( library,
                                     sfnt_id,
                                     face_सूचकांक,
                                     aface );

    प्रत्यागम error;
  }


  /* Common function to load a new FT_Face from a resource file. */
  स्थिर FT_Error
  FT_New_Face_From_Resource( FT_Library    library,
                             ध्रुव UInt8*  pathname,
                             FT_Long       face_सूचकांक,
                             FT_Face*      aface )
  {
    OSType    file_type;
    FT_Error  error;


    /* LWFN is a (very) specific file format, check for it explicitly */
    file_type = get_file_type_from_मार्गः( pathname );
    यदि ( file_type == TTAG_LWFN )
      प्रत्यागम FT_New_Face_From_LWFN( library, pathname, face_सूचकांक, aface );

    /* Otherwise the file type doesn't matter (there are more than  */
    /* `FFIL' and `tfil').  Just try opening it as a font suitcase; */
    /* if it works, fine.                                           */

    error = FT_New_Face_From_Suitcase( library, pathname, face_सूचकांक, aface );
    यदि ( error )
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
               FT_Long      face_सूचकांक,
               FT_Face*     aface )
  {
    FT_Open_तर्काः  तर्काः;
    FT_Error      error;


    /* test for valid `library' and `aface' delayed to FT_Open_Face() */
    यदि ( !pathname )
      प्रत्यागम FT_THROW( Invalid_तर्कः );

    *aface = रिक्त;

    /* try resourcefork based font: LWFN, FFIL */
    error = FT_New_Face_From_Resource( library, (UInt8 *)pathname,
                                       face_सूचकांक, aface );
    यदि ( error || *aface )
      प्रत्यागम error;

    /* let it fall through to normal loader (.ttf, .otf, etc.) */
    तर्काः.flags    = FT_OPEN_PATHNAME;
    तर्काः.pathname = (वर्ण*)pathname;

    प्रत्यागम FT_Open_Face( library, &तर्काः, face_सूचकांक, aface );
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
                          FT_Long       face_सूचकांक,
                          FT_Face*      aface )
  {
    FT_Error      error;
    FT_Open_तर्काः  तर्काः;

    OSErr  err;
    UInt8  pathname[मार्गः_MAX];


    /* check of `library' and `aface' delayed to */
    /* `FT_New_Face_From_Resource'               */

    यदि ( !ref )
      प्रत्यागम FT_THROW( Invalid_तर्कः );

    err = FSRef_Make_मार्गः( ref, pathname, परिमाण ( pathname ) );
    यदि ( err )
      error = FT_THROW( Cannot_Open_Resource );

    error = FT_New_Face_From_Resource( library, pathname, face_सूचकांक, aface );
    यदि ( error || *aface )
      प्रत्यागम error;

    /* fallback to datafork font */
    तर्काः.flags    = FT_OPEN_PATHNAME;
    तर्काः.pathname = (वर्ण*)pathname;
    प्रत्यागम FT_Open_Face( library, &तर्काः, face_सूचकांक, aface );
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
                           FT_Long        face_सूचकांक,
                           FT_Face*       aface )
  {
#यदि ( __LP64__ ) || ( defined( MAC_OS_X_संस्करणम्_10_5 ) && \
      ( MAC_OS_X_संस्करणम्_MIN_REQUIRED >= MAC_OS_X_संस्करणम्_10_5 ) )
    FT_अप्रयुक्तम्( library );
    FT_अप्रयुक्तम्( spec );
    FT_अप्रयुक्तम्( face_सूचकांक );
    FT_अप्रयुक्तम्( aface );

    प्रत्यागम FT_THROW( Unimplemented_Feature );
#अन्यथा
    FSRef  ref;


    /* check of `library' and `aface' delayed to `FT_New_Face_From_FSRef' */

    यदि ( !spec || FSpMakeFSRef( spec, &ref ) != noErr )
      प्रत्यागम FT_THROW( Invalid_तर्कः );
    अन्यथा
      प्रत्यागम FT_New_Face_From_FSRef( library, &ref, face_सूचकांक, aface );
#यदि_अन्तः
  }

#अन्यथा /* !FT_MACINTOSH */

  /* ANSI C doesn't like empty source files */
  प्रकारनिर्देश पूर्णाङ्क  ft_mac_dummy_;

#यदि_अन्तः /* !FT_MACINTOSH */


/*अन्तः*/
