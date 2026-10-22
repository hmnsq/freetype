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


#ଅନ୍ତର୍ଭାଵ <freetype/internal/ftstream.h>
#ଅନ୍ତର୍ଭାଵ <freetype/internal/ftdebug.h>


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#undef  FT_COMPONENT
#define FT_COMPONENT  stream


  FT_BASE_DEF( ଶୂନ୍ୟ )
  FT_Stream_OpenMemory( FT_Stream       stream,
                        ଅଚଳ FT_Byte*  base,
                        FT_ULong        size )
  {
    stream->base   = (FT_Byte*) base;
    stream->size   = size;
    stream->pos    = 0;
    stream->cursor = ଖାଲି;
    stream->ପଠ   = ଖାଲି;
    stream->close  = ଖାଲି;
  }


  FT_BASE_DEF( ଶୂନ୍ୟ )
  FT_Stream_Close( FT_Stream  stream )
  {
    ଯଦି ( stream && stream->close )
      stream->close( stream );
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_Seek( FT_Stream  stream,
                  FT_ULong   pos )
  {
    FT_Error  error = FT_Err_Ok;


    ଯଦି ( stream->ପଠ )
    {
      ଯଦି ( stream->ପଠ( stream, pos, ଖାଲି, 0 ) )
      {
        FT_ERROR(( "FT_Stream_Seek:"
                   " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
                   pos, stream->size ));

        error = FT_THROW( Invalid_Stream_Operation );
      }
    }
    /* note that seeking to the first position after the file is valid */
    ଅନ୍ୟଥା ଯଦି ( pos > stream->size )
    {
      FT_ERROR(( "FT_Stream_Seek:"
                 " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
                 pos, stream->size ));

      error = FT_THROW( Invalid_Stream_Operation );
    }

    ଯଦି ( !error )
      stream->pos = pos;

    ଫେରନ୍ତୁ error;
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_Skip( FT_Stream  stream,
                  FT_Long    distance )
  {
    ଯଦି ( distance < 0 )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Stream_Operation );

    ଫେରନ୍ତୁ FT_Stream_Seek( stream, stream->pos + (FT_ULong)distance );
  }


  FT_BASE_DEF( FT_ULong )
  FT_Stream_Pos( FT_Stream  stream )
  {
    ଫେରନ୍ତୁ stream->pos;
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_Read( FT_Stream  stream,
                  FT_Byte*   buffer,
                  FT_ULong   ଗଣନା_2 )
  {
    ଫେରନ୍ତୁ FT_Stream_ReadAt( stream, stream->pos, buffer, ଗଣନା_2 );
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_ReadAt( FT_Stream  stream,
                    FT_ULong   pos,
                    FT_Byte*   buffer,
                    FT_ULong   ଗଣନା_2 )
  {
    FT_Error  error = FT_Err_Ok;
    FT_ULong  read_bytes;


    ଯଦି ( pos >= stream->size )
    {
      FT_ERROR(( "FT_Stream_ReadAt:"
                 " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
                 pos, stream->size ));

      ଫେରନ୍ତୁ FT_THROW( Invalid_Stream_Operation );
    }

    ଯଦି ( stream->ପଠ )
      read_bytes = stream->ପଠ( stream, pos, buffer, ଗଣନା_2 );
    ଅନ୍ୟଥା
    {
      read_bytes = stream->size - pos;
      ଯଦି ( read_bytes > ଗଣନା_2 )
        read_bytes = ଗଣନା_2;

      /* Allow "reading" zero bytes without UB even if buffer is NULL */
      ଯଦି ( ଗଣନା_2 )
        FT_MEM_COPY( buffer, stream->base + pos, read_bytes );
    }

    stream->pos = pos + read_bytes;

    ଯଦି ( read_bytes < ଗଣନା_2 )
    {
      FT_ERROR(( "FT_Stream_ReadAt:"
                 " invalid read; expected %lu bytes, got %lu\n",
                 ଗଣନା_2, read_bytes ));

      error = FT_THROW( Invalid_Stream_Operation );
    }

    ଫେରନ୍ତୁ error;
  }


  FT_BASE_DEF( FT_ULong )
  FT_Stream_TryRead( FT_Stream  stream,
                     FT_Byte*   buffer,
                     FT_ULong   ଗଣନା_2 )
  {
    FT_ULong  read_bytes = 0;


    ଯଦି ( stream->pos >= stream->size )
      ଯାଅ Exit;

    ଯଦି ( stream->ପଠ )
      read_bytes = stream->ପଠ( stream, stream->pos, buffer, ଗଣନା_2 );
    ଅନ୍ୟଥା
    {
      read_bytes = stream->size - stream->pos;
      ଯଦି ( read_bytes > ଗଣନା_2 )
        read_bytes = ଗଣନା_2;

      /* Allow "reading" zero bytes without UB even if buffer is NULL */
      ଯଦି ( ଗଣନା_2 )
        FT_MEM_COPY( buffer, stream->base + stream->pos, read_bytes );
    }

    stream->pos += read_bytes;

  Exit:
    ଫେରନ୍ତୁ read_bytes;
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_ExtractFrame( FT_Stream  stream,
                          FT_ULong   ଗଣନା_2,
                          FT_Byte**  pbytes )
  {
    FT_Error  error;


    error = FT_Stream_EnterFrame( stream, ଗଣନା_2 );
    ଯଦି ( !error )
    {
      *pbytes = (FT_Byte*)stream->cursor;

      /* equivalent to FT_Stream_ExitFrame(), with no memory block release */
      stream->cursor = ଖାଲି;
      stream->limit  = ଖାଲି;
    }

    ଫେରନ୍ତୁ error;
  }


  FT_BASE_DEF( ଶୂନ୍ୟ )
  FT_Stream_ReleaseFrame( FT_Stream  stream,
                          FT_Byte**  pbytes )
  {
    ଯଦି ( stream && stream->ପଠ )
    {
      FT_Memory  memory = stream->memory;


#ifdef FT_DEBUG_MEMORY
      ft_mem_free( memory, *pbytes );
#ଅନ୍ୟଥା
      FT_FREE( *pbytes );
#endif
    }

    *pbytes = ଖାଲି;
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_EnterFrame( FT_Stream  stream,
                        FT_ULong   ଗଣନା_2 )
  {
    FT_Error  error = FT_Err_Ok;
    FT_ULong  read_bytes;


    FT_TRACE7(( "FT_Stream_EnterFrame: %lu bytes\n", ଗଣନା_2 ));

    /* check for nested frame access */
    FT_ASSERT( stream && stream->cursor == 0 );

    ଯଦି ( stream->ପଠ )
    {
      /* allocate the frame in memory */
      FT_Memory  memory = stream->memory;


      /* simple sanity check */
      ଯଦି ( ଗଣନା_2 > stream->size )
      {
        FT_ERROR(( "FT_Stream_EnterFrame:"
                   " frame size (%lu) larger than stream size (%lu)\n",
                   ଗଣନା_2, stream->size ));

        error = FT_THROW( Invalid_Stream_Operation );
        ଯାଅ Exit;
      }

#ifdef FT_DEBUG_MEMORY
      /* assume `ft_debug_file_` and `ft_debug_lineno_` are already set */
      stream->base = (unsigned ଅକ୍ଷର*)ft_mem_qalloc( memory,
                                                    (FT_Long)ଗଣନା_2,
                                                    &error );
      ଯଦି ( error )
        ଯାଅ Exit;
#ଅନ୍ୟଥା
      ଯଦି ( FT_QALLOC( stream->base, ଗଣନା_2 ) )
        ଯାଅ Exit;
#endif
      /* read it */
      read_bytes = stream->ପଠ( stream, stream->pos,
                                 stream->base, ଗଣନା_2 );
      ଯଦି ( read_bytes < ଗଣନା_2 )
      {
        FT_ERROR(( "FT_Stream_EnterFrame:"
                   " invalid read; expected %lu bytes, got %lu\n",
                   ଗଣନା_2, read_bytes ));

        FT_FREE( stream->base );
        error = FT_THROW( Invalid_Stream_Operation );
      }

      stream->cursor = stream->base;
      stream->limit  = FT_OFFSET( stream->cursor, ଗଣନା_2 );
      stream->pos   += read_bytes;
    }
    ଅନ୍ୟଥା
    {
      /* check current and new position */
      ଯଦି ( stream->pos >= stream->size        ||
           stream->size - stream->pos < ଗଣନା_2 )
      {
        FT_ERROR(( "FT_Stream_EnterFrame:"
                   " invalid i/o; pos = 0x%lx, count = %lu, size = 0x%lx\n",
                   stream->pos, ଗଣନା_2, stream->size ));

        error = FT_THROW( Invalid_Stream_Operation );
        ଯାଅ Exit;
      }

      /* set cursor */
      stream->cursor = stream->base + stream->pos;
      stream->limit  = stream->cursor + ଗଣନା_2;
      stream->pos   += ଗଣନା_2;
    }

  Exit:
    ଫେରନ୍ତୁ error;
  }


  FT_BASE_DEF( ଶୂନ୍ୟ )
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

    ଯଦି ( stream->ପଠ )
    {
      FT_Memory  memory = stream->memory;


#ifdef FT_DEBUG_MEMORY
      ft_mem_free( memory, stream->base );
      stream->base = ଖାଲି;
#ଅନ୍ୟଥା
      FT_FREE( stream->base );
#endif
    }

    stream->cursor = ଖାଲି;
    stream->limit  = ଖାଲି;
  }


  FT_BASE_DEF( FT_Byte )
  FT_Stream_GetByte( FT_Stream  stream )
  {
    FT_Byte  result;


    FT_ASSERT( stream && stream->cursor );

    result = 0;
    ଯଦି ( stream->cursor < stream->limit )
      result = *stream->cursor++;

    ଫେରନ୍ତୁ result;
  }


  FT_BASE_DEF( FT_UInt16 )
  FT_Stream_GetUShort( FT_Stream  stream )
  {
    FT_Byte*   p;
    FT_UInt16  result;


    FT_ASSERT( stream && stream->cursor );

    result         = 0;
    p              = stream->cursor;
    ଯଦି ( p + 1 < stream->limit )
      result       = FT_NEXT_USHORT( p );
    stream->cursor = p;

    ଫେରନ୍ତୁ result;
  }


  FT_BASE_DEF( FT_UInt16 )
  FT_Stream_GetUShortLE( FT_Stream  stream )
  {
    FT_Byte*   p;
    FT_UInt16  result;


    FT_ASSERT( stream && stream->cursor );

    result         = 0;
    p              = stream->cursor;
    ଯଦି ( p + 1 < stream->limit )
      result       = FT_NEXT_USHORT_LE( p );
    stream->cursor = p;

    ଫେରନ୍ତୁ result;
  }


  FT_BASE_DEF( FT_UInt32 )
  FT_Stream_GetUOffset( FT_Stream  stream )
  {
    FT_Byte*  p;
    FT_UInt32 result;


    FT_ASSERT( stream && stream->cursor );

    result         = 0;
    p              = stream->cursor;
    ଯଦି ( p + 2 < stream->limit )
      result       = FT_NEXT_UOFF3( p );
    stream->cursor = p;
    ଫେରନ୍ତୁ result;
  }


  FT_BASE_DEF( FT_UInt32 )
  FT_Stream_GetULong( FT_Stream  stream )
  {
    FT_Byte*  p;
    FT_UInt32 result;


    FT_ASSERT( stream && stream->cursor );

    result         = 0;
    p              = stream->cursor;
    ଯଦି ( p + 3 < stream->limit )
      result       = FT_NEXT_ULONG( p );
    stream->cursor = p;
    ଫେରନ୍ତୁ result;
  }


  FT_BASE_DEF( FT_UInt32 )
  FT_Stream_GetULongLE( FT_Stream  stream )
  {
    FT_Byte*  p;
    FT_UInt32 result;


    FT_ASSERT( stream && stream->cursor );

    result         = 0;
    p              = stream->cursor;
    ଯଦି ( p + 3 < stream->limit )
      result       = FT_NEXT_ULONG_LE( p );
    stream->cursor = p;
    ଫେରନ୍ତୁ result;
  }


  FT_BASE_DEF( FT_Byte )
  FT_Stream_ReadByte( FT_Stream  stream,
                      FT_Error*  error )
  {
    FT_Byte  result = 0;


    FT_ASSERT( stream );

    ଯଦି ( stream->pos < stream->size )
    {
      ଯଦି ( stream->ପଠ )
      {
        ଯଦି ( stream->ପଠ( stream, stream->pos, &result, 1L ) != 1L )
          ଯାଅ Fail;
      }
      ଅନ୍ୟଥା
        result = stream->base[stream->pos];
    }
    ଅନ୍ୟଥା
      ଯାଅ Fail;

    stream->pos++;

    *error = FT_Err_Ok;

    ଫେରନ୍ତୁ result;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadByte:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    ଫେରନ୍ତୁ result;
  }


  FT_BASE_DEF( FT_UInt16 )
  FT_Stream_ReadUShort( FT_Stream  stream,
                        FT_Error*  error )
  {
    FT_Byte    reads[2];
    FT_Byte*   p;
    FT_UInt16  result = 0;


    FT_ASSERT( stream );

    ଯଦି ( stream->pos + 1 < stream->size )
    {
      ଯଦି ( stream->ପଠ )
      {
        ଯଦି ( stream->ପଠ( stream, stream->pos, reads, 2L ) != 2L )
          ଯାଅ Fail;

        p = reads;
      }
      ଅନ୍ୟଥା
        p = stream->base + stream->pos;

      ଯଦି ( p )
        result = FT_NEXT_USHORT( p );
    }
    ଅନ୍ୟଥା
      ଯାଅ Fail;

    stream->pos += 2;

    *error = FT_Err_Ok;

    ଫେରନ୍ତୁ result;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadUShort:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    ଫେରନ୍ତୁ result;
  }


  FT_BASE_DEF( FT_UInt16 )
  FT_Stream_ReadUShortLE( FT_Stream  stream,
                          FT_Error*  error )
  {
    FT_Byte    reads[2];
    FT_Byte*   p;
    FT_UInt16  result = 0;


    FT_ASSERT( stream );

    ଯଦି ( stream->pos + 1 < stream->size )
    {
      ଯଦି ( stream->ପଠ )
      {
        ଯଦି ( stream->ପଠ( stream, stream->pos, reads, 2L ) != 2L )
          ଯାଅ Fail;

        p = reads;
      }
      ଅନ୍ୟଥା
        p = stream->base + stream->pos;

      ଯଦି ( p )
        result = FT_NEXT_USHORT_LE( p );
    }
    ଅନ୍ୟଥା
      ଯାଅ Fail;

    stream->pos += 2;

    *error = FT_Err_Ok;

    ଫେରନ୍ତୁ result;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadUShortLE:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    ଫେରନ୍ତୁ result;
  }


  FT_BASE_DEF( FT_ULong )
  FT_Stream_ReadUOffset( FT_Stream  stream,
                         FT_Error*  error )
  {
    FT_Byte   reads[3];
    FT_Byte*  p;
    FT_ULong  result = 0;


    FT_ASSERT( stream );

    ଯଦି ( stream->pos + 2 < stream->size )
    {
      ଯଦି ( stream->ପଠ )
      {
        ଯଦି (stream->ପଠ( stream, stream->pos, reads, 3L ) != 3L )
          ଯାଅ Fail;

        p = reads;
      }
      ଅନ୍ୟଥା
        p = stream->base + stream->pos;

      ଯଦି ( p )
        result = FT_NEXT_UOFF3( p );
    }
    ଅନ୍ୟଥା
      ଯାଅ Fail;

    stream->pos += 3;

    *error = FT_Err_Ok;

    ଫେରନ୍ତୁ result;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadUOffset:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    ଫେରନ୍ତୁ result;
  }


  FT_BASE_DEF( FT_UInt32 )
  FT_Stream_ReadULong( FT_Stream  stream,
                       FT_Error*  error )
  {
    FT_Byte   reads[4];
    FT_Byte*  p;
    FT_UInt32 result = 0;


    FT_ASSERT( stream );

    ଯଦି ( stream->pos + 3 < stream->size )
    {
      ଯଦି ( stream->ପଠ )
      {
        ଯଦି ( stream->ପଠ( stream, stream->pos, reads, 4L ) != 4L )
          ଯାଅ Fail;

        p = reads;
      }
      ଅନ୍ୟଥା
        p = stream->base + stream->pos;

      ଯଦି ( p )
        result = FT_NEXT_ULONG( p );
    }
    ଅନ୍ୟଥା
      ଯାଅ Fail;

    stream->pos += 4;

    *error = FT_Err_Ok;

    ଫେରନ୍ତୁ result;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadULong:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    ଫେରନ୍ତୁ result;
  }


  FT_BASE_DEF( FT_UInt32 )
  FT_Stream_ReadULongLE( FT_Stream  stream,
                         FT_Error*  error )
  {
    FT_Byte   reads[4];
    FT_Byte*  p;
    FT_UInt32 result = 0;


    FT_ASSERT( stream );

    ଯଦି ( stream->pos + 3 < stream->size )
    {
      ଯଦି ( stream->ପଠ )
      {
        ଯଦି ( stream->ପଠ( stream, stream->pos, reads, 4L ) != 4L )
          ଯାଅ Fail;

        p = reads;
      }
      ଅନ୍ୟଥା
        p = stream->base + stream->pos;

      ଯଦି ( p )
        result = FT_NEXT_ULONG_LE( p );
    }
    ଅନ୍ୟଥା
      ଯାଅ Fail;

    stream->pos += 4;

    *error = FT_Err_Ok;

    ଫେରନ୍ତୁ result;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadULongLE:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    ଫେରନ୍ତୁ result;
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_ReadFields( FT_Stream              stream,
                        ଅଚଳ FT_Frame_Field*  fields,
                        ଶୂନ୍ୟ*                  structure )
  {
    FT_Error  error;
    FT_Bool   frame_accessed = 0;
    FT_Byte*  cursor;


    ଯଦି ( !fields )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Argument );

    ଯଦି ( !stream )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Stream_Handle );

    cursor = stream->cursor;

    error = FT_Err_Ok;
    କର
    {
      FT_ULong  ମୂଲ୍ୟ;
      FT_Int    sign_shift;
      FT_Byte*  p;


      ବାଛ ( fields->ମୂଲ୍ୟ )
      {
      ସ୍ଥିତି ft_frame_start:  /* access a new frame */
        error = FT_Stream_EnterFrame( stream, fields->offset );
        ଯଦି ( error )
          ଯାଅ Exit;

        frame_accessed = 1;
        cursor         = stream->cursor;
        fields++;
        ଜାରି;  /* loop! */

      ସ୍ଥିତି ft_frame_bytes:  /* read a byte sequence */
      ସ୍ଥିତି ft_frame_skip:   /* skip some bytes      */
        {
          FT_Offset  len = fields->size;


          ଯଦି ( len > (FT_Offset)( stream->limit - cursor ) )
          {
            error = FT_THROW( Invalid_Stream_Operation );
            ଯାଅ Exit;
          }

          ଯଦି ( fields->ମୂଲ୍ୟ == ft_frame_bytes )
          {
            p = (FT_Byte*)structure + fields->offset;
            FT_MEM_COPY( p, cursor, len );
          }
          cursor += len;
          fields++;
          ଜାରି;
        }

      ସ୍ଥିତି ft_frame_byte:
      ସ୍ଥିତି ft_frame_schar:  /* read a single byte */
        ମୂଲ୍ୟ = FT_NEXT_BYTE( cursor );
        sign_shift = 24;
        ଭାଙ୍ଗ;

      ସ୍ଥିତି ft_frame_short_be:
      ସ୍ଥିତି ft_frame_ushort_be:  /* read a 2-byte big-endian short */
        ମୂଲ୍ୟ = FT_NEXT_USHORT( cursor );
        sign_shift = 16;
        ଭାଙ୍ଗ;

      ସ୍ଥିତି ft_frame_short_le:
      ସ୍ଥିତି ft_frame_ushort_le:  /* read a 2-byte little-endian short */
        ମୂଲ୍ୟ = FT_NEXT_USHORT_LE( cursor );
        sign_shift = 16;
        ଭାଙ୍ଗ;

      ସ୍ଥିତି ft_frame_long_be:
      ସ୍ଥିତି ft_frame_ulong_be:  /* read a 4-byte big-endian long */
        ମୂଲ୍ୟ = FT_NEXT_ULONG( cursor );
        sign_shift = 0;
        ଭାଙ୍ଗ;

      ସ୍ଥିତି ft_frame_long_le:
      ସ୍ଥିତି ft_frame_ulong_le:  /* read a 4-byte little-endian long */
        ମୂଲ୍ୟ = FT_NEXT_ULONG_LE( cursor );
        sign_shift = 0;
        ଭାଙ୍ଗ;

      ସ୍ଥିତି ft_frame_off3_be:
      ସ୍ଥିତି ft_frame_uoff3_be:  /* read a 3-byte big-endian long */
        ମୂଲ୍ୟ = FT_NEXT_UOFF3( cursor );
        sign_shift = 8;
        ଭାଙ୍ଗ;

      ସ୍ଥିତି ft_frame_off3_le:
      ସ୍ଥିତି ft_frame_uoff3_le:  /* read a 3-byte little-endian long */
        ମୂଲ୍ୟ = FT_NEXT_UOFF3_LE( cursor );
        sign_shift = 8;
        ଭାଙ୍ଗ;

      ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        /* otherwise, exit the loop */
        stream->cursor = cursor;
        ଯାଅ Exit;
      }

      /* now, compute the signed value if necessary */
      ଯଦି ( fields->ମୂଲ୍ୟ & FT_FRAME_OP_SIGNED )
        ମୂଲ୍ୟ = (FT_ULong)( (FT_Int32)( ମୂଲ୍ୟ << sign_shift ) >> sign_shift );

      /* finally, store the value in the object */

      p = (FT_Byte*)structure + fields->offset;
      ବାଛ ( fields->size )
      {
      ସ୍ଥିତି ( 8 / FT_CHAR_BIT ):
        *(FT_Byte*)p = (FT_Byte)ମୂଲ୍ୟ;
        ଭାଙ୍ଗ;

      ସ୍ଥିତି ( 16 / FT_CHAR_BIT ):
        *(FT_UShort*)p = (FT_UShort)ମୂଲ୍ୟ;
        ଭାଙ୍ଗ;

      ସ୍ଥିତି ( 32 / FT_CHAR_BIT ):
        *(FT_UInt32*)p = (FT_UInt32)ମୂଲ୍ୟ;
        ଭାଙ୍ଗ;

      ପୂର୍ବନିର୍ଦ୍ଧାରିତ:  /* for 64-bit systems */
        *(FT_ULong*)p = (FT_ULong)ମୂଲ୍ୟ;
      }

      /* go to next field */
      fields++;
    }
    ଯେତେବେଳେ ( 1 );

  Exit:
    /* close the frame if it was opened by this read */
    ଯଦି ( frame_accessed )
      FT_Stream_ExitFrame( stream );

    ଫେରନ୍ତୁ error;
  }


/* END */
