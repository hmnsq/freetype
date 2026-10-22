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


#অন্তর্ভাব <freetype/internal/ftstream.h>
#অন্তর্ভাব <freetype/internal/ftdebug.h>


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#undef  FT_COMPONENT
#define FT_COMPONENT  stream


  FT_BASE_DEF( শূন্য )
  FT_Stream_OpenMemory( FT_Stream       stream,
                        ধ্রুব FT_Byte*  base,
                        FT_ULong        size )
  {
    stream->base   = (FT_Byte*) base;
    stream->size   = size;
    stream->pos    = 0;
    stream->cursor = রিক্ত;
    stream->পঠ   = রিক্ত;
    stream->close  = রিক্ত;
  }


  FT_BASE_DEF( শূন্য )
  FT_Stream_Close( FT_Stream  stream )
  {
    যদি ( stream && stream->close )
      stream->close( stream );
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_Seek( FT_Stream  stream,
                  FT_ULong   pos )
  {
    FT_Error  error = FT_Err_Ok;


    যদি ( stream->পঠ )
    {
      যদি ( stream->পঠ( stream, pos, রিক্ত, 0 ) )
      {
        FT_ERROR(( "FT_Stream_Seek:"
                   " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
                   pos, stream->size ));

        error = FT_THROW( Invalid_Stream_Operation );
      }
    }
    /* note that seeking to the first position after the file is valid */
    অন্যথা যদি ( pos > stream->size )
    {
      FT_ERROR(( "FT_Stream_Seek:"
                 " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
                 pos, stream->size ));

      error = FT_THROW( Invalid_Stream_Operation );
    }

    যদি ( !error )
      stream->pos = pos;

    প্রত্যাগম error;
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_Skip( FT_Stream  stream,
                  FT_Long    distance )
  {
    যদি ( distance < 0 )
      প্রত্যাগম FT_THROW( Invalid_Stream_Operation );

    প্রত্যাগম FT_Stream_Seek( stream, stream->pos + (FT_ULong)distance );
  }


  FT_BASE_DEF( FT_ULong )
  FT_Stream_Pos( FT_Stream  stream )
  {
    প্রত্যাগম stream->pos;
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_Read( FT_Stream  stream,
                  FT_Byte*   buffer,
                  FT_ULong   গুনতি )
  {
    প্রত্যাগম FT_Stream_ReadAt( stream, stream->pos, buffer, গুনতি );
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_ReadAt( FT_Stream  stream,
                    FT_ULong   pos,
                    FT_Byte*   buffer,
                    FT_ULong   গুনতি )
  {
    FT_Error  error = FT_Err_Ok;
    FT_ULong  read_bytes;


    যদি ( pos >= stream->size )
    {
      FT_ERROR(( "FT_Stream_ReadAt:"
                 " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
                 pos, stream->size ));

      প্রত্যাগম FT_THROW( Invalid_Stream_Operation );
    }

    যদি ( stream->পঠ )
      read_bytes = stream->পঠ( stream, pos, buffer, গুনতি );
    অন্যথা
    {
      read_bytes = stream->size - pos;
      যদি ( read_bytes > গুনতি )
        read_bytes = গুনতি;

      /* Allow "reading" zero bytes without UB even if buffer is NULL */
      যদি ( গুনতি )
        FT_MEM_COPY( buffer, stream->base + pos, read_bytes );
    }

    stream->pos = pos + read_bytes;

    যদি ( read_bytes < গুনতি )
    {
      FT_ERROR(( "FT_Stream_ReadAt:"
                 " invalid read; expected %lu bytes, got %lu\n",
                 গুনতি, read_bytes ));

      error = FT_THROW( Invalid_Stream_Operation );
    }

    প্রত্যাগম error;
  }


  FT_BASE_DEF( FT_ULong )
  FT_Stream_TryRead( FT_Stream  stream,
                     FT_Byte*   buffer,
                     FT_ULong   গুনতি )
  {
    FT_ULong  read_bytes = 0;


    যদি ( stream->pos >= stream->size )
      গচ্ছ Exit;

    যদি ( stream->পঠ )
      read_bytes = stream->পঠ( stream, stream->pos, buffer, গুনতি );
    অন্যথা
    {
      read_bytes = stream->size - stream->pos;
      যদি ( read_bytes > গুনতি )
        read_bytes = গুনতি;

      /* Allow "reading" zero bytes without UB even if buffer is NULL */
      যদি ( গুনতি )
        FT_MEM_COPY( buffer, stream->base + stream->pos, read_bytes );
    }

    stream->pos += read_bytes;

  Exit:
    প্রত্যাগম read_bytes;
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_ExtractFrame( FT_Stream  stream,
                          FT_ULong   গুনতি,
                          FT_Byte**  pbytes )
  {
    FT_Error  error;


    error = FT_Stream_EnterFrame( stream, গুনতি );
    যদি ( !error )
    {
      *pbytes = (FT_Byte*)stream->cursor;

      /* equivalent to FT_Stream_ExitFrame(), with no memory block release */
      stream->cursor = রিক্ত;
      stream->limit  = রিক্ত;
    }

    প্রত্যাগম error;
  }


  FT_BASE_DEF( শূন্য )
  FT_Stream_ReleaseFrame( FT_Stream  stream,
                          FT_Byte**  pbytes )
  {
    যদি ( stream && stream->পঠ )
    {
      FT_Memory  memory = stream->memory;


#ifdef FT_DEBUG_MEMORY
      ft_mem_free( memory, *pbytes );
#অন্যথা
      FT_FREE( *pbytes );
#endif
    }

    *pbytes = রিক্ত;
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_EnterFrame( FT_Stream  stream,
                        FT_ULong   গুনতি )
  {
    FT_Error  error = FT_Err_Ok;
    FT_ULong  read_bytes;


    FT_TRACE7(( "FT_Stream_EnterFrame: %lu bytes\n", গুনতি ));

    /* check for nested frame access */
    FT_ASSERT( stream && stream->cursor == 0 );

    যদি ( stream->পঠ )
    {
      /* allocate the frame in memory */
      FT_Memory  memory = stream->memory;


      /* simple sanity check */
      যদি ( গুনতি > stream->size )
      {
        FT_ERROR(( "FT_Stream_EnterFrame:"
                   " frame size (%lu) larger than stream size (%lu)\n",
                   গুনতি, stream->size ));

        error = FT_THROW( Invalid_Stream_Operation );
        গচ্ছ Exit;
      }

#ifdef FT_DEBUG_MEMORY
      /* assume `ft_debug_file_` and `ft_debug_lineno_` are already set */
      stream->base = (unsigned বর্ণ*)ft_mem_qalloc( memory,
                                                    (FT_Long)গুনতি,
                                                    &error );
      যদি ( error )
        গচ্ছ Exit;
#অন্যথা
      যদি ( FT_QALLOC( stream->base, গুনতি ) )
        গচ্ছ Exit;
#endif
      /* read it */
      read_bytes = stream->পঠ( stream, stream->pos,
                                 stream->base, গুনতি );
      যদি ( read_bytes < গুনতি )
      {
        FT_ERROR(( "FT_Stream_EnterFrame:"
                   " invalid read; expected %lu bytes, got %lu\n",
                   গুনতি, read_bytes ));

        FT_FREE( stream->base );
        error = FT_THROW( Invalid_Stream_Operation );
      }

      stream->cursor = stream->base;
      stream->limit  = FT_OFFSET( stream->cursor, গুনতি );
      stream->pos   += read_bytes;
    }
    অন্যথা
    {
      /* check current and new position */
      যদি ( stream->pos >= stream->size        ||
           stream->size - stream->pos < গুনতি )
      {
        FT_ERROR(( "FT_Stream_EnterFrame:"
                   " invalid i/o; pos = 0x%lx, count = %lu, size = 0x%lx\n",
                   stream->pos, গুনতি, stream->size ));

        error = FT_THROW( Invalid_Stream_Operation );
        গচ্ছ Exit;
      }

      /* set cursor */
      stream->cursor = stream->base + stream->pos;
      stream->limit  = stream->cursor + গুনতি;
      stream->pos   += গুনতি;
    }

  Exit:
    প্রত্যাগম error;
  }


  FT_BASE_DEF( শূন্য )
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

    যদি ( stream->পঠ )
    {
      FT_Memory  memory = stream->memory;


#ifdef FT_DEBUG_MEMORY
      ft_mem_free( memory, stream->base );
      stream->base = রিক্ত;
#অন্যথা
      FT_FREE( stream->base );
#endif
    }

    stream->cursor = রিক্ত;
    stream->limit  = রিক্ত;
  }


  FT_BASE_DEF( FT_Byte )
  FT_Stream_GetByte( FT_Stream  stream )
  {
    FT_Byte  result;


    FT_ASSERT( stream && stream->cursor );

    result = 0;
    যদি ( stream->cursor < stream->limit )
      result = *stream->cursor++;

    প্রত্যাগম result;
  }


  FT_BASE_DEF( FT_UInt16 )
  FT_Stream_GetUShort( FT_Stream  stream )
  {
    FT_Byte*   p;
    FT_UInt16  result;


    FT_ASSERT( stream && stream->cursor );

    result         = 0;
    p              = stream->cursor;
    যদি ( p + 1 < stream->limit )
      result       = FT_NEXT_USHORT( p );
    stream->cursor = p;

    প্রত্যাগম result;
  }


  FT_BASE_DEF( FT_UInt16 )
  FT_Stream_GetUShortLE( FT_Stream  stream )
  {
    FT_Byte*   p;
    FT_UInt16  result;


    FT_ASSERT( stream && stream->cursor );

    result         = 0;
    p              = stream->cursor;
    যদি ( p + 1 < stream->limit )
      result       = FT_NEXT_USHORT_LE( p );
    stream->cursor = p;

    প্রত্যাগম result;
  }


  FT_BASE_DEF( FT_UInt32 )
  FT_Stream_GetUOffset( FT_Stream  stream )
  {
    FT_Byte*  p;
    FT_UInt32 result;


    FT_ASSERT( stream && stream->cursor );

    result         = 0;
    p              = stream->cursor;
    যদি ( p + 2 < stream->limit )
      result       = FT_NEXT_UOFF3( p );
    stream->cursor = p;
    প্রত্যাগম result;
  }


  FT_BASE_DEF( FT_UInt32 )
  FT_Stream_GetULong( FT_Stream  stream )
  {
    FT_Byte*  p;
    FT_UInt32 result;


    FT_ASSERT( stream && stream->cursor );

    result         = 0;
    p              = stream->cursor;
    যদি ( p + 3 < stream->limit )
      result       = FT_NEXT_ULONG( p );
    stream->cursor = p;
    প্রত্যাগম result;
  }


  FT_BASE_DEF( FT_UInt32 )
  FT_Stream_GetULongLE( FT_Stream  stream )
  {
    FT_Byte*  p;
    FT_UInt32 result;


    FT_ASSERT( stream && stream->cursor );

    result         = 0;
    p              = stream->cursor;
    যদি ( p + 3 < stream->limit )
      result       = FT_NEXT_ULONG_LE( p );
    stream->cursor = p;
    প্রত্যাগম result;
  }


  FT_BASE_DEF( FT_Byte )
  FT_Stream_ReadByte( FT_Stream  stream,
                      FT_Error*  error )
  {
    FT_Byte  result = 0;


    FT_ASSERT( stream );

    যদি ( stream->pos < stream->size )
    {
      যদি ( stream->পঠ )
      {
        যদি ( stream->পঠ( stream, stream->pos, &result, 1L ) != 1L )
          গচ্ছ Fail;
      }
      অন্যথা
        result = stream->base[stream->pos];
    }
    অন্যথা
      গচ্ছ Fail;

    stream->pos++;

    *error = FT_Err_Ok;

    প্রত্যাগম result;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadByte:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    প্রত্যাগম result;
  }


  FT_BASE_DEF( FT_UInt16 )
  FT_Stream_ReadUShort( FT_Stream  stream,
                        FT_Error*  error )
  {
    FT_Byte    reads[2];
    FT_Byte*   p;
    FT_UInt16  result = 0;


    FT_ASSERT( stream );

    যদি ( stream->pos + 1 < stream->size )
    {
      যদি ( stream->পঠ )
      {
        যদি ( stream->পঠ( stream, stream->pos, reads, 2L ) != 2L )
          গচ্ছ Fail;

        p = reads;
      }
      অন্যথা
        p = stream->base + stream->pos;

      যদি ( p )
        result = FT_NEXT_USHORT( p );
    }
    অন্যথা
      গচ্ছ Fail;

    stream->pos += 2;

    *error = FT_Err_Ok;

    প্রত্যাগম result;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadUShort:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    প্রত্যাগম result;
  }


  FT_BASE_DEF( FT_UInt16 )
  FT_Stream_ReadUShortLE( FT_Stream  stream,
                          FT_Error*  error )
  {
    FT_Byte    reads[2];
    FT_Byte*   p;
    FT_UInt16  result = 0;


    FT_ASSERT( stream );

    যদি ( stream->pos + 1 < stream->size )
    {
      যদি ( stream->পঠ )
      {
        যদি ( stream->পঠ( stream, stream->pos, reads, 2L ) != 2L )
          গচ্ছ Fail;

        p = reads;
      }
      অন্যথা
        p = stream->base + stream->pos;

      যদি ( p )
        result = FT_NEXT_USHORT_LE( p );
    }
    অন্যথা
      গচ্ছ Fail;

    stream->pos += 2;

    *error = FT_Err_Ok;

    প্রত্যাগম result;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadUShortLE:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    প্রত্যাগম result;
  }


  FT_BASE_DEF( FT_ULong )
  FT_Stream_ReadUOffset( FT_Stream  stream,
                         FT_Error*  error )
  {
    FT_Byte   reads[3];
    FT_Byte*  p;
    FT_ULong  result = 0;


    FT_ASSERT( stream );

    যদি ( stream->pos + 2 < stream->size )
    {
      যদি ( stream->পঠ )
      {
        যদি (stream->পঠ( stream, stream->pos, reads, 3L ) != 3L )
          গচ্ছ Fail;

        p = reads;
      }
      অন্যথা
        p = stream->base + stream->pos;

      যদি ( p )
        result = FT_NEXT_UOFF3( p );
    }
    অন্যথা
      গচ্ছ Fail;

    stream->pos += 3;

    *error = FT_Err_Ok;

    প্রত্যাগম result;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadUOffset:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    প্রত্যাগম result;
  }


  FT_BASE_DEF( FT_UInt32 )
  FT_Stream_ReadULong( FT_Stream  stream,
                       FT_Error*  error )
  {
    FT_Byte   reads[4];
    FT_Byte*  p;
    FT_UInt32 result = 0;


    FT_ASSERT( stream );

    যদি ( stream->pos + 3 < stream->size )
    {
      যদি ( stream->পঠ )
      {
        যদি ( stream->পঠ( stream, stream->pos, reads, 4L ) != 4L )
          গচ্ছ Fail;

        p = reads;
      }
      অন্যথা
        p = stream->base + stream->pos;

      যদি ( p )
        result = FT_NEXT_ULONG( p );
    }
    অন্যথা
      গচ্ছ Fail;

    stream->pos += 4;

    *error = FT_Err_Ok;

    প্রত্যাগম result;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadULong:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    প্রত্যাগম result;
  }


  FT_BASE_DEF( FT_UInt32 )
  FT_Stream_ReadULongLE( FT_Stream  stream,
                         FT_Error*  error )
  {
    FT_Byte   reads[4];
    FT_Byte*  p;
    FT_UInt32 result = 0;


    FT_ASSERT( stream );

    যদি ( stream->pos + 3 < stream->size )
    {
      যদি ( stream->পঠ )
      {
        যদি ( stream->পঠ( stream, stream->pos, reads, 4L ) != 4L )
          গচ্ছ Fail;

        p = reads;
      }
      অন্যথা
        p = stream->base + stream->pos;

      যদি ( p )
        result = FT_NEXT_ULONG_LE( p );
    }
    অন্যথা
      গচ্ছ Fail;

    stream->pos += 4;

    *error = FT_Err_Ok;

    প্রত্যাগম result;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadULongLE:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    প্রত্যাগম result;
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_ReadFields( FT_Stream              stream,
                        ধ্রুব FT_Frame_Field*  fields,
                        শূন্য*                  structure )
  {
    FT_Error  error;
    FT_Bool   frame_accessed = 0;
    FT_Byte*  cursor;


    যদি ( !fields )
      প্রত্যাগম FT_THROW( Invalid_Argument );

    যদি ( !stream )
      প্রত্যাগম FT_THROW( Invalid_Stream_Handle );

    cursor = stream->cursor;

    error = FT_Err_Ok;
    কুরু
    {
      FT_ULong  মান;
      FT_Int    sign_shift;
      FT_Byte*  p;


      বিকল্প ( fields->মান )
      {
      পক্ষ ft_frame_start:  /* access a new frame */
        error = FT_Stream_EnterFrame( stream, fields->offset );
        যদি ( error )
          গচ্ছ Exit;

        frame_accessed = 1;
        cursor         = stream->cursor;
        fields++;
        অনুবর্ত;  /* loop! */

      পক্ষ ft_frame_bytes:  /* read a byte sequence */
      পক্ষ ft_frame_skip:   /* skip some bytes      */
        {
          FT_Offset  len = fields->size;


          যদি ( len > (FT_Offset)( stream->limit - cursor ) )
          {
            error = FT_THROW( Invalid_Stream_Operation );
            গচ্ছ Exit;
          }

          যদি ( fields->মান == ft_frame_bytes )
          {
            p = (FT_Byte*)structure + fields->offset;
            FT_MEM_COPY( p, cursor, len );
          }
          cursor += len;
          fields++;
          অনুবর্ত;
        }

      পক্ষ ft_frame_byte:
      পক্ষ ft_frame_schar:  /* read a single byte */
        মান = FT_NEXT_BYTE( cursor );
        sign_shift = 24;
        ভঙ্গ;

      পক্ষ ft_frame_short_be:
      পক্ষ ft_frame_ushort_be:  /* read a 2-byte big-endian short */
        মান = FT_NEXT_USHORT( cursor );
        sign_shift = 16;
        ভঙ্গ;

      পক্ষ ft_frame_short_le:
      পক্ষ ft_frame_ushort_le:  /* read a 2-byte little-endian short */
        মান = FT_NEXT_USHORT_LE( cursor );
        sign_shift = 16;
        ভঙ্গ;

      পক্ষ ft_frame_long_be:
      পক্ষ ft_frame_ulong_be:  /* read a 4-byte big-endian long */
        মান = FT_NEXT_ULONG( cursor );
        sign_shift = 0;
        ভঙ্গ;

      পক্ষ ft_frame_long_le:
      পক্ষ ft_frame_ulong_le:  /* read a 4-byte little-endian long */
        মান = FT_NEXT_ULONG_LE( cursor );
        sign_shift = 0;
        ভঙ্গ;

      পক্ষ ft_frame_off3_be:
      পক্ষ ft_frame_uoff3_be:  /* read a 3-byte big-endian long */
        মান = FT_NEXT_UOFF3( cursor );
        sign_shift = 8;
        ভঙ্গ;

      পক্ষ ft_frame_off3_le:
      পক্ষ ft_frame_uoff3_le:  /* read a 3-byte little-endian long */
        মান = FT_NEXT_UOFF3_LE( cursor );
        sign_shift = 8;
        ভঙ্গ;

      স্বভাব:
        /* otherwise, exit the loop */
        stream->cursor = cursor;
        গচ্ছ Exit;
      }

      /* now, compute the signed value if necessary */
      যদি ( fields->মান & FT_FRAME_OP_SIGNED )
        মান = (FT_ULong)( (FT_Int32)( মান << sign_shift ) >> sign_shift );

      /* finally, store the value in the object */

      p = (FT_Byte*)structure + fields->offset;
      বিকল্প ( fields->size )
      {
      পক্ষ ( 8 / FT_CHAR_BIT ):
        *(FT_Byte*)p = (FT_Byte)মান;
        ভঙ্গ;

      পক্ষ ( 16 / FT_CHAR_BIT ):
        *(FT_UShort*)p = (FT_UShort)মান;
        ভঙ্গ;

      পক্ষ ( 32 / FT_CHAR_BIT ):
        *(FT_UInt32*)p = (FT_UInt32)মান;
        ভঙ্গ;

      স্বভাব:  /* for 64-bit systems */
        *(FT_ULong*)p = (FT_ULong)মান;
      }

      /* go to next field */
      fields++;
    }
    যাবত্ ( 1 );

  Exit:
    /* close the frame if it was opened by this read */
    যদি ( frame_accessed )
      FT_Stream_ExitFrame( stream );

    প্রত্যাগম error;
  }


/* END */
