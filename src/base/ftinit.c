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


#شامل <ft2build.h>
#شامل FT_CONFIG_CONFIG_H
#شامل <freetype/internal/ftobjs.h>
#شامل <freetype/internal/ftdebug.h>
#شامل <freetype/ftmodapi.h>


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#تعریف_منسوخ  FT_COMPONENT
#تعریف_کرو FT_COMPONENT  init


#تعریف_منسوخ  FT_USE_MODULE
#اگر_تعریف_شدہ __cplusplus
#تعریف_کرو FT_USE_MODULE( type, x )  خارجی "C" ثابت type  x;
#ورنہ
#تعریف_کرو FT_USE_MODULE( type, x )  خارجی ثابت type  x;
#اگر_ختم

#شامل FT_CONFIG_MODULES_H

#تعریف_منسوخ  FT_USE_MODULE
#تعریف_کرو FT_USE_MODULE( type, x )  (ثابت FT_Module_Class*)&(x),

  ساکن
  ثابت FT_Module_Class*  ثابت ft_default_modules[] =
  {
#شامل FT_CONFIG_MODULES_H
    0
  };


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( خالی )
  FT_جوڑو_Default_Modules( FT_Library  library )
  {
    FT_Error                       error;
    ثابت FT_Module_Class* ثابت*  cur;


    /* GCC 4.6 warns the type difference:
     *   FT_Module_Class** != const FT_Module_Class* const*
     */
    cur = (ثابت FT_Module_Class* ثابت*)ft_default_modules;

    /* test for valid `library' delayed to FT_Add_Module() */
    جب ( *cur )
    {
      error = FT_جوڑو_Module( library, *cur );
      /* notify errors, but don't stop */
      اگر ( error )
        FT_TRACE0(( "FT_Add_Default_Module:"
                    " Cannot install `%s', error = 0x%x\n",
                    (*cur)->module_name, error ));
      cur++;
    }
  }


#اگر_تعریف_شدہ FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES

#تعریف_کرو MAX_لمبائی  128

  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( خالی )
  FT_سیٹ_Default_Properties( FT_Library  library )
  {
    ثابت حرف*  env;
    ثابت حرف*  p;
    ثابت حرف*  q;

    حرف  module_name[MAX_لمبائی + 1];
    حرف  property_name[MAX_لمبائی + 1];
    حرف  property_قدر[MAX_لمبائی + 1];

    صحیح  i;


    env = ft_getenv( "FREETYPE_PROPERTIES" );
    اگر ( !env )
      واپس;

    برائے ( p = env; *p; p++ )
    {
      /* skip leading whitespace and separators */
      اگر ( *p == ' ' || *p == '\t' )
        جاری;

      /* read module name, followed by `:' */
      q = p;
      برائے ( i = 0; i < MAX_لمبائی; i++ )
      {
        اگر ( !*p || *p == ':' )
          توڑو;
        module_name[i] = *p++;
      }
      module_name[i] = '\0';

      اگر ( !*p || *p != ':' || p == q )
        توڑو;

      /* read property name, followed by `=' */
      q = ++p;
      برائے ( i = 0; i < MAX_لمبائی; i++ )
      {
        اگر ( !*p || *p == '=' )
          توڑو;
        property_name[i] = *p++;
      }
      property_name[i] = '\0';

      اگر ( !*p || *p != '=' || p == q )
        توڑو;

      /* read property value, followed by whitespace (if any) */
      q = ++p;
      برائے ( i = 0; i < MAX_لمبائی; i++ )
      {
        اگر ( !*p || *p == ' ' || *p == '\t' )
          توڑو;
        property_قدر[i] = *p++;
      }
      property_قدر[i] = '\0';

      اگر ( !( *p == '\0' || *p == ' ' || *p == '\t' ) || p == q )
        توڑو;

      /* we completely ignore errors */
      ft_property_string_سیٹ( library,
                              module_name,
                              property_name,
                              property_قدر );

      اگر ( !*p )
        توڑو;
    }
  }

#ورنہ

  FT_EXPORT_DEF( خالی )
  FT_سیٹ_Default_Properties( FT_Library  library )
  {
    FT_غیر_مستعمل( library );
  }

#اگر_ختم


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Init_FreeType( FT_Library  *alibrary )
  {
    FT_Error   error;
    FT_حافظہ  حافظہ;


#اگر_تعریف_شدہ FT_DEBUG_LOGGING
    ft_logging_init();
#اگر_ختم

    /* check of `alibrary' delayed to `FT_New_Library' */

    /* First of all, allocate a new system object -- this function is part */
    /* of the system-specific component, i.e. `ftsystem.c'.                */

    حافظہ = FT_New_حافظہ();
    اگر ( !حافظہ )
    {
      FT_ERROR(( "FT_Init_FreeType: cannot find memory manager\n" ));
      واپس FT_THROW( Unimplemented_Feature );
    }

    /* build a library out of it, then fill it with the set of */
    /* default drivers.                                        */

    error = FT_New_Library( حافظہ, alibrary );
    اگر ( error )
      FT_مکمل_حافظہ( حافظہ );
    ورنہ
      FT_جوڑو_Default_Modules( *alibrary );

    FT_سیٹ_Default_Properties( *alibrary );

    واپس error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_مکمل_Free_Type( FT_Library  library )
  {
    FT_حافظہ  حافظہ;


    اگر ( !library )
      واپس FT_THROW( Invalid_Library_Handle );

    حافظہ = library->حافظہ;

    /* Discard the library object */
    FT_مکمل_Library( library );

    /* discard memory manager */
    FT_مکمل_حافظہ( حافظہ );

#اگر_تعریف_شدہ FT_DEBUG_LOGGING
    ft_logging_deinit();
#اگر_ختم

    واپس FT_Err_Ok;
  }


/*انتہا*/
