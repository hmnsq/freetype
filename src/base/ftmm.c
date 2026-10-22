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


#ଅନ୍ତର୍ଭାଵ <freetype/internal/ftdebug.h>

#ଅନ୍ତର୍ଭାଵ <freetype/ftmm.h>
#ଅନ୍ତର୍ଭାଵ <freetype/internal/ftobjs.h>
#ଅନ୍ତର୍ଭାଵ <freetype/internal/services/svmm.h>
#ଅନ୍ତର୍ଭାଵ <freetype/internal/services/svmetric.h>


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#undef  FT_COMPONENT
#define FT_COMPONENT  mm


  ସ୍ଥିର FT_Error
  ft_face_get_mm_service( FT_Face                   face,
                          FT_Service_MultiMasters  *aservice )
  {
    FT_Error  error;


    *aservice = ଖାଲି;

    ଯଦି ( !face )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Face_Handle );

    error = FT_ERR( Invalid_Argument );

    ଯଦି ( FT_HAS_MULTIPLE_MASTERS( face ) )
    {
      FT_FACE_LOOKUP_SERVICE( face,
                              *aservice,
                              MULTI_MASTERS );

      ଯଦି ( *aservice )
        error = FT_Err_Ok;
    }

    ଫେରନ୍ତୁ error;
  }


  ସ୍ଥିର FT_Error
  ft_face_get_mvar_service( FT_Face                        face,
                            FT_Service_MetricsVariations  *aservice )
  {
    FT_Error  error;


    *aservice = ଖାଲି;

    ଯଦି ( !face )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Face_Handle );

    error = FT_ERR( Invalid_Argument );

    ଯଦି ( FT_HAS_MULTIPLE_MASTERS( face ) )
    {
      FT_FACE_LOOKUP_SERVICE( face,
                              *aservice,
                              METRICS_VARIATIONS );

      ଯଦି ( *aservice )
        error = FT_Err_Ok;
    }

    ଫେରନ୍ତୁ error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Multi_Master( FT_Face           face,
                       FT_Multi_Master  *amaster )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    ଯଦି ( !amaster )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service );
    ଯଦି ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      ଯଦି ( service->get_mm )
        error = service->get_mm( face, amaster );
    }

    ଫେରନ୍ତୁ error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_MM_Var( FT_Face      face,
                 FT_MM_Var*  *amaster )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    ଯଦି ( !amaster )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service );
    ଯଦି ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      ଯଦି ( service->get_mm_var )
        error = service->get_mm_var( face, amaster );
    }

    ଫେରନ୍ତୁ error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Done_MM_Var( FT_Library  library,
                  FT_MM_Var*  amaster )
  {
    FT_Memory  memory;


    ଯଦି ( !library )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Library_Handle );

    memory = library->memory;
    FT_FREE( amaster );

    ଫେରନ୍ତୁ FT_Err_Ok;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Set_MM_Design_Coordinates( FT_Face   face,
                                FT_UInt   ଅଙ୍କ_coords,
                                FT_Long*  coords )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    ଯଦି ( ଅଙ୍କ_coords && !coords )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service );
    ଯଦି ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      ଯଦି ( service->set_mm_design )
        error = service->set_mm_design( face, ଅଙ୍କ_coords, coords );

      ଯଦି ( !error )
      {
        ଯଦି ( ଅଙ୍କ_coords )
          face->face_flags |= FT_FACE_FLAG_VARIATION;
        ଅନ୍ୟଥା
          face->face_flags &= ~FT_FACE_FLAG_VARIATION;
      }
    }

    /* enforce recomputation of auto-hinting data */
    ଯଦି ( !error && face->autohint.finalizer )
    {
      face->autohint.finalizer( face->autohint.data );
      face->autohint.data = ଖାଲି;
    }

    ଫେରନ୍ତୁ error;
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

    ଯଦି ( len && !weightvector )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service );
    ଯଦି ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      ଯଦି ( service->set_mm_weightvector )
        error = service->set_mm_weightvector( face, len, weightvector );

      ଯଦି ( !error )
      {
        ଯଦି ( len )
          face->face_flags |= FT_FACE_FLAG_VARIATION;
        ଅନ୍ୟଥା
          face->face_flags &= ~FT_FACE_FLAG_VARIATION;
      }
    }

    /* enforce recomputation of auto-hinting data */
    ଯଦି ( !error && face->autohint.finalizer )
    {
      face->autohint.finalizer( face->autohint.data );
      face->autohint.data = ଖାଲି;
    }

    ଫେରନ୍ତୁ error;
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Get_MM_WeightVector( FT_Face    face,
                          FT_UInt*   len,
                          FT_Fixed*  weightvector )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    ଯଦି ( len && !weightvector )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service );
    ଯଦି ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      ଯଦି ( service->get_mm_weightvector )
        error = service->get_mm_weightvector( face, len, weightvector );
    }

    ଫେରନ୍ତୁ error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Set_Var_Design_Coordinates( FT_Face    face,
                                 FT_UInt    ଅଙ୍କ_coords,
                                 FT_Fixed*  coords )
  {
    FT_Error                      error;
    FT_Service_MultiMasters       service_mm   = ଖାଲି;
    FT_Service_MetricsVariations  service_mvar = ଖାଲି;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    ଯଦି ( ଅଙ୍କ_coords && !coords )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Argument );

    ଯଦି ( !ଅଙ୍କ_coords && !FT_IS_VARIATION( face ) )
      ଫେରନ୍ତୁ FT_Err_Ok;  /* nothing to be done */

    error = ft_face_get_mm_service( face, &service_mm );
    ଯଦି ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      ଯଦି ( service_mm->set_var_design )
        error = service_mm->set_var_design( face, ଅଙ୍କ_coords, coords );

      ଯଦି ( !error || error == -1 || error == -2 )
      {
        FT_Bool  is_variation_old = FT_IS_VARIATION( face );


        ଯଦି ( error != -1 )
        {
          ଯଦି ( error == -2 ) /* -2 means is_variable. */
          {
            face->face_flags |= FT_FACE_FLAG_VARIATION;
            error             = FT_Err_Ok;
          }
          ଅନ୍ୟଥା
            face->face_flags &= ~FT_FACE_FLAG_VARIATION;
        }

        ଯଦି ( service_mm->construct_ps_name )
        {
          ଯଦି ( error == -1 )
          {
            /* The PS name of a named instance and a non-named instance */
            /* usually differs, even if the axis values are identical.  */
            ଯଦି ( is_variation_old != FT_IS_VARIATION( face ) )
              service_mm->construct_ps_name( face );
          }
          ଅନ୍ୟଥା
            service_mm->construct_ps_name( face );
        }
      }

      /* internal error code -1 means `no change'; we can exit immediately */
      ଯଦି ( error == -1 )
        ଫେରନ୍ତୁ FT_Err_Ok;
    }

    ଯଦି ( !error )
    {
      (ଶୂନ୍ୟ)ft_face_get_mvar_service( face, &service_mvar );

      ଯଦି ( service_mvar && service_mvar->metrics_adjust )
        service_mvar->metrics_adjust( face );
    }

    /* enforce recomputation of auto-hinting data */
    ଯଦି ( !error && face->autohint.finalizer )
    {
      face->autohint.finalizer( face->autohint.data );
      face->autohint.data = ଖାଲି;
    }

    ଫେରନ୍ତୁ error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Var_Design_Coordinates( FT_Face    face,
                                 FT_UInt    ଅଙ୍କ_coords,
                                 FT_Fixed*  coords )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    ଯଦି ( !coords )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service );
    ଯଦି ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      ଯଦି ( service->get_var_design )
        error = service->get_var_design( face, ଅଙ୍କ_coords, coords );
    }

    ଫେରନ୍ତୁ error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Set_MM_Blend_Coordinates( FT_Face    face,
                               FT_UInt    ଅଙ୍କ_coords,
                               FT_Fixed*  coords )
  {
    FT_Error                      error;
    FT_Service_MultiMasters       service_mm   = ଖାଲି;
    FT_Service_MetricsVariations  service_mvar = ଖାଲି;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    ଯଦି ( ଅଙ୍କ_coords && !coords )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service_mm );
    ଯଦି ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      ଯଦି ( service_mm->set_mm_blend )
        error = service_mm->set_mm_blend( face, ଅଙ୍କ_coords, coords );

      ଯଦି ( !error || error == -1 )
      {
        FT_Bool  is_variation_old = FT_IS_VARIATION( face );


        ଯଦି ( ଅଙ୍କ_coords )
          face->face_flags |= FT_FACE_FLAG_VARIATION;
        ଅନ୍ୟଥା
          face->face_flags &= ~FT_FACE_FLAG_VARIATION;

        ଯଦି ( service_mm->construct_ps_name )
        {
          ଯଦି ( error == -1 )
          {
            /* The PS name of a named instance and a non-named instance */
            /* usually differs, even if the axis values are identical.  */
            ଯଦି ( is_variation_old != FT_IS_VARIATION( face ) )
              service_mm->construct_ps_name( face );
          }
          ଅନ୍ୟଥା
            service_mm->construct_ps_name( face );
        }
      }

      /* internal error code -1 means `no change'; we can exit immediately */
      ଯଦି ( error == -1 )
        ଫେରନ୍ତୁ FT_Err_Ok;
    }

    ଯଦି ( !error )
    {
      (ଶୂନ୍ୟ)ft_face_get_mvar_service( face, &service_mvar );

      ଯଦି ( service_mvar && service_mvar->metrics_adjust )
        service_mvar->metrics_adjust( face );
    }

    /* enforce recomputation of auto-hinting data */
    ଯଦି ( !error && face->autohint.finalizer )
    {
      face->autohint.finalizer( face->autohint.data );
      face->autohint.data = ଖାଲି;
    }

    ଫେରନ୍ତୁ error;
  }


  /* documentation is in ftmm.h */

  /* This is exactly the same as the previous function.  It exists for */
  /* orthogonality.                                                    */

  FT_EXPORT_DEF( FT_Error )
  FT_Set_Var_Blend_Coordinates( FT_Face    face,
                                FT_UInt    ଅଙ୍କ_coords,
                                FT_Fixed*  coords )
  {
    FT_Error                      error;
    FT_Service_MultiMasters       service_mm   = ଖାଲି;
    FT_Service_MetricsVariations  service_mvar = ଖାଲି;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    ଯଦି ( ଅଙ୍କ_coords && !coords )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service_mm );
    ଯଦି ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      ଯଦି ( service_mm->set_mm_blend )
        error = service_mm->set_mm_blend( face, ଅଙ୍କ_coords, coords );

      ଯଦି ( !error || error == -1 || error == -2 )
      {
        FT_Bool  is_variation_old = FT_IS_VARIATION( face );


        ଯଦି ( error != -1 )
        {
          ଯଦି ( error == -2 ) /* -2 means is_variable. */
          {
            face->face_flags |= FT_FACE_FLAG_VARIATION;
            error             = FT_Err_Ok;
          }
          ଅନ୍ୟଥା
            face->face_flags &= ~FT_FACE_FLAG_VARIATION;
        }

        ଯଦି ( service_mm->construct_ps_name )
        {
          ଯଦି ( error == -1 )
          {
            /* The PS name of a named instance and a non-named instance */
            /* usually differs, even if the axis values are identical.  */
            ଯଦି ( is_variation_old != FT_IS_VARIATION( face ) )
              service_mm->construct_ps_name( face );
          }
          ଅନ୍ୟଥା
            service_mm->construct_ps_name( face );
        }
      }

      /* internal error code -1 means `no change'; we can exit immediately */
      ଯଦି ( error == -1 )
        ଫେରନ୍ତୁ FT_Err_Ok;
    }

    ଯଦି ( !error )
    {
      (ଶୂନ୍ୟ)ft_face_get_mvar_service( face, &service_mvar );

      ଯଦି ( service_mvar && service_mvar->metrics_adjust )
        service_mvar->metrics_adjust( face );
    }

    /* enforce recomputation of auto-hinting data */
    ଯଦି ( !error && face->autohint.finalizer )
    {
      face->autohint.finalizer( face->autohint.data );
      face->autohint.data = ଖାଲି;
    }

    ଫେରନ୍ତୁ error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_MM_Blend_Coordinates( FT_Face    face,
                               FT_UInt    ଅଙ୍କ_coords,
                               FT_Fixed*  coords )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    ଯଦି ( !coords )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service );
    ଯଦି ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      ଯଦି ( service->get_mm_blend )
        error = service->get_mm_blend( face, ଅଙ୍କ_coords, coords );
    }

    ଫେରନ୍ତୁ error;
  }


  /* documentation is in ftmm.h */

  /* This is exactly the same as the previous function.  It exists for */
  /* orthogonality.                                                    */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Var_Blend_Coordinates( FT_Face    face,
                                FT_UInt    ଅଙ୍କ_coords,
                                FT_Fixed*  coords )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    ଯଦି ( !coords )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service );
    ଯଦି ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      ଯଦି ( service->get_mm_blend )
        error = service->get_mm_blend( face, ଅଙ୍କ_coords, coords );
    }

    ଫେରନ୍ତୁ error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Var_Axis_Flags( FT_MM_Var*  master,
                         FT_UInt     axis_index,
                         FT_UInt*    flags )
  {
    FT_UShort*  axis_flags;


    ଯଦି ( !master || !flags )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Argument );

    ଯଦି ( axis_index >= master->ଅଙ୍କ_axis )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Argument );

    /* the axis flags array immediately follows the data of `master' */
    axis_flags = (FT_UShort*)&( master[1] );
    *flags     = axis_flags[axis_index];

    ଫେରନ୍ତୁ FT_Err_Ok;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Set_Named_Instance( FT_Face  face,
                         FT_UInt  instance_index )
  {
    FT_Error  error;

    FT_Service_MultiMasters       service_mm   = ଖାଲି;
    FT_Service_MetricsVariations  service_mvar = ଖାଲି;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    error = ft_face_get_mm_service( face, &service_mm );
    ଯଦି ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      ଯଦି ( service_mm->set_named_instance )
        error = service_mm->set_named_instance( face, instance_index );

      ଯଦି ( !error || error == -1 )
      {
        FT_Bool  is_variation_old = FT_IS_VARIATION( face );


        face->face_flags &= ~FT_FACE_FLAG_VARIATION;
        face->face_index  = ( instance_index << 16 )        |
                            ( face->face_index & 0xFFFFL );

        ଯଦି ( service_mm->construct_ps_name )
        {
          ଯଦି ( error == -1 )
          {
            /* The PS name of a named instance and a non-named instance */
            /* usually differs, even if the axis values are identical.  */
            ଯଦି ( is_variation_old != FT_IS_VARIATION( face ) )
              service_mm->construct_ps_name( face );
          }
          ଅନ୍ୟଥା
            service_mm->construct_ps_name( face );
        }
      }

      /* internal error code -1 means `no change'; we can exit immediately */
      ଯଦି ( error == -1 )
        ଫେରନ୍ତୁ FT_Err_Ok;
    }

    ଯଦି ( !error )
    {
      (ଶୂନ୍ୟ)ft_face_get_mvar_service( face, &service_mvar );

      ଯଦି ( service_mvar && service_mvar->metrics_adjust )
        service_mvar->metrics_adjust( face );
    }

    /* enforce recomputation of auto-hinting data */
    ଯଦି ( !error && face->autohint.finalizer )
    {
      face->autohint.finalizer( face->autohint.data );
      face->autohint.data = ଖାଲି;
    }

    ଫେରନ୍ତୁ error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Default_Named_Instance( FT_Face   face,
                                 FT_UInt  *instance_index )
  {
    FT_Error  error;

    FT_Service_MultiMasters  service_mm = ଖାଲି;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    error = ft_face_get_mm_service( face, &service_mm );
    ଯଦି ( !error )
    {
      /* no error if `get_default_named_instance` is not available */
      ଯଦି ( service_mm->get_default_named_instance )
        error = service_mm->get_default_named_instance( face,
                                                        instance_index );
      ଅନ୍ୟଥା
        error = FT_Err_Ok;
    }

    ଫେରନ୍ତୁ error;
  }


/* END */
