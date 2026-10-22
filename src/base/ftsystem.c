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


#अन्तर्भाव <ft2build.h>
#अन्तर्भाव FT_CONFIG_CONFIG_H
#अन्तर्भाव <freetype/internal/ftdebug.h>
#अन्तर्भाव <freetype/internal/ftstream.h>
#अन्तर्भाव <freetype/ftsystem.h>
#अन्तर्भाव <freetype/fterrors.h>
#अन्तर्भाव <freetype/fttypes.h>


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
  FT_CALLBACK_DEF( शून्य* )
  ft_alloc( FT_Memory  memory,
            long       size )
  {
    FT_UNUSED( memory );

    प्रत्यागम ft_smalloc( (size_t)size );
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
  FT_CALLBACK_DEF( शून्य* )
  ft_realloc( FT_Memory  memory,
              long       cur_size,
              long       new_size,
              शून्य*      block )
  {
    FT_UNUSED( memory );
    FT_UNUSED( cur_size );

    प्रत्यागम ft_srealloc( block, (size_t)new_size );
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
  FT_CALLBACK_DEF( शून्य )
  ft_free( FT_Memory  memory,
           शून्य*      block )
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
  FT_CALLBACK_DEF( शून्य )
  ft_ansi_stream_close( FT_Stream  stream )
  {
    ft_fclose( STREAM_FILE( stream ) );

    stream->descriptor.pointer = रिक्त;
    stream->size               = 0;
    stream->base               = रिक्त;
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
                     unsigned वर्ण*  buffer,
                     unsigned long   गणनम् )
  {
    FT_FILE*  file;


    जइ ( offset > stream->size && !गणनम् )
      प्रत्यागम 1;

    file = STREAM_FILE( stream );

    जइ ( stream->pos != offset )
      ft_fseek( file, (long)offset, SEEK_SET );

    /* Avoid calling `fread` with `buffer=NULL` and `count=0`, */
    /* which is undefined behaviour.                           */
    जइ ( !गणनम् )
      प्रत्यागम 0;

    प्रत्यागम (unsigned long)ft_fread( buffer, 1, गणनम्, file );
  }


  /* documentation is in ftstream.h */

  FT_BASE_DEF( FT_Error )
  FT_Stream_Open( FT_Stream    stream,
                  ध्रुव वर्ण*  filepathname )
  {
    FT_FILE*  file;


    जइ ( !stream )
      प्रत्यागम FT_THROW( Invalid_Stream_Handle );

    stream->descriptor.pointer = रिक्त;
    stream->pathname.pointer   = (वर्ण*)filepathname;
    stream->base               = रिक्त;
    stream->pos                = 0;
    stream->पठ               = रिक्त;
    stream->close              = रिक्त;

    file = ft_fopen( filepathname, "rb" );
    जइ ( !file )
    {
      FT_ERROR(( "FT_Stream_Open:"
                 " could not open `%s'\n", filepathname ));

      प्रत्यागम FT_THROW( Cannot_Open_Resource );
    }

    ft_fseek( file, 0, SEEK_END );
    stream->size = (unsigned long)ft_ftell( file );
    जइ ( !stream->size )
    {
      FT_ERROR(( "FT_Stream_Open:" ));
      FT_ERROR(( " opened `%s' but zero-sized\n", filepathname ));
      ft_fclose( file );
      प्रत्यागम FT_THROW( Cannot_Open_Stream );
    }
    ft_fseek( file, 0, SEEK_SET );

    stream->descriptor.pointer = file;
    stream->पठ  = ft_ansi_stream_io;
    stream->close = ft_ansi_stream_close;

    FT_TRACE1(( "FT_Stream_Open:" ));
    FT_TRACE1(( " opened `%s' (%lu bytes) successfully\n",
                filepathname, stream->size ));

    प्रत्यागम FT_Err_Ok;
  }

#endif /* !FT_CONFIG_OPTION_DISABLE_STREAM_SUPPORT */

#ifdef FT_DEBUG_MEMORY

  बाह्य FT_Int
  ft_mem_debug_init( FT_Memory  memory );

  बाह्य शून्य
  ft_mem_debug_done( FT_Memory  memory );

#endif


  /* documentation is in ftobjs.h */

  FT_BASE_DEF( FT_Memory )
  FT_New_Memory( शून्य )
  {
    FT_Memory  memory;


    memory = (FT_Memory)ft_smalloc( परिमाण ( *memory ) );
    जइ ( memory )
    {
      memory->user    = रिक्त;
      memory->alloc   = ft_alloc;
      memory->realloc = ft_realloc;
      memory->मुक्त    = ft_free;
#ifdef FT_DEBUG_MEMORY
      ft_mem_debug_init( memory );
#endif
    }

    प्रत्यागम memory;
  }


  /* documentation is in ftobjs.h */

  FT_BASE_DEF( शून्य )
  FT_Done_Memory( FT_Memory  memory )
  {
#ifdef FT_DEBUG_MEMORY
    ft_mem_debug_done( memory );
#endif
    ft_sfree( memory );
  }


/* END */
