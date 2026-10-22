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


#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/freetype.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/tttags.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftdebug.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftstream.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "ftbase.h"


#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ FT_MACINTOSH

  /* This is for Mac OS X.  Without redefinition, OS_INLINE */
  /* expands to `static inline' which doesn't survive the   */
  /* -ansi compilation flag of GCC.                         */
#ਜੇ !HAVE_ANSI_OS_INLINE
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ  OS_INLINE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OS_INLINE  ਸਥਿਰ __inline__
#ਜੇ_ਅੰਤ

  /* `configure' checks the availability of `ResourceIndex' strictly */
  /* and sets HAVE_TYPE_RESOURCE_INDEX 1 or 0 always.  If it is      */
  /* not set (e.g., a build without `configure'), the availability   */
  /* is guessed from the SDK version.                                */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_TYPE_RESOURCE_ਸੂਚਕਾਂਕ
#ਜੇ !defined( MAC_OS_X_ਸੰਸਕਰਣ_10_5 ) || \
    ( MAC_OS_X_ਸੰਸਕਰਣ_MAX_ALLOWED < MAC_OS_X_ਸੰਸਕਰਣ_10_5 )
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HAVE_TYPE_RESOURCE_ਸੂਚਕਾਂਕ 0
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HAVE_TYPE_RESOURCE_ਸੂਚਕਾਂਕ 1
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ /* !HAVE_TYPE_RESOURCE_INDEX */

#ਜੇ ( HAVE_TYPE_RESOURCE_ਸੂਚਕਾਂਕ == 0 )
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਛੋਟਾ  Resource_ਸੂਚਕਾਂਕ;
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ <CoreServices/CoreServices.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ApplicationServices/ApplicationServices.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/syslimits.h> /* PATH_MAX */

  /* Don't want warnings about our own use of deprecated functions. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FT_DEPRECATED_ATTRIBUTE

#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/ftmac.h>

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ kATSOptionFlagsUnRestrictedScope /* since Mac OS X 10.1 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ kATSOptionFlagsUnRestrictedScope kATSOptionFlagsDefault
#ਜੇ_ਅੰਤ


  /* Set PREFER_LWFN to 1 if LWFN (Type 1) is preferred over
     TrueType in case *both* are available (this is not common,
     but it *is* possible). */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ PREFER_LWFN
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PREFER_LWFN  1
#ਜੇ_ਅੰਤ


  /* This function is deprecated because FSSpec is deprecated in Mac OS X  */
  FT_EXPORT_DEF( FT_Error )
  FT_GetFile_From_Mac_Name( ਅਚਲ ਅੱਖਰ*  fontName,
                            FSSpec*      ਰਸਤਾ_Spec,
                            FT_Long*     face_ਸੂਚਕਾਂਕ )
  {
    FT_ਅਣਵਰਤਿਆ( fontName );
    FT_ਅਣਵਰਤਿਆ( ਰਸਤਾ_Spec );
    FT_ਅਣਵਰਤਿਆ( face_ਸੂਚਕਾਂਕ );

    ਵਾਪਸ FT_THROW( Unimplemented_Feature );
  }


  /* Private function.                                         */
  /* The FSSpec type has been discouraged for a long time,     */
  /* unfortunately an FSRef replacement API for                */
  /* ATSFontGetFileSpecification() is only available in        */
  /* Mac OS X 10.5 and later.                                  */
  ਸਥਿਰ OSStatus
  FT_ATSFontGetFileReference( ATSFontRef  ats_font_id,
                              FSRef*      ats_font_ref )
  {
#ਜੇ defined( MAC_OS_X_ਸੰਸਕਰਣ_10_5 ) && \
    ( MAC_OS_X_ਸੰਸਕਰਣ_MIN_REQUIRED >= MAC_OS_X_ਸੰਸਕਰਣ_10_5 )

    OSStatus  err;

    err = ATSFontGetFileReference( ats_font_id, ats_font_ref );

    ਵਾਪਸ err;
#ਜਾਂਜੇ __LP64__ /* No 64bit Carbon API on legacy platforms */
    FT_ਅਣਵਰਤਿਆ( ats_font_id );
    FT_ਅਣਵਰਤਿਆ( ats_font_ref );


    ਵਾਪਸ fnfErr;
#ਨਹੀਂਤਾਂ /* 32bit Carbon API on legacy platforms */
    OSStatus  err;
    FSSpec    spec;


    err = ATSFontGetFileSpecification( ats_font_id, &spec );
    ਜੇ ( noErr == err )
      err = FSpMakeFSRef( &spec, ats_font_ref );

    ਵਾਪਸ err;
#ਜੇ_ਅੰਤ
  }


  ਸਥਿਰ FT_Error
  FT_GetFileRef_From_Mac_ATS_Name( ਅਚਲ ਅੱਖਰ*  fontName,
                                   FSRef*       ats_font_ref,
                                   FT_Long*     face_ਸੂਚਕਾਂਕ )
  {
    CFStringRef  cf_fontName;
    ATSFontRef   ats_font_id;


    *face_ਸੂਚਕਾਂਕ = 0;

    cf_fontName = CFStringCreateWithCString( ਨਿਰਾ, fontName,
                                             kCFStringEncodingMacRoman );
    ats_font_id = ATSFont_ਲੱਭੋ_From_Name( cf_fontName,
                                       kATSOptionFlagsUnRestrictedScope );
    CFRelease( cf_fontName );

    ਜੇ ( ats_font_id == 0 || ats_font_id == 0xFFFFFFFFUL )
      ਵਾਪਸ FT_THROW( Unknown_File_Format );

    ਜੇ ( noErr != FT_ATSFontGetFileReference( ats_font_id, ats_font_ref ) )
      ਵਾਪਸ FT_THROW( Unknown_File_Format );

    /* face_index calculation by searching preceding fontIDs */
    /* with same FSRef                                       */
    {
      ATSFontRef  id2 = ats_font_id - 1;
      FSRef       ref2;


      ਜਦੋਂ ( id2 > 0 )
      {
        ਜੇ ( noErr != FT_ATSFontGetFileReference( id2, &ref2 ) )
          ਰੋਕੋ;
        ਜੇ ( noErr != FSCompareFSRefs( ats_font_ref, &ref2 ) )
          ਰੋਕੋ;

        id2 --;
      }
      *face_ਸੂਚਕਾਂਕ = ats_font_id - ( id2 + 1 );
    }

    ਵਾਪਸ FT_Err_Ok;
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Get_File_ਰਸਤਾ_From_Mac_ATS_Name( ਅਚਲ ਅੱਖਰ*  fontName,
                                    UInt8*       ਰਸਤਾ,
                                    UInt32       max_ਰਸਤਾ_ਮਾਪ,
                                    FT_Long*     face_ਸੂਚਕਾਂਕ )
  {
    FSRef     ref;
    FT_Error  err;


    ਜੇ ( !fontName || !face_ਸੂਚਕਾਂਕ )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ);

    err = FT_GetFileRef_From_Mac_ATS_Name( fontName, &ref, face_ਸੂਚਕਾਂਕ );
    ਜੇ ( err )
      ਵਾਪਸ err;

    ਜੇ ( noErr != FSRef_Make_ਰਸਤਾ( &ref, ਰਸਤਾ, max_ਰਸਤਾ_ਮਾਪ ) )
      ਵਾਪਸ FT_THROW( Unknown_File_Format );

    ਵਾਪਸ FT_Err_Ok;
  }


  /* This function is deprecated because FSSpec is deprecated in Mac OS X  */
  FT_EXPORT_DEF( FT_Error )
  FT_GetFile_From_Mac_ATS_Name( ਅਚਲ ਅੱਖਰ*  fontName,
                                FSSpec*      ਰਸਤਾ_Spec,
                                FT_Long*     face_ਸੂਚਕਾਂਕ )
  {
#ਜੇ ( __LP64__ ) || ( defined( MAC_OS_X_ਸੰਸਕਰਣ_10_5 ) && \
      ( MAC_OS_X_ਸੰਸਕਰਣ_MIN_REQUIRED >= MAC_OS_X_ਸੰਸਕਰਣ_10_5 ) )
    FT_ਅਣਵਰਤਿਆ( fontName );
    FT_ਅਣਵਰਤਿਆ( ਰਸਤਾ_Spec );
    FT_ਅਣਵਰਤਿਆ( face_ਸੂਚਕਾਂਕ );

    ਵਾਪਸ FT_THROW( Unimplemented_Feature );
