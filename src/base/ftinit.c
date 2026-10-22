/****************************************************************************
 *
 * ftinit.c
 *
 *   FreeType initialization layer (body).
 *
 * Copyright (C) 1996-2026 by
 * David Turner, Robert Wilhelm, and Werner Lemberg.
 *
 * This file is part of the FreeType project, and may only be used,
 * modified, and distributed under the terms of the FreeType project
 * license, LICENSE.TXT.  By continuing to use, modify, or distribute
 * this file you indicate that you have read the license and
 * understand and accept it fully.
 *
 */

  /**************************************************************************
   *
   * The purpose of this file is to implement the following two
   * functions:
   *
   * FT_Add_Default_Modules():
   *   This function is used to add the set of default modules to a
   *   fresh new library object.  The set is taken from the header file
   *   `freetype/config/ftmodule.h'.  See the document `FreeType 2.0
   *   Build System' for more information.
   *
   * FT_Init_FreeType():
   *   This function creates a system object for the current platform,
   *   builds a library out of it, then calls FT_Default_Drivers().
   *
   * Note that even if FT_Init_FreeType() uses the implementation of the
   * system object defined at build time, client applications are still
   * able to provide their own `ftsystem.c'.
   *
   */


#అన్తర్భావ <ft2build.h>
#అన్తర్భావ FT_CONFIG_CONFIG_H
#అన్తర్భావ <freetype/internal/ftobjs.h>
#అన్తర్భావ <freetype/internal/ftdebug.h>
#అన్తర్భావ <freetype/ftmodapi.h>


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#నిర్వచనం_రద్దు  FT_COMPONENT
#నిర్వచించు FT_COMPONENT  init


#నిర్వచనం_రద్దు  FT_USE_MODULE
#నిర్వచితమైతే __cplusplus
#నిర్వచించు FT_USE_MODULE( type, x )  బాహ్య "C" స్థిరరాశి type  x;
#లేకపోతే
#నిర్వచించు FT_USE_MODULE( type, x )  బాహ్య స్థిరరాశి type  x;
#అయితే_ముగింపు

#అన్తర్భావ FT_CONFIG_MODULES_H

#నిర్వచనం_రద్దు  FT_USE_MODULE
#నిర్వచించు FT_USE_MODULE( type, x )  (స్థిరరాశి FT_Module_Class*)&(x),

  స్థిర
  స్థిరరాశి FT_Module_Class*  స్థిరరాశి ft_default_modules[] =
  {
#అన్తర్భావ FT_CONFIG_MODULES_H
    0
  };


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( శూన్య )
  FT_కూడు_Default_Modules( FT_Library  library )
  {
    FT_Error                       error;
    స్థిరరాశి FT_Module_Class* స్థిరరాశి*  cur;


    /* GCC 4.6 warns the type difference:
     *   FT_Module_Class** != const FT_Module_Class* const*
     */
    cur = (స్థిరరాశి FT_Module_Class* స్థిరరాశి*)ft_default_modules;

    /* test for valid `library' delayed to FT_Add_Module() */
    అయితే ( *cur )
    {
      error = FT_కూడు_Module( library, *cur );
      /* notify errors, but don't stop */
      ఒకవేళ ( error )
        FT_TRACE0(( "FT_Add_Default_Module:"
                    " Cannot install `%s', error = 0x%x\n",
                    (*cur)->module_name, error ));
      cur++;
    }
  }


#నిర్వచితమైతే FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES

