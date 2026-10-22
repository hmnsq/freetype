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
#تعریف_لغوه  FT_COMPONENT
#تعریف_کړه FT_COMPONENT  بهیر


  FT_بنسټ_DEF( فراغ )
  FT_بهیر_Open_حافظه( FT_بهیر       بهیر,
                        ثابت FT_Byte*  بنسټ,
                        FT_ULong        اندازه )
  {
    بهیر->بنسټ   = (FT_Byte*) بنسټ;
    بهیر->اندازه   = اندازه;
    بهیر->pos    = 0;
    بهیر->cursor = عدم;
    بهیر->اقرأ   = عدم;
    بهیر->close  = عدم;
  }


  FT_بنسټ_DEF( فراغ )
  FT_بهیر_Close( FT_بهیر  بهیر )
  {
    إذا ( بهیر && بهیر->close )
      بهیر->close( بهیر );
  }


  FT_بنسټ_DEF( FT_Error )
  FT_بهیر_Seek( FT_بهیر  بهیر,
                  FT_ULong   pos )
  {
    FT_Error  error = FT_Err_Ok;


    إذا ( بهیر->اقرأ )
    {
      إذا ( بهیر->اقرأ( بهیر, pos, عدم, 0 ) )
      {
        FT_ERROR(( "FT_Stream_Seek:"
                   " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
                   pos, بهیر->اندازه ));

        error = FT_THROW( Invalid_بهیر_Operation );
      }
    }
    /* note that seeking to the first position after the file is valid */
    وإلا إذا ( pos > بهیر->اندازه )
    {
      FT_ERROR(( "FT_Stream_Seek:"
                 " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
                 pos, بهیر->اندازه ));

      error = FT_THROW( Invalid_بهیر_Operation );
    }

    إذا ( !error )
      بهیر->pos = pos;

    أرجع error;
  }


  FT_بنسټ_DEF( FT_Error )
  FT_بهیر_Skip( FT_بهیر  بهیر,
                  FT_Long    واټن )
  {
    إذا ( واټن < 0 )
      أرجع FT_THROW( Invalid_بهیر_Operation );

    أرجع FT_بهیر_Seek( بهیر, بهیر->pos + (FT_ULong)واټن );
  }


  FT_بنسټ_DEF( FT_ULong )
  FT_بهیر_Pos( FT_بهیر  بهیر )
  {
    أرجع بهیر->pos;
  }


  FT_بنسټ_DEF( FT_Error )
  FT_بهیر_Read( FT_بهیر  بهیر,
                  FT_Byte*   بفر,
                  FT_ULong   شمېر )
  {
    أرجع FT_بهیر_Read_At( بهیر, بهیر->pos, بفر, شمېر );
  }


  FT_بنسټ_DEF( FT_Error )
  FT_بهیر_Read_At( FT_بهیر  بهیر,
                    FT_ULong   pos,
                    FT_Byte*   بفر,
                    FT_ULong   شمېر )
  {
    FT_Error  error = FT_Err_Ok;
    FT_ULong  read_بایټونه;


    إذا ( pos >= بهیر->اندازه )
    {
      FT_ERROR(( "FT_Stream_ReadAt:"
                 " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
                 pos, بهیر->اندازه ));

      أرجع FT_THROW( Invalid_بهیر_Operation );
    }

    إذا ( بهیر->اقرأ )
      read_بایټونه = بهیر->اقرأ( بهیر, pos, بفر, شمېر );
    وإلا
    {
      read_بایټونه = بهیر->اندازه - pos;
      إذا ( read_بایټونه > شمېر )
        read_بایټونه = شمېر;

      /* Allow "reading" zero bytes without UB even if buffer is NULL */
      إذا ( شمېر )
        FT_MEM_COPY( بفر, بهیر->بنسټ + pos, read_بایټونه );
    }

    بهیر->pos = pos + read_بایټونه;

    إذا ( read_بایټونه < شمېر )
    {
      FT_ERROR(( "FT_Stream_ReadAt:"
                 " invalid read; expected %lu bytes, got %lu\n",
                 شمېر, read_بایټونه ));

      error = FT_THROW( Invalid_بهیر_Operation );
    }

    أرجع error;
  }


  FT_بنسټ_DEF( FT_ULong )
  FT_بهیر_Try_Read( FT_بهیر  بهیر,
                     FT_Byte*   بفر,
                     FT_ULong   شمېر )
  {
    FT_ULong  read_بایټونه = 0;


    إذا ( بهیر->pos >= بهیر->اندازه )
      اذهب Exit;

    إذا ( بهیر->اقرأ )
      read_بایټونه = بهیر->اقرأ( بهیر, بهیر->pos, بفر, شمېر );
    وإلا
    {
      read_بایټونه = بهیر->اندازه - بهیر->pos;
      إذا ( read_بایټونه > شمېر )
        read_بایټونه = شمېر;

      /* Allow "reading" zero bytes without UB even if buffer is NULL */
      إذا ( شمېر )
        FT_MEM_COPY( بفر, بهیر->بنسټ + بهیر->pos, read_بایټونه );
    }

    بهیر->pos += read_بایټونه;

  Exit:
    أرجع read_بایټونه;
  }


  FT_بنسټ_DEF( FT_Error )
  FT_بهیر_ایستل_چوکاټ( FT_بهیر  بهیر,
                          FT_ULong   شمېر,
                          FT_Byte**  pbytes )
  {
    FT_Error  error;


    error = FT_بهیر_Enter_چوکاټ( بهیر, شمېر );
    إذا ( !error )
    {
      *pbytes = (FT_Byte*)بهیر->cursor;

      /* equivalent to FT_Stream_ExitFrame(), with no memory block release */
      بهیر->cursor = عدم;
      بهیر->حد  = عدم;
    }

    أرجع error;
  }


  FT_بنسټ_DEF( فراغ )
  FT_بهیر_Release_چوکاټ( FT_بهیر  بهیر,
                          FT_Byte**  pbytes )
  {
    إذا ( بهیر && بهیر->اقرأ )
    {
      FT_حافظه  حافظه = بهیر->حافظه;


#که_تعریف_شوی FT_DEBUG_حافظه
      ft_mem_free( حافظه, *pbytes );
#وإلا
      FT_FREE( *pbytes );
#د_که_پای
    }

    *pbytes = عدم;
  }


  FT_بنسټ_DEF( FT_Error )
  FT_بهیر_Enter_چوکاټ( FT_بهیر  بهیر,
                        FT_ULong   شمېر )
  {
    FT_Error  error = FT_Err_Ok;
    FT_ULong  read_بایټونه;


    FT_TRACE7(( "FT_Stream_EnterFrame: %lu bytes\n", شمېر ));

    /* check for nested frame access */
    FT_ASSERT( بهیر && بهیر->cursor == 0 );

    إذا ( بهیر->اقرأ )
    {
      /* allocate the frame in memory */
      FT_حافظه  حافظه = بهیر->حافظه;


      /* simple sanity check */
      إذا ( شمېر > بهیر->اندازه )
      {
        FT_ERROR(( "FT_Stream_EnterFrame:"
                   " frame size (%lu) larger than stream size (%lu)\n",
                   شمېر, بهیر->اندازه ));

        error = FT_THROW( Invalid_بهیر_Operation );
        اذهب Exit;
      }

#که_تعریف_شوی FT_DEBUG_حافظه
      /* assume `ft_debug_file_` and `ft_debug_lineno_` are already set */
      بهیر->بنسټ = (بینښه حرف*)ft_mem_qalloc( حافظه,
                                                    (FT_Long)شمېر,
                                                    &error );
      إذا ( error )
        اذهب Exit;
#وإلا
      إذا ( FT_QALLOC( بهیر->بنسټ, شمېر ) )
        اذهب Exit;
#د_که_پای
      /* read it */
      read_بایټونه = بهیر->اقرأ( بهیر, بهیر->pos,
                                 بهیر->بنسټ, شمېر );
      إذا ( read_بایټونه < شمېر )
      {
        FT_ERROR(( "FT_Stream_EnterFrame:"
                   " invalid read; expected %lu bytes, got %lu\n",
                   شمېر, read_بایټونه ));

        FT_FREE( بهیر->بنسټ );
        error = FT_THROW( Invalid_بهیر_Operation );
      }

      بهیر->cursor = بهیر->بنسټ;
      بهیر->حد  = FT_دننه_واټن( بهیر->cursor, شمېر );
      بهیر->pos   += read_بایټونه;
    }
    وإلا
    {
      /* check current and new position */
      إذا ( بهیر->pos >= بهیر->اندازه        ||
           بهیر->اندازه - بهیر->pos < شمېر )
      {
        FT_ERROR(( "FT_Stream_EnterFrame:"
                   " invalid i/o; pos = 0x%lx, count = %lu, size = 0x%lx\n",
                   بهیر->pos, شمېر, بهیر->اندازه ));

        error = FT_THROW( Invalid_بهیر_Operation );
        اذهب Exit;
      }

      /* set cursor */
      بهیر->cursor = بهیر->بنسټ + بهیر->pos;
      بهیر->حد  = بهیر->cursor + شمېر;
      بهیر->pos   += شمېر;
    }

  Exit:
    أرجع error;
  }


  FT_بنسټ_DEF( فراغ )
  FT_بهیر_Exit_چوکاټ( FT_بهیر  بهیر )
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

    FT_ASSERT( بهیر );

    إذا ( بهیر->اقرأ )
    {
      FT_حافظه  حافظه = بهیر->حافظه;


#که_تعریف_شوی FT_DEBUG_حافظه
      ft_mem_free( حافظه, بهیر->بنسټ );
      بهیر->بنسټ = عدم;
#وإلا
      FT_FREE( بهیر->بنسټ );
#د_که_پای
    }

    بهیر->cursor = عدم;
    بهیر->حد  = عدم;
  }


  FT_بنسټ_DEF( FT_Byte )
  FT_بهیر_Get_Byte( FT_بهیر  بهیر )
  {
    FT_Byte  پایله;


    FT_ASSERT( بهیر && بهیر->cursor );

    پایله = 0;
    إذا ( بهیر->cursor < بهیر->حد )
      پایله = *بهیر->cursor++;

    أرجع پایله;
  }


  FT_بنسټ_DEF( FT_UInt16 )
  FT_بهیر_Get_UShort( FT_بهیر  بهیر )
  {
    FT_Byte*   p;
    FT_UInt16  پایله;


    FT_ASSERT( بهیر && بهیر->cursor );

    پایله         = 0;
    p              = بهیر->cursor;
    إذا ( p + 1 < بهیر->حد )
      پایله       = FT_بل_USHORT( p );
    بهیر->cursor = p;

    أرجع پایله;
  }


  FT_بنسټ_DEF( FT_UInt16 )
  FT_بهیر_Get_UShort_LE( FT_بهیر  بهیر )
  {
    FT_Byte*   p;
    FT_UInt16  پایله;


    FT_ASSERT( بهیر && بهیر->cursor );

    پایله         = 0;
    p              = بهیر->cursor;
    إذا ( p + 1 < بهیر->حد )
      پایله       = FT_بل_USHORT_LE( p );
    بهیر->cursor = p;

    أرجع پایله;
  }


  FT_بنسټ_DEF( FT_UInt32 )
  FT_بهیر_Get_UOffset( FT_بهیر  بهیر )
  {
    FT_Byte*  p;
    FT_UInt32 پایله;


    FT_ASSERT( بهیر && بهیر->cursor );

    پایله         = 0;
    p              = بهیر->cursor;
    إذا ( p + 2 < بهیر->حد )
      پایله       = FT_بل_UOFF3( p );
    بهیر->cursor = p;
    أرجع پایله;
  }


  FT_بنسټ_DEF( FT_UInt32 )
  FT_بهیر_Get_ULong( FT_بهیر  بهیر )
  {
    FT_Byte*  p;
    FT_UInt32 پایله;


    FT_ASSERT( بهیر && بهیر->cursor );

    پایله         = 0;
    p              = بهیر->cursor;
    إذا ( p + 3 < بهیر->حد )
      پایله       = FT_بل_ULONG( p );
    بهیر->cursor = p;
    أرجع پایله;
  }


  FT_بنسټ_DEF( FT_UInt32 )
  FT_بهیر_Get_ULong_LE( FT_بهیر  بهیر )
  {
    FT_Byte*  p;
    FT_UInt32 پایله;


    FT_ASSERT( بهیر && بهیر->cursor );

    پایله         = 0;
    p              = بهیر->cursor;
    إذا ( p + 3 < بهیر->حد )
      پایله       = FT_بل_ULONG_LE( p );
    بهیر->cursor = p;
    أرجع پایله;
  }


  FT_بنسټ_DEF( FT_Byte )
  FT_بهیر_Read_Byte( FT_بهیر  بهیر,
                      FT_Error*  error )
  {
    FT_Byte  پایله = 0;


    FT_ASSERT( بهیر );

    إذا ( بهیر->pos < بهیر->اندازه )
    {
      إذا ( بهیر->اقرأ )
      {
        إذا ( بهیر->اقرأ( بهیر, بهیر->pos, &پایله, 1L ) != 1L )
          اذهب Fail;
      }
      وإلا
        پایله = بهیر->بنسټ[بهیر->pos];
    }
    وإلا
      اذهب Fail;

    بهیر->pos++;

    *error = FT_Err_Ok;

    أرجع پایله;

  Fail:
    *error = FT_THROW( Invalid_بهیر_Operation );
    FT_ERROR(( "FT_Stream_ReadByte:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               بهیر->pos, بهیر->اندازه ));

    أرجع پایله;
  }


  FT_بنسټ_DEF( FT_UInt16 )
  FT_بهیر_Read_UShort( FT_بهیر  بهیر,
                        FT_Error*  error )
  {
    FT_Byte    reads[2];
    FT_Byte*   p;
    FT_UInt16  پایله = 0;


    FT_ASSERT( بهیر );

    إذا ( بهیر->pos + 1 < بهیر->اندازه )
    {
      إذا ( بهیر->اقرأ )
      {
        إذا ( بهیر->اقرأ( بهیر, بهیر->pos, reads, 2L ) != 2L )
          اذهب Fail;

        p = reads;
      }
      وإلا
        p = بهیر->بنسټ + بهیر->pos;

      إذا ( p )
        پایله = FT_بل_USHORT( p );
    }
    وإلا
      اذهب Fail;

    بهیر->pos += 2;

    *error = FT_Err_Ok;

    أرجع پایله;

  Fail:
    *error = FT_THROW( Invalid_بهیر_Operation );
    FT_ERROR(( "FT_Stream_ReadUShort:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               بهیر->pos, بهیر->اندازه ));

    أرجع پایله;
  }


  FT_بنسټ_DEF( FT_UInt16 )
  FT_بهیر_Read_UShort_LE( FT_بهیر  بهیر,
                          FT_Error*  error )
  {
    FT_Byte    reads[2];
    FT_Byte*   p;
    FT_UInt16  پایله = 0;


    FT_ASSERT( بهیر );

    إذا ( بهیر->pos + 1 < بهیر->اندازه )
    {
      إذا ( بهیر->اقرأ )
      {
        إذا ( بهیر->اقرأ( بهیر, بهیر->pos, reads, 2L ) != 2L )
          اذهب Fail;

        p = reads;
      }
      وإلا
        p = بهیر->بنسټ + بهیر->pos;

      إذا ( p )
        پایله = FT_بل_USHORT_LE( p );
    }
    وإلا
      اذهب Fail;

    بهیر->pos += 2;

    *error = FT_Err_Ok;

    أرجع پایله;

  Fail:
    *error = FT_THROW( Invalid_بهیر_Operation );
    FT_ERROR(( "FT_Stream_ReadUShortLE:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               بهیر->pos, بهیر->اندازه ));

    أرجع پایله;
  }


  FT_بنسټ_DEF( FT_ULong )
  FT_بهیر_Read_UOffset( FT_بهیر  بهیر,
                         FT_Error*  error )
  {
    FT_Byte   reads[3];
    FT_Byte*  p;
    FT_ULong  پایله = 0;


    FT_ASSERT( بهیر );

    إذا ( بهیر->pos + 2 < بهیر->اندازه )
    {
      إذا ( بهیر->اقرأ )
      {
        إذا (بهیر->اقرأ( بهیر, بهیر->pos, reads, 3L ) != 3L )
          اذهب Fail;

        p = reads;
      }
      وإلا
        p = بهیر->بنسټ + بهیر->pos;

      إذا ( p )
        پایله = FT_بل_UOFF3( p );
    }
    وإلا
      اذهب Fail;

    بهیر->pos += 3;

    *error = FT_Err_Ok;

    أرجع پایله;

  Fail:
    *error = FT_THROW( Invalid_بهیر_Operation );
    FT_ERROR(( "FT_Stream_ReadUOffset:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               بهیر->pos, بهیر->اندازه ));

    أرجع پایله;
  }


  FT_بنسټ_DEF( FT_UInt32 )
  FT_بهیر_Read_ULong( FT_بهیر  بهیر,
                       FT_Error*  error )
  {
    FT_Byte   reads[4];
    FT_Byte*  p;
    FT_UInt32 پایله = 0;


    FT_ASSERT( بهیر );

    إذا ( بهیر->pos + 3 < بهیر->اندازه )
    {
      إذا ( بهیر->اقرأ )
      {
        إذا ( بهیر->اقرأ( بهیر, بهیر->pos, reads, 4L ) != 4L )
          اذهب Fail;

        p = reads;
      }
      وإلا
        p = بهیر->بنسټ + بهیر->pos;

      إذا ( p )
        پایله = FT_بل_ULONG( p );
    }
    وإلا
      اذهب Fail;

    بهیر->pos += 4;

    *error = FT_Err_Ok;

    أرجع پایله;

  Fail:
    *error = FT_THROW( Invalid_بهیر_Operation );
    FT_ERROR(( "FT_Stream_ReadULong:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               بهیر->pos, بهیر->اندازه ));

    أرجع پایله;
  }


  FT_بنسټ_DEF( FT_UInt32 )
  FT_بهیر_Read_ULong_LE( FT_بهیر  بهیر,
                         FT_Error*  error )
  {
    FT_Byte   reads[4];
    FT_Byte*  p;
    FT_UInt32 پایله = 0;


    FT_ASSERT( بهیر );

    إذا ( بهیر->pos + 3 < بهیر->اندازه )
    {
      إذا ( بهیر->اقرأ )
      {
        إذا ( بهیر->اقرأ( بهیر, بهیر->pos, reads, 4L ) != 4L )
          اذهب Fail;

        p = reads;
      }
      وإلا
        p = بهیر->بنسټ + بهیر->pos;

      إذا ( p )
        پایله = FT_بل_ULONG_LE( p );
    }
    وإلا
      اذهب Fail;

    بهیر->pos += 4;

    *error = FT_Err_Ok;

    أرجع پایله;

  Fail:
    *error = FT_THROW( Invalid_بهیر_Operation );
    FT_ERROR(( "FT_Stream_ReadULongLE:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               بهیر->pos, بهیر->اندازه ));

    أرجع پایله;
  }


  FT_بنسټ_DEF( FT_Error )
  FT_بهیر_Read_Fields( FT_بهیر              بهیر,
                        ثابت FT_چوکاټ_ډګر*  fields,
                        فراغ*                  structure )
  {
    FT_Error  error;
    FT_Bool   چوکاټ_accessed = 0;
    FT_Byte*  cursor;


    إذا ( !fields )
      أرجع FT_THROW( Invalid_دلیل );

    إذا ( !بهیر )
      أرجع FT_THROW( Invalid_بهیر_Handle );

    cursor = بهیر->cursor;

    error = FT_Err_Ok;
    افعل
    {
      FT_ULong  ارزښت;
      FT_Int    sign_ښويدنه;
      FT_Byte*  p;


      بدل ( fields->ارزښت )
      {
      حالة ft_چوکاټ_پیل:  /* access a new frame */
        error = FT_بهیر_Enter_چوکاټ( بهیر, fields->دننه_واټن );
        إذا ( error )
          اذهب Exit;

        چوکاټ_accessed = 1;
        cursor         = بهیر->cursor;
        fields++;
        استمر;  /* loop! */

      حالة ft_چوکاټ_بایټونه:  /* read a byte sequence */
      حالة ft_چوکاټ_skip:   /* skip some bytes      */
        {
          FT_دننه_واټن_2  len = fields->اندازه;


          إذا ( len > (FT_دننه_واټن_2)( بهیر->حد - cursor ) )
          {
            error = FT_THROW( Invalid_بهیر_Operation );
            اذهب Exit;
          }

          إذا ( fields->ارزښت == ft_چوکاټ_بایټونه )
          {
            p = (FT_Byte*)structure + fields->دننه_واټن;
            FT_MEM_COPY( p, cursor, len );
          }
          cursor += len;
          fields++;
          استمر;
        }

      حالة ft_چوکاټ_byte:
      حالة ft_چوکاټ_schar:  /* read a single byte */
        ارزښت = FT_بل_BYTE( cursor );
        sign_ښويدنه = 24;
        اكسر;

      حالة ft_چوکاټ_short_be:
      حالة ft_چوکاټ_ushort_be:  /* read a 2-byte big-endian short */
        ارزښت = FT_بل_USHORT( cursor );
        sign_ښويدنه = 16;
        اكسر;

      حالة ft_چوکاټ_short_le:
      حالة ft_چوکاټ_ushort_le:  /* read a 2-byte little-endian short */
        ارزښت = FT_بل_USHORT_LE( cursor );
        sign_ښويدنه = 16;
        اكسر;

      حالة ft_چوکاټ_long_be:
      حالة ft_چوکاټ_ulong_be:  /* read a 4-byte big-endian long */
        ارزښت = FT_بل_ULONG( cursor );
        sign_ښويدنه = 0;
        اكسر;

      حالة ft_چوکاټ_long_le:
      حالة ft_چوکاټ_ulong_le:  /* read a 4-byte little-endian long */
        ارزښت = FT_بل_ULONG_LE( cursor );
        sign_ښويدنه = 0;
        اكسر;

      حالة ft_چوکاټ_off3_be:
      حالة ft_چوکاټ_uoff3_be:  /* read a 3-byte big-endian long */
        ارزښت = FT_بل_UOFF3( cursor );
        sign_ښويدنه = 8;
        اكسر;

      حالة ft_چوکاټ_off3_le:
      حالة ft_چوکاټ_uoff3_le:  /* read a 3-byte little-endian long */
        ارزښت = FT_بل_UOFF3_LE( cursor );
        sign_ښويدنه = 8;
        اكسر;

      افتراضي:
        /* otherwise, exit the loop */
        بهیر->cursor = cursor;
        اذهب Exit;
      }

      /* now, compute the signed value if necessary */
      إذا ( fields->ارزښت & FT_چوکاټ_OP_SIGNED )
        ارزښت = (FT_ULong)( (FT_Int32)( ارزښت << sign_ښويدنه ) >> sign_ښويدنه );

      /* finally, store the value in the object */

      p = (FT_Byte*)structure + fields->دننه_واټن;
      بدل ( fields->اندازه )
      {
      حالة ( 8 / FT_CHAR_بټ ):
        *(FT_Byte*)p = (FT_Byte)ارزښت;
        اكسر;

      حالة ( 16 / FT_CHAR_بټ ):
        *(FT_UShort*)p = (FT_UShort)ارزښت;
        اكسر;

      حالة ( 32 / FT_CHAR_بټ ):
        *(FT_UInt32*)p = (FT_UInt32)ارزښت;
        اكسر;

      افتراضي:  /* for 64-bit systems */
        *(FT_ULong*)p = (FT_ULong)ارزښت;
      }

      /* go to next field */
      fields++;
    }
    طالما ( 1 );

  Exit:
    /* close the frame if it was opened by this read */
    إذا ( چوکاټ_accessed )
      FT_بهیر_Exit_چوکاټ( بهیر );

    أرجع error;
  }


/*پای_ټکی*/
