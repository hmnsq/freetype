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


#شامل <freetype/freetype.h>
#شامل <freetype/ftlogging.h>
#شامل <freetype/internal/ftdebug.h>
#شامل <freetype/internal/ftobjs.h>


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
  ایستا ثابت نویسه*  ft_default_trace_level = پوچ;
  ایستا FILE*        ft_fileptr             = پوچ;
  ایستا ثابت نویسه*  ft_component           = پوچ;
  ایستا FT_Bool      ft_component_flag      = FALSE;
  ایستا FT_Bool      ft_timestamp_flag      = FALSE;
  ایستا FT_Bool      ft_have_newline_char   = TRUE;
  ایستا ثابت نویسه*  ft_custom_trace_level  = پوچ;

  /* declared in ftdebug.h */

  dlg_handler            ft_default_log_handler = پوچ;
  FT_Custom_Log_Handler  custom_output_handler  = پوچ;

#endif /* FT_DEBUG_LOGGING */


#ifdef FT_DEBUG_LEVEL_ERROR

  /* documentation is in ftdebug.h */

  FT_BASE_DEF( تهی )
  FT_Message( ثابت نویسه*  fmt,
              ... )
  {
    va_list  ap;


    va_start( ap, fmt );
    vfprintf( stderr, fmt, ap );
    va_end( ap );
  }


  /* documentation is in ftdebug.h */

  FT_BASE_DEF( تهی )
  FT_Panic( ثابت نویسه*  fmt,
            ... )
  {
    va_list  ap;


    va_start( ap, fmt );
    vfprintf( stderr, fmt, ap );
    va_end( ap );

    exit( EXIT_FAILURE );
  }


  /* documentation is in ftdebug.h */

  FT_BASE_DEF( صحیح )
  FT_Throw( FT_Error     error,
            صحیح          سطر,
            ثابت نویسه*  file )
  {
#اگر 0
    /* activating the code in this block makes FreeType very chatty */
    fprintf( stderr,
             "%s:%d: error 0x%02x: %s\n",
             file,
             سطر,
             error,
             FT_Error_String( error ) );
#وگرنه
    FT_UNUSED( error );
    FT_UNUSED( سطر );
    FT_UNUSED( file );
#endif

    بازگشت 0;
  }

#endif /* FT_DEBUG_LEVEL_ERROR */


#ifdef FT_DEBUG_LEVEL_TRACE

  /* array of trace levels, initialized to 0; */
  /* this gets adjusted at run-time           */
  ایستا صحیح  ft_trace_levels_enabled[trace_شمار];

  /* array of trace levels, always initialized to 0 */
  ایستا صحیح  ft_trace_levels_disabled[trace_شمار];

  /* a pointer to either `ft_trace_levels_enabled' */
  /* or `ft_trace_levels_disabled'                 */
  صحیح*  ft_trace_levels;

  /* define array of trace toggle names */
#define FT_TRACE_DEF( x )  #x ,

  ایستا ثابت نویسه*  ft_trace_toggles[trace_شمار + 1] =
  {
#شامل <freetype/internal/fttrace.h>
    پوچ
  };

