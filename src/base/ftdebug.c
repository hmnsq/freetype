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


#ಅನ್ತರ್ಭಾವ <freetype/freetype.h>
#ಅನ್ತರ್ಭಾವ <freetype/ftlogging.h>
#ಅನ್ತರ್ಭಾವ <freetype/internal/ftdebug.h>
#ಅನ್ತರ್ಭಾವ <freetype/internal/ftobjs.h>


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
  ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ*  ft_default_trace_level = ರಿಕ್ತ;
  ಸ್ಥಿರ FILE*        ft_fileptr             = ರಿಕ್ತ;
  ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ*  ft_component           = ರಿಕ್ತ;
  ಸ್ಥಿರ FT_Bool      ft_component_flag      = FALSE;
  ಸ್ಥಿರ FT_Bool      ft_timestamp_flag      = FALSE;
  ಸ್ಥಿರ FT_Bool      ft_have_newline_char   = TRUE;
  ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ*  ft_custom_trace_level  = ರಿಕ್ತ;

  /* declared in ftdebug.h */

  dlg_handler            ft_default_log_handler = ರಿಕ್ತ;
  FT_Custom_Log_Handler  custom_output_handler  = ರಿಕ್ತ;

#endif /* FT_DEBUG_LOGGING */


#ifdef FT_DEBUG_LEVEL_ERROR

  /* documentation is in ftdebug.h */

  FT_BASE_DEF( ಶೂನ್ಯ )
  FT_Message( ಧ್ರುವ ವರ್ಣ*  fmt,
              ... )
  {
    va_list  ap;


    va_start( ap, fmt );
    vfprintf( stderr, fmt, ap );
    va_end( ap );
  }


  /* documentation is in ftdebug.h */

  FT_BASE_DEF( ಶೂನ್ಯ )
  FT_Panic( ಧ್ರುವ ವರ್ಣ*  fmt,
            ... )
  {
    va_list  ap;


    va_start( ap, fmt );
    vfprintf( stderr, fmt, ap );
    va_end( ap );

    exit( EXIT_FAILURE );
  }


  /* documentation is in ftdebug.h */

  FT_BASE_DEF( ಪೂರ್ಣಾಙ್ಕ )
  FT_Throw( FT_Error     error,
            ಪೂರ್ಣಾಙ್ಕ          ಸಾಲು,
            ಧ್ರುವ ವರ್ಣ*  file )
  {
#ಯದಿ 0
    /* activating the code in this block makes FreeType very chatty */
    fprintf( stderr,
             "%s:%d: error 0x%02x: %s\n",
             file,
             ಸಾಲು,
             error,
             FT_Error_String( error ) );
#ಅನ್ಯಥಾ
    FT_UNUSED( error );
    FT_UNUSED( ಸಾಲು );
    FT_UNUSED( file );
#endif

    ಪ್ರತ್ಯಾಗಮ 0;
  }

#endif /* FT_DEBUG_LEVEL_ERROR */


#ifdef FT_DEBUG_LEVEL_TRACE

  /* array of trace levels, initialized to 0; */
  /* this gets adjusted at run-time           */
  ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ  ft_trace_levels_enabled[trace_ಎಣಿಕೆ];

  /* array of trace levels, always initialized to 0 */
  ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ  ft_trace_levels_disabled[trace_ಎಣಿಕೆ];

  /* a pointer to either `ft_trace_levels_enabled' */
  /* or `ft_trace_levels_disabled'                 */
  ಪೂರ್ಣಾಙ್ಕ*  ft_trace_levels;

  /* define array of trace toggle names */
#define FT_TRACE_DEF( x )  #x ,

  ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ*  ft_trace_toggles[trace_ಎಣಿಕೆ + 1] =
  {
#ಅನ್ತರ್ಭಾವ <freetype/internal/fttrace.h>
    ರಿಕ್ತ
  };

