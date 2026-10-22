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


#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/freetype.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/ftlogging.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftdebug.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftobjs.h>


#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ FT_DEBUG_LOGGING

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
  ਸਥਿਰ ਅਚਲ ਅੱਖਰ*  ft_default_trace_level = ਨਿਰਾ;
  ਸਥਿਰ FILE*        ft_fileptr             = ਨਿਰਾ;
  ਸਥਿਰ ਅਚਲ ਅੱਖਰ*  ft_component           = ਨਿਰਾ;
  ਸਥਿਰ FT_Bool      ft_component_flag      = FALSE;
  ਸਥਿਰ FT_Bool      ft_timestamp_flag      = FALSE;
  ਸਥਿਰ FT_Bool      ft_have_newline_char   = TRUE;
  ਸਥਿਰ ਅਚਲ ਅੱਖਰ*  ft_custom_trace_level  = ਨਿਰਾ;

  /* declared in ftdebug.h */

  dlg_handler            ft_default_log_handler = ਨਿਰਾ;
  FT_Custom_Log_Handler  custom_output_handler  = ਨਿਰਾ;

#ਜੇ_ਅੰਤ /* FT_DEBUG_LOGGING */


#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ FT_DEBUG_LEVEL_ERROR

  /* documentation is in ftdebug.h */

  FT_ਅਧਾਰ_DEF( ਖਾਲੀ )
  FT_ਸੁਨੇਹਾ( ਅਚਲ ਅੱਖਰ*  fmt,
              ... )
  {
    va_list  ap;


    va_ਸ਼ੁਰੂ( ap, fmt );
    vfprintf( stderr, fmt, ap );
    va_ਸਿਰਾ( ap );
  }


  /* documentation is in ftdebug.h */

  FT_ਅਧਾਰ_DEF( ਖਾਲੀ )
  FT_Panic( ਅਚਲ ਅੱਖਰ*  fmt,
            ... )
  {
    va_list  ap;


    va_ਸ਼ੁਰੂ( ap, fmt );
    vfprintf( stderr, fmt, ap );
    va_ਸਿਰਾ( ap );

    ਬਾਹਰ_ਨਿਕਲੋ( EXIT_FAILURE );
  }


  /* documentation is in ftdebug.h */

  FT_ਅਧਾਰ_DEF( ਪੂਰਨ )
  FT_Throw( FT_Error     error,
            ਪੂਰਨ          ਸਤਰ,
            ਅਚਲ ਅੱਖਰ*  file )
  {
#ਜੇ 0
    /* activating the code in this block makes FreeType very chatty */
    fprintf( stderr,
             "%s:%d: error 0x%02x: %s\n",
             file,
             ਸਤਰ,
             error,
             FT_Error_String( error ) );
#ਨਹੀਂਤਾਂ
    FT_ਅਣਵਰਤਿਆ( error );
    FT_ਅਣਵਰਤਿਆ( ਸਤਰ );
    FT_ਅਣਵਰਤਿਆ( file );
#ਜੇ_ਅੰਤ

    ਵਾਪਸ 0;
  }

#ਜੇ_ਅੰਤ /* FT_DEBUG_LEVEL_ERROR */


