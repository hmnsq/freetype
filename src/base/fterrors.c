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


#అన్తర్భావ <freetype/internal/ftdebug.h>
#అన్తర్భావ <freetype/fterrors.h>


  /* documentation is in fterrors.h */

  FT_EXPORT_DEF( స్థిరరాశి అక్షరం* )
  FT_Error_String( FT_Error  error_సంకేతం )
  {
    ఒకవేళ ( error_సంకేతం <  0                                ||
         error_సంకేతం >= FT_ERR_CAT( FT_ERR_ఉపసర్గ, Max ) )
      తిరిగి ఖాళీ;

#ఒకవేళ defined( FT_CONFIG_OPTION_ERROR_అక్షరశ్రేణులు ) || \
    defined( FT_DEBUG_LEVEL_ERROR )

#నిర్వచనం_రద్దు FTERRORS_H_
#నిర్వచించు FT_ERROR_ప్రారంభం_LIST     ఎంపిక ( FT_ERROR_పునాది( error_సంకేతం ) ) {
#నిర్వచించు FT_ERRORDEF( e, v, s )    స్థితి v: తిరిగి s;
#నిర్వచించు FT_ERROR_చివర_LIST       }

#అన్తర్భావ <freetype/fterrors.h>

#అయితే_ముగింపు /* defined( FT_CONFIG_OPTION_ERROR_STRINGS ) || ... */

    తిరిగి ఖాళీ;
  }