#ਨਹੀਂਤਾਂ
    FSRef     ref;
    FT_Error  err;


    ਜੇ ( !fontName || !face_ਸੂਚਕਾਂਕ )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    err = FT_GetFileRef_From_Mac_ATS_Name( fontName, &ref, face_ਸੂਚਕਾਂਕ );
    ਜੇ ( err )
      ਵਾਪਸ err;

    ਜੇ ( noErr != FSGetCatalogInfo( &ref, kFSCatInfoNone, ਨਿਰਾ, ਨਿਰਾ,
                                    ਰਸਤਾ_Spec, ਨਿਰਾ ) )
      ਵਾਪਸ FT_THROW( Unknown_File_Format );

    ਵਾਪਸ FT_Err_Ok;
#ਜੇ_ਅੰਤ
  }


  ਸਥਿਰ OSErr
  FT_FSPathMakeRes( ਅਚਲ UInt8*    pathname,
                    Res_File_Ref_ਅੰਕ*  res )
  {
    OSErr  err;
    FSRef  ref;


    ਜੇ ( noErr != FSPathMakeRef( pathname, &ref, FALSE ) )
      ਵਾਪਸ FT_THROW( Cannot_Open_Resource );

    /* at present, no support for dfont format */
    err = FSOpenResourceFile( &ref, 0, ਨਿਰਾ, fsRdPerm, res );
    ਜੇ ( noErr == err )
      ਵਾਪਸ err;

    /* fallback to original resource-fork font */
    *res = FSOpenResFile( &ref, fsRdPerm );
    err  = ResError();

    ਵਾਪਸ err;
  }


  /* Return the file type for given pathname */
  ਸਥਿਰ OSType
  get_file_type_from_ਰਸਤਾ( ਅਚਲ UInt8*  pathname )
  {
    FSRef          ref;
    FSCatalogInfo  info;


    ਜੇ ( noErr != FSPathMakeRef( pathname, &ref, FALSE ) )
      ਵਾਪਸ ( OSType ) 0;

    ਜੇ ( noErr != FSGetCatalogInfo( &ref, kFSCatInfoFinderInfo, &info,
                                    ਨਿਰਾ, ਨਿਰਾ, ਨਿਰਾ ) )
      ਵਾਪਸ ( OSType ) 0;

    ਵਾਪਸ ( (FInfo *)( info.finderInfo ) )->fdType;
  }


  /* Given a PostScript font name, create the Macintosh LWFN file name. */
  ਸਥਿਰ ਖਾਲੀ
  create_lwfn_name( ਅੱਖਰ*   ps_name,
                    Str255  lwfn_file_name )
  {
    ਪੂਰਨ       max = 5, ਗਿਣਤੀ_2 = 0;
    FT_Byte*  p = lwfn_file_name;
    FT_Byte*  q = (FT_Byte*)ps_name;


    lwfn_file_name[0] = 0;

    ਜਦੋਂ ( *q )
    {
      ਜੇ ( ft_isupper( *q ) )
      {
        ਜੇ ( ਗਿਣਤੀ_2 )
          max = 3;
        ਗਿਣਤੀ_2 = 0;
      }
      ਜੇ ( ਗਿਣਤੀ_2 < max && ( ft_isalnum( *q ) || *q == '_' ) )
      {
        *++p = *q;
        lwfn_file_name[0]++;
        ਗਿਣਤੀ_2++;
      }
      q++;
    }
  }


  ਸਥਿਰ ਛੋਟਾ
  ਗਿਣਤੀ_faces_sfnt( ਅੱਖਰ*  fond_ਅੰਕੜੇ )
  {
    /* The count is 1 greater than the value in the FOND.  */
    /* Isn't that cute? :-)                                */

    ਵਾਪਸ EndianS16_BtoN( *( (ਛੋਟਾ*)( fond_ਅੰਕੜੇ +
                                        ਆਕਾਰ ( FamRec ) ) ) ) + 1;
  }


  ਸਥਿਰ ਛੋਟਾ
  ਗਿਣਤੀ_faces_scalable( ਅੱਖਰ*  fond_ਅੰਕੜੇ )
  {
    Assc_ਇੰਦਰਾਜ*  assoc;
    ਛੋਟਾ       i, face, face_all;


    face_all = EndianS16_BtoN( *( (ਛੋਟਾ *)( fond_ਅੰਕੜੇ +
                                             ਆਕਾਰ ( FamRec ) ) ) ) + 1;
    assoc    = (Assc_ਇੰਦਰਾਜ*)( fond_ਅੰਕੜੇ + ਆਕਾਰ ( FamRec ) + 2 );
    face     = 0;

    ਲਈ ( i = 0; i < face_all; i++ )
    {
      ਜੇ ( 0 == EndianS16_BtoN( assoc[i].font_ਮਾਪ ) )
        face++;
    }
    ਵਾਪਸ face;
  }


  /* Look inside the FOND data, answer whether there should be an SFNT
     resource, and answer the name of a possible LWFN Type 1 file.

     Thanks to Paul Miller (paulm@profoundeffects.com) for the fix
     to load a face OTHER than the first one in the FOND!
  */


  ਸਥਿਰ ਖਾਲੀ
  parse_fond( ਅੱਖਰ*   fond_ਅੰਕੜੇ,
              ਛੋਟਾ*  have_sfnt,
              ResID*  sfnt_id,
              Str255  lwfn_file_name,
              ਛੋਟਾ   face_ਸੂਚਕਾਂਕ )
  {
    Assc_ਇੰਦਰਾਜ*  assoc;
    Assc_ਇੰਦਰਾਜ*  ਅਧਾਰ_assoc;
    FamRec*     fond;


    *sfnt_id          = 0;
    *have_sfnt        = 0;
    lwfn_file_name[0] = 0;

    fond       = (FamRec*)fond_ਅੰਕੜੇ;
    assoc      = (Assc_ਇੰਦਰਾਜ*)( fond_ਅੰਕੜੇ + ਆਕਾਰ ( FamRec ) + 2 );
    ਅਧਾਰ_assoc = assoc;

    /* the maximum faces in a FOND is 48, size of StyleTable.indexes[] */
    ਜੇ ( 47 < face_ਸੂਚਕਾਂਕ )
      ਵਾਪਸ;

    /* Let's do a little range checking before we get too excited here */
    ਜੇ ( face_ਸੂਚਕਾਂਕ < ਗਿਣਤੀ_faces_sfnt( fond_ਅੰਕੜੇ ) )
    {
      assoc += face_ਸੂਚਕਾਂਕ;        /* add on the face_index! */

      /* if the face at this index is not scalable,
         fall back to the first one (old behavior) */
      ਜੇ ( EndianS16_BtoN( assoc->font_ਮਾਪ ) == 0 )
      {
        *have_sfnt = 1;
        *sfnt_id   = EndianS16_BtoN( assoc->fontID );
      }
      ਨਹੀਂਤਾਂ ਜੇ ( ਅਧਾਰ_assoc->font_ਮਾਪ == 0 )
      {
        *have_sfnt = 1;
        *sfnt_id   = EndianS16_BtoN( ਅਧਾਰ_assoc->fontID );
      }
    }

    ਜੇ ( EndianS32_BtoN( fond->ffStylOff ) )
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ*  p = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ*)fond_ਅੰਕੜੇ;
      Style_ਸਾਰਣੀ*     style;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਛੋਟਾ  string_ਗਿਣਤੀ;
      ਅੱਖਰ            ps_name[256];
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ*  ਨਾਂ[64];
      ਪੂਰਨ             i;


      p += EndianS32_BtoN( fond->ffStylOff );
      style = (Style_ਸਾਰਣੀ*)p;
      p += ਆਕਾਰ ( Style_ਸਾਰਣੀ );
      string_ਗਿਣਤੀ = EndianS16_BtoN( *(ਛੋਟਾ*)(p) );
      string_ਗਿਣਤੀ = FT_MIN( 64, string_ਗਿਣਤੀ );
      p += ਆਕਾਰ ( ਛੋਟਾ );

      ਲਈ ( i = 0; i < string_ਗਿਣਤੀ; i++ )
      {
        ਨਾਂ[i] = p;
        p       += ਨਾਂ[i][0];
        p++;
      }

      {
        size_t  ps_name_len = (size_t)ਨਾਂ[0][0];


        ਜੇ ( ps_name_len != 0 )
        {
          ft_memcpy( ps_name, ਨਾਂ[0] + 1, ps_name_len );
          ps_name[ps_name_len] = 0;
        }
        ਜੇ ( style->indexes[face_ਸੂਚਕਾਂਕ] > 1 &&
             style->indexes[face_ਸੂਚਕਾਂਕ] <= string_ਗਿਣਤੀ )
        {
          ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ*  suffixes = ਨਾਂ[style->indexes[face_ਸੂਚਕਾਂਕ] - 1];


          ਲਈ ( i = 1; i <= suffixes[0]; i++ )
          {
            ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ*  s;
            size_t          j = suffixes[i] - 1;


            ਜੇ ( j < string_ਗਿਣਤੀ && ( s = ਨਾਂ[j] ) != ਨਿਰਾ )
            {
              size_t  s_len = (size_t)s[0];


              ਜੇ ( s_len != 0 && ps_name_len + s_len < ਆਕਾਰ ( ps_name ) )
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


  ਸਥਿਰ  FT_Error
  lookup_lwfn_by_fond( ਅਚਲ UInt8*      ਰਸਤਾ_fond,
                       ConstStr255Param  ਅਧਾਰ_lwfn,
                       UInt8*            ਰਸਤਾ_lwfn,
                       size_t            ਰਸਤਾ_ਮਾਪ )
  {
    FSRef   ref, par_ref;
    size_t  dirname_len;


    /* Pathname for FSRef can be in various formats: HFS, HFS+, and POSIX. */
    /* We should not extract parent directory by string manipulation.      */

    ਜੇ ( noErr != FSPathMakeRef( ਰਸਤਾ_fond, &ref, FALSE ) )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    ਜੇ ( noErr != FSGetCatalogInfo( &ref, kFSCatInfoNone,
                                    ਨਿਰਾ, ਨਿਰਾ, ਨਿਰਾ, &par_ref ) )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    ਜੇ ( noErr != FSRef_Make_ਰਸਤਾ( &par_ref, ਰਸਤਾ_lwfn, ਰਸਤਾ_ਮਾਪ ) )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    ਜੇ ( ft_strlen( (ਅੱਖਰ *)ਰਸਤਾ_lwfn ) + 1 + ਅਧਾਰ_lwfn[0] > ਰਸਤਾ_ਮਾਪ )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    /* now we have absolute dirname in path_lwfn */
    ft_strcat( (ਅੱਖਰ *)ਰਸਤਾ_lwfn, "/" );
    dirname_len = ft_strlen( (ਅੱਖਰ *)ਰਸਤਾ_lwfn );
    ft_strcat( (ਅੱਖਰ *)ਰਸਤਾ_lwfn, (ਅੱਖਰ *)ਅਧਾਰ_lwfn + 1 );
    ਰਸਤਾ_lwfn[dirname_len + ਅਧਾਰ_lwfn[0]] = '\0';

    ਜੇ ( noErr != FSPathMakeRef( ਰਸਤਾ_lwfn, &ref, FALSE ) )
      ਵਾਪਸ FT_THROW( Cannot_Open_Resource );

    ਜੇ ( noErr != FSGetCatalogInfo( &ref, kFSCatInfoNone,
                                    ਨਿਰਾ, ਨਿਰਾ, ਨਿਰਾ, ਨਿਰਾ ) )
      ਵਾਪਸ FT_THROW( Cannot_Open_Resource );

    ਵਾਪਸ FT_Err_Ok;
  }


  ਸਥਿਰ ਛੋਟਾ
  ਗਿਣਤੀ_faces( Handle        fond,
               ਅਚਲ UInt8*  pathname )
  {
    ResID     sfnt_id;
    ਛੋਟਾ     have_sfnt, have_lwfn;
    Str255    lwfn_file_name;
    UInt8     buff[ਰਸਤਾ_MAX];
    FT_Error  err;
    ਛੋਟਾ     ਅੰਕ_faces;


    have_sfnt = have_lwfn = 0;

    parse_fond( *fond, &have_sfnt, &sfnt_id, lwfn_file_name, 0 );

    ਜੇ ( lwfn_file_name[0] )
    {
      err = lookup_lwfn_by_fond( pathname, lwfn_file_name,
                                 buff, ਆਕਾਰ ( buff ) );
      ਜੇ ( !err )
        have_lwfn = 1;
    }

    ਜੇ ( have_lwfn && ( !have_sfnt || PREFER_LWFN ) )
      ਅੰਕ_faces = 1;
    ਨਹੀਂਤਾਂ
      ਅੰਕ_faces = ਗਿਣਤੀ_faces_scalable( *fond );

    ਵਾਪਸ ਅੰਕ_faces;
  }


  /* Read Type 1 data from the POST resources inside the LWFN file,
     return a PFB buffer.  This is somewhat convoluted because the FT2
     PFB parser wants the ASCII header as one chunk, and the LWFN
     chunks are often not organized that way, so we glue chunks
     of the same type together. */
  ਸਥਿਰ FT_Error
  read_lwfn( FT_ਸਮ੍ਰਿਤੀ      ਸਮ੍ਰਿਤੀ,
             Res_File_Ref_ਅੰਕ  res,
             FT_Byte**      pfb_ਅੰਕੜੇ,
             FT_ULong*      ਮਾਪ )
  {
    FT_Error       error = FT_Err_Ok;
    ResID          res_id;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ  *ਬਫਰ, *p, *ਮਾਪ_p = ਨਿਰਾ;
    FT_ULong       ਕੁੱਲ_ਮਾਪ = 0;
    FT_ULong       old_ਕੁੱਲ_ਮਾਪ = 0;
    FT_ULong       post_ਮਾਪ, pfb_chunk_ਮਾਪ;
    Handle         post_ਅੰਕੜੇ;
    ਅੱਖਰ           ਕੋਡ, ਆਖਰੀ_ਕੋਡ;


    UseResFile( res );

    /* First pass: load all POST resources, and determine the size of */
    /* the output buffer.                                             */
    res_id    = 501;
    ਆਖਰੀ_ਕੋਡ = -1;

    ਲਈ (;;)
    {
      post_ਅੰਕੜੇ = Get1Resource( TTAG_POST, res_id++ );
      ਜੇ ( !post_ਅੰਕੜੇ )
        ਰੋਕੋ;  /* we are done */

      ਕੋਡ = (*post_ਅੰਕੜੇ)[0];

      ਜੇ ( ਕੋਡ != ਆਖਰੀ_ਕੋਡ )
      {
        ਜੇ ( ਕੋਡ == 5 )
          ਕੁੱਲ_ਮਾਪ += 2; /* just the end code */
        ਨਹੀਂਤਾਂ
          ਕੁੱਲ_ਮਾਪ += 6; /* code + 4 bytes chunk length */
      }

      ਕੁੱਲ_ਮਾਪ += (FT_ULong)Get_Handle_ਮਾਪ( post_ਅੰਕੜੇ ) - 2;
      ਆਖਰੀ_ਕੋਡ = ਕੋਡ;

      /* detect resource fork overflow */
      ਜੇ ( FT_MAC_RFORK_MAX_LEN < ਕੁੱਲ_ਮਾਪ )
      {
        error = FT_THROW( Array_Too_Large );
        ਜਾਓ Error;
      }

      old_ਕੁੱਲ_ਮਾਪ = ਕੁੱਲ_ਮਾਪ;
    }

    ਜੇ ( FT_QALLOC( ਬਫਰ, (FT_Long)ਕੁੱਲ_ਮਾਪ ) )
      ਜਾਓ Error;

    /* Second pass: append all POST data to the buffer, add PFB fields. */
    /* Glue all consecutive chunks of the same type together.           */
    p              = ਬਫਰ;
    res_id         = 501;
    ਆਖਰੀ_ਕੋਡ      = -1;
    pfb_chunk_ਮਾਪ = 0;

    ਲਈ (;;)
    {
      post_ਅੰਕੜੇ = Get1Resource( TTAG_POST, res_id++ );
      ਜੇ ( !post_ਅੰਕੜੇ )
        ਰੋਕੋ;  /* we are done */

      post_ਮਾਪ = (FT_ULong)Get_Handle_ਮਾਪ( post_ਅੰਕੜੇ ) - 2;
      ਕੋਡ = (*post_ਅੰਕੜੇ)[0];

      ਜੇ ( ਕੋਡ != ਆਖਰੀ_ਕੋਡ )
      {
        ਜੇ ( ਆਖਰੀ_ਕੋਡ != -1 )
        {
          /* we are done adding a chunk, fill in the size field */
          ਜੇ ( ਮਾਪ_p )
          {
            *ਮਾਪ_p++ = (FT_Byte)(   pfb_chunk_ਮਾਪ         & 0xFF );
            *ਮਾਪ_p++ = (FT_Byte)( ( pfb_chunk_ਮਾਪ >> 8  ) & 0xFF );
            *ਮਾਪ_p++ = (FT_Byte)( ( pfb_chunk_ਮਾਪ >> 16 ) & 0xFF );
            *ਮਾਪ_p++ = (FT_Byte)( ( pfb_chunk_ਮਾਪ >> 24 ) & 0xFF );
          }
          pfb_chunk_ਮਾਪ = 0;
        }

        *p++ = 0x80;
        ਜੇ ( ਕੋਡ == 5 )
          *p++ = 0x03;  /* the end */
        ਨਹੀਂਤਾਂ ਜੇ ( ਕੋਡ == 2 )
          *p++ = 0x02;  /* binary segment */
        ਨਹੀਂਤਾਂ
          *p++ = 0x01;  /* ASCII segment */

        ਜੇ ( ਕੋਡ != 5 )
        {
          ਮਾਪ_p = p;   /* save for later */
          p += 4;       /* make space for size field */
        }
      }

      ft_memcpy( p, *post_ਅੰਕੜੇ + 2, post_ਮਾਪ );
      pfb_chunk_ਮਾਪ += post_ਮਾਪ;
      p += post_ਮਾਪ;
      ਆਖਰੀ_ਕੋਡ = ਕੋਡ;
    }

    *pfb_ਅੰਕੜੇ = ਬਫਰ;
    *ਮਾਪ = ਕੁੱਲ_ਮਾਪ;

  Error:
    CloseResFile( res );
    ਵਾਪਸ error;
  }


  /* Create a new FT_Face from a file path to an LWFN file. */
  ਸਥਿਰ FT_Error
  FT_New_Face_From_LWFN( FT_Library    library,
                         ਅਚਲ UInt8*  pathname,
                         FT_Long       face_ਸੂਚਕਾਂਕ,
                         FT_Face*      aface )
  {
    FT_Byte*       pfb_ਅੰਕੜੇ;
    FT_ULong       pfb_ਮਾਪ;
    FT_Error       error;
    Res_File_Ref_ਅੰਕ  res;


    ਜੇ ( noErr != FT_FSPathMakeRes( pathname, &res ) )
      ਵਾਪਸ FT_THROW( Cannot_Open_Resource );

    pfb_ਅੰਕੜੇ = ਨਿਰਾ;
    pfb_ਮਾਪ = 0;
    error = read_lwfn( library->ਸਮ੍ਰਿਤੀ, res, &pfb_ਅੰਕੜੇ, &pfb_ਮਾਪ );
    CloseResFile( res ); /* PFB is already loaded, useless anymore */
    ਜੇ ( error )
      ਵਾਪਸ error;

    ਵਾਪਸ open_face_from_ਬਫਰ( library,
                                  pfb_ਅੰਕੜੇ,
                                  pfb_ਮਾਪ,
                                  face_ਸੂਚਕਾਂਕ,
                                  "type1",
                                  aface );
  }


  /* Create a new FT_Face from an SFNT resource, specified by res ID. */
  ਸਥਿਰ FT_Error
  FT_New_Face_From_SFNT( FT_Library  library,
                         ResID       sfnt_id,
                         FT_Long     face_ਸੂਚਕਾਂਕ,
                         FT_Face*    aface )
  {
    Handle     sfnt = ਨਿਰਾ;
    FT_Byte*   sfnt_ਅੰਕੜੇ;
    size_t     sfnt_ਮਾਪ;
    FT_Error   error  = FT_Err_Ok;
    FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ = library->ਸਮ੍ਰਿਤੀ;
    ਪੂਰਨ        is_cff, is_sfnt_ps;


    sfnt = GetResource( TTAG_sfnt, sfnt_id );
    ਜੇ ( !sfnt )
      ਵਾਪਸ FT_THROW( Invalid_Handle );

    sfnt_ਮਾਪ = (FT_ULong)Get_Handle_ਮਾਪ( sfnt );

    /* detect resource fork overflow */
    ਜੇ ( FT_MAC_RFORK_MAX_LEN < sfnt_ਮਾਪ )
      ਵਾਪਸ FT_THROW( Array_Too_Large );

    ਜੇ ( FT_QALLOC( sfnt_ਅੰਕੜੇ, (FT_Long)sfnt_ਮਾਪ ) )
    {
      ReleaseResource( sfnt );
      ਵਾਪਸ error;
    }

    ft_memcpy( sfnt_ਅੰਕੜੇ, *sfnt, sfnt_ਮਾਪ );
    ReleaseResource( sfnt );

    is_cff     = sfnt_ਮਾਪ > 4 && !ft_memcmp( sfnt_ਅੰਕੜੇ, "OTTO", 4 );
    is_sfnt_ps = sfnt_ਮਾਪ > 4 && !ft_memcmp( sfnt_ਅੰਕੜੇ, "typ1", 4 );

    ਜੇ ( is_sfnt_ps )
    {
      FT_ਧਾਰਾ  ਧਾਰਾ;


      ਜੇ ( FT_NEW( ਧਾਰਾ ) )
        ਜਾਓ Try_OpenType;

      FT_ਧਾਰਾ_Open_ਸਮ੍ਰਿਤੀ( ਧਾਰਾ, sfnt_ਅੰਕੜੇ, sfnt_ਮਾਪ );
      ਜੇ ( !open_face_PS_from_sfnt_ਧਾਰਾ( library,
                                           ਧਾਰਾ,
                                           face_ਸੂਚਕਾਂਕ,
                                           0, ਨਿਰਾ,
                                           aface ) )
      {
        FT_ਧਾਰਾ_Close( ਧਾਰਾ );
        FT_FREE( ਧਾਰਾ );
        FT_FREE( sfnt_ਅੰਕੜੇ );
        ਜਾਓ Exit;
      }

      FT_FREE( ਧਾਰਾ );
    }
  Try_OpenType:
    error = open_face_from_ਬਫਰ( library,
                                   sfnt_ਅੰਕੜੇ,
                                   sfnt_ਮਾਪ,
                                   face_ਸੂਚਕਾਂਕ,
                                   is_cff ? "cff" : "truetype",
                                   aface );
  Exit:
    ਵਾਪਸ error;
  }


  /* Create a new FT_Face from a file path to a suitcase file. */
  ਸਥਿਰ FT_Error
  FT_New_Face_From_Suitcase( FT_Library    library,
                             ਅਚਲ UInt8*  pathname,
                             FT_Long       face_ਸੂਚਕਾਂਕ,
                             FT_Face*      aface )
  {
    FT_Error       error = FT_ERR( Cannot_Open_Resource );
    Res_File_Ref_ਅੰਕ  res_ref;
    Resource_ਸੂਚਕਾਂਕ  res_ਸੂਚਕਾਂਕ;
    Handle         fond;
    ਛੋਟਾ          ਅੰਕ_faces_ਵਿੱਚ_res;
    FT_Long        ਗਿਣਤੀ_2;


    ਜੇ ( noErr != FT_FSPathMakeRes( pathname, &res_ref ) )
      ਵਾਪਸ FT_THROW( Cannot_Open_Resource );

    UseResFile( res_ref );
    ਜੇ ( ResError() )
      ਵਾਪਸ FT_THROW( Cannot_Open_Resource );

    res_ਸੂਚਕਾਂਕ        = 1;
    ਅੰਕ_faces_ਵਿੱਚ_res = 0;
    ਗਿਣਤੀ_2            = face_ਸੂਚਕਾਂਕ;
    ਜਦੋਂ ( ਗਿਣਤੀ_2 >= 0 )
    {
      ਛੋਟਾ  ਅੰਕ_faces_ਵਿੱਚ_fond;


      fond = Get1IndResource( TTAG_FOND, res_ਸੂਚਕਾਂਕ );
      ਜੇ ( ResError() )
        ਰੋਕੋ;

      ਅੰਕ_faces_ਵਿੱਚ_fond  = ਗਿਣਤੀ_faces( fond, pathname );
      ਅੰਕ_faces_ਵਿੱਚ_res  += ਅੰਕ_faces_ਵਿੱਚ_fond;

      ਜੇ ( ਗਿਣਤੀ_2 < ਅੰਕ_faces_ਵਿੱਚ_fond )
        error = FT_New_Face_From_FOND( library, fond, ਗਿਣਤੀ_2, aface );

      res_ਸੂਚਕਾਂਕ++;
      ਗਿਣਤੀ_2 -= ਅੰਕ_faces_ਵਿੱਚ_fond;
    }

    CloseResFile( res_ref );

    ਜੇ ( !error && aface && *aface )
    {
      (*aface)->ਅੰਕ_faces  = ਅੰਕ_faces_ਵਿੱਚ_res;
      (*aface)->face_ਸੂਚਕਾਂਕ = face_ਸੂਚਕਾਂਕ;
    }

    ਵਾਪਸ error;
  }


  /* documentation is in ftmac.h */

  FT_EXPORT_DEF( FT_Error )
  FT_New_Face_From_FOND( FT_Library  library,
                         Handle      fond,
                         FT_Long     face_ਸੂਚਕਾਂਕ,
                         FT_Face*    aface )
  {
    ਛੋਟਾ     have_sfnt, have_lwfn = 0;
    ResID     sfnt_id, fond_id;
    OSType    fond_type;
    Str255    fond_name;
    Str255    lwfn_file_name;
    UInt8     ਰਸਤਾ_lwfn[ਰਸਤਾ_MAX];
    OSErr     err;
    FT_Error  error = FT_Err_Ok;


    /* check of `library' and `aface' delayed to `FT_New_Face_From_XXX' */

    GetResInfo( fond, &fond_id, &fond_type, fond_name );
    ਜੇ ( ResError() != noErr || fond_type != TTAG_FOND )
      ਵਾਪਸ FT_THROW( Invalid_File_Format );

    parse_fond( *fond, &have_sfnt, &sfnt_id, lwfn_file_name, face_ਸੂਚਕਾਂਕ );

    ਜੇ ( lwfn_file_name[0] )
    {
      Res_File_Ref_ਅੰਕ  res;


      res = HomeResFile( fond );
      ਜੇ ( noErr != ResError() )
        ਜਾਓ ਮਿਲਿਆ_no_lwfn_file;

      {
        UInt8  ਰਸਤਾ_fond[ਰਸਤਾ_MAX];
        FSRef  ref;


        err = FSGetForkCBInfo( res, k_FSInvalid_Volume_Ref_ਅੰਕ,
                               ਨਿਰਾ, ਨਿਰਾ, ਨਿਰਾ, &ref, ਨਿਰਾ );
        ਜੇ ( noErr != err )
          ਜਾਓ ਮਿਲਿਆ_no_lwfn_file;

        err = FSRef_Make_ਰਸਤਾ( &ref, ਰਸਤਾ_fond, ਆਕਾਰ ( ਰਸਤਾ_fond ) );
        ਜੇ ( noErr != err )
          ਜਾਓ ਮਿਲਿਆ_no_lwfn_file;

        error = lookup_lwfn_by_fond( ਰਸਤਾ_fond, lwfn_file_name,
                                     ਰਸਤਾ_lwfn, ਆਕਾਰ ( ਰਸਤਾ_lwfn ) );
        ਜੇ ( !error )
          have_lwfn = 1;
      }
    }

    ਜੇ ( have_lwfn && ( !have_sfnt || PREFER_LWFN ) )
      error = FT_New_Face_From_LWFN( library,
                                     ਰਸਤਾ_lwfn,
                                     face_ਸੂਚਕਾਂਕ,
                                     aface );
    ਨਹੀਂਤਾਂ
      error = FT_THROW( Unknown_File_Format );

  ਮਿਲਿਆ_no_lwfn_file:
    ਜੇ ( have_sfnt && error )
      error = FT_New_Face_From_SFNT( library,
                                     sfnt_id,
                                     face_ਸੂਚਕਾਂਕ,
                                     aface );

    ਵਾਪਸ error;
  }


  /* Common function to load a new FT_Face from a resource file. */
  ਸਥਿਰ FT_Error
  FT_New_Face_From_Resource( FT_Library    library,
                             ਅਚਲ UInt8*  pathname,
                             FT_Long       face_ਸੂਚਕਾਂਕ,
                             FT_Face*      aface )
  {
    OSType    file_type;
    FT_Error  error;


    /* LWFN is a (very) specific file format, check for it explicitly */
    file_type = get_file_type_from_ਰਸਤਾ( pathname );
    ਜੇ ( file_type == TTAG_LWFN )
      ਵਾਪਸ FT_New_Face_From_LWFN( library, pathname, face_ਸੂਚਕਾਂਕ, aface );

    /* Otherwise the file type doesn't matter (there are more than  */
    /* `FFIL' and `tfil').  Just try opening it as a font suitcase; */
    /* if it works, fine.                                           */

    error = FT_New_Face_From_Suitcase( library, pathname, face_ਸੂਚਕਾਂਕ, aface );
    ਜੇ ( error )
    {
      /* let it fall through to normal loader (.ttf, .otf, etc.); */
      /* we signal this by returning no error and no FT_Face      */
      *aface = ਨਿਰਾ;
    }

    ਵਾਪਸ FT_Err_Ok;
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
               ਅਚਲ ਅੱਖਰ*  pathname,
               FT_Long      face_ਸੂਚਕਾਂਕ,
               FT_Face*     aface )
  {
    FT_Open_ਦਲੀਲਾਂ  ਦਲੀਲਾਂ;
    FT_Error      error;


    /* test for valid `library' and `aface' delayed to FT_Open_Face() */
    ਜੇ ( !pathname )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    *aface = ਨਿਰਾ;

    /* try resourcefork based font: LWFN, FFIL */
    error = FT_New_Face_From_Resource( library, (UInt8 *)pathname,
                                       face_ਸੂਚਕਾਂਕ, aface );
    ਜੇ ( error || *aface )
      ਵਾਪਸ error;

    /* let it fall through to normal loader (.ttf, .otf, etc.) */
    ਦਲੀਲਾਂ.flags    = FT_OPEN_PATHNAME;
    ਦਲੀਲਾਂ.pathname = (ਅੱਖਰ*)pathname;

    ਵਾਪਸ FT_Open_Face( library, &ਦਲੀਲਾਂ, face_ਸੂਚਕਾਂਕ, aface );
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
                          ਅਚਲ FSRef*  ref,
                          FT_Long       face_ਸੂਚਕਾਂਕ,
                          FT_Face*      aface )
  {
    FT_Error      error;
    FT_Open_ਦਲੀਲਾਂ  ਦਲੀਲਾਂ;

    OSErr  err;
    UInt8  pathname[ਰਸਤਾ_MAX];


    /* check of `library' and `aface' delayed to */
    /* `FT_New_Face_From_Resource'               */

    ਜੇ ( !ref )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    err = FSRef_Make_ਰਸਤਾ( ref, pathname, ਆਕਾਰ ( pathname ) );
    ਜੇ ( err )
      error = FT_THROW( Cannot_Open_Resource );

    error = FT_New_Face_From_Resource( library, pathname, face_ਸੂਚਕਾਂਕ, aface );
    ਜੇ ( error || *aface )
      ਵਾਪਸ error;

    /* fallback to datafork font */
    ਦਲੀਲਾਂ.flags    = FT_OPEN_PATHNAME;
    ਦਲੀਲਾਂ.pathname = (ਅੱਖਰ*)pathname;
    ਵਾਪਸ FT_Open_Face( library, &ਦਲੀਲਾਂ, face_ਸੂਚਕਾਂਕ, aface );
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
                           ਅਚਲ FSSpec*  spec,
                           FT_Long        face_ਸੂਚਕਾਂਕ,
                           FT_Face*       aface )
  {
#ਜੇ ( __LP64__ ) || ( defined( MAC_OS_X_ਸੰਸਕਰਣ_10_5 ) && \
      ( MAC_OS_X_ਸੰਸਕਰਣ_MIN_REQUIRED >= MAC_OS_X_ਸੰਸਕਰਣ_10_5 ) )
    FT_ਅਣਵਰਤਿਆ( library );
    FT_ਅਣਵਰਤਿਆ( spec );
    FT_ਅਣਵਰਤਿਆ( face_ਸੂਚਕਾਂਕ );
    FT_ਅਣਵਰਤਿਆ( aface );

    ਵਾਪਸ FT_THROW( Unimplemented_Feature );
#ਨਹੀਂਤਾਂ
    FSRef  ref;


    /* check of `library' and `aface' delayed to `FT_New_Face_From_FSRef' */

    ਜੇ ( !spec || FSpMakeFSRef( spec, &ref ) != noErr )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );
    ਨਹੀਂਤਾਂ
      ਵਾਪਸ FT_New_Face_From_FSRef( library, &ref, face_ਸੂਚਕਾਂਕ, aface );
#ਜੇ_ਅੰਤ
  }

#ਨਹੀਂਤਾਂ /* !FT_MACINTOSH */

  /* ANSI C doesn't like empty source files */
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਪੂਰਨ  ft_mac_dummy_;

#ਜੇ_ਅੰਤ /* !FT_MACINTOSH */


/*ਸਿਰਾ*/
