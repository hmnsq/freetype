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


#अन्तर्भाव <freetype/internal/ftdebug.h>

#अन्तर्भाव <freetype/ftmm.h>
#अन्तर्भाव <freetype/internal/ftobjs.h>
#अन्तर्भाव <freetype/internal/services/svmm.h>
#अन्तर्भाव <freetype/internal/services/svmetric.h>


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#undef  FT_COMPONENT
#define FT_COMPONENT  mm


  स्थिर FT_Error
  ft_face_get_mm_service( FT_Face                   face,
                          FT_Service_MultiMasters  *aservice )
  {
    FT_Error  error;


    *aservice = रिक्त;

    जर ( !face )
      परत FT_THROW( Invalid_Face_Handle );

    error = FT_ERR( Invalid_Argument );

    जर ( FT_HAS_MULTIPLE_MASTERS( face ) )
    {
      FT_FACE_LOOKUP_SERVICE( face,
                              *aservice,
                              MULTI_MASTERS );

      जर ( *aservice )
        error = FT_Err_Ok;
    }

    परत error;
  }


  स्थिर FT_Error
  ft_face_get_mvar_service( FT_Face                        face,
                            FT_Service_MetricsVariations  *aservice )
  {
    FT_Error  error;


    *aservice = रिक्त;

    जर ( !face )
      परत FT_THROW( Invalid_Face_Handle );

    error = FT_ERR( Invalid_Argument );

    जर ( FT_HAS_MULTIPLE_MASTERS( face ) )
    {
      FT_FACE_LOOKUP_SERVICE( face,
                              *aservice,
                              METRICS_VARIATIONS );

      जर ( *aservice )
        error = FT_Err_Ok;
    }

    परत error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Multi_Master( FT_Face           face,
                       FT_Multi_Master  *amaster )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    जर ( !amaster )
      परत FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service );
    जर ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      जर ( service->get_mm )
        error = service->get_mm( face, amaster );
    }

    परत error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_MM_Var( FT_Face      face,
                 FT_MM_Var*  *amaster )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    जर ( !amaster )
      परत FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service );
    जर ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      जर ( service->get_mm_var )
        error = service->get_mm_var( face, amaster );
    }

    परत error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Done_MM_Var( FT_Library  library,
                  FT_MM_Var*  amaster )
  {
    FT_Memory  memory;


    जर ( !library )
      परत FT_THROW( Invalid_Library_Handle );

    memory = library->memory;
    FT_FREE( amaster );

    परत FT_Err_Ok;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Set_MM_Design_Coordinates( FT_Face   face,
                                FT_UInt   अंक_coords,
                                FT_Long*  coords )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    जर ( अंक_coords && !coords )
      परत FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service );
    जर ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      जर ( service->set_mm_design )
        error = service->set_mm_design( face, अंक_coords, coords );

      जर ( !error )
      {
        जर ( अंक_coords )
          face->face_flags |= FT_FACE_FLAG_VARIATION;
        अन्यथा
          face->face_flags &= ~FT_FACE_FLAG_VARIATION;
      }
    }

    /* enforce recomputation of auto-hinting data */
    जर ( !error && face->autohint.finalizer )
    {
      face->autohint.finalizer( face->autohint.data );
      face->autohint.data = रिक्त;
    }

    परत error;
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

    जर ( len && !weightvector )
      परत FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service );
    जर ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      जर ( service->set_mm_weightvector )
        error = service->set_mm_weightvector( face, len, weightvector );

      जर ( !error )
      {
        जर ( len )
          face->face_flags |= FT_FACE_FLAG_VARIATION;
        अन्यथा
          face->face_flags &= ~FT_FACE_FLAG_VARIATION;
      }
    }

    /* enforce recomputation of auto-hinting data */
    जर ( !error && face->autohint.finalizer )
    {
      face->autohint.finalizer( face->autohint.data );
      face->autohint.data = रिक्त;
    }

    परत error;
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Get_MM_WeightVector( FT_Face    face,
                          FT_UInt*   len,
                          FT_Fixed*  weightvector )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    जर ( len && !weightvector )
      परत FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service );
    जर ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      जर ( service->get_mm_weightvector )
        error = service->get_mm_weightvector( face, len, weightvector );
    }

    परत error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Set_Var_Design_Coordinates( FT_Face    face,
                                 FT_UInt    अंक_coords,
                                 FT_Fixed*  coords )
  {
    FT_Error                      error;
    FT_Service_MultiMasters       service_mm   = रिक्त;
    FT_Service_MetricsVariations  service_mvar = रिक्त;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    जर ( अंक_coords && !coords )
      परत FT_THROW( Invalid_Argument );

    जर ( !अंक_coords && !FT_IS_VARIATION( face ) )
      परत FT_Err_Ok;  /* nothing to be done */

    error = ft_face_get_mm_service( face, &service_mm );
    जर ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      जर ( service_mm->set_var_design )
        error = service_mm->set_var_design( face, अंक_coords, coords );

      जर ( !error || error == -1 || error == -2 )
      {
        FT_Bool  is_variation_old = FT_IS_VARIATION( face );


        जर ( error != -1 )
        {
          जर ( error == -2 ) /* -2 means is_variable. */
          {
            face->face_flags |= FT_FACE_FLAG_VARIATION;
            error             = FT_Err_Ok;
          }
          अन्यथा
            face->face_flags &= ~FT_FACE_FLAG_VARIATION;
        }

        जर ( service_mm->construct_ps_name )
        {
          जर ( error == -1 )
          {
            /* The PS name of a named instance and a non-named instance */
            /* usually differs, even if the axis values are identical.  */
            जर ( is_variation_old != FT_IS_VARIATION( face ) )
              service_mm->construct_ps_name( face );
          }
          अन्यथा
            service_mm->construct_ps_name( face );
        }
      }

      /* internal error code -1 means `no change'; we can exit immediately */
      जर ( error == -1 )
        परत FT_Err_Ok;
    }

    जर ( !error )
    {
      (शून्य)ft_face_get_mvar_service( face, &service_mvar );

      जर ( service_mvar && service_mvar->metrics_adjust )
        service_mvar->metrics_adjust( face );
    }

    /* enforce recomputation of auto-hinting data */
    जर ( !error && face->autohint.finalizer )
    {
      face->autohint.finalizer( face->autohint.data );
      face->autohint.data = रिक्त;
    }

    परत error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Var_Design_Coordinates( FT_Face    face,
                                 FT_UInt    अंक_coords,
                                 FT_Fixed*  coords )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    जर ( !coords )
      परत FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service );
    जर ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      जर ( service->get_var_design )
        error = service->get_var_design( face, अंक_coords, coords );
    }

    परत error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Set_MM_Blend_Coordinates( FT_Face    face,
                               FT_UInt    अंक_coords,
                               FT_Fixed*  coords )
  {
    FT_Error                      error;
    FT_Service_MultiMasters       service_mm   = रिक्त;
    FT_Service_MetricsVariations  service_mvar = रिक्त;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    जर ( अंक_coords && !coords )
      परत FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service_mm );
    जर ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      जर ( service_mm->set_mm_blend )
        error = service_mm->set_mm_blend( face, अंक_coords, coords );

      जर ( !error || error == -1 )
      {
        FT_Bool  is_variation_old = FT_IS_VARIATION( face );


        जर ( अंक_coords )
          face->face_flags |= FT_FACE_FLAG_VARIATION;
        अन्यथा
          face->face_flags &= ~FT_FACE_FLAG_VARIATION;

        जर ( service_mm->construct_ps_name )
        {
          जर ( error == -1 )
          {
            /* The PS name of a named instance and a non-named instance */
            /* usually differs, even if the axis values are identical.  */
            जर ( is_variation_old != FT_IS_VARIATION( face ) )
              service_mm->construct_ps_name( face );
          }
          अन्यथा
            service_mm->construct_ps_name( face );
        }
      }

      /* internal error code -1 means `no change'; we can exit immediately */
      जर ( error == -1 )
        परत FT_Err_Ok;
    }

    जर ( !error )
    {
      (शून्य)ft_face_get_mvar_service( face, &service_mvar );

      जर ( service_mvar && service_mvar->metrics_adjust )
        service_mvar->metrics_adjust( face );
    }

    /* enforce recomputation of auto-hinting data */
    जर ( !error && face->autohint.finalizer )
    {
      face->autohint.finalizer( face->autohint.data );
      face->autohint.data = रिक्त;
    }

    परत error;
  }


  /* documentation is in ftmm.h */

  /* This is exactly the same as the previous function.  It exists for */
  /* orthogonality.                                                    */

  FT_EXPORT_DEF( FT_Error )
  FT_Set_Var_Blend_Coordinates( FT_Face    face,
                                FT_UInt    अंक_coords,
                                FT_Fixed*  coords )
  {
    FT_Error                      error;
    FT_Service_MultiMasters       service_mm   = रिक्त;
    FT_Service_MetricsVariations  service_mvar = रिक्त;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    जर ( अंक_coords && !coords )
      परत FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service_mm );
    जर ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      जर ( service_mm->set_mm_blend )
        error = service_mm->set_mm_blend( face, अंक_coords, coords );

      जर ( !error || error == -1 || error == -2 )
      {
        FT_Bool  is_variation_old = FT_IS_VARIATION( face );


        जर ( error != -1 )
        {
          जर ( error == -2 ) /* -2 means is_variable. */
          {
            face->face_flags |= FT_FACE_FLAG_VARIATION;
            error             = FT_Err_Ok;
          }
          अन्यथा
            face->face_flags &= ~FT_FACE_FLAG_VARIATION;
        }

        जर ( service_mm->construct_ps_name )
        {
          जर ( error == -1 )
          {
            /* The PS name of a named instance and a non-named instance */
            /* usually differs, even if the axis values are identical.  */
            जर ( is_variation_old != FT_IS_VARIATION( face ) )
              service_mm->construct_ps_name( face );
          }
          अन्यथा
            service_mm->construct_ps_name( face );
        }
      }

      /* internal error code -1 means `no change'; we can exit immediately */
      जर ( error == -1 )
        परत FT_Err_Ok;
    }

    जर ( !error )
    {
      (शून्य)ft_face_get_mvar_service( face, &service_mvar );

      जर ( service_mvar && service_mvar->metrics_adjust )
        service_mvar->metrics_adjust( face );
    }

    /* enforce recomputation of auto-hinting data */
    जर ( !error && face->autohint.finalizer )
    {
      face->autohint.finalizer( face->autohint.data );
      face->autohint.data = रिक्त;
    }

    परत error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_MM_Blend_Coordinates( FT_Face    face,
                               FT_UInt    अंक_coords,
                               FT_Fixed*  coords )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    जर ( !coords )
      परत FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service );
    जर ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      जर ( service->get_mm_blend )
        error = service->get_mm_blend( face, अंक_coords, coords );
    }

    परत error;
  }


  /* documentation is in ftmm.h */

  /* This is exactly the same as the previous function.  It exists for */
  /* orthogonality.                                                    */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Var_Blend_Coordinates( FT_Face    face,
                                FT_UInt    अंक_coords,
                                FT_Fixed*  coords )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    जर ( !coords )
      परत FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service );
    जर ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      जर ( service->get_mm_blend )
        error = service->get_mm_blend( face, अंक_coords, coords );
    }

    परत error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Var_Axis_Flags( FT_MM_Var*  master,
                         FT_UInt     axis_index,
                         FT_UInt*    flags )
  {
    FT_UShort*  axis_flags;


    जर ( !master || !flags )
      परत FT_THROW( Invalid_Argument );

    जर ( axis_index >= master->अंक_axis )
      परत FT_THROW( Invalid_Argument );

    /* the axis flags array immediately follows the data of `master' */
    axis_flags = (FT_UShort*)&( master[1] );
    *flags     = axis_flags[axis_index];

    परत FT_Err_Ok;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Set_Named_Instance( FT_Face  face,
                         FT_UInt  instance_index )
  {
    FT_Error  error;

    FT_Service_MultiMasters       service_mm   = रिक्त;
    FT_Service_MetricsVariations  service_mvar = रिक्त;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    error = ft_face_get_mm_service( face, &service_mm );
    जर ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      जर ( service_mm->set_named_instance )
        error = service_mm->set_named_instance( face, instance_index );

      जर ( !error || error == -1 )
      {
        FT_Bool  is_variation_old = FT_IS_VARIATION( face );


        face->face_flags &= ~FT_FACE_FLAG_VARIATION;
        face->face_index  = ( instance_index << 16 )        |
                            ( face->face_index & 0xFFFFL );

        जर ( service_mm->construct_ps_name )
        {
          जर ( error == -1 )
          {
            /* The PS name of a named instance and a non-named instance */
            /* usually differs, even if the axis values are identical.  */
            जर ( is_variation_old != FT_IS_VARIATION( face ) )
              service_mm->construct_ps_name( face );
          }
          अन्यथा
            service_mm->construct_ps_name( face );
        }
      }

      /* internal error code -1 means `no change'; we can exit immediately */
      जर ( error == -1 )
        परत FT_Err_Ok;
    }

    जर ( !error )
    {
      (शून्य)ft_face_get_mvar_service( face, &service_mvar );

      जर ( service_mvar && service_mvar->metrics_adjust )
        service_mvar->metrics_adjust( face );
    }

    /* enforce recomputation of auto-hinting data */
    जर ( !error && face->autohint.finalizer )
    {
      face->autohint.finalizer( face->autohint.data );
      face->autohint.data = रिक्त;
    }

    परत error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Default_Named_Instance( FT_Face   face,
                                 FT_UInt  *instance_index )
  {
    FT_Error  error;

    FT_Service_MultiMasters  service_mm = रिक्त;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    error = ft_face_get_mm_service( face, &service_mm );
    जर ( !error )
    {
      /* no error if `get_default_named_instance` is not available */
      जर ( service_mm->get_default_named_instance )
        error = service_mm->get_default_named_instance( face,
                                                        instance_index );
      अन्यथा
        error = FT_Err_Ok;
    }

    परत error;
  }


/* END */
