/****************************************************************************
 *
 * ftsystem.c
 *
 *   ANSI-specific FreeType low-level system interface (body).
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
   * This file contains the default interface used by FreeType to access
   * low-level, i.e. memory management, i/o access as well as thread
   * synchronisation.  It can be replaced by user-specific routines if
   * necessary.
   *
   */


#ਅਨ੍ਤਰ੍ਭਾਵ <ft2build.h>
#ਅਨ੍ਤਰ੍ਭਾਵ FT_CONFIG_CONFIG_H
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftdebug.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftstream.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/ftsystem.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/fterrors.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/fttypes.h>


  /**************************************************************************
   *
   *                      MEMORY MANAGEMENT INTERFACE
   *
   */

  /**************************************************************************
   *
   * It is not necessary to do any error checking for the
   * allocation-related functions.  This will be done by the higher level
   * routines like ft_mem_alloc() or ft_mem_realloc().
   *
   */


  /**************************************************************************
   *
   * @Function:
   *   ft_alloc
   *
   * @Description:
   *   The memory allocation function.
   *
   * @Input:
   *   memory ::
   *     A pointer to the memory object.
   *
   *   size ::
   *     The requested size in bytes.
   *
   * @Return:
   *   The address of newly allocated block.
   */
  FT_CALLBACK_DEF( ਖਾਲੀ* )
  ft_alloc( FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ,
            ਲੰਮਾ       ਮਾਪ )
  {
    FT_ਅਣਵਰਤਿਆ( ਸਮ੍ਰਿਤੀ );

    ਵਾਪਸ ft_smalloc( (size_t)ਮਾਪ );
  }


  /**************************************************************************
   *
   * @Function:
   *   ft_realloc
   *
   * @Description:
   *   The memory reallocation function.
   *
   * @Input:
   *   memory ::
   *     A pointer to the memory object.
   *
   *   cur_size ::
   *     The current size of the allocated memory block.
   *
   *   new_size ::
   *     The newly requested size in bytes.
   *
   *   block ::
   *     The current address of the block in memory.
   *
   * @Return:
   *   The address of the reallocated memory block.
   */
  FT_CALLBACK_DEF( ਖਾਲੀ* )
  ft_realloc( FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ,
              ਲੰਮਾ       cur_ਮਾਪ,
              ਲੰਮਾ       new_ਮਾਪ,
              ਖਾਲੀ*      ਖੰਡ )
  {
    FT_ਅਣਵਰਤਿਆ( ਸਮ੍ਰਿਤੀ );
    FT_ਅਣਵਰਤਿਆ( cur_ਮਾਪ );

    ਵਾਪਸ ft_srealloc( ਖੰਡ, (size_t)new_ਮਾਪ );
  }


  /**************************************************************************
   *
   * @Function:
   *   ft_free
   *
   * @Description:
   *   The memory release function.
   *
   * @Input:
   *   memory ::
   *     A pointer to the memory object.
   *
   *   block ::
   *     The address of block in memory to be freed.
   */
  FT_CALLBACK_DEF( ਖਾਲੀ )
  ft_free( FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ,
           ਖਾਲੀ*      ਖੰਡ )
  {
    FT_ਅਣਵਰਤਿਆ( ਸਮ੍ਰਿਤੀ );

    ft_sfree( ਖੰਡ );
  }


  /**************************************************************************
   *
   *                    RESOURCE MANAGEMENT INTERFACE
   *
   */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ FT_CONFIG_OPTION_DISABLE_ਧਾਰਾ_SUPPORT

  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ  FT_COMPONENT
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FT_COMPONENT  io

  /* We use the macro STREAM_FILE for convenience to extract the       */
  /* system-specific stream handle from a given FreeType stream object */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਧਾਰਾ_FILE( ਧਾਰਾ )  ( (FT_FILE*)ਧਾਰਾ->descriptor.ਸੂਚਕ )


  /**************************************************************************
   *
   * @Function:
   *   ft_ansi_stream_close
   *
   * @Description:
   *   The function to close a stream.
   *
   * @Input:
   *   stream ::
   *     A pointer to the stream object.
   */
  FT_CALLBACK_DEF( ਖਾਲੀ )
  ft_ansi_ਧਾਰਾ_close( FT_ਧਾਰਾ  ਧਾਰਾ )
  {
    ft_fclose( ਧਾਰਾ_FILE( ਧਾਰਾ ) );

    ਧਾਰਾ->descriptor.ਸੂਚਕ = ਨਿਰਾ;
    ਧਾਰਾ->ਮਾਪ               = 0;
    ਧਾਰਾ->ਅਧਾਰ               = ਨਿਰਾ;
  }


  /**************************************************************************
   *
   * @Function:
   *   ft_ansi_stream_io
   *
   * @Description:
   *   The function to open a stream.
   *
   * @Input:
   *   stream ::
   *     A pointer to the stream object.
   *
   *   offset ::
   *     The position in the data stream to start reading.
   *
   *   buffer ::
   *     The address of buffer to store the read data.
   *
   *   count ::
   *     The number of bytes to read from the stream.
   *
   * @Return:
   *   The number of bytes actually read.  If `count' is zero (that is,
   *   the function is used for seeking), a non-zero return value
   *   indicates an error.
   */
  FT_CALLBACK_DEF( ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ )
  ft_ansi_ਧਾਰਾ_io( FT_ਧਾਰਾ       ਧਾਰਾ,
                     ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ   ਵਿੱਥ,
                     ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ*  ਬਫਰ,
                     ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ   ਗਿਣਤੀ_2 )
  {
    FT_FILE*  file;


    ਜੇ ( ਵਿੱਥ > ਧਾਰਾ->ਮਾਪ && !ਗਿਣਤੀ_2 )
      ਵਾਪਸ 1;

    file = ਧਾਰਾ_FILE( ਧਾਰਾ );

    ਜੇ ( ਧਾਰਾ->pos != ਵਿੱਥ )
      ft_fseek( file, (ਲੰਮਾ)ਵਿੱਥ, SEEK_ਸੈੱਟ );

    /* Avoid calling `fread` with `buffer=NULL` and `count=0`, */
    /* which is undefined behaviour.                           */
    ਜੇ ( !ਗਿਣਤੀ_2 )
      ਵਾਪਸ 0;

    ਵਾਪਸ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ)ft_fread( ਬਫਰ, 1, ਗਿਣਤੀ_2, file );
  }


  /* documentation is in ftstream.h */

  FT_ਅਧਾਰ_DEF( FT_Error )
  FT_ਧਾਰਾ_Open( FT_ਧਾਰਾ    ਧਾਰਾ,
                  ਅਚਲ ਅੱਖਰ*  filepathname )
  {
    FT_FILE*  file;


    ਜੇ ( !ਧਾਰਾ )
      ਵਾਪਸ FT_THROW( Invalid_ਧਾਰਾ_Handle );

    ਧਾਰਾ->descriptor.ਸੂਚਕ = ਨਿਰਾ;
    ਧਾਰਾ->pathname.ਸੂਚਕ   = (ਅੱਖਰ*)filepathname;
    ਧਾਰਾ->ਅਧਾਰ               = ਨਿਰਾ;
    ਧਾਰਾ->pos                = 0;
    ਧਾਰਾ->ਪਠ               = ਨਿਰਾ;
    ਧਾਰਾ->close              = ਨਿਰਾ;

    file = ft_fopen( filepathname, "rb" );
    ਜੇ ( !file )
    {
      FT_ERROR(( "FT_Stream_Open:"
                 " could not open `%s'\n", filepathname ));

      ਵਾਪਸ FT_THROW( Cannot_Open_Resource );
    }

    ft_fseek( file, 0, SEEK_ਸਿਰਾ );
    ਧਾਰਾ->ਮਾਪ = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ)ft_ftell( file );
    ਜੇ ( !ਧਾਰਾ->ਮਾਪ )
    {
      FT_ERROR(( "FT_Stream_Open:" ));
      FT_ERROR(( " opened `%s' but zero-sized\n", filepathname ));
      ft_fclose( file );
      ਵਾਪਸ FT_THROW( Cannot_Open_ਧਾਰਾ );
    }
    ft_fseek( file, 0, SEEK_ਸੈੱਟ );

    ਧਾਰਾ->descriptor.ਸੂਚਕ = file;
    ਧਾਰਾ->ਪਠ  = ft_ansi_ਧਾਰਾ_io;
    ਧਾਰਾ->close = ft_ansi_ਧਾਰਾ_close;

    FT_TRACE1(( "FT_Stream_Open:" ));
    FT_TRACE1(( " opened `%s' (%lu bytes) successfully\n",
                filepathname, ਧਾਰਾ->ਮਾਪ ));

    ਵਾਪਸ FT_Err_Ok;
  }

