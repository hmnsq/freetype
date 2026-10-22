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


#ಅನ್ತರ್ಭಾವ <freetype/internal/ftdebug.h>
#ಅನ್ತರ್ಭಾವ <freetype/fterrors.h>


  /* documentation is in fterrors.h */

  FT_EXPORT_DEF( ಧ್ರುವ ವರ್ಣ* )
  FT_Error_String( FT_Error  error_code )
  {
    ಯದಿ ( error_code <  0                                ||
         error_code >= FT_ERR_CAT( FT_ERR_PREFIX, Max ) )
      ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

#ಯದಿ defined( FT_CONFIG_OPTION_ERROR_STRINGS ) || \
    defined( FT_DEBUG_LEVEL_ERROR )

#undef FTERRORS_H_
#define FT_ERROR_START_LIST     ವಿಕಲ್ಪ ( FT_ERROR_BASE( error_code ) ) {
#define FT_ERRORDEF( e, v, s )    ಪಕ್ಷ v: ಪ್ರತ್ಯಾಗಮ s;
#define FT_ERROR_END_LIST       }

#ಅನ್ತರ್ಭಾವ <freetype/fterrors.h>

#endif /* defined( FT_CONFIG_OPTION_ERROR_STRINGS ) || ... */

    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
  }
