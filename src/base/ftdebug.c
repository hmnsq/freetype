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


#അന്തര്ഭാവ <freetype/freetype.h>
#അന്തര്ഭാവ <freetype/ftlogging.h>
#അന്തര്ഭാവ <freetype/internal/ftdebug.h>
#അന്തര്ഭാവ <freetype/internal/ftobjs.h>


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
  സ്ഥിതം സ്ഥിരം അക്ഷരം*  ft_default_trace_level = ശൂന്യ;
  സ്ഥിതം FILE*        ft_fileptr             = ശൂന്യ;
  സ്ഥിതം സ്ഥിരം അക്ഷരം*  ft_component           = ശൂന്യ;
  സ്ഥിതം FT_Bool      ft_component_flag      = FALSE;
  സ്ഥിതം FT_Bool      ft_timestamp_flag      = FALSE;
  സ്ഥിതം FT_Bool      ft_have_newline_char   = TRUE;
  സ്ഥിതം സ്ഥിരം അക്ഷരം*  ft_custom_trace_level  = ശൂന്യ;

  /* declared in ftdebug.h */

  dlg_handler            ft_default_log_handler = ശൂന്യ;
  FT_Custom_Log_Handler  custom_output_handler  = ശൂന്യ;

#endif /* FT_DEBUG_LOGGING */


#ifdef FT_DEBUG_LEVEL_ERROR

  /* documentation is in ftdebug.h */

  FT_BASE_DEF( ശൂന്യം )
  FT_Message( സ്ഥിരം അക്ഷരം*  fmt,
              ... )
  {
    va_list  ap;


    va_start( ap, fmt );
    vfprintf( stderr, fmt, ap );
    va_end( ap );
  }


  /* documentation is in ftdebug.h */

  FT_BASE_DEF( ശൂന്യം )
  FT_Panic( സ്ഥിരം അക്ഷരം*  fmt,
            ... )
  {
    va_list  ap;


    va_start( ap, fmt );
    vfprintf( stderr, fmt, ap );
    va_end( ap );

    exit( EXIT_FAILURE );
  }


  /* documentation is in ftdebug.h */

  FT_BASE_DEF( പൂർണ്ണസംഖ്യ )
  FT_Throw( FT_Error     error,
            പൂർണ്ണസംഖ്യ          വരി,
            സ്ഥിരം അക്ഷരം*  file )
  {
#എങ്കിൽ 0
    /* activating the code in this block makes FreeType very chatty */
    fprintf( stderr,
             "%s:%d: error 0x%02x: %s\n",
             file,
             വരി,
             error,
             FT_Error_String( error ) );
#അല്ലെങ്കിൽ
    FT_UNUSED( error );
    FT_UNUSED( വരി );
    FT_UNUSED( file );
#endif

    തിരികെ 0;
  }

#endif /* FT_DEBUG_LEVEL_ERROR */


#ifdef FT_DEBUG_LEVEL_TRACE

  /* array of trace levels, initialized to 0; */
  /* this gets adjusted at run-time           */
  സ്ഥിതം പൂർണ്ണസംഖ്യ  ft_trace_levels_enabled[trace_എണ്ണം];

  /* array of trace levels, always initialized to 0 */
  സ്ഥിതം പൂർണ്ണസംഖ്യ  ft_trace_levels_disabled[trace_എണ്ണം];

  /* a pointer to either `ft_trace_levels_enabled' */
  /* or `ft_trace_levels_disabled'                 */
  പൂർണ്ണസംഖ്യ*  ft_trace_levels;

  /* define array of trace toggle names */
#define FT_TRACE_DEF( x )  #x ,

  സ്ഥിതം സ്ഥിരം അക്ഷരം*  ft_trace_toggles[trace_എണ്ണം + 1] =
  {
#അന്തര്ഭാവ <freetype/internal/fttrace.h>
    ശൂന്യ
  };

