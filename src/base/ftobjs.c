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

#که_تعریف_شوی TT_CONFIG_OPTION_VARC
#ضمن <freetype/internal/services/svvarc.h>
#د_که_پای

#ضمن <freetype/ftdriver.h>

#که_تعریف_شوی FT_CONFIG_OPTION_MAC_FONTS
#ضمن "ftbase.h"
#د_که_پای


#که_تعریف_شوی FT_DEBUG_LEVEL_TRACE

#ضمن <freetype/ftbitmap.h>

#إذا defined( _MSC_VER )      /* Visual C++ (and Intel C++)   */
  /* We disable the warning `conversion from XXX to YYY,     */
  /* possible loss of data' in order to compile cleanly with */
  /* the maximum level of warnings: `md5.c' is non-FreeType  */
  /* code, and it gets used during development builds only.  */
#لارښوونه warning( ټېل )
#لارښوونه warning( disable : 4244 )
#د_که_پای /* _MSC_VER */

  /* It's easiest to include `md5.c' directly.  However, since OpenSSL */
  /* also provides the same functions, there might be conflicts if     */
  /* both FreeType and OpenSSL are built as static libraries.  For     */
  /* this reason, we put the MD5 stuff into the `FT_' namespace.       */
#تعریف_کړه MD5_u32plus  FT_MD5_u32plus
#تعریف_کړه MD5_CTX      FT_MD5_CTX
#تعریف_کړه MD5_Init     FT_MD5_Init
#تعریف_کړه MD5_Update   FT_MD5_Update
#تعریف_کړه MD5_Final    FT_MD5_Final

#تعریف_لغوه  HAVE_OPENSSL

#ضمن "md5.c"

#إذا defined( _MSC_VER )
#لارښوونه warning( راوباسه )
#د_که_پای

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

#د_که_پای /* FT_DEBUG_LEVEL_TRACE */


