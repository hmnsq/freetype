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

#ಅನ್ತರ್ಭಾವ <freetype/internal/ftdebug.h>

#ಅನ್ತರ್ಭಾವ <freetype/internal/ftobjs.h>
#ಅನ್ತರ್ಭಾವ <freetype/internal/services/svpfr.h>


  /* check the format */
  ಸ್ಥಿರ FT_Service_PfrMetrics
  ft_pfr_check( FT_Face  face )
  {
    FT_Service_PfrMetrics  service = ಖಾಲಿ;


    ಆದರೆ ( face )
      FT_FACE_LOOKUP_SERVICE( face, service, PFR_METRICS );

    ಹಿಂತಿರುಗಿ service;
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


    ಆದರೆ ( !face )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_Face_Handle );

    service = ft_pfr_check( face );
    ಆದರೆ ( service )
    {
      error = service->get_metrics( face,
                                    aoutline_resolution,
                                    ametrics_resolution,
                                    ametrics_x_scale,
                                    ametrics_y_scale );
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
      FT_Fixed  x_scale, y_scale;


      /* this is not a PFR font */
      ಆದರೆ ( aoutline_resolution )
        *aoutline_resolution = face->units_ಪ್ರತಿ_EM;

      ಆದರೆ ( ametrics_resolution )
        *ametrics_resolution = face->units_ಪ್ರತಿ_EM;

      x_scale = y_scale = 0x10000L;
      ಆದರೆ ( face->ಗಾತ್ರ_2 )
      {
        x_scale = face->ಗಾತ್ರ_2->metrics.x_scale;
        y_scale = face->ಗಾತ್ರ_2->metrics.y_scale;
      }

      ಆದರೆ ( ametrics_x_scale )
        *ametrics_x_scale = x_scale;

      ಆದರೆ ( ametrics_y_scale )
        *ametrics_y_scale = y_scale;

      error = FT_THROW( Unknown_File_Format );
    }

    ಹಿಂತಿರುಗಿ error;
  }


  /* documentation is in ftpfr.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_PFR_Kerning( FT_Face     face,
                      FT_UInt     ಎಡ,
                      FT_UInt     ಬಲ,
                      FT_ಸದಿಶ  *avector )
  {
    FT_Error               error;
    FT_Service_PfrMetrics  service;


    ಆದರೆ ( !face )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_Face_Handle );

    ಆದರೆ ( !avector )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

    service = ft_pfr_check( face );
    ಆದರೆ ( service )
      error = service->get_kerning( face, ಎಡ, ಬಲ, avector );
    ಇಲ್ಲದಿದ್ದರೆ
      error = FT_Get_Kerning( face, ಎಡ, ಬಲ,
                              FT_KERNING_UNSCALED, avector );

    ಹಿಂತಿರುಗಿ error;
  }


  /* documentation is in ftpfr.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_PFR_Advance( FT_Face   face,
                      FT_UInt   gindex,
                      FT_Pos   *aadvance )
  {
    FT_Error               error;
    FT_Service_PfrMetrics  service;


    ಆದರೆ ( !face )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_Face_Handle );

    ಆದರೆ ( !aadvance )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

    service = ft_pfr_check( face );
    ಆದರೆ ( service )
      error = service->get_advance( face, gindex, aadvance );
    ಇಲ್ಲದಿದ್ದರೆ
      /* XXX: TODO: PROVIDE ADVANCE-LOADING METHOD TO ALL FONT DRIVERS */
      error = FT_THROW( Invalid_ವಾದ );

    ಹಿಂತಿರುಗಿ error;
  }


/*ಕೊನೆ*/
