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
#وصف_رد  FT_COMPONENT
#وصف_ڪريو FT_COMPONENT  وهڪرو


  FT_بنياد_DEF( فراغ )
  FT_وهڪرو_Open_يادگيري( FT_وهڪرو       وهڪرو,
                        ثابت FT_Byte*  بنياد,
                        FT_ULong        سائيز )
  {
    وهڪرو->بنياد   = (FT_Byte*) بنياد;
    وهڪرو->سائيز   = سائيز;
    وهڪرو->pos    = 0;
    وهڪرو->cursor = عدم;
    وهڪرو->اقرأ   = عدم;
    وهڪرو->close  = عدم;
  }


  FT_بنياد_DEF( فراغ )
  FT_وهڪرو_Close( FT_وهڪرو  وهڪرو )
  {
    إذا ( وهڪرو && وهڪرو->close )
      وهڪرو->close( وهڪرو );
  }


  FT_بنياد_DEF( FT_Error )
  FT_وهڪرو_Seek( FT_وهڪرو  وهڪرو,
                  FT_ULong   pos )
  {
    FT_Error  error = FT_Err_Ok;


    إذا ( وهڪرو->اقرأ )
    {
      إذا ( وهڪرو->اقرأ( وهڪرو, pos, عدم, 0 ) )
      {
        FT_ERROR(( "FT_Stream_Seek:"
                   " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
                   pos, وهڪرو->سائيز ));

        error = FT_THROW( Invalid_وهڪرو_Operation );
      }
    }
    /* note that seeking to the first position after the file is valid */
    وإلا إذا ( pos > وهڪرو->سائيز )
    {
      FT_ERROR(( "FT_Stream_Seek:"
                 " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
                 pos, وهڪرو->سائيز ));

      error = FT_THROW( Invalid_وهڪرو_Operation );
    }

    إذا ( !error )
      وهڪرو->pos = pos;

    أرجع error;
  }


  FT_بنياد_DEF( FT_Error )
  FT_وهڪرو_Skip( FT_وهڪرو  وهڪرو,
                  FT_Long    مفاصلو )
  {
    إذا ( مفاصلو < 0 )
      أرجع FT_THROW( Invalid_وهڪرو_Operation );

    أرجع FT_وهڪرو_Seek( وهڪرو, وهڪرو->pos + (FT_ULong)مفاصلو );
  }


  FT_بنياد_DEF( FT_ULong )
  FT_وهڪرو_Pos( FT_وهڪرو  وهڪرو )
  {
    أرجع وهڪرو->pos;
  }


  FT_بنياد_DEF( FT_Error )
  FT_وهڪرو_Read( FT_وهڪرو  وهڪرو,
                  FT_Byte*   بفر,
                  FT_ULong   ڳڻپ )
  {
    أرجع FT_وهڪرو_Read_At( وهڪرو, وهڪرو->pos, بفر, ڳڻپ );
  }


  FT_بنياد_DEF( FT_Error )
  FT_وهڪرو_Read_At( FT_وهڪرو  وهڪرو,
                    FT_ULong   pos,
                    FT_Byte*   بفر,
                    FT_ULong   ڳڻپ )
  {
    FT_Error  error = FT_Err_Ok;
    FT_ULong  read_بائيٽون;


    إذا ( pos >= وهڪرو->سائيز )
    {
      FT_ERROR(( "FT_Stream_ReadAt:"
                 " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
                 pos, وهڪرو->سائيز ));

      أرجع FT_THROW( Invalid_وهڪرو_Operation );
    }

    إذا ( وهڪرو->اقرأ )
      read_بائيٽون = وهڪرو->اقرأ( وهڪرو, pos, بفر, ڳڻپ );
    وإلا
    {
      read_بائيٽون = وهڪرو->سائيز - pos;
      إذا ( read_بائيٽون > ڳڻپ )
        read_بائيٽون = ڳڻپ;

      /* Allow "reading" zero bytes without UB even if buffer is NULL */
      إذا ( ڳڻپ )
        FT_MEM_COPY( بفر, وهڪرو->بنياد + pos, read_بائيٽون );
    }

    وهڪرو->pos = pos + read_بائيٽون;

    إذا ( read_بائيٽون < ڳڻپ )
    {
      FT_ERROR(( "FT_Stream_ReadAt:"
                 " invalid read; expected %lu bytes, got %lu\n",
                 ڳڻپ, read_بائيٽون ));

      error = FT_THROW( Invalid_وهڪرو_Operation );
    }

    أرجع error;
  }


  FT_بنياد_DEF( FT_ULong )
  FT_وهڪرو_Try_Read( FT_وهڪرو  وهڪرو,
                     FT_Byte*   بفر,
                     FT_ULong   ڳڻپ )
  {
    FT_ULong  read_بائيٽون = 0;


    إذا ( وهڪرو->pos >= وهڪرو->سائيز )
      اذهب Exit;

    إذا ( وهڪرو->اقرأ )
      read_بائيٽون = وهڪرو->اقرأ( وهڪرو, وهڪرو->pos, بفر, ڳڻپ );
    وإلا
    {
      read_بائيٽون = وهڪرو->سائيز - وهڪرو->pos;
      إذا ( read_بائيٽون > ڳڻپ )
        read_بائيٽون = ڳڻپ;

      /* Allow "reading" zero bytes without UB even if buffer is NULL */
      إذا ( ڳڻپ )
        FT_MEM_COPY( بفر, وهڪرو->بنياد + وهڪرو->pos, read_بائيٽون );
    }

    وهڪرو->pos += read_بائيٽون;

  Exit:
    أرجع read_بائيٽون;
  }


  FT_بنياد_DEF( FT_Error )
  FT_وهڪرو_ڪڍو_چوکٺو( FT_وهڪرو  وهڪرو,
                          FT_ULong   ڳڻپ,
                          FT_Byte**  pbytes )
  {
    FT_Error  error;


    error = FT_وهڪرو_Enter_چوکٺو( وهڪرو, ڳڻپ );
    إذا ( !error )
    {
      *pbytes = (FT_Byte*)وهڪرو->cursor;

      /* equivalent to FT_Stream_ExitFrame(), with no memory block release */
      وهڪرو->cursor = عدم;
      وهڪرو->حد_بندي  = عدم;
    }

    أرجع error;
  }


  FT_بنياد_DEF( فراغ )
  FT_وهڪرو_Release_چوکٺو( FT_وهڪرو  وهڪرو,
                          FT_Byte**  pbytes )
  {
    إذا ( وهڪرو && وهڪرو->اقرأ )
    {
      FT_يادگيري  يادگيري = وهڪرو->يادگيري;


#جيڪڏهن_وصف_ٿيل FT_DEBUG_يادگيري
      ft_mem_free( يادگيري, *pbytes );
#وإلا
      FT_FREE( *pbytes );
#جيڪڏهن_پورو
    }

    *pbytes = عدم;
  }


  FT_بنياد_DEF( FT_Error )
  FT_وهڪرو_Enter_چوکٺو( FT_وهڪرو  وهڪرو,
                        FT_ULong   ڳڻپ )
  {
    FT_Error  error = FT_Err_Ok;
    FT_ULong  read_بائيٽون;


    FT_TRACE7(( "FT_Stream_EnterFrame: %lu bytes\n", ڳڻپ ));

    /* check for nested frame access */
    FT_ASSERT( وهڪرو && وهڪرو->cursor == 0 );

    إذا ( وهڪرو->اقرأ )
    {
      /* allocate the frame in memory */
      FT_يادگيري  يادگيري = وهڪرو->يادگيري;


      /* simple sanity check */
      إذا ( ڳڻپ > وهڪرو->سائيز )
      {
        FT_ERROR(( "FT_Stream_EnterFrame:"
                   " frame size (%lu) larger than stream size (%lu)\n",
                   ڳڻپ, وهڪرو->سائيز ));

        error = FT_THROW( Invalid_وهڪرو_Operation );
        اذهب Exit;
      }

#جيڪڏهن_وصف_ٿيل FT_DEBUG_يادگيري
      /* assume `ft_debug_file_` and `ft_debug_lineno_` are already set */
      وهڪرو->بنياد = (بنانشان حرف*)ft_mem_qalloc( يادگيري,
                                                    (FT_Long)ڳڻپ,
                                                    &error );
      إذا ( error )
        اذهب Exit;
#وإلا
      إذا ( FT_QALLOC( وهڪرو->بنياد, ڳڻپ ) )
        اذهب Exit;
#جيڪڏهن_پورو
      /* read it */
      read_بائيٽون = وهڪرو->اقرأ( وهڪرو, وهڪرو->pos,
                                 وهڪرو->بنياد, ڳڻپ );
      إذا ( read_بائيٽون < ڳڻپ )
      {
        FT_ERROR(( "FT_Stream_EnterFrame:"
                   " invalid read; expected %lu bytes, got %lu\n",
                   ڳڻپ, read_بائيٽون ));

        FT_FREE( وهڪرو->بنياد );
        error = FT_THROW( Invalid_وهڪرو_Operation );
      }

      وهڪرو->cursor = وهڪرو->بنياد;
      وهڪرو->حد_بندي  = FT_وٿي( وهڪرو->cursor, ڳڻپ );
      وهڪرو->pos   += read_بائيٽون;
    }
    وإلا
    {
      /* check current and new position */
      إذا ( وهڪرو->pos >= وهڪرو->سائيز        ||
           وهڪرو->سائيز - وهڪرو->pos < ڳڻپ )
      {
        FT_ERROR(( "FT_Stream_EnterFrame:"
                   " invalid i/o; pos = 0x%lx, count = %lu, size = 0x%lx\n",
                   وهڪرو->pos, ڳڻپ, وهڪرو->سائيز ));

        error = FT_THROW( Invalid_وهڪرو_Operation );
        اذهب Exit;
      }

      /* set cursor */
      وهڪرو->cursor = وهڪرو->بنياد + وهڪرو->pos;
      وهڪرو->حد_بندي  = وهڪرو->cursor + ڳڻپ;
      وهڪرو->pos   += ڳڻپ;
    }

  Exit:
    أرجع error;
  }


  FT_بنياد_DEF( فراغ )
  FT_وهڪرو_Exit_چوکٺو( FT_وهڪرو  وهڪرو )
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

    FT_ASSERT( وهڪرو );

    إذا ( وهڪرو->اقرأ )
    {
      FT_يادگيري  يادگيري = وهڪرو->يادگيري;


#جيڪڏهن_وصف_ٿيل FT_DEBUG_يادگيري
      ft_mem_free( يادگيري, وهڪرو->بنياد );
      وهڪرو->بنياد = عدم;
#وإلا
      FT_FREE( وهڪرو->بنياد );
#جيڪڏهن_پورو
    }

    وهڪرو->cursor = عدم;
    وهڪرو->حد_بندي  = عدم;
  }


  FT_بنياد_DEF( FT_Byte )
  FT_وهڪرو_Get_Byte( FT_وهڪرو  وهڪرو )
  {
    FT_Byte  نتيجو;


    FT_ASSERT( وهڪرو && وهڪرو->cursor );

    نتيجو = 0;
    إذا ( وهڪرو->cursor < وهڪرو->حد_بندي )
      نتيجو = *وهڪرو->cursor++;

    أرجع نتيجو;
  }


  FT_بنياد_DEF( FT_UInt16 )
  FT_وهڪرو_Get_UShort( FT_وهڪرو  وهڪرو )
  {
    FT_Byte*   p;
    FT_UInt16  نتيجو;


    FT_ASSERT( وهڪرو && وهڪرو->cursor );

    نتيجو         = 0;
    p              = وهڪرو->cursor;
    إذا ( p + 1 < وهڪرو->حد_بندي )
      نتيجو       = FT_ايندڙ_USHORT( p );
    وهڪرو->cursor = p;

    أرجع نتيجو;
  }


  FT_بنياد_DEF( FT_UInt16 )
  FT_وهڪرو_Get_UShort_LE( FT_وهڪرو  وهڪرو )
  {
    FT_Byte*   p;
    FT_UInt16  نتيجو;


    FT_ASSERT( وهڪرو && وهڪرو->cursor );

    نتيجو         = 0;
    p              = وهڪرو->cursor;
    إذا ( p + 1 < وهڪرو->حد_بندي )
      نتيجو       = FT_ايندڙ_USHORT_LE( p );
    وهڪرو->cursor = p;

    أرجع نتيجو;
  }


  FT_بنياد_DEF( FT_UInt32 )
  FT_وهڪرو_Get_UOffset( FT_وهڪرو  وهڪرو )
  {
    FT_Byte*  p;
    FT_UInt32 نتيجو;


    FT_ASSERT( وهڪرو && وهڪرو->cursor );

    نتيجو         = 0;
    p              = وهڪرو->cursor;
    إذا ( p + 2 < وهڪرو->حد_بندي )
      نتيجو       = FT_ايندڙ_UOFF3( p );
    وهڪرو->cursor = p;
    أرجع نتيجو;
  }


  FT_بنياد_DEF( FT_UInt32 )
  FT_وهڪرو_Get_ULong( FT_وهڪرو  وهڪرو )
  {
    FT_Byte*  p;
    FT_UInt32 نتيجو;


    FT_ASSERT( وهڪرو && وهڪرو->cursor );

    نتيجو         = 0;
    p              = وهڪرو->cursor;
    إذا ( p + 3 < وهڪرو->حد_بندي )
      نتيجو       = FT_ايندڙ_ULONG( p );
    وهڪرو->cursor = p;
    أرجع نتيجو;
  }


  FT_بنياد_DEF( FT_UInt32 )
  FT_وهڪرو_Get_ULong_LE( FT_وهڪرو  وهڪرو )
  {
    FT_Byte*  p;
    FT_UInt32 نتيجو;


    FT_ASSERT( وهڪرو && وهڪرو->cursor );

    نتيجو         = 0;
    p              = وهڪرو->cursor;
    إذا ( p + 3 < وهڪرو->حد_بندي )
      نتيجو       = FT_ايندڙ_ULONG_LE( p );
    وهڪرو->cursor = p;
    أرجع نتيجو;
  }


  FT_بنياد_DEF( FT_Byte )
  FT_وهڪرو_Read_Byte( FT_وهڪرو  وهڪرو,
                      FT_Error*  error )
  {
    FT_Byte  نتيجو = 0;


    FT_ASSERT( وهڪرو );

    إذا ( وهڪرو->pos < وهڪرو->سائيز )
    {
      إذا ( وهڪرو->اقرأ )
      {
        إذا ( وهڪرو->اقرأ( وهڪرو, وهڪرو->pos, &نتيجو, 1L ) != 1L )
          اذهب Fail;
      }
      وإلا
        نتيجو = وهڪرو->بنياد[وهڪرو->pos];
    }
    وإلا
      اذهب Fail;

    وهڪرو->pos++;

    *error = FT_Err_Ok;

    أرجع نتيجو;

  Fail:
    *error = FT_THROW( Invalid_وهڪرو_Operation );
    FT_ERROR(( "FT_Stream_ReadByte:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               وهڪرو->pos, وهڪرو->سائيز ));

    أرجع نتيجو;
  }


  FT_بنياد_DEF( FT_UInt16 )
  FT_وهڪرو_Read_UShort( FT_وهڪرو  وهڪرو,
                        FT_Error*  error )
  {
    FT_Byte    reads[2];
    FT_Byte*   p;
    FT_UInt16  نتيجو = 0;


    FT_ASSERT( وهڪرو );

    إذا ( وهڪرو->pos + 1 < وهڪرو->سائيز )
    {
      إذا ( وهڪرو->اقرأ )
      {
        إذا ( وهڪرو->اقرأ( وهڪرو, وهڪرو->pos, reads, 2L ) != 2L )
          اذهب Fail;

        p = reads;
      }
      وإلا
        p = وهڪرو->بنياد + وهڪرو->pos;

      إذا ( p )
        نتيجو = FT_ايندڙ_USHORT( p );
    }
    وإلا
      اذهب Fail;

    وهڪرو->pos += 2;

    *error = FT_Err_Ok;

    أرجع نتيجو;

  Fail:
    *error = FT_THROW( Invalid_وهڪرو_Operation );
    FT_ERROR(( "FT_Stream_ReadUShort:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               وهڪرو->pos, وهڪرو->سائيز ));

    أرجع نتيجو;
  }


  FT_بنياد_DEF( FT_UInt16 )
  FT_وهڪرو_Read_UShort_LE( FT_وهڪرو  وهڪرو,
                          FT_Error*  error )
  {
    FT_Byte    reads[2];
    FT_Byte*   p;
    FT_UInt16  نتيجو = 0;


    FT_ASSERT( وهڪرو );

    إذا ( وهڪرو->pos + 1 < وهڪرو->سائيز )
    {
      إذا ( وهڪرو->اقرأ )
      {
        إذا ( وهڪرو->اقرأ( وهڪرو, وهڪرو->pos, reads, 2L ) != 2L )
          اذهب Fail;

        p = reads;
      }
      وإلا
        p = وهڪرو->بنياد + وهڪرو->pos;

      إذا ( p )
        نتيجو = FT_ايندڙ_USHORT_LE( p );
    }
    وإلا
      اذهب Fail;

    وهڪرو->pos += 2;

    *error = FT_Err_Ok;

    أرجع نتيجو;

  Fail:
    *error = FT_THROW( Invalid_وهڪرو_Operation );
    FT_ERROR(( "FT_Stream_ReadUShortLE:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               وهڪرو->pos, وهڪرو->سائيز ));

    أرجع نتيجو;
  }


  FT_بنياد_DEF( FT_ULong )
  FT_وهڪرو_Read_UOffset( FT_وهڪرو  وهڪرو,
                         FT_Error*  error )
  {
    FT_Byte   reads[3];
    FT_Byte*  p;
    FT_ULong  نتيجو = 0;


    FT_ASSERT( وهڪرو );

    إذا ( وهڪرو->pos + 2 < وهڪرو->سائيز )
    {
      إذا ( وهڪرو->اقرأ )
      {
        إذا (وهڪرو->اقرأ( وهڪرو, وهڪرو->pos, reads, 3L ) != 3L )
          اذهب Fail;

        p = reads;
      }
      وإلا
        p = وهڪرو->بنياد + وهڪرو->pos;

      إذا ( p )
        نتيجو = FT_ايندڙ_UOFF3( p );
    }
    وإلا
      اذهب Fail;

    وهڪرو->pos += 3;

    *error = FT_Err_Ok;

    أرجع نتيجو;

  Fail:
    *error = FT_THROW( Invalid_وهڪرو_Operation );
    FT_ERROR(( "FT_Stream_ReadUOffset:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               وهڪرو->pos, وهڪرو->سائيز ));

    أرجع نتيجو;
  }


  FT_بنياد_DEF( FT_UInt32 )
  FT_وهڪرو_Read_ULong( FT_وهڪرو  وهڪرو,
                       FT_Error*  error )
  {
    FT_Byte   reads[4];
    FT_Byte*  p;
    FT_UInt32 نتيجو = 0;


    FT_ASSERT( وهڪرو );

    إذا ( وهڪرو->pos + 3 < وهڪرو->سائيز )
    {
      إذا ( وهڪرو->اقرأ )
      {
        إذا ( وهڪرو->اقرأ( وهڪرو, وهڪرو->pos, reads, 4L ) != 4L )
          اذهب Fail;

        p = reads;
      }
      وإلا
        p = وهڪرو->بنياد + وهڪرو->pos;

      إذا ( p )
        نتيجو = FT_ايندڙ_ULONG( p );
    }
    وإلا
      اذهب Fail;

    وهڪرو->pos += 4;

    *error = FT_Err_Ok;

    أرجع نتيجو;

  Fail:
    *error = FT_THROW( Invalid_وهڪرو_Operation );
    FT_ERROR(( "FT_Stream_ReadULong:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               وهڪرو->pos, وهڪرو->سائيز ));

    أرجع نتيجو;
  }


  FT_بنياد_DEF( FT_UInt32 )
  FT_وهڪرو_Read_ULong_LE( FT_وهڪرو  وهڪرو,
                         FT_Error*  error )
  {
    FT_Byte   reads[4];
    FT_Byte*  p;
    FT_UInt32 نتيجو = 0;


    FT_ASSERT( وهڪرو );

    إذا ( وهڪرو->pos + 3 < وهڪرو->سائيز )
    {
      إذا ( وهڪرو->اقرأ )
      {
        إذا ( وهڪرو->اقرأ( وهڪرو, وهڪرو->pos, reads, 4L ) != 4L )
          اذهب Fail;

        p = reads;
      }
      وإلا
        p = وهڪرو->بنياد + وهڪرو->pos;

      إذا ( p )
        نتيجو = FT_ايندڙ_ULONG_LE( p );
    }
    وإلا
      اذهب Fail;

    وهڪرو->pos += 4;

    *error = FT_Err_Ok;

    أرجع نتيجو;

  Fail:
    *error = FT_THROW( Invalid_وهڪرو_Operation );
    FT_ERROR(( "FT_Stream_ReadULongLE:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               وهڪرو->pos, وهڪرو->سائيز ));

    أرجع نتيجو;
  }


  FT_بنياد_DEF( FT_Error )
  FT_وهڪرو_Read_Fields( FT_وهڪرو              وهڪرو,
                        ثابت FT_چوکٺو_ميدان*  fields,
                        فراغ*                  structure )
  {
    FT_Error  error;
    FT_Bool   چوکٺو_accessed = 0;
    FT_Byte*  cursor;


    إذا ( !fields )
      أرجع FT_THROW( Invalid_دليل );

    إذا ( !وهڪرو )
      أرجع FT_THROW( Invalid_وهڪرو_Handle );

    cursor = وهڪرو->cursor;

    error = FT_Err_Ok;
    افعل
    {
      FT_ULong  قدر;
      FT_Int    sign_سرڪايو;
      FT_Byte*  p;


      بدل ( fields->قدر )
      {
      حالة ft_چوکٺو_شروعات:  /* access a new frame */
        error = FT_وهڪرو_Enter_چوکٺو( وهڪرو, fields->وٿي );
        إذا ( error )
          اذهب Exit;

        چوکٺو_accessed = 1;
        cursor         = وهڪرو->cursor;
        fields++;
        استمر;  /* loop! */

      حالة ft_چوکٺو_بائيٽون:  /* read a byte sequence */
      حالة ft_چوکٺو_skip:   /* skip some bytes      */
        {
          FT_وٿي_2  len = fields->سائيز;


          إذا ( len > (FT_وٿي_2)( وهڪرو->حد_بندي - cursor ) )
          {
            error = FT_THROW( Invalid_وهڪرو_Operation );
            اذهب Exit;
          }

          إذا ( fields->قدر == ft_چوکٺو_بائيٽون )
          {
            p = (FT_Byte*)structure + fields->وٿي;
            FT_MEM_COPY( p, cursor, len );
          }
          cursor += len;
          fields++;
          استمر;
        }

      حالة ft_چوکٺو_byte:
      حالة ft_چوکٺو_schar:  /* read a single byte */
        قدر = FT_ايندڙ_BYTE( cursor );
        sign_سرڪايو = 24;
        اكسر;

      حالة ft_چوکٺو_short_be:
      حالة ft_چوکٺو_ushort_be:  /* read a 2-byte big-endian short */
        قدر = FT_ايندڙ_USHORT( cursor );
        sign_سرڪايو = 16;
        اكسر;

      حالة ft_چوکٺو_short_le:
      حالة ft_چوکٺو_ushort_le:  /* read a 2-byte little-endian short */
        قدر = FT_ايندڙ_USHORT_LE( cursor );
        sign_سرڪايو = 16;
        اكسر;

      حالة ft_چوکٺو_long_be:
      حالة ft_چوکٺو_ulong_be:  /* read a 4-byte big-endian long */
        قدر = FT_ايندڙ_ULONG( cursor );
        sign_سرڪايو = 0;
        اكسر;

      حالة ft_چوکٺو_long_le:
      حالة ft_چوکٺو_ulong_le:  /* read a 4-byte little-endian long */
        قدر = FT_ايندڙ_ULONG_LE( cursor );
        sign_سرڪايو = 0;
        اكسر;

      حالة ft_چوکٺو_off3_be:
      حالة ft_چوکٺو_uoff3_be:  /* read a 3-byte big-endian long */
        قدر = FT_ايندڙ_UOFF3( cursor );
        sign_سرڪايو = 8;
        اكسر;

      حالة ft_چوکٺو_off3_le:
      حالة ft_چوکٺو_uoff3_le:  /* read a 3-byte little-endian long */
        قدر = FT_ايندڙ_UOFF3_LE( cursor );
        sign_سرڪايو = 8;
        اكسر;

      افتراضي:
        /* otherwise, exit the loop */
        وهڪرو->cursor = cursor;
        اذهب Exit;
      }

      /* now, compute the signed value if necessary */
      إذا ( fields->قدر & FT_چوکٺو_OP_SIGNED )
        قدر = (FT_ULong)( (FT_Int32)( قدر << sign_سرڪايو ) >> sign_سرڪايو );

      /* finally, store the value in the object */

      p = (FT_Byte*)structure + fields->وٿي;
      بدل ( fields->سائيز )
      {
      حالة ( 8 / FT_CHAR_ٻٽو_انگ ):
        *(FT_Byte*)p = (FT_Byte)قدر;
        اكسر;

      حالة ( 16 / FT_CHAR_ٻٽو_انگ ):
        *(FT_UShort*)p = (FT_UShort)قدر;
        اكسر;

      حالة ( 32 / FT_CHAR_ٻٽو_انگ ):
        *(FT_UInt32*)p = (FT_UInt32)قدر;
        اكسر;

      افتراضي:  /* for 64-bit systems */
        *(FT_ULong*)p = (FT_ULong)قدر;
      }

      /* go to next field */
      fields++;
    }
    طالما ( 1 );

  Exit:
    /* close the frame if it was opened by this read */
    إذا ( چوکٺو_accessed )
      FT_وهڪرو_Exit_چوکٺو( وهڪرو );

    أرجع error;
  }


/*پڇاڙي*/
