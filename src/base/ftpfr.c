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

#అన్తర్భావ <freetype/internal/ftdebug.h>

#అన్తర్భావ <freetype/internal/ftobjs.h>
#అన్తర్భావ <freetype/internal/services/svpfr.h>


  /* check the format */
  స్థిర FT_Service_PfrMetrics
  ft_pfr_check( FT_Face  face )
  {
    FT_Service_PfrMetrics  service = ఖాళీ;


    ఒకవేళ ( face )
      FT_FACE_LOOKUP_SERVICE( face, service, PFR_METRICS );

    తిరిగి service;
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


    ఒకవేళ ( !face )
      తిరిగి FT_THROW( Invalid_Face_Handle );

    service = ft_pfr_check( face );
    ఒకవేళ ( service )
    {
      error = service->get_metrics( face,
                                    aoutline_resolution,
                                    ametrics_resolution,
                                    ametrics_x_scale,
                                    ametrics_y_scale );
    }
    లేకపోతే
    {
      FT_Fixed  x_scale, y_scale;


      /* this is not a PFR font */
      ఒకవేళ ( aoutline_resolution )
        *aoutline_resolution = face->units_చొప్పున_EM;

      ఒకవేళ ( ametrics_resolution )
        *ametrics_resolution = face->units_చొప్పున_EM;

      x_scale = y_scale = 0x10000L;
      ఒకవేళ ( face->కొలత )
      {
        x_scale = face->కొలత->metrics.x_scale;
        y_scale = face->కొలత->metrics.y_scale;
      }

      ఒకవేళ ( ametrics_x_scale )
        *ametrics_x_scale = x_scale;

      ఒకవేళ ( ametrics_y_scale )
        *ametrics_y_scale = y_scale;

      error = FT_THROW( Unknown_File_Format );
    }

    తిరిగి error;
  }


  /* documentation is in ftpfr.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_PFR_Kerning( FT_Face     face,
                      FT_UInt     ఎడమ,
                      FT_UInt     కుడి,
                      FT_సదిశ  *avector )
  {
    FT_Error               error;
    FT_Service_PfrMetrics  service;


    ఒకవేళ ( !face )
      తిరిగి FT_THROW( Invalid_Face_Handle );

    ఒకవేళ ( !avector )
      తిరిగి FT_THROW( Invalid_వాదం );

    service = ft_pfr_check( face );
    ఒకవేళ ( service )
      error = service->get_kerning( face, ఎడమ, కుడి, avector );
    లేకపోతే
      error = FT_Get_Kerning( face, ఎడమ, కుడి,
                              FT_KERNING_UNSCALED, avector );

    తిరిగి error;
  }


  /* documentation is in ftpfr.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_PFR_Advance( FT_Face   face,
                      FT_UInt   gindex,
                      FT_Pos   *aadvance )
  {
    FT_Error               error;
    FT_Service_PfrMetrics  service;


    ఒకవేళ ( !face )
      తిరిగి FT_THROW( Invalid_Face_Handle );

    ఒకవేళ ( !aadvance )
      తిరిగి FT_THROW( Invalid_వాదం );

    service = ft_pfr_check( face );
    ఒకవేళ ( service )
      error = service->get_advance( face, gindex, aadvance );
    లేకపోతే
      /* XXX: TODO: PROVIDE ADVANCE-LOADING METHOD TO ALL FONT DRIVERS */
      error = FT_THROW( Invalid_వాదం );

    తిరిగి error;
  }


/*చివర*/
