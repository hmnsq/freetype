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


#அந்தர்பாவ <freetype/freetype.h>
#அந்தர்பாவ <freetype/ftlogging.h>
#அந்தர்பாவ <freetype/internal/ftdebug.h>
#அந்தர்பாவ <freetype/internal/ftobjs.h>


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
  நிலையான மாறிலி எழுத்து*  ft_default_trace_level = வெற்று;
  நிலையான FILE*        ft_fileptr             = வெற்று;
  நிலையான மாறிலி எழுத்து*  ft_component           = வெற்று;
  நிலையான FT_Bool      ft_component_flag      = FALSE;
  நிலையான FT_Bool      ft_timestamp_flag      = FALSE;
  நிலையான FT_Bool      ft_have_newline_char   = TRUE;
  நிலையான மாறிலி எழுத்து*  ft_custom_trace_level  = வெற்று;

  /* declared in ftdebug.h */

  dlg_handler            ft_default_log_handler = வெற்று;
  FT_Custom_Log_Handler  custom_output_handler  = வெற்று;

#endif /* FT_DEBUG_LOGGING */


#ifdef FT_DEBUG_LEVEL_ERROR

  /* documentation is in ftdebug.h */

  FT_BASE_DEF( வெறுமை )
  FT_Message( மாறிலி எழுத்து*  fmt,
              ... )
  {
    va_list  ap;


    va_start( ap, fmt );
    vfprintf( stderr, fmt, ap );
    va_end( ap );
  }


  /* documentation is in ftdebug.h */

  FT_BASE_DEF( வெறுமை )
  FT_Panic( மாறிலி எழுத்து*  fmt,
            ... )
  {
    va_list  ap;


    va_start( ap, fmt );
    vfprintf( stderr, fmt, ap );
    va_end( ap );

    exit( EXIT_FAILURE );
  }


  /* documentation is in ftdebug.h */

  FT_BASE_DEF( முழு )
  FT_Throw( FT_Error     error,
            முழு          வரி,
            மாறிலி எழுத்து*  file )
  {
#எனில் 0
    /* activating the code in this block makes FreeType very chatty */
    fprintf( stderr,
             "%s:%d: error 0x%02x: %s\n",
             file,
             வரி,
             error,
             FT_Error_String( error ) );
#இல்லையெனில்
    FT_UNUSED( error );
    FT_UNUSED( வரி );
    FT_UNUSED( file );
#endif

    திரும்பு 0;
  }

#endif /* FT_DEBUG_LEVEL_ERROR */


#ifdef FT_DEBUG_LEVEL_TRACE

  /* array of trace levels, initialized to 0; */
  /* this gets adjusted at run-time           */
  நிலையான முழு  ft_trace_levels_enabled[trace_மொத்தம்];

  /* array of trace levels, always initialized to 0 */
  நிலையான முழு  ft_trace_levels_disabled[trace_மொத்தம்];

  /* a pointer to either `ft_trace_levels_enabled' */
  /* or `ft_trace_levels_disabled'                 */
  முழு*  ft_trace_levels;

  /* define array of trace toggle names */
#define FT_TRACE_DEF( x )  #x ,

  நிலையான மாறிலி எழுத்து*  ft_trace_toggles[trace_மொத்தம் + 1] =
  {
#அந்தர்பாவ <freetype/internal/fttrace.h>
    வெற்று
  };

