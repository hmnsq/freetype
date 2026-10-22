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


#اگر_تعریف_شدہ FT_DEBUG_LOGGING

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
  ساکن ثابت حرف*  ft_default_trace_level = تہی;
  ساکن FILE*        ft_fileptr             = تہی;
  ساکن ثابت حرف*  ft_component           = تہی;
  ساکن FT_Bool      ft_component_flag      = FALSE;
  ساکن FT_Bool      ft_timestamp_flag      = FALSE;
  ساکن FT_Bool      ft_have_newline_char   = TRUE;
  ساکن ثابت حرف*  ft_custom_trace_level  = تہی;

  /* declared in ftdebug.h */

  dlg_handler            ft_default_log_handler = تہی;
  FT_Custom_Log_Handler  custom_output_handler  = تہی;

#اگر_ختم /* FT_DEBUG_LOGGING */


#اگر_تعریف_شدہ FT_DEBUG_LEVEL_ERROR

  /* documentation is in ftdebug.h */

  FT_بنیاد_DEF( خالی )
  FT_پیغام( ثابت حرف*  fmt,
              ... )
  {
    va_list  ap;


    va_آغاز( ap, fmt );
    vfprintf( stderr, fmt, ap );
    va_انتہا( ap );
  }


  /* documentation is in ftdebug.h */

  FT_بنیاد_DEF( خالی )
  FT_Panic( ثابت حرف*  fmt,
            ... )
  {
    va_list  ap;


    va_آغاز( ap, fmt );
    vfprintf( stderr, fmt, ap );
    va_انتہا( ap );

    باہر_نکلو( EXIT_FAILURE );
  }


  /* documentation is in ftdebug.h */

  FT_بنیاد_DEF( صحیح )
  FT_Throw( FT_Error     error,
            صحیح          سطر,
            ثابت حرف*  file )
  {
#اگر 0
    /* activating the code in this block makes FreeType very chatty */
    fprintf( stderr,
             "%s:%d: error 0x%02x: %s\n",
             file,
             سطر,
             error,
             FT_Error_String( error ) );
#ورنہ
    FT_غیر_مستعمل( error );
    FT_غیر_مستعمل( سطر );
    FT_غیر_مستعمل( file );
#اگر_ختم

    واپس 0;
  }

#اگر_ختم /* FT_DEBUG_LEVEL_ERROR */


#اگر_تعریف_شدہ FT_DEBUG_LEVEL_TRACE

  /* array of trace levels, initialized to 0; */
  /* this gets adjusted at run-time           */
  ساکن صحیح  ft_trace_سطحیں_enabled[trace_گنتی];

  /* array of trace levels, always initialized to 0 */
  ساکن صحیح  ft_trace_سطحیں_disabled[trace_گنتی];

  /* a pointer to either `ft_trace_levels_enabled' */
  /* or `ft_trace_levels_disabled'                 */
  صحیح*  ft_trace_سطحیں;

  /* define array of trace toggle names */
#تعریف_کرو FT_TRACE_DEF( x )  #x ,

  ساکن ثابت حرف*  ft_trace_toggles[trace_گنتی + 1] =
  {
#شامل <freetype/internal/fttrace.h>
    تہی
  };

