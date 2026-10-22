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


#അന്തര്ഭാവ <ft2build.h>
#അന്തര്ഭാവ FT_CONFIG_CONFIG_H
#അന്തര്ഭാവ <freetype/internal/ftobjs.h>
#അന്തര്ഭാവ <freetype/internal/ftdebug.h>
#അന്തര്ഭാവ <freetype/ftmodapi.h>


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
#define FT_USE_MODULE( type, x )  ബാഹ്യ "C" സ്ഥിരം type  x;
#അല്ലെങ്കിൽ
#define FT_USE_MODULE( type, x )  ബാഹ്യ സ്ഥിരം type  x;
#endif

#അന്തര്ഭാവ FT_CONFIG_MODULES_H

#undef  FT_USE_MODULE
#define FT_USE_MODULE( type, x )  (സ്ഥിരം FT_Module_Class*)&(x),

  സ്ഥിതം
  സ്ഥിരം FT_Module_Class*  സ്ഥിരം ft_default_modules[] =
  {
#അന്തര്ഭാവ FT_CONFIG_MODULES_H
    0
  };


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( ശൂന്യം )
  FT_Add_Default_Modules( FT_Library  library )
  {
    FT_Error                       error;
    സ്ഥിരം FT_Module_Class* സ്ഥിരം*  cur;


    /* GCC 4.6 warns the type difference:
     *   FT_Module_Class** != const FT_Module_Class* const*
     */
    cur = (സ്ഥിരം FT_Module_Class* സ്ഥിരം*)ft_default_modules;

    /* test for valid `library' delayed to FT_Add_Module() */
    ആകുമ്പോൾ ( *cur )
    {
      error = FT_Add_Module( library, *cur );
      /* notify errors, but don't stop */
      എങ്കിൽ ( error )
        FT_TRACE0(( "FT_Add_Default_Module:"
                    " Cannot install `%s', error = 0x%x\n",
                    (*cur)->module_name, error ));
      cur++;
    }
  }


#ifdef FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES

#define MAX_LENGTH  128

  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( ശൂന്യം )
  FT_Set_Default_Properties( FT_Library  library )
  {
    സ്ഥിരം അക്ഷരം*  env;
    സ്ഥിരം അക്ഷരം*  p;
    സ്ഥിരം അക്ഷരം*  q;

    അക്ഷരം  module_name[MAX_LENGTH + 1];
    അക്ഷരം  property_name[MAX_LENGTH + 1];
    അക്ഷരം  property_മൂല്യം[MAX_LENGTH + 1];

    പൂർണ്ണസംഖ്യ  i;


    env = ft_getenv( "FREETYPE_PROPERTIES" );
    എങ്കിൽ ( !env )
      തിരികെ;

    വേണ്ടി ( p = env; *p; p++ )
    {
      /* skip leading whitespace and separators */
      എങ്കിൽ ( *p == ' ' || *p == '\t' )
        തുടരുക;

      /* read module name, followed by `:' */
      q = p;
      വേണ്ടി ( i = 0; i < MAX_LENGTH; i++ )
      {
        എങ്കിൽ ( !*p || *p == ':' )
          നിർത്തുക;
        module_name[i] = *p++;
      }
      module_name[i] = '\0';

      എങ്കിൽ ( !*p || *p != ':' || p == q )
        നിർത്തുക;

      /* read property name, followed by `=' */
      q = ++p;
      വേണ്ടി ( i = 0; i < MAX_LENGTH; i++ )
      {
        എങ്കിൽ ( !*p || *p == '=' )
          നിർത്തുക;
        property_name[i] = *p++;
      }
      property_name[i] = '\0';

      എങ്കിൽ ( !*p || *p != '=' || p == q )
        നിർത്തുക;

      /* read property value, followed by whitespace (if any) */
      q = ++p;
      വേണ്ടി ( i = 0; i < MAX_LENGTH; i++ )
      {
        എങ്കിൽ ( !*p || *p == ' ' || *p == '\t' )
          നിർത്തുക;
        property_മൂല്യം[i] = *p++;
      }
      property_മൂല്യം[i] = '\0';

      എങ്കിൽ ( !( *p == '\0' || *p == ' ' || *p == '\t' ) || p == q )
        നിർത്തുക;

      /* we completely ignore errors */
      ft_property_string_set( library,
                              module_name,
                              property_name,
                              property_മൂല്യം );

      എങ്കിൽ ( !*p )
        നിർത്തുക;
    }
  }

#അല്ലെങ്കിൽ

  FT_EXPORT_DEF( ശൂന്യം )
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
    എങ്കിൽ ( !memory )
    {
      FT_ERROR(( "FT_Init_FreeType: cannot find memory manager\n" ));
      തിരികെ FT_THROW( Unimplemented_Feature );
    }

    /* build a library out of it, then fill it with the set of */
    /* default drivers.                                        */

    error = FT_New_Library( memory, alibrary );
    എങ്കിൽ ( error )
      FT_Done_Memory( memory );
    അല്ലെങ്കിൽ
      FT_Add_Default_Modules( *alibrary );

    FT_Set_Default_Properties( *alibrary );

    തിരികെ error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Done_FreeType( FT_Library  library )
  {
    FT_Memory  memory;


    എങ്കിൽ ( !library )
      തിരികെ FT_THROW( Invalid_Library_Handle );

    memory = library->memory;

    /* Discard the library object */
    FT_Done_Library( library );

    /* discard memory manager */
    FT_Done_Memory( memory );

#ifdef FT_DEBUG_LOGGING
    ft_logging_deinit();
#endif

    തിരികെ FT_Err_Ok;
  }


/* END */
