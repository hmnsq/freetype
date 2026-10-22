/****************************************************************************
 *
 * ftdebug.c
 *
 *   Debugging and logging component (body).
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
   * This component contains various macros and functions used to ease the
   * debugging of the FreeType engine.  Its main purpose is in assertion
   * checking, tracing, and error detection.
   *
   * There are now three debugging modes:
   *
   * - trace mode
   *
   *   Error and trace messages are sent to the log file (which can be the
   *   standard error output).
   *
   * - error mode
   *
   *   Only error messages are generated.
   *
   * - release mode:
   *
   *   No error message is sent or generated.  The code is free from any
   *   debugging parts.
   *
   */


#समावेश <freetype/freetype.h>
#समावेश <freetype/ftlogging.h>
#समावेश <freetype/internal/ftdebug.h>
#समावेश <freetype/internal/ftobjs.h>


#अगर_घोषित FT_DEBUG_LOGGING

  /**************************************************************************
   *
   * Variables used to control logging.
   *
   * 1. `ft_default_trace_level` stores the value of trace levels, which are
   *    provided to FreeType using the `FT2_DEBUG` environment variable.
   *
   * 2. `ft_fileptr` stores the `FILE*` handle.
   *
   * 3. `ft_component` is a string that holds the name of `FT_COMPONENT`.
   *
   * 4. The flag `ft_component_flag` prints the name of `FT_COMPONENT` along
   *    with the actual log message if set to true.
   *
   * 5. The flag `ft_timestamp_flag` prints time along with the actual log
   *    message if set to true.
   *
   * 6. `ft_have_newline_char` is used to differentiate between a log
   *    message with and without a trailing newline character.
   *
   * 7. `ft_custom_trace_level` stores the custom trace level value, which
   *    is provided by the user at run-time.
   *
   * We use `static` to avoid 'unused variable' warnings.
   *
   */
  अटल स्थिर अक्षर*  ft_default_trace_level = शून्य;
  अटल खाता*        ft_fileptr             = शून्य;
  अटल स्थिर अक्षर*  ft_component           = शून्य;
  अटल FT_Bool      ft_component_flag      = FALSE;
  अटल FT_Bool      ft_timestamp_flag      = FALSE;
  अटल FT_Bool      ft_have_newline_char   = TRUE;
  अटल स्थिर अक्षर*  ft_custom_trace_level  = शून्य;

  /* declared in ftdebug.h */

  dlg_handler            ft_default_log_handler = शून्य;
  FT_Custom_Log_Handler  custom_output_handler  = शून्य;

#पूर्ण_अगर /* FT_DEBUG_LOGGING */


#अगर_घोषित FT_DEBUG_LEVEL_ERROR

  /* documentation is in ftdebug.h */

  FT_BASE_DEF( व्योम )
  FT_Message( स्थिर अक्षर*  fmt,
              ... )
  {
    बहु_सूची  ap;


    बहु_शुरू( ap, fmt );
    भख_लिखो( मानक_त्रुटि, fmt, ap );
    बहु_पूर्ण( ap );
  }


  /* documentation is in ftdebug.h */

  FT_BASE_DEF( व्योम )
  FT_Panic( स्थिर अक्षर*  fmt,
            ... )
  {
    बहु_सूची  ap;


    बहु_शुरू( ap, fmt );
    भख_लिखो( मानक_त्रुटि, fmt, ap );
    बहु_पूर्ण( ap );

    निकास( निकास_त्रुटि );
  }


  /* documentation is in ftdebug.h */

  FT_BASE_DEF( पूर्णांक )
  FT_Throw( FT_Error     error,
            पूर्णांक          पंक्ति,
            स्थिर अक्षर*  file )
  {
#अगर 0
    /* activating the code in this block makes FreeType very chatty */
    ख_लिखो( मानक_त्रुटि,
             "%s:%d: error 0x%02x: %s\n",
             file,
             पंक्ति,
             error,
             FT_Error_String( error ) );
#अन्यथा
    FT_UNUSED( error );
    FT_UNUSED( पंक्ति );
    FT_UNUSED( file );
#पूर्ण_अगर

    वापस 0;
  }

#पूर्ण_अगर /* FT_DEBUG_LEVEL_ERROR */


