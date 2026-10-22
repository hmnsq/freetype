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
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು  FT_COMPONENT
#ವ್ಯಾಖ್ಯಾನಿಸು FT_COMPONENT  psprops


  FT_ತಳ_CALLBACK_DEF( FT_Error )
  ps_property_ಗಣ( FT_Module    module,         /* PS_Driver */
                   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*  property_name,
                   ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ*  ಮೌಲ್ಯ,
                   FT_Bool      ಮೌಲ್ಯ_is_string )
  {
    FT_Error   error  = FT_Err_Ok;
    PS_Driver  driver = (PS_Driver)module;

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES
    FT_ಬಳಸದ( ಮೌಲ್ಯ_is_string );
#ಆದರೆ_ಅಂತ್ಯ


    ಆದರೆ ( !ft_strcmp( property_name, "darkening-parameters" ) )
    {
      FT_Int*  darken_params;
      FT_Int   x1, y1, x2, y2, x3, y3, x4, y4;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES
      FT_Int   dp[8];


      ಆದರೆ ( ಮೌಲ್ಯ_is_string )
      {
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*  s = (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*)ಮೌಲ್ಯ;
        ಅಕ್ಷರ*        ep;
        ಪೂರ್ಣಾಂಕ          i;


        /* eight comma-separated numbers */
        ಗಾಗಿ ( i = 0; i < 7; i++ )
        {
          dp[i] = (FT_Int)ft_strtol( s, &ep, 10 );
          ಆದರೆ ( *ep != ',' || s == ep )
            ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

          s = ep + 1;
        }

        dp[7] = (FT_Int)ft_strtol( s, &ep, 10 );
        ಆದರೆ ( !( *ep == '\0' || *ep == ' ' ) || s == ep )
          ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

        darken_params = dp;
      }
      ಇಲ್ಲದಿದ್ದರೆ
#ಆದರೆ_ಅಂತ್ಯ
        darken_params = (FT_Int*)ಮೌಲ್ಯ;

      x1 = darken_params[0];
      y1 = darken_params[1];
      x2 = darken_params[2];
      y2 = darken_params[3];
      x3 = darken_params[4];
      y3 = darken_params[5];
      x4 = darken_params[6];
      y4 = darken_params[7];

      ಆದರೆ ( x1 < 0   || x2 < 0   || x3 < 0   || x4 < 0   ||
           y1 < 0   || y2 < 0   || y3 < 0   || y4 < 0   ||
           x1 > x2  || x2 > x3  || x3 > x4              ||
           y1 > 500 || y2 > 500 || y3 > 500 || y4 > 500 )
        ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

      driver->darken_params[0] = x1;
      driver->darken_params[1] = y1;
      driver->darken_params[2] = x2;
      driver->darken_params[3] = y2;
      driver->darken_params[4] = x3;
      driver->darken_params[5] = y3;
      driver->darken_params[6] = x4;
      driver->darken_params[7] = y4;

      ಹಿಂತಿರುಗಿ error;
    }

    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ( !ft_strcmp( property_name, "hinting-engine" ) )
    {
#ಆದರೆ defined( CFF_CONFIG_OPTION_OLD_ENGINE ) || \
    defined( T1_CONFIG_OPTION_OLD_ENGINE  )
      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*  module_name = module->clazz->module_name;
#ಆದರೆ_ಅಂತ್ಯ


#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES
      ಆದರೆ ( ಮೌಲ್ಯ_is_string )
      {
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*  s = (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*)ಮೌಲ್ಯ;


        ಆದರೆ ( !ft_strcmp( s, "adobe" ) )
          driver->hinting_engine = FT_HINTING_ADOBE;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CFF_CONFIG_OPTION_OLD_ENGINE
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ( !ft_strcmp( module_name, "cff" ) &&
                  !ft_strcmp( s, "freetype" )      )
          driver->hinting_engine = FT_HINTING_FREETYPE;
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ T1_CONFIG_OPTION_OLD_ENGINE
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ( ( !ft_strcmp( module_name, "type1" ) ||
                    !ft_strcmp( module_name, "t1cid" ) ) &&
                  !ft_strcmp( s, "freetype" )            )
          driver->hinting_engine = FT_HINTING_FREETYPE;
#ಆದರೆ_ಅಂತ್ಯ

        ಇಲ್ಲದಿದ್ದರೆ
          ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );
      }
      ಇಲ್ಲದಿದ್ದರೆ
#ಆದರೆ_ಅಂತ್ಯ /* FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES */
      {
        FT_UInt*  hinting_engine = (FT_UInt*)ಮೌಲ್ಯ;


        ಆದರೆ ( *hinting_engine == FT_HINTING_ADOBE
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CFF_CONFIG_OPTION_OLD_ENGINE
             || ( *hinting_engine == FT_HINTING_FREETYPE &&
                  !ft_strcmp( module_name, "cff" )       )
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ T1_CONFIG_OPTION_OLD_ENGINE
             || ( *hinting_engine == FT_HINTING_FREETYPE &&
                  ( !ft_strcmp( module_name, "type1" ) ||
                    !ft_strcmp( module_name, "t1cid" ) ) )
#ಆದರೆ_ಅಂತ್ಯ
           )
          driver->hinting_engine = *hinting_engine;
        ಇಲ್ಲದಿದ್ದರೆ
          error = FT_ERR( Unimplemented_Feature );
      }

      ಹಿಂತಿರುಗಿ error;
    }

    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ( !ft_strcmp( property_name, "no-stem-darkening" ) )
    {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES
      ಆದರೆ ( ಮೌಲ್ಯ_is_string )
      {
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*  s   = (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*)ಮೌಲ್ಯ;
        ದೀರ್ಘ         nsd = ft_strtol( s, ಖಾಲಿ, 10 );


        ಆದರೆ ( !nsd )
          driver->no_stem_darkening = FALSE;
        ಇಲ್ಲದಿದ್ದರೆ
          driver->no_stem_darkening = TRUE;
      }
      ಇಲ್ಲದಿದ್ದರೆ
#ಆದರೆ_ಅಂತ್ಯ
      {
        FT_Bool*  no_stem_darkening = (FT_Bool*)ಮೌಲ್ಯ;


        driver->no_stem_darkening = *no_stem_darkening;
      }

      ಹಿಂತಿರುಗಿ error;
    }

    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ( !ft_strcmp( property_name, "random-seed" ) )
    {
      FT_Int32  random_ಬೀಜ;


#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES
      ಆದರೆ ( ಮೌಲ್ಯ_is_string )
      {
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*  s = (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*)ಮೌಲ್ಯ;


        random_ಬೀಜ = (FT_Int32)ft_strtol( s, ಖಾಲಿ, 10 );
      }
      ಇಲ್ಲದಿದ್ದರೆ
#ಆದರೆ_ಅಂತ್ಯ
        random_ಬೀಜ = *(FT_Int32*)ಮೌಲ್ಯ;

      ಆದರೆ ( random_ಬೀಜ < 0 )
        random_ಬೀಜ = 0;

      driver->random_ಬೀಜ = random_ಬೀಜ;

      ಹಿಂತಿರುಗಿ error;
    }

    FT_TRACE2(( "ps_property_set: missing property `%s'\n",
                property_name ));
    ಹಿಂತಿರುಗಿ FT_THROW( Missing_Property );
  }


  FT_ತಳ_CALLBACK_DEF( FT_Error )
  ps_property_get( FT_Module    module,         /* PS_Driver */
                   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*  property_name,
                   ಶೂನ್ಯ*        ಮೌಲ್ಯ )
  {
    FT_Error   error  = FT_Err_Ok;
    PS_Driver  driver = (PS_Driver)module;


    ಆದರೆ ( !ft_strcmp( property_name, "darkening-parameters" ) )
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

      ಹಿಂತಿರುಗಿ error;
    }

    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ( !ft_strcmp( property_name, "hinting-engine" ) )
    {
      FT_UInt   hinting_engine    = driver->hinting_engine;
      FT_UInt*  val               = (FT_UInt*)ಮೌಲ್ಯ;


      *val = hinting_engine;

      ಹಿಂತಿರುಗಿ error;
    }

    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ( !ft_strcmp( property_name, "no-stem-darkening" ) )
    {
      FT_Bool   no_stem_darkening = driver->no_stem_darkening;
      FT_Bool*  val               = (FT_Bool*)ಮೌಲ್ಯ;


      *val = no_stem_darkening;

      ಹಿಂತಿರುಗಿ error;
    }

    FT_TRACE2(( "ps_property_get: missing property `%s'\n",
                property_name ));
    ಹಿಂತಿರುಗಿ FT_THROW( Missing_Property );
  }


/*ಕೊನೆ*/
