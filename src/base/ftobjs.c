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

#جے_تعریف_شدہ TT_CONFIG_OPTION_VARC
#ضمن <freetype/internal/services/svvarc.h>
#جے_انت

#ضمن <freetype/ftdriver.h>

#جے_تعریف_شدہ FT_CONFIG_OPTION_MAC_FONTS
#ضمن "ftbase.h"
#جے_انت


#جے_تعریف_شدہ FT_DEBUG_LEVEL_TRACE

#ضمن <freetype/ftbitmap.h>

#إذا defined( _MSC_VER )      /* Visual C++ (and Intel C++)   */
  /* We disable the warning `conversion from XXX to YYY,     */
  /* possible loss of data' in order to compile cleanly with */
  /* the maximum level of warnings: `md5.c' is non-FreeType  */
  /* code, and it gets used during development builds only.  */
#ہدایت warning( ڈالو )
#ہدایت warning( disable : 4244 )
#جے_انت /* _MSC_VER */

  /* It's easiest to include `md5.c' directly.  However, since OpenSSL */
  /* also provides the same functions, there might be conflicts if     */
  /* both FreeType and OpenSSL are built as static libraries.  For     */
  /* this reason, we put the MD5 stuff into the `FT_' namespace.       */
#تعریف_کرو MD5_u32plus  FT_MD5_u32plus
#تعریف_کرو MD5_CTX      FT_MD5_CTX
#تعریف_کرو MD5_Init     FT_MD5_Init
#تعریف_کرو MD5_Update   FT_MD5_Update
#تعریف_کرو MD5_Final    FT_MD5_Final

#تعریف_رد  HAVE_OPENSSL

#ضمن "md5.c"

#إذا defined( _MSC_VER )
#ہدایت warning( نکالو )
#جے_انت

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

#جے_انت /* FT_DEBUG_LEVEL_TRACE */


