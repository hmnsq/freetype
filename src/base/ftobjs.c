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

#جيڪڏهن_وصف_ٿيل TT_CONFIG_OPTION_VARC
#ضمن <freetype/internal/services/svvarc.h>
#جيڪڏهن_پورو

#ضمن <freetype/ftdriver.h>

#جيڪڏهن_وصف_ٿيل FT_CONFIG_OPTION_MAC_FONTS
#ضمن "ftbase.h"
#جيڪڏهن_پورو


#جيڪڏهن_وصف_ٿيل FT_DEBUG_LEVEL_TRACE

#ضمن <freetype/ftbitmap.h>

#إذا defined( _MSC_VER )      /* Visual C++ (and Intel C++)   */
  /* We disable the warning `conversion from XXX to YYY,     */
  /* possible loss of data' in order to compile cleanly with */
  /* the maximum level of warnings: `md5.c' is non-FreeType  */
  /* code, and it gets used during development builds only.  */
#هدايت warning( ڌڪو )
#هدايت warning( disable : 4244 )
#جيڪڏهن_پورو /* _MSC_VER */

  /* It's easiest to include `md5.c' directly.  However, since OpenSSL */
  /* also provides the same functions, there might be conflicts if     */
  /* both FreeType and OpenSSL are built as static libraries.  For     */
  /* this reason, we put the MD5 stuff into the `FT_' namespace.       */
#وصف_ڪريو MD5_u32plus  FT_MD5_u32plus
#وصف_ڪريو MD5_CTX      FT_MD5_CTX
#وصف_ڪريو MD5_Init     FT_MD5_Init
#وصف_ڪريو MD5_Update   FT_MD5_Update
#وصف_ڪريو MD5_Final    FT_MD5_Final

#وصف_رد  HAVE_OPENSSL

#ضمن "md5.c"

#إذا defined( _MSC_VER )
#هدايت warning( ڪڍو )
#جيڪڏهن_پورو

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

#جيڪڏهن_پورو /* FT_DEBUG_LEVEL_TRACE */


