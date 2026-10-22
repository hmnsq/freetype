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


#समावेश <freetype/freetype.h>
#समावेश <freetype/tttags.h>
#समावेश <freetype/internal/ftdebug.h>
#समावेश <freetype/internal/ftstream.h>
#समावेश "ftbase.h"


#अगर_घोषित FT_MACINTOSH

  /* This is for Mac OS X.  Without redefinition, OS_INLINE */
  /* expands to `static inline' which doesn't survive the   */
  /* -ansi compilation flag of GCC.                         */
#अगर !HAVE_ANSI_OS_INLINE
#अघोषित  OS_INLINE
#घोषणा OS_INLINE  अटल __inline__
#पूर्ण_अगर

  /* `configure' checks the availability of `ResourceIndex' strictly */
  /* and sets HAVE_TYPE_RESOURCE_INDEX 1 or 0 always.  If it is      */
  /* not set (e.g., a build without `configure'), the availability   */
  /* is guessed from the SDK version.                                */
#अगर_अघोषित HAVE_TYPE_RESOURCE_सूचकांक
#अगर !defined( MAC_OS_X_VERSION_10_5 ) || \
    ( MAC_OS_X_VERSION_MAX_ALLOWED < MAC_OS_X_VERSION_10_5 )
#घोषणा HAVE_TYPE_RESOURCE_सूचकांक 0
#अन्यथा
#घोषणा HAVE_TYPE_RESOURCE_सूचकांक 1
#पूर्ण_अगर
#पूर्ण_अगर /* !HAVE_TYPE_RESOURCE_INDEX */

#अगर ( HAVE_TYPE_RESOURCE_सूचकांक == 0 )
  प्रकार लघु  Resource_सूचकांक;
#पूर्ण_अगर

#समावेश <CoreServices/CoreServices.h>
#समावेश <ApplicationServices/ApplicationServices.h>
#समावेश <sys/syslimits.h> /* PATH_MAX */

  /* Don't want warnings about our own use of deprecated functions. */
#घोषणा FT_DEPRECATED_ATTRIBUTE

#समावेश <freetype/ftmac.h>

#अगर_अघोषित kATSOptionFlagsUnRestrictedScope /* since Mac OS X 10.1 */
#घोषणा kATSOptionFlagsUnRestrictedScope kATSOptionFlagsDefault
#पूर्ण_अगर


  /* Set PREFER_LWFN to 1 if LWFN (Type 1) is preferred over
     TrueType in case *both* are available (this is not common,
     but it *is* possible). */
