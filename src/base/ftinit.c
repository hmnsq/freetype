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


#ଅନ୍ତର୍ଭାଵ <ft2build.h>
#ଅନ୍ତର୍ଭାଵ FT_CONFIG_CONFIG_H
#ଅନ୍ତର୍ଭାଵ <freetype/internal/ftobjs.h>
#ଅନ୍ତର୍ଭାଵ <freetype/internal/ftdebug.h>
#ଅନ୍ତର୍ଭାଵ <freetype/ftmodapi.h>


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
#define FT_USE_MODULE( type, x )  ବାହ୍ୟ "C" ଅଚଳ type  x;
#ଅନ୍ୟଥା
#define FT_USE_MODULE( type, x )  ବାହ୍ୟ ଅଚଳ type  x;
#endif

#ଅନ୍ତର୍ଭାଵ FT_CONFIG_MODULES_H

#undef  FT_USE_MODULE
#define FT_USE_MODULE( type, x )  (ଅଚଳ FT_Module_Class*)&(x),

  ସ୍ଥିର
  ଅଚଳ FT_Module_Class*  ଅଚଳ ft_default_modules[] =
  {
#ଅନ୍ତର୍ଭାଵ FT_CONFIG_MODULES_H
    0
  };


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( ଶୂନ୍ୟ )
  FT_Add_Default_Modules( FT_Library  library )
  {
    FT_Error                       error;
    ଅଚଳ FT_Module_Class* ଅଚଳ*  cur;


    /* GCC 4.6 warns the type difference:
     *   FT_Module_Class** != const FT_Module_Class* const*
     */
    cur = (ଅଚଳ FT_Module_Class* ଅଚଳ*)ft_default_modules;

    /* test for valid `library' delayed to FT_Add_Module() */
    ଯେତେବେଳେ ( *cur )
    {
      error = FT_Add_Module( library, *cur );
      /* notify errors, but don't stop */
      ଯଦି ( error )
        FT_TRACE0(( "FT_Add_Default_Module:"
                    " Cannot install `%s', error = 0x%x\n",
                    (*cur)->module_name, error ));
      cur++;
    }
  }


#ifdef FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES

#define MAX_LENGTH  128

  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( ଶୂନ୍ୟ )
  FT_Set_Default_Properties( FT_Library  library )
  {
    ଅଚଳ ଅକ୍ଷର*  env;
    ଅଚଳ ଅକ୍ଷର*  p;
    ଅଚଳ ଅକ୍ଷର*  q;

    ଅକ୍ଷର  module_name[MAX_LENGTH + 1];
    ଅକ୍ଷର  property_name[MAX_LENGTH + 1];
    ଅକ୍ଷର  property_ମୂଲ୍ୟ[MAX_LENGTH + 1];

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା  i;


    env = ft_getenv( "FREETYPE_PROPERTIES" );
    ଯଦି ( !env )
      ଫେରନ୍ତୁ;

    ପାଇଁ ( p = env; *p; p++ )
    {
      /* skip leading whitespace and separators */
      ଯଦି ( *p == ' ' || *p == '\t' )
        ଜାରି;

      /* read module name, followed by `:' */
      q = p;
      ପାଇଁ ( i = 0; i < MAX_LENGTH; i++ )
      {
        ଯଦି ( !*p || *p == ':' )
          ଭାଙ୍ଗ;
        module_name[i] = *p++;
      }
      module_name[i] = '\0';

      ଯଦି ( !*p || *p != ':' || p == q )
        ଭାଙ୍ଗ;

      /* read property name, followed by `=' */
      q = ++p;
      ପାଇଁ ( i = 0; i < MAX_LENGTH; i++ )
      {
        ଯଦି ( !*p || *p == '=' )
          ଭାଙ୍ଗ;
        property_name[i] = *p++;
      }
      property_name[i] = '\0';

      ଯଦି ( !*p || *p != '=' || p == q )
        ଭାଙ୍ଗ;

      /* read property value, followed by whitespace (if any) */
      q = ++p;
      ପାଇଁ ( i = 0; i < MAX_LENGTH; i++ )
      {
        ଯଦି ( !*p || *p == ' ' || *p == '\t' )
          ଭାଙ୍ଗ;
        property_ମୂଲ୍ୟ[i] = *p++;
      }
      property_ମୂଲ୍ୟ[i] = '\0';

      ଯଦି ( !( *p == '\0' || *p == ' ' || *p == '\t' ) || p == q )
        ଭାଙ୍ଗ;

      /* we completely ignore errors */
      ft_property_string_set( library,
                              module_name,
                              property_name,
                              property_ମୂଲ୍ୟ );

      ଯଦି ( !*p )
        ଭାଙ୍ଗ;
    }
  }

#ଅନ୍ୟଥା

  FT_EXPORT_DEF( ଶୂନ୍ୟ )
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
    ଯଦି ( !memory )
    {
      FT_ERROR(( "FT_Init_FreeType: cannot find memory manager\n" ));
      ଫେରନ୍ତୁ FT_THROW( Unimplemented_Feature );
    }

    /* build a library out of it, then fill it with the set of */
    /* default drivers.                                        */

    error = FT_New_Library( memory, alibrary );
    ଯଦି ( error )
      FT_Done_Memory( memory );
    ଅନ୍ୟଥା
      FT_Add_Default_Modules( *alibrary );

    FT_Set_Default_Properties( *alibrary );

    ଫେରନ୍ତୁ error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Done_FreeType( FT_Library  library )
  {
    FT_Memory  memory;


    ଯଦି ( !library )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Library_Handle );

    memory = library->memory;

    /* Discard the library object */
    FT_Done_Library( library );

    /* discard memory manager */
    FT_Done_Memory( memory );

#ifdef FT_DEBUG_LOGGING
    ft_logging_deinit();
#endif

    ଫେରନ୍ତୁ FT_Err_Ok;
  }


/* END */
