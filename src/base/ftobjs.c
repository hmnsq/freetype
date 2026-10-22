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

#اگر_تعریف_شده TT_CONFIG_OPTION_VARC
#ضمن <freetype/internal/services/svvarc.h>
#پایان_اگر

#ضمن <freetype/ftdriver.h>

#اگر_تعریف_شده FT_CONFIG_OPTION_MAC_FONTS
#ضمن "ftbase.h"
#پایان_اگر


#اگر_تعریف_شده FT_DEBUG_LEVEL_TRACE

#ضمن <freetype/ftbitmap.h>

#إذا defined( _MSC_VER )      /* Visual C++ (and Intel C++)   */
  /* We disable the warning `conversion from XXX to YYY,     */
  /* possible loss of data' in order to compile cleanly with */
  /* the maximum level of warnings: `md5.c' is non-FreeType  */
  /* code, and it gets used during development builds only.  */
#رهنمود warning( افزودن )
#رهنمود warning( disable : 4244 )
#پایان_اگر /* _MSC_VER */

  /* It's easiest to include `md5.c' directly.  However, since OpenSSL */
  /* also provides the same functions, there might be conflicts if     */
  /* both FreeType and OpenSSL are built as static libraries.  For     */
  /* this reason, we put the MD5 stuff into the `FT_' namespace.       */
#تعریف_کن MD5_u32plus  FT_MD5_u32plus
#تعریف_کن MD5_CTX      FT_MD5_CTX
#تعریف_کن MD5_Init     FT_MD5_Init
#تعریف_کن MD5_Update   FT_MD5_Update
#تعریف_کن MD5_Final    FT_MD5_Final

#لغو_تعریف  HAVE_OPENSSL

#ضمن "md5.c"

#إذا defined( _MSC_VER )
#رهنمود warning( برداشت )
#پایان_اگر

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

#پایان_اگر /* FT_DEBUG_LEVEL_TRACE */


