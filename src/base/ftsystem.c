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
  ft_alloc( FT_स्मृतिः  स्मृतिः,
            दीर्घ       आकार )
  {
    FT_अप्रयुक्तम्( स्मृतिः );

    प्रत्यागम ft_smalloc( (size_t)आकार );
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
  ft_realloc( FT_स्मृतिः  स्मृतिः,
              दीर्घ       cur_आकार,
              दीर्घ       new_आकार,
              शून्य*      खण्डम् )
  {
    FT_अप्रयुक्तम्( स्मृतिः );
    FT_अप्रयुक्तम्( cur_आकार );

    प्रत्यागम ft_srealloc( खण्डम्, (size_t)new_आकार );
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
  ft_free( FT_स्मृतिः  स्मृतिः,
           शून्य*      खण्डम् )
  {
    FT_अप्रयुक्तम्( स्मृतिः );

    ft_sfree( खण्डम् );
  }


  /**************************************************************************
   *
   *                    RESOURCE MANAGEMENT INTERFACE
   *
   */

#यदि_अपरिभाषितम् FT_CONFIG_OPTION_DISABLE_प्रवाहः_SUPPORT

  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#अपरिभाषय  FT_COMPONENT
#परिभाषय FT_COMPONENT  io

  /* We use the macro STREAM_FILE for convenience to extract the       */
  /* system-specific stream handle from a given FreeType stream object */
#परिभाषय प्रवाहः_FILE( प्रवाहः )  ( (FT_FILE*)प्रवाहः->descriptor.pointer )


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
  ft_ansi_प्रवाहः_close( FT_प्रवाहः  प्रवाहः )
  {
    ft_fclose( प्रवाहः_FILE( प्रवाहः ) );

    प्रवाहः->descriptor.pointer = रिक्त;
    प्रवाहः->आकार               = 0;
    प्रवाहः->आधारः               = रिक्त;
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
  FT_CALLBACK_DEF( अचिह्नित दीर्घ )
  ft_ansi_प्रवाहः_io( FT_प्रवाहः       प्रवाहः,
                     अचिह्नित दीर्घ   विस्थापनम्,
                     अचिह्नित वर्ण*  बफर,
                     अचिह्नित दीर्घ   गणनम् )
  {
    FT_FILE*  file;


    यदि ( विस्थापनम् > प्रवाहः->आकार && !गणनम् )
      प्रत्यागम 1;

    file = प्रवाहः_FILE( प्रवाहः );

    यदि ( प्रवाहः->pos != विस्थापनम् )
      ft_fseek( file, (दीर्घ)विस्थापनम्, SEEK_समुच्चयः );

    /* Avoid calling `fread` with `buffer=NULL` and `count=0`, */
    /* which is undefined behaviour.                           */
    यदि ( !गणनम् )
      प्रत्यागम 0;

    प्रत्यागम (अचिह्नित दीर्घ)ft_fread( बफर, 1, गणनम्, file );
  }


  /* documentation is in ftstream.h */

  FT_आधारः_DEF( FT_Error )
  FT_प्रवाहः_Open( FT_प्रवाहः    प्रवाहः,
                  ध्रुव वर्ण*  filepathname )
  {
    FT_FILE*  file;


    यदि ( !प्रवाहः )
      प्रत्यागम FT_THROW( Invalid_प्रवाहः_Handle );

    प्रवाहः->descriptor.pointer = रिक्त;
    प्रवाहः->pathname.pointer   = (वर्ण*)filepathname;
    प्रवाहः->आधारः               = रिक्त;
    प्रवाहः->pos                = 0;
    प्रवाहः->पठ               = रिक्त;
    प्रवाहः->close              = रिक्त;

    file = ft_fopen( filepathname, "rb" );
    यदि ( !file )
    {
      FT_ERROR(( "FT_Stream_Open:"
                 " could not open `%s'\n", filepathname ));

      प्रत्यागम FT_THROW( Cannot_Open_Resource );
    }

    ft_fseek( file, 0, SEEK_अन्तः );
    प्रवाहः->आकार = (अचिह्नित दीर्घ)ft_ftell( file );
    यदि ( !प्रवाहः->आकार )
    {
      FT_ERROR(( "FT_Stream_Open:" ));
      FT_ERROR(( " opened `%s' but zero-sized\n", filepathname ));
      ft_fclose( file );
      प्रत्यागम FT_THROW( Cannot_Open_प्रवाहः );
    }
    ft_fseek( file, 0, SEEK_समुच्चयः );

    प्रवाहः->descriptor.pointer = file;
    प्रवाहः->पठ  = ft_ansi_प्रवाहः_io;
    प्रवाहः->close = ft_ansi_प्रवाहः_close;

    FT_TRACE1(( "FT_Stream_Open:" ));
    FT_TRACE1(( " opened `%s' (%lu bytes) successfully\n",
                filepathname, प्रवाहः->आकार ));

    प्रत्यागम FT_Err_Ok;
  }

#यदि_अन्तः /* !FT_CONFIG_OPTION_DISABLE_STREAM_SUPPORT */

#यदि_परिभाषितम् FT_DEBUG_स्मृतिः

  बाह्य FT_Int
  ft_mem_debug_init( FT_स्मृतिः  स्मृतिः );

  बाह्य शून्य
  ft_mem_debug_समाप्तम्( FT_स्मृतिः  स्मृतिः );

#यदि_अन्तः


  /* documentation is in ftobjs.h */

  FT_आधारः_DEF( FT_स्मृतिः )
  FT_New_स्मृतिः( शून्य )
  {
    FT_स्मृतिः  स्मृतिः;


    स्मृतिः = (FT_स्मृतिः)ft_smalloc( परिमाण ( *स्मृतिः ) );
    यदि ( स्मृतिः )
    {
      स्मृतिः->user    = रिक्त;
      स्मृतिः->alloc   = ft_alloc;
      स्मृतिः->realloc = ft_realloc;
      स्मृतिः->मुक्त    = ft_free;
#यदि_परिभाषितम् FT_DEBUG_स्मृतिः
      ft_mem_debug_init( स्मृतिः );
#यदि_अन्तः
    }

    प्रत्यागम स्मृतिः;
  }


  /* documentation is in ftobjs.h */

  FT_आधारः_DEF( शून्य )
  FT_समाप्तम्_स्मृतिः( FT_स्मृतिः  स्मृतिः )
  {
#यदि_परिभाषितम् FT_DEBUG_स्मृतिः
    ft_mem_debug_समाप्तम्( स्मृतिः );
#यदि_अन्तः
    ft_sfree( स्मृतिः );
  }


/*अन्तः*/
