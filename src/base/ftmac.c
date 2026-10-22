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


#ضمن <freetype/freetype.h>
#ضمن <freetype/tttags.h>
#ضمن <freetype/internal/ftdebug.h>
#ضمن <freetype/internal/ftstream.h>
#ضمن "ftbase.h"


#که_تعریف_شوی FT_MACINTOSH

  /* This is for Mac OS X.  Without redefinition, OS_INLINE */
  /* expands to `static inline' which doesn't survive the   */
  /* -ansi compilation flag of GCC.                         */
#إذا !HAVE_ANSI_OS_INLINE
#تعریف_لغوه  OS_INLINE
#تعریف_کړه OS_INLINE  ساكن __inline__
#د_که_پای

  /* `configure' checks the availability of `ResourceIndex' strictly */
  /* and sets HAVE_TYPE_RESOURCE_INDEX 1 or 0 always.  If it is      */
  /* not set (e.g., a build without `configure'), the availability   */
  /* is guessed from the SDK version.                                */
#که_نه_تعریف_شوی HAVE_TYPE_RESOURCE_شاخص
#إذا !defined( MAC_OS_X_نسخه_10_5 ) || \
    ( MAC_OS_X_نسخه_MAX_ALLOWED < MAC_OS_X_نسخه_10_5 )
#تعریف_کړه HAVE_TYPE_RESOURCE_شاخص 0
#وإلا
#تعریف_کړه HAVE_TYPE_RESOURCE_شاخص 1
#د_که_پای
#د_که_پای /* !HAVE_TYPE_RESOURCE_INDEX */

#إذا ( HAVE_TYPE_RESOURCE_شاخص == 0 )
  تعريفنوع لنډ  Resource_شاخص;
#د_که_پای

#ضمن <CoreServices/CoreServices.h>
#ضمن <ApplicationServices/ApplicationServices.h>
#ضمن <sys/syslimits.h> /* PATH_MAX */

  /* Don't want warnings about our own use of deprecated functions. */
#تعریف_کړه FT_DEPRECATED_ATTRIBUTE

#ضمن <freetype/ftmac.h>

#که_نه_تعریف_شوی kATSOptionFlagsUnRestrictedScope /* since Mac OS X 10.1 */
#تعریف_کړه kATSOptionFlagsUnRestrictedScope kATSOptionFlagsDefault
#د_که_پای


  /* Set PREFER_LWFN to 1 if LWFN (Type 1) is preferred over
     TrueType in case *both* are available (this is not common,
     but it *is* possible). */
