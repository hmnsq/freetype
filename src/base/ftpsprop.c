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


#समावेश <freetype/ftdriver.h>
#समावेश <freetype/internal/ftdebug.h>
#समावेश <freetype/internal/psaux.h>
#समावेश <freetype/internal/ftobjs.h>
#समावेश <freetype/internal/ftpsprop.h>


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#अघोषित  FT_COMPONENT
#घोषणा FT_COMPONENT  psprops


  FT_BASE_CALLBACK_DEF( FT_Error )
  ps_property_set( FT_Module    module,         /* PS_Driver */
                   स्थिर अक्षर*  property_name,
                   स्थिर व्योम*  मूल्य,
                   FT_Bool      मूल्य_is_string )
  {
    FT_Error   error  = FT_Err_Ok;
    PS_Driver  driver = (PS_Driver)module;

#अगर_अघोषित FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES
    FT_UNUSED( मूल्य_is_string );
#पूर्ण_अगर


    अगर ( !ft_strcmp( property_name, "darkening-parameters" ) )
    {
      FT_Int*  darken_params;
      FT_Int   x1, y1, x2, y2, x3, y3, x4, y4;

#अगर_घोषित FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES
      FT_Int   dp[8];


      अगर ( मूल्य_is_string )
      {
        स्थिर अक्षर*  s = (स्थिर अक्षर*)मूल्य;
        अक्षर*        ep;
        पूर्णांक          i;


        /* eight comma-separated numbers */
        क्रम ( i = 0; i < 7; i++ )
        {
          dp[i] = (FT_Int)ft_strtol( s, &ep, 10 );
          अगर ( *ep != ',' || s == ep )
            वापस FT_THROW( Invalid_Argument );

          s = ep + 1;
        }

        dp[7] = (FT_Int)ft_strtol( s, &ep, 10 );
        अगर ( !( *ep == '\0' || *ep == ' ' ) || s == ep )
          वापस FT_THROW( Invalid_Argument );

        darken_params = dp;
      }
      अन्यथा
#पूर्ण_अगर
        darken_params = (FT_Int*)मूल्य;

      x1 = darken_params[0];
      y1 = darken_params[1];
      x2 = darken_params[2];
      y2 = darken_params[3];
      x3 = darken_params[4];
      y3 = darken_params[5];
      x4 = darken_params[6];
      y4 = darken_params[7];

      अगर ( x1 < 0   || x2 < 0   || x3 < 0   || x4 < 0   ||
           y1 < 0   || y2 < 0   || y3 < 0   || y4 < 0   ||
           x1 > x2  || x2 > x3  || x3 > x4              ||
           y1 > 500 || y2 > 500 || y3 > 500 || y4 > 500 )
        वापस FT_THROW( Invalid_Argument );

      driver->darken_params[0] = x1;
      driver->darken_params[1] = y1;
      driver->darken_params[2] = x2;
      driver->darken_params[3] = y2;
      driver->darken_params[4] = x3;
      driver->darken_params[5] = y3;
      driver->darken_params[6] = x4;
      driver->darken_params[7] = y4;

      वापस error;
    }

    अन्यथा अगर ( !ft_strcmp( property_name, "hinting-engine" ) )
    {
#अगर defined( CFF_CONFIG_OPTION_OLD_ENGINE ) || \
    defined( T1_CONFIG_OPTION_OLD_ENGINE  )
      स्थिर अक्षर*  module_name = module->clazz->module_name;
#पूर्ण_अगर


#अगर_घोषित FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES
      अगर ( मूल्य_is_string )
      {
        स्थिर अक्षर*  s = (स्थिर अक्षर*)मूल्य;


        अगर ( !ft_strcmp( s, "adobe" ) )
          driver->hinting_engine = FT_HINTING_ADOBE;

#अगर_घोषित CFF_CONFIG_OPTION_OLD_ENGINE
        अन्यथा अगर ( !ft_strcmp( module_name, "cff" ) &&
                  !ft_strcmp( s, "freetype" )      )
          driver->hinting_engine = FT_HINTING_FREETYPE;
#पूर्ण_अगर

#अगर_घोषित T1_CONFIG_OPTION_OLD_ENGINE
        अन्यथा अगर ( ( !ft_strcmp( module_name, "type1" ) ||
                    !ft_strcmp( module_name, "t1cid" ) ) &&
                  !ft_strcmp( s, "freetype" )            )
          driver->hinting_engine = FT_HINTING_FREETYPE;
#पूर्ण_अगर

        अन्यथा
          वापस FT_THROW( Invalid_Argument );
      }
      अन्यथा
#पूर्ण_अगर /* FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES */
      {
        FT_UInt*  hinting_engine = (FT_UInt*)मूल्य;


        अगर ( *hinting_engine == FT_HINTING_ADOBE
#अगर_घोषित CFF_CONFIG_OPTION_OLD_ENGINE
             || ( *hinting_engine == FT_HINTING_FREETYPE &&
                  !ft_strcmp( module_name, "cff" )       )
#पूर्ण_अगर
#अगर_घोषित T1_CONFIG_OPTION_OLD_ENGINE
             || ( *hinting_engine == FT_HINTING_FREETYPE &&
                  ( !ft_strcmp( module_name, "type1" ) ||
                    !ft_strcmp( module_name, "t1cid" ) ) )
#पूर्ण_अगर
           )
          driver->hinting_engine = *hinting_engine;
        अन्यथा
          error = FT_ERR( Unimplemented_Feature );
      }

      वापस error;
    }

    अन्यथा अगर ( !ft_strcmp( property_name, "no-stem-darkening" ) )
    {
#अगर_घोषित FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES
      अगर ( मूल्य_is_string )
      {
        स्थिर अक्षर*  s   = (स्थिर अक्षर*)मूल्य;
        दीर्घ         nsd = ft_strtol( s, शून्य, 10 );


        अगर ( !nsd )
          driver->no_stem_darkening = FALSE;
        अन्यथा
          driver->no_stem_darkening = TRUE;
      }
      अन्यथा
#पूर्ण_अगर
      {
        FT_Bool*  no_stem_darkening = (FT_Bool*)मूल्य;


        driver->no_stem_darkening = *no_stem_darkening;
      }

      वापस error;
    }

    अन्यथा अगर ( !ft_strcmp( property_name, "random-seed" ) )
    {
      FT_Int32  random_seed;


#अगर_घोषित FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES
      अगर ( मूल्य_is_string )
      {
        स्थिर अक्षर*  s = (स्थिर अक्षर*)मूल्य;


        random_seed = (FT_Int32)ft_strtol( s, शून्य, 10 );
      }
      अन्यथा
#पूर्ण_अगर
        random_seed = *(FT_Int32*)मूल्य;

      अगर ( random_seed < 0 )
        random_seed = 0;

      driver->random_seed = random_seed;

      वापस error;
    }

    FT_TRACE2(( "ps_property_set: missing property `%s'\n",
                property_name ));
    वापस FT_THROW( Missing_Property );
  }


  FT_BASE_CALLBACK_DEF( FT_Error )
  ps_property_get( FT_Module    module,         /* PS_Driver */
                   स्थिर अक्षर*  property_name,
                   व्योम*        मूल्य )
  {
    FT_Error   error  = FT_Err_Ok;
    PS_Driver  driver = (PS_Driver)module;


    अगर ( !ft_strcmp( property_name, "darkening-parameters" ) )
    {
      FT_Int*  darken_params = driver->darken_params;
      FT_Int*  val           = (FT_Int*)मूल्य;


      val[0] = darken_params[0];
      val[1] = darken_params[1];
      val[2] = darken_params[2];
      val[3] = darken_params[3];
      val[4] = darken_params[4];
      val[5] = darken_params[5];
      val[6] = darken_params[6];
      val[7] = darken_params[7];

      वापस error;
    }

    अन्यथा अगर ( !ft_strcmp( property_name, "hinting-engine" ) )
    {
      FT_UInt   hinting_engine    = driver->hinting_engine;
      FT_UInt*  val               = (FT_UInt*)मूल्य;


      *val = hinting_engine;

      वापस error;
    }

    अन्यथा अगर ( !ft_strcmp( property_name, "no-stem-darkening" ) )
    {
      FT_Bool   no_stem_darkening = driver->no_stem_darkening;
      FT_Bool*  val               = (FT_Bool*)मूल्य;


      *val = no_stem_darkening;

      वापस error;
    }

    FT_TRACE2(( "ps_property_get: missing property `%s'\n",
                property_name ));
    वापस FT_THROW( Missing_Property );
  }


/* END */
