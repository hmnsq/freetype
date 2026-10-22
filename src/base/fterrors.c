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


#અન્તર્ભાવ <freetype/internal/ftdebug.h>
#અન્તર્ભાવ <freetype/fterrors.h>


  /* documentation is in fterrors.h */

  FT_EXPORT_DEF( અચલ અક્ષર* )
  FT_Error_String( FT_Error  error_code )
  {
    જો ( error_code <  0                                ||
         error_code >= FT_ERR_CAT( FT_ERR_PREFIX, Max ) )
      પાછા ખાલી;

#જો defined( FT_CONFIG_OPTION_ERROR_STRINGS ) || \
    defined( FT_DEBUG_LEVEL_ERROR )

#undef FTERRORS_H_
#define FT_ERROR_START_LIST     પસંદગી ( FT_ERROR_BASE( error_code ) ) {
#define FT_ERRORDEF( e, v, s )    સ્થિતિ v: પાછા s;
#define FT_ERROR_END_LIST       }

#અન્તર્ભાવ <freetype/fterrors.h>

#endif /* defined( FT_CONFIG_OPTION_ERROR_STRINGS ) || ... */

    પાછા ખાલી;
  }
