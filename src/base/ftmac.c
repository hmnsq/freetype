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


#అన్తర్భావ <freetype/freetype.h>
#అన్తర్భావ <freetype/tttags.h>
#అన్తర్భావ <freetype/internal/ftdebug.h>
#అన్తర్భావ <freetype/internal/ftstream.h>
#అన్తర్భావ "ftbase.h"


#నిర్వచితమైతే FT_MACINTOSH

  /* This is for Mac OS X.  Without redefinition, OS_INLINE */
  /* expands to `static inline' which doesn't survive the   */
  /* -ansi compilation flag of GCC.                         */
#ఒకవేళ !HAVE_ANSI_OS_INLINE
#నిర్వచనం_రద్దు  OS_INLINE
#నిర్వచించు OS_INLINE  స్థిర __inline__
#అయితే_ముగింపు

  /* `configure' checks the availability of `ResourceIndex' strictly */
  /* and sets HAVE_TYPE_RESOURCE_INDEX 1 or 0 always.  If it is      */
  /* not set (e.g., a build without `configure'), the availability   */
  /* is guessed from the SDK version.                                */
#నిర్వచితం_కాకపోతే HAVE_TYPE_RESOURCE_సూచీ
#ఒకవేళ !defined( MAC_OS_X_సంస్కరణ_10_5 ) || \
    ( MAC_OS_X_సంస్కరణ_MAX_ALLOWED < MAC_OS_X_సంస్కరణ_10_5 )
#నిర్వచించు HAVE_TYPE_RESOURCE_సూచీ 0
#లేకపోతే
#నిర్వచించు HAVE_TYPE_RESOURCE_సూచీ 1
#అయితే_ముగింపు
#అయితే_ముగింపు /* !HAVE_TYPE_RESOURCE_INDEX */

#ఒకవేళ ( HAVE_TYPE_RESOURCE_సూచీ == 0 )
  రకనిర్వచనం పొట్టి  Resource_సూచీ;
#అయితే_ముగింపు

#అన్తర్భావ <CoreServices/CoreServices.h>
#అన్తర్భావ <ApplicationServices/ApplicationServices.h>
#అన్తర్భావ <sys/syslimits.h> /* PATH_MAX */

  /* Don't want warnings about our own use of deprecated functions. */
#నిర్వచించు FT_DEPRECATED_ATTRIBUTE

#అన్తర్భావ <freetype/ftmac.h>

#నిర్వచితం_కాకపోతే kATSOptionFlagsUnRestrictedScope /* since Mac OS X 10.1 */
#నిర్వచించు kATSOptionFlagsUnRestrictedScope kATSOptionFlagsDefault
#అయితే_ముగింపు


  /* Set PREFER_LWFN to 1 if LWFN (Type 1) is preferred over
     TrueType in case *both* are available (this is not common,
     but it *is* possible). */
