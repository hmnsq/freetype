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


#include <freetype/ftlist.h>
#include <freetype/ftoutln.h>
#include <freetype/ftfntfmt.h>
#include <freetype/otsvg.h>

#include <freetype/internal/ftvalid.h>
#include <freetype/internal/ftobjs.h>
#include <freetype/internal/ftdebug.h>
#include <freetype/internal/ftrfork.h>
#include <freetype/internal/ftstream.h>
#include <freetype/internal/sfnt.h>          /* for SFNT_Load_Table_Func */
#include <freetype/internal/psaux.h>         /* for PS_Driver            */
#include <freetype/internal/svginterface.h>

#include <freetype/tttables.h>
#include <freetype/tttags.h>
#include <freetype/ttnameid.h>

#include <freetype/internal/services/svprop.h>
#include <freetype/internal/services/svsfnt.h>
#include <freetype/internal/services/svpostnm.h>
#include <freetype/internal/services/svgldict.h>
#include <freetype/internal/services/svttcmap.h>
#include <freetype/internal/services/svkern.h>
#include <freetype/internal/services/svtteng.h>

#ifdef TT_CONFIG_OPTION_VARC
#include <freetype/internal/services/svvarc.h>
#endif

#include <freetype/ftdriver.h>

#ifdef FT_CONFIG_OPTION_MAC_FONTS
#include "ftbase.h"
#endif


#ifdef FT_DEBUG_LEVEL_TRACE

#include <freetype/ftbitmap.h>

#ᱡᱩᱫᱤ defined( _MSC_VER )      /* Visual C++ (and Intel C++)   */
  /* We disable the warning `conversion from XXX to YYY,     */
  /* possible loss of data' in order to compile cleanly with */
  /* the maximum level of warnings: `md5.c' is non-FreeType  */
  /* code, and it gets used during development builds only.  */
#pragma warning( push )
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

#include "md5.c"