#undef FT_TRACE_DEF


  /* documentation is in ftdebug.h */

  FT_BASE_DEF( FT_Int )
  FT_Trace_Get_ಎಣಿಕೆ( ಶೂನ್ಯ )
  {
    ಪ್ರತ್ಯಾಗಮ trace_ಎಣಿಕೆ;
  }


  /* documentation is in ftdebug.h */

  FT_BASE_DEF( ಧ್ರುವ ವರ್ಣ * )
  FT_Trace_Get_Name( FT_Int  idx )
  {
    ಪೂರ್ಣಾಙ್ಕ  max = FT_Trace_Get_ಎಣಿಕೆ();


    ಯದಿ ( idx < max )
      ಪ್ರತ್ಯಾಗಮ ft_trace_toggles[idx];
    ಅನ್ಯಥಾ
      ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
  }


  /* documentation is in ftdebug.h */

  FT_BASE_DEF( ಶೂನ್ಯ )
  FT_Trace_Disable( ಶೂನ್ಯ )
  {
    ft_trace_levels = ft_trace_levels_disabled;
  }


  /* documentation is in ftdebug.h */

  FT_BASE_DEF( ಶೂನ್ಯ )
  FT_Trace_Enable( ಶೂನ್ಯ )
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
  FT_BASE_DEF( ಶೂನ್ಯ )
  ft_debug_init( ಶೂನ್ಯ )
  {
    ಧ್ರುವ ವರ್ಣ*  ft2_debug = ರಿಕ್ತ;


#ifdef FT_DEBUG_LOGGING
    ಯದಿ ( ft_custom_trace_level != ರಿಕ್ತ )
      ft2_debug = ft_custom_trace_level;
    ಅನ್ಯಥಾ
      ft2_debug = ft_default_trace_level;
#ಅನ್ಯಥಾ
    ft2_debug = ft_getenv( "FT2_DEBUG" );
#endif

    ಯದಿ ( ft2_debug )
    {
      ಧ್ರುವ ವರ್ಣ*  p = ft2_debug;
      ಧ್ರುವ ವರ್ಣ*  q;


      ಪ್ರತಿ ( ; *p; p++ )
      {
        /* skip leading whitespace and separators */
        ಯದಿ ( *p == ' ' || *p == '\t' || *p == ',' || *p == ';' || *p == '=' )
          ಅನುವರ್ತ;

#ifdef FT_DEBUG_LOGGING

        /* check extra arguments for logging */
        ಯದಿ ( *p == '-' )
        {
          ಧ್ರುವ ವರ್ಣ*  r = ++p;


          ಯದಿ ( *r == 'v' )
          {
            ಧ್ರುವ ವರ್ಣ*  s = ++r;


            ft_component_flag = TRUE;

            ಯದಿ ( *s == 't' )
            {
              ft_timestamp_flag = TRUE;
              p++;
            }

            p++;
          }

          ಅನ್ಯಥಾ ಯದಿ ( *r == 't' )
          {
            ಧ್ರುವ ವರ್ಣ*  s = ++r;


            ft_timestamp_flag = TRUE;

            ಯದಿ ( *s == 'v' )
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
        ಯಾವತ್ ( *p && *p != ':' )
          p++;

        ಯದಿ ( !*p )
          ಭಙ್ಗ;

        ಯದಿ ( *p == ':' && p > q )
        {
          FT_Int  n, i, len = (FT_Int)( p - q );
          FT_Int  level = -1, found = -1;


          ಪ್ರತಿ ( n = 0; n < trace_ಎಣಿಕೆ; n++ )
          {
            ಧ್ರುವ ವರ್ಣ*  toggle = ft_trace_toggles[n];


            ಪ್ರತಿ ( i = 0; i < len; i++ )
            {
              ಯದಿ ( toggle[i] != q[i] )
                ಭಙ್ಗ;
            }

            ಯದಿ ( i == len && toggle[i] == 0 )
            {
              found = n;
              ಭಙ್ಗ;
            }
          }

          /* read level */
          p++;
          ಯದಿ ( *p )
          {
            level = *p - '0';
            ಯದಿ ( level < 0 || level > 7 )
              level = -1;
          }

          ಯದಿ ( found >= 0 && level >= 0 )
          {
            ಯದಿ ( found == trace_any )
            {
              /* special case for `any' */
              ಪ್ರತಿ ( n = 0; n < trace_ಎಣಿಕೆ; n++ )
                ft_trace_levels_enabled[n] = level;
            }
            ಅನ್ಯಥಾ
              ft_trace_levels_enabled[found] = level;
          }
        }
      }
    }

    ft_trace_levels = ft_trace_levels_enabled;
  }


#ಅನ್ಯಥಾ  /* !FT_DEBUG_LEVEL_TRACE */


  FT_BASE_DEF( ಶೂನ್ಯ )
  ft_debug_init( ಶೂನ್ಯ )
  {
    /* nothing */
  }


  FT_BASE_DEF( FT_Int )
  FT_Trace_Get_ಎಣಿಕೆ( ಶೂನ್ಯ )
  {
    ಪ್ರತ್ಯಾಗಮ 0;
  }


  FT_BASE_DEF( ಧ್ರುವ ವರ್ಣ * )
  FT_Trace_Get_Name( FT_Int  idx )
  {
    FT_UNUSED( idx );

    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
  }


  FT_BASE_DEF( ಶೂನ್ಯ )
  FT_Trace_Disable( ಶೂನ್ಯ )
  {
    /* nothing */
  }


  /* documentation is in ftdebug.h */

  FT_BASE_DEF( ಶೂನ್ಯ )
  FT_Trace_Enable( ಶೂನ್ಯ )
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

  FT_BASE_DEF( ಶೂನ್ಯ )
  ft_logging_init( ಶೂನ್ಯ )
  {
    ft_default_log_handler = ft_log_handler;
    ft_default_trace_level = ft_getenv( "FT2_DEBUG" );

    ಯದಿ ( ft_getenv( "FT_LOGGING_FILE" ) )
      ft_fileptr = ft_fopen( ft_getenv( "FT_LOGGING_FILE" ), "w" );
    ಅನ್ಯಥಾ
      ft_fileptr = stderr;

    ft_debug_init();

    /* Set the default output handler for 'dlg'. */
    dlg_set_handler( ft_default_log_handler, ರಿಕ್ತ );
  }


  FT_BASE_DEF( ಶೂನ್ಯ )
  ft_logging_deinit( ಶೂನ್ಯ )
  {
    ಯದಿ ( ft_fileptr != stderr )
      ft_fclose( ft_fileptr );
  }


  /**************************************************************************
   *
   * An output log handler for FreeType.
   *
   */
  FT_BASE_DEF( ಶೂನ್ಯ )
  ft_log_handler( ಧ್ರುವ ಸಂರಚನಾ dlg_origin*  origin,
                  ಧ್ರುವ ವರ್ಣ*               string,
                  ಶೂನ್ಯ*                     data )
  {
    ವರ್ಣ         features_buf[128];
    ವರ್ಣ*        bufp = features_buf;

    FT_UNUSED( data );


    ಯದಿ ( ft_have_newline_char )
    {
      ಧ್ರುವ ವರ್ಣ*  features        = ರಿಕ್ತ;
      size_t       features_length = 0;


#define FEATURES_TIMESTAMP            "[%h:%m] "
#define FEATURES_COMPONENT            "[%t] "
#define FEATURES_TIMESTAMP_COMPONENT  "[%h:%m %t] "

      ಯದಿ ( ft_timestamp_flag && ft_component_flag )
      {
        features        = FEATURES_TIMESTAMP_COMPONENT;
        features_length = ಪರಿಮಾಣ ( FEATURES_TIMESTAMP_COMPONENT );
      }
      ಅನ್ಯಥಾ ಯದಿ ( ft_timestamp_flag )
      {
        features        = FEATURES_TIMESTAMP;
        features_length = ಪರಿಮಾಣ ( FEATURES_TIMESTAMP );
      }
      ಅನ್ಯಥಾ ಯದಿ ( ft_component_flag )
      {
        features        = FEATURES_COMPONENT;
        features_length = ಪರಿಮಾಣ ( FEATURES_COMPONENT );
      }

      ಯದಿ ( ft_component_flag || ft_timestamp_flag )
      {
        ft_strncpy( features_buf, features, features_length );
        bufp += features_length - 1;
      }

      ಯದಿ ( ft_component_flag )
      {
        size_t  tag_length = ft_strlen( *origin->tags );
        size_t  i;


        /* To vertically align tracing messages we compensate the */
        /* different FT_COMPONENT string lengths by inserting an  */
        /* appropriate amount of space characters.                */
        ಪ್ರತಿ ( i = 0;
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
                                (ಧ್ರುವ ವರ್ಣ*)features_buf,
                                origin,
                                string,
                                dlg_default_output_styles,
                                true );

    ಯದಿ ( ft_strrchr( string, '\n' ) )
      ft_have_newline_char = TRUE;
    ಅನ್ಯಥಾ
      ft_have_newline_char = FALSE;
  }


  /* documentation is in ftdebug.h */
  FT_BASE_DEF( ಶೂನ್ಯ )
  ft_add_tag( ಧ್ರುವ ವರ್ಣ*  tag )
  {
    ft_component = tag;

    dlg_add_tag( tag, ರಿಕ್ತ );
  }


  /* documentation is in ftdebug.h */
  FT_BASE_DEF( ಶೂನ್ಯ )
  ft_remove_tag( ಧ್ರುವ ವರ್ಣ*  tag )
  {
    dlg_remove_tag( tag, ರಿಕ್ತ );
  }


  /* documentation is in ftlogging.h */

  FT_EXPORT_DEF( ಶೂನ್ಯ )
  FT_Trace_Set_Level( ಧ್ರುವ ವರ್ಣ*  level )
  {
    ft_component_flag     = FALSE;
    ft_timestamp_flag     = FALSE;
    ft_custom_trace_level = level;

    ft_debug_init();
  }


  /* documentation is in ftlogging.h */

  FT_EXPORT_DEF( ಶೂನ್ಯ )
  FT_Trace_Set_Default_Level( ಶೂನ್ಯ )
  {
    ft_component_flag     = FALSE;
    ft_timestamp_flag     = FALSE;
    ft_custom_trace_level = ರಿಕ್ತ;

    ft_debug_init();
  }


  /**************************************************************************
   *
   * Functions to handle a custom log handler.
   *
   */

  /* documentation is in ftlogging.h */

  FT_EXPORT_DEF( ಶೂನ್ಯ )
  FT_Set_Log_Handler( FT_Custom_Log_Handler  handler )
  {
    custom_output_handler = handler;
  }


  /* documentation is in ftlogging.h */

  FT_EXPORT_DEF( ಶೂನ್ಯ )
  FT_Set_Default_Log_Handler( ಶೂನ್ಯ )
  {
    custom_output_handler = ರಿಕ್ತ;
  }


  /* documentation is in ftdebug.h */
  FT_BASE_DEF( ಶೂನ್ಯ )
  FT_Logging_Callback( ಧ್ರುವ ವರ್ಣ*  fmt,
                       ... )
  {
    va_list  ap;


    va_start( ap, fmt );
    custom_output_handler( ft_component, fmt, ap );
    va_end( ap );
  }

#ಅನ್ಯಥಾ /* !FT_DEBUG_LOGGING */

  FT_EXPORT_DEF( ಶೂನ್ಯ )
  FT_Trace_Set_Level( ಧ್ರುವ ವರ್ಣ*  level )
  {
    FT_UNUSED( level );
  }


  FT_EXPORT_DEF( ಶೂನ್ಯ )
  FT_Trace_Set_Default_Level( ಶೂನ್ಯ )
  {
    /* nothing */
  }


  FT_EXPORT_DEF( ಶೂನ್ಯ )
  FT_Set_Log_Handler( FT_Custom_Log_Handler  handler )
  {
    FT_UNUSED( handler );
  }


  FT_EXPORT_DEF( ಶೂನ್ಯ )
  FT_Set_Default_Log_Handler( ಶೂನ್ಯ )
  {
    /* nothing */
  }

#endif /* !FT_DEBUG_LOGGING */


/* END */
