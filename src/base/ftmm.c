/****************************************************************************
 *
 * ftmm.c
 *
 *   Multiple Master font support (body).
 *
 * Copyright (C) 1996-2026 by
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

#ಅನ್ತರ್ಭಾವ <freetype/ftmm.h>
#ಅನ್ತರ್ಭಾವ <freetype/internal/ftobjs.h>
#ಅನ್ತರ್ಭಾವ <freetype/internal/services/svmm.h>
#ಅನ್ತರ್ಭಾವ <freetype/internal/services/svmetric.h>


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು  FT_COMPONENT
#ವ್ಯಾಖ್ಯಾನಿಸು FT_COMPONENT  mm


  ಸ್ಥಿರ FT_Error
  ft_face_get_mm_service( FT_Face                   face,
                          FT_Service_MultiMasters  *aservice )
  {
    FT_Error  error;


    *aservice = ಖಾಲಿ;

    ಆದರೆ ( !face )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_Face_Handle );

    error = FT_ERR( Invalid_ವಾದ );

    ಆದರೆ ( FT_HAS_ಅಪವರ್ತ್ಯ_MASTERS( face ) )
    {
      FT_FACE_LOOKUP_SERVICE( face,
                              *aservice,
                              MULTI_MASTERS );

      ಆದರೆ ( *aservice )
        error = FT_Err_Ok;
    }

    ಹಿಂತಿರುಗಿ error;
  }


  ಸ್ಥಿರ FT_Error
  ft_face_get_mvar_service( FT_Face                        face,
                            FT_Service_MetricsVariations  *aservice )
  {
    FT_Error  error;


    *aservice = ಖಾಲಿ;

    ಆದರೆ ( !face )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_Face_Handle );

    error = FT_ERR( Invalid_ವಾದ );

    ಆದರೆ ( FT_HAS_ಅಪವರ್ತ್ಯ_MASTERS( face ) )
    {
      FT_FACE_LOOKUP_SERVICE( face,
                              *aservice,
                              METRICS_VARIATIONS );

      ಆದರೆ ( *aservice )
        error = FT_Err_Ok;
    }

    ಹಿಂತಿರುಗಿ error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Multi_Master( FT_Face           face,
                       FT_Multi_Master  *amaster )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    ಆದರೆ ( !amaster )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

    error = ft_face_get_mm_service( face, &service );
    ಆದರೆ ( !error )
    {
      error = FT_ERR( Invalid_ವಾದ );
      ಆದರೆ ( service->get_mm )
        error = service->get_mm( face, amaster );
    }

    ಹಿಂತಿರುಗಿ error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_MM_Var( FT_Face      face,
                 FT_MM_Var*  *amaster )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    ಆದರೆ ( !amaster )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

    error = ft_face_get_mm_service( face, &service );
    ಆದರೆ ( !error )
    {
      error = FT_ERR( Invalid_ವಾದ );
      ಆದರೆ ( service->get_mm_var )
        error = service->get_mm_var( face, amaster );
    }

    ಹಿಂತಿರುಗಿ error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_ಮುಗಿದ_MM_Var( FT_Library  library,
                  FT_MM_Var*  amaster )
  {
    FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ;


    ಆದರೆ ( !library )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_Library_Handle );

    ಸ್ಮರಣೆ = library->ಸ್ಮರಣೆ;
    FT_FREE( amaster );

    ಹಿಂತಿರುಗಿ FT_Err_Ok;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_ಗಣ_MM_Design_Coordinates( FT_Face   face,
                                FT_UInt   ಅಂಕೆ_coords,
                                FT_Long*  coords )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    ಆದರೆ ( ಅಂಕೆ_coords && !coords )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

    error = ft_face_get_mm_service( face, &service );
    ಆದರೆ ( !error )
    {
      error = FT_ERR( Invalid_ವಾದ );
      ಆದರೆ ( service->ಗಣ_mm_design )
        error = service->ಗಣ_mm_design( face, ಅಂಕೆ_coords, coords );

      ಆದರೆ ( !error )
      {
        ಆದರೆ ( ಅಂಕೆ_coords )
          face->face_flags |= FT_FACE_FLAG_VARIATION;
        ಇಲ್ಲದಿದ್ದರೆ
          face->face_flags &= ~FT_FACE_FLAG_VARIATION;
      }
    }

    /* enforce recomputation of auto-hinting data */
    ಆದರೆ ( !error && face->autohint.finalizer )
    {
      face->autohint.finalizer( face->autohint.ದತ್ತಾಂಶ );
      face->autohint.ದತ್ತಾಂಶ = ಖಾಲಿ;
    }

    ಹಿಂತಿರುಗಿ error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_ಗಣ_MM_ತೂಕ_ಸದಿಶ( FT_Face    face,
                          FT_UInt    len,
                          FT_Fixed*  weightvector )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    ಆದರೆ ( len && !weightvector )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

    error = ft_face_get_mm_service( face, &service );
    ಆದರೆ ( !error )
    {
      error = FT_ERR( Invalid_ವಾದ );
      ಆದರೆ ( service->ಗಣ_mm_weightvector )
        error = service->ಗಣ_mm_weightvector( face, len, weightvector );

      ಆದರೆ ( !error )
      {
        ಆದರೆ ( len )
          face->face_flags |= FT_FACE_FLAG_VARIATION;
        ಇಲ್ಲದಿದ್ದರೆ
          face->face_flags &= ~FT_FACE_FLAG_VARIATION;
      }
    }

    /* enforce recomputation of auto-hinting data */
    ಆದರೆ ( !error && face->autohint.finalizer )
    {
      face->autohint.finalizer( face->autohint.ದತ್ತಾಂಶ );
      face->autohint.ದತ್ತಾಂಶ = ಖಾಲಿ;
    }

    ಹಿಂತಿರುಗಿ error;
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Get_MM_ತೂಕ_ಸದಿಶ( FT_Face    face,
                          FT_UInt*   len,
                          FT_Fixed*  weightvector )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    ಆದರೆ ( len && !weightvector )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

    error = ft_face_get_mm_service( face, &service );
    ಆದರೆ ( !error )
    {
      error = FT_ERR( Invalid_ವಾದ );
      ಆದರೆ ( service->get_mm_weightvector )
        error = service->get_mm_weightvector( face, len, weightvector );
    }

    ಹಿಂತಿರುಗಿ error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_ಗಣ_Var_Design_Coordinates( FT_Face    face,
                                 FT_UInt    ಅಂಕೆ_coords,
                                 FT_Fixed*  coords )
  {
    FT_Error                      error;
    FT_Service_MultiMasters       service_mm   = ಖಾಲಿ;
    FT_Service_MetricsVariations  service_mvar = ಖಾಲಿ;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    ಆದರೆ ( ಅಂಕೆ_coords && !coords )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

    ಆದರೆ ( !ಅಂಕೆ_coords && !FT_IS_VARIATION( face ) )
      ಹಿಂತಿರುಗಿ FT_Err_Ok;  /* nothing to be done */

    error = ft_face_get_mm_service( face, &service_mm );
    ಆದರೆ ( !error )
    {
      error = FT_ERR( Invalid_ವಾದ );
      ಆದರೆ ( service_mm->ಗಣ_var_design )
        error = service_mm->ಗಣ_var_design( face, ಅಂಕೆ_coords, coords );

      ಆದರೆ ( !error || error == -1 || error == -2 )
      {
        FT_Bool  is_variation_old = FT_IS_VARIATION( face );


        ಆದರೆ ( error != -1 )
        {
          ಆದರೆ ( error == -2 ) /* -2 means is_variable. */
          {
            face->face_flags |= FT_FACE_FLAG_VARIATION;
            error             = FT_Err_Ok;
          }
          ಇಲ್ಲದಿದ್ದರೆ
            face->face_flags &= ~FT_FACE_FLAG_VARIATION;
        }

        ಆದರೆ ( service_mm->construct_ps_name )
        {
          ಆದರೆ ( error == -1 )
          {
            /* The PS name of a named instance and a non-named instance */
            /* usually differs, even if the axis values are identical.  */
            ಆದರೆ ( is_variation_old != FT_IS_VARIATION( face ) )
              service_mm->construct_ps_name( face );
          }
          ಇಲ್ಲದಿದ್ದರೆ
            service_mm->construct_ps_name( face );
        }
      }

      /* internal error code -1 means `no change'; we can exit immediately */
      ಆದರೆ ( error == -1 )
        ಹಿಂತಿರುಗಿ FT_Err_Ok;
    }

    ಆದರೆ ( !error )
    {
      (ಶೂನ್ಯ)ft_face_get_mvar_service( face, &service_mvar );

      ಆದರೆ ( service_mvar && service_mvar->metrics_adjust )
        service_mvar->metrics_adjust( face );
    }

    /* enforce recomputation of auto-hinting data */
    ಆದರೆ ( !error && face->autohint.finalizer )
    {
      face->autohint.finalizer( face->autohint.ದತ್ತಾಂಶ );
      face->autohint.ದತ್ತಾಂಶ = ಖಾಲಿ;
    }

    ಹಿಂತಿರುಗಿ error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Var_Design_Coordinates( FT_Face    face,
                                 FT_UInt    ಅಂಕೆ_coords,
                                 FT_Fixed*  coords )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    ಆದರೆ ( !coords )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

    error = ft_face_get_mm_service( face, &service );
    ಆದರೆ ( !error )
    {
      error = FT_ERR( Invalid_ವಾದ );
      ಆದರೆ ( service->get_var_design )
        error = service->get_var_design( face, ಅಂಕೆ_coords, coords );
    }

    ಹಿಂತಿರುಗಿ error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_ಗಣ_MM_Blend_Coordinates( FT_Face    face,
                               FT_UInt    ಅಂಕೆ_coords,
                               FT_Fixed*  coords )
  {
    FT_Error                      error;
    FT_Service_MultiMasters       service_mm   = ಖಾಲಿ;
    FT_Service_MetricsVariations  service_mvar = ಖಾಲಿ;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    ಆದರೆ ( ಅಂಕೆ_coords && !coords )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

    error = ft_face_get_mm_service( face, &service_mm );
    ಆದರೆ ( !error )
    {
      error = FT_ERR( Invalid_ವಾದ );
      ಆದರೆ ( service_mm->ಗಣ_mm_blend )
        error = service_mm->ಗಣ_mm_blend( face, ಅಂಕೆ_coords, coords );

      ಆದರೆ ( !error || error == -1 )
      {
        FT_Bool  is_variation_old = FT_IS_VARIATION( face );


        ಆದರೆ ( ಅಂಕೆ_coords )
          face->face_flags |= FT_FACE_FLAG_VARIATION;
        ಇಲ್ಲದಿದ್ದರೆ
          face->face_flags &= ~FT_FACE_FLAG_VARIATION;

        ಆದರೆ ( service_mm->construct_ps_name )
        {
          ಆದರೆ ( error == -1 )
          {
            /* The PS name of a named instance and a non-named instance */
            /* usually differs, even if the axis values are identical.  */
            ಆದರೆ ( is_variation_old != FT_IS_VARIATION( face ) )
              service_mm->construct_ps_name( face );
          }
          ಇಲ್ಲದಿದ್ದರೆ
            service_mm->construct_ps_name( face );
        }
      }

      /* internal error code -1 means `no change'; we can exit immediately */
      ಆದರೆ ( error == -1 )
        ಹಿಂತಿರುಗಿ FT_Err_Ok;
    }

    ಆದರೆ ( !error )
    {
      (ಶೂನ್ಯ)ft_face_get_mvar_service( face, &service_mvar );

      ಆದರೆ ( service_mvar && service_mvar->metrics_adjust )
        service_mvar->metrics_adjust( face );
    }

    /* enforce recomputation of auto-hinting data */
    ಆದರೆ ( !error && face->autohint.finalizer )
    {
      face->autohint.finalizer( face->autohint.ದತ್ತಾಂಶ );
      face->autohint.ದತ್ತಾಂಶ = ಖಾಲಿ;
    }

    ಹಿಂತಿರುಗಿ error;
  }


  /* documentation is in ftmm.h */

  /* This is exactly the same as the previous function.  It exists for */
  /* orthogonality.                                                    */

  FT_EXPORT_DEF( FT_Error )
  FT_ಗಣ_Var_Blend_Coordinates( FT_Face    face,
                                FT_UInt    ಅಂಕೆ_coords,
                                FT_Fixed*  coords )
  {
    FT_Error                      error;
    FT_Service_MultiMasters       service_mm   = ಖಾಲಿ;
    FT_Service_MetricsVariations  service_mvar = ಖಾಲಿ;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    ಆದರೆ ( ಅಂಕೆ_coords && !coords )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

    error = ft_face_get_mm_service( face, &service_mm );
    ಆದರೆ ( !error )
    {
      error = FT_ERR( Invalid_ವಾದ );
      ಆದರೆ ( service_mm->ಗಣ_mm_blend )
        error = service_mm->ಗಣ_mm_blend( face, ಅಂಕೆ_coords, coords );

      ಆದರೆ ( !error || error == -1 || error == -2 )
      {
        FT_Bool  is_variation_old = FT_IS_VARIATION( face );


        ಆದರೆ ( error != -1 )
        {
          ಆದರೆ ( error == -2 ) /* -2 means is_variable. */
          {
            face->face_flags |= FT_FACE_FLAG_VARIATION;
            error             = FT_Err_Ok;
          }
          ಇಲ್ಲದಿದ್ದರೆ
            face->face_flags &= ~FT_FACE_FLAG_VARIATION;
        }

        ಆದರೆ ( service_mm->construct_ps_name )
        {
          ಆದರೆ ( error == -1 )
          {
            /* The PS name of a named instance and a non-named instance */
            /* usually differs, even if the axis values are identical.  */
            ಆದರೆ ( is_variation_old != FT_IS_VARIATION( face ) )
              service_mm->construct_ps_name( face );
          }
          ಇಲ್ಲದಿದ್ದರೆ
            service_mm->construct_ps_name( face );
        }
      }

      /* internal error code -1 means `no change'; we can exit immediately */
      ಆದರೆ ( error == -1 )
        ಹಿಂತಿರುಗಿ FT_Err_Ok;
    }

    ಆದರೆ ( !error )
    {
      (ಶೂನ್ಯ)ft_face_get_mvar_service( face, &service_mvar );

      ಆದರೆ ( service_mvar && service_mvar->metrics_adjust )
        service_mvar->metrics_adjust( face );
    }

    /* enforce recomputation of auto-hinting data */
    ಆದರೆ ( !error && face->autohint.finalizer )
    {
      face->autohint.finalizer( face->autohint.ದತ್ತಾಂಶ );
      face->autohint.ದತ್ತಾಂಶ = ಖಾಲಿ;
    }

    ಹಿಂತಿರುಗಿ error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_MM_Blend_Coordinates( FT_Face    face,
                               FT_UInt    ಅಂಕೆ_coords,
                               FT_Fixed*  coords )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    ಆದರೆ ( !coords )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

    error = ft_face_get_mm_service( face, &service );
    ಆದರೆ ( !error )
    {
      error = FT_ERR( Invalid_ವಾದ );
      ಆದರೆ ( service->get_mm_blend )
        error = service->get_mm_blend( face, ಅಂಕೆ_coords, coords );
    }

    ಹಿಂತಿರುಗಿ error;
  }


  /* documentation is in ftmm.h */

  /* This is exactly the same as the previous function.  It exists for */
  /* orthogonality.                                                    */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Var_Blend_Coordinates( FT_Face    face,
                                FT_UInt    ಅಂಕೆ_coords,
                                FT_Fixed*  coords )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    ಆದರೆ ( !coords )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

    error = ft_face_get_mm_service( face, &service );
    ಆದರೆ ( !error )
    {
      error = FT_ERR( Invalid_ವಾದ );
      ಆದರೆ ( service->get_mm_blend )
        error = service->get_mm_blend( face, ಅಂಕೆ_coords, coords );
    }

    ಹಿಂತಿರುಗಿ error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Var_Axis_Flags( FT_MM_Var*  master,
                         FT_UInt     axis_ಸೂಚ್ಯಂಕ,
                         FT_UInt*    flags )
  {
    FT_UShort*  axis_flags;


    ಆದರೆ ( !master || !flags )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

    ಆದರೆ ( axis_ಸೂಚ್ಯಂಕ >= master->ಅಂಕೆ_axis )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

    /* the axis flags array immediately follows the data of `master' */
    axis_flags = (FT_UShort*)&( master[1] );
    *flags     = axis_flags[axis_ಸೂಚ್ಯಂಕ];

    ಹಿಂತಿರುಗಿ FT_Err_Ok;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_ಗಣ_Named_Instance( FT_Face  face,
                         FT_UInt  instance_ಸೂಚ್ಯಂಕ )
  {
    FT_Error  error;

    FT_Service_MultiMasters       service_mm   = ಖಾಲಿ;
    FT_Service_MetricsVariations  service_mvar = ಖಾಲಿ;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    error = ft_face_get_mm_service( face, &service_mm );
    ಆದರೆ ( !error )
    {
      error = FT_ERR( Invalid_ವಾದ );
      ಆದರೆ ( service_mm->ಗಣ_named_instance )
        error = service_mm->ಗಣ_named_instance( face, instance_ಸೂಚ್ಯಂಕ );

      ಆದರೆ ( !error || error == -1 )
      {
        FT_Bool  is_variation_old = FT_IS_VARIATION( face );


        face->face_flags &= ~FT_FACE_FLAG_VARIATION;
        face->face_ಸೂಚ್ಯಂಕ  = ( instance_ಸೂಚ್ಯಂಕ << 16 )        |
                            ( face->face_ಸೂಚ್ಯಂಕ & 0xFFFFL );

        ಆದರೆ ( service_mm->construct_ps_name )
        {
          ಆದರೆ ( error == -1 )
          {
            /* The PS name of a named instance and a non-named instance */
            /* usually differs, even if the axis values are identical.  */
            ಆದರೆ ( is_variation_old != FT_IS_VARIATION( face ) )
              service_mm->construct_ps_name( face );
          }
          ಇಲ್ಲದಿದ್ದರೆ
            service_mm->construct_ps_name( face );
        }
      }

      /* internal error code -1 means `no change'; we can exit immediately */
      ಆದರೆ ( error == -1 )
        ಹಿಂತಿರುಗಿ FT_Err_Ok;
    }

    ಆದರೆ ( !error )
    {
      (ಶೂನ್ಯ)ft_face_get_mvar_service( face, &service_mvar );

      ಆದರೆ ( service_mvar && service_mvar->metrics_adjust )
        service_mvar->metrics_adjust( face );
    }

    /* enforce recomputation of auto-hinting data */
    ಆದರೆ ( !error && face->autohint.finalizer )
    {
      face->autohint.finalizer( face->autohint.ದತ್ತಾಂಶ );
      face->autohint.ದತ್ತಾಂಶ = ಖಾಲಿ;
    }

    ಹಿಂತಿರುಗಿ error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Default_Named_Instance( FT_Face   face,
                                 FT_UInt  *instance_ಸೂಚ್ಯಂಕ )
  {
    FT_Error  error;

    FT_Service_MultiMasters  service_mm = ಖಾಲಿ;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    error = ft_face_get_mm_service( face, &service_mm );
    ಆದರೆ ( !error )
    {
      /* no error if `get_default_named_instance` is not available */
      ಆದರೆ ( service_mm->get_default_named_instance )
        error = service_mm->get_default_named_instance( face,
                                                        instance_ಸೂಚ್ಯಂಕ );
      ಇಲ್ಲದಿದ್ದರೆ
        error = FT_Err_Ok;
    }

    ಹಿಂತಿರುಗಿ error;
  }


/*ಕೊನೆ*/
