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


#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftdebug.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/fterrors.h>


  /* documentation is in fterrors.h */

  FT_EXPORT_DEF( ਅਚਲ ਅੱਖਰ* )
  FT_Error_String( FT_Error  error_ਕੋਡ )
  {
    ਜੇ ( error_ਕੋਡ <  0                                ||
         error_ਕੋਡ >= FT_ERR_CAT( FT_ERR_ਅਗੇਤਰ, Max ) )
      ਵਾਪਸ ਨਿਰਾ;

#ਜੇ defined( FT_CONFIG_OPTION_ERROR_ਲੜੀਆਂ ) || \
    defined( FT_DEBUG_LEVEL_ERROR )

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ FTERRORS_H_
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FT_ERROR_ਸ਼ੁਰੂ_LIST     ਚੋਣ ( FT_ERROR_ਅਧਾਰ( error_ਕੋਡ ) ) {
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FT_ERRORDEF( e, v, s )    ਹਾਲਤ v: ਵਾਪਸ s;
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FT_ERROR_ਸਿਰਾ_LIST       }

#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/fterrors.h>

#ਜੇ_ਅੰਤ /* defined( FT_CONFIG_OPTION_ERROR_STRINGS ) || ... */

    ਵਾਪਸ ਨਿਰਾ;
  }
