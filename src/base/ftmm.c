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


#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftdebug.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/ftmm.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftobjs.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/services/svmm.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/services/svmetric.h>


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ  FT_COMPONENT
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FT_COMPONENT  mm


  ਸਥਿਰ FT_Error
  ft_face_get_mm_service( FT_Face                   face,
                          FT_Service_MultiMasters  *aservice )
  {
    FT_Error  error;


    *aservice = ਨਿਰਾ;

    ਜੇ ( !face )
      ਵਾਪਸ FT_THROW( Invalid_Face_Handle );

    error = FT_ERR( Invalid_ਦਲੀਲ );

    ਜੇ ( FT_HAS_ਗੁਣਜ_MASTERS( face ) )
    {
      FT_FACE_LOOKUP_SERVICE( face,
                              *aservice,
                              MULTI_MASTERS );

      ਜੇ ( *aservice )
        error = FT_Err_Ok;
    }

    ਵਾਪਸ error;
  }


  ਸਥਿਰ FT_Error
  ft_face_get_mvar_service( FT_Face                        face,
                            FT_Service_MetricsVariations  *aservice )
  {
    FT_Error  error;


    *aservice = ਨਿਰਾ;

    ਜੇ ( !face )
      ਵਾਪਸ FT_THROW( Invalid_Face_Handle );

    error = FT_ERR( Invalid_ਦਲੀਲ );

    ਜੇ ( FT_HAS_ਗੁਣਜ_MASTERS( face ) )
    {
      FT_FACE_LOOKUP_SERVICE( face,
                              *aservice,
                              METRICS_VARIATIONS );

      ਜੇ ( *aservice )
        error = FT_Err_Ok;
    }

    ਵਾਪਸ error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Multi_Master( FT_Face           face,
                       FT_Multi_Master  *amaster )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    ਜੇ ( !amaster )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    error = ft_face_get_mm_service( face, &service );
    ਜੇ ( !error )
    {
      error = FT_ERR( Invalid_ਦਲੀਲ );
      ਜੇ ( service->get_mm )
        error = service->get_mm( face, amaster );
    }

    ਵਾਪਸ error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_MM_Var( FT_Face      face,
                 FT_MM_Var*  *amaster )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    ਜੇ ( !amaster )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    error = ft_face_get_mm_service( face, &service );
    ਜੇ ( !error )
    {
      error = FT_ERR( Invalid_ਦਲੀਲ );
      ਜੇ ( service->get_mm_var )
        error = service->get_mm_var( face, amaster );
    }

    ਵਾਪਸ error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_ਪੂਰਾ_MM_Var( FT_Library  library,
                  FT_MM_Var*  amaster )
  {
    FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ;


    ਜੇ ( !library )
      ਵਾਪਸ FT_THROW( Invalid_Library_Handle );

    ਸਮ੍ਰਿਤੀ = library->ਸਮ੍ਰਿਤੀ;
    FT_FREE( amaster );

    ਵਾਪਸ FT_Err_Ok;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_ਸੈੱਟ_MM_Design_Coordinates( FT_Face   face,
                                FT_UInt   ਅੰਕ_coords,
                                FT_Long*  coords )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    ਜੇ ( ਅੰਕ_coords && !coords )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    error = ft_face_get_mm_service( face, &service );
    ਜੇ ( !error )
    {
      error = FT_ERR( Invalid_ਦਲੀਲ );
      ਜੇ ( service->ਸੈੱਟ_mm_design )
        error = service->ਸੈੱਟ_mm_design( face, ਅੰਕ_coords, coords );

      ਜੇ ( !error )
      {
        ਜੇ ( ਅੰਕ_coords )
          face->face_flags |= FT_FACE_FLAG_VARIATION;
        ਨਹੀਂਤਾਂ
          face->face_flags &= ~FT_FACE_FLAG_VARIATION;
      }
    }

    /* enforce recomputation of auto-hinting data */
    ਜੇ ( !error && face->autohint.finalizer )
    {
      face->autohint.finalizer( face->autohint.ਅੰਕੜੇ );
      face->autohint.ਅੰਕੜੇ = ਨਿਰਾ;
    }

    ਵਾਪਸ error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_ਸੈੱਟ_MM_ਭਾਰ_ਸਦਿਸ਼( FT_Face    face,
                          FT_UInt    len,
                          FT_Fixed*  weightvector )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    ਜੇ ( len && !weightvector )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    error = ft_face_get_mm_service( face, &service );
    ਜੇ ( !error )
    {
      error = FT_ERR( Invalid_ਦਲੀਲ );
      ਜੇ ( service->ਸੈੱਟ_mm_weightvector )
        error = service->ਸੈੱਟ_mm_weightvector( face, len, weightvector );

      ਜੇ ( !error )
      {
        ਜੇ ( len )
          face->face_flags |= FT_FACE_FLAG_VARIATION;
        ਨਹੀਂਤਾਂ
          face->face_flags &= ~FT_FACE_FLAG_VARIATION;
      }
    }

    /* enforce recomputation of auto-hinting data */
    ਜੇ ( !error && face->autohint.finalizer )
    {
      face->autohint.finalizer( face->autohint.ਅੰਕੜੇ );
      face->autohint.ਅੰਕੜੇ = ਨਿਰਾ;
    }

    ਵਾਪਸ error;
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Get_MM_ਭਾਰ_ਸਦਿਸ਼( FT_Face    face,
                          FT_UInt*   len,
                          FT_Fixed*  weightvector )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    ਜੇ ( len && !weightvector )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    error = ft_face_get_mm_service( face, &service );
    ਜੇ ( !error )
    {
      error = FT_ERR( Invalid_ਦਲੀਲ );
      ਜੇ ( service->get_mm_weightvector )
        error = service->get_mm_weightvector( face, len, weightvector );
    }

    ਵਾਪਸ error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_ਸੈੱਟ_Var_Design_Coordinates( FT_Face    face,
                                 FT_UInt    ਅੰਕ_coords,
                                 FT_Fixed*  coords )
  {
    FT_Error                      error;
    FT_Service_MultiMasters       service_mm   = ਨਿਰਾ;
    FT_Service_MetricsVariations  service_mvar = ਨਿਰਾ;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    ਜੇ ( ਅੰਕ_coords && !coords )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    ਜੇ ( !ਅੰਕ_coords && !FT_IS_VARIATION( face ) )
      ਵਾਪਸ FT_Err_Ok;  /* nothing to be done */

    error = ft_face_get_mm_service( face, &service_mm );
    ਜੇ ( !error )
    {
      error = FT_ERR( Invalid_ਦਲੀਲ );
      ਜੇ ( service_mm->ਸੈੱਟ_var_design )
        error = service_mm->ਸੈੱਟ_var_design( face, ਅੰਕ_coords, coords );

      ਜੇ ( !error || error == -1 || error == -2 )
      {
        FT_Bool  is_variation_old = FT_IS_VARIATION( face );


        ਜੇ ( error != -1 )
        {
          ਜੇ ( error == -2 ) /* -2 means is_variable. */
          {
            face->face_flags |= FT_FACE_FLAG_VARIATION;
            error             = FT_Err_Ok;
          }
          ਨਹੀਂਤਾਂ
            face->face_flags &= ~FT_FACE_FLAG_VARIATION;
        }

        ਜੇ ( service_mm->construct_ps_name )
        {
          ਜੇ ( error == -1 )
          {
            /* The PS name of a named instance and a non-named instance */
            /* usually differs, even if the axis values are identical.  */
            ਜੇ ( is_variation_old != FT_IS_VARIATION( face ) )
              service_mm->construct_ps_name( face );
          }
          ਨਹੀਂਤਾਂ
            service_mm->construct_ps_name( face );
        }
      }

      /* internal error code -1 means `no change'; we can exit immediately */
      ਜੇ ( error == -1 )
        ਵਾਪਸ FT_Err_Ok;
    }

    ਜੇ ( !error )
    {
      (ਖਾਲੀ)ft_face_get_mvar_service( face, &service_mvar );

      ਜੇ ( service_mvar && service_mvar->metrics_adjust )
        service_mvar->metrics_adjust( face );
    }

    /* enforce recomputation of auto-hinting data */
    ਜੇ ( !error && face->autohint.finalizer )
    {
      face->autohint.finalizer( face->autohint.ਅੰਕੜੇ );
      face->autohint.ਅੰਕੜੇ = ਨਿਰਾ;
    }

    ਵਾਪਸ error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Var_Design_Coordinates( FT_Face    face,
                                 FT_UInt    ਅੰਕ_coords,
                                 FT_Fixed*  coords )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    ਜੇ ( !coords )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    error = ft_face_get_mm_service( face, &service );
    ਜੇ ( !error )
    {
      error = FT_ERR( Invalid_ਦਲੀਲ );
      ਜੇ ( service->get_var_design )
        error = service->get_var_design( face, ਅੰਕ_coords, coords );
    }

    ਵਾਪਸ error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_ਸੈੱਟ_MM_Blend_Coordinates( FT_Face    face,
                               FT_UInt    ਅੰਕ_coords,
                               FT_Fixed*  coords )
  {
    FT_Error                      error;
    FT_Service_MultiMasters       service_mm   = ਨਿਰਾ;
    FT_Service_MetricsVariations  service_mvar = ਨਿਰਾ;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    ਜੇ ( ਅੰਕ_coords && !coords )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    error = ft_face_get_mm_service( face, &service_mm );
    ਜੇ ( !error )
    {
      error = FT_ERR( Invalid_ਦਲੀਲ );
      ਜੇ ( service_mm->ਸੈੱਟ_mm_blend )
        error = service_mm->ਸੈੱਟ_mm_blend( face, ਅੰਕ_coords, coords );

      ਜੇ ( !error || error == -1 )
      {
        FT_Bool  is_variation_old = FT_IS_VARIATION( face );


        ਜੇ ( ਅੰਕ_coords )
          face->face_flags |= FT_FACE_FLAG_VARIATION;
        ਨਹੀਂਤਾਂ
          face->face_flags &= ~FT_FACE_FLAG_VARIATION;

        ਜੇ ( service_mm->construct_ps_name )
        {
          ਜੇ ( error == -1 )
          {
            /* The PS name of a named instance and a non-named instance */
            /* usually differs, even if the axis values are identical.  */
            ਜੇ ( is_variation_old != FT_IS_VARIATION( face ) )
              service_mm->construct_ps_name( face );
          }
          ਨਹੀਂਤਾਂ
            service_mm->construct_ps_name( face );
        }
      }

      /* internal error code -1 means `no change'; we can exit immediately */
      ਜੇ ( error == -1 )
        ਵਾਪਸ FT_Err_Ok;
    }

    ਜੇ ( !error )
    {
      (ਖਾਲੀ)ft_face_get_mvar_service( face, &service_mvar );

      ਜੇ ( service_mvar && service_mvar->metrics_adjust )
        service_mvar->metrics_adjust( face );
    }

    /* enforce recomputation of auto-hinting data */
    ਜੇ ( !error && face->autohint.finalizer )
    {
      face->autohint.finalizer( face->autohint.ਅੰਕੜੇ );
      face->autohint.ਅੰਕੜੇ = ਨਿਰਾ;
    }

    ਵਾਪਸ error;
  }


  /* documentation is in ftmm.h */

  /* This is exactly the same as the previous function.  It exists for */
  /* orthogonality.                                                    */

  FT_EXPORT_DEF( FT_Error )
  FT_ਸੈੱਟ_Var_Blend_Coordinates( FT_Face    face,
                                FT_UInt    ਅੰਕ_coords,
                                FT_Fixed*  coords )
  {
    FT_Error                      error;
    FT_Service_MultiMasters       service_mm   = ਨਿਰਾ;
    FT_Service_MetricsVariations  service_mvar = ਨਿਰਾ;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    ਜੇ ( ਅੰਕ_coords && !coords )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    error = ft_face_get_mm_service( face, &service_mm );
    ਜੇ ( !error )
    {
      error = FT_ERR( Invalid_ਦਲੀਲ );
      ਜੇ ( service_mm->ਸੈੱਟ_mm_blend )
        error = service_mm->ਸੈੱਟ_mm_blend( face, ਅੰਕ_coords, coords );

      ਜੇ ( !error || error == -1 || error == -2 )
      {
        FT_Bool  is_variation_old = FT_IS_VARIATION( face );


        ਜੇ ( error != -1 )
        {
          ਜੇ ( error == -2 ) /* -2 means is_variable. */
          {
            face->face_flags |= FT_FACE_FLAG_VARIATION;
            error             = FT_Err_Ok;
          }
          ਨਹੀਂਤਾਂ
            face->face_flags &= ~FT_FACE_FLAG_VARIATION;
        }

        ਜੇ ( service_mm->construct_ps_name )
        {
          ਜੇ ( error == -1 )
          {
            /* The PS name of a named instance and a non-named instance */
            /* usually differs, even if the axis values are identical.  */
            ਜੇ ( is_variation_old != FT_IS_VARIATION( face ) )
              service_mm->construct_ps_name( face );
          }
          ਨਹੀਂਤਾਂ
            service_mm->construct_ps_name( face );
        }
      }

      /* internal error code -1 means `no change'; we can exit immediately */
      ਜੇ ( error == -1 )
        ਵਾਪਸ FT_Err_Ok;
    }

    ਜੇ ( !error )
    {
      (ਖਾਲੀ)ft_face_get_mvar_service( face, &service_mvar );

      ਜੇ ( service_mvar && service_mvar->metrics_adjust )
        service_mvar->metrics_adjust( face );
    }

    /* enforce recomputation of auto-hinting data */
    ਜੇ ( !error && face->autohint.finalizer )
    {
      face->autohint.finalizer( face->autohint.ਅੰਕੜੇ );
      face->autohint.ਅੰਕੜੇ = ਨਿਰਾ;
    }

    ਵਾਪਸ error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_MM_Blend_Coordinates( FT_Face    face,
                               FT_UInt    ਅੰਕ_coords,
                               FT_Fixed*  coords )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    ਜੇ ( !coords )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    error = ft_face_get_mm_service( face, &service );
    ਜੇ ( !error )
    {
      error = FT_ERR( Invalid_ਦਲੀਲ );
      ਜੇ ( service->get_mm_blend )
        error = service->get_mm_blend( face, ਅੰਕ_coords, coords );
    }

    ਵਾਪਸ error;
  }


  /* documentation is in ftmm.h */

  /* This is exactly the same as the previous function.  It exists for */
  /* orthogonality.                                                    */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Var_Blend_Coordinates( FT_Face    face,
                                FT_UInt    ਅੰਕ_coords,
                                FT_Fixed*  coords )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    ਜੇ ( !coords )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    error = ft_face_get_mm_service( face, &service );
    ਜੇ ( !error )
    {
      error = FT_ERR( Invalid_ਦਲੀਲ );
      ਜੇ ( service->get_mm_blend )
        error = service->get_mm_blend( face, ਅੰਕ_coords, coords );
    }

    ਵਾਪਸ error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Var_Axis_Flags( FT_MM_Var*  master,
                         FT_UInt     axis_ਸੂਚਕਾਂਕ,
                         FT_UInt*    flags )
  {
    FT_UShort*  axis_flags;


    ਜੇ ( !master || !flags )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    ਜੇ ( axis_ਸੂਚਕਾਂਕ >= master->ਅੰਕ_axis )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    /* the axis flags array immediately follows the data of `master' */
    axis_flags = (FT_UShort*)&( master[1] );
    *flags     = axis_flags[axis_ਸੂਚਕਾਂਕ];

    ਵਾਪਸ FT_Err_Ok;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_ਸੈੱਟ_Named_Instance( FT_Face  face,
                         FT_UInt  instance_ਸੂਚਕਾਂਕ )
  {
    FT_Error  error;

    FT_Service_MultiMasters       service_mm   = ਨਿਰਾ;
    FT_Service_MetricsVariations  service_mvar = ਨਿਰਾ;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    error = ft_face_get_mm_service( face, &service_mm );
    ਜੇ ( !error )
    {
      error = FT_ERR( Invalid_ਦਲੀਲ );
      ਜੇ ( service_mm->ਸੈੱਟ_named_instance )
        error = service_mm->ਸੈੱਟ_named_instance( face, instance_ਸੂਚਕਾਂਕ );

      ਜੇ ( !error || error == -1 )
      {
        FT_Bool  is_variation_old = FT_IS_VARIATION( face );


        face->face_flags &= ~FT_FACE_FLAG_VARIATION;
        face->face_ਸੂਚਕਾਂਕ  = ( instance_ਸੂਚਕਾਂਕ << 16 )        |
                            ( face->face_ਸੂਚਕਾਂਕ & 0xFFFFL );

        ਜੇ ( service_mm->construct_ps_name )
        {
          ਜੇ ( error == -1 )
          {
            /* The PS name of a named instance and a non-named instance */
            /* usually differs, even if the axis values are identical.  */
            ਜੇ ( is_variation_old != FT_IS_VARIATION( face ) )
              service_mm->construct_ps_name( face );
          }
          ਨਹੀਂਤਾਂ
            service_mm->construct_ps_name( face );
        }
      }

      /* internal error code -1 means `no change'; we can exit immediately */
      ਜੇ ( error == -1 )
        ਵਾਪਸ FT_Err_Ok;
    }

    ਜੇ ( !error )
    {
      (ਖਾਲੀ)ft_face_get_mvar_service( face, &service_mvar );

      ਜੇ ( service_mvar && service_mvar->metrics_adjust )
        service_mvar->metrics_adjust( face );
    }

    /* enforce recomputation of auto-hinting data */
    ਜੇ ( !error && face->autohint.finalizer )
    {
      face->autohint.finalizer( face->autohint.ਅੰਕੜੇ );
      face->autohint.ਅੰਕੜੇ = ਨਿਰਾ;
    }

    ਵਾਪਸ error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Default_Named_Instance( FT_Face   face,
                                 FT_UInt  *instance_ਸੂਚਕਾਂਕ )
  {
    FT_Error  error;

    FT_Service_MultiMasters  service_mm = ਨਿਰਾ;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    error = ft_face_get_mm_service( face, &service_mm );
    ਜੇ ( !error )
    {
      /* no error if `get_default_named_instance` is not available */
      ਜੇ ( service_mm->get_default_named_instance )
        error = service_mm->get_default_named_instance( face,
                                                        instance_ਸੂਚਕਾਂਕ );
      ਨਹੀਂਤਾਂ
        error = FT_Err_Ok;
    }

    ਵਾਪਸ error;
  }


/*ਸਿਰਾ*/
