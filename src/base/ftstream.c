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


#अन्तर्भाव <freetype/internal/ftstream.h>
#अन्तर्भाव <freetype/internal/ftdebug.h>


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#undef  FT_COMPONENT
#define FT_COMPONENT  stream


  FT_BASE_DEF( शून्य )
  FT_Stream_OpenMemory( FT_Stream       stream,
                        ध्रुव FT_Byte*  base,
                        FT_ULong        size )
  {
    stream->base   = (FT_Byte*) base;
    stream->size   = size;
    stream->pos    = 0;
    stream->cursor = रिक्त;
    stream->पठ   = रिक्त;
    stream->close  = रिक्त;
  }


  FT_BASE_DEF( शून्य )
  FT_Stream_Close( FT_Stream  stream )
  {
    सचे ( stream && stream->close )
      stream->close( stream );
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_Seek( FT_Stream  stream,
                  FT_ULong   pos )
  {
    FT_Error  error = FT_Err_Ok;


    सचे ( stream->पठ )
    {
      सचे ( stream->पठ( stream, pos, रिक्त, 0 ) )
      {
        FT_ERROR(( "FT_Stream_Seek:"
                   " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
                   pos, stream->size ));

        error = FT_THROW( Invalid_Stream_Operation );
      }
    }
    /* note that seeking to the first position after the file is valid */
    अञ्ञथा सचे ( pos > stream->size )
    {
      FT_ERROR(( "FT_Stream_Seek:"
                 " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
                 pos, stream->size ));

      error = FT_THROW( Invalid_Stream_Operation );
    }

    सचे ( !error )
      stream->pos = pos;

    पटिनिवत्त error;
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_Skip( FT_Stream  stream,
                  FT_Long    distance )
  {
    सचे ( distance < 0 )
      पटिनिवत्त FT_THROW( Invalid_Stream_Operation );

    पटिनिवत्त FT_Stream_Seek( stream, stream->pos + (FT_ULong)distance );
  }


  FT_BASE_DEF( FT_ULong )
  FT_Stream_Pos( FT_Stream  stream )
  {
    पटिनिवत्त stream->pos;
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_Read( FT_Stream  stream,
                  FT_Byte*   buffer,
                  FT_ULong   गणनम् )
  {
    पटिनिवत्त FT_Stream_ReadAt( stream, stream->pos, buffer, गणनम् );
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_ReadAt( FT_Stream  stream,
                    FT_ULong   pos,
                    FT_Byte*   buffer,
                    FT_ULong   गणनम् )
  {
    FT_Error  error = FT_Err_Ok;
    FT_ULong  read_bytes;


    सचे ( pos >= stream->size )
    {
      FT_ERROR(( "FT_Stream_ReadAt:"
                 " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
                 pos, stream->size ));

      पटिनिवत्त FT_THROW( Invalid_Stream_Operation );
    }

    सचे ( stream->पठ )
      read_bytes = stream->पठ( stream, pos, buffer, गणनम् );
    अञ्ञथा
    {
      read_bytes = stream->size - pos;
      सचे ( read_bytes > गणनम् )
        read_bytes = गणनम्;

      /* Allow "reading" zero bytes without UB even if buffer is NULL */
      सचे ( गणनम् )
        FT_MEM_COPY( buffer, stream->base + pos, read_bytes );
    }

    stream->pos = pos + read_bytes;

    सचे ( read_bytes < गणनम् )
    {
      FT_ERROR(( "FT_Stream_ReadAt:"
                 " invalid read; expected %lu bytes, got %lu\n",
                 गणनम्, read_bytes ));

      error = FT_THROW( Invalid_Stream_Operation );
    }

    पटिनिवत्त error;
  }


  FT_BASE_DEF( FT_ULong )
  FT_Stream_TryRead( FT_Stream  stream,
                     FT_Byte*   buffer,
                     FT_ULong   गणनम् )
  {
    FT_ULong  read_bytes = 0;


    सचे ( stream->pos >= stream->size )
      गच्छ Exit;

    सचे ( stream->पठ )
      read_bytes = stream->पठ( stream, stream->pos, buffer, गणनम् );
    अञ्ञथा
    {
      read_bytes = stream->size - stream->pos;
      सचे ( read_bytes > गणनम् )
        read_bytes = गणनम्;

      /* Allow "reading" zero bytes without UB even if buffer is NULL */
      सचे ( गणनम् )
        FT_MEM_COPY( buffer, stream->base + stream->pos, read_bytes );
    }

    stream->pos += read_bytes;

  Exit:
    पटिनिवत्त read_bytes;
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_ExtractFrame( FT_Stream  stream,
                          FT_ULong   गणनम्,
                          FT_Byte**  pbytes )
  {
    FT_Error  error;


    error = FT_Stream_EnterFrame( stream, गणनम् );
    सचे ( !error )
    {
      *pbytes = (FT_Byte*)stream->cursor;

      /* equivalent to FT_Stream_ExitFrame(), with no memory block release */
      stream->cursor = रिक्त;
      stream->limit  = रिक्त;
    }

    पटिनिवत्त error;
  }


  FT_BASE_DEF( शून्य )
  FT_Stream_ReleaseFrame( FT_Stream  stream,
                          FT_Byte**  pbytes )
  {
    सचे ( stream && stream->पठ )
    {
      FT_Memory  memory = stream->memory;


#ifdef FT_DEBUG_MEMORY
      ft_mem_free( memory, *pbytes );
#अञ्ञथा
      FT_FREE( *pbytes );
#endif
    }

    *pbytes = रिक्त;
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_EnterFrame( FT_Stream  stream,
                        FT_ULong   गणनम् )
  {
    FT_Error  error = FT_Err_Ok;
    FT_ULong  read_bytes;


    FT_TRACE7(( "FT_Stream_EnterFrame: %lu bytes\n", गणनम् ));

    /* check for nested frame access */
    FT_ASSERT( stream && stream->cursor == 0 );

    सचे ( stream->पठ )
    {
      /* allocate the frame in memory */
      FT_Memory  memory = stream->memory;


      /* simple sanity check */
      सचे ( गणनम् > stream->size )
      {
        FT_ERROR(( "FT_Stream_EnterFrame:"
                   " frame size (%lu) larger than stream size (%lu)\n",
                   गणनम्, stream->size ));

        error = FT_THROW( Invalid_Stream_Operation );
        गच्छ Exit;
      }

#ifdef FT_DEBUG_MEMORY
      /* assume `ft_debug_file_` and `ft_debug_lineno_` are already set */
      stream->base = (unsigned वर्ण*)ft_mem_qalloc( memory,
                                                    (FT_Long)गणनम्,
                                                    &error );
      सचे ( error )
        गच्छ Exit;
#अञ्ञथा
      सचे ( FT_QALLOC( stream->base, गणनम् ) )
        गच्छ Exit;
#endif
      /* read it */
      read_bytes = stream->पठ( stream, stream->pos,
                                 stream->base, गणनम् );
      सचे ( read_bytes < गणनम् )
      {
        FT_ERROR(( "FT_Stream_EnterFrame:"
                   " invalid read; expected %lu bytes, got %lu\n",
                   गणनम्, read_bytes ));

        FT_FREE( stream->base );
        error = FT_THROW( Invalid_Stream_Operation );
      }

      stream->cursor = stream->base;
      stream->limit  = FT_OFFSET( stream->cursor, गणनम् );
      stream->pos   += read_bytes;
    }
    अञ्ञथा
    {
      /* check current and new position */
      सचे ( stream->pos >= stream->size        ||
           stream->size - stream->pos < गणनम् )
      {
        FT_ERROR(( "FT_Stream_EnterFrame:"
                   " invalid i/o; pos = 0x%lx, count = %lu, size = 0x%lx\n",
                   stream->pos, गणनम्, stream->size ));

        error = FT_THROW( Invalid_Stream_Operation );
        गच्छ Exit;
      }

      /* set cursor */
      stream->cursor = stream->base + stream->pos;
      stream->limit  = stream->cursor + गणनम्;
      stream->pos   += गणनम्;
    }

  Exit:
    पटिनिवत्त error;
  }


  FT_BASE_DEF( शून्य )
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

    सचे ( stream->पठ )
    {
      FT_Memory  memory = stream->memory;


#ifdef FT_DEBUG_MEMORY
      ft_mem_free( memory, stream->base );
      stream->base = रिक्त;
#अञ्ञथा
      FT_FREE( stream->base );
#endif
    }

    stream->cursor = रिक्त;
    stream->limit  = रिक्त;
  }


  FT_BASE_DEF( FT_Byte )
  FT_Stream_GetByte( FT_Stream  stream )
  {
    FT_Byte  result;


    FT_ASSERT( stream && stream->cursor );

    result = 0;
    सचे ( stream->cursor < stream->limit )
      result = *stream->cursor++;

    पटिनिवत्त result;
  }


  FT_BASE_DEF( FT_UInt16 )
  FT_Stream_GetUShort( FT_Stream  stream )
  {
    FT_Byte*   p;
    FT_UInt16  result;


    FT_ASSERT( stream && stream->cursor );

    result         = 0;
    p              = stream->cursor;
    सचे ( p + 1 < stream->limit )
      result       = FT_NEXT_USHORT( p );
    stream->cursor = p;

    पटिनिवत्त result;
  }


  FT_BASE_DEF( FT_UInt16 )
  FT_Stream_GetUShortLE( FT_Stream  stream )
  {
    FT_Byte*   p;
    FT_UInt16  result;


    FT_ASSERT( stream && stream->cursor );

    result         = 0;
    p              = stream->cursor;
    सचे ( p + 1 < stream->limit )
      result       = FT_NEXT_USHORT_LE( p );
    stream->cursor = p;

    पटिनिवत्त result;
  }


  FT_BASE_DEF( FT_UInt32 )
  FT_Stream_GetUOffset( FT_Stream  stream )
  {
    FT_Byte*  p;
    FT_UInt32 result;


    FT_ASSERT( stream && stream->cursor );

    result         = 0;
    p              = stream->cursor;
    सचे ( p + 2 < stream->limit )
      result       = FT_NEXT_UOFF3( p );
    stream->cursor = p;
    पटिनिवत्त result;
  }


  FT_BASE_DEF( FT_UInt32 )
  FT_Stream_GetULong( FT_Stream  stream )
  {
    FT_Byte*  p;
    FT_UInt32 result;


    FT_ASSERT( stream && stream->cursor );

    result         = 0;
    p              = stream->cursor;
    सचे ( p + 3 < stream->limit )
      result       = FT_NEXT_ULONG( p );
    stream->cursor = p;
    पटिनिवत्त result;
  }


  FT_BASE_DEF( FT_UInt32 )
  FT_Stream_GetULongLE( FT_Stream  stream )
  {
    FT_Byte*  p;
    FT_UInt32 result;


    FT_ASSERT( stream && stream->cursor );

    result         = 0;
    p              = stream->cursor;
    सचे ( p + 3 < stream->limit )
      result       = FT_NEXT_ULONG_LE( p );
    stream->cursor = p;
    पटिनिवत्त result;
  }


  FT_BASE_DEF( FT_Byte )
  FT_Stream_ReadByte( FT_Stream  stream,
                      FT_Error*  error )
  {
    FT_Byte  result = 0;


    FT_ASSERT( stream );

    सचे ( stream->pos < stream->size )
    {
      सचे ( stream->पठ )
      {
        सचे ( stream->पठ( stream, stream->pos, &result, 1L ) != 1L )
          गच्छ Fail;
      }
      अञ्ञथा
        result = stream->base[stream->pos];
    }
    अञ्ञथा
      गच्छ Fail;

    stream->pos++;

    *error = FT_Err_Ok;

    पटिनिवत्त result;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadByte:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    पटिनिवत्त result;
  }


  FT_BASE_DEF( FT_UInt16 )
  FT_Stream_ReadUShort( FT_Stream  stream,
                        FT_Error*  error )
  {
    FT_Byte    reads[2];
    FT_Byte*   p;
    FT_UInt16  result = 0;


    FT_ASSERT( stream );

    सचे ( stream->pos + 1 < stream->size )
    {
      सचे ( stream->पठ )
      {
        सचे ( stream->पठ( stream, stream->pos, reads, 2L ) != 2L )
          गच्छ Fail;

        p = reads;
      }
      अञ्ञथा
        p = stream->base + stream->pos;

      सचे ( p )
        result = FT_NEXT_USHORT( p );
    }
    अञ्ञथा
      गच्छ Fail;

    stream->pos += 2;

    *error = FT_Err_Ok;

    पटिनिवत्त result;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadUShort:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    पटिनिवत्त result;
  }


  FT_BASE_DEF( FT_UInt16 )
  FT_Stream_ReadUShortLE( FT_Stream  stream,
                          FT_Error*  error )
  {
    FT_Byte    reads[2];
    FT_Byte*   p;
    FT_UInt16  result = 0;


    FT_ASSERT( stream );

    सचे ( stream->pos + 1 < stream->size )
    {
      सचे ( stream->पठ )
      {
        सचे ( stream->पठ( stream, stream->pos, reads, 2L ) != 2L )
          गच्छ Fail;

        p = reads;
      }
      अञ्ञथा
        p = stream->base + stream->pos;

      सचे ( p )
        result = FT_NEXT_USHORT_LE( p );
    }
    अञ्ञथा
      गच्छ Fail;

    stream->pos += 2;

    *error = FT_Err_Ok;

    पटिनिवत्त result;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadUShortLE:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    पटिनिवत्त result;
  }


  FT_BASE_DEF( FT_ULong )
  FT_Stream_ReadUOffset( FT_Stream  stream,
                         FT_Error*  error )
  {
    FT_Byte   reads[3];
    FT_Byte*  p;
    FT_ULong  result = 0;


    FT_ASSERT( stream );

    सचे ( stream->pos + 2 < stream->size )
    {
      सचे ( stream->पठ )
      {
        सचे (stream->पठ( stream, stream->pos, reads, 3L ) != 3L )
          गच्छ Fail;

        p = reads;
      }
      अञ्ञथा
        p = stream->base + stream->pos;

      सचे ( p )
        result = FT_NEXT_UOFF3( p );
    }
    अञ्ञथा
      गच्छ Fail;

    stream->pos += 3;

    *error = FT_Err_Ok;

    पटिनिवत्त result;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadUOffset:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    पटिनिवत्त result;
  }


  FT_BASE_DEF( FT_UInt32 )
  FT_Stream_ReadULong( FT_Stream  stream,
                       FT_Error*  error )
  {
    FT_Byte   reads[4];
    FT_Byte*  p;
    FT_UInt32 result = 0;


    FT_ASSERT( stream );

    सचे ( stream->pos + 3 < stream->size )
    {
      सचे ( stream->पठ )
      {
        सचे ( stream->पठ( stream, stream->pos, reads, 4L ) != 4L )
          गच्छ Fail;

        p = reads;
      }
      अञ्ञथा
        p = stream->base + stream->pos;

      सचे ( p )
        result = FT_NEXT_ULONG( p );
    }
    अञ्ञथा
      गच्छ Fail;

    stream->pos += 4;

    *error = FT_Err_Ok;

    पटिनिवत्त result;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadULong:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    पटिनिवत्त result;
  }


  FT_BASE_DEF( FT_UInt32 )
  FT_Stream_ReadULongLE( FT_Stream  stream,
                         FT_Error*  error )
  {
    FT_Byte   reads[4];
    FT_Byte*  p;
    FT_UInt32 result = 0;


    FT_ASSERT( stream );

    सचे ( stream->pos + 3 < stream->size )
    {
      सचे ( stream->पठ )
      {
        सचे ( stream->पठ( stream, stream->pos, reads, 4L ) != 4L )
          गच्छ Fail;

        p = reads;
      }
      अञ्ञथा
        p = stream->base + stream->pos;

      सचे ( p )
        result = FT_NEXT_ULONG_LE( p );
    }
    अञ्ञथा
      गच्छ Fail;

    stream->pos += 4;

    *error = FT_Err_Ok;

    पटिनिवत्त result;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadULongLE:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    पटिनिवत्त result;
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_ReadFields( FT_Stream              stream,
                        ध्रुव FT_Frame_Field*  fields,
                        शून्य*                  structure )
  {
    FT_Error  error;
    FT_Bool   frame_accessed = 0;
    FT_Byte*  cursor;


    सचे ( !fields )
      पटिनिवत्त FT_THROW( Invalid_Argument );

    सचे ( !stream )
      पटिनिवत्त FT_THROW( Invalid_Stream_Handle );

    cursor = stream->cursor;

    error = FT_Err_Ok;
    करोहि
    {
      FT_ULong  मानम्;
      FT_Int    sign_shift;
      FT_Byte*  p;


      विकल्प ( fields->मानम् )
      {
      पक्ष ft_frame_start:  /* access a new frame */
        error = FT_Stream_EnterFrame( stream, fields->offset );
        सचे ( error )
          गच्छ Exit;

        frame_accessed = 1;
        cursor         = stream->cursor;
        fields++;
        अनुवर्त;  /* loop! */

      पक्ष ft_frame_bytes:  /* read a byte sequence */
      पक्ष ft_frame_skip:   /* skip some bytes      */
        {
          FT_Offset  len = fields->size;


          सचे ( len > (FT_Offset)( stream->limit - cursor ) )
          {
            error = FT_THROW( Invalid_Stream_Operation );
            गच्छ Exit;
          }

          सचे ( fields->मानम् == ft_frame_bytes )
          {
            p = (FT_Byte*)structure + fields->offset;
            FT_MEM_COPY( p, cursor, len );
          }
          cursor += len;
          fields++;
          अनुवर्त;
        }

      पक्ष ft_frame_byte:
      पक्ष ft_frame_schar:  /* read a single byte */
        मानम् = FT_NEXT_BYTE( cursor );
        sign_shift = 24;
        भङ्ग;

      पक्ष ft_frame_short_be:
      पक्ष ft_frame_ushort_be:  /* read a 2-byte big-endian short */
        मानम् = FT_NEXT_USHORT( cursor );
        sign_shift = 16;
        भङ्ग;

      पक्ष ft_frame_short_le:
      पक्ष ft_frame_ushort_le:  /* read a 2-byte little-endian short */
        मानम् = FT_NEXT_USHORT_LE( cursor );
        sign_shift = 16;
        भङ्ग;

      पक्ष ft_frame_long_be:
      पक्ष ft_frame_ulong_be:  /* read a 4-byte big-endian long */
        मानम् = FT_NEXT_ULONG( cursor );
        sign_shift = 0;
        भङ्ग;

      पक्ष ft_frame_long_le:
      पक्ष ft_frame_ulong_le:  /* read a 4-byte little-endian long */
        मानम् = FT_NEXT_ULONG_LE( cursor );
        sign_shift = 0;
        भङ्ग;

      पक्ष ft_frame_off3_be:
      पक्ष ft_frame_uoff3_be:  /* read a 3-byte big-endian long */
        मानम् = FT_NEXT_UOFF3( cursor );
        sign_shift = 8;
        भङ्ग;

      पक्ष ft_frame_off3_le:
      पक्ष ft_frame_uoff3_le:  /* read a 3-byte little-endian long */
        मानम् = FT_NEXT_UOFF3_LE( cursor );
        sign_shift = 8;
        भङ्ग;

      स्वभाव:
        /* otherwise, exit the loop */
        stream->cursor = cursor;
        गच्छ Exit;
      }

      /* now, compute the signed value if necessary */
      सचे ( fields->मानम् & FT_FRAME_OP_SIGNED )
        मानम् = (FT_ULong)( (FT_Int32)( मानम् << sign_shift ) >> sign_shift );

      /* finally, store the value in the object */

      p = (FT_Byte*)structure + fields->offset;
      विकल्प ( fields->size )
      {
      पक्ष ( 8 / FT_CHAR_BIT ):
        *(FT_Byte*)p = (FT_Byte)मानम्;
        भङ्ग;

      पक्ष ( 16 / FT_CHAR_BIT ):
        *(FT_UShort*)p = (FT_UShort)मानम्;
        भङ्ग;

      पक्ष ( 32 / FT_CHAR_BIT ):
        *(FT_UInt32*)p = (FT_UInt32)मानम्;
        भङ्ग;

      स्वभाव:  /* for 64-bit systems */
        *(FT_ULong*)p = (FT_ULong)मानम्;
      }

      /* go to next field */
      fields++;
    }
    याव ( 1 );

  Exit:
    /* close the frame if it was opened by this read */
    सचे ( frame_accessed )
      FT_Stream_ExitFrame( stream );

    पटिनिवत्त error;
  }


/* END */
