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
  ft_alloc( FT_حافظہ  حافظہ,
            لما       ناپ )
  {
    FT_بن_ورتیا( حافظہ );

    أرجع ft_smalloc( (size_t)ناپ );
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
  ft_realloc( FT_حافظہ  حافظہ,
              لما       cur_ناپ,
              لما       new_ناپ,
              فراغ*      بلاک )
  {
    FT_بن_ورتیا( حافظہ );
    FT_بن_ورتیا( cur_ناپ );

    أرجع ft_srealloc( بلاک, (size_t)new_ناپ );
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
  ft_free( FT_حافظہ  حافظہ,
           فراغ*      بلاک )
  {
    FT_بن_ورتیا( حافظہ );

    ft_sfree( بلاک );
  }


  /**************************************************************************
   *
   *                    RESOURCE MANAGEMENT INTERFACE
   *
   */

#جے_بنا_تعریف FT_CONFIG_OPTION_DISABLE_دھارا_SUPPORT

  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#تعریف_رد  FT_COMPONENT
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
  FT_CALLBACK_DEF( فراغ )
  ft_ansi_دھارا_close( FT_دھارا  دھارا )
  {
    ft_fclose( دھارا_FILE( دھارا ) );

    دھارا->descriptor.اشارہ_گر = عدم;
    دھارا->ناپ               = 0;
    دھارا->ادھار               = عدم;
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
  FT_CALLBACK_DEF( بنانشانی لما )
  ft_ansi_دھارا_io( FT_دھارا       دھارا,
                     بنانشانی لما   وتھ,
                     بنانشانی حرف*  بفر,
                     بنانشانی لما   گنتی )
  {
    FT_FILE*  file;


    إذا ( وتھ > دھارا->ناپ && !گنتی )
      أرجع 1;

    file = دھارا_FILE( دھارا );

    إذا ( دھارا->pos != وتھ )
      ft_fseek( file, (لما)وتھ, SEEK_سیٹ );

    /* Avoid calling `fread` with `buffer=NULL` and `count=0`, */
    /* which is undefined behaviour.                           */
    إذا ( !گنتی )
      أرجع 0;

    أرجع (بنانشانی لما)ft_fread( بفر, 1, گنتی, file );
  }


  /* documentation is in ftstream.h */

  FT_ادھار_DEF( FT_Error )
  FT_دھارا_Open( FT_دھارا    دھارا,
                  ثابت حرف*  filepathname )
  {
    FT_FILE*  file;


    إذا ( !دھارا )
      أرجع FT_THROW( Invalid_دھارا_Handle );

    دھارا->descriptor.اشارہ_گر = عدم;
    دھارا->pathname.اشارہ_گر   = (حرف*)filepathname;
    دھارا->ادھار               = عدم;
    دھارا->pos                = 0;
    دھارا->اقرأ               = عدم;
    دھارا->close              = عدم;

    file = ft_fopen( filepathname, "rb" );
    إذا ( !file )
    {
      FT_ERROR(( "FT_Stream_Open:"
                 " could not open `%s'\n", filepathname ));

      أرجع FT_THROW( Cannot_Open_Resource );
    }

    ft_fseek( file, 0, SEEK_سرا );
    دھارا->ناپ = (بنانشانی لما)ft_ftell( file );
    إذا ( !دھارا->ناپ )
    {
      FT_ERROR(( "FT_Stream_Open:" ));
      FT_ERROR(( " opened `%s' but zero-sized\n", filepathname ));
      ft_fclose( file );
      أرجع FT_THROW( Cannot_Open_دھارا );
    }
    ft_fseek( file, 0, SEEK_سیٹ );

    دھارا->descriptor.اشارہ_گر = file;
    دھارا->اقرأ  = ft_ansi_دھارا_io;
    دھارا->close = ft_ansi_دھارا_close;

    FT_TRACE1(( "FT_Stream_Open:" ));
    FT_TRACE1(( " opened `%s' (%lu bytes) successfully\n",
                filepathname, دھارا->ناپ ));

    أرجع FT_Err_Ok;
  }

#جے_انت /* !FT_CONFIG_OPTION_DISABLE_STREAM_SUPPORT */

#جے_تعریف_شدہ FT_DEBUG_حافظہ

  خارجي FT_Int
  ft_mem_debug_init( FT_حافظہ  حافظہ );

  خارجي فراغ
  ft_mem_debug_پورا( FT_حافظہ  حافظہ );

#جے_انت


  /* documentation is in ftobjs.h */

  FT_ادھار_DEF( FT_حافظہ )
  FT_New_حافظہ( فراغ )
  {
    FT_حافظہ  حافظہ;


    حافظہ = (FT_حافظہ)ft_smalloc( حجم ( *حافظہ ) );
    إذا ( حافظہ )
    {
      حافظہ->user    = عدم;
      حافظہ->alloc   = ft_alloc;
      حافظہ->realloc = ft_realloc;
      حافظہ->حرر    = ft_free;
#جے_تعریف_شدہ FT_DEBUG_حافظہ
      ft_mem_debug_init( حافظہ );
#جے_انت
    }

    أرجع حافظہ;
  }


  /* documentation is in ftobjs.h */

  FT_ادھار_DEF( فراغ )
  FT_پورا_حافظہ( FT_حافظہ  حافظہ )
  {
#جے_تعریف_شدہ FT_DEBUG_حافظہ
    ft_mem_debug_پورا( حافظہ );
#جے_انت
    ft_sfree( حافظہ );
  }


/*سرا*/
