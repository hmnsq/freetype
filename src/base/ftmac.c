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


#ಅನ್ತರ್ಭಾವ <freetype/freetype.h>
#ಅನ್ತರ್ಭಾವ <freetype/tttags.h>
#ಅನ್ತರ್ಭಾವ <freetype/internal/ftdebug.h>
#ಅನ್ತರ್ಭಾವ <freetype/internal/ftstream.h>
#ಅನ್ತರ್ಭಾವ "ftbase.h"


#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ FT_MACINTOSH

  /* This is for Mac OS X.  Without redefinition, OS_INLINE */
  /* expands to `static inline' which doesn't survive the   */
  /* -ansi compilation flag of GCC.                         */
#ಆದರೆ !HAVE_ANSI_OS_INLINE
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು  OS_INLINE
#ವ್ಯಾಖ್ಯಾನಿಸು OS_INLINE  ಸ್ಥಿರ __inline__
#ಆದರೆ_ಅಂತ್ಯ

  /* `configure' checks the availability of `ResourceIndex' strictly */
  /* and sets HAVE_TYPE_RESOURCE_INDEX 1 or 0 always.  If it is      */
  /* not set (e.g., a build without `configure'), the availability   */
  /* is guessed from the SDK version.                                */
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_TYPE_RESOURCE_ಸೂಚ್ಯಂಕ
#ಆದರೆ !defined( MAC_OS_X_ಆವೃತ್ತಿ_10_5 ) || \
    ( MAC_OS_X_ಆವೃತ್ತಿ_MAX_ALLOWED < MAC_OS_X_ಆವೃತ್ತಿ_10_5 )
#ವ್ಯಾಖ್ಯಾನಿಸು HAVE_TYPE_RESOURCE_ಸೂಚ್ಯಂಕ 0
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು HAVE_TYPE_RESOURCE_ಸೂಚ್ಯಂಕ 1
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ /* !HAVE_TYPE_RESOURCE_INDEX */

#ಆದರೆ ( HAVE_TYPE_RESOURCE_ಸೂಚ್ಯಂಕ == 0 )
  ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಗಿಡ್ಡ  Resource_ಸೂಚ್ಯಂಕ;
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ <CoreServices/CoreServices.h>
#ಅನ್ತರ್ಭಾವ <ApplicationServices/ApplicationServices.h>
#ಅನ್ತರ್ಭಾವ <sys/syslimits.h> /* PATH_MAX */

  /* Don't want warnings about our own use of deprecated functions. */
#ವ್ಯಾಖ್ಯಾನಿಸು FT_DEPRECATED_ATTRIBUTE

