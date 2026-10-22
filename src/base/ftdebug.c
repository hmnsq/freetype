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


#అన్తర్భావ <freetype/freetype.h>
#అన్తర్భావ <freetype/ftlogging.h>
#అన్తర్భావ <freetype/internal/ftdebug.h>
#అన్తర్భావ <freetype/internal/ftobjs.h>


#నిర్వచితమైతే FT_DEBUG_LOGGING

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
  స్థిర స్థిరరాశి అక్షరం*  ft_default_trace_level = ఖాళీ;
  స్థిర FILE*        ft_fileptr             = ఖాళీ;
  స్థిర స్థిరరాశి అక్షరం*  ft_component           = ఖాళీ;
  స్థిర FT_Bool      ft_component_flag      = FALSE;
  స్థిర FT_Bool      ft_timestamp_flag      = FALSE;
  స్థిర FT_Bool      ft_have_newline_char   = TRUE;
  స్థిర స్థిరరాశి అక్షరం*  ft_custom_trace_level  = ఖాళీ;

  /* declared in ftdebug.h */

  dlg_handler            ft_default_log_handler = ఖాళీ;
  FT_Custom_Log_Handler  custom_output_handler  = ఖాళీ;

#అయితే_ముగింపు /* FT_DEBUG_LOGGING */


#నిర్వచితమైతే FT_DEBUG_LEVEL_ERROR

  /* documentation is in ftdebug.h */

  FT_పునాది_DEF( శూన్య )
  FT_సందేశం( స్థిరరాశి అక్షరం*  fmt,
              ... )
  {
    va_list  ap;


    va_ప్రారంభం( ap, fmt );
    vfprintf( stderr, fmt, ap );
    va_చివర( ap );
  }


  /* documentation is in ftdebug.h */

  FT_పునాది_DEF( శూన్య )
  FT_Panic( స్థిరరాశి అక్షరం*  fmt,
            ... )
  {
    va_list  ap;


    va_ప్రారంభం( ap, fmt );
    vfprintf( stderr, fmt, ap );
    va_చివర( ap );

    నిష్క్రమించు( EXIT_FAILURE );
  }


  /* documentation is in ftdebug.h */

  FT_పునాది_DEF( పూర్ణాంకం )
  FT_Throw( FT_Error     error,
            పూర్ణాంకం          పంక్తి,
            స్థిరరాశి అక్షరం*  file )
  {
#ఒకవేళ 0
    /* activating the code in this block makes FreeType very chatty */
    fprintf( stderr,
             "%s:%d: error 0x%02x: %s\n",
             file,
             పంక్తి,
             error,
             FT_Error_String( error ) );
#లేకపోతే
    FT_వాడని( error );
    FT_వాడని( పంక్తి );
    FT_వాడని( file );
#అయితే_ముగింపు

    తిరిగి 0;
  }

#అయితే_ముగింపు /* FT_DEBUG_LEVEL_ERROR */


#నిర్వచితమైతే FT_DEBUG_LEVEL_TRACE

  /* array of trace levels, initialized to 0; */
  /* this gets adjusted at run-time           */
  స్థిర పూర్ణాంకం  ft_trace_స్థాయిలు_enabled[trace_లెక్క];

  /* array of trace levels, always initialized to 0 */
  స్థిర పూర్ణాంకం  ft_trace_స్థాయిలు_disabled[trace_లెక్క];

  /* a pointer to either `ft_trace_levels_enabled' */
  /* or `ft_trace_levels_disabled'                 */
  పూర్ణాంకం*  ft_trace_స్థాయిలు;

  /* define array of trace toggle names */
#నిర్వచించు FT_TRACE_DEF( x )  #x ,

  స్థిర స్థిరరాశి అక్షరం*  ft_trace_toggles[trace_లెక్క + 1] =
  {
#అన్తర్భావ <freetype/internal/fttrace.h>
    ఖాళీ
  };