#تعریف_کرو GRID_FIT_METRICS


  /* forward declaration */
  ساكن FT_Error
  ft_open_face_اندرونی( FT_Library           library,
                         ثابت FT_Open_دلیلاں*  دلیلاں,
                         FT_Long              face_اشاریہ,
                         FT_Face             *aface,
                         FT_Bool              test_mac_fonts );


  FT_ادھار_DEF( FT_اشارہ_گر )
  ft_service_list_lookup( FT_ServiceDesc  service_descriptors,
                          ثابت حرف*     service_id )
  {
    FT_اشارہ_گر      نتیجہ = عدم;
    FT_ServiceDesc  desc   = service_descriptors;


    إذا ( desc && service_id )
    {
      لكل ( ; desc->serv_id != عدم; desc++ )
      {
        إذا ( ft_strcmp( desc->serv_id, service_id ) == 0 )
        {
          نتیجہ = (FT_اشارہ_گر)desc->serv_انکڑے;
          اكسر;
        }
      }
    }

    أرجع نتیجہ;
  }


  FT_ادھار_DEF( فراغ )
  ft_validator_init( FT_Validator        valid,
                     ثابت FT_Byte*      ادھار,
                     ثابت FT_Byte*      حد_بندی,
                     FT_ValidationLevel  level )
  {
    valid->ادھار  = ادھار;
    valid->حد_بندی = حد_بندی;
    valid->level = level;
    valid->error = FT_Err_Ok;
  }


  FT_ادھار_DEF( FT_Int )
  ft_validator_لگاتار( FT_Validator  valid )
  {
    /* This function doesn't work!  None should call it. */
    FT_بن_ورتیا( valid );

    أرجع -1;
  }


  FT_ادھار_DEF( فراغ )
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
  FT_ادھار_DEF( FT_Error )
  FT_دھارا_New( FT_Library           library,
                 ثابت FT_Open_دلیلاں*  دلیلاں,
                 FT_دھارا           *astream )
  {
    FT_Error   error;
    FT_حافظہ  حافظہ;
    FT_دھارا  دھارا = عدم;
    FT_UInt    منوال;


    *astream = عدم;

    إذا ( !library )
      أرجع FT_THROW( Invalid_Library_Handle );

    إذا ( !دلیلاں )
      أرجع FT_THROW( Invalid_دلیل );

    حافظہ = library->حافظہ;
    منوال   = دلیلاں->flags &
               ( FT_OPEN_حافظہ | FT_OPEN_دھارا | FT_OPEN_PATHNAME );

    إذا ( منوال == FT_OPEN_حافظہ )
    {
      /* create a memory-based stream */
      إذا ( FT_NEW( دھارا ) )
        اذهب Exit;

      FT_دھارا_Open_حافظہ( دھارا,
                            (ثابت FT_Byte*)دلیلاں->حافظہ_ادھار,
                            (FT_ULong)دلیلاں->حافظہ_ناپ );
      دھارا->حافظہ = حافظہ;
    }

#جے_بنا_تعریف FT_CONFIG_OPTION_DISABLE_دھارا_SUPPORT

    وإلا إذا ( منوال == FT_OPEN_PATHNAME )
    {
      /* create a normal system stream */
      إذا ( FT_NEW( دھارا ) )
        اذهب Exit;

      دھارا->حافظہ = حافظہ;
      error = FT_دھارا_Open( دھارا, دلیلاں->pathname );
      إذا ( error )
        FT_FREE( دھارا );
    }
    وإلا إذا ( ( منوال == FT_OPEN_دھارا ) && دلیلاں->دھارا )
    {
      /* use an existing, user-provided stream */

      /* in this case, we do not need to allocate a new stream object */
      /* since the caller is responsible for closing it himself       */
      دھارا         = دلیلاں->دھارا;
      دھارا->حافظہ = حافظہ;
      error          = FT_Err_Ok;
    }

#جے_انت

    وإلا
    {
      error = FT_THROW( Invalid_دلیل );
      إذا ( ( دلیلاں->flags & FT_OPEN_دھارا ) && دلیلاں->دھارا )
        FT_دھارا_Close( دلیلاں->دھارا );
    }

    إذا ( !error )
      *astream       = دھارا;

  Exit:
    أرجع error;
  }


  FT_ادھار_DEF( فراغ )
  FT_دھارا_Free( FT_دھارا  دھارا,
                  FT_Int     external )
  {
    إذا ( دھارا )
    {
      FT_حافظہ  حافظہ = دھارا->حافظہ;


      FT_دھارا_Close( دھارا );

      إذا ( !external )
        FT_FREE( دھارا );
    }
  }


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#تعریف_رد  FT_COMPONENT
#تعریف_کرو FT_COMPONENT  objs


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
  ft_glyphslot_init( FT_Glyph_تھاں  تھاں )
  {
    FT_Driver         driver   = تھاں->face->driver;
    FT_Driver_Class   clazz    = driver->clazz;
    FT_حافظہ         حافظہ   = driver->جڑ.حافظہ;
    FT_Error          error    = FT_Err_Ok;
    FT_تھاں_اندرونی  اندرونی = عدم;


    تھاں->library = driver->جڑ.library;

    إذا ( FT_NEW( اندرونی ) )
      اذهب Exit;

    تھاں->اندرونی = اندرونی;

    إذا ( FT_DRIVER_ورتدا_OUTLINES( driver ) )
      error = FT_GlyphLoader_New( حافظہ, &اندرونی->loader );

    إذا ( !error && clazz->init_تھاں )
      error = clazz->init_تھاں( تھاں );

#جے_تعریف_شدہ FT_CONFIG_OPTION_SVG
    /* if SVG table exists, allocate the space in `slot->other` */
    إذا ( تھاں->face->face_flags & FT_FACE_FLAG_SVG )
    {
      FT_SVG_Document  document = عدم;


      إذا ( FT_NEW( document ) )
        اذهب Exit;
      تھاں->other = document;
    }
#جے_انت

  Exit:
    أرجع error;
  }


  FT_ادھار_DEF( فراغ )
  ft_glyphslot_free_bitmap( FT_Glyph_تھاں  تھاں )
  {
    إذا ( تھاں->اندرونی && ( تھاں->اندرونی->flags & FT_GLYPH_OWN_BITMAP ) )
    {
      FT_حافظہ  حافظہ = FT_FACE_حافظہ( تھاں->face );


      FT_FREE( تھاں->bitmap.بفر );
      تھاں->اندرونی->flags &= ~FT_GLYPH_OWN_BITMAP;
    }
    وإلا
    {
      /* assume that the bitmap buffer was stolen or not */
      /* allocated from the heap                         */
      تھاں->bitmap.بفر = عدم;
    }
  }


  /* overflow-resistant presetting of bitmap position and dimensions; */
  /* also check whether the size is too large for rendering           */
  FT_ادھار_DEF( FT_Bool )
  ft_glyphslot_preset_bitmap( FT_Glyph_تھاں      تھاں,
                              FT_Render_منوال    منوال,
                              ثابت FT_سمتیہ*  origin )
  {
    FT_Outline*  outline = &تھاں->outline;
    FT_Bitmap*   bitmap  = &تھاں->bitmap;

    FT_Pixel_منوال  pixel_منوال;

    FT_BBox  cbox, pbox;
    FT_Pos   x_کھسکاؤ = 0;
    FT_Pos   y_کھسکاؤ = 0;
    FT_Pos   x_بایاں, y_چوٹی;
    FT_Pos   چوڑائی, اچائی, pitch;
    FT_Bool  ret;


    إذا ( تھاں->format == FT_GLYPH_FORMAT_SVG )
    {
      FT_Module    module;
      SVG_Service  svg_service;


      module      = FT_Get_Module( تھاں->library, "ot-svg" );
      svg_service = (SVG_Service)module->clazz->module_interface;

      أرجع (FT_Bool)svg_service->preset_تھاں( module, تھاں, FALSE );
    }
    وإلا إذا ( تھاں->format != FT_GLYPH_FORMAT_OUTLINE )
      أرجع 1;

    إذا ( origin )
    {
      x_کھسکاؤ = origin->x;
      y_کھسکاؤ = origin->y;
    }

    /* compute the control box, and grid-fit it, */
    /* taking into account the origin shift      */
    FT_Outline_Get_CBox( outline, &cbox );

    /* rough estimate of pixel box */
    pbox.xMin = ( cbox.xMin >> 6 ) + ( x_کھسکاؤ >> 6 );
    pbox.yMin = ( cbox.yMin >> 6 ) + ( y_کھسکاؤ >> 6 );
    pbox.xMax = ( cbox.xMax >> 6 ) + ( x_کھسکاؤ >> 6 );
    pbox.yMax = ( cbox.yMax >> 6 ) + ( y_کھسکاؤ >> 6 );

    /* tiny remainder box */
    cbox.xMin = ( cbox.xMin & 63 ) + ( x_کھسکاؤ & 63 );
    cbox.yMin = ( cbox.yMin & 63 ) + ( y_کھسکاؤ & 63 );
    cbox.xMax = ( cbox.xMax & 63 ) + ( x_کھسکاؤ & 63 );
    cbox.yMax = ( cbox.yMax & 63 ) + ( y_کھسکاؤ & 63 );

    بدل ( منوال )
    {
    حالة FT_RENDER_منوال_MONO:
      pixel_منوال = FT_PIXEL_منوال_MONO;
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
#جے_انت

    حالة FT_RENDER_منوال_LCD:
      pixel_منوال = FT_PIXEL_منوال_LCD;
      ft_lcd_padding( &cbox, تھاں, منوال );
      اذهب Adjust;

    حالة FT_RENDER_منوال_LCD_V:
      pixel_منوال = FT_PIXEL_منوال_LCD_V;
      ft_lcd_padding( &cbox, تھاں, منوال );
      اذهب Adjust;

    حالة FT_RENDER_منوال_عام:
    حالة FT_RENDER_منوال_LIGHT:
    افتراضي:
      pixel_منوال = FT_PIXEL_منوال_سلیٹی;
    Adjust:
      pbox.xMin += cbox.xMin >> 6;
      pbox.yMin += cbox.yMin >> 6;
      pbox.xMax += ( cbox.xMax + 63 ) >> 6;
      pbox.yMax += ( cbox.yMax + 63 ) >> 6;
    }

    x_بایاں = pbox.xMin;
    y_چوٹی  = pbox.yMax;

    چوڑائی  = pbox.xMax - pbox.xMin;
    اچائی = pbox.yMax - pbox.yMin;

    /* Flag the width or height unsuitable for rendering.   */
    /* The limit is based on the ppem value when available. */
    /* FT_Renderer modules should check the return value.   */
    ret = FT_BOOL(     چوڑائی >= 0x10000   ||    اچائی >= 0x10000   ||
                   pbox.xMin < -0x1000000 || pbox.xMax >= 0x1000000 ||
                   pbox.yMin < -0x1000000 || pbox.yMax >= 0x1000000 ||
             ( تھاں->face                                         &&
               ( چوڑائی  > 16 * تھاں->face->ناپ->metrics.x_ppem ||
                 اچائی > 16 * تھاں->face->ناپ->metrics.y_ppem ) ) );

    إذا ( ret )
      FT_TRACE3(( "ft_glyphslot_preset_bitmap: [%ld %ld %ld %ld]\n",
                  pbox.xMin, pbox.yMin, pbox.xMax, pbox.yMax ));

    بدل ( pixel_منوال )
    {
    حالة FT_PIXEL_منوال_MONO:
      pitch = ( ( چوڑائی + 15 ) >> 4 ) << 1;
      اكسر;

    حالة FT_PIXEL_منوال_LCD:
      چوڑائی *= 3;
      pitch  = FT_PAD_CEIL( چوڑائی, 4 );
      اكسر;

    حالة FT_PIXEL_منوال_LCD_V:
      اچائی *= 3;
      FALL_THROUGH;

    حالة FT_PIXEL_منوال_سلیٹی:
    افتراضي:
      pitch = چوڑائی;
    }

    تھاں->bitmap_بایاں = (FT_Int)x_بایاں;
    تھاں->bitmap_چوٹی  = (FT_Int)y_چوٹی;

    bitmap->pixel_منوال = (بنانشانی حرف)pixel_منوال;
    bitmap->عدد_grays  = 256;
    bitmap->چوڑائی      = (بنانشانی صحيح)چوڑائی;
    bitmap->صفاں       = (بنانشانی صحيح)اچائی;
    bitmap->pitch      = pitch;

    أرجع ret;
  }


  FT_ادھار_DEF( فراغ )
  ft_glyphslot_سیٹ_bitmap( FT_Glyph_تھاں  تھاں,
                           FT_Byte*      بفر )
  {
    ft_glyphslot_free_bitmap( تھاں );

    تھاں->bitmap.بفر = بفر;

    FT_ASSERT( (تھاں->اندرونی->flags & FT_GLYPH_OWN_BITMAP) == 0 );
  }


  FT_ادھار_DEF( FT_Error )
  ft_glyphslot_alloc_bitmap( FT_Glyph_تھاں  تھاں )
  {
    FT_حافظہ  حافظہ = FT_FACE_حافظہ( تھاں->face );
    FT_Error   error;


    إذا ( تھاں->اندرونی->flags & FT_GLYPH_OWN_BITMAP )
      FT_FREE( تھاں->bitmap.بفر );
    وإلا
      تھاں->اندرونی->flags |= FT_GLYPH_OWN_BITMAP;

    /* dimensions must be preset */
    FT_MEM_ALLOC_MULT( تھاں->bitmap.بفر,
                       تھاں->bitmap.صفاں,
                       تھاں->bitmap.pitch );
    أرجع error;
  }


  ساكن فراغ
  ft_glyphslot_clear( FT_Glyph_تھاں  تھاں )
  {
    /* free bitmap if needed */
    ft_glyphslot_free_bitmap( تھاں );

    /* clear all public fields in the glyph slot */
    تھاں->glyph_اشاریہ = 0;

    FT_ZERO( &تھاں->metrics );
    FT_ZERO( &تھاں->outline );

    تھاں->bitmap.چوڑائی      = 0;
    تھاں->bitmap.صفاں       = 0;
    تھاں->bitmap.pitch      = 0;
    تھاں->bitmap.pixel_منوال = 0;
    /* `slot->bitmap.buffer' has been handled by ft_glyphslot_free_bitmap */

    تھاں->bitmap_بایاں   = 0;
    تھاں->bitmap_چوٹی    = 0;
    تھاں->عدد_subglyphs = 0;
    تھاں->subglyphs     = عدم;
    تھاں->control_انکڑے  = عدم;
    تھاں->control_len   = 0;

#جے_بنا_تعریف FT_CONFIG_OPTION_SVG
    تھاں->other = عدم;
#وإلا
    إذا ( !( تھاں->face->face_flags & FT_FACE_FLAG_SVG ) )
      تھاں->other = عدم;
    وإلا
    {
      إذا ( تھاں->اندرونی->flags & FT_GLYPH_OWN_GZIP_SVG )
      {
        FT_حافظہ        حافظہ = تھاں->face->حافظہ;
        FT_SVG_Document  doc    = (FT_SVG_Document)تھاں->other;


        FT_FREE( doc->svg_document );
        تھاں->اندرونی->flags &= ~FT_GLYPH_OWN_GZIP_SVG;
      }
    }
#جے_انت

    تھاں->format = FT_GLYPH_FORMAT_NONE;

    تھاں->خطی_Hori_Advance = 0;
    تھاں->خطی_Vert_Advance = 0;
    تھاں->advance.x         = 0;
    تھاں->advance.y         = 0;
    تھاں->lsb_delta         = 0;
    تھاں->rsb_delta         = 0;
  }


  ساكن فراغ
  ft_glyphslot_پورا( FT_Glyph_تھاں  تھاں )
  {
    FT_Driver        driver = تھاں->face->driver;
    FT_Driver_Class  clazz  = driver->clazz;
    FT_حافظہ        حافظہ = driver->جڑ.حافظہ;

#جے_تعریف_شدہ FT_CONFIG_OPTION_SVG
    إذا ( تھاں->face->face_flags & FT_FACE_FLAG_SVG )
    {
      /* Free memory in case SVG was there.                          */
      /* `slot->internal` might be NULL in out-of-memory situations. */
      إذا ( تھاں->اندرونی && تھاں->اندرونی->flags & FT_GLYPH_OWN_GZIP_SVG )
      {
        FT_SVG_Document  doc = (FT_SVG_Document)تھاں->other;


        FT_FREE( doc->svg_document );

        تھاں->اندرونی->flags &= ~FT_GLYPH_OWN_GZIP_SVG;
      }

      FT_FREE( تھاں->other );
    }
#جے_انت

    إذا ( clazz->پورا_تھاں )
      clazz->پورا_تھاں( تھاں );

    /* free bitmap buffer if needed */
    ft_glyphslot_free_bitmap( تھاں );

    /* slot->internal might be NULL in out-of-memory situations */
    إذا ( تھاں->اندرونی )
    {
      /* free glyph loader */
      إذا ( FT_DRIVER_ورتدا_OUTLINES( driver ) )
      {
        FT_Glyph_Loader_پورا( تھاں->اندرونی->loader );
        تھاں->اندرونی->loader = عدم;
      }

      FT_FREE( تھاں->اندرونی );
    }
  }


  /* documentation is in ftobjs.h */

  FT_ادھار_DEF( FT_Error )
  FT_New_Glyph_تھاں( FT_Face        face,
                    FT_Glyph_تھاں  *aslot )
  {
    FT_Error         error;
    FT_Driver        driver;
    FT_Driver_Class  clazz;
    FT_حافظہ        حافظہ;
    FT_Glyph_تھاں     تھاں = عدم;


    إذا ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );

    إذا ( !face->driver )
      أرجع FT_THROW( Invalid_دلیل );

    driver = face->driver;
    clazz  = driver->clazz;
    حافظہ = driver->جڑ.حافظہ;

    FT_TRACE4(( "FT_New_GlyphSlot: Creating new slot object\n" ));
    إذا ( !FT_ALLOC( تھاں, clazz->تھاں_object_ناپ ) )
    {
      تھاں->face = face;

      error = ft_glyphslot_init( تھاں );
      إذا ( error )
      {
        ft_glyphslot_پورا( تھاں );
        FT_FREE( تھاں );
        اذهب Exit;
      }

      تھاں->اگلا  = face->glyph;
      face->glyph = تھاں;

      إذا ( aslot )
        *aslot = تھاں;
    }
    وإلا إذا ( aslot )
      *aslot = عدم;


  Exit:
    FT_TRACE4(( "FT_New_GlyphSlot: Return 0x%x\n", error ));

    أرجع error;
  }


  /* documentation is in ftobjs.h */

  FT_ادھار_DEF( فراغ )
  FT_پورا_Glyph_تھاں( FT_Glyph_تھاں  تھاں )
  {
    إذا ( تھاں )
    {
      FT_Driver     driver = تھاں->face->driver;
      FT_حافظہ     حافظہ = driver->جڑ.حافظہ;
      FT_Glyph_تھاں  prev;
      FT_Glyph_تھاں  cur;


      /* Remove slot from its parent face's list */
      prev = عدم;
      cur  = تھاں->face->glyph;

      طالما ( cur )
      {
        إذا ( cur == تھاں )
        {
          إذا ( !prev )
            تھاں->face->glyph = cur->اگلا;
          وإلا
            prev->اگلا = cur->اگلا;

          /* finalize client-specific data */
          إذا ( تھاں->generic.finalizer )
            تھاں->generic.finalizer( تھاں );

          ft_glyphslot_پورا( تھاں );
          FT_FREE( تھاں );
          اكسر;
        }
        prev = cur;
        cur  = cur->اگلا;
      }
    }
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( فراغ )
  FT_سیٹ_Transform( FT_Face     face,
                    FT_میٹرکس*  میٹرکس,
                    FT_سمتیہ*  delta )
  {
    FT_Face_اندرونی  اندرونی;


    إذا ( !face )
      أرجع;

    اندرونی = face->اندرونی;

    اندرونی->transform_flags = 0;

    إذا ( !میٹرکس || !FT_میٹرکس_Check( میٹرکس ) )
    {
      اندرونی->transform_میٹرکس.xx = 0x10000L;
      اندرونی->transform_میٹرکس.xy = 0;
      اندرونی->transform_میٹرکس.yx = 0;
      اندرونی->transform_میٹرکس.yy = 0x10000L;

      میٹرکس = &اندرونی->transform_میٹرکس;
    }
    وإلا
      اندرونی->transform_میٹرکس = *میٹرکس;

    /* set transform_flags bit flag 0 if `matrix' isn't the identity */
    إذا ( ( میٹرکس->xy | میٹرکس->yx ) ||
         میٹرکس->xx != 0x10000L      ||
         میٹرکس->yy != 0x10000L      )
      اندرونی->transform_flags |= 1;

    إذا ( !delta )
    {
      اندرونی->transform_delta.x = 0;
      اندرونی->transform_delta.y = 0;

      delta = &اندرونی->transform_delta;
    }
    وإلا
      اندرونی->transform_delta = *delta;

    /* set transform_flags bit flag 1 if `delta' isn't the null vector */
    إذا ( delta->x | delta->y )
      اندرونی->transform_flags |= 2;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( فراغ )
  FT_Get_Transform( FT_Face     face,
                    FT_میٹرکس*  میٹرکس,
                    FT_سمتیہ*  delta )
  {
    FT_Face_اندرونی  اندرونی;


    إذا ( !face )
      أرجع;

    اندرونی = face->اندرونی;

    إذا ( میٹرکس )
      *میٹرکس = اندرونی->transform_میٹرکس;

    إذا ( delta )
      *delta = اندرونی->transform_delta;
  }


  ساكن FT_Renderer
  ft_lookup_glyph_renderer( FT_Glyph_تھاں  تھاں );


#جے_تعریف_شدہ GRID_FIT_METRICS
  ساكن فراغ
  ft_glyphslot_grid_fit_metrics( FT_Glyph_تھاں  تھاں,
                                 FT_Bool       vertical )
  {
    FT_Glyph_Metrics*  metrics = &تھاں->metrics;
    FT_Pos             دایاں, bottom;


    إذا ( vertical )
    {
      metrics->horiBearingX = FT_PIX_FLOOR( metrics->horiBearingX );
      metrics->horiBearingY = FT_PIX_CEIL_LONG( metrics->horiBearingY );

      دایاں  = FT_PIX_CEIL_LONG( جوڑو_LONG( metrics->vertBearingX,
                                           metrics->چوڑائی ) );
      bottom = FT_PIX_CEIL_LONG( جوڑو_LONG( metrics->vertBearingY,
                                           metrics->اچائی ) );

      metrics->vertBearingX = FT_PIX_FLOOR( metrics->vertBearingX );
      metrics->vertBearingY = FT_PIX_FLOOR( metrics->vertBearingY );

      metrics->چوڑائی  = SUB_LONG( دایاں,
                                  metrics->vertBearingX );
      metrics->اچائی = SUB_LONG( bottom,
                                  metrics->vertBearingY );
    }
    وإلا
    {
      metrics->vertBearingX = FT_PIX_FLOOR( metrics->vertBearingX );
      metrics->vertBearingY = FT_PIX_FLOOR( metrics->vertBearingY );

      دایاں  = FT_PIX_CEIL_LONG( جوڑو_LONG( metrics->horiBearingX,
                                           metrics->چوڑائی ) );
      bottom = FT_PIX_FLOOR( SUB_LONG( metrics->horiBearingY,
                                       metrics->اچائی ) );

      metrics->horiBearingX = FT_PIX_FLOOR( metrics->horiBearingX );
      metrics->horiBearingY = FT_PIX_CEIL_LONG( metrics->horiBearingY );

      metrics->چوڑائی  = SUB_LONG( دایاں,
                                  metrics->horiBearingX );
      metrics->اچائی = SUB_LONG( metrics->horiBearingY,
                                  bottom );
    }

    metrics->horiAdvance = FT_PIX_گیڑا_LONG( metrics->horiAdvance );
    metrics->vertAdvance = FT_PIX_گیڑا_LONG( metrics->vertAdvance );
  }
#جے_انت /* GRID_FIT_METRICS */


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_لدو_Glyph( FT_Face   face,
                 FT_UInt   glyph_اشاریہ,
                 FT_Int32  لدو_flags )
  {
    FT_Error      error;
    FT_Driver     driver;
    FT_Glyph_تھاں  تھاں;
    FT_Library    library;
    FT_Bool       autohint = FALSE;
    FT_Module     hinter;


    إذا ( !face || !face->ناپ || !face->glyph )
      أرجع FT_THROW( Invalid_Face_Handle );

    /* The validity test for `glyph_index' is performed by the */
    /* font drivers.                                           */

    تھاں = face->glyph;
    ft_glyphslot_clear( تھاں );

    driver  = face->driver;
    library = driver->جڑ.library;
    hinter  = library->auto_hinter;

    /* undefined scale means no scale */
    إذا ( face->ناپ->metrics.x_ppem == 0 ||
         face->ناپ->metrics.y_ppem == 0 )
      لدو_flags |= FT_لدو_NO_SCALE;

    /* resolve load flags dependencies */

    إذا ( لدو_flags & FT_لدو_NO_RECURSE )
      لدو_flags |= FT_لدو_NO_SCALE         |
                    FT_لدو_IGNORE_TRANSFORM;

    إذا ( لدو_flags & FT_لدو_NO_SCALE )
    {
      لدو_flags |= FT_لدو_NO_HINTING |
                    FT_لدو_NO_BITMAP;

      لدو_flags &= ~FT_لدو_RENDER;
    }

    إذا ( لدو_flags & FT_لدو_BITMAP_METRICS_ONLY )
      لدو_flags &= ~FT_لدو_RENDER;

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
         !( لدو_flags & FT_لدو_NO_HINTING )             &&
         !( لدو_flags & FT_لدو_NO_AUTOHINT )            &&
         FT_IS_SCALABLE( face )                           &&
         !FT_IS_TRICKY( face )                            &&
         ( ( لدو_flags & FT_لدو_IGNORE_TRANSFORM )    ||
           ( face->اندرونی->transform_میٹرکس.yx == 0 &&
             face->اندرونی->transform_میٹرکس.xx != 0 ) ||
           ( face->اندرونی->transform_میٹرکس.xx == 0 &&
             face->اندرونی->transform_میٹرکس.yx != 0 ) ) )
    {
      إذا ( ( لدو_flags & FT_لدو_FORCE_AUTOHINT ) ||
           !FT_DRIVER_HAS_HINTER( driver )         )
        autohint = TRUE;
      وإلا
      {
        FT_Render_منوال  منوال = FT_لدو_نشانہ_منوال( لدو_flags );
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
        إذا ( ( منوال == FT_RENDER_منوال_LIGHT           &&
               ( !FT_DRIVER_HINTS_LIGHTLY( driver ) &&
                 !is_light_type1                    ) )         ||
             ( FT_IS_SFNT( face )                             &&
               ttface->عدد_locations                          &&
               ttface->font_پروگرام_ناپ == 0                 &&
               ttface->cvt_پروگرام_ناپ <= 7                  ) )
          autohint = TRUE;
      }
    }

#جے_تعریف_شدہ TT_CONFIG_OPTION_VARC
    /* Never auto-hint a VARC (variable composite) glyph: its components */
    /* are already loaded unhinted, and the auto-fitter cannot process   */
    /* the assembled composite (it also re-enters variation-coordinate   */
    /* setting).  Use the native hinter / unhinted load instead.         */
    إذا ( autohint && FT_IS_SFNT( face ) )
    {
      FT_Service_VARC  varc;


      FT_FACE_لبھو_SERVICE( face, varc, VARC );
      إذا ( varc && varc->has_glyph( face, glyph_اشاریہ ) )
        autohint = FALSE;
    }
#جے_انت

    إذا ( autohint )
    {
      FT_AutoHinter_Interface  hinting;


      /* XXX: The use of the `FT_LOAD_XXX_ONLY` flags is not very */
      /*      elegant.                                            */

      /* try to load SVG documents if available */
      إذا ( ( لدو_flags & FT_لدو_NO_SVG ) == 0 &&
           FT_HAS_SVG( face )                   )
      {
        error = driver->clazz->لدو_glyph( تھاں, face->ناپ,
                                           glyph_اشاریہ,
                                           لدو_flags | FT_لدو_SVG_ONLY );

        إذا ( !error && تھاں->format == FT_GLYPH_FORMAT_SVG )
          اذهب لدو_Ok;
      }

      /* try to load embedded bitmaps if available */
      إذا ( FT_HAS_FIXED_SIZES( face )              &&
           ( لدو_flags & FT_لدو_NO_BITMAP ) == 0 )
      {
        error = driver->clazz->لدو_glyph( تھاں, face->ناپ,
                                           glyph_اشاریہ,
                                           لدو_flags | FT_لدو_SBITS_ONLY );

        إذا ( !error && تھاں->format == FT_GLYPH_FORMAT_BITMAP )
          اذهب لدو_Ok;
      }

      {
        FT_Face_اندرونی  اندرونی        = face->اندرونی;
        FT_Int            transform_flags = اندرونی->transform_flags;


        /* since the auto-hinter calls FT_Load_Glyph by itself, */
        /* make sure that glyphs aren't transformed             */
        اندرونی->transform_flags = 0;

        /* load auto-hinted outline */
        hinting = (FT_AutoHinter_Interface)hinter->clazz->module_interface;

        error   = hinting->لدو_glyph( (FT_AutoHinter)hinter,
                                       تھاں, face->ناپ,
                                       glyph_اشاریہ, لدو_flags );

        اندرونی->transform_flags = transform_flags;
      }
    }
    وإلا
    {
      error = driver->clazz->لدو_glyph( تھاں,
                                         face->ناپ,
                                         glyph_اشاریہ,
                                         لدو_flags );
      إذا ( error )
        اذهب Exit;

      إذا ( تھاں->format == FT_GLYPH_FORMAT_OUTLINE )
      {
        /* check that the loaded outline is correct */
        error = FT_Outline_Check( &تھاں->outline );
        إذا ( error )
          اذهب Exit;

#جے_تعریف_شدہ GRID_FIT_METRICS
        إذا ( !( لدو_flags & FT_لدو_NO_HINTING ) )
          ft_glyphslot_grid_fit_metrics(
            تھاں,
            FT_BOOL( لدو_flags & FT_لدو_VERTICAL_LAYOUT ) );
#جے_انت
      }
    }

  لدو_Ok:
    /* compute the advance */
    إذا ( لدو_flags & FT_لدو_VERTICAL_LAYOUT )
    {
      تھاں->advance.x = 0;
      تھاں->advance.y = تھاں->metrics.vertAdvance;
    }
    وإلا
    {
      تھاں->advance.x = تھاں->metrics.horiAdvance;
      تھاں->advance.y = 0;
    }

    /* compute the linear advance in 16.16 pixels */
    إذا ( ( لدو_flags & FT_لدو_خطی_DESIGN ) == 0 &&
         FT_IS_SCALABLE( face )                      )
    {
      FT_ناپ_Metrics*  metrics = &face->ناپ->metrics;


      /* it's tricky! */
      تھاں->خطی_Hori_Advance = FT_MulDiv( تھاں->خطی_Hori_Advance,
                                           metrics->x_scale, 64 );

      تھاں->خطی_Vert_Advance = FT_MulDiv( تھاں->خطی_Vert_Advance,
                                           metrics->y_scale, 64 );
    }

    إذا ( ( لدو_flags & FT_لدو_IGNORE_TRANSFORM ) == 0 )
    {
      FT_Face_اندرونی  اندرونی = face->اندرونی;


      /* now, transform the glyph image if needed */
      إذا ( اندرونی->transform_flags )
      {
        /* get renderer */
        FT_Renderer  renderer = ft_lookup_glyph_renderer( تھاں );


        إذا ( renderer )
          error = renderer->clazz->transform_glyph(
                                     renderer, تھاں,
                                     &اندرونی->transform_میٹرکس,
                                     &اندرونی->transform_delta );
        وإلا إذا ( تھاں->format == FT_GLYPH_FORMAT_OUTLINE )
        {
          /* apply `standard' transformation if no renderer is available */
          إذا ( اندرونی->transform_flags & 1 )
            FT_Outline_Transform( &تھاں->outline,
                                  &اندرونی->transform_میٹرکس );

          إذا ( اندرونی->transform_flags & 2 )
            FT_Outline_Translate( &تھاں->outline,
                                  اندرونی->transform_delta.x,
                                  اندرونی->transform_delta.y );
        }

        /* transform advance */
        FT_سمتیہ_Transform( &تھاں->advance, &اندرونی->transform_میٹرکس );
      }
    }

    تھاں->glyph_اشاریہ          = glyph_اشاریہ;
    تھاں->اندرونی->لدو_flags = لدو_flags;

    /* do we need to render the image or preset the bitmap now? */
    إذا ( !error                                    &&
         ( لدو_flags & FT_لدو_NO_SCALE ) == 0    &&
         تھاں->format != FT_GLYPH_FORMAT_BITMAP    &&
         تھاں->format != FT_GLYPH_FORMAT_مرکب )
    {
      FT_Render_منوال  منوال = FT_لدو_نشانہ_منوال( لدو_flags );


      إذا ( منوال == FT_RENDER_منوال_عام   &&
           لدو_flags & FT_لدو_MONOCHROME )
        منوال = FT_RENDER_منوال_MONO;

      إذا ( لدو_flags & FT_لدو_RENDER )
        error = FT_Render_Glyph( تھاں, منوال );
      وإلا
        ft_glyphslot_preset_bitmap( تھاں, منوال, عدم );
    }

#جے_تعریف_شدہ FT_DEBUG_LEVEL_TRACE
    FT_TRACE5(( "FT_Load_Glyph: index %u, flags 0x%x\n",
                glyph_اشاریہ, لدو_flags ));
    FT_TRACE5(( "  bitmap %ux%u %s, %s (mode %d)\n",
                تھاں->bitmap.چوڑائی,
                تھاں->bitmap.صفاں,
                تھاں->outline.points ?
                  تھاں->bitmap.بفر ? "rendered"
                                      : "preset"
                                     :
                  تھاں->اندرونی->flags & FT_GLYPH_OWN_BITMAP ? "owned"
                                                              : "unowned",
                pixel_modes[تھاں->bitmap.pixel_منوال],
                تھاں->bitmap.pixel_منوال ));
    FT_TRACE5(( "\n" ));
    FT_TRACE5(( "  x advance: %f\n", (مزدوج)تھاں->advance.x / 64 ));
    FT_TRACE5(( "  y advance: %f\n", (مزدوج)تھاں->advance.y / 64 ));
    FT_TRACE5(( "  linear x advance: %f\n",
                (مزدوج)تھاں->خطی_Hori_Advance / 65536 ));
    FT_TRACE5(( "  linear y advance: %f\n",
                (مزدوج)تھاں->خطی_Vert_Advance / 65536 ));

    {
      FT_Glyph_Metrics*  metrics = &تھاں->metrics;


      FT_TRACE5(( "  metrics:\n" ));
      FT_TRACE5(( "    width:  %f\n", (مزدوج)metrics->چوڑائی / 64 ));
      FT_TRACE5(( "    height: %f\n", (مزدوج)metrics->اچائی / 64 ));
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
#جے_انت

  Exit:
    أرجع error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_لدو_Char( FT_Face   face,
                FT_ULong  char_کوڈ,
                FT_Int32  لدو_flags )
  {
    FT_UInt  glyph_اشاریہ;


    إذا ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );

    glyph_اشاریہ = (FT_UInt)char_کوڈ;
    إذا ( face->charmap )
      glyph_اشاریہ = FT_Get_Char_اشاریہ( face, char_کوڈ );

    أرجع FT_لدو_Glyph( face, glyph_اشاریہ, لدو_flags );
  }


  /* destructor for sizes list */
  ساكن فراغ
  destroy_ناپ( FT_حافظہ  حافظہ,
                فراغ*      ناپ_2,
                فراغ*      driver_ )
  {
    FT_ناپ    ناپ   = (FT_ناپ)ناپ_2;
    FT_Driver  driver = (FT_Driver)driver_;


    /* finalize format-specific stuff */
    إذا ( driver->clazz->پورا_ناپ )
      driver->clazz->پورا_ناپ( ناپ );

    /* finalize client-specific data */
    إذا ( ناپ->generic.finalizer )
      ناپ->generic.finalizer( ناپ );

    FT_FREE( ناپ->اندرونی );
    FT_FREE( ناپ );
  }


  ساكن فراغ
  ft_cmap_پورا_اندرونی( FT_CMap  cmap );


  ساكن فراغ
  destroy_charmaps( FT_Face    face,
                    FT_حافظہ  حافظہ )
  {
    FT_Int  n;


    إذا ( !face )
      أرجع;

    لكل ( n = 0; n < face->عدد_charmaps; n++ )
    {
      FT_CMap  cmap = FT_CMAP( face->charmaps[n] );


      ft_cmap_پورا_اندرونی( cmap );

      face->charmaps[n] = عدم;
    }

    FT_FREE( face->charmaps );
    face->عدد_charmaps = 0;
  }


  /* destructor for faces list */
  ساكن فراغ
  destroy_face( FT_حافظہ  حافظہ,
                فراغ*      face_,
                فراغ*      driver_ )
  {
    FT_Face          face   = (FT_Face)face_;
    FT_Driver        driver = (FT_Driver)driver_;
    FT_Driver_Class  clazz  = driver->clazz;


    /* discard auto-hinting data */
    إذا ( face->autohint.finalizer )
      face->autohint.finalizer( face->autohint.انکڑے );

    /* Discard glyph slots for this face.                           */
    /* Beware!  FT_Done_GlyphSlot() changes the field `face->glyph' */
    طالما ( face->glyph )
      FT_پورا_Glyph_تھاں( face->glyph );

    /* discard all sizes for this face */
    FT_List_Finalize( &face->sizes_list,
                      destroy_ناپ,
                      حافظہ,
                      driver );
    face->ناپ = عدم;

    /* discard charmaps */
    destroy_charmaps( face, حافظہ );

    /* finalize format-specific stuff */
    إذا ( clazz->پورا_face )
      clazz->پورا_face( face );

    /* close the stream for this face if needed */
    FT_دھارا_Free(
      face->دھارا,
      ( face->face_flags & FT_FACE_FLAG_EXTERNAL_دھارا ) != 0 );

    face->دھارا = عدم;

    /* now discard client data */
    إذا ( face->generic.finalizer )
      face->generic.finalizer( face );

    /* get rid of it */
    إذا ( face->اندرونی )
    {
      FT_FREE( face->اندرونی );
    }
    FT_FREE( face );
  }


  ساكن فراغ
  Destroy_Driver( FT_Driver  driver )
  {
    FT_List_Finalize( &driver->faces_list,
                      destroy_face,
                      driver->جڑ.حافظہ,
                      driver );
  }


  /* documentation is in ftobjs.h */

  FT_ادھار_DEF( FT_Error )
  لبھو_unicode_charmap( FT_Face  face )
  {
    FT_CharMap*  پہلا;
    FT_CharMap*  cur;


    /* caller should have already checked that `face' is valid */
    FT_ASSERT( face );

    پہلا = face->charmaps;

    إذا ( !پہلا )
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

    cur = پہلا + face->عدد_charmaps;  /* points after the last one */

    لكل ( ; --cur >= پہلا; )
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
               cur[0]->encoding_id == TT_APPLE_ID_بھریا_UNICODE  &&
               FT_Get_CMap_Format( cur[0] ) == 13               ) )
        {
          face->charmap = cur[0];
          أرجع FT_Err_Ok;
        }
      }
    }

    /* We do not have any UCS-4 charmap.                */
    /* Do the loop again and search for UCS-2 charmaps. */
    cur = پہلا + face->عدد_charmaps;

    لكل ( ; --cur >= پہلا; )
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
  لبھو_variant_selector_charmap( FT_Face  face )
  {
    FT_CharMap*  پہلا;
    FT_CharMap*  سرا;
    FT_CharMap*  cur;


    /* caller should have already checked that `face' is valid */
    FT_ASSERT( face );

    پہلا = face->charmaps;

    إذا ( !پہلا )
      أرجع عدم;

    سرا = پہلا + face->عدد_charmaps;  /* points after the last one */

    لكل ( cur = پہلا; cur < سرا; cur++ )
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
             FT_دھارا      *astream,
             FT_Bool        *anexternal_دھارا,
             FT_Long        face_اشاریہ,
             FT_Int         عدد_params,
             FT_Parameter*  params,
             FT_Face       *aface )
  {
    FT_حافظہ         حافظہ;
    FT_Driver_Class   clazz;
    FT_Face           face     = عدم;
    FT_Face_اندرونی  اندرونی = عدم;

    FT_Error          error, error2;


    clazz  = driver->clazz;
    حافظہ = driver->جڑ.حافظہ;

    /* allocate the face object and perform basic initialization */
    إذا ( FT_ALLOC( face, clazz->face_object_ناپ ) )
      اذهب Fail;

    face->driver = driver;
    face->حافظہ = حافظہ;
    face->دھارا = *astream;

    /* set the FT_FACE_FLAG_EXTERNAL_STREAM bit for FT_Done_Face */
    إذا ( *anexternal_دھارا )
      face->face_flags |= FT_FACE_FLAG_EXTERNAL_دھارا;

    إذا ( FT_NEW( اندرونی ) )
      اذهب Fail;

    face->اندرونی = اندرونی;

#جے_تعریف_شدہ FT_CONFIG_OPTION_INCREMENTAL
    {
      صحيح  i;


      face->اندرونی->incremental_interface = عدم;
      لكل ( i = 0; i < عدد_params && !face->اندرونی->incremental_interface;
            i++ )
        إذا ( params[i].ٹیگ == FT_PARAM_ٹیگ_INCREMENTAL )
          face->اندرونی->incremental_interface =
            (FT_Incremental_Interface)params[i].انکڑے;
    }
#جے_انت

    face->اندرونی->random_بیج = -1;

    إذا ( clazz->init_face )
      error = clazz->init_face( *astream,
                                face,
                                (FT_Int)face_اشاریہ,
                                عدد_params,
                                params );
    /* Stream may have been changed. */
    *astream = face->دھارا;
    *anexternal_دھارا =
      ( face->face_flags & FT_FACE_FLAG_EXTERNAL_دھارا ) != 0;
    إذا ( error )
      اذهب Fail;

    /* select Unicode charmap by default */
    error2 = لبھو_unicode_charmap( face );

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
      destroy_charmaps( face, حافظہ );
      إذا ( clazz->پورا_face )
        clazz->پورا_face( face );
      FT_FREE( اندرونی );
      FT_FREE( face );
      *aface = عدم;
    }

    أرجع error;
  }


  /* there's a Mac-specific extended implementation of FT_New_Face() */
  /* in src/base/ftmac.c                                             */

