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


#समावेश <freetype/ftlist.h>
#समावेश <freetype/ftoutln.h>
#समावेश <freetype/ftfntfmt.h>
#समावेश <freetype/otsvg.h>

#समावेश <freetype/internal/ftvalid.h>
#समावेश <freetype/internal/ftobjs.h>
#समावेश <freetype/internal/ftdebug.h>
#समावेश <freetype/internal/ftrfork.h>
#समावेश <freetype/internal/ftstream.h>
#समावेश <freetype/internal/sfnt.h>          /* for SFNT_Load_Table_Func */
#समावेश <freetype/internal/psaux.h>         /* for PS_Driver            */
#समावेश <freetype/internal/svginterface.h>

#समावेश <freetype/tttables.h>
#समावेश <freetype/tttags.h>
#समावेश <freetype/ttnameid.h>

#समावेश <freetype/internal/services/svprop.h>
#समावेश <freetype/internal/services/svsfnt.h>
#समावेश <freetype/internal/services/svpostnm.h>
#समावेश <freetype/internal/services/svgldict.h>
#समावेश <freetype/internal/services/svttcmap.h>
#समावेश <freetype/internal/services/svkern.h>
#समावेश <freetype/internal/services/svtteng.h>

#अगर_घोषित TT_CONFIG_OPTION_VARC
#समावेश <freetype/internal/services/svvarc.h>
#पूर्ण_अगर

#समावेश <freetype/ftdriver.h>

#अगर_घोषित FT_CONFIG_OPTION_MAC_FONTS
#समावेश "ftbase.h"
#पूर्ण_अगर


#अगर_घोषित FT_DEBUG_LEVEL_TRACE

#समावेश <freetype/ftbitmap.h>

#अगर defined( _MSC_VER )      /* Visual C++ (and Intel C++)   */
  /* We disable the warning `conversion from XXX to YYY,     */
  /* possible loss of data' in order to compile cleanly with */
  /* the maximum level of warnings: `md5.c' is non-FreeType  */
  /* code, and it gets used during development builds only.  */
#आशय warning( धकेलो )
#आशय warning( disable : 4244 )
#पूर्ण_अगर /* _MSC_VER */

  /* It's easiest to include `md5.c' directly.  However, since OpenSSL */
  /* also provides the same functions, there might be conflicts if     */
  /* both FreeType and OpenSSL are built as static libraries.  For     */
  /* this reason, we put the MD5 stuff into the `FT_' namespace.       */
#घोषणा MD5_u32plus  FT_MD5_u32plus
#घोषणा MD5_CTX      FT_MD5_CTX
#घोषणा MD5_Init     FT_MD5_Init
#घोषणा MD5_Update   FT_MD5_Update
#घोषणा MD5_Final    FT_MD5_Final

#अघोषित  HAVE_OPENSSL

#समावेश "md5.c"

#अगर defined( _MSC_VER )
#आशय warning( निकालो )
#पूर्ण_अगर

  /* This array must stay in sync with the @FT_Pixel_Mode enumeration */
  /* (in file `ftimage.h`).                                           */

  अटल स्थिर अक्षर* स्थिर  pixel_modes[] =
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

#पूर्ण_अगर /* FT_DEBUG_LEVEL_TRACE */


