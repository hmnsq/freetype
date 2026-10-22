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
                        अचर FT_Byte*  base,
                        FT_ULong        size )
  {
    stream->base   = (FT_Byte*) base;
    stream->size   = size;
    stream->pos    = 0;
    stream->cursor = रिक्त;
    stream->पठ   = रिक्त;
    stream->बंद  = रिक्त;
  }


  FT_BASE_DEF( शून्य )
  FT_Stream_Close( FT_Stream  stream )
  {
    यदि ( stream && stream->बंद )
      stream->बंद( stream );
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_Seek( FT_Stream  stream,
                  FT_ULong   pos )
  {
    FT_Error  error = FT_Err_Ok;


    यदि ( stream->पठ )
    {
      यदि ( stream->पठ( stream, pos, रिक्त, 0 ) )
      {
        FT_ERROR(( "FT_Stream_Seek:"
                   " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
                   pos, stream->size ));

        error = FT_THROW( Invalid_Stream_Operation );
      }
    }
    /* note that seeking to the first position after the file is valid */
    अन्यथा यदि ( pos > stream->size )
    {
      FT_ERROR(( "FT_Stream_Seek:"
                 " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
                 pos, stream->size ));

      error = FT_THROW( Invalid_Stream_Operation );
    }

    यदि ( !error )
      stream->pos = pos;

    फर्काऊ error;
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_Skip( FT_Stream  stream,
                  FT_Long    distance )
  {
    यदि ( distance < 0 )
      फर्काऊ FT_THROW( Invalid_Stream_Operation );

    फर्काऊ FT_Stream_Seek( stream, stream->pos + (FT_ULong)distance );
  }


  FT_BASE_DEF( FT_ULong )
  FT_Stream_Pos( FT_Stream  stream )
  {
    फर्काऊ stream->pos;
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_Read( FT_Stream  stream,
                  FT_Byte*   buffer,
                  FT_ULong   गन्ती )
  {
    फर्काऊ FT_Stream_ReadAt( stream, stream->pos, buffer, गन्ती );
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_ReadAt( FT_Stream  stream,
                    FT_ULong   pos,
                    FT_Byte*   buffer,
                    FT_ULong   गन्ती )
  {
    FT_Error  error = FT_Err_Ok;
    FT_ULong  read_bytes;


    यदि ( pos >= stream->size )
    {
      FT_ERROR(( "FT_Stream_ReadAt:"
                 " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
                 pos, stream->size ));

      फर्काऊ FT_THROW( Invalid_Stream_Operation );
    }

    यदि ( stream->पठ )
      read_bytes = stream->पठ( stream, pos, buffer, गन्ती );
    अन्यथा
    {
      read_bytes = stream->size - pos;
      यदि ( read_bytes > गन्ती )
        read_bytes = गन्ती;

      /* Allow "reading" zero bytes without UB even if buffer is NULL */
      यदि ( गन्ती )
        FT_MEM_COPY( buffer, stream->base + pos, read_bytes );
    }

    stream->pos = pos + read_bytes;

    यदि ( read_bytes < गन्ती )
    {
      FT_ERROR(( "FT_Stream_ReadAt:"
                 " invalid read; expected %lu bytes, got %lu\n",
                 गन्ती, read_bytes ));

      error = FT_THROW( Invalid_Stream_Operation );
    }

    फर्काऊ error;
  }


  FT_BASE_DEF( FT_ULong )
  FT_Stream_TryRead( FT_Stream  stream,
                     FT_Byte*   buffer,
                     FT_ULong   गन्ती )
  {
    FT_ULong  read_bytes = 0;


    यदि ( stream->pos >= stream->size )
      गच्छ Exit;

    यदि ( stream->पठ )
      read_bytes = stream->पठ( stream, stream->pos, buffer, गन्ती );
    अन्यथा
    {
      read_bytes = stream->size - stream->pos;
      यदि ( read_bytes > गन्ती )
        read_bytes = गन्ती;

      /* Allow "reading" zero bytes without UB even if buffer is NULL */
      यदि ( गन्ती )
        FT_MEM_COPY( buffer, stream->base + stream->pos, read_bytes );
    }

    stream->pos += read_bytes;

  Exit:
    फर्काऊ read_bytes;
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_ExtractFrame( FT_Stream  stream,
                          FT_ULong   गन्ती,
                          FT_Byte**  pbytes )
  {
    FT_Error  error;


    error = FT_Stream_EnterFrame( stream, गन्ती );
    यदि ( !error )
    {
      *pbytes = (FT_Byte*)stream->cursor;

      /* equivalent to FT_Stream_ExitFrame(), with no memory block release */
      stream->cursor = रिक्त;
      stream->limit  = रिक्त;
    }

    फर्काऊ error;
  }


  FT_BASE_DEF( शून्य )
  FT_Stream_ReleaseFrame( FT_Stream  stream,
                          FT_Byte**  pbytes )
  {
    यदि ( stream && stream->पठ )
    {
      FT_Memory  memory = stream->memory;


#ifdef FT_DEBUG_MEMORY
      ft_mem_free( memory, *pbytes );
#अन्यथा
      FT_FREE( *pbytes );
#endif
    }

    *pbytes = रिक्त;
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_EnterFrame( FT_Stream  stream,
                        FT_ULong   गन्ती )
  {
    FT_Error  error = FT_Err_Ok;
    FT_ULong  read_bytes;


    FT_TRACE7(( "FT_Stream_EnterFrame: %lu bytes\n", गन्ती ));

    /* check for nested frame access */
    FT_ASSERT( stream && stream->cursor == 0 );

    यदि ( stream->पठ )
    {
      /* allocate the frame in memory */
      FT_Memory  memory = stream->memory;


      /* simple sanity check */
      यदि ( गन्ती > stream->size )
      {
        FT_ERROR(( "FT_Stream_EnterFrame:"
                   " frame size (%lu) larger than stream size (%lu)\n",
                   गन्ती, stream->size ));

        error = FT_THROW( Invalid_Stream_Operation );
        गच्छ Exit;
      }

#ifdef FT_DEBUG_MEMORY
      /* assume `ft_debug_file_` and `ft_debug_lineno_` are already set */
      stream->base = (अचिन्हित वर्ण*)ft_mem_qalloc( memory,
                                                    (FT_Long)गन्ती,
                                                    &error );
      यदि ( error )
        गच्छ Exit;
#अन्यथा
      यदि ( FT_QALLOC( stream->base, गन्ती ) )
        गच्छ Exit;
#endif
      /* read it */
      read_bytes = stream->पठ( stream, stream->pos,
                                 stream->base, गन्ती );
      यदि ( read_bytes < गन्ती )
      {
        FT_ERROR(( "FT_Stream_EnterFrame:"
                   " invalid read; expected %lu bytes, got %lu\n",
                   गन्ती, read_bytes ));

        FT_FREE( stream->base );
        error = FT_THROW( Invalid_Stream_Operation );
      }

      stream->cursor = stream->base;
      stream->limit  = FT_OFFSET( stream->cursor, गन्ती );
      stream->pos   += read_bytes;
    }
    अन्यथा
    {
      /* check current and new position */
      यदि ( stream->pos >= stream->size        ||
           stream->size - stream->pos < गन्ती )
      {
        FT_ERROR(( "FT_Stream_EnterFrame:"
                   " invalid i/o; pos = 0x%lx, count = %lu, size = 0x%lx\n",
                   stream->pos, गन्ती, stream->size ));

        error = FT_THROW( Invalid_Stream_Operation );
        गच्छ Exit;
      }

      /* set cursor */
      stream->cursor = stream->base + stream->pos;
      stream->limit  = stream->cursor + गन्ती;
      stream->pos   += गन्ती;
    }

  Exit:
    फर्काऊ error;
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

    यदि ( stream->पठ )
    {
      FT_Memory  memory = stream->memory;


#ifdef FT_DEBUG_MEMORY
      ft_mem_free( memory, stream->base );
      stream->base = रिक्त;
#अन्यथा
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
    यदि ( stream->cursor < stream->limit )
      result = *stream->cursor++;

    फर्काऊ result;
  }


  FT_BASE_DEF( FT_UInt16 )
  FT_Stream_GetUShort( FT_Stream  stream )
  {
    FT_Byte*   p;
    FT_UInt16  result;


    FT_ASSERT( stream && stream->cursor );

    result         = 0;
    p              = stream->cursor;
    यदि ( p + 1 < stream->limit )
      result       = FT_NEXT_USHORT( p );
    stream->cursor = p;

    फर्काऊ result;
  }


  FT_BASE_DEF( FT_UInt16 )
  FT_Stream_GetUShortLE( FT_Stream  stream )
  {
    FT_Byte*   p;
    FT_UInt16  result;


    FT_ASSERT( stream && stream->cursor );

    result         = 0;
    p              = stream->cursor;
    यदि ( p + 1 < stream->limit )
      result       = FT_NEXT_USHORT_LE( p );
    stream->cursor = p;

    फर्काऊ result;
  }


  FT_BASE_DEF( FT_UInt32 )
  FT_Stream_GetUOffset( FT_Stream  stream )
  {
    FT_Byte*  p;
    FT_UInt32 result;


    FT_ASSERT( stream && stream->cursor );

    result         = 0;
    p              = stream->cursor;
    यदि ( p + 2 < stream->limit )
      result       = FT_NEXT_UOFF3( p );
    stream->cursor = p;
    फर्काऊ result;
  }


  FT_BASE_DEF( FT_UInt32 )
  FT_Stream_GetULong( FT_Stream  stream )
  {
    FT_Byte*  p;
    FT_UInt32 result;


    FT_ASSERT( stream && stream->cursor );

    result         = 0;
    p              = stream->cursor;
    यदि ( p + 3 < stream->limit )
      result       = FT_NEXT_ULONG( p );
    stream->cursor = p;
    फर्काऊ result;
  }


  FT_BASE_DEF( FT_UInt32 )
  FT_Stream_GetULongLE( FT_Stream  stream )
  {
    FT_Byte*  p;
    FT_UInt32 result;


    FT_ASSERT( stream && stream->cursor );

    result         = 0;
    p              = stream->cursor;
    यदि ( p + 3 < stream->limit )
      result       = FT_NEXT_ULONG_LE( p );
    stream->cursor = p;
    फर्काऊ result;
  }


  FT_BASE_DEF( FT_Byte )
  FT_Stream_ReadByte( FT_Stream  stream,
                      FT_Error*  error )
  {
    FT_Byte  result = 0;


    FT_ASSERT( stream );

    यदि ( stream->pos < stream->size )
    {
      यदि ( stream->पठ )
      {
        यदि ( stream->पठ( stream, stream->pos, &result, 1L ) != 1L )
          गच्छ Fail;
      }
      अन्यथा
        result = stream->base[stream->pos];
    }
    अन्यथा
      गच्छ Fail;

    stream->pos++;

    *error = FT_Err_Ok;

    फर्काऊ result;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadByte:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    फर्काऊ result;
  }


  FT_BASE_DEF( FT_UInt16 )
  FT_Stream_ReadUShort( FT_Stream  stream,
                        FT_Error*  error )
  {
    FT_Byte    reads[2];
    FT_Byte*   p;
    FT_UInt16  result = 0;


    FT_ASSERT( stream );

    यदि ( stream->pos + 1 < stream->size )
    {
      यदि ( stream->पठ )
      {
        यदि ( stream->पठ( stream, stream->pos, reads, 2L ) != 2L )
          गच्छ Fail;

        p = reads;
      }
      अन्यथा
        p = stream->base + stream->pos;

      यदि ( p )
        result = FT_NEXT_USHORT( p );
    }
    अन्यथा
      गच्छ Fail;

    stream->pos += 2;

    *error = FT_Err_Ok;

    फर्काऊ result;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadUShort:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    फर्काऊ result;
  }


  FT_BASE_DEF( FT_UInt16 )
  FT_Stream_ReadUShortLE( FT_Stream  stream,
                          FT_Error*  error )
  {
    FT_Byte    reads[2];
    FT_Byte*   p;
    FT_UInt16  result = 0;


    FT_ASSERT( stream );

    यदि ( stream->pos + 1 < stream->size )
    {
      यदि ( stream->पठ )
      {
        यदि ( stream->पठ( stream, stream->pos, reads, 2L ) != 2L )
          गच्छ Fail;

        p = reads;
      }
      अन्यथा
        p = stream->base + stream->pos;

      यदि ( p )
        result = FT_NEXT_USHORT_LE( p );
    }
    अन्यथा
      गच्छ Fail;

    stream->pos += 2;

    *error = FT_Err_Ok;

    फर्काऊ result;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadUShortLE:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    फर्काऊ result;
  }


  FT_BASE_DEF( FT_ULong )
  FT_Stream_ReadUOffset( FT_Stream  stream,
                         FT_Error*  error )
  {
    FT_Byte   reads[3];
    FT_Byte*  p;
    FT_ULong  result = 0;


    FT_ASSERT( stream );

    यदि ( stream->pos + 2 < stream->size )
    {
      यदि ( stream->पठ )
      {
        यदि (stream->पठ( stream, stream->pos, reads, 3L ) != 3L )
          गच्छ Fail;

        p = reads;
      }
      अन्यथा
        p = stream->base + stream->pos;

      यदि ( p )
        result = FT_NEXT_UOFF3( p );
    }
    अन्यथा
      गच्छ Fail;

    stream->pos += 3;

    *error = FT_Err_Ok;

    फर्काऊ result;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadUOffset:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    फर्काऊ result;
  }


  FT_BASE_DEF( FT_UInt32 )
  FT_Stream_ReadULong( FT_Stream  stream,
                       FT_Error*  error )
  {
    FT_Byte   reads[4];
    FT_Byte*  p;
    FT_UInt32 result = 0;


    FT_ASSERT( stream );

    यदि ( stream->pos + 3 < stream->size )
    {
      यदि ( stream->पठ )
      {
        यदि ( stream->पठ( stream, stream->pos, reads, 4L ) != 4L )
          गच्छ Fail;

        p = reads;
      }
      अन्यथा
        p = stream->base + stream->pos;

      यदि ( p )
        result = FT_NEXT_ULONG( p );
    }
    अन्यथा
      गच्छ Fail;

    stream->pos += 4;

    *error = FT_Err_Ok;

    फर्काऊ result;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadULong:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    फर्काऊ result;
  }


  FT_BASE_DEF( FT_UInt32 )
  FT_Stream_ReadULongLE( FT_Stream  stream,
                         FT_Error*  error )
  {
    FT_Byte   reads[4];
    FT_Byte*  p;
    FT_UInt32 result = 0;


    FT_ASSERT( stream );

    यदि ( stream->pos + 3 < stream->size )
    {
      यदि ( stream->पठ )
      {
        यदि ( stream->पठ( stream, stream->pos, reads, 4L ) != 4L )
          गच्छ Fail;

        p = reads;
      }
      अन्यथा
        p = stream->base + stream->pos;

      यदि ( p )
        result = FT_NEXT_ULONG_LE( p );
    }
    अन्यथा
      गच्छ Fail;

    stream->pos += 4;

    *error = FT_Err_Ok;

    फर्काऊ result;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadULongLE:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    फर्काऊ result;
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_ReadFields( FT_Stream              stream,
                        अचर FT_Frame_Field*  fields,
                        शून्य*                  structure )
  {
    FT_Error  error;
    FT_Bool   frame_accessed = 0;
    FT_Byte*  cursor;


    यदि ( !fields )
      फर्काऊ FT_THROW( Invalid_Argument );

    यदि ( !stream )
      फर्काऊ FT_THROW( Invalid_Stream_Handle );

    cursor = stream->cursor;

    error = FT_Err_Ok;
    गर
    {
      FT_ULong  मान;
      FT_Int    sign_shift;
      FT_Byte*  p;


      विकल्प ( fields->मान )
      {
      पक्ष ft_frame_start:  /* access a new frame */
        error = FT_Stream_EnterFrame( stream, fields->offset );
        यदि ( error )
          गच्छ Exit;

        frame_accessed = 1;
        cursor         = stream->cursor;
        fields++;
        जारी;  /* loop! */

      पक्ष ft_frame_bytes:  /* read a byte sequence */
      पक्ष ft_frame_skip:   /* skip some bytes      */
        {
          FT_Offset  len = fields->size;


          यदि ( len > (FT_Offset)( stream->limit - cursor ) )
          {
            error = FT_THROW( Invalid_Stream_Operation );
            गच्छ Exit;
          }

          यदि ( fields->मान == ft_frame_bytes )
          {
            p = (FT_Byte*)structure + fields->offset;
            FT_MEM_COPY( p, cursor, len );
          }
          cursor += len;
          fields++;
          जारी;
        }

      पक्ष ft_frame_byte:
      पक्ष ft_frame_schar:  /* read a single byte */
        मान = FT_NEXT_BYTE( cursor );
        sign_shift = 24;
        रोक;

      पक्ष ft_frame_short_be:
      पक्ष ft_frame_ushort_be:  /* read a 2-byte big-endian short */
        मान = FT_NEXT_USHORT( cursor );
        sign_shift = 16;
        रोक;

      पक्ष ft_frame_short_le:
      पक्ष ft_frame_ushort_le:  /* read a 2-byte little-endian short */
        मान = FT_NEXT_USHORT_LE( cursor );
        sign_shift = 16;
        रोक;

      पक्ष ft_frame_long_be:
      पक्ष ft_frame_ulong_be:  /* read a 4-byte big-endian long */
        मान = FT_NEXT_ULONG( cursor );
        sign_shift = 0;
        रोक;

      पक्ष ft_frame_long_le:
      पक्ष ft_frame_ulong_le:  /* read a 4-byte little-endian long */
        मान = FT_NEXT_ULONG_LE( cursor );
        sign_shift = 0;
        रोक;

      पक्ष ft_frame_off3_be:
      पक्ष ft_frame_uoff3_be:  /* read a 3-byte big-endian long */
        मान = FT_NEXT_UOFF3( cursor );
        sign_shift = 8;
        रोक;

      पक्ष ft_frame_off3_le:
      पक्ष ft_frame_uoff3_le:  /* read a 3-byte little-endian long */
        मान = FT_NEXT_UOFF3_LE( cursor );
        sign_shift = 8;
        रोक;

      स्वभाव:
        /* otherwise, exit the loop */
        stream->cursor = cursor;
        गच्छ Exit;
      }

      /* now, compute the signed value if necessary */
      यदि ( fields->मान & FT_FRAME_OP_SIGNED )
        मान = (FT_ULong)( (FT_Int32)( मान << sign_shift ) >> sign_shift );

      /* finally, store the value in the object */

      p = (FT_Byte*)structure + fields->offset;
      विकल्प ( fields->size )
      {
      पक्ष ( 8 / FT_CHAR_BIT ):
        *(FT_Byte*)p = (FT_Byte)मान;
        रोक;

      पक्ष ( 16 / FT_CHAR_BIT ):
        *(FT_UShort*)p = (FT_UShort)मान;
        रोक;

      पक्ष ( 32 / FT_CHAR_BIT ):
        *(FT_UInt32*)p = (FT_UInt32)मान;
        रोक;

      स्वभाव:  /* for 64-bit systems */
        *(FT_ULong*)p = (FT_ULong)मान;
      }

      /* go to next field */
      fields++;
    }
    जबसम्म ( 1 );

  Exit:
    /* close the frame if it was opened by this read */
    यदि ( frame_accessed )
      FT_Stream_ExitFrame( stream );

    फर्काऊ error;
  }


/* END */
