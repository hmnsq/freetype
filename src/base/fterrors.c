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


#अन्तर्भाव <freetype/internal/ftdebug.h>
#अन्तर्भाव <freetype/fterrors.h>


  /* documentation is in fterrors.h */

  FT_EXPORT_DEF( ध्रुव वर्ण* )
  FT_Error_String( FT_Error  error_code )
  {
    सचे ( error_code <  0                                ||
         error_code >= FT_ERR_CAT( FT_ERR_PREFIX, Max ) )
      पटिनिवत्त रिक्त;

#सचे defined( FT_CONFIG_OPTION_ERROR_STRINGS ) || \
    defined( FT_DEBUG_LEVEL_ERROR )

#undef FTERRORS_H_
#define FT_ERROR_START_LIST     विकल्प ( FT_ERROR_BASE( error_code ) ) {
#define FT_ERRORDEF( e, v, s )    पक्ष v: पटिनिवत्त s;
#define FT_ERROR_END_LIST       }

#अन्तर्भाव <freetype/fterrors.h>

#endif /* defined( FT_CONFIG_OPTION_ERROR_STRINGS ) || ... */

    पटिनिवत्त रिक्त;
  }
