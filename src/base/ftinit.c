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


#समावेश <ft2build.h>
#समावेश FT_CONFIG_CONFIG_H
#समावेश <freetype/internal/ftobjs.h>
#समावेश <freetype/internal/ftdebug.h>
#समावेश <freetype/ftmodapi.h>


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#अघोषित  FT_COMPONENT
#घोषणा FT_COMPONENT  init


#अघोषित  FT_USE_MODULE
#अगर_घोषित __cplusplus
#घोषणा FT_USE_MODULE( type, x )  बाह्य "C" स्थिर type  x;
#अन्यथा
#घोषणा FT_USE_MODULE( type, x )  बाह्य स्थिर type  x;
#पूर्ण_अगर

#समावेश FT_CONFIG_MODULES_H

#अघोषित  FT_USE_MODULE
#घोषणा FT_USE_MODULE( type, x )  (स्थिर FT_Module_Class*)&(x),

  अटल
  स्थिर FT_Module_Class*  स्थिर ft_default_modules[] =
  {
#समावेश FT_CONFIG_MODULES_H
    0
  };


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( व्योम )
  FT_Add_Default_Modules( FT_Library  library )
  {
    FT_Error                       error;
    स्थिर FT_Module_Class* स्थिर*  cur;


    /* GCC 4.6 warns the type difference:
     *   FT_Module_Class** != const FT_Module_Class* const*
     */
    cur = (स्थिर FT_Module_Class* स्थिर*)ft_default_modules;

    /* test for valid `library' delayed to FT_Add_Module() */
    जबतक ( *cur )
    {
      error = FT_Add_Module( library, *cur );
      /* notify errors, but don't stop */
      अगर ( error )
        FT_TRACE0(( "FT_Add_Default_Module:"
                    " Cannot install `%s', error = 0x%x\n",
                    (*cur)->module_name, error ));
      cur++;
    }
  }


#अगर_घोषित FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES

#घोषणा MAX_लंबाई  128

  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( व्योम )
  FT_Set_Default_Properties( FT_Library  library )
  {
    स्थिर अक्षर*  env;
    स्थिर अक्षर*  p;
    स्थिर अक्षर*  q;

    अक्षर  module_name[MAX_लंबाई + 1];
    अक्षर  property_name[MAX_लंबाई + 1];
    अक्षर  property_मूल्य[MAX_लंबाई + 1];

    पूर्णांक  i;


    env = ft_getenv( "FREETYPE_PROPERTIES" );
    अगर ( !env )
      वापस;

    क्रम ( p = env; *p; p++ )
    {
      /* skip leading whitespace and separators */
      अगर ( *p == ' ' || *p == '\t' )
        जारी;

      /* read module name, followed by `:' */
      q = p;
      क्रम ( i = 0; i < MAX_लंबाई; i++ )
      {
        अगर ( !*p || *p == ':' )
          अवरोध;
        module_name[i] = *p++;
      }
      module_name[i] = '\0';

      अगर ( !*p || *p != ':' || p == q )
        अवरोध;

      /* read property name, followed by `=' */
      q = ++p;
      क्रम ( i = 0; i < MAX_लंबाई; i++ )
      {
        अगर ( !*p || *p == '=' )
          अवरोध;
        property_name[i] = *p++;
      }
      property_name[i] = '\0';

      अगर ( !*p || *p != '=' || p == q )
        अवरोध;

      /* read property value, followed by whitespace (if any) */
      q = ++p;
      क्रम ( i = 0; i < MAX_लंबाई; i++ )
      {
        अगर ( !*p || *p == ' ' || *p == '\t' )
          अवरोध;
        property_मूल्य[i] = *p++;
      }
      property_मूल्य[i] = '\0';

      अगर ( !( *p == '\0' || *p == ' ' || *p == '\t' ) || p == q )
        अवरोध;

      /* we completely ignore errors */
      ft_property_string_set( library,
                              module_name,
                              property_name,
                              property_मूल्य );

      अगर ( !*p )
        अवरोध;
    }
  }

#अन्यथा

  FT_EXPORT_DEF( व्योम )
  FT_Set_Default_Properties( FT_Library  library )
  {
    FT_UNUSED( library );
  }

#पूर्ण_अगर


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Init_FreeType( FT_Library  *alibrary )
  {
    FT_Error   error;
    FT_Memory  memory;


#अगर_घोषित FT_DEBUG_LOGGING
    ft_logging_init();
#पूर्ण_अगर

    /* check of `alibrary' delayed to `FT_New_Library' */

    /* First of all, allocate a new system object -- this function is part */
    /* of the system-specific component, i.e. `ftsystem.c'.                */

    memory = FT_New_Memory();
    अगर ( !memory )
    {
      FT_ERROR(( "FT_Init_FreeType: cannot find memory manager\n" ));
      वापस FT_THROW( Unimplemented_Feature );
    }

    /* build a library out of it, then fill it with the set of */
    /* default drivers.                                        */

    error = FT_New_Library( memory, alibrary );
    अगर ( error )
      FT_Done_Memory( memory );
    अन्यथा
      FT_Add_Default_Modules( *alibrary );

    FT_Set_Default_Properties( *alibrary );

    वापस error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Done_FreeType( FT_Library  library )
  {
    FT_Memory  memory;


    अगर ( !library )
      वापस FT_THROW( Invalid_Library_Handle );

    memory = library->memory;

    /* Discard the library object */
    FT_Done_Library( library );

    /* discard memory manager */
    FT_Done_Memory( memory );

#अगर_घोषित FT_DEBUG_LOGGING
    ft_logging_deinit();
#पूर्ण_अगर

    वापस FT_Err_Ok;
  }


/* END */
