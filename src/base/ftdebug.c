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


#यदि_परिभाषितम् FT_DEBUG_LOGGING

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

#यदि_अन्तः /* FT_DEBUG_LOGGING */


#यदि_परिभाषितम् FT_DEBUG_LEVEL_ERROR

  /* documentation is in ftdebug.h */

  FT_आधारः_DEF( शून्य )
  FT_सन्देशः( ध्रुव वर्ण*  fmt,
              ... )
  {
    va_list  ap;


    va_start( ap, fmt );
    vfprintf( stderr, fmt, ap );
    va_अन्तः( ap );
  }


  /* documentation is in ftdebug.h */

  FT_आधारः_DEF( शून्य )
  FT_Panic( ध्रुव वर्ण*  fmt,
            ... )
  {
    va_list  ap;


    va_start( ap, fmt );
    vfprintf( stderr, fmt, ap );
    va_अन्तः( ap );

    निर्गच्छ( EXIT_FAILURE );
  }


  /* documentation is in ftdebug.h */

  FT_आधारः_DEF( पूर्णाङ्क )
  FT_Throw( FT_Error     error,
            पूर्णाङ्क          पङ्क्तिः,
            ध्रुव वर्ण*  file )
  {
#यदि 0
    /* activating the code in this block makes FreeType very chatty */
    fprintf( stderr,
             "%s:%d: error 0x%02x: %s\n",
             file,
             पङ्क्तिः,
             error,
             FT_Error_String( error ) );
#अन्यथा
    FT_अप्रयुक्तम्( error );
    FT_अप्रयुक्तम्( पङ्क्तिः );
    FT_अप्रयुक्तम्( file );
#यदि_अन्तः

    प्रत्यागम 0;
  }

#यदि_अन्तः /* FT_DEBUG_LEVEL_ERROR */


#यदि_परिभाषितम् FT_DEBUG_LEVEL_TRACE

  /* array of trace levels, initialized to 0; */
  /* this gets adjusted at run-time           */
  स्थिर पूर्णाङ्क  ft_trace_स्तराः_enabled[trace_गणनम्];

  /* array of trace levels, always initialized to 0 */
  स्थिर पूर्णाङ्क  ft_trace_स्तराः_disabled[trace_गणनम्];

  /* a pointer to either `ft_trace_levels_enabled' */
  /* or `ft_trace_levels_disabled'                 */
  पूर्णाङ्क*  ft_trace_स्तराः;

  /* define array of trace toggle names */
#परिभाषय FT_TRACE_DEF( x )  #x ,

  स्थिर ध्रुव वर्ण*  ft_trace_toggles[trace_गणनम् + 1] =
  {
#अन्तर्भाव <freetype/internal/fttrace.h>
    रिक्त
  };

