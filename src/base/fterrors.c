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


#അന്തര്ഭാവ <freetype/internal/ftdebug.h>
#അന്തര്ഭാവ <freetype/fterrors.h>


  /* documentation is in fterrors.h */

  FT_EXPORT_DEF( സ്ഥിരം അക്ഷരം* )
  FT_Error_String( FT_Error  error_code )
  {
    എങ്കിൽ ( error_code <  0                                ||
         error_code >= FT_ERR_CAT( FT_ERR_PREFIX, Max ) )
      തിരികെ ശൂന്യ;

#എങ്കിൽ defined( FT_CONFIG_OPTION_ERROR_STRINGS ) || \
    defined( FT_DEBUG_LEVEL_ERROR )

#undef FTERRORS_H_
#define FT_ERROR_START_LIST     തിരഞ്ഞെടുപ്പ് ( FT_ERROR_BASE( error_code ) ) {
#define FT_ERRORDEF( e, v, s )    അവസ്ഥ v: തിരികെ s;
#define FT_ERROR_END_LIST       }

#അന്തര്ഭാവ <freetype/fterrors.h>

#endif /* defined( FT_CONFIG_OPTION_ERROR_STRINGS ) || ... */

    തിരികെ ശൂന്യ;
  }
