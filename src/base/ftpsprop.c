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


#ଅନ୍ତର୍ଭାଵ <freetype/ftdriver.h>
#ଅନ୍ତର୍ଭାଵ <freetype/internal/ftdebug.h>
#ଅନ୍ତର୍ଭାଵ <freetype/internal/psaux.h>
#ଅନ୍ତର୍ଭାଵ <freetype/internal/ftobjs.h>
#ଅନ୍ତର୍ଭାଵ <freetype/internal/ftpsprop.h>


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
                   ଅଚଳ ଅକ୍ଷର*  property_name,
                   ଅଚଳ ଶୂନ୍ୟ*  ମୂଲ୍ୟ,
                   FT_Bool      ମୂଲ୍ୟ_is_string )
  {
    FT_Error   error  = FT_Err_Ok;
    PS_Driver  driver = (PS_Driver)module;

#ifndef FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES
    FT_UNUSED( ମୂଲ୍ୟ_is_string );
#endif


    ଯଦି ( !ft_strcmp( property_name, "darkening-parameters" ) )
    {
      FT_Int*  darken_params;
      FT_Int   x1, y1, x2, y2, x3, y3, x4, y4;

#ifdef FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES
      FT_Int   dp[8];


      ଯଦି ( ମୂଲ୍ୟ_is_string )
      {
        ଅଚଳ ଅକ୍ଷର*  s = (ଅଚଳ ଅକ୍ଷର*)ମୂଲ୍ୟ;
        ଅକ୍ଷର*        ep;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା          i;


        /* eight comma-separated numbers */
        ପାଇଁ ( i = 0; i < 7; i++ )
        {
          dp[i] = (FT_Int)ft_strtol( s, &ep, 10 );
          ଯଦି ( *ep != ',' || s == ep )
            ଫେରନ୍ତୁ FT_THROW( Invalid_Argument );

          s = ep + 1;
        }

        dp[7] = (FT_Int)ft_strtol( s, &ep, 10 );
        ଯଦି ( !( *ep == '\0' || *ep == ' ' ) || s == ep )
          ଫେରନ୍ତୁ FT_THROW( Invalid_Argument );

        darken_params = dp;
      }
      ଅନ୍ୟଥା
#endif
        darken_params = (FT_Int*)ମୂଲ୍ୟ;

      x1 = darken_params[0];
      y1 = darken_params[1];
      x2 = darken_params[2];
      y2 = darken_params[3];
      x3 = darken_params[4];
      y3 = darken_params[5];
      x4 = darken_params[6];
      y4 = darken_params[7];

      ଯଦି ( x1 < 0   || x2 < 0   || x3 < 0   || x4 < 0   ||
           y1 < 0   || y2 < 0   || y3 < 0   || y4 < 0   ||
           x1 > x2  || x2 > x3  || x3 > x4              ||
           y1 > 500 || y2 > 500 || y3 > 500 || y4 > 500 )
        ଫେରନ୍ତୁ FT_THROW( Invalid_Argument );

      driver->darken_params[0] = x1;
      driver->darken_params[1] = y1;
      driver->darken_params[2] = x2;
      driver->darken_params[3] = y2;
      driver->darken_params[4] = x3;
      driver->darken_params[5] = y3;
      driver->darken_params[6] = x4;
      driver->darken_params[7] = y4;

      ଫେରନ୍ତୁ error;
    }

    ଅନ୍ୟଥା ଯଦି ( !ft_strcmp( property_name, "hinting-engine" ) )
    {
#ଯଦି defined( CFF_CONFIG_OPTION_OLD_ENGINE ) || \
    defined( T1_CONFIG_OPTION_OLD_ENGINE  )
      ଅଚଳ ଅକ୍ଷର*  module_name = module->clazz->module_name;
#endif


#ifdef FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES
      ଯଦି ( ମୂଲ୍ୟ_is_string )
      {
        ଅଚଳ ଅକ୍ଷର*  s = (ଅଚଳ ଅକ୍ଷର*)ମୂଲ୍ୟ;


        ଯଦି ( !ft_strcmp( s, "adobe" ) )
          driver->hinting_engine = FT_HINTING_ADOBE;

#ifdef CFF_CONFIG_OPTION_OLD_ENGINE
        ଅନ୍ୟଥା ଯଦି ( !ft_strcmp( module_name, "cff" ) &&
                  !ft_strcmp( s, "freetype" )      )
          driver->hinting_engine = FT_HINTING_FREETYPE;
#endif

#ifdef T1_CONFIG_OPTION_OLD_ENGINE
        ଅନ୍ୟଥା ଯଦି ( ( !ft_strcmp( module_name, "type1" ) ||
                    !ft_strcmp( module_name, "t1cid" ) ) &&
                  !ft_strcmp( s, "freetype" )            )
          driver->hinting_engine = FT_HINTING_FREETYPE;
#endif

        ଅନ୍ୟଥା
          ଫେରନ୍ତୁ FT_THROW( Invalid_Argument );
      }
      ଅନ୍ୟଥା
#endif /* FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES */
      {
        FT_UInt*  hinting_engine = (FT_UInt*)ମୂଲ୍ୟ;


        ଯଦି ( *hinting_engine == FT_HINTING_ADOBE
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
        ଅନ୍ୟଥା
          error = FT_ERR( Unimplemented_Feature );
      }

      ଫେରନ୍ତୁ error;
    }

    ଅନ୍ୟଥା ଯଦି ( !ft_strcmp( property_name, "no-stem-darkening" ) )
    {
#ifdef FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES
      ଯଦି ( ମୂଲ୍ୟ_is_string )
      {
        ଅଚଳ ଅକ୍ଷର*  s   = (ଅଚଳ ଅକ୍ଷର*)ମୂଲ୍ୟ;
        long         nsd = ft_strtol( s, ଖାଲି, 10 );


        ଯଦି ( !nsd )
          driver->no_stem_darkening = FALSE;
        ଅନ୍ୟଥା
          driver->no_stem_darkening = TRUE;
      }
      ଅନ୍ୟଥା
#endif
      {
        FT_Bool*  no_stem_darkening = (FT_Bool*)ମୂଲ୍ୟ;


        driver->no_stem_darkening = *no_stem_darkening;
      }

      ଫେରନ୍ତୁ error;
    }

    ଅନ୍ୟଥା ଯଦି ( !ft_strcmp( property_name, "random-seed" ) )
    {
      FT_Int32  random_seed;


#ifdef FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES
      ଯଦି ( ମୂଲ୍ୟ_is_string )
      {
        ଅଚଳ ଅକ୍ଷର*  s = (ଅଚଳ ଅକ୍ଷର*)ମୂଲ୍ୟ;


        random_seed = (FT_Int32)ft_strtol( s, ଖାଲି, 10 );
      }
      ଅନ୍ୟଥା
#endif
        random_seed = *(FT_Int32*)ମୂଲ୍ୟ;

      ଯଦି ( random_seed < 0 )
        random_seed = 0;

      driver->random_seed = random_seed;

      ଫେରନ୍ତୁ error;
    }

    FT_TRACE2(( "ps_property_set: missing property `%s'\n",
                property_name ));
    ଫେରନ୍ତୁ FT_THROW( Missing_Property );
  }


  FT_BASE_CALLBACK_DEF( FT_Error )
  ps_property_get( FT_Module    module,         /* PS_Driver */
                   ଅଚଳ ଅକ୍ଷର*  property_name,
                   ଶୂନ୍ୟ*        ମୂଲ୍ୟ )
  {
    FT_Error   error  = FT_Err_Ok;
    PS_Driver  driver = (PS_Driver)module;


    ଯଦି ( !ft_strcmp( property_name, "darkening-parameters" ) )
    {
      FT_Int*  darken_params = driver->darken_params;
      FT_Int*  val           = (FT_Int*)ମୂଲ୍ୟ;


      val[0] = darken_params[0];
      val[1] = darken_params[1];
      val[2] = darken_params[2];
      val[3] = darken_params[3];
      val[4] = darken_params[4];
      val[5] = darken_params[5];
      val[6] = darken_params[6];
      val[7] = darken_params[7];

      ଫେରନ୍ତୁ error;
    }

    ଅନ୍ୟଥା ଯଦି ( !ft_strcmp( property_name, "hinting-engine" ) )
    {
      FT_UInt   hinting_engine    = driver->hinting_engine;
      FT_UInt*  val               = (FT_UInt*)ମୂଲ୍ୟ;


      *val = hinting_engine;

      ଫେରନ୍ତୁ error;
    }

    ଅନ୍ୟଥା ଯଦି ( !ft_strcmp( property_name, "no-stem-darkening" ) )
    {
      FT_Bool   no_stem_darkening = driver->no_stem_darkening;
      FT_Bool*  val               = (FT_Bool*)ମୂଲ୍ୟ;


      *val = no_stem_darkening;

      ଫେରନ୍ତୁ error;
    }

    FT_TRACE2(( "ps_property_get: missing property `%s'\n",
                property_name ));
    ଫେରନ୍ତୁ FT_THROW( Missing_Property );
  }


/* END */
