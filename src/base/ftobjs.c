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


#ଅନ୍ତର୍ଭାଵ <freetype/ftlist.h>
#ଅନ୍ତର୍ଭାଵ <freetype/ftoutln.h>
#ଅନ୍ତର୍ଭାଵ <freetype/ftfntfmt.h>
#ଅନ୍ତର୍ଭାଵ <freetype/otsvg.h>

#ଅନ୍ତର୍ଭାଵ <freetype/internal/ftvalid.h>
#ଅନ୍ତର୍ଭାଵ <freetype/internal/ftobjs.h>
#ଅନ୍ତର୍ଭାଵ <freetype/internal/ftdebug.h>
#ଅନ୍ତର୍ଭାଵ <freetype/internal/ftrfork.h>
#ଅନ୍ତର୍ଭାଵ <freetype/internal/ftstream.h>
#ଅନ୍ତର୍ଭାଵ <freetype/internal/sfnt.h>          /* for SFNT_Load_Table_Func */
#ଅନ୍ତର୍ଭାଵ <freetype/internal/psaux.h>         /* for PS_Driver            */
#ଅନ୍ତର୍ଭାଵ <freetype/internal/svginterface.h>

#ଅନ୍ତର୍ଭାଵ <freetype/tttables.h>
#ଅନ୍ତର୍ଭାଵ <freetype/tttags.h>
#ଅନ୍ତର୍ଭାଵ <freetype/ttnameid.h>

#ଅନ୍ତର୍ଭାଵ <freetype/internal/services/svprop.h>
#ଅନ୍ତର୍ଭାଵ <freetype/internal/services/svsfnt.h>
#ଅନ୍ତର୍ଭାଵ <freetype/internal/services/svpostnm.h>
#ଅନ୍ତର୍ଭାଵ <freetype/internal/services/svgldict.h>
#ଅନ୍ତର୍ଭାଵ <freetype/internal/services/svttcmap.h>
#ଅନ୍ତର୍ଭାଵ <freetype/internal/services/svkern.h>
#ଅନ୍ତର୍ଭାଵ <freetype/internal/services/svtteng.h>

#ifdef TT_CONFIG_OPTION_VARC
#ଅନ୍ତର୍ଭାଵ <freetype/internal/services/svvarc.h>
#endif

#ଅନ୍ତର୍ଭାଵ <freetype/ftdriver.h>

#ifdef FT_CONFIG_OPTION_MAC_FONTS
#ଅନ୍ତର୍ଭାଵ "ftbase.h"
#endif


#ifdef FT_DEBUG_LEVEL_TRACE

#ଅନ୍ତର୍ଭାଵ <freetype/ftbitmap.h>

#ଯଦି defined( _MSC_VER )      /* Visual C++ (and Intel C++)   */
  /* We disable the warning `conversion from XXX to YYY,     */
  /* possible loss of data' in order to compile cleanly with */
  /* the maximum level of warnings: `md5.c' is non-FreeType  */
  /* code, and it gets used during development builds only.  */
#pragma warning( ଠେଲ )
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

#ଅନ୍ତର୍ଭାଵ "md5.c"

#ଯଦି defined( _MSC_VER )
#pragma warning( କାଢ଼ )
#endif

  /* This array must stay in sync with the @FT_Pixel_Mode enumeration */
  /* (in file `ftimage.h`).                                           */

  ସ୍ଥିର ଅଚଳ ଅକ୍ଷର* ଅଚଳ  pixel_modes[] =
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
  ସ୍ଥିର FT_Error
  ft_open_face_internal( FT_Library           library,
                         ଅଚଳ FT_Open_Args*  args,
                         FT_Long              face_index,
                         FT_Face             *aface,
                         FT_Bool              test_mac_fonts );


  FT_BASE_DEF( FT_Pointer )
  ft_service_list_lookup( FT_ServiceDesc  service_descriptors,
                          ଅଚଳ ଅକ୍ଷର*     service_id )
  {
    FT_Pointer      result = ଖାଲି;
    FT_ServiceDesc  desc   = service_descriptors;


    ଯଦି ( desc && service_id )
    {
      ପାଇଁ ( ; desc->serv_id != ଖାଲି; desc++ )
      {
        ଯଦି ( ft_strcmp( desc->serv_id, service_id ) == 0 )
        {
          result = (FT_Pointer)desc->serv_data;
          ଭାଙ୍ଗ;
        }
      }
    }

    ଫେରନ୍ତୁ result;
  }


  FT_BASE_DEF( ଶୂନ୍ୟ )
  ft_validator_init( FT_Validator        valid,
                     ଅଚଳ FT_Byte*      base,
                     ଅଚଳ FT_Byte*      limit,
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

    ଫେରନ୍ତୁ -1;
  }


  FT_BASE_DEF( ଶୂନ୍ୟ )
  ft_validator_error( FT_Validator  valid,
                      FT_Error      error )
  {
    /* since the cast below also disables the compiler's */
    /* type check, we introduce a dummy variable, which  */
    /* will be optimized away                            */
    ଅସ୍ଥିର ft_jmp_buf* jump_buffer = &valid->jump_buffer;


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
                 ଅଚଳ FT_Open_Args*  args,
                 FT_Stream           *astream )
  {
    FT_Error   error;
    FT_Memory  memory;
    FT_Stream  stream = ଖାଲି;
    FT_UInt    mode;


    *astream = ଖାଲି;

    ଯଦି ( !library )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Library_Handle );

    ଯଦି ( !args )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Argument );

    memory = library->memory;
    mode   = args->flags &
               ( FT_OPEN_MEMORY | FT_OPEN_STREAM | FT_OPEN_PATHNAME );

    ଯଦି ( mode == FT_OPEN_MEMORY )
    {
      /* create a memory-based stream */
      ଯଦି ( FT_NEW( stream ) )
        ଯାଅ Exit;

      FT_Stream_OpenMemory( stream,
                            (ଅଚଳ FT_Byte*)args->memory_base,
                            (FT_ULong)args->memory_size );
      stream->memory = memory;
    }

#ifndef FT_CONFIG_OPTION_DISABLE_STREAM_SUPPORT

    ଅନ୍ୟଥା ଯଦି ( mode == FT_OPEN_PATHNAME )
    {
      /* create a normal system stream */
      ଯଦି ( FT_NEW( stream ) )
        ଯାଅ Exit;

      stream->memory = memory;
      error = FT_Stream_Open( stream, args->pathname );
      ଯଦି ( error )
        FT_FREE( stream );
    }
    ଅନ୍ୟଥା ଯଦି ( ( mode == FT_OPEN_STREAM ) && args->stream )
    {
      /* use an existing, user-provided stream */

      /* in this case, we do not need to allocate a new stream object */
      /* since the caller is responsible for closing it himself       */
      stream         = args->stream;
      stream->memory = memory;
      error          = FT_Err_Ok;
    }