#تعریف_کړه GRID_FIT_METRICS


  /* forward declaration */
  ساكن FT_Error
  ft_open_face_دننی( FT_Library           library,
                         ثابت FT_Open_دلیلونه*  دلیلونه,
                         FT_Long              face_شاخص,
                         FT_Face             *aface,
                         FT_Bool              test_mac_fonts );


  FT_بنسټ_DEF( FT_اشاره_ګر )
  ft_service_list_lookup( FT_ServiceDesc  service_descriptors,
                          ثابت حرف*     service_id )
  {
    FT_اشاره_ګر      پایله = عدم;
    FT_ServiceDesc  desc   = service_descriptors;


    إذا ( desc && service_id )
    {
      لكل ( ; desc->serv_id != عدم; desc++ )
      {
        إذا ( ft_strcmp( desc->serv_id, service_id ) == 0 )
        {
          پایله = (FT_اشاره_ګر)desc->serv_معلومات;
          اكسر;
        }
      }
    }

    أرجع پایله;
  }


  FT_بنسټ_DEF( فراغ )
  ft_validator_init( FT_Validator        valid,
                     ثابت FT_Byte*      بنسټ,
                     ثابت FT_Byte*      حد,
                     FT_ValidationLevel  level )
  {
    valid->بنسټ  = بنسټ;
    valid->حد = حد;
    valid->level = level;
    valid->error = FT_Err_Ok;
  }


  FT_بنسټ_DEF( FT_Int )
  ft_validator_پرلپسې( FT_Validator  valid )
  {
    /* This function doesn't work!  None should call it. */
    FT_ناکارول_شوی( valid );

    أرجع -1;
  }


  FT_بنسټ_DEF( فراغ )
  ft_validator_error( FT_Validator  valid,
                      FT_Error      error )
  {
    /* since the cast below also disables the compiler's */
    /* type check, we introduce a dummy variable, which  */
    /* will be optimized away                            */
    متطاير ft_jmp_buf* jump_بفر = &valid->jump_بفر;


    valid->error = error;

    /* throw away volatileness; use `jump_buffer' or the  */
    /* compiler may warn about an unused local variable   */
    ft_longjmp( *(ft_jmp_buf*) jump_بفر, 1 );
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
  FT_بنسټ_DEF( FT_Error )
  FT_بهیر_New( FT_Library           library,
                 ثابت FT_Open_دلیلونه*  دلیلونه,
                 FT_بهیر           *astream )
  {
    FT_Error   error;
    FT_حافظه  حافظه;
    FT_بهیر  بهیر = عدم;
    FT_UInt    ډېرپېښ;


    *astream = عدم;

    إذا ( !library )
      أرجع FT_THROW( Invalid_Library_Handle );

    إذا ( !دلیلونه )
      أرجع FT_THROW( Invalid_دلیل );

    حافظه = library->حافظه;
    ډېرپېښ   = دلیلونه->flags &
               ( FT_OPEN_حافظه | FT_OPEN_بهیر | FT_OPEN_PATHNAME );

    إذا ( ډېرپېښ == FT_OPEN_حافظه )
    {
      /* create a memory-based stream */
      إذا ( FT_NEW( بهیر ) )
        اذهب Exit;

      FT_بهیر_Open_حافظه( بهیر,
                            (ثابت FT_Byte*)دلیلونه->حافظه_بنسټ,
                            (FT_ULong)دلیلونه->حافظه_اندازه );
      بهیر->حافظه = حافظه;
    }

#که_نه_تعریف_شوی FT_CONFIG_OPTION_DISABLE_بهیر_SUPPORT

    وإلا إذا ( ډېرپېښ == FT_OPEN_PATHNAME )
    {
      /* create a normal system stream */
      إذا ( FT_NEW( بهیر ) )
        اذهب Exit;

      بهیر->حافظه = حافظه;
      error = FT_بهیر_Open( بهیر, دلیلونه->pathname );
      إذا ( error )
        FT_FREE( بهیر );
    }
    وإلا إذا ( ( ډېرپېښ == FT_OPEN_بهیر ) && دلیلونه->بهیر )
    {
      /* use an existing, user-provided stream */

      /* in this case, we do not need to allocate a new stream object */
      /* since the caller is responsible for closing it himself       */
      بهیر         = دلیلونه->بهیر;
      بهیر->حافظه = حافظه;
      error          = FT_Err_Ok;
    }

#د_که_پای

    وإلا
    {
      error = FT_THROW( Invalid_دلیل );
      إذا ( ( دلیلونه->flags & FT_OPEN_بهیر ) && دلیلونه->بهیر )
        FT_بهیر_Close( دلیلونه->بهیر );
    }

    إذا ( !error )
      *astream       = بهیر;

  Exit:
    أرجع error;
  }


  FT_بنسټ_DEF( فراغ )
  FT_بهیر_Free( FT_بهیر  بهیر,
                  FT_Int     external )
  {
    إذا ( بهیر )
    {
      FT_حافظه  حافظه = بهیر->حافظه;


      FT_بهیر_Close( بهیر );

      إذا ( !external )
        FT_FREE( بهیر );
    }
  }


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#تعریف_لغوه  FT_COMPONENT
#تعریف_کړه FT_COMPONENT  objs


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
  ft_glyphslot_init( FT_Glyph_خونه  خونه )
  {
    FT_Driver         driver   = خونه->face->driver;
    FT_Driver_Class   clazz    = driver->clazz;
    FT_حافظه         حافظه   = driver->ریښه.حافظه;
    FT_Error          error    = FT_Err_Ok;
    FT_خونه_دننی  دننی = عدم;


    خونه->library = driver->ریښه.library;

    إذا ( FT_NEW( دننی ) )
      اذهب Exit;

    خونه->دننی = دننی;

    إذا ( FT_DRIVER_کاروي_OUTLINES( driver ) )
      error = FT_GlyphLoader_New( حافظه, &دننی->loader );

    إذا ( !error && clazz->init_خونه )
      error = clazz->init_خونه( خونه );

#که_تعریف_شوی FT_CONFIG_OPTION_SVG
    /* if SVG table exists, allocate the space in `slot->other` */
    إذا ( خونه->face->face_flags & FT_FACE_FLAG_SVG )
    {
      FT_SVG_Document  document = عدم;


      إذا ( FT_NEW( document ) )
        اذهب Exit;
      خونه->other = document;
    }
#د_که_پای

  Exit:
    أرجع error;
  }


  FT_بنسټ_DEF( فراغ )
  ft_glyphslot_free_bitmap( FT_Glyph_خونه  خونه )
  {
    إذا ( خونه->دننی && ( خونه->دننی->flags & FT_GLYPH_OWN_BITMAP ) )
    {
      FT_حافظه  حافظه = FT_FACE_حافظه( خونه->face );


      FT_FREE( خونه->bitmap.بفر );
      خونه->دننی->flags &= ~FT_GLYPH_OWN_BITMAP;
    }
    وإلا
    {
      /* assume that the bitmap buffer was stolen or not */
      /* allocated from the heap                         */
      خونه->bitmap.بفر = عدم;
    }
  }


  /* overflow-resistant presetting of bitmap position and dimensions; */
  /* also check whether the size is too large for rendering           */
  FT_بنسټ_DEF( FT_Bool )
  ft_glyphslot_preset_bitmap( FT_Glyph_خونه      خونه,
                              FT_Render_ډېرپېښ    ډېرپېښ,
                              ثابت FT_ویکټور*  origin )
  {
    FT_Outline*  outline = &خونه->outline;
    FT_Bitmap*   bitmap  = &خونه->bitmap;

    FT_Pixel_ډېرپېښ  pixel_ډېرپېښ;

    FT_BBox  cbox, pbox;
    FT_Pos   x_ښويدنه = 0;
    FT_Pos   y_ښويدنه = 0;
    FT_Pos   x_کیڼ, y_سر;
    FT_Pos   پلنوالی, لوړوالی, pitch;
    FT_Bool  ret;


    إذا ( خونه->format == FT_GLYPH_FORMAT_SVG )
    {
      FT_Module    module;
      SVG_Service  svg_service;


      module      = FT_Get_Module( خونه->library, "ot-svg" );
      svg_service = (SVG_Service)module->clazz->module_interface;

      أرجع (FT_Bool)svg_service->preset_خونه( module, خونه, FALSE );
    }
    وإلا إذا ( خونه->format != FT_GLYPH_FORMAT_OUTLINE )
      أرجع 1;

    إذا ( origin )
    {
      x_ښويدنه = origin->x;
      y_ښويدنه = origin->y;
    }

    /* compute the control box, and grid-fit it, */
    /* taking into account the origin shift      */
    FT_Outline_Get_CBox( outline, &cbox );

    /* rough estimate of pixel box */
    pbox.xMin = ( cbox.xMin >> 6 ) + ( x_ښويدنه >> 6 );
    pbox.yMin = ( cbox.yMin >> 6 ) + ( y_ښويدنه >> 6 );
    pbox.xMax = ( cbox.xMax >> 6 ) + ( x_ښويدنه >> 6 );
    pbox.yMax = ( cbox.yMax >> 6 ) + ( y_ښويدنه >> 6 );

    /* tiny remainder box */
    cbox.xMin = ( cbox.xMin & 63 ) + ( x_ښويدنه & 63 );
    cbox.yMin = ( cbox.yMin & 63 ) + ( y_ښويدنه & 63 );
    cbox.xMax = ( cbox.xMax & 63 ) + ( x_ښويدنه & 63 );
    cbox.yMax = ( cbox.yMax & 63 ) + ( y_ښويدنه & 63 );

    بدل ( ډېرپېښ )
    {
    حالة FT_RENDER_ډېرپېښ_MONO:
      pixel_ډېرپېښ = FT_PIXEL_ډېرپېښ_MONO;
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
#د_که_پای

    حالة FT_RENDER_ډېرپېښ_LCD:
      pixel_ډېرپېښ = FT_PIXEL_ډېرپېښ_LCD;
      ft_lcd_padding( &cbox, خونه, ډېرپېښ );
      اذهب Adjust;

    حالة FT_RENDER_ډېرپېښ_LCD_V:
      pixel_ډېرپېښ = FT_PIXEL_ډېرپېښ_LCD_V;
      ft_lcd_padding( &cbox, خونه, ډېرپېښ );
      اذهب Adjust;

    حالة FT_RENDER_ډېرپېښ_نورمال:
    حالة FT_RENDER_ډېرپېښ_LIGHT:
    افتراضي:
      pixel_ډېرپېښ = FT_PIXEL_ډېرپېښ_خړ;
    Adjust:
      pbox.xMin += cbox.xMin >> 6;
      pbox.yMin += cbox.yMin >> 6;
      pbox.xMax += ( cbox.xMax + 63 ) >> 6;
      pbox.yMax += ( cbox.yMax + 63 ) >> 6;
    }

    x_کیڼ = pbox.xMin;
    y_سر  = pbox.yMax;

    پلنوالی  = pbox.xMax - pbox.xMin;
    لوړوالی = pbox.yMax - pbox.yMin;

    /* Flag the width or height unsuitable for rendering.   */
    /* The limit is based on the ppem value when available. */
    /* FT_Renderer modules should check the return value.   */
    ret = FT_BOOL(     پلنوالی >= 0x10000   ||    لوړوالی >= 0x10000   ||
                   pbox.xMin < -0x1000000 || pbox.xMax >= 0x1000000 ||
                   pbox.yMin < -0x1000000 || pbox.yMax >= 0x1000000 ||
             ( خونه->face                                         &&
               ( پلنوالی  > 16 * خونه->face->اندازه->metrics.x_ppem ||
                 لوړوالی > 16 * خونه->face->اندازه->metrics.y_ppem ) ) );

    إذا ( ret )
      FT_TRACE3(( "ft_glyphslot_preset_bitmap: [%ld %ld %ld %ld]\n",
                  pbox.xMin, pbox.yMin, pbox.xMax, pbox.yMax ));

    بدل ( pixel_ډېرپېښ )
    {
    حالة FT_PIXEL_ډېرپېښ_MONO:
      pitch = ( ( پلنوالی + 15 ) >> 4 ) << 1;
      اكسر;

    حالة FT_PIXEL_ډېرپېښ_LCD:
      پلنوالی *= 3;
      pitch  = FT_PAD_CEIL( پلنوالی, 4 );
      اكسر;

    حالة FT_PIXEL_ډېرپېښ_LCD_V:
      لوړوالی *= 3;
      FALL_THROUGH;

    حالة FT_PIXEL_ډېرپېښ_خړ:
    افتراضي:
      pitch = پلنوالی;
    }

    خونه->bitmap_کیڼ = (FT_Int)x_کیڼ;
    خونه->bitmap_سر  = (FT_Int)y_سر;

    bitmap->pixel_ډېرپېښ = (بینښه حرف)pixel_ډېرپېښ;
    bitmap->عدد_grays  = 256;
    bitmap->پلنوالی      = (بینښه صحيح)پلنوالی;
    bitmap->لیکې       = (بینښه صحيح)لوړوالی;
    bitmap->pitch      = pitch;

    أرجع ret;
  }


  FT_بنسټ_DEF( فراغ )
  ft_glyphslot_سېټ_bitmap( FT_Glyph_خونه  خونه,
                           FT_Byte*      بفر )
  {
    ft_glyphslot_free_bitmap( خونه );

    خونه->bitmap.بفر = بفر;

    FT_ASSERT( (خونه->دننی->flags & FT_GLYPH_OWN_BITMAP) == 0 );
  }


  FT_بنسټ_DEF( FT_Error )
  ft_glyphslot_alloc_bitmap( FT_Glyph_خونه  خونه )
  {
    FT_حافظه  حافظه = FT_FACE_حافظه( خونه->face );
    FT_Error   error;


    إذا ( خونه->دننی->flags & FT_GLYPH_OWN_BITMAP )
      FT_FREE( خونه->bitmap.بفر );
    وإلا
      خونه->دننی->flags |= FT_GLYPH_OWN_BITMAP;

    /* dimensions must be preset */
    FT_MEM_ALLOC_MULT( خونه->bitmap.بفر,
                       خونه->bitmap.لیکې,
                       خونه->bitmap.pitch );
    أرجع error;
  }


  ساكن فراغ
  ft_glyphslot_clear( FT_Glyph_خونه  خونه )
  {
    /* free bitmap if needed */
    ft_glyphslot_free_bitmap( خونه );

    /* clear all public fields in the glyph slot */
    خونه->glyph_شاخص = 0;

    FT_ZERO( &خونه->metrics );
    FT_ZERO( &خونه->outline );

    خونه->bitmap.پلنوالی      = 0;
    خونه->bitmap.لیکې       = 0;
    خونه->bitmap.pitch      = 0;
    خونه->bitmap.pixel_ډېرپېښ = 0;
    /* `slot->bitmap.buffer' has been handled by ft_glyphslot_free_bitmap */

    خونه->bitmap_کیڼ   = 0;
    خونه->bitmap_سر    = 0;
    خونه->عدد_subglyphs = 0;
    خونه->subglyphs     = عدم;
    خونه->control_معلومات  = عدم;
    خونه->control_len   = 0;

#که_نه_تعریف_شوی FT_CONFIG_OPTION_SVG
    خونه->other = عدم;
#وإلا
    إذا ( !( خونه->face->face_flags & FT_FACE_FLAG_SVG ) )
      خونه->other = عدم;
    وإلا
    {
      إذا ( خونه->دننی->flags & FT_GLYPH_OWN_GZIP_SVG )
      {
        FT_حافظه        حافظه = خونه->face->حافظه;
        FT_SVG_Document  doc    = (FT_SVG_Document)خونه->other;


        FT_FREE( doc->svg_document );
        خونه->دننی->flags &= ~FT_GLYPH_OWN_GZIP_SVG;
      }
    }
#د_که_پای

    خونه->format = FT_GLYPH_FORMAT_NONE;

    خونه->خطي_Hori_Advance = 0;
    خونه->خطي_Vert_Advance = 0;
    خونه->advance.x         = 0;
    خونه->advance.y         = 0;
    خونه->lsb_delta         = 0;
    خونه->rsb_delta         = 0;
  }


  ساكن فراغ
  ft_glyphslot_بشپړ( FT_Glyph_خونه  خونه )
  {
    FT_Driver        driver = خونه->face->driver;
    FT_Driver_Class  clazz  = driver->clazz;
    FT_حافظه        حافظه = driver->ریښه.حافظه;

#که_تعریف_شوی FT_CONFIG_OPTION_SVG
    إذا ( خونه->face->face_flags & FT_FACE_FLAG_SVG )
    {
      /* Free memory in case SVG was there.                          */
      /* `slot->internal` might be NULL in out-of-memory situations. */
      إذا ( خونه->دننی && خونه->دننی->flags & FT_GLYPH_OWN_GZIP_SVG )
      {
        FT_SVG_Document  doc = (FT_SVG_Document)خونه->other;


        FT_FREE( doc->svg_document );

        خونه->دننی->flags &= ~FT_GLYPH_OWN_GZIP_SVG;
      }

      FT_FREE( خونه->other );
    }
#د_که_پای

    إذا ( clazz->بشپړ_خونه )
      clazz->بشپړ_خونه( خونه );

    /* free bitmap buffer if needed */
    ft_glyphslot_free_bitmap( خونه );

    /* slot->internal might be NULL in out-of-memory situations */
    إذا ( خونه->دننی )
    {
      /* free glyph loader */
      إذا ( FT_DRIVER_کاروي_OUTLINES( driver ) )
      {
        FT_Glyph_Loader_بشپړ( خونه->دننی->loader );
        خونه->دننی->loader = عدم;
      }

      FT_FREE( خونه->دننی );
    }
  }


  /* documentation is in ftobjs.h */

  FT_بنسټ_DEF( FT_Error )
  FT_New_Glyph_خونه( FT_Face        face,
                    FT_Glyph_خونه  *aslot )
  {
    FT_Error         error;
    FT_Driver        driver;
    FT_Driver_Class  clazz;
    FT_حافظه        حافظه;
    FT_Glyph_خونه     خونه = عدم;


    إذا ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );

    إذا ( !face->driver )
      أرجع FT_THROW( Invalid_دلیل );

    driver = face->driver;
    clazz  = driver->clazz;
    حافظه = driver->ریښه.حافظه;

    FT_TRACE4(( "FT_New_GlyphSlot: Creating new slot object\n" ));
    إذا ( !FT_ALLOC( خونه, clazz->خونه_object_اندازه ) )
    {
      خونه->face = face;

      error = ft_glyphslot_init( خونه );
      إذا ( error )
      {
        ft_glyphslot_بشپړ( خونه );
        FT_FREE( خونه );
        اذهب Exit;
      }

      خونه->بل  = face->glyph;
      face->glyph = خونه;

      إذا ( aslot )
        *aslot = خونه;
    }
    وإلا إذا ( aslot )
      *aslot = عدم;


  Exit:
    FT_TRACE4(( "FT_New_GlyphSlot: Return 0x%x\n", error ));

    أرجع error;
  }


  /* documentation is in ftobjs.h */

  FT_بنسټ_DEF( فراغ )
  FT_بشپړ_Glyph_خونه( FT_Glyph_خونه  خونه )
  {
    إذا ( خونه )
    {
      FT_Driver     driver = خونه->face->driver;
      FT_حافظه     حافظه = driver->ریښه.حافظه;
      FT_Glyph_خونه  prev;
      FT_Glyph_خونه  cur;


      /* Remove slot from its parent face's list */
      prev = عدم;
      cur  = خونه->face->glyph;

      طالما ( cur )
      {
        إذا ( cur == خونه )
        {
          إذا ( !prev )
            خونه->face->glyph = cur->بل;
          وإلا
            prev->بل = cur->بل;

          /* finalize client-specific data */
          إذا ( خونه->generic.finalizer )
            خونه->generic.finalizer( خونه );

          ft_glyphslot_بشپړ( خونه );
          FT_FREE( خونه );
          اكسر;
        }
        prev = cur;
        cur  = cur->بل;
      }
    }
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( فراغ )
  FT_سېټ_Transform( FT_Face     face,
                    FT_ماتریکس*  ماتریکس,
                    FT_ویکټور*  delta )
  {
    FT_Face_دننی  دننی;


    إذا ( !face )
      أرجع;

    دننی = face->دننی;

    دننی->transform_flags = 0;

    إذا ( !ماتریکس || !FT_ماتریکس_Check( ماتریکس ) )
    {
      دننی->transform_ماتریکس.xx = 0x10000L;
      دننی->transform_ماتریکس.xy = 0;
      دننی->transform_ماتریکس.yx = 0;
      دننی->transform_ماتریکس.yy = 0x10000L;

      ماتریکس = &دننی->transform_ماتریکس;
    }
    وإلا
      دننی->transform_ماتریکس = *ماتریکس;

    /* set transform_flags bit flag 0 if `matrix' isn't the identity */
    إذا ( ( ماتریکس->xy | ماتریکس->yx ) ||
         ماتریکس->xx != 0x10000L      ||
         ماتریکس->yy != 0x10000L      )
      دننی->transform_flags |= 1;

    إذا ( !delta )
    {
      دننی->transform_delta.x = 0;
      دننی->transform_delta.y = 0;

      delta = &دننی->transform_delta;
    }
    وإلا
      دننی->transform_delta = *delta;

    /* set transform_flags bit flag 1 if `delta' isn't the null vector */
    إذا ( delta->x | delta->y )
      دننی->transform_flags |= 2;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( فراغ )
  FT_Get_Transform( FT_Face     face,
                    FT_ماتریکس*  ماتریکس,
                    FT_ویکټور*  delta )
  {
    FT_Face_دننی  دننی;


    إذا ( !face )
      أرجع;

    دننی = face->دننی;

    إذا ( ماتریکس )
      *ماتریکس = دننی->transform_ماتریکس;

    إذا ( delta )
      *delta = دننی->transform_delta;
  }


  ساكن FT_Renderer
  ft_lookup_glyph_renderer( FT_Glyph_خونه  خونه );


#که_تعریف_شوی GRID_FIT_METRICS
  ساكن فراغ
  ft_glyphslot_grid_fit_metrics( FT_Glyph_خونه  خونه,
                                 FT_Bool       vertical )
  {
    FT_Glyph_Metrics*  metrics = &خونه->metrics;
    FT_Pos             ښی, bottom;


    إذا ( vertical )
    {
      metrics->horiBearingX = FT_PIX_FLOOR( metrics->horiBearingX );
      metrics->horiBearingY = FT_PIX_CEIL_LONG( metrics->horiBearingY );

      ښی  = FT_PIX_CEIL_LONG( جمع_کړه_LONG( metrics->vertBearingX,
                                           metrics->پلنوالی ) );
      bottom = FT_PIX_CEIL_LONG( جمع_کړه_LONG( metrics->vertBearingY,
                                           metrics->لوړوالی ) );

      metrics->vertBearingX = FT_PIX_FLOOR( metrics->vertBearingX );
      metrics->vertBearingY = FT_PIX_FLOOR( metrics->vertBearingY );

      metrics->پلنوالی  = SUB_LONG( ښی,
                                  metrics->vertBearingX );
      metrics->لوړوالی = SUB_LONG( bottom,
                                  metrics->vertBearingY );
    }
    وإلا
    {
      metrics->vertBearingX = FT_PIX_FLOOR( metrics->vertBearingX );
      metrics->vertBearingY = FT_PIX_FLOOR( metrics->vertBearingY );

      ښی  = FT_PIX_CEIL_LONG( جمع_کړه_LONG( metrics->horiBearingX,
                                           metrics->پلنوالی ) );
      bottom = FT_PIX_FLOOR( SUB_LONG( metrics->horiBearingY,
                                       metrics->لوړوالی ) );

      metrics->horiBearingX = FT_PIX_FLOOR( metrics->horiBearingX );
      metrics->horiBearingY = FT_PIX_CEIL_LONG( metrics->horiBearingY );

      metrics->پلنوالی  = SUB_LONG( ښی,
                                  metrics->horiBearingX );
      metrics->لوړوالی = SUB_LONG( metrics->horiBearingY,
                                  bottom );
    }

    metrics->horiAdvance = FT_PIX_پړاو_LONG( metrics->horiAdvance );
    metrics->vertAdvance = FT_PIX_پړاو_LONG( metrics->vertAdvance );
  }
#د_که_پای /* GRID_FIT_METRICS */


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_بارول_Glyph( FT_Face   face,
                 FT_UInt   glyph_شاخص,
                 FT_Int32  بارول_flags )
  {
    FT_Error      error;
    FT_Driver     driver;
    FT_Glyph_خونه  خونه;
    FT_Library    library;
    FT_Bool       autohint = FALSE;
    FT_Module     hinter;


    إذا ( !face || !face->اندازه || !face->glyph )
      أرجع FT_THROW( Invalid_Face_Handle );

    /* The validity test for `glyph_index' is performed by the */
    /* font drivers.                                           */

    خونه = face->glyph;
    ft_glyphslot_clear( خونه );

    driver  = face->driver;
    library = driver->ریښه.library;
    hinter  = library->auto_hinter;

    /* undefined scale means no scale */
    إذا ( face->اندازه->metrics.x_ppem == 0 ||
         face->اندازه->metrics.y_ppem == 0 )
      بارول_flags |= FT_بارول_NO_SCALE;

    /* resolve load flags dependencies */

    إذا ( بارول_flags & FT_بارول_NO_RECURSE )
      بارول_flags |= FT_بارول_NO_SCALE         |
                    FT_بارول_IGNORE_TRANSFORM;

    إذا ( بارول_flags & FT_بارول_NO_SCALE )
    {
      بارول_flags |= FT_بارول_NO_HINTING |
                    FT_بارول_NO_BITMAP;

      بارول_flags &= ~FT_بارول_RENDER;
    }

    إذا ( بارول_flags & FT_بارول_BITMAP_METRICS_ONLY )
      بارول_flags &= ~FT_بارول_RENDER;

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
         !( بارول_flags & FT_بارول_NO_HINTING )             &&
         !( بارول_flags & FT_بارول_NO_AUTOHINT )            &&
         FT_IS_SCALABLE( face )                           &&
         !FT_IS_TRICKY( face )                            &&
         ( ( بارول_flags & FT_بارول_IGNORE_TRANSFORM )    ||
           ( face->دننی->transform_ماتریکس.yx == 0 &&
             face->دننی->transform_ماتریکس.xx != 0 ) ||
           ( face->دننی->transform_ماتریکس.xx == 0 &&
             face->دننی->transform_ماتریکس.yx != 0 ) ) )
    {
      إذا ( ( بارول_flags & FT_بارول_FORCE_AUTOHINT ) ||
           !FT_DRIVER_HAS_HINTER( driver )         )
        autohint = TRUE;
      وإلا
      {
        FT_Render_ډېرپېښ  ډېرپېښ = FT_بارول_موخه_ډېرپېښ( بارول_flags );
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
        إذا ( ( ډېرپېښ == FT_RENDER_ډېرپېښ_LIGHT           &&
               ( !FT_DRIVER_HINTS_LIGHTLY( driver ) &&
                 !is_light_type1                    ) )         ||
             ( FT_IS_SFNT( face )                             &&
               ttface->عدد_locations                          &&
               ttface->font_پروګرام_اندازه == 0                 &&
               ttface->cvt_پروګرام_اندازه <= 7                  ) )
          autohint = TRUE;
      }
    }

#که_تعریف_شوی TT_CONFIG_OPTION_VARC
    /* Never auto-hint a VARC (variable composite) glyph: its components */
    /* are already loaded unhinted, and the auto-fitter cannot process   */
    /* the assembled composite (it also re-enters variation-coordinate   */
    /* setting).  Use the native hinter / unhinted load instead.         */
    إذا ( autohint && FT_IS_SFNT( face ) )
    {
      FT_Service_VARC  varc;


      FT_FACE_پیدا_کړه_SERVICE( face, varc, VARC );
      إذا ( varc && varc->has_glyph( face, glyph_شاخص ) )
        autohint = FALSE;
    }
#د_که_پای

    إذا ( autohint )
    {
      FT_AutoHinter_Interface  hinting;


      /* XXX: The use of the `FT_LOAD_XXX_ONLY` flags is not very */
      /*      elegant.                                            */

      /* try to load SVG documents if available */
      إذا ( ( بارول_flags & FT_بارول_NO_SVG ) == 0 &&
           FT_HAS_SVG( face )                   )
      {
        error = driver->clazz->بارول_glyph( خونه, face->اندازه,
                                           glyph_شاخص,
                                           بارول_flags | FT_بارول_SVG_ONLY );

        إذا ( !error && خونه->format == FT_GLYPH_FORMAT_SVG )
          اذهب بارول_Ok;
      }

      /* try to load embedded bitmaps if available */
      إذا ( FT_HAS_FIXED_SIZES( face )              &&
           ( بارول_flags & FT_بارول_NO_BITMAP ) == 0 )
      {
        error = driver->clazz->بارول_glyph( خونه, face->اندازه,
                                           glyph_شاخص,
                                           بارول_flags | FT_بارول_SBITS_ONLY );

        إذا ( !error && خونه->format == FT_GLYPH_FORMAT_BITMAP )
          اذهب بارول_Ok;
      }

      {
        FT_Face_دننی  دننی        = face->دننی;
        FT_Int            transform_flags = دننی->transform_flags;


        /* since the auto-hinter calls FT_Load_Glyph by itself, */
        /* make sure that glyphs aren't transformed             */
        دننی->transform_flags = 0;

        /* load auto-hinted outline */
        hinting = (FT_AutoHinter_Interface)hinter->clazz->module_interface;

        error   = hinting->بارول_glyph( (FT_AutoHinter)hinter,
                                       خونه, face->اندازه,
                                       glyph_شاخص, بارول_flags );

        دننی->transform_flags = transform_flags;
      }
    }
    وإلا
    {
      error = driver->clazz->بارول_glyph( خونه,
                                         face->اندازه,
                                         glyph_شاخص,
                                         بارول_flags );
      إذا ( error )
        اذهب Exit;

      إذا ( خونه->format == FT_GLYPH_FORMAT_OUTLINE )
      {
        /* check that the loaded outline is correct */
        error = FT_Outline_Check( &خونه->outline );
        إذا ( error )
          اذهب Exit;

#که_تعریف_شوی GRID_FIT_METRICS
        إذا ( !( بارول_flags & FT_بارول_NO_HINTING ) )
          ft_glyphslot_grid_fit_metrics(
            خونه,
            FT_BOOL( بارول_flags & FT_بارول_VERTICAL_LAYOUT ) );
#د_که_پای
      }
    }

  بارول_Ok:
    /* compute the advance */
    إذا ( بارول_flags & FT_بارول_VERTICAL_LAYOUT )
    {
      خونه->advance.x = 0;
      خونه->advance.y = خونه->metrics.vertAdvance;
    }
    وإلا
    {
      خونه->advance.x = خونه->metrics.horiAdvance;
      خونه->advance.y = 0;
    }

    /* compute the linear advance in 16.16 pixels */
    إذا ( ( بارول_flags & FT_بارول_خطي_DESIGN ) == 0 &&
         FT_IS_SCALABLE( face )                      )
    {
      FT_اندازه_Metrics*  metrics = &face->اندازه->metrics;


      /* it's tricky! */
      خونه->خطي_Hori_Advance = FT_MulDiv( خونه->خطي_Hori_Advance,
                                           metrics->x_scale, 64 );

      خونه->خطي_Vert_Advance = FT_MulDiv( خونه->خطي_Vert_Advance,
                                           metrics->y_scale, 64 );
    }

    إذا ( ( بارول_flags & FT_بارول_IGNORE_TRANSFORM ) == 0 )
    {
      FT_Face_دننی  دننی = face->دننی;


      /* now, transform the glyph image if needed */
      إذا ( دننی->transform_flags )
      {
        /* get renderer */
        FT_Renderer  renderer = ft_lookup_glyph_renderer( خونه );


        إذا ( renderer )
          error = renderer->clazz->transform_glyph(
                                     renderer, خونه,
                                     &دننی->transform_ماتریکس,
                                     &دننی->transform_delta );
        وإلا إذا ( خونه->format == FT_GLYPH_FORMAT_OUTLINE )
        {
          /* apply `standard' transformation if no renderer is available */
          إذا ( دننی->transform_flags & 1 )
            FT_Outline_Transform( &خونه->outline,
                                  &دننی->transform_ماتریکس );

          إذا ( دننی->transform_flags & 2 )
            FT_Outline_Translate( &خونه->outline,
                                  دننی->transform_delta.x,
                                  دننی->transform_delta.y );
        }

        /* transform advance */
        FT_ویکټور_Transform( &خونه->advance, &دننی->transform_ماتریکس );
      }
    }

    خونه->glyph_شاخص          = glyph_شاخص;
    خونه->دننی->بارول_flags = بارول_flags;

    /* do we need to render the image or preset the bitmap now? */
    إذا ( !error                                    &&
         ( بارول_flags & FT_بارول_NO_SCALE ) == 0    &&
         خونه->format != FT_GLYPH_FORMAT_BITMAP    &&
         خونه->format != FT_GLYPH_FORMAT_مرکب )
    {
      FT_Render_ډېرپېښ  ډېرپېښ = FT_بارول_موخه_ډېرپېښ( بارول_flags );


      إذا ( ډېرپېښ == FT_RENDER_ډېرپېښ_نورمال   &&
           بارول_flags & FT_بارول_MONOCHROME )
        ډېرپېښ = FT_RENDER_ډېرپېښ_MONO;

      إذا ( بارول_flags & FT_بارول_RENDER )
        error = FT_Render_Glyph( خونه, ډېرپېښ );
      وإلا
        ft_glyphslot_preset_bitmap( خونه, ډېرپېښ, عدم );
    }

#که_تعریف_شوی FT_DEBUG_LEVEL_TRACE
    FT_TRACE5(( "FT_Load_Glyph: index %u, flags 0x%x\n",
                glyph_شاخص, بارول_flags ));
    FT_TRACE5(( "  bitmap %ux%u %s, %s (mode %d)\n",
                خونه->bitmap.پلنوالی,
                خونه->bitmap.لیکې,
                خونه->outline.points ?
                  خونه->bitmap.بفر ? "rendered"
                                      : "preset"
                                     :
                  خونه->دننی->flags & FT_GLYPH_OWN_BITMAP ? "owned"
                                                              : "unowned",
                pixel_modes[خونه->bitmap.pixel_ډېرپېښ],
                خونه->bitmap.pixel_ډېرپېښ ));
    FT_TRACE5(( "\n" ));
    FT_TRACE5(( "  x advance: %f\n", (مزدوج)خونه->advance.x / 64 ));
    FT_TRACE5(( "  y advance: %f\n", (مزدوج)خونه->advance.y / 64 ));
    FT_TRACE5(( "  linear x advance: %f\n",
                (مزدوج)خونه->خطي_Hori_Advance / 65536 ));
    FT_TRACE5(( "  linear y advance: %f\n",
                (مزدوج)خونه->خطي_Vert_Advance / 65536 ));

    {
      FT_Glyph_Metrics*  metrics = &خونه->metrics;


      FT_TRACE5(( "  metrics:\n" ));
      FT_TRACE5(( "    width:  %f\n", (مزدوج)metrics->پلنوالی / 64 ));
      FT_TRACE5(( "    height: %f\n", (مزدوج)metrics->لوړوالی / 64 ));
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
#د_که_پای

  Exit:
    أرجع error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_بارول_Char( FT_Face   face,
                FT_ULong  char_کوډ,
                FT_Int32  بارول_flags )
  {
    FT_UInt  glyph_شاخص;


    إذا ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );

    glyph_شاخص = (FT_UInt)char_کوډ;
    إذا ( face->charmap )
      glyph_شاخص = FT_Get_Char_شاخص( face, char_کوډ );

    أرجع FT_بارول_Glyph( face, glyph_شاخص, بارول_flags );
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
    إذا ( driver->clazz->بشپړ_اندازه )
      driver->clazz->بشپړ_اندازه( اندازه );

    /* finalize client-specific data */
    إذا ( اندازه->generic.finalizer )
      اندازه->generic.finalizer( اندازه );

    FT_FREE( اندازه->دننی );
    FT_FREE( اندازه );
  }


  ساكن فراغ
  ft_cmap_بشپړ_دننی( FT_CMap  cmap );


  ساكن فراغ
  destroy_charmaps( FT_Face    face,
                    FT_حافظه  حافظه )
  {
    FT_Int  n;


    إذا ( !face )
      أرجع;

    لكل ( n = 0; n < face->عدد_charmaps; n++ )
    {
      FT_CMap  cmap = FT_CMAP( face->charmaps[n] );


      ft_cmap_بشپړ_دننی( cmap );

      face->charmaps[n] = عدم;
    }

    FT_FREE( face->charmaps );
    face->عدد_charmaps = 0;
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
      face->autohint.finalizer( face->autohint.معلومات );

    /* Discard glyph slots for this face.                           */
    /* Beware!  FT_Done_GlyphSlot() changes the field `face->glyph' */
    طالما ( face->glyph )
      FT_بشپړ_Glyph_خونه( face->glyph );

    /* discard all sizes for this face */
    FT_List_Finalize( &face->sizes_list,
                      destroy_اندازه,
                      حافظه,
                      driver );
    face->اندازه = عدم;

    /* discard charmaps */
    destroy_charmaps( face, حافظه );

    /* finalize format-specific stuff */
    إذا ( clazz->بشپړ_face )
      clazz->بشپړ_face( face );

    /* close the stream for this face if needed */
    FT_بهیر_Free(
      face->بهیر,
      ( face->face_flags & FT_FACE_FLAG_EXTERNAL_بهیر ) != 0 );

    face->بهیر = عدم;

    /* now discard client data */
    إذا ( face->generic.finalizer )
      face->generic.finalizer( face );

    /* get rid of it */
    إذا ( face->دننی )
    {
      FT_FREE( face->دننی );
    }
    FT_FREE( face );
  }


  ساكن فراغ
  Destroy_Driver( FT_Driver  driver )
  {
    FT_List_Finalize( &driver->faces_list,
                      destroy_face,
                      driver->ریښه.حافظه,
                      driver );
  }


  /* documentation is in ftobjs.h */

  FT_بنسټ_DEF( FT_Error )
  پیدا_کړه_unicode_charmap( FT_Face  face )
  {
    FT_CharMap*  لومړی;
    FT_CharMap*  cur;


    /* caller should have already checked that `face' is valid */
    FT_ASSERT( face );

    لومړی = face->charmaps;

    إذا ( !لومړی )
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

    cur = لومړی + face->عدد_charmaps;  /* points after the last one */

    لكل ( ; --cur >= لومړی; )
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
               cur[0]->encoding_id == TT_APPLE_ID_ډک_UNICODE  &&
               FT_Get_CMap_Format( cur[0] ) == 13               ) )
        {
          face->charmap = cur[0];
          أرجع FT_Err_Ok;
        }
      }
    }

    /* We do not have any UCS-4 charmap.                */
    /* Do the loop again and search for UCS-2 charmaps. */
    cur = لومړی + face->عدد_charmaps;

    لكل ( ; --cur >= لومړی; )
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
  پیدا_کړه_variant_selector_charmap( FT_Face  face )
  {
    FT_CharMap*  لومړی;
    FT_CharMap*  پای_ټکی;
    FT_CharMap*  cur;


    /* caller should have already checked that `face' is valid */
    FT_ASSERT( face );

    لومړی = face->charmaps;

    إذا ( !لومړی )
      أرجع عدم;

    پای_ټکی = لومړی + face->عدد_charmaps;  /* points after the last one */

    لكل ( cur = لومړی; cur < پای_ټکی; cur++ )
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
             FT_بهیر      *astream,
             FT_Bool        *anexternal_بهیر,
             FT_Long        face_شاخص,
             FT_Int         عدد_params,
             FT_Parameter*  params,
             FT_Face       *aface )
  {
    FT_حافظه         حافظه;
    FT_Driver_Class   clazz;
    FT_Face           face     = عدم;
    FT_Face_دننی  دننی = عدم;

    FT_Error          error, error2;


    clazz  = driver->clazz;
    حافظه = driver->ریښه.حافظه;

    /* allocate the face object and perform basic initialization */
    إذا ( FT_ALLOC( face, clazz->face_object_اندازه ) )
      اذهب Fail;

    face->driver = driver;
    face->حافظه = حافظه;
    face->بهیر = *astream;

    /* set the FT_FACE_FLAG_EXTERNAL_STREAM bit for FT_Done_Face */
    إذا ( *anexternal_بهیر )
      face->face_flags |= FT_FACE_FLAG_EXTERNAL_بهیر;

    إذا ( FT_NEW( دننی ) )
      اذهب Fail;

    face->دننی = دننی;

#که_تعریف_شوی FT_CONFIG_OPTION_INCREMENTAL
    {
      صحيح  i;


      face->دننی->incremental_interface = عدم;
      لكل ( i = 0; i < عدد_params && !face->دننی->incremental_interface;
            i++ )
        إذا ( params[i].نښان == FT_PARAM_نښان_INCREMENTAL )
          face->دننی->incremental_interface =
            (FT_Incremental_Interface)params[i].معلومات;
    }
#د_که_پای

    face->دننی->random_تخم = -1;

    إذا ( clazz->init_face )
      error = clazz->init_face( *astream,
                                face,
                                (FT_Int)face_شاخص,
                                عدد_params,
                                params );
    /* Stream may have been changed. */
    *astream = face->بهیر;
    *anexternal_بهیر =
      ( face->face_flags & FT_FACE_FLAG_EXTERNAL_بهیر ) != 0;
    إذا ( error )
      اذهب Fail;

    /* select Unicode charmap by default */
    error2 = پیدا_کړه_unicode_charmap( face );

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
      إذا ( clazz->بشپړ_face )
        clazz->بشپړ_face( face );
      FT_FREE( دننی );
      FT_FREE( face );
      *aface = عدم;
    }

    أرجع error;
  }


  /* there's a Mac-specific extended implementation of FT_New_Face() */
  /* in src/base/ftmac.c                                             */

