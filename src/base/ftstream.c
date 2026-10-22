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
    ပေႃး ( stream && stream->close )
      stream->close( stream );
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_Seek( FT_Stream  stream,
                  FT_ULong   pos )
  {
    FT_Error  error = FT_Err_Ok;


    ပေႃး ( stream->read )
    {
      ပေႃး ( stream->read( stream, pos, NULL, 0 ) )
      {
        FT_ERROR(( "FT_Stream_Seek:"
                   " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
                   pos, stream->size ));

        error = FT_THROW( Invalid_Stream_Operation );
      }
    }
    /* note that seeking to the first position after the file is valid */
    ပေႃးဢမ်ႇ ပေႃး ( pos > stream->size )
    {
      FT_ERROR(( "FT_Stream_Seek:"
                 " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
                 pos, stream->size ));

      error = FT_THROW( Invalid_Stream_Operation );
    }

    ပေႃး ( !error )
      stream->pos = pos;

    ပွၵ်ႈ error;
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_Skip( FT_Stream  stream,
                  FT_Long    distance )
  {
    ပေႃး ( distance < 0 )
      ပွၵ်ႈ FT_THROW( Invalid_Stream_Operation );

    ပွၵ်ႈ FT_Stream_Seek( stream, stream->pos + (FT_ULong)distance );
  }


  FT_BASE_DEF( FT_ULong )
  FT_Stream_Pos( FT_Stream  stream )
  {
    ပွၵ်ႈ stream->pos;
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_Read( FT_Stream  stream,
                  FT_Byte*   buffer,
                  FT_ULong   count )
  {
    ပွၵ်ႈ FT_Stream_ReadAt( stream, stream->pos, buffer, count );
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_ReadAt( FT_Stream  stream,
                    FT_ULong   pos,
                    FT_Byte*   buffer,
                    FT_ULong   count )
  {
    FT_Error  error = FT_Err_Ok;
    FT_ULong  read_bytes;


    ပေႃး ( pos >= stream->size )
    {
      FT_ERROR(( "FT_Stream_ReadAt:"
                 " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
                 pos, stream->size ));

      ပွၵ်ႈ FT_THROW( Invalid_Stream_Operation );
    }

    ပေႃး ( stream->read )
      read_bytes = stream->read( stream, pos, buffer, count );
    ပေႃးဢမ်ႇ
    {
      read_bytes = stream->size - pos;
      ပေႃး ( read_bytes > count )
        read_bytes = count;

      /* Allow "reading" zero bytes without UB even if buffer is NULL */
      ပေႃး ( count )
        FT_MEM_COPY( buffer, stream->base + pos, read_bytes );
    }

    stream->pos = pos + read_bytes;

    ပေႃး ( read_bytes < count )
    {
      FT_ERROR(( "FT_Stream_ReadAt:"
                 " invalid read; expected %lu bytes, got %lu\n",
                 count, read_bytes ));

      error = FT_THROW( Invalid_Stream_Operation );
    }

    ပွၵ်ႈ error;
  }


  FT_BASE_DEF( FT_ULong )
  FT_Stream_TryRead( FT_Stream  stream,
                     FT_Byte*   buffer,
                     FT_ULong   count )
  {
    FT_ULong  read_bytes = 0;


    ပေႃး ( stream->pos >= stream->size )
      goto Exit;

    ပေႃး ( stream->read )
      read_bytes = stream->read( stream, stream->pos, buffer, count );
    ပေႃးဢမ်ႇ
    {
      read_bytes = stream->size - stream->pos;
      ပေႃး ( read_bytes > count )
        read_bytes = count;

      /* Allow "reading" zero bytes without UB even if buffer is NULL */
      ပေႃး ( count )
        FT_MEM_COPY( buffer, stream->base + stream->pos, read_bytes );
    }

    stream->pos += read_bytes;

  Exit:
    ပွၵ်ႈ read_bytes;
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_ExtractFrame( FT_Stream  stream,
                          FT_ULong   count,
                          FT_Byte**  pbytes )
  {
    FT_Error  error;


    error = FT_Stream_EnterFrame( stream, count );
    ပေႃး ( !error )
    {
      *pbytes = (FT_Byte*)stream->cursor;

      /* equivalent to FT_Stream_ExitFrame(), with no memory block release */
      stream->cursor = NULL;
      stream->limit  = NULL;
    }

    ပွၵ်ႈ error;
  }


  FT_BASE_DEF( void )
  FT_Stream_ReleaseFrame( FT_Stream  stream,
                          FT_Byte**  pbytes )
  {
    ပေႃး ( stream && stream->read )
    {
      FT_Memory  memory = stream->memory;


#ifdef FT_DEBUG_MEMORY
      ft_mem_free( memory, *pbytes );
#ပေႃးဢမ်ႇ
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

    ပေႃး ( stream->read )
    {
      /* allocate the frame in memory */
      FT_Memory  memory = stream->memory;


      /* simple sanity check */
      ပေႃး ( count > stream->size )
      {
        FT_ERROR(( "FT_Stream_EnterFrame:"
                   " frame size (%lu) larger than stream size (%lu)\n",
                   count, stream->size ));

        error = FT_THROW( Invalid_Stream_Operation );
        goto Exit;
      }

#ifdef FT_DEBUG_MEMORY
      /* assume `ft_debug_file_` and `ft_debug_lineno_` are already set */
      stream->base = (unsigned var_na*)ft_mem_qalloc( memory,
                                                    (FT_Long)count,
                                                    &error );
      ပေႃး ( error )
        goto Exit;
#ပေႃးဢမ်ႇ
      ပေႃး ( FT_QALLOC( stream->base, count ) )
        goto Exit;
#endif
      /* read it */
      read_bytes = stream->read( stream, stream->pos,
                                 stream->base, count );
      ပေႃး ( read_bytes < count )
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
    ပေႃးဢမ်ႇ
    {
      /* check current and new position */
      ပေႃး ( stream->pos >= stream->size        ||
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
    ပွၵ်ႈ error;
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

    ပေႃး ( stream->read )
    {
      FT_Memory  memory = stream->memory;


#ifdef FT_DEBUG_MEMORY
      ft_mem_free( memory, stream->base );
      stream->base = NULL;
#ပေႃးဢမ်ႇ
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
    ပေႃး ( stream->cursor < stream->limit )
      result = *stream->cursor++;

    ပွၵ်ႈ result;
  }


  FT_BASE_DEF( FT_UInt16 )
  FT_Stream_GetUShort( FT_Stream  stream )
  {
    FT_Byte*   p;
    FT_UInt16  result;


    FT_ASSERT( stream && stream->cursor );

    result         = 0;
    p              = stream->cursor;
    ပေႃး ( p + 1 < stream->limit )
      result       = FT_NEXT_USHORT( p );
    stream->cursor = p;

    ပွၵ်ႈ result;
  }


  FT_BASE_DEF( FT_UInt16 )
  FT_Stream_GetUShortLE( FT_Stream  stream )
  {
    FT_Byte*   p;
    FT_UInt16  result;


    FT_ASSERT( stream && stream->cursor );

    result         = 0;
    p              = stream->cursor;
    ပေႃး ( p + 1 < stream->limit )
      result       = FT_NEXT_USHORT_LE( p );
    stream->cursor = p;

    ပွၵ်ႈ result;
  }


  FT_BASE_DEF( FT_UInt32 )
  FT_Stream_GetUOffset( FT_Stream  stream )
  {
    FT_Byte*  p;
    FT_UInt32 result;


    FT_ASSERT( stream && stream->cursor );

    result         = 0;
    p              = stream->cursor;
    ပေႃး ( p + 2 < stream->limit )
      result       = FT_NEXT_UOFF3( p );
    stream->cursor = p;
    ပွၵ်ႈ result;
  }


  FT_BASE_DEF( FT_UInt32 )
  FT_Stream_GetULong( FT_Stream  stream )
  {
    FT_Byte*  p;
    FT_UInt32 result;


    FT_ASSERT( stream && stream->cursor );

    result         = 0;
    p              = stream->cursor;
    ပေႃး ( p + 3 < stream->limit )
      result       = FT_NEXT_ULONG( p );
    stream->cursor = p;
    ပွၵ်ႈ result;
  }


  FT_BASE_DEF( FT_UInt32 )
  FT_Stream_GetULongLE( FT_Stream  stream )
  {
    FT_Byte*  p;
    FT_UInt32 result;


    FT_ASSERT( stream && stream->cursor );

    result         = 0;
    p              = stream->cursor;
    ပေႃး ( p + 3 < stream->limit )
      result       = FT_NEXT_ULONG_LE( p );
    stream->cursor = p;
    ပွၵ်ႈ result;
  }


  FT_BASE_DEF( FT_Byte )
  FT_Stream_ReadByte( FT_Stream  stream,
                      FT_Error*  error )
  {
    FT_Byte  result = 0;


    FT_ASSERT( stream );

    ပေႃး ( stream->pos < stream->size )
    {
      ပေႃး ( stream->read )
      {
        ပေႃး ( stream->read( stream, stream->pos, &result, 1L ) != 1L )
          goto Fail;
      }
      ပေႃးဢမ်ႇ
        result = stream->base[stream->pos];
    }
    ပေႃးဢမ်ႇ
      goto Fail;

    stream->pos++;

    *error = FT_Err_Ok;

    ပွၵ်ႈ result;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadByte:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    ပွၵ်ႈ result;
  }


  FT_BASE_DEF( FT_UInt16 )
  FT_Stream_ReadUShort( FT_Stream  stream,
                        FT_Error*  error )
  {
    FT_Byte    reads[2];
    FT_Byte*   p;
    FT_UInt16  result = 0;


    FT_ASSERT( stream );

    ပေႃး ( stream->pos + 1 < stream->size )
    {
      ပေႃး ( stream->read )
      {
        ပေႃး ( stream->read( stream, stream->pos, reads, 2L ) != 2L )
          goto Fail;

        p = reads;
      }
      ပေႃးဢမ်ႇ
        p = stream->base + stream->pos;

      ပေႃး ( p )
        result = FT_NEXT_USHORT( p );
    }
    ပေႃးဢမ်ႇ
      goto Fail;

    stream->pos += 2;

    *error = FT_Err_Ok;

    ပွၵ်ႈ result;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadUShort:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    ပွၵ်ႈ result;
  }


  FT_BASE_DEF( FT_UInt16 )
  FT_Stream_ReadUShortLE( FT_Stream  stream,
                          FT_Error*  error )
  {
    FT_Byte    reads[2];
    FT_Byte*   p;
    FT_UInt16  result = 0;


    FT_ASSERT( stream );

    ပေႃး ( stream->pos + 1 < stream->size )
    {
      ပေႃး ( stream->read )
      {
        ပေႃး ( stream->read( stream, stream->pos, reads, 2L ) != 2L )
          goto Fail;

        p = reads;
      }
      ပေႃးဢမ်ႇ
        p = stream->base + stream->pos;

      ပေႃး ( p )
        result = FT_NEXT_USHORT_LE( p );
    }
    ပေႃးဢမ်ႇ
      goto Fail;

    stream->pos += 2;

    *error = FT_Err_Ok;

    ပွၵ်ႈ result;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadUShortLE:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    ပွၵ်ႈ result;
  }


  FT_BASE_DEF( FT_ULong )
  FT_Stream_ReadUOffset( FT_Stream  stream,
                         FT_Error*  error )
  {
    FT_Byte   reads[3];
    FT_Byte*  p;
    FT_ULong  result = 0;


    FT_ASSERT( stream );

    ပေႃး ( stream->pos + 2 < stream->size )
    {
      ပေႃး ( stream->read )
      {
        ပေႃး (stream->read( stream, stream->pos, reads, 3L ) != 3L )
          goto Fail;

        p = reads;
      }
      ပေႃးဢမ်ႇ
        p = stream->base + stream->pos;

      ပေႃး ( p )
        result = FT_NEXT_UOFF3( p );
    }
    ပေႃးဢမ်ႇ
      goto Fail;

    stream->pos += 3;

    *error = FT_Err_Ok;

    ပွၵ်ႈ result;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadUOffset:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    ပွၵ်ႈ result;
  }


  FT_BASE_DEF( FT_UInt32 )
  FT_Stream_ReadULong( FT_Stream  stream,
                       FT_Error*  error )
  {
    FT_Byte   reads[4];
    FT_Byte*  p;
    FT_UInt32 result = 0;


    FT_ASSERT( stream );

    ပေႃး ( stream->pos + 3 < stream->size )
    {
      ပေႃး ( stream->read )
      {
        ပေႃး ( stream->read( stream, stream->pos, reads, 4L ) != 4L )
          goto Fail;

        p = reads;
      }
      ပေႃးဢမ်ႇ
        p = stream->base + stream->pos;

      ပေႃး ( p )
        result = FT_NEXT_ULONG( p );
    }
    ပေႃးဢမ်ႇ
      goto Fail;

    stream->pos += 4;

    *error = FT_Err_Ok;

    ပွၵ်ႈ result;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadULong:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    ပွၵ်ႈ result;
  }


  FT_BASE_DEF( FT_UInt32 )
  FT_Stream_ReadULongLE( FT_Stream  stream,
                         FT_Error*  error )
  {
    FT_Byte   reads[4];
    FT_Byte*  p;
    FT_UInt32 result = 0;


    FT_ASSERT( stream );

    ပေႃး ( stream->pos + 3 < stream->size )
    {
      ပေႃး ( stream->read )
      {
        ပေႃး ( stream->read( stream, stream->pos, reads, 4L ) != 4L )
          goto Fail;

        p = reads;
      }
      ပေႃးဢမ်ႇ
        p = stream->base + stream->pos;

      ပေႃး ( p )
        result = FT_NEXT_ULONG_LE( p );
    }
    ပေႃးဢမ်ႇ
      goto Fail;

    stream->pos += 4;

    *error = FT_Err_Ok;

    ပွၵ်ႈ result;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadULongLE:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    ပွၵ်ႈ result;
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_ReadFields( FT_Stream              stream,
                        const FT_Frame_Field*  fields,
                        void*                  structure )
  {
    FT_Error  error;
    FT_Bool   frame_accessed = 0;
    FT_Byte*  cursor;


    ပေႃး ( !fields )
      ပွၵ်ႈ FT_THROW( Invalid_Argument );

    ပေႃး ( !stream )
      ပွၵ်ႈ FT_THROW( Invalid_Stream_Handle );

    cursor = stream->cursor;

    error = FT_Err_Ok;
    do
    {
      FT_ULong  value;
      FT_Int    sign_shift;
      FT_Byte*  p;


      switch ( fields->value )
      {
      pak_sa ft_frame_start:  /* access a new frame */
        error = FT_Stream_EnterFrame( stream, fields->offset );
        ပေႃး ( error )
          goto Exit;

        frame_accessed = 1;
        cursor         = stream->cursor;
        fields++;
        continue;  /* loop! */

      pak_sa ft_frame_bytes:  /* read a byte sequence */
      pak_sa ft_frame_skip:   /* skip some bytes      */
        {
          FT_Offset  len = fields->size;


          ပေႃး ( len > (FT_Offset)( stream->limit - cursor ) )
          {
            error = FT_THROW( Invalid_Stream_Operation );
            goto Exit;
          }

          ပေႃး ( fields->value == ft_frame_bytes )
          {
            p = (FT_Byte*)structure + fields->offset;
            FT_MEM_COPY( p, cursor, len );
          }
          cursor += len;
          fields++;
          continue;
        }

      pak_sa ft_frame_byte:
      pak_sa ft_frame_schar:  /* read a single byte */
        value = FT_NEXT_BYTE( cursor );
        sign_shift = 24;
        bha_nga;

      pak_sa ft_frame_short_be:
      pak_sa ft_frame_ushort_be:  /* read a 2-byte big-endian short */
        value = FT_NEXT_USHORT( cursor );
        sign_shift = 16;
        bha_nga;

      pak_sa ft_frame_short_le:
      pak_sa ft_frame_ushort_le:  /* read a 2-byte little-endian short */
        value = FT_NEXT_USHORT_LE( cursor );
        sign_shift = 16;
        bha_nga;

      pak_sa ft_frame_long_be:
      pak_sa ft_frame_ulong_be:  /* read a 4-byte big-endian long */
        value = FT_NEXT_ULONG( cursor );
        sign_shift = 0;
        bha_nga;

      pak_sa ft_frame_long_le:
      pak_sa ft_frame_ulong_le:  /* read a 4-byte little-endian long */
        value = FT_NEXT_ULONG_LE( cursor );
        sign_shift = 0;
        bha_nga;

      pak_sa ft_frame_off3_be:
      pak_sa ft_frame_uoff3_be:  /* read a 3-byte big-endian long */
        value = FT_NEXT_UOFF3( cursor );
        sign_shift = 8;
        bha_nga;

      pak_sa ft_frame_off3_le:
      pak_sa ft_frame_uoff3_le:  /* read a 3-byte little-endian long */
        value = FT_NEXT_UOFF3_LE( cursor );
        sign_shift = 8;
        bha_nga;

      default:
        /* otherwise, exit the loop */
        stream->cursor = cursor;
        goto Exit;
      }

      /* now, compute the signed value if necessary */
      ပေႃး ( fields->value & FT_FRAME_OP_SIGNED )
        value = (FT_ULong)( (FT_Int32)( value << sign_shift ) >> sign_shift );

      /* finally, store the value in the object */

      p = (FT_Byte*)structure + fields->offset;
      switch ( fields->size )
      {
      pak_sa ( 8 / FT_CHAR_BIT ):
        *(FT_Byte*)p = (FT_Byte)value;
        bha_nga;

      pak_sa ( 16 / FT_CHAR_BIT ):
        *(FT_UShort*)p = (FT_UShort)value;
        bha_nga;

      pak_sa ( 32 / FT_CHAR_BIT ):
        *(FT_UInt32*)p = (FT_UInt32)value;
        bha_nga;

      default:  /* for 64-bit systems */
        *(FT_ULong*)p = (FT_ULong)value;
      }

      /* go to next field */
      fields++;
    }
    ၶၢဝ်း ( 1 );

  Exit:
    /* close the frame if it was opened by this read */
    ပေႃး ( frame_accessed )
      FT_Stream_ExitFrame( stream );

    ပွၵ်ႈ error;
  }


/* END */
