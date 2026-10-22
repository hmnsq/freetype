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


#অন্তর্ভাব <freetype/ftlist.h>
#অন্তর্ভাব <freetype/ftoutln.h>
#অন্তর্ভাব <freetype/ftfntfmt.h>
#অন্তর্ভাব <freetype/otsvg.h>

#অন্তর্ভাব <freetype/internal/ftvalid.h>
#অন্তর্ভাব <freetype/internal/ftobjs.h>
#অন্তর্ভাব <freetype/internal/ftdebug.h>
#অন্তর্ভাব <freetype/internal/ftrfork.h>
#অন্তর্ভাব <freetype/internal/ftstream.h>
#অন্তর্ভাব <freetype/internal/sfnt.h>          /* for SFNT_Load_Table_Func */
#অন্তর্ভাব <freetype/internal/psaux.h>         /* for PS_Driver            */
#অন্তর্ভাব <freetype/internal/svginterface.h>

#অন্তর্ভাব <freetype/tttables.h>
#অন্তর্ভাব <freetype/tttags.h>
#অন্তর্ভাব <freetype/ttnameid.h>

#অন্তর্ভাব <freetype/internal/services/svprop.h>
#অন্তর্ভাব <freetype/internal/services/svsfnt.h>
#অন্তর্ভাব <freetype/internal/services/svpostnm.h>
#অন্তর্ভাব <freetype/internal/services/svgldict.h>
#অন্তর্ভাব <freetype/internal/services/svttcmap.h>
#অন্তর্ভাব <freetype/internal/services/svkern.h>
#অন্তর্ভাব <freetype/internal/services/svtteng.h>

#ifdef TT_CONFIG_OPTION_VARC
#অন্তর্ভাব <freetype/internal/services/svvarc.h>
#endif

#অন্তর্ভাব <freetype/ftdriver.h>

#ifdef FT_CONFIG_OPTION_MAC_FONTS
#অন্তর্ভাব "ftbase.h"
#endif


#ifdef FT_DEBUG_LEVEL_TRACE

#অন্তর্ভাব <freetype/ftbitmap.h>

#যদি defined( _MSC_VER )      /* Visual C++ (and Intel C++)   */
  /* We disable the warning `conversion from XXX to YYY,     */
  /* possible loss of data' in order to compile cleanly with */
  /* the maximum level of warnings: `md5.c' is non-FreeType  */
  /* code, and it gets used during development builds only.  */
#pragma warning( ঠেলো )
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

#অন্তর্ভাব "md5.c"

