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


#অন্তর্ভাব <freetype/freetype.h>
#অন্তর্ভাব <freetype/ftlogging.h>
#অন্তর্ভাব <freetype/internal/ftdebug.h>
#অন্তর্ভাব <freetype/internal/ftobjs.h>


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
  স্থির ধ্রুবক অক্ষর*  ft_default_trace_level = ফাঁকা;
  স্থির FILE*        ft_fileptr             = ফাঁকা;
  স্থির ধ্রুবক অক্ষর*  ft_component           = ফাঁকা;
  স্থির FT_Bool      ft_component_flag      = FALSE;
  স্থির FT_Bool      ft_timestamp_flag      = FALSE;
  স্থির FT_Bool      ft_have_newline_char   = TRUE;
  স্থির ধ্রুবক অক্ষর*  ft_custom_trace_level  = ফাঁকা;

  /* declared in ftdebug.h */

  dlg_handler            ft_default_log_handler = ফাঁকা;
  FT_Custom_Log_Handler  custom_output_handler  = ফাঁকা;

#endif /* FT_DEBUG_LOGGING */


#ifdef FT_DEBUG_LEVEL_ERROR

  /* documentation is in ftdebug.h */

  FT_BASE_DEF( শূন্য )
  FT_Message( ধ্রুবক অক্ষর*  fmt,
              ... )
  {
    va_list  ap;


    va_start( ap, fmt );
    vfprintf( stderr, fmt, ap );
    va_end( ap );
  }


  /* documentation is in ftdebug.h */

  FT_BASE_DEF( শূন্য )
  FT_Panic( ধ্রুবক অক্ষর*  fmt,
            ... )
  {
    va_list  ap;


    va_start( ap, fmt );
    vfprintf( stderr, fmt, ap );
    va_end( ap );

    exit( EXIT_FAILURE );
  }


  /* documentation is in ftdebug.h */

  FT_BASE_DEF( পূর্ণসংখ্যা )
  FT_Throw( FT_Error     error,
            পূর্ণসংখ্যা          সারি,
            ধ্রুবক অক্ষর*  file )
  {
#যদি 0
    /* activating the code in this block makes FreeType very chatty */
    fprintf( stderr,
             "%s:%d: error 0x%02x: %s\n",
             file,
             সারি,
             error,
             FT_Error_String( error ) );
#অন্যথা
    FT_UNUSED( error );
    FT_UNUSED( সারি );
    FT_UNUSED( file );
#endif

    ফেরত 0;
  }

#endif /* FT_DEBUG_LEVEL_ERROR */


#ifdef FT_DEBUG_LEVEL_TRACE

  /* array of trace levels, initialized to 0; */
  /* this gets adjusted at run-time           */
  স্থির পূর্ণসংখ্যা  ft_trace_levels_enabled[trace_গুনতি];

  /* array of trace levels, always initialized to 0 */
  স্থির পূর্ণসংখ্যা  ft_trace_levels_disabled[trace_গুনতি];

  /* a pointer to either `ft_trace_levels_enabled' */
  /* or `ft_trace_levels_disabled'                 */
  পূর্ণসংখ্যা*  ft_trace_levels;

  /* define array of trace toggle names */
#define FT_TRACE_DEF( x )  #x ,

  স্থির ধ্রুবক অক্ষর*  ft_trace_toggles[trace_গুনতি + 1] =
  {
#অন্তর্ভাব <freetype/internal/fttrace.h>
    ফাঁকা
  };

