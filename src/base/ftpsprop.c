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


#अन्तर्भाव <freetype/ftdriver.h>
#अन्तर्भाव <freetype/internal/ftdebug.h>
#अन्तर्भाव <freetype/internal/psaux.h>
#अन्तर्भाव <freetype/internal/ftobjs.h>
#अन्तर्भाव <freetype/internal/ftpsprop.h>


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
                   अचर वर्ण*  property_name,
                   अचर शून्य*  मान,
                   FT_Bool      मान_is_string )
  {
    FT_Error   error  = FT_Err_Ok;
    PS_Driver  driver = (PS_Driver)module;

#ifndef FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES
    FT_UNUSED( मान_is_string );
#endif


    यदि ( !ft_strcmp( property_name, "darkening-parameters" ) )
    {
      FT_Int*  darken_params;
      FT_Int   x1, y1, x2, y2, x3, y3, x4, y4;

#ifdef FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES
      FT_Int   dp[8];


      यदि ( मान_is_string )
      {
        अचर वर्ण*  s = (अचर वर्ण*)मान;
        वर्ण*        ep;
        पूर्णाङ्क          i;


        /* eight comma-separated numbers */
        लागि ( i = 0; i < 7; i++ )
        {
          dp[i] = (FT_Int)ft_strtol( s, &ep, 10 );
          यदि ( *ep != ',' || s == ep )
            फर्काऊ FT_THROW( Invalid_Argument );

          s = ep + 1;
        }

        dp[7] = (FT_Int)ft_strtol( s, &ep, 10 );
        यदि ( !( *ep == '\0' || *ep == ' ' ) || s == ep )
          फर्काऊ FT_THROW( Invalid_Argument );

        darken_params = dp;
      }
      अन्यथा
#endif
        darken_params = (FT_Int*)मान;

      x1 = darken_params[0];
      y1 = darken_params[1];
      x2 = darken_params[2];
      y2 = darken_params[3];
      x3 = darken_params[4];
      y3 = darken_params[5];
      x4 = darken_params[6];
      y4 = darken_params[7];

      यदि ( x1 < 0   || x2 < 0   || x3 < 0   || x4 < 0   ||
           y1 < 0   || y2 < 0   || y3 < 0   || y4 < 0   ||
           x1 > x2  || x2 > x3  || x3 > x4              ||
           y1 > 500 || y2 > 500 || y3 > 500 || y4 > 500 )
        फर्काऊ FT_THROW( Invalid_Argument );

      driver->darken_params[0] = x1;
      driver->darken_params[1] = y1;
      driver->darken_params[2] = x2;
      driver->darken_params[3] = y2;
      driver->darken_params[4] = x3;
      driver->darken_params[5] = y3;
      driver->darken_params[6] = x4;
      driver->darken_params[7] = y4;

      फर्काऊ error;
    }

    अन्यथा यदि ( !ft_strcmp( property_name, "hinting-engine" ) )
    {
#यदि defined( CFF_CONFIG_OPTION_OLD_ENGINE ) || \
    defined( T1_CONFIG_OPTION_OLD_ENGINE  )
      अचर वर्ण*  module_name = module->clazz->module_name;
#endif


#ifdef FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES
      यदि ( मान_is_string )
      {
        अचर वर्ण*  s = (अचर वर्ण*)मान;


        यदि ( !ft_strcmp( s, "adobe" ) )
          driver->hinting_engine = FT_HINTING_ADOBE;

#ifdef CFF_CONFIG_OPTION_OLD_ENGINE
        अन्यथा यदि ( !ft_strcmp( module_name, "cff" ) &&
                  !ft_strcmp( s, "freetype" )      )
          driver->hinting_engine = FT_HINTING_FREETYPE;
#endif

#ifdef T1_CONFIG_OPTION_OLD_ENGINE
        अन्यथा यदि ( ( !ft_strcmp( module_name, "type1" ) ||
                    !ft_strcmp( module_name, "t1cid" ) ) &&
                  !ft_strcmp( s, "freetype" )            )
          driver->hinting_engine = FT_HINTING_FREETYPE;
#endif

        अन्यथा
          फर्काऊ FT_THROW( Invalid_Argument );
      }
      अन्यथा
#endif /* FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES */
      {
        FT_UInt*  hinting_engine = (FT_UInt*)मान;


        यदि ( *hinting_engine == FT_HINTING_ADOBE
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
        अन्यथा
          error = FT_ERR( Unimplemented_Feature );
      }

      फर्काऊ error;
    }

    अन्यथा यदि ( !ft_strcmp( property_name, "no-stem-darkening" ) )
    {
#ifdef FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES
      यदि ( मान_is_string )
      {
        अचर वर्ण*  s   = (अचर वर्ण*)मान;
        दीर्घ         nsd = ft_strtol( s, रिक्त, 10 );


        यदि ( !nsd )
          driver->no_stem_darkening = FALSE;
        अन्यथा
          driver->no_stem_darkening = TRUE;
      }
      अन्यथा
#endif
      {
        FT_Bool*  no_stem_darkening = (FT_Bool*)मान;


        driver->no_stem_darkening = *no_stem_darkening;
      }

      फर्काऊ error;
    }

    अन्यथा यदि ( !ft_strcmp( property_name, "random-seed" ) )
    {
      FT_Int32  random_seed;


#ifdef FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES
      यदि ( मान_is_string )
      {
        अचर वर्ण*  s = (अचर वर्ण*)मान;


        random_seed = (FT_Int32)ft_strtol( s, रिक्त, 10 );
      }
      अन्यथा
#endif
        random_seed = *(FT_Int32*)मान;

      यदि ( random_seed < 0 )
        random_seed = 0;

      driver->random_seed = random_seed;

      फर्काऊ error;
    }

    FT_TRACE2(( "ps_property_set: missing property `%s'\n",
                property_name ));
    फर्काऊ FT_THROW( Missing_Property );
  }


  FT_BASE_CALLBACK_DEF( FT_Error )
  ps_property_get( FT_Module    module,         /* PS_Driver */
                   अचर वर्ण*  property_name,
                   शून्य*        मान )
  {
    FT_Error   error  = FT_Err_Ok;
    PS_Driver  driver = (PS_Driver)module;


    यदि ( !ft_strcmp( property_name, "darkening-parameters" ) )
    {
      FT_Int*  darken_params = driver->darken_params;
      FT_Int*  val           = (FT_Int*)मान;


      val[0] = darken_params[0];
      val[1] = darken_params[1];
      val[2] = darken_params[2];
      val[3] = darken_params[3];
      val[4] = darken_params[4];
      val[5] = darken_params[5];
      val[6] = darken_params[6];
      val[7] = darken_params[7];

      फर्काऊ error;
    }

    अन्यथा यदि ( !ft_strcmp( property_name, "hinting-engine" ) )
    {
      FT_UInt   hinting_engine    = driver->hinting_engine;
      FT_UInt*  val               = (FT_UInt*)मान;


      *val = hinting_engine;

      फर्काऊ error;
    }

    अन्यथा यदि ( !ft_strcmp( property_name, "no-stem-darkening" ) )
    {
      FT_Bool   no_stem_darkening = driver->no_stem_darkening;
      FT_Bool*  val               = (FT_Bool*)मान;


      *val = no_stem_darkening;

      फर्काऊ error;
    }

    FT_TRACE2(( "ps_property_get: missing property `%s'\n",
                property_name ));
    फर्काऊ FT_THROW( Missing_Property );
  }


/* END */
