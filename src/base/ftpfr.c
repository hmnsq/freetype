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

#ضمن <freetype/internal/ftdebug.h>

#ضمن <freetype/internal/ftobjs.h>
#ضمن <freetype/internal/services/svpfr.h>


  /* check the format */
  ساكن FT_Service_PfrMetrics
  ft_pfr_check( FT_Face  face )
  {
    FT_Service_PfrMetrics  service = عدم;


    𐤀𐤌 ( face )
      FT_FACE_LOOKUP_SERVICE( face, service, PFR_METRICS );

    أرجع service;
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


    𐤀𐤌 ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );

    service = ft_pfr_check( face );
    𐤀𐤌 ( service )
    {
      error = service->get_metrics( face,
                                    aoutline_resolution,
                                    ametrics_resolution,
                                    ametrics_x_scale,
                                    ametrics_y_scale );
    }
    وإلا
    {
      FT_Fixed  x_scale, y_scale;


      /* this is not a PFR font */
      𐤀𐤌 ( aoutline_resolution )
        *aoutline_resolution = face->units_per_EM;

      𐤀𐤌 ( ametrics_resolution )
        *ametrics_resolution = face->units_per_EM;

      x_scale = y_scale = 0x10000L;
      𐤀𐤌 ( face->size )
      {
        x_scale = face->size->metrics.x_scale;
        y_scale = face->size->metrics.y_scale;
      }

      𐤀𐤌 ( ametrics_x_scale )
        *ametrics_x_scale = x_scale;

      𐤀𐤌 ( ametrics_y_scale )
        *ametrics_y_scale = y_scale;

      error = FT_THROW( Unknown_File_Format );
    }

    أرجع error;
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


    𐤀𐤌 ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );

    𐤀𐤌 ( !avector )
      أرجع FT_THROW( Invalid_Argument );

    service = ft_pfr_check( face );
    𐤀𐤌 ( service )
      error = service->get_kerning( face, left, right, avector );
    وإلا
      error = FT_Get_Kerning( face, left, right,
                              FT_KERNING_UNSCALED, avector );

    أرجع error;
  }


  /* documentation is in ftpfr.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_PFR_Advance( FT_Face   face,
                      FT_UInt   gindex,
                      FT_Pos   *aadvance )
  {
    FT_Error               error;
    FT_Service_PfrMetrics  service;


    𐤀𐤌 ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );

    𐤀𐤌 ( !aadvance )
      أرجع FT_THROW( Invalid_Argument );

    service = ft_pfr_check( face );
    𐤀𐤌 ( service )
      error = service->get_advance( face, gindex, aadvance );
    وإلا
      /* XXX: TODO: PROVIDE ADVANCE-LOADING METHOD TO ALL FONT DRIVERS */
      error = FT_THROW( Invalid_Argument );

    أرجع error;
  }


/* END */
