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


#ਅਨ੍ਤਰ੍ਭਾਵ <ft2build.h>
#ਅਨ੍ਤਰ੍ਭਾਵ FT_CONFIG_CONFIG_H
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftobjs.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftdebug.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/ftmodapi.h>


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ  FT_COMPONENT
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FT_COMPONENT  init


#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ  FT_USE_MODULE
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __cplusplus
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FT_USE_MODULE( type, x )  ਬਾਹਰੀ "C" ਅਚਲ type  x;
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FT_USE_MODULE( type, x )  ਬਾਹਰੀ ਅਚਲ type  x;
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ FT_CONFIG_MODULES_H

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ  FT_USE_MODULE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FT_USE_MODULE( type, x )  (ਅਚਲ FT_Module_Class*)&(x),

  ਸਥਿਰ
  ਅਚਲ FT_Module_Class*  ਅਚਲ ft_default_modules[] =
  {
#ਅਨ੍ਤਰ੍ਭਾਵ FT_CONFIG_MODULES_H
    0
  };


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( ਖਾਲੀ )
  FT_ਜੋੜੋ_Default_Modules( FT_Library  library )
  {
    FT_Error                       error;
    ਅਚਲ FT_Module_Class* ਅਚਲ*  cur;


    /* GCC 4.6 warns the type difference:
     *   FT_Module_Class** != const FT_Module_Class* const*
     */
    cur = (ਅਚਲ FT_Module_Class* ਅਚਲ*)ft_default_modules;

    /* test for valid `library' delayed to FT_Add_Module() */
    ਜਦੋਂ ( *cur )
    {
      error = FT_ਜੋੜੋ_Module( library, *cur );
      /* notify errors, but don't stop */
      ਜੇ ( error )
        FT_TRACE0(( "FT_Add_Default_Module:"
                    " Cannot install `%s', error = 0x%x\n",
                    (*cur)->module_name, error ));
      cur++;
    }
  }


#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MAX_ਲੰਮਾਈ  128

  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( ਖਾਲੀ )
  FT_ਸੈੱਟ_Default_Properties( FT_Library  library )
  {
    ਅਚਲ ਅੱਖਰ*  env;
    ਅਚਲ ਅੱਖਰ*  p;
    ਅਚਲ ਅੱਖਰ*  q;

    ਅੱਖਰ  module_name[MAX_ਲੰਮਾਈ + 1];
    ਅੱਖਰ  property_name[MAX_ਲੰਮਾਈ + 1];
    ਅੱਖਰ  property_ਮੁੱਲ[MAX_ਲੰਮਾਈ + 1];

    ਪੂਰਨ  i;


    env = ft_getenv( "FREETYPE_PROPERTIES" );
    ਜੇ ( !env )
      ਵਾਪਸ;

    ਲਈ ( p = env; *p; p++ )
    {
      /* skip leading whitespace and separators */
      ਜੇ ( *p == ' ' || *p == '\t' )
        ਜਾਰੀ;

      /* read module name, followed by `:' */
      q = p;
      ਲਈ ( i = 0; i < MAX_ਲੰਮਾਈ; i++ )
      {
        ਜੇ ( !*p || *p == ':' )
          ਰੋਕੋ;
        module_name[i] = *p++;
      }
      module_name[i] = '\0';

      ਜੇ ( !*p || *p != ':' || p == q )
        ਰੋਕੋ;

      /* read property name, followed by `=' */
      q = ++p;
      ਲਈ ( i = 0; i < MAX_ਲੰਮਾਈ; i++ )
      {
        ਜੇ ( !*p || *p == '=' )
          ਰੋਕੋ;
        property_name[i] = *p++;
      }
      property_name[i] = '\0';

      ਜੇ ( !*p || *p != '=' || p == q )
        ਰੋਕੋ;

      /* read property value, followed by whitespace (if any) */
      q = ++p;
      ਲਈ ( i = 0; i < MAX_ਲੰਮਾਈ; i++ )
      {
        ਜੇ ( !*p || *p == ' ' || *p == '\t' )
          ਰੋਕੋ;
        property_ਮੁੱਲ[i] = *p++;
      }
      property_ਮੁੱਲ[i] = '\0';

      ਜੇ ( !( *p == '\0' || *p == ' ' || *p == '\t' ) || p == q )
        ਰੋਕੋ;

      /* we completely ignore errors */
      ft_property_string_ਸੈੱਟ( library,
                              module_name,
                              property_name,
                              property_ਮੁੱਲ );

      ਜੇ ( !*p )
        ਰੋਕੋ;
    }
  }

#ਨਹੀਂਤਾਂ

  FT_EXPORT_DEF( ਖਾਲੀ )
  FT_ਸੈੱਟ_Default_Properties( FT_Library  library )
  {
    FT_ਅਣਵਰਤਿਆ( library );
  }

#ਜੇ_ਅੰਤ


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Init_FreeType( FT_Library  *alibrary )
  {
    FT_Error   error;
    FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ;


#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ FT_DEBUG_LOGGING
    ft_logging_init();
#ਜੇ_ਅੰਤ

    /* check of `alibrary' delayed to `FT_New_Library' */

    /* First of all, allocate a new system object -- this function is part */
    /* of the system-specific component, i.e. `ftsystem.c'.                */

    ਸਮ੍ਰਿਤੀ = FT_New_ਸਮ੍ਰਿਤੀ();
    ਜੇ ( !ਸਮ੍ਰਿਤੀ )
    {
      FT_ERROR(( "FT_Init_FreeType: cannot find memory manager\n" ));
      ਵਾਪਸ FT_THROW( Unimplemented_Feature );
    }

    /* build a library out of it, then fill it with the set of */
    /* default drivers.                                        */

    error = FT_New_Library( ਸਮ੍ਰਿਤੀ, alibrary );
    ਜੇ ( error )
      FT_ਪੂਰਾ_ਸਮ੍ਰਿਤੀ( ਸਮ੍ਰਿਤੀ );
    ਨਹੀਂਤਾਂ
      FT_ਜੋੜੋ_Default_Modules( *alibrary );

    FT_ਸੈੱਟ_Default_Properties( *alibrary );

    ਵਾਪਸ error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_ਪੂਰਾ_Free_Type( FT_Library  library )
  {
    FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ;


    ਜੇ ( !library )
      ਵਾਪਸ FT_THROW( Invalid_Library_Handle );

    ਸਮ੍ਰਿਤੀ = library->ਸਮ੍ਰਿਤੀ;

    /* Discard the library object */
    FT_ਪੂਰਾ_Library( library );

    /* discard memory manager */
    FT_ਪੂਰਾ_ਸਮ੍ਰਿਤੀ( ਸਮ੍ਰਿਤੀ );

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ FT_DEBUG_LOGGING
    ft_logging_deinit();
#ਜੇ_ਅੰਤ

    ਵਾਪਸ FT_Err_Ok;
  }


/*ਸਿਰਾ*/