#که_نه_تعریف_شوی FT_MACINTOSH

  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_New_Face( FT_Library   library,
               ثابت حرف*  pathname,
               FT_Long      face_شاخص,
               FT_Face     *aface )
  {
    FT_Open_دلیلونه  دلیلونه;


    /* test for valid `library' and `aface' delayed to `FT_Open_Face' */
    إذا ( !pathname )
      أرجع FT_THROW( Invalid_دلیل );

    دلیلونه.flags    = FT_OPEN_PATHNAME;
    دلیلونه.pathname = (حرف*)pathname;
    دلیلونه.بهیر   = عدم;

    أرجع ft_open_face_دننی( library, &دلیلونه, face_شاخص, aface, 1 );
  }

#د_که_پای


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_New_حافظه_Face( FT_Library      library,
                      ثابت FT_Byte*  file_بنسټ,
                      FT_Long         file_اندازه,
                      FT_Long         face_شاخص,
                      FT_Face        *aface )
  {
    FT_Open_دلیلونه  دلیلونه;


    /* test for valid `library' and `face' delayed to `FT_Open_Face' */
    إذا ( !file_بنسټ )
      أرجع FT_THROW( Invalid_دلیل );

    دلیلونه.flags       = FT_OPEN_حافظه;
    دلیلونه.حافظه_بنسټ = file_بنسټ;
    دلیلونه.حافظه_اندازه = file_اندازه;
    دلیلونه.بهیر      = عدم;

    أرجع ft_open_face_دننی( library, &دلیلونه, face_شاخص, aface, 1 );
  }


