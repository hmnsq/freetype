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
#अपरिभाषय  FT_COMPONENT
#परिभाषय FT_COMPONENT  प्रवाहः


  FT_आधारः_DEF( शून्य )
  FT_प्रवाहः_Open_स्मृतिः( FT_प्रवाहः       प्रवाहः,
                        ध्रुव FT_Byte*  आधारः,
                        FT_ULong        आकार )
  {
    प्रवाहः->आधारः   = (FT_Byte*) आधारः;
    प्रवाहः->आकार   = आकार;
    प्रवाहः->pos    = 0;
    प्रवाहः->cursor = रिक्त;
    प्रवाहः->पठ   = रिक्त;
    प्रवाहः->close  = रिक्त;
  }


  FT_आधारः_DEF( शून्य )
  FT_प्रवाहः_Close( FT_प्रवाहः  प्रवाहः )
  {
    यदि ( प्रवाहः && प्रवाहः->close )
      प्रवाहः->close( प्रवाहः );
  }


  FT_आधारः_DEF( FT_Error )
  FT_प्रवाहः_Seek( FT_प्रवाहः  प्रवाहः,
                  FT_ULong   pos )
  {
    FT_Error  error = FT_Err_Ok;


    यदि ( प्रवाहः->पठ )
    {
      यदि ( प्रवाहः->पठ( प्रवाहः, pos, रिक्त, 0 ) )
      {
        FT_ERROR(( "FT_Stream_Seek:"
                   " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
                   pos, प्रवाहः->आकार ));

        error = FT_THROW( Invalid_प्रवाहः_Operation );
      }
    }
    /* note that seeking to the first position after the file is valid */
    अन्यथा यदि ( pos > प्रवाहः->आकार )
    {
      FT_ERROR(( "FT_Stream_Seek:"
                 " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
                 pos, प्रवाहः->आकार ));

      error = FT_THROW( Invalid_प्रवाहः_Operation );
    }

    यदि ( !error )
      प्रवाहः->pos = pos;

    प्रत्यागम error;
  }


  FT_आधारः_DEF( FT_Error )
  FT_प्रवाहः_Skip( FT_प्रवाहः  प्रवाहः,
                  FT_Long    distance )
  {
    यदि ( distance < 0 )
      प्रत्यागम FT_THROW( Invalid_प्रवाहः_Operation );

    प्रत्यागम FT_प्रवाहः_Seek( प्रवाहः, प्रवाहः->pos + (FT_ULong)distance );
  }


  FT_आधारः_DEF( FT_ULong )
  FT_प्रवाहः_Pos( FT_प्रवाहः  प्रवाहः )
  {
    प्रत्यागम प्रवाहः->pos;
  }


  FT_आधारः_DEF( FT_Error )
  FT_प्रवाहः_Read( FT_प्रवाहः  प्रवाहः,
                  FT_Byte*   बफर,
                  FT_ULong   गणनम् )
  {
    प्रत्यागम FT_प्रवाहः_Read_At( प्रवाहः, प्रवाहः->pos, बफर, गणनम् );
  }


  FT_आधारः_DEF( FT_Error )
  FT_प्रवाहः_Read_At( FT_प्रवाहः  प्रवाहः,
                    FT_ULong   pos,
                    FT_Byte*   बफर,
                    FT_ULong   गणनम् )
  {
    FT_Error  error = FT_Err_Ok;
    FT_ULong  read_अष्टकानि;


    यदि ( pos >= प्रवाहः->आकार )
    {
      FT_ERROR(( "FT_Stream_ReadAt:"
                 " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
                 pos, प्रवाहः->आकार ));

      प्रत्यागम FT_THROW( Invalid_प्रवाहः_Operation );
    }

    यदि ( प्रवाहः->पठ )
      read_अष्टकानि = प्रवाहः->पठ( प्रवाहः, pos, बफर, गणनम् );
    अन्यथा
    {
      read_अष्टकानि = प्रवाहः->आकार - pos;
      यदि ( read_अष्टकानि > गणनम् )
        read_अष्टकानि = गणनम्;

      /* Allow "reading" zero bytes without UB even if buffer is NULL */
      यदि ( गणनम् )
        FT_MEM_COPY( बफर, प्रवाहः->आधारः + pos, read_अष्टकानि );
    }

    प्रवाहः->pos = pos + read_अष्टकानि;

    यदि ( read_अष्टकानि < गणनम् )
    {
      FT_ERROR(( "FT_Stream_ReadAt:"
                 " invalid read; expected %lu bytes, got %lu\n",
                 गणनम्, read_अष्टकानि ));

      error = FT_THROW( Invalid_प्रवाहः_Operation );
    }

    प्रत्यागम error;
  }


  FT_आधारः_DEF( FT_ULong )
  FT_प्रवाहः_Try_Read( FT_प्रवाहः  प्रवाहः,
                     FT_Byte*   बफर,
                     FT_ULong   गणनम् )
  {
    FT_ULong  read_अष्टकानि = 0;


    यदि ( प्रवाहः->pos >= प्रवाहः->आकार )
      गच्छ Exit;

    यदि ( प्रवाहः->पठ )
      read_अष्टकानि = प्रवाहः->पठ( प्रवाहः, प्रवाहः->pos, बफर, गणनम् );
    अन्यथा
    {
      read_अष्टकानि = प्रवाहः->आकार - प्रवाहः->pos;
      यदि ( read_अष्टकानि > गणनम् )
        read_अष्टकानि = गणनम्;

      /* Allow "reading" zero bytes without UB even if buffer is NULL */
      यदि ( गणनम् )
        FT_MEM_COPY( बफर, प्रवाहः->आधारः + प्रवाहः->pos, read_अष्टकानि );
    }

    प्रवाहः->pos += read_अष्टकानि;

  Exit:
    प्रत्यागम read_अष्टकानि;
  }


  FT_आधारः_DEF( FT_Error )
  FT_प्रवाहः_Extract_Frame( FT_प्रवाहः  प्रवाहः,
                          FT_ULong   गणनम्,
                          FT_Byte**  pbytes )
  {
    FT_Error  error;


    error = FT_प्रवाहः_Enter_Frame( प्रवाहः, गणनम् );
    यदि ( !error )
    {
      *pbytes = (FT_Byte*)प्रवाहः->cursor;

      /* equivalent to FT_Stream_ExitFrame(), with no memory block release */
      प्रवाहः->cursor = रिक्त;
      प्रवाहः->सीमा  = रिक्त;
    }

    प्रत्यागम error;
  }


  FT_आधारः_DEF( शून्य )
  FT_प्रवाहः_Release_Frame( FT_प्रवाहः  प्रवाहः,
                          FT_Byte**  pbytes )
  {
    यदि ( प्रवाहः && प्रवाहः->पठ )
    {
      FT_स्मृतिः  स्मृतिः = प्रवाहः->स्मृतिः;


#यदि_परिभाषितम् FT_DEBUG_स्मृतिः
      ft_mem_free( स्मृतिः, *pbytes );
#अन्यथा
      FT_FREE( *pbytes );
#यदि_अन्तः
    }

    *pbytes = रिक्त;
  }


  FT_आधारः_DEF( FT_Error )
  FT_प्रवाहः_Enter_Frame( FT_प्रवाहः  प्रवाहः,
                        FT_ULong   गणनम् )
  {
    FT_Error  error = FT_Err_Ok;
    FT_ULong  read_अष्टकानि;


    FT_TRACE7(( "FT_Stream_EnterFrame: %lu bytes\n", गणनम् ));

    /* check for nested frame access */
    FT_ASSERT( प्रवाहः && प्रवाहः->cursor == 0 );

    यदि ( प्रवाहः->पठ )
    {
      /* allocate the frame in memory */
      FT_स्मृतिः  स्मृतिः = प्रवाहः->स्मृतिः;


      /* simple sanity check */
      यदि ( गणनम् > प्रवाहः->आकार )
      {
        FT_ERROR(( "FT_Stream_EnterFrame:"
                   " frame size (%lu) larger than stream size (%lu)\n",
                   गणनम्, प्रवाहः->आकार ));

        error = FT_THROW( Invalid_प्रवाहः_Operation );
        गच्छ Exit;
      }

#यदि_परिभाषितम् FT_DEBUG_स्मृतिः
      /* assume `ft_debug_file_` and `ft_debug_lineno_` are already set */
      प्रवाहः->आधारः = (अचिह्नित वर्ण*)ft_mem_qalloc( स्मृतिः,
                                                    (FT_Long)गणनम्,
                                                    &error );
      यदि ( error )
        गच्छ Exit;
#अन्यथा
      यदि ( FT_QALLOC( प्रवाहः->आधारः, गणनम् ) )
        गच्छ Exit;
#यदि_अन्तः
      /* read it */
      read_अष्टकानि = प्रवाहः->पठ( प्रवाहः, प्रवाहः->pos,
                                 प्रवाहः->आधारः, गणनम् );
      यदि ( read_अष्टकानि < गणनम् )
      {
        FT_ERROR(( "FT_Stream_EnterFrame:"
                   " invalid read; expected %lu bytes, got %lu\n",
                   गणनम्, read_अष्टकानि ));

        FT_FREE( प्रवाहः->आधारः );
        error = FT_THROW( Invalid_प्रवाहः_Operation );
      }

      प्रवाहः->cursor = प्रवाहः->आधारः;
      प्रवाहः->सीमा  = FT_विस्थापनम्( प्रवाहः->cursor, गणनम् );
      प्रवाहः->pos   += read_अष्टकानि;
    }
    अन्यथा
    {
      /* check current and new position */
      यदि ( प्रवाहः->pos >= प्रवाहः->आकार        ||
           प्रवाहः->आकार - प्रवाहः->pos < गणनम् )
      {
        FT_ERROR(( "FT_Stream_EnterFrame:"
                   " invalid i/o; pos = 0x%lx, count = %lu, size = 0x%lx\n",
                   प्रवाहः->pos, गणनम्, प्रवाहः->आकार ));

        error = FT_THROW( Invalid_प्रवाहः_Operation );
        गच्छ Exit;
      }

      /* set cursor */
      प्रवाहः->cursor = प्रवाहः->आधारः + प्रवाहः->pos;
      प्रवाहः->सीमा  = प्रवाहः->cursor + गणनम्;
      प्रवाहः->pos   += गणनम्;
    }

  Exit:
    प्रत्यागम error;
  }


  FT_आधारः_DEF( शून्य )
  FT_प्रवाहः_Exit_Frame( FT_प्रवाहः  प्रवाहः )
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

    FT_ASSERT( प्रवाहः );

    यदि ( प्रवाहः->पठ )
    {
      FT_स्मृतिः  स्मृतिः = प्रवाहः->स्मृतिः;


#यदि_परिभाषितम् FT_DEBUG_स्मृतिः
      ft_mem_free( स्मृतिः, प्रवाहः->आधारः );
      प्रवाहः->आधारः = रिक्त;
#अन्यथा
      FT_FREE( प्रवाहः->आधारः );
#यदि_अन्तः
    }

    प्रवाहः->cursor = रिक्त;
    प्रवाहः->सीमा  = रिक्त;
  }


  FT_आधारः_DEF( FT_Byte )
  FT_प्रवाहः_Get_Byte( FT_प्रवाहः  प्रवाहः )
  {
    FT_Byte  परिणाम;


    FT_ASSERT( प्रवाहः && प्रवाहः->cursor );

    परिणाम = 0;
    यदि ( प्रवाहः->cursor < प्रवाहः->सीमा )
      परिणाम = *प्रवाहः->cursor++;

    प्रत्यागम परिणाम;
  }


  FT_आधारः_DEF( FT_UInt16 )
  FT_प्रवाहः_Get_UShort( FT_प्रवाहः  प्रवाहः )
  {
    FT_Byte*   p;
    FT_UInt16  परिणाम;


    FT_ASSERT( प्रवाहः && प्रवाहः->cursor );

    परिणाम         = 0;
    p              = प्रवाहः->cursor;
    यदि ( p + 1 < प्रवाहः->सीमा )
      परिणाम       = FT_अग्र_USHORT( p );
    प्रवाहः->cursor = p;

    प्रत्यागम परिणाम;
  }


  FT_आधारः_DEF( FT_UInt16 )
  FT_प्रवाहः_Get_UShort_LE( FT_प्रवाहः  प्रवाहः )
  {
    FT_Byte*   p;
    FT_UInt16  परिणाम;


    FT_ASSERT( प्रवाहः && प्रवाहः->cursor );

    परिणाम         = 0;
    p              = प्रवाहः->cursor;
    यदि ( p + 1 < प्रवाहः->सीमा )
      परिणाम       = FT_अग्र_USHORT_LE( p );
    प्रवाहः->cursor = p;

    प्रत्यागम परिणाम;
  }


  FT_आधारः_DEF( FT_UInt32 )
  FT_प्रवाहः_Get_UOffset( FT_प्रवाहः  प्रवाहः )
  {
    FT_Byte*  p;
    FT_UInt32 परिणाम;


    FT_ASSERT( प्रवाहः && प्रवाहः->cursor );

    परिणाम         = 0;
    p              = प्रवाहः->cursor;
    यदि ( p + 2 < प्रवाहः->सीमा )
      परिणाम       = FT_अग्र_UOFF3( p );
    प्रवाहः->cursor = p;
    प्रत्यागम परिणाम;
  }


  FT_आधारः_DEF( FT_UInt32 )
  FT_प्रवाहः_Get_ULong( FT_प्रवाहः  प्रवाहः )
  {
    FT_Byte*  p;
    FT_UInt32 परिणाम;


    FT_ASSERT( प्रवाहः && प्रवाहः->cursor );

    परिणाम         = 0;
    p              = प्रवाहः->cursor;
    यदि ( p + 3 < प्रवाहः->सीमा )
      परिणाम       = FT_अग्र_ULONG( p );
    प्रवाहः->cursor = p;
    प्रत्यागम परिणाम;
  }


  FT_आधारः_DEF( FT_UInt32 )
  FT_प्रवाहः_Get_ULong_LE( FT_प्रवाहः  प्रवाहः )
  {
    FT_Byte*  p;
    FT_UInt32 परिणाम;


    FT_ASSERT( प्रवाहः && प्रवाहः->cursor );

    परिणाम         = 0;
    p              = प्रवाहः->cursor;
    यदि ( p + 3 < प्रवाहः->सीमा )
      परिणाम       = FT_अग्र_ULONG_LE( p );
    प्रवाहः->cursor = p;
    प्रत्यागम परिणाम;
  }


  FT_आधारः_DEF( FT_Byte )
  FT_प्रवाहः_Read_Byte( FT_प्रवाहः  प्रवाहः,
                      FT_Error*  error )
  {
    FT_Byte  परिणाम = 0;


    FT_ASSERT( प्रवाहः );

    यदि ( प्रवाहः->pos < प्रवाहः->आकार )
    {
      यदि ( प्रवाहः->पठ )
      {
        यदि ( प्रवाहः->पठ( प्रवाहः, प्रवाहः->pos, &परिणाम, 1L ) != 1L )
          गच्छ Fail;
      }
      अन्यथा
        परिणाम = प्रवाहः->आधारः[प्रवाहः->pos];
    }
    अन्यथा
      गच्छ Fail;

    प्रवाहः->pos++;

    *error = FT_Err_Ok;

    प्रत्यागम परिणाम;

  Fail:
    *error = FT_THROW( Invalid_प्रवाहः_Operation );
    FT_ERROR(( "FT_Stream_ReadByte:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               प्रवाहः->pos, प्रवाहः->आकार ));

    प्रत्यागम परिणाम;
  }


  FT_आधारः_DEF( FT_UInt16 )
  FT_प्रवाहः_Read_UShort( FT_प्रवाहः  प्रवाहः,
                        FT_Error*  error )
  {
    FT_Byte    reads[2];
    FT_Byte*   p;
    FT_UInt16  परिणाम = 0;


    FT_ASSERT( प्रवाहः );

    यदि ( प्रवाहः->pos + 1 < प्रवाहः->आकार )
    {
      यदि ( प्रवाहः->पठ )
      {
        यदि ( प्रवाहः->पठ( प्रवाहः, प्रवाहः->pos, reads, 2L ) != 2L )
          गच्छ Fail;

        p = reads;
      }
      अन्यथा
        p = प्रवाहः->आधारः + प्रवाहः->pos;

      यदि ( p )
        परिणाम = FT_अग्र_USHORT( p );
    }
    अन्यथा
      गच्छ Fail;

    प्रवाहः->pos += 2;

    *error = FT_Err_Ok;

    प्रत्यागम परिणाम;

  Fail:
    *error = FT_THROW( Invalid_प्रवाहः_Operation );
    FT_ERROR(( "FT_Stream_ReadUShort:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               प्रवाहः->pos, प्रवाहः->आकार ));

    प्रत्यागम परिणाम;
  }


  FT_आधारः_DEF( FT_UInt16 )
  FT_प्रवाहः_Read_UShort_LE( FT_प्रवाहः  प्रवाहः,
                          FT_Error*  error )
  {
    FT_Byte    reads[2];
    FT_Byte*   p;
    FT_UInt16  परिणाम = 0;


    FT_ASSERT( प्रवाहः );

    यदि ( प्रवाहः->pos + 1 < प्रवाहः->आकार )
    {
      यदि ( प्रवाहः->पठ )
      {
        यदि ( प्रवाहः->पठ( प्रवाहः, प्रवाहः->pos, reads, 2L ) != 2L )
          गच्छ Fail;

        p = reads;
      }
      अन्यथा
        p = प्रवाहः->आधारः + प्रवाहः->pos;

      यदि ( p )
        परिणाम = FT_अग्र_USHORT_LE( p );
    }
    अन्यथा
      गच्छ Fail;

    प्रवाहः->pos += 2;

    *error = FT_Err_Ok;

    प्रत्यागम परिणाम;

  Fail:
    *error = FT_THROW( Invalid_प्रवाहः_Operation );
    FT_ERROR(( "FT_Stream_ReadUShortLE:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               प्रवाहः->pos, प्रवाहः->आकार ));

    प्रत्यागम परिणाम;
  }


  FT_आधारः_DEF( FT_ULong )
  FT_प्रवाहः_Read_UOffset( FT_प्रवाहः  प्रवाहः,
                         FT_Error*  error )
  {
    FT_Byte   reads[3];
    FT_Byte*  p;
    FT_ULong  परिणाम = 0;


    FT_ASSERT( प्रवाहः );

    यदि ( प्रवाहः->pos + 2 < प्रवाहः->आकार )
    {
      यदि ( प्रवाहः->पठ )
      {
        यदि (प्रवाहः->पठ( प्रवाहः, प्रवाहः->pos, reads, 3L ) != 3L )
          गच्छ Fail;

        p = reads;
      }
      अन्यथा
        p = प्रवाहः->आधारः + प्रवाहः->pos;

      यदि ( p )
        परिणाम = FT_अग्र_UOFF3( p );
    }
    अन्यथा
      गच्छ Fail;

    प्रवाहः->pos += 3;

    *error = FT_Err_Ok;

    प्रत्यागम परिणाम;

  Fail:
    *error = FT_THROW( Invalid_प्रवाहः_Operation );
    FT_ERROR(( "FT_Stream_ReadUOffset:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               प्रवाहः->pos, प्रवाहः->आकार ));

    प्रत्यागम परिणाम;
  }


  FT_आधारः_DEF( FT_UInt32 )
  FT_प्रवाहः_Read_ULong( FT_प्रवाहः  प्रवाहः,
                       FT_Error*  error )
  {
    FT_Byte   reads[4];
    FT_Byte*  p;
    FT_UInt32 परिणाम = 0;


    FT_ASSERT( प्रवाहः );

    यदि ( प्रवाहः->pos + 3 < प्रवाहः->आकार )
    {
      यदि ( प्रवाहः->पठ )
      {
        यदि ( प्रवाहः->पठ( प्रवाहः, प्रवाहः->pos, reads, 4L ) != 4L )
          गच्छ Fail;

        p = reads;
      }
      अन्यथा
        p = प्रवाहः->आधारः + प्रवाहः->pos;

      यदि ( p )
        परिणाम = FT_अग्र_ULONG( p );
    }
    अन्यथा
      गच्छ Fail;

    प्रवाहः->pos += 4;

    *error = FT_Err_Ok;

    प्रत्यागम परिणाम;

  Fail:
    *error = FT_THROW( Invalid_प्रवाहः_Operation );
    FT_ERROR(( "FT_Stream_ReadULong:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               प्रवाहः->pos, प्रवाहः->आकार ));

    प्रत्यागम परिणाम;
  }


  FT_आधारः_DEF( FT_UInt32 )
  FT_प्रवाहः_Read_ULong_LE( FT_प्रवाहः  प्रवाहः,
                         FT_Error*  error )
  {
    FT_Byte   reads[4];
    FT_Byte*  p;
    FT_UInt32 परिणाम = 0;


    FT_ASSERT( प्रवाहः );

    यदि ( प्रवाहः->pos + 3 < प्रवाहः->आकार )
    {
      यदि ( प्रवाहः->पठ )
      {
        यदि ( प्रवाहः->पठ( प्रवाहः, प्रवाहः->pos, reads, 4L ) != 4L )
          गच्छ Fail;

        p = reads;
      }
      अन्यथा
        p = प्रवाहः->आधारः + प्रवाहः->pos;

      यदि ( p )
        परिणाम = FT_अग्र_ULONG_LE( p );
    }
    अन्यथा
      गच्छ Fail;

    प्रवाहः->pos += 4;

    *error = FT_Err_Ok;

    प्रत्यागम परिणाम;

  Fail:
    *error = FT_THROW( Invalid_प्रवाहः_Operation );
    FT_ERROR(( "FT_Stream_ReadULongLE:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               प्रवाहः->pos, प्रवाहः->आकार ));

    प्रत्यागम परिणाम;
  }


  FT_आधारः_DEF( FT_Error )
  FT_प्रवाहः_Read_Fields( FT_प्रवाहः              प्रवाहः,
                        ध्रुव FT_Frame_क्षेत्रम्*  fields,
                        शून्य*                  structure )
  {
    FT_Error  error;
    FT_Bool   frame_accessed = 0;
    FT_Byte*  cursor;


    यदि ( !fields )
      प्रत्यागम FT_THROW( Invalid_तर्कः );

    यदि ( !प्रवाहः )
      प्रत्यागम FT_THROW( Invalid_प्रवाहः_Handle );

    cursor = प्रवाहः->cursor;

    error = FT_Err_Ok;
    कुरु
    {
      FT_ULong  मानम्;
      FT_Int    sign_सरणम्;
      FT_Byte*  p;


      विकल्प ( fields->मानम् )
      {
      पक्ष ft_frame_start:  /* access a new frame */
        error = FT_प्रवाहः_Enter_Frame( प्रवाहः, fields->विस्थापनम् );
        यदि ( error )
          गच्छ Exit;

        frame_accessed = 1;
        cursor         = प्रवाहः->cursor;
        fields++;
        अनुवर्त;  /* loop! */

      पक्ष ft_frame_अष्टकानि:  /* read a byte sequence */
      पक्ष ft_frame_skip:   /* skip some bytes      */
        {
          FT_विस्थापनम्_2  len = fields->आकार;


          यदि ( len > (FT_विस्थापनम्_2)( प्रवाहः->सीमा - cursor ) )
          {
            error = FT_THROW( Invalid_प्रवाहः_Operation );
            गच्छ Exit;
          }

          यदि ( fields->मानम् == ft_frame_अष्टकानि )
          {
            p = (FT_Byte*)structure + fields->विस्थापनम्;
            FT_MEM_COPY( p, cursor, len );
          }
          cursor += len;
          fields++;
          अनुवर्त;
        }

      पक्ष ft_frame_byte:
      पक्ष ft_frame_schar:  /* read a single byte */
        मानम् = FT_अग्र_BYTE( cursor );
        sign_सरणम् = 24;
        भङ्ग;

      पक्ष ft_frame_short_be:
      पक्ष ft_frame_ushort_be:  /* read a 2-byte big-endian short */
        मानम् = FT_अग्र_USHORT( cursor );
        sign_सरणम् = 16;
        भङ्ग;

      पक्ष ft_frame_short_le:
      पक्ष ft_frame_ushort_le:  /* read a 2-byte little-endian short */
        मानम् = FT_अग्र_USHORT_LE( cursor );
        sign_सरणम् = 16;
        भङ्ग;

      पक्ष ft_frame_long_be:
      पक्ष ft_frame_ulong_be:  /* read a 4-byte big-endian long */
        मानम् = FT_अग्र_ULONG( cursor );
        sign_सरणम् = 0;
        भङ्ग;

      पक्ष ft_frame_long_le:
      पक्ष ft_frame_ulong_le:  /* read a 4-byte little-endian long */
        मानम् = FT_अग्र_ULONG_LE( cursor );
        sign_सरणम् = 0;
        भङ्ग;

      पक्ष ft_frame_off3_be:
      पक्ष ft_frame_uoff3_be:  /* read a 3-byte big-endian long */
        मानम् = FT_अग्र_UOFF3( cursor );
        sign_सरणम् = 8;
        भङ्ग;

      पक्ष ft_frame_off3_le:
      पक्ष ft_frame_uoff3_le:  /* read a 3-byte little-endian long */
        मानम् = FT_अग्र_UOFF3_LE( cursor );
        sign_सरणम् = 8;
        भङ्ग;

      स्वभाव:
        /* otherwise, exit the loop */
        प्रवाहः->cursor = cursor;
        गच्छ Exit;
      }

      /* now, compute the signed value if necessary */
      यदि ( fields->मानम् & FT_FRAME_OP_SIGNED )
        मानम् = (FT_ULong)( (FT_Int32)( मानम् << sign_सरणम् ) >> sign_सरणम् );

      /* finally, store the value in the object */

      p = (FT_Byte*)structure + fields->विस्थापनम्;
      विकल्प ( fields->आकार )
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
    यावत् ( 1 );

  Exit:
    /* close the frame if it was opened by this read */
    यदि ( frame_accessed )
      FT_प्रवाहः_Exit_Frame( प्रवाहः );

    प्रत्यागम error;
  }


/*अन्तः*/
