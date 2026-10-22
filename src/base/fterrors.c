/****************************************************************************
 *
 * fterrors.c
 *
 *   FreeType API for error code handling.
 *
 * Copyright (C) 2018-2026 by
 * Armin Hasitzka, David Turner, Robert Wilhelm, and Werner Lemberg.
 *
 * This file is part of the FreeType project, and may only be used,
 * modified, and distributed under the terms of the FreeType project
 * license, LICENSE.TXT.  By continuing to use, modify, or distribute
 * this file you indicate that you have read the license and
 * understand and accept it fully.
 *
 */


#ضمن <freetype/internal/ftdebug.h>
#ضمن <freetype/fterrors.h>


  /* documentation is in fterrors.h */

  FT_EXPORT_DEF( ثابت حرف* )
  FT_Error_String( FT_Error  error_ڪوڊ )
  {
    إذا ( error_ڪوڊ <  0                                ||
         error_ڪوڊ >= FT_ERR_CAT( FT_ERR_اڳياڙي, Max ) )
      أرجع عدم;

#إذا defined( FT_CONFIG_OPTION_ERROR_لڙيون ) || \
    defined( FT_DEBUG_LEVEL_ERROR )

#وصف_رد FTERRORS_H_
#وصف_ڪريو FT_ERROR_شروعات_LIST     بدل ( FT_ERROR_بنياد( error_ڪوڊ ) ) {
#وصف_ڪريو FT_ERRORDEF( e, v, s )    حالة v: أرجع s;
#وصف_ڪريو FT_ERROR_پڇاڙي_LIST       }

#ضمن <freetype/fterrors.h>

#جيڪڏهن_پورو /* defined( FT_CONFIG_OPTION_ERROR_STRINGS ) || ... */

    أرجع عدم;
  }
