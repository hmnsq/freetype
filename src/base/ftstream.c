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


#ضمن <freetype/internal/ftstream.h>
#ضمن <freetype/internal/ftdebug.h>


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#undef  FT_COMPONENT
#define FT_COMPONENT  stream


  FT_BASE_DEF( فراغ )
  FT_Stream_OpenMemory( FT_Stream       stream,
                        ثابت FT_Byte*  base,
                        FT_ULong        size )
  {
    stream->base   = (FT_Byte*) base;
    stream->size   = size;
    stream->pos    = 0;
    stream->cursor = عدم;
    stream->اقرأ   = عدم;
    stream->close  = عدم;
  }


  FT_BASE_DEF( فراغ )
  FT_Stream_Close( FT_Stream  stream )
  {
    إذا ( stream && stream->close )
      stream->close( stream );
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_Seek( FT_Stream  stream,
                  FT_ULong   pos )
  {
    FT_Error  error = FT_Err_Ok;


    إذا ( stream->اقرأ )
    {
      إذا ( stream->اقرأ( stream, pos, عدم, 0 ) )
      {
        FT_ERROR(( "FT_Stream_Seek:"
                   " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
                   pos, stream->size ));

        error = FT_THROW( Invalid_Stream_Operation );
      }
    }
    /* note that seeking to the first position after the file is valid */
    وإلا إذا ( pos > stream->size )
    {
      FT_ERROR(( "FT_Stream_Seek:"
                 " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
                 pos, stream->size ));

      error = FT_THROW( Invalid_Stream_Operation );
    }

    إذا ( !error )
      stream->pos = pos;

    أرجع error;
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_Skip( FT_Stream  stream,
                  FT_Long    distance )
  {
    إذا ( distance < 0 )
      أرجع FT_THROW( Invalid_Stream_Operation );

    أرجع FT_Stream_Seek( stream, stream->pos + (FT_ULong)distance );
  }


  FT_BASE_DEF( FT_ULong )
  FT_Stream_Pos( FT_Stream  stream )
  {
    أرجع stream->pos;
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_Read( FT_Stream  stream,
                  FT_Byte*   buffer,
                  FT_ULong   ژماردن )
  {
    أرجع FT_Stream_ReadAt( stream, stream->pos, buffer, ژماردن );
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_ReadAt( FT_Stream  stream,
                    FT_ULong   pos,
                    FT_Byte*   buffer,
                    FT_ULong   ژماردن )
  {
    FT_Error  error = FT_Err_Ok;
    FT_ULong  read_bytes;


    إذا ( pos >= stream->size )
    {
      FT_ERROR(( "FT_Stream_ReadAt:"
                 " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
                 pos, stream->size ));

      أرجع FT_THROW( Invalid_Stream_Operation );
    }

    إذا ( stream->اقرأ )
      read_bytes = stream->اقرأ( stream, pos, buffer, ژماردن );
    وإلا
    {
      read_bytes = stream->size - pos;
      إذا ( read_bytes > ژماردن )
        read_bytes = ژماردن;

      /* Allow "reading" zero bytes without UB even if buffer is NULL */
      إذا ( ژماردن )
        FT_MEM_COPY( buffer, stream->base + pos, read_bytes );
    }

    stream->pos = pos + read_bytes;

    إذا ( read_bytes < ژماردن )
    {
      FT_ERROR(( "FT_Stream_ReadAt:"
                 " invalid read; expected %lu bytes, got %lu\n",
                 ژماردن, read_bytes ));

      error = FT_THROW( Invalid_Stream_Operation );
    }

    أرجع error;
  }


  FT_BASE_DEF( FT_ULong )
  FT_Stream_TryRead( FT_Stream  stream,
                     FT_Byte*   buffer,
                     FT_ULong   ژماردن )
  {
    FT_ULong  read_bytes = 0;


    إذا ( stream->pos >= stream->size )
      اذهب Exit;

    إذا ( stream->اقرأ )
      read_bytes = stream->اقرأ( stream, stream->pos, buffer, ژماردن );
    وإلا
    {
      read_bytes = stream->size - stream->pos;
      إذا ( read_bytes > ژماردن )
        read_bytes = ژماردن;

      /* Allow "reading" zero bytes without UB even if buffer is NULL */
      إذا ( ژماردن )
        FT_MEM_COPY( buffer, stream->base + stream->pos, read_bytes );
    }

    stream->pos += read_bytes;

  Exit:
    أرجع read_bytes;
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_ExtractFrame( FT_Stream  stream,
                          FT_ULong   ژماردن,
                          FT_Byte**  pbytes )
  {
    FT_Error  error;


    error = FT_Stream_EnterFrame( stream, ژماردن );
    إذا ( !error )
    {
      *pbytes = (FT_Byte*)stream->cursor;

      /* equivalent to FT_Stream_ExitFrame(), with no memory block release */
      stream->cursor = عدم;
      stream->limit  = عدم;
    }

    أرجع error;
  }


  FT_BASE_DEF( فراغ )
  FT_Stream_ReleaseFrame( FT_Stream  stream,
                          FT_Byte**  pbytes )
  {
    إذا ( stream && stream->اقرأ )
    {
      FT_Memory  memory = stream->memory;


#ifdef FT_DEBUG_MEMORY
      ft_mem_free( memory, *pbytes );
#وإلا
      FT_FREE( *pbytes );
#endif
    }

    *pbytes = عدم;
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_EnterFrame( FT_Stream  stream,
                        FT_ULong   ژماردن )
  {
    FT_Error  error = FT_Err_Ok;
    FT_ULong  read_bytes;


    FT_TRACE7(( "FT_Stream_EnterFrame: %lu bytes\n", ژماردن ));

    /* check for nested frame access */
    FT_ASSERT( stream && stream->cursor == 0 );

    إذا ( stream->اقرأ )
    {
      /* allocate the frame in memory */
      FT_Memory  memory = stream->memory;


      /* simple sanity check */
      إذا ( ژماردن > stream->size )
      {
        FT_ERROR(( "FT_Stream_EnterFrame:"
                   " frame size (%lu) larger than stream size (%lu)\n",
                   ژماردن, stream->size ));

        error = FT_THROW( Invalid_Stream_Operation );
        اذهب Exit;
      }

#ifdef FT_DEBUG_MEMORY
      /* assume `ft_debug_file_` and `ft_debug_lineno_` are already set */
      stream->base = (unsigned حرف*)ft_mem_qalloc( memory,
                                                    (FT_Long)ژماردن,
                                                    &error );
      إذا ( error )
        اذهب Exit;
#وإلا
      إذا ( FT_QALLOC( stream->base, ژماردن ) )
        اذهب Exit;
#endif
      /* read it */
      read_bytes = stream->اقرأ( stream, stream->pos,
                                 stream->base, ژماردن );
      إذا ( read_bytes < ژماردن )
      {
        FT_ERROR(( "FT_Stream_EnterFrame:"
                   " invalid read; expected %lu bytes, got %lu\n",
                   ژماردن, read_bytes ));

        FT_FREE( stream->base );
        error = FT_THROW( Invalid_Stream_Operation );
      }

      stream->cursor = stream->base;
      stream->limit  = FT_OFFSET( stream->cursor, ژماردن );
      stream->pos   += read_bytes;
    }
    وإلا
    {
      /* check current and new position */
      إذا ( stream->pos >= stream->size        ||
           stream->size - stream->pos < ژماردن )
      {
        FT_ERROR(( "FT_Stream_EnterFrame:"
                   " invalid i/o; pos = 0x%lx, count = %lu, size = 0x%lx\n",
                   stream->pos, ژماردن, stream->size ));

        error = FT_THROW( Invalid_Stream_Operation );
        اذهب Exit;
      }

      /* set cursor */
      stream->cursor = stream->base + stream->pos;
      stream->limit  = stream->cursor + ژماردن;
      stream->pos   += ژماردن;
    }

  Exit:
    أرجع error;
  }


  FT_BASE_DEF( فراغ )
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

    إذا ( stream->اقرأ )
    {
      FT_Memory  memory = stream->memory;


#ifdef FT_DEBUG_MEMORY
      ft_mem_free( memory, stream->base );
      stream->base = عدم;
#وإلا
      FT_FREE( stream->base );
#endif
    }

    stream->cursor = عدم;
    stream->limit  = عدم;
  }


  FT_BASE_DEF( FT_Byte )
  FT_Stream_GetByte( FT_Stream  stream )
  {
    FT_Byte  result;


    FT_ASSERT( stream && stream->cursor );

    result = 0;
    إذا ( stream->cursor < stream->limit )
      result = *stream->cursor++;

    أرجع result;
  }


  FT_BASE_DEF( FT_UInt16 )
  FT_Stream_GetUShort( FT_Stream  stream )
  {
    FT_Byte*   p;
    FT_UInt16  result;


    FT_ASSERT( stream && stream->cursor );

    result         = 0;
    p              = stream->cursor;
    إذا ( p + 1 < stream->limit )
      result       = FT_NEXT_USHORT( p );
    stream->cursor = p;

    أرجع result;
  }


  FT_BASE_DEF( FT_UInt16 )
  FT_Stream_GetUShortLE( FT_Stream  stream )
  {
    FT_Byte*   p;
    FT_UInt16  result;


    FT_ASSERT( stream && stream->cursor );

    result         = 0;
    p              = stream->cursor;
    إذا ( p + 1 < stream->limit )
      result       = FT_NEXT_USHORT_LE( p );
    stream->cursor = p;

    أرجع result;
  }


  FT_BASE_DEF( FT_UInt32 )
  FT_Stream_GetUOffset( FT_Stream  stream )
  {
    FT_Byte*  p;
    FT_UInt32 result;


    FT_ASSERT( stream && stream->cursor );

    result         = 0;
    p              = stream->cursor;
    إذا ( p + 2 < stream->limit )
      result       = FT_NEXT_UOFF3( p );
    stream->cursor = p;
    أرجع result;
  }


  FT_BASE_DEF( FT_UInt32 )
  FT_Stream_GetULong( FT_Stream  stream )
  {
    FT_Byte*  p;
    FT_UInt32 result;


    FT_ASSERT( stream && stream->cursor );

    result         = 0;
    p              = stream->cursor;
    إذا ( p + 3 < stream->limit )
      result       = FT_NEXT_ULONG( p );
    stream->cursor = p;
    أرجع result;
  }


  FT_BASE_DEF( FT_UInt32 )
  FT_Stream_GetULongLE( FT_Stream  stream )
  {
    FT_Byte*  p;
    FT_UInt32 result;


    FT_ASSERT( stream && stream->cursor );

    result         = 0;
    p              = stream->cursor;
    إذا ( p + 3 < stream->limit )
      result       = FT_NEXT_ULONG_LE( p );
    stream->cursor = p;
    أرجع result;
  }


  FT_BASE_DEF( FT_Byte )
  FT_Stream_ReadByte( FT_Stream  stream,
                      FT_Error*  error )
  {
    FT_Byte  result = 0;


    FT_ASSERT( stream );

    إذا ( stream->pos < stream->size )
    {
      إذا ( stream->اقرأ )
      {
        إذا ( stream->اقرأ( stream, stream->pos, &result, 1L ) != 1L )
          اذهب Fail;
      }
      وإلا
        result = stream->base[stream->pos];
    }
    وإلا
      اذهب Fail;

    stream->pos++;

    *error = FT_Err_Ok;

    أرجع result;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadByte:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    أرجع result;
  }


  FT_BASE_DEF( FT_UInt16 )
  FT_Stream_ReadUShort( FT_Stream  stream,
                        FT_Error*  error )
  {
    FT_Byte    reads[2];
    FT_Byte*   p;
    FT_UInt16  result = 0;


    FT_ASSERT( stream );

    إذا ( stream->pos + 1 < stream->size )
    {
      إذا ( stream->اقرأ )
      {
        إذا ( stream->اقرأ( stream, stream->pos, reads, 2L ) != 2L )
          اذهب Fail;

        p = reads;
      }
      وإلا
        p = stream->base + stream->pos;

      إذا ( p )
        result = FT_NEXT_USHORT( p );
    }
    وإلا
      اذهب Fail;

    stream->pos += 2;

    *error = FT_Err_Ok;

    أرجع result;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadUShort:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    أرجع result;
  }


  FT_BASE_DEF( FT_UInt16 )
  FT_Stream_ReadUShortLE( FT_Stream  stream,
                          FT_Error*  error )
  {
    FT_Byte    reads[2];
    FT_Byte*   p;
    FT_UInt16  result = 0;


    FT_ASSERT( stream );

    إذا ( stream->pos + 1 < stream->size )
    {
      إذا ( stream->اقرأ )
      {
        إذا ( stream->اقرأ( stream, stream->pos, reads, 2L ) != 2L )
          اذهب Fail;

        p = reads;
      }
      وإلا
        p = stream->base + stream->pos;

      إذا ( p )
        result = FT_NEXT_USHORT_LE( p );
    }
    وإلا
      اذهب Fail;

    stream->pos += 2;

    *error = FT_Err_Ok;

    أرجع result;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadUShortLE:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    أرجع result;
  }


  FT_BASE_DEF( FT_ULong )
  FT_Stream_ReadUOffset( FT_Stream  stream,
                         FT_Error*  error )
  {
    FT_Byte   reads[3];
    FT_Byte*  p;
    FT_ULong  result = 0;


    FT_ASSERT( stream );

    إذا ( stream->pos + 2 < stream->size )
    {
      إذا ( stream->اقرأ )
      {
        إذا (stream->اقرأ( stream, stream->pos, reads, 3L ) != 3L )
          اذهب Fail;

        p = reads;
      }
      وإلا
        p = stream->base + stream->pos;

      إذا ( p )
        result = FT_NEXT_UOFF3( p );
    }
    وإلا
      اذهب Fail;

    stream->pos += 3;

    *error = FT_Err_Ok;

    أرجع result;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadUOffset:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    أرجع result;
  }


  FT_BASE_DEF( FT_UInt32 )
  FT_Stream_ReadULong( FT_Stream  stream,
                       FT_Error*  error )
  {
    FT_Byte   reads[4];
    FT_Byte*  p;
    FT_UInt32 result = 0;


    FT_ASSERT( stream );

    إذا ( stream->pos + 3 < stream->size )
    {
      إذا ( stream->اقرأ )
      {
        إذا ( stream->اقرأ( stream, stream->pos, reads, 4L ) != 4L )
          اذهب Fail;

        p = reads;
      }
      وإلا
        p = stream->base + stream->pos;

      إذا ( p )
        result = FT_NEXT_ULONG( p );
    }
    وإلا
      اذهب Fail;

    stream->pos += 4;

    *error = FT_Err_Ok;

    أرجع result;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadULong:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    أرجع result;
  }


  FT_BASE_DEF( FT_UInt32 )
  FT_Stream_ReadULongLE( FT_Stream  stream,
                         FT_Error*  error )
  {
    FT_Byte   reads[4];
    FT_Byte*  p;
    FT_UInt32 result = 0;


    FT_ASSERT( stream );

    إذا ( stream->pos + 3 < stream->size )
    {
      إذا ( stream->اقرأ )
      {
        إذا ( stream->اقرأ( stream, stream->pos, reads, 4L ) != 4L )
          اذهب Fail;

        p = reads;
      }
      وإلا
        p = stream->base + stream->pos;

      إذا ( p )
        result = FT_NEXT_ULONG_LE( p );
    }
    وإلا
      اذهب Fail;

    stream->pos += 4;

    *error = FT_Err_Ok;

    أرجع result;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadULongLE:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    أرجع result;
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_ReadFields( FT_Stream              stream,
                        ثابت FT_Frame_Field*  fields,
                        فراغ*                  structure )
  {
    FT_Error  error;
    FT_Bool   frame_accessed = 0;
    FT_Byte*  cursor;


    إذا ( !fields )
      أرجع FT_THROW( Invalid_Argument );

    إذا ( !stream )
      أرجع FT_THROW( Invalid_Stream_Handle );

    cursor = stream->cursor;

    error = FT_Err_Ok;
    افعل
    {
      FT_ULong  بەها;
      FT_Int    sign_shift;
      FT_Byte*  p;


      بدل ( fields->بەها )
      {
      حالة ft_frame_start:  /* access a new frame */
        error = FT_Stream_EnterFrame( stream, fields->offset );
        إذا ( error )
          اذهب Exit;

        frame_accessed = 1;
        cursor         = stream->cursor;
        fields++;
        استمر;  /* loop! */

      حالة ft_frame_bytes:  /* read a byte sequence */
      حالة ft_frame_skip:   /* skip some bytes      */
        {
          FT_Offset  len = fields->size;


          إذا ( len > (FT_Offset)( stream->limit - cursor ) )
          {
            error = FT_THROW( Invalid_Stream_Operation );
            اذهب Exit;
          }

          إذا ( fields->بەها == ft_frame_bytes )
          {
            p = (FT_Byte*)structure + fields->offset;
            FT_MEM_COPY( p, cursor, len );
          }
          cursor += len;
          fields++;
          استمر;
        }

      حالة ft_frame_byte:
      حالة ft_frame_schar:  /* read a single byte */
        بەها = FT_NEXT_BYTE( cursor );
        sign_shift = 24;
        اكسر;

      حالة ft_frame_short_be:
      حالة ft_frame_ushort_be:  /* read a 2-byte big-endian short */
        بەها = FT_NEXT_USHORT( cursor );
        sign_shift = 16;
        اكسر;

      حالة ft_frame_short_le:
      حالة ft_frame_ushort_le:  /* read a 2-byte little-endian short */
        بەها = FT_NEXT_USHORT_LE( cursor );
        sign_shift = 16;
        اكسر;

      حالة ft_frame_long_be:
      حالة ft_frame_ulong_be:  /* read a 4-byte big-endian long */
        بەها = FT_NEXT_ULONG( cursor );
        sign_shift = 0;
        اكسر;

      حالة ft_frame_long_le:
      حالة ft_frame_ulong_le:  /* read a 4-byte little-endian long */
        بەها = FT_NEXT_ULONG_LE( cursor );
        sign_shift = 0;
        اكسر;

      حالة ft_frame_off3_be:
      حالة ft_frame_uoff3_be:  /* read a 3-byte big-endian long */
        بەها = FT_NEXT_UOFF3( cursor );
        sign_shift = 8;
        اكسر;

      حالة ft_frame_off3_le:
      حالة ft_frame_uoff3_le:  /* read a 3-byte little-endian long */
        بەها = FT_NEXT_UOFF3_LE( cursor );
        sign_shift = 8;
        اكسر;

      افتراضي:
        /* otherwise, exit the loop */
        stream->cursor = cursor;
        اذهب Exit;
      }

      /* now, compute the signed value if necessary */
      إذا ( fields->بەها & FT_FRAME_OP_SIGNED )
        بەها = (FT_ULong)( (FT_Int32)( بەها << sign_shift ) >> sign_shift );

      /* finally, store the value in the object */

      p = (FT_Byte*)structure + fields->offset;
      بدل ( fields->size )
      {
      حالة ( 8 / FT_CHAR_BIT ):
        *(FT_Byte*)p = (FT_Byte)بەها;
        اكسر;

      حالة ( 16 / FT_CHAR_BIT ):
        *(FT_UShort*)p = (FT_UShort)بەها;
        اكسر;

      حالة ( 32 / FT_CHAR_BIT ):
        *(FT_UInt32*)p = (FT_UInt32)بەها;
        اكسر;

      افتراضي:  /* for 64-bit systems */
        *(FT_ULong*)p = (FT_ULong)بەها;
      }

      /* go to next field */
      fields++;
    }
    طالما ( 1 );

  Exit:
    /* close the frame if it was opened by this read */
    إذا ( frame_accessed )
      FT_Stream_ExitFrame( stream );

    أرجع error;
  }


/* END */
