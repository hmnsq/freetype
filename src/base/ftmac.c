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


#اگر_تعریف_شدہ FT_MACINTOSH

  /* This is for Mac OS X.  Without redefinition, OS_INLINE */
  /* expands to `static inline' which doesn't survive the   */
  /* -ansi compilation flag of GCC.                         */
#اگر !HAVE_ANSI_OS_INLINE
#تعریف_منسوخ  OS_INLINE
#تعریف_کرو OS_INLINE  ساکن __inline__
#اگر_ختم

  /* `configure' checks the availability of `ResourceIndex' strictly */
  /* and sets HAVE_TYPE_RESOURCE_INDEX 1 or 0 always.  If it is      */
  /* not set (e.g., a build without `configure'), the availability   */
  /* is guessed from the SDK version.                                */
#اگر_غیر_تعریف_شدہ HAVE_TYPE_RESOURCE_اشاریہ
#اگر !defined( MAC_OS_X_نسخہ_10_5 ) || \
    ( MAC_OS_X_نسخہ_MAX_ALLOWED < MAC_OS_X_نسخہ_10_5 )
#تعریف_کرو HAVE_TYPE_RESOURCE_اشاریہ 0
#ورنہ
#تعریف_کرو HAVE_TYPE_RESOURCE_اشاریہ 1
#اگر_ختم
#اگر_ختم /* !HAVE_TYPE_RESOURCE_INDEX */

#اگر ( HAVE_TYPE_RESOURCE_اشاریہ == 0 )
  قسمتعریف مختصر  Resource_اشاریہ;
#اگر_ختم

#شامل <CoreServices/CoreServices.h>
#شامل <ApplicationServices/ApplicationServices.h>
#شامل <sys/syslimits.h> /* PATH_MAX */

  /* Don't want warnings about our own use of deprecated functions. */
#تعریف_کرو FT_DEPRECATED_ATTRIBUTE

#شامل <freetype/ftmac.h>

#اگر_غیر_تعریف_شدہ kATSOptionFlagsUnRestrictedScope /* since Mac OS X 10.1 */
#تعریف_کرو kATSOptionFlagsUnRestrictedScope kATSOptionFlagsDefault
#اگر_ختم


  /* Set PREFER_LWFN to 1 if LWFN (Type 1) is preferred over
     TrueType in case *both* are available (this is not common,
     but it *is* possible). */
