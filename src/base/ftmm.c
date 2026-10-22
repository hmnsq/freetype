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


#அந்தர்பாவ <freetype/internal/ftdebug.h>

#அந்தர்பாவ <freetype/ftmm.h>
#அந்தர்பாவ <freetype/internal/ftobjs.h>
#அந்தர்பாவ <freetype/internal/services/svmm.h>
#அந்தர்பாவ <freetype/internal/services/svmetric.h>


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#undef  FT_COMPONENT
#define FT_COMPONENT  mm


  நிலையான FT_Error
  ft_face_get_mm_service( FT_Face                   face,
                          FT_Service_MultiMasters  *aservice )
  {
    FT_Error  error;


    *aservice = வெற்று;

    எனில் ( !face )
      திரும்பு FT_THROW( Invalid_Face_Handle );

    error = FT_ERR( Invalid_Argument );

    எனில் ( FT_HAS_MULTIPLE_MASTERS( face ) )
    {
      FT_FACE_LOOKUP_SERVICE( face,
                              *aservice,
                              MULTI_MASTERS );

      எனில் ( *aservice )
        error = FT_Err_Ok;
    }

    திரும்பு error;
  }


  நிலையான FT_Error
  ft_face_get_mvar_service( FT_Face                        face,
                            FT_Service_MetricsVariations  *aservice )
  {
    FT_Error  error;


    *aservice = வெற்று;

    எனில் ( !face )
      திரும்பு FT_THROW( Invalid_Face_Handle );

    error = FT_ERR( Invalid_Argument );

    எனில் ( FT_HAS_MULTIPLE_MASTERS( face ) )
    {
      FT_FACE_LOOKUP_SERVICE( face,
                              *aservice,
                              METRICS_VARIATIONS );

      எனில் ( *aservice )
        error = FT_Err_Ok;
    }

    திரும்பு error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Multi_Master( FT_Face           face,
                       FT_Multi_Master  *amaster )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    எனில் ( !amaster )
      திரும்பு FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service );
    எனில் ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      எனில் ( service->get_mm )
        error = service->get_mm( face, amaster );
    }

    திரும்பு error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_MM_Var( FT_Face      face,
                 FT_MM_Var*  *amaster )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    எனில் ( !amaster )
      திரும்பு FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service );
    எனில் ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      எனில் ( service->get_mm_var )
        error = service->get_mm_var( face, amaster );
    }

    திரும்பு error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Done_MM_Var( FT_Library  library,
                  FT_MM_Var*  amaster )
  {
    FT_Memory  memory;


    எனில் ( !library )
      திரும்பு FT_THROW( Invalid_Library_Handle );

    memory = library->memory;
    FT_FREE( amaster );

    திரும்பு FT_Err_Ok;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Set_MM_Design_Coordinates( FT_Face   face,
                                FT_UInt   இலக்கம்_coords,
                                FT_Long*  coords )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    எனில் ( இலக்கம்_coords && !coords )
      திரும்பு FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service );
    எனில் ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      எனில் ( service->set_mm_design )
        error = service->set_mm_design( face, இலக்கம்_coords, coords );

      எனில் ( !error )
      {
        எனில் ( இலக்கம்_coords )
          face->face_flags |= FT_FACE_FLAG_VARIATION;
        இல்லையெனில்
          face->face_flags &= ~FT_FACE_FLAG_VARIATION;
      }
    }

    /* enforce recomputation of auto-hinting data */
    எனில் ( !error && face->autohint.finalizer )
    {
      face->autohint.finalizer( face->autohint.data );
      face->autohint.data = வெற்று;
    }

    திரும்பு error;
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

    எனில் ( len && !weightvector )
      திரும்பு FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service );
    எனில் ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      எனில் ( service->set_mm_weightvector )
        error = service->set_mm_weightvector( face, len, weightvector );

      எனில் ( !error )
      {
        எனில் ( len )
          face->face_flags |= FT_FACE_FLAG_VARIATION;
        இல்லையெனில்
          face->face_flags &= ~FT_FACE_FLAG_VARIATION;
      }
    }

    /* enforce recomputation of auto-hinting data */
    எனில் ( !error && face->autohint.finalizer )
    {
      face->autohint.finalizer( face->autohint.data );
      face->autohint.data = வெற்று;
    }

    திரும்பு error;
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Get_MM_WeightVector( FT_Face    face,
                          FT_UInt*   len,
                          FT_Fixed*  weightvector )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    எனில் ( len && !weightvector )
      திரும்பு FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service );
    எனில் ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      எனில் ( service->get_mm_weightvector )
        error = service->get_mm_weightvector( face, len, weightvector );
    }

    திரும்பு error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Set_Var_Design_Coordinates( FT_Face    face,
                                 FT_UInt    இலக்கம்_coords,
                                 FT_Fixed*  coords )
  {
    FT_Error                      error;
    FT_Service_MultiMasters       service_mm   = வெற்று;
    FT_Service_MetricsVariations  service_mvar = வெற்று;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    எனில் ( இலக்கம்_coords && !coords )
      திரும்பு FT_THROW( Invalid_Argument );

    எனில் ( !இலக்கம்_coords && !FT_IS_VARIATION( face ) )
      திரும்பு FT_Err_Ok;  /* nothing to be done */

    error = ft_face_get_mm_service( face, &service_mm );
    எனில் ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      எனில் ( service_mm->set_var_design )
        error = service_mm->set_var_design( face, இலக்கம்_coords, coords );

      எனில் ( !error || error == -1 || error == -2 )
      {
        FT_Bool  is_variation_old = FT_IS_VARIATION( face );


        எனில் ( error != -1 )
        {
          எனில் ( error == -2 ) /* -2 means is_variable. */
          {
            face->face_flags |= FT_FACE_FLAG_VARIATION;
            error             = FT_Err_Ok;
          }
          இல்லையெனில்
            face->face_flags &= ~FT_FACE_FLAG_VARIATION;
        }

        எனில் ( service_mm->construct_ps_name )
        {
          எனில் ( error == -1 )
          {
            /* The PS name of a named instance and a non-named instance */
            /* usually differs, even if the axis values are identical.  */
            எனில் ( is_variation_old != FT_IS_VARIATION( face ) )
              service_mm->construct_ps_name( face );
          }
          இல்லையெனில்
            service_mm->construct_ps_name( face );
        }
      }

      /* internal error code -1 means `no change'; we can exit immediately */
      எனில் ( error == -1 )
        திரும்பு FT_Err_Ok;
    }

    எனில் ( !error )
    {
      (வெறுமை)ft_face_get_mvar_service( face, &service_mvar );

      எனில் ( service_mvar && service_mvar->metrics_adjust )
        service_mvar->metrics_adjust( face );
    }

    /* enforce recomputation of auto-hinting data */
    எனில் ( !error && face->autohint.finalizer )
    {
      face->autohint.finalizer( face->autohint.data );
      face->autohint.data = வெற்று;
    }

    திரும்பு error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Var_Design_Coordinates( FT_Face    face,
                                 FT_UInt    இலக்கம்_coords,
                                 FT_Fixed*  coords )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    எனில் ( !coords )
      திரும்பு FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service );
    எனில் ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      எனில் ( service->get_var_design )
        error = service->get_var_design( face, இலக்கம்_coords, coords );
    }

    திரும்பு error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Set_MM_Blend_Coordinates( FT_Face    face,
                               FT_UInt    இலக்கம்_coords,
                               FT_Fixed*  coords )
  {
    FT_Error                      error;
    FT_Service_MultiMasters       service_mm   = வெற்று;
    FT_Service_MetricsVariations  service_mvar = வெற்று;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    எனில் ( இலக்கம்_coords && !coords )
      திரும்பு FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service_mm );
    எனில் ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      எனில் ( service_mm->set_mm_blend )
        error = service_mm->set_mm_blend( face, இலக்கம்_coords, coords );

      எனில் ( !error || error == -1 )
      {
        FT_Bool  is_variation_old = FT_IS_VARIATION( face );


        எனில் ( இலக்கம்_coords )
          face->face_flags |= FT_FACE_FLAG_VARIATION;
        இல்லையெனில்
          face->face_flags &= ~FT_FACE_FLAG_VARIATION;

        எனில் ( service_mm->construct_ps_name )
        {
          எனில் ( error == -1 )
          {
            /* The PS name of a named instance and a non-named instance */
            /* usually differs, even if the axis values are identical.  */
            எனில் ( is_variation_old != FT_IS_VARIATION( face ) )
              service_mm->construct_ps_name( face );
          }
          இல்லையெனில்
            service_mm->construct_ps_name( face );
        }
      }

      /* internal error code -1 means `no change'; we can exit immediately */
      எனில் ( error == -1 )
        திரும்பு FT_Err_Ok;
    }

    எனில் ( !error )
    {
      (வெறுமை)ft_face_get_mvar_service( face, &service_mvar );

      எனில் ( service_mvar && service_mvar->metrics_adjust )
        service_mvar->metrics_adjust( face );
    }

    /* enforce recomputation of auto-hinting data */
    எனில் ( !error && face->autohint.finalizer )
    {
      face->autohint.finalizer( face->autohint.data );
      face->autohint.data = வெற்று;
    }

    திரும்பு error;
  }


  /* documentation is in ftmm.h */

  /* This is exactly the same as the previous function.  It exists for */
  /* orthogonality.                                                    */

  FT_EXPORT_DEF( FT_Error )
  FT_Set_Var_Blend_Coordinates( FT_Face    face,
                                FT_UInt    இலக்கம்_coords,
                                FT_Fixed*  coords )
  {
    FT_Error                      error;
    FT_Service_MultiMasters       service_mm   = வெற்று;
    FT_Service_MetricsVariations  service_mvar = வெற்று;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    எனில் ( இலக்கம்_coords && !coords )
      திரும்பு FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service_mm );
    எனில் ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      எனில் ( service_mm->set_mm_blend )
        error = service_mm->set_mm_blend( face, இலக்கம்_coords, coords );

      எனில் ( !error || error == -1 || error == -2 )
      {
        FT_Bool  is_variation_old = FT_IS_VARIATION( face );


        எனில் ( error != -1 )
        {
          எனில் ( error == -2 ) /* -2 means is_variable. */
          {
            face->face_flags |= FT_FACE_FLAG_VARIATION;
            error             = FT_Err_Ok;
          }
          இல்லையெனில்
            face->face_flags &= ~FT_FACE_FLAG_VARIATION;
        }

        எனில் ( service_mm->construct_ps_name )
        {
          எனில் ( error == -1 )
          {
            /* The PS name of a named instance and a non-named instance */
            /* usually differs, even if the axis values are identical.  */
            எனில் ( is_variation_old != FT_IS_VARIATION( face ) )
              service_mm->construct_ps_name( face );
          }
          இல்லையெனில்
            service_mm->construct_ps_name( face );
        }
      }

      /* internal error code -1 means `no change'; we can exit immediately */
      எனில் ( error == -1 )
        திரும்பு FT_Err_Ok;
    }

    எனில் ( !error )
    {
      (வெறுமை)ft_face_get_mvar_service( face, &service_mvar );

      எனில் ( service_mvar && service_mvar->metrics_adjust )
        service_mvar->metrics_adjust( face );
    }

    /* enforce recomputation of auto-hinting data */
    எனில் ( !error && face->autohint.finalizer )
    {
      face->autohint.finalizer( face->autohint.data );
      face->autohint.data = வெற்று;
    }

    திரும்பு error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_MM_Blend_Coordinates( FT_Face    face,
                               FT_UInt    இலக்கம்_coords,
                               FT_Fixed*  coords )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    எனில் ( !coords )
      திரும்பு FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service );
    எனில் ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      எனில் ( service->get_mm_blend )
        error = service->get_mm_blend( face, இலக்கம்_coords, coords );
    }

    திரும்பு error;
  }


  /* documentation is in ftmm.h */

  /* This is exactly the same as the previous function.  It exists for */
  /* orthogonality.                                                    */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Var_Blend_Coordinates( FT_Face    face,
                                FT_UInt    இலக்கம்_coords,
                                FT_Fixed*  coords )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    எனில் ( !coords )
      திரும்பு FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service );
    எனில் ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      எனில் ( service->get_mm_blend )
        error = service->get_mm_blend( face, இலக்கம்_coords, coords );
    }

    திரும்பு error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Var_Axis_Flags( FT_MM_Var*  master,
                         FT_UInt     axis_index,
                         FT_UInt*    flags )
  {
    FT_UShort*  axis_flags;


    எனில் ( !master || !flags )
      திரும்பு FT_THROW( Invalid_Argument );

    எனில் ( axis_index >= master->இலக்கம்_axis )
      திரும்பு FT_THROW( Invalid_Argument );

    /* the axis flags array immediately follows the data of `master' */
    axis_flags = (FT_UShort*)&( master[1] );
    *flags     = axis_flags[axis_index];

    திரும்பு FT_Err_Ok;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Set_Named_Instance( FT_Face  face,
                         FT_UInt  instance_index )
  {
    FT_Error  error;

    FT_Service_MultiMasters       service_mm   = வெற்று;
    FT_Service_MetricsVariations  service_mvar = வெற்று;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    error = ft_face_get_mm_service( face, &service_mm );
    எனில் ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      எனில் ( service_mm->set_named_instance )
        error = service_mm->set_named_instance( face, instance_index );

      எனில் ( !error || error == -1 )
      {
        FT_Bool  is_variation_old = FT_IS_VARIATION( face );


        face->face_flags &= ~FT_FACE_FLAG_VARIATION;
        face->face_index  = ( instance_index << 16 )        |
                            ( face->face_index & 0xFFFFL );

        எனில் ( service_mm->construct_ps_name )
        {
          எனில் ( error == -1 )
          {
            /* The PS name of a named instance and a non-named instance */
            /* usually differs, even if the axis values are identical.  */
            எனில் ( is_variation_old != FT_IS_VARIATION( face ) )
              service_mm->construct_ps_name( face );
          }
          இல்லையெனில்
            service_mm->construct_ps_name( face );
        }
      }

      /* internal error code -1 means `no change'; we can exit immediately */
      எனில் ( error == -1 )
        திரும்பு FT_Err_Ok;
    }

    எனில் ( !error )
    {
      (வெறுமை)ft_face_get_mvar_service( face, &service_mvar );

      எனில் ( service_mvar && service_mvar->metrics_adjust )
        service_mvar->metrics_adjust( face );
    }

    /* enforce recomputation of auto-hinting data */
    எனில் ( !error && face->autohint.finalizer )
    {
      face->autohint.finalizer( face->autohint.data );
      face->autohint.data = வெற்று;
    }

    திரும்பு error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Default_Named_Instance( FT_Face   face,
                                 FT_UInt  *instance_index )
  {
    FT_Error  error;

    FT_Service_MultiMasters  service_mm = வெற்று;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    error = ft_face_get_mm_service( face, &service_mm );
    எனில் ( !error )
    {
      /* no error if `get_default_named_instance` is not available */
      எனில் ( service_mm->get_default_named_instance )
        error = service_mm->get_default_named_instance( face,
                                                        instance_index );
      இல்லையெனில்
        error = FT_Err_Ok;
    }

    திரும்பு error;
  }


/* END */
