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


#అన్తర్భావ <freetype/internal/ftstream.h>
#అన్తర్భావ <freetype/internal/ftdebug.h>


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#నిర్వచనం_రద్దు  FT_COMPONENT
#నిర్వచించు FT_COMPONENT  ప్రవాహం


  FT_పునాది_DEF( శూన్య )
  FT_ప్రవాహం_Open_జ్ఞాపకం( FT_ప్రవాహం       ప్రవాహం,
                        స్థిరరాశి FT_Byte*  పునాది,
                        FT_ULong        కొలత )
  {
    ప్రవాహం->పునాది   = (FT_Byte*) పునాది;
    ప్రవాహం->కొలత   = కొలత;
    ప్రవాహం->pos    = 0;
    ప్రవాహం->cursor = ఖాళీ;
    ప్రవాహం->పఠ   = ఖాళీ;
    ప్రవాహం->close  = ఖాళీ;
  }


  FT_పునాది_DEF( శూన్య )
  FT_ప్రవాహం_Close( FT_ప్రవాహం  ప్రవాహం )
  {
    ఒకవేళ ( ప్రవాహం && ప్రవాహం->close )
      ప్రవాహం->close( ప్రవాహం );
  }


  FT_పునాది_DEF( FT_Error )
  FT_ప్రవాహం_Seek( FT_ప్రవాహం  ప్రవాహం,
                  FT_ULong   pos )
  {
    FT_Error  error = FT_Err_Ok;


    ఒకవేళ ( ప్రవాహం->పఠ )
    {
      ఒకవేళ ( ప్రవాహం->పఠ( ప్రవాహం, pos, ఖాళీ, 0 ) )
      {
        FT_ERROR(( "FT_Stream_Seek:"
                   " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
                   pos, ప్రవాహం->కొలత ));

        error = FT_THROW( Invalid_ప్రవాహం_Operation );
      }
    }
    /* note that seeking to the first position after the file is valid */
    లేకపోతే ఒకవేళ ( pos > ప్రవాహం->కొలత )
    {
      FT_ERROR(( "FT_Stream_Seek:"
                 " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
                 pos, ప్రవాహం->కొలత ));

      error = FT_THROW( Invalid_ప్రవాహం_Operation );
    }

    ఒకవేళ ( !error )
      ప్రవాహం->pos = pos;

    తిరిగి error;
  }


  FT_పునాది_DEF( FT_Error )
  FT_ప్రవాహం_Skip( FT_ప్రవాహం  ప్రవాహం,
                  FT_Long    దూరం )
  {
    ఒకవేళ ( దూరం < 0 )
      తిరిగి FT_THROW( Invalid_ప్రవాహం_Operation );

    తిరిగి FT_ప్రవాహం_Seek( ప్రవాహం, ప్రవాహం->pos + (FT_ULong)దూరం );
  }


  FT_పునాది_DEF( FT_ULong )
  FT_ప్రవాహం_Pos( FT_ప్రవాహం  ప్రవాహం )
  {
    తిరిగి ప్రవాహం->pos;
  }


  FT_పునాది_DEF( FT_Error )
  FT_ప్రవాహం_Read( FT_ప్రవాహం  ప్రవాహం,
                  FT_Byte*   బఫరు,
                  FT_ULong   లెక్క )
  {
    తిరిగి FT_ప్రవాహం_Read_At( ప్రవాహం, ప్రవాహం->pos, బఫరు, లెక్క );
  }


  FT_పునాది_DEF( FT_Error )
  FT_ప్రవాహం_Read_At( FT_ప్రవాహం  ప్రవాహం,
                    FT_ULong   pos,
                    FT_Byte*   బఫరు,
                    FT_ULong   లెక్క )
  {
    FT_Error  error = FT_Err_Ok;
    FT_ULong  read_బైట్లు;


    ఒకవేళ ( pos >= ప్రవాహం->కొలత )
    {
      FT_ERROR(( "FT_Stream_ReadAt:"
                 " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
                 pos, ప్రవాహం->కొలత ));

      తిరిగి FT_THROW( Invalid_ప్రవాహం_Operation );
    }

    ఒకవేళ ( ప్రవాహం->పఠ )
      read_బైట్లు = ప్రవాహం->పఠ( ప్రవాహం, pos, బఫరు, లెక్క );
    లేకపోతే
    {
      read_బైట్లు = ప్రవాహం->కొలత - pos;
      ఒకవేళ ( read_బైట్లు > లెక్క )
        read_బైట్లు = లెక్క;

      /* Allow "reading" zero bytes without UB even if buffer is NULL */
      ఒకవేళ ( లెక్క )
        FT_MEM_COPY( బఫరు, ప్రవాహం->పునాది + pos, read_బైట్లు );
    }

    ప్రవాహం->pos = pos + read_బైట్లు;

    ఒకవేళ ( read_బైట్లు < లెక్క )
    {
      FT_ERROR(( "FT_Stream_ReadAt:"
                 " invalid read; expected %lu bytes, got %lu\n",
                 లెక్క, read_బైట్లు ));

      error = FT_THROW( Invalid_ప్రవాహం_Operation );
    }

    తిరిగి error;
  }


  FT_పునాది_DEF( FT_ULong )
  FT_ప్రవాహం_Try_Read( FT_ప్రవాహం  ప్రవాహం,
                     FT_Byte*   బఫరు,
                     FT_ULong   లెక్క )
  {
    FT_ULong  read_బైట్లు = 0;


    ఒకవేళ ( ప్రవాహం->pos >= ప్రవాహం->కొలత )
      వెళ్ళు Exit;

    ఒకవేళ ( ప్రవాహం->పఠ )
      read_బైట్లు = ప్రవాహం->పఠ( ప్రవాహం, ప్రవాహం->pos, బఫరు, లెక్క );
    లేకపోతే
    {
      read_బైట్లు = ప్రవాహం->కొలత - ప్రవాహం->pos;
      ఒకవేళ ( read_బైట్లు > లెక్క )
        read_బైట్లు = లెక్క;

      /* Allow "reading" zero bytes without UB even if buffer is NULL */
      ఒకవేళ ( లెక్క )
        FT_MEM_COPY( బఫరు, ప్రవాహం->పునాది + ప్రవాహం->pos, read_బైట్లు );
    }

    ప్రవాహం->pos += read_బైట్లు;

  Exit:
    తిరిగి read_బైట్లు;
  }


  FT_పునాది_DEF( FT_Error )
  FT_ప్రవాహం_వెలికితీయి_చట్రం( FT_ప్రవాహం  ప్రవాహం,
                          FT_ULong   లెక్క,
                          FT_Byte**  pbytes )
  {
    FT_Error  error;


    error = FT_ప్రవాహం_Enter_చట్రం( ప్రవాహం, లెక్క );
    ఒకవేళ ( !error )
    {
      *pbytes = (FT_Byte*)ప్రవాహం->cursor;

      /* equivalent to FT_Stream_ExitFrame(), with no memory block release */
      ప్రవాహం->cursor = ఖాళీ;
      ప్రవాహం->హద్దు  = ఖాళీ;
    }

    తిరిగి error;
  }


  FT_పునాది_DEF( శూన్య )
  FT_ప్రవాహం_Release_చట్రం( FT_ప్రవాహం  ప్రవాహం,
                          FT_Byte**  pbytes )
  {
    ఒకవేళ ( ప్రవాహం && ప్రవాహం->పఠ )
    {
      FT_జ్ఞాపకం  జ్ఞాపకం = ప్రవాహం->జ్ఞాపకం;


#నిర్వచితమైతే FT_DEBUG_జ్ఞాపకం
      ft_mem_free( జ్ఞాపకం, *pbytes );
#లేకపోతే
      FT_FREE( *pbytes );
#అయితే_ముగింపు
    }

    *pbytes = ఖాళీ;
  }


  FT_పునాది_DEF( FT_Error )
  FT_ప్రవాహం_Enter_చట్రం( FT_ప్రవాహం  ప్రవాహం,
                        FT_ULong   లెక్క )
  {
    FT_Error  error = FT_Err_Ok;
    FT_ULong  read_బైట్లు;


    FT_TRACE7(( "FT_Stream_EnterFrame: %lu bytes\n", లెక్క ));

    /* check for nested frame access */
    FT_ASSERT( ప్రవాహం && ప్రవాహం->cursor == 0 );

    ఒకవేళ ( ప్రవాహం->పఠ )
    {
      /* allocate the frame in memory */
      FT_జ్ఞాపకం  జ్ఞాపకం = ప్రవాహం->జ్ఞాపకం;


      /* simple sanity check */
      ఒకవేళ ( లెక్క > ప్రవాహం->కొలత )
      {
        FT_ERROR(( "FT_Stream_EnterFrame:"
                   " frame size (%lu) larger than stream size (%lu)\n",
                   లెక్క, ప్రవాహం->కొలత ));

        error = FT_THROW( Invalid_ప్రవాహం_Operation );
        వెళ్ళు Exit;
      }

#నిర్వచితమైతే FT_DEBUG_జ్ఞాపకం
      /* assume `ft_debug_file_` and `ft_debug_lineno_` are already set */
      ప్రవాహం->పునాది = (గుర్తులేని అక్షరం*)ft_mem_qalloc( జ్ఞాపకం,
                                                    (FT_Long)లెక్క,
                                                    &error );
      ఒకవేళ ( error )
        వెళ్ళు Exit;
#లేకపోతే
      ఒకవేళ ( FT_QALLOC( ప్రవాహం->పునాది, లెక్క ) )
        వెళ్ళు Exit;
#అయితే_ముగింపు
      /* read it */
      read_బైట్లు = ప్రవాహం->పఠ( ప్రవాహం, ప్రవాహం->pos,
                                 ప్రవాహం->పునాది, లెక్క );
      ఒకవేళ ( read_బైట్లు < లెక్క )
      {
        FT_ERROR(( "FT_Stream_EnterFrame:"
                   " invalid read; expected %lu bytes, got %lu\n",
                   లెక్క, read_బైట్లు ));

        FT_FREE( ప్రవాహం->పునాది );
        error = FT_THROW( Invalid_ప్రవాహం_Operation );
      }

      ప్రవాహం->cursor = ప్రవాహం->పునాది;
      ప్రవాహం->హద్దు  = FT_స్థానభ్రంశం( ప్రవాహం->cursor, లెక్క );
      ప్రవాహం->pos   += read_బైట్లు;
    }
    లేకపోతే
    {
      /* check current and new position */
      ఒకవేళ ( ప్రవాహం->pos >= ప్రవాహం->కొలత        ||
           ప్రవాహం->కొలత - ప్రవాహం->pos < లెక్క )
      {
        FT_ERROR(( "FT_Stream_EnterFrame:"
                   " invalid i/o; pos = 0x%lx, count = %lu, size = 0x%lx\n",
                   ప్రవాహం->pos, లెక్క, ప్రవాహం->కొలత ));

        error = FT_THROW( Invalid_ప్రవాహం_Operation );
        వెళ్ళు Exit;
      }

      /* set cursor */
      ప్రవాహం->cursor = ప్రవాహం->పునాది + ప్రవాహం->pos;
      ప్రవాహం->హద్దు  = ప్రవాహం->cursor + లెక్క;
      ప్రవాహం->pos   += లెక్క;
    }

  Exit:
    తిరిగి error;
  }


  FT_పునాది_DEF( శూన్య )
  FT_ప్రవాహం_Exit_చట్రం( FT_ప్రవాహం  ప్రవాహం )
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

    FT_ASSERT( ప్రవాహం );

    ఒకవేళ ( ప్రవాహం->పఠ )
    {
      FT_జ్ఞాపకం  జ్ఞాపకం = ప్రవాహం->జ్ఞాపకం;


#నిర్వచితమైతే FT_DEBUG_జ్ఞాపకం
      ft_mem_free( జ్ఞాపకం, ప్రవాహం->పునాది );
      ప్రవాహం->పునాది = ఖాళీ;
#లేకపోతే
      FT_FREE( ప్రవాహం->పునాది );
#అయితే_ముగింపు
    }

    ప్రవాహం->cursor = ఖాళీ;
    ప్రవాహం->హద్దు  = ఖాళీ;
  }


  FT_పునాది_DEF( FT_Byte )
  FT_ప్రవాహం_Get_Byte( FT_ప్రవాహం  ప్రవాహం )
  {
    FT_Byte  ఫలితం;


    FT_ASSERT( ప్రవాహం && ప్రవాహం->cursor );

    ఫలితం = 0;
    ఒకవేళ ( ప్రవాహం->cursor < ప్రవాహం->హద్దు )
      ఫలితం = *ప్రవాహం->cursor++;

    తిరిగి ఫలితం;
  }


  FT_పునాది_DEF( FT_UInt16 )
  FT_ప్రవాహం_Get_UShort( FT_ప్రవాహం  ప్రవాహం )
  {
    FT_Byte*   p;
    FT_UInt16  ఫలితం;


    FT_ASSERT( ప్రవాహం && ప్రవాహం->cursor );

    ఫలితం         = 0;
    p              = ప్రవాహం->cursor;
    ఒకవేళ ( p + 1 < ప్రవాహం->హద్దు )
      ఫలితం       = FT_తదుపరి_USHORT( p );
    ప్రవాహం->cursor = p;

    తిరిగి ఫలితం;
  }


  FT_పునాది_DEF( FT_UInt16 )
  FT_ప్రవాహం_Get_UShort_LE( FT_ప్రవాహం  ప్రవాహం )
  {
    FT_Byte*   p;
    FT_UInt16  ఫలితం;


    FT_ASSERT( ప్రవాహం && ప్రవాహం->cursor );

    ఫలితం         = 0;
    p              = ప్రవాహం->cursor;
    ఒకవేళ ( p + 1 < ప్రవాహం->హద్దు )
      ఫలితం       = FT_తదుపరి_USHORT_LE( p );
    ప్రవాహం->cursor = p;

    తిరిగి ఫలితం;
  }


  FT_పునాది_DEF( FT_UInt32 )
  FT_ప్రవాహం_Get_UOffset( FT_ప్రవాహం  ప్రవాహం )
  {
    FT_Byte*  p;
    FT_UInt32 ఫలితం;


    FT_ASSERT( ప్రవాహం && ప్రవాహం->cursor );

    ఫలితం         = 0;
    p              = ప్రవాహం->cursor;
    ఒకవేళ ( p + 2 < ప్రవాహం->హద్దు )
      ఫలితం       = FT_తదుపరి_UOFF3( p );
    ప్రవాహం->cursor = p;
    తిరిగి ఫలితం;
  }


  FT_పునాది_DEF( FT_UInt32 )
  FT_ప్రవాహం_Get_ULong( FT_ప్రవాహం  ప్రవాహం )
  {
    FT_Byte*  p;
    FT_UInt32 ఫలితం;


    FT_ASSERT( ప్రవాహం && ప్రవాహం->cursor );

    ఫలితం         = 0;
    p              = ప్రవాహం->cursor;
    ఒకవేళ ( p + 3 < ప్రవాహం->హద్దు )
      ఫలితం       = FT_తదుపరి_ULONG( p );
    ప్రవాహం->cursor = p;
    తిరిగి ఫలితం;
  }


  FT_పునాది_DEF( FT_UInt32 )
  FT_ప్రవాహం_Get_ULong_LE( FT_ప్రవాహం  ప్రవాహం )
  {
    FT_Byte*  p;
    FT_UInt32 ఫలితం;


    FT_ASSERT( ప్రవాహం && ప్రవాహం->cursor );

    ఫలితం         = 0;
    p              = ప్రవాహం->cursor;
    ఒకవేళ ( p + 3 < ప్రవాహం->హద్దు )
      ఫలితం       = FT_తదుపరి_ULONG_LE( p );
    ప్రవాహం->cursor = p;
    తిరిగి ఫలితం;
  }


  FT_పునాది_DEF( FT_Byte )
  FT_ప్రవాహం_Read_Byte( FT_ప్రవాహం  ప్రవాహం,
                      FT_Error*  error )
  {
    FT_Byte  ఫలితం = 0;


    FT_ASSERT( ప్రవాహం );

    ఒకవేళ ( ప్రవాహం->pos < ప్రవాహం->కొలత )
    {
      ఒకవేళ ( ప్రవాహం->పఠ )
      {
        ఒకవేళ ( ప్రవాహం->పఠ( ప్రవాహం, ప్రవాహం->pos, &ఫలితం, 1L ) != 1L )
          వెళ్ళు Fail;
      }
      లేకపోతే
        ఫలితం = ప్రవాహం->పునాది[ప్రవాహం->pos];
    }
    లేకపోతే
      వెళ్ళు Fail;

    ప్రవాహం->pos++;

    *error = FT_Err_Ok;

    తిరిగి ఫలితం;

  Fail:
    *error = FT_THROW( Invalid_ప్రవాహం_Operation );
    FT_ERROR(( "FT_Stream_ReadByte:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               ప్రవాహం->pos, ప్రవాహం->కొలత ));

    తిరిగి ఫలితం;
  }


  FT_పునాది_DEF( FT_UInt16 )
  FT_ప్రవాహం_Read_UShort( FT_ప్రవాహం  ప్రవాహం,
                        FT_Error*  error )
  {
    FT_Byte    reads[2];
    FT_Byte*   p;
    FT_UInt16  ఫలితం = 0;


    FT_ASSERT( ప్రవాహం );

    ఒకవేళ ( ప్రవాహం->pos + 1 < ప్రవాహం->కొలత )
    {
      ఒకవేళ ( ప్రవాహం->పఠ )
      {
        ఒకవేళ ( ప్రవాహం->పఠ( ప్రవాహం, ప్రవాహం->pos, reads, 2L ) != 2L )
          వెళ్ళు Fail;

        p = reads;
      }
      లేకపోతే
        p = ప్రవాహం->పునాది + ప్రవాహం->pos;

      ఒకవేళ ( p )
        ఫలితం = FT_తదుపరి_USHORT( p );
    }
    లేకపోతే
      వెళ్ళు Fail;

    ప్రవాహం->pos += 2;

    *error = FT_Err_Ok;

    తిరిగి ఫలితం;

  Fail:
    *error = FT_THROW( Invalid_ప్రవాహం_Operation );
    FT_ERROR(( "FT_Stream_ReadUShort:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               ప్రవాహం->pos, ప్రవాహం->కొలత ));

    తిరిగి ఫలితం;
  }


  FT_పునాది_DEF( FT_UInt16 )
  FT_ప్రవాహం_Read_UShort_LE( FT_ప్రవాహం  ప్రవాహం,
                          FT_Error*  error )
  {
    FT_Byte    reads[2];
    FT_Byte*   p;
    FT_UInt16  ఫలితం = 0;


    FT_ASSERT( ప్రవాహం );

    ఒకవేళ ( ప్రవాహం->pos + 1 < ప్రవాహం->కొలత )
    {
      ఒకవేళ ( ప్రవాహం->పఠ )
      {
        ఒకవేళ ( ప్రవాహం->పఠ( ప్రవాహం, ప్రవాహం->pos, reads, 2L ) != 2L )
          వెళ్ళు Fail;

        p = reads;
      }
      లేకపోతే
        p = ప్రవాహం->పునాది + ప్రవాహం->pos;

      ఒకవేళ ( p )
        ఫలితం = FT_తదుపరి_USHORT_LE( p );
    }
    లేకపోతే
      వెళ్ళు Fail;

    ప్రవాహం->pos += 2;

    *error = FT_Err_Ok;

    తిరిగి ఫలితం;

  Fail:
    *error = FT_THROW( Invalid_ప్రవాహం_Operation );
    FT_ERROR(( "FT_Stream_ReadUShortLE:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               ప్రవాహం->pos, ప్రవాహం->కొలత ));

    తిరిగి ఫలితం;
  }


  FT_పునాది_DEF( FT_ULong )
  FT_ప్రవాహం_Read_UOffset( FT_ప్రవాహం  ప్రవాహం,
                         FT_Error*  error )
  {
    FT_Byte   reads[3];
    FT_Byte*  p;
    FT_ULong  ఫలితం = 0;


    FT_ASSERT( ప్రవాహం );

    ఒకవేళ ( ప్రవాహం->pos + 2 < ప్రవాహం->కొలత )
    {
      ఒకవేళ ( ప్రవాహం->పఠ )
      {
        ఒకవేళ (ప్రవాహం->పఠ( ప్రవాహం, ప్రవాహం->pos, reads, 3L ) != 3L )
          వెళ్ళు Fail;

        p = reads;
      }
      లేకపోతే
        p = ప్రవాహం->పునాది + ప్రవాహం->pos;

      ఒకవేళ ( p )
        ఫలితం = FT_తదుపరి_UOFF3( p );
    }
    లేకపోతే
      వెళ్ళు Fail;

    ప్రవాహం->pos += 3;

    *error = FT_Err_Ok;

    తిరిగి ఫలితం;

  Fail:
    *error = FT_THROW( Invalid_ప్రవాహం_Operation );
    FT_ERROR(( "FT_Stream_ReadUOffset:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               ప్రవాహం->pos, ప్రవాహం->కొలత ));

    తిరిగి ఫలితం;
  }


  FT_పునాది_DEF( FT_UInt32 )
  FT_ప్రవాహం_Read_ULong( FT_ప్రవాహం  ప్రవాహం,
                       FT_Error*  error )
  {
    FT_Byte   reads[4];
    FT_Byte*  p;
    FT_UInt32 ఫలితం = 0;


    FT_ASSERT( ప్రవాహం );

    ఒకవేళ ( ప్రవాహం->pos + 3 < ప్రవాహం->కొలత )
    {
      ఒకవేళ ( ప్రవాహం->పఠ )
      {
        ఒకవేళ ( ప్రవాహం->పఠ( ప్రవాహం, ప్రవాహం->pos, reads, 4L ) != 4L )
          వెళ్ళు Fail;

        p = reads;
      }
      లేకపోతే
        p = ప్రవాహం->పునాది + ప్రవాహం->pos;

      ఒకవేళ ( p )
        ఫలితం = FT_తదుపరి_ULONG( p );
    }
    లేకపోతే
      వెళ్ళు Fail;

    ప్రవాహం->pos += 4;

    *error = FT_Err_Ok;

    తిరిగి ఫలితం;

  Fail:
    *error = FT_THROW( Invalid_ప్రవాహం_Operation );
    FT_ERROR(( "FT_Stream_ReadULong:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               ప్రవాహం->pos, ప్రవాహం->కొలత ));

    తిరిగి ఫలితం;
  }


  FT_పునాది_DEF( FT_UInt32 )
  FT_ప్రవాహం_Read_ULong_LE( FT_ప్రవాహం  ప్రవాహం,
                         FT_Error*  error )
  {
    FT_Byte   reads[4];
    FT_Byte*  p;
    FT_UInt32 ఫలితం = 0;


    FT_ASSERT( ప్రవాహం );

    ఒకవేళ ( ప్రవాహం->pos + 3 < ప్రవాహం->కొలత )
    {
      ఒకవేళ ( ప్రవాహం->పఠ )
      {
        ఒకవేళ ( ప్రవాహం->పఠ( ప్రవాహం, ప్రవాహం->pos, reads, 4L ) != 4L )
          వెళ్ళు Fail;

        p = reads;
      }
      లేకపోతే
        p = ప్రవాహం->పునాది + ప్రవాహం->pos;

      ఒకవేళ ( p )
        ఫలితం = FT_తదుపరి_ULONG_LE( p );
    }
    లేకపోతే
      వెళ్ళు Fail;

    ప్రవాహం->pos += 4;

    *error = FT_Err_Ok;

    తిరిగి ఫలితం;

  Fail:
    *error = FT_THROW( Invalid_ప్రవాహం_Operation );
    FT_ERROR(( "FT_Stream_ReadULongLE:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               ప్రవాహం->pos, ప్రవాహం->కొలత ));

    తిరిగి ఫలితం;
  }


  FT_పునాది_DEF( FT_Error )
  FT_ప్రవాహం_Read_Fields( FT_ప్రవాహం              ప్రవాహం,
                        స్థిరరాశి FT_చట్రం_క్షేత్రం*  fields,
                        శూన్య*                  structure )
  {
    FT_Error  error;
    FT_Bool   చట్రం_accessed = 0;
    FT_Byte*  cursor;


    ఒకవేళ ( !fields )
      తిరిగి FT_THROW( Invalid_వాదం );

    ఒకవేళ ( !ప్రవాహం )
      తిరిగి FT_THROW( Invalid_ప్రవాహం_Handle );

    cursor = ప్రవాహం->cursor;

    error = FT_Err_Ok;
    చేయి
    {
      FT_ULong  విలువ;
      FT_Int    sign_జరుపు;
      FT_Byte*  p;


      ఎంపిక ( fields->విలువ )
      {
      స్థితి ft_చట్రం_ప్రారంభం:  /* access a new frame */
        error = FT_ప్రవాహం_Enter_చట్రం( ప్రవాహం, fields->స్థానభ్రంశం );
        ఒకవేళ ( error )
          వెళ్ళు Exit;

        చట్రం_accessed = 1;
        cursor         = ప్రవాహం->cursor;
        fields++;
        కొనసాగు;  /* loop! */

      స్థితి ft_చట్రం_బైట్లు:  /* read a byte sequence */
      స్థితి ft_చట్రం_skip:   /* skip some bytes      */
        {
          FT_స్థానభ్రంశం_2  len = fields->కొలత;


          ఒకవేళ ( len > (FT_స్థానభ్రంశం_2)( ప్రవాహం->హద్దు - cursor ) )
          {
            error = FT_THROW( Invalid_ప్రవాహం_Operation );
            వెళ్ళు Exit;
          }

          ఒకవేళ ( fields->విలువ == ft_చట్రం_బైట్లు )
          {
            p = (FT_Byte*)structure + fields->స్థానభ్రంశం;
            FT_MEM_COPY( p, cursor, len );
          }
          cursor += len;
          fields++;
          కొనసాగు;
        }

      స్థితి ft_చట్రం_byte:
      స్థితి ft_చట్రం_schar:  /* read a single byte */
        విలువ = FT_తదుపరి_BYTE( cursor );
        sign_జరుపు = 24;
        ఆపు;

      స్థితి ft_చట్రం_short_be:
      స్థితి ft_చట్రం_ushort_be:  /* read a 2-byte big-endian short */
        విలువ = FT_తదుపరి_USHORT( cursor );
        sign_జరుపు = 16;
        ఆపు;

      స్థితి ft_చట్రం_short_le:
      స్థితి ft_చట్రం_ushort_le:  /* read a 2-byte little-endian short */
        విలువ = FT_తదుపరి_USHORT_LE( cursor );
        sign_జరుపు = 16;
        ఆపు;

      స్థితి ft_చట్రం_long_be:
      స్థితి ft_చట్రం_ulong_be:  /* read a 4-byte big-endian long */
        విలువ = FT_తదుపరి_ULONG( cursor );
        sign_జరుపు = 0;
        ఆపు;

      స్థితి ft_చట్రం_long_le:
      స్థితి ft_చట్రం_ulong_le:  /* read a 4-byte little-endian long */
        విలువ = FT_తదుపరి_ULONG_LE( cursor );
        sign_జరుపు = 0;
        ఆపు;

      స్థితి ft_చట్రం_off3_be:
      స్థితి ft_చట్రం_uoff3_be:  /* read a 3-byte big-endian long */
        విలువ = FT_తదుపరి_UOFF3( cursor );
        sign_జరుపు = 8;
        ఆపు;

      స్థితి ft_చట్రం_off3_le:
      స్థితి ft_చట్రం_uoff3_le:  /* read a 3-byte little-endian long */
        విలువ = FT_తదుపరి_UOFF3_LE( cursor );
        sign_జరుపు = 8;
        ఆపు;

      అప్రమేయ:
        /* otherwise, exit the loop */
        ప్రవాహం->cursor = cursor;
        వెళ్ళు Exit;
      }

      /* now, compute the signed value if necessary */
      ఒకవేళ ( fields->విలువ & FT_చట్రం_OP_SIGNED )
        విలువ = (FT_ULong)( (FT_Int32)( విలువ << sign_జరుపు ) >> sign_జరుపు );

      /* finally, store the value in the object */

      p = (FT_Byte*)structure + fields->స్థానభ్రంశం;
      ఎంపిక ( fields->కొలత )
      {
      స్థితి ( 8 / FT_CHAR_బిట్టు ):
        *(FT_Byte*)p = (FT_Byte)విలువ;
        ఆపు;

      స్థితి ( 16 / FT_CHAR_బిట్టు ):
        *(FT_UShort*)p = (FT_UShort)విలువ;
        ఆపు;

      స్థితి ( 32 / FT_CHAR_బిట్టు ):
        *(FT_UInt32*)p = (FT_UInt32)విలువ;
        ఆపు;

      అప్రమేయ:  /* for 64-bit systems */
        *(FT_ULong*)p = (FT_ULong)విలువ;
      }

      /* go to next field */
      fields++;
    }
    అయితే ( 1 );

  Exit:
    /* close the frame if it was opened by this read */
    ఒకవేళ ( చట్రం_accessed )
      FT_ప్రవాహం_Exit_చట్రం( ప్రవాహం );

    తిరిగి error;
  }


/*చివర*/
