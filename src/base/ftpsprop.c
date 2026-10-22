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


#include <freetype/ftdriver.h>
#include <freetype/internal/ftdebug.h>
#include <freetype/internal/psaux.h>
#include <freetype/internal/ftobjs.h>
#include <freetype/internal/ftpsprop.h>


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
                   const char*  property_name,
                   const void*  value,
                   FT_Bool      value_is_string )
  {
    FT_Error   error  = FT_Err_Ok;
    PS_Driver  driver = (PS_Driver)module;

#ifndef FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES
    FT_UNUSED( value_is_string );
#endif


    якщо ( !ft_strcmp( property_name, "darkening-parameters" ) )
    {
      FT_Int*  darken_params;
      FT_Int   x1, y1, x2, y2, x3, y3, x4, y4;

#ifdef FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES
      FT_Int   dp[8];


      якщо ( value_is_string )
      {
        const char*  s = (const char*)value;
        char*        ep;
        ціле          i;


        /* eight comma-separated numbers */
        для ( i = 0; i < 7; i++ )
        {
          dp[i] = (FT_Int)ft_strtol( s, &ep, 10 );
          якщо ( *ep != ',' || s == ep )
            повернути FT_THROW( Invalid_Argument );

          s = ep + 1;
        }

        dp[7] = (FT_Int)ft_strtol( s, &ep, 10 );
        якщо ( !( *ep == '\0' || *ep == ' ' ) || s == ep )
          повернути FT_THROW( Invalid_Argument );

        darken_params = dp;
      }
      інакше
#endif
        darken_params = (FT_Int*)value;

      x1 = darken_params[0];
      y1 = darken_params[1];
      x2 = darken_params[2];
      y2 = darken_params[3];
      x3 = darken_params[4];
      y3 = darken_params[5];
      x4 = darken_params[6];
      y4 = darken_params[7];

      якщо ( x1 < 0   || x2 < 0   || x3 < 0   || x4 < 0   ||
           y1 < 0   || y2 < 0   || y3 < 0   || y4 < 0   ||
           x1 > x2  || x2 > x3  || x3 > x4              ||
           y1 > 500 || y2 > 500 || y3 > 500 || y4 > 500 )
        повернути FT_THROW( Invalid_Argument );

      driver->darken_params[0] = x1;
      driver->darken_params[1] = y1;
      driver->darken_params[2] = x2;
      driver->darken_params[3] = y2;
      driver->darken_params[4] = x3;
      driver->darken_params[5] = y3;
      driver->darken_params[6] = x4;
      driver->darken_params[7] = y4;

      повернути error;
    }

    інакше якщо ( !ft_strcmp( property_name, "hinting-engine" ) )
    {
#якщо defined( CFF_CONFIG_OPTION_OLD_ENGINE ) || \
    defined( T1_CONFIG_OPTION_OLD_ENGINE  )
      const char*  module_name = module->clazz->module_name;
#endif


#ifdef FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES
      якщо ( value_is_string )
      {
        const char*  s = (const char*)value;


        якщо ( !ft_strcmp( s, "adobe" ) )
          driver->hinting_engine = FT_HINTING_ADOBE;

#ifdef CFF_CONFIG_OPTION_OLD_ENGINE
        інакше якщо ( !ft_strcmp( module_name, "cff" ) &&
                  !ft_strcmp( s, "freetype" )      )
          driver->hinting_engine = FT_HINTING_FREETYPE;
#endif

#ifdef T1_CONFIG_OPTION_OLD_ENGINE
        інакше якщо ( ( !ft_strcmp( module_name, "type1" ) ||
                    !ft_strcmp( module_name, "t1cid" ) ) &&
                  !ft_strcmp( s, "freetype" )            )
          driver->hinting_engine = FT_HINTING_FREETYPE;
#endif

        інакше
          повернути FT_THROW( Invalid_Argument );
      }
      інакше
#endif /* FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES */
      {
        FT_UInt*  hinting_engine = (FT_UInt*)value;


        якщо ( *hinting_engine == FT_HINTING_ADOBE
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
        інакше
          error = FT_ERR( Unimplemented_Feature );
      }

      повернути error;
    }

    інакше якщо ( !ft_strcmp( property_name, "no-stem-darkening" ) )
    {
#ifdef FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES
      якщо ( value_is_string )
      {
        const char*  s   = (const char*)value;
        long         nsd = ft_strtol( s, NULL, 10 );


        якщо ( !nsd )
          driver->no_stem_darkening = FALSE;
        інакше
          driver->no_stem_darkening = TRUE;
      }
      інакше
#endif
      {
        FT_Bool*  no_stem_darkening = (FT_Bool*)value;


        driver->no_stem_darkening = *no_stem_darkening;
      }

      повернути error;
    }

    інакше якщо ( !ft_strcmp( property_name, "random-seed" ) )
    {
      FT_Int32  random_seed;


#ifdef FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES
      якщо ( value_is_string )
      {
        const char*  s = (const char*)value;


        random_seed = (FT_Int32)ft_strtol( s, NULL, 10 );
      }
      інакше
#endif
        random_seed = *(FT_Int32*)value;

      якщо ( random_seed < 0 )
        random_seed = 0;

      driver->random_seed = random_seed;

      повернути error;
    }

    FT_TRACE2(( "ps_property_set: missing property `%s'\n",
                property_name ));
    повернути FT_THROW( Missing_Property );
  }


  FT_BASE_CALLBACK_DEF( FT_Error )
  ps_property_get( FT_Module    module,         /* PS_Driver */
                   const char*  property_name,
                   void*        value )
  {
    FT_Error   error  = FT_Err_Ok;
    PS_Driver  driver = (PS_Driver)module;


    якщо ( !ft_strcmp( property_name, "darkening-parameters" ) )
    {
      FT_Int*  darken_params = driver->darken_params;
      FT_Int*  val           = (FT_Int*)value;


      val[0] = darken_params[0];
      val[1] = darken_params[1];
      val[2] = darken_params[2];
      val[3] = darken_params[3];
      val[4] = darken_params[4];
      val[5] = darken_params[5];
      val[6] = darken_params[6];
      val[7] = darken_params[7];

      повернути error;
    }

    інакше якщо ( !ft_strcmp( property_name, "hinting-engine" ) )
    {
      FT_UInt   hinting_engine    = driver->hinting_engine;
      FT_UInt*  val               = (FT_UInt*)value;


      *val = hinting_engine;

      повернути error;
    }

    інакше якщо ( !ft_strcmp( property_name, "no-stem-darkening" ) )
    {
      FT_Bool   no_stem_darkening = driver->no_stem_darkening;
      FT_Bool*  val               = (FT_Bool*)value;


      *val = no_stem_darkening;

      повернути error;
    }

    FT_TRACE2(( "ps_property_get: missing property `%s'\n",
                property_name ));
    повернути FT_THROW( Missing_Property );
  }


/* END */
