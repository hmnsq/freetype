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


#అన్తర్భావ <ft2build.h>
#అన్తర్భావ FT_CONFIG_CONFIG_H
#అన్తర్భావ <freetype/internal/ftdebug.h>
#అన్తర్భావ <freetype/internal/ftstream.h>
#అన్తర్భావ <freetype/ftsystem.h>
#అన్తర్భావ <freetype/fterrors.h>
#అన్తర్భావ <freetype/fttypes.h>


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
  FT_CALLBACK_DEF( శూన్య* )
  ft_alloc( FT_జ్ఞాపకం  జ్ఞాపకం,
            దీర్ఘ       కొలత )
  {
    FT_వాడని( జ్ఞాపకం );

    తిరిగి ft_smalloc( (size_t)కొలత );
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
  FT_CALLBACK_DEF( శూన్య* )
  ft_realloc( FT_జ్ఞాపకం  జ్ఞాపకం,
              దీర్ఘ       cur_కొలత,
              దీర్ఘ       new_కొలత,
              శూన్య*      ఖండం )
  {
    FT_వాడని( జ్ఞాపకం );
    FT_వాడని( cur_కొలత );

    తిరిగి ft_srealloc( ఖండం, (size_t)new_కొలత );
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
  FT_CALLBACK_DEF( శూన్య )
  ft_free( FT_జ్ఞాపకం  జ్ఞాపకం,
           శూన్య*      ఖండం )
  {
    FT_వాడని( జ్ఞాపకం );

    ft_sfree( ఖండం );
  }


  /**************************************************************************
   *
   *                    RESOURCE MANAGEMENT INTERFACE
   *
   */

#నిర్వచితం_కాకపోతే FT_CONFIG_OPTION_DISABLE_ప్రవాహం_SUPPORT

  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#నిర్వచనం_రద్దు  FT_COMPONENT
#నిర్వచించు FT_COMPONENT  io

  /* We use the macro STREAM_FILE for convenience to extract the       */
  /* system-specific stream handle from a given FreeType stream object */
#నిర్వచించు ప్రవాహం_FILE( ప్రవాహం )  ( (FT_FILE*)ప్రవాహం->descriptor.సూచిక )


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
  FT_CALLBACK_DEF( శూన్య )
  ft_ansi_ప్రవాహం_close( FT_ప్రవాహం  ప్రవాహం )
  {
    ft_fclose( ప్రవాహం_FILE( ప్రవాహం ) );

    ప్రవాహం->descriptor.సూచిక = ఖాళీ;
    ప్రవాహం->కొలత               = 0;
    ప్రవాహం->పునాది               = ఖాళీ;
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
  FT_CALLBACK_DEF( గుర్తులేని దీర్ఘ )
  ft_ansi_ప్రవాహం_io( FT_ప్రవాహం       ప్రవాహం,
                     గుర్తులేని దీర్ఘ   స్థానభ్రంశం,
                     గుర్తులేని అక్షరం*  బఫరు,
                     గుర్తులేని దీర్ఘ   లెక్క )
  {
    FT_FILE*  file;


    ఒకవేళ ( స్థానభ్రంశం > ప్రవాహం->కొలత && !లెక్క )
      తిరిగి 1;

    file = ప్రవాహం_FILE( ప్రవాహం );

    ఒకవేళ ( ప్రవాహం->pos != స్థానభ్రంశం )
      ft_fseek( file, (దీర్ఘ)స్థానభ్రంశం, SEEK_సమితి );

    /* Avoid calling `fread` with `buffer=NULL` and `count=0`, */
    /* which is undefined behaviour.                           */
    ఒకవేళ ( !లెక్క )
      తిరిగి 0;

    తిరిగి (గుర్తులేని దీర్ఘ)ft_fread( బఫరు, 1, లెక్క, file );
  }


  /* documentation is in ftstream.h */

  FT_పునాది_DEF( FT_Error )
  FT_ప్రవాహం_Open( FT_ప్రవాహం    ప్రవాహం,
                  స్థిరరాశి అక్షరం*  filepathname )
  {
    FT_FILE*  file;


    ఒకవేళ ( !ప్రవాహం )
      తిరిగి FT_THROW( Invalid_ప్రవాహం_Handle );

    ప్రవాహం->descriptor.సూచిక = ఖాళీ;
    ప్రవాహం->pathname.సూచిక   = (అక్షరం*)filepathname;
    ప్రవాహం->పునాది               = ఖాళీ;
    ప్రవాహం->pos                = 0;
    ప్రవాహం->పఠ               = ఖాళీ;
    ప్రవాహం->close              = ఖాళీ;

    file = ft_fopen( filepathname, "rb" );
    ఒకవేళ ( !file )
    {
      FT_ERROR(( "FT_Stream_Open:"
                 " could not open `%s'\n", filepathname ));

      తిరిగి FT_THROW( Cannot_Open_Resource );
    }

    ft_fseek( file, 0, SEEK_చివర );
    ప్రవాహం->కొలత = (గుర్తులేని దీర్ఘ)ft_ftell( file );
    ఒకవేళ ( !ప్రవాహం->కొలత )
    {
      FT_ERROR(( "FT_Stream_Open:" ));
      FT_ERROR(( " opened `%s' but zero-sized\n", filepathname ));
      ft_fclose( file );
      తిరిగి FT_THROW( Cannot_Open_ప్రవాహం );
    }
    ft_fseek( file, 0, SEEK_సమితి );

    ప్రవాహం->descriptor.సూచిక = file;
    ప్రవాహం->పఠ  = ft_ansi_ప్రవాహం_io;
    ప్రవాహం->close = ft_ansi_ప్రవాహం_close;

    FT_TRACE1(( "FT_Stream_Open:" ));
    FT_TRACE1(( " opened `%s' (%lu bytes) successfully\n",
                filepathname, ప్రవాహం->కొలత ));

    తిరిగి FT_Err_Ok;
  }

#అయితే_ముగింపు /* !FT_CONFIG_OPTION_DISABLE_STREAM_SUPPORT */

#నిర్వచితమైతే FT_DEBUG_జ్ఞాపకం

  బాహ్య FT_Int
  ft_mem_debug_init( FT_జ్ఞాపకం  జ్ఞాపకం );

  బాహ్య శూన్య
  ft_mem_debug_పూర్తి( FT_జ్ఞాపకం  జ్ఞాపకం );

#అయితే_ముగింపు


  /* documentation is in ftobjs.h */

  FT_పునాది_DEF( FT_జ్ఞాపకం )
  FT_New_జ్ఞాపకం( శూన్య )
  {
    FT_జ్ఞాపకం  జ్ఞాపకం;


    జ్ఞాపకం = (FT_జ్ఞాపకం)ft_smalloc( పరిమాణం ( *జ్ఞాపకం ) );
    ఒకవేళ ( జ్ఞాపకం )
    {
      జ్ఞాపకం->user    = ఖాళీ;
      జ్ఞాపకం->alloc   = ft_alloc;
      జ్ఞాపకం->realloc = ft_realloc;
      జ్ఞాపకం->విడుదల    = ft_free;
#నిర్వచితమైతే FT_DEBUG_జ్ఞాపకం
      ft_mem_debug_init( జ్ఞాపకం );
#అయితే_ముగింపు
    }

    తిరిగి జ్ఞాపకం;
  }


  /* documentation is in ftobjs.h */

  FT_పునాది_DEF( శూన్య )
  FT_పూర్తి_జ్ఞాపకం( FT_జ్ఞాపకం  జ్ఞాపకం )
  {
#నిర్వచితమైతే FT_DEBUG_జ్ఞాపకం
    ft_mem_debug_పూర్తి( జ్ఞాపకం );
#అయితే_ముగింపు
    ft_sfree( జ్ఞాపకం );
  }


/*చివర*/