#अगर_अघोषित PREFER_LWFN
#घोषणा PREFER_LWFN  1
#पूर्ण_अगर


  /* This function is deprecated because FSSpec is deprecated in Mac OS X  */
  FT_EXPORT_DEF( FT_Error )
  FT_GetFile_From_Mac_Name( स्थिर अक्षर*  fontName,
                            FSSpec*      pathSpec,
                            FT_Long*     face_सूचकांक )
  {
    FT_UNUSED( fontName );
    FT_UNUSED( pathSpec );
    FT_UNUSED( face_सूचकांक );

    वापस FT_THROW( Unimplemented_Feature );
  }


  /* Private function.                                         */
  /* The FSSpec type has been discouraged for a long time,     */
  /* unfortunately an FSRef replacement API for                */
  /* ATSFontGetFileSpecification() is only available in        */
  /* Mac OS X 10.5 and later.                                  */
  अटल OSStatus
  FT_ATSFontGetFileReference( ATSFontRef  ats_font_id,
                              FSRef*      ats_font_ref )
  {
#अगर defined( MAC_OS_X_VERSION_10_5 ) && \
    ( MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_5 )

    OSStatus  err;

    err = ATSFontGetFileReference( ats_font_id, ats_font_ref );

    वापस err;
#या_अगर __LP64__ /* No 64bit Carbon API on legacy platforms */
    FT_UNUSED( ats_font_id );
    FT_UNUSED( ats_font_ref );


    वापस fnfErr;
#अन्यथा /* 32bit Carbon API on legacy platforms */
    OSStatus  err;
    FSSpec    spec;


    err = ATSFontGetFileSpecification( ats_font_id, &spec );
    अगर ( noErr == err )
      err = FSpMakeFSRef( &spec, ats_font_ref );

    वापस err;
#पूर्ण_अगर
  }


  अटल FT_Error
  FT_GetFileRef_From_Mac_ATS_Name( स्थिर अक्षर*  fontName,
                                   FSRef*       ats_font_ref,
                                   FT_Long*     face_सूचकांक )
  {
    CFStringRef  cf_fontName;
    ATSFontRef   ats_font_id;


    *face_सूचकांक = 0;

    cf_fontName = CFStringCreateWithCString( शून्य, fontName,
                                             kCFStringEncodingMacRoman );
    ats_font_id = ATSFontFindFromName( cf_fontName,
                                       kATSOptionFlagsUnRestrictedScope );
    CFRelease( cf_fontName );

    अगर ( ats_font_id == 0 || ats_font_id == 0xFFFFFFFFUL )
      वापस FT_THROW( Unknown_File_Format );

    अगर ( noErr != FT_ATSFontGetFileReference( ats_font_id, ats_font_ref ) )
      वापस FT_THROW( Unknown_File_Format );

    /* face_index calculation by searching preceding fontIDs */
    /* with same FSRef                                       */
    {
      ATSFontRef  id2 = ats_font_id - 1;
      FSRef       ref2;


      जबतक ( id2 > 0 )
      {
        अगर ( noErr != FT_ATSFontGetFileReference( id2, &ref2 ) )
          अवरोध;
        अगर ( noErr != FSCompareFSRefs( ats_font_ref, &ref2 ) )
          अवरोध;

        id2 --;
      }
      *face_सूचकांक = ats_font_id - ( id2 + 1 );
    }

    वापस FT_Err_Ok;
  }


  FT_EXPORT_DEF( FT_Error )
  FT_GetFilePath_From_Mac_ATS_Name( स्थिर अक्षर*  fontName,
                                    UInt8*       path,
                                    UInt32       max_Path_आकार,
                                    FT_Long*     face_सूचकांक )
  {
    FSRef     ref;
    FT_Error  err;


    अगर ( !fontName || !face_सूचकांक )
      वापस FT_THROW( Invalid_Argument);

    err = FT_GetFileRef_From_Mac_ATS_Name( fontName, &ref, face_सूचकांक );
    अगर ( err )
      वापस err;

    अगर ( noErr != FSRefMakePath( &ref, path, max_Path_आकार ) )
      वापस FT_THROW( Unknown_File_Format );

    वापस FT_Err_Ok;
  }


  /* This function is deprecated because FSSpec is deprecated in Mac OS X  */
  FT_EXPORT_DEF( FT_Error )
  FT_GetFile_From_Mac_ATS_Name( स्थिर अक्षर*  fontName,
                                FSSpec*      pathSpec,
                                FT_Long*     face_सूचकांक )
  {
#अगर ( __LP64__ ) || ( defined( MAC_OS_X_VERSION_10_5 ) && \
      ( MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_5 ) )
    FT_UNUSED( fontName );
    FT_UNUSED( pathSpec );
    FT_UNUSED( face_सूचकांक );

    वापस FT_THROW( Unimplemented_Feature );
#अन्यथा
    FSRef     ref;
    FT_Error  err;


    अगर ( !fontName || !face_सूचकांक )
      वापस FT_THROW( Invalid_Argument );

    err = FT_GetFileRef_From_Mac_ATS_Name( fontName, &ref, face_सूचकांक );
    अगर ( err )
      वापस err;

    अगर ( noErr != FSGetCatalogInfo( &ref, kFSCatInfoNone, शून्य, शून्य,
                                    pathSpec, शून्य ) )
      वापस FT_THROW( Unknown_File_Format );

    वापस FT_Err_Ok;
