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


#ଅନ୍ତର୍ଭାଵ <freetype/internal/ftdebug.h>
#ଅନ୍ତର୍ଭାଵ <freetype/fterrors.h>


  /* documentation is in fterrors.h */

  FT_EXPORT_DEF( ଅଚଳ ଅକ୍ଷର* )
  FT_Error_String( FT_Error  error_code )
  {
    ଯଦି ( error_code <  0                                ||
         error_code >= FT_ERR_CAT( FT_ERR_PREFIX, Max ) )
      ଫେରନ୍ତୁ ଖାଲି;

#ଯଦି defined( FT_CONFIG_OPTION_ERROR_STRINGS ) || \
    defined( FT_DEBUG_LEVEL_ERROR )

#undef FTERRORS_H_
#define FT_ERROR_START_LIST     ବାଛ ( FT_ERROR_BASE( error_code ) ) {
#define FT_ERRORDEF( e, v, s )    ସ୍ଥିତି v: ଫେରନ୍ତୁ s;
#define FT_ERROR_END_LIST       }

#ଅନ୍ତର୍ଭାଵ <freetype/fterrors.h>

#endif /* defined( FT_CONFIG_OPTION_ERROR_STRINGS ) || ... */

    ଫେରନ୍ତୁ ଖାଲି;
  }
