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
#undef  FT_COMPONENT
#define FT_COMPONENT  init


#undef  FT_USE_MODULE
#ifdef __cplusplus
#define FT_USE_MODULE( type, x )  बाह्य "C" स्थिरांक type  x;
#अन्यथा
#define FT_USE_MODULE( type, x )  बाह्य स्थिरांक type  x;
#endif

#अन्तर्भाव FT_CONFIG_MODULES_H

#undef  FT_USE_MODULE
#define FT_USE_MODULE( type, x )  (स्थिरांक FT_Module_Class*)&(x),

  स्थिर
  स्थिरांक FT_Module_Class*  स्थिरांक ft_default_modules[] =
  {
#अन्तर्भाव FT_CONFIG_MODULES_H
    0
  };


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( शून्य )
  FT_Add_Default_Modules( FT_Library  library )
  {
    FT_Error                       error;
    स्थिरांक FT_Module_Class* स्थिरांक*  cur;


    /* GCC 4.6 warns the type difference:
     *   FT_Module_Class** != const FT_Module_Class* const*
     */
    cur = (स्थिरांक FT_Module_Class* स्थिरांक*)ft_default_modules;

    /* test for valid `library' delayed to FT_Add_Module() */
    पर्यंत ( *cur )
    {
      error = FT_Add_Module( library, *cur );
      /* notify errors, but don't stop */
      जर ( error )
        FT_TRACE0(( "FT_Add_Default_Module:"
                    " Cannot install `%s', error = 0x%x\n",
                    (*cur)->module_name, error ));
      cur++;
    }
  }


#ifdef FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES

#define MAX_LENGTH  128

  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( शून्य )
  FT_Set_Default_Properties( FT_Library  library )
  {
    स्थिरांक अक्षर*  env;
    स्थिरांक अक्षर*  p;
    स्थिरांक अक्षर*  q;

    अक्षर  module_name[MAX_LENGTH + 1];
    अक्षर  property_name[MAX_LENGTH + 1];
    अक्षर  property_मूल्य[MAX_LENGTH + 1];

    पूर्णांक  i;


    env = ft_getenv( "FREETYPE_PROPERTIES" );
    जर ( !env )
      परत;

    साठी ( p = env; *p; p++ )
    {
      /* skip leading whitespace and separators */
      जर ( *p == ' ' || *p == '\t' )
        सुरूठेवा;

      /* read module name, followed by `:' */
      q = p;
      साठी ( i = 0; i < MAX_LENGTH; i++ )
      {
        जर ( !*p || *p == ':' )
          थांबा;
        module_name[i] = *p++;
      }
      module_name[i] = '\0';

      जर ( !*p || *p != ':' || p == q )
        थांबा;

      /* read property name, followed by `=' */
      q = ++p;
      साठी ( i = 0; i < MAX_LENGTH; i++ )
      {
        जर ( !*p || *p == '=' )
          थांबा;
        property_name[i] = *p++;
      }
      property_name[i] = '\0';

      जर ( !*p || *p != '=' || p == q )
        थांबा;

      /* read property value, followed by whitespace (if any) */
      q = ++p;
      साठी ( i = 0; i < MAX_LENGTH; i++ )
      {
        जर ( !*p || *p == ' ' || *p == '\t' )
          थांबा;
        property_मूल्य[i] = *p++;
      }
      property_मूल्य[i] = '\0';

      जर ( !( *p == '\0' || *p == ' ' || *p == '\t' ) || p == q )
        थांबा;

      /* we completely ignore errors */
      ft_property_string_set( library,
                              module_name,
                              property_name,
                              property_मूल्य );

      जर ( !*p )
        थांबा;
    }
  }

#अन्यथा

  FT_EXPORT_DEF( शून्य )
  FT_Set_Default_Properties( FT_Library  library )
  {
    FT_UNUSED( library );
  }

#endif


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Init_FreeType( FT_Library  *alibrary )
  {
    FT_Error   error;
    FT_Memory  memory;


#ifdef FT_DEBUG_LOGGING
    ft_logging_init();
#endif

    /* check of `alibrary' delayed to `FT_New_Library' */

    /* First of all, allocate a new system object -- this function is part */
    /* of the system-specific component, i.e. `ftsystem.c'.                */

    memory = FT_New_Memory();
    जर ( !memory )
    {
      FT_ERROR(( "FT_Init_FreeType: cannot find memory manager\n" ));
      परत FT_THROW( Unimplemented_Feature );
    }

    /* build a library out of it, then fill it with the set of */
    /* default drivers.                                        */

    error = FT_New_Library( memory, alibrary );
    जर ( error )
      FT_Done_Memory( memory );
    अन्यथा
      FT_Add_Default_Modules( *alibrary );

    FT_Set_Default_Properties( *alibrary );

    परत error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Done_FreeType( FT_Library  library )
  {
    FT_Memory  memory;


    जर ( !library )
      परत FT_THROW( Invalid_Library_Handle );

    memory = library->memory;

    /* Discard the library object */
    FT_Done_Library( library );

    /* discard memory manager */
    FT_Done_Memory( memory );

#ifdef FT_DEBUG_LOGGING
    ft_logging_deinit();
#endif

    परत FT_Err_Ok;
  }


/* END */
