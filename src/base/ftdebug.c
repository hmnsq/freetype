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


#ଅନ୍ତର୍ଭାଵ <freetype/freetype.h>
#ଅନ୍ତର୍ଭାଵ <freetype/ftlogging.h>
#ଅନ୍ତର୍ଭାଵ <freetype/internal/ftdebug.h>
#ଅନ୍ତର୍ଭାଵ <freetype/internal/ftobjs.h>


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
  ସ୍ଥିର ଅଚଳ ଅକ୍ଷର*  ft_default_trace_level = ଖାଲି;
  ସ୍ଥିର FILE*        ft_fileptr             = ଖାଲି;
  ସ୍ଥିର ଅଚଳ ଅକ୍ଷର*  ft_component           = ଖାଲି;
  ସ୍ଥିର FT_Bool      ft_component_flag      = FALSE;
  ସ୍ଥିର FT_Bool      ft_timestamp_flag      = FALSE;
  ସ୍ଥିର FT_Bool      ft_have_newline_char   = TRUE;
  ସ୍ଥିର ଅଚଳ ଅକ୍ଷର*  ft_custom_trace_level  = ଖାଲି;

  /* declared in ftdebug.h */

  dlg_handler            ft_default_log_handler = ଖାଲି;
  FT_Custom_Log_Handler  custom_output_handler  = ଖାଲି;

#endif /* FT_DEBUG_LOGGING */


#ifdef FT_DEBUG_LEVEL_ERROR

  /* documentation is in ftdebug.h */

  FT_BASE_DEF( ଶୂନ୍ୟ )
  FT_Message( ଅଚଳ ଅକ୍ଷର*  fmt,
              ... )
  {
    va_list  ap;


    va_start( ap, fmt );
    vfprintf( stderr, fmt, ap );
    va_end( ap );
  }


  /* documentation is in ftdebug.h */

  FT_BASE_DEF( ଶୂନ୍ୟ )
  FT_Panic( ଅଚଳ ଅକ୍ଷର*  fmt,
            ... )
  {
    va_list  ap;


    va_start( ap, fmt );
    vfprintf( stderr, fmt, ap );
    va_end( ap );

    exit( EXIT_FAILURE );
  }


  /* documentation is in ftdebug.h */

  FT_BASE_DEF( ପୂର୍ଣ୍ଣସଂଖ୍ୟା )
  FT_Throw( FT_Error     error,
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା          ଧାଡ଼ି,
            ଅଚଳ ଅକ୍ଷର*  file )
  {
#ଯଦି 0
    /* activating the code in this block makes FreeType very chatty */
    fprintf( stderr,
             "%s:%d: error 0x%02x: %s\n",
             file,
             ଧାଡ଼ି,
             error,
             FT_Error_String( error ) );
#ଅନ୍ୟଥା
    FT_UNUSED( error );
    FT_UNUSED( ଧାଡ଼ି );
    FT_UNUSED( file );
#endif

    ଫେରନ୍ତୁ 0;
  }

#endif /* FT_DEBUG_LEVEL_ERROR */


#ifdef FT_DEBUG_LEVEL_TRACE

  /* array of trace levels, initialized to 0; */
  /* this gets adjusted at run-time           */
  ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା  ft_trace_levels_enabled[trace_ଗଣନା];

  /* array of trace levels, always initialized to 0 */
  ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା  ft_trace_levels_disabled[trace_ଗଣନା];

  /* a pointer to either `ft_trace_levels_enabled' */
  /* or `ft_trace_levels_disabled'                 */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା*  ft_trace_levels;

  /* define array of trace toggle names */
#define FT_TRACE_DEF( x )  #x ,

  ସ୍ଥିର ଅଚଳ ଅକ୍ଷର*  ft_trace_toggles[trace_ଗଣନା + 1] =
  {
#ଅନ୍ତର୍ଭାଵ <freetype/internal/fttrace.h>
    ଖାଲି
  };

