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

#شامل <freetype/internal/ftdebug.h>

#شامل <freetype/internal/ftobjs.h>
#شامل <freetype/internal/services/svpfr.h>


  /* check the format */
  ایستا FT_Service_PfrMetrics
  ft_pfr_check( FT_Face  face )
  {
    FT_Service_PfrMetrics  service = پوچ;


    اگر ( face )
      FT_FACE_LOOKUP_SERVICE( face, service, PFR_METRICS );

    بازگشت service;
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


    اگر ( !face )
      بازگشت FT_THROW( Invalid_Face_Handle );

    service = ft_pfr_check( face );
    اگر ( service )
    {
      error = service->get_metrics( face,
                                    aoutline_resolution,
                                    ametrics_resolution,
                                    ametrics_x_scale,
                                    ametrics_y_scale );
    }
    وگرنه
    {
      FT_Fixed  x_scale, y_scale;


      /* this is not a PFR font */
      اگر ( aoutline_resolution )
        *aoutline_resolution = face->units_per_EM;

      اگر ( ametrics_resolution )
        *ametrics_resolution = face->units_per_EM;

      x_scale = y_scale = 0x10000L;
      اگر ( face->size )
      {
        x_scale = face->size->metrics.x_scale;
        y_scale = face->size->metrics.y_scale;
      }

      اگر ( ametrics_x_scale )
        *ametrics_x_scale = x_scale;

      اگر ( ametrics_y_scale )
        *ametrics_y_scale = y_scale;

      error = FT_THROW( Unknown_File_Format );
    }

    بازگشت error;
  }


  /* documentation is in ftpfr.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_PFR_Kerning( FT_Face     face,
                      FT_UInt     چپ,
                      FT_UInt     راست,
                      FT_Vector  *avector )
  {
    FT_Error               error;
    FT_Service_PfrMetrics  service;


    اگر ( !face )
      بازگشت FT_THROW( Invalid_Face_Handle );

    اگر ( !avector )
      بازگشت FT_THROW( Invalid_Argument );

    service = ft_pfr_check( face );
    اگر ( service )
      error = service->get_kerning( face, چپ, راست, avector );
    وگرنه
      error = FT_Get_Kerning( face, چپ, راست,
                              FT_KERNING_UNSCALED, avector );

    بازگشت error;
  }


  /* documentation is in ftpfr.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_PFR_Advance( FT_Face   face,
                      FT_UInt   gindex,
                      FT_Pos   *aadvance )
  {
    FT_Error               error;
    FT_Service_PfrMetrics  service;


    اگر ( !face )
      بازگشت FT_THROW( Invalid_Face_Handle );

    اگر ( !aadvance )
      بازگشت FT_THROW( Invalid_Argument );

    service = ft_pfr_check( face );
    اگر ( service )
      error = service->get_advance( face, gindex, aadvance );
    وگرنه
      /* XXX: TODO: PROVIDE ADVANCE-LOADING METHOD TO ALL FONT DRIVERS */
      error = FT_THROW( Invalid_Argument );

    بازگشت error;
  }


/* END */
