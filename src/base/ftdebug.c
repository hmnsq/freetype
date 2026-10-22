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


#अन्तर्भाव <freetype/freetype.h>
#अन्तर्भाव <freetype/ftlogging.h>
#अन्तर्भाव <freetype/internal/ftdebug.h>
#अन्तर्भाव <freetype/internal/ftobjs.h>


#ifdef FT_DEBUG_LOGGING

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
  स्थिर ध्रुव वर्ण*  ft_default_trace_level = रिक्त;
  स्थिर FILE*        ft_fileptr             = रिक्त;
  स्थिर ध्रुव वर्ण*  ft_component           = रिक्त;
  स्थिर FT_Bool      ft_component_flag      = FALSE;
  स्थिर FT_Bool      ft_timestamp_flag      = FALSE;
  स्थिर FT_Bool      ft_have_newline_char   = TRUE;
  स्थिर ध्रुव वर्ण*  ft_custom_trace_level  = रिक्त;

  /* declared in ftdebug.h */

  dlg_handler            ft_default_log_handler = रिक्त;
  FT_Custom_Log_Handler  custom_output_handler  = रिक्त;

#endif /* FT_DEBUG_LOGGING */


#ifdef FT_DEBUG_LEVEL_ERROR

  /* documentation is in ftdebug.h */

  FT_BASE_DEF( शून्य )
  FT_Message( ध्रुव वर्ण*  fmt,
              ... )
  {
    va_list  ap;


    va_start( ap, fmt );
    vfprintf( stderr, fmt, ap );
    va_end( ap );
  }


  /* documentation is in ftdebug.h */

  FT_BASE_DEF( शून्य )
  FT_Panic( ध्रुव वर्ण*  fmt,
            ... )
  {
    va_list  ap;


    va_start( ap, fmt );
    vfprintf( stderr, fmt, ap );
    va_end( ap );

    exit( EXIT_FAILURE );
  }


  /* documentation is in ftdebug.h */

  FT_BASE_DEF( पूर्णाङ्क )
  FT_Throw( FT_Error     error,
            पूर्णाङ्क          पंक्ति,
            ध्रुव वर्ण*  file )
  {
#यदि 0
    /* activating the code in this block makes FreeType very chatty */
    fprintf( stderr,
             "%s:%d: error 0x%02x: %s\n",
             file,
             पंक्ति,
             error,
             FT_Error_String( error ) );
#अन्यथा
    FT_UNUSED( error );
    FT_UNUSED( पंक्ति );
    FT_UNUSED( file );
#endif

    प्रत्यागम 0;
  }

#endif /* FT_DEBUG_LEVEL_ERROR */


#ifdef FT_DEBUG_LEVEL_TRACE

  /* array of trace levels, initialized to 0; */
  /* this gets adjusted at run-time           */
  स्थिर पूर्णाङ्क  ft_trace_levels_enabled[trace_गिनती];

  /* array of trace levels, always initialized to 0 */
  स्थिर पूर्णाङ्क  ft_trace_levels_disabled[trace_गिनती];

  /* a pointer to either `ft_trace_levels_enabled' */
  /* or `ft_trace_levels_disabled'                 */
  पूर्णाङ्क*  ft_trace_levels;

  /* define array of trace toggle names */
#define FT_TRACE_DEF( x )  #x ,

  स्थिर ध्रुव वर्ण*  ft_trace_toggles[trace_गिनती + 1] =
  {
#अन्तर्भाव <freetype/internal/fttrace.h>
    रिक्त
  };