#अगर_घोषित FT_DEBUG_LEVEL_TRACE

  /* array of trace levels, initialized to 0; */
  /* this gets adjusted at run-time           */
  अटल पूर्णांक  ft_trace_levels_enabled[trace_गिनती];

  /* array of trace levels, always initialized to 0 */
  अटल पूर्णांक  ft_trace_levels_disabled[trace_गिनती];

  /* a pointer to either `ft_trace_levels_enabled' */
  /* or `ft_trace_levels_disabled'                 */
  पूर्णांक*  ft_trace_levels;

  /* define array of trace toggle names */
#घोषणा FT_TRACE_DEF( x )  #x ,

  अटल स्थिर अक्षर*  ft_trace_toggles[trace_गिनती + 1] =
  {
#समावेश <freetype/internal/fttrace.h>
    शून्य
  };

#अघोषित FT_TRACE_DEF


  /* documentation is in ftdebug.h */

  FT_BASE_DEF( FT_Int )
  FT_Trace_Get_गिनती( व्योम )
  {
    वापस trace_गिनती;
  }


  /* documentation is in ftdebug.h */

  FT_BASE_DEF( स्थिर अक्षर * )
  FT_Trace_Get_Name( FT_Int  idx )
  {
    पूर्णांक  max = FT_Trace_Get_गिनती();


    अगर ( idx < max )
      वापस ft_trace_toggles[idx];
    अन्यथा
      वापस शून्य;
  }


  /* documentation is in ftdebug.h */

  FT_BASE_DEF( व्योम )
  FT_Trace_Disable( व्योम )
  {
    ft_trace_levels = ft_trace_levels_disabled;
  }


  /* documentation is in ftdebug.h */

  FT_BASE_DEF( व्योम )
  FT_Trace_Enable( व्योम )
  {
    ft_trace_levels = ft_trace_levels_enabled;
  }


  /**************************************************************************
   *
   * Initialize the tracing sub-system.  This is done by retrieving the
   * value of the `FT2_DEBUG' environment variable.  It must be a list of
   * toggles, separated by spaces, `;', or `,'.  Example:
   *
   *   export FT2_DEBUG="any:3 memory:7 stream:5"
   *
   * This requests that all levels be set to 3, except the trace level for
   * the memory and stream components which are set to 7 and 5,
   * respectively.
   *
   * See the file `include/freetype/internal/fttrace.h' for details of
   * the available toggle names.
   *
   * The level must be between 0 and 7; 0 means quiet (except for serious
   * runtime errors), and 7 means _very_ verbose.
   */
  FT_BASE_DEF( व्योम )
  ft_debug_init( व्योम )
  {
    स्थिर अक्षर*  ft2_debug = शून्य;


#अगर_घोषित FT_DEBUG_LOGGING
    अगर ( ft_custom_trace_level != शून्य )
      ft2_debug = ft_custom_trace_level;
    अन्यथा
      ft2_debug = ft_default_trace_level;
#अन्यथा
    ft2_debug = ft_getenv( "FT2_DEBUG" );
#पूर्ण_अगर

    अगर ( ft2_debug )
    {
      स्थिर अक्षर*  p = ft2_debug;
      स्थिर अक्षर*  q;


      क्रम ( ; *p; p++ )
      {
        /* skip leading whitespace and separators */
        अगर ( *p == ' ' || *p == '\t' || *p == ',' || *p == ';' || *p == '=' )
          जारी;

#अगर_घोषित FT_DEBUG_LOGGING

        /* check extra arguments for logging */
        अगर ( *p == '-' )
        {
          स्थिर अक्षर*  r = ++p;


          अगर ( *r == 'v' )
          {
            स्थिर अक्षर*  s = ++r;


            ft_component_flag = TRUE;

            अगर ( *s == 't' )
            {
              ft_timestamp_flag = TRUE;
              p++;
            }

            p++;
          }

          अन्यथा अगर ( *r == 't' )
          {
            स्थिर अक्षर*  s = ++r;


            ft_timestamp_flag = TRUE;

            अगर ( *s == 'v' )
            {
              ft_component_flag = TRUE;
              p++;
            }

            p++;
          }
        }

#पूर्ण_अगर /* FT_DEBUG_LOGGING */

        /* read toggle name, followed by ':' */
        q = p;
        जबतक ( *p && *p != ':' )
          p++;

        अगर ( !*p )
          अवरोध;

        अगर ( *p == ':' && p > q )
        {
          FT_Int  n, i, len = (FT_Int)( p - q );
          FT_Int  level = -1, found = -1;


          क्रम ( n = 0; n < trace_गिनती; n++ )
          {
            स्थिर अक्षर*  toggle = ft_trace_toggles[n];


            क्रम ( i = 0; i < len; i++ )
            {
              अगर ( toggle[i] != q[i] )
                अवरोध;
            }

            अगर ( i == len && toggle[i] == 0 )
            {
              found = n;
              अवरोध;
            }
          }

          /* read level */
          p++;
          अगर ( *p )
          {
            level = *p - '0';
            अगर ( level < 0 || level > 7 )
              level = -1;
          }

          अगर ( found >= 0 && level >= 0 )
          {
            अगर ( found == trace_any )
            {
              /* special case for `any' */
              क्रम ( n = 0; n < trace_गिनती; n++ )
                ft_trace_levels_enabled[n] = level;
            }
            अन्यथा
              ft_trace_levels_enabled[found] = level;
          }
        }
      }
    }

    ft_trace_levels = ft_trace_levels_enabled;
  }