#که_نه_تعریف_شوی PREFER_LWFN
#تعریف_کړه PREFER_LWFN  1
#د_که_پای


  /* This function is deprecated because FSSpec is deprecated in Mac OS X  */
  FT_EXPORT_DEF( FT_Error )
  FT_GetFile_From_Mac_Name( ثابت حرف*  fontName,
                            FSSpec*      لار_Spec,
                            FT_Long*     face_شاخص )
  {
    FT_ناکارول_شوی( fontName );
    FT_ناکارول_شوی( لار_Spec );
    FT_ناکارول_شوی( face_شاخص );

    أرجع FT_THROW( Unimplemented_Feature );
  }


  /* Private function.                                         */
  /* The FSSpec type has been discouraged for a long time,     */
  /* unfortunately an FSRef replacement API for                */
  /* ATSFontGetFileSpecification() is only available in        */
  /* Mac OS X 10.5 and later.                                  */
  ساكن OSStatus
  FT_ATSFontGetFileReference( ATSFontRef  ats_font_id,
                              FSRef*      ats_font_ref )
  {
#إذا defined( MAC_OS_X_نسخه_10_5 ) && \
    ( MAC_OS_X_نسخه_MIN_REQUIRED >= MAC_OS_X_نسخه_10_5 )

    OSStatus  err;

    err = ATSFontGetFileReference( ats_font_id, ats_font_ref );

    أرجع err;
#وإلاإذا __LP64__ /* No 64bit Carbon API on legacy platforms */
    FT_ناکارول_شوی( ats_font_id );
    FT_ناکارول_شوی( ats_font_ref );


    أرجع fnfErr;
#وإلا /* 32bit Carbon API on legacy platforms */
    OSStatus  err;
    FSSpec    spec;


    err = ATSFontGetFileSpecification( ats_font_id, &spec );
    إذا ( noErr == err )
      err = FSpMakeFSRef( &spec, ats_font_ref );

    أرجع err;
#د_که_پای
  }


  ساكن FT_Error
  FT_GetFileRef_From_Mac_ATS_Name( ثابت حرف*  fontName,
                                   FSRef*       ats_font_ref,
                                   FT_Long*     face_شاخص )
  {
    CFStringRef  cf_fontName;
    ATSFontRef   ats_font_id;


    *face_شاخص = 0;

    cf_fontName = CFStringCreateWithCString( عدم, fontName,
                                             kCFStringEncodingMacRoman );
    ats_font_id = ATSFont_پیدا_کړه_From_Name( cf_fontName,
                                       kATSOptionFlagsUnRestrictedScope );
    CFRelease( cf_fontName );

    إذا ( ats_font_id == 0 || ats_font_id == 0xFFFFFFFFUL )
      أرجع FT_THROW( Unknown_File_Format );

    إذا ( noErr != FT_ATSFontGetFileReference( ats_font_id, ats_font_ref ) )
      أرجع FT_THROW( Unknown_File_Format );

    /* face_index calculation by searching preceding fontIDs */
    /* with same FSRef                                       */
    {
      ATSFontRef  id2 = ats_font_id - 1;
      FSRef       ref2;


      طالما ( id2 > 0 )
      {
        إذا ( noErr != FT_ATSFontGetFileReference( id2, &ref2 ) )
          اكسر;
        إذا ( noErr != FSCompareFSRefs( ats_font_ref, &ref2 ) )
          اكسر;

        id2 --;
      }
      *face_شاخص = ats_font_id - ( id2 + 1 );
    }

    أرجع FT_Err_Ok;
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Get_File_لار_From_Mac_ATS_Name( ثابت حرف*  fontName,
                                    UInt8*       لار,
                                    UInt32       max_لار_اندازه,
                                    FT_Long*     face_شاخص )
  {
    FSRef     ref;
    FT_Error  err;


    إذا ( !fontName || !face_شاخص )
      أرجع FT_THROW( Invalid_دلیل);

    err = FT_GetFileRef_From_Mac_ATS_Name( fontName, &ref, face_شاخص );
    إذا ( err )
      أرجع err;

    إذا ( noErr != FSRef_Make_لار( &ref, لار, max_لار_اندازه ) )
      أرجع FT_THROW( Unknown_File_Format );

    أرجع FT_Err_Ok;
  }


  /* This function is deprecated because FSSpec is deprecated in Mac OS X  */
  FT_EXPORT_DEF( FT_Error )
  FT_GetFile_From_Mac_ATS_Name( ثابت حرف*  fontName,
                                FSSpec*      لار_Spec,
                                FT_Long*     face_شاخص )
  {
#إذا ( __LP64__ ) || ( defined( MAC_OS_X_نسخه_10_5 ) && \
      ( MAC_OS_X_نسخه_MIN_REQUIRED >= MAC_OS_X_نسخه_10_5 ) )
    FT_ناکارول_شوی( fontName );
    FT_ناکارول_شوی( لار_Spec );
    FT_ناکارول_شوی( face_شاخص );

    أرجع FT_THROW( Unimplemented_Feature );
#وإلا
    FSRef     ref;
    FT_Error  err;


    إذا ( !fontName || !face_شاخص )
      أرجع FT_THROW( Invalid_دلیل );

    err = FT_GetFileRef_From_Mac_ATS_Name( fontName, &ref, face_شاخص );
    إذا ( err )
      أرجع err;

    إذا ( noErr != FSGetCatalogInfo( &ref, kFSCatInfoNone, عدم, عدم,
                                    لار_Spec, عدم ) )
      أرجع FT_THROW( Unknown_File_Format );

    أرجع FT_Err_Ok;
#د_که_پای
  }


  ساكن OSErr
  FT_FSPathMakeRes( ثابت UInt8*    pathname,
                    Res_File_Ref_عدد*  res )
  {
    OSErr  err;
    FSRef  ref;


    إذا ( noErr != FSPathMakeRef( pathname, &ref, FALSE ) )
      أرجع FT_THROW( Cannot_Open_Resource );

    /* at present, no support for dfont format */
    err = FSOpenResourceFile( &ref, 0, عدم, fsRdPerm, res );
    إذا ( noErr == err )
      أرجع err;

    /* fallback to original resource-fork font */
    *res = FSOpenResFile( &ref, fsRdPerm );
    err  = ResError();

    أرجع err;
  }


  /* Return the file type for given pathname */
  ساكن OSType
  get_file_type_from_لار( ثابت UInt8*  pathname )
  {
    FSRef          ref;
    FSCatalogInfo  info;


    إذا ( noErr != FSPathMakeRef( pathname, &ref, FALSE ) )
      أرجع ( OSType ) 0;

    إذا ( noErr != FSGetCatalogInfo( &ref, kFSCatInfoFinderInfo, &info,
                                    عدم, عدم, عدم ) )
      أرجع ( OSType ) 0;

    أرجع ( (FInfo *)( info.finderInfo ) )->fdType;
  }


  /* Given a PostScript font name, create the Macintosh LWFN file name. */
  ساكن فراغ
  create_lwfn_name( حرف*   ps_name,
                    Str255  lwfn_file_name )
  {
    صحيح       max = 5, شمېر = 0;
    FT_Byte*  p = lwfn_file_name;
    FT_Byte*  q = (FT_Byte*)ps_name;


    lwfn_file_name[0] = 0;

    طالما ( *q )
    {
      إذا ( ft_isupper( *q ) )
      {
        إذا ( شمېر )
          max = 3;
        شمېر = 0;
      }
      إذا ( شمېر < max && ( ft_isalnum( *q ) || *q == '_' ) )
      {
        *++p = *q;
        lwfn_file_name[0]++;
        شمېر++;
      }
      q++;
    }
  }


  ساكن لنډ
  شمېر_faces_sfnt( حرف*  fond_معلومات )
  {
    /* The count is 1 greater than the value in the FOND.  */
    /* Isn't that cute? :-)                                */

    أرجع EndianS16_BtoN( *( (لنډ*)( fond_معلومات +
                                        حجم ( FamRec ) ) ) ) + 1;
  }


  ساكن لنډ
  شمېر_faces_scalable( حرف*  fond_معلومات )
  {
    Assc_ثبت*  assoc;
    لنډ       i, face, face_all;


    face_all = EndianS16_BtoN( *( (لنډ *)( fond_معلومات +
                                             حجم ( FamRec ) ) ) ) + 1;
    assoc    = (Assc_ثبت*)( fond_معلومات + حجم ( FamRec ) + 2 );
    face     = 0;

    لكل ( i = 0; i < face_all; i++ )
    {
      إذا ( 0 == EndianS16_BtoN( assoc[i].font_اندازه ) )
        face++;
    }
    أرجع face;
  }


  /* Look inside the FOND data, answer whether there should be an SFNT
     resource, and answer the name of a possible LWFN Type 1 file.

     Thanks to Paul Miller (paulm@profoundeffects.com) for the fix
     to load a face OTHER than the first one in the FOND!
  */


  ساكن فراغ
  parse_fond( حرف*   fond_معلومات,
              لنډ*  have_sfnt,
              ResID*  sfnt_id,
              Str255  lwfn_file_name,
              لنډ   face_شاخص )
  {
    Assc_ثبت*  assoc;
    Assc_ثبت*  بنسټ_assoc;
    FamRec*     fond;


    *sfnt_id          = 0;
    *have_sfnt        = 0;
    lwfn_file_name[0] = 0;

    fond       = (FamRec*)fond_معلومات;
    assoc      = (Assc_ثبت*)( fond_معلومات + حجم ( FamRec ) + 2 );
    بنسټ_assoc = assoc;

    /* the maximum faces in a FOND is 48, size of StyleTable.indexes[] */
    إذا ( 47 < face_شاخص )
      أرجع;

    /* Let's do a little range checking before we get too excited here */
    إذا ( face_شاخص < شمېر_faces_sfnt( fond_معلومات ) )
    {
      assoc += face_شاخص;        /* add on the face_index! */

      /* if the face at this index is not scalable,
         fall back to the first one (old behavior) */
      إذا ( EndianS16_BtoN( assoc->font_اندازه ) == 0 )
      {
        *have_sfnt = 1;
        *sfnt_id   = EndianS16_BtoN( assoc->fontID );
      }
      وإلا إذا ( بنسټ_assoc->font_اندازه == 0 )
      {
        *have_sfnt = 1;
        *sfnt_id   = EndianS16_BtoN( بنسټ_assoc->fontID );
      }
    }

    إذا ( EndianS32_BtoN( fond->ffStylOff ) )
    {
      بینښه حرف*  p = (بینښه حرف*)fond_معلومات;
      Style_جدول*     style;
      بینښه لنډ  string_شمېر;
      حرف            ps_name[256];
      بینښه حرف*  نومونه[64];
      صحيح             i;


      p += EndianS32_BtoN( fond->ffStylOff );
      style = (Style_جدول*)p;
      p += حجم ( Style_جدول );
      string_شمېر = EndianS16_BtoN( *(لنډ*)(p) );
      string_شمېر = FT_MIN( 64, string_شمېر );
      p += حجم ( لنډ );

      لكل ( i = 0; i < string_شمېر; i++ )
      {
        نومونه[i] = p;
        p       += نومونه[i][0];
        p++;
      }

      {
        size_t  ps_name_len = (size_t)نومونه[0][0];


        إذا ( ps_name_len != 0 )
        {
          ft_memcpy( ps_name, نومونه[0] + 1, ps_name_len );
          ps_name[ps_name_len] = 0;
        }
        إذا ( style->indexes[face_شاخص] > 1 &&
             style->indexes[face_شاخص] <= string_شمېر )
        {
          بینښه حرف*  suffixes = نومونه[style->indexes[face_شاخص] - 1];


          لكل ( i = 1; i <= suffixes[0]; i++ )
          {
            بینښه حرف*  s;
            size_t          j = suffixes[i] - 1;


            إذا ( j < string_شمېر && ( s = نومونه[j] ) != عدم )
            {
              size_t  s_len = (size_t)s[0];


              إذا ( s_len != 0 && ps_name_len + s_len < حجم ( ps_name ) )
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


  ساكن  FT_Error
  lookup_lwfn_by_fond( ثابت UInt8*      لار_fond,
                       ConstStr255Param  بنسټ_lwfn,
                       UInt8*            لار_lwfn,
                       size_t            لار_اندازه )
  {
    FSRef   ref, par_ref;
    size_t  dirname_len;


    /* Pathname for FSRef can be in various formats: HFS, HFS+, and POSIX. */
    /* We should not extract parent directory by string manipulation.      */

    إذا ( noErr != FSPathMakeRef( لار_fond, &ref, FALSE ) )
      أرجع FT_THROW( Invalid_دلیل );

    إذا ( noErr != FSGetCatalogInfo( &ref, kFSCatInfoNone,
                                    عدم, عدم, عدم, &par_ref ) )
      أرجع FT_THROW( Invalid_دلیل );

    إذا ( noErr != FSRef_Make_لار( &par_ref, لار_lwfn, لار_اندازه ) )
      أرجع FT_THROW( Invalid_دلیل );

    إذا ( ft_strlen( (حرف *)لار_lwfn ) + 1 + بنسټ_lwfn[0] > لار_اندازه )
      أرجع FT_THROW( Invalid_دلیل );

    /* now we have absolute dirname in path_lwfn */
    ft_strcat( (حرف *)لار_lwfn, "/" );
    dirname_len = ft_strlen( (حرف *)لار_lwfn );
    ft_strcat( (حرف *)لار_lwfn, (حرف *)بنسټ_lwfn + 1 );
    لار_lwfn[dirname_len + بنسټ_lwfn[0]] = '\0';

    إذا ( noErr != FSPathMakeRef( لار_lwfn, &ref, FALSE ) )
      أرجع FT_THROW( Cannot_Open_Resource );

    إذا ( noErr != FSGetCatalogInfo( &ref, kFSCatInfoNone,
                                    عدم, عدم, عدم, عدم ) )
      أرجع FT_THROW( Cannot_Open_Resource );

    أرجع FT_Err_Ok;
  }


  ساكن لنډ
  شمېر_faces( Handle        fond,
               ثابت UInt8*  pathname )
  {
    ResID     sfnt_id;
    لنډ     have_sfnt, have_lwfn;
    Str255    lwfn_file_name;
    UInt8     buff[لار_MAX];
    FT_Error  err;
    لنډ     عدد_faces;


    have_sfnt = have_lwfn = 0;

    parse_fond( *fond, &have_sfnt, &sfnt_id, lwfn_file_name, 0 );

    إذا ( lwfn_file_name[0] )
    {
      err = lookup_lwfn_by_fond( pathname, lwfn_file_name,
                                 buff, حجم ( buff ) );
      إذا ( !err )
        have_lwfn = 1;
    }

    إذا ( have_lwfn && ( !have_sfnt || PREFER_LWFN ) )
      عدد_faces = 1;
    وإلا
      عدد_faces = شمېر_faces_scalable( *fond );

    أرجع عدد_faces;
  }


  /* Read Type 1 data from the POST resources inside the LWFN file,
     return a PFB buffer.  This is somewhat convoluted because the FT2
     PFB parser wants the ASCII header as one chunk, and the LWFN
     chunks are often not organized that way, so we glue chunks
     of the same type together. */
  ساكن FT_Error
  read_lwfn( FT_حافظه      حافظه,
             Res_File_Ref_عدد  res,
             FT_Byte**      pfb_معلومات,
             FT_ULong*      اندازه )
  {
    FT_Error       error = FT_Err_Ok;
    ResID          res_id;
    بینښه حرف  *بفر, *p, *اندازه_p = عدم;
    FT_ULong       ټول_اندازه = 0;
    FT_ULong       old_ټول_اندازه = 0;
    FT_ULong       post_اندازه, pfb_chunk_اندازه;
    Handle         post_معلومات;
    حرف           کوډ, وروستی_کوډ;


    UseResFile( res );

    /* First pass: load all POST resources, and determine the size of */
    /* the output buffer.                                             */
    res_id    = 501;
    وروستی_کوډ = -1;

    لكل (;;)
    {
      post_معلومات = Get1Resource( TTAG_POST, res_id++ );
      إذا ( !post_معلومات )
        اكسر;  /* we are done */

      کوډ = (*post_معلومات)[0];

      إذا ( کوډ != وروستی_کوډ )
      {
        إذا ( کوډ == 5 )
          ټول_اندازه += 2; /* just the end code */
        وإلا
          ټول_اندازه += 6; /* code + 4 bytes chunk length */
      }

      ټول_اندازه += (FT_ULong)Get_Handle_اندازه( post_معلومات ) - 2;
      وروستی_کوډ = کوډ;

      /* detect resource fork overflow */
      إذا ( FT_MAC_RFORK_MAX_LEN < ټول_اندازه )
      {
        error = FT_THROW( Array_Too_Large );
        اذهب Error;
      }

      old_ټول_اندازه = ټول_اندازه;
    }

    إذا ( FT_QALLOC( بفر, (FT_Long)ټول_اندازه ) )
      اذهب Error;

    /* Second pass: append all POST data to the buffer, add PFB fields. */
    /* Glue all consecutive chunks of the same type together.           */
    p              = بفر;
    res_id         = 501;
    وروستی_کوډ      = -1;
    pfb_chunk_اندازه = 0;

    لكل (;;)
    {
      post_معلومات = Get1Resource( TTAG_POST, res_id++ );
      إذا ( !post_معلومات )
        اكسر;  /* we are done */

      post_اندازه = (FT_ULong)Get_Handle_اندازه( post_معلومات ) - 2;
      کوډ = (*post_معلومات)[0];

      إذا ( کوډ != وروستی_کوډ )
      {
        إذا ( وروستی_کوډ != -1 )
        {
          /* we are done adding a chunk, fill in the size field */
          إذا ( اندازه_p )
          {
            *اندازه_p++ = (FT_Byte)(   pfb_chunk_اندازه         & 0xFF );
            *اندازه_p++ = (FT_Byte)( ( pfb_chunk_اندازه >> 8  ) & 0xFF );
            *اندازه_p++ = (FT_Byte)( ( pfb_chunk_اندازه >> 16 ) & 0xFF );
            *اندازه_p++ = (FT_Byte)( ( pfb_chunk_اندازه >> 24 ) & 0xFF );
          }
          pfb_chunk_اندازه = 0;
        }

        *p++ = 0x80;
        إذا ( کوډ == 5 )
          *p++ = 0x03;  /* the end */
        وإلا إذا ( کوډ == 2 )
          *p++ = 0x02;  /* binary segment */
        وإلا
          *p++ = 0x01;  /* ASCII segment */

        إذا ( کوډ != 5 )
        {
          اندازه_p = p;   /* save for later */
          p += 4;       /* make space for size field */
        }
      }

      ft_memcpy( p, *post_معلومات + 2, post_اندازه );
      pfb_chunk_اندازه += post_اندازه;
      p += post_اندازه;
      وروستی_کوډ = کوډ;
    }

    *pfb_معلومات = بفر;
    *اندازه = ټول_اندازه;

  Error:
    CloseResFile( res );
    أرجع error;
  }


  /* Create a new FT_Face from a file path to an LWFN file. */
  ساكن FT_Error
  FT_New_Face_From_LWFN( FT_Library    library,
                         ثابت UInt8*  pathname,
                         FT_Long       face_شاخص,
                         FT_Face*      aface )
  {
    FT_Byte*       pfb_معلومات;
    FT_ULong       pfb_اندازه;
    FT_Error       error;
    Res_File_Ref_عدد  res;


    إذا ( noErr != FT_FSPathMakeRes( pathname, &res ) )
      أرجع FT_THROW( Cannot_Open_Resource );

    pfb_معلومات = عدم;
    pfb_اندازه = 0;
    error = read_lwfn( library->حافظه, res, &pfb_معلومات, &pfb_اندازه );
    CloseResFile( res ); /* PFB is already loaded, useless anymore */
    إذا ( error )
      أرجع error;

    أرجع open_face_from_بفر( library,
                                  pfb_معلومات,
                                  pfb_اندازه,
                                  face_شاخص,
                                  "type1",
                                  aface );
  }


  /* Create a new FT_Face from an SFNT resource, specified by res ID. */
  ساكن FT_Error
  FT_New_Face_From_SFNT( FT_Library  library,
                         ResID       sfnt_id,
                         FT_Long     face_شاخص,
                         FT_Face*    aface )
  {
    Handle     sfnt = عدم;
    FT_Byte*   sfnt_معلومات;
    size_t     sfnt_اندازه;
    FT_Error   error  = FT_Err_Ok;
    FT_حافظه  حافظه = library->حافظه;
    صحيح        is_cff, is_sfnt_ps;


    sfnt = GetResource( TTAG_sfnt, sfnt_id );
    إذا ( !sfnt )
      أرجع FT_THROW( Invalid_Handle );

    sfnt_اندازه = (FT_ULong)Get_Handle_اندازه( sfnt );

    /* detect resource fork overflow */
    إذا ( FT_MAC_RFORK_MAX_LEN < sfnt_اندازه )
      أرجع FT_THROW( Array_Too_Large );

    إذا ( FT_QALLOC( sfnt_معلومات, (FT_Long)sfnt_اندازه ) )
    {
      ReleaseResource( sfnt );
      أرجع error;
    }

    ft_memcpy( sfnt_معلومات, *sfnt, sfnt_اندازه );
    ReleaseResource( sfnt );

    is_cff     = sfnt_اندازه > 4 && !ft_memcmp( sfnt_معلومات, "OTTO", 4 );
    is_sfnt_ps = sfnt_اندازه > 4 && !ft_memcmp( sfnt_معلومات, "typ1", 4 );

    إذا ( is_sfnt_ps )
    {
      FT_بهیر  بهیر;


      إذا ( FT_NEW( بهیر ) )
        اذهب Try_OpenType;

      FT_بهیر_Open_حافظه( بهیر, sfnt_معلومات, sfnt_اندازه );
      إذا ( !open_face_PS_from_sfnt_بهیر( library,
                                           بهیر,
                                           face_شاخص,
                                           0, عدم,
                                           aface ) )
      {
        FT_بهیر_Close( بهیر );
        FT_FREE( بهیر );
        FT_FREE( sfnt_معلومات );
        اذهب Exit;
      }

      FT_FREE( بهیر );
    }
  Try_OpenType:
    error = open_face_from_بفر( library,
                                   sfnt_معلومات,
                                   sfnt_اندازه,
                                   face_شاخص,
                                   is_cff ? "cff" : "truetype",
                                   aface );
  Exit:
    أرجع error;
  }


  /* Create a new FT_Face from a file path to a suitcase file. */
  ساكن FT_Error
  FT_New_Face_From_Suitcase( FT_Library    library,
                             ثابت UInt8*  pathname,
                             FT_Long       face_شاخص,
                             FT_Face*      aface )
  {
    FT_Error       error = FT_ERR( Cannot_Open_Resource );
    Res_File_Ref_عدد  res_ref;
    Resource_شاخص  res_شاخص;
    Handle         fond;
    لنډ          عدد_faces_دننه_res;
    FT_Long        شمېر;


    إذا ( noErr != FT_FSPathMakeRes( pathname, &res_ref ) )
      أرجع FT_THROW( Cannot_Open_Resource );

    UseResFile( res_ref );
    إذا ( ResError() )
      أرجع FT_THROW( Cannot_Open_Resource );

    res_شاخص        = 1;
    عدد_faces_دننه_res = 0;
    شمېر            = face_شاخص;
    طالما ( شمېر >= 0 )
    {
      لنډ  عدد_faces_دننه_fond;


      fond = Get1IndResource( TTAG_FOND, res_شاخص );
      إذا ( ResError() )
        اكسر;

      عدد_faces_دننه_fond  = شمېر_faces( fond, pathname );
      عدد_faces_دننه_res  += عدد_faces_دننه_fond;

      إذا ( شمېر < عدد_faces_دننه_fond )
        error = FT_New_Face_From_FOND( library, fond, شمېر, aface );

      res_شاخص++;
      شمېر -= عدد_faces_دننه_fond;
    }

    CloseResFile( res_ref );

    إذا ( !error && aface && *aface )
    {
      (*aface)->عدد_faces  = عدد_faces_دننه_res;
      (*aface)->face_شاخص = face_شاخص;
    }

    أرجع error;
  }


  /* documentation is in ftmac.h */

  FT_EXPORT_DEF( FT_Error )
  FT_New_Face_From_FOND( FT_Library  library,
                         Handle      fond,
                         FT_Long     face_شاخص,
                         FT_Face*    aface )
  {
    لنډ     have_sfnt, have_lwfn = 0;
    ResID     sfnt_id, fond_id;
    OSType    fond_type;
    Str255    fond_name;
    Str255    lwfn_file_name;
    UInt8     لار_lwfn[لار_MAX];
    OSErr     err;
    FT_Error  error = FT_Err_Ok;


    /* check of `library' and `aface' delayed to `FT_New_Face_From_XXX' */

    GetResInfo( fond, &fond_id, &fond_type, fond_name );
    إذا ( ResError() != noErr || fond_type != TTAG_FOND )
      أرجع FT_THROW( Invalid_File_Format );

    parse_fond( *fond, &have_sfnt, &sfnt_id, lwfn_file_name, face_شاخص );

    إذا ( lwfn_file_name[0] )
    {
      Res_File_Ref_عدد  res;


      res = HomeResFile( fond );
      إذا ( noErr != ResError() )
        اذهب وموندل_no_lwfn_file;

      {
        UInt8  لار_fond[لار_MAX];
        FSRef  ref;


        err = FSGetForkCBInfo( res, k_FSInvalid_Volume_Ref_عدد,
                               عدم, عدم, عدم, &ref, عدم );
        إذا ( noErr != err )
          اذهب وموندل_no_lwfn_file;

        err = FSRef_Make_لار( &ref, لار_fond, حجم ( لار_fond ) );
        إذا ( noErr != err )
          اذهب وموندل_no_lwfn_file;

        error = lookup_lwfn_by_fond( لار_fond, lwfn_file_name,
                                     لار_lwfn, حجم ( لار_lwfn ) );
        إذا ( !error )
          have_lwfn = 1;
      }
    }

    إذا ( have_lwfn && ( !have_sfnt || PREFER_LWFN ) )
      error = FT_New_Face_From_LWFN( library,
                                     لار_lwfn,
                                     face_شاخص,
                                     aface );
    وإلا
      error = FT_THROW( Unknown_File_Format );

  وموندل_no_lwfn_file:
    إذا ( have_sfnt && error )
      error = FT_New_Face_From_SFNT( library,
                                     sfnt_id,
                                     face_شاخص,
                                     aface );

    أرجع error;
  }


  /* Common function to load a new FT_Face from a resource file. */
  ساكن FT_Error
  FT_New_Face_From_Resource( FT_Library    library,
                             ثابت UInt8*  pathname,
                             FT_Long       face_شاخص,
                             FT_Face*      aface )
  {
    OSType    file_type;
    FT_Error  error;


    /* LWFN is a (very) specific file format, check for it explicitly */
    file_type = get_file_type_from_لار( pathname );
    إذا ( file_type == TTAG_LWFN )
      أرجع FT_New_Face_From_LWFN( library, pathname, face_شاخص, aface );

    /* Otherwise the file type doesn't matter (there are more than  */
    /* `FFIL' and `tfil').  Just try opening it as a font suitcase; */
    /* if it works, fine.                                           */

    error = FT_New_Face_From_Suitcase( library, pathname, face_شاخص, aface );
    إذا ( error )
    {
      /* let it fall through to normal loader (.ttf, .otf, etc.); */
      /* we signal this by returning no error and no FT_Face      */
      *aface = عدم;
    }

    أرجع FT_Err_Ok;
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
               ثابت حرف*  pathname,
               FT_Long      face_شاخص,
               FT_Face*     aface )
  {
    FT_Open_دلیلونه  دلیلونه;
    FT_Error      error;


    /* test for valid `library' and `aface' delayed to FT_Open_Face() */
    إذا ( !pathname )
      أرجع FT_THROW( Invalid_دلیل );

    *aface = عدم;

    /* try resourcefork based font: LWFN, FFIL */
    error = FT_New_Face_From_Resource( library, (UInt8 *)pathname,
                                       face_شاخص, aface );
    إذا ( error || *aface )
      أرجع error;

    /* let it fall through to normal loader (.ttf, .otf, etc.) */
    دلیلونه.flags    = FT_OPEN_PATHNAME;
    دلیلونه.pathname = (حرف*)pathname;

    أرجع FT_Open_Face( library, &دلیلونه, face_شاخص, aface );
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
                          FT_Long       face_شاخص,
                          FT_Face*      aface )
  {
    FT_Error      error;
    FT_Open_دلیلونه  دلیلونه;

    OSErr  err;
    UInt8  pathname[لار_MAX];


    /* check of `library' and `aface' delayed to */
    /* `FT_New_Face_From_Resource'               */

    إذا ( !ref )
      أرجع FT_THROW( Invalid_دلیل );

    err = FSRef_Make_لار( ref, pathname, حجم ( pathname ) );
    إذا ( err )
      error = FT_THROW( Cannot_Open_Resource );

    error = FT_New_Face_From_Resource( library, pathname, face_شاخص, aface );
    إذا ( error || *aface )
      أرجع error;

    /* fallback to datafork font */
    دلیلونه.flags    = FT_OPEN_PATHNAME;
    دلیلونه.pathname = (حرف*)pathname;
    أرجع FT_Open_Face( library, &دلیلونه, face_شاخص, aface );
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
                           FT_Long        face_شاخص,
                           FT_Face*       aface )
  {
#إذا ( __LP64__ ) || ( defined( MAC_OS_X_نسخه_10_5 ) && \
      ( MAC_OS_X_نسخه_MIN_REQUIRED >= MAC_OS_X_نسخه_10_5 ) )
    FT_ناکارول_شوی( library );
    FT_ناکارول_شوی( spec );
    FT_ناکارول_شوی( face_شاخص );
    FT_ناکارول_شوی( aface );

    أرجع FT_THROW( Unimplemented_Feature );
#وإلا
    FSRef  ref;


    /* check of `library' and `aface' delayed to `FT_New_Face_From_FSRef' */

    إذا ( !spec || FSpMakeFSRef( spec, &ref ) != noErr )
      أرجع FT_THROW( Invalid_دلیل );
    وإلا
      أرجع FT_New_Face_From_FSRef( library, &ref, face_شاخص, aface );
#د_که_پای
  }

#وإلا /* !FT_MACINTOSH */

  /* ANSI C doesn't like empty source files */
  تعريفنوع صحيح  ft_mac_dummy_;

#د_که_پای /* !FT_MACINTOSH */


/*پای_ټکی*/