#جے_بنا_تعریف FT_MACINTOSH

  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_New_Face( FT_Library   library,
               ثابت حرف*  pathname,
               FT_Long      face_اشاریہ,
               FT_Face     *aface )
  {
    FT_Open_دلیلاں  دلیلاں;


    /* test for valid `library' and `aface' delayed to `FT_Open_Face' */
    إذا ( !pathname )
      أرجع FT_THROW( Invalid_دلیل );

    دلیلاں.flags    = FT_OPEN_PATHNAME;
    دلیلاں.pathname = (حرف*)pathname;
    دلیلاں.دھارا   = عدم;

    أرجع ft_open_face_اندرونی( library, &دلیلاں, face_اشاریہ, aface, 1 );
  }

#جے_انت


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_New_حافظہ_Face( FT_Library      library,
                      ثابت FT_Byte*  file_ادھار,
                      FT_Long         file_ناپ,
                      FT_Long         face_اشاریہ,
                      FT_Face        *aface )
  {
    FT_Open_دلیلاں  دلیلاں;


    /* test for valid `library' and `face' delayed to `FT_Open_Face' */
    إذا ( !file_ادھار )
      أرجع FT_THROW( Invalid_دلیل );

    دلیلاں.flags       = FT_OPEN_حافظہ;
    دلیلاں.حافظہ_ادھار = file_ادھار;
    دلیلاں.حافظہ_ناپ = file_ناپ;
    دلیلاں.دھارا      = عدم;

    أرجع ft_open_face_اندرونی( library, &دلیلاں, face_اشاریہ, aface, 1 );
  }


#جے_تعریف_شدہ FT_CONFIG_OPTION_MAC_FONTS

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
  حافظہ_دھارا_close( FT_دھارا  دھارا )
  {
    FT_حافظہ  حافظہ = (FT_حافظہ)دھارا->descriptor.اشارہ_گر;


    FT_FREE( دھارا->ادھار );
    دھارا->ناپ  = 0;
    دھارا->close = عدم;
    FT_FREE( دھارا );
  }


  /* Create a new memory stream from a buffer and a size. */
  /* From `ftmac.c'.                                      */
  ساكن FT_Error
  new_حافظہ_دھارا( FT_Library           library,
                     FT_Byte*             ادھار,
                     FT_ULong             ناپ,
                     FT_دھارا_Close_Func  close,
                     FT_دھارا           *astream )
  {
    FT_Error   error;
    FT_حافظہ  حافظہ;
    FT_دھارا  دھارا = عدم;


    إذا ( !library )
      أرجع FT_THROW( Invalid_Library_Handle );

    إذا ( !ادھار )
      أرجع FT_THROW( Invalid_دلیل );

    *astream = عدم;
    حافظہ   = library->حافظہ;
    إذا ( FT_NEW( دھارا ) )
      اذهب Exit;

    FT_دھارا_Open_حافظہ( دھارا, ادھار, ناپ );

    دھارا->descriptor.اشارہ_گر = حافظہ;
    دھارا->close              = close;

    *astream = دھارا;

  Exit:
    أرجع error;
  }


  /* Create a new FT_Face given a buffer and a driver name. */
  /* From `ftmac.c'.                                        */
  FT_LOCAL_DEF( FT_Error )
  open_face_from_بفر( FT_Library   library,
                         FT_Byte*     ادھار,
                         FT_ULong     ناپ,
                         FT_Long      face_اشاریہ,
                         ثابت حرف*  driver_name,
                         FT_Face     *aface )
  {
    FT_Open_دلیلاں  دلیلاں;
    FT_Error      error;
    FT_حافظہ     حافظہ = library->حافظہ;


    دلیلاں.driver = عدم;
    دلیلاں.flags  = 0;

    إذا ( driver_name )
    {
      دلیلاں.driver = FT_Get_Module( library, driver_name );
      إذا ( !دلیلاں.driver )
      {
        FT_FREE( ادھار );
        أرجع FT_THROW( Missing_Module );
      }

      دلیلاں.flags = دلیلاں.flags | FT_OPEN_DRIVER;
    }

    /* `memory_stream_close` also frees the stream object. */
    error = new_حافظہ_دھارا( library,
                               ادھار,
                               ناپ,
                               حافظہ_دھارا_close,
                               &دلیلاں.دھارا );
    إذا ( error )
    {
      FT_FREE( ادھار );
      أرجع error;
    }

    دلیلاں.flags |= FT_OPEN_دھارا;

#جے_تعریف_شدہ FT_MACINTOSH
    /* At this point, the face index has served its purpose;  */
    /* whoever calls this function has already used it to     */
    /* locate the correct font data.  We should not propagate */
    /* this index to FT_Open_Face() (unless it is negative).  */

    إذا ( face_اشاریہ > 0 )
      face_اشاریہ &= 0x7FFF0000L; /* retain GX data */
#جے_انت

    أرجع ft_open_face_اندرونی( library, &دلیلاں, face_اشاریہ, aface, 0 );
  }


  /* Look up `TYP1' or `CID ' table from sfnt table directory.       */
  /* `offset' and `length' must exclude the binary header in tables. */

  /* Type 1 and CID-keyed font drivers should recognize sfnt-wrapped */
  /* format too.  Here, since we can't expect that the TrueType font */
  /* driver is loaded unconditionally, we must parse the font by     */
  /* ourselves.  We are only interested in the name of the table and */
  /* the offset.                                                     */

  ساكن FT_Error
  ft_lookup_PS_وچ_sfnt_دھارا( FT_دھارا  دھارا,
                               FT_Long    face_اشاریہ,
                               FT_ULong*  وتھ,
                               FT_ULong*  لمائی,
                               FT_Bool*   is_sfnt_cid )
  {
    FT_Error   error;
    FT_UShort  عدد_Tables;
    FT_Long    pstable_اشاریہ;
    FT_ULong   ٹیگ;
    صحيح        i;


    *وتھ = 0;
    *لمائی = 0;
    *is_sfnt_cid = FALSE;

    /* TODO: support for sfnt-wrapped PS/CID in TTC format */

    /* version check for 'typ1' (should be ignored?) */
    إذا ( FT_READ_ULONG( ٹیگ ) )
      أرجع error;
    إذا ( ٹیگ != TTAG_typ1 )
      أرجع FT_THROW( Unknown_File_Format );

    إذا ( FT_READ_USHORT( عدد_Tables ) )
      أرجع error;
    إذا ( FT_دھارا_SKIP( 2 * 3 ) ) /* skip binary search header */
      أرجع error;

    pstable_اشاریہ = -1;
    *is_sfnt_cid  = FALSE;

    لكل ( i = 0; i < عدد_Tables; i++ )
    {
      إذا ( FT_READ_ULONG( ٹیگ )     || FT_دھارا_SKIP( 4 )      ||
           FT_READ_ULONG( *وتھ ) || FT_READ_ULONG( *لمائی ) )
        أرجع error;

      إذا ( ٹیگ == TTAG_CID )
      {
        pstable_اشاریہ++;
        *وتھ += 22;
        *لمائی -= 22;
        *is_sfnt_cid = TRUE;
        إذا ( face_اشاریہ < 0 )
          أرجع FT_Err_Ok;
      }
      وإلا إذا ( ٹیگ == TTAG_TYP1 )
      {
        pstable_اشاریہ++;
        *وتھ += 24;
        *لمائی -= 24;
        *is_sfnt_cid = FALSE;
        إذا ( face_اشاریہ < 0 )
          أرجع FT_Err_Ok;
      }
      إذا ( face_اشاریہ >= 0 && pstable_اشاریہ == face_اشاریہ )
        أرجع FT_Err_Ok;
    }

    أرجع FT_THROW( جدول_Missing );
  }


  FT_LOCAL_DEF( FT_Error )
  open_face_PS_from_sfnt_دھارا( FT_Library     library,
                                 FT_دھارا      دھارا,
                                 FT_Long        face_اشاریہ,
                                 FT_Int         عدد_params,
                                 FT_Parameter  *params,
                                 FT_Face       *aface )
  {
    FT_Error   error;
    FT_حافظہ  حافظہ = library->حافظہ;
    FT_ULong   وتھ, لمائی;
    FT_ULong   pos;
    FT_Bool    is_sfnt_cid;
    FT_Byte*   sfnt_ps = عدم;

    FT_بن_ورتیا( عدد_params );
    FT_بن_ورتیا( params );


    /* ignore GX stuff */
    إذا ( face_اشاریہ > 0 )
      face_اشاریہ &= 0xFFFFL;

    pos = FT_دھارا_POS();

    error = ft_lookup_PS_وچ_sfnt_دھارا( دھارا,
                                         face_اشاریہ,
                                         &وتھ,
                                         &لمائی,
                                         &is_sfnt_cid );
    إذا ( error )
      اذهب Exit;

    إذا ( وتھ > دھارا->ناپ )
    {
      FT_TRACE2(( "open_face_PS_from_sfnt_stream: invalid table offset\n" ));
      error = FT_THROW( Invalid_جدول );
      اذهب Exit;
    }
    وإلا إذا ( لمائی > دھارا->ناپ - وتھ )
    {
      FT_TRACE2(( "open_face_PS_from_sfnt_stream: invalid table length\n" ));
      error = FT_THROW( Invalid_جدول );
      اذهب Exit;
    }

    error = FT_دھارا_Seek( دھارا, pos + وتھ );
    إذا ( error )
      اذهب Exit;

    إذا ( FT_QALLOC( sfnt_ps, (FT_Long)لمائی ) )
      اذهب Exit;

    error = FT_دھارا_Read( دھارا, (FT_Byte *)sfnt_ps, لمائی );
    إذا ( error )
    {
      FT_FREE( sfnt_ps );
      اذهب Exit;
    }

    error = open_face_from_بفر( library,
                                   sfnt_ps,
                                   لمائی,
                                   FT_MIN( face_اشاریہ, 0 ),
                                   is_sfnt_cid ? "t1cid" : "type1",
                                   aface );
  Exit:
    {
      FT_Error  error1;


      إذا ( FT_ERR_EQ( error, Unknown_File_Format ) )
      {
        error1 = FT_دھارا_Seek( دھارا, pos );
        إذا ( error1 )
          أرجع error1;
      }

      أرجع error;
    }
  }


