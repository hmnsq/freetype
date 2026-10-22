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


#अन्तर्भाव <freetype/ftlist.h>
#अन्तर्भाव <freetype/ftoutln.h>
#अन्तर्भाव <freetype/ftfntfmt.h>
#अन्तर्भाव <freetype/otsvg.h>

#अन्तर्भाव <freetype/internal/ftvalid.h>
#अन्तर्भाव <freetype/internal/ftobjs.h>
#अन्तर्भाव <freetype/internal/ftdebug.h>
#अन्तर्भाव <freetype/internal/ftrfork.h>
#अन्तर्भाव <freetype/internal/ftstream.h>
#अन्तर्भाव <freetype/internal/sfnt.h>          /* for SFNT_Load_Table_Func */
#अन्तर्भाव <freetype/internal/psaux.h>         /* for PS_Driver            */
#अन्तर्भाव <freetype/internal/svginterface.h>

#अन्तर्भाव <freetype/tttables.h>
#अन्तर्भाव <freetype/tttags.h>
#अन्तर्भाव <freetype/ttnameid.h>

#अन्तर्भाव <freetype/internal/services/svprop.h>
#अन्तर्भाव <freetype/internal/services/svsfnt.h>
#अन्तर्भाव <freetype/internal/services/svpostnm.h>
#अन्तर्भाव <freetype/internal/services/svgldict.h>
#अन्तर्भाव <freetype/internal/services/svttcmap.h>
#अन्तर्भाव <freetype/internal/services/svkern.h>
#अन्तर्भाव <freetype/internal/services/svtteng.h>

#यदि_परिभाषितम् TT_CONFIG_OPTION_VARC
#अन्तर्भाव <freetype/internal/services/svvarc.h>
#यदि_अन्तः

#अन्तर्भाव <freetype/ftdriver.h>

#यदि_परिभाषितम् FT_CONFIG_OPTION_MAC_FONTS
#अन्तर्भाव "ftbase.h"
#यदि_अन्तः


#यदि_परिभाषितम् FT_DEBUG_LEVEL_TRACE

#अन्तर्भाव <freetype/ftbitmap.h>

#यदि defined( _MSC_VER )      /* Visual C++ (and Intel C++)   */
  /* We disable the warning `conversion from XXX to YYY,     */
  /* possible loss of data' in order to compile cleanly with */
  /* the maximum level of warnings: `md5.c' is non-FreeType  */
  /* code, and it gets used during development builds only.  */
#निर्देशः warning( प्रक्षिप )
#निर्देशः warning( disable : 4244 )
#यदि_अन्तः /* _MSC_VER */

  /* It's easiest to include `md5.c' directly.  However, since OpenSSL */
  /* also provides the same functions, there might be conflicts if     */
  /* both FreeType and OpenSSL are built as static libraries.  For     */
  /* this reason, we put the MD5 stuff into the `FT_' namespace.       */
#परिभाषय MD5_u32plus  FT_MD5_u32plus
#परिभाषय MD5_CTX      FT_MD5_CTX
#परिभाषय MD5_Init     FT_MD5_Init
#परिभाषय MD5_Update   FT_MD5_Update
#परिभाषय MD5_Final    FT_MD5_Final

#अपरिभाषय  HAVE_OPENSSL

#अन्तर्भाव "md5.c"

#यदि defined( _MSC_VER )
#निर्देशः warning( निष्कासय )
#यदि_अन्तः

  /* This array must stay in sync with the @FT_Pixel_Mode enumeration */
  /* (in file `ftimage.h`).                                           */

  स्थिर ध्रुव वर्ण* ध्रुव  pixel_modes[] =
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

#यदि_अन्तः /* FT_DEBUG_LEVEL_TRACE */


