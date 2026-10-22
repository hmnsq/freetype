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
            دراز       اندازه )
  {
    FT_استفادهنشده( حافظه );

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
              دراز       cur_اندازه,
              دراز       new_اندازه,
              فراغ*      بلوک )
  {
    FT_استفادهنشده( حافظه );
    FT_استفادهنشده( cur_اندازه );

    أرجع ft_srealloc( بلوک, (size_t)new_اندازه );
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
           فراغ*      بلوک )
  {
    FT_استفادهنشده( حافظه );

    ft_sfree( بلوک );
  }


  /**************************************************************************
   *
   *                    RESOURCE MANAGEMENT INTERFACE
   *
   */

#اگر_تعریف_نشده FT_CONFIG_OPTION_DISABLE_جریان_SUPPORT

  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#لغو_تعریف  FT_COMPONENT
#تعریف_کن FT_COMPONENT  io

  /* We use the macro STREAM_FILE for convenience to extract the       */
  /* system-specific stream handle from a given FreeType stream object */
#تعریف_کن جریان_FILE( جریان )  ( (FT_FILE*)جریان->descriptor.اشاره_گر )


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
  ft_ansi_جریان_close( FT_جریان  جریان )
  {
    ft_fclose( جریان_FILE( جریان ) );

    جریان->descriptor.اشاره_گر = عدم;
    جریان->اندازه               = 0;
    جریان->پایه               = عدم;
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
  FT_CALLBACK_DEF( بیعلامت دراز )
  ft_ansi_جریان_io( FT_جریان       جریان,
                     بیعلامت دراز   جابه_جایی,
                     بیعلامت حرف*  میانگیر,
                     بیعلامت دراز   شمار )
  {
    FT_FILE*  file;


    إذا ( جابه_جایی > جریان->اندازه && !شمار )
      أرجع 1;

    file = جریان_FILE( جریان );

    إذا ( جریان->pos != جابه_جایی )
      ft_fseek( file, (دراز)جابه_جایی, SEEK_مجموعه );

    /* Avoid calling `fread` with `buffer=NULL` and `count=0`, */
    /* which is undefined behaviour.                           */
    إذا ( !شمار )
      أرجع 0;

    أرجع (بیعلامت دراز)ft_fread( میانگیر, 1, شمار, file );
  }


  /* documentation is in ftstream.h */

  FT_پایه_DEF( FT_Error )
  FT_جریان_Open( FT_جریان    جریان,
                  ثابت حرف*  filepathname )
  {
    FT_FILE*  file;


    إذا ( !جریان )
      أرجع FT_THROW( Invalid_جریان_Handle );

    جریان->descriptor.اشاره_گر = عدم;
    جریان->pathname.اشاره_گر   = (حرف*)filepathname;
    جریان->پایه               = عدم;
    جریان->pos                = 0;
    جریان->اقرأ               = عدم;
    جریان->close              = عدم;

    file = ft_fopen( filepathname, "rb" );
    إذا ( !file )
    {
      FT_ERROR(( "FT_Stream_Open:"
                 " could not open `%s'\n", filepathname ));

      أرجع FT_THROW( Cannot_Open_Resource );
    }

    ft_fseek( file, 0, SEEK_انتها );
    جریان->اندازه = (بیعلامت دراز)ft_ftell( file );
    إذا ( !جریان->اندازه )
    {
      FT_ERROR(( "FT_Stream_Open:" ));
      FT_ERROR(( " opened `%s' but zero-sized\n", filepathname ));
      ft_fclose( file );
      أرجع FT_THROW( Cannot_Open_جریان );
    }
    ft_fseek( file, 0, SEEK_مجموعه );

    جریان->descriptor.اشاره_گر = file;
    جریان->اقرأ  = ft_ansi_جریان_io;
    جریان->close = ft_ansi_جریان_close;

    FT_TRACE1(( "FT_Stream_Open:" ));
    FT_TRACE1(( " opened `%s' (%lu bytes) successfully\n",
                filepathname, جریان->اندازه ));

    أرجع FT_Err_Ok;
  }

#پایان_اگر /* !FT_CONFIG_OPTION_DISABLE_STREAM_SUPPORT */

#اگر_تعریف_شده FT_DEBUG_حافظه

  خارجي FT_Int
  ft_mem_debug_init( FT_حافظه  حافظه );

  خارجي فراغ
  ft_mem_debug_انجامشده( FT_حافظه  حافظه );

#پایان_اگر


  /* documentation is in ftobjs.h */

  FT_پایه_DEF( FT_حافظه )
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
#اگر_تعریف_شده FT_DEBUG_حافظه
      ft_mem_debug_init( حافظه );
#پایان_اگر
    }

    أرجع حافظه;
  }


  /* documentation is in ftobjs.h */

  FT_پایه_DEF( فراغ )
  FT_انجامشده_حافظه( FT_حافظه  حافظه )
  {
#اگر_تعریف_شده FT_DEBUG_حافظه
    ft_mem_debug_انجامشده( حافظه );
#پایان_اگر
    ft_sfree( حافظه );
  }


/*انتها*/