#যদি defined( _MSC_VER )
#pragma warning( বের_করো )
#endif

  /* This array must stay in sync with the @FT_Pixel_Mode enumeration */
  /* (in file `ftimage.h`).                                           */

  স্থির ধ্রুবক অক্ষর* ধ্রুবক  pixel_modes[] =
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
  স্থির FT_Error
  ft_open_face_internal( FT_Library           library,
                         ধ্রুবক FT_Open_Args*  args,
                         FT_Long              face_সূচক,
                         FT_Face             *aface,
                         FT_Bool              test_mac_fonts );


  FT_BASE_DEF( FT_Pointer )
  ft_service_list_lookup( FT_ServiceDesc  service_descriptors,
                          ধ্রুবক অক্ষর*     service_id )
  {
    FT_Pointer      ফলাফল = ফাঁকা;
    FT_ServiceDesc  desc   = service_descriptors;


    যদি ( desc && service_id )
    {
      জন্য ( ; desc->serv_id != ফাঁকা; desc++ )
      {
        যদি ( ft_strcmp( desc->serv_id, service_id ) == 0 )
        {
          ফলাফল = (FT_Pointer)desc->serv_তথ্য;
          বিরতি;
        }
      }
    }

    ফেরত ফলাফল;
  }


  FT_BASE_DEF( শূন্য )
  ft_validator_init( FT_Validator        valid,
                     ধ্রুবক FT_Byte*      base,
                     ধ্রুবক FT_Byte*      limit,
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

    ফেরত -1;
  }


  FT_BASE_DEF( শূন্য )
  ft_validator_error( FT_Validator  valid,
                      FT_Error      error )
  {
    /* since the cast below also disables the compiler's */
    /* type check, we introduce a dummy variable, which  */
    /* will be optimized away                            */
    অস্থির ft_jmp_buf* jump_বাফার = &valid->jump_বাফার;


    valid->error = error;

    /* throw away volatileness; use `jump_buffer' or the  */
    /* compiler may warn about an unused local variable   */
    ft_longjmp( *(ft_jmp_buf*) jump_বাফার, 1 );
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
                 ধ্রুবক FT_Open_Args*  args,
                 FT_Stream           *astream )
  {
    FT_Error   error;
    FT_Memory  memory;
    FT_Stream  stream = ফাঁকা;
    FT_UInt    mode;


    *astream = ফাঁকা;

    যদি ( !library )
      ফেরত FT_THROW( Invalid_Library_Handle );

    যদি ( !args )
      ফেরত FT_THROW( Invalid_Argument );

    memory = library->memory;
    mode   = args->flags &
               ( FT_OPEN_MEMORY | FT_OPEN_STREAM | FT_OPEN_PATHNAME );

    যদি ( mode == FT_OPEN_MEMORY )
    {
      /* create a memory-based stream */
      যদি ( FT_NEW( stream ) )
        যাও Exit;

      FT_Stream_OpenMemory( stream,
                            (ধ্রুবক FT_Byte*)args->memory_base,
                            (FT_ULong)args->memory_আকার );
      stream->memory = memory;
    }

#ifndef FT_CONFIG_OPTION_DISABLE_STREAM_SUPPORT

    অন্যথা যদি ( mode == FT_OPEN_PATHNAME )
    {
      /* create a normal system stream */
      যদি ( FT_NEW( stream ) )
        যাও Exit;

      stream->memory = memory;
      error = FT_Stream_Open( stream, args->pathname );
      যদি ( error )
        FT_FREE( stream );
    }
    অন্যথা যদি ( ( mode == FT_OPEN_STREAM ) && args->stream )
    {
      /* use an existing, user-provided stream */

      /* in this case, we do not need to allocate a new stream object */
      /* since the caller is responsible for closing it himself       */
      stream         = args->stream;
      stream->memory = memory;
      error          = FT_Err_Ok;
    }

#endif

    অন্যথা
    {
      error = FT_THROW( Invalid_Argument );
      যদি ( ( args->flags & FT_OPEN_STREAM ) && args->stream )
        FT_Stream_Close( args->stream );
    }

    যদি ( !error )
      *astream       = stream;

  Exit:
    ফেরত error;
  }


  FT_BASE_DEF( শূন্য )
  FT_Stream_Free( FT_Stream  stream,
                  FT_Int     external )
  {
    যদি ( stream )
    {
      FT_Memory  memory = stream->memory;


      FT_Stream_Close( stream );

      যদি ( !external )
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


  স্থির FT_Error
  ft_glyphslot_init( FT_GlyphSlot  slot )
  {
    FT_Driver         driver   = slot->face->driver;
    FT_Driver_Class   clazz    = driver->clazz;
    FT_Memory         memory   = driver->মূল.memory;
    FT_Error          error    = FT_Err_Ok;
    FT_Slot_Internal  internal = ফাঁকা;


    slot->library = driver->মূল.library;

    যদি ( FT_NEW( internal ) )
      যাও Exit;

    slot->internal = internal;

    যদি ( FT_DRIVER_USES_OUTLINES( driver ) )
      error = FT_GlyphLoader_New( memory, &internal->loader );

    যদি ( !error && clazz->init_slot )
      error = clazz->init_slot( slot );

#ifdef FT_CONFIG_OPTION_SVG
    /* if SVG table exists, allocate the space in `slot->other` */
    যদি ( slot->face->face_flags & FT_FACE_FLAG_SVG )
    {
      FT_SVG_Document  document = ফাঁকা;


      যদি ( FT_NEW( document ) )
        যাও Exit;
      slot->other = document;
    }
#endif

  Exit:
    ফেরত error;
  }


  FT_BASE_DEF( শূন্য )
  ft_glyphslot_free_bitmap( FT_GlyphSlot  slot )
  {
    যদি ( slot->internal && ( slot->internal->flags & FT_GLYPH_OWN_BITMAP ) )
    {
      FT_Memory  memory = FT_FACE_MEMORY( slot->face );


      FT_FREE( slot->bitmap.বাফার );
      slot->internal->flags &= ~FT_GLYPH_OWN_BITMAP;
    }
    অন্যথা
    {
      /* assume that the bitmap buffer was stolen or not */
      /* allocated from the heap                         */
      slot->bitmap.বাফার = ফাঁকা;
    }
  }


  /* overflow-resistant presetting of bitmap position and dimensions; */
  /* also check whether the size is too large for rendering           */
  FT_BASE_DEF( FT_Bool )
  ft_glyphslot_preset_bitmap( FT_GlyphSlot      slot,
                              FT_Render_Mode    mode,
                              ধ্রুবক FT_Vector*  origin )
  {
    FT_Outline*  outline = &slot->outline;
    FT_Bitmap*   bitmap  = &slot->bitmap;

    FT_Pixel_Mode  pixel_mode;

    FT_BBox  cbox, pbox;
    FT_Pos   x_shift = 0;
    FT_Pos   y_shift = 0;
    FT_Pos   x_বাম, y_শীর্ষ;
    FT_Pos   width, height, pitch;
    FT_Bool  ret;


    যদি ( slot->format == FT_GLYPH_FORMAT_SVG )
    {
      FT_Module    module;
      SVG_Service  svg_service;


      module      = FT_Get_Module( slot->library, "ot-svg" );
      svg_service = (SVG_Service)module->clazz->module_interface;

      ফেরত (FT_Bool)svg_service->preset_slot( module, slot, FALSE );
    }
    অন্যথা যদি ( slot->format != FT_GLYPH_FORMAT_OUTLINE )
      ফেরত 1;

    যদি ( origin )
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

    নির্বাচন ( mode )
    {
    ক্ষেত্র FT_RENDER_MODE_MONO:
      pixel_mode = FT_PIXEL_MODE_MONO;
#যদি 1
      /* x */

      /* undocumented but confirmed: bbox values get rounded;    */
      /* we do asymmetric rounding so that the center of a pixel */
      /* gets always included                                    */

      pbox.xMin += ( cbox.xMin + 31 ) >> 6;
      pbox.xMax += ( cbox.xMax + 32 ) >> 6;

      /* if the bbox collapsed, we add a pixel based on the total */
      /* rounding remainder to cover most of the original cbox    */

      যদি ( pbox.xMin == pbox.xMax )
      {
        যদি ( ( ( cbox.xMin + 31 ) & 63 ) - 31 +
             ( ( cbox.xMax + 32 ) & 63 ) - 32 < 0 )
          pbox.xMin -= 1;
        অন্যথা
          pbox.xMax += 1;
      }

      /* y */

      pbox.yMin += ( cbox.yMin + 31 ) >> 6;
      pbox.yMax += ( cbox.yMax + 32 ) >> 6;

      যদি ( pbox.yMin == pbox.yMax )
      {
        যদি ( ( ( cbox.yMin + 31 ) & 63 ) - 31 +
             ( ( cbox.yMax + 32 ) & 63 ) - 32 < 0 )
          pbox.yMin -= 1;
        অন্যথা
          pbox.yMax += 1;
      }

      বিরতি;
#অন্যথা
      যাও Adjust;
#endif

    ক্ষেত্র FT_RENDER_MODE_LCD:
      pixel_mode = FT_PIXEL_MODE_LCD;
      ft_lcd_padding( &cbox, slot, mode );
      যাও Adjust;

    ক্ষেত্র FT_RENDER_MODE_LCD_V:
      pixel_mode = FT_PIXEL_MODE_LCD_V;
      ft_lcd_padding( &cbox, slot, mode );
      যাও Adjust;

    ক্ষেত্র FT_RENDER_MODE_NORMAL:
    ক্ষেত্র FT_RENDER_MODE_LIGHT:
    পূর্বনির্ধারিত:
      pixel_mode = FT_PIXEL_MODE_GRAY;
    Adjust:
      pbox.xMin += cbox.xMin >> 6;
      pbox.yMin += cbox.yMin >> 6;
      pbox.xMax += ( cbox.xMax + 63 ) >> 6;
      pbox.yMax += ( cbox.yMax + 63 ) >> 6;
    }

    x_বাম = pbox.xMin;
    y_শীর্ষ  = pbox.yMax;

    width  = pbox.xMax - pbox.xMin;
    height = pbox.yMax - pbox.yMin;

    /* Flag the width or height unsuitable for rendering.   */
    /* The limit is based on the ppem value when available. */
    /* FT_Renderer modules should check the return value.   */
    ret = FT_BOOL(     width >= 0x10000   ||    height >= 0x10000   ||
                   pbox.xMin < -0x1000000 || pbox.xMax >= 0x1000000 ||
                   pbox.yMin < -0x1000000 || pbox.yMax >= 0x1000000 ||
             ( slot->face                                         &&
               ( width  > 16 * slot->face->আকার_2->metrics.x_ppem ||
                 height > 16 * slot->face->আকার_2->metrics.y_ppem ) ) );

    যদি ( ret )
      FT_TRACE3(( "ft_glyphslot_preset_bitmap: [%ld %ld %ld %ld]\n",
                  pbox.xMin, pbox.yMin, pbox.xMax, pbox.yMax ));

    নির্বাচন ( pixel_mode )
    {
    ক্ষেত্র FT_PIXEL_MODE_MONO:
      pitch = ( ( width + 15 ) >> 4 ) << 1;
      বিরতি;

    ক্ষেত্র FT_PIXEL_MODE_LCD:
      width *= 3;
      pitch  = FT_PAD_CEIL( width, 4 );
      বিরতি;

    ক্ষেত্র FT_PIXEL_MODE_LCD_V:
      height *= 3;
      FALL_THROUGH;

    ক্ষেত্র FT_PIXEL_MODE_GRAY:
    পূর্বনির্ধারিত:
      pitch = width;
    }

    slot->bitmap_বাম = (FT_Int)x_বাম;
    slot->bitmap_শীর্ষ  = (FT_Int)y_শীর্ষ;

    bitmap->pixel_mode = (unsigned অক্ষর)pixel_mode;
    bitmap->অঙ্ক_grays  = 256;
    bitmap->width      = (unsigned পূর্ণসংখ্যা)width;
    bitmap->rows       = (unsigned পূর্ণসংখ্যা)height;
    bitmap->pitch      = pitch;

    ফেরত ret;
  }


  FT_BASE_DEF( শূন্য )
  ft_glyphslot_set_bitmap( FT_GlyphSlot  slot,
                           FT_Byte*      বাফার )
  {
    ft_glyphslot_free_bitmap( slot );

    slot->bitmap.বাফার = বাফার;

    FT_ASSERT( (slot->internal->flags & FT_GLYPH_OWN_BITMAP) == 0 );
  }


  FT_BASE_DEF( FT_Error )
  ft_glyphslot_alloc_bitmap( FT_GlyphSlot  slot )
  {
    FT_Memory  memory = FT_FACE_MEMORY( slot->face );
    FT_Error   error;


    যদি ( slot->internal->flags & FT_GLYPH_OWN_BITMAP )
      FT_FREE( slot->bitmap.বাফার );
    অন্যথা
      slot->internal->flags |= FT_GLYPH_OWN_BITMAP;

    /* dimensions must be preset */
    FT_MEM_ALLOC_MULT( slot->bitmap.বাফার,
                       slot->bitmap.rows,
                       slot->bitmap.pitch );
    ফেরত error;
  }


  স্থির শূন্য
  ft_glyphslot_clear( FT_GlyphSlot  slot )
  {
    /* free bitmap if needed */
    ft_glyphslot_free_bitmap( slot );

    /* clear all public fields in the glyph slot */
    slot->glyph_সূচক = 0;

    FT_ZERO( &slot->metrics );
    FT_ZERO( &slot->outline );

    slot->bitmap.width      = 0;
    slot->bitmap.rows       = 0;
    slot->bitmap.pitch      = 0;
    slot->bitmap.pixel_mode = 0;
    /* `slot->bitmap.buffer' has been handled by ft_glyphslot_free_bitmap */

    slot->bitmap_বাম   = 0;
    slot->bitmap_শীর্ষ    = 0;
    slot->অঙ্ক_subglyphs = 0;
    slot->subglyphs     = ফাঁকা;
    slot->control_তথ্য  = ফাঁকা;
    slot->control_len   = 0;

#ifndef FT_CONFIG_OPTION_SVG
    slot->other = ফাঁকা;
#অন্যথা
    যদি ( !( slot->face->face_flags & FT_FACE_FLAG_SVG ) )
      slot->other = ফাঁকা;
    অন্যথা
    {
      যদি ( slot->internal->flags & FT_GLYPH_OWN_GZIP_SVG )
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


  স্থির শূন্য
  ft_glyphslot_done( FT_GlyphSlot  slot )
  {
    FT_Driver        driver = slot->face->driver;
    FT_Driver_Class  clazz  = driver->clazz;
    FT_Memory        memory = driver->মূল.memory;

#ifdef FT_CONFIG_OPTION_SVG
    যদি ( slot->face->face_flags & FT_FACE_FLAG_SVG )
    {
      /* Free memory in case SVG was there.                          */
      /* `slot->internal` might be NULL in out-of-memory situations. */
      যদি ( slot->internal && slot->internal->flags & FT_GLYPH_OWN_GZIP_SVG )
      {
        FT_SVG_Document  doc = (FT_SVG_Document)slot->other;


        FT_FREE( doc->svg_document );

        slot->internal->flags &= ~FT_GLYPH_OWN_GZIP_SVG;
      }

      FT_FREE( slot->other );
    }
#endif

    যদি ( clazz->done_slot )
      clazz->done_slot( slot );

    /* free bitmap buffer if needed */
    ft_glyphslot_free_bitmap( slot );

    /* slot->internal might be NULL in out-of-memory situations */
    যদি ( slot->internal )
    {
      /* free glyph loader */
      যদি ( FT_DRIVER_USES_OUTLINES( driver ) )
      {
        FT_GlyphLoader_Done( slot->internal->loader );
        slot->internal->loader = ফাঁকা;
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
    FT_GlyphSlot     slot = ফাঁকা;


    যদি ( !face )
      ফেরত FT_THROW( Invalid_Face_Handle );

    যদি ( !face->driver )
      ফেরত FT_THROW( Invalid_Argument );

    driver = face->driver;
    clazz  = driver->clazz;
    memory = driver->মূল.memory;

    FT_TRACE4(( "FT_New_GlyphSlot: Creating new slot object\n" ));
    যদি ( !FT_ALLOC( slot, clazz->slot_object_আকার ) )
    {
      slot->face = face;

      error = ft_glyphslot_init( slot );
      যদি ( error )
      {
        ft_glyphslot_done( slot );
        FT_FREE( slot );
        যাও Exit;
      }

      slot->পরবর্তী  = face->glyph;
      face->glyph = slot;

      যদি ( aslot )
        *aslot = slot;
    }
    অন্যথা যদি ( aslot )
      *aslot = ফাঁকা;


  Exit:
    FT_TRACE4(( "FT_New_GlyphSlot: Return 0x%x\n", error ));

    ফেরত error;
  }


  /* documentation is in ftobjs.h */

  FT_BASE_DEF( শূন্য )
  FT_Done_GlyphSlot( FT_GlyphSlot  slot )
  {
    যদি ( slot )
    {
      FT_Driver     driver = slot->face->driver;
      FT_Memory     memory = driver->মূল.memory;
      FT_GlyphSlot  পূর্ব;
      FT_GlyphSlot  cur;


      /* Remove slot from its parent face's list */
      পূর্ব = ফাঁকা;
      cur  = slot->face->glyph;

      যখন ( cur )
      {
        যদি ( cur == slot )
        {
          যদি ( !পূর্ব )
            slot->face->glyph = cur->পরবর্তী;
          অন্যথা
            পূর্ব->পরবর্তী = cur->পরবর্তী;

          /* finalize client-specific data */
          যদি ( slot->generic.finalizer )
            slot->generic.finalizer( slot );

          ft_glyphslot_done( slot );
          FT_FREE( slot );
          বিরতি;
        }
        পূর্ব = cur;
        cur  = cur->পরবর্তী;
      }
    }
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( শূন্য )
  FT_Set_Transform( FT_Face     face,
                    FT_Matrix*  matrix,
                    FT_Vector*  delta )
  {
    FT_Face_Internal  internal;


    যদি ( !face )
      ফেরত;

    internal = face->internal;

    internal->transform_flags = 0;

    যদি ( !matrix || !FT_Matrix_Check( matrix ) )
    {
      internal->transform_matrix.xx = 0x10000L;
      internal->transform_matrix.xy = 0;
      internal->transform_matrix.yx = 0;
      internal->transform_matrix.yy = 0x10000L;

      matrix = &internal->transform_matrix;
    }
    অন্যথা
      internal->transform_matrix = *matrix;

    /* set transform_flags bit flag 0 if `matrix' isn't the identity */
    যদি ( ( matrix->xy | matrix->yx ) ||
         matrix->xx != 0x10000L      ||
         matrix->yy != 0x10000L      )
      internal->transform_flags |= 1;

    যদি ( !delta )
    {
      internal->transform_delta.x = 0;
      internal->transform_delta.y = 0;

      delta = &internal->transform_delta;
    }
    অন্যথা
      internal->transform_delta = *delta;

    /* set transform_flags bit flag 1 if `delta' isn't the null vector */
    যদি ( delta->x | delta->y )
      internal->transform_flags |= 2;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( শূন্য )
  FT_Get_Transform( FT_Face     face,
                    FT_Matrix*  matrix,
                    FT_Vector*  delta )
  {
    FT_Face_Internal  internal;


    যদি ( !face )
      ফেরত;

    internal = face->internal;

    যদি ( matrix )
      *matrix = internal->transform_matrix;

    যদি ( delta )
      *delta = internal->transform_delta;
  }


  স্থির FT_Renderer
  ft_lookup_glyph_renderer( FT_GlyphSlot  slot );


#ifdef GRID_FIT_METRICS
  স্থির শূন্য
  ft_glyphslot_grid_fit_metrics( FT_GlyphSlot  slot,
                                 FT_Bool       vertical )
  {
    FT_Glyph_Metrics*  metrics = &slot->metrics;
    FT_Pos             ডান, bottom;


    যদি ( vertical )
    {
      metrics->horiBearingX = FT_PIX_FLOOR( metrics->horiBearingX );
      metrics->horiBearingY = FT_PIX_CEIL_LONG( metrics->horiBearingY );

      ডান  = FT_PIX_CEIL_LONG( ADD_LONG( metrics->vertBearingX,
                                           metrics->width ) );
      bottom = FT_PIX_CEIL_LONG( ADD_LONG( metrics->vertBearingY,
                                           metrics->height ) );

      metrics->vertBearingX = FT_PIX_FLOOR( metrics->vertBearingX );
      metrics->vertBearingY = FT_PIX_FLOOR( metrics->vertBearingY );

      metrics->width  = SUB_LONG( ডান,
                                  metrics->vertBearingX );
      metrics->height = SUB_LONG( bottom,
                                  metrics->vertBearingY );
    }
    অন্যথা
    {
      metrics->vertBearingX = FT_PIX_FLOOR( metrics->vertBearingX );
      metrics->vertBearingY = FT_PIX_FLOOR( metrics->vertBearingY );

      ডান  = FT_PIX_CEIL_LONG( ADD_LONG( metrics->horiBearingX,
                                           metrics->width ) );
      bottom = FT_PIX_FLOOR( SUB_LONG( metrics->horiBearingY,
                                       metrics->height ) );

      metrics->horiBearingX = FT_PIX_FLOOR( metrics->horiBearingX );
      metrics->horiBearingY = FT_PIX_CEIL_LONG( metrics->horiBearingY );

      metrics->width  = SUB_LONG( ডান,
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
                 FT_UInt   glyph_সূচক,
                 FT_Int32  load_flags )
  {
    FT_Error      error;
    FT_Driver     driver;
    FT_GlyphSlot  slot;
    FT_Library    library;
    FT_Bool       autohint = FALSE;
    FT_Module     hinter;


    যদি ( !face || !face->আকার_2 || !face->glyph )
      ফেরত FT_THROW( Invalid_Face_Handle );

    /* The validity test for `glyph_index' is performed by the */
    /* font drivers.                                           */

    slot = face->glyph;
    ft_glyphslot_clear( slot );

    driver  = face->driver;
    library = driver->মূল.library;
    hinter  = library->auto_hinter;

    /* undefined scale means no scale */
    যদি ( face->আকার_2->metrics.x_ppem == 0 ||
         face->আকার_2->metrics.y_ppem == 0 )
      load_flags |= FT_LOAD_NO_SCALE;

    /* resolve load flags dependencies */

    যদি ( load_flags & FT_LOAD_NO_RECURSE )
      load_flags |= FT_LOAD_NO_SCALE         |
                    FT_LOAD_IGNORE_TRANSFORM;

    যদি ( load_flags & FT_LOAD_NO_SCALE )
    {
      load_flags |= FT_LOAD_NO_HINTING |
                    FT_LOAD_NO_BITMAP;

      load_flags &= ~FT_LOAD_RENDER;
    }

    যদি ( load_flags & FT_LOAD_BITMAP_METRICS_ONLY )
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

    যদি ( hinter                                           &&
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
      যদি ( ( load_flags & FT_LOAD_FORCE_AUTOHINT ) ||
           !FT_DRIVER_HAS_HINTER( driver )         )
        autohint = TRUE;
      অন্যথা
      {
        FT_Render_Mode  mode = FT_LOAD_TARGET_MODE( load_flags );
        FT_Bool         is_light_type1;
        TT_Face         ttface = (TT_Face)face;


        /* only the new Adobe engine (for both CFF and Type 1) is `light'; */
        /* we use `strstr' to catch both `Type 1' and `CID Type 1'         */
        is_light_type1 =
          ft_strstr( FT_Get_Font_Format( face ), "Type 1" ) != ফাঁকা &&
          ((PS_Driver)driver)->hinting_engine == FT_HINTING_ADOBE;

        /* the check for `num_locations' assures that we actually    */
        /* test for instructions in a TTF and not in a CFF-based OTF */
        /*                                                           */
        /* we check the size of the `fpgm' and `prep' tables, too -- */
        /* the assumption is that there don't exist real TTFs where  */
        /* both `fpgm' and `prep' tables are missing                 */
        যদি ( ( mode == FT_RENDER_MODE_LIGHT           &&
               ( !FT_DRIVER_HINTS_LIGHTLY( driver ) &&
                 !is_light_type1                    ) )         ||
             ( FT_IS_SFNT( face )                             &&
               ttface->অঙ্ক_locations                          &&
               ttface->font_program_আকার == 0                 &&
               ttface->cvt_program_আকার <= 7                  ) )
          autohint = TRUE;
      }
    }

#ifdef TT_CONFIG_OPTION_VARC
    /* Never auto-hint a VARC (variable composite) glyph: its components */
    /* are already loaded unhinted, and the auto-fitter cannot process   */
    /* the assembled composite (it also re-enters variation-coordinate   */
    /* setting).  Use the native hinter / unhinted load instead.         */
    যদি ( autohint && FT_IS_SFNT( face ) )
    {
      FT_Service_VARC  varc;


      FT_FACE_FIND_SERVICE( face, varc, VARC );
      যদি ( varc && varc->has_glyph( face, glyph_সূচক ) )
        autohint = FALSE;
    }
#endif

    যদি ( autohint )
    {
      FT_AutoHinter_Interface  hinting;


      /* XXX: The use of the `FT_LOAD_XXX_ONLY` flags is not very */
      /*      elegant.                                            */

      /* try to load SVG documents if available */
      যদি ( ( load_flags & FT_LOAD_NO_SVG ) == 0 &&
           FT_HAS_SVG( face )                   )
      {
        error = driver->clazz->load_glyph( slot, face->আকার_2,
                                           glyph_সূচক,
                                           load_flags | FT_LOAD_SVG_ONLY );

        যদি ( !error && slot->format == FT_GLYPH_FORMAT_SVG )
          যাও Load_Ok;
      }

      /* try to load embedded bitmaps if available */
      যদি ( FT_HAS_FIXED_SIZES( face )              &&
           ( load_flags & FT_LOAD_NO_BITMAP ) == 0 )
      {
        error = driver->clazz->load_glyph( slot, face->আকার_2,
                                           glyph_সূচক,
                                           load_flags | FT_LOAD_SBITS_ONLY );

        যদি ( !error && slot->format == FT_GLYPH_FORMAT_BITMAP )
          যাও Load_Ok;
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
                                       slot, face->আকার_2,
                                       glyph_সূচক, load_flags );

        internal->transform_flags = transform_flags;
      }
    }
    অন্যথা
    {
      error = driver->clazz->load_glyph( slot,
                                         face->আকার_2,
                                         glyph_সূচক,
                                         load_flags );
      যদি ( error )
        যাও Exit;

      যদি ( slot->format == FT_GLYPH_FORMAT_OUTLINE )
      {
        /* check that the loaded outline is correct */
        error = FT_Outline_Check( &slot->outline );
        যদি ( error )
          যাও Exit;

#ifdef GRID_FIT_METRICS
        যদি ( !( load_flags & FT_LOAD_NO_HINTING ) )
          ft_glyphslot_grid_fit_metrics(
            slot,
            FT_BOOL( load_flags & FT_LOAD_VERTICAL_LAYOUT ) );
#endif
      }
    }

  Load_Ok:
    /* compute the advance */
    যদি ( load_flags & FT_LOAD_VERTICAL_LAYOUT )
    {
      slot->advance.x = 0;
      slot->advance.y = slot->metrics.vertAdvance;
    }
    অন্যথা
    {
      slot->advance.x = slot->metrics.horiAdvance;
      slot->advance.y = 0;
    }

    /* compute the linear advance in 16.16 pixels */
    যদি ( ( load_flags & FT_LOAD_LINEAR_DESIGN ) == 0 &&
         FT_IS_SCALABLE( face )                      )
    {
      FT_আকার_Metrics*  metrics = &face->আকার_2->metrics;


      /* it's tricky! */
      slot->linearHoriAdvance = FT_MulDiv( slot->linearHoriAdvance,
                                           metrics->x_scale, 64 );

      slot->linearVertAdvance = FT_MulDiv( slot->linearVertAdvance,
                                           metrics->y_scale, 64 );
    }

    যদি ( ( load_flags & FT_LOAD_IGNORE_TRANSFORM ) == 0 )
    {
      FT_Face_Internal  internal = face->internal;


      /* now, transform the glyph image if needed */
      যদি ( internal->transform_flags )
      {
        /* get renderer */
        FT_Renderer  renderer = ft_lookup_glyph_renderer( slot );


        যদি ( renderer )
          error = renderer->clazz->transform_glyph(
                                     renderer, slot,
                                     &internal->transform_matrix,
                                     &internal->transform_delta );
        অন্যথা যদি ( slot->format == FT_GLYPH_FORMAT_OUTLINE )
        {
          /* apply `standard' transformation if no renderer is available */
          যদি ( internal->transform_flags & 1 )
            FT_Outline_Transform( &slot->outline,
                                  &internal->transform_matrix );

          যদি ( internal->transform_flags & 2 )
            FT_Outline_Translate( &slot->outline,
                                  internal->transform_delta.x,
                                  internal->transform_delta.y );
        }

        /* transform advance */
        FT_Vector_Transform( &slot->advance, &internal->transform_matrix );
      }
    }

    slot->glyph_সূচক          = glyph_সূচক;
    slot->internal->load_flags = load_flags;

    /* do we need to render the image or preset the bitmap now? */
    যদি ( !error                                    &&
         ( load_flags & FT_LOAD_NO_SCALE ) == 0    &&
         slot->format != FT_GLYPH_FORMAT_BITMAP    &&
         slot->format != FT_GLYPH_FORMAT_COMPOSITE )
    {
      FT_Render_Mode  mode = FT_LOAD_TARGET_MODE( load_flags );


      যদি ( mode == FT_RENDER_MODE_NORMAL   &&
           load_flags & FT_LOAD_MONOCHROME )
        mode = FT_RENDER_MODE_MONO;

      যদি ( load_flags & FT_LOAD_RENDER )
        error = FT_Render_Glyph( slot, mode );
      অন্যথা
        ft_glyphslot_preset_bitmap( slot, mode, ফাঁকা );
    }

#ifdef FT_DEBUG_LEVEL_TRACE
    FT_TRACE5(( "FT_Load_Glyph: index %u, flags 0x%x\n",
                glyph_সূচক, load_flags ));
    FT_TRACE5(( "  bitmap %ux%u %s, %s (mode %d)\n",
                slot->bitmap.width,
                slot->bitmap.rows,
                slot->outline.points ?
                  slot->bitmap.বাফার ? "rendered"
                                      : "preset"
                                     :
                  slot->internal->flags & FT_GLYPH_OWN_BITMAP ? "owned"
                                                              : "unowned",
                pixel_modes[slot->bitmap.pixel_mode],
                slot->bitmap.pixel_mode ));
    FT_TRACE5(( "\n" ));
    FT_TRACE5(( "  x advance: %f\n", (দ্বিগুণ)slot->advance.x / 64 ));
    FT_TRACE5(( "  y advance: %f\n", (দ্বিগুণ)slot->advance.y / 64 ));
    FT_TRACE5(( "  linear x advance: %f\n",
                (দ্বিগুণ)slot->linearHoriAdvance / 65536 ));
    FT_TRACE5(( "  linear y advance: %f\n",
                (দ্বিগুণ)slot->linearVertAdvance / 65536 ));

    {
      FT_Glyph_Metrics*  metrics = &slot->metrics;


      FT_TRACE5(( "  metrics:\n" ));
      FT_TRACE5(( "    width:  %f\n", (দ্বিগুণ)metrics->width / 64 ));
      FT_TRACE5(( "    height: %f\n", (দ্বিগুণ)metrics->height / 64 ));
      FT_TRACE5(( "\n" ));
      FT_TRACE5(( "    horiBearingX: %f\n",
                  (দ্বিগুণ)metrics->horiBearingX / 64 ));
      FT_TRACE5(( "    horiBearingY: %f\n",
                  (দ্বিগুণ)metrics->horiBearingY / 64 ));
      FT_TRACE5(( "    horiAdvance:  %f\n",
                  (দ্বিগুণ)metrics->horiAdvance / 64 ));
      FT_TRACE5(( "\n" ));
      FT_TRACE5(( "    vertBearingX: %f\n",
                  (দ্বিগুণ)metrics->vertBearingX / 64 ));
      FT_TRACE5(( "    vertBearingY: %f\n",
                  (দ্বিগুণ)metrics->vertBearingY / 64 ));
      FT_TRACE5(( "    vertAdvance:  %f\n",
                  (দ্বিগুণ)metrics->vertAdvance / 64 ));
    }
#endif

  Exit:
    ফেরত error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Load_Char( FT_Face   face,
                FT_ULong  char_code,
                FT_Int32  load_flags )
  {
    FT_UInt  glyph_সূচক;


    যদি ( !face )
      ফেরত FT_THROW( Invalid_Face_Handle );

    glyph_সূচক = (FT_UInt)char_code;
    যদি ( face->charmap )
      glyph_সূচক = FT_Get_Char_সূচক( face, char_code );

    ফেরত FT_Load_Glyph( face, glyph_সূচক, load_flags );
  }


  /* destructor for sizes list */
  স্থির শূন্য
  destroy_আকার( FT_Memory  memory,
                শূন্য*      আকার_3,
                শূন্য*      driver_ )
  {
    FT_আকার    আকার_2   = (FT_আকার)আকার_3;
    FT_Driver  driver = (FT_Driver)driver_;


    /* finalize format-specific stuff */
    যদি ( driver->clazz->done_আকার )
      driver->clazz->done_আকার( আকার_2 );

    /* finalize client-specific data */
    যদি ( আকার_2->generic.finalizer )
      আকার_2->generic.finalizer( আকার_2 );

    FT_FREE( আকার_2->internal );
    FT_FREE( আকার_2 );
  }


  স্থির শূন্য
  ft_cmap_done_internal( FT_CMap  cmap );


  স্থির শূন্য
  destroy_charmaps( FT_Face    face,
                    FT_Memory  memory )
  {
    FT_Int  n;


    যদি ( !face )
      ফেরত;

    জন্য ( n = 0; n < face->অঙ্ক_charmaps; n++ )
    {
      FT_CMap  cmap = FT_CMAP( face->charmaps[n] );


      ft_cmap_done_internal( cmap );

      face->charmaps[n] = ফাঁকা;
    }

    FT_FREE( face->charmaps );
    face->অঙ্ক_charmaps = 0;
  }


  /* destructor for faces list */
  স্থির শূন্য
  destroy_face( FT_Memory  memory,
                শূন্য*      face_,
                শূন্য*      driver_ )
  {
    FT_Face          face   = (FT_Face)face_;
    FT_Driver        driver = (FT_Driver)driver_;
    FT_Driver_Class  clazz  = driver->clazz;


    /* discard auto-hinting data */
    যদি ( face->autohint.finalizer )
      face->autohint.finalizer( face->autohint.তথ্য );

    /* Discard glyph slots for this face.                           */
    /* Beware!  FT_Done_GlyphSlot() changes the field `face->glyph' */
    যখন ( face->glyph )
      FT_Done_GlyphSlot( face->glyph );

    /* discard all sizes for this face */
    FT_List_Finalize( &face->sizes_list,
                      destroy_আকার,
                      memory,
                      driver );
    face->আকার_2 = ফাঁকা;

    /* discard charmaps */
    destroy_charmaps( face, memory );

    /* finalize format-specific stuff */
    যদি ( clazz->done_face )
      clazz->done_face( face );

    /* close the stream for this face if needed */
    FT_Stream_Free(
      face->stream,
      ( face->face_flags & FT_FACE_FLAG_EXTERNAL_STREAM ) != 0 );

    face->stream = ফাঁকা;

    /* now discard client data */
    যদি ( face->generic.finalizer )
      face->generic.finalizer( face );

    /* get rid of it */
    যদি ( face->internal )
    {
      FT_FREE( face->internal );
    }
    FT_FREE( face );
  }


  স্থির শূন্য
  Destroy_Driver( FT_Driver  driver )
  {
    FT_List_Finalize( &driver->faces_list,
                      destroy_face,
                      driver->মূল.memory,
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

    যদি ( !first )
      ফেরত FT_THROW( Invalid_CharMap_Handle );

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

    cur = first + face->অঙ্ক_charmaps;  /* points after the last one */

    জন্য ( ; --cur >= first; )
    {
      যদি ( cur[0]->encoding == FT_ENCODING_UNICODE )
      {
        /* XXX If some new encodings to represent UCS-4 are added, */
        /*     they should be added here.                          */
        যদি ( ( cur[0]->platform_id == TT_PLATFORM_MICROSOFT &&
               cur[0]->encoding_id == TT_MS_ID_UCS_4        )     ||
             ( cur[0]->platform_id == TT_PLATFORM_APPLE_UNICODE &&
               cur[0]->encoding_id == TT_APPLE_ID_UNICODE_32    ) ||
             ( cur[0]->platform_id == TT_PLATFORM_APPLE_UNICODE &&
               cur[0]->encoding_id == TT_APPLE_ID_FULL_UNICODE  &&
               FT_Get_CMap_Format( cur[0] ) == 13               ) )
        {
          face->charmap = cur[0];
          ফেরত FT_Err_Ok;
        }
      }
    }

    /* We do not have any UCS-4 charmap.                */
    /* Do the loop again and search for UCS-2 charmaps. */
    cur = first + face->অঙ্ক_charmaps;

    জন্য ( ; --cur >= first; )
    {
      যদি ( cur[0]->encoding == FT_ENCODING_UNICODE )
      {
        FT_Long  format = FT_Get_CMap_Format( cur[0] );


        যদি ( format != 14 && format != 15 )
        {
          face->charmap = cur[0];
          ফেরত FT_Err_Ok;
        }
      }
    }

    ফেরত FT_THROW( Invalid_CharMap_Handle );
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
  স্থির FT_CharMap
  find_variant_selector_charmap( FT_Face  face )
  {
    FT_CharMap*  first;
    FT_CharMap*  end;
    FT_CharMap*  cur;


    /* caller should have already checked that `face' is valid */
    FT_ASSERT( face );

    first = face->charmaps;

    যদি ( !first )
      ফেরত ফাঁকা;

    end = first + face->অঙ্ক_charmaps;  /* points after the last one */

    জন্য ( cur = first; cur < end; cur++ )
    {
      যদি ( cur[0]->platform_id == TT_PLATFORM_APPLE_UNICODE    &&
           cur[0]->encoding_id == TT_APPLE_ID_VARIANT_SELECTOR )
      {
        FT_Long  format = FT_Get_CMap_Format( cur[0] );


        যদি ( format == 14 || format == 15 )
          ফেরত cur[0];
      }
    }

    ফেরত ফাঁকা;
  }


  /**************************************************************************
   *
   * @Function:
   *   open_face
   *
   * @Description:
   *   This function does some work for FT_Open_Face().
   */
  স্থির FT_Error
  open_face( FT_Driver      driver,
             FT_Stream      *astream,
             FT_Bool        *anexternal_stream,
             FT_Long        face_সূচক,
             FT_Int         অঙ্ক_params,
             FT_Parameter*  params,
             FT_Face       *aface )
  {
    FT_Memory         memory;
    FT_Driver_Class   clazz;
    FT_Face           face     = ফাঁকা;
    FT_Face_Internal  internal = ফাঁকা;

    FT_Error          error, error2;


    clazz  = driver->clazz;
    memory = driver->মূল.memory;

    /* allocate the face object and perform basic initialization */
    যদি ( FT_ALLOC( face, clazz->face_object_আকার ) )
      যাও Fail;

    face->driver = driver;
    face->memory = memory;
    face->stream = *astream;

    /* set the FT_FACE_FLAG_EXTERNAL_STREAM bit for FT_Done_Face */
    যদি ( *anexternal_stream )
      face->face_flags |= FT_FACE_FLAG_EXTERNAL_STREAM;

    যদি ( FT_NEW( internal ) )
      যাও Fail;

    face->internal = internal;

#ifdef FT_CONFIG_OPTION_INCREMENTAL
    {
      পূর্ণসংখ্যা  i;


      face->internal->incremental_interface = ফাঁকা;
      জন্য ( i = 0; i < অঙ্ক_params && !face->internal->incremental_interface;
            i++ )
        যদি ( params[i].tag == FT_PARAM_TAG_INCREMENTAL )
          face->internal->incremental_interface =
            (FT_Incremental_Interface)params[i].তথ্য;
    }
#endif

    face->internal->random_seed = -1;

    যদি ( clazz->init_face )
      error = clazz->init_face( *astream,
                                face,
                                (FT_Int)face_সূচক,
                                অঙ্ক_params,
                                params );
    /* Stream may have been changed. */
    *astream = face->stream;
    *anexternal_stream =
      ( face->face_flags & FT_FACE_FLAG_EXTERNAL_STREAM ) != 0;
    যদি ( error )
      যাও Fail;

    /* select Unicode charmap by default */
    error2 = find_unicode_charmap( face );

    /* if no Unicode charmap can be found, FT_Err_Invalid_CharMap_Handle */
    /* is returned.                                                      */

    /* no error should happen, but we want to play safe */
    যদি ( error2 && FT_ERR_NEQ( error2, Invalid_CharMap_Handle ) )
    {
      error = error2;
      যাও Fail;
    }

    *aface = face;

  Fail:
    যদি ( error )
    {
      destroy_charmaps( face, memory );
      যদি ( clazz->done_face )
        clazz->done_face( face );
      FT_FREE( internal );
      FT_FREE( face );
      *aface = ফাঁকা;
    }

    ফেরত error;
  }


  /* there's a Mac-specific extended implementation of FT_New_Face() */
  /* in src/base/ftmac.c                                             */

#ifndef FT_MACINTOSH

  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_New_Face( FT_Library   library,
               ধ্রুবক অক্ষর*  pathname,
               FT_Long      face_সূচক,
               FT_Face     *aface )
  {
    FT_Open_Args  args;


    /* test for valid `library' and `aface' delayed to `FT_Open_Face' */
    যদি ( !pathname )
      ফেরত FT_THROW( Invalid_Argument );

    args.flags    = FT_OPEN_PATHNAME;
    args.pathname = (অক্ষর*)pathname;
    args.stream   = ফাঁকা;

    ফেরত ft_open_face_internal( library, &args, face_সূচক, aface, 1 );
  }

#endif


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_New_Memory_Face( FT_Library      library,
                      ধ্রুবক FT_Byte*  file_base,
                      FT_Long         file_আকার,
                      FT_Long         face_সূচক,
                      FT_Face        *aface )
  {
    FT_Open_Args  args;


    /* test for valid `library' and `face' delayed to `FT_Open_Face' */
    যদি ( !file_base )
      ফেরত FT_THROW( Invalid_Argument );

    args.flags       = FT_OPEN_MEMORY;
    args.memory_base = file_base;
    args.memory_আকার = file_আকার;
    args.stream      = ফাঁকা;

    ফেরত ft_open_face_internal( library, &args, face_সূচক, aface, 1 );
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
  স্থির শূন্য
  memory_stream_close( FT_Stream  stream )
  {
    FT_Memory  memory = (FT_Memory)stream->descriptor.pointer;


    FT_FREE( stream->base );
    stream->আকার_2  = 0;
    stream->close = ফাঁকা;
    FT_FREE( stream );
  }


  /* Create a new memory stream from a buffer and a size. */
  /* From `ftmac.c'.                                      */
  স্থির FT_Error
  new_memory_stream( FT_Library           library,
                     FT_Byte*             base,
                     FT_ULong             আকার_2,
                     FT_Stream_CloseFunc  close,
                     FT_Stream           *astream )
  {
    FT_Error   error;
    FT_Memory  memory;
    FT_Stream  stream = ফাঁকা;


    যদি ( !library )
      ফেরত FT_THROW( Invalid_Library_Handle );

    যদি ( !base )
      ফেরত FT_THROW( Invalid_Argument );

    *astream = ফাঁকা;
    memory   = library->memory;
    যদি ( FT_NEW( stream ) )
      যাও Exit;

    FT_Stream_OpenMemory( stream, base, আকার_2 );

    stream->descriptor.pointer = memory;
    stream->close              = close;

    *astream = stream;

  Exit:
    ফেরত error;
  }


  /* Create a new FT_Face given a buffer and a driver name. */
  /* From `ftmac.c'.                                        */
  FT_LOCAL_DEF( FT_Error )
  open_face_from_বাফার( FT_Library   library,
                         FT_Byte*     base,
                         FT_ULong     আকার_2,
                         FT_Long      face_সূচক,
                         ধ্রুবক অক্ষর*  driver_name,
                         FT_Face     *aface )
  {
    FT_Open_Args  args;
    FT_Error      error;
    FT_Memory     memory = library->memory;


    args.driver = ফাঁকা;
    args.flags  = 0;

    যদি ( driver_name )
    {
      args.driver = FT_Get_Module( library, driver_name );
      যদি ( !args.driver )
      {
        FT_FREE( base );
        ফেরত FT_THROW( Missing_Module );
      }

      args.flags = args.flags | FT_OPEN_DRIVER;
    }

    /* `memory_stream_close` also frees the stream object. */
    error = new_memory_stream( library,
                               base,
                               আকার_2,
                               memory_stream_close,
                               &args.stream );
    যদি ( error )
    {
      FT_FREE( base );
      ফেরত error;
    }

    args.flags |= FT_OPEN_STREAM;

#ifdef FT_MACINTOSH
    /* At this point, the face index has served its purpose;  */
    /* whoever calls this function has already used it to     */
    /* locate the correct font data.  We should not propagate */
    /* this index to FT_Open_Face() (unless it is negative).  */

    যদি ( face_সূচক > 0 )
      face_সূচক &= 0x7FFF0000L; /* retain GX data */
#endif

    ফেরত ft_open_face_internal( library, &args, face_সূচক, aface, 0 );
  }


  /* Look up `TYP1' or `CID ' table from sfnt table directory.       */
  /* `offset' and `length' must exclude the binary header in tables. */

  /* Type 1 and CID-keyed font drivers should recognize sfnt-wrapped */
  /* format too.  Here, since we can't expect that the TrueType font */
  /* driver is loaded unconditionally, we must parse the font by     */
  /* ourselves.  We are only interested in the name of the table and */
  /* the offset.                                                     */

  স্থির FT_Error
  ft_lookup_PS_in_sfnt_stream( FT_Stream  stream,
                               FT_Long    face_সূচক,
                               FT_ULong*  offset,
                               FT_ULong*  দৈর্ঘ্য,
                               FT_Bool*   is_sfnt_cid )
  {
    FT_Error   error;
    FT_UShort  অঙ্ক_Tables;
    FT_Long    pstable_সূচক;
    FT_ULong   tag;
    পূর্ণসংখ্যা        i;


    *offset = 0;
    *দৈর্ঘ্য = 0;
    *is_sfnt_cid = FALSE;

    /* TODO: support for sfnt-wrapped PS/CID in TTC format */

    /* version check for 'typ1' (should be ignored?) */
    যদি ( FT_READ_ULONG( tag ) )
      ফেরত error;
    যদি ( tag != TTAG_typ1 )
      ফেরত FT_THROW( Unknown_File_Format );

    যদি ( FT_READ_USHORT( অঙ্ক_Tables ) )
      ফেরত error;
    যদি ( FT_STREAM_SKIP( 2 * 3 ) ) /* skip binary search header */
      ফেরত error;

    pstable_সূচক = -1;
    *is_sfnt_cid  = FALSE;

    জন্য ( i = 0; i < অঙ্ক_Tables; i++ )
    {
      যদি ( FT_READ_ULONG( tag )     || FT_STREAM_SKIP( 4 )      ||
           FT_READ_ULONG( *offset ) || FT_READ_ULONG( *দৈর্ঘ্য ) )
        ফেরত error;

      যদি ( tag == TTAG_CID )
      {
        pstable_সূচক++;
        *offset += 22;
        *দৈর্ঘ্য -= 22;
        *is_sfnt_cid = TRUE;
        যদি ( face_সূচক < 0 )
          ফেরত FT_Err_Ok;
      }
      অন্যথা যদি ( tag == TTAG_TYP1 )
      {
        pstable_সূচক++;
        *offset += 24;
        *দৈর্ঘ্য -= 24;
        *is_sfnt_cid = FALSE;
        যদি ( face_সূচক < 0 )
          ফেরত FT_Err_Ok;
      }
      যদি ( face_সূচক >= 0 && pstable_সূচক == face_সূচক )
        ফেরত FT_Err_Ok;
    }

    ফেরত FT_THROW( Table_Missing );
  }


  FT_LOCAL_DEF( FT_Error )
  open_face_PS_from_sfnt_stream( FT_Library     library,
                                 FT_Stream      stream,
                                 FT_Long        face_সূচক,
                                 FT_Int         অঙ্ক_params,
                                 FT_Parameter  *params,
                                 FT_Face       *aface )
  {
    FT_Error   error;
    FT_Memory  memory = library->memory;
    FT_ULong   offset, দৈর্ঘ্য;
    FT_ULong   pos;
    FT_Bool    is_sfnt_cid;
    FT_Byte*   sfnt_ps = ফাঁকা;

    FT_UNUSED( অঙ্ক_params );
    FT_UNUSED( params );


    /* ignore GX stuff */
    যদি ( face_সূচক > 0 )
      face_সূচক &= 0xFFFFL;

    pos = FT_STREAM_POS();

    error = ft_lookup_PS_in_sfnt_stream( stream,
                                         face_সূচক,
                                         &offset,
                                         &দৈর্ঘ্য,
                                         &is_sfnt_cid );
    যদি ( error )
      যাও Exit;

    যদি ( offset > stream->আকার_2 )
    {
      FT_TRACE2(( "open_face_PS_from_sfnt_stream: invalid table offset\n" ));
      error = FT_THROW( Invalid_Table );
      যাও Exit;
    }
    অন্যথা যদি ( দৈর্ঘ্য > stream->আকার_2 - offset )
    {
      FT_TRACE2(( "open_face_PS_from_sfnt_stream: invalid table length\n" ));
      error = FT_THROW( Invalid_Table );
      যাও Exit;
    }

    error = FT_Stream_Seek( stream, pos + offset );
    যদি ( error )
      যাও Exit;

    যদি ( FT_QALLOC( sfnt_ps, (FT_Long)দৈর্ঘ্য ) )
      যাও Exit;

    error = FT_Stream_Read( stream, (FT_Byte *)sfnt_ps, দৈর্ঘ্য );
    যদি ( error )
    {
      FT_FREE( sfnt_ps );
      যাও Exit;
    }

    error = open_face_from_বাফার( library,
                                   sfnt_ps,
                                   দৈর্ঘ্য,
                                   FT_MIN( face_সূচক, 0 ),
                                   is_sfnt_cid ? "t1cid" : "type1",
                                   aface );
  Exit:
    {
      FT_Error  error1;


      যদি ( FT_ERR_EQ( error, Unknown_File_Format ) )
      {
        error1 = FT_Stream_Seek( stream, pos );
        যদি ( error1 )
          ফেরত error1;
      }

      ফেরত error;
    }
  }


#ifndef FT_MACINTOSH

  /* The resource header says we've got resource_cnt `POST' (type1) */
  /* resources in this file.  They all need to be coalesced into    */
  /* one lump which gets passed on to the type1 driver.             */
  /* Here can be only one PostScript font in a file so face_index   */
  /* must be 0 (or -1).                                             */
  /*                                                                */
  স্থির FT_Error
  Mac_Read_POST_Resource( FT_Library  library,
                          FT_Stream   stream,
                          FT_Long    *offsets,
                          FT_Long     resource_cnt,
                          FT_Long     face_সূচক,
                          FT_Face    *aface )
  {
    FT_Error   error  = FT_ERR( Cannot_Open_Resource );
    FT_Memory  memory = library->memory;

    FT_Byte*   pfb_তথ্য = ফাঁকা;
    পূর্ণসংখ্যা        i, type, flags;
    FT_ULong   len;
    FT_ULong   pfb_len, pfb_pos, pfb_lenpos;
    FT_ULong   rlen, অস্থায়ী;


    যদি ( face_সূচক == -1 )
      face_সূচক = 0;
    যদি ( face_সূচক != 0 )
      ফেরত error;

    /* Find the length of all the POST resources, concatenated.  Assume */
    /* worst case (each resource in its own section).                   */
    pfb_len = 0;
    জন্য ( i = 0; i < resource_cnt; i++ )
    {
      error = FT_Stream_Seek( stream, (FT_ULong)offsets[i] );
      যদি ( error )
        যাও Exit;
      যদি ( FT_READ_ULONG( অস্থায়ী ) )  /* actually LONG */
        যাও Exit;

      /* FT2 allocator takes signed long buffer length,
       * too large value causing overflow should be checked
       */
      FT_TRACE4(( "                 POST fragment #%d: length=0x%08lx"
                  " total pfb_len=0x%08lx\n",
                  i, অস্থায়ী, pfb_len + অস্থায়ী + 6 ));

      যদি ( FT_MAC_RFORK_MAX_LEN < অস্থায়ী               ||
           FT_MAC_RFORK_MAX_LEN - অস্থায়ী < pfb_len + 6 )
      {
        FT_TRACE2(( "             MacOS resource length cannot exceed"
                    " 0x%08lx\n",
                    FT_MAC_RFORK_MAX_LEN ));

        error = FT_THROW( Invalid_Offset );
        যাও Exit;
      }

      pfb_len += অস্থায়ী + 6;
    }

    FT_TRACE2(( "             total buffer size to concatenate"
                " %ld POST fragments: 0x%08lx\n",
                 resource_cnt, pfb_len + 2 ));

    যদি ( pfb_len + 2 < 6 )
    {
      FT_TRACE2(( "             too long fragment length makes"
                  " pfb_len confused: pfb_len=0x%08lx\n",
                  pfb_len ));

      error = FT_THROW( Array_Too_Large );
      যাও Exit;
    }

    যদি ( FT_QALLOC( pfb_তথ্য, (FT_Long)pfb_len + 2 ) )
      যাও Exit;

    pfb_তথ্য[0] = 0x80;
    pfb_তথ্য[1] = 1;            /* Ascii section */
    pfb_তথ্য[2] = 0;            /* 4-byte length, fill in later */
    pfb_তথ্য[3] = 0;
    pfb_তথ্য[4] = 0;
    pfb_তথ্য[5] = 0;
    pfb_pos     = 6;
    pfb_lenpos  = 2;

    len  = 0;
    type = 1;

    জন্য ( i = 0; i < resource_cnt; i++ )
    {
      error = FT_Stream_Seek( stream, (FT_ULong)offsets[i] );
      যদি ( error )
        যাও Exit2;
      যদি ( FT_READ_ULONG( rlen ) )
        যাও Exit2;

      /* FT2 allocator takes signed long buffer length,
       * too large fragment length causing overflow should be checked
       */
      যদি ( 0x7FFFFFFFUL < rlen )
      {
        error = FT_THROW( Invalid_Offset );
        যাও Exit2;
      }

      যদি ( FT_READ_USHORT( flags ) )
        যাও Exit2;

      FT_TRACE3(( "POST fragment[%d]:"
                  " offsets=0x%08lx, rlen=0x%08lx, flags=0x%04x\n",
                  i, offsets[i], rlen, flags ));

      error = FT_ERR( Array_Too_Large );

      /* postpone the check of `rlen longer than buffer' */
      /* until `FT_Stream_Read'                          */

      যদি ( ( flags >> 8 ) == 0 )        /* Comment, should not be loaded */
      {
        FT_TRACE3(( "    Skip POST fragment #%d because it is a comment\n",
                    i ));
        চালু;
      }

      /* the flags are part of the resource, so rlen >= 2,  */
      /* but some fonts declare rlen = 0 for empty fragment */
      যদি ( rlen > 2 )
        rlen -= 2;
      অন্যথা
        rlen = 0;

      যদি ( ( flags >> 8 ) == type )
        len += rlen;
      অন্যথা
      {
        FT_TRACE3(( "    Write POST fragment #%d header (4-byte) to buffer"
                    " %p + 0x%08lx\n",
                    i, (শূন্য*)pfb_তথ্য, pfb_lenpos ));

        যদি ( pfb_lenpos + 3 > pfb_len + 2 )
          যাও Exit2;

        pfb_তথ্য[pfb_lenpos    ] = (FT_Byte)( len );
        pfb_তথ্য[pfb_lenpos + 1] = (FT_Byte)( len >> 8 );
        pfb_তথ্য[pfb_lenpos + 2] = (FT_Byte)( len >> 16 );
        pfb_তথ্য[pfb_lenpos + 3] = (FT_Byte)( len >> 24 );

        যদি ( ( flags >> 8 ) == 5 )      /* End of font mark */
          বিরতি;

        FT_TRACE3(( "    Write POST fragment #%d header (6-byte) to buffer"
                    " %p + 0x%08lx\n",
                    i, (শূন্য*)pfb_তথ্য, pfb_pos ));

        যদি ( pfb_pos + 6 > pfb_len + 2 )
          যাও Exit2;

        pfb_তথ্য[pfb_pos++] = 0x80;

        type = flags >> 8;
        len  = rlen;

        pfb_তথ্য[pfb_pos++] = (FT_Byte)type;
        pfb_lenpos          = pfb_pos;
        pfb_তথ্য[pfb_pos++] = 0;        /* 4-byte length, fill in later */
        pfb_তথ্য[pfb_pos++] = 0;
        pfb_তথ্য[pfb_pos++] = 0;
        pfb_তথ্য[pfb_pos++] = 0;
      }

      যদি ( pfb_pos > pfb_len || pfb_pos + rlen > pfb_len )
        যাও Exit2;

      FT_TRACE3(( "    Load POST fragment #%d (%lu byte) to buffer"
                  " %p + 0x%08lx\n",
                  i, rlen, (শূন্য*)pfb_তথ্য, pfb_pos ));

      error = FT_Stream_Read( stream, (FT_Byte *)pfb_তথ্য + pfb_pos, rlen );
      যদি ( error )
        যাও Exit2;

      pfb_pos += rlen;
    }

    error = FT_ERR( Array_Too_Large );

    যদি ( pfb_pos + 2 > pfb_len + 2 )
      যাও Exit2;
    pfb_তথ্য[pfb_pos++] = 0x80;
    pfb_তথ্য[pfb_pos++] = 3;

    যদি ( pfb_lenpos + 3 > pfb_len + 2 )
      যাও Exit2;
    pfb_তথ্য[pfb_lenpos    ] = (FT_Byte)( len );
    pfb_তথ্য[pfb_lenpos + 1] = (FT_Byte)( len >> 8 );
    pfb_তথ্য[pfb_lenpos + 2] = (FT_Byte)( len >> 16 );
    pfb_তথ্য[pfb_lenpos + 3] = (FT_Byte)( len >> 24 );

    ফেরত open_face_from_বাফার( library,
                                  pfb_তথ্য,
                                  pfb_pos,
                                  face_সূচক,
                                  "type1",
                                  aface );

  Exit2:
    যদি ( FT_ERR_EQ( error, Array_Too_Large ) )
      FT_TRACE2(( "  Abort due to too-short buffer to store"
                  " all POST fragments\n" ));
    অন্যথা যদি ( FT_ERR_EQ( error, Invalid_Offset ) )
      FT_TRACE2(( "  Abort due to invalid offset in a POST fragment\n" ));

    যদি ( error )
      error = FT_ERR( Cannot_Open_Resource );
    FT_FREE( pfb_তথ্য );

  Exit:
    ফেরত error;
  }


  /* The resource header says we've got resource_cnt `sfnt'      */
  /* (TrueType/OpenType) resources in this file.  Look through   */
  /* them for the one indicated by face_index, load it into mem, */
  /* pass it on to the truetype driver, and return it.           */
  /*                                                             */
  স্থির FT_Error
  Mac_Read_sfnt_Resource( FT_Library  library,
                          FT_Stream   stream,
                          FT_Long    *offsets,
                          FT_Long     resource_cnt,
                          FT_Long     face_সূচক,
                          FT_Face    *aface )
  {
    FT_Memory  memory = library->memory;
    FT_Byte*   sfnt_তথ্য = ফাঁকা;
    FT_Error   error;
    FT_ULong   flag_offset;
    FT_ULong   rlen;
    পূর্ণসংখ্যা        is_cff;
    FT_Long    face_সূচক_in_resource = 0;


    যদি ( face_সূচক < 0 )
      face_সূচক = -face_সূচক - 1;
    যদি ( face_সূচক >= resource_cnt )
      ফেরত FT_THROW( Cannot_Open_Resource );

    flag_offset = (FT_ULong)offsets[face_সূচক];
    error = FT_Stream_Seek( stream, flag_offset );
    যদি ( error )
      যাও Exit;

    যদি ( FT_READ_ULONG( rlen ) )
      যাও Exit;
    যদি ( !rlen )
      ফেরত FT_THROW( Cannot_Open_Resource );
    যদি ( rlen > FT_MAC_RFORK_MAX_LEN )
      ফেরত FT_THROW( Invalid_Offset );

    error = open_face_PS_from_sfnt_stream( library,
                                           stream,
                                           face_সূচক,
                                           0, ফাঁকা,
                                           aface );
    যদি ( !error )
      যাও Exit;

    /* rewind sfnt stream before open_face_PS_from_sfnt_stream() */
    error = FT_Stream_Seek( stream, flag_offset + 4 );
    যদি ( error )
      যাও Exit;

    যদি ( FT_QALLOC( sfnt_তথ্য, rlen ) )
      ফেরত error;
    error = FT_Stream_Read( stream, (FT_Byte *)sfnt_তথ্য, rlen );
    যদি ( error )
    {
      FT_FREE( sfnt_তথ্য );
      যাও Exit;
    }

    is_cff = rlen > 4 && !ft_memcmp( sfnt_তথ্য, "OTTO", 4 );
    error = open_face_from_বাফার( library,
                                   sfnt_তথ্য,
                                   rlen,
                                   face_সূচক_in_resource,
                                   is_cff ? "cff" : "truetype",
                                   aface );

  Exit:
    ফেরত error;
  }


  /* Check for a valid resource fork header, or a valid dfont    */
  /* header.  In a resource fork the first 16 bytes are repeated */
  /* at the location specified by bytes 4-7.  In a dfont bytes   */
  /* 4-7 point to 16 bytes of zeroes instead.                    */
  /*                                                             */
  স্থির FT_Error
  IsMacResource( FT_Library  library,
                 FT_Stream   stream,
                 FT_Long     resource_offset,
                 FT_Long     face_সূচক,
                 FT_Face    *aface )
  {
    FT_Memory  memory = library->memory;
    FT_Error   error;
    FT_Long    map_offset, rdata_pos;
    FT_Long    *তথ্য_offsets;
    FT_Long    গুনতি;


    error = FT_Raccess_Get_HeaderInfo( library, stream, resource_offset,
                                       &map_offset, &rdata_pos );
    যদি ( error )
      ফেরত error;

    /* POST resources must be sorted to concatenate properly */
    error = FT_Raccess_Get_তথ্য_Offsets( library, stream,
                                        map_offset, rdata_pos,
                                        TTAG_POST, TRUE,
                                        &তথ্য_offsets, &গুনতি );
    যদি ( !error )
    {
      error = Mac_Read_POST_Resource( library, stream, তথ্য_offsets, গুনতি,
                                      face_সূচক, aface );
      FT_FREE( তথ্য_offsets );
      /* POST exists in an LWFN providing a single face */
      যদি ( !error )
        (*aface)->অঙ্ক_faces = 1;
      ফেরত error;
    }

    /* sfnt resources should not be sorted to preserve the face order by
       QuickDraw API */
    error = FT_Raccess_Get_তথ্য_Offsets( library, stream,
                                        map_offset, rdata_pos,
                                        TTAG_sfnt, FALSE,
                                        &তথ্য_offsets, &গুনতি );
    যদি ( !error )
    {
      FT_Long  face_সূচক_internal = face_সূচক % গুনতি;


      error = Mac_Read_sfnt_Resource( library, stream, তথ্য_offsets, গুনতি,
                                      face_সূচক_internal, aface );
      FT_FREE( তথ্য_offsets );
      যদি ( !error )
      {
        (*aface)->অঙ্ক_faces  = গুনতি;
        (*aface)->face_সূচক = face_সূচক_internal;
      }
    }

    ফেরত error;
  }


  /* Check for a valid macbinary header, and if we find one   */
  /* check that the (flattened) resource fork in it is valid. */
  /*                                                          */
  স্থির FT_Error
  Is_Mac_দ্বিমিক( FT_Library  library,
               FT_Stream   stream,
               FT_Long     face_সূচক,
               FT_Face    *aface )
  {
    unsigned অক্ষর  header[128];
    FT_Error       error;
    FT_Long        dlen, offset;


    যদি ( !stream )
      ফেরত FT_THROW( Invalid_Stream_Operation );

    error = FT_Stream_Seek( stream, 0 );
    যদি ( error )
      যাও Exit;

    error = FT_Stream_Read( stream, (FT_Byte*)header, 128 );
    যদি ( error )
      যাও Exit;

    যদি (            header[ 0] !=   0 ||
                    header[74] !=   0 ||
                    header[82] !=   0 ||
                    header[ 1] ==   0 ||
                    header[ 1] >   33 ||
                    header[63] !=   0 ||
         header[2 + header[1]] !=   0 ||
                  header[0x53] > 0x7F )
      ফেরত FT_THROW( Unknown_File_Format );

    dlen = ( header[0x53] << 24 ) |
           ( header[0x54] << 16 ) |
           ( header[0x55] <<  8 ) |
             header[0x56];
#যদি 0
    rlen = ( header[0x57] << 24 ) |
           ( header[0x58] << 16 ) |
           ( header[0x59] <<  8 ) |
             header[0x5A];
#endif /* 0 */
    offset = 128 + ( ( dlen + 127 ) & ~127 );

    ফেরত IsMacResource( library, stream, offset, face_সূচক, aface );

  Exit:
    ফেরত error;
  }


  স্থির FT_Error
  load_face_in_embedded_rfork( FT_Library           library,
                               FT_Stream            stream,
                               FT_Long              face_সূচক,
                               FT_Face             *aface,
                               ধ্রুবক FT_Open_Args  *args )
  {

#undef  FT_COMPONENT
#define FT_COMPONENT  raccess

    FT_Memory  memory = library->memory;
    FT_Error   error  = FT_ERR( Unknown_File_Format );
    FT_UInt    i;

    অক্ষর*      file_names[FT_RACCESS_N_RULES];
    FT_Long    offsets[FT_RACCESS_N_RULES];
    FT_Error   errors[FT_RACCESS_N_RULES];
    FT_Bool    is_darwin_vfs, vfs_rfork_has_no_font = FALSE; /* not tested */

    FT_Open_Args  args2;
    FT_Stream     stream2 = ফাঁকা;


    FT_Raccess_Guess( library, stream,
                      args->pathname, file_names, offsets, errors );

    জন্য ( i = 0; i < FT_RACCESS_N_RULES; i++ )
    {
      is_darwin_vfs = ft_raccess_rule_by_darwin_vfs( library, i );
      যদি ( is_darwin_vfs && vfs_rfork_has_no_font )
      {
        FT_TRACE3(( "Skip rule %u: darwin vfs resource fork"
                    " is already checked and"
                    " no font is found\n",
                    i ));
        চালু;
      }

      যদি ( errors[i] )
      {
        FT_TRACE3(( "Error 0x%x has occurred in rule %u\n",
                    errors[i], i ));
        চালু;
      }

      args2.flags    = FT_OPEN_PATHNAME;
      args2.pathname = file_names[i] ? file_names[i] : args->pathname;

      FT_TRACE3(( "Try rule %u: %s (offset=%ld) ...",
                  i, args2.pathname, offsets[i] ));

      error = FT_Stream_New( library, &args2, &stream2 );
      যদি ( is_darwin_vfs && FT_ERR_EQ( error, Cannot_Open_Stream ) )
        vfs_rfork_has_no_font = TRUE;

      যদি ( error )
      {
        FT_TRACE3(( "failed\n" ));
        চালু;
      }

      error = IsMacResource( library, stream2, offsets[i],
                             face_সূচক, aface );
      FT_Stream_Free( stream2, 0 );

      FT_TRACE3(( "%s\n", error ? "failed": "successful" ));

      যদি ( !error )
          বিরতি;
      অন্যথা যদি ( is_darwin_vfs )
          vfs_rfork_has_no_font = TRUE;
    }

    জন্য (i = 0; i < FT_RACCESS_N_RULES; i++)
    {
      যদি ( file_names[i] )
        FT_FREE( file_names[i] );
    }

    /* Caller (load_mac_face) requires FT_Err_Unknown_File_Format. */
    যদি ( error )
      error = FT_ERR( Unknown_File_Format );

    ফেরত error;

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
  স্থির FT_Error
  load_mac_face( FT_Library           library,
                 FT_Stream            stream,
                 FT_Long              face_সূচক,
                 FT_Face             *aface,
                 ধ্রুবক FT_Open_Args  *args )
  {
    FT_Error error;
    FT_UNUSED( args );


    error = Is_Mac_দ্বিমিক( library, stream, face_সূচক, aface );
    যদি ( FT_ERR_EQ( error, Unknown_File_Format ) )
    {

#undef  FT_COMPONENT
#define FT_COMPONENT  raccess

#ifdef FT_DEBUG_LEVEL_TRACE
      FT_TRACE3(( "Try as dfont: " ));
      যদি ( !( args->flags & FT_OPEN_MEMORY ) )
        FT_TRACE3(( "%s ...", args->pathname ));
#endif

      error = IsMacResource( library, stream, 0, face_সূচক, aface );

      FT_TRACE3(( "%s\n", error ? "failed" : "successful" ));

#undef  FT_COMPONENT
#define FT_COMPONENT  objs

    }

    যদি ( ( FT_ERR_EQ( error, Unknown_File_Format )      ||
           FT_ERR_EQ( error, Invalid_Stream_Operation ) ) &&
         ( args->flags & FT_OPEN_PATHNAME )               )
      error = load_face_in_embedded_rfork( library, stream,
                                           face_সূচক, aface, args );
    ফেরত error;
  }
#endif

#endif  /* !FT_MACINTOSH && FT_CONFIG_OPTION_MAC_FONTS */


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Open_Face( FT_Library           library,
                ধ্রুবক FT_Open_Args*  args,
                FT_Long              face_সূচক,
                FT_Face             *aface )
  {
    ফেরত ft_open_face_internal( library, args, face_সূচক, aface, 1 );
  }


  স্থির FT_Error
  ft_open_face_internal( FT_Library           library,
                         ধ্রুবক FT_Open_Args*  args,
                         FT_Long              face_সূচক,
                         FT_Face             *aface,
                         FT_Bool              test_mac_fonts )
  {
    FT_Error     error;
    FT_Driver    driver = ফাঁকা;
    FT_Memory    memory = ফাঁকা;
    FT_Stream    stream = ফাঁকা;
    FT_Face      face   = ফাঁকা;
    FT_List_গ্রন্থি  গ্রন্থি   = ফাঁকা;
    FT_Bool      external_stream;
    FT_Module*   cur;
    FT_Module*   limit;

#ifndef FT_CONFIG_OPTION_MAC_FONTS
    FT_UNUSED( test_mac_fonts );
#endif


    /* only use lower 31 bits together with sign bit */
    যদি ( face_সূচক > 0 )
      face_সূচক &= 0x7FFFFFFFL;
    অন্যথা
    {
      face_সূচক  = -face_সূচক;
      face_সূচক &= 0x7FFFFFFFL;
      face_সূচক  = -face_সূচক;
    }

#ifdef FT_DEBUG_LEVEL_TRACE
    FT_TRACE3(( "FT_Open_Face: " ));
    যদি ( face_সূচক < 0 )
      FT_TRACE3(( "Requesting number of faces and named instances\n"));
    অন্যথা
    {
      FT_TRACE3(( "Requesting face %ld", face_সূচক & 0xFFFFL ));
      যদি ( face_সূচক & 0x7FFF0000L )
        FT_TRACE3(( ", named instance %ld", face_সূচক >> 16 ));
      FT_TRACE3(( "\n" ));
    }
#endif

    /* test for valid `library' delayed to `FT_Stream_New' */

    যদি ( !args )
      ফেরত FT_THROW( Invalid_Argument );

    external_stream = FT_BOOL( ( args->flags & FT_OPEN_STREAM ) &&
                               args->stream                     );

    /* create input stream */
    error = FT_Stream_New( library, args, &stream );
    যদি ( error )
      যাও Fail3;

    /* Do this error check after `FT_Stream_New` to ensure that the */
    /* 'close' callback is called.                                  */
    যদি ( !aface && face_সূচক >= 0 )
    {
      error = FT_THROW( Invalid_Argument );
      যাও Fail3;
    }

    memory = library->memory;

    /* If the font driver is specified in the `args' structure, use */
    /* it.  Otherwise, we scan the list of registered drivers.      */
    যদি ( ( args->flags & FT_OPEN_DRIVER ) && args->driver )
    {
      driver = FT_DRIVER( args->driver );

      /* not all modules are drivers, so check... */
      যদি ( FT_MODULE_IS_DRIVER( driver ) )
      {
        FT_Int         অঙ্ক_params = 0;
        FT_Parameter*  params     = ফাঁকা;


        যদি ( args->flags & FT_OPEN_PARAMS )
        {
          অঙ্ক_params = args->অঙ্ক_params;
          params     = args->params;
        }

        error = open_face( driver, &stream, &external_stream, face_সূচক,
                           অঙ্ক_params, params, &face );
        যদি ( !error )
          যাও Success;
      }
      অন্যথা
        error = FT_THROW( Invalid_Handle );

      FT_Stream_Free( stream, external_stream );
      যাও Fail;
    }
    অন্যথা
    {
      error = FT_ERR( Missing_Module );

      /* check each font driver for an appropriate format */
      cur   = library->modules;
      limit = cur + library->অঙ্ক_modules;

      জন্য ( ; cur < limit; cur++ )
      {
        /* not all modules are font drivers, so check... */
        যদি ( FT_MODULE_IS_DRIVER( cur[0] ) )
        {
          FT_Int         অঙ্ক_params = 0;
          FT_Parameter*  params     = ফাঁকা;


          driver = FT_DRIVER( cur[0] );

          যদি ( args->flags & FT_OPEN_PARAMS )
          {
            অঙ্ক_params = args->অঙ্ক_params;
            params     = args->params;
          }

          error = open_face( driver, &stream, &external_stream, face_সূচক,
                             অঙ্ক_params, params, &face );
          যদি ( !error )
            যাও Success;

#ifdef FT_CONFIG_OPTION_MAC_FONTS
          যদি ( test_mac_fonts                                           &&
               ft_strcmp( cur[0]->clazz->module_name, "truetype" ) == 0 &&
               FT_ERR_EQ( error, Table_Missing )                        )
          {
            /* TrueType but essential tables are missing */
            error = FT_Stream_Seek( stream, 0 );
            যদি ( error )
              বিরতি;

            error = open_face_PS_from_sfnt_stream( library,
                                                   stream,
                                                   face_সূচক,
                                                   অঙ্ক_params,
                                                   params,
                                                   aface );
            যদি ( !error )
            {
              FT_Stream_Free( stream, external_stream );
              ফেরত error;
            }
          }
#endif

          যদি ( FT_ERR_NEQ( error, Unknown_File_Format ) )
            যাও Fail3;
        }
      }

    Fail3:
      /* If we are on the mac, and we get an                          */
      /* FT_Err_Invalid_Stream_Operation it may be because we have an */
      /* empty data fork, so we need to check the resource fork.      */
      যদি ( FT_ERR_NEQ( error, Cannot_Open_Stream )       &&
           FT_ERR_NEQ( error, Unknown_File_Format )      &&
           FT_ERR_NEQ( error, Invalid_Stream_Operation ) )
        যাও Fail2;

#যদি !defined( FT_MACINTOSH ) && defined( FT_CONFIG_OPTION_MAC_FONTS )
      যদি ( test_mac_fonts )
      {
        error = load_mac_face( library, stream, face_সূচক, aface, args );
        যদি ( !error )
        {
          /* We don't want to go to Success here.  We've already done   */
          /* that.  On the other hand, if we succeeded we still need to */
          /* close this stream (we opened a different stream which      */
          /* extracted the interesting information out of this stream   */
          /* here.  That stream will still be open and the face will    */
          /* point to it).                                              */
          FT_Stream_Free( stream, external_stream );
          ফেরত error;
        }
      }

      যদি ( FT_ERR_NEQ( error, Unknown_File_Format ) )
        যাও Fail2;
#endif  /* !FT_MACINTOSH && FT_CONFIG_OPTION_MAC_FONTS */

      /* no driver is able to handle this format */
      error = FT_THROW( Unknown_File_Format );

  Fail2:
      FT_Stream_Free( stream, external_stream );
      যাও Fail;
    }

  Success:
    FT_TRACE4(( "FT_Open_Face: New face object, adding to list\n" ));

    /* add the face object to its driver's list */
    যদি ( FT_QNEW( গ্রন্থি ) )
      যাও Fail;

    গ্রন্থি->তথ্য = face;
    /* don't assume driver is the same as face->driver, so use */
    /* face->driver instead.                                   */
    FT_List_Add( &face->driver->faces_list, গ্রন্থি );

    /* now allocate a glyph slot object for the face */
    FT_TRACE4(( "FT_Open_Face: Creating glyph slot\n" ));

    যদি ( face_সূচক >= 0 )
    {
      error = FT_New_GlyphSlot( face, ফাঁকা );
      যদি ( error )
        যাও Fail;

      /* finally, allocate a size object for the face */
      {
        FT_আকার  আকার_2;


        FT_TRACE4(( "FT_Open_Face: Creating size object\n" ));

        error = FT_New_আকার( face, &আকার_2 );
        যদি ( error )
          যাও Fail;

        face->আকার_2 = আকার_2;
      }
    }

    /* some checks */

    যদি ( FT_IS_SCALABLE( face ) )
    {
      যদি ( face->height < 0 )
        face->height = (FT_Short)-face->height;

      যদি ( !FT_HAS_VERTICAL( face ) )
        face->max_advance_height = (FT_Short)face->height;
    }

    যদি ( FT_HAS_FIXED_SIZES( face ) )
    {
      FT_Int  i;


      জন্য ( i = 0; i < face->অঙ্ক_fixed_sizes; i++ )
      {
        FT_Bitmap_আকার*  bsize = face->available_sizes + i;


        যদি ( bsize->height < 0 )
          bsize->height = -bsize->height;
        যদি ( bsize->x_ppem < 0 )
          bsize->x_ppem = -bsize->x_ppem;
        যদি ( bsize->y_ppem < 0 )
          bsize->y_ppem = -bsize->y_ppem;

        /* check whether negation actually has worked */
        যদি ( bsize->height < 0 || bsize->x_ppem < 0 || bsize->y_ppem < 0 )
        {
          FT_TRACE0(( "FT_Open_Face:"
                      " Invalid bitmap dimensions for strike %d,"
                      " now disabled\n", i ));
          bsize->width  = 0;
          bsize->height = 0;
          bsize->আকার_2   = 0;
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

    যদি ( aface )
      *aface = face;
    অন্যথা
      FT_Done_Face( face );

    যাও Exit;

  Fail:
    যদি ( গ্রন্থি )
      FT_Done_Face( face );    /* face must be in the driver's list */
    অন্যথা যদি ( face )
      destroy_face( memory, face, driver );

  Exit:
#ifdef FT_DEBUG_LEVEL_TRACE
    যদি ( !error && face_সূচক < 0 )
    {
      FT_TRACE3(( "FT_Open_Face: The font has %ld face%s\n",
                  face->অঙ্ক_faces,
                  face->অঙ্ক_faces == 1 ? "" : "s" ));
      FT_TRACE3(( "              and %ld named instance%s for face %ld\n",
                  face->style_flags >> 16,
                  ( face->style_flags >> 16 ) == 1 ? "" : "s",
                  -face_সূচক - 1 ));
    }
#endif

    FT_TRACE4(( "FT_Open_Face: Return 0x%x\n", error ));

    ফেরত error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Attach_File( FT_Face      face,
                  ধ্রুবক অক্ষর*  filepathname )
  {
    FT_Open_Args  open;


    /* test for valid `face' delayed to `FT_Attach_Stream' */

    যদি ( !filepathname )
      ফেরত FT_THROW( Invalid_Argument );

    open.stream   = ফাঁকা;
    open.flags    = FT_OPEN_PATHNAME;
    open.pathname = (অক্ষর*)filepathname;

    ফেরত FT_Attach_Stream( face, &open );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Attach_Stream( FT_Face              face,
                    ধ্রুবক FT_Open_Args*  parameters )
  {
    FT_Stream  stream;
    FT_Error   error;
    FT_Driver  driver;

    FT_Driver_Class  clazz;


    /* test for valid `parameters' delayed to `FT_Stream_New' */

    যদি ( !face )
      ফেরত FT_THROW( Invalid_Face_Handle );

    driver = face->driver;
    যদি ( !driver )
      ফেরত FT_THROW( Invalid_Driver_Handle );

    error = FT_Stream_New( driver->মূল.library, parameters, &stream );
    যদি ( error )
      যাও Exit;

    /* we implement FT_Attach_Stream in each driver through the */
    /* `attach_file' interface                                  */

    error = FT_ERR( Unimplemented_Feature );
    clazz = driver->clazz;
    যদি ( clazz->attach_file )
      error = clazz->attach_file( face, stream );

    /* close the attached stream */
    FT_Stream_Free( stream,
                    FT_BOOL( parameters->stream                     &&
                             ( parameters->flags & FT_OPEN_STREAM ) ) );

  Exit:
    ফেরত error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Reference_Face( FT_Face  face )
  {
    যদি ( !face )
      ফেরত FT_THROW( Invalid_Face_Handle );

    face->internal->refcount++;

    ফেরত FT_Err_Ok;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Done_Face( FT_Face  face )
  {
    FT_Error     error;
    FT_Driver    driver;
    FT_Memory    memory;
    FT_List_গ্রন্থি  গ্রন্থি;


    error = FT_ERR( Invalid_Face_Handle );
    যদি ( face && face->driver )
    {
      face->internal->refcount--;
      যদি ( face->internal->refcount > 0 )
        error = FT_Err_Ok;
      অন্যথা
      {
        driver = face->driver;
        memory = driver->মূল.memory;

        /* find face in driver's list */
        গ্রন্থি = FT_List_Find( &driver->faces_list, face );
        যদি ( গ্রন্থি )
        {
          /* remove face object from the driver's list */
          FT_List_Remove( &driver->faces_list, গ্রন্থি );
          FT_FREE( গ্রন্থি );

          /* now destroy the object proper */
          destroy_face( memory, face, driver );
          error = FT_Err_Ok;
        }
      }
    }

    ফেরত error;
  }


  /* documentation is in ftobjs.h */

  FT_EXPORT_DEF( FT_Error )
  FT_New_আকার( FT_Face   face,
               FT_আকার  *asize )
  {
    FT_Error         error;
    FT_Memory        memory;
    FT_Driver        driver;
    FT_Driver_Class  clazz;

    FT_আকার          আকার_2 = ফাঁকা;
    FT_List_গ্রন্থি      গ্রন্থি = ফাঁকা;

    FT_আকার_Internal  internal = ফাঁকা;


    যদি ( !face )
      ফেরত FT_THROW( Invalid_Face_Handle );

    যদি ( !asize )
      ফেরত FT_THROW( Invalid_Argument );

    যদি ( !face->driver )
      ফেরত FT_THROW( Invalid_Driver_Handle );

    *asize = ফাঁকা;

    driver = face->driver;
    clazz  = driver->clazz;
    memory = face->memory;

    /* Allocate new size object and perform basic initialisation */
    যদি ( FT_ALLOC( আকার_2, clazz->আকার_object_আকার ) || FT_QNEW( গ্রন্থি ) )
      যাও Exit;

    আকার_2->face = face;

    যদি ( FT_NEW( internal ) )
      যাও Exit;

    আকার_2->internal = internal;

    যদি ( clazz->init_আকার )
      error = clazz->init_আকার( আকার_2 );

    /* in case of success, add to the face's list */
    যদি ( !error )
    {
      *asize     = আকার_2;
      গ্রন্থি->তথ্য = আকার_2;
      FT_List_Add( &face->sizes_list, গ্রন্থি );
    }

  Exit:
    যদি ( error )
    {
      FT_FREE( গ্রন্থি );
      যদি ( আকার_2 )
        FT_FREE( আকার_2->internal );
      FT_FREE( আকার_2 );
    }

    ফেরত error;
  }


  /* documentation is in ftobjs.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Done_আকার( FT_আকার  আকার_2 )
  {
    FT_Error     error;
    FT_Driver    driver;
    FT_Memory    memory;
    FT_Face      face;
    FT_List_গ্রন্থি  গ্রন্থি;


    যদি ( !আকার_2 )
      ফেরত FT_THROW( Invalid_আকার_Handle );

    face = আকার_2->face;
    যদি ( !face )
      ফেরত FT_THROW( Invalid_Face_Handle );

    driver = face->driver;
    যদি ( !driver )
      ফেরত FT_THROW( Invalid_Driver_Handle );

    memory = driver->মূল.memory;

    error = FT_Err_Ok;
    গ্রন্থি  = FT_List_Find( &face->sizes_list, আকার_2 );
    যদি ( গ্রন্থি )
    {
      FT_List_Remove( &face->sizes_list, গ্রন্থি );
      FT_FREE( গ্রন্থি );

      যদি ( face->আকার_2 == আকার_2 )
      {
        face->আকার_2 = ফাঁকা;
        যদি ( face->sizes_list.শীর্ষ_2 )
          face->আকার_2 = (FT_আকার)(face->sizes_list.শীর্ষ_2->তথ্য);
      }

      destroy_আকার( memory, আকার_2, driver );
    }
    অন্যথা
      error = FT_THROW( Invalid_আকার_Handle );

    ফেরত error;
  }


  /* documentation is in ftobjs.h */

  FT_BASE_DEF( FT_Error )
  FT_Match_আকার( FT_Face          face,
                 FT_আকার_Request  req,
                 FT_Bool          ignore_width,
                 FT_ULong*        আকার_সূচক )
  {
    FT_Int   i;
    FT_Long  w, h;


    যদি ( !FT_HAS_FIXED_SIZES( face ) )
      ফেরত FT_THROW( Invalid_Face_Handle );

    /* FT_Bitmap_Size doesn't provide enough info... */
    যদি ( req->type != FT_আকার_REQUEST_TYPE_NOMINAL )
      ফেরত FT_THROW( Unimplemented_Feature );

    w = FT_REQUEST_WIDTH ( req );
    h = FT_REQUEST_HEIGHT( req );

    যদি ( req->width && !req->height )
      h = w;
    অন্যথা যদি ( !req->width && req->height )
      w = h;

    w = FT_PIX_ROUND( w );
    h = FT_PIX_ROUND( h );

    যদি ( !w || !h )
      ফেরত FT_THROW( Invalid_Pixel_আকার );

    জন্য ( i = 0; i < face->অঙ্ক_fixed_sizes; i++ )
    {
      FT_Bitmap_আকার*  bsize = face->available_sizes + i;


      যদি ( h != FT_PIX_ROUND( bsize->y_ppem ) )
        চালু;

      যদি ( w == FT_PIX_ROUND( bsize->x_ppem ) || ignore_width )
      {
        FT_TRACE3(( "FT_Match_Size: bitmap strike %d matches\n", i ));

        যদি ( আকার_সূচক )
          *আকার_সূচক = (FT_ULong)i;

        ফেরত FT_Err_Ok;
      }
    }

    FT_TRACE3(( "FT_Match_Size: no matching bitmap strike\n" ));

    ফেরত FT_THROW( Invalid_Pixel_আকার );
  }


  /* documentation is in ftobjs.h */

  FT_BASE_DEF( শূন্য )
  ft_synthesize_vertical_metrics( FT_Glyph_Metrics*  metrics,
                                  FT_Pos             advance )
  {
    FT_Pos  height = metrics->height;


    /* compensate for glyph with bbox above/below the baseline */
    যদি ( metrics->horiBearingY < 0 )
    {
      যদি ( height < metrics->horiBearingY )
        height = metrics->horiBearingY;
    }
    অন্যথা যদি ( metrics->horiBearingY > 0 )
      height -= metrics->horiBearingY;

    /* the factor 1.2 is a heuristical value */
    যদি ( !advance )
      advance = height * 12 / 10;

    metrics->vertBearingX = metrics->horiBearingX - metrics->horiAdvance / 2;
    metrics->vertBearingY = ( advance - height ) / 2;
    metrics->vertAdvance  = advance;
  }


  স্থির শূন্য
  ft_recompute_scaled_metrics( FT_Face           face,
                               FT_আকার_Metrics*  metrics )
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
#অন্যথা /* !GRID_FIT_METRICS */
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


  FT_BASE_DEF( শূন্য )
  FT_Select_Metrics( FT_Face   face,
                     FT_ULong  strike_সূচক )
  {
    FT_আকার_Metrics*  metrics;
    FT_Bitmap_আকার*   bsize;


    metrics = &face->আকার_2->metrics;
    bsize   = face->available_sizes + strike_সূচক;

    metrics->x_ppem = (FT_UShort)( ( bsize->x_ppem + 32 ) >> 6 );
    metrics->y_ppem = (FT_UShort)( ( bsize->y_ppem + 32 ) >> 6 );

    যদি ( FT_IS_SCALABLE( face ) )
    {
      metrics->x_scale = FT_DivFix( bsize->x_ppem,
                                    face->units_per_EM );
      metrics->y_scale = FT_DivFix( bsize->y_ppem,
                                    face->units_per_EM );

      ft_recompute_scaled_metrics( face, metrics );
    }
    অন্যথা
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
                      FT_আকার_Request  req )
  {
    FT_Error  error = FT_Err_Ok;

    FT_আকার_Metrics*  metrics;


    metrics = &face->আকার_2->metrics;

    যদি ( FT_IS_SCALABLE( face ) )
    {
      FT_Long  w = 0, h = 0, scaled_w = 0, scaled_h = 0;


      নির্বাচন ( req->type )
      {
      ক্ষেত্র FT_আকার_REQUEST_TYPE_NOMINAL:
        w = h = face->units_per_EM;
        বিরতি;

      ক্ষেত্র FT_আকার_REQUEST_TYPE_REAL_DIM:
        w = h = face->ascender - face->descender;
        বিরতি;

      ক্ষেত্র FT_আকার_REQUEST_TYPE_BBOX:
        w = face->bbox.xMax - face->bbox.xMin;
        h = face->bbox.yMax - face->bbox.yMin;
        বিরতি;

      ক্ষেত্র FT_আকার_REQUEST_TYPE_CELL:
        w = face->max_advance_width;
        h = face->ascender - face->descender;
        বিরতি;

      ক্ষেত্র FT_আকার_REQUEST_TYPE_SCALES:
        metrics->x_scale = (FT_Fixed)req->width;
        metrics->y_scale = (FT_Fixed)req->height;
        যদি ( !metrics->x_scale )
          metrics->x_scale = metrics->y_scale;
        অন্যথা যদি ( !metrics->y_scale )
          metrics->y_scale = metrics->x_scale;
        যাও Calculate_Ppem;

      ক্ষেত্র FT_আকার_REQUEST_TYPE_MAX:
        বিরতি;
      }

      /* to be on the safe side */
      যদি ( w < 0 )
        w = -w;

      যদি ( h < 0 )
        h = -h;

      scaled_w = FT_REQUEST_WIDTH ( req );
      scaled_h = FT_REQUEST_HEIGHT( req );

      /* determine scales */
      যদি ( req->height || !req->width )
      {
        যদি ( h == 0 )
        {
          FT_ERROR(( "FT_Request_Metrics: Divide by zero\n" ));
          error = FT_ERR( Divide_By_Zero );
          যাও Exit;
        }

        metrics->y_scale = FT_DivFix( scaled_h, h );
      }

      যদি ( req->width )
      {
        যদি ( w == 0 )
        {
          FT_ERROR(( "FT_Request_Metrics: Divide by zero\n" ));
          error = FT_ERR( Divide_By_Zero );
          যাও Exit;
        }

        metrics->x_scale = FT_DivFix( scaled_w, w );
      }
      অন্যথা
      {
        metrics->x_scale = metrics->y_scale;
        scaled_w = FT_MulDiv( scaled_h, w, h );
      }

      যদি ( !req->height )
      {
        metrics->y_scale = metrics->x_scale;
        scaled_h = FT_MulDiv( scaled_w, h, w );
      }

      যদি ( req->type == FT_আকার_REQUEST_TYPE_CELL )
      {
        যদি ( metrics->y_scale > metrics->x_scale )
          metrics->y_scale = metrics->x_scale;
        অন্যথা
          metrics->x_scale = metrics->y_scale;
      }

  Calculate_Ppem:
      /* calculate the ppems */
      যদি ( req->type != FT_আকার_REQUEST_TYPE_NOMINAL )
      {
        scaled_w = FT_MulFix( face->units_per_EM, metrics->x_scale );
        scaled_h = FT_MulFix( face->units_per_EM, metrics->y_scale );
      }

      scaled_w = ( scaled_w + 32 ) >> 6;
      scaled_h = ( scaled_h + 32 ) >> 6;
      যদি ( scaled_w > (FT_Long)FT_USHORT_MAX ||
           scaled_h > (FT_Long)FT_USHORT_MAX )
      {
        FT_ERROR(( "FT_Request_Metrics: Resulting ppem size too large\n" ));
        error = FT_ERR( Invalid_Pixel_আকার );
        যাও Exit;
      }

      metrics->x_ppem = (FT_UShort)scaled_w;
      metrics->y_ppem = (FT_UShort)scaled_h;

      ft_recompute_scaled_metrics( face, metrics );
    }
    অন্যথা
    {
      FT_ZERO( metrics );
      metrics->x_scale = 1L << 16;
      metrics->y_scale = 1L << 16;
    }

  Exit:
    ফেরত error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Select_আকার( FT_Face  face,
                  FT_Int   strike_সূচক )
  {
    FT_Error         error = FT_Err_Ok;
    FT_Driver_Class  clazz;


    যদি ( !face || !FT_HAS_FIXED_SIZES( face ) )
      ফেরত FT_THROW( Invalid_Face_Handle );

    যদি ( strike_সূচক < 0 || strike_সূচক >= face->অঙ্ক_fixed_sizes )
      ফেরত FT_THROW( Invalid_Argument );

    clazz = face->driver->clazz;

    যদি ( clazz->select_আকার )
    {
      error = clazz->select_আকার( face->আকার_2, (FT_ULong)strike_সূচক );

      FT_TRACE5(( "FT_Select_Size (%s driver):\n",
                  face->driver->মূল.clazz->module_name ));
    }
    অন্যথা
    {
      FT_Select_Metrics( face, (FT_ULong)strike_সূচক );

      FT_TRACE5(( "FT_Select_Size:\n" ));
    }

#ifdef FT_DEBUG_LEVEL_TRACE
    {
      FT_আকার_Metrics*  metrics = &face->আকার_2->metrics;


      FT_TRACE5(( "  x scale: %ld (%f)\n",
                  metrics->x_scale, (দ্বিগুণ)metrics->x_scale / 65536 ));
      FT_TRACE5(( "  y scale: %ld (%f)\n",
                  metrics->y_scale, (দ্বিগুণ)metrics->y_scale / 65536 ));
      FT_TRACE5(( "  ascender: %f\n",
                  (দ্বিগুণ)metrics->ascender / 64 ));
      FT_TRACE5(( "  descender: %f\n",
                  (দ্বিগুণ)metrics->descender / 64 ));
      FT_TRACE5(( "  height: %f\n",
                  (দ্বিগুণ)metrics->height / 64 ));
      FT_TRACE5(( "  max advance: %f\n",
                  (দ্বিগুণ)metrics->max_advance / 64 ));
      FT_TRACE5(( "  x ppem: %d\n", metrics->x_ppem ));
      FT_TRACE5(( "  y ppem: %d\n", metrics->y_ppem ));
    }
#endif

    ফেরত error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Request_আকার( FT_Face          face,
                   FT_আকার_Request  req )
  {
    FT_Error         error;
    FT_Driver_Class  clazz;
    FT_ULong         strike_সূচক;


    যদি ( !face )
      ফেরত FT_THROW( Invalid_Face_Handle );

    যদি ( !face->আকার_2 )
      ফেরত FT_THROW( Invalid_আকার_Handle );

    যদি ( !req || req->width < 0 || req->height < 0 ||
         req->type >= FT_আকার_REQUEST_TYPE_MAX )
      ফেরত FT_THROW( Invalid_Argument );

    /* signal the auto-hinter to recompute its size metrics */
    /* (if requested)                                       */
    face->আকার_2->internal->autohint_metrics.x_scale = 0;

    clazz = face->driver->clazz;

    যদি ( clazz->request_আকার )
    {
      error = clazz->request_আকার( face->আকার_2, req );

      FT_TRACE5(( "FT_Request_Size (%s driver):\n",
                  face->driver->মূল.clazz->module_name ));
    }
    অন্যথা যদি ( !FT_IS_SCALABLE( face ) && FT_HAS_FIXED_SIZES( face ) )
    {
      /*
       * The reason that a driver doesn't have `request_size' defined is
       * either that the scaling here suffices or that the supported formats
       * are bitmap-only and size matching is not implemented.
       *
       * In the latter case, a simple size matching is done.
       */
      error = FT_Match_আকার( face, req, 0, &strike_সূচক );
      যদি ( error )
        যাও Exit;

      ফেরত FT_Select_আকার( face, (FT_Int)strike_সূচক );
    }
    অন্যথা
    {
      error = FT_Request_Metrics( face, req );
      যদি ( error )
        যাও Exit;

      FT_TRACE5(( "FT_Request_Size:\n" ));
    }

#ifdef FT_DEBUG_LEVEL_TRACE
    {
      FT_আকার_Metrics*  metrics = &face->আকার_2->metrics;


      FT_TRACE5(( "  x scale: %ld (%f)\n",
                  metrics->x_scale, (দ্বিগুণ)metrics->x_scale / 65536 ));
      FT_TRACE5(( "  y scale: %ld (%f)\n",
                  metrics->y_scale, (দ্বিগুণ)metrics->y_scale / 65536 ));
      FT_TRACE5(( "  ascender: %f\n",
                  (দ্বিগুণ)metrics->ascender / 64 ));
      FT_TRACE5(( "  descender: %f\n",
                  (দ্বিগুণ)metrics->descender / 64 ));
      FT_TRACE5(( "  height: %f\n",
                  (দ্বিগুণ)metrics->height / 64 ));
      FT_TRACE5(( "  max advance: %f\n",
                  (দ্বিগুণ)metrics->max_advance / 64 ));
      FT_TRACE5(( "  x ppem: %d\n", metrics->x_ppem ));
      FT_TRACE5(( "  y ppem: %d\n", metrics->y_ppem ));
    }
#endif

  Exit:
    ফেরত error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Set_Char_আকার( FT_Face     face,
                    FT_F26Dot6  char_width,
                    FT_F26Dot6  char_height,
                    FT_UInt     horz_resolution,
                    FT_UInt     vert_resolution )
  {
    FT_আকার_Request_Rec  req;


    /* check of `face' delayed to `FT_Request_Size' */

    যদি ( !char_width )
      char_width = char_height;
    অন্যথা যদি ( !char_height )
      char_height = char_width;

    যদি ( !horz_resolution )
      horz_resolution = vert_resolution;
    অন্যথা যদি ( !vert_resolution )
      vert_resolution = horz_resolution;

    যদি ( char_width  < 1 * 64 )
      char_width  = 1 * 64;
    যদি ( char_height < 1 * 64 )
      char_height = 1 * 64;

    যদি ( !horz_resolution )
      horz_resolution = vert_resolution = 72;

    req.type           = FT_আকার_REQUEST_TYPE_NOMINAL;
    req.width          = char_width;
    req.height         = char_height;
    req.horiResolution = horz_resolution;
    req.vertResolution = vert_resolution;

    ফেরত FT_Request_আকার( face, &req );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Set_Pixel_Sizes( FT_Face  face,
                      FT_UInt  pixel_width,
                      FT_UInt  pixel_height )
  {
    FT_আকার_Request_Rec  req;


    /* check of `face' delayed to `FT_Request_Size' */

    যদি ( pixel_width == 0 )
      pixel_width = pixel_height;
    অন্যথা যদি ( pixel_height == 0 )
      pixel_height = pixel_width;

    যদি ( pixel_width  < 1 )
      pixel_width  = 1;
    যদি ( pixel_height < 1 )
      pixel_height = 1;

    /* use `>=' to avoid potential compiler warning on 16bit platforms */
    যদি ( pixel_width >= 0xFFFFU )
      pixel_width = 0xFFFFU;
    যদি ( pixel_height >= 0xFFFFU )
      pixel_height = 0xFFFFU;

    req.type           = FT_আকার_REQUEST_TYPE_NOMINAL;
    req.width          = (FT_Long)( pixel_width << 6 );
    req.height         = (FT_Long)( pixel_height << 6 );
    req.horiResolution = 0;
    req.vertResolution = 0;

    ফেরত FT_Request_আকার( face, &req );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Kerning( FT_Face     face,
                  FT_UInt     বাম_glyph,
                  FT_UInt     ডান_glyph,
                  FT_UInt     kern_mode,
                  FT_Vector  *akerning )
  {
    FT_Error   error = FT_Err_Ok;
    FT_Driver  driver;


    যদি ( !face )
      ফেরত FT_THROW( Invalid_Face_Handle );

    যদি ( !akerning )
      ফেরত FT_THROW( Invalid_Argument );

    driver = face->driver;

    akerning->x = 0;
    akerning->y = 0;

    যদি ( driver->clazz->get_kerning )
    {
      error = driver->clazz->get_kerning( face,
                                          বাম_glyph,
                                          ডান_glyph,
                                          akerning );
      যদি ( !error )
      {
        যদি ( kern_mode != FT_KERNING_UNSCALED )
        {
          akerning->x = FT_MulFix( akerning->x, face->আকার_2->metrics.x_scale );
          akerning->y = FT_MulFix( akerning->y, face->আকার_2->metrics.y_scale );

          যদি ( kern_mode != FT_KERNING_UNFITTED )
          {
            FT_Pos  orig_x = akerning->x;
            FT_Pos  orig_y = akerning->y;


            /* we scale down kerning values for small ppem values */
            /* to avoid that rounding makes them too big.         */
            /* `25' has been determined heuristically.            */
            যদি ( face->আকার_2->metrics.x_ppem < 25 )
              akerning->x = FT_MulDiv( orig_x,
                                       face->আকার_2->metrics.x_ppem, 25 );
            যদি ( face->আকার_2->metrics.y_ppem < 25 )
              akerning->y = FT_MulDiv( orig_y,
                                       face->আকার_2->metrics.y_ppem, 25 );

            akerning->x = FT_PIX_ROUND( akerning->x );
            akerning->y = FT_PIX_ROUND( akerning->y );

#ifdef FT_DEBUG_LEVEL_TRACE
            {
              FT_Pos  orig_x_rounded = FT_PIX_ROUND( orig_x );
              FT_Pos  orig_y_rounded = FT_PIX_ROUND( orig_y );


              যদি ( akerning->x != orig_x_rounded ||
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

    ফেরত error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Track_Kerning( FT_Face    face,
                        FT_Fixed   point_আকার,
                        FT_Int     degree,
                        FT_Fixed*  akerning )
  {
    FT_Service_Kerning  service;
    FT_Error            error = FT_Err_Ok;


    যদি ( !face )
      ফেরত FT_THROW( Invalid_Face_Handle );

    যদি ( !akerning )
      ফেরত FT_THROW( Invalid_Argument );

    FT_FACE_FIND_SERVICE( face, service, KERNING );
    যদি ( !service )
      ফেরত FT_THROW( Unimplemented_Feature );

    error = service->get_track( face,
                                point_আকার,
                                degree,
                                akerning );

    ফেরত error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Select_Charmap( FT_Face      face,
                     FT_Encoding  encoding )
  {
    FT_CharMap*  cur;
    FT_CharMap*  limit;


    যদি ( !face )
      ফেরত FT_THROW( Invalid_Face_Handle );

    /* FT_ENCODING_NONE is a valid encoding for BDF, PCF, and Windows FNT */
    যদি ( encoding == FT_ENCODING_NONE && !face->অঙ্ক_charmaps )
      ফেরত FT_THROW( Invalid_Argument );

    /* FT_ENCODING_UNICODE is special.  We try to find the `best' Unicode */
    /* charmap available, i.e., one with UCS-4 characters, if possible.   */
    /*                                                                    */
    /* This is done by find_unicode_charmap() above, to share code.       */
    যদি ( encoding == FT_ENCODING_UNICODE )
      ফেরত find_unicode_charmap( face );

    cur = face->charmaps;
    যদি ( !cur )
      ফেরত FT_THROW( Invalid_CharMap_Handle );

    limit = cur + face->অঙ্ক_charmaps;

    জন্য ( ; cur < limit; cur++ )
    {
      যদি ( cur[0]->encoding == encoding )
      {
        face->charmap = cur[0];
        ফেরত FT_Err_Ok;
      }
    }

    ফেরত FT_THROW( Invalid_Argument );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Set_Charmap( FT_Face     face,
                  FT_CharMap  charmap )
  {
    FT_CharMap*  cur;
    FT_CharMap*  limit;


    যদি ( !face )
      ফেরত FT_THROW( Invalid_Face_Handle );

    cur = face->charmaps;
    যদি ( !cur || !charmap )
      ফেরত FT_THROW( Invalid_CharMap_Handle );

    limit = cur + face->অঙ্ক_charmaps;

    জন্য ( ; cur < limit; cur++ )
    {
      যদি ( cur[0] == charmap )
      {
        FT_Long  format = FT_Get_CMap_Format( charmap );


        যদি ( format != 14 && format != 15 )
        {
          face->charmap = cur[0];
          ফেরত FT_Err_Ok;
        }
      }
    }

    ফেরত FT_THROW( Invalid_Argument );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Int )
  FT_Get_Charmap_সূচক( FT_CharMap  charmap )
  {
    FT_Int  i;


    যদি ( !charmap || !charmap->face )
      ফেরত -1;

    জন্য ( i = 0; i < charmap->face->অঙ্ক_charmaps; i++ )
      যদি ( charmap->face->charmaps[i] == charmap )
        বিরতি;

    FT_ASSERT( i < charmap->face->অঙ্ক_charmaps );

    ফেরত i;
  }


  স্থির শূন্য
  ft_cmap_done_internal( FT_CMap  cmap )
  {
    FT_CMap_Class  clazz  = cmap->clazz;
    FT_Face        face   = cmap->charmap.face;
    FT_Memory      memory = FT_FACE_MEMORY( face );


    যদি ( clazz->done )
      clazz->done( cmap );

    FT_FREE( cmap );
  }


  FT_BASE_DEF( শূন্য )
  FT_CMap_Done( FT_CMap  cmap )
  {
    যদি ( cmap )
    {
      FT_Face    face   = cmap->charmap.face;
      FT_Memory  memory = FT_FACE_MEMORY( face );
      FT_Error   error;
      FT_Int     i, j;


      জন্য ( i = 0; i < face->অঙ্ক_charmaps; i++ )
      {
        যদি ( (FT_CMap)face->charmaps[i] == cmap )
        {
          FT_CharMap  last_charmap = face->charmaps[face->অঙ্ক_charmaps - 1];


          যদি ( FT_QRENEW_ARRAY( face->charmaps,
                                face->অঙ্ক_charmaps,
                                face->অঙ্ক_charmaps - 1 ) )
            ফেরত;

          /* remove it from our list of charmaps */
          জন্য ( j = i + 1; j < face->অঙ্ক_charmaps; j++ )
          {
            যদি ( j == face->অঙ্ক_charmaps - 1 )
              face->charmaps[j - 1] = last_charmap;
            অন্যথা
              face->charmaps[j - 1] = face->charmaps[j];
          }

          face->অঙ্ক_charmaps--;

          যদি ( (FT_CMap)face->charmap == cmap )
            face->charmap = ফাঁকা;

          ft_cmap_done_internal( cmap );

          বিরতি;
        }
      }
    }
  }


  FT_BASE_DEF( FT_Error )
  FT_CMap_New( FT_CMap_Class  clazz,
               FT_Pointer     init_তথ্য,
               FT_CharMap     charmap,
               FT_CMap       *acmap )
  {
    FT_Error   error;
    FT_Face    face;
    FT_Memory  memory;
    FT_CMap    cmap = ফাঁকা;


    যদি ( !clazz || !charmap || !charmap->face )
      ফেরত FT_THROW( Invalid_Argument );

    face   = charmap->face;
    memory = FT_FACE_MEMORY( face );

    যদি ( !FT_ALLOC( cmap, clazz->আকার_2 ) )
    {
      cmap->charmap = *charmap;
      cmap->clazz   = clazz;

      যদি ( clazz->init )
      {
        error = clazz->init( cmap, init_তথ্য );
        যদি ( error )
          যাও Fail;
      }

      /* add it to our list of charmaps */
      যদি ( FT_QRENEW_ARRAY( face->charmaps,
                            face->অঙ্ক_charmaps,
                            face->অঙ্ক_charmaps + 1 ) )
        যাও Fail;

      face->charmaps[face->অঙ্ক_charmaps++] = (FT_CharMap)cmap;
    }

  Exit:
    যদি ( acmap )
      *acmap = cmap;

    ফেরত error;

  Fail:
    ft_cmap_done_internal( cmap );
    cmap = ফাঁকা;
    যাও Exit;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt )
  FT_Get_Char_সূচক( FT_Face   face,
                     FT_ULong  charcode )
  {
    FT_UInt  ফলাফল = 0;


    যদি ( face && face->charmap )
    {
      FT_CMap  cmap = FT_CMAP( face->charmap );


      যদি ( charcode > 0xFFFFFFFFUL )
      {
        FT_TRACE1(( "FT_Get_Char_Index: too large charcode" ));
        FT_TRACE1(( " 0x%lx is truncated\n", charcode ));
      }

      ফলাফল = cmap->clazz->char_সূচক( cmap, (FT_UInt32)charcode );
      যদি ( ফলাফল >= (FT_UInt)face->অঙ্ক_glyphs )
        ফলাফল = 0;
    }

    ফেরত ফলাফল;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_ULong )
  FT_Get_First_Char( FT_Face   face,
                     FT_UInt  *agindex )
  {
    FT_ULong  ফলাফল = 0;
    FT_UInt   gindex = 0;


    /* only do something if we have a charmap, and we have glyphs at all */
    যদি ( face && face->charmap && face->অঙ্ক_glyphs )
    {
      gindex = FT_Get_Char_সূচক( face, 0 );
      যদি ( gindex == 0 )
        ফলাফল = FT_Get_পরবর্তী_Char( face, 0, &gindex );
    }

    যদি ( agindex )
      *agindex = gindex;

    ফেরত ফলাফল;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_ULong )
  FT_Get_পরবর্তী_Char( FT_Face   face,
                    FT_ULong  charcode,
                    FT_UInt  *agindex )
  {
    FT_ULong  ফলাফল = 0;
    FT_UInt   gindex = 0;


    যদি ( face && face->charmap && face->অঙ্ক_glyphs )
    {
      FT_UInt32  code = (FT_UInt32)charcode;
      FT_CMap    cmap = FT_CMAP( face->charmap );


      করো
      {
        gindex = cmap->clazz->char_পরবর্তী( cmap, &code );

      } যখন ( gindex >= (FT_UInt)face->অঙ্ক_glyphs );

      ফলাফল = ( gindex == 0 ) ? 0 : code;
    }

    যদি ( agindex )
      *agindex = gindex;

    ফেরত ফলাফল;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Face_Properties( FT_Face        face,
                      FT_UInt        অঙ্ক_properties,
                      FT_Parameter*  properties )
  {
    FT_Error  error = FT_Err_Ok;


    যদি ( অঙ্ক_properties > 0 && !properties )
    {
      error = FT_THROW( Invalid_Argument );
      যাও Exit;
    }

    জন্য ( ; অঙ্ক_properties > 0; অঙ্ক_properties-- )
    {
      যদি ( properties->tag == FT_PARAM_TAG_STEM_DARKENING )
      {
        যদি ( properties->তথ্য )
        {
          যদি ( *( (FT_Bool*)properties->তথ্য ) == TRUE )
            face->internal->no_stem_darkening = FALSE;
          অন্যথা
            face->internal->no_stem_darkening = TRUE;
        }
        অন্যথা
        {
          /* use module default */
          face->internal->no_stem_darkening = -1;
        }
      }
      অন্যথা যদি ( properties->tag == FT_PARAM_TAG_LCD_FILTER_WEIGHTS )
      {
        error = FT_THROW( Unimplemented_Feature );
        যাও Exit;
      }
      অন্যথা যদি ( properties->tag == FT_PARAM_TAG_RANDOM_SEED )
      {
        যদি ( properties->তথ্য )
        {
          face->internal->random_seed = *( (FT_Int32*)properties->তথ্য );
          যদি ( face->internal->random_seed < 0 )
            face->internal->random_seed = 0;
        }
        অন্যথা
        {
          /* use module default */
          face->internal->random_seed = -1;
        }
      }
      অন্যথা
      {
        error = FT_THROW( Invalid_Argument );
        যাও Exit;
      }

      যদি ( error )
        বিরতি;

      properties++;
    }

  Exit:
    ফেরত error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt )
  FT_Face_Get_Char_Variant_সূচক( FT_Face   face,
                               FT_ULong  charcode,
                               FT_ULong  variantSelector )
  {
    FT_UInt  ফলাফল = 0;


    যদি ( face                                           &&
         face->charmap                                  &&
         face->charmap->encoding == FT_ENCODING_UNICODE )
    {
      FT_CharMap  charmap = find_variant_selector_charmap( face );
      FT_CMap     ucmap = FT_CMAP( face->charmap );


      যদি ( charmap )
      {
        FT_CMap  vcmap = FT_CMAP( charmap );


        যদি ( charcode > 0xFFFFFFFFUL )
        {
          FT_TRACE1(( "FT_Face_GetCharVariantIndex:"
                      " too large charcode" ));
          FT_TRACE1(( " 0x%lx is truncated\n", charcode ));
        }
        যদি ( variantSelector > 0xFFFFFFFFUL )
        {
          FT_TRACE1(( "FT_Face_GetCharVariantIndex:"
                      " too large variantSelector" ));
          FT_TRACE1(( " 0x%lx is truncated\n", variantSelector ));
        }

        ফলাফল = vcmap->clazz->char_var_সূচক( vcmap, ucmap,
                                               (FT_UInt32)charcode,
                                               (FT_UInt32)variantSelector );
      }
    }

    ফেরত ফলাফল;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Int )
  FT_Face_GetCharVariantIsDefault( FT_Face   face,
                                   FT_ULong  charcode,
                                   FT_ULong  variantSelector )
  {
    FT_Int  ফলাফল = -1;


    যদি ( face )
    {
      FT_CharMap  charmap = find_variant_selector_charmap( face );


      যদি ( charmap )
      {
        FT_CMap  vcmap = FT_CMAP( charmap );


        যদি ( charcode > 0xFFFFFFFFUL )
        {
          FT_TRACE1(( "FT_Face_GetCharVariantIsDefault:"
                      " too large charcode" ));
          FT_TRACE1(( " 0x%lx is truncated\n", charcode ));
        }
        যদি ( variantSelector > 0xFFFFFFFFUL )
        {
          FT_TRACE1(( "FT_Face_GetCharVariantIsDefault:"
                      " too large variantSelector" ));
          FT_TRACE1(( " 0x%lx is truncated\n", variantSelector ));
        }

        ফলাফল = vcmap->clazz->char_var_default( vcmap,
                                                 (FT_UInt32)charcode,
                                                 (FT_UInt32)variantSelector );
      }
    }

    ফেরত ফলাফল;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt32* )
  FT_Face_GetVariantSelectors( FT_Face  face )
  {
    FT_UInt32  *ফলাফল = ফাঁকা;


    যদি ( face )
    {
      FT_CharMap  charmap = find_variant_selector_charmap( face );


      যদি ( charmap )
      {
        FT_CMap    vcmap  = FT_CMAP( charmap );
        FT_Memory  memory = FT_FACE_MEMORY( face );


        ফলাফল = vcmap->clazz->variant_list( vcmap, memory );
      }
    }

    ফেরত ফলাফল;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt32* )
  FT_Face_GetVariantsOfChar( FT_Face   face,
                             FT_ULong  charcode )
  {
    FT_UInt32  *ফলাফল = ফাঁকা;


    যদি ( face )
    {
      FT_CharMap  charmap = find_variant_selector_charmap( face );


      যদি ( charmap )
      {
        FT_CMap    vcmap  = FT_CMAP( charmap );
        FT_Memory  memory = FT_FACE_MEMORY( face );


        যদি ( charcode > 0xFFFFFFFFUL )
        {
          FT_TRACE1(( "FT_Face_GetVariantsOfChar: too large charcode" ));
          FT_TRACE1(( " 0x%lx is truncated\n", charcode ));
        }

        ফলাফল = vcmap->clazz->charvariant_list( vcmap, memory,
                                                 (FT_UInt32)charcode );
      }
    }
    ফেরত ফলাফল;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt32* )
  FT_Face_GetCharsOfVariant( FT_Face   face,
                             FT_ULong  variantSelector )
  {
    FT_UInt32  *ফলাফল = ফাঁকা;


    যদি ( face )
    {
      FT_CharMap  charmap = find_variant_selector_charmap( face );


      যদি ( charmap )
      {
        FT_CMap    vcmap  = FT_CMAP( charmap );
        FT_Memory  memory = FT_FACE_MEMORY( face );


        যদি ( variantSelector > 0xFFFFFFFFUL )
        {
          FT_TRACE1(( "FT_Get_Char_Index: too large variantSelector" ));
          FT_TRACE1(( " 0x%lx is truncated\n", variantSelector ));
        }

        ফলাফল = vcmap->clazz->variantchar_list( vcmap, memory,
                                                 (FT_UInt32)variantSelector );
      }
    }

    ফেরত ফলাফল;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt )
  FT_Get_Name_সূচক( FT_Face           face,
                     ধ্রুবক FT_String*  glyph_name )
  {
    FT_UInt  ফলাফল = 0;


    যদি ( face                       &&
         FT_HAS_GLYPH_NAMES( face ) &&
         glyph_name                 )
    {
      FT_Service_GlyphDict  service;


      FT_FACE_LOOKUP_SERVICE( face,
                              service,
                              GLYPH_DICT );

      যদি ( service && service->name_সূচক )
        ফলাফল = service->name_সূচক( face, glyph_name );
    }

    ফেরত ফলাফল;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Glyph_Name( FT_Face     face,
                     FT_UInt     glyph_সূচক,
                     FT_Pointer  বাফার,
                     FT_UInt     বাফার_max )
  {
    FT_Error              error;
    FT_Service_GlyphDict  service;


    যদি ( !face )
      ফেরত FT_THROW( Invalid_Face_Handle );

    যদি ( !বাফার || বাফার_max == 0 )
      ফেরত FT_THROW( Invalid_Argument );

    /* clean up buffer */
    ((FT_Byte*)বাফার)[0] = '\0';

    যদি ( (FT_Long)glyph_সূচক >= face->অঙ্ক_glyphs )
      ফেরত FT_THROW( Invalid_Glyph_সূচক );

    যদি ( !FT_HAS_GLYPH_NAMES( face ) )
      ফেরত FT_THROW( Invalid_Argument );

    FT_FACE_LOOKUP_SERVICE( face, service, GLYPH_DICT );
    যদি ( service && service->get_name )
      error = service->get_name( face, glyph_সূচক, বাফার, বাফার_max );
    অন্যথা
      error = FT_THROW( Invalid_Argument );

    ফেরত error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( ধ্রুবক অক্ষর* )
  FT_Get_Postscript_Name( FT_Face  face )
  {
    ধ্রুবক অক্ষর*  ফলাফল = ফাঁকা;


    যদি ( !face )
      যাও Exit;

    যদি ( !ফলাফল )
    {
      FT_Service_PsFontName  service;


      FT_FACE_LOOKUP_SERVICE( face,
                              service,
                              POSTSCRIPT_FONT_NAME );

      যদি ( service && service->get_ps_font_name )
        ফলাফল = service->get_ps_font_name( face );
    }

  Exit:
    ফেরত ফলাফল;
  }


  /* documentation is in tttables.h */

  FT_EXPORT_DEF( শূন্য* )
  FT_Get_Sfnt_Table( FT_Face      face,
                     FT_Sfnt_Tag  tag )
  {
    শূন্য*                  table = ফাঁকা;
    FT_Service_SFNT_Table  service;


    যদি ( face && FT_IS_SFNT( face ) )
    {
      FT_FACE_FIND_SERVICE( face, service, SFNT_TABLE );
      যদি ( service )
        table = service->get_table( face, tag );
    }

    ফেরত table;
  }


  /* documentation is in tttables.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Load_Sfnt_Table( FT_Face    face,
                      FT_ULong   tag,
                      FT_Long    offset,
                      FT_Byte*   বাফার,
                      FT_ULong*  দৈর্ঘ্য )
  {
    FT_Service_SFNT_Table  service;


    যদি ( !face || !FT_IS_SFNT( face ) )
      ফেরত FT_THROW( Invalid_Face_Handle );

    FT_FACE_FIND_SERVICE( face, service, SFNT_TABLE );
    যদি ( !service )
      ফেরত FT_THROW( Unimplemented_Feature );

    ফেরত service->load_table( face, tag, offset, বাফার, দৈর্ঘ্য );
  }


  /* documentation is in tttables.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Sfnt_Table_Info( FT_Face    face,
                      FT_UInt    table_সূচক,
                      FT_ULong  *tag,
                      FT_ULong  *দৈর্ঘ্য )
  {
    FT_Service_SFNT_Table  service;
    FT_ULong               offset;


    /* test for valid `length' delayed to `service->table_info' */

    যদি ( !face || !FT_IS_SFNT( face ) )
      ফেরত FT_THROW( Invalid_Face_Handle );

    FT_FACE_FIND_SERVICE( face, service, SFNT_TABLE );
    যদি ( !service )
      ফেরত FT_THROW( Unimplemented_Feature );

    ফেরত service->table_info( face, table_সূচক, tag, &offset, দৈর্ঘ্য );
  }


  /* documentation is in tttables.h */

  FT_EXPORT_DEF( FT_ULong )
  FT_Get_CMap_Language_ID( FT_CharMap  charmap )
  {
    FT_Service_TTCMaps  service;
    FT_Face             face;
    TT_CMapInfo         cmap_info;


    যদি ( !charmap || !charmap->face )
      ফেরত 0;

    face = charmap->face;
    FT_FACE_FIND_SERVICE( face, service, TT_CMAP );
    যদি ( !service )
      ফেরত 0;
    যদি ( service->get_cmap_info( charmap, &cmap_info ))
      ফেরত 0;

    ফেরত cmap_info.language;
  }


  /* documentation is in tttables.h */

  FT_EXPORT_DEF( FT_Long )
  FT_Get_CMap_Format( FT_CharMap  charmap )
  {
    FT_Service_TTCMaps  service;
    FT_Face             face;
    TT_CMapInfo         cmap_info;


    যদি ( !charmap || !charmap->face )
      ফেরত -1;

    face = charmap->face;
    FT_FACE_FIND_SERVICE( face, service, TT_CMAP );
    যদি ( !service )
      ফেরত -1;
    যদি ( service->get_cmap_info( charmap, &cmap_info ))
      ফেরত -1;

    ফেরত cmap_info.format;
  }


  /* documentation is in ftsizes.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Activate_আকার( FT_আকার  আকার_2 )
  {
    FT_Face  face;


    যদি ( !আকার_2 )
      ফেরত FT_THROW( Invalid_আকার_Handle );

    face = আকার_2->face;
    যদি ( !face || !face->driver )
      ফেরত FT_THROW( Invalid_Face_Handle );

    /* we don't need anything more complex than that; all size objects */
    /* are already listed by the face                                  */
    face->আকার_2 = আকার_2;

    ফেরত FT_Err_Ok;
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
                      FT_List_গ্রন্থি*     গ্রন্থি )
  {
    FT_List_গ্রন্থি  cur;
    FT_Renderer  ফলাফল = ফাঁকা;


    যদি ( !library )
      যাও Exit;

    cur = library->renderers.শীর্ষ_2;

    যদি ( গ্রন্থি )
    {
      যদি ( *গ্রন্থি )
        cur = (*গ্রন্থি)->পরবর্তী;
      *গ্রন্থি = ফাঁকা;
    }

    যখন ( cur )
    {
      FT_Renderer  renderer = FT_RENDERER( cur->তথ্য );


      যদি ( renderer->glyph_format == format )
      {
        যদি ( গ্রন্থি )
          *গ্রন্থি = cur;

        ফলাফল = renderer;
        বিরতি;
      }
      cur = cur->পরবর্তী;
    }

  Exit:
    ফেরত ফলাফল;
  }


  স্থির FT_Renderer
  ft_lookup_glyph_renderer( FT_GlyphSlot  slot )
  {
    FT_Face      face    = slot->face;
    FT_Library   library = FT_FACE_LIBRARY( face );
    FT_Renderer  ফলাফল  = library->cur_renderer;


    যদি ( !ফলাফল || ফলাফল->glyph_format != slot->format )
      ফলাফল = FT_Lookup_Renderer( library, slot->format, 0 );

    ফেরত ফলাফল;
  }


  স্থির শূন্য
  ft_set_current_renderer( FT_Library  library )
  {
    FT_Renderer  renderer;


    renderer = FT_Lookup_Renderer( library, FT_GLYPH_FORMAT_OUTLINE, 0 );
    library->cur_renderer = renderer;
  }


  স্থির FT_Error
  ft_add_renderer( FT_Module  module )
  {
    FT_Library   library = module->library;
    FT_Memory    memory  = library->memory;
    FT_Error     error;
    FT_List_গ্রন্থি  গ্রন্থি    = ফাঁকা;


    যদি ( FT_QNEW( গ্রন্থি ) )
      যাও Exit;

    {
      FT_Renderer         render = FT_RENDERER( module );
      FT_Renderer_Class*  clazz  = (FT_Renderer_Class*)module->clazz;


      render->clazz        = clazz;
      render->glyph_format = clazz->glyph_format;

      /* allocate raster object if needed */
      যদি ( clazz->raster_class && clazz->raster_class->raster_new )
      {
        error = clazz->raster_class->raster_new( memory, &render->raster );
        যদি ( error )
          যাও Fail;

        render->raster_render = clazz->raster_class->raster_render;
        render->render        = clazz->render_glyph;
      }

#ifdef FT_CONFIG_OPTION_SVG
      যদি ( clazz->glyph_format == FT_GLYPH_FORMAT_SVG )
        render->render = clazz->render_glyph;
#endif

      /* add to list */
      গ্রন্থি->তথ্য = module;
      FT_List_Add( &library->renderers, গ্রন্থি );

      ft_set_current_renderer( library );
    }

  Fail:
    যদি ( error )
      FT_FREE( গ্রন্থি );

  Exit:
    ফেরত error;
  }


  স্থির শূন্য
  ft_remove_renderer( FT_Module  module )
  {
    FT_Library   library;
    FT_Memory    memory;
    FT_List_গ্রন্থি  গ্রন্থি;


    library = module->library;
    যদি ( !library )
      ফেরত;

    memory = library->memory;

    গ্রন্থি = FT_List_Find( &library->renderers, module );
    যদি ( গ্রন্থি )
    {
      FT_Renderer  render = FT_RENDERER( module );


      /* release raster object, if any */
      যদি ( render->raster )
        render->clazz->raster_class->raster_done( render->raster );

      /* remove from list */
      FT_List_Remove( &library->renderers, গ্রন্থি );
      FT_FREE( গ্রন্থি );

      ft_set_current_renderer( library );
    }
  }


  /* documentation is in ftrender.h */

  FT_EXPORT_DEF( FT_Renderer )
  FT_Get_Renderer( FT_Library       library,
                   FT_Glyph_Format  format )
  {
    /* test for valid `library' delayed to `FT_Lookup_Renderer' */

    ফেরত FT_Lookup_Renderer( library, format, 0 );
  }


  /* documentation is in ftrender.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Set_Renderer( FT_Library     library,
                   FT_Renderer    renderer,
                   FT_UInt        অঙ্ক_params,
                   FT_Parameter*  parameters )
  {
    FT_List_গ্রন্থি  গ্রন্থি;
    FT_Error     error = FT_Err_Ok;

    FT_Renderer_SetModeFunc  set_mode;


    যদি ( !library )
    {
      error = FT_THROW( Invalid_Library_Handle );
      যাও Exit;
    }

    যদি ( !renderer )
    {
      error = FT_THROW( Invalid_Argument );
      যাও Exit;
    }

    যদি ( অঙ্ক_params > 0 && !parameters )
    {
      error = FT_THROW( Invalid_Argument );
      যাও Exit;
    }

    গ্রন্থি = FT_List_Find( &library->renderers, renderer );
    যদি ( !গ্রন্থি )
    {
      error = FT_THROW( Invalid_Argument );
      যাও Exit;
    }

    FT_List_Up( &library->renderers, গ্রন্থি );

    যদি ( renderer->glyph_format == FT_GLYPH_FORMAT_OUTLINE )
      library->cur_renderer = renderer;

    set_mode = renderer->clazz->set_mode;

    জন্য ( ; অঙ্ক_params > 0; অঙ্ক_params-- )
    {
      error = set_mode( renderer, parameters->tag, parameters->তথ্য );
      যদি ( error )
        বিরতি;
      parameters++;
    }

  Exit:
    ফেরত error;
  }


  FT_BASE_DEF( FT_Error )
  FT_Render_Glyph_Internal( FT_Library      library,
                            FT_GlyphSlot    slot,
                            FT_Render_Mode  render_mode )
  {
    FT_Error     error = FT_Err_Ok;
    FT_Face      face  = slot->face;
    FT_Renderer  renderer;
    FT_List_গ্রন্থি  গ্রন্থি  = ফাঁকা;


    /* try to render colored glyph layers as a special case */
    যদি ( slot->internal->load_flags & FT_LOAD_COLOR &&
         slot->format == FT_GLYPH_FORMAT_OUTLINE    )
    {
      FT_LayerIterator  iterator;

      FT_UInt  base_glyph = slot->glyph_সূচক;

      FT_Bool  have_layers;
      FT_UInt  glyph_সূচক;
      FT_UInt  color_সূচক;


      iterator.p  = ফাঁকা;
      have_layers = FT_Get_Color_Glyph_Layer( face,
                                              base_glyph,
                                              &glyph_সূচক,
                                              &color_সূচক,
                                              &iterator );
      যদি ( have_layers )
      {
        error = FT_New_GlyphSlot( face, ফাঁকা );
        যদি ( !error )
        {
          TT_Face       ttface = (TT_Face)face;
          SFNT_Service  sfnt   = (SFNT_Service)ttface->sfnt;


          করো
          {
            FT_Int32  load_flags = slot->internal->load_flags;


            /* disable the `FT_LOAD_COLOR' flag to avoid recursion */
            /* right here in this function                         */
            load_flags &= ~FT_LOAD_COLOR;

            /* render into the new `face->glyph' glyph slot */
            load_flags |= FT_LOAD_RENDER | FT_LOAD_NO_BITMAP;

            error = FT_Load_Glyph( face, glyph_সূচক, load_flags );
            যদি ( error )
              বিরতি;

            /* blend new `face->glyph' into old `slot'; */
            /* at the first call, `slot' is still empty */
            error = sfnt->colr_blend( ttface,
                                      color_সূচক,
                                      slot,
                                      face->glyph );
            যদি ( error )
              বিরতি;

          } যখন ( FT_Get_Color_Glyph_Layer( face,
                                              base_glyph,
                                              &glyph_সূচক,
                                              &color_সূচক,
                                              &iterator ) );

          যদি ( !error )
            slot->format = FT_GLYPH_FORMAT_BITMAP;

          /* this call also restores `slot' as the glyph slot */
          FT_Done_GlyphSlot( face->glyph );
        }

        যদি ( !error )
          ফেরত error;

        /* Failed to do the colored layer.  Draw outline instead. */
        slot->format = FT_GLYPH_FORMAT_OUTLINE;
      }
    }

    /* small shortcut for the very common case */
    যদি ( slot->format == FT_GLYPH_FORMAT_OUTLINE )
    {
      renderer = library->cur_renderer;
      গ্রন্থি     = library->renderers.শীর্ষ_2;
    }
    অন্যথা
      renderer = FT_Lookup_Renderer( library, slot->format, &গ্রন্থি );

    error = FT_ERR( Cannot_Render_Glyph );
    যখন ( renderer )
    {
      error = renderer->render( renderer, slot, render_mode, ফাঁকা );
      যদি ( !error                                   ||
           FT_ERR_NEQ( error, Cannot_Render_Glyph ) )
        বিরতি;

      /* `FT_Err_Cannot_Render_Glyph` is returned if the render mode */
      /* is unsupported by the current renderer for this glyph image */
      /* format.                                                     */

      /* now, look for another renderer that supports the same */
      /* format.                                               */
      renderer = FT_Lookup_Renderer( library, slot->format, &গ্রন্থি );
    }

    /* it is not an error if we cannot render a bitmap glyph */
    যদি ( FT_ERR_EQ( error, Cannot_Render_Glyph ) &&
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
    যদি ( !error                               &&
         ft_trace_levels[trace_checksum] >= 3 &&
         slot->bitmap.বাফার                  )
    {
      FT_Bitmap  bitmap;
      FT_Error   err;


      FT_Bitmap_Init( &bitmap );

      /* we convert to a single bitmap format for computing the checksum */
      /* this also converts the bitmap flow to `down' (i.e., pitch > 0)  */
      err = FT_Bitmap_Convert( library, &slot->bitmap, &bitmap, 1 );
      যদি ( !err )
      {
        MD5_CTX        ctx;
        unsigned অক্ষর  md5[16];
        unsigned long  coverage = 0;
        পূর্ণসংখ্যা            i, j;
        পূর্ণসংখ্যা            rows  = (পূর্ণসংখ্যা)bitmap.rows;
        পূর্ণসংখ্যা            pitch = bitmap.pitch;


        FT_TRACE3(( "FT_Render_Glyph: bitmap %dx%d, %s (mode %d)\n",
                    pitch,
                    rows,
                    pixel_modes[slot->bitmap.pixel_mode],
                    slot->bitmap.pixel_mode ));

        জন্য ( i = 0; i < rows; i++ )
          জন্য ( j = 0; j < pitch; j++ )
            coverage += bitmap.বাফার[i * pitch + j];

        FT_TRACE3(( "  Total coverage: %lu\n", coverage ));

        MD5_Init( &ctx );
        যদি ( bitmap.বাফার )
          MD5_Update( &ctx, bitmap.বাফার,
                      (unsigned long)rows * (unsigned long)pitch );
        MD5_Final( md5, &ctx );

        FT_TRACE3(( "  MD5 checksum: " ));
        জন্য ( i = 0; i < 16; i++ )
          FT_TRACE3(( "%02X", md5[i] ));
        FT_TRACE3(( "\n" ));
      }

      FT_Bitmap_Done( library, &bitmap );
    }

    /*
     * Dump bitmap in Netpbm format (PBM or PGM).
     */

    /* we use FT_TRACE7 in this block */
    যদি ( !error                               &&
         ft_trace_levels[trace_checksum] >= 7 &&
         slot->bitmap.বাফার                  )
    {
      যদি ( slot->bitmap.rows  < 128U &&
           slot->bitmap.width < 128U )
      {
        পূর্ণসংখ্যা  rows  = (পূর্ণসংখ্যা)slot->bitmap.rows;
        পূর্ণসংখ্যা  width = (পূর্ণসংখ্যা)slot->bitmap.width;
        পূর্ণসংখ্যা  pitch =      slot->bitmap.pitch;
        পূর্ণসংখ্যা  i, j, m;

        unsigned অক্ষর*  topleft = slot->bitmap.বাফার;


        যদি ( pitch < 0 )
          topleft -= pitch * ( rows - 1 );

        FT_TRACE7(( "Netpbm image: start\n" ));
        নির্বাচন ( slot->bitmap.pixel_mode )
        {
        ক্ষেত্র FT_PIXEL_MODE_MONO:
          FT_TRACE7(( "P1 %d %d\n", width, rows ));
          জন্য ( i = 0; i < rows; i++ )
          {
            জন্য ( j = 0; j < width; )
              জন্য ( m = 128; m > 0 && j < width; m >>= 1, j++ )
                FT_TRACE7(( " %d",
                            ( topleft[i * pitch + j / 8] & m ) != 0 ));
            FT_TRACE7(( "\n" ));
          }
          বিরতি;

        পূর্বনির্ধারিত:
          FT_TRACE7(( "P2 %d %d 255\n", width, rows ));
          জন্য ( i = 0; i < rows; i++ )
          {
            জন্য ( j = 0; j < width; j += 1 )
              FT_TRACE7(( " %3u", topleft[i * pitch + j] ));
            FT_TRACE7(( "\n" ));
          }
        }
        FT_TRACE7(( "Netpbm image: end\n" ));
      }
      অন্যথা
        FT_TRACE7(( "Netpbm image: too large, omitted\n" ));
    }

#undef  FT_COMPONENT
#define FT_COMPONENT  objs

#endif /* FT_DEBUG_LEVEL_TRACE */

    ফেরত error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Render_Glyph( FT_GlyphSlot    slot,
                   FT_Render_Mode  render_mode )
  {
    FT_Library  library;


    যদি ( !slot || !slot->face )
      ফেরত FT_THROW( Invalid_Argument );

    library = FT_FACE_LIBRARY( slot->face );

    ফেরত FT_Render_Glyph_Internal( library, slot, render_mode );
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
  স্থির শূন্য
  Destroy_Module( FT_Module  module )
  {
    ধ্রুবক FT_Module_Class*  clazz   = module->clazz;
    FT_Library              library = module->library;
    FT_Memory               memory  = module->memory;


    যদি ( library && library->auto_hinter == module )
      library->auto_hinter = ফাঁকা;

    /* if the module is a renderer */
    যদি ( FT_MODULE_IS_RENDERER( module ) )
      ft_remove_renderer( module );

    /* if the module is a font driver, add some steps */
    যদি ( FT_MODULE_IS_DRIVER( module ) )
      Destroy_Driver( FT_DRIVER( module ) );

    /* finalize the module object */
    যদি ( clazz->module_done )
      clazz->module_done( module );

    /* discard it */
    FT_FREE( module );
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Add_Module( FT_Library              library,
                 ধ্রুবক FT_Module_Class*  clazz )
  {
    FT_Error   error;
    FT_Memory  memory;
    FT_Module  module = ফাঁকা;
    FT_UInt    nn;


#define FREETYPE_VER_FIXED  ( ( (FT_Long)FREETYPE_MAJOR << 16 ) | \
                                FREETYPE_MINOR                  )

    যদি ( !library )
      ফেরত FT_THROW( Invalid_Library_Handle );

    যদি ( !clazz )
      ফেরত FT_THROW( Invalid_Argument );

    /* check FreeType version */
    যদি ( clazz->module_requires > FREETYPE_VER_FIXED )
      ফেরত FT_THROW( Invalid_Version );

    /* look for a module with the same name in the library's table */
    জন্য ( nn = 0; nn < library->অঙ্ক_modules; nn++ )
    {
      module = library->modules[nn];
      যদি ( ft_strcmp( module->clazz->module_name, clazz->module_name ) == 0 )
      {
        /* this installed module has the same name, compare their versions */
        যদি ( clazz->module_version <= module->clazz->module_version )
          ফেরত FT_THROW( Lower_Module_Version );

        /* remove the module from our list, then exit the loop to replace */
        /* it by our new version..                                        */
        FT_Remove_Module( library, module );
        বিরতি;
      }
    }

    memory = library->memory;
    error  = FT_Err_Ok;

    যদি ( library->অঙ্ক_modules >= FT_MAX_MODULES )
    {
      error = FT_THROW( Too_Many_Drivers );
      যাও Exit;
    }

    /* allocate module object */
    যদি ( FT_ALLOC( module, clazz->module_আকার ) )
      যাও Exit;

    /* base initialization */
    module->clazz   = clazz;
    module->library = library;
    module->memory  = memory;

    /* check whether the module is a renderer - this must be performed */
    /* before the normal module initialization                         */
    যদি ( FT_MODULE_IS_RENDERER( module ) )
    {
      /* add to the renderers list */
      error = ft_add_renderer( module );
      যদি ( error )
        যাও Fail;
    }

    /* is the module a auto-hinter? */
    যদি ( FT_MODULE_IS_HINTER( module ) )
      library->auto_hinter = module;

    /* if the module is a font driver */
    যদি ( FT_MODULE_IS_DRIVER( module ) )
    {
      FT_Driver  driver = FT_DRIVER( module );


      driver->clazz = (FT_Driver_Class)module->clazz;
    }

    যদি ( clazz->module_init )
    {
      error = clazz->module_init( module );
      যদি ( error )
        যাও Fail;
    }

    /* add module to the library's table */
    library->modules[library->অঙ্ক_modules++] = module;

  Exit:
    ফেরত error;

  Fail:
    যদি ( FT_MODULE_IS_RENDERER( module ) )
    {
      FT_Renderer  renderer = FT_RENDERER( module );


      যদি ( renderer->clazz                                          &&
           renderer->clazz->glyph_format == FT_GLYPH_FORMAT_OUTLINE &&
           renderer->raster                                         )
        renderer->clazz->raster_class->raster_done( renderer->raster );
    }

    FT_FREE( module );
    যাও Exit;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Module )
  FT_Get_Module( FT_Library   library,
                 ধ্রুবক অক্ষর*  module_name )
  {
    FT_Module   ফলাফল = ফাঁকা;
    FT_Module*  cur;
    FT_Module*  limit;


    যদি ( !library || !module_name )
      ফেরত ফলাফল;

    cur   = library->modules;
    limit = cur + library->অঙ্ক_modules;

    জন্য ( ; cur < limit; cur++ )
      যদি ( ft_strcmp( cur[0]->clazz->module_name, module_name ) == 0 )
      {
        ফলাফল = cur[0];
        বিরতি;
      }

    ফেরত ফলাফল;
  }


  /* documentation is in ftobjs.h */

  FT_BASE_DEF( ধ্রুবক শূন্য* )
  FT_Get_Module_Interface( FT_Library   library,
                           ধ্রুবক অক্ষর*  mod_name )
  {
    FT_Module  module;


    /* test for valid `library' delayed to FT_Get_Module() */

    module = FT_Get_Module( library, mod_name );

    ফেরত module ? module->clazz->module_interface : 0;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_module_get_service( FT_Module    module,
                         ধ্রুবক অক্ষর*  service_id,
                         FT_Bool      global )
  {
    FT_Pointer  ফলাফল = ফাঁকা;


    যদি ( module )
    {
      FT_ASSERT( module->clazz && module->clazz->get_interface );

      /* first, look for the service in the module */
      যদি ( module->clazz->get_interface )
        ফলাফল = module->clazz->get_interface( module, service_id );

      যদি ( global && !ফলাফল )
      {
        /* we didn't find it, look in all other modules then */
        FT_Library  library = module->library;
        FT_Module*  cur     = library->modules;
        FT_Module*  limit   = cur + library->অঙ্ক_modules;


        জন্য ( ; cur < limit; cur++ )
        {
          যদি ( cur[0] != module )
          {
            FT_ASSERT( cur[0]->clazz );

            যদি ( cur[0]->clazz->get_interface )
            {
              ফলাফল = cur[0]->clazz->get_interface( cur[0], service_id );
              যদি ( ফলাফল )
                বিরতি;
            }
          }
        }
      }
    }

    ফেরত ফলাফল;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Remove_Module( FT_Library  library,
                    FT_Module   module )
  {
    /* try to find the module from the table, then remove it from there */

    যদি ( !library )
      ফেরত FT_THROW( Invalid_Library_Handle );

    যদি ( module )
    {
      FT_Module*  cur   = library->modules;
      FT_Module*  limit = cur + library->অঙ্ক_modules;


      জন্য ( ; cur < limit; cur++ )
      {
        যদি ( cur[0] == module )
        {
          /* remove it from the table */
          library->অঙ্ক_modules--;
          limit--;
          যখন ( cur < limit )
          {
            cur[0] = cur[1];
            cur++;
          }
          limit[0] = ফাঁকা;

          /* destroy the module */
          Destroy_Module( module );

          ফেরত FT_Err_Ok;
        }
      }
    }
    ফেরত FT_THROW( Invalid_Driver_Handle );
  }


  স্থির FT_Error
  ft_property_do( FT_Library        library,
                  ধ্রুবক FT_String*  module_name,
                  ধ্রুবক FT_String*  property_name,
                  শূন্য*             মান,
                  FT_Bool           set,
                  FT_Bool           মান_is_string )
  {
    FT_Module*           cur;
    FT_Module*           limit;
    FT_Module_Interface  interface;

    FT_Service_Properties  service;

#ifdef FT_DEBUG_LEVEL_ERROR
    ধ্রুবক FT_String*  set_name  = "FT_Property_Set";
    ধ্রুবক FT_String*  get_name  = "FT_Property_Get";
    ধ্রুবক FT_String*  func_name = set ? set_name : get_name;
#endif

    FT_Bool  missing_func;


    যদি ( !library )
      ফেরত FT_THROW( Invalid_Library_Handle );

    যদি ( !module_name || !property_name || !মান )
      ফেরত FT_THROW( Invalid_Argument );

    cur   = library->modules;
    limit = cur + library->অঙ্ক_modules;

    /* search module */
    জন্য ( ; cur < limit; cur++ )
      যদি ( !ft_strcmp( cur[0]->clazz->module_name, module_name ) )
        বিরতি;

    যদি ( cur == limit )
    {
      FT_TRACE2(( "%s: can't find module `%s'\n",
                  func_name, module_name ));
      ফেরত FT_THROW( Missing_Module );
    }

    /* check whether we have a service interface */
    যদি ( !cur[0]->clazz->get_interface )
    {
      FT_TRACE2(( "%s: module `%s' doesn't support properties\n",
                  func_name, module_name ));
      ফেরত FT_THROW( Unimplemented_Feature );
    }

    /* search property service */
    interface = cur[0]->clazz->get_interface( cur[0],
                                              FT_SERVICE_ID_PROPERTIES );
    যদি ( !interface )
    {
      FT_TRACE2(( "%s: module `%s' doesn't support properties\n",
                  func_name, module_name ));
      ফেরত FT_THROW( Unimplemented_Feature );
    }

    service = (FT_Service_Properties)interface;

    যদি ( set )
      missing_func = FT_BOOL( !service->set_property );
    অন্যথা
      missing_func = FT_BOOL( !service->get_property );

    যদি ( missing_func )
    {
      FT_TRACE2(( "%s: property service of module `%s' is broken\n",
                  func_name, module_name ));
      ফেরত FT_THROW( Unimplemented_Feature );
    }

    ফেরত set ? service->set_property( cur[0],
                                        property_name,
                                        মান,
                                        মান_is_string )
               : service->get_property( cur[0],
                                        property_name,
                                        মান );
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Property_Set( FT_Library        library,
                   ধ্রুবক FT_String*  module_name,
                   ধ্রুবক FT_String*  property_name,
                   ধ্রুবক শূন্য*       মান )
  {
    ফেরত ft_property_do( library,
                           module_name,
                           property_name,
                           (শূন্য*)মান,
                           TRUE,
                           FALSE );
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Property_Get( FT_Library        library,
                   ধ্রুবক FT_String*  module_name,
                   ধ্রুবক FT_String*  property_name,
                   শূন্য*             মান )
  {
    ফেরত ft_property_do( library,
                           module_name,
                           property_name,
                           মান,
                           FALSE,
                           FALSE );
  }


#ifdef FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES

  /* this variant is used for handling the FREETYPE_PROPERTIES */
  /* environment variable                                      */

  FT_BASE_DEF( FT_Error )
  ft_property_string_set( FT_Library        library,
                          ধ্রুবক FT_String*  module_name,
                          ধ্রুবক FT_String*  property_name,
                          FT_String*        মান )
  {
    ফেরত ft_property_do( library,
                           module_name,
                           property_name,
                           (শূন্য*)মান,
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
    যদি ( !library )
      ফেরত FT_THROW( Invalid_Library_Handle );

    library->refcount++;

    ফেরত FT_Err_Ok;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_New_Library( FT_Memory    memory,
                  FT_Library  *alibrary )
  {
    FT_Library  library = ফাঁকা;
    FT_Error    error;


    যদি ( !memory || !alibrary )
      ফেরত FT_THROW( Invalid_Argument );

#ifndef FT_DEBUG_LOGGING
#ifdef FT_DEBUG_LEVEL_ERROR
    /* init debugging support */
    ft_debug_init();
#endif /* FT_DEBUG_LEVEL_ERROR */
#endif /* !FT_DEBUG_LOGGING */

    /* first of all, allocate the library object */
    যদি ( FT_NEW( library ) )
      ফেরত error;

    library->memory = memory;

    library->version_major = FREETYPE_MAJOR;
    library->version_minor = FREETYPE_MINOR;
    library->version_patch = FREETYPE_PATCH;

    library->refcount = 1;

    /* That's ok now */
    *alibrary = library;

    ফেরত FT_Err_Ok;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( শূন্য )
  FT_Library_Version( FT_Library   library,
                      FT_Int      *amajor,
                      FT_Int      *aminor,
                      FT_Int      *apatch )
  {
    FT_Int  major = 0;
    FT_Int  minor = 0;
    FT_Int  patch = 0;


    যদি ( library )
    {
      major = library->version_major;
      minor = library->version_minor;
      patch = library->version_patch;
    }

    যদি ( amajor )
      *amajor = major;

    যদি ( aminor )
      *aminor = minor;

    যদি ( apatch )
      *apatch = patch;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Done_Library( FT_Library  library )
  {
    FT_Memory  memory;


    যদি ( !library )
      ফেরত FT_THROW( Invalid_Library_Handle );

    library->refcount--;
    যদি ( library->refcount > 0 )
      যাও Exit;

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
      ধ্রুবক অক্ষর*  driver_name[] = { "type42", ফাঁকা };


      জন্য ( m = 0;
            m < আকার ( driver_name ) / আকার ( driver_name[0] );
            m++ )
      {
        জন্য ( n = 0; n < library->অঙ্ক_modules; n++ )
        {
          FT_Module    module      = library->modules[n];
          ধ্রুবক অক্ষর*  module_name = module->clazz->module_name;
          FT_List      faces;


          যদি ( driver_name[m]                                &&
               ft_strcmp( module_name, driver_name[m] ) != 0 )
            চালু;

          যদি ( ( module->clazz->module_flags & FT_MODULE_FONT_DRIVER ) == 0 )
            চালু;

          FT_TRACE7(( "FT_Done_Library: close faces for %s\n", module_name ));

          faces = &FT_DRIVER( module )->faces_list;
          যখন ( faces->শীর্ষ_2 )
          {
            FT_Done_Face( FT_FACE( faces->শীর্ষ_2->তথ্য ) );
            যদি ( faces->শীর্ষ_2 )
              FT_TRACE0(( "FT_Done_Library: failed to free some faces\n" ));
          }
        }
      }
    }

    /* Close all other modules in the library */
#যদি 1
    /* XXX Modules are removed in the reversed order so that  */
    /* type42 module is removed before truetype module.  This */
    /* avoids double free in some occasions.  It is a hack.   */
    যখন ( library->অঙ্ক_modules > 0 )
      FT_Remove_Module( library,
                        library->modules[library->অঙ্ক_modules - 1] );
#অন্যথা
    {
      FT_UInt  n;


      জন্য ( n = 0; n < library->অঙ্ক_modules; n++ )
      {
        FT_Module  module = library->modules[n];


        যদি ( module )
        {
          Destroy_Module( module );
          library->modules[n] = ফাঁকা;
        }
      }
    }
#endif

    FT_FREE( library );

  Exit:
    ফেরত FT_Err_Ok;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( শূন্য )
  FT_Set_Debug_Hook( FT_Library         library,
                     FT_UInt            hook_সূচক,
                     FT_DebugHook_Func  debug_hook )
  {
    যদি ( library && debug_hook &&
         hook_সূচক <
           ( আকার ( library->debug_hooks ) / আকার ( শূন্য* ) ) )
      library->debug_hooks[hook_সূচক] = debug_hook;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_TrueTypeEngineType )
  FT_Get_TrueType_Engine_Type( FT_Library  library )
  {
    FT_TrueTypeEngineType  ফলাফল = FT_TRUETYPE_ENGINE_TYPE_NONE;


    যদি ( library )
    {
      FT_Module  module = FT_Get_Module( library, "truetype" );


      যদি ( module )
      {
        FT_Service_TrueTypeEngine  service;


        service = (FT_Service_TrueTypeEngine)
                    ft_module_get_service( module,
                                           FT_SERVICE_ID_TRUETYPE_ENGINE,
                                           0 );
        যদি ( service )
          ফলাফল = service->engine_type;
      }
    }

    ফেরত ফলাফল;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_SubGlyph_Info( FT_GlyphSlot  glyph,
                        FT_UInt       sub_সূচক,
                        FT_Int       *p_সূচক,
                        FT_UInt      *p_flags,
                        FT_Int       *p_arg1,
                        FT_Int       *p_arg2,
                        FT_Matrix    *p_transform )
  {
    FT_Error  error = FT_ERR( Invalid_Argument );


    যদি ( glyph                                      &&
         glyph->subglyphs                           &&
         glyph->format == FT_GLYPH_FORMAT_COMPOSITE &&
         sub_সূচক < glyph->অঙ্ক_subglyphs           )
    {
      FT_SubGlyph  subg = glyph->subglyphs + sub_সূচক;


      *p_সূচক     = subg->index;
      *p_flags     = subg->flags;
      *p_arg1      = subg->arg1;
      *p_arg2      = subg->arg2;
      *p_transform = subg->transform;

      error = FT_Err_Ok;
    }

    ফেরত error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Get_Color_Glyph_Layer( FT_Face            face,
                            FT_UInt            base_glyph,
                            FT_UInt           *aglyph_সূচক,
                            FT_UInt           *acolor_সূচক,
                            FT_LayerIterator*  iterator )
  {
    TT_Face       ttface;
    SFNT_Service  sfnt;


    যদি ( !face                                   ||
         !aglyph_সূচক                           ||
         !acolor_সূচক                           ||
         !iterator                               ||
         base_glyph >= (FT_UInt)face->অঙ্ক_glyphs )
      ফেরত 0;

    যদি ( !FT_IS_SFNT( face ) )
      ফেরত 0;

    ttface = (TT_Face)face;
    sfnt   = (SFNT_Service)ttface->sfnt;

    যদি ( sfnt->get_colr_layer )
      ফেরত sfnt->get_colr_layer( ttface,
                                   base_glyph,
                                   aglyph_সূচক,
                                   acolor_সূচক,
                                   iterator );
    অন্যথা
      ফেরত 0;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Get_Color_Glyph_Paint( FT_Face                  face,
                            FT_UInt                  base_glyph,
                            FT_Color_মূল_Transform  মূল_transform,
                            FT_OpaquePaint*          paint )
  {
    TT_Face       ttface;
    SFNT_Service  sfnt;


    যদি ( !face || !paint )
      ফেরত 0;

    যদি ( !FT_IS_SFNT( face ) )
      ফেরত 0;

    ttface = (TT_Face)face;
    sfnt   = (SFNT_Service)ttface->sfnt;

    যদি ( sfnt->get_colr_glyph_paint )
      ফেরত sfnt->get_colr_glyph_paint( ttface,
                                         base_glyph,
                                         মূল_transform,
                                         paint );
    অন্যথা
      ফেরত 0;
  }


  /* documentation is in ftcolor.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Get_Color_Glyph_ClipBox( FT_Face      face,
                              FT_UInt      base_glyph,
                              FT_ClipBox*  clip_box )
  {
    TT_Face       ttface;
    SFNT_Service  sfnt;


    যদি ( !face || !clip_box )
      ফেরত 0;

    যদি ( !FT_IS_SFNT( face ) )
      ফেরত 0;

    ttface = (TT_Face)face;
    sfnt   = (SFNT_Service)ttface->sfnt;

    যদি ( sfnt->get_color_glyph_clipbox )
      ফেরত sfnt->get_color_glyph_clipbox( ttface,
                                            base_glyph,
                                            clip_box );
    অন্যথা
      ফেরত 0;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Get_Paint_Layers( FT_Face            face,
                       FT_LayerIterator*  layer_iterator,
                       FT_OpaquePaint*    paint )
  {
    TT_Face       ttface;
    SFNT_Service  sfnt;


    যদি ( !face || !paint || !layer_iterator )
      ফেরত 0;

    যদি ( !FT_IS_SFNT( face ) )
      ফেরত 0;

    ttface = (TT_Face)face;
    sfnt   = (SFNT_Service)ttface->sfnt;

    যদি ( sfnt->get_paint_layers )
      ফেরত sfnt->get_paint_layers( ttface, layer_iterator, paint );
    অন্যথা
      ফেরত 0;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Get_Paint( FT_Face face,
                FT_OpaquePaint  opaque_paint,
                FT_COLR_Paint*  paint )
  {
    TT_Face       ttface;
    SFNT_Service  sfnt;


    যদি ( !face || !paint )
      ফেরত 0;

    যদি ( !FT_IS_SFNT( face ) )
      ফেরত 0;

    ttface = (TT_Face)face;
    sfnt   = (SFNT_Service)ttface->sfnt;

    যদি ( sfnt->get_paint )
      ফেরত sfnt->get_paint( ttface, opaque_paint, paint );
    অন্যথা
      ফেরত 0;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Get_Colorline_Stops ( FT_Face                face,
                           FT_ColorStop *         color_stop,
                           FT_ColorStopIterator  *iterator )
  {
    TT_Face       ttface;
    SFNT_Service  sfnt;


    যদি ( !face || !color_stop || !iterator )
      ফেরত 0;

    যদি ( !FT_IS_SFNT( face ) )
      ফেরত 0;

    ttface = (TT_Face)face;
    sfnt   = (SFNT_Service)ttface->sfnt;

    যদি ( sfnt->get_colorline_stops )
      ফেরত sfnt->get_colorline_stops ( ttface, color_stop, iterator );
    অন্যথা
      ফেরত 0;
  }


/* END */