#ᱡᱩᱫᱤ defined( _MSC_VER )
#pragma warning( pop )
#endif

  /* This array must stay in sync with the @FT_Pixel_Mode enumeration */
  /* (in file `ftimage.h`).                                           */

  static const var_na* const  pixel_modes[] =
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
  static FT_Error
  ft_open_face_internal( FT_Library           library,
                         const FT_Open_Args*  args,
                         FT_Long              face_index,
                         FT_Face             *aface,
                         FT_Bool              test_mac_fonts );


  FT_BASE_DEF( FT_Pointer )
  ft_service_list_lookup( FT_ServiceDesc  service_descriptors,
                          const var_na*     service_id )
  {
    FT_Pointer      result = NULL;
    FT_ServiceDesc  desc   = service_descriptors;


    ᱡᱩᱫᱤ ( desc && service_id )
    {
      for ( ; desc->serv_id != NULL; desc++ )
      {
        ᱡᱩᱫᱤ ( ft_strcmp( desc->serv_id, service_id ) == 0 )
        {
          result = (FT_Pointer)desc->serv_data;
          bha_nga;
        }
      }
    }

    ᱨᱩᱣᱟᱹᱲ result;
  }


  FT_BASE_DEF( void )
  ft_validator_init( FT_Validator        valid,
                     const FT_Byte*      base,
                     const FT_Byte*      limit,
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

    ᱨᱩᱣᱟᱹᱲ -1;
  }


  FT_BASE_DEF( void )
  ft_validator_error( FT_Validator  valid,
                      FT_Error      error )
  {
    /* since the cast below also disables the compiler's */
    /* type check, we introduce a dummy variable, which  */
    /* will be optimized away                            */
    ca_ncala ft_jmp_buf* jump_buffer = &valid->jump_buffer;


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
                 const FT_Open_Args*  args,
                 FT_Stream           *astream )
  {
    FT_Error   error;
    FT_Memory  memory;
    FT_Stream  stream = NULL;
    FT_UInt    mode;


    *astream = NULL;

    ᱡᱩᱫᱤ ( !library )
      ᱨᱩᱣᱟᱹᱲ FT_THROW( Invalid_Library_Handle );

    ᱡᱩᱫᱤ ( !args )
      ᱨᱩᱣᱟᱹᱲ FT_THROW( Invalid_Argument );

    memory = library->memory;
    mode   = args->flags &
               ( FT_OPEN_MEMORY | FT_OPEN_STREAM | FT_OPEN_PATHNAME );

    ᱡᱩᱫᱤ ( mode == FT_OPEN_MEMORY )
    {
      /* create a memory-based stream */
      ᱡᱩᱫᱤ ( FT_NEW( stream ) )
        goto Exit;

      FT_Stream_OpenMemory( stream,
                            (const FT_Byte*)args->memory_base,
                            (FT_ULong)args->memory_size );
      stream->memory = memory;
    }

#ifndef FT_CONFIG_OPTION_DISABLE_STREAM_SUPPORT

    ᱵᱟᱝ ᱡᱩᱫᱤ ( mode == FT_OPEN_PATHNAME )
    {
      /* create a normal system stream */
      ᱡᱩᱫᱤ ( FT_NEW( stream ) )
        goto Exit;

      stream->memory = memory;
      error = FT_Stream_Open( stream, args->pathname );
      ᱡᱩᱫᱤ ( error )
        FT_FREE( stream );
    }
    ᱵᱟᱝ ᱡᱩᱫᱤ ( ( mode == FT_OPEN_STREAM ) && args->stream )
    {
      /* use an existing, user-provided stream */

      /* in this case, we do not need to allocate a new stream object */
      /* since the caller is responsible for closing it himself       */
      stream         = args->stream;
      stream->memory = memory;
      error          = FT_Err_Ok;
    }

#endif

    ᱵᱟᱝ
    {
      error = FT_THROW( Invalid_Argument );
      ᱡᱩᱫᱤ ( ( args->flags & FT_OPEN_STREAM ) && args->stream )
        FT_Stream_Close( args->stream );
    }

    ᱡᱩᱫᱤ ( !error )
      *astream       = stream;

  Exit:
    ᱨᱩᱣᱟᱹᱲ error;
  }


  FT_BASE_DEF( void )
  FT_Stream_Free( FT_Stream  stream,
                  FT_Int     external )
  {
    ᱡᱩᱫᱤ ( stream )
    {
      FT_Memory  memory = stream->memory;


      FT_Stream_Close( stream );

      ᱡᱩᱫᱤ ( !external )
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


  static FT_Error
  ft_glyphslot_init( FT_GlyphSlot  slot )
  {
    FT_Driver         driver   = slot->face->driver;
    FT_Driver_Class   clazz    = driver->clazz;
    FT_Memory         memory   = driver->root.memory;
    FT_Error          error    = FT_Err_Ok;
    FT_Slot_Internal  internal = NULL;


    slot->library = driver->root.library;

    ᱡᱩᱫᱤ ( FT_NEW( internal ) )
      goto Exit;

    slot->internal = internal;

    ᱡᱩᱫᱤ ( FT_DRIVER_USES_OUTLINES( driver ) )
      error = FT_GlyphLoader_New( memory, &internal->loader );

    ᱡᱩᱫᱤ ( !error && clazz->init_slot )
      error = clazz->init_slot( slot );

#ifdef FT_CONFIG_OPTION_SVG
    /* if SVG table exists, allocate the space in `slot->other` */
    ᱡᱩᱫᱤ ( slot->face->face_flags & FT_FACE_FLAG_SVG )
    {
      FT_SVG_Document  document = NULL;


      ᱡᱩᱫᱤ ( FT_NEW( document ) )
        goto Exit;
      slot->other = document;
    }
#endif

  Exit:
    ᱨᱩᱣᱟᱹᱲ error;
  }


  FT_BASE_DEF( void )
  ft_glyphslot_free_bitmap( FT_GlyphSlot  slot )
  {
    ᱡᱩᱫᱤ ( slot->internal && ( slot->internal->flags & FT_GLYPH_OWN_BITMAP ) )
    {
      FT_Memory  memory = FT_FACE_MEMORY( slot->face );


      FT_FREE( slot->bitmap.buffer );
      slot->internal->flags &= ~FT_GLYPH_OWN_BITMAP;
    }
    ᱵᱟᱝ
    {
      /* assume that the bitmap buffer was stolen or not */
      /* allocated from the heap                         */
      slot->bitmap.buffer = NULL;
    }
  }


  /* overflow-resistant presetting of bitmap position and dimensions; */
  /* also check whether the size is too large for rendering           */
  FT_BASE_DEF( FT_Bool )
  ft_glyphslot_preset_bitmap( FT_GlyphSlot      slot,
                              FT_Render_Mode    mode,
                              const FT_Vector*  origin )
  {
    FT_Outline*  outline = &slot->outline;
    FT_Bitmap*   bitmap  = &slot->bitmap;

    FT_Pixel_Mode  pixel_mode;

    FT_BBox  cbox, pbox;
    FT_Pos   x_shift = 0;
    FT_Pos   y_shift = 0;
    FT_Pos   x_left, y_top;
    FT_Pos   width, height, pitch;
    FT_Bool  ret;


    ᱡᱩᱫᱤ ( slot->format == FT_GLYPH_FORMAT_SVG )
    {
      FT_Module    module;
      SVG_Service  svg_service;


      module      = FT_Get_Module( slot->library, "ot-svg" );
      svg_service = (SVG_Service)module->clazz->module_interface;

      ᱨᱩᱣᱟᱹᱲ (FT_Bool)svg_service->preset_slot( module, slot, FALSE );
    }
    ᱵᱟᱝ ᱡᱩᱫᱤ ( slot->format != FT_GLYPH_FORMAT_OUTLINE )
      ᱨᱩᱣᱟᱹᱲ 1;

    ᱡᱩᱫᱤ ( origin )
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

    switch ( mode )
    {
    pak_sa FT_RENDER_MODE_MONO:
      pixel_mode = FT_PIXEL_MODE_MONO;
#ᱡᱩᱫᱤ 1
      /* x */

      /* undocumented but confirmed: bbox values get rounded;    */
      /* we do asymmetric rounding so that the center of a pixel */
      /* gets always included                                    */

      pbox.xMin += ( cbox.xMin + 31 ) >> 6;
      pbox.xMax += ( cbox.xMax + 32 ) >> 6;

      /* if the bbox collapsed, we add a pixel based on the total */
      /* rounding remainder to cover most of the original cbox    */

      ᱡᱩᱫᱤ ( pbox.xMin == pbox.xMax )
      {
        ᱡᱩᱫᱤ ( ( ( cbox.xMin + 31 ) & 63 ) - 31 +
             ( ( cbox.xMax + 32 ) & 63 ) - 32 < 0 )
          pbox.xMin -= 1;
        ᱵᱟᱝ
          pbox.xMax += 1;
      }

      /* y */

      pbox.yMin += ( cbox.yMin + 31 ) >> 6;
      pbox.yMax += ( cbox.yMax + 32 ) >> 6;

      ᱡᱩᱫᱤ ( pbox.yMin == pbox.yMax )
      {
        ᱡᱩᱫᱤ ( ( ( cbox.yMin + 31 ) & 63 ) - 31 +
             ( ( cbox.yMax + 32 ) & 63 ) - 32 < 0 )
          pbox.yMin -= 1;
        ᱵᱟᱝ
          pbox.yMax += 1;
      }

      bha_nga;
#ᱵᱟᱝ
      goto Adjust;
#endif

    pak_sa FT_RENDER_MODE_LCD:
      pixel_mode = FT_PIXEL_MODE_LCD;
      ft_lcd_padding( &cbox, slot, mode );
      goto Adjust;

    pak_sa FT_RENDER_MODE_LCD_V:
      pixel_mode = FT_PIXEL_MODE_LCD_V;
      ft_lcd_padding( &cbox, slot, mode );
      goto Adjust;

    pak_sa FT_RENDER_MODE_NORMAL:
    pak_sa FT_RENDER_MODE_LIGHT:
    default:
      pixel_mode = FT_PIXEL_MODE_GRAY;
    Adjust:
      pbox.xMin += cbox.xMin >> 6;
      pbox.yMin += cbox.yMin >> 6;
      pbox.xMax += ( cbox.xMax + 63 ) >> 6;
      pbox.yMax += ( cbox.yMax + 63 ) >> 6;
    }

    x_left = pbox.xMin;
    y_top  = pbox.yMax;

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

    ᱡᱩᱫᱤ ( ret )
      FT_TRACE3(( "ft_glyphslot_preset_bitmap: [%ld %ld %ld %ld]\n",
                  pbox.xMin, pbox.yMin, pbox.xMax, pbox.yMax ));

    switch ( pixel_mode )
    {
    pak_sa FT_PIXEL_MODE_MONO:
      pitch = ( ( width + 15 ) >> 4 ) << 1;
      bha_nga;

    pak_sa FT_PIXEL_MODE_LCD:
      width *= 3;
      pitch  = FT_PAD_CEIL( width, 4 );
      bha_nga;

    pak_sa FT_PIXEL_MODE_LCD_V:
      height *= 3;
      FALL_THROUGH;

    pak_sa FT_PIXEL_MODE_GRAY:
    default:
      pitch = width;
    }

    slot->bitmap_left = (FT_Int)x_left;
    slot->bitmap_top  = (FT_Int)y_top;

    bitmap->pixel_mode = (unsigned var_na)pixel_mode;
    bitmap->num_grays  = 256;
    bitmap->width      = (unsigned puur_naa_nka)width;
    bitmap->rows       = (unsigned puur_naa_nka)height;
    bitmap->pitch      = pitch;

    ᱨᱩᱣᱟᱹᱲ ret;
  }


  FT_BASE_DEF( void )
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


    ᱡᱩᱫᱤ ( slot->internal->flags & FT_GLYPH_OWN_BITMAP )
      FT_FREE( slot->bitmap.buffer );
    ᱵᱟᱝ
      slot->internal->flags |= FT_GLYPH_OWN_BITMAP;

    /* dimensions must be preset */
    FT_MEM_ALLOC_MULT( slot->bitmap.buffer,
                       slot->bitmap.rows,
                       slot->bitmap.pitch );
    ᱨᱩᱣᱟᱹᱲ error;
  }


  static void
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

    slot->bitmap_left   = 0;
    slot->bitmap_top    = 0;
    slot->num_subglyphs = 0;
    slot->subglyphs     = NULL;
    slot->control_data  = NULL;
    slot->control_len   = 0;

#ifndef FT_CONFIG_OPTION_SVG
    slot->other = NULL;
#ᱵᱟᱝ
    ᱡᱩᱫᱤ ( !( slot->face->face_flags & FT_FACE_FLAG_SVG ) )
      slot->other = NULL;
    ᱵᱟᱝ
    {
      ᱡᱩᱫᱤ ( slot->internal->flags & FT_GLYPH_OWN_GZIP_SVG )
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


  static void
  ft_glyphslot_done( FT_GlyphSlot  slot )
  {
    FT_Driver        driver = slot->face->driver;
    FT_Driver_Class  clazz  = driver->clazz;
    FT_Memory        memory = driver->root.memory;

#ifdef FT_CONFIG_OPTION_SVG
    ᱡᱩᱫᱤ ( slot->face->face_flags & FT_FACE_FLAG_SVG )
    {
      /* Free memory in case SVG was there.                          */
      /* `slot->internal` might be NULL in out-of-memory situations. */
      ᱡᱩᱫᱤ ( slot->internal && slot->internal->flags & FT_GLYPH_OWN_GZIP_SVG )
      {
        FT_SVG_Document  doc = (FT_SVG_Document)slot->other;


        FT_FREE( doc->svg_document );

        slot->internal->flags &= ~FT_GLYPH_OWN_GZIP_SVG;
      }

      FT_FREE( slot->other );
    }
#endif

    ᱡᱩᱫᱤ ( clazz->done_slot )
      clazz->done_slot( slot );

    /* free bitmap buffer if needed */
    ft_glyphslot_free_bitmap( slot );

    /* slot->internal might be NULL in out-of-memory situations */
    ᱡᱩᱫᱤ ( slot->internal )
    {
      /* free glyph loader */
      ᱡᱩᱫᱤ ( FT_DRIVER_USES_OUTLINES( driver ) )
      {
        FT_GlyphLoader_Done( slot->internal->loader );
        slot->internal->loader = NULL;
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
    FT_GlyphSlot     slot = NULL;


    ᱡᱩᱫᱤ ( !face )
      ᱨᱩᱣᱟᱹᱲ FT_THROW( Invalid_Face_Handle );

    ᱡᱩᱫᱤ ( !face->driver )
      ᱨᱩᱣᱟᱹᱲ FT_THROW( Invalid_Argument );

    driver = face->driver;
    clazz  = driver->clazz;
    memory = driver->root.memory;

    FT_TRACE4(( "FT_New_GlyphSlot: Creating new slot object\n" ));
    ᱡᱩᱫᱤ ( !FT_ALLOC( slot, clazz->slot_object_size ) )
    {
      slot->face = face;

      error = ft_glyphslot_init( slot );
      ᱡᱩᱫᱤ ( error )
      {
        ft_glyphslot_done( slot );
        FT_FREE( slot );
        goto Exit;
      }

      slot->next  = face->glyph;
      face->glyph = slot;

      ᱡᱩᱫᱤ ( aslot )
        *aslot = slot;
    }
    ᱵᱟᱝ ᱡᱩᱫᱤ ( aslot )
      *aslot = NULL;


  Exit:
    FT_TRACE4(( "FT_New_GlyphSlot: Return 0x%x\n", error ));

    ᱨᱩᱣᱟᱹᱲ error;
  }


  /* documentation is in ftobjs.h */

  FT_BASE_DEF( void )
  FT_Done_GlyphSlot( FT_GlyphSlot  slot )
  {
    ᱡᱩᱫᱤ ( slot )
    {
      FT_Driver     driver = slot->face->driver;
      FT_Memory     memory = driver->root.memory;
      FT_GlyphSlot  prev;
      FT_GlyphSlot  cur;


      /* Remove slot from its parent face's list */
      prev = NULL;
      cur  = slot->face->glyph;

      ᱡᱚᱠᱷᱚᱱ ( cur )
      {
        ᱡᱩᱫᱤ ( cur == slot )
        {
          ᱡᱩᱫᱤ ( !prev )
            slot->face->glyph = cur->next;
          ᱵᱟᱝ
            prev->next = cur->next;

          /* finalize client-specific data */
          ᱡᱩᱫᱤ ( slot->generic.finalizer )
            slot->generic.finalizer( slot );

          ft_glyphslot_done( slot );
          FT_FREE( slot );
          bha_nga;
        }
        prev = cur;
        cur  = cur->next;
      }
    }
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( void )
  FT_Set_Transform( FT_Face     face,
                    FT_Matrix*  matrix,
                    FT_Vector*  delta )
  {
    FT_Face_Internal  internal;


    ᱡᱩᱫᱤ ( !face )
      ᱨᱩᱣᱟᱹᱲ;

    internal = face->internal;

    internal->transform_flags = 0;

    ᱡᱩᱫᱤ ( !matrix || !FT_Matrix_Check( matrix ) )
    {
      internal->transform_matrix.xx = 0x10000L;
      internal->transform_matrix.xy = 0;
      internal->transform_matrix.yx = 0;
      internal->transform_matrix.yy = 0x10000L;

      matrix = &internal->transform_matrix;
    }
    ᱵᱟᱝ
      internal->transform_matrix = *matrix;

    /* set transform_flags bit flag 0 if `matrix' isn't the identity */
    ᱡᱩᱫᱤ ( ( matrix->xy | matrix->yx ) ||
         matrix->xx != 0x10000L      ||
         matrix->yy != 0x10000L      )
      internal->transform_flags |= 1;

    ᱡᱩᱫᱤ ( !delta )
    {
      internal->transform_delta.x = 0;
      internal->transform_delta.y = 0;

      delta = &internal->transform_delta;
    }
    ᱵᱟᱝ
      internal->transform_delta = *delta;

    /* set transform_flags bit flag 1 if `delta' isn't the null vector */
    ᱡᱩᱫᱤ ( delta->x | delta->y )
      internal->transform_flags |= 2;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( void )
  FT_Get_Transform( FT_Face     face,
                    FT_Matrix*  matrix,
                    FT_Vector*  delta )
  {
    FT_Face_Internal  internal;


    ᱡᱩᱫᱤ ( !face )
      ᱨᱩᱣᱟᱹᱲ;

    internal = face->internal;

    ᱡᱩᱫᱤ ( matrix )
      *matrix = internal->transform_matrix;

    ᱡᱩᱫᱤ ( delta )
      *delta = internal->transform_delta;
  }


  static FT_Renderer
  ft_lookup_glyph_renderer( FT_GlyphSlot  slot );


#ifdef GRID_FIT_METRICS
  static void
  ft_glyphslot_grid_fit_metrics( FT_GlyphSlot  slot,
                                 FT_Bool       vertical )
  {
    FT_Glyph_Metrics*  metrics = &slot->metrics;
    FT_Pos             right, bottom;


    ᱡᱩᱫᱤ ( vertical )
    {
      metrics->horiBearingX = FT_PIX_FLOOR( metrics->horiBearingX );
      metrics->horiBearingY = FT_PIX_CEIL_LONG( metrics->horiBearingY );

      right  = FT_PIX_CEIL_LONG( ADD_LONG( metrics->vertBearingX,
                                           metrics->width ) );
      bottom = FT_PIX_CEIL_LONG( ADD_LONG( metrics->vertBearingY,
                                           metrics->height ) );

      metrics->vertBearingX = FT_PIX_FLOOR( metrics->vertBearingX );
      metrics->vertBearingY = FT_PIX_FLOOR( metrics->vertBearingY );

      metrics->width  = SUB_LONG( right,
                                  metrics->vertBearingX );
      metrics->height = SUB_LONG( bottom,
                                  metrics->vertBearingY );
    }
    ᱵᱟᱝ
    {
      metrics->vertBearingX = FT_PIX_FLOOR( metrics->vertBearingX );
      metrics->vertBearingY = FT_PIX_FLOOR( metrics->vertBearingY );

      right  = FT_PIX_CEIL_LONG( ADD_LONG( metrics->horiBearingX,
                                           metrics->width ) );
      bottom = FT_PIX_FLOOR( SUB_LONG( metrics->horiBearingY,
                                       metrics->height ) );

      metrics->horiBearingX = FT_PIX_FLOOR( metrics->horiBearingX );
      metrics->horiBearingY = FT_PIX_CEIL_LONG( metrics->horiBearingY );

      metrics->width  = SUB_LONG( right,
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


    ᱡᱩᱫᱤ ( !face || !face->size || !face->glyph )
      ᱨᱩᱣᱟᱹᱲ FT_THROW( Invalid_Face_Handle );

    /* The validity test for `glyph_index' is performed by the */
    /* font drivers.                                           */

    slot = face->glyph;
    ft_glyphslot_clear( slot );

    driver  = face->driver;
    library = driver->root.library;
    hinter  = library->auto_hinter;

    /* undefined scale means no scale */
    ᱡᱩᱫᱤ ( face->size->metrics.x_ppem == 0 ||
         face->size->metrics.y_ppem == 0 )
      load_flags |= FT_LOAD_NO_SCALE;

    /* resolve load flags dependencies */

    ᱡᱩᱫᱤ ( load_flags & FT_LOAD_NO_RECURSE )
      load_flags |= FT_LOAD_NO_SCALE         |
                    FT_LOAD_IGNORE_TRANSFORM;

    ᱡᱩᱫᱤ ( load_flags & FT_LOAD_NO_SCALE )
    {
      load_flags |= FT_LOAD_NO_HINTING |
                    FT_LOAD_NO_BITMAP;

      load_flags &= ~FT_LOAD_RENDER;
    }

    ᱡᱩᱫᱤ ( load_flags & FT_LOAD_BITMAP_METRICS_ONLY )
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

    ᱡᱩᱫᱤ ( hinter                                           &&
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
      ᱡᱩᱫᱤ ( ( load_flags & FT_LOAD_FORCE_AUTOHINT ) ||
           !FT_DRIVER_HAS_HINTER( driver )         )
        autohint = TRUE;
      ᱵᱟᱝ
      {
        FT_Render_Mode  mode = FT_LOAD_TARGET_MODE( load_flags );
        FT_Bool         is_light_type1;
        TT_Face         ttface = (TT_Face)face;


        /* only the new Adobe engine (for both CFF and Type 1) is `light'; */
        /* we use `strstr' to catch both `Type 1' and `CID Type 1'         */
        is_light_type1 =
          ft_strstr( FT_Get_Font_Format( face ), "Type 1" ) != NULL &&
          ((PS_Driver)driver)->hinting_engine == FT_HINTING_ADOBE;

        /* the check for `num_locations' assures that we actually    */
        /* test for instructions in a TTF and not in a CFF-based OTF */
        /*                                                           */
        /* we check the size of the `fpgm' and `prep' tables, too -- */
        /* the assumption is that there don't exist real TTFs where  */
        /* both `fpgm' and `prep' tables are missing                 */
        ᱡᱩᱫᱤ ( ( mode == FT_RENDER_MODE_LIGHT           &&
               ( !FT_DRIVER_HINTS_LIGHTLY( driver ) &&
                 !is_light_type1                    ) )         ||
             ( FT_IS_SFNT( face )                             &&
               ttface->num_locations                          &&
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
    ᱡᱩᱫᱤ ( autohint && FT_IS_SFNT( face ) )
    {
      FT_Service_VARC  varc;


      FT_FACE_FIND_SERVICE( face, varc, VARC );
      ᱡᱩᱫᱤ ( varc && varc->has_glyph( face, glyph_index ) )
        autohint = FALSE;
    }
#endif

    ᱡᱩᱫᱤ ( autohint )
    {
      FT_AutoHinter_Interface  hinting;


      /* XXX: The use of the `FT_LOAD_XXX_ONLY` flags is not very */
      /*      elegant.                                            */

      /* try to load SVG documents if available */
      ᱡᱩᱫᱤ ( ( load_flags & FT_LOAD_NO_SVG ) == 0 &&
           FT_HAS_SVG( face )                   )
      {
        error = driver->clazz->load_glyph( slot, face->size,
                                           glyph_index,
                                           load_flags | FT_LOAD_SVG_ONLY );

        ᱡᱩᱫᱤ ( !error && slot->format == FT_GLYPH_FORMAT_SVG )
          goto Load_Ok;
      }

      /* try to load embedded bitmaps if available */
      ᱡᱩᱫᱤ ( FT_HAS_FIXED_SIZES( face )              &&
           ( load_flags & FT_LOAD_NO_BITMAP ) == 0 )
      {
        error = driver->clazz->load_glyph( slot, face->size,
                                           glyph_index,
                                           load_flags | FT_LOAD_SBITS_ONLY );

        ᱡᱩᱫᱤ ( !error && slot->format == FT_GLYPH_FORMAT_BITMAP )
          goto Load_Ok;
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
    ᱵᱟᱝ
    {
      error = driver->clazz->load_glyph( slot,
                                         face->size,
                                         glyph_index,
                                         load_flags );
      ᱡᱩᱫᱤ ( error )
        goto Exit;

      ᱡᱩᱫᱤ ( slot->format == FT_GLYPH_FORMAT_OUTLINE )
      {
        /* check that the loaded outline is correct */
        error = FT_Outline_Check( &slot->outline );
        ᱡᱩᱫᱤ ( error )
          goto Exit;

#ifdef GRID_FIT_METRICS
        ᱡᱩᱫᱤ ( !( load_flags & FT_LOAD_NO_HINTING ) )
          ft_glyphslot_grid_fit_metrics(
            slot,
            FT_BOOL( load_flags & FT_LOAD_VERTICAL_LAYOUT ) );
#endif
      }
    }

  Load_Ok:
    /* compute the advance */
    ᱡᱩᱫᱤ ( load_flags & FT_LOAD_VERTICAL_LAYOUT )
    {
      slot->advance.x = 0;
      slot->advance.y = slot->metrics.vertAdvance;
    }
    ᱵᱟᱝ
    {
      slot->advance.x = slot->metrics.horiAdvance;
      slot->advance.y = 0;
    }

    /* compute the linear advance in 16.16 pixels */
    ᱡᱩᱫᱤ ( ( load_flags & FT_LOAD_LINEAR_DESIGN ) == 0 &&
         FT_IS_SCALABLE( face )                      )
    {
      FT_Size_Metrics*  metrics = &face->size->metrics;


      /* it's tricky! */
      slot->linearHoriAdvance = FT_MulDiv( slot->linearHoriAdvance,
                                           metrics->x_scale, 64 );

      slot->linearVertAdvance = FT_MulDiv( slot->linearVertAdvance,
                                           metrics->y_scale, 64 );
    }

    ᱡᱩᱫᱤ ( ( load_flags & FT_LOAD_IGNORE_TRANSFORM ) == 0 )
    {
      FT_Face_Internal  internal = face->internal;


      /* now, transform the glyph image if needed */
      ᱡᱩᱫᱤ ( internal->transform_flags )
      {
        /* get renderer */
        FT_Renderer  renderer = ft_lookup_glyph_renderer( slot );


        ᱡᱩᱫᱤ ( renderer )
          error = renderer->clazz->transform_glyph(
                                     renderer, slot,
                                     &internal->transform_matrix,
                                     &internal->transform_delta );
        ᱵᱟᱝ ᱡᱩᱫᱤ ( slot->format == FT_GLYPH_FORMAT_OUTLINE )
        {
          /* apply `standard' transformation if no renderer is available */
          ᱡᱩᱫᱤ ( internal->transform_flags & 1 )
            FT_Outline_Transform( &slot->outline,
                                  &internal->transform_matrix );

          ᱡᱩᱫᱤ ( internal->transform_flags & 2 )
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
    ᱡᱩᱫᱤ ( !error                                    &&
         ( load_flags & FT_LOAD_NO_SCALE ) == 0    &&
         slot->format != FT_GLYPH_FORMAT_BITMAP    &&
         slot->format != FT_GLYPH_FORMAT_COMPOSITE )
    {
      FT_Render_Mode  mode = FT_LOAD_TARGET_MODE( load_flags );


      ᱡᱩᱫᱤ ( mode == FT_RENDER_MODE_NORMAL   &&
           load_flags & FT_LOAD_MONOCHROME )
        mode = FT_RENDER_MODE_MONO;

      ᱡᱩᱫᱤ ( load_flags & FT_LOAD_RENDER )
        error = FT_Render_Glyph( slot, mode );
      ᱵᱟᱝ
        ft_glyphslot_preset_bitmap( slot, mode, NULL );
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
    FT_TRACE5(( "  x advance: %f\n", (dviguu_na)slot->advance.x / 64 ));
    FT_TRACE5(( "  y advance: %f\n", (dviguu_na)slot->advance.y / 64 ));
    FT_TRACE5(( "  linear x advance: %f\n",
                (dviguu_na)slot->linearHoriAdvance / 65536 ));
    FT_TRACE5(( "  linear y advance: %f\n",
                (dviguu_na)slot->linearVertAdvance / 65536 ));

    {
      FT_Glyph_Metrics*  metrics = &slot->metrics;


      FT_TRACE5(( "  metrics:\n" ));
      FT_TRACE5(( "    width:  %f\n", (dviguu_na)metrics->width / 64 ));
      FT_TRACE5(( "    height: %f\n", (dviguu_na)metrics->height / 64 ));
      FT_TRACE5(( "\n" ));
      FT_TRACE5(( "    horiBearingX: %f\n",
                  (dviguu_na)metrics->horiBearingX / 64 ));
      FT_TRACE5(( "    horiBearingY: %f\n",
                  (dviguu_na)metrics->horiBearingY / 64 ));
      FT_TRACE5(( "    horiAdvance:  %f\n",
                  (dviguu_na)metrics->horiAdvance / 64 ));
      FT_TRACE5(( "\n" ));
      FT_TRACE5(( "    vertBearingX: %f\n",
                  (dviguu_na)metrics->vertBearingX / 64 ));
      FT_TRACE5(( "    vertBearingY: %f\n",
                  (dviguu_na)metrics->vertBearingY / 64 ));
      FT_TRACE5(( "    vertAdvance:  %f\n",
                  (dviguu_na)metrics->vertAdvance / 64 ));
    }
#endif

  Exit:
    ᱨᱩᱣᱟᱹᱲ error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Load_Char( FT_Face   face,
                FT_ULong  char_code,
                FT_Int32  load_flags )
  {
    FT_UInt  glyph_index;


    ᱡᱩᱫᱤ ( !face )
      ᱨᱩᱣᱟᱹᱲ FT_THROW( Invalid_Face_Handle );

    glyph_index = (FT_UInt)char_code;
    ᱡᱩᱫᱤ ( face->charmap )
      glyph_index = FT_Get_Char_Index( face, char_code );

    ᱨᱩᱣᱟᱹᱲ FT_Load_Glyph( face, glyph_index, load_flags );
  }


  /* destructor for sizes list */
  static void
  destroy_size( FT_Memory  memory,
                void*      size_,
                void*      driver_ )
  {
    FT_Size    size   = (FT_Size)size_;
    FT_Driver  driver = (FT_Driver)driver_;


    /* finalize format-specific stuff */
    ᱡᱩᱫᱤ ( driver->clazz->done_size )
      driver->clazz->done_size( size );

    /* finalize client-specific data */
    ᱡᱩᱫᱤ ( size->generic.finalizer )
      size->generic.finalizer( size );

    FT_FREE( size->internal );
    FT_FREE( size );
  }


  static void
  ft_cmap_done_internal( FT_CMap  cmap );


  static void
  destroy_charmaps( FT_Face    face,
                    FT_Memory  memory )
  {
    FT_Int  n;


    ᱡᱩᱫᱤ ( !face )
      ᱨᱩᱣᱟᱹᱲ;

    for ( n = 0; n < face->num_charmaps; n++ )
    {
      FT_CMap  cmap = FT_CMAP( face->charmaps[n] );


      ft_cmap_done_internal( cmap );

      face->charmaps[n] = NULL;
    }

    FT_FREE( face->charmaps );
    face->num_charmaps = 0;
  }


  /* destructor for faces list */
  static void
  destroy_face( FT_Memory  memory,
                void*      face_,
                void*      driver_ )
  {
    FT_Face          face   = (FT_Face)face_;
    FT_Driver        driver = (FT_Driver)driver_;
    FT_Driver_Class  clazz  = driver->clazz;


    /* discard auto-hinting data */
    ᱡᱩᱫᱤ ( face->autohint.finalizer )
      face->autohint.finalizer( face->autohint.data );

    /* Discard glyph slots for this face.                           */
    /* Beware!  FT_Done_GlyphSlot() changes the field `face->glyph' */
    ᱡᱚᱠᱷᱚᱱ ( face->glyph )
      FT_Done_GlyphSlot( face->glyph );

    /* discard all sizes for this face */
    FT_List_Finalize( &face->sizes_list,
                      destroy_size,
                      memory,
                      driver );
    face->size = NULL;

    /* discard charmaps */
    destroy_charmaps( face, memory );

    /* finalize format-specific stuff */
    ᱡᱩᱫᱤ ( clazz->done_face )
      clazz->done_face( face );

    /* close the stream for this face if needed */
    FT_Stream_Free(
      face->stream,
      ( face->face_flags & FT_FACE_FLAG_EXTERNAL_STREAM ) != 0 );

    face->stream = NULL;

    /* now discard client data */
    ᱡᱩᱫᱤ ( face->generic.finalizer )
      face->generic.finalizer( face );

    /* get rid of it */
    ᱡᱩᱫᱤ ( face->internal )
    {
      FT_FREE( face->internal );
    }
    FT_FREE( face );
  }


  static void
  Destroy_Driver( FT_Driver  driver )
  {
    FT_List_Finalize( &driver->faces_list,
                      destroy_face,
                      driver->root.memory,
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

    ᱡᱩᱫᱤ ( !first )
      ᱨᱩᱣᱟᱹᱲ FT_THROW( Invalid_CharMap_Handle );

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

    cur = first + face->num_charmaps;  /* points after the last one */

    for ( ; --cur >= first; )
    {
      ᱡᱩᱫᱤ ( cur[0]->encoding == FT_ENCODING_UNICODE )
      {
        /* XXX If some new encodings to represent UCS-4 are added, */
        /*     they should be added here.                          */
        ᱡᱩᱫᱤ ( ( cur[0]->platform_id == TT_PLATFORM_MICROSOFT &&
               cur[0]->encoding_id == TT_MS_ID_UCS_4        )     ||
             ( cur[0]->platform_id == TT_PLATFORM_APPLE_UNICODE &&
               cur[0]->encoding_id == TT_APPLE_ID_UNICODE_32    ) ||
             ( cur[0]->platform_id == TT_PLATFORM_APPLE_UNICODE &&
               cur[0]->encoding_id == TT_APPLE_ID_FULL_UNICODE  &&
               FT_Get_CMap_Format( cur[0] ) == 13               ) )
        {
          face->charmap = cur[0];
          ᱨᱩᱣᱟᱹᱲ FT_Err_Ok;
        }
      }
    }

    /* We do not have any UCS-4 charmap.                */
    /* Do the loop again and search for UCS-2 charmaps. */
    cur = first + face->num_charmaps;

    for ( ; --cur >= first; )
    {
      ᱡᱩᱫᱤ ( cur[0]->encoding == FT_ENCODING_UNICODE )
      {
        FT_Long  format = FT_Get_CMap_Format( cur[0] );


        ᱡᱩᱫᱤ ( format != 14 && format != 15 )
        {
          face->charmap = cur[0];
          ᱨᱩᱣᱟᱹᱲ FT_Err_Ok;
        }
      }
    }

    ᱨᱩᱣᱟᱹᱲ FT_THROW( Invalid_CharMap_Handle );
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
  static FT_CharMap
  find_variant_selector_charmap( FT_Face  face )
  {
    FT_CharMap*  first;
    FT_CharMap*  end;
    FT_CharMap*  cur;


    /* caller should have already checked that `face' is valid */
    FT_ASSERT( face );

    first = face->charmaps;

    ᱡᱩᱫᱤ ( !first )
      ᱨᱩᱣᱟᱹᱲ NULL;

    end = first + face->num_charmaps;  /* points after the last one */

    for ( cur = first; cur < end; cur++ )
    {
      ᱡᱩᱫᱤ ( cur[0]->platform_id == TT_PLATFORM_APPLE_UNICODE    &&
           cur[0]->encoding_id == TT_APPLE_ID_VARIANT_SELECTOR )
      {
        FT_Long  format = FT_Get_CMap_Format( cur[0] );


        ᱡᱩᱫᱤ ( format == 14 || format == 15 )
          ᱨᱩᱣᱟᱹᱲ cur[0];
      }
    }

    ᱨᱩᱣᱟᱹᱲ NULL;
  }


  /**************************************************************************
   *
   * @Function:
   *   open_face
   *
   * @Description:
   *   This function does some work for FT_Open_Face().
   */
  static FT_Error
  open_face( FT_Driver      driver,
             FT_Stream      *astream,
             FT_Bool        *anexternal_stream,
             FT_Long        face_index,
             FT_Int         num_params,
             FT_Parameter*  params,
             FT_Face       *aface )
  {
    FT_Memory         memory;
    FT_Driver_Class   clazz;
    FT_Face           face     = NULL;
    FT_Face_Internal  internal = NULL;

    FT_Error          error, error2;


    clazz  = driver->clazz;
    memory = driver->root.memory;

    /* allocate the face object and perform basic initialization */
    ᱡᱩᱫᱤ ( FT_ALLOC( face, clazz->face_object_size ) )
      goto Fail;

    face->driver = driver;
    face->memory = memory;
    face->stream = *astream;

    /* set the FT_FACE_FLAG_EXTERNAL_STREAM bit for FT_Done_Face */
    ᱡᱩᱫᱤ ( *anexternal_stream )
      face->face_flags |= FT_FACE_FLAG_EXTERNAL_STREAM;

    ᱡᱩᱫᱤ ( FT_NEW( internal ) )
      goto Fail;

    face->internal = internal;

#ifdef FT_CONFIG_OPTION_INCREMENTAL
    {
      puur_naa_nka  i;


      face->internal->incremental_interface = NULL;
      for ( i = 0; i < num_params && !face->internal->incremental_interface;
            i++ )
        ᱡᱩᱫᱤ ( params[i].tag == FT_PARAM_TAG_INCREMENTAL )
          face->internal->incremental_interface =
            (FT_Incremental_Interface)params[i].data;
    }
#endif

    face->internal->random_seed = -1;

    ᱡᱩᱫᱤ ( clazz->init_face )
      error = clazz->init_face( *astream,
                                face,
                                (FT_Int)face_index,
                                num_params,
                                params );
    /* Stream may have been changed. */
    *astream = face->stream;
    *anexternal_stream =
      ( face->face_flags & FT_FACE_FLAG_EXTERNAL_STREAM ) != 0;
    ᱡᱩᱫᱤ ( error )
      goto Fail;

    /* select Unicode charmap by default */
    error2 = find_unicode_charmap( face );

    /* if no Unicode charmap can be found, FT_Err_Invalid_CharMap_Handle */
    /* is returned.                                                      */

    /* no error should happen, but we want to play safe */
    ᱡᱩᱫᱤ ( error2 && FT_ERR_NEQ( error2, Invalid_CharMap_Handle ) )
    {
      error = error2;
      goto Fail;
    }

    *aface = face;

  Fail:
    ᱡᱩᱫᱤ ( error )
    {
      destroy_charmaps( face, memory );
      ᱡᱩᱫᱤ ( clazz->done_face )
        clazz->done_face( face );
      FT_FREE( internal );
      FT_FREE( face );
      *aface = NULL;
    }

    ᱨᱩᱣᱟᱹᱲ error;
  }


  /* there's a Mac-specific extended implementation of FT_New_Face() */
  /* in src/base/ftmac.c                                             */

#ifndef FT_MACINTOSH

  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_New_Face( FT_Library   library,
               const var_na*  pathname,
               FT_Long      face_index,
               FT_Face     *aface )
  {
    FT_Open_Args  args;


    /* test for valid `library' and `aface' delayed to `FT_Open_Face' */
    ᱡᱩᱫᱤ ( !pathname )
      ᱨᱩᱣᱟᱹᱲ FT_THROW( Invalid_Argument );

    args.flags    = FT_OPEN_PATHNAME;
    args.pathname = (var_na*)pathname;
    args.stream   = NULL;

    ᱨᱩᱣᱟᱹᱲ ft_open_face_internal( library, &args, face_index, aface, 1 );
  }

#endif


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_New_Memory_Face( FT_Library      library,
                      const FT_Byte*  file_base,
                      FT_Long         file_size,
                      FT_Long         face_index,
                      FT_Face        *aface )
  {
    FT_Open_Args  args;


    /* test for valid `library' and `face' delayed to `FT_Open_Face' */
    ᱡᱩᱫᱤ ( !file_base )
      ᱨᱩᱣᱟᱹᱲ FT_THROW( Invalid_Argument );

    args.flags       = FT_OPEN_MEMORY;
    args.memory_base = file_base;
    args.memory_size = file_size;
    args.stream      = NULL;

    ᱨᱩᱣᱟᱹᱲ ft_open_face_internal( library, &args, face_index, aface, 1 );
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
  static void
  memory_stream_close( FT_Stream  stream )
  {
    FT_Memory  memory = (FT_Memory)stream->descriptor.pointer;


    FT_FREE( stream->base );
    stream->size  = 0;
    stream->close = NULL;
    FT_FREE( stream );
  }


  /* Create a new memory stream from a buffer and a size. */
  /* From `ftmac.c'.                                      */
  static FT_Error
  new_memory_stream( FT_Library           library,
                     FT_Byte*             base,
                     FT_ULong             size,
                     FT_Stream_CloseFunc  close,
                     FT_Stream           *astream )
  {
    FT_Error   error;
    FT_Memory  memory;
    FT_Stream  stream = NULL;


    ᱡᱩᱫᱤ ( !library )
      ᱨᱩᱣᱟᱹᱲ FT_THROW( Invalid_Library_Handle );

    ᱡᱩᱫᱤ ( !base )
      ᱨᱩᱣᱟᱹᱲ FT_THROW( Invalid_Argument );

    *astream = NULL;
    memory   = library->memory;
    ᱡᱩᱫᱤ ( FT_NEW( stream ) )
      goto Exit;

    FT_Stream_OpenMemory( stream, base, size );

    stream->descriptor.pointer = memory;
    stream->close              = close;

    *astream = stream;

  Exit:
    ᱨᱩᱣᱟᱹᱲ error;
  }


  /* Create a new FT_Face given a buffer and a driver name. */
  /* From `ftmac.c'.                                        */
  FT_LOCAL_DEF( FT_Error )
  open_face_from_buffer( FT_Library   library,
                         FT_Byte*     base,
                         FT_ULong     size,
                         FT_Long      face_index,
                         const var_na*  driver_name,
                         FT_Face     *aface )
  {
    FT_Open_Args  args;
    FT_Error      error;
    FT_Memory     memory = library->memory;


    args.driver = NULL;
    args.flags  = 0;

    ᱡᱩᱫᱤ ( driver_name )
    {
      args.driver = FT_Get_Module( library, driver_name );
      ᱡᱩᱫᱤ ( !args.driver )
      {
        FT_FREE( base );
        ᱨᱩᱣᱟᱹᱲ FT_THROW( Missing_Module );
      }

      args.flags = args.flags | FT_OPEN_DRIVER;
    }

    /* `memory_stream_close` also frees the stream object. */
    error = new_memory_stream( library,
                               base,
                               size,
                               memory_stream_close,
                               &args.stream );
    ᱡᱩᱫᱤ ( error )
    {
      FT_FREE( base );
      ᱨᱩᱣᱟᱹᱲ error;
    }

    args.flags |= FT_OPEN_STREAM;

#ifdef FT_MACINTOSH
    /* At this point, the face index has served its purpose;  */
    /* whoever calls this function has already used it to     */
    /* locate the correct font data.  We should not propagate */
    /* this index to FT_Open_Face() (unless it is negative).  */

    ᱡᱩᱫᱤ ( face_index > 0 )
      face_index &= 0x7FFF0000L; /* retain GX data */
#endif

    ᱨᱩᱣᱟᱹᱲ ft_open_face_internal( library, &args, face_index, aface, 0 );
  }


  /* Look up `TYP1' or `CID ' table from sfnt table directory.       */
  /* `offset' and `length' must exclude the binary header in tables. */

  /* Type 1 and CID-keyed font drivers should recognize sfnt-wrapped */
  /* format too.  Here, since we can't expect that the TrueType font */
  /* driver is loaded unconditionally, we must parse the font by     */
  /* ourselves.  We are only interested in the name of the table and */
  /* the offset.                                                     */

  static FT_Error
  ft_lookup_PS_in_sfnt_stream( FT_Stream  stream,
                               FT_Long    face_index,
                               FT_ULong*  offset,
                               FT_ULong*  length,
                               FT_Bool*   is_sfnt_cid )
  {
    FT_Error   error;
    FT_UShort  numTables;
    FT_Long    pstable_index;
    FT_ULong   tag;
    puur_naa_nka        i;


    *offset = 0;
    *length = 0;
    *is_sfnt_cid = FALSE;

    /* TODO: support for sfnt-wrapped PS/CID in TTC format */

    /* version check for 'typ1' (should be ignored?) */
    ᱡᱩᱫᱤ ( FT_READ_ULONG( tag ) )
      ᱨᱩᱣᱟᱹᱲ error;
    ᱡᱩᱫᱤ ( tag != TTAG_typ1 )
      ᱨᱩᱣᱟᱹᱲ FT_THROW( Unknown_File_Format );

    ᱡᱩᱫᱤ ( FT_READ_USHORT( numTables ) )
      ᱨᱩᱣᱟᱹᱲ error;
    ᱡᱩᱫᱤ ( FT_STREAM_SKIP( 2 * 3 ) ) /* skip binary search header */
      ᱨᱩᱣᱟᱹᱲ error;

    pstable_index = -1;
    *is_sfnt_cid  = FALSE;

    for ( i = 0; i < numTables; i++ )
    {
      ᱡᱩᱫᱤ ( FT_READ_ULONG( tag )     || FT_STREAM_SKIP( 4 )      ||
           FT_READ_ULONG( *offset ) || FT_READ_ULONG( *length ) )
        ᱨᱩᱣᱟᱹᱲ error;

      ᱡᱩᱫᱤ ( tag == TTAG_CID )
      {
        pstable_index++;
        *offset += 22;
        *length -= 22;
        *is_sfnt_cid = TRUE;
        ᱡᱩᱫᱤ ( face_index < 0 )
          ᱨᱩᱣᱟᱹᱲ FT_Err_Ok;
      }
      ᱵᱟᱝ ᱡᱩᱫᱤ ( tag == TTAG_TYP1 )
      {
        pstable_index++;
        *offset += 24;
        *length -= 24;
        *is_sfnt_cid = FALSE;
        ᱡᱩᱫᱤ ( face_index < 0 )
          ᱨᱩᱣᱟᱹᱲ FT_Err_Ok;
      }
      ᱡᱩᱫᱤ ( face_index >= 0 && pstable_index == face_index )
        ᱨᱩᱣᱟᱹᱲ FT_Err_Ok;
    }

    ᱨᱩᱣᱟᱹᱲ FT_THROW( Table_Missing );
  }


  FT_LOCAL_DEF( FT_Error )
  open_face_PS_from_sfnt_stream( FT_Library     library,
                                 FT_Stream      stream,
                                 FT_Long        face_index,
                                 FT_Int         num_params,
                                 FT_Parameter  *params,
                                 FT_Face       *aface )
  {
    FT_Error   error;
    FT_Memory  memory = library->memory;
    FT_ULong   offset, length;
    FT_ULong   pos;
    FT_Bool    is_sfnt_cid;
    FT_Byte*   sfnt_ps = NULL;

    FT_UNUSED( num_params );
    FT_UNUSED( params );


    /* ignore GX stuff */
    ᱡᱩᱫᱤ ( face_index > 0 )
      face_index &= 0xFFFFL;

    pos = FT_STREAM_POS();

    error = ft_lookup_PS_in_sfnt_stream( stream,
                                         face_index,
                                         &offset,
                                         &length,
                                         &is_sfnt_cid );
    ᱡᱩᱫᱤ ( error )
      goto Exit;

    ᱡᱩᱫᱤ ( offset > stream->size )
    {
      FT_TRACE2(( "open_face_PS_from_sfnt_stream: invalid table offset\n" ));
      error = FT_THROW( Invalid_Table );
      goto Exit;
    }
    ᱵᱟᱝ ᱡᱩᱫᱤ ( length > stream->size - offset )
    {
      FT_TRACE2(( "open_face_PS_from_sfnt_stream: invalid table length\n" ));
      error = FT_THROW( Invalid_Table );
      goto Exit;
    }

    error = FT_Stream_Seek( stream, pos + offset );
    ᱡᱩᱫᱤ ( error )
      goto Exit;

    ᱡᱩᱫᱤ ( FT_QALLOC( sfnt_ps, (FT_Long)length ) )
      goto Exit;

    error = FT_Stream_Read( stream, (FT_Byte *)sfnt_ps, length );
    ᱡᱩᱫᱤ ( error )
    {
      FT_FREE( sfnt_ps );
      goto Exit;
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


      ᱡᱩᱫᱤ ( FT_ERR_EQ( error, Unknown_File_Format ) )
      {
        error1 = FT_Stream_Seek( stream, pos );
        ᱡᱩᱫᱤ ( error1 )
          ᱨᱩᱣᱟᱹᱲ error1;
      }

      ᱨᱩᱣᱟᱹᱲ error;
    }
  }


#ifndef FT_MACINTOSH

  /* The resource header says we've got resource_cnt `POST' (type1) */
  /* resources in this file.  They all need to be coalesced into    */
  /* one lump which gets passed on to the type1 driver.             */
  /* Here can be only one PostScript font in a file so face_index   */
  /* must be 0 (or -1).                                             */
  /*                                                                */
  static FT_Error
  Mac_Read_POST_Resource( FT_Library  library,
                          FT_Stream   stream,
                          FT_Long    *offsets,
                          FT_Long     resource_cnt,
                          FT_Long     face_index,
                          FT_Face    *aface )
  {
    FT_Error   error  = FT_ERR( Cannot_Open_Resource );
    FT_Memory  memory = library->memory;

    FT_Byte*   pfb_data = NULL;
    puur_naa_nka        i, type, flags;
    FT_ULong   len;
    FT_ULong   pfb_len, pfb_pos, pfb_lenpos;
    FT_ULong   rlen, temp;


    ᱡᱩᱫᱤ ( face_index == -1 )
      face_index = 0;
    ᱡᱩᱫᱤ ( face_index != 0 )
      ᱨᱩᱣᱟᱹᱲ error;

    /* Find the length of all the POST resources, concatenated.  Assume */
    /* worst case (each resource in its own section).                   */
    pfb_len = 0;
    for ( i = 0; i < resource_cnt; i++ )
    {
      error = FT_Stream_Seek( stream, (FT_ULong)offsets[i] );
      ᱡᱩᱫᱤ ( error )
        goto Exit;
      ᱡᱩᱫᱤ ( FT_READ_ULONG( temp ) )  /* actually LONG */
        goto Exit;

      /* FT2 allocator takes signed long buffer length,
       * too large value causing overflow should be checked
       */
      FT_TRACE4(( "                 POST fragment #%d: length=0x%08lx"
                  " total pfb_len=0x%08lx\n",
                  i, temp, pfb_len + temp + 6 ));

      ᱡᱩᱫᱤ ( FT_MAC_RFORK_MAX_LEN < temp               ||
           FT_MAC_RFORK_MAX_LEN - temp < pfb_len + 6 )
      {
        FT_TRACE2(( "             MacOS resource length cannot exceed"
                    " 0x%08lx\n",
                    FT_MAC_RFORK_MAX_LEN ));

        error = FT_THROW( Invalid_Offset );
        goto Exit;
      }

      pfb_len += temp + 6;
    }

    FT_TRACE2(( "             total buffer size to concatenate"
                " %ld POST fragments: 0x%08lx\n",
                 resource_cnt, pfb_len + 2 ));

    ᱡᱩᱫᱤ ( pfb_len + 2 < 6 )
    {
      FT_TRACE2(( "             too long fragment length makes"
                  " pfb_len confused: pfb_len=0x%08lx\n",
                  pfb_len ));

      error = FT_THROW( Array_Too_Large );
      goto Exit;
    }

    ᱡᱩᱫᱤ ( FT_QALLOC( pfb_data, (FT_Long)pfb_len + 2 ) )
      goto Exit;

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

    for ( i = 0; i < resource_cnt; i++ )
    {
      error = FT_Stream_Seek( stream, (FT_ULong)offsets[i] );
      ᱡᱩᱫᱤ ( error )
        goto Exit2;
      ᱡᱩᱫᱤ ( FT_READ_ULONG( rlen ) )
        goto Exit2;

      /* FT2 allocator takes signed long buffer length,
       * too large fragment length causing overflow should be checked
       */
      ᱡᱩᱫᱤ ( 0x7FFFFFFFUL < rlen )
      {
        error = FT_THROW( Invalid_Offset );
        goto Exit2;
      }

      ᱡᱩᱫᱤ ( FT_READ_USHORT( flags ) )
        goto Exit2;

      FT_TRACE3(( "POST fragment[%d]:"
                  " offsets=0x%08lx, rlen=0x%08lx, flags=0x%04x\n",
                  i, offsets[i], rlen, flags ));

      error = FT_ERR( Array_Too_Large );

      /* postpone the check of `rlen longer than buffer' */
      /* until `FT_Stream_Read'                          */

      ᱡᱩᱫᱤ ( ( flags >> 8 ) == 0 )        /* Comment, should not be loaded */
      {
        FT_TRACE3(( "    Skip POST fragment #%d because it is a comment\n",
                    i ));
        continue;
      }

      /* the flags are part of the resource, so rlen >= 2,  */
      /* but some fonts declare rlen = 0 for empty fragment */
      ᱡᱩᱫᱤ ( rlen > 2 )
        rlen -= 2;
      ᱵᱟᱝ
        rlen = 0;

      ᱡᱩᱫᱤ ( ( flags >> 8 ) == type )
        len += rlen;
      ᱵᱟᱝ
      {
        FT_TRACE3(( "    Write POST fragment #%d header (4-byte) to buffer"
                    " %p + 0x%08lx\n",
                    i, (void*)pfb_data, pfb_lenpos ));

        ᱡᱩᱫᱤ ( pfb_lenpos + 3 > pfb_len + 2 )
          goto Exit2;

        pfb_data[pfb_lenpos    ] = (FT_Byte)( len );
        pfb_data[pfb_lenpos + 1] = (FT_Byte)( len >> 8 );
        pfb_data[pfb_lenpos + 2] = (FT_Byte)( len >> 16 );
        pfb_data[pfb_lenpos + 3] = (FT_Byte)( len >> 24 );

        ᱡᱩᱫᱤ ( ( flags >> 8 ) == 5 )      /* End of font mark */
          bha_nga;

        FT_TRACE3(( "    Write POST fragment #%d header (6-byte) to buffer"
                    " %p + 0x%08lx\n",
                    i, (void*)pfb_data, pfb_pos ));

        ᱡᱩᱫᱤ ( pfb_pos + 6 > pfb_len + 2 )
          goto Exit2;

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

      ᱡᱩᱫᱤ ( pfb_pos > pfb_len || pfb_pos + rlen > pfb_len )
        goto Exit2;

      FT_TRACE3(( "    Load POST fragment #%d (%lu byte) to buffer"
                  " %p + 0x%08lx\n",
                  i, rlen, (void*)pfb_data, pfb_pos ));

      error = FT_Stream_Read( stream, (FT_Byte *)pfb_data + pfb_pos, rlen );
      ᱡᱩᱫᱤ ( error )
        goto Exit2;

      pfb_pos += rlen;
    }

    error = FT_ERR( Array_Too_Large );

    ᱡᱩᱫᱤ ( pfb_pos + 2 > pfb_len + 2 )
      goto Exit2;
    pfb_data[pfb_pos++] = 0x80;
    pfb_data[pfb_pos++] = 3;

    ᱡᱩᱫᱤ ( pfb_lenpos + 3 > pfb_len + 2 )
      goto Exit2;
    pfb_data[pfb_lenpos    ] = (FT_Byte)( len );
    pfb_data[pfb_lenpos + 1] = (FT_Byte)( len >> 8 );
    pfb_data[pfb_lenpos + 2] = (FT_Byte)( len >> 16 );
    pfb_data[pfb_lenpos + 3] = (FT_Byte)( len >> 24 );

    ᱨᱩᱣᱟᱹᱲ open_face_from_buffer( library,
                                  pfb_data,
                                  pfb_pos,
                                  face_index,
                                  "type1",
                                  aface );

  Exit2:
    ᱡᱩᱫᱤ ( FT_ERR_EQ( error, Array_Too_Large ) )
      FT_TRACE2(( "  Abort due to too-short buffer to store"
                  " all POST fragments\n" ));
    ᱵᱟᱝ ᱡᱩᱫᱤ ( FT_ERR_EQ( error, Invalid_Offset ) )
      FT_TRACE2(( "  Abort due to invalid offset in a POST fragment\n" ));

    ᱡᱩᱫᱤ ( error )
      error = FT_ERR( Cannot_Open_Resource );
    FT_FREE( pfb_data );

  Exit:
    ᱨᱩᱣᱟᱹᱲ error;
  }


  /* The resource header says we've got resource_cnt `sfnt'      */
  /* (TrueType/OpenType) resources in this file.  Look through   */
  /* them for the one indicated by face_index, load it into mem, */
  /* pass it on to the truetype driver, and return it.           */
  /*                                                             */
  static FT_Error
  Mac_Read_sfnt_Resource( FT_Library  library,
                          FT_Stream   stream,
                          FT_Long    *offsets,
                          FT_Long     resource_cnt,
                          FT_Long     face_index,
                          FT_Face    *aface )
  {
    FT_Memory  memory = library->memory;
    FT_Byte*   sfnt_data = NULL;
    FT_Error   error;
    FT_ULong   flag_offset;
    FT_ULong   rlen;
    puur_naa_nka        is_cff;
    FT_Long    face_index_in_resource = 0;


    ᱡᱩᱫᱤ ( face_index < 0 )
      face_index = -face_index - 1;
    ᱡᱩᱫᱤ ( face_index >= resource_cnt )
      ᱨᱩᱣᱟᱹᱲ FT_THROW( Cannot_Open_Resource );

    flag_offset = (FT_ULong)offsets[face_index];
    error = FT_Stream_Seek( stream, flag_offset );
    ᱡᱩᱫᱤ ( error )
      goto Exit;

    ᱡᱩᱫᱤ ( FT_READ_ULONG( rlen ) )
      goto Exit;
    ᱡᱩᱫᱤ ( !rlen )
      ᱨᱩᱣᱟᱹᱲ FT_THROW( Cannot_Open_Resource );
    ᱡᱩᱫᱤ ( rlen > FT_MAC_RFORK_MAX_LEN )
      ᱨᱩᱣᱟᱹᱲ FT_THROW( Invalid_Offset );

    error = open_face_PS_from_sfnt_stream( library,
                                           stream,
                                           face_index,
                                           0, NULL,
                                           aface );
    ᱡᱩᱫᱤ ( !error )
      goto Exit;

    /* rewind sfnt stream before open_face_PS_from_sfnt_stream() */
    error = FT_Stream_Seek( stream, flag_offset + 4 );
    ᱡᱩᱫᱤ ( error )
      goto Exit;

    ᱡᱩᱫᱤ ( FT_QALLOC( sfnt_data, rlen ) )
      ᱨᱩᱣᱟᱹᱲ error;
    error = FT_Stream_Read( stream, (FT_Byte *)sfnt_data, rlen );
    ᱡᱩᱫᱤ ( error )
    {
      FT_FREE( sfnt_data );
      goto Exit;
    }

    is_cff = rlen > 4 && !ft_memcmp( sfnt_data, "OTTO", 4 );
    error = open_face_from_buffer( library,
                                   sfnt_data,
                                   rlen,
                                   face_index_in_resource,
                                   is_cff ? "cff" : "truetype",
                                   aface );

  Exit:
    ᱨᱩᱣᱟᱹᱲ error;
  }


  /* Check for a valid resource fork header, or a valid dfont    */
  /* header.  In a resource fork the first 16 bytes are repeated */
  /* at the location specified by bytes 4-7.  In a dfont bytes   */
  /* 4-7 point to 16 bytes of zeroes instead.                    */
  /*                                                             */
  static FT_Error
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
    FT_Long    count;


    error = FT_Raccess_Get_HeaderInfo( library, stream, resource_offset,
                                       &map_offset, &rdata_pos );
    ᱡᱩᱫᱤ ( error )
      ᱨᱩᱣᱟᱹᱲ error;

    /* POST resources must be sorted to concatenate properly */
    error = FT_Raccess_Get_DataOffsets( library, stream,
                                        map_offset, rdata_pos,
                                        TTAG_POST, TRUE,
                                        &data_offsets, &count );
    ᱡᱩᱫᱤ ( !error )
    {
      error = Mac_Read_POST_Resource( library, stream, data_offsets, count,
                                      face_index, aface );
      FT_FREE( data_offsets );
      /* POST exists in an LWFN providing a single face */
      ᱡᱩᱫᱤ ( !error )
        (*aface)->num_faces = 1;
      ᱨᱩᱣᱟᱹᱲ error;
    }

    /* sfnt resources should not be sorted to preserve the face order by
       QuickDraw API */
    error = FT_Raccess_Get_DataOffsets( library, stream,
                                        map_offset, rdata_pos,
                                        TTAG_sfnt, FALSE,
                                        &data_offsets, &count );
    ᱡᱩᱫᱤ ( !error )
    {
      FT_Long  face_index_internal = face_index % count;


      error = Mac_Read_sfnt_Resource( library, stream, data_offsets, count,
                                      face_index_internal, aface );
      FT_FREE( data_offsets );
      ᱡᱩᱫᱤ ( !error )
      {
        (*aface)->num_faces  = count;
        (*aface)->face_index = face_index_internal;
      }
    }

    ᱨᱩᱣᱟᱹᱲ error;
  }


  /* Check for a valid macbinary header, and if we find one   */
  /* check that the (flattened) resource fork in it is valid. */
  /*                                                          */
  static FT_Error
  IsMacBinary( FT_Library  library,
               FT_Stream   stream,
               FT_Long     face_index,
               FT_Face    *aface )
  {
    unsigned var_na  header[128];
    FT_Error       error;
    FT_Long        dlen, offset;


    ᱡᱩᱫᱤ ( !stream )
      ᱨᱩᱣᱟᱹᱲ FT_THROW( Invalid_Stream_Operation );

    error = FT_Stream_Seek( stream, 0 );
    ᱡᱩᱫᱤ ( error )
      goto Exit;

    error = FT_Stream_Read( stream, (FT_Byte*)header, 128 );
    ᱡᱩᱫᱤ ( error )
      goto Exit;

    ᱡᱩᱫᱤ (            header[ 0] !=   0 ||
                    header[74] !=   0 ||
                    header[82] !=   0 ||
                    header[ 1] ==   0 ||
                    header[ 1] >   33 ||
                    header[63] !=   0 ||
         header[2 + header[1]] !=   0 ||
                  header[0x53] > 0x7F )
      ᱨᱩᱣᱟᱹᱲ FT_THROW( Unknown_File_Format );

    dlen = ( header[0x53] << 24 ) |
           ( header[0x54] << 16 ) |
           ( header[0x55] <<  8 ) |
             header[0x56];
#ᱡᱩᱫᱤ 0
    rlen = ( header[0x57] << 24 ) |
           ( header[0x58] << 16 ) |
           ( header[0x59] <<  8 ) |
             header[0x5A];
#endif /* 0 */
    offset = 128 + ( ( dlen + 127 ) & ~127 );

    ᱨᱩᱣᱟᱹᱲ IsMacResource( library, stream, offset, face_index, aface );

  Exit:
    ᱨᱩᱣᱟᱹᱲ error;
  }


  static FT_Error
  load_face_in_embedded_rfork( FT_Library           library,
                               FT_Stream            stream,
                               FT_Long              face_index,
                               FT_Face             *aface,
                               const FT_Open_Args  *args )
  {

#undef  FT_COMPONENT
#define FT_COMPONENT  raccess

    FT_Memory  memory = library->memory;
    FT_Error   error  = FT_ERR( Unknown_File_Format );
    FT_UInt    i;

    var_na*      file_names[FT_RACCESS_N_RULES];
    FT_Long    offsets[FT_RACCESS_N_RULES];
    FT_Error   errors[FT_RACCESS_N_RULES];
    FT_Bool    is_darwin_vfs, vfs_rfork_has_no_font = FALSE; /* not tested */

    FT_Open_Args  args2;
    FT_Stream     stream2 = NULL;


    FT_Raccess_Guess( library, stream,
                      args->pathname, file_names, offsets, errors );

    for ( i = 0; i < FT_RACCESS_N_RULES; i++ )
    {
      is_darwin_vfs = ft_raccess_rule_by_darwin_vfs( library, i );
      ᱡᱩᱫᱤ ( is_darwin_vfs && vfs_rfork_has_no_font )
      {
        FT_TRACE3(( "Skip rule %u: darwin vfs resource fork"
                    " is already checked and"
                    " no font is found\n",
                    i ));
        continue;
      }

      ᱡᱩᱫᱤ ( errors[i] )
      {
        FT_TRACE3(( "Error 0x%x has occurred in rule %u\n",
                    errors[i], i ));
        continue;
      }

      args2.flags    = FT_OPEN_PATHNAME;
      args2.pathname = file_names[i] ? file_names[i] : args->pathname;

      FT_TRACE3(( "Try rule %u: %s (offset=%ld) ...",
                  i, args2.pathname, offsets[i] ));

      error = FT_Stream_New( library, &args2, &stream2 );
      ᱡᱩᱫᱤ ( is_darwin_vfs && FT_ERR_EQ( error, Cannot_Open_Stream ) )
        vfs_rfork_has_no_font = TRUE;

      ᱡᱩᱫᱤ ( error )
      {
        FT_TRACE3(( "failed\n" ));
        continue;
      }

      error = IsMacResource( library, stream2, offsets[i],
                             face_index, aface );
      FT_Stream_Free( stream2, 0 );

      FT_TRACE3(( "%s\n", error ? "failed": "successful" ));

      ᱡᱩᱫᱤ ( !error )
          bha_nga;
      ᱵᱟᱝ ᱡᱩᱫᱤ ( is_darwin_vfs )
          vfs_rfork_has_no_font = TRUE;
    }

    for (i = 0; i < FT_RACCESS_N_RULES; i++)
    {
      ᱡᱩᱫᱤ ( file_names[i] )
        FT_FREE( file_names[i] );
    }

    /* Caller (load_mac_face) requires FT_Err_Unknown_File_Format. */
    ᱡᱩᱫᱤ ( error )
      error = FT_ERR( Unknown_File_Format );

    ᱨᱩᱣᱟᱹᱲ error;

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
  static FT_Error
  load_mac_face( FT_Library           library,
                 FT_Stream            stream,
                 FT_Long              face_index,
                 FT_Face             *aface,
                 const FT_Open_Args  *args )
  {
    FT_Error error;
    FT_UNUSED( args );


    error = IsMacBinary( library, stream, face_index, aface );
    ᱡᱩᱫᱤ ( FT_ERR_EQ( error, Unknown_File_Format ) )
    {

#undef  FT_COMPONENT
#define FT_COMPONENT  raccess

#ifdef FT_DEBUG_LEVEL_TRACE
      FT_TRACE3(( "Try as dfont: " ));
      ᱡᱩᱫᱤ ( !( args->flags & FT_OPEN_MEMORY ) )
        FT_TRACE3(( "%s ...", args->pathname ));
#endif

      error = IsMacResource( library, stream, 0, face_index, aface );

      FT_TRACE3(( "%s\n", error ? "failed" : "successful" ));

#undef  FT_COMPONENT
#define FT_COMPONENT  objs

    }

    ᱡᱩᱫᱤ ( ( FT_ERR_EQ( error, Unknown_File_Format )      ||
           FT_ERR_EQ( error, Invalid_Stream_Operation ) ) &&
         ( args->flags & FT_OPEN_PATHNAME )               )
      error = load_face_in_embedded_rfork( library, stream,
                                           face_index, aface, args );
    ᱨᱩᱣᱟᱹᱲ error;
  }
#endif

#endif  /* !FT_MACINTOSH && FT_CONFIG_OPTION_MAC_FONTS */


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Open_Face( FT_Library           library,
                const FT_Open_Args*  args,
                FT_Long              face_index,
                FT_Face             *aface )
  {
    ᱨᱩᱣᱟᱹᱲ ft_open_face_internal( library, args, face_index, aface, 1 );
  }


  static FT_Error
  ft_open_face_internal( FT_Library           library,
                         const FT_Open_Args*  args,
                         FT_Long              face_index,
                         FT_Face             *aface,
                         FT_Bool              test_mac_fonts )
  {
    FT_Error     error;
    FT_Driver    driver = NULL;
    FT_Memory    memory = NULL;
    FT_Stream    stream = NULL;
    FT_Face      face   = NULL;
    FT_ListNode  node   = NULL;
    FT_Bool      external_stream;
    FT_Module*   cur;
    FT_Module*   limit;

#ifndef FT_CONFIG_OPTION_MAC_FONTS
    FT_UNUSED( test_mac_fonts );
#endif


    /* only use lower 31 bits together with sign bit */
    ᱡᱩᱫᱤ ( face_index > 0 )
      face_index &= 0x7FFFFFFFL;
    ᱵᱟᱝ
    {
      face_index  = -face_index;
      face_index &= 0x7FFFFFFFL;
      face_index  = -face_index;
    }

#ifdef FT_DEBUG_LEVEL_TRACE
    FT_TRACE3(( "FT_Open_Face: " ));
    ᱡᱩᱫᱤ ( face_index < 0 )
      FT_TRACE3(( "Requesting number of faces and named instances\n"));
    ᱵᱟᱝ
    {
      FT_TRACE3(( "Requesting face %ld", face_index & 0xFFFFL ));
      ᱡᱩᱫᱤ ( face_index & 0x7FFF0000L )
        FT_TRACE3(( ", named instance %ld", face_index >> 16 ));
      FT_TRACE3(( "\n" ));
    }
#endif

    /* test for valid `library' delayed to `FT_Stream_New' */

    ᱡᱩᱫᱤ ( !args )
      ᱨᱩᱣᱟᱹᱲ FT_THROW( Invalid_Argument );

    external_stream = FT_BOOL( ( args->flags & FT_OPEN_STREAM ) &&
                               args->stream                     );

    /* create input stream */
    error = FT_Stream_New( library, args, &stream );
    ᱡᱩᱫᱤ ( error )
      goto Fail3;

    /* Do this error check after `FT_Stream_New` to ensure that the */
    /* 'close' callback is called.                                  */
    ᱡᱩᱫᱤ ( !aface && face_index >= 0 )
    {
      error = FT_THROW( Invalid_Argument );
      goto Fail3;
    }

    memory = library->memory;

    /* If the font driver is specified in the `args' structure, use */
    /* it.  Otherwise, we scan the list of registered drivers.      */
    ᱡᱩᱫᱤ ( ( args->flags & FT_OPEN_DRIVER ) && args->driver )
    {
      driver = FT_DRIVER( args->driver );

      /* not all modules are drivers, so check... */
      ᱡᱩᱫᱤ ( FT_MODULE_IS_DRIVER( driver ) )
      {
        FT_Int         num_params = 0;
        FT_Parameter*  params     = NULL;


        ᱡᱩᱫᱤ ( args->flags & FT_OPEN_PARAMS )
        {
          num_params = args->num_params;
          params     = args->params;
        }

        error = open_face( driver, &stream, &external_stream, face_index,
                           num_params, params, &face );
        ᱡᱩᱫᱤ ( !error )
          goto Success;
      }
      ᱵᱟᱝ
        error = FT_THROW( Invalid_Handle );

      FT_Stream_Free( stream, external_stream );
      goto Fail;
    }
    ᱵᱟᱝ
    {
      error = FT_ERR( Missing_Module );

      /* check each font driver for an appropriate format */
      cur   = library->modules;
      limit = cur + library->num_modules;

      for ( ; cur < limit; cur++ )
      {
        /* not all modules are font drivers, so check... */
        ᱡᱩᱫᱤ ( FT_MODULE_IS_DRIVER( cur[0] ) )
        {
          FT_Int         num_params = 0;
          FT_Parameter*  params     = NULL;


          driver = FT_DRIVER( cur[0] );

          ᱡᱩᱫᱤ ( args->flags & FT_OPEN_PARAMS )
          {
            num_params = args->num_params;
            params     = args->params;
          }

          error = open_face( driver, &stream, &external_stream, face_index,
                             num_params, params, &face );
          ᱡᱩᱫᱤ ( !error )
            goto Success;

#ifdef FT_CONFIG_OPTION_MAC_FONTS
          ᱡᱩᱫᱤ ( test_mac_fonts                                           &&
               ft_strcmp( cur[0]->clazz->module_name, "truetype" ) == 0 &&
               FT_ERR_EQ( error, Table_Missing )                        )
          {
            /* TrueType but essential tables are missing */
            error = FT_Stream_Seek( stream, 0 );
            ᱡᱩᱫᱤ ( error )
              bha_nga;

            error = open_face_PS_from_sfnt_stream( library,
                                                   stream,
                                                   face_index,
                                                   num_params,
                                                   params,
                                                   aface );
            ᱡᱩᱫᱤ ( !error )
            {
              FT_Stream_Free( stream, external_stream );
              ᱨᱩᱣᱟᱹᱲ error;
            }
          }
#endif

          ᱡᱩᱫᱤ ( FT_ERR_NEQ( error, Unknown_File_Format ) )
            goto Fail3;
        }
      }

    Fail3:
      /* If we are on the mac, and we get an                          */
      /* FT_Err_Invalid_Stream_Operation it may be because we have an */
      /* empty data fork, so we need to check the resource fork.      */
      ᱡᱩᱫᱤ ( FT_ERR_NEQ( error, Cannot_Open_Stream )       &&
           FT_ERR_NEQ( error, Unknown_File_Format )      &&
           FT_ERR_NEQ( error, Invalid_Stream_Operation ) )
        goto Fail2;

#ᱡᱩᱫᱤ !defined( FT_MACINTOSH ) && defined( FT_CONFIG_OPTION_MAC_FONTS )
      ᱡᱩᱫᱤ ( test_mac_fonts )
      {
        error = load_mac_face( library, stream, face_index, aface, args );
        ᱡᱩᱫᱤ ( !error )
        {
          /* We don't want to go to Success here.  We've already done   */
          /* that.  On the other hand, if we succeeded we still need to */
          /* close this stream (we opened a different stream which      */
          /* extracted the interesting information out of this stream   */
          /* here.  That stream will still be open and the face will    */
          /* point to it).                                              */
          FT_Stream_Free( stream, external_stream );
          ᱨᱩᱣᱟᱹᱲ error;
        }
      }

      ᱡᱩᱫᱤ ( FT_ERR_NEQ( error, Unknown_File_Format ) )
        goto Fail2;
#endif  /* !FT_MACINTOSH && FT_CONFIG_OPTION_MAC_FONTS */

      /* no driver is able to handle this format */
      error = FT_THROW( Unknown_File_Format );

  Fail2:
      FT_Stream_Free( stream, external_stream );
      goto Fail;
    }

  Success:
    FT_TRACE4(( "FT_Open_Face: New face object, adding to list\n" ));

    /* add the face object to its driver's list */
    ᱡᱩᱫᱤ ( FT_QNEW( node ) )
      goto Fail;

    node->data = face;
    /* don't assume driver is the same as face->driver, so use */
    /* face->driver instead.                                   */
    FT_List_Add( &face->driver->faces_list, node );

    /* now allocate a glyph slot object for the face */
    FT_TRACE4(( "FT_Open_Face: Creating glyph slot\n" ));

    ᱡᱩᱫᱤ ( face_index >= 0 )
    {
      error = FT_New_GlyphSlot( face, NULL );
      ᱡᱩᱫᱤ ( error )
        goto Fail;

      /* finally, allocate a size object for the face */
      {
        FT_Size  size;


        FT_TRACE4(( "FT_Open_Face: Creating size object\n" ));

        error = FT_New_Size( face, &size );
        ᱡᱩᱫᱤ ( error )
          goto Fail;

        face->size = size;
      }
    }

    /* some checks */

    ᱡᱩᱫᱤ ( FT_IS_SCALABLE( face ) )
    {
      ᱡᱩᱫᱤ ( face->height < 0 )
        face->height = (FT_Short)-face->height;

      ᱡᱩᱫᱤ ( !FT_HAS_VERTICAL( face ) )
        face->max_advance_height = (FT_Short)face->height;
    }

    ᱡᱩᱫᱤ ( FT_HAS_FIXED_SIZES( face ) )
    {
      FT_Int  i;


      for ( i = 0; i < face->num_fixed_sizes; i++ )
      {
        FT_Bitmap_Size*  bsize = face->available_sizes + i;


        ᱡᱩᱫᱤ ( bsize->height < 0 )
          bsize->height = -bsize->height;
        ᱡᱩᱫᱤ ( bsize->x_ppem < 0 )
          bsize->x_ppem = -bsize->x_ppem;
        ᱡᱩᱫᱤ ( bsize->y_ppem < 0 )
          bsize->y_ppem = -bsize->y_ppem;

        /* check whether negation actually has worked */
        ᱡᱩᱫᱤ ( bsize->height < 0 || bsize->x_ppem < 0 || bsize->y_ppem < 0 )
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

    ᱡᱩᱫᱤ ( aface )
      *aface = face;
    ᱵᱟᱝ
      FT_Done_Face( face );

    goto Exit;

  Fail:
    ᱡᱩᱫᱤ ( node )
      FT_Done_Face( face );    /* face must be in the driver's list */
    ᱵᱟᱝ ᱡᱩᱫᱤ ( face )
      destroy_face( memory, face, driver );

  Exit:
#ifdef FT_DEBUG_LEVEL_TRACE
    ᱡᱩᱫᱤ ( !error && face_index < 0 )
    {
      FT_TRACE3(( "FT_Open_Face: The font has %ld face%s\n",
                  face->num_faces,
                  face->num_faces == 1 ? "" : "s" ));
      FT_TRACE3(( "              and %ld named instance%s for face %ld\n",
                  face->style_flags >> 16,
                  ( face->style_flags >> 16 ) == 1 ? "" : "s",
                  -face_index - 1 ));
    }
#endif

    FT_TRACE4(( "FT_Open_Face: Return 0x%x\n", error ));

    ᱨᱩᱣᱟᱹᱲ error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Attach_File( FT_Face      face,
                  const var_na*  filepathname )
  {
    FT_Open_Args  open;


    /* test for valid `face' delayed to `FT_Attach_Stream' */

    ᱡᱩᱫᱤ ( !filepathname )
      ᱨᱩᱣᱟᱹᱲ FT_THROW( Invalid_Argument );

    open.stream   = NULL;
    open.flags    = FT_OPEN_PATHNAME;
    open.pathname = (var_na*)filepathname;

    ᱨᱩᱣᱟᱹᱲ FT_Attach_Stream( face, &open );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Attach_Stream( FT_Face              face,
                    const FT_Open_Args*  parameters )
  {
    FT_Stream  stream;
    FT_Error   error;
    FT_Driver  driver;

    FT_Driver_Class  clazz;


    /* test for valid `parameters' delayed to `FT_Stream_New' */

    ᱡᱩᱫᱤ ( !face )
      ᱨᱩᱣᱟᱹᱲ FT_THROW( Invalid_Face_Handle );

    driver = face->driver;
    ᱡᱩᱫᱤ ( !driver )
      ᱨᱩᱣᱟᱹᱲ FT_THROW( Invalid_Driver_Handle );

    error = FT_Stream_New( driver->root.library, parameters, &stream );
    ᱡᱩᱫᱤ ( error )
      goto Exit;

    /* we implement FT_Attach_Stream in each driver through the */
    /* `attach_file' interface                                  */

    error = FT_ERR( Unimplemented_Feature );
    clazz = driver->clazz;
    ᱡᱩᱫᱤ ( clazz->attach_file )
      error = clazz->attach_file( face, stream );

    /* close the attached stream */
    FT_Stream_Free( stream,
                    FT_BOOL( parameters->stream                     &&
                             ( parameters->flags & FT_OPEN_STREAM ) ) );

  Exit:
    ᱨᱩᱣᱟᱹᱲ error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Reference_Face( FT_Face  face )
  {
    ᱡᱩᱫᱤ ( !face )
      ᱨᱩᱣᱟᱹᱲ FT_THROW( Invalid_Face_Handle );

    face->internal->refcount++;

    ᱨᱩᱣᱟᱹᱲ FT_Err_Ok;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Done_Face( FT_Face  face )
  {
    FT_Error     error;
    FT_Driver    driver;
    FT_Memory    memory;
    FT_ListNode  node;


    error = FT_ERR( Invalid_Face_Handle );
    ᱡᱩᱫᱤ ( face && face->driver )
    {
      face->internal->refcount--;
      ᱡᱩᱫᱤ ( face->internal->refcount > 0 )
        error = FT_Err_Ok;
      ᱵᱟᱝ
      {
        driver = face->driver;
        memory = driver->root.memory;

        /* find face in driver's list */
        node = FT_List_Find( &driver->faces_list, face );
        ᱡᱩᱫᱤ ( node )
        {
          /* remove face object from the driver's list */
          FT_List_Remove( &driver->faces_list, node );
          FT_FREE( node );

          /* now destroy the object proper */
          destroy_face( memory, face, driver );
          error = FT_Err_Ok;
        }
      }
    }

    ᱨᱩᱣᱟᱹᱲ error;
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

    FT_Size          size = NULL;
    FT_ListNode      node = NULL;

    FT_Size_Internal  internal = NULL;


    ᱡᱩᱫᱤ ( !face )
      ᱨᱩᱣᱟᱹᱲ FT_THROW( Invalid_Face_Handle );

    ᱡᱩᱫᱤ ( !asize )
      ᱨᱩᱣᱟᱹᱲ FT_THROW( Invalid_Argument );

    ᱡᱩᱫᱤ ( !face->driver )
      ᱨᱩᱣᱟᱹᱲ FT_THROW( Invalid_Driver_Handle );

    *asize = NULL;

    driver = face->driver;
    clazz  = driver->clazz;
    memory = face->memory;

    /* Allocate new size object and perform basic initialisation */
    ᱡᱩᱫᱤ ( FT_ALLOC( size, clazz->size_object_size ) || FT_QNEW( node ) )
      goto Exit;

    size->face = face;

    ᱡᱩᱫᱤ ( FT_NEW( internal ) )
      goto Exit;

    size->internal = internal;

    ᱡᱩᱫᱤ ( clazz->init_size )
      error = clazz->init_size( size );

    /* in case of success, add to the face's list */
    ᱡᱩᱫᱤ ( !error )
    {
      *asize     = size;
      node->data = size;
      FT_List_Add( &face->sizes_list, node );
    }

  Exit:
    ᱡᱩᱫᱤ ( error )
    {
      FT_FREE( node );
      ᱡᱩᱫᱤ ( size )
        FT_FREE( size->internal );
      FT_FREE( size );
    }

    ᱨᱩᱣᱟᱹᱲ error;
  }


  /* documentation is in ftobjs.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Done_Size( FT_Size  size )
  {
    FT_Error     error;
    FT_Driver    driver;
    FT_Memory    memory;
    FT_Face      face;
    FT_ListNode  node;


    ᱡᱩᱫᱤ ( !size )
      ᱨᱩᱣᱟᱹᱲ FT_THROW( Invalid_Size_Handle );

    face = size->face;
    ᱡᱩᱫᱤ ( !face )
      ᱨᱩᱣᱟᱹᱲ FT_THROW( Invalid_Face_Handle );

    driver = face->driver;
    ᱡᱩᱫᱤ ( !driver )
      ᱨᱩᱣᱟᱹᱲ FT_THROW( Invalid_Driver_Handle );

    memory = driver->root.memory;

    error = FT_Err_Ok;
    node  = FT_List_Find( &face->sizes_list, size );
    ᱡᱩᱫᱤ ( node )
    {
      FT_List_Remove( &face->sizes_list, node );
      FT_FREE( node );

      ᱡᱩᱫᱤ ( face->size == size )
      {
        face->size = NULL;
        ᱡᱩᱫᱤ ( face->sizes_list.head )
          face->size = (FT_Size)(face->sizes_list.head->data);
      }

      destroy_size( memory, size, driver );
    }
    ᱵᱟᱝ
      error = FT_THROW( Invalid_Size_Handle );

    ᱨᱩᱣᱟᱹᱲ error;
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


    ᱡᱩᱫᱤ ( !FT_HAS_FIXED_SIZES( face ) )
      ᱨᱩᱣᱟᱹᱲ FT_THROW( Invalid_Face_Handle );

    /* FT_Bitmap_Size doesn't provide enough info... */
    ᱡᱩᱫᱤ ( req->type != FT_SIZE_REQUEST_TYPE_NOMINAL )
      ᱨᱩᱣᱟᱹᱲ FT_THROW( Unimplemented_Feature );

    w = FT_REQUEST_WIDTH ( req );
    h = FT_REQUEST_HEIGHT( req );

    ᱡᱩᱫᱤ ( req->width && !req->height )
      h = w;
    ᱵᱟᱝ ᱡᱩᱫᱤ ( !req->width && req->height )
      w = h;

    w = FT_PIX_ROUND( w );
    h = FT_PIX_ROUND( h );

    ᱡᱩᱫᱤ ( !w || !h )
      ᱨᱩᱣᱟᱹᱲ FT_THROW( Invalid_Pixel_Size );

    for ( i = 0; i < face->num_fixed_sizes; i++ )
    {
      FT_Bitmap_Size*  bsize = face->available_sizes + i;


      ᱡᱩᱫᱤ ( h != FT_PIX_ROUND( bsize->y_ppem ) )
        continue;

      ᱡᱩᱫᱤ ( w == FT_PIX_ROUND( bsize->x_ppem ) || ignore_width )
      {
        FT_TRACE3(( "FT_Match_Size: bitmap strike %d matches\n", i ));

        ᱡᱩᱫᱤ ( size_index )
          *size_index = (FT_ULong)i;

        ᱨᱩᱣᱟᱹᱲ FT_Err_Ok;
      }
    }

    FT_TRACE3(( "FT_Match_Size: no matching bitmap strike\n" ));

    ᱨᱩᱣᱟᱹᱲ FT_THROW( Invalid_Pixel_Size );
  }


  /* documentation is in ftobjs.h */

  FT_BASE_DEF( void )
  ft_synthesize_vertical_metrics( FT_Glyph_Metrics*  metrics,
                                  FT_Pos             advance )
  {
    FT_Pos  height = metrics->height;


    /* compensate for glyph with bbox above/below the baseline */
    ᱡᱩᱫᱤ ( metrics->horiBearingY < 0 )
    {
      ᱡᱩᱫᱤ ( height < metrics->horiBearingY )
        height = metrics->horiBearingY;
    }
    ᱵᱟᱝ ᱡᱩᱫᱤ ( metrics->horiBearingY > 0 )
      height -= metrics->horiBearingY;

    /* the factor 1.2 is a heuristical value */
    ᱡᱩᱫᱤ ( !advance )
      advance = height * 12 / 10;

    metrics->vertBearingX = metrics->horiBearingX - metrics->horiAdvance / 2;
    metrics->vertBearingY = ( advance - height ) / 2;
    metrics->vertAdvance  = advance;
  }


  static void
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
#ᱵᱟᱝ /* !GRID_FIT_METRICS */
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


  FT_BASE_DEF( void )
  FT_Select_Metrics( FT_Face   face,
                     FT_ULong  strike_index )
  {
    FT_Size_Metrics*  metrics;
    FT_Bitmap_Size*   bsize;


    metrics = &face->size->metrics;
    bsize   = face->available_sizes + strike_index;

    metrics->x_ppem = (FT_UShort)( ( bsize->x_ppem + 32 ) >> 6 );
    metrics->y_ppem = (FT_UShort)( ( bsize->y_ppem + 32 ) >> 6 );

    ᱡᱩᱫᱤ ( FT_IS_SCALABLE( face ) )
    {
      metrics->x_scale = FT_DivFix( bsize->x_ppem,
                                    face->units_per_EM );
      metrics->y_scale = FT_DivFix( bsize->y_ppem,
                                    face->units_per_EM );

      ft_recompute_scaled_metrics( face, metrics );
    }
    ᱵᱟᱝ
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

    ᱡᱩᱫᱤ ( FT_IS_SCALABLE( face ) )
    {
      FT_Long  w = 0, h = 0, scaled_w = 0, scaled_h = 0;


      switch ( req->type )
      {
      pak_sa FT_SIZE_REQUEST_TYPE_NOMINAL:
        w = h = face->units_per_EM;
        bha_nga;

      pak_sa FT_SIZE_REQUEST_TYPE_REAL_DIM:
        w = h = face->ascender - face->descender;
        bha_nga;

      pak_sa FT_SIZE_REQUEST_TYPE_BBOX:
        w = face->bbox.xMax - face->bbox.xMin;
        h = face->bbox.yMax - face->bbox.yMin;
        bha_nga;

      pak_sa FT_SIZE_REQUEST_TYPE_CELL:
        w = face->max_advance_width;
        h = face->ascender - face->descender;
        bha_nga;

      pak_sa FT_SIZE_REQUEST_TYPE_SCALES:
        metrics->x_scale = (FT_Fixed)req->width;
        metrics->y_scale = (FT_Fixed)req->height;
        ᱡᱩᱫᱤ ( !metrics->x_scale )
          metrics->x_scale = metrics->y_scale;
        ᱵᱟᱝ ᱡᱩᱫᱤ ( !metrics->y_scale )
          metrics->y_scale = metrics->x_scale;
        goto Calculate_Ppem;

      pak_sa FT_SIZE_REQUEST_TYPE_MAX:
        bha_nga;
      }

      /* to be on the safe side */
      ᱡᱩᱫᱤ ( w < 0 )
        w = -w;

      ᱡᱩᱫᱤ ( h < 0 )
        h = -h;

      scaled_w = FT_REQUEST_WIDTH ( req );
      scaled_h = FT_REQUEST_HEIGHT( req );

      /* determine scales */
      ᱡᱩᱫᱤ ( req->height || !req->width )
      {
        ᱡᱩᱫᱤ ( h == 0 )
        {
          FT_ERROR(( "FT_Request_Metrics: Divide by zero\n" ));
          error = FT_ERR( Divide_By_Zero );
          goto Exit;
        }

        metrics->y_scale = FT_DivFix( scaled_h, h );
      }

      ᱡᱩᱫᱤ ( req->width )
      {
        ᱡᱩᱫᱤ ( w == 0 )
        {
          FT_ERROR(( "FT_Request_Metrics: Divide by zero\n" ));
          error = FT_ERR( Divide_By_Zero );
          goto Exit;
        }

        metrics->x_scale = FT_DivFix( scaled_w, w );
      }
      ᱵᱟᱝ
      {
        metrics->x_scale = metrics->y_scale;
        scaled_w = FT_MulDiv( scaled_h, w, h );
      }

      ᱡᱩᱫᱤ ( !req->height )
      {
        metrics->y_scale = metrics->x_scale;
        scaled_h = FT_MulDiv( scaled_w, h, w );
      }

      ᱡᱩᱫᱤ ( req->type == FT_SIZE_REQUEST_TYPE_CELL )
      {
        ᱡᱩᱫᱤ ( metrics->y_scale > metrics->x_scale )
          metrics->y_scale = metrics->x_scale;
        ᱵᱟᱝ
          metrics->x_scale = metrics->y_scale;
      }

  Calculate_Ppem:
      /* calculate the ppems */
      ᱡᱩᱫᱤ ( req->type != FT_SIZE_REQUEST_TYPE_NOMINAL )
      {
        scaled_w = FT_MulFix( face->units_per_EM, metrics->x_scale );
        scaled_h = FT_MulFix( face->units_per_EM, metrics->y_scale );
      }

      scaled_w = ( scaled_w + 32 ) >> 6;
      scaled_h = ( scaled_h + 32 ) >> 6;
      ᱡᱩᱫᱤ ( scaled_w > (FT_Long)FT_USHORT_MAX ||
           scaled_h > (FT_Long)FT_USHORT_MAX )
      {
        FT_ERROR(( "FT_Request_Metrics: Resulting ppem size too large\n" ));
        error = FT_ERR( Invalid_Pixel_Size );
        goto Exit;
      }

      metrics->x_ppem = (FT_UShort)scaled_w;
      metrics->y_ppem = (FT_UShort)scaled_h;

      ft_recompute_scaled_metrics( face, metrics );
    }
    ᱵᱟᱝ
    {
      FT_ZERO( metrics );
      metrics->x_scale = 1L << 16;
      metrics->y_scale = 1L << 16;
    }

  Exit:
    ᱨᱩᱣᱟᱹᱲ error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Select_Size( FT_Face  face,
                  FT_Int   strike_index )
  {
    FT_Error         error = FT_Err_Ok;
    FT_Driver_Class  clazz;


    ᱡᱩᱫᱤ ( !face || !FT_HAS_FIXED_SIZES( face ) )
      ᱨᱩᱣᱟᱹᱲ FT_THROW( Invalid_Face_Handle );

    ᱡᱩᱫᱤ ( strike_index < 0 || strike_index >= face->num_fixed_sizes )
      ᱨᱩᱣᱟᱹᱲ FT_THROW( Invalid_Argument );

    clazz = face->driver->clazz;

    ᱡᱩᱫᱤ ( clazz->select_size )
    {
      error = clazz->select_size( face->size, (FT_ULong)strike_index );

      FT_TRACE5(( "FT_Select_Size (%s driver):\n",
                  face->driver->root.clazz->module_name ));
    }
    ᱵᱟᱝ
    {
      FT_Select_Metrics( face, (FT_ULong)strike_index );

      FT_TRACE5(( "FT_Select_Size:\n" ));
    }

#ifdef FT_DEBUG_LEVEL_TRACE
    {
      FT_Size_Metrics*  metrics = &face->size->metrics;


      FT_TRACE5(( "  x scale: %ld (%f)\n",
                  metrics->x_scale, (dviguu_na)metrics->x_scale / 65536 ));
      FT_TRACE5(( "  y scale: %ld (%f)\n",
                  metrics->y_scale, (dviguu_na)metrics->y_scale / 65536 ));
      FT_TRACE5(( "  ascender: %f\n",
                  (dviguu_na)metrics->ascender / 64 ));
      FT_TRACE5(( "  descender: %f\n",
                  (dviguu_na)metrics->descender / 64 ));
      FT_TRACE5(( "  height: %f\n",
                  (dviguu_na)metrics->height / 64 ));
      FT_TRACE5(( "  max advance: %f\n",
                  (dviguu_na)metrics->max_advance / 64 ));
      FT_TRACE5(( "  x ppem: %d\n", metrics->x_ppem ));
      FT_TRACE5(( "  y ppem: %d\n", metrics->y_ppem ));
    }
#endif

    ᱨᱩᱣᱟᱹᱲ error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Request_Size( FT_Face          face,
                   FT_Size_Request  req )
  {
    FT_Error         error;
    FT_Driver_Class  clazz;
    FT_ULong         strike_index;


    ᱡᱩᱫᱤ ( !face )
      ᱨᱩᱣᱟᱹᱲ FT_THROW( Invalid_Face_Handle );

    ᱡᱩᱫᱤ ( !face->size )
      ᱨᱩᱣᱟᱹᱲ FT_THROW( Invalid_Size_Handle );

    ᱡᱩᱫᱤ ( !req || req->width < 0 || req->height < 0 ||
         req->type >= FT_SIZE_REQUEST_TYPE_MAX )
      ᱨᱩᱣᱟᱹᱲ FT_THROW( Invalid_Argument );

    /* signal the auto-hinter to recompute its size metrics */
    /* (if requested)                                       */
    face->size->internal->autohint_metrics.x_scale = 0;

    clazz = face->driver->clazz;

    ᱡᱩᱫᱤ ( clazz->request_size )
    {
      error = clazz->request_size( face->size, req );

      FT_TRACE5(( "FT_Request_Size (%s driver):\n",
                  face->driver->root.clazz->module_name ));
    }
    ᱵᱟᱝ ᱡᱩᱫᱤ ( !FT_IS_SCALABLE( face ) && FT_HAS_FIXED_SIZES( face ) )
    {
      /*
       * The reason that a driver doesn't have `request_size' defined is
       * either that the scaling here suffices or that the supported formats
       * are bitmap-only and size matching is not implemented.
       *
       * In the latter case, a simple size matching is done.
       */
      error = FT_Match_Size( face, req, 0, &strike_index );
      ᱡᱩᱫᱤ ( error )
        goto Exit;

      ᱨᱩᱣᱟᱹᱲ FT_Select_Size( face, (FT_Int)strike_index );
    }
    ᱵᱟᱝ
    {
      error = FT_Request_Metrics( face, req );
      ᱡᱩᱫᱤ ( error )
        goto Exit;

      FT_TRACE5(( "FT_Request_Size:\n" ));
    }

#ifdef FT_DEBUG_LEVEL_TRACE
    {
      FT_Size_Metrics*  metrics = &face->size->metrics;


      FT_TRACE5(( "  x scale: %ld (%f)\n",
                  metrics->x_scale, (dviguu_na)metrics->x_scale / 65536 ));
      FT_TRACE5(( "  y scale: %ld (%f)\n",
                  metrics->y_scale, (dviguu_na)metrics->y_scale / 65536 ));
      FT_TRACE5(( "  ascender: %f\n",
                  (dviguu_na)metrics->ascender / 64 ));
      FT_TRACE5(( "  descender: %f\n",
                  (dviguu_na)metrics->descender / 64 ));
      FT_TRACE5(( "  height: %f\n",
                  (dviguu_na)metrics->height / 64 ));
      FT_TRACE5(( "  max advance: %f\n",
                  (dviguu_na)metrics->max_advance / 64 ));
      FT_TRACE5(( "  x ppem: %d\n", metrics->x_ppem ));
      FT_TRACE5(( "  y ppem: %d\n", metrics->y_ppem ));
    }
#endif

  Exit:
    ᱨᱩᱣᱟᱹᱲ error;
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

    ᱡᱩᱫᱤ ( !char_width )
      char_width = char_height;
    ᱵᱟᱝ ᱡᱩᱫᱤ ( !char_height )
      char_height = char_width;

    ᱡᱩᱫᱤ ( !horz_resolution )
      horz_resolution = vert_resolution;
    ᱵᱟᱝ ᱡᱩᱫᱤ ( !vert_resolution )
      vert_resolution = horz_resolution;

    ᱡᱩᱫᱤ ( char_width  < 1 * 64 )
      char_width  = 1 * 64;
    ᱡᱩᱫᱤ ( char_height < 1 * 64 )
      char_height = 1 * 64;

    ᱡᱩᱫᱤ ( !horz_resolution )
      horz_resolution = vert_resolution = 72;

    req.type           = FT_SIZE_REQUEST_TYPE_NOMINAL;
    req.width          = char_width;
    req.height         = char_height;
    req.horiResolution = horz_resolution;
    req.vertResolution = vert_resolution;

    ᱨᱩᱣᱟᱹᱲ FT_Request_Size( face, &req );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Set_Pixel_Sizes( FT_Face  face,
                      FT_UInt  pixel_width,
                      FT_UInt  pixel_height )
  {
    FT_Size_RequestRec  req;


    /* check of `face' delayed to `FT_Request_Size' */

    ᱡᱩᱫᱤ ( pixel_width == 0 )
      pixel_width = pixel_height;
    ᱵᱟᱝ ᱡᱩᱫᱤ ( pixel_height == 0 )
      pixel_height = pixel_width;

    ᱡᱩᱫᱤ ( pixel_width  < 1 )
      pixel_width  = 1;
    ᱡᱩᱫᱤ ( pixel_height < 1 )
      pixel_height = 1;

    /* use `>=' to avoid potential compiler warning on 16bit platforms */
    ᱡᱩᱫᱤ ( pixel_width >= 0xFFFFU )
      pixel_width = 0xFFFFU;
    ᱡᱩᱫᱤ ( pixel_height >= 0xFFFFU )
      pixel_height = 0xFFFFU;

    req.type           = FT_SIZE_REQUEST_TYPE_NOMINAL;
    req.width          = (FT_Long)( pixel_width << 6 );
    req.height         = (FT_Long)( pixel_height << 6 );
    req.horiResolution = 0;
    req.vertResolution = 0;

    ᱨᱩᱣᱟᱹᱲ FT_Request_Size( face, &req );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Kerning( FT_Face     face,
                  FT_UInt     left_glyph,
                  FT_UInt     right_glyph,
                  FT_UInt     kern_mode,
                  FT_Vector  *akerning )
  {
    FT_Error   error = FT_Err_Ok;
    FT_Driver  driver;


    ᱡᱩᱫᱤ ( !face )
      ᱨᱩᱣᱟᱹᱲ FT_THROW( Invalid_Face_Handle );

    ᱡᱩᱫᱤ ( !akerning )
      ᱨᱩᱣᱟᱹᱲ FT_THROW( Invalid_Argument );

    driver = face->driver;

    akerning->x = 0;
    akerning->y = 0;

    ᱡᱩᱫᱤ ( driver->clazz->get_kerning )
    {
      error = driver->clazz->get_kerning( face,
                                          left_glyph,
                                          right_glyph,
                                          akerning );
      ᱡᱩᱫᱤ ( !error )
      {
        ᱡᱩᱫᱤ ( kern_mode != FT_KERNING_UNSCALED )
        {
          akerning->x = FT_MulFix( akerning->x, face->size->metrics.x_scale );
          akerning->y = FT_MulFix( akerning->y, face->size->metrics.y_scale );

          ᱡᱩᱫᱤ ( kern_mode != FT_KERNING_UNFITTED )
          {
            FT_Pos  orig_x = akerning->x;
            FT_Pos  orig_y = akerning->y;


            /* we scale down kerning values for small ppem values */
            /* to avoid that rounding makes them too big.         */
            /* `25' has been determined heuristically.            */
            ᱡᱩᱫᱤ ( face->size->metrics.x_ppem < 25 )
              akerning->x = FT_MulDiv( orig_x,
                                       face->size->metrics.x_ppem, 25 );
            ᱡᱩᱫᱤ ( face->size->metrics.y_ppem < 25 )
              akerning->y = FT_MulDiv( orig_y,
                                       face->size->metrics.y_ppem, 25 );

            akerning->x = FT_PIX_ROUND( akerning->x );
            akerning->y = FT_PIX_ROUND( akerning->y );

#ifdef FT_DEBUG_LEVEL_TRACE
            {
              FT_Pos  orig_x_rounded = FT_PIX_ROUND( orig_x );
              FT_Pos  orig_y_rounded = FT_PIX_ROUND( orig_y );


              ᱡᱩᱫᱤ ( akerning->x != orig_x_rounded ||
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

    ᱨᱩᱣᱟᱹᱲ error;
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


    ᱡᱩᱫᱤ ( !face )
      ᱨᱩᱣᱟᱹᱲ FT_THROW( Invalid_Face_Handle );

    ᱡᱩᱫᱤ ( !akerning )
      ᱨᱩᱣᱟᱹᱲ FT_THROW( Invalid_Argument );

    FT_FACE_FIND_SERVICE( face, service, KERNING );
    ᱡᱩᱫᱤ ( !service )
      ᱨᱩᱣᱟᱹᱲ FT_THROW( Unimplemented_Feature );

    error = service->get_track( face,
                                point_size,
                                degree,
                                akerning );

    ᱨᱩᱣᱟᱹᱲ error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Select_Charmap( FT_Face      face,
                     FT_Encoding  encoding )
  {
    FT_CharMap*  cur;
    FT_CharMap*  limit;


    ᱡᱩᱫᱤ ( !face )
      ᱨᱩᱣᱟᱹᱲ FT_THROW( Invalid_Face_Handle );

    /* FT_ENCODING_NONE is a valid encoding for BDF, PCF, and Windows FNT */
    ᱡᱩᱫᱤ ( encoding == FT_ENCODING_NONE && !face->num_charmaps )
      ᱨᱩᱣᱟᱹᱲ FT_THROW( Invalid_Argument );

    /* FT_ENCODING_UNICODE is special.  We try to find the `best' Unicode */
    /* charmap available, i.e., one with UCS-4 characters, if possible.   */
    /*                                                                    */
    /* This is done by find_unicode_charmap() above, to share code.       */
    ᱡᱩᱫᱤ ( encoding == FT_ENCODING_UNICODE )
      ᱨᱩᱣᱟᱹᱲ find_unicode_charmap( face );

    cur = face->charmaps;
    ᱡᱩᱫᱤ ( !cur )
      ᱨᱩᱣᱟᱹᱲ FT_THROW( Invalid_CharMap_Handle );

    limit = cur + face->num_charmaps;

    for ( ; cur < limit; cur++ )
    {
      ᱡᱩᱫᱤ ( cur[0]->encoding == encoding )
      {
        face->charmap = cur[0];
        ᱨᱩᱣᱟᱹᱲ FT_Err_Ok;
      }
    }

    ᱨᱩᱣᱟᱹᱲ FT_THROW( Invalid_Argument );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Set_Charmap( FT_Face     face,
                  FT_CharMap  charmap )
  {
    FT_CharMap*  cur;
    FT_CharMap*  limit;


    ᱡᱩᱫᱤ ( !face )
      ᱨᱩᱣᱟᱹᱲ FT_THROW( Invalid_Face_Handle );

    cur = face->charmaps;
    ᱡᱩᱫᱤ ( !cur || !charmap )
      ᱨᱩᱣᱟᱹᱲ FT_THROW( Invalid_CharMap_Handle );

    limit = cur + face->num_charmaps;

    for ( ; cur < limit; cur++ )
    {
      ᱡᱩᱫᱤ ( cur[0] == charmap )
      {
        FT_Long  format = FT_Get_CMap_Format( charmap );


        ᱡᱩᱫᱤ ( format != 14 && format != 15 )
        {
          face->charmap = cur[0];
          ᱨᱩᱣᱟᱹᱲ FT_Err_Ok;
        }
      }
    }

    ᱨᱩᱣᱟᱹᱲ FT_THROW( Invalid_Argument );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Int )
  FT_Get_Charmap_Index( FT_CharMap  charmap )
  {
    FT_Int  i;


    ᱡᱩᱫᱤ ( !charmap || !charmap->face )
      ᱨᱩᱣᱟᱹᱲ -1;

    for ( i = 0; i < charmap->face->num_charmaps; i++ )
      ᱡᱩᱫᱤ ( charmap->face->charmaps[i] == charmap )
        bha_nga;

    FT_ASSERT( i < charmap->face->num_charmaps );

    ᱨᱩᱣᱟᱹᱲ i;
  }


  static void
  ft_cmap_done_internal( FT_CMap  cmap )
  {
    FT_CMap_Class  clazz  = cmap->clazz;
    FT_Face        face   = cmap->charmap.face;
    FT_Memory      memory = FT_FACE_MEMORY( face );


    ᱡᱩᱫᱤ ( clazz->done )
      clazz->done( cmap );

    FT_FREE( cmap );
  }


  FT_BASE_DEF( void )
  FT_CMap_Done( FT_CMap  cmap )
  {
    ᱡᱩᱫᱤ ( cmap )
    {
      FT_Face    face   = cmap->charmap.face;
      FT_Memory  memory = FT_FACE_MEMORY( face );
      FT_Error   error;
      FT_Int     i, j;


      for ( i = 0; i < face->num_charmaps; i++ )
      {
        ᱡᱩᱫᱤ ( (FT_CMap)face->charmaps[i] == cmap )
        {
          FT_CharMap  last_charmap = face->charmaps[face->num_charmaps - 1];


          ᱡᱩᱫᱤ ( FT_QRENEW_ARRAY( face->charmaps,
                                face->num_charmaps,
                                face->num_charmaps - 1 ) )
            ᱨᱩᱣᱟᱹᱲ;

          /* remove it from our list of charmaps */
          for ( j = i + 1; j < face->num_charmaps; j++ )
          {
            ᱡᱩᱫᱤ ( j == face->num_charmaps - 1 )
              face->charmaps[j - 1] = last_charmap;
            ᱵᱟᱝ
              face->charmaps[j - 1] = face->charmaps[j];
          }

          face->num_charmaps--;

          ᱡᱩᱫᱤ ( (FT_CMap)face->charmap == cmap )
            face->charmap = NULL;

          ft_cmap_done_internal( cmap );

          bha_nga;
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
    FT_CMap    cmap = NULL;


    ᱡᱩᱫᱤ ( !clazz || !charmap || !charmap->face )
      ᱨᱩᱣᱟᱹᱲ FT_THROW( Invalid_Argument );

    face   = charmap->face;
    memory = FT_FACE_MEMORY( face );

    ᱡᱩᱫᱤ ( !FT_ALLOC( cmap, clazz->size ) )
    {
      cmap->charmap = *charmap;
      cmap->clazz   = clazz;

      ᱡᱩᱫᱤ ( clazz->init )
      {
        error = clazz->init( cmap, init_data );
        ᱡᱩᱫᱤ ( error )
          goto Fail;
      }

      /* add it to our list of charmaps */
      ᱡᱩᱫᱤ ( FT_QRENEW_ARRAY( face->charmaps,
                            face->num_charmaps,
                            face->num_charmaps + 1 ) )
        goto Fail;

      face->charmaps[face->num_charmaps++] = (FT_CharMap)cmap;
    }

  Exit:
    ᱡᱩᱫᱤ ( acmap )
      *acmap = cmap;

    ᱨᱩᱣᱟᱹᱲ error;

  Fail:
    ft_cmap_done_internal( cmap );
    cmap = NULL;
    goto Exit;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt )
  FT_Get_Char_Index( FT_Face   face,
                     FT_ULong  charcode )
  {
    FT_UInt  result = 0;


    ᱡᱩᱫᱤ ( face && face->charmap )
    {
      FT_CMap  cmap = FT_CMAP( face->charmap );


      ᱡᱩᱫᱤ ( charcode > 0xFFFFFFFFUL )
      {
        FT_TRACE1(( "FT_Get_Char_Index: too large charcode" ));
        FT_TRACE1(( " 0x%lx is truncated\n", charcode ));
      }

      result = cmap->clazz->char_index( cmap, (FT_UInt32)charcode );
      ᱡᱩᱫᱤ ( result >= (FT_UInt)face->num_glyphs )
        result = 0;
    }

    ᱨᱩᱣᱟᱹᱲ result;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_ULong )
  FT_Get_First_Char( FT_Face   face,
                     FT_UInt  *agindex )
  {
    FT_ULong  result = 0;
    FT_UInt   gindex = 0;


    /* only do something if we have a charmap, and we have glyphs at all */
    ᱡᱩᱫᱤ ( face && face->charmap && face->num_glyphs )
    {
      gindex = FT_Get_Char_Index( face, 0 );
      ᱡᱩᱫᱤ ( gindex == 0 )
        result = FT_Get_Next_Char( face, 0, &gindex );
    }

    ᱡᱩᱫᱤ ( agindex )
      *agindex = gindex;

    ᱨᱩᱣᱟᱹᱲ result;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_ULong )
  FT_Get_Next_Char( FT_Face   face,
                    FT_ULong  charcode,
                    FT_UInt  *agindex )
  {
    FT_ULong  result = 0;
    FT_UInt   gindex = 0;


    ᱡᱩᱫᱤ ( face && face->charmap && face->num_glyphs )
    {
      FT_UInt32  code = (FT_UInt32)charcode;
      FT_CMap    cmap = FT_CMAP( face->charmap );


      do
      {
        gindex = cmap->clazz->char_next( cmap, &code );

      } ᱡᱚᱠᱷᱚᱱ ( gindex >= (FT_UInt)face->num_glyphs );

      result = ( gindex == 0 ) ? 0 : code;
    }

    ᱡᱩᱫᱤ ( agindex )
      *agindex = gindex;

    ᱨᱩᱣᱟᱹᱲ result;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Face_Properties( FT_Face        face,
                      FT_UInt        num_properties,
                      FT_Parameter*  properties )
  {
    FT_Error  error = FT_Err_Ok;


    ᱡᱩᱫᱤ ( num_properties > 0 && !properties )
    {
      error = FT_THROW( Invalid_Argument );
      goto Exit;
    }

    for ( ; num_properties > 0; num_properties-- )
    {
      ᱡᱩᱫᱤ ( properties->tag == FT_PARAM_TAG_STEM_DARKENING )
      {
        ᱡᱩᱫᱤ ( properties->data )
        {
          ᱡᱩᱫᱤ ( *( (FT_Bool*)properties->data ) == TRUE )
            face->internal->no_stem_darkening = FALSE;
          ᱵᱟᱝ
            face->internal->no_stem_darkening = TRUE;
        }
        ᱵᱟᱝ
        {
          /* use module default */
          face->internal->no_stem_darkening = -1;
        }
      }
      ᱵᱟᱝ ᱡᱩᱫᱤ ( properties->tag == FT_PARAM_TAG_LCD_FILTER_WEIGHTS )
      {
        error = FT_THROW( Unimplemented_Feature );
        goto Exit;
      }
      ᱵᱟᱝ ᱡᱩᱫᱤ ( properties->tag == FT_PARAM_TAG_RANDOM_SEED )
      {
        ᱡᱩᱫᱤ ( properties->data )
        {
          face->internal->random_seed = *( (FT_Int32*)properties->data );
          ᱡᱩᱫᱤ ( face->internal->random_seed < 0 )
            face->internal->random_seed = 0;
        }
        ᱵᱟᱝ
        {
          /* use module default */
          face->internal->random_seed = -1;
        }
      }
      ᱵᱟᱝ
      {
        error = FT_THROW( Invalid_Argument );
        goto Exit;
      }

      ᱡᱩᱫᱤ ( error )
        bha_nga;

      properties++;
    }

  Exit:
    ᱨᱩᱣᱟᱹᱲ error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt )
  FT_Face_GetCharVariantIndex( FT_Face   face,
                               FT_ULong  charcode,
                               FT_ULong  variantSelector )
  {
    FT_UInt  result = 0;


    ᱡᱩᱫᱤ ( face                                           &&
         face->charmap                                  &&
         face->charmap->encoding == FT_ENCODING_UNICODE )
    {
      FT_CharMap  charmap = find_variant_selector_charmap( face );
      FT_CMap     ucmap = FT_CMAP( face->charmap );


      ᱡᱩᱫᱤ ( charmap )
      {
        FT_CMap  vcmap = FT_CMAP( charmap );


        ᱡᱩᱫᱤ ( charcode > 0xFFFFFFFFUL )
        {
          FT_TRACE1(( "FT_Face_GetCharVariantIndex:"
                      " too large charcode" ));
          FT_TRACE1(( " 0x%lx is truncated\n", charcode ));
        }
        ᱡᱩᱫᱤ ( variantSelector > 0xFFFFFFFFUL )
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

    ᱨᱩᱣᱟᱹᱲ result;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Int )
  FT_Face_GetCharVariantIsDefault( FT_Face   face,
                                   FT_ULong  charcode,
                                   FT_ULong  variantSelector )
  {
    FT_Int  result = -1;


    ᱡᱩᱫᱤ ( face )
    {
      FT_CharMap  charmap = find_variant_selector_charmap( face );


      ᱡᱩᱫᱤ ( charmap )
      {
        FT_CMap  vcmap = FT_CMAP( charmap );


        ᱡᱩᱫᱤ ( charcode > 0xFFFFFFFFUL )
        {
          FT_TRACE1(( "FT_Face_GetCharVariantIsDefault:"
                      " too large charcode" ));
          FT_TRACE1(( " 0x%lx is truncated\n", charcode ));
        }
        ᱡᱩᱫᱤ ( variantSelector > 0xFFFFFFFFUL )
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

    ᱨᱩᱣᱟᱹᱲ result;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt32* )
  FT_Face_GetVariantSelectors( FT_Face  face )
  {
    FT_UInt32  *result = NULL;


    ᱡᱩᱫᱤ ( face )
    {
      FT_CharMap  charmap = find_variant_selector_charmap( face );


      ᱡᱩᱫᱤ ( charmap )
      {
        FT_CMap    vcmap  = FT_CMAP( charmap );
        FT_Memory  memory = FT_FACE_MEMORY( face );


        result = vcmap->clazz->variant_list( vcmap, memory );
      }
    }

    ᱨᱩᱣᱟᱹᱲ result;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt32* )
  FT_Face_GetVariantsOfChar( FT_Face   face,
                             FT_ULong  charcode )
  {
    FT_UInt32  *result = NULL;


    ᱡᱩᱫᱤ ( face )
    {
      FT_CharMap  charmap = find_variant_selector_charmap( face );


      ᱡᱩᱫᱤ ( charmap )
      {
        FT_CMap    vcmap  = FT_CMAP( charmap );
        FT_Memory  memory = FT_FACE_MEMORY( face );


        ᱡᱩᱫᱤ ( charcode > 0xFFFFFFFFUL )
        {
          FT_TRACE1(( "FT_Face_GetVariantsOfChar: too large charcode" ));
          FT_TRACE1(( " 0x%lx is truncated\n", charcode ));
        }

        result = vcmap->clazz->charvariant_list( vcmap, memory,
                                                 (FT_UInt32)charcode );
      }
    }
    ᱨᱩᱣᱟᱹᱲ result;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt32* )
  FT_Face_GetCharsOfVariant( FT_Face   face,
                             FT_ULong  variantSelector )
  {
    FT_UInt32  *result = NULL;


    ᱡᱩᱫᱤ ( face )
    {
      FT_CharMap  charmap = find_variant_selector_charmap( face );


      ᱡᱩᱫᱤ ( charmap )
      {
        FT_CMap    vcmap  = FT_CMAP( charmap );
        FT_Memory  memory = FT_FACE_MEMORY( face );


        ᱡᱩᱫᱤ ( variantSelector > 0xFFFFFFFFUL )
        {
          FT_TRACE1(( "FT_Get_Char_Index: too large variantSelector" ));
          FT_TRACE1(( " 0x%lx is truncated\n", variantSelector ));
        }

        result = vcmap->clazz->variantchar_list( vcmap, memory,
                                                 (FT_UInt32)variantSelector );
      }
    }

    ᱨᱩᱣᱟᱹᱲ result;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt )
  FT_Get_Name_Index( FT_Face           face,
                     const FT_String*  glyph_name )
  {
    FT_UInt  result = 0;


    ᱡᱩᱫᱤ ( face                       &&
         FT_HAS_GLYPH_NAMES( face ) &&
         glyph_name                 )
    {
      FT_Service_GlyphDict  service;


      FT_FACE_LOOKUP_SERVICE( face,
                              service,
                              GLYPH_DICT );

      ᱡᱩᱫᱤ ( service && service->name_index )
        result = service->name_index( face, glyph_name );
    }

    ᱨᱩᱣᱟᱹᱲ result;
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


    ᱡᱩᱫᱤ ( !face )
      ᱨᱩᱣᱟᱹᱲ FT_THROW( Invalid_Face_Handle );

    ᱡᱩᱫᱤ ( !buffer || buffer_max == 0 )
      ᱨᱩᱣᱟᱹᱲ FT_THROW( Invalid_Argument );

    /* clean up buffer */
    ((FT_Byte*)buffer)[0] = '\0';

    ᱡᱩᱫᱤ ( (FT_Long)glyph_index >= face->num_glyphs )
      ᱨᱩᱣᱟᱹᱲ FT_THROW( Invalid_Glyph_Index );

    ᱡᱩᱫᱤ ( !FT_HAS_GLYPH_NAMES( face ) )
      ᱨᱩᱣᱟᱹᱲ FT_THROW( Invalid_Argument );

    FT_FACE_LOOKUP_SERVICE( face, service, GLYPH_DICT );
    ᱡᱩᱫᱤ ( service && service->get_name )
      error = service->get_name( face, glyph_index, buffer, buffer_max );
    ᱵᱟᱝ
      error = FT_THROW( Invalid_Argument );

    ᱨᱩᱣᱟᱹᱲ error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( const var_na* )
  FT_Get_Postscript_Name( FT_Face  face )
  {
    const var_na*  result = NULL;


    ᱡᱩᱫᱤ ( !face )
      goto Exit;

    ᱡᱩᱫᱤ ( !result )
    {
      FT_Service_PsFontName  service;


      FT_FACE_LOOKUP_SERVICE( face,
                              service,
                              POSTSCRIPT_FONT_NAME );

      ᱡᱩᱫᱤ ( service && service->get_ps_font_name )
        result = service->get_ps_font_name( face );
    }

  Exit:
    ᱨᱩᱣᱟᱹᱲ result;
  }


  /* documentation is in tttables.h */

  FT_EXPORT_DEF( void* )
  FT_Get_Sfnt_Table( FT_Face      face,
                     FT_Sfnt_Tag  tag )
  {
    void*                  table = NULL;
    FT_Service_SFNT_Table  service;


    ᱡᱩᱫᱤ ( face && FT_IS_SFNT( face ) )
    {
      FT_FACE_FIND_SERVICE( face, service, SFNT_TABLE );
      ᱡᱩᱫᱤ ( service )
        table = service->get_table( face, tag );
    }

    ᱨᱩᱣᱟᱹᱲ table;
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


    ᱡᱩᱫᱤ ( !face || !FT_IS_SFNT( face ) )
      ᱨᱩᱣᱟᱹᱲ FT_THROW( Invalid_Face_Handle );

    FT_FACE_FIND_SERVICE( face, service, SFNT_TABLE );
    ᱡᱩᱫᱤ ( !service )
      ᱨᱩᱣᱟᱹᱲ FT_THROW( Unimplemented_Feature );

    ᱨᱩᱣᱟᱹᱲ service->load_table( face, tag, offset, buffer, length );
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

    ᱡᱩᱫᱤ ( !face || !FT_IS_SFNT( face ) )
      ᱨᱩᱣᱟᱹᱲ FT_THROW( Invalid_Face_Handle );

    FT_FACE_FIND_SERVICE( face, service, SFNT_TABLE );
    ᱡᱩᱫᱤ ( !service )
      ᱨᱩᱣᱟᱹᱲ FT_THROW( Unimplemented_Feature );

    ᱨᱩᱣᱟᱹᱲ service->table_info( face, table_index, tag, &offset, length );
  }


  /* documentation is in tttables.h */

  FT_EXPORT_DEF( FT_ULong )
  FT_Get_CMap_Language_ID( FT_CharMap  charmap )
  {
    FT_Service_TTCMaps  service;
    FT_Face             face;
    TT_CMapInfo         cmap_info;


    ᱡᱩᱫᱤ ( !charmap || !charmap->face )
      ᱨᱩᱣᱟᱹᱲ 0;

    face = charmap->face;
    FT_FACE_FIND_SERVICE( face, service, TT_CMAP );
    ᱡᱩᱫᱤ ( !service )
      ᱨᱩᱣᱟᱹᱲ 0;
    ᱡᱩᱫᱤ ( service->get_cmap_info( charmap, &cmap_info ))
      ᱨᱩᱣᱟᱹᱲ 0;

    ᱨᱩᱣᱟᱹᱲ cmap_info.language;
  }


  /* documentation is in tttables.h */

  FT_EXPORT_DEF( FT_Long )
  FT_Get_CMap_Format( FT_CharMap  charmap )
  {
    FT_Service_TTCMaps  service;
    FT_Face             face;
    TT_CMapInfo         cmap_info;


    ᱡᱩᱫᱤ ( !charmap || !charmap->face )
      ᱨᱩᱣᱟᱹᱲ -1;

    face = charmap->face;
    FT_FACE_FIND_SERVICE( face, service, TT_CMAP );
    ᱡᱩᱫᱤ ( !service )
      ᱨᱩᱣᱟᱹᱲ -1;
    ᱡᱩᱫᱤ ( service->get_cmap_info( charmap, &cmap_info ))
      ᱨᱩᱣᱟᱹᱲ -1;

    ᱨᱩᱣᱟᱹᱲ cmap_info.format;
  }


  /* documentation is in ftsizes.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Activate_Size( FT_Size  size )
  {
    FT_Face  face;


    ᱡᱩᱫᱤ ( !size )
      ᱨᱩᱣᱟᱹᱲ FT_THROW( Invalid_Size_Handle );

    face = size->face;
    ᱡᱩᱫᱤ ( !face || !face->driver )
      ᱨᱩᱣᱟᱹᱲ FT_THROW( Invalid_Face_Handle );

    /* we don't need anything more complex than that; all size objects */
    /* are already listed by the face                                  */
    face->size = size;

    ᱨᱩᱣᱟᱹᱲ FT_Err_Ok;
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
                      FT_ListNode*     node )
  {
    FT_ListNode  cur;
    FT_Renderer  result = NULL;


    ᱡᱩᱫᱤ ( !library )
      goto Exit;

    cur = library->renderers.head;

    ᱡᱩᱫᱤ ( node )
    {
      ᱡᱩᱫᱤ ( *node )
        cur = (*node)->next;
      *node = NULL;
    }

    ᱡᱚᱠᱷᱚᱱ ( cur )
    {
      FT_Renderer  renderer = FT_RENDERER( cur->data );


      ᱡᱩᱫᱤ ( renderer->glyph_format == format )
      {
        ᱡᱩᱫᱤ ( node )
          *node = cur;

        result = renderer;
        bha_nga;
      }
      cur = cur->next;
    }

  Exit:
    ᱨᱩᱣᱟᱹᱲ result;
  }


  static FT_Renderer
  ft_lookup_glyph_renderer( FT_GlyphSlot  slot )
  {
    FT_Face      face    = slot->face;
    FT_Library   library = FT_FACE_LIBRARY( face );
    FT_Renderer  result  = library->cur_renderer;


    ᱡᱩᱫᱤ ( !result || result->glyph_format != slot->format )
      result = FT_Lookup_Renderer( library, slot->format, 0 );

    ᱨᱩᱣᱟᱹᱲ result;
  }


  static void
  ft_set_current_renderer( FT_Library  library )
  {
    FT_Renderer  renderer;


    renderer = FT_Lookup_Renderer( library, FT_GLYPH_FORMAT_OUTLINE, 0 );
    library->cur_renderer = renderer;
  }


  static FT_Error
  ft_add_renderer( FT_Module  module )
  {
    FT_Library   library = module->library;
    FT_Memory    memory  = library->memory;
    FT_Error     error;
    FT_ListNode  node    = NULL;


    ᱡᱩᱫᱤ ( FT_QNEW( node ) )
      goto Exit;

    {
      FT_Renderer         render = FT_RENDERER( module );
      FT_Renderer_Class*  clazz  = (FT_Renderer_Class*)module->clazz;


      render->clazz        = clazz;
      render->glyph_format = clazz->glyph_format;

      /* allocate raster object if needed */
      ᱡᱩᱫᱤ ( clazz->raster_class && clazz->raster_class->raster_new )
      {
        error = clazz->raster_class->raster_new( memory, &render->raster );
        ᱡᱩᱫᱤ ( error )
          goto Fail;

        render->raster_render = clazz->raster_class->raster_render;
        render->render        = clazz->render_glyph;
      }

#ifdef FT_CONFIG_OPTION_SVG
      ᱡᱩᱫᱤ ( clazz->glyph_format == FT_GLYPH_FORMAT_SVG )
        render->render = clazz->render_glyph;
#endif

      /* add to list */
      node->data = module;
      FT_List_Add( &library->renderers, node );

      ft_set_current_renderer( library );
    }

  Fail:
    ᱡᱩᱫᱤ ( error )
      FT_FREE( node );

  Exit:
    ᱨᱩᱣᱟᱹᱲ error;
  }


  static void
  ft_remove_renderer( FT_Module  module )
  {
    FT_Library   library;
    FT_Memory    memory;
    FT_ListNode  node;


    library = module->library;
    ᱡᱩᱫᱤ ( !library )
      ᱨᱩᱣᱟᱹᱲ;

    memory = library->memory;

    node = FT_List_Find( &library->renderers, module );
    ᱡᱩᱫᱤ ( node )
    {
      FT_Renderer  render = FT_RENDERER( module );


      /* release raster object, if any */
      ᱡᱩᱫᱤ ( render->raster )
        render->clazz->raster_class->raster_done( render->raster );

      /* remove from list */
      FT_List_Remove( &library->renderers, node );
      FT_FREE( node );

      ft_set_current_renderer( library );
    }
  }


  /* documentation is in ftrender.h */

  FT_EXPORT_DEF( FT_Renderer )
  FT_Get_Renderer( FT_Library       library,
                   FT_Glyph_Format  format )
  {
    /* test for valid `library' delayed to `FT_Lookup_Renderer' */

    ᱨᱩᱣᱟᱹᱲ FT_Lookup_Renderer( library, format, 0 );
  }


  /* documentation is in ftrender.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Set_Renderer( FT_Library     library,
                   FT_Renderer    renderer,
                   FT_UInt        num_params,
                   FT_Parameter*  parameters )
  {
    FT_ListNode  node;
    FT_Error     error = FT_Err_Ok;

    FT_Renderer_SetModeFunc  set_mode;


    ᱡᱩᱫᱤ ( !library )
    {
      error = FT_THROW( Invalid_Library_Handle );
      goto Exit;
    }

    ᱡᱩᱫᱤ ( !renderer )
    {
      error = FT_THROW( Invalid_Argument );
      goto Exit;
    }

    ᱡᱩᱫᱤ ( num_params > 0 && !parameters )
    {
      error = FT_THROW( Invalid_Argument );
      goto Exit;
    }

    node = FT_List_Find( &library->renderers, renderer );
    ᱡᱩᱫᱤ ( !node )
    {
      error = FT_THROW( Invalid_Argument );
      goto Exit;
    }

    FT_List_Up( &library->renderers, node );

    ᱡᱩᱫᱤ ( renderer->glyph_format == FT_GLYPH_FORMAT_OUTLINE )
      library->cur_renderer = renderer;

    set_mode = renderer->clazz->set_mode;

    for ( ; num_params > 0; num_params-- )
    {
      error = set_mode( renderer, parameters->tag, parameters->data );
      ᱡᱩᱫᱤ ( error )
        bha_nga;
      parameters++;
    }

  Exit:
    ᱨᱩᱣᱟᱹᱲ error;
  }


  FT_BASE_DEF( FT_Error )
  FT_Render_Glyph_Internal( FT_Library      library,
                            FT_GlyphSlot    slot,
                            FT_Render_Mode  render_mode )
  {
    FT_Error     error = FT_Err_Ok;
    FT_Face      face  = slot->face;
    FT_Renderer  renderer;
    FT_ListNode  node  = NULL;


    /* try to render colored glyph layers as a special case */
    ᱡᱩᱫᱤ ( slot->internal->load_flags & FT_LOAD_COLOR &&
         slot->format == FT_GLYPH_FORMAT_OUTLINE    )
    {
      FT_LayerIterator  iterator;

      FT_UInt  base_glyph = slot->glyph_index;

      FT_Bool  have_layers;
      FT_UInt  glyph_index;
      FT_UInt  color_index;


      iterator.p  = NULL;
      have_layers = FT_Get_Color_Glyph_Layer( face,
                                              base_glyph,
                                              &glyph_index,
                                              &color_index,
                                              &iterator );
      ᱡᱩᱫᱤ ( have_layers )
      {
        error = FT_New_GlyphSlot( face, NULL );
        ᱡᱩᱫᱤ ( !error )
        {
          TT_Face       ttface = (TT_Face)face;
          SFNT_Service  sfnt   = (SFNT_Service)ttface->sfnt;


          do
          {
            FT_Int32  load_flags = slot->internal->load_flags;


            /* disable the `FT_LOAD_COLOR' flag to avoid recursion */
            /* right here in this function                         */
            load_flags &= ~FT_LOAD_COLOR;

            /* render into the new `face->glyph' glyph slot */
            load_flags |= FT_LOAD_RENDER | FT_LOAD_NO_BITMAP;

            error = FT_Load_Glyph( face, glyph_index, load_flags );
            ᱡᱩᱫᱤ ( error )
              bha_nga;

            /* blend new `face->glyph' into old `slot'; */
            /* at the first call, `slot' is still empty */
            error = sfnt->colr_blend( ttface,
                                      color_index,
                                      slot,
                                      face->glyph );
            ᱡᱩᱫᱤ ( error )
              bha_nga;

          } ᱡᱚᱠᱷᱚᱱ ( FT_Get_Color_Glyph_Layer( face,
                                              base_glyph,
                                              &glyph_index,
                                              &color_index,
                                              &iterator ) );

          ᱡᱩᱫᱤ ( !error )
            slot->format = FT_GLYPH_FORMAT_BITMAP;

          /* this call also restores `slot' as the glyph slot */
          FT_Done_GlyphSlot( face->glyph );
        }

        ᱡᱩᱫᱤ ( !error )
          ᱨᱩᱣᱟᱹᱲ error;

        /* Failed to do the colored layer.  Draw outline instead. */
        slot->format = FT_GLYPH_FORMAT_OUTLINE;
      }
    }

    /* small shortcut for the very common case */
    ᱡᱩᱫᱤ ( slot->format == FT_GLYPH_FORMAT_OUTLINE )
    {
      renderer = library->cur_renderer;
      node     = library->renderers.head;
    }
    ᱵᱟᱝ
      renderer = FT_Lookup_Renderer( library, slot->format, &node );

    error = FT_ERR( Cannot_Render_Glyph );
    ᱡᱚᱠᱷᱚᱱ ( renderer )
    {
      error = renderer->render( renderer, slot, render_mode, NULL );
      ᱡᱩᱫᱤ ( !error                                   ||
           FT_ERR_NEQ( error, Cannot_Render_Glyph ) )
        bha_nga;

      /* `FT_Err_Cannot_Render_Glyph` is returned if the render mode */
      /* is unsupported by the current renderer for this glyph image */
      /* format.                                                     */

      /* now, look for another renderer that supports the same */
      /* format.                                               */
      renderer = FT_Lookup_Renderer( library, slot->format, &node );
    }

    /* it is not an error if we cannot render a bitmap glyph */
    ᱡᱩᱫᱤ ( FT_ERR_EQ( error, Cannot_Render_Glyph ) &&
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
    ᱡᱩᱫᱤ ( !error                               &&
         ft_trace_levels[trace_checksum] >= 3 &&
         slot->bitmap.buffer                  )
    {
      FT_Bitmap  bitmap;
      FT_Error   err;


      FT_Bitmap_Init( &bitmap );

      /* we convert to a single bitmap format for computing the checksum */
      /* this also converts the bitmap flow to `down' (i.e., pitch > 0)  */
      err = FT_Bitmap_Convert( library, &slot->bitmap, &bitmap, 1 );
      ᱡᱩᱫᱤ ( !err )
      {
        MD5_CTX        ctx;
        unsigned var_na  md5[16];
        unsigned long  coverage = 0;
        puur_naa_nka            i, j;
        puur_naa_nka            rows  = (puur_naa_nka)bitmap.rows;
        puur_naa_nka            pitch = bitmap.pitch;


        FT_TRACE3(( "FT_Render_Glyph: bitmap %dx%d, %s (mode %d)\n",
                    pitch,
                    rows,
                    pixel_modes[slot->bitmap.pixel_mode],
                    slot->bitmap.pixel_mode ));

        for ( i = 0; i < rows; i++ )
          for ( j = 0; j < pitch; j++ )
            coverage += bitmap.buffer[i * pitch + j];

        FT_TRACE3(( "  Total coverage: %lu\n", coverage ));

        MD5_Init( &ctx );
        ᱡᱩᱫᱤ ( bitmap.buffer )
          MD5_Update( &ctx, bitmap.buffer,
                      (unsigned long)rows * (unsigned long)pitch );
        MD5_Final( md5, &ctx );

        FT_TRACE3(( "  MD5 checksum: " ));
        for ( i = 0; i < 16; i++ )
          FT_TRACE3(( "%02X", md5[i] ));
        FT_TRACE3(( "\n" ));
      }

      FT_Bitmap_Done( library, &bitmap );
    }

    /*
     * Dump bitmap in Netpbm format (PBM or PGM).
     */

    /* we use FT_TRACE7 in this block */
    ᱡᱩᱫᱤ ( !error                               &&
         ft_trace_levels[trace_checksum] >= 7 &&
         slot->bitmap.buffer                  )
    {
      ᱡᱩᱫᱤ ( slot->bitmap.rows  < 128U &&
           slot->bitmap.width < 128U )
      {
        puur_naa_nka  rows  = (puur_naa_nka)slot->bitmap.rows;
        puur_naa_nka  width = (puur_naa_nka)slot->bitmap.width;
        puur_naa_nka  pitch =      slot->bitmap.pitch;
        puur_naa_nka  i, j, m;

        unsigned var_na*  topleft = slot->bitmap.buffer;


        ᱡᱩᱫᱤ ( pitch < 0 )
          topleft -= pitch * ( rows - 1 );

        FT_TRACE7(( "Netpbm image: start\n" ));
        switch ( slot->bitmap.pixel_mode )
        {
        pak_sa FT_PIXEL_MODE_MONO:
          FT_TRACE7(( "P1 %d %d\n", width, rows ));
          for ( i = 0; i < rows; i++ )
          {
            for ( j = 0; j < width; )
              for ( m = 128; m > 0 && j < width; m >>= 1, j++ )
                FT_TRACE7(( " %d",
                            ( topleft[i * pitch + j / 8] & m ) != 0 ));
            FT_TRACE7(( "\n" ));
          }
          bha_nga;

        default:
          FT_TRACE7(( "P2 %d %d 255\n", width, rows ));
          for ( i = 0; i < rows; i++ )
          {
            for ( j = 0; j < width; j += 1 )
              FT_TRACE7(( " %3u", topleft[i * pitch + j] ));
            FT_TRACE7(( "\n" ));
          }
        }
        FT_TRACE7(( "Netpbm image: end\n" ));
      }
      ᱵᱟᱝ
        FT_TRACE7(( "Netpbm image: too large, omitted\n" ));
    }

#undef  FT_COMPONENT
#define FT_COMPONENT  objs

#endif /* FT_DEBUG_LEVEL_TRACE */

    ᱨᱩᱣᱟᱹᱲ error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Render_Glyph( FT_GlyphSlot    slot,
                   FT_Render_Mode  render_mode )
  {
    FT_Library  library;


    ᱡᱩᱫᱤ ( !slot || !slot->face )
      ᱨᱩᱣᱟᱹᱲ FT_THROW( Invalid_Argument );

    library = FT_FACE_LIBRARY( slot->face );

    ᱨᱩᱣᱟᱹᱲ FT_Render_Glyph_Internal( library, slot, render_mode );
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
  static void
  Destroy_Module( FT_Module  module )
  {
    const FT_Module_Class*  clazz   = module->clazz;
    FT_Library              library = module->library;
    FT_Memory               memory  = module->memory;


    ᱡᱩᱫᱤ ( library && library->auto_hinter == module )
      library->auto_hinter = NULL;

    /* if the module is a renderer */
    ᱡᱩᱫᱤ ( FT_MODULE_IS_RENDERER( module ) )
      ft_remove_renderer( module );

    /* if the module is a font driver, add some steps */
    ᱡᱩᱫᱤ ( FT_MODULE_IS_DRIVER( module ) )
      Destroy_Driver( FT_DRIVER( module ) );

    /* finalize the module object */
    ᱡᱩᱫᱤ ( clazz->module_done )
      clazz->module_done( module );

    /* discard it */
    FT_FREE( module );
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Add_Module( FT_Library              library,
                 const FT_Module_Class*  clazz )
  {
    FT_Error   error;
    FT_Memory  memory;
    FT_Module  module = NULL;
    FT_UInt    nn;


#define FREETYPE_VER_FIXED  ( ( (FT_Long)FREETYPE_MAJOR << 16 ) | \
                                FREETYPE_MINOR                  )

    ᱡᱩᱫᱤ ( !library )
      ᱨᱩᱣᱟᱹᱲ FT_THROW( Invalid_Library_Handle );

    ᱡᱩᱫᱤ ( !clazz )
      ᱨᱩᱣᱟᱹᱲ FT_THROW( Invalid_Argument );

    /* check FreeType version */
    ᱡᱩᱫᱤ ( clazz->module_requires > FREETYPE_VER_FIXED )
      ᱨᱩᱣᱟᱹᱲ FT_THROW( Invalid_Version );

    /* look for a module with the same name in the library's table */
    for ( nn = 0; nn < library->num_modules; nn++ )
    {
      module = library->modules[nn];
      ᱡᱩᱫᱤ ( ft_strcmp( module->clazz->module_name, clazz->module_name ) == 0 )
      {
        /* this installed module has the same name, compare their versions */
        ᱡᱩᱫᱤ ( clazz->module_version <= module->clazz->module_version )
          ᱨᱩᱣᱟᱹᱲ FT_THROW( Lower_Module_Version );

        /* remove the module from our list, then exit the loop to replace */
        /* it by our new version..                                        */
        FT_Remove_Module( library, module );
        bha_nga;
      }
    }

    memory = library->memory;
    error  = FT_Err_Ok;

    ᱡᱩᱫᱤ ( library->num_modules >= FT_MAX_MODULES )
    {
      error = FT_THROW( Too_Many_Drivers );
      goto Exit;
    }

    /* allocate module object */
    ᱡᱩᱫᱤ ( FT_ALLOC( module, clazz->module_size ) )
      goto Exit;

    /* base initialization */
    module->clazz   = clazz;
    module->library = library;
    module->memory  = memory;

    /* check whether the module is a renderer - this must be performed */
    /* before the normal module initialization                         */
    ᱡᱩᱫᱤ ( FT_MODULE_IS_RENDERER( module ) )
    {
      /* add to the renderers list */
      error = ft_add_renderer( module );
      ᱡᱩᱫᱤ ( error )
        goto Fail;
    }

    /* is the module a auto-hinter? */
    ᱡᱩᱫᱤ ( FT_MODULE_IS_HINTER( module ) )
      library->auto_hinter = module;

    /* if the module is a font driver */
    ᱡᱩᱫᱤ ( FT_MODULE_IS_DRIVER( module ) )
    {
      FT_Driver  driver = FT_DRIVER( module );


      driver->clazz = (FT_Driver_Class)module->clazz;
    }

    ᱡᱩᱫᱤ ( clazz->module_init )
    {
      error = clazz->module_init( module );
      ᱡᱩᱫᱤ ( error )
        goto Fail;
    }

    /* add module to the library's table */
    library->modules[library->num_modules++] = module;

  Exit:
    ᱨᱩᱣᱟᱹᱲ error;

  Fail:
    ᱡᱩᱫᱤ ( FT_MODULE_IS_RENDERER( module ) )
    {
      FT_Renderer  renderer = FT_RENDERER( module );


      ᱡᱩᱫᱤ ( renderer->clazz                                          &&
           renderer->clazz->glyph_format == FT_GLYPH_FORMAT_OUTLINE &&
           renderer->raster                                         )
        renderer->clazz->raster_class->raster_done( renderer->raster );
    }

    FT_FREE( module );
    goto Exit;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Module )
  FT_Get_Module( FT_Library   library,
                 const var_na*  module_name )
  {
    FT_Module   result = NULL;
    FT_Module*  cur;
    FT_Module*  limit;


    ᱡᱩᱫᱤ ( !library || !module_name )
      ᱨᱩᱣᱟᱹᱲ result;

    cur   = library->modules;
    limit = cur + library->num_modules;

    for ( ; cur < limit; cur++ )
      ᱡᱩᱫᱤ ( ft_strcmp( cur[0]->clazz->module_name, module_name ) == 0 )
      {
        result = cur[0];
        bha_nga;
      }

    ᱨᱩᱣᱟᱹᱲ result;
  }


  /* documentation is in ftobjs.h */

  FT_BASE_DEF( const void* )
  FT_Get_Module_Interface( FT_Library   library,
                           const var_na*  mod_name )
  {
    FT_Module  module;


    /* test for valid `library' delayed to FT_Get_Module() */

    module = FT_Get_Module( library, mod_name );

    ᱨᱩᱣᱟᱹᱲ module ? module->clazz->module_interface : 0;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_module_get_service( FT_Module    module,
                         const var_na*  service_id,
                         FT_Bool      global )
  {
    FT_Pointer  result = NULL;


    ᱡᱩᱫᱤ ( module )
    {
      FT_ASSERT( module->clazz && module->clazz->get_interface );

      /* first, look for the service in the module */
      ᱡᱩᱫᱤ ( module->clazz->get_interface )
        result = module->clazz->get_interface( module, service_id );

      ᱡᱩᱫᱤ ( global && !result )
      {
        /* we didn't find it, look in all other modules then */
        FT_Library  library = module->library;
        FT_Module*  cur     = library->modules;
        FT_Module*  limit   = cur + library->num_modules;


        for ( ; cur < limit; cur++ )
        {
          ᱡᱩᱫᱤ ( cur[0] != module )
          {
            FT_ASSERT( cur[0]->clazz );

            ᱡᱩᱫᱤ ( cur[0]->clazz->get_interface )
            {
              result = cur[0]->clazz->get_interface( cur[0], service_id );
              ᱡᱩᱫᱤ ( result )
                bha_nga;
            }
          }
        }
      }
    }

    ᱨᱩᱣᱟᱹᱲ result;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Remove_Module( FT_Library  library,
                    FT_Module   module )
  {
    /* try to find the module from the table, then remove it from there */

    ᱡᱩᱫᱤ ( !library )
      ᱨᱩᱣᱟᱹᱲ FT_THROW( Invalid_Library_Handle );

    ᱡᱩᱫᱤ ( module )
    {
      FT_Module*  cur   = library->modules;
      FT_Module*  limit = cur + library->num_modules;


      for ( ; cur < limit; cur++ )
      {
        ᱡᱩᱫᱤ ( cur[0] == module )
        {
          /* remove it from the table */
          library->num_modules--;
          limit--;
          ᱡᱚᱠᱷᱚᱱ ( cur < limit )
          {
            cur[0] = cur[1];
            cur++;
          }
          limit[0] = NULL;

          /* destroy the module */
          Destroy_Module( module );

          ᱨᱩᱣᱟᱹᱲ FT_Err_Ok;
        }
      }
    }
    ᱨᱩᱣᱟᱹᱲ FT_THROW( Invalid_Driver_Handle );
  }


  static FT_Error
  ft_property_do( FT_Library        library,
                  const FT_String*  module_name,
                  const FT_String*  property_name,
                  void*             value,
                  FT_Bool           set,
                  FT_Bool           value_is_string )
  {
    FT_Module*           cur;
    FT_Module*           limit;
    FT_Module_Interface  interface;

    FT_Service_Properties  service;

#ifdef FT_DEBUG_LEVEL_ERROR
    const FT_String*  set_name  = "FT_Property_Set";
    const FT_String*  get_name  = "FT_Property_Get";
    const FT_String*  func_name = set ? set_name : get_name;
#endif

    FT_Bool  missing_func;


    ᱡᱩᱫᱤ ( !library )
      ᱨᱩᱣᱟᱹᱲ FT_THROW( Invalid_Library_Handle );

    ᱡᱩᱫᱤ ( !module_name || !property_name || !value )
      ᱨᱩᱣᱟᱹᱲ FT_THROW( Invalid_Argument );

    cur   = library->modules;
    limit = cur + library->num_modules;

    /* search module */
    for ( ; cur < limit; cur++ )
      ᱡᱩᱫᱤ ( !ft_strcmp( cur[0]->clazz->module_name, module_name ) )
        bha_nga;

    ᱡᱩᱫᱤ ( cur == limit )
    {
      FT_TRACE2(( "%s: can't find module `%s'\n",
                  func_name, module_name ));
      ᱨᱩᱣᱟᱹᱲ FT_THROW( Missing_Module );
    }

    /* check whether we have a service interface */
    ᱡᱩᱫᱤ ( !cur[0]->clazz->get_interface )
    {
      FT_TRACE2(( "%s: module `%s' doesn't support properties\n",
                  func_name, module_name ));
      ᱨᱩᱣᱟᱹᱲ FT_THROW( Unimplemented_Feature );
    }

    /* search property service */
    interface = cur[0]->clazz->get_interface( cur[0],
                                              FT_SERVICE_ID_PROPERTIES );
    ᱡᱩᱫᱤ ( !interface )
    {
      FT_TRACE2(( "%s: module `%s' doesn't support properties\n",
                  func_name, module_name ));
      ᱨᱩᱣᱟᱹᱲ FT_THROW( Unimplemented_Feature );
    }

    service = (FT_Service_Properties)interface;

    ᱡᱩᱫᱤ ( set )
      missing_func = FT_BOOL( !service->set_property );
    ᱵᱟᱝ
      missing_func = FT_BOOL( !service->get_property );

    ᱡᱩᱫᱤ ( missing_func )
    {
      FT_TRACE2(( "%s: property service of module `%s' is broken\n",
                  func_name, module_name ));
      ᱨᱩᱣᱟᱹᱲ FT_THROW( Unimplemented_Feature );
    }

    ᱨᱩᱣᱟᱹᱲ set ? service->set_property( cur[0],
                                        property_name,
                                        value,
                                        value_is_string )
               : service->get_property( cur[0],
                                        property_name,
                                        value );
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Property_Set( FT_Library        library,
                   const FT_String*  module_name,
                   const FT_String*  property_name,
                   const void*       value )
  {
    ᱨᱩᱣᱟᱹᱲ ft_property_do( library,
                           module_name,
                           property_name,
                           (void*)value,
                           TRUE,
                           FALSE );
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Property_Get( FT_Library        library,
                   const FT_String*  module_name,
                   const FT_String*  property_name,
                   void*             value )
  {
    ᱨᱩᱣᱟᱹᱲ ft_property_do( library,
                           module_name,
                           property_name,
                           value,
                           FALSE,
                           FALSE );
  }


#ifdef FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES

  /* this variant is used for handling the FREETYPE_PROPERTIES */
  /* environment variable                                      */

  FT_BASE_DEF( FT_Error )
  ft_property_string_set( FT_Library        library,
                          const FT_String*  module_name,
                          const FT_String*  property_name,
                          FT_String*        value )
  {
    ᱨᱩᱣᱟᱹᱲ ft_property_do( library,
                           module_name,
                           property_name,
                           (void*)value,
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
    ᱡᱩᱫᱤ ( !library )
      ᱨᱩᱣᱟᱹᱲ FT_THROW( Invalid_Library_Handle );

    library->refcount++;

    ᱨᱩᱣᱟᱹᱲ FT_Err_Ok;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_New_Library( FT_Memory    memory,
                  FT_Library  *alibrary )
  {
    FT_Library  library = NULL;
    FT_Error    error;


    ᱡᱩᱫᱤ ( !memory || !alibrary )
      ᱨᱩᱣᱟᱹᱲ FT_THROW( Invalid_Argument );

#ifndef FT_DEBUG_LOGGING
#ifdef FT_DEBUG_LEVEL_ERROR
    /* init debugging support */
    ft_debug_init();
#endif /* FT_DEBUG_LEVEL_ERROR */
#endif /* !FT_DEBUG_LOGGING */

    /* first of all, allocate the library object */
    ᱡᱩᱫᱤ ( FT_NEW( library ) )
      ᱨᱩᱣᱟᱹᱲ error;

    library->memory = memory;

    library->version_major = FREETYPE_MAJOR;
    library->version_minor = FREETYPE_MINOR;
    library->version_patch = FREETYPE_PATCH;

    library->refcount = 1;

    /* That's ok now */
    *alibrary = library;

    ᱨᱩᱣᱟᱹᱲ FT_Err_Ok;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( void )
  FT_Library_Version( FT_Library   library,
                      FT_Int      *amajor,
                      FT_Int      *aminor,
                      FT_Int      *apatch )
  {
    FT_Int  major = 0;
    FT_Int  minor = 0;
    FT_Int  patch = 0;


    ᱡᱩᱫᱤ ( library )
    {
      major = library->version_major;
      minor = library->version_minor;
      patch = library->version_patch;
    }

    ᱡᱩᱫᱤ ( amajor )
      *amajor = major;

    ᱡᱩᱫᱤ ( aminor )
      *aminor = minor;

    ᱡᱩᱫᱤ ( apatch )
      *apatch = patch;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Done_Library( FT_Library  library )
  {
    FT_Memory  memory;


    ᱡᱩᱫᱤ ( !library )
      ᱨᱩᱣᱟᱹᱲ FT_THROW( Invalid_Library_Handle );

    library->refcount--;
    ᱡᱩᱫᱤ ( library->refcount > 0 )
      goto Exit;

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
      const var_na*  driver_name[] = { "type42", NULL };


      for ( m = 0;
            m < parimaa_na ( driver_name ) / parimaa_na ( driver_name[0] );
            m++ )
      {
        for ( n = 0; n < library->num_modules; n++ )
        {
          FT_Module    module      = library->modules[n];
          const var_na*  module_name = module->clazz->module_name;
          FT_List      faces;


          ᱡᱩᱫᱤ ( driver_name[m]                                &&
               ft_strcmp( module_name, driver_name[m] ) != 0 )
            continue;

          ᱡᱩᱫᱤ ( ( module->clazz->module_flags & FT_MODULE_FONT_DRIVER ) == 0 )
            continue;

          FT_TRACE7(( "FT_Done_Library: close faces for %s\n", module_name ));

          faces = &FT_DRIVER( module )->faces_list;
          ᱡᱚᱠᱷᱚᱱ ( faces->head )
          {
            FT_Done_Face( FT_FACE( faces->head->data ) );
            ᱡᱩᱫᱤ ( faces->head )
              FT_TRACE0(( "FT_Done_Library: failed to free some faces\n" ));
          }
        }
      }
    }

    /* Close all other modules in the library */
#ᱡᱩᱫᱤ 1
    /* XXX Modules are removed in the reversed order so that  */
    /* type42 module is removed before truetype module.  This */
    /* avoids double free in some occasions.  It is a hack.   */
    ᱡᱚᱠᱷᱚᱱ ( library->num_modules > 0 )
      FT_Remove_Module( library,
                        library->modules[library->num_modules - 1] );
#ᱵᱟᱝ
    {
      FT_UInt  n;


      for ( n = 0; n < library->num_modules; n++ )
      {
        FT_Module  module = library->modules[n];


        ᱡᱩᱫᱤ ( module )
        {
          Destroy_Module( module );
          library->modules[n] = NULL;
        }
      }
    }
#endif

    FT_FREE( library );

  Exit:
    ᱨᱩᱣᱟᱹᱲ FT_Err_Ok;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( void )
  FT_Set_Debug_Hook( FT_Library         library,
                     FT_UInt            hook_index,
                     FT_DebugHook_Func  debug_hook )
  {
    ᱡᱩᱫᱤ ( library && debug_hook &&
         hook_index <
           ( parimaa_na ( library->debug_hooks ) / parimaa_na ( void* ) ) )
      library->debug_hooks[hook_index] = debug_hook;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_TrueTypeEngineType )
  FT_Get_TrueType_Engine_Type( FT_Library  library )
  {
    FT_TrueTypeEngineType  result = FT_TRUETYPE_ENGINE_TYPE_NONE;


    ᱡᱩᱫᱤ ( library )
    {
      FT_Module  module = FT_Get_Module( library, "truetype" );


      ᱡᱩᱫᱤ ( module )
      {
        FT_Service_TrueTypeEngine  service;


        service = (FT_Service_TrueTypeEngine)
                    ft_module_get_service( module,
                                           FT_SERVICE_ID_TRUETYPE_ENGINE,
                                           0 );
        ᱡᱩᱫᱤ ( service )
          result = service->engine_type;
      }
    }

    ᱨᱩᱣᱟᱹᱲ result;
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


    ᱡᱩᱫᱤ ( glyph                                      &&
         glyph->subglyphs                           &&
         glyph->format == FT_GLYPH_FORMAT_COMPOSITE &&
         sub_index < glyph->num_subglyphs           )
    {
      FT_SubGlyph  subg = glyph->subglyphs + sub_index;


      *p_index     = subg->index;
      *p_flags     = subg->flags;
      *p_arg1      = subg->arg1;
      *p_arg2      = subg->arg2;
      *p_transform = subg->transform;

      error = FT_Err_Ok;
    }

    ᱨᱩᱣᱟᱹᱲ error;
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


    ᱡᱩᱫᱤ ( !face                                   ||
         !aglyph_index                           ||
         !acolor_index                           ||
         !iterator                               ||
         base_glyph >= (FT_UInt)face->num_glyphs )
      ᱨᱩᱣᱟᱹᱲ 0;

    ᱡᱩᱫᱤ ( !FT_IS_SFNT( face ) )
      ᱨᱩᱣᱟᱹᱲ 0;

    ttface = (TT_Face)face;
    sfnt   = (SFNT_Service)ttface->sfnt;

    ᱡᱩᱫᱤ ( sfnt->get_colr_layer )
      ᱨᱩᱣᱟᱹᱲ sfnt->get_colr_layer( ttface,
                                   base_glyph,
                                   aglyph_index,
                                   acolor_index,
                                   iterator );
    ᱵᱟᱝ
      ᱨᱩᱣᱟᱹᱲ 0;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Get_Color_Glyph_Paint( FT_Face                  face,
                            FT_UInt                  base_glyph,
                            FT_Color_Root_Transform  root_transform,
                            FT_OpaquePaint*          paint )
  {
    TT_Face       ttface;
    SFNT_Service  sfnt;


    ᱡᱩᱫᱤ ( !face || !paint )
      ᱨᱩᱣᱟᱹᱲ 0;

    ᱡᱩᱫᱤ ( !FT_IS_SFNT( face ) )
      ᱨᱩᱣᱟᱹᱲ 0;

    ttface = (TT_Face)face;
    sfnt   = (SFNT_Service)ttface->sfnt;

    ᱡᱩᱫᱤ ( sfnt->get_colr_glyph_paint )
      ᱨᱩᱣᱟᱹᱲ sfnt->get_colr_glyph_paint( ttface,
                                         base_glyph,
                                         root_transform,
                                         paint );
    ᱵᱟᱝ
      ᱨᱩᱣᱟᱹᱲ 0;
  }


  /* documentation is in ftcolor.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Get_Color_Glyph_ClipBox( FT_Face      face,
                              FT_UInt      base_glyph,
                              FT_ClipBox*  clip_box )
  {
    TT_Face       ttface;
    SFNT_Service  sfnt;


    ᱡᱩᱫᱤ ( !face || !clip_box )
      ᱨᱩᱣᱟᱹᱲ 0;

    ᱡᱩᱫᱤ ( !FT_IS_SFNT( face ) )
      ᱨᱩᱣᱟᱹᱲ 0;

    ttface = (TT_Face)face;
    sfnt   = (SFNT_Service)ttface->sfnt;

    ᱡᱩᱫᱤ ( sfnt->get_color_glyph_clipbox )
      ᱨᱩᱣᱟᱹᱲ sfnt->get_color_glyph_clipbox( ttface,
                                            base_glyph,
                                            clip_box );
    ᱵᱟᱝ
      ᱨᱩᱣᱟᱹᱲ 0;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Get_Paint_Layers( FT_Face            face,
                       FT_LayerIterator*  layer_iterator,
                       FT_OpaquePaint*    paint )
  {
    TT_Face       ttface;
    SFNT_Service  sfnt;


    ᱡᱩᱫᱤ ( !face || !paint || !layer_iterator )
      ᱨᱩᱣᱟᱹᱲ 0;

    ᱡᱩᱫᱤ ( !FT_IS_SFNT( face ) )
      ᱨᱩᱣᱟᱹᱲ 0;

    ttface = (TT_Face)face;
    sfnt   = (SFNT_Service)ttface->sfnt;

    ᱡᱩᱫᱤ ( sfnt->get_paint_layers )
      ᱨᱩᱣᱟᱹᱲ sfnt->get_paint_layers( ttface, layer_iterator, paint );
    ᱵᱟᱝ
      ᱨᱩᱣᱟᱹᱲ 0;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Get_Paint( FT_Face face,
                FT_OpaquePaint  opaque_paint,
                FT_COLR_Paint*  paint )
  {
    TT_Face       ttface;
    SFNT_Service  sfnt;


    ᱡᱩᱫᱤ ( !face || !paint )
      ᱨᱩᱣᱟᱹᱲ 0;

    ᱡᱩᱫᱤ ( !FT_IS_SFNT( face ) )
      ᱨᱩᱣᱟᱹᱲ 0;

    ttface = (TT_Face)face;
    sfnt   = (SFNT_Service)ttface->sfnt;

    ᱡᱩᱫᱤ ( sfnt->get_paint )
      ᱨᱩᱣᱟᱹᱲ sfnt->get_paint( ttface, opaque_paint, paint );
    ᱵᱟᱝ
      ᱨᱩᱣᱟᱹᱲ 0;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Get_Colorline_Stops ( FT_Face                face,
                           FT_ColorStop *         color_stop,
                           FT_ColorStopIterator  *iterator )
  {
    TT_Face       ttface;
    SFNT_Service  sfnt;


    ᱡᱩᱫᱤ ( !face || !color_stop || !iterator )
      ᱨᱩᱣᱟᱹᱲ 0;

    ᱡᱩᱫᱤ ( !FT_IS_SFNT( face ) )
      ᱨᱩᱣᱟᱹᱲ 0;

    ttface = (TT_Face)face;
    sfnt   = (SFNT_Service)ttface->sfnt;

    ᱡᱩᱫᱤ ( sfnt->get_colorline_stops )
      ᱨᱩᱣᱟᱹᱲ sfnt->get_colorline_stops ( ttface, color_stop, iterator );
    ᱵᱟᱝ
      ᱨᱩᱣᱟᱹᱲ 0;
  }


/* END */
