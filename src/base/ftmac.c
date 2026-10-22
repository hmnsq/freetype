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


#جيڪڏهن_وصف_ٿيل FT_MACINTOSH

  /* This is for Mac OS X.  Without redefinition, OS_INLINE */
  /* expands to `static inline' which doesn't survive the   */
  /* -ansi compilation flag of GCC.                         */
#إذا !HAVE_ANSI_OS_INLINE
#وصف_رد  OS_INLINE
#وصف_ڪريو OS_INLINE  ساكن __inline__
#جيڪڏهن_پورو

  /* `configure' checks the availability of `ResourceIndex' strictly */
  /* and sets HAVE_TYPE_RESOURCE_INDEX 1 or 0 always.  If it is      */
  /* not set (e.g., a build without `configure'), the availability   */
  /* is guessed from the SDK version.                                */
#جيڪڏهن_وصف_نه_ٿيل HAVE_TYPE_RESOURCE_اشاريو
#إذا !defined( MAC_OS_X_نسخو_10_5 ) || \
    ( MAC_OS_X_نسخو_MAX_ALLOWED < MAC_OS_X_نسخو_10_5 )
#وصف_ڪريو HAVE_TYPE_RESOURCE_اشاريو 0
#وإلا
#وصف_ڪريو HAVE_TYPE_RESOURCE_اشاريو 1
#جيڪڏهن_پورو
#جيڪڏهن_پورو /* !HAVE_TYPE_RESOURCE_INDEX */

#إذا ( HAVE_TYPE_RESOURCE_اشاريو == 0 )
  تعريفنوع ننڍو  Resource_اشاريو;
#جيڪڏهن_پورو

#ضمن <CoreServices/CoreServices.h>
#ضمن <ApplicationServices/ApplicationServices.h>
#ضمن <sys/syslimits.h> /* PATH_MAX */

  /* Don't want warnings about our own use of deprecated functions. */
#وصف_ڪريو FT_DEPRECATED_ATTRIBUTE

#ضمن <freetype/ftmac.h>

#جيڪڏهن_وصف_نه_ٿيل kATSOptionFlagsUnRestrictedScope /* since Mac OS X 10.1 */
#وصف_ڪريو kATSOptionFlagsUnRestrictedScope kATSOptionFlagsDefault
#جيڪڏهن_پورو


  /* Set PREFER_LWFN to 1 if LWFN (Type 1) is preferred over
     TrueType in case *both* are available (this is not common,
     but it *is* possible). */
