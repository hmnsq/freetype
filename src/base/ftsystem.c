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
  ft_alloc( FT_يادگيري  يادگيري,
            ڊگهو       سائيز )
  {
    FT_اڻ_استعمال_ٿيل( يادگيري );

    أرجع ft_smalloc( (size_t)سائيز );
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
  ft_realloc( FT_يادگيري  يادگيري,
              ڊگهو       cur_سائيز,
              ڊگهو       new_سائيز,
              فراغ*      بلاڪ )
  {
    FT_اڻ_استعمال_ٿيل( يادگيري );
    FT_اڻ_استعمال_ٿيل( cur_سائيز );

    أرجع ft_srealloc( بلاڪ, (size_t)new_سائيز );
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
  ft_free( FT_يادگيري  يادگيري,
           فراغ*      بلاڪ )
  {
    FT_اڻ_استعمال_ٿيل( يادگيري );

    ft_sfree( بلاڪ );
  }


  /**************************************************************************
   *
   *                    RESOURCE MANAGEMENT INTERFACE
   *
   */

#جيڪڏهن_وصف_نه_ٿيل FT_CONFIG_OPTION_DISABLE_وهڪرو_SUPPORT

  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#وصف_رد  FT_COMPONENT
#وصف_ڪريو FT_COMPONENT  io

  /* We use the macro STREAM_FILE for convenience to extract the       */
  /* system-specific stream handle from a given FreeType stream object */
#وصف_ڪريو وهڪرو_FILE( وهڪرو )  ( (FT_FILE*)وهڪرو->descriptor.اشارو )


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
  ft_ansi_وهڪرو_close( FT_وهڪرو  وهڪرو )
  {
    ft_fclose( وهڪرو_FILE( وهڪرو ) );

    وهڪرو->descriptor.اشارو = عدم;
    وهڪرو->سائيز               = 0;
    وهڪرو->بنياد               = عدم;
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
  FT_CALLBACK_DEF( بنانشان ڊگهو )
  ft_ansi_وهڪرو_io( FT_وهڪرو       وهڪرو,
                     بنانشان ڊگهو   وٿي,
                     بنانشان حرف*  بفر,
                     بنانشان ڊگهو   ڳڻپ )
  {
    FT_FILE*  file;


    إذا ( وٿي > وهڪرو->سائيز && !ڳڻپ )
      أرجع 1;

    file = وهڪرو_FILE( وهڪرو );

    إذا ( وهڪرو->pos != وٿي )
      ft_fseek( file, (ڊگهو)وٿي, SEEK_سيٽ );

    /* Avoid calling `fread` with `buffer=NULL` and `count=0`, */
    /* which is undefined behaviour.                           */
    إذا ( !ڳڻپ )
      أرجع 0;

    أرجع (بنانشان ڊگهو)ft_fread( بفر, 1, ڳڻپ, file );
  }


  /* documentation is in ftstream.h */

  FT_بنياد_DEF( FT_Error )
  FT_وهڪرو_Open( FT_وهڪرو    وهڪرو,
                  ثابت حرف*  filepathname )
  {
    FT_FILE*  file;


    إذا ( !وهڪرو )
      أرجع FT_THROW( Invalid_وهڪرو_Handle );

    وهڪرو->descriptor.اشارو = عدم;
    وهڪرو->pathname.اشارو   = (حرف*)filepathname;
    وهڪرو->بنياد               = عدم;
    وهڪرو->pos                = 0;
    وهڪرو->اقرأ               = عدم;
    وهڪرو->close              = عدم;

    file = ft_fopen( filepathname, "rb" );
    إذا ( !file )
    {
      FT_ERROR(( "FT_Stream_Open:"
                 " could not open `%s'\n", filepathname ));

      أرجع FT_THROW( Cannot_Open_Resource );
    }

    ft_fseek( file, 0, SEEK_پڇاڙي );
    وهڪرو->سائيز = (بنانشان ڊگهو)ft_ftell( file );
    إذا ( !وهڪرو->سائيز )
    {
      FT_ERROR(( "FT_Stream_Open:" ));
      FT_ERROR(( " opened `%s' but zero-sized\n", filepathname ));
      ft_fclose( file );
      أرجع FT_THROW( Cannot_Open_وهڪرو );
    }
    ft_fseek( file, 0, SEEK_سيٽ );

    وهڪرو->descriptor.اشارو = file;
    وهڪرو->اقرأ  = ft_ansi_وهڪرو_io;
    وهڪرو->close = ft_ansi_وهڪرو_close;

    FT_TRACE1(( "FT_Stream_Open:" ));
    FT_TRACE1(( " opened `%s' (%lu bytes) successfully\n",
                filepathname, وهڪرو->سائيز ));

    أرجع FT_Err_Ok;
  }

#جيڪڏهن_پورو /* !FT_CONFIG_OPTION_DISABLE_STREAM_SUPPORT */

#جيڪڏهن_وصف_ٿيل FT_DEBUG_يادگيري

  خارجي FT_Int
  ft_mem_debug_init( FT_يادگيري  يادگيري );

  خارجي فراغ
  ft_mem_debug_پورو( FT_يادگيري  يادگيري );

#جيڪڏهن_پورو


  /* documentation is in ftobjs.h */

  FT_بنياد_DEF( FT_يادگيري )
  FT_New_يادگيري( فراغ )
  {
    FT_يادگيري  يادگيري;


    يادگيري = (FT_يادگيري)ft_smalloc( حجم ( *يادگيري ) );
    إذا ( يادگيري )
    {
      يادگيري->user    = عدم;
      يادگيري->alloc   = ft_alloc;
      يادگيري->realloc = ft_realloc;
      يادگيري->حرر    = ft_free;
#جيڪڏهن_وصف_ٿيل FT_DEBUG_يادگيري
      ft_mem_debug_init( يادگيري );
#جيڪڏهن_پورو
    }

    أرجع يادگيري;
  }


  /* documentation is in ftobjs.h */

  FT_بنياد_DEF( فراغ )
  FT_پورو_يادگيري( FT_يادگيري  يادگيري )
  {
#جيڪڏهن_وصف_ٿيل FT_DEBUG_يادگيري
    ft_mem_debug_پورو( يادگيري );
#جيڪڏهن_پورو
    ft_sfree( يادگيري );
  }


/*پڇاڙي*/