#تعریف_کن GRID_FIT_METRICS


  /* forward declaration */
  ساكن FT_Error
  ft_open_face_داخلی( FT_Library           library,
                         ثابت FT_Open_آرگومانها*  آرگومانها,
                         FT_Long              face_شاخص,
                         FT_Face             *aface,
                         FT_Bool              test_mac_fonts );


  FT_پایه_DEF( FT_اشاره_گر )
  ft_service_list_lookup( FT_ServiceDesc  service_descriptors,
                          ثابت حرف*     service_id )
  {
    FT_اشاره_گر      نتیجه = عدم;
    FT_ServiceDesc  desc   = service_descriptors;


    إذا ( desc && service_id )
    {
      لكل ( ; desc->serv_id != عدم; desc++ )
      {
        إذا ( ft_strcmp( desc->serv_id, service_id ) == 0 )
        {
          نتیجه = (FT_اشاره_گر)desc->serv_داده;
          اكسر;
        }
      }
    }

    أرجع نتیجه;
  }


  FT_پایه_DEF( فراغ )
  ft_validator_init( FT_Validator        valid,
                     ثابت FT_Byte*      پایه,
                     ثابت FT_Byte*      حد,
                     FT_ValidationLevel  level )
  {
    valid->پایه  = پایه;
    valid->حد = حد;
    valid->level = level;
    valid->error = FT_Err_Ok;
  }


  FT_پایه_DEF( FT_Int )
  ft_validator_پیاپی( FT_Validator  valid )
  {
    /* This function doesn't work!  None should call it. */
    FT_استفادهنشده( valid );

    أرجع -1;
  }


  FT_پایه_DEF( فراغ )
  ft_validator_error( FT_Validator  valid,
                      FT_Error      error )
  {
    /* since the cast below also disables the compiler's */
    /* type check, we introduce a dummy variable, which  */
    /* will be optimized away                            */
    متطاير ft_jmp_buf* jump_میانگیر = &valid->jump_میانگیر;


    valid->error = error;

    /* throw away volatileness; use `jump_buffer' or the  */
    /* compiler may warn about an unused local variable   */
    ft_longjmp( *(ft_jmp_buf*) jump_میانگیر, 1 );
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
  FT_پایه_DEF( FT_Error )
  FT_جریان_New( FT_Library           library,
                 ثابت FT_Open_آرگومانها*  آرگومانها,
                 FT_جریان           *astream )
  {
    FT_Error   error;
    FT_حافظه  حافظه;
    FT_جریان  جریان = عدم;
    FT_UInt    نما;


    *astream = عدم;

    إذا ( !library )
      أرجع FT_THROW( Invalid_Library_Handle );

    إذا ( !آرگومانها )
      أرجع FT_THROW( Invalid_آرگومان );

    حافظه = library->حافظه;
    نما   = آرگومانها->flags &
               ( FT_OPEN_حافظه | FT_OPEN_جریان | FT_OPEN_PATHNAME );

    إذا ( نما == FT_OPEN_حافظه )
    {
      /* create a memory-based stream */
      إذا ( FT_NEW( جریان ) )
        اذهب Exit;

      FT_جریان_Open_حافظه( جریان,
                            (ثابت FT_Byte*)آرگومانها->حافظه_پایه,
                            (FT_ULong)آرگومانها->حافظه_اندازه );
      جریان->حافظه = حافظه;
    }

#اگر_تعریف_نشده FT_CONFIG_OPTION_DISABLE_جریان_SUPPORT

    وإلا إذا ( نما == FT_OPEN_PATHNAME )
    {
      /* create a normal system stream */
      إذا ( FT_NEW( جریان ) )
        اذهب Exit;

      جریان->حافظه = حافظه;
      error = FT_جریان_Open( جریان, آرگومانها->pathname );
      إذا ( error )
        FT_FREE( جریان );
    }
    وإلا إذا ( ( نما == FT_OPEN_جریان ) && آرگومانها->جریان )
    {
      /* use an existing, user-provided stream */

      /* in this case, we do not need to allocate a new stream object */
      /* since the caller is responsible for closing it himself       */
      جریان         = آرگومانها->جریان;
      جریان->حافظه = حافظه;
      error          = FT_Err_Ok;
    }

#پایان_اگر

    وإلا
    {
      error = FT_THROW( Invalid_آرگومان );
      إذا ( ( آرگومانها->flags & FT_OPEN_جریان ) && آرگومانها->جریان )
        FT_جریان_Close( آرگومانها->جریان );
    }

    إذا ( !error )
      *astream       = جریان;

  Exit:
    أرجع error;
  }


  FT_پایه_DEF( فراغ )
  FT_جریان_Free( FT_جریان  جریان,
                  FT_Int     external )
  {
    إذا ( جریان )
    {
      FT_حافظه  حافظه = جریان->حافظه;


      FT_جریان_Close( جریان );

      إذا ( !external )
        FT_FREE( جریان );
    }
  }


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#لغو_تعریف  FT_COMPONENT
#تعریف_کن FT_COMPONENT  objs


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
  ft_glyphslot_init( FT_Glyph_جایگاه  جایگاه )
  {
    FT_Driver         driver   = جایگاه->face->driver;
    FT_Driver_Class   clazz    = driver->clazz;
    FT_حافظه         حافظه   = driver->ریشه.حافظه;
    FT_Error          error    = FT_Err_Ok;
    FT_جایگاه_داخلی  داخلی = عدم;


    جایگاه->library = driver->ریشه.library;

    إذا ( FT_NEW( داخلی ) )
      اذهب Exit;

    جایگاه->داخلی = داخلی;

    إذا ( FT_DRIVER_استفاده_میکند_OUTLINES( driver ) )
      error = FT_GlyphLoader_New( حافظه, &داخلی->loader );

    إذا ( !error && clazz->init_جایگاه )
      error = clazz->init_جایگاه( جایگاه );

#اگر_تعریف_شده FT_CONFIG_OPTION_SVG
    /* if SVG table exists, allocate the space in `slot->other` */
    إذا ( جایگاه->face->face_flags & FT_FACE_FLAG_SVG )
    {
      FT_SVG_Document  document = عدم;


      إذا ( FT_NEW( document ) )
        اذهب Exit;
      جایگاه->other = document;
    }
#پایان_اگر

  Exit:
    أرجع error;
  }


  FT_پایه_DEF( فراغ )
  ft_glyphslot_free_bitmap( FT_Glyph_جایگاه  جایگاه )
  {
    إذا ( جایگاه->داخلی && ( جایگاه->داخلی->flags & FT_GLYPH_OWN_BITMAP ) )
    {
      FT_حافظه  حافظه = FT_FACE_حافظه( جایگاه->face );


      FT_FREE( جایگاه->bitmap.میانگیر );
      جایگاه->داخلی->flags &= ~FT_GLYPH_OWN_BITMAP;
    }
    وإلا
    {
      /* assume that the bitmap buffer was stolen or not */
      /* allocated from the heap                         */
      جایگاه->bitmap.میانگیر = عدم;
    }
  }


  /* overflow-resistant presetting of bitmap position and dimensions; */
  /* also check whether the size is too large for rendering           */
  FT_پایه_DEF( FT_Bool )
  ft_glyphslot_preset_bitmap( FT_Glyph_جایگاه      جایگاه,
                              FT_Render_نما    نما,
                              ثابت FT_وکتور*  origin )
  {
    FT_Outline*  outline = &جایگاه->outline;
    FT_Bitmap*   bitmap  = &جایگاه->bitmap;

    FT_Pixel_نما  pixel_نما;

    FT_BBox  cbox, pbox;
    FT_Pos   x_لغزش = 0;
    FT_Pos   y_لغزش = 0;
    FT_Pos   x_چپ, y_بالاترین;
    FT_Pos   عرض, ارتفاع, pitch;
    FT_Bool  ret;


    إذا ( جایگاه->format == FT_GLYPH_FORMAT_SVG )
    {
      FT_Module    module;
      SVG_Service  svg_service;


      module      = FT_Get_Module( جایگاه->library, "ot-svg" );
      svg_service = (SVG_Service)module->clazz->module_interface;

      أرجع (FT_Bool)svg_service->preset_جایگاه( module, جایگاه, FALSE );
    }
    وإلا إذا ( جایگاه->format != FT_GLYPH_FORMAT_OUTLINE )
      أرجع 1;

    إذا ( origin )
    {
      x_لغزش = origin->x;
      y_لغزش = origin->y;
    }

    /* compute the control box, and grid-fit it, */
    /* taking into account the origin shift      */
    FT_Outline_Get_CBox( outline, &cbox );

    /* rough estimate of pixel box */
    pbox.xMin = ( cbox.xMin >> 6 ) + ( x_لغزش >> 6 );
    pbox.yMin = ( cbox.yMin >> 6 ) + ( y_لغزش >> 6 );
    pbox.xMax = ( cbox.xMax >> 6 ) + ( x_لغزش >> 6 );
    pbox.yMax = ( cbox.yMax >> 6 ) + ( y_لغزش >> 6 );

    /* tiny remainder box */
    cbox.xMin = ( cbox.xMin & 63 ) + ( x_لغزش & 63 );
    cbox.yMin = ( cbox.yMin & 63 ) + ( y_لغزش & 63 );
    cbox.xMax = ( cbox.xMax & 63 ) + ( x_لغزش & 63 );
    cbox.yMax = ( cbox.yMax & 63 ) + ( y_لغزش & 63 );

    بدل ( نما )
    {
    حالة FT_RENDER_نما_MONO:
      pixel_نما = FT_PIXEL_نما_MONO;
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
#پایان_اگر

    حالة FT_RENDER_نما_LCD:
      pixel_نما = FT_PIXEL_نما_LCD;
      ft_lcd_padding( &cbox, جایگاه, نما );
      اذهب Adjust;

    حالة FT_RENDER_نما_LCD_V:
      pixel_نما = FT_PIXEL_نما_LCD_V;
      ft_lcd_padding( &cbox, جایگاه, نما );
      اذهب Adjust;

    حالة FT_RENDER_نما_نرمال:
    حالة FT_RENDER_نما_LIGHT:
    افتراضي:
      pixel_نما = FT_PIXEL_نما_خاکستری;
    Adjust:
      pbox.xMin += cbox.xMin >> 6;
      pbox.yMin += cbox.yMin >> 6;
      pbox.xMax += ( cbox.xMax + 63 ) >> 6;
      pbox.yMax += ( cbox.yMax + 63 ) >> 6;
    }

    x_چپ = pbox.xMin;
    y_بالاترین  = pbox.yMax;

    عرض  = pbox.xMax - pbox.xMin;
    ارتفاع = pbox.yMax - pbox.yMin;

    /* Flag the width or height unsuitable for rendering.   */
    /* The limit is based on the ppem value when available. */
    /* FT_Renderer modules should check the return value.   */
    ret = FT_BOOL(     عرض >= 0x10000   ||    ارتفاع >= 0x10000   ||
                   pbox.xMin < -0x1000000 || pbox.xMax >= 0x1000000 ||
                   pbox.yMin < -0x1000000 || pbox.yMax >= 0x1000000 ||
             ( جایگاه->face                                         &&
               ( عرض  > 16 * جایگاه->face->اندازه->metrics.x_ppem ||
                 ارتفاع > 16 * جایگاه->face->اندازه->metrics.y_ppem ) ) );

    إذا ( ret )
      FT_TRACE3(( "ft_glyphslot_preset_bitmap: [%ld %ld %ld %ld]\n",
                  pbox.xMin, pbox.yMin, pbox.xMax, pbox.yMax ));

    بدل ( pixel_نما )
    {
    حالة FT_PIXEL_نما_MONO:
      pitch = ( ( عرض + 15 ) >> 4 ) << 1;
      اكسر;

    حالة FT_PIXEL_نما_LCD:
      عرض *= 3;
      pitch  = FT_PAD_CEIL( عرض, 4 );
      اكسر;

    حالة FT_PIXEL_نما_LCD_V:
      ارتفاع *= 3;
      FALL_THROUGH;

    حالة FT_PIXEL_نما_خاکستری:
    افتراضي:
      pitch = عرض;
    }

    جایگاه->bitmap_چپ = (FT_Int)x_چپ;
    جایگاه->bitmap_بالاترین  = (FT_Int)y_بالاترین;

    bitmap->pixel_نما = (بیعلامت حرف)pixel_نما;
    bitmap->رقم_grays  = 256;
    bitmap->عرض      = (بیعلامت صحيح)عرض;
    bitmap->ردیفها       = (بیعلامت صحيح)ارتفاع;
    bitmap->pitch      = pitch;

    أرجع ret;
  }


  FT_پایه_DEF( فراغ )
  ft_glyphslot_مجموعه_bitmap( FT_Glyph_جایگاه  جایگاه,
                           FT_Byte*      میانگیر )
  {
    ft_glyphslot_free_bitmap( جایگاه );

    جایگاه->bitmap.میانگیر = میانگیر;

    FT_ASSERT( (جایگاه->داخلی->flags & FT_GLYPH_OWN_BITMAP) == 0 );
  }


  FT_پایه_DEF( FT_Error )
  ft_glyphslot_alloc_bitmap( FT_Glyph_جایگاه  جایگاه )
  {
    FT_حافظه  حافظه = FT_FACE_حافظه( جایگاه->face );
    FT_Error   error;


    إذا ( جایگاه->داخلی->flags & FT_GLYPH_OWN_BITMAP )
      FT_FREE( جایگاه->bitmap.میانگیر );
    وإلا
      جایگاه->داخلی->flags |= FT_GLYPH_OWN_BITMAP;

    /* dimensions must be preset */
    FT_MEM_ALLOC_MULT( جایگاه->bitmap.میانگیر,
                       جایگاه->bitmap.ردیفها,
                       جایگاه->bitmap.pitch );
    أرجع error;
  }


  ساكن فراغ
  ft_glyphslot_clear( FT_Glyph_جایگاه  جایگاه )
  {
    /* free bitmap if needed */
    ft_glyphslot_free_bitmap( جایگاه );

    /* clear all public fields in the glyph slot */
    جایگاه->glyph_شاخص = 0;

    FT_ZERO( &جایگاه->metrics );
    FT_ZERO( &جایگاه->outline );

    جایگاه->bitmap.عرض      = 0;
    جایگاه->bitmap.ردیفها       = 0;
    جایگاه->bitmap.pitch      = 0;
    جایگاه->bitmap.pixel_نما = 0;
    /* `slot->bitmap.buffer' has been handled by ft_glyphslot_free_bitmap */

    جایگاه->bitmap_چپ   = 0;
    جایگاه->bitmap_بالاترین    = 0;
    جایگاه->رقم_subglyphs = 0;
    جایگاه->subglyphs     = عدم;
    جایگاه->control_داده  = عدم;
    جایگاه->control_len   = 0;

#اگر_تعریف_نشده FT_CONFIG_OPTION_SVG
    جایگاه->other = عدم;
#وإلا
    إذا ( !( جایگاه->face->face_flags & FT_FACE_FLAG_SVG ) )
      جایگاه->other = عدم;
    وإلا
    {
      إذا ( جایگاه->داخلی->flags & FT_GLYPH_OWN_GZIP_SVG )
      {
        FT_حافظه        حافظه = جایگاه->face->حافظه;
        FT_SVG_Document  doc    = (FT_SVG_Document)جایگاه->other;


        FT_FREE( doc->svg_document );
        جایگاه->داخلی->flags &= ~FT_GLYPH_OWN_GZIP_SVG;
      }
    }
#پایان_اگر

    جایگاه->format = FT_GLYPH_FORMAT_NONE;

    جایگاه->خطی_Hori_Advance = 0;
    جایگاه->خطی_Vert_Advance = 0;
    جایگاه->advance.x         = 0;
    جایگاه->advance.y         = 0;
    جایگاه->lsb_delta         = 0;
    جایگاه->rsb_delta         = 0;
  }


  ساكن فراغ
  ft_glyphslot_انجامشده( FT_Glyph_جایگاه  جایگاه )
  {
    FT_Driver        driver = جایگاه->face->driver;
    FT_Driver_Class  clazz  = driver->clazz;
    FT_حافظه        حافظه = driver->ریشه.حافظه;

#اگر_تعریف_شده FT_CONFIG_OPTION_SVG
    إذا ( جایگاه->face->face_flags & FT_FACE_FLAG_SVG )
    {
      /* Free memory in case SVG was there.                          */
      /* `slot->internal` might be NULL in out-of-memory situations. */
      إذا ( جایگاه->داخلی && جایگاه->داخلی->flags & FT_GLYPH_OWN_GZIP_SVG )
      {
        FT_SVG_Document  doc = (FT_SVG_Document)جایگاه->other;


        FT_FREE( doc->svg_document );

        جایگاه->داخلی->flags &= ~FT_GLYPH_OWN_GZIP_SVG;
      }

      FT_FREE( جایگاه->other );
    }
#پایان_اگر

    إذا ( clazz->انجامشده_جایگاه )
      clazz->انجامشده_جایگاه( جایگاه );

    /* free bitmap buffer if needed */
    ft_glyphslot_free_bitmap( جایگاه );

    /* slot->internal might be NULL in out-of-memory situations */
    إذا ( جایگاه->داخلی )
    {
      /* free glyph loader */
      إذا ( FT_DRIVER_استفاده_میکند_OUTLINES( driver ) )
      {
        FT_Glyph_Loader_انجامشده( جایگاه->داخلی->loader );
        جایگاه->داخلی->loader = عدم;
      }

      FT_FREE( جایگاه->داخلی );
    }
  }


  /* documentation is in ftobjs.h */

  FT_پایه_DEF( FT_Error )
  FT_New_Glyph_جایگاه( FT_Face        face,
                    FT_Glyph_جایگاه  *aslot )
  {
    FT_Error         error;
    FT_Driver        driver;
    FT_Driver_Class  clazz;
    FT_حافظه        حافظه;
    FT_Glyph_جایگاه     جایگاه = عدم;


    إذا ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );

    إذا ( !face->driver )
      أرجع FT_THROW( Invalid_آرگومان );

    driver = face->driver;
    clazz  = driver->clazz;
    حافظه = driver->ریشه.حافظه;

    FT_TRACE4(( "FT_New_GlyphSlot: Creating new slot object\n" ));
    إذا ( !FT_ALLOC( جایگاه, clazz->جایگاه_object_اندازه ) )
    {
      جایگاه->face = face;

      error = ft_glyphslot_init( جایگاه );
      إذا ( error )
      {
        ft_glyphslot_انجامشده( جایگاه );
        FT_FREE( جایگاه );
        اذهب Exit;
      }

      جایگاه->بعدی  = face->glyph;
      face->glyph = جایگاه;

      إذا ( aslot )
        *aslot = جایگاه;
    }
    وإلا إذا ( aslot )
      *aslot = عدم;


  Exit:
    FT_TRACE4(( "FT_New_GlyphSlot: Return 0x%x\n", error ));

    أرجع error;
  }


  /* documentation is in ftobjs.h */

  FT_پایه_DEF( فراغ )
  FT_انجامشده_Glyph_جایگاه( FT_Glyph_جایگاه  جایگاه )
  {
    إذا ( جایگاه )
    {
      FT_Driver     driver = جایگاه->face->driver;
      FT_حافظه     حافظه = driver->ریشه.حافظه;
      FT_Glyph_جایگاه  prev;
      FT_Glyph_جایگاه  cur;


      /* Remove slot from its parent face's list */
      prev = عدم;
      cur  = جایگاه->face->glyph;

      طالما ( cur )
      {
        إذا ( cur == جایگاه )
        {
          إذا ( !prev )
            جایگاه->face->glyph = cur->بعدی;
          وإلا
            prev->بعدی = cur->بعدی;

          /* finalize client-specific data */
          إذا ( جایگاه->generic.finalizer )
            جایگاه->generic.finalizer( جایگاه );

          ft_glyphslot_انجامشده( جایگاه );
          FT_FREE( جایگاه );
          اكسر;
        }
        prev = cur;
        cur  = cur->بعدی;
      }
    }
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( فراغ )
  FT_مجموعه_Transform( FT_Face     face,
                    FT_ماتریس*  ماتریس,
                    FT_وکتور*  delta )
  {
    FT_Face_داخلی  داخلی;


    إذا ( !face )
      أرجع;

    داخلی = face->داخلی;

    داخلی->transform_flags = 0;

    إذا ( !ماتریس || !FT_ماتریس_Check( ماتریس ) )
    {
      داخلی->transform_ماتریس.xx = 0x10000L;
      داخلی->transform_ماتریس.xy = 0;
      داخلی->transform_ماتریس.yx = 0;
      داخلی->transform_ماتریس.yy = 0x10000L;

      ماتریس = &داخلی->transform_ماتریس;
    }
    وإلا
      داخلی->transform_ماتریس = *ماتریس;

    /* set transform_flags bit flag 0 if `matrix' isn't the identity */
    إذا ( ( ماتریس->xy | ماتریس->yx ) ||
         ماتریس->xx != 0x10000L      ||
         ماتریس->yy != 0x10000L      )
      داخلی->transform_flags |= 1;

    إذا ( !delta )
    {
      داخلی->transform_delta.x = 0;
      داخلی->transform_delta.y = 0;

      delta = &داخلی->transform_delta;
    }
    وإلا
      داخلی->transform_delta = *delta;

    /* set transform_flags bit flag 1 if `delta' isn't the null vector */
    إذا ( delta->x | delta->y )
      داخلی->transform_flags |= 2;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( فراغ )
  FT_Get_Transform( FT_Face     face,
                    FT_ماتریس*  ماتریس,
                    FT_وکتور*  delta )
  {
    FT_Face_داخلی  داخلی;


    إذا ( !face )
      أرجع;

    داخلی = face->داخلی;

    إذا ( ماتریس )
      *ماتریس = داخلی->transform_ماتریس;

    إذا ( delta )
      *delta = داخلی->transform_delta;
  }


  ساكن FT_Renderer
  ft_lookup_glyph_renderer( FT_Glyph_جایگاه  جایگاه );


#اگر_تعریف_شده GRID_FIT_METRICS
  ساكن فراغ
  ft_glyphslot_grid_fit_metrics( FT_Glyph_جایگاه  جایگاه,
                                 FT_Bool       vertical )
  {
    FT_Glyph_Metrics*  metrics = &جایگاه->metrics;
    FT_Pos             راست, bottom;


    إذا ( vertical )
    {
      metrics->horiBearingX = FT_PIX_FLOOR( metrics->horiBearingX );
      metrics->horiBearingY = FT_PIX_CEIL_LONG( metrics->horiBearingY );

      راست  = FT_PIX_CEIL_LONG( جمع_کن_LONG( metrics->vertBearingX,
                                           metrics->عرض ) );
      bottom = FT_PIX_CEIL_LONG( جمع_کن_LONG( metrics->vertBearingY,
                                           metrics->ارتفاع ) );

      metrics->vertBearingX = FT_PIX_FLOOR( metrics->vertBearingX );
      metrics->vertBearingY = FT_PIX_FLOOR( metrics->vertBearingY );

      metrics->عرض  = SUB_LONG( راست,
                                  metrics->vertBearingX );
      metrics->ارتفاع = SUB_LONG( bottom,
                                  metrics->vertBearingY );
    }
    وإلا
    {
      metrics->vertBearingX = FT_PIX_FLOOR( metrics->vertBearingX );
      metrics->vertBearingY = FT_PIX_FLOOR( metrics->vertBearingY );

      راست  = FT_PIX_CEIL_LONG( جمع_کن_LONG( metrics->horiBearingX,
                                           metrics->عرض ) );
      bottom = FT_PIX_FLOOR( SUB_LONG( metrics->horiBearingY,
                                       metrics->ارتفاع ) );

      metrics->horiBearingX = FT_PIX_FLOOR( metrics->horiBearingX );
      metrics->horiBearingY = FT_PIX_CEIL_LONG( metrics->horiBearingY );

      metrics->عرض  = SUB_LONG( راست,
                                  metrics->horiBearingX );
      metrics->ارتفاع = SUB_LONG( metrics->horiBearingY,
                                  bottom );
    }

    metrics->horiAdvance = FT_PIX_دور_LONG( metrics->horiAdvance );
    metrics->vertAdvance = FT_PIX_دور_LONG( metrics->vertAdvance );
  }
#پایان_اگر /* GRID_FIT_METRICS */


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_بارگیری_Glyph( FT_Face   face,
                 FT_UInt   glyph_شاخص,
                 FT_Int32  بارگیری_flags )
  {
    FT_Error      error;
    FT_Driver     driver;
    FT_Glyph_جایگاه  جایگاه;
    FT_Library    library;
    FT_Bool       autohint = FALSE;
    FT_Module     hinter;


    إذا ( !face || !face->اندازه || !face->glyph )
      أرجع FT_THROW( Invalid_Face_Handle );

    /* The validity test for `glyph_index' is performed by the */
    /* font drivers.                                           */

    جایگاه = face->glyph;
    ft_glyphslot_clear( جایگاه );

    driver  = face->driver;
    library = driver->ریشه.library;
    hinter  = library->auto_hinter;

    /* undefined scale means no scale */
    إذا ( face->اندازه->metrics.x_ppem == 0 ||
         face->اندازه->metrics.y_ppem == 0 )
      بارگیری_flags |= FT_بارگیری_NO_SCALE;

    /* resolve load flags dependencies */

    إذا ( بارگیری_flags & FT_بارگیری_NO_RECURSE )
      بارگیری_flags |= FT_بارگیری_NO_SCALE         |
                    FT_بارگیری_IGNORE_TRANSFORM;

    إذا ( بارگیری_flags & FT_بارگیری_NO_SCALE )
    {
      بارگیری_flags |= FT_بارگیری_NO_HINTING |
                    FT_بارگیری_NO_BITMAP;

      بارگیری_flags &= ~FT_بارگیری_RENDER;
    }

    إذا ( بارگیری_flags & FT_بارگیری_BITMAP_METRICS_ONLY )
      بارگیری_flags &= ~FT_بارگیری_RENDER;

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
         !( بارگیری_flags & FT_بارگیری_NO_HINTING )             &&
         !( بارگیری_flags & FT_بارگیری_NO_AUTOHINT )            &&
         FT_IS_SCALABLE( face )                           &&
         !FT_IS_TRICKY( face )                            &&
         ( ( بارگیری_flags & FT_بارگیری_IGNORE_TRANSFORM )    ||
           ( face->داخلی->transform_ماتریس.yx == 0 &&
             face->داخلی->transform_ماتریس.xx != 0 ) ||
           ( face->داخلی->transform_ماتریس.xx == 0 &&
             face->داخلی->transform_ماتریس.yx != 0 ) ) )
    {
      إذا ( ( بارگیری_flags & FT_بارگیری_FORCE_AUTOHINT ) ||
           !FT_DRIVER_HAS_HINTER( driver )         )
        autohint = TRUE;
      وإلا
      {
        FT_Render_نما  نما = FT_بارگیری_هدف_نما( بارگیری_flags );
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
        إذا ( ( نما == FT_RENDER_نما_LIGHT           &&
               ( !FT_DRIVER_HINTS_LIGHTLY( driver ) &&
                 !is_light_type1                    ) )         ||
             ( FT_IS_SFNT( face )                             &&
               ttface->رقم_locations                          &&
               ttface->font_برنامه_اندازه == 0                 &&
               ttface->cvt_برنامه_اندازه <= 7                  ) )
          autohint = TRUE;
      }
    }

#اگر_تعریف_شده TT_CONFIG_OPTION_VARC
    /* Never auto-hint a VARC (variable composite) glyph: its components */
    /* are already loaded unhinted, and the auto-fitter cannot process   */
    /* the assembled composite (it also re-enters variation-coordinate   */
    /* setting).  Use the native hinter / unhinted load instead.         */
    إذا ( autohint && FT_IS_SFNT( face ) )
    {
      FT_Service_VARC  varc;


      FT_FACE_پیدا_کن_SERVICE( face, varc, VARC );
      إذا ( varc && varc->has_glyph( face, glyph_شاخص ) )
        autohint = FALSE;
    }
#پایان_اگر

    إذا ( autohint )
    {
      FT_AutoHinter_Interface  hinting;


      /* XXX: The use of the `FT_LOAD_XXX_ONLY` flags is not very */
      /*      elegant.                                            */

      /* try to load SVG documents if available */
      إذا ( ( بارگیری_flags & FT_بارگیری_NO_SVG ) == 0 &&
           FT_HAS_SVG( face )                   )
      {
        error = driver->clazz->بارگیری_glyph( جایگاه, face->اندازه,
                                           glyph_شاخص,
                                           بارگیری_flags | FT_بارگیری_SVG_ONLY );

        إذا ( !error && جایگاه->format == FT_GLYPH_FORMAT_SVG )
          اذهب بارگیری_Ok;
      }

      /* try to load embedded bitmaps if available */
      إذا ( FT_HAS_FIXED_SIZES( face )              &&
           ( بارگیری_flags & FT_بارگیری_NO_BITMAP ) == 0 )
      {
        error = driver->clazz->بارگیری_glyph( جایگاه, face->اندازه,
                                           glyph_شاخص,
                                           بارگیری_flags | FT_بارگیری_SBITS_ONLY );

        إذا ( !error && جایگاه->format == FT_GLYPH_FORMAT_BITMAP )
          اذهب بارگیری_Ok;
      }

      {
        FT_Face_داخلی  داخلی        = face->داخلی;
        FT_Int            transform_flags = داخلی->transform_flags;


        /* since the auto-hinter calls FT_Load_Glyph by itself, */
        /* make sure that glyphs aren't transformed             */
        داخلی->transform_flags = 0;

        /* load auto-hinted outline */
        hinting = (FT_AutoHinter_Interface)hinter->clazz->module_interface;

        error   = hinting->بارگیری_glyph( (FT_AutoHinter)hinter,
                                       جایگاه, face->اندازه,
                                       glyph_شاخص, بارگیری_flags );

        داخلی->transform_flags = transform_flags;
      }
    }
    وإلا
    {
      error = driver->clazz->بارگیری_glyph( جایگاه,
                                         face->اندازه,
                                         glyph_شاخص,
                                         بارگیری_flags );
      إذا ( error )
        اذهب Exit;

      إذا ( جایگاه->format == FT_GLYPH_FORMAT_OUTLINE )
      {
        /* check that the loaded outline is correct */
        error = FT_Outline_Check( &جایگاه->outline );
        إذا ( error )
          اذهب Exit;

#اگر_تعریف_شده GRID_FIT_METRICS
        إذا ( !( بارگیری_flags & FT_بارگیری_NO_HINTING ) )
          ft_glyphslot_grid_fit_metrics(
            جایگاه,
            FT_BOOL( بارگیری_flags & FT_بارگیری_VERTICAL_LAYOUT ) );
#پایان_اگر
      }
    }

  بارگیری_Ok:
    /* compute the advance */
    إذا ( بارگیری_flags & FT_بارگیری_VERTICAL_LAYOUT )
    {
      جایگاه->advance.x = 0;
      جایگاه->advance.y = جایگاه->metrics.vertAdvance;
    }
    وإلا
    {
      جایگاه->advance.x = جایگاه->metrics.horiAdvance;
      جایگاه->advance.y = 0;
    }

    /* compute the linear advance in 16.16 pixels */
    إذا ( ( بارگیری_flags & FT_بارگیری_خطی_DESIGN ) == 0 &&
         FT_IS_SCALABLE( face )                      )
    {
      FT_اندازه_Metrics*  metrics = &face->اندازه->metrics;


      /* it's tricky! */
      جایگاه->خطی_Hori_Advance = FT_MulDiv( جایگاه->خطی_Hori_Advance,
                                           metrics->x_scale, 64 );

      جایگاه->خطی_Vert_Advance = FT_MulDiv( جایگاه->خطی_Vert_Advance,
                                           metrics->y_scale, 64 );
    }

    إذا ( ( بارگیری_flags & FT_بارگیری_IGNORE_TRANSFORM ) == 0 )
    {
      FT_Face_داخلی  داخلی = face->داخلی;


      /* now, transform the glyph image if needed */
      إذا ( داخلی->transform_flags )
      {
        /* get renderer */
        FT_Renderer  renderer = ft_lookup_glyph_renderer( جایگاه );


        إذا ( renderer )
          error = renderer->clazz->transform_glyph(
                                     renderer, جایگاه,
                                     &داخلی->transform_ماتریس,
                                     &داخلی->transform_delta );
        وإلا إذا ( جایگاه->format == FT_GLYPH_FORMAT_OUTLINE )
        {
          /* apply `standard' transformation if no renderer is available */
          إذا ( داخلی->transform_flags & 1 )
            FT_Outline_Transform( &جایگاه->outline,
                                  &داخلی->transform_ماتریس );

          إذا ( داخلی->transform_flags & 2 )
            FT_Outline_Translate( &جایگاه->outline,
                                  داخلی->transform_delta.x,
                                  داخلی->transform_delta.y );
        }

        /* transform advance */
        FT_وکتور_Transform( &جایگاه->advance, &داخلی->transform_ماتریس );
      }
    }

    جایگاه->glyph_شاخص          = glyph_شاخص;
    جایگاه->داخلی->بارگیری_flags = بارگیری_flags;

    /* do we need to render the image or preset the bitmap now? */
    إذا ( !error                                    &&
         ( بارگیری_flags & FT_بارگیری_NO_SCALE ) == 0    &&
         جایگاه->format != FT_GLYPH_FORMAT_BITMAP    &&
         جایگاه->format != FT_GLYPH_FORMAT_مرکب )
    {
      FT_Render_نما  نما = FT_بارگیری_هدف_نما( بارگیری_flags );


      إذا ( نما == FT_RENDER_نما_نرمال   &&
           بارگیری_flags & FT_بارگیری_MONOCHROME )
        نما = FT_RENDER_نما_MONO;

      إذا ( بارگیری_flags & FT_بارگیری_RENDER )
        error = FT_Render_Glyph( جایگاه, نما );
      وإلا
        ft_glyphslot_preset_bitmap( جایگاه, نما, عدم );
    }

#اگر_تعریف_شده FT_DEBUG_LEVEL_TRACE
    FT_TRACE5(( "FT_Load_Glyph: index %u, flags 0x%x\n",
                glyph_شاخص, بارگیری_flags ));
    FT_TRACE5(( "  bitmap %ux%u %s, %s (mode %d)\n",
                جایگاه->bitmap.عرض,
                جایگاه->bitmap.ردیفها,
                جایگاه->outline.points ?
                  جایگاه->bitmap.میانگیر ? "rendered"
                                      : "preset"
                                     :
                  جایگاه->داخلی->flags & FT_GLYPH_OWN_BITMAP ? "owned"
                                                              : "unowned",
                pixel_modes[جایگاه->bitmap.pixel_نما],
                جایگاه->bitmap.pixel_نما ));
    FT_TRACE5(( "\n" ));
    FT_TRACE5(( "  x advance: %f\n", (مزدوج)جایگاه->advance.x / 64 ));
    FT_TRACE5(( "  y advance: %f\n", (مزدوج)جایگاه->advance.y / 64 ));
    FT_TRACE5(( "  linear x advance: %f\n",
                (مزدوج)جایگاه->خطی_Hori_Advance / 65536 ));
    FT_TRACE5(( "  linear y advance: %f\n",
                (مزدوج)جایگاه->خطی_Vert_Advance / 65536 ));

    {
      FT_Glyph_Metrics*  metrics = &جایگاه->metrics;


      FT_TRACE5(( "  metrics:\n" ));
      FT_TRACE5(( "    width:  %f\n", (مزدوج)metrics->عرض / 64 ));
      FT_TRACE5(( "    height: %f\n", (مزدوج)metrics->ارتفاع / 64 ));
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
#پایان_اگر

  Exit:
    أرجع error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_بارگیری_Char( FT_Face   face,
                FT_ULong  char_کد,
                FT_Int32  بارگیری_flags )
  {
    FT_UInt  glyph_شاخص;


    إذا ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );

    glyph_شاخص = (FT_UInt)char_کد;
    إذا ( face->charmap )
      glyph_شاخص = FT_Get_Char_شاخص( face, char_کد );

    أرجع FT_بارگیری_Glyph( face, glyph_شاخص, بارگیری_flags );
  }


  /* destructor for sizes list */
  ساكن فراغ
  destroy_اندازه( FT_حافظه  حافظه,
                فراغ*      اندازه_2,
                فراغ*      driver_ )
  {
    FT_اندازه    اندازه   = (FT_اندازه)اندازه_2;
    FT_Driver  driver = (FT_Driver)driver_;


    /* finalize format-specific stuff */
    إذا ( driver->clazz->انجامشده_اندازه )
      driver->clazz->انجامشده_اندازه( اندازه );

    /* finalize client-specific data */
    إذا ( اندازه->generic.finalizer )
      اندازه->generic.finalizer( اندازه );

    FT_FREE( اندازه->داخلی );
    FT_FREE( اندازه );
  }


  ساكن فراغ
  ft_cmap_انجامشده_داخلی( FT_CMap  cmap );


  ساكن فراغ
  destroy_charmaps( FT_Face    face,
                    FT_حافظه  حافظه )
  {
    FT_Int  n;


    إذا ( !face )
      أرجع;

    لكل ( n = 0; n < face->رقم_charmaps; n++ )
    {
      FT_CMap  cmap = FT_CMAP( face->charmaps[n] );


      ft_cmap_انجامشده_داخلی( cmap );

      face->charmaps[n] = عدم;
    }

    FT_FREE( face->charmaps );
    face->رقم_charmaps = 0;
  }


  /* destructor for faces list */
  ساكن فراغ
  destroy_face( FT_حافظه  حافظه,
                فراغ*      face_,
                فراغ*      driver_ )
  {
    FT_Face          face   = (FT_Face)face_;
    FT_Driver        driver = (FT_Driver)driver_;
    FT_Driver_Class  clazz  = driver->clazz;


    /* discard auto-hinting data */
    إذا ( face->autohint.finalizer )
      face->autohint.finalizer( face->autohint.داده );

    /* Discard glyph slots for this face.                           */
    /* Beware!  FT_Done_GlyphSlot() changes the field `face->glyph' */
    طالما ( face->glyph )
      FT_انجامشده_Glyph_جایگاه( face->glyph );

    /* discard all sizes for this face */
    FT_List_Finalize( &face->sizes_list,
                      destroy_اندازه,
                      حافظه,
                      driver );
    face->اندازه = عدم;

    /* discard charmaps */
    destroy_charmaps( face, حافظه );

    /* finalize format-specific stuff */
    إذا ( clazz->انجامشده_face )
      clazz->انجامشده_face( face );

    /* close the stream for this face if needed */
    FT_جریان_Free(
      face->جریان,
      ( face->face_flags & FT_FACE_FLAG_EXTERNAL_جریان ) != 0 );

    face->جریان = عدم;

    /* now discard client data */
    إذا ( face->generic.finalizer )
      face->generic.finalizer( face );

    /* get rid of it */
    إذا ( face->داخلی )
    {
      FT_FREE( face->داخلی );
    }
    FT_FREE( face );
  }


  ساكن فراغ
  Destroy_Driver( FT_Driver  driver )
  {
    FT_List_Finalize( &driver->faces_list,
                      destroy_face,
                      driver->ریشه.حافظه,
                      driver );
  }


  /* documentation is in ftobjs.h */

  FT_پایه_DEF( FT_Error )
  پیدا_کن_unicode_charmap( FT_Face  face )
  {
    FT_CharMap*  اولی;
    FT_CharMap*  cur;


    /* caller should have already checked that `face' is valid */
    FT_ASSERT( face );

    اولی = face->charmaps;

    إذا ( !اولی )
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

    cur = اولی + face->رقم_charmaps;  /* points after the last one */

    لكل ( ; --cur >= اولی; )
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
               cur[0]->encoding_id == TT_APPLE_ID_پر_UNICODE  &&
               FT_Get_CMap_Format( cur[0] ) == 13               ) )
        {
          face->charmap = cur[0];
          أرجع FT_Err_Ok;
        }
      }
    }

    /* We do not have any UCS-4 charmap.                */
    /* Do the loop again and search for UCS-2 charmaps. */
    cur = اولی + face->رقم_charmaps;

    لكل ( ; --cur >= اولی; )
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
  پیدا_کن_variant_selector_charmap( FT_Face  face )
  {
    FT_CharMap*  اولی;
    FT_CharMap*  انتها;
    FT_CharMap*  cur;


    /* caller should have already checked that `face' is valid */
    FT_ASSERT( face );

    اولی = face->charmaps;

    إذا ( !اولی )
      أرجع عدم;

    انتها = اولی + face->رقم_charmaps;  /* points after the last one */

    لكل ( cur = اولی; cur < انتها; cur++ )
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
             FT_جریان      *astream,
             FT_Bool        *anexternal_جریان,
             FT_Long        face_شاخص,
             FT_Int         رقم_params,
             FT_Parameter*  params,
             FT_Face       *aface )
  {
    FT_حافظه         حافظه;
    FT_Driver_Class   clazz;
    FT_Face           face     = عدم;
    FT_Face_داخلی  داخلی = عدم;

    FT_Error          error, error2;


    clazz  = driver->clazz;
    حافظه = driver->ریشه.حافظه;

    /* allocate the face object and perform basic initialization */
    إذا ( FT_ALLOC( face, clazz->face_object_اندازه ) )
      اذهب Fail;

    face->driver = driver;
    face->حافظه = حافظه;
    face->جریان = *astream;

    /* set the FT_FACE_FLAG_EXTERNAL_STREAM bit for FT_Done_Face */
    إذا ( *anexternal_جریان )
      face->face_flags |= FT_FACE_FLAG_EXTERNAL_جریان;

    إذا ( FT_NEW( داخلی ) )
      اذهب Fail;

    face->داخلی = داخلی;

#اگر_تعریف_شده FT_CONFIG_OPTION_INCREMENTAL
    {
      صحيح  i;


      face->داخلی->incremental_interface = عدم;
      لكل ( i = 0; i < رقم_params && !face->داخلی->incremental_interface;
            i++ )
        إذا ( params[i].برچسب == FT_PARAM_برچسب_INCREMENTAL )
          face->داخلی->incremental_interface =
            (FT_Incremental_Interface)params[i].داده;
    }
#پایان_اگر

    face->داخلی->random_دانه = -1;

    إذا ( clazz->init_face )
      error = clazz->init_face( *astream,
                                face,
                                (FT_Int)face_شاخص,
                                رقم_params,
                                params );
    /* Stream may have been changed. */
    *astream = face->جریان;
    *anexternal_جریان =
      ( face->face_flags & FT_FACE_FLAG_EXTERNAL_جریان ) != 0;
    إذا ( error )
      اذهب Fail;

    /* select Unicode charmap by default */
    error2 = پیدا_کن_unicode_charmap( face );

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
      destroy_charmaps( face, حافظه );
      إذا ( clazz->انجامشده_face )
        clazz->انجامشده_face( face );
      FT_FREE( داخلی );
      FT_FREE( face );
      *aface = عدم;
    }

    أرجع error;
  }


  /* there's a Mac-specific extended implementation of FT_New_Face() */
  /* in src/base/ftmac.c                                             */

