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

#अन्तर्भाव <freetype/internal/ftdebug.h>

#अन्तर्भाव <freetype/internal/ftobjs.h>
#अन्तर्भाव <freetype/internal/services/svpfr.h>


  /* check the format */
  स्थिर FT_Service_PfrMetrics
  ft_pfr_check( FT_Face  face )
  {
    FT_Service_PfrMetrics  service = रिक्त;


    यदि ( face )
      FT_FACE_LOOKUP_SERVICE( face, service, PFR_METRICS );

    फर्काऊ service;
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


    यदि ( !face )
      फर्काऊ FT_THROW( Invalid_Face_Handle );

    service = ft_pfr_check( face );
    यदि ( service )
    {
      error = service->get_metrics( face,
                                    aoutline_resolution,
                                    ametrics_resolution,
                                    ametrics_x_scale,
                                    ametrics_y_scale );
    }
    अन्यथा
    {
      FT_Fixed  x_scale, y_scale;


      /* this is not a PFR font */
      यदि ( aoutline_resolution )
        *aoutline_resolution = face->units_per_EM;

      यदि ( ametrics_resolution )
        *ametrics_resolution = face->units_per_EM;

      x_scale = y_scale = 0x10000L;
      यदि ( face->size )
      {
        x_scale = face->size->metrics.x_scale;
        y_scale = face->size->metrics.y_scale;
      }

      यदि ( ametrics_x_scale )
        *ametrics_x_scale = x_scale;

      यदि ( ametrics_y_scale )
        *ametrics_y_scale = y_scale;

      error = FT_THROW( Unknown_File_Format );
    }

    फर्काऊ error;
  }


  /* documentation is in ftpfr.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_PFR_Kerning( FT_Face     face,
                      FT_UInt     बायाँ,
                      FT_UInt     दायाँ,
                      FT_Vector  *avector )
  {
    FT_Error               error;
    FT_Service_PfrMetrics  service;


    यदि ( !face )
      फर्काऊ FT_THROW( Invalid_Face_Handle );

    यदि ( !avector )
      फर्काऊ FT_THROW( Invalid_Argument );

    service = ft_pfr_check( face );
    यदि ( service )
      error = service->get_kerning( face, बायाँ, दायाँ, avector );
    अन्यथा
      error = FT_Get_Kerning( face, बायाँ, दायाँ,
                              FT_KERNING_UNSCALED, avector );

    फर्काऊ error;
  }


  /* documentation is in ftpfr.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_PFR_Advance( FT_Face   face,
                      FT_UInt   gindex,
                      FT_Pos   *aadvance )
  {
    FT_Error               error;
    FT_Service_PfrMetrics  service;


    यदि ( !face )
      फर्काऊ FT_THROW( Invalid_Face_Handle );

    यदि ( !aadvance )
      फर्काऊ FT_THROW( Invalid_Argument );

    service = ft_pfr_check( face );
    यदि ( service )
      error = service->get_advance( face, gindex, aadvance );
    अन्यथा
      /* XXX: TODO: PROVIDE ADVANCE-LOADING METHOD TO ALL FONT DRIVERS */
      error = FT_THROW( Invalid_Argument );

    फर्काऊ error;
  }


/* END */