#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ FT_DEBUG_LEVEL_TRACE

  /* array of trace levels, initialized to 0; */
  /* this gets adjusted at run-time           */
  ਸਥਿਰ ਪੂਰਨ  ft_trace_ਪੱਧਰ_enabled[trace_ਗਿਣਤੀ];

  /* array of trace levels, always initialized to 0 */
  ਸਥਿਰ ਪੂਰਨ  ft_trace_ਪੱਧਰ_disabled[trace_ਗਿਣਤੀ];

  /* a pointer to either `ft_trace_levels_enabled' */
  /* or `ft_trace_levels_disabled'                 */
  ਪੂਰਨ*  ft_trace_ਪੱਧਰ;

  /* define array of trace toggle names */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FT_TRACE_DEF( x )  #x ,

  ਸਥਿਰ ਅਚਲ ਅੱਖਰ*  ft_trace_toggles[trace_ਗਿਣਤੀ + 1] =
  {
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/fttrace.h>
    ਨਿਰਾ
  };

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ FT_TRACE_DEF


  /* documentation is in ftdebug.h */

  FT_ਅਧਾਰ_DEF( FT_Int )
  FT_Trace_Get_ਗਿਣਤੀ( ਖਾਲੀ )
  {
    ਵਾਪਸ trace_ਗਿਣਤੀ;
  }


  /* documentation is in ftdebug.h */

  FT_ਅਧਾਰ_DEF( ਅਚਲ ਅੱਖਰ * )
  FT_Trace_Get_Name( FT_Int  idx )
  {
    ਪੂਰਨ  max = FT_Trace_Get_ਗਿਣਤੀ();


    ਜੇ ( idx < max )
      ਵਾਪਸ ft_trace_toggles[idx];
    ਨਹੀਂਤਾਂ
      ਵਾਪਸ ਨਿਰਾ;
  }


  /* documentation is in ftdebug.h */

  FT_ਅਧਾਰ_DEF( ਖਾਲੀ )
  FT_Trace_Disable( ਖਾਲੀ )
  {
    ft_trace_ਪੱਧਰ = ft_trace_ਪੱਧਰ_disabled;
  }


  /* documentation is in ftdebug.h */

  FT_ਅਧਾਰ_DEF( ਖਾਲੀ )
  FT_Trace_Enable( ਖਾਲੀ )
  {
    ft_trace_ਪੱਧਰ = ft_trace_ਪੱਧਰ_enabled;
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
  FT_ਅਧਾਰ_DEF( ਖਾਲੀ )
  ft_debug_init( ਖਾਲੀ )
  {
    ਅਚਲ ਅੱਖਰ*  ft2_debug = ਨਿਰਾ;


#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ FT_DEBUG_LOGGING
    ਜੇ ( ft_custom_trace_level != ਨਿਰਾ )
      ft2_debug = ft_custom_trace_level;
    ਨਹੀਂਤਾਂ
      ft2_debug = ft_default_trace_level;
#ਨਹੀਂਤਾਂ
    ft2_debug = ft_getenv( "FT2_DEBUG" );
#ਜੇ_ਅੰਤ

    ਜੇ ( ft2_debug )
    {
      ਅਚਲ ਅੱਖਰ*  p = ft2_debug;
      ਅਚਲ ਅੱਖਰ*  q;


      ਲਈ ( ; *p; p++ )
      {
        /* skip leading whitespace and separators */
        ਜੇ ( *p == ' ' || *p == '\t' || *p == ',' || *p == ';' || *p == '=' )
          ਜਾਰੀ;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ FT_DEBUG_LOGGING

        /* check extra arguments for logging */
        ਜੇ ( *p == '-' )
        {
          ਅਚਲ ਅੱਖਰ*  r = ++p;


          ਜੇ ( *r == 'v' )
          {
            ਅਚਲ ਅੱਖਰ*  s = ++r;


            ft_component_flag = TRUE;

            ਜੇ ( *s == 't' )
            {
              ft_timestamp_flag = TRUE;
              p++;
            }

            p++;
          }

          ਨਹੀਂਤਾਂ ਜੇ ( *r == 't' )
          {
            ਅਚਲ ਅੱਖਰ*  s = ++r;


            ft_timestamp_flag = TRUE;

            ਜੇ ( *s == 'v' )
            {
              ft_component_flag = TRUE;
              p++;
            }

            p++;
          }
        }

#ਜੇ_ਅੰਤ /* FT_DEBUG_LOGGING */

        /* read toggle name, followed by ':' */
        q = p;
        ਜਦੋਂ ( *p && *p != ':' )
          p++;

        ਜੇ ( !*p )
          ਰੋਕੋ;

        ਜੇ ( *p == ':' && p > q )
        {
          FT_Int  n, i, len = (FT_Int)( p - q );
          FT_Int  level = -1, ਮਿਲਿਆ = -1;


          ਲਈ ( n = 0; n < trace_ਗਿਣਤੀ; n++ )
          {
            ਅਚਲ ਅੱਖਰ*  toggle = ft_trace_toggles[n];


            ਲਈ ( i = 0; i < len; i++ )
            {
              ਜੇ ( toggle[i] != q[i] )
                ਰੋਕੋ;
            }

            ਜੇ ( i == len && toggle[i] == 0 )
            {
              ਮਿਲਿਆ = n;
              ਰੋਕੋ;
            }
          }

          /* read level */
          p++;
          ਜੇ ( *p )
          {
            level = *p - '0';
            ਜੇ ( level < 0 || level > 7 )
              level = -1;
          }

          ਜੇ ( ਮਿਲਿਆ >= 0 && level >= 0 )
          {
            ਜੇ ( ਮਿਲਿਆ == trace_any )
            {
              /* special case for `any' */
              ਲਈ ( n = 0; n < trace_ਗਿਣਤੀ; n++ )
                ft_trace_ਪੱਧਰ_enabled[n] = level;
            }
            ਨਹੀਂਤਾਂ
              ft_trace_ਪੱਧਰ_enabled[ਮਿਲਿਆ] = level;
          }
        }
      }
    }

    ft_trace_ਪੱਧਰ = ft_trace_ਪੱਧਰ_enabled;
  }


#ਨਹੀਂਤਾਂ  /* !FT_DEBUG_LEVEL_TRACE */


  FT_ਅਧਾਰ_DEF( ਖਾਲੀ )
  ft_debug_init( ਖਾਲੀ )
  {
    /* nothing */
  }


  FT_ਅਧਾਰ_DEF( FT_Int )
  FT_Trace_Get_ਗਿਣਤੀ( ਖਾਲੀ )
  {
    ਵਾਪਸ 0;
  }


  FT_ਅਧਾਰ_DEF( ਅਚਲ ਅੱਖਰ * )
  FT_Trace_Get_Name( FT_Int  idx )
  {
    FT_ਅਣਵਰਤਿਆ( idx );

    ਵਾਪਸ ਨਿਰਾ;
  }


  FT_ਅਧਾਰ_DEF( ਖਾਲੀ )
  FT_Trace_Disable( ਖਾਲੀ )
  {
    /* nothing */
  }


  /* documentation is in ftdebug.h */

  FT_ਅਧਾਰ_DEF( ਖਾਲੀ )
  FT_Trace_Enable( ਖਾਲੀ )
  {
    /* nothing */
  }

#ਜੇ_ਅੰਤ /* !FT_DEBUG_LEVEL_TRACE */


#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ FT_DEBUG_LOGGING

  /**************************************************************************
   *
   * Initialize and de-initialize 'dlg' library.
   *
   */

  FT_ਅਧਾਰ_DEF( ਖਾਲੀ )
  ft_logging_init( ਖਾਲੀ )
  {
    ft_default_log_handler = ft_log_handler;
    ft_default_trace_level = ft_getenv( "FT2_DEBUG" );

    ਜੇ ( ft_getenv( "FT_LOGGING_FILE" ) )
      ft_fileptr = ft_fopen( ft_getenv( "FT_LOGGING_FILE" ), "w" );
    ਨਹੀਂਤਾਂ
      ft_fileptr = stderr;

    ft_debug_init();

    /* Set the default output handler for 'dlg'. */
    dlg_ਸੈੱਟ_handler( ft_default_log_handler, ਨਿਰਾ );
  }


  FT_ਅਧਾਰ_DEF( ਖਾਲੀ )
  ft_logging_deinit( ਖਾਲੀ )
  {
    ਜੇ ( ft_fileptr != stderr )
      ft_fclose( ft_fileptr );
  }


  /**************************************************************************
   *
   * An output log handler for FreeType.
   *
   */
  FT_ਅਧਾਰ_DEF( ਖਾਲੀ )
  ft_log_handler( ਅਚਲ ਬਣਤਰ dlg_origin*  origin,
                  ਅਚਲ ਅੱਖਰ*               string,
                  ਖਾਲੀ*                     ਅੰਕੜੇ )
  {
    ਅੱਖਰ         features_buf[128];
    ਅੱਖਰ*        bufp = features_buf;

    FT_ਅਣਵਰਤਿਆ( ਅੰਕੜੇ );


    ਜੇ ( ft_have_newline_char )
    {
      ਅਚਲ ਅੱਖਰ*  features        = ਨਿਰਾ;
      size_t       features_ਲੰਮਾਈ = 0;


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FEATURES_TIMESTAMP            "[%h:%m] "
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FEATURES_COMPONENT            "[%t] "
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FEATURES_TIMESTAMP_COMPONENT  "[%h:%m %t] "

      ਜੇ ( ft_timestamp_flag && ft_component_flag )
      {
        features        = FEATURES_TIMESTAMP_COMPONENT;
        features_ਲੰਮਾਈ = ਆਕਾਰ ( FEATURES_TIMESTAMP_COMPONENT );
      }
      ਨਹੀਂਤਾਂ ਜੇ ( ft_timestamp_flag )
      {
        features        = FEATURES_TIMESTAMP;
        features_ਲੰਮਾਈ = ਆਕਾਰ ( FEATURES_TIMESTAMP );
      }
      ਨਹੀਂਤਾਂ ਜੇ ( ft_component_flag )
      {
        features        = FEATURES_COMPONENT;
        features_ਲੰਮਾਈ = ਆਕਾਰ ( FEATURES_COMPONENT );
      }

      ਜੇ ( ft_component_flag || ft_timestamp_flag )
      {
        ft_strncpy( features_buf, features, features_ਲੰਮਾਈ );
        bufp += features_ਲੰਮਾਈ - 1;
      }

      ਜੇ ( ft_component_flag )
      {
        size_t  ਟੈਗ_ਲੰਮਾਈ = ft_strlen( *origin->ਟੈਗ_ਸੂਚੀ );
        size_t  i;


        /* To vertically align tracing messages we compensate the */
        /* different FT_COMPONENT string lengths by inserting an  */
        /* appropriate amount of space characters.                */
        ਲਈ ( i = 0;
              i < FT_MAX_TRACE_LEVEL_ਲੰਮਾਈ - ਟੈਗ_ਲੰਮਾਈ;
              i++ )
          *bufp++ = ' ';
      }
    }

    /* Finally add the format string for the tracing message. */
    *bufp++ = '%';
    *bufp++ = 'c';
    *bufp   = '\0';

    dlg_generic_outputf_ਧਾਰਾ( ft_fileptr,
                                (ਅਚਲ ਅੱਖਰ*)features_buf,
                                origin,
                                string,
                                dlg_default_output_styles,
                                true );

    ਜੇ ( ft_strrchr( string, '\n' ) )
      ft_have_newline_char = TRUE;
    ਨਹੀਂਤਾਂ
      ft_have_newline_char = FALSE;
  }


  /* documentation is in ftdebug.h */
  FT_ਅਧਾਰ_DEF( ਖਾਲੀ )
  ft_ਜੋੜੋ_ਟੈਗ( ਅਚਲ ਅੱਖਰ*  ਟੈਗ )
  {
    ft_component = ਟੈਗ;

    dlg_ਜੋੜੋ_ਟੈਗ( ਟੈਗ, ਨਿਰਾ );
  }


  /* documentation is in ftdebug.h */
  FT_ਅਧਾਰ_DEF( ਖਾਲੀ )
  ft_remove_ਟੈਗ( ਅਚਲ ਅੱਖਰ*  ਟੈਗ )
  {
    dlg_remove_ਟੈਗ( ਟੈਗ, ਨਿਰਾ );
  }


  /* documentation is in ftlogging.h */

  FT_EXPORT_DEF( ਖਾਲੀ )
  FT_Trace_ਸੈੱਟ_Level( ਅਚਲ ਅੱਖਰ*  level )
  {
    ft_component_flag     = FALSE;
    ft_timestamp_flag     = FALSE;
    ft_custom_trace_level = level;

    ft_debug_init();
  }


  /* documentation is in ftlogging.h */

  FT_EXPORT_DEF( ਖਾਲੀ )
  FT_Trace_ਸੈੱਟ_Default_Level( ਖਾਲੀ )
  {
    ft_component_flag     = FALSE;
    ft_timestamp_flag     = FALSE;
    ft_custom_trace_level = ਨਿਰਾ;

    ft_debug_init();
  }


  /**************************************************************************
   *
   * Functions to handle a custom log handler.
   *
   */

  /* documentation is in ftlogging.h */

  FT_EXPORT_DEF( ਖਾਲੀ )
  FT_ਸੈੱਟ_Log_Handler( FT_Custom_Log_Handler  handler )
  {
    custom_output_handler = handler;
  }


  /* documentation is in ftlogging.h */

  FT_EXPORT_DEF( ਖਾਲੀ )
  FT_ਸੈੱਟ_Default_Log_Handler( ਖਾਲੀ )
  {
    custom_output_handler = ਨਿਰਾ;
  }


  /* documentation is in ftdebug.h */
  FT_ਅਧਾਰ_DEF( ਖਾਲੀ )
  FT_Logging_Callback( ਅਚਲ ਅੱਖਰ*  fmt,
                       ... )
  {
    va_list  ap;


    va_ਸ਼ੁਰੂ( ap, fmt );
    custom_output_handler( ft_component, fmt, ap );
    va_ਸਿਰਾ( ap );
  }

#ਨਹੀਂਤਾਂ /* !FT_DEBUG_LOGGING */

  FT_EXPORT_DEF( ਖਾਲੀ )
  FT_Trace_ਸੈੱਟ_Level( ਅਚਲ ਅੱਖਰ*  level )
  {
    FT_ਅਣਵਰਤਿਆ( level );
  }


  FT_EXPORT_DEF( ਖਾਲੀ )
  FT_Trace_ਸੈੱਟ_Default_Level( ਖਾਲੀ )
  {
    /* nothing */
  }


  FT_EXPORT_DEF( ਖਾਲੀ )
  FT_ਸੈੱਟ_Log_Handler( FT_Custom_Log_Handler  handler )
  {
    FT_ਅਣਵਰਤਿਆ( handler );
  }


  FT_EXPORT_DEF( ਖਾਲੀ )
  FT_ਸੈੱਟ_Default_Log_Handler( ਖਾਲੀ )
  {
    /* nothing */
  }

#ਜੇ_ਅੰਤ /* !FT_DEBUG_LOGGING */


/*ਸਿਰਾ*/
