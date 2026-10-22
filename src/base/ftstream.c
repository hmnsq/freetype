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


#include <freetype/internal/ftstream.h>
#include <freetype/internal/ftdebug.h>


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#undef  FT_COMPONENT
#define FT_COMPONENT  stream


  FT_BASE_DEF( void )
  FT_Stream_OpenMemory( FT_Stream       stream,
                        const FT_Byte*  base,
                        FT_ULong        size )
  {
    stream->base   = (FT_Byte*) base;
    stream->size   = size;
    stream->pos    = 0;
    stream->cursor = NULL;
    stream->read   = NULL;
    stream->close  = NULL;
  }


  FT_BASE_DEF( void )
  FT_Stream_Close( FT_Stream  stream )
  {
    만약 ( stream && stream->close )
      stream->close( stream );
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_Seek( FT_Stream  stream,
                  FT_ULong   pos )
  {
    FT_Error  error = FT_Err_Ok;


    만약 ( stream->read )
    {
      만약 ( stream->read( stream, pos, NULL, 0 ) )
      {
        FT_ERROR(( "FT_Stream_Seek:"
                   " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
                   pos, stream->size ));

        error = FT_THROW( Invalid_Stream_Operation );
      }
    }
    /* note that seeking to the first position after the file is valid */
    아니면 만약 ( pos > stream->size )
    {
      FT_ERROR(( "FT_Stream_Seek:"
                 " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
                 pos, stream->size ));

      error = FT_THROW( Invalid_Stream_Operation );
    }

    만약 ( !error )
      stream->pos = pos;

    반환 error;
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_Skip( FT_Stream  stream,
                  FT_Long    distance )
  {
    만약 ( distance < 0 )
      반환 FT_THROW( Invalid_Stream_Operation );

    반환 FT_Stream_Seek( stream, stream->pos + (FT_ULong)distance );
  }


  FT_BASE_DEF( FT_ULong )
  FT_Stream_Pos( FT_Stream  stream )
  {
    반환 stream->pos;
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_Read( FT_Stream  stream,
                  FT_Byte*   buffer,
                  FT_ULong   count )
  {
    반환 FT_Stream_ReadAt( stream, stream->pos, buffer, count );
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_ReadAt( FT_Stream  stream,
                    FT_ULong   pos,
                    FT_Byte*   buffer,
                    FT_ULong   count )
  {
    FT_Error  error = FT_Err_Ok;
    FT_ULong  read_bytes;


    만약 ( pos >= stream->size )
    {
      FT_ERROR(( "FT_Stream_ReadAt:"
                 " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
                 pos, stream->size ));

      반환 FT_THROW( Invalid_Stream_Operation );
    }

    만약 ( stream->read )
      read_bytes = stream->read( stream, pos, buffer, count );
    아니면
    {
      read_bytes = stream->size - pos;
      만약 ( read_bytes > count )
        read_bytes = count;

      /* Allow "reading" zero bytes without UB even if buffer is NULL */
      만약 ( count )
        FT_MEM_COPY( buffer, stream->base + pos, read_bytes );
    }

    stream->pos = pos + read_bytes;

    만약 ( read_bytes < count )
    {
      FT_ERROR(( "FT_Stream_ReadAt:"
                 " invalid read; expected %lu bytes, got %lu\n",
                 count, read_bytes ));

      error = FT_THROW( Invalid_Stream_Operation );
    }

    반환 error;
  }


  FT_BASE_DEF( FT_ULong )
  FT_Stream_TryRead( FT_Stream  stream,
                     FT_Byte*   buffer,
                     FT_ULong   count )
  {
    FT_ULong  read_bytes = 0;


    만약 ( stream->pos >= stream->size )
      goto Exit;

    만약 ( stream->read )
      read_bytes = stream->read( stream, stream->pos, buffer, count );
    아니면
    {
      read_bytes = stream->size - stream->pos;
      만약 ( read_bytes > count )
        read_bytes = count;

      /* Allow "reading" zero bytes without UB even if buffer is NULL */
      만약 ( count )
        FT_MEM_COPY( buffer, stream->base + stream->pos, read_bytes );
    }

    stream->pos += read_bytes;

  Exit:
    반환 read_bytes;
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_ExtractFrame( FT_Stream  stream,
                          FT_ULong   count,
                          FT_Byte**  pbytes )
  {
    FT_Error  error;


    error = FT_Stream_EnterFrame( stream, count );
    만약 ( !error )
    {
      *pbytes = (FT_Byte*)stream->cursor;

      /* equivalent to FT_Stream_ExitFrame(), with no memory block release */
      stream->cursor = NULL;
      stream->limit  = NULL;
    }

    반환 error;
  }


  FT_BASE_DEF( void )
  FT_Stream_ReleaseFrame( FT_Stream  stream,
                          FT_Byte**  pbytes )
  {
    만약 ( stream && stream->read )
    {
      FT_Memory  memory = stream->memory;


#ifdef FT_DEBUG_MEMORY
      ft_mem_free( memory, *pbytes );
#아니면
      FT_FREE( *pbytes );
#endif
    }

    *pbytes = NULL;
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_EnterFrame( FT_Stream  stream,
                        FT_ULong   count )
  {
    FT_Error  error = FT_Err_Ok;
    FT_ULong  read_bytes;


    FT_TRACE7(( "FT_Stream_EnterFrame: %lu bytes\n", count ));

    /* check for nested frame access */
    FT_ASSERT( stream && stream->cursor == 0 );

    만약 ( stream->read )
    {
      /* allocate the frame in memory */
      FT_Memory  memory = stream->memory;


      /* simple sanity check */
      만약 ( count > stream->size )
      {
        FT_ERROR(( "FT_Stream_EnterFrame:"
                   " frame size (%lu) larger than stream size (%lu)\n",
                   count, stream->size ));

        error = FT_THROW( Invalid_Stream_Operation );
        goto Exit;
      }

#ifdef FT_DEBUG_MEMORY
      /* assume `ft_debug_file_` and `ft_debug_lineno_` are already set */
      stream->base = (unsigned char*)ft_mem_qalloc( memory,
                                                    (FT_Long)count,
                                                    &error );
      만약 ( error )
        goto Exit;
#아니면
      만약 ( FT_QALLOC( stream->base, count ) )
        goto Exit;
#endif
      /* read it */
      read_bytes = stream->read( stream, stream->pos,
                                 stream->base, count );
      만약 ( read_bytes < count )
      {
        FT_ERROR(( "FT_Stream_EnterFrame:"
                   " invalid read; expected %lu bytes, got %lu\n",
                   count, read_bytes ));

        FT_FREE( stream->base );
        error = FT_THROW( Invalid_Stream_Operation );
      }

      stream->cursor = stream->base;
      stream->limit  = FT_OFFSET( stream->cursor, count );
      stream->pos   += read_bytes;
    }
    아니면
    {
      /* check current and new position */
      만약 ( stream->pos >= stream->size        ||
           stream->size - stream->pos < count )
      {
        FT_ERROR(( "FT_Stream_EnterFrame:"
                   " invalid i/o; pos = 0x%lx, count = %lu, size = 0x%lx\n",
                   stream->pos, count, stream->size ));

        error = FT_THROW( Invalid_Stream_Operation );
        goto Exit;
      }

      /* set cursor */
      stream->cursor = stream->base + stream->pos;
      stream->limit  = stream->cursor + count;
      stream->pos   += count;
    }

  Exit:
    반환 error;
  }


  FT_BASE_DEF( void )
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

    만약 ( stream->read )
    {
      FT_Memory  memory = stream->memory;


#ifdef FT_DEBUG_MEMORY
      ft_mem_free( memory, stream->base );
      stream->base = NULL;
#아니면
      FT_FREE( stream->base );
#endif
    }

    stream->cursor = NULL;
    stream->limit  = NULL;
  }


  FT_BASE_DEF( FT_Byte )
  FT_Stream_GetByte( FT_Stream  stream )
  {
    FT_Byte  result;


    FT_ASSERT( stream && stream->cursor );

    result = 0;
    만약 ( stream->cursor < stream->limit )
      result = *stream->cursor++;

    반환 result;
  }


  FT_BASE_DEF( FT_UInt16 )
  FT_Stream_GetUShort( FT_Stream  stream )
  {
    FT_Byte*   p;
    FT_UInt16  result;


    FT_ASSERT( stream && stream->cursor );

    result         = 0;
    p              = stream->cursor;
    만약 ( p + 1 < stream->limit )
      result       = FT_NEXT_USHORT( p );
    stream->cursor = p;

    반환 result;
  }


  FT_BASE_DEF( FT_UInt16 )
  FT_Stream_GetUShortLE( FT_Stream  stream )
  {
    FT_Byte*   p;
    FT_UInt16  result;


    FT_ASSERT( stream && stream->cursor );

    result         = 0;
    p              = stream->cursor;
    만약 ( p + 1 < stream->limit )
      result       = FT_NEXT_USHORT_LE( p );
    stream->cursor = p;

    반환 result;
  }


  FT_BASE_DEF( FT_UInt32 )
  FT_Stream_GetUOffset( FT_Stream  stream )
  {
    FT_Byte*  p;
    FT_UInt32 result;


    FT_ASSERT( stream && stream->cursor );

    result         = 0;
    p              = stream->cursor;
    만약 ( p + 2 < stream->limit )
      result       = FT_NEXT_UOFF3( p );
    stream->cursor = p;
    반환 result;
  }


  FT_BASE_DEF( FT_UInt32 )
  FT_Stream_GetULong( FT_Stream  stream )
  {
    FT_Byte*  p;
    FT_UInt32 result;


    FT_ASSERT( stream && stream->cursor );

    result         = 0;
    p              = stream->cursor;
    만약 ( p + 3 < stream->limit )
      result       = FT_NEXT_ULONG( p );
    stream->cursor = p;
    반환 result;
  }


  FT_BASE_DEF( FT_UInt32 )
  FT_Stream_GetULongLE( FT_Stream  stream )
  {
    FT_Byte*  p;
    FT_UInt32 result;


    FT_ASSERT( stream && stream->cursor );

    result         = 0;
    p              = stream->cursor;
    만약 ( p + 3 < stream->limit )
      result       = FT_NEXT_ULONG_LE( p );
    stream->cursor = p;
    반환 result;
  }


  FT_BASE_DEF( FT_Byte )
  FT_Stream_ReadByte( FT_Stream  stream,
                      FT_Error*  error )
  {
    FT_Byte  result = 0;


    FT_ASSERT( stream );

    만약 ( stream->pos < stream->size )
    {
      만약 ( stream->read )
      {
        만약 ( stream->read( stream, stream->pos, &result, 1L ) != 1L )
          goto Fail;
      }
      아니면
        result = stream->base[stream->pos];
    }
    아니면
      goto Fail;

    stream->pos++;

    *error = FT_Err_Ok;

    반환 result;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadByte:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    반환 result;
  }


  FT_BASE_DEF( FT_UInt16 )
  FT_Stream_ReadUShort( FT_Stream  stream,
                        FT_Error*  error )
  {
    FT_Byte    reads[2];
    FT_Byte*   p;
    FT_UInt16  result = 0;


    FT_ASSERT( stream );

    만약 ( stream->pos + 1 < stream->size )
    {
      만약 ( stream->read )
      {
        만약 ( stream->read( stream, stream->pos, reads, 2L ) != 2L )
          goto Fail;

        p = reads;
      }
      아니면
        p = stream->base + stream->pos;

      만약 ( p )
        result = FT_NEXT_USHORT( p );
    }
    아니면
      goto Fail;

    stream->pos += 2;

    *error = FT_Err_Ok;

    반환 result;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadUShort:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    반환 result;
  }


  FT_BASE_DEF( FT_UInt16 )
  FT_Stream_ReadUShortLE( FT_Stream  stream,
                          FT_Error*  error )
  {
    FT_Byte    reads[2];
    FT_Byte*   p;
    FT_UInt16  result = 0;


    FT_ASSERT( stream );

    만약 ( stream->pos + 1 < stream->size )
    {
      만약 ( stream->read )
      {
        만약 ( stream->read( stream, stream->pos, reads, 2L ) != 2L )
          goto Fail;

        p = reads;
      }
      아니면
        p = stream->base + stream->pos;

      만약 ( p )
        result = FT_NEXT_USHORT_LE( p );
    }
    아니면
      goto Fail;

    stream->pos += 2;

    *error = FT_Err_Ok;

    반환 result;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadUShortLE:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    반환 result;
  }


  FT_BASE_DEF( FT_ULong )
  FT_Stream_ReadUOffset( FT_Stream  stream,
                         FT_Error*  error )
  {
    FT_Byte   reads[3];
    FT_Byte*  p;
    FT_ULong  result = 0;


    FT_ASSERT( stream );

    만약 ( stream->pos + 2 < stream->size )
    {
      만약 ( stream->read )
      {
        만약 (stream->read( stream, stream->pos, reads, 3L ) != 3L )
          goto Fail;

        p = reads;
      }
      아니면
        p = stream->base + stream->pos;

      만약 ( p )
        result = FT_NEXT_UOFF3( p );
    }
    아니면
      goto Fail;

    stream->pos += 3;

    *error = FT_Err_Ok;

    반환 result;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadUOffset:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    반환 result;
  }


  FT_BASE_DEF( FT_UInt32 )
  FT_Stream_ReadULong( FT_Stream  stream,
                       FT_Error*  error )
  {
    FT_Byte   reads[4];
    FT_Byte*  p;
    FT_UInt32 result = 0;


    FT_ASSERT( stream );

    만약 ( stream->pos + 3 < stream->size )
    {
      만약 ( stream->read )
      {
        만약 ( stream->read( stream, stream->pos, reads, 4L ) != 4L )
          goto Fail;

        p = reads;
      }
      아니면
        p = stream->base + stream->pos;

      만약 ( p )
        result = FT_NEXT_ULONG( p );
    }
    아니면
      goto Fail;

    stream->pos += 4;

    *error = FT_Err_Ok;

    반환 result;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadULong:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    반환 result;
  }


  FT_BASE_DEF( FT_UInt32 )
  FT_Stream_ReadULongLE( FT_Stream  stream,
                         FT_Error*  error )
  {
    FT_Byte   reads[4];
    FT_Byte*  p;
    FT_UInt32 result = 0;


    FT_ASSERT( stream );

    만약 ( stream->pos + 3 < stream->size )
    {
      만약 ( stream->read )
      {
        만약 ( stream->read( stream, stream->pos, reads, 4L ) != 4L )
          goto Fail;

        p = reads;
      }
      아니면
        p = stream->base + stream->pos;

      만약 ( p )
        result = FT_NEXT_ULONG_LE( p );
    }
    아니면
      goto Fail;

    stream->pos += 4;

    *error = FT_Err_Ok;

    반환 result;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadULongLE:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    반환 result;
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_ReadFields( FT_Stream              stream,
                        const FT_Frame_Field*  fields,
                        void*                  structure )
  {
    FT_Error  error;
    FT_Bool   frame_accessed = 0;
    FT_Byte*  cursor;


    만약 ( !fields )
      반환 FT_THROW( Invalid_Argument );

    만약 ( !stream )
      반환 FT_THROW( Invalid_Stream_Handle );

    cursor = stream->cursor;

    error = FT_Err_Ok;
    do
    {
      FT_ULong  value;
      FT_Int    sign_shift;
      FT_Byte*  p;


      switch ( fields->value )
      {
      case ft_frame_start:  /* access a new frame */
        error = FT_Stream_EnterFrame( stream, fields->offset );
        만약 ( error )
          goto Exit;

        frame_accessed = 1;
        cursor         = stream->cursor;
        fields++;
        continue;  /* loop! */

      case ft_frame_bytes:  /* read a byte sequence */
      case ft_frame_skip:   /* skip some bytes      */
        {
          FT_Offset  len = fields->size;


          만약 ( len > (FT_Offset)( stream->limit - cursor ) )
          {
            error = FT_THROW( Invalid_Stream_Operation );
            goto Exit;
          }

          만약 ( fields->value == ft_frame_bytes )
          {
            p = (FT_Byte*)structure + fields->offset;
            FT_MEM_COPY( p, cursor, len );
          }
          cursor += len;
          fields++;
          continue;
        }

      case ft_frame_byte:
      case ft_frame_schar:  /* read a single byte */
        value = FT_NEXT_BYTE( cursor );
        sign_shift = 24;
        break;

      case ft_frame_short_be:
      case ft_frame_ushort_be:  /* read a 2-byte big-endian short */
        value = FT_NEXT_USHORT( cursor );
        sign_shift = 16;
        break;

      case ft_frame_short_le:
      case ft_frame_ushort_le:  /* read a 2-byte little-endian short */
        value = FT_NEXT_USHORT_LE( cursor );
        sign_shift = 16;
        break;

      case ft_frame_long_be:
      case ft_frame_ulong_be:  /* read a 4-byte big-endian long */
        value = FT_NEXT_ULONG( cursor );
        sign_shift = 0;
        break;

      case ft_frame_long_le:
      case ft_frame_ulong_le:  /* read a 4-byte little-endian long */
        value = FT_NEXT_ULONG_LE( cursor );
        sign_shift = 0;
        break;

      case ft_frame_off3_be:
      case ft_frame_uoff3_be:  /* read a 3-byte big-endian long */
        value = FT_NEXT_UOFF3( cursor );
        sign_shift = 8;
        break;

      case ft_frame_off3_le:
      case ft_frame_uoff3_le:  /* read a 3-byte little-endian long */
        value = FT_NEXT_UOFF3_LE( cursor );
        sign_shift = 8;
        break;

      default:
        /* otherwise, exit the loop */
        stream->cursor = cursor;
        goto Exit;
      }

      /* now, compute the signed value if necessary */
      만약 ( fields->value & FT_FRAME_OP_SIGNED )
        value = (FT_ULong)( (FT_Int32)( value << sign_shift ) >> sign_shift );

      /* finally, store the value in the object */

      p = (FT_Byte*)structure + fields->offset;
      switch ( fields->size )
      {
      case ( 8 / FT_CHAR_BIT ):
        *(FT_Byte*)p = (FT_Byte)value;
        break;

      case ( 16 / FT_CHAR_BIT ):
        *(FT_UShort*)p = (FT_UShort)value;
        break;

      case ( 32 / FT_CHAR_BIT ):
        *(FT_UInt32*)p = (FT_UInt32)value;
        break;

      default:  /* for 64-bit systems */
        *(FT_ULong*)p = (FT_ULong)value;
      }

      /* go to next field */
      fields++;
    }
    동안 ( 1 );

  Exit:
    /* close the frame if it was opened by this read */
    만약 ( frame_accessed )
      FT_Stream_ExitFrame( stream );

    반환 error;
  }


/* END */
