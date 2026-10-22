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


#include <freetype/internal/ftdebug.h>

#include <freetype/ftmm.h>
#include <freetype/internal/ftobjs.h>
#include <freetype/internal/services/svmm.h>
#include <freetype/internal/services/svmetric.h>


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#undef  FT_COMPONENT
#define FT_COMPONENT  mm


  static FT_Error
  ft_face_get_mm_service( FT_Face                   face,
                          FT_Service_MultiMasters  *aservice )
  {
    FT_Error  error;


    *aservice = NULL;

    ꦪꦼꦤ꧀ ( !face )
      ꦮꦭꦶ FT_THROW( Invalid_Face_Handle );

    error = FT_ERR( Invalid_Argument );

    ꦪꦼꦤ꧀ ( FT_HAS_MULTIPLE_MASTERS( face ) )
    {
      FT_FACE_LOOKUP_SERVICE( face,
                              *aservice,
                              MULTI_MASTERS );

      ꦪꦼꦤ꧀ ( *aservice )
        error = FT_Err_Ok;
    }

    ꦮꦭꦶ error;
  }


  static FT_Error
  ft_face_get_mvar_service( FT_Face                        face,
                            FT_Service_MetricsVariations  *aservice )
  {
    FT_Error  error;


    *aservice = NULL;

    ꦪꦼꦤ꧀ ( !face )
      ꦮꦭꦶ FT_THROW( Invalid_Face_Handle );

    error = FT_ERR( Invalid_Argument );

    ꦪꦼꦤ꧀ ( FT_HAS_MULTIPLE_MASTERS( face ) )
    {
      FT_FACE_LOOKUP_SERVICE( face,
                              *aservice,
                              METRICS_VARIATIONS );

      ꦪꦼꦤ꧀ ( *aservice )
        error = FT_Err_Ok;
    }

    ꦮꦭꦶ error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Multi_Master( FT_Face           face,
                       FT_Multi_Master  *amaster )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    ꦪꦼꦤ꧀ ( !amaster )
      ꦮꦭꦶ FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service );
    ꦪꦼꦤ꧀ ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      ꦪꦼꦤ꧀ ( service->get_mm )
        error = service->get_mm( face, amaster );
    }

    ꦮꦭꦶ error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_MM_Var( FT_Face      face,
                 FT_MM_Var*  *amaster )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    ꦪꦼꦤ꧀ ( !amaster )
      ꦮꦭꦶ FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service );
    ꦪꦼꦤ꧀ ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      ꦪꦼꦤ꧀ ( service->get_mm_var )
        error = service->get_mm_var( face, amaster );
    }

    ꦮꦭꦶ error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Done_MM_Var( FT_Library  library,
                  FT_MM_Var*  amaster )
  {
    FT_Memory  memory;


    ꦪꦼꦤ꧀ ( !library )
      ꦮꦭꦶ FT_THROW( Invalid_Library_Handle );

    memory = library->memory;
    FT_FREE( amaster );

    ꦮꦭꦶ FT_Err_Ok;
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

    ꦪꦼꦤ꧀ ( num_coords && !coords )
      ꦮꦭꦶ FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service );
    ꦪꦼꦤ꧀ ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      ꦪꦼꦤ꧀ ( service->set_mm_design )
        error = service->set_mm_design( face, num_coords, coords );

      ꦪꦼꦤ꧀ ( !error )
      {
        ꦪꦼꦤ꧀ ( num_coords )
          face->face_flags |= FT_FACE_FLAG_VARIATION;
        ꦭꦶꦪ
          face->face_flags &= ~FT_FACE_FLAG_VARIATION;
      }
    }

    /* enforce recomputation of auto-hinting data */
    ꦪꦼꦤ꧀ ( !error && face->autohint.finalizer )
    {
      face->autohint.finalizer( face->autohint.data );
      face->autohint.data = NULL;
    }

    ꦮꦭꦶ error;
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

    ꦪꦼꦤ꧀ ( len && !weightvector )
      ꦮꦭꦶ FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service );
    ꦪꦼꦤ꧀ ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      ꦪꦼꦤ꧀ ( service->set_mm_weightvector )
        error = service->set_mm_weightvector( face, len, weightvector );

      ꦪꦼꦤ꧀ ( !error )
      {
        ꦪꦼꦤ꧀ ( len )
          face->face_flags |= FT_FACE_FLAG_VARIATION;
        ꦭꦶꦪ
          face->face_flags &= ~FT_FACE_FLAG_VARIATION;
      }
    }

    /* enforce recomputation of auto-hinting data */
    ꦪꦼꦤ꧀ ( !error && face->autohint.finalizer )
    {
      face->autohint.finalizer( face->autohint.data );
      face->autohint.data = NULL;
    }

    ꦮꦭꦶ error;
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Get_MM_WeightVector( FT_Face    face,
                          FT_UInt*   len,
                          FT_Fixed*  weightvector )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    ꦪꦼꦤ꧀ ( len && !weightvector )
      ꦮꦭꦶ FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service );
    ꦪꦼꦤ꧀ ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      ꦪꦼꦤ꧀ ( service->get_mm_weightvector )
        error = service->get_mm_weightvector( face, len, weightvector );
    }

    ꦮꦭꦶ error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Set_Var_Design_Coordinates( FT_Face    face,
                                 FT_UInt    num_coords,
                                 FT_Fixed*  coords )
  {
    FT_Error                      error;
    FT_Service_MultiMasters       service_mm   = NULL;
    FT_Service_MetricsVariations  service_mvar = NULL;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    ꦪꦼꦤ꧀ ( num_coords && !coords )
      ꦮꦭꦶ FT_THROW( Invalid_Argument );

    ꦪꦼꦤ꧀ ( !num_coords && !FT_IS_VARIATION( face ) )
      ꦮꦭꦶ FT_Err_Ok;  /* nothing to be done */

    error = ft_face_get_mm_service( face, &service_mm );
    ꦪꦼꦤ꧀ ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      ꦪꦼꦤ꧀ ( service_mm->set_var_design )
        error = service_mm->set_var_design( face, num_coords, coords );

      ꦪꦼꦤ꧀ ( !error || error == -1 || error == -2 )
      {
        FT_Bool  is_variation_old = FT_IS_VARIATION( face );


        ꦪꦼꦤ꧀ ( error != -1 )
        {
          ꦪꦼꦤ꧀ ( error == -2 ) /* -2 means is_variable. */
          {
            face->face_flags |= FT_FACE_FLAG_VARIATION;
            error             = FT_Err_Ok;
          }
          ꦭꦶꦪ
            face->face_flags &= ~FT_FACE_FLAG_VARIATION;
        }

        ꦪꦼꦤ꧀ ( service_mm->construct_ps_name )
        {
          ꦪꦼꦤ꧀ ( error == -1 )
          {
            /* The PS name of a named instance and a non-named instance */
            /* usually differs, even if the axis values are identical.  */
            ꦪꦼꦤ꧀ ( is_variation_old != FT_IS_VARIATION( face ) )
              service_mm->construct_ps_name( face );
          }
          ꦭꦶꦪ
            service_mm->construct_ps_name( face );
        }
      }

      /* internal error code -1 means `no change'; we can exit immediately */
      ꦪꦼꦤ꧀ ( error == -1 )
        ꦮꦭꦶ FT_Err_Ok;
    }

    ꦪꦼꦤ꧀ ( !error )
    {
      (void)ft_face_get_mvar_service( face, &service_mvar );

      ꦪꦼꦤ꧀ ( service_mvar && service_mvar->metrics_adjust )
        service_mvar->metrics_adjust( face );
    }

    /* enforce recomputation of auto-hinting data */
    ꦪꦼꦤ꧀ ( !error && face->autohint.finalizer )
    {
      face->autohint.finalizer( face->autohint.data );
      face->autohint.data = NULL;
    }

    ꦮꦭꦶ error;
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

    ꦪꦼꦤ꧀ ( !coords )
      ꦮꦭꦶ FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service );
    ꦪꦼꦤ꧀ ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      ꦪꦼꦤ꧀ ( service->get_var_design )
        error = service->get_var_design( face, num_coords, coords );
    }

    ꦮꦭꦶ error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Set_MM_Blend_Coordinates( FT_Face    face,
                               FT_UInt    num_coords,
                               FT_Fixed*  coords )
  {
    FT_Error                      error;
    FT_Service_MultiMasters       service_mm   = NULL;
    FT_Service_MetricsVariations  service_mvar = NULL;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    ꦪꦼꦤ꧀ ( num_coords && !coords )
      ꦮꦭꦶ FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service_mm );
    ꦪꦼꦤ꧀ ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      ꦪꦼꦤ꧀ ( service_mm->set_mm_blend )
        error = service_mm->set_mm_blend( face, num_coords, coords );

      ꦪꦼꦤ꧀ ( !error || error == -1 )
      {
        FT_Bool  is_variation_old = FT_IS_VARIATION( face );


        ꦪꦼꦤ꧀ ( num_coords )
          face->face_flags |= FT_FACE_FLAG_VARIATION;
        ꦭꦶꦪ
          face->face_flags &= ~FT_FACE_FLAG_VARIATION;

        ꦪꦼꦤ꧀ ( service_mm->construct_ps_name )
        {
          ꦪꦼꦤ꧀ ( error == -1 )
          {
            /* The PS name of a named instance and a non-named instance */
            /* usually differs, even if the axis values are identical.  */
            ꦪꦼꦤ꧀ ( is_variation_old != FT_IS_VARIATION( face ) )
              service_mm->construct_ps_name( face );
          }
          ꦭꦶꦪ
            service_mm->construct_ps_name( face );
        }
      }

      /* internal error code -1 means `no change'; we can exit immediately */
      ꦪꦼꦤ꧀ ( error == -1 )
        ꦮꦭꦶ FT_Err_Ok;
    }

    ꦪꦼꦤ꧀ ( !error )
    {
      (void)ft_face_get_mvar_service( face, &service_mvar );

      ꦪꦼꦤ꧀ ( service_mvar && service_mvar->metrics_adjust )
        service_mvar->metrics_adjust( face );
    }

    /* enforce recomputation of auto-hinting data */
    ꦪꦼꦤ꧀ ( !error && face->autohint.finalizer )
    {
      face->autohint.finalizer( face->autohint.data );
      face->autohint.data = NULL;
    }

    ꦮꦭꦶ error;
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
    FT_Service_MultiMasters       service_mm   = NULL;
    FT_Service_MetricsVariations  service_mvar = NULL;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    ꦪꦼꦤ꧀ ( num_coords && !coords )
      ꦮꦭꦶ FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service_mm );
    ꦪꦼꦤ꧀ ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      ꦪꦼꦤ꧀ ( service_mm->set_mm_blend )
        error = service_mm->set_mm_blend( face, num_coords, coords );

      ꦪꦼꦤ꧀ ( !error || error == -1 || error == -2 )
      {
        FT_Bool  is_variation_old = FT_IS_VARIATION( face );


        ꦪꦼꦤ꧀ ( error != -1 )
        {
          ꦪꦼꦤ꧀ ( error == -2 ) /* -2 means is_variable. */
          {
            face->face_flags |= FT_FACE_FLAG_VARIATION;
            error             = FT_Err_Ok;
          }
          ꦭꦶꦪ
            face->face_flags &= ~FT_FACE_FLAG_VARIATION;
        }

        ꦪꦼꦤ꧀ ( service_mm->construct_ps_name )
        {
          ꦪꦼꦤ꧀ ( error == -1 )
          {
            /* The PS name of a named instance and a non-named instance */
            /* usually differs, even if the axis values are identical.  */
            ꦪꦼꦤ꧀ ( is_variation_old != FT_IS_VARIATION( face ) )
              service_mm->construct_ps_name( face );
          }
          ꦭꦶꦪ
            service_mm->construct_ps_name( face );
        }
      }

      /* internal error code -1 means `no change'; we can exit immediately */
      ꦪꦼꦤ꧀ ( error == -1 )
        ꦮꦭꦶ FT_Err_Ok;
    }

    ꦪꦼꦤ꧀ ( !error )
    {
      (void)ft_face_get_mvar_service( face, &service_mvar );

      ꦪꦼꦤ꧀ ( service_mvar && service_mvar->metrics_adjust )
        service_mvar->metrics_adjust( face );
    }

    /* enforce recomputation of auto-hinting data */
    ꦪꦼꦤ꧀ ( !error && face->autohint.finalizer )
    {
      face->autohint.finalizer( face->autohint.data );
      face->autohint.data = NULL;
    }

    ꦮꦭꦶ error;
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

    ꦪꦼꦤ꧀ ( !coords )
      ꦮꦭꦶ FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service );
    ꦪꦼꦤ꧀ ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      ꦪꦼꦤ꧀ ( service->get_mm_blend )
        error = service->get_mm_blend( face, num_coords, coords );
    }

    ꦮꦭꦶ error;
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

    ꦪꦼꦤ꧀ ( !coords )
      ꦮꦭꦶ FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service );
    ꦪꦼꦤ꧀ ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      ꦪꦼꦤ꧀ ( service->get_mm_blend )
        error = service->get_mm_blend( face, num_coords, coords );
    }

    ꦮꦭꦶ error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Var_Axis_Flags( FT_MM_Var*  master,
                         FT_UInt     axis_index,
                         FT_UInt*    flags )
  {
    FT_UShort*  axis_flags;


    ꦪꦼꦤ꧀ ( !master || !flags )
      ꦮꦭꦶ FT_THROW( Invalid_Argument );

    ꦪꦼꦤ꧀ ( axis_index >= master->num_axis )
      ꦮꦭꦶ FT_THROW( Invalid_Argument );

    /* the axis flags array immediately follows the data of `master' */
    axis_flags = (FT_UShort*)&( master[1] );
    *flags     = axis_flags[axis_index];

    ꦮꦭꦶ FT_Err_Ok;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Set_Named_Instance( FT_Face  face,
                         FT_UInt  instance_index )
  {
    FT_Error  error;

    FT_Service_MultiMasters       service_mm   = NULL;
    FT_Service_MetricsVariations  service_mvar = NULL;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    error = ft_face_get_mm_service( face, &service_mm );
    ꦪꦼꦤ꧀ ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      ꦪꦼꦤ꧀ ( service_mm->set_named_instance )
        error = service_mm->set_named_instance( face, instance_index );

      ꦪꦼꦤ꧀ ( !error || error == -1 )
      {
        FT_Bool  is_variation_old = FT_IS_VARIATION( face );


        face->face_flags &= ~FT_FACE_FLAG_VARIATION;
        face->face_index  = ( instance_index << 16 )        |
                            ( face->face_index & 0xFFFFL );

        ꦪꦼꦤ꧀ ( service_mm->construct_ps_name )
        {
          ꦪꦼꦤ꧀ ( error == -1 )
          {
            /* The PS name of a named instance and a non-named instance */
            /* usually differs, even if the axis values are identical.  */
            ꦪꦼꦤ꧀ ( is_variation_old != FT_IS_VARIATION( face ) )
              service_mm->construct_ps_name( face );
          }
          ꦭꦶꦪ
            service_mm->construct_ps_name( face );
        }
      }

      /* internal error code -1 means `no change'; we can exit immediately */
      ꦪꦼꦤ꧀ ( error == -1 )
        ꦮꦭꦶ FT_Err_Ok;
    }

    ꦪꦼꦤ꧀ ( !error )
    {
      (void)ft_face_get_mvar_service( face, &service_mvar );

      ꦪꦼꦤ꧀ ( service_mvar && service_mvar->metrics_adjust )
        service_mvar->metrics_adjust( face );
    }

    /* enforce recomputation of auto-hinting data */
    ꦪꦼꦤ꧀ ( !error && face->autohint.finalizer )
    {
      face->autohint.finalizer( face->autohint.data );
      face->autohint.data = NULL;
    }

    ꦮꦭꦶ error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Default_Named_Instance( FT_Face   face,
                                 FT_UInt  *instance_index )
  {
    FT_Error  error;

    FT_Service_MultiMasters  service_mm = NULL;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    error = ft_face_get_mm_service( face, &service_mm );
    ꦪꦼꦤ꧀ ( !error )
    {
      /* no error if `get_default_named_instance` is not available */
      ꦪꦼꦤ꧀ ( service_mm->get_default_named_instance )
        error = service_mm->get_default_named_instance( face,
                                                        instance_index );
      ꦭꦶꦪ
        error = FT_Err_Ok;
    }

    ꦮꦭꦶ error;
  }


/* END */