#وصف_ڪريو GRID_FIT_METRICS


  /* forward declaration */
  ساكن FT_Error
  ft_open_face_اندروني( FT_Library           library,
                         ثابت FT_Open_دليلون*  دليلون,
                         FT_Long              face_اشاريو,
                         FT_Face             *aface,
                         FT_Bool              test_mac_fonts );


  FT_بنياد_DEF( FT_اشارو )
  ft_service_list_lookup( FT_ServiceDesc  service_descriptors,
                          ثابت حرف*     service_id )
  {
    FT_اشارو      نتيجو = عدم;
    FT_ServiceDesc  desc   = service_descriptors;


    إذا ( desc && service_id )
    {
      لكل ( ; desc->serv_id != عدم; desc++ )
      {
        إذا ( ft_strcmp( desc->serv_id, service_id ) == 0 )
        {
          نتيجو = (FT_اشارو)desc->serv_معلومات;
          اكسر;
        }
      }
    }

    أرجع نتيجو;
  }


  FT_بنياد_DEF( فراغ )
  ft_validator_init( FT_Validator        valid,
                     ثابت FT_Byte*      بنياد,
                     ثابت FT_Byte*      حد_بندي,
                     FT_ValidationLevel  level )
  {
    valid->بنياد  = بنياد;
    valid->حد_بندي = حد_بندي;
    valid->level = level;
    valid->error = FT_Err_Ok;
  }


  FT_بنياد_DEF( FT_Int )
  ft_validator_لاڳيتو( FT_Validator  valid )
  {
    /* This function doesn't work!  None should call it. */
    FT_اڻ_استعمال_ٿيل( valid );

    أرجع -1;
  }


  FT_بنياد_DEF( فراغ )
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
  FT_بنياد_DEF( FT_Error )
  FT_وهڪرو_New( FT_Library           library,
                 ثابت FT_Open_دليلون*  دليلون,
                 FT_وهڪرو           *astream )
  {
    FT_Error   error;
    FT_يادگيري  يادگيري;
    FT_وهڪرو  وهڪرو = عدم;
    FT_UInt    گهڻائي;


    *astream = عدم;

    إذا ( !library )
      أرجع FT_THROW( Invalid_Library_Handle );

    إذا ( !دليلون )
      أرجع FT_THROW( Invalid_دليل );

    يادگيري = library->يادگيري;
    گهڻائي   = دليلون->flags &
               ( FT_OPEN_يادگيري | FT_OPEN_وهڪرو | FT_OPEN_PATHNAME );

    إذا ( گهڻائي == FT_OPEN_يادگيري )
    {
      /* create a memory-based stream */
      إذا ( FT_NEW( وهڪرو ) )
        اذهب Exit;

      FT_وهڪرو_Open_يادگيري( وهڪرو,
                            (ثابت FT_Byte*)دليلون->يادگيري_بنياد,
                            (FT_ULong)دليلون->يادگيري_سائيز );
      وهڪرو->يادگيري = يادگيري;
    }

#جيڪڏهن_وصف_نه_ٿيل FT_CONFIG_OPTION_DISABLE_وهڪرو_SUPPORT

    وإلا إذا ( گهڻائي == FT_OPEN_PATHNAME )
    {
      /* create a normal system stream */
      إذا ( FT_NEW( وهڪرو ) )
        اذهب Exit;

      وهڪرو->يادگيري = يادگيري;
      error = FT_وهڪرو_Open( وهڪرو, دليلون->pathname );
      إذا ( error )
        FT_FREE( وهڪرو );
    }
    وإلا إذا ( ( گهڻائي == FT_OPEN_وهڪرو ) && دليلون->وهڪرو )
    {
      /* use an existing, user-provided stream */

      /* in this case, we do not need to allocate a new stream object */
      /* since the caller is responsible for closing it himself       */
      وهڪرو         = دليلون->وهڪرو;
      وهڪرو->يادگيري = يادگيري;
      error          = FT_Err_Ok;
    }

#جيڪڏهن_پورو

    وإلا
    {
      error = FT_THROW( Invalid_دليل );
      إذا ( ( دليلون->flags & FT_OPEN_وهڪرو ) && دليلون->وهڪرو )
        FT_وهڪرو_Close( دليلون->وهڪرو );
    }

    إذا ( !error )
      *astream       = وهڪرو;

  Exit:
    أرجع error;
  }


  FT_بنياد_DEF( فراغ )
  FT_وهڪرو_Free( FT_وهڪرو  وهڪرو,
                  FT_Int     external )
  {
    إذا ( وهڪرو )
    {
      FT_يادگيري  يادگيري = وهڪرو->يادگيري;


      FT_وهڪرو_Close( وهڪرو );

      إذا ( !external )
        FT_FREE( وهڪرو );
    }
  }


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#وصف_رد  FT_COMPONENT
#وصف_ڪريو FT_COMPONENT  objs


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
  ft_glyphslot_init( FT_Glyph_جڳهه  جڳهه )
  {
    FT_Driver         driver   = جڳهه->face->driver;
    FT_Driver_Class   clazz    = driver->clazz;
    FT_يادگيري         يادگيري   = driver->پاڙ.يادگيري;
    FT_Error          error    = FT_Err_Ok;
    FT_جڳهه_اندروني  اندروني = عدم;


    جڳهه->library = driver->پاڙ.library;

    إذا ( FT_NEW( اندروني ) )
      اذهب Exit;

    جڳهه->اندروني = اندروني;

    إذا ( FT_DRIVER_استعمال_ڪري_OUTLINES( driver ) )
      error = FT_GlyphLoader_New( يادگيري, &اندروني->loader );

    إذا ( !error && clazz->init_جڳهه )
      error = clazz->init_جڳهه( جڳهه );

#جيڪڏهن_وصف_ٿيل FT_CONFIG_OPTION_SVG
    /* if SVG table exists, allocate the space in `slot->other` */
    إذا ( جڳهه->face->face_flags & FT_FACE_FLAG_SVG )
    {
      FT_SVG_Document  document = عدم;


      إذا ( FT_NEW( document ) )
        اذهب Exit;
      جڳهه->other = document;
    }
#جيڪڏهن_پورو

  Exit:
    أرجع error;
  }


  FT_بنياد_DEF( فراغ )
  ft_glyphslot_free_bitmap( FT_Glyph_جڳهه  جڳهه )
  {
    إذا ( جڳهه->اندروني && ( جڳهه->اندروني->flags & FT_GLYPH_OWN_BITMAP ) )
    {
      FT_يادگيري  يادگيري = FT_FACE_يادگيري( جڳهه->face );


      FT_FREE( جڳهه->bitmap.بفر );
      جڳهه->اندروني->flags &= ~FT_GLYPH_OWN_BITMAP;
    }
    وإلا
    {
      /* assume that the bitmap buffer was stolen or not */
      /* allocated from the heap                         */
      جڳهه->bitmap.بفر = عدم;
    }
  }


  /* overflow-resistant presetting of bitmap position and dimensions; */
  /* also check whether the size is too large for rendering           */
  FT_بنياد_DEF( FT_Bool )
  ft_glyphslot_preset_bitmap( FT_Glyph_جڳهه      جڳهه,
                              FT_Render_گهڻائي    گهڻائي,
                              ثابت FT_ويڪٽر*  origin )
  {
    FT_Outline*  outline = &جڳهه->outline;
    FT_Bitmap*   bitmap  = &جڳهه->bitmap;

    FT_Pixel_گهڻائي  pixel_گهڻائي;

    FT_BBox  cbox, pbox;
    FT_Pos   x_سرڪايو = 0;
    FT_Pos   y_سرڪايو = 0;
    FT_Pos   x_کاٻو, y_چوٽي;
    FT_Pos   ويڪر, اوچائي, pitch;
    FT_Bool  ret;


    إذا ( جڳهه->format == FT_GLYPH_FORMAT_SVG )
    {
      FT_Module    module;
      SVG_Service  svg_service;


      module      = FT_Get_Module( جڳهه->library, "ot-svg" );
      svg_service = (SVG_Service)module->clazz->module_interface;

      أرجع (FT_Bool)svg_service->preset_جڳهه( module, جڳهه, FALSE );
    }
    وإلا إذا ( جڳهه->format != FT_GLYPH_FORMAT_OUTLINE )
      أرجع 1;

    إذا ( origin )
    {
      x_سرڪايو = origin->x;
      y_سرڪايو = origin->y;
    }

    /* compute the control box, and grid-fit it, */
    /* taking into account the origin shift      */
    FT_Outline_Get_CBox( outline, &cbox );

    /* rough estimate of pixel box */
    pbox.xMin = ( cbox.xMin >> 6 ) + ( x_سرڪايو >> 6 );
    pbox.yMin = ( cbox.yMin >> 6 ) + ( y_سرڪايو >> 6 );
    pbox.xMax = ( cbox.xMax >> 6 ) + ( x_سرڪايو >> 6 );
    pbox.yMax = ( cbox.yMax >> 6 ) + ( y_سرڪايو >> 6 );

    /* tiny remainder box */
    cbox.xMin = ( cbox.xMin & 63 ) + ( x_سرڪايو & 63 );
    cbox.yMin = ( cbox.yMin & 63 ) + ( y_سرڪايو & 63 );
    cbox.xMax = ( cbox.xMax & 63 ) + ( x_سرڪايو & 63 );
    cbox.yMax = ( cbox.yMax & 63 ) + ( y_سرڪايو & 63 );

    بدل ( گهڻائي )
    {
    حالة FT_RENDER_گهڻائي_MONO:
      pixel_گهڻائي = FT_PIXEL_گهڻائي_MONO;
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
#جيڪڏهن_پورو

    حالة FT_RENDER_گهڻائي_LCD:
      pixel_گهڻائي = FT_PIXEL_گهڻائي_LCD;
      ft_lcd_padding( &cbox, جڳهه, گهڻائي );
      اذهب Adjust;

    حالة FT_RENDER_گهڻائي_LCD_V:
      pixel_گهڻائي = FT_PIXEL_گهڻائي_LCD_V;
      ft_lcd_padding( &cbox, جڳهه, گهڻائي );
      اذهب Adjust;

    حالة FT_RENDER_گهڻائي_عام:
    حالة FT_RENDER_گهڻائي_LIGHT:
    افتراضي:
      pixel_گهڻائي = FT_PIXEL_گهڻائي_ناسي;
    Adjust:
      pbox.xMin += cbox.xMin >> 6;
      pbox.yMin += cbox.yMin >> 6;
      pbox.xMax += ( cbox.xMax + 63 ) >> 6;
      pbox.yMax += ( cbox.yMax + 63 ) >> 6;
    }

    x_کاٻو = pbox.xMin;
    y_چوٽي  = pbox.yMax;

    ويڪر  = pbox.xMax - pbox.xMin;
    اوچائي = pbox.yMax - pbox.yMin;

    /* Flag the width or height unsuitable for rendering.   */
    /* The limit is based on the ppem value when available. */
    /* FT_Renderer modules should check the return value.   */
    ret = FT_BOOL(     ويڪر >= 0x10000   ||    اوچائي >= 0x10000   ||
                   pbox.xMin < -0x1000000 || pbox.xMax >= 0x1000000 ||
                   pbox.yMin < -0x1000000 || pbox.yMax >= 0x1000000 ||
             ( جڳهه->face                                         &&
               ( ويڪر  > 16 * جڳهه->face->سائيز->metrics.x_ppem ||
                 اوچائي > 16 * جڳهه->face->سائيز->metrics.y_ppem ) ) );

    إذا ( ret )
      FT_TRACE3(( "ft_glyphslot_preset_bitmap: [%ld %ld %ld %ld]\n",
                  pbox.xMin, pbox.yMin, pbox.xMax, pbox.yMax ));

    بدل ( pixel_گهڻائي )
    {
    حالة FT_PIXEL_گهڻائي_MONO:
      pitch = ( ( ويڪر + 15 ) >> 4 ) << 1;
      اكسر;

    حالة FT_PIXEL_گهڻائي_LCD:
      ويڪر *= 3;
      pitch  = FT_PAD_CEIL( ويڪر, 4 );
      اكسر;

    حالة FT_PIXEL_گهڻائي_LCD_V:
      اوچائي *= 3;
      FALL_THROUGH;

    حالة FT_PIXEL_گهڻائي_ناسي:
    افتراضي:
      pitch = ويڪر;
    }

    جڳهه->bitmap_کاٻو = (FT_Int)x_کاٻو;
    جڳهه->bitmap_چوٽي  = (FT_Int)y_چوٽي;

    bitmap->pixel_گهڻائي = (بنانشان حرف)pixel_گهڻائي;
    bitmap->انگ_grays  = 256;
    bitmap->ويڪر      = (بنانشان صحيح)ويڪر;
    bitmap->صفون       = (بنانشان صحيح)اوچائي;
    bitmap->pitch      = pitch;

    أرجع ret;
  }


  FT_بنياد_DEF( فراغ )
  ft_glyphslot_سيٽ_bitmap( FT_Glyph_جڳهه  جڳهه,
                           FT_Byte*      بفر )
  {
    ft_glyphslot_free_bitmap( جڳهه );

    جڳهه->bitmap.بفر = بفر;

    FT_ASSERT( (جڳهه->اندروني->flags & FT_GLYPH_OWN_BITMAP) == 0 );
  }


  FT_بنياد_DEF( FT_Error )
  ft_glyphslot_alloc_bitmap( FT_Glyph_جڳهه  جڳهه )
  {
    FT_يادگيري  يادگيري = FT_FACE_يادگيري( جڳهه->face );
    FT_Error   error;


    إذا ( جڳهه->اندروني->flags & FT_GLYPH_OWN_BITMAP )
      FT_FREE( جڳهه->bitmap.بفر );
    وإلا
      جڳهه->اندروني->flags |= FT_GLYPH_OWN_BITMAP;

    /* dimensions must be preset */
    FT_MEM_ALLOC_MULT( جڳهه->bitmap.بفر,
                       جڳهه->bitmap.صفون,
                       جڳهه->bitmap.pitch );
    أرجع error;
  }


  ساكن فراغ
  ft_glyphslot_clear( FT_Glyph_جڳهه  جڳهه )
  {
    /* free bitmap if needed */
    ft_glyphslot_free_bitmap( جڳهه );

    /* clear all public fields in the glyph slot */
    جڳهه->glyph_اشاريو = 0;

    FT_ZERO( &جڳهه->metrics );
    FT_ZERO( &جڳهه->outline );

    جڳهه->bitmap.ويڪر      = 0;
    جڳهه->bitmap.صفون       = 0;
    جڳهه->bitmap.pitch      = 0;
    جڳهه->bitmap.pixel_گهڻائي = 0;
    /* `slot->bitmap.buffer' has been handled by ft_glyphslot_free_bitmap */

    جڳهه->bitmap_کاٻو   = 0;
    جڳهه->bitmap_چوٽي    = 0;
    جڳهه->انگ_subglyphs = 0;
    جڳهه->subglyphs     = عدم;
    جڳهه->control_معلومات  = عدم;
    جڳهه->control_len   = 0;

#جيڪڏهن_وصف_نه_ٿيل FT_CONFIG_OPTION_SVG
    جڳهه->other = عدم;
#وإلا
    إذا ( !( جڳهه->face->face_flags & FT_FACE_FLAG_SVG ) )
      جڳهه->other = عدم;
    وإلا
    {
      إذا ( جڳهه->اندروني->flags & FT_GLYPH_OWN_GZIP_SVG )
      {
        FT_يادگيري        يادگيري = جڳهه->face->يادگيري;
        FT_SVG_Document  doc    = (FT_SVG_Document)جڳهه->other;


        FT_FREE( doc->svg_document );
        جڳهه->اندروني->flags &= ~FT_GLYPH_OWN_GZIP_SVG;
      }
    }
#جيڪڏهن_پورو

    جڳهه->format = FT_GLYPH_FORMAT_NONE;

    جڳهه->لڪيري_Hori_Advance = 0;
    جڳهه->لڪيري_Vert_Advance = 0;
    جڳهه->advance.x         = 0;
    جڳهه->advance.y         = 0;
    جڳهه->lsb_delta         = 0;
    جڳهه->rsb_delta         = 0;
  }


  ساكن فراغ
  ft_glyphslot_پورو( FT_Glyph_جڳهه  جڳهه )
  {
    FT_Driver        driver = جڳهه->face->driver;
    FT_Driver_Class  clazz  = driver->clazz;
    FT_يادگيري        يادگيري = driver->پاڙ.يادگيري;

#جيڪڏهن_وصف_ٿيل FT_CONFIG_OPTION_SVG
    إذا ( جڳهه->face->face_flags & FT_FACE_FLAG_SVG )
    {
      /* Free memory in case SVG was there.                          */
      /* `slot->internal` might be NULL in out-of-memory situations. */
      إذا ( جڳهه->اندروني && جڳهه->اندروني->flags & FT_GLYPH_OWN_GZIP_SVG )
      {
        FT_SVG_Document  doc = (FT_SVG_Document)جڳهه->other;


        FT_FREE( doc->svg_document );

        جڳهه->اندروني->flags &= ~FT_GLYPH_OWN_GZIP_SVG;
      }

      FT_FREE( جڳهه->other );
    }
#جيڪڏهن_پورو

    إذا ( clazz->پورو_جڳهه )
      clazz->پورو_جڳهه( جڳهه );

    /* free bitmap buffer if needed */
    ft_glyphslot_free_bitmap( جڳهه );

    /* slot->internal might be NULL in out-of-memory situations */
    إذا ( جڳهه->اندروني )
    {
      /* free glyph loader */
      إذا ( FT_DRIVER_استعمال_ڪري_OUTLINES( driver ) )
      {
        FT_Glyph_Loader_پورو( جڳهه->اندروني->loader );
        جڳهه->اندروني->loader = عدم;
      }

      FT_FREE( جڳهه->اندروني );
    }
  }


  /* documentation is in ftobjs.h */

  FT_بنياد_DEF( FT_Error )
  FT_New_Glyph_جڳهه( FT_Face        face,
                    FT_Glyph_جڳهه  *aslot )
  {
    FT_Error         error;
    FT_Driver        driver;
    FT_Driver_Class  clazz;
    FT_يادگيري        يادگيري;
    FT_Glyph_جڳهه     جڳهه = عدم;


    إذا ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );

    إذا ( !face->driver )
      أرجع FT_THROW( Invalid_دليل );

    driver = face->driver;
    clazz  = driver->clazz;
    يادگيري = driver->پاڙ.يادگيري;

    FT_TRACE4(( "FT_New_GlyphSlot: Creating new slot object\n" ));
    إذا ( !FT_ALLOC( جڳهه, clazz->جڳهه_object_سائيز ) )
    {
      جڳهه->face = face;

      error = ft_glyphslot_init( جڳهه );
      إذا ( error )
      {
        ft_glyphslot_پورو( جڳهه );
        FT_FREE( جڳهه );
        اذهب Exit;
      }

      جڳهه->ايندڙ  = face->glyph;
      face->glyph = جڳهه;

      إذا ( aslot )
        *aslot = جڳهه;
    }
    وإلا إذا ( aslot )
      *aslot = عدم;


  Exit:
    FT_TRACE4(( "FT_New_GlyphSlot: Return 0x%x\n", error ));

    أرجع error;
  }


  /* documentation is in ftobjs.h */

  FT_بنياد_DEF( فراغ )
  FT_پورو_Glyph_جڳهه( FT_Glyph_جڳهه  جڳهه )
  {
    إذا ( جڳهه )
    {
      FT_Driver     driver = جڳهه->face->driver;
      FT_يادگيري     يادگيري = driver->پاڙ.يادگيري;
      FT_Glyph_جڳهه  prev;
      FT_Glyph_جڳهه  cur;


      /* Remove slot from its parent face's list */
      prev = عدم;
      cur  = جڳهه->face->glyph;

      طالما ( cur )
      {
        إذا ( cur == جڳهه )
        {
          إذا ( !prev )
            جڳهه->face->glyph = cur->ايندڙ;
          وإلا
            prev->ايندڙ = cur->ايندڙ;

          /* finalize client-specific data */
          إذا ( جڳهه->generic.finalizer )
            جڳهه->generic.finalizer( جڳهه );

          ft_glyphslot_پورو( جڳهه );
          FT_FREE( جڳهه );
          اكسر;
        }
        prev = cur;
        cur  = cur->ايندڙ;
      }
    }
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( فراغ )
  FT_سيٽ_Transform( FT_Face     face,
                    FT_ميٽرڪس*  ميٽرڪس,
                    FT_ويڪٽر*  delta )
  {
    FT_Face_اندروني  اندروني;


    إذا ( !face )
      أرجع;

    اندروني = face->اندروني;

    اندروني->transform_flags = 0;

    إذا ( !ميٽرڪس || !FT_ميٽرڪس_Check( ميٽرڪس ) )
    {
      اندروني->transform_ميٽرڪس.xx = 0x10000L;
      اندروني->transform_ميٽرڪس.xy = 0;
      اندروني->transform_ميٽرڪس.yx = 0;
      اندروني->transform_ميٽرڪس.yy = 0x10000L;

      ميٽرڪس = &اندروني->transform_ميٽرڪس;
    }
    وإلا
      اندروني->transform_ميٽرڪس = *ميٽرڪس;

    /* set transform_flags bit flag 0 if `matrix' isn't the identity */
    إذا ( ( ميٽرڪس->xy | ميٽرڪس->yx ) ||
         ميٽرڪس->xx != 0x10000L      ||
         ميٽرڪس->yy != 0x10000L      )
      اندروني->transform_flags |= 1;

    إذا ( !delta )
    {
      اندروني->transform_delta.x = 0;
      اندروني->transform_delta.y = 0;

      delta = &اندروني->transform_delta;
    }
    وإلا
      اندروني->transform_delta = *delta;

    /* set transform_flags bit flag 1 if `delta' isn't the null vector */
    إذا ( delta->x | delta->y )
      اندروني->transform_flags |= 2;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( فراغ )
  FT_Get_Transform( FT_Face     face,
                    FT_ميٽرڪس*  ميٽرڪس,
                    FT_ويڪٽر*  delta )
  {
    FT_Face_اندروني  اندروني;


    إذا ( !face )
      أرجع;

    اندروني = face->اندروني;

    إذا ( ميٽرڪس )
      *ميٽرڪس = اندروني->transform_ميٽرڪس;

    إذا ( delta )
      *delta = اندروني->transform_delta;
  }


  ساكن FT_Renderer
  ft_lookup_glyph_renderer( FT_Glyph_جڳهه  جڳهه );


#جيڪڏهن_وصف_ٿيل GRID_FIT_METRICS
  ساكن فراغ
  ft_glyphslot_grid_fit_metrics( FT_Glyph_جڳهه  جڳهه,
                                 FT_Bool       vertical )
  {
    FT_Glyph_Metrics*  metrics = &جڳهه->metrics;
    FT_Pos             ساڄو, bottom;


    إذا ( vertical )
    {
      metrics->horiBearingX = FT_PIX_FLOOR( metrics->horiBearingX );
      metrics->horiBearingY = FT_PIX_CEIL_LONG( metrics->horiBearingY );

      ساڄو  = FT_PIX_CEIL_LONG( جوڙيو_LONG( metrics->vertBearingX,
                                           metrics->ويڪر ) );
      bottom = FT_PIX_CEIL_LONG( جوڙيو_LONG( metrics->vertBearingY,
                                           metrics->اوچائي ) );

      metrics->vertBearingX = FT_PIX_FLOOR( metrics->vertBearingX );
      metrics->vertBearingY = FT_PIX_FLOOR( metrics->vertBearingY );

      metrics->ويڪر  = SUB_LONG( ساڄو,
                                  metrics->vertBearingX );
      metrics->اوچائي = SUB_LONG( bottom,
                                  metrics->vertBearingY );
    }
    وإلا
    {
      metrics->vertBearingX = FT_PIX_FLOOR( metrics->vertBearingX );
      metrics->vertBearingY = FT_PIX_FLOOR( metrics->vertBearingY );

      ساڄو  = FT_PIX_CEIL_LONG( جوڙيو_LONG( metrics->horiBearingX,
                                           metrics->ويڪر ) );
      bottom = FT_PIX_FLOOR( SUB_LONG( metrics->horiBearingY,
                                       metrics->اوچائي ) );

      metrics->horiBearingX = FT_PIX_FLOOR( metrics->horiBearingX );
      metrics->horiBearingY = FT_PIX_CEIL_LONG( metrics->horiBearingY );

      metrics->ويڪر  = SUB_LONG( ساڄو,
                                  metrics->horiBearingX );
      metrics->اوچائي = SUB_LONG( metrics->horiBearingY,
                                  bottom );
    }

    metrics->horiAdvance = FT_PIX_ڦيرو_LONG( metrics->horiAdvance );
    metrics->vertAdvance = FT_PIX_ڦيرو_LONG( metrics->vertAdvance );
  }
#جيڪڏهن_پورو /* GRID_FIT_METRICS */


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_ڀريو_Glyph( FT_Face   face,
                 FT_UInt   glyph_اشاريو,
                 FT_Int32  ڀريو_flags )
  {
    FT_Error      error;
    FT_Driver     driver;
    FT_Glyph_جڳهه  جڳهه;
    FT_Library    library;
    FT_Bool       autohint = FALSE;
    FT_Module     hinter;


    إذا ( !face || !face->سائيز || !face->glyph )
      أرجع FT_THROW( Invalid_Face_Handle );

    /* The validity test for `glyph_index' is performed by the */
    /* font drivers.                                           */

    جڳهه = face->glyph;
    ft_glyphslot_clear( جڳهه );

    driver  = face->driver;
    library = driver->پاڙ.library;
    hinter  = library->auto_hinter;

    /* undefined scale means no scale */
    إذا ( face->سائيز->metrics.x_ppem == 0 ||
         face->سائيز->metrics.y_ppem == 0 )
      ڀريو_flags |= FT_ڀريو_NO_SCALE;

    /* resolve load flags dependencies */

    إذا ( ڀريو_flags & FT_ڀريو_NO_RECURSE )
      ڀريو_flags |= FT_ڀريو_NO_SCALE         |
                    FT_ڀريو_IGNORE_TRANSFORM;

    إذا ( ڀريو_flags & FT_ڀريو_NO_SCALE )
    {
      ڀريو_flags |= FT_ڀريو_NO_HINTING |
                    FT_ڀريو_NO_BITMAP;

      ڀريو_flags &= ~FT_ڀريو_RENDER;
    }

    إذا ( ڀريو_flags & FT_ڀريو_BITMAP_METRICS_ONLY )
      ڀريو_flags &= ~FT_ڀريو_RENDER;

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
         !( ڀريو_flags & FT_ڀريو_NO_HINTING )             &&
         !( ڀريو_flags & FT_ڀريو_NO_AUTOHINT )            &&
         FT_IS_SCALABLE( face )                           &&
         !FT_IS_TRICKY( face )                            &&
         ( ( ڀريو_flags & FT_ڀريو_IGNORE_TRANSFORM )    ||
           ( face->اندروني->transform_ميٽرڪس.yx == 0 &&
             face->اندروني->transform_ميٽرڪس.xx != 0 ) ||
           ( face->اندروني->transform_ميٽرڪس.xx == 0 &&
             face->اندروني->transform_ميٽرڪس.yx != 0 ) ) )
    {
      إذا ( ( ڀريو_flags & FT_ڀريو_FORCE_AUTOHINT ) ||
           !FT_DRIVER_HAS_HINTER( driver )         )
        autohint = TRUE;
      وإلا
      {
        FT_Render_گهڻائي  گهڻائي = FT_ڀريو_هدف_گهڻائي( ڀريو_flags );
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
        إذا ( ( گهڻائي == FT_RENDER_گهڻائي_LIGHT           &&
               ( !FT_DRIVER_HINTS_LIGHTLY( driver ) &&
                 !is_light_type1                    ) )         ||
             ( FT_IS_SFNT( face )                             &&
               ttface->انگ_locations                          &&
               ttface->font_پروگرام_سائيز == 0                 &&
               ttface->cvt_پروگرام_سائيز <= 7                  ) )
          autohint = TRUE;
      }
    }

#جيڪڏهن_وصف_ٿيل TT_CONFIG_OPTION_VARC
    /* Never auto-hint a VARC (variable composite) glyph: its components */
    /* are already loaded unhinted, and the auto-fitter cannot process   */
    /* the assembled composite (it also re-enters variation-coordinate   */
    /* setting).  Use the native hinter / unhinted load instead.         */
    إذا ( autohint && FT_IS_SFNT( face ) )
    {
      FT_Service_VARC  varc;


      FT_FACE_ڳوليو_SERVICE( face, varc, VARC );
      إذا ( varc && varc->has_glyph( face, glyph_اشاريو ) )
        autohint = FALSE;
    }
#جيڪڏهن_پورو

    إذا ( autohint )
    {
      FT_AutoHinter_Interface  hinting;


      /* XXX: The use of the `FT_LOAD_XXX_ONLY` flags is not very */
      /*      elegant.                                            */

      /* try to load SVG documents if available */
      إذا ( ( ڀريو_flags & FT_ڀريو_NO_SVG ) == 0 &&
           FT_HAS_SVG( face )                   )
      {
        error = driver->clazz->ڀريو_glyph( جڳهه, face->سائيز,
                                           glyph_اشاريو,
                                           ڀريو_flags | FT_ڀريو_SVG_ONLY );

        إذا ( !error && جڳهه->format == FT_GLYPH_FORMAT_SVG )
          اذهب ڀريو_Ok;
      }

      /* try to load embedded bitmaps if available */
      إذا ( FT_HAS_FIXED_SIZES( face )              &&
           ( ڀريو_flags & FT_ڀريو_NO_BITMAP ) == 0 )
      {
        error = driver->clazz->ڀريو_glyph( جڳهه, face->سائيز,
                                           glyph_اشاريو,
                                           ڀريو_flags | FT_ڀريو_SBITS_ONLY );

        إذا ( !error && جڳهه->format == FT_GLYPH_FORMAT_BITMAP )
          اذهب ڀريو_Ok;
      }

      {
        FT_Face_اندروني  اندروني        = face->اندروني;
        FT_Int            transform_flags = اندروني->transform_flags;


        /* since the auto-hinter calls FT_Load_Glyph by itself, */
        /* make sure that glyphs aren't transformed             */
        اندروني->transform_flags = 0;

        /* load auto-hinted outline */
        hinting = (FT_AutoHinter_Interface)hinter->clazz->module_interface;

        error   = hinting->ڀريو_glyph( (FT_AutoHinter)hinter,
                                       جڳهه, face->سائيز,
                                       glyph_اشاريو, ڀريو_flags );

        اندروني->transform_flags = transform_flags;
      }
    }
    وإلا
    {
      error = driver->clazz->ڀريو_glyph( جڳهه,
                                         face->سائيز,
                                         glyph_اشاريو,
                                         ڀريو_flags );
      إذا ( error )
        اذهب Exit;

      إذا ( جڳهه->format == FT_GLYPH_FORMAT_OUTLINE )
      {
        /* check that the loaded outline is correct */
        error = FT_Outline_Check( &جڳهه->outline );
        إذا ( error )
          اذهب Exit;

#جيڪڏهن_وصف_ٿيل GRID_FIT_METRICS
        إذا ( !( ڀريو_flags & FT_ڀريو_NO_HINTING ) )
          ft_glyphslot_grid_fit_metrics(
            جڳهه,
            FT_BOOL( ڀريو_flags & FT_ڀريو_VERTICAL_LAYOUT ) );
#جيڪڏهن_پورو
      }
    }

  ڀريو_Ok:
    /* compute the advance */
    إذا ( ڀريو_flags & FT_ڀريو_VERTICAL_LAYOUT )
    {
      جڳهه->advance.x = 0;
      جڳهه->advance.y = جڳهه->metrics.vertAdvance;
    }
    وإلا
    {
      جڳهه->advance.x = جڳهه->metrics.horiAdvance;
      جڳهه->advance.y = 0;
    }

    /* compute the linear advance in 16.16 pixels */
    إذا ( ( ڀريو_flags & FT_ڀريو_لڪيري_DESIGN ) == 0 &&
         FT_IS_SCALABLE( face )                      )
    {
      FT_سائيز_Metrics*  metrics = &face->سائيز->metrics;


      /* it's tricky! */
      جڳهه->لڪيري_Hori_Advance = FT_MulDiv( جڳهه->لڪيري_Hori_Advance,
                                           metrics->x_scale, 64 );

      جڳهه->لڪيري_Vert_Advance = FT_MulDiv( جڳهه->لڪيري_Vert_Advance,
                                           metrics->y_scale, 64 );
    }

    إذا ( ( ڀريو_flags & FT_ڀريو_IGNORE_TRANSFORM ) == 0 )
    {
      FT_Face_اندروني  اندروني = face->اندروني;


      /* now, transform the glyph image if needed */
      إذا ( اندروني->transform_flags )
      {
        /* get renderer */
        FT_Renderer  renderer = ft_lookup_glyph_renderer( جڳهه );


        إذا ( renderer )
          error = renderer->clazz->transform_glyph(
                                     renderer, جڳهه,
                                     &اندروني->transform_ميٽرڪس,
                                     &اندروني->transform_delta );
        وإلا إذا ( جڳهه->format == FT_GLYPH_FORMAT_OUTLINE )
        {
          /* apply `standard' transformation if no renderer is available */
          إذا ( اندروني->transform_flags & 1 )
            FT_Outline_Transform( &جڳهه->outline,
                                  &اندروني->transform_ميٽرڪس );

          إذا ( اندروني->transform_flags & 2 )
            FT_Outline_Translate( &جڳهه->outline,
                                  اندروني->transform_delta.x,
                                  اندروني->transform_delta.y );
        }

        /* transform advance */
        FT_ويڪٽر_Transform( &جڳهه->advance, &اندروني->transform_ميٽرڪس );
      }
    }

    جڳهه->glyph_اشاريو          = glyph_اشاريو;
    جڳهه->اندروني->ڀريو_flags = ڀريو_flags;

    /* do we need to render the image or preset the bitmap now? */
    إذا ( !error                                    &&
         ( ڀريو_flags & FT_ڀريو_NO_SCALE ) == 0    &&
         جڳهه->format != FT_GLYPH_FORMAT_BITMAP    &&
         جڳهه->format != FT_GLYPH_FORMAT_مرکب )
    {
      FT_Render_گهڻائي  گهڻائي = FT_ڀريو_هدف_گهڻائي( ڀريو_flags );


      إذا ( گهڻائي == FT_RENDER_گهڻائي_عام   &&
           ڀريو_flags & FT_ڀريو_MONOCHROME )
        گهڻائي = FT_RENDER_گهڻائي_MONO;

      إذا ( ڀريو_flags & FT_ڀريو_RENDER )
        error = FT_Render_Glyph( جڳهه, گهڻائي );
      وإلا
        ft_glyphslot_preset_bitmap( جڳهه, گهڻائي, عدم );
    }

#جيڪڏهن_وصف_ٿيل FT_DEBUG_LEVEL_TRACE
    FT_TRACE5(( "FT_Load_Glyph: index %u, flags 0x%x\n",
                glyph_اشاريو, ڀريو_flags ));
    FT_TRACE5(( "  bitmap %ux%u %s, %s (mode %d)\n",
                جڳهه->bitmap.ويڪر,
                جڳهه->bitmap.صفون,
                جڳهه->outline.points ?
                  جڳهه->bitmap.بفر ? "rendered"
                                      : "preset"
                                     :
                  جڳهه->اندروني->flags & FT_GLYPH_OWN_BITMAP ? "owned"
                                                              : "unowned",
                pixel_modes[جڳهه->bitmap.pixel_گهڻائي],
                جڳهه->bitmap.pixel_گهڻائي ));
    FT_TRACE5(( "\n" ));
    FT_TRACE5(( "  x advance: %f\n", (مزدوج)جڳهه->advance.x / 64 ));
    FT_TRACE5(( "  y advance: %f\n", (مزدوج)جڳهه->advance.y / 64 ));
    FT_TRACE5(( "  linear x advance: %f\n",
                (مزدوج)جڳهه->لڪيري_Hori_Advance / 65536 ));
    FT_TRACE5(( "  linear y advance: %f\n",
                (مزدوج)جڳهه->لڪيري_Vert_Advance / 65536 ));

    {
      FT_Glyph_Metrics*  metrics = &جڳهه->metrics;


      FT_TRACE5(( "  metrics:\n" ));
      FT_TRACE5(( "    width:  %f\n", (مزدوج)metrics->ويڪر / 64 ));
      FT_TRACE5(( "    height: %f\n", (مزدوج)metrics->اوچائي / 64 ));
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
#جيڪڏهن_پورو

  Exit:
    أرجع error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_ڀريو_Char( FT_Face   face,
                FT_ULong  char_ڪوڊ,
                FT_Int32  ڀريو_flags )
  {
    FT_UInt  glyph_اشاريو;


    إذا ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );

    glyph_اشاريو = (FT_UInt)char_ڪوڊ;
    إذا ( face->charmap )
      glyph_اشاريو = FT_Get_Char_اشاريو( face, char_ڪوڊ );

    أرجع FT_ڀريو_Glyph( face, glyph_اشاريو, ڀريو_flags );
  }


  /* destructor for sizes list */
  ساكن فراغ
  destroy_سائيز( FT_يادگيري  يادگيري,
                فراغ*      سائيز_2,
                فراغ*      driver_ )
  {
    FT_سائيز    سائيز   = (FT_سائيز)سائيز_2;
    FT_Driver  driver = (FT_Driver)driver_;


    /* finalize format-specific stuff */
    إذا ( driver->clazz->پورو_سائيز )
      driver->clazz->پورو_سائيز( سائيز );

    /* finalize client-specific data */
    إذا ( سائيز->generic.finalizer )
      سائيز->generic.finalizer( سائيز );

    FT_FREE( سائيز->اندروني );
    FT_FREE( سائيز );
  }


  ساكن فراغ
  ft_cmap_پورو_اندروني( FT_CMap  cmap );


  ساكن فراغ
  destroy_charmaps( FT_Face    face,
                    FT_يادگيري  يادگيري )
  {
    FT_Int  n;


    إذا ( !face )
      أرجع;

    لكل ( n = 0; n < face->انگ_charmaps; n++ )
    {
      FT_CMap  cmap = FT_CMAP( face->charmaps[n] );


      ft_cmap_پورو_اندروني( cmap );

      face->charmaps[n] = عدم;
    }

    FT_FREE( face->charmaps );
    face->انگ_charmaps = 0;
  }


  /* destructor for faces list */
  ساكن فراغ
  destroy_face( FT_يادگيري  يادگيري,
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
      FT_پورو_Glyph_جڳهه( face->glyph );

    /* discard all sizes for this face */
    FT_List_Finalize( &face->sizes_list,
                      destroy_سائيز,
                      يادگيري,
                      driver );
    face->سائيز = عدم;

    /* discard charmaps */
    destroy_charmaps( face, يادگيري );

    /* finalize format-specific stuff */
    إذا ( clazz->پورو_face )
      clazz->پورو_face( face );

    /* close the stream for this face if needed */
    FT_وهڪرو_Free(
      face->وهڪرو,
      ( face->face_flags & FT_FACE_FLAG_EXTERNAL_وهڪرو ) != 0 );

    face->وهڪرو = عدم;

    /* now discard client data */
    إذا ( face->generic.finalizer )
      face->generic.finalizer( face );

    /* get rid of it */
    إذا ( face->اندروني )
    {
      FT_FREE( face->اندروني );
    }
    FT_FREE( face );
  }


  ساكن فراغ
  Destroy_Driver( FT_Driver  driver )
  {
    FT_List_Finalize( &driver->faces_list,
                      destroy_face,
                      driver->پاڙ.يادگيري,
                      driver );
  }


  /* documentation is in ftobjs.h */

  FT_بنياد_DEF( FT_Error )
  ڳوليو_unicode_charmap( FT_Face  face )
  {
    FT_CharMap*  پهريون;
    FT_CharMap*  cur;


    /* caller should have already checked that `face' is valid */
    FT_ASSERT( face );

    پهريون = face->charmaps;

    إذا ( !پهريون )
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

    cur = پهريون + face->انگ_charmaps;  /* points after the last one */

    لكل ( ; --cur >= پهريون; )
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
               cur[0]->encoding_id == TT_APPLE_ID_ڀريل_UNICODE  &&
               FT_Get_CMap_Format( cur[0] ) == 13               ) )
        {
          face->charmap = cur[0];
          أرجع FT_Err_Ok;
        }
      }
    }

    /* We do not have any UCS-4 charmap.                */
    /* Do the loop again and search for UCS-2 charmaps. */
    cur = پهريون + face->انگ_charmaps;

    لكل ( ; --cur >= پهريون; )
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
  ڳوليو_variant_selector_charmap( FT_Face  face )
  {
    FT_CharMap*  پهريون;
    FT_CharMap*  پڇاڙي;
    FT_CharMap*  cur;


    /* caller should have already checked that `face' is valid */
    FT_ASSERT( face );

    پهريون = face->charmaps;

    إذا ( !پهريون )
      أرجع عدم;

    پڇاڙي = پهريون + face->انگ_charmaps;  /* points after the last one */

    لكل ( cur = پهريون; cur < پڇاڙي; cur++ )
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
             FT_وهڪرو      *astream,
             FT_Bool        *anexternal_وهڪرو,
             FT_Long        face_اشاريو,
             FT_Int         انگ_params,
             FT_Parameter*  params,
             FT_Face       *aface )
  {
    FT_يادگيري         يادگيري;
    FT_Driver_Class   clazz;
    FT_Face           face     = عدم;
    FT_Face_اندروني  اندروني = عدم;

    FT_Error          error, error2;


    clazz  = driver->clazz;
    يادگيري = driver->پاڙ.يادگيري;

    /* allocate the face object and perform basic initialization */
    إذا ( FT_ALLOC( face, clazz->face_object_سائيز ) )
      اذهب Fail;

    face->driver = driver;
    face->يادگيري = يادگيري;
    face->وهڪرو = *astream;

    /* set the FT_FACE_FLAG_EXTERNAL_STREAM bit for FT_Done_Face */
    إذا ( *anexternal_وهڪرو )
      face->face_flags |= FT_FACE_FLAG_EXTERNAL_وهڪرو;

    إذا ( FT_NEW( اندروني ) )
      اذهب Fail;

    face->اندروني = اندروني;

#جيڪڏهن_وصف_ٿيل FT_CONFIG_OPTION_INCREMENTAL
    {
      صحيح  i;


      face->اندروني->incremental_interface = عدم;
      لكل ( i = 0; i < انگ_params && !face->اندروني->incremental_interface;
            i++ )
        إذا ( params[i].ٽيگ == FT_PARAM_ٽيگ_INCREMENTAL )
          face->اندروني->incremental_interface =
            (FT_Incremental_Interface)params[i].معلومات;
    }
#جيڪڏهن_پورو

    face->اندروني->random_ٻج = -1;

    إذا ( clazz->init_face )
      error = clazz->init_face( *astream,
                                face,
                                (FT_Int)face_اشاريو,
                                انگ_params,
                                params );
    /* Stream may have been changed. */
    *astream = face->وهڪرو;
    *anexternal_وهڪرو =
      ( face->face_flags & FT_FACE_FLAG_EXTERNAL_وهڪرو ) != 0;
    إذا ( error )
      اذهب Fail;

    /* select Unicode charmap by default */
    error2 = ڳوليو_unicode_charmap( face );

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
      destroy_charmaps( face, يادگيري );
      إذا ( clazz->پورو_face )
        clazz->پورو_face( face );
      FT_FREE( اندروني );
      FT_FREE( face );
      *aface = عدم;
    }

    أرجع error;
  }


  /* there's a Mac-specific extended implementation of FT_New_Face() */
  /* in src/base/ftmac.c                                             */

