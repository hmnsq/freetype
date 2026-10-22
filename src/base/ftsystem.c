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


#شامل <ft2build.h>
#شامل FT_CONFIG_CONFIG_H
#شامل <freetype/internal/ftdebug.h>
#شامل <freetype/internal/ftstream.h>
#شامل <freetype/ftsystem.h>
#شامل <freetype/fterrors.h>
#شامل <freetype/fttypes.h>


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
  FT_CALLBACK_DEF( خالی* )
  ft_alloc( FT_حافظہ  حافظہ,
            طویل       سائز )
  {
    FT_غیر_مستعمل( حافظہ );

    واپس ft_smalloc( (size_t)سائز );
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
  FT_CALLBACK_DEF( خالی* )
  ft_realloc( FT_حافظہ  حافظہ,
              طویل       cur_سائز,
              طویل       new_سائز,
              خالی*      بلاک )
  {
    FT_غیر_مستعمل( حافظہ );
    FT_غیر_مستعمل( cur_سائز );

    واپس ft_srealloc( بلاک, (size_t)new_سائز );
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
  FT_CALLBACK_DEF( خالی )
  ft_free( FT_حافظہ  حافظہ,
           خالی*      بلاک )
  {
    FT_غیر_مستعمل( حافظہ );

    ft_sfree( بلاک );
  }


  /**************************************************************************
   *
   *                    RESOURCE MANAGEMENT INTERFACE
   *
   */

#اگر_غیر_تعریف_شدہ FT_CONFIG_OPTION_DISABLE_دھارا_SUPPORT

  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#تعریف_منسوخ  FT_COMPONENT
#تعریف_کرو FT_COMPONENT  io

  /* We use the macro STREAM_FILE for convenience to extract the       */
  /* system-specific stream handle from a given FreeType stream object */
#تعریف_کرو دھارا_FILE( دھارا )  ( (FT_FILE*)دھارا->descriptor.اشارہ_گر )


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
  FT_CALLBACK_DEF( خالی )
  ft_ansi_دھارا_close( FT_دھارا  دھارا )
  {
    ft_fclose( دھارا_FILE( دھارا ) );

    دھارا->descriptor.اشارہ_گر = تہی;
    دھارا->سائز               = 0;
    دھارا->بنیاد               = تہی;
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
  FT_CALLBACK_DEF( غیرعلامتی طویل )
  ft_ansi_دھارا_io( FT_دھارا       دھارا,
                     غیرعلامتی طویل   انحراف,
                     غیرعلامتی حرف*  بفر,
                     غیرعلامتی طویل   گنتی )
  {
    FT_FILE*  file;


    اگر ( انحراف > دھارا->سائز && !گنتی )
      واپس 1;

    file = دھارا_FILE( دھارا );

    اگر ( دھارا->pos != انحراف )
      ft_fseek( file, (طویل)انحراف, SEEK_سیٹ );

    /* Avoid calling `fread` with `buffer=NULL` and `count=0`, */
    /* which is undefined behaviour.                           */
    اگر ( !گنتی )
      واپس 0;

    واپس (غیرعلامتی طویل)ft_fread( بفر, 1, گنتی, file );
  }


  /* documentation is in ftstream.h */

  FT_بنیاد_DEF( FT_Error )
  FT_دھارا_Open( FT_دھارا    دھارا,
                  ثابت حرف*  filepathname )
  {
    FT_FILE*  file;


    اگر ( !دھارا )
      واپس FT_THROW( Invalid_دھارا_Handle );

    دھارا->descriptor.اشارہ_گر = تہی;
    دھارا->pathname.اشارہ_گر   = (حرف*)filepathname;
    دھارا->بنیاد               = تہی;
    دھارا->pos                = 0;
    دھارا->اقرأ               = تہی;
    دھارا->close              = تہی;

    file = ft_fopen( filepathname, "rb" );
    اگر ( !file )
    {
      FT_ERROR(( "FT_Stream_Open:"
                 " could not open `%s'\n", filepathname ));

      واپس FT_THROW( Cannot_Open_Resource );
    }

    ft_fseek( file, 0, SEEK_انتہا );
    دھارا->سائز = (غیرعلامتی طویل)ft_ftell( file );
    اگر ( !دھارا->سائز )
    {
      FT_ERROR(( "FT_Stream_Open:" ));
      FT_ERROR(( " opened `%s' but zero-sized\n", filepathname ));
      ft_fclose( file );
      واپس FT_THROW( Cannot_Open_دھارا );
    }
    ft_fseek( file, 0, SEEK_سیٹ );

    دھارا->descriptor.اشارہ_گر = file;
    دھارا->اقرأ  = ft_ansi_دھارا_io;
    دھارا->close = ft_ansi_دھارا_close;

    FT_TRACE1(( "FT_Stream_Open:" ));
    FT_TRACE1(( " opened `%s' (%lu bytes) successfully\n",
                filepathname, دھارا->سائز ));

    واپس FT_Err_Ok;
  }

#اگر_ختم /* !FT_CONFIG_OPTION_DISABLE_STREAM_SUPPORT */

#اگر_تعریف_شدہ FT_DEBUG_حافظہ

  خارجی FT_Int
  ft_mem_debug_init( FT_حافظہ  حافظہ );

  خارجی خالی
  ft_mem_debug_مکمل( FT_حافظہ  حافظہ );

#اگر_ختم


  /* documentation is in ftobjs.h */

  FT_بنیاد_DEF( FT_حافظہ )
  FT_New_حافظہ( خالی )
  {
    FT_حافظہ  حافظہ;


    حافظہ = (FT_حافظہ)ft_smalloc( حجم ( *حافظہ ) );
    اگر ( حافظہ )
    {
      حافظہ->user    = تہی;
      حافظہ->alloc   = ft_alloc;
      حافظہ->realloc = ft_realloc;
      حافظہ->آزاد    = ft_free;
#اگر_تعریف_شدہ FT_DEBUG_حافظہ
      ft_mem_debug_init( حافظہ );
#اگر_ختم
    }

    واپس حافظہ;
  }


  /* documentation is in ftobjs.h */

  FT_بنیاد_DEF( خالی )
  FT_مکمل_حافظہ( FT_حافظہ  حافظہ )
  {
#اگر_تعریف_شدہ FT_DEBUG_حافظہ
    ft_mem_debug_مکمل( حافظہ );
#اگر_ختم
    ft_sfree( حافظہ );
  }


/*انتہا*/
