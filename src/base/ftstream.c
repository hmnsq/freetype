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


#அந்தர்பாவ <freetype/internal/ftstream.h>
#அந்தர்பாவ <freetype/internal/ftdebug.h>


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#undef  FT_COMPONENT
#define FT_COMPONENT  stream


  FT_BASE_DEF( வெறுமை )
  FT_Stream_OpenMemory( FT_Stream       stream,
                        மாறிலி FT_Byte*  base,
                        FT_ULong        size )
  {
    stream->base   = (FT_Byte*) base;
    stream->size   = size;
    stream->pos    = 0;
    stream->cursor = வெற்று;
    stream->பட   = வெற்று;
    stream->close  = வெற்று;
  }


  FT_BASE_DEF( வெறுமை )
  FT_Stream_Close( FT_Stream  stream )
  {
    எனில் ( stream && stream->close )
      stream->close( stream );
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_Seek( FT_Stream  stream,
                  FT_ULong   pos )
  {
    FT_Error  error = FT_Err_Ok;


    எனில் ( stream->பட )
    {
      எனில் ( stream->பட( stream, pos, வெற்று, 0 ) )
      {
        FT_ERROR(( "FT_Stream_Seek:"
                   " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
                   pos, stream->size ));

        error = FT_THROW( Invalid_Stream_Operation );
      }
    }
    /* note that seeking to the first position after the file is valid */
    இல்லையெனில் எனில் ( pos > stream->size )
    {
      FT_ERROR(( "FT_Stream_Seek:"
                 " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
                 pos, stream->size ));

      error = FT_THROW( Invalid_Stream_Operation );
    }

    எனில் ( !error )
      stream->pos = pos;

    திரும்பு error;
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_Skip( FT_Stream  stream,
                  FT_Long    distance )
  {
    எனில் ( distance < 0 )
      திரும்பு FT_THROW( Invalid_Stream_Operation );

    திரும்பு FT_Stream_Seek( stream, stream->pos + (FT_ULong)distance );
  }


  FT_BASE_DEF( FT_ULong )
  FT_Stream_Pos( FT_Stream  stream )
  {
    திரும்பு stream->pos;
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_Read( FT_Stream  stream,
                  FT_Byte*   buffer,
                  FT_ULong   மொத்தம் )
  {
    திரும்பு FT_Stream_ReadAt( stream, stream->pos, buffer, மொத்தம் );
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_ReadAt( FT_Stream  stream,
                    FT_ULong   pos,
                    FT_Byte*   buffer,
                    FT_ULong   மொத்தம் )
  {
    FT_Error  error = FT_Err_Ok;
    FT_ULong  read_bytes;


    எனில் ( pos >= stream->size )
    {
      FT_ERROR(( "FT_Stream_ReadAt:"
                 " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
                 pos, stream->size ));

      திரும்பு FT_THROW( Invalid_Stream_Operation );
    }

    எனில் ( stream->பட )
      read_bytes = stream->பட( stream, pos, buffer, மொத்தம் );
    இல்லையெனில்
    {
      read_bytes = stream->size - pos;
      எனில் ( read_bytes > மொத்தம் )
        read_bytes = மொத்தம்;

      /* Allow "reading" zero bytes without UB even if buffer is NULL */
      எனில் ( மொத்தம் )
        FT_MEM_COPY( buffer, stream->base + pos, read_bytes );
    }

    stream->pos = pos + read_bytes;

    எனில் ( read_bytes < மொத்தம் )
    {
      FT_ERROR(( "FT_Stream_ReadAt:"
                 " invalid read; expected %lu bytes, got %lu\n",
                 மொத்தம், read_bytes ));

      error = FT_THROW( Invalid_Stream_Operation );
    }

    திரும்பு error;
  }


  FT_BASE_DEF( FT_ULong )
  FT_Stream_TryRead( FT_Stream  stream,
                     FT_Byte*   buffer,
                     FT_ULong   மொத்தம் )
  {
    FT_ULong  read_bytes = 0;


    எனில் ( stream->pos >= stream->size )
      செல் Exit;

    எனில் ( stream->பட )
      read_bytes = stream->பட( stream, stream->pos, buffer, மொத்தம் );
    இல்லையெனில்
    {
      read_bytes = stream->size - stream->pos;
      எனில் ( read_bytes > மொத்தம் )
        read_bytes = மொத்தம்;

      /* Allow "reading" zero bytes without UB even if buffer is NULL */
      எனில் ( மொத்தம் )
        FT_MEM_COPY( buffer, stream->base + stream->pos, read_bytes );
    }

    stream->pos += read_bytes;

  Exit:
    திரும்பு read_bytes;
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_ExtractFrame( FT_Stream  stream,
                          FT_ULong   மொத்தம்,
                          FT_Byte**  pbytes )
  {
    FT_Error  error;


    error = FT_Stream_EnterFrame( stream, மொத்தம் );
    எனில் ( !error )
    {
      *pbytes = (FT_Byte*)stream->cursor;

      /* equivalent to FT_Stream_ExitFrame(), with no memory block release */
      stream->cursor = வெற்று;
      stream->limit  = வெற்று;
    }

    திரும்பு error;
  }


  FT_BASE_DEF( வெறுமை )
  FT_Stream_ReleaseFrame( FT_Stream  stream,
                          FT_Byte**  pbytes )
  {
    எனில் ( stream && stream->பட )
    {
      FT_Memory  memory = stream->memory;


#ifdef FT_DEBUG_MEMORY
      ft_mem_free( memory, *pbytes );
#இல்லையெனில்
      FT_FREE( *pbytes );
#endif
    }

    *pbytes = வெற்று;
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_EnterFrame( FT_Stream  stream,
                        FT_ULong   மொத்தம் )
  {
    FT_Error  error = FT_Err_Ok;
    FT_ULong  read_bytes;


    FT_TRACE7(( "FT_Stream_EnterFrame: %lu bytes\n", மொத்தம் ));

    /* check for nested frame access */
    FT_ASSERT( stream && stream->cursor == 0 );

    எனில் ( stream->பட )
    {
      /* allocate the frame in memory */
      FT_Memory  memory = stream->memory;


      /* simple sanity check */
      எனில் ( மொத்தம் > stream->size )
      {
        FT_ERROR(( "FT_Stream_EnterFrame:"
                   " frame size (%lu) larger than stream size (%lu)\n",
                   மொத்தம், stream->size ));

        error = FT_THROW( Invalid_Stream_Operation );
        செல் Exit;
      }

#ifdef FT_DEBUG_MEMORY
      /* assume `ft_debug_file_` and `ft_debug_lineno_` are already set */
      stream->base = (unsigned எழுத்து*)ft_mem_qalloc( memory,
                                                    (FT_Long)மொத்தம்,
                                                    &error );
      எனில் ( error )
        செல் Exit;
#இல்லையெனில்
      எனில் ( FT_QALLOC( stream->base, மொத்தம் ) )
        செல் Exit;
#endif
      /* read it */
      read_bytes = stream->பட( stream, stream->pos,
                                 stream->base, மொத்தம் );
      எனில் ( read_bytes < மொத்தம் )
      {
        FT_ERROR(( "FT_Stream_EnterFrame:"
                   " invalid read; expected %lu bytes, got %lu\n",
                   மொத்தம், read_bytes ));

        FT_FREE( stream->base );
        error = FT_THROW( Invalid_Stream_Operation );
      }

      stream->cursor = stream->base;
      stream->limit  = FT_OFFSET( stream->cursor, மொத்தம் );
      stream->pos   += read_bytes;
    }
    இல்லையெனில்
    {
      /* check current and new position */
      எனில் ( stream->pos >= stream->size        ||
           stream->size - stream->pos < மொத்தம் )
      {
        FT_ERROR(( "FT_Stream_EnterFrame:"
                   " invalid i/o; pos = 0x%lx, count = %lu, size = 0x%lx\n",
                   stream->pos, மொத்தம், stream->size ));

        error = FT_THROW( Invalid_Stream_Operation );
        செல் Exit;
      }

      /* set cursor */
      stream->cursor = stream->base + stream->pos;
      stream->limit  = stream->cursor + மொத்தம்;
      stream->pos   += மொத்தம்;
    }

  Exit:
    திரும்பு error;
  }


  FT_BASE_DEF( வெறுமை )
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

    எனில் ( stream->பட )
    {
      FT_Memory  memory = stream->memory;


#ifdef FT_DEBUG_MEMORY
      ft_mem_free( memory, stream->base );
      stream->base = வெற்று;
#இல்லையெனில்
      FT_FREE( stream->base );
#endif
    }

    stream->cursor = வெற்று;
    stream->limit  = வெற்று;
  }


  FT_BASE_DEF( FT_Byte )
  FT_Stream_GetByte( FT_Stream  stream )
  {
    FT_Byte  result;


    FT_ASSERT( stream && stream->cursor );

    result = 0;
    எனில் ( stream->cursor < stream->limit )
      result = *stream->cursor++;

    திரும்பு result;
  }


  FT_BASE_DEF( FT_UInt16 )
  FT_Stream_GetUShort( FT_Stream  stream )
  {
    FT_Byte*   p;
    FT_UInt16  result;


    FT_ASSERT( stream && stream->cursor );

    result         = 0;
    p              = stream->cursor;
    எனில் ( p + 1 < stream->limit )
      result       = FT_NEXT_USHORT( p );
    stream->cursor = p;

    திரும்பு result;
  }


  FT_BASE_DEF( FT_UInt16 )
  FT_Stream_GetUShortLE( FT_Stream  stream )
  {
    FT_Byte*   p;
    FT_UInt16  result;


    FT_ASSERT( stream && stream->cursor );

    result         = 0;
    p              = stream->cursor;
    எனில் ( p + 1 < stream->limit )
      result       = FT_NEXT_USHORT_LE( p );
    stream->cursor = p;

    திரும்பு result;
  }


  FT_BASE_DEF( FT_UInt32 )
  FT_Stream_GetUOffset( FT_Stream  stream )
  {
    FT_Byte*  p;
    FT_UInt32 result;


    FT_ASSERT( stream && stream->cursor );

    result         = 0;
    p              = stream->cursor;
    எனில் ( p + 2 < stream->limit )
      result       = FT_NEXT_UOFF3( p );
    stream->cursor = p;
    திரும்பு result;
  }


  FT_BASE_DEF( FT_UInt32 )
  FT_Stream_GetULong( FT_Stream  stream )
  {
    FT_Byte*  p;
    FT_UInt32 result;


    FT_ASSERT( stream && stream->cursor );

    result         = 0;
    p              = stream->cursor;
    எனில் ( p + 3 < stream->limit )
      result       = FT_NEXT_ULONG( p );
    stream->cursor = p;
    திரும்பு result;
  }


  FT_BASE_DEF( FT_UInt32 )
  FT_Stream_GetULongLE( FT_Stream  stream )
  {
    FT_Byte*  p;
    FT_UInt32 result;


    FT_ASSERT( stream && stream->cursor );

    result         = 0;
    p              = stream->cursor;
    எனில் ( p + 3 < stream->limit )
      result       = FT_NEXT_ULONG_LE( p );
    stream->cursor = p;
    திரும்பு result;
  }


  FT_BASE_DEF( FT_Byte )
  FT_Stream_ReadByte( FT_Stream  stream,
                      FT_Error*  error )
  {
    FT_Byte  result = 0;


    FT_ASSERT( stream );

    எனில் ( stream->pos < stream->size )
    {
      எனில் ( stream->பட )
      {
        எனில் ( stream->பட( stream, stream->pos, &result, 1L ) != 1L )
          செல் Fail;
      }
      இல்லையெனில்
        result = stream->base[stream->pos];
    }
    இல்லையெனில்
      செல் Fail;

    stream->pos++;

    *error = FT_Err_Ok;

    திரும்பு result;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadByte:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    திரும்பு result;
  }


  FT_BASE_DEF( FT_UInt16 )
  FT_Stream_ReadUShort( FT_Stream  stream,
                        FT_Error*  error )
  {
    FT_Byte    reads[2];
    FT_Byte*   p;
    FT_UInt16  result = 0;


    FT_ASSERT( stream );

    எனில் ( stream->pos + 1 < stream->size )
    {
      எனில் ( stream->பட )
      {
        எனில் ( stream->பட( stream, stream->pos, reads, 2L ) != 2L )
          செல் Fail;

        p = reads;
      }
      இல்லையெனில்
        p = stream->base + stream->pos;

      எனில் ( p )
        result = FT_NEXT_USHORT( p );
    }
    இல்லையெனில்
      செல் Fail;

    stream->pos += 2;

    *error = FT_Err_Ok;

    திரும்பு result;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadUShort:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    திரும்பு result;
  }


  FT_BASE_DEF( FT_UInt16 )
  FT_Stream_ReadUShortLE( FT_Stream  stream,
                          FT_Error*  error )
  {
    FT_Byte    reads[2];
    FT_Byte*   p;
    FT_UInt16  result = 0;


    FT_ASSERT( stream );

    எனில் ( stream->pos + 1 < stream->size )
    {
      எனில் ( stream->பட )
      {
        எனில் ( stream->பட( stream, stream->pos, reads, 2L ) != 2L )
          செல் Fail;

        p = reads;
      }
      இல்லையெனில்
        p = stream->base + stream->pos;

      எனில் ( p )
        result = FT_NEXT_USHORT_LE( p );
    }
    இல்லையெனில்
      செல் Fail;

    stream->pos += 2;

    *error = FT_Err_Ok;

    திரும்பு result;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadUShortLE:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    திரும்பு result;
  }


  FT_BASE_DEF( FT_ULong )
  FT_Stream_ReadUOffset( FT_Stream  stream,
                         FT_Error*  error )
  {
    FT_Byte   reads[3];
    FT_Byte*  p;
    FT_ULong  result = 0;


    FT_ASSERT( stream );

    எனில் ( stream->pos + 2 < stream->size )
    {
      எனில் ( stream->பட )
      {
        எனில் (stream->பட( stream, stream->pos, reads, 3L ) != 3L )
          செல் Fail;

        p = reads;
      }
      இல்லையெனில்
        p = stream->base + stream->pos;

      எனில் ( p )
        result = FT_NEXT_UOFF3( p );
    }
    இல்லையெனில்
      செல் Fail;

    stream->pos += 3;

    *error = FT_Err_Ok;

    திரும்பு result;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadUOffset:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    திரும்பு result;
  }


  FT_BASE_DEF( FT_UInt32 )
  FT_Stream_ReadULong( FT_Stream  stream,
                       FT_Error*  error )
  {
    FT_Byte   reads[4];
    FT_Byte*  p;
    FT_UInt32 result = 0;


    FT_ASSERT( stream );

    எனில் ( stream->pos + 3 < stream->size )
    {
      எனில் ( stream->பட )
      {
        எனில் ( stream->பட( stream, stream->pos, reads, 4L ) != 4L )
          செல் Fail;

        p = reads;
      }
      இல்லையெனில்
        p = stream->base + stream->pos;

      எனில் ( p )
        result = FT_NEXT_ULONG( p );
    }
    இல்லையெனில்
      செல் Fail;

    stream->pos += 4;

    *error = FT_Err_Ok;

    திரும்பு result;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadULong:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    திரும்பு result;
  }


  FT_BASE_DEF( FT_UInt32 )
  FT_Stream_ReadULongLE( FT_Stream  stream,
                         FT_Error*  error )
  {
    FT_Byte   reads[4];
    FT_Byte*  p;
    FT_UInt32 result = 0;


    FT_ASSERT( stream );

    எனில் ( stream->pos + 3 < stream->size )
    {
      எனில் ( stream->பட )
      {
        எனில் ( stream->பட( stream, stream->pos, reads, 4L ) != 4L )
          செல் Fail;

        p = reads;
      }
      இல்லையெனில்
        p = stream->base + stream->pos;

      எனில் ( p )
        result = FT_NEXT_ULONG_LE( p );
    }
    இல்லையெனில்
      செல் Fail;

    stream->pos += 4;

    *error = FT_Err_Ok;

    திரும்பு result;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadULongLE:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    திரும்பு result;
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_ReadFields( FT_Stream              stream,
                        மாறிலி FT_Frame_Field*  fields,
                        வெறுமை*                  structure )
  {
    FT_Error  error;
    FT_Bool   frame_accessed = 0;
    FT_Byte*  cursor;


    எனில் ( !fields )
      திரும்பு FT_THROW( Invalid_Argument );

    எனில் ( !stream )
      திரும்பு FT_THROW( Invalid_Stream_Handle );

    cursor = stream->cursor;

    error = FT_Err_Ok;
    செய்
    {
      FT_ULong  மதிப்பு;
      FT_Int    sign_shift;
      FT_Byte*  p;


      தேர்வு ( fields->மதிப்பு )
      {
      நிலை ft_frame_start:  /* access a new frame */
        error = FT_Stream_EnterFrame( stream, fields->offset );
        எனில் ( error )
          செல் Exit;

        frame_accessed = 1;
        cursor         = stream->cursor;
        fields++;
        தொடர்;  /* loop! */

      நிலை ft_frame_bytes:  /* read a byte sequence */
      நிலை ft_frame_skip:   /* skip some bytes      */
        {
          FT_Offset  len = fields->size;


          எனில் ( len > (FT_Offset)( stream->limit - cursor ) )
          {
            error = FT_THROW( Invalid_Stream_Operation );
            செல் Exit;
          }

          எனில் ( fields->மதிப்பு == ft_frame_bytes )
          {
            p = (FT_Byte*)structure + fields->offset;
            FT_MEM_COPY( p, cursor, len );
          }
          cursor += len;
          fields++;
          தொடர்;
        }

      நிலை ft_frame_byte:
      நிலை ft_frame_schar:  /* read a single byte */
        மதிப்பு = FT_NEXT_BYTE( cursor );
        sign_shift = 24;
        நிறுத்து;

      நிலை ft_frame_short_be:
      நிலை ft_frame_ushort_be:  /* read a 2-byte big-endian short */
        மதிப்பு = FT_NEXT_USHORT( cursor );
        sign_shift = 16;
        நிறுத்து;

      நிலை ft_frame_short_le:
      நிலை ft_frame_ushort_le:  /* read a 2-byte little-endian short */
        மதிப்பு = FT_NEXT_USHORT_LE( cursor );
        sign_shift = 16;
        நிறுத்து;

      நிலை ft_frame_long_be:
      நிலை ft_frame_ulong_be:  /* read a 4-byte big-endian long */
        மதிப்பு = FT_NEXT_ULONG( cursor );
        sign_shift = 0;
        நிறுத்து;

      நிலை ft_frame_long_le:
      நிலை ft_frame_ulong_le:  /* read a 4-byte little-endian long */
        மதிப்பு = FT_NEXT_ULONG_LE( cursor );
        sign_shift = 0;
        நிறுத்து;

      நிலை ft_frame_off3_be:
      நிலை ft_frame_uoff3_be:  /* read a 3-byte big-endian long */
        மதிப்பு = FT_NEXT_UOFF3( cursor );
        sign_shift = 8;
        நிறுத்து;

      நிலை ft_frame_off3_le:
      நிலை ft_frame_uoff3_le:  /* read a 3-byte little-endian long */
        மதிப்பு = FT_NEXT_UOFF3_LE( cursor );
        sign_shift = 8;
        நிறுத்து;

      இயல்பு:
        /* otherwise, exit the loop */
        stream->cursor = cursor;
        செல் Exit;
      }

      /* now, compute the signed value if necessary */
      எனில் ( fields->மதிப்பு & FT_FRAME_OP_SIGNED )
        மதிப்பு = (FT_ULong)( (FT_Int32)( மதிப்பு << sign_shift ) >> sign_shift );

      /* finally, store the value in the object */

      p = (FT_Byte*)structure + fields->offset;
      தேர்வு ( fields->size )
      {
      நிலை ( 8 / FT_CHAR_BIT ):
        *(FT_Byte*)p = (FT_Byte)மதிப்பு;
        நிறுத்து;

      நிலை ( 16 / FT_CHAR_BIT ):
        *(FT_UShort*)p = (FT_UShort)மதிப்பு;
        நிறுத்து;

      நிலை ( 32 / FT_CHAR_BIT ):
        *(FT_UInt32*)p = (FT_UInt32)மதிப்பு;
        நிறுத்து;

      இயல்பு:  /* for 64-bit systems */
        *(FT_ULong*)p = (FT_ULong)மதிப்பு;
      }

      /* go to next field */
      fields++;
    }
    போது ( 1 );

  Exit:
    /* close the frame if it was opened by this read */
    எனில் ( frame_accessed )
      FT_Stream_ExitFrame( stream );

    திரும்பு error;
  }


/* END */
