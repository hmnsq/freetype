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
            दीर्घ       size )
  {
    FT_UNUSED( memory );

    परत ft_smalloc( (माप_प्रकार)size );
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
              दीर्घ       cur_size,
              दीर्घ       new_size,
              शून्य*      block )
  {
    FT_UNUSED( memory );
    FT_UNUSED( cur_size );

    परत ft_srealloc( block, (माप_प्रकार)new_size );
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
  FT_CALLBACK_DEF( अचिन्हित दीर्घ )
  ft_ansi_stream_io( FT_Stream       stream,
                     अचिन्हित दीर्घ   offset,
                     अचिन्हित अक्षर*  buffer,
                     अचिन्हित दीर्घ   मोजणी )
  {
    FT_FILE*  file;


    जर ( offset > stream->size && !मोजणी )
      परत 1;

    file = STREAM_FILE( stream );

    जर ( stream->pos != offset )
      ft_fseek( file, (दीर्घ)offset, शुरू_से );

    /* Avoid calling `fread` with `buffer=NULL` and `count=0`, */
    /* which is undefined behaviour.                           */
    जर ( !मोजणी )
      परत 0;

    परत (अचिन्हित दीर्घ)ft_fread( buffer, 1, मोजणी, file );
  }


  /* documentation is in ftstream.h */

  FT_BASE_DEF( FT_Error )
  FT_Stream_Open( FT_Stream    stream,
                  स्थिरांक अक्षर*  filepathname )
  {
    FT_FILE*  file;


    जर ( !stream )
      परत FT_THROW( Invalid_Stream_Handle );

    stream->descriptor.pointer = रिक्त;
    stream->pathname.pointer   = (अक्षर*)filepathname;
    stream->base               = रिक्त;
    stream->pos                = 0;
    stream->पठ               = रिक्त;
    stream->बंद              = रिक्त;

    file = ft_fopen( filepathname, "rb" );
    जर ( !file )
    {
      FT_ERROR(( "FT_Stream_Open:"
                 " could not open `%s'\n", filepathname ));

      परत FT_THROW( Cannot_Open_Resource );
    }

    ft_fseek( file, 0, अंत_से );
    stream->size = (अचिन्हित दीर्घ)ft_ftell( file );
    जर ( !stream->size )
    {
      FT_ERROR(( "FT_Stream_Open:" ));
      FT_ERROR(( " opened `%s' but zero-sized\n", filepathname ));
      ft_fclose( file );
      परत FT_THROW( Cannot_Open_Stream );
    }
    ft_fseek( file, 0, शुरू_से );

    stream->descriptor.pointer = file;
    stream->पठ  = ft_ansi_stream_io;
    stream->बंद = ft_ansi_stream_close;

    FT_TRACE1(( "FT_Stream_Open:" ));
    FT_TRACE1(( " opened `%s' (%lu bytes) successfully\n",
                filepathname, stream->size ));

    परत FT_Err_Ok;
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


    memory = (FT_Memory)ft_smalloc( आकार ( *memory ) );
    जर ( memory )
    {
      memory->user    = रिक्त;
      memory->alloc   = ft_alloc;
      memory->पुनः_स्मृति = ft_realloc;
      memory->मुक्त    = ft_free;
#ifdef FT_DEBUG_MEMORY
      ft_mem_debug_init( memory );
#endif
    }

    परत memory;
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
