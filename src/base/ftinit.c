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


#அந்தர்பாவ <ft2build.h>
#அந்தர்பாவ FT_CONFIG_CONFIG_H
#அந்தர்பாவ <freetype/internal/ftobjs.h>
#அந்தர்பாவ <freetype/internal/ftdebug.h>
#அந்தர்பாவ <freetype/ftmodapi.h>


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
#define FT_USE_MODULE( type, x )  வெளி "C" மாறிலி type  x;
#இல்லையெனில்
#define FT_USE_MODULE( type, x )  வெளி மாறிலி type  x;
#endif

#அந்தர்பாவ FT_CONFIG_MODULES_H

#undef  FT_USE_MODULE
#define FT_USE_MODULE( type, x )  (மாறிலி FT_Module_Class*)&(x),

  நிலையான
  மாறிலி FT_Module_Class*  மாறிலி ft_default_modules[] =
  {
#அந்தர்பாவ FT_CONFIG_MODULES_H
    0
  };


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( வெறுமை )
  FT_Add_Default_Modules( FT_Library  library )
  {
    FT_Error                       error;
    மாறிலி FT_Module_Class* மாறிலி*  cur;


    /* GCC 4.6 warns the type difference:
     *   FT_Module_Class** != const FT_Module_Class* const*
     */
    cur = (மாறிலி FT_Module_Class* மாறிலி*)ft_default_modules;

    /* test for valid `library' delayed to FT_Add_Module() */
    போது ( *cur )
    {
      error = FT_Add_Module( library, *cur );
      /* notify errors, but don't stop */
      எனில் ( error )
        FT_TRACE0(( "FT_Add_Default_Module:"
                    " Cannot install `%s', error = 0x%x\n",
                    (*cur)->module_name, error ));
      cur++;
    }
  }


#ifdef FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES

#define MAX_LENGTH  128

  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( வெறுமை )
  FT_Set_Default_Properties( FT_Library  library )
  {
    மாறிலி எழுத்து*  env;
    மாறிலி எழுத்து*  p;
    மாறிலி எழுத்து*  q;

    எழுத்து  module_name[MAX_LENGTH + 1];
    எழுத்து  property_name[MAX_LENGTH + 1];
    எழுத்து  property_மதிப்பு[MAX_LENGTH + 1];

    முழு  i;


    env = ft_getenv( "FREETYPE_PROPERTIES" );
    எனில் ( !env )
      திரும்பு;

    க்கு ( p = env; *p; p++ )
    {
      /* skip leading whitespace and separators */
      எனில் ( *p == ' ' || *p == '\t' )
        தொடர்;

      /* read module name, followed by `:' */
      q = p;
      க்கு ( i = 0; i < MAX_LENGTH; i++ )
      {
        எனில் ( !*p || *p == ':' )
          நிறுத்து;
        module_name[i] = *p++;
      }
      module_name[i] = '\0';

      எனில் ( !*p || *p != ':' || p == q )
        நிறுத்து;

      /* read property name, followed by `=' */
      q = ++p;
      க்கு ( i = 0; i < MAX_LENGTH; i++ )
      {
        எனில் ( !*p || *p == '=' )
          நிறுத்து;
        property_name[i] = *p++;
      }
      property_name[i] = '\0';

      எனில் ( !*p || *p != '=' || p == q )
        நிறுத்து;

      /* read property value, followed by whitespace (if any) */
      q = ++p;
      க்கு ( i = 0; i < MAX_LENGTH; i++ )
      {
        எனில் ( !*p || *p == ' ' || *p == '\t' )
          நிறுத்து;
        property_மதிப்பு[i] = *p++;
      }
      property_மதிப்பு[i] = '\0';

      எனில் ( !( *p == '\0' || *p == ' ' || *p == '\t' ) || p == q )
        நிறுத்து;

      /* we completely ignore errors */
      ft_property_string_set( library,
                              module_name,
                              property_name,
                              property_மதிப்பு );

      எனில் ( !*p )
        நிறுத்து;
    }
  }

#இல்லையெனில்

  FT_EXPORT_DEF( வெறுமை )
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
    எனில் ( !memory )
    {
      FT_ERROR(( "FT_Init_FreeType: cannot find memory manager\n" ));
      திரும்பு FT_THROW( Unimplemented_Feature );
    }

    /* build a library out of it, then fill it with the set of */
    /* default drivers.                                        */

    error = FT_New_Library( memory, alibrary );
    எனில் ( error )
      FT_Done_Memory( memory );
    இல்லையெனில்
      FT_Add_Default_Modules( *alibrary );

    FT_Set_Default_Properties( *alibrary );

    திரும்பு error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Done_FreeType( FT_Library  library )
  {
    FT_Memory  memory;


    எனில் ( !library )
      திரும்பு FT_THROW( Invalid_Library_Handle );

    memory = library->memory;

    /* Discard the library object */
    FT_Done_Library( library );

    /* discard memory manager */
    FT_Done_Memory( memory );

#ifdef FT_DEBUG_LOGGING
    ft_logging_deinit();
#endif

    திரும்பு FT_Err_Ok;
  }


/* END */
