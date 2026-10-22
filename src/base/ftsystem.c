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


#ଅନ୍ତର୍ଭାଵ <ft2build.h>
#ଅନ୍ତର୍ଭାଵ FT_CONFIG_CONFIG_H
#ଅନ୍ତର୍ଭାଵ <freetype/internal/ftdebug.h>
#ଅନ୍ତର୍ଭାଵ <freetype/internal/ftstream.h>
#ଅନ୍ତର୍ଭାଵ <freetype/ftsystem.h>
#ଅନ୍ତର୍ଭାଵ <freetype/fterrors.h>
#ଅନ୍ତର୍ଭାଵ <freetype/fttypes.h>


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
  FT_CALLBACK_DEF( ଶୂନ୍ୟ* )
  ft_alloc( FT_Memory  memory,
            long       size )
  {
    FT_UNUSED( memory );

    ଫେରନ୍ତୁ ft_smalloc( (size_t)size );
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
  FT_CALLBACK_DEF( ଶୂନ୍ୟ* )
  ft_realloc( FT_Memory  memory,
              long       cur_size,
              long       new_size,
              ଶୂନ୍ୟ*      block )
  {
    FT_UNUSED( memory );
    FT_UNUSED( cur_size );

    ଫେରନ୍ତୁ ft_srealloc( block, (size_t)new_size );
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
  FT_CALLBACK_DEF( ଶୂନ୍ୟ )
  ft_free( FT_Memory  memory,
           ଶୂନ୍ୟ*      block )
  {
    FT_UNUSED( memory );

    ft_sfree( block );
  }


  /**************************************************************************
   *
   *                    RESOURCE MANAGEMENT INTERFACE
   *
   */

#ifndef FT_CONFIG_OPTION_DISABLE_STREAM_SUPPORT

  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#undef  FT_COMPONENT
#define FT_COMPONENT  io

  /* We use the macro STREAM_FILE for convenience to extract the       */
  /* system-specific stream handle from a given FreeType stream object */
#define STREAM_FILE( stream )  ( (FT_FILE*)stream->descriptor.pointer )


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
  FT_CALLBACK_DEF( ଶୂନ୍ୟ )
  ft_ansi_stream_close( FT_Stream  stream )
  {
    ft_fclose( STREAM_FILE( stream ) );

    stream->descriptor.pointer = ଖାଲି;
    stream->size               = 0;
    stream->base               = ଖାଲି;
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
  FT_CALLBACK_DEF( unsigned long )
  ft_ansi_stream_io( FT_Stream       stream,
                     unsigned long   offset,
                     unsigned ଅକ୍ଷର*  buffer,
                     unsigned long   ଗଣନା_2 )
  {
    FT_FILE*  file;


    ଯଦି ( offset > stream->size && !ଗଣନା_2 )
      ଫେରନ୍ତୁ 1;

    file = STREAM_FILE( stream );

    ଯଦି ( stream->pos != offset )
      ft_fseek( file, (long)offset, SEEK_SET );

    /* Avoid calling `fread` with `buffer=NULL` and `count=0`, */
    /* which is undefined behaviour.                           */
    ଯଦି ( !ଗଣନା_2 )
      ଫେରନ୍ତୁ 0;

    ଫେରନ୍ତୁ (unsigned long)ft_fread( buffer, 1, ଗଣନା_2, file );
  }


  /* documentation is in ftstream.h */

  FT_BASE_DEF( FT_Error )
  FT_Stream_Open( FT_Stream    stream,
                  ଅଚଳ ଅକ୍ଷର*  filepathname )
  {
    FT_FILE*  file;


    ଯଦି ( !stream )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Stream_Handle );

    stream->descriptor.pointer = ଖାଲି;
    stream->pathname.pointer   = (ଅକ୍ଷର*)filepathname;
    stream->base               = ଖାଲି;
    stream->pos                = 0;
    stream->ପଠ               = ଖାଲି;
    stream->close              = ଖାଲି;

    file = ft_fopen( filepathname, "rb" );
    ଯଦି ( !file )
    {
      FT_ERROR(( "FT_Stream_Open:"
                 " could not open `%s'\n", filepathname ));

      ଫେରନ୍ତୁ FT_THROW( Cannot_Open_Resource );
    }

    ft_fseek( file, 0, SEEK_END );
    stream->size = (unsigned long)ft_ftell( file );
    ଯଦି ( !stream->size )
    {
      FT_ERROR(( "FT_Stream_Open:" ));
      FT_ERROR(( " opened `%s' but zero-sized\n", filepathname ));
      ft_fclose( file );
      ଫେରନ୍ତୁ FT_THROW( Cannot_Open_Stream );
    }
    ft_fseek( file, 0, SEEK_SET );

    stream->descriptor.pointer = file;
    stream->ପଠ  = ft_ansi_stream_io;
    stream->close = ft_ansi_stream_close;

    FT_TRACE1(( "FT_Stream_Open:" ));
    FT_TRACE1(( " opened `%s' (%lu bytes) successfully\n",
                filepathname, stream->size ));

    ଫେରନ୍ତୁ FT_Err_Ok;
  }

#endif /* !FT_CONFIG_OPTION_DISABLE_STREAM_SUPPORT */

#ifdef FT_DEBUG_MEMORY

  ବାହ୍ୟ FT_Int
  ft_mem_debug_init( FT_Memory  memory );

  ବାହ୍ୟ ଶୂନ୍ୟ
  ft_mem_debug_done( FT_Memory  memory );

#endif


  /* documentation is in ftobjs.h */

  FT_BASE_DEF( FT_Memory )
  FT_New_Memory( ଶୂନ୍ୟ )
  {
    FT_Memory  memory;


    memory = (FT_Memory)ft_smalloc( ଆକାର ( *memory ) );
    ଯଦି ( memory )
    {
      memory->user    = ଖାଲି;
      memory->alloc   = ft_alloc;
      memory->realloc = ft_realloc;
      memory->ମୁକ୍ତ    = ft_free;
#ifdef FT_DEBUG_MEMORY
      ft_mem_debug_init( memory );
#endif
    }

    ଫେରନ୍ତୁ memory;
  }


  /* documentation is in ftobjs.h */

  FT_BASE_DEF( ଶୂନ୍ୟ )
  FT_Done_Memory( FT_Memory  memory )
  {
#ifdef FT_DEBUG_MEMORY
    ft_mem_debug_done( memory );
#endif
    ft_sfree( memory );
  }


/* END */
