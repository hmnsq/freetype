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


#ಅನ್ತರ್ಭಾವ <ft2build.h>
#ಅನ್ತರ್ಭಾವ FT_CONFIG_CONFIG_H
#ಅನ್ತರ್ಭಾವ <freetype/internal/ftobjs.h>
#ಅನ್ತರ್ಭಾವ <freetype/internal/ftdebug.h>
#ಅನ್ತರ್ಭಾವ <freetype/ftmodapi.h>


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
#define FT_USE_MODULE( type, x )  ಬಾಹ್ಯ "C" ಧ್ರುವ type  x;
#ಅನ್ಯಥಾ
#define FT_USE_MODULE( type, x )  ಬಾಹ್ಯ ಧ್ರುವ type  x;
#endif

#ಅನ್ತರ್ಭಾವ FT_CONFIG_MODULES_H

#undef  FT_USE_MODULE
#define FT_USE_MODULE( type, x )  (ಧ್ರುವ FT_Module_Class*)&(x),

  ಸ್ಥಿರ
  ಧ್ರುವ FT_Module_Class*  ಧ್ರುವ ft_default_modules[] =
  {
#ಅನ್ತರ್ಭಾವ FT_CONFIG_MODULES_H
    0
  };


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( ಶೂನ್ಯ )
  FT_Add_Default_Modules( FT_Library  library )
  {
    FT_Error                       error;
    ಧ್ರುವ FT_Module_Class* ಧ್ರುವ*  cur;


    /* GCC 4.6 warns the type difference:
     *   FT_Module_Class** != const FT_Module_Class* const*
     */
    cur = (ಧ್ರುವ FT_Module_Class* ಧ್ರುವ*)ft_default_modules;

    /* test for valid `library' delayed to FT_Add_Module() */
    ಯಾವತ್ ( *cur )
    {
      error = FT_Add_Module( library, *cur );
      /* notify errors, but don't stop */
      ಯದಿ ( error )
        FT_TRACE0(( "FT_Add_Default_Module:"
                    " Cannot install `%s', error = 0x%x\n",
                    (*cur)->module_name, error ));
      cur++;
    }
  }


#ifdef FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES

#define MAX_LENGTH  128

  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( ಶೂನ್ಯ )
  FT_Set_Default_Properties( FT_Library  library )
  {
    ಧ್ರುವ ವರ್ಣ*  env;
    ಧ್ರುವ ವರ್ಣ*  p;
    ಧ್ರುವ ವರ್ಣ*  q;

    ವರ್ಣ  module_name[MAX_LENGTH + 1];
    ವರ್ಣ  property_name[MAX_LENGTH + 1];
    ವರ್ಣ  property_ಮೌಲ್ಯ[MAX_LENGTH + 1];

    ಪೂರ್ಣಾಙ್ಕ  i;


    env = ft_getenv( "FREETYPE_PROPERTIES" );
    ಯದಿ ( !env )
      ಪ್ರತ್ಯಾಗಮ;

    ಪ್ರತಿ ( p = env; *p; p++ )
    {
      /* skip leading whitespace and separators */
      ಯದಿ ( *p == ' ' || *p == '\t' )
        ಅನುವರ್ತ;

      /* read module name, followed by `:' */
      q = p;
      ಪ್ರತಿ ( i = 0; i < MAX_LENGTH; i++ )
      {
        ಯದಿ ( !*p || *p == ':' )
          ಭಙ್ಗ;
        module_name[i] = *p++;
      }
      module_name[i] = '\0';

      ಯದಿ ( !*p || *p != ':' || p == q )
        ಭಙ್ಗ;

      /* read property name, followed by `=' */
      q = ++p;
      ಪ್ರತಿ ( i = 0; i < MAX_LENGTH; i++ )
      {
        ಯದಿ ( !*p || *p == '=' )
          ಭಙ್ಗ;
        property_name[i] = *p++;
      }
      property_name[i] = '\0';

      ಯದಿ ( !*p || *p != '=' || p == q )
        ಭಙ್ಗ;

      /* read property value, followed by whitespace (if any) */
      q = ++p;
      ಪ್ರತಿ ( i = 0; i < MAX_LENGTH; i++ )
      {
        ಯದಿ ( !*p || *p == ' ' || *p == '\t' )
          ಭಙ್ಗ;
        property_ಮೌಲ್ಯ[i] = *p++;
      }
      property_ಮೌಲ್ಯ[i] = '\0';

      ಯದಿ ( !( *p == '\0' || *p == ' ' || *p == '\t' ) || p == q )
        ಭಙ್ಗ;

      /* we completely ignore errors */
      ft_property_string_set( library,
                              module_name,
                              property_name,
                              property_ಮೌಲ್ಯ );

      ಯದಿ ( !*p )
        ಭಙ್ಗ;
    }
  }

#ಅನ್ಯಥಾ

  FT_EXPORT_DEF( ಶೂನ್ಯ )
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
    ಯದಿ ( !memory )
    {
      FT_ERROR(( "FT_Init_FreeType: cannot find memory manager\n" ));
      ಪ್ರತ್ಯಾಗಮ FT_THROW( Unimplemented_Feature );
    }

    /* build a library out of it, then fill it with the set of */
    /* default drivers.                                        */

    error = FT_New_Library( memory, alibrary );
    ಯದಿ ( error )
      FT_Done_Memory( memory );
    ಅನ್ಯಥಾ
      FT_Add_Default_Modules( *alibrary );

    FT_Set_Default_Properties( *alibrary );

    ಪ್ರತ್ಯಾಗಮ error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Done_FreeType( FT_Library  library )
  {
    FT_Memory  memory;


    ಯದಿ ( !library )
      ಪ್ರತ್ಯಾಗಮ FT_THROW( Invalid_Library_Handle );

    memory = library->memory;

    /* Discard the library object */
    FT_Done_Library( library );

    /* discard memory manager */
    FT_Done_Memory( memory );

#ifdef FT_DEBUG_LOGGING
    ft_logging_deinit();
#endif

    ಪ್ರತ್ಯಾಗಮ FT_Err_Ok;
  }


/* END */
