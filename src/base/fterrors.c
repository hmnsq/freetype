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

  FT_EXPORT_DEF( ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* )
  FT_Error_String( FT_Error  error_ಸಂಕೇತ )
  {
    ಆದರೆ ( error_ಸಂಕೇತ <  0                                ||
         error_ಸಂಕೇತ >= FT_ERR_CAT( FT_ERR_ಪೂರ್ವಪ್ರತ್ಯಯ, Max ) )
      ಹಿಂತಿರುಗಿ ಖಾಲಿ;

#ಆದರೆ defined( FT_CONFIG_OPTION_ERROR_ಅಕ್ಷರಸರಣಿಗಳು ) || \
    defined( FT_DEBUG_LEVEL_ERROR )

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು FTERRORS_H_
#ವ್ಯಾಖ್ಯಾನಿಸು FT_ERROR_ಆರಂಭ_LIST     ಆಯ್ಕೆ ( FT_ERROR_ತಳ( error_ಸಂಕೇತ ) ) {
#ವ್ಯಾಖ್ಯಾನಿಸು FT_ERRORDEF( e, v, s )    ಸ್ಥಿತಿ v: ಹಿಂತಿರುಗಿ s;
#ವ್ಯಾಖ್ಯಾನಿಸು FT_ERROR_ಕೊನೆ_LIST       }

#ಅನ್ತರ್ಭಾವ <freetype/fterrors.h>

#ಆದರೆ_ಅಂತ್ಯ /* defined( FT_CONFIG_OPTION_ERROR_STRINGS ) || ... */

    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
  }