#अन्यथा  /* !FT_DEBUG_LEVEL_TRACE */


  FT_BASE_DEF( व्योम )
  ft_debug_init( व्योम )
  {
    /* nothing */
  }


  FT_BASE_DEF( FT_Int )
  FT_Trace_Get_गिनती( व्योम )
  {
    वापस 0;
  }


  FT_BASE_DEF( स्थिर अक्षर * )
  FT_Trace_Get_Name( FT_Int  idx )
  {
    FT_UNUSED( idx );

    वापस शून्य;
  }


  FT_BASE_DEF( व्योम )
  FT_Trace_Disable( व्योम )
  {
    /* nothing */
  }


  /* documentation is in ftdebug.h */

  FT_BASE_DEF( व्योम )
  FT_Trace_Enable( व्योम )
  {
    /* nothing */
  }

#पूर्ण_अगर /* !FT_DEBUG_LEVEL_TRACE */


#अगर_घोषित FT_DEBUG_LOGGING

  /**************************************************************************
   *
   * Initialize and de-initialize 'dlg' library.
   *
   */

  FT_BASE_DEF( व्योम )
  ft_logging_init( व्योम )
  {
    ft_default_log_handler = ft_log_handler;
    ft_default_trace_level = ft_getenv( "FT2_DEBUG" );

    अगर ( ft_getenv( "FT_LOGGING_FILE" ) )
      ft_fileptr = ft_fopen( ft_getenv( "FT_LOGGING_FILE" ), "w" );
    अन्यथा
      ft_fileptr = मानक_त्रुटि;

    ft_debug_init();

    /* Set the default output handler for 'dlg'. */
    dlg_set_handler( ft_default_log_handler, शून्य );
  }


  FT_BASE_DEF( व्योम )
  ft_logging_deinit( व्योम )
  {
    अगर ( ft_fileptr != मानक_त्रुटि )
      ft_fclose( ft_fileptr );
  }


  /**************************************************************************
   *
   * An output log handler for FreeType.
   *
   */
  FT_BASE_DEF( व्योम )
  ft_log_handler( स्थिर काष्ठा dlg_origin*  origin,
                  स्थिर अक्षर*               string,
                  व्योम*                     तथ्य )
  {
    अक्षर         features_buf[128];
    अक्षर*        bufp = features_buf;

    FT_UNUSED( तथ्य );


    अगर ( ft_have_newline_char )
    {
      स्थिर अक्षर*  features        = शून्य;
      माप_प्रकार       features_लंबाई = 0;


#घोषणा FEATURES_TIMESTAMP            "[%h:%m] "
#घोषणा FEATURES_COMPONENT            "[%t] "
#घोषणा FEATURES_TIMESTAMP_COMPONENT  "[%h:%m %t] "

      अगर ( ft_timestamp_flag && ft_component_flag )
      {
        features        = FEATURES_TIMESTAMP_COMPONENT;
        features_लंबाई = माप ( FEATURES_TIMESTAMP_COMPONENT );
      }
      अन्यथा अगर ( ft_timestamp_flag )
      {
        features        = FEATURES_TIMESTAMP;
        features_लंबाई = माप ( FEATURES_TIMESTAMP );
      }
      अन्यथा अगर ( ft_component_flag )
      {
        features        = FEATURES_COMPONENT;
        features_लंबाई = माप ( FEATURES_COMPONENT );
      }

      अगर ( ft_component_flag || ft_timestamp_flag )
      {
        ft_strncpy( features_buf, features, features_लंबाई );
        bufp += features_लंबाई - 1;
      }

      अगर ( ft_component_flag )
      {
        माप_प्रकार  tag_लंबाई = ft_strlen( *origin->tags );
        माप_प्रकार  i;


        /* To vertically align tracing messages we compensate the */
        /* different FT_COMPONENT string lengths by inserting an  */
        /* appropriate amount of space characters.                */
        क्रम ( i = 0;
              i < FT_MAX_TRACE_LEVEL_लंबाई - tag_लंबाई;
              i++ )
          *bufp++ = ' ';
      }
    }

    /* Finally add the format string for the tracing message. */
    *bufp++ = '%';
    *bufp++ = 'c';
    *bufp   = '\0';

    dlg_generic_outputf_stream( ft_fileptr,
                                (स्थिर अक्षर*)features_buf,
                                origin,
                                string,
                                dlg_default_output_styles,
                                true );

    अगर ( ft_strrchr( string, '\n' ) )
      ft_have_newline_char = TRUE;
    अन्यथा
      ft_have_newline_char = FALSE;
  }


  /* documentation is in ftdebug.h */
  FT_BASE_DEF( व्योम )
  ft_add_tag( स्थिर अक्षर*  tag )
  {
    ft_component = tag;

    dlg_add_tag( tag, शून्य );
  }


  /* documentation is in ftdebug.h */
  FT_BASE_DEF( व्योम )
  ft_remove_tag( स्थिर अक्षर*  tag )
  {
    dlg_remove_tag( tag, शून्य );
  }


  /* documentation is in ftlogging.h */

  FT_EXPORT_DEF( व्योम )
  FT_Trace_Set_Level( स्थिर अक्षर*  level )
  {
    ft_component_flag     = FALSE;
    ft_timestamp_flag     = FALSE;
    ft_custom_trace_level = level;

    ft_debug_init();
  }


  /* documentation is in ftlogging.h */

  FT_EXPORT_DEF( व्योम )
  FT_Trace_Set_Default_Level( व्योम )
  {
    ft_component_flag     = FALSE;
    ft_timestamp_flag     = FALSE;
    ft_custom_trace_level = शून्य;

    ft_debug_init();
  }


  /**************************************************************************
   *
   * Functions to handle a custom log handler.
   *
   */

  /* documentation is in ftlogging.h */

  FT_EXPORT_DEF( व्योम )
  FT_Set_Log_Handler( FT_Custom_Log_Handler  handler )
  {
    custom_output_handler = handler;
  }


  /* documentation is in ftlogging.h */

  FT_EXPORT_DEF( व्योम )
  FT_Set_Default_Log_Handler( व्योम )
  {
    custom_output_handler = शून्य;
  }


  /* documentation is in ftdebug.h */
  FT_BASE_DEF( व्योम )
  FT_Logging_Callback( स्थिर अक्षर*  fmt,
                       ... )
  {
    बहु_सूची  ap;


    बहु_शुरू( ap, fmt );
    custom_output_handler( ft_component, fmt, ap );
    बहु_पूर्ण( ap );
  }

#अन्यथा /* !FT_DEBUG_LOGGING */

  FT_EXPORT_DEF( व्योम )
  FT_Trace_Set_Level( स्थिर अक्षर*  level )
  {
    FT_UNUSED( level );
  }


  FT_EXPORT_DEF( व्योम )
  FT_Trace_Set_Default_Level( व्योम )
  {
    /* nothing */
  }


  FT_EXPORT_DEF( व्योम )
  FT_Set_Log_Handler( FT_Custom_Log_Handler  handler )
  {
    FT_UNUSED( handler );
  }


  FT_EXPORT_DEF( व्योम )
  FT_Set_Default_Log_Handler( व्योम )
  {
    /* nothing */
  }

#पूर्ण_अगर /* !FT_DEBUG_LOGGING */


/* END */
