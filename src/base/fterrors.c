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


#شامل <freetype/internal/ftdebug.h>
#شامل <freetype/fterrors.h>


  /* documentation is in fterrors.h */

  FT_EXPORT_DEF( ثابت حرف* )
  FT_Error_String( FT_Error  error_رمز )
  {
    اگر ( error_رمز <  0                                ||
         error_رمز >= FT_ERR_CAT( FT_ERR_سابقہ, Max ) )
      واپس تہی;

#اگر defined( FT_CONFIG_OPTION_ERROR_لڑیاں ) || \
    defined( FT_DEBUG_LEVEL_ERROR )

#تعریف_منسوخ FTERRORS_H_
#تعریف_کرو FT_ERROR_آغاز_LIST     انتخاب ( FT_ERROR_بنیاد( error_رمز ) ) {
#تعریف_کرو FT_ERRORDEF( e, v, s )    حالت v: واپس s;
#تعریف_کرو FT_ERROR_انتہا_LIST       }

#شامل <freetype/fterrors.h>

#اگر_ختم /* defined( FT_CONFIG_OPTION_ERROR_STRINGS ) || ... */

    واپس تہی;
  }