#నిర్వచనం_రద్దు FT_TRACE_DEF


  /* documentation is in ftdebug.h */

  FT_పునాది_DEF( FT_Int )
  FT_Trace_Get_లెక్క( శూన్య )
  {
    తిరిగి trace_లెక్క;
  }


  /* documentation is in ftdebug.h */

  FT_పునాది_DEF( స్థిరరాశి అక్షరం * )
  FT_Trace_Get_Name( FT_Int  idx )
  {
    పూర్ణాంకం  max = FT_Trace_Get_లెక్క();


    ఒకవేళ ( idx < max )
      తిరిగి ft_trace_toggles[idx];
    లేకపోతే
      తిరిగి ఖాళీ;
  }


  /* documentation is in ftdebug.h */

  FT_పునాది_DEF( శూన్య )
  FT_Trace_Disable( శూన్య )
  {
    ft_trace_స్థాయిలు = ft_trace_స్థాయిలు_disabled;
  }


  /* documentation is in ftdebug.h */

  FT_పునాది_DEF( శూన్య )
  FT_Trace_Enable( శూన్య )
  {
    ft_trace_స్థాయిలు = ft_trace_స్థాయిలు_enabled;
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
  FT_పునాది_DEF( శూన్య )
  ft_debug_init( శూన్య )
  {
    స్థిరరాశి అక్షరం*  ft2_debug = ఖాళీ;


#నిర్వచితమైతే FT_DEBUG_LOGGING
    ఒకవేళ ( ft_custom_trace_level != ఖాళీ )
      ft2_debug = ft_custom_trace_level;
    లేకపోతే
      ft2_debug = ft_default_trace_level;
#లేకపోతే
    ft2_debug = ft_getenv( "FT2_DEBUG" );
#అయితే_ముగింపు

    ఒకవేళ ( ft2_debug )
    {
      స్థిరరాశి అక్షరం*  p = ft2_debug;
      స్థిరరాశి అక్షరం*  q;


      కొరకు ( ; *p; p++ )
      {
        /* skip leading whitespace and separators */
        ఒకవేళ ( *p == ' ' || *p == '\t' || *p == ',' || *p == ';' || *p == '=' )
          కొనసాగు;

#నిర్వచితమైతే FT_DEBUG_LOGGING

        /* check extra arguments for logging */
        ఒకవేళ ( *p == '-' )
        {
          స్థిరరాశి అక్షరం*  r = ++p;


          ఒకవేళ ( *r == 'v' )
          {
            స్థిరరాశి అక్షరం*  s = ++r;


            ft_component_flag = TRUE;

            ఒకవేళ ( *s == 't' )
            {
              ft_timestamp_flag = TRUE;
              p++;
            }

            p++;
          }

          లేకపోతే ఒకవేళ ( *r == 't' )
          {
            స్థిరరాశి అక్షరం*  s = ++r;


            ft_timestamp_flag = TRUE;

            ఒకవేళ ( *s == 'v' )
            {
              ft_component_flag = TRUE;
              p++;
            }

            p++;
          }
        }

#అయితే_ముగింపు /* FT_DEBUG_LOGGING */

        /* read toggle name, followed by ':' */
        q = p;
        అయితే ( *p && *p != ':' )
          p++;

        ఒకవేళ ( !*p )
          ఆపు;

        ఒకవేళ ( *p == ':' && p > q )
        {
          FT_Int  n, i, len = (FT_Int)( p - q );
          FT_Int  level = -1, దొరికింది = -1;


          కొరకు ( n = 0; n < trace_లెక్క; n++ )
          {
            స్థిరరాశి అక్షరం*  toggle = ft_trace_toggles[n];


            కొరకు ( i = 0; i < len; i++ )
            {
              ఒకవేళ ( toggle[i] != q[i] )
                ఆపు;
            }

            ఒకవేళ ( i == len && toggle[i] == 0 )
            {
              దొరికింది = n;
              ఆపు;
            }
          }

          /* read level */
          p++;
          ఒకవేళ ( *p )
          {
            level = *p - '0';
            ఒకవేళ ( level < 0 || level > 7 )
              level = -1;
          }

          ఒకవేళ ( దొరికింది >= 0 && level >= 0 )
          {
            ఒకవేళ ( దొరికింది == trace_any )
            {
              /* special case for `any' */
              కొరకు ( n = 0; n < trace_లెక్క; n++ )
                ft_trace_స్థాయిలు_enabled[n] = level;
            }
            లేకపోతే
              ft_trace_స్థాయిలు_enabled[దొరికింది] = level;
          }
        }
      }
    }

    ft_trace_స్థాయిలు = ft_trace_స్థాయిలు_enabled;
  }


#లేకపోతే  /* !FT_DEBUG_LEVEL_TRACE */


  FT_పునాది_DEF( శూన్య )
  ft_debug_init( శూన్య )
  {
    /* nothing */
  }


  FT_పునాది_DEF( FT_Int )
  FT_Trace_Get_లెక్క( శూన్య )
  {
    తిరిగి 0;
  }


  FT_పునాది_DEF( స్థిరరాశి అక్షరం * )
  FT_Trace_Get_Name( FT_Int  idx )
  {
    FT_వాడని( idx );

    తిరిగి ఖాళీ;
  }


  FT_పునాది_DEF( శూన్య )
  FT_Trace_Disable( శూన్య )
  {
    /* nothing */
  }


  /* documentation is in ftdebug.h */

  FT_పునాది_DEF( శూన్య )
  FT_Trace_Enable( శూన్య )
  {
    /* nothing */
  }

#అయితే_ముగింపు /* !FT_DEBUG_LEVEL_TRACE */


#నిర్వచితమైతే FT_DEBUG_LOGGING

  /**************************************************************************
   *
   * Initialize and de-initialize 'dlg' library.
   *
   */

  FT_పునాది_DEF( శూన్య )
  ft_logging_init( శూన్య )
  {
    ft_default_log_handler = ft_log_handler;
    ft_default_trace_level = ft_getenv( "FT2_DEBUG" );

    ఒకవేళ ( ft_getenv( "FT_LOGGING_FILE" ) )
      ft_fileptr = ft_fopen( ft_getenv( "FT_LOGGING_FILE" ), "w" );
    లేకపోతే
      ft_fileptr = stderr;

    ft_debug_init();

    /* Set the default output handler for 'dlg'. */
    dlg_సమితి_handler( ft_default_log_handler, ఖాళీ );
  }


  FT_పునాది_DEF( శూన్య )
  ft_logging_deinit( శూన్య )
  {
    ఒకవేళ ( ft_fileptr != stderr )
      ft_fclose( ft_fileptr );
  }


  /**************************************************************************
   *
   * An output log handler for FreeType.
   *
   */
  FT_పునాది_DEF( శూన్య )
  ft_log_handler( స్థిరరాశి నిర్మాణం dlg_origin*  origin,
                  స్థిరరాశి అక్షరం*               string,
                  శూన్య*                     దత్తాంశం )
  {
    అక్షరం         features_buf[128];
    అక్షరం*        bufp = features_buf;

    FT_వాడని( దత్తాంశం );


    ఒకవేళ ( ft_have_newline_char )
    {
      స్థిరరాశి అక్షరం*  features        = ఖాళీ;
      size_t       features_నిడివి = 0;


#నిర్వచించు FEATURES_TIMESTAMP            "[%h:%m] "
#నిర్వచించు FEATURES_COMPONENT            "[%t] "
#నిర్వచించు FEATURES_TIMESTAMP_COMPONENT  "[%h:%m %t] "

      ఒకవేళ ( ft_timestamp_flag && ft_component_flag )
      {
        features        = FEATURES_TIMESTAMP_COMPONENT;
        features_నిడివి = పరిమాణం ( FEATURES_TIMESTAMP_COMPONENT );
      }
      లేకపోతే ఒకవేళ ( ft_timestamp_flag )
      {
        features        = FEATURES_TIMESTAMP;
        features_నిడివి = పరిమాణం ( FEATURES_TIMESTAMP );
      }
      లేకపోతే ఒకవేళ ( ft_component_flag )
      {
        features        = FEATURES_COMPONENT;
        features_నిడివి = పరిమాణం ( FEATURES_COMPONENT );
      }

      ఒకవేళ ( ft_component_flag || ft_timestamp_flag )
      {
        ft_strncpy( features_buf, features, features_నిడివి );
        bufp += features_నిడివి - 1;
      }

      ఒకవేళ ( ft_component_flag )
      {
        size_t  ట్యాగ్_నిడివి = ft_strlen( *origin->ట్యాగులు );
        size_t  i;


        /* To vertically align tracing messages we compensate the */
        /* different FT_COMPONENT string lengths by inserting an  */
        /* appropriate amount of space characters.                */
        కొరకు ( i = 0;
              i < FT_MAX_TRACE_LEVEL_నిడివి - ట్యాగ్_నిడివి;
              i++ )
          *bufp++ = ' ';
      }
    }

    /* Finally add the format string for the tracing message. */
    *bufp++ = '%';
    *bufp++ = 'c';
    *bufp   = '\0';

    dlg_generic_outputf_ప్రవాహం( ft_fileptr,
                                (స్థిరరాశి అక్షరం*)features_buf,
                                origin,
                                string,
                                dlg_default_output_styles,
                                true );

    ఒకవేళ ( ft_strrchr( string, '\n' ) )
      ft_have_newline_char = TRUE;
    లేకపోతే
      ft_have_newline_char = FALSE;
  }


  /* documentation is in ftdebug.h */
  FT_పునాది_DEF( శూన్య )
  ft_కూడు_ట్యాగ్( స్థిరరాశి అక్షరం*  ట్యాగ్ )
  {
    ft_component = ట్యాగ్;

    dlg_కూడు_ట్యాగ్( ట్యాగ్, ఖాళీ );
  }


  /* documentation is in ftdebug.h */
  FT_పునాది_DEF( శూన్య )
  ft_remove_ట్యాగ్( స్థిరరాశి అక్షరం*  ట్యాగ్ )
  {
    dlg_remove_ట్యాగ్( ట్యాగ్, ఖాళీ );
  }


  /* documentation is in ftlogging.h */

  FT_EXPORT_DEF( శూన్య )
  FT_Trace_సమితి_Level( స్థిరరాశి అక్షరం*  level )
  {
    ft_component_flag     = FALSE;
    ft_timestamp_flag     = FALSE;
    ft_custom_trace_level = level;

    ft_debug_init();
  }


  /* documentation is in ftlogging.h */

  FT_EXPORT_DEF( శూన్య )
  FT_Trace_సమితి_Default_Level( శూన్య )
  {
    ft_component_flag     = FALSE;
    ft_timestamp_flag     = FALSE;
    ft_custom_trace_level = ఖాళీ;

    ft_debug_init();
  }


  /**************************************************************************
   *
   * Functions to handle a custom log handler.
   *
   */

  /* documentation is in ftlogging.h */

  FT_EXPORT_DEF( శూన్య )
  FT_సమితి_Log_Handler( FT_Custom_Log_Handler  handler )
  {
    custom_output_handler = handler;
  }


  /* documentation is in ftlogging.h */

  FT_EXPORT_DEF( శూన్య )
  FT_సమితి_Default_Log_Handler( శూన్య )
  {
    custom_output_handler = ఖాళీ;
  }


  /* documentation is in ftdebug.h */
  FT_పునాది_DEF( శూన్య )
  FT_Logging_Callback( స్థిరరాశి అక్షరం*  fmt,
                       ... )
  {
    va_list  ap;


    va_ప్రారంభం( ap, fmt );
    custom_output_handler( ft_component, fmt, ap );
    va_చివర( ap );
  }

#లేకపోతే /* !FT_DEBUG_LOGGING */

  FT_EXPORT_DEF( శూన్య )
  FT_Trace_సమితి_Level( స్థిరరాశి అక్షరం*  level )
  {
    FT_వాడని( level );
  }


  FT_EXPORT_DEF( శూన్య )
  FT_Trace_సమితి_Default_Level( శూన్య )
  {
    /* nothing */
  }


  FT_EXPORT_DEF( శూన్య )
  FT_సమితి_Log_Handler( FT_Custom_Log_Handler  handler )
  {
    FT_వాడని( handler );
  }


  FT_EXPORT_DEF( శూన్య )
  FT_సమితి_Default_Log_Handler( శూన్య )
  {
    /* nothing */
  }

#అయితే_ముగింపు /* !FT_DEBUG_LOGGING */


/*చివర*/
