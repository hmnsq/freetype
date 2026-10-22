/****************************************************************************
 *
 * ftpfr.c
 *
 *   FreeType API for accessing PFR-specific data (body).
 *
 * Copyright (C) 2002-2026 by
 * David Turner, Robert Wilhelm, and Werner Lemberg.
 *
 * This file is part of the FreeType project, and may only be used,
 * modified, and distributed under the terms of the FreeType project
 * license, LICENSE.TXT.  By continuing to use, modify, or distribute
 * this file you indicate that you have read the license and
 * understand and accept it fully.
 *
 */

#അന്തര്ഭാവ <freetype/internal/ftdebug.h>

#അന്തര്ഭാവ <freetype/internal/ftobjs.h>
#അന്തര്ഭാവ <freetype/internal/services/svpfr.h>


  /* check the format */
  സ്ഥിതം FT_Service_PfrMetrics
  ft_pfr_check( FT_Face  face )
  {
    FT_Service_PfrMetrics  service = ശൂന്യ;


    എങ്കിൽ ( face )
      FT_FACE_LOOKUP_SERVICE( face, service, PFR_METRICS );

    തിരികെ service;
  }


  /* documentation is in ftpfr.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_PFR_Metrics( FT_Face    face,
                      FT_UInt   *aoutline_resolution,
                      FT_UInt   *ametrics_resolution,
                      FT_Fixed  *ametrics_x_scale,
                      FT_Fixed  *ametrics_y_scale )
  {
    FT_Error               error = FT_Err_Ok;
    FT_Service_PfrMetrics  service;


    എങ്കിൽ ( !face )
      തിരികെ FT_THROW( Invalid_Face_Handle );

    service = ft_pfr_check( face );
    എങ്കിൽ ( service )
    {
      error = service->get_metrics( face,
                                    aoutline_resolution,
                                    ametrics_resolution,
                                    ametrics_x_scale,
                                    ametrics_y_scale );
    }
    അല്ലെങ്കിൽ
    {
      FT_Fixed  x_scale, y_scale;


      /* this is not a PFR font */
      എങ്കിൽ ( aoutline_resolution )
        *aoutline_resolution = face->units_per_EM;

      എങ്കിൽ ( ametrics_resolution )
        *ametrics_resolution = face->units_per_EM;

      x_scale = y_scale = 0x10000L;
      എങ്കിൽ ( face->size )
      {
        x_scale = face->size->metrics.x_scale;
        y_scale = face->size->metrics.y_scale;
      }

      എങ്കിൽ ( ametrics_x_scale )
        *ametrics_x_scale = x_scale;

      എങ്കിൽ ( ametrics_y_scale )
        *ametrics_y_scale = y_scale;

      error = FT_THROW( Unknown_File_Format );
    }

    തിരികെ error;
  }


  /* documentation is in ftpfr.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_PFR_Kerning( FT_Face     face,
                      FT_UInt     ഇടത്,
                      FT_UInt     വലത്,
                      FT_Vector  *avector )
  {
    FT_Error               error;
    FT_Service_PfrMetrics  service;


    എങ്കിൽ ( !face )
      തിരികെ FT_THROW( Invalid_Face_Handle );

    എങ്കിൽ ( !avector )
      തിരികെ FT_THROW( Invalid_Argument );

    service = ft_pfr_check( face );
    എങ്കിൽ ( service )
      error = service->get_kerning( face, ഇടത്, വലത്, avector );
    അല്ലെങ്കിൽ
      error = FT_Get_Kerning( face, ഇടത്, വലത്,
                              FT_KERNING_UNSCALED, avector );

    തിരികെ error;
  }


  /* documentation is in ftpfr.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_PFR_Advance( FT_Face   face,
                      FT_UInt   gindex,
                      FT_Pos   *aadvance )
  {
    FT_Error               error;
    FT_Service_PfrMetrics  service;


    എങ്കിൽ ( !face )
      തിരികെ FT_THROW( Invalid_Face_Handle );

    എങ്കിൽ ( !aadvance )
      തിരികെ FT_THROW( Invalid_Argument );

    service = ft_pfr_check( face );
    എങ്കിൽ ( service )
      error = service->get_advance( face, gindex, aadvance );
    അല്ലെങ്കിൽ
      /* XXX: TODO: PROVIDE ADVANCE-LOADING METHOD TO ALL FONT DRIVERS */
      error = FT_THROW( Invalid_Argument );

    തിരികെ error;
  }


/* END */
