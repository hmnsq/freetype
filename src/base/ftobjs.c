/****************************************************************************
 *
 * ftobjs.c
 *
 *   The FreeType private base classes (body).
 *
 * Copyright (C) 1996-2026 by
 * David Turner, Robert Wilhelm, and Werner Lemberg.
 *
 * This file is part of the FreeType project, and may only be used,
 * modified, and distributed under the terms of the FreeType project
 * license, LICENSE.TXT.  By continuing to use, modify, or distribute
 * this file you indicate that you have read the license and
 * understand and accept it fully.
 *
 */


#شامل <freetype/ftlist.h>
#شامل <freetype/ftoutln.h>
#شامل <freetype/ftfntfmt.h>
#شامل <freetype/otsvg.h>

#شامل <freetype/internal/ftvalid.h>
#شامل <freetype/internal/ftobjs.h>
#شامل <freetype/internal/ftdebug.h>
#شامل <freetype/internal/ftrfork.h>
#شامل <freetype/internal/ftstream.h>
#شامل <freetype/internal/sfnt.h>          /* for SFNT_Load_Table_Func */
#شامل <freetype/internal/psaux.h>         /* for PS_Driver            */
#شامل <freetype/internal/svginterface.h>

#شامل <freetype/tttables.h>
#شامل <freetype/tttags.h>
#شامل <freetype/ttnameid.h>

#شامل <freetype/internal/services/svprop.h>
#شامل <freetype/internal/services/svsfnt.h>
#شامل <freetype/internal/services/svpostnm.h>
#شامل <freetype/internal/services/svgldict.h>
#شامل <freetype/internal/services/svttcmap.h>
#شامل <freetype/internal/services/svkern.h>
#شامل <freetype/internal/services/svtteng.h>

#ifdef TT_CONFIG_OPTION_VARC
#شامل <freetype/internal/services/svvarc.h>
#endif

#شامل <freetype/ftdriver.h>

#ifdef FT_CONFIG_OPTION_MAC_FONTS
#شامل "ftbase.h"
#endif


#ifdef FT_DEBUG_LEVEL_TRACE

#شامل <freetype/ftbitmap.h>

#اگر defined( _MSC_VER )      /* Visual C++ (and Intel C++)   */
  /* We disable the warning `conversion from XXX to YYY,     */
  /* possible loss of data' in order to compile cleanly with */
  /* the maximum level of warnings: `md5.c' is non-FreeType  */
  /* code, and it gets used during development builds only.  */
#pragma warning( افزودن )
#pragma warning( disable : 4244 )
#endif /* _MSC_VER */

  /* It's easiest to include `md5.c' directly.  However, since OpenSSL */
  /* also provides the same functions, there might be conflicts if     */
  /* both FreeType and OpenSSL are built as static libraries.  For     */
  /* this reason, we put the MD5 stuff into the `FT_' namespace.       */
#define MD5_u32plus  FT_MD5_u32plus
#define MD5_CTX      FT_MD5_CTX
#define MD5_Init     FT_MD5_Init
#define MD5_Update   FT_MD5_Update
#define MD5_Final    FT_MD5_Final

#undef  HAVE_OPENSSL

#شامل "md5.c"

#اگر defined( _MSC_VER )
#pragma warning( برداشت )
#endif

  /* This array must stay in sync with the @FT_Pixel_Mode enumeration */
  /* (in file `ftimage.h`).                                           */

  ایستا ثابت نویسه* ثابت  pixel_modes[] =
  {
    "none",
    "monochrome bitmap",
    "gray 8-bit bitmap",
    "gray 2-bit bitmap",
    "gray 4-bit bitmap",
    "LCD 8-bit bitmap",
    "vertical LCD 8-bit bitmap",
    "BGRA 32-bit color image bitmap",
    "SDF 8-bit bitmap"
  };

#endif /* FT_DEBUG_LEVEL_TRACE */