#endif

    ଅନ୍ୟଥା
    {
      error = FT_THROW( Invalid_Argument );
      ଯଦି ( ( args->flags & FT_OPEN_STREAM ) && args->stream )
        FT_Stream_Close( args->stream );
    }

    ଯଦି ( !error )
      *astream       = stream;

  Exit:
    ଫେରନ୍ତୁ error;
  }


  FT_BASE_DEF( ଶୂନ୍ୟ )
  FT_Stream_Free( FT_Stream  stream,
                  FT_Int     external )
  {
    ଯଦି ( stream )
    {
      FT_Memory  memory = stream->memory;


      FT_Stream_Close( stream );

      ଯଦି ( !external )
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


  ସ୍ଥିର FT_Error
  ft_glyphslot_init( FT_GlyphSlot  slot )
  {
    FT_Driver         driver   = slot->face->driver;
    FT_Driver_Class   clazz    = driver->clazz;
    FT_Memory         memory   = driver->ମୂଳ.memory;
    FT_Error          error    = FT_Err_Ok;
    FT_Slot_Internal  internal = ଖାଲି;


    slot->library = driver->ମୂଳ.library;

    ଯଦି ( FT_NEW( internal ) )
      ଯାଅ Exit;

    slot->internal = internal;

    ଯଦି ( FT_DRIVER_USES_OUTLINES( driver ) )
      error = FT_GlyphLoader_New( memory, &internal->loader );

    ଯଦି ( !error && clazz->init_slot )
      error = clazz->init_slot( slot );

#ifdef FT_CONFIG_OPTION_SVG
    /* if SVG table exists, allocate the space in `slot->other` */
    ଯଦି ( slot->face->face_flags & FT_FACE_FLAG_SVG )
    {
      FT_SVG_Document  document = ଖାଲି;


      ଯଦି ( FT_NEW( document ) )
        ଯାଅ Exit;
      slot->other = document;
    }
#endif

  Exit:
    ଫେରନ୍ତୁ error;
  }


  FT_BASE_DEF( ଶୂନ୍ୟ )
  ft_glyphslot_free_bitmap( FT_GlyphSlot  slot )
  {
    ଯଦି ( slot->internal && ( slot->internal->flags & FT_GLYPH_OWN_BITMAP ) )
    {
      FT_Memory  memory = FT_FACE_MEMORY( slot->face );


      FT_FREE( slot->bitmap.buffer );
      slot->internal->flags &= ~FT_GLYPH_OWN_BITMAP;
    }
    ଅନ୍ୟଥା
    {
      /* assume that the bitmap buffer was stolen or not */
      /* allocated from the heap                         */
      slot->bitmap.buffer = ଖାଲି;
    }
  }


  /* overflow-resistant presetting of bitmap position and dimensions; */
  /* also check whether the size is too large for rendering           */
  FT_BASE_DEF( FT_Bool )
  ft_glyphslot_preset_bitmap( FT_GlyphSlot      slot,
                              FT_Render_Mode    mode,
                              ଅଚଳ FT_Vector*  origin )
  {
    FT_Outline*  outline = &slot->outline;
    FT_Bitmap*   bitmap  = &slot->bitmap;

    FT_Pixel_Mode  pixel_mode;

    FT_BBox  cbox, pbox;
    FT_Pos   x_shift = 0;
    FT_Pos   y_shift = 0;
    FT_Pos   x_ବାମ, y_ଶୀର୍ଷ;
    FT_Pos   width, height, pitch;
    FT_Bool  ret;


    ଯଦି ( slot->format == FT_GLYPH_FORMAT_SVG )
    {
      FT_Module    module;
      SVG_Service  svg_service;


      module      = FT_Get_Module( slot->library, "ot-svg" );
      svg_service = (SVG_Service)module->clazz->module_interface;

      ଫେରନ୍ତୁ (FT_Bool)svg_service->preset_slot( module, slot, FALSE );
    }
    ଅନ୍ୟଥା ଯଦି ( slot->format != FT_GLYPH_FORMAT_OUTLINE )
      ଫେରନ୍ତୁ 1;

    ଯଦି ( origin )
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

    ବାଛ ( mode )
    {
    ସ୍ଥିତି FT_RENDER_MODE_MONO:
      pixel_mode = FT_PIXEL_MODE_MONO;
#ଯଦି 1
      /* x */

      /* undocumented but confirmed: bbox values get rounded;    */
      /* we do asymmetric rounding so that the center of a pixel */
      /* gets always included                                    */

      pbox.xMin += ( cbox.xMin + 31 ) >> 6;
      pbox.xMax += ( cbox.xMax + 32 ) >> 6;

      /* if the bbox collapsed, we add a pixel based on the total */
      /* rounding remainder to cover most of the original cbox    */

      ଯଦି ( pbox.xMin == pbox.xMax )
      {
        ଯଦି ( ( ( cbox.xMin + 31 ) & 63 ) - 31 +
             ( ( cbox.xMax + 32 ) & 63 ) - 32 < 0 )
          pbox.xMin -= 1;
        ଅନ୍ୟଥା
          pbox.xMax += 1;
      }

      /* y */

      pbox.yMin += ( cbox.yMin + 31 ) >> 6;
      pbox.yMax += ( cbox.yMax + 32 ) >> 6;

      ଯଦି ( pbox.yMin == pbox.yMax )
      {
        ଯଦି ( ( ( cbox.yMin + 31 ) & 63 ) - 31 +
             ( ( cbox.yMax + 32 ) & 63 ) - 32 < 0 )
          pbox.yMin -= 1;
        ଅନ୍ୟଥା
          pbox.yMax += 1;
      }

      ଭାଙ୍ଗ;
#ଅନ୍ୟଥା
      ଯାଅ Adjust;
#endif

    ସ୍ଥିତି FT_RENDER_MODE_LCD:
      pixel_mode = FT_PIXEL_MODE_LCD;
      ft_lcd_padding( &cbox, slot, mode );
      ଯାଅ Adjust;

    ସ୍ଥିତି FT_RENDER_MODE_LCD_V:
      pixel_mode = FT_PIXEL_MODE_LCD_V;
      ft_lcd_padding( &cbox, slot, mode );
      ଯାଅ Adjust;

    ସ୍ଥିତି FT_RENDER_MODE_NORMAL:
    ସ୍ଥିତି FT_RENDER_MODE_LIGHT:
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      pixel_mode = FT_PIXEL_MODE_GRAY;
    Adjust:
      pbox.xMin += cbox.xMin >> 6;
      pbox.yMin += cbox.yMin >> 6;
      pbox.xMax += ( cbox.xMax + 63 ) >> 6;
      pbox.yMax += ( cbox.yMax + 63 ) >> 6;
    }

    x_ବାମ = pbox.xMin;
    y_ଶୀର୍ଷ  = pbox.yMax;

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

    ଯଦି ( ret )
      FT_TRACE3(( "ft_glyphslot_preset_bitmap: [%ld %ld %ld %ld]\n",
                  pbox.xMin, pbox.yMin, pbox.xMax, pbox.yMax ));

    ବାଛ ( pixel_mode )
    {
    ସ୍ଥିତି FT_PIXEL_MODE_MONO:
      pitch = ( ( width + 15 ) >> 4 ) << 1;
      ଭାଙ୍ଗ;

    ସ୍ଥିତି FT_PIXEL_MODE_LCD:
      width *= 3;
      pitch  = FT_PAD_CEIL( width, 4 );
      ଭାଙ୍ଗ;

    ସ୍ଥିତି FT_PIXEL_MODE_LCD_V:
      height *= 3;
      FALL_THROUGH;

    ସ୍ଥିତି FT_PIXEL_MODE_GRAY:
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      pitch = width;
    }

    slot->bitmap_ବାମ = (FT_Int)x_ବାମ;
    slot->bitmap_ଶୀର୍ଷ  = (FT_Int)y_ଶୀର୍ଷ;

    bitmap->pixel_mode = (unsigned ଅକ୍ଷର)pixel_mode;
    bitmap->ଅଙ୍କ_grays  = 256;
    bitmap->width      = (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା)width;
    bitmap->rows       = (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା)height;
    bitmap->pitch      = pitch;

    ଫେରନ୍ତୁ ret;
  }


  FT_BASE_DEF( ଶୂନ୍ୟ )
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


    ଯଦି ( slot->internal->flags & FT_GLYPH_OWN_BITMAP )
      FT_FREE( slot->bitmap.buffer );
    ଅନ୍ୟଥା
      slot->internal->flags |= FT_GLYPH_OWN_BITMAP;

    /* dimensions must be preset */
    FT_MEM_ALLOC_MULT( slot->bitmap.buffer,
                       slot->bitmap.rows,
                       slot->bitmap.pitch );
    ଫେରନ୍ତୁ error;
  }


  ସ୍ଥିର ଶୂନ୍ୟ
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

    slot->bitmap_ବାମ   = 0;
    slot->bitmap_ଶୀର୍ଷ    = 0;
    slot->ଅଙ୍କ_subglyphs = 0;
    slot->subglyphs     = ଖାଲି;
    slot->control_data  = ଖାଲି;
    slot->control_len   = 0;

#ifndef FT_CONFIG_OPTION_SVG
    slot->other = ଖାଲି;
#ଅନ୍ୟଥା
    ଯଦି ( !( slot->face->face_flags & FT_FACE_FLAG_SVG ) )
      slot->other = ଖାଲି;
    ଅନ୍ୟଥା
    {
      ଯଦି ( slot->internal->flags & FT_GLYPH_OWN_GZIP_SVG )
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


  ସ୍ଥିର ଶୂନ୍ୟ
  ft_glyphslot_done( FT_GlyphSlot  slot )
  {
    FT_Driver        driver = slot->face->driver;
    FT_Driver_Class  clazz  = driver->clazz;
    FT_Memory        memory = driver->ମୂଳ.memory;

#ifdef FT_CONFIG_OPTION_SVG
    ଯଦି ( slot->face->face_flags & FT_FACE_FLAG_SVG )
    {
      /* Free memory in case SVG was there.                          */
      /* `slot->internal` might be NULL in out-of-memory situations. */
      ଯଦି ( slot->internal && slot->internal->flags & FT_GLYPH_OWN_GZIP_SVG )
      {
        FT_SVG_Document  doc = (FT_SVG_Document)slot->other;


        FT_FREE( doc->svg_document );

        slot->internal->flags &= ~FT_GLYPH_OWN_GZIP_SVG;
      }

      FT_FREE( slot->other );
    }
#endif

    ଯଦି ( clazz->done_slot )
      clazz->done_slot( slot );

    /* free bitmap buffer if needed */
    ft_glyphslot_free_bitmap( slot );

    /* slot->internal might be NULL in out-of-memory situations */
    ଯଦି ( slot->internal )
    {
      /* free glyph loader */
      ଯଦି ( FT_DRIVER_USES_OUTLINES( driver ) )
      {
        FT_GlyphLoader_Done( slot->internal->loader );
        slot->internal->loader = ଖାଲି;
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
    FT_GlyphSlot     slot = ଖାଲି;


    ଯଦି ( !face )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Face_Handle );

    ଯଦି ( !face->driver )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Argument );

    driver = face->driver;
    clazz  = driver->clazz;
    memory = driver->ମୂଳ.memory;

    FT_TRACE4(( "FT_New_GlyphSlot: Creating new slot object\n" ));
    ଯଦି ( !FT_ALLOC( slot, clazz->slot_object_size ) )
    {
      slot->face = face;

      error = ft_glyphslot_init( slot );
      ଯଦି ( error )
      {
        ft_glyphslot_done( slot );
        FT_FREE( slot );
        ଯାଅ Exit;
      }

      slot->next  = face->glyph;
      face->glyph = slot;

      ଯଦି ( aslot )
        *aslot = slot;
    }
    ଅନ୍ୟଥା ଯଦି ( aslot )
      *aslot = ଖାଲି;


  Exit:
    FT_TRACE4(( "FT_New_GlyphSlot: Return 0x%x\n", error ));

    ଫେରନ୍ତୁ error;
  }


  /* documentation is in ftobjs.h */

  FT_BASE_DEF( ଶୂନ୍ୟ )
  FT_Done_GlyphSlot( FT_GlyphSlot  slot )
  {
    ଯଦି ( slot )
    {
      FT_Driver     driver = slot->face->driver;
      FT_Memory     memory = driver->ମୂଳ.memory;
      FT_GlyphSlot  prev;
      FT_GlyphSlot  cur;


      /* Remove slot from its parent face's list */
      prev = ଖାଲି;
      cur  = slot->face->glyph;

      ଯେତେବେଳେ ( cur )
      {
        ଯଦି ( cur == slot )
        {
          ଯଦି ( !prev )
            slot->face->glyph = cur->next;
          ଅନ୍ୟଥା
            prev->next = cur->next;

          /* finalize client-specific data */
          ଯଦି ( slot->generic.finalizer )
            slot->generic.finalizer( slot );

          ft_glyphslot_done( slot );
          FT_FREE( slot );
          ଭାଙ୍ଗ;
        }
        prev = cur;
        cur  = cur->next;
      }
    }
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( ଶୂନ୍ୟ )
  FT_Set_Transform( FT_Face     face,
                    FT_Matrix*  matrix,
                    FT_Vector*  delta )
  {
    FT_Face_Internal  internal;


    ଯଦି ( !face )
      ଫେରନ୍ତୁ;

    internal = face->internal;

    internal->transform_flags = 0;

    ଯଦି ( !matrix || !FT_Matrix_Check( matrix ) )
    {
      internal->transform_matrix.xx = 0x10000L;
      internal->transform_matrix.xy = 0;
      internal->transform_matrix.yx = 0;
      internal->transform_matrix.yy = 0x10000L;

      matrix = &internal->transform_matrix;
    }
    ଅନ୍ୟଥା
      internal->transform_matrix = *matrix;

    /* set transform_flags bit flag 0 if `matrix' isn't the identity */
    ଯଦି ( ( matrix->xy | matrix->yx ) ||
         matrix->xx != 0x10000L      ||
         matrix->yy != 0x10000L      )
      internal->transform_flags |= 1;

    ଯଦି ( !delta )
    {
      internal->transform_delta.x = 0;
      internal->transform_delta.y = 0;

      delta = &internal->transform_delta;
    }
    ଅନ୍ୟଥା
      internal->transform_delta = *delta;

    /* set transform_flags bit flag 1 if `delta' isn't the null vector */
    ଯଦି ( delta->x | delta->y )
      internal->transform_flags |= 2;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( ଶୂନ୍ୟ )
  FT_Get_Transform( FT_Face     face,
                    FT_Matrix*  matrix,
                    FT_Vector*  delta )
  {
    FT_Face_Internal  internal;


    ଯଦି ( !face )
      ଫେରନ୍ତୁ;

    internal = face->internal;

    ଯଦି ( matrix )
      *matrix = internal->transform_matrix;

    ଯଦି ( delta )
      *delta = internal->transform_delta;
  }


  ସ୍ଥିର FT_Renderer
  ft_lookup_glyph_renderer( FT_GlyphSlot  slot );


#ifdef GRID_FIT_METRICS
  ସ୍ଥିର ଶୂନ୍ୟ
  ft_glyphslot_grid_fit_metrics( FT_GlyphSlot  slot,
                                 FT_Bool       vertical )
  {
    FT_Glyph_Metrics*  metrics = &slot->metrics;
    FT_Pos             ଡାହାଣ, bottom;


    ଯଦି ( vertical )
    {
      metrics->horiBearingX = FT_PIX_FLOOR( metrics->horiBearingX );
      metrics->horiBearingY = FT_PIX_CEIL_LONG( metrics->horiBearingY );

      ଡାହାଣ  = FT_PIX_CEIL_LONG( ADD_LONG( metrics->vertBearingX,
                                           metrics->width ) );
      bottom = FT_PIX_CEIL_LONG( ADD_LONG( metrics->vertBearingY,
                                           metrics->height ) );

      metrics->vertBearingX = FT_PIX_FLOOR( metrics->vertBearingX );
      metrics->vertBearingY = FT_PIX_FLOOR( metrics->vertBearingY );

      metrics->width  = SUB_LONG( ଡାହାଣ,
                                  metrics->vertBearingX );
      metrics->height = SUB_LONG( bottom,
                                  metrics->vertBearingY );
    }
    ଅନ୍ୟଥା
    {
      metrics->vertBearingX = FT_PIX_FLOOR( metrics->vertBearingX );
      metrics->vertBearingY = FT_PIX_FLOOR( metrics->vertBearingY );

      ଡାହାଣ  = FT_PIX_CEIL_LONG( ADD_LONG( metrics->horiBearingX,
                                           metrics->width ) );
      bottom = FT_PIX_FLOOR( SUB_LONG( metrics->horiBearingY,
                                       metrics->height ) );

      metrics->horiBearingX = FT_PIX_FLOOR( metrics->horiBearingX );
      metrics->horiBearingY = FT_PIX_CEIL_LONG( metrics->horiBearingY );

      metrics->width  = SUB_LONG( ଡାହାଣ,
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


    ଯଦି ( !face || !face->size || !face->glyph )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Face_Handle );

    /* The validity test for `glyph_index' is performed by the */
    /* font drivers.                                           */

    slot = face->glyph;
    ft_glyphslot_clear( slot );

    driver  = face->driver;
    library = driver->ମୂଳ.library;
    hinter  = library->auto_hinter;

    /* undefined scale means no scale */
    ଯଦି ( face->size->metrics.x_ppem == 0 ||
         face->size->metrics.y_ppem == 0 )
      load_flags |= FT_LOAD_NO_SCALE;

    /* resolve load flags dependencies */

    ଯଦି ( load_flags & FT_LOAD_NO_RECURSE )
      load_flags |= FT_LOAD_NO_SCALE         |
                    FT_LOAD_IGNORE_TRANSFORM;

    ଯଦି ( load_flags & FT_LOAD_NO_SCALE )
    {
      load_flags |= FT_LOAD_NO_HINTING |
                    FT_LOAD_NO_BITMAP;

      load_flags &= ~FT_LOAD_RENDER;
    }

    ଯଦି ( load_flags & FT_LOAD_BITMAP_METRICS_ONLY )
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

    ଯଦି ( hinter                                           &&
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
      ଯଦି ( ( load_flags & FT_LOAD_FORCE_AUTOHINT ) ||
           !FT_DRIVER_HAS_HINTER( driver )         )
        autohint = TRUE;
      ଅନ୍ୟଥା
      {
        FT_Render_Mode  mode = FT_LOAD_TARGET_MODE( load_flags );
        FT_Bool         is_light_type1;
        TT_Face         ttface = (TT_Face)face;


        /* only the new Adobe engine (for both CFF and Type 1) is `light'; */
        /* we use `strstr' to catch both `Type 1' and `CID Type 1'         */
        is_light_type1 =
          ft_strstr( FT_Get_Font_Format( face ), "Type 1" ) != ଖାଲି &&
          ((PS_Driver)driver)->hinting_engine == FT_HINTING_ADOBE;

        /* the check for `num_locations' assures that we actually    */
        /* test for instructions in a TTF and not in a CFF-based OTF */
        /*                                                           */
        /* we check the size of the `fpgm' and `prep' tables, too -- */
        /* the assumption is that there don't exist real TTFs where  */
        /* both `fpgm' and `prep' tables are missing                 */
        ଯଦି ( ( mode == FT_RENDER_MODE_LIGHT           &&
               ( !FT_DRIVER_HINTS_LIGHTLY( driver ) &&
                 !is_light_type1                    ) )         ||
             ( FT_IS_SFNT( face )                             &&
               ttface->ଅଙ୍କ_locations                          &&
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
    ଯଦି ( autohint && FT_IS_SFNT( face ) )
    {
      FT_Service_VARC  varc;


      FT_FACE_FIND_SERVICE( face, varc, VARC );
      ଯଦି ( varc && varc->has_glyph( face, glyph_index ) )
        autohint = FALSE;
    }
#endif

    ଯଦି ( autohint )
    {
      FT_AutoHinter_Interface  hinting;


      /* XXX: The use of the `FT_LOAD_XXX_ONLY` flags is not very */
      /*      elegant.                                            */

      /* try to load SVG documents if available */
      ଯଦି ( ( load_flags & FT_LOAD_NO_SVG ) == 0 &&
           FT_HAS_SVG( face )                   )
      {
        error = driver->clazz->load_glyph( slot, face->size,
                                           glyph_index,
                                           load_flags | FT_LOAD_SVG_ONLY );

        ଯଦି ( !error && slot->format == FT_GLYPH_FORMAT_SVG )
          ଯାଅ Load_Ok;
      }

      /* try to load embedded bitmaps if available */
      ଯଦି ( FT_HAS_FIXED_SIZES( face )              &&
           ( load_flags & FT_LOAD_NO_BITMAP ) == 0 )
      {
        error = driver->clazz->load_glyph( slot, face->size,
                                           glyph_index,
                                           load_flags | FT_LOAD_SBITS_ONLY );

        ଯଦି ( !error && slot->format == FT_GLYPH_FORMAT_BITMAP )
          ଯାଅ Load_Ok;
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
    ଅନ୍ୟଥା
    {
      error = driver->clazz->load_glyph( slot,
                                         face->size,
                                         glyph_index,
                                         load_flags );
      ଯଦି ( error )
        ଯାଅ Exit;

      ଯଦି ( slot->format == FT_GLYPH_FORMAT_OUTLINE )
      {
        /* check that the loaded outline is correct */
        error = FT_Outline_Check( &slot->outline );
        ଯଦି ( error )
          ଯାଅ Exit;

#ifdef GRID_FIT_METRICS
        ଯଦି ( !( load_flags & FT_LOAD_NO_HINTING ) )
          ft_glyphslot_grid_fit_metrics(
            slot,
            FT_BOOL( load_flags & FT_LOAD_VERTICAL_LAYOUT ) );
#endif
      }
    }

  Load_Ok:
    /* compute the advance */
    ଯଦି ( load_flags & FT_LOAD_VERTICAL_LAYOUT )
    {
      slot->advance.x = 0;
      slot->advance.y = slot->metrics.vertAdvance;
    }
    ଅନ୍ୟଥା
    {
      slot->advance.x = slot->metrics.horiAdvance;
      slot->advance.y = 0;
    }

    /* compute the linear advance in 16.16 pixels */
    ଯଦି ( ( load_flags & FT_LOAD_LINEAR_DESIGN ) == 0 &&
         FT_IS_SCALABLE( face )                      )
    {
      FT_Size_Metrics*  metrics = &face->size->metrics;


      /* it's tricky! */
      slot->linearHoriAdvance = FT_MulDiv( slot->linearHoriAdvance,
                                           metrics->x_scale, 64 );

      slot->linearVertAdvance = FT_MulDiv( slot->linearVertAdvance,
                                           metrics->y_scale, 64 );
    }

    ଯଦି ( ( load_flags & FT_LOAD_IGNORE_TRANSFORM ) == 0 )
    {
      FT_Face_Internal  internal = face->internal;


      /* now, transform the glyph image if needed */
      ଯଦି ( internal->transform_flags )
      {
        /* get renderer */
        FT_Renderer  renderer = ft_lookup_glyph_renderer( slot );


        ଯଦି ( renderer )
          error = renderer->clazz->transform_glyph(
                                     renderer, slot,
                                     &internal->transform_matrix,
                                     &internal->transform_delta );
        ଅନ୍ୟଥା ଯଦି ( slot->format == FT_GLYPH_FORMAT_OUTLINE )
        {
          /* apply `standard' transformation if no renderer is available */
          ଯଦି ( internal->transform_flags & 1 )
            FT_Outline_Transform( &slot->outline,
                                  &internal->transform_matrix );

          ଯଦି ( internal->transform_flags & 2 )
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
    ଯଦି ( !error                                    &&
         ( load_flags & FT_LOAD_NO_SCALE ) == 0    &&
         slot->format != FT_GLYPH_FORMAT_BITMAP    &&
         slot->format != FT_GLYPH_FORMAT_COMPOSITE )
    {
      FT_Render_Mode  mode = FT_LOAD_TARGET_MODE( load_flags );


      ଯଦି ( mode == FT_RENDER_MODE_NORMAL   &&
           load_flags & FT_LOAD_MONOCHROME )
        mode = FT_RENDER_MODE_MONO;

      ଯଦି ( load_flags & FT_LOAD_RENDER )
        error = FT_Render_Glyph( slot, mode );
      ଅନ୍ୟଥା
        ft_glyphslot_preset_bitmap( slot, mode, ଖାଲି );
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
    FT_TRACE5(( "  x advance: %f\n", (ଦ୍ୱିଗୁଣ)slot->advance.x / 64 ));
    FT_TRACE5(( "  y advance: %f\n", (ଦ୍ୱିଗୁଣ)slot->advance.y / 64 ));
    FT_TRACE5(( "  linear x advance: %f\n",
                (ଦ୍ୱିଗୁଣ)slot->linearHoriAdvance / 65536 ));
    FT_TRACE5(( "  linear y advance: %f\n",
                (ଦ୍ୱିଗୁଣ)slot->linearVertAdvance / 65536 ));

    {
      FT_Glyph_Metrics*  metrics = &slot->metrics;


      FT_TRACE5(( "  metrics:\n" ));
      FT_TRACE5(( "    width:  %f\n", (ଦ୍ୱିଗୁଣ)metrics->width / 64 ));
      FT_TRACE5(( "    height: %f\n", (ଦ୍ୱିଗୁଣ)metrics->height / 64 ));
      FT_TRACE5(( "\n" ));
      FT_TRACE5(( "    horiBearingX: %f\n",
                  (ଦ୍ୱିଗୁଣ)metrics->horiBearingX / 64 ));
      FT_TRACE5(( "    horiBearingY: %f\n",
                  (ଦ୍ୱିଗୁଣ)metrics->horiBearingY / 64 ));
      FT_TRACE5(( "    horiAdvance:  %f\n",
                  (ଦ୍ୱିଗୁଣ)metrics->horiAdvance / 64 ));
      FT_TRACE5(( "\n" ));
      FT_TRACE5(( "    vertBearingX: %f\n",
                  (ଦ୍ୱିଗୁଣ)metrics->vertBearingX / 64 ));
      FT_TRACE5(( "    vertBearingY: %f\n",
                  (ଦ୍ୱିଗୁଣ)metrics->vertBearingY / 64 ));
      FT_TRACE5(( "    vertAdvance:  %f\n",
                  (ଦ୍ୱିଗୁଣ)metrics->vertAdvance / 64 ));
    }
#endif

  Exit:
    ଫେରନ୍ତୁ error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Load_Char( FT_Face   face,
                FT_ULong  char_code,
                FT_Int32  load_flags )
  {
    FT_UInt  glyph_index;


    ଯଦି ( !face )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Face_Handle );

    glyph_index = (FT_UInt)char_code;
    ଯଦି ( face->charmap )
      glyph_index = FT_Get_Char_Index( face, char_code );

    ଫେରନ୍ତୁ FT_Load_Glyph( face, glyph_index, load_flags );
  }


  /* destructor for sizes list */
  ସ୍ଥିର ଶୂନ୍ୟ
  destroy_size( FT_Memory  memory,
                ଶୂନ୍ୟ*      size_,
                ଶୂନ୍ୟ*      driver_ )
  {
    FT_Size    size   = (FT_Size)size_;
    FT_Driver  driver = (FT_Driver)driver_;


    /* finalize format-specific stuff */
    ଯଦି ( driver->clazz->done_size )
      driver->clazz->done_size( size );

    /* finalize client-specific data */
    ଯଦି ( size->generic.finalizer )
      size->generic.finalizer( size );

    FT_FREE( size->internal );
    FT_FREE( size );
  }


  ସ୍ଥିର ଶୂନ୍ୟ
  ft_cmap_done_internal( FT_CMap  cmap );


  ସ୍ଥିର ଶୂନ୍ୟ
  destroy_charmaps( FT_Face    face,
                    FT_Memory  memory )
  {
    FT_Int  n;


    ଯଦି ( !face )
      ଫେରନ୍ତୁ;

    ପାଇଁ ( n = 0; n < face->ଅଙ୍କ_charmaps; n++ )
    {
      FT_CMap  cmap = FT_CMAP( face->charmaps[n] );


      ft_cmap_done_internal( cmap );

      face->charmaps[n] = ଖାଲି;
    }

    FT_FREE( face->charmaps );
    face->ଅଙ୍କ_charmaps = 0;
  }


  /* destructor for faces list */
  ସ୍ଥିର ଶୂନ୍ୟ
  destroy_face( FT_Memory  memory,
                ଶୂନ୍ୟ*      face_,
                ଶୂନ୍ୟ*      driver_ )
  {
    FT_Face          face   = (FT_Face)face_;
    FT_Driver        driver = (FT_Driver)driver_;
    FT_Driver_Class  clazz  = driver->clazz;


    /* discard auto-hinting data */
    ଯଦି ( face->autohint.finalizer )
      face->autohint.finalizer( face->autohint.data );

    /* Discard glyph slots for this face.                           */
    /* Beware!  FT_Done_GlyphSlot() changes the field `face->glyph' */
    ଯେତେବେଳେ ( face->glyph )
      FT_Done_GlyphSlot( face->glyph );

    /* discard all sizes for this face */
    FT_List_Finalize( &face->sizes_list,
                      destroy_size,
                      memory,
                      driver );
    face->size = ଖାଲି;

    /* discard charmaps */
    destroy_charmaps( face, memory );

    /* finalize format-specific stuff */
    ଯଦି ( clazz->done_face )
      clazz->done_face( face );

    /* close the stream for this face if needed */
    FT_Stream_Free(
      face->stream,
      ( face->face_flags & FT_FACE_FLAG_EXTERNAL_STREAM ) != 0 );

    face->stream = ଖାଲି;

    /* now discard client data */
    ଯଦି ( face->generic.finalizer )
      face->generic.finalizer( face );

    /* get rid of it */
    ଯଦି ( face->internal )
    {
      FT_FREE( face->internal );
    }
    FT_FREE( face );
  }


  ସ୍ଥିର ଶୂନ୍ୟ
  Destroy_Driver( FT_Driver  driver )
  {
    FT_List_Finalize( &driver->faces_list,
                      destroy_face,
                      driver->ମୂଳ.memory,
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

    ଯଦି ( !first )
      ଫେରନ୍ତୁ FT_THROW( Invalid_CharMap_Handle );

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

    cur = first + face->ଅଙ୍କ_charmaps;  /* points after the last one */

    ପାଇଁ ( ; --cur >= first; )
    {
      ଯଦି ( cur[0]->encoding == FT_ENCODING_UNICODE )
      {
        /* XXX If some new encodings to represent UCS-4 are added, */
        /*     they should be added here.                          */
        ଯଦି ( ( cur[0]->platform_id == TT_PLATFORM_MICROSOFT &&
               cur[0]->encoding_id == TT_MS_ID_UCS_4        )     ||
             ( cur[0]->platform_id == TT_PLATFORM_APPLE_UNICODE &&
               cur[0]->encoding_id == TT_APPLE_ID_UNICODE_32    ) ||
             ( cur[0]->platform_id == TT_PLATFORM_APPLE_UNICODE &&
               cur[0]->encoding_id == TT_APPLE_ID_FULL_UNICODE  &&
               FT_Get_CMap_Format( cur[0] ) == 13               ) )
        {
          face->charmap = cur[0];
          ଫେରନ୍ତୁ FT_Err_Ok;
        }
      }
    }

    /* We do not have any UCS-4 charmap.                */
    /* Do the loop again and search for UCS-2 charmaps. */
    cur = first + face->ଅଙ୍କ_charmaps;

    ପାଇଁ ( ; --cur >= first; )
    {
      ଯଦି ( cur[0]->encoding == FT_ENCODING_UNICODE )
      {
        FT_Long  format = FT_Get_CMap_Format( cur[0] );


        ଯଦି ( format != 14 && format != 15 )
        {
          face->charmap = cur[0];
          ଫେରନ୍ତୁ FT_Err_Ok;
        }
      }
    }

    ଫେରନ୍ତୁ FT_THROW( Invalid_CharMap_Handle );
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
  ସ୍ଥିର FT_CharMap
  find_variant_selector_charmap( FT_Face  face )
  {
    FT_CharMap*  first;
    FT_CharMap*  end;
    FT_CharMap*  cur;


    /* caller should have already checked that `face' is valid */
    FT_ASSERT( face );

    first = face->charmaps;

    ଯଦି ( !first )
      ଫେରନ୍ତୁ ଖାଲି;

    end = first + face->ଅଙ୍କ_charmaps;  /* points after the last one */

    ପାଇଁ ( cur = first; cur < end; cur++ )
    {
      ଯଦି ( cur[0]->platform_id == TT_PLATFORM_APPLE_UNICODE    &&
           cur[0]->encoding_id == TT_APPLE_ID_VARIANT_SELECTOR )
      {
        FT_Long  format = FT_Get_CMap_Format( cur[0] );


        ଯଦି ( format == 14 || format == 15 )
          ଫେରନ୍ତୁ cur[0];
      }
    }

    ଫେରନ୍ତୁ ଖାଲି;
  }


  /**************************************************************************
   *
   * @Function:
   *   open_face
   *
   * @Description:
   *   This function does some work for FT_Open_Face().
   */
  ସ୍ଥିର FT_Error
  open_face( FT_Driver      driver,
             FT_Stream      *astream,
             FT_Bool        *anexternal_stream,
             FT_Long        face_index,
             FT_Int         ଅଙ୍କ_params,
             FT_Parameter*  params,
             FT_Face       *aface )
  {
    FT_Memory         memory;
    FT_Driver_Class   clazz;
    FT_Face           face     = ଖାଲି;
    FT_Face_Internal  internal = ଖାଲି;

    FT_Error          error, error2;


    clazz  = driver->clazz;
    memory = driver->ମୂଳ.memory;

    /* allocate the face object and perform basic initialization */
    ଯଦି ( FT_ALLOC( face, clazz->face_object_size ) )
      ଯାଅ Fail;

    face->driver = driver;
    face->memory = memory;
    face->stream = *astream;

    /* set the FT_FACE_FLAG_EXTERNAL_STREAM bit for FT_Done_Face */
    ଯଦି ( *anexternal_stream )
      face->face_flags |= FT_FACE_FLAG_EXTERNAL_STREAM;

    ଯଦି ( FT_NEW( internal ) )
      ଯାଅ Fail;

    face->internal = internal;

#ifdef FT_CONFIG_OPTION_INCREMENTAL
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା  i;


      face->internal->incremental_interface = ଖାଲି;
      ପାଇଁ ( i = 0; i < ଅଙ୍କ_params && !face->internal->incremental_interface;
            i++ )
        ଯଦି ( params[i].tag == FT_PARAM_TAG_INCREMENTAL )
          face->internal->incremental_interface =
            (FT_Incremental_Interface)params[i].data;
    }
#endif

    face->internal->random_seed = -1;

    ଯଦି ( clazz->init_face )
      error = clazz->init_face( *astream,
                                face,
                                (FT_Int)face_index,
                                ଅଙ୍କ_params,
                                params );
    /* Stream may have been changed. */
    *astream = face->stream;
    *anexternal_stream =
      ( face->face_flags & FT_FACE_FLAG_EXTERNAL_STREAM ) != 0;
    ଯଦି ( error )
      ଯାଅ Fail;

    /* select Unicode charmap by default */
    error2 = find_unicode_charmap( face );

    /* if no Unicode charmap can be found, FT_Err_Invalid_CharMap_Handle */
    /* is returned.                                                      */

    /* no error should happen, but we want to play safe */
    ଯଦି ( error2 && FT_ERR_NEQ( error2, Invalid_CharMap_Handle ) )
    {
      error = error2;
      ଯାଅ Fail;
    }

    *aface = face;

  Fail:
    ଯଦି ( error )
    {
      destroy_charmaps( face, memory );
      ଯଦି ( clazz->done_face )
        clazz->done_face( face );
      FT_FREE( internal );
      FT_FREE( face );
      *aface = ଖାଲି;
    }

    ଫେରନ୍ତୁ error;
  }


  /* there's a Mac-specific extended implementation of FT_New_Face() */
  /* in src/base/ftmac.c                                             */

#ifndef FT_MACINTOSH

  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_New_Face( FT_Library   library,
               ଅଚଳ ଅକ୍ଷର*  pathname,
               FT_Long      face_index,
               FT_Face     *aface )
  {
    FT_Open_Args  args;


    /* test for valid `library' and `aface' delayed to `FT_Open_Face' */
    ଯଦି ( !pathname )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Argument );

    args.flags    = FT_OPEN_PATHNAME;
    args.pathname = (ଅକ୍ଷର*)pathname;
    args.stream   = ଖାଲି;

    ଫେରନ୍ତୁ ft_open_face_internal( library, &args, face_index, aface, 1 );
  }

#endif


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_New_Memory_Face( FT_Library      library,
                      ଅଚଳ FT_Byte*  file_base,
                      FT_Long         file_size,
                      FT_Long         face_index,
                      FT_Face        *aface )
  {
    FT_Open_Args  args;


    /* test for valid `library' and `face' delayed to `FT_Open_Face' */
    ଯଦି ( !file_base )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Argument );

    args.flags       = FT_OPEN_MEMORY;
    args.memory_base = file_base;
    args.memory_size = file_size;
    args.stream      = ଖାଲି;

    ଫେରନ୍ତୁ ft_open_face_internal( library, &args, face_index, aface, 1 );
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
  ସ୍ଥିର ଶୂନ୍ୟ
  memory_stream_close( FT_Stream  stream )
  {
    FT_Memory  memory = (FT_Memory)stream->descriptor.pointer;


    FT_FREE( stream->base );
    stream->size  = 0;
    stream->close = ଖାଲି;
    FT_FREE( stream );
  }


  /* Create a new memory stream from a buffer and a size. */
  /* From `ftmac.c'.                                      */
  ସ୍ଥିର FT_Error
  new_memory_stream( FT_Library           library,
                     FT_Byte*             base,
                     FT_ULong             size,
                     FT_Stream_CloseFunc  close,
                     FT_Stream           *astream )
  {
    FT_Error   error;
    FT_Memory  memory;
    FT_Stream  stream = ଖାଲି;


    ଯଦି ( !library )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Library_Handle );

    ଯଦି ( !base )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Argument );

    *astream = ଖାଲି;
    memory   = library->memory;
    ଯଦି ( FT_NEW( stream ) )
      ଯାଅ Exit;

    FT_Stream_OpenMemory( stream, base, size );

    stream->descriptor.pointer = memory;
    stream->close              = close;

    *astream = stream;

  Exit:
    ଫେରନ୍ତୁ error;
  }


  /* Create a new FT_Face given a buffer and a driver name. */
  /* From `ftmac.c'.                                        */
  FT_LOCAL_DEF( FT_Error )
  open_face_from_buffer( FT_Library   library,
                         FT_Byte*     base,
                         FT_ULong     size,
                         FT_Long      face_index,
                         ଅଚଳ ଅକ୍ଷର*  driver_name,
                         FT_Face     *aface )
  {
    FT_Open_Args  args;
    FT_Error      error;
    FT_Memory     memory = library->memory;


    args.driver = ଖାଲି;
    args.flags  = 0;

    ଯଦି ( driver_name )
    {
      args.driver = FT_Get_Module( library, driver_name );
      ଯଦି ( !args.driver )
      {
        FT_FREE( base );
        ଫେରନ୍ତୁ FT_THROW( Missing_Module );
      }

      args.flags = args.flags | FT_OPEN_DRIVER;
    }

    /* `memory_stream_close` also frees the stream object. */
    error = new_memory_stream( library,
                               base,
                               size,
                               memory_stream_close,
                               &args.stream );
    ଯଦି ( error )
    {
      FT_FREE( base );
      ଫେରନ୍ତୁ error;
    }

    args.flags |= FT_OPEN_STREAM;

#ifdef FT_MACINTOSH
    /* At this point, the face index has served its purpose;  */
    /* whoever calls this function has already used it to     */
    /* locate the correct font data.  We should not propagate */
    /* this index to FT_Open_Face() (unless it is negative).  */

    ଯଦି ( face_index > 0 )
      face_index &= 0x7FFF0000L; /* retain GX data */
#endif

    ଫେରନ୍ତୁ ft_open_face_internal( library, &args, face_index, aface, 0 );
  }


  /* Look up `TYP1' or `CID ' table from sfnt table directory.       */
  /* `offset' and `length' must exclude the binary header in tables. */

  /* Type 1 and CID-keyed font drivers should recognize sfnt-wrapped */
  /* format too.  Here, since we can't expect that the TrueType font */
  /* driver is loaded unconditionally, we must parse the font by     */
  /* ourselves.  We are only interested in the name of the table and */
  /* the offset.                                                     */

  ସ୍ଥିର FT_Error
  ft_lookup_PS_in_sfnt_stream( FT_Stream  stream,
                               FT_Long    face_index,
                               FT_ULong*  offset,
                               FT_ULong*  length,
                               FT_Bool*   is_sfnt_cid )
  {
    FT_Error   error;
    FT_UShort  ଅଙ୍କ_Tables;
    FT_Long    pstable_index;
    FT_ULong   tag;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା        i;


    *offset = 0;
    *length = 0;
    *is_sfnt_cid = FALSE;

    /* TODO: support for sfnt-wrapped PS/CID in TTC format */

    /* version check for 'typ1' (should be ignored?) */
    ଯଦି ( FT_READ_ULONG( tag ) )
      ଫେରନ୍ତୁ error;
    ଯଦି ( tag != TTAG_typ1 )
      ଫେରନ୍ତୁ FT_THROW( Unknown_File_Format );

    ଯଦି ( FT_READ_USHORT( ଅଙ୍କ_Tables ) )
      ଫେରନ୍ତୁ error;
    ଯଦି ( FT_STREAM_SKIP( 2 * 3 ) ) /* skip binary search header */
      ଫେରନ୍ତୁ error;

    pstable_index = -1;
    *is_sfnt_cid  = FALSE;

    ପାଇଁ ( i = 0; i < ଅଙ୍କ_Tables; i++ )
    {
      ଯଦି ( FT_READ_ULONG( tag )     || FT_STREAM_SKIP( 4 )      ||
           FT_READ_ULONG( *offset ) || FT_READ_ULONG( *length ) )
        ଫେରନ୍ତୁ error;

      ଯଦି ( tag == TTAG_CID )
      {
        pstable_index++;
        *offset += 22;
        *length -= 22;
        *is_sfnt_cid = TRUE;
        ଯଦି ( face_index < 0 )
          ଫେରନ୍ତୁ FT_Err_Ok;
      }
      ଅନ୍ୟଥା ଯଦି ( tag == TTAG_TYP1 )
      {
        pstable_index++;
        *offset += 24;
        *length -= 24;
        *is_sfnt_cid = FALSE;
        ଯଦି ( face_index < 0 )
          ଫେରନ୍ତୁ FT_Err_Ok;
      }
      ଯଦି ( face_index >= 0 && pstable_index == face_index )
        ଫେରନ୍ତୁ FT_Err_Ok;
    }

    ଫେରନ୍ତୁ FT_THROW( Table_Missing );
  }


  FT_LOCAL_DEF( FT_Error )
  open_face_PS_from_sfnt_stream( FT_Library     library,
                                 FT_Stream      stream,
                                 FT_Long        face_index,
                                 FT_Int         ଅଙ୍କ_params,
                                 FT_Parameter  *params,
                                 FT_Face       *aface )
  {
    FT_Error   error;
    FT_Memory  memory = library->memory;
    FT_ULong   offset, length;
    FT_ULong   pos;
    FT_Bool    is_sfnt_cid;
    FT_Byte*   sfnt_ps = ଖାଲି;

    FT_UNUSED( ଅଙ୍କ_params );
    FT_UNUSED( params );


    /* ignore GX stuff */
    ଯଦି ( face_index > 0 )
      face_index &= 0xFFFFL;

    pos = FT_STREAM_POS();

    error = ft_lookup_PS_in_sfnt_stream( stream,
                                         face_index,
                                         &offset,
                                         &length,
                                         &is_sfnt_cid );
    ଯଦି ( error )
      ଯାଅ Exit;

    ଯଦି ( offset > stream->size )
    {
      FT_TRACE2(( "open_face_PS_from_sfnt_stream: invalid table offset\n" ));
      error = FT_THROW( Invalid_Table );
      ଯାଅ Exit;
    }
    ଅନ୍ୟଥା ଯଦି ( length > stream->size - offset )
    {
      FT_TRACE2(( "open_face_PS_from_sfnt_stream: invalid table length\n" ));
      error = FT_THROW( Invalid_Table );
      ଯାଅ Exit;
    }

    error = FT_Stream_Seek( stream, pos + offset );
    ଯଦି ( error )
      ଯାଅ Exit;

    ଯଦି ( FT_QALLOC( sfnt_ps, (FT_Long)length ) )
      ଯାଅ Exit;

    error = FT_Stream_Read( stream, (FT_Byte *)sfnt_ps, length );
    ଯଦି ( error )
    {
      FT_FREE( sfnt_ps );
      ଯାଅ Exit;
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


      ଯଦି ( FT_ERR_EQ( error, Unknown_File_Format ) )
      {
        error1 = FT_Stream_Seek( stream, pos );
        ଯଦି ( error1 )
          ଫେରନ୍ତୁ error1;
      }

      ଫେରନ୍ତୁ error;
    }
  }


#ifndef FT_MACINTOSH

  /* The resource header says we've got resource_cnt `POST' (type1) */
  /* resources in this file.  They all need to be coalesced into    */
  /* one lump which gets passed on to the type1 driver.             */
  /* Here can be only one PostScript font in a file so face_index   */
  /* must be 0 (or -1).                                             */
  /*                                                                */
  ସ୍ଥିର FT_Error
  Mac_Read_POST_Resource( FT_Library  library,
                          FT_Stream   stream,
                          FT_Long    *offsets,
                          FT_Long     resource_cnt,
                          FT_Long     face_index,
                          FT_Face    *aface )
  {
    FT_Error   error  = FT_ERR( Cannot_Open_Resource );
    FT_Memory  memory = library->memory;

    FT_Byte*   pfb_data = ଖାଲି;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା        i, type, flags;
    FT_ULong   len;
    FT_ULong   pfb_len, pfb_pos, pfb_lenpos;
    FT_ULong   rlen, temp;


    ଯଦି ( face_index == -1 )
      face_index = 0;
    ଯଦି ( face_index != 0 )
      ଫେରନ୍ତୁ error;

    /* Find the length of all the POST resources, concatenated.  Assume */
    /* worst case (each resource in its own section).                   */
    pfb_len = 0;
    ପାଇଁ ( i = 0; i < resource_cnt; i++ )
    {
      error = FT_Stream_Seek( stream, (FT_ULong)offsets[i] );
      ଯଦି ( error )
        ଯାଅ Exit;
      ଯଦି ( FT_READ_ULONG( temp ) )  /* actually LONG */
        ଯାଅ Exit;

      /* FT2 allocator takes signed long buffer length,
       * too large value causing overflow should be checked
       */
      FT_TRACE4(( "                 POST fragment #%d: length=0x%08lx"
                  " total pfb_len=0x%08lx\n",
                  i, temp, pfb_len + temp + 6 ));

      ଯଦି ( FT_MAC_RFORK_MAX_LEN < temp               ||
           FT_MAC_RFORK_MAX_LEN - temp < pfb_len + 6 )
      {
        FT_TRACE2(( "             MacOS resource length cannot exceed"
                    " 0x%08lx\n",
                    FT_MAC_RFORK_MAX_LEN ));

        error = FT_THROW( Invalid_Offset );
        ଯାଅ Exit;
      }

      pfb_len += temp + 6;
    }

    FT_TRACE2(( "             total buffer size to concatenate"
                " %ld POST fragments: 0x%08lx\n",
                 resource_cnt, pfb_len + 2 ));

    ଯଦି ( pfb_len + 2 < 6 )
    {
      FT_TRACE2(( "             too long fragment length makes"
                  " pfb_len confused: pfb_len=0x%08lx\n",
                  pfb_len ));

      error = FT_THROW( Array_Too_Large );
      ଯାଅ Exit;
    }

    ଯଦି ( FT_QALLOC( pfb_data, (FT_Long)pfb_len + 2 ) )
      ଯାଅ Exit;

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

    ପାଇଁ ( i = 0; i < resource_cnt; i++ )
    {
      error = FT_Stream_Seek( stream, (FT_ULong)offsets[i] );
      ଯଦି ( error )
        ଯାଅ Exit2;
      ଯଦି ( FT_READ_ULONG( rlen ) )
        ଯାଅ Exit2;

      /* FT2 allocator takes signed long buffer length,
       * too large fragment length causing overflow should be checked
       */
      ଯଦି ( 0x7FFFFFFFUL < rlen )
      {
        error = FT_THROW( Invalid_Offset );
        ଯାଅ Exit2;
      }

      ଯଦି ( FT_READ_USHORT( flags ) )
        ଯାଅ Exit2;

      FT_TRACE3(( "POST fragment[%d]:"
                  " offsets=0x%08lx, rlen=0x%08lx, flags=0x%04x\n",
                  i, offsets[i], rlen, flags ));

      error = FT_ERR( Array_Too_Large );

      /* postpone the check of `rlen longer than buffer' */
      /* until `FT_Stream_Read'                          */

      ଯଦି ( ( flags >> 8 ) == 0 )        /* Comment, should not be loaded */
      {
        FT_TRACE3(( "    Skip POST fragment #%d because it is a comment\n",
                    i ));
        ଜାରି;
      }

      /* the flags are part of the resource, so rlen >= 2,  */
      /* but some fonts declare rlen = 0 for empty fragment */
      ଯଦି ( rlen > 2 )
        rlen -= 2;
      ଅନ୍ୟଥା
        rlen = 0;

      ଯଦି ( ( flags >> 8 ) == type )
        len += rlen;
      ଅନ୍ୟଥା
      {
        FT_TRACE3(( "    Write POST fragment #%d header (4-byte) to buffer"
                    " %p + 0x%08lx\n",
                    i, (ଶୂନ୍ୟ*)pfb_data, pfb_lenpos ));

        ଯଦି ( pfb_lenpos + 3 > pfb_len + 2 )
          ଯାଅ Exit2;

        pfb_data[pfb_lenpos    ] = (FT_Byte)( len );
        pfb_data[pfb_lenpos + 1] = (FT_Byte)( len >> 8 );
        pfb_data[pfb_lenpos + 2] = (FT_Byte)( len >> 16 );
        pfb_data[pfb_lenpos + 3] = (FT_Byte)( len >> 24 );

        ଯଦି ( ( flags >> 8 ) == 5 )      /* End of font mark */
          ଭାଙ୍ଗ;

        FT_TRACE3(( "    Write POST fragment #%d header (6-byte) to buffer"
                    " %p + 0x%08lx\n",
                    i, (ଶୂନ୍ୟ*)pfb_data, pfb_pos ));

        ଯଦି ( pfb_pos + 6 > pfb_len + 2 )
          ଯାଅ Exit2;

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

      ଯଦି ( pfb_pos > pfb_len || pfb_pos + rlen > pfb_len )
        ଯାଅ Exit2;

      FT_TRACE3(( "    Load POST fragment #%d (%lu byte) to buffer"
                  " %p + 0x%08lx\n",
                  i, rlen, (ଶୂନ୍ୟ*)pfb_data, pfb_pos ));

      error = FT_Stream_Read( stream, (FT_Byte *)pfb_data + pfb_pos, rlen );
      ଯଦି ( error )
        ଯାଅ Exit2;

      pfb_pos += rlen;
    }

    error = FT_ERR( Array_Too_Large );

    ଯଦି ( pfb_pos + 2 > pfb_len + 2 )
      ଯାଅ Exit2;
    pfb_data[pfb_pos++] = 0x80;
    pfb_data[pfb_pos++] = 3;

    ଯଦି ( pfb_lenpos + 3 > pfb_len + 2 )
      ଯାଅ Exit2;
    pfb_data[pfb_lenpos    ] = (FT_Byte)( len );
    pfb_data[pfb_lenpos + 1] = (FT_Byte)( len >> 8 );
    pfb_data[pfb_lenpos + 2] = (FT_Byte)( len >> 16 );
    pfb_data[pfb_lenpos + 3] = (FT_Byte)( len >> 24 );

    ଫେରନ୍ତୁ open_face_from_buffer( library,
                                  pfb_data,
                                  pfb_pos,
                                  face_index,
                                  "type1",
                                  aface );

  Exit2:
    ଯଦି ( FT_ERR_EQ( error, Array_Too_Large ) )
      FT_TRACE2(( "  Abort due to too-short buffer to store"
                  " all POST fragments\n" ));
    ଅନ୍ୟଥା ଯଦି ( FT_ERR_EQ( error, Invalid_Offset ) )
      FT_TRACE2(( "  Abort due to invalid offset in a POST fragment\n" ));

    ଯଦି ( error )
      error = FT_ERR( Cannot_Open_Resource );
    FT_FREE( pfb_data );

  Exit:
    ଫେରନ୍ତୁ error;
  }


  /* The resource header says we've got resource_cnt `sfnt'      */
  /* (TrueType/OpenType) resources in this file.  Look through   */
  /* them for the one indicated by face_index, load it into mem, */
  /* pass it on to the truetype driver, and return it.           */
  /*                                                             */
  ସ୍ଥିର FT_Error
  Mac_Read_sfnt_Resource( FT_Library  library,
                          FT_Stream   stream,
                          FT_Long    *offsets,
                          FT_Long     resource_cnt,
                          FT_Long     face_index,
                          FT_Face    *aface )
  {
    FT_Memory  memory = library->memory;
    FT_Byte*   sfnt_data = ଖାଲି;
    FT_Error   error;
    FT_ULong   flag_offset;
    FT_ULong   rlen;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା        is_cff;
    FT_Long    face_index_in_resource = 0;


    ଯଦି ( face_index < 0 )
      face_index = -face_index - 1;
    ଯଦି ( face_index >= resource_cnt )
      ଫେରନ୍ତୁ FT_THROW( Cannot_Open_Resource );

    flag_offset = (FT_ULong)offsets[face_index];
    error = FT_Stream_Seek( stream, flag_offset );
    ଯଦି ( error )
      ଯାଅ Exit;

    ଯଦି ( FT_READ_ULONG( rlen ) )
      ଯାଅ Exit;
    ଯଦି ( !rlen )
      ଫେରନ୍ତୁ FT_THROW( Cannot_Open_Resource );
    ଯଦି ( rlen > FT_MAC_RFORK_MAX_LEN )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Offset );

    error = open_face_PS_from_sfnt_stream( library,
                                           stream,
                                           face_index,
                                           0, ଖାଲି,
                                           aface );
    ଯଦି ( !error )
      ଯାଅ Exit;

    /* rewind sfnt stream before open_face_PS_from_sfnt_stream() */
    error = FT_Stream_Seek( stream, flag_offset + 4 );
    ଯଦି ( error )
      ଯାଅ Exit;

    ଯଦି ( FT_QALLOC( sfnt_data, rlen ) )
      ଫେରନ୍ତୁ error;
    error = FT_Stream_Read( stream, (FT_Byte *)sfnt_data, rlen );
    ଯଦି ( error )
    {
      FT_FREE( sfnt_data );
      ଯାଅ Exit;
    }

    is_cff = rlen > 4 && !ft_memcmp( sfnt_data, "OTTO", 4 );
    error = open_face_from_buffer( library,
                                   sfnt_data,
                                   rlen,
                                   face_index_in_resource,
                                   is_cff ? "cff" : "truetype",
                                   aface );

  Exit:
    ଫେରନ୍ତୁ error;
  }


  /* Check for a valid resource fork header, or a valid dfont    */
  /* header.  In a resource fork the first 16 bytes are repeated */
  /* at the location specified by bytes 4-7.  In a dfont bytes   */
  /* 4-7 point to 16 bytes of zeroes instead.                    */
  /*                                                             */
  ସ୍ଥିର FT_Error
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
    FT_Long    ଗଣନା_2;


    error = FT_Raccess_Get_HeaderInfo( library, stream, resource_offset,
                                       &map_offset, &rdata_pos );
    ଯଦି ( error )
      ଫେରନ୍ତୁ error;

    /* POST resources must be sorted to concatenate properly */
    error = FT_Raccess_Get_DataOffsets( library, stream,
                                        map_offset, rdata_pos,
                                        TTAG_POST, TRUE,
                                        &data_offsets, &ଗଣନା_2 );
    ଯଦି ( !error )
    {
      error = Mac_Read_POST_Resource( library, stream, data_offsets, ଗଣନା_2,
                                      face_index, aface );
      FT_FREE( data_offsets );
      /* POST exists in an LWFN providing a single face */
      ଯଦି ( !error )
        (*aface)->ଅଙ୍କ_faces = 1;
      ଫେରନ୍ତୁ error;
    }

    /* sfnt resources should not be sorted to preserve the face order by
       QuickDraw API */
    error = FT_Raccess_Get_DataOffsets( library, stream,
                                        map_offset, rdata_pos,
                                        TTAG_sfnt, FALSE,
                                        &data_offsets, &ଗଣନା_2 );
    ଯଦି ( !error )
    {
      FT_Long  face_index_internal = face_index % ଗଣନା_2;


      error = Mac_Read_sfnt_Resource( library, stream, data_offsets, ଗଣନା_2,
                                      face_index_internal, aface );
      FT_FREE( data_offsets );
      ଯଦି ( !error )
      {
        (*aface)->ଅଙ୍କ_faces  = ଗଣନା_2;
        (*aface)->face_index = face_index_internal;
      }
    }

    ଫେରନ୍ତୁ error;
  }


  /* Check for a valid macbinary header, and if we find one   */
  /* check that the (flattened) resource fork in it is valid. */
  /*                                                          */
  ସ୍ଥିର FT_Error
  Is_Mac_ଦ୍ୱିମିକ( FT_Library  library,
               FT_Stream   stream,
               FT_Long     face_index,
               FT_Face    *aface )
  {
    unsigned ଅକ୍ଷର  header[128];
    FT_Error       error;
    FT_Long        dlen, offset;


    ଯଦି ( !stream )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Stream_Operation );

    error = FT_Stream_Seek( stream, 0 );
    ଯଦି ( error )
      ଯାଅ Exit;

    error = FT_Stream_Read( stream, (FT_Byte*)header, 128 );
    ଯଦି ( error )
      ଯାଅ Exit;

    ଯଦି (            header[ 0] !=   0 ||
                    header[74] !=   0 ||
                    header[82] !=   0 ||
                    header[ 1] ==   0 ||
                    header[ 1] >   33 ||
                    header[63] !=   0 ||
         header[2 + header[1]] !=   0 ||
                  header[0x53] > 0x7F )
      ଫେରନ୍ତୁ FT_THROW( Unknown_File_Format );

    dlen = ( header[0x53] << 24 ) |
           ( header[0x54] << 16 ) |
           ( header[0x55] <<  8 ) |
             header[0x56];
#ଯଦି 0
    rlen = ( header[0x57] << 24 ) |
           ( header[0x58] << 16 ) |
           ( header[0x59] <<  8 ) |
             header[0x5A];
#endif /* 0 */
    offset = 128 + ( ( dlen + 127 ) & ~127 );

    ଫେରନ୍ତୁ IsMacResource( library, stream, offset, face_index, aface );

  Exit:
    ଫେରନ୍ତୁ error;
  }


  ସ୍ଥିର FT_Error
  load_face_in_embedded_rfork( FT_Library           library,
                               FT_Stream            stream,
                               FT_Long              face_index,
                               FT_Face             *aface,
                               ଅଚଳ FT_Open_Args  *args )
  {

#undef  FT_COMPONENT
#define FT_COMPONENT  raccess

    FT_Memory  memory = library->memory;
    FT_Error   error  = FT_ERR( Unknown_File_Format );
    FT_UInt    i;

    ଅକ୍ଷର*      file_names[FT_RACCESS_N_RULES];
    FT_Long    offsets[FT_RACCESS_N_RULES];
    FT_Error   errors[FT_RACCESS_N_RULES];
    FT_Bool    is_darwin_vfs, vfs_rfork_has_no_font = FALSE; /* not tested */

    FT_Open_Args  args2;
    FT_Stream     stream2 = ଖାଲି;


    FT_Raccess_Guess( library, stream,
                      args->pathname, file_names, offsets, errors );

    ପାଇଁ ( i = 0; i < FT_RACCESS_N_RULES; i++ )
    {
      is_darwin_vfs = ft_raccess_rule_by_darwin_vfs( library, i );
      ଯଦି ( is_darwin_vfs && vfs_rfork_has_no_font )
      {
        FT_TRACE3(( "Skip rule %u: darwin vfs resource fork"
                    " is already checked and"
                    " no font is found\n",
                    i ));
        ଜାରି;
      }

      ଯଦି ( errors[i] )
      {
        FT_TRACE3(( "Error 0x%x has occurred in rule %u\n",
                    errors[i], i ));
        ଜାରି;
      }

      args2.flags    = FT_OPEN_PATHNAME;
      args2.pathname = file_names[i] ? file_names[i] : args->pathname;

      FT_TRACE3(( "Try rule %u: %s (offset=%ld) ...",
                  i, args2.pathname, offsets[i] ));

      error = FT_Stream_New( library, &args2, &stream2 );
      ଯଦି ( is_darwin_vfs && FT_ERR_EQ( error, Cannot_Open_Stream ) )
        vfs_rfork_has_no_font = TRUE;

      ଯଦି ( error )
      {
        FT_TRACE3(( "failed\n" ));
        ଜାରି;
      }

      error = IsMacResource( library, stream2, offsets[i],
                             face_index, aface );
      FT_Stream_Free( stream2, 0 );

      FT_TRACE3(( "%s\n", error ? "failed": "successful" ));

      ଯଦି ( !error )
          ଭାଙ୍ଗ;
      ଅନ୍ୟଥା ଯଦି ( is_darwin_vfs )
          vfs_rfork_has_no_font = TRUE;
    }

    ପାଇଁ (i = 0; i < FT_RACCESS_N_RULES; i++)
    {
      ଯଦି ( file_names[i] )
        FT_FREE( file_names[i] );
    }

    /* Caller (load_mac_face) requires FT_Err_Unknown_File_Format. */
    ଯଦି ( error )
      error = FT_ERR( Unknown_File_Format );

    ଫେରନ୍ତୁ error;

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
  ସ୍ଥିର FT_Error
  load_mac_face( FT_Library           library,
                 FT_Stream            stream,
                 FT_Long              face_index,
                 FT_Face             *aface,
                 ଅଚଳ FT_Open_Args  *args )
  {
    FT_Error error;
    FT_UNUSED( args );


    error = Is_Mac_ଦ୍ୱିମିକ( library, stream, face_index, aface );
    ଯଦି ( FT_ERR_EQ( error, Unknown_File_Format ) )
    {

#undef  FT_COMPONENT
#define FT_COMPONENT  raccess

#ifdef FT_DEBUG_LEVEL_TRACE
      FT_TRACE3(( "Try as dfont: " ));
      ଯଦି ( !( args->flags & FT_OPEN_MEMORY ) )
        FT_TRACE3(( "%s ...", args->pathname ));
#endif

      error = IsMacResource( library, stream, 0, face_index, aface );

      FT_TRACE3(( "%s\n", error ? "failed" : "successful" ));

#undef  FT_COMPONENT
#define FT_COMPONENT  objs

    }

    ଯଦି ( ( FT_ERR_EQ( error, Unknown_File_Format )      ||
           FT_ERR_EQ( error, Invalid_Stream_Operation ) ) &&
         ( args->flags & FT_OPEN_PATHNAME )               )
      error = load_face_in_embedded_rfork( library, stream,
                                           face_index, aface, args );
    ଫେରନ୍ତୁ error;
  }
#endif

#endif  /* !FT_MACINTOSH && FT_CONFIG_OPTION_MAC_FONTS */


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Open_Face( FT_Library           library,
                ଅଚଳ FT_Open_Args*  args,
                FT_Long              face_index,
                FT_Face             *aface )
  {
    ଫେରନ୍ତୁ ft_open_face_internal( library, args, face_index, aface, 1 );
  }


  ସ୍ଥିର FT_Error
  ft_open_face_internal( FT_Library           library,
                         ଅଚଳ FT_Open_Args*  args,
                         FT_Long              face_index,
                         FT_Face             *aface,
                         FT_Bool              test_mac_fonts )
  {
    FT_Error     error;
    FT_Driver    driver = ଖାଲି;
    FT_Memory    memory = ଖାଲି;
    FT_Stream    stream = ଖାଲି;
    FT_Face      face   = ଖାଲି;
    FT_List_ଗଣ୍ଠି  ଗଣ୍ଠି   = ଖାଲି;
    FT_Bool      external_stream;
    FT_Module*   cur;
    FT_Module*   limit;

#ifndef FT_CONFIG_OPTION_MAC_FONTS
    FT_UNUSED( test_mac_fonts );
#endif


    /* only use lower 31 bits together with sign bit */
    ଯଦି ( face_index > 0 )
      face_index &= 0x7FFFFFFFL;
    ଅନ୍ୟଥା
    {
      face_index  = -face_index;
      face_index &= 0x7FFFFFFFL;
      face_index  = -face_index;
    }

#ifdef FT_DEBUG_LEVEL_TRACE
    FT_TRACE3(( "FT_Open_Face: " ));
    ଯଦି ( face_index < 0 )
      FT_TRACE3(( "Requesting number of faces and named instances\n"));
    ଅନ୍ୟଥା
    {
      FT_TRACE3(( "Requesting face %ld", face_index & 0xFFFFL ));
      ଯଦି ( face_index & 0x7FFF0000L )
        FT_TRACE3(( ", named instance %ld", face_index >> 16 ));
      FT_TRACE3(( "\n" ));
    }
#endif

    /* test for valid `library' delayed to `FT_Stream_New' */

    ଯଦି ( !args )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Argument );

    external_stream = FT_BOOL( ( args->flags & FT_OPEN_STREAM ) &&
                               args->stream                     );

    /* create input stream */
    error = FT_Stream_New( library, args, &stream );
    ଯଦି ( error )
      ଯାଅ Fail3;

    /* Do this error check after `FT_Stream_New` to ensure that the */
    /* 'close' callback is called.                                  */
    ଯଦି ( !aface && face_index >= 0 )
    {
      error = FT_THROW( Invalid_Argument );
      ଯାଅ Fail3;
    }

    memory = library->memory;

    /* If the font driver is specified in the `args' structure, use */
    /* it.  Otherwise, we scan the list of registered drivers.      */
    ଯଦି ( ( args->flags & FT_OPEN_DRIVER ) && args->driver )
    {
      driver = FT_DRIVER( args->driver );

      /* not all modules are drivers, so check... */
      ଯଦି ( FT_MODULE_IS_DRIVER( driver ) )
      {
        FT_Int         ଅଙ୍କ_params = 0;
        FT_Parameter*  params     = ଖାଲି;


        ଯଦି ( args->flags & FT_OPEN_PARAMS )
        {
          ଅଙ୍କ_params = args->ଅଙ୍କ_params;
          params     = args->params;
        }

        error = open_face( driver, &stream, &external_stream, face_index,
                           ଅଙ୍କ_params, params, &face );
        ଯଦି ( !error )
          ଯାଅ Success;
      }
      ଅନ୍ୟଥା
        error = FT_THROW( Invalid_Handle );

      FT_Stream_Free( stream, external_stream );
      ଯାଅ Fail;
    }
    ଅନ୍ୟଥା
    {
      error = FT_ERR( Missing_Module );

      /* check each font driver for an appropriate format */
      cur   = library->modules;
      limit = cur + library->ଅଙ୍କ_modules;

      ପାଇଁ ( ; cur < limit; cur++ )
      {
        /* not all modules are font drivers, so check... */
        ଯଦି ( FT_MODULE_IS_DRIVER( cur[0] ) )
        {
          FT_Int         ଅଙ୍କ_params = 0;
          FT_Parameter*  params     = ଖାଲି;


          driver = FT_DRIVER( cur[0] );

          ଯଦି ( args->flags & FT_OPEN_PARAMS )
          {
            ଅଙ୍କ_params = args->ଅଙ୍କ_params;
            params     = args->params;
          }

          error = open_face( driver, &stream, &external_stream, face_index,
                             ଅଙ୍କ_params, params, &face );
          ଯଦି ( !error )
            ଯାଅ Success;

#ifdef FT_CONFIG_OPTION_MAC_FONTS
          ଯଦି ( test_mac_fonts                                           &&
               ft_strcmp( cur[0]->clazz->module_name, "truetype" ) == 0 &&
               FT_ERR_EQ( error, Table_Missing )                        )
          {
            /* TrueType but essential tables are missing */
            error = FT_Stream_Seek( stream, 0 );
            ଯଦି ( error )
              ଭାଙ୍ଗ;

            error = open_face_PS_from_sfnt_stream( library,
                                                   stream,
                                                   face_index,
                                                   ଅଙ୍କ_params,
                                                   params,
                                                   aface );
            ଯଦି ( !error )
            {
              FT_Stream_Free( stream, external_stream );
              ଫେରନ୍ତୁ error;
            }
          }
#endif

          ଯଦି ( FT_ERR_NEQ( error, Unknown_File_Format ) )
            ଯାଅ Fail3;
        }
      }

    Fail3:
      /* If we are on the mac, and we get an                          */
      /* FT_Err_Invalid_Stream_Operation it may be because we have an */
      /* empty data fork, so we need to check the resource fork.      */
      ଯଦି ( FT_ERR_NEQ( error, Cannot_Open_Stream )       &&
           FT_ERR_NEQ( error, Unknown_File_Format )      &&
           FT_ERR_NEQ( error, Invalid_Stream_Operation ) )
        ଯାଅ Fail2;

#ଯଦି !defined( FT_MACINTOSH ) && defined( FT_CONFIG_OPTION_MAC_FONTS )
      ଯଦି ( test_mac_fonts )
      {
        error = load_mac_face( library, stream, face_index, aface, args );
        ଯଦି ( !error )
        {
          /* We don't want to go to Success here.  We've already done   */
          /* that.  On the other hand, if we succeeded we still need to */
          /* close this stream (we opened a different stream which      */
          /* extracted the interesting information out of this stream   */
          /* here.  That stream will still be open and the face will    */
          /* point to it).                                              */
          FT_Stream_Free( stream, external_stream );
          ଫେରନ୍ତୁ error;
        }
      }

      ଯଦି ( FT_ERR_NEQ( error, Unknown_File_Format ) )
        ଯାଅ Fail2;
#endif  /* !FT_MACINTOSH && FT_CONFIG_OPTION_MAC_FONTS */

      /* no driver is able to handle this format */
      error = FT_THROW( Unknown_File_Format );

  Fail2:
      FT_Stream_Free( stream, external_stream );
      ଯାଅ Fail;
    }

  Success:
    FT_TRACE4(( "FT_Open_Face: New face object, adding to list\n" ));

    /* add the face object to its driver's list */
    ଯଦି ( FT_QNEW( ଗଣ୍ଠି ) )
      ଯାଅ Fail;

    ଗଣ୍ଠି->data = face;
    /* don't assume driver is the same as face->driver, so use */
    /* face->driver instead.                                   */
    FT_List_Add( &face->driver->faces_list, ଗଣ୍ଠି );

    /* now allocate a glyph slot object for the face */
    FT_TRACE4(( "FT_Open_Face: Creating glyph slot\n" ));

    ଯଦି ( face_index >= 0 )
    {
      error = FT_New_GlyphSlot( face, ଖାଲି );
      ଯଦି ( error )
        ଯାଅ Fail;

      /* finally, allocate a size object for the face */
      {
        FT_Size  size;


        FT_TRACE4(( "FT_Open_Face: Creating size object\n" ));

        error = FT_New_Size( face, &size );
        ଯଦି ( error )
          ଯାଅ Fail;

        face->size = size;
      }
    }

    /* some checks */

    ଯଦି ( FT_IS_SCALABLE( face ) )
    {
      ଯଦି ( face->height < 0 )
        face->height = (FT_Short)-face->height;

      ଯଦି ( !FT_HAS_VERTICAL( face ) )
        face->max_advance_height = (FT_Short)face->height;
    }

    ଯଦି ( FT_HAS_FIXED_SIZES( face ) )
    {
      FT_Int  i;


      ପାଇଁ ( i = 0; i < face->ଅଙ୍କ_fixed_sizes; i++ )
      {
        FT_Bitmap_Size*  bsize = face->available_sizes + i;


        ଯଦି ( bsize->height < 0 )
          bsize->height = -bsize->height;
        ଯଦି ( bsize->x_ppem < 0 )
          bsize->x_ppem = -bsize->x_ppem;
        ଯଦି ( bsize->y_ppem < 0 )
          bsize->y_ppem = -bsize->y_ppem;

        /* check whether negation actually has worked */
        ଯଦି ( bsize->height < 0 || bsize->x_ppem < 0 || bsize->y_ppem < 0 )
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

    ଯଦି ( aface )
      *aface = face;
    ଅନ୍ୟଥା
      FT_Done_Face( face );

    ଯାଅ Exit;

  Fail:
    ଯଦି ( ଗଣ୍ଠି )
      FT_Done_Face( face );    /* face must be in the driver's list */
    ଅନ୍ୟଥା ଯଦି ( face )
      destroy_face( memory, face, driver );

  Exit:
#ifdef FT_DEBUG_LEVEL_TRACE
    ଯଦି ( !error && face_index < 0 )
    {
      FT_TRACE3(( "FT_Open_Face: The font has %ld face%s\n",
                  face->ଅଙ୍କ_faces,
                  face->ଅଙ୍କ_faces == 1 ? "" : "s" ));
      FT_TRACE3(( "              and %ld named instance%s for face %ld\n",
                  face->style_flags >> 16,
                  ( face->style_flags >> 16 ) == 1 ? "" : "s",
                  -face_index - 1 ));
    }
#endif

    FT_TRACE4(( "FT_Open_Face: Return 0x%x\n", error ));

    ଫେରନ୍ତୁ error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Attach_File( FT_Face      face,
                  ଅଚଳ ଅକ୍ଷର*  filepathname )
  {
    FT_Open_Args  open;


    /* test for valid `face' delayed to `FT_Attach_Stream' */

    ଯଦି ( !filepathname )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Argument );

    open.stream   = ଖାଲି;
    open.flags    = FT_OPEN_PATHNAME;
    open.pathname = (ଅକ୍ଷର*)filepathname;

    ଫେରନ୍ତୁ FT_Attach_Stream( face, &open );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Attach_Stream( FT_Face              face,
                    ଅଚଳ FT_Open_Args*  parameters )
  {
    FT_Stream  stream;
    FT_Error   error;
    FT_Driver  driver;

    FT_Driver_Class  clazz;


    /* test for valid `parameters' delayed to `FT_Stream_New' */

    ଯଦି ( !face )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Face_Handle );

    driver = face->driver;
    ଯଦି ( !driver )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Driver_Handle );

    error = FT_Stream_New( driver->ମୂଳ.library, parameters, &stream );
    ଯଦି ( error )
      ଯାଅ Exit;

    /* we implement FT_Attach_Stream in each driver through the */
    /* `attach_file' interface                                  */

    error = FT_ERR( Unimplemented_Feature );
    clazz = driver->clazz;
    ଯଦି ( clazz->attach_file )
      error = clazz->attach_file( face, stream );

    /* close the attached stream */
    FT_Stream_Free( stream,
                    FT_BOOL( parameters->stream                     &&
                             ( parameters->flags & FT_OPEN_STREAM ) ) );

  Exit:
    ଫେରନ୍ତୁ error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Reference_Face( FT_Face  face )
  {
    ଯଦି ( !face )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Face_Handle );

    face->internal->refcount++;

    ଫେରନ୍ତୁ FT_Err_Ok;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Done_Face( FT_Face  face )
  {
    FT_Error     error;
    FT_Driver    driver;
    FT_Memory    memory;
    FT_List_ଗଣ୍ଠି  ଗଣ୍ଠି;


    error = FT_ERR( Invalid_Face_Handle );
    ଯଦି ( face && face->driver )
    {
      face->internal->refcount--;
      ଯଦି ( face->internal->refcount > 0 )
        error = FT_Err_Ok;
      ଅନ୍ୟଥା
      {
        driver = face->driver;
        memory = driver->ମୂଳ.memory;

        /* find face in driver's list */
        ଗଣ୍ଠି = FT_List_Find( &driver->faces_list, face );
        ଯଦି ( ଗଣ୍ଠି )
        {
          /* remove face object from the driver's list */
          FT_List_Remove( &driver->faces_list, ଗଣ୍ଠି );
          FT_FREE( ଗଣ୍ଠି );

          /* now destroy the object proper */
          destroy_face( memory, face, driver );
          error = FT_Err_Ok;
        }
      }
    }

    ଫେରନ୍ତୁ error;
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

    FT_Size          size = ଖାଲି;
    FT_List_ଗଣ୍ଠି      ଗଣ୍ଠି = ଖାଲି;

    FT_Size_Internal  internal = ଖାଲି;


    ଯଦି ( !face )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Face_Handle );

    ଯଦି ( !asize )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Argument );

    ଯଦି ( !face->driver )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Driver_Handle );

    *asize = ଖାଲି;

    driver = face->driver;
    clazz  = driver->clazz;
    memory = face->memory;

    /* Allocate new size object and perform basic initialisation */
    ଯଦି ( FT_ALLOC( size, clazz->size_object_size ) || FT_QNEW( ଗଣ୍ଠି ) )
      ଯାଅ Exit;

    size->face = face;

    ଯଦି ( FT_NEW( internal ) )
      ଯାଅ Exit;

    size->internal = internal;

    ଯଦି ( clazz->init_size )
      error = clazz->init_size( size );

    /* in case of success, add to the face's list */
    ଯଦି ( !error )
    {
      *asize     = size;
      ଗଣ୍ଠି->data = size;
      FT_List_Add( &face->sizes_list, ଗଣ୍ଠି );
    }

  Exit:
    ଯଦି ( error )
    {
      FT_FREE( ଗଣ୍ଠି );
      ଯଦି ( size )
        FT_FREE( size->internal );
      FT_FREE( size );
    }

    ଫେରନ୍ତୁ error;
  }


  /* documentation is in ftobjs.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Done_Size( FT_Size  size )
  {
    FT_Error     error;
    FT_Driver    driver;
    FT_Memory    memory;
    FT_Face      face;
    FT_List_ଗଣ୍ଠି  ଗଣ୍ଠି;


    ଯଦି ( !size )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Size_Handle );

    face = size->face;
    ଯଦି ( !face )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Face_Handle );

    driver = face->driver;
    ଯଦି ( !driver )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Driver_Handle );

    memory = driver->ମୂଳ.memory;

    error = FT_Err_Ok;
    ଗଣ୍ଠି  = FT_List_Find( &face->sizes_list, size );
    ଯଦି ( ଗଣ୍ଠି )
    {
      FT_List_Remove( &face->sizes_list, ଗଣ୍ଠି );
      FT_FREE( ଗଣ୍ଠି );

      ଯଦି ( face->size == size )
      {
        face->size = ଖାଲି;
        ଯଦି ( face->sizes_list.head )
          face->size = (FT_Size)(face->sizes_list.head->data);
      }

      destroy_size( memory, size, driver );
    }
    ଅନ୍ୟଥା
      error = FT_THROW( Invalid_Size_Handle );

    ଫେରନ୍ତୁ error;
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


    ଯଦି ( !FT_HAS_FIXED_SIZES( face ) )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Face_Handle );

    /* FT_Bitmap_Size doesn't provide enough info... */
    ଯଦି ( req->type != FT_SIZE_REQUEST_TYPE_NOMINAL )
      ଫେରନ୍ତୁ FT_THROW( Unimplemented_Feature );

    w = FT_REQUEST_WIDTH ( req );
    h = FT_REQUEST_HEIGHT( req );

    ଯଦି ( req->width && !req->height )
      h = w;
    ଅନ୍ୟଥା ଯଦି ( !req->width && req->height )
      w = h;

    w = FT_PIX_ROUND( w );
    h = FT_PIX_ROUND( h );

    ଯଦି ( !w || !h )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Pixel_Size );

    ପାଇଁ ( i = 0; i < face->ଅଙ୍କ_fixed_sizes; i++ )
    {
      FT_Bitmap_Size*  bsize = face->available_sizes + i;


      ଯଦି ( h != FT_PIX_ROUND( bsize->y_ppem ) )
        ଜାରି;

      ଯଦି ( w == FT_PIX_ROUND( bsize->x_ppem ) || ignore_width )
      {
        FT_TRACE3(( "FT_Match_Size: bitmap strike %d matches\n", i ));

        ଯଦି ( size_index )
          *size_index = (FT_ULong)i;

        ଫେରନ୍ତୁ FT_Err_Ok;
      }
    }

    FT_TRACE3(( "FT_Match_Size: no matching bitmap strike\n" ));

    ଫେରନ୍ତୁ FT_THROW( Invalid_Pixel_Size );
  }


  /* documentation is in ftobjs.h */

  FT_BASE_DEF( ଶୂନ୍ୟ )
  ft_synthesize_vertical_metrics( FT_Glyph_Metrics*  metrics,
                                  FT_Pos             advance )
  {
    FT_Pos  height = metrics->height;


    /* compensate for glyph with bbox above/below the baseline */
    ଯଦି ( metrics->horiBearingY < 0 )
    {
      ଯଦି ( height < metrics->horiBearingY )
        height = metrics->horiBearingY;
    }
    ଅନ୍ୟଥା ଯଦି ( metrics->horiBearingY > 0 )
      height -= metrics->horiBearingY;

    /* the factor 1.2 is a heuristical value */
    ଯଦି ( !advance )
      advance = height * 12 / 10;

    metrics->vertBearingX = metrics->horiBearingX - metrics->horiAdvance / 2;
    metrics->vertBearingY = ( advance - height ) / 2;
    metrics->vertAdvance  = advance;
  }


  ସ୍ଥିର ଶୂନ୍ୟ
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
#ଅନ୍ୟଥା /* !GRID_FIT_METRICS */
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


  FT_BASE_DEF( ଶୂନ୍ୟ )
  FT_Select_Metrics( FT_Face   face,
                     FT_ULong  strike_index )
  {
    FT_Size_Metrics*  metrics;
    FT_Bitmap_Size*   bsize;


    metrics = &face->size->metrics;
    bsize   = face->available_sizes + strike_index;

    metrics->x_ppem = (FT_UShort)( ( bsize->x_ppem + 32 ) >> 6 );
    metrics->y_ppem = (FT_UShort)( ( bsize->y_ppem + 32 ) >> 6 );

    ଯଦି ( FT_IS_SCALABLE( face ) )
    {
      metrics->x_scale = FT_DivFix( bsize->x_ppem,
                                    face->units_per_EM );
      metrics->y_scale = FT_DivFix( bsize->y_ppem,
                                    face->units_per_EM );

      ft_recompute_scaled_metrics( face, metrics );
    }
    ଅନ୍ୟଥା
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

    ଯଦି ( FT_IS_SCALABLE( face ) )
    {
      FT_Long  w = 0, h = 0, scaled_w = 0, scaled_h = 0;


      ବାଛ ( req->type )
      {
      ସ୍ଥିତି FT_SIZE_REQUEST_TYPE_NOMINAL:
        w = h = face->units_per_EM;
        ଭାଙ୍ଗ;

      ସ୍ଥିତି FT_SIZE_REQUEST_TYPE_REAL_DIM:
        w = h = face->ascender - face->descender;
        ଭାଙ୍ଗ;

      ସ୍ଥିତି FT_SIZE_REQUEST_TYPE_BBOX:
        w = face->bbox.xMax - face->bbox.xMin;
        h = face->bbox.yMax - face->bbox.yMin;
        ଭାଙ୍ଗ;

      ସ୍ଥିତି FT_SIZE_REQUEST_TYPE_CELL:
        w = face->max_advance_width;
        h = face->ascender - face->descender;
        ଭାଙ୍ଗ;

      ସ୍ଥିତି FT_SIZE_REQUEST_TYPE_SCALES:
        metrics->x_scale = (FT_Fixed)req->width;
        metrics->y_scale = (FT_Fixed)req->height;
        ଯଦି ( !metrics->x_scale )
          metrics->x_scale = metrics->y_scale;
        ଅନ୍ୟଥା ଯଦି ( !metrics->y_scale )
          metrics->y_scale = metrics->x_scale;
        ଯାଅ Calculate_Ppem;

      ସ୍ଥିତି FT_SIZE_REQUEST_TYPE_MAX:
        ଭାଙ୍ଗ;
      }

      /* to be on the safe side */
      ଯଦି ( w < 0 )
        w = -w;

      ଯଦି ( h < 0 )
        h = -h;

      scaled_w = FT_REQUEST_WIDTH ( req );
      scaled_h = FT_REQUEST_HEIGHT( req );

      /* determine scales */
      ଯଦି ( req->height || !req->width )
      {
        ଯଦି ( h == 0 )
        {
          FT_ERROR(( "FT_Request_Metrics: Divide by zero\n" ));
          error = FT_ERR( Divide_By_Zero );
          ଯାଅ Exit;
        }

        metrics->y_scale = FT_DivFix( scaled_h, h );
      }

      ଯଦି ( req->width )
      {
        ଯଦି ( w == 0 )
        {
          FT_ERROR(( "FT_Request_Metrics: Divide by zero\n" ));
          error = FT_ERR( Divide_By_Zero );
          ଯାଅ Exit;
        }

        metrics->x_scale = FT_DivFix( scaled_w, w );
      }
      ଅନ୍ୟଥା
      {
        metrics->x_scale = metrics->y_scale;
        scaled_w = FT_MulDiv( scaled_h, w, h );
      }

      ଯଦି ( !req->height )
      {
        metrics->y_scale = metrics->x_scale;
        scaled_h = FT_MulDiv( scaled_w, h, w );
      }

      ଯଦି ( req->type == FT_SIZE_REQUEST_TYPE_CELL )
      {
        ଯଦି ( metrics->y_scale > metrics->x_scale )
          metrics->y_scale = metrics->x_scale;
        ଅନ୍ୟଥା
          metrics->x_scale = metrics->y_scale;
      }

  Calculate_Ppem:
      /* calculate the ppems */
      ଯଦି ( req->type != FT_SIZE_REQUEST_TYPE_NOMINAL )
      {
        scaled_w = FT_MulFix( face->units_per_EM, metrics->x_scale );
        scaled_h = FT_MulFix( face->units_per_EM, metrics->y_scale );
      }

      scaled_w = ( scaled_w + 32 ) >> 6;
      scaled_h = ( scaled_h + 32 ) >> 6;
      ଯଦି ( scaled_w > (FT_Long)FT_USHORT_MAX ||
           scaled_h > (FT_Long)FT_USHORT_MAX )
      {
        FT_ERROR(( "FT_Request_Metrics: Resulting ppem size too large\n" ));
        error = FT_ERR( Invalid_Pixel_Size );
        ଯାଅ Exit;
      }

      metrics->x_ppem = (FT_UShort)scaled_w;
      metrics->y_ppem = (FT_UShort)scaled_h;

      ft_recompute_scaled_metrics( face, metrics );
    }
    ଅନ୍ୟଥା
    {
      FT_ZERO( metrics );
      metrics->x_scale = 1L << 16;
      metrics->y_scale = 1L << 16;
    }

  Exit:
    ଫେରନ୍ତୁ error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Select_Size( FT_Face  face,
                  FT_Int   strike_index )
  {
    FT_Error         error = FT_Err_Ok;
    FT_Driver_Class  clazz;


    ଯଦି ( !face || !FT_HAS_FIXED_SIZES( face ) )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Face_Handle );

    ଯଦି ( strike_index < 0 || strike_index >= face->ଅଙ୍କ_fixed_sizes )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Argument );

    clazz = face->driver->clazz;

    ଯଦି ( clazz->select_size )
    {
      error = clazz->select_size( face->size, (FT_ULong)strike_index );

      FT_TRACE5(( "FT_Select_Size (%s driver):\n",
                  face->driver->ମୂଳ.clazz->module_name ));
    }
    ଅନ୍ୟଥା
    {
      FT_Select_Metrics( face, (FT_ULong)strike_index );

      FT_TRACE5(( "FT_Select_Size:\n" ));
    }

#ifdef FT_DEBUG_LEVEL_TRACE
    {
      FT_Size_Metrics*  metrics = &face->size->metrics;


      FT_TRACE5(( "  x scale: %ld (%f)\n",
                  metrics->x_scale, (ଦ୍ୱିଗୁଣ)metrics->x_scale / 65536 ));
      FT_TRACE5(( "  y scale: %ld (%f)\n",
                  metrics->y_scale, (ଦ୍ୱିଗୁଣ)metrics->y_scale / 65536 ));
      FT_TRACE5(( "  ascender: %f\n",
                  (ଦ୍ୱିଗୁଣ)metrics->ascender / 64 ));
      FT_TRACE5(( "  descender: %f\n",
                  (ଦ୍ୱିଗୁଣ)metrics->descender / 64 ));
      FT_TRACE5(( "  height: %f\n",
                  (ଦ୍ୱିଗୁଣ)metrics->height / 64 ));
      FT_TRACE5(( "  max advance: %f\n",
                  (ଦ୍ୱିଗୁଣ)metrics->max_advance / 64 ));
      FT_TRACE5(( "  x ppem: %d\n", metrics->x_ppem ));
      FT_TRACE5(( "  y ppem: %d\n", metrics->y_ppem ));
    }
#endif

    ଫେରନ୍ତୁ error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Request_Size( FT_Face          face,
                   FT_Size_Request  req )
  {
    FT_Error         error;
    FT_Driver_Class  clazz;
    FT_ULong         strike_index;


    ଯଦି ( !face )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Face_Handle );

    ଯଦି ( !face->size )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Size_Handle );

    ଯଦି ( !req || req->width < 0 || req->height < 0 ||
         req->type >= FT_SIZE_REQUEST_TYPE_MAX )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Argument );

    /* signal the auto-hinter to recompute its size metrics */
    /* (if requested)                                       */
    face->size->internal->autohint_metrics.x_scale = 0;

    clazz = face->driver->clazz;

    ଯଦି ( clazz->request_size )
    {
      error = clazz->request_size( face->size, req );

      FT_TRACE5(( "FT_Request_Size (%s driver):\n",
                  face->driver->ମୂଳ.clazz->module_name ));
    }
    ଅନ୍ୟଥା ଯଦି ( !FT_IS_SCALABLE( face ) && FT_HAS_FIXED_SIZES( face ) )
    {
      /*
       * The reason that a driver doesn't have `request_size' defined is
       * either that the scaling here suffices or that the supported formats
       * are bitmap-only and size matching is not implemented.
       *
       * In the latter case, a simple size matching is done.
       */
      error = FT_Match_Size( face, req, 0, &strike_index );
      ଯଦି ( error )
        ଯାଅ Exit;

      ଫେରନ୍ତୁ FT_Select_Size( face, (FT_Int)strike_index );
    }
    ଅନ୍ୟଥା
    {
      error = FT_Request_Metrics( face, req );
      ଯଦି ( error )
        ଯାଅ Exit;

      FT_TRACE5(( "FT_Request_Size:\n" ));
    }

#ifdef FT_DEBUG_LEVEL_TRACE
    {
      FT_Size_Metrics*  metrics = &face->size->metrics;


      FT_TRACE5(( "  x scale: %ld (%f)\n",
                  metrics->x_scale, (ଦ୍ୱିଗୁଣ)metrics->x_scale / 65536 ));
      FT_TRACE5(( "  y scale: %ld (%f)\n",
                  metrics->y_scale, (ଦ୍ୱିଗୁଣ)metrics->y_scale / 65536 ));
      FT_TRACE5(( "  ascender: %f\n",
                  (ଦ୍ୱିଗୁଣ)metrics->ascender / 64 ));
      FT_TRACE5(( "  descender: %f\n",
                  (ଦ୍ୱିଗୁଣ)metrics->descender / 64 ));
      FT_TRACE5(( "  height: %f\n",
                  (ଦ୍ୱିଗୁଣ)metrics->height / 64 ));
      FT_TRACE5(( "  max advance: %f\n",
                  (ଦ୍ୱିଗୁଣ)metrics->max_advance / 64 ));
      FT_TRACE5(( "  x ppem: %d\n", metrics->x_ppem ));
      FT_TRACE5(( "  y ppem: %d\n", metrics->y_ppem ));
    }
#endif

  Exit:
    ଫେରନ୍ତୁ error;
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

    ଯଦି ( !char_width )
      char_width = char_height;
    ଅନ୍ୟଥା ଯଦି ( !char_height )
      char_height = char_width;

    ଯଦି ( !horz_resolution )
      horz_resolution = vert_resolution;
    ଅନ୍ୟଥା ଯଦି ( !vert_resolution )
      vert_resolution = horz_resolution;

    ଯଦି ( char_width  < 1 * 64 )
      char_width  = 1 * 64;
    ଯଦି ( char_height < 1 * 64 )
      char_height = 1 * 64;

    ଯଦି ( !horz_resolution )
      horz_resolution = vert_resolution = 72;

    req.type           = FT_SIZE_REQUEST_TYPE_NOMINAL;
    req.width          = char_width;
    req.height         = char_height;
    req.horiResolution = horz_resolution;
    req.vertResolution = vert_resolution;

    ଫେରନ୍ତୁ FT_Request_Size( face, &req );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Set_Pixel_Sizes( FT_Face  face,
                      FT_UInt  pixel_width,
                      FT_UInt  pixel_height )
  {
    FT_Size_RequestRec  req;


    /* check of `face' delayed to `FT_Request_Size' */

    ଯଦି ( pixel_width == 0 )
      pixel_width = pixel_height;
    ଅନ୍ୟଥା ଯଦି ( pixel_height == 0 )
      pixel_height = pixel_width;

    ଯଦି ( pixel_width  < 1 )
      pixel_width  = 1;
    ଯଦି ( pixel_height < 1 )
      pixel_height = 1;

    /* use `>=' to avoid potential compiler warning on 16bit platforms */
    ଯଦି ( pixel_width >= 0xFFFFU )
      pixel_width = 0xFFFFU;
    ଯଦି ( pixel_height >= 0xFFFFU )
      pixel_height = 0xFFFFU;

    req.type           = FT_SIZE_REQUEST_TYPE_NOMINAL;
    req.width          = (FT_Long)( pixel_width << 6 );
    req.height         = (FT_Long)( pixel_height << 6 );
    req.horiResolution = 0;
    req.vertResolution = 0;

    ଫେରନ୍ତୁ FT_Request_Size( face, &req );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Kerning( FT_Face     face,
                  FT_UInt     ବାମ_glyph,
                  FT_UInt     ଡାହାଣ_glyph,
                  FT_UInt     kern_mode,
                  FT_Vector  *akerning )
  {
    FT_Error   error = FT_Err_Ok;
    FT_Driver  driver;


    ଯଦି ( !face )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Face_Handle );

    ଯଦି ( !akerning )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Argument );

    driver = face->driver;

    akerning->x = 0;
    akerning->y = 0;

    ଯଦି ( driver->clazz->get_kerning )
    {
      error = driver->clazz->get_kerning( face,
                                          ବାମ_glyph,
                                          ଡାହାଣ_glyph,
                                          akerning );
      ଯଦି ( !error )
      {
        ଯଦି ( kern_mode != FT_KERNING_UNSCALED )
        {
          akerning->x = FT_MulFix( akerning->x, face->size->metrics.x_scale );
          akerning->y = FT_MulFix( akerning->y, face->size->metrics.y_scale );

          ଯଦି ( kern_mode != FT_KERNING_UNFITTED )
          {
            FT_Pos  orig_x = akerning->x;
            FT_Pos  orig_y = akerning->y;


            /* we scale down kerning values for small ppem values */
            /* to avoid that rounding makes them too big.         */
            /* `25' has been determined heuristically.            */
            ଯଦି ( face->size->metrics.x_ppem < 25 )
              akerning->x = FT_MulDiv( orig_x,
                                       face->size->metrics.x_ppem, 25 );
            ଯଦି ( face->size->metrics.y_ppem < 25 )
              akerning->y = FT_MulDiv( orig_y,
                                       face->size->metrics.y_ppem, 25 );

            akerning->x = FT_PIX_ROUND( akerning->x );
            akerning->y = FT_PIX_ROUND( akerning->y );

#ifdef FT_DEBUG_LEVEL_TRACE
            {
              FT_Pos  orig_x_rounded = FT_PIX_ROUND( orig_x );
              FT_Pos  orig_y_rounded = FT_PIX_ROUND( orig_y );


              ଯଦି ( akerning->x != orig_x_rounded ||
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

    ଫେରନ୍ତୁ error;
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


    ଯଦି ( !face )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Face_Handle );

    ଯଦି ( !akerning )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Argument );

    FT_FACE_FIND_SERVICE( face, service, KERNING );
    ଯଦି ( !service )
      ଫେରନ୍ତୁ FT_THROW( Unimplemented_Feature );

    error = service->get_track( face,
                                point_size,
                                degree,
                                akerning );

    ଫେରନ୍ତୁ error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Select_Charmap( FT_Face      face,
                     FT_Encoding  encoding )
  {
    FT_CharMap*  cur;
    FT_CharMap*  limit;


    ଯଦି ( !face )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Face_Handle );

    /* FT_ENCODING_NONE is a valid encoding for BDF, PCF, and Windows FNT */
    ଯଦି ( encoding == FT_ENCODING_NONE && !face->ଅଙ୍କ_charmaps )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Argument );

    /* FT_ENCODING_UNICODE is special.  We try to find the `best' Unicode */
    /* charmap available, i.e., one with UCS-4 characters, if possible.   */
    /*                                                                    */
    /* This is done by find_unicode_charmap() above, to share code.       */
    ଯଦି ( encoding == FT_ENCODING_UNICODE )
      ଫେରନ୍ତୁ find_unicode_charmap( face );

    cur = face->charmaps;
    ଯଦି ( !cur )
      ଫେରନ୍ତୁ FT_THROW( Invalid_CharMap_Handle );

    limit = cur + face->ଅଙ୍କ_charmaps;

    ପାଇଁ ( ; cur < limit; cur++ )
    {
      ଯଦି ( cur[0]->encoding == encoding )
      {
        face->charmap = cur[0];
        ଫେରନ୍ତୁ FT_Err_Ok;
      }
    }

    ଫେରନ୍ତୁ FT_THROW( Invalid_Argument );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Set_Charmap( FT_Face     face,
                  FT_CharMap  charmap )
  {
    FT_CharMap*  cur;
    FT_CharMap*  limit;


    ଯଦି ( !face )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Face_Handle );

    cur = face->charmaps;
    ଯଦି ( !cur || !charmap )
      ଫେରନ୍ତୁ FT_THROW( Invalid_CharMap_Handle );

    limit = cur + face->ଅଙ୍କ_charmaps;

    ପାଇଁ ( ; cur < limit; cur++ )
    {
      ଯଦି ( cur[0] == charmap )
      {
        FT_Long  format = FT_Get_CMap_Format( charmap );


        ଯଦି ( format != 14 && format != 15 )
        {
          face->charmap = cur[0];
          ଫେରନ୍ତୁ FT_Err_Ok;
        }
      }
    }

    ଫେରନ୍ତୁ FT_THROW( Invalid_Argument );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Int )
  FT_Get_Charmap_Index( FT_CharMap  charmap )
  {
    FT_Int  i;


    ଯଦି ( !charmap || !charmap->face )
      ଫେରନ୍ତୁ -1;

    ପାଇଁ ( i = 0; i < charmap->face->ଅଙ୍କ_charmaps; i++ )
      ଯଦି ( charmap->face->charmaps[i] == charmap )
        ଭାଙ୍ଗ;

    FT_ASSERT( i < charmap->face->ଅଙ୍କ_charmaps );

    ଫେରନ୍ତୁ i;
  }


  ସ୍ଥିର ଶୂନ୍ୟ
  ft_cmap_done_internal( FT_CMap  cmap )
  {
    FT_CMap_Class  clazz  = cmap->clazz;
    FT_Face        face   = cmap->charmap.face;
    FT_Memory      memory = FT_FACE_MEMORY( face );


    ଯଦି ( clazz->done )
      clazz->done( cmap );

    FT_FREE( cmap );
  }


  FT_BASE_DEF( ଶୂନ୍ୟ )
  FT_CMap_Done( FT_CMap  cmap )
  {
    ଯଦି ( cmap )
    {
      FT_Face    face   = cmap->charmap.face;
      FT_Memory  memory = FT_FACE_MEMORY( face );
      FT_Error   error;
      FT_Int     i, j;


      ପାଇଁ ( i = 0; i < face->ଅଙ୍କ_charmaps; i++ )
      {
        ଯଦି ( (FT_CMap)face->charmaps[i] == cmap )
        {
          FT_CharMap  last_charmap = face->charmaps[face->ଅଙ୍କ_charmaps - 1];


          ଯଦି ( FT_QRENEW_ARRAY( face->charmaps,
                                face->ଅଙ୍କ_charmaps,
                                face->ଅଙ୍କ_charmaps - 1 ) )
            ଫେରନ୍ତୁ;

          /* remove it from our list of charmaps */
          ପାଇଁ ( j = i + 1; j < face->ଅଙ୍କ_charmaps; j++ )
          {
            ଯଦି ( j == face->ଅଙ୍କ_charmaps - 1 )
              face->charmaps[j - 1] = last_charmap;
            ଅନ୍ୟଥା
              face->charmaps[j - 1] = face->charmaps[j];
          }

          face->ଅଙ୍କ_charmaps--;

          ଯଦି ( (FT_CMap)face->charmap == cmap )
            face->charmap = ଖାଲି;

          ft_cmap_done_internal( cmap );

          ଭାଙ୍ଗ;
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
    FT_CMap    cmap = ଖାଲି;


    ଯଦି ( !clazz || !charmap || !charmap->face )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Argument );

    face   = charmap->face;
    memory = FT_FACE_MEMORY( face );

    ଯଦି ( !FT_ALLOC( cmap, clazz->size ) )
    {
      cmap->charmap = *charmap;
      cmap->clazz   = clazz;

      ଯଦି ( clazz->init )
      {
        error = clazz->init( cmap, init_data );
        ଯଦି ( error )
          ଯାଅ Fail;
      }

      /* add it to our list of charmaps */
      ଯଦି ( FT_QRENEW_ARRAY( face->charmaps,
                            face->ଅଙ୍କ_charmaps,
                            face->ଅଙ୍କ_charmaps + 1 ) )
        ଯାଅ Fail;

      face->charmaps[face->ଅଙ୍କ_charmaps++] = (FT_CharMap)cmap;
    }

  Exit:
    ଯଦି ( acmap )
      *acmap = cmap;

    ଫେରନ୍ତୁ error;

  Fail:
    ft_cmap_done_internal( cmap );
    cmap = ଖାଲି;
    ଯାଅ Exit;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt )
  FT_Get_Char_Index( FT_Face   face,
                     FT_ULong  charcode )
  {
    FT_UInt  result = 0;


    ଯଦି ( face && face->charmap )
    {
      FT_CMap  cmap = FT_CMAP( face->charmap );


      ଯଦି ( charcode > 0xFFFFFFFFUL )
      {
        FT_TRACE1(( "FT_Get_Char_Index: too large charcode" ));
        FT_TRACE1(( " 0x%lx is truncated\n", charcode ));
      }

      result = cmap->clazz->char_index( cmap, (FT_UInt32)charcode );
      ଯଦି ( result >= (FT_UInt)face->ଅଙ୍କ_glyphs )
        result = 0;
    }

    ଫେରନ୍ତୁ result;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_ULong )
  FT_Get_First_Char( FT_Face   face,
                     FT_UInt  *agindex )
  {
    FT_ULong  result = 0;
    FT_UInt   gindex = 0;


    /* only do something if we have a charmap, and we have glyphs at all */
    ଯଦି ( face && face->charmap && face->ଅଙ୍କ_glyphs )
    {
      gindex = FT_Get_Char_Index( face, 0 );
      ଯଦି ( gindex == 0 )
        result = FT_Get_Next_Char( face, 0, &gindex );
    }

    ଯଦି ( agindex )
      *agindex = gindex;

    ଫେରନ୍ତୁ result;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_ULong )
  FT_Get_Next_Char( FT_Face   face,
                    FT_ULong  charcode,
                    FT_UInt  *agindex )
  {
    FT_ULong  result = 0;
    FT_UInt   gindex = 0;


    ଯଦି ( face && face->charmap && face->ଅଙ୍କ_glyphs )
    {
      FT_UInt32  code = (FT_UInt32)charcode;
      FT_CMap    cmap = FT_CMAP( face->charmap );


      କର
      {
        gindex = cmap->clazz->char_next( cmap, &code );

      } ଯେତେବେଳେ ( gindex >= (FT_UInt)face->ଅଙ୍କ_glyphs );

      result = ( gindex == 0 ) ? 0 : code;
    }

    ଯଦି ( agindex )
      *agindex = gindex;

    ଫେରନ୍ତୁ result;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Face_Properties( FT_Face        face,
                      FT_UInt        ଅଙ୍କ_properties,
                      FT_Parameter*  properties )
  {
    FT_Error  error = FT_Err_Ok;


    ଯଦି ( ଅଙ୍କ_properties > 0 && !properties )
    {
      error = FT_THROW( Invalid_Argument );
      ଯାଅ Exit;
    }

    ପାଇଁ ( ; ଅଙ୍କ_properties > 0; ଅଙ୍କ_properties-- )
    {
      ଯଦି ( properties->tag == FT_PARAM_TAG_STEM_DARKENING )
      {
        ଯଦି ( properties->data )
        {
          ଯଦି ( *( (FT_Bool*)properties->data ) == TRUE )
            face->internal->no_stem_darkening = FALSE;
          ଅନ୍ୟଥା
            face->internal->no_stem_darkening = TRUE;
        }
        ଅନ୍ୟଥା
        {
          /* use module default */
          face->internal->no_stem_darkening = -1;
        }
      }
      ଅନ୍ୟଥା ଯଦି ( properties->tag == FT_PARAM_TAG_LCD_FILTER_WEIGHTS )
      {
        error = FT_THROW( Unimplemented_Feature );
        ଯାଅ Exit;
      }
      ଅନ୍ୟଥା ଯଦି ( properties->tag == FT_PARAM_TAG_RANDOM_SEED )
      {
        ଯଦି ( properties->data )
        {
          face->internal->random_seed = *( (FT_Int32*)properties->data );
          ଯଦି ( face->internal->random_seed < 0 )
            face->internal->random_seed = 0;
        }
        ଅନ୍ୟଥା
        {
          /* use module default */
          face->internal->random_seed = -1;
        }
      }
      ଅନ୍ୟଥା
      {
        error = FT_THROW( Invalid_Argument );
        ଯାଅ Exit;
      }

      ଯଦି ( error )
        ଭାଙ୍ଗ;

      properties++;
    }

  Exit:
    ଫେରନ୍ତୁ error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt )
  FT_Face_GetCharVariantIndex( FT_Face   face,
                               FT_ULong  charcode,
                               FT_ULong  variantSelector )
  {
    FT_UInt  result = 0;


    ଯଦି ( face                                           &&
         face->charmap                                  &&
         face->charmap->encoding == FT_ENCODING_UNICODE )
    {
      FT_CharMap  charmap = find_variant_selector_charmap( face );
      FT_CMap     ucmap = FT_CMAP( face->charmap );


      ଯଦି ( charmap )
      {
        FT_CMap  vcmap = FT_CMAP( charmap );


        ଯଦି ( charcode > 0xFFFFFFFFUL )
        {
          FT_TRACE1(( "FT_Face_GetCharVariantIndex:"
                      " too large charcode" ));
          FT_TRACE1(( " 0x%lx is truncated\n", charcode ));
        }
        ଯଦି ( variantSelector > 0xFFFFFFFFUL )
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

    ଫେରନ୍ତୁ result;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Int )
  FT_Face_GetCharVariantIsDefault( FT_Face   face,
                                   FT_ULong  charcode,
                                   FT_ULong  variantSelector )
  {
    FT_Int  result = -1;


    ଯଦି ( face )
    {
      FT_CharMap  charmap = find_variant_selector_charmap( face );


      ଯଦି ( charmap )
      {
        FT_CMap  vcmap = FT_CMAP( charmap );


        ଯଦି ( charcode > 0xFFFFFFFFUL )
        {
          FT_TRACE1(( "FT_Face_GetCharVariantIsDefault:"
                      " too large charcode" ));
          FT_TRACE1(( " 0x%lx is truncated\n", charcode ));
        }
        ଯଦି ( variantSelector > 0xFFFFFFFFUL )
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

    ଫେରନ୍ତୁ result;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt32* )
  FT_Face_GetVariantSelectors( FT_Face  face )
  {
    FT_UInt32  *result = ଖାଲି;


    ଯଦି ( face )
    {
      FT_CharMap  charmap = find_variant_selector_charmap( face );


      ଯଦି ( charmap )
      {
        FT_CMap    vcmap  = FT_CMAP( charmap );
        FT_Memory  memory = FT_FACE_MEMORY( face );


        result = vcmap->clazz->variant_list( vcmap, memory );
      }
    }

    ଫେରନ୍ତୁ result;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt32* )
  FT_Face_GetVariantsOfChar( FT_Face   face,
                             FT_ULong  charcode )
  {
    FT_UInt32  *result = ଖାଲି;


    ଯଦି ( face )
    {
      FT_CharMap  charmap = find_variant_selector_charmap( face );


      ଯଦି ( charmap )
      {
        FT_CMap    vcmap  = FT_CMAP( charmap );
        FT_Memory  memory = FT_FACE_MEMORY( face );


        ଯଦି ( charcode > 0xFFFFFFFFUL )
        {
          FT_TRACE1(( "FT_Face_GetVariantsOfChar: too large charcode" ));
          FT_TRACE1(( " 0x%lx is truncated\n", charcode ));
        }

        result = vcmap->clazz->charvariant_list( vcmap, memory,
                                                 (FT_UInt32)charcode );
      }
    }
    ଫେରନ୍ତୁ result;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt32* )
  FT_Face_GetCharsOfVariant( FT_Face   face,
                             FT_ULong  variantSelector )
  {
    FT_UInt32  *result = ଖାଲି;


    ଯଦି ( face )
    {
      FT_CharMap  charmap = find_variant_selector_charmap( face );


      ଯଦି ( charmap )
      {
        FT_CMap    vcmap  = FT_CMAP( charmap );
        FT_Memory  memory = FT_FACE_MEMORY( face );


        ଯଦି ( variantSelector > 0xFFFFFFFFUL )
        {
          FT_TRACE1(( "FT_Get_Char_Index: too large variantSelector" ));
          FT_TRACE1(( " 0x%lx is truncated\n", variantSelector ));
        }

        result = vcmap->clazz->variantchar_list( vcmap, memory,
                                                 (FT_UInt32)variantSelector );
      }
    }

    ଫେରନ୍ତୁ result;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt )
  FT_Get_Name_Index( FT_Face           face,
                     ଅଚଳ FT_String*  glyph_name )
  {
    FT_UInt  result = 0;


    ଯଦି ( face                       &&
         FT_HAS_GLYPH_NAMES( face ) &&
         glyph_name                 )
    {
      FT_Service_GlyphDict  service;


      FT_FACE_LOOKUP_SERVICE( face,
                              service,
                              GLYPH_DICT );

      ଯଦି ( service && service->name_index )
        result = service->name_index( face, glyph_name );
    }

    ଫେରନ୍ତୁ result;
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


    ଯଦି ( !face )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Face_Handle );

    ଯଦି ( !buffer || buffer_max == 0 )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Argument );

    /* clean up buffer */
    ((FT_Byte*)buffer)[0] = '\0';

    ଯଦି ( (FT_Long)glyph_index >= face->ଅଙ୍କ_glyphs )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Glyph_Index );

    ଯଦି ( !FT_HAS_GLYPH_NAMES( face ) )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Argument );

    FT_FACE_LOOKUP_SERVICE( face, service, GLYPH_DICT );
    ଯଦି ( service && service->get_name )
      error = service->get_name( face, glyph_index, buffer, buffer_max );
    ଅନ୍ୟଥା
      error = FT_THROW( Invalid_Argument );

    ଫେରନ୍ତୁ error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( ଅଚଳ ଅକ୍ଷର* )
  FT_Get_Postscript_Name( FT_Face  face )
  {
    ଅଚଳ ଅକ୍ଷର*  result = ଖାଲି;


    ଯଦି ( !face )
      ଯାଅ Exit;

    ଯଦି ( !result )
    {
      FT_Service_PsFontName  service;


      FT_FACE_LOOKUP_SERVICE( face,
                              service,
                              POSTSCRIPT_FONT_NAME );

      ଯଦି ( service && service->get_ps_font_name )
        result = service->get_ps_font_name( face );
    }

  Exit:
    ଫେରନ୍ତୁ result;
  }


  /* documentation is in tttables.h */

  FT_EXPORT_DEF( ଶୂନ୍ୟ* )
  FT_Get_Sfnt_Table( FT_Face      face,
                     FT_Sfnt_Tag  tag )
  {
    ଶୂନ୍ୟ*                  table = ଖାଲି;
    FT_Service_SFNT_Table  service;


    ଯଦି ( face && FT_IS_SFNT( face ) )
    {
      FT_FACE_FIND_SERVICE( face, service, SFNT_TABLE );
      ଯଦି ( service )
        table = service->get_table( face, tag );
    }

    ଫେରନ୍ତୁ table;
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


    ଯଦି ( !face || !FT_IS_SFNT( face ) )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Face_Handle );

    FT_FACE_FIND_SERVICE( face, service, SFNT_TABLE );
    ଯଦି ( !service )
      ଫେରନ୍ତୁ FT_THROW( Unimplemented_Feature );

    ଫେରନ୍ତୁ service->load_table( face, tag, offset, buffer, length );
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

    ଯଦି ( !face || !FT_IS_SFNT( face ) )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Face_Handle );

    FT_FACE_FIND_SERVICE( face, service, SFNT_TABLE );
    ଯଦି ( !service )
      ଫେରନ୍ତୁ FT_THROW( Unimplemented_Feature );

    ଫେରନ୍ତୁ service->table_info( face, table_index, tag, &offset, length );
  }


  /* documentation is in tttables.h */

  FT_EXPORT_DEF( FT_ULong )
  FT_Get_CMap_Language_ID( FT_CharMap  charmap )
  {
    FT_Service_TTCMaps  service;
    FT_Face             face;
    TT_CMapInfo         cmap_info;


    ଯଦି ( !charmap || !charmap->face )
      ଫେରନ୍ତୁ 0;

    face = charmap->face;
    FT_FACE_FIND_SERVICE( face, service, TT_CMAP );
    ଯଦି ( !service )
      ଫେରନ୍ତୁ 0;
    ଯଦି ( service->get_cmap_info( charmap, &cmap_info ))
      ଫେରନ୍ତୁ 0;

    ଫେରନ୍ତୁ cmap_info.language;
  }


  /* documentation is in tttables.h */

  FT_EXPORT_DEF( FT_Long )
  FT_Get_CMap_Format( FT_CharMap  charmap )
  {
    FT_Service_TTCMaps  service;
    FT_Face             face;
    TT_CMapInfo         cmap_info;


    ଯଦି ( !charmap || !charmap->face )
      ଫେରନ୍ତୁ -1;

    face = charmap->face;
    FT_FACE_FIND_SERVICE( face, service, TT_CMAP );
    ଯଦି ( !service )
      ଫେରନ୍ତୁ -1;
    ଯଦି ( service->get_cmap_info( charmap, &cmap_info ))
      ଫେରନ୍ତୁ -1;

    ଫେରନ୍ତୁ cmap_info.format;
  }


  /* documentation is in ftsizes.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Activate_Size( FT_Size  size )
  {
    FT_Face  face;


    ଯଦି ( !size )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Size_Handle );

    face = size->face;
    ଯଦି ( !face || !face->driver )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Face_Handle );

    /* we don't need anything more complex than that; all size objects */
    /* are already listed by the face                                  */
    face->size = size;

    ଫେରନ୍ତୁ FT_Err_Ok;
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
                      FT_List_ଗଣ୍ଠି*     ଗଣ୍ଠି )
  {
    FT_List_ଗଣ୍ଠି  cur;
    FT_Renderer  result = ଖାଲି;


    ଯଦି ( !library )
      ଯାଅ Exit;

    cur = library->renderers.head;

    ଯଦି ( ଗଣ୍ଠି )
    {
      ଯଦି ( *ଗଣ୍ଠି )
        cur = (*ଗଣ୍ଠି)->next;
      *ଗଣ୍ଠି = ଖାଲି;
    }

    ଯେତେବେଳେ ( cur )
    {
      FT_Renderer  renderer = FT_RENDERER( cur->data );


      ଯଦି ( renderer->glyph_format == format )
      {
        ଯଦି ( ଗଣ୍ଠି )
          *ଗଣ୍ଠି = cur;

        result = renderer;
        ଭାଙ୍ଗ;
      }
      cur = cur->next;
    }

  Exit:
    ଫେରନ୍ତୁ result;
  }


  ସ୍ଥିର FT_Renderer
  ft_lookup_glyph_renderer( FT_GlyphSlot  slot )
  {
    FT_Face      face    = slot->face;
    FT_Library   library = FT_FACE_LIBRARY( face );
    FT_Renderer  result  = library->cur_renderer;


    ଯଦି ( !result || result->glyph_format != slot->format )
      result = FT_Lookup_Renderer( library, slot->format, 0 );

    ଫେରନ୍ତୁ result;
  }


  ସ୍ଥିର ଶୂନ୍ୟ
  ft_set_current_renderer( FT_Library  library )
  {
    FT_Renderer  renderer;


    renderer = FT_Lookup_Renderer( library, FT_GLYPH_FORMAT_OUTLINE, 0 );
    library->cur_renderer = renderer;
  }


  ସ୍ଥିର FT_Error
  ft_add_renderer( FT_Module  module )
  {
    FT_Library   library = module->library;
    FT_Memory    memory  = library->memory;
    FT_Error     error;
    FT_List_ଗଣ୍ଠି  ଗଣ୍ଠି    = ଖାଲି;


    ଯଦି ( FT_QNEW( ଗଣ୍ଠି ) )
      ଯାଅ Exit;

    {
      FT_Renderer         render = FT_RENDERER( module );
      FT_Renderer_Class*  clazz  = (FT_Renderer_Class*)module->clazz;


      render->clazz        = clazz;
      render->glyph_format = clazz->glyph_format;

      /* allocate raster object if needed */
      ଯଦି ( clazz->raster_class && clazz->raster_class->raster_new )
      {
        error = clazz->raster_class->raster_new( memory, &render->raster );
        ଯଦି ( error )
          ଯାଅ Fail;

        render->raster_render = clazz->raster_class->raster_render;
        render->render        = clazz->render_glyph;
      }

#ifdef FT_CONFIG_OPTION_SVG
      ଯଦି ( clazz->glyph_format == FT_GLYPH_FORMAT_SVG )
        render->render = clazz->render_glyph;
#endif

      /* add to list */
      ଗଣ୍ଠି->data = module;
      FT_List_Add( &library->renderers, ଗଣ୍ଠି );

      ft_set_current_renderer( library );
    }

  Fail:
    ଯଦି ( error )
      FT_FREE( ଗଣ୍ଠି );

  Exit:
    ଫେରନ୍ତୁ error;
  }


  ସ୍ଥିର ଶୂନ୍ୟ
  ft_remove_renderer( FT_Module  module )
  {
    FT_Library   library;
    FT_Memory    memory;
    FT_List_ଗଣ୍ଠି  ଗଣ୍ଠି;


    library = module->library;
    ଯଦି ( !library )
      ଫେରନ୍ତୁ;

    memory = library->memory;

    ଗଣ୍ଠି = FT_List_Find( &library->renderers, module );
    ଯଦି ( ଗଣ୍ଠି )
    {
      FT_Renderer  render = FT_RENDERER( module );


      /* release raster object, if any */
      ଯଦି ( render->raster )
        render->clazz->raster_class->raster_done( render->raster );

      /* remove from list */
      FT_List_Remove( &library->renderers, ଗଣ୍ଠି );
      FT_FREE( ଗଣ୍ଠି );

      ft_set_current_renderer( library );
    }
  }


  /* documentation is in ftrender.h */

  FT_EXPORT_DEF( FT_Renderer )
  FT_Get_Renderer( FT_Library       library,
                   FT_Glyph_Format  format )
  {
    /* test for valid `library' delayed to `FT_Lookup_Renderer' */

    ଫେରନ୍ତୁ FT_Lookup_Renderer( library, format, 0 );
  }


  /* documentation is in ftrender.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Set_Renderer( FT_Library     library,
                   FT_Renderer    renderer,
                   FT_UInt        ଅଙ୍କ_params,
                   FT_Parameter*  parameters )
  {
    FT_List_ଗଣ୍ଠି  ଗଣ୍ଠି;
    FT_Error     error = FT_Err_Ok;

    FT_Renderer_SetModeFunc  set_mode;


    ଯଦି ( !library )
    {
      error = FT_THROW( Invalid_Library_Handle );
      ଯାଅ Exit;
    }

    ଯଦି ( !renderer )
    {
      error = FT_THROW( Invalid_Argument );
      ଯାଅ Exit;
    }

    ଯଦି ( ଅଙ୍କ_params > 0 && !parameters )
    {
      error = FT_THROW( Invalid_Argument );
      ଯାଅ Exit;
    }

    ଗଣ୍ଠି = FT_List_Find( &library->renderers, renderer );
    ଯଦି ( !ଗଣ୍ଠି )
    {
      error = FT_THROW( Invalid_Argument );
      ଯାଅ Exit;
    }

    FT_List_Up( &library->renderers, ଗଣ୍ଠି );

    ଯଦି ( renderer->glyph_format == FT_GLYPH_FORMAT_OUTLINE )
      library->cur_renderer = renderer;

    set_mode = renderer->clazz->set_mode;

    ପାଇଁ ( ; ଅଙ୍କ_params > 0; ଅଙ୍କ_params-- )
    {
      error = set_mode( renderer, parameters->tag, parameters->data );
      ଯଦି ( error )
        ଭାଙ୍ଗ;
      parameters++;
    }

  Exit:
    ଫେରନ୍ତୁ error;
  }


  FT_BASE_DEF( FT_Error )
  FT_Render_Glyph_Internal( FT_Library      library,
                            FT_GlyphSlot    slot,
                            FT_Render_Mode  render_mode )
  {
    FT_Error     error = FT_Err_Ok;
    FT_Face      face  = slot->face;
    FT_Renderer  renderer;
    FT_List_ଗଣ୍ଠି  ଗଣ୍ଠି  = ଖାଲି;


    /* try to render colored glyph layers as a special case */
    ଯଦି ( slot->internal->load_flags & FT_LOAD_COLOR &&
         slot->format == FT_GLYPH_FORMAT_OUTLINE    )
    {
      FT_LayerIterator  iterator;

      FT_UInt  base_glyph = slot->glyph_index;

      FT_Bool  have_layers;
      FT_UInt  glyph_index;
      FT_UInt  color_index;


      iterator.p  = ଖାଲି;
      have_layers = FT_Get_Color_Glyph_Layer( face,
                                              base_glyph,
                                              &glyph_index,
                                              &color_index,
                                              &iterator );
      ଯଦି ( have_layers )
      {
        error = FT_New_GlyphSlot( face, ଖାଲି );
        ଯଦି ( !error )
        {
          TT_Face       ttface = (TT_Face)face;
          SFNT_Service  sfnt   = (SFNT_Service)ttface->sfnt;


          କର
          {
            FT_Int32  load_flags = slot->internal->load_flags;


            /* disable the `FT_LOAD_COLOR' flag to avoid recursion */
            /* right here in this function                         */
            load_flags &= ~FT_LOAD_COLOR;

            /* render into the new `face->glyph' glyph slot */
            load_flags |= FT_LOAD_RENDER | FT_LOAD_NO_BITMAP;

            error = FT_Load_Glyph( face, glyph_index, load_flags );
            ଯଦି ( error )
              ଭାଙ୍ଗ;

            /* blend new `face->glyph' into old `slot'; */
            /* at the first call, `slot' is still empty */
            error = sfnt->colr_blend( ttface,
                                      color_index,
                                      slot,
                                      face->glyph );
            ଯଦି ( error )
              ଭାଙ୍ଗ;

          } ଯେତେବେଳେ ( FT_Get_Color_Glyph_Layer( face,
                                              base_glyph,
                                              &glyph_index,
                                              &color_index,
                                              &iterator ) );

          ଯଦି ( !error )
            slot->format = FT_GLYPH_FORMAT_BITMAP;

          /* this call also restores `slot' as the glyph slot */
          FT_Done_GlyphSlot( face->glyph );
        }

        ଯଦି ( !error )
          ଫେରନ୍ତୁ error;

        /* Failed to do the colored layer.  Draw outline instead. */
        slot->format = FT_GLYPH_FORMAT_OUTLINE;
      }
    }

    /* small shortcut for the very common case */
    ଯଦି ( slot->format == FT_GLYPH_FORMAT_OUTLINE )
    {
      renderer = library->cur_renderer;
      ଗଣ୍ଠି     = library->renderers.head;
    }
    ଅନ୍ୟଥା
      renderer = FT_Lookup_Renderer( library, slot->format, &ଗଣ୍ଠି );

    error = FT_ERR( Cannot_Render_Glyph );
    ଯେତେବେଳେ ( renderer )
    {
      error = renderer->render( renderer, slot, render_mode, ଖାଲି );
      ଯଦି ( !error                                   ||
           FT_ERR_NEQ( error, Cannot_Render_Glyph ) )
        ଭାଙ୍ଗ;

      /* `FT_Err_Cannot_Render_Glyph` is returned if the render mode */
      /* is unsupported by the current renderer for this glyph image */
      /* format.                                                     */

      /* now, look for another renderer that supports the same */
      /* format.                                               */
      renderer = FT_Lookup_Renderer( library, slot->format, &ଗଣ୍ଠି );
    }

    /* it is not an error if we cannot render a bitmap glyph */
    ଯଦି ( FT_ERR_EQ( error, Cannot_Render_Glyph ) &&
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
    ଯଦି ( !error                               &&
         ft_trace_levels[trace_checksum] >= 3 &&
         slot->bitmap.buffer                  )
    {
      FT_Bitmap  bitmap;
      FT_Error   err;


      FT_Bitmap_Init( &bitmap );

      /* we convert to a single bitmap format for computing the checksum */
      /* this also converts the bitmap flow to `down' (i.e., pitch > 0)  */
      err = FT_Bitmap_Convert( library, &slot->bitmap, &bitmap, 1 );
      ଯଦି ( !err )
      {
        MD5_CTX        ctx;
        unsigned ଅକ୍ଷର  md5[16];
        unsigned long  coverage = 0;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା            i, j;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା            rows  = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)bitmap.rows;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା            pitch = bitmap.pitch;


        FT_TRACE3(( "FT_Render_Glyph: bitmap %dx%d, %s (mode %d)\n",
                    pitch,
                    rows,
                    pixel_modes[slot->bitmap.pixel_mode],
                    slot->bitmap.pixel_mode ));

        ପାଇଁ ( i = 0; i < rows; i++ )
          ପାଇଁ ( j = 0; j < pitch; j++ )
            coverage += bitmap.buffer[i * pitch + j];

        FT_TRACE3(( "  Total coverage: %lu\n", coverage ));

        MD5_Init( &ctx );
        ଯଦି ( bitmap.buffer )
          MD5_Update( &ctx, bitmap.buffer,
                      (unsigned long)rows * (unsigned long)pitch );
        MD5_Final( md5, &ctx );

        FT_TRACE3(( "  MD5 checksum: " ));
        ପାଇଁ ( i = 0; i < 16; i++ )
          FT_TRACE3(( "%02X", md5[i] ));
        FT_TRACE3(( "\n" ));
      }

      FT_Bitmap_Done( library, &bitmap );
    }

    /*
     * Dump bitmap in Netpbm format (PBM or PGM).
     */

    /* we use FT_TRACE7 in this block */
    ଯଦି ( !error                               &&
         ft_trace_levels[trace_checksum] >= 7 &&
         slot->bitmap.buffer                  )
    {
      ଯଦି ( slot->bitmap.rows  < 128U &&
           slot->bitmap.width < 128U )
      {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା  rows  = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)slot->bitmap.rows;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା  width = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)slot->bitmap.width;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା  pitch =      slot->bitmap.pitch;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା  i, j, m;

        unsigned ଅକ୍ଷର*  topleft = slot->bitmap.buffer;


        ଯଦି ( pitch < 0 )
          topleft -= pitch * ( rows - 1 );

        FT_TRACE7(( "Netpbm image: start\n" ));
        ବାଛ ( slot->bitmap.pixel_mode )
        {
        ସ୍ଥିତି FT_PIXEL_MODE_MONO:
          FT_TRACE7(( "P1 %d %d\n", width, rows ));
          ପାଇଁ ( i = 0; i < rows; i++ )
          {
            ପାଇଁ ( j = 0; j < width; )
              ପାଇଁ ( m = 128; m > 0 && j < width; m >>= 1, j++ )
                FT_TRACE7(( " %d",
                            ( topleft[i * pitch + j / 8] & m ) != 0 ));
            FT_TRACE7(( "\n" ));
          }
          ଭାଙ୍ଗ;

        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
          FT_TRACE7(( "P2 %d %d 255\n", width, rows ));
          ପାଇଁ ( i = 0; i < rows; i++ )
          {
            ପାଇଁ ( j = 0; j < width; j += 1 )
              FT_TRACE7(( " %3u", topleft[i * pitch + j] ));
            FT_TRACE7(( "\n" ));
          }
        }
        FT_TRACE7(( "Netpbm image: end\n" ));
      }
      ଅନ୍ୟଥା
        FT_TRACE7(( "Netpbm image: too large, omitted\n" ));
    }

#undef  FT_COMPONENT
#define FT_COMPONENT  objs

#endif /* FT_DEBUG_LEVEL_TRACE */

    ଫେରନ୍ତୁ error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Render_Glyph( FT_GlyphSlot    slot,
                   FT_Render_Mode  render_mode )
  {
    FT_Library  library;


    ଯଦି ( !slot || !slot->face )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Argument );

    library = FT_FACE_LIBRARY( slot->face );

    ଫେରନ୍ତୁ FT_Render_Glyph_Internal( library, slot, render_mode );
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
  ସ୍ଥିର ଶୂନ୍ୟ
  Destroy_Module( FT_Module  module )
  {
    ଅଚଳ FT_Module_Class*  clazz   = module->clazz;
    FT_Library              library = module->library;
    FT_Memory               memory  = module->memory;


    ଯଦି ( library && library->auto_hinter == module )
      library->auto_hinter = ଖାଲି;

    /* if the module is a renderer */
    ଯଦି ( FT_MODULE_IS_RENDERER( module ) )
      ft_remove_renderer( module );

    /* if the module is a font driver, add some steps */
    ଯଦି ( FT_MODULE_IS_DRIVER( module ) )
      Destroy_Driver( FT_DRIVER( module ) );

    /* finalize the module object */
    ଯଦି ( clazz->module_done )
      clazz->module_done( module );

    /* discard it */
    FT_FREE( module );
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Add_Module( FT_Library              library,
                 ଅଚଳ FT_Module_Class*  clazz )
  {
    FT_Error   error;
    FT_Memory  memory;
    FT_Module  module = ଖାଲି;
    FT_UInt    nn;


#define FREETYPE_VER_FIXED  ( ( (FT_Long)FREETYPE_MAJOR << 16 ) | \
                                FREETYPE_MINOR                  )

    ଯଦି ( !library )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Library_Handle );

    ଯଦି ( !clazz )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Argument );

    /* check FreeType version */
    ଯଦି ( clazz->module_requires > FREETYPE_VER_FIXED )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Version );

    /* look for a module with the same name in the library's table */
    ପାଇଁ ( nn = 0; nn < library->ଅଙ୍କ_modules; nn++ )
    {
      module = library->modules[nn];
      ଯଦି ( ft_strcmp( module->clazz->module_name, clazz->module_name ) == 0 )
      {
        /* this installed module has the same name, compare their versions */
        ଯଦି ( clazz->module_version <= module->clazz->module_version )
          ଫେରନ୍ତୁ FT_THROW( Lower_Module_Version );

        /* remove the module from our list, then exit the loop to replace */
        /* it by our new version..                                        */
        FT_Remove_Module( library, module );
        ଭାଙ୍ଗ;
      }
    }

    memory = library->memory;
    error  = FT_Err_Ok;

    ଯଦି ( library->ଅଙ୍କ_modules >= FT_MAX_MODULES )
    {
      error = FT_THROW( Too_Many_Drivers );
      ଯାଅ Exit;
    }

    /* allocate module object */
    ଯଦି ( FT_ALLOC( module, clazz->module_size ) )
      ଯାଅ Exit;

    /* base initialization */
    module->clazz   = clazz;
    module->library = library;
    module->memory  = memory;

    /* check whether the module is a renderer - this must be performed */
    /* before the normal module initialization                         */
    ଯଦି ( FT_MODULE_IS_RENDERER( module ) )
    {
      /* add to the renderers list */
      error = ft_add_renderer( module );
      ଯଦି ( error )
        ଯାଅ Fail;
    }

    /* is the module a auto-hinter? */
    ଯଦି ( FT_MODULE_IS_HINTER( module ) )
      library->auto_hinter = module;

    /* if the module is a font driver */
    ଯଦି ( FT_MODULE_IS_DRIVER( module ) )
    {
      FT_Driver  driver = FT_DRIVER( module );


      driver->clazz = (FT_Driver_Class)module->clazz;
    }

    ଯଦି ( clazz->module_init )
    {
      error = clazz->module_init( module );
      ଯଦି ( error )
        ଯାଅ Fail;
    }

    /* add module to the library's table */
    library->modules[library->ଅଙ୍କ_modules++] = module;

  Exit:
    ଫେରନ୍ତୁ error;

  Fail:
    ଯଦି ( FT_MODULE_IS_RENDERER( module ) )
    {
      FT_Renderer  renderer = FT_RENDERER( module );


      ଯଦି ( renderer->clazz                                          &&
           renderer->clazz->glyph_format == FT_GLYPH_FORMAT_OUTLINE &&
           renderer->raster                                         )
        renderer->clazz->raster_class->raster_done( renderer->raster );
    }

    FT_FREE( module );
    ଯାଅ Exit;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Module )
  FT_Get_Module( FT_Library   library,
                 ଅଚଳ ଅକ୍ଷର*  module_name )
  {
    FT_Module   result = ଖାଲି;
    FT_Module*  cur;
    FT_Module*  limit;


    ଯଦି ( !library || !module_name )
      ଫେରନ୍ତୁ result;

    cur   = library->modules;
    limit = cur + library->ଅଙ୍କ_modules;

    ପାଇଁ ( ; cur < limit; cur++ )
      ଯଦି ( ft_strcmp( cur[0]->clazz->module_name, module_name ) == 0 )
      {
        result = cur[0];
        ଭାଙ୍ଗ;
      }

    ଫେରନ୍ତୁ result;
  }


  /* documentation is in ftobjs.h */

  FT_BASE_DEF( ଅଚଳ ଶୂନ୍ୟ* )
  FT_Get_Module_Interface( FT_Library   library,
                           ଅଚଳ ଅକ୍ଷର*  mod_name )
  {
    FT_Module  module;


    /* test for valid `library' delayed to FT_Get_Module() */

    module = FT_Get_Module( library, mod_name );

    ଫେରନ୍ତୁ module ? module->clazz->module_interface : 0;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_module_get_service( FT_Module    module,
                         ଅଚଳ ଅକ୍ଷର*  service_id,
                         FT_Bool      global )
  {
    FT_Pointer  result = ଖାଲି;


    ଯଦି ( module )
    {
      FT_ASSERT( module->clazz && module->clazz->get_interface );

      /* first, look for the service in the module */
      ଯଦି ( module->clazz->get_interface )
        result = module->clazz->get_interface( module, service_id );

      ଯଦି ( global && !result )
      {
        /* we didn't find it, look in all other modules then */
        FT_Library  library = module->library;
        FT_Module*  cur     = library->modules;
        FT_Module*  limit   = cur + library->ଅଙ୍କ_modules;


        ପାଇଁ ( ; cur < limit; cur++ )
        {
          ଯଦି ( cur[0] != module )
          {
            FT_ASSERT( cur[0]->clazz );

            ଯଦି ( cur[0]->clazz->get_interface )
            {
              result = cur[0]->clazz->get_interface( cur[0], service_id );
              ଯଦି ( result )
                ଭାଙ୍ଗ;
            }
          }
        }
      }
    }

    ଫେରନ୍ତୁ result;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Remove_Module( FT_Library  library,
                    FT_Module   module )
  {
    /* try to find the module from the table, then remove it from there */

    ଯଦି ( !library )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Library_Handle );

    ଯଦି ( module )
    {
      FT_Module*  cur   = library->modules;
      FT_Module*  limit = cur + library->ଅଙ୍କ_modules;


      ପାଇଁ ( ; cur < limit; cur++ )
      {
        ଯଦି ( cur[0] == module )
        {
          /* remove it from the table */
          library->ଅଙ୍କ_modules--;
          limit--;
          ଯେତେବେଳେ ( cur < limit )
          {
            cur[0] = cur[1];
            cur++;
          }
          limit[0] = ଖାଲି;

          /* destroy the module */
          Destroy_Module( module );

          ଫେରନ୍ତୁ FT_Err_Ok;
        }
      }
    }
    ଫେରନ୍ତୁ FT_THROW( Invalid_Driver_Handle );
  }


  ସ୍ଥିର FT_Error
  ft_property_do( FT_Library        library,
                  ଅଚଳ FT_String*  module_name,
                  ଅଚଳ FT_String*  property_name,
                  ଶୂନ୍ୟ*             ମୂଲ୍ୟ,
                  FT_Bool           set,
                  FT_Bool           ମୂଲ୍ୟ_is_string )
  {
    FT_Module*           cur;
    FT_Module*           limit;
    FT_Module_Interface  interface;

    FT_Service_Properties  service;

#ifdef FT_DEBUG_LEVEL_ERROR
    ଅଚଳ FT_String*  set_name  = "FT_Property_Set";
    ଅଚଳ FT_String*  get_name  = "FT_Property_Get";
    ଅଚଳ FT_String*  func_name = set ? set_name : get_name;
#endif

    FT_Bool  missing_func;


    ଯଦି ( !library )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Library_Handle );

    ଯଦି ( !module_name || !property_name || !ମୂଲ୍ୟ )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Argument );

    cur   = library->modules;
    limit = cur + library->ଅଙ୍କ_modules;

    /* search module */
    ପାଇଁ ( ; cur < limit; cur++ )
      ଯଦି ( !ft_strcmp( cur[0]->clazz->module_name, module_name ) )
        ଭାଙ୍ଗ;

    ଯଦି ( cur == limit )
    {
      FT_TRACE2(( "%s: can't find module `%s'\n",
                  func_name, module_name ));
      ଫେରନ୍ତୁ FT_THROW( Missing_Module );
    }

    /* check whether we have a service interface */
    ଯଦି ( !cur[0]->clazz->get_interface )
    {
      FT_TRACE2(( "%s: module `%s' doesn't support properties\n",
                  func_name, module_name ));
      ଫେରନ୍ତୁ FT_THROW( Unimplemented_Feature );
    }

    /* search property service */
    interface = cur[0]->clazz->get_interface( cur[0],
                                              FT_SERVICE_ID_PROPERTIES );
    ଯଦି ( !interface )
    {
      FT_TRACE2(( "%s: module `%s' doesn't support properties\n",
                  func_name, module_name ));
      ଫେରନ୍ତୁ FT_THROW( Unimplemented_Feature );
    }

    service = (FT_Service_Properties)interface;

    ଯଦି ( set )
      missing_func = FT_BOOL( !service->set_property );
    ଅନ୍ୟଥା
      missing_func = FT_BOOL( !service->get_property );

    ଯଦି ( missing_func )
    {
      FT_TRACE2(( "%s: property service of module `%s' is broken\n",
                  func_name, module_name ));
      ଫେରନ୍ତୁ FT_THROW( Unimplemented_Feature );
    }

    ଫେରନ୍ତୁ set ? service->set_property( cur[0],
                                        property_name,
                                        ମୂଲ୍ୟ,
                                        ମୂଲ୍ୟ_is_string )
               : service->get_property( cur[0],
                                        property_name,
                                        ମୂଲ୍ୟ );
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Property_Set( FT_Library        library,
                   ଅଚଳ FT_String*  module_name,
                   ଅଚଳ FT_String*  property_name,
                   ଅଚଳ ଶୂନ୍ୟ*       ମୂଲ୍ୟ )
  {
    ଫେରନ୍ତୁ ft_property_do( library,
                           module_name,
                           property_name,
                           (ଶୂନ୍ୟ*)ମୂଲ୍ୟ,
                           TRUE,
                           FALSE );
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Property_Get( FT_Library        library,
                   ଅଚଳ FT_String*  module_name,
                   ଅଚଳ FT_String*  property_name,
                   ଶୂନ୍ୟ*             ମୂଲ୍ୟ )
  {
    ଫେରନ୍ତୁ ft_property_do( library,
                           module_name,
                           property_name,
                           ମୂଲ୍ୟ,
                           FALSE,
                           FALSE );
  }


#ifdef FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES

  /* this variant is used for handling the FREETYPE_PROPERTIES */
  /* environment variable                                      */

  FT_BASE_DEF( FT_Error )
  ft_property_string_set( FT_Library        library,
                          ଅଚଳ FT_String*  module_name,
                          ଅଚଳ FT_String*  property_name,
                          FT_String*        ମୂଲ୍ୟ )
  {
    ଫେରନ୍ତୁ ft_property_do( library,
                           module_name,
                           property_name,
                           (ଶୂନ୍ୟ*)ମୂଲ୍ୟ,
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
    ଯଦି ( !library )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Library_Handle );

    library->refcount++;

    ଫେରନ୍ତୁ FT_Err_Ok;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_New_Library( FT_Memory    memory,
                  FT_Library  *alibrary )
  {
    FT_Library  library = ଖାଲି;
    FT_Error    error;


    ଯଦି ( !memory || !alibrary )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Argument );

#ifndef FT_DEBUG_LOGGING
#ifdef FT_DEBUG_LEVEL_ERROR
    /* init debugging support */
    ft_debug_init();
#endif /* FT_DEBUG_LEVEL_ERROR */
#endif /* !FT_DEBUG_LOGGING */

    /* first of all, allocate the library object */
    ଯଦି ( FT_NEW( library ) )
      ଫେରନ୍ତୁ error;

    library->memory = memory;

    library->version_major = FREETYPE_MAJOR;
    library->version_minor = FREETYPE_MINOR;
    library->version_patch = FREETYPE_PATCH;

    library->refcount = 1;

    /* That's ok now */
    *alibrary = library;

    ଫେରନ୍ତୁ FT_Err_Ok;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( ଶୂନ୍ୟ )
  FT_Library_Version( FT_Library   library,
                      FT_Int      *amajor,
                      FT_Int      *aminor,
                      FT_Int      *apatch )
  {
    FT_Int  major = 0;
    FT_Int  minor = 0;
    FT_Int  patch = 0;


    ଯଦି ( library )
    {
      major = library->version_major;
      minor = library->version_minor;
      patch = library->version_patch;
    }

    ଯଦି ( amajor )
      *amajor = major;

    ଯଦି ( aminor )
      *aminor = minor;

    ଯଦି ( apatch )
      *apatch = patch;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Done_Library( FT_Library  library )
  {
    FT_Memory  memory;


    ଯଦି ( !library )
      ଫେରନ୍ତୁ FT_THROW( Invalid_Library_Handle );

    library->refcount--;
    ଯଦି ( library->refcount > 0 )
      ଯାଅ Exit;

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
      ଅଚଳ ଅକ୍ଷର*  driver_name[] = { "type42", ଖାଲି };


      ପାଇଁ ( m = 0;
            m < ଆକାର ( driver_name ) / ଆକାର ( driver_name[0] );
            m++ )
      {
        ପାଇଁ ( n = 0; n < library->ଅଙ୍କ_modules; n++ )
        {
          FT_Module    module      = library->modules[n];
          ଅଚଳ ଅକ୍ଷର*  module_name = module->clazz->module_name;
          FT_List      faces;


          ଯଦି ( driver_name[m]                                &&
               ft_strcmp( module_name, driver_name[m] ) != 0 )
            ଜାରି;

          ଯଦି ( ( module->clazz->module_flags & FT_MODULE_FONT_DRIVER ) == 0 )
            ଜାରି;

          FT_TRACE7(( "FT_Done_Library: close faces for %s\n", module_name ));

          faces = &FT_DRIVER( module )->faces_list;
          ଯେତେବେଳେ ( faces->head )
          {
            FT_Done_Face( FT_FACE( faces->head->data ) );
            ଯଦି ( faces->head )
              FT_TRACE0(( "FT_Done_Library: failed to free some faces\n" ));
          }
        }
      }
    }

    /* Close all other modules in the library */
#ଯଦି 1
    /* XXX Modules are removed in the reversed order so that  */
    /* type42 module is removed before truetype module.  This */
    /* avoids double free in some occasions.  It is a hack.   */
    ଯେତେବେଳେ ( library->ଅଙ୍କ_modules > 0 )
      FT_Remove_Module( library,
                        library->modules[library->ଅଙ୍କ_modules - 1] );
#ଅନ୍ୟଥା
    {
      FT_UInt  n;


      ପାଇଁ ( n = 0; n < library->ଅଙ୍କ_modules; n++ )
      {
        FT_Module  module = library->modules[n];


        ଯଦି ( module )
        {
          Destroy_Module( module );
          library->modules[n] = ଖାଲି;
        }
      }
    }
#endif

    FT_FREE( library );

  Exit:
    ଫେରନ୍ତୁ FT_Err_Ok;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( ଶୂନ୍ୟ )
  FT_Set_Debug_Hook( FT_Library         library,
                     FT_UInt            hook_index,
                     FT_DebugHook_Func  debug_hook )
  {
    ଯଦି ( library && debug_hook &&
         hook_index <
           ( ଆକାର ( library->debug_hooks ) / ଆକାର ( ଶୂନ୍ୟ* ) ) )
      library->debug_hooks[hook_index] = debug_hook;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_TrueTypeEngineType )
  FT_Get_TrueType_Engine_Type( FT_Library  library )
  {
    FT_TrueTypeEngineType  result = FT_TRUETYPE_ENGINE_TYPE_NONE;


    ଯଦି ( library )
    {
      FT_Module  module = FT_Get_Module( library, "truetype" );


      ଯଦି ( module )
      {
        FT_Service_TrueTypeEngine  service;


        service = (FT_Service_TrueTypeEngine)
                    ft_module_get_service( module,
                                           FT_SERVICE_ID_TRUETYPE_ENGINE,
                                           0 );
        ଯଦି ( service )
          result = service->engine_type;
      }
    }

    ଫେରନ୍ତୁ result;
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


    ଯଦି ( glyph                                      &&
         glyph->subglyphs                           &&
         glyph->format == FT_GLYPH_FORMAT_COMPOSITE &&
         sub_index < glyph->ଅଙ୍କ_subglyphs           )
    {
      FT_SubGlyph  subg = glyph->subglyphs + sub_index;


      *p_index     = subg->index;
      *p_flags     = subg->flags;
      *p_arg1      = subg->arg1;
      *p_arg2      = subg->arg2;
      *p_transform = subg->transform;

      error = FT_Err_Ok;
    }

    ଫେରନ୍ତୁ error;
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


    ଯଦି ( !face                                   ||
         !aglyph_index                           ||
         !acolor_index                           ||
         !iterator                               ||
         base_glyph >= (FT_UInt)face->ଅଙ୍କ_glyphs )
      ଫେରନ୍ତୁ 0;

    ଯଦି ( !FT_IS_SFNT( face ) )
      ଫେରନ୍ତୁ 0;

    ttface = (TT_Face)face;
    sfnt   = (SFNT_Service)ttface->sfnt;

    ଯଦି ( sfnt->get_colr_layer )
      ଫେରନ୍ତୁ sfnt->get_colr_layer( ttface,
                                   base_glyph,
                                   aglyph_index,
                                   acolor_index,
                                   iterator );
    ଅନ୍ୟଥା
      ଫେରନ୍ତୁ 0;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Get_Color_Glyph_Paint( FT_Face                  face,
                            FT_UInt                  base_glyph,
                            FT_Color_ମୂଳ_Transform  ମୂଳ_transform,
                            FT_OpaquePaint*          paint )
  {
    TT_Face       ttface;
    SFNT_Service  sfnt;


    ଯଦି ( !face || !paint )
      ଫେରନ୍ତୁ 0;

    ଯଦି ( !FT_IS_SFNT( face ) )
      ଫେରନ୍ତୁ 0;

    ttface = (TT_Face)face;
    sfnt   = (SFNT_Service)ttface->sfnt;

    ଯଦି ( sfnt->get_colr_glyph_paint )
      ଫେରନ୍ତୁ sfnt->get_colr_glyph_paint( ttface,
                                         base_glyph,
                                         ମୂଳ_transform,
                                         paint );
    ଅନ୍ୟଥା
      ଫେରନ୍ତୁ 0;
  }


  /* documentation is in ftcolor.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Get_Color_Glyph_ClipBox( FT_Face      face,
                              FT_UInt      base_glyph,
                              FT_ClipBox*  clip_box )
  {
    TT_Face       ttface;
    SFNT_Service  sfnt;


    ଯଦି ( !face || !clip_box )
      ଫେରନ୍ତୁ 0;

    ଯଦି ( !FT_IS_SFNT( face ) )
      ଫେରନ୍ତୁ 0;

    ttface = (TT_Face)face;
    sfnt   = (SFNT_Service)ttface->sfnt;

    ଯଦି ( sfnt->get_color_glyph_clipbox )
      ଫେରନ୍ତୁ sfnt->get_color_glyph_clipbox( ttface,
                                            base_glyph,
                                            clip_box );
    ଅନ୍ୟଥା
      ଫେରନ୍ତୁ 0;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Get_Paint_Layers( FT_Face            face,
                       FT_LayerIterator*  layer_iterator,
                       FT_OpaquePaint*    paint )
  {
    TT_Face       ttface;
    SFNT_Service  sfnt;


    ଯଦି ( !face || !paint || !layer_iterator )
      ଫେରନ୍ତୁ 0;

    ଯଦି ( !FT_IS_SFNT( face ) )
      ଫେରନ୍ତୁ 0;

    ttface = (TT_Face)face;
    sfnt   = (SFNT_Service)ttface->sfnt;

    ଯଦି ( sfnt->get_paint_layers )
      ଫେରନ୍ତୁ sfnt->get_paint_layers( ttface, layer_iterator, paint );
    ଅନ୍ୟଥା
      ଫେରନ୍ତୁ 0;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Get_Paint( FT_Face face,
                FT_OpaquePaint  opaque_paint,
                FT_COLR_Paint*  paint )
  {
    TT_Face       ttface;
    SFNT_Service  sfnt;


    ଯଦି ( !face || !paint )
      ଫେରନ୍ତୁ 0;

    ଯଦି ( !FT_IS_SFNT( face ) )
      ଫେରନ୍ତୁ 0;

    ttface = (TT_Face)face;
    sfnt   = (SFNT_Service)ttface->sfnt;

    ଯଦି ( sfnt->get_paint )
      ଫେରନ୍ତୁ sfnt->get_paint( ttface, opaque_paint, paint );
    ଅନ୍ୟଥା
      ଫେରନ୍ତୁ 0;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Get_Colorline_Stops ( FT_Face                face,
                           FT_ColorStop *         color_stop,
                           FT_ColorStopIterator  *iterator )
  {
    TT_Face       ttface;
    SFNT_Service  sfnt;


    ଯଦି ( !face || !color_stop || !iterator )
      ଫେରନ୍ତୁ 0;

    ଯଦି ( !FT_IS_SFNT( face ) )
      ଫେରନ୍ତୁ 0;

    ttface = (TT_Face)face;
    sfnt   = (SFNT_Service)ttface->sfnt;

    ଯଦି ( sfnt->get_colorline_stops )
      ଫେରନ୍ତୁ sfnt->get_colorline_stops ( ttface, color_stop, iterator );
    ଅନ୍ୟଥା
      ଫେରନ୍ତୁ 0;
  }


/* END */
