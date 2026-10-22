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


#اگر_تعریف_نشده FTBASE_H_
#تعریف_کن FTBASE_H_


#ضمن <freetype/internal/ftobjs.h>


FT_BEGIN_سرآیند


  FT_DECLARE_GLYPH( ft_bitmap_glyph_class )
  FT_DECLARE_GLYPH( ft_outline_glyph_class )
  FT_DECLARE_GLYPH( ft_svg_glyph_class )


#اگر_تعریف_شده FT_CONFIG_OPTION_MAC_FONTS

  /* MacOS resource fork cannot exceed 16MB at least for Carbon code; */
  /* see https://jeffpar.github.io/kbarchive/kb/130/Q130437/          */
#تعریف_کن FT_MAC_RFORK_MAX_LEN  0x00FFFFFFUL


  /* Assume the stream is sfnt-wrapped PS Type1 or sfnt-wrapped CID-keyed */
  /* font, and try to load a face specified by the face_index.            */
  FT_LOCAL( FT_Error )
  open_face_PS_from_sfnt_جریان( FT_Library     library,
                                 FT_جریان      جریان,
                                 FT_Long        face_شاخص,
                                 FT_Int         رقم_params,
                                 FT_Parameter  *params,
                                 FT_Face       *aface );


  /* Create a new FT_Face given a buffer and a driver name. */
  /* From ftmac.c.                                          */
  FT_LOCAL( FT_Error )
  open_face_from_میانگیر( FT_Library   library,
                         FT_Byte*     پایه,
                         FT_ULong     اندازه,
                         FT_Long      face_شاخص,
                         ثابت حرف*  driver_name,
                         FT_Face     *aface );


#إذا  defined( FT_CONFIG_OPTION_GUESSING_EMBEDDED_RFORK ) && \
    !defined( FT_MACINTOSH )
  /* Mac OS X/Darwin kernel often changes recommended method to access */
  /* the resource fork and older methods makes the kernel issue the    */
  /* warning of deprecated method.  To calm it down, the methods based */
  /* on Darwin VFS should be grouped and skip the rest methods after   */
  /* the case the resource is opened but found to lack a font in it.   */
  FT_LOCAL( FT_Bool )
  ft_raccess_قاعده_by_darwin_vfs( FT_Library library, FT_UInt  قاعده_شاخص );
#پایان_اگر

#پایان_اگر /* FT_CONFIG_OPTION_MAC_FONTS */


FT_انتها_سرآیند

#پایان_اگر /* FTBASE_H_ */


/*انتها*/
