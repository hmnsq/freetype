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

    nếu ( !face )
      trả_về FT_THROW( Invalid_Face_Handle );

    error = FT_ERR( Invalid_Argument );

    nếu ( FT_HAS_MULTIPLE_MASTERS( face ) )
    {
      FT_FACE_LOOKUP_SERVICE( face,
                              *aservice,
                              MULTI_MASTERS );

      nếu ( *aservice )
        error = FT_Err_Ok;
    }

    trả_về error;
  }


  static FT_Error
  ft_face_get_mvar_service( FT_Face                        face,
                            FT_Service_MetricsVariations  *aservice )
  {
    FT_Error  error;


    *aservice = NULL;

    nếu ( !face )
      trả_về FT_THROW( Invalid_Face_Handle );

    error = FT_ERR( Invalid_Argument );

    nếu ( FT_HAS_MULTIPLE_MASTERS( face ) )
    {
      FT_FACE_LOOKUP_SERVICE( face,
                              *aservice,
                              METRICS_VARIATIONS );

      nếu ( *aservice )
        error = FT_Err_Ok;
    }

    trả_về error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Multi_Master( FT_Face           face,
                       FT_Multi_Master  *amaster )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    nếu ( !amaster )
      trả_về FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service );
    nếu ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      nếu ( service->get_mm )
        error = service->get_mm( face, amaster );
    }

    trả_về error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_MM_Var( FT_Face      face,
                 FT_MM_Var*  *amaster )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    nếu ( !amaster )
      trả_về FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service );
    nếu ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      nếu ( service->get_mm_var )
        error = service->get_mm_var( face, amaster );
    }

    trả_về error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Done_MM_Var( FT_Library  library,
                  FT_MM_Var*  amaster )
  {
    FT_Memory  memory;


    nếu ( !library )
      trả_về FT_THROW( Invalid_Library_Handle );

    memory = library->memory;
    FT_FREE( amaster );

    trả_về FT_Err_Ok;
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

    nếu ( num_coords && !coords )
      trả_về FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service );
    nếu ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      nếu ( service->set_mm_design )
        error = service->set_mm_design( face, num_coords, coords );

      nếu ( !error )
      {
        nếu ( num_coords )
          face->face_flags |= FT_FACE_FLAG_VARIATION;
        không_thì
          face->face_flags &= ~FT_FACE_FLAG_VARIATION;
      }
    }

    /* enforce recomputation of auto-hinting data */
    nếu ( !error && face->autohint.finalizer )
    {
      face->autohint.finalizer( face->autohint.data );
      face->autohint.data = NULL;
    }

    trả_về error;
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

    nếu ( len && !weightvector )
      trả_về FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service );
    nếu ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      nếu ( service->set_mm_weightvector )
        error = service->set_mm_weightvector( face, len, weightvector );

      nếu ( !error )
      {
        nếu ( len )
          face->face_flags |= FT_FACE_FLAG_VARIATION;
        không_thì
          face->face_flags &= ~FT_FACE_FLAG_VARIATION;
      }
    }

    /* enforce recomputation of auto-hinting data */
    nếu ( !error && face->autohint.finalizer )
    {
      face->autohint.finalizer( face->autohint.data );
      face->autohint.data = NULL;
    }

    trả_về error;
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Get_MM_WeightVector( FT_Face    face,
                          FT_UInt*   len,
                          FT_Fixed*  weightvector )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    nếu ( len && !weightvector )
      trả_về FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service );
    nếu ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      nếu ( service->get_mm_weightvector )
        error = service->get_mm_weightvector( face, len, weightvector );
    }

    trả_về error;
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

    nếu ( num_coords && !coords )
      trả_về FT_THROW( Invalid_Argument );

    nếu ( !num_coords && !FT_IS_VARIATION( face ) )
      trả_về FT_Err_Ok;  /* nothing to be done */

    error = ft_face_get_mm_service( face, &service_mm );
    nếu ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      nếu ( service_mm->set_var_design )
        error = service_mm->set_var_design( face, num_coords, coords );

      nếu ( !error || error == -1 || error == -2 )
      {
        FT_Bool  is_variation_old = FT_IS_VARIATION( face );


        nếu ( error != -1 )
        {
          nếu ( error == -2 ) /* -2 means is_variable. */
          {
            face->face_flags |= FT_FACE_FLAG_VARIATION;
            error             = FT_Err_Ok;
          }
          không_thì
            face->face_flags &= ~FT_FACE_FLAG_VARIATION;
        }

        nếu ( service_mm->construct_ps_name )
        {
          nếu ( error == -1 )
          {
            /* The PS name of a named instance and a non-named instance */
            /* usually differs, even if the axis values are identical.  */
            nếu ( is_variation_old != FT_IS_VARIATION( face ) )
              service_mm->construct_ps_name( face );
          }
          không_thì
            service_mm->construct_ps_name( face );
        }
      }

      /* internal error code -1 means `no change'; we can exit immediately */
      nếu ( error == -1 )
        trả_về FT_Err_Ok;
    }

    nếu ( !error )
    {
      (void)ft_face_get_mvar_service( face, &service_mvar );

      nếu ( service_mvar && service_mvar->metrics_adjust )
        service_mvar->metrics_adjust( face );
    }

    /* enforce recomputation of auto-hinting data */
    nếu ( !error && face->autohint.finalizer )
    {
      face->autohint.finalizer( face->autohint.data );
      face->autohint.data = NULL;
    }

    trả_về error;
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

    nếu ( !coords )
      trả_về FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service );
    nếu ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      nếu ( service->get_var_design )
        error = service->get_var_design( face, num_coords, coords );
    }

    trả_về error;
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

    nếu ( num_coords && !coords )
      trả_về FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service_mm );
    nếu ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      nếu ( service_mm->set_mm_blend )
        error = service_mm->set_mm_blend( face, num_coords, coords );

      nếu ( !error || error == -1 )
      {
        FT_Bool  is_variation_old = FT_IS_VARIATION( face );


        nếu ( num_coords )
          face->face_flags |= FT_FACE_FLAG_VARIATION;
        không_thì
          face->face_flags &= ~FT_FACE_FLAG_VARIATION;

        nếu ( service_mm->construct_ps_name )
        {
          nếu ( error == -1 )
          {
            /* The PS name of a named instance and a non-named instance */
            /* usually differs, even if the axis values are identical.  */
            nếu ( is_variation_old != FT_IS_VARIATION( face ) )
              service_mm->construct_ps_name( face );
          }
          không_thì
            service_mm->construct_ps_name( face );
        }
      }

      /* internal error code -1 means `no change'; we can exit immediately */
      nếu ( error == -1 )
        trả_về FT_Err_Ok;
    }

    nếu ( !error )
    {
      (void)ft_face_get_mvar_service( face, &service_mvar );

      nếu ( service_mvar && service_mvar->metrics_adjust )
        service_mvar->metrics_adjust( face );
    }

    /* enforce recomputation of auto-hinting data */
    nếu ( !error && face->autohint.finalizer )
    {
      face->autohint.finalizer( face->autohint.data );
      face->autohint.data = NULL;
    }

    trả_về error;
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

    nếu ( num_coords && !coords )
      trả_về FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service_mm );
    nếu ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      nếu ( service_mm->set_mm_blend )
        error = service_mm->set_mm_blend( face, num_coords, coords );

      nếu ( !error || error == -1 || error == -2 )
      {
        FT_Bool  is_variation_old = FT_IS_VARIATION( face );


        nếu ( error != -1 )
        {
          nếu ( error == -2 ) /* -2 means is_variable. */
          {
            face->face_flags |= FT_FACE_FLAG_VARIATION;
            error             = FT_Err_Ok;
          }
          không_thì
            face->face_flags &= ~FT_FACE_FLAG_VARIATION;
        }

        nếu ( service_mm->construct_ps_name )
        {
          nếu ( error == -1 )
          {
            /* The PS name of a named instance and a non-named instance */
            /* usually differs, even if the axis values are identical.  */
            nếu ( is_variation_old != FT_IS_VARIATION( face ) )
              service_mm->construct_ps_name( face );
          }
          không_thì
            service_mm->construct_ps_name( face );
        }
      }

      /* internal error code -1 means `no change'; we can exit immediately */
      nếu ( error == -1 )
        trả_về FT_Err_Ok;
    }

    nếu ( !error )
    {
      (void)ft_face_get_mvar_service( face, &service_mvar );

      nếu ( service_mvar && service_mvar->metrics_adjust )
        service_mvar->metrics_adjust( face );
    }

    /* enforce recomputation of auto-hinting data */
    nếu ( !error && face->autohint.finalizer )
    {
      face->autohint.finalizer( face->autohint.data );
      face->autohint.data = NULL;
    }

    trả_về error;
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

    nếu ( !coords )
      trả_về FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service );
    nếu ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      nếu ( service->get_mm_blend )
        error = service->get_mm_blend( face, num_coords, coords );
    }

    trả_về error;
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

    nếu ( !coords )
      trả_về FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service );
    nếu ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      nếu ( service->get_mm_blend )
        error = service->get_mm_blend( face, num_coords, coords );
    }

    trả_về error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Var_Axis_Flags( FT_MM_Var*  master,
                         FT_UInt     axis_index,
                         FT_UInt*    flags )
  {
    FT_UShort*  axis_flags;


    nếu ( !master || !flags )
      trả_về FT_THROW( Invalid_Argument );

    nếu ( axis_index >= master->num_axis )
      trả_về FT_THROW( Invalid_Argument );

    /* the axis flags array immediately follows the data of `master' */
    axis_flags = (FT_UShort*)&( master[1] );
    *flags     = axis_flags[axis_index];

    trả_về FT_Err_Ok;
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
    nếu ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      nếu ( service_mm->set_named_instance )
        error = service_mm->set_named_instance( face, instance_index );

      nếu ( !error || error == -1 )
      {
        FT_Bool  is_variation_old = FT_IS_VARIATION( face );


        face->face_flags &= ~FT_FACE_FLAG_VARIATION;
        face->face_index  = ( instance_index << 16 )        |
                            ( face->face_index & 0xFFFFL );

        nếu ( service_mm->construct_ps_name )
        {
          nếu ( error == -1 )
          {
            /* The PS name of a named instance and a non-named instance */
            /* usually differs, even if the axis values are identical.  */
            nếu ( is_variation_old != FT_IS_VARIATION( face ) )
              service_mm->construct_ps_name( face );
          }
          không_thì
            service_mm->construct_ps_name( face );
        }
      }

      /* internal error code -1 means `no change'; we can exit immediately */
      nếu ( error == -1 )
        trả_về FT_Err_Ok;
    }

    nếu ( !error )
    {
      (void)ft_face_get_mvar_service( face, &service_mvar );

      nếu ( service_mvar && service_mvar->metrics_adjust )
        service_mvar->metrics_adjust( face );
    }

    /* enforce recomputation of auto-hinting data */
    nếu ( !error && face->autohint.finalizer )
    {
      face->autohint.finalizer( face->autohint.data );
      face->autohint.data = NULL;
    }

    trả_về error;
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
    nếu ( !error )
    {
      /* no error if `get_default_named_instance` is not available */
      nếu ( service_mm->get_default_named_instance )
        error = service_mm->get_default_named_instance( face,
                                                        instance_index );
      không_thì
        error = FT_Err_Ok;
    }

    trả_về error;
  }


/* END */
