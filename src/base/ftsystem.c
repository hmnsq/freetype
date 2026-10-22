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


#ಅನ್ತರ್ಭಾವ <ft2build.h>
#ಅನ್ತರ್ಭಾವ FT_CONFIG_CONFIG_H
#ಅನ್ತರ್ಭಾವ <freetype/internal/ftdebug.h>
#ಅನ್ತರ್ಭಾವ <freetype/internal/ftstream.h>
#ಅನ್ತರ್ಭಾವ <freetype/ftsystem.h>
#ಅನ್ತರ್ಭಾವ <freetype/fterrors.h>
#ಅನ್ತರ್ಭಾವ <freetype/fttypes.h>


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
  FT_CALLBACK_DEF( ಶೂನ್ಯ* )
  ft_alloc( FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ,
            ದೀರ್ಘ       ಗಾತ್ರ_2 )
  {
    FT_ಬಳಸದ( ಸ್ಮರಣೆ );

    ಹಿಂತಿರುಗಿ ft_smalloc( (size_t)ಗಾತ್ರ_2 );
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
  FT_CALLBACK_DEF( ಶೂನ್ಯ* )
  ft_realloc( FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ,
              ದೀರ್ಘ       cur_ಗಾತ್ರ,
              ದೀರ್ಘ       new_ಗಾತ್ರ,
              ಶೂನ್ಯ*      ಖಂಡ )
  {
    FT_ಬಳಸದ( ಸ್ಮರಣೆ );
    FT_ಬಳಸದ( cur_ಗಾತ್ರ );

    ಹಿಂತಿರುಗಿ ft_srealloc( ಖಂಡ, (size_t)new_ಗಾತ್ರ );
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
  FT_CALLBACK_DEF( ಶೂನ್ಯ )
  ft_free( FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ,
           ಶೂನ್ಯ*      ಖಂಡ )
  {
    FT_ಬಳಸದ( ಸ್ಮರಣೆ );

    ft_sfree( ಖಂಡ );
  }


  /**************************************************************************
   *
   *                    RESOURCE MANAGEMENT INTERFACE
   *
   */

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ FT_CONFIG_OPTION_DISABLE_ಹರಿವು_SUPPORT

  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು  FT_COMPONENT
#ವ್ಯಾಖ್ಯಾನಿಸು FT_COMPONENT  io

  /* We use the macro STREAM_FILE for convenience to extract the       */
  /* system-specific stream handle from a given FreeType stream object */
#ವ್ಯಾಖ್ಯಾನಿಸು ಹರಿವು_FILE( ಹರಿವು )  ( (FT_FILE*)ಹರಿವು->descriptor.ಸೂಚಕ )


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
  FT_CALLBACK_DEF( ಶೂನ್ಯ )
  ft_ansi_ಹರಿವು_close( FT_ಹರಿವು  ಹರಿವು )
  {
    ft_fclose( ಹರಿವು_FILE( ಹರಿವು ) );

    ಹರಿವು->descriptor.ಸೂಚಕ = ಖಾಲಿ;
    ಹರಿವು->ಗಾತ್ರ_2               = 0;
    ಹರಿವು->ತಳ               = ಖಾಲಿ;
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
  FT_CALLBACK_DEF( ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ )
  ft_ansi_ಹರಿವು_io( FT_ಹರಿವು       ಹರಿವು,
                     ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ   ಸ್ಥಾನಾಂತರ,
                     ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ*  ಬಫರ್,
                     ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ   ಎಣಿಕೆ_2 )
  {
    FT_FILE*  file;


    ಆದರೆ ( ಸ್ಥಾನಾಂತರ > ಹರಿವು->ಗಾತ್ರ_2 && !ಎಣಿಕೆ_2 )
      ಹಿಂತಿರುಗಿ 1;

    file = ಹರಿವು_FILE( ಹರಿವು );

    ಆದರೆ ( ಹರಿವು->pos != ಸ್ಥಾನಾಂತರ )
      ft_fseek( file, (ದೀರ್ಘ)ಸ್ಥಾನಾಂತರ, SEEK_ಗಣ );

    /* Avoid calling `fread` with `buffer=NULL` and `count=0`, */
    /* which is undefined behaviour.                           */
    ಆದರೆ ( !ಎಣಿಕೆ_2 )
      ಹಿಂತಿರುಗಿ 0;

    ಹಿಂತಿರುಗಿ (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ)ft_fread( ಬಫರ್, 1, ಎಣಿಕೆ_2, file );
  }


  /* documentation is in ftstream.h */

  FT_ತಳ_DEF( FT_Error )
  FT_ಹರಿವು_Open( FT_ಹರಿವು    ಹರಿವು,
                  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*  filepathname )
  {
    FT_FILE*  file;


    ಆದರೆ ( !ಹರಿವು )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ಹರಿವು_Handle );

    ಹರಿವು->descriptor.ಸೂಚಕ = ಖಾಲಿ;
    ಹರಿವು->pathname.ಸೂಚಕ   = (ಅಕ್ಷರ*)filepathname;
    ಹರಿವು->ತಳ               = ಖಾಲಿ;
    ಹರಿವು->pos                = 0;
    ಹರಿವು->ಪಠ               = ಖಾಲಿ;
    ಹರಿವು->close              = ಖಾಲಿ;

    file = ft_fopen( filepathname, "rb" );
    ಆದರೆ ( !file )
    {
      FT_ERROR(( "FT_Stream_Open:"
                 " could not open `%s'\n", filepathname ));

      ಹಿಂತಿರುಗಿ FT_THROW( Cannot_Open_Resource );
    }

    ft_fseek( file, 0, SEEK_ಕೊನೆ );
    ಹರಿವು->ಗಾತ್ರ_2 = (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ)ft_ftell( file );
    ಆದರೆ ( !ಹರಿವು->ಗಾತ್ರ_2 )
    {
      FT_ERROR(( "FT_Stream_Open:" ));
      FT_ERROR(( " opened `%s' but zero-sized\n", filepathname ));
      ft_fclose( file );
      ಹಿಂತಿರುಗಿ FT_THROW( Cannot_Open_ಹರಿವು );
    }
    ft_fseek( file, 0, SEEK_ಗಣ );

    ಹರಿವು->descriptor.ಸೂಚಕ = file;
    ಹರಿವು->ಪಠ  = ft_ansi_ಹರಿವು_io;
    ಹರಿವು->close = ft_ansi_ಹರಿವು_close;

    FT_TRACE1(( "FT_Stream_Open:" ));
    FT_TRACE1(( " opened `%s' (%lu bytes) successfully\n",
                filepathname, ಹರಿವು->ಗಾತ್ರ_2 ));

    ಹಿಂತಿರುಗಿ FT_Err_Ok;
  }

#ಆದರೆ_ಅಂತ್ಯ /* !FT_CONFIG_OPTION_DISABLE_STREAM_SUPPORT */

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ FT_DEBUG_ಸ್ಮರಣೆ

  ಬಾಹ್ಯ FT_Int
  ft_mem_debug_init( FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ );

  ಬಾಹ್ಯ ಶೂನ್ಯ
  ft_mem_debug_ಮುಗಿದ( FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ );

#ಆದರೆ_ಅಂತ್ಯ


  /* documentation is in ftobjs.h */

  FT_ತಳ_DEF( FT_ಸ್ಮರಣೆ )
  FT_New_ಸ್ಮರಣೆ( ಶೂನ್ಯ )
  {
    FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ;


    ಸ್ಮರಣೆ = (FT_ಸ್ಮರಣೆ)ft_smalloc( ಗಾತ್ರ ( *ಸ್ಮರಣೆ ) );
    ಆದರೆ ( ಸ್ಮರಣೆ )
    {
      ಸ್ಮರಣೆ->user    = ಖಾಲಿ;
      ಸ್ಮರಣೆ->alloc   = ft_alloc;
      ಸ್ಮರಣೆ->realloc = ft_realloc;
      ಸ್ಮರಣೆ->ಬಿಡುಗಡೆ    = ft_free;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ FT_DEBUG_ಸ್ಮರಣೆ
      ft_mem_debug_init( ಸ್ಮರಣೆ );
#ಆದರೆ_ಅಂತ್ಯ
    }

    ಹಿಂತಿರುಗಿ ಸ್ಮರಣೆ;
  }


  /* documentation is in ftobjs.h */

  FT_ತಳ_DEF( ಶೂನ್ಯ )
  FT_ಮುಗಿದ_ಸ್ಮರಣೆ( FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ )
  {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ FT_DEBUG_ಸ್ಮರಣೆ
    ft_mem_debug_ಮುಗಿದ( ಸ್ಮರಣೆ );
#ಆದರೆ_ಅಂತ್ಯ
    ft_sfree( ಸ್ಮರಣೆ );
  }


/*ಕೊನೆ*/
