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


#अन्तर्भाव <ft2build.h>
#अन्तर्भाव FT_CONFIG_CONFIG_H
#अन्तर्भाव <freetype/internal/ftobjs.h>
#अन्तर्भाव <freetype/internal/ftdebug.h>
#अन्तर्भाव <freetype/ftmodapi.h>


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#अपरिभाषय  FT_COMPONENT
#परिभाषय FT_COMPONENT  init


#अपरिभाषय  FT_USE_MODULE
#यदि_परिभाषितम् __cplusplus
#परिभाषय FT_USE_MODULE( type, x )  बाह्य "C" ध्रुव type  x;
#अन्यथा
#परिभाषय FT_USE_MODULE( type, x )  बाह्य ध्रुव type  x;
#यदि_अन्तः

#अन्तर्भाव FT_CONFIG_MODULES_H

#अपरिभाषय  FT_USE_MODULE
#परिभाषय FT_USE_MODULE( type, x )  (ध्रुव FT_Module_Class*)&(x),

  स्थिर
  ध्रुव FT_Module_Class*  ध्रुव ft_default_modules[] =
  {
#अन्तर्भाव FT_CONFIG_MODULES_H
    0
  };


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( शून्य )
  FT_सङ्कलय_Default_Modules( FT_Library  library )
  {
    FT_Error                       error;
    ध्रुव FT_Module_Class* ध्रुव*  cur;


    /* GCC 4.6 warns the type difference:
     *   FT_Module_Class** != const FT_Module_Class* const*
     */
    cur = (ध्रुव FT_Module_Class* ध्रुव*)ft_default_modules;

    /* test for valid `library' delayed to FT_Add_Module() */
    यावत् ( *cur )
    {
      error = FT_सङ्कलय_Module( library, *cur );
      /* notify errors, but don't stop */
      यदि ( error )
        FT_TRACE0(( "FT_Add_Default_Module:"
                    " Cannot install `%s', error = 0x%x\n",
                    (*cur)->module_name, error ));
      cur++;
    }
  }


#यदि_परिभाषितम् FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES

#परिभाषय MAX_दीर्घता  128

  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( शून्य )
  FT_समुच्चयः_Default_Properties( FT_Library  library )
  {
    ध्रुव वर्ण*  env;
    ध्रुव वर्ण*  p;
    ध्रुव वर्ण*  q;

    वर्ण  module_name[MAX_दीर्घता + 1];
    वर्ण  property_name[MAX_दीर्घता + 1];
    वर्ण  property_मानम्[MAX_दीर्घता + 1];

    पूर्णाङ्क  i;


    env = ft_getenv( "FREETYPE_PROPERTIES" );
    यदि ( !env )
      प्रत्यागम;

    प्रति ( p = env; *p; p++ )
    {
      /* skip leading whitespace and separators */
      यदि ( *p == ' ' || *p == '\t' )
        अनुवर्त;

      /* read module name, followed by `:' */
      q = p;
      प्रति ( i = 0; i < MAX_दीर्घता; i++ )
      {
        यदि ( !*p || *p == ':' )
          भङ्ग;
        module_name[i] = *p++;
      }
      module_name[i] = '\0';

      यदि ( !*p || *p != ':' || p == q )
        भङ्ग;

      /* read property name, followed by `=' */
      q = ++p;
      प्रति ( i = 0; i < MAX_दीर्घता; i++ )
      {
        यदि ( !*p || *p == '=' )
          भङ्ग;
        property_name[i] = *p++;
      }
      property_name[i] = '\0';

      यदि ( !*p || *p != '=' || p == q )
        भङ्ग;

      /* read property value, followed by whitespace (if any) */
      q = ++p;
      प्रति ( i = 0; i < MAX_दीर्घता; i++ )
      {
        यदि ( !*p || *p == ' ' || *p == '\t' )
          भङ्ग;
        property_मानम्[i] = *p++;
      }
      property_मानम्[i] = '\0';

      यदि ( !( *p == '\0' || *p == ' ' || *p == '\t' ) || p == q )
        भङ्ग;

      /* we completely ignore errors */
      ft_property_string_समुच्चयः( library,
                              module_name,
                              property_name,
                              property_मानम् );

      यदि ( !*p )
        भङ्ग;
    }
  }

#अन्यथा

  FT_EXPORT_DEF( शून्य )
  FT_समुच्चयः_Default_Properties( FT_Library  library )
  {
    FT_अप्रयुक्तम्( library );
  }

#यदि_अन्तः


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Init_FreeType( FT_Library  *alibrary )
  {
    FT_Error   error;
    FT_स्मृतिः  स्मृतिः;


#यदि_परिभाषितम् FT_DEBUG_LOGGING
    ft_logging_init();
#यदि_अन्तः

    /* check of `alibrary' delayed to `FT_New_Library' */

    /* First of all, allocate a new system object -- this function is part */
    /* of the system-specific component, i.e. `ftsystem.c'.                */

    स्मृतिः = FT_New_स्मृतिः();
    यदि ( !स्मृतिः )
    {
      FT_ERROR(( "FT_Init_FreeType: cannot find memory manager\n" ));
      प्रत्यागम FT_THROW( Unimplemented_Feature );
    }

    /* build a library out of it, then fill it with the set of */
    /* default drivers.                                        */

    error = FT_New_Library( स्मृतिः, alibrary );
    यदि ( error )
      FT_समाप्तम्_स्मृतिः( स्मृतिः );
    अन्यथा
      FT_सङ्कलय_Default_Modules( *alibrary );

    FT_समुच्चयः_Default_Properties( *alibrary );

    प्रत्यागम error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_समाप्तम्_Free_Type( FT_Library  library )
  {
    FT_स्मृतिः  स्मृतिः;


    यदि ( !library )
      प्रत्यागम FT_THROW( Invalid_Library_Handle );

    स्मृतिः = library->स्मृतिः;

    /* Discard the library object */
    FT_समाप्तम्_Library( library );

    /* discard memory manager */
    FT_समाप्तम्_स्मृतिः( स्मृतिः );

#यदि_परिभाषितम् FT_DEBUG_LOGGING
    ft_logging_deinit();
#यदि_अन्तः

    प्रत्यागम FT_Err_Ok;
  }


/*अन्तः*/
