/****************************************************************************
 *
 * ftbase.h
 *
 *   Private functions used in the `base' module (specification).
 *
 * Copyright (C) 2008-2026 by
 * David Turner, Robert Wilhelm, Werner Lemberg, and suzuki toshiya.
 *
 * This file is part of the FreeType project, and may only be used,
 * modified, and distributed under the terms of the FreeType project
 * license, LICENSE.TXT.  By continuing to use, modify, or distribute
 * this file you indicate that you have read the license and
 * understand and accept it fully.
 *
 */


#यदि_अपरिभाषितम् FTBASE_H_
#परिभाषय FTBASE_H_


#अन्तर्भाव <freetype/internal/ftobjs.h>


FT_BEGIN_शीर्षकम्


  FT_DECLARE_GLYPH( ft_bitmap_glyph_class )
  FT_DECLARE_GLYPH( ft_outline_glyph_class )
  FT_DECLARE_GLYPH( ft_svg_glyph_class )


#यदि_परिभाषितम् FT_CONFIG_OPTION_MAC_FONTS

  /* MacOS resource fork cannot exceed 16MB at least for Carbon code; */
  /* see https://jeffpar.github.io/kbarchive/kb/130/Q130437/          */
#परिभाषय FT_MAC_RFORK_MAX_LEN  0x00FFFFFFUL


  /* Assume the stream is sfnt-wrapped PS Type1 or sfnt-wrapped CID-keyed */
  /* font, and try to load a face specified by the face_index.            */
  FT_LOCAL( FT_Error )
  open_face_PS_from_sfnt_प्रवाहः( FT_Library     library,
                                 FT_प्रवाहः      प्रवाहः,
                                 FT_Long        face_सूचकांक,
                                 FT_Int         अङ्कः_params,
                                 FT_Parameter  *params,
                                 FT_Face       *aface );


  /* Create a new FT_Face given a buffer and a driver name. */
  /* From ftmac.c.                                          */
  FT_LOCAL( FT_Error )
  open_face_from_बफर( FT_Library   library,
                         FT_Byte*     आधारः,
                         FT_ULong     आकार,
                         FT_Long      face_सूचकांक,
                         ध्रुव वर्ण*  driver_name,
                         FT_Face     *aface );


#यदि  defined( FT_CONFIG_OPTION_GUESSING_EMBEDDED_RFORK ) && \
    !defined( FT_MACINTOSH )
  /* Mac OS X/Darwin kernel often changes recommended method to access */
  /* the resource fork and older methods makes the kernel issue the    */
  /* warning of deprecated method.  To calm it down, the methods based */
  /* on Darwin VFS should be grouped and skip the rest methods after   */
  /* the case the resource is opened but found to lack a font in it.   */
  FT_LOCAL( FT_Bool )
  ft_raccess_नियमः_by_darwin_vfs( FT_Library library, FT_UInt  नियमः_सूचकांक );
#यदि_अन्तः

#यदि_अन्तः /* FT_CONFIG_OPTION_MAC_FONTS */


FT_अन्तः_शीर्षकम्

#यदि_अन्तः /* FTBASE_H_ */


/*अन्तः*/
