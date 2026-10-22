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
  FT_Error_String( FT_Error  error_کوډ )
  {
    إذا ( error_کوډ <  0                                ||
         error_کوډ >= FT_ERR_CAT( FT_ERR_مختاړی, Max ) )
      أرجع عدم;

#إذا defined( FT_CONFIG_OPTION_ERROR_مزي_ګانې ) || \
    defined( FT_DEBUG_LEVEL_ERROR )

#تعریف_لغوه FTERRORS_H_
#تعریف_کړه FT_ERROR_پیل_LIST     بدل ( FT_ERROR_بنسټ( error_کوډ ) ) {
#تعریف_کړه FT_ERRORDEF( e, v, s )    حالة v: أرجع s;
#تعریف_کړه FT_ERROR_پای_ټکی_LIST       }

#ضمن <freetype/fterrors.h>

#د_که_پای /* defined( FT_CONFIG_OPTION_ERROR_STRINGS ) || ... */

    أرجع عدم;
  }
