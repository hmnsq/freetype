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
#لغو_تعریف  FT_COMPONENT
#تعریف_کن FT_COMPONENT  جریان


  FT_پایه_DEF( فراغ )
  FT_جریان_Open_حافظه( FT_جریان       جریان,
                        ثابت FT_Byte*  پایه,
                        FT_ULong        اندازه )
  {
    جریان->پایه   = (FT_Byte*) پایه;
    جریان->اندازه   = اندازه;
    جریان->pos    = 0;
    جریان->cursor = عدم;
    جریان->اقرأ   = عدم;
    جریان->close  = عدم;
  }


  FT_پایه_DEF( فراغ )
  FT_جریان_Close( FT_جریان  جریان )
  {
    إذا ( جریان && جریان->close )
      جریان->close( جریان );
  }


  FT_پایه_DEF( FT_Error )
  FT_جریان_Seek( FT_جریان  جریان,
                  FT_ULong   pos )
  {
    FT_Error  error = FT_Err_Ok;


    إذا ( جریان->اقرأ )
    {
      إذا ( جریان->اقرأ( جریان, pos, عدم, 0 ) )
      {
        FT_ERROR(( "FT_Stream_Seek:"
                   " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
                   pos, جریان->اندازه ));

        error = FT_THROW( Invalid_جریان_Operation );
      }
    }
    /* note that seeking to the first position after the file is valid */
    وإلا إذا ( pos > جریان->اندازه )
    {
      FT_ERROR(( "FT_Stream_Seek:"
                 " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
                 pos, جریان->اندازه ));

      error = FT_THROW( Invalid_جریان_Operation );
    }

    إذا ( !error )
      جریان->pos = pos;

    أرجع error;
  }


  FT_پایه_DEF( FT_Error )
  FT_جریان_Skip( FT_جریان  جریان,
                  FT_Long    فاصله )
  {
    إذا ( فاصله < 0 )
      أرجع FT_THROW( Invalid_جریان_Operation );

    أرجع FT_جریان_Seek( جریان, جریان->pos + (FT_ULong)فاصله );
  }


  FT_پایه_DEF( FT_ULong )
  FT_جریان_Pos( FT_جریان  جریان )
  {
    أرجع جریان->pos;
  }


  FT_پایه_DEF( FT_Error )
  FT_جریان_Read( FT_جریان  جریان,
                  FT_Byte*   میانگیر,
                  FT_ULong   شمار )
  {
    أرجع FT_جریان_Read_At( جریان, جریان->pos, میانگیر, شمار );
  }


  FT_پایه_DEF( FT_Error )
  FT_جریان_Read_At( FT_جریان  جریان,
                    FT_ULong   pos,
                    FT_Byte*   میانگیر,
                    FT_ULong   شمار )
  {
    FT_Error  error = FT_Err_Ok;
    FT_ULong  read_بایتها;


    إذا ( pos >= جریان->اندازه )
    {
      FT_ERROR(( "FT_Stream_ReadAt:"
                 " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
                 pos, جریان->اندازه ));

      أرجع FT_THROW( Invalid_جریان_Operation );
    }

    إذا ( جریان->اقرأ )
      read_بایتها = جریان->اقرأ( جریان, pos, میانگیر, شمار );
    وإلا
    {
      read_بایتها = جریان->اندازه - pos;
      إذا ( read_بایتها > شمار )
        read_بایتها = شمار;

      /* Allow "reading" zero bytes without UB even if buffer is NULL */
      إذا ( شمار )
        FT_MEM_COPY( میانگیر, جریان->پایه + pos, read_بایتها );
    }

    جریان->pos = pos + read_بایتها;

    إذا ( read_بایتها < شمار )
    {
      FT_ERROR(( "FT_Stream_ReadAt:"
                 " invalid read; expected %lu bytes, got %lu\n",
                 شمار, read_بایتها ));

      error = FT_THROW( Invalid_جریان_Operation );
    }

    أرجع error;
  }


  FT_پایه_DEF( FT_ULong )
  FT_جریان_Try_Read( FT_جریان  جریان,
                     FT_Byte*   میانگیر,
                     FT_ULong   شمار )
  {
    FT_ULong  read_بایتها = 0;


    إذا ( جریان->pos >= جریان->اندازه )
      اذهب Exit;

    إذا ( جریان->اقرأ )
      read_بایتها = جریان->اقرأ( جریان, جریان->pos, میانگیر, شمار );
    وإلا
    {
      read_بایتها = جریان->اندازه - جریان->pos;
      إذا ( read_بایتها > شمار )
        read_بایتها = شمار;

      /* Allow "reading" zero bytes without UB even if buffer is NULL */
      إذا ( شمار )
        FT_MEM_COPY( میانگیر, جریان->پایه + جریان->pos, read_بایتها );
    }

    جریان->pos += read_بایتها;

  Exit:
    أرجع read_بایتها;
  }


  FT_پایه_DEF( FT_Error )
  FT_جریان_استخراج_قاب( FT_جریان  جریان,
                          FT_ULong   شمار,
                          FT_Byte**  pbytes )
  {
    FT_Error  error;


    error = FT_جریان_Enter_قاب( جریان, شمار );
    إذا ( !error )
    {
      *pbytes = (FT_Byte*)جریان->cursor;

      /* equivalent to FT_Stream_ExitFrame(), with no memory block release */
      جریان->cursor = عدم;
      جریان->حد  = عدم;
    }

    أرجع error;
  }


  FT_پایه_DEF( فراغ )
  FT_جریان_Release_قاب( FT_جریان  جریان,
                          FT_Byte**  pbytes )
  {
    إذا ( جریان && جریان->اقرأ )
    {
      FT_حافظه  حافظه = جریان->حافظه;


#اگر_تعریف_شده FT_DEBUG_حافظه
      ft_mem_free( حافظه, *pbytes );
#وإلا
      FT_FREE( *pbytes );
#پایان_اگر
    }

    *pbytes = عدم;
  }


  FT_پایه_DEF( FT_Error )
  FT_جریان_Enter_قاب( FT_جریان  جریان,
                        FT_ULong   شمار )
  {
    FT_Error  error = FT_Err_Ok;
    FT_ULong  read_بایتها;


    FT_TRACE7(( "FT_Stream_EnterFrame: %lu bytes\n", شمار ));

    /* check for nested frame access */
    FT_ASSERT( جریان && جریان->cursor == 0 );

    إذا ( جریان->اقرأ )
    {
      /* allocate the frame in memory */
      FT_حافظه  حافظه = جریان->حافظه;


      /* simple sanity check */
      إذا ( شمار > جریان->اندازه )
      {
        FT_ERROR(( "FT_Stream_EnterFrame:"
                   " frame size (%lu) larger than stream size (%lu)\n",
                   شمار, جریان->اندازه ));

        error = FT_THROW( Invalid_جریان_Operation );
        اذهب Exit;
      }

#اگر_تعریف_شده FT_DEBUG_حافظه
      /* assume `ft_debug_file_` and `ft_debug_lineno_` are already set */
      جریان->پایه = (بیعلامت حرف*)ft_mem_qalloc( حافظه,
                                                    (FT_Long)شمار,
                                                    &error );
      إذا ( error )
        اذهب Exit;
#وإلا
      إذا ( FT_QALLOC( جریان->پایه, شمار ) )
        اذهب Exit;
#پایان_اگر
      /* read it */
      read_بایتها = جریان->اقرأ( جریان, جریان->pos,
                                 جریان->پایه, شمار );
      إذا ( read_بایتها < شمار )
      {
        FT_ERROR(( "FT_Stream_EnterFrame:"
                   " invalid read; expected %lu bytes, got %lu\n",
                   شمار, read_بایتها ));

        FT_FREE( جریان->پایه );
        error = FT_THROW( Invalid_جریان_Operation );
      }

      جریان->cursor = جریان->پایه;
      جریان->حد  = FT_جابه_جایی( جریان->cursor, شمار );
      جریان->pos   += read_بایتها;
    }
    وإلا
    {
      /* check current and new position */
      إذا ( جریان->pos >= جریان->اندازه        ||
           جریان->اندازه - جریان->pos < شمار )
      {
        FT_ERROR(( "FT_Stream_EnterFrame:"
                   " invalid i/o; pos = 0x%lx, count = %lu, size = 0x%lx\n",
                   جریان->pos, شمار, جریان->اندازه ));

        error = FT_THROW( Invalid_جریان_Operation );
        اذهب Exit;
      }

      /* set cursor */
      جریان->cursor = جریان->پایه + جریان->pos;
      جریان->حد  = جریان->cursor + شمار;
      جریان->pos   += شمار;
    }

  Exit:
    أرجع error;
  }


  FT_پایه_DEF( فراغ )
  FT_جریان_Exit_قاب( FT_جریان  جریان )
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

    FT_ASSERT( جریان );

    إذا ( جریان->اقرأ )
    {
      FT_حافظه  حافظه = جریان->حافظه;


#اگر_تعریف_شده FT_DEBUG_حافظه
      ft_mem_free( حافظه, جریان->پایه );
      جریان->پایه = عدم;
#وإلا
      FT_FREE( جریان->پایه );
#پایان_اگر
    }

    جریان->cursor = عدم;
    جریان->حد  = عدم;
  }


  FT_پایه_DEF( FT_Byte )
  FT_جریان_Get_Byte( FT_جریان  جریان )
  {
    FT_Byte  نتیجه;


    FT_ASSERT( جریان && جریان->cursor );

    نتیجه = 0;
    إذا ( جریان->cursor < جریان->حد )
      نتیجه = *جریان->cursor++;

    أرجع نتیجه;
  }


  FT_پایه_DEF( FT_UInt16 )
  FT_جریان_Get_UShort( FT_جریان  جریان )
  {
    FT_Byte*   p;
    FT_UInt16  نتیجه;


    FT_ASSERT( جریان && جریان->cursor );

    نتیجه         = 0;
    p              = جریان->cursor;
    إذا ( p + 1 < جریان->حد )
      نتیجه       = FT_بعدی_USHORT( p );
    جریان->cursor = p;

    أرجع نتیجه;
  }


  FT_پایه_DEF( FT_UInt16 )
  FT_جریان_Get_UShort_LE( FT_جریان  جریان )
  {
    FT_Byte*   p;
    FT_UInt16  نتیجه;


    FT_ASSERT( جریان && جریان->cursor );

    نتیجه         = 0;
    p              = جریان->cursor;
    إذا ( p + 1 < جریان->حد )
      نتیجه       = FT_بعدی_USHORT_LE( p );
    جریان->cursor = p;

    أرجع نتیجه;
  }


  FT_پایه_DEF( FT_UInt32 )
  FT_جریان_Get_UOffset( FT_جریان  جریان )
  {
    FT_Byte*  p;
    FT_UInt32 نتیجه;


    FT_ASSERT( جریان && جریان->cursor );

    نتیجه         = 0;
    p              = جریان->cursor;
    إذا ( p + 2 < جریان->حد )
      نتیجه       = FT_بعدی_UOFF3( p );
    جریان->cursor = p;
    أرجع نتیجه;
  }


  FT_پایه_DEF( FT_UInt32 )
  FT_جریان_Get_ULong( FT_جریان  جریان )
  {
    FT_Byte*  p;
    FT_UInt32 نتیجه;


    FT_ASSERT( جریان && جریان->cursor );

    نتیجه         = 0;
    p              = جریان->cursor;
    إذا ( p + 3 < جریان->حد )
      نتیجه       = FT_بعدی_ULONG( p );
    جریان->cursor = p;
    أرجع نتیجه;
  }


  FT_پایه_DEF( FT_UInt32 )
  FT_جریان_Get_ULong_LE( FT_جریان  جریان )
  {
    FT_Byte*  p;
    FT_UInt32 نتیجه;


    FT_ASSERT( جریان && جریان->cursor );

    نتیجه         = 0;
    p              = جریان->cursor;
    إذا ( p + 3 < جریان->حد )
      نتیجه       = FT_بعدی_ULONG_LE( p );
    جریان->cursor = p;
    أرجع نتیجه;
  }


  FT_پایه_DEF( FT_Byte )
  FT_جریان_Read_Byte( FT_جریان  جریان,
                      FT_Error*  error )
  {
    FT_Byte  نتیجه = 0;


    FT_ASSERT( جریان );

    إذا ( جریان->pos < جریان->اندازه )
    {
      إذا ( جریان->اقرأ )
      {
        إذا ( جریان->اقرأ( جریان, جریان->pos, &نتیجه, 1L ) != 1L )
          اذهب Fail;
      }
      وإلا
        نتیجه = جریان->پایه[جریان->pos];
    }
    وإلا
      اذهب Fail;

    جریان->pos++;

    *error = FT_Err_Ok;

    أرجع نتیجه;

  Fail:
    *error = FT_THROW( Invalid_جریان_Operation );
    FT_ERROR(( "FT_Stream_ReadByte:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               جریان->pos, جریان->اندازه ));

    أرجع نتیجه;
  }


  FT_پایه_DEF( FT_UInt16 )
  FT_جریان_Read_UShort( FT_جریان  جریان,
                        FT_Error*  error )
  {
    FT_Byte    reads[2];
    FT_Byte*   p;
    FT_UInt16  نتیجه = 0;


    FT_ASSERT( جریان );

    إذا ( جریان->pos + 1 < جریان->اندازه )
    {
      إذا ( جریان->اقرأ )
      {
        إذا ( جریان->اقرأ( جریان, جریان->pos, reads, 2L ) != 2L )
          اذهب Fail;

        p = reads;
      }
      وإلا
        p = جریان->پایه + جریان->pos;

      إذا ( p )
        نتیجه = FT_بعدی_USHORT( p );
    }
    وإلا
      اذهب Fail;

    جریان->pos += 2;

    *error = FT_Err_Ok;

    أرجع نتیجه;

  Fail:
    *error = FT_THROW( Invalid_جریان_Operation );
    FT_ERROR(( "FT_Stream_ReadUShort:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               جریان->pos, جریان->اندازه ));

    أرجع نتیجه;
  }


  FT_پایه_DEF( FT_UInt16 )
  FT_جریان_Read_UShort_LE( FT_جریان  جریان,
                          FT_Error*  error )
  {
    FT_Byte    reads[2];
    FT_Byte*   p;
    FT_UInt16  نتیجه = 0;


    FT_ASSERT( جریان );

    إذا ( جریان->pos + 1 < جریان->اندازه )
    {
      إذا ( جریان->اقرأ )
      {
        إذا ( جریان->اقرأ( جریان, جریان->pos, reads, 2L ) != 2L )
          اذهب Fail;

        p = reads;
      }
      وإلا
        p = جریان->پایه + جریان->pos;

      إذا ( p )
        نتیجه = FT_بعدی_USHORT_LE( p );
    }
    وإلا
      اذهب Fail;

    جریان->pos += 2;

    *error = FT_Err_Ok;

    أرجع نتیجه;

  Fail:
    *error = FT_THROW( Invalid_جریان_Operation );
    FT_ERROR(( "FT_Stream_ReadUShortLE:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               جریان->pos, جریان->اندازه ));

    أرجع نتیجه;
  }


  FT_پایه_DEF( FT_ULong )
  FT_جریان_Read_UOffset( FT_جریان  جریان,
                         FT_Error*  error )
  {
    FT_Byte   reads[3];
    FT_Byte*  p;
    FT_ULong  نتیجه = 0;


    FT_ASSERT( جریان );

    إذا ( جریان->pos + 2 < جریان->اندازه )
    {
      إذا ( جریان->اقرأ )
      {
        إذا (جریان->اقرأ( جریان, جریان->pos, reads, 3L ) != 3L )
          اذهب Fail;

        p = reads;
      }
      وإلا
        p = جریان->پایه + جریان->pos;

      إذا ( p )
        نتیجه = FT_بعدی_UOFF3( p );
    }
    وإلا
      اذهب Fail;

    جریان->pos += 3;

    *error = FT_Err_Ok;

    أرجع نتیجه;

  Fail:
    *error = FT_THROW( Invalid_جریان_Operation );
    FT_ERROR(( "FT_Stream_ReadUOffset:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               جریان->pos, جریان->اندازه ));

    أرجع نتیجه;
  }


  FT_پایه_DEF( FT_UInt32 )
  FT_جریان_Read_ULong( FT_جریان  جریان,
                       FT_Error*  error )
  {
    FT_Byte   reads[4];
    FT_Byte*  p;
    FT_UInt32 نتیجه = 0;


    FT_ASSERT( جریان );

    إذا ( جریان->pos + 3 < جریان->اندازه )
    {
      إذا ( جریان->اقرأ )
      {
        إذا ( جریان->اقرأ( جریان, جریان->pos, reads, 4L ) != 4L )
          اذهب Fail;

        p = reads;
      }
      وإلا
        p = جریان->پایه + جریان->pos;

      إذا ( p )
        نتیجه = FT_بعدی_ULONG( p );
    }
    وإلا
      اذهب Fail;

    جریان->pos += 4;

    *error = FT_Err_Ok;

    أرجع نتیجه;

  Fail:
    *error = FT_THROW( Invalid_جریان_Operation );
    FT_ERROR(( "FT_Stream_ReadULong:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               جریان->pos, جریان->اندازه ));

    أرجع نتیجه;
  }


  FT_پایه_DEF( FT_UInt32 )
  FT_جریان_Read_ULong_LE( FT_جریان  جریان,
                         FT_Error*  error )
  {
    FT_Byte   reads[4];
    FT_Byte*  p;
    FT_UInt32 نتیجه = 0;


    FT_ASSERT( جریان );

    إذا ( جریان->pos + 3 < جریان->اندازه )
    {
      إذا ( جریان->اقرأ )
      {
        إذا ( جریان->اقرأ( جریان, جریان->pos, reads, 4L ) != 4L )
          اذهب Fail;

        p = reads;
      }
      وإلا
        p = جریان->پایه + جریان->pos;

      إذا ( p )
        نتیجه = FT_بعدی_ULONG_LE( p );
    }
    وإلا
      اذهب Fail;

    جریان->pos += 4;

    *error = FT_Err_Ok;

    أرجع نتیجه;

  Fail:
    *error = FT_THROW( Invalid_جریان_Operation );
    FT_ERROR(( "FT_Stream_ReadULongLE:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               جریان->pos, جریان->اندازه ));

    أرجع نتیجه;
  }


  FT_پایه_DEF( FT_Error )
  FT_جریان_Read_Fields( FT_جریان              جریان,
                        ثابت FT_قاب_فیلد*  fields,
                        فراغ*                  structure )
  {
    FT_Error  error;
    FT_Bool   قاب_accessed = 0;
    FT_Byte*  cursor;


    إذا ( !fields )
      أرجع FT_THROW( Invalid_آرگومان );

    إذا ( !جریان )
      أرجع FT_THROW( Invalid_جریان_Handle );

    cursor = جریان->cursor;

    error = FT_Err_Ok;
    افعل
    {
      FT_ULong  مقدار;
      FT_Int    sign_لغزش;
      FT_Byte*  p;


      بدل ( fields->مقدار )
      {
      حالة ft_قاب_شروع:  /* access a new frame */
        error = FT_جریان_Enter_قاب( جریان, fields->جابه_جایی );
        إذا ( error )
          اذهب Exit;

        قاب_accessed = 1;
        cursor         = جریان->cursor;
        fields++;
        استمر;  /* loop! */

      حالة ft_قاب_بایتها:  /* read a byte sequence */
      حالة ft_قاب_skip:   /* skip some bytes      */
        {
          FT_جابه_جایی_2  len = fields->اندازه;


          إذا ( len > (FT_جابه_جایی_2)( جریان->حد - cursor ) )
          {
            error = FT_THROW( Invalid_جریان_Operation );
            اذهب Exit;
          }

          إذا ( fields->مقدار == ft_قاب_بایتها )
          {
            p = (FT_Byte*)structure + fields->جابه_جایی;
            FT_MEM_COPY( p, cursor, len );
          }
          cursor += len;
          fields++;
          استمر;
        }

      حالة ft_قاب_byte:
      حالة ft_قاب_schar:  /* read a single byte */
        مقدار = FT_بعدی_BYTE( cursor );
        sign_لغزش = 24;
        اكسر;

      حالة ft_قاب_short_be:
      حالة ft_قاب_ushort_be:  /* read a 2-byte big-endian short */
        مقدار = FT_بعدی_USHORT( cursor );
        sign_لغزش = 16;
        اكسر;

      حالة ft_قاب_short_le:
      حالة ft_قاب_ushort_le:  /* read a 2-byte little-endian short */
        مقدار = FT_بعدی_USHORT_LE( cursor );
        sign_لغزش = 16;
        اكسر;

      حالة ft_قاب_long_be:
      حالة ft_قاب_ulong_be:  /* read a 4-byte big-endian long */
        مقدار = FT_بعدی_ULONG( cursor );
        sign_لغزش = 0;
        اكسر;

      حالة ft_قاب_long_le:
      حالة ft_قاب_ulong_le:  /* read a 4-byte little-endian long */
        مقدار = FT_بعدی_ULONG_LE( cursor );
        sign_لغزش = 0;
        اكسر;

      حالة ft_قاب_off3_be:
      حالة ft_قاب_uoff3_be:  /* read a 3-byte big-endian long */
        مقدار = FT_بعدی_UOFF3( cursor );
        sign_لغزش = 8;
        اكسر;

      حالة ft_قاب_off3_le:
      حالة ft_قاب_uoff3_le:  /* read a 3-byte little-endian long */
        مقدار = FT_بعدی_UOFF3_LE( cursor );
        sign_لغزش = 8;
        اكسر;

      افتراضي:
        /* otherwise, exit the loop */
        جریان->cursor = cursor;
        اذهب Exit;
      }

      /* now, compute the signed value if necessary */
      إذا ( fields->مقدار & FT_قاب_OP_SIGNED )
        مقدار = (FT_ULong)( (FT_Int32)( مقدار << sign_لغزش ) >> sign_لغزش );

      /* finally, store the value in the object */

      p = (FT_Byte*)structure + fields->جابه_جایی;
      بدل ( fields->اندازه )
      {
      حالة ( 8 / FT_CHAR_بیت ):
        *(FT_Byte*)p = (FT_Byte)مقدار;
        اكسر;

      حالة ( 16 / FT_CHAR_بیت ):
        *(FT_UShort*)p = (FT_UShort)مقدار;
        اكسر;

      حالة ( 32 / FT_CHAR_بیت ):
        *(FT_UInt32*)p = (FT_UInt32)مقدار;
        اكسر;

      افتراضي:  /* for 64-bit systems */
        *(FT_ULong*)p = (FT_ULong)مقدار;
      }

      /* go to next field */
      fields++;
    }
    طالما ( 1 );

  Exit:
    /* close the frame if it was opened by this read */
    إذا ( قاب_accessed )
      FT_جریان_Exit_قاب( جریان );

    أرجع error;
  }


/*انتها*/