#اگر_غیر_تعریف_شدہ PREFER_LWFN
#تعریف_کرو PREFER_LWFN  1
#اگر_ختم


  /* This function is deprecated because FSSpec is deprecated in Mac OS X  */
  FT_EXPORT_DEF( FT_Error )
  FT_GetFile_From_Mac_Name( ثابت حرف*  fontName,
                            FSSpec*      راستہ_Spec,
                            FT_Long*     face_اشاریہ )
  {
    FT_غیر_مستعمل( fontName );
    FT_غیر_مستعمل( راستہ_Spec );
    FT_غیر_مستعمل( face_اشاریہ );

    واپس FT_THROW( Unimplemented_Feature );
  }


  /* Private function.                                         */
  /* The FSSpec type has been discouraged for a long time,     */
  /* unfortunately an FSRef replacement API for                */
  /* ATSFontGetFileSpecification() is only available in        */
  /* Mac OS X 10.5 and later.                                  */
  ساکن OSStatus
  FT_ATSFontGetFileReference( ATSFontRef  ats_font_id,
                              FSRef*      ats_font_ref )
  {
#اگر defined( MAC_OS_X_نسخہ_10_5 ) && \
    ( MAC_OS_X_نسخہ_MIN_REQUIRED >= MAC_OS_X_نسخہ_10_5 )

    OSStatus  err;

    err = ATSFontGetFileReference( ats_font_id, ats_font_ref );

    واپس err;
#وگرنہ_اگر __LP64__ /* No 64bit Carbon API on legacy platforms */
    FT_غیر_مستعمل( ats_font_id );
    FT_غیر_مستعمل( ats_font_ref );


    واپس fnfErr;
#ورنہ /* 32bit Carbon API on legacy platforms */
    OSStatus  err;
    FSSpec    spec;


    err = ATSFontGetFileSpecification( ats_font_id, &spec );
    اگر ( noErr == err )
      err = FSpMakeFSRef( &spec, ats_font_ref );

    واپس err;
#اگر_ختم
  }


  ساکن FT_Error
  FT_GetFileRef_From_Mac_ATS_Name( ثابت حرف*  fontName,
                                   FSRef*       ats_font_ref,
                                   FT_Long*     face_اشاریہ )
  {
    CFStringRef  cf_fontName;
    ATSFontRef   ats_font_id;


    *face_اشاریہ = 0;

    cf_fontName = CFStringCreateWithCString( تہی, fontName,
                                             kCFStringEncodingMacRoman );
    ats_font_id = ATSFont_ڈھونڈو_From_Name( cf_fontName,
                                       kATSOptionFlagsUnRestrictedScope );
    CFRelease( cf_fontName );

    اگر ( ats_font_id == 0 || ats_font_id == 0xFFFFFFFFUL )
      واپس FT_THROW( Unknown_File_Format );

    اگر ( noErr != FT_ATSFontGetFileReference( ats_font_id, ats_font_ref ) )
      واپس FT_THROW( Unknown_File_Format );

    /* face_index calculation by searching preceding fontIDs */
    /* with same FSRef                                       */
    {
      ATSFontRef  id2 = ats_font_id - 1;
      FSRef       ref2;


      جب ( id2 > 0 )
      {
        اگر ( noErr != FT_ATSFontGetFileReference( id2, &ref2 ) )
          توڑو;
        اگر ( noErr != FSCompareFSRefs( ats_font_ref, &ref2 ) )
          توڑو;

        id2 --;
      }
      *face_اشاریہ = ats_font_id - ( id2 + 1 );
    }

    واپس FT_Err_Ok;
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Get_File_راستہ_From_Mac_ATS_Name( ثابت حرف*  fontName,
                                    UInt8*       راستہ,
                                    UInt32       max_راستہ_سائز,
                                    FT_Long*     face_اشاریہ )
  {
    FSRef     ref;
    FT_Error  err;


    اگر ( !fontName || !face_اشاریہ )
      واپس FT_THROW( Invalid_دلیل);

    err = FT_GetFileRef_From_Mac_ATS_Name( fontName, &ref, face_اشاریہ );
    اگر ( err )
      واپس err;

    اگر ( noErr != FSRef_Make_راستہ( &ref, راستہ, max_راستہ_سائز ) )
      واپس FT_THROW( Unknown_File_Format );

    واپس FT_Err_Ok;
  }


  /* This function is deprecated because FSSpec is deprecated in Mac OS X  */
  FT_EXPORT_DEF( FT_Error )
  FT_GetFile_From_Mac_ATS_Name( ثابت حرف*  fontName,
                                FSSpec*      راستہ_Spec,
                                FT_Long*     face_اشاریہ )
  {
#اگر ( __LP64__ ) || ( defined( MAC_OS_X_نسخہ_10_5 ) && \
      ( MAC_OS_X_نسخہ_MIN_REQUIRED >= MAC_OS_X_نسخہ_10_5 ) )
    FT_غیر_مستعمل( fontName );
    FT_غیر_مستعمل( راستہ_Spec );
    FT_غیر_مستعمل( face_اشاریہ );

    واپس FT_THROW( Unimplemented_Feature );
#ورنہ
    FSRef     ref;
    FT_Error  err;


    اگر ( !fontName || !face_اشاریہ )
      واپس FT_THROW( Invalid_دلیل );

    err = FT_GetFileRef_From_Mac_ATS_Name( fontName, &ref, face_اشاریہ );
    اگر ( err )
      واپس err;

    اگر ( noErr != FSGetCatalogInfo( &ref, kFSCatInfoNone, تہی, تہی,
                                    راستہ_Spec, تہی ) )
      واپس FT_THROW( Unknown_File_Format );

    واپس FT_Err_Ok;
#اگر_ختم
  }


  ساکن OSErr
  FT_FSPathMakeRes( ثابت UInt8*    pathname,
                    Res_File_Ref_عدد*  res )
  {
    OSErr  err;
    FSRef  ref;


    اگر ( noErr != FSPathMakeRef( pathname, &ref, FALSE ) )
      واپس FT_THROW( Cannot_Open_Resource );

    /* at present, no support for dfont format */
    err = FSOpenResourceFile( &ref, 0, تہی, fsRdPerm, res );
    اگر ( noErr == err )
      واپس err;

    /* fallback to original resource-fork font */
    *res = FSOpenResFile( &ref, fsRdPerm );
    err  = ResError();

    واپس err;
  }


  /* Return the file type for given pathname */
  ساکن OSType
  get_file_type_from_راستہ( ثابت UInt8*  pathname )
  {
    FSRef          ref;
    FSCatalogInfo  info;


    اگر ( noErr != FSPathMakeRef( pathname, &ref, FALSE ) )
      واپس ( OSType ) 0;

    اگر ( noErr != FSGetCatalogInfo( &ref, kFSCatInfoFinderInfo, &info,
                                    تہی, تہی, تہی ) )
      واپس ( OSType ) 0;

    واپس ( (FInfo *)( info.finderInfo ) )->fdType;
  }


  /* Given a PostScript font name, create the Macintosh LWFN file name. */
  ساکن خالی
  create_lwfn_name( حرف*   ps_name,
                    Str255  lwfn_file_name )
  {
    صحیح       max = 5, گنتی = 0;
    FT_Byte*  p = lwfn_file_name;
    FT_Byte*  q = (FT_Byte*)ps_name;


    lwfn_file_name[0] = 0;

    جب ( *q )
    {
      اگر ( ft_isupper( *q ) )
      {
        اگر ( گنتی )
          max = 3;
        گنتی = 0;
      }
      اگر ( گنتی < max && ( ft_isalnum( *q ) || *q == '_' ) )
      {
        *++p = *q;
        lwfn_file_name[0]++;
        گنتی++;
      }
      q++;
    }
  }


  ساکن مختصر
  گنتی_faces_sfnt( حرف*  fond_ڈیٹا )
  {
    /* The count is 1 greater than the value in the FOND.  */
    /* Isn't that cute? :-)                                */

    واپس EndianS16_BtoN( *( (مختصر*)( fond_ڈیٹا +
                                        حجم ( FamRec ) ) ) ) + 1;
  }


  ساکن مختصر
  گنتی_faces_scalable( حرف*  fond_ڈیٹا )
  {
    Assc_اندراج*  assoc;
    مختصر       i, face, face_all;


    face_all = EndianS16_BtoN( *( (مختصر *)( fond_ڈیٹا +
                                             حجم ( FamRec ) ) ) ) + 1;
    assoc    = (Assc_اندراج*)( fond_ڈیٹا + حجم ( FamRec ) + 2 );
    face     = 0;

    برائے ( i = 0; i < face_all; i++ )
    {
      اگر ( 0 == EndianS16_BtoN( assoc[i].font_سائز ) )
        face++;
    }
    واپس face;
  }


  /* Look inside the FOND data, answer whether there should be an SFNT
     resource, and answer the name of a possible LWFN Type 1 file.

     Thanks to Paul Miller (paulm@profoundeffects.com) for the fix
     to load a face OTHER than the first one in the FOND!
  */


  ساکن خالی
  parse_fond( حرف*   fond_ڈیٹا,
              مختصر*  have_sfnt,
              ResID*  sfnt_id,
              Str255  lwfn_file_name,
              مختصر   face_اشاریہ )
  {
    Assc_اندراج*  assoc;
    Assc_اندراج*  بنیاد_assoc;
    FamRec*     fond;


    *sfnt_id          = 0;
    *have_sfnt        = 0;
    lwfn_file_name[0] = 0;

    fond       = (FamRec*)fond_ڈیٹا;
    assoc      = (Assc_اندراج*)( fond_ڈیٹا + حجم ( FamRec ) + 2 );
    بنیاد_assoc = assoc;

    /* the maximum faces in a FOND is 48, size of StyleTable.indexes[] */
    اگر ( 47 < face_اشاریہ )
      واپس;

    /* Let's do a little range checking before we get too excited here */
    اگر ( face_اشاریہ < گنتی_faces_sfnt( fond_ڈیٹا ) )
    {
      assoc += face_اشاریہ;        /* add on the face_index! */

      /* if the face at this index is not scalable,
         fall back to the first one (old behavior) */
      اگر ( EndianS16_BtoN( assoc->font_سائز ) == 0 )
      {
        *have_sfnt = 1;
        *sfnt_id   = EndianS16_BtoN( assoc->fontID );
      }
      ورنہ اگر ( بنیاد_assoc->font_سائز == 0 )
      {
        *have_sfnt = 1;
        *sfnt_id   = EndianS16_BtoN( بنیاد_assoc->fontID );
      }
    }

    اگر ( EndianS32_BtoN( fond->ffStylOff ) )
    {
      غیرعلامتی حرف*  p = (غیرعلامتی حرف*)fond_ڈیٹا;
      Style_جدول*     style;
      غیرعلامتی مختصر  string_گنتی;
      حرف            ps_name[256];
      غیرعلامتی حرف*  نام[64];
      صحیح             i;


      p += EndianS32_BtoN( fond->ffStylOff );
      style = (Style_جدول*)p;
      p += حجم ( Style_جدول );
      string_گنتی = EndianS16_BtoN( *(مختصر*)(p) );
      string_گنتی = FT_MIN( 64, string_گنتی );
      p += حجم ( مختصر );

      برائے ( i = 0; i < string_گنتی; i++ )
      {
        نام[i] = p;
        p       += نام[i][0];
        p++;
      }

      {
        size_t  ps_name_len = (size_t)نام[0][0];


        اگر ( ps_name_len != 0 )
        {
          ft_memcpy( ps_name, نام[0] + 1, ps_name_len );
          ps_name[ps_name_len] = 0;
        }
        اگر ( style->indexes[face_اشاریہ] > 1 &&
             style->indexes[face_اشاریہ] <= string_گنتی )
        {
          غیرعلامتی حرف*  suffixes = نام[style->indexes[face_اشاریہ] - 1];


          برائے ( i = 1; i <= suffixes[0]; i++ )
          {
            غیرعلامتی حرف*  s;
            size_t          j = suffixes[i] - 1;


            اگر ( j < string_گنتی && ( s = نام[j] ) != تہی )
            {
              size_t  s_len = (size_t)s[0];


              اگر ( s_len != 0 && ps_name_len + s_len < حجم ( ps_name ) )
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


  ساکن  FT_Error
  lookup_lwfn_by_fond( ثابت UInt8*      راستہ_fond,
                       ConstStr255Param  بنیاد_lwfn,
                       UInt8*            راستہ_lwfn,
                       size_t            راستہ_سائز )
  {
    FSRef   ref, par_ref;
    size_t  dirname_len;


    /* Pathname for FSRef can be in various formats: HFS, HFS+, and POSIX. */
    /* We should not extract parent directory by string manipulation.      */

    اگر ( noErr != FSPathMakeRef( راستہ_fond, &ref, FALSE ) )
      واپس FT_THROW( Invalid_دلیل );

    اگر ( noErr != FSGetCatalogInfo( &ref, kFSCatInfoNone,
                                    تہی, تہی, تہی, &par_ref ) )
      واپس FT_THROW( Invalid_دلیل );

    اگر ( noErr != FSRef_Make_راستہ( &par_ref, راستہ_lwfn, راستہ_سائز ) )
      واپس FT_THROW( Invalid_دلیل );

    اگر ( ft_strlen( (حرف *)راستہ_lwfn ) + 1 + بنیاد_lwfn[0] > راستہ_سائز )
      واپس FT_THROW( Invalid_دلیل );

    /* now we have absolute dirname in path_lwfn */
    ft_strcat( (حرف *)راستہ_lwfn, "/" );
    dirname_len = ft_strlen( (حرف *)راستہ_lwfn );
    ft_strcat( (حرف *)راستہ_lwfn, (حرف *)بنیاد_lwfn + 1 );
    راستہ_lwfn[dirname_len + بنیاد_lwfn[0]] = '\0';

    اگر ( noErr != FSPathMakeRef( راستہ_lwfn, &ref, FALSE ) )
      واپس FT_THROW( Cannot_Open_Resource );

    اگر ( noErr != FSGetCatalogInfo( &ref, kFSCatInfoNone,
                                    تہی, تہی, تہی, تہی ) )
      واپس FT_THROW( Cannot_Open_Resource );

    واپس FT_Err_Ok;
  }


  ساکن مختصر
  گنتی_faces( Handle        fond,
               ثابت UInt8*  pathname )
  {
    ResID     sfnt_id;
    مختصر     have_sfnt, have_lwfn;
    Str255    lwfn_file_name;
    UInt8     buff[راستہ_MAX];
    FT_Error  err;
    مختصر     عدد_faces;


    have_sfnt = have_lwfn = 0;

    parse_fond( *fond, &have_sfnt, &sfnt_id, lwfn_file_name, 0 );

    اگر ( lwfn_file_name[0] )
    {
      err = lookup_lwfn_by_fond( pathname, lwfn_file_name,
                                 buff, حجم ( buff ) );
      اگر ( !err )
        have_lwfn = 1;
    }

    اگر ( have_lwfn && ( !have_sfnt || PREFER_LWFN ) )
      عدد_faces = 1;
    ورنہ
      عدد_faces = گنتی_faces_scalable( *fond );

    واپس عدد_faces;
  }


  /* Read Type 1 data from the POST resources inside the LWFN file,
     return a PFB buffer.  This is somewhat convoluted because the FT2
     PFB parser wants the ASCII header as one chunk, and the LWFN
     chunks are often not organized that way, so we glue chunks
     of the same type together. */
  ساکن FT_Error
  read_lwfn( FT_حافظہ      حافظہ,
             Res_File_Ref_عدد  res,
             FT_Byte**      pfb_ڈیٹا,
             FT_ULong*      سائز )
  {
    FT_Error       error = FT_Err_Ok;
    ResID          res_id;
    غیرعلامتی حرف  *بفر, *p, *سائز_p = تہی;
    FT_ULong       کل_سائز = 0;
    FT_ULong       old_کل_سائز = 0;
    FT_ULong       post_سائز, pfb_chunk_سائز;
    Handle         post_ڈیٹا;
    حرف           رمز, آخری_رمز;


    UseResFile( res );

    /* First pass: load all POST resources, and determine the size of */
    /* the output buffer.                                             */
    res_id    = 501;
    آخری_رمز = -1;

    برائے (;;)
    {
      post_ڈیٹا = Get1Resource( TTAG_POST, res_id++ );
      اگر ( !post_ڈیٹا )
        توڑو;  /* we are done */

      رمز = (*post_ڈیٹا)[0];

      اگر ( رمز != آخری_رمز )
      {
        اگر ( رمز == 5 )
          کل_سائز += 2; /* just the end code */
        ورنہ
          کل_سائز += 6; /* code + 4 bytes chunk length */
      }

      کل_سائز += (FT_ULong)Get_Handle_سائز( post_ڈیٹا ) - 2;
      آخری_رمز = رمز;

      /* detect resource fork overflow */
      اگر ( FT_MAC_RFORK_MAX_LEN < کل_سائز )
      {
        error = FT_THROW( Array_Too_Large );
        جاؤ Error;
      }

      old_کل_سائز = کل_سائز;
    }

    اگر ( FT_QALLOC( بفر, (FT_Long)کل_سائز ) )
      جاؤ Error;

    /* Second pass: append all POST data to the buffer, add PFB fields. */
    /* Glue all consecutive chunks of the same type together.           */
    p              = بفر;
    res_id         = 501;
    آخری_رمز      = -1;
    pfb_chunk_سائز = 0;

    برائے (;;)
    {
      post_ڈیٹا = Get1Resource( TTAG_POST, res_id++ );
      اگر ( !post_ڈیٹا )
        توڑو;  /* we are done */

      post_سائز = (FT_ULong)Get_Handle_سائز( post_ڈیٹا ) - 2;
      رمز = (*post_ڈیٹا)[0];

      اگر ( رمز != آخری_رمز )
      {
        اگر ( آخری_رمز != -1 )
        {
          /* we are done adding a chunk, fill in the size field */
          اگر ( سائز_p )
          {
            *سائز_p++ = (FT_Byte)(   pfb_chunk_سائز         & 0xFF );
            *سائز_p++ = (FT_Byte)( ( pfb_chunk_سائز >> 8  ) & 0xFF );
            *سائز_p++ = (FT_Byte)( ( pfb_chunk_سائز >> 16 ) & 0xFF );
            *سائز_p++ = (FT_Byte)( ( pfb_chunk_سائز >> 24 ) & 0xFF );
          }
          pfb_chunk_سائز = 0;
        }

        *p++ = 0x80;
        اگر ( رمز == 5 )
          *p++ = 0x03;  /* the end */
        ورنہ اگر ( رمز == 2 )
          *p++ = 0x02;  /* binary segment */
        ورنہ
          *p++ = 0x01;  /* ASCII segment */

        اگر ( رمز != 5 )
        {
          سائز_p = p;   /* save for later */
          p += 4;       /* make space for size field */
        }
      }

      ft_memcpy( p, *post_ڈیٹا + 2, post_سائز );
      pfb_chunk_سائز += post_سائز;
      p += post_سائز;
      آخری_رمز = رمز;
    }

    *pfb_ڈیٹا = بفر;
    *سائز = کل_سائز;

  Error:
    CloseResFile( res );
    واپس error;
  }


  /* Create a new FT_Face from a file path to an LWFN file. */
  ساکن FT_Error
  FT_New_Face_From_LWFN( FT_Library    library,
                         ثابت UInt8*  pathname,
                         FT_Long       face_اشاریہ,
                         FT_Face*      aface )
  {
    FT_Byte*       pfb_ڈیٹا;
    FT_ULong       pfb_سائز;
    FT_Error       error;
    Res_File_Ref_عدد  res;


    اگر ( noErr != FT_FSPathMakeRes( pathname, &res ) )
      واپس FT_THROW( Cannot_Open_Resource );

    pfb_ڈیٹا = تہی;
    pfb_سائز = 0;
    error = read_lwfn( library->حافظہ, res, &pfb_ڈیٹا, &pfb_سائز );
    CloseResFile( res ); /* PFB is already loaded, useless anymore */
    اگر ( error )
      واپس error;

    واپس open_face_from_بفر( library,
                                  pfb_ڈیٹا,
                                  pfb_سائز,
                                  face_اشاریہ,
                                  "type1",
                                  aface );
  }


  /* Create a new FT_Face from an SFNT resource, specified by res ID. */
  ساکن FT_Error
  FT_New_Face_From_SFNT( FT_Library  library,
                         ResID       sfnt_id,
                         FT_Long     face_اشاریہ,
                         FT_Face*    aface )
  {
    Handle     sfnt = تہی;
    FT_Byte*   sfnt_ڈیٹا;
    size_t     sfnt_سائز;
    FT_Error   error  = FT_Err_Ok;
    FT_حافظہ  حافظہ = library->حافظہ;
    صحیح        is_cff, is_sfnt_ps;


    sfnt = GetResource( TTAG_sfnt, sfnt_id );
    اگر ( !sfnt )
      واپس FT_THROW( Invalid_Handle );

    sfnt_سائز = (FT_ULong)Get_Handle_سائز( sfnt );

    /* detect resource fork overflow */
    اگر ( FT_MAC_RFORK_MAX_LEN < sfnt_سائز )
      واپس FT_THROW( Array_Too_Large );

    اگر ( FT_QALLOC( sfnt_ڈیٹا, (FT_Long)sfnt_سائز ) )
    {
      ReleaseResource( sfnt );
      واپس error;
    }

    ft_memcpy( sfnt_ڈیٹا, *sfnt, sfnt_سائز );
    ReleaseResource( sfnt );

    is_cff     = sfnt_سائز > 4 && !ft_memcmp( sfnt_ڈیٹا, "OTTO", 4 );
    is_sfnt_ps = sfnt_سائز > 4 && !ft_memcmp( sfnt_ڈیٹا, "typ1", 4 );

    اگر ( is_sfnt_ps )
    {
      FT_دھارا  دھارا;


      اگر ( FT_NEW( دھارا ) )
        جاؤ Try_OpenType;

      FT_دھارا_Open_حافظہ( دھارا, sfnt_ڈیٹا, sfnt_سائز );
      اگر ( !open_face_PS_from_sfnt_دھارا( library,
                                           دھارا,
                                           face_اشاریہ,
                                           0, تہی,
                                           aface ) )
      {
        FT_دھارا_Close( دھارا );
        FT_FREE( دھارا );
        FT_FREE( sfnt_ڈیٹا );
        جاؤ Exit;
      }

      FT_FREE( دھارا );
    }
  Try_OpenType:
    error = open_face_from_بفر( library,
                                   sfnt_ڈیٹا,
                                   sfnt_سائز,
                                   face_اشاریہ,
                                   is_cff ? "cff" : "truetype",
                                   aface );
  Exit:
    واپس error;
  }


  /* Create a new FT_Face from a file path to a suitcase file. */
  ساکن FT_Error
  FT_New_Face_From_Suitcase( FT_Library    library,
                             ثابت UInt8*  pathname,
                             FT_Long       face_اشاریہ,
                             FT_Face*      aface )
  {
    FT_Error       error = FT_ERR( Cannot_Open_Resource );
    Res_File_Ref_عدد  res_ref;
    Resource_اشاریہ  res_اشاریہ;
    Handle         fond;
    مختصر          عدد_faces_اندر_res;
    FT_Long        گنتی;


    اگر ( noErr != FT_FSPathMakeRes( pathname, &res_ref ) )
      واپس FT_THROW( Cannot_Open_Resource );

    UseResFile( res_ref );
    اگر ( ResError() )
      واپس FT_THROW( Cannot_Open_Resource );

    res_اشاریہ        = 1;
    عدد_faces_اندر_res = 0;
    گنتی            = face_اشاریہ;
    جب ( گنتی >= 0 )
    {
      مختصر  عدد_faces_اندر_fond;


      fond = Get1IndResource( TTAG_FOND, res_اشاریہ );
      اگر ( ResError() )
        توڑو;

      عدد_faces_اندر_fond  = گنتی_faces( fond, pathname );
      عدد_faces_اندر_res  += عدد_faces_اندر_fond;

      اگر ( گنتی < عدد_faces_اندر_fond )
        error = FT_New_Face_From_FOND( library, fond, گنتی, aface );

      res_اشاریہ++;
      گنتی -= عدد_faces_اندر_fond;
    }

    CloseResFile( res_ref );

    اگر ( !error && aface && *aface )
    {
      (*aface)->عدد_faces  = عدد_faces_اندر_res;
      (*aface)->face_اشاریہ = face_اشاریہ;
    }

    واپس error;
  }


  /* documentation is in ftmac.h */

  FT_EXPORT_DEF( FT_Error )
  FT_New_Face_From_FOND( FT_Library  library,
                         Handle      fond,
                         FT_Long     face_اشاریہ,
                         FT_Face*    aface )
  {
    مختصر     have_sfnt, have_lwfn = 0;
    ResID     sfnt_id, fond_id;
    OSType    fond_type;
    Str255    fond_name;
    Str255    lwfn_file_name;
    UInt8     راستہ_lwfn[راستہ_MAX];
    OSErr     err;
    FT_Error  error = FT_Err_Ok;


    /* check of `library' and `aface' delayed to `FT_New_Face_From_XXX' */

    GetResInfo( fond, &fond_id, &fond_type, fond_name );
    اگر ( ResError() != noErr || fond_type != TTAG_FOND )
      واپس FT_THROW( Invalid_File_Format );

    parse_fond( *fond, &have_sfnt, &sfnt_id, lwfn_file_name, face_اشاریہ );

    اگر ( lwfn_file_name[0] )
    {
      Res_File_Ref_عدد  res;


      res = HomeResFile( fond );
      اگر ( noErr != ResError() )
        جاؤ ملا_no_lwfn_file;

      {
        UInt8  راستہ_fond[راستہ_MAX];
        FSRef  ref;


        err = FSGetForkCBInfo( res, k_FSInvalid_Volume_Ref_عدد,
                               تہی, تہی, تہی, &ref, تہی );
        اگر ( noErr != err )
          جاؤ ملا_no_lwfn_file;

        err = FSRef_Make_راستہ( &ref, راستہ_fond, حجم ( راستہ_fond ) );
        اگر ( noErr != err )
          جاؤ ملا_no_lwfn_file;

        error = lookup_lwfn_by_fond( راستہ_fond, lwfn_file_name,
                                     راستہ_lwfn, حجم ( راستہ_lwfn ) );
        اگر ( !error )
          have_lwfn = 1;
      }
    }

    اگر ( have_lwfn && ( !have_sfnt || PREFER_LWFN ) )
      error = FT_New_Face_From_LWFN( library,
                                     راستہ_lwfn,
                                     face_اشاریہ,
                                     aface );
    ورنہ
      error = FT_THROW( Unknown_File_Format );

  ملا_no_lwfn_file:
    اگر ( have_sfnt && error )
      error = FT_New_Face_From_SFNT( library,
                                     sfnt_id,
                                     face_اشاریہ,
                                     aface );

    واپس error;
  }


  /* Common function to load a new FT_Face from a resource file. */
  ساکن FT_Error
  FT_New_Face_From_Resource( FT_Library    library,
                             ثابت UInt8*  pathname,
                             FT_Long       face_اشاریہ,
                             FT_Face*      aface )
  {
    OSType    file_type;
    FT_Error  error;


    /* LWFN is a (very) specific file format, check for it explicitly */
    file_type = get_file_type_from_راستہ( pathname );
    اگر ( file_type == TTAG_LWFN )
      واپس FT_New_Face_From_LWFN( library, pathname, face_اشاریہ, aface );

    /* Otherwise the file type doesn't matter (there are more than  */
    /* `FFIL' and `tfil').  Just try opening it as a font suitcase; */
    /* if it works, fine.                                           */

    error = FT_New_Face_From_Suitcase( library, pathname, face_اشاریہ, aface );
    اگر ( error )
    {
      /* let it fall through to normal loader (.ttf, .otf, etc.); */
      /* we signal this by returning no error and no FT_Face      */
      *aface = تہی;
    }

    واپس FT_Err_Ok;
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
               FT_Long      face_اشاریہ,
               FT_Face*     aface )
  {
    FT_Open_دلائل  دلائل;
    FT_Error      error;


    /* test for valid `library' and `aface' delayed to FT_Open_Face() */
    اگر ( !pathname )
      واپس FT_THROW( Invalid_دلیل );

    *aface = تہی;

    /* try resourcefork based font: LWFN, FFIL */
    error = FT_New_Face_From_Resource( library, (UInt8 *)pathname,
                                       face_اشاریہ, aface );
    اگر ( error || *aface )
      واپس error;

    /* let it fall through to normal loader (.ttf, .otf, etc.) */
    دلائل.flags    = FT_OPEN_PATHNAME;
    دلائل.pathname = (حرف*)pathname;

    واپس FT_Open_Face( library, &دلائل, face_اشاریہ, aface );
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
                          FT_Long       face_اشاریہ,
                          FT_Face*      aface )
  {
    FT_Error      error;
    FT_Open_دلائل  دلائل;

    OSErr  err;
    UInt8  pathname[راستہ_MAX];


    /* check of `library' and `aface' delayed to */
    /* `FT_New_Face_From_Resource'               */

    اگر ( !ref )
      واپس FT_THROW( Invalid_دلیل );

    err = FSRef_Make_راستہ( ref, pathname, حجم ( pathname ) );
    اگر ( err )
      error = FT_THROW( Cannot_Open_Resource );

    error = FT_New_Face_From_Resource( library, pathname, face_اشاریہ, aface );
    اگر ( error || *aface )
      واپس error;

    /* fallback to datafork font */
    دلائل.flags    = FT_OPEN_PATHNAME;
    دلائل.pathname = (حرف*)pathname;
    واپس FT_Open_Face( library, &دلائل, face_اشاریہ, aface );
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
                           FT_Long        face_اشاریہ,
                           FT_Face*       aface )
  {
#اگر ( __LP64__ ) || ( defined( MAC_OS_X_نسخہ_10_5 ) && \
      ( MAC_OS_X_نسخہ_MIN_REQUIRED >= MAC_OS_X_نسخہ_10_5 ) )
    FT_غیر_مستعمل( library );
    FT_غیر_مستعمل( spec );
    FT_غیر_مستعمل( face_اشاریہ );
    FT_غیر_مستعمل( aface );

    واپس FT_THROW( Unimplemented_Feature );
#ورنہ
    FSRef  ref;


    /* check of `library' and `aface' delayed to `FT_New_Face_From_FSRef' */

    اگر ( !spec || FSpMakeFSRef( spec, &ref ) != noErr )
      واپس FT_THROW( Invalid_دلیل );
    ورنہ
      واپس FT_New_Face_From_FSRef( library, &ref, face_اشاریہ, aface );
#اگر_ختم
  }

#ورنہ /* !FT_MACINTOSH */

  /* ANSI C doesn't like empty source files */
  قسمتعریف صحیح  ft_mac_dummy_;

#اگر_ختم /* !FT_MACINTOSH */


/*انتہا*/
