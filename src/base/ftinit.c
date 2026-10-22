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


#অন্তর্ভাব <ft2build.h>
#অন্তর্ভাব FT_CONFIG_CONFIG_H
#অন্তর্ভাব <freetype/internal/ftobjs.h>
#অন্তর্ভাব <freetype/internal/ftdebug.h>
#অন্তর্ভাব <freetype/ftmodapi.h>


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
#define FT_USE_MODULE( type, x )  বাহ্যিক "C" ধ্রুবক type  x;
#অন্যথা
#define FT_USE_MODULE( type, x )  বাহ্যিক ধ্রুবক type  x;
#endif

#অন্তর্ভাব FT_CONFIG_MODULES_H

#undef  FT_USE_MODULE
#define FT_USE_MODULE( type, x )  (ধ্রুবক FT_Module_Class*)&(x),

  স্থির
  ধ্রুবক FT_Module_Class*  ধ্রুবক ft_default_modules[] =
  {
#অন্তর্ভাব FT_CONFIG_MODULES_H
    0
  };


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( শূন্য )
  FT_Add_Default_Modules( FT_Library  library )
  {
    FT_Error                       error;
    ধ্রুবক FT_Module_Class* ধ্রুবক*  cur;


    /* GCC 4.6 warns the type difference:
     *   FT_Module_Class** != const FT_Module_Class* const*
     */
    cur = (ধ্রুবক FT_Module_Class* ধ্রুবক*)ft_default_modules;

    /* test for valid `library' delayed to FT_Add_Module() */
    যখন ( *cur )
    {
      error = FT_Add_Module( library, *cur );
      /* notify errors, but don't stop */
      যদি ( error )
        FT_TRACE0(( "FT_Add_Default_Module:"
                    " Cannot install `%s', error = 0x%x\n",
                    (*cur)->module_name, error ));
      cur++;
    }
  }


#ifdef FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES

#define MAX_দৈর্ঘ্য  128

  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( শূন্য )
  FT_Set_Default_Properties( FT_Library  library )
  {
    ধ্রুবক অক্ষর*  env;
    ধ্রুবক অক্ষর*  p;
    ধ্রুবক অক্ষর*  q;

    অক্ষর  module_name[MAX_দৈর্ঘ্য + 1];
    অক্ষর  property_name[MAX_দৈর্ঘ্য + 1];
    অক্ষর  property_মান[MAX_দৈর্ঘ্য + 1];

    পূর্ণসংখ্যা  i;


    env = ft_getenv( "FREETYPE_PROPERTIES" );
    যদি ( !env )
      ফেরত;

    জন্য ( p = env; *p; p++ )
    {
      /* skip leading whitespace and separators */
      যদি ( *p == ' ' || *p == '\t' )
        চালু;

      /* read module name, followed by `:' */
      q = p;
      জন্য ( i = 0; i < MAX_দৈর্ঘ্য; i++ )
      {
        যদি ( !*p || *p == ':' )
          বিরতি;
        module_name[i] = *p++;
      }
      module_name[i] = '\0';

      যদি ( !*p || *p != ':' || p == q )
        বিরতি;

      /* read property name, followed by `=' */
      q = ++p;
      জন্য ( i = 0; i < MAX_দৈর্ঘ্য; i++ )
      {
        যদি ( !*p || *p == '=' )
          বিরতি;
        property_name[i] = *p++;
      }
      property_name[i] = '\0';

      যদি ( !*p || *p != '=' || p == q )
        বিরতি;

      /* read property value, followed by whitespace (if any) */
      q = ++p;
      জন্য ( i = 0; i < MAX_দৈর্ঘ্য; i++ )
      {
        যদি ( !*p || *p == ' ' || *p == '\t' )
          বিরতি;
        property_মান[i] = *p++;
      }
      property_মান[i] = '\0';

      যদি ( !( *p == '\0' || *p == ' ' || *p == '\t' ) || p == q )
        বিরতি;

      /* we completely ignore errors */
      ft_property_string_set( library,
                              module_name,
                              property_name,
                              property_মান );

      যদি ( !*p )
        বিরতি;
    }
  }

#অন্যথা

  FT_EXPORT_DEF( শূন্য )
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
    যদি ( !memory )
    {
      FT_ERROR(( "FT_Init_FreeType: cannot find memory manager\n" ));
      ফেরত FT_THROW( Unimplemented_Feature );
    }

    /* build a library out of it, then fill it with the set of */
    /* default drivers.                                        */

    error = FT_New_Library( memory, alibrary );
    যদি ( error )
      FT_Done_Memory( memory );
    অন্যথা
      FT_Add_Default_Modules( *alibrary );

    FT_Set_Default_Properties( *alibrary );

    ফেরত error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Done_FreeType( FT_Library  library )
  {
    FT_Memory  memory;


    যদি ( !library )
      ফেরত FT_THROW( Invalid_Library_Handle );

    memory = library->memory;

    /* Discard the library object */
    FT_Done_Library( library );

    /* discard memory manager */
    FT_Done_Memory( memory );

#ifdef FT_DEBUG_LOGGING
    ft_logging_deinit();
#endif

    ফেরত FT_Err_Ok;
  }


/* END */