#undef FT_TRACE_DEF


  /* documentation is in ftdebug.h */

  FT_BASE_DEF( FT_Int )
  FT_Trace_Get_மொத்தம்( வெறுமை )
  {
    திரும்பு trace_மொத்தம்;
  }


  /* documentation is in ftdebug.h */

  FT_BASE_DEF( மாறிலி எழுத்து * )
  FT_Trace_Get_Name( FT_Int  idx )
  {
    முழு  max = FT_Trace_Get_மொத்தம்();


    எனில் ( idx < max )
      திரும்பு ft_trace_toggles[idx];
    இல்லையெனில்
      திரும்பு வெற்று;
  }


  /* documentation is in ftdebug.h */

  FT_BASE_DEF( வெறுமை )
  FT_Trace_Disable( வெறுமை )
  {
    ft_trace_levels = ft_trace_levels_disabled;
  }


  /* documentation is in ftdebug.h */

  FT_BASE_DEF( வெறுமை )
  FT_Trace_Enable( வெறுமை )
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
  FT_BASE_DEF( வெறுமை )
  ft_debug_init( வெறுமை )
  {
    மாறிலி எழுத்து*  ft2_debug = வெற்று;


#ifdef FT_DEBUG_LOGGING
    எனில் ( ft_custom_trace_level != வெற்று )
      ft2_debug = ft_custom_trace_level;
    இல்லையெனில்
      ft2_debug = ft_default_trace_level;
#இல்லையெனில்
    ft2_debug = ft_getenv( "FT2_DEBUG" );
#endif

    எனில் ( ft2_debug )
    {
      மாறிலி எழுத்து*  p = ft2_debug;
      மாறிலி எழுத்து*  q;


      க்கு ( ; *p; p++ )
      {
        /* skip leading whitespace and separators */
        எனில் ( *p == ' ' || *p == '\t' || *p == ',' || *p == ';' || *p == '=' )
          தொடர்;

#ifdef FT_DEBUG_LOGGING

        /* check extra arguments for logging */
        எனில் ( *p == '-' )
        {
          மாறிலி எழுத்து*  r = ++p;


          எனில் ( *r == 'v' )
          {
            மாறிலி எழுத்து*  s = ++r;


            ft_component_flag = TRUE;

            எனில் ( *s == 't' )
            {
              ft_timestamp_flag = TRUE;
              p++;
            }

            p++;
          }

          இல்லையெனில் எனில் ( *r == 't' )
          {
            மாறிலி எழுத்து*  s = ++r;


            ft_timestamp_flag = TRUE;

            எனில் ( *s == 'v' )
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
        போது ( *p && *p != ':' )
          p++;

        எனில் ( !*p )
          நிறுத்து;

        எனில் ( *p == ':' && p > q )
        {
          FT_Int  n, i, len = (FT_Int)( p - q );
          FT_Int  level = -1, found = -1;


          க்கு ( n = 0; n < trace_மொத்தம்; n++ )
          {
            மாறிலி எழுத்து*  toggle = ft_trace_toggles[n];


            க்கு ( i = 0; i < len; i++ )
            {
              எனில் ( toggle[i] != q[i] )
                நிறுத்து;
            }

            எனில் ( i == len && toggle[i] == 0 )
            {
              found = n;
              நிறுத்து;
            }
          }

          /* read level */
          p++;
          எனில் ( *p )
          {
            level = *p - '0';
            எனில் ( level < 0 || level > 7 )
              level = -1;
          }

          எனில் ( found >= 0 && level >= 0 )
          {
            எனில் ( found == trace_any )
            {
              /* special case for `any' */
              க்கு ( n = 0; n < trace_மொத்தம்; n++ )
                ft_trace_levels_enabled[n] = level;
            }
            இல்லையெனில்
              ft_trace_levels_enabled[found] = level;
          }
        }
      }
    }

    ft_trace_levels = ft_trace_levels_enabled;
  }


#இல்லையெனில்  /* !FT_DEBUG_LEVEL_TRACE */


  FT_BASE_DEF( வெறுமை )
  ft_debug_init( வெறுமை )
  {
    /* nothing */
  }


  FT_BASE_DEF( FT_Int )
  FT_Trace_Get_மொத்தம்( வெறுமை )
  {
    திரும்பு 0;
  }


  FT_BASE_DEF( மாறிலி எழுத்து * )
  FT_Trace_Get_Name( FT_Int  idx )
  {
    FT_UNUSED( idx );

    திரும்பு வெற்று;
  }


  FT_BASE_DEF( வெறுமை )
  FT_Trace_Disable( வெறுமை )
  {
    /* nothing */
  }


  /* documentation is in ftdebug.h */

  FT_BASE_DEF( வெறுமை )
  FT_Trace_Enable( வெறுமை )
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

  FT_BASE_DEF( வெறுமை )
  ft_logging_init( வெறுமை )
  {
    ft_default_log_handler = ft_log_handler;
    ft_default_trace_level = ft_getenv( "FT2_DEBUG" );

    எனில் ( ft_getenv( "FT_LOGGING_FILE" ) )
      ft_fileptr = ft_fopen( ft_getenv( "FT_LOGGING_FILE" ), "w" );
    இல்லையெனில்
      ft_fileptr = stderr;

    ft_debug_init();

    /* Set the default output handler for 'dlg'. */
    dlg_set_handler( ft_default_log_handler, வெற்று );
  }


  FT_BASE_DEF( வெறுமை )
  ft_logging_deinit( வெறுமை )
  {
    எனில் ( ft_fileptr != stderr )
      ft_fclose( ft_fileptr );
  }


  /**************************************************************************
   *
   * An output log handler for FreeType.
   *
   */
  FT_BASE_DEF( வெறுமை )
  ft_log_handler( மாறிலி கட்டமைப்பு dlg_origin*  origin,
                  மாறிலி எழுத்து*               string,
                  வெறுமை*                     data )
  {
    எழுத்து         features_buf[128];
    எழுத்து*        bufp = features_buf;

    FT_UNUSED( data );


    எனில் ( ft_have_newline_char )
    {
      மாறிலி எழுத்து*  features        = வெற்று;
      size_t       features_length = 0;


#define FEATURES_TIMESTAMP            "[%h:%m] "
#define FEATURES_COMPONENT            "[%t] "
#define FEATURES_TIMESTAMP_COMPONENT  "[%h:%m %t] "

      எனில் ( ft_timestamp_flag && ft_component_flag )
      {
        features        = FEATURES_TIMESTAMP_COMPONENT;
        features_length = அளவு ( FEATURES_TIMESTAMP_COMPONENT );
      }
      இல்லையெனில் எனில் ( ft_timestamp_flag )
      {
        features        = FEATURES_TIMESTAMP;
        features_length = அளவு ( FEATURES_TIMESTAMP );
      }
      இல்லையெனில் எனில் ( ft_component_flag )
      {
        features        = FEATURES_COMPONENT;
        features_length = அளவு ( FEATURES_COMPONENT );
      }

      எனில் ( ft_component_flag || ft_timestamp_flag )
      {
        ft_strncpy( features_buf, features, features_length );
        bufp += features_length - 1;
      }

      எனில் ( ft_component_flag )
      {
        size_t  tag_length = ft_strlen( *origin->tags );
        size_t  i;


        /* To vertically align tracing messages we compensate the */
        /* different FT_COMPONENT string lengths by inserting an  */
        /* appropriate amount of space characters.                */
        க்கு ( i = 0;
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
                                (மாறிலி எழுத்து*)features_buf,
                                origin,
                                string,
                                dlg_default_output_styles,
                                true );

    எனில் ( ft_strrchr( string, '\n' ) )
      ft_have_newline_char = TRUE;
    இல்லையெனில்
      ft_have_newline_char = FALSE;
  }


  /* documentation is in ftdebug.h */
  FT_BASE_DEF( வெறுமை )
  ft_add_tag( மாறிலி எழுத்து*  tag )
  {
    ft_component = tag;

    dlg_add_tag( tag, வெற்று );
  }


  /* documentation is in ftdebug.h */
  FT_BASE_DEF( வெறுமை )
  ft_remove_tag( மாறிலி எழுத்து*  tag )
  {
    dlg_remove_tag( tag, வெற்று );
  }


  /* documentation is in ftlogging.h */

  FT_EXPORT_DEF( வெறுமை )
  FT_Trace_Set_Level( மாறிலி எழுத்து*  level )
  {
    ft_component_flag     = FALSE;
    ft_timestamp_flag     = FALSE;
    ft_custom_trace_level = level;

    ft_debug_init();
  }


  /* documentation is in ftlogging.h */

  FT_EXPORT_DEF( வெறுமை )
  FT_Trace_Set_Default_Level( வெறுமை )
  {
    ft_component_flag     = FALSE;
    ft_timestamp_flag     = FALSE;
    ft_custom_trace_level = வெற்று;

    ft_debug_init();
  }


  /**************************************************************************
   *
   * Functions to handle a custom log handler.
   *
   */

  /* documentation is in ftlogging.h */

  FT_EXPORT_DEF( வெறுமை )
  FT_Set_Log_Handler( FT_Custom_Log_Handler  handler )
  {
    custom_output_handler = handler;
  }


  /* documentation is in ftlogging.h */

  FT_EXPORT_DEF( வெறுமை )
  FT_Set_Default_Log_Handler( வெறுமை )
  {
    custom_output_handler = வெற்று;
  }


  /* documentation is in ftdebug.h */
  FT_BASE_DEF( வெறுமை )
  FT_Logging_Callback( மாறிலி எழுத்து*  fmt,
                       ... )
  {
    va_list  ap;


    va_start( ap, fmt );
    custom_output_handler( ft_component, fmt, ap );
    va_end( ap );
  }

#இல்லையெனில் /* !FT_DEBUG_LOGGING */

  FT_EXPORT_DEF( வெறுமை )
  FT_Trace_Set_Level( மாறிலி எழுத்து*  level )
  {
    FT_UNUSED( level );
  }


  FT_EXPORT_DEF( வெறுமை )
  FT_Trace_Set_Default_Level( வெறுமை )
  {
    /* nothing */
  }


  FT_EXPORT_DEF( வெறுமை )
  FT_Set_Log_Handler( FT_Custom_Log_Handler  handler )
  {
    FT_UNUSED( handler );
  }


  FT_EXPORT_DEF( வெறுமை )
  FT_Set_Default_Log_Handler( வெறுமை )
  {
    /* nothing */
  }

#endif /* !FT_DEBUG_LOGGING */


/* END */
