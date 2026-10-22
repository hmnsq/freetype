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
#undef  FT_COMPONENT
#define FT_COMPONENT  stream


  FT_BASE_DEF( ಶೂನ್ಯ )
  FT_Stream_OpenMemory( FT_Stream       stream,
                        ಧ್ರುವ FT_Byte*  base,
                        FT_ULong        size )
  {
    stream->base   = (FT_Byte*) base;
    stream->size   = size;
    stream->pos    = 0;
    stream->cursor = ರಿಕ್ತ;
    stream->ಪಠ   = ರಿಕ್ತ;
    stream->close  = ರಿಕ್ತ;
  }


  FT_BASE_DEF( ಶೂನ್ಯ )
  FT_Stream_Close( FT_Stream  stream )
  {
    ಯದಿ ( stream && stream->close )
      stream->close( stream );
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_Seek( FT_Stream  stream,
                  FT_ULong   pos )
  {
    FT_Error  error = FT_Err_Ok;


    ಯದಿ ( stream->ಪಠ )
    {
      ಯದಿ ( stream->ಪಠ( stream, pos, ರಿಕ್ತ, 0 ) )
      {
        FT_ERROR(( "FT_Stream_Seek:"
                   " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
                   pos, stream->size ));

        error = FT_THROW( Invalid_Stream_Operation );
      }
    }
    /* note that seeking to the first position after the file is valid */
    ಅನ್ಯಥಾ ಯದಿ ( pos > stream->size )
    {
      FT_ERROR(( "FT_Stream_Seek:"
                 " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
                 pos, stream->size ));

      error = FT_THROW( Invalid_Stream_Operation );
    }

    ಯದಿ ( !error )
      stream->pos = pos;

    ಪ್ರತ್ಯಾಗಮ error;
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_Skip( FT_Stream  stream,
                  FT_Long    distance )
  {
    ಯದಿ ( distance < 0 )
      ಪ್ರತ್ಯಾಗಮ FT_THROW( Invalid_Stream_Operation );

    ಪ್ರತ್ಯಾಗಮ FT_Stream_Seek( stream, stream->pos + (FT_ULong)distance );
  }


  FT_BASE_DEF( FT_ULong )
  FT_Stream_Pos( FT_Stream  stream )
  {
    ಪ್ರತ್ಯಾಗಮ stream->pos;
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_Read( FT_Stream  stream,
                  FT_Byte*   buffer,
                  FT_ULong   ಎಣಿಕೆ )
  {
    ಪ್ರತ್ಯಾಗಮ FT_Stream_ReadAt( stream, stream->pos, buffer, ಎಣಿಕೆ );
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_ReadAt( FT_Stream  stream,
                    FT_ULong   pos,
                    FT_Byte*   buffer,
                    FT_ULong   ಎಣಿಕೆ )
  {
    FT_Error  error = FT_Err_Ok;
    FT_ULong  read_bytes;


    ಯದಿ ( pos >= stream->size )
    {
      FT_ERROR(( "FT_Stream_ReadAt:"
                 " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
                 pos, stream->size ));

      ಪ್ರತ್ಯಾಗಮ FT_THROW( Invalid_Stream_Operation );
    }

    ಯದಿ ( stream->ಪಠ )
      read_bytes = stream->ಪಠ( stream, pos, buffer, ಎಣಿಕೆ );
    ಅನ್ಯಥಾ
    {
      read_bytes = stream->size - pos;
      ಯದಿ ( read_bytes > ಎಣಿಕೆ )
        read_bytes = ಎಣಿಕೆ;

      /* Allow "reading" zero bytes without UB even if buffer is NULL */
      ಯದಿ ( ಎಣಿಕೆ )
        FT_MEM_COPY( buffer, stream->base + pos, read_bytes );
    }

    stream->pos = pos + read_bytes;

    ಯದಿ ( read_bytes < ಎಣಿಕೆ )
    {
      FT_ERROR(( "FT_Stream_ReadAt:"
                 " invalid read; expected %lu bytes, got %lu\n",
                 ಎಣಿಕೆ, read_bytes ));

      error = FT_THROW( Invalid_Stream_Operation );
    }

    ಪ್ರತ್ಯಾಗಮ error;
  }


  FT_BASE_DEF( FT_ULong )
  FT_Stream_TryRead( FT_Stream  stream,
                     FT_Byte*   buffer,
                     FT_ULong   ಎಣಿಕೆ )
  {
    FT_ULong  read_bytes = 0;


    ಯದಿ ( stream->pos >= stream->size )
      ಗಚ್ಛ Exit;

    ಯದಿ ( stream->ಪಠ )
      read_bytes = stream->ಪಠ( stream, stream->pos, buffer, ಎಣಿಕೆ );
    ಅನ್ಯಥಾ
    {
      read_bytes = stream->size - stream->pos;
      ಯದಿ ( read_bytes > ಎಣಿಕೆ )
        read_bytes = ಎಣಿಕೆ;

      /* Allow "reading" zero bytes without UB even if buffer is NULL */
      ಯದಿ ( ಎಣಿಕೆ )
        FT_MEM_COPY( buffer, stream->base + stream->pos, read_bytes );
    }

    stream->pos += read_bytes;

  Exit:
    ಪ್ರತ್ಯಾಗಮ read_bytes;
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_ExtractFrame( FT_Stream  stream,
                          FT_ULong   ಎಣಿಕೆ,
                          FT_Byte**  pbytes )
  {
    FT_Error  error;


    error = FT_Stream_EnterFrame( stream, ಎಣಿಕೆ );
    ಯದಿ ( !error )
    {
      *pbytes = (FT_Byte*)stream->cursor;

      /* equivalent to FT_Stream_ExitFrame(), with no memory block release */
      stream->cursor = ರಿಕ್ತ;
      stream->limit  = ರಿಕ್ತ;
    }

    ಪ್ರತ್ಯಾಗಮ error;
  }


  FT_BASE_DEF( ಶೂನ್ಯ )
  FT_Stream_ReleaseFrame( FT_Stream  stream,
                          FT_Byte**  pbytes )
  {
    ಯದಿ ( stream && stream->ಪಠ )
    {
      FT_Memory  memory = stream->memory;


#ifdef FT_DEBUG_MEMORY
      ft_mem_free( memory, *pbytes );
#ಅನ್ಯಥಾ
      FT_FREE( *pbytes );
#endif
    }

    *pbytes = ರಿಕ್ತ;
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_EnterFrame( FT_Stream  stream,
                        FT_ULong   ಎಣಿಕೆ )
  {
    FT_Error  error = FT_Err_Ok;
    FT_ULong  read_bytes;


    FT_TRACE7(( "FT_Stream_EnterFrame: %lu bytes\n", ಎಣಿಕೆ ));

    /* check for nested frame access */
    FT_ASSERT( stream && stream->cursor == 0 );

    ಯದಿ ( stream->ಪಠ )
    {
      /* allocate the frame in memory */
      FT_Memory  memory = stream->memory;


      /* simple sanity check */
      ಯದಿ ( ಎಣಿಕೆ > stream->size )
      {
        FT_ERROR(( "FT_Stream_EnterFrame:"
                   " frame size (%lu) larger than stream size (%lu)\n",
                   ಎಣಿಕೆ, stream->size ));

        error = FT_THROW( Invalid_Stream_Operation );
        ಗಚ್ಛ Exit;
      }

#ifdef FT_DEBUG_MEMORY
      /* assume `ft_debug_file_` and `ft_debug_lineno_` are already set */
      stream->base = (unsigned ವರ್ಣ*)ft_mem_qalloc( memory,
                                                    (FT_Long)ಎಣಿಕೆ,
                                                    &error );
      ಯದಿ ( error )
        ಗಚ್ಛ Exit;
#ಅನ್ಯಥಾ
      ಯದಿ ( FT_QALLOC( stream->base, ಎಣಿಕೆ ) )
        ಗಚ್ಛ Exit;
#endif
      /* read it */
      read_bytes = stream->ಪಠ( stream, stream->pos,
                                 stream->base, ಎಣಿಕೆ );
      ಯದಿ ( read_bytes < ಎಣಿಕೆ )
      {
        FT_ERROR(( "FT_Stream_EnterFrame:"
                   " invalid read; expected %lu bytes, got %lu\n",
                   ಎಣಿಕೆ, read_bytes ));

        FT_FREE( stream->base );
        error = FT_THROW( Invalid_Stream_Operation );
      }

      stream->cursor = stream->base;
      stream->limit  = FT_OFFSET( stream->cursor, ಎಣಿಕೆ );
      stream->pos   += read_bytes;
    }
    ಅನ್ಯಥಾ
    {
      /* check current and new position */
      ಯದಿ ( stream->pos >= stream->size        ||
           stream->size - stream->pos < ಎಣಿಕೆ )
      {
        FT_ERROR(( "FT_Stream_EnterFrame:"
                   " invalid i/o; pos = 0x%lx, count = %lu, size = 0x%lx\n",
                   stream->pos, ಎಣಿಕೆ, stream->size ));

        error = FT_THROW( Invalid_Stream_Operation );
        ಗಚ್ಛ Exit;
      }

      /* set cursor */
      stream->cursor = stream->base + stream->pos;
      stream->limit  = stream->cursor + ಎಣಿಕೆ;
      stream->pos   += ಎಣಿಕೆ;
    }

  Exit:
    ಪ್ರತ್ಯಾಗಮ error;
  }


  FT_BASE_DEF( ಶೂನ್ಯ )
  FT_Stream_ExitFrame( FT_Stream  stream )
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

    FT_ASSERT( stream );

    ಯದಿ ( stream->ಪಠ )
    {
      FT_Memory  memory = stream->memory;


#ifdef FT_DEBUG_MEMORY
      ft_mem_free( memory, stream->base );
      stream->base = ರಿಕ್ತ;
#ಅನ್ಯಥಾ
      FT_FREE( stream->base );
#endif
    }

    stream->cursor = ರಿಕ್ತ;
    stream->limit  = ರಿಕ್ತ;
  }


  FT_BASE_DEF( FT_Byte )
  FT_Stream_GetByte( FT_Stream  stream )
  {
    FT_Byte  result;


    FT_ASSERT( stream && stream->cursor );

    result = 0;
    ಯದಿ ( stream->cursor < stream->limit )
      result = *stream->cursor++;

    ಪ್ರತ್ಯಾಗಮ result;
  }


  FT_BASE_DEF( FT_UInt16 )
  FT_Stream_GetUShort( FT_Stream  stream )
  {
    FT_Byte*   p;
    FT_UInt16  result;


    FT_ASSERT( stream && stream->cursor );

    result         = 0;
    p              = stream->cursor;
    ಯದಿ ( p + 1 < stream->limit )
      result       = FT_NEXT_USHORT( p );
    stream->cursor = p;

    ಪ್ರತ್ಯಾಗಮ result;
  }


  FT_BASE_DEF( FT_UInt16 )
  FT_Stream_GetUShortLE( FT_Stream  stream )
  {
    FT_Byte*   p;
    FT_UInt16  result;


    FT_ASSERT( stream && stream->cursor );

    result         = 0;
    p              = stream->cursor;
    ಯದಿ ( p + 1 < stream->limit )
      result       = FT_NEXT_USHORT_LE( p );
    stream->cursor = p;

    ಪ್ರತ್ಯಾಗಮ result;
  }


  FT_BASE_DEF( FT_UInt32 )
  FT_Stream_GetUOffset( FT_Stream  stream )
  {
    FT_Byte*  p;
    FT_UInt32 result;


    FT_ASSERT( stream && stream->cursor );

    result         = 0;
    p              = stream->cursor;
    ಯದಿ ( p + 2 < stream->limit )
      result       = FT_NEXT_UOFF3( p );
    stream->cursor = p;
    ಪ್ರತ್ಯಾಗಮ result;
  }


  FT_BASE_DEF( FT_UInt32 )
  FT_Stream_GetULong( FT_Stream  stream )
  {
    FT_Byte*  p;
    FT_UInt32 result;


    FT_ASSERT( stream && stream->cursor );

    result         = 0;
    p              = stream->cursor;
    ಯದಿ ( p + 3 < stream->limit )
      result       = FT_NEXT_ULONG( p );
    stream->cursor = p;
    ಪ್ರತ್ಯಾಗಮ result;
  }


  FT_BASE_DEF( FT_UInt32 )
  FT_Stream_GetULongLE( FT_Stream  stream )
  {
    FT_Byte*  p;
    FT_UInt32 result;


    FT_ASSERT( stream && stream->cursor );

    result         = 0;
    p              = stream->cursor;
    ಯದಿ ( p + 3 < stream->limit )
      result       = FT_NEXT_ULONG_LE( p );
    stream->cursor = p;
    ಪ್ರತ್ಯಾಗಮ result;
  }


  FT_BASE_DEF( FT_Byte )
  FT_Stream_ReadByte( FT_Stream  stream,
                      FT_Error*  error )
  {
    FT_Byte  result = 0;


    FT_ASSERT( stream );

    ಯದಿ ( stream->pos < stream->size )
    {
      ಯದಿ ( stream->ಪಠ )
      {
        ಯದಿ ( stream->ಪಠ( stream, stream->pos, &result, 1L ) != 1L )
          ಗಚ್ಛ Fail;
      }
      ಅನ್ಯಥಾ
        result = stream->base[stream->pos];
    }
    ಅನ್ಯಥಾ
      ಗಚ್ಛ Fail;

    stream->pos++;

    *error = FT_Err_Ok;

    ಪ್ರತ್ಯಾಗಮ result;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadByte:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    ಪ್ರತ್ಯಾಗಮ result;
  }


  FT_BASE_DEF( FT_UInt16 )
  FT_Stream_ReadUShort( FT_Stream  stream,
                        FT_Error*  error )
  {
    FT_Byte    reads[2];
    FT_Byte*   p;
    FT_UInt16  result = 0;


    FT_ASSERT( stream );

    ಯದಿ ( stream->pos + 1 < stream->size )
    {
      ಯದಿ ( stream->ಪಠ )
      {
        ಯದಿ ( stream->ಪಠ( stream, stream->pos, reads, 2L ) != 2L )
          ಗಚ್ಛ Fail;

        p = reads;
      }
      ಅನ್ಯಥಾ
        p = stream->base + stream->pos;

      ಯದಿ ( p )
        result = FT_NEXT_USHORT( p );
    }
    ಅನ್ಯಥಾ
      ಗಚ್ಛ Fail;

    stream->pos += 2;

    *error = FT_Err_Ok;

    ಪ್ರತ್ಯಾಗಮ result;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadUShort:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    ಪ್ರತ್ಯಾಗಮ result;
  }


  FT_BASE_DEF( FT_UInt16 )
  FT_Stream_ReadUShortLE( FT_Stream  stream,
                          FT_Error*  error )
  {
    FT_Byte    reads[2];
    FT_Byte*   p;
    FT_UInt16  result = 0;


    FT_ASSERT( stream );

    ಯದಿ ( stream->pos + 1 < stream->size )
    {
      ಯದಿ ( stream->ಪಠ )
      {
        ಯದಿ ( stream->ಪಠ( stream, stream->pos, reads, 2L ) != 2L )
          ಗಚ್ಛ Fail;

        p = reads;
      }
      ಅನ್ಯಥಾ
        p = stream->base + stream->pos;

      ಯದಿ ( p )
        result = FT_NEXT_USHORT_LE( p );
    }
    ಅನ್ಯಥಾ
      ಗಚ್ಛ Fail;

    stream->pos += 2;

    *error = FT_Err_Ok;

    ಪ್ರತ್ಯಾಗಮ result;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadUShortLE:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    ಪ್ರತ್ಯಾಗಮ result;
  }


  FT_BASE_DEF( FT_ULong )
  FT_Stream_ReadUOffset( FT_Stream  stream,
                         FT_Error*  error )
  {
    FT_Byte   reads[3];
    FT_Byte*  p;
    FT_ULong  result = 0;


    FT_ASSERT( stream );

    ಯದಿ ( stream->pos + 2 < stream->size )
    {
      ಯದಿ ( stream->ಪಠ )
      {
        ಯದಿ (stream->ಪಠ( stream, stream->pos, reads, 3L ) != 3L )
          ಗಚ್ಛ Fail;

        p = reads;
      }
      ಅನ್ಯಥಾ
        p = stream->base + stream->pos;

      ಯದಿ ( p )
        result = FT_NEXT_UOFF3( p );
    }
    ಅನ್ಯಥಾ
      ಗಚ್ಛ Fail;

    stream->pos += 3;

    *error = FT_Err_Ok;

    ಪ್ರತ್ಯಾಗಮ result;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadUOffset:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    ಪ್ರತ್ಯಾಗಮ result;
  }


  FT_BASE_DEF( FT_UInt32 )
  FT_Stream_ReadULong( FT_Stream  stream,
                       FT_Error*  error )
  {
    FT_Byte   reads[4];
    FT_Byte*  p;
    FT_UInt32 result = 0;


    FT_ASSERT( stream );

    ಯದಿ ( stream->pos + 3 < stream->size )
    {
      ಯದಿ ( stream->ಪಠ )
      {
        ಯದಿ ( stream->ಪಠ( stream, stream->pos, reads, 4L ) != 4L )
          ಗಚ್ಛ Fail;

        p = reads;
      }
      ಅನ್ಯಥಾ
        p = stream->base + stream->pos;

      ಯದಿ ( p )
        result = FT_NEXT_ULONG( p );
    }
    ಅನ್ಯಥಾ
      ಗಚ್ಛ Fail;

    stream->pos += 4;

    *error = FT_Err_Ok;

    ಪ್ರತ್ಯಾಗಮ result;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadULong:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    ಪ್ರತ್ಯಾಗಮ result;
  }


  FT_BASE_DEF( FT_UInt32 )
  FT_Stream_ReadULongLE( FT_Stream  stream,
                         FT_Error*  error )
  {
    FT_Byte   reads[4];
    FT_Byte*  p;
    FT_UInt32 result = 0;


    FT_ASSERT( stream );

    ಯದಿ ( stream->pos + 3 < stream->size )
    {
      ಯದಿ ( stream->ಪಠ )
      {
        ಯದಿ ( stream->ಪಠ( stream, stream->pos, reads, 4L ) != 4L )
          ಗಚ್ಛ Fail;

        p = reads;
      }
      ಅನ್ಯಥಾ
        p = stream->base + stream->pos;

      ಯದಿ ( p )
        result = FT_NEXT_ULONG_LE( p );
    }
    ಅನ್ಯಥಾ
      ಗಚ್ಛ Fail;

    stream->pos += 4;

    *error = FT_Err_Ok;

    ಪ್ರತ್ಯಾಗಮ result;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadULongLE:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    ಪ್ರತ್ಯಾಗಮ result;
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_ReadFields( FT_Stream              stream,
                        ಧ್ರುವ FT_Frame_Field*  fields,
                        ಶೂನ್ಯ*                  structure )
  {
    FT_Error  error;
    FT_Bool   frame_accessed = 0;
    FT_Byte*  cursor;


    ಯದಿ ( !fields )
      ಪ್ರತ್ಯಾಗಮ FT_THROW( Invalid_Argument );

    ಯದಿ ( !stream )
      ಪ್ರತ್ಯಾಗಮ FT_THROW( Invalid_Stream_Handle );

    cursor = stream->cursor;

    error = FT_Err_Ok;
    ಕುರು
    {
      FT_ULong  ಮೌಲ್ಯ;
      FT_Int    sign_shift;
      FT_Byte*  p;


      ವಿಕಲ್ಪ ( fields->ಮೌಲ್ಯ )
      {
      ಪಕ್ಷ ft_frame_start:  /* access a new frame */
        error = FT_Stream_EnterFrame( stream, fields->offset );
        ಯದಿ ( error )
          ಗಚ್ಛ Exit;

        frame_accessed = 1;
        cursor         = stream->cursor;
        fields++;
        ಅನುವರ್ತ;  /* loop! */

      ಪಕ್ಷ ft_frame_bytes:  /* read a byte sequence */
      ಪಕ್ಷ ft_frame_skip:   /* skip some bytes      */
        {
          FT_Offset  len = fields->size;


          ಯದಿ ( len > (FT_Offset)( stream->limit - cursor ) )
          {
            error = FT_THROW( Invalid_Stream_Operation );
            ಗಚ್ಛ Exit;
          }

          ಯದಿ ( fields->ಮೌಲ್ಯ == ft_frame_bytes )
          {
            p = (FT_Byte*)structure + fields->offset;
            FT_MEM_COPY( p, cursor, len );
          }
          cursor += len;
          fields++;
          ಅನುವರ್ತ;
        }

      ಪಕ್ಷ ft_frame_byte:
      ಪಕ್ಷ ft_frame_schar:  /* read a single byte */
        ಮೌಲ್ಯ = FT_NEXT_BYTE( cursor );
        sign_shift = 24;
        ಭಙ್ಗ;

      ಪಕ್ಷ ft_frame_short_be:
      ಪಕ್ಷ ft_frame_ushort_be:  /* read a 2-byte big-endian short */
        ಮೌಲ್ಯ = FT_NEXT_USHORT( cursor );
        sign_shift = 16;
        ಭಙ್ಗ;

      ಪಕ್ಷ ft_frame_short_le:
      ಪಕ್ಷ ft_frame_ushort_le:  /* read a 2-byte little-endian short */
        ಮೌಲ್ಯ = FT_NEXT_USHORT_LE( cursor );
        sign_shift = 16;
        ಭಙ್ಗ;

      ಪಕ್ಷ ft_frame_long_be:
      ಪಕ್ಷ ft_frame_ulong_be:  /* read a 4-byte big-endian long */
        ಮೌಲ್ಯ = FT_NEXT_ULONG( cursor );
        sign_shift = 0;
        ಭಙ್ಗ;

      ಪಕ್ಷ ft_frame_long_le:
      ಪಕ್ಷ ft_frame_ulong_le:  /* read a 4-byte little-endian long */
        ಮೌಲ್ಯ = FT_NEXT_ULONG_LE( cursor );
        sign_shift = 0;
        ಭಙ್ಗ;

      ಪಕ್ಷ ft_frame_off3_be:
      ಪಕ್ಷ ft_frame_uoff3_be:  /* read a 3-byte big-endian long */
        ಮೌಲ್ಯ = FT_NEXT_UOFF3( cursor );
        sign_shift = 8;
        ಭಙ್ಗ;

      ಪಕ್ಷ ft_frame_off3_le:
      ಪಕ್ಷ ft_frame_uoff3_le:  /* read a 3-byte little-endian long */
        ಮೌಲ್ಯ = FT_NEXT_UOFF3_LE( cursor );
        sign_shift = 8;
        ಭಙ್ಗ;

      ಸ್ವಭಾವ:
        /* otherwise, exit the loop */
        stream->cursor = cursor;
        ಗಚ್ಛ Exit;
      }

      /* now, compute the signed value if necessary */
      ಯದಿ ( fields->ಮೌಲ್ಯ & FT_FRAME_OP_SIGNED )
        ಮೌಲ್ಯ = (FT_ULong)( (FT_Int32)( ಮೌಲ್ಯ << sign_shift ) >> sign_shift );

      /* finally, store the value in the object */

      p = (FT_Byte*)structure + fields->offset;
      ವಿಕಲ್ಪ ( fields->size )
      {
      ಪಕ್ಷ ( 8 / FT_CHAR_BIT ):
        *(FT_Byte*)p = (FT_Byte)ಮೌಲ್ಯ;
        ಭಙ್ಗ;

      ಪಕ್ಷ ( 16 / FT_CHAR_BIT ):
        *(FT_UShort*)p = (FT_UShort)ಮೌಲ್ಯ;
        ಭಙ್ಗ;

      ಪಕ್ಷ ( 32 / FT_CHAR_BIT ):
        *(FT_UInt32*)p = (FT_UInt32)ಮೌಲ್ಯ;
        ಭಙ್ಗ;

      ಸ್ವಭಾವ:  /* for 64-bit systems */
        *(FT_ULong*)p = (FT_ULong)ಮೌಲ್ಯ;
      }

      /* go to next field */
      fields++;
    }
    ಯಾವತ್ ( 1 );

  Exit:
    /* close the frame if it was opened by this read */
    ಯದಿ ( frame_accessed )
      FT_Stream_ExitFrame( stream );

    ಪ್ರತ್ಯಾಗಮ error;
  }


/* END */