#घोषणा GRID_FIT_METRICS


  /* forward declaration */
  अटल FT_Error
  ft_open_face_internal( FT_Library           library,
                         स्थिर FT_Open_Args*  args,
                         FT_Long              face_सूचकांक,
                         FT_Face             *aface,
                         FT_Bool              test_mac_fonts );


  FT_BASE_DEF( FT_Pointer )
  ft_service_list_lookup( FT_ServiceDesc  service_descriptors,
                          स्थिर अक्षर*     service_id )
  {
    FT_Pointer      परिणाम = शून्य;
    FT_ServiceDesc  desc   = service_descriptors;


    अगर ( desc && service_id )
    {
      क्रम ( ; desc->serv_id != शून्य; desc++ )
      {
        अगर ( ft_strcmp( desc->serv_id, service_id ) == 0 )
        {
          परिणाम = (FT_Pointer)desc->serv_तथ्य;
          अवरोध;
        }
      }
    }

    वापस परिणाम;
  }


  FT_BASE_DEF( व्योम )
  ft_validator_init( FT_Validator        valid,
                     स्थिर FT_Byte*      base,
                     स्थिर FT_Byte*      limit,
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

    वापस -1;
  }


  FT_BASE_DEF( व्योम )
  ft_validator_error( FT_Validator  valid,
                      FT_Error      error )
  {
    /* since the cast below also disables the compiler's */
    /* type check, we introduce a dummy variable, which  */
    /* will be optimized away                            */
    अस्थिर ft_jmp_buf* jump_बफर = &valid->jump_बफर;


    valid->error = error;

    /* throw away volatileness; use `jump_buffer' or the  */
    /* compiler may warn about an unused local variable   */
    ft_longjmp( *(ft_jmp_buf*) jump_बफर, 1 );
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
                 स्थिर FT_Open_Args*  args,
                 FT_Stream           *astream )
  {
    FT_Error   error;
    FT_Memory  memory;
    FT_Stream  stream = शून्य;
    FT_UInt    mode;


    *astream = शून्य;

    अगर ( !library )
      वापस FT_THROW( Invalid_Library_Handle );

    अगर ( !args )
      वापस FT_THROW( Invalid_Argument );

    memory = library->memory;
    mode   = args->flags &
               ( FT_OPEN_MEMORY | FT_OPEN_STREAM | FT_OPEN_PATHNAME );

    अगर ( mode == FT_OPEN_MEMORY )
    {
      /* create a memory-based stream */
      अगर ( FT_NEW( stream ) )
        जाओ Exit;

      FT_Stream_OpenMemory( stream,
                            (स्थिर FT_Byte*)args->memory_base,
                            (FT_ULong)args->memory_आकार );
      stream->memory = memory;
    }

#अगर_अघोषित FT_CONFIG_OPTION_DISABLE_STREAM_SUPPORT

    अन्यथा अगर ( mode == FT_OPEN_PATHNAME )
    {
      /* create a normal system stream */
      अगर ( FT_NEW( stream ) )
        जाओ Exit;

      stream->memory = memory;
      error = FT_Stream_Open( stream, args->pathname );
      अगर ( error )
        FT_FREE( stream );
    }
    अन्यथा अगर ( ( mode == FT_OPEN_STREAM ) && args->stream )
    {
      /* use an existing, user-provided stream */

      /* in this case, we do not need to allocate a new stream object */
      /* since the caller is responsible for closing it himself       */
      stream         = args->stream;
      stream->memory = memory;
      error          = FT_Err_Ok;
    }

#पूर्ण_अगर

    अन्यथा
    {
      error = FT_THROW( Invalid_Argument );
      अगर ( ( args->flags & FT_OPEN_STREAM ) && args->stream )
        FT_Stream_Close( args->stream );
    }

    अगर ( !error )
      *astream       = stream;

  Exit:
    वापस error;
  }


  FT_BASE_DEF( व्योम )
  FT_Stream_Free( FT_Stream  stream,
                  FT_Int     external )
  {
    अगर ( stream )
    {
      FT_Memory  memory = stream->memory;


      FT_Stream_Close( stream );

      अगर ( !external )
        FT_FREE( stream );
    }
  }


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#अघोषित  FT_COMPONENT
#घोषणा FT_COMPONENT  objs


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


  अटल FT_Error
  ft_glyphslot_init( FT_GlyphSlot  slot )
  {
    FT_Driver         driver   = slot->face->driver;
    FT_Driver_Class   clazz    = driver->clazz;
    FT_Memory         memory   = driver->मूल.memory;
    FT_Error          error    = FT_Err_Ok;
    FT_Slot_Internal  internal = शून्य;


    slot->library = driver->मूल.library;

    अगर ( FT_NEW( internal ) )
      जाओ Exit;

    slot->internal = internal;

    अगर ( FT_DRIVER_USES_OUTLINES( driver ) )
      error = FT_GlyphLoader_New( memory, &internal->loader );

    अगर ( !error && clazz->init_slot )
      error = clazz->init_slot( slot );

#अगर_घोषित FT_CONFIG_OPTION_SVG
    /* if SVG table exists, allocate the space in `slot->other` */
    अगर ( slot->face->face_flags & FT_FACE_FLAG_SVG )
    {
      FT_SVG_Document  document = शून्य;


      अगर ( FT_NEW( document ) )
        जाओ Exit;
      slot->other = document;
    }
#पूर्ण_अगर

  Exit:
    वापस error;
  }


  FT_BASE_DEF( व्योम )
  ft_glyphslot_free_bitmap( FT_GlyphSlot  slot )
  {
    अगर ( slot->internal && ( slot->internal->flags & FT_GLYPH_OWN_BITMAP ) )
    {
      FT_Memory  memory = FT_FACE_MEMORY( slot->face );


      FT_FREE( slot->bitmap.बफर );
      slot->internal->flags &= ~FT_GLYPH_OWN_BITMAP;
    }
    अन्यथा
    {
      /* assume that the bitmap buffer was stolen or not */
      /* allocated from the heap                         */
      slot->bitmap.बफर = शून्य;
    }
  }


  /* overflow-resistant presetting of bitmap position and dimensions; */
  /* also check whether the size is too large for rendering           */
  FT_BASE_DEF( FT_Bool )
  ft_glyphslot_preset_bitmap( FT_GlyphSlot      slot,
                              FT_Render_Mode    mode,
                              स्थिर FT_Vector*  origin )
  {
    FT_Outline*  outline = &slot->outline;
    FT_Bitmap*   bitmap  = &slot->bitmap;

    FT_Pixel_Mode  pixel_mode;

    FT_BBox  cbox, pbox;
    FT_Pos   x_shift = 0;
    FT_Pos   y_shift = 0;
    FT_Pos   x_बायाँ, y_शीर्ष;
    FT_Pos   width, height, pitch;
    FT_Bool  ret;


    अगर ( slot->format == FT_GLYPH_FORMAT_SVG )
    {
      FT_Module    module;
      SVG_Service  svg_service;


      module      = FT_Get_Module( slot->library, "ot-svg" );
      svg_service = (SVG_Service)module->clazz->module_interface;

      वापस (FT_Bool)svg_service->preset_slot( module, slot, FALSE );
    }
    अन्यथा अगर ( slot->format != FT_GLYPH_FORMAT_OUTLINE )
      वापस 1;

    अगर ( origin )
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

    चयन ( mode )
    {
    हाल FT_RENDER_MODE_MONO:
      pixel_mode = FT_PIXEL_MODE_MONO;
#अगर 1
      /* x */

      /* undocumented but confirmed: bbox values get rounded;    */
      /* we do asymmetric rounding so that the center of a pixel */
      /* gets always included                                    */

      pbox.xMin += ( cbox.xMin + 31 ) >> 6;
      pbox.xMax += ( cbox.xMax + 32 ) >> 6;

      /* if the bbox collapsed, we add a pixel based on the total */
      /* rounding remainder to cover most of the original cbox    */

      अगर ( pbox.xMin == pbox.xMax )
      {
        अगर ( ( ( cbox.xMin + 31 ) & 63 ) - 31 +
             ( ( cbox.xMax + 32 ) & 63 ) - 32 < 0 )
          pbox.xMin -= 1;
        अन्यथा
          pbox.xMax += 1;
      }

      /* y */

      pbox.yMin += ( cbox.yMin + 31 ) >> 6;
      pbox.yMax += ( cbox.yMax + 32 ) >> 6;

      अगर ( pbox.yMin == pbox.yMax )
      {
        अगर ( ( ( cbox.yMin + 31 ) & 63 ) - 31 +
             ( ( cbox.yMax + 32 ) & 63 ) - 32 < 0 )
          pbox.yMin -= 1;
        अन्यथा
          pbox.yMax += 1;
      }

      अवरोध;
#अन्यथा
      जाओ Adjust;
#पूर्ण_अगर

    हाल FT_RENDER_MODE_LCD:
      pixel_mode = FT_PIXEL_MODE_LCD;
      ft_lcd_padding( &cbox, slot, mode );
      जाओ Adjust;

    हाल FT_RENDER_MODE_LCD_V:
      pixel_mode = FT_PIXEL_MODE_LCD_V;
      ft_lcd_padding( &cbox, slot, mode );
      जाओ Adjust;

    हाल FT_RENDER_MODE_NORMAL:
    हाल FT_RENDER_MODE_LIGHT:
    शेष:
      pixel_mode = FT_PIXEL_MODE_GRAY;
    Adjust:
      pbox.xMin += cbox.xMin >> 6;
      pbox.yMin += cbox.yMin >> 6;
      pbox.xMax += ( cbox.xMax + 63 ) >> 6;
      pbox.yMax += ( cbox.yMax + 63 ) >> 6;
    }

    x_बायाँ = pbox.xMin;
    y_शीर्ष  = pbox.yMax;

    width  = pbox.xMax - pbox.xMin;
    height = pbox.yMax - pbox.yMin;

    /* Flag the width or height unsuitable for rendering.   */
    /* The limit is based on the ppem value when available. */
    /* FT_Renderer modules should check the return value.   */
    ret = FT_BOOL(     width >= 0x10000   ||    height >= 0x10000   ||
                   pbox.xMin < -0x1000000 || pbox.xMax >= 0x1000000 ||
                   pbox.yMin < -0x1000000 || pbox.yMax >= 0x1000000 ||
             ( slot->face                                         &&
               ( width  > 16 * slot->face->आकार->metrics.x_ppem ||
                 height > 16 * slot->face->आकार->metrics.y_ppem ) ) );

    अगर ( ret )
      FT_TRACE3(( "ft_glyphslot_preset_bitmap: [%ld %ld %ld %ld]\n",
                  pbox.xMin, pbox.yMin, pbox.xMax, pbox.yMax ));

    चयन ( pixel_mode )
    {
    हाल FT_PIXEL_MODE_MONO:
      pitch = ( ( width + 15 ) >> 4 ) << 1;
      अवरोध;

    हाल FT_PIXEL_MODE_LCD:
      width *= 3;
      pitch  = FT_PAD_CEIL( width, 4 );
      अवरोध;

    हाल FT_PIXEL_MODE_LCD_V:
      height *= 3;
      FALL_THROUGH;

    हाल FT_PIXEL_MODE_GRAY:
    शेष:
      pitch = width;
    }

    slot->bitmap_बायाँ = (FT_Int)x_बायाँ;
    slot->bitmap_शीर्ष  = (FT_Int)y_शीर्ष;

    bitmap->pixel_mode = (अचिन्हित अक्षर)pixel_mode;
    bitmap->अंक_grays  = 256;
    bitmap->width      = (अचिन्हित पूर्णांक)width;
    bitmap->rows       = (अचिन्हित पूर्णांक)height;
    bitmap->pitch      = pitch;

    वापस ret;
  }


  FT_BASE_DEF( व्योम )
  ft_glyphslot_set_bitmap( FT_GlyphSlot  slot,
                           FT_Byte*      बफर )
  {
    ft_glyphslot_free_bitmap( slot );

    slot->bitmap.बफर = बफर;

    FT_ASSERT( (slot->internal->flags & FT_GLYPH_OWN_BITMAP) == 0 );
  }


  FT_BASE_DEF( FT_Error )
  ft_glyphslot_alloc_bitmap( FT_GlyphSlot  slot )
  {
    FT_Memory  memory = FT_FACE_MEMORY( slot->face );
    FT_Error   error;


    अगर ( slot->internal->flags & FT_GLYPH_OWN_BITMAP )
      FT_FREE( slot->bitmap.बफर );
    अन्यथा
      slot->internal->flags |= FT_GLYPH_OWN_BITMAP;

    /* dimensions must be preset */
    FT_MEM_ALLOC_MULT( slot->bitmap.बफर,
                       slot->bitmap.rows,
                       slot->bitmap.pitch );
    वापस error;
  }


  अटल व्योम
  ft_glyphslot_clear( FT_GlyphSlot  slot )
  {
    /* free bitmap if needed */
    ft_glyphslot_free_bitmap( slot );

    /* clear all public fields in the glyph slot */
    slot->glyph_सूचकांक = 0;

    FT_ZERO( &slot->metrics );
    FT_ZERO( &slot->outline );

    slot->bitmap.width      = 0;
    slot->bitmap.rows       = 0;
    slot->bitmap.pitch      = 0;
    slot->bitmap.pixel_mode = 0;
    /* `slot->bitmap.buffer' has been handled by ft_glyphslot_free_bitmap */

    slot->bitmap_बायाँ   = 0;
    slot->bitmap_शीर्ष    = 0;
    slot->अंक_subglyphs = 0;
    slot->subglyphs     = शून्य;
    slot->control_तथ्य  = शून्य;
    slot->control_len   = 0;

#अगर_अघोषित FT_CONFIG_OPTION_SVG
    slot->other = शून्य;
#अन्यथा
    अगर ( !( slot->face->face_flags & FT_FACE_FLAG_SVG ) )
      slot->other = शून्य;
    अन्यथा
    {
      अगर ( slot->internal->flags & FT_GLYPH_OWN_GZIP_SVG )
      {
        FT_Memory        memory = slot->face->memory;
        FT_SVG_Document  doc    = (FT_SVG_Document)slot->other;


        FT_FREE( doc->svg_document );
        slot->internal->flags &= ~FT_GLYPH_OWN_GZIP_SVG;
      }
    }
#पूर्ण_अगर

    slot->format = FT_GLYPH_FORMAT_NONE;

    slot->linearHoriAdvance = 0;
    slot->linearVertAdvance = 0;
    slot->advance.x         = 0;
    slot->advance.y         = 0;
    slot->lsb_delta         = 0;
    slot->rsb_delta         = 0;
  }


  अटल व्योम
  ft_glyphslot_done( FT_GlyphSlot  slot )
  {
    FT_Driver        driver = slot->face->driver;
    FT_Driver_Class  clazz  = driver->clazz;
    FT_Memory        memory = driver->मूल.memory;

#अगर_घोषित FT_CONFIG_OPTION_SVG
    अगर ( slot->face->face_flags & FT_FACE_FLAG_SVG )
    {
      /* Free memory in case SVG was there.                          */
      /* `slot->internal` might be NULL in out-of-memory situations. */
      अगर ( slot->internal && slot->internal->flags & FT_GLYPH_OWN_GZIP_SVG )
      {
        FT_SVG_Document  doc = (FT_SVG_Document)slot->other;


        FT_FREE( doc->svg_document );

        slot->internal->flags &= ~FT_GLYPH_OWN_GZIP_SVG;
      }

      FT_FREE( slot->other );
    }
#पूर्ण_अगर

    अगर ( clazz->done_slot )
      clazz->done_slot( slot );

    /* free bitmap buffer if needed */
    ft_glyphslot_free_bitmap( slot );

    /* slot->internal might be NULL in out-of-memory situations */
    अगर ( slot->internal )
    {
      /* free glyph loader */
      अगर ( FT_DRIVER_USES_OUTLINES( driver ) )
      {
        FT_GlyphLoader_Done( slot->internal->loader );
        slot->internal->loader = शून्य;
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
    FT_GlyphSlot     slot = शून्य;


    अगर ( !face )
      वापस FT_THROW( Invalid_Face_Handle );

    अगर ( !face->driver )
      वापस FT_THROW( Invalid_Argument );

    driver = face->driver;
    clazz  = driver->clazz;
    memory = driver->मूल.memory;

    FT_TRACE4(( "FT_New_GlyphSlot: Creating new slot object\n" ));
    अगर ( !FT_ALLOC( slot, clazz->slot_object_आकार ) )
    {
      slot->face = face;

      error = ft_glyphslot_init( slot );
      अगर ( error )
      {
        ft_glyphslot_done( slot );
        FT_FREE( slot );
        जाओ Exit;
      }

      slot->अग्र  = face->glyph;
      face->glyph = slot;

      अगर ( aslot )
        *aslot = slot;
    }
    अन्यथा अगर ( aslot )
      *aslot = शून्य;


  Exit:
    FT_TRACE4(( "FT_New_GlyphSlot: Return 0x%x\n", error ));

    वापस error;
  }


  /* documentation is in ftobjs.h */

  FT_BASE_DEF( व्योम )
  FT_Done_GlyphSlot( FT_GlyphSlot  slot )
  {
    अगर ( slot )
    {
      FT_Driver     driver = slot->face->driver;
      FT_Memory     memory = driver->मूल.memory;
      FT_GlyphSlot  पूर्व;
      FT_GlyphSlot  cur;


      /* Remove slot from its parent face's list */
      पूर्व = शून्य;
      cur  = slot->face->glyph;

      जबतक ( cur )
      {
        अगर ( cur == slot )
        {
          अगर ( !पूर्व )
            slot->face->glyph = cur->अग्र;
          अन्यथा
            पूर्व->अग्र = cur->अग्र;

          /* finalize client-specific data */
          अगर ( slot->generic.finalizer )
            slot->generic.finalizer( slot );

          ft_glyphslot_done( slot );
          FT_FREE( slot );
          अवरोध;
        }
        पूर्व = cur;
        cur  = cur->अग्र;
      }
    }
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( व्योम )
  FT_Set_Transform( FT_Face     face,
                    FT_Matrix*  matrix,
                    FT_Vector*  delta )
  {
    FT_Face_Internal  internal;


    अगर ( !face )
      वापस;

    internal = face->internal;

    internal->transform_flags = 0;

    अगर ( !matrix || !FT_Matrix_Check( matrix ) )
    {
      internal->transform_matrix.xx = 0x10000L;
      internal->transform_matrix.xy = 0;
      internal->transform_matrix.yx = 0;
      internal->transform_matrix.yy = 0x10000L;

      matrix = &internal->transform_matrix;
    }
    अन्यथा
      internal->transform_matrix = *matrix;

    /* set transform_flags bit flag 0 if `matrix' isn't the identity */
    अगर ( ( matrix->xy | matrix->yx ) ||
         matrix->xx != 0x10000L      ||
         matrix->yy != 0x10000L      )
      internal->transform_flags |= 1;

    अगर ( !delta )
    {
      internal->transform_delta.x = 0;
      internal->transform_delta.y = 0;

      delta = &internal->transform_delta;
    }
    अन्यथा
      internal->transform_delta = *delta;

    /* set transform_flags bit flag 1 if `delta' isn't the null vector */
    अगर ( delta->x | delta->y )
      internal->transform_flags |= 2;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( व्योम )
  FT_Get_Transform( FT_Face     face,
                    FT_Matrix*  matrix,
                    FT_Vector*  delta )
  {
    FT_Face_Internal  internal;


    अगर ( !face )
      वापस;

    internal = face->internal;

    अगर ( matrix )
      *matrix = internal->transform_matrix;

    अगर ( delta )
      *delta = internal->transform_delta;
  }


  अटल FT_Renderer
  ft_lookup_glyph_renderer( FT_GlyphSlot  slot );


#अगर_घोषित GRID_FIT_METRICS
  अटल व्योम
  ft_glyphslot_grid_fit_metrics( FT_GlyphSlot  slot,
                                 FT_Bool       vertical )
  {
    FT_Glyph_Metrics*  metrics = &slot->metrics;
    FT_Pos             दायाँ, bottom;


    अगर ( vertical )
    {
      metrics->horiBearingX = FT_PIX_FLOOR( metrics->horiBearingX );
      metrics->horiBearingY = FT_PIX_CEIL_LONG( metrics->horiBearingY );

      दायाँ  = FT_PIX_CEIL_LONG( ADD_LONG( metrics->vertBearingX,
                                           metrics->width ) );
      bottom = FT_PIX_CEIL_LONG( ADD_LONG( metrics->vertBearingY,
                                           metrics->height ) );

      metrics->vertBearingX = FT_PIX_FLOOR( metrics->vertBearingX );
      metrics->vertBearingY = FT_PIX_FLOOR( metrics->vertBearingY );

      metrics->width  = SUB_LONG( दायाँ,
                                  metrics->vertBearingX );
      metrics->height = SUB_LONG( bottom,
                                  metrics->vertBearingY );
    }
    अन्यथा
    {
      metrics->vertBearingX = FT_PIX_FLOOR( metrics->vertBearingX );
      metrics->vertBearingY = FT_PIX_FLOOR( metrics->vertBearingY );

      दायाँ  = FT_PIX_CEIL_LONG( ADD_LONG( metrics->horiBearingX,
                                           metrics->width ) );
      bottom = FT_PIX_FLOOR( SUB_LONG( metrics->horiBearingY,
                                       metrics->height ) );

      metrics->horiBearingX = FT_PIX_FLOOR( metrics->horiBearingX );
      metrics->horiBearingY = FT_PIX_CEIL_LONG( metrics->horiBearingY );

      metrics->width  = SUB_LONG( दायाँ,
                                  metrics->horiBearingX );
      metrics->height = SUB_LONG( metrics->horiBearingY,
                                  bottom );
    }

    metrics->horiAdvance = FT_PIX_ROUND_LONG( metrics->horiAdvance );
    metrics->vertAdvance = FT_PIX_ROUND_LONG( metrics->vertAdvance );
  }
#पूर्ण_अगर /* GRID_FIT_METRICS */


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Load_Glyph( FT_Face   face,
                 FT_UInt   glyph_सूचकांक,
                 FT_Int32  load_flags )
  {
    FT_Error      error;
    FT_Driver     driver;
    FT_GlyphSlot  slot;
    FT_Library    library;
    FT_Bool       autohint = FALSE;
    FT_Module     hinter;


    अगर ( !face || !face->आकार || !face->glyph )
      वापस FT_THROW( Invalid_Face_Handle );

    /* The validity test for `glyph_index' is performed by the */
    /* font drivers.                                           */

    slot = face->glyph;
    ft_glyphslot_clear( slot );

    driver  = face->driver;
    library = driver->मूल.library;
    hinter  = library->auto_hinter;

    /* undefined scale means no scale */
    अगर ( face->आकार->metrics.x_ppem == 0 ||
         face->आकार->metrics.y_ppem == 0 )
      load_flags |= FT_LOAD_NO_SCALE;

    /* resolve load flags dependencies */

    अगर ( load_flags & FT_LOAD_NO_RECURSE )
      load_flags |= FT_LOAD_NO_SCALE         |
                    FT_LOAD_IGNORE_TRANSFORM;

    अगर ( load_flags & FT_LOAD_NO_SCALE )
    {
      load_flags |= FT_LOAD_NO_HINTING |
                    FT_LOAD_NO_BITMAP;

      load_flags &= ~FT_LOAD_RENDER;
    }

    अगर ( load_flags & FT_LOAD_BITMAP_METRICS_ONLY )
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

    अगर ( hinter                                           &&
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
      अगर ( ( load_flags & FT_LOAD_FORCE_AUTOHINT ) ||
           !FT_DRIVER_HAS_HINTER( driver )         )
        autohint = TRUE;
      अन्यथा
      {
        FT_Render_Mode  mode = FT_LOAD_TARGET_MODE( load_flags );
        FT_Bool         is_light_type1;
        TT_Face         ttface = (TT_Face)face;


        /* only the new Adobe engine (for both CFF and Type 1) is `light'; */
        /* we use `strstr' to catch both `Type 1' and `CID Type 1'         */
        is_light_type1 =
          ft_strstr( FT_Get_Font_Format( face ), "Type 1" ) != शून्य &&
          ((PS_Driver)driver)->hinting_engine == FT_HINTING_ADOBE;

        /* the check for `num_locations' assures that we actually    */
        /* test for instructions in a TTF and not in a CFF-based OTF */
        /*                                                           */
        /* we check the size of the `fpgm' and `prep' tables, too -- */
        /* the assumption is that there don't exist real TTFs where  */
        /* both `fpgm' and `prep' tables are missing                 */
        अगर ( ( mode == FT_RENDER_MODE_LIGHT           &&
               ( !FT_DRIVER_HINTS_LIGHTLY( driver ) &&
                 !is_light_type1                    ) )         ||
             ( FT_IS_SFNT( face )                             &&
               ttface->अंक_locations                          &&
               ttface->font_program_आकार == 0                 &&
               ttface->cvt_program_आकार <= 7                  ) )
          autohint = TRUE;
      }
    }

#अगर_घोषित TT_CONFIG_OPTION_VARC
    /* Never auto-hint a VARC (variable composite) glyph: its components */
    /* are already loaded unhinted, and the auto-fitter cannot process   */
    /* the assembled composite (it also re-enters variation-coordinate   */
    /* setting).  Use the native hinter / unhinted load instead.         */
    अगर ( autohint && FT_IS_SFNT( face ) )
    {
      FT_Service_VARC  varc;


      FT_FACE_FIND_SERVICE( face, varc, VARC );
      अगर ( varc && varc->has_glyph( face, glyph_सूचकांक ) )
        autohint = FALSE;
    }
#पूर्ण_अगर

    अगर ( autohint )
    {
      FT_AutoHinter_Interface  hinting;


      /* XXX: The use of the `FT_LOAD_XXX_ONLY` flags is not very */
      /*      elegant.                                            */

      /* try to load SVG documents if available */
      अगर ( ( load_flags & FT_LOAD_NO_SVG ) == 0 &&
           FT_HAS_SVG( face )                   )
      {
        error = driver->clazz->load_glyph( slot, face->आकार,
                                           glyph_सूचकांक,
                                           load_flags | FT_LOAD_SVG_ONLY );

        अगर ( !error && slot->format == FT_GLYPH_FORMAT_SVG )
          जाओ Load_Ok;
      }

      /* try to load embedded bitmaps if available */
      अगर ( FT_HAS_FIXED_SIZES( face )              &&
           ( load_flags & FT_LOAD_NO_BITMAP ) == 0 )
      {
        error = driver->clazz->load_glyph( slot, face->आकार,
                                           glyph_सूचकांक,
                                           load_flags | FT_LOAD_SBITS_ONLY );

        अगर ( !error && slot->format == FT_GLYPH_FORMAT_BITMAP )
          जाओ Load_Ok;
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
                                       slot, face->आकार,
                                       glyph_सूचकांक, load_flags );

        internal->transform_flags = transform_flags;
      }
    }
    अन्यथा
    {
      error = driver->clazz->load_glyph( slot,
                                         face->आकार,
                                         glyph_सूचकांक,
                                         load_flags );
      अगर ( error )
        जाओ Exit;

      अगर ( slot->format == FT_GLYPH_FORMAT_OUTLINE )
      {
        /* check that the loaded outline is correct */
        error = FT_Outline_Check( &slot->outline );
        अगर ( error )
          जाओ Exit;

#अगर_घोषित GRID_FIT_METRICS
        अगर ( !( load_flags & FT_LOAD_NO_HINTING ) )
          ft_glyphslot_grid_fit_metrics(
            slot,
            FT_BOOL( load_flags & FT_LOAD_VERTICAL_LAYOUT ) );
#पूर्ण_अगर
      }
    }

  Load_Ok:
    /* compute the advance */
    अगर ( load_flags & FT_LOAD_VERTICAL_LAYOUT )
    {
      slot->advance.x = 0;
      slot->advance.y = slot->metrics.vertAdvance;
    }
    अन्यथा
    {
      slot->advance.x = slot->metrics.horiAdvance;
      slot->advance.y = 0;
    }

    /* compute the linear advance in 16.16 pixels */
    अगर ( ( load_flags & FT_LOAD_LINEAR_DESIGN ) == 0 &&
         FT_IS_SCALABLE( face )                      )
    {
      FT_आकार_Metrics*  metrics = &face->आकार->metrics;


      /* it's tricky! */
      slot->linearHoriAdvance = FT_MulDiv( slot->linearHoriAdvance,
                                           metrics->x_scale, 64 );

      slot->linearVertAdvance = FT_MulDiv( slot->linearVertAdvance,
                                           metrics->y_scale, 64 );
    }

    अगर ( ( load_flags & FT_LOAD_IGNORE_TRANSFORM ) == 0 )
    {
      FT_Face_Internal  internal = face->internal;


      /* now, transform the glyph image if needed */
      अगर ( internal->transform_flags )
      {
        /* get renderer */
        FT_Renderer  renderer = ft_lookup_glyph_renderer( slot );


        अगर ( renderer )
          error = renderer->clazz->transform_glyph(
                                     renderer, slot,
                                     &internal->transform_matrix,
                                     &internal->transform_delta );
        अन्यथा अगर ( slot->format == FT_GLYPH_FORMAT_OUTLINE )
        {
          /* apply `standard' transformation if no renderer is available */
          अगर ( internal->transform_flags & 1 )
            FT_Outline_Transform( &slot->outline,
                                  &internal->transform_matrix );

          अगर ( internal->transform_flags & 2 )
            FT_Outline_Translate( &slot->outline,
                                  internal->transform_delta.x,
                                  internal->transform_delta.y );
        }

        /* transform advance */
        FT_Vector_Transform( &slot->advance, &internal->transform_matrix );
      }
    }

    slot->glyph_सूचकांक          = glyph_सूचकांक;
    slot->internal->load_flags = load_flags;

    /* do we need to render the image or preset the bitmap now? */
    अगर ( !error                                    &&
         ( load_flags & FT_LOAD_NO_SCALE ) == 0    &&
         slot->format != FT_GLYPH_FORMAT_BITMAP    &&
         slot->format != FT_GLYPH_FORMAT_COMPOSITE )
    {
      FT_Render_Mode  mode = FT_LOAD_TARGET_MODE( load_flags );


      अगर ( mode == FT_RENDER_MODE_NORMAL   &&
           load_flags & FT_LOAD_MONOCHROME )
        mode = FT_RENDER_MODE_MONO;

      अगर ( load_flags & FT_LOAD_RENDER )
        error = FT_Render_Glyph( slot, mode );
      अन्यथा
        ft_glyphslot_preset_bitmap( slot, mode, शून्य );
    }

#अगर_घोषित FT_DEBUG_LEVEL_TRACE
    FT_TRACE5(( "FT_Load_Glyph: index %u, flags 0x%x\n",
                glyph_सूचकांक, load_flags ));
    FT_TRACE5(( "  bitmap %ux%u %s, %s (mode %d)\n",
                slot->bitmap.width,
                slot->bitmap.rows,
                slot->outline.points ?
                  slot->bitmap.बफर ? "rendered"
                                      : "preset"
                                     :
                  slot->internal->flags & FT_GLYPH_OWN_BITMAP ? "owned"
                                                              : "unowned",
                pixel_modes[slot->bitmap.pixel_mode],
                slot->bitmap.pixel_mode ));
    FT_TRACE5(( "\n" ));
    FT_TRACE5(( "  x advance: %f\n", (द्विगुन)slot->advance.x / 64 ));
    FT_TRACE5(( "  y advance: %f\n", (द्विगुन)slot->advance.y / 64 ));
    FT_TRACE5(( "  linear x advance: %f\n",
                (द्विगुन)slot->linearHoriAdvance / 65536 ));
    FT_TRACE5(( "  linear y advance: %f\n",
                (द्विगुन)slot->linearVertAdvance / 65536 ));

    {
      FT_Glyph_Metrics*  metrics = &slot->metrics;


      FT_TRACE5(( "  metrics:\n" ));
      FT_TRACE5(( "    width:  %f\n", (द्विगुन)metrics->width / 64 ));
      FT_TRACE5(( "    height: %f\n", (द्विगुन)metrics->height / 64 ));
      FT_TRACE5(( "\n" ));
      FT_TRACE5(( "    horiBearingX: %f\n",
                  (द्विगुन)metrics->horiBearingX / 64 ));
      FT_TRACE5(( "    horiBearingY: %f\n",
                  (द्विगुन)metrics->horiBearingY / 64 ));
      FT_TRACE5(( "    horiAdvance:  %f\n",
                  (द्विगुन)metrics->horiAdvance / 64 ));
      FT_TRACE5(( "\n" ));
      FT_TRACE5(( "    vertBearingX: %f\n",
                  (द्विगुन)metrics->vertBearingX / 64 ));
      FT_TRACE5(( "    vertBearingY: %f\n",
                  (द्विगुन)metrics->vertBearingY / 64 ));
      FT_TRACE5(( "    vertAdvance:  %f\n",
                  (द्विगुन)metrics->vertAdvance / 64 ));
    }
#पूर्ण_अगर

  Exit:
    वापस error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Load_Char( FT_Face   face,
                FT_ULong  char_code,
                FT_Int32  load_flags )
  {
    FT_UInt  glyph_सूचकांक;


    अगर ( !face )
      वापस FT_THROW( Invalid_Face_Handle );

    glyph_सूचकांक = (FT_UInt)char_code;
    अगर ( face->charmap )
      glyph_सूचकांक = FT_Get_Char_सूचकांक( face, char_code );

    वापस FT_Load_Glyph( face, glyph_सूचकांक, load_flags );
  }


  /* destructor for sizes list */
  अटल व्योम
  destroy_आकार( FT_Memory  memory,
                व्योम*      आकार_2,
                व्योम*      driver_ )
  {
    FT_आकार    आकार   = (FT_आकार)आकार_2;
    FT_Driver  driver = (FT_Driver)driver_;


    /* finalize format-specific stuff */
    अगर ( driver->clazz->done_आकार )
      driver->clazz->done_आकार( आकार );

    /* finalize client-specific data */
    अगर ( आकार->generic.finalizer )
      आकार->generic.finalizer( आकार );

    FT_FREE( आकार->internal );
    FT_FREE( आकार );
  }


  अटल व्योम
  ft_cmap_done_internal( FT_CMap  cmap );


  अटल व्योम
  destroy_charmaps( FT_Face    face,
                    FT_Memory  memory )
  {
    FT_Int  n;


    अगर ( !face )
      वापस;

    क्रम ( n = 0; n < face->अंक_charmaps; n++ )
    {
      FT_CMap  cmap = FT_CMAP( face->charmaps[n] );


      ft_cmap_done_internal( cmap );

      face->charmaps[n] = शून्य;
    }

    FT_FREE( face->charmaps );
    face->अंक_charmaps = 0;
  }


  /* destructor for faces list */
  अटल व्योम
  destroy_face( FT_Memory  memory,
                व्योम*      face_,
                व्योम*      driver_ )
  {
    FT_Face          face   = (FT_Face)face_;
    FT_Driver        driver = (FT_Driver)driver_;
    FT_Driver_Class  clazz  = driver->clazz;


    /* discard auto-hinting data */
    अगर ( face->autohint.finalizer )
      face->autohint.finalizer( face->autohint.तथ्य );

    /* Discard glyph slots for this face.                           */
    /* Beware!  FT_Done_GlyphSlot() changes the field `face->glyph' */
    जबतक ( face->glyph )
      FT_Done_GlyphSlot( face->glyph );

    /* discard all sizes for this face */
    FT_List_Finalize( &face->sizes_list,
                      destroy_आकार,
                      memory,
                      driver );
    face->आकार = शून्य;

    /* discard charmaps */
    destroy_charmaps( face, memory );

    /* finalize format-specific stuff */
    अगर ( clazz->done_face )
      clazz->done_face( face );

    /* close the stream for this face if needed */
    FT_Stream_Free(
      face->stream,
      ( face->face_flags & FT_FACE_FLAG_EXTERNAL_STREAM ) != 0 );

    face->stream = शून्य;

    /* now discard client data */
    अगर ( face->generic.finalizer )
      face->generic.finalizer( face );

    /* get rid of it */
    अगर ( face->internal )
    {
      FT_FREE( face->internal );
    }
    FT_FREE( face );
  }


  अटल व्योम
  Destroy_Driver( FT_Driver  driver )
  {
    FT_List_Finalize( &driver->faces_list,
                      destroy_face,
                      driver->मूल.memory,
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

    अगर ( !first )
      वापस FT_THROW( Invalid_CharMap_Handle );

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

    cur = first + face->अंक_charmaps;  /* points after the last one */

    क्रम ( ; --cur >= first; )
    {
      अगर ( cur[0]->encoding == FT_ENCODING_UNICODE )
      {
        /* XXX If some new encodings to represent UCS-4 are added, */
        /*     they should be added here.                          */
        अगर ( ( cur[0]->platform_id == TT_PLATFORM_MICROSOFT &&
               cur[0]->encoding_id == TT_MS_ID_UCS_4        )     ||
             ( cur[0]->platform_id == TT_PLATFORM_APPLE_UNICODE &&
               cur[0]->encoding_id == TT_APPLE_ID_UNICODE_32    ) ||
             ( cur[0]->platform_id == TT_PLATFORM_APPLE_UNICODE &&
               cur[0]->encoding_id == TT_APPLE_ID_FULL_UNICODE  &&
               FT_Get_CMap_Format( cur[0] ) == 13               ) )
        {
          face->charmap = cur[0];
          वापस FT_Err_Ok;
        }
      }
    }

    /* We do not have any UCS-4 charmap.                */
    /* Do the loop again and search for UCS-2 charmaps. */
    cur = first + face->अंक_charmaps;

    क्रम ( ; --cur >= first; )
    {
      अगर ( cur[0]->encoding == FT_ENCODING_UNICODE )
      {
        FT_Long  format = FT_Get_CMap_Format( cur[0] );


        अगर ( format != 14 && format != 15 )
        {
          face->charmap = cur[0];
          वापस FT_Err_Ok;
        }
      }
    }

    वापस FT_THROW( Invalid_CharMap_Handle );
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
  अटल FT_CharMap
  find_variant_selector_charmap( FT_Face  face )
  {
    FT_CharMap*  first;
    FT_CharMap*  end;
    FT_CharMap*  cur;


    /* caller should have already checked that `face' is valid */
    FT_ASSERT( face );

    first = face->charmaps;

    अगर ( !first )
      वापस शून्य;

    end = first + face->अंक_charmaps;  /* points after the last one */

    क्रम ( cur = first; cur < end; cur++ )
    {
      अगर ( cur[0]->platform_id == TT_PLATFORM_APPLE_UNICODE    &&
           cur[0]->encoding_id == TT_APPLE_ID_VARIANT_SELECTOR )
      {
        FT_Long  format = FT_Get_CMap_Format( cur[0] );


        अगर ( format == 14 || format == 15 )
          वापस cur[0];
      }
    }

    वापस शून्य;
  }


  /**************************************************************************
   *
   * @Function:
   *   open_face
   *
   * @Description:
   *   This function does some work for FT_Open_Face().
   */
  अटल FT_Error
  open_face( FT_Driver      driver,
             FT_Stream      *astream,
             FT_Bool        *anexternal_stream,
             FT_Long        face_सूचकांक,
             FT_Int         अंक_params,
             FT_Parameter*  params,
             FT_Face       *aface )
  {
    FT_Memory         memory;
    FT_Driver_Class   clazz;
    FT_Face           face     = शून्य;
    FT_Face_Internal  internal = शून्य;

    FT_Error          error, error2;


    clazz  = driver->clazz;
    memory = driver->मूल.memory;

    /* allocate the face object and perform basic initialization */
    अगर ( FT_ALLOC( face, clazz->face_object_आकार ) )
      जाओ Fail;

    face->driver = driver;
    face->memory = memory;
    face->stream = *astream;

    /* set the FT_FACE_FLAG_EXTERNAL_STREAM bit for FT_Done_Face */
    अगर ( *anexternal_stream )
      face->face_flags |= FT_FACE_FLAG_EXTERNAL_STREAM;

    अगर ( FT_NEW( internal ) )
      जाओ Fail;

    face->internal = internal;

#अगर_घोषित FT_CONFIG_OPTION_INCREMENTAL
    {
      पूर्णांक  i;


      face->internal->incremental_interface = शून्य;
      क्रम ( i = 0; i < अंक_params && !face->internal->incremental_interface;
            i++ )
        अगर ( params[i].tag == FT_PARAM_TAG_INCREMENTAL )
          face->internal->incremental_interface =
            (FT_Incremental_Interface)params[i].तथ्य;
    }
#पूर्ण_अगर

    face->internal->random_seed = -1;

    अगर ( clazz->init_face )
      error = clazz->init_face( *astream,
                                face,
                                (FT_Int)face_सूचकांक,
                                अंक_params,
                                params );
    /* Stream may have been changed. */
    *astream = face->stream;
    *anexternal_stream =
      ( face->face_flags & FT_FACE_FLAG_EXTERNAL_STREAM ) != 0;
    अगर ( error )
      जाओ Fail;

    /* select Unicode charmap by default */
    error2 = find_unicode_charmap( face );

    /* if no Unicode charmap can be found, FT_Err_Invalid_CharMap_Handle */
    /* is returned.                                                      */

    /* no error should happen, but we want to play safe */
    अगर ( error2 && FT_ERR_NEQ( error2, Invalid_CharMap_Handle ) )
    {
      error = error2;
      जाओ Fail;
    }

    *aface = face;

  Fail:
    अगर ( error )
    {
      destroy_charmaps( face, memory );
      अगर ( clazz->done_face )
        clazz->done_face( face );
      FT_FREE( internal );
      FT_FREE( face );
      *aface = शून्य;
    }

    वापस error;
  }


  /* there's a Mac-specific extended implementation of FT_New_Face() */
  /* in src/base/ftmac.c                                             */

#अगर_अघोषित FT_MACINTOSH

  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_New_Face( FT_Library   library,
               स्थिर अक्षर*  pathname,
               FT_Long      face_सूचकांक,
               FT_Face     *aface )
  {
    FT_Open_Args  args;


    /* test for valid `library' and `aface' delayed to `FT_Open_Face' */
    अगर ( !pathname )
      वापस FT_THROW( Invalid_Argument );

    args.flags    = FT_OPEN_PATHNAME;
    args.pathname = (अक्षर*)pathname;
    args.stream   = शून्य;

    वापस ft_open_face_internal( library, &args, face_सूचकांक, aface, 1 );
  }

#पूर्ण_अगर


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_New_Memory_Face( FT_Library      library,
                      स्थिर FT_Byte*  file_base,
                      FT_Long         file_आकार,
                      FT_Long         face_सूचकांक,
                      FT_Face        *aface )
  {
    FT_Open_Args  args;


    /* test for valid `library' and `face' delayed to `FT_Open_Face' */
    अगर ( !file_base )
      वापस FT_THROW( Invalid_Argument );

    args.flags       = FT_OPEN_MEMORY;
    args.memory_base = file_base;
    args.memory_आकार = file_आकार;
    args.stream      = शून्य;

    वापस ft_open_face_internal( library, &args, face_सूचकांक, aface, 1 );
  }


#अगर_घोषित FT_CONFIG_OPTION_MAC_FONTS

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
  अटल व्योम
  memory_stream_close( FT_Stream  stream )
  {
    FT_Memory  memory = (FT_Memory)stream->descriptor.pointer;


    FT_FREE( stream->base );
    stream->आकार  = 0;
    stream->बंद = शून्य;
    FT_FREE( stream );
  }


  /* Create a new memory stream from a buffer and a size. */
  /* From `ftmac.c'.                                      */
  अटल FT_Error
  new_memory_stream( FT_Library           library,
                     FT_Byte*             base,
                     FT_ULong             आकार,
                     FT_Stream_CloseFunc  बंद,
                     FT_Stream           *astream )
  {
    FT_Error   error;
    FT_Memory  memory;
    FT_Stream  stream = शून्य;


    अगर ( !library )
      वापस FT_THROW( Invalid_Library_Handle );

    अगर ( !base )
      वापस FT_THROW( Invalid_Argument );

    *astream = शून्य;
    memory   = library->memory;
    अगर ( FT_NEW( stream ) )
      जाओ Exit;

    FT_Stream_OpenMemory( stream, base, आकार );

    stream->descriptor.pointer = memory;
    stream->बंद              = बंद;

    *astream = stream;

  Exit:
    वापस error;
  }


  /* Create a new FT_Face given a buffer and a driver name. */
  /* From `ftmac.c'.                                        */
  FT_LOCAL_DEF( FT_Error )
  open_face_from_बफर( FT_Library   library,
                         FT_Byte*     base,
                         FT_ULong     आकार,
                         FT_Long      face_सूचकांक,
                         स्थिर अक्षर*  driver_name,
                         FT_Face     *aface )
  {
    FT_Open_Args  args;
    FT_Error      error;
    FT_Memory     memory = library->memory;


    args.driver = शून्य;
    args.flags  = 0;

    अगर ( driver_name )
    {
      args.driver = FT_Get_Module( library, driver_name );
      अगर ( !args.driver )
      {
        FT_FREE( base );
        वापस FT_THROW( Missing_Module );
      }

      args.flags = args.flags | FT_OPEN_DRIVER;
    }

    /* `memory_stream_close` also frees the stream object. */
    error = new_memory_stream( library,
                               base,
                               आकार,
                               memory_stream_close,
                               &args.stream );
    अगर ( error )
    {
      FT_FREE( base );
      वापस error;
    }

    args.flags |= FT_OPEN_STREAM;

#अगर_घोषित FT_MACINTOSH
    /* At this point, the face index has served its purpose;  */
    /* whoever calls this function has already used it to     */
    /* locate the correct font data.  We should not propagate */
    /* this index to FT_Open_Face() (unless it is negative).  */

    अगर ( face_सूचकांक > 0 )
      face_सूचकांक &= 0x7FFF0000L; /* retain GX data */
#पूर्ण_अगर

    वापस ft_open_face_internal( library, &args, face_सूचकांक, aface, 0 );
  }


  /* Look up `TYP1' or `CID ' table from sfnt table directory.       */
  /* `offset' and `length' must exclude the binary header in tables. */

  /* Type 1 and CID-keyed font drivers should recognize sfnt-wrapped */
  /* format too.  Here, since we can't expect that the TrueType font */
  /* driver is loaded unconditionally, we must parse the font by     */
  /* ourselves.  We are only interested in the name of the table and */
  /* the offset.                                                     */

  अटल FT_Error
  ft_lookup_PS_in_sfnt_stream( FT_Stream  stream,
                               FT_Long    face_सूचकांक,
                               FT_ULong*  offset,
                               FT_ULong*  लंबाई,
                               FT_Bool*   is_sfnt_cid )
  {
    FT_Error   error;
    FT_UShort  अंक_Tables;
    FT_Long    pstable_सूचकांक;
    FT_ULong   tag;
    पूर्णांक        i;


    *offset = 0;
    *लंबाई = 0;
    *is_sfnt_cid = FALSE;

    /* TODO: support for sfnt-wrapped PS/CID in TTC format */

    /* version check for 'typ1' (should be ignored?) */
    अगर ( FT_READ_ULONG( tag ) )
      वापस error;
    अगर ( tag != TTAG_typ1 )
      वापस FT_THROW( Unknown_File_Format );

    अगर ( FT_READ_USHORT( अंक_Tables ) )
      वापस error;
    अगर ( FT_STREAM_SKIP( 2 * 3 ) ) /* skip binary search header */
      वापस error;

    pstable_सूचकांक = -1;
    *is_sfnt_cid  = FALSE;

    क्रम ( i = 0; i < अंक_Tables; i++ )
    {
      अगर ( FT_READ_ULONG( tag )     || FT_STREAM_SKIP( 4 )      ||
           FT_READ_ULONG( *offset ) || FT_READ_ULONG( *लंबाई ) )
        वापस error;

      अगर ( tag == TTAG_CID )
      {
        pstable_सूचकांक++;
        *offset += 22;
        *लंबाई -= 22;
        *is_sfnt_cid = TRUE;
        अगर ( face_सूचकांक < 0 )
          वापस FT_Err_Ok;
      }
      अन्यथा अगर ( tag == TTAG_TYP1 )
      {
        pstable_सूचकांक++;
        *offset += 24;
        *लंबाई -= 24;
        *is_sfnt_cid = FALSE;
        अगर ( face_सूचकांक < 0 )
          वापस FT_Err_Ok;
      }
      अगर ( face_सूचकांक >= 0 && pstable_सूचकांक == face_सूचकांक )
        वापस FT_Err_Ok;
    }

    वापस FT_THROW( Table_Missing );
  }


  FT_LOCAL_DEF( FT_Error )
  open_face_PS_from_sfnt_stream( FT_Library     library,
                                 FT_Stream      stream,
                                 FT_Long        face_सूचकांक,
                                 FT_Int         अंक_params,
                                 FT_Parameter  *params,
                                 FT_Face       *aface )
  {
    FT_Error   error;
    FT_Memory  memory = library->memory;
    FT_ULong   offset, लंबाई;
    FT_ULong   pos;
    FT_Bool    is_sfnt_cid;
    FT_Byte*   sfnt_ps = शून्य;

    FT_UNUSED( अंक_params );
    FT_UNUSED( params );


    /* ignore GX stuff */
    अगर ( face_सूचकांक > 0 )
      face_सूचकांक &= 0xFFFFL;

    pos = FT_STREAM_POS();

    error = ft_lookup_PS_in_sfnt_stream( stream,
                                         face_सूचकांक,
                                         &offset,
                                         &लंबाई,
                                         &is_sfnt_cid );
    अगर ( error )
      जाओ Exit;

    अगर ( offset > stream->आकार )
    {
      FT_TRACE2(( "open_face_PS_from_sfnt_stream: invalid table offset\n" ));
      error = FT_THROW( Invalid_Table );
      जाओ Exit;
    }
    अन्यथा अगर ( लंबाई > stream->आकार - offset )
    {
      FT_TRACE2(( "open_face_PS_from_sfnt_stream: invalid table length\n" ));
      error = FT_THROW( Invalid_Table );
      जाओ Exit;
    }

    error = FT_Stream_Seek( stream, pos + offset );
    अगर ( error )
      जाओ Exit;

    अगर ( FT_QALLOC( sfnt_ps, (FT_Long)लंबाई ) )
      जाओ Exit;

    error = FT_Stream_Read( stream, (FT_Byte *)sfnt_ps, लंबाई );
    अगर ( error )
    {
      FT_FREE( sfnt_ps );
      जाओ Exit;
    }

    error = open_face_from_बफर( library,
                                   sfnt_ps,
                                   लंबाई,
                                   FT_MIN( face_सूचकांक, 0 ),
                                   is_sfnt_cid ? "t1cid" : "type1",
                                   aface );
  Exit:
    {
      FT_Error  error1;


      अगर ( FT_ERR_EQ( error, Unknown_File_Format ) )
      {
        error1 = FT_Stream_Seek( stream, pos );
        अगर ( error1 )
          वापस error1;
      }

      वापस error;
    }
  }


#अगर_अघोषित FT_MACINTOSH

  /* The resource header says we've got resource_cnt `POST' (type1) */
  /* resources in this file.  They all need to be coalesced into    */
  /* one lump which gets passed on to the type1 driver.             */
  /* Here can be only one PostScript font in a file so face_index   */
  /* must be 0 (or -1).                                             */
  /*                                                                */
  अटल FT_Error
  Mac_Read_POST_Resource( FT_Library  library,
                          FT_Stream   stream,
                          FT_Long    *offsets,
                          FT_Long     resource_cnt,
                          FT_Long     face_सूचकांक,
                          FT_Face    *aface )
  {
    FT_Error   error  = FT_ERR( Cannot_Open_Resource );
    FT_Memory  memory = library->memory;

    FT_Byte*   pfb_तथ्य = शून्य;
    पूर्णांक        i, type, flags;
    FT_ULong   len;
    FT_ULong   pfb_len, pfb_pos, pfb_lenpos;
    FT_ULong   rlen, अस्थायी;


    अगर ( face_सूचकांक == -1 )
      face_सूचकांक = 0;
    अगर ( face_सूचकांक != 0 )
      वापस error;

    /* Find the length of all the POST resources, concatenated.  Assume */
    /* worst case (each resource in its own section).                   */
    pfb_len = 0;
    क्रम ( i = 0; i < resource_cnt; i++ )
    {
      error = FT_Stream_Seek( stream, (FT_ULong)offsets[i] );
      अगर ( error )
        जाओ Exit;
      अगर ( FT_READ_ULONG( अस्थायी ) )  /* actually LONG */
        जाओ Exit;

      /* FT2 allocator takes signed long buffer length,
       * too large value causing overflow should be checked
       */
      FT_TRACE4(( "                 POST fragment #%d: length=0x%08lx"
                  " total pfb_len=0x%08lx\n",
                  i, अस्थायी, pfb_len + अस्थायी + 6 ));

      अगर ( FT_MAC_RFORK_MAX_LEN < अस्थायी               ||
           FT_MAC_RFORK_MAX_LEN - अस्थायी < pfb_len + 6 )
      {
        FT_TRACE2(( "             MacOS resource length cannot exceed"
                    " 0x%08lx\n",
                    FT_MAC_RFORK_MAX_LEN ));

        error = FT_THROW( Invalid_Offset );
        जाओ Exit;
      }

      pfb_len += अस्थायी + 6;
    }

    FT_TRACE2(( "             total buffer size to concatenate"
                " %ld POST fragments: 0x%08lx\n",
                 resource_cnt, pfb_len + 2 ));

    अगर ( pfb_len + 2 < 6 )
    {
      FT_TRACE2(( "             too long fragment length makes"
                  " pfb_len confused: pfb_len=0x%08lx\n",
                  pfb_len ));

      error = FT_THROW( Array_Too_Large );
      जाओ Exit;
    }

    अगर ( FT_QALLOC( pfb_तथ्य, (FT_Long)pfb_len + 2 ) )
      जाओ Exit;

    pfb_तथ्य[0] = 0x80;
    pfb_तथ्य[1] = 1;            /* Ascii section */
    pfb_तथ्य[2] = 0;            /* 4-byte length, fill in later */
    pfb_तथ्य[3] = 0;
    pfb_तथ्य[4] = 0;
    pfb_तथ्य[5] = 0;
    pfb_pos     = 6;
    pfb_lenpos  = 2;

    len  = 0;
    type = 1;

    क्रम ( i = 0; i < resource_cnt; i++ )
    {
      error = FT_Stream_Seek( stream, (FT_ULong)offsets[i] );
      अगर ( error )
        जाओ Exit2;
      अगर ( FT_READ_ULONG( rlen ) )
        जाओ Exit2;

      /* FT2 allocator takes signed long buffer length,
       * too large fragment length causing overflow should be checked
       */
      अगर ( 0x7FFFFFFFUL < rlen )
      {
        error = FT_THROW( Invalid_Offset );
        जाओ Exit2;
      }

      अगर ( FT_READ_USHORT( flags ) )
        जाओ Exit2;

      FT_TRACE3(( "POST fragment[%d]:"
                  " offsets=0x%08lx, rlen=0x%08lx, flags=0x%04x\n",
                  i, offsets[i], rlen, flags ));

      error = FT_ERR( Array_Too_Large );

      /* postpone the check of `rlen longer than buffer' */
      /* until `FT_Stream_Read'                          */

      अगर ( ( flags >> 8 ) == 0 )        /* Comment, should not be loaded */
      {
        FT_TRACE3(( "    Skip POST fragment #%d because it is a comment\n",
                    i ));
        जारी;
      }

      /* the flags are part of the resource, so rlen >= 2,  */
      /* but some fonts declare rlen = 0 for empty fragment */
      अगर ( rlen > 2 )
        rlen -= 2;
      अन्यथा
        rlen = 0;

      अगर ( ( flags >> 8 ) == type )
        len += rlen;
      अन्यथा
      {
        FT_TRACE3(( "    Write POST fragment #%d header (4-byte) to buffer"
                    " %p + 0x%08lx\n",
                    i, (व्योम*)pfb_तथ्य, pfb_lenpos ));

        अगर ( pfb_lenpos + 3 > pfb_len + 2 )
          जाओ Exit2;

        pfb_तथ्य[pfb_lenpos    ] = (FT_Byte)( len );
        pfb_तथ्य[pfb_lenpos + 1] = (FT_Byte)( len >> 8 );
        pfb_तथ्य[pfb_lenpos + 2] = (FT_Byte)( len >> 16 );
        pfb_तथ्य[pfb_lenpos + 3] = (FT_Byte)( len >> 24 );

        अगर ( ( flags >> 8 ) == 5 )      /* End of font mark */
          अवरोध;

        FT_TRACE3(( "    Write POST fragment #%d header (6-byte) to buffer"
                    " %p + 0x%08lx\n",
                    i, (व्योम*)pfb_तथ्य, pfb_pos ));

        अगर ( pfb_pos + 6 > pfb_len + 2 )
          जाओ Exit2;

        pfb_तथ्य[pfb_pos++] = 0x80;

        type = flags >> 8;
        len  = rlen;

        pfb_तथ्य[pfb_pos++] = (FT_Byte)type;
        pfb_lenpos          = pfb_pos;
        pfb_तथ्य[pfb_pos++] = 0;        /* 4-byte length, fill in later */
        pfb_तथ्य[pfb_pos++] = 0;
        pfb_तथ्य[pfb_pos++] = 0;
        pfb_तथ्य[pfb_pos++] = 0;
      }

      अगर ( pfb_pos > pfb_len || pfb_pos + rlen > pfb_len )
        जाओ Exit2;

      FT_TRACE3(( "    Load POST fragment #%d (%lu byte) to buffer"
                  " %p + 0x%08lx\n",
                  i, rlen, (व्योम*)pfb_तथ्य, pfb_pos ));

      error = FT_Stream_Read( stream, (FT_Byte *)pfb_तथ्य + pfb_pos, rlen );
      अगर ( error )
        जाओ Exit2;

      pfb_pos += rlen;
    }

    error = FT_ERR( Array_Too_Large );

    अगर ( pfb_pos + 2 > pfb_len + 2 )
      जाओ Exit2;
    pfb_तथ्य[pfb_pos++] = 0x80;
    pfb_तथ्य[pfb_pos++] = 3;

    अगर ( pfb_lenpos + 3 > pfb_len + 2 )
      जाओ Exit2;
    pfb_तथ्य[pfb_lenpos    ] = (FT_Byte)( len );
    pfb_तथ्य[pfb_lenpos + 1] = (FT_Byte)( len >> 8 );
    pfb_तथ्य[pfb_lenpos + 2] = (FT_Byte)( len >> 16 );
    pfb_तथ्य[pfb_lenpos + 3] = (FT_Byte)( len >> 24 );

    वापस open_face_from_बफर( library,
                                  pfb_तथ्य,
                                  pfb_pos,
                                  face_सूचकांक,
                                  "type1",
                                  aface );

  Exit2:
    अगर ( FT_ERR_EQ( error, Array_Too_Large ) )
      FT_TRACE2(( "  Abort due to too-short buffer to store"
                  " all POST fragments\n" ));
    अन्यथा अगर ( FT_ERR_EQ( error, Invalid_Offset ) )
      FT_TRACE2(( "  Abort due to invalid offset in a POST fragment\n" ));

    अगर ( error )
      error = FT_ERR( Cannot_Open_Resource );
    FT_FREE( pfb_तथ्य );

  Exit:
    वापस error;
  }


  /* The resource header says we've got resource_cnt `sfnt'      */
  /* (TrueType/OpenType) resources in this file.  Look through   */
  /* them for the one indicated by face_index, load it into mem, */
  /* pass it on to the truetype driver, and return it.           */
  /*                                                             */
  अटल FT_Error
  Mac_Read_sfnt_Resource( FT_Library  library,
                          FT_Stream   stream,
                          FT_Long    *offsets,
                          FT_Long     resource_cnt,
                          FT_Long     face_सूचकांक,
                          FT_Face    *aface )
  {
    FT_Memory  memory = library->memory;
    FT_Byte*   sfnt_तथ्य = शून्य;
    FT_Error   error;
    FT_ULong   flag_offset;
    FT_ULong   rlen;
    पूर्णांक        is_cff;
    FT_Long    face_सूचकांक_in_resource = 0;


    अगर ( face_सूचकांक < 0 )
      face_सूचकांक = -face_सूचकांक - 1;
    अगर ( face_सूचकांक >= resource_cnt )
      वापस FT_THROW( Cannot_Open_Resource );

    flag_offset = (FT_ULong)offsets[face_सूचकांक];
    error = FT_Stream_Seek( stream, flag_offset );
    अगर ( error )
      जाओ Exit;

    अगर ( FT_READ_ULONG( rlen ) )
      जाओ Exit;
    अगर ( !rlen )
      वापस FT_THROW( Cannot_Open_Resource );
    अगर ( rlen > FT_MAC_RFORK_MAX_LEN )
      वापस FT_THROW( Invalid_Offset );

    error = open_face_PS_from_sfnt_stream( library,
                                           stream,
                                           face_सूचकांक,
                                           0, शून्य,
                                           aface );
    अगर ( !error )
      जाओ Exit;

    /* rewind sfnt stream before open_face_PS_from_sfnt_stream() */
    error = FT_Stream_Seek( stream, flag_offset + 4 );
    अगर ( error )
      जाओ Exit;

    अगर ( FT_QALLOC( sfnt_तथ्य, rlen ) )
      वापस error;
    error = FT_Stream_Read( stream, (FT_Byte *)sfnt_तथ्य, rlen );
    अगर ( error )
    {
      FT_FREE( sfnt_तथ्य );
      जाओ Exit;
    }

    is_cff = rlen > 4 && !ft_memcmp( sfnt_तथ्य, "OTTO", 4 );
    error = open_face_from_बफर( library,
                                   sfnt_तथ्य,
                                   rlen,
                                   face_सूचकांक_in_resource,
                                   is_cff ? "cff" : "truetype",
                                   aface );

  Exit:
    वापस error;
  }


  /* Check for a valid resource fork header, or a valid dfont    */
  /* header.  In a resource fork the first 16 bytes are repeated */
  /* at the location specified by bytes 4-7.  In a dfont bytes   */
  /* 4-7 point to 16 bytes of zeroes instead.                    */
  /*                                                             */
  अटल FT_Error
  IsMacResource( FT_Library  library,
                 FT_Stream   stream,
                 FT_Long     resource_offset,
                 FT_Long     face_सूचकांक,
                 FT_Face    *aface )
  {
    FT_Memory  memory = library->memory;
    FT_Error   error;
    FT_Long    map_offset, rdata_pos;
    FT_Long    *तथ्य_offsets;
    FT_Long    गिनती;


    error = FT_Raccess_Get_HeaderInfo( library, stream, resource_offset,
                                       &map_offset, &rdata_pos );
    अगर ( error )
      वापस error;

    /* POST resources must be sorted to concatenate properly */
    error = FT_Raccess_Get_तथ्य_Offsets( library, stream,
                                        map_offset, rdata_pos,
                                        TTAG_POST, TRUE,
                                        &तथ्य_offsets, &गिनती );
    अगर ( !error )
    {
      error = Mac_Read_POST_Resource( library, stream, तथ्य_offsets, गिनती,
                                      face_सूचकांक, aface );
      FT_FREE( तथ्य_offsets );
      /* POST exists in an LWFN providing a single face */
      अगर ( !error )
        (*aface)->अंक_faces = 1;
      वापस error;
    }

    /* sfnt resources should not be sorted to preserve the face order by
       QuickDraw API */
    error = FT_Raccess_Get_तथ्य_Offsets( library, stream,
                                        map_offset, rdata_pos,
                                        TTAG_sfnt, FALSE,
                                        &तथ्य_offsets, &गिनती );
    अगर ( !error )
    {
      FT_Long  face_सूचकांक_internal = face_सूचकांक % गिनती;


      error = Mac_Read_sfnt_Resource( library, stream, तथ्य_offsets, गिनती,
                                      face_सूचकांक_internal, aface );
      FT_FREE( तथ्य_offsets );
      अगर ( !error )
      {
        (*aface)->अंक_faces  = गिनती;
        (*aface)->face_सूचकांक = face_सूचकांक_internal;
      }
    }

    वापस error;
  }


  /* Check for a valid macbinary header, and if we find one   */
  /* check that the (flattened) resource fork in it is valid. */
  /*                                                          */
  अटल FT_Error
  Is_Mac_द्विआधारी( FT_Library  library,
               FT_Stream   stream,
               FT_Long     face_सूचकांक,
               FT_Face    *aface )
  {
    अचिन्हित अक्षर  header[128];
    FT_Error       error;
    FT_Long        dlen, offset;


    अगर ( !stream )
      वापस FT_THROW( Invalid_Stream_Operation );

    error = FT_Stream_Seek( stream, 0 );
    अगर ( error )
      जाओ Exit;

    error = FT_Stream_Read( stream, (FT_Byte*)header, 128 );
    अगर ( error )
      जाओ Exit;

    अगर (            header[ 0] !=   0 ||
                    header[74] !=   0 ||
                    header[82] !=   0 ||
                    header[ 1] ==   0 ||
                    header[ 1] >   33 ||
                    header[63] !=   0 ||
         header[2 + header[1]] !=   0 ||
                  header[0x53] > 0x7F )
      वापस FT_THROW( Unknown_File_Format );

    dlen = ( header[0x53] << 24 ) |
           ( header[0x54] << 16 ) |
           ( header[0x55] <<  8 ) |
             header[0x56];
#अगर 0
    rlen = ( header[0x57] << 24 ) |
           ( header[0x58] << 16 ) |
           ( header[0x59] <<  8 ) |
             header[0x5A];
#पूर्ण_अगर /* 0 */
    offset = 128 + ( ( dlen + 127 ) & ~127 );

    वापस IsMacResource( library, stream, offset, face_सूचकांक, aface );

  Exit:
    वापस error;
  }


  अटल FT_Error
  load_face_in_embedded_rfork( FT_Library           library,
                               FT_Stream            stream,
                               FT_Long              face_सूचकांक,
                               FT_Face             *aface,
                               स्थिर FT_Open_Args  *args )
  {

#अघोषित  FT_COMPONENT
#घोषणा FT_COMPONENT  raccess

    FT_Memory  memory = library->memory;
    FT_Error   error  = FT_ERR( Unknown_File_Format );
    FT_UInt    i;

    अक्षर*      file_names[FT_RACCESS_N_RULES];
    FT_Long    offsets[FT_RACCESS_N_RULES];
    FT_Error   errors[FT_RACCESS_N_RULES];
    FT_Bool    is_darwin_vfs, vfs_rfork_has_no_font = FALSE; /* not tested */

    FT_Open_Args  args2;
    FT_Stream     stream2 = शून्य;


    FT_Raccess_Guess( library, stream,
                      args->pathname, file_names, offsets, errors );

    क्रम ( i = 0; i < FT_RACCESS_N_RULES; i++ )
    {
      is_darwin_vfs = ft_raccess_rule_by_darwin_vfs( library, i );
      अगर ( is_darwin_vfs && vfs_rfork_has_no_font )
      {
        FT_TRACE3(( "Skip rule %u: darwin vfs resource fork"
                    " is already checked and"
                    " no font is found\n",
                    i ));
        जारी;
      }

      अगर ( errors[i] )
      {
        FT_TRACE3(( "Error 0x%x has occurred in rule %u\n",
                    errors[i], i ));
        जारी;
      }

      args2.flags    = FT_OPEN_PATHNAME;
      args2.pathname = file_names[i] ? file_names[i] : args->pathname;

      FT_TRACE3(( "Try rule %u: %s (offset=%ld) ...",
                  i, args2.pathname, offsets[i] ));

      error = FT_Stream_New( library, &args2, &stream2 );
      अगर ( is_darwin_vfs && FT_ERR_EQ( error, Cannot_Open_Stream ) )
        vfs_rfork_has_no_font = TRUE;

      अगर ( error )
      {
        FT_TRACE3(( "failed\n" ));
        जारी;
      }

      error = IsMacResource( library, stream2, offsets[i],
                             face_सूचकांक, aface );
      FT_Stream_Free( stream2, 0 );

      FT_TRACE3(( "%s\n", error ? "failed": "successful" ));

      अगर ( !error )
          अवरोध;
      अन्यथा अगर ( is_darwin_vfs )
          vfs_rfork_has_no_font = TRUE;
    }

    क्रम (i = 0; i < FT_RACCESS_N_RULES; i++)
    {
      अगर ( file_names[i] )
        FT_FREE( file_names[i] );
    }

    /* Caller (load_mac_face) requires FT_Err_Unknown_File_Format. */
    अगर ( error )
      error = FT_ERR( Unknown_File_Format );

    वापस error;

#अघोषित  FT_COMPONENT
#घोषणा FT_COMPONENT  objs

  }


  /* Check for some macintosh formats without Carbon framework.    */
  /* Is this a macbinary file?  If so look at the resource fork.   */
  /* Is this a mac dfont file?                                     */
  /* Is this an old style resource fork? (in data)                 */
  /* Else call load_face_in_embedded_rfork to try extra rules      */
  /* (defined in `ftrfork.c').                                     */
  /*                                                               */
  अटल FT_Error
  load_mac_face( FT_Library           library,
                 FT_Stream            stream,
                 FT_Long              face_सूचकांक,
                 FT_Face             *aface,
                 स्थिर FT_Open_Args  *args )
  {
    FT_Error error;
    FT_UNUSED( args );


    error = Is_Mac_द्विआधारी( library, stream, face_सूचकांक, aface );
    अगर ( FT_ERR_EQ( error, Unknown_File_Format ) )
    {

#अघोषित  FT_COMPONENT
#घोषणा FT_COMPONENT  raccess

#अगर_घोषित FT_DEBUG_LEVEL_TRACE
      FT_TRACE3(( "Try as dfont: " ));
      अगर ( !( args->flags & FT_OPEN_MEMORY ) )
        FT_TRACE3(( "%s ...", args->pathname ));
#पूर्ण_अगर

      error = IsMacResource( library, stream, 0, face_सूचकांक, aface );

      FT_TRACE3(( "%s\n", error ? "failed" : "successful" ));

#अघोषित  FT_COMPONENT
#घोषणा FT_COMPONENT  objs

    }

    अगर ( ( FT_ERR_EQ( error, Unknown_File_Format )      ||
           FT_ERR_EQ( error, Invalid_Stream_Operation ) ) &&
         ( args->flags & FT_OPEN_PATHNAME )               )
      error = load_face_in_embedded_rfork( library, stream,
                                           face_सूचकांक, aface, args );
    वापस error;
  }
#पूर्ण_अगर

#पूर्ण_अगर  /* !FT_MACINTOSH && FT_CONFIG_OPTION_MAC_FONTS */


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Open_Face( FT_Library           library,
                स्थिर FT_Open_Args*  args,
                FT_Long              face_सूचकांक,
                FT_Face             *aface )
  {
    वापस ft_open_face_internal( library, args, face_सूचकांक, aface, 1 );
  }


  अटल FT_Error
  ft_open_face_internal( FT_Library           library,
                         स्थिर FT_Open_Args*  args,
                         FT_Long              face_सूचकांक,
                         FT_Face             *aface,
                         FT_Bool              test_mac_fonts )
  {
    FT_Error     error;
    FT_Driver    driver = शून्य;
    FT_Memory    memory = शून्य;
    FT_Stream    stream = शून्य;
    FT_Face      face   = शून्य;
    FT_List_गाँठ  गाँठ   = शून्य;
    FT_Bool      external_stream;
    FT_Module*   cur;
    FT_Module*   limit;

#अगर_अघोषित FT_CONFIG_OPTION_MAC_FONTS
    FT_UNUSED( test_mac_fonts );
#पूर्ण_अगर


    /* only use lower 31 bits together with sign bit */
    अगर ( face_सूचकांक > 0 )
      face_सूचकांक &= 0x7FFFFFFFL;
    अन्यथा
    {
      face_सूचकांक  = -face_सूचकांक;
      face_सूचकांक &= 0x7FFFFFFFL;
      face_सूचकांक  = -face_सूचकांक;
    }

#अगर_घोषित FT_DEBUG_LEVEL_TRACE
    FT_TRACE3(( "FT_Open_Face: " ));
    अगर ( face_सूचकांक < 0 )
      FT_TRACE3(( "Requesting number of faces and named instances\n"));
    अन्यथा
    {
      FT_TRACE3(( "Requesting face %ld", face_सूचकांक & 0xFFFFL ));
      अगर ( face_सूचकांक & 0x7FFF0000L )
        FT_TRACE3(( ", named instance %ld", face_सूचकांक >> 16 ));
      FT_TRACE3(( "\n" ));
    }
#पूर्ण_अगर

    /* test for valid `library' delayed to `FT_Stream_New' */

    अगर ( !args )
      वापस FT_THROW( Invalid_Argument );

    external_stream = FT_BOOL( ( args->flags & FT_OPEN_STREAM ) &&
                               args->stream                     );

    /* create input stream */
    error = FT_Stream_New( library, args, &stream );
    अगर ( error )
      जाओ Fail3;

    /* Do this error check after `FT_Stream_New` to ensure that the */
    /* 'close' callback is called.                                  */
    अगर ( !aface && face_सूचकांक >= 0 )
    {
      error = FT_THROW( Invalid_Argument );
      जाओ Fail3;
    }

    memory = library->memory;

    /* If the font driver is specified in the `args' structure, use */
    /* it.  Otherwise, we scan the list of registered drivers.      */
    अगर ( ( args->flags & FT_OPEN_DRIVER ) && args->driver )
    {
      driver = FT_DRIVER( args->driver );

      /* not all modules are drivers, so check... */
      अगर ( FT_MODULE_IS_DRIVER( driver ) )
      {
        FT_Int         अंक_params = 0;
        FT_Parameter*  params     = शून्य;


        अगर ( args->flags & FT_OPEN_PARAMS )
        {
          अंक_params = args->अंक_params;
          params     = args->params;
        }

        error = open_face( driver, &stream, &external_stream, face_सूचकांक,
                           अंक_params, params, &face );
        अगर ( !error )
          जाओ Success;
      }
      अन्यथा
        error = FT_THROW( Invalid_Handle );

      FT_Stream_Free( stream, external_stream );
      जाओ Fail;
    }
    अन्यथा
    {
      error = FT_ERR( Missing_Module );

      /* check each font driver for an appropriate format */
      cur   = library->modules;
      limit = cur + library->अंक_modules;

      क्रम ( ; cur < limit; cur++ )
      {
        /* not all modules are font drivers, so check... */
        अगर ( FT_MODULE_IS_DRIVER( cur[0] ) )
        {
          FT_Int         अंक_params = 0;
          FT_Parameter*  params     = शून्य;


          driver = FT_DRIVER( cur[0] );

          अगर ( args->flags & FT_OPEN_PARAMS )
          {
            अंक_params = args->अंक_params;
            params     = args->params;
          }

          error = open_face( driver, &stream, &external_stream, face_सूचकांक,
                             अंक_params, params, &face );
          अगर ( !error )
            जाओ Success;

#अगर_घोषित FT_CONFIG_OPTION_MAC_FONTS
          अगर ( test_mac_fonts                                           &&
               ft_strcmp( cur[0]->clazz->module_name, "truetype" ) == 0 &&
               FT_ERR_EQ( error, Table_Missing )                        )
          {
            /* TrueType but essential tables are missing */
            error = FT_Stream_Seek( stream, 0 );
            अगर ( error )
              अवरोध;

            error = open_face_PS_from_sfnt_stream( library,
                                                   stream,
                                                   face_सूचकांक,
                                                   अंक_params,
                                                   params,
                                                   aface );
            अगर ( !error )
            {
              FT_Stream_Free( stream, external_stream );
              वापस error;
            }
          }
#पूर्ण_अगर

          अगर ( FT_ERR_NEQ( error, Unknown_File_Format ) )
            जाओ Fail3;
        }
      }

    Fail3:
      /* If we are on the mac, and we get an                          */
      /* FT_Err_Invalid_Stream_Operation it may be because we have an */
      /* empty data fork, so we need to check the resource fork.      */
      अगर ( FT_ERR_NEQ( error, Cannot_Open_Stream )       &&
           FT_ERR_NEQ( error, Unknown_File_Format )      &&
           FT_ERR_NEQ( error, Invalid_Stream_Operation ) )
        जाओ Fail2;

#अगर !defined( FT_MACINTOSH ) && defined( FT_CONFIG_OPTION_MAC_FONTS )
      अगर ( test_mac_fonts )
      {
        error = load_mac_face( library, stream, face_सूचकांक, aface, args );
        अगर ( !error )
        {
          /* We don't want to go to Success here.  We've already done   */
          /* that.  On the other hand, if we succeeded we still need to */
          /* close this stream (we opened a different stream which      */
          /* extracted the interesting information out of this stream   */
          /* here.  That stream will still be open and the face will    */
          /* point to it).                                              */
          FT_Stream_Free( stream, external_stream );
          वापस error;
        }
      }

      अगर ( FT_ERR_NEQ( error, Unknown_File_Format ) )
        जाओ Fail2;
#पूर्ण_अगर  /* !FT_MACINTOSH && FT_CONFIG_OPTION_MAC_FONTS */

      /* no driver is able to handle this format */
      error = FT_THROW( Unknown_File_Format );

  Fail2:
      FT_Stream_Free( stream, external_stream );
      जाओ Fail;
    }

  Success:
    FT_TRACE4(( "FT_Open_Face: New face object, adding to list\n" ));

    /* add the face object to its driver's list */
    अगर ( FT_QNEW( गाँठ ) )
      जाओ Fail;

    गाँठ->तथ्य = face;
    /* don't assume driver is the same as face->driver, so use */
    /* face->driver instead.                                   */
    FT_List_Add( &face->driver->faces_list, गाँठ );

    /* now allocate a glyph slot object for the face */
    FT_TRACE4(( "FT_Open_Face: Creating glyph slot\n" ));

    अगर ( face_सूचकांक >= 0 )
    {
      error = FT_New_GlyphSlot( face, शून्य );
      अगर ( error )
        जाओ Fail;

      /* finally, allocate a size object for the face */
      {
        FT_आकार  आकार;


        FT_TRACE4(( "FT_Open_Face: Creating size object\n" ));

        error = FT_New_आकार( face, &आकार );
        अगर ( error )
          जाओ Fail;

        face->आकार = आकार;
      }
    }

    /* some checks */

    अगर ( FT_IS_SCALABLE( face ) )
    {
      अगर ( face->height < 0 )
        face->height = (FT_Short)-face->height;

      अगर ( !FT_HAS_VERTICAL( face ) )
        face->max_advance_height = (FT_Short)face->height;
    }

    अगर ( FT_HAS_FIXED_SIZES( face ) )
    {
      FT_Int  i;


      क्रम ( i = 0; i < face->अंक_fixed_sizes; i++ )
      {
        FT_Bitmap_आकार*  bsize = face->available_sizes + i;


        अगर ( bsize->height < 0 )
          bsize->height = -bsize->height;
        अगर ( bsize->x_ppem < 0 )
          bsize->x_ppem = -bsize->x_ppem;
        अगर ( bsize->y_ppem < 0 )
          bsize->y_ppem = -bsize->y_ppem;

        /* check whether negation actually has worked */
        अगर ( bsize->height < 0 || bsize->x_ppem < 0 || bsize->y_ppem < 0 )
        {
          FT_TRACE0(( "FT_Open_Face:"
                      " Invalid bitmap dimensions for strike %d,"
                      " now disabled\n", i ));
          bsize->width  = 0;
          bsize->height = 0;
          bsize->आकार   = 0;
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

    अगर ( aface )
      *aface = face;
    अन्यथा
      FT_Done_Face( face );

    जाओ Exit;

  Fail:
    अगर ( गाँठ )
      FT_Done_Face( face );    /* face must be in the driver's list */
    अन्यथा अगर ( face )
      destroy_face( memory, face, driver );

  Exit:
#अगर_घोषित FT_DEBUG_LEVEL_TRACE
    अगर ( !error && face_सूचकांक < 0 )
    {
      FT_TRACE3(( "FT_Open_Face: The font has %ld face%s\n",
                  face->अंक_faces,
                  face->अंक_faces == 1 ? "" : "s" ));
      FT_TRACE3(( "              and %ld named instance%s for face %ld\n",
                  face->style_flags >> 16,
                  ( face->style_flags >> 16 ) == 1 ? "" : "s",
                  -face_सूचकांक - 1 ));
    }
#पूर्ण_अगर

    FT_TRACE4(( "FT_Open_Face: Return 0x%x\n", error ));

    वापस error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Attach_File( FT_Face      face,
                  स्थिर अक्षर*  filepathname )
  {
    FT_Open_Args  खोलो;


    /* test for valid `face' delayed to `FT_Attach_Stream' */

    अगर ( !filepathname )
      वापस FT_THROW( Invalid_Argument );

    खोलो.stream   = शून्य;
    खोलो.flags    = FT_OPEN_PATHNAME;
    खोलो.pathname = (अक्षर*)filepathname;

    वापस FT_Attach_Stream( face, &खोलो );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Attach_Stream( FT_Face              face,
                    स्थिर FT_Open_Args*  parameters )
  {
    FT_Stream  stream;
    FT_Error   error;
    FT_Driver  driver;

    FT_Driver_Class  clazz;


    /* test for valid `parameters' delayed to `FT_Stream_New' */

    अगर ( !face )
      वापस FT_THROW( Invalid_Face_Handle );

    driver = face->driver;
    अगर ( !driver )
      वापस FT_THROW( Invalid_Driver_Handle );

    error = FT_Stream_New( driver->मूल.library, parameters, &stream );
    अगर ( error )
      जाओ Exit;

    /* we implement FT_Attach_Stream in each driver through the */
    /* `attach_file' interface                                  */

    error = FT_ERR( Unimplemented_Feature );
    clazz = driver->clazz;
    अगर ( clazz->attach_file )
      error = clazz->attach_file( face, stream );

    /* close the attached stream */
    FT_Stream_Free( stream,
                    FT_BOOL( parameters->stream                     &&
                             ( parameters->flags & FT_OPEN_STREAM ) ) );

  Exit:
    वापस error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Reference_Face( FT_Face  face )
  {
    अगर ( !face )
      वापस FT_THROW( Invalid_Face_Handle );

    face->internal->refcount++;

    वापस FT_Err_Ok;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Done_Face( FT_Face  face )
  {
    FT_Error     error;
    FT_Driver    driver;
    FT_Memory    memory;
    FT_List_गाँठ  गाँठ;


    error = FT_ERR( Invalid_Face_Handle );
    अगर ( face && face->driver )
    {
      face->internal->refcount--;
      अगर ( face->internal->refcount > 0 )
        error = FT_Err_Ok;
      अन्यथा
      {
        driver = face->driver;
        memory = driver->मूल.memory;

        /* find face in driver's list */
        गाँठ = FT_List_Find( &driver->faces_list, face );
        अगर ( गाँठ )
        {
          /* remove face object from the driver's list */
          FT_List_Remove( &driver->faces_list, गाँठ );
          FT_FREE( गाँठ );

          /* now destroy the object proper */
          destroy_face( memory, face, driver );
          error = FT_Err_Ok;
        }
      }
    }

    वापस error;
  }


  /* documentation is in ftobjs.h */

  FT_EXPORT_DEF( FT_Error )
  FT_New_आकार( FT_Face   face,
               FT_आकार  *asize )
  {
    FT_Error         error;
    FT_Memory        memory;
    FT_Driver        driver;
    FT_Driver_Class  clazz;

    FT_आकार          आकार = शून्य;
    FT_List_गाँठ      गाँठ = शून्य;

    FT_आकार_Internal  internal = शून्य;


    अगर ( !face )
      वापस FT_THROW( Invalid_Face_Handle );

    अगर ( !asize )
      वापस FT_THROW( Invalid_Argument );

    अगर ( !face->driver )
      वापस FT_THROW( Invalid_Driver_Handle );

    *asize = शून्य;

    driver = face->driver;
    clazz  = driver->clazz;
    memory = face->memory;

    /* Allocate new size object and perform basic initialisation */
    अगर ( FT_ALLOC( आकार, clazz->आकार_object_आकार ) || FT_QNEW( गाँठ ) )
      जाओ Exit;

    आकार->face = face;

    अगर ( FT_NEW( internal ) )
      जाओ Exit;

    आकार->internal = internal;

    अगर ( clazz->init_आकार )
      error = clazz->init_आकार( आकार );

    /* in case of success, add to the face's list */
    अगर ( !error )
    {
      *asize     = आकार;
      गाँठ->तथ्य = आकार;
      FT_List_Add( &face->sizes_list, गाँठ );
    }

  Exit:
    अगर ( error )
    {
      FT_FREE( गाँठ );
      अगर ( आकार )
        FT_FREE( आकार->internal );
      FT_FREE( आकार );
    }

    वापस error;
  }


  /* documentation is in ftobjs.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Done_आकार( FT_आकार  आकार )
  {
    FT_Error     error;
    FT_Driver    driver;
    FT_Memory    memory;
    FT_Face      face;
    FT_List_गाँठ  गाँठ;


    अगर ( !आकार )
      वापस FT_THROW( Invalid_आकार_Handle );

    face = आकार->face;
    अगर ( !face )
      वापस FT_THROW( Invalid_Face_Handle );

    driver = face->driver;
    अगर ( !driver )
      वापस FT_THROW( Invalid_Driver_Handle );

    memory = driver->मूल.memory;

    error = FT_Err_Ok;
    गाँठ  = FT_List_Find( &face->sizes_list, आकार );
    अगर ( गाँठ )
    {
      FT_List_Remove( &face->sizes_list, गाँठ );
      FT_FREE( गाँठ );

      अगर ( face->आकार == आकार )
      {
        face->आकार = शून्य;
        अगर ( face->sizes_list.शीर्ष_2 )
          face->आकार = (FT_आकार)(face->sizes_list.शीर्ष_2->तथ्य);
      }

      destroy_आकार( memory, आकार, driver );
    }
    अन्यथा
      error = FT_THROW( Invalid_आकार_Handle );

    वापस error;
  }


  /* documentation is in ftobjs.h */

  FT_BASE_DEF( FT_Error )
  FT_Match_आकार( FT_Face          face,
                 FT_आकार_Request  req,
                 FT_Bool          ignore_width,
                 FT_ULong*        आकार_सूचकांक )
  {
    FT_Int   i;
    FT_Long  w, h;


    अगर ( !FT_HAS_FIXED_SIZES( face ) )
      वापस FT_THROW( Invalid_Face_Handle );

    /* FT_Bitmap_Size doesn't provide enough info... */
    अगर ( req->type != FT_आकार_REQUEST_TYPE_NOMINAL )
      वापस FT_THROW( Unimplemented_Feature );

    w = FT_REQUEST_WIDTH ( req );
    h = FT_REQUEST_HEIGHT( req );

    अगर ( req->width && !req->height )
      h = w;
    अन्यथा अगर ( !req->width && req->height )
      w = h;

    w = FT_PIX_ROUND( w );
    h = FT_PIX_ROUND( h );

    अगर ( !w || !h )
      वापस FT_THROW( Invalid_Pixel_आकार );

    क्रम ( i = 0; i < face->अंक_fixed_sizes; i++ )
    {
      FT_Bitmap_आकार*  bsize = face->available_sizes + i;


      अगर ( h != FT_PIX_ROUND( bsize->y_ppem ) )
        जारी;

      अगर ( w == FT_PIX_ROUND( bsize->x_ppem ) || ignore_width )
      {
        FT_TRACE3(( "FT_Match_Size: bitmap strike %d matches\n", i ));

        अगर ( आकार_सूचकांक )
          *आकार_सूचकांक = (FT_ULong)i;

        वापस FT_Err_Ok;
      }
    }

    FT_TRACE3(( "FT_Match_Size: no matching bitmap strike\n" ));

    वापस FT_THROW( Invalid_Pixel_आकार );
  }


  /* documentation is in ftobjs.h */

  FT_BASE_DEF( व्योम )
  ft_synthesize_vertical_metrics( FT_Glyph_Metrics*  metrics,
                                  FT_Pos             advance )
  {
    FT_Pos  height = metrics->height;


    /* compensate for glyph with bbox above/below the baseline */
    अगर ( metrics->horiBearingY < 0 )
    {
      अगर ( height < metrics->horiBearingY )
        height = metrics->horiBearingY;
    }
    अन्यथा अगर ( metrics->horiBearingY > 0 )
      height -= metrics->horiBearingY;

    /* the factor 1.2 is a heuristical value */
    अगर ( !advance )
      advance = height * 12 / 10;

    metrics->vertBearingX = metrics->horiBearingX - metrics->horiAdvance / 2;
    metrics->vertBearingY = ( advance - height ) / 2;
    metrics->vertAdvance  = advance;
  }


  अटल व्योम
  ft_recompute_scaled_metrics( FT_Face           face,
                               FT_आकार_Metrics*  metrics )
  {
    /* Compute root ascender, descender, test height, and max_advance */

#अगर_घोषित GRID_FIT_METRICS
    metrics->ascender    = FT_PIX_CEIL( FT_MulFix( face->ascender,
                                                   metrics->y_scale ) );

    metrics->descender   = FT_PIX_FLOOR( FT_MulFix( face->descender,
                                                    metrics->y_scale ) );

    metrics->height      = FT_PIX_ROUND( FT_MulFix( face->height,
                                                    metrics->y_scale ) );

    metrics->max_advance = FT_PIX_ROUND( FT_MulFix( face->max_advance_width,
                                                    metrics->x_scale ) );
#अन्यथा /* !GRID_FIT_METRICS */
    metrics->ascender    = FT_MulFix( face->ascender,
                                      metrics->y_scale );

    metrics->descender   = FT_MulFix( face->descender,
                                      metrics->y_scale );

    metrics->height      = FT_MulFix( face->height,
                                      metrics->y_scale );

    metrics->max_advance = FT_MulFix( face->max_advance_width,
                                      metrics->x_scale );
#पूर्ण_अगर /* !GRID_FIT_METRICS */
  }


  FT_BASE_DEF( व्योम )
  FT_Select_Metrics( FT_Face   face,
                     FT_ULong  strike_सूचकांक )
  {
    FT_आकार_Metrics*  metrics;
    FT_Bitmap_आकार*   bsize;


    metrics = &face->आकार->metrics;
    bsize   = face->available_sizes + strike_सूचकांक;

    metrics->x_ppem = (FT_UShort)( ( bsize->x_ppem + 32 ) >> 6 );
    metrics->y_ppem = (FT_UShort)( ( bsize->y_ppem + 32 ) >> 6 );

    अगर ( FT_IS_SCALABLE( face ) )
    {
      metrics->x_scale = FT_DivFix( bsize->x_ppem,
                                    face->units_per_EM );
      metrics->y_scale = FT_DivFix( bsize->y_ppem,
                                    face->units_per_EM );

      ft_recompute_scaled_metrics( face, metrics );
    }
    अन्यथा
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
                      FT_आकार_Request  req )
  {
    FT_Error  error = FT_Err_Ok;

    FT_आकार_Metrics*  metrics;


    metrics = &face->आकार->metrics;

    अगर ( FT_IS_SCALABLE( face ) )
    {
      FT_Long  w = 0, h = 0, scaled_w = 0, scaled_h = 0;


      चयन ( req->type )
      {
      हाल FT_आकार_REQUEST_TYPE_NOMINAL:
        w = h = face->units_per_EM;
        अवरोध;

      हाल FT_आकार_REQUEST_TYPE_REAL_DIM:
        w = h = face->ascender - face->descender;
        अवरोध;

      हाल FT_आकार_REQUEST_TYPE_BBOX:
        w = face->bbox.xMax - face->bbox.xMin;
        h = face->bbox.yMax - face->bbox.yMin;
        अवरोध;

      हाल FT_आकार_REQUEST_TYPE_CELL:
        w = face->max_advance_width;
        h = face->ascender - face->descender;
        अवरोध;

      हाल FT_आकार_REQUEST_TYPE_SCALES:
        metrics->x_scale = (FT_Fixed)req->width;
        metrics->y_scale = (FT_Fixed)req->height;
        अगर ( !metrics->x_scale )
          metrics->x_scale = metrics->y_scale;
        अन्यथा अगर ( !metrics->y_scale )
          metrics->y_scale = metrics->x_scale;
        जाओ Calculate_Ppem;

      हाल FT_आकार_REQUEST_TYPE_MAX:
        अवरोध;
      }

      /* to be on the safe side */
      अगर ( w < 0 )
        w = -w;

      अगर ( h < 0 )
        h = -h;

      scaled_w = FT_REQUEST_WIDTH ( req );
      scaled_h = FT_REQUEST_HEIGHT( req );

      /* determine scales */
      अगर ( req->height || !req->width )
      {
        अगर ( h == 0 )
        {
          FT_ERROR(( "FT_Request_Metrics: Divide by zero\n" ));
          error = FT_ERR( Divide_By_Zero );
          जाओ Exit;
        }

        metrics->y_scale = FT_DivFix( scaled_h, h );
      }

      अगर ( req->width )
      {
        अगर ( w == 0 )
        {
          FT_ERROR(( "FT_Request_Metrics: Divide by zero\n" ));
          error = FT_ERR( Divide_By_Zero );
          जाओ Exit;
        }

        metrics->x_scale = FT_DivFix( scaled_w, w );
      }
      अन्यथा
      {
        metrics->x_scale = metrics->y_scale;
        scaled_w = FT_MulDiv( scaled_h, w, h );
      }

      अगर ( !req->height )
      {
        metrics->y_scale = metrics->x_scale;
        scaled_h = FT_MulDiv( scaled_w, h, w );
      }

      अगर ( req->type == FT_आकार_REQUEST_TYPE_CELL )
      {
        अगर ( metrics->y_scale > metrics->x_scale )
          metrics->y_scale = metrics->x_scale;
        अन्यथा
          metrics->x_scale = metrics->y_scale;
      }

  Calculate_Ppem:
      /* calculate the ppems */
      अगर ( req->type != FT_आकार_REQUEST_TYPE_NOMINAL )
      {
        scaled_w = FT_MulFix( face->units_per_EM, metrics->x_scale );
        scaled_h = FT_MulFix( face->units_per_EM, metrics->y_scale );
      }

      scaled_w = ( scaled_w + 32 ) >> 6;
      scaled_h = ( scaled_h + 32 ) >> 6;
      अगर ( scaled_w > (FT_Long)FT_USHORT_MAX ||
           scaled_h > (FT_Long)FT_USHORT_MAX )
      {
        FT_ERROR(( "FT_Request_Metrics: Resulting ppem size too large\n" ));
        error = FT_ERR( Invalid_Pixel_आकार );
        जाओ Exit;
      }

      metrics->x_ppem = (FT_UShort)scaled_w;
      metrics->y_ppem = (FT_UShort)scaled_h;

      ft_recompute_scaled_metrics( face, metrics );
    }
    अन्यथा
    {
      FT_ZERO( metrics );
      metrics->x_scale = 1L << 16;
      metrics->y_scale = 1L << 16;
    }

  Exit:
    वापस error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Select_आकार( FT_Face  face,
                  FT_Int   strike_सूचकांक )
  {
    FT_Error         error = FT_Err_Ok;
    FT_Driver_Class  clazz;


    अगर ( !face || !FT_HAS_FIXED_SIZES( face ) )
      वापस FT_THROW( Invalid_Face_Handle );

    अगर ( strike_सूचकांक < 0 || strike_सूचकांक >= face->अंक_fixed_sizes )
      वापस FT_THROW( Invalid_Argument );

    clazz = face->driver->clazz;

    अगर ( clazz->select_आकार )
    {
      error = clazz->select_आकार( face->आकार, (FT_ULong)strike_सूचकांक );

      FT_TRACE5(( "FT_Select_Size (%s driver):\n",
                  face->driver->मूल.clazz->module_name ));
    }
    अन्यथा
    {
      FT_Select_Metrics( face, (FT_ULong)strike_सूचकांक );

      FT_TRACE5(( "FT_Select_Size:\n" ));
    }

#अगर_घोषित FT_DEBUG_LEVEL_TRACE
    {
      FT_आकार_Metrics*  metrics = &face->आकार->metrics;


      FT_TRACE5(( "  x scale: %ld (%f)\n",
                  metrics->x_scale, (द्विगुन)metrics->x_scale / 65536 ));
      FT_TRACE5(( "  y scale: %ld (%f)\n",
                  metrics->y_scale, (द्विगुन)metrics->y_scale / 65536 ));
      FT_TRACE5(( "  ascender: %f\n",
                  (द्विगुन)metrics->ascender / 64 ));
      FT_TRACE5(( "  descender: %f\n",
                  (द्विगुन)metrics->descender / 64 ));
      FT_TRACE5(( "  height: %f\n",
                  (द्विगुन)metrics->height / 64 ));
      FT_TRACE5(( "  max advance: %f\n",
                  (द्विगुन)metrics->max_advance / 64 ));
      FT_TRACE5(( "  x ppem: %d\n", metrics->x_ppem ));
      FT_TRACE5(( "  y ppem: %d\n", metrics->y_ppem ));
    }
#पूर्ण_अगर

    वापस error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Request_आकार( FT_Face          face,
                   FT_आकार_Request  req )
  {
    FT_Error         error;
    FT_Driver_Class  clazz;
    FT_ULong         strike_सूचकांक;


    अगर ( !face )
      वापस FT_THROW( Invalid_Face_Handle );

    अगर ( !face->आकार )
      वापस FT_THROW( Invalid_आकार_Handle );

    अगर ( !req || req->width < 0 || req->height < 0 ||
         req->type >= FT_आकार_REQUEST_TYPE_MAX )
      वापस FT_THROW( Invalid_Argument );

    /* signal the auto-hinter to recompute its size metrics */
    /* (if requested)                                       */
    face->आकार->internal->autohint_metrics.x_scale = 0;

    clazz = face->driver->clazz;

    अगर ( clazz->request_आकार )
    {
      error = clazz->request_आकार( face->आकार, req );

      FT_TRACE5(( "FT_Request_Size (%s driver):\n",
                  face->driver->मूल.clazz->module_name ));
    }
    अन्यथा अगर ( !FT_IS_SCALABLE( face ) && FT_HAS_FIXED_SIZES( face ) )
    {
      /*
       * The reason that a driver doesn't have `request_size' defined is
       * either that the scaling here suffices or that the supported formats
       * are bitmap-only and size matching is not implemented.
       *
       * In the latter case, a simple size matching is done.
       */
      error = FT_Match_आकार( face, req, 0, &strike_सूचकांक );
      अगर ( error )
        जाओ Exit;

      वापस FT_Select_आकार( face, (FT_Int)strike_सूचकांक );
    }
    अन्यथा
    {
      error = FT_Request_Metrics( face, req );
      अगर ( error )
        जाओ Exit;

      FT_TRACE5(( "FT_Request_Size:\n" ));
    }

#अगर_घोषित FT_DEBUG_LEVEL_TRACE
    {
      FT_आकार_Metrics*  metrics = &face->आकार->metrics;


      FT_TRACE5(( "  x scale: %ld (%f)\n",
                  metrics->x_scale, (द्विगुन)metrics->x_scale / 65536 ));
      FT_TRACE5(( "  y scale: %ld (%f)\n",
                  metrics->y_scale, (द्विगुन)metrics->y_scale / 65536 ));
      FT_TRACE5(( "  ascender: %f\n",
                  (द्विगुन)metrics->ascender / 64 ));
      FT_TRACE5(( "  descender: %f\n",
                  (द्विगुन)metrics->descender / 64 ));
      FT_TRACE5(( "  height: %f\n",
                  (द्विगुन)metrics->height / 64 ));
      FT_TRACE5(( "  max advance: %f\n",
                  (द्विगुन)metrics->max_advance / 64 ));
      FT_TRACE5(( "  x ppem: %d\n", metrics->x_ppem ));
      FT_TRACE5(( "  y ppem: %d\n", metrics->y_ppem ));
    }
#पूर्ण_अगर

  Exit:
    वापस error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Set_Char_आकार( FT_Face     face,
                    FT_F26Dot6  char_width,
                    FT_F26Dot6  char_height,
                    FT_UInt     horz_resolution,
                    FT_UInt     vert_resolution )
  {
    FT_आकार_Request_Rec  req;


    /* check of `face' delayed to `FT_Request_Size' */

    अगर ( !char_width )
      char_width = char_height;
    अन्यथा अगर ( !char_height )
      char_height = char_width;

    अगर ( !horz_resolution )
      horz_resolution = vert_resolution;
    अन्यथा अगर ( !vert_resolution )
      vert_resolution = horz_resolution;

    अगर ( char_width  < 1 * 64 )
      char_width  = 1 * 64;
    अगर ( char_height < 1 * 64 )
      char_height = 1 * 64;

    अगर ( !horz_resolution )
      horz_resolution = vert_resolution = 72;

    req.type           = FT_आकार_REQUEST_TYPE_NOMINAL;
    req.width          = char_width;
    req.height         = char_height;
    req.horiResolution = horz_resolution;
    req.vertResolution = vert_resolution;

    वापस FT_Request_आकार( face, &req );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Set_Pixel_Sizes( FT_Face  face,
                      FT_UInt  pixel_width,
                      FT_UInt  pixel_height )
  {
    FT_आकार_Request_Rec  req;


    /* check of `face' delayed to `FT_Request_Size' */

    अगर ( pixel_width == 0 )
      pixel_width = pixel_height;
    अन्यथा अगर ( pixel_height == 0 )
      pixel_height = pixel_width;

    अगर ( pixel_width  < 1 )
      pixel_width  = 1;
    अगर ( pixel_height < 1 )
      pixel_height = 1;

    /* use `>=' to avoid potential compiler warning on 16bit platforms */
    अगर ( pixel_width >= 0xFFFFU )
      pixel_width = 0xFFFFU;
    अगर ( pixel_height >= 0xFFFFU )
      pixel_height = 0xFFFFU;

    req.type           = FT_आकार_REQUEST_TYPE_NOMINAL;
    req.width          = (FT_Long)( pixel_width << 6 );
    req.height         = (FT_Long)( pixel_height << 6 );
    req.horiResolution = 0;
    req.vertResolution = 0;

    वापस FT_Request_आकार( face, &req );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Kerning( FT_Face     face,
                  FT_UInt     बायाँ_glyph,
                  FT_UInt     दायाँ_glyph,
                  FT_UInt     kern_mode,
                  FT_Vector  *akerning )
  {
    FT_Error   error = FT_Err_Ok;
    FT_Driver  driver;


    अगर ( !face )
      वापस FT_THROW( Invalid_Face_Handle );

    अगर ( !akerning )
      वापस FT_THROW( Invalid_Argument );

    driver = face->driver;

    akerning->x = 0;
    akerning->y = 0;

    अगर ( driver->clazz->get_kerning )
    {
      error = driver->clazz->get_kerning( face,
                                          बायाँ_glyph,
                                          दायाँ_glyph,
                                          akerning );
      अगर ( !error )
      {
        अगर ( kern_mode != FT_KERNING_UNSCALED )
        {
          akerning->x = FT_MulFix( akerning->x, face->आकार->metrics.x_scale );
          akerning->y = FT_MulFix( akerning->y, face->आकार->metrics.y_scale );

          अगर ( kern_mode != FT_KERNING_UNFITTED )
          {
            FT_Pos  orig_x = akerning->x;
            FT_Pos  orig_y = akerning->y;


            /* we scale down kerning values for small ppem values */
            /* to avoid that rounding makes them too big.         */
            /* `25' has been determined heuristically.            */
            अगर ( face->आकार->metrics.x_ppem < 25 )
              akerning->x = FT_MulDiv( orig_x,
                                       face->आकार->metrics.x_ppem, 25 );
            अगर ( face->आकार->metrics.y_ppem < 25 )
              akerning->y = FT_MulDiv( orig_y,
                                       face->आकार->metrics.y_ppem, 25 );

            akerning->x = FT_PIX_ROUND( akerning->x );
            akerning->y = FT_PIX_ROUND( akerning->y );

#अगर_घोषित FT_DEBUG_LEVEL_TRACE
            {
              FT_Pos  orig_x_rounded = FT_PIX_ROUND( orig_x );
              FT_Pos  orig_y_rounded = FT_PIX_ROUND( orig_y );


              अगर ( akerning->x != orig_x_rounded ||
                   akerning->y != orig_y_rounded )
                FT_TRACE5(( "FT_Get_Kerning: horizontal kerning"
                            " (%ld, %ld) scaled down to (%ld, %ld) pixels\n",
                            orig_x_rounded / 64, orig_y_rounded / 64,
                            akerning->x / 64, akerning->y / 64 ));
            }
#पूर्ण_अगर
          }
        }
      }
    }

    वापस error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Track_Kerning( FT_Face    face,
                        FT_Fixed   point_आकार,
                        FT_Int     degree,
                        FT_Fixed*  akerning )
  {
    FT_Service_Kerning  service;
    FT_Error            error = FT_Err_Ok;


    अगर ( !face )
      वापस FT_THROW( Invalid_Face_Handle );

    अगर ( !akerning )
      वापस FT_THROW( Invalid_Argument );

    FT_FACE_FIND_SERVICE( face, service, KERNING );
    अगर ( !service )
      वापस FT_THROW( Unimplemented_Feature );

    error = service->get_track( face,
                                point_आकार,
                                degree,
                                akerning );

    वापस error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Select_Charmap( FT_Face      face,
                     FT_Encoding  encoding )
  {
    FT_CharMap*  cur;
    FT_CharMap*  limit;


    अगर ( !face )
      वापस FT_THROW( Invalid_Face_Handle );

    /* FT_ENCODING_NONE is a valid encoding for BDF, PCF, and Windows FNT */
    अगर ( encoding == FT_ENCODING_NONE && !face->अंक_charmaps )
      वापस FT_THROW( Invalid_Argument );

    /* FT_ENCODING_UNICODE is special.  We try to find the `best' Unicode */
    /* charmap available, i.e., one with UCS-4 characters, if possible.   */
    /*                                                                    */
    /* This is done by find_unicode_charmap() above, to share code.       */
    अगर ( encoding == FT_ENCODING_UNICODE )
      वापस find_unicode_charmap( face );

    cur = face->charmaps;
    अगर ( !cur )
      वापस FT_THROW( Invalid_CharMap_Handle );

    limit = cur + face->अंक_charmaps;

    क्रम ( ; cur < limit; cur++ )
    {
      अगर ( cur[0]->encoding == encoding )
      {
        face->charmap = cur[0];
        वापस FT_Err_Ok;
      }
    }

    वापस FT_THROW( Invalid_Argument );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Set_Charmap( FT_Face     face,
                  FT_CharMap  charmap )
  {
    FT_CharMap*  cur;
    FT_CharMap*  limit;


    अगर ( !face )
      वापस FT_THROW( Invalid_Face_Handle );

    cur = face->charmaps;
    अगर ( !cur || !charmap )
      वापस FT_THROW( Invalid_CharMap_Handle );

    limit = cur + face->अंक_charmaps;

    क्रम ( ; cur < limit; cur++ )
    {
      अगर ( cur[0] == charmap )
      {
        FT_Long  format = FT_Get_CMap_Format( charmap );


        अगर ( format != 14 && format != 15 )
        {
          face->charmap = cur[0];
          वापस FT_Err_Ok;
        }
      }
    }

    वापस FT_THROW( Invalid_Argument );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Int )
  FT_Get_Charmap_सूचकांक( FT_CharMap  charmap )
  {
    FT_Int  i;


    अगर ( !charmap || !charmap->face )
      वापस -1;

    क्रम ( i = 0; i < charmap->face->अंक_charmaps; i++ )
      अगर ( charmap->face->charmaps[i] == charmap )
        अवरोध;

    FT_ASSERT( i < charmap->face->अंक_charmaps );

    वापस i;
  }


  अटल व्योम
  ft_cmap_done_internal( FT_CMap  cmap )
  {
    FT_CMap_Class  clazz  = cmap->clazz;
    FT_Face        face   = cmap->charmap.face;
    FT_Memory      memory = FT_FACE_MEMORY( face );


    अगर ( clazz->done )
      clazz->done( cmap );

    FT_FREE( cmap );
  }


  FT_BASE_DEF( व्योम )
  FT_CMap_Done( FT_CMap  cmap )
  {
    अगर ( cmap )
    {
      FT_Face    face   = cmap->charmap.face;
      FT_Memory  memory = FT_FACE_MEMORY( face );
      FT_Error   error;
      FT_Int     i, j;


      क्रम ( i = 0; i < face->अंक_charmaps; i++ )
      {
        अगर ( (FT_CMap)face->charmaps[i] == cmap )
        {
          FT_CharMap  last_charmap = face->charmaps[face->अंक_charmaps - 1];


          अगर ( FT_QRENEW_ARRAY( face->charmaps,
                                face->अंक_charmaps,
                                face->अंक_charmaps - 1 ) )
            वापस;

          /* remove it from our list of charmaps */
          क्रम ( j = i + 1; j < face->अंक_charmaps; j++ )
          {
            अगर ( j == face->अंक_charmaps - 1 )
              face->charmaps[j - 1] = last_charmap;
            अन्यथा
              face->charmaps[j - 1] = face->charmaps[j];
          }

          face->अंक_charmaps--;

          अगर ( (FT_CMap)face->charmap == cmap )
            face->charmap = शून्य;

          ft_cmap_done_internal( cmap );

          अवरोध;
        }
      }
    }
  }


  FT_BASE_DEF( FT_Error )
  FT_CMap_New( FT_CMap_Class  clazz,
               FT_Pointer     init_तथ्य,
               FT_CharMap     charmap,
               FT_CMap       *acmap )
  {
    FT_Error   error;
    FT_Face    face;
    FT_Memory  memory;
    FT_CMap    cmap = शून्य;


    अगर ( !clazz || !charmap || !charmap->face )
      वापस FT_THROW( Invalid_Argument );

    face   = charmap->face;
    memory = FT_FACE_MEMORY( face );

    अगर ( !FT_ALLOC( cmap, clazz->आकार ) )
    {
      cmap->charmap = *charmap;
      cmap->clazz   = clazz;

      अगर ( clazz->init )
      {
        error = clazz->init( cmap, init_तथ्य );
        अगर ( error )
          जाओ Fail;
      }

      /* add it to our list of charmaps */
      अगर ( FT_QRENEW_ARRAY( face->charmaps,
                            face->अंक_charmaps,
                            face->अंक_charmaps + 1 ) )
        जाओ Fail;

      face->charmaps[face->अंक_charmaps++] = (FT_CharMap)cmap;
    }

  Exit:
    अगर ( acmap )
      *acmap = cmap;

    वापस error;

  Fail:
    ft_cmap_done_internal( cmap );
    cmap = शून्य;
    जाओ Exit;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt )
  FT_Get_Char_सूचकांक( FT_Face   face,
                     FT_ULong  charcode )
  {
    FT_UInt  परिणाम = 0;


    अगर ( face && face->charmap )
    {
      FT_CMap  cmap = FT_CMAP( face->charmap );


      अगर ( charcode > 0xFFFFFFFFUL )
      {
        FT_TRACE1(( "FT_Get_Char_Index: too large charcode" ));
        FT_TRACE1(( " 0x%lx is truncated\n", charcode ));
      }

      परिणाम = cmap->clazz->char_सूचकांक( cmap, (FT_UInt32)charcode );
      अगर ( परिणाम >= (FT_UInt)face->अंक_glyphs )
        परिणाम = 0;
    }

    वापस परिणाम;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_ULong )
  FT_Get_First_Char( FT_Face   face,
                     FT_UInt  *agindex )
  {
    FT_ULong  परिणाम = 0;
    FT_UInt   gindex = 0;


    /* only do something if we have a charmap, and we have glyphs at all */
    अगर ( face && face->charmap && face->अंक_glyphs )
    {
      gindex = FT_Get_Char_सूचकांक( face, 0 );
      अगर ( gindex == 0 )
        परिणाम = FT_Get_अग्र_Char( face, 0, &gindex );
    }

    अगर ( agindex )
      *agindex = gindex;

    वापस परिणाम;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_ULong )
  FT_Get_अग्र_Char( FT_Face   face,
                    FT_ULong  charcode,
                    FT_UInt  *agindex )
  {
    FT_ULong  परिणाम = 0;
    FT_UInt   gindex = 0;


    अगर ( face && face->charmap && face->अंक_glyphs )
    {
      FT_UInt32  code = (FT_UInt32)charcode;
      FT_CMap    cmap = FT_CMAP( face->charmap );


      करो
      {
        gindex = cmap->clazz->char_अग्र( cmap, &code );

      } जबतक ( gindex >= (FT_UInt)face->अंक_glyphs );

      परिणाम = ( gindex == 0 ) ? 0 : code;
    }

    अगर ( agindex )
      *agindex = gindex;

    वापस परिणाम;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Face_Properties( FT_Face        face,
                      FT_UInt        अंक_properties,
                      FT_Parameter*  properties )
  {
    FT_Error  error = FT_Err_Ok;


    अगर ( अंक_properties > 0 && !properties )
    {
      error = FT_THROW( Invalid_Argument );
      जाओ Exit;
    }

    क्रम ( ; अंक_properties > 0; अंक_properties-- )
    {
      अगर ( properties->tag == FT_PARAM_TAG_STEM_DARKENING )
      {
        अगर ( properties->तथ्य )
        {
          अगर ( *( (FT_Bool*)properties->तथ्य ) == TRUE )
            face->internal->no_stem_darkening = FALSE;
          अन्यथा
            face->internal->no_stem_darkening = TRUE;
        }
        अन्यथा
        {
          /* use module default */
          face->internal->no_stem_darkening = -1;
        }
      }
      अन्यथा अगर ( properties->tag == FT_PARAM_TAG_LCD_FILTER_WEIGHTS )
      {
        error = FT_THROW( Unimplemented_Feature );
        जाओ Exit;
      }
      अन्यथा अगर ( properties->tag == FT_PARAM_TAG_RANDOM_SEED )
      {
        अगर ( properties->तथ्य )
        {
          face->internal->random_seed = *( (FT_Int32*)properties->तथ्य );
          अगर ( face->internal->random_seed < 0 )
            face->internal->random_seed = 0;
        }
        अन्यथा
        {
          /* use module default */
          face->internal->random_seed = -1;
        }
      }
      अन्यथा
      {
        error = FT_THROW( Invalid_Argument );
        जाओ Exit;
      }

      अगर ( error )
        अवरोध;

      properties++;
    }

  Exit:
    वापस error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt )
  FT_Face_Get_Char_Variant_सूचकांक( FT_Face   face,
                               FT_ULong  charcode,
                               FT_ULong  variantSelector )
  {
    FT_UInt  परिणाम = 0;


    अगर ( face                                           &&
         face->charmap                                  &&
         face->charmap->encoding == FT_ENCODING_UNICODE )
    {
      FT_CharMap  charmap = find_variant_selector_charmap( face );
      FT_CMap     ucmap = FT_CMAP( face->charmap );


      अगर ( charmap )
      {
        FT_CMap  vcmap = FT_CMAP( charmap );


        अगर ( charcode > 0xFFFFFFFFUL )
        {
          FT_TRACE1(( "FT_Face_GetCharVariantIndex:"
                      " too large charcode" ));
          FT_TRACE1(( " 0x%lx is truncated\n", charcode ));
        }
        अगर ( variantSelector > 0xFFFFFFFFUL )
        {
          FT_TRACE1(( "FT_Face_GetCharVariantIndex:"
                      " too large variantSelector" ));
          FT_TRACE1(( " 0x%lx is truncated\n", variantSelector ));
        }

        परिणाम = vcmap->clazz->char_var_सूचकांक( vcmap, ucmap,
                                               (FT_UInt32)charcode,
                                               (FT_UInt32)variantSelector );
      }
    }

    वापस परिणाम;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Int )
  FT_Face_GetCharVariantIsDefault( FT_Face   face,
                                   FT_ULong  charcode,
                                   FT_ULong  variantSelector )
  {
    FT_Int  परिणाम = -1;


    अगर ( face )
    {
      FT_CharMap  charmap = find_variant_selector_charmap( face );


      अगर ( charmap )
      {
        FT_CMap  vcmap = FT_CMAP( charmap );


        अगर ( charcode > 0xFFFFFFFFUL )
        {
          FT_TRACE1(( "FT_Face_GetCharVariantIsDefault:"
                      " too large charcode" ));
          FT_TRACE1(( " 0x%lx is truncated\n", charcode ));
        }
        अगर ( variantSelector > 0xFFFFFFFFUL )
        {
          FT_TRACE1(( "FT_Face_GetCharVariantIsDefault:"
                      " too large variantSelector" ));
          FT_TRACE1(( " 0x%lx is truncated\n", variantSelector ));
        }

        परिणाम = vcmap->clazz->char_var_default( vcmap,
                                                 (FT_UInt32)charcode,
                                                 (FT_UInt32)variantSelector );
      }
    }

    वापस परिणाम;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt32* )
  FT_Face_GetVariantSelectors( FT_Face  face )
  {
    FT_UInt32  *परिणाम = शून्य;


    अगर ( face )
    {
      FT_CharMap  charmap = find_variant_selector_charmap( face );


      अगर ( charmap )
      {
        FT_CMap    vcmap  = FT_CMAP( charmap );
        FT_Memory  memory = FT_FACE_MEMORY( face );


        परिणाम = vcmap->clazz->variant_list( vcmap, memory );
      }
    }

    वापस परिणाम;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt32* )
  FT_Face_GetVariantsOfChar( FT_Face   face,
                             FT_ULong  charcode )
  {
    FT_UInt32  *परिणाम = शून्य;


    अगर ( face )
    {
      FT_CharMap  charmap = find_variant_selector_charmap( face );


      अगर ( charmap )
      {
        FT_CMap    vcmap  = FT_CMAP( charmap );
        FT_Memory  memory = FT_FACE_MEMORY( face );


        अगर ( charcode > 0xFFFFFFFFUL )
        {
          FT_TRACE1(( "FT_Face_GetVariantsOfChar: too large charcode" ));
          FT_TRACE1(( " 0x%lx is truncated\n", charcode ));
        }

        परिणाम = vcmap->clazz->charvariant_list( vcmap, memory,
                                                 (FT_UInt32)charcode );
      }
    }
    वापस परिणाम;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt32* )
  FT_Face_GetCharsOfVariant( FT_Face   face,
                             FT_ULong  variantSelector )
  {
    FT_UInt32  *परिणाम = शून्य;


    अगर ( face )
    {
      FT_CharMap  charmap = find_variant_selector_charmap( face );


      अगर ( charmap )
      {
        FT_CMap    vcmap  = FT_CMAP( charmap );
        FT_Memory  memory = FT_FACE_MEMORY( face );


        अगर ( variantSelector > 0xFFFFFFFFUL )
        {
          FT_TRACE1(( "FT_Get_Char_Index: too large variantSelector" ));
          FT_TRACE1(( " 0x%lx is truncated\n", variantSelector ));
        }

        परिणाम = vcmap->clazz->variantchar_list( vcmap, memory,
                                                 (FT_UInt32)variantSelector );
      }
    }

    वापस परिणाम;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt )
  FT_Get_Name_सूचकांक( FT_Face           face,
                     स्थिर FT_String*  glyph_name )
  {
    FT_UInt  परिणाम = 0;


    अगर ( face                       &&
         FT_HAS_GLYPH_NAMES( face ) &&
         glyph_name                 )
    {
      FT_Service_GlyphDict  service;


      FT_FACE_LOOKUP_SERVICE( face,
                              service,
                              GLYPH_DICT );

      अगर ( service && service->name_सूचकांक )
        परिणाम = service->name_सूचकांक( face, glyph_name );
    }

    वापस परिणाम;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Glyph_Name( FT_Face     face,
                     FT_UInt     glyph_सूचकांक,
                     FT_Pointer  बफर,
                     FT_UInt     बफर_max )
  {
    FT_Error              error;
    FT_Service_GlyphDict  service;


    अगर ( !face )
      वापस FT_THROW( Invalid_Face_Handle );

    अगर ( !बफर || बफर_max == 0 )
      वापस FT_THROW( Invalid_Argument );

    /* clean up buffer */
    ((FT_Byte*)बफर)[0] = '\0';

    अगर ( (FT_Long)glyph_सूचकांक >= face->अंक_glyphs )
      वापस FT_THROW( Invalid_Glyph_सूचकांक );

    अगर ( !FT_HAS_GLYPH_NAMES( face ) )
      वापस FT_THROW( Invalid_Argument );

    FT_FACE_LOOKUP_SERVICE( face, service, GLYPH_DICT );
    अगर ( service && service->get_name )
      error = service->get_name( face, glyph_सूचकांक, बफर, बफर_max );
    अन्यथा
      error = FT_THROW( Invalid_Argument );

    वापस error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( स्थिर अक्षर* )
  FT_Get_Postscript_Name( FT_Face  face )
  {
    स्थिर अक्षर*  परिणाम = शून्य;


    अगर ( !face )
      जाओ Exit;

    अगर ( !परिणाम )
    {
      FT_Service_PsFontName  service;


      FT_FACE_LOOKUP_SERVICE( face,
                              service,
                              POSTSCRIPT_FONT_NAME );

      अगर ( service && service->get_ps_font_name )
        परिणाम = service->get_ps_font_name( face );
    }

  Exit:
    वापस परिणाम;
  }


  /* documentation is in tttables.h */

  FT_EXPORT_DEF( व्योम* )
  FT_Get_Sfnt_Table( FT_Face      face,
                     FT_Sfnt_Tag  tag )
  {
    व्योम*                  table = शून्य;
    FT_Service_SFNT_Table  service;


    अगर ( face && FT_IS_SFNT( face ) )
    {
      FT_FACE_FIND_SERVICE( face, service, SFNT_TABLE );
      अगर ( service )
        table = service->get_table( face, tag );
    }

    वापस table;
  }


  /* documentation is in tttables.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Load_Sfnt_Table( FT_Face    face,
                      FT_ULong   tag,
                      FT_Long    offset,
                      FT_Byte*   बफर,
                      FT_ULong*  लंबाई )
  {
    FT_Service_SFNT_Table  service;


    अगर ( !face || !FT_IS_SFNT( face ) )
      वापस FT_THROW( Invalid_Face_Handle );

    FT_FACE_FIND_SERVICE( face, service, SFNT_TABLE );
    अगर ( !service )
      वापस FT_THROW( Unimplemented_Feature );

    वापस service->load_table( face, tag, offset, बफर, लंबाई );
  }


  /* documentation is in tttables.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Sfnt_Table_Info( FT_Face    face,
                      FT_UInt    table_सूचकांक,
                      FT_ULong  *tag,
                      FT_ULong  *लंबाई )
  {
    FT_Service_SFNT_Table  service;
    FT_ULong               offset;


    /* test for valid `length' delayed to `service->table_info' */

    अगर ( !face || !FT_IS_SFNT( face ) )
      वापस FT_THROW( Invalid_Face_Handle );

    FT_FACE_FIND_SERVICE( face, service, SFNT_TABLE );
    अगर ( !service )
      वापस FT_THROW( Unimplemented_Feature );

    वापस service->table_info( face, table_सूचकांक, tag, &offset, लंबाई );
  }


  /* documentation is in tttables.h */

  FT_EXPORT_DEF( FT_ULong )
  FT_Get_CMap_Language_ID( FT_CharMap  charmap )
  {
    FT_Service_TTCMaps  service;
    FT_Face             face;
    TT_CMapInfo         cmap_info;


    अगर ( !charmap || !charmap->face )
      वापस 0;

    face = charmap->face;
    FT_FACE_FIND_SERVICE( face, service, TT_CMAP );
    अगर ( !service )
      वापस 0;
    अगर ( service->get_cmap_info( charmap, &cmap_info ))
      वापस 0;

    वापस cmap_info.language;
  }


  /* documentation is in tttables.h */

  FT_EXPORT_DEF( FT_Long )
  FT_Get_CMap_Format( FT_CharMap  charmap )
  {
    FT_Service_TTCMaps  service;
    FT_Face             face;
    TT_CMapInfo         cmap_info;


    अगर ( !charmap || !charmap->face )
      वापस -1;

    face = charmap->face;
    FT_FACE_FIND_SERVICE( face, service, TT_CMAP );
    अगर ( !service )
      वापस -1;
    अगर ( service->get_cmap_info( charmap, &cmap_info ))
      वापस -1;

    वापस cmap_info.format;
  }


  /* documentation is in ftsizes.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Activate_आकार( FT_आकार  आकार )
  {
    FT_Face  face;


    अगर ( !आकार )
      वापस FT_THROW( Invalid_आकार_Handle );

    face = आकार->face;
    अगर ( !face || !face->driver )
      वापस FT_THROW( Invalid_Face_Handle );

    /* we don't need anything more complex than that; all size objects */
    /* are already listed by the face                                  */
    face->आकार = आकार;

    वापस FT_Err_Ok;
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
                      FT_List_गाँठ*     गाँठ )
  {
    FT_List_गाँठ  cur;
    FT_Renderer  परिणाम = शून्य;


    अगर ( !library )
      जाओ Exit;

    cur = library->renderers.शीर्ष_2;

    अगर ( गाँठ )
    {
      अगर ( *गाँठ )
        cur = (*गाँठ)->अग्र;
      *गाँठ = शून्य;
    }

    जबतक ( cur )
    {
      FT_Renderer  renderer = FT_RENDERER( cur->तथ्य );


      अगर ( renderer->glyph_format == format )
      {
        अगर ( गाँठ )
          *गाँठ = cur;

        परिणाम = renderer;
        अवरोध;
      }
      cur = cur->अग्र;
    }

  Exit:
    वापस परिणाम;
  }


  अटल FT_Renderer
  ft_lookup_glyph_renderer( FT_GlyphSlot  slot )
  {
    FT_Face      face    = slot->face;
    FT_Library   library = FT_FACE_LIBRARY( face );
    FT_Renderer  परिणाम  = library->cur_renderer;


    अगर ( !परिणाम || परिणाम->glyph_format != slot->format )
      परिणाम = FT_Lookup_Renderer( library, slot->format, 0 );

    वापस परिणाम;
  }


  अटल व्योम
  ft_set_current_renderer( FT_Library  library )
  {
    FT_Renderer  renderer;


    renderer = FT_Lookup_Renderer( library, FT_GLYPH_FORMAT_OUTLINE, 0 );
    library->cur_renderer = renderer;
  }


  अटल FT_Error
  ft_add_renderer( FT_Module  module )
  {
    FT_Library   library = module->library;
    FT_Memory    memory  = library->memory;
    FT_Error     error;
    FT_List_गाँठ  गाँठ    = शून्य;


    अगर ( FT_QNEW( गाँठ ) )
      जाओ Exit;

    {
      FT_Renderer         render = FT_RENDERER( module );
      FT_Renderer_Class*  clazz  = (FT_Renderer_Class*)module->clazz;


      render->clazz        = clazz;
      render->glyph_format = clazz->glyph_format;

      /* allocate raster object if needed */
      अगर ( clazz->raster_class && clazz->raster_class->raster_new )
      {
        error = clazz->raster_class->raster_new( memory, &render->raster );
        अगर ( error )
          जाओ Fail;

        render->raster_render = clazz->raster_class->raster_render;
        render->render        = clazz->render_glyph;
      }

#अगर_घोषित FT_CONFIG_OPTION_SVG
      अगर ( clazz->glyph_format == FT_GLYPH_FORMAT_SVG )
        render->render = clazz->render_glyph;
#पूर्ण_अगर

      /* add to list */
      गाँठ->तथ्य = module;
      FT_List_Add( &library->renderers, गाँठ );

      ft_set_current_renderer( library );
    }

  Fail:
    अगर ( error )
      FT_FREE( गाँठ );

  Exit:
    वापस error;
  }


  अटल व्योम
  ft_remove_renderer( FT_Module  module )
  {
    FT_Library   library;
    FT_Memory    memory;
    FT_List_गाँठ  गाँठ;


    library = module->library;
    अगर ( !library )
      वापस;

    memory = library->memory;

    गाँठ = FT_List_Find( &library->renderers, module );
    अगर ( गाँठ )
    {
      FT_Renderer  render = FT_RENDERER( module );


      /* release raster object, if any */
      अगर ( render->raster )
        render->clazz->raster_class->raster_done( render->raster );

      /* remove from list */
      FT_List_Remove( &library->renderers, गाँठ );
      FT_FREE( गाँठ );

      ft_set_current_renderer( library );
    }
  }


  /* documentation is in ftrender.h */

  FT_EXPORT_DEF( FT_Renderer )
  FT_Get_Renderer( FT_Library       library,
                   FT_Glyph_Format  format )
  {
    /* test for valid `library' delayed to `FT_Lookup_Renderer' */

    वापस FT_Lookup_Renderer( library, format, 0 );
  }


  /* documentation is in ftrender.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Set_Renderer( FT_Library     library,
                   FT_Renderer    renderer,
                   FT_UInt        अंक_params,
                   FT_Parameter*  parameters )
  {
    FT_List_गाँठ  गाँठ;
    FT_Error     error = FT_Err_Ok;

    FT_Renderer_SetModeFunc  set_mode;


    अगर ( !library )
    {
      error = FT_THROW( Invalid_Library_Handle );
      जाओ Exit;
    }

    अगर ( !renderer )
    {
      error = FT_THROW( Invalid_Argument );
      जाओ Exit;
    }

    अगर ( अंक_params > 0 && !parameters )
    {
      error = FT_THROW( Invalid_Argument );
      जाओ Exit;
    }

    गाँठ = FT_List_Find( &library->renderers, renderer );
    अगर ( !गाँठ )
    {
      error = FT_THROW( Invalid_Argument );
      जाओ Exit;
    }

    FT_List_Up( &library->renderers, गाँठ );

    अगर ( renderer->glyph_format == FT_GLYPH_FORMAT_OUTLINE )
      library->cur_renderer = renderer;

    set_mode = renderer->clazz->set_mode;

    क्रम ( ; अंक_params > 0; अंक_params-- )
    {
      error = set_mode( renderer, parameters->tag, parameters->तथ्य );
      अगर ( error )
        अवरोध;
      parameters++;
    }

  Exit:
    वापस error;
  }


  FT_BASE_DEF( FT_Error )
  FT_Render_Glyph_Internal( FT_Library      library,
                            FT_GlyphSlot    slot,
                            FT_Render_Mode  render_mode )
  {
    FT_Error     error = FT_Err_Ok;
    FT_Face      face  = slot->face;
    FT_Renderer  renderer;
    FT_List_गाँठ  गाँठ  = शून्य;


    /* try to render colored glyph layers as a special case */
    अगर ( slot->internal->load_flags & FT_LOAD_COLOR &&
         slot->format == FT_GLYPH_FORMAT_OUTLINE    )
    {
      FT_LayerIterator  iterator;

      FT_UInt  base_glyph = slot->glyph_सूचकांक;

      FT_Bool  have_layers;
      FT_UInt  glyph_सूचकांक;
      FT_UInt  color_सूचकांक;


      iterator.p  = शून्य;
      have_layers = FT_Get_Color_Glyph_Layer( face,
                                              base_glyph,
                                              &glyph_सूचकांक,
                                              &color_सूचकांक,
                                              &iterator );
      अगर ( have_layers )
      {
        error = FT_New_GlyphSlot( face, शून्य );
        अगर ( !error )
        {
          TT_Face       ttface = (TT_Face)face;
          SFNT_Service  sfnt   = (SFNT_Service)ttface->sfnt;


          करो
          {
            FT_Int32  load_flags = slot->internal->load_flags;


            /* disable the `FT_LOAD_COLOR' flag to avoid recursion */
            /* right here in this function                         */
            load_flags &= ~FT_LOAD_COLOR;

            /* render into the new `face->glyph' glyph slot */
            load_flags |= FT_LOAD_RENDER | FT_LOAD_NO_BITMAP;

            error = FT_Load_Glyph( face, glyph_सूचकांक, load_flags );
            अगर ( error )
              अवरोध;

            /* blend new `face->glyph' into old `slot'; */
            /* at the first call, `slot' is still empty */
            error = sfnt->colr_blend( ttface,
                                      color_सूचकांक,
                                      slot,
                                      face->glyph );
            अगर ( error )
              अवरोध;

          } जबतक ( FT_Get_Color_Glyph_Layer( face,
                                              base_glyph,
                                              &glyph_सूचकांक,
                                              &color_सूचकांक,
                                              &iterator ) );

          अगर ( !error )
            slot->format = FT_GLYPH_FORMAT_BITMAP;

          /* this call also restores `slot' as the glyph slot */
          FT_Done_GlyphSlot( face->glyph );
        }

        अगर ( !error )
          वापस error;

        /* Failed to do the colored layer.  Draw outline instead. */
        slot->format = FT_GLYPH_FORMAT_OUTLINE;
      }
    }

    /* small shortcut for the very common case */
    अगर ( slot->format == FT_GLYPH_FORMAT_OUTLINE )
    {
      renderer = library->cur_renderer;
      गाँठ     = library->renderers.शीर्ष_2;
    }
    अन्यथा
      renderer = FT_Lookup_Renderer( library, slot->format, &गाँठ );

    error = FT_ERR( Cannot_Render_Glyph );
    जबतक ( renderer )
    {
      error = renderer->render( renderer, slot, render_mode, शून्य );
      अगर ( !error                                   ||
           FT_ERR_NEQ( error, Cannot_Render_Glyph ) )
        अवरोध;

      /* `FT_Err_Cannot_Render_Glyph` is returned if the render mode */
      /* is unsupported by the current renderer for this glyph image */
      /* format.                                                     */

      /* now, look for another renderer that supports the same */
      /* format.                                               */
      renderer = FT_Lookup_Renderer( library, slot->format, &गाँठ );
    }

    /* it is not an error if we cannot render a bitmap glyph */
    अगर ( FT_ERR_EQ( error, Cannot_Render_Glyph ) &&
         slot->format == FT_GLYPH_FORMAT_BITMAP  )
      error = FT_Err_Ok;

#अगर_घोषित FT_DEBUG_LEVEL_TRACE

#अघोषित  FT_COMPONENT
#घोषणा FT_COMPONENT  checksum

    /*
     * Computing the MD5 checksum is expensive, unnecessarily distorting a
     * possible profiling of FreeType if compiled with tracing support.  For
     * this reason, we execute the following code only if explicitly
     * requested.
     */

    /* we use FT_TRACE3 in this block */
    अगर ( !error                               &&
         ft_trace_levels[trace_checksum] >= 3 &&
         slot->bitmap.बफर                  )
    {
      FT_Bitmap  bitmap;
      FT_Error   err;


      FT_Bitmap_Init( &bitmap );

      /* we convert to a single bitmap format for computing the checksum */
      /* this also converts the bitmap flow to `down' (i.e., pitch > 0)  */
      err = FT_Bitmap_Convert( library, &slot->bitmap, &bitmap, 1 );
      अगर ( !err )
      {
        MD5_CTX        ctx;
        अचिन्हित अक्षर  md5[16];
        अचिन्हित दीर्घ  coverage = 0;
        पूर्णांक            i, j;
        पूर्णांक            rows  = (पूर्णांक)bitmap.rows;
        पूर्णांक            pitch = bitmap.pitch;


        FT_TRACE3(( "FT_Render_Glyph: bitmap %dx%d, %s (mode %d)\n",
                    pitch,
                    rows,
                    pixel_modes[slot->bitmap.pixel_mode],
                    slot->bitmap.pixel_mode ));

        क्रम ( i = 0; i < rows; i++ )
          क्रम ( j = 0; j < pitch; j++ )
            coverage += bitmap.बफर[i * pitch + j];

        FT_TRACE3(( "  Total coverage: %lu\n", coverage ));

        MD5_Init( &ctx );
        अगर ( bitmap.बफर )
          MD5_Update( &ctx, bitmap.बफर,
                      (अचिन्हित दीर्घ)rows * (अचिन्हित दीर्घ)pitch );
        MD5_Final( md5, &ctx );

        FT_TRACE3(( "  MD5 checksum: " ));
        क्रम ( i = 0; i < 16; i++ )
          FT_TRACE3(( "%02X", md5[i] ));
        FT_TRACE3(( "\n" ));
      }

      FT_Bitmap_Done( library, &bitmap );
    }

    /*
     * Dump bitmap in Netpbm format (PBM or PGM).
     */

    /* we use FT_TRACE7 in this block */
    अगर ( !error                               &&
         ft_trace_levels[trace_checksum] >= 7 &&
         slot->bitmap.बफर                  )
    {
      अगर ( slot->bitmap.rows  < 128U &&
           slot->bitmap.width < 128U )
      {
        पूर्णांक  rows  = (पूर्णांक)slot->bitmap.rows;
        पूर्णांक  width = (पूर्णांक)slot->bitmap.width;
        पूर्णांक  pitch =      slot->bitmap.pitch;
        पूर्णांक  i, j, m;

        अचिन्हित अक्षर*  topleft = slot->bitmap.बफर;


        अगर ( pitch < 0 )
          topleft -= pitch * ( rows - 1 );

        FT_TRACE7(( "Netpbm image: start\n" ));
        चयन ( slot->bitmap.pixel_mode )
        {
        हाल FT_PIXEL_MODE_MONO:
          FT_TRACE7(( "P1 %d %d\n", width, rows ));
          क्रम ( i = 0; i < rows; i++ )
          {
            क्रम ( j = 0; j < width; )
              क्रम ( m = 128; m > 0 && j < width; m >>= 1, j++ )
                FT_TRACE7(( " %d",
                            ( topleft[i * pitch + j / 8] & m ) != 0 ));
            FT_TRACE7(( "\n" ));
          }
          अवरोध;

        शेष:
          FT_TRACE7(( "P2 %d %d 255\n", width, rows ));
          क्रम ( i = 0; i < rows; i++ )
          {
            क्रम ( j = 0; j < width; j += 1 )
              FT_TRACE7(( " %3u", topleft[i * pitch + j] ));
            FT_TRACE7(( "\n" ));
          }
        }
        FT_TRACE7(( "Netpbm image: end\n" ));
      }
      अन्यथा
        FT_TRACE7(( "Netpbm image: too large, omitted\n" ));
    }

#अघोषित  FT_COMPONENT
#घोषणा FT_COMPONENT  objs

#पूर्ण_अगर /* FT_DEBUG_LEVEL_TRACE */

    वापस error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Render_Glyph( FT_GlyphSlot    slot,
                   FT_Render_Mode  render_mode )
  {
    FT_Library  library;


    अगर ( !slot || !slot->face )
      वापस FT_THROW( Invalid_Argument );

    library = FT_FACE_LIBRARY( slot->face );

    वापस FT_Render_Glyph_Internal( library, slot, render_mode );
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
  अटल व्योम
  Destroy_Module( FT_Module  module )
  {
    स्थिर FT_Module_Class*  clazz   = module->clazz;
    FT_Library              library = module->library;
    FT_Memory               memory  = module->memory;


    अगर ( library && library->auto_hinter == module )
      library->auto_hinter = शून्य;

    /* if the module is a renderer */
    अगर ( FT_MODULE_IS_RENDERER( module ) )
      ft_remove_renderer( module );

    /* if the module is a font driver, add some steps */
    अगर ( FT_MODULE_IS_DRIVER( module ) )
      Destroy_Driver( FT_DRIVER( module ) );

    /* finalize the module object */
    अगर ( clazz->module_done )
      clazz->module_done( module );

    /* discard it */
    FT_FREE( module );
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Add_Module( FT_Library              library,
                 स्थिर FT_Module_Class*  clazz )
  {
    FT_Error   error;
    FT_Memory  memory;
    FT_Module  module = शून्य;
    FT_UInt    nn;


#घोषणा FREETYPE_VER_FIXED  ( ( (FT_Long)FREETYPE_MAJOR << 16 ) | \
                                FREETYPE_MINOR                  )

    अगर ( !library )
      वापस FT_THROW( Invalid_Library_Handle );

    अगर ( !clazz )
      वापस FT_THROW( Invalid_Argument );

    /* check FreeType version */
    अगर ( clazz->module_requires > FREETYPE_VER_FIXED )
      वापस FT_THROW( Invalid_Version );

    /* look for a module with the same name in the library's table */
    क्रम ( nn = 0; nn < library->अंक_modules; nn++ )
    {
      module = library->modules[nn];
      अगर ( ft_strcmp( module->clazz->module_name, clazz->module_name ) == 0 )
      {
        /* this installed module has the same name, compare their versions */
        अगर ( clazz->module_version <= module->clazz->module_version )
          वापस FT_THROW( Lower_Module_Version );

        /* remove the module from our list, then exit the loop to replace */
        /* it by our new version..                                        */
        FT_Remove_Module( library, module );
        अवरोध;
      }
    }

    memory = library->memory;
    error  = FT_Err_Ok;

    अगर ( library->अंक_modules >= FT_MAX_MODULES )
    {
      error = FT_THROW( Too_Many_Drivers );
      जाओ Exit;
    }

    /* allocate module object */
    अगर ( FT_ALLOC( module, clazz->module_आकार ) )
      जाओ Exit;

    /* base initialization */
    module->clazz   = clazz;
    module->library = library;
    module->memory  = memory;

    /* check whether the module is a renderer - this must be performed */
    /* before the normal module initialization                         */
    अगर ( FT_MODULE_IS_RENDERER( module ) )
    {
      /* add to the renderers list */
      error = ft_add_renderer( module );
      अगर ( error )
        जाओ Fail;
    }

    /* is the module a auto-hinter? */
    अगर ( FT_MODULE_IS_HINTER( module ) )
      library->auto_hinter = module;

    /* if the module is a font driver */
    अगर ( FT_MODULE_IS_DRIVER( module ) )
    {
      FT_Driver  driver = FT_DRIVER( module );


      driver->clazz = (FT_Driver_Class)module->clazz;
    }

    अगर ( clazz->module_init )
    {
      error = clazz->module_init( module );
      अगर ( error )
        जाओ Fail;
    }

    /* add module to the library's table */
    library->modules[library->अंक_modules++] = module;

  Exit:
    वापस error;

  Fail:
    अगर ( FT_MODULE_IS_RENDERER( module ) )
    {
      FT_Renderer  renderer = FT_RENDERER( module );


      अगर ( renderer->clazz                                          &&
           renderer->clazz->glyph_format == FT_GLYPH_FORMAT_OUTLINE &&
           renderer->raster                                         )
        renderer->clazz->raster_class->raster_done( renderer->raster );
    }

    FT_FREE( module );
    जाओ Exit;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Module )
  FT_Get_Module( FT_Library   library,
                 स्थिर अक्षर*  module_name )
  {
    FT_Module   परिणाम = शून्य;
    FT_Module*  cur;
    FT_Module*  limit;


    अगर ( !library || !module_name )
      वापस परिणाम;

    cur   = library->modules;
    limit = cur + library->अंक_modules;

    क्रम ( ; cur < limit; cur++ )
      अगर ( ft_strcmp( cur[0]->clazz->module_name, module_name ) == 0 )
      {
        परिणाम = cur[0];
        अवरोध;
      }

    वापस परिणाम;
  }


  /* documentation is in ftobjs.h */

  FT_BASE_DEF( स्थिर व्योम* )
  FT_Get_Module_Interface( FT_Library   library,
                           स्थिर अक्षर*  mod_name )
  {
    FT_Module  module;


    /* test for valid `library' delayed to FT_Get_Module() */

    module = FT_Get_Module( library, mod_name );

    वापस module ? module->clazz->module_interface : 0;
  }


  FT_BASE_DEF( FT_Pointer )
  ft_module_get_service( FT_Module    module,
                         स्थिर अक्षर*  service_id,
                         FT_Bool      global )
  {
    FT_Pointer  परिणाम = शून्य;


    अगर ( module )
    {
      FT_ASSERT( module->clazz && module->clazz->get_interface );

      /* first, look for the service in the module */
      अगर ( module->clazz->get_interface )
        परिणाम = module->clazz->get_interface( module, service_id );

      अगर ( global && !परिणाम )
      {
        /* we didn't find it, look in all other modules then */
        FT_Library  library = module->library;
        FT_Module*  cur     = library->modules;
        FT_Module*  limit   = cur + library->अंक_modules;


        क्रम ( ; cur < limit; cur++ )
        {
          अगर ( cur[0] != module )
          {
            FT_ASSERT( cur[0]->clazz );

            अगर ( cur[0]->clazz->get_interface )
            {
              परिणाम = cur[0]->clazz->get_interface( cur[0], service_id );
              अगर ( परिणाम )
                अवरोध;
            }
          }
        }
      }
    }

    वापस परिणाम;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Remove_Module( FT_Library  library,
                    FT_Module   module )
  {
    /* try to find the module from the table, then remove it from there */

    अगर ( !library )
      वापस FT_THROW( Invalid_Library_Handle );

    अगर ( module )
    {
      FT_Module*  cur   = library->modules;
      FT_Module*  limit = cur + library->अंक_modules;


      क्रम ( ; cur < limit; cur++ )
      {
        अगर ( cur[0] == module )
        {
          /* remove it from the table */
          library->अंक_modules--;
          limit--;
          जबतक ( cur < limit )
          {
            cur[0] = cur[1];
            cur++;
          }
          limit[0] = शून्य;

          /* destroy the module */
          Destroy_Module( module );

          वापस FT_Err_Ok;
        }
      }
    }
    वापस FT_THROW( Invalid_Driver_Handle );
  }


  अटल FT_Error
  ft_property_do( FT_Library        library,
                  स्थिर FT_String*  module_name,
                  स्थिर FT_String*  property_name,
                  व्योम*             मूल्य,
                  FT_Bool           set,
                  FT_Bool           मूल्य_is_string )
  {
    FT_Module*           cur;
    FT_Module*           limit;
    FT_Module_Interface  interface;

    FT_Service_Properties  service;

#अगर_घोषित FT_DEBUG_LEVEL_ERROR
    स्थिर FT_String*  set_name  = "FT_Property_Set";
    स्थिर FT_String*  get_name  = "FT_Property_Get";
    स्थिर FT_String*  func_name = set ? set_name : get_name;
#पूर्ण_अगर

    FT_Bool  missing_func;


    अगर ( !library )
      वापस FT_THROW( Invalid_Library_Handle );

    अगर ( !module_name || !property_name || !मूल्य )
      वापस FT_THROW( Invalid_Argument );

    cur   = library->modules;
    limit = cur + library->अंक_modules;

    /* search module */
    क्रम ( ; cur < limit; cur++ )
      अगर ( !ft_strcmp( cur[0]->clazz->module_name, module_name ) )
        अवरोध;

    अगर ( cur == limit )
    {
      FT_TRACE2(( "%s: can't find module `%s'\n",
                  func_name, module_name ));
      वापस FT_THROW( Missing_Module );
    }

    /* check whether we have a service interface */
    अगर ( !cur[0]->clazz->get_interface )
    {
      FT_TRACE2(( "%s: module `%s' doesn't support properties\n",
                  func_name, module_name ));
      वापस FT_THROW( Unimplemented_Feature );
    }

    /* search property service */
    interface = cur[0]->clazz->get_interface( cur[0],
                                              FT_SERVICE_ID_PROPERTIES );
    अगर ( !interface )
    {
      FT_TRACE2(( "%s: module `%s' doesn't support properties\n",
                  func_name, module_name ));
      वापस FT_THROW( Unimplemented_Feature );
    }

    service = (FT_Service_Properties)interface;

    अगर ( set )
      missing_func = FT_BOOL( !service->set_property );
    अन्यथा
      missing_func = FT_BOOL( !service->get_property );

    अगर ( missing_func )
    {
      FT_TRACE2(( "%s: property service of module `%s' is broken\n",
                  func_name, module_name ));
      वापस FT_THROW( Unimplemented_Feature );
    }

    वापस set ? service->set_property( cur[0],
                                        property_name,
                                        मूल्य,
                                        मूल्य_is_string )
               : service->get_property( cur[0],
                                        property_name,
                                        मूल्य );
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Property_Set( FT_Library        library,
                   स्थिर FT_String*  module_name,
                   स्थिर FT_String*  property_name,
                   स्थिर व्योम*       मूल्य )
  {
    वापस ft_property_do( library,
                           module_name,
                           property_name,
                           (व्योम*)मूल्य,
                           TRUE,
                           FALSE );
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Property_Get( FT_Library        library,
                   स्थिर FT_String*  module_name,
                   स्थिर FT_String*  property_name,
                   व्योम*             मूल्य )
  {
    वापस ft_property_do( library,
                           module_name,
                           property_name,
                           मूल्य,
                           FALSE,
                           FALSE );
  }


#अगर_घोषित FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES

  /* this variant is used for handling the FREETYPE_PROPERTIES */
  /* environment variable                                      */

  FT_BASE_DEF( FT_Error )
  ft_property_string_set( FT_Library        library,
                          स्थिर FT_String*  module_name,
                          स्थिर FT_String*  property_name,
                          FT_String*        मूल्य )
  {
    वापस ft_property_do( library,
                           module_name,
                           property_name,
                           (व्योम*)मूल्य,
                           TRUE,
                           TRUE );
  }

#पूर्ण_अगर


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
    अगर ( !library )
      वापस FT_THROW( Invalid_Library_Handle );

    library->refcount++;

    वापस FT_Err_Ok;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_New_Library( FT_Memory    memory,
                  FT_Library  *alibrary )
  {
    FT_Library  library = शून्य;
    FT_Error    error;


    अगर ( !memory || !alibrary )
      वापस FT_THROW( Invalid_Argument );

#अगर_अघोषित FT_DEBUG_LOGGING
#अगर_घोषित FT_DEBUG_LEVEL_ERROR
    /* init debugging support */
    ft_debug_init();
#पूर्ण_अगर /* FT_DEBUG_LEVEL_ERROR */
#पूर्ण_अगर /* !FT_DEBUG_LOGGING */

    /* first of all, allocate the library object */
    अगर ( FT_NEW( library ) )
      वापस error;

    library->memory = memory;

    library->version_major = FREETYPE_MAJOR;
    library->version_minor = FREETYPE_MINOR;
    library->version_patch = FREETYPE_PATCH;

    library->refcount = 1;

    /* That's ok now */
    *alibrary = library;

    वापस FT_Err_Ok;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( व्योम )
  FT_Library_Version( FT_Library   library,
                      FT_Int      *amajor,
                      FT_Int      *aminor,
                      FT_Int      *apatch )
  {
    FT_Int  major = 0;
    FT_Int  minor = 0;
    FT_Int  patch = 0;


    अगर ( library )
    {
      major = library->version_major;
      minor = library->version_minor;
      patch = library->version_patch;
    }

    अगर ( amajor )
      *amajor = major;

    अगर ( aminor )
      *aminor = minor;

    अगर ( apatch )
      *apatch = patch;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Done_Library( FT_Library  library )
  {
    FT_Memory  memory;


    अगर ( !library )
      वापस FT_THROW( Invalid_Library_Handle );

    library->refcount--;
    अगर ( library->refcount > 0 )
      जाओ Exit;

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
      स्थिर अक्षर*  driver_name[] = { "type42", शून्य };


      क्रम ( m = 0;
            m < माप ( driver_name ) / माप ( driver_name[0] );
            m++ )
      {
        क्रम ( n = 0; n < library->अंक_modules; n++ )
        {
          FT_Module    module      = library->modules[n];
          स्थिर अक्षर*  module_name = module->clazz->module_name;
          FT_List      faces;


          अगर ( driver_name[m]                                &&
               ft_strcmp( module_name, driver_name[m] ) != 0 )
            जारी;

          अगर ( ( module->clazz->module_flags & FT_MODULE_FONT_DRIVER ) == 0 )
            जारी;

          FT_TRACE7(( "FT_Done_Library: close faces for %s\n", module_name ));

          faces = &FT_DRIVER( module )->faces_list;
          जबतक ( faces->शीर्ष_2 )
          {
            FT_Done_Face( FT_FACE( faces->शीर्ष_2->तथ्य ) );
            अगर ( faces->शीर्ष_2 )
              FT_TRACE0(( "FT_Done_Library: failed to free some faces\n" ));
          }
        }
      }
    }

    /* Close all other modules in the library */
#अगर 1
    /* XXX Modules are removed in the reversed order so that  */
    /* type42 module is removed before truetype module.  This */
    /* avoids double free in some occasions.  It is a hack.   */
    जबतक ( library->अंक_modules > 0 )
      FT_Remove_Module( library,
                        library->modules[library->अंक_modules - 1] );
#अन्यथा
    {
      FT_UInt  n;


      क्रम ( n = 0; n < library->अंक_modules; n++ )
      {
        FT_Module  module = library->modules[n];


        अगर ( module )
        {
          Destroy_Module( module );
          library->modules[n] = शून्य;
        }
      }
    }
#पूर्ण_अगर

    FT_FREE( library );

  Exit:
    वापस FT_Err_Ok;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( व्योम )
  FT_Set_Debug_Hook( FT_Library         library,
                     FT_UInt            hook_सूचकांक,
                     FT_DebugHook_Func  debug_hook )
  {
    अगर ( library && debug_hook &&
         hook_सूचकांक <
           ( माप ( library->debug_hooks ) / माप ( व्योम* ) ) )
      library->debug_hooks[hook_सूचकांक] = debug_hook;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_TrueTypeEngineType )
  FT_Get_TrueType_Engine_Type( FT_Library  library )
  {
    FT_TrueTypeEngineType  परिणाम = FT_TRUETYPE_ENGINE_TYPE_NONE;


    अगर ( library )
    {
      FT_Module  module = FT_Get_Module( library, "truetype" );


      अगर ( module )
      {
        FT_Service_TrueTypeEngine  service;


        service = (FT_Service_TrueTypeEngine)
                    ft_module_get_service( module,
                                           FT_SERVICE_ID_TRUETYPE_ENGINE,
                                           0 );
        अगर ( service )
          परिणाम = service->engine_type;
      }
    }

    वापस परिणाम;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_SubGlyph_Info( FT_GlyphSlot  glyph,
                        FT_UInt       sub_सूचकांक,
                        FT_Int       *p_सूचकांक,
                        FT_UInt      *p_flags,
                        FT_Int       *p_arg1,
                        FT_Int       *p_arg2,
                        FT_Matrix    *p_transform )
  {
    FT_Error  error = FT_ERR( Invalid_Argument );


    अगर ( glyph                                      &&
         glyph->subglyphs                           &&
         glyph->format == FT_GLYPH_FORMAT_COMPOSITE &&
         sub_सूचकांक < glyph->अंक_subglyphs           )
    {
      FT_SubGlyph  subg = glyph->subglyphs + sub_सूचकांक;


      *p_सूचकांक     = subg->index;
      *p_flags     = subg->flags;
      *p_arg1      = subg->arg1;
      *p_arg2      = subg->arg2;
      *p_transform = subg->transform;

      error = FT_Err_Ok;
    }

    वापस error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Get_Color_Glyph_Layer( FT_Face            face,
                            FT_UInt            base_glyph,
                            FT_UInt           *aglyph_सूचकांक,
                            FT_UInt           *acolor_सूचकांक,
                            FT_LayerIterator*  iterator )
  {
    TT_Face       ttface;
    SFNT_Service  sfnt;


    अगर ( !face                                   ||
         !aglyph_सूचकांक                           ||
         !acolor_सूचकांक                           ||
         !iterator                               ||
         base_glyph >= (FT_UInt)face->अंक_glyphs )
      वापस 0;

    अगर ( !FT_IS_SFNT( face ) )
      वापस 0;

    ttface = (TT_Face)face;
    sfnt   = (SFNT_Service)ttface->sfnt;

    अगर ( sfnt->get_colr_layer )
      वापस sfnt->get_colr_layer( ttface,
                                   base_glyph,
                                   aglyph_सूचकांक,
                                   acolor_सूचकांक,
                                   iterator );
    अन्यथा
      वापस 0;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Get_Color_Glyph_Paint( FT_Face                  face,
                            FT_UInt                  base_glyph,
                            FT_Color_मूल_Transform  मूल_transform,
                            FT_OpaquePaint*          paint )
  {
    TT_Face       ttface;
    SFNT_Service  sfnt;


    अगर ( !face || !paint )
      वापस 0;

    अगर ( !FT_IS_SFNT( face ) )
      वापस 0;

    ttface = (TT_Face)face;
    sfnt   = (SFNT_Service)ttface->sfnt;

    अगर ( sfnt->get_colr_glyph_paint )
      वापस sfnt->get_colr_glyph_paint( ttface,
                                         base_glyph,
                                         मूल_transform,
                                         paint );
    अन्यथा
      वापस 0;
  }


  /* documentation is in ftcolor.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Get_Color_Glyph_ClipBox( FT_Face      face,
                              FT_UInt      base_glyph,
                              FT_ClipBox*  clip_box )
  {
    TT_Face       ttface;
    SFNT_Service  sfnt;


    अगर ( !face || !clip_box )
      वापस 0;

    अगर ( !FT_IS_SFNT( face ) )
      वापस 0;

    ttface = (TT_Face)face;
    sfnt   = (SFNT_Service)ttface->sfnt;

    अगर ( sfnt->get_color_glyph_clipbox )
      वापस sfnt->get_color_glyph_clipbox( ttface,
                                            base_glyph,
                                            clip_box );
    अन्यथा
      वापस 0;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Get_Paint_Layers( FT_Face            face,
                       FT_LayerIterator*  layer_iterator,
                       FT_OpaquePaint*    paint )
  {
    TT_Face       ttface;
    SFNT_Service  sfnt;


    अगर ( !face || !paint || !layer_iterator )
      वापस 0;

    अगर ( !FT_IS_SFNT( face ) )
      वापस 0;

    ttface = (TT_Face)face;
    sfnt   = (SFNT_Service)ttface->sfnt;

    अगर ( sfnt->get_paint_layers )
      वापस sfnt->get_paint_layers( ttface, layer_iterator, paint );
    अन्यथा
      वापस 0;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Get_Paint( FT_Face face,
                FT_OpaquePaint  opaque_paint,
                FT_COLR_Paint*  paint )
  {
    TT_Face       ttface;
    SFNT_Service  sfnt;


    अगर ( !face || !paint )
      वापस 0;

    अगर ( !FT_IS_SFNT( face ) )
      वापस 0;

    ttface = (TT_Face)face;
    sfnt   = (SFNT_Service)ttface->sfnt;

    अगर ( sfnt->get_paint )
      वापस sfnt->get_paint( ttface, opaque_paint, paint );
    अन्यथा
      वापस 0;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Get_Colorline_Stops ( FT_Face                face,
                           FT_ColorStop *         color_stop,
                           FT_ColorStopIterator  *iterator )
  {
    TT_Face       ttface;
    SFNT_Service  sfnt;


    अगर ( !face || !color_stop || !iterator )
      वापस 0;

    अगर ( !FT_IS_SFNT( face ) )
      वापस 0;

    ttface = (TT_Face)face;
    sfnt   = (SFNT_Service)ttface->sfnt;

    अगर ( sfnt->get_colorline_stops )
      वापस sfnt->get_colorline_stops ( ttface, color_stop, iterator );
    अन्यथा
      वापस 0;
  }


/* END */