#که_تعریف_شوی FT_CONFIG_OPTION_MAC_FONTS

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
  حافظه_بهیر_close( FT_بهیر  بهیر )
  {
    FT_حافظه  حافظه = (FT_حافظه)بهیر->descriptor.اشاره_ګر;


    FT_FREE( بهیر->بنسټ );
    بهیر->اندازه  = 0;
    بهیر->close = عدم;
    FT_FREE( بهیر );
  }


  /* Create a new memory stream from a buffer and a size. */
  /* From `ftmac.c'.                                      */
  ساكن FT_Error
  new_حافظه_بهیر( FT_Library           library,
                     FT_Byte*             بنسټ,
                     FT_ULong             اندازه,
                     FT_بهیر_Close_Func  close,
                     FT_بهیر           *astream )
  {
    FT_Error   error;
    FT_حافظه  حافظه;
    FT_بهیر  بهیر = عدم;


    إذا ( !library )
      أرجع FT_THROW( Invalid_Library_Handle );

    إذا ( !بنسټ )
      أرجع FT_THROW( Invalid_دلیل );

    *astream = عدم;
    حافظه   = library->حافظه;
    إذا ( FT_NEW( بهیر ) )
      اذهب Exit;

    FT_بهیر_Open_حافظه( بهیر, بنسټ, اندازه );

    بهیر->descriptor.اشاره_ګر = حافظه;
    بهیر->close              = close;

    *astream = بهیر;

  Exit:
    أرجع error;
  }


  /* Create a new FT_Face given a buffer and a driver name. */
  /* From `ftmac.c'.                                        */
  FT_LOCAL_DEF( FT_Error )
  open_face_from_بفر( FT_Library   library,
                         FT_Byte*     بنسټ,
                         FT_ULong     اندازه,
                         FT_Long      face_شاخص,
                         ثابت حرف*  driver_name,
                         FT_Face     *aface )
  {
    FT_Open_دلیلونه  دلیلونه;
    FT_Error      error;
    FT_حافظه     حافظه = library->حافظه;


    دلیلونه.driver = عدم;
    دلیلونه.flags  = 0;

    إذا ( driver_name )
    {
      دلیلونه.driver = FT_Get_Module( library, driver_name );
      إذا ( !دلیلونه.driver )
      {
        FT_FREE( بنسټ );
        أرجع FT_THROW( Missing_Module );
      }

      دلیلونه.flags = دلیلونه.flags | FT_OPEN_DRIVER;
    }

    /* `memory_stream_close` also frees the stream object. */
    error = new_حافظه_بهیر( library,
                               بنسټ,
                               اندازه,
                               حافظه_بهیر_close,
                               &دلیلونه.بهیر );
    إذا ( error )
    {
      FT_FREE( بنسټ );
      أرجع error;
    }

    دلیلونه.flags |= FT_OPEN_بهیر;

#که_تعریف_شوی FT_MACINTOSH
    /* At this point, the face index has served its purpose;  */
    /* whoever calls this function has already used it to     */
    /* locate the correct font data.  We should not propagate */
    /* this index to FT_Open_Face() (unless it is negative).  */

    إذا ( face_شاخص > 0 )
      face_شاخص &= 0x7FFF0000L; /* retain GX data */
#د_که_پای

    أرجع ft_open_face_دننی( library, &دلیلونه, face_شاخص, aface, 0 );
  }


  /* Look up `TYP1' or `CID ' table from sfnt table directory.       */
  /* `offset' and `length' must exclude the binary header in tables. */

  /* Type 1 and CID-keyed font drivers should recognize sfnt-wrapped */
  /* format too.  Here, since we can't expect that the TrueType font */
  /* driver is loaded unconditionally, we must parse the font by     */
  /* ourselves.  We are only interested in the name of the table and */
  /* the offset.                                                     */

  ساكن FT_Error
  ft_lookup_PS_دننه_sfnt_بهیر( FT_بهیر  بهیر,
                               FT_Long    face_شاخص,
                               FT_ULong*  دننه_واټن,
                               FT_ULong*  اوږدتیا,
                               FT_Bool*   is_sfnt_cid )
  {
    FT_Error   error;
    FT_UShort  عدد_Tables;
    FT_Long    pstable_شاخص;
    FT_ULong   نښان;
    صحيح        i;


    *دننه_واټن = 0;
    *اوږدتیا = 0;
    *is_sfnt_cid = FALSE;

    /* TODO: support for sfnt-wrapped PS/CID in TTC format */

    /* version check for 'typ1' (should be ignored?) */
    إذا ( FT_READ_ULONG( نښان ) )
      أرجع error;
    إذا ( نښان != TTAG_typ1 )
      أرجع FT_THROW( Unknown_File_Format );

    إذا ( FT_READ_USHORT( عدد_Tables ) )
      أرجع error;
    إذا ( FT_بهیر_SKIP( 2 * 3 ) ) /* skip binary search header */
      أرجع error;

    pstable_شاخص = -1;
    *is_sfnt_cid  = FALSE;

    لكل ( i = 0; i < عدد_Tables; i++ )
    {
      إذا ( FT_READ_ULONG( نښان )     || FT_بهیر_SKIP( 4 )      ||
           FT_READ_ULONG( *دننه_واټن ) || FT_READ_ULONG( *اوږدتیا ) )
        أرجع error;

      إذا ( نښان == TTAG_CID )
      {
        pstable_شاخص++;
        *دننه_واټن += 22;
        *اوږدتیا -= 22;
        *is_sfnt_cid = TRUE;
        إذا ( face_شاخص < 0 )
          أرجع FT_Err_Ok;
      }
      وإلا إذا ( نښان == TTAG_TYP1 )
      {
        pstable_شاخص++;
        *دننه_واټن += 24;
        *اوږدتیا -= 24;
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
  open_face_PS_from_sfnt_بهیر( FT_Library     library,
                                 FT_بهیر      بهیر,
                                 FT_Long        face_شاخص,
                                 FT_Int         عدد_params,
                                 FT_Parameter  *params,
                                 FT_Face       *aface )
  {
    FT_Error   error;
    FT_حافظه  حافظه = library->حافظه;
    FT_ULong   دننه_واټن, اوږدتیا;
    FT_ULong   pos;
    FT_Bool    is_sfnt_cid;
    FT_Byte*   sfnt_ps = عدم;

    FT_ناکارول_شوی( عدد_params );
    FT_ناکارول_شوی( params );


    /* ignore GX stuff */
    إذا ( face_شاخص > 0 )
      face_شاخص &= 0xFFFFL;

    pos = FT_بهیر_POS();

    error = ft_lookup_PS_دننه_sfnt_بهیر( بهیر,
                                         face_شاخص,
                                         &دننه_واټن,
                                         &اوږدتیا,
                                         &is_sfnt_cid );
    إذا ( error )
      اذهب Exit;

    إذا ( دننه_واټن > بهیر->اندازه )
    {
      FT_TRACE2(( "open_face_PS_from_sfnt_stream: invalid table offset\n" ));
      error = FT_THROW( Invalid_جدول );
      اذهب Exit;
    }
    وإلا إذا ( اوږدتیا > بهیر->اندازه - دننه_واټن )
    {
      FT_TRACE2(( "open_face_PS_from_sfnt_stream: invalid table length\n" ));
      error = FT_THROW( Invalid_جدول );
      اذهب Exit;
    }

    error = FT_بهیر_Seek( بهیر, pos + دننه_واټن );
    إذا ( error )
      اذهب Exit;

    إذا ( FT_QALLOC( sfnt_ps, (FT_Long)اوږدتیا ) )
      اذهب Exit;

    error = FT_بهیر_Read( بهیر, (FT_Byte *)sfnt_ps, اوږدتیا );
    إذا ( error )
    {
      FT_FREE( sfnt_ps );
      اذهب Exit;
    }

    error = open_face_from_بفر( library,
                                   sfnt_ps,
                                   اوږدتیا,
                                   FT_MIN( face_شاخص, 0 ),
                                   is_sfnt_cid ? "t1cid" : "type1",
                                   aface );
  Exit:
    {
      FT_Error  error1;


      إذا ( FT_ERR_EQ( error, Unknown_File_Format ) )
      {
        error1 = FT_بهیر_Seek( بهیر, pos );
        إذا ( error1 )
          أرجع error1;
      }

      أرجع error;
    }
  }


#که_نه_تعریف_شوی FT_MACINTOSH

  /* The resource header says we've got resource_cnt `POST' (type1) */
  /* resources in this file.  They all need to be coalesced into    */
  /* one lump which gets passed on to the type1 driver.             */
  /* Here can be only one PostScript font in a file so face_index   */
  /* must be 0 (or -1).                                             */
  /*                                                                */
  ساكن FT_Error
  Mac_Read_POST_Resource( FT_Library  library,
                          FT_بهیر   بهیر,
                          FT_Long    *offsets,
                          FT_Long     resource_cnt,
                          FT_Long     face_شاخص,
                          FT_Face    *aface )
  {
    FT_Error   error  = FT_ERR( Cannot_Open_Resource );
    FT_حافظه  حافظه = library->حافظه;

    FT_Byte*   pfb_معلومات = عدم;
    صحيح        i, type, flags;
    FT_ULong   len;
    FT_ULong   pfb_len, pfb_pos, pfb_lenpos;
    FT_ULong   rlen, لنډمهاله;


    إذا ( face_شاخص == -1 )
      face_شاخص = 0;
    إذا ( face_شاخص != 0 )
      أرجع error;

    /* Find the length of all the POST resources, concatenated.  Assume */
    /* worst case (each resource in its own section).                   */
    pfb_len = 0;
    لكل ( i = 0; i < resource_cnt; i++ )
    {
      error = FT_بهیر_Seek( بهیر, (FT_ULong)offsets[i] );
      إذا ( error )
        اذهب Exit;
      إذا ( FT_READ_ULONG( لنډمهاله ) )  /* actually LONG */
        اذهب Exit;

      /* FT2 allocator takes signed long buffer length,
       * too large value causing overflow should be checked
       */
      FT_TRACE4(( "                 POST fragment #%d: length=0x%08lx"
                  " total pfb_len=0x%08lx\n",
                  i, لنډمهاله, pfb_len + لنډمهاله + 6 ));

      إذا ( FT_MAC_RFORK_MAX_LEN < لنډمهاله               ||
           FT_MAC_RFORK_MAX_LEN - لنډمهاله < pfb_len + 6 )
      {
        FT_TRACE2(( "             MacOS resource length cannot exceed"
                    " 0x%08lx\n",
                    FT_MAC_RFORK_MAX_LEN ));

        error = FT_THROW( Invalid_دننه_واټن );
        اذهب Exit;
      }

      pfb_len += لنډمهاله + 6;
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

    إذا ( FT_QALLOC( pfb_معلومات, (FT_Long)pfb_len + 2 ) )
      اذهب Exit;

    pfb_معلومات[0] = 0x80;
    pfb_معلومات[1] = 1;            /* Ascii section */
    pfb_معلومات[2] = 0;            /* 4-byte length, fill in later */
    pfb_معلومات[3] = 0;
    pfb_معلومات[4] = 0;
    pfb_معلومات[5] = 0;
    pfb_pos     = 6;
    pfb_lenpos  = 2;

    len  = 0;
    type = 1;

    لكل ( i = 0; i < resource_cnt; i++ )
    {
      error = FT_بهیر_Seek( بهیر, (FT_ULong)offsets[i] );
      إذا ( error )
        اذهب Exit2;
      إذا ( FT_READ_ULONG( rlen ) )
        اذهب Exit2;

      /* FT2 allocator takes signed long buffer length,
       * too large fragment length causing overflow should be checked
       */
      إذا ( 0x7FFFFFFFUL < rlen )
      {
        error = FT_THROW( Invalid_دننه_واټن );
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
                    i, (فراغ*)pfb_معلومات, pfb_lenpos ));

        إذا ( pfb_lenpos + 3 > pfb_len + 2 )
          اذهب Exit2;

        pfb_معلومات[pfb_lenpos    ] = (FT_Byte)( len );
        pfb_معلومات[pfb_lenpos + 1] = (FT_Byte)( len >> 8 );
        pfb_معلومات[pfb_lenpos + 2] = (FT_Byte)( len >> 16 );
        pfb_معلومات[pfb_lenpos + 3] = (FT_Byte)( len >> 24 );

        إذا ( ( flags >> 8 ) == 5 )      /* End of font mark */
          اكسر;

        FT_TRACE3(( "    Write POST fragment #%d header (6-byte) to buffer"
                    " %p + 0x%08lx\n",
                    i, (فراغ*)pfb_معلومات, pfb_pos ));

        إذا ( pfb_pos + 6 > pfb_len + 2 )
          اذهب Exit2;

        pfb_معلومات[pfb_pos++] = 0x80;

        type = flags >> 8;
        len  = rlen;

        pfb_معلومات[pfb_pos++] = (FT_Byte)type;
        pfb_lenpos          = pfb_pos;
        pfb_معلومات[pfb_pos++] = 0;        /* 4-byte length, fill in later */
        pfb_معلومات[pfb_pos++] = 0;
        pfb_معلومات[pfb_pos++] = 0;
        pfb_معلومات[pfb_pos++] = 0;
      }

      إذا ( pfb_pos > pfb_len || pfb_pos + rlen > pfb_len )
        اذهب Exit2;

      FT_TRACE3(( "    Load POST fragment #%d (%lu byte) to buffer"
                  " %p + 0x%08lx\n",
                  i, rlen, (فراغ*)pfb_معلومات, pfb_pos ));

      error = FT_بهیر_Read( بهیر, (FT_Byte *)pfb_معلومات + pfb_pos, rlen );
      إذا ( error )
        اذهب Exit2;

      pfb_pos += rlen;
    }

    error = FT_ERR( Array_Too_Large );

    إذا ( pfb_pos + 2 > pfb_len + 2 )
      اذهب Exit2;
    pfb_معلومات[pfb_pos++] = 0x80;
    pfb_معلومات[pfb_pos++] = 3;

    إذا ( pfb_lenpos + 3 > pfb_len + 2 )
      اذهب Exit2;
    pfb_معلومات[pfb_lenpos    ] = (FT_Byte)( len );
    pfb_معلومات[pfb_lenpos + 1] = (FT_Byte)( len >> 8 );
    pfb_معلومات[pfb_lenpos + 2] = (FT_Byte)( len >> 16 );
    pfb_معلومات[pfb_lenpos + 3] = (FT_Byte)( len >> 24 );

    أرجع open_face_from_بفر( library,
                                  pfb_معلومات,
                                  pfb_pos,
                                  face_شاخص,
                                  "type1",
                                  aface );

  Exit2:
    إذا ( FT_ERR_EQ( error, Array_Too_Large ) )
      FT_TRACE2(( "  Abort due to too-short buffer to store"
                  " all POST fragments\n" ));
    وإلا إذا ( FT_ERR_EQ( error, Invalid_دننه_واټن ) )
      FT_TRACE2(( "  Abort due to invalid offset in a POST fragment\n" ));

    إذا ( error )
      error = FT_ERR( Cannot_Open_Resource );
    FT_FREE( pfb_معلومات );

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
                          FT_بهیر   بهیر,
                          FT_Long    *offsets,
                          FT_Long     resource_cnt,
                          FT_Long     face_شاخص,
                          FT_Face    *aface )
  {
    FT_حافظه  حافظه = library->حافظه;
    FT_Byte*   sfnt_معلومات = عدم;
    FT_Error   error;
    FT_ULong   flag_دننه_واټن;
    FT_ULong   rlen;
    صحيح        is_cff;
    FT_Long    face_شاخص_دننه_resource = 0;


    إذا ( face_شاخص < 0 )
      face_شاخص = -face_شاخص - 1;
    إذا ( face_شاخص >= resource_cnt )
      أرجع FT_THROW( Cannot_Open_Resource );

    flag_دننه_واټن = (FT_ULong)offsets[face_شاخص];
    error = FT_بهیر_Seek( بهیر, flag_دننه_واټن );
    إذا ( error )
      اذهب Exit;

    إذا ( FT_READ_ULONG( rlen ) )
      اذهب Exit;
    إذا ( !rlen )
      أرجع FT_THROW( Cannot_Open_Resource );
    إذا ( rlen > FT_MAC_RFORK_MAX_LEN )
      أرجع FT_THROW( Invalid_دننه_واټن );

    error = open_face_PS_from_sfnt_بهیر( library,
                                           بهیر,
                                           face_شاخص,
                                           0, عدم,
                                           aface );
    إذا ( !error )
      اذهب Exit;

    /* rewind sfnt stream before open_face_PS_from_sfnt_stream() */
    error = FT_بهیر_Seek( بهیر, flag_دننه_واټن + 4 );
    إذا ( error )
      اذهب Exit;

    إذا ( FT_QALLOC( sfnt_معلومات, rlen ) )
      أرجع error;
    error = FT_بهیر_Read( بهیر, (FT_Byte *)sfnt_معلومات, rlen );
    إذا ( error )
    {
      FT_FREE( sfnt_معلومات );
      اذهب Exit;
    }

    is_cff = rlen > 4 && !ft_memcmp( sfnt_معلومات, "OTTO", 4 );
    error = open_face_from_بفر( library,
                                   sfnt_معلومات,
                                   rlen,
                                   face_شاخص_دننه_resource,
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
                 FT_بهیر   بهیر,
                 FT_Long     resource_دننه_واټن,
                 FT_Long     face_شاخص,
                 FT_Face    *aface )
  {
    FT_حافظه  حافظه = library->حافظه;
    FT_Error   error;
    FT_Long    map_دننه_واټن, rdata_pos;
    FT_Long    *معلومات_offsets;
    FT_Long    شمېر;


    error = FT_Raccess_Get_سرلیک_Info( library, بهیر, resource_دننه_واټن,
                                       &map_دننه_واټن, &rdata_pos );
    إذا ( error )
      أرجع error;

    /* POST resources must be sorted to concatenate properly */
    error = FT_Raccess_Get_معلومات_Offsets( library, بهیر,
                                        map_دننه_واټن, rdata_pos,
                                        TTAG_POST, TRUE,
                                        &معلومات_offsets, &شمېر );
    إذا ( !error )
    {
      error = Mac_Read_POST_Resource( library, بهیر, معلومات_offsets, شمېر,
                                      face_شاخص, aface );
      FT_FREE( معلومات_offsets );
      /* POST exists in an LWFN providing a single face */
      إذا ( !error )
        (*aface)->عدد_faces = 1;
      أرجع error;
    }

    /* sfnt resources should not be sorted to preserve the face order by
       QuickDraw API */
    error = FT_Raccess_Get_معلومات_Offsets( library, بهیر,
                                        map_دننه_واټن, rdata_pos,
                                        TTAG_sfnt, FALSE,
                                        &معلومات_offsets, &شمېر );
    إذا ( !error )
    {
      FT_Long  face_شاخص_دننی = face_شاخص % شمېر;


      error = Mac_Read_sfnt_Resource( library, بهیر, معلومات_offsets, شمېر,
                                      face_شاخص_دننی, aface );
      FT_FREE( معلومات_offsets );
      إذا ( !error )
      {
        (*aface)->عدد_faces  = شمېر;
        (*aface)->face_شاخص = face_شاخص_دننی;
      }
    }

    أرجع error;
  }


  /* Check for a valid macbinary header, and if we find one   */
  /* check that the (flattened) resource fork in it is valid. */
  /*                                                          */
  ساكن FT_Error
  Is_Mac_دوګونی( FT_Library  library,
               FT_بهیر   بهیر,
               FT_Long     face_شاخص,
               FT_Face    *aface )
  {
    بینښه حرف  سرلیک[128];
    FT_Error       error;
    FT_Long        dlen, دننه_واټن;


    إذا ( !بهیر )
      أرجع FT_THROW( Invalid_بهیر_Operation );

    error = FT_بهیر_Seek( بهیر, 0 );
    إذا ( error )
      اذهب Exit;

    error = FT_بهیر_Read( بهیر, (FT_Byte*)سرلیک, 128 );
    إذا ( error )
      اذهب Exit;

    إذا (            سرلیک[ 0] !=   0 ||
                    سرلیک[74] !=   0 ||
                    سرلیک[82] !=   0 ||
                    سرلیک[ 1] ==   0 ||
                    سرلیک[ 1] >   33 ||
                    سرلیک[63] !=   0 ||
         سرلیک[2 + سرلیک[1]] !=   0 ||
                  سرلیک[0x53] > 0x7F )
      أرجع FT_THROW( Unknown_File_Format );

    dlen = ( سرلیک[0x53] << 24 ) |
           ( سرلیک[0x54] << 16 ) |
           ( سرلیک[0x55] <<  8 ) |
             سرلیک[0x56];
#إذا 0
    rlen = ( سرلیک[0x57] << 24 ) |
           ( سرلیک[0x58] << 16 ) |
           ( سرلیک[0x59] <<  8 ) |
             سرلیک[0x5A];
#د_که_پای /* 0 */
    دننه_واټن = 128 + ( ( dlen + 127 ) & ~127 );

    أرجع IsMacResource( library, بهیر, دننه_واټن, face_شاخص, aface );

  Exit:
    أرجع error;
  }


  ساكن FT_Error
  بارول_face_دننه_embedded_rfork( FT_Library           library,
                               FT_بهیر            بهیر,
                               FT_Long              face_شاخص,
                               FT_Face             *aface,
                               ثابت FT_Open_دلیلونه  *دلیلونه )
  {

#تعریف_لغوه  FT_COMPONENT
#تعریف_کړه FT_COMPONENT  raccess

    FT_حافظه  حافظه = library->حافظه;
    FT_Error   error  = FT_ERR( Unknown_File_Format );
    FT_UInt    i;

    حرف*      file_نومونه[FT_RACCESS_N_قواعد];
    FT_Long    offsets[FT_RACCESS_N_قواعد];
    FT_Error   errors[FT_RACCESS_N_قواعد];
    FT_Bool    is_darwin_vfs, vfs_rfork_has_no_font = FALSE; /* not tested */

    FT_Open_دلیلونه  args2;
    FT_بهیر     stream2 = عدم;


    FT_Raccess_Guess( library, بهیر,
                      دلیلونه->pathname, file_نومونه, offsets, errors );

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
      args2.pathname = file_نومونه[i] ? file_نومونه[i] : دلیلونه->pathname;

      FT_TRACE3(( "Try rule %u: %s (offset=%ld) ...",
                  i, args2.pathname, offsets[i] ));

      error = FT_بهیر_New( library, &args2, &stream2 );
      إذا ( is_darwin_vfs && FT_ERR_EQ( error, Cannot_Open_بهیر ) )
        vfs_rfork_has_no_font = TRUE;

      إذا ( error )
      {
        FT_TRACE3(( "failed\n" ));
        استمر;
      }

      error = IsMacResource( library, stream2, offsets[i],
                             face_شاخص, aface );
      FT_بهیر_Free( stream2, 0 );

      FT_TRACE3(( "%s\n", error ? "failed": "successful" ));

      إذا ( !error )
          اكسر;
      وإلا إذا ( is_darwin_vfs )
          vfs_rfork_has_no_font = TRUE;
    }

    لكل (i = 0; i < FT_RACCESS_N_قواعد; i++)
    {
      إذا ( file_نومونه[i] )
        FT_FREE( file_نومونه[i] );
    }

    /* Caller (load_mac_face) requires FT_Err_Unknown_File_Format. */
    إذا ( error )
      error = FT_ERR( Unknown_File_Format );

    أرجع error;

#تعریف_لغوه  FT_COMPONENT
#تعریف_کړه FT_COMPONENT  objs

  }


  /* Check for some macintosh formats without Carbon framework.    */
  /* Is this a macbinary file?  If so look at the resource fork.   */
  /* Is this a mac dfont file?                                     */
  /* Is this an old style resource fork? (in data)                 */
  /* Else call load_face_in_embedded_rfork to try extra rules      */
  /* (defined in `ftrfork.c').                                     */
  /*                                                               */
  ساكن FT_Error
  بارول_mac_face( FT_Library           library,
                 FT_بهیر            بهیر,
                 FT_Long              face_شاخص,
                 FT_Face             *aface,
                 ثابت FT_Open_دلیلونه  *دلیلونه )
  {
    FT_Error error;
    FT_ناکارول_شوی( دلیلونه );


    error = Is_Mac_دوګونی( library, بهیر, face_شاخص, aface );
    إذا ( FT_ERR_EQ( error, Unknown_File_Format ) )
    {

#تعریف_لغوه  FT_COMPONENT
#تعریف_کړه FT_COMPONENT  raccess

#که_تعریف_شوی FT_DEBUG_LEVEL_TRACE
      FT_TRACE3(( "Try as dfont: " ));
      إذا ( !( دلیلونه->flags & FT_OPEN_حافظه ) )
        FT_TRACE3(( "%s ...", دلیلونه->pathname ));
#د_که_پای

      error = IsMacResource( library, بهیر, 0, face_شاخص, aface );

      FT_TRACE3(( "%s\n", error ? "failed" : "successful" ));

#تعریف_لغوه  FT_COMPONENT
#تعریف_کړه FT_COMPONENT  objs

    }

    إذا ( ( FT_ERR_EQ( error, Unknown_File_Format )      ||
           FT_ERR_EQ( error, Invalid_بهیر_Operation ) ) &&
         ( دلیلونه->flags & FT_OPEN_PATHNAME )               )
      error = بارول_face_دننه_embedded_rfork( library, بهیر,
                                           face_شاخص, aface, دلیلونه );
    أرجع error;
  }
#د_که_پای

#د_که_پای  /* !FT_MACINTOSH && FT_CONFIG_OPTION_MAC_FONTS */


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Open_Face( FT_Library           library,
                ثابت FT_Open_دلیلونه*  دلیلونه,
                FT_Long              face_شاخص,
                FT_Face             *aface )
  {
    أرجع ft_open_face_دننی( library, دلیلونه, face_شاخص, aface, 1 );
  }


  ساكن FT_Error
  ft_open_face_دننی( FT_Library           library,
                         ثابت FT_Open_دلیلونه*  دلیلونه,
                         FT_Long              face_شاخص,
                         FT_Face             *aface,
                         FT_Bool              test_mac_fonts )
  {
    FT_Error     error;
    FT_Driver    driver = عدم;
    FT_حافظه    حافظه = عدم;
    FT_بهیر    بهیر = عدم;
    FT_Face      face   = عدم;
    FT_List_غوټه  غوټه   = عدم;
    FT_Bool      external_بهیر;
    FT_Module*   cur;
    FT_Module*   حد;

#که_نه_تعریف_شوی FT_CONFIG_OPTION_MAC_FONTS
    FT_ناکارول_شوی( test_mac_fonts );
#د_که_پای


    /* only use lower 31 bits together with sign bit */
    إذا ( face_شاخص > 0 )
      face_شاخص &= 0x7FFFFFFFL;
    وإلا
    {
      face_شاخص  = -face_شاخص;
      face_شاخص &= 0x7FFFFFFFL;
      face_شاخص  = -face_شاخص;
    }

#که_تعریف_شوی FT_DEBUG_LEVEL_TRACE
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
#د_که_پای

    /* test for valid `library' delayed to `FT_Stream_New' */

    إذا ( !دلیلونه )
      أرجع FT_THROW( Invalid_دلیل );

    external_بهیر = FT_BOOL( ( دلیلونه->flags & FT_OPEN_بهیر ) &&
                               دلیلونه->بهیر                     );

    /* create input stream */
    error = FT_بهیر_New( library, دلیلونه, &بهیر );
    إذا ( error )
      اذهب Fail3;

    /* Do this error check after `FT_Stream_New` to ensure that the */
    /* 'close' callback is called.                                  */
    إذا ( !aface && face_شاخص >= 0 )
    {
      error = FT_THROW( Invalid_دلیل );
      اذهب Fail3;
    }

    حافظه = library->حافظه;

    /* If the font driver is specified in the `args' structure, use */
    /* it.  Otherwise, we scan the list of registered drivers.      */
    إذا ( ( دلیلونه->flags & FT_OPEN_DRIVER ) && دلیلونه->driver )
    {
      driver = FT_DRIVER( دلیلونه->driver );

      /* not all modules are drivers, so check... */
      إذا ( FT_MODULE_IS_DRIVER( driver ) )
      {
        FT_Int         عدد_params = 0;
        FT_Parameter*  params     = عدم;


        إذا ( دلیلونه->flags & FT_OPEN_PARAMS )
        {
          عدد_params = دلیلونه->عدد_params;
          params     = دلیلونه->params;
        }

        error = open_face( driver, &بهیر, &external_بهیر, face_شاخص,
                           عدد_params, params, &face );
        إذا ( !error )
          اذهب Success;
      }
      وإلا
        error = FT_THROW( Invalid_Handle );

      FT_بهیر_Free( بهیر, external_بهیر );
      اذهب Fail;
    }
    وإلا
    {
      error = FT_ERR( Missing_Module );

      /* check each font driver for an appropriate format */
      cur   = library->modules;
      حد = cur + library->عدد_modules;

      لكل ( ; cur < حد; cur++ )
      {
        /* not all modules are font drivers, so check... */
        إذا ( FT_MODULE_IS_DRIVER( cur[0] ) )
        {
          FT_Int         عدد_params = 0;
          FT_Parameter*  params     = عدم;


          driver = FT_DRIVER( cur[0] );

          إذا ( دلیلونه->flags & FT_OPEN_PARAMS )
          {
            عدد_params = دلیلونه->عدد_params;
            params     = دلیلونه->params;
          }

          error = open_face( driver, &بهیر, &external_بهیر, face_شاخص,
                             عدد_params, params, &face );
          إذا ( !error )
            اذهب Success;

#که_تعریف_شوی FT_CONFIG_OPTION_MAC_FONTS
          إذا ( test_mac_fonts                                           &&
               ft_strcmp( cur[0]->clazz->module_name, "truetype" ) == 0 &&
               FT_ERR_EQ( error, جدول_Missing )                        )
          {
            /* TrueType but essential tables are missing */
            error = FT_بهیر_Seek( بهیر, 0 );
            إذا ( error )
              اكسر;

            error = open_face_PS_from_sfnt_بهیر( library,
                                                   بهیر,
                                                   face_شاخص,
                                                   عدد_params,
                                                   params,
                                                   aface );
            إذا ( !error )
            {
              FT_بهیر_Free( بهیر, external_بهیر );
              أرجع error;
            }
          }
#د_که_پای

          إذا ( FT_ERR_NEQ( error, Unknown_File_Format ) )
            اذهب Fail3;
        }
      }

    Fail3:
      /* If we are on the mac, and we get an                          */
      /* FT_Err_Invalid_Stream_Operation it may be because we have an */
      /* empty data fork, so we need to check the resource fork.      */
      إذا ( FT_ERR_NEQ( error, Cannot_Open_بهیر )       &&
           FT_ERR_NEQ( error, Unknown_File_Format )      &&
           FT_ERR_NEQ( error, Invalid_بهیر_Operation ) )
        اذهب Fail2;

#إذا !defined( FT_MACINTOSH ) && defined( FT_CONFIG_OPTION_MAC_FONTS )
      إذا ( test_mac_fonts )
      {
        error = بارول_mac_face( library, بهیر, face_شاخص, aface, دلیلونه );
        إذا ( !error )
        {
          /* We don't want to go to Success here.  We've already done   */
          /* that.  On the other hand, if we succeeded we still need to */
          /* close this stream (we opened a different stream which      */
          /* extracted the interesting information out of this stream   */
          /* here.  That stream will still be open and the face will    */
          /* point to it).                                              */
          FT_بهیر_Free( بهیر, external_بهیر );
          أرجع error;
        }
      }

      إذا ( FT_ERR_NEQ( error, Unknown_File_Format ) )
        اذهب Fail2;
#د_که_پای  /* !FT_MACINTOSH && FT_CONFIG_OPTION_MAC_FONTS */

      /* no driver is able to handle this format */
      error = FT_THROW( Unknown_File_Format );

  Fail2:
      FT_بهیر_Free( بهیر, external_بهیر );
      اذهب Fail;
    }

  Success:
    FT_TRACE4(( "FT_Open_Face: New face object, adding to list\n" ));

    /* add the face object to its driver's list */
    إذا ( FT_QNEW( غوټه ) )
      اذهب Fail;

    غوټه->معلومات = face;
    /* don't assume driver is the same as face->driver, so use */
    /* face->driver instead.                                   */
    FT_List_جمع_کړه( &face->driver->faces_list, غوټه );

    /* now allocate a glyph slot object for the face */
    FT_TRACE4(( "FT_Open_Face: Creating glyph slot\n" ));

    إذا ( face_شاخص >= 0 )
    {
      error = FT_New_Glyph_خونه( face, عدم );
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
      إذا ( face->لوړوالی < 0 )
        face->لوړوالی = (FT_Short)-face->لوړوالی;

      إذا ( !FT_HAS_VERTICAL( face ) )
        face->max_advance_لوړوالی = (FT_Short)face->لوړوالی;
    }

    إذا ( FT_HAS_FIXED_SIZES( face ) )
    {
      FT_Int  i;


      لكل ( i = 0; i < face->عدد_fixed_sizes; i++ )
      {
        FT_Bitmap_اندازه*  bsize = face->شته_sizes + i;


        إذا ( bsize->لوړوالی < 0 )
          bsize->لوړوالی = -bsize->لوړوالی;
        إذا ( bsize->x_ppem < 0 )
          bsize->x_ppem = -bsize->x_ppem;
        إذا ( bsize->y_ppem < 0 )
          bsize->y_ppem = -bsize->y_ppem;

        /* check whether negation actually has worked */
        إذا ( bsize->لوړوالی < 0 || bsize->x_ppem < 0 || bsize->y_ppem < 0 )
        {
          FT_TRACE0(( "FT_Open_Face:"
                      " Invalid bitmap dimensions for strike %d,"
                      " now disabled\n", i ));
          bsize->پلنوالی  = 0;
          bsize->لوړوالی = 0;
          bsize->اندازه   = 0;
          bsize->x_ppem = 0;
          bsize->y_ppem = 0;
        }
      }
    }

    /* initialize internal face data */
    {
      FT_Face_دننی  دننی = face->دننی;


      دننی->transform_ماتریکس.xx = 0x10000L;
      دننی->transform_ماتریکس.xy = 0;
      دننی->transform_ماتریکس.yx = 0;
      دننی->transform_ماتریکس.yy = 0x10000L;

      دننی->transform_delta.x = 0;
      دننی->transform_delta.y = 0;

      دننی->refcount = 1;

      دننی->no_stem_darkening = -1;
    }

    إذا ( aface )
      *aface = face;
    وإلا
      FT_بشپړ_Face( face );

    اذهب Exit;

  Fail:
    إذا ( غوټه )
      FT_بشپړ_Face( face );    /* face must be in the driver's list */
    وإلا إذا ( face )
      destroy_face( حافظه, face, driver );

  Exit:
#که_تعریف_شوی FT_DEBUG_LEVEL_TRACE
    إذا ( !error && face_شاخص < 0 )
    {
      FT_TRACE3(( "FT_Open_Face: The font has %ld face%s\n",
                  face->عدد_faces,
                  face->عدد_faces == 1 ? "" : "s" ));
      FT_TRACE3(( "              and %ld named instance%s for face %ld\n",
                  face->style_flags >> 16,
                  ( face->style_flags >> 16 ) == 1 ? "" : "s",
                  -face_شاخص - 1 ));
    }
#د_که_پای

    FT_TRACE4(( "FT_Open_Face: Return 0x%x\n", error ));

    أرجع error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Attach_File( FT_Face      face,
                  ثابت حرف*  filepathname )
  {
    FT_Open_دلیلونه  open;


    /* test for valid `face' delayed to `FT_Attach_Stream' */

    إذا ( !filepathname )
      أرجع FT_THROW( Invalid_دلیل );

    open.بهیر   = عدم;
    open.flags    = FT_OPEN_PATHNAME;
    open.pathname = (حرف*)filepathname;

    أرجع FT_Attach_بهیر( face, &open );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Attach_بهیر( FT_Face              face,
                    ثابت FT_Open_دلیلونه*  parameters )
  {
    FT_بهیر  بهیر;
    FT_Error   error;
    FT_Driver  driver;

    FT_Driver_Class  clazz;


    /* test for valid `parameters' delayed to `FT_Stream_New' */

    إذا ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );

    driver = face->driver;
    إذا ( !driver )
      أرجع FT_THROW( Invalid_Driver_Handle );

    error = FT_بهیر_New( driver->ریښه.library, parameters, &بهیر );
    إذا ( error )
      اذهب Exit;

    /* we implement FT_Attach_Stream in each driver through the */
    /* `attach_file' interface                                  */

    error = FT_ERR( Unimplemented_Feature );
    clazz = driver->clazz;
    إذا ( clazz->attach_file )
      error = clazz->attach_file( face, بهیر );

    /* close the attached stream */
    FT_بهیر_Free( بهیر,
                    FT_BOOL( parameters->بهیر                     &&
                             ( parameters->flags & FT_OPEN_بهیر ) ) );

  Exit:
    أرجع error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Reference_Face( FT_Face  face )
  {
    إذا ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );

    face->دننی->refcount++;

    أرجع FT_Err_Ok;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_بشپړ_Face( FT_Face  face )
  {
    FT_Error     error;
    FT_Driver    driver;
    FT_حافظه    حافظه;
    FT_List_غوټه  غوټه;


    error = FT_ERR( Invalid_Face_Handle );
    إذا ( face && face->driver )
    {
      face->دننی->refcount--;
      إذا ( face->دننی->refcount > 0 )
        error = FT_Err_Ok;
      وإلا
      {
        driver = face->driver;
        حافظه = driver->ریښه.حافظه;

        /* find face in driver's list */
        غوټه = FT_List_پیدا_کړه( &driver->faces_list, face );
        إذا ( غوټه )
        {
          /* remove face object from the driver's list */
          FT_List_Remove( &driver->faces_list, غوټه );
          FT_FREE( غوټه );

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
    FT_List_غوټه      غوټه = عدم;

    FT_اندازه_دننی  دننی = عدم;


    إذا ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );

    إذا ( !asize )
      أرجع FT_THROW( Invalid_دلیل );

    إذا ( !face->driver )
      أرجع FT_THROW( Invalid_Driver_Handle );

    *asize = عدم;

    driver = face->driver;
    clazz  = driver->clazz;
    حافظه = face->حافظه;

    /* Allocate new size object and perform basic initialisation */
    إذا ( FT_ALLOC( اندازه, clazz->اندازه_object_اندازه ) || FT_QNEW( غوټه ) )
      اذهب Exit;

    اندازه->face = face;

    إذا ( FT_NEW( دننی ) )
      اذهب Exit;

    اندازه->دننی = دننی;

    إذا ( clazz->init_اندازه )
      error = clazz->init_اندازه( اندازه );

    /* in case of success, add to the face's list */
    إذا ( !error )
    {
      *asize     = اندازه;
      غوټه->معلومات = اندازه;
      FT_List_جمع_کړه( &face->sizes_list, غوټه );
    }

  Exit:
    إذا ( error )
    {
      FT_FREE( غوټه );
      إذا ( اندازه )
        FT_FREE( اندازه->دننی );
      FT_FREE( اندازه );
    }

    أرجع error;
  }


  /* documentation is in ftobjs.h */

  FT_EXPORT_DEF( FT_Error )
  FT_بشپړ_اندازه( FT_اندازه  اندازه )
  {
    FT_Error     error;
    FT_Driver    driver;
    FT_حافظه    حافظه;
    FT_Face      face;
    FT_List_غوټه  غوټه;


    إذا ( !اندازه )
      أرجع FT_THROW( Invalid_اندازه_Handle );

    face = اندازه->face;
    إذا ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );

    driver = face->driver;
    إذا ( !driver )
      أرجع FT_THROW( Invalid_Driver_Handle );

    حافظه = driver->ریښه.حافظه;

    error = FT_Err_Ok;
    غوټه  = FT_List_پیدا_کړه( &face->sizes_list, اندازه );
    إذا ( غوټه )
    {
      FT_List_Remove( &face->sizes_list, غوټه );
      FT_FREE( غوټه );

      إذا ( face->اندازه == اندازه )
      {
        face->اندازه = عدم;
        إذا ( face->sizes_list.سر_2 )
          face->اندازه = (FT_اندازه)(face->sizes_list.سر_2->معلومات);
      }

      destroy_اندازه( حافظه, اندازه, driver );
    }
    وإلا
      error = FT_THROW( Invalid_اندازه_Handle );

    أرجع error;
  }


  /* documentation is in ftobjs.h */

  FT_بنسټ_DEF( FT_Error )
  FT_سمون_اندازه( FT_Face          face,
                 FT_اندازه_Request  req,
                 FT_Bool          ignore_پلنوالی,
                 FT_ULong*        اندازه_شاخص )
  {
    FT_Int   i;
    FT_Long  w, h;


    إذا ( !FT_HAS_FIXED_SIZES( face ) )
      أرجع FT_THROW( Invalid_Face_Handle );

    /* FT_Bitmap_Size doesn't provide enough info... */
    إذا ( req->type != FT_اندازه_REQUEST_TYPE_NOMINAL )
      أرجع FT_THROW( Unimplemented_Feature );

    w = FT_REQUEST_پلنوالی ( req );
    h = FT_REQUEST_لوړوالی( req );

    إذا ( req->پلنوالی && !req->لوړوالی )
      h = w;
    وإلا إذا ( !req->پلنوالی && req->لوړوالی )
      w = h;

    w = FT_PIX_پړاو( w );
    h = FT_PIX_پړاو( h );

    إذا ( !w || !h )
      أرجع FT_THROW( Invalid_Pixel_اندازه );

    لكل ( i = 0; i < face->عدد_fixed_sizes; i++ )
    {
      FT_Bitmap_اندازه*  bsize = face->شته_sizes + i;


      إذا ( h != FT_PIX_پړاو( bsize->y_ppem ) )
        استمر;

      إذا ( w == FT_PIX_پړاو( bsize->x_ppem ) || ignore_پلنوالی )
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

  FT_بنسټ_DEF( فراغ )
  ft_synthesize_vertical_metrics( FT_Glyph_Metrics*  metrics,
                                  FT_Pos             advance )
  {
    FT_Pos  لوړوالی = metrics->لوړوالی;


    /* compensate for glyph with bbox above/below the baseline */
    إذا ( metrics->horiBearingY < 0 )
    {
      إذا ( لوړوالی < metrics->horiBearingY )
        لوړوالی = metrics->horiBearingY;
    }
    وإلا إذا ( metrics->horiBearingY > 0 )
      لوړوالی -= metrics->horiBearingY;

    /* the factor 1.2 is a heuristical value */
    إذا ( !advance )
      advance = لوړوالی * 12 / 10;

    metrics->vertBearingX = metrics->horiBearingX - metrics->horiAdvance / 2;
    metrics->vertBearingY = ( advance - لوړوالی ) / 2;
    metrics->vertAdvance  = advance;
  }


  ساكن فراغ
  ft_recompute_scaled_metrics( FT_Face           face,
                               FT_اندازه_Metrics*  metrics )
  {
    /* Compute root ascender, descender, test height, and max_advance */

#که_تعریف_شوی GRID_FIT_METRICS
    metrics->ascender    = FT_PIX_CEIL( FT_MulFix( face->ascender,
                                                   metrics->y_scale ) );

    metrics->descender   = FT_PIX_FLOOR( FT_MulFix( face->descender,
                                                    metrics->y_scale ) );

    metrics->لوړوالی      = FT_PIX_پړاو( FT_MulFix( face->لوړوالی,
                                                    metrics->y_scale ) );

    metrics->max_advance = FT_PIX_پړاو( FT_MulFix( face->max_advance_پلنوالی,
                                                    metrics->x_scale ) );
#وإلا /* !GRID_FIT_METRICS */
    metrics->ascender    = FT_MulFix( face->ascender,
                                      metrics->y_scale );

    metrics->descender   = FT_MulFix( face->descender,
                                      metrics->y_scale );

    metrics->لوړوالی      = FT_MulFix( face->لوړوالی,
                                      metrics->y_scale );

    metrics->max_advance = FT_MulFix( face->max_advance_پلنوالی,
                                      metrics->x_scale );
#د_که_پای /* !GRID_FIT_METRICS */
  }


  FT_بنسټ_DEF( فراغ )
  FT_Select_Metrics( FT_Face   face,
                     FT_ULong  strike_شاخص )
  {
    FT_اندازه_Metrics*  metrics;
    FT_Bitmap_اندازه*   bsize;


    metrics = &face->اندازه->metrics;
    bsize   = face->شته_sizes + strike_شاخص;

    metrics->x_ppem = (FT_UShort)( ( bsize->x_ppem + 32 ) >> 6 );
    metrics->y_ppem = (FT_UShort)( ( bsize->y_ppem + 32 ) >> 6 );

    إذا ( FT_IS_SCALABLE( face ) )
    {
      metrics->x_scale = FT_DivFix( bsize->x_ppem,
                                    face->units_فی_EM );
      metrics->y_scale = FT_DivFix( bsize->y_ppem,
                                    face->units_فی_EM );

      ft_recompute_scaled_metrics( face, metrics );
    }
    وإلا
    {
      metrics->x_scale     = 1L << 16;
      metrics->y_scale     = 1L << 16;
      metrics->ascender    = bsize->y_ppem;
      metrics->descender   = 0;
      metrics->لوړوالی      = bsize->لوړوالی << 6;
      metrics->max_advance = bsize->x_ppem;
    }
  }


  FT_بنسټ_DEF( FT_Error )
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
        w = h = face->units_فی_EM;
        اكسر;

      حالة FT_اندازه_REQUEST_TYPE_REAL_DIM:
        w = h = face->ascender - face->descender;
        اكسر;

      حالة FT_اندازه_REQUEST_TYPE_BBOX:
        w = face->bbox.xMax - face->bbox.xMin;
        h = face->bbox.yMax - face->bbox.yMin;
        اكسر;

      حالة FT_اندازه_REQUEST_TYPE_CELL:
        w = face->max_advance_پلنوالی;
        h = face->ascender - face->descender;
        اكسر;

      حالة FT_اندازه_REQUEST_TYPE_SCALES:
        metrics->x_scale = (FT_Fixed)req->پلنوالی;
        metrics->y_scale = (FT_Fixed)req->لوړوالی;
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

      scaled_w = FT_REQUEST_پلنوالی ( req );
      scaled_h = FT_REQUEST_لوړوالی( req );

      /* determine scales */
      إذا ( req->لوړوالی || !req->پلنوالی )
      {
        إذا ( h == 0 )
        {
          FT_ERROR(( "FT_Request_Metrics: Divide by zero\n" ));
          error = FT_ERR( Divide_By_Zero );
          اذهب Exit;
        }

        metrics->y_scale = FT_DivFix( scaled_h, h );
      }

      إذا ( req->پلنوالی )
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

      إذا ( !req->لوړوالی )
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
        scaled_w = FT_MulFix( face->units_فی_EM, metrics->x_scale );
        scaled_h = FT_MulFix( face->units_فی_EM, metrics->y_scale );
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

    إذا ( strike_شاخص < 0 || strike_شاخص >= face->عدد_fixed_sizes )
      أرجع FT_THROW( Invalid_دلیل );

    clazz = face->driver->clazz;

    إذا ( clazz->select_اندازه )
    {
      error = clazz->select_اندازه( face->اندازه, (FT_ULong)strike_شاخص );

      FT_TRACE5(( "FT_Select_Size (%s driver):\n",
                  face->driver->ریښه.clazz->module_name ));
    }
    وإلا
    {
      FT_Select_Metrics( face, (FT_ULong)strike_شاخص );

      FT_TRACE5(( "FT_Select_Size:\n" ));
    }

#که_تعریف_شوی FT_DEBUG_LEVEL_TRACE
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
                  (مزدوج)metrics->لوړوالی / 64 ));
      FT_TRACE5(( "  max advance: %f\n",
                  (مزدوج)metrics->max_advance / 64 ));
      FT_TRACE5(( "  x ppem: %d\n", metrics->x_ppem ));
      FT_TRACE5(( "  y ppem: %d\n", metrics->y_ppem ));
    }
#د_که_پای

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

    إذا ( !req || req->پلنوالی < 0 || req->لوړوالی < 0 ||
         req->type >= FT_اندازه_REQUEST_TYPE_MAX )
      أرجع FT_THROW( Invalid_دلیل );

    /* signal the auto-hinter to recompute its size metrics */
    /* (if requested)                                       */
    face->اندازه->دننی->autohint_metrics.x_scale = 0;

    clazz = face->driver->clazz;

    إذا ( clazz->request_اندازه )
    {
      error = clazz->request_اندازه( face->اندازه, req );

      FT_TRACE5(( "FT_Request_Size (%s driver):\n",
                  face->driver->ریښه.clazz->module_name ));
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
      error = FT_سمون_اندازه( face, req, 0, &strike_شاخص );
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

#که_تعریف_شوی FT_DEBUG_LEVEL_TRACE
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
                  (مزدوج)metrics->لوړوالی / 64 ));
      FT_TRACE5(( "  max advance: %f\n",
                  (مزدوج)metrics->max_advance / 64 ));
      FT_TRACE5(( "  x ppem: %d\n", metrics->x_ppem ));
      FT_TRACE5(( "  y ppem: %d\n", metrics->y_ppem ));
    }
#د_که_پای

  Exit:
    أرجع error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_سېټ_Char_اندازه( FT_Face     face,
                    FT_F26Dot6  char_پلنوالی,
                    FT_F26Dot6  char_لوړوالی,
                    FT_UInt     horz_resolution,
                    FT_UInt     vert_resolution )
  {
    FT_اندازه_Request_Rec  req;


    /* check of `face' delayed to `FT_Request_Size' */

    إذا ( !char_پلنوالی )
      char_پلنوالی = char_لوړوالی;
    وإلا إذا ( !char_لوړوالی )
      char_لوړوالی = char_پلنوالی;

    إذا ( !horz_resolution )
      horz_resolution = vert_resolution;
    وإلا إذا ( !vert_resolution )
      vert_resolution = horz_resolution;

    إذا ( char_پلنوالی  < 1 * 64 )
      char_پلنوالی  = 1 * 64;
    إذا ( char_لوړوالی < 1 * 64 )
      char_لوړوالی = 1 * 64;

    إذا ( !horz_resolution )
      horz_resolution = vert_resolution = 72;

    req.type           = FT_اندازه_REQUEST_TYPE_NOMINAL;
    req.پلنوالی          = char_پلنوالی;
    req.لوړوالی         = char_لوړوالی;
    req.horiResolution = horz_resolution;
    req.vertResolution = vert_resolution;

    أرجع FT_Request_اندازه( face, &req );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_سېټ_Pixel_Sizes( FT_Face  face,
                      FT_UInt  pixel_پلنوالی,
                      FT_UInt  pixel_لوړوالی )
  {
    FT_اندازه_Request_Rec  req;


    /* check of `face' delayed to `FT_Request_Size' */

    إذا ( pixel_پلنوالی == 0 )
      pixel_پلنوالی = pixel_لوړوالی;
    وإلا إذا ( pixel_لوړوالی == 0 )
      pixel_لوړوالی = pixel_پلنوالی;

    إذا ( pixel_پلنوالی  < 1 )
      pixel_پلنوالی  = 1;
    إذا ( pixel_لوړوالی < 1 )
      pixel_لوړوالی = 1;

    /* use `>=' to avoid potential compiler warning on 16bit platforms */
    إذا ( pixel_پلنوالی >= 0xFFFFU )
      pixel_پلنوالی = 0xFFFFU;
    إذا ( pixel_لوړوالی >= 0xFFFFU )
      pixel_لوړوالی = 0xFFFFU;

    req.type           = FT_اندازه_REQUEST_TYPE_NOMINAL;
    req.پلنوالی          = (FT_Long)( pixel_پلنوالی << 6 );
    req.لوړوالی         = (FT_Long)( pixel_لوړوالی << 6 );
    req.horiResolution = 0;
    req.vertResolution = 0;

    أرجع FT_Request_اندازه( face, &req );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Kerning( FT_Face     face,
                  FT_UInt     کیڼ_glyph,
                  FT_UInt     ښی_glyph,
                  FT_UInt     kern_ډېرپېښ,
                  FT_ویکټور  *akerning )
  {
    FT_Error   error = FT_Err_Ok;
    FT_Driver  driver;


    إذا ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );

    إذا ( !akerning )
      أرجع FT_THROW( Invalid_دلیل );

    driver = face->driver;

    akerning->x = 0;
    akerning->y = 0;

    إذا ( driver->clazz->get_kerning )
    {
      error = driver->clazz->get_kerning( face,
                                          کیڼ_glyph,
                                          ښی_glyph,
                                          akerning );
      إذا ( !error )
      {
        إذا ( kern_ډېرپېښ != FT_KERNING_UNSCALED )
        {
          akerning->x = FT_MulFix( akerning->x, face->اندازه->metrics.x_scale );
          akerning->y = FT_MulFix( akerning->y, face->اندازه->metrics.y_scale );

          إذا ( kern_ډېرپېښ != FT_KERNING_UNFITTED )
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

            akerning->x = FT_PIX_پړاو( akerning->x );
            akerning->y = FT_PIX_پړاو( akerning->y );

#که_تعریف_شوی FT_DEBUG_LEVEL_TRACE
            {
              FT_Pos  orig_x_rounded = FT_PIX_پړاو( orig_x );
              FT_Pos  orig_y_rounded = FT_PIX_پړاو( orig_y );


              إذا ( akerning->x != orig_x_rounded ||
                   akerning->y != orig_y_rounded )
                FT_TRACE5(( "FT_Get_Kerning: horizontal kerning"
                            " (%ld, %ld) scaled down to (%ld, %ld) pixels\n",
                            orig_x_rounded / 64, orig_y_rounded / 64,
                            akerning->x / 64, akerning->y / 64 ));
            }
#د_که_پای
          }
        }
      }
    }

    أرجع error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Track_Kerning( FT_Face    face,
                        FT_Fixed   ټکی_اندازه,
                        FT_Int     degree,
                        FT_Fixed*  akerning )
  {
    FT_Service_Kerning  service;
    FT_Error            error = FT_Err_Ok;


    إذا ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );

    إذا ( !akerning )
      أرجع FT_THROW( Invalid_دلیل );

    FT_FACE_پیدا_کړه_SERVICE( face, service, KERNING );
    إذا ( !service )
      أرجع FT_THROW( Unimplemented_Feature );

    error = service->get_track( face,
                                ټکی_اندازه,
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
    إذا ( encoding == FT_ENCODING_NONE && !face->عدد_charmaps )
      أرجع FT_THROW( Invalid_دلیل );

    /* FT_ENCODING_UNICODE is special.  We try to find the `best' Unicode */
    /* charmap available, i.e., one with UCS-4 characters, if possible.   */
    /*                                                                    */
    /* This is done by find_unicode_charmap() above, to share code.       */
    إذا ( encoding == FT_ENCODING_UNICODE )
      أرجع پیدا_کړه_unicode_charmap( face );

    cur = face->charmaps;
    إذا ( !cur )
      أرجع FT_THROW( Invalid_CharMap_Handle );

    حد = cur + face->عدد_charmaps;

    لكل ( ; cur < حد; cur++ )
    {
      إذا ( cur[0]->encoding == encoding )
      {
        face->charmap = cur[0];
        أرجع FT_Err_Ok;
      }
    }

    أرجع FT_THROW( Invalid_دلیل );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_سېټ_Charmap( FT_Face     face,
                  FT_CharMap  charmap )
  {
    FT_CharMap*  cur;
    FT_CharMap*  حد;


    إذا ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );

    cur = face->charmaps;
    إذا ( !cur || !charmap )
      أرجع FT_THROW( Invalid_CharMap_Handle );

    حد = cur + face->عدد_charmaps;

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

    أرجع FT_THROW( Invalid_دلیل );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Int )
  FT_Get_Charmap_شاخص( FT_CharMap  charmap )
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
  ft_cmap_بشپړ_دننی( FT_CMap  cmap )
  {
    FT_CMap_Class  clazz  = cmap->clazz;
    FT_Face        face   = cmap->charmap.face;
    FT_حافظه      حافظه = FT_FACE_حافظه( face );


    إذا ( clazz->بشپړ )
      clazz->بشپړ( cmap );

    FT_FREE( cmap );
  }


  FT_بنسټ_DEF( فراغ )
  FT_CMap_بشپړ( FT_CMap  cmap )
  {
    إذا ( cmap )
    {
      FT_Face    face   = cmap->charmap.face;
      FT_حافظه  حافظه = FT_FACE_حافظه( face );
      FT_Error   error;
      FT_Int     i, j;


      لكل ( i = 0; i < face->عدد_charmaps; i++ )
      {
        إذا ( (FT_CMap)face->charmaps[i] == cmap )
        {
          FT_CharMap  وروستی_charmap = face->charmaps[face->عدد_charmaps - 1];


          إذا ( FT_QRENEW_ARRAY( face->charmaps,
                                face->عدد_charmaps,
                                face->عدد_charmaps - 1 ) )
            أرجع;

          /* remove it from our list of charmaps */
          لكل ( j = i + 1; j < face->عدد_charmaps; j++ )
          {
            إذا ( j == face->عدد_charmaps - 1 )
              face->charmaps[j - 1] = وروستی_charmap;
            وإلا
              face->charmaps[j - 1] = face->charmaps[j];
          }

          face->عدد_charmaps--;

          إذا ( (FT_CMap)face->charmap == cmap )
            face->charmap = عدم;

          ft_cmap_بشپړ_دننی( cmap );

          اكسر;
        }
      }
    }
  }


  FT_بنسټ_DEF( FT_Error )
  FT_CMap_New( FT_CMap_Class  clazz,
               FT_اشاره_ګر     init_معلومات,
               FT_CharMap     charmap,
               FT_CMap       *acmap )
  {
    FT_Error   error;
    FT_Face    face;
    FT_حافظه  حافظه;
    FT_CMap    cmap = عدم;


    إذا ( !clazz || !charmap || !charmap->face )
      أرجع FT_THROW( Invalid_دلیل );

    face   = charmap->face;
    حافظه = FT_FACE_حافظه( face );

    إذا ( !FT_ALLOC( cmap, clazz->اندازه ) )
    {
      cmap->charmap = *charmap;
      cmap->clazz   = clazz;

      إذا ( clazz->init )
      {
        error = clazz->init( cmap, init_معلومات );
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
    ft_cmap_بشپړ_دننی( cmap );
    cmap = عدم;
    اذهب Exit;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt )
  FT_Get_Char_شاخص( FT_Face   face,
                     FT_ULong  charcode )
  {
    FT_UInt  پایله = 0;


    إذا ( face && face->charmap )
    {
      FT_CMap  cmap = FT_CMAP( face->charmap );


      إذا ( charcode > 0xFFFFFFFFUL )
      {
        FT_TRACE1(( "FT_Get_Char_Index: too large charcode" ));
        FT_TRACE1(( " 0x%lx is truncated\n", charcode ));
      }

      پایله = cmap->clazz->char_شاخص( cmap, (FT_UInt32)charcode );
      إذا ( پایله >= (FT_UInt)face->عدد_glyphs )
        پایله = 0;
    }

    أرجع پایله;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_ULong )
  FT_Get_لومړی_Char( FT_Face   face,
                     FT_UInt  *agindex )
  {
    FT_ULong  پایله = 0;
    FT_UInt   gindex = 0;


    /* only do something if we have a charmap, and we have glyphs at all */
    إذا ( face && face->charmap && face->عدد_glyphs )
    {
      gindex = FT_Get_Char_شاخص( face, 0 );
      إذا ( gindex == 0 )
        پایله = FT_Get_بل_Char( face, 0, &gindex );
    }

    إذا ( agindex )
      *agindex = gindex;

    أرجع پایله;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_ULong )
  FT_Get_بل_Char( FT_Face   face,
                    FT_ULong  charcode,
                    FT_UInt  *agindex )
  {
    FT_ULong  پایله = 0;
    FT_UInt   gindex = 0;


    إذا ( face && face->charmap && face->عدد_glyphs )
    {
      FT_UInt32  کوډ = (FT_UInt32)charcode;
      FT_CMap    cmap = FT_CMAP( face->charmap );


      افعل
      {
        gindex = cmap->clazz->char_بل( cmap, &کوډ );

      } طالما ( gindex >= (FT_UInt)face->عدد_glyphs );

      پایله = ( gindex == 0 ) ? 0 : کوډ;
    }

    إذا ( agindex )
      *agindex = gindex;

    أرجع پایله;
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
      error = FT_THROW( Invalid_دلیل );
      اذهب Exit;
    }

    لكل ( ; عدد_properties > 0; عدد_properties-- )
    {
      إذا ( properties->نښان == FT_PARAM_نښان_STEM_DARKENING )
      {
        إذا ( properties->معلومات )
        {
          إذا ( *( (FT_Bool*)properties->معلومات ) == TRUE )
            face->دننی->no_stem_darkening = FALSE;
          وإلا
            face->دننی->no_stem_darkening = TRUE;
        }
        وإلا
        {
          /* use module default */
          face->دننی->no_stem_darkening = -1;
        }
      }
      وإلا إذا ( properties->نښان == FT_PARAM_نښان_LCD_FILTER_WEIGHTS )
      {
        error = FT_THROW( Unimplemented_Feature );
        اذهب Exit;
      }
      وإلا إذا ( properties->نښان == FT_PARAM_نښان_RANDOM_تخم )
      {
        إذا ( properties->معلومات )
        {
          face->دننی->random_تخم = *( (FT_Int32*)properties->معلومات );
          إذا ( face->دننی->random_تخم < 0 )
            face->دننی->random_تخم = 0;
        }
        وإلا
        {
          /* use module default */
          face->دننی->random_تخم = -1;
        }
      }
      وإلا
      {
        error = FT_THROW( Invalid_دلیل );
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
    FT_UInt  پایله = 0;


    إذا ( face                                           &&
         face->charmap                                  &&
         face->charmap->encoding == FT_ENCODING_UNICODE )
    {
      FT_CharMap  charmap = پیدا_کړه_variant_selector_charmap( face );
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

        پایله = vcmap->clazz->char_var_شاخص( vcmap, ucmap,
                                               (FT_UInt32)charcode,
                                               (FT_UInt32)variantSelector );
      }
    }

    أرجع پایله;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Int )
  FT_Face_GetCharVariantIsDefault( FT_Face   face,
                                   FT_ULong  charcode,
                                   FT_ULong  variantSelector )
  {
    FT_Int  پایله = -1;


    إذا ( face )
    {
      FT_CharMap  charmap = پیدا_کړه_variant_selector_charmap( face );


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

        پایله = vcmap->clazz->char_var_default( vcmap,
                                                 (FT_UInt32)charcode,
                                                 (FT_UInt32)variantSelector );
      }
    }

    أرجع پایله;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt32* )
  FT_Face_GetVariantSelectors( FT_Face  face )
  {
    FT_UInt32  *پایله = عدم;


    إذا ( face )
    {
      FT_CharMap  charmap = پیدا_کړه_variant_selector_charmap( face );


      إذا ( charmap )
      {
        FT_CMap    vcmap  = FT_CMAP( charmap );
        FT_حافظه  حافظه = FT_FACE_حافظه( face );


        پایله = vcmap->clazz->variant_list( vcmap, حافظه );
      }
    }

    أرجع پایله;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt32* )
  FT_Face_Get_Variants_د_Char( FT_Face   face,
                             FT_ULong  charcode )
  {
    FT_UInt32  *پایله = عدم;


    إذا ( face )
    {
      FT_CharMap  charmap = پیدا_کړه_variant_selector_charmap( face );


      إذا ( charmap )
      {
        FT_CMap    vcmap  = FT_CMAP( charmap );
        FT_حافظه  حافظه = FT_FACE_حافظه( face );


        إذا ( charcode > 0xFFFFFFFFUL )
        {
          FT_TRACE1(( "FT_Face_GetVariantsOfChar: too large charcode" ));
          FT_TRACE1(( " 0x%lx is truncated\n", charcode ));
        }

        پایله = vcmap->clazz->charvariant_list( vcmap, حافظه,
                                                 (FT_UInt32)charcode );
      }
    }
    أرجع پایله;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt32* )
  FT_Face_Get_Chars_د_Variant( FT_Face   face,
                             FT_ULong  variantSelector )
  {
    FT_UInt32  *پایله = عدم;


    إذا ( face )
    {
      FT_CharMap  charmap = پیدا_کړه_variant_selector_charmap( face );


      إذا ( charmap )
      {
        FT_CMap    vcmap  = FT_CMAP( charmap );
        FT_حافظه  حافظه = FT_FACE_حافظه( face );


        إذا ( variantSelector > 0xFFFFFFFFUL )
        {
          FT_TRACE1(( "FT_Get_Char_Index: too large variantSelector" ));
          FT_TRACE1(( " 0x%lx is truncated\n", variantSelector ));
        }

        پایله = vcmap->clazz->variantchar_list( vcmap, حافظه,
                                                 (FT_UInt32)variantSelector );
      }
    }

    أرجع پایله;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt )
  FT_Get_Name_شاخص( FT_Face           face,
                     ثابت FT_String*  glyph_name )
  {
    FT_UInt  پایله = 0;


    إذا ( face                       &&
         FT_HAS_GLYPH_نومونه( face ) &&
         glyph_name                 )
    {
      FT_Service_GlyphDict  service;


      FT_FACE_LOOKUP_SERVICE( face,
                              service,
                              GLYPH_DICT );

      إذا ( service && service->name_شاخص )
        پایله = service->name_شاخص( face, glyph_name );
    }

    أرجع پایله;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Glyph_Name( FT_Face     face,
                     FT_UInt     glyph_شاخص,
                     FT_اشاره_ګر  بفر,
                     FT_UInt     بفر_max )
  {
    FT_Error              error;
    FT_Service_GlyphDict  service;


    إذا ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );

    إذا ( !بفر || بفر_max == 0 )
      أرجع FT_THROW( Invalid_دلیل );

    /* clean up buffer */
    ((FT_Byte*)بفر)[0] = '\0';

    إذا ( (FT_Long)glyph_شاخص >= face->عدد_glyphs )
      أرجع FT_THROW( Invalid_Glyph_شاخص );

    إذا ( !FT_HAS_GLYPH_نومونه( face ) )
      أرجع FT_THROW( Invalid_دلیل );

    FT_FACE_LOOKUP_SERVICE( face, service, GLYPH_DICT );
    إذا ( service && service->get_name )
      error = service->get_name( face, glyph_شاخص, بفر, بفر_max );
    وإلا
      error = FT_THROW( Invalid_دلیل );

    أرجع error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( ثابت حرف* )
  FT_Get_Postscript_Name( FT_Face  face )
  {
    ثابت حرف*  پایله = عدم;


    إذا ( !face )
      اذهب Exit;

    إذا ( !پایله )
    {
      FT_Service_PsFontName  service;


      FT_FACE_LOOKUP_SERVICE( face,
                              service,
                              POSTSCRIPT_FONT_NAME );

      إذا ( service && service->get_ps_font_name )
        پایله = service->get_ps_font_name( face );
    }

  Exit:
    أرجع پایله;
  }


  /* documentation is in tttables.h */

  FT_EXPORT_DEF( فراغ* )
  FT_Get_Sfnt_جدول( FT_Face      face,
                     FT_Sfnt_نښان  نښان )
  {
    فراغ*                  جدول = عدم;
    FT_Service_SFNT_جدول  service;


    إذا ( face && FT_IS_SFNT( face ) )
    {
      FT_FACE_پیدا_کړه_SERVICE( face, service, SFNT_جدول );
      إذا ( service )
        جدول = service->get_جدول( face, نښان );
    }

    أرجع جدول;
  }


  /* documentation is in tttables.h */

  FT_EXPORT_DEF( FT_Error )
  FT_بارول_Sfnt_جدول( FT_Face    face,
                      FT_ULong   نښان,
                      FT_Long    دننه_واټن,
                      FT_Byte*   بفر,
                      FT_ULong*  اوږدتیا )
  {
    FT_Service_SFNT_جدول  service;


    إذا ( !face || !FT_IS_SFNT( face ) )
      أرجع FT_THROW( Invalid_Face_Handle );

    FT_FACE_پیدا_کړه_SERVICE( face, service, SFNT_جدول );
    إذا ( !service )
      أرجع FT_THROW( Unimplemented_Feature );

    أرجع service->بارول_جدول( face, نښان, دننه_واټن, بفر, اوږدتیا );
  }


  /* documentation is in tttables.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Sfnt_جدول_Info( FT_Face    face,
                      FT_UInt    جدول_شاخص,
                      FT_ULong  *نښان,
                      FT_ULong  *اوږدتیا )
  {
    FT_Service_SFNT_جدول  service;
    FT_ULong               دننه_واټن;


    /* test for valid `length' delayed to `service->table_info' */

    إذا ( !face || !FT_IS_SFNT( face ) )
      أرجع FT_THROW( Invalid_Face_Handle );

    FT_FACE_پیدا_کړه_SERVICE( face, service, SFNT_جدول );
    إذا ( !service )
      أرجع FT_THROW( Unimplemented_Feature );

    أرجع service->جدول_info( face, جدول_شاخص, نښان, &دننه_واټن, اوږدتیا );
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
    FT_FACE_پیدا_کړه_SERVICE( face, service, TT_CMAP );
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
    FT_FACE_پیدا_کړه_SERVICE( face, service, TT_CMAP );
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
  FT_بنسټ_DEF( FT_Renderer )
  FT_Lookup_Renderer( FT_Library       library,
                      FT_Glyph_Format  format,
                      FT_List_غوټه*     غوټه )
  {
    FT_List_غوټه  cur;
    FT_Renderer  پایله = عدم;


    إذا ( !library )
      اذهب Exit;

    cur = library->renderers.سر_2;

    إذا ( غوټه )
    {
      إذا ( *غوټه )
        cur = (*غوټه)->بل;
      *غوټه = عدم;
    }

    طالما ( cur )
    {
      FT_Renderer  renderer = FT_RENDERER( cur->معلومات );


      إذا ( renderer->glyph_format == format )
      {
        إذا ( غوټه )
          *غوټه = cur;

        پایله = renderer;
        اكسر;
      }
      cur = cur->بل;
    }

  Exit:
    أرجع پایله;
  }


  ساكن FT_Renderer
  ft_lookup_glyph_renderer( FT_Glyph_خونه  خونه )
  {
    FT_Face      face    = خونه->face;
    FT_Library   library = FT_FACE_LIBRARY( face );
    FT_Renderer  پایله  = library->cur_renderer;


    إذا ( !پایله || پایله->glyph_format != خونه->format )
      پایله = FT_Lookup_Renderer( library, خونه->format, 0 );

    أرجع پایله;
  }


  ساكن فراغ
  ft_سېټ_current_renderer( FT_Library  library )
  {
    FT_Renderer  renderer;


    renderer = FT_Lookup_Renderer( library, FT_GLYPH_FORMAT_OUTLINE, 0 );
    library->cur_renderer = renderer;
  }


  ساكن FT_Error
  ft_جمع_کړه_renderer( FT_Module  module )
  {
    FT_Library   library = module->library;
    FT_حافظه    حافظه  = library->حافظه;
    FT_Error     error;
    FT_List_غوټه  غوټه    = عدم;


    إذا ( FT_QNEW( غوټه ) )
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

#که_تعریف_شوی FT_CONFIG_OPTION_SVG
      إذا ( clazz->glyph_format == FT_GLYPH_FORMAT_SVG )
        render->render = clazz->render_glyph;
#د_که_پای

      /* add to list */
      غوټه->معلومات = module;
      FT_List_جمع_کړه( &library->renderers, غوټه );

      ft_سېټ_current_renderer( library );
    }

  Fail:
    إذا ( error )
      FT_FREE( غوټه );

  Exit:
    أرجع error;
  }


  ساكن فراغ
  ft_remove_renderer( FT_Module  module )
  {
    FT_Library   library;
    FT_حافظه    حافظه;
    FT_List_غوټه  غوټه;


    library = module->library;
    إذا ( !library )
      أرجع;

    حافظه = library->حافظه;

    غوټه = FT_List_پیدا_کړه( &library->renderers, module );
    إذا ( غوټه )
    {
      FT_Renderer  render = FT_RENDERER( module );


      /* release raster object, if any */
      إذا ( render->raster )
        render->clazz->raster_class->raster_بشپړ( render->raster );

      /* remove from list */
      FT_List_Remove( &library->renderers, غوټه );
      FT_FREE( غوټه );

      ft_سېټ_current_renderer( library );
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
  FT_سېټ_Renderer( FT_Library     library,
                   FT_Renderer    renderer,
                   FT_UInt        عدد_params,
                   FT_Parameter*  parameters )
  {
    FT_List_غوټه  غوټه;
    FT_Error     error = FT_Err_Ok;

    FT_Renderer_سېټ_ډېرپېښ_Func  سېټ_ډېرپېښ;


    إذا ( !library )
    {
      error = FT_THROW( Invalid_Library_Handle );
      اذهب Exit;
    }

    إذا ( !renderer )
    {
      error = FT_THROW( Invalid_دلیل );
      اذهب Exit;
    }

    إذا ( عدد_params > 0 && !parameters )
    {
      error = FT_THROW( Invalid_دلیل );
      اذهب Exit;
    }

    غوټه = FT_List_پیدا_کړه( &library->renderers, renderer );
    إذا ( !غوټه )
    {
      error = FT_THROW( Invalid_دلیل );
      اذهب Exit;
    }

    FT_List_پورته( &library->renderers, غوټه );

    إذا ( renderer->glyph_format == FT_GLYPH_FORMAT_OUTLINE )
      library->cur_renderer = renderer;

    سېټ_ډېرپېښ = renderer->clazz->سېټ_ډېرپېښ;

    لكل ( ; عدد_params > 0; عدد_params-- )
    {
      error = سېټ_ډېرپېښ( renderer, parameters->نښان, parameters->معلومات );
      إذا ( error )
        اكسر;
      parameters++;
    }

  Exit:
    أرجع error;
  }


  FT_بنسټ_DEF( FT_Error )
  FT_Render_Glyph_دننی( FT_Library      library,
                            FT_Glyph_خونه    خونه,
                            FT_Render_ډېرپېښ  render_ډېرپېښ )
  {
    FT_Error     error = FT_Err_Ok;
    FT_Face      face  = خونه->face;
    FT_Renderer  renderer;
    FT_List_غوټه  غوټه  = عدم;


    /* try to render colored glyph layers as a special case */
    إذا ( خونه->دننی->بارول_flags & FT_بارول_COLOR &&
         خونه->format == FT_GLYPH_FORMAT_OUTLINE    )
    {
      FT_LayerIterator  iterator;

      FT_UInt  بنسټ_glyph = خونه->glyph_شاخص;

      FT_Bool  have_layers;
      FT_UInt  glyph_شاخص;
      FT_UInt  color_شاخص;


      iterator.p  = عدم;
      have_layers = FT_Get_Color_Glyph_Layer( face,
                                              بنسټ_glyph,
                                              &glyph_شاخص,
                                              &color_شاخص,
                                              &iterator );
      إذا ( have_layers )
      {
        error = FT_New_Glyph_خونه( face, عدم );
        إذا ( !error )
        {
          TT_Face       ttface = (TT_Face)face;
          SFNT_Service  sfnt   = (SFNT_Service)ttface->sfnt;


          افعل
          {
            FT_Int32  بارول_flags = خونه->دننی->بارول_flags;


            /* disable the `FT_LOAD_COLOR' flag to avoid recursion */
            /* right here in this function                         */
            بارول_flags &= ~FT_بارول_COLOR;

            /* render into the new `face->glyph' glyph slot */
            بارول_flags |= FT_بارول_RENDER | FT_بارول_NO_BITMAP;

            error = FT_بارول_Glyph( face, glyph_شاخص, بارول_flags );
            إذا ( error )
              اكسر;

            /* blend new `face->glyph' into old `slot'; */
            /* at the first call, `slot' is still empty */
            error = sfnt->colr_blend( ttface,
                                      color_شاخص,
                                      خونه,
                                      face->glyph );
            إذا ( error )
              اكسر;

          } طالما ( FT_Get_Color_Glyph_Layer( face,
                                              بنسټ_glyph,
                                              &glyph_شاخص,
                                              &color_شاخص,
                                              &iterator ) );

          إذا ( !error )
            خونه->format = FT_GLYPH_FORMAT_BITMAP;

          /* this call also restores `slot' as the glyph slot */
          FT_بشپړ_Glyph_خونه( face->glyph );
        }

        إذا ( !error )
          أرجع error;

        /* Failed to do the colored layer.  Draw outline instead. */
        خونه->format = FT_GLYPH_FORMAT_OUTLINE;
      }
    }

    /* small shortcut for the very common case */
    إذا ( خونه->format == FT_GLYPH_FORMAT_OUTLINE )
    {
      renderer = library->cur_renderer;
      غوټه     = library->renderers.سر_2;
    }
    وإلا
      renderer = FT_Lookup_Renderer( library, خونه->format, &غوټه );

    error = FT_ERR( Cannot_Render_Glyph );
    طالما ( renderer )
    {
      error = renderer->render( renderer, خونه, render_ډېرپېښ, عدم );
      إذا ( !error                                   ||
           FT_ERR_NEQ( error, Cannot_Render_Glyph ) )
        اكسر;

      /* `FT_Err_Cannot_Render_Glyph` is returned if the render mode */
      /* is unsupported by the current renderer for this glyph image */
      /* format.                                                     */

      /* now, look for another renderer that supports the same */
      /* format.                                               */
      renderer = FT_Lookup_Renderer( library, خونه->format, &غوټه );
    }

    /* it is not an error if we cannot render a bitmap glyph */
    إذا ( FT_ERR_EQ( error, Cannot_Render_Glyph ) &&
         خونه->format == FT_GLYPH_FORMAT_BITMAP  )
      error = FT_Err_Ok;

#که_تعریف_شوی FT_DEBUG_LEVEL_TRACE

#تعریف_لغوه  FT_COMPONENT
#تعریف_کړه FT_COMPONENT  checksum

    /*
     * Computing the MD5 checksum is expensive, unnecessarily distorting a
     * possible profiling of FreeType if compiled with tracing support.  For
     * this reason, we execute the following code only if explicitly
     * requested.
     */

    /* we use FT_TRACE3 in this block */
    إذا ( !error                               &&
         ft_trace_پوړونه[trace_checksum] >= 3 &&
         خونه->bitmap.بفر                  )
    {
      FT_Bitmap  bitmap;
      FT_Error   err;


      FT_Bitmap_Init( &bitmap );

      /* we convert to a single bitmap format for computing the checksum */
      /* this also converts the bitmap flow to `down' (i.e., pitch > 0)  */
      err = FT_Bitmap_Convert( library, &خونه->bitmap, &bitmap, 1 );
      إذا ( !err )
      {
        MD5_CTX        ctx;
        بینښه حرف  md5[16];
        بینښه اوږد  coverage = 0;
        صحيح            i, j;
        صحيح            لیکې  = (صحيح)bitmap.لیکې;
        صحيح            pitch = bitmap.pitch;


        FT_TRACE3(( "FT_Render_Glyph: bitmap %dx%d, %s (mode %d)\n",
                    pitch,
                    لیکې,
                    pixel_modes[خونه->bitmap.pixel_ډېرپېښ],
                    خونه->bitmap.pixel_ډېرپېښ ));

        لكل ( i = 0; i < لیکې; i++ )
          لكل ( j = 0; j < pitch; j++ )
            coverage += bitmap.بفر[i * pitch + j];

        FT_TRACE3(( "  Total coverage: %lu\n", coverage ));

        MD5_Init( &ctx );
        إذا ( bitmap.بفر )
          MD5_Update( &ctx, bitmap.بفر,
                      (بینښه اوږد)لیکې * (بینښه اوږد)pitch );
        MD5_Final( md5, &ctx );

        FT_TRACE3(( "  MD5 checksum: " ));
        لكل ( i = 0; i < 16; i++ )
          FT_TRACE3(( "%02X", md5[i] ));
        FT_TRACE3(( "\n" ));
      }

      FT_Bitmap_بشپړ( library, &bitmap );
    }

    /*
     * Dump bitmap in Netpbm format (PBM or PGM).
     */

    /* we use FT_TRACE7 in this block */
    إذا ( !error                               &&
         ft_trace_پوړونه[trace_checksum] >= 7 &&
         خونه->bitmap.بفر                  )
    {
      إذا ( خونه->bitmap.لیکې  < 128U &&
           خونه->bitmap.پلنوالی < 128U )
      {
        صحيح  لیکې  = (صحيح)خونه->bitmap.لیکې;
        صحيح  پلنوالی = (صحيح)خونه->bitmap.پلنوالی;
        صحيح  pitch =      خونه->bitmap.pitch;
        صحيح  i, j, m;

        بینښه حرف*  topleft = خونه->bitmap.بفر;


        إذا ( pitch < 0 )
          topleft -= pitch * ( لیکې - 1 );

        FT_TRACE7(( "Netpbm image: start\n" ));
        بدل ( خونه->bitmap.pixel_ډېرپېښ )
        {
        حالة FT_PIXEL_ډېرپېښ_MONO:
          FT_TRACE7(( "P1 %d %d\n", پلنوالی, لیکې ));
          لكل ( i = 0; i < لیکې; i++ )
          {
            لكل ( j = 0; j < پلنوالی; )
              لكل ( m = 128; m > 0 && j < پلنوالی; m >>= 1, j++ )
                FT_TRACE7(( " %d",
                            ( topleft[i * pitch + j / 8] & m ) != 0 ));
            FT_TRACE7(( "\n" ));
          }
          اكسر;

        افتراضي:
          FT_TRACE7(( "P2 %d %d 255\n", پلنوالی, لیکې ));
          لكل ( i = 0; i < لیکې; i++ )
          {
            لكل ( j = 0; j < پلنوالی; j += 1 )
              FT_TRACE7(( " %3u", topleft[i * pitch + j] ));
            FT_TRACE7(( "\n" ));
          }
        }
        FT_TRACE7(( "Netpbm image: end\n" ));
      }
      وإلا
        FT_TRACE7(( "Netpbm image: too large, omitted\n" ));
    }

#تعریف_لغوه  FT_COMPONENT
#تعریف_کړه FT_COMPONENT  objs

#د_که_پای /* FT_DEBUG_LEVEL_TRACE */

    أرجع error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Render_Glyph( FT_Glyph_خونه    خونه,
                   FT_Render_ډېرپېښ  render_ډېرپېښ )
  {
    FT_Library  library;


    إذا ( !خونه || !خونه->face )
      أرجع FT_THROW( Invalid_دلیل );

    library = FT_FACE_LIBRARY( خونه->face );

    أرجع FT_Render_Glyph_دننی( library, خونه, render_ډېرپېښ );
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
    إذا ( clazz->module_بشپړ )
      clazz->module_بشپړ( module );

    /* discard it */
    FT_FREE( module );
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_جمع_کړه_Module( FT_Library              library,
                 ثابت FT_Module_Class*  clazz )
  {
    FT_Error   error;
    FT_حافظه  حافظه;
    FT_Module  module = عدم;
    FT_UInt    nn;


#تعریف_کړه FREETYPE_VER_FIXED  ( ( (FT_Long)FREETYPE_MAJOR << 16 ) | \
                                FREETYPE_MINOR                  )

    إذا ( !library )
      أرجع FT_THROW( Invalid_Library_Handle );

    إذا ( !clazz )
      أرجع FT_THROW( Invalid_دلیل );

    /* check FreeType version */
    إذا ( clazz->module_requires > FREETYPE_VER_FIXED )
      أرجع FT_THROW( Invalid_نسخه );

    /* look for a module with the same name in the library's table */
    لكل ( nn = 0; nn < library->عدد_modules; nn++ )
    {
      module = library->modules[nn];
      إذا ( ft_strcmp( module->clazz->module_name, clazz->module_name ) == 0 )
      {
        /* this installed module has the same name, compare their versions */
        إذا ( clazz->module_نسخه <= module->clazz->module_نسخه )
          أرجع FT_THROW( ښکتنی_Module_نسخه );

        /* remove the module from our list, then exit the loop to replace */
        /* it by our new version..                                        */
        FT_Remove_Module( library, module );
        اكسر;
      }
    }

    حافظه = library->حافظه;
    error  = FT_Err_Ok;

    إذا ( library->عدد_modules >= FT_MAX_MODULES )
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
      error = ft_جمع_کړه_renderer( module );
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
        renderer->clazz->raster_class->raster_بشپړ( renderer->raster );
    }

    FT_FREE( module );
    اذهب Exit;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Module )
  FT_Get_Module( FT_Library   library,
                 ثابت حرف*  module_name )
  {
    FT_Module   پایله = عدم;
    FT_Module*  cur;
    FT_Module*  حد;


    إذا ( !library || !module_name )
      أرجع پایله;

    cur   = library->modules;
    حد = cur + library->عدد_modules;

    لكل ( ; cur < حد; cur++ )
      إذا ( ft_strcmp( cur[0]->clazz->module_name, module_name ) == 0 )
      {
        پایله = cur[0];
        اكسر;
      }

    أرجع پایله;
  }


  /* documentation is in ftobjs.h */

  FT_بنسټ_DEF( ثابت فراغ* )
  FT_Get_Module_Interface( FT_Library   library,
                           ثابت حرف*  mod_name )
  {
    FT_Module  module;


    /* test for valid `library' delayed to FT_Get_Module() */

    module = FT_Get_Module( library, mod_name );

    أرجع module ? module->clazz->module_interface : 0;
  }


  FT_بنسټ_DEF( FT_اشاره_ګر )
  ft_module_get_service( FT_Module    module,
                         ثابت حرف*  service_id,
                         FT_Bool      global )
  {
    FT_اشاره_ګر  پایله = عدم;


    إذا ( module )
    {
      FT_ASSERT( module->clazz && module->clazz->get_interface );

      /* first, look for the service in the module */
      إذا ( module->clazz->get_interface )
        پایله = module->clazz->get_interface( module, service_id );

      إذا ( global && !پایله )
      {
        /* we didn't find it, look in all other modules then */
        FT_Library  library = module->library;
        FT_Module*  cur     = library->modules;
        FT_Module*  حد   = cur + library->عدد_modules;


        لكل ( ; cur < حد; cur++ )
        {
          إذا ( cur[0] != module )
          {
            FT_ASSERT( cur[0]->clazz );

            إذا ( cur[0]->clazz->get_interface )
            {
              پایله = cur[0]->clazz->get_interface( cur[0], service_id );
              إذا ( پایله )
                اكسر;
            }
          }
        }
      }
    }

    أرجع پایله;
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
      FT_Module*  حد = cur + library->عدد_modules;


      لكل ( ; cur < حد; cur++ )
      {
        إذا ( cur[0] == module )
        {
          /* remove it from the table */
          library->عدد_modules--;
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
                  فراغ*             ارزښت,
                  FT_Bool           سېټ,
                  FT_Bool           ارزښت_is_string )
  {
    FT_Module*           cur;
    FT_Module*           حد;
    FT_Module_Interface  interface;

    FT_Service_Properties  service;

#که_تعریف_شوی FT_DEBUG_LEVEL_ERROR
    ثابت FT_String*  سېټ_name  = "FT_Property_Set";
    ثابت FT_String*  get_name  = "FT_Property_Get";
    ثابت FT_String*  func_name = سېټ ? سېټ_name : get_name;
#د_که_پای

    FT_Bool  missing_func;


    إذا ( !library )
      أرجع FT_THROW( Invalid_Library_Handle );

    إذا ( !module_name || !property_name || !ارزښت )
      أرجع FT_THROW( Invalid_دلیل );

    cur   = library->modules;
    حد = cur + library->عدد_modules;

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

    إذا ( سېټ )
      missing_func = FT_BOOL( !service->سېټ_property );
    وإلا
      missing_func = FT_BOOL( !service->get_property );

    إذا ( missing_func )
    {
      FT_TRACE2(( "%s: property service of module `%s' is broken\n",
                  func_name, module_name ));
      أرجع FT_THROW( Unimplemented_Feature );
    }

    أرجع سېټ ? service->سېټ_property( cur[0],
                                        property_name,
                                        ارزښت,
                                        ارزښت_is_string )
               : service->get_property( cur[0],
                                        property_name,
                                        ارزښت );
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Property_سېټ( FT_Library        library,
                   ثابت FT_String*  module_name,
                   ثابت FT_String*  property_name,
                   ثابت فراغ*       ارزښت )
  {
    أرجع ft_property_do( library,
                           module_name,
                           property_name,
                           (فراغ*)ارزښت,
                           TRUE,
                           FALSE );
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Property_Get( FT_Library        library,
                   ثابت FT_String*  module_name,
                   ثابت FT_String*  property_name,
                   فراغ*             ارزښت )
  {
    أرجع ft_property_do( library,
                           module_name,
                           property_name,
                           ارزښت,
                           FALSE,
                           FALSE );
  }


#که_تعریف_شوی FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES

  /* this variant is used for handling the FREETYPE_PROPERTIES */
  /* environment variable                                      */

  FT_بنسټ_DEF( FT_Error )
  ft_property_string_سېټ( FT_Library        library,
                          ثابت FT_String*  module_name,
                          ثابت FT_String*  property_name,
                          FT_String*        ارزښت )
  {
    أرجع ft_property_do( library,
                           module_name,
                           property_name,
                           (فراغ*)ارزښت,
                           TRUE,
                           TRUE );
  }

#د_که_پای


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
      أرجع FT_THROW( Invalid_دلیل );

#که_نه_تعریف_شوی FT_DEBUG_LOGGING
#که_تعریف_شوی FT_DEBUG_LEVEL_ERROR
    /* init debugging support */
    ft_debug_init();
#د_که_پای /* FT_DEBUG_LEVEL_ERROR */
#د_که_پای /* !FT_DEBUG_LOGGING */

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
  FT_بشپړ_Library( FT_Library  library )
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
          طالما ( faces->سر_2 )
          {
            FT_بشپړ_Face( FT_FACE( faces->سر_2->معلومات ) );
            إذا ( faces->سر_2 )
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
#د_که_پای

    FT_FREE( library );

  Exit:
    أرجع FT_Err_Ok;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( فراغ )
  FT_سېټ_Debug_Hook( FT_Library         library,
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
    FT_TrueTypeEngineType  پایله = FT_TRUETYPE_ENGINE_TYPE_NONE;


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
          پایله = service->engine_type;
      }
    }

    أرجع پایله;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_SubGlyph_Info( FT_Glyph_خونه  glyph,
                        FT_UInt       sub_شاخص,
                        FT_Int       *p_شاخص,
                        FT_UInt      *p_flags,
                        FT_Int       *p_arg1,
                        FT_Int       *p_arg2,
                        FT_ماتریکس    *p_transform )
  {
    FT_Error  error = FT_ERR( Invalid_دلیل );


    إذا ( glyph                                      &&
         glyph->subglyphs                           &&
         glyph->format == FT_GLYPH_FORMAT_مرکب &&
         sub_شاخص < glyph->عدد_subglyphs           )
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
                            FT_UInt            بنسټ_glyph,
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
         بنسټ_glyph >= (FT_UInt)face->عدد_glyphs )
      أرجع 0;

    إذا ( !FT_IS_SFNT( face ) )
      أرجع 0;

    ttface = (TT_Face)face;
    sfnt   = (SFNT_Service)ttface->sfnt;

    إذا ( sfnt->get_colr_layer )
      أرجع sfnt->get_colr_layer( ttface,
                                   بنسټ_glyph,
                                   aglyph_شاخص,
                                   acolor_شاخص,
                                   iterator );
    وإلا
      أرجع 0;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Get_Color_Glyph_Paint( FT_Face                  face,
                            FT_UInt                  بنسټ_glyph,
                            FT_Color_ریښه_Transform  ریښه_transform,
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
                                         بنسټ_glyph,
                                         ریښه_transform,
                                         paint );
    وإلا
      أرجع 0;
  }


  /* documentation is in ftcolor.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Get_Color_Glyph_ClipBox( FT_Face      face,
                              FT_UInt      بنسټ_glyph,
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
                                            بنسټ_glyph,
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


/*پای_ټکی*/
