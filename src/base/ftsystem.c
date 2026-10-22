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


#ضمن <ft2build.h>
#ضمن FT_CONFIG_CONFIG_H
#ضمن <freetype/internal/ftdebug.h>
#ضمن <freetype/internal/ftstream.h>
#ضمن <freetype/ftsystem.h>
#ضمن <freetype/fterrors.h>
#ضمن <freetype/fttypes.h>


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
  FT_CALLBACK_DEF( فراغ* )
  ft_alloc( FT_حافظه  حافظه,
            اوږد       اندازه )
  {
    FT_ناکارول_شوی( حافظه );

    أرجع ft_smalloc( (size_t)اندازه );
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
  FT_CALLBACK_DEF( فراغ* )
  ft_realloc( FT_حافظه  حافظه,
              اوږد       cur_اندازه,
              اوږد       new_اندازه,
              فراغ*      بلاک )
  {
    FT_ناکارول_شوی( حافظه );
    FT_ناکارول_شوی( cur_اندازه );

    أرجع ft_srealloc( بلاک, (size_t)new_اندازه );
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
  FT_CALLBACK_DEF( فراغ )
  ft_free( FT_حافظه  حافظه,
           فراغ*      بلاک )
  {
    FT_ناکارول_شوی( حافظه );

    ft_sfree( بلاک );
  }


  /**************************************************************************
   *
   *                    RESOURCE MANAGEMENT INTERFACE
   *
   */

#که_نه_تعریف_شوی FT_CONFIG_OPTION_DISABLE_بهیر_SUPPORT

  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#تعریف_لغوه  FT_COMPONENT
#تعریف_کړه FT_COMPONENT  io

  /* We use the macro STREAM_FILE for convenience to extract the       */
  /* system-specific stream handle from a given FreeType stream object */
#تعریف_کړه بهیر_FILE( بهیر )  ( (FT_FILE*)بهیر->descriptor.اشاره_ګر )


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
  FT_CALLBACK_DEF( فراغ )
  ft_ansi_بهیر_close( FT_بهیر  بهیر )
  {
    ft_fclose( بهیر_FILE( بهیر ) );

    بهیر->descriptor.اشاره_ګر = عدم;
    بهیر->اندازه               = 0;
    بهیر->بنسټ               = عدم;
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
  FT_CALLBACK_DEF( بینښه اوږد )
  ft_ansi_بهیر_io( FT_بهیر       بهیر,
                     بینښه اوږد   دننه_واټن,
                     بینښه حرف*  بفر,
                     بینښه اوږد   شمېر )
  {
    FT_FILE*  file;


    إذا ( دننه_واټن > بهیر->اندازه && !شمېر )
      أرجع 1;

    file = بهیر_FILE( بهیر );

    إذا ( بهیر->pos != دننه_واټن )
      ft_fseek( file, (اوږد)دننه_واټن, SEEK_سېټ );

    /* Avoid calling `fread` with `buffer=NULL` and `count=0`, */
    /* which is undefined behaviour.                           */
    إذا ( !شمېر )
      أرجع 0;

    أرجع (بینښه اوږد)ft_fread( بفر, 1, شمېر, file );
  }


  /* documentation is in ftstream.h */

  FT_بنسټ_DEF( FT_Error )
  FT_بهیر_Open( FT_بهیر    بهیر,
                  ثابت حرف*  filepathname )
  {
    FT_FILE*  file;


    إذا ( !بهیر )
      أرجع FT_THROW( Invalid_بهیر_Handle );

    بهیر->descriptor.اشاره_ګر = عدم;
    بهیر->pathname.اشاره_ګر   = (حرف*)filepathname;
    بهیر->بنسټ               = عدم;
    بهیر->pos                = 0;
    بهیر->اقرأ               = عدم;
    بهیر->close              = عدم;

    file = ft_fopen( filepathname, "rb" );
    إذا ( !file )
    {
      FT_ERROR(( "FT_Stream_Open:"
                 " could not open `%s'\n", filepathname ));

      أرجع FT_THROW( Cannot_Open_Resource );
    }

    ft_fseek( file, 0, SEEK_پای_ټکی );
    بهیر->اندازه = (بینښه اوږد)ft_ftell( file );
    إذا ( !بهیر->اندازه )
    {
      FT_ERROR(( "FT_Stream_Open:" ));
      FT_ERROR(( " opened `%s' but zero-sized\n", filepathname ));
      ft_fclose( file );
      أرجع FT_THROW( Cannot_Open_بهیر );
    }
    ft_fseek( file, 0, SEEK_سېټ );

    بهیر->descriptor.اشاره_ګر = file;
    بهیر->اقرأ  = ft_ansi_بهیر_io;
    بهیر->close = ft_ansi_بهیر_close;

    FT_TRACE1(( "FT_Stream_Open:" ));
    FT_TRACE1(( " opened `%s' (%lu bytes) successfully\n",
                filepathname, بهیر->اندازه ));

    أرجع FT_Err_Ok;
  }

#د_که_پای /* !FT_CONFIG_OPTION_DISABLE_STREAM_SUPPORT */

#که_تعریف_شوی FT_DEBUG_حافظه

  خارجي FT_Int
  ft_mem_debug_init( FT_حافظه  حافظه );

  خارجي فراغ
  ft_mem_debug_بشپړ( FT_حافظه  حافظه );

#د_که_پای


  /* documentation is in ftobjs.h */

  FT_بنسټ_DEF( FT_حافظه )
  FT_New_حافظه( فراغ )
  {
    FT_حافظه  حافظه;


    حافظه = (FT_حافظه)ft_smalloc( حجم ( *حافظه ) );
    إذا ( حافظه )
    {
      حافظه->user    = عدم;
      حافظه->alloc   = ft_alloc;
      حافظه->realloc = ft_realloc;
      حافظه->حرر    = ft_free;
#که_تعریف_شوی FT_DEBUG_حافظه
      ft_mem_debug_init( حافظه );
#د_که_پای
    }

    أرجع حافظه;
  }


  /* documentation is in ftobjs.h */

  FT_بنسټ_DEF( فراغ )
  FT_بشپړ_حافظه( FT_حافظه  حافظه )
  {
#که_تعریف_شوی FT_DEBUG_حافظه
    ft_mem_debug_بشپړ( حافظه );
#د_که_پای
    ft_sfree( حافظه );
  }


/*پای_ټکی*/