#अपरिभाषय FT_TRACE_DEF


  /* documentation is in ftdebug.h */

  FT_आधारः_DEF( FT_Int )
  FT_Trace_Get_गणनम्( शून्य )
  {
    प्रत्यागम trace_गणनम्;
  }


  /* documentation is in ftdebug.h */

  FT_आधारः_DEF( ध्रुव वर्ण * )
  FT_Trace_Get_Name( FT_Int  idx )
  {
    पूर्णाङ्क  max = FT_Trace_Get_गणनम्();


    यदि ( idx < max )
      प्रत्यागम ft_trace_toggles[idx];
    अन्यथा
      प्रत्यागम रिक्त;
  }


  /* documentation is in ftdebug.h */

  FT_आधारः_DEF( शून्य )
  FT_Trace_Disable( शून्य )
  {
    ft_trace_स्तराः = ft_trace_स्तराः_disabled;
  }


  /* documentation is in ftdebug.h */

  FT_आधारः_DEF( शून्य )
  FT_Trace_Enable( शून्य )
  {
    ft_trace_स्तराः = ft_trace_स्तराः_enabled;
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
  FT_आधारः_DEF( शून्य )
  ft_debug_init( शून्य )
  {
    ध्रुव वर्ण*  ft2_debug = रिक्त;


#यदि_परिभाषितम् FT_DEBUG_LOGGING
    यदि ( ft_custom_trace_level != रिक्त )
      ft2_debug = ft_custom_trace_level;
    अन्यथा
      ft2_debug = ft_default_trace_level;
#अन्यथा
    ft2_debug = ft_getenv( "FT2_DEBUG" );
#यदि_अन्तः

    यदि ( ft2_debug )
    {
      ध्रुव वर्ण*  p = ft2_debug;
      ध्रुव वर्ण*  q;


      प्रति ( ; *p; p++ )
      {
        /* skip leading whitespace and separators */
        यदि ( *p == ' ' || *p == '\t' || *p == ',' || *p == ';' || *p == '=' )
          अनुवर्त;

#यदि_परिभाषितम् FT_DEBUG_LOGGING

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

#यदि_अन्तः /* FT_DEBUG_LOGGING */

        /* read toggle name, followed by ':' */
        q = p;
        यावत् ( *p && *p != ':' )
          p++;

        यदि ( !*p )
          भङ्ग;

        यदि ( *p == ':' && p > q )
        {
          FT_Int  n, i, len = (FT_Int)( p - q );
          FT_Int  level = -1, प्राप्तम् = -1;


          प्रति ( n = 0; n < trace_गणनम्; n++ )
          {
            ध्रुव वर्ण*  toggle = ft_trace_toggles[n];


            प्रति ( i = 0; i < len; i++ )
            {
              यदि ( toggle[i] != q[i] )
                भङ्ग;
            }

            यदि ( i == len && toggle[i] == 0 )
            {
              प्राप्तम् = n;
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

          यदि ( प्राप्तम् >= 0 && level >= 0 )
          {
            यदि ( प्राप्तम् == trace_any )
            {
              /* special case for `any' */
              प्रति ( n = 0; n < trace_गणनम्; n++ )
                ft_trace_स्तराः_enabled[n] = level;
            }
            अन्यथा
              ft_trace_स्तराः_enabled[प्राप्तम्] = level;
          }
        }
      }
    }

    ft_trace_स्तराः = ft_trace_स्तराः_enabled;
  }


#अन्यथा  /* !FT_DEBUG_LEVEL_TRACE */


  FT_आधारः_DEF( शून्य )
  ft_debug_init( शून्य )
  {
    /* nothing */
  }


  FT_आधारः_DEF( FT_Int )
  FT_Trace_Get_गणनम्( शून्य )
  {
    प्रत्यागम 0;
  }


  FT_आधारः_DEF( ध्रुव वर्ण * )
  FT_Trace_Get_Name( FT_Int  idx )
  {
    FT_अप्रयुक्तम्( idx );

    प्रत्यागम रिक्त;
  }


  FT_आधारः_DEF( शून्य )
  FT_Trace_Disable( शून्य )
  {
    /* nothing */
  }


  /* documentation is in ftdebug.h */

  FT_आधारः_DEF( शून्य )
  FT_Trace_Enable( शून्य )
  {
    /* nothing */
  }

#यदि_अन्तः /* !FT_DEBUG_LEVEL_TRACE */


#यदि_परिभाषितम् FT_DEBUG_LOGGING

  /**************************************************************************
   *
   * Initialize and de-initialize 'dlg' library.
   *
   */

  FT_आधारः_DEF( शून्य )
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
    dlg_समुच्चयः_handler( ft_default_log_handler, रिक्त );
  }


  FT_आधारः_DEF( शून्य )
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
  FT_आधारः_DEF( शून्य )
  ft_log_handler( ध्रुव संरचना dlg_origin*  origin,
                  ध्रुव वर्ण*               string,
                  शून्य*                     दत्त )
  {
    वर्ण         features_buf[128];
    वर्ण*        bufp = features_buf;

    FT_अप्रयुक्तम्( दत्त );


    यदि ( ft_have_newline_char )
    {
      ध्रुव वर्ण*  features        = रिक्त;
      size_t       features_दीर्घता = 0;


#परिभाषय FEATURES_TIMESTAMP            "[%h:%m] "
#परिभाषय FEATURES_COMPONENT            "[%t] "
#परिभाषय FEATURES_TIMESTAMP_COMPONENT  "[%h:%m %t] "

      यदि ( ft_timestamp_flag && ft_component_flag )
      {
        features        = FEATURES_TIMESTAMP_COMPONENT;
        features_दीर्घता = परिमाण ( FEATURES_TIMESTAMP_COMPONENT );
      }
      अन्यथा यदि ( ft_timestamp_flag )
      {
        features        = FEATURES_TIMESTAMP;
        features_दीर्घता = परिमाण ( FEATURES_TIMESTAMP );
      }
      अन्यथा यदि ( ft_component_flag )
      {
        features        = FEATURES_COMPONENT;
        features_दीर्घता = परिमाण ( FEATURES_COMPONENT );
      }

      यदि ( ft_component_flag || ft_timestamp_flag )
      {
        ft_strncpy( features_buf, features, features_दीर्घता );
        bufp += features_दीर्घता - 1;
      }

      यदि ( ft_component_flag )
      {
        size_t  चिह्नकम्_दीर्घता = ft_strlen( *origin->चिह्नकानि );
        size_t  i;


        /* To vertically align tracing messages we compensate the */
        /* different FT_COMPONENT string lengths by inserting an  */
        /* appropriate amount of space characters.                */
        प्रति ( i = 0;
              i < FT_MAX_TRACE_LEVEL_दीर्घता - चिह्नकम्_दीर्घता;
              i++ )
          *bufp++ = ' ';
      }
    }

    /* Finally add the format string for the tracing message. */
    *bufp++ = '%';
    *bufp++ = 'c';
    *bufp   = '\0';

    dlg_generic_outputf_प्रवाहः( ft_fileptr,
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
  FT_आधारः_DEF( शून्य )
  ft_सङ्कलय_चिह्नकम्( ध्रुव वर्ण*  चिह्नकम् )
  {
    ft_component = चिह्नकम्;

    dlg_सङ्कलय_चिह्नकम्( चिह्नकम्, रिक्त );
  }


  /* documentation is in ftdebug.h */
  FT_आधारः_DEF( शून्य )
  ft_remove_चिह्नकम्( ध्रुव वर्ण*  चिह्नकम् )
  {
    dlg_remove_चिह्नकम्( चिह्नकम्, रिक्त );
  }


  /* documentation is in ftlogging.h */

  FT_EXPORT_DEF( शून्य )
  FT_Trace_समुच्चयः_Level( ध्रुव वर्ण*  level )
  {
    ft_component_flag     = FALSE;
    ft_timestamp_flag     = FALSE;
    ft_custom_trace_level = level;

    ft_debug_init();
  }


  /* documentation is in ftlogging.h */

  FT_EXPORT_DEF( शून्य )
  FT_Trace_समुच्चयः_Default_Level( शून्य )
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
  FT_समुच्चयः_Log_Handler( FT_Custom_Log_Handler  handler )
  {
    custom_output_handler = handler;
  }


  /* documentation is in ftlogging.h */

  FT_EXPORT_DEF( शून्य )
  FT_समुच्चयः_Default_Log_Handler( शून्य )
  {
    custom_output_handler = रिक्त;
  }


  /* documentation is in ftdebug.h */
  FT_आधारः_DEF( शून्य )
  FT_Logging_Callback( ध्रुव वर्ण*  fmt,
                       ... )
  {
    va_list  ap;


    va_start( ap, fmt );
    custom_output_handler( ft_component, fmt, ap );
    va_अन्तः( ap );
  }

#अन्यथा /* !FT_DEBUG_LOGGING */

  FT_EXPORT_DEF( शून्य )
  FT_Trace_समुच्चयः_Level( ध्रुव वर्ण*  level )
  {
    FT_अप्रयुक्तम्( level );
  }


  FT_EXPORT_DEF( शून्य )
  FT_Trace_समुच्चयः_Default_Level( शून्य )
  {
    /* nothing */
  }


  FT_EXPORT_DEF( शून्य )
  FT_समुच्चयः_Log_Handler( FT_Custom_Log_Handler  handler )
  {
    FT_अप्रयुक्तम्( handler );
  }


  FT_EXPORT_DEF( शून्य )
  FT_समुच्चयः_Default_Log_Handler( शून्य )
  {
    /* nothing */
  }

#यदि_अन्तः /* !FT_DEBUG_LOGGING */


/*अन्तः*/