#ਜੇ_ਅੰਤ /* !FT_CONFIG_OPTION_DISABLE_STREAM_SUPPORT */

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ FT_DEBUG_ਸਮ੍ਰਿਤੀ

  ਬਾਹਰੀ FT_Int
  ft_mem_debug_init( FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ );

  ਬਾਹਰੀ ਖਾਲੀ
  ft_mem_debug_ਪੂਰਾ( FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ );

#ਜੇ_ਅੰਤ


  /* documentation is in ftobjs.h */

  FT_ਅਧਾਰ_DEF( FT_ਸਮ੍ਰਿਤੀ )
  FT_New_ਸਮ੍ਰਿਤੀ( ਖਾਲੀ )
  {
    FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ;


    ਸਮ੍ਰਿਤੀ = (FT_ਸਮ੍ਰਿਤੀ)ft_smalloc( ਆਕਾਰ ( *ਸਮ੍ਰਿਤੀ ) );
    ਜੇ ( ਸਮ੍ਰਿਤੀ )
    {
      ਸਮ੍ਰਿਤੀ->user    = ਨਿਰਾ;
      ਸਮ੍ਰਿਤੀ->alloc   = ft_alloc;
      ਸਮ੍ਰਿਤੀ->realloc = ft_realloc;
      ਸਮ੍ਰਿਤੀ->ਮੁਕਤ    = ft_free;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ FT_DEBUG_ਸਮ੍ਰਿਤੀ
      ft_mem_debug_init( ਸਮ੍ਰਿਤੀ );
#ਜੇ_ਅੰਤ
    }

    ਵਾਪਸ ਸਮ੍ਰਿਤੀ;
  }


  /* documentation is in ftobjs.h */

  FT_ਅਧਾਰ_DEF( ਖਾਲੀ )
  FT_ਪੂਰਾ_ਸਮ੍ਰਿਤੀ( FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ )
  {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ FT_DEBUG_ਸਮ੍ਰਿਤੀ
    ft_mem_debug_ਪੂਰਾ( ਸਮ੍ਰਿਤੀ );
#ਜੇ_ਅੰਤ
    ft_sfree( ਸਮ੍ਰਿਤੀ );
  }


/*ਸਿਰਾ*/
