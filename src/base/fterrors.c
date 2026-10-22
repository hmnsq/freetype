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


#समावेश <freetype/internal/ftdebug.h>
#समावेश <freetype/fterrors.h>


  /* documentation is in fterrors.h */

  FT_EXPORT_DEF( स्थिर अक्षर* )
  FT_Error_String( FT_Error  error_code )
  {
    अगर ( error_code <  0                                ||
         error_code >= FT_ERR_CAT( FT_ERR_PREFIX, Max ) )
      वापस शून्य;

#अगर defined( FT_CONFIG_OPTION_ERROR_STRINGS ) || \
    defined( FT_DEBUG_LEVEL_ERROR )

#अघोषित FTERRORS_H_
#घोषणा FT_ERROR_START_LIST     चयन ( FT_ERROR_BASE( error_code ) ) {
#घोषणा FT_ERRORDEF( e, v, s )    हाल v: वापस s;
#घोषणा FT_ERROR_END_LIST       }

#समावेश <freetype/fterrors.h>

#पूर्ण_अगर /* defined( FT_CONFIG_OPTION_ERROR_STRINGS ) || ... */

    वापस शून्य;
  }
