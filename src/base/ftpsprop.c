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


#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/ftdriver.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftdebug.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/psaux.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftobjs.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftpsprop.h>


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ  FT_COMPONENT
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FT_COMPONENT  psprops


  FT_ਅਧਾਰ_CALLBACK_DEF( FT_Error )
  ps_property_ਸੈੱਟ( FT_Module    module,         /* PS_Driver */
                   ਅਚਲ ਅੱਖਰ*  property_name,
                   ਅਚਲ ਖਾਲੀ*  ਮੁੱਲ,
                   FT_Bool      ਮੁੱਲ_is_string )
  {
    FT_Error   error  = FT_Err_Ok;
    PS_Driver  driver = (PS_Driver)module;

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES
    FT_ਅਣਵਰਤਿਆ( ਮੁੱਲ_is_string );
#ਜੇ_ਅੰਤ


    ਜੇ ( !ft_strcmp( property_name, "darkening-parameters" ) )
    {
      FT_Int*  darken_params;
      FT_Int   x1, y1, x2, y2, x3, y3, x4, y4;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES
      FT_Int   dp[8];


      ਜੇ ( ਮੁੱਲ_is_string )
      {
        ਅਚਲ ਅੱਖਰ*  s = (ਅਚਲ ਅੱਖਰ*)ਮੁੱਲ;
        ਅੱਖਰ*        ep;
        ਪੂਰਨ          i;


        /* eight comma-separated numbers */
        ਲਈ ( i = 0; i < 7; i++ )
        {
          dp[i] = (FT_Int)ft_strtol( s, &ep, 10 );
          ਜੇ ( *ep != ',' || s == ep )
            ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

          s = ep + 1;
        }

        dp[7] = (FT_Int)ft_strtol( s, &ep, 10 );
        ਜੇ ( !( *ep == '\0' || *ep == ' ' ) || s == ep )
          ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

        darken_params = dp;
      }
      ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
        darken_params = (FT_Int*)ਮੁੱਲ;

      x1 = darken_params[0];
      y1 = darken_params[1];
      x2 = darken_params[2];
      y2 = darken_params[3];
      x3 = darken_params[4];
      y3 = darken_params[5];
      x4 = darken_params[6];
      y4 = darken_params[7];

      ਜੇ ( x1 < 0   || x2 < 0   || x3 < 0   || x4 < 0   ||
           y1 < 0   || y2 < 0   || y3 < 0   || y4 < 0   ||
           x1 > x2  || x2 > x3  || x3 > x4              ||
           y1 > 500 || y2 > 500 || y3 > 500 || y4 > 500 )
        ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

      driver->darken_params[0] = x1;
      driver->darken_params[1] = y1;
      driver->darken_params[2] = x2;
      driver->darken_params[3] = y2;
      driver->darken_params[4] = x3;
      driver->darken_params[5] = y3;
      driver->darken_params[6] = x4;
      driver->darken_params[7] = y4;

      ਵਾਪਸ error;
    }

    ਨਹੀਂਤਾਂ ਜੇ ( !ft_strcmp( property_name, "hinting-engine" ) )
    {
#ਜੇ defined( CFF_CONFIG_OPTION_OLD_ENGINE ) || \
    defined( T1_CONFIG_OPTION_OLD_ENGINE  )
      ਅਚਲ ਅੱਖਰ*  module_name = module->clazz->module_name;
#ਜੇ_ਅੰਤ


#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES
      ਜੇ ( ਮੁੱਲ_is_string )
      {
        ਅਚਲ ਅੱਖਰ*  s = (ਅਚਲ ਅੱਖਰ*)ਮੁੱਲ;


        ਜੇ ( !ft_strcmp( s, "adobe" ) )
          driver->hinting_engine = FT_HINTING_ADOBE;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CFF_CONFIG_OPTION_OLD_ENGINE
        ਨਹੀਂਤਾਂ ਜੇ ( !ft_strcmp( module_name, "cff" ) &&
                  !ft_strcmp( s, "freetype" )      )
          driver->hinting_engine = FT_HINTING_FREETYPE;
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ T1_CONFIG_OPTION_OLD_ENGINE
        ਨਹੀਂਤਾਂ ਜੇ ( ( !ft_strcmp( module_name, "type1" ) ||
                    !ft_strcmp( module_name, "t1cid" ) ) &&
                  !ft_strcmp( s, "freetype" )            )
          driver->hinting_engine = FT_HINTING_FREETYPE;
#ਜੇ_ਅੰਤ

        ਨਹੀਂਤਾਂ
          ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );
      }
      ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ /* FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES */
      {
        FT_UInt*  hinting_engine = (FT_UInt*)ਮੁੱਲ;


        ਜੇ ( *hinting_engine == FT_HINTING_ADOBE
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CFF_CONFIG_OPTION_OLD_ENGINE
             || ( *hinting_engine == FT_HINTING_FREETYPE &&
                  !ft_strcmp( module_name, "cff" )       )
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ T1_CONFIG_OPTION_OLD_ENGINE
             || ( *hinting_engine == FT_HINTING_FREETYPE &&
                  ( !ft_strcmp( module_name, "type1" ) ||
                    !ft_strcmp( module_name, "t1cid" ) ) )
#ਜੇ_ਅੰਤ
           )
          driver->hinting_engine = *hinting_engine;
        ਨਹੀਂਤਾਂ
          error = FT_ERR( Unimplemented_Feature );
      }

      ਵਾਪਸ error;
    }

    ਨਹੀਂਤਾਂ ਜੇ ( !ft_strcmp( property_name, "no-stem-darkening" ) )
    {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES
      ਜੇ ( ਮੁੱਲ_is_string )
      {
        ਅਚਲ ਅੱਖਰ*  s   = (ਅਚਲ ਅੱਖਰ*)ਮੁੱਲ;
        ਲੰਮਾ         nsd = ft_strtol( s, ਨਿਰਾ, 10 );


        ਜੇ ( !nsd )
          driver->no_stem_darkening = FALSE;
        ਨਹੀਂਤਾਂ
          driver->no_stem_darkening = TRUE;
      }
      ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
      {
        FT_Bool*  no_stem_darkening = (FT_Bool*)ਮੁੱਲ;


        driver->no_stem_darkening = *no_stem_darkening;
      }

      ਵਾਪਸ error;
    }

    ਨਹੀਂਤਾਂ ਜੇ ( !ft_strcmp( property_name, "random-seed" ) )
    {
      FT_Int32  random_ਬੀਜ;


#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES
      ਜੇ ( ਮੁੱਲ_is_string )
      {
        ਅਚਲ ਅੱਖਰ*  s = (ਅਚਲ ਅੱਖਰ*)ਮੁੱਲ;


        random_ਬੀਜ = (FT_Int32)ft_strtol( s, ਨਿਰਾ, 10 );
      }
      ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
        random_ਬੀਜ = *(FT_Int32*)ਮੁੱਲ;

      ਜੇ ( random_ਬੀਜ < 0 )
        random_ਬੀਜ = 0;

      driver->random_ਬੀਜ = random_ਬੀਜ;

      ਵਾਪਸ error;
    }

    FT_TRACE2(( "ps_property_set: missing property `%s'\n",
                property_name ));
    ਵਾਪਸ FT_THROW( Missing_Property );
  }


  FT_ਅਧਾਰ_CALLBACK_DEF( FT_Error )
  ps_property_get( FT_Module    module,         /* PS_Driver */
                   ਅਚਲ ਅੱਖਰ*  property_name,
                   ਖਾਲੀ*        ਮੁੱਲ )
  {
    FT_Error   error  = FT_Err_Ok;
    PS_Driver  driver = (PS_Driver)module;


    ਜੇ ( !ft_strcmp( property_name, "darkening-parameters" ) )
    {
      FT_Int*  darken_params = driver->darken_params;
      FT_Int*  val           = (FT_Int*)ਮੁੱਲ;


      val[0] = darken_params[0];
      val[1] = darken_params[1];
      val[2] = darken_params[2];
      val[3] = darken_params[3];
      val[4] = darken_params[4];
      val[5] = darken_params[5];
      val[6] = darken_params[6];
      val[7] = darken_params[7];

      ਵਾਪਸ error;
    }

    ਨਹੀਂਤਾਂ ਜੇ ( !ft_strcmp( property_name, "hinting-engine" ) )
    {
      FT_UInt   hinting_engine    = driver->hinting_engine;
      FT_UInt*  val               = (FT_UInt*)ਮੁੱਲ;


      *val = hinting_engine;

      ਵਾਪਸ error;
    }

    ਨਹੀਂਤਾਂ ਜੇ ( !ft_strcmp( property_name, "no-stem-darkening" ) )
    {
      FT_Bool   no_stem_darkening = driver->no_stem_darkening;
      FT_Bool*  val               = (FT_Bool*)ਮੁੱਲ;


      *val = no_stem_darkening;

      ਵਾਪਸ error;
    }

    FT_TRACE2(( "ps_property_get: missing property `%s'\n",
                property_name ));
    ਵਾਪਸ FT_THROW( Missing_Property );
  }


/*ਸਿਰਾ*/
