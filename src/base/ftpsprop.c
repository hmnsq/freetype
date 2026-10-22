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


#അന്തര്ഭാവ <freetype/ftdriver.h>
#അന്തര്ഭാവ <freetype/internal/ftdebug.h>
#അന്തര്ഭാവ <freetype/internal/psaux.h>
#അന്തര്ഭാവ <freetype/internal/ftobjs.h>
#അന്തര്ഭാവ <freetype/internal/ftpsprop.h>


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
                   സ്ഥിരം അക്ഷരം*  property_name,
                   സ്ഥിരം ശൂന്യം*  മൂല്യം,
                   FT_Bool      മൂല്യം_is_string )
  {
    FT_Error   error  = FT_Err_Ok;
    PS_Driver  driver = (PS_Driver)module;

#ifndef FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES
    FT_UNUSED( മൂല്യം_is_string );
#endif


    എങ്കിൽ ( !ft_strcmp( property_name, "darkening-parameters" ) )
    {
      FT_Int*  darken_params;
      FT_Int   x1, y1, x2, y2, x3, y3, x4, y4;

#ifdef FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES
      FT_Int   dp[8];


      എങ്കിൽ ( മൂല്യം_is_string )
      {
        സ്ഥിരം അക്ഷരം*  s = (സ്ഥിരം അക്ഷരം*)മൂല്യം;
        അക്ഷരം*        ep;
        പൂർണ്ണസംഖ്യ          i;


        /* eight comma-separated numbers */
        വേണ്ടി ( i = 0; i < 7; i++ )
        {
          dp[i] = (FT_Int)ft_strtol( s, &ep, 10 );
          എങ്കിൽ ( *ep != ',' || s == ep )
            തിരികെ FT_THROW( Invalid_Argument );

          s = ep + 1;
        }

        dp[7] = (FT_Int)ft_strtol( s, &ep, 10 );
        എങ്കിൽ ( !( *ep == '\0' || *ep == ' ' ) || s == ep )
          തിരികെ FT_THROW( Invalid_Argument );

        darken_params = dp;
      }
      അല്ലെങ്കിൽ
#endif
        darken_params = (FT_Int*)മൂല്യം;

      x1 = darken_params[0];
      y1 = darken_params[1];
      x2 = darken_params[2];
      y2 = darken_params[3];
      x3 = darken_params[4];
      y3 = darken_params[5];
      x4 = darken_params[6];
      y4 = darken_params[7];

      എങ്കിൽ ( x1 < 0   || x2 < 0   || x3 < 0   || x4 < 0   ||
           y1 < 0   || y2 < 0   || y3 < 0   || y4 < 0   ||
           x1 > x2  || x2 > x3  || x3 > x4              ||
           y1 > 500 || y2 > 500 || y3 > 500 || y4 > 500 )
        തിരികെ FT_THROW( Invalid_Argument );

      driver->darken_params[0] = x1;
      driver->darken_params[1] = y1;
      driver->darken_params[2] = x2;
      driver->darken_params[3] = y2;
      driver->darken_params[4] = x3;
      driver->darken_params[5] = y3;
      driver->darken_params[6] = x4;
      driver->darken_params[7] = y4;

      തിരികെ error;
    }

    അല്ലെങ്കിൽ എങ്കിൽ ( !ft_strcmp( property_name, "hinting-engine" ) )
    {
#എങ്കിൽ defined( CFF_CONFIG_OPTION_OLD_ENGINE ) || \
    defined( T1_CONFIG_OPTION_OLD_ENGINE  )
      സ്ഥിരം അക്ഷരം*  module_name = module->clazz->module_name;
#endif


#ifdef FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES
      എങ്കിൽ ( മൂല്യം_is_string )
      {
        സ്ഥിരം അക്ഷരം*  s = (സ്ഥിരം അക്ഷരം*)മൂല്യം;


        എങ്കിൽ ( !ft_strcmp( s, "adobe" ) )
          driver->hinting_engine = FT_HINTING_ADOBE;

#ifdef CFF_CONFIG_OPTION_OLD_ENGINE
        അല്ലെങ്കിൽ എങ്കിൽ ( !ft_strcmp( module_name, "cff" ) &&
                  !ft_strcmp( s, "freetype" )      )
          driver->hinting_engine = FT_HINTING_FREETYPE;
#endif

#ifdef T1_CONFIG_OPTION_OLD_ENGINE
        അല്ലെങ്കിൽ എങ്കിൽ ( ( !ft_strcmp( module_name, "type1" ) ||
                    !ft_strcmp( module_name, "t1cid" ) ) &&
                  !ft_strcmp( s, "freetype" )            )
          driver->hinting_engine = FT_HINTING_FREETYPE;
#endif

        അല്ലെങ്കിൽ
          തിരികെ FT_THROW( Invalid_Argument );
      }
      അല്ലെങ്കിൽ
#endif /* FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES */
      {
        FT_UInt*  hinting_engine = (FT_UInt*)മൂല്യം;


        എങ്കിൽ ( *hinting_engine == FT_HINTING_ADOBE
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
        അല്ലെങ്കിൽ
          error = FT_ERR( Unimplemented_Feature );
      }

      തിരികെ error;
    }

    അല്ലെങ്കിൽ എങ്കിൽ ( !ft_strcmp( property_name, "no-stem-darkening" ) )
    {
#ifdef FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES
      എങ്കിൽ ( മൂല്യം_is_string )
      {
        സ്ഥിരം അക്ഷരം*  s   = (സ്ഥിരം അക്ഷരം*)മൂല്യം;
        long         nsd = ft_strtol( s, ശൂന്യ, 10 );


        എങ്കിൽ ( !nsd )
          driver->no_stem_darkening = FALSE;
        അല്ലെങ്കിൽ
          driver->no_stem_darkening = TRUE;
      }
      അല്ലെങ്കിൽ
#endif
      {
        FT_Bool*  no_stem_darkening = (FT_Bool*)മൂല്യം;


        driver->no_stem_darkening = *no_stem_darkening;
      }

      തിരികെ error;
    }

    അല്ലെങ്കിൽ എങ്കിൽ ( !ft_strcmp( property_name, "random-seed" ) )
    {
      FT_Int32  random_seed;


#ifdef FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES
      എങ്കിൽ ( മൂല്യം_is_string )
      {
        സ്ഥിരം അക്ഷരം*  s = (സ്ഥിരം അക്ഷരം*)മൂല്യം;


        random_seed = (FT_Int32)ft_strtol( s, ശൂന്യ, 10 );
      }
      അല്ലെങ്കിൽ
#endif
        random_seed = *(FT_Int32*)മൂല്യം;

      എങ്കിൽ ( random_seed < 0 )
        random_seed = 0;

      driver->random_seed = random_seed;

      തിരികെ error;
    }

    FT_TRACE2(( "ps_property_set: missing property `%s'\n",
                property_name ));
    തിരികെ FT_THROW( Missing_Property );
  }


  FT_BASE_CALLBACK_DEF( FT_Error )
  ps_property_get( FT_Module    module,         /* PS_Driver */
                   സ്ഥിരം അക്ഷരം*  property_name,
                   ശൂന്യം*        മൂല്യം )
  {
    FT_Error   error  = FT_Err_Ok;
    PS_Driver  driver = (PS_Driver)module;


    എങ്കിൽ ( !ft_strcmp( property_name, "darkening-parameters" ) )
    {
      FT_Int*  darken_params = driver->darken_params;
      FT_Int*  val           = (FT_Int*)മൂല്യം;


      val[0] = darken_params[0];
      val[1] = darken_params[1];
      val[2] = darken_params[2];
      val[3] = darken_params[3];
      val[4] = darken_params[4];
      val[5] = darken_params[5];
      val[6] = darken_params[6];
      val[7] = darken_params[7];

      തിരികെ error;
    }

    അല്ലെങ്കിൽ എങ്കിൽ ( !ft_strcmp( property_name, "hinting-engine" ) )
    {
      FT_UInt   hinting_engine    = driver->hinting_engine;
      FT_UInt*  val               = (FT_UInt*)മൂല്യം;


      *val = hinting_engine;

      തിരികെ error;
    }

    അല്ലെങ്കിൽ എങ്കിൽ ( !ft_strcmp( property_name, "no-stem-darkening" ) )
    {
      FT_Bool   no_stem_darkening = driver->no_stem_darkening;
      FT_Bool*  val               = (FT_Bool*)മൂല്യം;


      *val = no_stem_darkening;

      തിരികെ error;
    }

    FT_TRACE2(( "ps_property_get: missing property `%s'\n",
                property_name ));
    തിരികെ FT_THROW( Missing_Property );
  }


/* END */