#جيڪڏهن_وصف_نه_ٿيل FT_MACINTOSH

  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_New_Face( FT_Library   library,
               ثابت حرف*  pathname,
               FT_Long      face_اشاريو,
               FT_Face     *aface )
  {
    FT_Open_دليلون  دليلون;


    /* test for valid `library' and `aface' delayed to `FT_Open_Face' */
    إذا ( !pathname )
      أرجع FT_THROW( Invalid_دليل );

    دليلون.flags    = FT_OPEN_PATHNAME;
    دليلون.pathname = (حرف*)pathname;
    دليلون.وهڪرو   = عدم;

    أرجع ft_open_face_اندروني( library, &دليلون, face_اشاريو, aface, 1 );
  }

#جيڪڏهن_پورو


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_New_يادگيري_Face( FT_Library      library,
                      ثابت FT_Byte*  file_بنياد,
                      FT_Long         file_سائيز,
                      FT_Long         face_اشاريو,
                      FT_Face        *aface )
  {
    FT_Open_دليلون  دليلون;


    /* test for valid `library' and `face' delayed to `FT_Open_Face' */
    إذا ( !file_بنياد )
      أرجع FT_THROW( Invalid_دليل );

    دليلون.flags       = FT_OPEN_يادگيري;
    دليلون.يادگيري_بنياد = file_بنياد;
    دليلون.يادگيري_سائيز = file_سائيز;
    دليلون.وهڪرو      = عدم;

    أرجع ft_open_face_اندروني( library, &دليلون, face_اشاريو, aface, 1 );
  }


#جيڪڏهن_وصف_ٿيل FT_CONFIG_OPTION_MAC_FONTS

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
  يادگيري_وهڪرو_close( FT_وهڪرو  وهڪرو )
  {
    FT_يادگيري  يادگيري = (FT_يادگيري)وهڪرو->descriptor.اشارو;


    FT_FREE( وهڪرو->بنياد );
    وهڪرو->سائيز  = 0;
    وهڪرو->close = عدم;
    FT_FREE( وهڪرو );
  }


  /* Create a new memory stream from a buffer and a size. */
  /* From `ftmac.c'.                                      */
  ساكن FT_Error
  new_يادگيري_وهڪرو( FT_Library           library,
                     FT_Byte*             بنياد,
                     FT_ULong             سائيز,
                     FT_وهڪرو_Close_Func  close,
                     FT_وهڪرو           *astream )
  {
    FT_Error   error;
    FT_يادگيري  يادگيري;
    FT_وهڪرو  وهڪرو = عدم;


    إذا ( !library )
      أرجع FT_THROW( Invalid_Library_Handle );

    إذا ( !بنياد )
      أرجع FT_THROW( Invalid_دليل );

    *astream = عدم;
    يادگيري   = library->يادگيري;
    إذا ( FT_NEW( وهڪرو ) )
      اذهب Exit;

    FT_وهڪرو_Open_يادگيري( وهڪرو, بنياد, سائيز );

    وهڪرو->descriptor.اشارو = يادگيري;
    وهڪرو->close              = close;

    *astream = وهڪرو;

  Exit:
    أرجع error;
  }


  /* Create a new FT_Face given a buffer and a driver name. */
  /* From `ftmac.c'.                                        */
  FT_LOCAL_DEF( FT_Error )
  open_face_from_بفر( FT_Library   library,
                         FT_Byte*     بنياد,
                         FT_ULong     سائيز,
                         FT_Long      face_اشاريو,
                         ثابت حرف*  driver_name,
                         FT_Face     *aface )
  {
    FT_Open_دليلون  دليلون;
    FT_Error      error;
    FT_يادگيري     يادگيري = library->يادگيري;


    دليلون.driver = عدم;
    دليلون.flags  = 0;

    إذا ( driver_name )
    {
      دليلون.driver = FT_Get_Module( library, driver_name );
      إذا ( !دليلون.driver )
      {
        FT_FREE( بنياد );
        أرجع FT_THROW( Missing_Module );
      }

      دليلون.flags = دليلون.flags | FT_OPEN_DRIVER;
    }

    /* `memory_stream_close` also frees the stream object. */
    error = new_يادگيري_وهڪرو( library,
                               بنياد,
                               سائيز,
                               يادگيري_وهڪرو_close,
                               &دليلون.وهڪرو );
    إذا ( error )
    {
      FT_FREE( بنياد );
      أرجع error;
    }

    دليلون.flags |= FT_OPEN_وهڪرو;

#جيڪڏهن_وصف_ٿيل FT_MACINTOSH
    /* At this point, the face index has served its purpose;  */
    /* whoever calls this function has already used it to     */
    /* locate the correct font data.  We should not propagate */
    /* this index to FT_Open_Face() (unless it is negative).  */

    إذا ( face_اشاريو > 0 )
      face_اشاريو &= 0x7FFF0000L; /* retain GX data */
#جيڪڏهن_پورو

    أرجع ft_open_face_اندروني( library, &دليلون, face_اشاريو, aface, 0 );
  }


  /* Look up `TYP1' or `CID ' table from sfnt table directory.       */
  /* `offset' and `length' must exclude the binary header in tables. */

  /* Type 1 and CID-keyed font drivers should recognize sfnt-wrapped */
  /* format too.  Here, since we can't expect that the TrueType font */
  /* driver is loaded unconditionally, we must parse the font by     */
  /* ourselves.  We are only interested in the name of the table and */
  /* the offset.                                                     */

  ساكن FT_Error
  ft_lookup_PS_اندريون_sfnt_وهڪرو( FT_وهڪرو  وهڪرو,
                               FT_Long    face_اشاريو,
                               FT_ULong*  وٿي,
                               FT_ULong*  ڊيگهائي,
                               FT_Bool*   is_sfnt_cid )
  {
    FT_Error   error;
    FT_UShort  انگ_Tables;
    FT_Long    pstable_اشاريو;
    FT_ULong   ٽيگ;
    صحيح        i;


    *وٿي = 0;
    *ڊيگهائي = 0;
    *is_sfnt_cid = FALSE;

    /* TODO: support for sfnt-wrapped PS/CID in TTC format */

    /* version check for 'typ1' (should be ignored?) */
    إذا ( FT_READ_ULONG( ٽيگ ) )
      أرجع error;
    إذا ( ٽيگ != TTAG_typ1 )
      أرجع FT_THROW( Unknown_File_Format );

    إذا ( FT_READ_USHORT( انگ_Tables ) )
      أرجع error;
    إذا ( FT_وهڪرو_SKIP( 2 * 3 ) ) /* skip binary search header */
      أرجع error;

    pstable_اشاريو = -1;
    *is_sfnt_cid  = FALSE;

    لكل ( i = 0; i < انگ_Tables; i++ )
    {
      إذا ( FT_READ_ULONG( ٽيگ )     || FT_وهڪرو_SKIP( 4 )      ||
           FT_READ_ULONG( *وٿي ) || FT_READ_ULONG( *ڊيگهائي ) )
        أرجع error;

      إذا ( ٽيگ == TTAG_CID )
      {
        pstable_اشاريو++;
        *وٿي += 22;
        *ڊيگهائي -= 22;
        *is_sfnt_cid = TRUE;
        إذا ( face_اشاريو < 0 )
          أرجع FT_Err_Ok;
      }
      وإلا إذا ( ٽيگ == TTAG_TYP1 )
      {
        pstable_اشاريو++;
        *وٿي += 24;
        *ڊيگهائي -= 24;
        *is_sfnt_cid = FALSE;
        إذا ( face_اشاريو < 0 )
          أرجع FT_Err_Ok;
      }
      إذا ( face_اشاريو >= 0 && pstable_اشاريو == face_اشاريو )
        أرجع FT_Err_Ok;
    }

    أرجع FT_THROW( جدول_Missing );
  }


  FT_LOCAL_DEF( FT_Error )
  open_face_PS_from_sfnt_وهڪرو( FT_Library     library,
                                 FT_وهڪرو      وهڪرو,
                                 FT_Long        face_اشاريو,
                                 FT_Int         انگ_params,
                                 FT_Parameter  *params,
                                 FT_Face       *aface )
  {
    FT_Error   error;
    FT_يادگيري  يادگيري = library->يادگيري;
    FT_ULong   وٿي, ڊيگهائي;
    FT_ULong   pos;
    FT_Bool    is_sfnt_cid;
    FT_Byte*   sfnt_ps = عدم;

    FT_اڻ_استعمال_ٿيل( انگ_params );
    FT_اڻ_استعمال_ٿيل( params );


    /* ignore GX stuff */
    إذا ( face_اشاريو > 0 )
      face_اشاريو &= 0xFFFFL;

    pos = FT_وهڪرو_POS();

    error = ft_lookup_PS_اندريون_sfnt_وهڪرو( وهڪرو,
                                         face_اشاريو,
                                         &وٿي,
                                         &ڊيگهائي,
                                         &is_sfnt_cid );
    إذا ( error )
      اذهب Exit;

    إذا ( وٿي > وهڪرو->سائيز )
    {
      FT_TRACE2(( "open_face_PS_from_sfnt_stream: invalid table offset\n" ));
      error = FT_THROW( Invalid_جدول );
      اذهب Exit;
    }
    وإلا إذا ( ڊيگهائي > وهڪرو->سائيز - وٿي )
    {
      FT_TRACE2(( "open_face_PS_from_sfnt_stream: invalid table length\n" ));
      error = FT_THROW( Invalid_جدول );
      اذهب Exit;
    }

    error = FT_وهڪرو_Seek( وهڪرو, pos + وٿي );
    إذا ( error )
      اذهب Exit;

    إذا ( FT_QALLOC( sfnt_ps, (FT_Long)ڊيگهائي ) )
      اذهب Exit;

    error = FT_وهڪرو_Read( وهڪرو, (FT_Byte *)sfnt_ps, ڊيگهائي );
    إذا ( error )
    {
      FT_FREE( sfnt_ps );
      اذهب Exit;
    }

    error = open_face_from_بفر( library,
                                   sfnt_ps,
                                   ڊيگهائي,
                                   FT_MIN( face_اشاريو, 0 ),
                                   is_sfnt_cid ? "t1cid" : "type1",
                                   aface );
  Exit:
    {
      FT_Error  error1;


      إذا ( FT_ERR_EQ( error, Unknown_File_Format ) )
      {
        error1 = FT_وهڪرو_Seek( وهڪرو, pos );
        إذا ( error1 )
          أرجع error1;
      }

      أرجع error;
    }
  }