#اگر_تعریف_نشده FT_MACINTOSH

  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_New_Face( FT_Library   library,
               ثابت حرف*  pathname,
               FT_Long      face_شاخص,
               FT_Face     *aface )
  {
    FT_Open_آرگومانها  آرگومانها;


    /* test for valid `library' and `aface' delayed to `FT_Open_Face' */
    إذا ( !pathname )
      أرجع FT_THROW( Invalid_آرگومان );

    آرگومانها.flags    = FT_OPEN_PATHNAME;
    آرگومانها.pathname = (حرف*)pathname;
    آرگومانها.جریان   = عدم;

    أرجع ft_open_face_داخلی( library, &آرگومانها, face_شاخص, aface, 1 );
  }

#پایان_اگر


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_New_حافظه_Face( FT_Library      library,
                      ثابت FT_Byte*  file_پایه,
                      FT_Long         file_اندازه,
                      FT_Long         face_شاخص,
                      FT_Face        *aface )
  {
    FT_Open_آرگومانها  آرگومانها;


    /* test for valid `library' and `face' delayed to `FT_Open_Face' */
    إذا ( !file_پایه )
      أرجع FT_THROW( Invalid_آرگومان );

    آرگومانها.flags       = FT_OPEN_حافظه;
    آرگومانها.حافظه_پایه = file_پایه;
    آرگومانها.حافظه_اندازه = file_اندازه;
    آرگومانها.جریان      = عدم;

    أرجع ft_open_face_داخلی( library, &آرگومانها, face_شاخص, aface, 1 );
  }


#اگر_تعریف_شده FT_CONFIG_OPTION_MAC_FONTS

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
  حافظه_جریان_close( FT_جریان  جریان )
  {
    FT_حافظه  حافظه = (FT_حافظه)جریان->descriptor.اشاره_گر;


    FT_FREE( جریان->پایه );
    جریان->اندازه  = 0;
    جریان->close = عدم;
    FT_FREE( جریان );
  }


  /* Create a new memory stream from a buffer and a size. */
  /* From `ftmac.c'.                                      */
  ساكن FT_Error
  new_حافظه_جریان( FT_Library           library,
                     FT_Byte*             پایه,
                     FT_ULong             اندازه,
                     FT_جریان_Close_Func  close,
                     FT_جریان           *astream )
  {
    FT_Error   error;
    FT_حافظه  حافظه;
    FT_جریان  جریان = عدم;


    إذا ( !library )
      أرجع FT_THROW( Invalid_Library_Handle );

    إذا ( !پایه )
      أرجع FT_THROW( Invalid_آرگومان );

    *astream = عدم;
    حافظه   = library->حافظه;
    إذا ( FT_NEW( جریان ) )
      اذهب Exit;

    FT_جریان_Open_حافظه( جریان, پایه, اندازه );

    جریان->descriptor.اشاره_گر = حافظه;
    جریان->close              = close;

    *astream = جریان;

  Exit:
    أرجع error;
  }


  /* Create a new FT_Face given a buffer and a driver name. */
  /* From `ftmac.c'.                                        */
  FT_LOCAL_DEF( FT_Error )
  open_face_from_میانگیر( FT_Library   library,
                         FT_Byte*     پایه,
                         FT_ULong     اندازه,
                         FT_Long      face_شاخص,
                         ثابت حرف*  driver_name,
                         FT_Face     *aface )
  {
    FT_Open_آرگومانها  آرگومانها;
    FT_Error      error;
    FT_حافظه     حافظه = library->حافظه;


    آرگومانها.driver = عدم;
    آرگومانها.flags  = 0;

    إذا ( driver_name )
    {
      آرگومانها.driver = FT_Get_Module( library, driver_name );
      إذا ( !آرگومانها.driver )
      {
        FT_FREE( پایه );
        أرجع FT_THROW( Missing_Module );
      }

      آرگومانها.flags = آرگومانها.flags | FT_OPEN_DRIVER;
    }

    /* `memory_stream_close` also frees the stream object. */
    error = new_حافظه_جریان( library,
                               پایه,
                               اندازه,
                               حافظه_جریان_close,
                               &آرگومانها.جریان );
    إذا ( error )
    {
      FT_FREE( پایه );
      أرجع error;
    }

    آرگومانها.flags |= FT_OPEN_جریان;

#اگر_تعریف_شده FT_MACINTOSH
    /* At this point, the face index has served its purpose;  */
    /* whoever calls this function has already used it to     */
    /* locate the correct font data.  We should not propagate */
    /* this index to FT_Open_Face() (unless it is negative).  */

    إذا ( face_شاخص > 0 )
      face_شاخص &= 0x7FFF0000L; /* retain GX data */
#پایان_اگر

    أرجع ft_open_face_داخلی( library, &آرگومانها, face_شاخص, aface, 0 );
  }


  /* Look up `TYP1' or `CID ' table from sfnt table directory.       */
  /* `offset' and `length' must exclude the binary header in tables. */

  /* Type 1 and CID-keyed font drivers should recognize sfnt-wrapped */
  /* format too.  Here, since we can't expect that the TrueType font */
  /* driver is loaded unconditionally, we must parse the font by     */
  /* ourselves.  We are only interested in the name of the table and */
  /* the offset.                                                     */

  ساكن FT_Error
  ft_lookup_PS_درون_sfnt_جریان( FT_جریان  جریان,
                               FT_Long    face_شاخص,
                               FT_ULong*  جابه_جایی,
                               FT_ULong*  درازی,
                               FT_Bool*   is_sfnt_cid )
  {
    FT_Error   error;
    FT_UShort  رقم_Tables;
    FT_Long    pstable_شاخص;
    FT_ULong   برچسب;
    صحيح        i;


    *جابه_جایی = 0;
    *درازی = 0;
    *is_sfnt_cid = FALSE;

    /* TODO: support for sfnt-wrapped PS/CID in TTC format */

    /* version check for 'typ1' (should be ignored?) */
    إذا ( FT_READ_ULONG( برچسب ) )
      أرجع error;
    إذا ( برچسب != TTAG_typ1 )
      أرجع FT_THROW( Unknown_File_Format );

    إذا ( FT_READ_USHORT( رقم_Tables ) )
      أرجع error;
    إذا ( FT_جریان_SKIP( 2 * 3 ) ) /* skip binary search header */
      أرجع error;

    pstable_شاخص = -1;
    *is_sfnt_cid  = FALSE;

    لكل ( i = 0; i < رقم_Tables; i++ )
    {
      إذا ( FT_READ_ULONG( برچسب )     || FT_جریان_SKIP( 4 )      ||
           FT_READ_ULONG( *جابه_جایی ) || FT_READ_ULONG( *درازی ) )
        أرجع error;

      إذا ( برچسب == TTAG_CID )
      {
        pstable_شاخص++;
        *جابه_جایی += 22;
        *درازی -= 22;
        *is_sfnt_cid = TRUE;
        إذا ( face_شاخص < 0 )
          أرجع FT_Err_Ok;
      }
      وإلا إذا ( برچسب == TTAG_TYP1 )
      {
        pstable_شاخص++;
        *جابه_جایی += 24;
        *درازی -= 24;
        *is_sfnt_cid = FALSE;
        إذا ( face_شاخص < 0 )
          أرجع FT_Err_Ok;
      }
      إذا ( face_شاخص >= 0 && pstable_شاخص == face_شاخص )
        أرجع FT_Err_Ok;
    }

    أرجع FT_THROW( جدول_Missing );
  }


  FT_LOCAL_DEF( FT_Error )
  open_face_PS_from_sfnt_جریان( FT_Library     library,
                                 FT_جریان      جریان,
                                 FT_Long        face_شاخص,
                                 FT_Int         رقم_params,
                                 FT_Parameter  *params,
                                 FT_Face       *aface )
  {
    FT_Error   error;
    FT_حافظه  حافظه = library->حافظه;
    FT_ULong   جابه_جایی, درازی;
    FT_ULong   pos;
    FT_Bool    is_sfnt_cid;
    FT_Byte*   sfnt_ps = عدم;

    FT_استفادهنشده( رقم_params );
    FT_استفادهنشده( params );


    /* ignore GX stuff */
    إذا ( face_شاخص > 0 )
      face_شاخص &= 0xFFFFL;

    pos = FT_جریان_POS();

    error = ft_lookup_PS_درون_sfnt_جریان( جریان,
                                         face_شاخص,
                                         &جابه_جایی,
                                         &درازی,
                                         &is_sfnt_cid );
    إذا ( error )
      اذهب Exit;

    إذا ( جابه_جایی > جریان->اندازه )
    {
      FT_TRACE2(( "open_face_PS_from_sfnt_stream: invalid table offset\n" ));
      error = FT_THROW( Invalid_جدول );
      اذهب Exit;
    }
    وإلا إذا ( درازی > جریان->اندازه - جابه_جایی )
    {
      FT_TRACE2(( "open_face_PS_from_sfnt_stream: invalid table length\n" ));
      error = FT_THROW( Invalid_جدول );
      اذهب Exit;
    }

    error = FT_جریان_Seek( جریان, pos + جابه_جایی );
    إذا ( error )
      اذهب Exit;

    إذا ( FT_QALLOC( sfnt_ps, (FT_Long)درازی ) )
      اذهب Exit;

    error = FT_جریان_Read( جریان, (FT_Byte *)sfnt_ps, درازی );
    إذا ( error )
    {
      FT_FREE( sfnt_ps );
      اذهب Exit;
    }

    error = open_face_from_میانگیر( library,
                                   sfnt_ps,
                                   درازی,
                                   FT_MIN( face_شاخص, 0 ),
                                   is_sfnt_cid ? "t1cid" : "type1",
                                   aface );
  Exit:
    {
      FT_Error  error1;


      إذا ( FT_ERR_EQ( error, Unknown_File_Format ) )
      {
        error1 = FT_جریان_Seek( جریان, pos );
        إذا ( error1 )
          أرجع error1;
      }

      أرجع error;
    }
  }


