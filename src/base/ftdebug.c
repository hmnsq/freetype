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


#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ FT_DEBUG_LOGGING

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
  ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*  ft_default_trace_level = ಖಾಲಿ;
  ಸ್ಥಿರ FILE*        ft_fileptr             = ಖಾಲಿ;
  ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*  ft_component           = ಖಾಲಿ;
  ಸ್ಥಿರ FT_Bool      ft_component_flag      = FALSE;
  ಸ್ಥಿರ FT_Bool      ft_timestamp_flag      = FALSE;
  ಸ್ಥಿರ FT_Bool      ft_have_newline_char   = TRUE;
  ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*  ft_custom_trace_level  = ಖಾಲಿ;

  /* declared in ftdebug.h */

  dlg_handler            ft_default_log_handler = ಖಾಲಿ;
  FT_Custom_Log_Handler  custom_output_handler  = ಖಾಲಿ;

#ಆದರೆ_ಅಂತ್ಯ /* FT_DEBUG_LOGGING */


#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ FT_DEBUG_LEVEL_ERROR

  /* documentation is in ftdebug.h */

  FT_ತಳ_DEF( ಶೂನ್ಯ )
  FT_ಸಂದೇಶ( ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*  fmt,
              ... )
  {
    va_list  ap;


    va_ಆರಂಭ( ap, fmt );
    vfprintf( stderr, fmt, ap );
    va_ಕೊನೆ( ap );
  }


  /* documentation is in ftdebug.h */

  FT_ತಳ_DEF( ಶೂನ್ಯ )
  FT_Panic( ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*  fmt,
            ... )
  {
    va_list  ap;


    va_ಆರಂಭ( ap, fmt );
    vfprintf( stderr, fmt, ap );
    va_ಕೊನೆ( ap );

    ನಿರ್ಗಮಿಸು( EXIT_FAILURE );
  }


  /* documentation is in ftdebug.h */

  FT_ತಳ_DEF( ಪೂರ್ಣಾಂಕ )
  FT_Throw( FT_Error     error,
            ಪೂರ್ಣಾಂಕ          ಸಾಲು,
            ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*  file )
  {
#ಆದರೆ 0
    /* activating the code in this block makes FreeType very chatty */
    fprintf( stderr,
             "%s:%d: error 0x%02x: %s\n",
             file,
             ಸಾಲು,
             error,
             FT_Error_String( error ) );
#ಇಲ್ಲದಿದ್ದರೆ
    FT_ಬಳಸದ( error );
    FT_ಬಳಸದ( ಸಾಲು );
    FT_ಬಳಸದ( file );
#ಆದರೆ_ಅಂತ್ಯ

    ಹಿಂತಿರುಗಿ 0;
  }

#ಆದರೆ_ಅಂತ್ಯ /* FT_DEBUG_LEVEL_ERROR */


#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ FT_DEBUG_LEVEL_TRACE

  /* array of trace levels, initialized to 0; */
  /* this gets adjusted at run-time           */
  ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ  ft_trace_ಮಟ್ಟಗಳು_enabled[trace_ಎಣಿಕೆ];

  /* array of trace levels, always initialized to 0 */
  ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ  ft_trace_ಮಟ್ಟಗಳು_disabled[trace_ಎಣಿಕೆ];

  /* a pointer to either `ft_trace_levels_enabled' */
  /* or `ft_trace_levels_disabled'                 */
  ಪೂರ್ಣಾಂಕ*  ft_trace_ಮಟ್ಟಗಳು;

  /* define array of trace toggle names */
