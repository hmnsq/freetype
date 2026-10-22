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


#شامل <freetype/internal/ftstream.h>
#شامل <freetype/internal/ftdebug.h>


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#تعریف_منسوخ  FT_COMPONENT
#تعریف_کرو FT_COMPONENT  دھارا


  FT_بنیاد_DEF( خالی )
  FT_دھارا_Open_حافظہ( FT_دھارا       دھارا,
                        ثابت FT_Byte*  بنیاد,
                        FT_ULong        سائز )
  {
    دھارا->بنیاد   = (FT_Byte*) بنیاد;
    دھارا->سائز   = سائز;
    دھارا->pos    = 0;
    دھارا->cursor = تہی;
    دھارا->اقرأ   = تہی;
    دھارا->close  = تہی;
  }


  FT_بنیاد_DEF( خالی )
  FT_دھارا_Close( FT_دھارا  دھارا )
  {
    اگر ( دھارا && دھارا->close )
      دھارا->close( دھارا );
  }


  FT_بنیاد_DEF( FT_Error )
  FT_دھارا_Seek( FT_دھارا  دھارا,
                  FT_ULong   pos )
  {
    FT_Error  error = FT_Err_Ok;


    اگر ( دھارا->اقرأ )
    {
      اگر ( دھارا->اقرأ( دھارا, pos, تہی, 0 ) )
      {
        FT_ERROR(( "FT_Stream_Seek:"
                   " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
                   pos, دھارا->سائز ));

        error = FT_THROW( Invalid_دھارا_Operation );
      }
    }
    /* note that seeking to the first position after the file is valid */
    ورنہ اگر ( pos > دھارا->سائز )
    {
      FT_ERROR(( "FT_Stream_Seek:"
                 " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
                 pos, دھارا->سائز ));

      error = FT_THROW( Invalid_دھارا_Operation );
    }

    اگر ( !error )
      دھارا->pos = pos;

    واپس error;
  }


  FT_بنیاد_DEF( FT_Error )
  FT_دھارا_Skip( FT_دھارا  دھارا,
                  FT_Long    فاصلہ )
  {
    اگر ( فاصلہ < 0 )
      واپس FT_THROW( Invalid_دھارا_Operation );

    واپس FT_دھارا_Seek( دھارا, دھارا->pos + (FT_ULong)فاصلہ );
  }


  FT_بنیاد_DEF( FT_ULong )
  FT_دھارا_Pos( FT_دھارا  دھارا )
  {
    واپس دھارا->pos;
  }


  FT_بنیاد_DEF( FT_Error )
  FT_دھارا_Read( FT_دھارا  دھارا,
                  FT_Byte*   بفر,
                  FT_ULong   گنتی )
  {
    واپس FT_دھارا_Read_At( دھارا, دھارا->pos, بفر, گنتی );
  }


  FT_بنیاد_DEF( FT_Error )
  FT_دھارا_Read_At( FT_دھارا  دھارا,
                    FT_ULong   pos,
                    FT_Byte*   بفر,
                    FT_ULong   گنتی )
  {
    FT_Error  error = FT_Err_Ok;
    FT_ULong  read_بائٹیں;


    اگر ( pos >= دھارا->سائز )
    {
      FT_ERROR(( "FT_Stream_ReadAt:"
                 " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
                 pos, دھارا->سائز ));

      واپس FT_THROW( Invalid_دھارا_Operation );
    }

    اگر ( دھارا->اقرأ )
      read_بائٹیں = دھارا->اقرأ( دھارا, pos, بفر, گنتی );
    ورنہ
    {
      read_بائٹیں = دھارا->سائز - pos;
      اگر ( read_بائٹیں > گنتی )
        read_بائٹیں = گنتی;

      /* Allow "reading" zero bytes without UB even if buffer is NULL */
      اگر ( گنتی )
        FT_MEM_COPY( بفر, دھارا->بنیاد + pos, read_بائٹیں );
    }

    دھارا->pos = pos + read_بائٹیں;

    اگر ( read_بائٹیں < گنتی )
    {
      FT_ERROR(( "FT_Stream_ReadAt:"
                 " invalid read; expected %lu bytes, got %lu\n",
                 گنتی, read_بائٹیں ));

      error = FT_THROW( Invalid_دھارا_Operation );
    }

    واپس error;
  }


  FT_بنیاد_DEF( FT_ULong )
  FT_دھارا_Try_Read( FT_دھارا  دھارا,
                     FT_Byte*   بفر,
                     FT_ULong   گنتی )
  {
    FT_ULong  read_بائٹیں = 0;


    اگر ( دھارا->pos >= دھارا->سائز )
      جاؤ Exit;

    اگر ( دھارا->اقرأ )
      read_بائٹیں = دھارا->اقرأ( دھارا, دھارا->pos, بفر, گنتی );
    ورنہ
    {
      read_بائٹیں = دھارا->سائز - دھارا->pos;
      اگر ( read_بائٹیں > گنتی )
        read_بائٹیں = گنتی;

      /* Allow "reading" zero bytes without UB even if buffer is NULL */
      اگر ( گنتی )
        FT_MEM_COPY( بفر, دھارا->بنیاد + دھارا->pos, read_بائٹیں );
    }

    دھارا->pos += read_بائٹیں;

  Exit:
    واپس read_بائٹیں;
  }


  FT_بنیاد_DEF( FT_Error )
  FT_دھارا_نکالو_چوکھٹا( FT_دھارا  دھارا,
                          FT_ULong   گنتی,
                          FT_Byte**  pbytes )
  {
    FT_Error  error;


    error = FT_دھارا_Enter_چوکھٹا( دھارا, گنتی );
    اگر ( !error )
    {
      *pbytes = (FT_Byte*)دھارا->cursor;

      /* equivalent to FT_Stream_ExitFrame(), with no memory block release */
      دھارا->cursor = تہی;
      دھارا->حد_بندی  = تہی;
    }

    واپس error;
  }


  FT_بنیاد_DEF( خالی )
  FT_دھارا_Release_چوکھٹا( FT_دھارا  دھارا,
                          FT_Byte**  pbytes )
  {
    اگر ( دھارا && دھارا->اقرأ )
    {
      FT_حافظہ  حافظہ = دھارا->حافظہ;


#اگر_تعریف_شدہ FT_DEBUG_حافظہ
      ft_mem_free( حافظہ, *pbytes );
#ورنہ
      FT_FREE( *pbytes );
#اگر_ختم
    }

    *pbytes = تہی;
  }


  FT_بنیاد_DEF( FT_Error )
  FT_دھارا_Enter_چوکھٹا( FT_دھارا  دھارا,
                        FT_ULong   گنتی )
  {
    FT_Error  error = FT_Err_Ok;
    FT_ULong  read_بائٹیں;


    FT_TRACE7(( "FT_Stream_EnterFrame: %lu bytes\n", گنتی ));

    /* check for nested frame access */
    FT_ASSERT( دھارا && دھارا->cursor == 0 );

    اگر ( دھارا->اقرأ )
    {
      /* allocate the frame in memory */
      FT_حافظہ  حافظہ = دھارا->حافظہ;


      /* simple sanity check */
      اگر ( گنتی > دھارا->سائز )
      {
        FT_ERROR(( "FT_Stream_EnterFrame:"
                   " frame size (%lu) larger than stream size (%lu)\n",
                   گنتی, دھارا->سائز ));

        error = FT_THROW( Invalid_دھارا_Operation );
        جاؤ Exit;
      }

#اگر_تعریف_شدہ FT_DEBUG_حافظہ
      /* assume `ft_debug_file_` and `ft_debug_lineno_` are already set */
      دھارا->بنیاد = (غیرعلامتی حرف*)ft_mem_qalloc( حافظہ,
                                                    (FT_Long)گنتی,
                                                    &error );
      اگر ( error )
        جاؤ Exit;
#ورنہ
      اگر ( FT_QALLOC( دھارا->بنیاد, گنتی ) )
        جاؤ Exit;
#اگر_ختم
      /* read it */
      read_بائٹیں = دھارا->اقرأ( دھارا, دھارا->pos,
                                 دھارا->بنیاد, گنتی );
      اگر ( read_بائٹیں < گنتی )
      {
        FT_ERROR(( "FT_Stream_EnterFrame:"
                   " invalid read; expected %lu bytes, got %lu\n",
                   گنتی, read_بائٹیں ));

        FT_FREE( دھارا->بنیاد );
        error = FT_THROW( Invalid_دھارا_Operation );
      }

      دھارا->cursor = دھارا->بنیاد;
      دھارا->حد_بندی  = FT_انحراف( دھارا->cursor, گنتی );
      دھارا->pos   += read_بائٹیں;
    }
    ورنہ
    {
      /* check current and new position */
      اگر ( دھارا->pos >= دھارا->سائز        ||
           دھارا->سائز - دھارا->pos < گنتی )
      {
        FT_ERROR(( "FT_Stream_EnterFrame:"
                   " invalid i/o; pos = 0x%lx, count = %lu, size = 0x%lx\n",
                   دھارا->pos, گنتی, دھارا->سائز ));

        error = FT_THROW( Invalid_دھارا_Operation );
        جاؤ Exit;
      }

      /* set cursor */
      دھارا->cursor = دھارا->بنیاد + دھارا->pos;
      دھارا->حد_بندی  = دھارا->cursor + گنتی;
      دھارا->pos   += گنتی;
    }

  Exit:
    واپس error;
  }


  FT_بنیاد_DEF( خالی )
  FT_دھارا_Exit_چوکھٹا( FT_دھارا  دھارا )
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

    FT_ASSERT( دھارا );

    اگر ( دھارا->اقرأ )
    {
      FT_حافظہ  حافظہ = دھارا->حافظہ;


#اگر_تعریف_شدہ FT_DEBUG_حافظہ
      ft_mem_free( حافظہ, دھارا->بنیاد );
      دھارا->بنیاد = تہی;
#ورنہ
      FT_FREE( دھارا->بنیاد );
#اگر_ختم
    }

    دھارا->cursor = تہی;
    دھارا->حد_بندی  = تہی;
  }


  FT_بنیاد_DEF( FT_Byte )
  FT_دھارا_Get_Byte( FT_دھارا  دھارا )
  {
    FT_Byte  نتیجہ;


    FT_ASSERT( دھارا && دھارا->cursor );

    نتیجہ = 0;
    اگر ( دھارا->cursor < دھارا->حد_بندی )
      نتیجہ = *دھارا->cursor++;

    واپس نتیجہ;
  }


  FT_بنیاد_DEF( FT_UInt16 )
  FT_دھارا_Get_UShort( FT_دھارا  دھارا )
  {
    FT_Byte*   p;
    FT_UInt16  نتیجہ;


    FT_ASSERT( دھارا && دھارا->cursor );

    نتیجہ         = 0;
    p              = دھارا->cursor;
    اگر ( p + 1 < دھارا->حد_بندی )
      نتیجہ       = FT_اگلا_USHORT( p );
    دھارا->cursor = p;

    واپس نتیجہ;
  }


  FT_بنیاد_DEF( FT_UInt16 )
  FT_دھارا_Get_UShort_LE( FT_دھارا  دھارا )
  {
    FT_Byte*   p;
    FT_UInt16  نتیجہ;


    FT_ASSERT( دھارا && دھارا->cursor );

    نتیجہ         = 0;
    p              = دھارا->cursor;
    اگر ( p + 1 < دھارا->حد_بندی )
      نتیجہ       = FT_اگلا_USHORT_LE( p );
    دھارا->cursor = p;

    واپس نتیجہ;
  }


  FT_بنیاد_DEF( FT_UInt32 )
  FT_دھارا_Get_UOffset( FT_دھارا  دھارا )
  {
    FT_Byte*  p;
    FT_UInt32 نتیجہ;


    FT_ASSERT( دھارا && دھارا->cursor );

    نتیجہ         = 0;
    p              = دھارا->cursor;
    اگر ( p + 2 < دھارا->حد_بندی )
      نتیجہ       = FT_اگلا_UOFF3( p );
    دھارا->cursor = p;
    واپس نتیجہ;
  }


  FT_بنیاد_DEF( FT_UInt32 )
  FT_دھارا_Get_ULong( FT_دھارا  دھارا )
  {
    FT_Byte*  p;
    FT_UInt32 نتیجہ;


    FT_ASSERT( دھارا && دھارا->cursor );

    نتیجہ         = 0;
    p              = دھارا->cursor;
    اگر ( p + 3 < دھارا->حد_بندی )
      نتیجہ       = FT_اگلا_ULONG( p );
    دھارا->cursor = p;
    واپس نتیجہ;
  }


  FT_بنیاد_DEF( FT_UInt32 )
  FT_دھارا_Get_ULong_LE( FT_دھارا  دھارا )
  {
    FT_Byte*  p;
    FT_UInt32 نتیجہ;


    FT_ASSERT( دھارا && دھارا->cursor );

    نتیجہ         = 0;
    p              = دھارا->cursor;
    اگر ( p + 3 < دھارا->حد_بندی )
      نتیجہ       = FT_اگلا_ULONG_LE( p );
    دھارا->cursor = p;
    واپس نتیجہ;
  }


  FT_بنیاد_DEF( FT_Byte )
  FT_دھارا_Read_Byte( FT_دھارا  دھارا,
                      FT_Error*  error )
  {
    FT_Byte  نتیجہ = 0;


    FT_ASSERT( دھارا );

    اگر ( دھارا->pos < دھارا->سائز )
    {
      اگر ( دھارا->اقرأ )
      {
        اگر ( دھارا->اقرأ( دھارا, دھارا->pos, &نتیجہ, 1L ) != 1L )
          جاؤ Fail;
      }
      ورنہ
        نتیجہ = دھارا->بنیاد[دھارا->pos];
    }
    ورنہ
      جاؤ Fail;

    دھارا->pos++;

    *error = FT_Err_Ok;

    واپس نتیجہ;

  Fail:
    *error = FT_THROW( Invalid_دھارا_Operation );
    FT_ERROR(( "FT_Stream_ReadByte:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               دھارا->pos, دھارا->سائز ));

    واپس نتیجہ;
  }


  FT_بنیاد_DEF( FT_UInt16 )
  FT_دھارا_Read_UShort( FT_دھارا  دھارا,
                        FT_Error*  error )
  {
    FT_Byte    reads[2];
    FT_Byte*   p;
    FT_UInt16  نتیجہ = 0;


    FT_ASSERT( دھارا );

    اگر ( دھارا->pos + 1 < دھارا->سائز )
    {
      اگر ( دھارا->اقرأ )
      {
        اگر ( دھارا->اقرأ( دھارا, دھارا->pos, reads, 2L ) != 2L )
          جاؤ Fail;

        p = reads;
      }
      ورنہ
        p = دھارا->بنیاد + دھارا->pos;

      اگر ( p )
        نتیجہ = FT_اگلا_USHORT( p );
    }
    ورنہ
      جاؤ Fail;

    دھارا->pos += 2;

    *error = FT_Err_Ok;

    واپس نتیجہ;

  Fail:
    *error = FT_THROW( Invalid_دھارا_Operation );
    FT_ERROR(( "FT_Stream_ReadUShort:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               دھارا->pos, دھارا->سائز ));

    واپس نتیجہ;
  }


  FT_بنیاد_DEF( FT_UInt16 )
  FT_دھارا_Read_UShort_LE( FT_دھارا  دھارا,
                          FT_Error*  error )
  {
    FT_Byte    reads[2];
    FT_Byte*   p;
    FT_UInt16  نتیجہ = 0;


    FT_ASSERT( دھارا );

    اگر ( دھارا->pos + 1 < دھارا->سائز )
    {
      اگر ( دھارا->اقرأ )
      {
        اگر ( دھارا->اقرأ( دھارا, دھارا->pos, reads, 2L ) != 2L )
          جاؤ Fail;

        p = reads;
      }
      ورنہ
        p = دھارا->بنیاد + دھارا->pos;

      اگر ( p )
        نتیجہ = FT_اگلا_USHORT_LE( p );
    }
    ورنہ
      جاؤ Fail;

    دھارا->pos += 2;

    *error = FT_Err_Ok;

    واپس نتیجہ;

  Fail:
    *error = FT_THROW( Invalid_دھارا_Operation );
    FT_ERROR(( "FT_Stream_ReadUShortLE:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               دھارا->pos, دھارا->سائز ));

    واپس نتیجہ;
  }


  FT_بنیاد_DEF( FT_ULong )
  FT_دھارا_Read_UOffset( FT_دھارا  دھارا,
                         FT_Error*  error )
  {
    FT_Byte   reads[3];
    FT_Byte*  p;
    FT_ULong  نتیجہ = 0;


    FT_ASSERT( دھارا );

    اگر ( دھارا->pos + 2 < دھارا->سائز )
    {
      اگر ( دھارا->اقرأ )
      {
        اگر (دھارا->اقرأ( دھارا, دھارا->pos, reads, 3L ) != 3L )
          جاؤ Fail;

        p = reads;
      }
      ورنہ
        p = دھارا->بنیاد + دھارا->pos;

      اگر ( p )
        نتیجہ = FT_اگلا_UOFF3( p );
    }
    ورنہ
      جاؤ Fail;

    دھارا->pos += 3;

    *error = FT_Err_Ok;

    واپس نتیجہ;

  Fail:
    *error = FT_THROW( Invalid_دھارا_Operation );
    FT_ERROR(( "FT_Stream_ReadUOffset:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               دھارا->pos, دھارا->سائز ));

    واپس نتیجہ;
  }


  FT_بنیاد_DEF( FT_UInt32 )
  FT_دھارا_Read_ULong( FT_دھارا  دھارا,
                       FT_Error*  error )
  {
    FT_Byte   reads[4];
    FT_Byte*  p;
    FT_UInt32 نتیجہ = 0;


    FT_ASSERT( دھارا );

    اگر ( دھارا->pos + 3 < دھارا->سائز )
    {
      اگر ( دھارا->اقرأ )
      {
        اگر ( دھارا->اقرأ( دھارا, دھارا->pos, reads, 4L ) != 4L )
          جاؤ Fail;

        p = reads;
      }
      ورنہ
        p = دھارا->بنیاد + دھارا->pos;

      اگر ( p )
        نتیجہ = FT_اگلا_ULONG( p );
    }
    ورنہ
      جاؤ Fail;

    دھارا->pos += 4;

    *error = FT_Err_Ok;

    واپس نتیجہ;

  Fail:
    *error = FT_THROW( Invalid_دھارا_Operation );
    FT_ERROR(( "FT_Stream_ReadULong:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               دھارا->pos, دھارا->سائز ));

    واپس نتیجہ;
  }


  FT_بنیاد_DEF( FT_UInt32 )
  FT_دھارا_Read_ULong_LE( FT_دھارا  دھارا,
                         FT_Error*  error )
  {
    FT_Byte   reads[4];
    FT_Byte*  p;
    FT_UInt32 نتیجہ = 0;


    FT_ASSERT( دھارا );

    اگر ( دھارا->pos + 3 < دھارا->سائز )
    {
      اگر ( دھارا->اقرأ )
      {
        اگر ( دھارا->اقرأ( دھارا, دھارا->pos, reads, 4L ) != 4L )
          جاؤ Fail;

        p = reads;
      }
      ورنہ
        p = دھارا->بنیاد + دھارا->pos;

      اگر ( p )
        نتیجہ = FT_اگلا_ULONG_LE( p );
    }
    ورنہ
      جاؤ Fail;

    دھارا->pos += 4;

    *error = FT_Err_Ok;

    واپس نتیجہ;

  Fail:
    *error = FT_THROW( Invalid_دھارا_Operation );
    FT_ERROR(( "FT_Stream_ReadULongLE:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               دھارا->pos, دھارا->سائز ));

    واپس نتیجہ;
  }


  FT_بنیاد_DEF( FT_Error )
  FT_دھارا_Read_Fields( FT_دھارا              دھارا,
                        ثابت FT_چوکھٹا_میدان*  fields,
                        خالی*                  structure )
  {
    FT_Error  error;
    FT_Bool   چوکھٹا_accessed = 0;
    FT_Byte*  cursor;


    اگر ( !fields )
      واپس FT_THROW( Invalid_دلیل );

    اگر ( !دھارا )
      واپس FT_THROW( Invalid_دھارا_Handle );

    cursor = دھارا->cursor;

    error = FT_Err_Ok;
    کرو
    {
      FT_ULong  قدر;
      FT_Int    sign_سرکاؤ;
      FT_Byte*  p;


      انتخاب ( fields->قدر )
      {
      حالت ft_چوکھٹا_آغاز:  /* access a new frame */
        error = FT_دھارا_Enter_چوکھٹا( دھارا, fields->انحراف );
        اگر ( error )
          جاؤ Exit;

        چوکھٹا_accessed = 1;
        cursor         = دھارا->cursor;
        fields++;
        جاری;  /* loop! */

      حالت ft_چوکھٹا_بائٹیں:  /* read a byte sequence */
      حالت ft_چوکھٹا_skip:   /* skip some bytes      */
        {
          FT_انحراف_2  len = fields->سائز;


          اگر ( len > (FT_انحراف_2)( دھارا->حد_بندی - cursor ) )
          {
            error = FT_THROW( Invalid_دھارا_Operation );
            جاؤ Exit;
          }

          اگر ( fields->قدر == ft_چوکھٹا_بائٹیں )
          {
            p = (FT_Byte*)structure + fields->انحراف;
            FT_MEM_COPY( p, cursor, len );
          }
          cursor += len;
          fields++;
          جاری;
        }

      حالت ft_چوکھٹا_byte:
      حالت ft_چوکھٹا_schar:  /* read a single byte */
        قدر = FT_اگلا_BYTE( cursor );
        sign_سرکاؤ = 24;
        توڑو;

      حالت ft_چوکھٹا_short_be:
      حالت ft_چوکھٹا_ushort_be:  /* read a 2-byte big-endian short */
        قدر = FT_اگلا_USHORT( cursor );
        sign_سرکاؤ = 16;
        توڑو;

      حالت ft_چوکھٹا_short_le:
      حالت ft_چوکھٹا_ushort_le:  /* read a 2-byte little-endian short */
        قدر = FT_اگلا_USHORT_LE( cursor );
        sign_سرکاؤ = 16;
        توڑو;

      حالت ft_چوکھٹا_long_be:
      حالت ft_چوکھٹا_ulong_be:  /* read a 4-byte big-endian long */
        قدر = FT_اگلا_ULONG( cursor );
        sign_سرکاؤ = 0;
        توڑو;

      حالت ft_چوکھٹا_long_le:
      حالت ft_چوکھٹا_ulong_le:  /* read a 4-byte little-endian long */
        قدر = FT_اگلا_ULONG_LE( cursor );
        sign_سرکاؤ = 0;
        توڑو;

      حالت ft_چوکھٹا_off3_be:
      حالت ft_چوکھٹا_uoff3_be:  /* read a 3-byte big-endian long */
        قدر = FT_اگلا_UOFF3( cursor );
        sign_سرکاؤ = 8;
        توڑو;

      حالت ft_چوکھٹا_off3_le:
      حالت ft_چوکھٹا_uoff3_le:  /* read a 3-byte little-endian long */
        قدر = FT_اگلا_UOFF3_LE( cursor );
        sign_سرکاؤ = 8;
        توڑو;

      طےشدہ:
        /* otherwise, exit the loop */
        دھارا->cursor = cursor;
        جاؤ Exit;
      }

      /* now, compute the signed value if necessary */
      اگر ( fields->قدر & FT_چوکھٹا_OP_SIGNED )
        قدر = (FT_ULong)( (FT_Int32)( قدر << sign_سرکاؤ ) >> sign_سرکاؤ );

      /* finally, store the value in the object */

      p = (FT_Byte*)structure + fields->انحراف;
      انتخاب ( fields->سائز )
      {
      حالت ( 8 / FT_CHAR_ثنائی_ہندسہ ):
        *(FT_Byte*)p = (FT_Byte)قدر;
        توڑو;

      حالت ( 16 / FT_CHAR_ثنائی_ہندسہ ):
        *(FT_UShort*)p = (FT_UShort)قدر;
        توڑو;

      حالت ( 32 / FT_CHAR_ثنائی_ہندسہ ):
        *(FT_UInt32*)p = (FT_UInt32)قدر;
        توڑو;

      طےشدہ:  /* for 64-bit systems */
        *(FT_ULong*)p = (FT_ULong)قدر;
      }

      /* go to next field */
      fields++;
    }
    جب ( 1 );

  Exit:
    /* close the frame if it was opened by this read */
    اگر ( چوکھٹا_accessed )
      FT_دھارا_Exit_چوکھٹا( دھارا );

    واپس error;
  }


/*انتہا*/