#اگر_تعریف_نشده FT_MACINTOSH

  /* The resource header says we've got resource_cnt `POST' (type1) */
  /* resources in this file.  They all need to be coalesced into    */
  /* one lump which gets passed on to the type1 driver.             */
  /* Here can be only one PostScript font in a file so face_index   */
  /* must be 0 (or -1).                                             */
  /*                                                                */
  ساكن FT_Error
  Mac_Read_POST_Resource( FT_Library  library,
                          FT_جریان   جریان,
                          FT_Long    *offsets,
                          FT_Long     resource_cnt,
                          FT_Long     face_شاخص,
                          FT_Face    *aface )
  {
    FT_Error   error  = FT_ERR( Cannot_Open_Resource );
    FT_حافظه  حافظه = library->حافظه;

    FT_Byte*   pfb_داده = عدم;
    صحيح        i, type, flags;
    FT_ULong   len;
    FT_ULong   pfb_len, pfb_pos, pfb_lenpos;
    FT_ULong   rlen, موقت;


    إذا ( face_شاخص == -1 )
      face_شاخص = 0;
    إذا ( face_شاخص != 0 )
      أرجع error;

    /* Find the length of all the POST resources, concatenated.  Assume */
    /* worst case (each resource in its own section).                   */
    pfb_len = 0;
    لكل ( i = 0; i < resource_cnt; i++ )
    {
      error = FT_جریان_Seek( جریان, (FT_ULong)offsets[i] );
      إذا ( error )
        اذهب Exit;
      إذا ( FT_READ_ULONG( موقت ) )  /* actually LONG */
        اذهب Exit;

      /* FT2 allocator takes signed long buffer length,
       * too large value causing overflow should be checked
       */
      FT_TRACE4(( "                 POST fragment #%d: length=0x%08lx"
                  " total pfb_len=0x%08lx\n",
                  i, موقت, pfb_len + موقت + 6 ));

      إذا ( FT_MAC_RFORK_MAX_LEN < موقت               ||
           FT_MAC_RFORK_MAX_LEN - موقت < pfb_len + 6 )
      {
        FT_TRACE2(( "             MacOS resource length cannot exceed"
                    " 0x%08lx\n",
                    FT_MAC_RFORK_MAX_LEN ));

        error = FT_THROW( Invalid_جابه_جایی );
        اذهب Exit;
      }

      pfb_len += موقت + 6;
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

    إذا ( FT_QALLOC( pfb_داده, (FT_Long)pfb_len + 2 ) )
      اذهب Exit;

    pfb_داده[0] = 0x80;
    pfb_داده[1] = 1;            /* Ascii section */
    pfb_داده[2] = 0;            /* 4-byte length, fill in later */
    pfb_داده[3] = 0;
    pfb_داده[4] = 0;
    pfb_داده[5] = 0;
    pfb_pos     = 6;
    pfb_lenpos  = 2;

    len  = 0;
    type = 1;

    لكل ( i = 0; i < resource_cnt; i++ )
    {
      error = FT_جریان_Seek( جریان, (FT_ULong)offsets[i] );
      إذا ( error )
        اذهب Exit2;
      إذا ( FT_READ_ULONG( rlen ) )
        اذهب Exit2;

      /* FT2 allocator takes signed long buffer length,
       * too large fragment length causing overflow should be checked
       */
      إذا ( 0x7FFFFFFFUL < rlen )
      {
        error = FT_THROW( Invalid_جابه_جایی );
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
                    i, (فراغ*)pfb_داده, pfb_lenpos ));

        إذا ( pfb_lenpos + 3 > pfb_len + 2 )
          اذهب Exit2;

        pfb_داده[pfb_lenpos    ] = (FT_Byte)( len );
        pfb_داده[pfb_lenpos + 1] = (FT_Byte)( len >> 8 );
        pfb_داده[pfb_lenpos + 2] = (FT_Byte)( len >> 16 );
        pfb_داده[pfb_lenpos + 3] = (FT_Byte)( len >> 24 );

        إذا ( ( flags >> 8 ) == 5 )      /* End of font mark */
          اكسر;

        FT_TRACE3(( "    Write POST fragment #%d header (6-byte) to buffer"
                    " %p + 0x%08lx\n",
                    i, (فراغ*)pfb_داده, pfb_pos ));

        إذا ( pfb_pos + 6 > pfb_len + 2 )
          اذهب Exit2;

        pfb_داده[pfb_pos++] = 0x80;

        type = flags >> 8;
        len  = rlen;

        pfb_داده[pfb_pos++] = (FT_Byte)type;
        pfb_lenpos          = pfb_pos;
        pfb_داده[pfb_pos++] = 0;        /* 4-byte length, fill in later */
        pfb_داده[pfb_pos++] = 0;
        pfb_داده[pfb_pos++] = 0;
        pfb_داده[pfb_pos++] = 0;
      }

      إذا ( pfb_pos > pfb_len || pfb_pos + rlen > pfb_len )
        اذهب Exit2;

      FT_TRACE3(( "    Load POST fragment #%d (%lu byte) to buffer"
                  " %p + 0x%08lx\n",
                  i, rlen, (فراغ*)pfb_داده, pfb_pos ));

      error = FT_جریان_Read( جریان, (FT_Byte *)pfb_داده + pfb_pos, rlen );
      إذا ( error )
        اذهب Exit2;

      pfb_pos += rlen;
    }

    error = FT_ERR( Array_Too_Large );

    إذا ( pfb_pos + 2 > pfb_len + 2 )
      اذهب Exit2;
    pfb_داده[pfb_pos++] = 0x80;
    pfb_داده[pfb_pos++] = 3;

    إذا ( pfb_lenpos + 3 > pfb_len + 2 )
      اذهب Exit2;
    pfb_داده[pfb_lenpos    ] = (FT_Byte)( len );
    pfb_داده[pfb_lenpos + 1] = (FT_Byte)( len >> 8 );
    pfb_داده[pfb_lenpos + 2] = (FT_Byte)( len >> 16 );
    pfb_داده[pfb_lenpos + 3] = (FT_Byte)( len >> 24 );

    أرجع open_face_from_میانگیر( library,
                                  pfb_داده,
                                  pfb_pos,
                                  face_شاخص,
                                  "type1",
                                  aface );

  Exit2:
    إذا ( FT_ERR_EQ( error, Array_Too_Large ) )
      FT_TRACE2(( "  Abort due to too-short buffer to store"
                  " all POST fragments\n" ));
    وإلا إذا ( FT_ERR_EQ( error, Invalid_جابه_جایی ) )
      FT_TRACE2(( "  Abort due to invalid offset in a POST fragment\n" ));

    إذا ( error )
      error = FT_ERR( Cannot_Open_Resource );
    FT_FREE( pfb_داده );

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
                          FT_جریان   جریان,
                          FT_Long    *offsets,
                          FT_Long     resource_cnt,
                          FT_Long     face_شاخص,
                          FT_Face    *aface )
  {
    FT_حافظه  حافظه = library->حافظه;
    FT_Byte*   sfnt_داده = عدم;
    FT_Error   error;
    FT_ULong   flag_جابه_جایی;
    FT_ULong   rlen;
    صحيح        is_cff;
    FT_Long    face_شاخص_درون_resource = 0;


    إذا ( face_شاخص < 0 )
      face_شاخص = -face_شاخص - 1;
    إذا ( face_شاخص >= resource_cnt )
      أرجع FT_THROW( Cannot_Open_Resource );

    flag_جابه_جایی = (FT_ULong)offsets[face_شاخص];
    error = FT_جریان_Seek( جریان, flag_جابه_جایی );
    إذا ( error )
      اذهب Exit;

    إذا ( FT_READ_ULONG( rlen ) )
      اذهب Exit;
    إذا ( !rlen )
      أرجع FT_THROW( Cannot_Open_Resource );
    إذا ( rlen > FT_MAC_RFORK_MAX_LEN )
      أرجع FT_THROW( Invalid_جابه_جایی );

    error = open_face_PS_from_sfnt_جریان( library,
                                           جریان,
                                           face_شاخص,
                                           0, عدم,
                                           aface );
    إذا ( !error )
      اذهب Exit;

    /* rewind sfnt stream before open_face_PS_from_sfnt_stream() */
    error = FT_جریان_Seek( جریان, flag_جابه_جایی + 4 );
    إذا ( error )
      اذهب Exit;

    إذا ( FT_QALLOC( sfnt_داده, rlen ) )
      أرجع error;
    error = FT_جریان_Read( جریان, (FT_Byte *)sfnt_داده, rlen );
    إذا ( error )
    {
      FT_FREE( sfnt_داده );
      اذهب Exit;
    }

    is_cff = rlen > 4 && !ft_memcmp( sfnt_داده, "OTTO", 4 );
    error = open_face_from_میانگیر( library,
                                   sfnt_داده,
                                   rlen,
                                   face_شاخص_درون_resource,
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
                 FT_جریان   جریان,
                 FT_Long     resource_جابه_جایی,
                 FT_Long     face_شاخص,
                 FT_Face    *aface )
  {
    FT_حافظه  حافظه = library->حافظه;
    FT_Error   error;
    FT_Long    map_جابه_جایی, rdata_pos;
    FT_Long    *داده_offsets;
    FT_Long    شمار;


    error = FT_Raccess_Get_سرآیند_Info( library, جریان, resource_جابه_جایی,
                                       &map_جابه_جایی, &rdata_pos );
    إذا ( error )
      أرجع error;

    /* POST resources must be sorted to concatenate properly */
    error = FT_Raccess_Get_داده_Offsets( library, جریان,
                                        map_جابه_جایی, rdata_pos,
                                        TTAG_POST, TRUE,
                                        &داده_offsets, &شمار );
    إذا ( !error )
    {
      error = Mac_Read_POST_Resource( library, جریان, داده_offsets, شمار,
                                      face_شاخص, aface );
      FT_FREE( داده_offsets );
      /* POST exists in an LWFN providing a single face */
      إذا ( !error )
        (*aface)->رقم_faces = 1;
      أرجع error;
    }

    /* sfnt resources should not be sorted to preserve the face order by
       QuickDraw API */
    error = FT_Raccess_Get_داده_Offsets( library, جریان,
                                        map_جابه_جایی, rdata_pos,
                                        TTAG_sfnt, FALSE,
                                        &داده_offsets, &شمار );
    إذا ( !error )
    {
      FT_Long  face_شاخص_داخلی = face_شاخص % شمار;


      error = Mac_Read_sfnt_Resource( library, جریان, داده_offsets, شمار,
                                      face_شاخص_داخلی, aface );
      FT_FREE( داده_offsets );
      إذا ( !error )
      {
        (*aface)->رقم_faces  = شمار;
        (*aface)->face_شاخص = face_شاخص_داخلی;
      }
    }

    أرجع error;
  }


  /* Check for a valid macbinary header, and if we find one   */
  /* check that the (flattened) resource fork in it is valid. */
  /*                                                          */
  ساكن FT_Error
  Is_Mac_دودویی( FT_Library  library,
               FT_جریان   جریان,
               FT_Long     face_شاخص,
               FT_Face    *aface )
  {
    بیعلامت حرف  سرآیند[128];
    FT_Error       error;
    FT_Long        dlen, جابه_جایی;


    إذا ( !جریان )
      أرجع FT_THROW( Invalid_جریان_Operation );

    error = FT_جریان_Seek( جریان, 0 );
    إذا ( error )
      اذهب Exit;

    error = FT_جریان_Read( جریان, (FT_Byte*)سرآیند, 128 );
    إذا ( error )
      اذهب Exit;

    إذا (            سرآیند[ 0] !=   0 ||
                    سرآیند[74] !=   0 ||
                    سرآیند[82] !=   0 ||
                    سرآیند[ 1] ==   0 ||
                    سرآیند[ 1] >   33 ||
                    سرآیند[63] !=   0 ||
         سرآیند[2 + سرآیند[1]] !=   0 ||
                  سرآیند[0x53] > 0x7F )
      أرجع FT_THROW( Unknown_File_Format );

    dlen = ( سرآیند[0x53] << 24 ) |
           ( سرآیند[0x54] << 16 ) |
           ( سرآیند[0x55] <<  8 ) |
             سرآیند[0x56];
#إذا 0
    rlen = ( سرآیند[0x57] << 24 ) |
           ( سرآیند[0x58] << 16 ) |
           ( سرآیند[0x59] <<  8 ) |
             سرآیند[0x5A];
#پایان_اگر /* 0 */
    جابه_جایی = 128 + ( ( dlen + 127 ) & ~127 );

    أرجع IsMacResource( library, جریان, جابه_جایی, face_شاخص, aface );

  Exit:
    أرجع error;
  }


  ساكن FT_Error
  بارگیری_face_درون_embedded_rfork( FT_Library           library,
                               FT_جریان            جریان,
                               FT_Long              face_شاخص,
                               FT_Face             *aface,
                               ثابت FT_Open_آرگومانها  *آرگومانها )
  {

#لغو_تعریف  FT_COMPONENT
#تعریف_کن FT_COMPONENT  raccess

    FT_حافظه  حافظه = library->حافظه;
    FT_Error   error  = FT_ERR( Unknown_File_Format );
    FT_UInt    i;

    حرف*      file_نامها[FT_RACCESS_N_قواعد];
    FT_Long    offsets[FT_RACCESS_N_قواعد];
    FT_Error   errors[FT_RACCESS_N_قواعد];
    FT_Bool    is_darwin_vfs, vfs_rfork_has_no_font = FALSE; /* not tested */

    FT_Open_آرگومانها  args2;
    FT_جریان     stream2 = عدم;


    FT_Raccess_Guess( library, جریان,
                      آرگومانها->pathname, file_نامها, offsets, errors );

    لكل ( i = 0; i < FT_RACCESS_N_قواعد; i++ )
    {
      is_darwin_vfs = ft_raccess_قاعده_by_darwin_vfs( library, i );
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
      args2.pathname = file_نامها[i] ? file_نامها[i] : آرگومانها->pathname;

      FT_TRACE3(( "Try rule %u: %s (offset=%ld) ...",
                  i, args2.pathname, offsets[i] ));

      error = FT_جریان_New( library, &args2, &stream2 );
      إذا ( is_darwin_vfs && FT_ERR_EQ( error, Cannot_Open_جریان ) )
        vfs_rfork_has_no_font = TRUE;

      إذا ( error )
      {
        FT_TRACE3(( "failed\n" ));
        استمر;
      }

      error = IsMacResource( library, stream2, offsets[i],
                             face_شاخص, aface );
      FT_جریان_Free( stream2, 0 );

      FT_TRACE3(( "%s\n", error ? "failed": "successful" ));

      إذا ( !error )
          اكسر;
      وإلا إذا ( is_darwin_vfs )
          vfs_rfork_has_no_font = TRUE;
    }

    لكل (i = 0; i < FT_RACCESS_N_قواعد; i++)
    {
      إذا ( file_نامها[i] )
        FT_FREE( file_نامها[i] );
    }

    /* Caller (load_mac_face) requires FT_Err_Unknown_File_Format. */
    إذا ( error )
      error = FT_ERR( Unknown_File_Format );

    أرجع error;

#لغو_تعریف  FT_COMPONENT
#تعریف_کن FT_COMPONENT  objs

  }


  /* Check for some macintosh formats without Carbon framework.    */
  /* Is this a macbinary file?  If so look at the resource fork.   */
  /* Is this a mac dfont file?                                     */
  /* Is this an old style resource fork? (in data)                 */
  /* Else call load_face_in_embedded_rfork to try extra rules      */
  /* (defined in `ftrfork.c').                                     */
  /*                                                               */
  ساكن FT_Error
  بارگیری_mac_face( FT_Library           library,
                 FT_جریان            جریان,
                 FT_Long              face_شاخص,
                 FT_Face             *aface,
                 ثابت FT_Open_آرگومانها  *آرگومانها )
  {
    FT_Error error;
    FT_استفادهنشده( آرگومانها );


    error = Is_Mac_دودویی( library, جریان, face_شاخص, aface );
    إذا ( FT_ERR_EQ( error, Unknown_File_Format ) )
    {

#لغو_تعریف  FT_COMPONENT
#تعریف_کن FT_COMPONENT  raccess

#اگر_تعریف_شده FT_DEBUG_LEVEL_TRACE
      FT_TRACE3(( "Try as dfont: " ));
      إذا ( !( آرگومانها->flags & FT_OPEN_حافظه ) )
        FT_TRACE3(( "%s ...", آرگومانها->pathname ));
#پایان_اگر

      error = IsMacResource( library, جریان, 0, face_شاخص, aface );

      FT_TRACE3(( "%s\n", error ? "failed" : "successful" ));

#لغو_تعریف  FT_COMPONENT
#تعریف_کن FT_COMPONENT  objs

    }

    إذا ( ( FT_ERR_EQ( error, Unknown_File_Format )      ||
           FT_ERR_EQ( error, Invalid_جریان_Operation ) ) &&
         ( آرگومانها->flags & FT_OPEN_PATHNAME )               )
      error = بارگیری_face_درون_embedded_rfork( library, جریان,
                                           face_شاخص, aface, آرگومانها );
    أرجع error;
  }
#پایان_اگر

#پایان_اگر  /* !FT_MACINTOSH && FT_CONFIG_OPTION_MAC_FONTS */


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Open_Face( FT_Library           library,
                ثابت FT_Open_آرگومانها*  آرگومانها,
                FT_Long              face_شاخص,
                FT_Face             *aface )
  {
    أرجع ft_open_face_داخلی( library, آرگومانها, face_شاخص, aface, 1 );
  }


  ساكن FT_Error
  ft_open_face_داخلی( FT_Library           library,
                         ثابت FT_Open_آرگومانها*  آرگومانها,
                         FT_Long              face_شاخص,
                         FT_Face             *aface,
                         FT_Bool              test_mac_fonts )
  {
    FT_Error     error;
    FT_Driver    driver = عدم;
    FT_حافظه    حافظه = عدم;
    FT_جریان    جریان = عدم;
    FT_Face      face   = عدم;
    FT_List_گره  گره   = عدم;
    FT_Bool      external_جریان;
    FT_Module*   cur;
    FT_Module*   حد;

#اگر_تعریف_نشده FT_CONFIG_OPTION_MAC_FONTS
    FT_استفادهنشده( test_mac_fonts );
#پایان_اگر


    /* only use lower 31 bits together with sign bit */
    إذا ( face_شاخص > 0 )
      face_شاخص &= 0x7FFFFFFFL;
    وإلا
    {
      face_شاخص  = -face_شاخص;
      face_شاخص &= 0x7FFFFFFFL;
      face_شاخص  = -face_شاخص;
    }

#اگر_تعریف_شده FT_DEBUG_LEVEL_TRACE
    FT_TRACE3(( "FT_Open_Face: " ));
    إذا ( face_شاخص < 0 )
      FT_TRACE3(( "Requesting number of faces and named instances\n"));
    وإلا
    {
      FT_TRACE3(( "Requesting face %ld", face_شاخص & 0xFFFFL ));
      إذا ( face_شاخص & 0x7FFF0000L )
        FT_TRACE3(( ", named instance %ld", face_شاخص >> 16 ));
      FT_TRACE3(( "\n" ));
    }
#پایان_اگر

    /* test for valid `library' delayed to `FT_Stream_New' */

    إذا ( !آرگومانها )
      أرجع FT_THROW( Invalid_آرگومان );

    external_جریان = FT_BOOL( ( آرگومانها->flags & FT_OPEN_جریان ) &&
                               آرگومانها->جریان                     );

    /* create input stream */
    error = FT_جریان_New( library, آرگومانها, &جریان );
    إذا ( error )
      اذهب Fail3;

    /* Do this error check after `FT_Stream_New` to ensure that the */
    /* 'close' callback is called.                                  */
    إذا ( !aface && face_شاخص >= 0 )
    {
      error = FT_THROW( Invalid_آرگومان );
      اذهب Fail3;
    }

    حافظه = library->حافظه;

    /* If the font driver is specified in the `args' structure, use */
    /* it.  Otherwise, we scan the list of registered drivers.      */
    إذا ( ( آرگومانها->flags & FT_OPEN_DRIVER ) && آرگومانها->driver )
    {
      driver = FT_DRIVER( آرگومانها->driver );

      /* not all modules are drivers, so check... */
      إذا ( FT_MODULE_IS_DRIVER( driver ) )
      {
        FT_Int         رقم_params = 0;
        FT_Parameter*  params     = عدم;


        إذا ( آرگومانها->flags & FT_OPEN_PARAMS )
        {
          رقم_params = آرگومانها->رقم_params;
          params     = آرگومانها->params;
        }

        error = open_face( driver, &جریان, &external_جریان, face_شاخص,
                           رقم_params, params, &face );
        إذا ( !error )
          اذهب Success;
      }
      وإلا
        error = FT_THROW( Invalid_Handle );

      FT_جریان_Free( جریان, external_جریان );
      اذهب Fail;
    }
    وإلا
    {
      error = FT_ERR( Missing_Module );

      /* check each font driver for an appropriate format */
      cur   = library->modules;
      حد = cur + library->رقم_modules;

      لكل ( ; cur < حد; cur++ )
      {
        /* not all modules are font drivers, so check... */
        إذا ( FT_MODULE_IS_DRIVER( cur[0] ) )
        {
          FT_Int         رقم_params = 0;
          FT_Parameter*  params     = عدم;


          driver = FT_DRIVER( cur[0] );

          إذا ( آرگومانها->flags & FT_OPEN_PARAMS )
          {
            رقم_params = آرگومانها->رقم_params;
            params     = آرگومانها->params;
          }

          error = open_face( driver, &جریان, &external_جریان, face_شاخص,
                             رقم_params, params, &face );
          إذا ( !error )
            اذهب Success;

#اگر_تعریف_شده FT_CONFIG_OPTION_MAC_FONTS
          إذا ( test_mac_fonts                                           &&
               ft_strcmp( cur[0]->clazz->module_name, "truetype" ) == 0 &&
               FT_ERR_EQ( error, جدول_Missing )                        )
          {
            /* TrueType but essential tables are missing */
            error = FT_جریان_Seek( جریان, 0 );
            إذا ( error )
              اكسر;

            error = open_face_PS_from_sfnt_جریان( library,
                                                   جریان,
                                                   face_شاخص,
                                                   رقم_params,
                                                   params,
                                                   aface );
            إذا ( !error )
            {
              FT_جریان_Free( جریان, external_جریان );
              أرجع error;
            }
          }
#پایان_اگر

          إذا ( FT_ERR_NEQ( error, Unknown_File_Format ) )
            اذهب Fail3;
        }
      }

    Fail3:
      /* If we are on the mac, and we get an                          */
      /* FT_Err_Invalid_Stream_Operation it may be because we have an */
      /* empty data fork, so we need to check the resource fork.      */
      إذا ( FT_ERR_NEQ( error, Cannot_Open_جریان )       &&
           FT_ERR_NEQ( error, Unknown_File_Format )      &&
           FT_ERR_NEQ( error, Invalid_جریان_Operation ) )
        اذهب Fail2;

#إذا !defined( FT_MACINTOSH ) && defined( FT_CONFIG_OPTION_MAC_FONTS )
      إذا ( test_mac_fonts )
      {
        error = بارگیری_mac_face( library, جریان, face_شاخص, aface, آرگومانها );
        إذا ( !error )
        {
          /* We don't want to go to Success here.  We've already done   */
          /* that.  On the other hand, if we succeeded we still need to */
          /* close this stream (we opened a different stream which      */
          /* extracted the interesting information out of this stream   */
          /* here.  That stream will still be open and the face will    */
          /* point to it).                                              */
          FT_جریان_Free( جریان, external_جریان );
          أرجع error;
        }
      }

      إذا ( FT_ERR_NEQ( error, Unknown_File_Format ) )
        اذهب Fail2;
#پایان_اگر  /* !FT_MACINTOSH && FT_CONFIG_OPTION_MAC_FONTS */

      /* no driver is able to handle this format */
      error = FT_THROW( Unknown_File_Format );

  Fail2:
      FT_جریان_Free( جریان, external_جریان );
      اذهب Fail;
    }

  Success:
    FT_TRACE4(( "FT_Open_Face: New face object, adding to list\n" ));

    /* add the face object to its driver's list */
    إذا ( FT_QNEW( گره ) )
      اذهب Fail;

    گره->داده = face;
    /* don't assume driver is the same as face->driver, so use */
    /* face->driver instead.                                   */
    FT_List_جمع_کن( &face->driver->faces_list, گره );

    /* now allocate a glyph slot object for the face */
    FT_TRACE4(( "FT_Open_Face: Creating glyph slot\n" ));

    إذا ( face_شاخص >= 0 )
    {
      error = FT_New_Glyph_جایگاه( face, عدم );
      إذا ( error )
        اذهب Fail;

      /* finally, allocate a size object for the face */
      {
        FT_اندازه  اندازه;


        FT_TRACE4(( "FT_Open_Face: Creating size object\n" ));

        error = FT_New_اندازه( face, &اندازه );
        إذا ( error )
          اذهب Fail;

        face->اندازه = اندازه;
      }
    }

    /* some checks */

    إذا ( FT_IS_SCALABLE( face ) )
    {
      إذا ( face->ارتفاع < 0 )
        face->ارتفاع = (FT_Short)-face->ارتفاع;

      إذا ( !FT_HAS_VERTICAL( face ) )
        face->max_advance_ارتفاع = (FT_Short)face->ارتفاع;
    }

    إذا ( FT_HAS_FIXED_SIZES( face ) )
    {
      FT_Int  i;


      لكل ( i = 0; i < face->رقم_fixed_sizes; i++ )
      {
        FT_Bitmap_اندازه*  bsize = face->موجود_sizes + i;


        إذا ( bsize->ارتفاع < 0 )
          bsize->ارتفاع = -bsize->ارتفاع;
        إذا ( bsize->x_ppem < 0 )
          bsize->x_ppem = -bsize->x_ppem;
        إذا ( bsize->y_ppem < 0 )
          bsize->y_ppem = -bsize->y_ppem;

        /* check whether negation actually has worked */
        إذا ( bsize->ارتفاع < 0 || bsize->x_ppem < 0 || bsize->y_ppem < 0 )
        {
          FT_TRACE0(( "FT_Open_Face:"
                      " Invalid bitmap dimensions for strike %d,"
                      " now disabled\n", i ));
          bsize->عرض  = 0;
          bsize->ارتفاع = 0;
          bsize->اندازه   = 0;
          bsize->x_ppem = 0;
          bsize->y_ppem = 0;
        }
      }
    }

    /* initialize internal face data */
    {
      FT_Face_داخلی  داخلی = face->داخلی;


      داخلی->transform_ماتریس.xx = 0x10000L;
      داخلی->transform_ماتریس.xy = 0;
      داخلی->transform_ماتریس.yx = 0;
      داخلی->transform_ماتریس.yy = 0x10000L;

      داخلی->transform_delta.x = 0;
      داخلی->transform_delta.y = 0;

      داخلی->refcount = 1;

      داخلی->no_stem_darkening = -1;
    }

    إذا ( aface )
      *aface = face;
    وإلا
      FT_انجامشده_Face( face );

    اذهب Exit;

  Fail:
    إذا ( گره )
      FT_انجامشده_Face( face );    /* face must be in the driver's list */
    وإلا إذا ( face )
      destroy_face( حافظه, face, driver );

  Exit:
#اگر_تعریف_شده FT_DEBUG_LEVEL_TRACE
    إذا ( !error && face_شاخص < 0 )
    {
      FT_TRACE3(( "FT_Open_Face: The font has %ld face%s\n",
                  face->رقم_faces,
                  face->رقم_faces == 1 ? "" : "s" ));
      FT_TRACE3(( "              and %ld named instance%s for face %ld\n",
                  face->style_flags >> 16,
                  ( face->style_flags >> 16 ) == 1 ? "" : "s",
                  -face_شاخص - 1 ));
    }
#پایان_اگر

    FT_TRACE4(( "FT_Open_Face: Return 0x%x\n", error ));

    أرجع error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Attach_File( FT_Face      face,
                  ثابت حرف*  filepathname )
  {
    FT_Open_آرگومانها  open;


    /* test for valid `face' delayed to `FT_Attach_Stream' */

    إذا ( !filepathname )
      أرجع FT_THROW( Invalid_آرگومان );

    open.جریان   = عدم;
    open.flags    = FT_OPEN_PATHNAME;
    open.pathname = (حرف*)filepathname;

    أرجع FT_Attach_جریان( face, &open );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Attach_جریان( FT_Face              face,
                    ثابت FT_Open_آرگومانها*  parameters )
  {
    FT_جریان  جریان;
    FT_Error   error;
    FT_Driver  driver;

    FT_Driver_Class  clazz;


    /* test for valid `parameters' delayed to `FT_Stream_New' */

    إذا ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );

    driver = face->driver;
    إذا ( !driver )
      أرجع FT_THROW( Invalid_Driver_Handle );

    error = FT_جریان_New( driver->ریشه.library, parameters, &جریان );
    إذا ( error )
      اذهب Exit;

    /* we implement FT_Attach_Stream in each driver through the */
    /* `attach_file' interface                                  */

    error = FT_ERR( Unimplemented_Feature );
    clazz = driver->clazz;
    إذا ( clazz->attach_file )
      error = clazz->attach_file( face, جریان );

    /* close the attached stream */
    FT_جریان_Free( جریان,
                    FT_BOOL( parameters->جریان                     &&
                             ( parameters->flags & FT_OPEN_جریان ) ) );

  Exit:
    أرجع error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Reference_Face( FT_Face  face )
  {
    إذا ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );

    face->داخلی->refcount++;

    أرجع FT_Err_Ok;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_انجامشده_Face( FT_Face  face )
  {
    FT_Error     error;
    FT_Driver    driver;
    FT_حافظه    حافظه;
    FT_List_گره  گره;


    error = FT_ERR( Invalid_Face_Handle );
    إذا ( face && face->driver )
    {
      face->داخلی->refcount--;
      إذا ( face->داخلی->refcount > 0 )
        error = FT_Err_Ok;
      وإلا
      {
        driver = face->driver;
        حافظه = driver->ریشه.حافظه;

        /* find face in driver's list */
        گره = FT_List_پیدا_کن( &driver->faces_list, face );
        إذا ( گره )
        {
          /* remove face object from the driver's list */
          FT_List_Remove( &driver->faces_list, گره );
          FT_FREE( گره );

          /* now destroy the object proper */
          destroy_face( حافظه, face, driver );
          error = FT_Err_Ok;
        }
      }
    }

    أرجع error;
  }


  /* documentation is in ftobjs.h */

  FT_EXPORT_DEF( FT_Error )
  FT_New_اندازه( FT_Face   face,
               FT_اندازه  *asize )
  {
    FT_Error         error;
    FT_حافظه        حافظه;
    FT_Driver        driver;
    FT_Driver_Class  clazz;

    FT_اندازه          اندازه = عدم;
    FT_List_گره      گره = عدم;

    FT_اندازه_داخلی  داخلی = عدم;


    إذا ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );

    إذا ( !asize )
      أرجع FT_THROW( Invalid_آرگومان );

    إذا ( !face->driver )
      أرجع FT_THROW( Invalid_Driver_Handle );

    *asize = عدم;

    driver = face->driver;
    clazz  = driver->clazz;
    حافظه = face->حافظه;

    /* Allocate new size object and perform basic initialisation */
    إذا ( FT_ALLOC( اندازه, clazz->اندازه_object_اندازه ) || FT_QNEW( گره ) )
      اذهب Exit;

    اندازه->face = face;

    إذا ( FT_NEW( داخلی ) )
      اذهب Exit;

    اندازه->داخلی = داخلی;

    إذا ( clazz->init_اندازه )
      error = clazz->init_اندازه( اندازه );

    /* in case of success, add to the face's list */
    إذا ( !error )
    {
      *asize     = اندازه;
      گره->داده = اندازه;
      FT_List_جمع_کن( &face->sizes_list, گره );
    }

  Exit:
    إذا ( error )
    {
      FT_FREE( گره );
      إذا ( اندازه )
        FT_FREE( اندازه->داخلی );
      FT_FREE( اندازه );
    }

    أرجع error;
  }


  /* documentation is in ftobjs.h */

  FT_EXPORT_DEF( FT_Error )
  FT_انجامشده_اندازه( FT_اندازه  اندازه )
  {
    FT_Error     error;
    FT_Driver    driver;
    FT_حافظه    حافظه;
    FT_Face      face;
    FT_List_گره  گره;


    إذا ( !اندازه )
      أرجع FT_THROW( Invalid_اندازه_Handle );

    face = اندازه->face;
    إذا ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );

    driver = face->driver;
    إذا ( !driver )
      أرجع FT_THROW( Invalid_Driver_Handle );

    حافظه = driver->ریشه.حافظه;

    error = FT_Err_Ok;
    گره  = FT_List_پیدا_کن( &face->sizes_list, اندازه );
    إذا ( گره )
    {
      FT_List_Remove( &face->sizes_list, گره );
      FT_FREE( گره );

      إذا ( face->اندازه == اندازه )
      {
        face->اندازه = عدم;
        إذا ( face->sizes_list.سر )
          face->اندازه = (FT_اندازه)(face->sizes_list.سر->داده);
      }

      destroy_اندازه( حافظه, اندازه, driver );
    }
    وإلا
      error = FT_THROW( Invalid_اندازه_Handle );

    أرجع error;
  }


  /* documentation is in ftobjs.h */

  FT_پایه_DEF( FT_Error )
  FT_تطابق_اندازه( FT_Face          face,
                 FT_اندازه_Request  req,
                 FT_Bool          ignore_عرض,
                 FT_ULong*        اندازه_شاخص )
  {
    FT_Int   i;
    FT_Long  w, h;


    إذا ( !FT_HAS_FIXED_SIZES( face ) )
      أرجع FT_THROW( Invalid_Face_Handle );

    /* FT_Bitmap_Size doesn't provide enough info... */
    إذا ( req->type != FT_اندازه_REQUEST_TYPE_NOMINAL )
      أرجع FT_THROW( Unimplemented_Feature );

    w = FT_REQUEST_عرض ( req );
    h = FT_REQUEST_ارتفاع( req );

    إذا ( req->عرض && !req->ارتفاع )
      h = w;
    وإلا إذا ( !req->عرض && req->ارتفاع )
      w = h;

    w = FT_PIX_دور( w );
    h = FT_PIX_دور( h );

    إذا ( !w || !h )
      أرجع FT_THROW( Invalid_Pixel_اندازه );

    لكل ( i = 0; i < face->رقم_fixed_sizes; i++ )
    {
      FT_Bitmap_اندازه*  bsize = face->موجود_sizes + i;


      إذا ( h != FT_PIX_دور( bsize->y_ppem ) )
        استمر;

      إذا ( w == FT_PIX_دور( bsize->x_ppem ) || ignore_عرض )
      {
        FT_TRACE3(( "FT_Match_Size: bitmap strike %d matches\n", i ));

        إذا ( اندازه_شاخص )
          *اندازه_شاخص = (FT_ULong)i;

        أرجع FT_Err_Ok;
      }
    }

    FT_TRACE3(( "FT_Match_Size: no matching bitmap strike\n" ));

    أرجع FT_THROW( Invalid_Pixel_اندازه );
  }


  /* documentation is in ftobjs.h */

  FT_پایه_DEF( فراغ )
  ft_synthesize_vertical_metrics( FT_Glyph_Metrics*  metrics,
                                  FT_Pos             advance )
  {
    FT_Pos  ارتفاع = metrics->ارتفاع;


    /* compensate for glyph with bbox above/below the baseline */
    إذا ( metrics->horiBearingY < 0 )
    {
      إذا ( ارتفاع < metrics->horiBearingY )
        ارتفاع = metrics->horiBearingY;
    }
    وإلا إذا ( metrics->horiBearingY > 0 )
      ارتفاع -= metrics->horiBearingY;

    /* the factor 1.2 is a heuristical value */
    إذا ( !advance )
      advance = ارتفاع * 12 / 10;

    metrics->vertBearingX = metrics->horiBearingX - metrics->horiAdvance / 2;
    metrics->vertBearingY = ( advance - ارتفاع ) / 2;
    metrics->vertAdvance  = advance;
  }


  ساكن فراغ
  ft_recompute_scaled_metrics( FT_Face           face,
                               FT_اندازه_Metrics*  metrics )
  {
    /* Compute root ascender, descender, test height, and max_advance */

#اگر_تعریف_شده GRID_FIT_METRICS
    metrics->ascender    = FT_PIX_CEIL( FT_MulFix( face->ascender,
                                                   metrics->y_scale ) );

    metrics->descender   = FT_PIX_FLOOR( FT_MulFix( face->descender,
                                                    metrics->y_scale ) );

    metrics->ارتفاع      = FT_PIX_دور( FT_MulFix( face->ارتفاع,
                                                    metrics->y_scale ) );

    metrics->max_advance = FT_PIX_دور( FT_MulFix( face->max_advance_عرض,
                                                    metrics->x_scale ) );
#وإلا /* !GRID_FIT_METRICS */
    metrics->ascender    = FT_MulFix( face->ascender,
                                      metrics->y_scale );

    metrics->descender   = FT_MulFix( face->descender,
                                      metrics->y_scale );

    metrics->ارتفاع      = FT_MulFix( face->ارتفاع,
                                      metrics->y_scale );

    metrics->max_advance = FT_MulFix( face->max_advance_عرض,
                                      metrics->x_scale );
#پایان_اگر /* !GRID_FIT_METRICS */
  }


  FT_پایه_DEF( فراغ )
  FT_Select_Metrics( FT_Face   face,
                     FT_ULong  strike_شاخص )
  {
    FT_اندازه_Metrics*  metrics;
    FT_Bitmap_اندازه*   bsize;


    metrics = &face->اندازه->metrics;
    bsize   = face->موجود_sizes + strike_شاخص;

    metrics->x_ppem = (FT_UShort)( ( bsize->x_ppem + 32 ) >> 6 );
    metrics->y_ppem = (FT_UShort)( ( bsize->y_ppem + 32 ) >> 6 );

    إذا ( FT_IS_SCALABLE( face ) )
    {
      metrics->x_scale = FT_DivFix( bsize->x_ppem,
                                    face->units_در_هر_EM );
      metrics->y_scale = FT_DivFix( bsize->y_ppem,
                                    face->units_در_هر_EM );

      ft_recompute_scaled_metrics( face, metrics );
    }
    وإلا
    {
      metrics->x_scale     = 1L << 16;
      metrics->y_scale     = 1L << 16;
      metrics->ascender    = bsize->y_ppem;
      metrics->descender   = 0;
      metrics->ارتفاع      = bsize->ارتفاع << 6;
      metrics->max_advance = bsize->x_ppem;
    }
  }


  FT_پایه_DEF( FT_Error )
  FT_Request_Metrics( FT_Face          face,
                      FT_اندازه_Request  req )
  {
    FT_Error  error = FT_Err_Ok;

    FT_اندازه_Metrics*  metrics;


    metrics = &face->اندازه->metrics;

    إذا ( FT_IS_SCALABLE( face ) )
    {
      FT_Long  w = 0, h = 0, scaled_w = 0, scaled_h = 0;


      بدل ( req->type )
      {
      حالة FT_اندازه_REQUEST_TYPE_NOMINAL:
        w = h = face->units_در_هر_EM;
        اكسر;

      حالة FT_اندازه_REQUEST_TYPE_REAL_DIM:
        w = h = face->ascender - face->descender;
        اكسر;

      حالة FT_اندازه_REQUEST_TYPE_BBOX:
        w = face->bbox.xMax - face->bbox.xMin;
        h = face->bbox.yMax - face->bbox.yMin;
        اكسر;

      حالة FT_اندازه_REQUEST_TYPE_CELL:
        w = face->max_advance_عرض;
        h = face->ascender - face->descender;
        اكسر;

      حالة FT_اندازه_REQUEST_TYPE_SCALES:
        metrics->x_scale = (FT_Fixed)req->عرض;
        metrics->y_scale = (FT_Fixed)req->ارتفاع;
        إذا ( !metrics->x_scale )
          metrics->x_scale = metrics->y_scale;
        وإلا إذا ( !metrics->y_scale )
          metrics->y_scale = metrics->x_scale;
        اذهب Calculate_Ppem;

      حالة FT_اندازه_REQUEST_TYPE_MAX:
        اكسر;
      }

      /* to be on the safe side */
      إذا ( w < 0 )
        w = -w;

      إذا ( h < 0 )
        h = -h;

      scaled_w = FT_REQUEST_عرض ( req );
      scaled_h = FT_REQUEST_ارتفاع( req );

      /* determine scales */
      إذا ( req->ارتفاع || !req->عرض )
      {
        إذا ( h == 0 )
        {
          FT_ERROR(( "FT_Request_Metrics: Divide by zero\n" ));
          error = FT_ERR( Divide_By_Zero );
          اذهب Exit;
        }

        metrics->y_scale = FT_DivFix( scaled_h, h );
      }

      إذا ( req->عرض )
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

      إذا ( !req->ارتفاع )
      {
        metrics->y_scale = metrics->x_scale;
        scaled_h = FT_MulDiv( scaled_w, h, w );
      }

      إذا ( req->type == FT_اندازه_REQUEST_TYPE_CELL )
      {
        إذا ( metrics->y_scale > metrics->x_scale )
          metrics->y_scale = metrics->x_scale;
        وإلا
          metrics->x_scale = metrics->y_scale;
      }

  Calculate_Ppem:
      /* calculate the ppems */
      إذا ( req->type != FT_اندازه_REQUEST_TYPE_NOMINAL )
      {
        scaled_w = FT_MulFix( face->units_در_هر_EM, metrics->x_scale );
        scaled_h = FT_MulFix( face->units_در_هر_EM, metrics->y_scale );
      }

      scaled_w = ( scaled_w + 32 ) >> 6;
      scaled_h = ( scaled_h + 32 ) >> 6;
      إذا ( scaled_w > (FT_Long)FT_USHORT_MAX ||
           scaled_h > (FT_Long)FT_USHORT_MAX )
      {
        FT_ERROR(( "FT_Request_Metrics: Resulting ppem size too large\n" ));
        error = FT_ERR( Invalid_Pixel_اندازه );
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
  FT_Select_اندازه( FT_Face  face,
                  FT_Int   strike_شاخص )
  {
    FT_Error         error = FT_Err_Ok;
    FT_Driver_Class  clazz;


    إذا ( !face || !FT_HAS_FIXED_SIZES( face ) )
      أرجع FT_THROW( Invalid_Face_Handle );

    إذا ( strike_شاخص < 0 || strike_شاخص >= face->رقم_fixed_sizes )
      أرجع FT_THROW( Invalid_آرگومان );

    clazz = face->driver->clazz;

    إذا ( clazz->select_اندازه )
    {
      error = clazz->select_اندازه( face->اندازه, (FT_ULong)strike_شاخص );

      FT_TRACE5(( "FT_Select_Size (%s driver):\n",
                  face->driver->ریشه.clazz->module_name ));
    }
    وإلا
    {
      FT_Select_Metrics( face, (FT_ULong)strike_شاخص );

      FT_TRACE5(( "FT_Select_Size:\n" ));
    }

#اگر_تعریف_شده FT_DEBUG_LEVEL_TRACE
    {
      FT_اندازه_Metrics*  metrics = &face->اندازه->metrics;


      FT_TRACE5(( "  x scale: %ld (%f)\n",
                  metrics->x_scale, (مزدوج)metrics->x_scale / 65536 ));
      FT_TRACE5(( "  y scale: %ld (%f)\n",
                  metrics->y_scale, (مزدوج)metrics->y_scale / 65536 ));
      FT_TRACE5(( "  ascender: %f\n",
                  (مزدوج)metrics->ascender / 64 ));
      FT_TRACE5(( "  descender: %f\n",
                  (مزدوج)metrics->descender / 64 ));
      FT_TRACE5(( "  height: %f\n",
                  (مزدوج)metrics->ارتفاع / 64 ));
      FT_TRACE5(( "  max advance: %f\n",
                  (مزدوج)metrics->max_advance / 64 ));
      FT_TRACE5(( "  x ppem: %d\n", metrics->x_ppem ));
      FT_TRACE5(( "  y ppem: %d\n", metrics->y_ppem ));
    }
#پایان_اگر

    أرجع error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Request_اندازه( FT_Face          face,
                   FT_اندازه_Request  req )
  {
    FT_Error         error;
    FT_Driver_Class  clazz;
    FT_ULong         strike_شاخص;


    إذا ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );

    إذا ( !face->اندازه )
      أرجع FT_THROW( Invalid_اندازه_Handle );

    إذا ( !req || req->عرض < 0 || req->ارتفاع < 0 ||
         req->type >= FT_اندازه_REQUEST_TYPE_MAX )
      أرجع FT_THROW( Invalid_آرگومان );

    /* signal the auto-hinter to recompute its size metrics */
    /* (if requested)                                       */
    face->اندازه->داخلی->autohint_metrics.x_scale = 0;

    clazz = face->driver->clazz;

    إذا ( clazz->request_اندازه )
    {
      error = clazz->request_اندازه( face->اندازه, req );

      FT_TRACE5(( "FT_Request_Size (%s driver):\n",
                  face->driver->ریشه.clazz->module_name ));
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
      error = FT_تطابق_اندازه( face, req, 0, &strike_شاخص );
      إذا ( error )
        اذهب Exit;

      أرجع FT_Select_اندازه( face, (FT_Int)strike_شاخص );
    }
    وإلا
    {
      error = FT_Request_Metrics( face, req );
      إذا ( error )
        اذهب Exit;

      FT_TRACE5(( "FT_Request_Size:\n" ));
    }

#اگر_تعریف_شده FT_DEBUG_LEVEL_TRACE
    {
      FT_اندازه_Metrics*  metrics = &face->اندازه->metrics;


      FT_TRACE5(( "  x scale: %ld (%f)\n",
                  metrics->x_scale, (مزدوج)metrics->x_scale / 65536 ));
      FT_TRACE5(( "  y scale: %ld (%f)\n",
                  metrics->y_scale, (مزدوج)metrics->y_scale / 65536 ));
      FT_TRACE5(( "  ascender: %f\n",
                  (مزدوج)metrics->ascender / 64 ));
      FT_TRACE5(( "  descender: %f\n",
                  (مزدوج)metrics->descender / 64 ));
      FT_TRACE5(( "  height: %f\n",
                  (مزدوج)metrics->ارتفاع / 64 ));
      FT_TRACE5(( "  max advance: %f\n",
                  (مزدوج)metrics->max_advance / 64 ));
      FT_TRACE5(( "  x ppem: %d\n", metrics->x_ppem ));
      FT_TRACE5(( "  y ppem: %d\n", metrics->y_ppem ));
    }
#پایان_اگر

  Exit:
    أرجع error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_مجموعه_Char_اندازه( FT_Face     face,
                    FT_F26Dot6  char_عرض,
                    FT_F26Dot6  char_ارتفاع,
                    FT_UInt     horz_resolution,
                    FT_UInt     vert_resolution )
  {
    FT_اندازه_Request_Rec  req;


    /* check of `face' delayed to `FT_Request_Size' */

    إذا ( !char_عرض )
      char_عرض = char_ارتفاع;
    وإلا إذا ( !char_ارتفاع )
      char_ارتفاع = char_عرض;

    إذا ( !horz_resolution )
      horz_resolution = vert_resolution;
    وإلا إذا ( !vert_resolution )
      vert_resolution = horz_resolution;

    إذا ( char_عرض  < 1 * 64 )
      char_عرض  = 1 * 64;
    إذا ( char_ارتفاع < 1 * 64 )
      char_ارتفاع = 1 * 64;

    إذا ( !horz_resolution )
      horz_resolution = vert_resolution = 72;

    req.type           = FT_اندازه_REQUEST_TYPE_NOMINAL;
    req.عرض          = char_عرض;
    req.ارتفاع         = char_ارتفاع;
    req.horiResolution = horz_resolution;
    req.vertResolution = vert_resolution;

    أرجع FT_Request_اندازه( face, &req );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_مجموعه_Pixel_Sizes( FT_Face  face,
                      FT_UInt  pixel_عرض,
                      FT_UInt  pixel_ارتفاع )
  {
    FT_اندازه_Request_Rec  req;


    /* check of `face' delayed to `FT_Request_Size' */

    إذا ( pixel_عرض == 0 )
      pixel_عرض = pixel_ارتفاع;
    وإلا إذا ( pixel_ارتفاع == 0 )
      pixel_ارتفاع = pixel_عرض;

    إذا ( pixel_عرض  < 1 )
      pixel_عرض  = 1;
    إذا ( pixel_ارتفاع < 1 )
      pixel_ارتفاع = 1;

    /* use `>=' to avoid potential compiler warning on 16bit platforms */
    إذا ( pixel_عرض >= 0xFFFFU )
      pixel_عرض = 0xFFFFU;
    إذا ( pixel_ارتفاع >= 0xFFFFU )
      pixel_ارتفاع = 0xFFFFU;

    req.type           = FT_اندازه_REQUEST_TYPE_NOMINAL;
    req.عرض          = (FT_Long)( pixel_عرض << 6 );
    req.ارتفاع         = (FT_Long)( pixel_ارتفاع << 6 );
    req.horiResolution = 0;
    req.vertResolution = 0;

    أرجع FT_Request_اندازه( face, &req );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Kerning( FT_Face     face,
                  FT_UInt     چپ_glyph,
                  FT_UInt     راست_glyph,
                  FT_UInt     kern_نما,
                  FT_وکتور  *akerning )
  {
    FT_Error   error = FT_Err_Ok;
    FT_Driver  driver;


    إذا ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );

    إذا ( !akerning )
      أرجع FT_THROW( Invalid_آرگومان );

    driver = face->driver;

    akerning->x = 0;
    akerning->y = 0;

    إذا ( driver->clazz->get_kerning )
    {
      error = driver->clazz->get_kerning( face,
                                          چپ_glyph,
                                          راست_glyph,
                                          akerning );
      إذا ( !error )
      {
        إذا ( kern_نما != FT_KERNING_UNSCALED )
        {
          akerning->x = FT_MulFix( akerning->x, face->اندازه->metrics.x_scale );
          akerning->y = FT_MulFix( akerning->y, face->اندازه->metrics.y_scale );

          إذا ( kern_نما != FT_KERNING_UNFITTED )
          {
            FT_Pos  orig_x = akerning->x;
            FT_Pos  orig_y = akerning->y;


            /* we scale down kerning values for small ppem values */
            /* to avoid that rounding makes them too big.         */
            /* `25' has been determined heuristically.            */
            إذا ( face->اندازه->metrics.x_ppem < 25 )
              akerning->x = FT_MulDiv( orig_x,
                                       face->اندازه->metrics.x_ppem, 25 );
            إذا ( face->اندازه->metrics.y_ppem < 25 )
              akerning->y = FT_MulDiv( orig_y,
                                       face->اندازه->metrics.y_ppem, 25 );

            akerning->x = FT_PIX_دور( akerning->x );
            akerning->y = FT_PIX_دور( akerning->y );

#اگر_تعریف_شده FT_DEBUG_LEVEL_TRACE
            {
              FT_Pos  orig_x_rounded = FT_PIX_دور( orig_x );
              FT_Pos  orig_y_rounded = FT_PIX_دور( orig_y );


              إذا ( akerning->x != orig_x_rounded ||
                   akerning->y != orig_y_rounded )
                FT_TRACE5(( "FT_Get_Kerning: horizontal kerning"
                            " (%ld, %ld) scaled down to (%ld, %ld) pixels\n",
                            orig_x_rounded / 64, orig_y_rounded / 64,
                            akerning->x / 64, akerning->y / 64 ));
            }
#پایان_اگر
          }
        }
      }
    }

    أرجع error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Track_Kerning( FT_Face    face,
                        FT_Fixed   نقطه_اندازه,
                        FT_Int     degree,
                        FT_Fixed*  akerning )
  {
    FT_Service_Kerning  service;
    FT_Error            error = FT_Err_Ok;


    إذا ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );

    إذا ( !akerning )
      أرجع FT_THROW( Invalid_آرگومان );

    FT_FACE_پیدا_کن_SERVICE( face, service, KERNING );
    إذا ( !service )
      أرجع FT_THROW( Unimplemented_Feature );

    error = service->get_track( face,
                                نقطه_اندازه,
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
    FT_CharMap*  حد;


    إذا ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );

    /* FT_ENCODING_NONE is a valid encoding for BDF, PCF, and Windows FNT */
    إذا ( encoding == FT_ENCODING_NONE && !face->رقم_charmaps )
      أرجع FT_THROW( Invalid_آرگومان );

    /* FT_ENCODING_UNICODE is special.  We try to find the `best' Unicode */
    /* charmap available, i.e., one with UCS-4 characters, if possible.   */
    /*                                                                    */
    /* This is done by find_unicode_charmap() above, to share code.       */
    إذا ( encoding == FT_ENCODING_UNICODE )
      أرجع پیدا_کن_unicode_charmap( face );

    cur = face->charmaps;
    إذا ( !cur )
      أرجع FT_THROW( Invalid_CharMap_Handle );

    حد = cur + face->رقم_charmaps;

    لكل ( ; cur < حد; cur++ )
    {
      إذا ( cur[0]->encoding == encoding )
      {
        face->charmap = cur[0];
        أرجع FT_Err_Ok;
      }
    }

    أرجع FT_THROW( Invalid_آرگومان );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_مجموعه_Charmap( FT_Face     face,
                  FT_CharMap  charmap )
  {
    FT_CharMap*  cur;
    FT_CharMap*  حد;


    إذا ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );

    cur = face->charmaps;
    إذا ( !cur || !charmap )
      أرجع FT_THROW( Invalid_CharMap_Handle );

    حد = cur + face->رقم_charmaps;

    لكل ( ; cur < حد; cur++ )
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

    أرجع FT_THROW( Invalid_آرگومان );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Int )
  FT_Get_Charmap_شاخص( FT_CharMap  charmap )
  {
    FT_Int  i;


    إذا ( !charmap || !charmap->face )
      أرجع -1;

    لكل ( i = 0; i < charmap->face->رقم_charmaps; i++ )
      إذا ( charmap->face->charmaps[i] == charmap )
        اكسر;

    FT_ASSERT( i < charmap->face->رقم_charmaps );

    أرجع i;
  }


  ساكن فراغ
  ft_cmap_انجامشده_داخلی( FT_CMap  cmap )
  {
    FT_CMap_Class  clazz  = cmap->clazz;
    FT_Face        face   = cmap->charmap.face;
    FT_حافظه      حافظه = FT_FACE_حافظه( face );


    إذا ( clazz->انجامشده )
      clazz->انجامشده( cmap );

    FT_FREE( cmap );
  }


  FT_پایه_DEF( فراغ )
  FT_CMap_انجامشده( FT_CMap  cmap )
  {
    إذا ( cmap )
    {
      FT_Face    face   = cmap->charmap.face;
      FT_حافظه  حافظه = FT_FACE_حافظه( face );
      FT_Error   error;
      FT_Int     i, j;


      لكل ( i = 0; i < face->رقم_charmaps; i++ )
      {
        إذا ( (FT_CMap)face->charmaps[i] == cmap )
        {
          FT_CharMap  آخرین_charmap = face->charmaps[face->رقم_charmaps - 1];


          إذا ( FT_QRENEW_ARRAY( face->charmaps,
                                face->رقم_charmaps,
                                face->رقم_charmaps - 1 ) )
            أرجع;

          /* remove it from our list of charmaps */
          لكل ( j = i + 1; j < face->رقم_charmaps; j++ )
          {
            إذا ( j == face->رقم_charmaps - 1 )
              face->charmaps[j - 1] = آخرین_charmap;
            وإلا
              face->charmaps[j - 1] = face->charmaps[j];
          }

          face->رقم_charmaps--;

          إذا ( (FT_CMap)face->charmap == cmap )
            face->charmap = عدم;

          ft_cmap_انجامشده_داخلی( cmap );

          اكسر;
        }
      }
    }
  }


  FT_پایه_DEF( FT_Error )
  FT_CMap_New( FT_CMap_Class  clazz,
               FT_اشاره_گر     init_داده,
               FT_CharMap     charmap,
               FT_CMap       *acmap )
  {
    FT_Error   error;
    FT_Face    face;
    FT_حافظه  حافظه;
    FT_CMap    cmap = عدم;


    إذا ( !clazz || !charmap || !charmap->face )
      أرجع FT_THROW( Invalid_آرگومان );

    face   = charmap->face;
    حافظه = FT_FACE_حافظه( face );

    إذا ( !FT_ALLOC( cmap, clazz->اندازه ) )
    {
      cmap->charmap = *charmap;
      cmap->clazz   = clazz;

      إذا ( clazz->init )
      {
        error = clazz->init( cmap, init_داده );
        إذا ( error )
          اذهب Fail;
      }

      /* add it to our list of charmaps */
      إذا ( FT_QRENEW_ARRAY( face->charmaps,
                            face->رقم_charmaps,
                            face->رقم_charmaps + 1 ) )
        اذهب Fail;

      face->charmaps[face->رقم_charmaps++] = (FT_CharMap)cmap;
    }

  Exit:
    إذا ( acmap )
      *acmap = cmap;

    أرجع error;

  Fail:
    ft_cmap_انجامشده_داخلی( cmap );
    cmap = عدم;
    اذهب Exit;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt )
  FT_Get_Char_شاخص( FT_Face   face,
                     FT_ULong  charcode )
  {
    FT_UInt  نتیجه = 0;


    إذا ( face && face->charmap )
    {
      FT_CMap  cmap = FT_CMAP( face->charmap );


      إذا ( charcode > 0xFFFFFFFFUL )
      {
        FT_TRACE1(( "FT_Get_Char_Index: too large charcode" ));
        FT_TRACE1(( " 0x%lx is truncated\n", charcode ));
      }

      نتیجه = cmap->clazz->char_شاخص( cmap, (FT_UInt32)charcode );
      إذا ( نتیجه >= (FT_UInt)face->رقم_glyphs )
        نتیجه = 0;
    }

    أرجع نتیجه;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_ULong )
  FT_Get_اولی_Char( FT_Face   face,
                     FT_UInt  *agindex )
  {
    FT_ULong  نتیجه = 0;
    FT_UInt   gindex = 0;


    /* only do something if we have a charmap, and we have glyphs at all */
    إذا ( face && face->charmap && face->رقم_glyphs )
    {
      gindex = FT_Get_Char_شاخص( face, 0 );
      إذا ( gindex == 0 )
        نتیجه = FT_Get_بعدی_Char( face, 0, &gindex );
    }

    إذا ( agindex )
      *agindex = gindex;

    أرجع نتیجه;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_ULong )
  FT_Get_بعدی_Char( FT_Face   face,
                    FT_ULong  charcode,
                    FT_UInt  *agindex )
  {
    FT_ULong  نتیجه = 0;
    FT_UInt   gindex = 0;


    إذا ( face && face->charmap && face->رقم_glyphs )
    {
      FT_UInt32  کد = (FT_UInt32)charcode;
      FT_CMap    cmap = FT_CMAP( face->charmap );


      افعل
      {
        gindex = cmap->clazz->char_بعدی( cmap, &کد );

      } طالما ( gindex >= (FT_UInt)face->رقم_glyphs );

      نتیجه = ( gindex == 0 ) ? 0 : کد;
    }

    إذا ( agindex )
      *agindex = gindex;

    أرجع نتیجه;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Face_Properties( FT_Face        face,
                      FT_UInt        رقم_properties,
                      FT_Parameter*  properties )
  {
    FT_Error  error = FT_Err_Ok;


    إذا ( رقم_properties > 0 && !properties )
    {
      error = FT_THROW( Invalid_آرگومان );
      اذهب Exit;
    }

    لكل ( ; رقم_properties > 0; رقم_properties-- )
    {
      إذا ( properties->برچسب == FT_PARAM_برچسب_STEM_DARKENING )
      {
        إذا ( properties->داده )
        {
          إذا ( *( (FT_Bool*)properties->داده ) == TRUE )
            face->داخلی->no_stem_darkening = FALSE;
          وإلا
            face->داخلی->no_stem_darkening = TRUE;
        }
        وإلا
        {
          /* use module default */
          face->داخلی->no_stem_darkening = -1;
        }
      }
      وإلا إذا ( properties->برچسب == FT_PARAM_برچسب_LCD_FILTER_WEIGHTS )
      {
        error = FT_THROW( Unimplemented_Feature );
        اذهب Exit;
      }
      وإلا إذا ( properties->برچسب == FT_PARAM_برچسب_RANDOM_دانه )
      {
        إذا ( properties->داده )
        {
          face->داخلی->random_دانه = *( (FT_Int32*)properties->داده );
          إذا ( face->داخلی->random_دانه < 0 )
            face->داخلی->random_دانه = 0;
        }
        وإلا
        {
          /* use module default */
          face->داخلی->random_دانه = -1;
        }
      }
      وإلا
      {
        error = FT_THROW( Invalid_آرگومان );
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
  FT_Face_Get_Char_Variant_شاخص( FT_Face   face,
                               FT_ULong  charcode,
                               FT_ULong  variantSelector )
  {
    FT_UInt  نتیجه = 0;


    إذا ( face                                           &&
         face->charmap                                  &&
         face->charmap->encoding == FT_ENCODING_UNICODE )
    {
      FT_CharMap  charmap = پیدا_کن_variant_selector_charmap( face );
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

        نتیجه = vcmap->clazz->char_var_شاخص( vcmap, ucmap,
                                               (FT_UInt32)charcode,
                                               (FT_UInt32)variantSelector );
      }
    }

    أرجع نتیجه;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Int )
  FT_Face_GetCharVariantIsDefault( FT_Face   face,
                                   FT_ULong  charcode,
                                   FT_ULong  variantSelector )
  {
    FT_Int  نتیجه = -1;


    إذا ( face )
    {
      FT_CharMap  charmap = پیدا_کن_variant_selector_charmap( face );


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

        نتیجه = vcmap->clazz->char_var_default( vcmap,
                                                 (FT_UInt32)charcode,
                                                 (FT_UInt32)variantSelector );
      }
    }

    أرجع نتیجه;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt32* )
  FT_Face_GetVariantSelectors( FT_Face  face )
  {
    FT_UInt32  *نتیجه = عدم;


    إذا ( face )
    {
      FT_CharMap  charmap = پیدا_کن_variant_selector_charmap( face );


      إذا ( charmap )
      {
        FT_CMap    vcmap  = FT_CMAP( charmap );
        FT_حافظه  حافظه = FT_FACE_حافظه( face );


        نتیجه = vcmap->clazz->variant_list( vcmap, حافظه );
      }
    }

    أرجع نتیجه;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt32* )
  FT_Face_Get_Variants_از_Char( FT_Face   face,
                             FT_ULong  charcode )
  {
    FT_UInt32  *نتیجه = عدم;


    إذا ( face )
    {
      FT_CharMap  charmap = پیدا_کن_variant_selector_charmap( face );


      إذا ( charmap )
      {
        FT_CMap    vcmap  = FT_CMAP( charmap );
        FT_حافظه  حافظه = FT_FACE_حافظه( face );


        إذا ( charcode > 0xFFFFFFFFUL )
        {
          FT_TRACE1(( "FT_Face_GetVariantsOfChar: too large charcode" ));
          FT_TRACE1(( " 0x%lx is truncated\n", charcode ));
        }

        نتیجه = vcmap->clazz->charvariant_list( vcmap, حافظه,
                                                 (FT_UInt32)charcode );
      }
    }
    أرجع نتیجه;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt32* )
  FT_Face_Get_Chars_از_Variant( FT_Face   face,
                             FT_ULong  variantSelector )
  {
    FT_UInt32  *نتیجه = عدم;


    إذا ( face )
    {
      FT_CharMap  charmap = پیدا_کن_variant_selector_charmap( face );


      إذا ( charmap )
      {
        FT_CMap    vcmap  = FT_CMAP( charmap );
        FT_حافظه  حافظه = FT_FACE_حافظه( face );


        إذا ( variantSelector > 0xFFFFFFFFUL )
        {
          FT_TRACE1(( "FT_Get_Char_Index: too large variantSelector" ));
          FT_TRACE1(( " 0x%lx is truncated\n", variantSelector ));
        }

        نتیجه = vcmap->clazz->variantchar_list( vcmap, حافظه,
                                                 (FT_UInt32)variantSelector );
      }
    }

    أرجع نتیجه;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt )
  FT_Get_Name_شاخص( FT_Face           face,
                     ثابت FT_String*  glyph_name )
  {
    FT_UInt  نتیجه = 0;


    إذا ( face                       &&
         FT_HAS_GLYPH_نامها( face ) &&
         glyph_name                 )
    {
      FT_Service_GlyphDict  service;


      FT_FACE_LOOKUP_SERVICE( face,
                              service,
                              GLYPH_DICT );

      إذا ( service && service->name_شاخص )
        نتیجه = service->name_شاخص( face, glyph_name );
    }

    أرجع نتیجه;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Glyph_Name( FT_Face     face,
                     FT_UInt     glyph_شاخص,
                     FT_اشاره_گر  میانگیر,
                     FT_UInt     میانگیر_max )
  {
    FT_Error              error;
    FT_Service_GlyphDict  service;


    إذا ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );

    إذا ( !میانگیر || میانگیر_max == 0 )
      أرجع FT_THROW( Invalid_آرگومان );

    /* clean up buffer */
    ((FT_Byte*)میانگیر)[0] = '\0';

    إذا ( (FT_Long)glyph_شاخص >= face->رقم_glyphs )
      أرجع FT_THROW( Invalid_Glyph_شاخص );

    إذا ( !FT_HAS_GLYPH_نامها( face ) )
      أرجع FT_THROW( Invalid_آرگومان );

    FT_FACE_LOOKUP_SERVICE( face, service, GLYPH_DICT );
    إذا ( service && service->get_name )
      error = service->get_name( face, glyph_شاخص, میانگیر, میانگیر_max );
    وإلا
      error = FT_THROW( Invalid_آرگومان );

    أرجع error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( ثابت حرف* )
  FT_Get_Postscript_Name( FT_Face  face )
  {
    ثابت حرف*  نتیجه = عدم;


    إذا ( !face )
      اذهب Exit;

    إذا ( !نتیجه )
    {
      FT_Service_PsFontName  service;


      FT_FACE_LOOKUP_SERVICE( face,
                              service,
                              POSTSCRIPT_FONT_NAME );

      إذا ( service && service->get_ps_font_name )
        نتیجه = service->get_ps_font_name( face );
    }

  Exit:
    أرجع نتیجه;
  }


  /* documentation is in tttables.h */

  FT_EXPORT_DEF( فراغ* )
  FT_Get_Sfnt_جدول( FT_Face      face,
                     FT_Sfnt_برچسب  برچسب )
  {
    فراغ*                  جدول = عدم;
    FT_Service_SFNT_جدول  service;


    إذا ( face && FT_IS_SFNT( face ) )
    {
      FT_FACE_پیدا_کن_SERVICE( face, service, SFNT_جدول );
      إذا ( service )
        جدول = service->get_جدول( face, برچسب );
    }

    أرجع جدول;
  }


  /* documentation is in tttables.h */

  FT_EXPORT_DEF( FT_Error )
  FT_بارگیری_Sfnt_جدول( FT_Face    face,
                      FT_ULong   برچسب,
                      FT_Long    جابه_جایی,
                      FT_Byte*   میانگیر,
                      FT_ULong*  درازی )
  {
    FT_Service_SFNT_جدول  service;


    إذا ( !face || !FT_IS_SFNT( face ) )
      أرجع FT_THROW( Invalid_Face_Handle );

    FT_FACE_پیدا_کن_SERVICE( face, service, SFNT_جدول );
    إذا ( !service )
      أرجع FT_THROW( Unimplemented_Feature );

    أرجع service->بارگیری_جدول( face, برچسب, جابه_جایی, میانگیر, درازی );
  }


  /* documentation is in tttables.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Sfnt_جدول_Info( FT_Face    face,
                      FT_UInt    جدول_شاخص,
                      FT_ULong  *برچسب,
                      FT_ULong  *درازی )
  {
    FT_Service_SFNT_جدول  service;
    FT_ULong               جابه_جایی;


    /* test for valid `length' delayed to `service->table_info' */

    إذا ( !face || !FT_IS_SFNT( face ) )
      أرجع FT_THROW( Invalid_Face_Handle );

    FT_FACE_پیدا_کن_SERVICE( face, service, SFNT_جدول );
    إذا ( !service )
      أرجع FT_THROW( Unimplemented_Feature );

    أرجع service->جدول_info( face, جدول_شاخص, برچسب, &جابه_جایی, درازی );
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
    FT_FACE_پیدا_کن_SERVICE( face, service, TT_CMAP );
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
    FT_FACE_پیدا_کن_SERVICE( face, service, TT_CMAP );
    إذا ( !service )
      أرجع -1;
    إذا ( service->get_cmap_info( charmap, &cmap_info ))
      أرجع -1;

    أرجع cmap_info.format;
  }


  /* documentation is in ftsizes.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Activate_اندازه( FT_اندازه  اندازه )
  {
    FT_Face  face;


    إذا ( !اندازه )
      أرجع FT_THROW( Invalid_اندازه_Handle );

    face = اندازه->face;
    إذا ( !face || !face->driver )
      أرجع FT_THROW( Invalid_Face_Handle );

    /* we don't need anything more complex than that; all size objects */
    /* are already listed by the face                                  */
    face->اندازه = اندازه;

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
  FT_پایه_DEF( FT_Renderer )
  FT_Lookup_Renderer( FT_Library       library,
                      FT_Glyph_Format  format,
                      FT_List_گره*     گره )
  {
    FT_List_گره  cur;
    FT_Renderer  نتیجه = عدم;


    إذا ( !library )
      اذهب Exit;

    cur = library->renderers.سر;

    إذا ( گره )
    {
      إذا ( *گره )
        cur = (*گره)->بعدی;
      *گره = عدم;
    }

    طالما ( cur )
    {
      FT_Renderer  renderer = FT_RENDERER( cur->داده );


      إذا ( renderer->glyph_format == format )
      {
        إذا ( گره )
          *گره = cur;

        نتیجه = renderer;
        اكسر;
      }
      cur = cur->بعدی;
    }

  Exit:
    أرجع نتیجه;
  }


  ساكن FT_Renderer
  ft_lookup_glyph_renderer( FT_Glyph_جایگاه  جایگاه )
  {
    FT_Face      face    = جایگاه->face;
    FT_Library   library = FT_FACE_LIBRARY( face );
    FT_Renderer  نتیجه  = library->cur_renderer;


    إذا ( !نتیجه || نتیجه->glyph_format != جایگاه->format )
      نتیجه = FT_Lookup_Renderer( library, جایگاه->format, 0 );

    أرجع نتیجه;
  }


  ساكن فراغ
  ft_مجموعه_current_renderer( FT_Library  library )
  {
    FT_Renderer  renderer;


    renderer = FT_Lookup_Renderer( library, FT_GLYPH_FORMAT_OUTLINE, 0 );
    library->cur_renderer = renderer;
  }


  ساكن FT_Error
  ft_جمع_کن_renderer( FT_Module  module )
  {
    FT_Library   library = module->library;
    FT_حافظه    حافظه  = library->حافظه;
    FT_Error     error;
    FT_List_گره  گره    = عدم;


    إذا ( FT_QNEW( گره ) )
      اذهب Exit;

    {
      FT_Renderer         render = FT_RENDERER( module );
      FT_Renderer_Class*  clazz  = (FT_Renderer_Class*)module->clazz;


      render->clazz        = clazz;
      render->glyph_format = clazz->glyph_format;

      /* allocate raster object if needed */
      إذا ( clazz->raster_class && clazz->raster_class->raster_new )
      {
        error = clazz->raster_class->raster_new( حافظه, &render->raster );
        إذا ( error )
          اذهب Fail;

        render->raster_render = clazz->raster_class->raster_render;
        render->render        = clazz->render_glyph;
      }

#اگر_تعریف_شده FT_CONFIG_OPTION_SVG
      إذا ( clazz->glyph_format == FT_GLYPH_FORMAT_SVG )
        render->render = clazz->render_glyph;
#پایان_اگر

      /* add to list */
      گره->داده = module;
      FT_List_جمع_کن( &library->renderers, گره );

      ft_مجموعه_current_renderer( library );
    }

  Fail:
    إذا ( error )
      FT_FREE( گره );

  Exit:
    أرجع error;
  }


  ساكن فراغ
  ft_remove_renderer( FT_Module  module )
  {
    FT_Library   library;
    FT_حافظه    حافظه;
    FT_List_گره  گره;


    library = module->library;
    إذا ( !library )
      أرجع;

    حافظه = library->حافظه;

    گره = FT_List_پیدا_کن( &library->renderers, module );
    إذا ( گره )
    {
      FT_Renderer  render = FT_RENDERER( module );


      /* release raster object, if any */
      إذا ( render->raster )
        render->clazz->raster_class->raster_انجامشده( render->raster );

      /* remove from list */
      FT_List_Remove( &library->renderers, گره );
      FT_FREE( گره );

      ft_مجموعه_current_renderer( library );
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
  FT_مجموعه_Renderer( FT_Library     library,
                   FT_Renderer    renderer,
                   FT_UInt        رقم_params,
                   FT_Parameter*  parameters )
  {
    FT_List_گره  گره;
    FT_Error     error = FT_Err_Ok;

    FT_Renderer_مجموعه_نما_Func  مجموعه_نما;


    إذا ( !library )
    {
      error = FT_THROW( Invalid_Library_Handle );
      اذهب Exit;
    }

    إذا ( !renderer )
    {
      error = FT_THROW( Invalid_آرگومان );
      اذهب Exit;
    }

    إذا ( رقم_params > 0 && !parameters )
    {
      error = FT_THROW( Invalid_آرگومان );
      اذهب Exit;
    }

    گره = FT_List_پیدا_کن( &library->renderers, renderer );
    إذا ( !گره )
    {
      error = FT_THROW( Invalid_آرگومان );
      اذهب Exit;
    }

    FT_List_به_بالا( &library->renderers, گره );

    إذا ( renderer->glyph_format == FT_GLYPH_FORMAT_OUTLINE )
      library->cur_renderer = renderer;

    مجموعه_نما = renderer->clazz->مجموعه_نما;

    لكل ( ; رقم_params > 0; رقم_params-- )
    {
      error = مجموعه_نما( renderer, parameters->برچسب, parameters->داده );
      إذا ( error )
        اكسر;
      parameters++;
    }

  Exit:
    أرجع error;
  }


  FT_پایه_DEF( FT_Error )
  FT_Render_Glyph_داخلی( FT_Library      library,
                            FT_Glyph_جایگاه    جایگاه,
                            FT_Render_نما  render_نما )
  {
    FT_Error     error = FT_Err_Ok;
    FT_Face      face  = جایگاه->face;
    FT_Renderer  renderer;
    FT_List_گره  گره  = عدم;


    /* try to render colored glyph layers as a special case */
    إذا ( جایگاه->داخلی->بارگیری_flags & FT_بارگیری_COLOR &&
         جایگاه->format == FT_GLYPH_FORMAT_OUTLINE    )
    {
      FT_LayerIterator  iterator;

      FT_UInt  پایه_glyph = جایگاه->glyph_شاخص;

      FT_Bool  have_layers;
      FT_UInt  glyph_شاخص;
      FT_UInt  color_شاخص;


      iterator.p  = عدم;
      have_layers = FT_Get_Color_Glyph_Layer( face,
                                              پایه_glyph,
                                              &glyph_شاخص,
                                              &color_شاخص,
                                              &iterator );
      إذا ( have_layers )
      {
        error = FT_New_Glyph_جایگاه( face, عدم );
        إذا ( !error )
        {
          TT_Face       ttface = (TT_Face)face;
          SFNT_Service  sfnt   = (SFNT_Service)ttface->sfnt;


          افعل
          {
            FT_Int32  بارگیری_flags = جایگاه->داخلی->بارگیری_flags;


            /* disable the `FT_LOAD_COLOR' flag to avoid recursion */
            /* right here in this function                         */
            بارگیری_flags &= ~FT_بارگیری_COLOR;

            /* render into the new `face->glyph' glyph slot */
            بارگیری_flags |= FT_بارگیری_RENDER | FT_بارگیری_NO_BITMAP;

            error = FT_بارگیری_Glyph( face, glyph_شاخص, بارگیری_flags );
            إذا ( error )
              اكسر;

            /* blend new `face->glyph' into old `slot'; */
            /* at the first call, `slot' is still empty */
            error = sfnt->colr_blend( ttface,
                                      color_شاخص,
                                      جایگاه,
                                      face->glyph );
            إذا ( error )
              اكسر;

          } طالما ( FT_Get_Color_Glyph_Layer( face,
                                              پایه_glyph,
                                              &glyph_شاخص,
                                              &color_شاخص,
                                              &iterator ) );

          إذا ( !error )
            جایگاه->format = FT_GLYPH_FORMAT_BITMAP;

          /* this call also restores `slot' as the glyph slot */
          FT_انجامشده_Glyph_جایگاه( face->glyph );
        }

        إذا ( !error )
          أرجع error;

        /* Failed to do the colored layer.  Draw outline instead. */
        جایگاه->format = FT_GLYPH_FORMAT_OUTLINE;
      }
    }

    /* small shortcut for the very common case */
    إذا ( جایگاه->format == FT_GLYPH_FORMAT_OUTLINE )
    {
      renderer = library->cur_renderer;
      گره     = library->renderers.سر;
    }
    وإلا
      renderer = FT_Lookup_Renderer( library, جایگاه->format, &گره );

    error = FT_ERR( Cannot_Render_Glyph );
    طالما ( renderer )
    {
      error = renderer->render( renderer, جایگاه, render_نما, عدم );
      إذا ( !error                                   ||
           FT_ERR_NEQ( error, Cannot_Render_Glyph ) )
        اكسر;

      /* `FT_Err_Cannot_Render_Glyph` is returned if the render mode */
      /* is unsupported by the current renderer for this glyph image */
      /* format.                                                     */

      /* now, look for another renderer that supports the same */
      /* format.                                               */
      renderer = FT_Lookup_Renderer( library, جایگاه->format, &گره );
    }

    /* it is not an error if we cannot render a bitmap glyph */
    إذا ( FT_ERR_EQ( error, Cannot_Render_Glyph ) &&
         جایگاه->format == FT_GLYPH_FORMAT_BITMAP  )
      error = FT_Err_Ok;

#اگر_تعریف_شده FT_DEBUG_LEVEL_TRACE

#لغو_تعریف  FT_COMPONENT
#تعریف_کن FT_COMPONENT  checksum

    /*
     * Computing the MD5 checksum is expensive, unnecessarily distorting a
     * possible profiling of FreeType if compiled with tracing support.  For
     * this reason, we execute the following code only if explicitly
     * requested.
     */

    /* we use FT_TRACE3 in this block */
    إذا ( !error                               &&
         ft_trace_سطوح[trace_checksum] >= 3 &&
         جایگاه->bitmap.میانگیر                  )
    {
      FT_Bitmap  bitmap;
      FT_Error   err;


      FT_Bitmap_Init( &bitmap );

      /* we convert to a single bitmap format for computing the checksum */
      /* this also converts the bitmap flow to `down' (i.e., pitch > 0)  */
      err = FT_Bitmap_Convert( library, &جایگاه->bitmap, &bitmap, 1 );
      إذا ( !err )
      {
        MD5_CTX        ctx;
        بیعلامت حرف  md5[16];
        بیعلامت دراز  coverage = 0;
        صحيح            i, j;
        صحيح            ردیفها  = (صحيح)bitmap.ردیفها;
        صحيح            pitch = bitmap.pitch;


        FT_TRACE3(( "FT_Render_Glyph: bitmap %dx%d, %s (mode %d)\n",
                    pitch,
                    ردیفها,
                    pixel_modes[جایگاه->bitmap.pixel_نما],
                    جایگاه->bitmap.pixel_نما ));

        لكل ( i = 0; i < ردیفها; i++ )
          لكل ( j = 0; j < pitch; j++ )
            coverage += bitmap.میانگیر[i * pitch + j];

        FT_TRACE3(( "  Total coverage: %lu\n", coverage ));

        MD5_Init( &ctx );
        إذا ( bitmap.میانگیر )
          MD5_Update( &ctx, bitmap.میانگیر,
                      (بیعلامت دراز)ردیفها * (بیعلامت دراز)pitch );
        MD5_Final( md5, &ctx );

        FT_TRACE3(( "  MD5 checksum: " ));
        لكل ( i = 0; i < 16; i++ )
          FT_TRACE3(( "%02X", md5[i] ));
        FT_TRACE3(( "\n" ));
      }

      FT_Bitmap_انجامشده( library, &bitmap );
    }

    /*
     * Dump bitmap in Netpbm format (PBM or PGM).
     */

    /* we use FT_TRACE7 in this block */
    إذا ( !error                               &&
         ft_trace_سطوح[trace_checksum] >= 7 &&
         جایگاه->bitmap.میانگیر                  )
    {
      إذا ( جایگاه->bitmap.ردیفها  < 128U &&
           جایگاه->bitmap.عرض < 128U )
      {
        صحيح  ردیفها  = (صحيح)جایگاه->bitmap.ردیفها;
        صحيح  عرض = (صحيح)جایگاه->bitmap.عرض;
        صحيح  pitch =      جایگاه->bitmap.pitch;
        صحيح  i, j, m;

        بیعلامت حرف*  topleft = جایگاه->bitmap.میانگیر;


        إذا ( pitch < 0 )
          topleft -= pitch * ( ردیفها - 1 );

        FT_TRACE7(( "Netpbm image: start\n" ));
        بدل ( جایگاه->bitmap.pixel_نما )
        {
        حالة FT_PIXEL_نما_MONO:
          FT_TRACE7(( "P1 %d %d\n", عرض, ردیفها ));
          لكل ( i = 0; i < ردیفها; i++ )
          {
            لكل ( j = 0; j < عرض; )
              لكل ( m = 128; m > 0 && j < عرض; m >>= 1, j++ )
                FT_TRACE7(( " %d",
                            ( topleft[i * pitch + j / 8] & m ) != 0 ));
            FT_TRACE7(( "\n" ));
          }
          اكسر;

        افتراضي:
          FT_TRACE7(( "P2 %d %d 255\n", عرض, ردیفها ));
          لكل ( i = 0; i < ردیفها; i++ )
          {
            لكل ( j = 0; j < عرض; j += 1 )
              FT_TRACE7(( " %3u", topleft[i * pitch + j] ));
            FT_TRACE7(( "\n" ));
          }
        }
        FT_TRACE7(( "Netpbm image: end\n" ));
      }
      وإلا
        FT_TRACE7(( "Netpbm image: too large, omitted\n" ));
    }

#لغو_تعریف  FT_COMPONENT
#تعریف_کن FT_COMPONENT  objs

#پایان_اگر /* FT_DEBUG_LEVEL_TRACE */

    أرجع error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Render_Glyph( FT_Glyph_جایگاه    جایگاه,
                   FT_Render_نما  render_نما )
  {
    FT_Library  library;


    إذا ( !جایگاه || !جایگاه->face )
      أرجع FT_THROW( Invalid_آرگومان );

    library = FT_FACE_LIBRARY( جایگاه->face );

    أرجع FT_Render_Glyph_داخلی( library, جایگاه, render_نما );
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
    FT_حافظه               حافظه  = module->حافظه;


    إذا ( library && library->auto_hinter == module )
      library->auto_hinter = عدم;

    /* if the module is a renderer */
    إذا ( FT_MODULE_IS_RENDERER( module ) )
      ft_remove_renderer( module );

    /* if the module is a font driver, add some steps */
    إذا ( FT_MODULE_IS_DRIVER( module ) )
      Destroy_Driver( FT_DRIVER( module ) );

    /* finalize the module object */
    إذا ( clazz->module_انجامشده )
      clazz->module_انجامشده( module );

    /* discard it */
    FT_FREE( module );
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_جمع_کن_Module( FT_Library              library,
                 ثابت FT_Module_Class*  clazz )
  {
    FT_Error   error;
    FT_حافظه  حافظه;
    FT_Module  module = عدم;
    FT_UInt    nn;


#تعریف_کن FREETYPE_VER_FIXED  ( ( (FT_Long)FREETYPE_MAJOR << 16 ) | \
                                FREETYPE_MINOR                  )

    إذا ( !library )
      أرجع FT_THROW( Invalid_Library_Handle );

    إذا ( !clazz )
      أرجع FT_THROW( Invalid_آرگومان );

    /* check FreeType version */
    إذا ( clazz->module_requires > FREETYPE_VER_FIXED )
      أرجع FT_THROW( Invalid_نسخه );

    /* look for a module with the same name in the library's table */
    لكل ( nn = 0; nn < library->رقم_modules; nn++ )
    {
      module = library->modules[nn];
      إذا ( ft_strcmp( module->clazz->module_name, clazz->module_name ) == 0 )
      {
        /* this installed module has the same name, compare their versions */
        إذا ( clazz->module_نسخه <= module->clazz->module_نسخه )
          أرجع FT_THROW( پایینی_Module_نسخه );

        /* remove the module from our list, then exit the loop to replace */
        /* it by our new version..                                        */
        FT_Remove_Module( library, module );
        اكسر;
      }
    }

    حافظه = library->حافظه;
    error  = FT_Err_Ok;

    إذا ( library->رقم_modules >= FT_MAX_MODULES )
    {
      error = FT_THROW( Too_Many_Drivers );
      اذهب Exit;
    }

    /* allocate module object */
    إذا ( FT_ALLOC( module, clazz->module_اندازه ) )
      اذهب Exit;

    /* base initialization */
    module->clazz   = clazz;
    module->library = library;
    module->حافظه  = حافظه;

    /* check whether the module is a renderer - this must be performed */
    /* before the normal module initialization                         */
    إذا ( FT_MODULE_IS_RENDERER( module ) )
    {
      /* add to the renderers list */
      error = ft_جمع_کن_renderer( module );
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
    library->modules[library->رقم_modules++] = module;

  Exit:
    أرجع error;

  Fail:
    إذا ( FT_MODULE_IS_RENDERER( module ) )
    {
      FT_Renderer  renderer = FT_RENDERER( module );


      إذا ( renderer->clazz                                          &&
           renderer->clazz->glyph_format == FT_GLYPH_FORMAT_OUTLINE &&
           renderer->raster                                         )
        renderer->clazz->raster_class->raster_انجامشده( renderer->raster );
    }

    FT_FREE( module );
    اذهب Exit;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Module )
  FT_Get_Module( FT_Library   library,
                 ثابت حرف*  module_name )
  {
    FT_Module   نتیجه = عدم;
    FT_Module*  cur;
    FT_Module*  حد;


    إذا ( !library || !module_name )
      أرجع نتیجه;

    cur   = library->modules;
    حد = cur + library->رقم_modules;

    لكل ( ; cur < حد; cur++ )
      إذا ( ft_strcmp( cur[0]->clazz->module_name, module_name ) == 0 )
      {
        نتیجه = cur[0];
        اكسر;
      }

    أرجع نتیجه;
  }


  /* documentation is in ftobjs.h */

  FT_پایه_DEF( ثابت فراغ* )
  FT_Get_Module_Interface( FT_Library   library,
                           ثابت حرف*  mod_name )
  {
    FT_Module  module;


    /* test for valid `library' delayed to FT_Get_Module() */

    module = FT_Get_Module( library, mod_name );

    أرجع module ? module->clazz->module_interface : 0;
  }


  FT_پایه_DEF( FT_اشاره_گر )
  ft_module_get_service( FT_Module    module,
                         ثابت حرف*  service_id,
                         FT_Bool      global )
  {
    FT_اشاره_گر  نتیجه = عدم;


    إذا ( module )
    {
      FT_ASSERT( module->clazz && module->clazz->get_interface );

      /* first, look for the service in the module */
      إذا ( module->clazz->get_interface )
        نتیجه = module->clazz->get_interface( module, service_id );

      إذا ( global && !نتیجه )
      {
        /* we didn't find it, look in all other modules then */
        FT_Library  library = module->library;
        FT_Module*  cur     = library->modules;
        FT_Module*  حد   = cur + library->رقم_modules;


        لكل ( ; cur < حد; cur++ )
        {
          إذا ( cur[0] != module )
          {
            FT_ASSERT( cur[0]->clazz );

            إذا ( cur[0]->clazz->get_interface )
            {
              نتیجه = cur[0]->clazz->get_interface( cur[0], service_id );
              إذا ( نتیجه )
                اكسر;
            }
          }
        }
      }
    }

    أرجع نتیجه;
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
      FT_Module*  حد = cur + library->رقم_modules;


      لكل ( ; cur < حد; cur++ )
      {
        إذا ( cur[0] == module )
        {
          /* remove it from the table */
          library->رقم_modules--;
          حد--;
          طالما ( cur < حد )
          {
            cur[0] = cur[1];
            cur++;
          }
          حد[0] = عدم;

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
                  فراغ*             مقدار,
                  FT_Bool           مجموعه,
                  FT_Bool           مقدار_is_string )
  {
    FT_Module*           cur;
    FT_Module*           حد;
    FT_Module_Interface  interface;

    FT_Service_Properties  service;

#اگر_تعریف_شده FT_DEBUG_LEVEL_ERROR
    ثابت FT_String*  مجموعه_name  = "FT_Property_Set";
    ثابت FT_String*  get_name  = "FT_Property_Get";
    ثابت FT_String*  func_name = مجموعه ? مجموعه_name : get_name;
#پایان_اگر

    FT_Bool  missing_func;


    إذا ( !library )
      أرجع FT_THROW( Invalid_Library_Handle );

    إذا ( !module_name || !property_name || !مقدار )
      أرجع FT_THROW( Invalid_آرگومان );

    cur   = library->modules;
    حد = cur + library->رقم_modules;

    /* search module */
    لكل ( ; cur < حد; cur++ )
      إذا ( !ft_strcmp( cur[0]->clazz->module_name, module_name ) )
        اكسر;

    إذا ( cur == حد )
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

    إذا ( مجموعه )
      missing_func = FT_BOOL( !service->مجموعه_property );
    وإلا
      missing_func = FT_BOOL( !service->get_property );

    إذا ( missing_func )
    {
      FT_TRACE2(( "%s: property service of module `%s' is broken\n",
                  func_name, module_name ));
      أرجع FT_THROW( Unimplemented_Feature );
    }

    أرجع مجموعه ? service->مجموعه_property( cur[0],
                                        property_name,
                                        مقدار,
                                        مقدار_is_string )
               : service->get_property( cur[0],
                                        property_name,
                                        مقدار );
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Property_مجموعه( FT_Library        library,
                   ثابت FT_String*  module_name,
                   ثابت FT_String*  property_name,
                   ثابت فراغ*       مقدار )
  {
    أرجع ft_property_do( library,
                           module_name,
                           property_name,
                           (فراغ*)مقدار,
                           TRUE,
                           FALSE );
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Property_Get( FT_Library        library,
                   ثابت FT_String*  module_name,
                   ثابت FT_String*  property_name,
                   فراغ*             مقدار )
  {
    أرجع ft_property_do( library,
                           module_name,
                           property_name,
                           مقدار,
                           FALSE,
                           FALSE );
  }


#اگر_تعریف_شده FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES

  /* this variant is used for handling the FREETYPE_PROPERTIES */
  /* environment variable                                      */

  FT_پایه_DEF( FT_Error )
  ft_property_string_مجموعه( FT_Library        library,
                          ثابت FT_String*  module_name,
                          ثابت FT_String*  property_name,
                          FT_String*        مقدار )
  {
    أرجع ft_property_do( library,
                           module_name,
                           property_name,
                           (فراغ*)مقدار,
                           TRUE,
                           TRUE );
  }

#پایان_اگر


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
  FT_New_Library( FT_حافظه    حافظه,
                  FT_Library  *alibrary )
  {
    FT_Library  library = عدم;
    FT_Error    error;


    إذا ( !حافظه || !alibrary )
      أرجع FT_THROW( Invalid_آرگومان );

#اگر_تعریف_نشده FT_DEBUG_LOGGING
#اگر_تعریف_شده FT_DEBUG_LEVEL_ERROR
    /* init debugging support */
    ft_debug_init();
#پایان_اگر /* FT_DEBUG_LEVEL_ERROR */
#پایان_اگر /* !FT_DEBUG_LOGGING */

    /* first of all, allocate the library object */
    إذا ( FT_NEW( library ) )
      أرجع error;

    library->حافظه = حافظه;

    library->نسخه_major = FREETYPE_MAJOR;
    library->نسخه_minor = FREETYPE_MINOR;
    library->نسخه_patch = FREETYPE_PATCH;

    library->refcount = 1;

    /* That's ok now */
    *alibrary = library;

    أرجع FT_Err_Ok;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( فراغ )
  FT_Library_نسخه( FT_Library   library,
                      FT_Int      *amajor,
                      FT_Int      *aminor,
                      FT_Int      *apatch )
  {
    FT_Int  major = 0;
    FT_Int  minor = 0;
    FT_Int  patch = 0;


    إذا ( library )
    {
      major = library->نسخه_major;
      minor = library->نسخه_minor;
      patch = library->نسخه_patch;
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
  FT_انجامشده_Library( FT_Library  library )
  {
    FT_حافظه  حافظه;


    إذا ( !library )
      أرجع FT_THROW( Invalid_Library_Handle );

    library->refcount--;
    إذا ( library->refcount > 0 )
      اذهب Exit;

    حافظه = library->حافظه;

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
        لكل ( n = 0; n < library->رقم_modules; n++ )
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
          طالما ( faces->سر )
          {
            FT_انجامشده_Face( FT_FACE( faces->سر->داده ) );
            إذا ( faces->سر )
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
    طالما ( library->رقم_modules > 0 )
      FT_Remove_Module( library,
                        library->modules[library->رقم_modules - 1] );
#وإلا
    {
      FT_UInt  n;


      لكل ( n = 0; n < library->رقم_modules; n++ )
      {
        FT_Module  module = library->modules[n];


        إذا ( module )
        {
          Destroy_Module( module );
          library->modules[n] = عدم;
        }
      }
    }
#پایان_اگر

    FT_FREE( library );

  Exit:
    أرجع FT_Err_Ok;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( فراغ )
  FT_مجموعه_Debug_Hook( FT_Library         library,
                     FT_UInt            hook_شاخص,
                     FT_DebugHook_Func  debug_hook )
  {
    إذا ( library && debug_hook &&
         hook_شاخص <
           ( حجم ( library->debug_hooks ) / حجم ( فراغ* ) ) )
      library->debug_hooks[hook_شاخص] = debug_hook;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_TrueTypeEngineType )
  FT_Get_TrueType_Engine_Type( FT_Library  library )
  {
    FT_TrueTypeEngineType  نتیجه = FT_TRUETYPE_ENGINE_TYPE_NONE;


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
          نتیجه = service->engine_type;
      }
    }

    أرجع نتیجه;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_SubGlyph_Info( FT_Glyph_جایگاه  glyph,
                        FT_UInt       sub_شاخص,
                        FT_Int       *p_شاخص,
                        FT_UInt      *p_flags,
                        FT_Int       *p_arg1,
                        FT_Int       *p_arg2,
                        FT_ماتریس    *p_transform )
  {
    FT_Error  error = FT_ERR( Invalid_آرگومان );


    إذا ( glyph                                      &&
         glyph->subglyphs                           &&
         glyph->format == FT_GLYPH_FORMAT_مرکب &&
         sub_شاخص < glyph->رقم_subglyphs           )
    {
      FT_SubGlyph  subg = glyph->subglyphs + sub_شاخص;


      *p_شاخص     = subg->index;
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
                            FT_UInt            پایه_glyph,
                            FT_UInt           *aglyph_شاخص,
                            FT_UInt           *acolor_شاخص,
                            FT_LayerIterator*  iterator )
  {
    TT_Face       ttface;
    SFNT_Service  sfnt;


    إذا ( !face                                   ||
         !aglyph_شاخص                           ||
         !acolor_شاخص                           ||
         !iterator                               ||
         پایه_glyph >= (FT_UInt)face->رقم_glyphs )
      أرجع 0;

    إذا ( !FT_IS_SFNT( face ) )
      أرجع 0;

    ttface = (TT_Face)face;
    sfnt   = (SFNT_Service)ttface->sfnt;

    إذا ( sfnt->get_colr_layer )
      أرجع sfnt->get_colr_layer( ttface,
                                   پایه_glyph,
                                   aglyph_شاخص,
                                   acolor_شاخص,
                                   iterator );
    وإلا
      أرجع 0;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Get_Color_Glyph_Paint( FT_Face                  face,
                            FT_UInt                  پایه_glyph,
                            FT_Color_ریشه_Transform  ریشه_transform,
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
                                         پایه_glyph,
                                         ریشه_transform,
                                         paint );
    وإلا
      أرجع 0;
  }


  /* documentation is in ftcolor.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Get_Color_Glyph_ClipBox( FT_Face      face,
                              FT_UInt      پایه_glyph,
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
                                            پایه_glyph,
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


/*انتها*/
