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


#ضمن <freetype/internal/ftdebug.h>

#ضمن <freetype/ftmm.h>
#ضمن <freetype/internal/ftobjs.h>
#ضمن <freetype/internal/services/svmm.h>
#ضمن <freetype/internal/services/svmetric.h>


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#undef  FT_COMPONENT
#define FT_COMPONENT  mm


  ساكن FT_Error
  ft_face_get_mm_service( FT_Face                   face,
                          FT_Service_MultiMasters  *aservice )
  {
    FT_Error  error;


    *aservice = عدم;

    אם ( !face )
      החזר FT_THROW( Invalid_Face_Handle );

    error = FT_ERR( Invalid_Argument );

    אם ( FT_HAS_MULTIPLE_MASTERS( face ) )
    {
      FT_FACE_LOOKUP_SERVICE( face,
                              *aservice,
                              MULTI_MASTERS );

      אם ( *aservice )
        error = FT_Err_Ok;
    }

    החזר error;
  }


  ساكن FT_Error
  ft_face_get_mvar_service( FT_Face                        face,
                            FT_Service_MetricsVariations  *aservice )
  {
    FT_Error  error;


    *aservice = عدم;

    אם ( !face )
      החזר FT_THROW( Invalid_Face_Handle );

    error = FT_ERR( Invalid_Argument );

    אם ( FT_HAS_MULTIPLE_MASTERS( face ) )
    {
      FT_FACE_LOOKUP_SERVICE( face,
                              *aservice,
                              METRICS_VARIATIONS );

      אם ( *aservice )
        error = FT_Err_Ok;
    }

    החזר error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Multi_Master( FT_Face           face,
                       FT_Multi_Master  *amaster )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    אם ( !amaster )
      החזר FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service );
    אם ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      אם ( service->get_mm )
        error = service->get_mm( face, amaster );
    }

    החזר error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_MM_Var( FT_Face      face,
                 FT_MM_Var*  *amaster )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    אם ( !amaster )
      החזר FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service );
    אם ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      אם ( service->get_mm_var )
        error = service->get_mm_var( face, amaster );
    }

    החזר error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Done_MM_Var( FT_Library  library,
                  FT_MM_Var*  amaster )
  {
    FT_Memory  memory;


    אם ( !library )
      החזר FT_THROW( Invalid_Library_Handle );

    memory = library->memory;
    FT_FREE( amaster );

    החזר FT_Err_Ok;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Set_MM_Design_Coordinates( FT_Face   face,
                                FT_UInt   num_coords,
                                FT_Long*  coords )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    אם ( num_coords && !coords )
      החזר FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service );
    אם ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      אם ( service->set_mm_design )
        error = service->set_mm_design( face, num_coords, coords );

      אם ( !error )
      {
        אם ( num_coords )
          face->face_flags |= FT_FACE_FLAG_VARIATION;
        אחרת
          face->face_flags &= ~FT_FACE_FLAG_VARIATION;
      }
    }

    /* enforce recomputation of auto-hinting data */
    אם ( !error && face->autohint.finalizer )
    {
      face->autohint.finalizer( face->autohint.data );
      face->autohint.data = عدم;
    }

    החזר error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Set_MM_WeightVector( FT_Face    face,
                          FT_UInt    len,
                          FT_Fixed*  weightvector )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    אם ( len && !weightvector )
      החזר FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service );
    אם ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      אם ( service->set_mm_weightvector )
        error = service->set_mm_weightvector( face, len, weightvector );

      אם ( !error )
      {
        אם ( len )
          face->face_flags |= FT_FACE_FLAG_VARIATION;
        אחרת
          face->face_flags &= ~FT_FACE_FLAG_VARIATION;
      }
    }

    /* enforce recomputation of auto-hinting data */
    אם ( !error && face->autohint.finalizer )
    {
      face->autohint.finalizer( face->autohint.data );
      face->autohint.data = عدم;
    }

    החזר error;
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Get_MM_WeightVector( FT_Face    face,
                          FT_UInt*   len,
                          FT_Fixed*  weightvector )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    אם ( len && !weightvector )
      החזר FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service );
    אם ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      אם ( service->get_mm_weightvector )
        error = service->get_mm_weightvector( face, len, weightvector );
    }

    החזר error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Set_Var_Design_Coordinates( FT_Face    face,
                                 FT_UInt    num_coords,
                                 FT_Fixed*  coords )
  {
    FT_Error                      error;
    FT_Service_MultiMasters       service_mm   = عدم;
    FT_Service_MetricsVariations  service_mvar = عدم;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    אם ( num_coords && !coords )
      החזר FT_THROW( Invalid_Argument );

    אם ( !num_coords && !FT_IS_VARIATION( face ) )
      החזר FT_Err_Ok;  /* nothing to be done */

    error = ft_face_get_mm_service( face, &service_mm );
    אם ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      אם ( service_mm->set_var_design )
        error = service_mm->set_var_design( face, num_coords, coords );

      אם ( !error || error == -1 || error == -2 )
      {
        FT_Bool  is_variation_old = FT_IS_VARIATION( face );


        אם ( error != -1 )
        {
          אם ( error == -2 ) /* -2 means is_variable. */
          {
            face->face_flags |= FT_FACE_FLAG_VARIATION;
            error             = FT_Err_Ok;
          }
          אחרת
            face->face_flags &= ~FT_FACE_FLAG_VARIATION;
        }

        אם ( service_mm->construct_ps_name )
        {
          אם ( error == -1 )
          {
            /* The PS name of a named instance and a non-named instance */
            /* usually differs, even if the axis values are identical.  */
            אם ( is_variation_old != FT_IS_VARIATION( face ) )
              service_mm->construct_ps_name( face );
          }
          אחרת
            service_mm->construct_ps_name( face );
        }
      }

      /* internal error code -1 means `no change'; we can exit immediately */
      אם ( error == -1 )
        החזר FT_Err_Ok;
    }

    אם ( !error )
    {
      (فراغ)ft_face_get_mvar_service( face, &service_mvar );

      אם ( service_mvar && service_mvar->metrics_adjust )
        service_mvar->metrics_adjust( face );
    }

    /* enforce recomputation of auto-hinting data */
    אם ( !error && face->autohint.finalizer )
    {
      face->autohint.finalizer( face->autohint.data );
      face->autohint.data = عدم;
    }

    החזר error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Var_Design_Coordinates( FT_Face    face,
                                 FT_UInt    num_coords,
                                 FT_Fixed*  coords )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    אם ( !coords )
      החזר FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service );
    אם ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      אם ( service->get_var_design )
        error = service->get_var_design( face, num_coords, coords );
    }

    החזר error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Set_MM_Blend_Coordinates( FT_Face    face,
                               FT_UInt    num_coords,
                               FT_Fixed*  coords )
  {
    FT_Error                      error;
    FT_Service_MultiMasters       service_mm   = عدم;
    FT_Service_MetricsVariations  service_mvar = عدم;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    אם ( num_coords && !coords )
      החזר FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service_mm );
    אם ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      אם ( service_mm->set_mm_blend )
        error = service_mm->set_mm_blend( face, num_coords, coords );

      אם ( !error || error == -1 )
      {
        FT_Bool  is_variation_old = FT_IS_VARIATION( face );


        אם ( num_coords )
          face->face_flags |= FT_FACE_FLAG_VARIATION;
        אחרת
          face->face_flags &= ~FT_FACE_FLAG_VARIATION;

        אם ( service_mm->construct_ps_name )
        {
          אם ( error == -1 )
          {
            /* The PS name of a named instance and a non-named instance */
            /* usually differs, even if the axis values are identical.  */
            אם ( is_variation_old != FT_IS_VARIATION( face ) )
              service_mm->construct_ps_name( face );
          }
          אחרת
            service_mm->construct_ps_name( face );
        }
      }

      /* internal error code -1 means `no change'; we can exit immediately */
      אם ( error == -1 )
        החזר FT_Err_Ok;
    }

    אם ( !error )
    {
      (فراغ)ft_face_get_mvar_service( face, &service_mvar );

      אם ( service_mvar && service_mvar->metrics_adjust )
        service_mvar->metrics_adjust( face );
    }

    /* enforce recomputation of auto-hinting data */
    אם ( !error && face->autohint.finalizer )
    {
      face->autohint.finalizer( face->autohint.data );
      face->autohint.data = عدم;
    }

    החזר error;
  }


  /* documentation is in ftmm.h */

  /* This is exactly the same as the previous function.  It exists for */
  /* orthogonality.                                                    */

  FT_EXPORT_DEF( FT_Error )
  FT_Set_Var_Blend_Coordinates( FT_Face    face,
                                FT_UInt    num_coords,
                                FT_Fixed*  coords )
  {
    FT_Error                      error;
    FT_Service_MultiMasters       service_mm   = عدم;
    FT_Service_MetricsVariations  service_mvar = عدم;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    אם ( num_coords && !coords )
      החזר FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service_mm );
    אם ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      אם ( service_mm->set_mm_blend )
        error = service_mm->set_mm_blend( face, num_coords, coords );

      אם ( !error || error == -1 || error == -2 )
      {
        FT_Bool  is_variation_old = FT_IS_VARIATION( face );


        אם ( error != -1 )
        {
          אם ( error == -2 ) /* -2 means is_variable. */
          {
            face->face_flags |= FT_FACE_FLAG_VARIATION;
            error             = FT_Err_Ok;
          }
          אחרת
            face->face_flags &= ~FT_FACE_FLAG_VARIATION;
        }

        אם ( service_mm->construct_ps_name )
        {
          אם ( error == -1 )
          {
            /* The PS name of a named instance and a non-named instance */
            /* usually differs, even if the axis values are identical.  */
            אם ( is_variation_old != FT_IS_VARIATION( face ) )
              service_mm->construct_ps_name( face );
          }
          אחרת
            service_mm->construct_ps_name( face );
        }
      }

      /* internal error code -1 means `no change'; we can exit immediately */
      אם ( error == -1 )
        החזר FT_Err_Ok;
    }

    אם ( !error )
    {
      (فراغ)ft_face_get_mvar_service( face, &service_mvar );

      אם ( service_mvar && service_mvar->metrics_adjust )
        service_mvar->metrics_adjust( face );
    }

    /* enforce recomputation of auto-hinting data */
    אם ( !error && face->autohint.finalizer )
    {
      face->autohint.finalizer( face->autohint.data );
      face->autohint.data = عدم;
    }

    החזר error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_MM_Blend_Coordinates( FT_Face    face,
                               FT_UInt    num_coords,
                               FT_Fixed*  coords )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    אם ( !coords )
      החזר FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service );
    אם ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      אם ( service->get_mm_blend )
        error = service->get_mm_blend( face, num_coords, coords );
    }

    החזר error;
  }


  /* documentation is in ftmm.h */

  /* This is exactly the same as the previous function.  It exists for */
  /* orthogonality.                                                    */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Var_Blend_Coordinates( FT_Face    face,
                                FT_UInt    num_coords,
                                FT_Fixed*  coords )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    אם ( !coords )
      החזר FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service );
    אם ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      אם ( service->get_mm_blend )
        error = service->get_mm_blend( face, num_coords, coords );
    }

    החזר error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Var_Axis_Flags( FT_MM_Var*  master,
                         FT_UInt     axis_index,
                         FT_UInt*    flags )
  {
    FT_UShort*  axis_flags;


    אם ( !master || !flags )
      החזר FT_THROW( Invalid_Argument );

    אם ( axis_index >= master->num_axis )
      החזר FT_THROW( Invalid_Argument );

    /* the axis flags array immediately follows the data of `master' */
    axis_flags = (FT_UShort*)&( master[1] );
    *flags     = axis_flags[axis_index];

    החזר FT_Err_Ok;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Set_Named_Instance( FT_Face  face,
                         FT_UInt  instance_index )
  {
    FT_Error  error;

    FT_Service_MultiMasters       service_mm   = عدم;
    FT_Service_MetricsVariations  service_mvar = عدم;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    error = ft_face_get_mm_service( face, &service_mm );
    אם ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      אם ( service_mm->set_named_instance )
        error = service_mm->set_named_instance( face, instance_index );

      אם ( !error || error == -1 )
      {
        FT_Bool  is_variation_old = FT_IS_VARIATION( face );


        face->face_flags &= ~FT_FACE_FLAG_VARIATION;
        face->face_index  = ( instance_index << 16 )        |
                            ( face->face_index & 0xFFFFL );

        אם ( service_mm->construct_ps_name )
        {
          אם ( error == -1 )
          {
            /* The PS name of a named instance and a non-named instance */
            /* usually differs, even if the axis values are identical.  */
            אם ( is_variation_old != FT_IS_VARIATION( face ) )
              service_mm->construct_ps_name( face );
          }
          אחרת
            service_mm->construct_ps_name( face );
        }
      }

      /* internal error code -1 means `no change'; we can exit immediately */
      אם ( error == -1 )
        החזר FT_Err_Ok;
    }

    אם ( !error )
    {
      (فراغ)ft_face_get_mvar_service( face, &service_mvar );

      אם ( service_mvar && service_mvar->metrics_adjust )
        service_mvar->metrics_adjust( face );
    }

    /* enforce recomputation of auto-hinting data */
    אם ( !error && face->autohint.finalizer )
    {
      face->autohint.finalizer( face->autohint.data );
      face->autohint.data = عدم;
    }

    החזר error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Default_Named_Instance( FT_Face   face,
                                 FT_UInt  *instance_index )
  {
    FT_Error  error;

    FT_Service_MultiMasters  service_mm = عدم;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    error = ft_face_get_mm_service( face, &service_mm );
    אם ( !error )
    {
      /* no error if `get_default_named_instance` is not available */
      אם ( service_mm->get_default_named_instance )
        error = service_mm->get_default_named_instance( face,
                                                        instance_index );
      אחרת
        error = FT_Err_Ok;
    }

    החזר error;
  }


/* END */
