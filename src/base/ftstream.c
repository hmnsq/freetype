/****************************************************************************
 *
 * ftstream.c
 *
 *   I/O stream support (body).
 *
 * Copyright (C) 2000-2026 by
 * David Turner, Robert Wilhelm, and Werner Lemberg.
 *
 * This file is part of the FreeType project, and may only be used,
 * modified, and distributed under the terms of the FreeType project
 * license, LICENSE.TXT.  By continuing to use, modify, or distribute
 * this file you indicate that you have read the license and
 * understand and accept it fully.
 *
 */


#ಅನ್ತರ್ಭಾವ <freetype/internal/ftstream.h>
#ಅನ್ತರ್ಭಾವ <freetype/internal/ftdebug.h>


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು  FT_COMPONENT
#ವ್ಯಾಖ್ಯಾನಿಸು FT_COMPONENT  ಹರಿವು


  FT_ತಳ_DEF( ಶೂನ್ಯ )
  FT_ಹರಿವು_Open_ಸ್ಮರಣೆ( FT_ಹರಿವು       ಹರಿವು,
                        ಸ್ಥಿರಾಂಕ FT_Byte*  ತಳ,
                        FT_ULong        ಗಾತ್ರ_2 )
  {
    ಹರಿವು->ತಳ   = (FT_Byte*) ತಳ;
    ಹರಿವು->ಗಾತ್ರ_2   = ಗಾತ್ರ_2;
    ಹರಿವು->pos    = 0;
    ಹರಿವು->cursor = ಖಾಲಿ;
    ಹರಿವು->ಪಠ   = ಖಾಲಿ;
    ಹರಿವು->close  = ಖಾಲಿ;
  }


  FT_ತಳ_DEF( ಶೂನ್ಯ )
  FT_ಹರಿವು_Close( FT_ಹರಿವು  ಹರಿವು )
  {
    ಆದರೆ ( ಹರಿವು && ಹರಿವು->close )
      ಹರಿವು->close( ಹರಿವು );
  }


  FT_ತಳ_DEF( FT_Error )
  FT_ಹರಿವು_Seek( FT_ಹರಿವು  ಹರಿವು,
                  FT_ULong   pos )
  {
    FT_Error  error = FT_Err_Ok;


    ಆದರೆ ( ಹರಿವು->ಪಠ )
    {
      ಆದರೆ ( ಹರಿವು->ಪಠ( ಹರಿವು, pos, ಖಾಲಿ, 0 ) )
      {
        FT_ERROR(( "FT_Stream_Seek:"
                   " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
                   pos, ಹರಿವು->ಗಾತ್ರ_2 ));

        error = FT_THROW( Invalid_ಹರಿವು_Operation );
      }
    }
    /* note that seeking to the first position after the file is valid */
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ( pos > ಹರಿವು->ಗಾತ್ರ_2 )
    {
      FT_ERROR(( "FT_Stream_Seek:"
                 " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
                 pos, ಹರಿವು->ಗಾತ್ರ_2 ));

      error = FT_THROW( Invalid_ಹರಿವು_Operation );
    }

    ಆದರೆ ( !error )
      ಹರಿವು->pos = pos;

    ಹಿಂತಿರುಗಿ error;
  }


  FT_ತಳ_DEF( FT_Error )
  FT_ಹರಿವು_Skip( FT_ಹರಿವು  ಹರಿವು,
                  FT_Long    ದೂರ )
  {
    ಆದರೆ ( ದೂರ < 0 )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ಹರಿವು_Operation );

    ಹಿಂತಿರುಗಿ FT_ಹರಿವು_Seek( ಹರಿವು, ಹರಿವು->pos + (FT_ULong)ದೂರ );
  }


  FT_ತಳ_DEF( FT_ULong )
  FT_ಹರಿವು_Pos( FT_ಹರಿವು  ಹರಿವು )
  {
    ಹಿಂತಿರುಗಿ ಹರಿವು->pos;
  }


  FT_ತಳ_DEF( FT_Error )
  FT_ಹರಿವು_Read( FT_ಹರಿವು  ಹರಿವು,
                  FT_Byte*   ಬಫರ್,
                  FT_ULong   ಎಣಿಕೆ_2 )
  {
    ಹಿಂತಿರುಗಿ FT_ಹರಿವು_Read_At( ಹರಿವು, ಹರಿವು->pos, ಬಫರ್, ಎಣಿಕೆ_2 );
  }


  FT_ತಳ_DEF( FT_Error )
  FT_ಹರಿವು_Read_At( FT_ಹರಿವು  ಹರಿವು,
                    FT_ULong   pos,
                    FT_Byte*   ಬಫರ್,
                    FT_ULong   ಎಣಿಕೆ_2 )
  {
    FT_Error  error = FT_Err_Ok;
    FT_ULong  read_ಬೈಟುಗಳು;


    ಆದರೆ ( pos >= ಹರಿವು->ಗಾತ್ರ_2 )
    {
      FT_ERROR(( "FT_Stream_ReadAt:"
                 " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
                 pos, ಹರಿವು->ಗಾತ್ರ_2 ));

      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ಹರಿವು_Operation );
    }

    ಆದರೆ ( ಹರಿವು->ಪಠ )
      read_ಬೈಟುಗಳು = ಹರಿವು->ಪಠ( ಹರಿವು, pos, ಬಫರ್, ಎಣಿಕೆ_2 );
    ಇಲ್ಲದಿದ್ದರೆ
    {
      read_ಬೈಟುಗಳು = ಹರಿವು->ಗಾತ್ರ_2 - pos;
      ಆದರೆ ( read_ಬೈಟುಗಳು > ಎಣಿಕೆ_2 )
        read_ಬೈಟುಗಳು = ಎಣಿಕೆ_2;

      /* Allow "reading" zero bytes without UB even if buffer is NULL */
      ಆದರೆ ( ಎಣಿಕೆ_2 )
        FT_MEM_COPY( ಬಫರ್, ಹರಿವು->ತಳ + pos, read_ಬೈಟುಗಳು );
    }

    ಹರಿವು->pos = pos + read_ಬೈಟುಗಳು;

    ಆದರೆ ( read_ಬೈಟುಗಳು < ಎಣಿಕೆ_2 )
    {
      FT_ERROR(( "FT_Stream_ReadAt:"
                 " invalid read; expected %lu bytes, got %lu\n",
                 ಎಣಿಕೆ_2, read_ಬೈಟುಗಳು ));

      error = FT_THROW( Invalid_ಹರಿವು_Operation );
    }

    ಹಿಂತಿರುಗಿ error;
  }


  FT_ತಳ_DEF( FT_ULong )
  FT_ಹರಿವು_Try_Read( FT_ಹರಿವು  ಹರಿವು,
                     FT_Byte*   ಬಫರ್,
                     FT_ULong   ಎಣಿಕೆ_2 )
  {
    FT_ULong  read_ಬೈಟುಗಳು = 0;


    ಆದರೆ ( ಹರಿವು->pos >= ಹರಿವು->ಗಾತ್ರ_2 )
      ಹೋಗು Exit;

    ಆದರೆ ( ಹರಿವು->ಪಠ )
      read_ಬೈಟುಗಳು = ಹರಿವು->ಪಠ( ಹರಿವು, ಹರಿವು->pos, ಬಫರ್, ಎಣಿಕೆ_2 );
    ಇಲ್ಲದಿದ್ದರೆ
    {
      read_ಬೈಟುಗಳು = ಹರಿವು->ಗಾತ್ರ_2 - ಹರಿವು->pos;
      ಆದರೆ ( read_ಬೈಟುಗಳು > ಎಣಿಕೆ_2 )
        read_ಬೈಟುಗಳು = ಎಣಿಕೆ_2;

      /* Allow "reading" zero bytes without UB even if buffer is NULL */
      ಆದರೆ ( ಎಣಿಕೆ_2 )
        FT_MEM_COPY( ಬಫರ್, ಹರಿವು->ತಳ + ಹರಿವು->pos, read_ಬೈಟುಗಳು );
    }

    ಹರಿವು->pos += read_ಬೈಟುಗಳು;

  Exit:
    ಹಿಂತಿರುಗಿ read_ಬೈಟುಗಳು;
  }


  FT_ತಳ_DEF( FT_Error )
  FT_ಹರಿವು_ಹೊರತೆಗೆ_ಚೌಕಟ್ಟು( FT_ಹರಿವು  ಹರಿವು,
                          FT_ULong   ಎಣಿಕೆ_2,
                          FT_Byte**  pbytes )
  {
    FT_Error  error;


    error = FT_ಹರಿವು_Enter_ಚೌಕಟ್ಟು( ಹರಿವು, ಎಣಿಕೆ_2 );
    ಆದರೆ ( !error )
    {
      *pbytes = (FT_Byte*)ಹರಿವು->cursor;

      /* equivalent to FT_Stream_ExitFrame(), with no memory block release */
      ಹರಿವು->cursor = ಖಾಲಿ;
      ಹರಿವು->ಎಲ್ಲೆ  = ಖಾಲಿ;
    }

    ಹಿಂತಿರುಗಿ error;
  }


  FT_ತಳ_DEF( ಶೂನ್ಯ )
  FT_ಹರಿವು_Release_ಚೌಕಟ್ಟು( FT_ಹರಿವು  ಹರಿವು,
                          FT_Byte**  pbytes )
  {
    ಆದರೆ ( ಹರಿವು && ಹರಿವು->ಪಠ )
    {
      FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ = ಹರಿವು->ಸ್ಮರಣೆ;


#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ FT_DEBUG_ಸ್ಮರಣೆ
      ft_mem_free( ಸ್ಮರಣೆ, *pbytes );
#ಇಲ್ಲದಿದ್ದರೆ
      FT_FREE( *pbytes );
#ಆದರೆ_ಅಂತ್ಯ
    }

    *pbytes = ಖಾಲಿ;
  }


  FT_ತಳ_DEF( FT_Error )
  FT_ಹರಿವು_Enter_ಚೌಕಟ್ಟು( FT_ಹರಿವು  ಹರಿವು,
                        FT_ULong   ಎಣಿಕೆ_2 )
  {
    FT_Error  error = FT_Err_Ok;
    FT_ULong  read_ಬೈಟುಗಳು;


    FT_TRACE7(( "FT_Stream_EnterFrame: %lu bytes\n", ಎಣಿಕೆ_2 ));

    /* check for nested frame access */
    FT_ASSERT( ಹರಿವು && ಹರಿವು->cursor == 0 );

    ಆದರೆ ( ಹರಿವು->ಪಠ )
    {
      /* allocate the frame in memory */
      FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ = ಹರಿವು->ಸ್ಮರಣೆ;


      /* simple sanity check */
      ಆದರೆ ( ಎಣಿಕೆ_2 > ಹರಿವು->ಗಾತ್ರ_2 )
      {
        FT_ERROR(( "FT_Stream_EnterFrame:"
                   " frame size (%lu) larger than stream size (%lu)\n",
                   ಎಣಿಕೆ_2, ಹರಿವು->ಗಾತ್ರ_2 ));

        error = FT_THROW( Invalid_ಹರಿವು_Operation );
        ಹೋಗು Exit;
      }

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ FT_DEBUG_ಸ್ಮರಣೆ
      /* assume `ft_debug_file_` and `ft_debug_lineno_` are already set */
      ಹರಿವು->ತಳ = (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ*)ft_mem_qalloc( ಸ್ಮರಣೆ,
                                                    (FT_Long)ಎಣಿಕೆ_2,
                                                    &error );
      ಆದರೆ ( error )
        ಹೋಗು Exit;
#ಇಲ್ಲದಿದ್ದರೆ
      ಆದರೆ ( FT_QALLOC( ಹರಿವು->ತಳ, ಎಣಿಕೆ_2 ) )
        ಹೋಗು Exit;
#ಆದರೆ_ಅಂತ್ಯ
      /* read it */
      read_ಬೈಟುಗಳು = ಹರಿವು->ಪಠ( ಹರಿವು, ಹರಿವು->pos,
                                 ಹರಿವು->ತಳ, ಎಣಿಕೆ_2 );
      ಆದರೆ ( read_ಬೈಟುಗಳು < ಎಣಿಕೆ_2 )
      {
        FT_ERROR(( "FT_Stream_EnterFrame:"
                   " invalid read; expected %lu bytes, got %lu\n",
                   ಎಣಿಕೆ_2, read_ಬೈಟುಗಳು ));

        FT_FREE( ಹರಿವು->ತಳ );
        error = FT_THROW( Invalid_ಹರಿವು_Operation );
      }

      ಹರಿವು->cursor = ಹರಿವು->ತಳ;
      ಹರಿವು->ಎಲ್ಲೆ  = FT_ಸ್ಥಾನಾಂತರ( ಹರಿವು->cursor, ಎಣಿಕೆ_2 );
      ಹರಿವು->pos   += read_ಬೈಟುಗಳು;
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
      /* check current and new position */
      ಆದರೆ ( ಹರಿವು->pos >= ಹರಿವು->ಗಾತ್ರ_2        ||
           ಹರಿವು->ಗಾತ್ರ_2 - ಹರಿವು->pos < ಎಣಿಕೆ_2 )
      {
        FT_ERROR(( "FT_Stream_EnterFrame:"
                   " invalid i/o; pos = 0x%lx, count = %lu, size = 0x%lx\n",
                   ಹರಿವು->pos, ಎಣಿಕೆ_2, ಹರಿವು->ಗಾತ್ರ_2 ));

        error = FT_THROW( Invalid_ಹರಿವು_Operation );
        ಹೋಗು Exit;
      }

      /* set cursor */
      ಹರಿವು->cursor = ಹರಿವು->ತಳ + ಹರಿವು->pos;
      ಹರಿವು->ಎಲ್ಲೆ  = ಹರಿವು->cursor + ಎಣಿಕೆ_2;
      ಹರಿವು->pos   += ಎಣಿಕೆ_2;
    }

  Exit:
    ಹಿಂತಿರುಗಿ error;
  }


  FT_ತಳ_DEF( ಶೂನ್ಯ )
  FT_ಹರಿವು_Exit_ಚೌಕಟ್ಟು( FT_ಹರಿವು  ಹರಿವು )
  {
    /* IMPORTANT: The assertion stream->cursor != 0 was removed, given    */
    /*            that it is possible to access a frame of length 0 in    */
    /*            some weird fonts (usually, when accessing an array of   */
    /*            0 records, like in some strange kern tables).           */
    /*                                                                    */
    /*  In this case, the loader code handles the 0-length table          */
    /*  gracefully; however, stream.cursor is really set to 0 by the      */
    /*  FT_Stream_EnterFrame() call, and this is not an error.            */

    FT_TRACE7(( "FT_Stream_ExitFrame\n" ));

    FT_ASSERT( ಹರಿವು );

    ಆದರೆ ( ಹರಿವು->ಪಠ )
    {
      FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ = ಹರಿವು->ಸ್ಮರಣೆ;


#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ FT_DEBUG_ಸ್ಮರಣೆ
      ft_mem_free( ಸ್ಮರಣೆ, ಹರಿವು->ತಳ );
      ಹರಿವು->ತಳ = ಖಾಲಿ;
#ಇಲ್ಲದಿದ್ದರೆ
      FT_FREE( ಹರಿವು->ತಳ );
#ಆದರೆ_ಅಂತ್ಯ
    }

    ಹರಿವು->cursor = ಖಾಲಿ;
    ಹರಿವು->ಎಲ್ಲೆ  = ಖಾಲಿ;
  }


  FT_ತಳ_DEF( FT_Byte )
  FT_ಹರಿವು_Get_Byte( FT_ಹರಿವು  ಹರಿವು )
  {
    FT_Byte  ಫಲಿತಾಂಶ;


    FT_ASSERT( ಹರಿವು && ಹರಿವು->cursor );

    ಫಲಿತಾಂಶ = 0;
    ಆದರೆ ( ಹರಿವು->cursor < ಹರಿವು->ಎಲ್ಲೆ )
      ಫಲಿತಾಂಶ = *ಹರಿವು->cursor++;

    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
  }


  FT_ತಳ_DEF( FT_UInt16 )
  FT_ಹರಿವು_Get_UShort( FT_ಹರಿವು  ಹರಿವು )
  {
    FT_Byte*   p;
    FT_UInt16  ಫಲಿತಾಂಶ;


    FT_ASSERT( ಹರಿವು && ಹರಿವು->cursor );

    ಫಲಿತಾಂಶ         = 0;
    p              = ಹರಿವು->cursor;
    ಆದರೆ ( p + 1 < ಹರಿವು->ಎಲ್ಲೆ )
      ಫಲಿತಾಂಶ       = FT_ಮುಂದಿನ_USHORT( p );
    ಹರಿವು->cursor = p;

    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
  }


  FT_ತಳ_DEF( FT_UInt16 )
  FT_ಹರಿವು_Get_UShort_LE( FT_ಹರಿವು  ಹರಿವು )
  {
    FT_Byte*   p;
    FT_UInt16  ಫಲಿತಾಂಶ;


    FT_ASSERT( ಹರಿವು && ಹರಿವು->cursor );

    ಫಲಿತಾಂಶ         = 0;
    p              = ಹರಿವು->cursor;
    ಆದರೆ ( p + 1 < ಹರಿವು->ಎಲ್ಲೆ )
      ಫಲಿತಾಂಶ       = FT_ಮುಂದಿನ_USHORT_LE( p );
    ಹರಿವು->cursor = p;

    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
  }


  FT_ತಳ_DEF( FT_UInt32 )
  FT_ಹರಿವು_Get_UOffset( FT_ಹರಿವು  ಹರಿವು )
  {
    FT_Byte*  p;
    FT_UInt32 ಫಲಿತಾಂಶ;


    FT_ASSERT( ಹರಿವು && ಹರಿವು->cursor );

    ಫಲಿತಾಂಶ         = 0;
    p              = ಹರಿವು->cursor;
    ಆದರೆ ( p + 2 < ಹರಿವು->ಎಲ್ಲೆ )
      ಫಲಿತಾಂಶ       = FT_ಮುಂದಿನ_UOFF3( p );
    ಹರಿವು->cursor = p;
    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
  }


  FT_ತಳ_DEF( FT_UInt32 )
  FT_ಹರಿವು_Get_ULong( FT_ಹರಿವು  ಹರಿವು )
  {
    FT_Byte*  p;
    FT_UInt32 ಫಲಿತಾಂಶ;


    FT_ASSERT( ಹರಿವು && ಹರಿವು->cursor );

    ಫಲಿತಾಂಶ         = 0;
    p              = ಹರಿವು->cursor;
    ಆದರೆ ( p + 3 < ಹರಿವು->ಎಲ್ಲೆ )
      ಫಲಿತಾಂಶ       = FT_ಮುಂದಿನ_ULONG( p );
    ಹರಿವು->cursor = p;
    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
  }


  FT_ತಳ_DEF( FT_UInt32 )
  FT_ಹರಿವು_Get_ULong_LE( FT_ಹರಿವು  ಹರಿವು )
  {
    FT_Byte*  p;
    FT_UInt32 ಫಲಿತಾಂಶ;


    FT_ASSERT( ಹರಿವು && ಹರಿವು->cursor );

    ಫಲಿತಾಂಶ         = 0;
    p              = ಹರಿವು->cursor;
    ಆದರೆ ( p + 3 < ಹರಿವು->ಎಲ್ಲೆ )
      ಫಲಿತಾಂಶ       = FT_ಮುಂದಿನ_ULONG_LE( p );
    ಹರಿವು->cursor = p;
    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
  }


  FT_ತಳ_DEF( FT_Byte )
  FT_ಹರಿವು_Read_Byte( FT_ಹರಿವು  ಹರಿವು,
                      FT_Error*  error )
  {
    FT_Byte  ಫಲಿತಾಂಶ = 0;


    FT_ASSERT( ಹರಿವು );

    ಆದರೆ ( ಹರಿವು->pos < ಹರಿವು->ಗಾತ್ರ_2 )
    {
      ಆದರೆ ( ಹರಿವು->ಪಠ )
      {
        ಆದರೆ ( ಹರಿವು->ಪಠ( ಹರಿವು, ಹರಿವು->pos, &ಫಲಿತಾಂಶ, 1L ) != 1L )
          ಹೋಗು Fail;
      }
      ಇಲ್ಲದಿದ್ದರೆ
        ಫಲಿತಾಂಶ = ಹರಿವು->ತಳ[ಹರಿವು->pos];
    }
    ಇಲ್ಲದಿದ್ದರೆ
      ಹೋಗು Fail;

    ಹರಿವು->pos++;

    *error = FT_Err_Ok;

    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;

  Fail:
    *error = FT_THROW( Invalid_ಹರಿವು_Operation );
    FT_ERROR(( "FT_Stream_ReadByte:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               ಹರಿವು->pos, ಹರಿವು->ಗಾತ್ರ_2 ));

    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
  }


  FT_ತಳ_DEF( FT_UInt16 )
  FT_ಹರಿವು_Read_UShort( FT_ಹರಿವು  ಹರಿವು,
                        FT_Error*  error )
  {
    FT_Byte    reads[2];
    FT_Byte*   p;
    FT_UInt16  ಫಲಿತಾಂಶ = 0;


    FT_ASSERT( ಹರಿವು );

    ಆದರೆ ( ಹರಿವು->pos + 1 < ಹರಿವು->ಗಾತ್ರ_2 )
    {
      ಆದರೆ ( ಹರಿವು->ಪಠ )
      {
        ಆದರೆ ( ಹರಿವು->ಪಠ( ಹರಿವು, ಹರಿವು->pos, reads, 2L ) != 2L )
          ಹೋಗು Fail;

        p = reads;
      }
      ಇಲ್ಲದಿದ್ದರೆ
        p = ಹರಿವು->ತಳ + ಹರಿವು->pos;

      ಆದರೆ ( p )
        ಫಲಿತಾಂಶ = FT_ಮುಂದಿನ_USHORT( p );
    }
    ಇಲ್ಲದಿದ್ದರೆ
      ಹೋಗು Fail;

    ಹರಿವು->pos += 2;

    *error = FT_Err_Ok;

    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;

  Fail:
    *error = FT_THROW( Invalid_ಹರಿವು_Operation );
    FT_ERROR(( "FT_Stream_ReadUShort:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               ಹರಿವು->pos, ಹರಿವು->ಗಾತ್ರ_2 ));

    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
  }


  FT_ತಳ_DEF( FT_UInt16 )
  FT_ಹರಿವು_Read_UShort_LE( FT_ಹರಿವು  ಹರಿವು,
                          FT_Error*  error )
  {
    FT_Byte    reads[2];
    FT_Byte*   p;
    FT_UInt16  ಫಲಿತಾಂಶ = 0;


    FT_ASSERT( ಹರಿವು );

    ಆದರೆ ( ಹರಿವು->pos + 1 < ಹರಿವು->ಗಾತ್ರ_2 )
    {
      ಆದರೆ ( ಹರಿವು->ಪಠ )
      {
        ಆದರೆ ( ಹರಿವು->ಪಠ( ಹರಿವು, ಹರಿವು->pos, reads, 2L ) != 2L )
          ಹೋಗು Fail;

        p = reads;
      }
      ಇಲ್ಲದಿದ್ದರೆ
        p = ಹರಿವು->ತಳ + ಹರಿವು->pos;

      ಆದರೆ ( p )
        ಫಲಿತಾಂಶ = FT_ಮುಂದಿನ_USHORT_LE( p );
    }
    ಇಲ್ಲದಿದ್ದರೆ
      ಹೋಗು Fail;

    ಹರಿವು->pos += 2;

    *error = FT_Err_Ok;

    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;

  Fail:
    *error = FT_THROW( Invalid_ಹರಿವು_Operation );
    FT_ERROR(( "FT_Stream_ReadUShortLE:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               ಹರಿವು->pos, ಹರಿವು->ಗಾತ್ರ_2 ));

    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
  }


  FT_ತಳ_DEF( FT_ULong )
  FT_ಹರಿವು_Read_UOffset( FT_ಹರಿವು  ಹರಿವು,
                         FT_Error*  error )
  {
    FT_Byte   reads[3];
    FT_Byte*  p;
    FT_ULong  ಫಲಿತಾಂಶ = 0;


    FT_ASSERT( ಹರಿವು );

    ಆದರೆ ( ಹರಿವು->pos + 2 < ಹರಿವು->ಗಾತ್ರ_2 )
    {
      ಆದರೆ ( ಹರಿವು->ಪಠ )
      {
        ಆದರೆ (ಹರಿವು->ಪಠ( ಹರಿವು, ಹರಿವು->pos, reads, 3L ) != 3L )
          ಹೋಗು Fail;

        p = reads;
      }
      ಇಲ್ಲದಿದ್ದರೆ
        p = ಹರಿವು->ತಳ + ಹರಿವು->pos;

      ಆದರೆ ( p )
        ಫಲಿತಾಂಶ = FT_ಮುಂದಿನ_UOFF3( p );
    }
    ಇಲ್ಲದಿದ್ದರೆ
      ಹೋಗು Fail;

    ಹರಿವು->pos += 3;

    *error = FT_Err_Ok;

    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;

  Fail:
    *error = FT_THROW( Invalid_ಹರಿವು_Operation );
    FT_ERROR(( "FT_Stream_ReadUOffset:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               ಹರಿವು->pos, ಹರಿವು->ಗಾತ್ರ_2 ));

    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
  }


  FT_ತಳ_DEF( FT_UInt32 )
  FT_ಹರಿವು_Read_ULong( FT_ಹರಿವು  ಹರಿವು,
                       FT_Error*  error )
  {
    FT_Byte   reads[4];
    FT_Byte*  p;
    FT_UInt32 ಫಲಿತಾಂಶ = 0;


    FT_ASSERT( ಹರಿವು );

    ಆದರೆ ( ಹರಿವು->pos + 3 < ಹರಿವು->ಗಾತ್ರ_2 )
    {
      ಆದರೆ ( ಹರಿವು->ಪಠ )
      {
        ಆದರೆ ( ಹರಿವು->ಪಠ( ಹರಿವು, ಹರಿವು->pos, reads, 4L ) != 4L )
          ಹೋಗು Fail;

        p = reads;
      }
      ಇಲ್ಲದಿದ್ದರೆ
        p = ಹರಿವು->ತಳ + ಹರಿವು->pos;

      ಆದರೆ ( p )
        ಫಲಿತಾಂಶ = FT_ಮುಂದಿನ_ULONG( p );
    }
    ಇಲ್ಲದಿದ್ದರೆ
      ಹೋಗು Fail;

    ಹರಿವು->pos += 4;

    *error = FT_Err_Ok;

    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;

  Fail:
    *error = FT_THROW( Invalid_ಹರಿವು_Operation );
    FT_ERROR(( "FT_Stream_ReadULong:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               ಹರಿವು->pos, ಹರಿವು->ಗಾತ್ರ_2 ));

    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
  }


  FT_ತಳ_DEF( FT_UInt32 )
  FT_ಹರಿವು_Read_ULong_LE( FT_ಹರಿವು  ಹರಿವು,
                         FT_Error*  error )
  {
    FT_Byte   reads[4];
    FT_Byte*  p;
    FT_UInt32 ಫಲಿತಾಂಶ = 0;


    FT_ASSERT( ಹರಿವು );

    ಆದರೆ ( ಹರಿವು->pos + 3 < ಹರಿವು->ಗಾತ್ರ_2 )
    {
      ಆದರೆ ( ಹರಿವು->ಪಠ )
      {
        ಆದರೆ ( ಹರಿವು->ಪಠ( ಹರಿವು, ಹರಿವು->pos, reads, 4L ) != 4L )
          ಹೋಗು Fail;

        p = reads;
      }
      ಇಲ್ಲದಿದ್ದರೆ
        p = ಹರಿವು->ತಳ + ಹರಿವು->pos;

      ಆದರೆ ( p )
        ಫಲಿತಾಂಶ = FT_ಮುಂದಿನ_ULONG_LE( p );
    }
    ಇಲ್ಲದಿದ್ದರೆ
      ಹೋಗು Fail;

    ಹರಿವು->pos += 4;

    *error = FT_Err_Ok;

    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;

  Fail:
    *error = FT_THROW( Invalid_ಹರಿವು_Operation );
    FT_ERROR(( "FT_Stream_ReadULongLE:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               ಹರಿವು->pos, ಹರಿವು->ಗಾತ್ರ_2 ));

    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
  }


  FT_ತಳ_DEF( FT_Error )
  FT_ಹರಿವು_Read_Fields( FT_ಹರಿವು              ಹರಿವು,
                        ಸ್ಥಿರಾಂಕ FT_ಚೌಕಟ್ಟು_ಕ್ಷೇತ್ರ*  fields,
                        ಶೂನ್ಯ*                  structure )
  {
    FT_Error  error;
    FT_Bool   ಚೌಕಟ್ಟು_accessed = 0;
    FT_Byte*  cursor;


    ಆದರೆ ( !fields )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

    ಆದರೆ ( !ಹರಿವು )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ಹರಿವು_Handle );

    cursor = ಹರಿವು->cursor;

    error = FT_Err_Ok;
    ಮಾಡು
    {
      FT_ULong  ಮೌಲ್ಯ;
      FT_Int    sign_ಸರಿಸು;
      FT_Byte*  p;


      ಆಯ್ಕೆ ( fields->ಮೌಲ್ಯ )
      {
      ಸ್ಥಿತಿ ft_ಚೌಕಟ್ಟು_ಆರಂಭ:  /* access a new frame */
        error = FT_ಹರಿವು_Enter_ಚೌಕಟ್ಟು( ಹರಿವು, fields->ಸ್ಥಾನಾಂತರ );
        ಆದರೆ ( error )
          ಹೋಗು Exit;

        ಚೌಕಟ್ಟು_accessed = 1;
        cursor         = ಹರಿವು->cursor;
        fields++;
        ಮುಂದುವರಿ;  /* loop! */

      ಸ್ಥಿತಿ ft_ಚೌಕಟ್ಟು_ಬೈಟುಗಳು:  /* read a byte sequence */
      ಸ್ಥಿತಿ ft_ಚೌಕಟ್ಟು_skip:   /* skip some bytes      */
        {
          FT_ಸ್ಥಾನಾಂತರ_2  len = fields->ಗಾತ್ರ_2;


          ಆದರೆ ( len > (FT_ಸ್ಥಾನಾಂತರ_2)( ಹರಿವು->ಎಲ್ಲೆ - cursor ) )
          {
            error = FT_THROW( Invalid_ಹರಿವು_Operation );
            ಹೋಗು Exit;
          }

          ಆದರೆ ( fields->ಮೌಲ್ಯ == ft_ಚೌಕಟ್ಟು_ಬೈಟುಗಳು )
          {
            p = (FT_Byte*)structure + fields->ಸ್ಥಾನಾಂತರ;
            FT_MEM_COPY( p, cursor, len );
          }
          cursor += len;
          fields++;
          ಮುಂದುವರಿ;
        }

      ಸ್ಥಿತಿ ft_ಚೌಕಟ್ಟು_byte:
      ಸ್ಥಿತಿ ft_ಚೌಕಟ್ಟು_schar:  /* read a single byte */
        ಮೌಲ್ಯ = FT_ಮುಂದಿನ_BYTE( cursor );
        sign_ಸರಿಸು = 24;
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ ft_ಚೌಕಟ್ಟು_short_be:
      ಸ್ಥಿತಿ ft_ಚೌಕಟ್ಟು_ushort_be:  /* read a 2-byte big-endian short */
        ಮೌಲ್ಯ = FT_ಮುಂದಿನ_USHORT( cursor );
        sign_ಸರಿಸು = 16;
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ ft_ಚೌಕಟ್ಟು_short_le:
      ಸ್ಥಿತಿ ft_ಚೌಕಟ್ಟು_ushort_le:  /* read a 2-byte little-endian short */
        ಮೌಲ್ಯ = FT_ಮುಂದಿನ_USHORT_LE( cursor );
        sign_ಸರಿಸು = 16;
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ ft_ಚೌಕಟ್ಟು_long_be:
      ಸ್ಥಿತಿ ft_ಚೌಕಟ್ಟು_ulong_be:  /* read a 4-byte big-endian long */
        ಮೌಲ್ಯ = FT_ಮುಂದಿನ_ULONG( cursor );
        sign_ಸರಿಸು = 0;
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ ft_ಚೌಕಟ್ಟು_long_le:
      ಸ್ಥಿತಿ ft_ಚೌಕಟ್ಟು_ulong_le:  /* read a 4-byte little-endian long */
        ಮೌಲ್ಯ = FT_ಮುಂದಿನ_ULONG_LE( cursor );
        sign_ಸರಿಸು = 0;
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ ft_ಚೌಕಟ್ಟು_off3_be:
      ಸ್ಥಿತಿ ft_ಚೌಕಟ್ಟು_uoff3_be:  /* read a 3-byte big-endian long */
        ಮೌಲ್ಯ = FT_ಮುಂದಿನ_UOFF3( cursor );
        sign_ಸರಿಸು = 8;
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ ft_ಚೌಕಟ್ಟು_off3_le:
      ಸ್ಥಿತಿ ft_ಚೌಕಟ್ಟು_uoff3_le:  /* read a 3-byte little-endian long */
        ಮೌಲ್ಯ = FT_ಮುಂದಿನ_UOFF3_LE( cursor );
        sign_ಸರಿಸು = 8;
        ನಿಲ್ಲಿಸು;

      ಪೂರ್ವನಿಯೋಜಿತ:
        /* otherwise, exit the loop */
        ಹರಿವು->cursor = cursor;
        ಹೋಗು Exit;
      }

      /* now, compute the signed value if necessary */
      ಆದರೆ ( fields->ಮೌಲ್ಯ & FT_ಚೌಕಟ್ಟು_OP_SIGNED )
        ಮೌಲ್ಯ = (FT_ULong)( (FT_Int32)( ಮೌಲ್ಯ << sign_ಸರಿಸು ) >> sign_ಸರಿಸು );

      /* finally, store the value in the object */

      p = (FT_Byte*)structure + fields->ಸ್ಥಾನಾಂತರ;
      ಆಯ್ಕೆ ( fields->ಗಾತ್ರ_2 )
      {
      ಸ್ಥಿತಿ ( 8 / FT_CHAR_ಬಿಟ್ ):
        *(FT_Byte*)p = (FT_Byte)ಮೌಲ್ಯ;
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ ( 16 / FT_CHAR_ಬಿಟ್ ):
        *(FT_UShort*)p = (FT_UShort)ಮೌಲ್ಯ;
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ ( 32 / FT_CHAR_ಬಿಟ್ ):
        *(FT_UInt32*)p = (FT_UInt32)ಮೌಲ್ಯ;
        ನಿಲ್ಲಿಸು;

      ಪೂರ್ವನಿಯೋಜಿತ:  /* for 64-bit systems */
        *(FT_ULong*)p = (FT_ULong)ಮೌಲ್ಯ;
      }

      /* go to next field */
      fields++;
    }
    ಆಗ ( 1 );

  Exit:
    /* close the frame if it was opened by this read */
    ಆದರೆ ( ಚೌಕಟ್ಟು_accessed )
      FT_ಹರಿವು_Exit_ಚೌಕಟ್ಟು( ಹರಿವು );

    ಹಿಂತಿರುಗಿ error;
  }


/*ಕೊನೆ*/
