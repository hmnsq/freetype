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


#അന്തര്ഭാവ <freetype/internal/ftstream.h>
#അന്തര്ഭാവ <freetype/internal/ftdebug.h>


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#undef  FT_COMPONENT
#define FT_COMPONENT  stream


  FT_BASE_DEF( ശൂന്യം )
  FT_Stream_OpenMemory( FT_Stream       stream,
                        സ്ഥിരം FT_Byte*  base,
                        FT_ULong        size )
  {
    stream->base   = (FT_Byte*) base;
    stream->size   = size;
    stream->pos    = 0;
    stream->cursor = ശൂന്യ;
    stream->പഠ   = ശൂന്യ;
    stream->close  = ശൂന്യ;
  }


  FT_BASE_DEF( ശൂന്യം )
  FT_Stream_Close( FT_Stream  stream )
  {
    എങ്കിൽ ( stream && stream->close )
      stream->close( stream );
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_Seek( FT_Stream  stream,
                  FT_ULong   pos )
  {
    FT_Error  error = FT_Err_Ok;


    എങ്കിൽ ( stream->പഠ )
    {
      എങ്കിൽ ( stream->പഠ( stream, pos, ശൂന്യ, 0 ) )
      {
        FT_ERROR(( "FT_Stream_Seek:"
                   " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
                   pos, stream->size ));

        error = FT_THROW( Invalid_Stream_Operation );
      }
    }
    /* note that seeking to the first position after the file is valid */
    അല്ലെങ്കിൽ എങ്കിൽ ( pos > stream->size )
    {
      FT_ERROR(( "FT_Stream_Seek:"
                 " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
                 pos, stream->size ));

      error = FT_THROW( Invalid_Stream_Operation );
    }

    എങ്കിൽ ( !error )
      stream->pos = pos;

    തിരികെ error;
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_Skip( FT_Stream  stream,
                  FT_Long    distance )
  {
    എങ്കിൽ ( distance < 0 )
      തിരികെ FT_THROW( Invalid_Stream_Operation );

    തിരികെ FT_Stream_Seek( stream, stream->pos + (FT_ULong)distance );
  }


  FT_BASE_DEF( FT_ULong )
  FT_Stream_Pos( FT_Stream  stream )
  {
    തിരികെ stream->pos;
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_Read( FT_Stream  stream,
                  FT_Byte*   buffer,
                  FT_ULong   എണ്ണം )
  {
    തിരികെ FT_Stream_ReadAt( stream, stream->pos, buffer, എണ്ണം );
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_ReadAt( FT_Stream  stream,
                    FT_ULong   pos,
                    FT_Byte*   buffer,
                    FT_ULong   എണ്ണം )
  {
    FT_Error  error = FT_Err_Ok;
    FT_ULong  read_bytes;


    എങ്കിൽ ( pos >= stream->size )
    {
      FT_ERROR(( "FT_Stream_ReadAt:"
                 " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
                 pos, stream->size ));

      തിരികെ FT_THROW( Invalid_Stream_Operation );
    }

    എങ്കിൽ ( stream->പഠ )
      read_bytes = stream->പഠ( stream, pos, buffer, എണ്ണം );
    അല്ലെങ്കിൽ
    {
      read_bytes = stream->size - pos;
      എങ്കിൽ ( read_bytes > എണ്ണം )
        read_bytes = എണ്ണം;

      /* Allow "reading" zero bytes without UB even if buffer is NULL */
      എങ്കിൽ ( എണ്ണം )
        FT_MEM_COPY( buffer, stream->base + pos, read_bytes );
    }

    stream->pos = pos + read_bytes;

    എങ്കിൽ ( read_bytes < എണ്ണം )
    {
      FT_ERROR(( "FT_Stream_ReadAt:"
                 " invalid read; expected %lu bytes, got %lu\n",
                 എണ്ണം, read_bytes ));

      error = FT_THROW( Invalid_Stream_Operation );
    }

    തിരികെ error;
  }


  FT_BASE_DEF( FT_ULong )
  FT_Stream_TryRead( FT_Stream  stream,
                     FT_Byte*   buffer,
                     FT_ULong   എണ്ണം )
  {
    FT_ULong  read_bytes = 0;


    എങ്കിൽ ( stream->pos >= stream->size )
      പോകുക Exit;

    എങ്കിൽ ( stream->പഠ )
      read_bytes = stream->പഠ( stream, stream->pos, buffer, എണ്ണം );
    അല്ലെങ്കിൽ
    {
      read_bytes = stream->size - stream->pos;
      എങ്കിൽ ( read_bytes > എണ്ണം )
        read_bytes = എണ്ണം;

      /* Allow "reading" zero bytes without UB even if buffer is NULL */
      എങ്കിൽ ( എണ്ണം )
        FT_MEM_COPY( buffer, stream->base + stream->pos, read_bytes );
    }

    stream->pos += read_bytes;

  Exit:
    തിരികെ read_bytes;
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_ExtractFrame( FT_Stream  stream,
                          FT_ULong   എണ്ണം,
                          FT_Byte**  pbytes )
  {
    FT_Error  error;


    error = FT_Stream_EnterFrame( stream, എണ്ണം );
    എങ്കിൽ ( !error )
    {
      *pbytes = (FT_Byte*)stream->cursor;

      /* equivalent to FT_Stream_ExitFrame(), with no memory block release */
      stream->cursor = ശൂന്യ;
      stream->limit  = ശൂന്യ;
    }

    തിരികെ error;
  }


  FT_BASE_DEF( ശൂന്യം )
  FT_Stream_ReleaseFrame( FT_Stream  stream,
                          FT_Byte**  pbytes )
  {
    എങ്കിൽ ( stream && stream->പഠ )
    {
      FT_Memory  memory = stream->memory;


#ifdef FT_DEBUG_MEMORY
      ft_mem_free( memory, *pbytes );
#അല്ലെങ്കിൽ
      FT_FREE( *pbytes );
#endif
    }

    *pbytes = ശൂന്യ;
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_EnterFrame( FT_Stream  stream,
                        FT_ULong   എണ്ണം )
  {
    FT_Error  error = FT_Err_Ok;
    FT_ULong  read_bytes;


    FT_TRACE7(( "FT_Stream_EnterFrame: %lu bytes\n", എണ്ണം ));

    /* check for nested frame access */
    FT_ASSERT( stream && stream->cursor == 0 );

    എങ്കിൽ ( stream->പഠ )
    {
      /* allocate the frame in memory */
      FT_Memory  memory = stream->memory;


      /* simple sanity check */
      എങ്കിൽ ( എണ്ണം > stream->size )
      {
        FT_ERROR(( "FT_Stream_EnterFrame:"
                   " frame size (%lu) larger than stream size (%lu)\n",
                   എണ്ണം, stream->size ));

        error = FT_THROW( Invalid_Stream_Operation );
        പോകുക Exit;
      }

#ifdef FT_DEBUG_MEMORY
      /* assume `ft_debug_file_` and `ft_debug_lineno_` are already set */
      stream->base = (unsigned അക്ഷരം*)ft_mem_qalloc( memory,
                                                    (FT_Long)എണ്ണം,
                                                    &error );
      എങ്കിൽ ( error )
        പോകുക Exit;
#അല്ലെങ്കിൽ
      എങ്കിൽ ( FT_QALLOC( stream->base, എണ്ണം ) )
        പോകുക Exit;
#endif
      /* read it */
      read_bytes = stream->പഠ( stream, stream->pos,
                                 stream->base, എണ്ണം );
      എങ്കിൽ ( read_bytes < എണ്ണം )
      {
        FT_ERROR(( "FT_Stream_EnterFrame:"
                   " invalid read; expected %lu bytes, got %lu\n",
                   എണ്ണം, read_bytes ));

        FT_FREE( stream->base );
        error = FT_THROW( Invalid_Stream_Operation );
      }

      stream->cursor = stream->base;
      stream->limit  = FT_OFFSET( stream->cursor, എണ്ണം );
      stream->pos   += read_bytes;
    }
    അല്ലെങ്കിൽ
    {
      /* check current and new position */
      എങ്കിൽ ( stream->pos >= stream->size        ||
           stream->size - stream->pos < എണ്ണം )
      {
        FT_ERROR(( "FT_Stream_EnterFrame:"
                   " invalid i/o; pos = 0x%lx, count = %lu, size = 0x%lx\n",
                   stream->pos, എണ്ണം, stream->size ));

        error = FT_THROW( Invalid_Stream_Operation );
        പോകുക Exit;
      }

      /* set cursor */
      stream->cursor = stream->base + stream->pos;
      stream->limit  = stream->cursor + എണ്ണം;
      stream->pos   += എണ്ണം;
    }

  Exit:
    തിരികെ error;
  }


  FT_BASE_DEF( ശൂന്യം )
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

    എങ്കിൽ ( stream->പഠ )
    {
      FT_Memory  memory = stream->memory;


#ifdef FT_DEBUG_MEMORY
      ft_mem_free( memory, stream->base );
      stream->base = ശൂന്യ;
#അല്ലെങ്കിൽ
      FT_FREE( stream->base );
#endif
    }

    stream->cursor = ശൂന്യ;
    stream->limit  = ശൂന്യ;
  }


  FT_BASE_DEF( FT_Byte )
  FT_Stream_GetByte( FT_Stream  stream )
  {
    FT_Byte  result;


    FT_ASSERT( stream && stream->cursor );

    result = 0;
    എങ്കിൽ ( stream->cursor < stream->limit )
      result = *stream->cursor++;

    തിരികെ result;
  }


  FT_BASE_DEF( FT_UInt16 )
  FT_Stream_GetUShort( FT_Stream  stream )
  {
    FT_Byte*   p;
    FT_UInt16  result;


    FT_ASSERT( stream && stream->cursor );

    result         = 0;
    p              = stream->cursor;
    എങ്കിൽ ( p + 1 < stream->limit )
      result       = FT_NEXT_USHORT( p );
    stream->cursor = p;

    തിരികെ result;
  }


  FT_BASE_DEF( FT_UInt16 )
  FT_Stream_GetUShortLE( FT_Stream  stream )
  {
    FT_Byte*   p;
    FT_UInt16  result;


    FT_ASSERT( stream && stream->cursor );

    result         = 0;
    p              = stream->cursor;
    എങ്കിൽ ( p + 1 < stream->limit )
      result       = FT_NEXT_USHORT_LE( p );
    stream->cursor = p;

    തിരികെ result;
  }


  FT_BASE_DEF( FT_UInt32 )
  FT_Stream_GetUOffset( FT_Stream  stream )
  {
    FT_Byte*  p;
    FT_UInt32 result;


    FT_ASSERT( stream && stream->cursor );

    result         = 0;
    p              = stream->cursor;
    എങ്കിൽ ( p + 2 < stream->limit )
      result       = FT_NEXT_UOFF3( p );
    stream->cursor = p;
    തിരികെ result;
  }


  FT_BASE_DEF( FT_UInt32 )
  FT_Stream_GetULong( FT_Stream  stream )
  {
    FT_Byte*  p;
    FT_UInt32 result;


    FT_ASSERT( stream && stream->cursor );

    result         = 0;
    p              = stream->cursor;
    എങ്കിൽ ( p + 3 < stream->limit )
      result       = FT_NEXT_ULONG( p );
    stream->cursor = p;
    തിരികെ result;
  }


  FT_BASE_DEF( FT_UInt32 )
  FT_Stream_GetULongLE( FT_Stream  stream )
  {
    FT_Byte*  p;
    FT_UInt32 result;


    FT_ASSERT( stream && stream->cursor );

    result         = 0;
    p              = stream->cursor;
    എങ്കിൽ ( p + 3 < stream->limit )
      result       = FT_NEXT_ULONG_LE( p );
    stream->cursor = p;
    തിരികെ result;
  }


  FT_BASE_DEF( FT_Byte )
  FT_Stream_ReadByte( FT_Stream  stream,
                      FT_Error*  error )
  {
    FT_Byte  result = 0;


    FT_ASSERT( stream );

    എങ്കിൽ ( stream->pos < stream->size )
    {
      എങ്കിൽ ( stream->പഠ )
      {
        എങ്കിൽ ( stream->പഠ( stream, stream->pos, &result, 1L ) != 1L )
          പോകുക Fail;
      }
      അല്ലെങ്കിൽ
        result = stream->base[stream->pos];
    }
    അല്ലെങ്കിൽ
      പോകുക Fail;

    stream->pos++;

    *error = FT_Err_Ok;

    തിരികെ result;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadByte:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    തിരികെ result;
  }


  FT_BASE_DEF( FT_UInt16 )
  FT_Stream_ReadUShort( FT_Stream  stream,
                        FT_Error*  error )
  {
    FT_Byte    reads[2];
    FT_Byte*   p;
    FT_UInt16  result = 0;


    FT_ASSERT( stream );

    എങ്കിൽ ( stream->pos + 1 < stream->size )
    {
      എങ്കിൽ ( stream->പഠ )
      {
        എങ്കിൽ ( stream->പഠ( stream, stream->pos, reads, 2L ) != 2L )
          പോകുക Fail;

        p = reads;
      }
      അല്ലെങ്കിൽ
        p = stream->base + stream->pos;

      എങ്കിൽ ( p )
        result = FT_NEXT_USHORT( p );
    }
    അല്ലെങ്കിൽ
      പോകുക Fail;

    stream->pos += 2;

    *error = FT_Err_Ok;

    തിരികെ result;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadUShort:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    തിരികെ result;
  }


  FT_BASE_DEF( FT_UInt16 )
  FT_Stream_ReadUShortLE( FT_Stream  stream,
                          FT_Error*  error )
  {
    FT_Byte    reads[2];
    FT_Byte*   p;
    FT_UInt16  result = 0;


    FT_ASSERT( stream );

    എങ്കിൽ ( stream->pos + 1 < stream->size )
    {
      എങ്കിൽ ( stream->പഠ )
      {
        എങ്കിൽ ( stream->പഠ( stream, stream->pos, reads, 2L ) != 2L )
          പോകുക Fail;

        p = reads;
      }
      അല്ലെങ്കിൽ
        p = stream->base + stream->pos;

      എങ്കിൽ ( p )
        result = FT_NEXT_USHORT_LE( p );
    }
    അല്ലെങ്കിൽ
      പോകുക Fail;

    stream->pos += 2;

    *error = FT_Err_Ok;

    തിരികെ result;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadUShortLE:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    തിരികെ result;
  }


  FT_BASE_DEF( FT_ULong )
  FT_Stream_ReadUOffset( FT_Stream  stream,
                         FT_Error*  error )
  {
    FT_Byte   reads[3];
    FT_Byte*  p;
    FT_ULong  result = 0;


    FT_ASSERT( stream );

    എങ്കിൽ ( stream->pos + 2 < stream->size )
    {
      എങ്കിൽ ( stream->പഠ )
      {
        എങ്കിൽ (stream->പഠ( stream, stream->pos, reads, 3L ) != 3L )
          പോകുക Fail;

        p = reads;
      }
      അല്ലെങ്കിൽ
        p = stream->base + stream->pos;

      എങ്കിൽ ( p )
        result = FT_NEXT_UOFF3( p );
    }
    അല്ലെങ്കിൽ
      പോകുക Fail;

    stream->pos += 3;

    *error = FT_Err_Ok;

    തിരികെ result;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadUOffset:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    തിരികെ result;
  }


  FT_BASE_DEF( FT_UInt32 )
  FT_Stream_ReadULong( FT_Stream  stream,
                       FT_Error*  error )
  {
    FT_Byte   reads[4];
    FT_Byte*  p;
    FT_UInt32 result = 0;


    FT_ASSERT( stream );

    എങ്കിൽ ( stream->pos + 3 < stream->size )
    {
      എങ്കിൽ ( stream->പഠ )
      {
        എങ്കിൽ ( stream->പഠ( stream, stream->pos, reads, 4L ) != 4L )
          പോകുക Fail;

        p = reads;
      }
      അല്ലെങ്കിൽ
        p = stream->base + stream->pos;

      എങ്കിൽ ( p )
        result = FT_NEXT_ULONG( p );
    }
    അല്ലെങ്കിൽ
      പോകുക Fail;

    stream->pos += 4;

    *error = FT_Err_Ok;

    തിരികെ result;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadULong:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    തിരികെ result;
  }


  FT_BASE_DEF( FT_UInt32 )
  FT_Stream_ReadULongLE( FT_Stream  stream,
                         FT_Error*  error )
  {
    FT_Byte   reads[4];
    FT_Byte*  p;
    FT_UInt32 result = 0;


    FT_ASSERT( stream );

    എങ്കിൽ ( stream->pos + 3 < stream->size )
    {
      എങ്കിൽ ( stream->പഠ )
      {
        എങ്കിൽ ( stream->പഠ( stream, stream->pos, reads, 4L ) != 4L )
          പോകുക Fail;

        p = reads;
      }
      അല്ലെങ്കിൽ
        p = stream->base + stream->pos;

      എങ്കിൽ ( p )
        result = FT_NEXT_ULONG_LE( p );
    }
    അല്ലെങ്കിൽ
      പോകുക Fail;

    stream->pos += 4;

    *error = FT_Err_Ok;

    തിരികെ result;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadULongLE:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    തിരികെ result;
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_ReadFields( FT_Stream              stream,
                        സ്ഥിരം FT_Frame_Field*  fields,
                        ശൂന്യം*                  structure )
  {
    FT_Error  error;
    FT_Bool   frame_accessed = 0;
    FT_Byte*  cursor;


    എങ്കിൽ ( !fields )
      തിരികെ FT_THROW( Invalid_Argument );

    എങ്കിൽ ( !stream )
      തിരികെ FT_THROW( Invalid_Stream_Handle );

    cursor = stream->cursor;

    error = FT_Err_Ok;
    ചെയ്യുക
    {
      FT_ULong  മൂല്യം;
      FT_Int    sign_shift;
      FT_Byte*  p;


      തിരഞ്ഞെടുപ്പ് ( fields->മൂല്യം )
      {
      അവസ്ഥ ft_frame_start:  /* access a new frame */
        error = FT_Stream_EnterFrame( stream, fields->offset );
        എങ്കിൽ ( error )
          പോകുക Exit;

        frame_accessed = 1;
        cursor         = stream->cursor;
        fields++;
        തുടരുക;  /* loop! */

      അവസ്ഥ ft_frame_bytes:  /* read a byte sequence */
      അവസ്ഥ ft_frame_skip:   /* skip some bytes      */
        {
          FT_Offset  len = fields->size;


          എങ്കിൽ ( len > (FT_Offset)( stream->limit - cursor ) )
          {
            error = FT_THROW( Invalid_Stream_Operation );
            പോകുക Exit;
          }

          എങ്കിൽ ( fields->മൂല്യം == ft_frame_bytes )
          {
            p = (FT_Byte*)structure + fields->offset;
            FT_MEM_COPY( p, cursor, len );
          }
          cursor += len;
          fields++;
          തുടരുക;
        }

      അവസ്ഥ ft_frame_byte:
      അവസ്ഥ ft_frame_schar:  /* read a single byte */
        മൂല്യം = FT_NEXT_BYTE( cursor );
        sign_shift = 24;
        നിർത്തുക;

      അവസ്ഥ ft_frame_short_be:
      അവസ്ഥ ft_frame_ushort_be:  /* read a 2-byte big-endian short */
        മൂല്യം = FT_NEXT_USHORT( cursor );
        sign_shift = 16;
        നിർത്തുക;

      അവസ്ഥ ft_frame_short_le:
      അവസ്ഥ ft_frame_ushort_le:  /* read a 2-byte little-endian short */
        മൂല്യം = FT_NEXT_USHORT_LE( cursor );
        sign_shift = 16;
        നിർത്തുക;

      അവസ്ഥ ft_frame_long_be:
      അവസ്ഥ ft_frame_ulong_be:  /* read a 4-byte big-endian long */
        മൂല്യം = FT_NEXT_ULONG( cursor );
        sign_shift = 0;
        നിർത്തുക;

      അവസ്ഥ ft_frame_long_le:
      അവസ്ഥ ft_frame_ulong_le:  /* read a 4-byte little-endian long */
        മൂല്യം = FT_NEXT_ULONG_LE( cursor );
        sign_shift = 0;
        നിർത്തുക;

      അവസ്ഥ ft_frame_off3_be:
      അവസ്ഥ ft_frame_uoff3_be:  /* read a 3-byte big-endian long */
        മൂല്യം = FT_NEXT_UOFF3( cursor );
        sign_shift = 8;
        നിർത്തുക;

      അവസ്ഥ ft_frame_off3_le:
      അവസ്ഥ ft_frame_uoff3_le:  /* read a 3-byte little-endian long */
        മൂല്യം = FT_NEXT_UOFF3_LE( cursor );
        sign_shift = 8;
        നിർത്തുക;

      സ്ഥിരസ്ഥിതി:
        /* otherwise, exit the loop */
        stream->cursor = cursor;
        പോകുക Exit;
      }

      /* now, compute the signed value if necessary */
      എങ്കിൽ ( fields->മൂല്യം & FT_FRAME_OP_SIGNED )
        മൂല്യം = (FT_ULong)( (FT_Int32)( മൂല്യം << sign_shift ) >> sign_shift );

      /* finally, store the value in the object */

      p = (FT_Byte*)structure + fields->offset;
      തിരഞ്ഞെടുപ്പ് ( fields->size )
      {
      അവസ്ഥ ( 8 / FT_CHAR_BIT ):
        *(FT_Byte*)p = (FT_Byte)മൂല്യം;
        നിർത്തുക;

      അവസ്ഥ ( 16 / FT_CHAR_BIT ):
        *(FT_UShort*)p = (FT_UShort)മൂല്യം;
        നിർത്തുക;

      അവസ്ഥ ( 32 / FT_CHAR_BIT ):
        *(FT_UInt32*)p = (FT_UInt32)മൂല്യം;
        നിർത്തുക;

      സ്ഥിരസ്ഥിതി:  /* for 64-bit systems */
        *(FT_ULong*)p = (FT_ULong)മൂല്യം;
      }

      /* go to next field */
      fields++;
    }
    ആകുമ്പോൾ ( 1 );

  Exit:
    /* close the frame if it was opened by this read */
    എങ്കിൽ ( frame_accessed )
      FT_Stream_ExitFrame( stream );

    തിരികെ error;
  }


/* END */