#جيڪڏهن_وصف_نه_ٿيل PREFER_LWFN
#وصف_ڪريو PREFER_LWFN  1
#جيڪڏهن_پورو


  /* This function is deprecated because FSSpec is deprecated in Mac OS X  */
  FT_EXPORT_DEF( FT_Error )
  FT_GetFile_From_Mac_Name( ثابت حرف*  fontName,
                            FSSpec*      رستو_Spec,
                            FT_Long*     face_اشاريو )
  {
    FT_اڻ_استعمال_ٿيل( fontName );
    FT_اڻ_استعمال_ٿيل( رستو_Spec );
    FT_اڻ_استعمال_ٿيل( face_اشاريو );

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
#إذا defined( MAC_OS_X_نسخو_10_5 ) && \
    ( MAC_OS_X_نسخو_MIN_REQUIRED >= MAC_OS_X_نسخو_10_5 )

    OSStatus  err;

    err = ATSFontGetFileReference( ats_font_id, ats_font_ref );

    أرجع err;
#وإلاإذا __LP64__ /* No 64bit Carbon API on legacy platforms */
    FT_اڻ_استعمال_ٿيل( ats_font_id );
    FT_اڻ_استعمال_ٿيل( ats_font_ref );


    أرجع fnfErr;
#وإلا /* 32bit Carbon API on legacy platforms */
    OSStatus  err;
    FSSpec    spec;


    err = ATSFontGetFileSpecification( ats_font_id, &spec );
    إذا ( noErr == err )
      err = FSpMakeFSRef( &spec, ats_font_ref );

    أرجع err;
#جيڪڏهن_پورو
  }


  ساكن FT_Error
  FT_GetFileRef_From_Mac_ATS_Name( ثابت حرف*  fontName,
                                   FSRef*       ats_font_ref,
                                   FT_Long*     face_اشاريو )
  {
    CFStringRef  cf_fontName;
    ATSFontRef   ats_font_id;


    *face_اشاريو = 0;

    cf_fontName = CFStringCreateWithCString( عدم, fontName,
                                             kCFStringEncodingMacRoman );
    ats_font_id = ATSFont_ڳوليو_From_Name( cf_fontName,
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
      *face_اشاريو = ats_font_id - ( id2 + 1 );
    }

    أرجع FT_Err_Ok;
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Get_File_رستو_From_Mac_ATS_Name( ثابت حرف*  fontName,
                                    UInt8*       رستو,
                                    UInt32       max_رستو_سائيز,
                                    FT_Long*     face_اشاريو )
  {
    FSRef     ref;
    FT_Error  err;


    إذا ( !fontName || !face_اشاريو )
      أرجع FT_THROW( Invalid_دليل);

    err = FT_GetFileRef_From_Mac_ATS_Name( fontName, &ref, face_اشاريو );
    إذا ( err )
      أرجع err;

    إذا ( noErr != FSRef_Make_رستو( &ref, رستو, max_رستو_سائيز ) )
      أرجع FT_THROW( Unknown_File_Format );

    أرجع FT_Err_Ok;
  }


  /* This function is deprecated because FSSpec is deprecated in Mac OS X  */
  FT_EXPORT_DEF( FT_Error )
  FT_GetFile_From_Mac_ATS_Name( ثابت حرف*  fontName,
                                FSSpec*      رستو_Spec,
                                FT_Long*     face_اشاريو )
  {
#إذا ( __LP64__ ) || ( defined( MAC_OS_X_نسخو_10_5 ) && \
      ( MAC_OS_X_نسخو_MIN_REQUIRED >= MAC_OS_X_نسخو_10_5 ) )
    FT_اڻ_استعمال_ٿيل( fontName );
    FT_اڻ_استعمال_ٿيل( رستو_Spec );
    FT_اڻ_استعمال_ٿيل( face_اشاريو );

    أرجع FT_THROW( Unimplemented_Feature );
#وإلا
    FSRef     ref;
    FT_Error  err;


    إذا ( !fontName || !face_اشاريو )
      أرجع FT_THROW( Invalid_دليل );

    err = FT_GetFileRef_From_Mac_ATS_Name( fontName, &ref, face_اشاريو );
    إذا ( err )
      أرجع err;

    إذا ( noErr != FSGetCatalogInfo( &ref, kFSCatInfoNone, عدم, عدم,
                                    رستو_Spec, عدم ) )
      أرجع FT_THROW( Unknown_File_Format );

    أرجع FT_Err_Ok;
#جيڪڏهن_پورو
  }


  ساكن OSErr
  FT_FSPathMakeRes( ثابت UInt8*    pathname,
                    Res_File_Ref_انگ*  res )
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
  get_file_type_from_رستو( ثابت UInt8*  pathname )
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
    صحيح       max = 5, ڳڻپ = 0;
    FT_Byte*  p = lwfn_file_name;
    FT_Byte*  q = (FT_Byte*)ps_name;


    lwfn_file_name[0] = 0;

    طالما ( *q )
    {
      إذا ( ft_isupper( *q ) )
      {
        إذا ( ڳڻپ )
          max = 3;
        ڳڻپ = 0;
      }
      إذا ( ڳڻپ < max && ( ft_isalnum( *q ) || *q == '_' ) )
      {
        *++p = *q;
        lwfn_file_name[0]++;
        ڳڻپ++;
      }
      q++;
    }
  }


  ساكن ننڍو
  ڳڻپ_faces_sfnt( حرف*  fond_معلومات )
  {
    /* The count is 1 greater than the value in the FOND.  */
    /* Isn't that cute? :-)                                */

    أرجع EndianS16_BtoN( *( (ننڍو*)( fond_معلومات +
                                        حجم ( FamRec ) ) ) ) + 1;
  }


  ساكن ننڍو
  ڳڻپ_faces_scalable( حرف*  fond_معلومات )
  {
    Assc_داخلا*  assoc;
    ننڍو       i, face, face_all;


    face_all = EndianS16_BtoN( *( (ننڍو *)( fond_معلومات +
                                             حجم ( FamRec ) ) ) ) + 1;
    assoc    = (Assc_داخلا*)( fond_معلومات + حجم ( FamRec ) + 2 );
    face     = 0;

    لكل ( i = 0; i < face_all; i++ )
    {
      إذا ( 0 == EndianS16_BtoN( assoc[i].font_سائيز ) )
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
              ننڍو*  have_sfnt,
              ResID*  sfnt_id,
              Str255  lwfn_file_name,
              ننڍو   face_اشاريو )
  {
    Assc_داخلا*  assoc;
    Assc_داخلا*  بنياد_assoc;
    FamRec*     fond;


    *sfnt_id          = 0;
    *have_sfnt        = 0;
    lwfn_file_name[0] = 0;

    fond       = (FamRec*)fond_معلومات;
    assoc      = (Assc_داخلا*)( fond_معلومات + حجم ( FamRec ) + 2 );
    بنياد_assoc = assoc;

    /* the maximum faces in a FOND is 48, size of StyleTable.indexes[] */
    إذا ( 47 < face_اشاريو )
      أرجع;

    /* Let's do a little range checking before we get too excited here */
    إذا ( face_اشاريو < ڳڻپ_faces_sfnt( fond_معلومات ) )
    {
      assoc += face_اشاريو;        /* add on the face_index! */

      /* if the face at this index is not scalable,
         fall back to the first one (old behavior) */
      إذا ( EndianS16_BtoN( assoc->font_سائيز ) == 0 )
      {
        *have_sfnt = 1;
        *sfnt_id   = EndianS16_BtoN( assoc->fontID );
      }
      وإلا إذا ( بنياد_assoc->font_سائيز == 0 )
      {
        *have_sfnt = 1;
        *sfnt_id   = EndianS16_BtoN( بنياد_assoc->fontID );
      }
    }

    إذا ( EndianS32_BtoN( fond->ffStylOff ) )
    {
      بنانشان حرف*  p = (بنانشان حرف*)fond_معلومات;
      Style_جدول*     style;
      بنانشان ننڍو  string_ڳڻپ;
      حرف            ps_name[256];
      بنانشان حرف*  نالا[64];
      صحيح             i;


      p += EndianS32_BtoN( fond->ffStylOff );
      style = (Style_جدول*)p;
      p += حجم ( Style_جدول );
      string_ڳڻپ = EndianS16_BtoN( *(ننڍو*)(p) );
      string_ڳڻپ = FT_MIN( 64, string_ڳڻپ );
      p += حجم ( ننڍو );

      لكل ( i = 0; i < string_ڳڻپ; i++ )
      {
        نالا[i] = p;
        p       += نالا[i][0];
        p++;
      }

      {
        size_t  ps_name_len = (size_t)نالا[0][0];


        إذا ( ps_name_len != 0 )
        {
          ft_memcpy( ps_name, نالا[0] + 1, ps_name_len );
          ps_name[ps_name_len] = 0;
        }
        إذا ( style->indexes[face_اشاريو] > 1 &&
             style->indexes[face_اشاريو] <= string_ڳڻپ )
        {
          بنانشان حرف*  suffixes = نالا[style->indexes[face_اشاريو] - 1];


          لكل ( i = 1; i <= suffixes[0]; i++ )
          {
            بنانشان حرف*  s;
            size_t          j = suffixes[i] - 1;


            إذا ( j < string_ڳڻپ && ( s = نالا[j] ) != عدم )
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
  lookup_lwfn_by_fond( ثابت UInt8*      رستو_fond,
                       ConstStr255Param  بنياد_lwfn,
                       UInt8*            رستو_lwfn,
                       size_t            رستو_سائيز )
  {
    FSRef   ref, par_ref;
    size_t  dirname_len;


    /* Pathname for FSRef can be in various formats: HFS, HFS+, and POSIX. */
    /* We should not extract parent directory by string manipulation.      */

    إذا ( noErr != FSPathMakeRef( رستو_fond, &ref, FALSE ) )
      أرجع FT_THROW( Invalid_دليل );

    إذا ( noErr != FSGetCatalogInfo( &ref, kFSCatInfoNone,
                                    عدم, عدم, عدم, &par_ref ) )
      أرجع FT_THROW( Invalid_دليل );

    إذا ( noErr != FSRef_Make_رستو( &par_ref, رستو_lwfn, رستو_سائيز ) )
      أرجع FT_THROW( Invalid_دليل );

    إذا ( ft_strlen( (حرف *)رستو_lwfn ) + 1 + بنياد_lwfn[0] > رستو_سائيز )
      أرجع FT_THROW( Invalid_دليل );

    /* now we have absolute dirname in path_lwfn */
    ft_strcat( (حرف *)رستو_lwfn, "/" );
    dirname_len = ft_strlen( (حرف *)رستو_lwfn );
    ft_strcat( (حرف *)رستو_lwfn, (حرف *)بنياد_lwfn + 1 );
    رستو_lwfn[dirname_len + بنياد_lwfn[0]] = '\0';

    إذا ( noErr != FSPathMakeRef( رستو_lwfn, &ref, FALSE ) )
      أرجع FT_THROW( Cannot_Open_Resource );

    إذا ( noErr != FSGetCatalogInfo( &ref, kFSCatInfoNone,
                                    عدم, عدم, عدم, عدم ) )
      أرجع FT_THROW( Cannot_Open_Resource );

    أرجع FT_Err_Ok;
  }


  ساكن ننڍو
  ڳڻپ_faces( Handle        fond,
               ثابت UInt8*  pathname )
  {
    ResID     sfnt_id;
    ننڍو     have_sfnt, have_lwfn;
    Str255    lwfn_file_name;
    UInt8     buff[رستو_MAX];
    FT_Error  err;
    ننڍو     انگ_faces;


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
      انگ_faces = 1;
    وإلا
      انگ_faces = ڳڻپ_faces_scalable( *fond );

    أرجع انگ_faces;
  }


  /* Read Type 1 data from the POST resources inside the LWFN file,
     return a PFB buffer.  This is somewhat convoluted because the FT2
     PFB parser wants the ASCII header as one chunk, and the LWFN
     chunks are often not organized that way, so we glue chunks
     of the same type together. */
  ساكن FT_Error
  read_lwfn( FT_يادگيري      يادگيري,
             Res_File_Ref_انگ  res,
             FT_Byte**      pfb_معلومات,
             FT_ULong*      سائيز )
  {
    FT_Error       error = FT_Err_Ok;
    ResID          res_id;
    بنانشان حرف  *بفر, *p, *سائيز_p = عدم;
    FT_ULong       ڪل_سائيز = 0;
    FT_ULong       old_ڪل_سائيز = 0;
    FT_ULong       post_سائيز, pfb_chunk_سائيز;
    Handle         post_معلومات;
    حرف           ڪوڊ, آخري_ڪوڊ;


    UseResFile( res );

    /* First pass: load all POST resources, and determine the size of */
    /* the output buffer.                                             */
    res_id    = 501;
    آخري_ڪوڊ = -1;

    لكل (;;)
    {
      post_معلومات = Get1Resource( TTAG_POST, res_id++ );
      إذا ( !post_معلومات )
        اكسر;  /* we are done */

      ڪوڊ = (*post_معلومات)[0];

      إذا ( ڪوڊ != آخري_ڪوڊ )
      {
        إذا ( ڪوڊ == 5 )
          ڪل_سائيز += 2; /* just the end code */
        وإلا
          ڪل_سائيز += 6; /* code + 4 bytes chunk length */
      }

      ڪل_سائيز += (FT_ULong)Get_Handle_سائيز( post_معلومات ) - 2;
      آخري_ڪوڊ = ڪوڊ;

      /* detect resource fork overflow */
      إذا ( FT_MAC_RFORK_MAX_LEN < ڪل_سائيز )
      {
        error = FT_THROW( Array_Too_Large );
        اذهب Error;
      }

      old_ڪل_سائيز = ڪل_سائيز;
    }

    إذا ( FT_QALLOC( بفر, (FT_Long)ڪل_سائيز ) )
      اذهب Error;

    /* Second pass: append all POST data to the buffer, add PFB fields. */
    /* Glue all consecutive chunks of the same type together.           */
    p              = بفر;
    res_id         = 501;
    آخري_ڪوڊ      = -1;
    pfb_chunk_سائيز = 0;

    لكل (;;)
    {
      post_معلومات = Get1Resource( TTAG_POST, res_id++ );
      إذا ( !post_معلومات )
        اكسر;  /* we are done */

      post_سائيز = (FT_ULong)Get_Handle_سائيز( post_معلومات ) - 2;
      ڪوڊ = (*post_معلومات)[0];

      إذا ( ڪوڊ != آخري_ڪوڊ )
      {
        إذا ( آخري_ڪوڊ != -1 )
        {
          /* we are done adding a chunk, fill in the size field */
          إذا ( سائيز_p )
          {
            *سائيز_p++ = (FT_Byte)(   pfb_chunk_سائيز         & 0xFF );
            *سائيز_p++ = (FT_Byte)( ( pfb_chunk_سائيز >> 8  ) & 0xFF );
            *سائيز_p++ = (FT_Byte)( ( pfb_chunk_سائيز >> 16 ) & 0xFF );
            *سائيز_p++ = (FT_Byte)( ( pfb_chunk_سائيز >> 24 ) & 0xFF );
          }
          pfb_chunk_سائيز = 0;
        }

        *p++ = 0x80;
        إذا ( ڪوڊ == 5 )
          *p++ = 0x03;  /* the end */
        وإلا إذا ( ڪوڊ == 2 )
          *p++ = 0x02;  /* binary segment */
        وإلا
          *p++ = 0x01;  /* ASCII segment */

        إذا ( ڪوڊ != 5 )
        {
          سائيز_p = p;   /* save for later */
          p += 4;       /* make space for size field */
        }
      }

      ft_memcpy( p, *post_معلومات + 2, post_سائيز );
      pfb_chunk_سائيز += post_سائيز;
      p += post_سائيز;
      آخري_ڪوڊ = ڪوڊ;
    }

    *pfb_معلومات = بفر;
    *سائيز = ڪل_سائيز;

  Error:
    CloseResFile( res );
    أرجع error;
  }


  /* Create a new FT_Face from a file path to an LWFN file. */
  ساكن FT_Error
  FT_New_Face_From_LWFN( FT_Library    library,
                         ثابت UInt8*  pathname,
                         FT_Long       face_اشاريو,
                         FT_Face*      aface )
  {
    FT_Byte*       pfb_معلومات;
    FT_ULong       pfb_سائيز;
    FT_Error       error;
    Res_File_Ref_انگ  res;


    إذا ( noErr != FT_FSPathMakeRes( pathname, &res ) )
      أرجع FT_THROW( Cannot_Open_Resource );

    pfb_معلومات = عدم;
    pfb_سائيز = 0;
    error = read_lwfn( library->يادگيري, res, &pfb_معلومات, &pfb_سائيز );
    CloseResFile( res ); /* PFB is already loaded, useless anymore */
    إذا ( error )
      أرجع error;

    أرجع open_face_from_بفر( library,
                                  pfb_معلومات,
                                  pfb_سائيز,
                                  face_اشاريو,
                                  "type1",
                                  aface );
  }


  /* Create a new FT_Face from an SFNT resource, specified by res ID. */
  ساكن FT_Error
  FT_New_Face_From_SFNT( FT_Library  library,
                         ResID       sfnt_id,
                         FT_Long     face_اشاريو,
                         FT_Face*    aface )
  {
    Handle     sfnt = عدم;
    FT_Byte*   sfnt_معلومات;
    size_t     sfnt_سائيز;
    FT_Error   error  = FT_Err_Ok;
    FT_يادگيري  يادگيري = library->يادگيري;
    صحيح        is_cff, is_sfnt_ps;


    sfnt = GetResource( TTAG_sfnt, sfnt_id );
    إذا ( !sfnt )
      أرجع FT_THROW( Invalid_Handle );

    sfnt_سائيز = (FT_ULong)Get_Handle_سائيز( sfnt );

    /* detect resource fork overflow */
    إذا ( FT_MAC_RFORK_MAX_LEN < sfnt_سائيز )
      أرجع FT_THROW( Array_Too_Large );

    إذا ( FT_QALLOC( sfnt_معلومات, (FT_Long)sfnt_سائيز ) )
    {
      ReleaseResource( sfnt );
      أرجع error;
    }

    ft_memcpy( sfnt_معلومات, *sfnt, sfnt_سائيز );
    ReleaseResource( sfnt );

    is_cff     = sfnt_سائيز > 4 && !ft_memcmp( sfnt_معلومات, "OTTO", 4 );
    is_sfnt_ps = sfnt_سائيز > 4 && !ft_memcmp( sfnt_معلومات, "typ1", 4 );

    إذا ( is_sfnt_ps )
    {
      FT_وهڪرو  وهڪرو;


      إذا ( FT_NEW( وهڪرو ) )
        اذهب Try_OpenType;

      FT_وهڪرو_Open_يادگيري( وهڪرو, sfnt_معلومات, sfnt_سائيز );
      إذا ( !open_face_PS_from_sfnt_وهڪرو( library,
                                           وهڪرو,
                                           face_اشاريو,
                                           0, عدم,
                                           aface ) )
      {
        FT_وهڪرو_Close( وهڪرو );
        FT_FREE( وهڪرو );
        FT_FREE( sfnt_معلومات );
        اذهب Exit;
      }

      FT_FREE( وهڪرو );
    }
  Try_OpenType:
    error = open_face_from_بفر( library,
                                   sfnt_معلومات,
                                   sfnt_سائيز,
                                   face_اشاريو,
                                   is_cff ? "cff" : "truetype",
                                   aface );
  Exit:
    أرجع error;
  }


  /* Create a new FT_Face from a file path to a suitcase file. */
  ساكن FT_Error
  FT_New_Face_From_Suitcase( FT_Library    library,
                             ثابت UInt8*  pathname,
                             FT_Long       face_اشاريو,
                             FT_Face*      aface )
  {
    FT_Error       error = FT_ERR( Cannot_Open_Resource );
    Res_File_Ref_انگ  res_ref;
    Resource_اشاريو  res_اشاريو;
    Handle         fond;
    ننڍو          انگ_faces_اندريون_res;
    FT_Long        ڳڻپ;


    إذا ( noErr != FT_FSPathMakeRes( pathname, &res_ref ) )
      أرجع FT_THROW( Cannot_Open_Resource );

    UseResFile( res_ref );
    إذا ( ResError() )
      أرجع FT_THROW( Cannot_Open_Resource );

    res_اشاريو        = 1;
    انگ_faces_اندريون_res = 0;
    ڳڻپ            = face_اشاريو;
    طالما ( ڳڻپ >= 0 )
    {
      ننڍو  انگ_faces_اندريون_fond;


      fond = Get1IndResource( TTAG_FOND, res_اشاريو );
      إذا ( ResError() )
        اكسر;

      انگ_faces_اندريون_fond  = ڳڻپ_faces( fond, pathname );
      انگ_faces_اندريون_res  += انگ_faces_اندريون_fond;

      إذا ( ڳڻپ < انگ_faces_اندريون_fond )
        error = FT_New_Face_From_FOND( library, fond, ڳڻپ, aface );

      res_اشاريو++;
      ڳڻپ -= انگ_faces_اندريون_fond;
    }

    CloseResFile( res_ref );

    إذا ( !error && aface && *aface )
    {
      (*aface)->انگ_faces  = انگ_faces_اندريون_res;
      (*aface)->face_اشاريو = face_اشاريو;
    }

    أرجع error;
  }


  /* documentation is in ftmac.h */

  FT_EXPORT_DEF( FT_Error )
  FT_New_Face_From_FOND( FT_Library  library,
                         Handle      fond,
                         FT_Long     face_اشاريو,
                         FT_Face*    aface )
  {
    ننڍو     have_sfnt, have_lwfn = 0;
    ResID     sfnt_id, fond_id;
    OSType    fond_type;
    Str255    fond_name;
    Str255    lwfn_file_name;
    UInt8     رستو_lwfn[رستو_MAX];
    OSErr     err;
    FT_Error  error = FT_Err_Ok;


    /* check of `library' and `aface' delayed to `FT_New_Face_From_XXX' */

    GetResInfo( fond, &fond_id, &fond_type, fond_name );
    إذا ( ResError() != noErr || fond_type != TTAG_FOND )
      أرجع FT_THROW( Invalid_File_Format );

    parse_fond( *fond, &have_sfnt, &sfnt_id, lwfn_file_name, face_اشاريو );

    إذا ( lwfn_file_name[0] )
    {
      Res_File_Ref_انگ  res;


      res = HomeResFile( fond );
      إذا ( noErr != ResError() )
        اذهب مليو_no_lwfn_file;

      {
        UInt8  رستو_fond[رستو_MAX];
        FSRef  ref;


        err = FSGetForkCBInfo( res, k_FSInvalid_Volume_Ref_انگ,
                               عدم, عدم, عدم, &ref, عدم );
        إذا ( noErr != err )
          اذهب مليو_no_lwfn_file;

        err = FSRef_Make_رستو( &ref, رستو_fond, حجم ( رستو_fond ) );
        إذا ( noErr != err )
          اذهب مليو_no_lwfn_file;

        error = lookup_lwfn_by_fond( رستو_fond, lwfn_file_name,
                                     رستو_lwfn, حجم ( رستو_lwfn ) );
        إذا ( !error )
          have_lwfn = 1;
      }
    }

    إذا ( have_lwfn && ( !have_sfnt || PREFER_LWFN ) )
      error = FT_New_Face_From_LWFN( library,
                                     رستو_lwfn,
                                     face_اشاريو,
                                     aface );
    وإلا
      error = FT_THROW( Unknown_File_Format );

  مليو_no_lwfn_file:
    إذا ( have_sfnt && error )
      error = FT_New_Face_From_SFNT( library,
                                     sfnt_id,
                                     face_اشاريو,
                                     aface );

    أرجع error;
  }


  /* Common function to load a new FT_Face from a resource file. */
  ساكن FT_Error
  FT_New_Face_From_Resource( FT_Library    library,
                             ثابت UInt8*  pathname,
                             FT_Long       face_اشاريو,
                             FT_Face*      aface )
  {
    OSType    file_type;
    FT_Error  error;


    /* LWFN is a (very) specific file format, check for it explicitly */
    file_type = get_file_type_from_رستو( pathname );
    إذا ( file_type == TTAG_LWFN )
      أرجع FT_New_Face_From_LWFN( library, pathname, face_اشاريو, aface );

    /* Otherwise the file type doesn't matter (there are more than  */
    /* `FFIL' and `tfil').  Just try opening it as a font suitcase; */
    /* if it works, fine.                                           */

    error = FT_New_Face_From_Suitcase( library, pathname, face_اشاريو, aface );
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
               FT_Long      face_اشاريو,
               FT_Face*     aface )
  {
    FT_Open_دليلون  دليلون;
    FT_Error      error;


    /* test for valid `library' and `aface' delayed to FT_Open_Face() */
    إذا ( !pathname )
      أرجع FT_THROW( Invalid_دليل );

    *aface = عدم;

    /* try resourcefork based font: LWFN, FFIL */
    error = FT_New_Face_From_Resource( library, (UInt8 *)pathname,
                                       face_اشاريو, aface );
    إذا ( error || *aface )
      أرجع error;

    /* let it fall through to normal loader (.ttf, .otf, etc.) */
    دليلون.flags    = FT_OPEN_PATHNAME;
    دليلون.pathname = (حرف*)pathname;

    أرجع FT_Open_Face( library, &دليلون, face_اشاريو, aface );
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
                          FT_Long       face_اشاريو,
                          FT_Face*      aface )
  {
    FT_Error      error;
    FT_Open_دليلون  دليلون;

    OSErr  err;
    UInt8  pathname[رستو_MAX];


    /* check of `library' and `aface' delayed to */
    /* `FT_New_Face_From_Resource'               */

    إذا ( !ref )
      أرجع FT_THROW( Invalid_دليل );

    err = FSRef_Make_رستو( ref, pathname, حجم ( pathname ) );
    إذا ( err )
      error = FT_THROW( Cannot_Open_Resource );

    error = FT_New_Face_From_Resource( library, pathname, face_اشاريو, aface );
    إذا ( error || *aface )
      أرجع error;

    /* fallback to datafork font */
    دليلون.flags    = FT_OPEN_PATHNAME;
    دليلون.pathname = (حرف*)pathname;
    أرجع FT_Open_Face( library, &دليلون, face_اشاريو, aface );
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
                           FT_Long        face_اشاريو,
                           FT_Face*       aface )
  {
#إذا ( __LP64__ ) || ( defined( MAC_OS_X_نسخو_10_5 ) && \
      ( MAC_OS_X_نسخو_MIN_REQUIRED >= MAC_OS_X_نسخو_10_5 ) )
    FT_اڻ_استعمال_ٿيل( library );
    FT_اڻ_استعمال_ٿيل( spec );
    FT_اڻ_استعمال_ٿيل( face_اشاريو );
    FT_اڻ_استعمال_ٿيل( aface );

    أرجع FT_THROW( Unimplemented_Feature );
#وإلا
    FSRef  ref;


    /* check of `library' and `aface' delayed to `FT_New_Face_From_FSRef' */

    إذا ( !spec || FSpMakeFSRef( spec, &ref ) != noErr )
      أرجع FT_THROW( Invalid_دليل );
    وإلا
      أرجع FT_New_Face_From_FSRef( library, &ref, face_اشاريو, aface );
#جيڪڏهن_پورو
  }

#وإلا /* !FT_MACINTOSH */

  /* ANSI C doesn't like empty source files */
  تعريفنوع صحيح  ft_mac_dummy_;

#جيڪڏهن_پورو /* !FT_MACINTOSH */


/*پڇاڙي*/
