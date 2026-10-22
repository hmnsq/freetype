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


#అన్తర్భావ <freetype/internal/ftdebug.h>

#అన్తర్భావ <freetype/ftmm.h>
#అన్తర్భావ <freetype/internal/ftobjs.h>
#అన్తర్భావ <freetype/internal/services/svmm.h>
#అన్తర్భావ <freetype/internal/services/svmetric.h>


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#నిర్వచనం_రద్దు  FT_COMPONENT
#నిర్వచించు FT_COMPONENT  mm


  స్థిర FT_Error
  ft_face_get_mm_service( FT_Face                   face,
                          FT_Service_MultiMasters  *aservice )
  {
    FT_Error  error;


    *aservice = ఖాళీ;

    ఒకవేళ ( !face )
      తిరిగి FT_THROW( Invalid_Face_Handle );

    error = FT_ERR( Invalid_వాదం );

    ఒకవేళ ( FT_HAS_గుణిజం_MASTERS( face ) )
    {
      FT_FACE_LOOKUP_SERVICE( face,
                              *aservice,
                              MULTI_MASTERS );

      ఒకవేళ ( *aservice )
        error = FT_Err_Ok;
    }

    తిరిగి error;
  }


  స్థిర FT_Error
  ft_face_get_mvar_service( FT_Face                        face,
                            FT_Service_MetricsVariations  *aservice )
  {
    FT_Error  error;


    *aservice = ఖాళీ;

    ఒకవేళ ( !face )
      తిరిగి FT_THROW( Invalid_Face_Handle );

    error = FT_ERR( Invalid_వాదం );

    ఒకవేళ ( FT_HAS_గుణిజం_MASTERS( face ) )
    {
      FT_FACE_LOOKUP_SERVICE( face,
                              *aservice,
                              METRICS_VARIATIONS );

      ఒకవేళ ( *aservice )
        error = FT_Err_Ok;
    }

    తిరిగి error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Multi_Master( FT_Face           face,
                       FT_Multi_Master  *amaster )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    ఒకవేళ ( !amaster )
      తిరిగి FT_THROW( Invalid_వాదం );

    error = ft_face_get_mm_service( face, &service );
    ఒకవేళ ( !error )
    {
      error = FT_ERR( Invalid_వాదం );
      ఒకవేళ ( service->get_mm )
        error = service->get_mm( face, amaster );
    }

    తిరిగి error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_MM_Var( FT_Face      face,
                 FT_MM_Var*  *amaster )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    ఒకవేళ ( !amaster )
      తిరిగి FT_THROW( Invalid_వాదం );

    error = ft_face_get_mm_service( face, &service );
    ఒకవేళ ( !error )
    {
      error = FT_ERR( Invalid_వాదం );
      ఒకవేళ ( service->get_mm_var )
        error = service->get_mm_var( face, amaster );
    }

    తిరిగి error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_పూర్తి_MM_Var( FT_Library  library,
                  FT_MM_Var*  amaster )
  {
    FT_జ్ఞాపకం  జ్ఞాపకం;


    ఒకవేళ ( !library )
      తిరిగి FT_THROW( Invalid_Library_Handle );

    జ్ఞాపకం = library->జ్ఞాపకం;
    FT_FREE( amaster );

    తిరిగి FT_Err_Ok;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_సమితి_MM_Design_Coordinates( FT_Face   face,
                                FT_UInt   అంకె_coords,
                                FT_Long*  coords )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    ఒకవేళ ( అంకె_coords && !coords )
      తిరిగి FT_THROW( Invalid_వాదం );

    error = ft_face_get_mm_service( face, &service );
    ఒకవేళ ( !error )
    {
      error = FT_ERR( Invalid_వాదం );
      ఒకవేళ ( service->సమితి_mm_design )
        error = service->సమితి_mm_design( face, అంకె_coords, coords );

      ఒకవేళ ( !error )
      {
        ఒకవేళ ( అంకె_coords )
          face->face_flags |= FT_FACE_FLAG_VARIATION;
        లేకపోతే
          face->face_flags &= ~FT_FACE_FLAG_VARIATION;
      }
    }

    /* enforce recomputation of auto-hinting data */
    ఒకవేళ ( !error && face->autohint.finalizer )
    {
      face->autohint.finalizer( face->autohint.దత్తాంశం );
      face->autohint.దత్తాంశం = ఖాళీ;
    }

    తిరిగి error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_సమితి_MM_బరువు_సదిశ( FT_Face    face,
                          FT_UInt    len,
                          FT_Fixed*  weightvector )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    ఒకవేళ ( len && !weightvector )
      తిరిగి FT_THROW( Invalid_వాదం );

    error = ft_face_get_mm_service( face, &service );
    ఒకవేళ ( !error )
    {
      error = FT_ERR( Invalid_వాదం );
      ఒకవేళ ( service->సమితి_mm_weightvector )
        error = service->సమితి_mm_weightvector( face, len, weightvector );

      ఒకవేళ ( !error )
      {
        ఒకవేళ ( len )
          face->face_flags |= FT_FACE_FLAG_VARIATION;
        లేకపోతే
          face->face_flags &= ~FT_FACE_FLAG_VARIATION;
      }
    }

    /* enforce recomputation of auto-hinting data */
    ఒకవేళ ( !error && face->autohint.finalizer )
    {
      face->autohint.finalizer( face->autohint.దత్తాంశం );
      face->autohint.దత్తాంశం = ఖాళీ;
    }

    తిరిగి error;
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Get_MM_బరువు_సదిశ( FT_Face    face,
                          FT_UInt*   len,
                          FT_Fixed*  weightvector )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    ఒకవేళ ( len && !weightvector )
      తిరిగి FT_THROW( Invalid_వాదం );

    error = ft_face_get_mm_service( face, &service );
    ఒకవేళ ( !error )
    {
      error = FT_ERR( Invalid_వాదం );
      ఒకవేళ ( service->get_mm_weightvector )
        error = service->get_mm_weightvector( face, len, weightvector );
    }

    తిరిగి error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_సమితి_Var_Design_Coordinates( FT_Face    face,
                                 FT_UInt    అంకె_coords,
                                 FT_Fixed*  coords )
  {
    FT_Error                      error;
    FT_Service_MultiMasters       service_mm   = ఖాళీ;
    FT_Service_MetricsVariations  service_mvar = ఖాళీ;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    ఒకవేళ ( అంకె_coords && !coords )
      తిరిగి FT_THROW( Invalid_వాదం );

    ఒకవేళ ( !అంకె_coords && !FT_IS_VARIATION( face ) )
      తిరిగి FT_Err_Ok;  /* nothing to be done */

    error = ft_face_get_mm_service( face, &service_mm );
    ఒకవేళ ( !error )
    {
      error = FT_ERR( Invalid_వాదం );
      ఒకవేళ ( service_mm->సమితి_var_design )
        error = service_mm->సమితి_var_design( face, అంకె_coords, coords );

      ఒకవేళ ( !error || error == -1 || error == -2 )
      {
        FT_Bool  is_variation_old = FT_IS_VARIATION( face );


        ఒకవేళ ( error != -1 )
        {
          ఒకవేళ ( error == -2 ) /* -2 means is_variable. */
          {
            face->face_flags |= FT_FACE_FLAG_VARIATION;
            error             = FT_Err_Ok;
          }
          లేకపోతే
            face->face_flags &= ~FT_FACE_FLAG_VARIATION;
        }

        ఒకవేళ ( service_mm->construct_ps_name )
        {
          ఒకవేళ ( error == -1 )
          {
            /* The PS name of a named instance and a non-named instance */
            /* usually differs, even if the axis values are identical.  */
            ఒకవేళ ( is_variation_old != FT_IS_VARIATION( face ) )
              service_mm->construct_ps_name( face );
          }
          లేకపోతే
            service_mm->construct_ps_name( face );
        }
      }

      /* internal error code -1 means `no change'; we can exit immediately */
      ఒకవేళ ( error == -1 )
        తిరిగి FT_Err_Ok;
    }

    ఒకవేళ ( !error )
    {
      (శూన్య)ft_face_get_mvar_service( face, &service_mvar );

      ఒకవేళ ( service_mvar && service_mvar->metrics_adjust )
        service_mvar->metrics_adjust( face );
    }

    /* enforce recomputation of auto-hinting data */
    ఒకవేళ ( !error && face->autohint.finalizer )
    {
      face->autohint.finalizer( face->autohint.దత్తాంశం );
      face->autohint.దత్తాంశం = ఖాళీ;
    }

    తిరిగి error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Var_Design_Coordinates( FT_Face    face,
                                 FT_UInt    అంకె_coords,
                                 FT_Fixed*  coords )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    ఒకవేళ ( !coords )
      తిరిగి FT_THROW( Invalid_వాదం );

    error = ft_face_get_mm_service( face, &service );
    ఒకవేళ ( !error )
    {
      error = FT_ERR( Invalid_వాదం );
      ఒకవేళ ( service->get_var_design )
        error = service->get_var_design( face, అంకె_coords, coords );
    }

    తిరిగి error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_సమితి_MM_Blend_Coordinates( FT_Face    face,
                               FT_UInt    అంకె_coords,
                               FT_Fixed*  coords )
  {
    FT_Error                      error;
    FT_Service_MultiMasters       service_mm   = ఖాళీ;
    FT_Service_MetricsVariations  service_mvar = ఖాళీ;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    ఒకవేళ ( అంకె_coords && !coords )
      తిరిగి FT_THROW( Invalid_వాదం );

    error = ft_face_get_mm_service( face, &service_mm );
    ఒకవేళ ( !error )
    {
      error = FT_ERR( Invalid_వాదం );
      ఒకవేళ ( service_mm->సమితి_mm_blend )
        error = service_mm->సమితి_mm_blend( face, అంకె_coords, coords );

      ఒకవేళ ( !error || error == -1 )
      {
        FT_Bool  is_variation_old = FT_IS_VARIATION( face );


        ఒకవేళ ( అంకె_coords )
          face->face_flags |= FT_FACE_FLAG_VARIATION;
        లేకపోతే
          face->face_flags &= ~FT_FACE_FLAG_VARIATION;

        ఒకవేళ ( service_mm->construct_ps_name )
        {
          ఒకవేళ ( error == -1 )
          {
            /* The PS name of a named instance and a non-named instance */
            /* usually differs, even if the axis values are identical.  */
            ఒకవేళ ( is_variation_old != FT_IS_VARIATION( face ) )
              service_mm->construct_ps_name( face );
          }
          లేకపోతే
            service_mm->construct_ps_name( face );
        }
      }

      /* internal error code -1 means `no change'; we can exit immediately */
      ఒకవేళ ( error == -1 )
        తిరిగి FT_Err_Ok;
    }

    ఒకవేళ ( !error )
    {
      (శూన్య)ft_face_get_mvar_service( face, &service_mvar );

      ఒకవేళ ( service_mvar && service_mvar->metrics_adjust )
        service_mvar->metrics_adjust( face );
    }

    /* enforce recomputation of auto-hinting data */
    ఒకవేళ ( !error && face->autohint.finalizer )
    {
      face->autohint.finalizer( face->autohint.దత్తాంశం );
      face->autohint.దత్తాంశం = ఖాళీ;
    }

    తిరిగి error;
  }


  /* documentation is in ftmm.h */

  /* This is exactly the same as the previous function.  It exists for */
  /* orthogonality.                                                    */

  FT_EXPORT_DEF( FT_Error )
  FT_సమితి_Var_Blend_Coordinates( FT_Face    face,
                                FT_UInt    అంకె_coords,
                                FT_Fixed*  coords )
  {
    FT_Error                      error;
    FT_Service_MultiMasters       service_mm   = ఖాళీ;
    FT_Service_MetricsVariations  service_mvar = ఖాళీ;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    ఒకవేళ ( అంకె_coords && !coords )
      తిరిగి FT_THROW( Invalid_వాదం );

    error = ft_face_get_mm_service( face, &service_mm );
    ఒకవేళ ( !error )
    {
      error = FT_ERR( Invalid_వాదం );
      ఒకవేళ ( service_mm->సమితి_mm_blend )
        error = service_mm->సమితి_mm_blend( face, అంకె_coords, coords );

      ఒకవేళ ( !error || error == -1 || error == -2 )
      {
        FT_Bool  is_variation_old = FT_IS_VARIATION( face );


        ఒకవేళ ( error != -1 )
        {
          ఒకవేళ ( error == -2 ) /* -2 means is_variable. */
          {
            face->face_flags |= FT_FACE_FLAG_VARIATION;
            error             = FT_Err_Ok;
          }
          లేకపోతే
            face->face_flags &= ~FT_FACE_FLAG_VARIATION;
        }

        ఒకవేళ ( service_mm->construct_ps_name )
        {
          ఒకవేళ ( error == -1 )
          {
            /* The PS name of a named instance and a non-named instance */
            /* usually differs, even if the axis values are identical.  */
            ఒకవేళ ( is_variation_old != FT_IS_VARIATION( face ) )
              service_mm->construct_ps_name( face );
          }
          లేకపోతే
            service_mm->construct_ps_name( face );
        }
      }

      /* internal error code -1 means `no change'; we can exit immediately */
      ఒకవేళ ( error == -1 )
        తిరిగి FT_Err_Ok;
    }

    ఒకవేళ ( !error )
    {
      (శూన్య)ft_face_get_mvar_service( face, &service_mvar );

      ఒకవేళ ( service_mvar && service_mvar->metrics_adjust )
        service_mvar->metrics_adjust( face );
    }

    /* enforce recomputation of auto-hinting data */
    ఒకవేళ ( !error && face->autohint.finalizer )
    {
      face->autohint.finalizer( face->autohint.దత్తాంశం );
      face->autohint.దత్తాంశం = ఖాళీ;
    }

    తిరిగి error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_MM_Blend_Coordinates( FT_Face    face,
                               FT_UInt    అంకె_coords,
                               FT_Fixed*  coords )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    ఒకవేళ ( !coords )
      తిరిగి FT_THROW( Invalid_వాదం );

    error = ft_face_get_mm_service( face, &service );
    ఒకవేళ ( !error )
    {
      error = FT_ERR( Invalid_వాదం );
      ఒకవేళ ( service->get_mm_blend )
        error = service->get_mm_blend( face, అంకె_coords, coords );
    }

    తిరిగి error;
  }


  /* documentation is in ftmm.h */

  /* This is exactly the same as the previous function.  It exists for */
  /* orthogonality.                                                    */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Var_Blend_Coordinates( FT_Face    face,
                                FT_UInt    అంకె_coords,
                                FT_Fixed*  coords )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    ఒకవేళ ( !coords )
      తిరిగి FT_THROW( Invalid_వాదం );

    error = ft_face_get_mm_service( face, &service );
    ఒకవేళ ( !error )
    {
      error = FT_ERR( Invalid_వాదం );
      ఒకవేళ ( service->get_mm_blend )
        error = service->get_mm_blend( face, అంకె_coords, coords );
    }

    తిరిగి error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Var_Axis_Flags( FT_MM_Var*  master,
                         FT_UInt     axis_సూచీ,
                         FT_UInt*    flags )
  {
    FT_UShort*  axis_flags;


    ఒకవేళ ( !master || !flags )
      తిరిగి FT_THROW( Invalid_వాదం );

    ఒకవేళ ( axis_సూచీ >= master->అంకె_axis )
      తిరిగి FT_THROW( Invalid_వాదం );

    /* the axis flags array immediately follows the data of `master' */
    axis_flags = (FT_UShort*)&( master[1] );
    *flags     = axis_flags[axis_సూచీ];

    తిరిగి FT_Err_Ok;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_సమితి_Named_Instance( FT_Face  face,
                         FT_UInt  instance_సూచీ )
  {
    FT_Error  error;

    FT_Service_MultiMasters       service_mm   = ఖాళీ;
    FT_Service_MetricsVariations  service_mvar = ఖాళీ;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    error = ft_face_get_mm_service( face, &service_mm );
    ఒకవేళ ( !error )
    {
      error = FT_ERR( Invalid_వాదం );
      ఒకవేళ ( service_mm->సమితి_named_instance )
        error = service_mm->సమితి_named_instance( face, instance_సూచీ );

      ఒకవేళ ( !error || error == -1 )
      {
        FT_Bool  is_variation_old = FT_IS_VARIATION( face );


        face->face_flags &= ~FT_FACE_FLAG_VARIATION;
        face->face_సూచీ  = ( instance_సూచీ << 16 )        |
                            ( face->face_సూచీ & 0xFFFFL );

        ఒకవేళ ( service_mm->construct_ps_name )
        {
          ఒకవేళ ( error == -1 )
          {
            /* The PS name of a named instance and a non-named instance */
            /* usually differs, even if the axis values are identical.  */
            ఒకవేళ ( is_variation_old != FT_IS_VARIATION( face ) )
              service_mm->construct_ps_name( face );
          }
          లేకపోతే
            service_mm->construct_ps_name( face );
        }
      }

      /* internal error code -1 means `no change'; we can exit immediately */
      ఒకవేళ ( error == -1 )
        తిరిగి FT_Err_Ok;
    }

    ఒకవేళ ( !error )
    {
      (శూన్య)ft_face_get_mvar_service( face, &service_mvar );

      ఒకవేళ ( service_mvar && service_mvar->metrics_adjust )
        service_mvar->metrics_adjust( face );
    }

    /* enforce recomputation of auto-hinting data */
    ఒకవేళ ( !error && face->autohint.finalizer )
    {
      face->autohint.finalizer( face->autohint.దత్తాంశం );
      face->autohint.దత్తాంశం = ఖాళీ;
    }

    తిరిగి error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Default_Named_Instance( FT_Face   face,
                                 FT_UInt  *instance_సూచీ )
  {
    FT_Error  error;

    FT_Service_MultiMasters  service_mm = ఖాళీ;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    error = ft_face_get_mm_service( face, &service_mm );
    ఒకవేళ ( !error )
    {
      /* no error if `get_default_named_instance` is not available */
      ఒకవేళ ( service_mm->get_default_named_instance )
        error = service_mm->get_default_named_instance( face,
                                                        instance_సూచీ );
      లేకపోతే
        error = FT_Err_Ok;
    }

    తిరిగి error;
  }


/*చివర*/