#ವ್ಯಾಖ್ಯಾನಿಸು FT_TRACE_DEF( x )  #x ,

  ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*  ft_trace_toggles[trace_ಎಣಿಕೆ + 1] =
  {
#ಅನ್ತರ್ಭಾವ <freetype/internal/fttrace.h>
    ಖಾಲಿ
  };

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು FT_TRACE_DEF


  /* documentation is in ftdebug.h */

  FT_ತಳ_DEF( FT_Int )
  FT_Trace_Get_ಎಣಿಕೆ( ಶೂನ್ಯ )
  {
    ಹಿಂತಿರುಗಿ trace_ಎಣಿಕೆ;
  }


  /* documentation is in ftdebug.h */

  FT_ತಳ_DEF( ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * )
  FT_Trace_Get_Name( FT_Int  idx )
  {
    ಪೂರ್ಣಾಂಕ  max = FT_Trace_Get_ಎಣಿಕೆ();


    ಆದರೆ ( idx < max )
      ಹಿಂತಿರುಗಿ ft_trace_toggles[idx];
    ಇಲ್ಲದಿದ್ದರೆ
      ಹಿಂತಿರುಗಿ ಖಾಲಿ;
  }


  /* documentation is in ftdebug.h */

  FT_ತಳ_DEF( ಶೂನ್ಯ )
  FT_Trace_Disable( ಶೂನ್ಯ )
  {
    ft_trace_ಮಟ್ಟಗಳು = ft_trace_ಮಟ್ಟಗಳು_disabled;
  }


  /* documentation is in ftdebug.h */

  FT_ತಳ_DEF( ಶೂನ್ಯ )
  FT_Trace_Enable( ಶೂನ್ಯ )
  {
    ft_trace_ಮಟ್ಟಗಳು = ft_trace_ಮಟ್ಟಗಳು_enabled;
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
  FT_ತಳ_DEF( ಶೂನ್ಯ )
  ft_debug_init( ಶೂನ್ಯ )
  {
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*  ft2_debug = ಖಾಲಿ;


#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ FT_DEBUG_LOGGING
    ಆದರೆ ( ft_custom_trace_level != ಖಾಲಿ )
      ft2_debug = ft_custom_trace_level;
    ಇಲ್ಲದಿದ್ದರೆ
      ft2_debug = ft_default_trace_level;
#ಇಲ್ಲದಿದ್ದರೆ
    ft2_debug = ft_getenv( "FT2_DEBUG" );
#ಆದರೆ_ಅಂತ್ಯ

    ಆದರೆ ( ft2_debug )
    {
      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*  p = ft2_debug;
      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*  q;


      ಗಾಗಿ ( ; *p; p++ )
      {
        /* skip leading whitespace and separators */
        ಆದರೆ ( *p == ' ' || *p == '\t' || *p == ',' || *p == ';' || *p == '=' )
          ಮುಂದುವರಿ;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ FT_DEBUG_LOGGING

        /* check extra arguments for logging */
        ಆದರೆ ( *p == '-' )
        {
          ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*  r = ++p;


          ಆದರೆ ( *r == 'v' )
          {
            ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*  s = ++r;


            ft_component_flag = TRUE;

            ಆದರೆ ( *s == 't' )
            {
              ft_timestamp_flag = TRUE;
              p++;
            }

            p++;
          }

          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ( *r == 't' )
          {
            ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*  s = ++r;


            ft_timestamp_flag = TRUE;

            ಆದರೆ ( *s == 'v' )
            {
              ft_component_flag = TRUE;
              p++;
            }

            p++;
          }
        }

#ಆದರೆ_ಅಂತ್ಯ /* FT_DEBUG_LOGGING */

        /* read toggle name, followed by ':' */
        q = p;
        ಆಗ ( *p && *p != ':' )
          p++;

        ಆದರೆ ( !*p )
          ನಿಲ್ಲಿಸು;

        ಆದರೆ ( *p == ':' && p > q )
        {
          FT_Int  n, i, len = (FT_Int)( p - q );
          FT_Int  level = -1, ಸಿಕ್ಕಿತು = -1;


          ಗಾಗಿ ( n = 0; n < trace_ಎಣಿಕೆ; n++ )
          {
            ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*  toggle = ft_trace_toggles[n];


            ಗಾಗಿ ( i = 0; i < len; i++ )
            {
              ಆದರೆ ( toggle[i] != q[i] )
                ನಿಲ್ಲಿಸು;
            }

            ಆದರೆ ( i == len && toggle[i] == 0 )
            {
              ಸಿಕ್ಕಿತು = n;
              ನಿಲ್ಲಿಸು;
            }
          }

          /* read level */
          p++;
          ಆದರೆ ( *p )
          {
            level = *p - '0';
            ಆದರೆ ( level < 0 || level > 7 )
              level = -1;
          }

          ಆದರೆ ( ಸಿಕ್ಕಿತು >= 0 && level >= 0 )
          {
            ಆದರೆ ( ಸಿಕ್ಕಿತು == trace_any )
            {
              /* special case for `any' */
              ಗಾಗಿ ( n = 0; n < trace_ಎಣಿಕೆ; n++ )
                ft_trace_ಮಟ್ಟಗಳು_enabled[n] = level;
            }
            ಇಲ್ಲದಿದ್ದರೆ
              ft_trace_ಮಟ್ಟಗಳು_enabled[ಸಿಕ್ಕಿತು] = level;
          }
        }
      }
    }

    ft_trace_ಮಟ್ಟಗಳು = ft_trace_ಮಟ್ಟಗಳು_enabled;
  }


#ಇಲ್ಲದಿದ್ದರೆ  /* !FT_DEBUG_LEVEL_TRACE */


  FT_ತಳ_DEF( ಶೂನ್ಯ )
  ft_debug_init( ಶೂನ್ಯ )
  {
    /* nothing */
  }


  FT_ತಳ_DEF( FT_Int )
  FT_Trace_Get_ಎಣಿಕೆ( ಶೂನ್ಯ )
  {
    ಹಿಂತಿರುಗಿ 0;
  }


  FT_ತಳ_DEF( ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * )
  FT_Trace_Get_Name( FT_Int  idx )
  {
    FT_ಬಳಸದ( idx );

    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
  }


  FT_ತಳ_DEF( ಶೂನ್ಯ )
  FT_Trace_Disable( ಶೂನ್ಯ )
  {
    /* nothing */
  }


  /* documentation is in ftdebug.h */

  FT_ತಳ_DEF( ಶೂನ್ಯ )
  FT_Trace_Enable( ಶೂನ್ಯ )
  {
    /* nothing */
  }

#ಆದರೆ_ಅಂತ್ಯ /* !FT_DEBUG_LEVEL_TRACE */


#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ FT_DEBUG_LOGGING

  /**************************************************************************
   *
   * Initialize and de-initialize 'dlg' library.
   *
   */

  FT_ತಳ_DEF( ಶೂನ್ಯ )
  ft_logging_init( ಶೂನ್ಯ )
  {
    ft_default_log_handler = ft_log_handler;
    ft_default_trace_level = ft_getenv( "FT2_DEBUG" );

    ಆದರೆ ( ft_getenv( "FT_LOGGING_FILE" ) )
      ft_fileptr = ft_fopen( ft_getenv( "FT_LOGGING_FILE" ), "w" );
    ಇಲ್ಲದಿದ್ದರೆ
      ft_fileptr = stderr;

    ft_debug_init();

    /* Set the default output handler for 'dlg'. */
    dlg_ಗಣ_handler( ft_default_log_handler, ಖಾಲಿ );
  }


  FT_ತಳ_DEF( ಶೂನ್ಯ )
  ft_logging_deinit( ಶೂನ್ಯ )
  {
    ಆದರೆ ( ft_fileptr != stderr )
      ft_fclose( ft_fileptr );
  }


  /**************************************************************************
   *
   * An output log handler for FreeType.
   *
   */
  FT_ತಳ_DEF( ಶೂನ್ಯ )
  ft_log_handler( ಸ್ಥಿರಾಂಕ ರಚನೆ dlg_origin*  origin,
                  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*               string,
                  ಶೂನ್ಯ*                     ದತ್ತಾಂಶ )
  {
    ಅಕ್ಷರ         features_buf[128];
    ಅಕ್ಷರ*        bufp = features_buf;

    FT_ಬಳಸದ( ದತ್ತಾಂಶ );


    ಆದರೆ ( ft_have_newline_char )
    {
      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*  features        = ಖಾಲಿ;
      size_t       features_ಉದ್ದಳತೆ = 0;


#ವ್ಯಾಖ್ಯಾನಿಸು FEATURES_TIMESTAMP            "[%h:%m] "
#ವ್ಯಾಖ್ಯಾನಿಸು FEATURES_COMPONENT            "[%t] "
#ವ್ಯಾಖ್ಯಾನಿಸು FEATURES_TIMESTAMP_COMPONENT  "[%h:%m %t] "

      ಆದರೆ ( ft_timestamp_flag && ft_component_flag )
      {
        features        = FEATURES_TIMESTAMP_COMPONENT;
        features_ಉದ್ದಳತೆ = ಗಾತ್ರ ( FEATURES_TIMESTAMP_COMPONENT );
      }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ( ft_timestamp_flag )
      {
        features        = FEATURES_TIMESTAMP;
        features_ಉದ್ದಳತೆ = ಗಾತ್ರ ( FEATURES_TIMESTAMP );
      }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ( ft_component_flag )
      {
        features        = FEATURES_COMPONENT;
        features_ಉದ್ದಳತೆ = ಗಾತ್ರ ( FEATURES_COMPONENT );
      }

      ಆದರೆ ( ft_component_flag || ft_timestamp_flag )
      {
        ft_strncpy( features_buf, features, features_ಉದ್ದಳತೆ );
        bufp += features_ಉದ್ದಳತೆ - 1;
      }

      ಆದರೆ ( ft_component_flag )
      {
        size_t  ಟ್ಯಾಗ್_ಉದ್ದಳತೆ = ft_strlen( *origin->ಟ್ಯಾಗುಗಳು );
        size_t  i;


        /* To vertically align tracing messages we compensate the */
        /* different FT_COMPONENT string lengths by inserting an  */
        /* appropriate amount of space characters.                */
        ಗಾಗಿ ( i = 0;
              i < FT_MAX_TRACE_LEVEL_ಉದ್ದಳತೆ - ಟ್ಯಾಗ್_ಉದ್ದಳತೆ;
              i++ )
          *bufp++ = ' ';
      }
    }

    /* Finally add the format string for the tracing message. */
    *bufp++ = '%';
    *bufp++ = 'c';
    *bufp   = '\0';

    dlg_generic_outputf_ಹರಿವು( ft_fileptr,
                                (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*)features_buf,
                                origin,
                                string,
                                dlg_default_output_styles,
                                true );

    ಆದರೆ ( ft_strrchr( string, '\n' ) )
      ft_have_newline_char = TRUE;
    ಇಲ್ಲದಿದ್ದರೆ
      ft_have_newline_char = FALSE;
  }


  /* documentation is in ftdebug.h */
  FT_ತಳ_DEF( ಶೂನ್ಯ )
  ft_ಕೂಡಿಸು_ಟ್ಯಾಗ್( ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*  ಟ್ಯಾಗ್ )
  {
    ft_component = ಟ್ಯಾಗ್;

    dlg_ಕೂಡಿಸು_ಟ್ಯಾಗ್( ಟ್ಯಾಗ್, ಖಾಲಿ );
  }


  /* documentation is in ftdebug.h */
  FT_ತಳ_DEF( ಶೂನ್ಯ )
  ft_remove_ಟ್ಯಾಗ್( ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*  ಟ್ಯಾಗ್ )
  {
    dlg_remove_ಟ್ಯಾಗ್( ಟ್ಯಾಗ್, ಖಾಲಿ );
  }


  /* documentation is in ftlogging.h */

  FT_EXPORT_DEF( ಶೂನ್ಯ )
  FT_Trace_ಗಣ_Level( ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*  level )
  {
    ft_component_flag     = FALSE;
    ft_timestamp_flag     = FALSE;
    ft_custom_trace_level = level;

    ft_debug_init();
  }


  /* documentation is in ftlogging.h */

  FT_EXPORT_DEF( ಶೂನ್ಯ )
  FT_Trace_ಗಣ_Default_Level( ಶೂನ್ಯ )
  {
    ft_component_flag     = FALSE;
    ft_timestamp_flag     = FALSE;
    ft_custom_trace_level = ಖಾಲಿ;

    ft_debug_init();
  }


  /**************************************************************************
   *
   * Functions to handle a custom log handler.
   *
   */

  /* documentation is in ftlogging.h */

  FT_EXPORT_DEF( ಶೂನ್ಯ )
  FT_ಗಣ_Log_Handler( FT_Custom_Log_Handler  handler )
  {
    custom_output_handler = handler;
  }


  /* documentation is in ftlogging.h */

  FT_EXPORT_DEF( ಶೂನ್ಯ )
  FT_ಗಣ_Default_Log_Handler( ಶೂನ್ಯ )
  {
    custom_output_handler = ಖಾಲಿ;
  }


  /* documentation is in ftdebug.h */
  FT_ತಳ_DEF( ಶೂನ್ಯ )
  FT_Logging_Callback( ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*  fmt,
                       ... )
  {
    va_list  ap;


    va_ಆರಂಭ( ap, fmt );
    custom_output_handler( ft_component, fmt, ap );
    va_ಕೊನೆ( ap );
  }

#ಇಲ್ಲದಿದ್ದರೆ /* !FT_DEBUG_LOGGING */

  FT_EXPORT_DEF( ಶೂನ್ಯ )
  FT_Trace_ಗಣ_Level( ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*  level )
  {
    FT_ಬಳಸದ( level );
  }


  FT_EXPORT_DEF( ಶೂನ್ಯ )
  FT_Trace_ಗಣ_Default_Level( ಶೂನ್ಯ )
  {
    /* nothing */
  }


  FT_EXPORT_DEF( ಶೂನ್ಯ )
  FT_ಗಣ_Log_Handler( FT_Custom_Log_Handler  handler )
  {
    FT_ಬಳಸದ( handler );
  }


  FT_EXPORT_DEF( ಶೂನ್ಯ )
  FT_ಗಣ_Default_Log_Handler( ಶೂನ್ಯ )
  {
    /* nothing */
  }

#ಆದರೆ_ಅಂತ್ಯ /* !FT_DEBUG_LOGGING */


/*ಕೊನೆ*/
