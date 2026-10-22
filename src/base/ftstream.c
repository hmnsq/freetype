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


#અન્તર્ભાવ <freetype/internal/ftstream.h>
#અન્તર્ભાવ <freetype/internal/ftdebug.h>


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#undef  FT_COMPONENT
#define FT_COMPONENT  stream


  FT_BASE_DEF( શૂન્ય )
  FT_Stream_OpenMemory( FT_Stream       stream,
                        અચલ FT_Byte*  base,
                        FT_ULong        size )
  {
    stream->base   = (FT_Byte*) base;
    stream->size   = size;
    stream->pos    = 0;
    stream->cursor = ખાલી;
    stream->પઠ   = ખાલી;
    stream->close  = ખાલી;
  }


  FT_BASE_DEF( શૂન્ય )
  FT_Stream_Close( FT_Stream  stream )
  {
    જો ( stream && stream->close )
      stream->close( stream );
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_Seek( FT_Stream  stream,
                  FT_ULong   pos )
  {
    FT_Error  error = FT_Err_Ok;


    જો ( stream->પઠ )
    {
      જો ( stream->પઠ( stream, pos, ખાલી, 0 ) )
      {
        FT_ERROR(( "FT_Stream_Seek:"
                   " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
                   pos, stream->size ));

        error = FT_THROW( Invalid_Stream_Operation );
      }
    }
    /* note that seeking to the first position after the file is valid */
    નહીંતર જો ( pos > stream->size )
    {
      FT_ERROR(( "FT_Stream_Seek:"
                 " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
                 pos, stream->size ));

      error = FT_THROW( Invalid_Stream_Operation );
    }

    જો ( !error )
      stream->pos = pos;

    પાછા error;
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_Skip( FT_Stream  stream,
                  FT_Long    distance )
  {
    જો ( distance < 0 )
      પાછા FT_THROW( Invalid_Stream_Operation );

    પાછા FT_Stream_Seek( stream, stream->pos + (FT_ULong)distance );
  }


  FT_BASE_DEF( FT_ULong )
  FT_Stream_Pos( FT_Stream  stream )
  {
    પાછા stream->pos;
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_Read( FT_Stream  stream,
                  FT_Byte*   buffer,
                  FT_ULong   ગણતરી )
  {
    પાછા FT_Stream_ReadAt( stream, stream->pos, buffer, ગણતરી );
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_ReadAt( FT_Stream  stream,
                    FT_ULong   pos,
                    FT_Byte*   buffer,
                    FT_ULong   ગણતરી )
  {
    FT_Error  error = FT_Err_Ok;
    FT_ULong  read_bytes;


    જો ( pos >= stream->size )
    {
      FT_ERROR(( "FT_Stream_ReadAt:"
                 " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
                 pos, stream->size ));

      પાછા FT_THROW( Invalid_Stream_Operation );
    }

    જો ( stream->પઠ )
      read_bytes = stream->પઠ( stream, pos, buffer, ગણતરી );
    નહીંતર
    {
      read_bytes = stream->size - pos;
      જો ( read_bytes > ગણતરી )
        read_bytes = ગણતરી;

      /* Allow "reading" zero bytes without UB even if buffer is NULL */
      જો ( ગણતરી )
        FT_MEM_COPY( buffer, stream->base + pos, read_bytes );
    }

    stream->pos = pos + read_bytes;

    જો ( read_bytes < ગણતરી )
    {
      FT_ERROR(( "FT_Stream_ReadAt:"
                 " invalid read; expected %lu bytes, got %lu\n",
                 ગણતરી, read_bytes ));

      error = FT_THROW( Invalid_Stream_Operation );
    }

    પાછા error;
  }


  FT_BASE_DEF( FT_ULong )
  FT_Stream_TryRead( FT_Stream  stream,
                     FT_Byte*   buffer,
                     FT_ULong   ગણતરી )
  {
    FT_ULong  read_bytes = 0;


    જો ( stream->pos >= stream->size )
      જાઓ Exit;

    જો ( stream->પઠ )
      read_bytes = stream->પઠ( stream, stream->pos, buffer, ગણતરી );
    નહીંતર
    {
      read_bytes = stream->size - stream->pos;
      જો ( read_bytes > ગણતરી )
        read_bytes = ગણતરી;

      /* Allow "reading" zero bytes without UB even if buffer is NULL */
      જો ( ગણતરી )
        FT_MEM_COPY( buffer, stream->base + stream->pos, read_bytes );
    }

    stream->pos += read_bytes;

  Exit:
    પાછા read_bytes;
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_ExtractFrame( FT_Stream  stream,
                          FT_ULong   ગણતરી,
                          FT_Byte**  pbytes )
  {
    FT_Error  error;


    error = FT_Stream_EnterFrame( stream, ગણતરી );
    જો ( !error )
    {
      *pbytes = (FT_Byte*)stream->cursor;

      /* equivalent to FT_Stream_ExitFrame(), with no memory block release */
      stream->cursor = ખાલી;
      stream->limit  = ખાલી;
    }

    પાછા error;
  }


  FT_BASE_DEF( શૂન્ય )
  FT_Stream_ReleaseFrame( FT_Stream  stream,
                          FT_Byte**  pbytes )
  {
    જો ( stream && stream->પઠ )
    {
      FT_Memory  memory = stream->memory;


#ifdef FT_DEBUG_MEMORY
      ft_mem_free( memory, *pbytes );
#નહીંતર
      FT_FREE( *pbytes );
#endif
    }

    *pbytes = ખાલી;
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_EnterFrame( FT_Stream  stream,
                        FT_ULong   ગણતરી )
  {
    FT_Error  error = FT_Err_Ok;
    FT_ULong  read_bytes;


    FT_TRACE7(( "FT_Stream_EnterFrame: %lu bytes\n", ગણતરી ));

    /* check for nested frame access */
    FT_ASSERT( stream && stream->cursor == 0 );

    જો ( stream->પઠ )
    {
      /* allocate the frame in memory */
      FT_Memory  memory = stream->memory;


      /* simple sanity check */
      જો ( ગણતરી > stream->size )
      {
        FT_ERROR(( "FT_Stream_EnterFrame:"
                   " frame size (%lu) larger than stream size (%lu)\n",
                   ગણતરી, stream->size ));

        error = FT_THROW( Invalid_Stream_Operation );
        જાઓ Exit;
      }

#ifdef FT_DEBUG_MEMORY
      /* assume `ft_debug_file_` and `ft_debug_lineno_` are already set */
      stream->base = (unsigned અક્ષર*)ft_mem_qalloc( memory,
                                                    (FT_Long)ગણતરી,
                                                    &error );
      જો ( error )
        જાઓ Exit;
#નહીંતર
      જો ( FT_QALLOC( stream->base, ગણતરી ) )
        જાઓ Exit;
#endif
      /* read it */
      read_bytes = stream->પઠ( stream, stream->pos,
                                 stream->base, ગણતરી );
      જો ( read_bytes < ગણતરી )
      {
        FT_ERROR(( "FT_Stream_EnterFrame:"
                   " invalid read; expected %lu bytes, got %lu\n",
                   ગણતરી, read_bytes ));

        FT_FREE( stream->base );
        error = FT_THROW( Invalid_Stream_Operation );
      }

      stream->cursor = stream->base;
      stream->limit  = FT_OFFSET( stream->cursor, ગણતરી );
      stream->pos   += read_bytes;
    }
    નહીંતર
    {
      /* check current and new position */
      જો ( stream->pos >= stream->size        ||
           stream->size - stream->pos < ગણતરી )
      {
        FT_ERROR(( "FT_Stream_EnterFrame:"
                   " invalid i/o; pos = 0x%lx, count = %lu, size = 0x%lx\n",
                   stream->pos, ગણતરી, stream->size ));

        error = FT_THROW( Invalid_Stream_Operation );
        જાઓ Exit;
      }

      /* set cursor */
      stream->cursor = stream->base + stream->pos;
      stream->limit  = stream->cursor + ગણતરી;
      stream->pos   += ગણતરી;
    }

  Exit:
    પાછા error;
  }


  FT_BASE_DEF( શૂન્ય )
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

    જો ( stream->પઠ )
    {
      FT_Memory  memory = stream->memory;


#ifdef FT_DEBUG_MEMORY
      ft_mem_free( memory, stream->base );
      stream->base = ખાલી;
#નહીંતર
      FT_FREE( stream->base );
#endif
    }

    stream->cursor = ખાલી;
    stream->limit  = ખાલી;
  }


  FT_BASE_DEF( FT_Byte )
  FT_Stream_GetByte( FT_Stream  stream )
  {
    FT_Byte  result;


    FT_ASSERT( stream && stream->cursor );

    result = 0;
    જો ( stream->cursor < stream->limit )
      result = *stream->cursor++;

    પાછા result;
  }


  FT_BASE_DEF( FT_UInt16 )
  FT_Stream_GetUShort( FT_Stream  stream )
  {
    FT_Byte*   p;
    FT_UInt16  result;


    FT_ASSERT( stream && stream->cursor );

    result         = 0;
    p              = stream->cursor;
    જો ( p + 1 < stream->limit )
      result       = FT_NEXT_USHORT( p );
    stream->cursor = p;

    પાછા result;
  }


  FT_BASE_DEF( FT_UInt16 )
  FT_Stream_GetUShortLE( FT_Stream  stream )
  {
    FT_Byte*   p;
    FT_UInt16  result;


    FT_ASSERT( stream && stream->cursor );

    result         = 0;
    p              = stream->cursor;
    જો ( p + 1 < stream->limit )
      result       = FT_NEXT_USHORT_LE( p );
    stream->cursor = p;

    પાછા result;
  }


  FT_BASE_DEF( FT_UInt32 )
  FT_Stream_GetUOffset( FT_Stream  stream )
  {
    FT_Byte*  p;
    FT_UInt32 result;


    FT_ASSERT( stream && stream->cursor );

    result         = 0;
    p              = stream->cursor;
    જો ( p + 2 < stream->limit )
      result       = FT_NEXT_UOFF3( p );
    stream->cursor = p;
    પાછા result;
  }


  FT_BASE_DEF( FT_UInt32 )
  FT_Stream_GetULong( FT_Stream  stream )
  {
    FT_Byte*  p;
    FT_UInt32 result;


    FT_ASSERT( stream && stream->cursor );

    result         = 0;
    p              = stream->cursor;
    જો ( p + 3 < stream->limit )
      result       = FT_NEXT_ULONG( p );
    stream->cursor = p;
    પાછા result;
  }


  FT_BASE_DEF( FT_UInt32 )
  FT_Stream_GetULongLE( FT_Stream  stream )
  {
    FT_Byte*  p;
    FT_UInt32 result;


    FT_ASSERT( stream && stream->cursor );

    result         = 0;
    p              = stream->cursor;
    જો ( p + 3 < stream->limit )
      result       = FT_NEXT_ULONG_LE( p );
    stream->cursor = p;
    પાછા result;
  }


  FT_BASE_DEF( FT_Byte )
  FT_Stream_ReadByte( FT_Stream  stream,
                      FT_Error*  error )
  {
    FT_Byte  result = 0;


    FT_ASSERT( stream );

    જો ( stream->pos < stream->size )
    {
      જો ( stream->પઠ )
      {
        જો ( stream->પઠ( stream, stream->pos, &result, 1L ) != 1L )
          જાઓ Fail;
      }
      નહીંતર
        result = stream->base[stream->pos];
    }
    નહીંતર
      જાઓ Fail;

    stream->pos++;

    *error = FT_Err_Ok;

    પાછા result;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadByte:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    પાછા result;
  }


  FT_BASE_DEF( FT_UInt16 )
  FT_Stream_ReadUShort( FT_Stream  stream,
                        FT_Error*  error )
  {
    FT_Byte    reads[2];
    FT_Byte*   p;
    FT_UInt16  result = 0;


    FT_ASSERT( stream );

    જો ( stream->pos + 1 < stream->size )
    {
      જો ( stream->પઠ )
      {
        જો ( stream->પઠ( stream, stream->pos, reads, 2L ) != 2L )
          જાઓ Fail;

        p = reads;
      }
      નહીંતર
        p = stream->base + stream->pos;

      જો ( p )
        result = FT_NEXT_USHORT( p );
    }
    નહીંતર
      જાઓ Fail;

    stream->pos += 2;

    *error = FT_Err_Ok;

    પાછા result;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadUShort:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    પાછા result;
  }


  FT_BASE_DEF( FT_UInt16 )
  FT_Stream_ReadUShortLE( FT_Stream  stream,
                          FT_Error*  error )
  {
    FT_Byte    reads[2];
    FT_Byte*   p;
    FT_UInt16  result = 0;


    FT_ASSERT( stream );

    જો ( stream->pos + 1 < stream->size )
    {
      જો ( stream->પઠ )
      {
        જો ( stream->પઠ( stream, stream->pos, reads, 2L ) != 2L )
          જાઓ Fail;

        p = reads;
      }
      નહીંતર
        p = stream->base + stream->pos;

      જો ( p )
        result = FT_NEXT_USHORT_LE( p );
    }
    નહીંતર
      જાઓ Fail;

    stream->pos += 2;

    *error = FT_Err_Ok;

    પાછા result;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadUShortLE:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    પાછા result;
  }


  FT_BASE_DEF( FT_ULong )
  FT_Stream_ReadUOffset( FT_Stream  stream,
                         FT_Error*  error )
  {
    FT_Byte   reads[3];
    FT_Byte*  p;
    FT_ULong  result = 0;


    FT_ASSERT( stream );

    જો ( stream->pos + 2 < stream->size )
    {
      જો ( stream->પઠ )
      {
        જો (stream->પઠ( stream, stream->pos, reads, 3L ) != 3L )
          જાઓ Fail;

        p = reads;
      }
      નહીંતર
        p = stream->base + stream->pos;

      જો ( p )
        result = FT_NEXT_UOFF3( p );
    }
    નહીંતર
      જાઓ Fail;

    stream->pos += 3;

    *error = FT_Err_Ok;

    પાછા result;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadUOffset:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    પાછા result;
  }


  FT_BASE_DEF( FT_UInt32 )
  FT_Stream_ReadULong( FT_Stream  stream,
                       FT_Error*  error )
  {
    FT_Byte   reads[4];
    FT_Byte*  p;
    FT_UInt32 result = 0;


    FT_ASSERT( stream );

    જો ( stream->pos + 3 < stream->size )
    {
      જો ( stream->પઠ )
      {
        જો ( stream->પઠ( stream, stream->pos, reads, 4L ) != 4L )
          જાઓ Fail;

        p = reads;
      }
      નહીંતર
        p = stream->base + stream->pos;

      જો ( p )
        result = FT_NEXT_ULONG( p );
    }
    નહીંતર
      જાઓ Fail;

    stream->pos += 4;

    *error = FT_Err_Ok;

    પાછા result;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadULong:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    પાછા result;
  }


  FT_BASE_DEF( FT_UInt32 )
  FT_Stream_ReadULongLE( FT_Stream  stream,
                         FT_Error*  error )
  {
    FT_Byte   reads[4];
    FT_Byte*  p;
    FT_UInt32 result = 0;


    FT_ASSERT( stream );

    જો ( stream->pos + 3 < stream->size )
    {
      જો ( stream->પઠ )
      {
        જો ( stream->પઠ( stream, stream->pos, reads, 4L ) != 4L )
          જાઓ Fail;

        p = reads;
      }
      નહીંતર
        p = stream->base + stream->pos;

      જો ( p )
        result = FT_NEXT_ULONG_LE( p );
    }
    નહીંતર
      જાઓ Fail;

    stream->pos += 4;

    *error = FT_Err_Ok;

    પાછા result;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadULongLE:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    પાછા result;
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_ReadFields( FT_Stream              stream,
                        અચલ FT_Frame_Field*  fields,
                        શૂન્ય*                  structure )
  {
    FT_Error  error;
    FT_Bool   frame_accessed = 0;
    FT_Byte*  cursor;


    જો ( !fields )
      પાછા FT_THROW( Invalid_Argument );

    જો ( !stream )
      પાછા FT_THROW( Invalid_Stream_Handle );

    cursor = stream->cursor;

    error = FT_Err_Ok;
    કરો
    {
      FT_ULong  કિંમત;
      FT_Int    sign_shift;
      FT_Byte*  p;


      પસંદગી ( fields->કિંમત )
      {
      સ્થિતિ ft_frame_start:  /* access a new frame */
        error = FT_Stream_EnterFrame( stream, fields->offset );
        જો ( error )
          જાઓ Exit;

        frame_accessed = 1;
        cursor         = stream->cursor;
        fields++;
        ચાલુ;  /* loop! */

      સ્થિતિ ft_frame_bytes:  /* read a byte sequence */
      સ્થિતિ ft_frame_skip:   /* skip some bytes      */
        {
          FT_Offset  len = fields->size;


          જો ( len > (FT_Offset)( stream->limit - cursor ) )
          {
            error = FT_THROW( Invalid_Stream_Operation );
            જાઓ Exit;
          }

          જો ( fields->કિંમત == ft_frame_bytes )
          {
            p = (FT_Byte*)structure + fields->offset;
            FT_MEM_COPY( p, cursor, len );
          }
          cursor += len;
          fields++;
          ચાલુ;
        }

      સ્થિતિ ft_frame_byte:
      સ્થિતિ ft_frame_schar:  /* read a single byte */
        કિંમત = FT_NEXT_BYTE( cursor );
        sign_shift = 24;
        તોડો;

      સ્થિતિ ft_frame_short_be:
      સ્થિતિ ft_frame_ushort_be:  /* read a 2-byte big-endian short */
        કિંમત = FT_NEXT_USHORT( cursor );
        sign_shift = 16;
        તોડો;

      સ્થિતિ ft_frame_short_le:
      સ્થિતિ ft_frame_ushort_le:  /* read a 2-byte little-endian short */
        કિંમત = FT_NEXT_USHORT_LE( cursor );
        sign_shift = 16;
        તોડો;

      સ્થિતિ ft_frame_long_be:
      સ્થિતિ ft_frame_ulong_be:  /* read a 4-byte big-endian long */
        કિંમત = FT_NEXT_ULONG( cursor );
        sign_shift = 0;
        તોડો;

      સ્થિતિ ft_frame_long_le:
      સ્થિતિ ft_frame_ulong_le:  /* read a 4-byte little-endian long */
        કિંમત = FT_NEXT_ULONG_LE( cursor );
        sign_shift = 0;
        તોડો;

      સ્થિતિ ft_frame_off3_be:
      સ્થિતિ ft_frame_uoff3_be:  /* read a 3-byte big-endian long */
        કિંમત = FT_NEXT_UOFF3( cursor );
        sign_shift = 8;
        તોડો;

      સ્થિતિ ft_frame_off3_le:
      સ્થિતિ ft_frame_uoff3_le:  /* read a 3-byte little-endian long */
        કિંમત = FT_NEXT_UOFF3_LE( cursor );
        sign_shift = 8;
        તોડો;

      મૂળભૂત:
        /* otherwise, exit the loop */
        stream->cursor = cursor;
        જાઓ Exit;
      }

      /* now, compute the signed value if necessary */
      જો ( fields->કિંમત & FT_FRAME_OP_SIGNED )
        કિંમત = (FT_ULong)( (FT_Int32)( કિંમત << sign_shift ) >> sign_shift );

      /* finally, store the value in the object */

      p = (FT_Byte*)structure + fields->offset;
      પસંદગી ( fields->size )
      {
      સ્થિતિ ( 8 / FT_CHAR_BIT ):
        *(FT_Byte*)p = (FT_Byte)કિંમત;
        તોડો;

      સ્થિતિ ( 16 / FT_CHAR_BIT ):
        *(FT_UShort*)p = (FT_UShort)કિંમત;
        તોડો;

      સ્થિતિ ( 32 / FT_CHAR_BIT ):
        *(FT_UInt32*)p = (FT_UInt32)કિંમત;
        તોડો;

      મૂળભૂત:  /* for 64-bit systems */
        *(FT_ULong*)p = (FT_ULong)કિંમત;
      }

      /* go to next field */
      fields++;
    }
    જ્યારે ( 1 );

  Exit:
    /* close the frame if it was opened by this read */
    જો ( frame_accessed )
      FT_Stream_ExitFrame( stream );

    પાછા error;
  }


/* END */