#undef FT_TRACE_DEF


  /* documentation is in ftdebug.h */

  FT_BASE_DEF( FT_Int )
  FT_Trace_Get_എണ്ണം( ശൂന്യം )
  {
    തിരികെ trace_എണ്ണം;
  }


  /* documentation is in ftdebug.h */

  FT_BASE_DEF( സ്ഥിരം അക്ഷരം * )
  FT_Trace_Get_Name( FT_Int  idx )
  {
    പൂർണ്ണസംഖ്യ  max = FT_Trace_Get_എണ്ണം();


    എങ്കിൽ ( idx < max )
      തിരികെ ft_trace_toggles[idx];
    അല്ലെങ്കിൽ
      തിരികെ ശൂന്യ;
  }


  /* documentation is in ftdebug.h */

  FT_BASE_DEF( ശൂന്യം )
  FT_Trace_Disable( ശൂന്യം )
  {
    ft_trace_levels = ft_trace_levels_disabled;
  }


  /* documentation is in ftdebug.h */

  FT_BASE_DEF( ശൂന്യം )
  FT_Trace_Enable( ശൂന്യം )
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
  FT_BASE_DEF( ശൂന്യം )
  ft_debug_init( ശൂന്യം )
  {
    സ്ഥിരം അക്ഷരം*  ft2_debug = ശൂന്യ;


#ifdef FT_DEBUG_LOGGING
    എങ്കിൽ ( ft_custom_trace_level != ശൂന്യ )
      ft2_debug = ft_custom_trace_level;
    അല്ലെങ്കിൽ
      ft2_debug = ft_default_trace_level;
#അല്ലെങ്കിൽ
    ft2_debug = ft_getenv( "FT2_DEBUG" );
#endif

    എങ്കിൽ ( ft2_debug )
    {
      സ്ഥിരം അക്ഷരം*  p = ft2_debug;
      സ്ഥിരം അക്ഷരം*  q;


      വേണ്ടി ( ; *p; p++ )
      {
        /* skip leading whitespace and separators */
        എങ്കിൽ ( *p == ' ' || *p == '\t' || *p == ',' || *p == ';' || *p == '=' )
          തുടരുക;

#ifdef FT_DEBUG_LOGGING

        /* check extra arguments for logging */
        എങ്കിൽ ( *p == '-' )
        {
          സ്ഥിരം അക്ഷരം*  r = ++p;


          എങ്കിൽ ( *r == 'v' )
          {
            സ്ഥിരം അക്ഷരം*  s = ++r;


            ft_component_flag = TRUE;

            എങ്കിൽ ( *s == 't' )
            {
              ft_timestamp_flag = TRUE;
              p++;
            }

            p++;
          }

          അല്ലെങ്കിൽ എങ്കിൽ ( *r == 't' )
          {
            സ്ഥിരം അക്ഷരം*  s = ++r;


            ft_timestamp_flag = TRUE;

            എങ്കിൽ ( *s == 'v' )
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
        ആകുമ്പോൾ ( *p && *p != ':' )
          p++;

        എങ്കിൽ ( !*p )
          നിർത്തുക;

        എങ്കിൽ ( *p == ':' && p > q )
        {
          FT_Int  n, i, len = (FT_Int)( p - q );
          FT_Int  level = -1, found = -1;


          വേണ്ടി ( n = 0; n < trace_എണ്ണം; n++ )
          {
            സ്ഥിരം അക്ഷരം*  toggle = ft_trace_toggles[n];


            വേണ്ടി ( i = 0; i < len; i++ )
            {
              എങ്കിൽ ( toggle[i] != q[i] )
                നിർത്തുക;
            }

            എങ്കിൽ ( i == len && toggle[i] == 0 )
            {
              found = n;
              നിർത്തുക;
            }
          }

          /* read level */
          p++;
          എങ്കിൽ ( *p )
          {
            level = *p - '0';
            എങ്കിൽ ( level < 0 || level > 7 )
              level = -1;
          }

          എങ്കിൽ ( found >= 0 && level >= 0 )
          {
            എങ്കിൽ ( found == trace_any )
            {
              /* special case for `any' */
              വേണ്ടി ( n = 0; n < trace_എണ്ണം; n++ )
                ft_trace_levels_enabled[n] = level;
            }
            അല്ലെങ്കിൽ
              ft_trace_levels_enabled[found] = level;
          }
        }
      }
    }

    ft_trace_levels = ft_trace_levels_enabled;
  }


#അല്ലെങ്കിൽ  /* !FT_DEBUG_LEVEL_TRACE */


  FT_BASE_DEF( ശൂന്യം )
  ft_debug_init( ശൂന്യം )
  {
    /* nothing */
  }


  FT_BASE_DEF( FT_Int )
  FT_Trace_Get_എണ്ണം( ശൂന്യം )
  {
    തിരികെ 0;
  }


  FT_BASE_DEF( സ്ഥിരം അക്ഷരം * )
  FT_Trace_Get_Name( FT_Int  idx )
  {
    FT_UNUSED( idx );

    തിരികെ ശൂന്യ;
  }


  FT_BASE_DEF( ശൂന്യം )
  FT_Trace_Disable( ശൂന്യം )
  {
    /* nothing */
  }


  /* documentation is in ftdebug.h */

  FT_BASE_DEF( ശൂന്യം )
  FT_Trace_Enable( ശൂന്യം )
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

  FT_BASE_DEF( ശൂന്യം )
  ft_logging_init( ശൂന്യം )
  {
    ft_default_log_handler = ft_log_handler;
    ft_default_trace_level = ft_getenv( "FT2_DEBUG" );

    എങ്കിൽ ( ft_getenv( "FT_LOGGING_FILE" ) )
      ft_fileptr = ft_fopen( ft_getenv( "FT_LOGGING_FILE" ), "w" );
    അല്ലെങ്കിൽ
      ft_fileptr = stderr;

    ft_debug_init();

    /* Set the default output handler for 'dlg'. */
    dlg_set_handler( ft_default_log_handler, ശൂന്യ );
  }


  FT_BASE_DEF( ശൂന്യം )
  ft_logging_deinit( ശൂന്യം )
  {
    എങ്കിൽ ( ft_fileptr != stderr )
      ft_fclose( ft_fileptr );
  }


  /**************************************************************************
   *
   * An output log handler for FreeType.
   *
   */
  FT_BASE_DEF( ശൂന്യം )
  ft_log_handler( സ്ഥിരം ഘടന dlg_origin*  origin,
                  സ്ഥിരം അക്ഷരം*               string,
                  ശൂന്യം*                     data )
  {
    അക്ഷരം         features_buf[128];
    അക്ഷരം*        bufp = features_buf;

    FT_UNUSED( data );


    എങ്കിൽ ( ft_have_newline_char )
    {
      സ്ഥിരം അക്ഷരം*  features        = ശൂന്യ;
      size_t       features_length = 0;


#define FEATURES_TIMESTAMP            "[%h:%m] "
#define FEATURES_COMPONENT            "[%t] "
#define FEATURES_TIMESTAMP_COMPONENT  "[%h:%m %t] "

      എങ്കിൽ ( ft_timestamp_flag && ft_component_flag )
      {
        features        = FEATURES_TIMESTAMP_COMPONENT;
        features_length = വലുപ്പം ( FEATURES_TIMESTAMP_COMPONENT );
      }
      അല്ലെങ്കിൽ എങ്കിൽ ( ft_timestamp_flag )
      {
        features        = FEATURES_TIMESTAMP;
        features_length = വലുപ്പം ( FEATURES_TIMESTAMP );
      }
      അല്ലെങ്കിൽ എങ്കിൽ ( ft_component_flag )
      {
        features        = FEATURES_COMPONENT;
        features_length = വലുപ്പം ( FEATURES_COMPONENT );
      }

      എങ്കിൽ ( ft_component_flag || ft_timestamp_flag )
      {
        ft_strncpy( features_buf, features, features_length );
        bufp += features_length - 1;
      }

      എങ്കിൽ ( ft_component_flag )
      {
        size_t  tag_length = ft_strlen( *origin->tags );
        size_t  i;


        /* To vertically align tracing messages we compensate the */
        /* different FT_COMPONENT string lengths by inserting an  */
        /* appropriate amount of space characters.                */
        വേണ്ടി ( i = 0;
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
                                (സ്ഥിരം അക്ഷരം*)features_buf,
                                origin,
                                string,
                                dlg_default_output_styles,
                                true );

    എങ്കിൽ ( ft_strrchr( string, '\n' ) )
      ft_have_newline_char = TRUE;
    അല്ലെങ്കിൽ
      ft_have_newline_char = FALSE;
  }


  /* documentation is in ftdebug.h */
  FT_BASE_DEF( ശൂന്യം )
  ft_add_tag( സ്ഥിരം അക്ഷരം*  tag )
  {
    ft_component = tag;

    dlg_add_tag( tag, ശൂന്യ );
  }


  /* documentation is in ftdebug.h */
  FT_BASE_DEF( ശൂന്യം )
  ft_remove_tag( സ്ഥിരം അക്ഷരം*  tag )
  {
    dlg_remove_tag( tag, ശൂന്യ );
  }


  /* documentation is in ftlogging.h */

  FT_EXPORT_DEF( ശൂന്യം )
  FT_Trace_Set_Level( സ്ഥിരം അക്ഷരം*  level )
  {
    ft_component_flag     = FALSE;
    ft_timestamp_flag     = FALSE;
    ft_custom_trace_level = level;

    ft_debug_init();
  }


  /* documentation is in ftlogging.h */

  FT_EXPORT_DEF( ശൂന്യം )
  FT_Trace_Set_Default_Level( ശൂന്യം )
  {
    ft_component_flag     = FALSE;
    ft_timestamp_flag     = FALSE;
    ft_custom_trace_level = ശൂന്യ;

    ft_debug_init();
  }


  /**************************************************************************
   *
   * Functions to handle a custom log handler.
   *
   */

  /* documentation is in ftlogging.h */

  FT_EXPORT_DEF( ശൂന്യം )
  FT_Set_Log_Handler( FT_Custom_Log_Handler  handler )
  {
    custom_output_handler = handler;
  }


  /* documentation is in ftlogging.h */

  FT_EXPORT_DEF( ശൂന്യം )
  FT_Set_Default_Log_Handler( ശൂന്യം )
  {
    custom_output_handler = ശൂന്യ;
  }


  /* documentation is in ftdebug.h */
  FT_BASE_DEF( ശൂന്യം )
  FT_Logging_Callback( സ്ഥിരം അക്ഷരം*  fmt,
                       ... )
  {
    va_list  ap;


    va_start( ap, fmt );
    custom_output_handler( ft_component, fmt, ap );
    va_end( ap );
  }

#അല്ലെങ്കിൽ /* !FT_DEBUG_LOGGING */

  FT_EXPORT_DEF( ശൂന്യം )
  FT_Trace_Set_Level( സ്ഥിരം അക്ഷരം*  level )
  {
    FT_UNUSED( level );
  }


  FT_EXPORT_DEF( ശൂന്യം )
  FT_Trace_Set_Default_Level( ശൂന്യം )
  {
    /* nothing */
  }


  FT_EXPORT_DEF( ശൂന്യം )
  FT_Set_Log_Handler( FT_Custom_Log_Handler  handler )
  {
    FT_UNUSED( handler );
  }


  FT_EXPORT_DEF( ശൂന്യം )
  FT_Set_Default_Log_Handler( ശൂന്യം )
  {
    /* nothing */
  }

#endif /* !FT_DEBUG_LOGGING */


/* END */
