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


#অন্তর্ভাব <freetype/internal/ftdebug.h>

#অন্তর্ভাব <freetype/ftmm.h>
#অন্তর্ভাব <freetype/internal/ftobjs.h>
#অন্তর্ভাব <freetype/internal/services/svmm.h>
#অন্তর্ভাব <freetype/internal/services/svmetric.h>


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#undef  FT_COMPONENT
#define FT_COMPONENT  mm


  স্থির FT_Error
  ft_face_get_mm_service( FT_Face                   face,
                          FT_Service_MultiMasters  *aservice )
  {
    FT_Error  error;


    *aservice = ফাঁকা;

    যদি ( !face )
      ফেরত FT_THROW( Invalid_Face_Handle );

    error = FT_ERR( Invalid_Argument );

    যদি ( FT_HAS_MULTIPLE_MASTERS( face ) )
    {
      FT_FACE_LOOKUP_SERVICE( face,
                              *aservice,
                              MULTI_MASTERS );

      যদি ( *aservice )
        error = FT_Err_Ok;
    }

    ফেরত error;
  }


  স্থির FT_Error
  ft_face_get_mvar_service( FT_Face                        face,
                            FT_Service_MetricsVariations  *aservice )
  {
    FT_Error  error;


    *aservice = ফাঁকা;

    যদি ( !face )
      ফেরত FT_THROW( Invalid_Face_Handle );

    error = FT_ERR( Invalid_Argument );

    যদি ( FT_HAS_MULTIPLE_MASTERS( face ) )
    {
      FT_FACE_LOOKUP_SERVICE( face,
                              *aservice,
                              METRICS_VARIATIONS );

      যদি ( *aservice )
        error = FT_Err_Ok;
    }

    ফেরত error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Multi_Master( FT_Face           face,
                       FT_Multi_Master  *amaster )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    যদি ( !amaster )
      ফেরত FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service );
    যদি ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      যদি ( service->get_mm )
        error = service->get_mm( face, amaster );
    }

    ফেরত error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_MM_Var( FT_Face      face,
                 FT_MM_Var*  *amaster )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    যদি ( !amaster )
      ফেরত FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service );
    যদি ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      যদি ( service->get_mm_var )
        error = service->get_mm_var( face, amaster );
    }

    ফেরত error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Done_MM_Var( FT_Library  library,
                  FT_MM_Var*  amaster )
  {
    FT_Memory  memory;


    যদি ( !library )
      ফেরত FT_THROW( Invalid_Library_Handle );

    memory = library->memory;
    FT_FREE( amaster );

    ফেরত FT_Err_Ok;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Set_MM_Design_Coordinates( FT_Face   face,
                                FT_UInt   অঙ্ক_coords,
                                FT_Long*  coords )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    যদি ( অঙ্ক_coords && !coords )
      ফেরত FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service );
    যদি ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      যদি ( service->set_mm_design )
        error = service->set_mm_design( face, অঙ্ক_coords, coords );

      যদি ( !error )
      {
        যদি ( অঙ্ক_coords )
          face->face_flags |= FT_FACE_FLAG_VARIATION;
        অন্যথা
          face->face_flags &= ~FT_FACE_FLAG_VARIATION;
      }
    }

    /* enforce recomputation of auto-hinting data */
    যদি ( !error && face->autohint.finalizer )
    {
      face->autohint.finalizer( face->autohint.তথ্য );
      face->autohint.তথ্য = ফাঁকা;
    }

    ফেরত error;
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

    যদি ( len && !weightvector )
      ফেরত FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service );
    যদি ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      যদি ( service->set_mm_weightvector )
        error = service->set_mm_weightvector( face, len, weightvector );

      যদি ( !error )
      {
        যদি ( len )
          face->face_flags |= FT_FACE_FLAG_VARIATION;
        অন্যথা
          face->face_flags &= ~FT_FACE_FLAG_VARIATION;
      }
    }

    /* enforce recomputation of auto-hinting data */
    যদি ( !error && face->autohint.finalizer )
    {
      face->autohint.finalizer( face->autohint.তথ্য );
      face->autohint.তথ্য = ফাঁকা;
    }

    ফেরত error;
  }


  FT_EXPORT_DEF( FT_Error )
  FT_Get_MM_WeightVector( FT_Face    face,
                          FT_UInt*   len,
                          FT_Fixed*  weightvector )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    যদি ( len && !weightvector )
      ফেরত FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service );
    যদি ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      যদি ( service->get_mm_weightvector )
        error = service->get_mm_weightvector( face, len, weightvector );
    }

    ফেরত error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Set_Var_Design_Coordinates( FT_Face    face,
                                 FT_UInt    অঙ্ক_coords,
                                 FT_Fixed*  coords )
  {
    FT_Error                      error;
    FT_Service_MultiMasters       service_mm   = ফাঁকা;
    FT_Service_MetricsVariations  service_mvar = ফাঁকা;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    যদি ( অঙ্ক_coords && !coords )
      ফেরত FT_THROW( Invalid_Argument );

    যদি ( !অঙ্ক_coords && !FT_IS_VARIATION( face ) )
      ফেরত FT_Err_Ok;  /* nothing to be done */

    error = ft_face_get_mm_service( face, &service_mm );
    যদি ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      যদি ( service_mm->set_var_design )
        error = service_mm->set_var_design( face, অঙ্ক_coords, coords );

      যদি ( !error || error == -1 || error == -2 )
      {
        FT_Bool  is_variation_old = FT_IS_VARIATION( face );


        যদি ( error != -1 )
        {
          যদি ( error == -2 ) /* -2 means is_variable. */
          {
            face->face_flags |= FT_FACE_FLAG_VARIATION;
            error             = FT_Err_Ok;
          }
          অন্যথা
            face->face_flags &= ~FT_FACE_FLAG_VARIATION;
        }

        যদি ( service_mm->construct_ps_name )
        {
          যদি ( error == -1 )
          {
            /* The PS name of a named instance and a non-named instance */
            /* usually differs, even if the axis values are identical.  */
            যদি ( is_variation_old != FT_IS_VARIATION( face ) )
              service_mm->construct_ps_name( face );
          }
          অন্যথা
            service_mm->construct_ps_name( face );
        }
      }

      /* internal error code -1 means `no change'; we can exit immediately */
      যদি ( error == -1 )
        ফেরত FT_Err_Ok;
    }

    যদি ( !error )
    {
      (শূন্য)ft_face_get_mvar_service( face, &service_mvar );

      যদি ( service_mvar && service_mvar->metrics_adjust )
        service_mvar->metrics_adjust( face );
    }

    /* enforce recomputation of auto-hinting data */
    যদি ( !error && face->autohint.finalizer )
    {
      face->autohint.finalizer( face->autohint.তথ্য );
      face->autohint.তথ্য = ফাঁকা;
    }

    ফেরত error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Var_Design_Coordinates( FT_Face    face,
                                 FT_UInt    অঙ্ক_coords,
                                 FT_Fixed*  coords )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    যদি ( !coords )
      ফেরত FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service );
    যদি ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      যদি ( service->get_var_design )
        error = service->get_var_design( face, অঙ্ক_coords, coords );
    }

    ফেরত error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Set_MM_Blend_Coordinates( FT_Face    face,
                               FT_UInt    অঙ্ক_coords,
                               FT_Fixed*  coords )
  {
    FT_Error                      error;
    FT_Service_MultiMasters       service_mm   = ফাঁকা;
    FT_Service_MetricsVariations  service_mvar = ফাঁকা;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    যদি ( অঙ্ক_coords && !coords )
      ফেরত FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service_mm );
    যদি ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      যদি ( service_mm->set_mm_blend )
        error = service_mm->set_mm_blend( face, অঙ্ক_coords, coords );

      যদি ( !error || error == -1 )
      {
        FT_Bool  is_variation_old = FT_IS_VARIATION( face );


        যদি ( অঙ্ক_coords )
          face->face_flags |= FT_FACE_FLAG_VARIATION;
        অন্যথা
          face->face_flags &= ~FT_FACE_FLAG_VARIATION;

        যদি ( service_mm->construct_ps_name )
        {
          যদি ( error == -1 )
          {
            /* The PS name of a named instance and a non-named instance */
            /* usually differs, even if the axis values are identical.  */
            যদি ( is_variation_old != FT_IS_VARIATION( face ) )
              service_mm->construct_ps_name( face );
          }
          অন্যথা
            service_mm->construct_ps_name( face );
        }
      }

      /* internal error code -1 means `no change'; we can exit immediately */
      যদি ( error == -1 )
        ফেরত FT_Err_Ok;
    }

    যদি ( !error )
    {
      (শূন্য)ft_face_get_mvar_service( face, &service_mvar );

      যদি ( service_mvar && service_mvar->metrics_adjust )
        service_mvar->metrics_adjust( face );
    }

    /* enforce recomputation of auto-hinting data */
    যদি ( !error && face->autohint.finalizer )
    {
      face->autohint.finalizer( face->autohint.তথ্য );
      face->autohint.তথ্য = ফাঁকা;
    }

    ফেরত error;
  }


  /* documentation is in ftmm.h */

  /* This is exactly the same as the previous function.  It exists for */
  /* orthogonality.                                                    */

  FT_EXPORT_DEF( FT_Error )
  FT_Set_Var_Blend_Coordinates( FT_Face    face,
                                FT_UInt    অঙ্ক_coords,
                                FT_Fixed*  coords )
  {
    FT_Error                      error;
    FT_Service_MultiMasters       service_mm   = ফাঁকা;
    FT_Service_MetricsVariations  service_mvar = ফাঁকা;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    যদি ( অঙ্ক_coords && !coords )
      ফেরত FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service_mm );
    যদি ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      যদি ( service_mm->set_mm_blend )
        error = service_mm->set_mm_blend( face, অঙ্ক_coords, coords );

      যদি ( !error || error == -1 || error == -2 )
      {
        FT_Bool  is_variation_old = FT_IS_VARIATION( face );


        যদি ( error != -1 )
        {
          যদি ( error == -2 ) /* -2 means is_variable. */
          {
            face->face_flags |= FT_FACE_FLAG_VARIATION;
            error             = FT_Err_Ok;
          }
          অন্যথা
            face->face_flags &= ~FT_FACE_FLAG_VARIATION;
        }

        যদি ( service_mm->construct_ps_name )
        {
          যদি ( error == -1 )
          {
            /* The PS name of a named instance and a non-named instance */
            /* usually differs, even if the axis values are identical.  */
            যদি ( is_variation_old != FT_IS_VARIATION( face ) )
              service_mm->construct_ps_name( face );
          }
          অন্যথা
            service_mm->construct_ps_name( face );
        }
      }

      /* internal error code -1 means `no change'; we can exit immediately */
      যদি ( error == -1 )
        ফেরত FT_Err_Ok;
    }

    যদি ( !error )
    {
      (শূন্য)ft_face_get_mvar_service( face, &service_mvar );

      যদি ( service_mvar && service_mvar->metrics_adjust )
        service_mvar->metrics_adjust( face );
    }

    /* enforce recomputation of auto-hinting data */
    যদি ( !error && face->autohint.finalizer )
    {
      face->autohint.finalizer( face->autohint.তথ্য );
      face->autohint.তথ্য = ফাঁকা;
    }

    ফেরত error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_MM_Blend_Coordinates( FT_Face    face,
                               FT_UInt    অঙ্ক_coords,
                               FT_Fixed*  coords )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    যদি ( !coords )
      ফেরত FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service );
    যদি ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      যদি ( service->get_mm_blend )
        error = service->get_mm_blend( face, অঙ্ক_coords, coords );
    }

    ফেরত error;
  }


  /* documentation is in ftmm.h */

  /* This is exactly the same as the previous function.  It exists for */
  /* orthogonality.                                                    */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Var_Blend_Coordinates( FT_Face    face,
                                FT_UInt    অঙ্ক_coords,
                                FT_Fixed*  coords )
  {
    FT_Error                 error;
    FT_Service_MultiMasters  service;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    যদি ( !coords )
      ফেরত FT_THROW( Invalid_Argument );

    error = ft_face_get_mm_service( face, &service );
    যদি ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      যদি ( service->get_mm_blend )
        error = service->get_mm_blend( face, অঙ্ক_coords, coords );
    }

    ফেরত error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Var_Axis_Flags( FT_MM_Var*  master,
                         FT_UInt     axis_সূচক,
                         FT_UInt*    flags )
  {
    FT_UShort*  axis_flags;


    যদি ( !master || !flags )
      ফেরত FT_THROW( Invalid_Argument );

    যদি ( axis_সূচক >= master->অঙ্ক_axis )
      ফেরত FT_THROW( Invalid_Argument );

    /* the axis flags array immediately follows the data of `master' */
    axis_flags = (FT_UShort*)&( master[1] );
    *flags     = axis_flags[axis_সূচক];

    ফেরত FT_Err_Ok;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Set_Named_Instance( FT_Face  face,
                         FT_UInt  instance_সূচক )
  {
    FT_Error  error;

    FT_Service_MultiMasters       service_mm   = ফাঁকা;
    FT_Service_MetricsVariations  service_mvar = ফাঁকা;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    error = ft_face_get_mm_service( face, &service_mm );
    যদি ( !error )
    {
      error = FT_ERR( Invalid_Argument );
      যদি ( service_mm->set_named_instance )
        error = service_mm->set_named_instance( face, instance_সূচক );

      যদি ( !error || error == -1 )
      {
        FT_Bool  is_variation_old = FT_IS_VARIATION( face );


        face->face_flags &= ~FT_FACE_FLAG_VARIATION;
        face->face_সূচক  = ( instance_সূচক << 16 )        |
                            ( face->face_সূচক & 0xFFFFL );

        যদি ( service_mm->construct_ps_name )
        {
          যদি ( error == -1 )
          {
            /* The PS name of a named instance and a non-named instance */
            /* usually differs, even if the axis values are identical.  */
            যদি ( is_variation_old != FT_IS_VARIATION( face ) )
              service_mm->construct_ps_name( face );
          }
          অন্যথা
            service_mm->construct_ps_name( face );
        }
      }

      /* internal error code -1 means `no change'; we can exit immediately */
      যদি ( error == -1 )
        ফেরত FT_Err_Ok;
    }

    যদি ( !error )
    {
      (শূন্য)ft_face_get_mvar_service( face, &service_mvar );

      যদি ( service_mvar && service_mvar->metrics_adjust )
        service_mvar->metrics_adjust( face );
    }

    /* enforce recomputation of auto-hinting data */
    যদি ( !error && face->autohint.finalizer )
    {
      face->autohint.finalizer( face->autohint.তথ্য );
      face->autohint.তথ্য = ফাঁকা;
    }

    ফেরত error;
  }


  /* documentation is in ftmm.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Default_Named_Instance( FT_Face   face,
                                 FT_UInt  *instance_সূচক )
  {
    FT_Error  error;

    FT_Service_MultiMasters  service_mm = ফাঁকা;


    /* check of `face' delayed to `ft_face_get_mm_service' */

    error = ft_face_get_mm_service( face, &service_mm );
    যদি ( !error )
    {
      /* no error if `get_default_named_instance` is not available */
      যদি ( service_mm->get_default_named_instance )
        error = service_mm->get_default_named_instance( face,
                                                        instance_সূচক );
      অন্যথা
        error = FT_Err_Ok;
    }

    ফেরত error;
  }


/* END */
