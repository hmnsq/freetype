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


#અન્તર્ભાવ <freetype/ftdriver.h>
#અન્તર્ભાવ <freetype/internal/ftdebug.h>
#અન્તર્ભાવ <freetype/internal/psaux.h>
#અન્તર્ભાવ <freetype/internal/ftobjs.h>
#અન્તર્ભાવ <freetype/internal/ftpsprop.h>


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
                   અચલ અક્ષર*  property_name,
                   અચલ શૂન્ય*  કિંમત,
                   FT_Bool      કિંમત_is_string )
  {
    FT_Error   error  = FT_Err_Ok;
    PS_Driver  driver = (PS_Driver)module;

#ifndef FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES
    FT_UNUSED( કિંમત_is_string );
#endif


    જો ( !ft_strcmp( property_name, "darkening-parameters" ) )
    {
      FT_Int*  darken_params;
      FT_Int   x1, y1, x2, y2, x3, y3, x4, y4;

#ifdef FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES
      FT_Int   dp[8];


      જો ( કિંમત_is_string )
      {
        અચલ અક્ષર*  s = (અચલ અક્ષર*)કિંમત;
        અક્ષર*        ep;
        પૂર્ણાંક          i;


        /* eight comma-separated numbers */
        માટે ( i = 0; i < 7; i++ )
        {
          dp[i] = (FT_Int)ft_strtol( s, &ep, 10 );
          જો ( *ep != ',' || s == ep )
            પાછા FT_THROW( Invalid_Argument );

          s = ep + 1;
        }

        dp[7] = (FT_Int)ft_strtol( s, &ep, 10 );
        જો ( !( *ep == '\0' || *ep == ' ' ) || s == ep )
          પાછા FT_THROW( Invalid_Argument );

        darken_params = dp;
      }
      નહીંતર
#endif
        darken_params = (FT_Int*)કિંમત;

      x1 = darken_params[0];
      y1 = darken_params[1];
      x2 = darken_params[2];
      y2 = darken_params[3];
      x3 = darken_params[4];
      y3 = darken_params[5];
      x4 = darken_params[6];
      y4 = darken_params[7];

      જો ( x1 < 0   || x2 < 0   || x3 < 0   || x4 < 0   ||
           y1 < 0   || y2 < 0   || y3 < 0   || y4 < 0   ||
           x1 > x2  || x2 > x3  || x3 > x4              ||
           y1 > 500 || y2 > 500 || y3 > 500 || y4 > 500 )
        પાછા FT_THROW( Invalid_Argument );

      driver->darken_params[0] = x1;
      driver->darken_params[1] = y1;
      driver->darken_params[2] = x2;
      driver->darken_params[3] = y2;
      driver->darken_params[4] = x3;
      driver->darken_params[5] = y3;
      driver->darken_params[6] = x4;
      driver->darken_params[7] = y4;

      પાછા error;
    }

    નહીંતર જો ( !ft_strcmp( property_name, "hinting-engine" ) )
    {
#જો defined( CFF_CONFIG_OPTION_OLD_ENGINE ) || \
    defined( T1_CONFIG_OPTION_OLD_ENGINE  )
      અચલ અક્ષર*  module_name = module->clazz->module_name;
#endif


#ifdef FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES
      જો ( કિંમત_is_string )
      {
        અચલ અક્ષર*  s = (અચલ અક્ષર*)કિંમત;


        જો ( !ft_strcmp( s, "adobe" ) )
          driver->hinting_engine = FT_HINTING_ADOBE;

#ifdef CFF_CONFIG_OPTION_OLD_ENGINE
        નહીંતર જો ( !ft_strcmp( module_name, "cff" ) &&
                  !ft_strcmp( s, "freetype" )      )
          driver->hinting_engine = FT_HINTING_FREETYPE;
#endif

#ifdef T1_CONFIG_OPTION_OLD_ENGINE
        નહીંતર જો ( ( !ft_strcmp( module_name, "type1" ) ||
                    !ft_strcmp( module_name, "t1cid" ) ) &&
                  !ft_strcmp( s, "freetype" )            )
          driver->hinting_engine = FT_HINTING_FREETYPE;
#endif

        નહીંતર
          પાછા FT_THROW( Invalid_Argument );
      }
      નહીંતર
#endif /* FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES */
      {
        FT_UInt*  hinting_engine = (FT_UInt*)કિંમત;


        જો ( *hinting_engine == FT_HINTING_ADOBE
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
        નહીંતર
          error = FT_ERR( Unimplemented_Feature );
      }

      પાછા error;
    }

    નહીંતર જો ( !ft_strcmp( property_name, "no-stem-darkening" ) )
    {
#ifdef FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES
      જો ( કિંમત_is_string )
      {
        અચલ અક્ષર*  s   = (અચલ અક્ષર*)કિંમત;
        long         nsd = ft_strtol( s, ખાલી, 10 );


        જો ( !nsd )
          driver->no_stem_darkening = FALSE;
        નહીંતર
          driver->no_stem_darkening = TRUE;
      }
      નહીંતર
#endif
      {
        FT_Bool*  no_stem_darkening = (FT_Bool*)કિંમત;


        driver->no_stem_darkening = *no_stem_darkening;
      }

      પાછા error;
    }

    નહીંતર જો ( !ft_strcmp( property_name, "random-seed" ) )
    {
      FT_Int32  random_seed;


#ifdef FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES
      જો ( કિંમત_is_string )
      {
        અચલ અક્ષર*  s = (અચલ અક્ષર*)કિંમત;


        random_seed = (FT_Int32)ft_strtol( s, ખાલી, 10 );
      }
      નહીંતર
#endif
        random_seed = *(FT_Int32*)કિંમત;

      જો ( random_seed < 0 )
        random_seed = 0;

      driver->random_seed = random_seed;

      પાછા error;
    }

    FT_TRACE2(( "ps_property_set: missing property `%s'\n",
                property_name ));
    પાછા FT_THROW( Missing_Property );
  }


  FT_BASE_CALLBACK_DEF( FT_Error )
  ps_property_get( FT_Module    module,         /* PS_Driver */
                   અચલ અક્ષર*  property_name,
                   શૂન્ય*        કિંમત )
  {
    FT_Error   error  = FT_Err_Ok;
    PS_Driver  driver = (PS_Driver)module;


    જો ( !ft_strcmp( property_name, "darkening-parameters" ) )
    {
      FT_Int*  darken_params = driver->darken_params;
      FT_Int*  val           = (FT_Int*)કિંમત;


      val[0] = darken_params[0];
      val[1] = darken_params[1];
      val[2] = darken_params[2];
      val[3] = darken_params[3];
      val[4] = darken_params[4];
      val[5] = darken_params[5];
      val[6] = darken_params[6];
      val[7] = darken_params[7];

      પાછા error;
    }

    નહીંતર જો ( !ft_strcmp( property_name, "hinting-engine" ) )
    {
      FT_UInt   hinting_engine    = driver->hinting_engine;
      FT_UInt*  val               = (FT_UInt*)કિંમત;


      *val = hinting_engine;

      પાછા error;
    }

    નહીંતર જો ( !ft_strcmp( property_name, "no-stem-darkening" ) )
    {
      FT_Bool   no_stem_darkening = driver->no_stem_darkening;
      FT_Bool*  val               = (FT_Bool*)કિંમત;


      *val = no_stem_darkening;

      પાછા error;
    }

    FT_TRACE2(( "ps_property_get: missing property `%s'\n",
                property_name ));
    પાછા FT_THROW( Missing_Property );
  }


/* END */
