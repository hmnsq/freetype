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


#ضمن <freetype/ftlist.h>
#ضمن <freetype/ftoutln.h>
#ضمن <freetype/ftfntfmt.h>
#ضمن <freetype/otsvg.h>

#ضمن <freetype/internal/ftvalid.h>
#ضمن <freetype/internal/ftobjs.h>
#ضمن <freetype/internal/ftdebug.h>
#ضمن <freetype/internal/ftrfork.h>
#ضمن <freetype/internal/ftstream.h>
#ضمن <freetype/internal/sfnt.h>          /* for SFNT_Load_Table_Func */
#ضمن <freetype/internal/psaux.h>         /* for PS_Driver            */
#ضمن <freetype/internal/svginterface.h>

#ضمن <freetype/tttables.h>
#ضمن <freetype/tttags.h>
#ضمن <freetype/ttnameid.h>

#ضمن <freetype/internal/services/svprop.h>
#ضمن <freetype/internal/services/svsfnt.h>
#ضمن <freetype/internal/services/svpostnm.h>
#ضمن <freetype/internal/services/svgldict.h>
#ضمن <freetype/internal/services/svttcmap.h>
#ضمن <freetype/internal/services/svkern.h>
#ضمن <freetype/internal/services/svtteng.h>

#ifdef TT_CONFIG_OPTION_VARC
#ضمن <freetype/internal/services/svvarc.h>
#endif

#ضمن <freetype/ftdriver.h>

#ifdef FT_CONFIG_OPTION_MAC_FONTS
#ضمن "ftbase.h"
#endif


#ifdef FT_DEBUG_LEVEL_TRACE

#ضمن <freetype/ftbitmap.h>

#إذا defined( _MSC_VER )      /* Visual C++ (and Intel C++)   */
  /* We disable the warning `conversion from XXX to YYY,     */
  /* possible loss of data' in order to compile cleanly with */
  /* the maximum level of warnings: `md5.c' is non-FreeType  */
  /* code, and it gets used during development builds only.  */
#pragma warning( ڈالو )
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

#ضمن "md5.c"

