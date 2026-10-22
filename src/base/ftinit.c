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


#ضمن <ft2build.h>
#ضمن FT_CONFIG_CONFIG_H
#ضمن <freetype/internal/ftobjs.h>
#ضمن <freetype/internal/ftdebug.h>
#ضمن <freetype/ftmodapi.h>


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#تعریف_رد  FT_COMPONENT
#تعریف_کرو FT_COMPONENT  init


#تعریف_رد  FT_USE_MODULE
#جے_تعریف_شدہ __cplusplus
#تعریف_کرو FT_USE_MODULE( type, x )  خارجي "C" ثابت type  x;
#وإلا
#تعریف_کرو FT_USE_MODULE( type, x )  خارجي ثابت type  x;
#جے_انت

#ضمن FT_CONFIG_MODULES_H

#تعریف_رد  FT_USE_MODULE
#تعریف_کرو FT_USE_MODULE( type, x )  (ثابت FT_Module_Class*)&(x),

  ساكن
  ثابت FT_Module_Class*  ثابت ft_default_modules[] =
  {
#ضمن FT_CONFIG_MODULES_H
    0
  };


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( فراغ )
  FT_جوڑو_Default_Modules( FT_Library  library )
  {
    FT_Error                       error;
    ثابت FT_Module_Class* ثابت*  cur;


    /* GCC 4.6 warns the type difference:
     *   FT_Module_Class** != const FT_Module_Class* const*
     */
    cur = (ثابت FT_Module_Class* ثابت*)ft_default_modules;

    /* test for valid `library' delayed to FT_Add_Module() */
    طالما ( *cur )
    {
      error = FT_جوڑو_Module( library, *cur );
      /* notify errors, but don't stop */
      إذا ( error )
        FT_TRACE0(( "FT_Add_Default_Module:"
                    " Cannot install `%s', error = 0x%x\n",
                    (*cur)->module_name, error ));
      cur++;
    }
  }


#جے_تعریف_شدہ FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES

#تعریف_کرو MAX_لمائی  128

  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( فراغ )
  FT_سیٹ_Default_Properties( FT_Library  library )
  {
    ثابت حرف*  env;
    ثابت حرف*  p;
    ثابت حرف*  q;

    حرف  module_name[MAX_لمائی + 1];
    حرف  property_name[MAX_لمائی + 1];
    حرف  property_قدر[MAX_لمائی + 1];

    صحيح  i;


    env = ft_getenv( "FREETYPE_PROPERTIES" );
    إذا ( !env )
      أرجع;

    لكل ( p = env; *p; p++ )
    {
      /* skip leading whitespace and separators */
      إذا ( *p == ' ' || *p == '\t' )
        استمر;

      /* read module name, followed by `:' */
      q = p;
      لكل ( i = 0; i < MAX_لمائی; i++ )
      {
        إذا ( !*p || *p == ':' )
          اكسر;
        module_name[i] = *p++;
      }
      module_name[i] = '\0';

      إذا ( !*p || *p != ':' || p == q )
        اكسر;

      /* read property name, followed by `=' */
      q = ++p;
      لكل ( i = 0; i < MAX_لمائی; i++ )
      {
        إذا ( !*p || *p == '=' )
          اكسر;
        property_name[i] = *p++;
      }
      property_name[i] = '\0';

      إذا ( !*p || *p != '=' || p == q )
        اكسر;

      /* read property value, followed by whitespace (if any) */
      q = ++p;
      لكل ( i = 0; i < MAX_لمائی; i++ )
      {
        إذا ( !*p || *p == ' ' || *p == '\t' )
          اكسر;
        property_قدر[i] = *p++;
      }
      property_قدر[i] = '\0';

      إذا ( !( *p == '\0' || *p == ' ' || *p == '\t' ) || p == q )
        اكسر;

      /* we completely ignore errors */
      ft_property_string_سیٹ( library,
                              module_name,
                              property_name,
                              property_قدر );

      إذا ( !*p )
        اكسر;
    }
  }

#وإلا

  FT_EXPORT_DEF( فراغ )
  FT_سیٹ_Default_Properties( FT_Library  library )
  {
    FT_بن_ورتیا( library );
  }

#جے_انت


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Init_FreeType( FT_Library  *alibrary )
  {
    FT_Error   error;
    FT_حافظہ  حافظہ;


#جے_تعریف_شدہ FT_DEBUG_LOGGING
    ft_logging_init();
#جے_انت

    /* check of `alibrary' delayed to `FT_New_Library' */

    /* First of all, allocate a new system object -- this function is part */
    /* of the system-specific component, i.e. `ftsystem.c'.                */

    حافظہ = FT_New_حافظہ();
    إذا ( !حافظہ )
    {
      FT_ERROR(( "FT_Init_FreeType: cannot find memory manager\n" ));
      أرجع FT_THROW( Unimplemented_Feature );
    }

    /* build a library out of it, then fill it with the set of */
    /* default drivers.                                        */

    error = FT_New_Library( حافظہ, alibrary );
    إذا ( error )
      FT_پورا_حافظہ( حافظہ );
    وإلا
      FT_جوڑو_Default_Modules( *alibrary );

    FT_سیٹ_Default_Properties( *alibrary );

    أرجع error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_پورا_Free_Type( FT_Library  library )
  {
    FT_حافظہ  حافظہ;


    إذا ( !library )
      أرجع FT_THROW( Invalid_Library_Handle );

    حافظہ = library->حافظہ;

    /* Discard the library object */
    FT_پورا_Library( library );

    /* discard memory manager */
    FT_پورا_حافظہ( حافظہ );

#جے_تعریف_شدہ FT_DEBUG_LOGGING
    ft_logging_deinit();
#جے_انت

    أرجع FT_Err_Ok;
  }


/*سرا*/
