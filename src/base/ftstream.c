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


#شامل <freetype/internal/ftstream.h>
#شامل <freetype/internal/ftdebug.h>


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#undef  FT_COMPONENT
#define FT_COMPONENT  stream


  FT_BASE_DEF( تهی )
  FT_Stream_OpenMemory( FT_Stream       stream,
                        ثابت FT_Byte*  base,
                        FT_ULong        size )
  {
    stream->base   = (FT_Byte*) base;
    stream->size   = size;
    stream->pos    = 0;
    stream->cursor = پوچ;
    stream->اقرأ   = پوچ;
    stream->close  = پوچ;
  }


  FT_BASE_DEF( تهی )
  FT_Stream_Close( FT_Stream  stream )
  {
    اگر ( stream && stream->close )
      stream->close( stream );
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_Seek( FT_Stream  stream,
                  FT_ULong   pos )
  {
    FT_Error  error = FT_Err_Ok;


    اگر ( stream->اقرأ )
    {
      اگر ( stream->اقرأ( stream, pos, پوچ, 0 ) )
      {
        FT_ERROR(( "FT_Stream_Seek:"
                   " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
                   pos, stream->size ));

        error = FT_THROW( Invalid_Stream_Operation );
      }
    }
    /* note that seeking to the first position after the file is valid */
    وگرنه اگر ( pos > stream->size )
    {
      FT_ERROR(( "FT_Stream_Seek:"
                 " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
                 pos, stream->size ));

      error = FT_THROW( Invalid_Stream_Operation );
    }

    اگر ( !error )
      stream->pos = pos;

    بازگشت error;
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_Skip( FT_Stream  stream,
                  FT_Long    distance )
  {
    اگر ( distance < 0 )
      بازگشت FT_THROW( Invalid_Stream_Operation );

    بازگشت FT_Stream_Seek( stream, stream->pos + (FT_ULong)distance );
  }


  FT_BASE_DEF( FT_ULong )
  FT_Stream_Pos( FT_Stream  stream )
  {
    بازگشت stream->pos;
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_Read( FT_Stream  stream,
                  FT_Byte*   buffer,
                  FT_ULong   شمار )
  {
    بازگشت FT_Stream_ReadAt( stream, stream->pos, buffer, شمار );
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_ReadAt( FT_Stream  stream,
                    FT_ULong   pos,
                    FT_Byte*   buffer,
                    FT_ULong   شمار )
  {
    FT_Error  error = FT_Err_Ok;
    FT_ULong  read_bytes;


    اگر ( pos >= stream->size )
    {
      FT_ERROR(( "FT_Stream_ReadAt:"
                 " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
                 pos, stream->size ));

      بازگشت FT_THROW( Invalid_Stream_Operation );
    }

    اگر ( stream->اقرأ )
      read_bytes = stream->اقرأ( stream, pos, buffer, شمار );
    وگرنه
    {
      read_bytes = stream->size - pos;
      اگر ( read_bytes > شمار )
        read_bytes = شمار;

      /* Allow "reading" zero bytes without UB even if buffer is NULL */
      اگر ( شمار )
        FT_MEM_COPY( buffer, stream->base + pos, read_bytes );
    }

    stream->pos = pos + read_bytes;

    اگر ( read_bytes < شمار )
    {
      FT_ERROR(( "FT_Stream_ReadAt:"
                 " invalid read; expected %lu bytes, got %lu\n",
                 شمار, read_bytes ));

      error = FT_THROW( Invalid_Stream_Operation );
    }

    بازگشت error;
  }


  FT_BASE_DEF( FT_ULong )
  FT_Stream_TryRead( FT_Stream  stream,
                     FT_Byte*   buffer,
                     FT_ULong   شمار )
  {
    FT_ULong  read_bytes = 0;


    اگر ( stream->pos >= stream->size )
      برو Exit;

    اگر ( stream->اقرأ )
      read_bytes = stream->اقرأ( stream, stream->pos, buffer, شمار );
    وگرنه
    {
      read_bytes = stream->size - stream->pos;
      اگر ( read_bytes > شمار )
        read_bytes = شمار;

      /* Allow "reading" zero bytes without UB even if buffer is NULL */
      اگر ( شمار )
        FT_MEM_COPY( buffer, stream->base + stream->pos, read_bytes );
    }

    stream->pos += read_bytes;

  Exit:
    بازگشت read_bytes;
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_ExtractFrame( FT_Stream  stream,
                          FT_ULong   شمار,
                          FT_Byte**  pbytes )
  {
    FT_Error  error;


    error = FT_Stream_EnterFrame( stream, شمار );
    اگر ( !error )
    {
      *pbytes = (FT_Byte*)stream->cursor;

      /* equivalent to FT_Stream_ExitFrame(), with no memory block release */
      stream->cursor = پوچ;
      stream->limit  = پوچ;
    }

    بازگشت error;
  }


  FT_BASE_DEF( تهی )
  FT_Stream_ReleaseFrame( FT_Stream  stream,
                          FT_Byte**  pbytes )
  {
    اگر ( stream && stream->اقرأ )
    {
      FT_Memory  memory = stream->memory;


#ifdef FT_DEBUG_MEMORY
      ft_mem_free( memory, *pbytes );
#وگرنه
      FT_FREE( *pbytes );
#endif
    }

    *pbytes = پوچ;
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_EnterFrame( FT_Stream  stream,
                        FT_ULong   شمار )
  {
    FT_Error  error = FT_Err_Ok;
    FT_ULong  read_bytes;


    FT_TRACE7(( "FT_Stream_EnterFrame: %lu bytes\n", شمار ));

    /* check for nested frame access */
    FT_ASSERT( stream && stream->cursor == 0 );

    اگر ( stream->اقرأ )
    {
      /* allocate the frame in memory */
      FT_Memory  memory = stream->memory;


      /* simple sanity check */
      اگر ( شمار > stream->size )
      {
        FT_ERROR(( "FT_Stream_EnterFrame:"
                   " frame size (%lu) larger than stream size (%lu)\n",
                   شمار, stream->size ));

        error = FT_THROW( Invalid_Stream_Operation );
        برو Exit;
      }

#ifdef FT_DEBUG_MEMORY
      /* assume `ft_debug_file_` and `ft_debug_lineno_` are already set */
      stream->base = (unsigned نویسه*)ft_mem_qalloc( memory,
                                                    (FT_Long)شمار,
                                                    &error );
      اگر ( error )
        برو Exit;
#وگرنه
      اگر ( FT_QALLOC( stream->base, شمار ) )
        برو Exit;
#endif
      /* read it */
      read_bytes = stream->اقرأ( stream, stream->pos,
                                 stream->base, شمار );
      اگر ( read_bytes < شمار )
      {
        FT_ERROR(( "FT_Stream_EnterFrame:"
                   " invalid read; expected %lu bytes, got %lu\n",
                   شمار, read_bytes ));

        FT_FREE( stream->base );
        error = FT_THROW( Invalid_Stream_Operation );
      }

      stream->cursor = stream->base;
      stream->limit  = FT_OFFSET( stream->cursor, شمار );
      stream->pos   += read_bytes;
    }
    وگرنه
    {
      /* check current and new position */
      اگر ( stream->pos >= stream->size        ||
           stream->size - stream->pos < شمار )
      {
        FT_ERROR(( "FT_Stream_EnterFrame:"
                   " invalid i/o; pos = 0x%lx, count = %lu, size = 0x%lx\n",
                   stream->pos, شمار, stream->size ));

        error = FT_THROW( Invalid_Stream_Operation );
        برو Exit;
      }

      /* set cursor */
      stream->cursor = stream->base + stream->pos;
      stream->limit  = stream->cursor + شمار;
      stream->pos   += شمار;
    }

  Exit:
    بازگشت error;
  }


  FT_BASE_DEF( تهی )
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

    اگر ( stream->اقرأ )
    {
      FT_Memory  memory = stream->memory;


#ifdef FT_DEBUG_MEMORY
      ft_mem_free( memory, stream->base );
      stream->base = پوچ;
#وگرنه
      FT_FREE( stream->base );
#endif
    }

    stream->cursor = پوچ;
    stream->limit  = پوچ;
  }


  FT_BASE_DEF( FT_Byte )
  FT_Stream_GetByte( FT_Stream  stream )
  {
    FT_Byte  result;


    FT_ASSERT( stream && stream->cursor );

    result = 0;
    اگر ( stream->cursor < stream->limit )
      result = *stream->cursor++;

    بازگشت result;
  }


  FT_BASE_DEF( FT_UInt16 )
  FT_Stream_GetUShort( FT_Stream  stream )
  {
    FT_Byte*   p;
    FT_UInt16  result;


    FT_ASSERT( stream && stream->cursor );

    result         = 0;
    p              = stream->cursor;
    اگر ( p + 1 < stream->limit )
      result       = FT_NEXT_USHORT( p );
    stream->cursor = p;

    بازگشت result;
  }


  FT_BASE_DEF( FT_UInt16 )
  FT_Stream_GetUShortLE( FT_Stream  stream )
  {
    FT_Byte*   p;
    FT_UInt16  result;


    FT_ASSERT( stream && stream->cursor );

    result         = 0;
    p              = stream->cursor;
    اگر ( p + 1 < stream->limit )
      result       = FT_NEXT_USHORT_LE( p );
    stream->cursor = p;

    بازگشت result;
  }


  FT_BASE_DEF( FT_UInt32 )
  FT_Stream_GetUOffset( FT_Stream  stream )
  {
    FT_Byte*  p;
    FT_UInt32 result;


    FT_ASSERT( stream && stream->cursor );

    result         = 0;
    p              = stream->cursor;
    اگر ( p + 2 < stream->limit )
      result       = FT_NEXT_UOFF3( p );
    stream->cursor = p;
    بازگشت result;
  }


  FT_BASE_DEF( FT_UInt32 )
  FT_Stream_GetULong( FT_Stream  stream )
  {
    FT_Byte*  p;
    FT_UInt32 result;


    FT_ASSERT( stream && stream->cursor );

    result         = 0;
    p              = stream->cursor;
    اگر ( p + 3 < stream->limit )
      result       = FT_NEXT_ULONG( p );
    stream->cursor = p;
    بازگشت result;
  }


  FT_BASE_DEF( FT_UInt32 )
  FT_Stream_GetULongLE( FT_Stream  stream )
  {
    FT_Byte*  p;
    FT_UInt32 result;


    FT_ASSERT( stream && stream->cursor );

    result         = 0;
    p              = stream->cursor;
    اگر ( p + 3 < stream->limit )
      result       = FT_NEXT_ULONG_LE( p );
    stream->cursor = p;
    بازگشت result;
  }


  FT_BASE_DEF( FT_Byte )
  FT_Stream_ReadByte( FT_Stream  stream,
                      FT_Error*  error )
  {
    FT_Byte  result = 0;


    FT_ASSERT( stream );

    اگر ( stream->pos < stream->size )
    {
      اگر ( stream->اقرأ )
      {
        اگر ( stream->اقرأ( stream, stream->pos, &result, 1L ) != 1L )
          برو Fail;
      }
      وگرنه
        result = stream->base[stream->pos];
    }
    وگرنه
      برو Fail;

    stream->pos++;

    *error = FT_Err_Ok;

    بازگشت result;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadByte:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    بازگشت result;
  }


  FT_BASE_DEF( FT_UInt16 )
  FT_Stream_ReadUShort( FT_Stream  stream,
                        FT_Error*  error )
  {
    FT_Byte    reads[2];
    FT_Byte*   p;
    FT_UInt16  result = 0;


    FT_ASSERT( stream );

    اگر ( stream->pos + 1 < stream->size )
    {
      اگر ( stream->اقرأ )
      {
        اگر ( stream->اقرأ( stream, stream->pos, reads, 2L ) != 2L )
          برو Fail;

        p = reads;
      }
      وگرنه
        p = stream->base + stream->pos;

      اگر ( p )
        result = FT_NEXT_USHORT( p );
    }
    وگرنه
      برو Fail;

    stream->pos += 2;

    *error = FT_Err_Ok;

    بازگشت result;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadUShort:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    بازگشت result;
  }


  FT_BASE_DEF( FT_UInt16 )
  FT_Stream_ReadUShortLE( FT_Stream  stream,
                          FT_Error*  error )
  {
    FT_Byte    reads[2];
    FT_Byte*   p;
    FT_UInt16  result = 0;


    FT_ASSERT( stream );

    اگر ( stream->pos + 1 < stream->size )
    {
      اگر ( stream->اقرأ )
      {
        اگر ( stream->اقرأ( stream, stream->pos, reads, 2L ) != 2L )
          برو Fail;

        p = reads;
      }
      وگرنه
        p = stream->base + stream->pos;

      اگر ( p )
        result = FT_NEXT_USHORT_LE( p );
    }
    وگرنه
      برو Fail;

    stream->pos += 2;

    *error = FT_Err_Ok;

    بازگشت result;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadUShortLE:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    بازگشت result;
  }


  FT_BASE_DEF( FT_ULong )
  FT_Stream_ReadUOffset( FT_Stream  stream,
                         FT_Error*  error )
  {
    FT_Byte   reads[3];
    FT_Byte*  p;
    FT_ULong  result = 0;


    FT_ASSERT( stream );

    اگر ( stream->pos + 2 < stream->size )
    {
      اگر ( stream->اقرأ )
      {
        اگر (stream->اقرأ( stream, stream->pos, reads, 3L ) != 3L )
          برو Fail;

        p = reads;
      }
      وگرنه
        p = stream->base + stream->pos;

      اگر ( p )
        result = FT_NEXT_UOFF3( p );
    }
    وگرنه
      برو Fail;

    stream->pos += 3;

    *error = FT_Err_Ok;

    بازگشت result;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadUOffset:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    بازگشت result;
  }


  FT_BASE_DEF( FT_UInt32 )
  FT_Stream_ReadULong( FT_Stream  stream,
                       FT_Error*  error )
  {
    FT_Byte   reads[4];
    FT_Byte*  p;
    FT_UInt32 result = 0;


    FT_ASSERT( stream );

    اگر ( stream->pos + 3 < stream->size )
    {
      اگر ( stream->اقرأ )
      {
        اگر ( stream->اقرأ( stream, stream->pos, reads, 4L ) != 4L )
          برو Fail;

        p = reads;
      }
      وگرنه
        p = stream->base + stream->pos;

      اگر ( p )
        result = FT_NEXT_ULONG( p );
    }
    وگرنه
      برو Fail;

    stream->pos += 4;

    *error = FT_Err_Ok;

    بازگشت result;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadULong:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    بازگشت result;
  }


  FT_BASE_DEF( FT_UInt32 )
  FT_Stream_ReadULongLE( FT_Stream  stream,
                         FT_Error*  error )
  {
    FT_Byte   reads[4];
    FT_Byte*  p;
    FT_UInt32 result = 0;


    FT_ASSERT( stream );

    اگر ( stream->pos + 3 < stream->size )
    {
      اگر ( stream->اقرأ )
      {
        اگر ( stream->اقرأ( stream, stream->pos, reads, 4L ) != 4L )
          برو Fail;

        p = reads;
      }
      وگرنه
        p = stream->base + stream->pos;

      اگر ( p )
        result = FT_NEXT_ULONG_LE( p );
    }
    وگرنه
      برو Fail;

    stream->pos += 4;

    *error = FT_Err_Ok;

    بازگشت result;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadULongLE:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    بازگشت result;
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_ReadFields( FT_Stream              stream,
                        ثابت FT_Frame_Field*  fields,
                        تهی*                  structure )
  {
    FT_Error  error;
    FT_Bool   frame_accessed = 0;
    FT_Byte*  cursor;


    اگر ( !fields )
      بازگشت FT_THROW( Invalid_Argument );

    اگر ( !stream )
      بازگشت FT_THROW( Invalid_Stream_Handle );

    cursor = stream->cursor;

    error = FT_Err_Ok;
    انجامبده
    {
      FT_ULong  مقدار;
      FT_Int    sign_shift;
      FT_Byte*  p;


      انتخاب ( fields->مقدار )
      {
      حالت ft_frame_start:  /* access a new frame */
        error = FT_Stream_EnterFrame( stream, fields->offset );
        اگر ( error )
          برو Exit;

        frame_accessed = 1;
        cursor         = stream->cursor;
        fields++;
        ادامه;  /* loop! */

      حالت ft_frame_bytes:  /* read a byte sequence */
      حالت ft_frame_skip:   /* skip some bytes      */
        {
          FT_Offset  len = fields->size;


          اگر ( len > (FT_Offset)( stream->limit - cursor ) )
          {
            error = FT_THROW( Invalid_Stream_Operation );
            برو Exit;
          }

          اگر ( fields->مقدار == ft_frame_bytes )
          {
            p = (FT_Byte*)structure + fields->offset;
            FT_MEM_COPY( p, cursor, len );
          }
          cursor += len;
          fields++;
          ادامه;
        }

      حالت ft_frame_byte:
      حالت ft_frame_schar:  /* read a single byte */
        مقدار = FT_NEXT_BYTE( cursor );
        sign_shift = 24;
        بشکن;

      حالت ft_frame_short_be:
      حالت ft_frame_ushort_be:  /* read a 2-byte big-endian short */
        مقدار = FT_NEXT_USHORT( cursor );
        sign_shift = 16;
        بشکن;

      حالت ft_frame_short_le:
      حالت ft_frame_ushort_le:  /* read a 2-byte little-endian short */
        مقدار = FT_NEXT_USHORT_LE( cursor );
        sign_shift = 16;
        بشکن;

      حالت ft_frame_long_be:
      حالت ft_frame_ulong_be:  /* read a 4-byte big-endian long */
        مقدار = FT_NEXT_ULONG( cursor );
        sign_shift = 0;
        بشکن;

      حالت ft_frame_long_le:
      حالت ft_frame_ulong_le:  /* read a 4-byte little-endian long */
        مقدار = FT_NEXT_ULONG_LE( cursor );
        sign_shift = 0;
        بشکن;

      حالت ft_frame_off3_be:
      حالت ft_frame_uoff3_be:  /* read a 3-byte big-endian long */
        مقدار = FT_NEXT_UOFF3( cursor );
        sign_shift = 8;
        بشکن;

      حالت ft_frame_off3_le:
      حالت ft_frame_uoff3_le:  /* read a 3-byte little-endian long */
        مقدار = FT_NEXT_UOFF3_LE( cursor );
        sign_shift = 8;
        بشکن;

      پیشفرض:
        /* otherwise, exit the loop */
        stream->cursor = cursor;
        برو Exit;
      }

      /* now, compute the signed value if necessary */
      اگر ( fields->مقدار & FT_FRAME_OP_SIGNED )
        مقدار = (FT_ULong)( (FT_Int32)( مقدار << sign_shift ) >> sign_shift );

      /* finally, store the value in the object */

      p = (FT_Byte*)structure + fields->offset;
      انتخاب ( fields->size )
      {
      حالت ( 8 / FT_CHAR_BIT ):
        *(FT_Byte*)p = (FT_Byte)مقدار;
        بشکن;

      حالت ( 16 / FT_CHAR_BIT ):
        *(FT_UShort*)p = (FT_UShort)مقدار;
        بشکن;

      حالت ( 32 / FT_CHAR_BIT ):
        *(FT_UInt32*)p = (FT_UInt32)مقدار;
        بشکن;

      پیشفرض:  /* for 64-bit systems */
        *(FT_ULong*)p = (FT_ULong)مقدار;
      }

      /* go to next field */
      fields++;
    }
    وقتی ( 1 );

  Exit:
    /* close the frame if it was opened by this read */
    اگر ( frame_accessed )
      FT_Stream_ExitFrame( stream );

    بازگشت error;
  }


/* END */