#undef FT_TRACE_DEF


  /* documentation is in ftdebug.h */

  FT_BASE_DEF( FT_Int )
  FT_Trace_Get_গুনতি( শূন্য )
  {
    ফেরত trace_গুনতি;
  }


  /* documentation is in ftdebug.h */

  FT_BASE_DEF( ধ্রুবক অক্ষর * )
  FT_Trace_Get_Name( FT_Int  idx )
  {
    পূর্ণসংখ্যা  max = FT_Trace_Get_গুনতি();


    যদি ( idx < max )
      ফেরত ft_trace_toggles[idx];
    অন্যথা
      ফেরত ফাঁকা;
  }


  /* documentation is in ftdebug.h */

  FT_BASE_DEF( শূন্য )
  FT_Trace_Disable( শূন্য )
  {
    ft_trace_levels = ft_trace_levels_disabled;
  }


  /* documentation is in ftdebug.h */

  FT_BASE_DEF( শূন্য )
  FT_Trace_Enable( শূন্য )
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
  FT_BASE_DEF( শূন্য )
  ft_debug_init( শূন্য )
  {
    ধ্রুবক অক্ষর*  ft2_debug = ফাঁকা;


#ifdef FT_DEBUG_LOGGING
    যদি ( ft_custom_trace_level != ফাঁকা )
      ft2_debug = ft_custom_trace_level;
    অন্যথা
      ft2_debug = ft_default_trace_level;
#অন্যথা
    ft2_debug = ft_getenv( "FT2_DEBUG" );
#endif

    যদি ( ft2_debug )
    {
      ধ্রুবক অক্ষর*  p = ft2_debug;
      ধ্রুবক অক্ষর*  q;


      জন্য ( ; *p; p++ )
      {
        /* skip leading whitespace and separators */
        যদি ( *p == ' ' || *p == '\t' || *p == ',' || *p == ';' || *p == '=' )
          চালু;

#ifdef FT_DEBUG_LOGGING

        /* check extra arguments for logging */
        যদি ( *p == '-' )
        {
          ধ্রুবক অক্ষর*  r = ++p;


          যদি ( *r == 'v' )
          {
            ধ্রুবক অক্ষর*  s = ++r;


            ft_component_flag = TRUE;

            যদি ( *s == 't' )
            {
              ft_timestamp_flag = TRUE;
              p++;
            }

            p++;
          }

          অন্যথা যদি ( *r == 't' )
          {
            ধ্রুবক অক্ষর*  s = ++r;


            ft_timestamp_flag = TRUE;

            যদি ( *s == 'v' )
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
        যখন ( *p && *p != ':' )
          p++;

        যদি ( !*p )
          বিরতি;

        যদি ( *p == ':' && p > q )
        {
          FT_Int  n, i, len = (FT_Int)( p - q );
          FT_Int  level = -1, found = -1;


          জন্য ( n = 0; n < trace_গুনতি; n++ )
          {
            ধ্রুবক অক্ষর*  toggle = ft_trace_toggles[n];


            জন্য ( i = 0; i < len; i++ )
            {
              যদি ( toggle[i] != q[i] )
                বিরতি;
            }

            যদি ( i == len && toggle[i] == 0 )
            {
              found = n;
              বিরতি;
            }
          }

          /* read level */
          p++;
          যদি ( *p )
          {
            level = *p - '0';
            যদি ( level < 0 || level > 7 )
              level = -1;
          }

          যদি ( found >= 0 && level >= 0 )
          {
            যদি ( found == trace_any )
            {
              /* special case for `any' */
              জন্য ( n = 0; n < trace_গুনতি; n++ )
                ft_trace_levels_enabled[n] = level;
            }
            অন্যথা
              ft_trace_levels_enabled[found] = level;
          }
        }
      }
    }

    ft_trace_levels = ft_trace_levels_enabled;
  }


#অন্যথা  /* !FT_DEBUG_LEVEL_TRACE */


  FT_BASE_DEF( শূন্য )
  ft_debug_init( শূন্য )
  {
    /* nothing */
  }


  FT_BASE_DEF( FT_Int )
  FT_Trace_Get_গুনতি( শূন্য )
  {
    ফেরত 0;
  }


  FT_BASE_DEF( ধ্রুবক অক্ষর * )
  FT_Trace_Get_Name( FT_Int  idx )
  {
    FT_UNUSED( idx );

    ফেরত ফাঁকা;
  }


  FT_BASE_DEF( শূন্য )
  FT_Trace_Disable( শূন্য )
  {
    /* nothing */
  }


  /* documentation is in ftdebug.h */

  FT_BASE_DEF( শূন্য )
  FT_Trace_Enable( শূন্য )
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

  FT_BASE_DEF( শূন্য )
  ft_logging_init( শূন্য )
  {
    ft_default_log_handler = ft_log_handler;
    ft_default_trace_level = ft_getenv( "FT2_DEBUG" );

    যদি ( ft_getenv( "FT_LOGGING_FILE" ) )
      ft_fileptr = ft_fopen( ft_getenv( "FT_LOGGING_FILE" ), "w" );
    অন্যথা
      ft_fileptr = stderr;

    ft_debug_init();

    /* Set the default output handler for 'dlg'. */
    dlg_set_handler( ft_default_log_handler, ফাঁকা );
  }


  FT_BASE_DEF( শূন্য )
  ft_logging_deinit( শূন্য )
  {
    যদি ( ft_fileptr != stderr )
      ft_fclose( ft_fileptr );
  }


  /**************************************************************************
   *
   * An output log handler for FreeType.
   *
   */
  FT_BASE_DEF( শূন্য )
  ft_log_handler( ধ্রুবক কাঠামো dlg_origin*  origin,
                  ধ্রুবক অক্ষর*               string,
                  শূন্য*                     তথ্য )
  {
    অক্ষর         features_buf[128];
    অক্ষর*        bufp = features_buf;

    FT_UNUSED( তথ্য );


    যদি ( ft_have_newline_char )
    {
      ধ্রুবক অক্ষর*  features        = ফাঁকা;
      size_t       features_দৈর্ঘ্য = 0;


#define FEATURES_TIMESTAMP            "[%h:%m] "
#define FEATURES_COMPONENT            "[%t] "
#define FEATURES_TIMESTAMP_COMPONENT  "[%h:%m %t] "

      যদি ( ft_timestamp_flag && ft_component_flag )
      {
        features        = FEATURES_TIMESTAMP_COMPONENT;
        features_দৈর্ঘ্য = আকার ( FEATURES_TIMESTAMP_COMPONENT );
      }
      অন্যথা যদি ( ft_timestamp_flag )
      {
        features        = FEATURES_TIMESTAMP;
        features_দৈর্ঘ্য = আকার ( FEATURES_TIMESTAMP );
      }
      অন্যথা যদি ( ft_component_flag )
      {
        features        = FEATURES_COMPONENT;
        features_দৈর্ঘ্য = আকার ( FEATURES_COMPONENT );
      }

      যদি ( ft_component_flag || ft_timestamp_flag )
      {
        ft_strncpy( features_buf, features, features_দৈর্ঘ্য );
        bufp += features_দৈর্ঘ্য - 1;
      }

      যদি ( ft_component_flag )
      {
        size_t  tag_দৈর্ঘ্য = ft_strlen( *origin->tags );
        size_t  i;


        /* To vertically align tracing messages we compensate the */
        /* different FT_COMPONENT string lengths by inserting an  */
        /* appropriate amount of space characters.                */
        জন্য ( i = 0;
              i < FT_MAX_TRACE_LEVEL_দৈর্ঘ্য - tag_দৈর্ঘ্য;
              i++ )
          *bufp++ = ' ';
      }
    }

    /* Finally add the format string for the tracing message. */
    *bufp++ = '%';
    *bufp++ = 'c';
    *bufp   = '\0';

    dlg_generic_outputf_stream( ft_fileptr,
                                (ধ্রুবক অক্ষর*)features_buf,
                                origin,
                                string,
                                dlg_default_output_styles,
                                true );

    যদি ( ft_strrchr( string, '\n' ) )
      ft_have_newline_char = TRUE;
    অন্যথা
      ft_have_newline_char = FALSE;
  }


  /* documentation is in ftdebug.h */
  FT_BASE_DEF( শূন্য )
  ft_add_tag( ধ্রুবক অক্ষর*  tag )
  {
    ft_component = tag;

    dlg_add_tag( tag, ফাঁকা );
  }


  /* documentation is in ftdebug.h */
  FT_BASE_DEF( শূন্য )
  ft_remove_tag( ধ্রুবক অক্ষর*  tag )
  {
    dlg_remove_tag( tag, ফাঁকা );
  }


  /* documentation is in ftlogging.h */

  FT_EXPORT_DEF( শূন্য )
  FT_Trace_Set_Level( ধ্রুবক অক্ষর*  level )
  {
    ft_component_flag     = FALSE;
    ft_timestamp_flag     = FALSE;
    ft_custom_trace_level = level;

    ft_debug_init();
  }


  /* documentation is in ftlogging.h */

  FT_EXPORT_DEF( শূন্য )
  FT_Trace_Set_Default_Level( শূন্য )
  {
    ft_component_flag     = FALSE;
    ft_timestamp_flag     = FALSE;
    ft_custom_trace_level = ফাঁকা;

    ft_debug_init();
  }


  /**************************************************************************
   *
   * Functions to handle a custom log handler.
   *
   */

  /* documentation is in ftlogging.h */

  FT_EXPORT_DEF( শূন্য )
  FT_Set_Log_Handler( FT_Custom_Log_Handler  handler )
  {
    custom_output_handler = handler;
  }


  /* documentation is in ftlogging.h */

  FT_EXPORT_DEF( শূন্য )
  FT_Set_Default_Log_Handler( শূন্য )
  {
    custom_output_handler = ফাঁকা;
  }


  /* documentation is in ftdebug.h */
  FT_BASE_DEF( শূন্য )
  FT_Logging_Callback( ধ্রুবক অক্ষর*  fmt,
                       ... )
  {
    va_list  ap;


    va_start( ap, fmt );
    custom_output_handler( ft_component, fmt, ap );
    va_end( ap );
  }

#অন্যথা /* !FT_DEBUG_LOGGING */

  FT_EXPORT_DEF( শূন্য )
  FT_Trace_Set_Level( ধ্রুবক অক্ষর*  level )
  {
    FT_UNUSED( level );
  }


  FT_EXPORT_DEF( শূন্য )
  FT_Trace_Set_Default_Level( শূন্য )
  {
    /* nothing */
  }


  FT_EXPORT_DEF( শূন্য )
  FT_Set_Log_Handler( FT_Custom_Log_Handler  handler )
  {
    FT_UNUSED( handler );
  }


  FT_EXPORT_DEF( শূন্য )
  FT_Set_Default_Log_Handler( শূন্য )
  {
    /* nothing */
  }

#endif /* !FT_DEBUG_LOGGING */


/* END */
