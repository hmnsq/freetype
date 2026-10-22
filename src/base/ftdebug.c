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


#ضمن <freetype/freetype.h>
#ضمن <freetype/ftlogging.h>
#ضمن <freetype/internal/ftdebug.h>
#ضمن <freetype/internal/ftobjs.h>


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
  ساكن ثابت حرف*  ft_default_trace_level = عدم;
  ساكن FILE*        ft_fileptr             = عدم;
  ساكن ثابت حرف*  ft_component           = عدم;
  ساكن FT_Bool      ft_component_flag      = FALSE;
  ساكن FT_Bool      ft_timestamp_flag      = FALSE;
  ساكن FT_Bool      ft_have_newline_char   = TRUE;
  ساكن ثابت حرف*  ft_custom_trace_level  = عدم;

  /* declared in ftdebug.h */

  dlg_handler            ft_default_log_handler = عدم;
  FT_Custom_Log_Handler  custom_output_handler  = عدم;

#endif /* FT_DEBUG_LOGGING */


#ifdef FT_DEBUG_LEVEL_ERROR

  /* documentation is in ftdebug.h */

  FT_BASE_DEF( فراغ )
  FT_Message( ثابت حرف*  fmt,
              ... )
  {
    va_list  ap;


    va_start( ap, fmt );
    vfprintf( stderr, fmt, ap );
    va_end( ap );
  }


  /* documentation is in ftdebug.h */

  FT_BASE_DEF( فراغ )
  FT_Panic( ثابت حرف*  fmt,
            ... )
  {
    va_list  ap;


    va_start( ap, fmt );
    vfprintf( stderr, fmt, ap );
    va_end( ap );

    exit( EXIT_FAILURE );
  }


  /* documentation is in ftdebug.h */

  FT_BASE_DEF( صحيح )
  FT_Throw( FT_Error     error,
            صحيح          line,
            ثابت حرف*  file )
  {
#ܐܢ 0
    /* activating the code in this block makes FreeType very chatty */
    fprintf( stderr,
             "%s:%d: error 0x%02x: %s\n",
             file,
             line,
             error,
             FT_Error_String( error ) );
#ܐܚܪܢܐ
    FT_UNUSED( error );
    FT_UNUSED( line );
    FT_UNUSED( file );
#endif

    ܦܢܐ 0;
  }

#endif /* FT_DEBUG_LEVEL_ERROR */


#ifdef FT_DEBUG_LEVEL_TRACE

  /* array of trace levels, initialized to 0; */
  /* this gets adjusted at run-time           */
  ساكن صحيح  ft_trace_levels_enabled[trace_count];

  /* array of trace levels, always initialized to 0 */
  ساكن صحيح  ft_trace_levels_disabled[trace_count];

  /* a pointer to either `ft_trace_levels_enabled' */
  /* or `ft_trace_levels_disabled'                 */
  صحيح*  ft_trace_levels;

  /* define array of trace toggle names */
#define FT_TRACE_DEF( x )  #x ,

  ساكن ثابت حرف*  ft_trace_toggles[trace_count + 1] =
  {
#ضمن <freetype/internal/fttrace.h>
    عدم
  };

