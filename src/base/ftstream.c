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


#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftstream.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftdebug.h>


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ  FT_COMPONENT
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FT_COMPONENT  ਧਾਰਾ


  FT_ਅਧਾਰ_DEF( ਖਾਲੀ )
  FT_ਧਾਰਾ_Open_ਸਮ੍ਰਿਤੀ( FT_ਧਾਰਾ       ਧਾਰਾ,
                        ਅਚਲ FT_Byte*  ਅਧਾਰ,
                        FT_ULong        ਮਾਪ )
  {
    ਧਾਰਾ->ਅਧਾਰ   = (FT_Byte*) ਅਧਾਰ;
    ਧਾਰਾ->ਮਾਪ   = ਮਾਪ;
    ਧਾਰਾ->pos    = 0;
    ਧਾਰਾ->cursor = ਨਿਰਾ;
    ਧਾਰਾ->ਪਠ   = ਨਿਰਾ;
    ਧਾਰਾ->close  = ਨਿਰਾ;
  }


  FT_ਅਧਾਰ_DEF( ਖਾਲੀ )
  FT_ਧਾਰਾ_Close( FT_ਧਾਰਾ  ਧਾਰਾ )
  {
    ਜੇ ( ਧਾਰਾ && ਧਾਰਾ->close )
      ਧਾਰਾ->close( ਧਾਰਾ );
  }


  FT_ਅਧਾਰ_DEF( FT_Error )
  FT_ਧਾਰਾ_Seek( FT_ਧਾਰਾ  ਧਾਰਾ,
                  FT_ULong   pos )
  {
    FT_Error  error = FT_Err_Ok;


    ਜੇ ( ਧਾਰਾ->ਪਠ )
    {
      ਜੇ ( ਧਾਰਾ->ਪਠ( ਧਾਰਾ, pos, ਨਿਰਾ, 0 ) )
      {
        FT_ERROR(( "FT_Stream_Seek:"
                   " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
                   pos, ਧਾਰਾ->ਮਾਪ ));

        error = FT_THROW( Invalid_ਧਾਰਾ_Operation );
      }
    }
    /* note that seeking to the first position after the file is valid */
    ਨਹੀਂਤਾਂ ਜੇ ( pos > ਧਾਰਾ->ਮਾਪ )
    {
      FT_ERROR(( "FT_Stream_Seek:"
                 " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
                 pos, ਧਾਰਾ->ਮਾਪ ));

      error = FT_THROW( Invalid_ਧਾਰਾ_Operation );
    }

    ਜੇ ( !error )
      ਧਾਰਾ->pos = pos;

    ਵਾਪਸ error;
  }


  FT_ਅਧਾਰ_DEF( FT_Error )
  FT_ਧਾਰਾ_Skip( FT_ਧਾਰਾ  ਧਾਰਾ,
                  FT_Long    ਦੂਰੀ )
  {
    ਜੇ ( ਦੂਰੀ < 0 )
      ਵਾਪਸ FT_THROW( Invalid_ਧਾਰਾ_Operation );

    ਵਾਪਸ FT_ਧਾਰਾ_Seek( ਧਾਰਾ, ਧਾਰਾ->pos + (FT_ULong)ਦੂਰੀ );
  }


  FT_ਅਧਾਰ_DEF( FT_ULong )
  FT_ਧਾਰਾ_Pos( FT_ਧਾਰਾ  ਧਾਰਾ )
  {
    ਵਾਪਸ ਧਾਰਾ->pos;
  }


  FT_ਅਧਾਰ_DEF( FT_Error )
  FT_ਧਾਰਾ_Read( FT_ਧਾਰਾ  ਧਾਰਾ,
                  FT_Byte*   ਬਫਰ,
                  FT_ULong   ਗਿਣਤੀ_2 )
  {
    ਵਾਪਸ FT_ਧਾਰਾ_Read_At( ਧਾਰਾ, ਧਾਰਾ->pos, ਬਫਰ, ਗਿਣਤੀ_2 );
  }


  FT_ਅਧਾਰ_DEF( FT_Error )
  FT_ਧਾਰਾ_Read_At( FT_ਧਾਰਾ  ਧਾਰਾ,
                    FT_ULong   pos,
                    FT_Byte*   ਬਫਰ,
                    FT_ULong   ਗਿਣਤੀ_2 )
  {
    FT_Error  error = FT_Err_Ok;
    FT_ULong  read_ਬਾਈਟਾਂ;


    ਜੇ ( pos >= ਧਾਰਾ->ਮਾਪ )
    {
      FT_ERROR(( "FT_Stream_ReadAt:"
                 " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
                 pos, ਧਾਰਾ->ਮਾਪ ));

      ਵਾਪਸ FT_THROW( Invalid_ਧਾਰਾ_Operation );
    }

    ਜੇ ( ਧਾਰਾ->ਪਠ )
      read_ਬਾਈਟਾਂ = ਧਾਰਾ->ਪਠ( ਧਾਰਾ, pos, ਬਫਰ, ਗਿਣਤੀ_2 );
    ਨਹੀਂਤਾਂ
    {
      read_ਬਾਈਟਾਂ = ਧਾਰਾ->ਮਾਪ - pos;
      ਜੇ ( read_ਬਾਈਟਾਂ > ਗਿਣਤੀ_2 )
        read_ਬਾਈਟਾਂ = ਗਿਣਤੀ_2;

      /* Allow "reading" zero bytes without UB even if buffer is NULL */
      ਜੇ ( ਗਿਣਤੀ_2 )
        FT_MEM_COPY( ਬਫਰ, ਧਾਰਾ->ਅਧਾਰ + pos, read_ਬਾਈਟਾਂ );
    }

    ਧਾਰਾ->pos = pos + read_ਬਾਈਟਾਂ;

    ਜੇ ( read_ਬਾਈਟਾਂ < ਗਿਣਤੀ_2 )
    {
      FT_ERROR(( "FT_Stream_ReadAt:"
                 " invalid read; expected %lu bytes, got %lu\n",
                 ਗਿਣਤੀ_2, read_ਬਾਈਟਾਂ ));

      error = FT_THROW( Invalid_ਧਾਰਾ_Operation );
    }

    ਵਾਪਸ error;
  }


  FT_ਅਧਾਰ_DEF( FT_ULong )
  FT_ਧਾਰਾ_Try_Read( FT_ਧਾਰਾ  ਧਾਰਾ,
                     FT_Byte*   ਬਫਰ,
                     FT_ULong   ਗਿਣਤੀ_2 )
  {
    FT_ULong  read_ਬਾਈਟਾਂ = 0;


    ਜੇ ( ਧਾਰਾ->pos >= ਧਾਰਾ->ਮਾਪ )
      ਜਾਓ Exit;

    ਜੇ ( ਧਾਰਾ->ਪਠ )
      read_ਬਾਈਟਾਂ = ਧਾਰਾ->ਪਠ( ਧਾਰਾ, ਧਾਰਾ->pos, ਬਫਰ, ਗਿਣਤੀ_2 );
    ਨਹੀਂਤਾਂ
    {
      read_ਬਾਈਟਾਂ = ਧਾਰਾ->ਮਾਪ - ਧਾਰਾ->pos;
      ਜੇ ( read_ਬਾਈਟਾਂ > ਗਿਣਤੀ_2 )
        read_ਬਾਈਟਾਂ = ਗਿਣਤੀ_2;

      /* Allow "reading" zero bytes without UB even if buffer is NULL */
      ਜੇ ( ਗਿਣਤੀ_2 )
        FT_MEM_COPY( ਬਫਰ, ਧਾਰਾ->ਅਧਾਰ + ਧਾਰਾ->pos, read_ਬਾਈਟਾਂ );
    }

    ਧਾਰਾ->pos += read_ਬਾਈਟਾਂ;

  Exit:
    ਵਾਪਸ read_ਬਾਈਟਾਂ;
  }


  FT_ਅਧਾਰ_DEF( FT_Error )
  FT_ਧਾਰਾ_ਕੱਢੋ_ਚੌਖਟਾ( FT_ਧਾਰਾ  ਧਾਰਾ,
                          FT_ULong   ਗਿਣਤੀ_2,
                          FT_Byte**  pbytes )
  {
    FT_Error  error;


    error = FT_ਧਾਰਾ_Enter_ਚੌਖਟਾ( ਧਾਰਾ, ਗਿਣਤੀ_2 );
    ਜੇ ( !error )
    {
      *pbytes = (FT_Byte*)ਧਾਰਾ->cursor;

      /* equivalent to FT_Stream_ExitFrame(), with no memory block release */
      ਧਾਰਾ->cursor = ਨਿਰਾ;
      ਧਾਰਾ->ਹੱਦਬੰਦੀ  = ਨਿਰਾ;
    }

    ਵਾਪਸ error;
  }


  FT_ਅਧਾਰ_DEF( ਖਾਲੀ )
  FT_ਧਾਰਾ_Release_ਚੌਖਟਾ( FT_ਧਾਰਾ  ਧਾਰਾ,
                          FT_Byte**  pbytes )
  {
    ਜੇ ( ਧਾਰਾ && ਧਾਰਾ->ਪਠ )
    {
      FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ = ਧਾਰਾ->ਸਮ੍ਰਿਤੀ;


#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ FT_DEBUG_ਸਮ੍ਰਿਤੀ
      ft_mem_free( ਸਮ੍ਰਿਤੀ, *pbytes );
#ਨਹੀਂਤਾਂ
      FT_FREE( *pbytes );
#ਜੇ_ਅੰਤ
    }

    *pbytes = ਨਿਰਾ;
  }


  FT_ਅਧਾਰ_DEF( FT_Error )
  FT_ਧਾਰਾ_Enter_ਚੌਖਟਾ( FT_ਧਾਰਾ  ਧਾਰਾ,
                        FT_ULong   ਗਿਣਤੀ_2 )
  {
    FT_Error  error = FT_Err_Ok;
    FT_ULong  read_ਬਾਈਟਾਂ;


    FT_TRACE7(( "FT_Stream_EnterFrame: %lu bytes\n", ਗਿਣਤੀ_2 ));

    /* check for nested frame access */
    FT_ASSERT( ਧਾਰਾ && ਧਾਰਾ->cursor == 0 );

    ਜੇ ( ਧਾਰਾ->ਪਠ )
    {
      /* allocate the frame in memory */
      FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ = ਧਾਰਾ->ਸਮ੍ਰਿਤੀ;


      /* simple sanity check */
      ਜੇ ( ਗਿਣਤੀ_2 > ਧਾਰਾ->ਮਾਪ )
      {
        FT_ERROR(( "FT_Stream_EnterFrame:"
                   " frame size (%lu) larger than stream size (%lu)\n",
                   ਗਿਣਤੀ_2, ਧਾਰਾ->ਮਾਪ ));

        error = FT_THROW( Invalid_ਧਾਰਾ_Operation );
        ਜਾਓ Exit;
      }

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ FT_DEBUG_ਸਮ੍ਰਿਤੀ
      /* assume `ft_debug_file_` and `ft_debug_lineno_` are already set */
      ਧਾਰਾ->ਅਧਾਰ = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ*)ft_mem_qalloc( ਸਮ੍ਰਿਤੀ,
                                                    (FT_Long)ਗਿਣਤੀ_2,
                                                    &error );
      ਜੇ ( error )
        ਜਾਓ Exit;
#ਨਹੀਂਤਾਂ
      ਜੇ ( FT_QALLOC( ਧਾਰਾ->ਅਧਾਰ, ਗਿਣਤੀ_2 ) )
        ਜਾਓ Exit;
#ਜੇ_ਅੰਤ
      /* read it */
      read_ਬਾਈਟਾਂ = ਧਾਰਾ->ਪਠ( ਧਾਰਾ, ਧਾਰਾ->pos,
                                 ਧਾਰਾ->ਅਧਾਰ, ਗਿਣਤੀ_2 );
      ਜੇ ( read_ਬਾਈਟਾਂ < ਗਿਣਤੀ_2 )
      {
        FT_ERROR(( "FT_Stream_EnterFrame:"
                   " invalid read; expected %lu bytes, got %lu\n",
                   ਗਿਣਤੀ_2, read_ਬਾਈਟਾਂ ));

        FT_FREE( ਧਾਰਾ->ਅਧਾਰ );
        error = FT_THROW( Invalid_ਧਾਰਾ_Operation );
      }

      ਧਾਰਾ->cursor = ਧਾਰਾ->ਅਧਾਰ;
      ਧਾਰਾ->ਹੱਦਬੰਦੀ  = FT_ਵਿੱਥ( ਧਾਰਾ->cursor, ਗਿਣਤੀ_2 );
      ਧਾਰਾ->pos   += read_ਬਾਈਟਾਂ;
    }
    ਨਹੀਂਤਾਂ
    {
      /* check current and new position */
      ਜੇ ( ਧਾਰਾ->pos >= ਧਾਰਾ->ਮਾਪ        ||
           ਧਾਰਾ->ਮਾਪ - ਧਾਰਾ->pos < ਗਿਣਤੀ_2 )
      {
        FT_ERROR(( "FT_Stream_EnterFrame:"
                   " invalid i/o; pos = 0x%lx, count = %lu, size = 0x%lx\n",
                   ਧਾਰਾ->pos, ਗਿਣਤੀ_2, ਧਾਰਾ->ਮਾਪ ));

        error = FT_THROW( Invalid_ਧਾਰਾ_Operation );
        ਜਾਓ Exit;
      }

      /* set cursor */
      ਧਾਰਾ->cursor = ਧਾਰਾ->ਅਧਾਰ + ਧਾਰਾ->pos;
      ਧਾਰਾ->ਹੱਦਬੰਦੀ  = ਧਾਰਾ->cursor + ਗਿਣਤੀ_2;
      ਧਾਰਾ->pos   += ਗਿਣਤੀ_2;
    }

  Exit:
    ਵਾਪਸ error;
  }


  FT_ਅਧਾਰ_DEF( ਖਾਲੀ )
  FT_ਧਾਰਾ_Exit_ਚੌਖਟਾ( FT_ਧਾਰਾ  ਧਾਰਾ )
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

    FT_ASSERT( ਧਾਰਾ );

    ਜੇ ( ਧਾਰਾ->ਪਠ )
    {
      FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ = ਧਾਰਾ->ਸਮ੍ਰਿਤੀ;


#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ FT_DEBUG_ਸਮ੍ਰਿਤੀ
      ft_mem_free( ਸਮ੍ਰਿਤੀ, ਧਾਰਾ->ਅਧਾਰ );
      ਧਾਰਾ->ਅਧਾਰ = ਨਿਰਾ;
#ਨਹੀਂਤਾਂ
      FT_FREE( ਧਾਰਾ->ਅਧਾਰ );
#ਜੇ_ਅੰਤ
    }

    ਧਾਰਾ->cursor = ਨਿਰਾ;
    ਧਾਰਾ->ਹੱਦਬੰਦੀ  = ਨਿਰਾ;
  }


  FT_ਅਧਾਰ_DEF( FT_Byte )
  FT_ਧਾਰਾ_Get_Byte( FT_ਧਾਰਾ  ਧਾਰਾ )
  {
    FT_Byte  ਨਤੀਜਾ;


    FT_ASSERT( ਧਾਰਾ && ਧਾਰਾ->cursor );

    ਨਤੀਜਾ = 0;
    ਜੇ ( ਧਾਰਾ->cursor < ਧਾਰਾ->ਹੱਦਬੰਦੀ )
      ਨਤੀਜਾ = *ਧਾਰਾ->cursor++;

    ਵਾਪਸ ਨਤੀਜਾ;
  }


  FT_ਅਧਾਰ_DEF( FT_UInt16 )
  FT_ਧਾਰਾ_Get_UShort( FT_ਧਾਰਾ  ਧਾਰਾ )
  {
    FT_Byte*   p;
    FT_UInt16  ਨਤੀਜਾ;


    FT_ASSERT( ਧਾਰਾ && ਧਾਰਾ->cursor );

    ਨਤੀਜਾ         = 0;
    p              = ਧਾਰਾ->cursor;
    ਜੇ ( p + 1 < ਧਾਰਾ->ਹੱਦਬੰਦੀ )
      ਨਤੀਜਾ       = FT_ਅਗਲਾ_USHORT( p );
    ਧਾਰਾ->cursor = p;

    ਵਾਪਸ ਨਤੀਜਾ;
  }


  FT_ਅਧਾਰ_DEF( FT_UInt16 )
  FT_ਧਾਰਾ_Get_UShort_LE( FT_ਧਾਰਾ  ਧਾਰਾ )
  {
    FT_Byte*   p;
    FT_UInt16  ਨਤੀਜਾ;


    FT_ASSERT( ਧਾਰਾ && ਧਾਰਾ->cursor );

    ਨਤੀਜਾ         = 0;
    p              = ਧਾਰਾ->cursor;
    ਜੇ ( p + 1 < ਧਾਰਾ->ਹੱਦਬੰਦੀ )
      ਨਤੀਜਾ       = FT_ਅਗਲਾ_USHORT_LE( p );
    ਧਾਰਾ->cursor = p;

    ਵਾਪਸ ਨਤੀਜਾ;
  }


  FT_ਅਧਾਰ_DEF( FT_UInt32 )
  FT_ਧਾਰਾ_Get_UOffset( FT_ਧਾਰਾ  ਧਾਰਾ )
  {
    FT_Byte*  p;
    FT_UInt32 ਨਤੀਜਾ;


    FT_ASSERT( ਧਾਰਾ && ਧਾਰਾ->cursor );

    ਨਤੀਜਾ         = 0;
    p              = ਧਾਰਾ->cursor;
    ਜੇ ( p + 2 < ਧਾਰਾ->ਹੱਦਬੰਦੀ )
      ਨਤੀਜਾ       = FT_ਅਗਲਾ_UOFF3( p );
    ਧਾਰਾ->cursor = p;
    ਵਾਪਸ ਨਤੀਜਾ;
  }


  FT_ਅਧਾਰ_DEF( FT_UInt32 )
  FT_ਧਾਰਾ_Get_ULong( FT_ਧਾਰਾ  ਧਾਰਾ )
  {
    FT_Byte*  p;
    FT_UInt32 ਨਤੀਜਾ;


    FT_ASSERT( ਧਾਰਾ && ਧਾਰਾ->cursor );

    ਨਤੀਜਾ         = 0;
    p              = ਧਾਰਾ->cursor;
    ਜੇ ( p + 3 < ਧਾਰਾ->ਹੱਦਬੰਦੀ )
      ਨਤੀਜਾ       = FT_ਅਗਲਾ_ULONG( p );
    ਧਾਰਾ->cursor = p;
    ਵਾਪਸ ਨਤੀਜਾ;
  }


  FT_ਅਧਾਰ_DEF( FT_UInt32 )
  FT_ਧਾਰਾ_Get_ULong_LE( FT_ਧਾਰਾ  ਧਾਰਾ )
  {
    FT_Byte*  p;
    FT_UInt32 ਨਤੀਜਾ;


    FT_ASSERT( ਧਾਰਾ && ਧਾਰਾ->cursor );

    ਨਤੀਜਾ         = 0;
    p              = ਧਾਰਾ->cursor;
    ਜੇ ( p + 3 < ਧਾਰਾ->ਹੱਦਬੰਦੀ )
      ਨਤੀਜਾ       = FT_ਅਗਲਾ_ULONG_LE( p );
    ਧਾਰਾ->cursor = p;
    ਵਾਪਸ ਨਤੀਜਾ;
  }


  FT_ਅਧਾਰ_DEF( FT_Byte )
  FT_ਧਾਰਾ_Read_Byte( FT_ਧਾਰਾ  ਧਾਰਾ,
                      FT_Error*  error )
  {
    FT_Byte  ਨਤੀਜਾ = 0;


    FT_ASSERT( ਧਾਰਾ );

    ਜੇ ( ਧਾਰਾ->pos < ਧਾਰਾ->ਮਾਪ )
    {
      ਜੇ ( ਧਾਰਾ->ਪਠ )
      {
        ਜੇ ( ਧਾਰਾ->ਪਠ( ਧਾਰਾ, ਧਾਰਾ->pos, &ਨਤੀਜਾ, 1L ) != 1L )
          ਜਾਓ Fail;
      }
      ਨਹੀਂਤਾਂ
        ਨਤੀਜਾ = ਧਾਰਾ->ਅਧਾਰ[ਧਾਰਾ->pos];
    }
    ਨਹੀਂਤਾਂ
      ਜਾਓ Fail;

    ਧਾਰਾ->pos++;

    *error = FT_Err_Ok;

    ਵਾਪਸ ਨਤੀਜਾ;

  Fail:
    *error = FT_THROW( Invalid_ਧਾਰਾ_Operation );
    FT_ERROR(( "FT_Stream_ReadByte:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               ਧਾਰਾ->pos, ਧਾਰਾ->ਮਾਪ ));

    ਵਾਪਸ ਨਤੀਜਾ;
  }


  FT_ਅਧਾਰ_DEF( FT_UInt16 )
  FT_ਧਾਰਾ_Read_UShort( FT_ਧਾਰਾ  ਧਾਰਾ,
                        FT_Error*  error )
  {
    FT_Byte    reads[2];
    FT_Byte*   p;
    FT_UInt16  ਨਤੀਜਾ = 0;


    FT_ASSERT( ਧਾਰਾ );

    ਜੇ ( ਧਾਰਾ->pos + 1 < ਧਾਰਾ->ਮਾਪ )
    {
      ਜੇ ( ਧਾਰਾ->ਪਠ )
      {
        ਜੇ ( ਧਾਰਾ->ਪਠ( ਧਾਰਾ, ਧਾਰਾ->pos, reads, 2L ) != 2L )
          ਜਾਓ Fail;

        p = reads;
      }
      ਨਹੀਂਤਾਂ
        p = ਧਾਰਾ->ਅਧਾਰ + ਧਾਰਾ->pos;

      ਜੇ ( p )
        ਨਤੀਜਾ = FT_ਅਗਲਾ_USHORT( p );
    }
    ਨਹੀਂਤਾਂ
      ਜਾਓ Fail;

    ਧਾਰਾ->pos += 2;

    *error = FT_Err_Ok;

    ਵਾਪਸ ਨਤੀਜਾ;

  Fail:
    *error = FT_THROW( Invalid_ਧਾਰਾ_Operation );
    FT_ERROR(( "FT_Stream_ReadUShort:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               ਧਾਰਾ->pos, ਧਾਰਾ->ਮਾਪ ));

    ਵਾਪਸ ਨਤੀਜਾ;
  }


  FT_ਅਧਾਰ_DEF( FT_UInt16 )
  FT_ਧਾਰਾ_Read_UShort_LE( FT_ਧਾਰਾ  ਧਾਰਾ,
                          FT_Error*  error )
  {
    FT_Byte    reads[2];
    FT_Byte*   p;
    FT_UInt16  ਨਤੀਜਾ = 0;


    FT_ASSERT( ਧਾਰਾ );

    ਜੇ ( ਧਾਰਾ->pos + 1 < ਧਾਰਾ->ਮਾਪ )
    {
      ਜੇ ( ਧਾਰਾ->ਪਠ )
      {
        ਜੇ ( ਧਾਰਾ->ਪਠ( ਧਾਰਾ, ਧਾਰਾ->pos, reads, 2L ) != 2L )
          ਜਾਓ Fail;

        p = reads;
      }
      ਨਹੀਂਤਾਂ
        p = ਧਾਰਾ->ਅਧਾਰ + ਧਾਰਾ->pos;

      ਜੇ ( p )
        ਨਤੀਜਾ = FT_ਅਗਲਾ_USHORT_LE( p );
    }
    ਨਹੀਂਤਾਂ
      ਜਾਓ Fail;

    ਧਾਰਾ->pos += 2;

    *error = FT_Err_Ok;

    ਵਾਪਸ ਨਤੀਜਾ;

  Fail:
    *error = FT_THROW( Invalid_ਧਾਰਾ_Operation );
    FT_ERROR(( "FT_Stream_ReadUShortLE:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               ਧਾਰਾ->pos, ਧਾਰਾ->ਮਾਪ ));

    ਵਾਪਸ ਨਤੀਜਾ;
  }


  FT_ਅਧਾਰ_DEF( FT_ULong )
  FT_ਧਾਰਾ_Read_UOffset( FT_ਧਾਰਾ  ਧਾਰਾ,
                         FT_Error*  error )
  {
    FT_Byte   reads[3];
    FT_Byte*  p;
    FT_ULong  ਨਤੀਜਾ = 0;


    FT_ASSERT( ਧਾਰਾ );

    ਜੇ ( ਧਾਰਾ->pos + 2 < ਧਾਰਾ->ਮਾਪ )
    {
      ਜੇ ( ਧਾਰਾ->ਪਠ )
      {
        ਜੇ (ਧਾਰਾ->ਪਠ( ਧਾਰਾ, ਧਾਰਾ->pos, reads, 3L ) != 3L )
          ਜਾਓ Fail;

        p = reads;
      }
      ਨਹੀਂਤਾਂ
        p = ਧਾਰਾ->ਅਧਾਰ + ਧਾਰਾ->pos;

      ਜੇ ( p )
        ਨਤੀਜਾ = FT_ਅਗਲਾ_UOFF3( p );
    }
    ਨਹੀਂਤਾਂ
      ਜਾਓ Fail;

    ਧਾਰਾ->pos += 3;

    *error = FT_Err_Ok;

    ਵਾਪਸ ਨਤੀਜਾ;

  Fail:
    *error = FT_THROW( Invalid_ਧਾਰਾ_Operation );
    FT_ERROR(( "FT_Stream_ReadUOffset:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               ਧਾਰਾ->pos, ਧਾਰਾ->ਮਾਪ ));

    ਵਾਪਸ ਨਤੀਜਾ;
  }


  FT_ਅਧਾਰ_DEF( FT_UInt32 )
  FT_ਧਾਰਾ_Read_ULong( FT_ਧਾਰਾ  ਧਾਰਾ,
                       FT_Error*  error )
  {
    FT_Byte   reads[4];
    FT_Byte*  p;
    FT_UInt32 ਨਤੀਜਾ = 0;


    FT_ASSERT( ਧਾਰਾ );

    ਜੇ ( ਧਾਰਾ->pos + 3 < ਧਾਰਾ->ਮਾਪ )
    {
      ਜੇ ( ਧਾਰਾ->ਪਠ )
      {
        ਜੇ ( ਧਾਰਾ->ਪਠ( ਧਾਰਾ, ਧਾਰਾ->pos, reads, 4L ) != 4L )
          ਜਾਓ Fail;

        p = reads;
      }
      ਨਹੀਂਤਾਂ
        p = ਧਾਰਾ->ਅਧਾਰ + ਧਾਰਾ->pos;

      ਜੇ ( p )
        ਨਤੀਜਾ = FT_ਅਗਲਾ_ULONG( p );
    }
    ਨਹੀਂਤਾਂ
      ਜਾਓ Fail;

    ਧਾਰਾ->pos += 4;

    *error = FT_Err_Ok;

    ਵਾਪਸ ਨਤੀਜਾ;

  Fail:
    *error = FT_THROW( Invalid_ਧਾਰਾ_Operation );
    FT_ERROR(( "FT_Stream_ReadULong:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               ਧਾਰਾ->pos, ਧਾਰਾ->ਮਾਪ ));

    ਵਾਪਸ ਨਤੀਜਾ;
  }


  FT_ਅਧਾਰ_DEF( FT_UInt32 )
  FT_ਧਾਰਾ_Read_ULong_LE( FT_ਧਾਰਾ  ਧਾਰਾ,
                         FT_Error*  error )
  {
    FT_Byte   reads[4];
    FT_Byte*  p;
    FT_UInt32 ਨਤੀਜਾ = 0;


    FT_ASSERT( ਧਾਰਾ );

    ਜੇ ( ਧਾਰਾ->pos + 3 < ਧਾਰਾ->ਮਾਪ )
    {
      ਜੇ ( ਧਾਰਾ->ਪਠ )
      {
        ਜੇ ( ਧਾਰਾ->ਪਠ( ਧਾਰਾ, ਧਾਰਾ->pos, reads, 4L ) != 4L )
          ਜਾਓ Fail;

        p = reads;
      }
      ਨਹੀਂਤਾਂ
        p = ਧਾਰਾ->ਅਧਾਰ + ਧਾਰਾ->pos;

      ਜੇ ( p )
        ਨਤੀਜਾ = FT_ਅਗਲਾ_ULONG_LE( p );
    }
    ਨਹੀਂਤਾਂ
      ਜਾਓ Fail;

    ਧਾਰਾ->pos += 4;

    *error = FT_Err_Ok;

    ਵਾਪਸ ਨਤੀਜਾ;

  Fail:
    *error = FT_THROW( Invalid_ਧਾਰਾ_Operation );
    FT_ERROR(( "FT_Stream_ReadULongLE:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               ਧਾਰਾ->pos, ਧਾਰਾ->ਮਾਪ ));

    ਵਾਪਸ ਨਤੀਜਾ;
  }


  FT_ਅਧਾਰ_DEF( FT_Error )
  FT_ਧਾਰਾ_Read_Fields( FT_ਧਾਰਾ              ਧਾਰਾ,
                        ਅਚਲ FT_ਚੌਖਟਾ_ਖੇਤਰ*  fields,
                        ਖਾਲੀ*                  structure )
  {
    FT_Error  error;
    FT_Bool   ਚੌਖਟਾ_accessed = 0;
    FT_Byte*  cursor;


    ਜੇ ( !fields )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    ਜੇ ( !ਧਾਰਾ )
      ਵਾਪਸ FT_THROW( Invalid_ਧਾਰਾ_Handle );

    cursor = ਧਾਰਾ->cursor;

    error = FT_Err_Ok;
    ਕਰੋ
    {
      FT_ULong  ਮੁੱਲ;
      FT_Int    sign_ਖਿਸਕਾਓ;
      FT_Byte*  p;


      ਚੋਣ ( fields->ਮੁੱਲ )
      {
      ਹਾਲਤ ft_ਚੌਖਟਾ_ਸ਼ੁਰੂ:  /* access a new frame */
        error = FT_ਧਾਰਾ_Enter_ਚੌਖਟਾ( ਧਾਰਾ, fields->ਵਿੱਥ );
        ਜੇ ( error )
          ਜਾਓ Exit;

        ਚੌਖਟਾ_accessed = 1;
        cursor         = ਧਾਰਾ->cursor;
        fields++;
        ਜਾਰੀ;  /* loop! */

      ਹਾਲਤ ft_ਚੌਖਟਾ_ਬਾਈਟਾਂ:  /* read a byte sequence */
      ਹਾਲਤ ft_ਚੌਖਟਾ_skip:   /* skip some bytes      */
        {
          FT_ਵਿੱਥ_2  len = fields->ਮਾਪ;


          ਜੇ ( len > (FT_ਵਿੱਥ_2)( ਧਾਰਾ->ਹੱਦਬੰਦੀ - cursor ) )
          {
            error = FT_THROW( Invalid_ਧਾਰਾ_Operation );
            ਜਾਓ Exit;
          }

          ਜੇ ( fields->ਮੁੱਲ == ft_ਚੌਖਟਾ_ਬਾਈਟਾਂ )
          {
            p = (FT_Byte*)structure + fields->ਵਿੱਥ;
            FT_MEM_COPY( p, cursor, len );
          }
          cursor += len;
          fields++;
          ਜਾਰੀ;
        }

      ਹਾਲਤ ft_ਚੌਖਟਾ_byte:
      ਹਾਲਤ ft_ਚੌਖਟਾ_schar:  /* read a single byte */
        ਮੁੱਲ = FT_ਅਗਲਾ_BYTE( cursor );
        sign_ਖਿਸਕਾਓ = 24;
        ਰੋਕੋ;

      ਹਾਲਤ ft_ਚੌਖਟਾ_short_be:
      ਹਾਲਤ ft_ਚੌਖਟਾ_ushort_be:  /* read a 2-byte big-endian short */
        ਮੁੱਲ = FT_ਅਗਲਾ_USHORT( cursor );
        sign_ਖਿਸਕਾਓ = 16;
        ਰੋਕੋ;

      ਹਾਲਤ ft_ਚੌਖਟਾ_short_le:
      ਹਾਲਤ ft_ਚੌਖਟਾ_ushort_le:  /* read a 2-byte little-endian short */
        ਮੁੱਲ = FT_ਅਗਲਾ_USHORT_LE( cursor );
        sign_ਖਿਸਕਾਓ = 16;
        ਰੋਕੋ;

      ਹਾਲਤ ft_ਚੌਖਟਾ_long_be:
      ਹਾਲਤ ft_ਚੌਖਟਾ_ulong_be:  /* read a 4-byte big-endian long */
        ਮੁੱਲ = FT_ਅਗਲਾ_ULONG( cursor );
        sign_ਖਿਸਕਾਓ = 0;
        ਰੋਕੋ;

      ਹਾਲਤ ft_ਚੌਖਟਾ_long_le:
      ਹਾਲਤ ft_ਚੌਖਟਾ_ulong_le:  /* read a 4-byte little-endian long */
        ਮੁੱਲ = FT_ਅਗਲਾ_ULONG_LE( cursor );
        sign_ਖਿਸਕਾਓ = 0;
        ਰੋਕੋ;

      ਹਾਲਤ ft_ਚੌਖਟਾ_off3_be:
      ਹਾਲਤ ft_ਚੌਖਟਾ_uoff3_be:  /* read a 3-byte big-endian long */
        ਮੁੱਲ = FT_ਅਗਲਾ_UOFF3( cursor );
        sign_ਖਿਸਕਾਓ = 8;
        ਰੋਕੋ;

      ਹਾਲਤ ft_ਚੌਖਟਾ_off3_le:
      ਹਾਲਤ ft_ਚੌਖਟਾ_uoff3_le:  /* read a 3-byte little-endian long */
        ਮੁੱਲ = FT_ਅਗਲਾ_UOFF3_LE( cursor );
        sign_ਖਿਸਕਾਓ = 8;
        ਰੋਕੋ;

      ਮੂਲ:
        /* otherwise, exit the loop */
        ਧਾਰਾ->cursor = cursor;
        ਜਾਓ Exit;
      }

      /* now, compute the signed value if necessary */
      ਜੇ ( fields->ਮੁੱਲ & FT_ਚੌਖਟਾ_OP_SIGNED )
        ਮੁੱਲ = (FT_ULong)( (FT_Int32)( ਮੁੱਲ << sign_ਖਿਸਕਾਓ ) >> sign_ਖਿਸਕਾਓ );

      /* finally, store the value in the object */

      p = (FT_Byte*)structure + fields->ਵਿੱਥ;
      ਚੋਣ ( fields->ਮਾਪ )
      {
      ਹਾਲਤ ( 8 / FT_CHAR_ਦੋਆਧਾਰੀ_ਅੰਕ ):
        *(FT_Byte*)p = (FT_Byte)ਮੁੱਲ;
        ਰੋਕੋ;

      ਹਾਲਤ ( 16 / FT_CHAR_ਦੋਆਧਾਰੀ_ਅੰਕ ):
        *(FT_UShort*)p = (FT_UShort)ਮੁੱਲ;
        ਰੋਕੋ;

      ਹਾਲਤ ( 32 / FT_CHAR_ਦੋਆਧਾਰੀ_ਅੰਕ ):
        *(FT_UInt32*)p = (FT_UInt32)ਮੁੱਲ;
        ਰੋਕੋ;

      ਮੂਲ:  /* for 64-bit systems */
        *(FT_ULong*)p = (FT_ULong)ਮੁੱਲ;
      }

      /* go to next field */
      fields++;
    }
    ਜਦੋਂ ( 1 );

  Exit:
    /* close the frame if it was opened by this read */
    ਜੇ ( ਚੌਖਟਾ_accessed )
      FT_ਧਾਰਾ_Exit_ਚੌਖਟਾ( ਧਾਰਾ );

    ਵਾਪਸ error;
  }


/*ਸਿਰਾ*/