#परिभाषय GRID_FIT_METRICS


  /* forward declaration */
  स्थिर FT_Error
  ft_open_face_आन्तरिकम्( FT_Library           library,
                         ध्रुव FT_Open_तर्काः*  तर्काः,
                         FT_Long              face_सूचकांक,
                         FT_Face             *aface,
                         FT_Bool              test_mac_fonts );


  FT_आधारः_DEF( FT_Pointer )
  ft_service_list_lookup( FT_ServiceDesc  service_descriptors,
                          ध्रुव वर्ण*     service_id )
  {
    FT_Pointer      परिणाम = रिक्त;
    FT_ServiceDesc  desc   = service_descriptors;


    यदि ( desc && service_id )
    {
      प्रति ( ; desc->serv_id != रिक्त; desc++ )
      {
        यदि ( ft_strcmp( desc->serv_id, service_id ) == 0 )
        {
          परिणाम = (FT_Pointer)desc->serv_दत्त;
          भङ्ग;
        }
      }
    }

    प्रत्यागम परिणाम;
  }


  FT_आधारः_DEF( शून्य )
  ft_validator_init( FT_Validator        valid,
                     ध्रुव FT_Byte*      आधारः,
                     ध्रुव FT_Byte*      सीमा,
                     FT_ValidationLevel  level )
  {
    valid->आधारः  = आधारः;
    valid->सीमा = सीमा;
    valid->level = level;
    valid->error = FT_Err_Ok;
  }


  FT_आधारः_DEF( FT_Int )
  ft_validator_सततम्( FT_Validator  valid )
  {
    /* This function doesn't work!  None should call it. */
    FT_अप्रयुक्तम्( valid );

    प्रत्यागम -1;
  }


  FT_आधारः_DEF( शून्य )
  ft_validator_error( FT_Validator  valid,
                      FT_Error      error )
  {
    /* since the cast below also disables the compiler's */
    /* type check, we introduce a dummy variable, which  */
    /* will be optimized away                            */
    चञ्चल ft_jmp_buf* jump_बफर = &valid->jump_बफर;


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
  FT_आधारः_DEF( FT_Error )
  FT_प्रवाहः_New( FT_Library           library,
                 ध्रुव FT_Open_तर्काः*  तर्काः,
                 FT_प्रवाहः           *astream )
  {
    FT_Error   error;
    FT_स्मृतिः  स्मृतिः;
    FT_प्रवाहः  प्रवाहः = रिक्त;
    FT_UInt    बहुलकम्;


    *astream = रिक्त;

    यदि ( !library )
      प्रत्यागम FT_THROW( Invalid_Library_Handle );

    यदि ( !तर्काः )
      प्रत्यागम FT_THROW( Invalid_तर्कः );

    स्मृतिः = library->स्मृतिः;
    बहुलकम्   = तर्काः->flags &
               ( FT_OPEN_स्मृतिः | FT_OPEN_प्रवाहः | FT_OPEN_PATHNAME );

    यदि ( बहुलकम् == FT_OPEN_स्मृतिः )
    {
      /* create a memory-based stream */
      यदि ( FT_NEW( प्रवाहः ) )
        गच्छ Exit;

      FT_प्रवाहः_Open_स्मृतिः( प्रवाहः,
                            (ध्रुव FT_Byte*)तर्काः->स्मृतिः_आधारः,
                            (FT_ULong)तर्काः->स्मृतिः_आकार );
      प्रवाहः->स्मृतिः = स्मृतिः;
    }

#यदि_अपरिभाषितम् FT_CONFIG_OPTION_DISABLE_प्रवाहः_SUPPORT

    अन्यथा यदि ( बहुलकम् == FT_OPEN_PATHNAME )
    {
      /* create a normal system stream */
      यदि ( FT_NEW( प्रवाहः ) )
        गच्छ Exit;

      प्रवाहः->स्मृतिः = स्मृतिः;
      error = FT_प्रवाहः_Open( प्रवाहः, तर्काः->pathname );
      यदि ( error )
        FT_FREE( प्रवाहः );
    }
    अन्यथा यदि ( ( बहुलकम् == FT_OPEN_प्रवाहः ) && तर्काः->प्रवाहः )
    {
      /* use an existing, user-provided stream */

      /* in this case, we do not need to allocate a new stream object */
      /* since the caller is responsible for closing it himself       */
      प्रवाहः         = तर्काः->प्रवाहः;
      प्रवाहः->स्मृतिः = स्मृतिः;
      error          = FT_Err_Ok;
    }

#यदि_अन्तः

    अन्यथा
    {
      error = FT_THROW( Invalid_तर्कः );
      यदि ( ( तर्काः->flags & FT_OPEN_प्रवाहः ) && तर्काः->प्रवाहः )
        FT_प्रवाहः_Close( तर्काः->प्रवाहः );
    }

    यदि ( !error )
      *astream       = प्रवाहः;

  Exit:
    प्रत्यागम error;
  }


  FT_आधारः_DEF( शून्य )
  FT_प्रवाहः_Free( FT_प्रवाहः  प्रवाहः,
                  FT_Int     external )
  {
    यदि ( प्रवाहः )
    {
      FT_स्मृतिः  स्मृतिः = प्रवाहः->स्मृतिः;


      FT_प्रवाहः_Close( प्रवाहः );

      यदि ( !external )
        FT_FREE( प्रवाहः );
    }
  }


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#अपरिभाषय  FT_COMPONENT
#परिभाषय FT_COMPONENT  objs


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


  स्थिर FT_Error
  ft_glyphslot_init( FT_GlyphSlot  slot )
  {
    FT_Driver         driver   = slot->face->driver;
    FT_Driver_Class   clazz    = driver->clazz;
    FT_स्मृतिः         स्मृतिः   = driver->मूलम्.स्मृतिः;
    FT_Error          error    = FT_Err_Ok;
    FT_Slot_आन्तरिकम्  आन्तरिकम् = रिक्त;


    slot->library = driver->मूलम्.library;

    यदि ( FT_NEW( आन्तरिकम् ) )
      गच्छ Exit;

    slot->आन्तरिकम् = आन्तरिकम्;

    यदि ( FT_DRIVER_उपयुङ्क्ते_OUTLINES( driver ) )
      error = FT_GlyphLoader_New( स्मृतिः, &आन्तरिकम्->loader );

    यदि ( !error && clazz->init_slot )
      error = clazz->init_slot( slot );

#यदि_परिभाषितम् FT_CONFIG_OPTION_SVG
    /* if SVG table exists, allocate the space in `slot->other` */
    यदि ( slot->face->face_flags & FT_FACE_FLAG_SVG )
    {
      FT_SVG_Document  document = रिक्त;


      यदि ( FT_NEW( document ) )
        गच्छ Exit;
      slot->other = document;
    }
#यदि_अन्तः

  Exit:
    प्रत्यागम error;
  }


  FT_आधारः_DEF( शून्य )
  ft_glyphslot_free_bitmap( FT_GlyphSlot  slot )
  {
    यदि ( slot->आन्तरिकम् && ( slot->आन्तरिकम्->flags & FT_GLYPH_OWN_BITMAP ) )
    {
      FT_स्मृतिः  स्मृतिः = FT_FACE_स्मृतिः( slot->face );


      FT_FREE( slot->bitmap.बफर );
      slot->आन्तरिकम्->flags &= ~FT_GLYPH_OWN_BITMAP;
    }
    अन्यथा
    {
      /* assume that the bitmap buffer was stolen or not */
      /* allocated from the heap                         */
      slot->bitmap.बफर = रिक्त;
    }
  }


  /* overflow-resistant presetting of bitmap position and dimensions; */
  /* also check whether the size is too large for rendering           */
  FT_आधारः_DEF( FT_Bool )
  ft_glyphslot_preset_bitmap( FT_GlyphSlot      slot,
                              FT_Render_बहुलकम्    बहुलकम्,
                              ध्रुव FT_सदिशम्*  origin )
  {
    FT_Outline*  outline = &slot->outline;
    FT_Bitmap*   bitmap  = &slot->bitmap;

    FT_Pixel_बहुलकम्  pixel_बहुलकम्;

    FT_BBox  cbox, pbox;
    FT_Pos   x_सरणम् = 0;
    FT_Pos   y_सरणम् = 0;
    FT_Pos   x_वामः, y_शिखरम्;
    FT_Pos   विस्तारः, height, pitch;
    FT_Bool  ret;


    यदि ( slot->format == FT_GLYPH_FORMAT_SVG )
    {
      FT_Module    module;
      SVG_Service  svg_service;


      module      = FT_Get_Module( slot->library, "ot-svg" );
      svg_service = (SVG_Service)module->clazz->module_interface;

      प्रत्यागम (FT_Bool)svg_service->preset_slot( module, slot, FALSE );
    }
    अन्यथा यदि ( slot->format != FT_GLYPH_FORMAT_OUTLINE )
      प्रत्यागम 1;

    यदि ( origin )
    {
      x_सरणम् = origin->x;
      y_सरणम् = origin->y;
    }

    /* compute the control box, and grid-fit it, */
    /* taking into account the origin shift      */
    FT_Outline_Get_CBox( outline, &cbox );

    /* rough estimate of pixel box */
    pbox.xMin = ( cbox.xMin >> 6 ) + ( x_सरणम् >> 6 );
    pbox.yMin = ( cbox.yMin >> 6 ) + ( y_सरणम् >> 6 );
    pbox.xMax = ( cbox.xMax >> 6 ) + ( x_सरणम् >> 6 );
    pbox.yMax = ( cbox.yMax >> 6 ) + ( y_सरणम् >> 6 );

    /* tiny remainder box */
    cbox.xMin = ( cbox.xMin & 63 ) + ( x_सरणम् & 63 );
    cbox.yMin = ( cbox.yMin & 63 ) + ( y_सरणम् & 63 );
    cbox.xMax = ( cbox.xMax & 63 ) + ( x_सरणम् & 63 );
    cbox.yMax = ( cbox.yMax & 63 ) + ( y_सरणम् & 63 );

    विकल्प ( बहुलकम् )
    {
    पक्ष FT_RENDER_बहुलकम्_MONO:
      pixel_बहुलकम् = FT_PIXEL_बहुलकम्_MONO;
#यदि 1
      /* x */

      /* undocumented but confirmed: bbox values get rounded;    */
      /* we do asymmetric rounding so that the center of a pixel */
      /* gets always included                                    */

      pbox.xMin += ( cbox.xMin + 31 ) >> 6;
      pbox.xMax += ( cbox.xMax + 32 ) >> 6;

      /* if the bbox collapsed, we add a pixel based on the total */
      /* rounding remainder to cover most of the original cbox    */

      यदि ( pbox.xMin == pbox.xMax )
      {
        यदि ( ( ( cbox.xMin + 31 ) & 63 ) - 31 +
             ( ( cbox.xMax + 32 ) & 63 ) - 32 < 0 )
          pbox.xMin -= 1;
        अन्यथा
          pbox.xMax += 1;
      }

      /* y */

      pbox.yMin += ( cbox.yMin + 31 ) >> 6;
      pbox.yMax += ( cbox.yMax + 32 ) >> 6;

      यदि ( pbox.yMin == pbox.yMax )
      {
        यदि ( ( ( cbox.yMin + 31 ) & 63 ) - 31 +
             ( ( cbox.yMax + 32 ) & 63 ) - 32 < 0 )
          pbox.yMin -= 1;
        अन्यथा
          pbox.yMax += 1;
      }

      भङ्ग;
#अन्यथा
      गच्छ Adjust;
#यदि_अन्तः

    पक्ष FT_RENDER_बहुलकम्_LCD:
      pixel_बहुलकम् = FT_PIXEL_बहुलकम्_LCD;
      ft_lcd_padding( &cbox, slot, बहुलकम् );
      गच्छ Adjust;

    पक्ष FT_RENDER_बहुलकम्_LCD_V:
      pixel_बहुलकम् = FT_PIXEL_बहुलकम्_LCD_V;
      ft_lcd_padding( &cbox, slot, बहुलकम् );
      गच्छ Adjust;

    पक्ष FT_RENDER_बहुलकम्_प्रसामान्यम्:
    पक्ष FT_RENDER_बहुलकम्_LIGHT:
    स्वभाव:
      pixel_बहुलकम् = FT_PIXEL_बहुलकम्_GRAY;
    Adjust:
      pbox.xMin += cbox.xMin >> 6;
      pbox.yMin += cbox.yMin >> 6;
      pbox.xMax += ( cbox.xMax + 63 ) >> 6;
      pbox.yMax += ( cbox.yMax + 63 ) >> 6;
    }

    x_वामः = pbox.xMin;
    y_शिखरम्  = pbox.yMax;

    विस्तारः  = pbox.xMax - pbox.xMin;
    height = pbox.yMax - pbox.yMin;

    /* Flag the width or height unsuitable for rendering.   */
    /* The limit is based on the ppem value when available. */
    /* FT_Renderer modules should check the return value.   */
    ret = FT_BOOL(     विस्तारः >= 0x10000   ||    height >= 0x10000   ||
                   pbox.xMin < -0x1000000 || pbox.xMax >= 0x1000000 ||
                   pbox.yMin < -0x1000000 || pbox.yMax >= 0x1000000 ||
             ( slot->face                                         &&
               ( विस्तारः  > 16 * slot->face->आकार->metrics.x_ppem ||
                 height > 16 * slot->face->आकार->metrics.y_ppem ) ) );

    यदि ( ret )
      FT_TRACE3(( "ft_glyphslot_preset_bitmap: [%ld %ld %ld %ld]\n",
                  pbox.xMin, pbox.yMin, pbox.xMax, pbox.yMax ));

    विकल्प ( pixel_बहुलकम् )
    {
    पक्ष FT_PIXEL_बहुलकम्_MONO:
      pitch = ( ( विस्तारः + 15 ) >> 4 ) << 1;
      भङ्ग;

    पक्ष FT_PIXEL_बहुलकम्_LCD:
      विस्तारः *= 3;
      pitch  = FT_PAD_CEIL( विस्तारः, 4 );
      भङ्ग;

    पक्ष FT_PIXEL_बहुलकम्_LCD_V:
      height *= 3;
      FALL_THROUGH;

    पक्ष FT_PIXEL_बहुलकम्_GRAY:
    स्वभाव:
      pitch = विस्तारः;
    }

    slot->bitmap_वामः = (FT_Int)x_वामः;
    slot->bitmap_शिखरम्  = (FT_Int)y_शिखरम्;

    bitmap->pixel_बहुलकम् = (अचिह्नित वर्ण)pixel_बहुलकम्;
    bitmap->अङ्कः_grays  = 256;
    bitmap->विस्तारः      = (अचिह्नित पूर्णाङ्क)विस्तारः;
    bitmap->पङ्क्तिकाः       = (अचिह्नित पूर्णाङ्क)height;
    bitmap->pitch      = pitch;

    प्रत्यागम ret;
  }


  FT_आधारः_DEF( शून्य )
  ft_glyphslot_समुच्चयः_bitmap( FT_GlyphSlot  slot,
                           FT_Byte*      बफर )
  {
    ft_glyphslot_free_bitmap( slot );

    slot->bitmap.बफर = बफर;

    FT_ASSERT( (slot->आन्तरिकम्->flags & FT_GLYPH_OWN_BITMAP) == 0 );
  }


  FT_आधारः_DEF( FT_Error )
  ft_glyphslot_alloc_bitmap( FT_GlyphSlot  slot )
  {
    FT_स्मृतिः  स्मृतिः = FT_FACE_स्मृतिः( slot->face );
    FT_Error   error;


    यदि ( slot->आन्तरिकम्->flags & FT_GLYPH_OWN_BITMAP )
      FT_FREE( slot->bitmap.बफर );
    अन्यथा
      slot->आन्तरिकम्->flags |= FT_GLYPH_OWN_BITMAP;

    /* dimensions must be preset */
    FT_MEM_ALLOC_MULT( slot->bitmap.बफर,
                       slot->bitmap.पङ्क्तिकाः,
                       slot->bitmap.pitch );
    प्रत्यागम error;
  }


  स्थिर शून्य
  ft_glyphslot_clear( FT_GlyphSlot  slot )
  {
    /* free bitmap if needed */
    ft_glyphslot_free_bitmap( slot );

    /* clear all public fields in the glyph slot */
    slot->glyph_सूचकांक = 0;

    FT_ZERO( &slot->metrics );
    FT_ZERO( &slot->outline );

    slot->bitmap.विस्तारः      = 0;
    slot->bitmap.पङ्क्तिकाः       = 0;
    slot->bitmap.pitch      = 0;
    slot->bitmap.pixel_बहुलकम् = 0;
    /* `slot->bitmap.buffer' has been handled by ft_glyphslot_free_bitmap */

    slot->bitmap_वामः   = 0;
    slot->bitmap_शिखरम्    = 0;
    slot->अङ्कः_subglyphs = 0;
    slot->subglyphs     = रिक्त;
    slot->control_दत्त  = रिक्त;
    slot->control_len   = 0;

#यदि_अपरिभाषितम् FT_CONFIG_OPTION_SVG
    slot->other = रिक्त;
#अन्यथा
    यदि ( !( slot->face->face_flags & FT_FACE_FLAG_SVG ) )
      slot->other = रिक्त;
    अन्यथा
    {
      यदि ( slot->आन्तरिकम्->flags & FT_GLYPH_OWN_GZIP_SVG )
      {
        FT_स्मृतिः        स्मृतिः = slot->face->स्मृतिः;
        FT_SVG_Document  doc    = (FT_SVG_Document)slot->other;


        FT_FREE( doc->svg_document );
        slot->आन्तरिकम्->flags &= ~FT_GLYPH_OWN_GZIP_SVG;
      }
    }
#यदि_अन्तः

    slot->format = FT_GLYPH_FORMAT_NONE;

    slot->रैखिकम्_Hori_Advance = 0;
    slot->रैखिकम्_Vert_Advance = 0;
    slot->advance.x         = 0;
    slot->advance.y         = 0;
    slot->lsb_delta         = 0;
    slot->rsb_delta         = 0;
  }


  स्थिर शून्य
  ft_glyphslot_समाप्तम्( FT_GlyphSlot  slot )
  {
    FT_Driver        driver = slot->face->driver;
    FT_Driver_Class  clazz  = driver->clazz;
    FT_स्मृतिः        स्मृतिः = driver->मूलम्.स्मृतिः;

#यदि_परिभाषितम् FT_CONFIG_OPTION_SVG
    यदि ( slot->face->face_flags & FT_FACE_FLAG_SVG )
    {
      /* Free memory in case SVG was there.                          */
      /* `slot->internal` might be NULL in out-of-memory situations. */
      यदि ( slot->आन्तरिकम् && slot->आन्तरिकम्->flags & FT_GLYPH_OWN_GZIP_SVG )
      {
        FT_SVG_Document  doc = (FT_SVG_Document)slot->other;


        FT_FREE( doc->svg_document );

        slot->आन्तरिकम्->flags &= ~FT_GLYPH_OWN_GZIP_SVG;
      }

      FT_FREE( slot->other );
    }
#यदि_अन्तः

    यदि ( clazz->समाप्तम्_slot )
      clazz->समाप्तम्_slot( slot );

    /* free bitmap buffer if needed */
    ft_glyphslot_free_bitmap( slot );

    /* slot->internal might be NULL in out-of-memory situations */
    यदि ( slot->आन्तरिकम् )
    {
      /* free glyph loader */
      यदि ( FT_DRIVER_उपयुङ्क्ते_OUTLINES( driver ) )
      {
        FT_Glyph_Loader_समाप्तम्( slot->आन्तरिकम्->loader );
        slot->आन्तरिकम्->loader = रिक्त;
      }

      FT_FREE( slot->आन्तरिकम् );
    }
  }


  /* documentation is in ftobjs.h */

  FT_आधारः_DEF( FT_Error )
  FT_New_GlyphSlot( FT_Face        face,
                    FT_GlyphSlot  *aslot )
  {
    FT_Error         error;
    FT_Driver        driver;
    FT_Driver_Class  clazz;
    FT_स्मृतिः        स्मृतिः;
    FT_GlyphSlot     slot = रिक्त;


    यदि ( !face )
      प्रत्यागम FT_THROW( Invalid_Face_Handle );

    यदि ( !face->driver )
      प्रत्यागम FT_THROW( Invalid_तर्कः );

    driver = face->driver;
    clazz  = driver->clazz;
    स्मृतिः = driver->मूलम्.स्मृतिः;

    FT_TRACE4(( "FT_New_GlyphSlot: Creating new slot object\n" ));
    यदि ( !FT_ALLOC( slot, clazz->slot_object_आकार ) )
    {
      slot->face = face;

      error = ft_glyphslot_init( slot );
      यदि ( error )
      {
        ft_glyphslot_समाप्तम्( slot );
        FT_FREE( slot );
        गच्छ Exit;
      }

      slot->अग्र  = face->glyph;
      face->glyph = slot;

      यदि ( aslot )
        *aslot = slot;
    }
    अन्यथा यदि ( aslot )
      *aslot = रिक्त;


  Exit:
    FT_TRACE4(( "FT_New_GlyphSlot: Return 0x%x\n", error ));

    प्रत्यागम error;
  }


  /* documentation is in ftobjs.h */

  FT_आधारः_DEF( शून्य )
  FT_समाप्तम्_Glyph_Slot( FT_GlyphSlot  slot )
  {
    यदि ( slot )
    {
      FT_Driver     driver = slot->face->driver;
      FT_स्मृतिः     स्मृतिः = driver->मूलम्.स्मृतिः;
      FT_GlyphSlot  पूर्व;
      FT_GlyphSlot  cur;


      /* Remove slot from its parent face's list */
      पूर्व = रिक्त;
      cur  = slot->face->glyph;

      यावत् ( cur )
      {
        यदि ( cur == slot )
        {
          यदि ( !पूर्व )
            slot->face->glyph = cur->अग्र;
          अन्यथा
            पूर्व->अग्र = cur->अग्र;

          /* finalize client-specific data */
          यदि ( slot->generic.finalizer )
            slot->generic.finalizer( slot );

          ft_glyphslot_समाप्तम्( slot );
          FT_FREE( slot );
          भङ्ग;
        }
        पूर्व = cur;
        cur  = cur->अग्र;
      }
    }
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( शून्य )
  FT_समुच्चयः_Transform( FT_Face     face,
                    FT_Matrix*  matrix,
                    FT_सदिशम्*  delta )
  {
    FT_Face_आन्तरिकम्  आन्तरिकम्;


    यदि ( !face )
      प्रत्यागम;

    आन्तरिकम् = face->आन्तरिकम्;

    आन्तरिकम्->transform_flags = 0;

    यदि ( !matrix || !FT_Matrix_Check( matrix ) )
    {
      आन्तरिकम्->transform_matrix.xx = 0x10000L;
      आन्तरिकम्->transform_matrix.xy = 0;
      आन्तरिकम्->transform_matrix.yx = 0;
      आन्तरिकम्->transform_matrix.yy = 0x10000L;

      matrix = &आन्तरिकम्->transform_matrix;
    }
    अन्यथा
      आन्तरिकम्->transform_matrix = *matrix;

    /* set transform_flags bit flag 0 if `matrix' isn't the identity */
    यदि ( ( matrix->xy | matrix->yx ) ||
         matrix->xx != 0x10000L      ||
         matrix->yy != 0x10000L      )
      आन्तरिकम्->transform_flags |= 1;

    यदि ( !delta )
    {
      आन्तरिकम्->transform_delta.x = 0;
      आन्तरिकम्->transform_delta.y = 0;

      delta = &आन्तरिकम्->transform_delta;
    }
    अन्यथा
      आन्तरिकम्->transform_delta = *delta;

    /* set transform_flags bit flag 1 if `delta' isn't the null vector */
    यदि ( delta->x | delta->y )
      आन्तरिकम्->transform_flags |= 2;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( शून्य )
  FT_Get_Transform( FT_Face     face,
                    FT_Matrix*  matrix,
                    FT_सदिशम्*  delta )
  {
    FT_Face_आन्तरिकम्  आन्तरिकम्;


    यदि ( !face )
      प्रत्यागम;

    आन्तरिकम् = face->आन्तरिकम्;

    यदि ( matrix )
      *matrix = आन्तरिकम्->transform_matrix;

    यदि ( delta )
      *delta = आन्तरिकम्->transform_delta;
  }


  स्थिर FT_Renderer
  ft_lookup_glyph_renderer( FT_GlyphSlot  slot );


#यदि_परिभाषितम् GRID_FIT_METRICS
  स्थिर शून्य
  ft_glyphslot_grid_fit_metrics( FT_GlyphSlot  slot,
                                 FT_Bool       vertical )
  {
    FT_Glyph_Metrics*  metrics = &slot->metrics;
    FT_Pos             दक्षिणः, bottom;


    यदि ( vertical )
    {
      metrics->horiBearingX = FT_PIX_FLOOR( metrics->horiBearingX );
      metrics->horiBearingY = FT_PIX_CEIL_LONG( metrics->horiBearingY );

      दक्षिणः  = FT_PIX_CEIL_LONG( सङ्कलय_LONG( metrics->vertBearingX,
                                           metrics->विस्तारः ) );
      bottom = FT_PIX_CEIL_LONG( सङ्कलय_LONG( metrics->vertBearingY,
                                           metrics->height ) );

      metrics->vertBearingX = FT_PIX_FLOOR( metrics->vertBearingX );
      metrics->vertBearingY = FT_PIX_FLOOR( metrics->vertBearingY );

      metrics->विस्तारः  = SUB_LONG( दक्षिणः,
                                  metrics->vertBearingX );
      metrics->height = SUB_LONG( bottom,
                                  metrics->vertBearingY );
    }
    अन्यथा
    {
      metrics->vertBearingX = FT_PIX_FLOOR( metrics->vertBearingX );
      metrics->vertBearingY = FT_PIX_FLOOR( metrics->vertBearingY );

      दक्षिणः  = FT_PIX_CEIL_LONG( सङ्कलय_LONG( metrics->horiBearingX,
                                           metrics->विस्तारः ) );
      bottom = FT_PIX_FLOOR( SUB_LONG( metrics->horiBearingY,
                                       metrics->height ) );

      metrics->horiBearingX = FT_PIX_FLOOR( metrics->horiBearingX );
      metrics->horiBearingY = FT_PIX_CEIL_LONG( metrics->horiBearingY );

      metrics->विस्तारः  = SUB_LONG( दक्षिणः,
                                  metrics->horiBearingX );
      metrics->height = SUB_LONG( metrics->horiBearingY,
                                  bottom );
    }

    metrics->horiAdvance = FT_PIX_आवर्तनम्_LONG( metrics->horiAdvance );
    metrics->vertAdvance = FT_PIX_आवर्तनम्_LONG( metrics->vertAdvance );
  }
#यदि_अन्तः /* GRID_FIT_METRICS */


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_आरोपय_Glyph( FT_Face   face,
                 FT_UInt   glyph_सूचकांक,
                 FT_Int32  आरोपय_flags )
  {
    FT_Error      error;
    FT_Driver     driver;
    FT_GlyphSlot  slot;
    FT_Library    library;
    FT_Bool       autohint = FALSE;
    FT_Module     hinter;


    यदि ( !face || !face->आकार || !face->glyph )
      प्रत्यागम FT_THROW( Invalid_Face_Handle );

    /* The validity test for `glyph_index' is performed by the */
    /* font drivers.                                           */

    slot = face->glyph;
    ft_glyphslot_clear( slot );

    driver  = face->driver;
    library = driver->मूलम्.library;
    hinter  = library->auto_hinter;

    /* undefined scale means no scale */
    यदि ( face->आकार->metrics.x_ppem == 0 ||
         face->आकार->metrics.y_ppem == 0 )
      आरोपय_flags |= FT_आरोपय_NO_SCALE;

    /* resolve load flags dependencies */

    यदि ( आरोपय_flags & FT_आरोपय_NO_RECURSE )
      आरोपय_flags |= FT_आरोपय_NO_SCALE         |
                    FT_आरोपय_IGNORE_TRANSFORM;

    यदि ( आरोपय_flags & FT_आरोपय_NO_SCALE )
    {
      आरोपय_flags |= FT_आरोपय_NO_HINTING |
                    FT_आरोपय_NO_BITMAP;

      आरोपय_flags &= ~FT_आरोपय_RENDER;
    }

    यदि ( आरोपय_flags & FT_आरोपय_BITMAP_METRICS_ONLY )
      आरोपय_flags &= ~FT_आरोपय_RENDER;

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

    यदि ( hinter                                           &&
         !( आरोपय_flags & FT_आरोपय_NO_HINTING )             &&
         !( आरोपय_flags & FT_आरोपय_NO_AUTOHINT )            &&
         FT_IS_SCALABLE( face )                           &&
         !FT_IS_TRICKY( face )                            &&
         ( ( आरोपय_flags & FT_आरोपय_IGNORE_TRANSFORM )    ||
           ( face->आन्तरिकम्->transform_matrix.yx == 0 &&
             face->आन्तरिकम्->transform_matrix.xx != 0 ) ||
           ( face->आन्तरिकम्->transform_matrix.xx == 0 &&
             face->आन्तरिकम्->transform_matrix.yx != 0 ) ) )
    {
      यदि ( ( आरोपय_flags & FT_आरोपय_FORCE_AUTOHINT ) ||
           !FT_DRIVER_HAS_HINTER( driver )         )
        autohint = TRUE;
      अन्यथा
      {
        FT_Render_बहुलकम्  बहुलकम् = FT_आरोपय_TARGET_बहुलकम्( आरोपय_flags );
        FT_Bool         is_light_type1;
        TT_Face         ttface = (TT_Face)face;


        /* only the new Adobe engine (for both CFF and Type 1) is `light'; */
        /* we use `strstr' to catch both `Type 1' and `CID Type 1'         */
        is_light_type1 =
          ft_strstr( FT_Get_Font_Format( face ), "Type 1" ) != रिक्त &&
          ((PS_Driver)driver)->hinting_engine == FT_HINTING_ADOBE;

        /* the check for `num_locations' assures that we actually    */
        /* test for instructions in a TTF and not in a CFF-based OTF */
        /*                                                           */
        /* we check the size of the `fpgm' and `prep' tables, too -- */
        /* the assumption is that there don't exist real TTFs where  */
        /* both `fpgm' and `prep' tables are missing                 */
        यदि ( ( बहुलकम् == FT_RENDER_बहुलकम्_LIGHT           &&
               ( !FT_DRIVER_HINTS_LIGHTLY( driver ) &&
                 !is_light_type1                    ) )         ||
             ( FT_IS_SFNT( face )                             &&
               ttface->अङ्कः_locations                          &&
               ttface->font_क्रमादेशः_आकार == 0                 &&
               ttface->cvt_क्रमादेशः_आकार <= 7                  ) )
          autohint = TRUE;
      }
    }

#यदि_परिभाषितम् TT_CONFIG_OPTION_VARC
    /* Never auto-hint a VARC (variable composite) glyph: its components */
    /* are already loaded unhinted, and the auto-fitter cannot process   */
    /* the assembled composite (it also re-enters variation-coordinate   */
    /* setting).  Use the native hinter / unhinted load instead.         */
    यदि ( autohint && FT_IS_SFNT( face ) )
    {
      FT_Service_VARC  varc;


      FT_FACE_FIND_SERVICE( face, varc, VARC );
      यदि ( varc && varc->has_glyph( face, glyph_सूचकांक ) )
        autohint = FALSE;
    }
#यदि_अन्तः

    यदि ( autohint )
    {
      FT_AutoHinter_Interface  hinting;


      /* XXX: The use of the `FT_LOAD_XXX_ONLY` flags is not very */
      /*      elegant.                                            */

      /* try to load SVG documents if available */
      यदि ( ( आरोपय_flags & FT_आरोपय_NO_SVG ) == 0 &&
           FT_HAS_SVG( face )                   )
      {
        error = driver->clazz->आरोपय_glyph( slot, face->आकार,
                                           glyph_सूचकांक,
                                           आरोपय_flags | FT_आरोपय_SVG_ONLY );

        यदि ( !error && slot->format == FT_GLYPH_FORMAT_SVG )
          गच्छ आरोपय_Ok;
      }

      /* try to load embedded bitmaps if available */
      यदि ( FT_HAS_FIXED_SIZES( face )              &&
           ( आरोपय_flags & FT_आरोपय_NO_BITMAP ) == 0 )
      {
        error = driver->clazz->आरोपय_glyph( slot, face->आकार,
                                           glyph_सूचकांक,
                                           आरोपय_flags | FT_आरोपय_SBITS_ONLY );

        यदि ( !error && slot->format == FT_GLYPH_FORMAT_BITMAP )
          गच्छ आरोपय_Ok;
      }

      {
        FT_Face_आन्तरिकम्  आन्तरिकम्        = face->आन्तरिकम्;
        FT_Int            transform_flags = आन्तरिकम्->transform_flags;


        /* since the auto-hinter calls FT_Load_Glyph by itself, */
        /* make sure that glyphs aren't transformed             */
        आन्तरिकम्->transform_flags = 0;

        /* load auto-hinted outline */
        hinting = (FT_AutoHinter_Interface)hinter->clazz->module_interface;

        error   = hinting->आरोपय_glyph( (FT_AutoHinter)hinter,
                                       slot, face->आकार,
                                       glyph_सूचकांक, आरोपय_flags );

        आन्तरिकम्->transform_flags = transform_flags;
      }
    }
    अन्यथा
    {
      error = driver->clazz->आरोपय_glyph( slot,
                                         face->आकार,
                                         glyph_सूचकांक,
                                         आरोपय_flags );
      यदि ( error )
        गच्छ Exit;

      यदि ( slot->format == FT_GLYPH_FORMAT_OUTLINE )
      {
        /* check that the loaded outline is correct */
        error = FT_Outline_Check( &slot->outline );
        यदि ( error )
          गच्छ Exit;

#यदि_परिभाषितम् GRID_FIT_METRICS
        यदि ( !( आरोपय_flags & FT_आरोपय_NO_HINTING ) )
          ft_glyphslot_grid_fit_metrics(
            slot,
            FT_BOOL( आरोपय_flags & FT_आरोपय_VERTICAL_LAYOUT ) );
#यदि_अन्तः
      }
    }

  आरोपय_Ok:
    /* compute the advance */
    यदि ( आरोपय_flags & FT_आरोपय_VERTICAL_LAYOUT )
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
    यदि ( ( आरोपय_flags & FT_आरोपय_रैखिकम्_DESIGN ) == 0 &&
         FT_IS_SCALABLE( face )                      )
    {
      FT_आकार_Metrics*  metrics = &face->आकार->metrics;


      /* it's tricky! */
      slot->रैखिकम्_Hori_Advance = FT_MulDiv( slot->रैखिकम्_Hori_Advance,
                                           metrics->x_scale, 64 );

      slot->रैखिकम्_Vert_Advance = FT_MulDiv( slot->रैखिकम्_Vert_Advance,
                                           metrics->y_scale, 64 );
    }

    यदि ( ( आरोपय_flags & FT_आरोपय_IGNORE_TRANSFORM ) == 0 )
    {
      FT_Face_आन्तरिकम्  आन्तरिकम् = face->आन्तरिकम्;


      /* now, transform the glyph image if needed */
      यदि ( आन्तरिकम्->transform_flags )
      {
        /* get renderer */
        FT_Renderer  renderer = ft_lookup_glyph_renderer( slot );


        यदि ( renderer )
          error = renderer->clazz->transform_glyph(
                                     renderer, slot,
                                     &आन्तरिकम्->transform_matrix,
                                     &आन्तरिकम्->transform_delta );
        अन्यथा यदि ( slot->format == FT_GLYPH_FORMAT_OUTLINE )
        {
          /* apply `standard' transformation if no renderer is available */
          यदि ( आन्तरिकम्->transform_flags & 1 )
            FT_Outline_Transform( &slot->outline,
                                  &आन्तरिकम्->transform_matrix );

          यदि ( आन्तरिकम्->transform_flags & 2 )
            FT_Outline_Translate( &slot->outline,
                                  आन्तरिकम्->transform_delta.x,
                                  आन्तरिकम्->transform_delta.y );
        }

        /* transform advance */
        FT_सदिशम्_Transform( &slot->advance, &आन्तरिकम्->transform_matrix );
      }
    }

    slot->glyph_सूचकांक          = glyph_सूचकांक;
    slot->आन्तरिकम्->आरोपय_flags = आरोपय_flags;

    /* do we need to render the image or preset the bitmap now? */
    यदि ( !error                                    &&
         ( आरोपय_flags & FT_आरोपय_NO_SCALE ) == 0    &&
         slot->format != FT_GLYPH_FORMAT_BITMAP    &&
         slot->format != FT_GLYPH_FORMAT_संयुक्त )
    {
      FT_Render_बहुलकम्  बहुलकम् = FT_आरोपय_TARGET_बहुलकम्( आरोपय_flags );


      यदि ( बहुलकम् == FT_RENDER_बहुलकम्_प्रसामान्यम्   &&
           आरोपय_flags & FT_आरोपय_MONOCHROME )
        बहुलकम् = FT_RENDER_बहुलकम्_MONO;

      यदि ( आरोपय_flags & FT_आरोपय_RENDER )
        error = FT_Render_Glyph( slot, बहुलकम् );
      अन्यथा
        ft_glyphslot_preset_bitmap( slot, बहुलकम्, रिक्त );
    }

#यदि_परिभाषितम् FT_DEBUG_LEVEL_TRACE
    FT_TRACE5(( "FT_Load_Glyph: index %u, flags 0x%x\n",
                glyph_सूचकांक, आरोपय_flags ));
    FT_TRACE5(( "  bitmap %ux%u %s, %s (mode %d)\n",
                slot->bitmap.विस्तारः,
                slot->bitmap.पङ्क्तिकाः,
                slot->outline.points ?
                  slot->bitmap.बफर ? "rendered"
                                      : "preset"
                                     :
                  slot->आन्तरिकम्->flags & FT_GLYPH_OWN_BITMAP ? "owned"
                                                              : "unowned",
                pixel_modes[slot->bitmap.pixel_बहुलकम्],
                slot->bitmap.pixel_बहुलकम् ));
    FT_TRACE5(( "\n" ));
    FT_TRACE5(( "  x advance: %f\n", (द्विगुण)slot->advance.x / 64 ));
    FT_TRACE5(( "  y advance: %f\n", (द्विगुण)slot->advance.y / 64 ));
    FT_TRACE5(( "  linear x advance: %f\n",
                (द्विगुण)slot->रैखिकम्_Hori_Advance / 65536 ));
    FT_TRACE5(( "  linear y advance: %f\n",
                (द्विगुण)slot->रैखिकम्_Vert_Advance / 65536 ));

    {
      FT_Glyph_Metrics*  metrics = &slot->metrics;


      FT_TRACE5(( "  metrics:\n" ));
      FT_TRACE5(( "    width:  %f\n", (द्विगुण)metrics->विस्तारः / 64 ));
      FT_TRACE5(( "    height: %f\n", (द्विगुण)metrics->height / 64 ));
      FT_TRACE5(( "\n" ));
      FT_TRACE5(( "    horiBearingX: %f\n",
                  (द्विगुण)metrics->horiBearingX / 64 ));
      FT_TRACE5(( "    horiBearingY: %f\n",
                  (द्विगुण)metrics->horiBearingY / 64 ));
      FT_TRACE5(( "    horiAdvance:  %f\n",
                  (द्विगुण)metrics->horiAdvance / 64 ));
      FT_TRACE5(( "\n" ));
      FT_TRACE5(( "    vertBearingX: %f\n",
                  (द्विगुण)metrics->vertBearingX / 64 ));
      FT_TRACE5(( "    vertBearingY: %f\n",
                  (द्विगुण)metrics->vertBearingY / 64 ));
      FT_TRACE5(( "    vertAdvance:  %f\n",
                  (द्विगुण)metrics->vertAdvance / 64 ));
    }
#यदि_अन्तः

  Exit:
    प्रत्यागम error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_आरोपय_Char( FT_Face   face,
                FT_ULong  char_संहिता,
                FT_Int32  आरोपय_flags )
  {
    FT_UInt  glyph_सूचकांक;


    यदि ( !face )
      प्रत्यागम FT_THROW( Invalid_Face_Handle );

    glyph_सूचकांक = (FT_UInt)char_संहिता;
    यदि ( face->charmap )
      glyph_सूचकांक = FT_Get_Char_सूचकांक( face, char_संहिता );

    प्रत्यागम FT_आरोपय_Glyph( face, glyph_सूचकांक, आरोपय_flags );
  }


  /* destructor for sizes list */
  स्थिर शून्य
  destroy_आकार( FT_स्मृतिः  स्मृतिः,
                शून्य*      आकार_2,
                शून्य*      driver_ )
  {
    FT_आकार    आकार   = (FT_आकार)आकार_2;
    FT_Driver  driver = (FT_Driver)driver_;


    /* finalize format-specific stuff */
    यदि ( driver->clazz->समाप्तम्_आकार )
      driver->clazz->समाप्तम्_आकार( आकार );

    /* finalize client-specific data */
    यदि ( आकार->generic.finalizer )
      आकार->generic.finalizer( आकार );

    FT_FREE( आकार->आन्तरिकम् );
    FT_FREE( आकार );
  }


  स्थिर शून्य
  ft_cmap_समाप्तम्_आन्तरिकम्( FT_CMap  cmap );


  स्थिर शून्य
  destroy_charmaps( FT_Face    face,
                    FT_स्मृतिः  स्मृतिः )
  {
    FT_Int  n;


    यदि ( !face )
      प्रत्यागम;

    प्रति ( n = 0; n < face->अङ्कः_charmaps; n++ )
    {
      FT_CMap  cmap = FT_CMAP( face->charmaps[n] );


      ft_cmap_समाप्तम्_आन्तरिकम्( cmap );

      face->charmaps[n] = रिक्त;
    }

    FT_FREE( face->charmaps );
    face->अङ्कः_charmaps = 0;
  }


  /* destructor for faces list */
  स्थिर शून्य
  destroy_face( FT_स्मृतिः  स्मृतिः,
                शून्य*      face_,
                शून्य*      driver_ )
  {
    FT_Face          face   = (FT_Face)face_;
    FT_Driver        driver = (FT_Driver)driver_;
    FT_Driver_Class  clazz  = driver->clazz;


    /* discard auto-hinting data */
    यदि ( face->autohint.finalizer )
      face->autohint.finalizer( face->autohint.दत्त );

    /* Discard glyph slots for this face.                           */
    /* Beware!  FT_Done_GlyphSlot() changes the field `face->glyph' */
    यावत् ( face->glyph )
      FT_समाप्तम्_Glyph_Slot( face->glyph );

    /* discard all sizes for this face */
    FT_List_Finalize( &face->sizes_list,
                      destroy_आकार,
                      स्मृतिः,
                      driver );
    face->आकार = रिक्त;

    /* discard charmaps */
    destroy_charmaps( face, स्मृतिः );

    /* finalize format-specific stuff */
    यदि ( clazz->समाप्तम्_face )
      clazz->समाप्तम्_face( face );

    /* close the stream for this face if needed */
    FT_प्रवाहः_Free(
      face->प्रवाहः,
      ( face->face_flags & FT_FACE_FLAG_EXTERNAL_प्रवाहः ) != 0 );

    face->प्रवाहः = रिक्त;

    /* now discard client data */
    यदि ( face->generic.finalizer )
      face->generic.finalizer( face );

    /* get rid of it */
    यदि ( face->आन्तरिकम् )
    {
      FT_FREE( face->आन्तरिकम् );
    }
    FT_FREE( face );
  }


  स्थिर शून्य
  Destroy_Driver( FT_Driver  driver )
  {
    FT_List_Finalize( &driver->faces_list,
                      destroy_face,
                      driver->मूलम्.स्मृतिः,
                      driver );
  }


  /* documentation is in ftobjs.h */

  FT_आधारः_DEF( FT_Error )
  find_unicode_charmap( FT_Face  face )
  {
    FT_CharMap*  first;
    FT_CharMap*  cur;


    /* caller should have already checked that `face' is valid */
    FT_ASSERT( face );

    first = face->charmaps;

    यदि ( !first )
      प्रत्यागम FT_THROW( Invalid_CharMap_Handle );

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

    cur = first + face->अङ्कः_charmaps;  /* points after the last one */

    प्रति ( ; --cur >= first; )
    {
      यदि ( cur[0]->encoding == FT_ENCODING_UNICODE )
      {
        /* XXX If some new encodings to represent UCS-4 are added, */
        /*     they should be added here.                          */
        यदि ( ( cur[0]->platform_id == TT_PLATFORM_MICROSOFT &&
               cur[0]->encoding_id == TT_MS_ID_UCS_4        )     ||
             ( cur[0]->platform_id == TT_PLATFORM_APPLE_UNICODE &&
               cur[0]->encoding_id == TT_APPLE_ID_UNICODE_32    ) ||
             ( cur[0]->platform_id == TT_PLATFORM_APPLE_UNICODE &&
               cur[0]->encoding_id == TT_APPLE_ID_पूर्णम्_UNICODE  &&
               FT_Get_CMap_Format( cur[0] ) == 13               ) )
        {
          face->charmap = cur[0];
          प्रत्यागम FT_Err_Ok;
        }
      }
    }

    /* We do not have any UCS-4 charmap.                */
    /* Do the loop again and search for UCS-2 charmaps. */
    cur = first + face->अङ्कः_charmaps;

    प्रति ( ; --cur >= first; )
    {
      यदि ( cur[0]->encoding == FT_ENCODING_UNICODE )
      {
        FT_Long  format = FT_Get_CMap_Format( cur[0] );


        यदि ( format != 14 && format != 15 )
        {
          face->charmap = cur[0];
          प्रत्यागम FT_Err_Ok;
        }
      }
    }

    प्रत्यागम FT_THROW( Invalid_CharMap_Handle );
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
  स्थिर FT_CharMap
  find_variant_selector_charmap( FT_Face  face )
  {
    FT_CharMap*  first;
    FT_CharMap*  अन्तः;
    FT_CharMap*  cur;


    /* caller should have already checked that `face' is valid */
    FT_ASSERT( face );

    first = face->charmaps;

    यदि ( !first )
      प्रत्यागम रिक्त;

    अन्तः = first + face->अङ्कः_charmaps;  /* points after the last one */

    प्रति ( cur = first; cur < अन्तः; cur++ )
    {
      यदि ( cur[0]->platform_id == TT_PLATFORM_APPLE_UNICODE    &&
           cur[0]->encoding_id == TT_APPLE_ID_VARIANT_SELECTOR )
      {
        FT_Long  format = FT_Get_CMap_Format( cur[0] );


        यदि ( format == 14 || format == 15 )
          प्रत्यागम cur[0];
      }
    }

    प्रत्यागम रिक्त;
  }


  /**************************************************************************
   *
   * @Function:
   *   open_face
   *
   * @Description:
   *   This function does some work for FT_Open_Face().
   */
  स्थिर FT_Error
  open_face( FT_Driver      driver,
             FT_प्रवाहः      *astream,
             FT_Bool        *anexternal_प्रवाहः,
             FT_Long        face_सूचकांक,
             FT_Int         अङ्कः_params,
             FT_Parameter*  params,
             FT_Face       *aface )
  {
    FT_स्मृतिः         स्मृतिः;
    FT_Driver_Class   clazz;
    FT_Face           face     = रिक्त;
    FT_Face_आन्तरिकम्  आन्तरिकम् = रिक्त;

    FT_Error          error, error2;


    clazz  = driver->clazz;
    स्मृतिः = driver->मूलम्.स्मृतिः;

    /* allocate the face object and perform basic initialization */
    यदि ( FT_ALLOC( face, clazz->face_object_आकार ) )
      गच्छ Fail;

    face->driver = driver;
    face->स्मृतिः = स्मृतिः;
    face->प्रवाहः = *astream;

    /* set the FT_FACE_FLAG_EXTERNAL_STREAM bit for FT_Done_Face */
    यदि ( *anexternal_प्रवाहः )
      face->face_flags |= FT_FACE_FLAG_EXTERNAL_प्रवाहः;

    यदि ( FT_NEW( आन्तरिकम् ) )
      गच्छ Fail;

    face->आन्तरिकम् = आन्तरिकम्;

#यदि_परिभाषितम् FT_CONFIG_OPTION_INCREMENTAL
    {
      पूर्णाङ्क  i;


      face->आन्तरिकम्->incremental_interface = रिक्त;
      प्रति ( i = 0; i < अङ्कः_params && !face->आन्तरिकम्->incremental_interface;
            i++ )
        यदि ( params[i].चिह्नकम् == FT_PARAM_चिह्नकम्_INCREMENTAL )
          face->आन्तरिकम्->incremental_interface =
            (FT_Incremental_Interface)params[i].दत्त;
    }
#यदि_अन्तः

    face->आन्तरिकम्->random_बीजम् = -1;

    यदि ( clazz->init_face )
      error = clazz->init_face( *astream,
                                face,
                                (FT_Int)face_सूचकांक,
                                अङ्कः_params,
                                params );
    /* Stream may have been changed. */
    *astream = face->प्रवाहः;
    *anexternal_प्रवाहः =
      ( face->face_flags & FT_FACE_FLAG_EXTERNAL_प्रवाहः ) != 0;
    यदि ( error )
      गच्छ Fail;

    /* select Unicode charmap by default */
    error2 = find_unicode_charmap( face );

    /* if no Unicode charmap can be found, FT_Err_Invalid_CharMap_Handle */
    /* is returned.                                                      */

    /* no error should happen, but we want to play safe */
    यदि ( error2 && FT_ERR_NEQ( error2, Invalid_CharMap_Handle ) )
    {
      error = error2;
      गच्छ Fail;
    }

    *aface = face;

  Fail:
    यदि ( error )
    {
      destroy_charmaps( face, स्मृतिः );
      यदि ( clazz->समाप्तम्_face )
        clazz->समाप्तम्_face( face );
      FT_FREE( आन्तरिकम् );
      FT_FREE( face );
      *aface = रिक्त;
    }

    प्रत्यागम error;
  }


  /* there's a Mac-specific extended implementation of FT_New_Face() */
  /* in src/base/ftmac.c                                             */

#यदि_अपरिभाषितम् FT_MACINTOSH

  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_New_Face( FT_Library   library,
               ध्रुव वर्ण*  pathname,
               FT_Long      face_सूचकांक,
               FT_Face     *aface )
  {
    FT_Open_तर्काः  तर्काः;


    /* test for valid `library' and `aface' delayed to `FT_Open_Face' */
    यदि ( !pathname )
      प्रत्यागम FT_THROW( Invalid_तर्कः );

    तर्काः.flags    = FT_OPEN_PATHNAME;
    तर्काः.pathname = (वर्ण*)pathname;
    तर्काः.प्रवाहः   = रिक्त;

    प्रत्यागम ft_open_face_आन्तरिकम्( library, &तर्काः, face_सूचकांक, aface, 1 );
  }

#यदि_अन्तः


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_New_स्मृतिः_Face( FT_Library      library,
                      ध्रुव FT_Byte*  file_आधारः,
                      FT_Long         file_आकार,
                      FT_Long         face_सूचकांक,
                      FT_Face        *aface )
  {
    FT_Open_तर्काः  तर्काः;


    /* test for valid `library' and `face' delayed to `FT_Open_Face' */
    यदि ( !file_आधारः )
      प्रत्यागम FT_THROW( Invalid_तर्कः );

    तर्काः.flags       = FT_OPEN_स्मृतिः;
    तर्काः.स्मृतिः_आधारः = file_आधारः;
    तर्काः.स्मृतिः_आकार = file_आकार;
    तर्काः.प्रवाहः      = रिक्त;

    प्रत्यागम ft_open_face_आन्तरिकम्( library, &तर्काः, face_सूचकांक, aface, 1 );
  }


#यदि_परिभाषितम् FT_CONFIG_OPTION_MAC_FONTS

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
  स्थिर शून्य
  स्मृतिः_प्रवाहः_close( FT_प्रवाहः  प्रवाहः )
  {
    FT_स्मृतिः  स्मृतिः = (FT_स्मृतिः)प्रवाहः->descriptor.pointer;


    FT_FREE( प्रवाहः->आधारः );
    प्रवाहः->आकार  = 0;
    प्रवाहः->close = रिक्त;
    FT_FREE( प्रवाहः );
  }


  /* Create a new memory stream from a buffer and a size. */
  /* From `ftmac.c'.                                      */
  स्थिर FT_Error
  new_स्मृतिः_प्रवाहः( FT_Library           library,
                     FT_Byte*             आधारः,
                     FT_ULong             आकार,
                     FT_प्रवाहः_Close_Func  close,
                     FT_प्रवाहः           *astream )
  {
    FT_Error   error;
    FT_स्मृतिः  स्मृतिः;
    FT_प्रवाहः  प्रवाहः = रिक्त;


    यदि ( !library )
      प्रत्यागम FT_THROW( Invalid_Library_Handle );

    यदि ( !आधारः )
      प्रत्यागम FT_THROW( Invalid_तर्कः );

    *astream = रिक्त;
    स्मृतिः   = library->स्मृतिः;
    यदि ( FT_NEW( प्रवाहः ) )
      गच्छ Exit;

    FT_प्रवाहः_Open_स्मृतिः( प्रवाहः, आधारः, आकार );

    प्रवाहः->descriptor.pointer = स्मृतिः;
    प्रवाहः->close              = close;

    *astream = प्रवाहः;

  Exit:
    प्रत्यागम error;
  }


  /* Create a new FT_Face given a buffer and a driver name. */
  /* From `ftmac.c'.                                        */
  FT_LOCAL_DEF( FT_Error )
  open_face_from_बफर( FT_Library   library,
                         FT_Byte*     आधारः,
                         FT_ULong     आकार,
                         FT_Long      face_सूचकांक,
                         ध्रुव वर्ण*  driver_name,
                         FT_Face     *aface )
  {
    FT_Open_तर्काः  तर्काः;
    FT_Error      error;
    FT_स्मृतिः     स्मृतिः = library->स्मृतिः;


    तर्काः.driver = रिक्त;
    तर्काः.flags  = 0;

    यदि ( driver_name )
    {
      तर्काः.driver = FT_Get_Module( library, driver_name );
      यदि ( !तर्काः.driver )
      {
        FT_FREE( आधारः );
        प्रत्यागम FT_THROW( Missing_Module );
      }

      तर्काः.flags = तर्काः.flags | FT_OPEN_DRIVER;
    }

    /* `memory_stream_close` also frees the stream object. */
    error = new_स्मृतिः_प्रवाहः( library,
                               आधारः,
                               आकार,
                               स्मृतिः_प्रवाहः_close,
                               &तर्काः.प्रवाहः );
    यदि ( error )
    {
      FT_FREE( आधारः );
      प्रत्यागम error;
    }

    तर्काः.flags |= FT_OPEN_प्रवाहः;

#यदि_परिभाषितम् FT_MACINTOSH
    /* At this point, the face index has served its purpose;  */
    /* whoever calls this function has already used it to     */
    /* locate the correct font data.  We should not propagate */
    /* this index to FT_Open_Face() (unless it is negative).  */

    यदि ( face_सूचकांक > 0 )
      face_सूचकांक &= 0x7FFF0000L; /* retain GX data */
#यदि_अन्तः

    प्रत्यागम ft_open_face_आन्तरिकम्( library, &तर्काः, face_सूचकांक, aface, 0 );
  }


  /* Look up `TYP1' or `CID ' table from sfnt table directory.       */
  /* `offset' and `length' must exclude the binary header in tables. */

  /* Type 1 and CID-keyed font drivers should recognize sfnt-wrapped */
  /* format too.  Here, since we can't expect that the TrueType font */
  /* driver is loaded unconditionally, we must parse the font by     */
  /* ourselves.  We are only interested in the name of the table and */
  /* the offset.                                                     */

  स्थिर FT_Error
  ft_lookup_PS_अन्तर्_sfnt_प्रवाहः( FT_प्रवाहः  प्रवाहः,
                               FT_Long    face_सूचकांक,
                               FT_ULong*  विस्थापनम्,
                               FT_ULong*  दीर्घता,
                               FT_Bool*   is_sfnt_cid )
  {
    FT_Error   error;
    FT_UShort  अङ्कः_Tables;
    FT_Long    pstable_सूचकांक;
    FT_ULong   चिह्नकम्;
    पूर्णाङ्क        i;


    *विस्थापनम् = 0;
    *दीर्घता = 0;
    *is_sfnt_cid = FALSE;

    /* TODO: support for sfnt-wrapped PS/CID in TTC format */

    /* version check for 'typ1' (should be ignored?) */
    यदि ( FT_READ_ULONG( चिह्नकम् ) )
      प्रत्यागम error;
    यदि ( चिह्नकम् != TTAG_typ1 )
      प्रत्यागम FT_THROW( Unknown_File_Format );

    यदि ( FT_READ_USHORT( अङ्कः_Tables ) )
      प्रत्यागम error;
    यदि ( FT_प्रवाहः_SKIP( 2 * 3 ) ) /* skip binary search header */
      प्रत्यागम error;

    pstable_सूचकांक = -1;
    *is_sfnt_cid  = FALSE;

    प्रति ( i = 0; i < अङ्कः_Tables; i++ )
    {
      यदि ( FT_READ_ULONG( चिह्नकम् )     || FT_प्रवाहः_SKIP( 4 )      ||
           FT_READ_ULONG( *विस्थापनम् ) || FT_READ_ULONG( *दीर्घता ) )
        प्रत्यागम error;

      यदि ( चिह्नकम् == TTAG_CID )
      {
        pstable_सूचकांक++;
        *विस्थापनम् += 22;
        *दीर्घता -= 22;
        *is_sfnt_cid = TRUE;
        यदि ( face_सूचकांक < 0 )
          प्रत्यागम FT_Err_Ok;
      }
      अन्यथा यदि ( चिह्नकम् == TTAG_TYP1 )
      {
        pstable_सूचकांक++;
        *विस्थापनम् += 24;
        *दीर्घता -= 24;
        *is_sfnt_cid = FALSE;
        यदि ( face_सूचकांक < 0 )
          प्रत्यागम FT_Err_Ok;
      }
      यदि ( face_सूचकांक >= 0 && pstable_सूचकांक == face_सूचकांक )
        प्रत्यागम FT_Err_Ok;
    }

    प्रत्यागम FT_THROW( सारणी_Missing );
  }


  FT_LOCAL_DEF( FT_Error )
  open_face_PS_from_sfnt_प्रवाहः( FT_Library     library,
                                 FT_प्रवाहः      प्रवाहः,
                                 FT_Long        face_सूचकांक,
                                 FT_Int         अङ्कः_params,
                                 FT_Parameter  *params,
                                 FT_Face       *aface )
  {
    FT_Error   error;
    FT_स्मृतिः  स्मृतिः = library->स्मृतिः;
    FT_ULong   विस्थापनम्, दीर्घता;
    FT_ULong   pos;
    FT_Bool    is_sfnt_cid;
    FT_Byte*   sfnt_ps = रिक्त;

    FT_अप्रयुक्तम्( अङ्कः_params );
    FT_अप्रयुक्तम्( params );


    /* ignore GX stuff */
    यदि ( face_सूचकांक > 0 )
      face_सूचकांक &= 0xFFFFL;

    pos = FT_प्रवाहः_POS();

    error = ft_lookup_PS_अन्तर्_sfnt_प्रवाहः( प्रवाहः,
                                         face_सूचकांक,
                                         &विस्थापनम्,
                                         &दीर्घता,
                                         &is_sfnt_cid );
    यदि ( error )
      गच्छ Exit;

    यदि ( विस्थापनम् > प्रवाहः->आकार )
    {
      FT_TRACE2(( "open_face_PS_from_sfnt_stream: invalid table offset\n" ));
      error = FT_THROW( Invalid_सारणी );
      गच्छ Exit;
    }
    अन्यथा यदि ( दीर्घता > प्रवाहः->आकार - विस्थापनम् )
    {
      FT_TRACE2(( "open_face_PS_from_sfnt_stream: invalid table length\n" ));
      error = FT_THROW( Invalid_सारणी );
      गच्छ Exit;
    }

    error = FT_प्रवाहः_Seek( प्रवाहः, pos + विस्थापनम् );
    यदि ( error )
      गच्छ Exit;

    यदि ( FT_QALLOC( sfnt_ps, (FT_Long)दीर्घता ) )
      गच्छ Exit;

    error = FT_प्रवाहः_Read( प्रवाहः, (FT_Byte *)sfnt_ps, दीर्घता );
    यदि ( error )
    {
      FT_FREE( sfnt_ps );
      गच्छ Exit;
    }

    error = open_face_from_बफर( library,
                                   sfnt_ps,
                                   दीर्घता,
                                   FT_MIN( face_सूचकांक, 0 ),
                                   is_sfnt_cid ? "t1cid" : "type1",
                                   aface );
  Exit:
    {
      FT_Error  error1;


      यदि ( FT_ERR_EQ( error, Unknown_File_Format ) )
      {
        error1 = FT_प्रवाहः_Seek( प्रवाहः, pos );
        यदि ( error1 )
          प्रत्यागम error1;
      }

      प्रत्यागम error;
    }
  }


#यदि_अपरिभाषितम् FT_MACINTOSH

  /* The resource header says we've got resource_cnt `POST' (type1) */
  /* resources in this file.  They all need to be coalesced into    */
  /* one lump which gets passed on to the type1 driver.             */
  /* Here can be only one PostScript font in a file so face_index   */
  /* must be 0 (or -1).                                             */
  /*                                                                */
  स्थिर FT_Error
  Mac_Read_POST_Resource( FT_Library  library,
                          FT_प्रवाहः   प्रवाहः,
                          FT_Long    *offsets,
                          FT_Long     resource_cnt,
                          FT_Long     face_सूचकांक,
                          FT_Face    *aface )
  {
    FT_Error   error  = FT_ERR( Cannot_Open_Resource );
    FT_स्मृतिः  स्मृतिः = library->स्मृतिः;

    FT_Byte*   pfb_दत्त = रिक्त;
    पूर्णाङ्क        i, type, flags;
    FT_ULong   len;
    FT_ULong   pfb_len, pfb_pos, pfb_lenpos;
    FT_ULong   rlen, अस्थायी;


    यदि ( face_सूचकांक == -1 )
      face_सूचकांक = 0;
    यदि ( face_सूचकांक != 0 )
      प्रत्यागम error;

    /* Find the length of all the POST resources, concatenated.  Assume */
    /* worst case (each resource in its own section).                   */
    pfb_len = 0;
    प्रति ( i = 0; i < resource_cnt; i++ )
    {
      error = FT_प्रवाहः_Seek( प्रवाहः, (FT_ULong)offsets[i] );
      यदि ( error )
        गच्छ Exit;
      यदि ( FT_READ_ULONG( अस्थायी ) )  /* actually LONG */
        गच्छ Exit;

      /* FT2 allocator takes signed long buffer length,
       * too large value causing overflow should be checked
       */
      FT_TRACE4(( "                 POST fragment #%d: length=0x%08lx"
                  " total pfb_len=0x%08lx\n",
                  i, अस्थायी, pfb_len + अस्थायी + 6 ));

      यदि ( FT_MAC_RFORK_MAX_LEN < अस्थायी               ||
           FT_MAC_RFORK_MAX_LEN - अस्थायी < pfb_len + 6 )
      {
        FT_TRACE2(( "             MacOS resource length cannot exceed"
                    " 0x%08lx\n",
                    FT_MAC_RFORK_MAX_LEN ));

        error = FT_THROW( Invalid_विस्थापनम् );
        गच्छ Exit;
      }

      pfb_len += अस्थायी + 6;
    }

    FT_TRACE2(( "             total buffer size to concatenate"
                " %ld POST fragments: 0x%08lx\n",
                 resource_cnt, pfb_len + 2 ));

    यदि ( pfb_len + 2 < 6 )
    {
      FT_TRACE2(( "             too long fragment length makes"
                  " pfb_len confused: pfb_len=0x%08lx\n",
                  pfb_len ));

      error = FT_THROW( Array_Too_Large );
      गच्छ Exit;
    }

    यदि ( FT_QALLOC( pfb_दत्त, (FT_Long)pfb_len + 2 ) )
      गच्छ Exit;

    pfb_दत्त[0] = 0x80;
    pfb_दत्त[1] = 1;            /* Ascii section */
    pfb_दत्त[2] = 0;            /* 4-byte length, fill in later */
    pfb_दत्त[3] = 0;
    pfb_दत्त[4] = 0;
    pfb_दत्त[5] = 0;
    pfb_pos     = 6;
    pfb_lenpos  = 2;

    len  = 0;
    type = 1;

    प्रति ( i = 0; i < resource_cnt; i++ )
    {
      error = FT_प्रवाहः_Seek( प्रवाहः, (FT_ULong)offsets[i] );
      यदि ( error )
        गच्छ Exit2;
      यदि ( FT_READ_ULONG( rlen ) )
        गच्छ Exit2;

      /* FT2 allocator takes signed long buffer length,
       * too large fragment length causing overflow should be checked
       */
      यदि ( 0x7FFFFFFFUL < rlen )
      {
        error = FT_THROW( Invalid_विस्थापनम् );
        गच्छ Exit2;
      }

      यदि ( FT_READ_USHORT( flags ) )
        गच्छ Exit2;

      FT_TRACE3(( "POST fragment[%d]:"
                  " offsets=0x%08lx, rlen=0x%08lx, flags=0x%04x\n",
                  i, offsets[i], rlen, flags ));

      error = FT_ERR( Array_Too_Large );

      /* postpone the check of `rlen longer than buffer' */
      /* until `FT_Stream_Read'                          */

      यदि ( ( flags >> 8 ) == 0 )        /* Comment, should not be loaded */
      {
        FT_TRACE3(( "    Skip POST fragment #%d because it is a comment\n",
                    i ));
        अनुवर्त;
      }

      /* the flags are part of the resource, so rlen >= 2,  */
      /* but some fonts declare rlen = 0 for empty fragment */
      यदि ( rlen > 2 )
        rlen -= 2;
      अन्यथा
        rlen = 0;

      यदि ( ( flags >> 8 ) == type )
        len += rlen;
      अन्यथा
      {
        FT_TRACE3(( "    Write POST fragment #%d header (4-byte) to buffer"
                    " %p + 0x%08lx\n",
                    i, (शून्य*)pfb_दत्त, pfb_lenpos ));

        यदि ( pfb_lenpos + 3 > pfb_len + 2 )
          गच्छ Exit2;

        pfb_दत्त[pfb_lenpos    ] = (FT_Byte)( len );
        pfb_दत्त[pfb_lenpos + 1] = (FT_Byte)( len >> 8 );
        pfb_दत्त[pfb_lenpos + 2] = (FT_Byte)( len >> 16 );
        pfb_दत्त[pfb_lenpos + 3] = (FT_Byte)( len >> 24 );

        यदि ( ( flags >> 8 ) == 5 )      /* End of font mark */
          भङ्ग;

        FT_TRACE3(( "    Write POST fragment #%d header (6-byte) to buffer"
                    " %p + 0x%08lx\n",
                    i, (शून्य*)pfb_दत्त, pfb_pos ));

        यदि ( pfb_pos + 6 > pfb_len + 2 )
          गच्छ Exit2;

        pfb_दत्त[pfb_pos++] = 0x80;

        type = flags >> 8;
        len  = rlen;

        pfb_दत्त[pfb_pos++] = (FT_Byte)type;
        pfb_lenpos          = pfb_pos;
        pfb_दत्त[pfb_pos++] = 0;        /* 4-byte length, fill in later */
        pfb_दत्त[pfb_pos++] = 0;
        pfb_दत्त[pfb_pos++] = 0;
        pfb_दत्त[pfb_pos++] = 0;
      }

      यदि ( pfb_pos > pfb_len || pfb_pos + rlen > pfb_len )
        गच्छ Exit2;

      FT_TRACE3(( "    Load POST fragment #%d (%lu byte) to buffer"
                  " %p + 0x%08lx\n",
                  i, rlen, (शून्य*)pfb_दत्त, pfb_pos ));

      error = FT_प्रवाहः_Read( प्रवाहः, (FT_Byte *)pfb_दत्त + pfb_pos, rlen );
      यदि ( error )
        गच्छ Exit2;

      pfb_pos += rlen;
    }

    error = FT_ERR( Array_Too_Large );

    यदि ( pfb_pos + 2 > pfb_len + 2 )
      गच्छ Exit2;
    pfb_दत्त[pfb_pos++] = 0x80;
    pfb_दत्त[pfb_pos++] = 3;

    यदि ( pfb_lenpos + 3 > pfb_len + 2 )
      गच्छ Exit2;
    pfb_दत्त[pfb_lenpos    ] = (FT_Byte)( len );
    pfb_दत्त[pfb_lenpos + 1] = (FT_Byte)( len >> 8 );
    pfb_दत्त[pfb_lenpos + 2] = (FT_Byte)( len >> 16 );
    pfb_दत्त[pfb_lenpos + 3] = (FT_Byte)( len >> 24 );

    प्रत्यागम open_face_from_बफर( library,
                                  pfb_दत्त,
                                  pfb_pos,
                                  face_सूचकांक,
                                  "type1",
                                  aface );

  Exit2:
    यदि ( FT_ERR_EQ( error, Array_Too_Large ) )
      FT_TRACE2(( "  Abort due to too-short buffer to store"
                  " all POST fragments\n" ));
    अन्यथा यदि ( FT_ERR_EQ( error, Invalid_विस्थापनम् ) )
      FT_TRACE2(( "  Abort due to invalid offset in a POST fragment\n" ));

    यदि ( error )
      error = FT_ERR( Cannot_Open_Resource );
    FT_FREE( pfb_दत्त );

  Exit:
    प्रत्यागम error;
  }


  /* The resource header says we've got resource_cnt `sfnt'      */
  /* (TrueType/OpenType) resources in this file.  Look through   */
  /* them for the one indicated by face_index, load it into mem, */
  /* pass it on to the truetype driver, and return it.           */
  /*                                                             */
  स्थिर FT_Error
  Mac_Read_sfnt_Resource( FT_Library  library,
                          FT_प्रवाहः   प्रवाहः,
                          FT_Long    *offsets,
                          FT_Long     resource_cnt,
                          FT_Long     face_सूचकांक,
                          FT_Face    *aface )
  {
    FT_स्मृतिः  स्मृतिः = library->स्मृतिः;
    FT_Byte*   sfnt_दत्त = रिक्त;
    FT_Error   error;
    FT_ULong   flag_विस्थापनम्;
    FT_ULong   rlen;
    पूर्णाङ्क        is_cff;
    FT_Long    face_सूचकांक_अन्तर्_resource = 0;


    यदि ( face_सूचकांक < 0 )
      face_सूचकांक = -face_सूचकांक - 1;
    यदि ( face_सूचकांक >= resource_cnt )
      प्रत्यागम FT_THROW( Cannot_Open_Resource );

    flag_विस्थापनम् = (FT_ULong)offsets[face_सूचकांक];
    error = FT_प्रवाहः_Seek( प्रवाहः, flag_विस्थापनम् );
    यदि ( error )
      गच्छ Exit;

    यदि ( FT_READ_ULONG( rlen ) )
      गच्छ Exit;
    यदि ( !rlen )
      प्रत्यागम FT_THROW( Cannot_Open_Resource );
    यदि ( rlen > FT_MAC_RFORK_MAX_LEN )
      प्रत्यागम FT_THROW( Invalid_विस्थापनम् );

    error = open_face_PS_from_sfnt_प्रवाहः( library,
                                           प्रवाहः,
                                           face_सूचकांक,
                                           0, रिक्त,
                                           aface );
    यदि ( !error )
      गच्छ Exit;

    /* rewind sfnt stream before open_face_PS_from_sfnt_stream() */
    error = FT_प्रवाहः_Seek( प्रवाहः, flag_विस्थापनम् + 4 );
    यदि ( error )
      गच्छ Exit;

    यदि ( FT_QALLOC( sfnt_दत्त, rlen ) )
      प्रत्यागम error;
    error = FT_प्रवाहः_Read( प्रवाहः, (FT_Byte *)sfnt_दत्त, rlen );
    यदि ( error )
    {
      FT_FREE( sfnt_दत्त );
      गच्छ Exit;
    }

    is_cff = rlen > 4 && !ft_memcmp( sfnt_दत्त, "OTTO", 4 );
    error = open_face_from_बफर( library,
                                   sfnt_दत्त,
                                   rlen,
                                   face_सूचकांक_अन्तर्_resource,
                                   is_cff ? "cff" : "truetype",
                                   aface );

  Exit:
    प्रत्यागम error;
  }


  /* Check for a valid resource fork header, or a valid dfont    */
  /* header.  In a resource fork the first 16 bytes are repeated */
  /* at the location specified by bytes 4-7.  In a dfont bytes   */
  /* 4-7 point to 16 bytes of zeroes instead.                    */
  /*                                                             */
  स्थिर FT_Error
  IsMacResource( FT_Library  library,
                 FT_प्रवाहः   प्रवाहः,
                 FT_Long     resource_विस्थापनम्,
                 FT_Long     face_सूचकांक,
                 FT_Face    *aface )
  {
    FT_स्मृतिः  स्मृतिः = library->स्मृतिः;
    FT_Error   error;
    FT_Long    map_विस्थापनम्, rdata_pos;
    FT_Long    *दत्त_offsets;
    FT_Long    गणनम्;


    error = FT_Raccess_Get_शीर्षकम्_Info( library, प्रवाहः, resource_विस्थापनम्,
                                       &map_विस्थापनम्, &rdata_pos );
    यदि ( error )
      प्रत्यागम error;

    /* POST resources must be sorted to concatenate properly */
    error = FT_Raccess_Get_दत्त_Offsets( library, प्रवाहः,
                                        map_विस्थापनम्, rdata_pos,
                                        TTAG_POST, TRUE,
                                        &दत्त_offsets, &गणनम् );
    यदि ( !error )
    {
      error = Mac_Read_POST_Resource( library, प्रवाहः, दत्त_offsets, गणनम्,
                                      face_सूचकांक, aface );
      FT_FREE( दत्त_offsets );
      /* POST exists in an LWFN providing a single face */
      यदि ( !error )
        (*aface)->अङ्कः_faces = 1;
      प्रत्यागम error;
    }

    /* sfnt resources should not be sorted to preserve the face order by
       QuickDraw API */
    error = FT_Raccess_Get_दत्त_Offsets( library, प्रवाहः,
                                        map_विस्थापनम्, rdata_pos,
                                        TTAG_sfnt, FALSE,
                                        &दत्त_offsets, &गणनम् );
    यदि ( !error )
    {
      FT_Long  face_सूचकांक_आन्तरिकम् = face_सूचकांक % गणनम्;


      error = Mac_Read_sfnt_Resource( library, प्रवाहः, दत्त_offsets, गणनम्,
                                      face_सूचकांक_आन्तरिकम्, aface );
      FT_FREE( दत्त_offsets );
      यदि ( !error )
      {
        (*aface)->अङ्कः_faces  = गणनम्;
        (*aface)->face_सूचकांक = face_सूचकांक_आन्तरिकम्;
      }
    }

    प्रत्यागम error;
  }


  /* Check for a valid macbinary header, and if we find one   */
  /* check that the (flattened) resource fork in it is valid. */
  /*                                                          */
  स्थिर FT_Error
  Is_Mac_द्विमान( FT_Library  library,
               FT_प्रवाहः   प्रवाहः,
               FT_Long     face_सूचकांक,
               FT_Face    *aface )
  {
    अचिह्नित वर्ण  शीर्षकम्[128];
    FT_Error       error;
    FT_Long        dlen, विस्थापनम्;


    यदि ( !प्रवाहः )
      प्रत्यागम FT_THROW( Invalid_प्रवाहः_Operation );

    error = FT_प्रवाहः_Seek( प्रवाहः, 0 );
    यदि ( error )
      गच्छ Exit;

    error = FT_प्रवाहः_Read( प्रवाहः, (FT_Byte*)शीर्षकम्, 128 );
    यदि ( error )
      गच्छ Exit;

    यदि (            शीर्षकम्[ 0] !=   0 ||
                    शीर्षकम्[74] !=   0 ||
                    शीर्षकम्[82] !=   0 ||
                    शीर्षकम्[ 1] ==   0 ||
                    शीर्षकम्[ 1] >   33 ||
                    शीर्षकम्[63] !=   0 ||
         शीर्षकम्[2 + शीर्षकम्[1]] !=   0 ||
                  शीर्षकम्[0x53] > 0x7F )
      प्रत्यागम FT_THROW( Unknown_File_Format );

    dlen = ( शीर्षकम्[0x53] << 24 ) |
           ( शीर्षकम्[0x54] << 16 ) |
           ( शीर्षकम्[0x55] <<  8 ) |
             शीर्षकम्[0x56];
#यदि 0
    rlen = ( शीर्षकम्[0x57] << 24 ) |
           ( शीर्षकम्[0x58] << 16 ) |
           ( शीर्षकम्[0x59] <<  8 ) |
             शीर्षकम्[0x5A];
#यदि_अन्तः /* 0 */
    विस्थापनम् = 128 + ( ( dlen + 127 ) & ~127 );

    प्रत्यागम IsMacResource( library, प्रवाहः, विस्थापनम्, face_सूचकांक, aface );

  Exit:
    प्रत्यागम error;
  }


  स्थिर FT_Error
  आरोपय_face_अन्तर्_embedded_rfork( FT_Library           library,
                               FT_प्रवाहः            प्रवाहः,
                               FT_Long              face_सूचकांक,
                               FT_Face             *aface,
                               ध्रुव FT_Open_तर्काः  *तर्काः )
  {

#अपरिभाषय  FT_COMPONENT
#परिभाषय FT_COMPONENT  raccess

    FT_स्मृतिः  स्मृतिः = library->स्मृतिः;
    FT_Error   error  = FT_ERR( Unknown_File_Format );
    FT_UInt    i;

    वर्ण*      file_नामानि[FT_RACCESS_N_नियमाः];
    FT_Long    offsets[FT_RACCESS_N_नियमाः];
    FT_Error   errors[FT_RACCESS_N_नियमाः];
    FT_Bool    is_darwin_vfs, vfs_rfork_has_no_font = FALSE; /* not tested */

    FT_Open_तर्काः  args2;
    FT_प्रवाहः     stream2 = रिक्त;


    FT_Raccess_Guess( library, प्रवाहः,
                      तर्काः->pathname, file_नामानि, offsets, errors );

    प्रति ( i = 0; i < FT_RACCESS_N_नियमाः; i++ )
    {
      is_darwin_vfs = ft_raccess_नियमः_by_darwin_vfs( library, i );
      यदि ( is_darwin_vfs && vfs_rfork_has_no_font )
      {
        FT_TRACE3(( "Skip rule %u: darwin vfs resource fork"
                    " is already checked and"
                    " no font is found\n",
                    i ));
        अनुवर्त;
      }

      यदि ( errors[i] )
      {
        FT_TRACE3(( "Error 0x%x has occurred in rule %u\n",
                    errors[i], i ));
        अनुवर्त;
      }

      args2.flags    = FT_OPEN_PATHNAME;
      args2.pathname = file_नामानि[i] ? file_नामानि[i] : तर्काः->pathname;

      FT_TRACE3(( "Try rule %u: %s (offset=%ld) ...",
                  i, args2.pathname, offsets[i] ));

      error = FT_प्रवाहः_New( library, &args2, &stream2 );
      यदि ( is_darwin_vfs && FT_ERR_EQ( error, Cannot_Open_प्रवाहः ) )
        vfs_rfork_has_no_font = TRUE;

      यदि ( error )
      {
        FT_TRACE3(( "failed\n" ));
        अनुवर्त;
      }

      error = IsMacResource( library, stream2, offsets[i],
                             face_सूचकांक, aface );
      FT_प्रवाहः_Free( stream2, 0 );

      FT_TRACE3(( "%s\n", error ? "failed": "successful" ));

      यदि ( !error )
          भङ्ग;
      अन्यथा यदि ( is_darwin_vfs )
          vfs_rfork_has_no_font = TRUE;
    }

    प्रति (i = 0; i < FT_RACCESS_N_नियमाः; i++)
    {
      यदि ( file_नामानि[i] )
        FT_FREE( file_नामानि[i] );
    }

    /* Caller (load_mac_face) requires FT_Err_Unknown_File_Format. */
    यदि ( error )
      error = FT_ERR( Unknown_File_Format );

    प्रत्यागम error;

#अपरिभाषय  FT_COMPONENT
#परिभाषय FT_COMPONENT  objs

  }


  /* Check for some macintosh formats without Carbon framework.    */
  /* Is this a macbinary file?  If so look at the resource fork.   */
  /* Is this a mac dfont file?                                     */
  /* Is this an old style resource fork? (in data)                 */
  /* Else call load_face_in_embedded_rfork to try extra rules      */
  /* (defined in `ftrfork.c').                                     */
  /*                                                               */
  स्थिर FT_Error
  आरोपय_mac_face( FT_Library           library,
                 FT_प्रवाहः            प्रवाहः,
                 FT_Long              face_सूचकांक,
                 FT_Face             *aface,
                 ध्रुव FT_Open_तर्काः  *तर्काः )
  {
    FT_Error error;
    FT_अप्रयुक्तम्( तर्काः );


    error = Is_Mac_द्विमान( library, प्रवाहः, face_सूचकांक, aface );
    यदि ( FT_ERR_EQ( error, Unknown_File_Format ) )
    {

#अपरिभाषय  FT_COMPONENT
#परिभाषय FT_COMPONENT  raccess

#यदि_परिभाषितम् FT_DEBUG_LEVEL_TRACE
      FT_TRACE3(( "Try as dfont: " ));
      यदि ( !( तर्काः->flags & FT_OPEN_स्मृतिः ) )
        FT_TRACE3(( "%s ...", तर्काः->pathname ));
#यदि_अन्तः

      error = IsMacResource( library, प्रवाहः, 0, face_सूचकांक, aface );

      FT_TRACE3(( "%s\n", error ? "failed" : "successful" ));

#अपरिभाषय  FT_COMPONENT
#परिभाषय FT_COMPONENT  objs

    }

    यदि ( ( FT_ERR_EQ( error, Unknown_File_Format )      ||
           FT_ERR_EQ( error, Invalid_प्रवाहः_Operation ) ) &&
         ( तर्काः->flags & FT_OPEN_PATHNAME )               )
      error = आरोपय_face_अन्तर्_embedded_rfork( library, प्रवाहः,
                                           face_सूचकांक, aface, तर्काः );
    प्रत्यागम error;
  }
#यदि_अन्तः

#यदि_अन्तः  /* !FT_MACINTOSH && FT_CONFIG_OPTION_MAC_FONTS */


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Open_Face( FT_Library           library,
                ध्रुव FT_Open_तर्काः*  तर्काः,
                FT_Long              face_सूचकांक,
                FT_Face             *aface )
  {
    प्रत्यागम ft_open_face_आन्तरिकम्( library, तर्काः, face_सूचकांक, aface, 1 );
  }


  स्थिर FT_Error
  ft_open_face_आन्तरिकम्( FT_Library           library,
                         ध्रुव FT_Open_तर्काः*  तर्काः,
                         FT_Long              face_सूचकांक,
                         FT_Face             *aface,
                         FT_Bool              test_mac_fonts )
  {
    FT_Error     error;
    FT_Driver    driver = रिक्त;
    FT_स्मृतिः    स्मृतिः = रिक्त;
    FT_प्रवाहः    प्रवाहः = रिक्त;
    FT_Face      face   = रिक्त;
    FT_List_ग्रन्थिः  ग्रन्थिः   = रिक्त;
    FT_Bool      external_प्रवाहः;
    FT_Module*   cur;
    FT_Module*   सीमा;

#यदि_अपरिभाषितम् FT_CONFIG_OPTION_MAC_FONTS
    FT_अप्रयुक्तम्( test_mac_fonts );
#यदि_अन्तः


    /* only use lower 31 bits together with sign bit */
    यदि ( face_सूचकांक > 0 )
      face_सूचकांक &= 0x7FFFFFFFL;
    अन्यथा
    {
      face_सूचकांक  = -face_सूचकांक;
      face_सूचकांक &= 0x7FFFFFFFL;
      face_सूचकांक  = -face_सूचकांक;
    }

#यदि_परिभाषितम् FT_DEBUG_LEVEL_TRACE
    FT_TRACE3(( "FT_Open_Face: " ));
    यदि ( face_सूचकांक < 0 )
      FT_TRACE3(( "Requesting number of faces and named instances\n"));
    अन्यथा
    {
      FT_TRACE3(( "Requesting face %ld", face_सूचकांक & 0xFFFFL ));
      यदि ( face_सूचकांक & 0x7FFF0000L )
        FT_TRACE3(( ", named instance %ld", face_सूचकांक >> 16 ));
      FT_TRACE3(( "\n" ));
    }
#यदि_अन्तः

    /* test for valid `library' delayed to `FT_Stream_New' */

    यदि ( !तर्काः )
      प्रत्यागम FT_THROW( Invalid_तर्कः );

    external_प्रवाहः = FT_BOOL( ( तर्काः->flags & FT_OPEN_प्रवाहः ) &&
                               तर्काः->प्रवाहः                     );

    /* create input stream */
    error = FT_प्रवाहः_New( library, तर्काः, &प्रवाहः );
    यदि ( error )
      गच्छ Fail3;

    /* Do this error check after `FT_Stream_New` to ensure that the */
    /* 'close' callback is called.                                  */
    यदि ( !aface && face_सूचकांक >= 0 )
    {
      error = FT_THROW( Invalid_तर्कः );
      गच्छ Fail3;
    }

    स्मृतिः = library->स्मृतिः;

    /* If the font driver is specified in the `args' structure, use */
    /* it.  Otherwise, we scan the list of registered drivers.      */
    यदि ( ( तर्काः->flags & FT_OPEN_DRIVER ) && तर्काः->driver )
    {
      driver = FT_DRIVER( तर्काः->driver );

      /* not all modules are drivers, so check... */
      यदि ( FT_MODULE_IS_DRIVER( driver ) )
      {
        FT_Int         अङ्कः_params = 0;
        FT_Parameter*  params     = रिक्त;


        यदि ( तर्काः->flags & FT_OPEN_PARAMS )
        {
          अङ्कः_params = तर्काः->अङ्कः_params;
          params     = तर्काः->params;
        }

        error = open_face( driver, &प्रवाहः, &external_प्रवाहः, face_सूचकांक,
                           अङ्कः_params, params, &face );
        यदि ( !error )
          गच्छ Success;
      }
      अन्यथा
        error = FT_THROW( Invalid_Handle );

      FT_प्रवाहः_Free( प्रवाहः, external_प्रवाहः );
      गच्छ Fail;
    }
    अन्यथा
    {
      error = FT_ERR( Missing_Module );

      /* check each font driver for an appropriate format */
      cur   = library->modules;
      सीमा = cur + library->अङ्कः_modules;

      प्रति ( ; cur < सीमा; cur++ )
      {
        /* not all modules are font drivers, so check... */
        यदि ( FT_MODULE_IS_DRIVER( cur[0] ) )
        {
          FT_Int         अङ्कः_params = 0;
          FT_Parameter*  params     = रिक्त;


          driver = FT_DRIVER( cur[0] );

          यदि ( तर्काः->flags & FT_OPEN_PARAMS )
          {
            अङ्कः_params = तर्काः->अङ्कः_params;
            params     = तर्काः->params;
          }

          error = open_face( driver, &प्रवाहः, &external_प्रवाहः, face_सूचकांक,
                             अङ्कः_params, params, &face );
          यदि ( !error )
            गच्छ Success;

#यदि_परिभाषितम् FT_CONFIG_OPTION_MAC_FONTS
          यदि ( test_mac_fonts                                           &&
               ft_strcmp( cur[0]->clazz->module_name, "truetype" ) == 0 &&
               FT_ERR_EQ( error, सारणी_Missing )                        )
          {
            /* TrueType but essential tables are missing */
            error = FT_प्रवाहः_Seek( प्रवाहः, 0 );
            यदि ( error )
              भङ्ग;

            error = open_face_PS_from_sfnt_प्रवाहः( library,
                                                   प्रवाहः,
                                                   face_सूचकांक,
                                                   अङ्कः_params,
                                                   params,
                                                   aface );
            यदि ( !error )
            {
              FT_प्रवाहः_Free( प्रवाहः, external_प्रवाहः );
              प्रत्यागम error;
            }
          }
#यदि_अन्तः

          यदि ( FT_ERR_NEQ( error, Unknown_File_Format ) )
            गच्छ Fail3;
        }
      }

    Fail3:
      /* If we are on the mac, and we get an                          */
      /* FT_Err_Invalid_Stream_Operation it may be because we have an */
      /* empty data fork, so we need to check the resource fork.      */
      यदि ( FT_ERR_NEQ( error, Cannot_Open_प्रवाहः )       &&
           FT_ERR_NEQ( error, Unknown_File_Format )      &&
           FT_ERR_NEQ( error, Invalid_प्रवाहः_Operation ) )
        गच्छ Fail2;

#यदि !defined( FT_MACINTOSH ) && defined( FT_CONFIG_OPTION_MAC_FONTS )
      यदि ( test_mac_fonts )
      {
        error = आरोपय_mac_face( library, प्रवाहः, face_सूचकांक, aface, तर्काः );
        यदि ( !error )
        {
          /* We don't want to go to Success here.  We've already done   */
          /* that.  On the other hand, if we succeeded we still need to */
          /* close this stream (we opened a different stream which      */
          /* extracted the interesting information out of this stream   */
          /* here.  That stream will still be open and the face will    */
          /* point to it).                                              */
          FT_प्रवाहः_Free( प्रवाहः, external_प्रवाहः );
          प्रत्यागम error;
        }
      }

      यदि ( FT_ERR_NEQ( error, Unknown_File_Format ) )
        गच्छ Fail2;
#यदि_अन्तः  /* !FT_MACINTOSH && FT_CONFIG_OPTION_MAC_FONTS */

      /* no driver is able to handle this format */
      error = FT_THROW( Unknown_File_Format );

  Fail2:
      FT_प्रवाहः_Free( प्रवाहः, external_प्रवाहः );
      गच्छ Fail;
    }

  Success:
    FT_TRACE4(( "FT_Open_Face: New face object, adding to list\n" ));

    /* add the face object to its driver's list */
    यदि ( FT_QNEW( ग्रन्थिः ) )
      गच्छ Fail;

    ग्रन्थिः->दत्त = face;
    /* don't assume driver is the same as face->driver, so use */
    /* face->driver instead.                                   */
    FT_List_सङ्कलय( &face->driver->faces_list, ग्रन्थिः );

    /* now allocate a glyph slot object for the face */
    FT_TRACE4(( "FT_Open_Face: Creating glyph slot\n" ));

    यदि ( face_सूचकांक >= 0 )
    {
      error = FT_New_GlyphSlot( face, रिक्त );
      यदि ( error )
        गच्छ Fail;

      /* finally, allocate a size object for the face */
      {
        FT_आकार  आकार;


        FT_TRACE4(( "FT_Open_Face: Creating size object\n" ));

        error = FT_New_आकार( face, &आकार );
        यदि ( error )
          गच्छ Fail;

        face->आकार = आकार;
      }
    }

    /* some checks */

    यदि ( FT_IS_SCALABLE( face ) )
    {
      यदि ( face->height < 0 )
        face->height = (FT_Short)-face->height;

      यदि ( !FT_HAS_VERTICAL( face ) )
        face->max_advance_height = (FT_Short)face->height;
    }

    यदि ( FT_HAS_FIXED_SIZES( face ) )
    {
      FT_Int  i;


      प्रति ( i = 0; i < face->अङ्कः_fixed_sizes; i++ )
      {
        FT_Bitmap_आकार*  bsize = face->उपलब्धम्_sizes + i;


        यदि ( bsize->height < 0 )
          bsize->height = -bsize->height;
        यदि ( bsize->x_ppem < 0 )
          bsize->x_ppem = -bsize->x_ppem;
        यदि ( bsize->y_ppem < 0 )
          bsize->y_ppem = -bsize->y_ppem;

        /* check whether negation actually has worked */
        यदि ( bsize->height < 0 || bsize->x_ppem < 0 || bsize->y_ppem < 0 )
        {
          FT_TRACE0(( "FT_Open_Face:"
                      " Invalid bitmap dimensions for strike %d,"
                      " now disabled\n", i ));
          bsize->विस्तारः  = 0;
          bsize->height = 0;
          bsize->आकार   = 0;
          bsize->x_ppem = 0;
          bsize->y_ppem = 0;
        }
      }
    }

    /* initialize internal face data */
    {
      FT_Face_आन्तरिकम्  आन्तरिकम् = face->आन्तरिकम्;


      आन्तरिकम्->transform_matrix.xx = 0x10000L;
      आन्तरिकम्->transform_matrix.xy = 0;
      आन्तरिकम्->transform_matrix.yx = 0;
      आन्तरिकम्->transform_matrix.yy = 0x10000L;

      आन्तरिकम्->transform_delta.x = 0;
      आन्तरिकम्->transform_delta.y = 0;

      आन्तरिकम्->refcount = 1;

      आन्तरिकम्->no_stem_darkening = -1;
    }

    यदि ( aface )
      *aface = face;
    अन्यथा
      FT_समाप्तम्_Face( face );

    गच्छ Exit;

  Fail:
    यदि ( ग्रन्थिः )
      FT_समाप्तम्_Face( face );    /* face must be in the driver's list */
    अन्यथा यदि ( face )
      destroy_face( स्मृतिः, face, driver );

  Exit:
#यदि_परिभाषितम् FT_DEBUG_LEVEL_TRACE
    यदि ( !error && face_सूचकांक < 0 )
    {
      FT_TRACE3(( "FT_Open_Face: The font has %ld face%s\n",
                  face->अङ्कः_faces,
                  face->अङ्कः_faces == 1 ? "" : "s" ));
      FT_TRACE3(( "              and %ld named instance%s for face %ld\n",
                  face->style_flags >> 16,
                  ( face->style_flags >> 16 ) == 1 ? "" : "s",
                  -face_सूचकांक - 1 ));
    }
#यदि_अन्तः

    FT_TRACE4(( "FT_Open_Face: Return 0x%x\n", error ));

    प्रत्यागम error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Attach_File( FT_Face      face,
                  ध्रुव वर्ण*  filepathname )
  {
    FT_Open_तर्काः  उद्घाटय;


    /* test for valid `face' delayed to `FT_Attach_Stream' */

    यदि ( !filepathname )
      प्रत्यागम FT_THROW( Invalid_तर्कः );

    उद्घाटय.प्रवाहः   = रिक्त;
    उद्घाटय.flags    = FT_OPEN_PATHNAME;
    उद्घाटय.pathname = (वर्ण*)filepathname;

    प्रत्यागम FT_Attach_प्रवाहः( face, &उद्घाटय );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Attach_प्रवाहः( FT_Face              face,
                    ध्रुव FT_Open_तर्काः*  parameters )
  {
    FT_प्रवाहः  प्रवाहः;
    FT_Error   error;
    FT_Driver  driver;

    FT_Driver_Class  clazz;


    /* test for valid `parameters' delayed to `FT_Stream_New' */

    यदि ( !face )
      प्रत्यागम FT_THROW( Invalid_Face_Handle );

    driver = face->driver;
    यदि ( !driver )
      प्रत्यागम FT_THROW( Invalid_Driver_Handle );

    error = FT_प्रवाहः_New( driver->मूलम्.library, parameters, &प्रवाहः );
    यदि ( error )
      गच्छ Exit;

    /* we implement FT_Attach_Stream in each driver through the */
    /* `attach_file' interface                                  */

    error = FT_ERR( Unimplemented_Feature );
    clazz = driver->clazz;
    यदि ( clazz->attach_file )
      error = clazz->attach_file( face, प्रवाहः );

    /* close the attached stream */
    FT_प्रवाहः_Free( प्रवाहः,
                    FT_BOOL( parameters->प्रवाहः                     &&
                             ( parameters->flags & FT_OPEN_प्रवाहः ) ) );

  Exit:
    प्रत्यागम error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Reference_Face( FT_Face  face )
  {
    यदि ( !face )
      प्रत्यागम FT_THROW( Invalid_Face_Handle );

    face->आन्तरिकम्->refcount++;

    प्रत्यागम FT_Err_Ok;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_समाप्तम्_Face( FT_Face  face )
  {
    FT_Error     error;
    FT_Driver    driver;
    FT_स्मृतिः    स्मृतिः;
    FT_List_ग्रन्थिः  ग्रन्थिः;


    error = FT_ERR( Invalid_Face_Handle );
    यदि ( face && face->driver )
    {
      face->आन्तरिकम्->refcount--;
      यदि ( face->आन्तरिकम्->refcount > 0 )
        error = FT_Err_Ok;
      अन्यथा
      {
        driver = face->driver;
        स्मृतिः = driver->मूलम्.स्मृतिः;

        /* find face in driver's list */
        ग्रन्थिः = FT_List_Find( &driver->faces_list, face );
        यदि ( ग्रन्थिः )
        {
          /* remove face object from the driver's list */
          FT_List_Remove( &driver->faces_list, ग्रन्थिः );
          FT_FREE( ग्रन्थिः );

          /* now destroy the object proper */
          destroy_face( स्मृतिः, face, driver );
          error = FT_Err_Ok;
        }
      }
    }

    प्रत्यागम error;
  }


  /* documentation is in ftobjs.h */

  FT_EXPORT_DEF( FT_Error )
  FT_New_आकार( FT_Face   face,
               FT_आकार  *asize )
  {
    FT_Error         error;
    FT_स्मृतिः        स्मृतिः;
    FT_Driver        driver;
    FT_Driver_Class  clazz;

    FT_आकार          आकार = रिक्त;
    FT_List_ग्रन्थिः      ग्रन्थिः = रिक्त;

    FT_आकार_आन्तरिकम्  आन्तरिकम् = रिक्त;


    यदि ( !face )
      प्रत्यागम FT_THROW( Invalid_Face_Handle );

    यदि ( !asize )
      प्रत्यागम FT_THROW( Invalid_तर्कः );

    यदि ( !face->driver )
      प्रत्यागम FT_THROW( Invalid_Driver_Handle );

    *asize = रिक्त;

    driver = face->driver;
    clazz  = driver->clazz;
    स्मृतिः = face->स्मृतिः;

    /* Allocate new size object and perform basic initialisation */
    यदि ( FT_ALLOC( आकार, clazz->आकार_object_आकार ) || FT_QNEW( ग्रन्थिः ) )
      गच्छ Exit;

    आकार->face = face;

    यदि ( FT_NEW( आन्तरिकम् ) )
      गच्छ Exit;

    आकार->आन्तरिकम् = आन्तरिकम्;

    यदि ( clazz->init_आकार )
      error = clazz->init_आकार( आकार );

    /* in case of success, add to the face's list */
    यदि ( !error )
    {
      *asize     = आकार;
      ग्रन्थिः->दत्त = आकार;
      FT_List_सङ्कलय( &face->sizes_list, ग्रन्थिः );
    }

  Exit:
    यदि ( error )
    {
      FT_FREE( ग्रन्थिः );
      यदि ( आकार )
        FT_FREE( आकार->आन्तरिकम् );
      FT_FREE( आकार );
    }

    प्रत्यागम error;
  }


  /* documentation is in ftobjs.h */

  FT_EXPORT_DEF( FT_Error )
  FT_समाप्तम्_आकार( FT_आकार  आकार )
  {
    FT_Error     error;
    FT_Driver    driver;
    FT_स्मृतिः    स्मृतिः;
    FT_Face      face;
    FT_List_ग्रन्थिः  ग्रन्थिः;


    यदि ( !आकार )
      प्रत्यागम FT_THROW( Invalid_आकार_Handle );

    face = आकार->face;
    यदि ( !face )
      प्रत्यागम FT_THROW( Invalid_Face_Handle );

    driver = face->driver;
    यदि ( !driver )
      प्रत्यागम FT_THROW( Invalid_Driver_Handle );

    स्मृतिः = driver->मूलम्.स्मृतिः;

    error = FT_Err_Ok;
    ग्रन्थिः  = FT_List_Find( &face->sizes_list, आकार );
    यदि ( ग्रन्थिः )
    {
      FT_List_Remove( &face->sizes_list, ग्रन्थिः );
      FT_FREE( ग्रन्थिः );

      यदि ( face->आकार == आकार )
      {
        face->आकार = रिक्त;
        यदि ( face->sizes_list.शीर्ष )
          face->आकार = (FT_आकार)(face->sizes_list.शीर्ष->दत्त);
      }

      destroy_आकार( स्मृतिः, आकार, driver );
    }
    अन्यथा
      error = FT_THROW( Invalid_आकार_Handle );

    प्रत्यागम error;
  }


  /* documentation is in ftobjs.h */

  FT_आधारः_DEF( FT_Error )
  FT_मेलनम्_आकार( FT_Face          face,
                 FT_आकार_Request  req,
                 FT_Bool          ignore_विस्तारः,
                 FT_ULong*        आकार_सूचकांक )
  {
    FT_Int   i;
    FT_Long  w, h;


    यदि ( !FT_HAS_FIXED_SIZES( face ) )
      प्रत्यागम FT_THROW( Invalid_Face_Handle );

    /* FT_Bitmap_Size doesn't provide enough info... */
    यदि ( req->type != FT_आकार_REQUEST_TYPE_NOMINAL )
      प्रत्यागम FT_THROW( Unimplemented_Feature );

    w = FT_REQUEST_विस्तारः ( req );
    h = FT_REQUEST_HEIGHT( req );

    यदि ( req->विस्तारः && !req->height )
      h = w;
    अन्यथा यदि ( !req->विस्तारः && req->height )
      w = h;

    w = FT_PIX_आवर्तनम्( w );
    h = FT_PIX_आवर्तनम्( h );

    यदि ( !w || !h )
      प्रत्यागम FT_THROW( Invalid_Pixel_आकार );

    प्रति ( i = 0; i < face->अङ्कः_fixed_sizes; i++ )
    {
      FT_Bitmap_आकार*  bsize = face->उपलब्धम्_sizes + i;


      यदि ( h != FT_PIX_आवर्तनम्( bsize->y_ppem ) )
        अनुवर्त;

      यदि ( w == FT_PIX_आवर्तनम्( bsize->x_ppem ) || ignore_विस्तारः )
      {
        FT_TRACE3(( "FT_Match_Size: bitmap strike %d matches\n", i ));

        यदि ( आकार_सूचकांक )
          *आकार_सूचकांक = (FT_ULong)i;

        प्रत्यागम FT_Err_Ok;
      }
    }

    FT_TRACE3(( "FT_Match_Size: no matching bitmap strike\n" ));

    प्रत्यागम FT_THROW( Invalid_Pixel_आकार );
  }


  /* documentation is in ftobjs.h */

  FT_आधारः_DEF( शून्य )
  ft_synthesize_vertical_metrics( FT_Glyph_Metrics*  metrics,
                                  FT_Pos             advance )
  {
    FT_Pos  height = metrics->height;


    /* compensate for glyph with bbox above/below the baseline */
    यदि ( metrics->horiBearingY < 0 )
    {
      यदि ( height < metrics->horiBearingY )
        height = metrics->horiBearingY;
    }
    अन्यथा यदि ( metrics->horiBearingY > 0 )
      height -= metrics->horiBearingY;

    /* the factor 1.2 is a heuristical value */
    यदि ( !advance )
      advance = height * 12 / 10;

    metrics->vertBearingX = metrics->horiBearingX - metrics->horiAdvance / 2;
    metrics->vertBearingY = ( advance - height ) / 2;
    metrics->vertAdvance  = advance;
  }


  स्थिर शून्य
  ft_recompute_scaled_metrics( FT_Face           face,
                               FT_आकार_Metrics*  metrics )
  {
    /* Compute root ascender, descender, test height, and max_advance */

#यदि_परिभाषितम् GRID_FIT_METRICS
    metrics->ascender    = FT_PIX_CEIL( FT_MulFix( face->ascender,
                                                   metrics->y_scale ) );

    metrics->descender   = FT_PIX_FLOOR( FT_MulFix( face->descender,
                                                    metrics->y_scale ) );

    metrics->height      = FT_PIX_आवर्तनम्( FT_MulFix( face->height,
                                                    metrics->y_scale ) );

    metrics->max_advance = FT_PIX_आवर्तनम्( FT_MulFix( face->max_advance_विस्तारः,
                                                    metrics->x_scale ) );
#अन्यथा /* !GRID_FIT_METRICS */
    metrics->ascender    = FT_MulFix( face->ascender,
                                      metrics->y_scale );

    metrics->descender   = FT_MulFix( face->descender,
                                      metrics->y_scale );

    metrics->height      = FT_MulFix( face->height,
                                      metrics->y_scale );

    metrics->max_advance = FT_MulFix( face->max_advance_विस्तारः,
                                      metrics->x_scale );
#यदि_अन्तः /* !GRID_FIT_METRICS */
  }


  FT_आधारः_DEF( शून्य )
  FT_Select_Metrics( FT_Face   face,
                     FT_ULong  strike_सूचकांक )
  {
    FT_आकार_Metrics*  metrics;
    FT_Bitmap_आकार*   bsize;


    metrics = &face->आकार->metrics;
    bsize   = face->उपलब्धम्_sizes + strike_सूचकांक;

    metrics->x_ppem = (FT_UShort)( ( bsize->x_ppem + 32 ) >> 6 );
    metrics->y_ppem = (FT_UShort)( ( bsize->y_ppem + 32 ) >> 6 );

    यदि ( FT_IS_SCALABLE( face ) )
    {
      metrics->x_scale = FT_DivFix( bsize->x_ppem,
                                    face->units_प्रत्येकम्_EM );
      metrics->y_scale = FT_DivFix( bsize->y_ppem,
                                    face->units_प्रत्येकम्_EM );

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


  FT_आधारः_DEF( FT_Error )
  FT_Request_Metrics( FT_Face          face,
                      FT_आकार_Request  req )
  {
    FT_Error  error = FT_Err_Ok;

    FT_आकार_Metrics*  metrics;


    metrics = &face->आकार->metrics;

    यदि ( FT_IS_SCALABLE( face ) )
    {
      FT_Long  w = 0, h = 0, scaled_w = 0, scaled_h = 0;


      विकल्प ( req->type )
      {
      पक्ष FT_आकार_REQUEST_TYPE_NOMINAL:
        w = h = face->units_प्रत्येकम्_EM;
        भङ्ग;

      पक्ष FT_आकार_REQUEST_TYPE_REAL_DIM:
        w = h = face->ascender - face->descender;
        भङ्ग;

      पक्ष FT_आकार_REQUEST_TYPE_BBOX:
        w = face->bbox.xMax - face->bbox.xMin;
        h = face->bbox.yMax - face->bbox.yMin;
        भङ्ग;

      पक्ष FT_आकार_REQUEST_TYPE_CELL:
        w = face->max_advance_विस्तारः;
        h = face->ascender - face->descender;
        भङ्ग;

      पक्ष FT_आकार_REQUEST_TYPE_SCALES:
        metrics->x_scale = (FT_Fixed)req->विस्तारः;
        metrics->y_scale = (FT_Fixed)req->height;
        यदि ( !metrics->x_scale )
          metrics->x_scale = metrics->y_scale;
        अन्यथा यदि ( !metrics->y_scale )
          metrics->y_scale = metrics->x_scale;
        गच्छ Calculate_Ppem;

      पक्ष FT_आकार_REQUEST_TYPE_MAX:
        भङ्ग;
      }

      /* to be on the safe side */
      यदि ( w < 0 )
        w = -w;

      यदि ( h < 0 )
        h = -h;

      scaled_w = FT_REQUEST_विस्तारः ( req );
      scaled_h = FT_REQUEST_HEIGHT( req );

      /* determine scales */
      यदि ( req->height || !req->विस्तारः )
      {
        यदि ( h == 0 )
        {
          FT_ERROR(( "FT_Request_Metrics: Divide by zero\n" ));
          error = FT_ERR( Divide_By_Zero );
          गच्छ Exit;
        }

        metrics->y_scale = FT_DivFix( scaled_h, h );
      }

      यदि ( req->विस्तारः )
      {
        यदि ( w == 0 )
        {
          FT_ERROR(( "FT_Request_Metrics: Divide by zero\n" ));
          error = FT_ERR( Divide_By_Zero );
          गच्छ Exit;
        }

        metrics->x_scale = FT_DivFix( scaled_w, w );
      }
      अन्यथा
      {
        metrics->x_scale = metrics->y_scale;
        scaled_w = FT_MulDiv( scaled_h, w, h );
      }

      यदि ( !req->height )
      {
        metrics->y_scale = metrics->x_scale;
        scaled_h = FT_MulDiv( scaled_w, h, w );
      }

      यदि ( req->type == FT_आकार_REQUEST_TYPE_CELL )
      {
        यदि ( metrics->y_scale > metrics->x_scale )
          metrics->y_scale = metrics->x_scale;
        अन्यथा
          metrics->x_scale = metrics->y_scale;
      }

  Calculate_Ppem:
      /* calculate the ppems */
      यदि ( req->type != FT_आकार_REQUEST_TYPE_NOMINAL )
      {
        scaled_w = FT_MulFix( face->units_प्रत्येकम्_EM, metrics->x_scale );
        scaled_h = FT_MulFix( face->units_प्रत्येकम्_EM, metrics->y_scale );
      }

      scaled_w = ( scaled_w + 32 ) >> 6;
      scaled_h = ( scaled_h + 32 ) >> 6;
      यदि ( scaled_w > (FT_Long)FT_USHORT_MAX ||
           scaled_h > (FT_Long)FT_USHORT_MAX )
      {
        FT_ERROR(( "FT_Request_Metrics: Resulting ppem size too large\n" ));
        error = FT_ERR( Invalid_Pixel_आकार );
        गच्छ Exit;
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
    प्रत्यागम error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Select_आकार( FT_Face  face,
                  FT_Int   strike_सूचकांक )
  {
    FT_Error         error = FT_Err_Ok;
    FT_Driver_Class  clazz;


    यदि ( !face || !FT_HAS_FIXED_SIZES( face ) )
      प्रत्यागम FT_THROW( Invalid_Face_Handle );

    यदि ( strike_सूचकांक < 0 || strike_सूचकांक >= face->अङ्कः_fixed_sizes )
      प्रत्यागम FT_THROW( Invalid_तर्कः );

    clazz = face->driver->clazz;

    यदि ( clazz->select_आकार )
    {
      error = clazz->select_आकार( face->आकार, (FT_ULong)strike_सूचकांक );

      FT_TRACE5(( "FT_Select_Size (%s driver):\n",
                  face->driver->मूलम्.clazz->module_name ));
    }
    अन्यथा
    {
      FT_Select_Metrics( face, (FT_ULong)strike_सूचकांक );

      FT_TRACE5(( "FT_Select_Size:\n" ));
    }

#यदि_परिभाषितम् FT_DEBUG_LEVEL_TRACE
    {
      FT_आकार_Metrics*  metrics = &face->आकार->metrics;


      FT_TRACE5(( "  x scale: %ld (%f)\n",
                  metrics->x_scale, (द्विगुण)metrics->x_scale / 65536 ));
      FT_TRACE5(( "  y scale: %ld (%f)\n",
                  metrics->y_scale, (द्विगुण)metrics->y_scale / 65536 ));
      FT_TRACE5(( "  ascender: %f\n",
                  (द्विगुण)metrics->ascender / 64 ));
      FT_TRACE5(( "  descender: %f\n",
                  (द्विगुण)metrics->descender / 64 ));
      FT_TRACE5(( "  height: %f\n",
                  (द्विगुण)metrics->height / 64 ));
      FT_TRACE5(( "  max advance: %f\n",
                  (द्विगुण)metrics->max_advance / 64 ));
      FT_TRACE5(( "  x ppem: %d\n", metrics->x_ppem ));
      FT_TRACE5(( "  y ppem: %d\n", metrics->y_ppem ));
    }
#यदि_अन्तः

    प्रत्यागम error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Request_आकार( FT_Face          face,
                   FT_आकार_Request  req )
  {
    FT_Error         error;
    FT_Driver_Class  clazz;
    FT_ULong         strike_सूचकांक;


    यदि ( !face )
      प्रत्यागम FT_THROW( Invalid_Face_Handle );

    यदि ( !face->आकार )
      प्रत्यागम FT_THROW( Invalid_आकार_Handle );

    यदि ( !req || req->विस्तारः < 0 || req->height < 0 ||
         req->type >= FT_आकार_REQUEST_TYPE_MAX )
      प्रत्यागम FT_THROW( Invalid_तर्कः );

    /* signal the auto-hinter to recompute its size metrics */
    /* (if requested)                                       */
    face->आकार->आन्तरिकम्->autohint_metrics.x_scale = 0;

    clazz = face->driver->clazz;

    यदि ( clazz->request_आकार )
    {
      error = clazz->request_आकार( face->आकार, req );

      FT_TRACE5(( "FT_Request_Size (%s driver):\n",
                  face->driver->मूलम्.clazz->module_name ));
    }
    अन्यथा यदि ( !FT_IS_SCALABLE( face ) && FT_HAS_FIXED_SIZES( face ) )
    {
      /*
       * The reason that a driver doesn't have `request_size' defined is
       * either that the scaling here suffices or that the supported formats
       * are bitmap-only and size matching is not implemented.
       *
       * In the latter case, a simple size matching is done.
       */
      error = FT_मेलनम्_आकार( face, req, 0, &strike_सूचकांक );
      यदि ( error )
        गच्छ Exit;

      प्रत्यागम FT_Select_आकार( face, (FT_Int)strike_सूचकांक );
    }
    अन्यथा
    {
      error = FT_Request_Metrics( face, req );
      यदि ( error )
        गच्छ Exit;

      FT_TRACE5(( "FT_Request_Size:\n" ));
    }

#यदि_परिभाषितम् FT_DEBUG_LEVEL_TRACE
    {
      FT_आकार_Metrics*  metrics = &face->आकार->metrics;


      FT_TRACE5(( "  x scale: %ld (%f)\n",
                  metrics->x_scale, (द्विगुण)metrics->x_scale / 65536 ));
      FT_TRACE5(( "  y scale: %ld (%f)\n",
                  metrics->y_scale, (द्विगुण)metrics->y_scale / 65536 ));
      FT_TRACE5(( "  ascender: %f\n",
                  (द्विगुण)metrics->ascender / 64 ));
      FT_TRACE5(( "  descender: %f\n",
                  (द्विगुण)metrics->descender / 64 ));
      FT_TRACE5(( "  height: %f\n",
                  (द्विगुण)metrics->height / 64 ));
      FT_TRACE5(( "  max advance: %f\n",
                  (द्विगुण)metrics->max_advance / 64 ));
      FT_TRACE5(( "  x ppem: %d\n", metrics->x_ppem ));
      FT_TRACE5(( "  y ppem: %d\n", metrics->y_ppem ));
    }
#यदि_अन्तः

  Exit:
    प्रत्यागम error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_समुच्चयः_Char_आकार( FT_Face     face,
                    FT_F26Dot6  char_विस्तारः,
                    FT_F26Dot6  char_height,
                    FT_UInt     horz_resolution,
                    FT_UInt     vert_resolution )
  {
    FT_आकार_Request_Rec  req;


    /* check of `face' delayed to `FT_Request_Size' */

    यदि ( !char_विस्तारः )
      char_विस्तारः = char_height;
    अन्यथा यदि ( !char_height )
      char_height = char_विस्तारः;

    यदि ( !horz_resolution )
      horz_resolution = vert_resolution;
    अन्यथा यदि ( !vert_resolution )
      vert_resolution = horz_resolution;

    यदि ( char_विस्तारः  < 1 * 64 )
      char_विस्तारः  = 1 * 64;
    यदि ( char_height < 1 * 64 )
      char_height = 1 * 64;

    यदि ( !horz_resolution )
      horz_resolution = vert_resolution = 72;

    req.type           = FT_आकार_REQUEST_TYPE_NOMINAL;
    req.विस्तारः          = char_विस्तारः;
    req.height         = char_height;
    req.horiResolution = horz_resolution;
    req.vertResolution = vert_resolution;

    प्रत्यागम FT_Request_आकार( face, &req );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_समुच्चयः_Pixel_Sizes( FT_Face  face,
                      FT_UInt  pixel_विस्तारः,
                      FT_UInt  pixel_height )
  {
    FT_आकार_Request_Rec  req;


    /* check of `face' delayed to `FT_Request_Size' */

    यदि ( pixel_विस्तारः == 0 )
      pixel_विस्तारः = pixel_height;
    अन्यथा यदि ( pixel_height == 0 )
      pixel_height = pixel_विस्तारः;

    यदि ( pixel_विस्तारः  < 1 )
      pixel_विस्तारः  = 1;
    यदि ( pixel_height < 1 )
      pixel_height = 1;

    /* use `>=' to avoid potential compiler warning on 16bit platforms */
    यदि ( pixel_विस्तारः >= 0xFFFFU )
      pixel_विस्तारः = 0xFFFFU;
    यदि ( pixel_height >= 0xFFFFU )
      pixel_height = 0xFFFFU;

    req.type           = FT_आकार_REQUEST_TYPE_NOMINAL;
    req.विस्तारः          = (FT_Long)( pixel_विस्तारः << 6 );
    req.height         = (FT_Long)( pixel_height << 6 );
    req.horiResolution = 0;
    req.vertResolution = 0;

    प्रत्यागम FT_Request_आकार( face, &req );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Kerning( FT_Face     face,
                  FT_UInt     वामः_glyph,
                  FT_UInt     दक्षिणः_glyph,
                  FT_UInt     kern_बहुलकम्,
                  FT_सदिशम्  *akerning )
  {
    FT_Error   error = FT_Err_Ok;
    FT_Driver  driver;


    यदि ( !face )
      प्रत्यागम FT_THROW( Invalid_Face_Handle );

    यदि ( !akerning )
      प्रत्यागम FT_THROW( Invalid_तर्कः );

    driver = face->driver;

    akerning->x = 0;
    akerning->y = 0;

    यदि ( driver->clazz->get_kerning )
    {
      error = driver->clazz->get_kerning( face,
                                          वामः_glyph,
                                          दक्षिणः_glyph,
                                          akerning );
      यदि ( !error )
      {
        यदि ( kern_बहुलकम् != FT_KERNING_UNSCALED )
        {
          akerning->x = FT_MulFix( akerning->x, face->आकार->metrics.x_scale );
          akerning->y = FT_MulFix( akerning->y, face->आकार->metrics.y_scale );

          यदि ( kern_बहुलकम् != FT_KERNING_UNFITTED )
          {
            FT_Pos  orig_x = akerning->x;
            FT_Pos  orig_y = akerning->y;


            /* we scale down kerning values for small ppem values */
            /* to avoid that rounding makes them too big.         */
            /* `25' has been determined heuristically.            */
            यदि ( face->आकार->metrics.x_ppem < 25 )
              akerning->x = FT_MulDiv( orig_x,
                                       face->आकार->metrics.x_ppem, 25 );
            यदि ( face->आकार->metrics.y_ppem < 25 )
              akerning->y = FT_MulDiv( orig_y,
                                       face->आकार->metrics.y_ppem, 25 );

            akerning->x = FT_PIX_आवर्तनम्( akerning->x );
            akerning->y = FT_PIX_आवर्तनम्( akerning->y );

#यदि_परिभाषितम् FT_DEBUG_LEVEL_TRACE
            {
              FT_Pos  orig_x_rounded = FT_PIX_आवर्तनम्( orig_x );
              FT_Pos  orig_y_rounded = FT_PIX_आवर्तनम्( orig_y );


              यदि ( akerning->x != orig_x_rounded ||
                   akerning->y != orig_y_rounded )
                FT_TRACE5(( "FT_Get_Kerning: horizontal kerning"
                            " (%ld, %ld) scaled down to (%ld, %ld) pixels\n",
                            orig_x_rounded / 64, orig_y_rounded / 64,
                            akerning->x / 64, akerning->y / 64 ));
            }
#यदि_अन्तः
          }
        }
      }
    }

    प्रत्यागम error;
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


    यदि ( !face )
      प्रत्यागम FT_THROW( Invalid_Face_Handle );

    यदि ( !akerning )
      प्रत्यागम FT_THROW( Invalid_तर्कः );

    FT_FACE_FIND_SERVICE( face, service, KERNING );
    यदि ( !service )
      प्रत्यागम FT_THROW( Unimplemented_Feature );

    error = service->get_track( face,
                                point_आकार,
                                degree,
                                akerning );

    प्रत्यागम error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Select_Charmap( FT_Face      face,
                     FT_Encoding  encoding )
  {
    FT_CharMap*  cur;
    FT_CharMap*  सीमा;


    यदि ( !face )
      प्रत्यागम FT_THROW( Invalid_Face_Handle );

    /* FT_ENCODING_NONE is a valid encoding for BDF, PCF, and Windows FNT */
    यदि ( encoding == FT_ENCODING_NONE && !face->अङ्कः_charmaps )
      प्रत्यागम FT_THROW( Invalid_तर्कः );

    /* FT_ENCODING_UNICODE is special.  We try to find the `best' Unicode */
    /* charmap available, i.e., one with UCS-4 characters, if possible.   */
    /*                                                                    */
    /* This is done by find_unicode_charmap() above, to share code.       */
    यदि ( encoding == FT_ENCODING_UNICODE )
      प्रत्यागम find_unicode_charmap( face );

    cur = face->charmaps;
    यदि ( !cur )
      प्रत्यागम FT_THROW( Invalid_CharMap_Handle );

    सीमा = cur + face->अङ्कः_charmaps;

    प्रति ( ; cur < सीमा; cur++ )
    {
      यदि ( cur[0]->encoding == encoding )
      {
        face->charmap = cur[0];
        प्रत्यागम FT_Err_Ok;
      }
    }

    प्रत्यागम FT_THROW( Invalid_तर्कः );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_समुच्चयः_Charmap( FT_Face     face,
                  FT_CharMap  charmap )
  {
    FT_CharMap*  cur;
    FT_CharMap*  सीमा;


    यदि ( !face )
      प्रत्यागम FT_THROW( Invalid_Face_Handle );

    cur = face->charmaps;
    यदि ( !cur || !charmap )
      प्रत्यागम FT_THROW( Invalid_CharMap_Handle );

    सीमा = cur + face->अङ्कः_charmaps;

    प्रति ( ; cur < सीमा; cur++ )
    {
      यदि ( cur[0] == charmap )
      {
        FT_Long  format = FT_Get_CMap_Format( charmap );


        यदि ( format != 14 && format != 15 )
        {
          face->charmap = cur[0];
          प्रत्यागम FT_Err_Ok;
        }
      }
    }

    प्रत्यागम FT_THROW( Invalid_तर्कः );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Int )
  FT_Get_Charmap_सूचकांक( FT_CharMap  charmap )
  {
    FT_Int  i;


    यदि ( !charmap || !charmap->face )
      प्रत्यागम -1;

    प्रति ( i = 0; i < charmap->face->अङ्कः_charmaps; i++ )
      यदि ( charmap->face->charmaps[i] == charmap )
        भङ्ग;

    FT_ASSERT( i < charmap->face->अङ्कः_charmaps );

    प्रत्यागम i;
  }


  स्थिर शून्य
  ft_cmap_समाप्तम्_आन्तरिकम्( FT_CMap  cmap )
  {
    FT_CMap_Class  clazz  = cmap->clazz;
    FT_Face        face   = cmap->charmap.face;
    FT_स्मृतिः      स्मृतिः = FT_FACE_स्मृतिः( face );


    यदि ( clazz->समाप्तम् )
      clazz->समाप्तम्( cmap );

    FT_FREE( cmap );
  }


  FT_आधारः_DEF( शून्य )
  FT_CMap_समाप्तम्( FT_CMap  cmap )
  {
    यदि ( cmap )
    {
      FT_Face    face   = cmap->charmap.face;
      FT_स्मृतिः  स्मृतिः = FT_FACE_स्मृतिः( face );
      FT_Error   error;
      FT_Int     i, j;


      प्रति ( i = 0; i < face->अङ्कः_charmaps; i++ )
      {
        यदि ( (FT_CMap)face->charmaps[i] == cmap )
        {
          FT_CharMap  चरमः_charmap = face->charmaps[face->अङ्कः_charmaps - 1];


          यदि ( FT_QRENEW_ARRAY( face->charmaps,
                                face->अङ्कः_charmaps,
                                face->अङ्कः_charmaps - 1 ) )
            प्रत्यागम;

          /* remove it from our list of charmaps */
          प्रति ( j = i + 1; j < face->अङ्कः_charmaps; j++ )
          {
            यदि ( j == face->अङ्कः_charmaps - 1 )
              face->charmaps[j - 1] = चरमः_charmap;
            अन्यथा
              face->charmaps[j - 1] = face->charmaps[j];
          }

          face->अङ्कः_charmaps--;

          यदि ( (FT_CMap)face->charmap == cmap )
            face->charmap = रिक्त;

          ft_cmap_समाप्तम्_आन्तरिकम्( cmap );

          भङ्ग;
        }
      }
    }
  }


  FT_आधारः_DEF( FT_Error )
  FT_CMap_New( FT_CMap_Class  clazz,
               FT_Pointer     init_दत्त,
               FT_CharMap     charmap,
               FT_CMap       *acmap )
  {
    FT_Error   error;
    FT_Face    face;
    FT_स्मृतिः  स्मृतिः;
    FT_CMap    cmap = रिक्त;


    यदि ( !clazz || !charmap || !charmap->face )
      प्रत्यागम FT_THROW( Invalid_तर्कः );

    face   = charmap->face;
    स्मृतिः = FT_FACE_स्मृतिः( face );

    यदि ( !FT_ALLOC( cmap, clazz->आकार ) )
    {
      cmap->charmap = *charmap;
      cmap->clazz   = clazz;

      यदि ( clazz->init )
      {
        error = clazz->init( cmap, init_दत्त );
        यदि ( error )
          गच्छ Fail;
      }

      /* add it to our list of charmaps */
      यदि ( FT_QRENEW_ARRAY( face->charmaps,
                            face->अङ्कः_charmaps,
                            face->अङ्कः_charmaps + 1 ) )
        गच्छ Fail;

      face->charmaps[face->अङ्कः_charmaps++] = (FT_CharMap)cmap;
    }

  Exit:
    यदि ( acmap )
      *acmap = cmap;

    प्रत्यागम error;

  Fail:
    ft_cmap_समाप्तम्_आन्तरिकम्( cmap );
    cmap = रिक्त;
    गच्छ Exit;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt )
  FT_Get_Char_सूचकांक( FT_Face   face,
                     FT_ULong  charcode )
  {
    FT_UInt  परिणाम = 0;


    यदि ( face && face->charmap )
    {
      FT_CMap  cmap = FT_CMAP( face->charmap );


      यदि ( charcode > 0xFFFFFFFFUL )
      {
        FT_TRACE1(( "FT_Get_Char_Index: too large charcode" ));
        FT_TRACE1(( " 0x%lx is truncated\n", charcode ));
      }

      परिणाम = cmap->clazz->char_सूचकांक( cmap, (FT_UInt32)charcode );
      यदि ( परिणाम >= (FT_UInt)face->अङ्कः_glyphs )
        परिणाम = 0;
    }

    प्रत्यागम परिणाम;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_ULong )
  FT_Get_First_Char( FT_Face   face,
                     FT_UInt  *agindex )
  {
    FT_ULong  परिणाम = 0;
    FT_UInt   gindex = 0;


    /* only do something if we have a charmap, and we have glyphs at all */
    यदि ( face && face->charmap && face->अङ्कः_glyphs )
    {
      gindex = FT_Get_Char_सूचकांक( face, 0 );
      यदि ( gindex == 0 )
        परिणाम = FT_Get_अग्र_Char( face, 0, &gindex );
    }

    यदि ( agindex )
      *agindex = gindex;

    प्रत्यागम परिणाम;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_ULong )
  FT_Get_अग्र_Char( FT_Face   face,
                    FT_ULong  charcode,
                    FT_UInt  *agindex )
  {
    FT_ULong  परिणाम = 0;
    FT_UInt   gindex = 0;


    यदि ( face && face->charmap && face->अङ्कः_glyphs )
    {
      FT_UInt32  संहिता = (FT_UInt32)charcode;
      FT_CMap    cmap = FT_CMAP( face->charmap );


      कुरु
      {
        gindex = cmap->clazz->char_अग्र( cmap, &संहिता );

      } यावत् ( gindex >= (FT_UInt)face->अङ्कः_glyphs );

      परिणाम = ( gindex == 0 ) ? 0 : संहिता;
    }

    यदि ( agindex )
      *agindex = gindex;

    प्रत्यागम परिणाम;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Face_Properties( FT_Face        face,
                      FT_UInt        अङ्कः_properties,
                      FT_Parameter*  properties )
  {
    FT_Error  error = FT_Err_Ok;


    यदि ( अङ्कः_properties > 0 && !properties )
    {
      error = FT_THROW( Invalid_तर्कः );
      गच्छ Exit;
    }

    प्रति ( ; अङ्कः_properties > 0; अङ्कः_properties-- )
    {
      यदि ( properties->चिह्नकम् == FT_PARAM_चिह्नकम्_STEM_DARKENING )
      {
        यदि ( properties->दत्त )
        {
          यदि ( *( (FT_Bool*)properties->दत्त ) == TRUE )
            face->आन्तरिकम्->no_stem_darkening = FALSE;
          अन्यथा
            face->आन्तरिकम्->no_stem_darkening = TRUE;
        }
        अन्यथा
        {
          /* use module default */
          face->आन्तरिकम्->no_stem_darkening = -1;
        }
      }
      अन्यथा यदि ( properties->चिह्नकम् == FT_PARAM_चिह्नकम्_LCD_FILTER_WEIGHTS )
      {
        error = FT_THROW( Unimplemented_Feature );
        गच्छ Exit;
      }
      अन्यथा यदि ( properties->चिह्नकम् == FT_PARAM_चिह्नकम्_RANDOM_बीजम् )
      {
        यदि ( properties->दत्त )
        {
          face->आन्तरिकम्->random_बीजम् = *( (FT_Int32*)properties->दत्त );
          यदि ( face->आन्तरिकम्->random_बीजम् < 0 )
            face->आन्तरिकम्->random_बीजम् = 0;
        }
        अन्यथा
        {
          /* use module default */
          face->आन्तरिकम्->random_बीजम् = -1;
        }
      }
      अन्यथा
      {
        error = FT_THROW( Invalid_तर्कः );
        गच्छ Exit;
      }

      यदि ( error )
        भङ्ग;

      properties++;
    }

  Exit:
    प्रत्यागम error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt )
  FT_Face_Get_Char_Variant_सूचकांक( FT_Face   face,
                               FT_ULong  charcode,
                               FT_ULong  variantSelector )
  {
    FT_UInt  परिणाम = 0;


    यदि ( face                                           &&
         face->charmap                                  &&
         face->charmap->encoding == FT_ENCODING_UNICODE )
    {
      FT_CharMap  charmap = find_variant_selector_charmap( face );
      FT_CMap     ucmap = FT_CMAP( face->charmap );


      यदि ( charmap )
      {
        FT_CMap  vcmap = FT_CMAP( charmap );


        यदि ( charcode > 0xFFFFFFFFUL )
        {
          FT_TRACE1(( "FT_Face_GetCharVariantIndex:"
                      " too large charcode" ));
          FT_TRACE1(( " 0x%lx is truncated\n", charcode ));
        }
        यदि ( variantSelector > 0xFFFFFFFFUL )
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

    प्रत्यागम परिणाम;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Int )
  FT_Face_GetCharVariantIsDefault( FT_Face   face,
                                   FT_ULong  charcode,
                                   FT_ULong  variantSelector )
  {
    FT_Int  परिणाम = -1;


    यदि ( face )
    {
      FT_CharMap  charmap = find_variant_selector_charmap( face );


      यदि ( charmap )
      {
        FT_CMap  vcmap = FT_CMAP( charmap );


        यदि ( charcode > 0xFFFFFFFFUL )
        {
          FT_TRACE1(( "FT_Face_GetCharVariantIsDefault:"
                      " too large charcode" ));
          FT_TRACE1(( " 0x%lx is truncated\n", charcode ));
        }
        यदि ( variantSelector > 0xFFFFFFFFUL )
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

    प्रत्यागम परिणाम;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt32* )
  FT_Face_GetVariantSelectors( FT_Face  face )
  {
    FT_UInt32  *परिणाम = रिक्त;


    यदि ( face )
    {
      FT_CharMap  charmap = find_variant_selector_charmap( face );


      यदि ( charmap )
      {
        FT_CMap    vcmap  = FT_CMAP( charmap );
        FT_स्मृतिः  स्मृतिः = FT_FACE_स्मृतिः( face );


        परिणाम = vcmap->clazz->variant_list( vcmap, स्मृतिः );
      }
    }

    प्रत्यागम परिणाम;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt32* )
  FT_Face_Get_Variants_सम्बन्धि_Char( FT_Face   face,
                             FT_ULong  charcode )
  {
    FT_UInt32  *परिणाम = रिक्त;


    यदि ( face )
    {
      FT_CharMap  charmap = find_variant_selector_charmap( face );


      यदि ( charmap )
      {
        FT_CMap    vcmap  = FT_CMAP( charmap );
        FT_स्मृतिः  स्मृतिः = FT_FACE_स्मृतिः( face );


        यदि ( charcode > 0xFFFFFFFFUL )
        {
          FT_TRACE1(( "FT_Face_GetVariantsOfChar: too large charcode" ));
          FT_TRACE1(( " 0x%lx is truncated\n", charcode ));
        }

        परिणाम = vcmap->clazz->charvariant_list( vcmap, स्मृतिः,
                                                 (FT_UInt32)charcode );
      }
    }
    प्रत्यागम परिणाम;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt32* )
  FT_Face_Get_Chars_सम्बन्धि_Variant( FT_Face   face,
                             FT_ULong  variantSelector )
  {
    FT_UInt32  *परिणाम = रिक्त;


    यदि ( face )
    {
      FT_CharMap  charmap = find_variant_selector_charmap( face );


      यदि ( charmap )
      {
        FT_CMap    vcmap  = FT_CMAP( charmap );
        FT_स्मृतिः  स्मृतिः = FT_FACE_स्मृतिः( face );


        यदि ( variantSelector > 0xFFFFFFFFUL )
        {
          FT_TRACE1(( "FT_Get_Char_Index: too large variantSelector" ));
          FT_TRACE1(( " 0x%lx is truncated\n", variantSelector ));
        }

        परिणाम = vcmap->clazz->variantchar_list( vcmap, स्मृतिः,
                                                 (FT_UInt32)variantSelector );
      }
    }

    प्रत्यागम परिणाम;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt )
  FT_Get_Name_सूचकांक( FT_Face           face,
                     ध्रुव FT_String*  glyph_name )
  {
    FT_UInt  परिणाम = 0;


    यदि ( face                       &&
         FT_HAS_GLYPH_नामानि( face ) &&
         glyph_name                 )
    {
      FT_Service_GlyphDict  service;


      FT_FACE_LOOKUP_SERVICE( face,
                              service,
                              GLYPH_DICT );

      यदि ( service && service->name_सूचकांक )
        परिणाम = service->name_सूचकांक( face, glyph_name );
    }

    प्रत्यागम परिणाम;
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


    यदि ( !face )
      प्रत्यागम FT_THROW( Invalid_Face_Handle );

    यदि ( !बफर || बफर_max == 0 )
      प्रत्यागम FT_THROW( Invalid_तर्कः );

    /* clean up buffer */
    ((FT_Byte*)बफर)[0] = '\0';

    यदि ( (FT_Long)glyph_सूचकांक >= face->अङ्कः_glyphs )
      प्रत्यागम FT_THROW( Invalid_Glyph_सूचकांक );

    यदि ( !FT_HAS_GLYPH_नामानि( face ) )
      प्रत्यागम FT_THROW( Invalid_तर्कः );

    FT_FACE_LOOKUP_SERVICE( face, service, GLYPH_DICT );
    यदि ( service && service->get_name )
      error = service->get_name( face, glyph_सूचकांक, बफर, बफर_max );
    अन्यथा
      error = FT_THROW( Invalid_तर्कः );

    प्रत्यागम error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( ध्रुव वर्ण* )
  FT_Get_Postscript_Name( FT_Face  face )
  {
    ध्रुव वर्ण*  परिणाम = रिक्त;


    यदि ( !face )
      गच्छ Exit;

    यदि ( !परिणाम )
    {
      FT_Service_PsFontName  service;


      FT_FACE_LOOKUP_SERVICE( face,
                              service,
                              POSTSCRIPT_FONT_NAME );

      यदि ( service && service->get_ps_font_name )
        परिणाम = service->get_ps_font_name( face );
    }

  Exit:
    प्रत्यागम परिणाम;
  }


  /* documentation is in tttables.h */

  FT_EXPORT_DEF( शून्य* )
  FT_Get_Sfnt_सारणी( FT_Face      face,
                     FT_Sfnt_चिह्नकम्  चिह्नकम् )
  {
    शून्य*                  सारणी = रिक्त;
    FT_Service_SFNT_सारणी  service;


    यदि ( face && FT_IS_SFNT( face ) )
    {
      FT_FACE_FIND_SERVICE( face, service, SFNT_सारणी );
      यदि ( service )
        सारणी = service->get_सारणी( face, चिह्नकम् );
    }

    प्रत्यागम सारणी;
  }


  /* documentation is in tttables.h */

  FT_EXPORT_DEF( FT_Error )
  FT_आरोपय_Sfnt_सारणी( FT_Face    face,
                      FT_ULong   चिह्नकम्,
                      FT_Long    विस्थापनम्,
                      FT_Byte*   बफर,
                      FT_ULong*  दीर्घता )
  {
    FT_Service_SFNT_सारणी  service;


    यदि ( !face || !FT_IS_SFNT( face ) )
      प्रत्यागम FT_THROW( Invalid_Face_Handle );

    FT_FACE_FIND_SERVICE( face, service, SFNT_सारणी );
    यदि ( !service )
      प्रत्यागम FT_THROW( Unimplemented_Feature );

    प्रत्यागम service->आरोपय_सारणी( face, चिह्नकम्, विस्थापनम्, बफर, दीर्घता );
  }


  /* documentation is in tttables.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Sfnt_सारणी_Info( FT_Face    face,
                      FT_UInt    सारणी_सूचकांक,
                      FT_ULong  *चिह्नकम्,
                      FT_ULong  *दीर्घता )
  {
    FT_Service_SFNT_सारणी  service;
    FT_ULong               विस्थापनम्;


    /* test for valid `length' delayed to `service->table_info' */

    यदि ( !face || !FT_IS_SFNT( face ) )
      प्रत्यागम FT_THROW( Invalid_Face_Handle );

    FT_FACE_FIND_SERVICE( face, service, SFNT_सारणी );
    यदि ( !service )
      प्रत्यागम FT_THROW( Unimplemented_Feature );

    प्रत्यागम service->सारणी_info( face, सारणी_सूचकांक, चिह्नकम्, &विस्थापनम्, दीर्घता );
  }


  /* documentation is in tttables.h */

  FT_EXPORT_DEF( FT_ULong )
  FT_Get_CMap_Language_ID( FT_CharMap  charmap )
  {
    FT_Service_TTCMaps  service;
    FT_Face             face;
    TT_CMapInfo         cmap_info;


    यदि ( !charmap || !charmap->face )
      प्रत्यागम 0;

    face = charmap->face;
    FT_FACE_FIND_SERVICE( face, service, TT_CMAP );
    यदि ( !service )
      प्रत्यागम 0;
    यदि ( service->get_cmap_info( charmap, &cmap_info ))
      प्रत्यागम 0;

    प्रत्यागम cmap_info.language;
  }


  /* documentation is in tttables.h */

  FT_EXPORT_DEF( FT_Long )
  FT_Get_CMap_Format( FT_CharMap  charmap )
  {
    FT_Service_TTCMaps  service;
    FT_Face             face;
    TT_CMapInfo         cmap_info;


    यदि ( !charmap || !charmap->face )
      प्रत्यागम -1;

    face = charmap->face;
    FT_FACE_FIND_SERVICE( face, service, TT_CMAP );
    यदि ( !service )
      प्रत्यागम -1;
    यदि ( service->get_cmap_info( charmap, &cmap_info ))
      प्रत्यागम -1;

    प्रत्यागम cmap_info.format;
  }


  /* documentation is in ftsizes.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Activate_आकार( FT_आकार  आकार )
  {
    FT_Face  face;


    यदि ( !आकार )
      प्रत्यागम FT_THROW( Invalid_आकार_Handle );

    face = आकार->face;
    यदि ( !face || !face->driver )
      प्रत्यागम FT_THROW( Invalid_Face_Handle );

    /* we don't need anything more complex than that; all size objects */
    /* are already listed by the face                                  */
    face->आकार = आकार;

    प्रत्यागम FT_Err_Ok;
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
  FT_आधारः_DEF( FT_Renderer )
  FT_Lookup_Renderer( FT_Library       library,
                      FT_Glyph_Format  format,
                      FT_List_ग्रन्थिः*     ग्रन्थिः )
  {
    FT_List_ग्रन्थिः  cur;
    FT_Renderer  परिणाम = रिक्त;


    यदि ( !library )
      गच्छ Exit;

    cur = library->renderers.शीर्ष;

    यदि ( ग्रन्थिः )
    {
      यदि ( *ग्रन्थिः )
        cur = (*ग्रन्थिः)->अग्र;
      *ग्रन्थिः = रिक्त;
    }

    यावत् ( cur )
    {
      FT_Renderer  renderer = FT_RENDERER( cur->दत्त );


      यदि ( renderer->glyph_format == format )
      {
        यदि ( ग्रन्थिः )
          *ग्रन्थिः = cur;

        परिणाम = renderer;
        भङ्ग;
      }
      cur = cur->अग्र;
    }

  Exit:
    प्रत्यागम परिणाम;
  }


  स्थिर FT_Renderer
  ft_lookup_glyph_renderer( FT_GlyphSlot  slot )
  {
    FT_Face      face    = slot->face;
    FT_Library   library = FT_FACE_LIBRARY( face );
    FT_Renderer  परिणाम  = library->cur_renderer;


    यदि ( !परिणाम || परिणाम->glyph_format != slot->format )
      परिणाम = FT_Lookup_Renderer( library, slot->format, 0 );

    प्रत्यागम परिणाम;
  }


  स्थिर शून्य
  ft_समुच्चयः_current_renderer( FT_Library  library )
  {
    FT_Renderer  renderer;


    renderer = FT_Lookup_Renderer( library, FT_GLYPH_FORMAT_OUTLINE, 0 );
    library->cur_renderer = renderer;
  }


  स्थिर FT_Error
  ft_सङ्कलय_renderer( FT_Module  module )
  {
    FT_Library   library = module->library;
    FT_स्मृतिः    स्मृतिः  = library->स्मृतिः;
    FT_Error     error;
    FT_List_ग्रन्थिः  ग्रन्थिः    = रिक्त;


    यदि ( FT_QNEW( ग्रन्थिः ) )
      गच्छ Exit;

    {
      FT_Renderer         render = FT_RENDERER( module );
      FT_Renderer_Class*  clazz  = (FT_Renderer_Class*)module->clazz;


      render->clazz        = clazz;
      render->glyph_format = clazz->glyph_format;

      /* allocate raster object if needed */
      यदि ( clazz->raster_class && clazz->raster_class->raster_new )
      {
        error = clazz->raster_class->raster_new( स्मृतिः, &render->raster );
        यदि ( error )
          गच्छ Fail;

        render->raster_render = clazz->raster_class->raster_render;
        render->render        = clazz->render_glyph;
      }

#यदि_परिभाषितम् FT_CONFIG_OPTION_SVG
      यदि ( clazz->glyph_format == FT_GLYPH_FORMAT_SVG )
        render->render = clazz->render_glyph;
#यदि_अन्तः

      /* add to list */
      ग्रन्थिः->दत्त = module;
      FT_List_सङ्कलय( &library->renderers, ग्रन्थिः );

      ft_समुच्चयः_current_renderer( library );
    }

  Fail:
    यदि ( error )
      FT_FREE( ग्रन्थिः );

  Exit:
    प्रत्यागम error;
  }


  स्थिर शून्य
  ft_remove_renderer( FT_Module  module )
  {
    FT_Library   library;
    FT_स्मृतिः    स्मृतिः;
    FT_List_ग्रन्थिः  ग्रन्थिः;


    library = module->library;
    यदि ( !library )
      प्रत्यागम;

    स्मृतिः = library->स्मृतिः;

    ग्रन्थिः = FT_List_Find( &library->renderers, module );
    यदि ( ग्रन्थिः )
    {
      FT_Renderer  render = FT_RENDERER( module );


      /* release raster object, if any */
      यदि ( render->raster )
        render->clazz->raster_class->raster_समाप्तम्( render->raster );

      /* remove from list */
      FT_List_Remove( &library->renderers, ग्रन्थिः );
      FT_FREE( ग्रन्थिः );

      ft_समुच्चयः_current_renderer( library );
    }
  }


  /* documentation is in ftrender.h */

  FT_EXPORT_DEF( FT_Renderer )
  FT_Get_Renderer( FT_Library       library,
                   FT_Glyph_Format  format )
  {
    /* test for valid `library' delayed to `FT_Lookup_Renderer' */

    प्रत्यागम FT_Lookup_Renderer( library, format, 0 );
  }


  /* documentation is in ftrender.h */

  FT_EXPORT_DEF( FT_Error )
  FT_समुच्चयः_Renderer( FT_Library     library,
                   FT_Renderer    renderer,
                   FT_UInt        अङ्कः_params,
                   FT_Parameter*  parameters )
  {
    FT_List_ग्रन्थिः  ग्रन्थिः;
    FT_Error     error = FT_Err_Ok;

    FT_Renderer_समुच्चयः_बहुलकम्_Func  समुच्चयः_बहुलकम्;


    यदि ( !library )
    {
      error = FT_THROW( Invalid_Library_Handle );
      गच्छ Exit;
    }

    यदि ( !renderer )
    {
      error = FT_THROW( Invalid_तर्कः );
      गच्छ Exit;
    }

    यदि ( अङ्कः_params > 0 && !parameters )
    {
      error = FT_THROW( Invalid_तर्कः );
      गच्छ Exit;
    }

    ग्रन्थिः = FT_List_Find( &library->renderers, renderer );
    यदि ( !ग्रन्थिः )
    {
      error = FT_THROW( Invalid_तर्कः );
      गच्छ Exit;
    }

    FT_List_ऊर्ध्वम्( &library->renderers, ग्रन्थिः );

    यदि ( renderer->glyph_format == FT_GLYPH_FORMAT_OUTLINE )
      library->cur_renderer = renderer;

    समुच्चयः_बहुलकम् = renderer->clazz->समुच्चयः_बहुलकम्;

    प्रति ( ; अङ्कः_params > 0; अङ्कः_params-- )
    {
      error = समुच्चयः_बहुलकम्( renderer, parameters->चिह्नकम्, parameters->दत्त );
      यदि ( error )
        भङ्ग;
      parameters++;
    }

  Exit:
    प्रत्यागम error;
  }


  FT_आधारः_DEF( FT_Error )
  FT_Render_Glyph_आन्तरिकम्( FT_Library      library,
                            FT_GlyphSlot    slot,
                            FT_Render_बहुलकम्  render_बहुलकम् )
  {
    FT_Error     error = FT_Err_Ok;
    FT_Face      face  = slot->face;
    FT_Renderer  renderer;
    FT_List_ग्रन्थिः  ग्रन्थिः  = रिक्त;


    /* try to render colored glyph layers as a special case */
    यदि ( slot->आन्तरिकम्->आरोपय_flags & FT_आरोपय_COLOR &&
         slot->format == FT_GLYPH_FORMAT_OUTLINE    )
    {
      FT_LayerIterator  iterator;

      FT_UInt  आधारः_glyph = slot->glyph_सूचकांक;

      FT_Bool  have_layers;
      FT_UInt  glyph_सूचकांक;
      FT_UInt  color_सूचकांक;


      iterator.p  = रिक्त;
      have_layers = FT_Get_Color_Glyph_Layer( face,
                                              आधारः_glyph,
                                              &glyph_सूचकांक,
                                              &color_सूचकांक,
                                              &iterator );
      यदि ( have_layers )
      {
        error = FT_New_GlyphSlot( face, रिक्त );
        यदि ( !error )
        {
          TT_Face       ttface = (TT_Face)face;
          SFNT_Service  sfnt   = (SFNT_Service)ttface->sfnt;


          कुरु
          {
            FT_Int32  आरोपय_flags = slot->आन्तरिकम्->आरोपय_flags;


            /* disable the `FT_LOAD_COLOR' flag to avoid recursion */
            /* right here in this function                         */
            आरोपय_flags &= ~FT_आरोपय_COLOR;

            /* render into the new `face->glyph' glyph slot */
            आरोपय_flags |= FT_आरोपय_RENDER | FT_आरोपय_NO_BITMAP;

            error = FT_आरोपय_Glyph( face, glyph_सूचकांक, आरोपय_flags );
            यदि ( error )
              भङ्ग;

            /* blend new `face->glyph' into old `slot'; */
            /* at the first call, `slot' is still empty */
            error = sfnt->colr_blend( ttface,
                                      color_सूचकांक,
                                      slot,
                                      face->glyph );
            यदि ( error )
              भङ्ग;

          } यावत् ( FT_Get_Color_Glyph_Layer( face,
                                              आधारः_glyph,
                                              &glyph_सूचकांक,
                                              &color_सूचकांक,
                                              &iterator ) );

          यदि ( !error )
            slot->format = FT_GLYPH_FORMAT_BITMAP;

          /* this call also restores `slot' as the glyph slot */
          FT_समाप्तम्_Glyph_Slot( face->glyph );
        }

        यदि ( !error )
          प्रत्यागम error;

        /* Failed to do the colored layer.  Draw outline instead. */
        slot->format = FT_GLYPH_FORMAT_OUTLINE;
      }
    }

    /* small shortcut for the very common case */
    यदि ( slot->format == FT_GLYPH_FORMAT_OUTLINE )
    {
      renderer = library->cur_renderer;
      ग्रन्थिः     = library->renderers.शीर्ष;
    }
    अन्यथा
      renderer = FT_Lookup_Renderer( library, slot->format, &ग्रन्थिः );

    error = FT_ERR( Cannot_Render_Glyph );
    यावत् ( renderer )
    {
      error = renderer->render( renderer, slot, render_बहुलकम्, रिक्त );
      यदि ( !error                                   ||
           FT_ERR_NEQ( error, Cannot_Render_Glyph ) )
        भङ्ग;

      /* `FT_Err_Cannot_Render_Glyph` is returned if the render mode */
      /* is unsupported by the current renderer for this glyph image */
      /* format.                                                     */

      /* now, look for another renderer that supports the same */
      /* format.                                               */
      renderer = FT_Lookup_Renderer( library, slot->format, &ग्रन्थिः );
    }

    /* it is not an error if we cannot render a bitmap glyph */
    यदि ( FT_ERR_EQ( error, Cannot_Render_Glyph ) &&
         slot->format == FT_GLYPH_FORMAT_BITMAP  )
      error = FT_Err_Ok;

#यदि_परिभाषितम् FT_DEBUG_LEVEL_TRACE

#अपरिभाषय  FT_COMPONENT
#परिभाषय FT_COMPONENT  checksum

    /*
     * Computing the MD5 checksum is expensive, unnecessarily distorting a
     * possible profiling of FreeType if compiled with tracing support.  For
     * this reason, we execute the following code only if explicitly
     * requested.
     */

    /* we use FT_TRACE3 in this block */
    यदि ( !error                               &&
         ft_trace_स्तराः[trace_checksum] >= 3 &&
         slot->bitmap.बफर                  )
    {
      FT_Bitmap  bitmap;
      FT_Error   err;


      FT_Bitmap_Init( &bitmap );

      /* we convert to a single bitmap format for computing the checksum */
      /* this also converts the bitmap flow to `down' (i.e., pitch > 0)  */
      err = FT_Bitmap_Convert( library, &slot->bitmap, &bitmap, 1 );
      यदि ( !err )
      {
        MD5_CTX        ctx;
        अचिह्नित वर्ण  md5[16];
        अचिह्नित दीर्घ  coverage = 0;
        पूर्णाङ्क            i, j;
        पूर्णाङ्क            पङ्क्तिकाः  = (पूर्णाङ्क)bitmap.पङ्क्तिकाः;
        पूर्णाङ्क            pitch = bitmap.pitch;


        FT_TRACE3(( "FT_Render_Glyph: bitmap %dx%d, %s (mode %d)\n",
                    pitch,
                    पङ्क्तिकाः,
                    pixel_modes[slot->bitmap.pixel_बहुलकम्],
                    slot->bitmap.pixel_बहुलकम् ));

        प्रति ( i = 0; i < पङ्क्तिकाः; i++ )
          प्रति ( j = 0; j < pitch; j++ )
            coverage += bitmap.बफर[i * pitch + j];

        FT_TRACE3(( "  Total coverage: %lu\n", coverage ));

        MD5_Init( &ctx );
        यदि ( bitmap.बफर )
          MD5_Update( &ctx, bitmap.बफर,
                      (अचिह्नित दीर्घ)पङ्क्तिकाः * (अचिह्नित दीर्घ)pitch );
        MD5_Final( md5, &ctx );

        FT_TRACE3(( "  MD5 checksum: " ));
        प्रति ( i = 0; i < 16; i++ )
          FT_TRACE3(( "%02X", md5[i] ));
        FT_TRACE3(( "\n" ));
      }

      FT_Bitmap_समाप्तम्( library, &bitmap );
    }

    /*
     * Dump bitmap in Netpbm format (PBM or PGM).
     */

    /* we use FT_TRACE7 in this block */
    यदि ( !error                               &&
         ft_trace_स्तराः[trace_checksum] >= 7 &&
         slot->bitmap.बफर                  )
    {
      यदि ( slot->bitmap.पङ्क्तिकाः  < 128U &&
           slot->bitmap.विस्तारः < 128U )
      {
        पूर्णाङ्क  पङ्क्तिकाः  = (पूर्णाङ्क)slot->bitmap.पङ्क्तिकाः;
        पूर्णाङ्क  विस्तारः = (पूर्णाङ्क)slot->bitmap.विस्तारः;
        पूर्णाङ्क  pitch =      slot->bitmap.pitch;
        पूर्णाङ्क  i, j, m;

        अचिह्नित वर्ण*  topleft = slot->bitmap.बफर;


        यदि ( pitch < 0 )
          topleft -= pitch * ( पङ्क्तिकाः - 1 );

        FT_TRACE7(( "Netpbm image: start\n" ));
        विकल्प ( slot->bitmap.pixel_बहुलकम् )
        {
        पक्ष FT_PIXEL_बहुलकम्_MONO:
          FT_TRACE7(( "P1 %d %d\n", विस्तारः, पङ्क्तिकाः ));
          प्रति ( i = 0; i < पङ्क्तिकाः; i++ )
          {
            प्रति ( j = 0; j < विस्तारः; )
              प्रति ( m = 128; m > 0 && j < विस्तारः; m >>= 1, j++ )
                FT_TRACE7(( " %d",
                            ( topleft[i * pitch + j / 8] & m ) != 0 ));
            FT_TRACE7(( "\n" ));
          }
          भङ्ग;

        स्वभाव:
          FT_TRACE7(( "P2 %d %d 255\n", विस्तारः, पङ्क्तिकाः ));
          प्रति ( i = 0; i < पङ्क्तिकाः; i++ )
          {
            प्रति ( j = 0; j < विस्तारः; j += 1 )
              FT_TRACE7(( " %3u", topleft[i * pitch + j] ));
            FT_TRACE7(( "\n" ));
          }
        }
        FT_TRACE7(( "Netpbm image: end\n" ));
      }
      अन्यथा
        FT_TRACE7(( "Netpbm image: too large, omitted\n" ));
    }

#अपरिभाषय  FT_COMPONENT
#परिभाषय FT_COMPONENT  objs

#यदि_अन्तः /* FT_DEBUG_LEVEL_TRACE */

    प्रत्यागम error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Render_Glyph( FT_GlyphSlot    slot,
                   FT_Render_बहुलकम्  render_बहुलकम् )
  {
    FT_Library  library;


    यदि ( !slot || !slot->face )
      प्रत्यागम FT_THROW( Invalid_तर्कः );

    library = FT_FACE_LIBRARY( slot->face );

    प्रत्यागम FT_Render_Glyph_आन्तरिकम्( library, slot, render_बहुलकम् );
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
  स्थिर शून्य
  Destroy_Module( FT_Module  module )
  {
    ध्रुव FT_Module_Class*  clazz   = module->clazz;
    FT_Library              library = module->library;
    FT_स्मृतिः               स्मृतिः  = module->स्मृतिः;


    यदि ( library && library->auto_hinter == module )
      library->auto_hinter = रिक्त;

    /* if the module is a renderer */
    यदि ( FT_MODULE_IS_RENDERER( module ) )
      ft_remove_renderer( module );

    /* if the module is a font driver, add some steps */
    यदि ( FT_MODULE_IS_DRIVER( module ) )
      Destroy_Driver( FT_DRIVER( module ) );

    /* finalize the module object */
    यदि ( clazz->module_समाप्तम् )
      clazz->module_समाप्तम्( module );

    /* discard it */
    FT_FREE( module );
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_सङ्कलय_Module( FT_Library              library,
                 ध्रुव FT_Module_Class*  clazz )
  {
    FT_Error   error;
    FT_स्मृतिः  स्मृतिः;
    FT_Module  module = रिक्त;
    FT_UInt    nn;


#परिभाषय FREETYPE_VER_FIXED  ( ( (FT_Long)FREETYPE_MAJOR << 16 ) | \
                                FREETYPE_MINOR                  )

    यदि ( !library )
      प्रत्यागम FT_THROW( Invalid_Library_Handle );

    यदि ( !clazz )
      प्रत्यागम FT_THROW( Invalid_तर्कः );

    /* check FreeType version */
    यदि ( clazz->module_requires > FREETYPE_VER_FIXED )
      प्रत्यागम FT_THROW( Invalid_संस्करणम् );

    /* look for a module with the same name in the library's table */
    प्रति ( nn = 0; nn < library->अङ्कः_modules; nn++ )
    {
      module = library->modules[nn];
      यदि ( ft_strcmp( module->clazz->module_name, clazz->module_name ) == 0 )
      {
        /* this installed module has the same name, compare their versions */
        यदि ( clazz->module_संस्करणम् <= module->clazz->module_संस्करणम् )
          प्रत्यागम FT_THROW( अधस्तनम्_Module_संस्करणम् );

        /* remove the module from our list, then exit the loop to replace */
        /* it by our new version..                                        */
        FT_Remove_Module( library, module );
        भङ्ग;
      }
    }

    स्मृतिः = library->स्मृतिः;
    error  = FT_Err_Ok;

    यदि ( library->अङ्कः_modules >= FT_MAX_MODULES )
    {
      error = FT_THROW( Too_Many_Drivers );
      गच्छ Exit;
    }

    /* allocate module object */
    यदि ( FT_ALLOC( module, clazz->module_आकार ) )
      गच्छ Exit;

    /* base initialization */
    module->clazz   = clazz;
    module->library = library;
    module->स्मृतिः  = स्मृतिः;

    /* check whether the module is a renderer - this must be performed */
    /* before the normal module initialization                         */
    यदि ( FT_MODULE_IS_RENDERER( module ) )
    {
      /* add to the renderers list */
      error = ft_सङ्कलय_renderer( module );
      यदि ( error )
        गच्छ Fail;
    }

    /* is the module a auto-hinter? */
    यदि ( FT_MODULE_IS_HINTER( module ) )
      library->auto_hinter = module;

    /* if the module is a font driver */
    यदि ( FT_MODULE_IS_DRIVER( module ) )
    {
      FT_Driver  driver = FT_DRIVER( module );


      driver->clazz = (FT_Driver_Class)module->clazz;
    }

    यदि ( clazz->module_init )
    {
      error = clazz->module_init( module );
      यदि ( error )
        गच्छ Fail;
    }

    /* add module to the library's table */
    library->modules[library->अङ्कः_modules++] = module;

  Exit:
    प्रत्यागम error;

  Fail:
    यदि ( FT_MODULE_IS_RENDERER( module ) )
    {
      FT_Renderer  renderer = FT_RENDERER( module );


      यदि ( renderer->clazz                                          &&
           renderer->clazz->glyph_format == FT_GLYPH_FORMAT_OUTLINE &&
           renderer->raster                                         )
        renderer->clazz->raster_class->raster_समाप्तम्( renderer->raster );
    }

    FT_FREE( module );
    गच्छ Exit;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Module )
  FT_Get_Module( FT_Library   library,
                 ध्रुव वर्ण*  module_name )
  {
    FT_Module   परिणाम = रिक्त;
    FT_Module*  cur;
    FT_Module*  सीमा;


    यदि ( !library || !module_name )
      प्रत्यागम परिणाम;

    cur   = library->modules;
    सीमा = cur + library->अङ्कः_modules;

    प्रति ( ; cur < सीमा; cur++ )
      यदि ( ft_strcmp( cur[0]->clazz->module_name, module_name ) == 0 )
      {
        परिणाम = cur[0];
        भङ्ग;
      }

    प्रत्यागम परिणाम;
  }


  /* documentation is in ftobjs.h */

  FT_आधारः_DEF( ध्रुव शून्य* )
  FT_Get_Module_Interface( FT_Library   library,
                           ध्रुव वर्ण*  mod_name )
  {
    FT_Module  module;


    /* test for valid `library' delayed to FT_Get_Module() */

    module = FT_Get_Module( library, mod_name );

    प्रत्यागम module ? module->clazz->module_interface : 0;
  }


  FT_आधारः_DEF( FT_Pointer )
  ft_module_get_service( FT_Module    module,
                         ध्रुव वर्ण*  service_id,
                         FT_Bool      global )
  {
    FT_Pointer  परिणाम = रिक्त;


    यदि ( module )
    {
      FT_ASSERT( module->clazz && module->clazz->get_interface );

      /* first, look for the service in the module */
      यदि ( module->clazz->get_interface )
        परिणाम = module->clazz->get_interface( module, service_id );

      यदि ( global && !परिणाम )
      {
        /* we didn't find it, look in all other modules then */
        FT_Library  library = module->library;
        FT_Module*  cur     = library->modules;
        FT_Module*  सीमा   = cur + library->अङ्कः_modules;


        प्रति ( ; cur < सीमा; cur++ )
        {
          यदि ( cur[0] != module )
          {
            FT_ASSERT( cur[0]->clazz );

            यदि ( cur[0]->clazz->get_interface )
            {
              परिणाम = cur[0]->clazz->get_interface( cur[0], service_id );
              यदि ( परिणाम )
                भङ्ग;
            }
          }
        }
      }
    }

    प्रत्यागम परिणाम;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Remove_Module( FT_Library  library,
                    FT_Module   module )
  {
    /* try to find the module from the table, then remove it from there */

    यदि ( !library )
      प्रत्यागम FT_THROW( Invalid_Library_Handle );

    यदि ( module )
    {
      FT_Module*  cur   = library->modules;
      FT_Module*  सीमा = cur + library->अङ्कः_modules;


      प्रति ( ; cur < सीमा; cur++ )
      {
        यदि ( cur[0] == module )
        {
          /* remove it from the table */
          library->अङ्कः_modules--;
          सीमा--;
          यावत् ( cur < सीमा )
          {
            cur[0] = cur[1];
            cur++;
          }
          सीमा[0] = रिक्त;

          /* destroy the module */
          Destroy_Module( module );

          प्रत्यागम FT_Err_Ok;
        }
      }
    }
    प्रत्यागम FT_THROW( Invalid_Driver_Handle );
  }


  स्थिर FT_Error
  ft_property_do( FT_Library        library,
                  ध्रुव FT_String*  module_name,
                  ध्रुव FT_String*  property_name,
                  शून्य*             मानम्,
                  FT_Bool           समुच्चयः,
                  FT_Bool           मानम्_is_string )
  {
    FT_Module*           cur;
    FT_Module*           सीमा;
    FT_Module_Interface  interface;

    FT_Service_Properties  service;

#यदि_परिभाषितम् FT_DEBUG_LEVEL_ERROR
    ध्रुव FT_String*  समुच्चयः_name  = "FT_Property_Set";
    ध्रुव FT_String*  get_name  = "FT_Property_Get";
    ध्रुव FT_String*  func_name = समुच्चयः ? समुच्चयः_name : get_name;
#यदि_अन्तः

    FT_Bool  missing_func;


    यदि ( !library )
      प्रत्यागम FT_THROW( Invalid_Library_Handle );

    यदि ( !module_name || !property_name || !मानम् )
      प्रत्यागम FT_THROW( Invalid_तर्कः );

    cur   = library->modules;
    सीमा = cur + library->अङ्कः_modules;

    /* search module */
    प्रति ( ; cur < सीमा; cur++ )
      यदि ( !ft_strcmp( cur[0]->clazz->module_name, module_name ) )
        भङ्ग;

    यदि ( cur == सीमा )
    {
      FT_TRACE2(( "%s: can't find module `%s'\n",
                  func_name, module_name ));
      प्रत्यागम FT_THROW( Missing_Module );
    }

    /* check whether we have a service interface */
    यदि ( !cur[0]->clazz->get_interface )
    {
      FT_TRACE2(( "%s: module `%s' doesn't support properties\n",
                  func_name, module_name ));
      प्रत्यागम FT_THROW( Unimplemented_Feature );
    }

    /* search property service */
    interface = cur[0]->clazz->get_interface( cur[0],
                                              FT_SERVICE_ID_PROPERTIES );
    यदि ( !interface )
    {
      FT_TRACE2(( "%s: module `%s' doesn't support properties\n",
                  func_name, module_name ));
      प्रत्यागम FT_THROW( Unimplemented_Feature );
    }

    service = (FT_Service_Properties)interface;

    यदि ( समुच्चयः )
      missing_func = FT_BOOL( !service->समुच्चयः_property );
    अन्यथा
      missing_func = FT_BOOL( !service->get_property );

    यदि ( missing_func )
    {
      FT_TRACE2(( "%s: property service of module `%s' is broken\n",
                  func_name, module_name ));
      प्रत्यागम FT_THROW( Unimplemented_Feature );
    }

    प्रत्यागम समुच्चयः ? service->समुच्चयः_property( cur[0],
                                        property_name,
                                        मानम्,
                                        मानम्_is_string )
               : service->get_property( cur[0],
                                        property_name,
                                        मानम् );
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Property_समुच्चयः( FT_Library        library,
                   ध्रुव FT_String*  module_name,
                   ध्रुव FT_String*  property_name,
                   ध्रुव शून्य*       मानम् )
  {
    प्रत्यागम ft_property_do( library,
                           module_name,
                           property_name,
                           (शून्य*)मानम्,
                           TRUE,
                           FALSE );
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Property_Get( FT_Library        library,
                   ध्रुव FT_String*  module_name,
                   ध्रुव FT_String*  property_name,
                   शून्य*             मानम् )
  {
    प्रत्यागम ft_property_do( library,
                           module_name,
                           property_name,
                           मानम्,
                           FALSE,
                           FALSE );
  }


#यदि_परिभाषितम् FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES

  /* this variant is used for handling the FREETYPE_PROPERTIES */
  /* environment variable                                      */

  FT_आधारः_DEF( FT_Error )
  ft_property_string_समुच्चयः( FT_Library        library,
                          ध्रुव FT_String*  module_name,
                          ध्रुव FT_String*  property_name,
                          FT_String*        मानम् )
  {
    प्रत्यागम ft_property_do( library,
                           module_name,
                           property_name,
                           (शून्य*)मानम्,
                           TRUE,
                           TRUE );
  }

#यदि_अन्तः


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
    यदि ( !library )
      प्रत्यागम FT_THROW( Invalid_Library_Handle );

    library->refcount++;

    प्रत्यागम FT_Err_Ok;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_New_Library( FT_स्मृतिः    स्मृतिः,
                  FT_Library  *alibrary )
  {
    FT_Library  library = रिक्त;
    FT_Error    error;


    यदि ( !स्मृतिः || !alibrary )
      प्रत्यागम FT_THROW( Invalid_तर्कः );

#यदि_अपरिभाषितम् FT_DEBUG_LOGGING
#यदि_परिभाषितम् FT_DEBUG_LEVEL_ERROR
    /* init debugging support */
    ft_debug_init();
#यदि_अन्तः /* FT_DEBUG_LEVEL_ERROR */
#यदि_अन्तः /* !FT_DEBUG_LOGGING */

    /* first of all, allocate the library object */
    यदि ( FT_NEW( library ) )
      प्रत्यागम error;

    library->स्मृतिः = स्मृतिः;

    library->संस्करणम्_major = FREETYPE_MAJOR;
    library->संस्करणम्_minor = FREETYPE_MINOR;
    library->संस्करणम्_patch = FREETYPE_PATCH;

    library->refcount = 1;

    /* That's ok now */
    *alibrary = library;

    प्रत्यागम FT_Err_Ok;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( शून्य )
  FT_Library_संस्करणम्( FT_Library   library,
                      FT_Int      *amajor,
                      FT_Int      *aminor,
                      FT_Int      *apatch )
  {
    FT_Int  major = 0;
    FT_Int  minor = 0;
    FT_Int  patch = 0;


    यदि ( library )
    {
      major = library->संस्करणम्_major;
      minor = library->संस्करणम्_minor;
      patch = library->संस्करणम्_patch;
    }

    यदि ( amajor )
      *amajor = major;

    यदि ( aminor )
      *aminor = minor;

    यदि ( apatch )
      *apatch = patch;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_समाप्तम्_Library( FT_Library  library )
  {
    FT_स्मृतिः  स्मृतिः;


    यदि ( !library )
      प्रत्यागम FT_THROW( Invalid_Library_Handle );

    library->refcount--;
    यदि ( library->refcount > 0 )
      गच्छ Exit;

    स्मृतिः = library->स्मृतिः;

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
      ध्रुव वर्ण*  driver_name[] = { "type42", रिक्त };


      प्रति ( m = 0;
            m < परिमाण ( driver_name ) / परिमाण ( driver_name[0] );
            m++ )
      {
        प्रति ( n = 0; n < library->अङ्कः_modules; n++ )
        {
          FT_Module    module      = library->modules[n];
          ध्रुव वर्ण*  module_name = module->clazz->module_name;
          FT_List      faces;


          यदि ( driver_name[m]                                &&
               ft_strcmp( module_name, driver_name[m] ) != 0 )
            अनुवर्त;

          यदि ( ( module->clazz->module_flags & FT_MODULE_FONT_DRIVER ) == 0 )
            अनुवर्त;

          FT_TRACE7(( "FT_Done_Library: close faces for %s\n", module_name ));

          faces = &FT_DRIVER( module )->faces_list;
          यावत् ( faces->शीर्ष )
          {
            FT_समाप्तम्_Face( FT_FACE( faces->शीर्ष->दत्त ) );
            यदि ( faces->शीर्ष )
              FT_TRACE0(( "FT_Done_Library: failed to free some faces\n" ));
          }
        }
      }
    }

    /* Close all other modules in the library */
#यदि 1
    /* XXX Modules are removed in the reversed order so that  */
    /* type42 module is removed before truetype module.  This */
    /* avoids double free in some occasions.  It is a hack.   */
    यावत् ( library->अङ्कः_modules > 0 )
      FT_Remove_Module( library,
                        library->modules[library->अङ्कः_modules - 1] );
#अन्यथा
    {
      FT_UInt  n;


      प्रति ( n = 0; n < library->अङ्कः_modules; n++ )
      {
        FT_Module  module = library->modules[n];


        यदि ( module )
        {
          Destroy_Module( module );
          library->modules[n] = रिक्त;
        }
      }
    }
#यदि_अन्तः

    FT_FREE( library );

  Exit:
    प्रत्यागम FT_Err_Ok;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( शून्य )
  FT_समुच्चयः_Debug_Hook( FT_Library         library,
                     FT_UInt            hook_सूचकांक,
                     FT_DebugHook_Func  debug_hook )
  {
    यदि ( library && debug_hook &&
         hook_सूचकांक <
           ( परिमाण ( library->debug_hooks ) / परिमाण ( शून्य* ) ) )
      library->debug_hooks[hook_सूचकांक] = debug_hook;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_TrueTypeEngineType )
  FT_Get_TrueType_Engine_Type( FT_Library  library )
  {
    FT_TrueTypeEngineType  परिणाम = FT_TRUETYPE_ENGINE_TYPE_NONE;


    यदि ( library )
    {
      FT_Module  module = FT_Get_Module( library, "truetype" );


      यदि ( module )
      {
        FT_Service_TrueTypeEngine  service;


        service = (FT_Service_TrueTypeEngine)
                    ft_module_get_service( module,
                                           FT_SERVICE_ID_TRUETYPE_ENGINE,
                                           0 );
        यदि ( service )
          परिणाम = service->engine_type;
      }
    }

    प्रत्यागम परिणाम;
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
    FT_Error  error = FT_ERR( Invalid_तर्कः );


    यदि ( glyph                                      &&
         glyph->subglyphs                           &&
         glyph->format == FT_GLYPH_FORMAT_संयुक्त &&
         sub_सूचकांक < glyph->अङ्कः_subglyphs           )
    {
      FT_SubGlyph  subg = glyph->subglyphs + sub_सूचकांक;


      *p_सूचकांक     = subg->index;
      *p_flags     = subg->flags;
      *p_arg1      = subg->arg1;
      *p_arg2      = subg->arg2;
      *p_transform = subg->transform;

      error = FT_Err_Ok;
    }

    प्रत्यागम error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Get_Color_Glyph_Layer( FT_Face            face,
                            FT_UInt            आधारः_glyph,
                            FT_UInt           *aglyph_सूचकांक,
                            FT_UInt           *acolor_सूचकांक,
                            FT_LayerIterator*  iterator )
  {
    TT_Face       ttface;
    SFNT_Service  sfnt;


    यदि ( !face                                   ||
         !aglyph_सूचकांक                           ||
         !acolor_सूचकांक                           ||
         !iterator                               ||
         आधारः_glyph >= (FT_UInt)face->अङ्कः_glyphs )
      प्रत्यागम 0;

    यदि ( !FT_IS_SFNT( face ) )
      प्रत्यागम 0;

    ttface = (TT_Face)face;
    sfnt   = (SFNT_Service)ttface->sfnt;

    यदि ( sfnt->get_colr_layer )
      प्रत्यागम sfnt->get_colr_layer( ttface,
                                   आधारः_glyph,
                                   aglyph_सूचकांक,
                                   acolor_सूचकांक,
                                   iterator );
    अन्यथा
      प्रत्यागम 0;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Get_Color_Glyph_Paint( FT_Face                  face,
                            FT_UInt                  आधारः_glyph,
                            FT_Color_मूलम्_Transform  मूलम्_transform,
                            FT_OpaquePaint*          paint )
  {
    TT_Face       ttface;
    SFNT_Service  sfnt;


    यदि ( !face || !paint )
      प्रत्यागम 0;

    यदि ( !FT_IS_SFNT( face ) )
      प्रत्यागम 0;

    ttface = (TT_Face)face;
    sfnt   = (SFNT_Service)ttface->sfnt;

    यदि ( sfnt->get_colr_glyph_paint )
      प्रत्यागम sfnt->get_colr_glyph_paint( ttface,
                                         आधारः_glyph,
                                         मूलम्_transform,
                                         paint );
    अन्यथा
      प्रत्यागम 0;
  }


  /* documentation is in ftcolor.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Get_Color_Glyph_ClipBox( FT_Face      face,
                              FT_UInt      आधारः_glyph,
                              FT_ClipBox*  clip_box )
  {
    TT_Face       ttface;
    SFNT_Service  sfnt;


    यदि ( !face || !clip_box )
      प्रत्यागम 0;

    यदि ( !FT_IS_SFNT( face ) )
      प्रत्यागम 0;

    ttface = (TT_Face)face;
    sfnt   = (SFNT_Service)ttface->sfnt;

    यदि ( sfnt->get_color_glyph_clipbox )
      प्रत्यागम sfnt->get_color_glyph_clipbox( ttface,
                                            आधारः_glyph,
                                            clip_box );
    अन्यथा
      प्रत्यागम 0;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Get_Paint_Layers( FT_Face            face,
                       FT_LayerIterator*  layer_iterator,
                       FT_OpaquePaint*    paint )
  {
    TT_Face       ttface;
    SFNT_Service  sfnt;


    यदि ( !face || !paint || !layer_iterator )
      प्रत्यागम 0;

    यदि ( !FT_IS_SFNT( face ) )
      प्रत्यागम 0;

    ttface = (TT_Face)face;
    sfnt   = (SFNT_Service)ttface->sfnt;

    यदि ( sfnt->get_paint_layers )
      प्रत्यागम sfnt->get_paint_layers( ttface, layer_iterator, paint );
    अन्यथा
      प्रत्यागम 0;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Get_Paint( FT_Face face,
                FT_OpaquePaint  opaque_paint,
                FT_COLR_Paint*  paint )
  {
    TT_Face       ttface;
    SFNT_Service  sfnt;


    यदि ( !face || !paint )
      प्रत्यागम 0;

    यदि ( !FT_IS_SFNT( face ) )
      प्रत्यागम 0;

    ttface = (TT_Face)face;
    sfnt   = (SFNT_Service)ttface->sfnt;

    यदि ( sfnt->get_paint )
      प्रत्यागम sfnt->get_paint( ttface, opaque_paint, paint );
    अन्यथा
      प्रत्यागम 0;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Get_Colorline_Stops ( FT_Face                face,
                           FT_ColorStop *         color_stop,
                           FT_ColorStopIterator  *iterator )
  {
    TT_Face       ttface;
    SFNT_Service  sfnt;


    यदि ( !face || !color_stop || !iterator )
      प्रत्यागम 0;

    यदि ( !FT_IS_SFNT( face ) )
      प्रत्यागम 0;

    ttface = (TT_Face)face;
    sfnt   = (SFNT_Service)ttface->sfnt;

    यदि ( sfnt->get_colorline_stops )
      प्रत्यागम sfnt->get_colorline_stops ( ttface, color_stop, iterator );
    अन्यथा
      प्रत्यागम 0;
  }


/*अन्तः*/