#undef FT_TRACE_DEF


  /* documentation is in ftdebug.h */

  FT_BASE_DEF( FT_Int )
  FT_Trace_Get_Count( فراغ )
  {
    ܦܢܐ trace_count;
  }


  /* documentation is in ftdebug.h */

  FT_BASE_DEF( ثابت حرف * )
  FT_Trace_Get_Name( FT_Int  idx )
  {
    صحيح  max = FT_Trace_Get_Count();


    ܐܢ ( idx < max )
      ܦܢܐ ft_trace_toggles[idx];
    ܐܚܪܢܐ
      ܦܢܐ عدم;
  }


  /* documentation is in ftdebug.h */

  FT_BASE_DEF( فراغ )
  FT_Trace_Disable( فراغ )
  {
    ft_trace_levels = ft_trace_levels_disabled;
  }


  /* documentation is in ftdebug.h */

  FT_BASE_DEF( فراغ )
  FT_Trace_Enable( فراغ )
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
  FT_BASE_DEF( فراغ )
  ft_debug_init( فراغ )
  {
    ثابت حرف*  ft2_debug = عدم;


#ifdef FT_DEBUG_LOGGING
    ܐܢ ( ft_custom_trace_level != عدم )
      ft2_debug = ft_custom_trace_level;
    ܐܚܪܢܐ
      ft2_debug = ft_default_trace_level;
#ܐܚܪܢܐ
    ft2_debug = ft_getenv( "FT2_DEBUG" );
#endif

    ܐܢ ( ft2_debug )
    {
      ثابت حرف*  p = ft2_debug;
      ثابت حرف*  q;


      لكل ( ; *p; p++ )
      {
        /* skip leading whitespace and separators */
        ܐܢ ( *p == ' ' || *p == '\t' || *p == ',' || *p == ';' || *p == '=' )
          استمر;

#ifdef FT_DEBUG_LOGGING

        /* check extra arguments for logging */
        ܐܢ ( *p == '-' )
        {
          ثابت حرف*  r = ++p;


          ܐܢ ( *r == 'v' )
          {
            ثابت حرف*  s = ++r;


            ft_component_flag = TRUE;

            ܐܢ ( *s == 't' )
            {
              ft_timestamp_flag = TRUE;
              p++;
            }

            p++;
          }

          ܐܚܪܢܐ ܐܢ ( *r == 't' )
          {
            ثابت حرف*  s = ++r;


            ft_timestamp_flag = TRUE;

            ܐܢ ( *s == 'v' )
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
        ܟܡܐ ( *p && *p != ':' )
          p++;

        ܐܢ ( !*p )
          اكسر;

        ܐܢ ( *p == ':' && p > q )
        {
          FT_Int  n, i, len = (FT_Int)( p - q );
          FT_Int  level = -1, found = -1;


          لكل ( n = 0; n < trace_count; n++ )
          {
            ثابت حرف*  toggle = ft_trace_toggles[n];


            لكل ( i = 0; i < len; i++ )
            {
              ܐܢ ( toggle[i] != q[i] )
                اكسر;
            }

            ܐܢ ( i == len && toggle[i] == 0 )
            {
              found = n;
              اكسر;
            }
          }

          /* read level */
          p++;
          ܐܢ ( *p )
          {
            level = *p - '0';
            ܐܢ ( level < 0 || level > 7 )
              level = -1;
          }

          ܐܢ ( found >= 0 && level >= 0 )
          {
            ܐܢ ( found == trace_any )
            {
              /* special case for `any' */
              لكل ( n = 0; n < trace_count; n++ )
                ft_trace_levels_enabled[n] = level;
            }
            ܐܚܪܢܐ
              ft_trace_levels_enabled[found] = level;
          }
        }
      }
    }

    ft_trace_levels = ft_trace_levels_enabled;
  }


#ܐܚܪܢܐ  /* !FT_DEBUG_LEVEL_TRACE */


  FT_BASE_DEF( فراغ )
  ft_debug_init( فراغ )
  {
    /* nothing */
  }


  FT_BASE_DEF( FT_Int )
  FT_Trace_Get_Count( فراغ )
  {
    ܦܢܐ 0;
  }


  FT_BASE_DEF( ثابت حرف * )
  FT_Trace_Get_Name( FT_Int  idx )
  {
    FT_UNUSED( idx );

    ܦܢܐ عدم;
  }


  FT_BASE_DEF( فراغ )
  FT_Trace_Disable( فراغ )
  {
    /* nothing */
  }


  /* documentation is in ftdebug.h */

  FT_BASE_DEF( فراغ )
  FT_Trace_Enable( فراغ )
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

  FT_BASE_DEF( فراغ )
  ft_logging_init( فراغ )
  {
    ft_default_log_handler = ft_log_handler;
    ft_default_trace_level = ft_getenv( "FT2_DEBUG" );

    ܐܢ ( ft_getenv( "FT_LOGGING_FILE" ) )
      ft_fileptr = ft_fopen( ft_getenv( "FT_LOGGING_FILE" ), "w" );
    ܐܚܪܢܐ
      ft_fileptr = stderr;

    ft_debug_init();

    /* Set the default output handler for 'dlg'. */
    dlg_set_handler( ft_default_log_handler, عدم );
  }


  FT_BASE_DEF( فراغ )
  ft_logging_deinit( فراغ )
  {
    ܐܢ ( ft_fileptr != stderr )
      ft_fclose( ft_fileptr );
  }


  /**************************************************************************
   *
   * An output log handler for FreeType.
   *
   */
  FT_BASE_DEF( فراغ )
  ft_log_handler( ثابت بنية dlg_origin*  origin,
                  ثابت حرف*               string,
                  فراغ*                     data )
  {
    حرف         features_buf[128];
    حرف*        bufp = features_buf;

    FT_UNUSED( data );


    ܐܢ ( ft_have_newline_char )
    {
      ثابت حرف*  features        = عدم;
      size_t       features_length = 0;


#define FEATURES_TIMESTAMP            "[%h:%m] "
#define FEATURES_COMPONENT            "[%t] "
#define FEATURES_TIMESTAMP_COMPONENT  "[%h:%m %t] "

      ܐܢ ( ft_timestamp_flag && ft_component_flag )
      {
        features        = FEATURES_TIMESTAMP_COMPONENT;
        features_length = حجم ( FEATURES_TIMESTAMP_COMPONENT );
      }
      ܐܚܪܢܐ ܐܢ ( ft_timestamp_flag )
      {
        features        = FEATURES_TIMESTAMP;
        features_length = حجم ( FEATURES_TIMESTAMP );
      }
      ܐܚܪܢܐ ܐܢ ( ft_component_flag )
      {
        features        = FEATURES_COMPONENT;
        features_length = حجم ( FEATURES_COMPONENT );
      }

      ܐܢ ( ft_component_flag || ft_timestamp_flag )
      {
        ft_strncpy( features_buf, features, features_length );
        bufp += features_length - 1;
      }

      ܐܢ ( ft_component_flag )
      {
        size_t  tag_length = ft_strlen( *origin->tags );
        size_t  i;


        /* To vertically align tracing messages we compensate the */
        /* different FT_COMPONENT string lengths by inserting an  */
        /* appropriate amount of space characters.                */
        لكل ( i = 0;
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
                                (ثابت حرف*)features_buf,
                                origin,
                                string,
                                dlg_default_output_styles,
                                true );

    ܐܢ ( ft_strrchr( string, '\n' ) )
      ft_have_newline_char = TRUE;
    ܐܚܪܢܐ
      ft_have_newline_char = FALSE;
  }


  /* documentation is in ftdebug.h */
  FT_BASE_DEF( فراغ )
  ft_add_tag( ثابت حرف*  tag )
  {
    ft_component = tag;

    dlg_add_tag( tag, عدم );
  }


  /* documentation is in ftdebug.h */
  FT_BASE_DEF( فراغ )
  ft_remove_tag( ثابت حرف*  tag )
  {
    dlg_remove_tag( tag, عدم );
  }


  /* documentation is in ftlogging.h */

  FT_EXPORT_DEF( فراغ )
  FT_Trace_Set_Level( ثابت حرف*  level )
  {
    ft_component_flag     = FALSE;
    ft_timestamp_flag     = FALSE;
    ft_custom_trace_level = level;

    ft_debug_init();
  }


  /* documentation is in ftlogging.h */

  FT_EXPORT_DEF( فراغ )
  FT_Trace_Set_Default_Level( فراغ )
  {
    ft_component_flag     = FALSE;
    ft_timestamp_flag     = FALSE;
    ft_custom_trace_level = عدم;

    ft_debug_init();
  }


  /**************************************************************************
   *
   * Functions to handle a custom log handler.
   *
   */

  /* documentation is in ftlogging.h */

  FT_EXPORT_DEF( فراغ )
  FT_Set_Log_Handler( FT_Custom_Log_Handler  handler )
  {
    custom_output_handler = handler;
  }


  /* documentation is in ftlogging.h */

  FT_EXPORT_DEF( فراغ )
  FT_Set_Default_Log_Handler( فراغ )
  {
    custom_output_handler = عدم;
  }


  /* documentation is in ftdebug.h */
  FT_BASE_DEF( فراغ )
  FT_Logging_Callback( ثابت حرف*  fmt,
                       ... )
  {
    va_list  ap;


    va_start( ap, fmt );
    custom_output_handler( ft_component, fmt, ap );
    va_end( ap );
  }

#ܐܚܪܢܐ /* !FT_DEBUG_LOGGING */

  FT_EXPORT_DEF( فراغ )
  FT_Trace_Set_Level( ثابت حرف*  level )
  {
    FT_UNUSED( level );
  }


  FT_EXPORT_DEF( فراغ )
  FT_Trace_Set_Default_Level( فراغ )
  {
    /* nothing */
  }


  FT_EXPORT_DEF( فراغ )
  FT_Set_Log_Handler( FT_Custom_Log_Handler  handler )
  {
    FT_UNUSED( handler );
  }


  FT_EXPORT_DEF( فراغ )
  FT_Set_Default_Log_Handler( فراغ )
  {
    /* nothing */
  }

#endif /* !FT_DEBUG_LOGGING */


/* END */