#تعریف_منسوخ FT_TRACE_DEF


  /* documentation is in ftdebug.h */

  FT_بنیاد_DEF( FT_Int )
  FT_Trace_Get_گنتی( خالی )
  {
    واپس trace_گنتی;
  }


  /* documentation is in ftdebug.h */

  FT_بنیاد_DEF( ثابت حرف * )
  FT_Trace_Get_Name( FT_Int  idx )
  {
    صحیح  max = FT_Trace_Get_گنتی();


    اگر ( idx < max )
      واپس ft_trace_toggles[idx];
    ورنہ
      واپس تہی;
  }


  /* documentation is in ftdebug.h */

  FT_بنیاد_DEF( خالی )
  FT_Trace_Disable( خالی )
  {
    ft_trace_سطحیں = ft_trace_سطحیں_disabled;
  }


  /* documentation is in ftdebug.h */

  FT_بنیاد_DEF( خالی )
  FT_Trace_Enable( خالی )
  {
    ft_trace_سطحیں = ft_trace_سطحیں_enabled;
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
  FT_بنیاد_DEF( خالی )
  ft_debug_init( خالی )
  {
    ثابت حرف*  ft2_debug = تہی;


#اگر_تعریف_شدہ FT_DEBUG_LOGGING
    اگر ( ft_custom_trace_level != تہی )
      ft2_debug = ft_custom_trace_level;
    ورنہ
      ft2_debug = ft_default_trace_level;
#ورنہ
    ft2_debug = ft_getenv( "FT2_DEBUG" );
#اگر_ختم

    اگر ( ft2_debug )
    {
      ثابت حرف*  p = ft2_debug;
      ثابت حرف*  q;


      برائے ( ; *p; p++ )
      {
        /* skip leading whitespace and separators */
        اگر ( *p == ' ' || *p == '\t' || *p == ',' || *p == ';' || *p == '=' )
          جاری;

#اگر_تعریف_شدہ FT_DEBUG_LOGGING

        /* check extra arguments for logging */
        اگر ( *p == '-' )
        {
          ثابت حرف*  r = ++p;


          اگر ( *r == 'v' )
          {
            ثابت حرف*  s = ++r;


            ft_component_flag = TRUE;

            اگر ( *s == 't' )
            {
              ft_timestamp_flag = TRUE;
              p++;
            }

            p++;
          }

          ورنہ اگر ( *r == 't' )
          {
            ثابت حرف*  s = ++r;


            ft_timestamp_flag = TRUE;

            اگر ( *s == 'v' )
            {
              ft_component_flag = TRUE;
              p++;
            }

            p++;
          }
        }

#اگر_ختم /* FT_DEBUG_LOGGING */

        /* read toggle name, followed by ':' */
        q = p;
        جب ( *p && *p != ':' )
          p++;

        اگر ( !*p )
          توڑو;

        اگر ( *p == ':' && p > q )
        {
          FT_Int  n, i, len = (FT_Int)( p - q );
          FT_Int  level = -1, ملا = -1;


          برائے ( n = 0; n < trace_گنتی; n++ )
          {
            ثابت حرف*  toggle = ft_trace_toggles[n];


            برائے ( i = 0; i < len; i++ )
            {
              اگر ( toggle[i] != q[i] )
                توڑو;
            }

            اگر ( i == len && toggle[i] == 0 )
            {
              ملا = n;
              توڑو;
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

          اگر ( ملا >= 0 && level >= 0 )
          {
            اگر ( ملا == trace_any )
            {
              /* special case for `any' */
              برائے ( n = 0; n < trace_گنتی; n++ )
                ft_trace_سطحیں_enabled[n] = level;
            }
            ورنہ
              ft_trace_سطحیں_enabled[ملا] = level;
          }
        }
      }
    }

    ft_trace_سطحیں = ft_trace_سطحیں_enabled;
  }


#ورنہ  /* !FT_DEBUG_LEVEL_TRACE */


  FT_بنیاد_DEF( خالی )
  ft_debug_init( خالی )
  {
    /* nothing */
  }


  FT_بنیاد_DEF( FT_Int )
  FT_Trace_Get_گنتی( خالی )
  {
    واپس 0;
  }


  FT_بنیاد_DEF( ثابت حرف * )
  FT_Trace_Get_Name( FT_Int  idx )
  {
    FT_غیر_مستعمل( idx );

    واپس تہی;
  }


  FT_بنیاد_DEF( خالی )
  FT_Trace_Disable( خالی )
  {
    /* nothing */
  }


  /* documentation is in ftdebug.h */

  FT_بنیاد_DEF( خالی )
  FT_Trace_Enable( خالی )
  {
    /* nothing */
  }

#اگر_ختم /* !FT_DEBUG_LEVEL_TRACE */


#اگر_تعریف_شدہ FT_DEBUG_LOGGING

  /**************************************************************************
   *
   * Initialize and de-initialize 'dlg' library.
   *
   */

  FT_بنیاد_DEF( خالی )
  ft_logging_init( خالی )
  {
    ft_default_log_handler = ft_log_handler;
    ft_default_trace_level = ft_getenv( "FT2_DEBUG" );

    اگر ( ft_getenv( "FT_LOGGING_FILE" ) )
      ft_fileptr = ft_fopen( ft_getenv( "FT_LOGGING_FILE" ), "w" );
    ورنہ
      ft_fileptr = stderr;

    ft_debug_init();

    /* Set the default output handler for 'dlg'. */
    dlg_سیٹ_handler( ft_default_log_handler, تہی );
  }


  FT_بنیاد_DEF( خالی )
  ft_logging_deinit( خالی )
  {
    اگر ( ft_fileptr != stderr )
      ft_fclose( ft_fileptr );
  }


  /**************************************************************************
   *
   * An output log handler for FreeType.
   *
   */
  FT_بنیاد_DEF( خالی )
  ft_log_handler( ثابت ساخت dlg_origin*  origin,
                  ثابت حرف*               string,
                  خالی*                     ڈیٹا )
  {
    حرف         features_buf[128];
    حرف*        bufp = features_buf;

    FT_غیر_مستعمل( ڈیٹا );


    اگر ( ft_have_newline_char )
    {
      ثابت حرف*  features        = تہی;
      size_t       features_لمبائی = 0;


#تعریف_کرو FEATURES_TIMESTAMP            "[%h:%m] "
#تعریف_کرو FEATURES_COMPONENT            "[%t] "
#تعریف_کرو FEATURES_TIMESTAMP_COMPONENT  "[%h:%m %t] "

      اگر ( ft_timestamp_flag && ft_component_flag )
      {
        features        = FEATURES_TIMESTAMP_COMPONENT;
        features_لمبائی = حجم ( FEATURES_TIMESTAMP_COMPONENT );
      }
      ورنہ اگر ( ft_timestamp_flag )
      {
        features        = FEATURES_TIMESTAMP;
        features_لمبائی = حجم ( FEATURES_TIMESTAMP );
      }
      ورنہ اگر ( ft_component_flag )
      {
        features        = FEATURES_COMPONENT;
        features_لمبائی = حجم ( FEATURES_COMPONENT );
      }

      اگر ( ft_component_flag || ft_timestamp_flag )
      {
        ft_strncpy( features_buf, features, features_لمبائی );
        bufp += features_لمبائی - 1;
      }

      اگر ( ft_component_flag )
      {
        size_t  لیبل_لمبائی = ft_strlen( *origin->لیبل_فہرست );
        size_t  i;


        /* To vertically align tracing messages we compensate the */
        /* different FT_COMPONENT string lengths by inserting an  */
        /* appropriate amount of space characters.                */
        برائے ( i = 0;
              i < FT_MAX_TRACE_LEVEL_لمبائی - لیبل_لمبائی;
              i++ )
          *bufp++ = ' ';
      }
    }

    /* Finally add the format string for the tracing message. */
    *bufp++ = '%';
    *bufp++ = 'c';
    *bufp   = '\0';

    dlg_generic_outputf_دھارا( ft_fileptr,
                                (ثابت حرف*)features_buf,
                                origin,
                                string,
                                dlg_default_output_styles,
                                true );

    اگر ( ft_strrchr( string, '\n' ) )
      ft_have_newline_char = TRUE;
    ورنہ
      ft_have_newline_char = FALSE;
  }


  /* documentation is in ftdebug.h */
  FT_بنیاد_DEF( خالی )
  ft_جوڑو_لیبل( ثابت حرف*  لیبل )
  {
    ft_component = لیبل;

    dlg_جوڑو_لیبل( لیبل, تہی );
  }


  /* documentation is in ftdebug.h */
  FT_بنیاد_DEF( خالی )
  ft_remove_لیبل( ثابت حرف*  لیبل )
  {
    dlg_remove_لیبل( لیبل, تہی );
  }


  /* documentation is in ftlogging.h */

  FT_EXPORT_DEF( خالی )
  FT_Trace_سیٹ_Level( ثابت حرف*  level )
  {
    ft_component_flag     = FALSE;
    ft_timestamp_flag     = FALSE;
    ft_custom_trace_level = level;

    ft_debug_init();
  }


  /* documentation is in ftlogging.h */

  FT_EXPORT_DEF( خالی )
  FT_Trace_سیٹ_Default_Level( خالی )
  {
    ft_component_flag     = FALSE;
    ft_timestamp_flag     = FALSE;
    ft_custom_trace_level = تہی;

    ft_debug_init();
  }


  /**************************************************************************
   *
   * Functions to handle a custom log handler.
   *
   */

  /* documentation is in ftlogging.h */

  FT_EXPORT_DEF( خالی )
  FT_سیٹ_Log_Handler( FT_Custom_Log_Handler  handler )
  {
    custom_output_handler = handler;
  }


  /* documentation is in ftlogging.h */

  FT_EXPORT_DEF( خالی )
  FT_سیٹ_Default_Log_Handler( خالی )
  {
    custom_output_handler = تہی;
  }


  /* documentation is in ftdebug.h */
  FT_بنیاد_DEF( خالی )
  FT_Logging_Callback( ثابت حرف*  fmt,
                       ... )
  {
    va_list  ap;


    va_آغاز( ap, fmt );
    custom_output_handler( ft_component, fmt, ap );
    va_انتہا( ap );
  }

#ورنہ /* !FT_DEBUG_LOGGING */

  FT_EXPORT_DEF( خالی )
  FT_Trace_سیٹ_Level( ثابت حرف*  level )
  {
    FT_غیر_مستعمل( level );
  }


  FT_EXPORT_DEF( خالی )
  FT_Trace_سیٹ_Default_Level( خالی )
  {
    /* nothing */
  }


  FT_EXPORT_DEF( خالی )
  FT_سیٹ_Log_Handler( FT_Custom_Log_Handler  handler )
  {
    FT_غیر_مستعمل( handler );
  }


  FT_EXPORT_DEF( خالی )
  FT_سیٹ_Default_Log_Handler( خالی )
  {
    /* nothing */
  }

#اگر_ختم /* !FT_DEBUG_LOGGING */


/*انتہا*/