#undef FT_TRACE_DEF


  /* documentation is in ftdebug.h */

  FT_BASE_DEF( FT_Int )
  FT_Trace_Get_गिनती( शून्य )
  {
    प्रत्यागम trace_गिनती;
  }


  /* documentation is in ftdebug.h */

  FT_BASE_DEF( ध्रुव वर्ण * )
  FT_Trace_Get_Name( FT_Int  idx )
  {
    पूर्णाङ्क  max = FT_Trace_Get_गिनती();


    यदि ( idx < max )
      प्रत्यागम ft_trace_toggles[idx];
    अन्यथा
      प्रत्यागम रिक्त;
  }


  /* documentation is in ftdebug.h */

  FT_BASE_DEF( शून्य )
  FT_Trace_Disable( शून्य )
  {
    ft_trace_levels = ft_trace_levels_disabled;
  }


  /* documentation is in ftdebug.h */

  FT_BASE_DEF( शून्य )
  FT_Trace_Enable( शून्य )
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
  FT_BASE_DEF( शून्य )
  ft_debug_init( शून्य )
  {
    ध्रुव वर्ण*  ft2_debug = रिक्त;


#ifdef FT_DEBUG_LOGGING
    यदि ( ft_custom_trace_level != रिक्त )
      ft2_debug = ft_custom_trace_level;
    अन्यथा
      ft2_debug = ft_default_trace_level;
#अन्यथा
    ft2_debug = ft_getenv( "FT2_DEBUG" );
#endif

    यदि ( ft2_debug )
    {
      ध्रुव वर्ण*  p = ft2_debug;
      ध्रुव वर्ण*  q;


      प्रति ( ; *p; p++ )
      {
        /* skip leading whitespace and separators */
        यदि ( *p == ' ' || *p == '\t' || *p == ',' || *p == ';' || *p == '=' )
          अनुवर्त;

#ifdef FT_DEBUG_LOGGING

        /* check extra arguments for logging */
        यदि ( *p == '-' )
        {
          ध्रुव वर्ण*  r = ++p;


          यदि ( *r == 'v' )
          {
            ध्रुव वर्ण*  s = ++r;


            ft_component_flag = TRUE;

            यदि ( *s == 't' )
            {
              ft_timestamp_flag = TRUE;
              p++;
            }

            p++;
          }

          अन्यथा यदि ( *r == 't' )
          {
            ध्रुव वर्ण*  s = ++r;


            ft_timestamp_flag = TRUE;

            यदि ( *s == 'v' )
            {
              ft_component_flag = TRUE;
              p++;
            }

            p++;
          }
        }

#endif /* FT_DEBUG_LOGGING */

        /* read toggle name, followed by ':' */
        q = p;
        यावत् ( *p && *p != ':' )
          p++;

        यदि ( !*p )
          भङ्ग;

        यदि ( *p == ':' && p > q )
        {
          FT_Int  n, i, len = (FT_Int)( p - q );
          FT_Int  level = -1, found = -1;


          प्रति ( n = 0; n < trace_गिनती; n++ )
          {
            ध्रुव वर्ण*  toggle = ft_trace_toggles[n];


            प्रति ( i = 0; i < len; i++ )
            {
              यदि ( toggle[i] != q[i] )
                भङ्ग;
            }

            यदि ( i == len && toggle[i] == 0 )
            {
              found = n;
              भङ्ग;
            }
          }

          /* read level */
          p++;
          यदि ( *p )
          {
            level = *p - '0';
            यदि ( level < 0 || level > 7 )
              level = -1;
          }

          यदि ( found >= 0 && level >= 0 )
          {
            यदि ( found == trace_any )
            {
              /* special case for `any' */
              प्रति ( n = 0; n < trace_गिनती; n++ )
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


  FT_BASE_DEF( शून्य )
  ft_debug_init( शून्य )
  {
    /* nothing */
  }


  FT_BASE_DEF( FT_Int )
  FT_Trace_Get_गिनती( शून्य )
  {
    प्रत्यागम 0;
  }


  FT_BASE_DEF( ध्रुव वर्ण * )
  FT_Trace_Get_Name( FT_Int  idx )
  {
    FT_UNUSED( idx );

    प्रत्यागम रिक्त;
  }


  FT_BASE_DEF( शून्य )
  FT_Trace_Disable( शून्य )
  {
    /* nothing */
  }


  /* documentation is in ftdebug.h */

  FT_BASE_DEF( शून्य )
  FT_Trace_Enable( शून्य )
  {
    /* nothing */
  }

#endif /* !FT_DEBUG_LEVEL_TRACE */


#ifdef FT_DEBUG_LOGGING

  /**************************************************************************
   *
   * Initialize and de-initialize 'dlg' library.
   *
   */

  FT_BASE_DEF( शून्य )
  ft_logging_init( शून्य )
  {
    ft_default_log_handler = ft_log_handler;
    ft_default_trace_level = ft_getenv( "FT2_DEBUG" );

    यदि ( ft_getenv( "FT_LOGGING_FILE" ) )
      ft_fileptr = ft_fopen( ft_getenv( "FT_LOGGING_FILE" ), "w" );
    अन्यथा
      ft_fileptr = stderr;

    ft_debug_init();

    /* Set the default output handler for 'dlg'. */
    dlg_set_handler( ft_default_log_handler, रिक्त );
  }


  FT_BASE_DEF( शून्य )
  ft_logging_deinit( शून्य )
  {
    यदि ( ft_fileptr != stderr )
      ft_fclose( ft_fileptr );
  }


  /**************************************************************************
   *
   * An output log handler for FreeType.
   *
   */
  FT_BASE_DEF( शून्य )
  ft_log_handler( ध्रुव संरचना dlg_origin*  origin,
                  ध्रुव वर्ण*               string,
                  शून्य*                     data )
  {
    वर्ण         features_buf[128];
    वर्ण*        bufp = features_buf;

    FT_UNUSED( data );


    यदि ( ft_have_newline_char )
    {
      ध्रुव वर्ण*  features        = रिक्त;
      size_t       features_length = 0;


#define FEATURES_TIMESTAMP            "[%h:%m] "
#define FEATURES_COMPONENT            "[%t] "
#define FEATURES_TIMESTAMP_COMPONENT  "[%h:%m %t] "

      यदि ( ft_timestamp_flag && ft_component_flag )
      {
        features        = FEATURES_TIMESTAMP_COMPONENT;
        features_length = परिमाण ( FEATURES_TIMESTAMP_COMPONENT );
      }
      अन्यथा यदि ( ft_timestamp_flag )
      {
        features        = FEATURES_TIMESTAMP;
        features_length = परिमाण ( FEATURES_TIMESTAMP );
      }
      अन्यथा यदि ( ft_component_flag )
      {
        features        = FEATURES_COMPONENT;
        features_length = परिमाण ( FEATURES_COMPONENT );
      }

      यदि ( ft_component_flag || ft_timestamp_flag )
      {
        ft_strncpy( features_buf, features, features_length );
        bufp += features_length - 1;
      }

      यदि ( ft_component_flag )
      {
        size_t  tag_length = ft_strlen( *origin->tags );
        size_t  i;


        /* To vertically align tracing messages we compensate the */
        /* different FT_COMPONENT string lengths by inserting an  */
        /* appropriate amount of space characters.                */
        प्रति ( i = 0;
              i < FT_MAX_TRACE_LEVEL_LENGTH - tag_length;
              i++ )
          *bufp++ = ' ';
      }
    }

    /* Finally add the format string for the tracing message. */
    *bufp++ = '%';
    *bufp++ = 'c';
    *bufp   = '\0';

    dlg_generic_outputf_stream( ft_fileptr,
                                (ध्रुव वर्ण*)features_buf,
                                origin,
                                string,
                                dlg_default_output_styles,
                                true );

    यदि ( ft_strrchr( string, '\n' ) )
      ft_have_newline_char = TRUE;
    अन्यथा
      ft_have_newline_char = FALSE;
  }


  /* documentation is in ftdebug.h */
  FT_BASE_DEF( शून्य )
  ft_add_tag( ध्रुव वर्ण*  tag )
  {
    ft_component = tag;

    dlg_add_tag( tag, रिक्त );
  }


  /* documentation is in ftdebug.h */
  FT_BASE_DEF( शून्य )
  ft_remove_tag( ध्रुव वर्ण*  tag )
  {
    dlg_remove_tag( tag, रिक्त );
  }


  /* documentation is in ftlogging.h */

  FT_EXPORT_DEF( शून्य )
  FT_Trace_Set_Level( ध्रुव वर्ण*  level )
  {
    ft_component_flag     = FALSE;
    ft_timestamp_flag     = FALSE;
    ft_custom_trace_level = level;

    ft_debug_init();
  }


  /* documentation is in ftlogging.h */

  FT_EXPORT_DEF( शून्य )
  FT_Trace_Set_Default_Level( शून्य )
  {
    ft_component_flag     = FALSE;
    ft_timestamp_flag     = FALSE;
    ft_custom_trace_level = रिक्त;

    ft_debug_init();
  }


  /**************************************************************************
   *
   * Functions to handle a custom log handler.
   *
   */

  /* documentation is in ftlogging.h */

  FT_EXPORT_DEF( शून्य )
  FT_Set_Log_Handler( FT_Custom_Log_Handler  handler )
  {
    custom_output_handler = handler;
  }


  /* documentation is in ftlogging.h */

  FT_EXPORT_DEF( शून्य )
  FT_Set_Default_Log_Handler( शून्य )
  {
    custom_output_handler = रिक्त;
  }


  /* documentation is in ftdebug.h */
  FT_BASE_DEF( शून्य )
  FT_Logging_Callback( ध्रुव वर्ण*  fmt,
                       ... )
  {
    va_list  ap;


    va_start( ap, fmt );
    custom_output_handler( ft_component, fmt, ap );
    va_end( ap );
  }

#अन्यथा /* !FT_DEBUG_LOGGING */

  FT_EXPORT_DEF( शून्य )
  FT_Trace_Set_Level( ध्रुव वर्ण*  level )
  {
    FT_UNUSED( level );
  }


  FT_EXPORT_DEF( शून्य )
  FT_Trace_Set_Default_Level( शून्य )
  {
    /* nothing */
  }


  FT_EXPORT_DEF( शून्य )
  FT_Set_Log_Handler( FT_Custom_Log_Handler  handler )
  {
    FT_UNUSED( handler );
  }


  FT_EXPORT_DEF( शून्य )
  FT_Set_Default_Log_Handler( शून्य )
  {
    /* nothing */
  }

#endif /* !FT_DEBUG_LOGGING */


/* END */