#undef FT_TRACE_DEF


  /* documentation is in ftdebug.h */

  FT_BASE_DEF( FT_Int )
  FT_Trace_Get_شمار( تهی )
  {
    بازگشت trace_شمار;
  }


  /* documentation is in ftdebug.h */

  FT_BASE_DEF( ثابت نویسه * )
  FT_Trace_Get_Name( FT_Int  idx )
  {
    صحیح  max = FT_Trace_Get_شمار();


    اگر ( idx < max )
      بازگشت ft_trace_toggles[idx];
    وگرنه
      بازگشت پوچ;
  }


  /* documentation is in ftdebug.h */

  FT_BASE_DEF( تهی )
  FT_Trace_Disable( تهی )
  {
    ft_trace_levels = ft_trace_levels_disabled;
  }


  /* documentation is in ftdebug.h */

  FT_BASE_DEF( تهی )
  FT_Trace_Enable( تهی )
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
  FT_BASE_DEF( تهی )
  ft_debug_init( تهی )
  {
    ثابت نویسه*  ft2_debug = پوچ;


#ifdef FT_DEBUG_LOGGING
    اگر ( ft_custom_trace_level != پوچ )
      ft2_debug = ft_custom_trace_level;
    وگرنه
      ft2_debug = ft_default_trace_level;
#وگرنه
    ft2_debug = ft_getenv( "FT2_DEBUG" );
#endif

    اگر ( ft2_debug )
    {
      ثابت نویسه*  p = ft2_debug;
      ثابت نویسه*  q;


      برای ( ; *p; p++ )
      {
        /* skip leading whitespace and separators */
        اگر ( *p == ' ' || *p == '\t' || *p == ',' || *p == ';' || *p == '=' )
          ادامه;

#ifdef FT_DEBUG_LOGGING

        /* check extra arguments for logging */
        اگر ( *p == '-' )
        {
          ثابت نویسه*  r = ++p;


          اگر ( *r == 'v' )
          {
            ثابت نویسه*  s = ++r;


            ft_component_flag = TRUE;

            اگر ( *s == 't' )
            {
              ft_timestamp_flag = TRUE;
              p++;
            }

            p++;
          }

          وگرنه اگر ( *r == 't' )
          {
            ثابت نویسه*  s = ++r;


            ft_timestamp_flag = TRUE;

            اگر ( *s == 'v' )
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
        وقتی ( *p && *p != ':' )
          p++;

        اگر ( !*p )
          بشکن;

        اگر ( *p == ':' && p > q )
        {
          FT_Int  n, i, len = (FT_Int)( p - q );
          FT_Int  level = -1, found = -1;


          برای ( n = 0; n < trace_شمار; n++ )
          {
            ثابت نویسه*  toggle = ft_trace_toggles[n];


            برای ( i = 0; i < len; i++ )
            {
              اگر ( toggle[i] != q[i] )
                بشکن;
            }

            اگر ( i == len && toggle[i] == 0 )
            {
              found = n;
              بشکن;
            }
          }

          /* read level */
          p++;
          اگر ( *p )
          {
            level = *p - '0';
            اگر ( level < 0 || level > 7 )
              level = -1;
          }

          اگر ( found >= 0 && level >= 0 )
          {
            اگر ( found == trace_any )
            {
              /* special case for `any' */
              برای ( n = 0; n < trace_شمار; n++ )
                ft_trace_levels_enabled[n] = level;
            }
            وگرنه
              ft_trace_levels_enabled[found] = level;
          }
        }
      }
    }

    ft_trace_levels = ft_trace_levels_enabled;
  }


#وگرنه  /* !FT_DEBUG_LEVEL_TRACE */


  FT_BASE_DEF( تهی )
  ft_debug_init( تهی )
  {
    /* nothing */
  }


  FT_BASE_DEF( FT_Int )
  FT_Trace_Get_شمار( تهی )
  {
    بازگشت 0;
  }


  FT_BASE_DEF( ثابت نویسه * )
  FT_Trace_Get_Name( FT_Int  idx )
  {
    FT_UNUSED( idx );

    بازگشت پوچ;
  }


  FT_BASE_DEF( تهی )
  FT_Trace_Disable( تهی )
  {
    /* nothing */
  }


  /* documentation is in ftdebug.h */

  FT_BASE_DEF( تهی )
  FT_Trace_Enable( تهی )
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

  FT_BASE_DEF( تهی )
  ft_logging_init( تهی )
  {
    ft_default_log_handler = ft_log_handler;
    ft_default_trace_level = ft_getenv( "FT2_DEBUG" );

    اگر ( ft_getenv( "FT_LOGGING_FILE" ) )
      ft_fileptr = ft_fopen( ft_getenv( "FT_LOGGING_FILE" ), "w" );
    وگرنه
      ft_fileptr = stderr;

    ft_debug_init();

    /* Set the default output handler for 'dlg'. */
    dlg_set_handler( ft_default_log_handler, پوچ );
  }


  FT_BASE_DEF( تهی )
  ft_logging_deinit( تهی )
  {
    اگر ( ft_fileptr != stderr )
      ft_fclose( ft_fileptr );
  }


  /**************************************************************************
   *
   * An output log handler for FreeType.
   *
   */
  FT_BASE_DEF( تهی )
  ft_log_handler( ثابت ساختار dlg_origin*  origin,
                  ثابت نویسه*               string,
                  تهی*                     data )
  {
    نویسه         features_buf[128];
    نویسه*        bufp = features_buf;

    FT_UNUSED( data );


    اگر ( ft_have_newline_char )
    {
      ثابت نویسه*  features        = پوچ;
      size_t       features_length = 0;


#define FEATURES_TIMESTAMP            "[%h:%m] "
#define FEATURES_COMPONENT            "[%t] "
#define FEATURES_TIMESTAMP_COMPONENT  "[%h:%m %t] "

      اگر ( ft_timestamp_flag && ft_component_flag )
      {
        features        = FEATURES_TIMESTAMP_COMPONENT;
        features_length = اندازه ( FEATURES_TIMESTAMP_COMPONENT );
      }
      وگرنه اگر ( ft_timestamp_flag )
      {
        features        = FEATURES_TIMESTAMP;
        features_length = اندازه ( FEATURES_TIMESTAMP );
      }
      وگرنه اگر ( ft_component_flag )
      {
        features        = FEATURES_COMPONENT;
        features_length = اندازه ( FEATURES_COMPONENT );
      }

      اگر ( ft_component_flag || ft_timestamp_flag )
      {
        ft_strncpy( features_buf, features, features_length );
        bufp += features_length - 1;
      }

      اگر ( ft_component_flag )
      {
        size_t  tag_length = ft_strlen( *origin->tags );
        size_t  i;


        /* To vertically align tracing messages we compensate the */
        /* different FT_COMPONENT string lengths by inserting an  */
        /* appropriate amount of space characters.                */
        برای ( i = 0;
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
                                (ثابت نویسه*)features_buf,
                                origin,
                                string,
                                dlg_default_output_styles,
                                true );

    اگر ( ft_strrchr( string, '\n' ) )
      ft_have_newline_char = TRUE;
    وگرنه
      ft_have_newline_char = FALSE;
  }


  /* documentation is in ftdebug.h */
  FT_BASE_DEF( تهی )
  ft_add_tag( ثابت نویسه*  tag )
  {
    ft_component = tag;

    dlg_add_tag( tag, پوچ );
  }


  /* documentation is in ftdebug.h */
  FT_BASE_DEF( تهی )
  ft_remove_tag( ثابت نویسه*  tag )
  {
    dlg_remove_tag( tag, پوچ );
  }


  /* documentation is in ftlogging.h */

  FT_EXPORT_DEF( تهی )
  FT_Trace_Set_Level( ثابت نویسه*  level )
  {
    ft_component_flag     = FALSE;
    ft_timestamp_flag     = FALSE;
    ft_custom_trace_level = level;

    ft_debug_init();
  }


  /* documentation is in ftlogging.h */

  FT_EXPORT_DEF( تهی )
  FT_Trace_Set_Default_Level( تهی )
  {
    ft_component_flag     = FALSE;
    ft_timestamp_flag     = FALSE;
    ft_custom_trace_level = پوچ;

    ft_debug_init();
  }


  /**************************************************************************
   *
   * Functions to handle a custom log handler.
   *
   */

  /* documentation is in ftlogging.h */

  FT_EXPORT_DEF( تهی )
  FT_Set_Log_Handler( FT_Custom_Log_Handler  handler )
  {
    custom_output_handler = handler;
  }


  /* documentation is in ftlogging.h */

  FT_EXPORT_DEF( تهی )
  FT_Set_Default_Log_Handler( تهی )
  {
    custom_output_handler = پوچ;
  }


  /* documentation is in ftdebug.h */
  FT_BASE_DEF( تهی )
  FT_Logging_Callback( ثابت نویسه*  fmt,
                       ... )
  {
    va_list  ap;


    va_start( ap, fmt );
    custom_output_handler( ft_component, fmt, ap );
    va_end( ap );
  }

#وگرنه /* !FT_DEBUG_LOGGING */

  FT_EXPORT_DEF( تهی )
  FT_Trace_Set_Level( ثابت نویسه*  level )
  {
    FT_UNUSED( level );
  }


  FT_EXPORT_DEF( تهی )
  FT_Trace_Set_Default_Level( تهی )
  {
    /* nothing */
  }


  FT_EXPORT_DEF( تهی )
  FT_Set_Log_Handler( FT_Custom_Log_Handler  handler )
  {
    FT_UNUSED( handler );
  }


  FT_EXPORT_DEF( تهی )
  FT_Set_Default_Log_Handler( تهی )
  {
    /* nothing */
  }

#endif /* !FT_DEBUG_LOGGING */


/* END */