#నిర్వచించు MAX_నిడివి  128

  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( శూన్య )
  FT_సమితి_Default_Properties( FT_Library  library )
  {
    స్థిరరాశి అక్షరం*  env;
    స్థిరరాశి అక్షరం*  p;
    స్థిరరాశి అక్షరం*  q;

    అక్షరం  module_name[MAX_నిడివి + 1];
    అక్షరం  property_name[MAX_నిడివి + 1];
    అక్షరం  property_విలువ[MAX_నిడివి + 1];

    పూర్ణాంకం  i;


    env = ft_getenv( "FREETYPE_PROPERTIES" );
    ఒకవేళ ( !env )
      తిరిగి;

    కొరకు ( p = env; *p; p++ )
    {
      /* skip leading whitespace and separators */
      ఒకవేళ ( *p == ' ' || *p == '\t' )
        కొనసాగు;

      /* read module name, followed by `:' */
      q = p;
      కొరకు ( i = 0; i < MAX_నిడివి; i++ )
      {
        ఒకవేళ ( !*p || *p == ':' )
          ఆపు;
        module_name[i] = *p++;
      }
      module_name[i] = '\0';

      ఒకవేళ ( !*p || *p != ':' || p == q )
        ఆపు;

      /* read property name, followed by `=' */
      q = ++p;
      కొరకు ( i = 0; i < MAX_నిడివి; i++ )
      {
        ఒకవేళ ( !*p || *p == '=' )
          ఆపు;
        property_name[i] = *p++;
      }
      property_name[i] = '\0';

      ఒకవేళ ( !*p || *p != '=' || p == q )
        ఆపు;

      /* read property value, followed by whitespace (if any) */
      q = ++p;
      కొరకు ( i = 0; i < MAX_నిడివి; i++ )
      {
        ఒకవేళ ( !*p || *p == ' ' || *p == '\t' )
          ఆపు;
        property_విలువ[i] = *p++;
      }
      property_విలువ[i] = '\0';

      ఒకవేళ ( !( *p == '\0' || *p == ' ' || *p == '\t' ) || p == q )
        ఆపు;

      /* we completely ignore errors */
      ft_property_string_సమితి( library,
                              module_name,
                              property_name,
                              property_విలువ );

      ఒకవేళ ( !*p )
        ఆపు;
    }
  }

#లేకపోతే

  FT_EXPORT_DEF( శూన్య )
  FT_సమితి_Default_Properties( FT_Library  library )
  {
    FT_వాడని( library );
  }

#అయితే_ముగింపు


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Init_FreeType( FT_Library  *alibrary )
  {
    FT_Error   error;
    FT_జ్ఞాపకం  జ్ఞాపకం;


#నిర్వచితమైతే FT_DEBUG_LOGGING
    ft_logging_init();
#అయితే_ముగింపు

    /* check of `alibrary' delayed to `FT_New_Library' */

    /* First of all, allocate a new system object -- this function is part */
    /* of the system-specific component, i.e. `ftsystem.c'.                */

    జ్ఞాపకం = FT_New_జ్ఞాపకం();
    ఒకవేళ ( !జ్ఞాపకం )
    {
      FT_ERROR(( "FT_Init_FreeType: cannot find memory manager\n" ));
      తిరిగి FT_THROW( Unimplemented_Feature );
    }

    /* build a library out of it, then fill it with the set of */
    /* default drivers.                                        */

    error = FT_New_Library( జ్ఞాపకం, alibrary );
    ఒకవేళ ( error )
      FT_పూర్తి_జ్ఞాపకం( జ్ఞాపకం );
    లేకపోతే
      FT_కూడు_Default_Modules( *alibrary );

    FT_సమితి_Default_Properties( *alibrary );

    తిరిగి error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_పూర్తి_Free_Type( FT_Library  library )
  {
    FT_జ్ఞాపకం  జ్ఞాపకం;


    ఒకవేళ ( !library )
      తిరిగి FT_THROW( Invalid_Library_Handle );

    జ్ఞాపకం = library->జ్ఞాపకం;

    /* Discard the library object */
    FT_పూర్తి_Library( library );

    /* discard memory manager */
    FT_పూర్తి_జ్ఞాపకం( జ్ఞాపకం );

#నిర్వచితమైతే FT_DEBUG_LOGGING
    ft_logging_deinit();
#అయితే_ముగింపు

    తిరిగి FT_Err_Ok;
  }


/*చివర*/
