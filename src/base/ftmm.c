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


#समावेश <freetype/internal/ftdebug.h>

#समावेश <freetype/ftmm.h>
#समावेश <freetype/internal/ftobjs.h>
#समावेश <freetype/internal/services/svmm.h>
#समावेश <freetype/internal/services/svmetric.h>


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#अघोषित  FT_COMPONENT
#घोषणा FT_COMPONENT  mm


  अटल FT_Error
  ft_face_get_mm_service( FT_Face                   face,
                          FT_Service_MultiMasters  *aservice )
  {
    FT_Error  error;


    *aservice = शून्य;

    अगर ( !face )
      वापस FT_THROW( Invalid_Face_Handle );

    error = FT_ERR( Invalid_Argument );

    अगर ( FT_HAS_MULTIPLE_MASTERS( face ) )
    {
      FT_FACE_LOOKUP_SERVICE( face,
                              *aservice,
                              MULTI_MASTERS );

      अगर ( *aservice )
        error = FT_Err_Ok;
    }

    वापस error;
  }


  अटल FT_Error
  ft_face_get_mvar_service( FT_Face                        face,
                            FT_Service_MetricsVariations  *aservice )
  {
    FT_Error  error;


    *aservice = शून्य;

    अगर ( !face )
      वापस FT_THROW( Invalid_Face_Handle );

    error = FT_ERR( Invalid_Argument );

    अगर ( FT_HAS_MULTIPLE_MASTERS( face ) )
    {
      FT_FACE_LOOKUP_SERVICE( face,
                              *aservice,
                              METRICS_VARIATIONS );

      अगर ( *aservice )
        error = FT_Err_Ok;
    }

    वापस error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Multi_Master( FT_Face           face,
                       FT_Multi_Master  *amaster )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    अगर ( !amaster )
      वापस FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service );
    अगर ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      अगर ( service->get_mm )
        error = service->get_mm( face, amaster );
    }

    वापस error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_MM_Var( FT_Face      face,
                 FT_MM_Var*  *amaster )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    अगर ( !amaster )
      वापस FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service );
    अगर ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      अगर ( service->get_mm_var )
        error = service->get_mm_var( face, amaster );
    }

    वापस error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Done_MM_Var( FT_Library  library,
                  FT_MM_Var*  amaster )
  {
    FT_Memory  memory;


    अगर ( !library )
      वापस FT_THROW( Invalid_Library_Handle );

    memory = library->memory;
    FT_FREE( amaster );

    वापस FT_Err_Ok;
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

    अगर ( अंक_coords && !coords )
      वापस FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service );
    अगर ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      अगर ( service->set_mm_design )
        error = service->set_mm_design( face, अंक_coords, coords );

      अगर ( !error )
      {
        अगर ( अंक_coords )
          face->face_flags |= FT_FACE_FLAG_VARIATION;
        अन्यथा
          face->face_flags &= ~FT_FACE_FLAG_VARIATION;
      }
    }

    /* enforce recomputation of auto-hinting data */
    अगर ( !error && face->autohint.finalizer )
    {
      face->autohint.finalizer( face->autohint.तथ्य );
      face->autohint.तथ्य = शून्य;
    }

    वापस error;
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

    अगर ( len && !weightvector )
      वापस FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service );
    अगर ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      अगर ( service->set_mm_weightvector )
        error = service->set_mm_weightvector( face, len, weightvector );

      अगर ( !error )
      {
        अगर ( len )
          face->face_flags |= FT_FACE_FLAG_VARIATION;
        अन्यथा
          face->face_flags &= ~FT_FACE_FLAG_VARIATION;
      }
    }

    /* enforce recomputation of auto-hinting data */
    अगर ( !error && face->autohint.finalizer )
    {
      face->autohint.finalizer( face->autohint.तथ्य );
      face->autohint.तथ्य = शून्य;
    }

    वापस error;
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Get_MM_WeightVector( FT_Face    face,
                          FT_UInt*   len,
                          FT_Fixed*  weightvector )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    अगर ( len && !weightvector )
      वापस FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service );
    अगर ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      अगर ( service->get_mm_weightvector )
        error = service->get_mm_weightvector( face, len, weightvector );
    }

    वापस error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Set_Var_Design_Coordinates( FT_Face    face,
                                 FT_UInt    अंक_coords,
                                 FT_Fixed*  coords )
  {
    FT_Error                      error;
    FT_Service_MultiMasters       service_mm   = शून्य;
    FT_Service_MetricsVariations  service_mvar = शून्य;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    अगर ( अंक_coords && !coords )
      वापस FT_THROW( Invalid_Argument );

    अगर ( !अंक_coords && !FT_IS_VARIATION( face ) )
      वापस FT_Err_Ok;  /* nothing to be done */

    error = ft_face_get_mm_service( face, &service_mm );
    अगर ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      अगर ( service_mm->set_var_design )
        error = service_mm->set_var_design( face, अंक_coords, coords );

      अगर ( !error || error == -1 || error == -2 )
      {
        FT_Bool  is_variation_old = FT_IS_VARIATION( face );


        अगर ( error != -1 )
        {
          अगर ( error == -2 ) /* -2 means is_variable. */
          {
            face->face_flags |= FT_FACE_FLAG_VARIATION;
            error             = FT_Err_Ok;
          }
          अन्यथा
            face->face_flags &= ~FT_FACE_FLAG_VARIATION;
        }

        अगर ( service_mm->construct_ps_name )
        {
          अगर ( error == -1 )
          {
            /* The PS name of a named instance and a non-named instance */
            /* usually differs, even if the axis values are identical.  */
            अगर ( is_variation_old != FT_IS_VARIATION( face ) )
              service_mm->construct_ps_name( face );
          }
          अन्यथा
            service_mm->construct_ps_name( face );
        }
      }

      /* internal error code -1 means `no change'; we can exit immediately */
      अगर ( error == -1 )
        वापस FT_Err_Ok;
    }

    अगर ( !error )
    {
      (व्योम)ft_face_get_mvar_service( face, &service_mvar );

      अगर ( service_mvar && service_mvar->metrics_adjust )
        service_mvar->metrics_adjust( face );
    }

    /* enforce recomputation of auto-hinting data */
    अगर ( !error && face->autohint.finalizer )
    {
      face->autohint.finalizer( face->autohint.तथ्य );
      face->autohint.तथ्य = शून्य;
    }

    वापस error;
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

    अगर ( !coords )
      वापस FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service );
    अगर ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      अगर ( service->get_var_design )
        error = service->get_var_design( face, अंक_coords, coords );
    }

    वापस error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Set_MM_Blend_Coordinates( FT_Face    face,
                               FT_UInt    अंक_coords,
                               FT_Fixed*  coords )
  {
    FT_Error                      error;
    FT_Service_MultiMasters       service_mm   = शून्य;
    FT_Service_MetricsVariations  service_mvar = शून्य;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    अगर ( अंक_coords && !coords )
      वापस FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service_mm );
    अगर ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      अगर ( service_mm->set_mm_blend )
        error = service_mm->set_mm_blend( face, अंक_coords, coords );

      अगर ( !error || error == -1 )
      {
        FT_Bool  is_variation_old = FT_IS_VARIATION( face );


        अगर ( अंक_coords )
          face->face_flags |= FT_FACE_FLAG_VARIATION;
        अन्यथा
          face->face_flags &= ~FT_FACE_FLAG_VARIATION;

        अगर ( service_mm->construct_ps_name )
        {
          अगर ( error == -1 )
          {
            /* The PS name of a named instance and a non-named instance */
            /* usually differs, even if the axis values are identical.  */
            अगर ( is_variation_old != FT_IS_VARIATION( face ) )
              service_mm->construct_ps_name( face );
          }
          अन्यथा
            service_mm->construct_ps_name( face );
        }
      }

      /* internal error code -1 means `no change'; we can exit immediately */
      अगर ( error == -1 )
        वापस FT_Err_Ok;
    }

    अगर ( !error )
    {
      (व्योम)ft_face_get_mvar_service( face, &service_mvar );

      अगर ( service_mvar && service_mvar->metrics_adjust )
        service_mvar->metrics_adjust( face );
    }

    /* enforce recomputation of auto-hinting data */
    अगर ( !error && face->autohint.finalizer )
    {
      face->autohint.finalizer( face->autohint.तथ्य );
      face->autohint.तथ्य = शून्य;
    }

    वापस error;
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
    FT_Service_MultiMasters       service_mm   = शून्य;
    FT_Service_MetricsVariations  service_mvar = शून्य;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    अगर ( अंक_coords && !coords )
      वापस FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service_mm );
    अगर ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      अगर ( service_mm->set_mm_blend )
        error = service_mm->set_mm_blend( face, अंक_coords, coords );

      अगर ( !error || error == -1 || error == -2 )
      {
        FT_Bool  is_variation_old = FT_IS_VARIATION( face );


        अगर ( error != -1 )
        {
          अगर ( error == -2 ) /* -2 means is_variable. */
          {
            face->face_flags |= FT_FACE_FLAG_VARIATION;
            error             = FT_Err_Ok;
          }
          अन्यथा
            face->face_flags &= ~FT_FACE_FLAG_VARIATION;
        }

        अगर ( service_mm->construct_ps_name )
        {
          अगर ( error == -1 )
          {
            /* The PS name of a named instance and a non-named instance */
            /* usually differs, even if the axis values are identical.  */
            अगर ( is_variation_old != FT_IS_VARIATION( face ) )
              service_mm->construct_ps_name( face );
          }
          अन्यथा
            service_mm->construct_ps_name( face );
        }
      }

      /* internal error code -1 means `no change'; we can exit immediately */
      अगर ( error == -1 )
        वापस FT_Err_Ok;
    }

    अगर ( !error )
    {
      (व्योम)ft_face_get_mvar_service( face, &service_mvar );

      अगर ( service_mvar && service_mvar->metrics_adjust )
        service_mvar->metrics_adjust( face );
    }

    /* enforce recomputation of auto-hinting data */
    अगर ( !error && face->autohint.finalizer )
    {
      face->autohint.finalizer( face->autohint.तथ्य );
      face->autohint.तथ्य = शून्य;
    }

    वापस error;
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

    अगर ( !coords )
      वापस FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service );
    अगर ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      अगर ( service->get_mm_blend )
        error = service->get_mm_blend( face, अंक_coords, coords );
    }

    वापस error;
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

    अगर ( !coords )
      वापस FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service );
    अगर ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      अगर ( service->get_mm_blend )
        error = service->get_mm_blend( face, अंक_coords, coords );
    }

    वापस error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Var_Axis_Flags( FT_MM_Var*  master,
                         FT_UInt     axis_सूचकांक,
                         FT_UInt*    flags )
  {
    FT_UShort*  axis_flags;


    अगर ( !master || !flags )
      वापस FT_THROW( Invalid_Argument );

    अगर ( axis_सूचकांक >= master->अंक_axis )
      वापस FT_THROW( Invalid_Argument );

    /* the axis flags array immediately follows the data of `master' */
    axis_flags = (FT_UShort*)&( master[1] );
    *flags     = axis_flags[axis_सूचकांक];

    वापस FT_Err_Ok;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Set_Named_Instance( FT_Face  face,
                         FT_UInt  instance_सूचकांक )
  {
    FT_Error  error;

    FT_Service_MultiMasters       service_mm   = शून्य;
    FT_Service_MetricsVariations  service_mvar = शून्य;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    error = ft_face_get_mm_service( face, &service_mm );
    अगर ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      अगर ( service_mm->set_named_instance )
        error = service_mm->set_named_instance( face, instance_सूचकांक );

      अगर ( !error || error == -1 )
      {
        FT_Bool  is_variation_old = FT_IS_VARIATION( face );


        face->face_flags &= ~FT_FACE_FLAG_VARIATION;
        face->face_सूचकांक  = ( instance_सूचकांक << 16 )        |
                            ( face->face_सूचकांक & 0xFFFFL );

        अगर ( service_mm->construct_ps_name )
        {
          अगर ( error == -1 )
          {
            /* The PS name of a named instance and a non-named instance */
            /* usually differs, even if the axis values are identical.  */
            अगर ( is_variation_old != FT_IS_VARIATION( face ) )
              service_mm->construct_ps_name( face );
          }
          अन्यथा
            service_mm->construct_ps_name( face );
        }
      }

      /* internal error code -1 means `no change'; we can exit immediately */
      अगर ( error == -1 )
        वापस FT_Err_Ok;
    }

    अगर ( !error )
    {
      (व्योम)ft_face_get_mvar_service( face, &service_mvar );

      अगर ( service_mvar && service_mvar->metrics_adjust )
        service_mvar->metrics_adjust( face );
    }

    /* enforce recomputation of auto-hinting data */
    अगर ( !error && face->autohint.finalizer )
    {
      face->autohint.finalizer( face->autohint.तथ्य );
      face->autohint.तथ्य = शून्य;
    }

    वापस error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Default_Named_Instance( FT_Face   face,
                                 FT_UInt  *instance_सूचकांक )
  {
    FT_Error  error;

    FT_Service_MultiMasters  service_mm = शून्य;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    error = ft_face_get_mm_service( face, &service_mm );
    अगर ( !error )
    {
      /* no error if `get_default_named_instance` is not available */
      अगर ( service_mm->get_default_named_instance )
        error = service_mm->get_default_named_instance( face,
                                                        instance_सूचकांक );
      अन्यथा
        error = FT_Err_Ok;
    }

    वापस error;
  }


/* END */