#undef FT_TRACE_DEF


  /* documentation is in ftdebug.h */

  FT_BASE_DEF( FT_Int )
  FT_Trace_Get_ଗଣନା( ଶୂନ୍ୟ )
  {
    ଫେରନ୍ତୁ trace_ଗଣନା;
  }


  /* documentation is in ftdebug.h */

  FT_BASE_DEF( ଅଚଳ ଅକ୍ଷର * )
  FT_Trace_Get_Name( FT_Int  idx )
  {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା  max = FT_Trace_Get_ଗଣନା();


    ଯଦି ( idx < max )
      ଫେରନ୍ତୁ ft_trace_toggles[idx];
    ଅନ୍ୟଥା
      ଫେରନ୍ତୁ ଖାଲି;
  }


  /* documentation is in ftdebug.h */

  FT_BASE_DEF( ଶୂନ୍ୟ )
  FT_Trace_Disable( ଶୂନ୍ୟ )
  {
    ft_trace_levels = ft_trace_levels_disabled;
  }


  /* documentation is in ftdebug.h */

  FT_BASE_DEF( ଶୂନ୍ୟ )
  FT_Trace_Enable( ଶୂନ୍ୟ )
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
  FT_BASE_DEF( ଶୂନ୍ୟ )
  ft_debug_init( ଶୂନ୍ୟ )
  {
    ଅଚଳ ଅକ୍ଷର*  ft2_debug = ଖାଲି;


#ifdef FT_DEBUG_LOGGING
    ଯଦି ( ft_custom_trace_level != ଖାଲି )
      ft2_debug = ft_custom_trace_level;
    ଅନ୍ୟଥା
      ft2_debug = ft_default_trace_level;
#ଅନ୍ୟଥା
    ft2_debug = ft_getenv( "FT2_DEBUG" );
#endif

    ଯଦି ( ft2_debug )
    {
      ଅଚଳ ଅକ୍ଷର*  p = ft2_debug;
      ଅଚଳ ଅକ୍ଷର*  q;


      ପାଇଁ ( ; *p; p++ )
      {
        /* skip leading whitespace and separators */
        ଯଦି ( *p == ' ' || *p == '\t' || *p == ',' || *p == ';' || *p == '=' )
          ଜାରି;

#ifdef FT_DEBUG_LOGGING

        /* check extra arguments for logging */
        ଯଦି ( *p == '-' )
        {
          ଅଚଳ ଅକ୍ଷର*  r = ++p;


          ଯଦି ( *r == 'v' )
          {
            ଅଚଳ ଅକ୍ଷର*  s = ++r;


            ft_component_flag = TRUE;

            ଯଦି ( *s == 't' )
            {
              ft_timestamp_flag = TRUE;
              p++;
            }

            p++;
          }

          ଅନ୍ୟଥା ଯଦି ( *r == 't' )
          {
            ଅଚଳ ଅକ୍ଷର*  s = ++r;


            ft_timestamp_flag = TRUE;

            ଯଦି ( *s == 'v' )
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
        ଯେତେବେଳେ ( *p && *p != ':' )
          p++;

        ଯଦି ( !*p )
          ଭାଙ୍ଗ;

        ଯଦି ( *p == ':' && p > q )
        {
          FT_Int  n, i, len = (FT_Int)( p - q );
          FT_Int  level = -1, found = -1;


          ପାଇଁ ( n = 0; n < trace_ଗଣନା; n++ )
          {
            ଅଚଳ ଅକ୍ଷର*  toggle = ft_trace_toggles[n];


            ପାଇଁ ( i = 0; i < len; i++ )
            {
              ଯଦି ( toggle[i] != q[i] )
                ଭାଙ୍ଗ;
            }

            ଯଦି ( i == len && toggle[i] == 0 )
            {
              found = n;
              ଭାଙ୍ଗ;
            }
          }

          /* read level */
          p++;
          ଯଦି ( *p )
          {
            level = *p - '0';
            ଯଦି ( level < 0 || level > 7 )
              level = -1;
          }

          ଯଦି ( found >= 0 && level >= 0 )
          {
            ଯଦି ( found == trace_any )
            {
              /* special case for `any' */
              ପାଇଁ ( n = 0; n < trace_ଗଣନା; n++ )
                ft_trace_levels_enabled[n] = level;
            }
            ଅନ୍ୟଥା
              ft_trace_levels_enabled[found] = level;
          }
        }
      }
    }

    ft_trace_levels = ft_trace_levels_enabled;
  }


#ଅନ୍ୟଥା  /* !FT_DEBUG_LEVEL_TRACE */


  FT_BASE_DEF( ଶୂନ୍ୟ )
  ft_debug_init( ଶୂନ୍ୟ )
  {
    /* nothing */
  }


  FT_BASE_DEF( FT_Int )
  FT_Trace_Get_ଗଣନା( ଶୂନ୍ୟ )
  {
    ଫେରନ୍ତୁ 0;
  }


  FT_BASE_DEF( ଅଚଳ ଅକ୍ଷର * )
  FT_Trace_Get_Name( FT_Int  idx )
  {
    FT_UNUSED( idx );

    ଫେରନ୍ତୁ ଖାଲି;
  }


  FT_BASE_DEF( ଶୂନ୍ୟ )
  FT_Trace_Disable( ଶୂନ୍ୟ )
  {
    /* nothing */
  }


  /* documentation is in ftdebug.h */

  FT_BASE_DEF( ଶୂନ୍ୟ )
  FT_Trace_Enable( ଶୂନ୍ୟ )
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

  FT_BASE_DEF( ଶୂନ୍ୟ )
  ft_logging_init( ଶୂନ୍ୟ )
  {
    ft_default_log_handler = ft_log_handler;
    ft_default_trace_level = ft_getenv( "FT2_DEBUG" );

    ଯଦି ( ft_getenv( "FT_LOGGING_FILE" ) )
      ft_fileptr = ft_fopen( ft_getenv( "FT_LOGGING_FILE" ), "w" );
    ଅନ୍ୟଥା
      ft_fileptr = stderr;

    ft_debug_init();

    /* Set the default output handler for 'dlg'. */
    dlg_set_handler( ft_default_log_handler, ଖାଲି );
  }


  FT_BASE_DEF( ଶୂନ୍ୟ )
  ft_logging_deinit( ଶୂନ୍ୟ )
  {
    ଯଦି ( ft_fileptr != stderr )
      ft_fclose( ft_fileptr );
  }


  /**************************************************************************
   *
   * An output log handler for FreeType.
   *
   */
  FT_BASE_DEF( ଶୂନ୍ୟ )
  ft_log_handler( ଅଚଳ ସଂରଚନା dlg_origin*  origin,
                  ଅଚଳ ଅକ୍ଷର*               string,
                  ଶୂନ୍ୟ*                     data )
  {
    ଅକ୍ଷର         features_buf[128];
    ଅକ୍ଷର*        bufp = features_buf;

    FT_UNUSED( data );


    ଯଦି ( ft_have_newline_char )
    {
      ଅଚଳ ଅକ୍ଷର*  features        = ଖାଲି;
      size_t       features_length = 0;


#define FEATURES_TIMESTAMP            "[%h:%m] "
#define FEATURES_COMPONENT            "[%t] "
#define FEATURES_TIMESTAMP_COMPONENT  "[%h:%m %t] "

      ଯଦି ( ft_timestamp_flag && ft_component_flag )
      {
        features        = FEATURES_TIMESTAMP_COMPONENT;
        features_length = ଆକାର ( FEATURES_TIMESTAMP_COMPONENT );
      }
      ଅନ୍ୟଥା ଯଦି ( ft_timestamp_flag )
      {
        features        = FEATURES_TIMESTAMP;
        features_length = ଆକାର ( FEATURES_TIMESTAMP );
      }
      ଅନ୍ୟଥା ଯଦି ( ft_component_flag )
      {
        features        = FEATURES_COMPONENT;
        features_length = ଆକାର ( FEATURES_COMPONENT );
      }

      ଯଦି ( ft_component_flag || ft_timestamp_flag )
      {
        ft_strncpy( features_buf, features, features_length );
        bufp += features_length - 1;
      }

      ଯଦି ( ft_component_flag )
      {
        size_t  tag_length = ft_strlen( *origin->tags );
        size_t  i;


        /* To vertically align tracing messages we compensate the */
        /* different FT_COMPONENT string lengths by inserting an  */
        /* appropriate amount of space characters.                */
        ପାଇଁ ( i = 0;
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
                                (ଅଚଳ ଅକ୍ଷର*)features_buf,
                                origin,
                                string,
                                dlg_default_output_styles,
                                true );

    ଯଦି ( ft_strrchr( string, '\n' ) )
      ft_have_newline_char = TRUE;
    ଅନ୍ୟଥା
      ft_have_newline_char = FALSE;
  }


  /* documentation is in ftdebug.h */
  FT_BASE_DEF( ଶୂନ୍ୟ )
  ft_add_tag( ଅଚଳ ଅକ୍ଷର*  tag )
  {
    ft_component = tag;

    dlg_add_tag( tag, ଖାଲି );
  }


  /* documentation is in ftdebug.h */
  FT_BASE_DEF( ଶୂନ୍ୟ )
  ft_remove_tag( ଅଚଳ ଅକ୍ଷର*  tag )
  {
    dlg_remove_tag( tag, ଖାଲି );
  }


  /* documentation is in ftlogging.h */

  FT_EXPORT_DEF( ଶୂନ୍ୟ )
  FT_Trace_Set_Level( ଅଚଳ ଅକ୍ଷର*  level )
  {
    ft_component_flag     = FALSE;
    ft_timestamp_flag     = FALSE;
    ft_custom_trace_level = level;

    ft_debug_init();
  }


  /* documentation is in ftlogging.h */

  FT_EXPORT_DEF( ଶୂନ୍ୟ )
  FT_Trace_Set_Default_Level( ଶୂନ୍ୟ )
  {
    ft_component_flag     = FALSE;
    ft_timestamp_flag     = FALSE;
    ft_custom_trace_level = ଖାଲି;

    ft_debug_init();
  }


  /**************************************************************************
   *
   * Functions to handle a custom log handler.
   *
   */

  /* documentation is in ftlogging.h */

  FT_EXPORT_DEF( ଶୂନ୍ୟ )
  FT_Set_Log_Handler( FT_Custom_Log_Handler  handler )
  {
    custom_output_handler = handler;
  }


  /* documentation is in ftlogging.h */

  FT_EXPORT_DEF( ଶୂନ୍ୟ )
  FT_Set_Default_Log_Handler( ଶୂନ୍ୟ )
  {
    custom_output_handler = ଖାଲି;
  }


  /* documentation is in ftdebug.h */
  FT_BASE_DEF( ଶୂନ୍ୟ )
  FT_Logging_Callback( ଅଚଳ ଅକ୍ଷର*  fmt,
                       ... )
  {
    va_list  ap;


    va_start( ap, fmt );
    custom_output_handler( ft_component, fmt, ap );
    va_end( ap );
  }

#ଅନ୍ୟଥା /* !FT_DEBUG_LOGGING */

  FT_EXPORT_DEF( ଶୂନ୍ୟ )
  FT_Trace_Set_Level( ଅଚଳ ଅକ୍ଷର*  level )
  {
    FT_UNUSED( level );
  }


  FT_EXPORT_DEF( ଶୂନ୍ୟ )
  FT_Trace_Set_Default_Level( ଶୂନ୍ୟ )
  {
    /* nothing */
  }


  FT_EXPORT_DEF( ଶୂନ୍ୟ )
  FT_Set_Log_Handler( FT_Custom_Log_Handler  handler )
  {
    FT_UNUSED( handler );
  }


  FT_EXPORT_DEF( ଶୂନ୍ୟ )
  FT_Set_Default_Log_Handler( ଶୂନ୍ୟ )
  {
    /* nothing */
  }

#endif /* !FT_DEBUG_LOGGING */


/* END */
