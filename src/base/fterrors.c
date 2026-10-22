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
  FT_Error_String( FT_Error  error_संहिता )
  {
    यदि ( error_संहिता <  0                                ||
         error_संहिता >= FT_ERR_CAT( FT_ERR_उपसर्गः, Max ) )
      प्रत्यागम रिक्त;

#यदि defined( FT_CONFIG_OPTION_ERROR_वाक्यानि ) || \
    defined( FT_DEBUG_LEVEL_ERROR )

#अपरिभाषय FTERRORS_H_
#परिभाषय FT_ERROR_START_LIST     विकल्प ( FT_ERROR_आधारः( error_संहिता ) ) {
#परिभाषय FT_ERRORDEF( e, v, s )    पक्ष v: प्रत्यागम s;
#परिभाषय FT_ERROR_अन्तः_LIST       }

#अन्तर्भाव <freetype/fterrors.h>

#यदि_अन्तः /* defined( FT_CONFIG_OPTION_ERROR_STRINGS ) || ... */

    प्रत्यागम रिक्त;
  }
