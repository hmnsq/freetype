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


#అన్తర్భావ <freetype/ftdriver.h>
#అన్తర్భావ <freetype/internal/ftdebug.h>
#అన్తర్భావ <freetype/internal/psaux.h>
#అన్తర్భావ <freetype/internal/ftobjs.h>
#అన్తర్భావ <freetype/internal/ftpsprop.h>


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#నిర్వచనం_రద్దు  FT_COMPONENT
#నిర్వచించు FT_COMPONENT  psprops


  FT_పునాది_CALLBACK_DEF( FT_Error )
  ps_property_సమితి( FT_Module    module,         /* PS_Driver */
                   స్థిరరాశి అక్షరం*  property_name,
                   స్థిరరాశి శూన్య*  విలువ,
                   FT_Bool      విలువ_is_string )
  {
    FT_Error   error  = FT_Err_Ok;
    PS_Driver  driver = (PS_Driver)module;

#నిర్వచితం_కాకపోతే FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES
    FT_వాడని( విలువ_is_string );
#అయితే_ముగింపు


    ఒకవేళ ( !ft_strcmp( property_name, "darkening-parameters" ) )
    {
      FT_Int*  darken_params;
      FT_Int   x1, y1, x2, y2, x3, y3, x4, y4;

#నిర్వచితమైతే FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES
      FT_Int   dp[8];


      ఒకవేళ ( విలువ_is_string )
      {
        స్థిరరాశి అక్షరం*  s = (స్థిరరాశి అక్షరం*)విలువ;
        అక్షరం*        ep;
        పూర్ణాంకం          i;


        /* eight comma-separated numbers */
        కొరకు ( i = 0; i < 7; i++ )
        {
          dp[i] = (FT_Int)ft_strtol( s, &ep, 10 );
          ఒకవేళ ( *ep != ',' || s == ep )
            తిరిగి FT_THROW( Invalid_వాదం );

          s = ep + 1;
        }

        dp[7] = (FT_Int)ft_strtol( s, &ep, 10 );
        ఒకవేళ ( !( *ep == '\0' || *ep == ' ' ) || s == ep )
          తిరిగి FT_THROW( Invalid_వాదం );

        darken_params = dp;
      }
      లేకపోతే
#అయితే_ముగింపు
        darken_params = (FT_Int*)విలువ;

      x1 = darken_params[0];
      y1 = darken_params[1];
      x2 = darken_params[2];
      y2 = darken_params[3];
      x3 = darken_params[4];
      y3 = darken_params[5];
      x4 = darken_params[6];
      y4 = darken_params[7];

      ఒకవేళ ( x1 < 0   || x2 < 0   || x3 < 0   || x4 < 0   ||
           y1 < 0   || y2 < 0   || y3 < 0   || y4 < 0   ||
           x1 > x2  || x2 > x3  || x3 > x4              ||
           y1 > 500 || y2 > 500 || y3 > 500 || y4 > 500 )
        తిరిగి FT_THROW( Invalid_వాదం );

      driver->darken_params[0] = x1;
      driver->darken_params[1] = y1;
      driver->darken_params[2] = x2;
      driver->darken_params[3] = y2;
      driver->darken_params[4] = x3;
      driver->darken_params[5] = y3;
      driver->darken_params[6] = x4;
      driver->darken_params[7] = y4;

      తిరిగి error;
    }

    లేకపోతే ఒకవేళ ( !ft_strcmp( property_name, "hinting-engine" ) )
    {
#ఒకవేళ defined( CFF_CONFIG_OPTION_OLD_ENGINE ) || \
    defined( T1_CONFIG_OPTION_OLD_ENGINE  )
      స్థిరరాశి అక్షరం*  module_name = module->clazz->module_name;
#అయితే_ముగింపు


#నిర్వచితమైతే FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES
      ఒకవేళ ( విలువ_is_string )
      {
        స్థిరరాశి అక్షరం*  s = (స్థిరరాశి అక్షరం*)విలువ;


        ఒకవేళ ( !ft_strcmp( s, "adobe" ) )
          driver->hinting_engine = FT_HINTING_ADOBE;

#నిర్వచితమైతే CFF_CONFIG_OPTION_OLD_ENGINE
        లేకపోతే ఒకవేళ ( !ft_strcmp( module_name, "cff" ) &&
                  !ft_strcmp( s, "freetype" )      )
          driver->hinting_engine = FT_HINTING_FREETYPE;
#అయితే_ముగింపు

#నిర్వచితమైతే T1_CONFIG_OPTION_OLD_ENGINE
        లేకపోతే ఒకవేళ ( ( !ft_strcmp( module_name, "type1" ) ||
                    !ft_strcmp( module_name, "t1cid" ) ) &&
                  !ft_strcmp( s, "freetype" )            )
          driver->hinting_engine = FT_HINTING_FREETYPE;
#అయితే_ముగింపు

        లేకపోతే
          తిరిగి FT_THROW( Invalid_వాదం );
      }
      లేకపోతే
#అయితే_ముగింపు /* FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES */
      {
        FT_UInt*  hinting_engine = (FT_UInt*)విలువ;


        ఒకవేళ ( *hinting_engine == FT_HINTING_ADOBE
#నిర్వచితమైతే CFF_CONFIG_OPTION_OLD_ENGINE
             || ( *hinting_engine == FT_HINTING_FREETYPE &&
                  !ft_strcmp( module_name, "cff" )       )
#అయితే_ముగింపు
#నిర్వచితమైతే T1_CONFIG_OPTION_OLD_ENGINE
             || ( *hinting_engine == FT_HINTING_FREETYPE &&
                  ( !ft_strcmp( module_name, "type1" ) ||
                    !ft_strcmp( module_name, "t1cid" ) ) )
#అయితే_ముగింపు
           )
          driver->hinting_engine = *hinting_engine;
        లేకపోతే
          error = FT_ERR( Unimplemented_Feature );
      }

      తిరిగి error;
    }

    లేకపోతే ఒకవేళ ( !ft_strcmp( property_name, "no-stem-darkening" ) )
    {
#నిర్వచితమైతే FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES
      ఒకవేళ ( విలువ_is_string )
      {
        స్థిరరాశి అక్షరం*  s   = (స్థిరరాశి అక్షరం*)విలువ;
        దీర్ఘ         nsd = ft_strtol( s, ఖాళీ, 10 );


        ఒకవేళ ( !nsd )
          driver->no_stem_darkening = FALSE;
        లేకపోతే
          driver->no_stem_darkening = TRUE;
      }
      లేకపోతే
#అయితే_ముగింపు
      {
        FT_Bool*  no_stem_darkening = (FT_Bool*)విలువ;


        driver->no_stem_darkening = *no_stem_darkening;
      }

      తిరిగి error;
    }

    లేకపోతే ఒకవేళ ( !ft_strcmp( property_name, "random-seed" ) )
    {
      FT_Int32  random_విత్తనం;


#నిర్వచితమైతే FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES
      ఒకవేళ ( విలువ_is_string )
      {
        స్థిరరాశి అక్షరం*  s = (స్థిరరాశి అక్షరం*)విలువ;


        random_విత్తనం = (FT_Int32)ft_strtol( s, ఖాళీ, 10 );
      }
      లేకపోతే
#అయితే_ముగింపు
        random_విత్తనం = *(FT_Int32*)విలువ;

      ఒకవేళ ( random_విత్తనం < 0 )
        random_విత్తనం = 0;

      driver->random_విత్తనం = random_విత్తనం;

      తిరిగి error;
    }

    FT_TRACE2(( "ps_property_set: missing property `%s'\n",
                property_name ));
    తిరిగి FT_THROW( Missing_Property );
  }


  FT_పునాది_CALLBACK_DEF( FT_Error )
  ps_property_get( FT_Module    module,         /* PS_Driver */
                   స్థిరరాశి అక్షరం*  property_name,
                   శూన్య*        విలువ )
  {
    FT_Error   error  = FT_Err_Ok;
    PS_Driver  driver = (PS_Driver)module;


    ఒకవేళ ( !ft_strcmp( property_name, "darkening-parameters" ) )
    {
      FT_Int*  darken_params = driver->darken_params;
      FT_Int*  val           = (FT_Int*)విలువ;


      val[0] = darken_params[0];
      val[1] = darken_params[1];
      val[2] = darken_params[2];
      val[3] = darken_params[3];
      val[4] = darken_params[4];
      val[5] = darken_params[5];
      val[6] = darken_params[6];
      val[7] = darken_params[7];

      తిరిగి error;
    }

    లేకపోతే ఒకవేళ ( !ft_strcmp( property_name, "hinting-engine" ) )
    {
      FT_UInt   hinting_engine    = driver->hinting_engine;
      FT_UInt*  val               = (FT_UInt*)విలువ;


      *val = hinting_engine;

      తిరిగి error;
    }

    లేకపోతే ఒకవేళ ( !ft_strcmp( property_name, "no-stem-darkening" ) )
    {
      FT_Bool   no_stem_darkening = driver->no_stem_darkening;
      FT_Bool*  val               = (FT_Bool*)విలువ;


      *val = no_stem_darkening;

      తిరిగి error;
    }

    FT_TRACE2(( "ps_property_get: missing property `%s'\n",
                property_name ));
    తిరిగి FT_THROW( Missing_Property );
  }


/*చివర*/