#ಅನ್ತರ್ಭಾವ <freetype/ftmac.h>

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ kATSOptionFlagsUnRestrictedScope /* since Mac OS X 10.1 */
#ವ್ಯಾಖ್ಯಾನಿಸು kATSOptionFlagsUnRestrictedScope kATSOptionFlagsDefault
#ಆದರೆ_ಅಂತ್ಯ


  /* Set PREFER_LWFN to 1 if LWFN (Type 1) is preferred over
     TrueType in case *both* are available (this is not common,
     but it *is* possible). */
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ PREFER_LWFN
#ವ್ಯಾಖ್ಯಾನಿಸು PREFER_LWFN  1
#ಆದರೆ_ಅಂತ್ಯ


  /* This function is deprecated because FSSpec is deprecated in Mac OS X  */
  FT_EXPORT_DEF( FT_Error )
  FT_GetFile_From_Mac_Name( ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*  fontName,
                            FSSpec*      ಮಾರ್ಗ_Spec,
                            FT_Long*     face_ಸೂಚ್ಯಂಕ )
  {
    FT_ಬಳಸದ( fontName );
    FT_ಬಳಸದ( ಮಾರ್ಗ_Spec );
    FT_ಬಳಸದ( face_ಸೂಚ್ಯಂಕ );

    ಹಿಂತಿರುಗಿ FT_THROW( Unimplemented_Feature );
  }


  /* Private function.                                         */
  /* The FSSpec type has been discouraged for a long time,     */
  /* unfortunately an FSRef replacement API for                */
  /* ATSFontGetFileSpecification() is only available in        */
  /* Mac OS X 10.5 and later.                                  */
  ಸ್ಥಿರ OSStatus
  FT_ATSFontGetFileReference( ATSFontRef  ats_font_id,
                              FSRef*      ats_font_ref )
  {
#ಆದರೆ defined( MAC_OS_X_ಆವೃತ್ತಿ_10_5 ) && \
    ( MAC_OS_X_ಆವೃತ್ತಿ_MIN_REQUIRED >= MAC_OS_X_ಆವೃತ್ತಿ_10_5 )

    OSStatus  err;

    err = ATSFontGetFileReference( ats_font_id, ats_font_ref );

    ಹಿಂತಿರುಗಿ err;
#ಅಥವಾಆದರೆ __LP64__ /* No 64bit Carbon API on legacy platforms */
    FT_ಬಳಸದ( ats_font_id );
    FT_ಬಳಸದ( ats_font_ref );


    ಹಿಂತಿರುಗಿ fnfErr;
#ಇಲ್ಲದಿದ್ದರೆ /* 32bit Carbon API on legacy platforms */
    OSStatus  err;
    FSSpec    spec;


    err = ATSFontGetFileSpecification( ats_font_id, &spec );
    ಆದರೆ ( noErr == err )
      err = FSpMakeFSRef( &spec, ats_font_ref );

    ಹಿಂತಿರುಗಿ err;
#ಆದರೆ_ಅಂತ್ಯ
  }


  ಸ್ಥಿರ FT_Error
  FT_GetFileRef_From_Mac_ATS_Name( ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*  fontName,
                                   FSRef*       ats_font_ref,
                                   FT_Long*     face_ಸೂಚ್ಯಂಕ )
  {
    CFStringRef  cf_fontName;
    ATSFontRef   ats_font_id;


    *face_ಸೂಚ್ಯಂಕ = 0;

    cf_fontName = CFStringCreateWithCString( ಖಾಲಿ, fontName,
                                             kCFStringEncodingMacRoman );
    ats_font_id = ATSFont_ಕಂಡುಹಿಡಿ_From_Name( cf_fontName,
                                       kATSOptionFlagsUnRestrictedScope );
    CFRelease( cf_fontName );

    ಆದರೆ ( ats_font_id == 0 || ats_font_id == 0xFFFFFFFFUL )
      ಹಿಂತಿರುಗಿ FT_THROW( Unknown_File_Format );

    ಆದರೆ ( noErr != FT_ATSFontGetFileReference( ats_font_id, ats_font_ref ) )
      ಹಿಂತಿರುಗಿ FT_THROW( Unknown_File_Format );

    /* face_index calculation by searching preceding fontIDs */
    /* with same FSRef                                       */
    {
      ATSFontRef  id2 = ats_font_id - 1;
      FSRef       ref2;


      ಆಗ ( id2 > 0 )
      {
        ಆದರೆ ( noErr != FT_ATSFontGetFileReference( id2, &ref2 ) )
          ನಿಲ್ಲಿಸು;
        ಆದರೆ ( noErr != FSCompareFSRefs( ats_font_ref, &ref2 ) )
          ನಿಲ್ಲಿಸು;

        id2 --;
      }
      *face_ಸೂಚ್ಯಂಕ = ats_font_id - ( id2 + 1 );
    }

    ಹಿಂತಿರುಗಿ FT_Err_Ok;
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Get_File_ಮಾರ್ಗ_From_Mac_ATS_Name( ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*  fontName,
                                    UInt8*       ಮಾರ್ಗ,
                                    UInt32       max_ಮಾರ್ಗ_ಗಾತ್ರ,
                                    FT_Long*     face_ಸೂಚ್ಯಂಕ )
  {
    FSRef     ref;
    FT_Error  err;


    ಆದರೆ ( !fontName || !face_ಸೂಚ್ಯಂಕ )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ);

    err = FT_GetFileRef_From_Mac_ATS_Name( fontName, &ref, face_ಸೂಚ್ಯಂಕ );
    ಆದರೆ ( err )
      ಹಿಂತಿರುಗಿ err;

    ಆದರೆ ( noErr != FSRef_Make_ಮಾರ್ಗ( &ref, ಮಾರ್ಗ, max_ಮಾರ್ಗ_ಗಾತ್ರ ) )
      ಹಿಂತಿರುಗಿ FT_THROW( Unknown_File_Format );

    ಹಿಂತಿರುಗಿ FT_Err_Ok;
  }


  /* This function is deprecated because FSSpec is deprecated in Mac OS X  */
  FT_EXPORT_DEF( FT_Error )
  FT_GetFile_From_Mac_ATS_Name( ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*  fontName,
                                FSSpec*      ಮಾರ್ಗ_Spec,
                                FT_Long*     face_ಸೂಚ್ಯಂಕ )
  {
#ಆದರೆ ( __LP64__ ) || ( defined( MAC_OS_X_ಆವೃತ್ತಿ_10_5 ) && \
      ( MAC_OS_X_ಆವೃತ್ತಿ_MIN_REQUIRED >= MAC_OS_X_ಆವೃತ್ತಿ_10_5 ) )
    FT_ಬಳಸದ( fontName );
    FT_ಬಳಸದ( ಮಾರ್ಗ_Spec );
    FT_ಬಳಸದ( face_ಸೂಚ್ಯಂಕ );

    ಹಿಂತಿರುಗಿ FT_THROW( Unimplemented_Feature );
#ಇಲ್ಲದಿದ್ದರೆ
    FSRef     ref;
    FT_Error  err;


    ಆದರೆ ( !fontName || !face_ಸೂಚ್ಯಂಕ )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

    err = FT_GetFileRef_From_Mac_ATS_Name( fontName, &ref, face_ಸೂಚ್ಯಂಕ );
    ಆದರೆ ( err )
      ಹಿಂತಿರುಗಿ err;

    ಆದರೆ ( noErr != FSGetCatalogInfo( &ref, kFSCatInfoNone, ಖಾಲಿ, ಖಾಲಿ,
                                    ಮಾರ್ಗ_Spec, ಖಾಲಿ ) )
      ಹಿಂತಿರುಗಿ FT_THROW( Unknown_File_Format );

    ಹಿಂತಿರುಗಿ FT_Err_Ok;