#నిర్వచితం_కాకపోతే PREFER_LWFN
#నిర్వచించు PREFER_LWFN  1
#అయితే_ముగింపు


  /* This function is deprecated because FSSpec is deprecated in Mac OS X  */
  FT_EXPORT_DEF( FT_Error )
  FT_GetFile_From_Mac_Name( స్థిరరాశి అక్షరం*  fontName,
                            FSSpec*      మార్గం_Spec,
                            FT_Long*     face_సూచీ )
  {
    FT_వాడని( fontName );
    FT_వాడని( మార్గం_Spec );
    FT_వాడని( face_సూచీ );

    తిరిగి FT_THROW( Unimplemented_Feature );
  }


  /* Private function.                                         */
  /* The FSSpec type has been discouraged for a long time,     */
  /* unfortunately an FSRef replacement API for                */
  /* ATSFontGetFileSpecification() is only available in        */
  /* Mac OS X 10.5 and later.                                  */
  స్థిర OSStatus
  FT_ATSFontGetFileReference( ATSFontRef  ats_font_id,
                              FSRef*      ats_font_ref )
  {
#ఒకవేళ defined( MAC_OS_X_సంస్కరణ_10_5 ) && \
    ( MAC_OS_X_సంస్కరణ_MIN_REQUIRED >= MAC_OS_X_సంస్కరణ_10_5 )

    OSStatus  err;

    err = ATSFontGetFileReference( ats_font_id, ats_font_ref );

    తిరిగి err;
#లేదావేళ __LP64__ /* No 64bit Carbon API on legacy platforms */
    FT_వాడని( ats_font_id );
    FT_వాడని( ats_font_ref );


    తిరిగి fnfErr;
#లేకపోతే /* 32bit Carbon API on legacy platforms */
    OSStatus  err;
    FSSpec    spec;


    err = ATSFontGetFileSpecification( ats_font_id, &spec );
    ఒకవేళ ( noErr == err )
      err = FSpMakeFSRef( &spec, ats_font_ref );

    తిరిగి err;
#అయితే_ముగింపు
  }


  స్థిర FT_Error
  FT_GetFileRef_From_Mac_ATS_Name( స్థిరరాశి అక్షరం*  fontName,
                                   FSRef*       ats_font_ref,
                                   FT_Long*     face_సూచీ )
  {
    CFStringRef  cf_fontName;
    ATSFontRef   ats_font_id;


    *face_సూచీ = 0;

    cf_fontName = CFStringCreateWithCString( ఖాళీ, fontName,
                                             kCFStringEncodingMacRoman );
    ats_font_id = ATSFont_కనుగొను_From_Name( cf_fontName,
                                       kATSOptionFlagsUnRestrictedScope );
    CFRelease( cf_fontName );

    ఒకవేళ ( ats_font_id == 0 || ats_font_id == 0xFFFFFFFFUL )
      తిరిగి FT_THROW( Unknown_File_Format );

    ఒకవేళ ( noErr != FT_ATSFontGetFileReference( ats_font_id, ats_font_ref ) )
      తిరిగి FT_THROW( Unknown_File_Format );

    /* face_index calculation by searching preceding fontIDs */
    /* with same FSRef                                       */
    {
      ATSFontRef  id2 = ats_font_id - 1;
      FSRef       ref2;


      అయితే ( id2 > 0 )
      {
        ఒకవేళ ( noErr != FT_ATSFontGetFileReference( id2, &ref2 ) )
          ఆపు;
        ఒకవేళ ( noErr != FSCompareFSRefs( ats_font_ref, &ref2 ) )
          ఆపు;

        id2 --;
      }
      *face_సూచీ = ats_font_id - ( id2 + 1 );
    }

    తిరిగి FT_Err_Ok;
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Get_File_మార్గం_From_Mac_ATS_Name( స్థిరరాశి అక్షరం*  fontName,
                                    UInt8*       మార్గం,
                                    UInt32       max_మార్గం_కొలత,
                                    FT_Long*     face_సూచీ )
  {
    FSRef     ref;
    FT_Error  err;


    ఒకవేళ ( !fontName || !face_సూచీ )
      తిరిగి FT_THROW( Invalid_వాదం);

    err = FT_GetFileRef_From_Mac_ATS_Name( fontName, &ref, face_సూచీ );
    ఒకవేళ ( err )
      తిరిగి err;

    ఒకవేళ ( noErr != FSRef_Make_మార్గం( &ref, మార్గం, max_మార్గం_కొలత ) )
      తిరిగి FT_THROW( Unknown_File_Format );

    తిరిగి FT_Err_Ok;
  }


  /* This function is deprecated because FSSpec is deprecated in Mac OS X  */
  FT_EXPORT_DEF( FT_Error )
  FT_GetFile_From_Mac_ATS_Name( స్థిరరాశి అక్షరం*  fontName,
                                FSSpec*      మార్గం_Spec,
                                FT_Long*     face_సూచీ )
  {
#ఒకవేళ ( __LP64__ ) || ( defined( MAC_OS_X_సంస్కరణ_10_5 ) && \
      ( MAC_OS_X_సంస్కరణ_MIN_REQUIRED >= MAC_OS_X_సంస్కరణ_10_5 ) )
    FT_వాడని( fontName );
    FT_వాడని( మార్గం_Spec );
    FT_వాడని( face_సూచీ );

    తిరిగి FT_THROW( Unimplemented_Feature );
#లేకపోతే
    FSRef     ref;
    FT_Error  err;


    ఒకవేళ ( !fontName || !face_సూచీ )
      తిరిగి FT_THROW( Invalid_వాదం );

    err = FT_GetFileRef_From_Mac_ATS_Name( fontName, &ref, face_సూచీ );
    ఒకవేళ ( err )
      తిరిగి err;

    ఒకవేళ ( noErr != FSGetCatalogInfo( &ref, kFSCatInfoNone, ఖాళీ, ఖాళీ,
                                    మార్గం_Spec, ఖాళీ ) )
      తిరిగి FT_THROW( Unknown_File_Format );

    తిరిగి FT_Err_Ok;
#అయితే_ముగింపు
  }


  స్థిర OSErr
  FT_FSPathMakeRes( స్థిరరాశి UInt8*    pathname,
                    Res_File_Ref_అంకె*  res )
  {
    OSErr  err;
    FSRef  ref;


    ఒకవేళ ( noErr != FSPathMakeRef( pathname, &ref, FALSE ) )
      తిరిగి FT_THROW( Cannot_Open_Resource );

    /* at present, no support for dfont format */
    err = FSOpenResourceFile( &ref, 0, ఖాళీ, fsRdPerm, res );
    ఒకవేళ ( noErr == err )
      తిరిగి err;

    /* fallback to original resource-fork font */
    *res = FSOpenResFile( &ref, fsRdPerm );
    err  = ResError();

    తిరిగి err;
  }


  /* Return the file type for given pathname */
  స్థిర OSType
  get_file_type_from_మార్గం( స్థిరరాశి UInt8*  pathname )
  {
    FSRef          ref;
    FSCatalogInfo  info;


    ఒకవేళ ( noErr != FSPathMakeRef( pathname, &ref, FALSE ) )
      తిరిగి ( OSType ) 0;

    ఒకవేళ ( noErr != FSGetCatalogInfo( &ref, kFSCatInfoFinderInfo, &info,
                                    ఖాళీ, ఖాళీ, ఖాళీ ) )
      తిరిగి ( OSType ) 0;

    తిరిగి ( (FInfo *)( info.finderInfo ) )->fdType;
  }


  /* Given a PostScript font name, create the Macintosh LWFN file name. */
  స్థిర శూన్య
  create_lwfn_name( అక్షరం*   ps_name,
                    Str255  lwfn_file_name )
  {
    పూర్ణాంకం       max = 5, లెక్క = 0;
    FT_Byte*  p = lwfn_file_name;
    FT_Byte*  q = (FT_Byte*)ps_name;


    lwfn_file_name[0] = 0;

    అయితే ( *q )
    {
      ఒకవేళ ( ft_isupper( *q ) )
      {
        ఒకవేళ ( లెక్క )
          max = 3;
        లెక్క = 0;
      }
      ఒకవేళ ( లెక్క < max && ( ft_isalnum( *q ) || *q == '_' ) )
      {
        *++p = *q;
        lwfn_file_name[0]++;
        లెక్క++;
      }
      q++;
    }
  }


  స్థిర పొట్టి
  లెక్క_faces_sfnt( అక్షరం*  fond_దత్తాంశం )
  {
    /* The count is 1 greater than the value in the FOND.  */
    /* Isn't that cute? :-)                                */

    తిరిగి EndianS16_BtoN( *( (పొట్టి*)( fond_దత్తాంశం +
                                        పరిమాణం ( FamRec ) ) ) ) + 1;
  }


  స్థిర పొట్టి
  లెక్క_faces_scalable( అక్షరం*  fond_దత్తాంశం )
  {
    Assc_నమోదు*  assoc;
    పొట్టి       i, face, face_all;


    face_all = EndianS16_BtoN( *( (పొట్టి *)( fond_దత్తాంశం +
                                             పరిమాణం ( FamRec ) ) ) ) + 1;
    assoc    = (Assc_నమోదు*)( fond_దత్తాంశం + పరిమాణం ( FamRec ) + 2 );
    face     = 0;

    కొరకు ( i = 0; i < face_all; i++ )
    {
      ఒకవేళ ( 0 == EndianS16_BtoN( assoc[i].font_కొలత ) )
        face++;
    }
    తిరిగి face;
  }


  /* Look inside the FOND data, answer whether there should be an SFNT
     resource, and answer the name of a possible LWFN Type 1 file.

     Thanks to Paul Miller (paulm@profoundeffects.com) for the fix
     to load a face OTHER than the first one in the FOND!
  */


  స్థిర శూన్య
  parse_fond( అక్షరం*   fond_దత్తాంశం,
              పొట్టి*  have_sfnt,
              ResID*  sfnt_id,
              Str255  lwfn_file_name,
              పొట్టి   face_సూచీ )
  {
    Assc_నమోదు*  assoc;
    Assc_నమోదు*  పునాది_assoc;
    FamRec*     fond;


    *sfnt_id          = 0;
    *have_sfnt        = 0;
    lwfn_file_name[0] = 0;

    fond       = (FamRec*)fond_దత్తాంశం;
    assoc      = (Assc_నమోదు*)( fond_దత్తాంశం + పరిమాణం ( FamRec ) + 2 );
    పునాది_assoc = assoc;

    /* the maximum faces in a FOND is 48, size of StyleTable.indexes[] */
    ఒకవేళ ( 47 < face_సూచీ )
      తిరిగి;

    /* Let's do a little range checking before we get too excited here */
    ఒకవేళ ( face_సూచీ < లెక్క_faces_sfnt( fond_దత్తాంశం ) )
    {
      assoc += face_సూచీ;        /* add on the face_index! */

      /* if the face at this index is not scalable,
         fall back to the first one (old behavior) */
      ఒకవేళ ( EndianS16_BtoN( assoc->font_కొలత ) == 0 )
      {
        *have_sfnt = 1;
        *sfnt_id   = EndianS16_BtoN( assoc->fontID );
      }
      లేకపోతే ఒకవేళ ( పునాది_assoc->font_కొలత == 0 )
      {
        *have_sfnt = 1;
        *sfnt_id   = EndianS16_BtoN( పునాది_assoc->fontID );
      }
    }

    ఒకవేళ ( EndianS32_BtoN( fond->ffStylOff ) )
    {
      గుర్తులేని అక్షరం*  p = (గుర్తులేని అక్షరం*)fond_దత్తాంశం;
      Style_పట్టిక*     style;
      గుర్తులేని పొట్టి  string_లెక్క;
      అక్షరం            ps_name[256];
      గుర్తులేని అక్షరం*  పేర్లు[64];
      పూర్ణాంకం             i;


      p += EndianS32_BtoN( fond->ffStylOff );
      style = (Style_పట్టిక*)p;
      p += పరిమాణం ( Style_పట్టిక );
      string_లెక్క = EndianS16_BtoN( *(పొట్టి*)(p) );
      string_లెక్క = FT_MIN( 64, string_లెక్క );
      p += పరిమాణం ( పొట్టి );

      కొరకు ( i = 0; i < string_లెక్క; i++ )
      {
        పేర్లు[i] = p;
        p       += పేర్లు[i][0];
        p++;
      }

      {
        size_t  ps_name_len = (size_t)పేర్లు[0][0];


        ఒకవేళ ( ps_name_len != 0 )
        {
          ft_memcpy( ps_name, పేర్లు[0] + 1, ps_name_len );
          ps_name[ps_name_len] = 0;
        }
        ఒకవేళ ( style->indexes[face_సూచీ] > 1 &&
             style->indexes[face_సూచీ] <= string_లెక్క )
        {
          గుర్తులేని అక్షరం*  suffixes = పేర్లు[style->indexes[face_సూచీ] - 1];


          కొరకు ( i = 1; i <= suffixes[0]; i++ )
          {
            గుర్తులేని అక్షరం*  s;
            size_t          j = suffixes[i] - 1;


            ఒకవేళ ( j < string_లెక్క && ( s = పేర్లు[j] ) != ఖాళీ )
            {
              size_t  s_len = (size_t)s[0];


              ఒకవేళ ( s_len != 0 && ps_name_len + s_len < పరిమాణం ( ps_name ) )
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


  స్థిర  FT_Error
  lookup_lwfn_by_fond( స్థిరరాశి UInt8*      మార్గం_fond,
                       ConstStr255Param  పునాది_lwfn,
                       UInt8*            మార్గం_lwfn,
                       size_t            మార్గం_కొలత )
  {
    FSRef   ref, par_ref;
    size_t  dirname_len;


    /* Pathname for FSRef can be in various formats: HFS, HFS+, and POSIX. */
    /* We should not extract parent directory by string manipulation.      */

    ఒకవేళ ( noErr != FSPathMakeRef( మార్గం_fond, &ref, FALSE ) )
      తిరిగి FT_THROW( Invalid_వాదం );

    ఒకవేళ ( noErr != FSGetCatalogInfo( &ref, kFSCatInfoNone,
                                    ఖాళీ, ఖాళీ, ఖాళీ, &par_ref ) )
      తిరిగి FT_THROW( Invalid_వాదం );

    ఒకవేళ ( noErr != FSRef_Make_మార్గం( &par_ref, మార్గం_lwfn, మార్గం_కొలత ) )
      తిరిగి FT_THROW( Invalid_వాదం );

    ఒకవేళ ( ft_strlen( (అక్షరం *)మార్గం_lwfn ) + 1 + పునాది_lwfn[0] > మార్గం_కొలత )
      తిరిగి FT_THROW( Invalid_వాదం );

    /* now we have absolute dirname in path_lwfn */
    ft_strcat( (అక్షరం *)మార్గం_lwfn, "/" );
    dirname_len = ft_strlen( (అక్షరం *)మార్గం_lwfn );
    ft_strcat( (అక్షరం *)మార్గం_lwfn, (అక్షరం *)పునాది_lwfn + 1 );
    మార్గం_lwfn[dirname_len + పునాది_lwfn[0]] = '\0';

    ఒకవేళ ( noErr != FSPathMakeRef( మార్గం_lwfn, &ref, FALSE ) )
      తిరిగి FT_THROW( Cannot_Open_Resource );

    ఒకవేళ ( noErr != FSGetCatalogInfo( &ref, kFSCatInfoNone,
                                    ఖాళీ, ఖాళీ, ఖాళీ, ఖాళీ ) )
      తిరిగి FT_THROW( Cannot_Open_Resource );

    తిరిగి FT_Err_Ok;
  }


  స్థిర పొట్టి
  లెక్క_faces( Handle        fond,
               స్థిరరాశి UInt8*  pathname )
  {
    ResID     sfnt_id;
    పొట్టి     have_sfnt, have_lwfn;
    Str255    lwfn_file_name;
    UInt8     buff[మార్గం_MAX];
    FT_Error  err;
    పొట్టి     అంకె_faces;


    have_sfnt = have_lwfn = 0;

    parse_fond( *fond, &have_sfnt, &sfnt_id, lwfn_file_name, 0 );

    ఒకవేళ ( lwfn_file_name[0] )
    {
      err = lookup_lwfn_by_fond( pathname, lwfn_file_name,
                                 buff, పరిమాణం ( buff ) );
      ఒకవేళ ( !err )
        have_lwfn = 1;
    }

    ఒకవేళ ( have_lwfn && ( !have_sfnt || PREFER_LWFN ) )
      అంకె_faces = 1;
    లేకపోతే
      అంకె_faces = లెక్క_faces_scalable( *fond );

    తిరిగి అంకె_faces;
  }


  /* Read Type 1 data from the POST resources inside the LWFN file,
     return a PFB buffer.  This is somewhat convoluted because the FT2
     PFB parser wants the ASCII header as one chunk, and the LWFN
     chunks are often not organized that way, so we glue chunks
     of the same type together. */
  స్థిర FT_Error
  read_lwfn( FT_జ్ఞాపకం      జ్ఞాపకం,
             Res_File_Ref_అంకె  res,
             FT_Byte**      pfb_దత్తాంశం,
             FT_ULong*      కొలత )
  {
    FT_Error       error = FT_Err_Ok;
    ResID          res_id;
    గుర్తులేని అక్షరం  *బఫరు, *p, *కొలత_p = ఖాళీ;
    FT_ULong       మొత్తం_కొలత = 0;
    FT_ULong       old_మొత్తం_కొలత = 0;
    FT_ULong       post_కొలత, pfb_chunk_కొలత;
    Handle         post_దత్తాంశం;
    అక్షరం           సంకేతం_2, చివరి_సంకేతం;


    UseResFile( res );

    /* First pass: load all POST resources, and determine the size of */
    /* the output buffer.                                             */
    res_id    = 501;
    చివరి_సంకేతం = -1;

    కొరకు (;;)
    {
      post_దత్తాంశం = Get1Resource( TTAG_POST, res_id++ );
      ఒకవేళ ( !post_దత్తాంశం )
        ఆపు;  /* we are done */

      సంకేతం_2 = (*post_దత్తాంశం)[0];

      ఒకవేళ ( సంకేతం_2 != చివరి_సంకేతం )
      {
        ఒకవేళ ( సంకేతం_2 == 5 )
          మొత్తం_కొలత += 2; /* just the end code */
        లేకపోతే
          మొత్తం_కొలత += 6; /* code + 4 bytes chunk length */
      }

      మొత్తం_కొలత += (FT_ULong)Get_Handle_కొలత( post_దత్తాంశం ) - 2;
      చివరి_సంకేతం = సంకేతం_2;

      /* detect resource fork overflow */
      ఒకవేళ ( FT_MAC_RFORK_MAX_LEN < మొత్తం_కొలత )
      {
        error = FT_THROW( Array_Too_Large );
        వెళ్ళు Error;
      }

      old_మొత్తం_కొలత = మొత్తం_కొలత;
    }

    ఒకవేళ ( FT_QALLOC( బఫరు, (FT_Long)మొత్తం_కొలత ) )
      వెళ్ళు Error;

    /* Second pass: append all POST data to the buffer, add PFB fields. */
    /* Glue all consecutive chunks of the same type together.           */
    p              = బఫరు;
    res_id         = 501;
    చివరి_సంకేతం      = -1;
    pfb_chunk_కొలత = 0;

    కొరకు (;;)
    {
      post_దత్తాంశం = Get1Resource( TTAG_POST, res_id++ );
      ఒకవేళ ( !post_దత్తాంశం )
        ఆపు;  /* we are done */

      post_కొలత = (FT_ULong)Get_Handle_కొలత( post_దత్తాంశం ) - 2;
      సంకేతం_2 = (*post_దత్తాంశం)[0];

      ఒకవేళ ( సంకేతం_2 != చివరి_సంకేతం )
      {
        ఒకవేళ ( చివరి_సంకేతం != -1 )
        {
          /* we are done adding a chunk, fill in the size field */
          ఒకవేళ ( కొలత_p )
          {
            *కొలత_p++ = (FT_Byte)(   pfb_chunk_కొలత         & 0xFF );
            *కొలత_p++ = (FT_Byte)( ( pfb_chunk_కొలత >> 8  ) & 0xFF );
            *కొలత_p++ = (FT_Byte)( ( pfb_chunk_కొలత >> 16 ) & 0xFF );
            *కొలత_p++ = (FT_Byte)( ( pfb_chunk_కొలత >> 24 ) & 0xFF );
          }
          pfb_chunk_కొలత = 0;
        }

        *p++ = 0x80;
        ఒకవేళ ( సంకేతం_2 == 5 )
          *p++ = 0x03;  /* the end */
        లేకపోతే ఒకవేళ ( సంకేతం_2 == 2 )
          *p++ = 0x02;  /* binary segment */
        లేకపోతే
          *p++ = 0x01;  /* ASCII segment */

        ఒకవేళ ( సంకేతం_2 != 5 )
        {
          కొలత_p = p;   /* save for later */
          p += 4;       /* make space for size field */
        }
      }

      ft_memcpy( p, *post_దత్తాంశం + 2, post_కొలత );
      pfb_chunk_కొలత += post_కొలత;
      p += post_కొలత;
      చివరి_సంకేతం = సంకేతం_2;
    }

    *pfb_దత్తాంశం = బఫరు;
    *కొలత = మొత్తం_కొలత;

  Error:
    CloseResFile( res );
    తిరిగి error;
  }


  /* Create a new FT_Face from a file path to an LWFN file. */
  స్థిర FT_Error
  FT_New_Face_From_LWFN( FT_Library    library,
                         స్థిరరాశి UInt8*  pathname,
                         FT_Long       face_సూచీ,
                         FT_Face*      aface )
  {
    FT_Byte*       pfb_దత్తాంశం;
    FT_ULong       pfb_కొలత;
    FT_Error       error;
    Res_File_Ref_అంకె  res;


    ఒకవేళ ( noErr != FT_FSPathMakeRes( pathname, &res ) )
      తిరిగి FT_THROW( Cannot_Open_Resource );

    pfb_దత్తాంశం = ఖాళీ;
    pfb_కొలత = 0;
    error = read_lwfn( library->జ్ఞాపకం, res, &pfb_దత్తాంశం, &pfb_కొలత );
    CloseResFile( res ); /* PFB is already loaded, useless anymore */
    ఒకవేళ ( error )
      తిరిగి error;

    తిరిగి open_face_from_బఫరు( library,
                                  pfb_దత్తాంశం,
                                  pfb_కొలత,
                                  face_సూచీ,
                                  "type1",
                                  aface );
  }


  /* Create a new FT_Face from an SFNT resource, specified by res ID. */
  స్థిర FT_Error
  FT_New_Face_From_SFNT( FT_Library  library,
                         ResID       sfnt_id,
                         FT_Long     face_సూచీ,
                         FT_Face*    aface )
  {
    Handle     sfnt = ఖాళీ;
    FT_Byte*   sfnt_దత్తాంశం;
    size_t     sfnt_కొలత;
    FT_Error   error  = FT_Err_Ok;
    FT_జ్ఞాపకం  జ్ఞాపకం = library->జ్ఞాపకం;
    పూర్ణాంకం        is_cff, is_sfnt_ps;


    sfnt = GetResource( TTAG_sfnt, sfnt_id );
    ఒకవేళ ( !sfnt )
      తిరిగి FT_THROW( Invalid_Handle );

    sfnt_కొలత = (FT_ULong)Get_Handle_కొలత( sfnt );

    /* detect resource fork overflow */
    ఒకవేళ ( FT_MAC_RFORK_MAX_LEN < sfnt_కొలత )
      తిరిగి FT_THROW( Array_Too_Large );

    ఒకవేళ ( FT_QALLOC( sfnt_దత్తాంశం, (FT_Long)sfnt_కొలత ) )
    {
      ReleaseResource( sfnt );
      తిరిగి error;
    }

    ft_memcpy( sfnt_దత్తాంశం, *sfnt, sfnt_కొలత );
    ReleaseResource( sfnt );

    is_cff     = sfnt_కొలత > 4 && !ft_memcmp( sfnt_దత్తాంశం, "OTTO", 4 );
    is_sfnt_ps = sfnt_కొలత > 4 && !ft_memcmp( sfnt_దత్తాంశం, "typ1", 4 );

    ఒకవేళ ( is_sfnt_ps )
    {
      FT_ప్రవాహం  ప్రవాహం;


      ఒకవేళ ( FT_NEW( ప్రవాహం ) )
        వెళ్ళు Try_OpenType;

      FT_ప్రవాహం_Open_జ్ఞాపకం( ప్రవాహం, sfnt_దత్తాంశం, sfnt_కొలత );
      ఒకవేళ ( !open_face_PS_from_sfnt_ప్రవాహం( library,
                                           ప్రవాహం,
                                           face_సూచీ,
                                           0, ఖాళీ,
                                           aface ) )
      {
        FT_ప్రవాహం_Close( ప్రవాహం );
        FT_FREE( ప్రవాహం );
        FT_FREE( sfnt_దత్తాంశం );
        వెళ్ళు Exit;
      }

      FT_FREE( ప్రవాహం );
    }
  Try_OpenType:
    error = open_face_from_బఫరు( library,
                                   sfnt_దత్తాంశం,
                                   sfnt_కొలత,
                                   face_సూచీ,
                                   is_cff ? "cff" : "truetype",
                                   aface );
  Exit:
    తిరిగి error;
  }


  /* Create a new FT_Face from a file path to a suitcase file. */
  స్థిర FT_Error
  FT_New_Face_From_Suitcase( FT_Library    library,
                             స్థిరరాశి UInt8*  pathname,
                             FT_Long       face_సూచీ,
                             FT_Face*      aface )
  {
    FT_Error       error = FT_ERR( Cannot_Open_Resource );
    Res_File_Ref_అంకె  res_ref;
    Resource_సూచీ  res_సూచీ;
    Handle         fond;
    పొట్టి          అంకె_faces_లోపల_res;
    FT_Long        లెక్క;


    ఒకవేళ ( noErr != FT_FSPathMakeRes( pathname, &res_ref ) )
      తిరిగి FT_THROW( Cannot_Open_Resource );

    UseResFile( res_ref );
    ఒకవేళ ( ResError() )
      తిరిగి FT_THROW( Cannot_Open_Resource );

    res_సూచీ        = 1;
    అంకె_faces_లోపల_res = 0;
    లెక్క            = face_సూచీ;
    అయితే ( లెక్క >= 0 )
    {
      పొట్టి  అంకె_faces_లోపల_fond;


      fond = Get1IndResource( TTAG_FOND, res_సూచీ );
      ఒకవేళ ( ResError() )
        ఆపు;

      అంకె_faces_లోపల_fond  = లెక్క_faces( fond, pathname );
      అంకె_faces_లోపల_res  += అంకె_faces_లోపల_fond;

      ఒకవేళ ( లెక్క < అంకె_faces_లోపల_fond )
        error = FT_New_Face_From_FOND( library, fond, లెక్క, aface );

      res_సూచీ++;
      లెక్క -= అంకె_faces_లోపల_fond;
    }

    CloseResFile( res_ref );

    ఒకవేళ ( !error && aface && *aface )
    {
      (*aface)->అంకె_faces  = అంకె_faces_లోపల_res;
      (*aface)->face_సూచీ = face_సూచీ;
    }

    తిరిగి error;
  }


  /* documentation is in ftmac.h */

  FT_EXPORT_DEF( FT_Error )
  FT_New_Face_From_FOND( FT_Library  library,
                         Handle      fond,
                         FT_Long     face_సూచీ,
                         FT_Face*    aface )
  {
    పొట్టి     have_sfnt, have_lwfn = 0;
    ResID     sfnt_id, fond_id;
    OSType    fond_type;
    Str255    fond_name;
    Str255    lwfn_file_name;
    UInt8     మార్గం_lwfn[మార్గం_MAX];
    OSErr     err;
    FT_Error  error = FT_Err_Ok;


    /* check of `library' and `aface' delayed to `FT_New_Face_From_XXX' */

    GetResInfo( fond, &fond_id, &fond_type, fond_name );
    ఒకవేళ ( ResError() != noErr || fond_type != TTAG_FOND )
      తిరిగి FT_THROW( Invalid_File_Format );

    parse_fond( *fond, &have_sfnt, &sfnt_id, lwfn_file_name, face_సూచీ );

    ఒకవేళ ( lwfn_file_name[0] )
    {
      Res_File_Ref_అంకె  res;


      res = HomeResFile( fond );
      ఒకవేళ ( noErr != ResError() )
        వెళ్ళు దొరికింది_no_lwfn_file;

      {
        UInt8  మార్గం_fond[మార్గం_MAX];
        FSRef  ref;


        err = FSGetForkCBInfo( res, k_FSInvalid_Volume_Ref_అంకె,
                               ఖాళీ, ఖాళీ, ఖాళీ, &ref, ఖాళీ );
        ఒకవేళ ( noErr != err )
          వెళ్ళు దొరికింది_no_lwfn_file;

        err = FSRef_Make_మార్గం( &ref, మార్గం_fond, పరిమాణం ( మార్గం_fond ) );
        ఒకవేళ ( noErr != err )
          వెళ్ళు దొరికింది_no_lwfn_file;

        error = lookup_lwfn_by_fond( మార్గం_fond, lwfn_file_name,
                                     మార్గం_lwfn, పరిమాణం ( మార్గం_lwfn ) );
        ఒకవేళ ( !error )
          have_lwfn = 1;
      }
    }

    ఒకవేళ ( have_lwfn && ( !have_sfnt || PREFER_LWFN ) )
      error = FT_New_Face_From_LWFN( library,
                                     మార్గం_lwfn,
                                     face_సూచీ,
                                     aface );
    లేకపోతే
      error = FT_THROW( Unknown_File_Format );

  దొరికింది_no_lwfn_file:
    ఒకవేళ ( have_sfnt && error )
      error = FT_New_Face_From_SFNT( library,
                                     sfnt_id,
                                     face_సూచీ,
                                     aface );

    తిరిగి error;
  }


  /* Common function to load a new FT_Face from a resource file. */
  స్థిర FT_Error
  FT_New_Face_From_Resource( FT_Library    library,
                             స్థిరరాశి UInt8*  pathname,
                             FT_Long       face_సూచీ,
                             FT_Face*      aface )
  {
    OSType    file_type;
    FT_Error  error;


    /* LWFN is a (very) specific file format, check for it explicitly */
    file_type = get_file_type_from_మార్గం( pathname );
    ఒకవేళ ( file_type == TTAG_LWFN )
      తిరిగి FT_New_Face_From_LWFN( library, pathname, face_సూచీ, aface );

    /* Otherwise the file type doesn't matter (there are more than  */
    /* `FFIL' and `tfil').  Just try opening it as a font suitcase; */
    /* if it works, fine.                                           */

    error = FT_New_Face_From_Suitcase( library, pathname, face_సూచీ, aface );
    ఒకవేళ ( error )
    {
      /* let it fall through to normal loader (.ttf, .otf, etc.); */
      /* we signal this by returning no error and no FT_Face      */
      *aface = ఖాళీ;
    }

    తిరిగి FT_Err_Ok;
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
               స్థిరరాశి అక్షరం*  pathname,
               FT_Long      face_సూచీ,
               FT_Face*     aface )
  {
    FT_Open_వాదనలు  వాదనలు;
    FT_Error      error;


    /* test for valid `library' and `aface' delayed to FT_Open_Face() */
    ఒకవేళ ( !pathname )
      తిరిగి FT_THROW( Invalid_వాదం );

    *aface = ఖాళీ;

    /* try resourcefork based font: LWFN, FFIL */
    error = FT_New_Face_From_Resource( library, (UInt8 *)pathname,
                                       face_సూచీ, aface );
    ఒకవేళ ( error || *aface )
      తిరిగి error;

    /* let it fall through to normal loader (.ttf, .otf, etc.) */
    వాదనలు.flags    = FT_OPEN_PATHNAME;
    వాదనలు.pathname = (అక్షరం*)pathname;

    తిరిగి FT_Open_Face( library, &వాదనలు, face_సూచీ, aface );
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
                          స్థిరరాశి FSRef*  ref,
                          FT_Long       face_సూచీ,
                          FT_Face*      aface )
  {
    FT_Error      error;
    FT_Open_వాదనలు  వాదనలు;

    OSErr  err;
    UInt8  pathname[మార్గం_MAX];


    /* check of `library' and `aface' delayed to */
    /* `FT_New_Face_From_Resource'               */

    ఒకవేళ ( !ref )
      తిరిగి FT_THROW( Invalid_వాదం );

    err = FSRef_Make_మార్గం( ref, pathname, పరిమాణం ( pathname ) );
    ఒకవేళ ( err )
      error = FT_THROW( Cannot_Open_Resource );

    error = FT_New_Face_From_Resource( library, pathname, face_సూచీ, aface );
    ఒకవేళ ( error || *aface )
      తిరిగి error;

    /* fallback to datafork font */
    వాదనలు.flags    = FT_OPEN_PATHNAME;
    వాదనలు.pathname = (అక్షరం*)pathname;
    తిరిగి FT_Open_Face( library, &వాదనలు, face_సూచీ, aface );
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
                           స్థిరరాశి FSSpec*  spec,
                           FT_Long        face_సూచీ,
                           FT_Face*       aface )
  {
#ఒకవేళ ( __LP64__ ) || ( defined( MAC_OS_X_సంస్కరణ_10_5 ) && \
      ( MAC_OS_X_సంస్కరణ_MIN_REQUIRED >= MAC_OS_X_సంస్కరణ_10_5 ) )
    FT_వాడని( library );
    FT_వాడని( spec );
    FT_వాడని( face_సూచీ );
    FT_వాడని( aface );

    తిరిగి FT_THROW( Unimplemented_Feature );
#లేకపోతే
    FSRef  ref;


    /* check of `library' and `aface' delayed to `FT_New_Face_From_FSRef' */

    ఒకవేళ ( !spec || FSpMakeFSRef( spec, &ref ) != noErr )
      తిరిగి FT_THROW( Invalid_వాదం );
    లేకపోతే
      తిరిగి FT_New_Face_From_FSRef( library, &ref, face_సూచీ, aface );
#అయితే_ముగింపు
  }

#లేకపోతే /* !FT_MACINTOSH */

  /* ANSI C doesn't like empty source files */
  రకనిర్వచనం పూర్ణాంకం  ft_mac_dummy_;

#అయితే_ముగింపు /* !FT_MACINTOSH */


/*చివర*/
