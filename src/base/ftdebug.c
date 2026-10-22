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


#અન્તર્ભાવ <freetype/freetype.h>
#અન્તર્ભાવ <freetype/ftlogging.h>
#અન્તર્ભાવ <freetype/internal/ftdebug.h>
#અન્તર્ભાવ <freetype/internal/ftobjs.h>


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
  સ્થિર અચલ અક્ષર*  ft_default_trace_level = ખાલી;
  સ્થિર FILE*        ft_fileptr             = ખાલી;
  સ્થિર અચલ અક્ષર*  ft_component           = ખાલી;
  સ્થિર FT_Bool      ft_component_flag      = FALSE;
  સ્થિર FT_Bool      ft_timestamp_flag      = FALSE;
  સ્થિર FT_Bool      ft_have_newline_char   = TRUE;
  સ્થિર અચલ અક્ષર*  ft_custom_trace_level  = ખાલી;

  /* declared in ftdebug.h */

  dlg_handler            ft_default_log_handler = ખાલી;
  FT_Custom_Log_Handler  custom_output_handler  = ખાલી;

#endif /* FT_DEBUG_LOGGING */


#ifdef FT_DEBUG_LEVEL_ERROR

  /* documentation is in ftdebug.h */

  FT_BASE_DEF( શૂન્ય )
  FT_Message( અચલ અક્ષર*  fmt,
              ... )
  {
    va_list  ap;


    va_start( ap, fmt );
    vfprintf( stderr, fmt, ap );
    va_end( ap );
  }


  /* documentation is in ftdebug.h */

  FT_BASE_DEF( શૂન્ય )
  FT_Panic( અચલ અક્ષર*  fmt,
            ... )
  {
    va_list  ap;


    va_start( ap, fmt );
    vfprintf( stderr, fmt, ap );
    va_end( ap );

    exit( EXIT_FAILURE );
  }


  /* documentation is in ftdebug.h */

  FT_BASE_DEF( પૂર્ણાંક )
  FT_Throw( FT_Error     error,
            પૂર્ણાંક          લીટી,
            અચલ અક્ષર*  file )
  {
#જો 0
    /* activating the code in this block makes FreeType very chatty */
    fprintf( stderr,
             "%s:%d: error 0x%02x: %s\n",
             file,
             લીટી,
             error,
             FT_Error_String( error ) );
#નહીંતર
    FT_UNUSED( error );
    FT_UNUSED( લીટી );
    FT_UNUSED( file );
#endif

    પાછા 0;
  }

#endif /* FT_DEBUG_LEVEL_ERROR */


#ifdef FT_DEBUG_LEVEL_TRACE

  /* array of trace levels, initialized to 0; */
  /* this gets adjusted at run-time           */
  સ્થિર પૂર્ણાંક  ft_trace_levels_enabled[trace_ગણતરી];

  /* array of trace levels, always initialized to 0 */
  સ્થિર પૂર્ણાંક  ft_trace_levels_disabled[trace_ગણતરી];

  /* a pointer to either `ft_trace_levels_enabled' */
  /* or `ft_trace_levels_disabled'                 */
  પૂર્ણાંક*  ft_trace_levels;

  /* define array of trace toggle names */
#define FT_TRACE_DEF( x )  #x ,

  સ્થિર અચલ અક્ષર*  ft_trace_toggles[trace_ગણતરી + 1] =
  {
#અન્તર્ભાવ <freetype/internal/fttrace.h>
    ખાલી
  };