#جيڪڏهن_وصف_نه_ٿيل FT_MACINTOSH

  /* The resource header says we've got resource_cnt `POST' (type1) */
  /* resources in this file.  They all need to be coalesced into    */
  /* one lump which gets passed on to the type1 driver.             */
  /* Here can be only one PostScript font in a file so face_index   */
  /* must be 0 (or -1).                                             */
  /*                                                                */
  ساكن FT_Error
  Mac_Read_POST_Resource( FT_Library  library,
                          FT_وهڪرو   وهڪرو,
                          FT_Long    *offsets,
                          FT_Long     resource_cnt,
                          FT_Long     face_اشاريو,
                          FT_Face    *aface )
  {
    FT_Error   error  = FT_ERR( Cannot_Open_Resource );
    FT_يادگيري  يادگيري = library->يادگيري;

    FT_Byte*   pfb_معلومات = عدم;
    صحيح        i, type, flags;
    FT_ULong   len;
    FT_ULong   pfb_len, pfb_pos, pfb_lenpos;
    FT_ULong   rlen, عارضي;


    إذا ( face_اشاريو == -1 )
      face_اشاريو = 0;
    إذا ( face_اشاريو != 0 )
      أرجع error;

    /* Find the length of all the POST resources, concatenated.  Assume */
    /* worst case (each resource in its own section).                   */
    pfb_len = 0;
    لكل ( i = 0; i < resource_cnt; i++ )
    {
      error = FT_وهڪرو_Seek( وهڪرو, (FT_ULong)offsets[i] );
      إذا ( error )
        اذهب Exit;
      إذا ( FT_READ_ULONG( عارضي ) )  /* actually LONG */
        اذهب Exit;

      /* FT2 allocator takes signed long buffer length,
       * too large value causing overflow should be checked
       */
      FT_TRACE4(( "                 POST fragment #%d: length=0x%08lx"
                  " total pfb_len=0x%08lx\n",
                  i, عارضي, pfb_len + عارضي + 6 ));

      إذا ( FT_MAC_RFORK_MAX_LEN < عارضي               ||
           FT_MAC_RFORK_MAX_LEN - عارضي < pfb_len + 6 )
      {
        FT_TRACE2(( "             MacOS resource length cannot exceed"
                    " 0x%08lx\n",
                    FT_MAC_RFORK_MAX_LEN ));

        error = FT_THROW( Invalid_وٿي );
        اذهب Exit;
      }

      pfb_len += عارضي + 6;
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
      error = FT_وهڪرو_Seek( وهڪرو, (FT_ULong)offsets[i] );
      إذا ( error )
        اذهب Exit2;
      إذا ( FT_READ_ULONG( rlen ) )
        اذهب Exit2;

      /* FT2 allocator takes signed long buffer length,
       * too large fragment length causing overflow should be checked
       */
      إذا ( 0x7FFFFFFFUL < rlen )
      {
        error = FT_THROW( Invalid_وٿي );
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

      error = FT_وهڪرو_Read( وهڪرو, (FT_Byte *)pfb_معلومات + pfb_pos, rlen );
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
                                  face_اشاريو,
                                  "type1",
                                  aface );

  Exit2:
    إذا ( FT_ERR_EQ( error, Array_Too_Large ) )
      FT_TRACE2(( "  Abort due to too-short buffer to store"
                  " all POST fragments\n" ));
    وإلا إذا ( FT_ERR_EQ( error, Invalid_وٿي ) )
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
                          FT_وهڪرو   وهڪرو,
                          FT_Long    *offsets,
                          FT_Long     resource_cnt,
                          FT_Long     face_اشاريو,
                          FT_Face    *aface )
  {
    FT_يادگيري  يادگيري = library->يادگيري;
    FT_Byte*   sfnt_معلومات = عدم;
    FT_Error   error;
    FT_ULong   flag_وٿي;
    FT_ULong   rlen;
    صحيح        is_cff;
    FT_Long    face_اشاريو_اندريون_resource = 0;


    إذا ( face_اشاريو < 0 )
      face_اشاريو = -face_اشاريو - 1;
    إذا ( face_اشاريو >= resource_cnt )
      أرجع FT_THROW( Cannot_Open_Resource );

    flag_وٿي = (FT_ULong)offsets[face_اشاريو];
    error = FT_وهڪرو_Seek( وهڪرو, flag_وٿي );
    إذا ( error )
      اذهب Exit;

    إذا ( FT_READ_ULONG( rlen ) )
      اذهب Exit;
    إذا ( !rlen )
      أرجع FT_THROW( Cannot_Open_Resource );
    إذا ( rlen > FT_MAC_RFORK_MAX_LEN )
      أرجع FT_THROW( Invalid_وٿي );

    error = open_face_PS_from_sfnt_وهڪرو( library,
                                           وهڪرو,
                                           face_اشاريو,
                                           0, عدم,
                                           aface );
    إذا ( !error )
      اذهب Exit;

    /* rewind sfnt stream before open_face_PS_from_sfnt_stream() */
    error = FT_وهڪرو_Seek( وهڪرو, flag_وٿي + 4 );
    إذا ( error )
      اذهب Exit;

    إذا ( FT_QALLOC( sfnt_معلومات, rlen ) )
      أرجع error;
    error = FT_وهڪرو_Read( وهڪرو, (FT_Byte *)sfnt_معلومات, rlen );
    إذا ( error )
    {
      FT_FREE( sfnt_معلومات );
      اذهب Exit;
    }

    is_cff = rlen > 4 && !ft_memcmp( sfnt_معلومات, "OTTO", 4 );
    error = open_face_from_بفر( library,
                                   sfnt_معلومات,
                                   rlen,
                                   face_اشاريو_اندريون_resource,
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
                 FT_وهڪرو   وهڪرو,
                 FT_Long     resource_وٿي,
                 FT_Long     face_اشاريو,
                 FT_Face    *aface )
  {
    FT_يادگيري  يادگيري = library->يادگيري;
    FT_Error   error;
    FT_Long    map_وٿي, rdata_pos;
    FT_Long    *معلومات_offsets;
    FT_Long    ڳڻپ;


    error = FT_Raccess_Get_سرو_Info( library, وهڪرو, resource_وٿي,
                                       &map_وٿي, &rdata_pos );
    إذا ( error )
      أرجع error;

    /* POST resources must be sorted to concatenate properly */
    error = FT_Raccess_Get_معلومات_Offsets( library, وهڪرو,
                                        map_وٿي, rdata_pos,
                                        TTAG_POST, TRUE,
                                        &معلومات_offsets, &ڳڻپ );
    إذا ( !error )
    {
      error = Mac_Read_POST_Resource( library, وهڪرو, معلومات_offsets, ڳڻپ,
                                      face_اشاريو, aface );
      FT_FREE( معلومات_offsets );
      /* POST exists in an LWFN providing a single face */
      إذا ( !error )
        (*aface)->انگ_faces = 1;
      أرجع error;
    }

    /* sfnt resources should not be sorted to preserve the face order by
       QuickDraw API */
    error = FT_Raccess_Get_معلومات_Offsets( library, وهڪرو,
                                        map_وٿي, rdata_pos,
                                        TTAG_sfnt, FALSE,
                                        &معلومات_offsets, &ڳڻپ );
    إذا ( !error )
    {
      FT_Long  face_اشاريو_اندروني = face_اشاريو % ڳڻپ;


      error = Mac_Read_sfnt_Resource( library, وهڪرو, معلومات_offsets, ڳڻپ,
                                      face_اشاريو_اندروني, aface );
      FT_FREE( معلومات_offsets );
      إذا ( !error )
      {
        (*aface)->انگ_faces  = ڳڻپ;
        (*aface)->face_اشاريو = face_اشاريو_اندروني;
      }
    }

    أرجع error;
  }


  /* Check for a valid macbinary header, and if we find one   */
  /* check that the (flattened) resource fork in it is valid. */
  /*                                                          */
  ساكن FT_Error
  Is_Mac_ٻٽو( FT_Library  library,
               FT_وهڪرو   وهڪرو,
               FT_Long     face_اشاريو,
               FT_Face    *aface )
  {
    بنانشان حرف  سرو[128];
    FT_Error       error;
    FT_Long        dlen, وٿي;


    إذا ( !وهڪرو )
      أرجع FT_THROW( Invalid_وهڪرو_Operation );

    error = FT_وهڪرو_Seek( وهڪرو, 0 );
    إذا ( error )
      اذهب Exit;

    error = FT_وهڪرو_Read( وهڪرو, (FT_Byte*)سرو, 128 );
    إذا ( error )
      اذهب Exit;

    إذا (            سرو[ 0] !=   0 ||
                    سرو[74] !=   0 ||
                    سرو[82] !=   0 ||
                    سرو[ 1] ==   0 ||
                    سرو[ 1] >   33 ||
                    سرو[63] !=   0 ||
         سرو[2 + سرو[1]] !=   0 ||
                  سرو[0x53] > 0x7F )
      أرجع FT_THROW( Unknown_File_Format );

    dlen = ( سرو[0x53] << 24 ) |
           ( سرو[0x54] << 16 ) |
           ( سرو[0x55] <<  8 ) |
             سرو[0x56];
#إذا 0
    rlen = ( سرو[0x57] << 24 ) |
           ( سرو[0x58] << 16 ) |
           ( سرو[0x59] <<  8 ) |
             سرو[0x5A];
#جيڪڏهن_پورو /* 0 */
    وٿي = 128 + ( ( dlen + 127 ) & ~127 );

    أرجع IsMacResource( library, وهڪرو, وٿي, face_اشاريو, aface );

  Exit:
    أرجع error;
  }


  ساكن FT_Error
  ڀريو_face_اندريون_embedded_rfork( FT_Library           library,
                               FT_وهڪرو            وهڪرو,
                               FT_Long              face_اشاريو,
                               FT_Face             *aface,
                               ثابت FT_Open_دليلون  *دليلون )
  {

#وصف_رد  FT_COMPONENT
#وصف_ڪريو FT_COMPONENT  raccess

    FT_يادگيري  يادگيري = library->يادگيري;
    FT_Error   error  = FT_ERR( Unknown_File_Format );
    FT_UInt    i;

    حرف*      file_نالا[FT_RACCESS_N_قاعدا];
    FT_Long    offsets[FT_RACCESS_N_قاعدا];
    FT_Error   errors[FT_RACCESS_N_قاعدا];
    FT_Bool    is_darwin_vfs, vfs_rfork_has_no_font = FALSE; /* not tested */

    FT_Open_دليلون  args2;
    FT_وهڪرو     stream2 = عدم;


    FT_Raccess_Guess( library, وهڪرو,
                      دليلون->pathname, file_نالا, offsets, errors );

    لكل ( i = 0; i < FT_RACCESS_N_قاعدا; i++ )
    {
      is_darwin_vfs = ft_raccess_قاعدو_by_darwin_vfs( library, i );
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
      args2.pathname = file_نالا[i] ? file_نالا[i] : دليلون->pathname;

      FT_TRACE3(( "Try rule %u: %s (offset=%ld) ...",
                  i, args2.pathname, offsets[i] ));

      error = FT_وهڪرو_New( library, &args2, &stream2 );
      إذا ( is_darwin_vfs && FT_ERR_EQ( error, Cannot_Open_وهڪرو ) )
        vfs_rfork_has_no_font = TRUE;

      إذا ( error )
      {
        FT_TRACE3(( "failed\n" ));
        استمر;
      }

      error = IsMacResource( library, stream2, offsets[i],
                             face_اشاريو, aface );
      FT_وهڪرو_Free( stream2, 0 );

      FT_TRACE3(( "%s\n", error ? "failed": "successful" ));

      إذا ( !error )
          اكسر;
      وإلا إذا ( is_darwin_vfs )
          vfs_rfork_has_no_font = TRUE;
    }

    لكل (i = 0; i < FT_RACCESS_N_قاعدا; i++)
    {
      إذا ( file_نالا[i] )
        FT_FREE( file_نالا[i] );
    }

    /* Caller (load_mac_face) requires FT_Err_Unknown_File_Format. */
    إذا ( error )
      error = FT_ERR( Unknown_File_Format );

    أرجع error;

#وصف_رد  FT_COMPONENT
#وصف_ڪريو FT_COMPONENT  objs

  }


  /* Check for some macintosh formats without Carbon framework.    */
  /* Is this a macbinary file?  If so look at the resource fork.   */
  /* Is this a mac dfont file?                                     */
  /* Is this an old style resource fork? (in data)                 */
  /* Else call load_face_in_embedded_rfork to try extra rules      */
  /* (defined in `ftrfork.c').                                     */
  /*                                                               */
  ساكن FT_Error
  ڀريو_mac_face( FT_Library           library,
                 FT_وهڪرو            وهڪرو,
                 FT_Long              face_اشاريو,
                 FT_Face             *aface,
                 ثابت FT_Open_دليلون  *دليلون )
  {
    FT_Error error;
    FT_اڻ_استعمال_ٿيل( دليلون );


    error = Is_Mac_ٻٽو( library, وهڪرو, face_اشاريو, aface );
    إذا ( FT_ERR_EQ( error, Unknown_File_Format ) )
    {

#وصف_رد  FT_COMPONENT
#وصف_ڪريو FT_COMPONENT  raccess

#جيڪڏهن_وصف_ٿيل FT_DEBUG_LEVEL_TRACE
      FT_TRACE3(( "Try as dfont: " ));
      إذا ( !( دليلون->flags & FT_OPEN_يادگيري ) )
        FT_TRACE3(( "%s ...", دليلون->pathname ));
#جيڪڏهن_پورو

      error = IsMacResource( library, وهڪرو, 0, face_اشاريو, aface );

      FT_TRACE3(( "%s\n", error ? "failed" : "successful" ));

#وصف_رد  FT_COMPONENT
#وصف_ڪريو FT_COMPONENT  objs

    }

    إذا ( ( FT_ERR_EQ( error, Unknown_File_Format )      ||
           FT_ERR_EQ( error, Invalid_وهڪرو_Operation ) ) &&
         ( دليلون->flags & FT_OPEN_PATHNAME )               )
      error = ڀريو_face_اندريون_embedded_rfork( library, وهڪرو,
                                           face_اشاريو, aface, دليلون );
    أرجع error;
  }
#جيڪڏهن_پورو

#جيڪڏهن_پورو  /* !FT_MACINTOSH && FT_CONFIG_OPTION_MAC_FONTS */


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Open_Face( FT_Library           library,
                ثابت FT_Open_دليلون*  دليلون,
                FT_Long              face_اشاريو,
                FT_Face             *aface )
  {
    أرجع ft_open_face_اندروني( library, دليلون, face_اشاريو, aface, 1 );
  }


  ساكن FT_Error
  ft_open_face_اندروني( FT_Library           library,
                         ثابت FT_Open_دليلون*  دليلون,
                         FT_Long              face_اشاريو,
                         FT_Face             *aface,
                         FT_Bool              test_mac_fonts )
  {
    FT_Error     error;
    FT_Driver    driver = عدم;
    FT_يادگيري    يادگيري = عدم;
    FT_وهڪرو    وهڪرو = عدم;
    FT_Face      face   = عدم;
    FT_List_ڳنڍ  ڳنڍ   = عدم;
    FT_Bool      external_وهڪرو;
    FT_Module*   cur;
    FT_Module*   حد_بندي;

#جيڪڏهن_وصف_نه_ٿيل FT_CONFIG_OPTION_MAC_FONTS
    FT_اڻ_استعمال_ٿيل( test_mac_fonts );
#جيڪڏهن_پورو


    /* only use lower 31 bits together with sign bit */
    إذا ( face_اشاريو > 0 )
      face_اشاريو &= 0x7FFFFFFFL;
    وإلا
    {
      face_اشاريو  = -face_اشاريو;
      face_اشاريو &= 0x7FFFFFFFL;
      face_اشاريو  = -face_اشاريو;
    }

#جيڪڏهن_وصف_ٿيل FT_DEBUG_LEVEL_TRACE
    FT_TRACE3(( "FT_Open_Face: " ));
    إذا ( face_اشاريو < 0 )
      FT_TRACE3(( "Requesting number of faces and named instances\n"));
    وإلا
    {
      FT_TRACE3(( "Requesting face %ld", face_اشاريو & 0xFFFFL ));
      إذا ( face_اشاريو & 0x7FFF0000L )
        FT_TRACE3(( ", named instance %ld", face_اشاريو >> 16 ));
      FT_TRACE3(( "\n" ));
    }
#جيڪڏهن_پورو

    /* test for valid `library' delayed to `FT_Stream_New' */

    إذا ( !دليلون )
      أرجع FT_THROW( Invalid_دليل );

    external_وهڪرو = FT_BOOL( ( دليلون->flags & FT_OPEN_وهڪرو ) &&
                               دليلون->وهڪرو                     );

    /* create input stream */
    error = FT_وهڪرو_New( library, دليلون, &وهڪرو );
    إذا ( error )
      اذهب Fail3;

    /* Do this error check after `FT_Stream_New` to ensure that the */
    /* 'close' callback is called.                                  */
    إذا ( !aface && face_اشاريو >= 0 )
    {
      error = FT_THROW( Invalid_دليل );
      اذهب Fail3;
    }

    يادگيري = library->يادگيري;

    /* If the font driver is specified in the `args' structure, use */
    /* it.  Otherwise, we scan the list of registered drivers.      */
    إذا ( ( دليلون->flags & FT_OPEN_DRIVER ) && دليلون->driver )
    {
      driver = FT_DRIVER( دليلون->driver );

      /* not all modules are drivers, so check... */
      إذا ( FT_MODULE_IS_DRIVER( driver ) )
      {
        FT_Int         انگ_params = 0;
        FT_Parameter*  params     = عدم;


        إذا ( دليلون->flags & FT_OPEN_PARAMS )
        {
          انگ_params = دليلون->انگ_params;
          params     = دليلون->params;
        }

        error = open_face( driver, &وهڪرو, &external_وهڪرو, face_اشاريو,
                           انگ_params, params, &face );
        إذا ( !error )
          اذهب Success;
      }
      وإلا
        error = FT_THROW( Invalid_Handle );

      FT_وهڪرو_Free( وهڪرو, external_وهڪرو );
      اذهب Fail;
    }
    وإلا
    {
      error = FT_ERR( Missing_Module );

      /* check each font driver for an appropriate format */
      cur   = library->modules;
      حد_بندي = cur + library->انگ_modules;

      لكل ( ; cur < حد_بندي; cur++ )
      {
        /* not all modules are font drivers, so check... */
        إذا ( FT_MODULE_IS_DRIVER( cur[0] ) )
        {
          FT_Int         انگ_params = 0;
          FT_Parameter*  params     = عدم;


          driver = FT_DRIVER( cur[0] );

          إذا ( دليلون->flags & FT_OPEN_PARAMS )
          {
            انگ_params = دليلون->انگ_params;
            params     = دليلون->params;
          }

          error = open_face( driver, &وهڪرو, &external_وهڪرو, face_اشاريو,
                             انگ_params, params, &face );
          إذا ( !error )
            اذهب Success;

#جيڪڏهن_وصف_ٿيل FT_CONFIG_OPTION_MAC_FONTS
          إذا ( test_mac_fonts                                           &&
               ft_strcmp( cur[0]->clazz->module_name, "truetype" ) == 0 &&
               FT_ERR_EQ( error, جدول_Missing )                        )
          {
            /* TrueType but essential tables are missing */
            error = FT_وهڪرو_Seek( وهڪرو, 0 );
            إذا ( error )
              اكسر;

            error = open_face_PS_from_sfnt_وهڪرو( library,
                                                   وهڪرو,
                                                   face_اشاريو,
                                                   انگ_params,
                                                   params,
                                                   aface );
            إذا ( !error )
            {
              FT_وهڪرو_Free( وهڪرو, external_وهڪرو );
              أرجع error;
            }
          }
#جيڪڏهن_پورو

          إذا ( FT_ERR_NEQ( error, Unknown_File_Format ) )
            اذهب Fail3;
        }
      }

    Fail3:
      /* If we are on the mac, and we get an                          */
      /* FT_Err_Invalid_Stream_Operation it may be because we have an */
      /* empty data fork, so we need to check the resource fork.      */
      إذا ( FT_ERR_NEQ( error, Cannot_Open_وهڪرو )       &&
           FT_ERR_NEQ( error, Unknown_File_Format )      &&
           FT_ERR_NEQ( error, Invalid_وهڪرو_Operation ) )
        اذهب Fail2;

#إذا !defined( FT_MACINTOSH ) && defined( FT_CONFIG_OPTION_MAC_FONTS )
      إذا ( test_mac_fonts )
      {
        error = ڀريو_mac_face( library, وهڪرو, face_اشاريو, aface, دليلون );
        إذا ( !error )
        {
          /* We don't want to go to Success here.  We've already done   */
          /* that.  On the other hand, if we succeeded we still need to */
          /* close this stream (we opened a different stream which      */
          /* extracted the interesting information out of this stream   */
          /* here.  That stream will still be open and the face will    */
          /* point to it).                                              */
          FT_وهڪرو_Free( وهڪرو, external_وهڪرو );
          أرجع error;
        }
      }

      إذا ( FT_ERR_NEQ( error, Unknown_File_Format ) )
        اذهب Fail2;
#جيڪڏهن_پورو  /* !FT_MACINTOSH && FT_CONFIG_OPTION_MAC_FONTS */

      /* no driver is able to handle this format */
      error = FT_THROW( Unknown_File_Format );

  Fail2:
      FT_وهڪرو_Free( وهڪرو, external_وهڪرو );
      اذهب Fail;
    }

  Success:
    FT_TRACE4(( "FT_Open_Face: New face object, adding to list\n" ));

    /* add the face object to its driver's list */
    إذا ( FT_QNEW( ڳنڍ ) )
      اذهب Fail;

    ڳنڍ->معلومات = face;
    /* don't assume driver is the same as face->driver, so use */
    /* face->driver instead.                                   */
    FT_List_جوڙيو( &face->driver->faces_list, ڳنڍ );

    /* now allocate a glyph slot object for the face */
    FT_TRACE4(( "FT_Open_Face: Creating glyph slot\n" ));

    إذا ( face_اشاريو >= 0 )
    {
      error = FT_New_Glyph_جڳهه( face, عدم );
      إذا ( error )
        اذهب Fail;

      /* finally, allocate a size object for the face */
      {
        FT_سائيز  سائيز;


        FT_TRACE4(( "FT_Open_Face: Creating size object\n" ));

        error = FT_New_سائيز( face, &سائيز );
        إذا ( error )
          اذهب Fail;

        face->سائيز = سائيز;
      }
    }

    /* some checks */

    إذا ( FT_IS_SCALABLE( face ) )
    {
      إذا ( face->اوچائي < 0 )
        face->اوچائي = (FT_Short)-face->اوچائي;

      إذا ( !FT_HAS_VERTICAL( face ) )
        face->max_advance_اوچائي = (FT_Short)face->اوچائي;
    }

    إذا ( FT_HAS_FIXED_SIZES( face ) )
    {
      FT_Int  i;


      لكل ( i = 0; i < face->انگ_fixed_sizes; i++ )
      {
        FT_Bitmap_سائيز*  bsize = face->موجود_sizes + i;


        إذا ( bsize->اوچائي < 0 )
          bsize->اوچائي = -bsize->اوچائي;
        إذا ( bsize->x_ppem < 0 )
          bsize->x_ppem = -bsize->x_ppem;
        إذا ( bsize->y_ppem < 0 )
          bsize->y_ppem = -bsize->y_ppem;

        /* check whether negation actually has worked */
        إذا ( bsize->اوچائي < 0 || bsize->x_ppem < 0 || bsize->y_ppem < 0 )
        {
          FT_TRACE0(( "FT_Open_Face:"
                      " Invalid bitmap dimensions for strike %d,"
                      " now disabled\n", i ));
          bsize->ويڪر  = 0;
          bsize->اوچائي = 0;
          bsize->سائيز   = 0;
          bsize->x_ppem = 0;
          bsize->y_ppem = 0;
        }
      }
    }

    /* initialize internal face data */
    {
      FT_Face_اندروني  اندروني = face->اندروني;


      اندروني->transform_ميٽرڪس.xx = 0x10000L;
      اندروني->transform_ميٽرڪس.xy = 0;
      اندروني->transform_ميٽرڪس.yx = 0;
      اندروني->transform_ميٽرڪس.yy = 0x10000L;

      اندروني->transform_delta.x = 0;
      اندروني->transform_delta.y = 0;

      اندروني->refcount = 1;

      اندروني->no_stem_darkening = -1;
    }

    إذا ( aface )
      *aface = face;
    وإلا
      FT_پورو_Face( face );

    اذهب Exit;

  Fail:
    إذا ( ڳنڍ )
      FT_پورو_Face( face );    /* face must be in the driver's list */
    وإلا إذا ( face )
      destroy_face( يادگيري, face, driver );

  Exit:
#جيڪڏهن_وصف_ٿيل FT_DEBUG_LEVEL_TRACE
    إذا ( !error && face_اشاريو < 0 )
    {
      FT_TRACE3(( "FT_Open_Face: The font has %ld face%s\n",
                  face->انگ_faces,
                  face->انگ_faces == 1 ? "" : "s" ));
      FT_TRACE3(( "              and %ld named instance%s for face %ld\n",
                  face->style_flags >> 16,
                  ( face->style_flags >> 16 ) == 1 ? "" : "s",
                  -face_اشاريو - 1 ));
    }
#جيڪڏهن_پورو

    FT_TRACE4(( "FT_Open_Face: Return 0x%x\n", error ));

    أرجع error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Attach_File( FT_Face      face,
                  ثابت حرف*  filepathname )
  {
    FT_Open_دليلون  open;


    /* test for valid `face' delayed to `FT_Attach_Stream' */

    إذا ( !filepathname )
      أرجع FT_THROW( Invalid_دليل );

    open.وهڪرو   = عدم;
    open.flags    = FT_OPEN_PATHNAME;
    open.pathname = (حرف*)filepathname;

    أرجع FT_Attach_وهڪرو( face, &open );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Attach_وهڪرو( FT_Face              face,
                    ثابت FT_Open_دليلون*  parameters )
  {
    FT_وهڪرو  وهڪرو;
    FT_Error   error;
    FT_Driver  driver;

    FT_Driver_Class  clazz;


    /* test for valid `parameters' delayed to `FT_Stream_New' */

    إذا ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );

    driver = face->driver;
    إذا ( !driver )
      أرجع FT_THROW( Invalid_Driver_Handle );

    error = FT_وهڪرو_New( driver->پاڙ.library, parameters, &وهڪرو );
    إذا ( error )
      اذهب Exit;

    /* we implement FT_Attach_Stream in each driver through the */
    /* `attach_file' interface                                  */

    error = FT_ERR( Unimplemented_Feature );
    clazz = driver->clazz;
    إذا ( clazz->attach_file )
      error = clazz->attach_file( face, وهڪرو );

    /* close the attached stream */
    FT_وهڪرو_Free( وهڪرو,
                    FT_BOOL( parameters->وهڪرو                     &&
                             ( parameters->flags & FT_OPEN_وهڪرو ) ) );

  Exit:
    أرجع error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Reference_Face( FT_Face  face )
  {
    إذا ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );

    face->اندروني->refcount++;

    أرجع FT_Err_Ok;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_پورو_Face( FT_Face  face )
  {
    FT_Error     error;
    FT_Driver    driver;
    FT_يادگيري    يادگيري;
    FT_List_ڳنڍ  ڳنڍ;


    error = FT_ERR( Invalid_Face_Handle );
    إذا ( face && face->driver )
    {
      face->اندروني->refcount--;
      إذا ( face->اندروني->refcount > 0 )
        error = FT_Err_Ok;
      وإلا
      {
        driver = face->driver;
        يادگيري = driver->پاڙ.يادگيري;

        /* find face in driver's list */
        ڳنڍ = FT_List_ڳوليو( &driver->faces_list, face );
        إذا ( ڳنڍ )
        {
          /* remove face object from the driver's list */
          FT_List_Remove( &driver->faces_list, ڳنڍ );
          FT_FREE( ڳنڍ );

          /* now destroy the object proper */
          destroy_face( يادگيري, face, driver );
          error = FT_Err_Ok;
        }
      }
    }

    أرجع error;
  }


  /* documentation is in ftobjs.h */

  FT_EXPORT_DEF( FT_Error )
  FT_New_سائيز( FT_Face   face,
               FT_سائيز  *asize )
  {
    FT_Error         error;
    FT_يادگيري        يادگيري;
    FT_Driver        driver;
    FT_Driver_Class  clazz;

    FT_سائيز          سائيز = عدم;
    FT_List_ڳنڍ      ڳنڍ = عدم;

    FT_سائيز_اندروني  اندروني = عدم;


    إذا ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );

    إذا ( !asize )
      أرجع FT_THROW( Invalid_دليل );

    إذا ( !face->driver )
      أرجع FT_THROW( Invalid_Driver_Handle );

    *asize = عدم;

    driver = face->driver;
    clazz  = driver->clazz;
    يادگيري = face->يادگيري;

    /* Allocate new size object and perform basic initialisation */
    إذا ( FT_ALLOC( سائيز, clazz->سائيز_object_سائيز ) || FT_QNEW( ڳنڍ ) )
      اذهب Exit;

    سائيز->face = face;

    إذا ( FT_NEW( اندروني ) )
      اذهب Exit;

    سائيز->اندروني = اندروني;

    إذا ( clazz->init_سائيز )
      error = clazz->init_سائيز( سائيز );

    /* in case of success, add to the face's list */
    إذا ( !error )
    {
      *asize     = سائيز;
      ڳنڍ->معلومات = سائيز;
      FT_List_جوڙيو( &face->sizes_list, ڳنڍ );
    }

  Exit:
    إذا ( error )
    {
      FT_FREE( ڳنڍ );
      إذا ( سائيز )
        FT_FREE( سائيز->اندروني );
      FT_FREE( سائيز );
    }

    أرجع error;
  }


  /* documentation is in ftobjs.h */

  FT_EXPORT_DEF( FT_Error )
  FT_پورو_سائيز( FT_سائيز  سائيز )
  {
    FT_Error     error;
    FT_Driver    driver;
    FT_يادگيري    يادگيري;
    FT_Face      face;
    FT_List_ڳنڍ  ڳنڍ;


    إذا ( !سائيز )
      أرجع FT_THROW( Invalid_سائيز_Handle );

    face = سائيز->face;
    إذا ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );

    driver = face->driver;
    إذا ( !driver )
      أرجع FT_THROW( Invalid_Driver_Handle );

    يادگيري = driver->پاڙ.يادگيري;

    error = FT_Err_Ok;
    ڳنڍ  = FT_List_ڳوليو( &face->sizes_list, سائيز );
    إذا ( ڳنڍ )
    {
      FT_List_Remove( &face->sizes_list, ڳنڍ );
      FT_FREE( ڳنڍ );

      إذا ( face->سائيز == سائيز )
      {
        face->سائيز = عدم;
        إذا ( face->sizes_list.سر )
          face->سائيز = (FT_سائيز)(face->sizes_list.سر->معلومات);
      }

      destroy_سائيز( يادگيري, سائيز, driver );
    }
    وإلا
      error = FT_THROW( Invalid_سائيز_Handle );

    أرجع error;
  }


  /* documentation is in ftobjs.h */

  FT_بنياد_DEF( FT_Error )
  FT_ميلاپ_سائيز( FT_Face          face,
                 FT_سائيز_Request  req,
                 FT_Bool          ignore_ويڪر,
                 FT_ULong*        سائيز_اشاريو )
  {
    FT_Int   i;
    FT_Long  w, h;


    إذا ( !FT_HAS_FIXED_SIZES( face ) )
      أرجع FT_THROW( Invalid_Face_Handle );

    /* FT_Bitmap_Size doesn't provide enough info... */
    إذا ( req->type != FT_سائيز_REQUEST_TYPE_NOMINAL )
      أرجع FT_THROW( Unimplemented_Feature );

    w = FT_REQUEST_ويڪر ( req );
    h = FT_REQUEST_اوچائي( req );

    إذا ( req->ويڪر && !req->اوچائي )
      h = w;
    وإلا إذا ( !req->ويڪر && req->اوچائي )
      w = h;

    w = FT_PIX_ڦيرو( w );
    h = FT_PIX_ڦيرو( h );

    إذا ( !w || !h )
      أرجع FT_THROW( Invalid_Pixel_سائيز );

    لكل ( i = 0; i < face->انگ_fixed_sizes; i++ )
    {
      FT_Bitmap_سائيز*  bsize = face->موجود_sizes + i;


      إذا ( h != FT_PIX_ڦيرو( bsize->y_ppem ) )
        استمر;

      إذا ( w == FT_PIX_ڦيرو( bsize->x_ppem ) || ignore_ويڪر )
      {
        FT_TRACE3(( "FT_Match_Size: bitmap strike %d matches\n", i ));

        إذا ( سائيز_اشاريو )
          *سائيز_اشاريو = (FT_ULong)i;

        أرجع FT_Err_Ok;
      }
    }

    FT_TRACE3(( "FT_Match_Size: no matching bitmap strike\n" ));

    أرجع FT_THROW( Invalid_Pixel_سائيز );
  }


  /* documentation is in ftobjs.h */

  FT_بنياد_DEF( فراغ )
  ft_synthesize_vertical_metrics( FT_Glyph_Metrics*  metrics,
                                  FT_Pos             advance )
  {
    FT_Pos  اوچائي = metrics->اوچائي;


    /* compensate for glyph with bbox above/below the baseline */
    إذا ( metrics->horiBearingY < 0 )
    {
      إذا ( اوچائي < metrics->horiBearingY )
        اوچائي = metrics->horiBearingY;
    }
    وإلا إذا ( metrics->horiBearingY > 0 )
      اوچائي -= metrics->horiBearingY;

    /* the factor 1.2 is a heuristical value */
    إذا ( !advance )
      advance = اوچائي * 12 / 10;

    metrics->vertBearingX = metrics->horiBearingX - metrics->horiAdvance / 2;
    metrics->vertBearingY = ( advance - اوچائي ) / 2;
    metrics->vertAdvance  = advance;
  }


  ساكن فراغ
  ft_recompute_scaled_metrics( FT_Face           face,
                               FT_سائيز_Metrics*  metrics )
  {
    /* Compute root ascender, descender, test height, and max_advance */

#جيڪڏهن_وصف_ٿيل GRID_FIT_METRICS
    metrics->ascender    = FT_PIX_CEIL( FT_MulFix( face->ascender,
                                                   metrics->y_scale ) );

    metrics->descender   = FT_PIX_FLOOR( FT_MulFix( face->descender,
                                                    metrics->y_scale ) );

    metrics->اوچائي      = FT_PIX_ڦيرو( FT_MulFix( face->اوچائي,
                                                    metrics->y_scale ) );

    metrics->max_advance = FT_PIX_ڦيرو( FT_MulFix( face->max_advance_ويڪر,
                                                    metrics->x_scale ) );
#وإلا /* !GRID_FIT_METRICS */
    metrics->ascender    = FT_MulFix( face->ascender,
                                      metrics->y_scale );

    metrics->descender   = FT_MulFix( face->descender,
                                      metrics->y_scale );

    metrics->اوچائي      = FT_MulFix( face->اوچائي,
                                      metrics->y_scale );

    metrics->max_advance = FT_MulFix( face->max_advance_ويڪر,
                                      metrics->x_scale );
#جيڪڏهن_پورو /* !GRID_FIT_METRICS */
  }


  FT_بنياد_DEF( فراغ )
  FT_Select_Metrics( FT_Face   face,
                     FT_ULong  strike_اشاريو )
  {
    FT_سائيز_Metrics*  metrics;
    FT_Bitmap_سائيز*   bsize;


    metrics = &face->سائيز->metrics;
    bsize   = face->موجود_sizes + strike_اشاريو;

    metrics->x_ppem = (FT_UShort)( ( bsize->x_ppem + 32 ) >> 6 );
    metrics->y_ppem = (FT_UShort)( ( bsize->y_ppem + 32 ) >> 6 );

    إذا ( FT_IS_SCALABLE( face ) )
    {
      metrics->x_scale = FT_DivFix( bsize->x_ppem,
                                    face->units_في_EM );
      metrics->y_scale = FT_DivFix( bsize->y_ppem,
                                    face->units_في_EM );

      ft_recompute_scaled_metrics( face, metrics );
    }
    وإلا
    {
      metrics->x_scale     = 1L << 16;
      metrics->y_scale     = 1L << 16;
      metrics->ascender    = bsize->y_ppem;
      metrics->descender   = 0;
      metrics->اوچائي      = bsize->اوچائي << 6;
      metrics->max_advance = bsize->x_ppem;
    }
  }


  FT_بنياد_DEF( FT_Error )
  FT_Request_Metrics( FT_Face          face,
                      FT_سائيز_Request  req )
  {
    FT_Error  error = FT_Err_Ok;

    FT_سائيز_Metrics*  metrics;


    metrics = &face->سائيز->metrics;

    إذا ( FT_IS_SCALABLE( face ) )
    {
      FT_Long  w = 0, h = 0, scaled_w = 0, scaled_h = 0;


      بدل ( req->type )
      {
      حالة FT_سائيز_REQUEST_TYPE_NOMINAL:
        w = h = face->units_في_EM;
        اكسر;

      حالة FT_سائيز_REQUEST_TYPE_REAL_DIM:
        w = h = face->ascender - face->descender;
        اكسر;

      حالة FT_سائيز_REQUEST_TYPE_BBOX:
        w = face->bbox.xMax - face->bbox.xMin;
        h = face->bbox.yMax - face->bbox.yMin;
        اكسر;

      حالة FT_سائيز_REQUEST_TYPE_CELL:
        w = face->max_advance_ويڪر;
        h = face->ascender - face->descender;
        اكسر;

      حالة FT_سائيز_REQUEST_TYPE_SCALES:
        metrics->x_scale = (FT_Fixed)req->ويڪر;
        metrics->y_scale = (FT_Fixed)req->اوچائي;
        إذا ( !metrics->x_scale )
          metrics->x_scale = metrics->y_scale;
        وإلا إذا ( !metrics->y_scale )
          metrics->y_scale = metrics->x_scale;
        اذهب Calculate_Ppem;

      حالة FT_سائيز_REQUEST_TYPE_MAX:
        اكسر;
      }

      /* to be on the safe side */
      إذا ( w < 0 )
        w = -w;

      إذا ( h < 0 )
        h = -h;

      scaled_w = FT_REQUEST_ويڪر ( req );
      scaled_h = FT_REQUEST_اوچائي( req );

      /* determine scales */
      إذا ( req->اوچائي || !req->ويڪر )
      {
        إذا ( h == 0 )
        {
          FT_ERROR(( "FT_Request_Metrics: Divide by zero\n" ));
          error = FT_ERR( Divide_By_Zero );
          اذهب Exit;
        }

        metrics->y_scale = FT_DivFix( scaled_h, h );
      }

      إذا ( req->ويڪر )
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

      إذا ( !req->اوچائي )
      {
        metrics->y_scale = metrics->x_scale;
        scaled_h = FT_MulDiv( scaled_w, h, w );
      }

      إذا ( req->type == FT_سائيز_REQUEST_TYPE_CELL )
      {
        إذا ( metrics->y_scale > metrics->x_scale )
          metrics->y_scale = metrics->x_scale;
        وإلا
          metrics->x_scale = metrics->y_scale;
      }

  Calculate_Ppem:
      /* calculate the ppems */
      إذا ( req->type != FT_سائيز_REQUEST_TYPE_NOMINAL )
      {
        scaled_w = FT_MulFix( face->units_في_EM, metrics->x_scale );
        scaled_h = FT_MulFix( face->units_في_EM, metrics->y_scale );
      }

      scaled_w = ( scaled_w + 32 ) >> 6;
      scaled_h = ( scaled_h + 32 ) >> 6;
      إذا ( scaled_w > (FT_Long)FT_USHORT_MAX ||
           scaled_h > (FT_Long)FT_USHORT_MAX )
      {
        FT_ERROR(( "FT_Request_Metrics: Resulting ppem size too large\n" ));
        error = FT_ERR( Invalid_Pixel_سائيز );
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
  FT_Select_سائيز( FT_Face  face,
                  FT_Int   strike_اشاريو )
  {
    FT_Error         error = FT_Err_Ok;
    FT_Driver_Class  clazz;


    إذا ( !face || !FT_HAS_FIXED_SIZES( face ) )
      أرجع FT_THROW( Invalid_Face_Handle );

    إذا ( strike_اشاريو < 0 || strike_اشاريو >= face->انگ_fixed_sizes )
      أرجع FT_THROW( Invalid_دليل );

    clazz = face->driver->clazz;

    إذا ( clazz->select_سائيز )
    {
      error = clazz->select_سائيز( face->سائيز, (FT_ULong)strike_اشاريو );

      FT_TRACE5(( "FT_Select_Size (%s driver):\n",
                  face->driver->پاڙ.clazz->module_name ));
    }
    وإلا
    {
      FT_Select_Metrics( face, (FT_ULong)strike_اشاريو );

      FT_TRACE5(( "FT_Select_Size:\n" ));
    }

#جيڪڏهن_وصف_ٿيل FT_DEBUG_LEVEL_TRACE
    {
      FT_سائيز_Metrics*  metrics = &face->سائيز->metrics;


      FT_TRACE5(( "  x scale: %ld (%f)\n",
                  metrics->x_scale, (مزدوج)metrics->x_scale / 65536 ));
      FT_TRACE5(( "  y scale: %ld (%f)\n",
                  metrics->y_scale, (مزدوج)metrics->y_scale / 65536 ));
      FT_TRACE5(( "  ascender: %f\n",
                  (مزدوج)metrics->ascender / 64 ));
      FT_TRACE5(( "  descender: %f\n",
                  (مزدوج)metrics->descender / 64 ));
      FT_TRACE5(( "  height: %f\n",
                  (مزدوج)metrics->اوچائي / 64 ));
      FT_TRACE5(( "  max advance: %f\n",
                  (مزدوج)metrics->max_advance / 64 ));
      FT_TRACE5(( "  x ppem: %d\n", metrics->x_ppem ));
      FT_TRACE5(( "  y ppem: %d\n", metrics->y_ppem ));
    }
#جيڪڏهن_پورو

    أرجع error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Request_سائيز( FT_Face          face,
                   FT_سائيز_Request  req )
  {
    FT_Error         error;
    FT_Driver_Class  clazz;
    FT_ULong         strike_اشاريو;


    إذا ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );

    إذا ( !face->سائيز )
      أرجع FT_THROW( Invalid_سائيز_Handle );

    إذا ( !req || req->ويڪر < 0 || req->اوچائي < 0 ||
         req->type >= FT_سائيز_REQUEST_TYPE_MAX )
      أرجع FT_THROW( Invalid_دليل );

    /* signal the auto-hinter to recompute its size metrics */
    /* (if requested)                                       */
    face->سائيز->اندروني->autohint_metrics.x_scale = 0;

    clazz = face->driver->clazz;

    إذا ( clazz->request_سائيز )
    {
      error = clazz->request_سائيز( face->سائيز, req );

      FT_TRACE5(( "FT_Request_Size (%s driver):\n",
                  face->driver->پاڙ.clazz->module_name ));
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
      error = FT_ميلاپ_سائيز( face, req, 0, &strike_اشاريو );
      إذا ( error )
        اذهب Exit;

      أرجع FT_Select_سائيز( face, (FT_Int)strike_اشاريو );
    }
    وإلا
    {
      error = FT_Request_Metrics( face, req );
      إذا ( error )
        اذهب Exit;

      FT_TRACE5(( "FT_Request_Size:\n" ));
    }

#جيڪڏهن_وصف_ٿيل FT_DEBUG_LEVEL_TRACE
    {
      FT_سائيز_Metrics*  metrics = &face->سائيز->metrics;


      FT_TRACE5(( "  x scale: %ld (%f)\n",
                  metrics->x_scale, (مزدوج)metrics->x_scale / 65536 ));
      FT_TRACE5(( "  y scale: %ld (%f)\n",
                  metrics->y_scale, (مزدوج)metrics->y_scale / 65536 ));
      FT_TRACE5(( "  ascender: %f\n",
                  (مزدوج)metrics->ascender / 64 ));
      FT_TRACE5(( "  descender: %f\n",
                  (مزدوج)metrics->descender / 64 ));
      FT_TRACE5(( "  height: %f\n",
                  (مزدوج)metrics->اوچائي / 64 ));
      FT_TRACE5(( "  max advance: %f\n",
                  (مزدوج)metrics->max_advance / 64 ));
      FT_TRACE5(( "  x ppem: %d\n", metrics->x_ppem ));
      FT_TRACE5(( "  y ppem: %d\n", metrics->y_ppem ));
    }
#جيڪڏهن_پورو

  Exit:
    أرجع error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_سيٽ_Char_سائيز( FT_Face     face,
                    FT_F26Dot6  char_ويڪر,
                    FT_F26Dot6  char_اوچائي,
                    FT_UInt     horz_resolution,
                    FT_UInt     vert_resolution )
  {
    FT_سائيز_Request_Rec  req;


    /* check of `face' delayed to `FT_Request_Size' */

    إذا ( !char_ويڪر )
      char_ويڪر = char_اوچائي;
    وإلا إذا ( !char_اوچائي )
      char_اوچائي = char_ويڪر;

    إذا ( !horz_resolution )
      horz_resolution = vert_resolution;
    وإلا إذا ( !vert_resolution )
      vert_resolution = horz_resolution;

    إذا ( char_ويڪر  < 1 * 64 )
      char_ويڪر  = 1 * 64;
    إذا ( char_اوچائي < 1 * 64 )
      char_اوچائي = 1 * 64;

    إذا ( !horz_resolution )
      horz_resolution = vert_resolution = 72;

    req.type           = FT_سائيز_REQUEST_TYPE_NOMINAL;
    req.ويڪر          = char_ويڪر;
    req.اوچائي         = char_اوچائي;
    req.horiResolution = horz_resolution;
    req.vertResolution = vert_resolution;

    أرجع FT_Request_سائيز( face, &req );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_سيٽ_Pixel_Sizes( FT_Face  face,
                      FT_UInt  pixel_ويڪر,
                      FT_UInt  pixel_اوچائي )
  {
    FT_سائيز_Request_Rec  req;


    /* check of `face' delayed to `FT_Request_Size' */

    إذا ( pixel_ويڪر == 0 )
      pixel_ويڪر = pixel_اوچائي;
    وإلا إذا ( pixel_اوچائي == 0 )
      pixel_اوچائي = pixel_ويڪر;

    إذا ( pixel_ويڪر  < 1 )
      pixel_ويڪر  = 1;
    إذا ( pixel_اوچائي < 1 )
      pixel_اوچائي = 1;

    /* use `>=' to avoid potential compiler warning on 16bit platforms */
    إذا ( pixel_ويڪر >= 0xFFFFU )
      pixel_ويڪر = 0xFFFFU;
    إذا ( pixel_اوچائي >= 0xFFFFU )
      pixel_اوچائي = 0xFFFFU;

    req.type           = FT_سائيز_REQUEST_TYPE_NOMINAL;
    req.ويڪر          = (FT_Long)( pixel_ويڪر << 6 );
    req.اوچائي         = (FT_Long)( pixel_اوچائي << 6 );
    req.horiResolution = 0;
    req.vertResolution = 0;

    أرجع FT_Request_سائيز( face, &req );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Kerning( FT_Face     face,
                  FT_UInt     کاٻو_glyph,
                  FT_UInt     ساڄو_glyph,
                  FT_UInt     kern_گهڻائي,
                  FT_ويڪٽر  *akerning )
  {
    FT_Error   error = FT_Err_Ok;
    FT_Driver  driver;


    إذا ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );

    إذا ( !akerning )
      أرجع FT_THROW( Invalid_دليل );

    driver = face->driver;

    akerning->x = 0;
    akerning->y = 0;

    إذا ( driver->clazz->get_kerning )
    {
      error = driver->clazz->get_kerning( face,
                                          کاٻو_glyph,
                                          ساڄو_glyph,
                                          akerning );
      إذا ( !error )
      {
        إذا ( kern_گهڻائي != FT_KERNING_UNSCALED )
        {
          akerning->x = FT_MulFix( akerning->x, face->سائيز->metrics.x_scale );
          akerning->y = FT_MulFix( akerning->y, face->سائيز->metrics.y_scale );

          إذا ( kern_گهڻائي != FT_KERNING_UNFITTED )
          {
            FT_Pos  orig_x = akerning->x;
            FT_Pos  orig_y = akerning->y;


            /* we scale down kerning values for small ppem values */
            /* to avoid that rounding makes them too big.         */
            /* `25' has been determined heuristically.            */
            إذا ( face->سائيز->metrics.x_ppem < 25 )
              akerning->x = FT_MulDiv( orig_x,
                                       face->سائيز->metrics.x_ppem, 25 );
            إذا ( face->سائيز->metrics.y_ppem < 25 )
              akerning->y = FT_MulDiv( orig_y,
                                       face->سائيز->metrics.y_ppem, 25 );

            akerning->x = FT_PIX_ڦيرو( akerning->x );
            akerning->y = FT_PIX_ڦيرو( akerning->y );

#جيڪڏهن_وصف_ٿيل FT_DEBUG_LEVEL_TRACE
            {
              FT_Pos  orig_x_rounded = FT_PIX_ڦيرو( orig_x );
              FT_Pos  orig_y_rounded = FT_PIX_ڦيرو( orig_y );


              إذا ( akerning->x != orig_x_rounded ||
                   akerning->y != orig_y_rounded )
                FT_TRACE5(( "FT_Get_Kerning: horizontal kerning"
                            " (%ld, %ld) scaled down to (%ld, %ld) pixels\n",
                            orig_x_rounded / 64, orig_y_rounded / 64,
                            akerning->x / 64, akerning->y / 64 ));
            }
#جيڪڏهن_پورو
          }
        }
      }
    }

    أرجع error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Track_Kerning( FT_Face    face,
                        FT_Fixed   نقطو_سائيز,
                        FT_Int     degree,
                        FT_Fixed*  akerning )
  {
    FT_Service_Kerning  service;
    FT_Error            error = FT_Err_Ok;


    إذا ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );

    إذا ( !akerning )
      أرجع FT_THROW( Invalid_دليل );

    FT_FACE_ڳوليو_SERVICE( face, service, KERNING );
    إذا ( !service )
      أرجع FT_THROW( Unimplemented_Feature );

    error = service->get_track( face,
                                نقطو_سائيز,
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
    FT_CharMap*  حد_بندي;


    إذا ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );

    /* FT_ENCODING_NONE is a valid encoding for BDF, PCF, and Windows FNT */
    إذا ( encoding == FT_ENCODING_NONE && !face->انگ_charmaps )
      أرجع FT_THROW( Invalid_دليل );

    /* FT_ENCODING_UNICODE is special.  We try to find the `best' Unicode */
    /* charmap available, i.e., one with UCS-4 characters, if possible.   */
    /*                                                                    */
    /* This is done by find_unicode_charmap() above, to share code.       */
    إذا ( encoding == FT_ENCODING_UNICODE )
      أرجع ڳوليو_unicode_charmap( face );

    cur = face->charmaps;
    إذا ( !cur )
      أرجع FT_THROW( Invalid_CharMap_Handle );

    حد_بندي = cur + face->انگ_charmaps;

    لكل ( ; cur < حد_بندي; cur++ )
    {
      إذا ( cur[0]->encoding == encoding )
      {
        face->charmap = cur[0];
        أرجع FT_Err_Ok;
      }
    }

    أرجع FT_THROW( Invalid_دليل );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_سيٽ_Charmap( FT_Face     face,
                  FT_CharMap  charmap )
  {
    FT_CharMap*  cur;
    FT_CharMap*  حد_بندي;


    إذا ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );

    cur = face->charmaps;
    إذا ( !cur || !charmap )
      أرجع FT_THROW( Invalid_CharMap_Handle );

    حد_بندي = cur + face->انگ_charmaps;

    لكل ( ; cur < حد_بندي; cur++ )
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

    أرجع FT_THROW( Invalid_دليل );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Int )
  FT_Get_Charmap_اشاريو( FT_CharMap  charmap )
  {
    FT_Int  i;


    إذا ( !charmap || !charmap->face )
      أرجع -1;

    لكل ( i = 0; i < charmap->face->انگ_charmaps; i++ )
      إذا ( charmap->face->charmaps[i] == charmap )
        اكسر;

    FT_ASSERT( i < charmap->face->انگ_charmaps );

    أرجع i;
  }


  ساكن فراغ
  ft_cmap_پورو_اندروني( FT_CMap  cmap )
  {
    FT_CMap_Class  clazz  = cmap->clazz;
    FT_Face        face   = cmap->charmap.face;
    FT_يادگيري      يادگيري = FT_FACE_يادگيري( face );


    إذا ( clazz->پورو )
      clazz->پورو( cmap );

    FT_FREE( cmap );
  }


  FT_بنياد_DEF( فراغ )
  FT_CMap_پورو( FT_CMap  cmap )
  {
    إذا ( cmap )
    {
      FT_Face    face   = cmap->charmap.face;
      FT_يادگيري  يادگيري = FT_FACE_يادگيري( face );
      FT_Error   error;
      FT_Int     i, j;


      لكل ( i = 0; i < face->انگ_charmaps; i++ )
      {
        إذا ( (FT_CMap)face->charmaps[i] == cmap )
        {
          FT_CharMap  آخري_charmap = face->charmaps[face->انگ_charmaps - 1];


          إذا ( FT_QRENEW_ARRAY( face->charmaps,
                                face->انگ_charmaps,
                                face->انگ_charmaps - 1 ) )
            أرجع;

          /* remove it from our list of charmaps */
          لكل ( j = i + 1; j < face->انگ_charmaps; j++ )
          {
            إذا ( j == face->انگ_charmaps - 1 )
              face->charmaps[j - 1] = آخري_charmap;
            وإلا
              face->charmaps[j - 1] = face->charmaps[j];
          }

          face->انگ_charmaps--;

          إذا ( (FT_CMap)face->charmap == cmap )
            face->charmap = عدم;

          ft_cmap_پورو_اندروني( cmap );

          اكسر;
        }
      }
    }
  }


  FT_بنياد_DEF( FT_Error )
  FT_CMap_New( FT_CMap_Class  clazz,
               FT_اشارو     init_معلومات,
               FT_CharMap     charmap,
               FT_CMap       *acmap )
  {
    FT_Error   error;
    FT_Face    face;
    FT_يادگيري  يادگيري;
    FT_CMap    cmap = عدم;


    إذا ( !clazz || !charmap || !charmap->face )
      أرجع FT_THROW( Invalid_دليل );

    face   = charmap->face;
    يادگيري = FT_FACE_يادگيري( face );

    إذا ( !FT_ALLOC( cmap, clazz->سائيز ) )
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
                            face->انگ_charmaps,
                            face->انگ_charmaps + 1 ) )
        اذهب Fail;

      face->charmaps[face->انگ_charmaps++] = (FT_CharMap)cmap;
    }

  Exit:
    إذا ( acmap )
      *acmap = cmap;

    أرجع error;

  Fail:
    ft_cmap_پورو_اندروني( cmap );
    cmap = عدم;
    اذهب Exit;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt )
  FT_Get_Char_اشاريو( FT_Face   face,
                     FT_ULong  charcode )
  {
    FT_UInt  نتيجو = 0;


    إذا ( face && face->charmap )
    {
      FT_CMap  cmap = FT_CMAP( face->charmap );


      إذا ( charcode > 0xFFFFFFFFUL )
      {
        FT_TRACE1(( "FT_Get_Char_Index: too large charcode" ));
        FT_TRACE1(( " 0x%lx is truncated\n", charcode ));
      }

      نتيجو = cmap->clazz->char_اشاريو( cmap, (FT_UInt32)charcode );
      إذا ( نتيجو >= (FT_UInt)face->انگ_glyphs )
        نتيجو = 0;
    }

    أرجع نتيجو;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_ULong )
  FT_Get_پهريون_Char( FT_Face   face,
                     FT_UInt  *agindex )
  {
    FT_ULong  نتيجو = 0;
    FT_UInt   gindex = 0;


    /* only do something if we have a charmap, and we have glyphs at all */
    إذا ( face && face->charmap && face->انگ_glyphs )
    {
      gindex = FT_Get_Char_اشاريو( face, 0 );
      إذا ( gindex == 0 )
        نتيجو = FT_Get_ايندڙ_Char( face, 0, &gindex );
    }

    إذا ( agindex )
      *agindex = gindex;

    أرجع نتيجو;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_ULong )
  FT_Get_ايندڙ_Char( FT_Face   face,
                    FT_ULong  charcode,
                    FT_UInt  *agindex )
  {
    FT_ULong  نتيجو = 0;
    FT_UInt   gindex = 0;


    إذا ( face && face->charmap && face->انگ_glyphs )
    {
      FT_UInt32  ڪوڊ = (FT_UInt32)charcode;
      FT_CMap    cmap = FT_CMAP( face->charmap );


      افعل
      {
        gindex = cmap->clazz->char_ايندڙ( cmap, &ڪوڊ );

      } طالما ( gindex >= (FT_UInt)face->انگ_glyphs );

      نتيجو = ( gindex == 0 ) ? 0 : ڪوڊ;
    }

    إذا ( agindex )
      *agindex = gindex;

    أرجع نتيجو;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Face_Properties( FT_Face        face,
                      FT_UInt        انگ_properties,
                      FT_Parameter*  properties )
  {
    FT_Error  error = FT_Err_Ok;


    إذا ( انگ_properties > 0 && !properties )
    {
      error = FT_THROW( Invalid_دليل );
      اذهب Exit;
    }

    لكل ( ; انگ_properties > 0; انگ_properties-- )
    {
      إذا ( properties->ٽيگ == FT_PARAM_ٽيگ_STEM_DARKENING )
      {
        إذا ( properties->معلومات )
        {
          إذا ( *( (FT_Bool*)properties->معلومات ) == TRUE )
            face->اندروني->no_stem_darkening = FALSE;
          وإلا
            face->اندروني->no_stem_darkening = TRUE;
        }
        وإلا
        {
          /* use module default */
          face->اندروني->no_stem_darkening = -1;
        }
      }
      وإلا إذا ( properties->ٽيگ == FT_PARAM_ٽيگ_LCD_FILTER_WEIGHTS )
      {
        error = FT_THROW( Unimplemented_Feature );
        اذهب Exit;
      }
      وإلا إذا ( properties->ٽيگ == FT_PARAM_ٽيگ_RANDOM_ٻج )
      {
        إذا ( properties->معلومات )
        {
          face->اندروني->random_ٻج = *( (FT_Int32*)properties->معلومات );
          إذا ( face->اندروني->random_ٻج < 0 )
            face->اندروني->random_ٻج = 0;
        }
        وإلا
        {
          /* use module default */
          face->اندروني->random_ٻج = -1;
        }
      }
      وإلا
      {
        error = FT_THROW( Invalid_دليل );
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
  FT_Face_Get_Char_Variant_اشاريو( FT_Face   face,
                               FT_ULong  charcode,
                               FT_ULong  variantSelector )
  {
    FT_UInt  نتيجو = 0;


    إذا ( face                                           &&
         face->charmap                                  &&
         face->charmap->encoding == FT_ENCODING_UNICODE )
    {
      FT_CharMap  charmap = ڳوليو_variant_selector_charmap( face );
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

        نتيجو = vcmap->clazz->char_var_اشاريو( vcmap, ucmap,
                                               (FT_UInt32)charcode,
                                               (FT_UInt32)variantSelector );
      }
    }

    أرجع نتيجو;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Int )
  FT_Face_GetCharVariantIsDefault( FT_Face   face,
                                   FT_ULong  charcode,
                                   FT_ULong  variantSelector )
  {
    FT_Int  نتيجو = -1;


    إذا ( face )
    {
      FT_CharMap  charmap = ڳوليو_variant_selector_charmap( face );


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

        نتيجو = vcmap->clazz->char_var_default( vcmap,
                                                 (FT_UInt32)charcode,
                                                 (FT_UInt32)variantSelector );
      }
    }

    أرجع نتيجو;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt32* )
  FT_Face_GetVariantSelectors( FT_Face  face )
  {
    FT_UInt32  *نتيجو = عدم;


    إذا ( face )
    {
      FT_CharMap  charmap = ڳوليو_variant_selector_charmap( face );


      إذا ( charmap )
      {
        FT_CMap    vcmap  = FT_CMAP( charmap );
        FT_يادگيري  يادگيري = FT_FACE_يادگيري( face );


        نتيجو = vcmap->clazz->variant_list( vcmap, يادگيري );
      }
    }

    أرجع نتيجو;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt32* )
  FT_Face_Get_Variants_جو_Char( FT_Face   face,
                             FT_ULong  charcode )
  {
    FT_UInt32  *نتيجو = عدم;


    إذا ( face )
    {
      FT_CharMap  charmap = ڳوليو_variant_selector_charmap( face );


      إذا ( charmap )
      {
        FT_CMap    vcmap  = FT_CMAP( charmap );
        FT_يادگيري  يادگيري = FT_FACE_يادگيري( face );


        إذا ( charcode > 0xFFFFFFFFUL )
        {
          FT_TRACE1(( "FT_Face_GetVariantsOfChar: too large charcode" ));
          FT_TRACE1(( " 0x%lx is truncated\n", charcode ));
        }

        نتيجو = vcmap->clazz->charvariant_list( vcmap, يادگيري,
                                                 (FT_UInt32)charcode );
      }
    }
    أرجع نتيجو;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt32* )
  FT_Face_Get_Chars_جو_Variant( FT_Face   face,
                             FT_ULong  variantSelector )
  {
    FT_UInt32  *نتيجو = عدم;


    إذا ( face )
    {
      FT_CharMap  charmap = ڳوليو_variant_selector_charmap( face );


      إذا ( charmap )
      {
        FT_CMap    vcmap  = FT_CMAP( charmap );
        FT_يادگيري  يادگيري = FT_FACE_يادگيري( face );


        إذا ( variantSelector > 0xFFFFFFFFUL )
        {
          FT_TRACE1(( "FT_Get_Char_Index: too large variantSelector" ));
          FT_TRACE1(( " 0x%lx is truncated\n", variantSelector ));
        }

        نتيجو = vcmap->clazz->variantchar_list( vcmap, يادگيري,
                                                 (FT_UInt32)variantSelector );
      }
    }

    أرجع نتيجو;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt )
  FT_Get_Name_اشاريو( FT_Face           face,
                     ثابت FT_String*  glyph_name )
  {
    FT_UInt  نتيجو = 0;


    إذا ( face                       &&
         FT_HAS_GLYPH_نالا( face ) &&
         glyph_name                 )
    {
      FT_Service_GlyphDict  service;


      FT_FACE_LOOKUP_SERVICE( face,
                              service,
                              GLYPH_DICT );

      إذا ( service && service->name_اشاريو )
        نتيجو = service->name_اشاريو( face, glyph_name );
    }

    أرجع نتيجو;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Glyph_Name( FT_Face     face,
                     FT_UInt     glyph_اشاريو,
                     FT_اشارو  بفر,
                     FT_UInt     بفر_max )
  {
    FT_Error              error;
    FT_Service_GlyphDict  service;


    إذا ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );

    إذا ( !بفر || بفر_max == 0 )
      أرجع FT_THROW( Invalid_دليل );

    /* clean up buffer */
    ((FT_Byte*)بفر)[0] = '\0';

    إذا ( (FT_Long)glyph_اشاريو >= face->انگ_glyphs )
      أرجع FT_THROW( Invalid_Glyph_اشاريو );

    إذا ( !FT_HAS_GLYPH_نالا( face ) )
      أرجع FT_THROW( Invalid_دليل );

    FT_FACE_LOOKUP_SERVICE( face, service, GLYPH_DICT );
    إذا ( service && service->get_name )
      error = service->get_name( face, glyph_اشاريو, بفر, بفر_max );
    وإلا
      error = FT_THROW( Invalid_دليل );

    أرجع error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( ثابت حرف* )
  FT_Get_Postscript_Name( FT_Face  face )
  {
    ثابت حرف*  نتيجو = عدم;


    إذا ( !face )
      اذهب Exit;

    إذا ( !نتيجو )
    {
      FT_Service_PsFontName  service;


      FT_FACE_LOOKUP_SERVICE( face,
                              service,
                              POSTSCRIPT_FONT_NAME );

      إذا ( service && service->get_ps_font_name )
        نتيجو = service->get_ps_font_name( face );
    }

  Exit:
    أرجع نتيجو;
  }


  /* documentation is in tttables.h */

  FT_EXPORT_DEF( فراغ* )
  FT_Get_Sfnt_جدول( FT_Face      face,
                     FT_Sfnt_ٽيگ  ٽيگ )
  {
    فراغ*                  جدول = عدم;
    FT_Service_SFNT_جدول  service;


    إذا ( face && FT_IS_SFNT( face ) )
    {
      FT_FACE_ڳوليو_SERVICE( face, service, SFNT_جدول );
      إذا ( service )
        جدول = service->get_جدول( face, ٽيگ );
    }

    أرجع جدول;
  }


  /* documentation is in tttables.h */

  FT_EXPORT_DEF( FT_Error )
  FT_ڀريو_Sfnt_جدول( FT_Face    face,
                      FT_ULong   ٽيگ,
                      FT_Long    وٿي,
                      FT_Byte*   بفر,
                      FT_ULong*  ڊيگهائي )
  {
    FT_Service_SFNT_جدول  service;


    إذا ( !face || !FT_IS_SFNT( face ) )
      أرجع FT_THROW( Invalid_Face_Handle );

    FT_FACE_ڳوليو_SERVICE( face, service, SFNT_جدول );
    إذا ( !service )
      أرجع FT_THROW( Unimplemented_Feature );

    أرجع service->ڀريو_جدول( face, ٽيگ, وٿي, بفر, ڊيگهائي );
  }


  /* documentation is in tttables.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Sfnt_جدول_Info( FT_Face    face,
                      FT_UInt    جدول_اشاريو,
                      FT_ULong  *ٽيگ,
                      FT_ULong  *ڊيگهائي )
  {
    FT_Service_SFNT_جدول  service;
    FT_ULong               وٿي;


    /* test for valid `length' delayed to `service->table_info' */

    إذا ( !face || !FT_IS_SFNT( face ) )
      أرجع FT_THROW( Invalid_Face_Handle );

    FT_FACE_ڳوليو_SERVICE( face, service, SFNT_جدول );
    إذا ( !service )
      أرجع FT_THROW( Unimplemented_Feature );

    أرجع service->جدول_info( face, جدول_اشاريو, ٽيگ, &وٿي, ڊيگهائي );
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
    FT_FACE_ڳوليو_SERVICE( face, service, TT_CMAP );
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
    FT_FACE_ڳوليو_SERVICE( face, service, TT_CMAP );
    إذا ( !service )
      أرجع -1;
    إذا ( service->get_cmap_info( charmap, &cmap_info ))
      أرجع -1;

    أرجع cmap_info.format;
  }


  /* documentation is in ftsizes.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Activate_سائيز( FT_سائيز  سائيز )
  {
    FT_Face  face;


    إذا ( !سائيز )
      أرجع FT_THROW( Invalid_سائيز_Handle );

    face = سائيز->face;
    إذا ( !face || !face->driver )
      أرجع FT_THROW( Invalid_Face_Handle );

    /* we don't need anything more complex than that; all size objects */
    /* are already listed by the face                                  */
    face->سائيز = سائيز;

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
  FT_بنياد_DEF( FT_Renderer )
  FT_Lookup_Renderer( FT_Library       library,
                      FT_Glyph_Format  format,
                      FT_List_ڳنڍ*     ڳنڍ )
  {
    FT_List_ڳنڍ  cur;
    FT_Renderer  نتيجو = عدم;


    إذا ( !library )
      اذهب Exit;

    cur = library->renderers.سر;

    إذا ( ڳنڍ )
    {
      إذا ( *ڳنڍ )
        cur = (*ڳنڍ)->ايندڙ;
      *ڳنڍ = عدم;
    }

    طالما ( cur )
    {
      FT_Renderer  renderer = FT_RENDERER( cur->معلومات );


      إذا ( renderer->glyph_format == format )
      {
        إذا ( ڳنڍ )
          *ڳنڍ = cur;

        نتيجو = renderer;
        اكسر;
      }
      cur = cur->ايندڙ;
    }

  Exit:
    أرجع نتيجو;
  }


  ساكن FT_Renderer
  ft_lookup_glyph_renderer( FT_Glyph_جڳهه  جڳهه )
  {
    FT_Face      face    = جڳهه->face;
    FT_Library   library = FT_FACE_LIBRARY( face );
    FT_Renderer  نتيجو  = library->cur_renderer;


    إذا ( !نتيجو || نتيجو->glyph_format != جڳهه->format )
      نتيجو = FT_Lookup_Renderer( library, جڳهه->format, 0 );

    أرجع نتيجو;
  }


  ساكن فراغ
  ft_سيٽ_current_renderer( FT_Library  library )
  {
    FT_Renderer  renderer;


    renderer = FT_Lookup_Renderer( library, FT_GLYPH_FORMAT_OUTLINE, 0 );
    library->cur_renderer = renderer;
  }


  ساكن FT_Error
  ft_جوڙيو_renderer( FT_Module  module )
  {
    FT_Library   library = module->library;
    FT_يادگيري    يادگيري  = library->يادگيري;
    FT_Error     error;
    FT_List_ڳنڍ  ڳنڍ    = عدم;


    إذا ( FT_QNEW( ڳنڍ ) )
      اذهب Exit;

    {
      FT_Renderer         render = FT_RENDERER( module );
      FT_Renderer_Class*  clazz  = (FT_Renderer_Class*)module->clazz;


      render->clazz        = clazz;
      render->glyph_format = clazz->glyph_format;

      /* allocate raster object if needed */
      إذا ( clazz->raster_class && clazz->raster_class->raster_new )
      {
        error = clazz->raster_class->raster_new( يادگيري, &render->raster );
        إذا ( error )
          اذهب Fail;

        render->raster_render = clazz->raster_class->raster_render;
        render->render        = clazz->render_glyph;
      }

#جيڪڏهن_وصف_ٿيل FT_CONFIG_OPTION_SVG
      إذا ( clazz->glyph_format == FT_GLYPH_FORMAT_SVG )
        render->render = clazz->render_glyph;
#جيڪڏهن_پورو

      /* add to list */
      ڳنڍ->معلومات = module;
      FT_List_جوڙيو( &library->renderers, ڳنڍ );

      ft_سيٽ_current_renderer( library );
    }

  Fail:
    إذا ( error )
      FT_FREE( ڳنڍ );

  Exit:
    أرجع error;
  }


  ساكن فراغ
  ft_remove_renderer( FT_Module  module )
  {
    FT_Library   library;
    FT_يادگيري    يادگيري;
    FT_List_ڳنڍ  ڳنڍ;


    library = module->library;
    إذا ( !library )
      أرجع;

    يادگيري = library->يادگيري;

    ڳنڍ = FT_List_ڳوليو( &library->renderers, module );
    إذا ( ڳنڍ )
    {
      FT_Renderer  render = FT_RENDERER( module );


      /* release raster object, if any */
      إذا ( render->raster )
        render->clazz->raster_class->raster_پورو( render->raster );

      /* remove from list */
      FT_List_Remove( &library->renderers, ڳنڍ );
      FT_FREE( ڳنڍ );

      ft_سيٽ_current_renderer( library );
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
  FT_سيٽ_Renderer( FT_Library     library,
                   FT_Renderer    renderer,
                   FT_UInt        انگ_params,
                   FT_Parameter*  parameters )
  {
    FT_List_ڳنڍ  ڳنڍ;
    FT_Error     error = FT_Err_Ok;

    FT_Renderer_سيٽ_گهڻائي_Func  سيٽ_گهڻائي;


    إذا ( !library )
    {
      error = FT_THROW( Invalid_Library_Handle );
      اذهب Exit;
    }

    إذا ( !renderer )
    {
      error = FT_THROW( Invalid_دليل );
      اذهب Exit;
    }

    إذا ( انگ_params > 0 && !parameters )
    {
      error = FT_THROW( Invalid_دليل );
      اذهب Exit;
    }

    ڳنڍ = FT_List_ڳوليو( &library->renderers, renderer );
    إذا ( !ڳنڍ )
    {
      error = FT_THROW( Invalid_دليل );
      اذهب Exit;
    }

    FT_List_مٿي( &library->renderers, ڳنڍ );

    إذا ( renderer->glyph_format == FT_GLYPH_FORMAT_OUTLINE )
      library->cur_renderer = renderer;

    سيٽ_گهڻائي = renderer->clazz->سيٽ_گهڻائي;

    لكل ( ; انگ_params > 0; انگ_params-- )
    {
      error = سيٽ_گهڻائي( renderer, parameters->ٽيگ, parameters->معلومات );
      إذا ( error )
        اكسر;
      parameters++;
    }

  Exit:
    أرجع error;
  }


  FT_بنياد_DEF( FT_Error )
  FT_Render_Glyph_اندروني( FT_Library      library,
                            FT_Glyph_جڳهه    جڳهه,
                            FT_Render_گهڻائي  render_گهڻائي )
  {
    FT_Error     error = FT_Err_Ok;
    FT_Face      face  = جڳهه->face;
    FT_Renderer  renderer;
    FT_List_ڳنڍ  ڳنڍ  = عدم;


    /* try to render colored glyph layers as a special case */
    إذا ( جڳهه->اندروني->ڀريو_flags & FT_ڀريو_COLOR &&
         جڳهه->format == FT_GLYPH_FORMAT_OUTLINE    )
    {
      FT_LayerIterator  iterator;

      FT_UInt  بنياد_glyph = جڳهه->glyph_اشاريو;

      FT_Bool  have_layers;
      FT_UInt  glyph_اشاريو;
      FT_UInt  color_اشاريو;


      iterator.p  = عدم;
      have_layers = FT_Get_Color_Glyph_Layer( face,
                                              بنياد_glyph,
                                              &glyph_اشاريو,
                                              &color_اشاريو,
                                              &iterator );
      إذا ( have_layers )
      {
        error = FT_New_Glyph_جڳهه( face, عدم );
        إذا ( !error )
        {
          TT_Face       ttface = (TT_Face)face;
          SFNT_Service  sfnt   = (SFNT_Service)ttface->sfnt;


          افعل
          {
            FT_Int32  ڀريو_flags = جڳهه->اندروني->ڀريو_flags;


            /* disable the `FT_LOAD_COLOR' flag to avoid recursion */
            /* right here in this function                         */
            ڀريو_flags &= ~FT_ڀريو_COLOR;

            /* render into the new `face->glyph' glyph slot */
            ڀريو_flags |= FT_ڀريو_RENDER | FT_ڀريو_NO_BITMAP;

            error = FT_ڀريو_Glyph( face, glyph_اشاريو, ڀريو_flags );
            إذا ( error )
              اكسر;

            /* blend new `face->glyph' into old `slot'; */
            /* at the first call, `slot' is still empty */
            error = sfnt->colr_blend( ttface,
                                      color_اشاريو,
                                      جڳهه,
                                      face->glyph );
            إذا ( error )
              اكسر;

          } طالما ( FT_Get_Color_Glyph_Layer( face,
                                              بنياد_glyph,
                                              &glyph_اشاريو,
                                              &color_اشاريو,
                                              &iterator ) );

          إذا ( !error )
            جڳهه->format = FT_GLYPH_FORMAT_BITMAP;

          /* this call also restores `slot' as the glyph slot */
          FT_پورو_Glyph_جڳهه( face->glyph );
        }

        إذا ( !error )
          أرجع error;

        /* Failed to do the colored layer.  Draw outline instead. */
        جڳهه->format = FT_GLYPH_FORMAT_OUTLINE;
      }
    }

    /* small shortcut for the very common case */
    إذا ( جڳهه->format == FT_GLYPH_FORMAT_OUTLINE )
    {
      renderer = library->cur_renderer;
      ڳنڍ     = library->renderers.سر;
    }
    وإلا
      renderer = FT_Lookup_Renderer( library, جڳهه->format, &ڳنڍ );

    error = FT_ERR( Cannot_Render_Glyph );
    طالما ( renderer )
    {
      error = renderer->render( renderer, جڳهه, render_گهڻائي, عدم );
      إذا ( !error                                   ||
           FT_ERR_NEQ( error, Cannot_Render_Glyph ) )
        اكسر;

      /* `FT_Err_Cannot_Render_Glyph` is returned if the render mode */
      /* is unsupported by the current renderer for this glyph image */
      /* format.                                                     */

      /* now, look for another renderer that supports the same */
      /* format.                                               */
      renderer = FT_Lookup_Renderer( library, جڳهه->format, &ڳنڍ );
    }

    /* it is not an error if we cannot render a bitmap glyph */
    إذا ( FT_ERR_EQ( error, Cannot_Render_Glyph ) &&
         جڳهه->format == FT_GLYPH_FORMAT_BITMAP  )
      error = FT_Err_Ok;

#جيڪڏهن_وصف_ٿيل FT_DEBUG_LEVEL_TRACE

#وصف_رد  FT_COMPONENT
#وصف_ڪريو FT_COMPONENT  checksum

    /*
     * Computing the MD5 checksum is expensive, unnecessarily distorting a
     * possible profiling of FreeType if compiled with tracing support.  For
     * this reason, we execute the following code only if explicitly
     * requested.
     */

    /* we use FT_TRACE3 in this block */
    إذا ( !error                               &&
         ft_trace_سطحون[trace_checksum] >= 3 &&
         جڳهه->bitmap.بفر                  )
    {
      FT_Bitmap  bitmap;
      FT_Error   err;


      FT_Bitmap_Init( &bitmap );

      /* we convert to a single bitmap format for computing the checksum */
      /* this also converts the bitmap flow to `down' (i.e., pitch > 0)  */
      err = FT_Bitmap_Convert( library, &جڳهه->bitmap, &bitmap, 1 );
      إذا ( !err )
      {
        MD5_CTX        ctx;
        بنانشان حرف  md5[16];
        بنانشان ڊگهو  coverage = 0;
        صحيح            i, j;
        صحيح            صفون  = (صحيح)bitmap.صفون;
        صحيح            pitch = bitmap.pitch;


        FT_TRACE3(( "FT_Render_Glyph: bitmap %dx%d, %s (mode %d)\n",
                    pitch,
                    صفون,
                    pixel_modes[جڳهه->bitmap.pixel_گهڻائي],
                    جڳهه->bitmap.pixel_گهڻائي ));

        لكل ( i = 0; i < صفون; i++ )
          لكل ( j = 0; j < pitch; j++ )
            coverage += bitmap.بفر[i * pitch + j];

        FT_TRACE3(( "  Total coverage: %lu\n", coverage ));

        MD5_Init( &ctx );
        إذا ( bitmap.بفر )
          MD5_Update( &ctx, bitmap.بفر,
                      (بنانشان ڊگهو)صفون * (بنانشان ڊگهو)pitch );
        MD5_Final( md5, &ctx );

        FT_TRACE3(( "  MD5 checksum: " ));
        لكل ( i = 0; i < 16; i++ )
          FT_TRACE3(( "%02X", md5[i] ));
        FT_TRACE3(( "\n" ));
      }

      FT_Bitmap_پورو( library, &bitmap );
    }

    /*
     * Dump bitmap in Netpbm format (PBM or PGM).
     */

    /* we use FT_TRACE7 in this block */
    إذا ( !error                               &&
         ft_trace_سطحون[trace_checksum] >= 7 &&
         جڳهه->bitmap.بفر                  )
    {
      إذا ( جڳهه->bitmap.صفون  < 128U &&
           جڳهه->bitmap.ويڪر < 128U )
      {
        صحيح  صفون  = (صحيح)جڳهه->bitmap.صفون;
        صحيح  ويڪر = (صحيح)جڳهه->bitmap.ويڪر;
        صحيح  pitch =      جڳهه->bitmap.pitch;
        صحيح  i, j, m;

        بنانشان حرف*  topleft = جڳهه->bitmap.بفر;


        إذا ( pitch < 0 )
          topleft -= pitch * ( صفون - 1 );

        FT_TRACE7(( "Netpbm image: start\n" ));
        بدل ( جڳهه->bitmap.pixel_گهڻائي )
        {
        حالة FT_PIXEL_گهڻائي_MONO:
          FT_TRACE7(( "P1 %d %d\n", ويڪر, صفون ));
          لكل ( i = 0; i < صفون; i++ )
          {
            لكل ( j = 0; j < ويڪر; )
              لكل ( m = 128; m > 0 && j < ويڪر; m >>= 1, j++ )
                FT_TRACE7(( " %d",
                            ( topleft[i * pitch + j / 8] & m ) != 0 ));
            FT_TRACE7(( "\n" ));
          }
          اكسر;

        افتراضي:
          FT_TRACE7(( "P2 %d %d 255\n", ويڪر, صفون ));
          لكل ( i = 0; i < صفون; i++ )
          {
            لكل ( j = 0; j < ويڪر; j += 1 )
              FT_TRACE7(( " %3u", topleft[i * pitch + j] ));
            FT_TRACE7(( "\n" ));
          }
        }
        FT_TRACE7(( "Netpbm image: end\n" ));
      }
      وإلا
        FT_TRACE7(( "Netpbm image: too large, omitted\n" ));
    }

#وصف_رد  FT_COMPONENT
#وصف_ڪريو FT_COMPONENT  objs

#جيڪڏهن_پورو /* FT_DEBUG_LEVEL_TRACE */

    أرجع error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Render_Glyph( FT_Glyph_جڳهه    جڳهه,
                   FT_Render_گهڻائي  render_گهڻائي )
  {
    FT_Library  library;


    إذا ( !جڳهه || !جڳهه->face )
      أرجع FT_THROW( Invalid_دليل );

    library = FT_FACE_LIBRARY( جڳهه->face );

    أرجع FT_Render_Glyph_اندروني( library, جڳهه, render_گهڻائي );
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
    FT_يادگيري               يادگيري  = module->يادگيري;


    إذا ( library && library->auto_hinter == module )
      library->auto_hinter = عدم;

    /* if the module is a renderer */
    إذا ( FT_MODULE_IS_RENDERER( module ) )
      ft_remove_renderer( module );

    /* if the module is a font driver, add some steps */
    إذا ( FT_MODULE_IS_DRIVER( module ) )
      Destroy_Driver( FT_DRIVER( module ) );

    /* finalize the module object */
    إذا ( clazz->module_پورو )
      clazz->module_پورو( module );

    /* discard it */
    FT_FREE( module );
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_جوڙيو_Module( FT_Library              library,
                 ثابت FT_Module_Class*  clazz )
  {
    FT_Error   error;
    FT_يادگيري  يادگيري;
    FT_Module  module = عدم;
    FT_UInt    nn;


#وصف_ڪريو FREETYPE_VER_FIXED  ( ( (FT_Long)FREETYPE_MAJOR << 16 ) | \
                                FREETYPE_MINOR                  )

    إذا ( !library )
      أرجع FT_THROW( Invalid_Library_Handle );

    إذا ( !clazz )
      أرجع FT_THROW( Invalid_دليل );

    /* check FreeType version */
    إذا ( clazz->module_requires > FREETYPE_VER_FIXED )
      أرجع FT_THROW( Invalid_نسخو );

    /* look for a module with the same name in the library's table */
    لكل ( nn = 0; nn < library->انگ_modules; nn++ )
    {
      module = library->modules[nn];
      إذا ( ft_strcmp( module->clazz->module_name, clazz->module_name ) == 0 )
      {
        /* this installed module has the same name, compare their versions */
        إذا ( clazz->module_نسخو <= module->clazz->module_نسخو )
          أرجع FT_THROW( هيٺاهون_Module_نسخو );

        /* remove the module from our list, then exit the loop to replace */
        /* it by our new version..                                        */
        FT_Remove_Module( library, module );
        اكسر;
      }
    }

    يادگيري = library->يادگيري;
    error  = FT_Err_Ok;

    إذا ( library->انگ_modules >= FT_MAX_MODULES )
    {
      error = FT_THROW( Too_Many_Drivers );
      اذهب Exit;
    }

    /* allocate module object */
    إذا ( FT_ALLOC( module, clazz->module_سائيز ) )
      اذهب Exit;

    /* base initialization */
    module->clazz   = clazz;
    module->library = library;
    module->يادگيري  = يادگيري;

    /* check whether the module is a renderer - this must be performed */
    /* before the normal module initialization                         */
    إذا ( FT_MODULE_IS_RENDERER( module ) )
    {
      /* add to the renderers list */
      error = ft_جوڙيو_renderer( module );
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
    library->modules[library->انگ_modules++] = module;

  Exit:
    أرجع error;

  Fail:
    إذا ( FT_MODULE_IS_RENDERER( module ) )
    {
      FT_Renderer  renderer = FT_RENDERER( module );


      إذا ( renderer->clazz                                          &&
           renderer->clazz->glyph_format == FT_GLYPH_FORMAT_OUTLINE &&
           renderer->raster                                         )
        renderer->clazz->raster_class->raster_پورو( renderer->raster );
    }

    FT_FREE( module );
    اذهب Exit;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Module )
  FT_Get_Module( FT_Library   library,
                 ثابت حرف*  module_name )
  {
    FT_Module   نتيجو = عدم;
    FT_Module*  cur;
    FT_Module*  حد_بندي;


    إذا ( !library || !module_name )
      أرجع نتيجو;

    cur   = library->modules;
    حد_بندي = cur + library->انگ_modules;

    لكل ( ; cur < حد_بندي; cur++ )
      إذا ( ft_strcmp( cur[0]->clazz->module_name, module_name ) == 0 )
      {
        نتيجو = cur[0];
        اكسر;
      }

    أرجع نتيجو;
  }


  /* documentation is in ftobjs.h */

  FT_بنياد_DEF( ثابت فراغ* )
  FT_Get_Module_Interface( FT_Library   library,
                           ثابت حرف*  mod_name )
  {
    FT_Module  module;


    /* test for valid `library' delayed to FT_Get_Module() */

    module = FT_Get_Module( library, mod_name );

    أرجع module ? module->clazz->module_interface : 0;
  }


  FT_بنياد_DEF( FT_اشارو )
  ft_module_get_service( FT_Module    module,
                         ثابت حرف*  service_id,
                         FT_Bool      global )
  {
    FT_اشارو  نتيجو = عدم;


    إذا ( module )
    {
      FT_ASSERT( module->clazz && module->clazz->get_interface );

      /* first, look for the service in the module */
      إذا ( module->clazz->get_interface )
        نتيجو = module->clazz->get_interface( module, service_id );

      إذا ( global && !نتيجو )
      {
        /* we didn't find it, look in all other modules then */
        FT_Library  library = module->library;
        FT_Module*  cur     = library->modules;
        FT_Module*  حد_بندي   = cur + library->انگ_modules;


        لكل ( ; cur < حد_بندي; cur++ )
        {
          إذا ( cur[0] != module )
          {
            FT_ASSERT( cur[0]->clazz );

            إذا ( cur[0]->clazz->get_interface )
            {
              نتيجو = cur[0]->clazz->get_interface( cur[0], service_id );
              إذا ( نتيجو )
                اكسر;
            }
          }
        }
      }
    }

    أرجع نتيجو;
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
      FT_Module*  حد_بندي = cur + library->انگ_modules;


      لكل ( ; cur < حد_بندي; cur++ )
      {
        إذا ( cur[0] == module )
        {
          /* remove it from the table */
          library->انگ_modules--;
          حد_بندي--;
          طالما ( cur < حد_بندي )
          {
            cur[0] = cur[1];
            cur++;
          }
          حد_بندي[0] = عدم;

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
                  FT_Bool           سيٽ,
                  FT_Bool           قدر_is_string )
  {
    FT_Module*           cur;
    FT_Module*           حد_بندي;
    FT_Module_Interface  interface;

    FT_Service_Properties  service;

#جيڪڏهن_وصف_ٿيل FT_DEBUG_LEVEL_ERROR
    ثابت FT_String*  سيٽ_name  = "FT_Property_Set";
    ثابت FT_String*  get_name  = "FT_Property_Get";
    ثابت FT_String*  func_name = سيٽ ? سيٽ_name : get_name;
#جيڪڏهن_پورو

    FT_Bool  missing_func;


    إذا ( !library )
      أرجع FT_THROW( Invalid_Library_Handle );

    إذا ( !module_name || !property_name || !قدر )
      أرجع FT_THROW( Invalid_دليل );

    cur   = library->modules;
    حد_بندي = cur + library->انگ_modules;

    /* search module */
    لكل ( ; cur < حد_بندي; cur++ )
      إذا ( !ft_strcmp( cur[0]->clazz->module_name, module_name ) )
        اكسر;

    إذا ( cur == حد_بندي )
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

    إذا ( سيٽ )
      missing_func = FT_BOOL( !service->سيٽ_property );
    وإلا
      missing_func = FT_BOOL( !service->get_property );

    إذا ( missing_func )
    {
      FT_TRACE2(( "%s: property service of module `%s' is broken\n",
                  func_name, module_name ));
      أرجع FT_THROW( Unimplemented_Feature );
    }

    أرجع سيٽ ? service->سيٽ_property( cur[0],
                                        property_name,
                                        قدر,
                                        قدر_is_string )
               : service->get_property( cur[0],
                                        property_name,
                                        قدر );
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Property_سيٽ( FT_Library        library,
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


#جيڪڏهن_وصف_ٿيل FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES

  /* this variant is used for handling the FREETYPE_PROPERTIES */
  /* environment variable                                      */

  FT_بنياد_DEF( FT_Error )
  ft_property_string_سيٽ( FT_Library        library,
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

#جيڪڏهن_پورو


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
  FT_New_Library( FT_يادگيري    يادگيري,
                  FT_Library  *alibrary )
  {
    FT_Library  library = عدم;
    FT_Error    error;


    إذا ( !يادگيري || !alibrary )
      أرجع FT_THROW( Invalid_دليل );

#جيڪڏهن_وصف_نه_ٿيل FT_DEBUG_LOGGING
#جيڪڏهن_وصف_ٿيل FT_DEBUG_LEVEL_ERROR
    /* init debugging support */
    ft_debug_init();
#جيڪڏهن_پورو /* FT_DEBUG_LEVEL_ERROR */
#جيڪڏهن_پورو /* !FT_DEBUG_LOGGING */

    /* first of all, allocate the library object */
    إذا ( FT_NEW( library ) )
      أرجع error;

    library->يادگيري = يادگيري;

    library->نسخو_major = FREETYPE_MAJOR;
    library->نسخو_minor = FREETYPE_MINOR;
    library->نسخو_patch = FREETYPE_PATCH;

    library->refcount = 1;

    /* That's ok now */
    *alibrary = library;

    أرجع FT_Err_Ok;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( فراغ )
  FT_Library_نسخو( FT_Library   library,
                      FT_Int      *amajor,
                      FT_Int      *aminor,
                      FT_Int      *apatch )
  {
    FT_Int  major = 0;
    FT_Int  minor = 0;
    FT_Int  patch = 0;


    إذا ( library )
    {
      major = library->نسخو_major;
      minor = library->نسخو_minor;
      patch = library->نسخو_patch;
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
  FT_پورو_Library( FT_Library  library )
  {
    FT_يادگيري  يادگيري;


    إذا ( !library )
      أرجع FT_THROW( Invalid_Library_Handle );

    library->refcount--;
    إذا ( library->refcount > 0 )
      اذهب Exit;

    يادگيري = library->يادگيري;

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
        لكل ( n = 0; n < library->انگ_modules; n++ )
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
            FT_پورو_Face( FT_FACE( faces->سر->معلومات ) );
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
    طالما ( library->انگ_modules > 0 )
      FT_Remove_Module( library,
                        library->modules[library->انگ_modules - 1] );
#وإلا
    {
      FT_UInt  n;


      لكل ( n = 0; n < library->انگ_modules; n++ )
      {
        FT_Module  module = library->modules[n];


        إذا ( module )
        {
          Destroy_Module( module );
          library->modules[n] = عدم;
        }
      }
    }
#جيڪڏهن_پورو

    FT_FREE( library );

  Exit:
    أرجع FT_Err_Ok;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( فراغ )
  FT_سيٽ_Debug_Hook( FT_Library         library,
                     FT_UInt            hook_اشاريو,
                     FT_DebugHook_Func  debug_hook )
  {
    إذا ( library && debug_hook &&
         hook_اشاريو <
           ( حجم ( library->debug_hooks ) / حجم ( فراغ* ) ) )
      library->debug_hooks[hook_اشاريو] = debug_hook;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_TrueTypeEngineType )
  FT_Get_TrueType_Engine_Type( FT_Library  library )
  {
    FT_TrueTypeEngineType  نتيجو = FT_TRUETYPE_ENGINE_TYPE_NONE;


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
          نتيجو = service->engine_type;
      }
    }

    أرجع نتيجو;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_SubGlyph_Info( FT_Glyph_جڳهه  glyph,
                        FT_UInt       sub_اشاريو,
                        FT_Int       *p_اشاريو,
                        FT_UInt      *p_flags,
                        FT_Int       *p_arg1,
                        FT_Int       *p_arg2,
                        FT_ميٽرڪس    *p_transform )
  {
    FT_Error  error = FT_ERR( Invalid_دليل );


    إذا ( glyph                                      &&
         glyph->subglyphs                           &&
         glyph->format == FT_GLYPH_FORMAT_مرکب &&
         sub_اشاريو < glyph->انگ_subglyphs           )
    {
      FT_SubGlyph  subg = glyph->subglyphs + sub_اشاريو;


      *p_اشاريو     = subg->index;
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
                            FT_UInt            بنياد_glyph,
                            FT_UInt           *aglyph_اشاريو,
                            FT_UInt           *acolor_اشاريو,
                            FT_LayerIterator*  iterator )
  {
    TT_Face       ttface;
    SFNT_Service  sfnt;


    إذا ( !face                                   ||
         !aglyph_اشاريو                           ||
         !acolor_اشاريو                           ||
         !iterator                               ||
         بنياد_glyph >= (FT_UInt)face->انگ_glyphs )
      أرجع 0;

    إذا ( !FT_IS_SFNT( face ) )
      أرجع 0;

    ttface = (TT_Face)face;
    sfnt   = (SFNT_Service)ttface->sfnt;

    إذا ( sfnt->get_colr_layer )
      أرجع sfnt->get_colr_layer( ttface,
                                   بنياد_glyph,
                                   aglyph_اشاريو,
                                   acolor_اشاريو,
                                   iterator );
    وإلا
      أرجع 0;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Get_Color_Glyph_Paint( FT_Face                  face,
                            FT_UInt                  بنياد_glyph,
                            FT_Color_پاڙ_Transform  پاڙ_transform,
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
                                         بنياد_glyph,
                                         پاڙ_transform,
                                         paint );
    وإلا
      أرجع 0;
  }


  /* documentation is in ftcolor.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Get_Color_Glyph_ClipBox( FT_Face      face,
                              FT_UInt      بنياد_glyph,
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
                                            بنياد_glyph,
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


/*پڇاڙي*/
