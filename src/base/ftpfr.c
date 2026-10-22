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

#include <freetype/internal/ftdebug.h>

#include <freetype/internal/ftobjs.h>
#include <freetype/internal/services/svpfr.h>


  /* check the format */
  static FT_Service_PfrMetrics
  ft_pfr_check( FT_Face  face )
  {
    FT_Service_PfrMetrics  service = NULL;


    ប្រសិនបើ ( face )
      FT_FACE_LOOKUP_SERVICE( face, service, PFR_METRICS );

    ត្រឡប់ service;
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


    ប្រសិនបើ ( !face )
      ត្រឡប់ FT_THROW( Invalid_Face_Handle );

    service = ft_pfr_check( face );
    ប្រសិនបើ ( service )
    {
      error = service->get_metrics( face,
                                    aoutline_resolution,
                                    ametrics_resolution,
                                    ametrics_x_scale,
                                    ametrics_y_scale );
    }
    បើមិនដូច្នេះ
    {
      FT_Fixed  x_scale, y_scale;


      /* this is not a PFR font */
      ប្រសិនបើ ( aoutline_resolution )
        *aoutline_resolution = face->units_per_EM;

      ប្រសិនបើ ( ametrics_resolution )
        *ametrics_resolution = face->units_per_EM;

      x_scale = y_scale = 0x10000L;
      ប្រសិនបើ ( face->size )
      {
        x_scale = face->size->metrics.x_scale;
        y_scale = face->size->metrics.y_scale;
      }

      ប្រសិនបើ ( ametrics_x_scale )
        *ametrics_x_scale = x_scale;

      ប្រសិនបើ ( ametrics_y_scale )
        *ametrics_y_scale = y_scale;

      error = FT_THROW( Unknown_File_Format );
    }

    ត្រឡប់ error;
  }


  /* documentation is in ftpfr.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_PFR_Kerning( FT_Face     face,
                      FT_UInt     left,
                      FT_UInt     right,
                      FT_Vector  *avector )
  {
    FT_Error               error;
    FT_Service_PfrMetrics  service;


    ប្រសិនបើ ( !face )
      ត្រឡប់ FT_THROW( Invalid_Face_Handle );

    ប្រសិនបើ ( !avector )
      ត្រឡប់ FT_THROW( Invalid_Argument );

    service = ft_pfr_check( face );
    ប្រសិនបើ ( service )
      error = service->get_kerning( face, left, right, avector );
    បើមិនដូច្នេះ
      error = FT_Get_Kerning( face, left, right,
                              FT_KERNING_UNSCALED, avector );

    ត្រឡប់ error;
  }


  /* documentation is in ftpfr.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_PFR_Advance( FT_Face   face,
                      FT_UInt   gindex,
                      FT_Pos   *aadvance )
  {
    FT_Error               error;
    FT_Service_PfrMetrics  service;


    ប្រសិនបើ ( !face )
      ត្រឡប់ FT_THROW( Invalid_Face_Handle );

    ប្រសិនបើ ( !aadvance )
      ត្រឡប់ FT_THROW( Invalid_Argument );

    service = ft_pfr_check( face );
    ប្រសិនបើ ( service )
      error = service->get_advance( face, gindex, aadvance );
    បើមិនដូច្នេះ
      /* XXX: TODO: PROVIDE ADVANCE-LOADING METHOD TO ALL FONT DRIVERS */
      error = FT_THROW( Invalid_Argument );

    ត្រឡប់ error;
  }


/* END */
