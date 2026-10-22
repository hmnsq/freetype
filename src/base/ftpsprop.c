/****************************************************************************
 *
 * ftpsprop.c
 *
 *   Get and set properties of PostScript drivers (body).
 *   See `ftdriver.h' for available properties.
 *
 * Copyright (C) 2017-2026 by
 * David Turner, Robert Wilhelm, and Werner Lemberg.
 *
 * This file is part of the FreeType project, and may only be used,
 * modified, and distributed under the terms of the FreeType project
 * license, LICENSE.TXT.  By continuing to use, modify, or distribute
 * this file you indicate that you have read the license and
 * understand and accept it fully.
 *
 */


#ಅನ್ತರ್ಭಾವ <freetype/ftdriver.h>
#ಅನ್ತರ್ಭಾವ <freetype/internal/ftdebug.h>
#ಅನ್ತರ್ಭಾವ <freetype/internal/psaux.h>
#ಅನ್ತರ್ಭಾವ <freetype/internal/ftobjs.h>
#ಅನ್ತರ್ಭಾವ <freetype/internal/ftpsprop.h>


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#undef  FT_COMPONENT
#define FT_COMPONENT  psprops


  FT_BASE_CALLBACK_DEF( FT_Error )
  ps_property_set( FT_Module    module,         /* PS_Driver */
                   ಧ್ರುವ ವರ್ಣ*  property_name,
                   ಧ್ರುವ ಶೂನ್ಯ*  ಮೌಲ್ಯ,
                   FT_Bool      ಮೌಲ್ಯ_is_string )
  {
    FT_Error   error  = FT_Err_Ok;
    PS_Driver  driver = (PS_Driver)module;

#ifndef FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES
    FT_UNUSED( ಮೌಲ್ಯ_is_string );
#endif


    ಯದಿ ( !ft_strcmp( property_name, "darkening-parameters" ) )
    {
      FT_Int*  darken_params;
      FT_Int   x1, y1, x2, y2, x3, y3, x4, y4;

#ifdef FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES
      FT_Int   dp[8];


      ಯದಿ ( ಮೌಲ್ಯ_is_string )
      {
        ಧ್ರುವ ವರ್ಣ*  s = (ಧ್ರುವ ವರ್ಣ*)ಮೌಲ್ಯ;
        ವರ್ಣ*        ep;
        ಪೂರ್ಣಾಙ್ಕ          i;


        /* eight comma-separated numbers */
        ಪ್ರತಿ ( i = 0; i < 7; i++ )
        {
          dp[i] = (FT_Int)ft_strtol( s, &ep, 10 );
          ಯದಿ ( *ep != ',' || s == ep )
            ಪ್ರತ್ಯಾಗಮ FT_THROW( Invalid_Argument );

          s = ep + 1;
        }

        dp[7] = (FT_Int)ft_strtol( s, &ep, 10 );
        ಯದಿ ( !( *ep == '\0' || *ep == ' ' ) || s == ep )
          ಪ್ರತ್ಯಾಗಮ FT_THROW( Invalid_Argument );

        darken_params = dp;
      }
      ಅನ್ಯಥಾ
#endif
        darken_params = (FT_Int*)ಮೌಲ್ಯ;

      x1 = darken_params[0];
      y1 = darken_params[1];
      x2 = darken_params[2];
      y2 = darken_params[3];
      x3 = darken_params[4];
      y3 = darken_params[5];
      x4 = darken_params[6];
      y4 = darken_params[7];

      ಯದಿ ( x1 < 0   || x2 < 0   || x3 < 0   || x4 < 0   ||
           y1 < 0   || y2 < 0   || y3 < 0   || y4 < 0   ||
           x1 > x2  || x2 > x3  || x3 > x4              ||
           y1 > 500 || y2 > 500 || y3 > 500 || y4 > 500 )
        ಪ್ರತ್ಯಾಗಮ FT_THROW( Invalid_Argument );

      driver->darken_params[0] = x1;
      driver->darken_params[1] = y1;
      driver->darken_params[2] = x2;
      driver->darken_params[3] = y2;
      driver->darken_params[4] = x3;
      driver->darken_params[5] = y3;
      driver->darken_params[6] = x4;
      driver->darken_params[7] = y4;

      ಪ್ರತ್ಯಾಗಮ error;
    }

    ಅನ್ಯಥಾ ಯದಿ ( !ft_strcmp( property_name, "hinting-engine" ) )
    {
#ಯದಿ defined( CFF_CONFIG_OPTION_OLD_ENGINE ) || \
    defined( T1_CONFIG_OPTION_OLD_ENGINE  )
      ಧ್ರುವ ವರ್ಣ*  module_name = module->clazz->module_name;
#endif


#ifdef FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES
      ಯದಿ ( ಮೌಲ್ಯ_is_string )
      {
        ಧ್ರುವ ವರ್ಣ*  s = (ಧ್ರುವ ವರ್ಣ*)ಮೌಲ್ಯ;


        ಯದಿ ( !ft_strcmp( s, "adobe" ) )
          driver->hinting_engine = FT_HINTING_ADOBE;

#ifdef CFF_CONFIG_OPTION_OLD_ENGINE
        ಅನ್ಯಥಾ ಯದಿ ( !ft_strcmp( module_name, "cff" ) &&
                  !ft_strcmp( s, "freetype" )      )
          driver->hinting_engine = FT_HINTING_FREETYPE;
#endif

#ifdef T1_CONFIG_OPTION_OLD_ENGINE
        ಅನ್ಯಥಾ ಯದಿ ( ( !ft_strcmp( module_name, "type1" ) ||
                    !ft_strcmp( module_name, "t1cid" ) ) &&
                  !ft_strcmp( s, "freetype" )            )
          driver->hinting_engine = FT_HINTING_FREETYPE;
#endif

        ಅನ್ಯಥಾ
          ಪ್ರತ್ಯಾಗಮ FT_THROW( Invalid_Argument );
      }
      ಅನ್ಯಥಾ
#endif /* FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES */
      {
        FT_UInt*  hinting_engine = (FT_UInt*)ಮೌಲ್ಯ;


        ಯದಿ ( *hinting_engine == FT_HINTING_ADOBE
#ifdef CFF_CONFIG_OPTION_OLD_ENGINE
             || ( *hinting_engine == FT_HINTING_FREETYPE &&
                  !ft_strcmp( module_name, "cff" )       )
#endif
#ifdef T1_CONFIG_OPTION_OLD_ENGINE
             || ( *hinting_engine == FT_HINTING_FREETYPE &&
                  ( !ft_strcmp( module_name, "type1" ) ||
                    !ft_strcmp( module_name, "t1cid" ) ) )
#endif
           )
          driver->hinting_engine = *hinting_engine;
        ಅನ್ಯಥಾ
          error = FT_ERR( Unimplemented_Feature );
      }

      ಪ್ರತ್ಯಾಗಮ error;
    }

    ಅನ್ಯಥಾ ಯದಿ ( !ft_strcmp( property_name, "no-stem-darkening" ) )
    {
#ifdef FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES
      ಯದಿ ( ಮೌಲ್ಯ_is_string )
      {
        ಧ್ರುವ ವರ್ಣ*  s   = (ಧ್ರುವ ವರ್ಣ*)ಮೌಲ್ಯ;
        long         nsd = ft_strtol( s, ರಿಕ್ತ, 10 );


        ಯದಿ ( !nsd )
          driver->no_stem_darkening = FALSE;
        ಅನ್ಯಥಾ
          driver->no_stem_darkening = TRUE;
      }
      ಅನ್ಯಥಾ
#endif
      {
        FT_Bool*  no_stem_darkening = (FT_Bool*)ಮೌಲ್ಯ;


        driver->no_stem_darkening = *no_stem_darkening;
      }

      ಪ್ರತ್ಯಾಗಮ error;
    }

    ಅನ್ಯಥಾ ಯದಿ ( !ft_strcmp( property_name, "random-seed" ) )
    {
      FT_Int32  random_seed;


#ifdef FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES
      ಯದಿ ( ಮೌಲ್ಯ_is_string )
      {
        ಧ್ರುವ ವರ್ಣ*  s = (ಧ್ರುವ ವರ್ಣ*)ಮೌಲ್ಯ;


        random_seed = (FT_Int32)ft_strtol( s, ರಿಕ್ತ, 10 );
      }
      ಅನ್ಯಥಾ
#endif
        random_seed = *(FT_Int32*)ಮೌಲ್ಯ;

      ಯದಿ ( random_seed < 0 )
        random_seed = 0;

      driver->random_seed = random_seed;

      ಪ್ರತ್ಯಾಗಮ error;
    }

    FT_TRACE2(( "ps_property_set: missing property `%s'\n",
                property_name ));
    ಪ್ರತ್ಯಾಗಮ FT_THROW( Missing_Property );
  }


  FT_BASE_CALLBACK_DEF( FT_Error )
  ps_property_get( FT_Module    module,         /* PS_Driver */
                   ಧ್ರುವ ವರ್ಣ*  property_name,
                   ಶೂನ್ಯ*        ಮೌಲ್ಯ )
  {
    FT_Error   error  = FT_Err_Ok;
    PS_Driver  driver = (PS_Driver)module;


    ಯದಿ ( !ft_strcmp( property_name, "darkening-parameters" ) )
    {
      FT_Int*  darken_params = driver->darken_params;
      FT_Int*  val           = (FT_Int*)ಮೌಲ್ಯ;


      val[0] = darken_params[0];
      val[1] = darken_params[1];
      val[2] = darken_params[2];
      val[3] = darken_params[3];
      val[4] = darken_params[4];
      val[5] = darken_params[5];
      val[6] = darken_params[6];
      val[7] = darken_params[7];

      ಪ್ರತ್ಯಾಗಮ error;
    }

    ಅನ್ಯಥಾ ಯದಿ ( !ft_strcmp( property_name, "hinting-engine" ) )
    {
      FT_UInt   hinting_engine    = driver->hinting_engine;
      FT_UInt*  val               = (FT_UInt*)ಮೌಲ್ಯ;


      *val = hinting_engine;

      ಪ್ರತ್ಯಾಗಮ error;
    }

    ಅನ್ಯಥಾ ಯದಿ ( !ft_strcmp( property_name, "no-stem-darkening" ) )
    {
      FT_Bool   no_stem_darkening = driver->no_stem_darkening;
      FT_Bool*  val               = (FT_Bool*)ಮೌಲ್ಯ;


      *val = no_stem_darkening;

      ಪ್ರತ್ಯಾಗಮ error;
    }

    FT_TRACE2(( "ps_property_get: missing property `%s'\n",
                property_name ));
    ಪ್ರತ್ಯಾಗಮ FT_THROW( Missing_Property );
  }


/* END */
