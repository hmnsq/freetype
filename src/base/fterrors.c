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


#அந்தர்பாவ <freetype/internal/ftdebug.h>
#அந்தர்பாவ <freetype/fterrors.h>


  /* documentation is in fterrors.h */

  FT_EXPORT_DEF( மாறிலி எழுத்து* )
  FT_Error_String( FT_Error  error_code )
  {
    எனில் ( error_code <  0                                ||
         error_code >= FT_ERR_CAT( FT_ERR_PREFIX, Max ) )
      திரும்பு வெற்று;

#எனில் defined( FT_CONFIG_OPTION_ERROR_STRINGS ) || \
    defined( FT_DEBUG_LEVEL_ERROR )

#undef FTERRORS_H_
#define FT_ERROR_START_LIST     தேர்வு ( FT_ERROR_BASE( error_code ) ) {
#define FT_ERRORDEF( e, v, s )    நிலை v: திரும்பு s;
#define FT_ERROR_END_LIST       }

#அந்தர்பாவ <freetype/fterrors.h>

#endif /* defined( FT_CONFIG_OPTION_ERROR_STRINGS ) || ... */

    திரும்பு வெற்று;
  }