#ಆದರೆ_ಅಂತ್ಯ
  }


  ಸ್ಥಿರ OSErr
  FT_FSPathMakeRes( ಸ್ಥಿರಾಂಕ UInt8*    pathname,
                    Res_File_Ref_ಅಂಕೆ*  res )
  {
    OSErr  err;
    FSRef  ref;


    ಆದರೆ ( noErr != FSPathMakeRef( pathname, &ref, FALSE ) )
      ಹಿಂತಿರುಗಿ FT_THROW( Cannot_Open_Resource );

    /* at present, no support for dfont format */
    err = FSOpenResourceFile( &ref, 0, ಖಾಲಿ, fsRdPerm, res );
    ಆದರೆ ( noErr == err )
      ಹಿಂತಿರುಗಿ err;

    /* fallback to original resource-fork font */
    *res = FSOpenResFile( &ref, fsRdPerm );
    err  = ResError();

    ಹಿಂತಿರುಗಿ err;
  }


  /* Return the file type for given pathname */
  ಸ್ಥಿರ OSType
  get_file_type_from_ಮಾರ್ಗ( ಸ್ಥಿರಾಂಕ UInt8*  pathname )
  {
    FSRef          ref;
    FSCatalogInfo  info;


    ಆದರೆ ( noErr != FSPathMakeRef( pathname, &ref, FALSE ) )
      ಹಿಂತಿರುಗಿ ( OSType ) 0;

    ಆದರೆ ( noErr != FSGetCatalogInfo( &ref, kFSCatInfoFinderInfo, &info,
                                    ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ ) )
      ಹಿಂತಿರುಗಿ ( OSType ) 0;

    ಹಿಂತಿರುಗಿ ( (FInfo *)( info.finderInfo ) )->fdType;
  }


  /* Given a PostScript font name, create the Macintosh LWFN file name. */
  ಸ್ಥಿರ ಶೂನ್ಯ
  create_lwfn_name( ಅಕ್ಷರ*   ps_name,
                    Str255  lwfn_file_name )
  {
    ಪೂರ್ಣಾಂಕ       max = 5, ಎಣಿಕೆ_2 = 0;
    FT_Byte*  p = lwfn_file_name;
    FT_Byte*  q = (FT_Byte*)ps_name;


    lwfn_file_name[0] = 0;

    ಆಗ ( *q )
    {
      ಆದರೆ ( ft_isupper( *q ) )
      {
        ಆದರೆ ( ಎಣಿಕೆ_2 )
          max = 3;
        ಎಣಿಕೆ_2 = 0;
      }
      ಆದರೆ ( ಎಣಿಕೆ_2 < max && ( ft_isalnum( *q ) || *q == '_' ) )
      {
        *++p = *q;
        lwfn_file_name[0]++;
        ಎಣಿಕೆ_2++;
      }
      q++;
    }
  }


  ಸ್ಥಿರ ಗಿಡ್ಡ
  ಎಣಿಕೆ_faces_sfnt( ಅಕ್ಷರ*  fond_ದತ್ತಾಂಶ )
  {
    /* The count is 1 greater than the value in the FOND.  */
    /* Isn't that cute? :-)                                */

    ಹಿಂತಿರುಗಿ EndianS16_BtoN( *( (ಗಿಡ್ಡ*)( fond_ದತ್ತಾಂಶ +
                                        ಗಾತ್ರ ( FamRec ) ) ) ) + 1;
  }


  ಸ್ಥಿರ ಗಿಡ್ಡ
  ಎಣಿಕೆ_faces_scalable( ಅಕ್ಷರ*  fond_ದತ್ತಾಂಶ )
  {
    Assc_ನಮೂದು*  assoc;
    ಗಿಡ್ಡ       i, face, face_all;


    face_all = EndianS16_BtoN( *( (ಗಿಡ್ಡ *)( fond_ದತ್ತಾಂಶ +
                                             ಗಾತ್ರ ( FamRec ) ) ) ) + 1;
    assoc    = (Assc_ನಮೂದು*)( fond_ದತ್ತಾಂಶ + ಗಾತ್ರ ( FamRec ) + 2 );
    face     = 0;

    ಗಾಗಿ ( i = 0; i < face_all; i++ )
    {
      ಆದರೆ ( 0 == EndianS16_BtoN( assoc[i].font_ಗಾತ್ರ ) )
        face++;
    }
    ಹಿಂತಿರುಗಿ face;
  }


  /* Look inside the FOND data, answer whether there should be an SFNT
     resource, and answer the name of a possible LWFN Type 1 file.

     Thanks to Paul Miller (paulm@profoundeffects.com) for the fix
     to load a face OTHER than the first one in the FOND!
  */


  ಸ್ಥಿರ ಶೂನ್ಯ
  parse_fond( ಅಕ್ಷರ*   fond_ದತ್ತಾಂಶ,
              ಗಿಡ್ಡ*  have_sfnt,
              ResID*  sfnt_id,
              Str255  lwfn_file_name,
              ಗಿಡ್ಡ   face_ಸೂಚ್ಯಂಕ )
  {
    Assc_ನಮೂದು*  assoc;
    Assc_ನಮೂದು*  ತಳ_assoc;
    FamRec*     fond;


    *sfnt_id          = 0;
    *have_sfnt        = 0;
    lwfn_file_name[0] = 0;

    fond       = (FamRec*)fond_ದತ್ತಾಂಶ;
    assoc      = (Assc_ನಮೂದು*)( fond_ದತ್ತಾಂಶ + ಗಾತ್ರ ( FamRec ) + 2 );
    ತಳ_assoc = assoc;

    /* the maximum faces in a FOND is 48, size of StyleTable.indexes[] */
    ಆದರೆ ( 47 < face_ಸೂಚ್ಯಂಕ )
      ಹಿಂತಿರುಗಿ;

    /* Let's do a little range checking before we get too excited here */
    ಆದರೆ ( face_ಸೂಚ್ಯಂಕ < ಎಣಿಕೆ_faces_sfnt( fond_ದತ್ತಾಂಶ ) )
    {
      assoc += face_ಸೂಚ್ಯಂಕ;        /* add on the face_index! */

      /* if the face at this index is not scalable,
         fall back to the first one (old behavior) */
      ಆದರೆ ( EndianS16_BtoN( assoc->font_ಗಾತ್ರ ) == 0 )
      {
        *have_sfnt = 1;
        *sfnt_id   = EndianS16_BtoN( assoc->fontID );
      }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ( ತಳ_assoc->font_ಗಾತ್ರ == 0 )
      {
        *have_sfnt = 1;
        *sfnt_id   = EndianS16_BtoN( ತಳ_assoc->fontID );
      }
    }

    ಆದರೆ ( EndianS32_BtoN( fond->ffStylOff ) )
    {
      ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ*  p = (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ*)fond_ದತ್ತಾಂಶ;
      Style_ಕೋಷ್ಟಕ*     style;
      ಚಿಹ್ನೆರಹಿತ ಗಿಡ್ಡ  string_ಎಣಿಕೆ;
      ಅಕ್ಷರ            ps_name[256];
      ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ*  ಹೆಸರುಗಳು[64];
      ಪೂರ್ಣಾಂಕ             i;


      p += EndianS32_BtoN( fond->ffStylOff );
      style = (Style_ಕೋಷ್ಟಕ*)p;
      p += ಗಾತ್ರ ( Style_ಕೋಷ್ಟಕ );
      string_ಎಣಿಕೆ = EndianS16_BtoN( *(ಗಿಡ್ಡ*)(p) );
      string_ಎಣಿಕೆ = FT_MIN( 64, string_ಎಣಿಕೆ );
      p += ಗಾತ್ರ ( ಗಿಡ್ಡ );

      ಗಾಗಿ ( i = 0; i < string_ಎಣಿಕೆ; i++ )
      {
        ಹೆಸರುಗಳು[i] = p;
        p       += ಹೆಸರುಗಳು[i][0];
        p++;
      }

      {
        size_t  ps_name_len = (size_t)ಹೆಸರುಗಳು[0][0];


        ಆದರೆ ( ps_name_len != 0 )
        {
          ft_memcpy( ps_name, ಹೆಸರುಗಳು[0] + 1, ps_name_len );
          ps_name[ps_name_len] = 0;
        }
        ಆದರೆ ( style->indexes[face_ಸೂಚ್ಯಂಕ] > 1 &&
             style->indexes[face_ಸೂಚ್ಯಂಕ] <= string_ಎಣಿಕೆ )
        {
          ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ*  suffixes = ಹೆಸರುಗಳು[style->indexes[face_ಸೂಚ್ಯಂಕ] - 1];


          ಗಾಗಿ ( i = 1; i <= suffixes[0]; i++ )
          {
            ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ*  s;
            size_t          j = suffixes[i] - 1;


            ಆದರೆ ( j < string_ಎಣಿಕೆ && ( s = ಹೆಸರುಗಳು[j] ) != ಖಾಲಿ )
            {
              size_t  s_len = (size_t)s[0];


              ಆದರೆ ( s_len != 0 && ps_name_len + s_len < ಗಾತ್ರ ( ps_name ) )
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


  ಸ್ಥಿರ  FT_Error
  lookup_lwfn_by_fond( ಸ್ಥಿರಾಂಕ UInt8*      ಮಾರ್ಗ_fond,
                       ConstStr255Param  ತಳ_lwfn,
                       UInt8*            ಮಾರ್ಗ_lwfn,
                       size_t            ಮಾರ್ಗ_ಗಾತ್ರ )
  {
    FSRef   ref, par_ref;
    size_t  dirname_len;


    /* Pathname for FSRef can be in various formats: HFS, HFS+, and POSIX. */
    /* We should not extract parent directory by string manipulation.      */

    ಆದರೆ ( noErr != FSPathMakeRef( ಮಾರ್ಗ_fond, &ref, FALSE ) )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

    ಆದರೆ ( noErr != FSGetCatalogInfo( &ref, kFSCatInfoNone,
                                    ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ, &par_ref ) )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

    ಆದರೆ ( noErr != FSRef_Make_ಮಾರ್ಗ( &par_ref, ಮಾರ್ಗ_lwfn, ಮಾರ್ಗ_ಗಾತ್ರ ) )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

    ಆದರೆ ( ft_strlen( (ಅಕ್ಷರ *)ಮಾರ್ಗ_lwfn ) + 1 + ತಳ_lwfn[0] > ಮಾರ್ಗ_ಗಾತ್ರ )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

    /* now we have absolute dirname in path_lwfn */
    ft_strcat( (ಅಕ್ಷರ *)ಮಾರ್ಗ_lwfn, "/" );
    dirname_len = ft_strlen( (ಅಕ್ಷರ *)ಮಾರ್ಗ_lwfn );
    ft_strcat( (ಅಕ್ಷರ *)ಮಾರ್ಗ_lwfn, (ಅಕ್ಷರ *)ತಳ_lwfn + 1 );
    ಮಾರ್ಗ_lwfn[dirname_len + ತಳ_lwfn[0]] = '\0';

    ಆದರೆ ( noErr != FSPathMakeRef( ಮಾರ್ಗ_lwfn, &ref, FALSE ) )
      ಹಿಂತಿರುಗಿ FT_THROW( Cannot_Open_Resource );

    ಆದರೆ ( noErr != FSGetCatalogInfo( &ref, kFSCatInfoNone,
                                    ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ ) )
      ಹಿಂತಿರುಗಿ FT_THROW( Cannot_Open_Resource );

    ಹಿಂತಿರುಗಿ FT_Err_Ok;
  }


  ಸ್ಥಿರ ಗಿಡ್ಡ
  ಎಣಿಕೆ_faces( Handle        fond,
               ಸ್ಥಿರಾಂಕ UInt8*  pathname )
  {
    ResID     sfnt_id;
    ಗಿಡ್ಡ     have_sfnt, have_lwfn;
    Str255    lwfn_file_name;
    UInt8     buff[ಮಾರ್ಗ_MAX];
    FT_Error  err;
    ಗಿಡ್ಡ     ಅಂಕೆ_faces;


    have_sfnt = have_lwfn = 0;

    parse_fond( *fond, &have_sfnt, &sfnt_id, lwfn_file_name, 0 );

    ಆದರೆ ( lwfn_file_name[0] )
    {
      err = lookup_lwfn_by_fond( pathname, lwfn_file_name,
                                 buff, ಗಾತ್ರ ( buff ) );
      ಆದರೆ ( !err )
        have_lwfn = 1;
    }

    ಆದರೆ ( have_lwfn && ( !have_sfnt || PREFER_LWFN ) )
      ಅಂಕೆ_faces = 1;
    ಇಲ್ಲದಿದ್ದರೆ
      ಅಂಕೆ_faces = ಎಣಿಕೆ_faces_scalable( *fond );

    ಹಿಂತಿರುಗಿ ಅಂಕೆ_faces;
  }


  /* Read Type 1 data from the POST resources inside the LWFN file,
     return a PFB buffer.  This is somewhat convoluted because the FT2
     PFB parser wants the ASCII header as one chunk, and the LWFN
     chunks are often not organized that way, so we glue chunks
     of the same type together. */
  ಸ್ಥಿರ FT_Error
  read_lwfn( FT_ಸ್ಮರಣೆ      ಸ್ಮರಣೆ,
             Res_File_Ref_ಅಂಕೆ  res,
             FT_Byte**      pfb_ದತ್ತಾಂಶ,
             FT_ULong*      ಗಾತ್ರ_2 )
  {
    FT_Error       error = FT_Err_Ok;
    ResID          res_id;
    ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ  *ಬಫರ್, *p, *ಗಾತ್ರ_p = ಖಾಲಿ;
    FT_ULong       ಒಟ್ಟು_ಗಾತ್ರ = 0;
    FT_ULong       old_ಒಟ್ಟು_ಗಾತ್ರ = 0;
    FT_ULong       post_ಗಾತ್ರ, pfb_chunk_ಗಾತ್ರ;
    Handle         post_ದತ್ತಾಂಶ;
    ಅಕ್ಷರ           ಸಂಕೇತ_2, ಕೊನೆಯ_ಸಂಕೇತ;


    UseResFile( res );

    /* First pass: load all POST resources, and determine the size of */
    /* the output buffer.                                             */
    res_id    = 501;
    ಕೊನೆಯ_ಸಂಕೇತ = -1;

    ಗಾಗಿ (;;)
    {
      post_ದತ್ತಾಂಶ = Get1Resource( TTAG_POST, res_id++ );
      ಆದರೆ ( !post_ದತ್ತಾಂಶ )
        ನಿಲ್ಲಿಸು;  /* we are done */

      ಸಂಕೇತ_2 = (*post_ದತ್ತಾಂಶ)[0];

      ಆದರೆ ( ಸಂಕೇತ_2 != ಕೊನೆಯ_ಸಂಕೇತ )
      {
        ಆದರೆ ( ಸಂಕೇತ_2 == 5 )
          ಒಟ್ಟು_ಗಾತ್ರ += 2; /* just the end code */
        ಇಲ್ಲದಿದ್ದರೆ
          ಒಟ್ಟು_ಗಾತ್ರ += 6; /* code + 4 bytes chunk length */
      }

      ಒಟ್ಟು_ಗಾತ್ರ += (FT_ULong)Get_Handle_ಗಾತ್ರ( post_ದತ್ತಾಂಶ ) - 2;
      ಕೊನೆಯ_ಸಂಕೇತ = ಸಂಕೇತ_2;

      /* detect resource fork overflow */
      ಆದರೆ ( FT_MAC_RFORK_MAX_LEN < ಒಟ್ಟು_ಗಾತ್ರ )
      {
        error = FT_THROW( Array_Too_Large );
        ಹೋಗು Error;
      }

      old_ಒಟ್ಟು_ಗಾತ್ರ = ಒಟ್ಟು_ಗಾತ್ರ;
    }

    ಆದರೆ ( FT_QALLOC( ಬಫರ್, (FT_Long)ಒಟ್ಟು_ಗಾತ್ರ ) )
      ಹೋಗು Error;

    /* Second pass: append all POST data to the buffer, add PFB fields. */
    /* Glue all consecutive chunks of the same type together.           */
    p              = ಬಫರ್;
    res_id         = 501;
    ಕೊನೆಯ_ಸಂಕೇತ      = -1;
    pfb_chunk_ಗಾತ್ರ = 0;

    ಗಾಗಿ (;;)
    {
      post_ದತ್ತಾಂಶ = Get1Resource( TTAG_POST, res_id++ );
      ಆದರೆ ( !post_ದತ್ತಾಂಶ )
        ನಿಲ್ಲಿಸು;  /* we are done */

      post_ಗಾತ್ರ = (FT_ULong)Get_Handle_ಗಾತ್ರ( post_ದತ್ತಾಂಶ ) - 2;
      ಸಂಕೇತ_2 = (*post_ದತ್ತಾಂಶ)[0];

      ಆದರೆ ( ಸಂಕೇತ_2 != ಕೊನೆಯ_ಸಂಕೇತ )
      {
        ಆದರೆ ( ಕೊನೆಯ_ಸಂಕೇತ != -1 )
        {
          /* we are done adding a chunk, fill in the size field */
          ಆದರೆ ( ಗಾತ್ರ_p )
          {
            *ಗಾತ್ರ_p++ = (FT_Byte)(   pfb_chunk_ಗಾತ್ರ         & 0xFF );
            *ಗಾತ್ರ_p++ = (FT_Byte)( ( pfb_chunk_ಗಾತ್ರ >> 8  ) & 0xFF );
            *ಗಾತ್ರ_p++ = (FT_Byte)( ( pfb_chunk_ಗಾತ್ರ >> 16 ) & 0xFF );
            *ಗಾತ್ರ_p++ = (FT_Byte)( ( pfb_chunk_ಗಾತ್ರ >> 24 ) & 0xFF );
          }
          pfb_chunk_ಗಾತ್ರ = 0;
        }

        *p++ = 0x80;
        ಆದರೆ ( ಸಂಕೇತ_2 == 5 )
          *p++ = 0x03;  /* the end */
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ( ಸಂಕೇತ_2 == 2 )
          *p++ = 0x02;  /* binary segment */
        ಇಲ್ಲದಿದ್ದರೆ
          *p++ = 0x01;  /* ASCII segment */

        ಆದರೆ ( ಸಂಕೇತ_2 != 5 )
        {
          ಗಾತ್ರ_p = p;   /* save for later */
          p += 4;       /* make space for size field */
        }
      }

      ft_memcpy( p, *post_ದತ್ತಾಂಶ + 2, post_ಗಾತ್ರ );
      pfb_chunk_ಗಾತ್ರ += post_ಗಾತ್ರ;
      p += post_ಗಾತ್ರ;
      ಕೊನೆಯ_ಸಂಕೇತ = ಸಂಕೇತ_2;
    }

    *pfb_ದತ್ತಾಂಶ = ಬಫರ್;
    *ಗಾತ್ರ_2 = ಒಟ್ಟು_ಗಾತ್ರ;

  Error:
    CloseResFile( res );
    ಹಿಂತಿರುಗಿ error;
  }


  /* Create a new FT_Face from a file path to an LWFN file. */
  ಸ್ಥಿರ FT_Error
  FT_New_Face_From_LWFN( FT_Library    library,
                         ಸ್ಥಿರಾಂಕ UInt8*  pathname,
                         FT_Long       face_ಸೂಚ್ಯಂಕ,
                         FT_Face*      aface )
  {
    FT_Byte*       pfb_ದತ್ತಾಂಶ;
    FT_ULong       pfb_ಗಾತ್ರ;
    FT_Error       error;
    Res_File_Ref_ಅಂಕೆ  res;


    ಆದರೆ ( noErr != FT_FSPathMakeRes( pathname, &res ) )
      ಹಿಂತಿರುಗಿ FT_THROW( Cannot_Open_Resource );

    pfb_ದತ್ತಾಂಶ = ಖಾಲಿ;
    pfb_ಗಾತ್ರ = 0;
    error = read_lwfn( library->ಸ್ಮರಣೆ, res, &pfb_ದತ್ತಾಂಶ, &pfb_ಗಾತ್ರ );
    CloseResFile( res ); /* PFB is already loaded, useless anymore */
    ಆದರೆ ( error )
      ಹಿಂತಿರುಗಿ error;

    ಹಿಂತಿರುಗಿ open_face_from_ಬಫರ್( library,
                                  pfb_ದತ್ತಾಂಶ,
                                  pfb_ಗಾತ್ರ,
                                  face_ಸೂಚ್ಯಂಕ,
                                  "type1",
                                  aface );
  }


  /* Create a new FT_Face from an SFNT resource, specified by res ID. */
  ಸ್ಥಿರ FT_Error
  FT_New_Face_From_SFNT( FT_Library  library,
                         ResID       sfnt_id,
                         FT_Long     face_ಸೂಚ್ಯಂಕ,
                         FT_Face*    aface )
  {
    Handle     sfnt = ಖಾಲಿ;
    FT_Byte*   sfnt_ದತ್ತಾಂಶ;
    size_t     sfnt_ಗಾತ್ರ;
    FT_Error   error  = FT_Err_Ok;
    FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ = library->ಸ್ಮರಣೆ;
    ಪೂರ್ಣಾಂಕ        is_cff, is_sfnt_ps;


    sfnt = GetResource( TTAG_sfnt, sfnt_id );
    ಆದರೆ ( !sfnt )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_Handle );

    sfnt_ಗಾತ್ರ = (FT_ULong)Get_Handle_ಗಾತ್ರ( sfnt );

    /* detect resource fork overflow */
    ಆದರೆ ( FT_MAC_RFORK_MAX_LEN < sfnt_ಗಾತ್ರ )
      ಹಿಂತಿರುಗಿ FT_THROW( Array_Too_Large );

    ಆದರೆ ( FT_QALLOC( sfnt_ದತ್ತಾಂಶ, (FT_Long)sfnt_ಗಾತ್ರ ) )
    {
      ReleaseResource( sfnt );
      ಹಿಂತಿರುಗಿ error;
    }

    ft_memcpy( sfnt_ದತ್ತಾಂಶ, *sfnt, sfnt_ಗಾತ್ರ );
    ReleaseResource( sfnt );

    is_cff     = sfnt_ಗಾತ್ರ > 4 && !ft_memcmp( sfnt_ದತ್ತಾಂಶ, "OTTO", 4 );
    is_sfnt_ps = sfnt_ಗಾತ್ರ > 4 && !ft_memcmp( sfnt_ದತ್ತಾಂಶ, "typ1", 4 );

    ಆದರೆ ( is_sfnt_ps )
    {
      FT_ಹರಿವು  ಹರಿವು;


      ಆದರೆ ( FT_NEW( ಹರಿವು ) )
        ಹೋಗು Try_OpenType;

      FT_ಹರಿವು_Open_ಸ್ಮರಣೆ( ಹರಿವು, sfnt_ದತ್ತಾಂಶ, sfnt_ಗಾತ್ರ );
      ಆದರೆ ( !open_face_PS_from_sfnt_ಹರಿವು( library,
                                           ಹರಿವು,
                                           face_ಸೂಚ್ಯಂಕ,
                                           0, ಖಾಲಿ,
                                           aface ) )
      {
        FT_ಹರಿವು_Close( ಹರಿವು );
        FT_FREE( ಹರಿವು );
        FT_FREE( sfnt_ದತ್ತಾಂಶ );
        ಹೋಗು Exit;
      }

      FT_FREE( ಹರಿವು );
    }
  Try_OpenType:
    error = open_face_from_ಬಫರ್( library,
                                   sfnt_ದತ್ತಾಂಶ,
                                   sfnt_ಗಾತ್ರ,
                                   face_ಸೂಚ್ಯಂಕ,
                                   is_cff ? "cff" : "truetype",
                                   aface );
  Exit:
    ಹಿಂತಿರುಗಿ error;
  }


  /* Create a new FT_Face from a file path to a suitcase file. */
  ಸ್ಥಿರ FT_Error
  FT_New_Face_From_Suitcase( FT_Library    library,
                             ಸ್ಥಿರಾಂಕ UInt8*  pathname,
                             FT_Long       face_ಸೂಚ್ಯಂಕ,
                             FT_Face*      aface )
  {
    FT_Error       error = FT_ERR( Cannot_Open_Resource );
    Res_File_Ref_ಅಂಕೆ  res_ref;
    Resource_ಸೂಚ್ಯಂಕ  res_ಸೂಚ್ಯಂಕ;
    Handle         fond;
    ಗಿಡ್ಡ          ಅಂಕೆ_faces_ಒಳ_res;
    FT_Long        ಎಣಿಕೆ_2;


    ಆದರೆ ( noErr != FT_FSPathMakeRes( pathname, &res_ref ) )
      ಹಿಂತಿರುಗಿ FT_THROW( Cannot_Open_Resource );

    UseResFile( res_ref );
    ಆದರೆ ( ResError() )
      ಹಿಂತಿರುಗಿ FT_THROW( Cannot_Open_Resource );

    res_ಸೂಚ್ಯಂಕ        = 1;
    ಅಂಕೆ_faces_ಒಳ_res = 0;
    ಎಣಿಕೆ_2            = face_ಸೂಚ್ಯಂಕ;
    ಆಗ ( ಎಣಿಕೆ_2 >= 0 )
    {
      ಗಿಡ್ಡ  ಅಂಕೆ_faces_ಒಳ_fond;


      fond = Get1IndResource( TTAG_FOND, res_ಸೂಚ್ಯಂಕ );
      ಆದರೆ ( ResError() )
        ನಿಲ್ಲಿಸು;

      ಅಂಕೆ_faces_ಒಳ_fond  = ಎಣಿಕೆ_faces( fond, pathname );
      ಅಂಕೆ_faces_ಒಳ_res  += ಅಂಕೆ_faces_ಒಳ_fond;

      ಆದರೆ ( ಎಣಿಕೆ_2 < ಅಂಕೆ_faces_ಒಳ_fond )
        error = FT_New_Face_From_FOND( library, fond, ಎಣಿಕೆ_2, aface );

      res_ಸೂಚ್ಯಂಕ++;
      ಎಣಿಕೆ_2 -= ಅಂಕೆ_faces_ಒಳ_fond;
    }

    CloseResFile( res_ref );

    ಆದರೆ ( !error && aface && *aface )
    {
      (*aface)->ಅಂಕೆ_faces  = ಅಂಕೆ_faces_ಒಳ_res;
      (*aface)->face_ಸೂಚ್ಯಂಕ = face_ಸೂಚ್ಯಂಕ;
    }

    ಹಿಂತಿರುಗಿ error;
  }


  /* documentation is in ftmac.h */

  FT_EXPORT_DEF( FT_Error )
  FT_New_Face_From_FOND( FT_Library  library,
                         Handle      fond,
                         FT_Long     face_ಸೂಚ್ಯಂಕ,
                         FT_Face*    aface )
  {
    ಗಿಡ್ಡ     have_sfnt, have_lwfn = 0;
    ResID     sfnt_id, fond_id;
    OSType    fond_type;
    Str255    fond_name;
    Str255    lwfn_file_name;
    UInt8     ಮಾರ್ಗ_lwfn[ಮಾರ್ಗ_MAX];
    OSErr     err;
    FT_Error  error = FT_Err_Ok;


    /* check of `library' and `aface' delayed to `FT_New_Face_From_XXX' */

    GetResInfo( fond, &fond_id, &fond_type, fond_name );
    ಆದರೆ ( ResError() != noErr || fond_type != TTAG_FOND )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_File_Format );

    parse_fond( *fond, &have_sfnt, &sfnt_id, lwfn_file_name, face_ಸೂಚ್ಯಂಕ );

    ಆದರೆ ( lwfn_file_name[0] )
    {
      Res_File_Ref_ಅಂಕೆ  res;


      res = HomeResFile( fond );
      ಆದರೆ ( noErr != ResError() )
        ಹೋಗು ಸಿಕ್ಕಿತು_no_lwfn_file;

      {
        UInt8  ಮಾರ್ಗ_fond[ಮಾರ್ಗ_MAX];
        FSRef  ref;


        err = FSGetForkCBInfo( res, k_FSInvalid_Volume_Ref_ಅಂಕೆ,
                               ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ, &ref, ಖಾಲಿ );
        ಆದರೆ ( noErr != err )
          ಹೋಗು ಸಿಕ್ಕಿತು_no_lwfn_file;

        err = FSRef_Make_ಮಾರ್ಗ( &ref, ಮಾರ್ಗ_fond, ಗಾತ್ರ ( ಮಾರ್ಗ_fond ) );
        ಆದರೆ ( noErr != err )
          ಹೋಗು ಸಿಕ್ಕಿತು_no_lwfn_file;

        error = lookup_lwfn_by_fond( ಮಾರ್ಗ_fond, lwfn_file_name,
                                     ಮಾರ್ಗ_lwfn, ಗಾತ್ರ ( ಮಾರ್ಗ_lwfn ) );
        ಆದರೆ ( !error )
          have_lwfn = 1;
      }
    }

    ಆದರೆ ( have_lwfn && ( !have_sfnt || PREFER_LWFN ) )
      error = FT_New_Face_From_LWFN( library,
                                     ಮಾರ್ಗ_lwfn,
                                     face_ಸೂಚ್ಯಂಕ,
                                     aface );
    ಇಲ್ಲದಿದ್ದರೆ
      error = FT_THROW( Unknown_File_Format );

  ಸಿಕ್ಕಿತು_no_lwfn_file:
    ಆದರೆ ( have_sfnt && error )
      error = FT_New_Face_From_SFNT( library,
                                     sfnt_id,
                                     face_ಸೂಚ್ಯಂಕ,
                                     aface );

    ಹಿಂತಿರುಗಿ error;
  }


  /* Common function to load a new FT_Face from a resource file. */
  ಸ್ಥಿರ FT_Error
  FT_New_Face_From_Resource( FT_Library    library,
                             ಸ್ಥಿರಾಂಕ UInt8*  pathname,
                             FT_Long       face_ಸೂಚ್ಯಂಕ,
                             FT_Face*      aface )
  {
    OSType    file_type;
    FT_Error  error;


    /* LWFN is a (very) specific file format, check for it explicitly */
    file_type = get_file_type_from_ಮಾರ್ಗ( pathname );
    ಆದರೆ ( file_type == TTAG_LWFN )
      ಹಿಂತಿರುಗಿ FT_New_Face_From_LWFN( library, pathname, face_ಸೂಚ್ಯಂಕ, aface );

    /* Otherwise the file type doesn't matter (there are more than  */
    /* `FFIL' and `tfil').  Just try opening it as a font suitcase; */
    /* if it works, fine.                                           */

    error = FT_New_Face_From_Suitcase( library, pathname, face_ಸೂಚ್ಯಂಕ, aface );
    ಆದರೆ ( error )
    {
      /* let it fall through to normal loader (.ttf, .otf, etc.); */
      /* we signal this by returning no error and no FT_Face      */
      *aface = ಖಾಲಿ;
    }

    ಹಿಂತಿರುಗಿ FT_Err_Ok;
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
               ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*  pathname,
               FT_Long      face_ಸೂಚ್ಯಂಕ,
               FT_Face*     aface )
  {
    FT_Open_ವಾದಗಳು  ವಾದಗಳು;
    FT_Error      error;


    /* test for valid `library' and `aface' delayed to FT_Open_Face() */
    ಆದರೆ ( !pathname )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

    *aface = ಖಾಲಿ;

    /* try resourcefork based font: LWFN, FFIL */
    error = FT_New_Face_From_Resource( library, (UInt8 *)pathname,
                                       face_ಸೂಚ್ಯಂಕ, aface );
    ಆದರೆ ( error || *aface )
      ಹಿಂತಿರುಗಿ error;

    /* let it fall through to normal loader (.ttf, .otf, etc.) */
    ವಾದಗಳು.flags    = FT_OPEN_PATHNAME;
    ವಾದಗಳು.pathname = (ಅಕ್ಷರ*)pathname;

    ಹಿಂತಿರುಗಿ FT_Open_Face( library, &ವಾದಗಳು, face_ಸೂಚ್ಯಂಕ, aface );
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
                          ಸ್ಥಿರಾಂಕ FSRef*  ref,
                          FT_Long       face_ಸೂಚ್ಯಂಕ,
                          FT_Face*      aface )
  {
    FT_Error      error;
    FT_Open_ವಾದಗಳು  ವಾದಗಳು;

    OSErr  err;
    UInt8  pathname[ಮಾರ್ಗ_MAX];


    /* check of `library' and `aface' delayed to */
    /* `FT_New_Face_From_Resource'               */

    ಆದರೆ ( !ref )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

    err = FSRef_Make_ಮಾರ್ಗ( ref, pathname, ಗಾತ್ರ ( pathname ) );
    ಆದರೆ ( err )
      error = FT_THROW( Cannot_Open_Resource );

    error = FT_New_Face_From_Resource( library, pathname, face_ಸೂಚ್ಯಂಕ, aface );
    ಆದರೆ ( error || *aface )
      ಹಿಂತಿರುಗಿ error;

    /* fallback to datafork font */
    ವಾದಗಳು.flags    = FT_OPEN_PATHNAME;
    ವಾದಗಳು.pathname = (ಅಕ್ಷರ*)pathname;
    ಹಿಂತಿರುಗಿ FT_Open_Face( library, &ವಾದಗಳು, face_ಸೂಚ್ಯಂಕ, aface );
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
                           ಸ್ಥಿರಾಂಕ FSSpec*  spec,
                           FT_Long        face_ಸೂಚ್ಯಂಕ,
                           FT_Face*       aface )
  {
#ಆದರೆ ( __LP64__ ) || ( defined( MAC_OS_X_ಆವೃತ್ತಿ_10_5 ) && \
      ( MAC_OS_X_ಆವೃತ್ತಿ_MIN_REQUIRED >= MAC_OS_X_ಆವೃತ್ತಿ_10_5 ) )
    FT_ಬಳಸದ( library );
    FT_ಬಳಸದ( spec );
    FT_ಬಳಸದ( face_ಸೂಚ್ಯಂಕ );
    FT_ಬಳಸದ( aface );

    ಹಿಂತಿರುಗಿ FT_THROW( Unimplemented_Feature );
#ಇಲ್ಲದಿದ್ದರೆ
    FSRef  ref;


    /* check of `library' and `aface' delayed to `FT_New_Face_From_FSRef' */

    ಆದರೆ ( !spec || FSpMakeFSRef( spec, &ref ) != noErr )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );
    ಇಲ್ಲದಿದ್ದರೆ
      ಹಿಂತಿರುಗಿ FT_New_Face_From_FSRef( library, &ref, face_ಸೂಚ್ಯಂಕ, aface );
#ಆದರೆ_ಅಂತ್ಯ
  }

#ಇಲ್ಲದಿದ್ದರೆ /* !FT_MACINTOSH */

  /* ANSI C doesn't like empty source files */
  ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಪೂರ್ಣಾಂಕ  ft_mac_dummy_;

#ಆದರೆ_ಅಂತ್ಯ /* !FT_MACINTOSH */


/*ಕೊನೆ*/