#إذا defined( _MSC_VER )
#pragma warning( نکالو )
#endif

  /* This array must stay in sync with the @FT_Pixel_Mode enumeration */
  /* (in file `ftimage.h`).                                           */

  ساكن ثابت حرف* ثابت  pixel_modes[] =
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
  ساكن FT_Error
  ft_open_face_internal( FT_Library           library,
                         ثابت FT_Open_Args*  args,
                         FT_Long              face_index,
                         FT_Face             *aface,
                         FT_Bool              test_mac_fonts );


  FT_BASE_DEF( FT_Pointer )
  ft_service_list_lookup( FT_ServiceDesc  service_descriptors,
                          ثابت حرف*     service_id )
  {
    FT_Pointer      result = عدم;
    FT_ServiceDesc  desc   = service_descriptors;


    إذا ( desc && service_id )
    {
      لكل ( ; desc->serv_id != عدم; desc++ )
      {
        إذا ( ft_strcmp( desc->serv_id, service_id ) == 0 )
        {
          result = (FT_Pointer)desc->serv_data;
          اكسر;
        }
      }
    }

    أرجع result;
  }


  FT_BASE_DEF( فراغ )
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

    أرجع -1;
  }


  FT_BASE_DEF( فراغ )
  ft_validator_error( FT_Validator  valid,
                      FT_Error      error )
  {
    /* since the cast below also disables the compiler's */
    /* type check, we introduce a dummy variable, which  */
    /* will be optimized away                            */
    متطاير ft_jmp_buf* jump_buffer = &valid->jump_buffer;


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
    FT_Stream  stream = عدم;
    FT_UInt    mode;


    *astream = عدم;

    إذا ( !library )
      أرجع FT_THROW( Invalid_Library_Handle );

    إذا ( !args )
      أرجع FT_THROW( Invalid_Argument );

    memory = library->memory;
    mode   = args->flags &
               ( FT_OPEN_MEMORY | FT_OPEN_STREAM | FT_OPEN_PATHNAME );

    إذا ( mode == FT_OPEN_MEMORY )
    {
      /* create a memory-based stream */
      إذا ( FT_NEW( stream ) )
        اذهب Exit;

      FT_Stream_OpenMemory( stream,
                            (ثابت FT_Byte*)args->memory_base,
                            (FT_ULong)args->memory_size );
      stream->memory = memory;
    }

#ifndef FT_CONFIG_OPTION_DISABLE_STREAM_SUPPORT

    وإلا إذا ( mode == FT_OPEN_PATHNAME )
    {
      /* create a normal system stream */
      إذا ( FT_NEW( stream ) )
        اذهب Exit;

      stream->memory = memory;
      error = FT_Stream_Open( stream, args->pathname );
      إذا ( error )
        FT_FREE( stream );
    }
    وإلا إذا ( ( mode == FT_OPEN_STREAM ) && args->stream )
    {
      /* use an existing, user-provided stream */

      /* in this case, we do not need to allocate a new stream object */
      /* since the caller is responsible for closing it himself       */
      stream         = args->stream;
      stream->memory = memory;
      error          = FT_Err_Ok;
    }

#endif

    وإلا
    {
      error = FT_THROW( Invalid_Argument );
      إذا ( ( args->flags & FT_OPEN_STREAM ) && args->stream )
        FT_Stream_Close( args->stream );
    }

    إذا ( !error )
      *astream       = stream;

  Exit:
    أرجع error;
  }


  FT_BASE_DEF( فراغ )
  FT_Stream_Free( FT_Stream  stream,
                  FT_Int     external )
  {
    إذا ( stream )
    {
      FT_Memory  memory = stream->memory;


      FT_Stream_Close( stream );

      إذا ( !external )
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


  ساكن FT_Error
  ft_glyphslot_init( FT_GlyphSlot  slot )
  {
    FT_Driver         driver   = slot->face->driver;
    FT_Driver_Class   clazz    = driver->clazz;
    FT_Memory         memory   = driver->جڑ.memory;
    FT_Error          error    = FT_Err_Ok;
    FT_Slot_Internal  internal = عدم;


    slot->library = driver->جڑ.library;

    إذا ( FT_NEW( internal ) )
      اذهب Exit;

    slot->internal = internal;

    إذا ( FT_DRIVER_USES_OUTLINES( driver ) )
      error = FT_GlyphLoader_New( memory, &internal->loader );

    إذا ( !error && clazz->init_slot )
      error = clazz->init_slot( slot );

#ifdef FT_CONFIG_OPTION_SVG
    /* if SVG table exists, allocate the space in `slot->other` */
    إذا ( slot->face->face_flags & FT_FACE_FLAG_SVG )
    {
      FT_SVG_Document  document = عدم;


      إذا ( FT_NEW( document ) )
        اذهب Exit;
      slot->other = document;
    }
#endif

  Exit:
    أرجع error;
  }


  FT_BASE_DEF( فراغ )
  ft_glyphslot_free_bitmap( FT_GlyphSlot  slot )
  {
    إذا ( slot->internal && ( slot->internal->flags & FT_GLYPH_OWN_BITMAP ) )
    {
      FT_Memory  memory = FT_FACE_MEMORY( slot->face );


      FT_FREE( slot->bitmap.buffer );
      slot->internal->flags &= ~FT_GLYPH_OWN_BITMAP;
    }
    وإلا
    {
      /* assume that the bitmap buffer was stolen or not */
      /* allocated from the heap                         */
      slot->bitmap.buffer = عدم;
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
    FT_Pos   x_بایاں, y_چوٹی;
    FT_Pos   width, height, pitch;
    FT_Bool  ret;


    إذا ( slot->format == FT_GLYPH_FORMAT_SVG )
    {
      FT_Module    module;
      SVG_Service  svg_service;


      module      = FT_Get_Module( slot->library, "ot-svg" );
      svg_service = (SVG_Service)module->clazz->module_interface;

      أرجع (FT_Bool)svg_service->preset_slot( module, slot, FALSE );
    }
    وإلا إذا ( slot->format != FT_GLYPH_FORMAT_OUTLINE )
      أرجع 1;

    إذا ( origin )
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

    بدل ( mode )
    {
    حالة FT_RENDER_MODE_MONO:
      pixel_mode = FT_PIXEL_MODE_MONO;
#إذا 1
      /* x */

      /* undocumented but confirmed: bbox values get rounded;    */
      /* we do asymmetric rounding so that the center of a pixel */
      /* gets always included                                    */

      pbox.xMin += ( cbox.xMin + 31 ) >> 6;
      pbox.xMax += ( cbox.xMax + 32 ) >> 6;

      /* if the bbox collapsed, we add a pixel based on the total */
      /* rounding remainder to cover most of the original cbox    */

      إذا ( pbox.xMin == pbox.xMax )
      {
        إذا ( ( ( cbox.xMin + 31 ) & 63 ) - 31 +
             ( ( cbox.xMax + 32 ) & 63 ) - 32 < 0 )
          pbox.xMin -= 1;
        وإلا
          pbox.xMax += 1;
      }

      /* y */

      pbox.yMin += ( cbox.yMin + 31 ) >> 6;
      pbox.yMax += ( cbox.yMax + 32 ) >> 6;

      إذا ( pbox.yMin == pbox.yMax )
      {
        إذا ( ( ( cbox.yMin + 31 ) & 63 ) - 31 +
             ( ( cbox.yMax + 32 ) & 63 ) - 32 < 0 )
          pbox.yMin -= 1;
        وإلا
          pbox.yMax += 1;
      }

      اكسر;
#وإلا
      اذهب Adjust;
#endif

    حالة FT_RENDER_MODE_LCD:
      pixel_mode = FT_PIXEL_MODE_LCD;
      ft_lcd_padding( &cbox, slot, mode );
      اذهب Adjust;

    حالة FT_RENDER_MODE_LCD_V:
      pixel_mode = FT_PIXEL_MODE_LCD_V;
      ft_lcd_padding( &cbox, slot, mode );
      اذهب Adjust;

    حالة FT_RENDER_MODE_NORMAL:
    حالة FT_RENDER_MODE_LIGHT:
    افتراضي:
      pixel_mode = FT_PIXEL_MODE_GRAY;
    Adjust:
      pbox.xMin += cbox.xMin >> 6;
      pbox.yMin += cbox.yMin >> 6;
      pbox.xMax += ( cbox.xMax + 63 ) >> 6;
      pbox.yMax += ( cbox.yMax + 63 ) >> 6;
    }

    x_بایاں = pbox.xMin;
    y_چوٹی  = pbox.yMax;

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

    إذا ( ret )
      FT_TRACE3(( "ft_glyphslot_preset_bitmap: [%ld %ld %ld %ld]\n",
                  pbox.xMin, pbox.yMin, pbox.xMax, pbox.yMax ));

    بدل ( pixel_mode )
    {
    حالة FT_PIXEL_MODE_MONO:
      pitch = ( ( width + 15 ) >> 4 ) << 1;
      اكسر;

    حالة FT_PIXEL_MODE_LCD:
      width *= 3;
      pitch  = FT_PAD_CEIL( width, 4 );
      اكسر;

    حالة FT_PIXEL_MODE_LCD_V:
      height *= 3;
      FALL_THROUGH;

    حالة FT_PIXEL_MODE_GRAY:
    افتراضي:
      pitch = width;
    }

    slot->bitmap_بایاں = (FT_Int)x_بایاں;
    slot->bitmap_چوٹی  = (FT_Int)y_چوٹی;

    bitmap->pixel_mode = (unsigned حرف)pixel_mode;
    bitmap->عدد_grays  = 256;
    bitmap->width      = (unsigned صحيح)width;
    bitmap->rows       = (unsigned صحيح)height;
    bitmap->pitch      = pitch;

    أرجع ret;
  }


  FT_BASE_DEF( فراغ )
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


    إذا ( slot->internal->flags & FT_GLYPH_OWN_BITMAP )
      FT_FREE( slot->bitmap.buffer );
    وإلا
      slot->internal->flags |= FT_GLYPH_OWN_BITMAP;

    /* dimensions must be preset */
    FT_MEM_ALLOC_MULT( slot->bitmap.buffer,
                       slot->bitmap.rows,
                       slot->bitmap.pitch );
    أرجع error;
  }


  ساكن فراغ
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

    slot->bitmap_بایاں   = 0;
    slot->bitmap_چوٹی    = 0;
    slot->عدد_subglyphs = 0;
    slot->subglyphs     = عدم;
    slot->control_data  = عدم;
    slot->control_len   = 0;

#ifndef FT_CONFIG_OPTION_SVG
    slot->other = عدم;
#وإلا
    إذا ( !( slot->face->face_flags & FT_FACE_FLAG_SVG ) )
      slot->other = عدم;
    وإلا
    {
      إذا ( slot->internal->flags & FT_GLYPH_OWN_GZIP_SVG )
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


  ساكن فراغ
  ft_glyphslot_done( FT_GlyphSlot  slot )
  {
    FT_Driver        driver = slot->face->driver;
    FT_Driver_Class  clazz  = driver->clazz;
    FT_Memory        memory = driver->جڑ.memory;

#ifdef FT_CONFIG_OPTION_SVG
    إذا ( slot->face->face_flags & FT_FACE_FLAG_SVG )
    {
      /* Free memory in case SVG was there.                          */
      /* `slot->internal` might be NULL in out-of-memory situations. */
      إذا ( slot->internal && slot->internal->flags & FT_GLYPH_OWN_GZIP_SVG )
      {
        FT_SVG_Document  doc = (FT_SVG_Document)slot->other;


        FT_FREE( doc->svg_document );

        slot->internal->flags &= ~FT_GLYPH_OWN_GZIP_SVG;
      }

      FT_FREE( slot->other );
    }
#endif

    إذا ( clazz->done_slot )
      clazz->done_slot( slot );

    /* free bitmap buffer if needed */
    ft_glyphslot_free_bitmap( slot );

    /* slot->internal might be NULL in out-of-memory situations */
    إذا ( slot->internal )
    {
      /* free glyph loader */
      إذا ( FT_DRIVER_USES_OUTLINES( driver ) )
      {
        FT_GlyphLoader_Done( slot->internal->loader );
        slot->internal->loader = عدم;
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
    FT_GlyphSlot     slot = عدم;


    إذا ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );

    إذا ( !face->driver )
      أرجع FT_THROW( Invalid_Argument );

    driver = face->driver;
    clazz  = driver->clazz;
    memory = driver->جڑ.memory;

    FT_TRACE4(( "FT_New_GlyphSlot: Creating new slot object\n" ));
    إذا ( !FT_ALLOC( slot, clazz->slot_object_size ) )
    {
      slot->face = face;

      error = ft_glyphslot_init( slot );
      إذا ( error )
      {
        ft_glyphslot_done( slot );
        FT_FREE( slot );
        اذهب Exit;
      }

      slot->next  = face->glyph;
      face->glyph = slot;

      إذا ( aslot )
        *aslot = slot;
    }
    وإلا إذا ( aslot )
      *aslot = عدم;


  Exit:
    FT_TRACE4(( "FT_New_GlyphSlot: Return 0x%x\n", error ));

    أرجع error;
  }


  /* documentation is in ftobjs.h */

  FT_BASE_DEF( فراغ )
  FT_Done_GlyphSlot( FT_GlyphSlot  slot )
  {
    إذا ( slot )
    {
      FT_Driver     driver = slot->face->driver;
      FT_Memory     memory = driver->جڑ.memory;
      FT_GlyphSlot  prev;
      FT_GlyphSlot  cur;


      /* Remove slot from its parent face's list */
      prev = عدم;
      cur  = slot->face->glyph;

      طالما ( cur )
      {
        إذا ( cur == slot )
        {
          إذا ( !prev )
            slot->face->glyph = cur->next;
          وإلا
            prev->next = cur->next;

          /* finalize client-specific data */
          إذا ( slot->generic.finalizer )
            slot->generic.finalizer( slot );

          ft_glyphslot_done( slot );
          FT_FREE( slot );
          اكسر;
        }
        prev = cur;
        cur  = cur->next;
      }
    }
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( فراغ )
  FT_Set_Transform( FT_Face     face,
                    FT_Matrix*  matrix,
                    FT_Vector*  delta )
  {
    FT_Face_Internal  internal;


    إذا ( !face )
      أرجع;

    internal = face->internal;

    internal->transform_flags = 0;

    إذا ( !matrix || !FT_Matrix_Check( matrix ) )
    {
      internal->transform_matrix.xx = 0x10000L;
      internal->transform_matrix.xy = 0;
      internal->transform_matrix.yx = 0;
      internal->transform_matrix.yy = 0x10000L;

      matrix = &internal->transform_matrix;
    }
    وإلا
      internal->transform_matrix = *matrix;

    /* set transform_flags bit flag 0 if `matrix' isn't the identity */
    إذا ( ( matrix->xy | matrix->yx ) ||
         matrix->xx != 0x10000L      ||
         matrix->yy != 0x10000L      )
      internal->transform_flags |= 1;

    إذا ( !delta )
    {
      internal->transform_delta.x = 0;
      internal->transform_delta.y = 0;

      delta = &internal->transform_delta;
    }
    وإلا
      internal->transform_delta = *delta;

    /* set transform_flags bit flag 1 if `delta' isn't the null vector */
    إذا ( delta->x | delta->y )
      internal->transform_flags |= 2;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( فراغ )
  FT_Get_Transform( FT_Face     face,
                    FT_Matrix*  matrix,
                    FT_Vector*  delta )
  {
    FT_Face_Internal  internal;


    إذا ( !face )
      أرجع;

    internal = face->internal;

    إذا ( matrix )
      *matrix = internal->transform_matrix;

    إذا ( delta )
      *delta = internal->transform_delta;
  }


  ساكن FT_Renderer
  ft_lookup_glyph_renderer( FT_GlyphSlot  slot );


#ifdef GRID_FIT_METRICS
  ساكن فراغ
  ft_glyphslot_grid_fit_metrics( FT_GlyphSlot  slot,
                                 FT_Bool       vertical )
  {
    FT_Glyph_Metrics*  metrics = &slot->metrics;
    FT_Pos             دایاں, bottom;


    إذا ( vertical )
    {
      metrics->horiBearingX = FT_PIX_FLOOR( metrics->horiBearingX );
      metrics->horiBearingY = FT_PIX_CEIL_LONG( metrics->horiBearingY );

      دایاں  = FT_PIX_CEIL_LONG( ADD_LONG( metrics->vertBearingX,
                                           metrics->width ) );
      bottom = FT_PIX_CEIL_LONG( ADD_LONG( metrics->vertBearingY,
                                           metrics->height ) );

      metrics->vertBearingX = FT_PIX_FLOOR( metrics->vertBearingX );
      metrics->vertBearingY = FT_PIX_FLOOR( metrics->vertBearingY );

      metrics->width  = SUB_LONG( دایاں,
                                  metrics->vertBearingX );
      metrics->height = SUB_LONG( bottom,
                                  metrics->vertBearingY );
    }
    وإلا
    {
      metrics->vertBearingX = FT_PIX_FLOOR( metrics->vertBearingX );
      metrics->vertBearingY = FT_PIX_FLOOR( metrics->vertBearingY );

      دایاں  = FT_PIX_CEIL_LONG( ADD_LONG( metrics->horiBearingX,
                                           metrics->width ) );
      bottom = FT_PIX_FLOOR( SUB_LONG( metrics->horiBearingY,
                                       metrics->height ) );

      metrics->horiBearingX = FT_PIX_FLOOR( metrics->horiBearingX );
      metrics->horiBearingY = FT_PIX_CEIL_LONG( metrics->horiBearingY );

      metrics->width  = SUB_LONG( دایاں,
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


    إذا ( !face || !face->size || !face->glyph )
      أرجع FT_THROW( Invalid_Face_Handle );

    /* The validity test for `glyph_index' is performed by the */
    /* font drivers.                                           */

    slot = face->glyph;
    ft_glyphslot_clear( slot );

    driver  = face->driver;
    library = driver->جڑ.library;
    hinter  = library->auto_hinter;

    /* undefined scale means no scale */
    إذا ( face->size->metrics.x_ppem == 0 ||
         face->size->metrics.y_ppem == 0 )
      load_flags |= FT_LOAD_NO_SCALE;

    /* resolve load flags dependencies */

    إذا ( load_flags & FT_LOAD_NO_RECURSE )
      load_flags |= FT_LOAD_NO_SCALE         |
                    FT_LOAD_IGNORE_TRANSFORM;

    إذا ( load_flags & FT_LOAD_NO_SCALE )
    {
      load_flags |= FT_LOAD_NO_HINTING |
                    FT_LOAD_NO_BITMAP;

      load_flags &= ~FT_LOAD_RENDER;
    }

    إذا ( load_flags & FT_LOAD_BITMAP_METRICS_ONLY )
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

    إذا ( hinter                                           &&
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
      إذا ( ( load_flags & FT_LOAD_FORCE_AUTOHINT ) ||
           !FT_DRIVER_HAS_HINTER( driver )         )
        autohint = TRUE;
      وإلا
      {
        FT_Render_Mode  mode = FT_LOAD_TARGET_MODE( load_flags );
        FT_Bool         is_light_type1;
        TT_Face         ttface = (TT_Face)face;


        /* only the new Adobe engine (for both CFF and Type 1) is `light'; */
        /* we use `strstr' to catch both `Type 1' and `CID Type 1'         */
        is_light_type1 =
          ft_strstr( FT_Get_Font_Format( face ), "Type 1" ) != عدم &&
          ((PS_Driver)driver)->hinting_engine == FT_HINTING_ADOBE;

        /* the check for `num_locations' assures that we actually    */
        /* test for instructions in a TTF and not in a CFF-based OTF */
        /*                                                           */
        /* we check the size of the `fpgm' and `prep' tables, too -- */
        /* the assumption is that there don't exist real TTFs where  */
        /* both `fpgm' and `prep' tables are missing                 */
        إذا ( ( mode == FT_RENDER_MODE_LIGHT           &&
               ( !FT_DRIVER_HINTS_LIGHTLY( driver ) &&
                 !is_light_type1                    ) )         ||
             ( FT_IS_SFNT( face )                             &&
               ttface->عدد_locations                          &&
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
    إذا ( autohint && FT_IS_SFNT( face ) )
    {
      FT_Service_VARC  varc;


      FT_FACE_FIND_SERVICE( face, varc, VARC );
      إذا ( varc && varc->has_glyph( face, glyph_index ) )
        autohint = FALSE;
    }
#endif

    إذا ( autohint )
    {
      FT_AutoHinter_Interface  hinting;


      /* XXX: The use of the `FT_LOAD_XXX_ONLY` flags is not very */
      /*      elegant.                                            */

      /* try to load SVG documents if available */
      إذا ( ( load_flags & FT_LOAD_NO_SVG ) == 0 &&
           FT_HAS_SVG( face )                   )
      {
        error = driver->clazz->load_glyph( slot, face->size,
                                           glyph_index,
                                           load_flags | FT_LOAD_SVG_ONLY );

        إذا ( !error && slot->format == FT_GLYPH_FORMAT_SVG )
          اذهب Load_Ok;
      }

      /* try to load embedded bitmaps if available */
      إذا ( FT_HAS_FIXED_SIZES( face )              &&
           ( load_flags & FT_LOAD_NO_BITMAP ) == 0 )
      {
        error = driver->clazz->load_glyph( slot, face->size,
                                           glyph_index,
                                           load_flags | FT_LOAD_SBITS_ONLY );

        إذا ( !error && slot->format == FT_GLYPH_FORMAT_BITMAP )
          اذهب Load_Ok;
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
    وإلا
    {
      error = driver->clazz->load_glyph( slot,
                                         face->size,
                                         glyph_index,
                                         load_flags );
      إذا ( error )
        اذهب Exit;

      إذا ( slot->format == FT_GLYPH_FORMAT_OUTLINE )
      {
        /* check that the loaded outline is correct */
        error = FT_Outline_Check( &slot->outline );
        إذا ( error )
          اذهب Exit;

#ifdef GRID_FIT_METRICS
        إذا ( !( load_flags & FT_LOAD_NO_HINTING ) )
          ft_glyphslot_grid_fit_metrics(
            slot,
            FT_BOOL( load_flags & FT_LOAD_VERTICAL_LAYOUT ) );
#endif
      }
    }

  Load_Ok:
    /* compute the advance */
    إذا ( load_flags & FT_LOAD_VERTICAL_LAYOUT )
    {
      slot->advance.x = 0;
      slot->advance.y = slot->metrics.vertAdvance;
    }
    وإلا
    {
      slot->advance.x = slot->metrics.horiAdvance;
      slot->advance.y = 0;
    }

    /* compute the linear advance in 16.16 pixels */
    إذا ( ( load_flags & FT_LOAD_LINEAR_DESIGN ) == 0 &&
         FT_IS_SCALABLE( face )                      )
    {
      FT_Size_Metrics*  metrics = &face->size->metrics;


      /* it's tricky! */
      slot->linearHoriAdvance = FT_MulDiv( slot->linearHoriAdvance,
                                           metrics->x_scale, 64 );

      slot->linearVertAdvance = FT_MulDiv( slot->linearVertAdvance,
                                           metrics->y_scale, 64 );
    }

    إذا ( ( load_flags & FT_LOAD_IGNORE_TRANSFORM ) == 0 )
    {
      FT_Face_Internal  internal = face->internal;


      /* now, transform the glyph image if needed */
      إذا ( internal->transform_flags )
      {
        /* get renderer */
        FT_Renderer  renderer = ft_lookup_glyph_renderer( slot );


        إذا ( renderer )
          error = renderer->clazz->transform_glyph(
                                     renderer, slot,
                                     &internal->transform_matrix,
                                     &internal->transform_delta );
        وإلا إذا ( slot->format == FT_GLYPH_FORMAT_OUTLINE )
        {
          /* apply `standard' transformation if no renderer is available */
          إذا ( internal->transform_flags & 1 )
            FT_Outline_Transform( &slot->outline,
                                  &internal->transform_matrix );

          إذا ( internal->transform_flags & 2 )
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
    إذا ( !error                                    &&
         ( load_flags & FT_LOAD_NO_SCALE ) == 0    &&
         slot->format != FT_GLYPH_FORMAT_BITMAP    &&
         slot->format != FT_GLYPH_FORMAT_COMPOSITE )
    {
      FT_Render_Mode  mode = FT_LOAD_TARGET_MODE( load_flags );


      إذا ( mode == FT_RENDER_MODE_NORMAL   &&
           load_flags & FT_LOAD_MONOCHROME )
        mode = FT_RENDER_MODE_MONO;

      إذا ( load_flags & FT_LOAD_RENDER )
        error = FT_Render_Glyph( slot, mode );
      وإلا
        ft_glyphslot_preset_bitmap( slot, mode, عدم );
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
    FT_TRACE5(( "  x advance: %f\n", (مزدوج)slot->advance.x / 64 ));
    FT_TRACE5(( "  y advance: %f\n", (مزدوج)slot->advance.y / 64 ));
    FT_TRACE5(( "  linear x advance: %f\n",
                (مزدوج)slot->linearHoriAdvance / 65536 ));
    FT_TRACE5(( "  linear y advance: %f\n",
                (مزدوج)slot->linearVertAdvance / 65536 ));

    {
      FT_Glyph_Metrics*  metrics = &slot->metrics;


      FT_TRACE5(( "  metrics:\n" ));
      FT_TRACE5(( "    width:  %f\n", (مزدوج)metrics->width / 64 ));
      FT_TRACE5(( "    height: %f\n", (مزدوج)metrics->height / 64 ));
      FT_TRACE5(( "\n" ));
      FT_TRACE5(( "    horiBearingX: %f\n",
                  (مزدوج)metrics->horiBearingX / 64 ));
      FT_TRACE5(( "    horiBearingY: %f\n",
                  (مزدوج)metrics->horiBearingY / 64 ));
      FT_TRACE5(( "    horiAdvance:  %f\n",
                  (مزدوج)metrics->horiAdvance / 64 ));
      FT_TRACE5(( "\n" ));
      FT_TRACE5(( "    vertBearingX: %f\n",
                  (مزدوج)metrics->vertBearingX / 64 ));
      FT_TRACE5(( "    vertBearingY: %f\n",
                  (مزدوج)metrics->vertBearingY / 64 ));
      FT_TRACE5(( "    vertAdvance:  %f\n",
                  (مزدوج)metrics->vertAdvance / 64 ));
    }
#endif

  Exit:
    أرجع error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Load_Char( FT_Face   face,
                FT_ULong  char_code,
                FT_Int32  load_flags )
  {
    FT_UInt  glyph_index;


    إذا ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );

    glyph_index = (FT_UInt)char_code;
    إذا ( face->charmap )
      glyph_index = FT_Get_Char_Index( face, char_code );

    أرجع FT_Load_Glyph( face, glyph_index, load_flags );
  }


  /* destructor for sizes list */
  ساكن فراغ
  destroy_size( FT_Memory  memory,
                فراغ*      size_,
                فراغ*      driver_ )
  {
    FT_Size    size   = (FT_Size)size_;
    FT_Driver  driver = (FT_Driver)driver_;


    /* finalize format-specific stuff */
    إذا ( driver->clazz->done_size )
      driver->clazz->done_size( size );

    /* finalize client-specific data */
    إذا ( size->generic.finalizer )
      size->generic.finalizer( size );

    FT_FREE( size->internal );
    FT_FREE( size );
  }


  ساكن فراغ
  ft_cmap_done_internal( FT_CMap  cmap );


  ساكن فراغ
  destroy_charmaps( FT_Face    face,
                    FT_Memory  memory )
  {
    FT_Int  n;


    إذا ( !face )
      أرجع;

    لكل ( n = 0; n < face->عدد_charmaps; n++ )
    {
      FT_CMap  cmap = FT_CMAP( face->charmaps[n] );


      ft_cmap_done_internal( cmap );

      face->charmaps[n] = عدم;
    }

    FT_FREE( face->charmaps );
    face->عدد_charmaps = 0;
  }


  /* destructor for faces list */
  ساكن فراغ
  destroy_face( FT_Memory  memory,
                فراغ*      face_,
                فراغ*      driver_ )
  {
    FT_Face          face   = (FT_Face)face_;
    FT_Driver        driver = (FT_Driver)driver_;
    FT_Driver_Class  clazz  = driver->clazz;


    /* discard auto-hinting data */
    إذا ( face->autohint.finalizer )
      face->autohint.finalizer( face->autohint.data );

    /* Discard glyph slots for this face.                           */
    /* Beware!  FT_Done_GlyphSlot() changes the field `face->glyph' */
    طالما ( face->glyph )
      FT_Done_GlyphSlot( face->glyph );

    /* discard all sizes for this face */
    FT_List_Finalize( &face->sizes_list,
                      destroy_size,
                      memory,
                      driver );
    face->size = عدم;

    /* discard charmaps */
    destroy_charmaps( face, memory );

    /* finalize format-specific stuff */
    إذا ( clazz->done_face )
      clazz->done_face( face );

    /* close the stream for this face if needed */
    FT_Stream_Free(
      face->stream,
      ( face->face_flags & FT_FACE_FLAG_EXTERNAL_STREAM ) != 0 );

    face->stream = عدم;

    /* now discard client data */
    إذا ( face->generic.finalizer )
      face->generic.finalizer( face );

    /* get rid of it */
    إذا ( face->internal )
    {
      FT_FREE( face->internal );
    }
    FT_FREE( face );
  }


  ساكن فراغ
  Destroy_Driver( FT_Driver  driver )
  {
    FT_List_Finalize( &driver->faces_list,
                      destroy_face,
                      driver->جڑ.memory,
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

    إذا ( !first )
      أرجع FT_THROW( Invalid_CharMap_Handle );

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

    cur = first + face->عدد_charmaps;  /* points after the last one */

    لكل ( ; --cur >= first; )
    {
      إذا ( cur[0]->encoding == FT_ENCODING_UNICODE )
      {
        /* XXX If some new encodings to represent UCS-4 are added, */
        /*     they should be added here.                          */
        إذا ( ( cur[0]->platform_id == TT_PLATFORM_MICROSOFT &&
               cur[0]->encoding_id == TT_MS_ID_UCS_4        )     ||
             ( cur[0]->platform_id == TT_PLATFORM_APPLE_UNICODE &&
               cur[0]->encoding_id == TT_APPLE_ID_UNICODE_32    ) ||
             ( cur[0]->platform_id == TT_PLATFORM_APPLE_UNICODE &&
               cur[0]->encoding_id == TT_APPLE_ID_FULL_UNICODE  &&
               FT_Get_CMap_Format( cur[0] ) == 13               ) )
        {
          face->charmap = cur[0];
          أرجع FT_Err_Ok;
        }
      }
    }

    /* We do not have any UCS-4 charmap.                */
    /* Do the loop again and search for UCS-2 charmaps. */
    cur = first + face->عدد_charmaps;

    لكل ( ; --cur >= first; )
    {
      إذا ( cur[0]->encoding == FT_ENCODING_UNICODE )
      {
        FT_Long  format = FT_Get_CMap_Format( cur[0] );


        إذا ( format != 14 && format != 15 )
        {
          face->charmap = cur[0];
          أرجع FT_Err_Ok;
        }
      }
    }

    أرجع FT_THROW( Invalid_CharMap_Handle );
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
  ساكن FT_CharMap
  find_variant_selector_charmap( FT_Face  face )
  {
    FT_CharMap*  first;
    FT_CharMap*  end;
    FT_CharMap*  cur;


    /* caller should have already checked that `face' is valid */
    FT_ASSERT( face );

    first = face->charmaps;

    إذا ( !first )
      أرجع عدم;

    end = first + face->عدد_charmaps;  /* points after the last one */

    لكل ( cur = first; cur < end; cur++ )
    {
      إذا ( cur[0]->platform_id == TT_PLATFORM_APPLE_UNICODE    &&
           cur[0]->encoding_id == TT_APPLE_ID_VARIANT_SELECTOR )
      {
        FT_Long  format = FT_Get_CMap_Format( cur[0] );


        إذا ( format == 14 || format == 15 )
          أرجع cur[0];
      }
    }

    أرجع عدم;
  }


  /**************************************************************************
   *
   * @Function:
   *   open_face
   *
   * @Description:
   *   This function does some work for FT_Open_Face().
   */
  ساكن FT_Error
  open_face( FT_Driver      driver,
             FT_Stream      *astream,
             FT_Bool        *anexternal_stream,
             FT_Long        face_index,
             FT_Int         عدد_params,
             FT_Parameter*  params,
             FT_Face       *aface )
  {
    FT_Memory         memory;
    FT_Driver_Class   clazz;
    FT_Face           face     = عدم;
    FT_Face_Internal  internal = عدم;

    FT_Error          error, error2;


    clazz  = driver->clazz;
    memory = driver->جڑ.memory;

    /* allocate the face object and perform basic initialization */
    إذا ( FT_ALLOC( face, clazz->face_object_size ) )
      اذهب Fail;

    face->driver = driver;
    face->memory = memory;
    face->stream = *astream;

    /* set the FT_FACE_FLAG_EXTERNAL_STREAM bit for FT_Done_Face */
    إذا ( *anexternal_stream )
      face->face_flags |= FT_FACE_FLAG_EXTERNAL_STREAM;

    إذا ( FT_NEW( internal ) )
      اذهب Fail;

    face->internal = internal;

#ifdef FT_CONFIG_OPTION_INCREMENTAL
    {
      صحيح  i;


      face->internal->incremental_interface = عدم;
      لكل ( i = 0; i < عدد_params && !face->internal->incremental_interface;
            i++ )
        إذا ( params[i].tag == FT_PARAM_TAG_INCREMENTAL )
          face->internal->incremental_interface =
            (FT_Incremental_Interface)params[i].data;
    }
#endif

    face->internal->random_seed = -1;

    إذا ( clazz->init_face )
      error = clazz->init_face( *astream,
                                face,
                                (FT_Int)face_index,
                                عدد_params,
                                params );
    /* Stream may have been changed. */
    *astream = face->stream;
    *anexternal_stream =
      ( face->face_flags & FT_FACE_FLAG_EXTERNAL_STREAM ) != 0;
    إذا ( error )
      اذهب Fail;

    /* select Unicode charmap by default */
    error2 = find_unicode_charmap( face );

    /* if no Unicode charmap can be found, FT_Err_Invalid_CharMap_Handle */
    /* is returned.                                                      */

    /* no error should happen, but we want to play safe */
    إذا ( error2 && FT_ERR_NEQ( error2, Invalid_CharMap_Handle ) )
    {
      error = error2;
      اذهب Fail;
    }

    *aface = face;

  Fail:
    إذا ( error )
    {
      destroy_charmaps( face, memory );
      إذا ( clazz->done_face )
        clazz->done_face( face );
      FT_FREE( internal );
      FT_FREE( face );
      *aface = عدم;
    }

    أرجع error;
  }


  /* there's a Mac-specific extended implementation of FT_New_Face() */
  /* in src/base/ftmac.c                                             */

#ifndef FT_MACINTOSH

  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_New_Face( FT_Library   library,
               ثابت حرف*  pathname,
               FT_Long      face_index,
               FT_Face     *aface )
  {
    FT_Open_Args  args;


    /* test for valid `library' and `aface' delayed to `FT_Open_Face' */
    إذا ( !pathname )
      أرجع FT_THROW( Invalid_Argument );

    args.flags    = FT_OPEN_PATHNAME;
    args.pathname = (حرف*)pathname;
    args.stream   = عدم;

    أرجع ft_open_face_internal( library, &args, face_index, aface, 1 );
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
    إذا ( !file_base )
      أرجع FT_THROW( Invalid_Argument );

    args.flags       = FT_OPEN_MEMORY;
    args.memory_base = file_base;
    args.memory_size = file_size;
    args.stream      = عدم;

    أرجع ft_open_face_internal( library, &args, face_index, aface, 1 );
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
  ساكن فراغ
  memory_stream_close( FT_Stream  stream )
  {
    FT_Memory  memory = (FT_Memory)stream->descriptor.pointer;


    FT_FREE( stream->base );
    stream->size  = 0;
    stream->close = عدم;
    FT_FREE( stream );
  }


  /* Create a new memory stream from a buffer and a size. */
  /* From `ftmac.c'.                                      */
  ساكن FT_Error
  new_memory_stream( FT_Library           library,
                     FT_Byte*             base,
                     FT_ULong             size,
                     FT_Stream_CloseFunc  close,
                     FT_Stream           *astream )
  {
    FT_Error   error;
    FT_Memory  memory;
    FT_Stream  stream = عدم;


    إذا ( !library )
      أرجع FT_THROW( Invalid_Library_Handle );

    إذا ( !base )
      أرجع FT_THROW( Invalid_Argument );

    *astream = عدم;
    memory   = library->memory;
    إذا ( FT_NEW( stream ) )
      اذهب Exit;

    FT_Stream_OpenMemory( stream, base, size );

    stream->descriptor.pointer = memory;
    stream->close              = close;

    *astream = stream;

  Exit:
    أرجع error;
  }


  /* Create a new FT_Face given a buffer and a driver name. */
  /* From `ftmac.c'.                                        */
  FT_LOCAL_DEF( FT_Error )
  open_face_from_buffer( FT_Library   library,
                         FT_Byte*     base,
                         FT_ULong     size,
                         FT_Long      face_index,
                         ثابت حرف*  driver_name,
                         FT_Face     *aface )
  {
    FT_Open_Args  args;
    FT_Error      error;
    FT_Memory     memory = library->memory;


    args.driver = عدم;
    args.flags  = 0;

    إذا ( driver_name )
    {
      args.driver = FT_Get_Module( library, driver_name );
      إذا ( !args.driver )
      {
        FT_FREE( base );
        أرجع FT_THROW( Missing_Module );
      }

      args.flags = args.flags | FT_OPEN_DRIVER;
    }

    /* `memory_stream_close` also frees the stream object. */
    error = new_memory_stream( library,
                               base,
                               size,
                               memory_stream_close,
                               &args.stream );
    إذا ( error )
    {
      FT_FREE( base );
      أرجع error;
    }

    args.flags |= FT_OPEN_STREAM;

#ifdef FT_MACINTOSH
    /* At this point, the face index has served its purpose;  */
    /* whoever calls this function has already used it to     */
    /* locate the correct font data.  We should not propagate */
    /* this index to FT_Open_Face() (unless it is negative).  */

    إذا ( face_index > 0 )
      face_index &= 0x7FFF0000L; /* retain GX data */
#endif

    أرجع ft_open_face_internal( library, &args, face_index, aface, 0 );
  }


  /* Look up `TYP1' or `CID ' table from sfnt table directory.       */
  /* `offset' and `length' must exclude the binary header in tables. */

  /* Type 1 and CID-keyed font drivers should recognize sfnt-wrapped */
  /* format too.  Here, since we can't expect that the TrueType font */
  /* driver is loaded unconditionally, we must parse the font by     */
  /* ourselves.  We are only interested in the name of the table and */
  /* the offset.                                                     */

  ساكن FT_Error
  ft_lookup_PS_in_sfnt_stream( FT_Stream  stream,
                               FT_Long    face_index,
                               FT_ULong*  offset,
                               FT_ULong*  length,
                               FT_Bool*   is_sfnt_cid )
  {
    FT_Error   error;
    FT_UShort  عدد_Tables;
    FT_Long    pstable_index;
    FT_ULong   tag;
    صحيح        i;


    *offset = 0;
    *length = 0;
    *is_sfnt_cid = FALSE;

    /* TODO: support for sfnt-wrapped PS/CID in TTC format */

    /* version check for 'typ1' (should be ignored?) */
    إذا ( FT_READ_ULONG( tag ) )
      أرجع error;
    إذا ( tag != TTAG_typ1 )
      أرجع FT_THROW( Unknown_File_Format );

    إذا ( FT_READ_USHORT( عدد_Tables ) )
      أرجع error;
    إذا ( FT_STREAM_SKIP( 2 * 3 ) ) /* skip binary search header */
      أرجع error;

    pstable_index = -1;
    *is_sfnt_cid  = FALSE;

    لكل ( i = 0; i < عدد_Tables; i++ )
    {
      إذا ( FT_READ_ULONG( tag )     || FT_STREAM_SKIP( 4 )      ||
           FT_READ_ULONG( *offset ) || FT_READ_ULONG( *length ) )
        أرجع error;

      إذا ( tag == TTAG_CID )
      {
        pstable_index++;
        *offset += 22;
        *length -= 22;
        *is_sfnt_cid = TRUE;
        إذا ( face_index < 0 )
          أرجع FT_Err_Ok;
      }
      وإلا إذا ( tag == TTAG_TYP1 )
      {
        pstable_index++;
        *offset += 24;
        *length -= 24;
        *is_sfnt_cid = FALSE;
        إذا ( face_index < 0 )
          أرجع FT_Err_Ok;
      }
      إذا ( face_index >= 0 && pstable_index == face_index )
        أرجع FT_Err_Ok;
    }

    أرجع FT_THROW( Table_Missing );
  }


  FT_LOCAL_DEF( FT_Error )
  open_face_PS_from_sfnt_stream( FT_Library     library,
                                 FT_Stream      stream,
                                 FT_Long        face_index,
                                 FT_Int         عدد_params,
                                 FT_Parameter  *params,
                                 FT_Face       *aface )
  {
    FT_Error   error;
    FT_Memory  memory = library->memory;
    FT_ULong   offset, length;
    FT_ULong   pos;
    FT_Bool    is_sfnt_cid;
    FT_Byte*   sfnt_ps = عدم;

    FT_UNUSED( عدد_params );
    FT_UNUSED( params );


    /* ignore GX stuff */
    إذا ( face_index > 0 )
      face_index &= 0xFFFFL;

    pos = FT_STREAM_POS();

    error = ft_lookup_PS_in_sfnt_stream( stream,
                                         face_index,
                                         &offset,
                                         &length,
                                         &is_sfnt_cid );
    إذا ( error )
      اذهب Exit;

    إذا ( offset > stream->size )
    {
      FT_TRACE2(( "open_face_PS_from_sfnt_stream: invalid table offset\n" ));
      error = FT_THROW( Invalid_Table );
      اذهب Exit;
    }
    وإلا إذا ( length > stream->size - offset )
    {
      FT_TRACE2(( "open_face_PS_from_sfnt_stream: invalid table length\n" ));
      error = FT_THROW( Invalid_Table );
      اذهب Exit;
    }

    error = FT_Stream_Seek( stream, pos + offset );
    إذا ( error )
      اذهب Exit;

    إذا ( FT_QALLOC( sfnt_ps, (FT_Long)length ) )
      اذهب Exit;

    error = FT_Stream_Read( stream, (FT_Byte *)sfnt_ps, length );
    إذا ( error )
    {
      FT_FREE( sfnt_ps );
      اذهب Exit;
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


      إذا ( FT_ERR_EQ( error, Unknown_File_Format ) )
      {
        error1 = FT_Stream_Seek( stream, pos );
        إذا ( error1 )
          أرجع error1;
      }

      أرجع error;
    }
  }


#ifndef FT_MACINTOSH

  /* The resource header says we've got resource_cnt `POST' (type1) */
  /* resources in this file.  They all need to be coalesced into    */
  /* one lump which gets passed on to the type1 driver.             */
  /* Here can be only one PostScript font in a file so face_index   */
  /* must be 0 (or -1).                                             */
  /*                                                                */
  ساكن FT_Error
  Mac_Read_POST_Resource( FT_Library  library,
                          FT_Stream   stream,
                          FT_Long    *offsets,
                          FT_Long     resource_cnt,
                          FT_Long     face_index,
                          FT_Face    *aface )
  {
    FT_Error   error  = FT_ERR( Cannot_Open_Resource );
    FT_Memory  memory = library->memory;

    FT_Byte*   pfb_data = عدم;
    صحيح        i, type, flags;
    FT_ULong   len;
    FT_ULong   pfb_len, pfb_pos, pfb_lenpos;
    FT_ULong   rlen, temp;


    إذا ( face_index == -1 )
      face_index = 0;
    إذا ( face_index != 0 )
      أرجع error;

    /* Find the length of all the POST resources, concatenated.  Assume */
    /* worst case (each resource in its own section).                   */
    pfb_len = 0;
    لكل ( i = 0; i < resource_cnt; i++ )
    {
      error = FT_Stream_Seek( stream, (FT_ULong)offsets[i] );
      إذا ( error )
        اذهب Exit;
      إذا ( FT_READ_ULONG( temp ) )  /* actually LONG */
        اذهب Exit;

      /* FT2 allocator takes signed long buffer length,
       * too large value causing overflow should be checked
       */
      FT_TRACE4(( "                 POST fragment #%d: length=0x%08lx"
                  " total pfb_len=0x%08lx\n",
                  i, temp, pfb_len + temp + 6 ));

      إذا ( FT_MAC_RFORK_MAX_LEN < temp               ||
           FT_MAC_RFORK_MAX_LEN - temp < pfb_len + 6 )
      {
        FT_TRACE2(( "             MacOS resource length cannot exceed"
                    " 0x%08lx\n",
                    FT_MAC_RFORK_MAX_LEN ));

        error = FT_THROW( Invalid_Offset );
        اذهب Exit;
      }

      pfb_len += temp + 6;
    }

    FT_TRACE2(( "             total buffer size to concatenate"
                " %ld POST fragments: 0x%08lx\n",
                 resource_cnt, pfb_len + 2 ));

    إذا ( pfb_len + 2 < 6 )
    {
      FT_TRACE2(( "             too long fragment length makes"
                  " pfb_len confused: pfb_len=0x%08lx\n",
                  pfb_len ));

      error = FT_THROW( Array_Too_Large );
      اذهب Exit;
    }

    إذا ( FT_QALLOC( pfb_data, (FT_Long)pfb_len + 2 ) )
      اذهب Exit;

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

    لكل ( i = 0; i < resource_cnt; i++ )
    {
      error = FT_Stream_Seek( stream, (FT_ULong)offsets[i] );
      إذا ( error )
        اذهب Exit2;
      إذا ( FT_READ_ULONG( rlen ) )
        اذهب Exit2;

      /* FT2 allocator takes signed long buffer length,
       * too large fragment length causing overflow should be checked
       */
      إذا ( 0x7FFFFFFFUL < rlen )
      {
        error = FT_THROW( Invalid_Offset );
        اذهب Exit2;
      }

      إذا ( FT_READ_USHORT( flags ) )
        اذهب Exit2;

      FT_TRACE3(( "POST fragment[%d]:"
                  " offsets=0x%08lx, rlen=0x%08lx, flags=0x%04x\n",
                  i, offsets[i], rlen, flags ));

      error = FT_ERR( Array_Too_Large );

      /* postpone the check of `rlen longer than buffer' */
      /* until `FT_Stream_Read'                          */

      إذا ( ( flags >> 8 ) == 0 )        /* Comment, should not be loaded */
      {
        FT_TRACE3(( "    Skip POST fragment #%d because it is a comment\n",
                    i ));
        استمر;
      }

      /* the flags are part of the resource, so rlen >= 2,  */
      /* but some fonts declare rlen = 0 for empty fragment */
      إذا ( rlen > 2 )
        rlen -= 2;
      وإلا
        rlen = 0;

      إذا ( ( flags >> 8 ) == type )
        len += rlen;
      وإلا
      {
        FT_TRACE3(( "    Write POST fragment #%d header (4-byte) to buffer"
                    " %p + 0x%08lx\n",
                    i, (فراغ*)pfb_data, pfb_lenpos ));

        إذا ( pfb_lenpos + 3 > pfb_len + 2 )
          اذهب Exit2;

        pfb_data[pfb_lenpos    ] = (FT_Byte)( len );
        pfb_data[pfb_lenpos + 1] = (FT_Byte)( len >> 8 );
        pfb_data[pfb_lenpos + 2] = (FT_Byte)( len >> 16 );
        pfb_data[pfb_lenpos + 3] = (FT_Byte)( len >> 24 );

        إذا ( ( flags >> 8 ) == 5 )      /* End of font mark */
          اكسر;

        FT_TRACE3(( "    Write POST fragment #%d header (6-byte) to buffer"
                    " %p + 0x%08lx\n",
                    i, (فراغ*)pfb_data, pfb_pos ));

        إذا ( pfb_pos + 6 > pfb_len + 2 )
          اذهب Exit2;

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

      إذا ( pfb_pos > pfb_len || pfb_pos + rlen > pfb_len )
        اذهب Exit2;

      FT_TRACE3(( "    Load POST fragment #%d (%lu byte) to buffer"
                  " %p + 0x%08lx\n",
                  i, rlen, (فراغ*)pfb_data, pfb_pos ));

      error = FT_Stream_Read( stream, (FT_Byte *)pfb_data + pfb_pos, rlen );
      إذا ( error )
        اذهب Exit2;

      pfb_pos += rlen;
    }

    error = FT_ERR( Array_Too_Large );

    إذا ( pfb_pos + 2 > pfb_len + 2 )
      اذهب Exit2;
    pfb_data[pfb_pos++] = 0x80;
    pfb_data[pfb_pos++] = 3;

    إذا ( pfb_lenpos + 3 > pfb_len + 2 )
      اذهب Exit2;
    pfb_data[pfb_lenpos    ] = (FT_Byte)( len );
    pfb_data[pfb_lenpos + 1] = (FT_Byte)( len >> 8 );
    pfb_data[pfb_lenpos + 2] = (FT_Byte)( len >> 16 );
    pfb_data[pfb_lenpos + 3] = (FT_Byte)( len >> 24 );

    أرجع open_face_from_buffer( library,
                                  pfb_data,
                                  pfb_pos,
                                  face_index,
                                  "type1",
                                  aface );

  Exit2:
    إذا ( FT_ERR_EQ( error, Array_Too_Large ) )
      FT_TRACE2(( "  Abort due to too-short buffer to store"
                  " all POST fragments\n" ));
    وإلا إذا ( FT_ERR_EQ( error, Invalid_Offset ) )
      FT_TRACE2(( "  Abort due to invalid offset in a POST fragment\n" ));

    إذا ( error )
      error = FT_ERR( Cannot_Open_Resource );
    FT_FREE( pfb_data );

  Exit:
    أرجع error;
  }


  /* The resource header says we've got resource_cnt `sfnt'      */
  /* (TrueType/OpenType) resources in this file.  Look through   */
  /* them for the one indicated by face_index, load it into mem, */
  /* pass it on to the truetype driver, and return it.           */
  /*                                                             */
  ساكن FT_Error
  Mac_Read_sfnt_Resource( FT_Library  library,
                          FT_Stream   stream,
                          FT_Long    *offsets,
                          FT_Long     resource_cnt,
                          FT_Long     face_index,
                          FT_Face    *aface )
  {
    FT_Memory  memory = library->memory;
    FT_Byte*   sfnt_data = عدم;
    FT_Error   error;
    FT_ULong   flag_offset;
    FT_ULong   rlen;
    صحيح        is_cff;
    FT_Long    face_index_in_resource = 0;


    إذا ( face_index < 0 )
      face_index = -face_index - 1;
    إذا ( face_index >= resource_cnt )
      أرجع FT_THROW( Cannot_Open_Resource );

    flag_offset = (FT_ULong)offsets[face_index];
    error = FT_Stream_Seek( stream, flag_offset );
    إذا ( error )
      اذهب Exit;

    إذا ( FT_READ_ULONG( rlen ) )
      اذهب Exit;
    إذا ( !rlen )
      أرجع FT_THROW( Cannot_Open_Resource );
    إذا ( rlen > FT_MAC_RFORK_MAX_LEN )
      أرجع FT_THROW( Invalid_Offset );

    error = open_face_PS_from_sfnt_stream( library,
                                           stream,
                                           face_index,
                                           0, عدم,
                                           aface );
    إذا ( !error )
      اذهب Exit;

    /* rewind sfnt stream before open_face_PS_from_sfnt_stream() */
    error = FT_Stream_Seek( stream, flag_offset + 4 );
    إذا ( error )
      اذهب Exit;

    إذا ( FT_QALLOC( sfnt_data, rlen ) )
      أرجع error;
    error = FT_Stream_Read( stream, (FT_Byte *)sfnt_data, rlen );
    إذا ( error )
    {
      FT_FREE( sfnt_data );
      اذهب Exit;
    }

    is_cff = rlen > 4 && !ft_memcmp( sfnt_data, "OTTO", 4 );
    error = open_face_from_buffer( library,
                                   sfnt_data,
                                   rlen,
                                   face_index_in_resource,
                                   is_cff ? "cff" : "truetype",
                                   aface );

  Exit:
    أرجع error;
  }


  /* Check for a valid resource fork header, or a valid dfont    */
  /* header.  In a resource fork the first 16 bytes are repeated */
  /* at the location specified by bytes 4-7.  In a dfont bytes   */
  /* 4-7 point to 16 bytes of zeroes instead.                    */
  /*                                                             */
  ساكن FT_Error
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
    FT_Long    گنتی;


    error = FT_Raccess_Get_HeaderInfo( library, stream, resource_offset,
                                       &map_offset, &rdata_pos );
    إذا ( error )
      أرجع error;

    /* POST resources must be sorted to concatenate properly */
    error = FT_Raccess_Get_DataOffsets( library, stream,
                                        map_offset, rdata_pos,
                                        TTAG_POST, TRUE,
                                        &data_offsets, &گنتی );
    إذا ( !error )
    {
      error = Mac_Read_POST_Resource( library, stream, data_offsets, گنتی,
                                      face_index, aface );
      FT_FREE( data_offsets );
      /* POST exists in an LWFN providing a single face */
      إذا ( !error )
        (*aface)->عدد_faces = 1;
      أرجع error;
    }

    /* sfnt resources should not be sorted to preserve the face order by
       QuickDraw API */
    error = FT_Raccess_Get_DataOffsets( library, stream,
                                        map_offset, rdata_pos,
                                        TTAG_sfnt, FALSE,
                                        &data_offsets, &گنتی );
    إذا ( !error )
    {
      FT_Long  face_index_internal = face_index % گنتی;


      error = Mac_Read_sfnt_Resource( library, stream, data_offsets, گنتی,
                                      face_index_internal, aface );
      FT_FREE( data_offsets );
      إذا ( !error )
      {
        (*aface)->عدد_faces  = گنتی;
        (*aface)->face_index = face_index_internal;
      }
    }

    أرجع error;
  }


  /* Check for a valid macbinary header, and if we find one   */
  /* check that the (flattened) resource fork in it is valid. */
  /*                                                          */
  ساكن FT_Error
  Is_Mac_ثنائی( FT_Library  library,
               FT_Stream   stream,
               FT_Long     face_index,
               FT_Face    *aface )
  {
    unsigned حرف  header[128];
    FT_Error       error;
    FT_Long        dlen, offset;


    إذا ( !stream )
      أرجع FT_THROW( Invalid_Stream_Operation );

    error = FT_Stream_Seek( stream, 0 );
    إذا ( error )
      اذهب Exit;

    error = FT_Stream_Read( stream, (FT_Byte*)header, 128 );
    إذا ( error )
      اذهب Exit;

    إذا (            header[ 0] !=   0 ||
                    header[74] !=   0 ||
                    header[82] !=   0 ||
                    header[ 1] ==   0 ||
                    header[ 1] >   33 ||
                    header[63] !=   0 ||
         header[2 + header[1]] !=   0 ||
                  header[0x53] > 0x7F )
      أرجع FT_THROW( Unknown_File_Format );

    dlen = ( header[0x53] << 24 ) |
           ( header[0x54] << 16 ) |
           ( header[0x55] <<  8 ) |
             header[0x56];
#إذا 0
    rlen = ( header[0x57] << 24 ) |
           ( header[0x58] << 16 ) |
           ( header[0x59] <<  8 ) |
             header[0x5A];
#endif /* 0 */
    offset = 128 + ( ( dlen + 127 ) & ~127 );

    أرجع IsMacResource( library, stream, offset, face_index, aface );

  Exit:
    أرجع error;
  }


  ساكن FT_Error
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

    حرف*      file_names[FT_RACCESS_N_RULES];
    FT_Long    offsets[FT_RACCESS_N_RULES];
    FT_Error   errors[FT_RACCESS_N_RULES];
    FT_Bool    is_darwin_vfs, vfs_rfork_has_no_font = FALSE; /* not tested */

    FT_Open_Args  args2;
    FT_Stream     stream2 = عدم;


    FT_Raccess_Guess( library, stream,
                      args->pathname, file_names, offsets, errors );

    لكل ( i = 0; i < FT_RACCESS_N_RULES; i++ )
    {
      is_darwin_vfs = ft_raccess_rule_by_darwin_vfs( library, i );
      إذا ( is_darwin_vfs && vfs_rfork_has_no_font )
      {
        FT_TRACE3(( "Skip rule %u: darwin vfs resource fork"
                    " is already checked and"
                    " no font is found\n",
                    i ));
        استمر;
      }

      إذا ( errors[i] )
      {
        FT_TRACE3(( "Error 0x%x has occurred in rule %u\n",
                    errors[i], i ));
        استمر;
      }

      args2.flags    = FT_OPEN_PATHNAME;
      args2.pathname = file_names[i] ? file_names[i] : args->pathname;

      FT_TRACE3(( "Try rule %u: %s (offset=%ld) ...",
                  i, args2.pathname, offsets[i] ));

      error = FT_Stream_New( library, &args2, &stream2 );
      إذا ( is_darwin_vfs && FT_ERR_EQ( error, Cannot_Open_Stream ) )
        vfs_rfork_has_no_font = TRUE;

      إذا ( error )
      {
        FT_TRACE3(( "failed\n" ));
        استمر;
      }

      error = IsMacResource( library, stream2, offsets[i],
                             face_index, aface );
      FT_Stream_Free( stream2, 0 );

      FT_TRACE3(( "%s\n", error ? "failed": "successful" ));

      إذا ( !error )
          اكسر;
      وإلا إذا ( is_darwin_vfs )
          vfs_rfork_has_no_font = TRUE;
    }

    لكل (i = 0; i < FT_RACCESS_N_RULES; i++)
    {
      إذا ( file_names[i] )
        FT_FREE( file_names[i] );
    }

    /* Caller (load_mac_face) requires FT_Err_Unknown_File_Format. */
    إذا ( error )
      error = FT_ERR( Unknown_File_Format );

    أرجع error;

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
  ساكن FT_Error
  load_mac_face( FT_Library           library,
                 FT_Stream            stream,
                 FT_Long              face_index,
                 FT_Face             *aface,
                 ثابت FT_Open_Args  *args )
  {
    FT_Error error;
    FT_UNUSED( args );


    error = Is_Mac_ثنائی( library, stream, face_index, aface );
    إذا ( FT_ERR_EQ( error, Unknown_File_Format ) )
    {

#undef  FT_COMPONENT
#define FT_COMPONENT  raccess

#ifdef FT_DEBUG_LEVEL_TRACE
      FT_TRACE3(( "Try as dfont: " ));
      إذا ( !( args->flags & FT_OPEN_MEMORY ) )
        FT_TRACE3(( "%s ...", args->pathname ));
#endif

      error = IsMacResource( library, stream, 0, face_index, aface );

      FT_TRACE3(( "%s\n", error ? "failed" : "successful" ));

#undef  FT_COMPONENT
#define FT_COMPONENT  objs

    }

    إذا ( ( FT_ERR_EQ( error, Unknown_File_Format )      ||
           FT_ERR_EQ( error, Invalid_Stream_Operation ) ) &&
         ( args->flags & FT_OPEN_PATHNAME )               )
      error = load_face_in_embedded_rfork( library, stream,
                                           face_index, aface, args );
    أرجع error;
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
    أرجع ft_open_face_internal( library, args, face_index, aface, 1 );
  }


  ساكن FT_Error
  ft_open_face_internal( FT_Library           library,
                         ثابت FT_Open_Args*  args,
                         FT_Long              face_index,
                         FT_Face             *aface,
                         FT_Bool              test_mac_fonts )
  {
    FT_Error     error;
    FT_Driver    driver = عدم;
    FT_Memory    memory = عدم;
    FT_Stream    stream = عدم;
    FT_Face      face   = عدم;
    FT_List_گرہ  گرہ   = عدم;
    FT_Bool      external_stream;
    FT_Module*   cur;
    FT_Module*   limit;

#ifndef FT_CONFIG_OPTION_MAC_FONTS
    FT_UNUSED( test_mac_fonts );
#endif


    /* only use lower 31 bits together with sign bit */
    إذا ( face_index > 0 )
      face_index &= 0x7FFFFFFFL;
    وإلا
    {
      face_index  = -face_index;
      face_index &= 0x7FFFFFFFL;
      face_index  = -face_index;
    }

#ifdef FT_DEBUG_LEVEL_TRACE
    FT_TRACE3(( "FT_Open_Face: " ));
    إذا ( face_index < 0 )
      FT_TRACE3(( "Requesting number of faces and named instances\n"));
    وإلا
    {
      FT_TRACE3(( "Requesting face %ld", face_index & 0xFFFFL ));
      إذا ( face_index & 0x7FFF0000L )
        FT_TRACE3(( ", named instance %ld", face_index >> 16 ));
      FT_TRACE3(( "\n" ));
    }
#endif

    /* test for valid `library' delayed to `FT_Stream_New' */

    إذا ( !args )
      أرجع FT_THROW( Invalid_Argument );

    external_stream = FT_BOOL( ( args->flags & FT_OPEN_STREAM ) &&
                               args->stream                     );

    /* create input stream */
    error = FT_Stream_New( library, args, &stream );
    إذا ( error )
      اذهب Fail3;

    /* Do this error check after `FT_Stream_New` to ensure that the */
    /* 'close' callback is called.                                  */
    إذا ( !aface && face_index >= 0 )
    {
      error = FT_THROW( Invalid_Argument );
      اذهب Fail3;
    }

    memory = library->memory;

    /* If the font driver is specified in the `args' structure, use */
    /* it.  Otherwise, we scan the list of registered drivers.      */
    إذا ( ( args->flags & FT_OPEN_DRIVER ) && args->driver )
    {
      driver = FT_DRIVER( args->driver );

      /* not all modules are drivers, so check... */
      إذا ( FT_MODULE_IS_DRIVER( driver ) )
      {
        FT_Int         عدد_params = 0;
        FT_Parameter*  params     = عدم;


        إذا ( args->flags & FT_OPEN_PARAMS )
        {
          عدد_params = args->عدد_params;
          params     = args->params;
        }

        error = open_face( driver, &stream, &external_stream, face_index,
                           عدد_params, params, &face );
        إذا ( !error )
          اذهب Success;
      }
      وإلا
        error = FT_THROW( Invalid_Handle );

      FT_Stream_Free( stream, external_stream );
      اذهب Fail;
    }
    وإلا
    {
      error = FT_ERR( Missing_Module );

      /* check each font driver for an appropriate format */
      cur   = library->modules;
      limit = cur + library->عدد_modules;

      لكل ( ; cur < limit; cur++ )
      {
        /* not all modules are font drivers, so check... */
        إذا ( FT_MODULE_IS_DRIVER( cur[0] ) )
        {
          FT_Int         عدد_params = 0;
          FT_Parameter*  params     = عدم;


          driver = FT_DRIVER( cur[0] );

          إذا ( args->flags & FT_OPEN_PARAMS )
          {
            عدد_params = args->عدد_params;
            params     = args->params;
          }

          error = open_face( driver, &stream, &external_stream, face_index,
                             عدد_params, params, &face );
          إذا ( !error )
            اذهب Success;

#ifdef FT_CONFIG_OPTION_MAC_FONTS
          إذا ( test_mac_fonts                                           &&
               ft_strcmp( cur[0]->clazz->module_name, "truetype" ) == 0 &&
               FT_ERR_EQ( error, Table_Missing )                        )
          {
            /* TrueType but essential tables are missing */
            error = FT_Stream_Seek( stream, 0 );
            إذا ( error )
              اكسر;

            error = open_face_PS_from_sfnt_stream( library,
                                                   stream,
                                                   face_index,
                                                   عدد_params,
                                                   params,
                                                   aface );
            إذا ( !error )
            {
              FT_Stream_Free( stream, external_stream );
              أرجع error;
            }
          }
#endif

          إذا ( FT_ERR_NEQ( error, Unknown_File_Format ) )
            اذهب Fail3;
        }
      }

    Fail3:
      /* If we are on the mac, and we get an                          */
      /* FT_Err_Invalid_Stream_Operation it may be because we have an */
      /* empty data fork, so we need to check the resource fork.      */
      إذا ( FT_ERR_NEQ( error, Cannot_Open_Stream )       &&
           FT_ERR_NEQ( error, Unknown_File_Format )      &&
           FT_ERR_NEQ( error, Invalid_Stream_Operation ) )
        اذهب Fail2;

#إذا !defined( FT_MACINTOSH ) && defined( FT_CONFIG_OPTION_MAC_FONTS )
      إذا ( test_mac_fonts )
      {
        error = load_mac_face( library, stream, face_index, aface, args );
        إذا ( !error )
        {
          /* We don't want to go to Success here.  We've already done   */
          /* that.  On the other hand, if we succeeded we still need to */
          /* close this stream (we opened a different stream which      */
          /* extracted the interesting information out of this stream   */
          /* here.  That stream will still be open and the face will    */
          /* point to it).                                              */
          FT_Stream_Free( stream, external_stream );
          أرجع error;
        }
      }

      إذا ( FT_ERR_NEQ( error, Unknown_File_Format ) )
        اذهب Fail2;
#endif  /* !FT_MACINTOSH && FT_CONFIG_OPTION_MAC_FONTS */

      /* no driver is able to handle this format */
      error = FT_THROW( Unknown_File_Format );

  Fail2:
      FT_Stream_Free( stream, external_stream );
      اذهب Fail;
    }

  Success:
    FT_TRACE4(( "FT_Open_Face: New face object, adding to list\n" ));

    /* add the face object to its driver's list */
    إذا ( FT_QNEW( گرہ ) )
      اذهب Fail;

    گرہ->data = face;
    /* don't assume driver is the same as face->driver, so use */
    /* face->driver instead.                                   */
    FT_List_Add( &face->driver->faces_list, گرہ );

    /* now allocate a glyph slot object for the face */
    FT_TRACE4(( "FT_Open_Face: Creating glyph slot\n" ));

    إذا ( face_index >= 0 )
    {
      error = FT_New_GlyphSlot( face, عدم );
      إذا ( error )
        اذهب Fail;

      /* finally, allocate a size object for the face */
      {
        FT_Size  size;


        FT_TRACE4(( "FT_Open_Face: Creating size object\n" ));

        error = FT_New_Size( face, &size );
        إذا ( error )
          اذهب Fail;

        face->size = size;
      }
    }

    /* some checks */

    إذا ( FT_IS_SCALABLE( face ) )
    {
      إذا ( face->height < 0 )
        face->height = (FT_Short)-face->height;

      إذا ( !FT_HAS_VERTICAL( face ) )
        face->max_advance_height = (FT_Short)face->height;
    }

    إذا ( FT_HAS_FIXED_SIZES( face ) )
    {
      FT_Int  i;


      لكل ( i = 0; i < face->عدد_fixed_sizes; i++ )
      {
        FT_Bitmap_Size*  bsize = face->available_sizes + i;


        إذا ( bsize->height < 0 )
          bsize->height = -bsize->height;
        إذا ( bsize->x_ppem < 0 )
          bsize->x_ppem = -bsize->x_ppem;
        إذا ( bsize->y_ppem < 0 )
          bsize->y_ppem = -bsize->y_ppem;

        /* check whether negation actually has worked */
        إذا ( bsize->height < 0 || bsize->x_ppem < 0 || bsize->y_ppem < 0 )
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

    إذا ( aface )
      *aface = face;
    وإلا
      FT_Done_Face( face );

    اذهب Exit;

  Fail:
    إذا ( گرہ )
      FT_Done_Face( face );    /* face must be in the driver's list */
    وإلا إذا ( face )
      destroy_face( memory, face, driver );

  Exit:
#ifdef FT_DEBUG_LEVEL_TRACE
    إذا ( !error && face_index < 0 )
    {
      FT_TRACE3(( "FT_Open_Face: The font has %ld face%s\n",
                  face->عدد_faces,
                  face->عدد_faces == 1 ? "" : "s" ));
      FT_TRACE3(( "              and %ld named instance%s for face %ld\n",
                  face->style_flags >> 16,
                  ( face->style_flags >> 16 ) == 1 ? "" : "s",
                  -face_index - 1 ));
    }
#endif

    FT_TRACE4(( "FT_Open_Face: Return 0x%x\n", error ));

    أرجع error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Attach_File( FT_Face      face,
                  ثابت حرف*  filepathname )
  {
    FT_Open_Args  open;


    /* test for valid `face' delayed to `FT_Attach_Stream' */

    إذا ( !filepathname )
      أرجع FT_THROW( Invalid_Argument );

    open.stream   = عدم;
    open.flags    = FT_OPEN_PATHNAME;
    open.pathname = (حرف*)filepathname;

    أرجع FT_Attach_Stream( face, &open );
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

    إذا ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );

    driver = face->driver;
    إذا ( !driver )
      أرجع FT_THROW( Invalid_Driver_Handle );

    error = FT_Stream_New( driver->جڑ.library, parameters, &stream );
    إذا ( error )
      اذهب Exit;

    /* we implement FT_Attach_Stream in each driver through the */
    /* `attach_file' interface                                  */

    error = FT_ERR( Unimplemented_Feature );
    clazz = driver->clazz;
    إذا ( clazz->attach_file )
      error = clazz->attach_file( face, stream );

    /* close the attached stream */
    FT_Stream_Free( stream,
                    FT_BOOL( parameters->stream                     &&
                             ( parameters->flags & FT_OPEN_STREAM ) ) );

  Exit:
    أرجع error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Reference_Face( FT_Face  face )
  {
    إذا ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );

    face->internal->refcount++;

    أرجع FT_Err_Ok;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Done_Face( FT_Face  face )
  {
    FT_Error     error;
    FT_Driver    driver;
    FT_Memory    memory;
    FT_List_گرہ  گرہ;


    error = FT_ERR( Invalid_Face_Handle );
    إذا ( face && face->driver )
    {
      face->internal->refcount--;
      إذا ( face->internal->refcount > 0 )
        error = FT_Err_Ok;
      وإلا
      {
        driver = face->driver;
        memory = driver->جڑ.memory;

        /* find face in driver's list */
        گرہ = FT_List_Find( &driver->faces_list, face );
        إذا ( گرہ )
        {
          /* remove face object from the driver's list */
          FT_List_Remove( &driver->faces_list, گرہ );
          FT_FREE( گرہ );

          /* now destroy the object proper */
          destroy_face( memory, face, driver );
          error = FT_Err_Ok;
        }
      }
    }

    أرجع error;
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

    FT_Size          size = عدم;
    FT_List_گرہ      گرہ = عدم;

    FT_Size_Internal  internal = عدم;


    إذا ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );

    إذا ( !asize )
      أرجع FT_THROW( Invalid_Argument );

    إذا ( !face->driver )
      أرجع FT_THROW( Invalid_Driver_Handle );

    *asize = عدم;

    driver = face->driver;
    clazz  = driver->clazz;
    memory = face->memory;

    /* Allocate new size object and perform basic initialisation */
    إذا ( FT_ALLOC( size, clazz->size_object_size ) || FT_QNEW( گرہ ) )
      اذهب Exit;

    size->face = face;

    إذا ( FT_NEW( internal ) )
      اذهب Exit;

    size->internal = internal;

    إذا ( clazz->init_size )
      error = clazz->init_size( size );

    /* in case of success, add to the face's list */
    إذا ( !error )
    {
      *asize     = size;
      گرہ->data = size;
      FT_List_Add( &face->sizes_list, گرہ );
    }

  Exit:
    إذا ( error )
    {
      FT_FREE( گرہ );
      إذا ( size )
        FT_FREE( size->internal );
      FT_FREE( size );
    }

    أرجع error;
  }


  /* documentation is in ftobjs.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Done_Size( FT_Size  size )
  {
    FT_Error     error;
    FT_Driver    driver;
    FT_Memory    memory;
    FT_Face      face;
    FT_List_گرہ  گرہ;


    إذا ( !size )
      أرجع FT_THROW( Invalid_Size_Handle );

    face = size->face;
    إذا ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );

    driver = face->driver;
    إذا ( !driver )
      أرجع FT_THROW( Invalid_Driver_Handle );

    memory = driver->جڑ.memory;

    error = FT_Err_Ok;
    گرہ  = FT_List_Find( &face->sizes_list, size );
    إذا ( گرہ )
    {
      FT_List_Remove( &face->sizes_list, گرہ );
      FT_FREE( گرہ );

      إذا ( face->size == size )
      {
        face->size = عدم;
        إذا ( face->sizes_list.head )
          face->size = (FT_Size)(face->sizes_list.head->data);
      }

      destroy_size( memory, size, driver );
    }
    وإلا
      error = FT_THROW( Invalid_Size_Handle );

    أرجع error;
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


    إذا ( !FT_HAS_FIXED_SIZES( face ) )
      أرجع FT_THROW( Invalid_Face_Handle );

    /* FT_Bitmap_Size doesn't provide enough info... */
    إذا ( req->type != FT_SIZE_REQUEST_TYPE_NOMINAL )
      أرجع FT_THROW( Unimplemented_Feature );

    w = FT_REQUEST_WIDTH ( req );
    h = FT_REQUEST_HEIGHT( req );

    إذا ( req->width && !req->height )
      h = w;
    وإلا إذا ( !req->width && req->height )
      w = h;

    w = FT_PIX_ROUND( w );
    h = FT_PIX_ROUND( h );

    إذا ( !w || !h )
      أرجع FT_THROW( Invalid_Pixel_Size );

    لكل ( i = 0; i < face->عدد_fixed_sizes; i++ )
    {
      FT_Bitmap_Size*  bsize = face->available_sizes + i;


      إذا ( h != FT_PIX_ROUND( bsize->y_ppem ) )
        استمر;

      إذا ( w == FT_PIX_ROUND( bsize->x_ppem ) || ignore_width )
      {
        FT_TRACE3(( "FT_Match_Size: bitmap strike %d matches\n", i ));

        إذا ( size_index )
          *size_index = (FT_ULong)i;

        أرجع FT_Err_Ok;
      }
    }

    FT_TRACE3(( "FT_Match_Size: no matching bitmap strike\n" ));

    أرجع FT_THROW( Invalid_Pixel_Size );
  }


  /* documentation is in ftobjs.h */

  FT_BASE_DEF( فراغ )
  ft_synthesize_vertical_metrics( FT_Glyph_Metrics*  metrics,
                                  FT_Pos             advance )
  {
    FT_Pos  height = metrics->height;


    /* compensate for glyph with bbox above/below the baseline */
    إذا ( metrics->horiBearingY < 0 )
    {
      إذا ( height < metrics->horiBearingY )
        height = metrics->horiBearingY;
    }
    وإلا إذا ( metrics->horiBearingY > 0 )
      height -= metrics->horiBearingY;

    /* the factor 1.2 is a heuristical value */
    إذا ( !advance )
      advance = height * 12 / 10;

    metrics->vertBearingX = metrics->horiBearingX - metrics->horiAdvance / 2;
    metrics->vertBearingY = ( advance - height ) / 2;
    metrics->vertAdvance  = advance;
  }


  ساكن فراغ
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
#وإلا /* !GRID_FIT_METRICS */
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


  FT_BASE_DEF( فراغ )
  FT_Select_Metrics( FT_Face   face,
                     FT_ULong  strike_index )
  {
    FT_Size_Metrics*  metrics;
    FT_Bitmap_Size*   bsize;


    metrics = &face->size->metrics;
    bsize   = face->available_sizes + strike_index;

    metrics->x_ppem = (FT_UShort)( ( bsize->x_ppem + 32 ) >> 6 );
    metrics->y_ppem = (FT_UShort)( ( bsize->y_ppem + 32 ) >> 6 );

    إذا ( FT_IS_SCALABLE( face ) )
    {
      metrics->x_scale = FT_DivFix( bsize->x_ppem,
                                    face->units_per_EM );
      metrics->y_scale = FT_DivFix( bsize->y_ppem,
                                    face->units_per_EM );

      ft_recompute_scaled_metrics( face, metrics );
    }
    وإلا
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

    إذا ( FT_IS_SCALABLE( face ) )
    {
      FT_Long  w = 0, h = 0, scaled_w = 0, scaled_h = 0;


      بدل ( req->type )
      {
      حالة FT_SIZE_REQUEST_TYPE_NOMINAL:
        w = h = face->units_per_EM;
        اكسر;

      حالة FT_SIZE_REQUEST_TYPE_REAL_DIM:
        w = h = face->ascender - face->descender;
        اكسر;

      حالة FT_SIZE_REQUEST_TYPE_BBOX:
        w = face->bbox.xMax - face->bbox.xMin;
        h = face->bbox.yMax - face->bbox.yMin;
        اكسر;

      حالة FT_SIZE_REQUEST_TYPE_CELL:
        w = face->max_advance_width;
        h = face->ascender - face->descender;
        اكسر;

      حالة FT_SIZE_REQUEST_TYPE_SCALES:
        metrics->x_scale = (FT_Fixed)req->width;
        metrics->y_scale = (FT_Fixed)req->height;
        إذا ( !metrics->x_scale )
          metrics->x_scale = metrics->y_scale;
        وإلا إذا ( !metrics->y_scale )
          metrics->y_scale = metrics->x_scale;
        اذهب Calculate_Ppem;

      حالة FT_SIZE_REQUEST_TYPE_MAX:
        اكسر;
      }

      /* to be on the safe side */
      إذا ( w < 0 )
        w = -w;

      إذا ( h < 0 )
        h = -h;

      scaled_w = FT_REQUEST_WIDTH ( req );
      scaled_h = FT_REQUEST_HEIGHT( req );

      /* determine scales */
      إذا ( req->height || !req->width )
      {
        إذا ( h == 0 )
        {
          FT_ERROR(( "FT_Request_Metrics: Divide by zero\n" ));
          error = FT_ERR( Divide_By_Zero );
          اذهب Exit;
        }

        metrics->y_scale = FT_DivFix( scaled_h, h );
      }

      إذا ( req->width )
      {
        إذا ( w == 0 )
        {
          FT_ERROR(( "FT_Request_Metrics: Divide by zero\n" ));
          error = FT_ERR( Divide_By_Zero );
          اذهب Exit;
        }

        metrics->x_scale = FT_DivFix( scaled_w, w );
      }
      وإلا
      {
        metrics->x_scale = metrics->y_scale;
        scaled_w = FT_MulDiv( scaled_h, w, h );
      }

      إذا ( !req->height )
      {
        metrics->y_scale = metrics->x_scale;
        scaled_h = FT_MulDiv( scaled_w, h, w );
      }

      إذا ( req->type == FT_SIZE_REQUEST_TYPE_CELL )
      {
        إذا ( metrics->y_scale > metrics->x_scale )
          metrics->y_scale = metrics->x_scale;
        وإلا
          metrics->x_scale = metrics->y_scale;
      }

  Calculate_Ppem:
      /* calculate the ppems */
      إذا ( req->type != FT_SIZE_REQUEST_TYPE_NOMINAL )
      {
        scaled_w = FT_MulFix( face->units_per_EM, metrics->x_scale );
        scaled_h = FT_MulFix( face->units_per_EM, metrics->y_scale );
      }

      scaled_w = ( scaled_w + 32 ) >> 6;
      scaled_h = ( scaled_h + 32 ) >> 6;
      إذا ( scaled_w > (FT_Long)FT_USHORT_MAX ||
           scaled_h > (FT_Long)FT_USHORT_MAX )
      {
        FT_ERROR(( "FT_Request_Metrics: Resulting ppem size too large\n" ));
        error = FT_ERR( Invalid_Pixel_Size );
        اذهب Exit;
      }

      metrics->x_ppem = (FT_UShort)scaled_w;
      metrics->y_ppem = (FT_UShort)scaled_h;

      ft_recompute_scaled_metrics( face, metrics );
    }
    وإلا
    {
      FT_ZERO( metrics );
      metrics->x_scale = 1L << 16;
      metrics->y_scale = 1L << 16;
    }

  Exit:
    أرجع error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Select_Size( FT_Face  face,
                  FT_Int   strike_index )
  {
    FT_Error         error = FT_Err_Ok;
    FT_Driver_Class  clazz;


    إذا ( !face || !FT_HAS_FIXED_SIZES( face ) )
      أرجع FT_THROW( Invalid_Face_Handle );

    إذا ( strike_index < 0 || strike_index >= face->عدد_fixed_sizes )
      أرجع FT_THROW( Invalid_Argument );

    clazz = face->driver->clazz;

    إذا ( clazz->select_size )
    {
      error = clazz->select_size( face->size, (FT_ULong)strike_index );

      FT_TRACE5(( "FT_Select_Size (%s driver):\n",
                  face->driver->جڑ.clazz->module_name ));
    }
    وإلا
    {
      FT_Select_Metrics( face, (FT_ULong)strike_index );

      FT_TRACE5(( "FT_Select_Size:\n" ));
    }

#ifdef FT_DEBUG_LEVEL_TRACE
    {
      FT_Size_Metrics*  metrics = &face->size->metrics;


      FT_TRACE5(( "  x scale: %ld (%f)\n",
                  metrics->x_scale, (مزدوج)metrics->x_scale / 65536 ));
      FT_TRACE5(( "  y scale: %ld (%f)\n",
                  metrics->y_scale, (مزدوج)metrics->y_scale / 65536 ));
      FT_TRACE5(( "  ascender: %f\n",
                  (مزدوج)metrics->ascender / 64 ));
      FT_TRACE5(( "  descender: %f\n",
                  (مزدوج)metrics->descender / 64 ));
      FT_TRACE5(( "  height: %f\n",
                  (مزدوج)metrics->height / 64 ));
      FT_TRACE5(( "  max advance: %f\n",
                  (مزدوج)metrics->max_advance / 64 ));
      FT_TRACE5(( "  x ppem: %d\n", metrics->x_ppem ));
      FT_TRACE5(( "  y ppem: %d\n", metrics->y_ppem ));
    }
#endif

    أرجع error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Request_Size( FT_Face          face,
                   FT_Size_Request  req )
  {
    FT_Error         error;
    FT_Driver_Class  clazz;
    FT_ULong         strike_index;


    إذا ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );

    إذا ( !face->size )
      أرجع FT_THROW( Invalid_Size_Handle );

    إذا ( !req || req->width < 0 || req->height < 0 ||
         req->type >= FT_SIZE_REQUEST_TYPE_MAX )
      أرجع FT_THROW( Invalid_Argument );

    /* signal the auto-hinter to recompute its size metrics */
    /* (if requested)                                       */
    face->size->internal->autohint_metrics.x_scale = 0;

    clazz = face->driver->clazz;

    إذا ( clazz->request_size )
    {
      error = clazz->request_size( face->size, req );

      FT_TRACE5(( "FT_Request_Size (%s driver):\n",
                  face->driver->جڑ.clazz->module_name ));
    }
    وإلا إذا ( !FT_IS_SCALABLE( face ) && FT_HAS_FIXED_SIZES( face ) )
    {
      /*
       * The reason that a driver doesn't have `request_size' defined is
       * either that the scaling here suffices or that the supported formats
       * are bitmap-only and size matching is not implemented.
       *
       * In the latter case, a simple size matching is done.
       */
      error = FT_Match_Size( face, req, 0, &strike_index );
      إذا ( error )
        اذهب Exit;

      أرجع FT_Select_Size( face, (FT_Int)strike_index );
    }
    وإلا
    {
      error = FT_Request_Metrics( face, req );
      إذا ( error )
        اذهب Exit;

      FT_TRACE5(( "FT_Request_Size:\n" ));
    }

#ifdef FT_DEBUG_LEVEL_TRACE
    {
      FT_Size_Metrics*  metrics = &face->size->metrics;


      FT_TRACE5(( "  x scale: %ld (%f)\n",
                  metrics->x_scale, (مزدوج)metrics->x_scale / 65536 ));
      FT_TRACE5(( "  y scale: %ld (%f)\n",
                  metrics->y_scale, (مزدوج)metrics->y_scale / 65536 ));
      FT_TRACE5(( "  ascender: %f\n",
                  (مزدوج)metrics->ascender / 64 ));
      FT_TRACE5(( "  descender: %f\n",
                  (مزدوج)metrics->descender / 64 ));
      FT_TRACE5(( "  height: %f\n",
                  (مزدوج)metrics->height / 64 ));
      FT_TRACE5(( "  max advance: %f\n",
                  (مزدوج)metrics->max_advance / 64 ));
      FT_TRACE5(( "  x ppem: %d\n", metrics->x_ppem ));
      FT_TRACE5(( "  y ppem: %d\n", metrics->y_ppem ));
    }
#endif

  Exit:
    أرجع error;
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

    إذا ( !char_width )
      char_width = char_height;
    وإلا إذا ( !char_height )
      char_height = char_width;

    إذا ( !horz_resolution )
      horz_resolution = vert_resolution;
    وإلا إذا ( !vert_resolution )
      vert_resolution = horz_resolution;

    إذا ( char_width  < 1 * 64 )
      char_width  = 1 * 64;
    إذا ( char_height < 1 * 64 )
      char_height = 1 * 64;

    إذا ( !horz_resolution )
      horz_resolution = vert_resolution = 72;

    req.type           = FT_SIZE_REQUEST_TYPE_NOMINAL;
    req.width          = char_width;
    req.height         = char_height;
    req.horiResolution = horz_resolution;
    req.vertResolution = vert_resolution;

    أرجع FT_Request_Size( face, &req );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Set_Pixel_Sizes( FT_Face  face,
                      FT_UInt  pixel_width,
                      FT_UInt  pixel_height )
  {
    FT_Size_RequestRec  req;


    /* check of `face' delayed to `FT_Request_Size' */

    إذا ( pixel_width == 0 )
      pixel_width = pixel_height;
    وإلا إذا ( pixel_height == 0 )
      pixel_height = pixel_width;

    إذا ( pixel_width  < 1 )
      pixel_width  = 1;
    إذا ( pixel_height < 1 )
      pixel_height = 1;

    /* use `>=' to avoid potential compiler warning on 16bit platforms */
    إذا ( pixel_width >= 0xFFFFU )
      pixel_width = 0xFFFFU;
    إذا ( pixel_height >= 0xFFFFU )
      pixel_height = 0xFFFFU;

    req.type           = FT_SIZE_REQUEST_TYPE_NOMINAL;
    req.width          = (FT_Long)( pixel_width << 6 );
    req.height         = (FT_Long)( pixel_height << 6 );
    req.horiResolution = 0;
    req.vertResolution = 0;

    أرجع FT_Request_Size( face, &req );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Kerning( FT_Face     face,
                  FT_UInt     بایاں_glyph,
                  FT_UInt     دایاں_glyph,
                  FT_UInt     kern_mode,
                  FT_Vector  *akerning )
  {
    FT_Error   error = FT_Err_Ok;
    FT_Driver  driver;


    إذا ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );

    إذا ( !akerning )
      أرجع FT_THROW( Invalid_Argument );

    driver = face->driver;

    akerning->x = 0;
    akerning->y = 0;

    إذا ( driver->clazz->get_kerning )
    {
      error = driver->clazz->get_kerning( face,
                                          بایاں_glyph,
                                          دایاں_glyph,
                                          akerning );
      إذا ( !error )
      {
        إذا ( kern_mode != FT_KERNING_UNSCALED )
        {
          akerning->x = FT_MulFix( akerning->x, face->size->metrics.x_scale );
          akerning->y = FT_MulFix( akerning->y, face->size->metrics.y_scale );

          إذا ( kern_mode != FT_KERNING_UNFITTED )
          {
            FT_Pos  orig_x = akerning->x;
            FT_Pos  orig_y = akerning->y;


            /* we scale down kerning values for small ppem values */
            /* to avoid that rounding makes them too big.         */
            /* `25' has been determined heuristically.            */
            إذا ( face->size->metrics.x_ppem < 25 )
              akerning->x = FT_MulDiv( orig_x,
                                       face->size->metrics.x_ppem, 25 );
            إذا ( face->size->metrics.y_ppem < 25 )
              akerning->y = FT_MulDiv( orig_y,
                                       face->size->metrics.y_ppem, 25 );

            akerning->x = FT_PIX_ROUND( akerning->x );
            akerning->y = FT_PIX_ROUND( akerning->y );

#ifdef FT_DEBUG_LEVEL_TRACE
            {
              FT_Pos  orig_x_rounded = FT_PIX_ROUND( orig_x );
              FT_Pos  orig_y_rounded = FT_PIX_ROUND( orig_y );


              إذا ( akerning->x != orig_x_rounded ||
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

    أرجع error;
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


    إذا ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );

    إذا ( !akerning )
      أرجع FT_THROW( Invalid_Argument );

    FT_FACE_FIND_SERVICE( face, service, KERNING );
    إذا ( !service )
      أرجع FT_THROW( Unimplemented_Feature );

    error = service->get_track( face,
                                point_size,
                                degree,
                                akerning );

    أرجع error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Select_Charmap( FT_Face      face,
                     FT_Encoding  encoding )
  {
    FT_CharMap*  cur;
    FT_CharMap*  limit;


    إذا ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );

    /* FT_ENCODING_NONE is a valid encoding for BDF, PCF, and Windows FNT */
    إذا ( encoding == FT_ENCODING_NONE && !face->عدد_charmaps )
      أرجع FT_THROW( Invalid_Argument );

    /* FT_ENCODING_UNICODE is special.  We try to find the `best' Unicode */
    /* charmap available, i.e., one with UCS-4 characters, if possible.   */
    /*                                                                    */
    /* This is done by find_unicode_charmap() above, to share code.       */
    إذا ( encoding == FT_ENCODING_UNICODE )
      أرجع find_unicode_charmap( face );

    cur = face->charmaps;
    إذا ( !cur )
      أرجع FT_THROW( Invalid_CharMap_Handle );

    limit = cur + face->عدد_charmaps;

    لكل ( ; cur < limit; cur++ )
    {
      إذا ( cur[0]->encoding == encoding )
      {
        face->charmap = cur[0];
        أرجع FT_Err_Ok;
      }
    }

    أرجع FT_THROW( Invalid_Argument );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Set_Charmap( FT_Face     face,
                  FT_CharMap  charmap )
  {
    FT_CharMap*  cur;
    FT_CharMap*  limit;


    إذا ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );

    cur = face->charmaps;
    إذا ( !cur || !charmap )
      أرجع FT_THROW( Invalid_CharMap_Handle );

    limit = cur + face->عدد_charmaps;

    لكل ( ; cur < limit; cur++ )
    {
      إذا ( cur[0] == charmap )
      {
        FT_Long  format = FT_Get_CMap_Format( charmap );


        إذا ( format != 14 && format != 15 )
        {
          face->charmap = cur[0];
          أرجع FT_Err_Ok;
        }
      }
    }

    أرجع FT_THROW( Invalid_Argument );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Int )
  FT_Get_Charmap_Index( FT_CharMap  charmap )
  {
    FT_Int  i;


    إذا ( !charmap || !charmap->face )
      أرجع -1;

    لكل ( i = 0; i < charmap->face->عدد_charmaps; i++ )
      إذا ( charmap->face->charmaps[i] == charmap )
        اكسر;

    FT_ASSERT( i < charmap->face->عدد_charmaps );

    أرجع i;
  }


  ساكن فراغ
  ft_cmap_done_internal( FT_CMap  cmap )
  {
    FT_CMap_Class  clazz  = cmap->clazz;
    FT_Face        face   = cmap->charmap.face;
    FT_Memory      memory = FT_FACE_MEMORY( face );


    إذا ( clazz->done )
      clazz->done( cmap );

    FT_FREE( cmap );
  }


  FT_BASE_DEF( فراغ )
  FT_CMap_Done( FT_CMap  cmap )
  {
    إذا ( cmap )
    {
      FT_Face    face   = cmap->charmap.face;
      FT_Memory  memory = FT_FACE_MEMORY( face );
      FT_Error   error;
      FT_Int     i, j;


      لكل ( i = 0; i < face->عدد_charmaps; i++ )
      {
        إذا ( (FT_CMap)face->charmaps[i] == cmap )
        {
          FT_CharMap  last_charmap = face->charmaps[face->عدد_charmaps - 1];


          إذا ( FT_QRENEW_ARRAY( face->charmaps,
                                face->عدد_charmaps,
                                face->عدد_charmaps - 1 ) )
            أرجع;

          /* remove it from our list of charmaps */
          لكل ( j = i + 1; j < face->عدد_charmaps; j++ )
          {
            إذا ( j == face->عدد_charmaps - 1 )
              face->charmaps[j - 1] = last_charmap;
            وإلا
              face->charmaps[j - 1] = face->charmaps[j];
          }

          face->عدد_charmaps--;

          إذا ( (FT_CMap)face->charmap == cmap )
            face->charmap = عدم;

          ft_cmap_done_internal( cmap );

          اكسر;
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
    FT_CMap    cmap = عدم;


    إذا ( !clazz || !charmap || !charmap->face )
      أرجع FT_THROW( Invalid_Argument );

    face   = charmap->face;
    memory = FT_FACE_MEMORY( face );

    إذا ( !FT_ALLOC( cmap, clazz->size ) )
    {
      cmap->charmap = *charmap;
      cmap->clazz   = clazz;

      إذا ( clazz->init )
      {
        error = clazz->init( cmap, init_data );
        إذا ( error )
          اذهب Fail;
      }

      /* add it to our list of charmaps */
      إذا ( FT_QRENEW_ARRAY( face->charmaps,
                            face->عدد_charmaps,
                            face->عدد_charmaps + 1 ) )
        اذهب Fail;

      face->charmaps[face->عدد_charmaps++] = (FT_CharMap)cmap;
    }

  Exit:
    إذا ( acmap )
      *acmap = cmap;

    أرجع error;

  Fail:
    ft_cmap_done_internal( cmap );
    cmap = عدم;
    اذهب Exit;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt )
  FT_Get_Char_Index( FT_Face   face,
                     FT_ULong  charcode )
  {
    FT_UInt  result = 0;


    إذا ( face && face->charmap )
    {
      FT_CMap  cmap = FT_CMAP( face->charmap );


      إذا ( charcode > 0xFFFFFFFFUL )
      {
        FT_TRACE1(( "FT_Get_Char_Index: too large charcode" ));
        FT_TRACE1(( " 0x%lx is truncated\n", charcode ));
      }

      result = cmap->clazz->char_index( cmap, (FT_UInt32)charcode );
      إذا ( result >= (FT_UInt)face->عدد_glyphs )
        result = 0;
    }

    أرجع result;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_ULong )
  FT_Get_First_Char( FT_Face   face,
                     FT_UInt  *agindex )
  {
    FT_ULong  result = 0;
    FT_UInt   gindex = 0;


    /* only do something if we have a charmap, and we have glyphs at all */
    إذا ( face && face->charmap && face->عدد_glyphs )
    {
      gindex = FT_Get_Char_Index( face, 0 );
      إذا ( gindex == 0 )
        result = FT_Get_Next_Char( face, 0, &gindex );
    }

    إذا ( agindex )
      *agindex = gindex;

    أرجع result;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_ULong )
  FT_Get_Next_Char( FT_Face   face,
                    FT_ULong  charcode,
                    FT_UInt  *agindex )
  {
    FT_ULong  result = 0;
    FT_UInt   gindex = 0;


    إذا ( face && face->charmap && face->عدد_glyphs )
    {
      FT_UInt32  code = (FT_UInt32)charcode;
      FT_CMap    cmap = FT_CMAP( face->charmap );


      افعل
      {
        gindex = cmap->clazz->char_next( cmap, &code );

      } طالما ( gindex >= (FT_UInt)face->عدد_glyphs );

      result = ( gindex == 0 ) ? 0 : code;
    }

    إذا ( agindex )
      *agindex = gindex;

    أرجع result;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Face_Properties( FT_Face        face,
                      FT_UInt        عدد_properties,
                      FT_Parameter*  properties )
  {
    FT_Error  error = FT_Err_Ok;


    إذا ( عدد_properties > 0 && !properties )
    {
      error = FT_THROW( Invalid_Argument );
      اذهب Exit;
    }

    لكل ( ; عدد_properties > 0; عدد_properties-- )
    {
      إذا ( properties->tag == FT_PARAM_TAG_STEM_DARKENING )
      {
        إذا ( properties->data )
        {
          إذا ( *( (FT_Bool*)properties->data ) == TRUE )
            face->internal->no_stem_darkening = FALSE;
          وإلا
            face->internal->no_stem_darkening = TRUE;
        }
        وإلا
        {
          /* use module default */
          face->internal->no_stem_darkening = -1;
        }
      }
      وإلا إذا ( properties->tag == FT_PARAM_TAG_LCD_FILTER_WEIGHTS )
      {
        error = FT_THROW( Unimplemented_Feature );
        اذهب Exit;
      }
      وإلا إذا ( properties->tag == FT_PARAM_TAG_RANDOM_SEED )
      {
        إذا ( properties->data )
        {
          face->internal->random_seed = *( (FT_Int32*)properties->data );
          إذا ( face->internal->random_seed < 0 )
            face->internal->random_seed = 0;
        }
        وإلا
        {
          /* use module default */
          face->internal->random_seed = -1;
        }
      }
      وإلا
      {
        error = FT_THROW( Invalid_Argument );
        اذهب Exit;
      }

      إذا ( error )
        اكسر;

      properties++;
    }

  Exit:
    أرجع error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt )
  FT_Face_GetCharVariantIndex( FT_Face   face,
                               FT_ULong  charcode,
                               FT_ULong  variantSelector )
  {
    FT_UInt  result = 0;


    إذا ( face                                           &&
         face->charmap                                  &&
         face->charmap->encoding == FT_ENCODING_UNICODE )
    {
      FT_CharMap  charmap = find_variant_selector_charmap( face );
      FT_CMap     ucmap = FT_CMAP( face->charmap );


      إذا ( charmap )
      {
        FT_CMap  vcmap = FT_CMAP( charmap );


        إذا ( charcode > 0xFFFFFFFFUL )
        {
          FT_TRACE1(( "FT_Face_GetCharVariantIndex:"
                      " too large charcode" ));
          FT_TRACE1(( " 0x%lx is truncated\n", charcode ));
        }
        إذا ( variantSelector > 0xFFFFFFFFUL )
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

    أرجع result;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Int )
  FT_Face_GetCharVariantIsDefault( FT_Face   face,
                                   FT_ULong  charcode,
                                   FT_ULong  variantSelector )
  {
    FT_Int  result = -1;


    إذا ( face )
    {
      FT_CharMap  charmap = find_variant_selector_charmap( face );


      إذا ( charmap )
      {
        FT_CMap  vcmap = FT_CMAP( charmap );


        إذا ( charcode > 0xFFFFFFFFUL )
        {
          FT_TRACE1(( "FT_Face_GetCharVariantIsDefault:"
                      " too large charcode" ));
          FT_TRACE1(( " 0x%lx is truncated\n", charcode ));
        }
        إذا ( variantSelector > 0xFFFFFFFFUL )
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

    أرجع result;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt32* )
  FT_Face_GetVariantSelectors( FT_Face  face )
  {
    FT_UInt32  *result = عدم;


    إذا ( face )
    {
      FT_CharMap  charmap = find_variant_selector_charmap( face );


      إذا ( charmap )
      {
        FT_CMap    vcmap  = FT_CMAP( charmap );
        FT_Memory  memory = FT_FACE_MEMORY( face );


        result = vcmap->clazz->variant_list( vcmap, memory );
      }
    }

    أرجع result;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt32* )
  FT_Face_GetVariantsOfChar( FT_Face   face,
                             FT_ULong  charcode )
  {
    FT_UInt32  *result = عدم;


    إذا ( face )
    {
      FT_CharMap  charmap = find_variant_selector_charmap( face );


      إذا ( charmap )
      {
        FT_CMap    vcmap  = FT_CMAP( charmap );
        FT_Memory  memory = FT_FACE_MEMORY( face );


        إذا ( charcode > 0xFFFFFFFFUL )
        {
          FT_TRACE1(( "FT_Face_GetVariantsOfChar: too large charcode" ));
          FT_TRACE1(( " 0x%lx is truncated\n", charcode ));
        }

        result = vcmap->clazz->charvariant_list( vcmap, memory,
                                                 (FT_UInt32)charcode );
      }
    }
    أرجع result;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt32* )
  FT_Face_GetCharsOfVariant( FT_Face   face,
                             FT_ULong  variantSelector )
  {
    FT_UInt32  *result = عدم;


    إذا ( face )
    {
      FT_CharMap  charmap = find_variant_selector_charmap( face );


      إذا ( charmap )
      {
        FT_CMap    vcmap  = FT_CMAP( charmap );
        FT_Memory  memory = FT_FACE_MEMORY( face );


        إذا ( variantSelector > 0xFFFFFFFFUL )
        {
          FT_TRACE1(( "FT_Get_Char_Index: too large variantSelector" ));
          FT_TRACE1(( " 0x%lx is truncated\n", variantSelector ));
        }

        result = vcmap->clazz->variantchar_list( vcmap, memory,
                                                 (FT_UInt32)variantSelector );
      }
    }

    أرجع result;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt )
  FT_Get_Name_Index( FT_Face           face,
                     ثابت FT_String*  glyph_name )
  {
    FT_UInt  result = 0;


    إذا ( face                       &&
         FT_HAS_GLYPH_NAMES( face ) &&
         glyph_name                 )
    {
      FT_Service_GlyphDict  service;


      FT_FACE_LOOKUP_SERVICE( face,
                              service,
                              GLYPH_DICT );

      إذا ( service && service->name_index )
        result = service->name_index( face, glyph_name );
    }

    أرجع result;
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


    إذا ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );

    إذا ( !buffer || buffer_max == 0 )
      أرجع FT_THROW( Invalid_Argument );

    /* clean up buffer */
    ((FT_Byte*)buffer)[0] = '\0';

    إذا ( (FT_Long)glyph_index >= face->عدد_glyphs )
      أرجع FT_THROW( Invalid_Glyph_Index );

    إذا ( !FT_HAS_GLYPH_NAMES( face ) )
      أرجع FT_THROW( Invalid_Argument );

    FT_FACE_LOOKUP_SERVICE( face, service, GLYPH_DICT );
    إذا ( service && service->get_name )
      error = service->get_name( face, glyph_index, buffer, buffer_max );
    وإلا
      error = FT_THROW( Invalid_Argument );

    أرجع error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( ثابت حرف* )
  FT_Get_Postscript_Name( FT_Face  face )
  {
    ثابت حرف*  result = عدم;


    إذا ( !face )
      اذهب Exit;

    إذا ( !result )
    {
      FT_Service_PsFontName  service;


      FT_FACE_LOOKUP_SERVICE( face,
                              service,
                              POSTSCRIPT_FONT_NAME );

      إذا ( service && service->get_ps_font_name )
        result = service->get_ps_font_name( face );
    }

  Exit:
    أرجع result;
  }


  /* documentation is in tttables.h */

  FT_EXPORT_DEF( فراغ* )
  FT_Get_Sfnt_Table( FT_Face      face,
                     FT_Sfnt_Tag  tag )
  {
    فراغ*                  table = عدم;
    FT_Service_SFNT_Table  service;


    إذا ( face && FT_IS_SFNT( face ) )
    {
      FT_FACE_FIND_SERVICE( face, service, SFNT_TABLE );
      إذا ( service )
        table = service->get_table( face, tag );
    }

    أرجع table;
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


    إذا ( !face || !FT_IS_SFNT( face ) )
      أرجع FT_THROW( Invalid_Face_Handle );

    FT_FACE_FIND_SERVICE( face, service, SFNT_TABLE );
    إذا ( !service )
      أرجع FT_THROW( Unimplemented_Feature );

    أرجع service->load_table( face, tag, offset, buffer, length );
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

    إذا ( !face || !FT_IS_SFNT( face ) )
      أرجع FT_THROW( Invalid_Face_Handle );

    FT_FACE_FIND_SERVICE( face, service, SFNT_TABLE );
    إذا ( !service )
      أرجع FT_THROW( Unimplemented_Feature );

    أرجع service->table_info( face, table_index, tag, &offset, length );
  }


  /* documentation is in tttables.h */

  FT_EXPORT_DEF( FT_ULong )
  FT_Get_CMap_Language_ID( FT_CharMap  charmap )
  {
    FT_Service_TTCMaps  service;
    FT_Face             face;
    TT_CMapInfo         cmap_info;


    إذا ( !charmap || !charmap->face )
      أرجع 0;

    face = charmap->face;
    FT_FACE_FIND_SERVICE( face, service, TT_CMAP );
    إذا ( !service )
      أرجع 0;
    إذا ( service->get_cmap_info( charmap, &cmap_info ))
      أرجع 0;

    أرجع cmap_info.language;
  }


  /* documentation is in tttables.h */

  FT_EXPORT_DEF( FT_Long )
  FT_Get_CMap_Format( FT_CharMap  charmap )
  {
    FT_Service_TTCMaps  service;
    FT_Face             face;
    TT_CMapInfo         cmap_info;


    إذا ( !charmap || !charmap->face )
      أرجع -1;

    face = charmap->face;
    FT_FACE_FIND_SERVICE( face, service, TT_CMAP );
    إذا ( !service )
      أرجع -1;
    إذا ( service->get_cmap_info( charmap, &cmap_info ))
      أرجع -1;

    أرجع cmap_info.format;
  }


  /* documentation is in ftsizes.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Activate_Size( FT_Size  size )
  {
    FT_Face  face;


    إذا ( !size )
      أرجع FT_THROW( Invalid_Size_Handle );

    face = size->face;
    إذا ( !face || !face->driver )
      أرجع FT_THROW( Invalid_Face_Handle );

    /* we don't need anything more complex than that; all size objects */
    /* are already listed by the face                                  */
    face->size = size;

    أرجع FT_Err_Ok;
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
                      FT_List_گرہ*     گرہ )
  {
    FT_List_گرہ  cur;
    FT_Renderer  result = عدم;


    إذا ( !library )
      اذهب Exit;

    cur = library->renderers.head;

    إذا ( گرہ )
    {
      إذا ( *گرہ )
        cur = (*گرہ)->next;
      *گرہ = عدم;
    }

    طالما ( cur )
    {
      FT_Renderer  renderer = FT_RENDERER( cur->data );


      إذا ( renderer->glyph_format == format )
      {
        إذا ( گرہ )
          *گرہ = cur;

        result = renderer;
        اكسر;
      }
      cur = cur->next;
    }

  Exit:
    أرجع result;
  }


  ساكن FT_Renderer
  ft_lookup_glyph_renderer( FT_GlyphSlot  slot )
  {
    FT_Face      face    = slot->face;
    FT_Library   library = FT_FACE_LIBRARY( face );
    FT_Renderer  result  = library->cur_renderer;


    إذا ( !result || result->glyph_format != slot->format )
      result = FT_Lookup_Renderer( library, slot->format, 0 );

    أرجع result;
  }


  ساكن فراغ
  ft_set_current_renderer( FT_Library  library )
  {
    FT_Renderer  renderer;


    renderer = FT_Lookup_Renderer( library, FT_GLYPH_FORMAT_OUTLINE, 0 );
    library->cur_renderer = renderer;
  }


  ساكن FT_Error
  ft_add_renderer( FT_Module  module )
  {
    FT_Library   library = module->library;
    FT_Memory    memory  = library->memory;
    FT_Error     error;
    FT_List_گرہ  گرہ    = عدم;


    إذا ( FT_QNEW( گرہ ) )
      اذهب Exit;

    {
      FT_Renderer         render = FT_RENDERER( module );
      FT_Renderer_Class*  clazz  = (FT_Renderer_Class*)module->clazz;


      render->clazz        = clazz;
      render->glyph_format = clazz->glyph_format;

      /* allocate raster object if needed */
      إذا ( clazz->raster_class && clazz->raster_class->raster_new )
      {
        error = clazz->raster_class->raster_new( memory, &render->raster );
        إذا ( error )
          اذهب Fail;

        render->raster_render = clazz->raster_class->raster_render;
        render->render        = clazz->render_glyph;
      }

#ifdef FT_CONFIG_OPTION_SVG
      إذا ( clazz->glyph_format == FT_GLYPH_FORMAT_SVG )
        render->render = clazz->render_glyph;
#endif

      /* add to list */
      گرہ->data = module;
      FT_List_Add( &library->renderers, گرہ );

      ft_set_current_renderer( library );
    }

  Fail:
    إذا ( error )
      FT_FREE( گرہ );

  Exit:
    أرجع error;
  }


  ساكن فراغ
  ft_remove_renderer( FT_Module  module )
  {
    FT_Library   library;
    FT_Memory    memory;
    FT_List_گرہ  گرہ;


    library = module->library;
    إذا ( !library )
      أرجع;

    memory = library->memory;

    گرہ = FT_List_Find( &library->renderers, module );
    إذا ( گرہ )
    {
      FT_Renderer  render = FT_RENDERER( module );


      /* release raster object, if any */
      إذا ( render->raster )
        render->clazz->raster_class->raster_done( render->raster );

      /* remove from list */
      FT_List_Remove( &library->renderers, گرہ );
      FT_FREE( گرہ );

      ft_set_current_renderer( library );
    }
  }


  /* documentation is in ftrender.h */

  FT_EXPORT_DEF( FT_Renderer )
  FT_Get_Renderer( FT_Library       library,
                   FT_Glyph_Format  format )
  {
    /* test for valid `library' delayed to `FT_Lookup_Renderer' */

    أرجع FT_Lookup_Renderer( library, format, 0 );
  }


  /* documentation is in ftrender.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Set_Renderer( FT_Library     library,
                   FT_Renderer    renderer,
                   FT_UInt        عدد_params,
                   FT_Parameter*  parameters )
  {
    FT_List_گرہ  گرہ;
    FT_Error     error = FT_Err_Ok;

    FT_Renderer_SetModeFunc  set_mode;


    إذا ( !library )
    {
      error = FT_THROW( Invalid_Library_Handle );
      اذهب Exit;
    }

    إذا ( !renderer )
    {
      error = FT_THROW( Invalid_Argument );
      اذهب Exit;
    }

    إذا ( عدد_params > 0 && !parameters )
    {
      error = FT_THROW( Invalid_Argument );
      اذهب Exit;
    }

    گرہ = FT_List_Find( &library->renderers, renderer );
    إذا ( !گرہ )
    {
      error = FT_THROW( Invalid_Argument );
      اذهب Exit;
    }

    FT_List_Up( &library->renderers, گرہ );

    إذا ( renderer->glyph_format == FT_GLYPH_FORMAT_OUTLINE )
      library->cur_renderer = renderer;

    set_mode = renderer->clazz->set_mode;

    لكل ( ; عدد_params > 0; عدد_params-- )
    {
      error = set_mode( renderer, parameters->tag, parameters->data );
      إذا ( error )
        اكسر;
      parameters++;
    }

  Exit:
    أرجع error;
  }


  FT_BASE_DEF( FT_Error )
  FT_Render_Glyph_Internal( FT_Library      library,
                            FT_GlyphSlot    slot,
                            FT_Render_Mode  render_mode )
  {
    FT_Error     error = FT_Err_Ok;
    FT_Face      face  = slot->face;
    FT_Renderer  renderer;
    FT_List_گرہ  گرہ  = عدم;


    /* try to render colored glyph layers as a special case */
    إذا ( slot->internal->load_flags & FT_LOAD_COLOR &&
         slot->format == FT_GLYPH_FORMAT_OUTLINE    )
    {
      FT_LayerIterator  iterator;

      FT_UInt  base_glyph = slot->glyph_index;

      FT_Bool  have_layers;
      FT_UInt  glyph_index;
      FT_UInt  color_index;


      iterator.p  = عدم;
      have_layers = FT_Get_Color_Glyph_Layer( face,
                                              base_glyph,
                                              &glyph_index,
                                              &color_index,
                                              &iterator );
      إذا ( have_layers )
      {
        error = FT_New_GlyphSlot( face, عدم );
        إذا ( !error )
        {
          TT_Face       ttface = (TT_Face)face;
          SFNT_Service  sfnt   = (SFNT_Service)ttface->sfnt;


          افعل
          {
            FT_Int32  load_flags = slot->internal->load_flags;


            /* disable the `FT_LOAD_COLOR' flag to avoid recursion */
            /* right here in this function                         */
            load_flags &= ~FT_LOAD_COLOR;

            /* render into the new `face->glyph' glyph slot */
            load_flags |= FT_LOAD_RENDER | FT_LOAD_NO_BITMAP;

            error = FT_Load_Glyph( face, glyph_index, load_flags );
            إذا ( error )
              اكسر;

            /* blend new `face->glyph' into old `slot'; */
            /* at the first call, `slot' is still empty */
            error = sfnt->colr_blend( ttface,
                                      color_index,
                                      slot,
                                      face->glyph );
            إذا ( error )
              اكسر;

          } طالما ( FT_Get_Color_Glyph_Layer( face,
                                              base_glyph,
                                              &glyph_index,
                                              &color_index,
                                              &iterator ) );

          إذا ( !error )
            slot->format = FT_GLYPH_FORMAT_BITMAP;

          /* this call also restores `slot' as the glyph slot */
          FT_Done_GlyphSlot( face->glyph );
        }

        إذا ( !error )
          أرجع error;

        /* Failed to do the colored layer.  Draw outline instead. */
        slot->format = FT_GLYPH_FORMAT_OUTLINE;
      }
    }

    /* small shortcut for the very common case */
    إذا ( slot->format == FT_GLYPH_FORMAT_OUTLINE )
    {
      renderer = library->cur_renderer;
      گرہ     = library->renderers.head;
    }
    وإلا
      renderer = FT_Lookup_Renderer( library, slot->format, &گرہ );

    error = FT_ERR( Cannot_Render_Glyph );
    طالما ( renderer )
    {
      error = renderer->render( renderer, slot, render_mode, عدم );
      إذا ( !error                                   ||
           FT_ERR_NEQ( error, Cannot_Render_Glyph ) )
        اكسر;

      /* `FT_Err_Cannot_Render_Glyph` is returned if the render mode */
      /* is unsupported by the current renderer for this glyph image */
      /* format.                                                     */

      /* now, look for another renderer that supports the same */
      /* format.                                               */
      renderer = FT_Lookup_Renderer( library, slot->format, &گرہ );
    }

    /* it is not an error if we cannot render a bitmap glyph */
    إذا ( FT_ERR_EQ( error, Cannot_Render_Glyph ) &&
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
    إذا ( !error                               &&
         ft_trace_levels[trace_checksum] >= 3 &&
         slot->bitmap.buffer                  )
    {
      FT_Bitmap  bitmap;
      FT_Error   err;


      FT_Bitmap_Init( &bitmap );

      /* we convert to a single bitmap format for computing the checksum */
      /* this also converts the bitmap flow to `down' (i.e., pitch > 0)  */
      err = FT_Bitmap_Convert( library, &slot->bitmap, &bitmap, 1 );
      إذا ( !err )
      {
        MD5_CTX        ctx;
        unsigned حرف  md5[16];
        unsigned long  coverage = 0;
        صحيح            i, j;
        صحيح            rows  = (صحيح)bitmap.rows;
        صحيح            pitch = bitmap.pitch;


        FT_TRACE3(( "FT_Render_Glyph: bitmap %dx%d, %s (mode %d)\n",
                    pitch,
                    rows,
                    pixel_modes[slot->bitmap.pixel_mode],
                    slot->bitmap.pixel_mode ));

        لكل ( i = 0; i < rows; i++ )
          لكل ( j = 0; j < pitch; j++ )
            coverage += bitmap.buffer[i * pitch + j];

        FT_TRACE3(( "  Total coverage: %lu\n", coverage ));

        MD5_Init( &ctx );
        إذا ( bitmap.buffer )
          MD5_Update( &ctx, bitmap.buffer,
                      (unsigned long)rows * (unsigned long)pitch );
        MD5_Final( md5, &ctx );

        FT_TRACE3(( "  MD5 checksum: " ));
        لكل ( i = 0; i < 16; i++ )
          FT_TRACE3(( "%02X", md5[i] ));
        FT_TRACE3(( "\n" ));
      }

      FT_Bitmap_Done( library, &bitmap );
    }

    /*
     * Dump bitmap in Netpbm format (PBM or PGM).
     */

    /* we use FT_TRACE7 in this block */
    إذا ( !error                               &&
         ft_trace_levels[trace_checksum] >= 7 &&
         slot->bitmap.buffer                  )
    {
      إذا ( slot->bitmap.rows  < 128U &&
           slot->bitmap.width < 128U )
      {
        صحيح  rows  = (صحيح)slot->bitmap.rows;
        صحيح  width = (صحيح)slot->bitmap.width;
        صحيح  pitch =      slot->bitmap.pitch;
        صحيح  i, j, m;

        unsigned حرف*  topleft = slot->bitmap.buffer;


        إذا ( pitch < 0 )
          topleft -= pitch * ( rows - 1 );

        FT_TRACE7(( "Netpbm image: start\n" ));
        بدل ( slot->bitmap.pixel_mode )
        {
        حالة FT_PIXEL_MODE_MONO:
          FT_TRACE7(( "P1 %d %d\n", width, rows ));
          لكل ( i = 0; i < rows; i++ )
          {
            لكل ( j = 0; j < width; )
              لكل ( m = 128; m > 0 && j < width; m >>= 1, j++ )
                FT_TRACE7(( " %d",
                            ( topleft[i * pitch + j / 8] & m ) != 0 ));
            FT_TRACE7(( "\n" ));
          }
          اكسر;

        افتراضي:
          FT_TRACE7(( "P2 %d %d 255\n", width, rows ));
          لكل ( i = 0; i < rows; i++ )
          {
            لكل ( j = 0; j < width; j += 1 )
              FT_TRACE7(( " %3u", topleft[i * pitch + j] ));
            FT_TRACE7(( "\n" ));
          }
        }
        FT_TRACE7(( "Netpbm image: end\n" ));
      }
      وإلا
        FT_TRACE7(( "Netpbm image: too large, omitted\n" ));
    }

#undef  FT_COMPONENT
#define FT_COMPONENT  objs

#endif /* FT_DEBUG_LEVEL_TRACE */

    أرجع error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Render_Glyph( FT_GlyphSlot    slot,
                   FT_Render_Mode  render_mode )
  {
    FT_Library  library;


    إذا ( !slot || !slot->face )
      أرجع FT_THROW( Invalid_Argument );

    library = FT_FACE_LIBRARY( slot->face );

    أرجع FT_Render_Glyph_Internal( library, slot, render_mode );
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
  ساكن فراغ
  Destroy_Module( FT_Module  module )
  {
    ثابت FT_Module_Class*  clazz   = module->clazz;
    FT_Library              library = module->library;
    FT_Memory               memory  = module->memory;


    إذا ( library && library->auto_hinter == module )
      library->auto_hinter = عدم;

    /* if the module is a renderer */
    إذا ( FT_MODULE_IS_RENDERER( module ) )
      ft_remove_renderer( module );

    /* if the module is a font driver, add some steps */
    إذا ( FT_MODULE_IS_DRIVER( module ) )
      Destroy_Driver( FT_DRIVER( module ) );

    /* finalize the module object */
    إذا ( clazz->module_done )
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
    FT_Module  module = عدم;
    FT_UInt    nn;


#define FREETYPE_VER_FIXED  ( ( (FT_Long)FREETYPE_MAJOR << 16 ) | \
                                FREETYPE_MINOR                  )

    إذا ( !library )
      أرجع FT_THROW( Invalid_Library_Handle );

    إذا ( !clazz )
      أرجع FT_THROW( Invalid_Argument );

    /* check FreeType version */
    إذا ( clazz->module_requires > FREETYPE_VER_FIXED )
      أرجع FT_THROW( Invalid_Version );

    /* look for a module with the same name in the library's table */
    لكل ( nn = 0; nn < library->عدد_modules; nn++ )
    {
      module = library->modules[nn];
      إذا ( ft_strcmp( module->clazz->module_name, clazz->module_name ) == 0 )
      {
        /* this installed module has the same name, compare their versions */
        إذا ( clazz->module_version <= module->clazz->module_version )
          أرجع FT_THROW( Lower_Module_Version );

        /* remove the module from our list, then exit the loop to replace */
        /* it by our new version..                                        */
        FT_Remove_Module( library, module );
        اكسر;
      }
    }

    memory = library->memory;
    error  = FT_Err_Ok;

    إذا ( library->عدد_modules >= FT_MAX_MODULES )
    {
      error = FT_THROW( Too_Many_Drivers );
      اذهب Exit;
    }

    /* allocate module object */
    إذا ( FT_ALLOC( module, clazz->module_size ) )
      اذهب Exit;

    /* base initialization */
    module->clazz   = clazz;
    module->library = library;
    module->memory  = memory;

    /* check whether the module is a renderer - this must be performed */
    /* before the normal module initialization                         */
    إذا ( FT_MODULE_IS_RENDERER( module ) )
    {
      /* add to the renderers list */
      error = ft_add_renderer( module );
      إذا ( error )
        اذهب Fail;
    }

    /* is the module a auto-hinter? */
    إذا ( FT_MODULE_IS_HINTER( module ) )
      library->auto_hinter = module;

    /* if the module is a font driver */
    إذا ( FT_MODULE_IS_DRIVER( module ) )
    {
      FT_Driver  driver = FT_DRIVER( module );


      driver->clazz = (FT_Driver_Class)module->clazz;
    }

    إذا ( clazz->module_init )
    {
      error = clazz->module_init( module );
      إذا ( error )
        اذهب Fail;
    }

    /* add module to the library's table */
    library->modules[library->عدد_modules++] = module;

  Exit:
    أرجع error;

  Fail:
    إذا ( FT_MODULE_IS_RENDERER( module ) )
    {
      FT_Renderer  renderer = FT_RENDERER( module );


      إذا ( renderer->clazz                                          &&
           renderer->clazz->glyph_format == FT_GLYPH_FORMAT_OUTLINE &&
           renderer->raster                                         )
        renderer->clazz->raster_class->raster_done( renderer->raster );
    }

    FT_FREE( module );
    اذهب Exit;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Module )
  FT_Get_Module( FT_Library   library,
                 ثابت حرف*  module_name )
  {
    FT_Module   result = عدم;
    FT_Module*  cur;
    FT_Module*  limit;


    إذا ( !library || !module_name )
      أرجع result;

    cur   = library->modules;
    limit = cur + library->عدد_modules;

    لكل ( ; cur < limit; cur++ )
      إذا ( ft_strcmp( cur[0]->clazz->module_name, module_name ) == 0 )
      {
        result = cur[0];
        اكسر;
      }

    أرجع result;
  }


  /* documentation is in ftobjs.h */

  FT_BASE_DEF( ثابت فراغ* )
  FT_Get_Module_Interface( FT_Library   library,
                           ثابت حرف*  mod_name )
  {
    FT_Module  module;


    /* test for valid `library' delayed to FT_Get_Module() */

    module = FT_Get_Module( library, mod_name );

    أرجع module ? module->clazz->module_interface : 0;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_module_get_service( FT_Module    module,
                         ثابت حرف*  service_id,
                         FT_Bool      global )
  {
    FT_Pointer  result = عدم;


    إذا ( module )
    {
      FT_ASSERT( module->clazz && module->clazz->get_interface );

      /* first, look for the service in the module */
      إذا ( module->clazz->get_interface )
        result = module->clazz->get_interface( module, service_id );

      إذا ( global && !result )
      {
        /* we didn't find it, look in all other modules then */
        FT_Library  library = module->library;
        FT_Module*  cur     = library->modules;
        FT_Module*  limit   = cur + library->عدد_modules;


        لكل ( ; cur < limit; cur++ )
        {
          إذا ( cur[0] != module )
          {
            FT_ASSERT( cur[0]->clazz );

            إذا ( cur[0]->clazz->get_interface )
            {
              result = cur[0]->clazz->get_interface( cur[0], service_id );
              إذا ( result )
                اكسر;
            }
          }
        }
      }
    }

    أرجع result;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Remove_Module( FT_Library  library,
                    FT_Module   module )
  {
    /* try to find the module from the table, then remove it from there */

    إذا ( !library )
      أرجع FT_THROW( Invalid_Library_Handle );

    إذا ( module )
    {
      FT_Module*  cur   = library->modules;
      FT_Module*  limit = cur + library->عدد_modules;


      لكل ( ; cur < limit; cur++ )
      {
        إذا ( cur[0] == module )
        {
          /* remove it from the table */
          library->عدد_modules--;
          limit--;
          طالما ( cur < limit )
          {
            cur[0] = cur[1];
            cur++;
          }
          limit[0] = عدم;

          /* destroy the module */
          Destroy_Module( module );

          أرجع FT_Err_Ok;
        }
      }
    }
    أرجع FT_THROW( Invalid_Driver_Handle );
  }


  ساكن FT_Error
  ft_property_do( FT_Library        library,
                  ثابت FT_String*  module_name,
                  ثابت FT_String*  property_name,
                  فراغ*             قدر,
                  FT_Bool           set,
                  FT_Bool           قدر_is_string )
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


    إذا ( !library )
      أرجع FT_THROW( Invalid_Library_Handle );

    إذا ( !module_name || !property_name || !قدر )
      أرجع FT_THROW( Invalid_Argument );

    cur   = library->modules;
    limit = cur + library->عدد_modules;

    /* search module */
    لكل ( ; cur < limit; cur++ )
      إذا ( !ft_strcmp( cur[0]->clazz->module_name, module_name ) )
        اكسر;

    إذا ( cur == limit )
    {
      FT_TRACE2(( "%s: can't find module `%s'\n",
                  func_name, module_name ));
      أرجع FT_THROW( Missing_Module );
    }

    /* check whether we have a service interface */
    إذا ( !cur[0]->clazz->get_interface )
    {
      FT_TRACE2(( "%s: module `%s' doesn't support properties\n",
                  func_name, module_name ));
      أرجع FT_THROW( Unimplemented_Feature );
    }

    /* search property service */
    interface = cur[0]->clazz->get_interface( cur[0],
                                              FT_SERVICE_ID_PROPERTIES );
    إذا ( !interface )
    {
      FT_TRACE2(( "%s: module `%s' doesn't support properties\n",
                  func_name, module_name ));
      أرجع FT_THROW( Unimplemented_Feature );
    }

    service = (FT_Service_Properties)interface;

    إذا ( set )
      missing_func = FT_BOOL( !service->set_property );
    وإلا
      missing_func = FT_BOOL( !service->get_property );

    إذا ( missing_func )
    {
      FT_TRACE2(( "%s: property service of module `%s' is broken\n",
                  func_name, module_name ));
      أرجع FT_THROW( Unimplemented_Feature );
    }

    أرجع set ? service->set_property( cur[0],
                                        property_name,
                                        قدر,
                                        قدر_is_string )
               : service->get_property( cur[0],
                                        property_name,
                                        قدر );
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Property_Set( FT_Library        library,
                   ثابت FT_String*  module_name,
                   ثابت FT_String*  property_name,
                   ثابت فراغ*       قدر )
  {
    أرجع ft_property_do( library,
                           module_name,
                           property_name,
                           (فراغ*)قدر,
                           TRUE,
                           FALSE );
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Property_Get( FT_Library        library,
                   ثابت FT_String*  module_name,
                   ثابت FT_String*  property_name,
                   فراغ*             قدر )
  {
    أرجع ft_property_do( library,
                           module_name,
                           property_name,
                           قدر,
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
                          FT_String*        قدر )
  {
    أرجع ft_property_do( library,
                           module_name,
                           property_name,
                           (فراغ*)قدر,
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
    إذا ( !library )
      أرجع FT_THROW( Invalid_Library_Handle );

    library->refcount++;

    أرجع FT_Err_Ok;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_New_Library( FT_Memory    memory,
                  FT_Library  *alibrary )
  {
    FT_Library  library = عدم;
    FT_Error    error;


    إذا ( !memory || !alibrary )
      أرجع FT_THROW( Invalid_Argument );

#ifndef FT_DEBUG_LOGGING
#ifdef FT_DEBUG_LEVEL_ERROR
    /* init debugging support */
    ft_debug_init();
#endif /* FT_DEBUG_LEVEL_ERROR */
#endif /* !FT_DEBUG_LOGGING */

    /* first of all, allocate the library object */
    إذا ( FT_NEW( library ) )
      أرجع error;

    library->memory = memory;

    library->version_major = FREETYPE_MAJOR;
    library->version_minor = FREETYPE_MINOR;
    library->version_patch = FREETYPE_PATCH;

    library->refcount = 1;

    /* That's ok now */
    *alibrary = library;

    أرجع FT_Err_Ok;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( فراغ )
  FT_Library_Version( FT_Library   library,
                      FT_Int      *amajor,
                      FT_Int      *aminor,
                      FT_Int      *apatch )
  {
    FT_Int  major = 0;
    FT_Int  minor = 0;
    FT_Int  patch = 0;


    إذا ( library )
    {
      major = library->version_major;
      minor = library->version_minor;
      patch = library->version_patch;
    }

    إذا ( amajor )
      *amajor = major;

    إذا ( aminor )
      *aminor = minor;

    إذا ( apatch )
      *apatch = patch;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Done_Library( FT_Library  library )
  {
    FT_Memory  memory;


    إذا ( !library )
      أرجع FT_THROW( Invalid_Library_Handle );

    library->refcount--;
    إذا ( library->refcount > 0 )
      اذهب Exit;

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
      ثابت حرف*  driver_name[] = { "type42", عدم };


      لكل ( m = 0;
            m < حجم ( driver_name ) / حجم ( driver_name[0] );
            m++ )
      {
        لكل ( n = 0; n < library->عدد_modules; n++ )
        {
          FT_Module    module      = library->modules[n];
          ثابت حرف*  module_name = module->clazz->module_name;
          FT_List      faces;


          إذا ( driver_name[m]                                &&
               ft_strcmp( module_name, driver_name[m] ) != 0 )
            استمر;

          إذا ( ( module->clazz->module_flags & FT_MODULE_FONT_DRIVER ) == 0 )
            استمر;

          FT_TRACE7(( "FT_Done_Library: close faces for %s\n", module_name ));

          faces = &FT_DRIVER( module )->faces_list;
          طالما ( faces->head )
          {
            FT_Done_Face( FT_FACE( faces->head->data ) );
            إذا ( faces->head )
              FT_TRACE0(( "FT_Done_Library: failed to free some faces\n" ));
          }
        }
      }
    }

    /* Close all other modules in the library */
#إذا 1
    /* XXX Modules are removed in the reversed order so that  */
    /* type42 module is removed before truetype module.  This */
    /* avoids double free in some occasions.  It is a hack.   */
    طالما ( library->عدد_modules > 0 )
      FT_Remove_Module( library,
                        library->modules[library->عدد_modules - 1] );
#وإلا
    {
      FT_UInt  n;


      لكل ( n = 0; n < library->عدد_modules; n++ )
      {
        FT_Module  module = library->modules[n];


        إذا ( module )
        {
          Destroy_Module( module );
          library->modules[n] = عدم;
        }
      }
    }
#endif

    FT_FREE( library );

  Exit:
    أرجع FT_Err_Ok;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( فراغ )
  FT_Set_Debug_Hook( FT_Library         library,
                     FT_UInt            hook_index,
                     FT_DebugHook_Func  debug_hook )
  {
    إذا ( library && debug_hook &&
         hook_index <
           ( حجم ( library->debug_hooks ) / حجم ( فراغ* ) ) )
      library->debug_hooks[hook_index] = debug_hook;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_TrueTypeEngineType )
  FT_Get_TrueType_Engine_Type( FT_Library  library )
  {
    FT_TrueTypeEngineType  result = FT_TRUETYPE_ENGINE_TYPE_NONE;


    إذا ( library )
    {
      FT_Module  module = FT_Get_Module( library, "truetype" );


      إذا ( module )
      {
        FT_Service_TrueTypeEngine  service;


        service = (FT_Service_TrueTypeEngine)
                    ft_module_get_service( module,
                                           FT_SERVICE_ID_TRUETYPE_ENGINE,
                                           0 );
        إذا ( service )
          result = service->engine_type;
      }
    }

    أرجع result;
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


    إذا ( glyph                                      &&
         glyph->subglyphs                           &&
         glyph->format == FT_GLYPH_FORMAT_COMPOSITE &&
         sub_index < glyph->عدد_subglyphs           )
    {
      FT_SubGlyph  subg = glyph->subglyphs + sub_index;


      *p_index     = subg->index;
      *p_flags     = subg->flags;
      *p_arg1      = subg->arg1;
      *p_arg2      = subg->arg2;
      *p_transform = subg->transform;

      error = FT_Err_Ok;
    }

    أرجع error;
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


    إذا ( !face                                   ||
         !aglyph_index                           ||
         !acolor_index                           ||
         !iterator                               ||
         base_glyph >= (FT_UInt)face->عدد_glyphs )
      أرجع 0;

    إذا ( !FT_IS_SFNT( face ) )
      أرجع 0;

    ttface = (TT_Face)face;
    sfnt   = (SFNT_Service)ttface->sfnt;

    إذا ( sfnt->get_colr_layer )
      أرجع sfnt->get_colr_layer( ttface,
                                   base_glyph,
                                   aglyph_index,
                                   acolor_index,
                                   iterator );
    وإلا
      أرجع 0;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Get_Color_Glyph_Paint( FT_Face                  face,
                            FT_UInt                  base_glyph,
                            FT_Color_جڑ_Transform  جڑ_transform,
                            FT_OpaquePaint*          paint )
  {
    TT_Face       ttface;
    SFNT_Service  sfnt;


    إذا ( !face || !paint )
      أرجع 0;

    إذا ( !FT_IS_SFNT( face ) )
      أرجع 0;

    ttface = (TT_Face)face;
    sfnt   = (SFNT_Service)ttface->sfnt;

    إذا ( sfnt->get_colr_glyph_paint )
      أرجع sfnt->get_colr_glyph_paint( ttface,
                                         base_glyph,
                                         جڑ_transform,
                                         paint );
    وإلا
      أرجع 0;
  }


  /* documentation is in ftcolor.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Get_Color_Glyph_ClipBox( FT_Face      face,
                              FT_UInt      base_glyph,
                              FT_ClipBox*  clip_box )
  {
    TT_Face       ttface;
    SFNT_Service  sfnt;


    إذا ( !face || !clip_box )
      أرجع 0;

    إذا ( !FT_IS_SFNT( face ) )
      أرجع 0;

    ttface = (TT_Face)face;
    sfnt   = (SFNT_Service)ttface->sfnt;

    إذا ( sfnt->get_color_glyph_clipbox )
      أرجع sfnt->get_color_glyph_clipbox( ttface,
                                            base_glyph,
                                            clip_box );
    وإلا
      أرجع 0;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Get_Paint_Layers( FT_Face            face,
                       FT_LayerIterator*  layer_iterator,
                       FT_OpaquePaint*    paint )
  {
    TT_Face       ttface;
    SFNT_Service  sfnt;


    إذا ( !face || !paint || !layer_iterator )
      أرجع 0;

    إذا ( !FT_IS_SFNT( face ) )
      أرجع 0;

    ttface = (TT_Face)face;
    sfnt   = (SFNT_Service)ttface->sfnt;

    إذا ( sfnt->get_paint_layers )
      أرجع sfnt->get_paint_layers( ttface, layer_iterator, paint );
    وإلا
      أرجع 0;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Get_Paint( FT_Face face,
                FT_OpaquePaint  opaque_paint,
                FT_COLR_Paint*  paint )
  {
    TT_Face       ttface;
    SFNT_Service  sfnt;


    إذا ( !face || !paint )
      أرجع 0;

    إذا ( !FT_IS_SFNT( face ) )
      أرجع 0;

    ttface = (TT_Face)face;
    sfnt   = (SFNT_Service)ttface->sfnt;

    إذا ( sfnt->get_paint )
      أرجع sfnt->get_paint( ttface, opaque_paint, paint );
    وإلا
      أرجع 0;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Get_Colorline_Stops ( FT_Face                face,
                           FT_ColorStop *         color_stop,
                           FT_ColorStopIterator  *iterator )
  {
    TT_Face       ttface;
    SFNT_Service  sfnt;


    إذا ( !face || !color_stop || !iterator )
      أرجع 0;

    إذا ( !FT_IS_SFNT( face ) )
      أرجع 0;

    ttface = (TT_Face)face;
    sfnt   = (SFNT_Service)ttface->sfnt;

    إذا ( sfnt->get_colorline_stops )
      أرجع sfnt->get_colorline_stops ( ttface, color_stop, iterator );
    وإلا
      أرجع 0;
  }


/* END */