#define GRID_FIT_METRICS


  /* forward declaration */
  ایستا FT_Error
  ft_open_face_internal( FT_Library           library,
                         ثابت FT_Open_Args*  args,
                         FT_Long              face_index,
                         FT_Face             *aface,
                         FT_Bool              test_mac_fonts );


  FT_BASE_DEF( FT_Pointer )
  ft_service_list_lookup( FT_ServiceDesc  service_descriptors,
                          ثابت نویسه*     service_id )
  {
    FT_Pointer      result = پوچ;
    FT_ServiceDesc  desc   = service_descriptors;


    اگر ( desc && service_id )
    {
      برای ( ; desc->serv_id != پوچ; desc++ )
      {
        اگر ( ft_strcmp( desc->serv_id, service_id ) == 0 )
        {
          result = (FT_Pointer)desc->serv_data;
          بشکن;
        }
      }
    }

    بازگشت result;
  }


  FT_BASE_DEF( تهی )
  ft_validator_init( FT_Validator        valid,
                     ثابت FT_Byte*      base,
                     ثابت FT_Byte*      limit,
                     FT_ValidationLevel  level )
  {
    valid->base  = base;
    valid->limit = limit;
    valid->level = level;
    valid->error = FT_Err_Ok;
  }


  FT_BASE_DEF( FT_Int )
  ft_validator_run( FT_Validator  valid )
  {
    /* This function doesn't work!  None should call it. */
    FT_UNUSED( valid );

    بازگشت -1;
  }


  FT_BASE_DEF( تهی )
  ft_validator_error( FT_Validator  valid,
                      FT_Error      error )
  {
    /* since the cast below also disables the compiler's */
    /* type check, we introduce a dummy variable, which  */
    /* will be optimized away                            */
    فرار ft_jmp_buf* jump_buffer = &valid->jump_buffer;


    valid->error = error;

    /* throw away volatileness; use `jump_buffer' or the  */
    /* compiler may warn about an unused local variable   */
    ft_longjmp( *(ft_jmp_buf*) jump_buffer, 1 );
  }


  /*************************************************************************/
  /*************************************************************************/
  /*************************************************************************/
  /****                                                                 ****/
  /****                                                                 ****/
  /****                           S T R E A M                           ****/
  /****                                                                 ****/
  /****                                                                 ****/
  /*************************************************************************/
  /*************************************************************************/
  /*************************************************************************/


  /* create a new input stream from an FT_Open_Args structure */
  /*                                                          */
  FT_BASE_DEF( FT_Error )
  FT_Stream_New( FT_Library           library,
                 ثابت FT_Open_Args*  args,
                 FT_Stream           *astream )
  {
    FT_Error   error;
    FT_Memory  memory;
    FT_Stream  stream = پوچ;
    FT_UInt    mode;


    *astream = پوچ;

    اگر ( !library )
      بازگشت FT_THROW( Invalid_Library_Handle );

    اگر ( !args )
      بازگشت FT_THROW( Invalid_Argument );

    memory = library->memory;
    mode   = args->flags &
               ( FT_OPEN_MEMORY | FT_OPEN_STREAM | FT_OPEN_PATHNAME );

    اگر ( mode == FT_OPEN_MEMORY )
    {
      /* create a memory-based stream */
      اگر ( FT_NEW( stream ) )
        برو Exit;

      FT_Stream_OpenMemory( stream,
                            (ثابت FT_Byte*)args->memory_base,
                            (FT_ULong)args->memory_size );
      stream->memory = memory;
    }

#ifndef FT_CONFIG_OPTION_DISABLE_STREAM_SUPPORT

    وگرنه اگر ( mode == FT_OPEN_PATHNAME )
    {
      /* create a normal system stream */
      اگر ( FT_NEW( stream ) )
        برو Exit;

      stream->memory = memory;
      error = FT_Stream_Open( stream, args->pathname );
      اگر ( error )
        FT_FREE( stream );
    }
    وگرنه اگر ( ( mode == FT_OPEN_STREAM ) && args->stream )
    {
      /* use an existing, user-provided stream */

      /* in this case, we do not need to allocate a new stream object */
      /* since the caller is responsible for closing it himself       */
      stream         = args->stream;
      stream->memory = memory;
      error          = FT_Err_Ok;
    }

#endif

    وگرنه
    {
      error = FT_THROW( Invalid_Argument );
      اگر ( ( args->flags & FT_OPEN_STREAM ) && args->stream )
        FT_Stream_Close( args->stream );
    }

    اگر ( !error )
      *astream       = stream;

  Exit:
    بازگشت error;
  }


  FT_BASE_DEF( تهی )
  FT_Stream_Free( FT_Stream  stream,
                  FT_Int     external )
  {
    اگر ( stream )
    {
      FT_Memory  memory = stream->memory;


      FT_Stream_Close( stream );

      اگر ( !external )
        FT_FREE( stream );
    }
  }


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#undef  FT_COMPONENT
#define FT_COMPONENT  objs


  /*************************************************************************/
  /*************************************************************************/
  /*************************************************************************/
  /****                                                                 ****/
  /****                                                                 ****/
  /****               FACE, SIZE & GLYPH SLOT OBJECTS                   ****/
  /****                                                                 ****/
  /****                                                                 ****/
  /*************************************************************************/
  /*************************************************************************/
  /*************************************************************************/


  ایستا FT_Error
  ft_glyphslot_init( FT_GlyphSlot  slot )
  {
    FT_Driver         driver   = slot->face->driver;
    FT_Driver_Class   clazz    = driver->clazz;
    FT_Memory         memory   = driver->ریشه.memory;
    FT_Error          error    = FT_Err_Ok;
    FT_Slot_Internal  internal = پوچ;


    slot->library = driver->ریشه.library;

    اگر ( FT_NEW( internal ) )
      برو Exit;

    slot->internal = internal;

    اگر ( FT_DRIVER_USES_OUTLINES( driver ) )
      error = FT_GlyphLoader_New( memory, &internal->loader );

    اگر ( !error && clazz->init_slot )
      error = clazz->init_slot( slot );

#ifdef FT_CONFIG_OPTION_SVG
    /* if SVG table exists, allocate the space in `slot->other` */
    اگر ( slot->face->face_flags & FT_FACE_FLAG_SVG )
    {
      FT_SVG_Document  document = پوچ;


      اگر ( FT_NEW( document ) )
        برو Exit;
      slot->other = document;
    }
#endif

  Exit:
    بازگشت error;
  }


  FT_BASE_DEF( تهی )
  ft_glyphslot_free_bitmap( FT_GlyphSlot  slot )
  {
    اگر ( slot->internal && ( slot->internal->flags & FT_GLYPH_OWN_BITMAP ) )
    {
      FT_Memory  memory = FT_FACE_MEMORY( slot->face );


      FT_FREE( slot->bitmap.buffer );
      slot->internal->flags &= ~FT_GLYPH_OWN_BITMAP;
    }
    وگرنه
    {
      /* assume that the bitmap buffer was stolen or not */
      /* allocated from the heap                         */
      slot->bitmap.buffer = پوچ;
    }
  }


  /* overflow-resistant presetting of bitmap position and dimensions; */
  /* also check whether the size is too large for rendering           */
  FT_BASE_DEF( FT_Bool )
  ft_glyphslot_preset_bitmap( FT_GlyphSlot      slot,
                              FT_Render_Mode    mode,
                              ثابت FT_Vector*  origin )
  {
    FT_Outline*  outline = &slot->outline;
    FT_Bitmap*   bitmap  = &slot->bitmap;

    FT_Pixel_Mode  pixel_mode;

    FT_BBox  cbox, pbox;
    FT_Pos   x_shift = 0;
    FT_Pos   y_shift = 0;
    FT_Pos   x_چپ, y_بالاترین;
    FT_Pos   width, height, pitch;
    FT_Bool  ret;


    اگر ( slot->format == FT_GLYPH_FORMAT_SVG )
    {
      FT_Module    module;
      SVG_Service  svg_service;


      module      = FT_Get_Module( slot->library, "ot-svg" );
      svg_service = (SVG_Service)module->clazz->module_interface;

      بازگشت (FT_Bool)svg_service->preset_slot( module, slot, FALSE );
    }
    وگرنه اگر ( slot->format != FT_GLYPH_FORMAT_OUTLINE )
      بازگشت 1;

    اگر ( origin )
    {
      x_shift = origin->x;
      y_shift = origin->y;
    }

    /* compute the control box, and grid-fit it, */
    /* taking into account the origin shift      */
    FT_Outline_Get_CBox( outline, &cbox );

    /* rough estimate of pixel box */
    pbox.xMin = ( cbox.xMin >> 6 ) + ( x_shift >> 6 );
    pbox.yMin = ( cbox.yMin >> 6 ) + ( y_shift >> 6 );
    pbox.xMax = ( cbox.xMax >> 6 ) + ( x_shift >> 6 );
    pbox.yMax = ( cbox.yMax >> 6 ) + ( y_shift >> 6 );

    /* tiny remainder box */
    cbox.xMin = ( cbox.xMin & 63 ) + ( x_shift & 63 );
    cbox.yMin = ( cbox.yMin & 63 ) + ( y_shift & 63 );
    cbox.xMax = ( cbox.xMax & 63 ) + ( x_shift & 63 );
    cbox.yMax = ( cbox.yMax & 63 ) + ( y_shift & 63 );

    انتخاب ( mode )
    {
    حالت FT_RENDER_MODE_MONO:
      pixel_mode = FT_PIXEL_MODE_MONO;
#اگر 1
      /* x */

      /* undocumented but confirmed: bbox values get rounded;    */
      /* we do asymmetric rounding so that the center of a pixel */
      /* gets always included                                    */

      pbox.xMin += ( cbox.xMin + 31 ) >> 6;
      pbox.xMax += ( cbox.xMax + 32 ) >> 6;

      /* if the bbox collapsed, we add a pixel based on the total */
      /* rounding remainder to cover most of the original cbox    */

      اگر ( pbox.xMin == pbox.xMax )
      {
        اگر ( ( ( cbox.xMin + 31 ) & 63 ) - 31 +
             ( ( cbox.xMax + 32 ) & 63 ) - 32 < 0 )
          pbox.xMin -= 1;
        وگرنه
          pbox.xMax += 1;
      }

      /* y */

      pbox.yMin += ( cbox.yMin + 31 ) >> 6;
      pbox.yMax += ( cbox.yMax + 32 ) >> 6;

      اگر ( pbox.yMin == pbox.yMax )
      {
        اگر ( ( ( cbox.yMin + 31 ) & 63 ) - 31 +
             ( ( cbox.yMax + 32 ) & 63 ) - 32 < 0 )
          pbox.yMin -= 1;
        وگرنه
          pbox.yMax += 1;
      }

      بشکن;
#وگرنه
      برو Adjust;
#endif

    حالت FT_RENDER_MODE_LCD:
      pixel_mode = FT_PIXEL_MODE_LCD;
      ft_lcd_padding( &cbox, slot, mode );
      برو Adjust;

    حالت FT_RENDER_MODE_LCD_V:
      pixel_mode = FT_PIXEL_MODE_LCD_V;
      ft_lcd_padding( &cbox, slot, mode );
      برو Adjust;

    حالت FT_RENDER_MODE_NORMAL:
    حالت FT_RENDER_MODE_LIGHT:
    پیشفرض:
      pixel_mode = FT_PIXEL_MODE_GRAY;
    Adjust:
      pbox.xMin += cbox.xMin >> 6;
      pbox.yMin += cbox.yMin >> 6;
      pbox.xMax += ( cbox.xMax + 63 ) >> 6;
      pbox.yMax += ( cbox.yMax + 63 ) >> 6;
    }

    x_چپ = pbox.xMin;
    y_بالاترین  = pbox.yMax;

    width  = pbox.xMax - pbox.xMin;
    height = pbox.yMax - pbox.yMin;

    /* Flag the width or height unsuitable for rendering.   */
    /* The limit is based on the ppem value when available. */
    /* FT_Renderer modules should check the return value.   */
    ret = FT_BOOL(     width >= 0x10000   ||    height >= 0x10000   ||
                   pbox.xMin < -0x1000000 || pbox.xMax >= 0x1000000 ||
                   pbox.yMin < -0x1000000 || pbox.yMax >= 0x1000000 ||
             ( slot->face                                         &&
               ( width  > 16 * slot->face->size->metrics.x_ppem ||
                 height > 16 * slot->face->size->metrics.y_ppem ) ) );

    اگر ( ret )
      FT_TRACE3(( "ft_glyphslot_preset_bitmap: [%ld %ld %ld %ld]\n",
                  pbox.xMin, pbox.yMin, pbox.xMax, pbox.yMax ));

    انتخاب ( pixel_mode )
    {
    حالت FT_PIXEL_MODE_MONO:
      pitch = ( ( width + 15 ) >> 4 ) << 1;
      بشکن;

    حالت FT_PIXEL_MODE_LCD:
      width *= 3;
      pitch  = FT_PAD_CEIL( width, 4 );
      بشکن;

    حالت FT_PIXEL_MODE_LCD_V:
      height *= 3;
      FALL_THROUGH;

    حالت FT_PIXEL_MODE_GRAY:
    پیشفرض:
      pitch = width;
    }

    slot->bitmap_چپ = (FT_Int)x_چپ;
    slot->bitmap_بالاترین  = (FT_Int)y_بالاترین;

    bitmap->pixel_mode = (unsigned نویسه)pixel_mode;
    bitmap->رقم_grays  = 256;
    bitmap->width      = (unsigned صحیح)width;
    bitmap->rows       = (unsigned صحیح)height;
    bitmap->pitch      = pitch;

    بازگشت ret;
  }


  FT_BASE_DEF( تهی )
  ft_glyphslot_set_bitmap( FT_GlyphSlot  slot,
                           FT_Byte*      buffer )
  {
    ft_glyphslot_free_bitmap( slot );

    slot->bitmap.buffer = buffer;

    FT_ASSERT( (slot->internal->flags & FT_GLYPH_OWN_BITMAP) == 0 );
  }


  FT_BASE_DEF( FT_Error )
  ft_glyphslot_alloc_bitmap( FT_GlyphSlot  slot )
  {
    FT_Memory  memory = FT_FACE_MEMORY( slot->face );
    FT_Error   error;


    اگر ( slot->internal->flags & FT_GLYPH_OWN_BITMAP )
      FT_FREE( slot->bitmap.buffer );
    وگرنه
      slot->internal->flags |= FT_GLYPH_OWN_BITMAP;

    /* dimensions must be preset */
    FT_MEM_ALLOC_MULT( slot->bitmap.buffer,
                       slot->bitmap.rows,
                       slot->bitmap.pitch );
    بازگشت error;
  }


  ایستا تهی
  ft_glyphslot_clear( FT_GlyphSlot  slot )
  {
    /* free bitmap if needed */
    ft_glyphslot_free_bitmap( slot );

    /* clear all public fields in the glyph slot */
    slot->glyph_index = 0;

    FT_ZERO( &slot->metrics );
    FT_ZERO( &slot->outline );

    slot->bitmap.width      = 0;
    slot->bitmap.rows       = 0;
    slot->bitmap.pitch      = 0;
    slot->bitmap.pixel_mode = 0;
    /* `slot->bitmap.buffer' has been handled by ft_glyphslot_free_bitmap */

    slot->bitmap_چپ   = 0;
    slot->bitmap_بالاترین    = 0;
    slot->رقم_subglyphs = 0;
    slot->subglyphs     = پوچ;
    slot->control_data  = پوچ;
    slot->control_len   = 0;

#ifndef FT_CONFIG_OPTION_SVG
    slot->other = پوچ;
#وگرنه
    اگر ( !( slot->face->face_flags & FT_FACE_FLAG_SVG ) )
      slot->other = پوچ;
    وگرنه
    {
      اگر ( slot->internal->flags & FT_GLYPH_OWN_GZIP_SVG )
      {
        FT_Memory        memory = slot->face->memory;
        FT_SVG_Document  doc    = (FT_SVG_Document)slot->other;


        FT_FREE( doc->svg_document );
        slot->internal->flags &= ~FT_GLYPH_OWN_GZIP_SVG;
      }
    }
#endif

    slot->format = FT_GLYPH_FORMAT_NONE;

    slot->linearHoriAdvance = 0;
    slot->linearVertAdvance = 0;
    slot->advance.x         = 0;
    slot->advance.y         = 0;
    slot->lsb_delta         = 0;
    slot->rsb_delta         = 0;
  }


  ایستا تهی
  ft_glyphslot_done( FT_GlyphSlot  slot )
  {
    FT_Driver        driver = slot->face->driver;
    FT_Driver_Class  clazz  = driver->clazz;
    FT_Memory        memory = driver->ریشه.memory;

#ifdef FT_CONFIG_OPTION_SVG
    اگر ( slot->face->face_flags & FT_FACE_FLAG_SVG )
    {
      /* Free memory in case SVG was there.                          */
      /* `slot->internal` might be NULL in out-of-memory situations. */
      اگر ( slot->internal && slot->internal->flags & FT_GLYPH_OWN_GZIP_SVG )
      {
        FT_SVG_Document  doc = (FT_SVG_Document)slot->other;


        FT_FREE( doc->svg_document );

        slot->internal->flags &= ~FT_GLYPH_OWN_GZIP_SVG;
      }

      FT_FREE( slot->other );
    }
#endif

    اگر ( clazz->done_slot )
      clazz->done_slot( slot );

    /* free bitmap buffer if needed */
    ft_glyphslot_free_bitmap( slot );

    /* slot->internal might be NULL in out-of-memory situations */
    اگر ( slot->internal )
    {
      /* free glyph loader */
      اگر ( FT_DRIVER_USES_OUTLINES( driver ) )
      {
        FT_GlyphLoader_Done( slot->internal->loader );
        slot->internal->loader = پوچ;
      }

      FT_FREE( slot->internal );
    }
  }


  /* documentation is in ftobjs.h */

  FT_BASE_DEF( FT_Error )
  FT_New_GlyphSlot( FT_Face        face,
                    FT_GlyphSlot  *aslot )
  {
    FT_Error         error;
    FT_Driver        driver;
    FT_Driver_Class  clazz;
    FT_Memory        memory;
    FT_GlyphSlot     slot = پوچ;


    اگر ( !face )
      بازگشت FT_THROW( Invalid_Face_Handle );

    اگر ( !face->driver )
      بازگشت FT_THROW( Invalid_Argument );

    driver = face->driver;
    clazz  = driver->clazz;
    memory = driver->ریشه.memory;

    FT_TRACE4(( "FT_New_GlyphSlot: Creating new slot object\n" ));
    اگر ( !FT_ALLOC( slot, clazz->slot_object_size ) )
    {
      slot->face = face;

      error = ft_glyphslot_init( slot );
      اگر ( error )
      {
        ft_glyphslot_done( slot );
        FT_FREE( slot );
        برو Exit;
      }

      slot->next  = face->glyph;
      face->glyph = slot;

      اگر ( aslot )
        *aslot = slot;
    }
    وگرنه اگر ( aslot )
      *aslot = پوچ;


  Exit:
    FT_TRACE4(( "FT_New_GlyphSlot: Return 0x%x\n", error ));

    بازگشت error;
  }


  /* documentation is in ftobjs.h */

  FT_BASE_DEF( تهی )
  FT_Done_GlyphSlot( FT_GlyphSlot  slot )
  {
    اگر ( slot )
    {
      FT_Driver     driver = slot->face->driver;
      FT_Memory     memory = driver->ریشه.memory;
      FT_GlyphSlot  prev;
      FT_GlyphSlot  cur;


      /* Remove slot from its parent face's list */
      prev = پوچ;
      cur  = slot->face->glyph;

      وقتی ( cur )
      {
        اگر ( cur == slot )
        {
          اگر ( !prev )
            slot->face->glyph = cur->next;
          وگرنه
            prev->next = cur->next;

          /* finalize client-specific data */
          اگر ( slot->generic.finalizer )
            slot->generic.finalizer( slot );

          ft_glyphslot_done( slot );
          FT_FREE( slot );
          بشکن;
        }
        prev = cur;
        cur  = cur->next;
      }
    }
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( تهی )
  FT_Set_Transform( FT_Face     face,
                    FT_Matrix*  matrix,
                    FT_Vector*  delta )
  {
    FT_Face_Internal  internal;


    اگر ( !face )
      بازگشت;

    internal = face->internal;

    internal->transform_flags = 0;

    اگر ( !matrix || !FT_Matrix_Check( matrix ) )
    {
      internal->transform_matrix.xx = 0x10000L;
      internal->transform_matrix.xy = 0;
      internal->transform_matrix.yx = 0;
      internal->transform_matrix.yy = 0x10000L;

      matrix = &internal->transform_matrix;
    }
    وگرنه
      internal->transform_matrix = *matrix;

    /* set transform_flags bit flag 0 if `matrix' isn't the identity */
    اگر ( ( matrix->xy | matrix->yx ) ||
         matrix->xx != 0x10000L      ||
         matrix->yy != 0x10000L      )
      internal->transform_flags |= 1;

    اگر ( !delta )
    {
      internal->transform_delta.x = 0;
      internal->transform_delta.y = 0;

      delta = &internal->transform_delta;
    }
    وگرنه
      internal->transform_delta = *delta;

    /* set transform_flags bit flag 1 if `delta' isn't the null vector */
    اگر ( delta->x | delta->y )
      internal->transform_flags |= 2;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( تهی )
  FT_Get_Transform( FT_Face     face,
                    FT_Matrix*  matrix,
                    FT_Vector*  delta )
  {
    FT_Face_Internal  internal;


    اگر ( !face )
      بازگشت;

    internal = face->internal;

    اگر ( matrix )
      *matrix = internal->transform_matrix;

    اگر ( delta )
      *delta = internal->transform_delta;
  }


  ایستا FT_Renderer
  ft_lookup_glyph_renderer( FT_GlyphSlot  slot );


#ifdef GRID_FIT_METRICS
  ایستا تهی
  ft_glyphslot_grid_fit_metrics( FT_GlyphSlot  slot,
                                 FT_Bool       vertical )
  {
    FT_Glyph_Metrics*  metrics = &slot->metrics;
    FT_Pos             راست, bottom;


    اگر ( vertical )
    {
      metrics->horiBearingX = FT_PIX_FLOOR( metrics->horiBearingX );
      metrics->horiBearingY = FT_PIX_CEIL_LONG( metrics->horiBearingY );

      راست  = FT_PIX_CEIL_LONG( ADD_LONG( metrics->vertBearingX,
                                           metrics->width ) );
      bottom = FT_PIX_CEIL_LONG( ADD_LONG( metrics->vertBearingY,
                                           metrics->height ) );

      metrics->vertBearingX = FT_PIX_FLOOR( metrics->vertBearingX );
      metrics->vertBearingY = FT_PIX_FLOOR( metrics->vertBearingY );

      metrics->width  = SUB_LONG( راست,
                                  metrics->vertBearingX );
      metrics->height = SUB_LONG( bottom,
                                  metrics->vertBearingY );
    }
    وگرنه
    {
      metrics->vertBearingX = FT_PIX_FLOOR( metrics->vertBearingX );
      metrics->vertBearingY = FT_PIX_FLOOR( metrics->vertBearingY );

      راست  = FT_PIX_CEIL_LONG( ADD_LONG( metrics->horiBearingX,
                                           metrics->width ) );
      bottom = FT_PIX_FLOOR( SUB_LONG( metrics->horiBearingY,
                                       metrics->height ) );

      metrics->horiBearingX = FT_PIX_FLOOR( metrics->horiBearingX );
      metrics->horiBearingY = FT_PIX_CEIL_LONG( metrics->horiBearingY );

      metrics->width  = SUB_LONG( راست,
                                  metrics->horiBearingX );
      metrics->height = SUB_LONG( metrics->horiBearingY,
                                  bottom );
    }

    metrics->horiAdvance = FT_PIX_ROUND_LONG( metrics->horiAdvance );
    metrics->vertAdvance = FT_PIX_ROUND_LONG( metrics->vertAdvance );
  }
#endif /* GRID_FIT_METRICS */


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Load_Glyph( FT_Face   face,
                 FT_UInt   glyph_index,
                 FT_Int32  load_flags )
  {
    FT_Error      error;
    FT_Driver     driver;
    FT_GlyphSlot  slot;
    FT_Library    library;
    FT_Bool       autohint = FALSE;
    FT_Module     hinter;


    اگر ( !face || !face->size || !face->glyph )
      بازگشت FT_THROW( Invalid_Face_Handle );

    /* The validity test for `glyph_index' is performed by the */
    /* font drivers.                                           */

    slot = face->glyph;
    ft_glyphslot_clear( slot );

    driver  = face->driver;
    library = driver->ریشه.library;
    hinter  = library->auto_hinter;

    /* undefined scale means no scale */
    اگر ( face->size->metrics.x_ppem == 0 ||
         face->size->metrics.y_ppem == 0 )
      load_flags |= FT_LOAD_NO_SCALE;

    /* resolve load flags dependencies */

    اگر ( load_flags & FT_LOAD_NO_RECURSE )
      load_flags |= FT_LOAD_NO_SCALE         |
                    FT_LOAD_IGNORE_TRANSFORM;

    اگر ( load_flags & FT_LOAD_NO_SCALE )
    {
      load_flags |= FT_LOAD_NO_HINTING |
                    FT_LOAD_NO_BITMAP;

      load_flags &= ~FT_LOAD_RENDER;
    }

    اگر ( load_flags & FT_LOAD_BITMAP_METRICS_ONLY )
      load_flags &= ~FT_LOAD_RENDER;

    /*
     * Determine whether we need to auto-hint or not.
     * The general rules are:
     *
     * - Do only auto-hinting if we have
     *
     *   - a hinter module,
     *   - a scalable font,
     *   - not a tricky font, and
     *   - no transforms except simple slants and/or rotations by
     *     integer multiples of 90 degrees.
     *
     * - Then, auto-hint if FT_LOAD_FORCE_AUTOHINT is set or if we don't
     *   have a native font hinter.
     *
     * - Otherwise, auto-hint for LIGHT hinting mode or if there isn't
     *   any hinting bytecode in the TrueType/OpenType font.
     *
     * - Exception: The font is `tricky' and requires the native hinter to
     *   load properly.
     */

    اگر ( hinter                                           &&
         !( load_flags & FT_LOAD_NO_HINTING )             &&
         !( load_flags & FT_LOAD_NO_AUTOHINT )            &&
         FT_IS_SCALABLE( face )                           &&
         !FT_IS_TRICKY( face )                            &&
         ( ( load_flags & FT_LOAD_IGNORE_TRANSFORM )    ||
           ( face->internal->transform_matrix.yx == 0 &&
             face->internal->transform_matrix.xx != 0 ) ||
           ( face->internal->transform_matrix.xx == 0 &&
             face->internal->transform_matrix.yx != 0 ) ) )
    {
      اگر ( ( load_flags & FT_LOAD_FORCE_AUTOHINT ) ||
           !FT_DRIVER_HAS_HINTER( driver )         )
        autohint = TRUE;
      وگرنه
      {
        FT_Render_Mode  mode = FT_LOAD_TARGET_MODE( load_flags );
        FT_Bool         is_light_type1;
        TT_Face         ttface = (TT_Face)face;


        /* only the new Adobe engine (for both CFF and Type 1) is `light'; */
        /* we use `strstr' to catch both `Type 1' and `CID Type 1'         */
        is_light_type1 =
          ft_strstr( FT_Get_Font_Format( face ), "Type 1" ) != پوچ &&
          ((PS_Driver)driver)->hinting_engine == FT_HINTING_ADOBE;

        /* the check for `num_locations' assures that we actually    */
        /* test for instructions in a TTF and not in a CFF-based OTF */
        /*                                                           */
        /* we check the size of the `fpgm' and `prep' tables, too -- */
        /* the assumption is that there don't exist real TTFs where  */
        /* both `fpgm' and `prep' tables are missing                 */
        اگر ( ( mode == FT_RENDER_MODE_LIGHT           &&
               ( !FT_DRIVER_HINTS_LIGHTLY( driver ) &&
                 !is_light_type1                    ) )         ||
             ( FT_IS_SFNT( face )                             &&
               ttface->رقم_locations                          &&
               ttface->font_program_size == 0                 &&
               ttface->cvt_program_size <= 7                  ) )
          autohint = TRUE;
      }
    }

#ifdef TT_CONFIG_OPTION_VARC
    /* Never auto-hint a VARC (variable composite) glyph: its components */
    /* are already loaded unhinted, and the auto-fitter cannot process   */
    /* the assembled composite (it also re-enters variation-coordinate   */
    /* setting).  Use the native hinter / unhinted load instead.         */
    اگر ( autohint && FT_IS_SFNT( face ) )
    {
      FT_Service_VARC  varc;


      FT_FACE_FIND_SERVICE( face, varc, VARC );
      اگر ( varc && varc->has_glyph( face, glyph_index ) )
        autohint = FALSE;
    }
#endif

    اگر ( autohint )
    {
      FT_AutoHinter_Interface  hinting;


      /* XXX: The use of the `FT_LOAD_XXX_ONLY` flags is not very */
      /*      elegant.                                            */

      /* try to load SVG documents if available */
      اگر ( ( load_flags & FT_LOAD_NO_SVG ) == 0 &&
           FT_HAS_SVG( face )                   )
      {
        error = driver->clazz->load_glyph( slot, face->size,
                                           glyph_index,
                                           load_flags | FT_LOAD_SVG_ONLY );

        اگر ( !error && slot->format == FT_GLYPH_FORMAT_SVG )
          برو Load_Ok;
      }

      /* try to load embedded bitmaps if available */
      اگر ( FT_HAS_FIXED_SIZES( face )              &&
           ( load_flags & FT_LOAD_NO_BITMAP ) == 0 )
      {
        error = driver->clazz->load_glyph( slot, face->size,
                                           glyph_index,
                                           load_flags | FT_LOAD_SBITS_ONLY );

        اگر ( !error && slot->format == FT_GLYPH_FORMAT_BITMAP )
          برو Load_Ok;
      }

      {
        FT_Face_Internal  internal        = face->internal;
        FT_Int            transform_flags = internal->transform_flags;


        /* since the auto-hinter calls FT_Load_Glyph by itself, */
        /* make sure that glyphs aren't transformed             */
        internal->transform_flags = 0;

        /* load auto-hinted outline */
        hinting = (FT_AutoHinter_Interface)hinter->clazz->module_interface;

        error   = hinting->load_glyph( (FT_AutoHinter)hinter,
                                       slot, face->size,
                                       glyph_index, load_flags );

        internal->transform_flags = transform_flags;
      }
    }
    وگرنه
    {
      error = driver->clazz->load_glyph( slot,
                                         face->size,
                                         glyph_index,
                                         load_flags );
      اگر ( error )
        برو Exit;

      اگر ( slot->format == FT_GLYPH_FORMAT_OUTLINE )
      {
        /* check that the loaded outline is correct */
        error = FT_Outline_Check( &slot->outline );
        اگر ( error )
          برو Exit;

#ifdef GRID_FIT_METRICS
        اگر ( !( load_flags & FT_LOAD_NO_HINTING ) )
          ft_glyphslot_grid_fit_metrics(
            slot,
            FT_BOOL( load_flags & FT_LOAD_VERTICAL_LAYOUT ) );
#endif
      }
    }

  Load_Ok:
    /* compute the advance */
    اگر ( load_flags & FT_LOAD_VERTICAL_LAYOUT )
    {
      slot->advance.x = 0;
      slot->advance.y = slot->metrics.vertAdvance;
    }
    وگرنه
    {
      slot->advance.x = slot->metrics.horiAdvance;
      slot->advance.y = 0;
    }

    /* compute the linear advance in 16.16 pixels */
    اگر ( ( load_flags & FT_LOAD_LINEAR_DESIGN ) == 0 &&
         FT_IS_SCALABLE( face )                      )
    {
      FT_Size_Metrics*  metrics = &face->size->metrics;


      /* it's tricky! */
      slot->linearHoriAdvance = FT_MulDiv( slot->linearHoriAdvance,
                                           metrics->x_scale, 64 );

      slot->linearVertAdvance = FT_MulDiv( slot->linearVertAdvance,
                                           metrics->y_scale, 64 );
    }

    اگر ( ( load_flags & FT_LOAD_IGNORE_TRANSFORM ) == 0 )
    {
      FT_Face_Internal  internal = face->internal;


      /* now, transform the glyph image if needed */
      اگر ( internal->transform_flags )
      {
        /* get renderer */
        FT_Renderer  renderer = ft_lookup_glyph_renderer( slot );


        اگر ( renderer )
          error = renderer->clazz->transform_glyph(
                                     renderer, slot,
                                     &internal->transform_matrix,
                                     &internal->transform_delta );
        وگرنه اگر ( slot->format == FT_GLYPH_FORMAT_OUTLINE )
        {
          /* apply `standard' transformation if no renderer is available */
          اگر ( internal->transform_flags & 1 )
            FT_Outline_Transform( &slot->outline,
                                  &internal->transform_matrix );

          اگر ( internal->transform_flags & 2 )
            FT_Outline_Translate( &slot->outline,
                                  internal->transform_delta.x,
                                  internal->transform_delta.y );
        }

        /* transform advance */
        FT_Vector_Transform( &slot->advance, &internal->transform_matrix );
      }
    }

    slot->glyph_index          = glyph_index;
    slot->internal->load_flags = load_flags;

    /* do we need to render the image or preset the bitmap now? */
    اگر ( !error                                    &&
         ( load_flags & FT_LOAD_NO_SCALE ) == 0    &&
         slot->format != FT_GLYPH_FORMAT_BITMAP    &&
         slot->format != FT_GLYPH_FORMAT_COMPOSITE )
    {
      FT_Render_Mode  mode = FT_LOAD_TARGET_MODE( load_flags );


      اگر ( mode == FT_RENDER_MODE_NORMAL   &&
           load_flags & FT_LOAD_MONOCHROME )
        mode = FT_RENDER_MODE_MONO;

      اگر ( load_flags & FT_LOAD_RENDER )
        error = FT_Render_Glyph( slot, mode );
      وگرنه
        ft_glyphslot_preset_bitmap( slot, mode, پوچ );
    }

#ifdef FT_DEBUG_LEVEL_TRACE
    FT_TRACE5(( "FT_Load_Glyph: index %u, flags 0x%x\n",
                glyph_index, load_flags ));
    FT_TRACE5(( "  bitmap %ux%u %s, %s (mode %d)\n",
                slot->bitmap.width,
                slot->bitmap.rows,
                slot->outline.points ?
                  slot->bitmap.buffer ? "rendered"
                                      : "preset"
                                     :
                  slot->internal->flags & FT_GLYPH_OWN_BITMAP ? "owned"
                                                              : "unowned",
                pixel_modes[slot->bitmap.pixel_mode],
                slot->bitmap.pixel_mode ));
    FT_TRACE5(( "\n" ));
    FT_TRACE5(( "  x advance: %f\n", (دوگانه)slot->advance.x / 64 ));
    FT_TRACE5(( "  y advance: %f\n", (دوگانه)slot->advance.y / 64 ));
    FT_TRACE5(( "  linear x advance: %f\n",
                (دوگانه)slot->linearHoriAdvance / 65536 ));
    FT_TRACE5(( "  linear y advance: %f\n",
                (دوگانه)slot->linearVertAdvance / 65536 ));

    {
      FT_Glyph_Metrics*  metrics = &slot->metrics;


      FT_TRACE5(( "  metrics:\n" ));
      FT_TRACE5(( "    width:  %f\n", (دوگانه)metrics->width / 64 ));
      FT_TRACE5(( "    height: %f\n", (دوگانه)metrics->height / 64 ));
      FT_TRACE5(( "\n" ));
      FT_TRACE5(( "    horiBearingX: %f\n",
                  (دوگانه)metrics->horiBearingX / 64 ));
      FT_TRACE5(( "    horiBearingY: %f\n",
                  (دوگانه)metrics->horiBearingY / 64 ));
      FT_TRACE5(( "    horiAdvance:  %f\n",
                  (دوگانه)metrics->horiAdvance / 64 ));
      FT_TRACE5(( "\n" ));
      FT_TRACE5(( "    vertBearingX: %f\n",
                  (دوگانه)metrics->vertBearingX / 64 ));
      FT_TRACE5(( "    vertBearingY: %f\n",
                  (دوگانه)metrics->vertBearingY / 64 ));
      FT_TRACE5(( "    vertAdvance:  %f\n",
                  (دوگانه)metrics->vertAdvance / 64 ));
    }
#endif

  Exit:
    بازگشت error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Load_Char( FT_Face   face,
                FT_ULong  char_code,
                FT_Int32  load_flags )
  {
    FT_UInt  glyph_index;


    اگر ( !face )
      بازگشت FT_THROW( Invalid_Face_Handle );

    glyph_index = (FT_UInt)char_code;
    اگر ( face->charmap )
      glyph_index = FT_Get_Char_Index( face, char_code );

    بازگشت FT_Load_Glyph( face, glyph_index, load_flags );
  }


  /* destructor for sizes list */
  ایستا تهی
  destroy_size( FT_Memory  memory,
                تهی*      size_,
                تهی*      driver_ )
  {
    FT_Size    size   = (FT_Size)size_;
    FT_Driver  driver = (FT_Driver)driver_;


    /* finalize format-specific stuff */
    اگر ( driver->clazz->done_size )
      driver->clazz->done_size( size );

    /* finalize client-specific data */
    اگر ( size->generic.finalizer )
      size->generic.finalizer( size );

    FT_FREE( size->internal );
    FT_FREE( size );
  }


  ایستا تهی
  ft_cmap_done_internal( FT_CMap  cmap );


  ایستا تهی
  destroy_charmaps( FT_Face    face,
                    FT_Memory  memory )
  {
    FT_Int  n;


    اگر ( !face )
      بازگشت;

    برای ( n = 0; n < face->رقم_charmaps; n++ )
    {
      FT_CMap  cmap = FT_CMAP( face->charmaps[n] );


      ft_cmap_done_internal( cmap );

      face->charmaps[n] = پوچ;
    }

    FT_FREE( face->charmaps );
    face->رقم_charmaps = 0;
  }


  /* destructor for faces list */
  ایستا تهی
  destroy_face( FT_Memory  memory,
                تهی*      face_,
                تهی*      driver_ )
  {
    FT_Face          face   = (FT_Face)face_;
    FT_Driver        driver = (FT_Driver)driver_;
    FT_Driver_Class  clazz  = driver->clazz;


    /* discard auto-hinting data */
    اگر ( face->autohint.finalizer )
      face->autohint.finalizer( face->autohint.data );

    /* Discard glyph slots for this face.                           */
    /* Beware!  FT_Done_GlyphSlot() changes the field `face->glyph' */
    وقتی ( face->glyph )
      FT_Done_GlyphSlot( face->glyph );

    /* discard all sizes for this face */
    FT_List_Finalize( &face->sizes_list,
                      destroy_size,
                      memory,
                      driver );
    face->size = پوچ;

    /* discard charmaps */
    destroy_charmaps( face, memory );

    /* finalize format-specific stuff */
    اگر ( clazz->done_face )
      clazz->done_face( face );

    /* close the stream for this face if needed */
    FT_Stream_Free(
      face->stream,
      ( face->face_flags & FT_FACE_FLAG_EXTERNAL_STREAM ) != 0 );

    face->stream = پوچ;

    /* now discard client data */
    اگر ( face->generic.finalizer )
      face->generic.finalizer( face );

    /* get rid of it */
    اگر ( face->internal )
    {
      FT_FREE( face->internal );
    }
    FT_FREE( face );
  }


  ایستا تهی
  Destroy_Driver( FT_Driver  driver )
  {
    FT_List_Finalize( &driver->faces_list,
                      destroy_face,
                      driver->ریشه.memory,
                      driver );
  }


  /* documentation is in ftobjs.h */

  FT_BASE_DEF( FT_Error )
  find_unicode_charmap( FT_Face  face )
  {
    FT_CharMap*  first;
    FT_CharMap*  cur;


    /* caller should have already checked that `face' is valid */
    FT_ASSERT( face );

    first = face->charmaps;

    اگر ( !first )
      بازگشت FT_THROW( Invalid_CharMap_Handle );

    /*
     * The original TrueType specification(s) only specified charmap
     * formats that are capable of mapping 8 or 16 bit character codes to
     * glyph indices.
     *
     * However, recent updates to the Apple and OpenType specifications
     * introduced new formats that are capable of mapping 32-bit character
     * codes as well.  And these are already used on some fonts, mainly to
     * map non-BMP Asian ideographs as defined in Unicode.
     *
     * For compatibility purposes, these fonts generally come with
     * *several* Unicode charmaps:
     *
     * - One of them in the "old" 16-bit format, that cannot access
     *   all glyphs in the font.
     *
     * - Another one in the "new" 32-bit format, that can access all
     *   the glyphs.
     *
     * This function has been written to always favor a 32-bit charmap
     * when found.  Otherwise, a 16-bit one is returned when found.
     */

    /* Since the `interesting' table, with IDs (3,10), is normally the */
    /* last one, we loop backwards.  This loses with type1 fonts with  */
    /* non-BMP characters (<.0001%), this wins with .ttf with non-BMP  */
    /* chars (.01% ?), and this is the same about 99.99% of the time!  */

    cur = first + face->رقم_charmaps;  /* points after the last one */

    برای ( ; --cur >= first; )
    {
      اگر ( cur[0]->encoding == FT_ENCODING_UNICODE )
      {
        /* XXX If some new encodings to represent UCS-4 are added, */
        /*     they should be added here.                          */
        اگر ( ( cur[0]->platform_id == TT_PLATFORM_MICROSOFT &&
               cur[0]->encoding_id == TT_MS_ID_UCS_4        )     ||
             ( cur[0]->platform_id == TT_PLATFORM_APPLE_UNICODE &&
               cur[0]->encoding_id == TT_APPLE_ID_UNICODE_32    ) ||
             ( cur[0]->platform_id == TT_PLATFORM_APPLE_UNICODE &&
               cur[0]->encoding_id == TT_APPLE_ID_FULL_UNICODE  &&
               FT_Get_CMap_Format( cur[0] ) == 13               ) )
        {
          face->charmap = cur[0];
          بازگشت FT_Err_Ok;
        }
      }
    }

    /* We do not have any UCS-4 charmap.                */
    /* Do the loop again and search for UCS-2 charmaps. */
    cur = first + face->رقم_charmaps;

    برای ( ; --cur >= first; )
    {
      اگر ( cur[0]->encoding == FT_ENCODING_UNICODE )
      {
        FT_Long  format = FT_Get_CMap_Format( cur[0] );


        اگر ( format != 14 && format != 15 )
        {
          face->charmap = cur[0];
          بازگشت FT_Err_Ok;
        }
      }
    }

    بازگشت FT_THROW( Invalid_CharMap_Handle );
  }


  /**************************************************************************
   *
   * @Function:
   *   find_variant_selector_charmap
   *
   * @Description:
   *   This function finds the variant selector charmap, if there is one.
   *   There can only be one (platform=0, specific=5, format=14 or 15).
   */
  ایستا FT_CharMap
  find_variant_selector_charmap( FT_Face  face )
  {
    FT_CharMap*  first;
    FT_CharMap*  end;
    FT_CharMap*  cur;


    /* caller should have already checked that `face' is valid */
    FT_ASSERT( face );

    first = face->charmaps;

    اگر ( !first )
      بازگشت پوچ;

    end = first + face->رقم_charmaps;  /* points after the last one */

    برای ( cur = first; cur < end; cur++ )
    {
      اگر ( cur[0]->platform_id == TT_PLATFORM_APPLE_UNICODE    &&
           cur[0]->encoding_id == TT_APPLE_ID_VARIANT_SELECTOR )
      {
        FT_Long  format = FT_Get_CMap_Format( cur[0] );


        اگر ( format == 14 || format == 15 )
          بازگشت cur[0];
      }
    }

    بازگشت پوچ;
  }


  /**************************************************************************
   *
   * @Function:
   *   open_face
   *
   * @Description:
   *   This function does some work for FT_Open_Face().
   */
  ایستا FT_Error
  open_face( FT_Driver      driver,
             FT_Stream      *astream,
             FT_Bool        *anexternal_stream,
             FT_Long        face_index,
             FT_Int         رقم_params,
             FT_Parameter*  params,
             FT_Face       *aface )
  {
    FT_Memory         memory;
    FT_Driver_Class   clazz;
    FT_Face           face     = پوچ;
    FT_Face_Internal  internal = پوچ;

    FT_Error          error, error2;


    clazz  = driver->clazz;
    memory = driver->ریشه.memory;

    /* allocate the face object and perform basic initialization */
    اگر ( FT_ALLOC( face, clazz->face_object_size ) )
      برو Fail;

    face->driver = driver;
    face->memory = memory;
    face->stream = *astream;

    /* set the FT_FACE_FLAG_EXTERNAL_STREAM bit for FT_Done_Face */
    اگر ( *anexternal_stream )
      face->face_flags |= FT_FACE_FLAG_EXTERNAL_STREAM;

    اگر ( FT_NEW( internal ) )
      برو Fail;

    face->internal = internal;

#ifdef FT_CONFIG_OPTION_INCREMENTAL
    {
      صحیح  i;


      face->internal->incremental_interface = پوچ;
      برای ( i = 0; i < رقم_params && !face->internal->incremental_interface;
            i++ )
        اگر ( params[i].tag == FT_PARAM_TAG_INCREMENTAL )
          face->internal->incremental_interface =
            (FT_Incremental_Interface)params[i].data;
    }
#endif

    face->internal->random_seed = -1;

    اگر ( clazz->init_face )
      error = clazz->init_face( *astream,
                                face,
                                (FT_Int)face_index,
                                رقم_params,
                                params );
    /* Stream may have been changed. */
    *astream = face->stream;
    *anexternal_stream =
      ( face->face_flags & FT_FACE_FLAG_EXTERNAL_STREAM ) != 0;
    اگر ( error )
      برو Fail;

    /* select Unicode charmap by default */
    error2 = find_unicode_charmap( face );

    /* if no Unicode charmap can be found, FT_Err_Invalid_CharMap_Handle */
    /* is returned.                                                      */

    /* no error should happen, but we want to play safe */
    اگر ( error2 && FT_ERR_NEQ( error2, Invalid_CharMap_Handle ) )
    {
      error = error2;
      برو Fail;
    }

    *aface = face;

  Fail:
    اگر ( error )
    {
      destroy_charmaps( face, memory );
      اگر ( clazz->done_face )
        clazz->done_face( face );
      FT_FREE( internal );
      FT_FREE( face );
      *aface = پوچ;
    }

    بازگشت error;
  }


  /* there's a Mac-specific extended implementation of FT_New_Face() */
  /* in src/base/ftmac.c                                             */

#ifndef FT_MACINTOSH

  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_New_Face( FT_Library   library,
               ثابت نویسه*  pathname,
               FT_Long      face_index,
               FT_Face     *aface )
  {
    FT_Open_Args  args;


    /* test for valid `library' and `aface' delayed to `FT_Open_Face' */
    اگر ( !pathname )
      بازگشت FT_THROW( Invalid_Argument );

    args.flags    = FT_OPEN_PATHNAME;
    args.pathname = (نویسه*)pathname;
    args.stream   = پوچ;

    بازگشت ft_open_face_internal( library, &args, face_index, aface, 1 );
  }

#endif


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_New_Memory_Face( FT_Library      library,
                      ثابت FT_Byte*  file_base,
                      FT_Long         file_size,
                      FT_Long         face_index,
                      FT_Face        *aface )
  {
    FT_Open_Args  args;


    /* test for valid `library' and `face' delayed to `FT_Open_Face' */
    اگر ( !file_base )
      بازگشت FT_THROW( Invalid_Argument );

    args.flags       = FT_OPEN_MEMORY;
    args.memory_base = file_base;
    args.memory_size = file_size;
    args.stream      = پوچ;

    بازگشت ft_open_face_internal( library, &args, face_index, aface, 1 );
  }


#ifdef FT_CONFIG_OPTION_MAC_FONTS

  /* The behavior here is very similar to that in base/ftmac.c, but it     */
  /* is designed to work on non-mac systems, so no mac specific calls.     */
  /*                                                                       */
  /* We look at the file and determine if it is a mac dfont file or a mac  */
  /* resource file, or a macbinary file containing a mac resource file.    */
  /*                                                                       */
  /* Unlike ftmac I'm not going to look at a `FOND'.  I don't really see   */
  /* the point, especially since there may be multiple `FOND' resources.   */
  /* Instead I'll just look for `sfnt' and `POST' resources, ordered as    */
  /* they occur in the file.                                               */
  /*                                                                       */
  /* Note that multiple `POST' resources do not mean multiple postscript   */
  /* fonts; they all get jammed together to make what is essentially a     */
  /* pfb file.                                                             */
  /*                                                                       */
  /* We aren't interested in `NFNT' or `FONT' bitmap resources.            */
  /*                                                                       */
  /* As soon as we get an `sfnt' load it into memory and pass it off to    */
  /* FT_Open_Face.                                                         */
  /*                                                                       */
  /* If we have a (set of) `POST' resources, massage them into a (memory)  */
  /* pfb file and pass that to FT_Open_Face.  (As with ftmac.c I'm not     */
  /* going to try to save the kerning info.  After all that lives in the   */
  /* `FOND' which isn't in the file containing the `POST' resources so     */
  /* we don't really have access to it.                                    */


  /* Finalizer for a memory stream; gets called by FT_Done_Face(). */
  /* It frees the memory it uses.                                  */
  /* From `ftmac.c'.                                               */
  ایستا تهی
  memory_stream_close( FT_Stream  stream )
  {
    FT_Memory  memory = (FT_Memory)stream->descriptor.pointer;


    FT_FREE( stream->base );
    stream->size  = 0;
    stream->close = پوچ;
    FT_FREE( stream );
  }


  /* Create a new memory stream from a buffer and a size. */
  /* From `ftmac.c'.                                      */
  ایستا FT_Error
  new_memory_stream( FT_Library           library,
                     FT_Byte*             base,
                     FT_ULong             size,
                     FT_Stream_CloseFunc  close,
                     FT_Stream           *astream )
  {
    FT_Error   error;
    FT_Memory  memory;
    FT_Stream  stream = پوچ;


    اگر ( !library )
      بازگشت FT_THROW( Invalid_Library_Handle );

    اگر ( !base )
      بازگشت FT_THROW( Invalid_Argument );

    *astream = پوچ;
    memory   = library->memory;
    اگر ( FT_NEW( stream ) )
      برو Exit;

    FT_Stream_OpenMemory( stream, base, size );

    stream->descriptor.pointer = memory;
    stream->close              = close;

    *astream = stream;

  Exit:
    بازگشت error;
  }


  /* Create a new FT_Face given a buffer and a driver name. */
  /* From `ftmac.c'.                                        */
  FT_LOCAL_DEF( FT_Error )
  open_face_from_buffer( FT_Library   library,
                         FT_Byte*     base,
                         FT_ULong     size,
                         FT_Long      face_index,
                         ثابت نویسه*  driver_name,
                         FT_Face     *aface )
  {
    FT_Open_Args  args;
    FT_Error      error;
    FT_Memory     memory = library->memory;


    args.driver = پوچ;
    args.flags  = 0;

    اگر ( driver_name )
    {
      args.driver = FT_Get_Module( library, driver_name );
      اگر ( !args.driver )
      {
        FT_FREE( base );
        بازگشت FT_THROW( Missing_Module );
      }

      args.flags = args.flags | FT_OPEN_DRIVER;
    }

    /* `memory_stream_close` also frees the stream object. */
    error = new_memory_stream( library,
                               base,
                               size,
                               memory_stream_close,
                               &args.stream );
    اگر ( error )
    {
      FT_FREE( base );
      بازگشت error;
    }

    args.flags |= FT_OPEN_STREAM;

#ifdef FT_MACINTOSH
    /* At this point, the face index has served its purpose;  */
    /* whoever calls this function has already used it to     */
    /* locate the correct font data.  We should not propagate */
    /* this index to FT_Open_Face() (unless it is negative).  */

    اگر ( face_index > 0 )
      face_index &= 0x7FFF0000L; /* retain GX data */
#endif

    بازگشت ft_open_face_internal( library, &args, face_index, aface, 0 );
  }


  /* Look up `TYP1' or `CID ' table from sfnt table directory.       */
  /* `offset' and `length' must exclude the binary header in tables. */

  /* Type 1 and CID-keyed font drivers should recognize sfnt-wrapped */
  /* format too.  Here, since we can't expect that the TrueType font */
  /* driver is loaded unconditionally, we must parse the font by     */
  /* ourselves.  We are only interested in the name of the table and */
  /* the offset.                                                     */

  ایستا FT_Error
  ft_lookup_PS_in_sfnt_stream( FT_Stream  stream,
                               FT_Long    face_index,
                               FT_ULong*  offset,
                               FT_ULong*  length,
                               FT_Bool*   is_sfnt_cid )
  {
    FT_Error   error;
    FT_UShort  رقم_Tables;
    FT_Long    pstable_index;
    FT_ULong   tag;
    صحیح        i;


    *offset = 0;
    *length = 0;
    *is_sfnt_cid = FALSE;

    /* TODO: support for sfnt-wrapped PS/CID in TTC format */

    /* version check for 'typ1' (should be ignored?) */
    اگر ( FT_READ_ULONG( tag ) )
      بازگشت error;
    اگر ( tag != TTAG_typ1 )
      بازگشت FT_THROW( Unknown_File_Format );

    اگر ( FT_READ_USHORT( رقم_Tables ) )
      بازگشت error;
    اگر ( FT_STREAM_SKIP( 2 * 3 ) ) /* skip binary search header */
      بازگشت error;

    pstable_index = -1;
    *is_sfnt_cid  = FALSE;

    برای ( i = 0; i < رقم_Tables; i++ )
    {
      اگر ( FT_READ_ULONG( tag )     || FT_STREAM_SKIP( 4 )      ||
           FT_READ_ULONG( *offset ) || FT_READ_ULONG( *length ) )
        بازگشت error;

      اگر ( tag == TTAG_CID )
      {
        pstable_index++;
        *offset += 22;
        *length -= 22;
        *is_sfnt_cid = TRUE;
        اگر ( face_index < 0 )
          بازگشت FT_Err_Ok;
      }
      وگرنه اگر ( tag == TTAG_TYP1 )
      {
        pstable_index++;
        *offset += 24;
        *length -= 24;
        *is_sfnt_cid = FALSE;
        اگر ( face_index < 0 )
          بازگشت FT_Err_Ok;
      }
      اگر ( face_index >= 0 && pstable_index == face_index )
        بازگشت FT_Err_Ok;
    }

    بازگشت FT_THROW( Table_Missing );
  }


  FT_LOCAL_DEF( FT_Error )
  open_face_PS_from_sfnt_stream( FT_Library     library,
                                 FT_Stream      stream,
                                 FT_Long        face_index,
                                 FT_Int         رقم_params,
                                 FT_Parameter  *params,
                                 FT_Face       *aface )
  {
    FT_Error   error;
    FT_Memory  memory = library->memory;
    FT_ULong   offset, length;
    FT_ULong   pos;
    FT_Bool    is_sfnt_cid;
    FT_Byte*   sfnt_ps = پوچ;

    FT_UNUSED( رقم_params );
    FT_UNUSED( params );


    /* ignore GX stuff */
    اگر ( face_index > 0 )
      face_index &= 0xFFFFL;

    pos = FT_STREAM_POS();

    error = ft_lookup_PS_in_sfnt_stream( stream,
                                         face_index,
                                         &offset,
                                         &length,
                                         &is_sfnt_cid );
    اگر ( error )
      برو Exit;

    اگر ( offset > stream->size )
    {
      FT_TRACE2(( "open_face_PS_from_sfnt_stream: invalid table offset\n" ));
      error = FT_THROW( Invalid_Table );
      برو Exit;
    }
    وگرنه اگر ( length > stream->size - offset )
    {
      FT_TRACE2(( "open_face_PS_from_sfnt_stream: invalid table length\n" ));
      error = FT_THROW( Invalid_Table );
      برو Exit;
    }

    error = FT_Stream_Seek( stream, pos + offset );
    اگر ( error )
      برو Exit;

    اگر ( FT_QALLOC( sfnt_ps, (FT_Long)length ) )
      برو Exit;

    error = FT_Stream_Read( stream, (FT_Byte *)sfnt_ps, length );
    اگر ( error )
    {
      FT_FREE( sfnt_ps );
      برو Exit;
    }

    error = open_face_from_buffer( library,
                                   sfnt_ps,
                                   length,
                                   FT_MIN( face_index, 0 ),
                                   is_sfnt_cid ? "t1cid" : "type1",
                                   aface );
  Exit:
    {
      FT_Error  error1;


      اگر ( FT_ERR_EQ( error, Unknown_File_Format ) )
      {
        error1 = FT_Stream_Seek( stream, pos );
        اگر ( error1 )
          بازگشت error1;
      }

      بازگشت error;
    }
  }


#ifndef FT_MACINTOSH

  /* The resource header says we've got resource_cnt `POST' (type1) */
  /* resources in this file.  They all need to be coalesced into    */
  /* one lump which gets passed on to the type1 driver.             */
  /* Here can be only one PostScript font in a file so face_index   */
  /* must be 0 (or -1).                                             */
  /*                                                                */
  ایستا FT_Error
  Mac_Read_POST_Resource( FT_Library  library,
                          FT_Stream   stream,
                          FT_Long    *offsets,
                          FT_Long     resource_cnt,
                          FT_Long     face_index,
                          FT_Face    *aface )
  {
    FT_Error   error  = FT_ERR( Cannot_Open_Resource );
    FT_Memory  memory = library->memory;

    FT_Byte*   pfb_data = پوچ;
    صحیح        i, type, flags;
    FT_ULong   len;
    FT_ULong   pfb_len, pfb_pos, pfb_lenpos;
    FT_ULong   rlen, temp;


    اگر ( face_index == -1 )
      face_index = 0;
    اگر ( face_index != 0 )
      بازگشت error;

    /* Find the length of all the POST resources, concatenated.  Assume */
    /* worst case (each resource in its own section).                   */
    pfb_len = 0;
    برای ( i = 0; i < resource_cnt; i++ )
    {
      error = FT_Stream_Seek( stream, (FT_ULong)offsets[i] );
      اگر ( error )
        برو Exit;
      اگر ( FT_READ_ULONG( temp ) )  /* actually LONG */
        برو Exit;

      /* FT2 allocator takes signed long buffer length,
       * too large value causing overflow should be checked
       */
      FT_TRACE4(( "                 POST fragment #%d: length=0x%08lx"
                  " total pfb_len=0x%08lx\n",
                  i, temp, pfb_len + temp + 6 ));

      اگر ( FT_MAC_RFORK_MAX_LEN < temp               ||
           FT_MAC_RFORK_MAX_LEN - temp < pfb_len + 6 )
      {
        FT_TRACE2(( "             MacOS resource length cannot exceed"
                    " 0x%08lx\n",
                    FT_MAC_RFORK_MAX_LEN ));

        error = FT_THROW( Invalid_Offset );
        برو Exit;
      }

      pfb_len += temp + 6;
    }

    FT_TRACE2(( "             total buffer size to concatenate"
                " %ld POST fragments: 0x%08lx\n",
                 resource_cnt, pfb_len + 2 ));

    اگر ( pfb_len + 2 < 6 )
    {
      FT_TRACE2(( "             too long fragment length makes"
                  " pfb_len confused: pfb_len=0x%08lx\n",
                  pfb_len ));

      error = FT_THROW( Array_Too_Large );
      برو Exit;
    }

    اگر ( FT_QALLOC( pfb_data, (FT_Long)pfb_len + 2 ) )
      برو Exit;

    pfb_data[0] = 0x80;
    pfb_data[1] = 1;            /* Ascii section */
    pfb_data[2] = 0;            /* 4-byte length, fill in later */
    pfb_data[3] = 0;
    pfb_data[4] = 0;
    pfb_data[5] = 0;
    pfb_pos     = 6;
    pfb_lenpos  = 2;

    len  = 0;
    type = 1;

    برای ( i = 0; i < resource_cnt; i++ )
    {
      error = FT_Stream_Seek( stream, (FT_ULong)offsets[i] );
      اگر ( error )
        برو Exit2;
      اگر ( FT_READ_ULONG( rlen ) )
        برو Exit2;

      /* FT2 allocator takes signed long buffer length,
       * too large fragment length causing overflow should be checked
       */
      اگر ( 0x7FFFFFFFUL < rlen )
      {
        error = FT_THROW( Invalid_Offset );
        برو Exit2;
      }

      اگر ( FT_READ_USHORT( flags ) )
        برو Exit2;

      FT_TRACE3(( "POST fragment[%d]:"
                  " offsets=0x%08lx, rlen=0x%08lx, flags=0x%04x\n",
                  i, offsets[i], rlen, flags ));

      error = FT_ERR( Array_Too_Large );

      /* postpone the check of `rlen longer than buffer' */
      /* until `FT_Stream_Read'                          */

      اگر ( ( flags >> 8 ) == 0 )        /* Comment, should not be loaded */
      {
        FT_TRACE3(( "    Skip POST fragment #%d because it is a comment\n",
                    i ));
        ادامه;
      }

      /* the flags are part of the resource, so rlen >= 2,  */
      /* but some fonts declare rlen = 0 for empty fragment */
      اگر ( rlen > 2 )
        rlen -= 2;
      وگرنه
        rlen = 0;

      اگر ( ( flags >> 8 ) == type )
        len += rlen;
      وگرنه
      {
        FT_TRACE3(( "    Write POST fragment #%d header (4-byte) to buffer"
                    " %p + 0x%08lx\n",
                    i, (تهی*)pfb_data, pfb_lenpos ));

        اگر ( pfb_lenpos + 3 > pfb_len + 2 )
          برو Exit2;

        pfb_data[pfb_lenpos    ] = (FT_Byte)( len );
        pfb_data[pfb_lenpos + 1] = (FT_Byte)( len >> 8 );
        pfb_data[pfb_lenpos + 2] = (FT_Byte)( len >> 16 );
        pfb_data[pfb_lenpos + 3] = (FT_Byte)( len >> 24 );

        اگر ( ( flags >> 8 ) == 5 )      /* End of font mark */
          بشکن;

        FT_TRACE3(( "    Write POST fragment #%d header (6-byte) to buffer"
                    " %p + 0x%08lx\n",
                    i, (تهی*)pfb_data, pfb_pos ));

        اگر ( pfb_pos + 6 > pfb_len + 2 )
          برو Exit2;

        pfb_data[pfb_pos++] = 0x80;

        type = flags >> 8;
        len  = rlen;

        pfb_data[pfb_pos++] = (FT_Byte)type;
        pfb_lenpos          = pfb_pos;
        pfb_data[pfb_pos++] = 0;        /* 4-byte length, fill in later */
        pfb_data[pfb_pos++] = 0;
        pfb_data[pfb_pos++] = 0;
        pfb_data[pfb_pos++] = 0;
      }

      اگر ( pfb_pos > pfb_len || pfb_pos + rlen > pfb_len )
        برو Exit2;

      FT_TRACE3(( "    Load POST fragment #%d (%lu byte) to buffer"
                  " %p + 0x%08lx\n",
                  i, rlen, (تهی*)pfb_data, pfb_pos ));

      error = FT_Stream_Read( stream, (FT_Byte *)pfb_data + pfb_pos, rlen );
      اگر ( error )
        برو Exit2;

      pfb_pos += rlen;
    }

    error = FT_ERR( Array_Too_Large );

    اگر ( pfb_pos + 2 > pfb_len + 2 )
      برو Exit2;
    pfb_data[pfb_pos++] = 0x80;
    pfb_data[pfb_pos++] = 3;

    اگر ( pfb_lenpos + 3 > pfb_len + 2 )
      برو Exit2;
    pfb_data[pfb_lenpos    ] = (FT_Byte)( len );
    pfb_data[pfb_lenpos + 1] = (FT_Byte)( len >> 8 );
    pfb_data[pfb_lenpos + 2] = (FT_Byte)( len >> 16 );
    pfb_data[pfb_lenpos + 3] = (FT_Byte)( len >> 24 );

    بازگشت open_face_from_buffer( library,
                                  pfb_data,
                                  pfb_pos,
                                  face_index,
                                  "type1",
                                  aface );

  Exit2:
    اگر ( FT_ERR_EQ( error, Array_Too_Large ) )
      FT_TRACE2(( "  Abort due to too-short buffer to store"
                  " all POST fragments\n" ));
    وگرنه اگر ( FT_ERR_EQ( error, Invalid_Offset ) )
      FT_TRACE2(( "  Abort due to invalid offset in a POST fragment\n" ));

    اگر ( error )
      error = FT_ERR( Cannot_Open_Resource );
    FT_FREE( pfb_data );

  Exit:
    بازگشت error;
  }


  /* The resource header says we've got resource_cnt `sfnt'      */
  /* (TrueType/OpenType) resources in this file.  Look through   */
  /* them for the one indicated by face_index, load it into mem, */
  /* pass it on to the truetype driver, and return it.           */
  /*                                                             */
  ایستا FT_Error
  Mac_Read_sfnt_Resource( FT_Library  library,
                          FT_Stream   stream,
                          FT_Long    *offsets,
                          FT_Long     resource_cnt,
                          FT_Long     face_index,
                          FT_Face    *aface )
  {
    FT_Memory  memory = library->memory;
    FT_Byte*   sfnt_data = پوچ;
    FT_Error   error;
    FT_ULong   flag_offset;
    FT_ULong   rlen;
    صحیح        is_cff;
    FT_Long    face_index_in_resource = 0;


    اگر ( face_index < 0 )
      face_index = -face_index - 1;
    اگر ( face_index >= resource_cnt )
      بازگشت FT_THROW( Cannot_Open_Resource );

    flag_offset = (FT_ULong)offsets[face_index];
    error = FT_Stream_Seek( stream, flag_offset );
    اگر ( error )
      برو Exit;

    اگر ( FT_READ_ULONG( rlen ) )
      برو Exit;
    اگر ( !rlen )
      بازگشت FT_THROW( Cannot_Open_Resource );
    اگر ( rlen > FT_MAC_RFORK_MAX_LEN )
      بازگشت FT_THROW( Invalid_Offset );

    error = open_face_PS_from_sfnt_stream( library,
                                           stream,
                                           face_index,
                                           0, پوچ,
                                           aface );
    اگر ( !error )
      برو Exit;

    /* rewind sfnt stream before open_face_PS_from_sfnt_stream() */
    error = FT_Stream_Seek( stream, flag_offset + 4 );
    اگر ( error )
      برو Exit;

    اگر ( FT_QALLOC( sfnt_data, rlen ) )
      بازگشت error;
    error = FT_Stream_Read( stream, (FT_Byte *)sfnt_data, rlen );
    اگر ( error )
    {
      FT_FREE( sfnt_data );
      برو Exit;
    }

    is_cff = rlen > 4 && !ft_memcmp( sfnt_data, "OTTO", 4 );
    error = open_face_from_buffer( library,
                                   sfnt_data,
                                   rlen,
                                   face_index_in_resource,
                                   is_cff ? "cff" : "truetype",
                                   aface );

  Exit:
    بازگشت error;
  }


  /* Check for a valid resource fork header, or a valid dfont    */
  /* header.  In a resource fork the first 16 bytes are repeated */
  /* at the location specified by bytes 4-7.  In a dfont bytes   */
  /* 4-7 point to 16 bytes of zeroes instead.                    */
  /*                                                             */
  ایستا FT_Error
  IsMacResource( FT_Library  library,
                 FT_Stream   stream,
                 FT_Long     resource_offset,
                 FT_Long     face_index,
                 FT_Face    *aface )
  {
    FT_Memory  memory = library->memory;
    FT_Error   error;
    FT_Long    map_offset, rdata_pos;
    FT_Long    *data_offsets;
    FT_Long    شمار;


    error = FT_Raccess_Get_HeaderInfo( library, stream, resource_offset,
                                       &map_offset, &rdata_pos );
    اگر ( error )
      بازگشت error;

    /* POST resources must be sorted to concatenate properly */
    error = FT_Raccess_Get_DataOffsets( library, stream,
                                        map_offset, rdata_pos,
                                        TTAG_POST, TRUE,
                                        &data_offsets, &شمار );
    اگر ( !error )
    {
      error = Mac_Read_POST_Resource( library, stream, data_offsets, شمار,
                                      face_index, aface );
      FT_FREE( data_offsets );
      /* POST exists in an LWFN providing a single face */
      اگر ( !error )
        (*aface)->رقم_faces = 1;
      بازگشت error;
    }

    /* sfnt resources should not be sorted to preserve the face order by
       QuickDraw API */
    error = FT_Raccess_Get_DataOffsets( library, stream,
                                        map_offset, rdata_pos,
                                        TTAG_sfnt, FALSE,
                                        &data_offsets, &شمار );
    اگر ( !error )
    {
      FT_Long  face_index_internal = face_index % شمار;


      error = Mac_Read_sfnt_Resource( library, stream, data_offsets, شمار,
                                      face_index_internal, aface );
      FT_FREE( data_offsets );
      اگر ( !error )
      {
        (*aface)->رقم_faces  = شمار;
        (*aface)->face_index = face_index_internal;
      }
    }

    بازگشت error;
  }


  /* Check for a valid macbinary header, and if we find one   */
  /* check that the (flattened) resource fork in it is valid. */
  /*                                                          */
  ایستا FT_Error
  Is_Mac_دودویی( FT_Library  library,
               FT_Stream   stream,
               FT_Long     face_index,
               FT_Face    *aface )
  {
    unsigned نویسه  header[128];
    FT_Error       error;
    FT_Long        dlen, offset;


    اگر ( !stream )
      بازگشت FT_THROW( Invalid_Stream_Operation );

    error = FT_Stream_Seek( stream, 0 );
    اگر ( error )
      برو Exit;

    error = FT_Stream_Read( stream, (FT_Byte*)header, 128 );
    اگر ( error )
      برو Exit;

    اگر (            header[ 0] !=   0 ||
                    header[74] !=   0 ||
                    header[82] !=   0 ||
                    header[ 1] ==   0 ||
                    header[ 1] >   33 ||
                    header[63] !=   0 ||
         header[2 + header[1]] !=   0 ||
                  header[0x53] > 0x7F )
      بازگشت FT_THROW( Unknown_File_Format );

    dlen = ( header[0x53] << 24 ) |
           ( header[0x54] << 16 ) |
           ( header[0x55] <<  8 ) |
             header[0x56];
#اگر 0
    rlen = ( header[0x57] << 24 ) |
           ( header[0x58] << 16 ) |
           ( header[0x59] <<  8 ) |
             header[0x5A];
#endif /* 0 */
    offset = 128 + ( ( dlen + 127 ) & ~127 );

    بازگشت IsMacResource( library, stream, offset, face_index, aface );

  Exit:
    بازگشت error;
  }


  ایستا FT_Error
  load_face_in_embedded_rfork( FT_Library           library,
                               FT_Stream            stream,
                               FT_Long              face_index,
                               FT_Face             *aface,
                               ثابت FT_Open_Args  *args )
  {

#undef  FT_COMPONENT
#define FT_COMPONENT  raccess

    FT_Memory  memory = library->memory;
    FT_Error   error  = FT_ERR( Unknown_File_Format );
    FT_UInt    i;

    نویسه*      file_names[FT_RACCESS_N_RULES];
    FT_Long    offsets[FT_RACCESS_N_RULES];
    FT_Error   errors[FT_RACCESS_N_RULES];
    FT_Bool    is_darwin_vfs, vfs_rfork_has_no_font = FALSE; /* not tested */

    FT_Open_Args  args2;
    FT_Stream     stream2 = پوچ;


    FT_Raccess_Guess( library, stream,
                      args->pathname, file_names, offsets, errors );

    برای ( i = 0; i < FT_RACCESS_N_RULES; i++ )
    {
      is_darwin_vfs = ft_raccess_rule_by_darwin_vfs( library, i );
      اگر ( is_darwin_vfs && vfs_rfork_has_no_font )
      {
        FT_TRACE3(( "Skip rule %u: darwin vfs resource fork"
                    " is already checked and"
                    " no font is found\n",
                    i ));
        ادامه;
      }

      اگر ( errors[i] )
      {
        FT_TRACE3(( "Error 0x%x has occurred in rule %u\n",
                    errors[i], i ));
        ادامه;
      }

      args2.flags    = FT_OPEN_PATHNAME;
      args2.pathname = file_names[i] ? file_names[i] : args->pathname;

      FT_TRACE3(( "Try rule %u: %s (offset=%ld) ...",
                  i, args2.pathname, offsets[i] ));

      error = FT_Stream_New( library, &args2, &stream2 );
      اگر ( is_darwin_vfs && FT_ERR_EQ( error, Cannot_Open_Stream ) )
        vfs_rfork_has_no_font = TRUE;

      اگر ( error )
      {
        FT_TRACE3(( "failed\n" ));
        ادامه;
      }

      error = IsMacResource( library, stream2, offsets[i],
                             face_index, aface );
      FT_Stream_Free( stream2, 0 );

      FT_TRACE3(( "%s\n", error ? "failed": "successful" ));

      اگر ( !error )
          بشکن;
      وگرنه اگر ( is_darwin_vfs )
          vfs_rfork_has_no_font = TRUE;
    }

    برای (i = 0; i < FT_RACCESS_N_RULES; i++)
    {
      اگر ( file_names[i] )
        FT_FREE( file_names[i] );
    }

    /* Caller (load_mac_face) requires FT_Err_Unknown_File_Format. */
    اگر ( error )
      error = FT_ERR( Unknown_File_Format );

    بازگشت error;

#undef  FT_COMPONENT
#define FT_COMPONENT  objs

  }


  /* Check for some macintosh formats without Carbon framework.    */
  /* Is this a macbinary file?  If so look at the resource fork.   */
  /* Is this a mac dfont file?                                     */
  /* Is this an old style resource fork? (in data)                 */
  /* Else call load_face_in_embedded_rfork to try extra rules      */
  /* (defined in `ftrfork.c').                                     */
  /*                                                               */
  ایستا FT_Error
  load_mac_face( FT_Library           library,
                 FT_Stream            stream,
                 FT_Long              face_index,
                 FT_Face             *aface,
                 ثابت FT_Open_Args  *args )
  {
    FT_Error error;
    FT_UNUSED( args );


    error = Is_Mac_دودویی( library, stream, face_index, aface );
    اگر ( FT_ERR_EQ( error, Unknown_File_Format ) )
    {

#undef  FT_COMPONENT
#define FT_COMPONENT  raccess

#ifdef FT_DEBUG_LEVEL_TRACE
      FT_TRACE3(( "Try as dfont: " ));
      اگر ( !( args->flags & FT_OPEN_MEMORY ) )
        FT_TRACE3(( "%s ...", args->pathname ));
#endif

      error = IsMacResource( library, stream, 0, face_index, aface );

      FT_TRACE3(( "%s\n", error ? "failed" : "successful" ));

#undef  FT_COMPONENT
#define FT_COMPONENT  objs

    }

    اگر ( ( FT_ERR_EQ( error, Unknown_File_Format )      ||
           FT_ERR_EQ( error, Invalid_Stream_Operation ) ) &&
         ( args->flags & FT_OPEN_PATHNAME )               )
      error = load_face_in_embedded_rfork( library, stream,
                                           face_index, aface, args );
    بازگشت error;
  }
#endif

#endif  /* !FT_MACINTOSH && FT_CONFIG_OPTION_MAC_FONTS */


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Open_Face( FT_Library           library,
                ثابت FT_Open_Args*  args,
                FT_Long              face_index,
                FT_Face             *aface )
  {
    بازگشت ft_open_face_internal( library, args, face_index, aface, 1 );
  }


  ایستا FT_Error
  ft_open_face_internal( FT_Library           library,
                         ثابت FT_Open_Args*  args,
                         FT_Long              face_index,
                         FT_Face             *aface,
                         FT_Bool              test_mac_fonts )
  {
    FT_Error     error;
    FT_Driver    driver = پوچ;
    FT_Memory    memory = پوچ;
    FT_Stream    stream = پوچ;
    FT_Face      face   = پوچ;
    FT_List_گره  گره   = پوچ;
    FT_Bool      external_stream;
    FT_Module*   cur;
    FT_Module*   limit;

#ifndef FT_CONFIG_OPTION_MAC_FONTS
    FT_UNUSED( test_mac_fonts );
#endif


    /* only use lower 31 bits together with sign bit */
    اگر ( face_index > 0 )
      face_index &= 0x7FFFFFFFL;
    وگرنه
    {
      face_index  = -face_index;
      face_index &= 0x7FFFFFFFL;
      face_index  = -face_index;
    }

#ifdef FT_DEBUG_LEVEL_TRACE
    FT_TRACE3(( "FT_Open_Face: " ));
    اگر ( face_index < 0 )
      FT_TRACE3(( "Requesting number of faces and named instances\n"));
    وگرنه
    {
      FT_TRACE3(( "Requesting face %ld", face_index & 0xFFFFL ));
      اگر ( face_index & 0x7FFF0000L )
        FT_TRACE3(( ", named instance %ld", face_index >> 16 ));
      FT_TRACE3(( "\n" ));
    }
#endif

    /* test for valid `library' delayed to `FT_Stream_New' */

    اگر ( !args )
      بازگشت FT_THROW( Invalid_Argument );

    external_stream = FT_BOOL( ( args->flags & FT_OPEN_STREAM ) &&
                               args->stream                     );

    /* create input stream */
    error = FT_Stream_New( library, args, &stream );
    اگر ( error )
      برو Fail3;

    /* Do this error check after `FT_Stream_New` to ensure that the */
    /* 'close' callback is called.                                  */
    اگر ( !aface && face_index >= 0 )
    {
      error = FT_THROW( Invalid_Argument );
      برو Fail3;
    }

    memory = library->memory;

    /* If the font driver is specified in the `args' structure, use */
    /* it.  Otherwise, we scan the list of registered drivers.      */
    اگر ( ( args->flags & FT_OPEN_DRIVER ) && args->driver )
    {
      driver = FT_DRIVER( args->driver );

      /* not all modules are drivers, so check... */
      اگر ( FT_MODULE_IS_DRIVER( driver ) )
      {
        FT_Int         رقم_params = 0;
        FT_Parameter*  params     = پوچ;


        اگر ( args->flags & FT_OPEN_PARAMS )
        {
          رقم_params = args->رقم_params;
          params     = args->params;
        }

        error = open_face( driver, &stream, &external_stream, face_index,
                           رقم_params, params, &face );
        اگر ( !error )
          برو Success;
      }
      وگرنه
        error = FT_THROW( Invalid_Handle );

      FT_Stream_Free( stream, external_stream );
      برو Fail;
    }
    وگرنه
    {
      error = FT_ERR( Missing_Module );

      /* check each font driver for an appropriate format */
      cur   = library->modules;
      limit = cur + library->رقم_modules;

      برای ( ; cur < limit; cur++ )
      {
        /* not all modules are font drivers, so check... */
        اگر ( FT_MODULE_IS_DRIVER( cur[0] ) )
        {
          FT_Int         رقم_params = 0;
          FT_Parameter*  params     = پوچ;


          driver = FT_DRIVER( cur[0] );

          اگر ( args->flags & FT_OPEN_PARAMS )
          {
            رقم_params = args->رقم_params;
            params     = args->params;
          }

          error = open_face( driver, &stream, &external_stream, face_index,
                             رقم_params, params, &face );
          اگر ( !error )
            برو Success;

#ifdef FT_CONFIG_OPTION_MAC_FONTS
          اگر ( test_mac_fonts                                           &&
               ft_strcmp( cur[0]->clazz->module_name, "truetype" ) == 0 &&
               FT_ERR_EQ( error, Table_Missing )                        )
          {
            /* TrueType but essential tables are missing */
            error = FT_Stream_Seek( stream, 0 );
            اگر ( error )
              بشکن;

            error = open_face_PS_from_sfnt_stream( library,
                                                   stream,
                                                   face_index,
                                                   رقم_params,
                                                   params,
                                                   aface );
            اگر ( !error )
            {
              FT_Stream_Free( stream, external_stream );
              بازگشت error;
            }
          }
#endif

          اگر ( FT_ERR_NEQ( error, Unknown_File_Format ) )
            برو Fail3;
        }
      }

    Fail3:
      /* If we are on the mac, and we get an                          */
      /* FT_Err_Invalid_Stream_Operation it may be because we have an */
      /* empty data fork, so we need to check the resource fork.      */
      اگر ( FT_ERR_NEQ( error, Cannot_Open_Stream )       &&
           FT_ERR_NEQ( error, Unknown_File_Format )      &&
           FT_ERR_NEQ( error, Invalid_Stream_Operation ) )
        برو Fail2;

#اگر !defined( FT_MACINTOSH ) && defined( FT_CONFIG_OPTION_MAC_FONTS )
      اگر ( test_mac_fonts )
      {
        error = load_mac_face( library, stream, face_index, aface, args );
        اگر ( !error )
        {
          /* We don't want to go to Success here.  We've already done   */
          /* that.  On the other hand, if we succeeded we still need to */
          /* close this stream (we opened a different stream which      */
          /* extracted the interesting information out of this stream   */
          /* here.  That stream will still be open and the face will    */
          /* point to it).                                              */
          FT_Stream_Free( stream, external_stream );
          بازگشت error;
        }
      }

      اگر ( FT_ERR_NEQ( error, Unknown_File_Format ) )
        برو Fail2;
#endif  /* !FT_MACINTOSH && FT_CONFIG_OPTION_MAC_FONTS */

      /* no driver is able to handle this format */
      error = FT_THROW( Unknown_File_Format );

  Fail2:
      FT_Stream_Free( stream, external_stream );
      برو Fail;
    }

  Success:
    FT_TRACE4(( "FT_Open_Face: New face object, adding to list\n" ));

    /* add the face object to its driver's list */
    اگر ( FT_QNEW( گره ) )
      برو Fail;

    گره->data = face;
    /* don't assume driver is the same as face->driver, so use */
    /* face->driver instead.                                   */
    FT_List_Add( &face->driver->faces_list, گره );

    /* now allocate a glyph slot object for the face */
    FT_TRACE4(( "FT_Open_Face: Creating glyph slot\n" ));

    اگر ( face_index >= 0 )
    {
      error = FT_New_GlyphSlot( face, پوچ );
      اگر ( error )
        برو Fail;

      /* finally, allocate a size object for the face */
      {
        FT_Size  size;


        FT_TRACE4(( "FT_Open_Face: Creating size object\n" ));

        error = FT_New_Size( face, &size );
        اگر ( error )
          برو Fail;

        face->size = size;
      }
    }

    /* some checks */

    اگر ( FT_IS_SCALABLE( face ) )
    {
      اگر ( face->height < 0 )
        face->height = (FT_Short)-face->height;

      اگر ( !FT_HAS_VERTICAL( face ) )
        face->max_advance_height = (FT_Short)face->height;
    }

    اگر ( FT_HAS_FIXED_SIZES( face ) )
    {
      FT_Int  i;


      برای ( i = 0; i < face->رقم_fixed_sizes; i++ )
      {
        FT_Bitmap_Size*  bsize = face->available_sizes + i;


        اگر ( bsize->height < 0 )
          bsize->height = -bsize->height;
        اگر ( bsize->x_ppem < 0 )
          bsize->x_ppem = -bsize->x_ppem;
        اگر ( bsize->y_ppem < 0 )
          bsize->y_ppem = -bsize->y_ppem;

        /* check whether negation actually has worked */
        اگر ( bsize->height < 0 || bsize->x_ppem < 0 || bsize->y_ppem < 0 )
        {
          FT_TRACE0(( "FT_Open_Face:"
                      " Invalid bitmap dimensions for strike %d,"
                      " now disabled\n", i ));
          bsize->width  = 0;
          bsize->height = 0;
          bsize->size   = 0;
          bsize->x_ppem = 0;
          bsize->y_ppem = 0;
        }
      }
    }

    /* initialize internal face data */
    {
      FT_Face_Internal  internal = face->internal;


      internal->transform_matrix.xx = 0x10000L;
      internal->transform_matrix.xy = 0;
      internal->transform_matrix.yx = 0;
      internal->transform_matrix.yy = 0x10000L;

      internal->transform_delta.x = 0;
      internal->transform_delta.y = 0;

      internal->refcount = 1;

      internal->no_stem_darkening = -1;
    }

    اگر ( aface )
      *aface = face;
    وگرنه
      FT_Done_Face( face );

    برو Exit;

  Fail:
    اگر ( گره )
      FT_Done_Face( face );    /* face must be in the driver's list */
    وگرنه اگر ( face )
      destroy_face( memory, face, driver );

  Exit:
#ifdef FT_DEBUG_LEVEL_TRACE
    اگر ( !error && face_index < 0 )
    {
      FT_TRACE3(( "FT_Open_Face: The font has %ld face%s\n",
                  face->رقم_faces,
                  face->رقم_faces == 1 ? "" : "s" ));
      FT_TRACE3(( "              and %ld named instance%s for face %ld\n",
                  face->style_flags >> 16,
                  ( face->style_flags >> 16 ) == 1 ? "" : "s",
                  -face_index - 1 ));
    }
#endif

    FT_TRACE4(( "FT_Open_Face: Return 0x%x\n", error ));

    بازگشت error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Attach_File( FT_Face      face,
                  ثابت نویسه*  filepathname )
  {
    FT_Open_Args  open;


    /* test for valid `face' delayed to `FT_Attach_Stream' */

    اگر ( !filepathname )
      بازگشت FT_THROW( Invalid_Argument );

    open.stream   = پوچ;
    open.flags    = FT_OPEN_PATHNAME;
    open.pathname = (نویسه*)filepathname;

    بازگشت FT_Attach_Stream( face, &open );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Attach_Stream( FT_Face              face,
                    ثابت FT_Open_Args*  parameters )
  {
    FT_Stream  stream;
    FT_Error   error;
    FT_Driver  driver;

    FT_Driver_Class  clazz;


    /* test for valid `parameters' delayed to `FT_Stream_New' */

    اگر ( !face )
      بازگشت FT_THROW( Invalid_Face_Handle );

    driver = face->driver;
    اگر ( !driver )
      بازگشت FT_THROW( Invalid_Driver_Handle );

    error = FT_Stream_New( driver->ریشه.library, parameters, &stream );
    اگر ( error )
      برو Exit;

    /* we implement FT_Attach_Stream in each driver through the */
    /* `attach_file' interface                                  */

    error = FT_ERR( Unimplemented_Feature );
    clazz = driver->clazz;
    اگر ( clazz->attach_file )
      error = clazz->attach_file( face, stream );

    /* close the attached stream */
    FT_Stream_Free( stream,
                    FT_BOOL( parameters->stream                     &&
                             ( parameters->flags & FT_OPEN_STREAM ) ) );

  Exit:
    بازگشت error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Reference_Face( FT_Face  face )
  {
    اگر ( !face )
      بازگشت FT_THROW( Invalid_Face_Handle );

    face->internal->refcount++;

    بازگشت FT_Err_Ok;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Done_Face( FT_Face  face )
  {
    FT_Error     error;
    FT_Driver    driver;
    FT_Memory    memory;
    FT_List_گره  گره;


    error = FT_ERR( Invalid_Face_Handle );
    اگر ( face && face->driver )
    {
      face->internal->refcount--;
      اگر ( face->internal->refcount > 0 )
        error = FT_Err_Ok;
      وگرنه
      {
        driver = face->driver;
        memory = driver->ریشه.memory;

        /* find face in driver's list */
        گره = FT_List_Find( &driver->faces_list, face );
        اگر ( گره )
        {
          /* remove face object from the driver's list */
          FT_List_Remove( &driver->faces_list, گره );
          FT_FREE( گره );

          /* now destroy the object proper */
          destroy_face( memory, face, driver );
          error = FT_Err_Ok;
        }
      }
    }

    بازگشت error;
  }


  /* documentation is in ftobjs.h */

  FT_EXPORT_DEF( FT_Error )
  FT_New_Size( FT_Face   face,
               FT_Size  *asize )
  {
    FT_Error         error;
    FT_Memory        memory;
    FT_Driver        driver;
    FT_Driver_Class  clazz;

    FT_Size          size = پوچ;
    FT_List_گره      گره = پوچ;

    FT_Size_Internal  internal = پوچ;


    اگر ( !face )
      بازگشت FT_THROW( Invalid_Face_Handle );

    اگر ( !asize )
      بازگشت FT_THROW( Invalid_Argument );

    اگر ( !face->driver )
      بازگشت FT_THROW( Invalid_Driver_Handle );

    *asize = پوچ;

    driver = face->driver;
    clazz  = driver->clazz;
    memory = face->memory;

    /* Allocate new size object and perform basic initialisation */
    اگر ( FT_ALLOC( size, clazz->size_object_size ) || FT_QNEW( گره ) )
      برو Exit;

    size->face = face;

    اگر ( FT_NEW( internal ) )
      برو Exit;

    size->internal = internal;

    اگر ( clazz->init_size )
      error = clazz->init_size( size );

    /* in case of success, add to the face's list */
    اگر ( !error )
    {
      *asize     = size;
      گره->data = size;
      FT_List_Add( &face->sizes_list, گره );
    }

  Exit:
    اگر ( error )
    {
      FT_FREE( گره );
      اگر ( size )
        FT_FREE( size->internal );
      FT_FREE( size );
    }

    بازگشت error;
  }


  /* documentation is in ftobjs.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Done_Size( FT_Size  size )
  {
    FT_Error     error;
    FT_Driver    driver;
    FT_Memory    memory;
    FT_Face      face;
    FT_List_گره  گره;


    اگر ( !size )
      بازگشت FT_THROW( Invalid_Size_Handle );

    face = size->face;
    اگر ( !face )
      بازگشت FT_THROW( Invalid_Face_Handle );

    driver = face->driver;
    اگر ( !driver )
      بازگشت FT_THROW( Invalid_Driver_Handle );

    memory = driver->ریشه.memory;

    error = FT_Err_Ok;
    گره  = FT_List_Find( &face->sizes_list, size );
    اگر ( گره )
    {
      FT_List_Remove( &face->sizes_list, گره );
      FT_FREE( گره );

      اگر ( face->size == size )
      {
        face->size = پوچ;
        اگر ( face->sizes_list.head )
          face->size = (FT_Size)(face->sizes_list.head->data);
      }

      destroy_size( memory, size, driver );
    }
    وگرنه
      error = FT_THROW( Invalid_Size_Handle );

    بازگشت error;
  }


  /* documentation is in ftobjs.h */

  FT_BASE_DEF( FT_Error )
  FT_Match_Size( FT_Face          face,
                 FT_Size_Request  req,
                 FT_Bool          ignore_width,
                 FT_ULong*        size_index )
  {
    FT_Int   i;
    FT_Long  w, h;


    اگر ( !FT_HAS_FIXED_SIZES( face ) )
      بازگشت FT_THROW( Invalid_Face_Handle );

    /* FT_Bitmap_Size doesn't provide enough info... */
    اگر ( req->type != FT_SIZE_REQUEST_TYPE_NOMINAL )
      بازگشت FT_THROW( Unimplemented_Feature );

    w = FT_REQUEST_WIDTH ( req );
    h = FT_REQUEST_HEIGHT( req );

    اگر ( req->width && !req->height )
      h = w;
    وگرنه اگر ( !req->width && req->height )
      w = h;

    w = FT_PIX_ROUND( w );
    h = FT_PIX_ROUND( h );

    اگر ( !w || !h )
      بازگشت FT_THROW( Invalid_Pixel_Size );

    برای ( i = 0; i < face->رقم_fixed_sizes; i++ )
    {
      FT_Bitmap_Size*  bsize = face->available_sizes + i;


      اگر ( h != FT_PIX_ROUND( bsize->y_ppem ) )
        ادامه;

      اگر ( w == FT_PIX_ROUND( bsize->x_ppem ) || ignore_width )
      {
        FT_TRACE3(( "FT_Match_Size: bitmap strike %d matches\n", i ));

        اگر ( size_index )
          *size_index = (FT_ULong)i;

        بازگشت FT_Err_Ok;
      }
    }

    FT_TRACE3(( "FT_Match_Size: no matching bitmap strike\n" ));

    بازگشت FT_THROW( Invalid_Pixel_Size );
  }


  /* documentation is in ftobjs.h */

  FT_BASE_DEF( تهی )
  ft_synthesize_vertical_metrics( FT_Glyph_Metrics*  metrics,
                                  FT_Pos             advance )
  {
    FT_Pos  height = metrics->height;


    /* compensate for glyph with bbox above/below the baseline */
    اگر ( metrics->horiBearingY < 0 )
    {
      اگر ( height < metrics->horiBearingY )
        height = metrics->horiBearingY;
    }
    وگرنه اگر ( metrics->horiBearingY > 0 )
      height -= metrics->horiBearingY;

    /* the factor 1.2 is a heuristical value */
    اگر ( !advance )
      advance = height * 12 / 10;

    metrics->vertBearingX = metrics->horiBearingX - metrics->horiAdvance / 2;
    metrics->vertBearingY = ( advance - height ) / 2;
    metrics->vertAdvance  = advance;
  }


  ایستا تهی
  ft_recompute_scaled_metrics( FT_Face           face,
                               FT_Size_Metrics*  metrics )
  {
    /* Compute root ascender, descender, test height, and max_advance */

#ifdef GRID_FIT_METRICS
    metrics->ascender    = FT_PIX_CEIL( FT_MulFix( face->ascender,
                                                   metrics->y_scale ) );

    metrics->descender   = FT_PIX_FLOOR( FT_MulFix( face->descender,
                                                    metrics->y_scale ) );

    metrics->height      = FT_PIX_ROUND( FT_MulFix( face->height,
                                                    metrics->y_scale ) );

    metrics->max_advance = FT_PIX_ROUND( FT_MulFix( face->max_advance_width,
                                                    metrics->x_scale ) );
#وگرنه /* !GRID_FIT_METRICS */
    metrics->ascender    = FT_MulFix( face->ascender,
                                      metrics->y_scale );

    metrics->descender   = FT_MulFix( face->descender,
                                      metrics->y_scale );

    metrics->height      = FT_MulFix( face->height,
                                      metrics->y_scale );

    metrics->max_advance = FT_MulFix( face->max_advance_width,
                                      metrics->x_scale );
#endif /* !GRID_FIT_METRICS */
  }


  FT_BASE_DEF( تهی )
  FT_Select_Metrics( FT_Face   face,
                     FT_ULong  strike_index )
  {
    FT_Size_Metrics*  metrics;
    FT_Bitmap_Size*   bsize;


    metrics = &face->size->metrics;
    bsize   = face->available_sizes + strike_index;

    metrics->x_ppem = (FT_UShort)( ( bsize->x_ppem + 32 ) >> 6 );
    metrics->y_ppem = (FT_UShort)( ( bsize->y_ppem + 32 ) >> 6 );

    اگر ( FT_IS_SCALABLE( face ) )
    {
      metrics->x_scale = FT_DivFix( bsize->x_ppem,
                                    face->units_per_EM );
      metrics->y_scale = FT_DivFix( bsize->y_ppem,
                                    face->units_per_EM );

      ft_recompute_scaled_metrics( face, metrics );
    }
    وگرنه
    {
      metrics->x_scale     = 1L << 16;
      metrics->y_scale     = 1L << 16;
      metrics->ascender    = bsize->y_ppem;
      metrics->descender   = 0;
      metrics->height      = bsize->height << 6;
      metrics->max_advance = bsize->x_ppem;
    }
  }


  FT_BASE_DEF( FT_Error )
  FT_Request_Metrics( FT_Face          face,
                      FT_Size_Request  req )
  {
    FT_Error  error = FT_Err_Ok;

    FT_Size_Metrics*  metrics;


    metrics = &face->size->metrics;

    اگر ( FT_IS_SCALABLE( face ) )
    {
      FT_Long  w = 0, h = 0, scaled_w = 0, scaled_h = 0;


      انتخاب ( req->type )
      {
      حالت FT_SIZE_REQUEST_TYPE_NOMINAL:
        w = h = face->units_per_EM;
        بشکن;

      حالت FT_SIZE_REQUEST_TYPE_REAL_DIM:
        w = h = face->ascender - face->descender;
        بشکن;

      حالت FT_SIZE_REQUEST_TYPE_BBOX:
        w = face->bbox.xMax - face->bbox.xMin;
        h = face->bbox.yMax - face->bbox.yMin;
        بشکن;

      حالت FT_SIZE_REQUEST_TYPE_CELL:
        w = face->max_advance_width;
        h = face->ascender - face->descender;
        بشکن;

      حالت FT_SIZE_REQUEST_TYPE_SCALES:
        metrics->x_scale = (FT_Fixed)req->width;
        metrics->y_scale = (FT_Fixed)req->height;
        اگر ( !metrics->x_scale )
          metrics->x_scale = metrics->y_scale;
        وگرنه اگر ( !metrics->y_scale )
          metrics->y_scale = metrics->x_scale;
        برو Calculate_Ppem;

      حالت FT_SIZE_REQUEST_TYPE_MAX:
        بشکن;
      }

      /* to be on the safe side */
      اگر ( w < 0 )
        w = -w;

      اگر ( h < 0 )
        h = -h;

      scaled_w = FT_REQUEST_WIDTH ( req );
      scaled_h = FT_REQUEST_HEIGHT( req );

      /* determine scales */
      اگر ( req->height || !req->width )
      {
        اگر ( h == 0 )
        {
          FT_ERROR(( "FT_Request_Metrics: Divide by zero\n" ));
          error = FT_ERR( Divide_By_Zero );
          برو Exit;
        }

        metrics->y_scale = FT_DivFix( scaled_h, h );
      }

      اگر ( req->width )
      {
        اگر ( w == 0 )
        {
          FT_ERROR(( "FT_Request_Metrics: Divide by zero\n" ));
          error = FT_ERR( Divide_By_Zero );
          برو Exit;
        }

        metrics->x_scale = FT_DivFix( scaled_w, w );
      }
      وگرنه
      {
        metrics->x_scale = metrics->y_scale;
        scaled_w = FT_MulDiv( scaled_h, w, h );
      }

      اگر ( !req->height )
      {
        metrics->y_scale = metrics->x_scale;
        scaled_h = FT_MulDiv( scaled_w, h, w );
      }

      اگر ( req->type == FT_SIZE_REQUEST_TYPE_CELL )
      {
        اگر ( metrics->y_scale > metrics->x_scale )
          metrics->y_scale = metrics->x_scale;
        وگرنه
          metrics->x_scale = metrics->y_scale;
      }

  Calculate_Ppem:
      /* calculate the ppems */
      اگر ( req->type != FT_SIZE_REQUEST_TYPE_NOMINAL )
      {
        scaled_w = FT_MulFix( face->units_per_EM, metrics->x_scale );
        scaled_h = FT_MulFix( face->units_per_EM, metrics->y_scale );
      }

      scaled_w = ( scaled_w + 32 ) >> 6;
      scaled_h = ( scaled_h + 32 ) >> 6;
      اگر ( scaled_w > (FT_Long)FT_USHORT_MAX ||
           scaled_h > (FT_Long)FT_USHORT_MAX )
      {
        FT_ERROR(( "FT_Request_Metrics: Resulting ppem size too large\n" ));
        error = FT_ERR( Invalid_Pixel_Size );
        برو Exit;
      }

      metrics->x_ppem = (FT_UShort)scaled_w;
      metrics->y_ppem = (FT_UShort)scaled_h;

      ft_recompute_scaled_metrics( face, metrics );
    }
    وگرنه
    {
      FT_ZERO( metrics );
      metrics->x_scale = 1L << 16;
      metrics->y_scale = 1L << 16;
    }

  Exit:
    بازگشت error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Select_Size( FT_Face  face,
                  FT_Int   strike_index )
  {
    FT_Error         error = FT_Err_Ok;
    FT_Driver_Class  clazz;


    اگر ( !face || !FT_HAS_FIXED_SIZES( face ) )
      بازگشت FT_THROW( Invalid_Face_Handle );

    اگر ( strike_index < 0 || strike_index >= face->رقم_fixed_sizes )
      بازگشت FT_THROW( Invalid_Argument );

    clazz = face->driver->clazz;

    اگر ( clazz->select_size )
    {
      error = clazz->select_size( face->size, (FT_ULong)strike_index );

      FT_TRACE5(( "FT_Select_Size (%s driver):\n",
                  face->driver->ریشه.clazz->module_name ));
    }
    وگرنه
    {
      FT_Select_Metrics( face, (FT_ULong)strike_index );

      FT_TRACE5(( "FT_Select_Size:\n" ));
    }

#ifdef FT_DEBUG_LEVEL_TRACE
    {
      FT_Size_Metrics*  metrics = &face->size->metrics;


      FT_TRACE5(( "  x scale: %ld (%f)\n",
                  metrics->x_scale, (دوگانه)metrics->x_scale / 65536 ));
      FT_TRACE5(( "  y scale: %ld (%f)\n",
                  metrics->y_scale, (دوگانه)metrics->y_scale / 65536 ));
      FT_TRACE5(( "  ascender: %f\n",
                  (دوگانه)metrics->ascender / 64 ));
      FT_TRACE5(( "  descender: %f\n",
                  (دوگانه)metrics->descender / 64 ));
      FT_TRACE5(( "  height: %f\n",
                  (دوگانه)metrics->height / 64 ));
      FT_TRACE5(( "  max advance: %f\n",
                  (دوگانه)metrics->max_advance / 64 ));
      FT_TRACE5(( "  x ppem: %d\n", metrics->x_ppem ));
      FT_TRACE5(( "  y ppem: %d\n", metrics->y_ppem ));
    }
#endif

    بازگشت error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Request_Size( FT_Face          face,
                   FT_Size_Request  req )
  {
    FT_Error         error;
    FT_Driver_Class  clazz;
    FT_ULong         strike_index;


    اگر ( !face )
      بازگشت FT_THROW( Invalid_Face_Handle );

    اگر ( !face->size )
      بازگشت FT_THROW( Invalid_Size_Handle );

    اگر ( !req || req->width < 0 || req->height < 0 ||
         req->type >= FT_SIZE_REQUEST_TYPE_MAX )
      بازگشت FT_THROW( Invalid_Argument );

    /* signal the auto-hinter to recompute its size metrics */
    /* (if requested)                                       */
    face->size->internal->autohint_metrics.x_scale = 0;

    clazz = face->driver->clazz;

    اگر ( clazz->request_size )
    {
      error = clazz->request_size( face->size, req );

      FT_TRACE5(( "FT_Request_Size (%s driver):\n",
                  face->driver->ریشه.clazz->module_name ));
    }
    وگرنه اگر ( !FT_IS_SCALABLE( face ) && FT_HAS_FIXED_SIZES( face ) )
    {
      /*
       * The reason that a driver doesn't have `request_size' defined is
       * either that the scaling here suffices or that the supported formats
       * are bitmap-only and size matching is not implemented.
       *
       * In the latter case, a simple size matching is done.
       */
      error = FT_Match_Size( face, req, 0, &strike_index );
      اگر ( error )
        برو Exit;

      بازگشت FT_Select_Size( face, (FT_Int)strike_index );
    }
    وگرنه
    {
      error = FT_Request_Metrics( face, req );
      اگر ( error )
        برو Exit;

      FT_TRACE5(( "FT_Request_Size:\n" ));
    }

#ifdef FT_DEBUG_LEVEL_TRACE
    {
      FT_Size_Metrics*  metrics = &face->size->metrics;


      FT_TRACE5(( "  x scale: %ld (%f)\n",
                  metrics->x_scale, (دوگانه)metrics->x_scale / 65536 ));
      FT_TRACE5(( "  y scale: %ld (%f)\n",
                  metrics->y_scale, (دوگانه)metrics->y_scale / 65536 ));
      FT_TRACE5(( "  ascender: %f\n",
                  (دوگانه)metrics->ascender / 64 ));
      FT_TRACE5(( "  descender: %f\n",
                  (دوگانه)metrics->descender / 64 ));
      FT_TRACE5(( "  height: %f\n",
                  (دوگانه)metrics->height / 64 ));
      FT_TRACE5(( "  max advance: %f\n",
                  (دوگانه)metrics->max_advance / 64 ));
      FT_TRACE5(( "  x ppem: %d\n", metrics->x_ppem ));
      FT_TRACE5(( "  y ppem: %d\n", metrics->y_ppem ));
    }
#endif

  Exit:
    بازگشت error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Set_Char_Size( FT_Face     face,
                    FT_F26Dot6  char_width,
                    FT_F26Dot6  char_height,
                    FT_UInt     horz_resolution,
                    FT_UInt     vert_resolution )
  {
    FT_Size_RequestRec  req;


    /* check of `face' delayed to `FT_Request_Size' */

    اگر ( !char_width )
      char_width = char_height;
    وگرنه اگر ( !char_height )
      char_height = char_width;

    اگر ( !horz_resolution )
      horz_resolution = vert_resolution;
    وگرنه اگر ( !vert_resolution )
      vert_resolution = horz_resolution;

    اگر ( char_width  < 1 * 64 )
      char_width  = 1 * 64;
    اگر ( char_height < 1 * 64 )
      char_height = 1 * 64;

    اگر ( !horz_resolution )
      horz_resolution = vert_resolution = 72;

    req.type           = FT_SIZE_REQUEST_TYPE_NOMINAL;
    req.width          = char_width;
    req.height         = char_height;
    req.horiResolution = horz_resolution;
    req.vertResolution = vert_resolution;

    بازگشت FT_Request_Size( face, &req );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Set_Pixel_Sizes( FT_Face  face,
                      FT_UInt  pixel_width,
                      FT_UInt  pixel_height )
  {
    FT_Size_RequestRec  req;


    /* check of `face' delayed to `FT_Request_Size' */

    اگر ( pixel_width == 0 )
      pixel_width = pixel_height;
    وگرنه اگر ( pixel_height == 0 )
      pixel_height = pixel_width;

    اگر ( pixel_width  < 1 )
      pixel_width  = 1;
    اگر ( pixel_height < 1 )
      pixel_height = 1;

    /* use `>=' to avoid potential compiler warning on 16bit platforms */
    اگر ( pixel_width >= 0xFFFFU )
      pixel_width = 0xFFFFU;
    اگر ( pixel_height >= 0xFFFFU )
      pixel_height = 0xFFFFU;

    req.type           = FT_SIZE_REQUEST_TYPE_NOMINAL;
    req.width          = (FT_Long)( pixel_width << 6 );
    req.height         = (FT_Long)( pixel_height << 6 );
    req.horiResolution = 0;
    req.vertResolution = 0;

    بازگشت FT_Request_Size( face, &req );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Kerning( FT_Face     face,
                  FT_UInt     چپ_glyph,
                  FT_UInt     راست_glyph,
                  FT_UInt     kern_mode,
                  FT_Vector  *akerning )
  {
    FT_Error   error = FT_Err_Ok;
    FT_Driver  driver;


    اگر ( !face )
      بازگشت FT_THROW( Invalid_Face_Handle );

    اگر ( !akerning )
      بازگشت FT_THROW( Invalid_Argument );

    driver = face->driver;

    akerning->x = 0;
    akerning->y = 0;

    اگر ( driver->clazz->get_kerning )
    {
      error = driver->clazz->get_kerning( face,
                                          چپ_glyph,
                                          راست_glyph,
                                          akerning );
      اگر ( !error )
      {
        اگر ( kern_mode != FT_KERNING_UNSCALED )
        {
          akerning->x = FT_MulFix( akerning->x, face->size->metrics.x_scale );
          akerning->y = FT_MulFix( akerning->y, face->size->metrics.y_scale );

          اگر ( kern_mode != FT_KERNING_UNFITTED )
          {
            FT_Pos  orig_x = akerning->x;
            FT_Pos  orig_y = akerning->y;


            /* we scale down kerning values for small ppem values */
            /* to avoid that rounding makes them too big.         */
            /* `25' has been determined heuristically.            */
            اگر ( face->size->metrics.x_ppem < 25 )
              akerning->x = FT_MulDiv( orig_x,
                                       face->size->metrics.x_ppem, 25 );
            اگر ( face->size->metrics.y_ppem < 25 )
              akerning->y = FT_MulDiv( orig_y,
                                       face->size->metrics.y_ppem, 25 );

            akerning->x = FT_PIX_ROUND( akerning->x );
            akerning->y = FT_PIX_ROUND( akerning->y );

#ifdef FT_DEBUG_LEVEL_TRACE
            {
              FT_Pos  orig_x_rounded = FT_PIX_ROUND( orig_x );
              FT_Pos  orig_y_rounded = FT_PIX_ROUND( orig_y );


              اگر ( akerning->x != orig_x_rounded ||
                   akerning->y != orig_y_rounded )
                FT_TRACE5(( "FT_Get_Kerning: horizontal kerning"
                            " (%ld, %ld) scaled down to (%ld, %ld) pixels\n",
                            orig_x_rounded / 64, orig_y_rounded / 64,
                            akerning->x / 64, akerning->y / 64 ));
            }
#endif
          }
        }
      }
    }

    بازگشت error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Track_Kerning( FT_Face    face,
                        FT_Fixed   point_size,
                        FT_Int     degree,
                        FT_Fixed*  akerning )
  {
    FT_Service_Kerning  service;
    FT_Error            error = FT_Err_Ok;


    اگر ( !face )
      بازگشت FT_THROW( Invalid_Face_Handle );

    اگر ( !akerning )
      بازگشت FT_THROW( Invalid_Argument );

    FT_FACE_FIND_SERVICE( face, service, KERNING );
    اگر ( !service )
      بازگشت FT_THROW( Unimplemented_Feature );

    error = service->get_track( face,
                                point_size,
                                degree,
                                akerning );

    بازگشت error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Select_Charmap( FT_Face      face,
                     FT_Encoding  encoding )
  {
    FT_CharMap*  cur;
    FT_CharMap*  limit;


    اگر ( !face )
      بازگشت FT_THROW( Invalid_Face_Handle );

    /* FT_ENCODING_NONE is a valid encoding for BDF, PCF, and Windows FNT */
    اگر ( encoding == FT_ENCODING_NONE && !face->رقم_charmaps )
      بازگشت FT_THROW( Invalid_Argument );

    /* FT_ENCODING_UNICODE is special.  We try to find the `best' Unicode */
    /* charmap available, i.e., one with UCS-4 characters, if possible.   */
    /*                                                                    */
    /* This is done by find_unicode_charmap() above, to share code.       */
    اگر ( encoding == FT_ENCODING_UNICODE )
      بازگشت find_unicode_charmap( face );

    cur = face->charmaps;
    اگر ( !cur )
      بازگشت FT_THROW( Invalid_CharMap_Handle );

    limit = cur + face->رقم_charmaps;

    برای ( ; cur < limit; cur++ )
    {
      اگر ( cur[0]->encoding == encoding )
      {
        face->charmap = cur[0];
        بازگشت FT_Err_Ok;
      }
    }

    بازگشت FT_THROW( Invalid_Argument );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Set_Charmap( FT_Face     face,
                  FT_CharMap  charmap )
  {
    FT_CharMap*  cur;
    FT_CharMap*  limit;


    اگر ( !face )
      بازگشت FT_THROW( Invalid_Face_Handle );

    cur = face->charmaps;
    اگر ( !cur || !charmap )
      بازگشت FT_THROW( Invalid_CharMap_Handle );

    limit = cur + face->رقم_charmaps;

    برای ( ; cur < limit; cur++ )
    {
      اگر ( cur[0] == charmap )
      {
        FT_Long  format = FT_Get_CMap_Format( charmap );


        اگر ( format != 14 && format != 15 )
        {
          face->charmap = cur[0];
          بازگشت FT_Err_Ok;
        }
      }
    }

    بازگشت FT_THROW( Invalid_Argument );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Int )
  FT_Get_Charmap_Index( FT_CharMap  charmap )
  {
    FT_Int  i;


    اگر ( !charmap || !charmap->face )
      بازگشت -1;

    برای ( i = 0; i < charmap->face->رقم_charmaps; i++ )
      اگر ( charmap->face->charmaps[i] == charmap )
        بشکن;

    FT_ASSERT( i < charmap->face->رقم_charmaps );

    بازگشت i;
  }


  ایستا تهی
  ft_cmap_done_internal( FT_CMap  cmap )
  {
    FT_CMap_Class  clazz  = cmap->clazz;
    FT_Face        face   = cmap->charmap.face;
    FT_Memory      memory = FT_FACE_MEMORY( face );


    اگر ( clazz->done )
      clazz->done( cmap );

    FT_FREE( cmap );
  }


  FT_BASE_DEF( تهی )
  FT_CMap_Done( FT_CMap  cmap )
  {
    اگر ( cmap )
    {
      FT_Face    face   = cmap->charmap.face;
      FT_Memory  memory = FT_FACE_MEMORY( face );
      FT_Error   error;
      FT_Int     i, j;


      برای ( i = 0; i < face->رقم_charmaps; i++ )
      {
        اگر ( (FT_CMap)face->charmaps[i] == cmap )
        {
          FT_CharMap  last_charmap = face->charmaps[face->رقم_charmaps - 1];


          اگر ( FT_QRENEW_ARRAY( face->charmaps,
                                face->رقم_charmaps,
                                face->رقم_charmaps - 1 ) )
            بازگشت;

          /* remove it from our list of charmaps */
          برای ( j = i + 1; j < face->رقم_charmaps; j++ )
          {
            اگر ( j == face->رقم_charmaps - 1 )
              face->charmaps[j - 1] = last_charmap;
            وگرنه
              face->charmaps[j - 1] = face->charmaps[j];
          }

          face->رقم_charmaps--;

          اگر ( (FT_CMap)face->charmap == cmap )
            face->charmap = پوچ;

          ft_cmap_done_internal( cmap );

          بشکن;
        }
      }
    }
  }


  FT_BASE_DEF( FT_Error )
  FT_CMap_New( FT_CMap_Class  clazz,
               FT_Pointer     init_data,
               FT_CharMap     charmap,
               FT_CMap       *acmap )
  {
    FT_Error   error;
    FT_Face    face;
    FT_Memory  memory;
    FT_CMap    cmap = پوچ;


    اگر ( !clazz || !charmap || !charmap->face )
      بازگشت FT_THROW( Invalid_Argument );

    face   = charmap->face;
    memory = FT_FACE_MEMORY( face );

    اگر ( !FT_ALLOC( cmap, clazz->size ) )
    {
      cmap->charmap = *charmap;
      cmap->clazz   = clazz;

      اگر ( clazz->init )
      {
        error = clazz->init( cmap, init_data );
        اگر ( error )
          برو Fail;
      }

      /* add it to our list of charmaps */
      اگر ( FT_QRENEW_ARRAY( face->charmaps,
                            face->رقم_charmaps,
                            face->رقم_charmaps + 1 ) )
        برو Fail;

      face->charmaps[face->رقم_charmaps++] = (FT_CharMap)cmap;
    }

  Exit:
    اگر ( acmap )
      *acmap = cmap;

    بازگشت error;

  Fail:
    ft_cmap_done_internal( cmap );
    cmap = پوچ;
    برو Exit;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt )
  FT_Get_Char_Index( FT_Face   face,
                     FT_ULong  charcode )
  {
    FT_UInt  result = 0;


    اگر ( face && face->charmap )
    {
      FT_CMap  cmap = FT_CMAP( face->charmap );


      اگر ( charcode > 0xFFFFFFFFUL )
      {
        FT_TRACE1(( "FT_Get_Char_Index: too large charcode" ));
        FT_TRACE1(( " 0x%lx is truncated\n", charcode ));
      }

      result = cmap->clazz->char_index( cmap, (FT_UInt32)charcode );
      اگر ( result >= (FT_UInt)face->رقم_glyphs )
        result = 0;
    }

    بازگشت result;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_ULong )
  FT_Get_First_Char( FT_Face   face,
                     FT_UInt  *agindex )
  {
    FT_ULong  result = 0;
    FT_UInt   gindex = 0;


    /* only do something if we have a charmap, and we have glyphs at all */
    اگر ( face && face->charmap && face->رقم_glyphs )
    {
      gindex = FT_Get_Char_Index( face, 0 );
      اگر ( gindex == 0 )
        result = FT_Get_Next_Char( face, 0, &gindex );
    }

    اگر ( agindex )
      *agindex = gindex;

    بازگشت result;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_ULong )
  FT_Get_Next_Char( FT_Face   face,
                    FT_ULong  charcode,
                    FT_UInt  *agindex )
  {
    FT_ULong  result = 0;
    FT_UInt   gindex = 0;


    اگر ( face && face->charmap && face->رقم_glyphs )
    {
      FT_UInt32  code = (FT_UInt32)charcode;
      FT_CMap    cmap = FT_CMAP( face->charmap );


      انجامبده
      {
        gindex = cmap->clazz->char_next( cmap, &code );

      } وقتی ( gindex >= (FT_UInt)face->رقم_glyphs );

      result = ( gindex == 0 ) ? 0 : code;
    }

    اگر ( agindex )
      *agindex = gindex;

    بازگشت result;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Face_Properties( FT_Face        face,
                      FT_UInt        رقم_properties,
                      FT_Parameter*  properties )
  {
    FT_Error  error = FT_Err_Ok;


    اگر ( رقم_properties > 0 && !properties )
    {
      error = FT_THROW( Invalid_Argument );
      برو Exit;
    }

    برای ( ; رقم_properties > 0; رقم_properties-- )
    {
      اگر ( properties->tag == FT_PARAM_TAG_STEM_DARKENING )
      {
        اگر ( properties->data )
        {
          اگر ( *( (FT_Bool*)properties->data ) == TRUE )
            face->internal->no_stem_darkening = FALSE;
          وگرنه
            face->internal->no_stem_darkening = TRUE;
        }
        وگرنه
        {
          /* use module default */
          face->internal->no_stem_darkening = -1;
        }
      }
      وگرنه اگر ( properties->tag == FT_PARAM_TAG_LCD_FILTER_WEIGHTS )
      {
        error = FT_THROW( Unimplemented_Feature );
        برو Exit;
      }
      وگرنه اگر ( properties->tag == FT_PARAM_TAG_RANDOM_SEED )
      {
        اگر ( properties->data )
        {
          face->internal->random_seed = *( (FT_Int32*)properties->data );
          اگر ( face->internal->random_seed < 0 )
            face->internal->random_seed = 0;
        }
        وگرنه
        {
          /* use module default */
          face->internal->random_seed = -1;
        }
      }
      وگرنه
      {
        error = FT_THROW( Invalid_Argument );
        برو Exit;
      }

      اگر ( error )
        بشکن;

      properties++;
    }

  Exit:
    بازگشت error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt )
  FT_Face_GetCharVariantIndex( FT_Face   face,
                               FT_ULong  charcode,
                               FT_ULong  variantSelector )
  {
    FT_UInt  result = 0;


    اگر ( face                                           &&
         face->charmap                                  &&
         face->charmap->encoding == FT_ENCODING_UNICODE )
    {
      FT_CharMap  charmap = find_variant_selector_charmap( face );
      FT_CMap     ucmap = FT_CMAP( face->charmap );


      اگر ( charmap )
      {
        FT_CMap  vcmap = FT_CMAP( charmap );


        اگر ( charcode > 0xFFFFFFFFUL )
        {
          FT_TRACE1(( "FT_Face_GetCharVariantIndex:"
                      " too large charcode" ));
          FT_TRACE1(( " 0x%lx is truncated\n", charcode ));
        }
        اگر ( variantSelector > 0xFFFFFFFFUL )
        {
          FT_TRACE1(( "FT_Face_GetCharVariantIndex:"
                      " too large variantSelector" ));
          FT_TRACE1(( " 0x%lx is truncated\n", variantSelector ));
        }

        result = vcmap->clazz->char_var_index( vcmap, ucmap,
                                               (FT_UInt32)charcode,
                                               (FT_UInt32)variantSelector );
      }
    }

    بازگشت result;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Int )
  FT_Face_GetCharVariantIsDefault( FT_Face   face,
                                   FT_ULong  charcode,
                                   FT_ULong  variantSelector )
  {
    FT_Int  result = -1;


    اگر ( face )
    {
      FT_CharMap  charmap = find_variant_selector_charmap( face );


      اگر ( charmap )
      {
        FT_CMap  vcmap = FT_CMAP( charmap );


        اگر ( charcode > 0xFFFFFFFFUL )
        {
          FT_TRACE1(( "FT_Face_GetCharVariantIsDefault:"
                      " too large charcode" ));
          FT_TRACE1(( " 0x%lx is truncated\n", charcode ));
        }
        اگر ( variantSelector > 0xFFFFFFFFUL )
        {
          FT_TRACE1(( "FT_Face_GetCharVariantIsDefault:"
                      " too large variantSelector" ));
          FT_TRACE1(( " 0x%lx is truncated\n", variantSelector ));
        }

        result = vcmap->clazz->char_var_default( vcmap,
                                                 (FT_UInt32)charcode,
                                                 (FT_UInt32)variantSelector );
      }
    }

    بازگشت result;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt32* )
  FT_Face_GetVariantSelectors( FT_Face  face )
  {
    FT_UInt32  *result = پوچ;


    اگر ( face )
    {
      FT_CharMap  charmap = find_variant_selector_charmap( face );


      اگر ( charmap )
      {
        FT_CMap    vcmap  = FT_CMAP( charmap );
        FT_Memory  memory = FT_FACE_MEMORY( face );


        result = vcmap->clazz->variant_list( vcmap, memory );
      }
    }

    بازگشت result;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt32* )
  FT_Face_GetVariantsOfChar( FT_Face   face,
                             FT_ULong  charcode )
  {
    FT_UInt32  *result = پوچ;


    اگر ( face )
    {
      FT_CharMap  charmap = find_variant_selector_charmap( face );


      اگر ( charmap )
      {
        FT_CMap    vcmap  = FT_CMAP( charmap );
        FT_Memory  memory = FT_FACE_MEMORY( face );


        اگر ( charcode > 0xFFFFFFFFUL )
        {
          FT_TRACE1(( "FT_Face_GetVariantsOfChar: too large charcode" ));
          FT_TRACE1(( " 0x%lx is truncated\n", charcode ));
        }

        result = vcmap->clazz->charvariant_list( vcmap, memory,
                                                 (FT_UInt32)charcode );
      }
    }
    بازگشت result;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt32* )
  FT_Face_GetCharsOfVariant( FT_Face   face,
                             FT_ULong  variantSelector )
  {
    FT_UInt32  *result = پوچ;


    اگر ( face )
    {
      FT_CharMap  charmap = find_variant_selector_charmap( face );


      اگر ( charmap )
      {
        FT_CMap    vcmap  = FT_CMAP( charmap );
        FT_Memory  memory = FT_FACE_MEMORY( face );


        اگر ( variantSelector > 0xFFFFFFFFUL )
        {
          FT_TRACE1(( "FT_Get_Char_Index: too large variantSelector" ));
          FT_TRACE1(( " 0x%lx is truncated\n", variantSelector ));
        }

        result = vcmap->clazz->variantchar_list( vcmap, memory,
                                                 (FT_UInt32)variantSelector );
      }
    }

    بازگشت result;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt )
  FT_Get_Name_Index( FT_Face           face,
                     ثابت FT_String*  glyph_name )
  {
    FT_UInt  result = 0;


    اگر ( face                       &&
         FT_HAS_GLYPH_NAMES( face ) &&
         glyph_name                 )
    {
      FT_Service_GlyphDict  service;


      FT_FACE_LOOKUP_SERVICE( face,
                              service,
                              GLYPH_DICT );

      اگر ( service && service->name_index )
        result = service->name_index( face, glyph_name );
    }

    بازگشت result;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Glyph_Name( FT_Face     face,
                     FT_UInt     glyph_index,
                     FT_Pointer  buffer,
                     FT_UInt     buffer_max )
  {
    FT_Error              error;
    FT_Service_GlyphDict  service;


    اگر ( !face )
      بازگشت FT_THROW( Invalid_Face_Handle );

    اگر ( !buffer || buffer_max == 0 )
      بازگشت FT_THROW( Invalid_Argument );

    /* clean up buffer */
    ((FT_Byte*)buffer)[0] = '\0';

    اگر ( (FT_Long)glyph_index >= face->رقم_glyphs )
      بازگشت FT_THROW( Invalid_Glyph_Index );

    اگر ( !FT_HAS_GLYPH_NAMES( face ) )
      بازگشت FT_THROW( Invalid_Argument );

    FT_FACE_LOOKUP_SERVICE( face, service, GLYPH_DICT );
    اگر ( service && service->get_name )
      error = service->get_name( face, glyph_index, buffer, buffer_max );
    وگرنه
      error = FT_THROW( Invalid_Argument );

    بازگشت error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( ثابت نویسه* )
  FT_Get_Postscript_Name( FT_Face  face )
  {
    ثابت نویسه*  result = پوچ;


    اگر ( !face )
      برو Exit;

    اگر ( !result )
    {
      FT_Service_PsFontName  service;


      FT_FACE_LOOKUP_SERVICE( face,
                              service,
                              POSTSCRIPT_FONT_NAME );

      اگر ( service && service->get_ps_font_name )
        result = service->get_ps_font_name( face );
    }

  Exit:
    بازگشت result;
  }


  /* documentation is in tttables.h */

  FT_EXPORT_DEF( تهی* )
  FT_Get_Sfnt_Table( FT_Face      face,
                     FT_Sfnt_Tag  tag )
  {
    تهی*                  table = پوچ;
    FT_Service_SFNT_Table  service;


    اگر ( face && FT_IS_SFNT( face ) )
    {
      FT_FACE_FIND_SERVICE( face, service, SFNT_TABLE );
      اگر ( service )
        table = service->get_table( face, tag );
    }

    بازگشت table;
  }


  /* documentation is in tttables.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Load_Sfnt_Table( FT_Face    face,
                      FT_ULong   tag,
                      FT_Long    offset,
                      FT_Byte*   buffer,
                      FT_ULong*  length )
  {
    FT_Service_SFNT_Table  service;


    اگر ( !face || !FT_IS_SFNT( face ) )
      بازگشت FT_THROW( Invalid_Face_Handle );

    FT_FACE_FIND_SERVICE( face, service, SFNT_TABLE );
    اگر ( !service )
      بازگشت FT_THROW( Unimplemented_Feature );

    بازگشت service->load_table( face, tag, offset, buffer, length );
  }


  /* documentation is in tttables.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Sfnt_Table_Info( FT_Face    face,
                      FT_UInt    table_index,
                      FT_ULong  *tag,
                      FT_ULong  *length )
  {
    FT_Service_SFNT_Table  service;
    FT_ULong               offset;


    /* test for valid `length' delayed to `service->table_info' */

    اگر ( !face || !FT_IS_SFNT( face ) )
      بازگشت FT_THROW( Invalid_Face_Handle );

    FT_FACE_FIND_SERVICE( face, service, SFNT_TABLE );
    اگر ( !service )
      بازگشت FT_THROW( Unimplemented_Feature );

    بازگشت service->table_info( face, table_index, tag, &offset, length );
  }


  /* documentation is in tttables.h */

  FT_EXPORT_DEF( FT_ULong )
  FT_Get_CMap_Language_ID( FT_CharMap  charmap )
  {
    FT_Service_TTCMaps  service;
    FT_Face             face;
    TT_CMapInfo         cmap_info;


    اگر ( !charmap || !charmap->face )
      بازگشت 0;

    face = charmap->face;
    FT_FACE_FIND_SERVICE( face, service, TT_CMAP );
    اگر ( !service )
      بازگشت 0;
    اگر ( service->get_cmap_info( charmap, &cmap_info ))
      بازگشت 0;

    بازگشت cmap_info.language;
  }


  /* documentation is in tttables.h */

  FT_EXPORT_DEF( FT_Long )
  FT_Get_CMap_Format( FT_CharMap  charmap )
  {
    FT_Service_TTCMaps  service;
    FT_Face             face;
    TT_CMapInfo         cmap_info;


    اگر ( !charmap || !charmap->face )
      بازگشت -1;

    face = charmap->face;
    FT_FACE_FIND_SERVICE( face, service, TT_CMAP );
    اگر ( !service )
      بازگشت -1;
    اگر ( service->get_cmap_info( charmap, &cmap_info ))
      بازگشت -1;

    بازگشت cmap_info.format;
  }


  /* documentation is in ftsizes.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Activate_Size( FT_Size  size )
  {
    FT_Face  face;


    اگر ( !size )
      بازگشت FT_THROW( Invalid_Size_Handle );

    face = size->face;
    اگر ( !face || !face->driver )
      بازگشت FT_THROW( Invalid_Face_Handle );

    /* we don't need anything more complex than that; all size objects */
    /* are already listed by the face                                  */
    face->size = size;

    بازگشت FT_Err_Ok;
  }


  /*************************************************************************/
  /*************************************************************************/
  /*************************************************************************/
  /****                                                                 ****/
  /****                                                                 ****/
  /****                        R E N D E R E R S                        ****/
  /****                                                                 ****/
  /****                                                                 ****/
  /*************************************************************************/
  /*************************************************************************/
  /*************************************************************************/

  /* lookup a renderer by glyph format in the library's list */
  FT_BASE_DEF( FT_Renderer )
  FT_Lookup_Renderer( FT_Library       library,
                      FT_Glyph_Format  format,
                      FT_List_گره*     گره )
  {
    FT_List_گره  cur;
    FT_Renderer  result = پوچ;


    اگر ( !library )
      برو Exit;

    cur = library->renderers.head;

    اگر ( گره )
    {
      اگر ( *گره )
        cur = (*گره)->next;
      *گره = پوچ;
    }

    وقتی ( cur )
    {
      FT_Renderer  renderer = FT_RENDERER( cur->data );


      اگر ( renderer->glyph_format == format )
      {
        اگر ( گره )
          *گره = cur;

        result = renderer;
        بشکن;
      }
      cur = cur->next;
    }

  Exit:
    بازگشت result;
  }


  ایستا FT_Renderer
  ft_lookup_glyph_renderer( FT_GlyphSlot  slot )
  {
    FT_Face      face    = slot->face;
    FT_Library   library = FT_FACE_LIBRARY( face );
    FT_Renderer  result  = library->cur_renderer;


    اگر ( !result || result->glyph_format != slot->format )
      result = FT_Lookup_Renderer( library, slot->format, 0 );

    بازگشت result;
  }


  ایستا تهی
  ft_set_current_renderer( FT_Library  library )
  {
    FT_Renderer  renderer;


    renderer = FT_Lookup_Renderer( library, FT_GLYPH_FORMAT_OUTLINE, 0 );
    library->cur_renderer = renderer;
  }


  ایستا FT_Error
  ft_add_renderer( FT_Module  module )
  {
    FT_Library   library = module->library;
    FT_Memory    memory  = library->memory;
    FT_Error     error;
    FT_List_گره  گره    = پوچ;


    اگر ( FT_QNEW( گره ) )
      برو Exit;

    {
      FT_Renderer         render = FT_RENDERER( module );
      FT_Renderer_Class*  clazz  = (FT_Renderer_Class*)module->clazz;


      render->clazz        = clazz;
      render->glyph_format = clazz->glyph_format;

      /* allocate raster object if needed */
      اگر ( clazz->raster_class && clazz->raster_class->raster_new )
      {
        error = clazz->raster_class->raster_new( memory, &render->raster );
        اگر ( error )
          برو Fail;

        render->raster_render = clazz->raster_class->raster_render;
        render->render        = clazz->render_glyph;
      }

#ifdef FT_CONFIG_OPTION_SVG
      اگر ( clazz->glyph_format == FT_GLYPH_FORMAT_SVG )
        render->render = clazz->render_glyph;
#endif

      /* add to list */
      گره->data = module;
      FT_List_Add( &library->renderers, گره );

      ft_set_current_renderer( library );
    }

  Fail:
    اگر ( error )
      FT_FREE( گره );

  Exit:
    بازگشت error;
  }


  ایستا تهی
  ft_remove_renderer( FT_Module  module )
  {
    FT_Library   library;
    FT_Memory    memory;
    FT_List_گره  گره;


    library = module->library;
    اگر ( !library )
      بازگشت;

    memory = library->memory;

    گره = FT_List_Find( &library->renderers, module );
    اگر ( گره )
    {
      FT_Renderer  render = FT_RENDERER( module );


      /* release raster object, if any */
      اگر ( render->raster )
        render->clazz->raster_class->raster_done( render->raster );

      /* remove from list */
      FT_List_Remove( &library->renderers, گره );
      FT_FREE( گره );

      ft_set_current_renderer( library );
    }
  }


  /* documentation is in ftrender.h */

  FT_EXPORT_DEF( FT_Renderer )
  FT_Get_Renderer( FT_Library       library,
                   FT_Glyph_Format  format )
  {
    /* test for valid `library' delayed to `FT_Lookup_Renderer' */

    بازگشت FT_Lookup_Renderer( library, format, 0 );
  }


  /* documentation is in ftrender.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Set_Renderer( FT_Library     library,
                   FT_Renderer    renderer,
                   FT_UInt        رقم_params,
                   FT_Parameter*  parameters )
  {
    FT_List_گره  گره;
    FT_Error     error = FT_Err_Ok;

    FT_Renderer_SetModeFunc  set_mode;


    اگر ( !library )
    {
      error = FT_THROW( Invalid_Library_Handle );
      برو Exit;
    }

    اگر ( !renderer )
    {
      error = FT_THROW( Invalid_Argument );
      برو Exit;
    }

    اگر ( رقم_params > 0 && !parameters )
    {
      error = FT_THROW( Invalid_Argument );
      برو Exit;
    }

    گره = FT_List_Find( &library->renderers, renderer );
    اگر ( !گره )
    {
      error = FT_THROW( Invalid_Argument );
      برو Exit;
    }

    FT_List_Up( &library->renderers, گره );

    اگر ( renderer->glyph_format == FT_GLYPH_FORMAT_OUTLINE )
      library->cur_renderer = renderer;

    set_mode = renderer->clazz->set_mode;

    برای ( ; رقم_params > 0; رقم_params-- )
    {
      error = set_mode( renderer, parameters->tag, parameters->data );
      اگر ( error )
        بشکن;
      parameters++;
    }

  Exit:
    بازگشت error;
  }


  FT_BASE_DEF( FT_Error )
  FT_Render_Glyph_Internal( FT_Library      library,
                            FT_GlyphSlot    slot,
                            FT_Render_Mode  render_mode )
  {
    FT_Error     error = FT_Err_Ok;
    FT_Face      face  = slot->face;
    FT_Renderer  renderer;
    FT_List_گره  گره  = پوچ;


    /* try to render colored glyph layers as a special case */
    اگر ( slot->internal->load_flags & FT_LOAD_COLOR &&
         slot->format == FT_GLYPH_FORMAT_OUTLINE    )
    {
      FT_LayerIterator  iterator;

      FT_UInt  base_glyph = slot->glyph_index;

      FT_Bool  have_layers;
      FT_UInt  glyph_index;
      FT_UInt  color_index;


      iterator.p  = پوچ;
      have_layers = FT_Get_Color_Glyph_Layer( face,
                                              base_glyph,
                                              &glyph_index,
                                              &color_index,
                                              &iterator );
      اگر ( have_layers )
      {
        error = FT_New_GlyphSlot( face, پوچ );
        اگر ( !error )
        {
          TT_Face       ttface = (TT_Face)face;
          SFNT_Service  sfnt   = (SFNT_Service)ttface->sfnt;


          انجامبده
          {
            FT_Int32  load_flags = slot->internal->load_flags;


            /* disable the `FT_LOAD_COLOR' flag to avoid recursion */
            /* right here in this function                         */
            load_flags &= ~FT_LOAD_COLOR;

            /* render into the new `face->glyph' glyph slot */
            load_flags |= FT_LOAD_RENDER | FT_LOAD_NO_BITMAP;

            error = FT_Load_Glyph( face, glyph_index, load_flags );
            اگر ( error )
              بشکن;

            /* blend new `face->glyph' into old `slot'; */
            /* at the first call, `slot' is still empty */
            error = sfnt->colr_blend( ttface,
                                      color_index,
                                      slot,
                                      face->glyph );
            اگر ( error )
              بشکن;

          } وقتی ( FT_Get_Color_Glyph_Layer( face,
                                              base_glyph,
                                              &glyph_index,
                                              &color_index,
                                              &iterator ) );

          اگر ( !error )
            slot->format = FT_GLYPH_FORMAT_BITMAP;

          /* this call also restores `slot' as the glyph slot */
          FT_Done_GlyphSlot( face->glyph );
        }

        اگر ( !error )
          بازگشت error;

        /* Failed to do the colored layer.  Draw outline instead. */
        slot->format = FT_GLYPH_FORMAT_OUTLINE;
      }
    }

    /* small shortcut for the very common case */
    اگر ( slot->format == FT_GLYPH_FORMAT_OUTLINE )
    {
      renderer = library->cur_renderer;
      گره     = library->renderers.head;
    }
    وگرنه
      renderer = FT_Lookup_Renderer( library, slot->format, &گره );

    error = FT_ERR( Cannot_Render_Glyph );
    وقتی ( renderer )
    {
      error = renderer->render( renderer, slot, render_mode, پوچ );
      اگر ( !error                                   ||
           FT_ERR_NEQ( error, Cannot_Render_Glyph ) )
        بشکن;

      /* `FT_Err_Cannot_Render_Glyph` is returned if the render mode */
      /* is unsupported by the current renderer for this glyph image */
      /* format.                                                     */

      /* now, look for another renderer that supports the same */
      /* format.                                               */
      renderer = FT_Lookup_Renderer( library, slot->format, &گره );
    }

    /* it is not an error if we cannot render a bitmap glyph */
    اگر ( FT_ERR_EQ( error, Cannot_Render_Glyph ) &&
         slot->format == FT_GLYPH_FORMAT_BITMAP  )
      error = FT_Err_Ok;

#ifdef FT_DEBUG_LEVEL_TRACE

#undef  FT_COMPONENT
#define FT_COMPONENT  checksum

    /*
     * Computing the MD5 checksum is expensive, unnecessarily distorting a
     * possible profiling of FreeType if compiled with tracing support.  For
     * this reason, we execute the following code only if explicitly
     * requested.
     */

    /* we use FT_TRACE3 in this block */
    اگر ( !error                               &&
         ft_trace_levels[trace_checksum] >= 3 &&
         slot->bitmap.buffer                  )
    {
      FT_Bitmap  bitmap;
      FT_Error   err;


      FT_Bitmap_Init( &bitmap );

      /* we convert to a single bitmap format for computing the checksum */
      /* this also converts the bitmap flow to `down' (i.e., pitch > 0)  */
      err = FT_Bitmap_Convert( library, &slot->bitmap, &bitmap, 1 );
      اگر ( !err )
      {
        MD5_CTX        ctx;
        unsigned نویسه  md5[16];
        unsigned long  coverage = 0;
        صحیح            i, j;
        صحیح            rows  = (صحیح)bitmap.rows;
        صحیح            pitch = bitmap.pitch;


        FT_TRACE3(( "FT_Render_Glyph: bitmap %dx%d, %s (mode %d)\n",
                    pitch,
                    rows,
                    pixel_modes[slot->bitmap.pixel_mode],
                    slot->bitmap.pixel_mode ));

        برای ( i = 0; i < rows; i++ )
          برای ( j = 0; j < pitch; j++ )
            coverage += bitmap.buffer[i * pitch + j];

        FT_TRACE3(( "  Total coverage: %lu\n", coverage ));

        MD5_Init( &ctx );
        اگر ( bitmap.buffer )
          MD5_Update( &ctx, bitmap.buffer,
                      (unsigned long)rows * (unsigned long)pitch );
        MD5_Final( md5, &ctx );

        FT_TRACE3(( "  MD5 checksum: " ));
        برای ( i = 0; i < 16; i++ )
          FT_TRACE3(( "%02X", md5[i] ));
        FT_TRACE3(( "\n" ));
      }

      FT_Bitmap_Done( library, &bitmap );
    }

    /*
     * Dump bitmap in Netpbm format (PBM or PGM).
     */

    /* we use FT_TRACE7 in this block */
    اگر ( !error                               &&
         ft_trace_levels[trace_checksum] >= 7 &&
         slot->bitmap.buffer                  )
    {
      اگر ( slot->bitmap.rows  < 128U &&
           slot->bitmap.width < 128U )
      {
        صحیح  rows  = (صحیح)slot->bitmap.rows;
        صحیح  width = (صحیح)slot->bitmap.width;
        صحیح  pitch =      slot->bitmap.pitch;
        صحیح  i, j, m;

        unsigned نویسه*  topleft = slot->bitmap.buffer;


        اگر ( pitch < 0 )
          topleft -= pitch * ( rows - 1 );

        FT_TRACE7(( "Netpbm image: start\n" ));
        انتخاب ( slot->bitmap.pixel_mode )
        {
        حالت FT_PIXEL_MODE_MONO:
          FT_TRACE7(( "P1 %d %d\n", width, rows ));
          برای ( i = 0; i < rows; i++ )
          {
            برای ( j = 0; j < width; )
              برای ( m = 128; m > 0 && j < width; m >>= 1, j++ )
                FT_TRACE7(( " %d",
                            ( topleft[i * pitch + j / 8] & m ) != 0 ));
            FT_TRACE7(( "\n" ));
          }
          بشکن;

        پیشفرض:
          FT_TRACE7(( "P2 %d %d 255\n", width, rows ));
          برای ( i = 0; i < rows; i++ )
          {
            برای ( j = 0; j < width; j += 1 )
              FT_TRACE7(( " %3u", topleft[i * pitch + j] ));
            FT_TRACE7(( "\n" ));
          }
        }
        FT_TRACE7(( "Netpbm image: end\n" ));
      }
      وگرنه
        FT_TRACE7(( "Netpbm image: too large, omitted\n" ));
    }

#undef  FT_COMPONENT
#define FT_COMPONENT  objs

#endif /* FT_DEBUG_LEVEL_TRACE */

    بازگشت error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Render_Glyph( FT_GlyphSlot    slot,
                   FT_Render_Mode  render_mode )
  {
    FT_Library  library;


    اگر ( !slot || !slot->face )
      بازگشت FT_THROW( Invalid_Argument );

    library = FT_FACE_LIBRARY( slot->face );

    بازگشت FT_Render_Glyph_Internal( library, slot, render_mode );
  }


  /*************************************************************************/
  /*************************************************************************/
  /*************************************************************************/
  /****                                                                 ****/
  /****                                                                 ****/
  /****                         M O D U L E S                           ****/
  /****                                                                 ****/
  /****                                                                 ****/
  /*************************************************************************/
  /*************************************************************************/
  /*************************************************************************/


  /**************************************************************************
   *
   * @Function:
   *   Destroy_Module
   *
   * @Description:
   *   Destroys a given module object.  For drivers, this also destroys
   *   all child faces.
   *
   * @InOut:
   *   module ::
   *     A handle to the target driver object.
   *
   * @Note:
   *   The driver _must_ be LOCKED!
   */
  ایستا تهی
  Destroy_Module( FT_Module  module )
  {
    ثابت FT_Module_Class*  clazz   = module->clazz;
    FT_Library              library = module->library;
    FT_Memory               memory  = module->memory;


    اگر ( library && library->auto_hinter == module )
      library->auto_hinter = پوچ;

    /* if the module is a renderer */
    اگر ( FT_MODULE_IS_RENDERER( module ) )
      ft_remove_renderer( module );

    /* if the module is a font driver, add some steps */
    اگر ( FT_MODULE_IS_DRIVER( module ) )
      Destroy_Driver( FT_DRIVER( module ) );

    /* finalize the module object */
    اگر ( clazz->module_done )
      clazz->module_done( module );

    /* discard it */
    FT_FREE( module );
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Add_Module( FT_Library              library,
                 ثابت FT_Module_Class*  clazz )
  {
    FT_Error   error;
    FT_Memory  memory;
    FT_Module  module = پوچ;
    FT_UInt    nn;


#define FREETYPE_VER_FIXED  ( ( (FT_Long)FREETYPE_MAJOR << 16 ) | \
                                FREETYPE_MINOR                  )

    اگر ( !library )
      بازگشت FT_THROW( Invalid_Library_Handle );

    اگر ( !clazz )
      بازگشت FT_THROW( Invalid_Argument );

    /* check FreeType version */
    اگر ( clazz->module_requires > FREETYPE_VER_FIXED )
      بازگشت FT_THROW( Invalid_Version );

    /* look for a module with the same name in the library's table */
    برای ( nn = 0; nn < library->رقم_modules; nn++ )
    {
      module = library->modules[nn];
      اگر ( ft_strcmp( module->clazz->module_name, clazz->module_name ) == 0 )
      {
        /* this installed module has the same name, compare their versions */
        اگر ( clazz->module_version <= module->clazz->module_version )
          بازگشت FT_THROW( Lower_Module_Version );

        /* remove the module from our list, then exit the loop to replace */
        /* it by our new version..                                        */
        FT_Remove_Module( library, module );
        بشکن;
      }
    }

    memory = library->memory;
    error  = FT_Err_Ok;

    اگر ( library->رقم_modules >= FT_MAX_MODULES )
    {
      error = FT_THROW( Too_Many_Drivers );
      برو Exit;
    }

    /* allocate module object */
    اگر ( FT_ALLOC( module, clazz->module_size ) )
      برو Exit;

    /* base initialization */
    module->clazz   = clazz;
    module->library = library;
    module->memory  = memory;

    /* check whether the module is a renderer - this must be performed */
    /* before the normal module initialization                         */
    اگر ( FT_MODULE_IS_RENDERER( module ) )
    {
      /* add to the renderers list */
      error = ft_add_renderer( module );
      اگر ( error )
        برو Fail;
    }

    /* is the module a auto-hinter? */
    اگر ( FT_MODULE_IS_HINTER( module ) )
      library->auto_hinter = module;

    /* if the module is a font driver */
    اگر ( FT_MODULE_IS_DRIVER( module ) )
    {
      FT_Driver  driver = FT_DRIVER( module );


      driver->clazz = (FT_Driver_Class)module->clazz;
    }

    اگر ( clazz->module_init )
    {
      error = clazz->module_init( module );
      اگر ( error )
        برو Fail;
    }

    /* add module to the library's table */
    library->modules[library->رقم_modules++] = module;

  Exit:
    بازگشت error;

  Fail:
    اگر ( FT_MODULE_IS_RENDERER( module ) )
    {
      FT_Renderer  renderer = FT_RENDERER( module );


      اگر ( renderer->clazz                                          &&
           renderer->clazz->glyph_format == FT_GLYPH_FORMAT_OUTLINE &&
           renderer->raster                                         )
        renderer->clazz->raster_class->raster_done( renderer->raster );
    }

    FT_FREE( module );
    برو Exit;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Module )
  FT_Get_Module( FT_Library   library,
                 ثابت نویسه*  module_name )
  {
    FT_Module   result = پوچ;
    FT_Module*  cur;
    FT_Module*  limit;


    اگر ( !library || !module_name )
      بازگشت result;

    cur   = library->modules;
    limit = cur + library->رقم_modules;

    برای ( ; cur < limit; cur++ )
      اگر ( ft_strcmp( cur[0]->clazz->module_name, module_name ) == 0 )
      {
        result = cur[0];
        بشکن;
      }

    بازگشت result;
  }


  /* documentation is in ftobjs.h */

  FT_BASE_DEF( ثابت تهی* )
  FT_Get_Module_Interface( FT_Library   library,
                           ثابت نویسه*  mod_name )
  {
    FT_Module  module;


    /* test for valid `library' delayed to FT_Get_Module() */

    module = FT_Get_Module( library, mod_name );

    بازگشت module ? module->clazz->module_interface : 0;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_module_get_service( FT_Module    module,
                         ثابت نویسه*  service_id,
                         FT_Bool      global )
  {
    FT_Pointer  result = پوچ;


    اگر ( module )
    {
      FT_ASSERT( module->clazz && module->clazz->get_interface );

      /* first, look for the service in the module */
      اگر ( module->clazz->get_interface )
        result = module->clazz->get_interface( module, service_id );

      اگر ( global && !result )
      {
        /* we didn't find it, look in all other modules then */
        FT_Library  library = module->library;
        FT_Module*  cur     = library->modules;
        FT_Module*  limit   = cur + library->رقم_modules;


        برای ( ; cur < limit; cur++ )
        {
          اگر ( cur[0] != module )
          {
            FT_ASSERT( cur[0]->clazz );

            اگر ( cur[0]->clazz->get_interface )
            {
              result = cur[0]->clazz->get_interface( cur[0], service_id );
              اگر ( result )
                بشکن;
            }
          }
        }
      }
    }

    بازگشت result;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Remove_Module( FT_Library  library,
                    FT_Module   module )
  {
    /* try to find the module from the table, then remove it from there */

    اگر ( !library )
      بازگشت FT_THROW( Invalid_Library_Handle );

    اگر ( module )
    {
      FT_Module*  cur   = library->modules;
      FT_Module*  limit = cur + library->رقم_modules;


      برای ( ; cur < limit; cur++ )
      {
        اگر ( cur[0] == module )
        {
          /* remove it from the table */
          library->رقم_modules--;
          limit--;
          وقتی ( cur < limit )
          {
            cur[0] = cur[1];
            cur++;
          }
          limit[0] = پوچ;

          /* destroy the module */
          Destroy_Module( module );

          بازگشت FT_Err_Ok;
        }
      }
    }
    بازگشت FT_THROW( Invalid_Driver_Handle );
  }


  ایستا FT_Error
  ft_property_do( FT_Library        library,
                  ثابت FT_String*  module_name,
                  ثابت FT_String*  property_name,
                  تهی*             مقدار,
                  FT_Bool           set,
                  FT_Bool           مقدار_is_string )
  {
    FT_Module*           cur;
    FT_Module*           limit;
    FT_Module_Interface  interface;

    FT_Service_Properties  service;

#ifdef FT_DEBUG_LEVEL_ERROR
    ثابت FT_String*  set_name  = "FT_Property_Set";
    ثابت FT_String*  get_name  = "FT_Property_Get";
    ثابت FT_String*  func_name = set ? set_name : get_name;
#endif

    FT_Bool  missing_func;


    اگر ( !library )
      بازگشت FT_THROW( Invalid_Library_Handle );

    اگر ( !module_name || !property_name || !مقدار )
      بازگشت FT_THROW( Invalid_Argument );

    cur   = library->modules;
    limit = cur + library->رقم_modules;

    /* search module */
    برای ( ; cur < limit; cur++ )
      اگر ( !ft_strcmp( cur[0]->clazz->module_name, module_name ) )
        بشکن;

    اگر ( cur == limit )
    {
      FT_TRACE2(( "%s: can't find module `%s'\n",
                  func_name, module_name ));
      بازگشت FT_THROW( Missing_Module );
    }

    /* check whether we have a service interface */
    اگر ( !cur[0]->clazz->get_interface )
    {
      FT_TRACE2(( "%s: module `%s' doesn't support properties\n",
                  func_name, module_name ));
      بازگشت FT_THROW( Unimplemented_Feature );
    }

    /* search property service */
    interface = cur[0]->clazz->get_interface( cur[0],
                                              FT_SERVICE_ID_PROPERTIES );
    اگر ( !interface )
    {
      FT_TRACE2(( "%s: module `%s' doesn't support properties\n",
                  func_name, module_name ));
      بازگشت FT_THROW( Unimplemented_Feature );
    }

    service = (FT_Service_Properties)interface;

    اگر ( set )
      missing_func = FT_BOOL( !service->set_property );
    وگرنه
      missing_func = FT_BOOL( !service->get_property );

    اگر ( missing_func )
    {
      FT_TRACE2(( "%s: property service of module `%s' is broken\n",
                  func_name, module_name ));
      بازگشت FT_THROW( Unimplemented_Feature );
    }

    بازگشت set ? service->set_property( cur[0],
                                        property_name,
                                        مقدار,
                                        مقدار_is_string )
               : service->get_property( cur[0],
                                        property_name,
                                        مقدار );
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Property_Set( FT_Library        library,
                   ثابت FT_String*  module_name,
                   ثابت FT_String*  property_name,
                   ثابت تهی*       مقدار )
  {
    بازگشت ft_property_do( library,
                           module_name,
                           property_name,
                           (تهی*)مقدار,
                           TRUE,
                           FALSE );
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Property_Get( FT_Library        library,
                   ثابت FT_String*  module_name,
                   ثابت FT_String*  property_name,
                   تهی*             مقدار )
  {
    بازگشت ft_property_do( library,
                           module_name,
                           property_name,
                           مقدار,
                           FALSE,
                           FALSE );
  }


#ifdef FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES

  /* this variant is used for handling the FREETYPE_PROPERTIES */
  /* environment variable                                      */

  FT_BASE_DEF( FT_Error )
  ft_property_string_set( FT_Library        library,
                          ثابت FT_String*  module_name,
                          ثابت FT_String*  property_name,
                          FT_String*        مقدار )
  {
    بازگشت ft_property_do( library,
                           module_name,
                           property_name,
                           (تهی*)مقدار,
                           TRUE,
                           TRUE );
  }

#endif


  /*************************************************************************/
  /*************************************************************************/
  /*************************************************************************/
  /****                                                                 ****/
  /****                                                                 ****/
  /****                         L I B R A R Y                           ****/
  /****                                                                 ****/
  /****                                                                 ****/
  /*************************************************************************/
  /*************************************************************************/
  /*************************************************************************/


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Reference_Library( FT_Library  library )
  {
    اگر ( !library )
      بازگشت FT_THROW( Invalid_Library_Handle );

    library->refcount++;

    بازگشت FT_Err_Ok;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_New_Library( FT_Memory    memory,
                  FT_Library  *alibrary )
  {
    FT_Library  library = پوچ;
    FT_Error    error;


    اگر ( !memory || !alibrary )
      بازگشت FT_THROW( Invalid_Argument );

#ifndef FT_DEBUG_LOGGING
#ifdef FT_DEBUG_LEVEL_ERROR
    /* init debugging support */
    ft_debug_init();
#endif /* FT_DEBUG_LEVEL_ERROR */
#endif /* !FT_DEBUG_LOGGING */

    /* first of all, allocate the library object */
    اگر ( FT_NEW( library ) )
      بازگشت error;

    library->memory = memory;

    library->version_major = FREETYPE_MAJOR;
    library->version_minor = FREETYPE_MINOR;
    library->version_patch = FREETYPE_PATCH;

    library->refcount = 1;

    /* That's ok now */
    *alibrary = library;

    بازگشت FT_Err_Ok;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( تهی )
  FT_Library_Version( FT_Library   library,
                      FT_Int      *amajor,
                      FT_Int      *aminor,
                      FT_Int      *apatch )
  {
    FT_Int  major = 0;
    FT_Int  minor = 0;
    FT_Int  patch = 0;


    اگر ( library )
    {
      major = library->version_major;
      minor = library->version_minor;
      patch = library->version_patch;
    }

    اگر ( amajor )
      *amajor = major;

    اگر ( aminor )
      *aminor = minor;

    اگر ( apatch )
      *apatch = patch;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Done_Library( FT_Library  library )
  {
    FT_Memory  memory;


    اگر ( !library )
      بازگشت FT_THROW( Invalid_Library_Handle );

    library->refcount--;
    اگر ( library->refcount > 0 )
      برو Exit;

    memory = library->memory;

    /*
     * Close all faces in the library.  If we don't do this, we can have
     * some subtle memory leaks.
     *
     * Example:
     *
     * - the cff font driver uses the pshinter module in cff_size_done
     * - if the pshinter module is destroyed before the cff font driver,
     *   opened FT_Face objects managed by the driver are not properly
     *   destroyed, resulting in a memory leak
     *
     * Some faces are dependent on other faces, like Type42 faces that
     * depend on TrueType faces synthesized internally.
     *
     * The order of drivers should be specified in driver_name[].
     */
    {
      FT_UInt      m, n;
      ثابت نویسه*  driver_name[] = { "type42", پوچ };


      برای ( m = 0;
            m < اندازه ( driver_name ) / اندازه ( driver_name[0] );
            m++ )
      {
        برای ( n = 0; n < library->رقم_modules; n++ )
        {
          FT_Module    module      = library->modules[n];
          ثابت نویسه*  module_name = module->clazz->module_name;
          FT_List      faces;


          اگر ( driver_name[m]                                &&
               ft_strcmp( module_name, driver_name[m] ) != 0 )
            ادامه;

          اگر ( ( module->clazz->module_flags & FT_MODULE_FONT_DRIVER ) == 0 )
            ادامه;

          FT_TRACE7(( "FT_Done_Library: close faces for %s\n", module_name ));

          faces = &FT_DRIVER( module )->faces_list;
          وقتی ( faces->head )
          {
            FT_Done_Face( FT_FACE( faces->head->data ) );
            اگر ( faces->head )
              FT_TRACE0(( "FT_Done_Library: failed to free some faces\n" ));
          }
        }
      }
    }

    /* Close all other modules in the library */
#اگر 1
    /* XXX Modules are removed in the reversed order so that  */
    /* type42 module is removed before truetype module.  This */
    /* avoids double free in some occasions.  It is a hack.   */
    وقتی ( library->رقم_modules > 0 )
      FT_Remove_Module( library,
                        library->modules[library->رقم_modules - 1] );
#وگرنه
    {
      FT_UInt  n;


      برای ( n = 0; n < library->رقم_modules; n++ )
      {
        FT_Module  module = library->modules[n];


        اگر ( module )
        {
          Destroy_Module( module );
          library->modules[n] = پوچ;
        }
      }
    }
#endif

    FT_FREE( library );

  Exit:
    بازگشت FT_Err_Ok;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( تهی )
  FT_Set_Debug_Hook( FT_Library         library,
                     FT_UInt            hook_index,
                     FT_DebugHook_Func  debug_hook )
  {
    اگر ( library && debug_hook &&
         hook_index <
           ( اندازه ( library->debug_hooks ) / اندازه ( تهی* ) ) )
      library->debug_hooks[hook_index] = debug_hook;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_TrueTypeEngineType )
  FT_Get_TrueType_Engine_Type( FT_Library  library )
  {
    FT_TrueTypeEngineType  result = FT_TRUETYPE_ENGINE_TYPE_NONE;


    اگر ( library )
    {
      FT_Module  module = FT_Get_Module( library, "truetype" );


      اگر ( module )
      {
        FT_Service_TrueTypeEngine  service;


        service = (FT_Service_TrueTypeEngine)
                    ft_module_get_service( module,
                                           FT_SERVICE_ID_TRUETYPE_ENGINE,
                                           0 );
        اگر ( service )
          result = service->engine_type;
      }
    }

    بازگشت result;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_SubGlyph_Info( FT_GlyphSlot  glyph,
                        FT_UInt       sub_index,
                        FT_Int       *p_index,
                        FT_UInt      *p_flags,
                        FT_Int       *p_arg1,
                        FT_Int       *p_arg2,
                        FT_Matrix    *p_transform )
  {
    FT_Error  error = FT_ERR( Invalid_Argument );


    اگر ( glyph                                      &&
         glyph->subglyphs                           &&
         glyph->format == FT_GLYPH_FORMAT_COMPOSITE &&
         sub_index < glyph->رقم_subglyphs           )
    {
      FT_SubGlyph  subg = glyph->subglyphs + sub_index;


      *p_index     = subg->index;
      *p_flags     = subg->flags;
      *p_arg1      = subg->arg1;
      *p_arg2      = subg->arg2;
      *p_transform = subg->transform;

      error = FT_Err_Ok;
    }

    بازگشت error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Get_Color_Glyph_Layer( FT_Face            face,
                            FT_UInt            base_glyph,
                            FT_UInt           *aglyph_index,
                            FT_UInt           *acolor_index,
                            FT_LayerIterator*  iterator )
  {
    TT_Face       ttface;
    SFNT_Service  sfnt;


    اگر ( !face                                   ||
         !aglyph_index                           ||
         !acolor_index                           ||
         !iterator                               ||
         base_glyph >= (FT_UInt)face->رقم_glyphs )
      بازگشت 0;

    اگر ( !FT_IS_SFNT( face ) )
      بازگشت 0;

    ttface = (TT_Face)face;
    sfnt   = (SFNT_Service)ttface->sfnt;

    اگر ( sfnt->get_colr_layer )
      بازگشت sfnt->get_colr_layer( ttface,
                                   base_glyph,
                                   aglyph_index,
                                   acolor_index,
                                   iterator );
    وگرنه
      بازگشت 0;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Get_Color_Glyph_Paint( FT_Face                  face,
                            FT_UInt                  base_glyph,
                            FT_Color_ریشه_Transform  ریشه_transform,
                            FT_OpaquePaint*          paint )
  {
    TT_Face       ttface;
    SFNT_Service  sfnt;


    اگر ( !face || !paint )
      بازگشت 0;

    اگر ( !FT_IS_SFNT( face ) )
      بازگشت 0;

    ttface = (TT_Face)face;
    sfnt   = (SFNT_Service)ttface->sfnt;

    اگر ( sfnt->get_colr_glyph_paint )
      بازگشت sfnt->get_colr_glyph_paint( ttface,
                                         base_glyph,
                                         ریشه_transform,
                                         paint );
    وگرنه
      بازگشت 0;
  }


  /* documentation is in ftcolor.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Get_Color_Glyph_ClipBox( FT_Face      face,
                              FT_UInt      base_glyph,
                              FT_ClipBox*  clip_box )
  {
    TT_Face       ttface;
    SFNT_Service  sfnt;


    اگر ( !face || !clip_box )
      بازگشت 0;

    اگر ( !FT_IS_SFNT( face ) )
      بازگشت 0;

    ttface = (TT_Face)face;
    sfnt   = (SFNT_Service)ttface->sfnt;

    اگر ( sfnt->get_color_glyph_clipbox )
      بازگشت sfnt->get_color_glyph_clipbox( ttface,
                                            base_glyph,
                                            clip_box );
    وگرنه
      بازگشت 0;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Get_Paint_Layers( FT_Face            face,
                       FT_LayerIterator*  layer_iterator,
                       FT_OpaquePaint*    paint )
  {
    TT_Face       ttface;
    SFNT_Service  sfnt;


    اگر ( !face || !paint || !layer_iterator )
      بازگشت 0;

    اگر ( !FT_IS_SFNT( face ) )
      بازگشت 0;

    ttface = (TT_Face)face;
    sfnt   = (SFNT_Service)ttface->sfnt;

    اگر ( sfnt->get_paint_layers )
      بازگشت sfnt->get_paint_layers( ttface, layer_iterator, paint );
    وگرنه
      بازگشت 0;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Get_Paint( FT_Face face,
                FT_OpaquePaint  opaque_paint,
                FT_COLR_Paint*  paint )
  {
    TT_Face       ttface;
    SFNT_Service  sfnt;


    اگر ( !face || !paint )
      بازگشت 0;

    اگر ( !FT_IS_SFNT( face ) )
      بازگشت 0;

    ttface = (TT_Face)face;
    sfnt   = (SFNT_Service)ttface->sfnt;

    اگر ( sfnt->get_paint )
      بازگشت sfnt->get_paint( ttface, opaque_paint, paint );
    وگرنه
      بازگشت 0;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Get_Colorline_Stops ( FT_Face                face,
                           FT_ColorStop *         color_stop,
                           FT_ColorStopIterator  *iterator )
  {
    TT_Face       ttface;
    SFNT_Service  sfnt;


    اگر ( !face || !color_stop || !iterator )
      بازگشت 0;

    اگر ( !FT_IS_SFNT( face ) )
      بازگشت 0;

    ttface = (TT_Face)face;
    sfnt   = (SFNT_Service)ttface->sfnt;

    اگر ( sfnt->get_colorline_stops )
      بازگشت sfnt->get_colorline_stops ( ttface, color_stop, iterator );
    وگرنه
      بازگشت 0;
  }


/* END */
