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


#شامل <freetype/internal/ftdebug.h>

#شامل <freetype/ftmm.h>
#شامل <freetype/internal/ftobjs.h>
#شامل <freetype/internal/services/svmm.h>
#شامل <freetype/internal/services/svmetric.h>


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#تعریف_منسوخ  FT_COMPONENT
#تعریف_کرو FT_COMPONENT  mm


  ساکن FT_Error
  ft_face_get_mm_service( FT_Face                   face,
                          FT_Service_MultiMasters  *aservice )
  {
    FT_Error  error;


    *aservice = تہی;

    اگر ( !face )
      واپس FT_THROW( Invalid_Face_Handle );

    error = FT_ERR( Invalid_دلیل );

    اگر ( FT_HAS_مضرب_MASTERS( face ) )
    {
      FT_FACE_LOOKUP_SERVICE( face,
                              *aservice,
                              MULTI_MASTERS );

      اگر ( *aservice )
        error = FT_Err_Ok;
    }

    واپس error;
  }


  ساکن FT_Error
  ft_face_get_mvar_service( FT_Face                        face,
                            FT_Service_MetricsVariations  *aservice )
  {
    FT_Error  error;


    *aservice = تہی;

    اگر ( !face )
      واپس FT_THROW( Invalid_Face_Handle );

    error = FT_ERR( Invalid_دلیل );

    اگر ( FT_HAS_مضرب_MASTERS( face ) )
    {
      FT_FACE_LOOKUP_SERVICE( face,
                              *aservice,
                              METRICS_VARIATIONS );

      اگر ( *aservice )
        error = FT_Err_Ok;
    }

    واپس error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Multi_Master( FT_Face           face,
                       FT_Multi_Master  *amaster )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    اگر ( !amaster )
      واپس FT_THROW( Invalid_دلیل );

    error = ft_face_get_mm_service( face, &service );
    اگر ( !error )
    {
      error = FT_ERR( Invalid_دلیل );
      اگر ( service->get_mm )
        error = service->get_mm( face, amaster );
    }

    واپس error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_MM_Var( FT_Face      face,
                 FT_MM_Var*  *amaster )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    اگر ( !amaster )
      واپس FT_THROW( Invalid_دلیل );

    error = ft_face_get_mm_service( face, &service );
    اگر ( !error )
    {
      error = FT_ERR( Invalid_دلیل );
      اگر ( service->get_mm_var )
        error = service->get_mm_var( face, amaster );
    }

    واپس error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_مکمل_MM_Var( FT_Library  library,
                  FT_MM_Var*  amaster )
  {
    FT_حافظہ  حافظہ;


    اگر ( !library )
      واپس FT_THROW( Invalid_Library_Handle );

    حافظہ = library->حافظہ;
    FT_FREE( amaster );

    واپس FT_Err_Ok;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_سیٹ_MM_Design_Coordinates( FT_Face   face,
                                FT_UInt   عدد_coords,
                                FT_Long*  coords )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    اگر ( عدد_coords && !coords )
      واپس FT_THROW( Invalid_دلیل );

    error = ft_face_get_mm_service( face, &service );
    اگر ( !error )
    {
      error = FT_ERR( Invalid_دلیل );
      اگر ( service->سیٹ_mm_design )
        error = service->سیٹ_mm_design( face, عدد_coords, coords );

      اگر ( !error )
      {
        اگر ( عدد_coords )
          face->face_flags |= FT_FACE_FLAG_VARIATION;
        ورنہ
          face->face_flags &= ~FT_FACE_FLAG_VARIATION;
      }
    }

    /* enforce recomputation of auto-hinting data */
    اگر ( !error && face->autohint.finalizer )
    {
      face->autohint.finalizer( face->autohint.ڈیٹا );
      face->autohint.ڈیٹا = تہی;
    }

    واپس error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_سیٹ_MM_وزن_سمتیہ( FT_Face    face,
                          FT_UInt    len,
                          FT_Fixed*  weightvector )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    اگر ( len && !weightvector )
      واپس FT_THROW( Invalid_دلیل );

    error = ft_face_get_mm_service( face, &service );
    اگر ( !error )
    {
      error = FT_ERR( Invalid_دلیل );
      اگر ( service->سیٹ_mm_weightvector )
        error = service->سیٹ_mm_weightvector( face, len, weightvector );

      اگر ( !error )
      {
        اگر ( len )
          face->face_flags |= FT_FACE_FLAG_VARIATION;
        ورنہ
          face->face_flags &= ~FT_FACE_FLAG_VARIATION;
      }
    }

    /* enforce recomputation of auto-hinting data */
    اگر ( !error && face->autohint.finalizer )
    {
      face->autohint.finalizer( face->autohint.ڈیٹا );
      face->autohint.ڈیٹا = تہی;
    }

    واپس error;
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Get_MM_وزن_سمتیہ( FT_Face    face,
                          FT_UInt*   len,
                          FT_Fixed*  weightvector )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    اگر ( len && !weightvector )
      واپس FT_THROW( Invalid_دلیل );

    error = ft_face_get_mm_service( face, &service );
    اگر ( !error )
    {
      error = FT_ERR( Invalid_دلیل );
      اگر ( service->get_mm_weightvector )
        error = service->get_mm_weightvector( face, len, weightvector );
    }

    واپس error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_سیٹ_Var_Design_Coordinates( FT_Face    face,
                                 FT_UInt    عدد_coords,
                                 FT_Fixed*  coords )
  {
    FT_Error                      error;
    FT_Service_MultiMasters       service_mm   = تہی;
    FT_Service_MetricsVariations  service_mvar = تہی;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    اگر ( عدد_coords && !coords )
      واپس FT_THROW( Invalid_دلیل );

    اگر ( !عدد_coords && !FT_IS_VARIATION( face ) )
      واپس FT_Err_Ok;  /* nothing to be done */

    error = ft_face_get_mm_service( face, &service_mm );
    اگر ( !error )
    {
      error = FT_ERR( Invalid_دلیل );
      اگر ( service_mm->سیٹ_var_design )
        error = service_mm->سیٹ_var_design( face, عدد_coords, coords );

      اگر ( !error || error == -1 || error == -2 )
      {
        FT_Bool  is_variation_old = FT_IS_VARIATION( face );


        اگر ( error != -1 )
        {
          اگر ( error == -2 ) /* -2 means is_variable. */
          {
            face->face_flags |= FT_FACE_FLAG_VARIATION;
            error             = FT_Err_Ok;
          }
          ورنہ
            face->face_flags &= ~FT_FACE_FLAG_VARIATION;
        }

        اگر ( service_mm->construct_ps_name )
        {
          اگر ( error == -1 )
          {
            /* The PS name of a named instance and a non-named instance */
            /* usually differs, even if the axis values are identical.  */
            اگر ( is_variation_old != FT_IS_VARIATION( face ) )
              service_mm->construct_ps_name( face );
          }
          ورنہ
            service_mm->construct_ps_name( face );
        }
      }

      /* internal error code -1 means `no change'; we can exit immediately */
      اگر ( error == -1 )
        واپس FT_Err_Ok;
    }

    اگر ( !error )
    {
      (خالی)ft_face_get_mvar_service( face, &service_mvar );

      اگر ( service_mvar && service_mvar->metrics_adjust )
        service_mvar->metrics_adjust( face );
    }

    /* enforce recomputation of auto-hinting data */
    اگر ( !error && face->autohint.finalizer )
    {
      face->autohint.finalizer( face->autohint.ڈیٹا );
      face->autohint.ڈیٹا = تہی;
    }

    واپس error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Var_Design_Coordinates( FT_Face    face,
                                 FT_UInt    عدد_coords,
                                 FT_Fixed*  coords )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    اگر ( !coords )
      واپس FT_THROW( Invalid_دلیل );

    error = ft_face_get_mm_service( face, &service );
    اگر ( !error )
    {
      error = FT_ERR( Invalid_دلیل );
      اگر ( service->get_var_design )
        error = service->get_var_design( face, عدد_coords, coords );
    }

    واپس error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_سیٹ_MM_Blend_Coordinates( FT_Face    face,
                               FT_UInt    عدد_coords,
                               FT_Fixed*  coords )
  {
    FT_Error                      error;
    FT_Service_MultiMasters       service_mm   = تہی;
    FT_Service_MetricsVariations  service_mvar = تہی;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    اگر ( عدد_coords && !coords )
      واپس FT_THROW( Invalid_دلیل );

    error = ft_face_get_mm_service( face, &service_mm );
    اگر ( !error )
    {
      error = FT_ERR( Invalid_دلیل );
      اگر ( service_mm->سیٹ_mm_blend )
        error = service_mm->سیٹ_mm_blend( face, عدد_coords, coords );

      اگر ( !error || error == -1 )
      {
        FT_Bool  is_variation_old = FT_IS_VARIATION( face );


        اگر ( عدد_coords )
          face->face_flags |= FT_FACE_FLAG_VARIATION;
        ورنہ
          face->face_flags &= ~FT_FACE_FLAG_VARIATION;

        اگر ( service_mm->construct_ps_name )
        {
          اگر ( error == -1 )
          {
            /* The PS name of a named instance and a non-named instance */
            /* usually differs, even if the axis values are identical.  */
            اگر ( is_variation_old != FT_IS_VARIATION( face ) )
              service_mm->construct_ps_name( face );
          }
          ورنہ
            service_mm->construct_ps_name( face );
        }
      }

      /* internal error code -1 means `no change'; we can exit immediately */
      اگر ( error == -1 )
        واپس FT_Err_Ok;
    }

    اگر ( !error )
    {
      (خالی)ft_face_get_mvar_service( face, &service_mvar );

      اگر ( service_mvar && service_mvar->metrics_adjust )
        service_mvar->metrics_adjust( face );
    }

    /* enforce recomputation of auto-hinting data */
    اگر ( !error && face->autohint.finalizer )
    {
      face->autohint.finalizer( face->autohint.ڈیٹا );
      face->autohint.ڈیٹا = تہی;
    }

    واپس error;
  }


  /* documentation is in ftmm.h */

  /* This is exactly the same as the previous function.  It exists for */
  /* orthogonality.                                                    */

  FT_EXPORT_DEF( FT_Error )
  FT_سیٹ_Var_Blend_Coordinates( FT_Face    face,
                                FT_UInt    عدد_coords,
                                FT_Fixed*  coords )
  {
    FT_Error                      error;
    FT_Service_MultiMasters       service_mm   = تہی;
    FT_Service_MetricsVariations  service_mvar = تہی;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    اگر ( عدد_coords && !coords )
      واپس FT_THROW( Invalid_دلیل );

    error = ft_face_get_mm_service( face, &service_mm );
    اگر ( !error )
    {
      error = FT_ERR( Invalid_دلیل );
      اگر ( service_mm->سیٹ_mm_blend )
        error = service_mm->سیٹ_mm_blend( face, عدد_coords, coords );

      اگر ( !error || error == -1 || error == -2 )
      {
        FT_Bool  is_variation_old = FT_IS_VARIATION( face );


        اگر ( error != -1 )
        {
          اگر ( error == -2 ) /* -2 means is_variable. */
          {
            face->face_flags |= FT_FACE_FLAG_VARIATION;
            error             = FT_Err_Ok;
          }
          ورنہ
            face->face_flags &= ~FT_FACE_FLAG_VARIATION;
        }

        اگر ( service_mm->construct_ps_name )
        {
          اگر ( error == -1 )
          {
            /* The PS name of a named instance and a non-named instance */
            /* usually differs, even if the axis values are identical.  */
            اگر ( is_variation_old != FT_IS_VARIATION( face ) )
              service_mm->construct_ps_name( face );
          }
          ورنہ
            service_mm->construct_ps_name( face );
        }
      }

      /* internal error code -1 means `no change'; we can exit immediately */
      اگر ( error == -1 )
        واپس FT_Err_Ok;
    }

    اگر ( !error )
    {
      (خالی)ft_face_get_mvar_service( face, &service_mvar );

      اگر ( service_mvar && service_mvar->metrics_adjust )
        service_mvar->metrics_adjust( face );
    }

    /* enforce recomputation of auto-hinting data */
    اگر ( !error && face->autohint.finalizer )
    {
      face->autohint.finalizer( face->autohint.ڈیٹا );
      face->autohint.ڈیٹا = تہی;
    }

    واپس error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_MM_Blend_Coordinates( FT_Face    face,
                               FT_UInt    عدد_coords,
                               FT_Fixed*  coords )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    اگر ( !coords )
      واپس FT_THROW( Invalid_دلیل );

    error = ft_face_get_mm_service( face, &service );
    اگر ( !error )
    {
      error = FT_ERR( Invalid_دلیل );
      اگر ( service->get_mm_blend )
        error = service->get_mm_blend( face, عدد_coords, coords );
    }

    واپس error;
  }


  /* documentation is in ftmm.h */

  /* This is exactly the same as the previous function.  It exists for */
  /* orthogonality.                                                    */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Var_Blend_Coordinates( FT_Face    face,
                                FT_UInt    عدد_coords,
                                FT_Fixed*  coords )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    اگر ( !coords )
      واپس FT_THROW( Invalid_دلیل );

    error = ft_face_get_mm_service( face, &service );
    اگر ( !error )
    {
      error = FT_ERR( Invalid_دلیل );
      اگر ( service->get_mm_blend )
        error = service->get_mm_blend( face, عدد_coords, coords );
    }

    واپس error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Var_Axis_Flags( FT_MM_Var*  master,
                         FT_UInt     axis_اشاریہ,
                         FT_UInt*    flags )
  {
    FT_UShort*  axis_flags;


    اگر ( !master || !flags )
      واپس FT_THROW( Invalid_دلیل );

    اگر ( axis_اشاریہ >= master->عدد_axis )
      واپس FT_THROW( Invalid_دلیل );

    /* the axis flags array immediately follows the data of `master' */
    axis_flags = (FT_UShort*)&( master[1] );
    *flags     = axis_flags[axis_اشاریہ];

    واپس FT_Err_Ok;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_سیٹ_Named_Instance( FT_Face  face,
                         FT_UInt  instance_اشاریہ )
  {
    FT_Error  error;

    FT_Service_MultiMasters       service_mm   = تہی;
    FT_Service_MetricsVariations  service_mvar = تہی;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    error = ft_face_get_mm_service( face, &service_mm );
    اگر ( !error )
    {
      error = FT_ERR( Invalid_دلیل );
      اگر ( service_mm->سیٹ_named_instance )
        error = service_mm->سیٹ_named_instance( face, instance_اشاریہ );

      اگر ( !error || error == -1 )
      {
        FT_Bool  is_variation_old = FT_IS_VARIATION( face );


        face->face_flags &= ~FT_FACE_FLAG_VARIATION;
        face->face_اشاریہ  = ( instance_اشاریہ << 16 )        |
                            ( face->face_اشاریہ & 0xFFFFL );

        اگر ( service_mm->construct_ps_name )
        {
          اگر ( error == -1 )
          {
            /* The PS name of a named instance and a non-named instance */
            /* usually differs, even if the axis values are identical.  */
            اگر ( is_variation_old != FT_IS_VARIATION( face ) )
              service_mm->construct_ps_name( face );
          }
          ورنہ
            service_mm->construct_ps_name( face );
        }
      }

      /* internal error code -1 means `no change'; we can exit immediately */
      اگر ( error == -1 )
        واپس FT_Err_Ok;
    }

    اگر ( !error )
    {
      (خالی)ft_face_get_mvar_service( face, &service_mvar );

      اگر ( service_mvar && service_mvar->metrics_adjust )
        service_mvar->metrics_adjust( face );
    }

    /* enforce recomputation of auto-hinting data */
    اگر ( !error && face->autohint.finalizer )
    {
      face->autohint.finalizer( face->autohint.ڈیٹا );
      face->autohint.ڈیٹا = تہی;
    }

    واپس error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Default_Named_Instance( FT_Face   face,
                                 FT_UInt  *instance_اشاریہ )
  {
    FT_Error  error;

    FT_Service_MultiMasters  service_mm = تہی;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    error = ft_face_get_mm_service( face, &service_mm );
    اگر ( !error )
    {
      /* no error if `get_default_named_instance` is not available */
      اگر ( service_mm->get_default_named_instance )
        error = service_mm->get_default_named_instance( face,
                                                        instance_اشاریہ );
      ورنہ
        error = FT_Err_Ok;
    }

    واپس error;
  }


/*انتہا*/
