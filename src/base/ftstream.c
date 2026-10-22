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


#समावेश <freetype/internal/ftstream.h>
#समावेश <freetype/internal/ftdebug.h>


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#अघोषित  FT_COMPONENT
#घोषणा FT_COMPONENT  stream


  FT_BASE_DEF( व्योम )
  FT_Stream_OpenMemory( FT_Stream       stream,
                        स्थिर FT_Byte*  base,
                        FT_ULong        आकार )
  {
    stream->base   = (FT_Byte*) base;
    stream->आकार   = आकार;
    stream->pos    = 0;
    stream->cursor = शून्य;
    stream->पढ़ो   = शून्य;
    stream->बंद  = शून्य;
  }


  FT_BASE_DEF( व्योम )
  FT_Stream_Close( FT_Stream  stream )
  {
    अगर ( stream && stream->बंद )
      stream->बंद( stream );
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_Seek( FT_Stream  stream,
                  FT_ULong   pos )
  {
    FT_Error  error = FT_Err_Ok;


    अगर ( stream->पढ़ो )
    {
      अगर ( stream->पढ़ो( stream, pos, शून्य, 0 ) )
      {
        FT_ERROR(( "FT_Stream_Seek:"
                   " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
                   pos, stream->आकार ));

        error = FT_THROW( Invalid_Stream_Operation );
      }
    }
    /* note that seeking to the first position after the file is valid */
    अन्यथा अगर ( pos > stream->आकार )
    {
      FT_ERROR(( "FT_Stream_Seek:"
                 " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
                 pos, stream->आकार ));

      error = FT_THROW( Invalid_Stream_Operation );
    }

    अगर ( !error )
      stream->pos = pos;

    वापस error;
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_Skip( FT_Stream  stream,
                  FT_Long    distance )
  {
    अगर ( distance < 0 )
      वापस FT_THROW( Invalid_Stream_Operation );

    वापस FT_Stream_Seek( stream, stream->pos + (FT_ULong)distance );
  }


  FT_BASE_DEF( FT_ULong )
  FT_Stream_Pos( FT_Stream  stream )
  {
    वापस stream->pos;
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_Read( FT_Stream  stream,
                  FT_Byte*   बफर,
                  FT_ULong   गिनती )
  {
    वापस FT_Stream_ReadAt( stream, stream->pos, बफर, गिनती );
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_ReadAt( FT_Stream  stream,
                    FT_ULong   pos,
                    FT_Byte*   बफर,
                    FT_ULong   गिनती )
  {
    FT_Error  error = FT_Err_Ok;
    FT_ULong  read_bytes;


    अगर ( pos >= stream->आकार )
    {
      FT_ERROR(( "FT_Stream_ReadAt:"
                 " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
                 pos, stream->आकार ));

      वापस FT_THROW( Invalid_Stream_Operation );
    }

    अगर ( stream->पढ़ो )
      read_bytes = stream->पढ़ो( stream, pos, बफर, गिनती );
    अन्यथा
    {
      read_bytes = stream->आकार - pos;
      अगर ( read_bytes > गिनती )
        read_bytes = गिनती;

      /* Allow "reading" zero bytes without UB even if buffer is NULL */
      अगर ( गिनती )
        FT_MEM_COPY( बफर, stream->base + pos, read_bytes );
    }

    stream->pos = pos + read_bytes;

    अगर ( read_bytes < गिनती )
    {
      FT_ERROR(( "FT_Stream_ReadAt:"
                 " invalid read; expected %lu bytes, got %lu\n",
                 गिनती, read_bytes ));

      error = FT_THROW( Invalid_Stream_Operation );
    }

    वापस error;
  }


  FT_BASE_DEF( FT_ULong )
  FT_Stream_TryRead( FT_Stream  stream,
                     FT_Byte*   बफर,
                     FT_ULong   गिनती )
  {
    FT_ULong  read_bytes = 0;


    अगर ( stream->pos >= stream->आकार )
      जाओ Exit;

    अगर ( stream->पढ़ो )
      read_bytes = stream->पढ़ो( stream, stream->pos, बफर, गिनती );
    अन्यथा
    {
      read_bytes = stream->आकार - stream->pos;
      अगर ( read_bytes > गिनती )
        read_bytes = गिनती;

      /* Allow "reading" zero bytes without UB even if buffer is NULL */
      अगर ( गिनती )
        FT_MEM_COPY( बफर, stream->base + stream->pos, read_bytes );
    }

    stream->pos += read_bytes;

  Exit:
    वापस read_bytes;
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_ExtractFrame( FT_Stream  stream,
                          FT_ULong   गिनती,
                          FT_Byte**  pbytes )
  {
    FT_Error  error;


    error = FT_Stream_EnterFrame( stream, गिनती );
    अगर ( !error )
    {
      *pbytes = (FT_Byte*)stream->cursor;

      /* equivalent to FT_Stream_ExitFrame(), with no memory block release */
      stream->cursor = शून्य;
      stream->limit  = शून्य;
    }

    वापस error;
  }


  FT_BASE_DEF( व्योम )
  FT_Stream_ReleaseFrame( FT_Stream  stream,
                          FT_Byte**  pbytes )
  {
    अगर ( stream && stream->पढ़ो )
    {
      FT_Memory  memory = stream->memory;


#अगर_घोषित FT_DEBUG_MEMORY
      ft_mem_free( memory, *pbytes );
#अन्यथा
      FT_FREE( *pbytes );
#पूर्ण_अगर
    }

    *pbytes = शून्य;
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_EnterFrame( FT_Stream  stream,
                        FT_ULong   गिनती )
  {
    FT_Error  error = FT_Err_Ok;
    FT_ULong  read_bytes;


    FT_TRACE7(( "FT_Stream_EnterFrame: %lu bytes\n", गिनती ));

    /* check for nested frame access */
    FT_ASSERT( stream && stream->cursor == 0 );

    अगर ( stream->पढ़ो )
    {
      /* allocate the frame in memory */
      FT_Memory  memory = stream->memory;


      /* simple sanity check */
      अगर ( गिनती > stream->आकार )
      {
        FT_ERROR(( "FT_Stream_EnterFrame:"
                   " frame size (%lu) larger than stream size (%lu)\n",
                   गिनती, stream->आकार ));

        error = FT_THROW( Invalid_Stream_Operation );
        जाओ Exit;
      }

#अगर_घोषित FT_DEBUG_MEMORY
      /* assume `ft_debug_file_` and `ft_debug_lineno_` are already set */
      stream->base = (अचिन्हित अक्षर*)ft_mem_qalloc( memory,
                                                    (FT_Long)गिनती,
                                                    &error );
      अगर ( error )
        जाओ Exit;
#अन्यथा
      अगर ( FT_QALLOC( stream->base, गिनती ) )
        जाओ Exit;
#पूर्ण_अगर
      /* read it */
      read_bytes = stream->पढ़ो( stream, stream->pos,
                                 stream->base, गिनती );
      अगर ( read_bytes < गिनती )
      {
        FT_ERROR(( "FT_Stream_EnterFrame:"
                   " invalid read; expected %lu bytes, got %lu\n",
                   गिनती, read_bytes ));

        FT_FREE( stream->base );
        error = FT_THROW( Invalid_Stream_Operation );
      }

      stream->cursor = stream->base;
      stream->limit  = FT_OFFSET( stream->cursor, गिनती );
      stream->pos   += read_bytes;
    }
    अन्यथा
    {
      /* check current and new position */
      अगर ( stream->pos >= stream->आकार        ||
           stream->आकार - stream->pos < गिनती )
      {
        FT_ERROR(( "FT_Stream_EnterFrame:"
                   " invalid i/o; pos = 0x%lx, count = %lu, size = 0x%lx\n",
                   stream->pos, गिनती, stream->आकार ));

        error = FT_THROW( Invalid_Stream_Operation );
        जाओ Exit;
      }

      /* set cursor */
      stream->cursor = stream->base + stream->pos;
      stream->limit  = stream->cursor + गिनती;
      stream->pos   += गिनती;
    }

  Exit:
    वापस error;
  }


  FT_BASE_DEF( व्योम )
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

    अगर ( stream->पढ़ो )
    {
      FT_Memory  memory = stream->memory;


#अगर_घोषित FT_DEBUG_MEMORY
      ft_mem_free( memory, stream->base );
      stream->base = शून्य;
#अन्यथा
      FT_FREE( stream->base );
#पूर्ण_अगर
    }

    stream->cursor = शून्य;
    stream->limit  = शून्य;
  }


  FT_BASE_DEF( FT_Byte )
  FT_Stream_GetByte( FT_Stream  stream )
  {
    FT_Byte  परिणाम;


    FT_ASSERT( stream && stream->cursor );

    परिणाम = 0;
    अगर ( stream->cursor < stream->limit )
      परिणाम = *stream->cursor++;

    वापस परिणाम;
  }


  FT_BASE_DEF( FT_UInt16 )
  FT_Stream_GetUShort( FT_Stream  stream )
  {
    FT_Byte*   p;
    FT_UInt16  परिणाम;


    FT_ASSERT( stream && stream->cursor );

    परिणाम         = 0;
    p              = stream->cursor;
    अगर ( p + 1 < stream->limit )
      परिणाम       = FT_अग्र_USHORT( p );
    stream->cursor = p;

    वापस परिणाम;
  }


  FT_BASE_DEF( FT_UInt16 )
  FT_Stream_GetUShortLE( FT_Stream  stream )
  {
    FT_Byte*   p;
    FT_UInt16  परिणाम;


    FT_ASSERT( stream && stream->cursor );

    परिणाम         = 0;
    p              = stream->cursor;
    अगर ( p + 1 < stream->limit )
      परिणाम       = FT_अग्र_USHORT_LE( p );
    stream->cursor = p;

    वापस परिणाम;
  }


  FT_BASE_DEF( FT_UInt32 )
  FT_Stream_GetUOffset( FT_Stream  stream )
  {
    FT_Byte*  p;
    FT_UInt32 परिणाम;


    FT_ASSERT( stream && stream->cursor );

    परिणाम         = 0;
    p              = stream->cursor;
    अगर ( p + 2 < stream->limit )
      परिणाम       = FT_अग्र_UOFF3( p );
    stream->cursor = p;
    वापस परिणाम;
  }


  FT_BASE_DEF( FT_UInt32 )
  FT_Stream_GetULong( FT_Stream  stream )
  {
    FT_Byte*  p;
    FT_UInt32 परिणाम;


    FT_ASSERT( stream && stream->cursor );

    परिणाम         = 0;
    p              = stream->cursor;
    अगर ( p + 3 < stream->limit )
      परिणाम       = FT_अग्र_ULONG( p );
    stream->cursor = p;
    वापस परिणाम;
  }


  FT_BASE_DEF( FT_UInt32 )
  FT_Stream_GetULongLE( FT_Stream  stream )
  {
    FT_Byte*  p;
    FT_UInt32 परिणाम;


    FT_ASSERT( stream && stream->cursor );

    परिणाम         = 0;
    p              = stream->cursor;
    अगर ( p + 3 < stream->limit )
      परिणाम       = FT_अग्र_ULONG_LE( p );
    stream->cursor = p;
    वापस परिणाम;
  }


  FT_BASE_DEF( FT_Byte )
  FT_Stream_ReadByte( FT_Stream  stream,
                      FT_Error*  error )
  {
    FT_Byte  परिणाम = 0;


    FT_ASSERT( stream );

    अगर ( stream->pos < stream->आकार )
    {
      अगर ( stream->पढ़ो )
      {
        अगर ( stream->पढ़ो( stream, stream->pos, &परिणाम, 1L ) != 1L )
          जाओ Fail;
      }
      अन्यथा
        परिणाम = stream->base[stream->pos];
    }
    अन्यथा
      जाओ Fail;

    stream->pos++;

    *error = FT_Err_Ok;

    वापस परिणाम;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadByte:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->आकार ));

    वापस परिणाम;
  }


  FT_BASE_DEF( FT_UInt16 )
  FT_Stream_ReadUShort( FT_Stream  stream,
                        FT_Error*  error )
  {
    FT_Byte    reads[2];
    FT_Byte*   p;
    FT_UInt16  परिणाम = 0;


    FT_ASSERT( stream );

    अगर ( stream->pos + 1 < stream->आकार )
    {
      अगर ( stream->पढ़ो )
      {
        अगर ( stream->पढ़ो( stream, stream->pos, reads, 2L ) != 2L )
          जाओ Fail;

        p = reads;
      }
      अन्यथा
        p = stream->base + stream->pos;

      अगर ( p )
        परिणाम = FT_अग्र_USHORT( p );
    }
    अन्यथा
      जाओ Fail;

    stream->pos += 2;

    *error = FT_Err_Ok;

    वापस परिणाम;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadUShort:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->आकार ));

    वापस परिणाम;
  }


  FT_BASE_DEF( FT_UInt16 )
  FT_Stream_ReadUShortLE( FT_Stream  stream,
                          FT_Error*  error )
  {
    FT_Byte    reads[2];
    FT_Byte*   p;
    FT_UInt16  परिणाम = 0;


    FT_ASSERT( stream );

    अगर ( stream->pos + 1 < stream->आकार )
    {
      अगर ( stream->पढ़ो )
      {
        अगर ( stream->पढ़ो( stream, stream->pos, reads, 2L ) != 2L )
          जाओ Fail;

        p = reads;
      }
      अन्यथा
        p = stream->base + stream->pos;

      अगर ( p )
        परिणाम = FT_अग्र_USHORT_LE( p );
    }
    अन्यथा
      जाओ Fail;

    stream->pos += 2;

    *error = FT_Err_Ok;

    वापस परिणाम;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadUShortLE:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->आकार ));

    वापस परिणाम;
  }


  FT_BASE_DEF( FT_ULong )
  FT_Stream_ReadUOffset( FT_Stream  stream,
                         FT_Error*  error )
  {
    FT_Byte   reads[3];
    FT_Byte*  p;
    FT_ULong  परिणाम = 0;


    FT_ASSERT( stream );

    अगर ( stream->pos + 2 < stream->आकार )
    {
      अगर ( stream->पढ़ो )
      {
        अगर (stream->पढ़ो( stream, stream->pos, reads, 3L ) != 3L )
          जाओ Fail;

        p = reads;
      }
      अन्यथा
        p = stream->base + stream->pos;

      अगर ( p )
        परिणाम = FT_अग्र_UOFF3( p );
    }
    अन्यथा
      जाओ Fail;

    stream->pos += 3;

    *error = FT_Err_Ok;

    वापस परिणाम;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadUOffset:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->आकार ));

    वापस परिणाम;
  }


  FT_BASE_DEF( FT_UInt32 )
  FT_Stream_ReadULong( FT_Stream  stream,
                       FT_Error*  error )
  {
    FT_Byte   reads[4];
    FT_Byte*  p;
    FT_UInt32 परिणाम = 0;


    FT_ASSERT( stream );

    अगर ( stream->pos + 3 < stream->आकार )
    {
      अगर ( stream->पढ़ो )
      {
        अगर ( stream->पढ़ो( stream, stream->pos, reads, 4L ) != 4L )
          जाओ Fail;

        p = reads;
      }
      अन्यथा
        p = stream->base + stream->pos;

      अगर ( p )
        परिणाम = FT_अग्र_ULONG( p );
    }
    अन्यथा
      जाओ Fail;

    stream->pos += 4;

    *error = FT_Err_Ok;

    वापस परिणाम;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadULong:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->आकार ));

    वापस परिणाम;
  }


  FT_BASE_DEF( FT_UInt32 )
  FT_Stream_ReadULongLE( FT_Stream  stream,
                         FT_Error*  error )
  {
    FT_Byte   reads[4];
    FT_Byte*  p;
    FT_UInt32 परिणाम = 0;


    FT_ASSERT( stream );

    अगर ( stream->pos + 3 < stream->आकार )
    {
      अगर ( stream->पढ़ो )
      {
        अगर ( stream->पढ़ो( stream, stream->pos, reads, 4L ) != 4L )
          जाओ Fail;

        p = reads;
      }
      अन्यथा
        p = stream->base + stream->pos;

      अगर ( p )
        परिणाम = FT_अग्र_ULONG_LE( p );
    }
    अन्यथा
      जाओ Fail;

    stream->pos += 4;

    *error = FT_Err_Ok;

    वापस परिणाम;

  Fail:
    *error = FT_THROW( Invalid_Stream_Operation );
    FT_ERROR(( "FT_Stream_ReadULongLE:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->आकार ));

    वापस परिणाम;
  }


  FT_BASE_DEF( FT_Error )
  FT_Stream_ReadFields( FT_Stream              stream,
                        स्थिर FT_Frame_Field*  fields,
                        व्योम*                  structure )
  {
    FT_Error  error;
    FT_Bool   frame_accessed = 0;
    FT_Byte*  cursor;


    अगर ( !fields )
      वापस FT_THROW( Invalid_Argument );

    अगर ( !stream )
      वापस FT_THROW( Invalid_Stream_Handle );

    cursor = stream->cursor;

    error = FT_Err_Ok;
    करो
    {
      FT_ULong  मूल्य;
      FT_Int    sign_shift;
      FT_Byte*  p;


      चयन ( fields->मूल्य )
      {
      हाल ft_frame_start:  /* access a new frame */
        error = FT_Stream_EnterFrame( stream, fields->offset );
        अगर ( error )
          जाओ Exit;

        frame_accessed = 1;
        cursor         = stream->cursor;
        fields++;
        जारी;  /* loop! */

      हाल ft_frame_bytes:  /* read a byte sequence */
      हाल ft_frame_skip:   /* skip some bytes      */
        {
          FT_Offset  len = fields->आकार;


          अगर ( len > (FT_Offset)( stream->limit - cursor ) )
          {
            error = FT_THROW( Invalid_Stream_Operation );
            जाओ Exit;
          }

          अगर ( fields->मूल्य == ft_frame_bytes )
          {
            p = (FT_Byte*)structure + fields->offset;
            FT_MEM_COPY( p, cursor, len );
          }
          cursor += len;
          fields++;
          जारी;
        }

      हाल ft_frame_byte:
      हाल ft_frame_schar:  /* read a single byte */
        मूल्य = FT_अग्र_BYTE( cursor );
        sign_shift = 24;
        अवरोध;

      हाल ft_frame_short_be:
      हाल ft_frame_ushort_be:  /* read a 2-byte big-endian short */
        मूल्य = FT_अग्र_USHORT( cursor );
        sign_shift = 16;
        अवरोध;

      हाल ft_frame_short_le:
      हाल ft_frame_ushort_le:  /* read a 2-byte little-endian short */
        मूल्य = FT_अग्र_USHORT_LE( cursor );
        sign_shift = 16;
        अवरोध;

      हाल ft_frame_long_be:
      हाल ft_frame_ulong_be:  /* read a 4-byte big-endian long */
        मूल्य = FT_अग्र_ULONG( cursor );
        sign_shift = 0;
        अवरोध;

      हाल ft_frame_long_le:
      हाल ft_frame_ulong_le:  /* read a 4-byte little-endian long */
        मूल्य = FT_अग्र_ULONG_LE( cursor );
        sign_shift = 0;
        अवरोध;

      हाल ft_frame_off3_be:
      हाल ft_frame_uoff3_be:  /* read a 3-byte big-endian long */
        मूल्य = FT_अग्र_UOFF3( cursor );
        sign_shift = 8;
        अवरोध;

      हाल ft_frame_off3_le:
      हाल ft_frame_uoff3_le:  /* read a 3-byte little-endian long */
        मूल्य = FT_अग्र_UOFF3_LE( cursor );
        sign_shift = 8;
        अवरोध;

      शेष:
        /* otherwise, exit the loop */
        stream->cursor = cursor;
        जाओ Exit;
      }

      /* now, compute the signed value if necessary */
      अगर ( fields->मूल्य & FT_FRAME_OP_SIGNED )
        मूल्य = (FT_ULong)( (FT_Int32)( मूल्य << sign_shift ) >> sign_shift );

      /* finally, store the value in the object */

      p = (FT_Byte*)structure + fields->offset;
      चयन ( fields->आकार )
      {
      हाल ( 8 / FT_CHAR_BIT ):
        *(FT_Byte*)p = (FT_Byte)मूल्य;
        अवरोध;

      हाल ( 16 / FT_CHAR_BIT ):
        *(FT_UShort*)p = (FT_UShort)मूल्य;
        अवरोध;

      हाल ( 32 / FT_CHAR_BIT ):
        *(FT_UInt32*)p = (FT_UInt32)मूल्य;
        अवरोध;

      शेष:  /* for 64-bit systems */
        *(FT_ULong*)p = (FT_ULong)मूल्य;
      }

      /* go to next field */
      fields++;
    }
    जबतक ( 1 );

  Exit:
    /* close the frame if it was opened by this read */
    अगर ( frame_accessed )
      FT_Stream_ExitFrame( stream );

    वापस error;
  }


/* END */