#पूर्ण_अगर
  }


  अटल OSErr
  FT_FSPathMakeRes( स्थिर UInt8*    pathname,
                    Res_File_Ref_अंक*  res )
  {
    OSErr  err;
    FSRef  ref;


    अगर ( noErr != FSPathMakeRef( pathname, &ref, FALSE ) )
      वापस FT_THROW( Cannot_Open_Resource );

    /* at present, no support for dfont format */
    err = FSOpenResourceFile( &ref, 0, शून्य, fsRdPerm, res );
    अगर ( noErr == err )
      वापस err;

    /* fallback to original resource-fork font */
    *res = FSOpenResFile( &ref, fsRdPerm );
    err  = ResError();

    वापस err;
  }


  /* Return the file type for given pathname */
  अटल OSType
  get_file_type_from_path( स्थिर UInt8*  pathname )
  {
    FSRef          ref;
    FSCatalogInfo  info;


    अगर ( noErr != FSPathMakeRef( pathname, &ref, FALSE ) )
      वापस ( OSType ) 0;

    अगर ( noErr != FSGetCatalogInfo( &ref, kFSCatInfoFinderInfo, &info,
                                    शून्य, शून्य, शून्य ) )
      वापस ( OSType ) 0;

    वापस ( (FInfo *)( info.finderInfo ) )->fdType;
  }


  /* Given a PostScript font name, create the Macintosh LWFN file name. */
  अटल व्योम
  create_lwfn_name( अक्षर*   ps_name,
                    Str255  lwfn_file_name )
  {
    पूर्णांक       max = 5, गिनती = 0;
    FT_Byte*  p = lwfn_file_name;
    FT_Byte*  q = (FT_Byte*)ps_name;


    lwfn_file_name[0] = 0;

    जबतक ( *q )
    {
      अगर ( ft_isupper( *q ) )
      {
        अगर ( गिनती )
          max = 3;
        गिनती = 0;
      }
      अगर ( गिनती < max && ( ft_isalnum( *q ) || *q == '_' ) )
      {
        *++p = *q;
        lwfn_file_name[0]++;
        गिनती++;
      }
      q++;
    }
  }


  अटल लघु
  गिनती_faces_sfnt( अक्षर*  fond_तथ्य )
  {
    /* The count is 1 greater than the value in the FOND.  */
    /* Isn't that cute? :-)                                */

    वापस EndianS16_BtoN( *( (लघु*)( fond_तथ्य +
                                        माप ( FamRec ) ) ) ) + 1;
  }


  अटल लघु
  गिनती_faces_scalable( अक्षर*  fond_तथ्य )
  {
    AsscEntry*  assoc;
    लघु       i, face, face_all;


    face_all = EndianS16_BtoN( *( (लघु *)( fond_तथ्य +
                                             माप ( FamRec ) ) ) ) + 1;
    assoc    = (AsscEntry*)( fond_तथ्य + माप ( FamRec ) + 2 );
    face     = 0;

    क्रम ( i = 0; i < face_all; i++ )
    {
      अगर ( 0 == EndianS16_BtoN( assoc[i].font_आकार ) )
        face++;
    }
    वापस face;
  }


  /* Look inside the FOND data, answer whether there should be an SFNT
     resource, and answer the name of a possible LWFN Type 1 file.

     Thanks to Paul Miller (paulm@profoundeffects.com) for the fix
     to load a face OTHER than the first one in the FOND!
  */


  अटल व्योम
  parse_fond( अक्षर*   fond_तथ्य,
              लघु*  have_sfnt,
              ResID*  sfnt_id,
              Str255  lwfn_file_name,
              लघु   face_सूचकांक )
  {
    AsscEntry*  assoc;
    AsscEntry*  base_assoc;
    FamRec*     fond;


    *sfnt_id          = 0;
    *have_sfnt        = 0;
    lwfn_file_name[0] = 0;

    fond       = (FamRec*)fond_तथ्य;
    assoc      = (AsscEntry*)( fond_तथ्य + माप ( FamRec ) + 2 );
    base_assoc = assoc;

    /* the maximum faces in a FOND is 48, size of StyleTable.indexes[] */
    अगर ( 47 < face_सूचकांक )
      वापस;

    /* Let's do a little range checking before we get too excited here */
    अगर ( face_सूचकांक < गिनती_faces_sfnt( fond_तथ्य ) )
    {
      assoc += face_सूचकांक;        /* add on the face_index! */

      /* if the face at this index is not scalable,
         fall back to the first one (old behavior) */
      अगर ( EndianS16_BtoN( assoc->font_आकार ) == 0 )
      {
        *have_sfnt = 1;
        *sfnt_id   = EndianS16_BtoN( assoc->fontID );
      }
      अन्यथा अगर ( base_assoc->font_आकार == 0 )
      {
        *have_sfnt = 1;
        *sfnt_id   = EndianS16_BtoN( base_assoc->fontID );
      }
    }

    अगर ( EndianS32_BtoN( fond->ffStylOff ) )
    {
      अचिन्हित अक्षर*  p = (अचिन्हित अक्षर*)fond_तथ्य;
      StyleTable*     style;
      अचिन्हित लघु  string_गिनती;
      अक्षर            ps_name[256];
      अचिन्हित अक्षर*  names[64];
      पूर्णांक             i;


      p += EndianS32_BtoN( fond->ffStylOff );
      style = (StyleTable*)p;
      p += माप ( StyleTable );
      string_गिनती = EndianS16_BtoN( *(लघु*)(p) );
      string_गिनती = FT_MIN( 64, string_गिनती );
      p += माप ( लघु );

      क्रम ( i = 0; i < string_गिनती; i++ )
      {
        names[i] = p;
        p       += names[i][0];
        p++;
      }

      {
        माप_प्रकार  ps_name_len = (माप_प्रकार)names[0][0];


        अगर ( ps_name_len != 0 )
        {
          ft_memcpy( ps_name, names[0] + 1, ps_name_len );
          ps_name[ps_name_len] = 0;
        }
        अगर ( style->indexes[face_सूचकांक] > 1 &&
             style->indexes[face_सूचकांक] <= string_गिनती )
        {
          अचिन्हित अक्षर*  suffixes = names[style->indexes[face_सूचकांक] - 1];


          क्रम ( i = 1; i <= suffixes[0]; i++ )
          {
            अचिन्हित अक्षर*  s;
            माप_प्रकार          j = suffixes[i] - 1;


            अगर ( j < string_गिनती && ( s = names[j] ) != शून्य )
            {
              माप_प्रकार  s_len = (माप_प्रकार)s[0];


              अगर ( s_len != 0 && ps_name_len + s_len < माप ( ps_name ) )
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


  अटल  FT_Error
  lookup_lwfn_by_fond( स्थिर UInt8*      path_fond,
                       ConstStr255Param  base_lwfn,
                       UInt8*            path_lwfn,
                       माप_प्रकार            path_आकार )
  {
    FSRef   ref, par_ref;
    माप_प्रकार  dirname_len;


    /* Pathname for FSRef can be in various formats: HFS, HFS+, and POSIX. */
    /* We should not extract parent directory by string manipulation.      */

    अगर ( noErr != FSPathMakeRef( path_fond, &ref, FALSE ) )
      वापस FT_THROW( Invalid_Argument );

    अगर ( noErr != FSGetCatalogInfo( &ref, kFSCatInfoNone,
                                    शून्य, शून्य, शून्य, &par_ref ) )
      वापस FT_THROW( Invalid_Argument );

    अगर ( noErr != FSRefMakePath( &par_ref, path_lwfn, path_आकार ) )
      वापस FT_THROW( Invalid_Argument );

    अगर ( ft_strlen( (अक्षर *)path_lwfn ) + 1 + base_lwfn[0] > path_आकार )
      वापस FT_THROW( Invalid_Argument );

    /* now we have absolute dirname in path_lwfn */
    ft_strcat( (अक्षर *)path_lwfn, "/" );
    dirname_len = ft_strlen( (अक्षर *)path_lwfn );
    ft_strcat( (अक्षर *)path_lwfn, (अक्षर *)base_lwfn + 1 );
    path_lwfn[dirname_len + base_lwfn[0]] = '\0';

    अगर ( noErr != FSPathMakeRef( path_lwfn, &ref, FALSE ) )
      वापस FT_THROW( Cannot_Open_Resource );

    अगर ( noErr != FSGetCatalogInfo( &ref, kFSCatInfoNone,
                                    शून्य, शून्य, शून्य, शून्य ) )
      वापस FT_THROW( Cannot_Open_Resource );

    वापस FT_Err_Ok;
  }


  अटल लघु
  गिनती_faces( Handle        fond,
               स्थिर UInt8*  pathname )
  {
    ResID     sfnt_id;
    लघु     have_sfnt, have_lwfn;
    Str255    lwfn_file_name;
    UInt8     buff[PATH_MAX];
    FT_Error  err;
    लघु     अंक_faces;


    have_sfnt = have_lwfn = 0;

    parse_fond( *fond, &have_sfnt, &sfnt_id, lwfn_file_name, 0 );

    अगर ( lwfn_file_name[0] )
    {
      err = lookup_lwfn_by_fond( pathname, lwfn_file_name,
                                 buff, माप ( buff ) );
      अगर ( !err )
        have_lwfn = 1;
    }

    अगर ( have_lwfn && ( !have_sfnt || PREFER_LWFN ) )
      अंक_faces = 1;
    अन्यथा
      अंक_faces = गिनती_faces_scalable( *fond );

    वापस अंक_faces;
  }


  /* Read Type 1 data from the POST resources inside the LWFN file,
     return a PFB buffer.  This is somewhat convoluted because the FT2
     PFB parser wants the ASCII header as one chunk, and the LWFN
     chunks are often not organized that way, so we glue chunks
     of the same type together. */
  अटल FT_Error
  read_lwfn( FT_Memory      memory,
             Res_File_Ref_अंक  res,
             FT_Byte**      pfb_तथ्य,
             FT_ULong*      आकार )
  {
    FT_Error       error = FT_Err_Ok;
    ResID          res_id;
    अचिन्हित अक्षर  *बफर, *p, *आकार_p = शून्य;
    FT_ULong       योग_आकार = 0;
    FT_ULong       old_योग_आकार = 0;
    FT_ULong       post_आकार, pfb_chunk_आकार;
    Handle         post_तथ्य;
    अक्षर           code, last_code;


    UseResFile( res );

    /* First pass: load all POST resources, and determine the size of */
    /* the output buffer.                                             */
    res_id    = 501;
    last_code = -1;

    क्रम (;;)
    {
      post_तथ्य = Get1Resource( TTAG_POST, res_id++ );
      अगर ( !post_तथ्य )
        अवरोध;  /* we are done */

      code = (*post_तथ्य)[0];

      अगर ( code != last_code )
      {
        अगर ( code == 5 )
          योग_आकार += 2; /* just the end code */
        अन्यथा
          योग_आकार += 6; /* code + 4 bytes chunk length */
      }

      योग_आकार += (FT_ULong)Get_Handle_आकार( post_तथ्य ) - 2;
      last_code = code;

      /* detect resource fork overflow */
      अगर ( FT_MAC_RFORK_MAX_LEN < योग_आकार )
      {
        error = FT_THROW( Array_Too_Large );
        जाओ Error;
      }

      old_योग_आकार = योग_आकार;
    }

    अगर ( FT_QALLOC( बफर, (FT_Long)योग_आकार ) )
      जाओ Error;

    /* Second pass: append all POST data to the buffer, add PFB fields. */
    /* Glue all consecutive chunks of the same type together.           */
    p              = बफर;
    res_id         = 501;
    last_code      = -1;
    pfb_chunk_आकार = 0;

    क्रम (;;)
    {
      post_तथ्य = Get1Resource( TTAG_POST, res_id++ );
      अगर ( !post_तथ्य )
        अवरोध;  /* we are done */

      post_आकार = (FT_ULong)Get_Handle_आकार( post_तथ्य ) - 2;
      code = (*post_तथ्य)[0];

      अगर ( code != last_code )
      {
        अगर ( last_code != -1 )
        {
          /* we are done adding a chunk, fill in the size field */
          अगर ( आकार_p )
          {
            *आकार_p++ = (FT_Byte)(   pfb_chunk_आकार         & 0xFF );
            *आकार_p++ = (FT_Byte)( ( pfb_chunk_आकार >> 8  ) & 0xFF );
            *आकार_p++ = (FT_Byte)( ( pfb_chunk_आकार >> 16 ) & 0xFF );
            *आकार_p++ = (FT_Byte)( ( pfb_chunk_आकार >> 24 ) & 0xFF );
          }
          pfb_chunk_आकार = 0;
        }

        *p++ = 0x80;
        अगर ( code == 5 )
          *p++ = 0x03;  /* the end */
        अन्यथा अगर ( code == 2 )
          *p++ = 0x02;  /* binary segment */
        अन्यथा
          *p++ = 0x01;  /* ASCII segment */

        अगर ( code != 5 )
        {
          आकार_p = p;   /* save for later */
          p += 4;       /* make space for size field */
        }
      }

      ft_memcpy( p, *post_तथ्य + 2, post_आकार );
      pfb_chunk_आकार += post_आकार;
      p += post_आकार;
      last_code = code;
    }

    *pfb_तथ्य = बफर;
    *आकार = योग_आकार;

  Error:
    CloseResFile( res );
    वापस error;
  }


  /* Create a new FT_Face from a file path to an LWFN file. */
  अटल FT_Error
  FT_New_Face_From_LWFN( FT_Library    library,
                         स्थिर UInt8*  pathname,
                         FT_Long       face_सूचकांक,
                         FT_Face*      aface )
  {
    FT_Byte*       pfb_तथ्य;
    FT_ULong       pfb_आकार;
    FT_Error       error;
    Res_File_Ref_अंक  res;


    अगर ( noErr != FT_FSPathMakeRes( pathname, &res ) )
      वापस FT_THROW( Cannot_Open_Resource );

    pfb_तथ्य = शून्य;
    pfb_आकार = 0;
    error = read_lwfn( library->memory, res, &pfb_तथ्य, &pfb_आकार );
    CloseResFile( res ); /* PFB is already loaded, useless anymore */
    अगर ( error )
      वापस error;

    वापस open_face_from_बफर( library,
                                  pfb_तथ्य,
                                  pfb_आकार,
                                  face_सूचकांक,
                                  "type1",
                                  aface );
  }


  /* Create a new FT_Face from an SFNT resource, specified by res ID. */
  अटल FT_Error
  FT_New_Face_From_SFNT( FT_Library  library,
                         ResID       sfnt_id,
                         FT_Long     face_सूचकांक,
                         FT_Face*    aface )
  {
    Handle     sfnt = शून्य;
    FT_Byte*   sfnt_तथ्य;
    माप_प्रकार     sfnt_आकार;
    FT_Error   error  = FT_Err_Ok;
    FT_Memory  memory = library->memory;
    पूर्णांक        is_cff, is_sfnt_ps;


    sfnt = GetResource( TTAG_sfnt, sfnt_id );
    अगर ( !sfnt )
      वापस FT_THROW( Invalid_Handle );

    sfnt_आकार = (FT_ULong)Get_Handle_आकार( sfnt );

    /* detect resource fork overflow */
    अगर ( FT_MAC_RFORK_MAX_LEN < sfnt_आकार )
      वापस FT_THROW( Array_Too_Large );

    अगर ( FT_QALLOC( sfnt_तथ्य, (FT_Long)sfnt_आकार ) )
    {
      ReleaseResource( sfnt );
      वापस error;
    }

    ft_memcpy( sfnt_तथ्य, *sfnt, sfnt_आकार );
    ReleaseResource( sfnt );

    is_cff     = sfnt_आकार > 4 && !ft_memcmp( sfnt_तथ्य, "OTTO", 4 );
    is_sfnt_ps = sfnt_आकार > 4 && !ft_memcmp( sfnt_तथ्य, "typ1", 4 );

    अगर ( is_sfnt_ps )
    {
      FT_Stream  stream;


      अगर ( FT_NEW( stream ) )
        जाओ Try_OpenType;

      FT_Stream_OpenMemory( stream, sfnt_तथ्य, sfnt_आकार );
      अगर ( !open_face_PS_from_sfnt_stream( library,
                                           stream,
                                           face_सूचकांक,
                                           0, शून्य,
                                           aface ) )
      {
        FT_Stream_Close( stream );
        FT_FREE( stream );
        FT_FREE( sfnt_तथ्य );
        जाओ Exit;
      }

      FT_FREE( stream );
    }
  Try_OpenType:
    error = open_face_from_बफर( library,
                                   sfnt_तथ्य,
                                   sfnt_आकार,
                                   face_सूचकांक,
                                   is_cff ? "cff" : "truetype",
                                   aface );
  Exit:
    वापस error;
  }


  /* Create a new FT_Face from a file path to a suitcase file. */
  अटल FT_Error
  FT_New_Face_From_Suitcase( FT_Library    library,
                             स्थिर UInt8*  pathname,
                             FT_Long       face_सूचकांक,
                             FT_Face*      aface )
  {
    FT_Error       error = FT_ERR( Cannot_Open_Resource );
    Res_File_Ref_अंक  res_ref;
    Resource_सूचकांक  res_सूचकांक;
    Handle         fond;
    लघु          अंक_faces_in_res;
    FT_Long        गिनती;


    अगर ( noErr != FT_FSPathMakeRes( pathname, &res_ref ) )
      वापस FT_THROW( Cannot_Open_Resource );

    UseResFile( res_ref );
    अगर ( ResError() )
      वापस FT_THROW( Cannot_Open_Resource );

    res_सूचकांक        = 1;
    अंक_faces_in_res = 0;
    गिनती            = face_सूचकांक;
    जबतक ( गिनती >= 0 )
    {
      लघु  अंक_faces_in_fond;


      fond = Get1IndResource( TTAG_FOND, res_सूचकांक );
      अगर ( ResError() )
        अवरोध;

      अंक_faces_in_fond  = गिनती_faces( fond, pathname );
      अंक_faces_in_res  += अंक_faces_in_fond;

      अगर ( गिनती < अंक_faces_in_fond )
        error = FT_New_Face_From_FOND( library, fond, गिनती, aface );

      res_सूचकांक++;
      गिनती -= अंक_faces_in_fond;
    }

    CloseResFile( res_ref );

    अगर ( !error && aface && *aface )
    {
      (*aface)->अंक_faces  = अंक_faces_in_res;
      (*aface)->face_सूचकांक = face_सूचकांक;
    }

    वापस error;
  }


  /* documentation is in ftmac.h */

  FT_EXPORT_DEF( FT_Error )
  FT_New_Face_From_FOND( FT_Library  library,
                         Handle      fond,
                         FT_Long     face_सूचकांक,
                         FT_Face*    aface )
  {
    लघु     have_sfnt, have_lwfn = 0;
    ResID     sfnt_id, fond_id;
    OSType    fond_type;
    Str255    fond_name;
    Str255    lwfn_file_name;
    UInt8     path_lwfn[PATH_MAX];
    OSErr     err;
    FT_Error  error = FT_Err_Ok;


    /* check of `library' and `aface' delayed to `FT_New_Face_From_XXX' */

    GetResInfo( fond, &fond_id, &fond_type, fond_name );
    अगर ( ResError() != noErr || fond_type != TTAG_FOND )
      वापस FT_THROW( Invalid_File_Format );

    parse_fond( *fond, &have_sfnt, &sfnt_id, lwfn_file_name, face_सूचकांक );

    अगर ( lwfn_file_name[0] )
    {
      Res_File_Ref_अंक  res;


      res = HomeResFile( fond );
      अगर ( noErr != ResError() )
        जाओ found_no_lwfn_file;

      {
        UInt8  path_fond[PATH_MAX];
        FSRef  ref;


        err = FSGetForkCBInfo( res, k_FSInvalid_Volume_Ref_अंक,
                               शून्य, शून्य, शून्य, &ref, शून्य );
        अगर ( noErr != err )
          जाओ found_no_lwfn_file;

        err = FSRefMakePath( &ref, path_fond, माप ( path_fond ) );
        अगर ( noErr != err )
          जाओ found_no_lwfn_file;

        error = lookup_lwfn_by_fond( path_fond, lwfn_file_name,
                                     path_lwfn, माप ( path_lwfn ) );
        अगर ( !error )
          have_lwfn = 1;
      }
    }

    अगर ( have_lwfn && ( !have_sfnt || PREFER_LWFN ) )
      error = FT_New_Face_From_LWFN( library,
                                     path_lwfn,
                                     face_सूचकांक,
                                     aface );
    अन्यथा
      error = FT_THROW( Unknown_File_Format );

  found_no_lwfn_file:
    अगर ( have_sfnt && error )
      error = FT_New_Face_From_SFNT( library,
                                     sfnt_id,
                                     face_सूचकांक,
                                     aface );

    वापस error;
  }


  /* Common function to load a new FT_Face from a resource file. */
  अटल FT_Error
  FT_New_Face_From_Resource( FT_Library    library,
                             स्थिर UInt8*  pathname,
                             FT_Long       face_सूचकांक,
                             FT_Face*      aface )
  {
    OSType    file_type;
    FT_Error  error;


    /* LWFN is a (very) specific file format, check for it explicitly */
    file_type = get_file_type_from_path( pathname );
    अगर ( file_type == TTAG_LWFN )
      वापस FT_New_Face_From_LWFN( library, pathname, face_सूचकांक, aface );

    /* Otherwise the file type doesn't matter (there are more than  */
    /* `FFIL' and `tfil').  Just try opening it as a font suitcase; */
    /* if it works, fine.                                           */

    error = FT_New_Face_From_Suitcase( library, pathname, face_सूचकांक, aface );
    अगर ( error )
    {
      /* let it fall through to normal loader (.ttf, .otf, etc.); */
      /* we signal this by returning no error and no FT_Face      */
      *aface = शून्य;
    }

    वापस FT_Err_Ok;
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
               स्थिर अक्षर*  pathname,
               FT_Long      face_सूचकांक,
               FT_Face*     aface )
  {
    FT_Open_Args  args;
    FT_Error      error;


    /* test for valid `library' and `aface' delayed to FT_Open_Face() */
    अगर ( !pathname )
      वापस FT_THROW( Invalid_Argument );

    *aface = शून्य;

    /* try resourcefork based font: LWFN, FFIL */
    error = FT_New_Face_From_Resource( library, (UInt8 *)pathname,
                                       face_सूचकांक, aface );
    अगर ( error || *aface )
      वापस error;

    /* let it fall through to normal loader (.ttf, .otf, etc.) */
    args.flags    = FT_OPEN_PATHNAME;
    args.pathname = (अक्षर*)pathname;

    वापस FT_Open_Face( library, &args, face_सूचकांक, aface );
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
                          स्थिर FSRef*  ref,
                          FT_Long       face_सूचकांक,
                          FT_Face*      aface )
  {
    FT_Error      error;
    FT_Open_Args  args;

    OSErr  err;
    UInt8  pathname[PATH_MAX];


    /* check of `library' and `aface' delayed to */
    /* `FT_New_Face_From_Resource'               */

    अगर ( !ref )
      वापस FT_THROW( Invalid_Argument );

    err = FSRefMakePath( ref, pathname, माप ( pathname ) );
    अगर ( err )
      error = FT_THROW( Cannot_Open_Resource );

    error = FT_New_Face_From_Resource( library, pathname, face_सूचकांक, aface );
    अगर ( error || *aface )
      वापस error;

    /* fallback to datafork font */
    args.flags    = FT_OPEN_PATHNAME;
    args.pathname = (अक्षर*)pathname;
    वापस FT_Open_Face( library, &args, face_सूचकांक, aface );
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
                           स्थिर FSSpec*  spec,
                           FT_Long        face_सूचकांक,
                           FT_Face*       aface )
  {
#अगर ( __LP64__ ) || ( defined( MAC_OS_X_VERSION_10_5 ) && \
      ( MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_5 ) )
    FT_UNUSED( library );
    FT_UNUSED( spec );
    FT_UNUSED( face_सूचकांक );
    FT_UNUSED( aface );

    वापस FT_THROW( Unimplemented_Feature );
#अन्यथा
    FSRef  ref;


    /* check of `library' and `aface' delayed to `FT_New_Face_From_FSRef' */

    अगर ( !spec || FSpMakeFSRef( spec, &ref ) != noErr )
      वापस FT_THROW( Invalid_Argument );
    अन्यथा
      वापस FT_New_Face_From_FSRef( library, &ref, face_सूचकांक, aface );
#पूर्ण_अगर
  }

#अन्यथा /* !FT_MACINTOSH */

  /* ANSI C doesn't like empty source files */
  प्रकार पूर्णांक  ft_mac_dummy_;

#पूर्ण_अगर /* !FT_MACINTOSH */


/* END */
