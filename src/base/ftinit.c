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


#અન્તર્ભાવ <ft2build.h>
#અન્તર્ભાવ FT_CONFIG_CONFIG_H
#અન્તર્ભાવ <freetype/internal/ftobjs.h>
#અન્તર્ભાવ <freetype/internal/ftdebug.h>
#અન્તર્ભાવ <freetype/ftmodapi.h>


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
#define FT_USE_MODULE( type, x )  બાહ્ય "C" અચલ type  x;
#નહીંતર
#define FT_USE_MODULE( type, x )  બાહ્ય અચલ type  x;
#endif

#અન્તર્ભાવ FT_CONFIG_MODULES_H

#undef  FT_USE_MODULE
#define FT_USE_MODULE( type, x )  (અચલ FT_Module_Class*)&(x),

  સ્થિર
  અચલ FT_Module_Class*  અચલ ft_default_modules[] =
  {
#અન્તર્ભાવ FT_CONFIG_MODULES_H
    0
  };


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( શૂન્ય )
  FT_Add_Default_Modules( FT_Library  library )
  {
    FT_Error                       error;
    અચલ FT_Module_Class* અચલ*  cur;


    /* GCC 4.6 warns the type difference:
     *   FT_Module_Class** != const FT_Module_Class* const*
     */
    cur = (અચલ FT_Module_Class* અચલ*)ft_default_modules;

    /* test for valid `library' delayed to FT_Add_Module() */
    જ્યારે ( *cur )
    {
      error = FT_Add_Module( library, *cur );
      /* notify errors, but don't stop */
      જો ( error )
        FT_TRACE0(( "FT_Add_Default_Module:"
                    " Cannot install `%s', error = 0x%x\n",
                    (*cur)->module_name, error ));
      cur++;
    }
  }


#ifdef FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES

#define MAX_LENGTH  128

  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( શૂન્ય )
  FT_Set_Default_Properties( FT_Library  library )
  {
    અચલ અક્ષર*  env;
    અચલ અક્ષર*  p;
    અચલ અક્ષર*  q;

    અક્ષર  module_name[MAX_LENGTH + 1];
    અક્ષર  property_name[MAX_LENGTH + 1];
    અક્ષર  property_કિંમત[MAX_LENGTH + 1];

    પૂર્ણાંક  i;


    env = ft_getenv( "FREETYPE_PROPERTIES" );
    જો ( !env )
      પાછા;

    માટે ( p = env; *p; p++ )
    {
      /* skip leading whitespace and separators */
      જો ( *p == ' ' || *p == '\t' )
        ચાલુ;

      /* read module name, followed by `:' */
      q = p;
      માટે ( i = 0; i < MAX_LENGTH; i++ )
      {
        જો ( !*p || *p == ':' )
          તોડો;
        module_name[i] = *p++;
      }
      module_name[i] = '\0';

      જો ( !*p || *p != ':' || p == q )
        તોડો;

      /* read property name, followed by `=' */
      q = ++p;
      માટે ( i = 0; i < MAX_LENGTH; i++ )
      {
        જો ( !*p || *p == '=' )
          તોડો;
        property_name[i] = *p++;
      }
      property_name[i] = '\0';

      જો ( !*p || *p != '=' || p == q )
        તોડો;

      /* read property value, followed by whitespace (if any) */
      q = ++p;
      માટે ( i = 0; i < MAX_LENGTH; i++ )
      {
        જો ( !*p || *p == ' ' || *p == '\t' )
          તોડો;
        property_કિંમત[i] = *p++;
      }
      property_કિંમત[i] = '\0';

      જો ( !( *p == '\0' || *p == ' ' || *p == '\t' ) || p == q )
        તોડો;

      /* we completely ignore errors */
      ft_property_string_set( library,
                              module_name,
                              property_name,
                              property_કિંમત );

      જો ( !*p )
        તોડો;
    }
  }

#નહીંતર

  FT_EXPORT_DEF( શૂન્ય )
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
    જો ( !memory )
    {
      FT_ERROR(( "FT_Init_FreeType: cannot find memory manager\n" ));
      પાછા FT_THROW( Unimplemented_Feature );
    }

    /* build a library out of it, then fill it with the set of */
    /* default drivers.                                        */

    error = FT_New_Library( memory, alibrary );
    જો ( error )
      FT_Done_Memory( memory );
    નહીંતર
      FT_Add_Default_Modules( *alibrary );

    FT_Set_Default_Properties( *alibrary );

    પાછા error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Done_FreeType( FT_Library  library )
  {
    FT_Memory  memory;


    જો ( !library )
      પાછા FT_THROW( Invalid_Library_Handle );

    memory = library->memory;

    /* Discard the library object */
    FT_Done_Library( library );

    /* discard memory manager */
    FT_Done_Memory( memory );

#ifdef FT_DEBUG_LOGGING
    ft_logging_deinit();
#endif

    પાછા FT_Err_Ok;
  }


/* END */
