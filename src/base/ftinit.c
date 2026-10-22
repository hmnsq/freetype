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
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು  FT_COMPONENT
#ವ್ಯಾಖ್ಯಾನಿಸು FT_COMPONENT  init


#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು  FT_USE_MODULE
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __cplusplus
#ವ್ಯಾಖ್ಯಾನಿಸು FT_USE_MODULE( type, x )  ಬಾಹ್ಯ "C" ಸ್ಥಿರಾಂಕ type  x;
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು FT_USE_MODULE( type, x )  ಬಾಹ್ಯ ಸ್ಥಿರಾಂಕ type  x;
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ FT_CONFIG_MODULES_H

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು  FT_USE_MODULE
#ವ್ಯಾಖ್ಯಾನಿಸು FT_USE_MODULE( type, x )  (ಸ್ಥಿರಾಂಕ FT_Module_Class*)&(x),

  ಸ್ಥಿರ
  ಸ್ಥಿರಾಂಕ FT_Module_Class*  ಸ್ಥಿರಾಂಕ ft_default_modules[] =
  {
#ಅನ್ತರ್ಭಾವ FT_CONFIG_MODULES_H
    0
  };


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( ಶೂನ್ಯ )
  FT_ಕೂಡಿಸು_Default_Modules( FT_Library  library )
  {
    FT_Error                       error;
    ಸ್ಥಿರಾಂಕ FT_Module_Class* ಸ್ಥಿರಾಂಕ*  cur;


    /* GCC 4.6 warns the type difference:
     *   FT_Module_Class** != const FT_Module_Class* const*
     */
    cur = (ಸ್ಥಿರಾಂಕ FT_Module_Class* ಸ್ಥಿರಾಂಕ*)ft_default_modules;

    /* test for valid `library' delayed to FT_Add_Module() */
    ಆಗ ( *cur )
    {
      error = FT_ಕೂಡಿಸು_Module( library, *cur );
      /* notify errors, but don't stop */
      ಆದರೆ ( error )
        FT_TRACE0(( "FT_Add_Default_Module:"
                    " Cannot install `%s', error = 0x%x\n",
                    (*cur)->module_name, error ));
      cur++;
    }
  }


#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES

#ವ್ಯಾಖ್ಯಾನಿಸು MAX_ಉದ್ದಳತೆ  128

  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( ಶೂನ್ಯ )
  FT_ಗಣ_Default_Properties( FT_Library  library )
  {
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*  env;
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*  p;
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*  q;

    ಅಕ್ಷರ  module_name[MAX_ಉದ್ದಳತೆ + 1];
    ಅಕ್ಷರ  property_name[MAX_ಉದ್ದಳತೆ + 1];
    ಅಕ್ಷರ  property_ಮೌಲ್ಯ[MAX_ಉದ್ದಳತೆ + 1];

    ಪೂರ್ಣಾಂಕ  i;


    env = ft_getenv( "FREETYPE_PROPERTIES" );
    ಆದರೆ ( !env )
      ಹಿಂತಿರುಗಿ;

    ಗಾಗಿ ( p = env; *p; p++ )
    {
      /* skip leading whitespace and separators */
      ಆದರೆ ( *p == ' ' || *p == '\t' )
        ಮುಂದುವರಿ;

      /* read module name, followed by `:' */
      q = p;
      ಗಾಗಿ ( i = 0; i < MAX_ಉದ್ದಳತೆ; i++ )
      {
        ಆದರೆ ( !*p || *p == ':' )
          ನಿಲ್ಲಿಸು;
        module_name[i] = *p++;
      }
      module_name[i] = '\0';

      ಆದರೆ ( !*p || *p != ':' || p == q )
        ನಿಲ್ಲಿಸು;

      /* read property name, followed by `=' */
      q = ++p;
      ಗಾಗಿ ( i = 0; i < MAX_ಉದ್ದಳತೆ; i++ )
      {
        ಆದರೆ ( !*p || *p == '=' )
          ನಿಲ್ಲಿಸು;
        property_name[i] = *p++;
      }
      property_name[i] = '\0';

      ಆದರೆ ( !*p || *p != '=' || p == q )
        ನಿಲ್ಲಿಸು;

      /* read property value, followed by whitespace (if any) */
      q = ++p;
      ಗಾಗಿ ( i = 0; i < MAX_ಉದ್ದಳತೆ; i++ )
      {
        ಆದರೆ ( !*p || *p == ' ' || *p == '\t' )
          ನಿಲ್ಲಿಸು;
        property_ಮೌಲ್ಯ[i] = *p++;
      }
      property_ಮೌಲ್ಯ[i] = '\0';

      ಆದರೆ ( !( *p == '\0' || *p == ' ' || *p == '\t' ) || p == q )
        ನಿಲ್ಲಿಸು;

      /* we completely ignore errors */
      ft_property_string_ಗಣ( library,
                              module_name,
                              property_name,
                              property_ಮೌಲ್ಯ );

      ಆದರೆ ( !*p )
        ನಿಲ್ಲಿಸು;
    }
  }

#ಇಲ್ಲದಿದ್ದರೆ

  FT_EXPORT_DEF( ಶೂನ್ಯ )
  FT_ಗಣ_Default_Properties( FT_Library  library )
  {
    FT_ಬಳಸದ( library );
  }

#ಆದರೆ_ಅಂತ್ಯ


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Init_FreeType( FT_Library  *alibrary )
  {
    FT_Error   error;
    FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ;


#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ FT_DEBUG_LOGGING
    ft_logging_init();
#ಆದರೆ_ಅಂತ್ಯ

    /* check of `alibrary' delayed to `FT_New_Library' */

    /* First of all, allocate a new system object -- this function is part */
    /* of the system-specific component, i.e. `ftsystem.c'.                */

    ಸ್ಮರಣೆ = FT_New_ಸ್ಮರಣೆ();
    ಆದರೆ ( !ಸ್ಮರಣೆ )
    {
      FT_ERROR(( "FT_Init_FreeType: cannot find memory manager\n" ));
      ಹಿಂತಿರುಗಿ FT_THROW( Unimplemented_Feature );
    }

    /* build a library out of it, then fill it with the set of */
    /* default drivers.                                        */

    error = FT_New_Library( ಸ್ಮರಣೆ, alibrary );
    ಆದರೆ ( error )
      FT_ಮುಗಿದ_ಸ್ಮರಣೆ( ಸ್ಮರಣೆ );
    ಇಲ್ಲದಿದ್ದರೆ
      FT_ಕೂಡಿಸು_Default_Modules( *alibrary );

    FT_ಗಣ_Default_Properties( *alibrary );

    ಹಿಂತಿರುಗಿ error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_ಮುಗಿದ_Free_Type( FT_Library  library )
  {
    FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ;


    ಆದರೆ ( !library )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_Library_Handle );

    ಸ್ಮರಣೆ = library->ಸ್ಮರಣೆ;

    /* Discard the library object */
    FT_ಮುಗಿದ_Library( library );

    /* discard memory manager */
    FT_ಮುಗಿದ_ಸ್ಮರಣೆ( ಸ್ಮರಣೆ );

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ FT_DEBUG_LOGGING
    ft_logging_deinit();
#ಆದರೆ_ಅಂತ್ಯ

    ಹಿಂತಿರುಗಿ FT_Err_Ok;
  }


/*ಕೊನೆ*/
