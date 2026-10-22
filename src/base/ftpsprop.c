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


#شامل <freetype/ftdriver.h>
#شامل <freetype/internal/ftdebug.h>
#شامل <freetype/internal/psaux.h>
#شامل <freetype/internal/ftobjs.h>
#شامل <freetype/internal/ftpsprop.h>


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#تعریف_منسوخ  FT_COMPONENT
#تعریف_کرو FT_COMPONENT  psprops


  FT_بنیاد_CALLBACK_DEF( FT_Error )
  ps_property_سیٹ( FT_Module    module,         /* PS_Driver */
                   ثابت حرف*  property_name,
                   ثابت خالی*  قدر,
                   FT_Bool      قدر_is_string )
  {
    FT_Error   error  = FT_Err_Ok;
    PS_Driver  driver = (PS_Driver)module;

#اگر_غیر_تعریف_شدہ FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES
    FT_غیر_مستعمل( قدر_is_string );
#اگر_ختم


    اگر ( !ft_strcmp( property_name, "darkening-parameters" ) )
    {
      FT_Int*  darken_params;
      FT_Int   x1, y1, x2, y2, x3, y3, x4, y4;

#اگر_تعریف_شدہ FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES
      FT_Int   dp[8];


      اگر ( قدر_is_string )
      {
        ثابت حرف*  s = (ثابت حرف*)قدر;
        حرف*        ep;
        صحیح          i;


        /* eight comma-separated numbers */
        برائے ( i = 0; i < 7; i++ )
        {
          dp[i] = (FT_Int)ft_strtol( s, &ep, 10 );
          اگر ( *ep != ',' || s == ep )
            واپس FT_THROW( Invalid_دلیل );

          s = ep + 1;
        }

        dp[7] = (FT_Int)ft_strtol( s, &ep, 10 );
        اگر ( !( *ep == '\0' || *ep == ' ' ) || s == ep )
          واپس FT_THROW( Invalid_دلیل );

        darken_params = dp;
      }
      ورنہ
#اگر_ختم
        darken_params = (FT_Int*)قدر;

      x1 = darken_params[0];
      y1 = darken_params[1];
      x2 = darken_params[2];
      y2 = darken_params[3];
      x3 = darken_params[4];
      y3 = darken_params[5];
      x4 = darken_params[6];
      y4 = darken_params[7];

      اگر ( x1 < 0   || x2 < 0   || x3 < 0   || x4 < 0   ||
           y1 < 0   || y2 < 0   || y3 < 0   || y4 < 0   ||
           x1 > x2  || x2 > x3  || x3 > x4              ||
           y1 > 500 || y2 > 500 || y3 > 500 || y4 > 500 )
        واپس FT_THROW( Invalid_دلیل );

      driver->darken_params[0] = x1;
      driver->darken_params[1] = y1;
      driver->darken_params[2] = x2;
      driver->darken_params[3] = y2;
      driver->darken_params[4] = x3;
      driver->darken_params[5] = y3;
      driver->darken_params[6] = x4;
      driver->darken_params[7] = y4;

      واپس error;
    }

    ورنہ اگر ( !ft_strcmp( property_name, "hinting-engine" ) )
    {
#اگر defined( CFF_CONFIG_OPTION_OLD_ENGINE ) || \
    defined( T1_CONFIG_OPTION_OLD_ENGINE  )
      ثابت حرف*  module_name = module->clazz->module_name;
#اگر_ختم


#اگر_تعریف_شدہ FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES
      اگر ( قدر_is_string )
      {
        ثابت حرف*  s = (ثابت حرف*)قدر;


        اگر ( !ft_strcmp( s, "adobe" ) )
          driver->hinting_engine = FT_HINTING_ADOBE;

#اگر_تعریف_شدہ CFF_CONFIG_OPTION_OLD_ENGINE
        ورنہ اگر ( !ft_strcmp( module_name, "cff" ) &&
                  !ft_strcmp( s, "freetype" )      )
          driver->hinting_engine = FT_HINTING_FREETYPE;
#اگر_ختم

#اگر_تعریف_شدہ T1_CONFIG_OPTION_OLD_ENGINE
        ورنہ اگر ( ( !ft_strcmp( module_name, "type1" ) ||
                    !ft_strcmp( module_name, "t1cid" ) ) &&
                  !ft_strcmp( s, "freetype" )            )
          driver->hinting_engine = FT_HINTING_FREETYPE;
#اگر_ختم

        ورنہ
          واپس FT_THROW( Invalid_دلیل );
      }
      ورنہ
#اگر_ختم /* FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES */
      {
        FT_UInt*  hinting_engine = (FT_UInt*)قدر;


        اگر ( *hinting_engine == FT_HINTING_ADOBE
#اگر_تعریف_شدہ CFF_CONFIG_OPTION_OLD_ENGINE
             || ( *hinting_engine == FT_HINTING_FREETYPE &&
                  !ft_strcmp( module_name, "cff" )       )
#اگر_ختم
#اگر_تعریف_شدہ T1_CONFIG_OPTION_OLD_ENGINE
             || ( *hinting_engine == FT_HINTING_FREETYPE &&
                  ( !ft_strcmp( module_name, "type1" ) ||
                    !ft_strcmp( module_name, "t1cid" ) ) )
#اگر_ختم
           )
          driver->hinting_engine = *hinting_engine;
        ورنہ
          error = FT_ERR( Unimplemented_Feature );
      }

      واپس error;
    }

    ورنہ اگر ( !ft_strcmp( property_name, "no-stem-darkening" ) )
    {
#اگر_تعریف_شدہ FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES
      اگر ( قدر_is_string )
      {
        ثابت حرف*  s   = (ثابت حرف*)قدر;
        طویل         nsd = ft_strtol( s, تہی, 10 );


        اگر ( !nsd )
          driver->no_stem_darkening = FALSE;
        ورنہ
          driver->no_stem_darkening = TRUE;
      }
      ورنہ
#اگر_ختم
      {
        FT_Bool*  no_stem_darkening = (FT_Bool*)قدر;


        driver->no_stem_darkening = *no_stem_darkening;
      }

      واپس error;
    }

    ورنہ اگر ( !ft_strcmp( property_name, "random-seed" ) )
    {
      FT_Int32  random_بیج;


#اگر_تعریف_شدہ FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES
      اگر ( قدر_is_string )
      {
        ثابت حرف*  s = (ثابت حرف*)قدر;


        random_بیج = (FT_Int32)ft_strtol( s, تہی, 10 );
      }
      ورنہ
#اگر_ختم
        random_بیج = *(FT_Int32*)قدر;

      اگر ( random_بیج < 0 )
        random_بیج = 0;

      driver->random_بیج = random_بیج;

      واپس error;
    }

    FT_TRACE2(( "ps_property_set: missing property `%s'\n",
                property_name ));
    واپس FT_THROW( Missing_Property );
  }


  FT_بنیاد_CALLBACK_DEF( FT_Error )
  ps_property_get( FT_Module    module,         /* PS_Driver */
                   ثابت حرف*  property_name,
                   خالی*        قدر )
  {
    FT_Error   error  = FT_Err_Ok;
    PS_Driver  driver = (PS_Driver)module;


    اگر ( !ft_strcmp( property_name, "darkening-parameters" ) )
    {
      FT_Int*  darken_params = driver->darken_params;
      FT_Int*  val           = (FT_Int*)قدر;


      val[0] = darken_params[0];
      val[1] = darken_params[1];
      val[2] = darken_params[2];
      val[3] = darken_params[3];
      val[4] = darken_params[4];
      val[5] = darken_params[5];
      val[6] = darken_params[6];
      val[7] = darken_params[7];

      واپس error;
    }

    ورنہ اگر ( !ft_strcmp( property_name, "hinting-engine" ) )
    {
      FT_UInt   hinting_engine    = driver->hinting_engine;
      FT_UInt*  val               = (FT_UInt*)قدر;


      *val = hinting_engine;

      واپس error;
    }

    ورنہ اگر ( !ft_strcmp( property_name, "no-stem-darkening" ) )
    {
      FT_Bool   no_stem_darkening = driver->no_stem_darkening;
      FT_Bool*  val               = (FT_Bool*)قدر;


      *val = no_stem_darkening;

      واپس error;
    }

    FT_TRACE2(( "ps_property_get: missing property `%s'\n",
                property_name ));
    واپس FT_THROW( Missing_Property );
  }


/*انتہا*/