#جے_بنا_تعریف FT_MACINTOSH

  /* The resource header says we've got resource_cnt `POST' (type1) */
  /* resources in this file.  They all need to be coalesced into    */
  /* one lump which gets passed on to the type1 driver.             */
  /* Here can be only one PostScript font in a file so face_index   */
  /* must be 0 (or -1).                                             */
  /*                                                                */
  ساكن FT_Error
  Mac_Read_POST_Resource( FT_Library  library,
                          FT_دھارا   دھارا,
                          FT_Long    *offsets,
                          FT_Long     resource_cnt,
                          FT_Long     face_اشاریہ,
                          FT_Face    *aface )
  {
    FT_Error   error  = FT_ERR( Cannot_Open_Resource );
    FT_حافظہ  حافظہ = library->حافظہ;

    FT_Byte*   pfb_انکڑے = عدم;
    صحيح        i, type, flags;
    FT_ULong   len;
    FT_ULong   pfb_len, pfb_pos, pfb_lenpos;
    FT_ULong   rlen, عارضی;


    إذا ( face_اشاریہ == -1 )
      face_اشاریہ = 0;
    إذا ( face_اشاریہ != 0 )
      أرجع error;

    /* Find the length of all the POST resources, concatenated.  Assume */
    /* worst case (each resource in its own section).                   */
    pfb_len = 0;
    لكل ( i = 0; i < resource_cnt; i++ )
    {
      error = FT_دھارا_Seek( دھارا, (FT_ULong)offsets[i] );
      إذا ( error )
        اذهب Exit;
      إذا ( FT_READ_ULONG( عارضی ) )  /* actually LONG */
        اذهب Exit;

      /* FT2 allocator takes signed long buffer length,
       * too large value causing overflow should be checked
       */
      FT_TRACE4(( "                 POST fragment #%d: length=0x%08lx"
                  " total pfb_len=0x%08lx\n",
                  i, عارضی, pfb_len + عارضی + 6 ));

      إذا ( FT_MAC_RFORK_MAX_LEN < عارضی               ||
           FT_MAC_RFORK_MAX_LEN - عارضی < pfb_len + 6 )
      {
        FT_TRACE2(( "             MacOS resource length cannot exceed"
                    " 0x%08lx\n",
                    FT_MAC_RFORK_MAX_LEN ));

        error = FT_THROW( Invalid_وتھ );
        اذهب Exit;
      }

      pfb_len += عارضی + 6;
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

    إذا ( FT_QALLOC( pfb_انکڑے, (FT_Long)pfb_len + 2 ) )
      اذهب Exit;

    pfb_انکڑے[0] = 0x80;
    pfb_انکڑے[1] = 1;            /* Ascii section */
    pfb_انکڑے[2] = 0;            /* 4-byte length, fill in later */
    pfb_انکڑے[3] = 0;
    pfb_انکڑے[4] = 0;
    pfb_انکڑے[5] = 0;
    pfb_pos     = 6;
    pfb_lenpos  = 2;

    len  = 0;
    type = 1;

    لكل ( i = 0; i < resource_cnt; i++ )
    {
      error = FT_دھارا_Seek( دھارا, (FT_ULong)offsets[i] );
      إذا ( error )
        اذهب Exit2;
      إذا ( FT_READ_ULONG( rlen ) )
        اذهب Exit2;

      /* FT2 allocator takes signed long buffer length,
       * too large fragment length causing overflow should be checked
       */
      إذا ( 0x7FFFFFFFUL < rlen )
      {
        error = FT_THROW( Invalid_وتھ );
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
                    i, (فراغ*)pfb_انکڑے, pfb_lenpos ));

        إذا ( pfb_lenpos + 3 > pfb_len + 2 )
          اذهب Exit2;

        pfb_انکڑے[pfb_lenpos    ] = (FT_Byte)( len );
        pfb_انکڑے[pfb_lenpos + 1] = (FT_Byte)( len >> 8 );
        pfb_انکڑے[pfb_lenpos + 2] = (FT_Byte)( len >> 16 );
        pfb_انکڑے[pfb_lenpos + 3] = (FT_Byte)( len >> 24 );

        إذا ( ( flags >> 8 ) == 5 )      /* End of font mark */
          اكسر;

        FT_TRACE3(( "    Write POST fragment #%d header (6-byte) to buffer"
                    " %p + 0x%08lx\n",
                    i, (فراغ*)pfb_انکڑے, pfb_pos ));

        إذا ( pfb_pos + 6 > pfb_len + 2 )
          اذهب Exit2;

        pfb_انکڑے[pfb_pos++] = 0x80;

        type = flags >> 8;
        len  = rlen;

        pfb_انکڑے[pfb_pos++] = (FT_Byte)type;
        pfb_lenpos          = pfb_pos;
        pfb_انکڑے[pfb_pos++] = 0;        /* 4-byte length, fill in later */
        pfb_انکڑے[pfb_pos++] = 0;
        pfb_انکڑے[pfb_pos++] = 0;
        pfb_انکڑے[pfb_pos++] = 0;
      }

      إذا ( pfb_pos > pfb_len || pfb_pos + rlen > pfb_len )
        اذهب Exit2;

      FT_TRACE3(( "    Load POST fragment #%d (%lu byte) to buffer"
                  " %p + 0x%08lx\n",
                  i, rlen, (فراغ*)pfb_انکڑے, pfb_pos ));

      error = FT_دھارا_Read( دھارا, (FT_Byte *)pfb_انکڑے + pfb_pos, rlen );
      إذا ( error )
        اذهب Exit2;

      pfb_pos += rlen;
    }

    error = FT_ERR( Array_Too_Large );

    إذا ( pfb_pos + 2 > pfb_len + 2 )
      اذهب Exit2;
    pfb_انکڑے[pfb_pos++] = 0x80;
    pfb_انکڑے[pfb_pos++] = 3;

    إذا ( pfb_lenpos + 3 > pfb_len + 2 )
      اذهب Exit2;
    pfb_انکڑے[pfb_lenpos    ] = (FT_Byte)( len );
    pfb_انکڑے[pfb_lenpos + 1] = (FT_Byte)( len >> 8 );
    pfb_انکڑے[pfb_lenpos + 2] = (FT_Byte)( len >> 16 );
    pfb_انکڑے[pfb_lenpos + 3] = (FT_Byte)( len >> 24 );

    أرجع open_face_from_بفر( library,
                                  pfb_انکڑے,
                                  pfb_pos,
                                  face_اشاریہ,
                                  "type1",
                                  aface );

  Exit2:
    إذا ( FT_ERR_EQ( error, Array_Too_Large ) )
      FT_TRACE2(( "  Abort due to too-short buffer to store"
                  " all POST fragments\n" ));
    وإلا إذا ( FT_ERR_EQ( error, Invalid_وتھ ) )
      FT_TRACE2(( "  Abort due to invalid offset in a POST fragment\n" ));

    إذا ( error )
      error = FT_ERR( Cannot_Open_Resource );
    FT_FREE( pfb_انکڑے );

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
                          FT_دھارا   دھارا,
                          FT_Long    *offsets,
                          FT_Long     resource_cnt,
                          FT_Long     face_اشاریہ,
                          FT_Face    *aface )
  {
    FT_حافظہ  حافظہ = library->حافظہ;
    FT_Byte*   sfnt_انکڑے = عدم;
    FT_Error   error;
    FT_ULong   flag_وتھ;
    FT_ULong   rlen;
    صحيح        is_cff;
    FT_Long    face_اشاریہ_وچ_resource = 0;


    إذا ( face_اشاریہ < 0 )
      face_اشاریہ = -face_اشاریہ - 1;
    إذا ( face_اشاریہ >= resource_cnt )
      أرجع FT_THROW( Cannot_Open_Resource );

    flag_وتھ = (FT_ULong)offsets[face_اشاریہ];
    error = FT_دھارا_Seek( دھارا, flag_وتھ );
    إذا ( error )
      اذهب Exit;

    إذا ( FT_READ_ULONG( rlen ) )
      اذهب Exit;
    إذا ( !rlen )
      أرجع FT_THROW( Cannot_Open_Resource );
    إذا ( rlen > FT_MAC_RFORK_MAX_LEN )
      أرجع FT_THROW( Invalid_وتھ );

    error = open_face_PS_from_sfnt_دھارا( library,
                                           دھارا,
                                           face_اشاریہ,
                                           0, عدم,
                                           aface );
    إذا ( !error )
      اذهب Exit;

    /* rewind sfnt stream before open_face_PS_from_sfnt_stream() */
    error = FT_دھارا_Seek( دھارا, flag_وتھ + 4 );
    إذا ( error )
      اذهب Exit;

    إذا ( FT_QALLOC( sfnt_انکڑے, rlen ) )
      أرجع error;
    error = FT_دھارا_Read( دھارا, (FT_Byte *)sfnt_انکڑے, rlen );
    إذا ( error )
    {
      FT_FREE( sfnt_انکڑے );
      اذهب Exit;
    }

    is_cff = rlen > 4 && !ft_memcmp( sfnt_انکڑے, "OTTO", 4 );
    error = open_face_from_بفر( library,
                                   sfnt_انکڑے,
                                   rlen,
                                   face_اشاریہ_وچ_resource,
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
                 FT_دھارا   دھارا,
                 FT_Long     resource_وتھ,
                 FT_Long     face_اشاریہ,
                 FT_Face    *aface )
  {
    FT_حافظہ  حافظہ = library->حافظہ;
    FT_Error   error;
    FT_Long    map_وتھ, rdata_pos;
    FT_Long    *انکڑے_offsets;
    FT_Long    گنتی;


    error = FT_Raccess_Get_سرخی_Info( library, دھارا, resource_وتھ,
                                       &map_وتھ, &rdata_pos );
    إذا ( error )
      أرجع error;

    /* POST resources must be sorted to concatenate properly */
    error = FT_Raccess_Get_انکڑے_Offsets( library, دھارا,
                                        map_وتھ, rdata_pos,
                                        TTAG_POST, TRUE,
                                        &انکڑے_offsets, &گنتی );
    إذا ( !error )
    {
      error = Mac_Read_POST_Resource( library, دھارا, انکڑے_offsets, گنتی,
                                      face_اشاریہ, aface );
      FT_FREE( انکڑے_offsets );
      /* POST exists in an LWFN providing a single face */
      إذا ( !error )
        (*aface)->عدد_faces = 1;
      أرجع error;
    }

    /* sfnt resources should not be sorted to preserve the face order by
       QuickDraw API */
    error = FT_Raccess_Get_انکڑے_Offsets( library, دھارا,
                                        map_وتھ, rdata_pos,
                                        TTAG_sfnt, FALSE,
                                        &انکڑے_offsets, &گنتی );
    إذا ( !error )
    {
      FT_Long  face_اشاریہ_اندرونی = face_اشاریہ % گنتی;


      error = Mac_Read_sfnt_Resource( library, دھارا, انکڑے_offsets, گنتی,
                                      face_اشاریہ_اندرونی, aface );
      FT_FREE( انکڑے_offsets );
      إذا ( !error )
      {
        (*aface)->عدد_faces  = گنتی;
        (*aface)->face_اشاریہ = face_اشاریہ_اندرونی;
      }
    }

    أرجع error;
  }


  /* Check for a valid macbinary header, and if we find one   */
  /* check that the (flattened) resource fork in it is valid. */
  /*                                                          */
  ساكن FT_Error
  Is_Mac_ثنائی( FT_Library  library,
               FT_دھارا   دھارا,
               FT_Long     face_اشاریہ,
               FT_Face    *aface )
  {
    بنانشانی حرف  سرخی[128];
    FT_Error       error;
    FT_Long        dlen, وتھ;


    إذا ( !دھارا )
      أرجع FT_THROW( Invalid_دھارا_Operation );

    error = FT_دھارا_Seek( دھارا, 0 );
    إذا ( error )
      اذهب Exit;

    error = FT_دھارا_Read( دھارا, (FT_Byte*)سرخی, 128 );
    إذا ( error )
      اذهب Exit;

    إذا (            سرخی[ 0] !=   0 ||
                    سرخی[74] !=   0 ||
                    سرخی[82] !=   0 ||
                    سرخی[ 1] ==   0 ||
                    سرخی[ 1] >   33 ||
                    سرخی[63] !=   0 ||
         سرخی[2 + سرخی[1]] !=   0 ||
                  سرخی[0x53] > 0x7F )
      أرجع FT_THROW( Unknown_File_Format );

    dlen = ( سرخی[0x53] << 24 ) |
           ( سرخی[0x54] << 16 ) |
           ( سرخی[0x55] <<  8 ) |
             سرخی[0x56];
#إذا 0
    rlen = ( سرخی[0x57] << 24 ) |
           ( سرخی[0x58] << 16 ) |
           ( سرخی[0x59] <<  8 ) |
             سرخی[0x5A];
#جے_انت /* 0 */
    وتھ = 128 + ( ( dlen + 127 ) & ~127 );

    أرجع IsMacResource( library, دھارا, وتھ, face_اشاریہ, aface );

  Exit:
    أرجع error;
  }


  ساكن FT_Error
  لدو_face_وچ_embedded_rfork( FT_Library           library,
                               FT_دھارا            دھارا,
                               FT_Long              face_اشاریہ,
                               FT_Face             *aface,
                               ثابت FT_Open_دلیلاں  *دلیلاں )
  {

#تعریف_رد  FT_COMPONENT
#تعریف_کرو FT_COMPONENT  raccess

    FT_حافظہ  حافظہ = library->حافظہ;
    FT_Error   error  = FT_ERR( Unknown_File_Format );
    FT_UInt    i;

    حرف*      file_ناں[FT_RACCESS_N_قاعدے];
    FT_Long    offsets[FT_RACCESS_N_قاعدے];
    FT_Error   errors[FT_RACCESS_N_قاعدے];
    FT_Bool    is_darwin_vfs, vfs_rfork_has_no_font = FALSE; /* not tested */

    FT_Open_دلیلاں  args2;
    FT_دھارا     stream2 = عدم;


    FT_Raccess_Guess( library, دھارا,
                      دلیلاں->pathname, file_ناں, offsets, errors );

    لكل ( i = 0; i < FT_RACCESS_N_قاعدے; i++ )
    {
      is_darwin_vfs = ft_raccess_قاعدہ_by_darwin_vfs( library, i );
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
      args2.pathname = file_ناں[i] ? file_ناں[i] : دلیلاں->pathname;

      FT_TRACE3(( "Try rule %u: %s (offset=%ld) ...",
                  i, args2.pathname, offsets[i] ));

      error = FT_دھارا_New( library, &args2, &stream2 );
      إذا ( is_darwin_vfs && FT_ERR_EQ( error, Cannot_Open_دھارا ) )
        vfs_rfork_has_no_font = TRUE;

      إذا ( error )
      {
        FT_TRACE3(( "failed\n" ));
        استمر;
      }

      error = IsMacResource( library, stream2, offsets[i],
                             face_اشاریہ, aface );
      FT_دھارا_Free( stream2, 0 );

      FT_TRACE3(( "%s\n", error ? "failed": "successful" ));

      إذا ( !error )
          اكسر;
      وإلا إذا ( is_darwin_vfs )
          vfs_rfork_has_no_font = TRUE;
    }

    لكل (i = 0; i < FT_RACCESS_N_قاعدے; i++)
    {
      إذا ( file_ناں[i] )
        FT_FREE( file_ناں[i] );
    }

    /* Caller (load_mac_face) requires FT_Err_Unknown_File_Format. */
    إذا ( error )
      error = FT_ERR( Unknown_File_Format );

    أرجع error;

#تعریف_رد  FT_COMPONENT
#تعریف_کرو FT_COMPONENT  objs

  }


  /* Check for some macintosh formats without Carbon framework.    */
  /* Is this a macbinary file?  If so look at the resource fork.   */
  /* Is this a mac dfont file?                                     */
  /* Is this an old style resource fork? (in data)                 */
  /* Else call load_face_in_embedded_rfork to try extra rules      */
  /* (defined in `ftrfork.c').                                     */
  /*                                                               */
  ساكن FT_Error
  لدو_mac_face( FT_Library           library,
                 FT_دھارا            دھارا,
                 FT_Long              face_اشاریہ,
                 FT_Face             *aface,
                 ثابت FT_Open_دلیلاں  *دلیلاں )
  {
    FT_Error error;
    FT_بن_ورتیا( دلیلاں );


    error = Is_Mac_ثنائی( library, دھارا, face_اشاریہ, aface );
    إذا ( FT_ERR_EQ( error, Unknown_File_Format ) )
    {

#تعریف_رد  FT_COMPONENT
#تعریف_کرو FT_COMPONENT  raccess

#جے_تعریف_شدہ FT_DEBUG_LEVEL_TRACE
      FT_TRACE3(( "Try as dfont: " ));
      إذا ( !( دلیلاں->flags & FT_OPEN_حافظہ ) )
        FT_TRACE3(( "%s ...", دلیلاں->pathname ));
#جے_انت

      error = IsMacResource( library, دھارا, 0, face_اشاریہ, aface );

      FT_TRACE3(( "%s\n", error ? "failed" : "successful" ));

#تعریف_رد  FT_COMPONENT
#تعریف_کرو FT_COMPONENT  objs

    }

    إذا ( ( FT_ERR_EQ( error, Unknown_File_Format )      ||
           FT_ERR_EQ( error, Invalid_دھارا_Operation ) ) &&
         ( دلیلاں->flags & FT_OPEN_PATHNAME )               )
      error = لدو_face_وچ_embedded_rfork( library, دھارا,
                                           face_اشاریہ, aface, دلیلاں );
    أرجع error;
  }
#جے_انت

#جے_انت  /* !FT_MACINTOSH && FT_CONFIG_OPTION_MAC_FONTS */


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Open_Face( FT_Library           library,
                ثابت FT_Open_دلیلاں*  دلیلاں,
                FT_Long              face_اشاریہ,
                FT_Face             *aface )
  {
    أرجع ft_open_face_اندرونی( library, دلیلاں, face_اشاریہ, aface, 1 );
  }


  ساكن FT_Error
  ft_open_face_اندرونی( FT_Library           library,
                         ثابت FT_Open_دلیلاں*  دلیلاں,
                         FT_Long              face_اشاریہ,
                         FT_Face             *aface,
                         FT_Bool              test_mac_fonts )
  {
    FT_Error     error;
    FT_Driver    driver = عدم;
    FT_حافظہ    حافظہ = عدم;
    FT_دھارا    دھارا = عدم;
    FT_Face      face   = عدم;
    FT_List_گرہ  گرہ   = عدم;
    FT_Bool      external_دھارا;
    FT_Module*   cur;
    FT_Module*   حد_بندی;

#جے_بنا_تعریف FT_CONFIG_OPTION_MAC_FONTS
    FT_بن_ورتیا( test_mac_fonts );
#جے_انت


    /* only use lower 31 bits together with sign bit */
    إذا ( face_اشاریہ > 0 )
      face_اشاریہ &= 0x7FFFFFFFL;
    وإلا
    {
      face_اشاریہ  = -face_اشاریہ;
      face_اشاریہ &= 0x7FFFFFFFL;
      face_اشاریہ  = -face_اشاریہ;
    }

#جے_تعریف_شدہ FT_DEBUG_LEVEL_TRACE
    FT_TRACE3(( "FT_Open_Face: " ));
    إذا ( face_اشاریہ < 0 )
      FT_TRACE3(( "Requesting number of faces and named instances\n"));
    وإلا
    {
      FT_TRACE3(( "Requesting face %ld", face_اشاریہ & 0xFFFFL ));
      إذا ( face_اشاریہ & 0x7FFF0000L )
        FT_TRACE3(( ", named instance %ld", face_اشاریہ >> 16 ));
      FT_TRACE3(( "\n" ));
    }
#جے_انت

    /* test for valid `library' delayed to `FT_Stream_New' */

    إذا ( !دلیلاں )
      أرجع FT_THROW( Invalid_دلیل );

    external_دھارا = FT_BOOL( ( دلیلاں->flags & FT_OPEN_دھارا ) &&
                               دلیلاں->دھارا                     );

    /* create input stream */
    error = FT_دھارا_New( library, دلیلاں, &دھارا );
    إذا ( error )
      اذهب Fail3;

    /* Do this error check after `FT_Stream_New` to ensure that the */
    /* 'close' callback is called.                                  */
    إذا ( !aface && face_اشاریہ >= 0 )
    {
      error = FT_THROW( Invalid_دلیل );
      اذهب Fail3;
    }

    حافظہ = library->حافظہ;

    /* If the font driver is specified in the `args' structure, use */
    /* it.  Otherwise, we scan the list of registered drivers.      */
    إذا ( ( دلیلاں->flags & FT_OPEN_DRIVER ) && دلیلاں->driver )
    {
      driver = FT_DRIVER( دلیلاں->driver );

      /* not all modules are drivers, so check... */
      إذا ( FT_MODULE_IS_DRIVER( driver ) )
      {
        FT_Int         عدد_params = 0;
        FT_Parameter*  params     = عدم;


        إذا ( دلیلاں->flags & FT_OPEN_PARAMS )
        {
          عدد_params = دلیلاں->عدد_params;
          params     = دلیلاں->params;
        }

        error = open_face( driver, &دھارا, &external_دھارا, face_اشاریہ,
                           عدد_params, params, &face );
        إذا ( !error )
          اذهب Success;
      }
      وإلا
        error = FT_THROW( Invalid_Handle );

      FT_دھارا_Free( دھارا, external_دھارا );
      اذهب Fail;
    }
    وإلا
    {
      error = FT_ERR( Missing_Module );

      /* check each font driver for an appropriate format */
      cur   = library->modules;
      حد_بندی = cur + library->عدد_modules;

      لكل ( ; cur < حد_بندی; cur++ )
      {
        /* not all modules are font drivers, so check... */
        إذا ( FT_MODULE_IS_DRIVER( cur[0] ) )
        {
          FT_Int         عدد_params = 0;
          FT_Parameter*  params     = عدم;


          driver = FT_DRIVER( cur[0] );

          إذا ( دلیلاں->flags & FT_OPEN_PARAMS )
          {
            عدد_params = دلیلاں->عدد_params;
            params     = دلیلاں->params;
          }

          error = open_face( driver, &دھارا, &external_دھارا, face_اشاریہ,
                             عدد_params, params, &face );
          إذا ( !error )
            اذهب Success;

#جے_تعریف_شدہ FT_CONFIG_OPTION_MAC_FONTS
          إذا ( test_mac_fonts                                           &&
               ft_strcmp( cur[0]->clazz->module_name, "truetype" ) == 0 &&
               FT_ERR_EQ( error, جدول_Missing )                        )
          {
            /* TrueType but essential tables are missing */
            error = FT_دھارا_Seek( دھارا, 0 );
            إذا ( error )
              اكسر;

            error = open_face_PS_from_sfnt_دھارا( library,
                                                   دھارا,
                                                   face_اشاریہ,
                                                   عدد_params,
                                                   params,
                                                   aface );
            إذا ( !error )
            {
              FT_دھارا_Free( دھارا, external_دھارا );
              أرجع error;
            }
          }
#جے_انت

          إذا ( FT_ERR_NEQ( error, Unknown_File_Format ) )
            اذهب Fail3;
        }
      }

    Fail3:
      /* If we are on the mac, and we get an                          */
      /* FT_Err_Invalid_Stream_Operation it may be because we have an */
      /* empty data fork, so we need to check the resource fork.      */
      إذا ( FT_ERR_NEQ( error, Cannot_Open_دھارا )       &&
           FT_ERR_NEQ( error, Unknown_File_Format )      &&
           FT_ERR_NEQ( error, Invalid_دھارا_Operation ) )
        اذهب Fail2;

#إذا !defined( FT_MACINTOSH ) && defined( FT_CONFIG_OPTION_MAC_FONTS )
      إذا ( test_mac_fonts )
      {
        error = لدو_mac_face( library, دھارا, face_اشاریہ, aface, دلیلاں );
        إذا ( !error )
        {
          /* We don't want to go to Success here.  We've already done   */
          /* that.  On the other hand, if we succeeded we still need to */
          /* close this stream (we opened a different stream which      */
          /* extracted the interesting information out of this stream   */
          /* here.  That stream will still be open and the face will    */
          /* point to it).                                              */
          FT_دھارا_Free( دھارا, external_دھارا );
          أرجع error;
        }
      }

      إذا ( FT_ERR_NEQ( error, Unknown_File_Format ) )
        اذهب Fail2;
#جے_انت  /* !FT_MACINTOSH && FT_CONFIG_OPTION_MAC_FONTS */

      /* no driver is able to handle this format */
      error = FT_THROW( Unknown_File_Format );

  Fail2:
      FT_دھارا_Free( دھارا, external_دھارا );
      اذهب Fail;
    }

  Success:
    FT_TRACE4(( "FT_Open_Face: New face object, adding to list\n" ));

    /* add the face object to its driver's list */
    إذا ( FT_QNEW( گرہ ) )
      اذهب Fail;

    گرہ->انکڑے = face;
    /* don't assume driver is the same as face->driver, so use */
    /* face->driver instead.                                   */
    FT_List_جوڑو( &face->driver->faces_list, گرہ );

    /* now allocate a glyph slot object for the face */
    FT_TRACE4(( "FT_Open_Face: Creating glyph slot\n" ));

    إذا ( face_اشاریہ >= 0 )
    {
      error = FT_New_Glyph_تھاں( face, عدم );
      إذا ( error )
        اذهب Fail;

      /* finally, allocate a size object for the face */
      {
        FT_ناپ  ناپ;


        FT_TRACE4(( "FT_Open_Face: Creating size object\n" ));

        error = FT_New_ناپ( face, &ناپ );
        إذا ( error )
          اذهب Fail;

        face->ناپ = ناپ;
      }
    }

    /* some checks */

    إذا ( FT_IS_SCALABLE( face ) )
    {
      إذا ( face->اچائی < 0 )
        face->اچائی = (FT_Short)-face->اچائی;

      إذا ( !FT_HAS_VERTICAL( face ) )
        face->max_advance_اچائی = (FT_Short)face->اچائی;
    }

    إذا ( FT_HAS_FIXED_SIZES( face ) )
    {
      FT_Int  i;


      لكل ( i = 0; i < face->عدد_fixed_sizes; i++ )
      {
        FT_Bitmap_ناپ*  bsize = face->دستیاب_sizes + i;


        إذا ( bsize->اچائی < 0 )
          bsize->اچائی = -bsize->اچائی;
        إذا ( bsize->x_ppem < 0 )
          bsize->x_ppem = -bsize->x_ppem;
        إذا ( bsize->y_ppem < 0 )
          bsize->y_ppem = -bsize->y_ppem;

        /* check whether negation actually has worked */
        إذا ( bsize->اچائی < 0 || bsize->x_ppem < 0 || bsize->y_ppem < 0 )
        {
          FT_TRACE0(( "FT_Open_Face:"
                      " Invalid bitmap dimensions for strike %d,"
                      " now disabled\n", i ));
          bsize->چوڑائی  = 0;
          bsize->اچائی = 0;
          bsize->ناپ   = 0;
          bsize->x_ppem = 0;
          bsize->y_ppem = 0;
        }
      }
    }

    /* initialize internal face data */
    {
      FT_Face_اندرونی  اندرونی = face->اندرونی;


      اندرونی->transform_میٹرکس.xx = 0x10000L;
      اندرونی->transform_میٹرکس.xy = 0;
      اندرونی->transform_میٹرکس.yx = 0;
      اندرونی->transform_میٹرکس.yy = 0x10000L;

      اندرونی->transform_delta.x = 0;
      اندرونی->transform_delta.y = 0;

      اندرونی->refcount = 1;

      اندرونی->no_stem_darkening = -1;
    }

    إذا ( aface )
      *aface = face;
    وإلا
      FT_پورا_Face( face );

    اذهب Exit;

  Fail:
    إذا ( گرہ )
      FT_پورا_Face( face );    /* face must be in the driver's list */
    وإلا إذا ( face )
      destroy_face( حافظہ, face, driver );

  Exit:
#جے_تعریف_شدہ FT_DEBUG_LEVEL_TRACE
    إذا ( !error && face_اشاریہ < 0 )
    {
      FT_TRACE3(( "FT_Open_Face: The font has %ld face%s\n",
                  face->عدد_faces,
                  face->عدد_faces == 1 ? "" : "s" ));
      FT_TRACE3(( "              and %ld named instance%s for face %ld\n",
                  face->style_flags >> 16,
                  ( face->style_flags >> 16 ) == 1 ? "" : "s",
                  -face_اشاریہ - 1 ));
    }
#جے_انت

    FT_TRACE4(( "FT_Open_Face: Return 0x%x\n", error ));

    أرجع error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Attach_File( FT_Face      face,
                  ثابت حرف*  filepathname )
  {
    FT_Open_دلیلاں  open;


    /* test for valid `face' delayed to `FT_Attach_Stream' */

    إذا ( !filepathname )
      أرجع FT_THROW( Invalid_دلیل );

    open.دھارا   = عدم;
    open.flags    = FT_OPEN_PATHNAME;
    open.pathname = (حرف*)filepathname;

    أرجع FT_Attach_دھارا( face, &open );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Attach_دھارا( FT_Face              face,
                    ثابت FT_Open_دلیلاں*  parameters )
  {
    FT_دھارا  دھارا;
    FT_Error   error;
    FT_Driver  driver;

    FT_Driver_Class  clazz;


    /* test for valid `parameters' delayed to `FT_Stream_New' */

    إذا ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );

    driver = face->driver;
    إذا ( !driver )
      أرجع FT_THROW( Invalid_Driver_Handle );

    error = FT_دھارا_New( driver->جڑ.library, parameters, &دھارا );
    إذا ( error )
      اذهب Exit;

    /* we implement FT_Attach_Stream in each driver through the */
    /* `attach_file' interface                                  */

    error = FT_ERR( Unimplemented_Feature );
    clazz = driver->clazz;
    إذا ( clazz->attach_file )
      error = clazz->attach_file( face, دھارا );

    /* close the attached stream */
    FT_دھارا_Free( دھارا,
                    FT_BOOL( parameters->دھارا                     &&
                             ( parameters->flags & FT_OPEN_دھارا ) ) );

  Exit:
    أرجع error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Reference_Face( FT_Face  face )
  {
    إذا ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );

    face->اندرونی->refcount++;

    أرجع FT_Err_Ok;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_پورا_Face( FT_Face  face )
  {
    FT_Error     error;
    FT_Driver    driver;
    FT_حافظہ    حافظہ;
    FT_List_گرہ  گرہ;


    error = FT_ERR( Invalid_Face_Handle );
    إذا ( face && face->driver )
    {
      face->اندرونی->refcount--;
      إذا ( face->اندرونی->refcount > 0 )
        error = FT_Err_Ok;
      وإلا
      {
        driver = face->driver;
        حافظہ = driver->جڑ.حافظہ;

        /* find face in driver's list */
        گرہ = FT_List_لبھو( &driver->faces_list, face );
        إذا ( گرہ )
        {
          /* remove face object from the driver's list */
          FT_List_Remove( &driver->faces_list, گرہ );
          FT_FREE( گرہ );

          /* now destroy the object proper */
          destroy_face( حافظہ, face, driver );
          error = FT_Err_Ok;
        }
      }
    }

    أرجع error;
  }


  /* documentation is in ftobjs.h */

  FT_EXPORT_DEF( FT_Error )
  FT_New_ناپ( FT_Face   face,
               FT_ناپ  *asize )
  {
    FT_Error         error;
    FT_حافظہ        حافظہ;
    FT_Driver        driver;
    FT_Driver_Class  clazz;

    FT_ناپ          ناپ = عدم;
    FT_List_گرہ      گرہ = عدم;

    FT_ناپ_اندرونی  اندرونی = عدم;


    إذا ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );

    إذا ( !asize )
      أرجع FT_THROW( Invalid_دلیل );

    إذا ( !face->driver )
      أرجع FT_THROW( Invalid_Driver_Handle );

    *asize = عدم;

    driver = face->driver;
    clazz  = driver->clazz;
    حافظہ = face->حافظہ;

    /* Allocate new size object and perform basic initialisation */
    إذا ( FT_ALLOC( ناپ, clazz->ناپ_object_ناپ ) || FT_QNEW( گرہ ) )
      اذهب Exit;

    ناپ->face = face;

    إذا ( FT_NEW( اندرونی ) )
      اذهب Exit;

    ناپ->اندرونی = اندرونی;

    إذا ( clazz->init_ناپ )
      error = clazz->init_ناپ( ناپ );

    /* in case of success, add to the face's list */
    إذا ( !error )
    {
      *asize     = ناپ;
      گرہ->انکڑے = ناپ;
      FT_List_جوڑو( &face->sizes_list, گرہ );
    }

  Exit:
    إذا ( error )
    {
      FT_FREE( گرہ );
      إذا ( ناپ )
        FT_FREE( ناپ->اندرونی );
      FT_FREE( ناپ );
    }

    أرجع error;
  }


  /* documentation is in ftobjs.h */

  FT_EXPORT_DEF( FT_Error )
  FT_پورا_ناپ( FT_ناپ  ناپ )
  {
    FT_Error     error;
    FT_Driver    driver;
    FT_حافظہ    حافظہ;
    FT_Face      face;
    FT_List_گرہ  گرہ;


    إذا ( !ناپ )
      أرجع FT_THROW( Invalid_ناپ_Handle );

    face = ناپ->face;
    إذا ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );

    driver = face->driver;
    إذا ( !driver )
      أرجع FT_THROW( Invalid_Driver_Handle );

    حافظہ = driver->جڑ.حافظہ;

    error = FT_Err_Ok;
    گرہ  = FT_List_لبھو( &face->sizes_list, ناپ );
    إذا ( گرہ )
    {
      FT_List_Remove( &face->sizes_list, گرہ );
      FT_FREE( گرہ );

      إذا ( face->ناپ == ناپ )
      {
        face->ناپ = عدم;
        إذا ( face->sizes_list.سر )
          face->ناپ = (FT_ناپ)(face->sizes_list.سر->انکڑے);
      }

      destroy_ناپ( حافظہ, ناپ, driver );
    }
    وإلا
      error = FT_THROW( Invalid_ناپ_Handle );

    أرجع error;
  }


  /* documentation is in ftobjs.h */

  FT_ادھار_DEF( FT_Error )
  FT_میل_ناپ( FT_Face          face,
                 FT_ناپ_Request  req,
                 FT_Bool          ignore_چوڑائی,
                 FT_ULong*        ناپ_اشاریہ )
  {
    FT_Int   i;
    FT_Long  w, h;


    إذا ( !FT_HAS_FIXED_SIZES( face ) )
      أرجع FT_THROW( Invalid_Face_Handle );

    /* FT_Bitmap_Size doesn't provide enough info... */
    إذا ( req->type != FT_ناپ_REQUEST_TYPE_NOMINAL )
      أرجع FT_THROW( Unimplemented_Feature );

    w = FT_REQUEST_چوڑائی ( req );
    h = FT_REQUEST_اچائی( req );

    إذا ( req->چوڑائی && !req->اچائی )
      h = w;
    وإلا إذا ( !req->چوڑائی && req->اچائی )
      w = h;

    w = FT_PIX_گیڑا( w );
    h = FT_PIX_گیڑا( h );

    إذا ( !w || !h )
      أرجع FT_THROW( Invalid_Pixel_ناپ );

    لكل ( i = 0; i < face->عدد_fixed_sizes; i++ )
    {
      FT_Bitmap_ناپ*  bsize = face->دستیاب_sizes + i;


      إذا ( h != FT_PIX_گیڑا( bsize->y_ppem ) )
        استمر;

      إذا ( w == FT_PIX_گیڑا( bsize->x_ppem ) || ignore_چوڑائی )
      {
        FT_TRACE3(( "FT_Match_Size: bitmap strike %d matches\n", i ));

        إذا ( ناپ_اشاریہ )
          *ناپ_اشاریہ = (FT_ULong)i;

        أرجع FT_Err_Ok;
      }
    }

    FT_TRACE3(( "FT_Match_Size: no matching bitmap strike\n" ));

    أرجع FT_THROW( Invalid_Pixel_ناپ );
  }


  /* documentation is in ftobjs.h */

  FT_ادھار_DEF( فراغ )
  ft_synthesize_vertical_metrics( FT_Glyph_Metrics*  metrics,
                                  FT_Pos             advance )
  {
    FT_Pos  اچائی = metrics->اچائی;


    /* compensate for glyph with bbox above/below the baseline */
    إذا ( metrics->horiBearingY < 0 )
    {
      إذا ( اچائی < metrics->horiBearingY )
        اچائی = metrics->horiBearingY;
    }
    وإلا إذا ( metrics->horiBearingY > 0 )
      اچائی -= metrics->horiBearingY;

    /* the factor 1.2 is a heuristical value */
    إذا ( !advance )
      advance = اچائی * 12 / 10;

    metrics->vertBearingX = metrics->horiBearingX - metrics->horiAdvance / 2;
    metrics->vertBearingY = ( advance - اچائی ) / 2;
    metrics->vertAdvance  = advance;
  }


  ساكن فراغ
  ft_recompute_scaled_metrics( FT_Face           face,
                               FT_ناپ_Metrics*  metrics )
  {
    /* Compute root ascender, descender, test height, and max_advance */

#جے_تعریف_شدہ GRID_FIT_METRICS
    metrics->ascender    = FT_PIX_CEIL( FT_MulFix( face->ascender,
                                                   metrics->y_scale ) );

    metrics->descender   = FT_PIX_FLOOR( FT_MulFix( face->descender,
                                                    metrics->y_scale ) );

    metrics->اچائی      = FT_PIX_گیڑا( FT_MulFix( face->اچائی,
                                                    metrics->y_scale ) );

    metrics->max_advance = FT_PIX_گیڑا( FT_MulFix( face->max_advance_چوڑائی,
                                                    metrics->x_scale ) );
#وإلا /* !GRID_FIT_METRICS */
    metrics->ascender    = FT_MulFix( face->ascender,
                                      metrics->y_scale );

    metrics->descender   = FT_MulFix( face->descender,
                                      metrics->y_scale );

    metrics->اچائی      = FT_MulFix( face->اچائی,
                                      metrics->y_scale );

    metrics->max_advance = FT_MulFix( face->max_advance_چوڑائی,
                                      metrics->x_scale );
#جے_انت /* !GRID_FIT_METRICS */
  }


  FT_ادھار_DEF( فراغ )
  FT_Select_Metrics( FT_Face   face,
                     FT_ULong  strike_اشاریہ )
  {
    FT_ناپ_Metrics*  metrics;
    FT_Bitmap_ناپ*   bsize;


    metrics = &face->ناپ->metrics;
    bsize   = face->دستیاب_sizes + strike_اشاریہ;

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
      metrics->اچائی      = bsize->اچائی << 6;
      metrics->max_advance = bsize->x_ppem;
    }
  }


  FT_ادھار_DEF( FT_Error )
  FT_Request_Metrics( FT_Face          face,
                      FT_ناپ_Request  req )
  {
    FT_Error  error = FT_Err_Ok;

    FT_ناپ_Metrics*  metrics;


    metrics = &face->ناپ->metrics;

    إذا ( FT_IS_SCALABLE( face ) )
    {
      FT_Long  w = 0, h = 0, scaled_w = 0, scaled_h = 0;


      بدل ( req->type )
      {
      حالة FT_ناپ_REQUEST_TYPE_NOMINAL:
        w = h = face->units_فی_EM;
        اكسر;

      حالة FT_ناپ_REQUEST_TYPE_REAL_DIM:
        w = h = face->ascender - face->descender;
        اكسر;

      حالة FT_ناپ_REQUEST_TYPE_BBOX:
        w = face->bbox.xMax - face->bbox.xMin;
        h = face->bbox.yMax - face->bbox.yMin;
        اكسر;

      حالة FT_ناپ_REQUEST_TYPE_CELL:
        w = face->max_advance_چوڑائی;
        h = face->ascender - face->descender;
        اكسر;

      حالة FT_ناپ_REQUEST_TYPE_SCALES:
        metrics->x_scale = (FT_Fixed)req->چوڑائی;
        metrics->y_scale = (FT_Fixed)req->اچائی;
        إذا ( !metrics->x_scale )
          metrics->x_scale = metrics->y_scale;
        وإلا إذا ( !metrics->y_scale )
          metrics->y_scale = metrics->x_scale;
        اذهب Calculate_Ppem;

      حالة FT_ناپ_REQUEST_TYPE_MAX:
        اكسر;
      }

      /* to be on the safe side */
      إذا ( w < 0 )
        w = -w;

      إذا ( h < 0 )
        h = -h;

      scaled_w = FT_REQUEST_چوڑائی ( req );
      scaled_h = FT_REQUEST_اچائی( req );

      /* determine scales */
      إذا ( req->اچائی || !req->چوڑائی )
      {
        إذا ( h == 0 )
        {
          FT_ERROR(( "FT_Request_Metrics: Divide by zero\n" ));
          error = FT_ERR( Divide_By_Zero );
          اذهب Exit;
        }

        metrics->y_scale = FT_DivFix( scaled_h, h );
      }

      إذا ( req->چوڑائی )
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

      إذا ( !req->اچائی )
      {
        metrics->y_scale = metrics->x_scale;
        scaled_h = FT_MulDiv( scaled_w, h, w );
      }

      إذا ( req->type == FT_ناپ_REQUEST_TYPE_CELL )
      {
        إذا ( metrics->y_scale > metrics->x_scale )
          metrics->y_scale = metrics->x_scale;
        وإلا
          metrics->x_scale = metrics->y_scale;
      }

  Calculate_Ppem:
      /* calculate the ppems */
      إذا ( req->type != FT_ناپ_REQUEST_TYPE_NOMINAL )
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
        error = FT_ERR( Invalid_Pixel_ناپ );
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
  FT_Select_ناپ( FT_Face  face,
                  FT_Int   strike_اشاریہ )
  {
    FT_Error         error = FT_Err_Ok;
    FT_Driver_Class  clazz;


    إذا ( !face || !FT_HAS_FIXED_SIZES( face ) )
      أرجع FT_THROW( Invalid_Face_Handle );

    إذا ( strike_اشاریہ < 0 || strike_اشاریہ >= face->عدد_fixed_sizes )
      أرجع FT_THROW( Invalid_دلیل );

    clazz = face->driver->clazz;

    إذا ( clazz->select_ناپ )
    {
      error = clazz->select_ناپ( face->ناپ, (FT_ULong)strike_اشاریہ );

      FT_TRACE5(( "FT_Select_Size (%s driver):\n",
                  face->driver->جڑ.clazz->module_name ));
    }
    وإلا
    {
      FT_Select_Metrics( face, (FT_ULong)strike_اشاریہ );

      FT_TRACE5(( "FT_Select_Size:\n" ));
    }

#جے_تعریف_شدہ FT_DEBUG_LEVEL_TRACE
    {
      FT_ناپ_Metrics*  metrics = &face->ناپ->metrics;


      FT_TRACE5(( "  x scale: %ld (%f)\n",
                  metrics->x_scale, (مزدوج)metrics->x_scale / 65536 ));
      FT_TRACE5(( "  y scale: %ld (%f)\n",
                  metrics->y_scale, (مزدوج)metrics->y_scale / 65536 ));
      FT_TRACE5(( "  ascender: %f\n",
                  (مزدوج)metrics->ascender / 64 ));
      FT_TRACE5(( "  descender: %f\n",
                  (مزدوج)metrics->descender / 64 ));
      FT_TRACE5(( "  height: %f\n",
                  (مزدوج)metrics->اچائی / 64 ));
      FT_TRACE5(( "  max advance: %f\n",
                  (مزدوج)metrics->max_advance / 64 ));
      FT_TRACE5(( "  x ppem: %d\n", metrics->x_ppem ));
      FT_TRACE5(( "  y ppem: %d\n", metrics->y_ppem ));
    }
#جے_انت

    أرجع error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Request_ناپ( FT_Face          face,
                   FT_ناپ_Request  req )
  {
    FT_Error         error;
    FT_Driver_Class  clazz;
    FT_ULong         strike_اشاریہ;


    إذا ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );

    إذا ( !face->ناپ )
      أرجع FT_THROW( Invalid_ناپ_Handle );

    إذا ( !req || req->چوڑائی < 0 || req->اچائی < 0 ||
         req->type >= FT_ناپ_REQUEST_TYPE_MAX )
      أرجع FT_THROW( Invalid_دلیل );

    /* signal the auto-hinter to recompute its size metrics */
    /* (if requested)                                       */
    face->ناپ->اندرونی->autohint_metrics.x_scale = 0;

    clazz = face->driver->clazz;

    إذا ( clazz->request_ناپ )
    {
      error = clazz->request_ناپ( face->ناپ, req );

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
      error = FT_میل_ناپ( face, req, 0, &strike_اشاریہ );
      إذا ( error )
        اذهب Exit;

      أرجع FT_Select_ناپ( face, (FT_Int)strike_اشاریہ );
    }
    وإلا
    {
      error = FT_Request_Metrics( face, req );
      إذا ( error )
        اذهب Exit;

      FT_TRACE5(( "FT_Request_Size:\n" ));
    }

#جے_تعریف_شدہ FT_DEBUG_LEVEL_TRACE
    {
      FT_ناپ_Metrics*  metrics = &face->ناپ->metrics;


      FT_TRACE5(( "  x scale: %ld (%f)\n",
                  metrics->x_scale, (مزدوج)metrics->x_scale / 65536 ));
      FT_TRACE5(( "  y scale: %ld (%f)\n",
                  metrics->y_scale, (مزدوج)metrics->y_scale / 65536 ));
      FT_TRACE5(( "  ascender: %f\n",
                  (مزدوج)metrics->ascender / 64 ));
      FT_TRACE5(( "  descender: %f\n",
                  (مزدوج)metrics->descender / 64 ));
      FT_TRACE5(( "  height: %f\n",
                  (مزدوج)metrics->اچائی / 64 ));
      FT_TRACE5(( "  max advance: %f\n",
                  (مزدوج)metrics->max_advance / 64 ));
      FT_TRACE5(( "  x ppem: %d\n", metrics->x_ppem ));
      FT_TRACE5(( "  y ppem: %d\n", metrics->y_ppem ));
    }
#جے_انت

  Exit:
    أرجع error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_سیٹ_Char_ناپ( FT_Face     face,
                    FT_F26Dot6  char_چوڑائی,
                    FT_F26Dot6  char_اچائی,
                    FT_UInt     horz_resolution,
                    FT_UInt     vert_resolution )
  {
    FT_ناپ_Request_Rec  req;


    /* check of `face' delayed to `FT_Request_Size' */

    إذا ( !char_چوڑائی )
      char_چوڑائی = char_اچائی;
    وإلا إذا ( !char_اچائی )
      char_اچائی = char_چوڑائی;

    إذا ( !horz_resolution )
      horz_resolution = vert_resolution;
    وإلا إذا ( !vert_resolution )
      vert_resolution = horz_resolution;

    إذا ( char_چوڑائی  < 1 * 64 )
      char_چوڑائی  = 1 * 64;
    إذا ( char_اچائی < 1 * 64 )
      char_اچائی = 1 * 64;

    إذا ( !horz_resolution )
      horz_resolution = vert_resolution = 72;

    req.type           = FT_ناپ_REQUEST_TYPE_NOMINAL;
    req.چوڑائی          = char_چوڑائی;
    req.اچائی         = char_اچائی;
    req.horiResolution = horz_resolution;
    req.vertResolution = vert_resolution;

    أرجع FT_Request_ناپ( face, &req );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_سیٹ_Pixel_Sizes( FT_Face  face,
                      FT_UInt  pixel_چوڑائی,
                      FT_UInt  pixel_اچائی )
  {
    FT_ناپ_Request_Rec  req;


    /* check of `face' delayed to `FT_Request_Size' */

    إذا ( pixel_چوڑائی == 0 )
      pixel_چوڑائی = pixel_اچائی;
    وإلا إذا ( pixel_اچائی == 0 )
      pixel_اچائی = pixel_چوڑائی;

    إذا ( pixel_چوڑائی  < 1 )
      pixel_چوڑائی  = 1;
    إذا ( pixel_اچائی < 1 )
      pixel_اچائی = 1;

    /* use `>=' to avoid potential compiler warning on 16bit platforms */
    إذا ( pixel_چوڑائی >= 0xFFFFU )
      pixel_چوڑائی = 0xFFFFU;
    إذا ( pixel_اچائی >= 0xFFFFU )
      pixel_اچائی = 0xFFFFU;

    req.type           = FT_ناپ_REQUEST_TYPE_NOMINAL;
    req.چوڑائی          = (FT_Long)( pixel_چوڑائی << 6 );
    req.اچائی         = (FT_Long)( pixel_اچائی << 6 );
    req.horiResolution = 0;
    req.vertResolution = 0;

    أرجع FT_Request_ناپ( face, &req );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Kerning( FT_Face     face,
                  FT_UInt     بایاں_glyph,
                  FT_UInt     دایاں_glyph,
                  FT_UInt     kern_منوال,
                  FT_سمتیہ  *akerning )
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
                                          بایاں_glyph,
                                          دایاں_glyph,
                                          akerning );
      إذا ( !error )
      {
        إذا ( kern_منوال != FT_KERNING_UNSCALED )
        {
          akerning->x = FT_MulFix( akerning->x, face->ناپ->metrics.x_scale );
          akerning->y = FT_MulFix( akerning->y, face->ناپ->metrics.y_scale );

          إذا ( kern_منوال != FT_KERNING_UNFITTED )
          {
            FT_Pos  orig_x = akerning->x;
            FT_Pos  orig_y = akerning->y;


            /* we scale down kerning values for small ppem values */
            /* to avoid that rounding makes them too big.         */
            /* `25' has been determined heuristically.            */
            إذا ( face->ناپ->metrics.x_ppem < 25 )
              akerning->x = FT_MulDiv( orig_x,
                                       face->ناپ->metrics.x_ppem, 25 );
            إذا ( face->ناپ->metrics.y_ppem < 25 )
              akerning->y = FT_MulDiv( orig_y,
                                       face->ناپ->metrics.y_ppem, 25 );

            akerning->x = FT_PIX_گیڑا( akerning->x );
            akerning->y = FT_PIX_گیڑا( akerning->y );

#جے_تعریف_شدہ FT_DEBUG_LEVEL_TRACE
            {
              FT_Pos  orig_x_rounded = FT_PIX_گیڑا( orig_x );
              FT_Pos  orig_y_rounded = FT_PIX_گیڑا( orig_y );


              إذا ( akerning->x != orig_x_rounded ||
                   akerning->y != orig_y_rounded )
                FT_TRACE5(( "FT_Get_Kerning: horizontal kerning"
                            " (%ld, %ld) scaled down to (%ld, %ld) pixels\n",
                            orig_x_rounded / 64, orig_y_rounded / 64,
                            akerning->x / 64, akerning->y / 64 ));
            }
#جے_انت
          }
        }
      }
    }

    أرجع error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Track_Kerning( FT_Face    face,
                        FT_Fixed   نقطہ_ناپ,
                        FT_Int     degree,
                        FT_Fixed*  akerning )
  {
    FT_Service_Kerning  service;
    FT_Error            error = FT_Err_Ok;


    إذا ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );

    إذا ( !akerning )
      أرجع FT_THROW( Invalid_دلیل );

    FT_FACE_لبھو_SERVICE( face, service, KERNING );
    إذا ( !service )
      أرجع FT_THROW( Unimplemented_Feature );

    error = service->get_track( face,
                                نقطہ_ناپ,
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
    FT_CharMap*  حد_بندی;


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
      أرجع لبھو_unicode_charmap( face );

    cur = face->charmaps;
    إذا ( !cur )
      أرجع FT_THROW( Invalid_CharMap_Handle );

    حد_بندی = cur + face->عدد_charmaps;

    لكل ( ; cur < حد_بندی; cur++ )
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
  FT_سیٹ_Charmap( FT_Face     face,
                  FT_CharMap  charmap )
  {
    FT_CharMap*  cur;
    FT_CharMap*  حد_بندی;


    إذا ( !face )
      أرجع FT_THROW( Invalid_Face_Handle );

    cur = face->charmaps;
    إذا ( !cur || !charmap )
      أرجع FT_THROW( Invalid_CharMap_Handle );

    حد_بندی = cur + face->عدد_charmaps;

    لكل ( ; cur < حد_بندی; cur++ )
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
  FT_Get_Charmap_اشاریہ( FT_CharMap  charmap )
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
  ft_cmap_پورا_اندرونی( FT_CMap  cmap )
  {
    FT_CMap_Class  clazz  = cmap->clazz;
    FT_Face        face   = cmap->charmap.face;
    FT_حافظہ      حافظہ = FT_FACE_حافظہ( face );


    إذا ( clazz->پورا )
      clazz->پورا( cmap );

    FT_FREE( cmap );
  }


  FT_ادھار_DEF( فراغ )
  FT_CMap_پورا( FT_CMap  cmap )
  {
    إذا ( cmap )
    {
      FT_Face    face   = cmap->charmap.face;
      FT_حافظہ  حافظہ = FT_FACE_حافظہ( face );
      FT_Error   error;
      FT_Int     i, j;


      لكل ( i = 0; i < face->عدد_charmaps; i++ )
      {
        إذا ( (FT_CMap)face->charmaps[i] == cmap )
        {
          FT_CharMap  آخری_charmap = face->charmaps[face->عدد_charmaps - 1];


          إذا ( FT_QRENEW_ARRAY( face->charmaps,
                                face->عدد_charmaps,
                                face->عدد_charmaps - 1 ) )
            أرجع;

          /* remove it from our list of charmaps */
          لكل ( j = i + 1; j < face->عدد_charmaps; j++ )
          {
            إذا ( j == face->عدد_charmaps - 1 )
              face->charmaps[j - 1] = آخری_charmap;
            وإلا
              face->charmaps[j - 1] = face->charmaps[j];
          }

          face->عدد_charmaps--;

          إذا ( (FT_CMap)face->charmap == cmap )
            face->charmap = عدم;

          ft_cmap_پورا_اندرونی( cmap );

          اكسر;
        }
      }
    }
  }


  FT_ادھار_DEF( FT_Error )
  FT_CMap_New( FT_CMap_Class  clazz,
               FT_اشارہ_گر     init_انکڑے,
               FT_CharMap     charmap,
               FT_CMap       *acmap )
  {
    FT_Error   error;
    FT_Face    face;
    FT_حافظہ  حافظہ;
    FT_CMap    cmap = عدم;


    إذا ( !clazz || !charmap || !charmap->face )
      أرجع FT_THROW( Invalid_دلیل );

    face   = charmap->face;
    حافظہ = FT_FACE_حافظہ( face );

    إذا ( !FT_ALLOC( cmap, clazz->ناپ ) )
    {
      cmap->charmap = *charmap;
      cmap->clazz   = clazz;

      إذا ( clazz->init )
      {
        error = clazz->init( cmap, init_انکڑے );
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
    ft_cmap_پورا_اندرونی( cmap );
    cmap = عدم;
    اذهب Exit;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt )
  FT_Get_Char_اشاریہ( FT_Face   face,
                     FT_ULong  charcode )
  {
    FT_UInt  نتیجہ = 0;


    إذا ( face && face->charmap )
    {
      FT_CMap  cmap = FT_CMAP( face->charmap );


      إذا ( charcode > 0xFFFFFFFFUL )
      {
        FT_TRACE1(( "FT_Get_Char_Index: too large charcode" ));
        FT_TRACE1(( " 0x%lx is truncated\n", charcode ));
      }

      نتیجہ = cmap->clazz->char_اشاریہ( cmap, (FT_UInt32)charcode );
      إذا ( نتیجہ >= (FT_UInt)face->عدد_glyphs )
        نتیجہ = 0;
    }

    أرجع نتیجہ;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_ULong )
  FT_Get_پہلا_Char( FT_Face   face,
                     FT_UInt  *agindex )
  {
    FT_ULong  نتیجہ = 0;
    FT_UInt   gindex = 0;


    /* only do something if we have a charmap, and we have glyphs at all */
    إذا ( face && face->charmap && face->عدد_glyphs )
    {
      gindex = FT_Get_Char_اشاریہ( face, 0 );
      إذا ( gindex == 0 )
        نتیجہ = FT_Get_اگلا_Char( face, 0, &gindex );
    }

    إذا ( agindex )
      *agindex = gindex;

    أرجع نتیجہ;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_ULong )
  FT_Get_اگلا_Char( FT_Face   face,
                    FT_ULong  charcode,
                    FT_UInt  *agindex )
  {
    FT_ULong  نتیجہ = 0;
    FT_UInt   gindex = 0;


    إذا ( face && face->charmap && face->عدد_glyphs )
    {
      FT_UInt32  کوڈ = (FT_UInt32)charcode;
      FT_CMap    cmap = FT_CMAP( face->charmap );


      افعل
      {
        gindex = cmap->clazz->char_اگلا( cmap, &کوڈ );

      } طالما ( gindex >= (FT_UInt)face->عدد_glyphs );

      نتیجہ = ( gindex == 0 ) ? 0 : کوڈ;
    }

    إذا ( agindex )
      *agindex = gindex;

    أرجع نتیجہ;
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
      إذا ( properties->ٹیگ == FT_PARAM_ٹیگ_STEM_DARKENING )
      {
        إذا ( properties->انکڑے )
        {
          إذا ( *( (FT_Bool*)properties->انکڑے ) == TRUE )
            face->اندرونی->no_stem_darkening = FALSE;
          وإلا
            face->اندرونی->no_stem_darkening = TRUE;
        }
        وإلا
        {
          /* use module default */
          face->اندرونی->no_stem_darkening = -1;
        }
      }
      وإلا إذا ( properties->ٹیگ == FT_PARAM_ٹیگ_LCD_FILTER_WEIGHTS )
      {
        error = FT_THROW( Unimplemented_Feature );
        اذهب Exit;
      }
      وإلا إذا ( properties->ٹیگ == FT_PARAM_ٹیگ_RANDOM_بیج )
      {
        إذا ( properties->انکڑے )
        {
          face->اندرونی->random_بیج = *( (FT_Int32*)properties->انکڑے );
          إذا ( face->اندرونی->random_بیج < 0 )
            face->اندرونی->random_بیج = 0;
        }
        وإلا
        {
          /* use module default */
          face->اندرونی->random_بیج = -1;
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
  FT_Face_Get_Char_Variant_اشاریہ( FT_Face   face,
                               FT_ULong  charcode,
                               FT_ULong  variantSelector )
  {
    FT_UInt  نتیجہ = 0;


    إذا ( face                                           &&
         face->charmap                                  &&
         face->charmap->encoding == FT_ENCODING_UNICODE )
    {
      FT_CharMap  charmap = لبھو_variant_selector_charmap( face );
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

        نتیجہ = vcmap->clazz->char_var_اشاریہ( vcmap, ucmap,
                                               (FT_UInt32)charcode,
                                               (FT_UInt32)variantSelector );
      }
    }

    أرجع نتیجہ;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Int )
  FT_Face_GetCharVariantIsDefault( FT_Face   face,
                                   FT_ULong  charcode,
                                   FT_ULong  variantSelector )
  {
    FT_Int  نتیجہ = -1;


    إذا ( face )
    {
      FT_CharMap  charmap = لبھو_variant_selector_charmap( face );


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

        نتیجہ = vcmap->clazz->char_var_default( vcmap,
                                                 (FT_UInt32)charcode,
                                                 (FT_UInt32)variantSelector );
      }
    }

    أرجع نتیجہ;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt32* )
  FT_Face_GetVariantSelectors( FT_Face  face )
  {
    FT_UInt32  *نتیجہ = عدم;


    إذا ( face )
    {
      FT_CharMap  charmap = لبھو_variant_selector_charmap( face );


      إذا ( charmap )
      {
        FT_CMap    vcmap  = FT_CMAP( charmap );
        FT_حافظہ  حافظہ = FT_FACE_حافظہ( face );


        نتیجہ = vcmap->clazz->variant_list( vcmap, حافظہ );
      }
    }

    أرجع نتیجہ;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt32* )
  FT_Face_Get_Variants_دا_Char( FT_Face   face,
                             FT_ULong  charcode )
  {
    FT_UInt32  *نتیجہ = عدم;


    إذا ( face )
    {
      FT_CharMap  charmap = لبھو_variant_selector_charmap( face );


      إذا ( charmap )
      {
        FT_CMap    vcmap  = FT_CMAP( charmap );
        FT_حافظہ  حافظہ = FT_FACE_حافظہ( face );


        إذا ( charcode > 0xFFFFFFFFUL )
        {
          FT_TRACE1(( "FT_Face_GetVariantsOfChar: too large charcode" ));
          FT_TRACE1(( " 0x%lx is truncated\n", charcode ));
        }

        نتیجہ = vcmap->clazz->charvariant_list( vcmap, حافظہ,
                                                 (FT_UInt32)charcode );
      }
    }
    أرجع نتیجہ;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt32* )
  FT_Face_Get_Chars_دا_Variant( FT_Face   face,
                             FT_ULong  variantSelector )
  {
    FT_UInt32  *نتیجہ = عدم;


    إذا ( face )
    {
      FT_CharMap  charmap = لبھو_variant_selector_charmap( face );


      إذا ( charmap )
      {
        FT_CMap    vcmap  = FT_CMAP( charmap );
        FT_حافظہ  حافظہ = FT_FACE_حافظہ( face );


        إذا ( variantSelector > 0xFFFFFFFFUL )
        {
          FT_TRACE1(( "FT_Get_Char_Index: too large variantSelector" ));
          FT_TRACE1(( " 0x%lx is truncated\n", variantSelector ));
        }

        نتیجہ = vcmap->clazz->variantchar_list( vcmap, حافظہ,
                                                 (FT_UInt32)variantSelector );
      }
    }

    أرجع نتیجہ;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt )
  FT_Get_Name_اشاریہ( FT_Face           face,
                     ثابت FT_String*  glyph_name )
  {
    FT_UInt  نتیجہ = 0;


    إذا ( face                       &&
         FT_HAS_GLYPH_ناں( face ) &&
         glyph_name                 )
    {
      FT_Service_GlyphDict  service;


      FT_FACE_LOOKUP_SERVICE( face,
                              service,
                              GLYPH_DICT );

      إذا ( service && service->name_اشاریہ )
        نتیجہ = service->name_اشاریہ( face, glyph_name );
    }

    أرجع نتیجہ;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Glyph_Name( FT_Face     face,
                     FT_UInt     glyph_اشاریہ,
                     FT_اشارہ_گر  بفر,
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

    إذا ( (FT_Long)glyph_اشاریہ >= face->عدد_glyphs )
      أرجع FT_THROW( Invalid_Glyph_اشاریہ );

    إذا ( !FT_HAS_GLYPH_ناں( face ) )
      أرجع FT_THROW( Invalid_دلیل );

    FT_FACE_LOOKUP_SERVICE( face, service, GLYPH_DICT );
    إذا ( service && service->get_name )
      error = service->get_name( face, glyph_اشاریہ, بفر, بفر_max );
    وإلا
      error = FT_THROW( Invalid_دلیل );

    أرجع error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( ثابت حرف* )
  FT_Get_Postscript_Name( FT_Face  face )
  {
    ثابت حرف*  نتیجہ = عدم;


    إذا ( !face )
      اذهب Exit;

    إذا ( !نتیجہ )
    {
      FT_Service_PsFontName  service;


      FT_FACE_LOOKUP_SERVICE( face,
                              service,
                              POSTSCRIPT_FONT_NAME );

      إذا ( service && service->get_ps_font_name )
        نتیجہ = service->get_ps_font_name( face );
    }

  Exit:
    أرجع نتیجہ;
  }


  /* documentation is in tttables.h */

  FT_EXPORT_DEF( فراغ* )
  FT_Get_Sfnt_جدول( FT_Face      face,
                     FT_Sfnt_ٹیگ  ٹیگ )
  {
    فراغ*                  جدول = عدم;
    FT_Service_SFNT_جدول  service;


    إذا ( face && FT_IS_SFNT( face ) )
    {
      FT_FACE_لبھو_SERVICE( face, service, SFNT_جدول );
      إذا ( service )
        جدول = service->get_جدول( face, ٹیگ );
    }

    أرجع جدول;
  }


  /* documentation is in tttables.h */

  FT_EXPORT_DEF( FT_Error )
  FT_لدو_Sfnt_جدول( FT_Face    face,
                      FT_ULong   ٹیگ,
                      FT_Long    وتھ,
                      FT_Byte*   بفر,
                      FT_ULong*  لمائی )
  {
    FT_Service_SFNT_جدول  service;


    إذا ( !face || !FT_IS_SFNT( face ) )
      أرجع FT_THROW( Invalid_Face_Handle );

    FT_FACE_لبھو_SERVICE( face, service, SFNT_جدول );
    إذا ( !service )
      أرجع FT_THROW( Unimplemented_Feature );

    أرجع service->لدو_جدول( face, ٹیگ, وتھ, بفر, لمائی );
  }


  /* documentation is in tttables.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Sfnt_جدول_Info( FT_Face    face,
                      FT_UInt    جدول_اشاریہ,
                      FT_ULong  *ٹیگ,
                      FT_ULong  *لمائی )
  {
    FT_Service_SFNT_جدول  service;
    FT_ULong               وتھ;


    /* test for valid `length' delayed to `service->table_info' */

    إذا ( !face || !FT_IS_SFNT( face ) )
      أرجع FT_THROW( Invalid_Face_Handle );

    FT_FACE_لبھو_SERVICE( face, service, SFNT_جدول );
    إذا ( !service )
      أرجع FT_THROW( Unimplemented_Feature );

    أرجع service->جدول_info( face, جدول_اشاریہ, ٹیگ, &وتھ, لمائی );
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
    FT_FACE_لبھو_SERVICE( face, service, TT_CMAP );
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
    FT_FACE_لبھو_SERVICE( face, service, TT_CMAP );
    إذا ( !service )
      أرجع -1;
    إذا ( service->get_cmap_info( charmap, &cmap_info ))
      أرجع -1;

    أرجع cmap_info.format;
  }


  /* documentation is in ftsizes.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Activate_ناپ( FT_ناپ  ناپ )
  {
    FT_Face  face;


    إذا ( !ناپ )
      أرجع FT_THROW( Invalid_ناپ_Handle );

    face = ناپ->face;
    إذا ( !face || !face->driver )
      أرجع FT_THROW( Invalid_Face_Handle );

    /* we don't need anything more complex than that; all size objects */
    /* are already listed by the face                                  */
    face->ناپ = ناپ;

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
  FT_ادھار_DEF( FT_Renderer )
  FT_Lookup_Renderer( FT_Library       library,
                      FT_Glyph_Format  format,
                      FT_List_گرہ*     گرہ )
  {
    FT_List_گرہ  cur;
    FT_Renderer  نتیجہ = عدم;


    إذا ( !library )
      اذهب Exit;

    cur = library->renderers.سر;

    إذا ( گرہ )
    {
      إذا ( *گرہ )
        cur = (*گرہ)->اگلا;
      *گرہ = عدم;
    }

    طالما ( cur )
    {
      FT_Renderer  renderer = FT_RENDERER( cur->انکڑے );


      إذا ( renderer->glyph_format == format )
      {
        إذا ( گرہ )
          *گرہ = cur;

        نتیجہ = renderer;
        اكسر;
      }
      cur = cur->اگلا;
    }

  Exit:
    أرجع نتیجہ;
  }


  ساكن FT_Renderer
  ft_lookup_glyph_renderer( FT_Glyph_تھاں  تھاں )
  {
    FT_Face      face    = تھاں->face;
    FT_Library   library = FT_FACE_LIBRARY( face );
    FT_Renderer  نتیجہ  = library->cur_renderer;


    إذا ( !نتیجہ || نتیجہ->glyph_format != تھاں->format )
      نتیجہ = FT_Lookup_Renderer( library, تھاں->format, 0 );

    أرجع نتیجہ;
  }


  ساكن فراغ
  ft_سیٹ_current_renderer( FT_Library  library )
  {
    FT_Renderer  renderer;


    renderer = FT_Lookup_Renderer( library, FT_GLYPH_FORMAT_OUTLINE, 0 );
    library->cur_renderer = renderer;
  }


  ساكن FT_Error
  ft_جوڑو_renderer( FT_Module  module )
  {
    FT_Library   library = module->library;
    FT_حافظہ    حافظہ  = library->حافظہ;
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
        error = clazz->raster_class->raster_new( حافظہ, &render->raster );
        إذا ( error )
          اذهب Fail;

        render->raster_render = clazz->raster_class->raster_render;
        render->render        = clazz->render_glyph;
      }

#جے_تعریف_شدہ FT_CONFIG_OPTION_SVG
      إذا ( clazz->glyph_format == FT_GLYPH_FORMAT_SVG )
        render->render = clazz->render_glyph;
#جے_انت

      /* add to list */
      گرہ->انکڑے = module;
      FT_List_جوڑو( &library->renderers, گرہ );

      ft_سیٹ_current_renderer( library );
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
    FT_حافظہ    حافظہ;
    FT_List_گرہ  گرہ;


    library = module->library;
    إذا ( !library )
      أرجع;

    حافظہ = library->حافظہ;

    گرہ = FT_List_لبھو( &library->renderers, module );
    إذا ( گرہ )
    {
      FT_Renderer  render = FT_RENDERER( module );


      /* release raster object, if any */
      إذا ( render->raster )
        render->clazz->raster_class->raster_پورا( render->raster );

      /* remove from list */
      FT_List_Remove( &library->renderers, گرہ );
      FT_FREE( گرہ );

      ft_سیٹ_current_renderer( library );
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
  FT_سیٹ_Renderer( FT_Library     library,
                   FT_Renderer    renderer,
                   FT_UInt        عدد_params,
                   FT_Parameter*  parameters )
  {
    FT_List_گرہ  گرہ;
    FT_Error     error = FT_Err_Ok;

    FT_Renderer_سیٹ_منوال_Func  سیٹ_منوال;


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

    گرہ = FT_List_لبھو( &library->renderers, renderer );
    إذا ( !گرہ )
    {
      error = FT_THROW( Invalid_دلیل );
      اذهب Exit;
    }

    FT_List_اتے( &library->renderers, گرہ );

    إذا ( renderer->glyph_format == FT_GLYPH_FORMAT_OUTLINE )
      library->cur_renderer = renderer;

    سیٹ_منوال = renderer->clazz->سیٹ_منوال;

    لكل ( ; عدد_params > 0; عدد_params-- )
    {
      error = سیٹ_منوال( renderer, parameters->ٹیگ, parameters->انکڑے );
      إذا ( error )
        اكسر;
      parameters++;
    }

  Exit:
    أرجع error;
  }


  FT_ادھار_DEF( FT_Error )
  FT_Render_Glyph_اندرونی( FT_Library      library,
                            FT_Glyph_تھاں    تھاں,
                            FT_Render_منوال  render_منوال )
  {
    FT_Error     error = FT_Err_Ok;
    FT_Face      face  = تھاں->face;
    FT_Renderer  renderer;
    FT_List_گرہ  گرہ  = عدم;


    /* try to render colored glyph layers as a special case */
    إذا ( تھاں->اندرونی->لدو_flags & FT_لدو_COLOR &&
         تھاں->format == FT_GLYPH_FORMAT_OUTLINE    )
    {
      FT_LayerIterator  iterator;

      FT_UInt  ادھار_glyph = تھاں->glyph_اشاریہ;

      FT_Bool  have_layers;
      FT_UInt  glyph_اشاریہ;
      FT_UInt  color_اشاریہ;


      iterator.p  = عدم;
      have_layers = FT_Get_Color_Glyph_Layer( face,
                                              ادھار_glyph,
                                              &glyph_اشاریہ,
                                              &color_اشاریہ,
                                              &iterator );
      إذا ( have_layers )
      {
        error = FT_New_Glyph_تھاں( face, عدم );
        إذا ( !error )
        {
          TT_Face       ttface = (TT_Face)face;
          SFNT_Service  sfnt   = (SFNT_Service)ttface->sfnt;


          افعل
          {
            FT_Int32  لدو_flags = تھاں->اندرونی->لدو_flags;


            /* disable the `FT_LOAD_COLOR' flag to avoid recursion */
            /* right here in this function                         */
            لدو_flags &= ~FT_لدو_COLOR;

            /* render into the new `face->glyph' glyph slot */
            لدو_flags |= FT_لدو_RENDER | FT_لدو_NO_BITMAP;

            error = FT_لدو_Glyph( face, glyph_اشاریہ, لدو_flags );
            إذا ( error )
              اكسر;

            /* blend new `face->glyph' into old `slot'; */
            /* at the first call, `slot' is still empty */
            error = sfnt->colr_blend( ttface,
                                      color_اشاریہ,
                                      تھاں,
                                      face->glyph );
            إذا ( error )
              اكسر;

          } طالما ( FT_Get_Color_Glyph_Layer( face,
                                              ادھار_glyph,
                                              &glyph_اشاریہ,
                                              &color_اشاریہ,
                                              &iterator ) );

          إذا ( !error )
            تھاں->format = FT_GLYPH_FORMAT_BITMAP;

          /* this call also restores `slot' as the glyph slot */
          FT_پورا_Glyph_تھاں( face->glyph );
        }

        إذا ( !error )
          أرجع error;

        /* Failed to do the colored layer.  Draw outline instead. */
        تھاں->format = FT_GLYPH_FORMAT_OUTLINE;
      }
    }

    /* small shortcut for the very common case */
    إذا ( تھاں->format == FT_GLYPH_FORMAT_OUTLINE )
    {
      renderer = library->cur_renderer;
      گرہ     = library->renderers.سر;
    }
    وإلا
      renderer = FT_Lookup_Renderer( library, تھاں->format, &گرہ );

    error = FT_ERR( Cannot_Render_Glyph );
    طالما ( renderer )
    {
      error = renderer->render( renderer, تھاں, render_منوال, عدم );
      إذا ( !error                                   ||
           FT_ERR_NEQ( error, Cannot_Render_Glyph ) )
        اكسر;

      /* `FT_Err_Cannot_Render_Glyph` is returned if the render mode */
      /* is unsupported by the current renderer for this glyph image */
      /* format.                                                     */

      /* now, look for another renderer that supports the same */
      /* format.                                               */
      renderer = FT_Lookup_Renderer( library, تھاں->format, &گرہ );
    }

    /* it is not an error if we cannot render a bitmap glyph */
    إذا ( FT_ERR_EQ( error, Cannot_Render_Glyph ) &&
         تھاں->format == FT_GLYPH_FORMAT_BITMAP  )
      error = FT_Err_Ok;

#جے_تعریف_شدہ FT_DEBUG_LEVEL_TRACE

#تعریف_رد  FT_COMPONENT
#تعریف_کرو FT_COMPONENT  checksum

    /*
     * Computing the MD5 checksum is expensive, unnecessarily distorting a
     * possible profiling of FreeType if compiled with tracing support.  For
     * this reason, we execute the following code only if explicitly
     * requested.
     */

    /* we use FT_TRACE3 in this block */
    إذا ( !error                               &&
         ft_trace_پدھراں[trace_checksum] >= 3 &&
         تھاں->bitmap.بفر                  )
    {
      FT_Bitmap  bitmap;
      FT_Error   err;


      FT_Bitmap_Init( &bitmap );

      /* we convert to a single bitmap format for computing the checksum */
      /* this also converts the bitmap flow to `down' (i.e., pitch > 0)  */
      err = FT_Bitmap_Convert( library, &تھاں->bitmap, &bitmap, 1 );
      إذا ( !err )
      {
        MD5_CTX        ctx;
        بنانشانی حرف  md5[16];
        بنانشانی لما  coverage = 0;
        صحيح            i, j;
        صحيح            صفاں  = (صحيح)bitmap.صفاں;
        صحيح            pitch = bitmap.pitch;


        FT_TRACE3(( "FT_Render_Glyph: bitmap %dx%d, %s (mode %d)\n",
                    pitch,
                    صفاں,
                    pixel_modes[تھاں->bitmap.pixel_منوال],
                    تھاں->bitmap.pixel_منوال ));

        لكل ( i = 0; i < صفاں; i++ )
          لكل ( j = 0; j < pitch; j++ )
            coverage += bitmap.بفر[i * pitch + j];

        FT_TRACE3(( "  Total coverage: %lu\n", coverage ));

        MD5_Init( &ctx );
        إذا ( bitmap.بفر )
          MD5_Update( &ctx, bitmap.بفر,
                      (بنانشانی لما)صفاں * (بنانشانی لما)pitch );
        MD5_Final( md5, &ctx );

        FT_TRACE3(( "  MD5 checksum: " ));
        لكل ( i = 0; i < 16; i++ )
          FT_TRACE3(( "%02X", md5[i] ));
        FT_TRACE3(( "\n" ));
      }

      FT_Bitmap_پورا( library, &bitmap );
    }

    /*
     * Dump bitmap in Netpbm format (PBM or PGM).
     */

    /* we use FT_TRACE7 in this block */
    إذا ( !error                               &&
         ft_trace_پدھراں[trace_checksum] >= 7 &&
         تھاں->bitmap.بفر                  )
    {
      إذا ( تھاں->bitmap.صفاں  < 128U &&
           تھاں->bitmap.چوڑائی < 128U )
      {
        صحيح  صفاں  = (صحيح)تھاں->bitmap.صفاں;
        صحيح  چوڑائی = (صحيح)تھاں->bitmap.چوڑائی;
        صحيح  pitch =      تھاں->bitmap.pitch;
        صحيح  i, j, m;

        بنانشانی حرف*  topleft = تھاں->bitmap.بفر;


        إذا ( pitch < 0 )
          topleft -= pitch * ( صفاں - 1 );

        FT_TRACE7(( "Netpbm image: start\n" ));
        بدل ( تھاں->bitmap.pixel_منوال )
        {
        حالة FT_PIXEL_منوال_MONO:
          FT_TRACE7(( "P1 %d %d\n", چوڑائی, صفاں ));
          لكل ( i = 0; i < صفاں; i++ )
          {
            لكل ( j = 0; j < چوڑائی; )
              لكل ( m = 128; m > 0 && j < چوڑائی; m >>= 1, j++ )
                FT_TRACE7(( " %d",
                            ( topleft[i * pitch + j / 8] & m ) != 0 ));
            FT_TRACE7(( "\n" ));
          }
          اكسر;

        افتراضي:
          FT_TRACE7(( "P2 %d %d 255\n", چوڑائی, صفاں ));
          لكل ( i = 0; i < صفاں; i++ )
          {
            لكل ( j = 0; j < چوڑائی; j += 1 )
              FT_TRACE7(( " %3u", topleft[i * pitch + j] ));
            FT_TRACE7(( "\n" ));
          }
        }
        FT_TRACE7(( "Netpbm image: end\n" ));
      }
      وإلا
        FT_TRACE7(( "Netpbm image: too large, omitted\n" ));
    }

#تعریف_رد  FT_COMPONENT
#تعریف_کرو FT_COMPONENT  objs

#جے_انت /* FT_DEBUG_LEVEL_TRACE */

    أرجع error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Render_Glyph( FT_Glyph_تھاں    تھاں,
                   FT_Render_منوال  render_منوال )
  {
    FT_Library  library;


    إذا ( !تھاں || !تھاں->face )
      أرجع FT_THROW( Invalid_دلیل );

    library = FT_FACE_LIBRARY( تھاں->face );

    أرجع FT_Render_Glyph_اندرونی( library, تھاں, render_منوال );
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
    FT_حافظہ               حافظہ  = module->حافظہ;


    إذا ( library && library->auto_hinter == module )
      library->auto_hinter = عدم;

    /* if the module is a renderer */
    إذا ( FT_MODULE_IS_RENDERER( module ) )
      ft_remove_renderer( module );

    /* if the module is a font driver, add some steps */
    إذا ( FT_MODULE_IS_DRIVER( module ) )
      Destroy_Driver( FT_DRIVER( module ) );

    /* finalize the module object */
    إذا ( clazz->module_پورا )
      clazz->module_پورا( module );

    /* discard it */
    FT_FREE( module );
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_جوڑو_Module( FT_Library              library,
                 ثابت FT_Module_Class*  clazz )
  {
    FT_Error   error;
    FT_حافظہ  حافظہ;
    FT_Module  module = عدم;
    FT_UInt    nn;


#تعریف_کرو FREETYPE_VER_FIXED  ( ( (FT_Long)FREETYPE_MAJOR << 16 ) | \
                                FREETYPE_MINOR                  )

    إذا ( !library )
      أرجع FT_THROW( Invalid_Library_Handle );

    إذا ( !clazz )
      أرجع FT_THROW( Invalid_دلیل );

    /* check FreeType version */
    إذا ( clazz->module_requires > FREETYPE_VER_FIXED )
      أرجع FT_THROW( Invalid_نسخہ );

    /* look for a module with the same name in the library's table */
    لكل ( nn = 0; nn < library->عدد_modules; nn++ )
    {
      module = library->modules[nn];
      إذا ( ft_strcmp( module->clazz->module_name, clazz->module_name ) == 0 )
      {
        /* this installed module has the same name, compare their versions */
        إذا ( clazz->module_نسخہ <= module->clazz->module_نسخہ )
          أرجع FT_THROW( ہیٹھلا_Module_نسخہ );

        /* remove the module from our list, then exit the loop to replace */
        /* it by our new version..                                        */
        FT_Remove_Module( library, module );
        اكسر;
      }
    }

    حافظہ = library->حافظہ;
    error  = FT_Err_Ok;

    إذا ( library->عدد_modules >= FT_MAX_MODULES )
    {
      error = FT_THROW( Too_Many_Drivers );
      اذهب Exit;
    }

    /* allocate module object */
    إذا ( FT_ALLOC( module, clazz->module_ناپ ) )
      اذهب Exit;

    /* base initialization */
    module->clazz   = clazz;
    module->library = library;
    module->حافظہ  = حافظہ;

    /* check whether the module is a renderer - this must be performed */
    /* before the normal module initialization                         */
    إذا ( FT_MODULE_IS_RENDERER( module ) )
    {
      /* add to the renderers list */
      error = ft_جوڑو_renderer( module );
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
        renderer->clazz->raster_class->raster_پورا( renderer->raster );
    }

    FT_FREE( module );
    اذهب Exit;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Module )
  FT_Get_Module( FT_Library   library,
                 ثابت حرف*  module_name )
  {
    FT_Module   نتیجہ = عدم;
    FT_Module*  cur;
    FT_Module*  حد_بندی;


    إذا ( !library || !module_name )
      أرجع نتیجہ;

    cur   = library->modules;
    حد_بندی = cur + library->عدد_modules;

    لكل ( ; cur < حد_بندی; cur++ )
      إذا ( ft_strcmp( cur[0]->clazz->module_name, module_name ) == 0 )
      {
        نتیجہ = cur[0];
        اكسر;
      }

    أرجع نتیجہ;
  }


  /* documentation is in ftobjs.h */

  FT_ادھار_DEF( ثابت فراغ* )
  FT_Get_Module_Interface( FT_Library   library,
                           ثابت حرف*  mod_name )
  {
    FT_Module  module;


    /* test for valid `library' delayed to FT_Get_Module() */

    module = FT_Get_Module( library, mod_name );

    أرجع module ? module->clazz->module_interface : 0;
  }


  FT_ادھار_DEF( FT_اشارہ_گر )
  ft_module_get_service( FT_Module    module,
                         ثابت حرف*  service_id,
                         FT_Bool      global )
  {
    FT_اشارہ_گر  نتیجہ = عدم;


    إذا ( module )
    {
      FT_ASSERT( module->clazz && module->clazz->get_interface );

      /* first, look for the service in the module */
      إذا ( module->clazz->get_interface )
        نتیجہ = module->clazz->get_interface( module, service_id );

      إذا ( global && !نتیجہ )
      {
        /* we didn't find it, look in all other modules then */
        FT_Library  library = module->library;
        FT_Module*  cur     = library->modules;
        FT_Module*  حد_بندی   = cur + library->عدد_modules;


        لكل ( ; cur < حد_بندی; cur++ )
        {
          إذا ( cur[0] != module )
          {
            FT_ASSERT( cur[0]->clazz );

            إذا ( cur[0]->clazz->get_interface )
            {
              نتیجہ = cur[0]->clazz->get_interface( cur[0], service_id );
              إذا ( نتیجہ )
                اكسر;
            }
          }
        }
      }
    }

    أرجع نتیجہ;
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
      FT_Module*  حد_بندی = cur + library->عدد_modules;


      لكل ( ; cur < حد_بندی; cur++ )
      {
        إذا ( cur[0] == module )
        {
          /* remove it from the table */
          library->عدد_modules--;
          حد_بندی--;
          طالما ( cur < حد_بندی )
          {
            cur[0] = cur[1];
            cur++;
          }
          حد_بندی[0] = عدم;

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
                  FT_Bool           سیٹ,
                  FT_Bool           قدر_is_string )
  {
    FT_Module*           cur;
    FT_Module*           حد_بندی;
    FT_Module_Interface  interface;

    FT_Service_Properties  service;

#جے_تعریف_شدہ FT_DEBUG_LEVEL_ERROR
    ثابت FT_String*  سیٹ_name  = "FT_Property_Set";
    ثابت FT_String*  get_name  = "FT_Property_Get";
    ثابت FT_String*  func_name = سیٹ ? سیٹ_name : get_name;
#جے_انت

    FT_Bool  missing_func;


    إذا ( !library )
      أرجع FT_THROW( Invalid_Library_Handle );

    إذا ( !module_name || !property_name || !قدر )
      أرجع FT_THROW( Invalid_دلیل );

    cur   = library->modules;
    حد_بندی = cur + library->عدد_modules;

    /* search module */
    لكل ( ; cur < حد_بندی; cur++ )
      إذا ( !ft_strcmp( cur[0]->clazz->module_name, module_name ) )
        اكسر;

    إذا ( cur == حد_بندی )
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

    إذا ( سیٹ )
      missing_func = FT_BOOL( !service->سیٹ_property );
    وإلا
      missing_func = FT_BOOL( !service->get_property );

    إذا ( missing_func )
    {
      FT_TRACE2(( "%s: property service of module `%s' is broken\n",
                  func_name, module_name ));
      أرجع FT_THROW( Unimplemented_Feature );
    }

    أرجع سیٹ ? service->سیٹ_property( cur[0],
                                        property_name,
                                        قدر,
                                        قدر_is_string )
               : service->get_property( cur[0],
                                        property_name,
                                        قدر );
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Property_سیٹ( FT_Library        library,
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


#جے_تعریف_شدہ FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES

  /* this variant is used for handling the FREETYPE_PROPERTIES */
  /* environment variable                                      */

  FT_ادھار_DEF( FT_Error )
  ft_property_string_سیٹ( FT_Library        library,
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

#جے_انت


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
  FT_New_Library( FT_حافظہ    حافظہ,
                  FT_Library  *alibrary )
  {
    FT_Library  library = عدم;
    FT_Error    error;


    إذا ( !حافظہ || !alibrary )
      أرجع FT_THROW( Invalid_دلیل );

#جے_بنا_تعریف FT_DEBUG_LOGGING
#جے_تعریف_شدہ FT_DEBUG_LEVEL_ERROR
    /* init debugging support */
    ft_debug_init();
#جے_انت /* FT_DEBUG_LEVEL_ERROR */
#جے_انت /* !FT_DEBUG_LOGGING */

    /* first of all, allocate the library object */
    إذا ( FT_NEW( library ) )
      أرجع error;

    library->حافظہ = حافظہ;

    library->نسخہ_major = FREETYPE_MAJOR;
    library->نسخہ_minor = FREETYPE_MINOR;
    library->نسخہ_patch = FREETYPE_PATCH;

    library->refcount = 1;

    /* That's ok now */
    *alibrary = library;

    أرجع FT_Err_Ok;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( فراغ )
  FT_Library_نسخہ( FT_Library   library,
                      FT_Int      *amajor,
                      FT_Int      *aminor,
                      FT_Int      *apatch )
  {
    FT_Int  major = 0;
    FT_Int  minor = 0;
    FT_Int  patch = 0;


    إذا ( library )
    {
      major = library->نسخہ_major;
      minor = library->نسخہ_minor;
      patch = library->نسخہ_patch;
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
  FT_پورا_Library( FT_Library  library )
  {
    FT_حافظہ  حافظہ;


    إذا ( !library )
      أرجع FT_THROW( Invalid_Library_Handle );

    library->refcount--;
    إذا ( library->refcount > 0 )
      اذهب Exit;

    حافظہ = library->حافظہ;

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
          طالما ( faces->سر )
          {
            FT_پورا_Face( FT_FACE( faces->سر->انکڑے ) );
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
#جے_انت

    FT_FREE( library );

  Exit:
    أرجع FT_Err_Ok;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( فراغ )
  FT_سیٹ_Debug_Hook( FT_Library         library,
                     FT_UInt            hook_اشاریہ,
                     FT_DebugHook_Func  debug_hook )
  {
    إذا ( library && debug_hook &&
         hook_اشاریہ <
           ( حجم ( library->debug_hooks ) / حجم ( فراغ* ) ) )
      library->debug_hooks[hook_اشاریہ] = debug_hook;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_TrueTypeEngineType )
  FT_Get_TrueType_Engine_Type( FT_Library  library )
  {
    FT_TrueTypeEngineType  نتیجہ = FT_TRUETYPE_ENGINE_TYPE_NONE;


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
          نتیجہ = service->engine_type;
      }
    }

    أرجع نتیجہ;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_SubGlyph_Info( FT_Glyph_تھاں  glyph,
                        FT_UInt       sub_اشاریہ,
                        FT_Int       *p_اشاریہ,
                        FT_UInt      *p_flags,
                        FT_Int       *p_arg1,
                        FT_Int       *p_arg2,
                        FT_میٹرکس    *p_transform )
  {
    FT_Error  error = FT_ERR( Invalid_دلیل );


    إذا ( glyph                                      &&
         glyph->subglyphs                           &&
         glyph->format == FT_GLYPH_FORMAT_مرکب &&
         sub_اشاریہ < glyph->عدد_subglyphs           )
    {
      FT_SubGlyph  subg = glyph->subglyphs + sub_اشاریہ;


      *p_اشاریہ     = subg->index;
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
                            FT_UInt            ادھار_glyph,
                            FT_UInt           *aglyph_اشاریہ,
                            FT_UInt           *acolor_اشاریہ,
                            FT_LayerIterator*  iterator )
  {
    TT_Face       ttface;
    SFNT_Service  sfnt;


    إذا ( !face                                   ||
         !aglyph_اشاریہ                           ||
         !acolor_اشاریہ                           ||
         !iterator                               ||
         ادھار_glyph >= (FT_UInt)face->عدد_glyphs )
      أرجع 0;

    إذا ( !FT_IS_SFNT( face ) )
      أرجع 0;

    ttface = (TT_Face)face;
    sfnt   = (SFNT_Service)ttface->sfnt;

    إذا ( sfnt->get_colr_layer )
      أرجع sfnt->get_colr_layer( ttface,
                                   ادھار_glyph,
                                   aglyph_اشاریہ,
                                   acolor_اشاریہ,
                                   iterator );
    وإلا
      أرجع 0;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Get_Color_Glyph_Paint( FT_Face                  face,
                            FT_UInt                  ادھار_glyph,
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
                                         ادھار_glyph,
                                         جڑ_transform,
                                         paint );
    وإلا
      أرجع 0;
  }


  /* documentation is in ftcolor.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Get_Color_Glyph_ClipBox( FT_Face      face,
                              FT_UInt      ادھار_glyph,
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
                                            ادھار_glyph,
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


/*سرا*/