#undef FT_TRACE_DEF


  /* documentation is in ftdebug.h */

  FT_BASE_DEF( FT_Int )
  FT_Trace_Get_ગણતરી( શૂન્ય )
  {
    પાછા trace_ગણતરી;
  }


  /* documentation is in ftdebug.h */

  FT_BASE_DEF( અચલ અક્ષર * )
  FT_Trace_Get_Name( FT_Int  idx )
  {
    પૂર્ણાંક  max = FT_Trace_Get_ગણતરી();


    જો ( idx < max )
      પાછા ft_trace_toggles[idx];
    નહીંતર
      પાછા ખાલી;
  }


  /* documentation is in ftdebug.h */

  FT_BASE_DEF( શૂન્ય )
  FT_Trace_Disable( શૂન્ય )
  {
    ft_trace_levels = ft_trace_levels_disabled;
  }


  /* documentation is in ftdebug.h */

  FT_BASE_DEF( શૂન્ય )
  FT_Trace_Enable( શૂન્ય )
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
  FT_BASE_DEF( શૂન્ય )
  ft_debug_init( શૂન્ય )
  {
    અચલ અક્ષર*  ft2_debug = ખાલી;


#ifdef FT_DEBUG_LOGGING
    જો ( ft_custom_trace_level != ખાલી )
      ft2_debug = ft_custom_trace_level;
    નહીંતર
      ft2_debug = ft_default_trace_level;
#નહીંતર
    ft2_debug = ft_getenv( "FT2_DEBUG" );
#endif

    જો ( ft2_debug )
    {
      અચલ અક્ષર*  p = ft2_debug;
      અચલ અક્ષર*  q;


      માટે ( ; *p; p++ )
      {
        /* skip leading whitespace and separators */
        જો ( *p == ' ' || *p == '\t' || *p == ',' || *p == ';' || *p == '=' )
          ચાલુ;

#ifdef FT_DEBUG_LOGGING

        /* check extra arguments for logging */
        જો ( *p == '-' )
        {
          અચલ અક્ષર*  r = ++p;


          જો ( *r == 'v' )
          {
            અચલ અક્ષર*  s = ++r;


            ft_component_flag = TRUE;

            જો ( *s == 't' )
            {
              ft_timestamp_flag = TRUE;
              p++;
            }

            p++;
          }

          નહીંતર જો ( *r == 't' )
          {
            અચલ અક્ષર*  s = ++r;


            ft_timestamp_flag = TRUE;

            જો ( *s == 'v' )
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
        જ્યારે ( *p && *p != ':' )
          p++;

        જો ( !*p )
          તોડો;

        જો ( *p == ':' && p > q )
        {
          FT_Int  n, i, len = (FT_Int)( p - q );
          FT_Int  level = -1, found = -1;


          માટે ( n = 0; n < trace_ગણતરી; n++ )
          {
            અચલ અક્ષર*  toggle = ft_trace_toggles[n];


            માટે ( i = 0; i < len; i++ )
            {
              જો ( toggle[i] != q[i] )
                તોડો;
            }

            જો ( i == len && toggle[i] == 0 )
            {
              found = n;
              તોડો;
            }
          }

          /* read level */
          p++;
          જો ( *p )
          {
            level = *p - '0';
            જો ( level < 0 || level > 7 )
              level = -1;
          }

          જો ( found >= 0 && level >= 0 )
          {
            જો ( found == trace_any )
            {
              /* special case for `any' */
              માટે ( n = 0; n < trace_ગણતરી; n++ )
                ft_trace_levels_enabled[n] = level;
            }
            નહીંતર
              ft_trace_levels_enabled[found] = level;
          }
        }
      }
    }

    ft_trace_levels = ft_trace_levels_enabled;
  }


#નહીંતર  /* !FT_DEBUG_LEVEL_TRACE */


  FT_BASE_DEF( શૂન્ય )
  ft_debug_init( શૂન્ય )
  {
    /* nothing */
  }


  FT_BASE_DEF( FT_Int )
  FT_Trace_Get_ગણતરી( શૂન્ય )
  {
    પાછા 0;
  }


  FT_BASE_DEF( અચલ અક્ષર * )
  FT_Trace_Get_Name( FT_Int  idx )
  {
    FT_UNUSED( idx );

    પાછા ખાલી;
  }


  FT_BASE_DEF( શૂન્ય )
  FT_Trace_Disable( શૂન્ય )
  {
    /* nothing */
  }


  /* documentation is in ftdebug.h */

  FT_BASE_DEF( શૂન્ય )
  FT_Trace_Enable( શૂન્ય )
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

  FT_BASE_DEF( શૂન્ય )
  ft_logging_init( શૂન્ય )
  {
    ft_default_log_handler = ft_log_handler;
    ft_default_trace_level = ft_getenv( "FT2_DEBUG" );

    જો ( ft_getenv( "FT_LOGGING_FILE" ) )
      ft_fileptr = ft_fopen( ft_getenv( "FT_LOGGING_FILE" ), "w" );
    નહીંતર
      ft_fileptr = stderr;

    ft_debug_init();

    /* Set the default output handler for 'dlg'. */
    dlg_set_handler( ft_default_log_handler, ખાલી );
  }


  FT_BASE_DEF( શૂન્ય )
  ft_logging_deinit( શૂન્ય )
  {
    જો ( ft_fileptr != stderr )
      ft_fclose( ft_fileptr );
  }


  /**************************************************************************
   *
   * An output log handler for FreeType.
   *
   */
  FT_BASE_DEF( શૂન્ય )
  ft_log_handler( અચલ સંરચના dlg_origin*  origin,
                  અચલ અક્ષર*               string,
                  શૂન્ય*                     data )
  {
    અક્ષર         features_buf[128];
    અક્ષર*        bufp = features_buf;

    FT_UNUSED( data );


    જો ( ft_have_newline_char )
    {
      અચલ અક્ષર*  features        = ખાલી;
      size_t       features_length = 0;


#define FEATURES_TIMESTAMP            "[%h:%m] "
#define FEATURES_COMPONENT            "[%t] "
#define FEATURES_TIMESTAMP_COMPONENT  "[%h:%m %t] "

      જો ( ft_timestamp_flag && ft_component_flag )
      {
        features        = FEATURES_TIMESTAMP_COMPONENT;
        features_length = કદ ( FEATURES_TIMESTAMP_COMPONENT );
      }
      નહીંતર જો ( ft_timestamp_flag )
      {
        features        = FEATURES_TIMESTAMP;
        features_length = કદ ( FEATURES_TIMESTAMP );
      }
      નહીંતર જો ( ft_component_flag )
      {
        features        = FEATURES_COMPONENT;
        features_length = કદ ( FEATURES_COMPONENT );
      }

      જો ( ft_component_flag || ft_timestamp_flag )
      {
        ft_strncpy( features_buf, features, features_length );
        bufp += features_length - 1;
      }

      જો ( ft_component_flag )
      {
        size_t  tag_length = ft_strlen( *origin->tags );
        size_t  i;


        /* To vertically align tracing messages we compensate the */
        /* different FT_COMPONENT string lengths by inserting an  */
        /* appropriate amount of space characters.                */
        માટે ( i = 0;
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
                                (અચલ અક્ષર*)features_buf,
                                origin,
                                string,
                                dlg_default_output_styles,
                                true );

    જો ( ft_strrchr( string, '\n' ) )
      ft_have_newline_char = TRUE;
    નહીંતર
      ft_have_newline_char = FALSE;
  }


  /* documentation is in ftdebug.h */
  FT_BASE_DEF( શૂન્ય )
  ft_add_tag( અચલ અક્ષર*  tag )
  {
    ft_component = tag;

    dlg_add_tag( tag, ખાલી );
  }


  /* documentation is in ftdebug.h */
  FT_BASE_DEF( શૂન્ય )
  ft_remove_tag( અચલ અક્ષર*  tag )
  {
    dlg_remove_tag( tag, ખાલી );
  }


  /* documentation is in ftlogging.h */

  FT_EXPORT_DEF( શૂન્ય )
  FT_Trace_Set_Level( અચલ અક્ષર*  level )
  {
    ft_component_flag     = FALSE;
    ft_timestamp_flag     = FALSE;
    ft_custom_trace_level = level;

    ft_debug_init();
  }


  /* documentation is in ftlogging.h */

  FT_EXPORT_DEF( શૂન્ય )
  FT_Trace_Set_Default_Level( શૂન્ય )
  {
    ft_component_flag     = FALSE;
    ft_timestamp_flag     = FALSE;
    ft_custom_trace_level = ખાલી;

    ft_debug_init();
  }


  /**************************************************************************
   *
   * Functions to handle a custom log handler.
   *
   */

  /* documentation is in ftlogging.h */

  FT_EXPORT_DEF( શૂન્ય )
  FT_Set_Log_Handler( FT_Custom_Log_Handler  handler )
  {
    custom_output_handler = handler;
  }


  /* documentation is in ftlogging.h */

  FT_EXPORT_DEF( શૂન્ય )
  FT_Set_Default_Log_Handler( શૂન્ય )
  {
    custom_output_handler = ખાલી;
  }


  /* documentation is in ftdebug.h */
  FT_BASE_DEF( શૂન્ય )
  FT_Logging_Callback( અચલ અક્ષર*  fmt,
                       ... )
  {
    va_list  ap;


    va_start( ap, fmt );
    custom_output_handler( ft_component, fmt, ap );
    va_end( ap );
  }

#નહીંતર /* !FT_DEBUG_LOGGING */

  FT_EXPORT_DEF( શૂન્ય )
  FT_Trace_Set_Level( અચલ અક્ષર*  level )
  {
    FT_UNUSED( level );
  }


  FT_EXPORT_DEF( શૂન્ય )
  FT_Trace_Set_Default_Level( શૂન્ય )
  {
    /* nothing */
  }


  FT_EXPORT_DEF( શૂન્ય )
  FT_Set_Log_Handler( FT_Custom_Log_Handler  handler )
  {
    FT_UNUSED( handler );
  }


  FT_EXPORT_DEF( શૂન્ય )
  FT_Set_Default_Log_Handler( શૂન્ય )
  {
    /* nothing */
  }

#endif /* !FT_DEBUG_LOGGING */


/* END */
