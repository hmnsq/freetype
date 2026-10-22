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


#شامل <freetype/ftlist.h>
#شامل <freetype/ftoutln.h>
#شامل <freetype/ftfntfmt.h>
#شامل <freetype/otsvg.h>

#شامل <freetype/internal/ftvalid.h>
#شامل <freetype/internal/ftobjs.h>
#شامل <freetype/internal/ftdebug.h>
#شامل <freetype/internal/ftrfork.h>
#شامل <freetype/internal/ftstream.h>
#شامل <freetype/internal/sfnt.h>          /* for SFNT_Load_Table_Func */
#شامل <freetype/internal/psaux.h>         /* for PS_Driver            */
#شامل <freetype/internal/svginterface.h>

#شامل <freetype/tttables.h>
#شامل <freetype/tttags.h>
#شامل <freetype/ttnameid.h>

#شامل <freetype/internal/services/svprop.h>
#شامل <freetype/internal/services/svsfnt.h>
#شامل <freetype/internal/services/svpostnm.h>
#شامل <freetype/internal/services/svgldict.h>
#شامل <freetype/internal/services/svttcmap.h>
#شامل <freetype/internal/services/svkern.h>
#شامل <freetype/internal/services/svtteng.h>

#اگر_تعریف_شدہ TT_CONFIG_OPTION_VARC
#شامل <freetype/internal/services/svvarc.h>
#اگر_ختم

#شامل <freetype/ftdriver.h>

#اگر_تعریف_شدہ FT_CONFIG_OPTION_MAC_FONTS
#شامل "ftbase.h"
#اگر_ختم


#اگر_تعریف_شدہ FT_DEBUG_LEVEL_TRACE

#شامل <freetype/ftbitmap.h>

#اگر defined( _MSC_VER )      /* Visual C++ (and Intel C++)   */
  /* We disable the warning `conversion from XXX to YYY,     */
  /* possible loss of data' in order to compile cleanly with */
  /* the maximum level of warnings: `md5.c' is non-FreeType  */
  /* code, and it gets used during development builds only.  */
#ہدایت warning( ڈالو )
#ہدایت warning( disable : 4244 )
#اگر_ختم /* _MSC_VER */

  /* It's easiest to include `md5.c' directly.  However, since OpenSSL */
  /* also provides the same functions, there might be conflicts if     */
  /* both FreeType and OpenSSL are built as static libraries.  For     */
  /* this reason, we put the MD5 stuff into the `FT_' namespace.       */
#تعریف_کرو MD5_u32plus  FT_MD5_u32plus
#تعریف_کرو MD5_CTX      FT_MD5_CTX
#تعریف_کرو MD5_Init     FT_MD5_Init
#تعریف_کرو MD5_Update   FT_MD5_Update
#تعریف_کرو MD5_Final    FT_MD5_Final

#تعریف_منسوخ  HAVE_OPENSSL

#شامل "md5.c"

#اگر defined( _MSC_VER )
#ہدایت warning( نکالو )
#اگر_ختم

  /* This array must stay in sync with the @FT_Pixel_Mode enumeration */
  /* (in file `ftimage.h`).                                           */

  ساکن ثابت حرف* ثابت  pixel_modes[] =
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

#اگر_ختم /* FT_DEBUG_LEVEL_TRACE */


#تعریف_کرو GRID_FIT_METRICS


  /* forward declaration */
  ساکن FT_Error
  ft_open_face_اندرونی( FT_Library           library,
                         ثابت FT_Open_دلائل*  دلائل,
                         FT_Long              face_اشاریہ,
                         FT_Face             *aface,
                         FT_Bool              test_mac_fonts );


  FT_بنیاد_DEF( FT_اشارہ_گر )
  ft_service_list_lookup( FT_ServiceDesc  service_descriptors,
                          ثابت حرف*     service_id )
  {
    FT_اشارہ_گر      نتیجہ = تہی;
    FT_ServiceDesc  desc   = service_descriptors;


    اگر ( desc && service_id )
    {
      برائے ( ; desc->serv_id != تہی; desc++ )
      {
        اگر ( ft_strcmp( desc->serv_id, service_id ) == 0 )
        {
          نتیجہ = (FT_اشارہ_گر)desc->serv_ڈیٹا;
          توڑو;
        }
      }
    }

    واپس نتیجہ;
  }


  FT_بنیاد_DEF( خالی )
  ft_validator_init( FT_Validator        valid,
                     ثابت FT_Byte*      بنیاد,
                     ثابت FT_Byte*      حد_بندی,
                     FT_ValidationLevel  level )
  {
    valid->بنیاد  = بنیاد;
    valid->حد_بندی = حد_بندی;
    valid->level = level;
    valid->error = FT_Err_Ok;
  }


  FT_بنیاد_DEF( FT_Int )
  ft_validator_تسلسل( FT_Validator  valid )
  {
    /* This function doesn't work!  None should call it. */
    FT_غیر_مستعمل( valid );

    واپس -1;
  }


  FT_بنیاد_DEF( خالی )
  ft_validator_error( FT_Validator  valid,
                      FT_Error      error )
  {
    /* since the cast below also disables the compiler's */
    /* type check, we introduce a dummy variable, which  */
    /* will be optimized away                            */
    متلون ft_jmp_buf* jump_بفر = &valid->jump_بفر;


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
  FT_بنیاد_DEF( FT_Error )
  FT_دھارا_New( FT_Library           library,
                 ثابت FT_Open_دلائل*  دلائل,
                 FT_دھارا           *astream )
  {
    FT_Error   error;
    FT_حافظہ  حافظہ;
    FT_دھارا  دھارا = تہی;
    FT_UInt    منوال;


    *astream = تہی;

    اگر ( !library )
      واپس FT_THROW( Invalid_Library_Handle );

    اگر ( !دلائل )
      واپس FT_THROW( Invalid_دلیل );

    حافظہ = library->حافظہ;
    منوال   = دلائل->flags &
               ( FT_OPEN_حافظہ | FT_OPEN_دھارا | FT_OPEN_PATHNAME );

    اگر ( منوال == FT_OPEN_حافظہ )
    {
      /* create a memory-based stream */
      اگر ( FT_NEW( دھارا ) )
        جاؤ Exit;

      FT_دھارا_Open_حافظہ( دھارا,
                            (ثابت FT_Byte*)دلائل->حافظہ_بنیاد,
                            (FT_ULong)دلائل->حافظہ_سائز );
      دھارا->حافظہ = حافظہ;
    }

#اگر_غیر_تعریف_شدہ FT_CONFIG_OPTION_DISABLE_دھارا_SUPPORT

    ورنہ اگر ( منوال == FT_OPEN_PATHNAME )
    {
      /* create a normal system stream */
      اگر ( FT_NEW( دھارا ) )
        جاؤ Exit;

      دھارا->حافظہ = حافظہ;
      error = FT_دھارا_Open( دھارا, دلائل->pathname );
      اگر ( error )
        FT_FREE( دھارا );
    }
    ورنہ اگر ( ( منوال == FT_OPEN_دھارا ) && دلائل->دھارا )
    {
      /* use an existing, user-provided stream */

      /* in this case, we do not need to allocate a new stream object */
      /* since the caller is responsible for closing it himself       */
      دھارا         = دلائل->دھارا;
      دھارا->حافظہ = حافظہ;
      error          = FT_Err_Ok;
    }

#اگر_ختم

    ورنہ
    {
      error = FT_THROW( Invalid_دلیل );
      اگر ( ( دلائل->flags & FT_OPEN_دھارا ) && دلائل->دھارا )
        FT_دھارا_Close( دلائل->دھارا );
    }

    اگر ( !error )
      *astream       = دھارا;

  Exit:
    واپس error;
  }


  FT_بنیاد_DEF( خالی )
  FT_دھارا_Free( FT_دھارا  دھارا,
                  FT_Int     external )
  {
    اگر ( دھارا )
    {
      FT_حافظہ  حافظہ = دھارا->حافظہ;


      FT_دھارا_Close( دھارا );

      اگر ( !external )
        FT_FREE( دھارا );
    }
  }


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#تعریف_منسوخ  FT_COMPONENT
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


  ساکن FT_Error
  ft_glyphslot_init( FT_Glyph_خانہ  خانہ )
  {
    FT_Driver         driver   = خانہ->face->driver;
    FT_Driver_Class   clazz    = driver->clazz;
    FT_حافظہ         حافظہ   = driver->جڑ.حافظہ;
    FT_Error          error    = FT_Err_Ok;
    FT_خانہ_اندرونی  اندرونی_2 = تہی;


    خانہ->library = driver->جڑ.library;

    اگر ( FT_NEW( اندرونی_2 ) )
      جاؤ Exit;

    خانہ->اندرونی_2 = اندرونی_2;

    اگر ( FT_DRIVER_استعمال_کرتا_OUTLINES( driver ) )
      error = FT_GlyphLoader_New( حافظہ, &اندرونی_2->loader );

    اگر ( !error && clazz->init_خانہ )
      error = clazz->init_خانہ( خانہ );

#اگر_تعریف_شدہ FT_CONFIG_OPTION_SVG
    /* if SVG table exists, allocate the space in `slot->other` */
    اگر ( خانہ->face->face_flags & FT_FACE_FLAG_SVG )
    {
      FT_SVG_Document  document = تہی;


      اگر ( FT_NEW( document ) )
        جاؤ Exit;
      خانہ->other = document;
    }
#اگر_ختم

  Exit:
    واپس error;
  }


  FT_بنیاد_DEF( خالی )
  ft_glyphslot_free_bitmap( FT_Glyph_خانہ  خانہ )
  {
    اگر ( خانہ->اندرونی_2 && ( خانہ->اندرونی_2->flags & FT_GLYPH_OWN_BITMAP ) )
    {
      FT_حافظہ  حافظہ = FT_FACE_حافظہ( خانہ->face );


      FT_FREE( خانہ->bitmap.بفر );
      خانہ->اندرونی_2->flags &= ~FT_GLYPH_OWN_BITMAP;
    }
    ورنہ
    {
      /* assume that the bitmap buffer was stolen or not */
      /* allocated from the heap                         */
      خانہ->bitmap.بفر = تہی;
    }
  }


  /* overflow-resistant presetting of bitmap position and dimensions; */
  /* also check whether the size is too large for rendering           */
  FT_بنیاد_DEF( FT_Bool )
  ft_glyphslot_preset_bitmap( FT_Glyph_خانہ      خانہ,
                              FT_Render_منوال    منوال,
                              ثابت FT_سمتیہ*  origin )
  {
    FT_Outline*  outline = &خانہ->outline;
    FT_Bitmap*   bitmap  = &خانہ->bitmap;

    FT_Pixel_منوال  pixel_منوال;

    FT_BBox  cbox, pbox;
    FT_Pos   x_سرکاؤ = 0;
    FT_Pos   y_سرکاؤ = 0;
    FT_Pos   x_بایاں, y_چوٹی;
    FT_Pos   چوڑائی, اونچائی, pitch;
    FT_Bool  ret;


    اگر ( خانہ->format == FT_GLYPH_FORMAT_SVG )
    {
      FT_Module    module;
      SVG_Service  svg_service;


      module      = FT_Get_Module( خانہ->library, "ot-svg" );
      svg_service = (SVG_Service)module->clazz->module_interface;

      واپس (FT_Bool)svg_service->preset_خانہ( module, خانہ, FALSE );
    }
    ورنہ اگر ( خانہ->format != FT_GLYPH_FORMAT_OUTLINE )
      واپس 1;

    اگر ( origin )
    {
      x_سرکاؤ = origin->x;
      y_سرکاؤ = origin->y;
    }

    /* compute the control box, and grid-fit it, */
    /* taking into account the origin shift      */
    FT_Outline_Get_CBox( outline, &cbox );

    /* rough estimate of pixel box */
    pbox.xMin = ( cbox.xMin >> 6 ) + ( x_سرکاؤ >> 6 );
    pbox.yMin = ( cbox.yMin >> 6 ) + ( y_سرکاؤ >> 6 );
    pbox.xMax = ( cbox.xMax >> 6 ) + ( x_سرکاؤ >> 6 );
    pbox.yMax = ( cbox.yMax >> 6 ) + ( y_سرکاؤ >> 6 );

    /* tiny remainder box */
    cbox.xMin = ( cbox.xMin & 63 ) + ( x_سرکاؤ & 63 );
    cbox.yMin = ( cbox.yMin & 63 ) + ( y_سرکاؤ & 63 );
    cbox.xMax = ( cbox.xMax & 63 ) + ( x_سرکاؤ & 63 );
    cbox.yMax = ( cbox.yMax & 63 ) + ( y_سرکاؤ & 63 );

    انتخاب ( منوال )
    {
    حالت FT_RENDER_منوال_MONO:
      pixel_منوال = FT_PIXEL_منوال_MONO;
#اگر 1
      /* x */

      /* undocumented but confirmed: bbox values get rounded;    */
      /* we do asymmetric rounding so that the center of a pixel */
      /* gets always included                                    */

      pbox.xMin += ( cbox.xMin + 31 ) >> 6;
      pbox.xMax += ( cbox.xMax + 32 ) >> 6;

      /* if the bbox collapsed, we add a pixel based on the total */
      /* rounding remainder to cover most of the original cbox    */

      اگر ( pbox.xMin == pbox.xMax )
      {
        اگر ( ( ( cbox.xMin + 31 ) & 63 ) - 31 +
             ( ( cbox.xMax + 32 ) & 63 ) - 32 < 0 )
          pbox.xMin -= 1;
        ورنہ
          pbox.xMax += 1;
      }

      /* y */

      pbox.yMin += ( cbox.yMin + 31 ) >> 6;
      pbox.yMax += ( cbox.yMax + 32 ) >> 6;

      اگر ( pbox.yMin == pbox.yMax )
      {
        اگر ( ( ( cbox.yMin + 31 ) & 63 ) - 31 +
             ( ( cbox.yMax + 32 ) & 63 ) - 32 < 0 )
          pbox.yMin -= 1;
        ورنہ
          pbox.yMax += 1;
      }

      توڑو;
#ورنہ
      جاؤ Adjust;
#اگر_ختم

    حالت FT_RENDER_منوال_LCD:
      pixel_منوال = FT_PIXEL_منوال_LCD;
      ft_lcd_padding( &cbox, خانہ, منوال );
      جاؤ Adjust;

    حالت FT_RENDER_منوال_LCD_V:
      pixel_منوال = FT_PIXEL_منوال_LCD_V;
      ft_lcd_padding( &cbox, خانہ, منوال );
      جاؤ Adjust;

    حالت FT_RENDER_منوال_معمول:
    حالت FT_RENDER_منوال_LIGHT:
    طےشدہ:
      pixel_منوال = FT_PIXEL_منوال_سرمئی;
    Adjust:
      pbox.xMin += cbox.xMin >> 6;
      pbox.yMin += cbox.yMin >> 6;
      pbox.xMax += ( cbox.xMax + 63 ) >> 6;
      pbox.yMax += ( cbox.yMax + 63 ) >> 6;
    }

    x_بایاں = pbox.xMin;
    y_چوٹی  = pbox.yMax;

    چوڑائی  = pbox.xMax - pbox.xMin;
    اونچائی = pbox.yMax - pbox.yMin;

    /* Flag the width or height unsuitable for rendering.   */
    /* The limit is based on the ppem value when available. */
    /* FT_Renderer modules should check the return value.   */
    ret = FT_BOOL(     چوڑائی >= 0x10000   ||    اونچائی >= 0x10000   ||
                   pbox.xMin < -0x1000000 || pbox.xMax >= 0x1000000 ||
                   pbox.yMin < -0x1000000 || pbox.yMax >= 0x1000000 ||
             ( خانہ->face                                         &&
               ( چوڑائی  > 16 * خانہ->face->سائز->metrics.x_ppem ||
                 اونچائی > 16 * خانہ->face->سائز->metrics.y_ppem ) ) );

    اگر ( ret )
      FT_TRACE3(( "ft_glyphslot_preset_bitmap: [%ld %ld %ld %ld]\n",
                  pbox.xMin, pbox.yMin, pbox.xMax, pbox.yMax ));

    انتخاب ( pixel_منوال )
    {
    حالت FT_PIXEL_منوال_MONO:
      pitch = ( ( چوڑائی + 15 ) >> 4 ) << 1;
      توڑو;

    حالت FT_PIXEL_منوال_LCD:
      چوڑائی *= 3;
      pitch  = FT_PAD_CEIL( چوڑائی, 4 );
      توڑو;

    حالت FT_PIXEL_منوال_LCD_V:
      اونچائی *= 3;
      FALL_THROUGH;

    حالت FT_PIXEL_منوال_سرمئی:
    طےشدہ:
      pitch = چوڑائی;
    }

    خانہ->bitmap_بایاں = (FT_Int)x_بایاں;
    خانہ->bitmap_چوٹی  = (FT_Int)y_چوٹی;

    bitmap->pixel_منوال = (غیرعلامتی حرف)pixel_منوال;
    bitmap->عدد_grays  = 256;
    bitmap->چوڑائی      = (غیرعلامتی صحیح)چوڑائی;
    bitmap->صفیں       = (غیرعلامتی صحیح)اونچائی;
    bitmap->pitch      = pitch;

    واپس ret;
  }


  FT_بنیاد_DEF( خالی )
  ft_glyphslot_سیٹ_bitmap( FT_Glyph_خانہ  خانہ,
                           FT_Byte*      بفر )
  {
    ft_glyphslot_free_bitmap( خانہ );

    خانہ->bitmap.بفر = بفر;

    FT_ASSERT( (خانہ->اندرونی_2->flags & FT_GLYPH_OWN_BITMAP) == 0 );
  }


  FT_بنیاد_DEF( FT_Error )
  ft_glyphslot_alloc_bitmap( FT_Glyph_خانہ  خانہ )
  {
    FT_حافظہ  حافظہ = FT_FACE_حافظہ( خانہ->face );
    FT_Error   error;


    اگر ( خانہ->اندرونی_2->flags & FT_GLYPH_OWN_BITMAP )
      FT_FREE( خانہ->bitmap.بفر );
    ورنہ
      خانہ->اندرونی_2->flags |= FT_GLYPH_OWN_BITMAP;

    /* dimensions must be preset */
    FT_MEM_ALLOC_MULT( خانہ->bitmap.بفر,
                       خانہ->bitmap.صفیں,
                       خانہ->bitmap.pitch );
    واپس error;
  }


  ساکن خالی
  ft_glyphslot_clear( FT_Glyph_خانہ  خانہ )
  {
    /* free bitmap if needed */
    ft_glyphslot_free_bitmap( خانہ );

    /* clear all public fields in the glyph slot */
    خانہ->glyph_اشاریہ = 0;

    FT_ZERO( &خانہ->metrics );
    FT_ZERO( &خانہ->outline );

    خانہ->bitmap.چوڑائی      = 0;
    خانہ->bitmap.صفیں       = 0;
    خانہ->bitmap.pitch      = 0;
    خانہ->bitmap.pixel_منوال = 0;
    /* `slot->bitmap.buffer' has been handled by ft_glyphslot_free_bitmap */

    خانہ->bitmap_بایاں   = 0;
    خانہ->bitmap_چوٹی    = 0;
    خانہ->عدد_subglyphs = 0;
    خانہ->subglyphs     = تہی;
    خانہ->control_ڈیٹا  = تہی;
    خانہ->control_len   = 0;

#اگر_غیر_تعریف_شدہ FT_CONFIG_OPTION_SVG
    خانہ->other = تہی;
#ورنہ
    اگر ( !( خانہ->face->face_flags & FT_FACE_FLAG_SVG ) )
      خانہ->other = تہی;
    ورنہ
    {
      اگر ( خانہ->اندرونی_2->flags & FT_GLYPH_OWN_GZIP_SVG )
      {
        FT_حافظہ        حافظہ = خانہ->face->حافظہ;
        FT_SVG_Document  doc    = (FT_SVG_Document)خانہ->other;


        FT_FREE( doc->svg_document );
        خانہ->اندرونی_2->flags &= ~FT_GLYPH_OWN_GZIP_SVG;
      }
    }
#اگر_ختم

    خانہ->format = FT_GLYPH_FORMAT_NONE;

    خانہ->خطی_Hori_Advance = 0;
    خانہ->خطی_Vert_Advance = 0;
    خانہ->advance.x         = 0;
    خانہ->advance.y         = 0;
    خانہ->lsb_delta         = 0;
    خانہ->rsb_delta         = 0;
  }


  ساکن خالی
  ft_glyphslot_مکمل( FT_Glyph_خانہ  خانہ )
  {
    FT_Driver        driver = خانہ->face->driver;
    FT_Driver_Class  clazz  = driver->clazz;
    FT_حافظہ        حافظہ = driver->جڑ.حافظہ;

#اگر_تعریف_شدہ FT_CONFIG_OPTION_SVG
    اگر ( خانہ->face->face_flags & FT_FACE_FLAG_SVG )
    {
      /* Free memory in case SVG was there.                          */
      /* `slot->internal` might be NULL in out-of-memory situations. */
      اگر ( خانہ->اندرونی_2 && خانہ->اندرونی_2->flags & FT_GLYPH_OWN_GZIP_SVG )
      {
        FT_SVG_Document  doc = (FT_SVG_Document)خانہ->other;


        FT_FREE( doc->svg_document );

        خانہ->اندرونی_2->flags &= ~FT_GLYPH_OWN_GZIP_SVG;
      }

      FT_FREE( خانہ->other );
    }
#اگر_ختم

    اگر ( clazz->مکمل_خانہ )
      clazz->مکمل_خانہ( خانہ );

    /* free bitmap buffer if needed */
    ft_glyphslot_free_bitmap( خانہ );

    /* slot->internal might be NULL in out-of-memory situations */
    اگر ( خانہ->اندرونی_2 )
    {
      /* free glyph loader */
      اگر ( FT_DRIVER_استعمال_کرتا_OUTLINES( driver ) )
      {
        FT_Glyph_Loader_مکمل( خانہ->اندرونی_2->loader );
        خانہ->اندرونی_2->loader = تہی;
      }

      FT_FREE( خانہ->اندرونی_2 );
    }
  }


  /* documentation is in ftobjs.h */

  FT_بنیاد_DEF( FT_Error )
  FT_New_Glyph_خانہ( FT_Face        face,
                    FT_Glyph_خانہ  *aslot )
  {
    FT_Error         error;
    FT_Driver        driver;
    FT_Driver_Class  clazz;
    FT_حافظہ        حافظہ;
    FT_Glyph_خانہ     خانہ = تہی;


    اگر ( !face )
      واپس FT_THROW( Invalid_Face_Handle );

    اگر ( !face->driver )
      واپس FT_THROW( Invalid_دلیل );

    driver = face->driver;
    clazz  = driver->clazz;
    حافظہ = driver->جڑ.حافظہ;

    FT_TRACE4(( "FT_New_GlyphSlot: Creating new slot object\n" ));
    اگر ( !FT_ALLOC( خانہ, clazz->خانہ_object_سائز ) )
    {
      خانہ->face = face;

      error = ft_glyphslot_init( خانہ );
      اگر ( error )
      {
        ft_glyphslot_مکمل( خانہ );
        FT_FREE( خانہ );
        جاؤ Exit;
      }

      خانہ->اگلا  = face->glyph;
      face->glyph = خانہ;

      اگر ( aslot )
        *aslot = خانہ;
    }
    ورنہ اگر ( aslot )
      *aslot = تہی;


  Exit:
    FT_TRACE4(( "FT_New_GlyphSlot: Return 0x%x\n", error ));

    واپس error;
  }


  /* documentation is in ftobjs.h */

  FT_بنیاد_DEF( خالی )
  FT_مکمل_Glyph_خانہ( FT_Glyph_خانہ  خانہ )
  {
    اگر ( خانہ )
    {
      FT_Driver     driver = خانہ->face->driver;
      FT_حافظہ     حافظہ = driver->جڑ.حافظہ;
      FT_Glyph_خانہ  پچھلا;
      FT_Glyph_خانہ  cur;


      /* Remove slot from its parent face's list */
      پچھلا = تہی;
      cur  = خانہ->face->glyph;

      جب ( cur )
      {
        اگر ( cur == خانہ )
        {
          اگر ( !پچھلا )
            خانہ->face->glyph = cur->اگلا;
          ورنہ
            پچھلا->اگلا = cur->اگلا;

          /* finalize client-specific data */
          اگر ( خانہ->generic.finalizer )
            خانہ->generic.finalizer( خانہ );

          ft_glyphslot_مکمل( خانہ );
          FT_FREE( خانہ );
          توڑو;
        }
        پچھلا = cur;
        cur  = cur->اگلا;
      }
    }
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( خالی )
  FT_سیٹ_Transform( FT_Face     face,
                    FT_میٹرکس*  میٹرکس,
                    FT_سمتیہ*  delta )
  {
    FT_Face_اندرونی  اندرونی_2;


    اگر ( !face )
      واپس;

    اندرونی_2 = face->اندرونی_2;

    اندرونی_2->transform_flags = 0;

    اگر ( !میٹرکس || !FT_میٹرکس_Check( میٹرکس ) )
    {
      اندرونی_2->transform_میٹرکس.xx = 0x10000L;
      اندرونی_2->transform_میٹرکس.xy = 0;
      اندرونی_2->transform_میٹرکس.yx = 0;
      اندرونی_2->transform_میٹرکس.yy = 0x10000L;

      میٹرکس = &اندرونی_2->transform_میٹرکس;
    }
    ورنہ
      اندرونی_2->transform_میٹرکس = *میٹرکس;

    /* set transform_flags bit flag 0 if `matrix' isn't the identity */
    اگر ( ( میٹرکس->xy | میٹرکس->yx ) ||
         میٹرکس->xx != 0x10000L      ||
         میٹرکس->yy != 0x10000L      )
      اندرونی_2->transform_flags |= 1;

    اگر ( !delta )
    {
      اندرونی_2->transform_delta.x = 0;
      اندرونی_2->transform_delta.y = 0;

      delta = &اندرونی_2->transform_delta;
    }
    ورنہ
      اندرونی_2->transform_delta = *delta;

    /* set transform_flags bit flag 1 if `delta' isn't the null vector */
    اگر ( delta->x | delta->y )
      اندرونی_2->transform_flags |= 2;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( خالی )
  FT_Get_Transform( FT_Face     face,
                    FT_میٹرکس*  میٹرکس,
                    FT_سمتیہ*  delta )
  {
    FT_Face_اندرونی  اندرونی_2;


    اگر ( !face )
      واپس;

    اندرونی_2 = face->اندرونی_2;

    اگر ( میٹرکس )
      *میٹرکس = اندرونی_2->transform_میٹرکس;

    اگر ( delta )
      *delta = اندرونی_2->transform_delta;
  }


  ساکن FT_Renderer
  ft_lookup_glyph_renderer( FT_Glyph_خانہ  خانہ );


#اگر_تعریف_شدہ GRID_FIT_METRICS
  ساکن خالی
  ft_glyphslot_grid_fit_metrics( FT_Glyph_خانہ  خانہ,
                                 FT_Bool       vertical )
  {
    FT_Glyph_Metrics*  metrics = &خانہ->metrics;
    FT_Pos             دایاں, bottom;


    اگر ( vertical )
    {
      metrics->horiBearingX = FT_PIX_FLOOR( metrics->horiBearingX );
      metrics->horiBearingY = FT_PIX_CEIL_LONG( metrics->horiBearingY );

      دایاں  = FT_PIX_CEIL_LONG( جوڑو_LONG( metrics->vertBearingX,
                                           metrics->چوڑائی ) );
      bottom = FT_PIX_CEIL_LONG( جوڑو_LONG( metrics->vertBearingY,
                                           metrics->اونچائی ) );

      metrics->vertBearingX = FT_PIX_FLOOR( metrics->vertBearingX );
      metrics->vertBearingY = FT_PIX_FLOOR( metrics->vertBearingY );

      metrics->چوڑائی  = SUB_LONG( دایاں,
                                  metrics->vertBearingX );
      metrics->اونچائی = SUB_LONG( bottom,
                                  metrics->vertBearingY );
    }
    ورنہ
    {
      metrics->vertBearingX = FT_PIX_FLOOR( metrics->vertBearingX );
      metrics->vertBearingY = FT_PIX_FLOOR( metrics->vertBearingY );

      دایاں  = FT_PIX_CEIL_LONG( جوڑو_LONG( metrics->horiBearingX,
                                           metrics->چوڑائی ) );
      bottom = FT_PIX_FLOOR( SUB_LONG( metrics->horiBearingY,
                                       metrics->اونچائی ) );

      metrics->horiBearingX = FT_PIX_FLOOR( metrics->horiBearingX );
      metrics->horiBearingY = FT_PIX_CEIL_LONG( metrics->horiBearingY );

      metrics->چوڑائی  = SUB_LONG( دایاں,
                                  metrics->horiBearingX );
      metrics->اونچائی = SUB_LONG( metrics->horiBearingY,
                                  bottom );
    }

    metrics->horiAdvance = FT_PIX_باری_LONG( metrics->horiAdvance );
    metrics->vertAdvance = FT_PIX_باری_LONG( metrics->vertAdvance );
  }
#اگر_ختم /* GRID_FIT_METRICS */


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_بھرو_Glyph( FT_Face   face,
                 FT_UInt   glyph_اشاریہ,
                 FT_Int32  بھرو_flags )
  {
    FT_Error      error;
    FT_Driver     driver;
    FT_Glyph_خانہ  خانہ;
    FT_Library    library;
    FT_Bool       autohint = FALSE;
    FT_Module     hinter;


    اگر ( !face || !face->سائز || !face->glyph )
      واپس FT_THROW( Invalid_Face_Handle );

    /* The validity test for `glyph_index' is performed by the */
    /* font drivers.                                           */

    خانہ = face->glyph;
    ft_glyphslot_clear( خانہ );

    driver  = face->driver;
    library = driver->جڑ.library;
    hinter  = library->auto_hinter;

    /* undefined scale means no scale */
    اگر ( face->سائز->metrics.x_ppem == 0 ||
         face->سائز->metrics.y_ppem == 0 )
      بھرو_flags |= FT_بھرو_NO_SCALE;

    /* resolve load flags dependencies */

    اگر ( بھرو_flags & FT_بھرو_NO_RECURSE )
      بھرو_flags |= FT_بھرو_NO_SCALE         |
                    FT_بھرو_IGNORE_TRANSFORM;

    اگر ( بھرو_flags & FT_بھرو_NO_SCALE )
    {
      بھرو_flags |= FT_بھرو_NO_HINTING |
                    FT_بھرو_NO_BITMAP;

      بھرو_flags &= ~FT_بھرو_RENDER;
    }

    اگر ( بھرو_flags & FT_بھرو_BITMAP_METRICS_ONLY )
      بھرو_flags &= ~FT_بھرو_RENDER;

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

    اگر ( hinter                                           &&
         !( بھرو_flags & FT_بھرو_NO_HINTING )             &&
         !( بھرو_flags & FT_بھرو_NO_AUTOHINT )            &&
         FT_IS_SCALABLE( face )                           &&
         !FT_IS_TRICKY( face )                            &&
         ( ( بھرو_flags & FT_بھرو_IGNORE_TRANSFORM )    ||
           ( face->اندرونی_2->transform_میٹرکس.yx == 0 &&
             face->اندرونی_2->transform_میٹرکس.xx != 0 ) ||
           ( face->اندرونی_2->transform_میٹرکس.xx == 0 &&
             face->اندرونی_2->transform_میٹرکس.yx != 0 ) ) )
    {
      اگر ( ( بھرو_flags & FT_بھرو_FORCE_AUTOHINT ) ||
           !FT_DRIVER_HAS_HINTER( driver )         )
        autohint = TRUE;
      ورنہ
      {
        FT_Render_منوال  منوال = FT_بھرو_ہدف_منوال( بھرو_flags );
        FT_Bool         is_light_type1;
        TT_Face         ttface = (TT_Face)face;


        /* only the new Adobe engine (for both CFF and Type 1) is `light'; */
        /* we use `strstr' to catch both `Type 1' and `CID Type 1'         */
        is_light_type1 =
          ft_strstr( FT_Get_Font_Format( face ), "Type 1" ) != تہی &&
          ((PS_Driver)driver)->hinting_engine == FT_HINTING_ADOBE;

        /* the check for `num_locations' assures that we actually    */
        /* test for instructions in a TTF and not in a CFF-based OTF */
        /*                                                           */
        /* we check the size of the `fpgm' and `prep' tables, too -- */
        /* the assumption is that there don't exist real TTFs where  */
        /* both `fpgm' and `prep' tables are missing                 */
        اگر ( ( منوال == FT_RENDER_منوال_LIGHT           &&
               ( !FT_DRIVER_HINTS_LIGHTLY( driver ) &&
                 !is_light_type1                    ) )         ||
             ( FT_IS_SFNT( face )                             &&
               ttface->عدد_locations                          &&
               ttface->font_پروگرام_سائز == 0                 &&
               ttface->cvt_پروگرام_سائز <= 7                  ) )
          autohint = TRUE;
      }
    }

#اگر_تعریف_شدہ TT_CONFIG_OPTION_VARC
    /* Never auto-hint a VARC (variable composite) glyph: its components */
    /* are already loaded unhinted, and the auto-fitter cannot process   */
    /* the assembled composite (it also re-enters variation-coordinate   */
    /* setting).  Use the native hinter / unhinted load instead.         */
    اگر ( autohint && FT_IS_SFNT( face ) )
    {
      FT_Service_VARC  varc;


      FT_FACE_ڈھونڈو_SERVICE( face, varc, VARC );
      اگر ( varc && varc->has_glyph( face, glyph_اشاریہ ) )
        autohint = FALSE;
    }
#اگر_ختم

    اگر ( autohint )
    {
      FT_AutoHinter_Interface  hinting;


      /* XXX: The use of the `FT_LOAD_XXX_ONLY` flags is not very */
      /*      elegant.                                            */

      /* try to load SVG documents if available */
      اگر ( ( بھرو_flags & FT_بھرو_NO_SVG ) == 0 &&
           FT_HAS_SVG( face )                   )
      {
        error = driver->clazz->بھرو_glyph( خانہ, face->سائز,
                                           glyph_اشاریہ,
                                           بھرو_flags | FT_بھرو_SVG_ONLY );

        اگر ( !error && خانہ->format == FT_GLYPH_FORMAT_SVG )
          جاؤ بھرو_Ok;
      }

      /* try to load embedded bitmaps if available */
      اگر ( FT_HAS_FIXED_SIZES( face )              &&
           ( بھرو_flags & FT_بھرو_NO_BITMAP ) == 0 )
      {
        error = driver->clazz->بھرو_glyph( خانہ, face->سائز,
                                           glyph_اشاریہ,
                                           بھرو_flags | FT_بھرو_SBITS_ONLY );

        اگر ( !error && خانہ->format == FT_GLYPH_FORMAT_BITMAP )
          جاؤ بھرو_Ok;
      }

      {
        FT_Face_اندرونی  اندرونی_2        = face->اندرونی_2;
        FT_Int            transform_flags = اندرونی_2->transform_flags;


        /* since the auto-hinter calls FT_Load_Glyph by itself, */
        /* make sure that glyphs aren't transformed             */
        اندرونی_2->transform_flags = 0;

        /* load auto-hinted outline */
        hinting = (FT_AutoHinter_Interface)hinter->clazz->module_interface;

        error   = hinting->بھرو_glyph( (FT_AutoHinter)hinter,
                                       خانہ, face->سائز,
                                       glyph_اشاریہ, بھرو_flags );

        اندرونی_2->transform_flags = transform_flags;
      }
    }
    ورنہ
    {
      error = driver->clazz->بھرو_glyph( خانہ,
                                         face->سائز,
                                         glyph_اشاریہ,
                                         بھرو_flags );
      اگر ( error )
        جاؤ Exit;

      اگر ( خانہ->format == FT_GLYPH_FORMAT_OUTLINE )
      {
        /* check that the loaded outline is correct */
        error = FT_Outline_Check( &خانہ->outline );
        اگر ( error )
          جاؤ Exit;

#اگر_تعریف_شدہ GRID_FIT_METRICS
        اگر ( !( بھرو_flags & FT_بھرو_NO_HINTING ) )
          ft_glyphslot_grid_fit_metrics(
            خانہ,
            FT_BOOL( بھرو_flags & FT_بھرو_VERTICAL_LAYOUT ) );
#اگر_ختم
      }
    }

  بھرو_Ok:
    /* compute the advance */
    اگر ( بھرو_flags & FT_بھرو_VERTICAL_LAYOUT )
    {
      خانہ->advance.x = 0;
      خانہ->advance.y = خانہ->metrics.vertAdvance;
    }
    ورنہ
    {
      خانہ->advance.x = خانہ->metrics.horiAdvance;
      خانہ->advance.y = 0;
    }

    /* compute the linear advance in 16.16 pixels */
    اگر ( ( بھرو_flags & FT_بھرو_خطی_DESIGN ) == 0 &&
         FT_IS_SCALABLE( face )                      )
    {
      FT_سائز_Metrics*  metrics = &face->سائز->metrics;


      /* it's tricky! */
      خانہ->خطی_Hori_Advance = FT_MulDiv( خانہ->خطی_Hori_Advance,
                                           metrics->x_scale, 64 );

      خانہ->خطی_Vert_Advance = FT_MulDiv( خانہ->خطی_Vert_Advance,
                                           metrics->y_scale, 64 );
    }

    اگر ( ( بھرو_flags & FT_بھرو_IGNORE_TRANSFORM ) == 0 )
    {
      FT_Face_اندرونی  اندرونی_2 = face->اندرونی_2;


      /* now, transform the glyph image if needed */
      اگر ( اندرونی_2->transform_flags )
      {
        /* get renderer */
        FT_Renderer  renderer = ft_lookup_glyph_renderer( خانہ );


        اگر ( renderer )
          error = renderer->clazz->transform_glyph(
                                     renderer, خانہ,
                                     &اندرونی_2->transform_میٹرکس,
                                     &اندرونی_2->transform_delta );
        ورنہ اگر ( خانہ->format == FT_GLYPH_FORMAT_OUTLINE )
        {
          /* apply `standard' transformation if no renderer is available */
          اگر ( اندرونی_2->transform_flags & 1 )
            FT_Outline_Transform( &خانہ->outline,
                                  &اندرونی_2->transform_میٹرکس );

          اگر ( اندرونی_2->transform_flags & 2 )
            FT_Outline_Translate( &خانہ->outline,
                                  اندرونی_2->transform_delta.x,
                                  اندرونی_2->transform_delta.y );
        }

        /* transform advance */
        FT_سمتیہ_Transform( &خانہ->advance, &اندرونی_2->transform_میٹرکس );
      }
    }

    خانہ->glyph_اشاریہ          = glyph_اشاریہ;
    خانہ->اندرونی_2->بھرو_flags = بھرو_flags;

    /* do we need to render the image or preset the bitmap now? */
    اگر ( !error                                    &&
         ( بھرو_flags & FT_بھرو_NO_SCALE ) == 0    &&
         خانہ->format != FT_GLYPH_FORMAT_BITMAP    &&
         خانہ->format != FT_GLYPH_FORMAT_مرکب )
    {
      FT_Render_منوال  منوال = FT_بھرو_ہدف_منوال( بھرو_flags );


      اگر ( منوال == FT_RENDER_منوال_معمول   &&
           بھرو_flags & FT_بھرو_MONOCHROME )
        منوال = FT_RENDER_منوال_MONO;

      اگر ( بھرو_flags & FT_بھرو_RENDER )
        error = FT_Render_Glyph( خانہ, منوال );
      ورنہ
        ft_glyphslot_preset_bitmap( خانہ, منوال, تہی );
    }

#اگر_تعریف_شدہ FT_DEBUG_LEVEL_TRACE
    FT_TRACE5(( "FT_Load_Glyph: index %u, flags 0x%x\n",
                glyph_اشاریہ, بھرو_flags ));
    FT_TRACE5(( "  bitmap %ux%u %s, %s (mode %d)\n",
                خانہ->bitmap.چوڑائی,
                خانہ->bitmap.صفیں,
                خانہ->outline.points ?
                  خانہ->bitmap.بفر ? "rendered"
                                      : "preset"
                                     :
                  خانہ->اندرونی_2->flags & FT_GLYPH_OWN_BITMAP ? "owned"
                                                              : "unowned",
                pixel_modes[خانہ->bitmap.pixel_منوال],
                خانہ->bitmap.pixel_منوال ));
    FT_TRACE5(( "\n" ));
    FT_TRACE5(( "  x advance: %f\n", (دوہرا)خانہ->advance.x / 64 ));
    FT_TRACE5(( "  y advance: %f\n", (دوہرا)خانہ->advance.y / 64 ));
    FT_TRACE5(( "  linear x advance: %f\n",
                (دوہرا)خانہ->خطی_Hori_Advance / 65536 ));
    FT_TRACE5(( "  linear y advance: %f\n",
                (دوہرا)خانہ->خطی_Vert_Advance / 65536 ));

    {
      FT_Glyph_Metrics*  metrics = &خانہ->metrics;


      FT_TRACE5(( "  metrics:\n" ));
      FT_TRACE5(( "    width:  %f\n", (دوہرا)metrics->چوڑائی / 64 ));
      FT_TRACE5(( "    height: %f\n", (دوہرا)metrics->اونچائی / 64 ));
      FT_TRACE5(( "\n" ));
      FT_TRACE5(( "    horiBearingX: %f\n",
                  (دوہرا)metrics->horiBearingX / 64 ));
      FT_TRACE5(( "    horiBearingY: %f\n",
                  (دوہرا)metrics->horiBearingY / 64 ));
      FT_TRACE5(( "    horiAdvance:  %f\n",
                  (دوہرا)metrics->horiAdvance / 64 ));
      FT_TRACE5(( "\n" ));
      FT_TRACE5(( "    vertBearingX: %f\n",
                  (دوہرا)metrics->vertBearingX / 64 ));
      FT_TRACE5(( "    vertBearingY: %f\n",
                  (دوہرا)metrics->vertBearingY / 64 ));
      FT_TRACE5(( "    vertAdvance:  %f\n",
                  (دوہرا)metrics->vertAdvance / 64 ));
    }
#اگر_ختم

  Exit:
    واپس error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_بھرو_Char( FT_Face   face,
                FT_ULong  char_رمز,
                FT_Int32  بھرو_flags )
  {
    FT_UInt  glyph_اشاریہ;


    اگر ( !face )
      واپس FT_THROW( Invalid_Face_Handle );

    glyph_اشاریہ = (FT_UInt)char_رمز;
    اگر ( face->charmap )
      glyph_اشاریہ = FT_Get_Char_اشاریہ( face, char_رمز );

    واپس FT_بھرو_Glyph( face, glyph_اشاریہ, بھرو_flags );
  }


  /* destructor for sizes list */
  ساکن خالی
  destroy_سائز( FT_حافظہ  حافظہ,
                خالی*      سائز_2,
                خالی*      driver_ )
  {
    FT_سائز    سائز   = (FT_سائز)سائز_2;
    FT_Driver  driver = (FT_Driver)driver_;


    /* finalize format-specific stuff */
    اگر ( driver->clazz->مکمل_سائز )
      driver->clazz->مکمل_سائز( سائز );

    /* finalize client-specific data */
    اگر ( سائز->generic.finalizer )
      سائز->generic.finalizer( سائز );

    FT_FREE( سائز->اندرونی_2 );
    FT_FREE( سائز );
  }


  ساکن خالی
  ft_cmap_مکمل_اندرونی( FT_CMap  cmap );


  ساکن خالی
  destroy_charmaps( FT_Face    face,
                    FT_حافظہ  حافظہ )
  {
    FT_Int  n;


    اگر ( !face )
      واپس;

    برائے ( n = 0; n < face->عدد_charmaps; n++ )
    {
      FT_CMap  cmap = FT_CMAP( face->charmaps[n] );


      ft_cmap_مکمل_اندرونی( cmap );

      face->charmaps[n] = تہی;
    }

    FT_FREE( face->charmaps );
    face->عدد_charmaps = 0;
  }


  /* destructor for faces list */
  ساکن خالی
  destroy_face( FT_حافظہ  حافظہ,
                خالی*      face_,
                خالی*      driver_ )
  {
    FT_Face          face   = (FT_Face)face_;
    FT_Driver        driver = (FT_Driver)driver_;
    FT_Driver_Class  clazz  = driver->clazz;


    /* discard auto-hinting data */
    اگر ( face->autohint.finalizer )
      face->autohint.finalizer( face->autohint.ڈیٹا );

    /* Discard glyph slots for this face.                           */
    /* Beware!  FT_Done_GlyphSlot() changes the field `face->glyph' */
    جب ( face->glyph )
      FT_مکمل_Glyph_خانہ( face->glyph );

    /* discard all sizes for this face */
    FT_List_Finalize( &face->sizes_list,
                      destroy_سائز,
                      حافظہ,
                      driver );
    face->سائز = تہی;

    /* discard charmaps */
    destroy_charmaps( face, حافظہ );

    /* finalize format-specific stuff */
    اگر ( clazz->مکمل_face )
      clazz->مکمل_face( face );

    /* close the stream for this face if needed */
    FT_دھارا_Free(
      face->دھارا,
      ( face->face_flags & FT_FACE_FLAG_EXTERNAL_دھارا ) != 0 );

    face->دھارا = تہی;

    /* now discard client data */
    اگر ( face->generic.finalizer )
      face->generic.finalizer( face );

    /* get rid of it */
    اگر ( face->اندرونی_2 )
    {
      FT_FREE( face->اندرونی_2 );
    }
    FT_FREE( face );
  }


  ساکن خالی
  Destroy_Driver( FT_Driver  driver )
  {
    FT_List_Finalize( &driver->faces_list,
                      destroy_face,
                      driver->جڑ.حافظہ,
                      driver );
  }


  /* documentation is in ftobjs.h */

  FT_بنیاد_DEF( FT_Error )
  ڈھونڈو_unicode_charmap( FT_Face  face )
  {
    FT_CharMap*  پہلا;
    FT_CharMap*  cur;


    /* caller should have already checked that `face' is valid */
    FT_ASSERT( face );

    پہلا = face->charmaps;

    اگر ( !پہلا )
      واپس FT_THROW( Invalid_CharMap_Handle );

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

    برائے ( ; --cur >= پہلا; )
    {
      اگر ( cur[0]->encoding == FT_ENCODING_UNICODE )
      {
        /* XXX If some new encodings to represent UCS-4 are added, */
        /*     they should be added here.                          */
        اگر ( ( cur[0]->platform_id == TT_PLATFORM_MICROSOFT &&
               cur[0]->encoding_id == TT_MS_ID_UCS_4        )     ||
             ( cur[0]->platform_id == TT_PLATFORM_APPLE_UNICODE &&
               cur[0]->encoding_id == TT_APPLE_ID_UNICODE_32    ) ||
             ( cur[0]->platform_id == TT_PLATFORM_APPLE_UNICODE &&
               cur[0]->encoding_id == TT_APPLE_ID_بھرا_UNICODE  &&
               FT_Get_CMap_Format( cur[0] ) == 13               ) )
        {
          face->charmap = cur[0];
          واپس FT_Err_Ok;
        }
      }
    }

    /* We do not have any UCS-4 charmap.                */
    /* Do the loop again and search for UCS-2 charmaps. */
    cur = پہلا + face->عدد_charmaps;

    برائے ( ; --cur >= پہلا; )
    {
      اگر ( cur[0]->encoding == FT_ENCODING_UNICODE )
      {
        FT_Long  format = FT_Get_CMap_Format( cur[0] );


        اگر ( format != 14 && format != 15 )
        {
          face->charmap = cur[0];
          واپس FT_Err_Ok;
        }
      }
    }

    واپس FT_THROW( Invalid_CharMap_Handle );
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
  ساکن FT_CharMap
  ڈھونڈو_variant_selector_charmap( FT_Face  face )
  {
    FT_CharMap*  پہلا;
    FT_CharMap*  انتہا;
    FT_CharMap*  cur;


    /* caller should have already checked that `face' is valid */
    FT_ASSERT( face );

    پہلا = face->charmaps;

    اگر ( !پہلا )
      واپس تہی;

    انتہا = پہلا + face->عدد_charmaps;  /* points after the last one */

    برائے ( cur = پہلا; cur < انتہا; cur++ )
    {
      اگر ( cur[0]->platform_id == TT_PLATFORM_APPLE_UNICODE    &&
           cur[0]->encoding_id == TT_APPLE_ID_VARIANT_SELECTOR )
      {
        FT_Long  format = FT_Get_CMap_Format( cur[0] );


        اگر ( format == 14 || format == 15 )
          واپس cur[0];
      }
    }

    واپس تہی;
  }


  /**************************************************************************
   *
   * @Function:
   *   open_face
   *
   * @Description:
   *   This function does some work for FT_Open_Face().
   */
  ساکن FT_Error
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
    FT_Face           face     = تہی;
    FT_Face_اندرونی  اندرونی_2 = تہی;

    FT_Error          error, error2;


    clazz  = driver->clazz;
    حافظہ = driver->جڑ.حافظہ;

    /* allocate the face object and perform basic initialization */
    اگر ( FT_ALLOC( face, clazz->face_object_سائز ) )
      جاؤ Fail;

    face->driver = driver;
    face->حافظہ = حافظہ;
    face->دھارا = *astream;

    /* set the FT_FACE_FLAG_EXTERNAL_STREAM bit for FT_Done_Face */
    اگر ( *anexternal_دھارا )
      face->face_flags |= FT_FACE_FLAG_EXTERNAL_دھارا;

    اگر ( FT_NEW( اندرونی_2 ) )
      جاؤ Fail;

    face->اندرونی_2 = اندرونی_2;

#اگر_تعریف_شدہ FT_CONFIG_OPTION_INCREMENTAL
    {
      صحیح  i;


      face->اندرونی_2->incremental_interface = تہی;
      برائے ( i = 0; i < عدد_params && !face->اندرونی_2->incremental_interface;
            i++ )
        اگر ( params[i].لیبل == FT_PARAM_لیبل_INCREMENTAL )
          face->اندرونی_2->incremental_interface =
            (FT_Incremental_Interface)params[i].ڈیٹا;
    }
#اگر_ختم

    face->اندرونی_2->random_بیج = -1;

    اگر ( clazz->init_face )
      error = clazz->init_face( *astream,
                                face,
                                (FT_Int)face_اشاریہ,
                                عدد_params,
                                params );
    /* Stream may have been changed. */
    *astream = face->دھارا;
    *anexternal_دھارا =
      ( face->face_flags & FT_FACE_FLAG_EXTERNAL_دھارا ) != 0;
    اگر ( error )
      جاؤ Fail;

    /* select Unicode charmap by default */
    error2 = ڈھونڈو_unicode_charmap( face );

    /* if no Unicode charmap can be found, FT_Err_Invalid_CharMap_Handle */
    /* is returned.                                                      */

    /* no error should happen, but we want to play safe */
    اگر ( error2 && FT_ERR_NEQ( error2, Invalid_CharMap_Handle ) )
    {
      error = error2;
      جاؤ Fail;
    }

    *aface = face;

  Fail:
    اگر ( error )
    {
      destroy_charmaps( face, حافظہ );
      اگر ( clazz->مکمل_face )
        clazz->مکمل_face( face );
      FT_FREE( اندرونی_2 );
      FT_FREE( face );
      *aface = تہی;
    }

    واپس error;
  }


  /* there's a Mac-specific extended implementation of FT_New_Face() */
  /* in src/base/ftmac.c                                             */

#اگر_غیر_تعریف_شدہ FT_MACINTOSH

  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_New_Face( FT_Library   library,
               ثابت حرف*  pathname,
               FT_Long      face_اشاریہ,
               FT_Face     *aface )
  {
    FT_Open_دلائل  دلائل;


    /* test for valid `library' and `aface' delayed to `FT_Open_Face' */
    اگر ( !pathname )
      واپس FT_THROW( Invalid_دلیل );

    دلائل.flags    = FT_OPEN_PATHNAME;
    دلائل.pathname = (حرف*)pathname;
    دلائل.دھارا   = تہی;

    واپس ft_open_face_اندرونی( library, &دلائل, face_اشاریہ, aface, 1 );
  }

#اگر_ختم


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_New_حافظہ_Face( FT_Library      library,
                      ثابت FT_Byte*  file_بنیاد,
                      FT_Long         file_سائز,
                      FT_Long         face_اشاریہ,
                      FT_Face        *aface )
  {
    FT_Open_دلائل  دلائل;


    /* test for valid `library' and `face' delayed to `FT_Open_Face' */
    اگر ( !file_بنیاد )
      واپس FT_THROW( Invalid_دلیل );

    دلائل.flags       = FT_OPEN_حافظہ;
    دلائل.حافظہ_بنیاد = file_بنیاد;
    دلائل.حافظہ_سائز = file_سائز;
    دلائل.دھارا      = تہی;

    واپس ft_open_face_اندرونی( library, &دلائل, face_اشاریہ, aface, 1 );
  }


#اگر_تعریف_شدہ FT_CONFIG_OPTION_MAC_FONTS

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
  ساکن خالی
  حافظہ_دھارا_close( FT_دھارا  دھارا )
  {
    FT_حافظہ  حافظہ = (FT_حافظہ)دھارا->descriptor.اشارہ_گر;


    FT_FREE( دھارا->بنیاد );
    دھارا->سائز  = 0;
    دھارا->close = تہی;
    FT_FREE( دھارا );
  }


  /* Create a new memory stream from a buffer and a size. */
  /* From `ftmac.c'.                                      */
  ساکن FT_Error
  new_حافظہ_دھارا( FT_Library           library,
                     FT_Byte*             بنیاد,
                     FT_ULong             سائز,
                     FT_دھارا_Close_Func  close,
                     FT_دھارا           *astream )
  {
    FT_Error   error;
    FT_حافظہ  حافظہ;
    FT_دھارا  دھارا = تہی;


    اگر ( !library )
      واپس FT_THROW( Invalid_Library_Handle );

    اگر ( !بنیاد )
      واپس FT_THROW( Invalid_دلیل );

    *astream = تہی;
    حافظہ   = library->حافظہ;
    اگر ( FT_NEW( دھارا ) )
      جاؤ Exit;

    FT_دھارا_Open_حافظہ( دھارا, بنیاد, سائز );

    دھارا->descriptor.اشارہ_گر = حافظہ;
    دھارا->close              = close;

    *astream = دھارا;

  Exit:
    واپس error;
  }


  /* Create a new FT_Face given a buffer and a driver name. */
  /* From `ftmac.c'.                                        */
  FT_LOCAL_DEF( FT_Error )
  open_face_from_بفر( FT_Library   library,
                         FT_Byte*     بنیاد,
                         FT_ULong     سائز,
                         FT_Long      face_اشاریہ,
                         ثابت حرف*  driver_name,
                         FT_Face     *aface )
  {
    FT_Open_دلائل  دلائل;
    FT_Error      error;
    FT_حافظہ     حافظہ = library->حافظہ;


    دلائل.driver = تہی;
    دلائل.flags  = 0;

    اگر ( driver_name )
    {
      دلائل.driver = FT_Get_Module( library, driver_name );
      اگر ( !دلائل.driver )
      {
        FT_FREE( بنیاد );
        واپس FT_THROW( Missing_Module );
      }

      دلائل.flags = دلائل.flags | FT_OPEN_DRIVER;
    }

    /* `memory_stream_close` also frees the stream object. */
    error = new_حافظہ_دھارا( library,
                               بنیاد,
                               سائز,
                               حافظہ_دھارا_close,
                               &دلائل.دھارا );
    اگر ( error )
    {
      FT_FREE( بنیاد );
      واپس error;
    }

    دلائل.flags |= FT_OPEN_دھارا;

#اگر_تعریف_شدہ FT_MACINTOSH
    /* At this point, the face index has served its purpose;  */
    /* whoever calls this function has already used it to     */
    /* locate the correct font data.  We should not propagate */
    /* this index to FT_Open_Face() (unless it is negative).  */

    اگر ( face_اشاریہ > 0 )
      face_اشاریہ &= 0x7FFF0000L; /* retain GX data */
#اگر_ختم

    واپس ft_open_face_اندرونی( library, &دلائل, face_اشاریہ, aface, 0 );
  }


  /* Look up `TYP1' or `CID ' table from sfnt table directory.       */
  /* `offset' and `length' must exclude the binary header in tables. */

  /* Type 1 and CID-keyed font drivers should recognize sfnt-wrapped */
  /* format too.  Here, since we can't expect that the TrueType font */
  /* driver is loaded unconditionally, we must parse the font by     */
  /* ourselves.  We are only interested in the name of the table and */
  /* the offset.                                                     */

  ساکن FT_Error
  ft_lookup_PS_اندر_sfnt_دھارا( FT_دھارا  دھارا,
                               FT_Long    face_اشاریہ,
                               FT_ULong*  انحراف,
                               FT_ULong*  لمبائی,
                               FT_Bool*   is_sfnt_cid )
  {
    FT_Error   error;
    FT_UShort  عدد_Tables;
    FT_Long    pstable_اشاریہ;
    FT_ULong   لیبل;
    صحیح        i;


    *انحراف = 0;
    *لمبائی = 0;
    *is_sfnt_cid = FALSE;

    /* TODO: support for sfnt-wrapped PS/CID in TTC format */

    /* version check for 'typ1' (should be ignored?) */
    اگر ( FT_READ_ULONG( لیبل ) )
      واپس error;
    اگر ( لیبل != TTAG_typ1 )
      واپس FT_THROW( Unknown_File_Format );

    اگر ( FT_READ_USHORT( عدد_Tables ) )
      واپس error;
    اگر ( FT_دھارا_SKIP( 2 * 3 ) ) /* skip binary search header */
      واپس error;

    pstable_اشاریہ = -1;
    *is_sfnt_cid  = FALSE;

    برائے ( i = 0; i < عدد_Tables; i++ )
    {
      اگر ( FT_READ_ULONG( لیبل )     || FT_دھارا_SKIP( 4 )      ||
           FT_READ_ULONG( *انحراف ) || FT_READ_ULONG( *لمبائی ) )
        واپس error;

      اگر ( لیبل == TTAG_CID )
      {
        pstable_اشاریہ++;
        *انحراف += 22;
        *لمبائی -= 22;
        *is_sfnt_cid = TRUE;
        اگر ( face_اشاریہ < 0 )
          واپس FT_Err_Ok;
      }
      ورنہ اگر ( لیبل == TTAG_TYP1 )
      {
        pstable_اشاریہ++;
        *انحراف += 24;
        *لمبائی -= 24;
        *is_sfnt_cid = FALSE;
        اگر ( face_اشاریہ < 0 )
          واپس FT_Err_Ok;
      }
      اگر ( face_اشاریہ >= 0 && pstable_اشاریہ == face_اشاریہ )
        واپس FT_Err_Ok;
    }

    واپس FT_THROW( جدول_Missing );
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
    FT_ULong   انحراف, لمبائی;
    FT_ULong   pos;
    FT_Bool    is_sfnt_cid;
    FT_Byte*   sfnt_ps = تہی;

    FT_غیر_مستعمل( عدد_params );
    FT_غیر_مستعمل( params );


    /* ignore GX stuff */
    اگر ( face_اشاریہ > 0 )
      face_اشاریہ &= 0xFFFFL;

    pos = FT_دھارا_POS();

    error = ft_lookup_PS_اندر_sfnt_دھارا( دھارا,
                                         face_اشاریہ,
                                         &انحراف,
                                         &لمبائی,
                                         &is_sfnt_cid );
    اگر ( error )
      جاؤ Exit;

    اگر ( انحراف > دھارا->سائز )
    {
      FT_TRACE2(( "open_face_PS_from_sfnt_stream: invalid table offset\n" ));
      error = FT_THROW( Invalid_جدول );
      جاؤ Exit;
    }
    ورنہ اگر ( لمبائی > دھارا->سائز - انحراف )
    {
      FT_TRACE2(( "open_face_PS_from_sfnt_stream: invalid table length\n" ));
      error = FT_THROW( Invalid_جدول );
      جاؤ Exit;
    }

    error = FT_دھارا_Seek( دھارا, pos + انحراف );
    اگر ( error )
      جاؤ Exit;

    اگر ( FT_QALLOC( sfnt_ps, (FT_Long)لمبائی ) )
      جاؤ Exit;

    error = FT_دھارا_Read( دھارا, (FT_Byte *)sfnt_ps, لمبائی );
    اگر ( error )
    {
      FT_FREE( sfnt_ps );
      جاؤ Exit;
    }

    error = open_face_from_بفر( library,
                                   sfnt_ps,
                                   لمبائی,
                                   FT_MIN( face_اشاریہ, 0 ),
                                   is_sfnt_cid ? "t1cid" : "type1",
                                   aface );
  Exit:
    {
      FT_Error  error1;


      اگر ( FT_ERR_EQ( error, Unknown_File_Format ) )
      {
        error1 = FT_دھارا_Seek( دھارا, pos );
        اگر ( error1 )
          واپس error1;
      }

      واپس error;
    }
  }


#اگر_غیر_تعریف_شدہ FT_MACINTOSH

  /* The resource header says we've got resource_cnt `POST' (type1) */
  /* resources in this file.  They all need to be coalesced into    */
  /* one lump which gets passed on to the type1 driver.             */
  /* Here can be only one PostScript font in a file so face_index   */
  /* must be 0 (or -1).                                             */
  /*                                                                */
  ساکن FT_Error
  Mac_Read_POST_Resource( FT_Library  library,
                          FT_دھارا   دھارا,
                          FT_Long    *offsets,
                          FT_Long     resource_cnt,
                          FT_Long     face_اشاریہ,
                          FT_Face    *aface )
  {
    FT_Error   error  = FT_ERR( Cannot_Open_Resource );
    FT_حافظہ  حافظہ = library->حافظہ;

    FT_Byte*   pfb_ڈیٹا = تہی;
    صحیح        i, type, flags;
    FT_ULong   len;
    FT_ULong   pfb_len, pfb_pos, pfb_lenpos;
    FT_ULong   rlen, عارضی;


    اگر ( face_اشاریہ == -1 )
      face_اشاریہ = 0;
    اگر ( face_اشاریہ != 0 )
      واپس error;

    /* Find the length of all the POST resources, concatenated.  Assume */
    /* worst case (each resource in its own section).                   */
    pfb_len = 0;
    برائے ( i = 0; i < resource_cnt; i++ )
    {
      error = FT_دھارا_Seek( دھارا, (FT_ULong)offsets[i] );
      اگر ( error )
        جاؤ Exit;
      اگر ( FT_READ_ULONG( عارضی ) )  /* actually LONG */
        جاؤ Exit;

      /* FT2 allocator takes signed long buffer length,
       * too large value causing overflow should be checked
       */
      FT_TRACE4(( "                 POST fragment #%d: length=0x%08lx"
                  " total pfb_len=0x%08lx\n",
                  i, عارضی, pfb_len + عارضی + 6 ));

      اگر ( FT_MAC_RFORK_MAX_LEN < عارضی               ||
           FT_MAC_RFORK_MAX_LEN - عارضی < pfb_len + 6 )
      {
        FT_TRACE2(( "             MacOS resource length cannot exceed"
                    " 0x%08lx\n",
                    FT_MAC_RFORK_MAX_LEN ));

        error = FT_THROW( Invalid_انحراف );
        جاؤ Exit;
      }

      pfb_len += عارضی + 6;
    }

    FT_TRACE2(( "             total buffer size to concatenate"
                " %ld POST fragments: 0x%08lx\n",
                 resource_cnt, pfb_len + 2 ));

    اگر ( pfb_len + 2 < 6 )
    {
      FT_TRACE2(( "             too long fragment length makes"
                  " pfb_len confused: pfb_len=0x%08lx\n",
                  pfb_len ));

      error = FT_THROW( Array_Too_Large );
      جاؤ Exit;
    }

    اگر ( FT_QALLOC( pfb_ڈیٹا, (FT_Long)pfb_len + 2 ) )
      جاؤ Exit;

    pfb_ڈیٹا[0] = 0x80;
    pfb_ڈیٹا[1] = 1;            /* Ascii section */
    pfb_ڈیٹا[2] = 0;            /* 4-byte length, fill in later */
    pfb_ڈیٹا[3] = 0;
    pfb_ڈیٹا[4] = 0;
    pfb_ڈیٹا[5] = 0;
    pfb_pos     = 6;
    pfb_lenpos  = 2;

    len  = 0;
    type = 1;

    برائے ( i = 0; i < resource_cnt; i++ )
    {
      error = FT_دھارا_Seek( دھارا, (FT_ULong)offsets[i] );
      اگر ( error )
        جاؤ Exit2;
      اگر ( FT_READ_ULONG( rlen ) )
        جاؤ Exit2;

      /* FT2 allocator takes signed long buffer length,
       * too large fragment length causing overflow should be checked
       */
      اگر ( 0x7FFFFFFFUL < rlen )
      {
        error = FT_THROW( Invalid_انحراف );
        جاؤ Exit2;
      }

      اگر ( FT_READ_USHORT( flags ) )
        جاؤ Exit2;

      FT_TRACE3(( "POST fragment[%d]:"
                  " offsets=0x%08lx, rlen=0x%08lx, flags=0x%04x\n",
                  i, offsets[i], rlen, flags ));

      error = FT_ERR( Array_Too_Large );

      /* postpone the check of `rlen longer than buffer' */
      /* until `FT_Stream_Read'                          */

      اگر ( ( flags >> 8 ) == 0 )        /* Comment, should not be loaded */
      {
        FT_TRACE3(( "    Skip POST fragment #%d because it is a comment\n",
                    i ));
        جاری;
      }

      /* the flags are part of the resource, so rlen >= 2,  */
      /* but some fonts declare rlen = 0 for empty fragment */
      اگر ( rlen > 2 )
        rlen -= 2;
      ورنہ
        rlen = 0;

      اگر ( ( flags >> 8 ) == type )
        len += rlen;
      ورنہ
      {
        FT_TRACE3(( "    Write POST fragment #%d header (4-byte) to buffer"
                    " %p + 0x%08lx\n",
                    i, (خالی*)pfb_ڈیٹا, pfb_lenpos ));

        اگر ( pfb_lenpos + 3 > pfb_len + 2 )
          جاؤ Exit2;

        pfb_ڈیٹا[pfb_lenpos    ] = (FT_Byte)( len );
        pfb_ڈیٹا[pfb_lenpos + 1] = (FT_Byte)( len >> 8 );
        pfb_ڈیٹا[pfb_lenpos + 2] = (FT_Byte)( len >> 16 );
        pfb_ڈیٹا[pfb_lenpos + 3] = (FT_Byte)( len >> 24 );

        اگر ( ( flags >> 8 ) == 5 )      /* End of font mark */
          توڑو;

        FT_TRACE3(( "    Write POST fragment #%d header (6-byte) to buffer"
                    " %p + 0x%08lx\n",
                    i, (خالی*)pfb_ڈیٹا, pfb_pos ));

        اگر ( pfb_pos + 6 > pfb_len + 2 )
          جاؤ Exit2;

        pfb_ڈیٹا[pfb_pos++] = 0x80;

        type = flags >> 8;
        len  = rlen;

        pfb_ڈیٹا[pfb_pos++] = (FT_Byte)type;
        pfb_lenpos          = pfb_pos;
        pfb_ڈیٹا[pfb_pos++] = 0;        /* 4-byte length, fill in later */
        pfb_ڈیٹا[pfb_pos++] = 0;
        pfb_ڈیٹا[pfb_pos++] = 0;
        pfb_ڈیٹا[pfb_pos++] = 0;
      }

      اگر ( pfb_pos > pfb_len || pfb_pos + rlen > pfb_len )
        جاؤ Exit2;

      FT_TRACE3(( "    Load POST fragment #%d (%lu byte) to buffer"
                  " %p + 0x%08lx\n",
                  i, rlen, (خالی*)pfb_ڈیٹا, pfb_pos ));

      error = FT_دھارا_Read( دھارا, (FT_Byte *)pfb_ڈیٹا + pfb_pos, rlen );
      اگر ( error )
        جاؤ Exit2;

      pfb_pos += rlen;
    }

    error = FT_ERR( Array_Too_Large );

    اگر ( pfb_pos + 2 > pfb_len + 2 )
      جاؤ Exit2;
    pfb_ڈیٹا[pfb_pos++] = 0x80;
    pfb_ڈیٹا[pfb_pos++] = 3;

    اگر ( pfb_lenpos + 3 > pfb_len + 2 )
      جاؤ Exit2;
    pfb_ڈیٹا[pfb_lenpos    ] = (FT_Byte)( len );
    pfb_ڈیٹا[pfb_lenpos + 1] = (FT_Byte)( len >> 8 );
    pfb_ڈیٹا[pfb_lenpos + 2] = (FT_Byte)( len >> 16 );
    pfb_ڈیٹا[pfb_lenpos + 3] = (FT_Byte)( len >> 24 );

    واپس open_face_from_بفر( library,
                                  pfb_ڈیٹا,
                                  pfb_pos,
                                  face_اشاریہ,
                                  "type1",
                                  aface );

  Exit2:
    اگر ( FT_ERR_EQ( error, Array_Too_Large ) )
      FT_TRACE2(( "  Abort due to too-short buffer to store"
                  " all POST fragments\n" ));
    ورنہ اگر ( FT_ERR_EQ( error, Invalid_انحراف ) )
      FT_TRACE2(( "  Abort due to invalid offset in a POST fragment\n" ));

    اگر ( error )
      error = FT_ERR( Cannot_Open_Resource );
    FT_FREE( pfb_ڈیٹا );

  Exit:
    واپس error;
  }


  /* The resource header says we've got resource_cnt `sfnt'      */
  /* (TrueType/OpenType) resources in this file.  Look through   */
  /* them for the one indicated by face_index, load it into mem, */
  /* pass it on to the truetype driver, and return it.           */
  /*                                                             */
  ساکن FT_Error
  Mac_Read_sfnt_Resource( FT_Library  library,
                          FT_دھارا   دھارا,
                          FT_Long    *offsets,
                          FT_Long     resource_cnt,
                          FT_Long     face_اشاریہ,
                          FT_Face    *aface )
  {
    FT_حافظہ  حافظہ = library->حافظہ;
    FT_Byte*   sfnt_ڈیٹا = تہی;
    FT_Error   error;
    FT_ULong   flag_انحراف;
    FT_ULong   rlen;
    صحیح        is_cff;
    FT_Long    face_اشاریہ_اندر_resource = 0;


    اگر ( face_اشاریہ < 0 )
      face_اشاریہ = -face_اشاریہ - 1;
    اگر ( face_اشاریہ >= resource_cnt )
      واپس FT_THROW( Cannot_Open_Resource );

    flag_انحراف = (FT_ULong)offsets[face_اشاریہ];
    error = FT_دھارا_Seek( دھارا, flag_انحراف );
    اگر ( error )
      جاؤ Exit;

    اگر ( FT_READ_ULONG( rlen ) )
      جاؤ Exit;
    اگر ( !rlen )
      واپس FT_THROW( Cannot_Open_Resource );
    اگر ( rlen > FT_MAC_RFORK_MAX_LEN )
      واپس FT_THROW( Invalid_انحراف );

    error = open_face_PS_from_sfnt_دھارا( library,
                                           دھارا,
                                           face_اشاریہ,
                                           0, تہی,
                                           aface );
    اگر ( !error )
      جاؤ Exit;

    /* rewind sfnt stream before open_face_PS_from_sfnt_stream() */
    error = FT_دھارا_Seek( دھارا, flag_انحراف + 4 );
    اگر ( error )
      جاؤ Exit;

    اگر ( FT_QALLOC( sfnt_ڈیٹا, rlen ) )
      واپس error;
    error = FT_دھارا_Read( دھارا, (FT_Byte *)sfnt_ڈیٹا, rlen );
    اگر ( error )
    {
      FT_FREE( sfnt_ڈیٹا );
      جاؤ Exit;
    }

    is_cff = rlen > 4 && !ft_memcmp( sfnt_ڈیٹا, "OTTO", 4 );
    error = open_face_from_بفر( library,
                                   sfnt_ڈیٹا,
                                   rlen,
                                   face_اشاریہ_اندر_resource,
                                   is_cff ? "cff" : "truetype",
                                   aface );

  Exit:
    واپس error;
  }


  /* Check for a valid resource fork header, or a valid dfont    */
  /* header.  In a resource fork the first 16 bytes are repeated */
  /* at the location specified by bytes 4-7.  In a dfont bytes   */
  /* 4-7 point to 16 bytes of zeroes instead.                    */
  /*                                                             */
  ساکن FT_Error
  IsMacResource( FT_Library  library,
                 FT_دھارا   دھارا,
                 FT_Long     resource_انحراف,
                 FT_Long     face_اشاریہ,
                 FT_Face    *aface )
  {
    FT_حافظہ  حافظہ = library->حافظہ;
    FT_Error   error;
    FT_Long    map_انحراف, rdata_pos;
    FT_Long    *ڈیٹا_offsets;
    FT_Long    گنتی;


    error = FT_Raccess_Get_سرخی_Info( library, دھارا, resource_انحراف,
                                       &map_انحراف, &rdata_pos );
    اگر ( error )
      واپس error;

    /* POST resources must be sorted to concatenate properly */
    error = FT_Raccess_Get_ڈیٹا_Offsets( library, دھارا,
                                        map_انحراف, rdata_pos,
                                        TTAG_POST, TRUE,
                                        &ڈیٹا_offsets, &گنتی );
    اگر ( !error )
    {
      error = Mac_Read_POST_Resource( library, دھارا, ڈیٹا_offsets, گنتی,
                                      face_اشاریہ, aface );
      FT_FREE( ڈیٹا_offsets );
      /* POST exists in an LWFN providing a single face */
      اگر ( !error )
        (*aface)->عدد_faces = 1;
      واپس error;
    }

    /* sfnt resources should not be sorted to preserve the face order by
       QuickDraw API */
    error = FT_Raccess_Get_ڈیٹا_Offsets( library, دھارا,
                                        map_انحراف, rdata_pos,
                                        TTAG_sfnt, FALSE,
                                        &ڈیٹا_offsets, &گنتی );
    اگر ( !error )
    {
      FT_Long  face_اشاریہ_اندرونی = face_اشاریہ % گنتی;


      error = Mac_Read_sfnt_Resource( library, دھارا, ڈیٹا_offsets, گنتی,
                                      face_اشاریہ_اندرونی, aface );
      FT_FREE( ڈیٹا_offsets );
      اگر ( !error )
      {
        (*aface)->عدد_faces  = گنتی;
        (*aface)->face_اشاریہ = face_اشاریہ_اندرونی;
      }
    }

    واپس error;
  }


  /* Check for a valid macbinary header, and if we find one   */
  /* check that the (flattened) resource fork in it is valid. */
  /*                                                          */
  ساکن FT_Error
  Is_Mac_ثنائی( FT_Library  library,
               FT_دھارا   دھارا,
               FT_Long     face_اشاریہ,
               FT_Face    *aface )
  {
    غیرعلامتی حرف  سرخی[128];
    FT_Error       error;
    FT_Long        dlen, انحراف;


    اگر ( !دھارا )
      واپس FT_THROW( Invalid_دھارا_Operation );

    error = FT_دھارا_Seek( دھارا, 0 );
    اگر ( error )
      جاؤ Exit;

    error = FT_دھارا_Read( دھارا, (FT_Byte*)سرخی, 128 );
    اگر ( error )
      جاؤ Exit;

    اگر (            سرخی[ 0] !=   0 ||
                    سرخی[74] !=   0 ||
                    سرخی[82] !=   0 ||
                    سرخی[ 1] ==   0 ||
                    سرخی[ 1] >   33 ||
                    سرخی[63] !=   0 ||
         سرخی[2 + سرخی[1]] !=   0 ||
                  سرخی[0x53] > 0x7F )
      واپس FT_THROW( Unknown_File_Format );

    dlen = ( سرخی[0x53] << 24 ) |
           ( سرخی[0x54] << 16 ) |
           ( سرخی[0x55] <<  8 ) |
             سرخی[0x56];
#اگر 0
    rlen = ( سرخی[0x57] << 24 ) |
           ( سرخی[0x58] << 16 ) |
           ( سرخی[0x59] <<  8 ) |
             سرخی[0x5A];
#اگر_ختم /* 0 */
    انحراف = 128 + ( ( dlen + 127 ) & ~127 );

    واپس IsMacResource( library, دھارا, انحراف, face_اشاریہ, aface );

  Exit:
    واپس error;
  }


  ساکن FT_Error
  بھرو_face_اندر_embedded_rfork( FT_Library           library,
                               FT_دھارا            دھارا,
                               FT_Long              face_اشاریہ,
                               FT_Face             *aface,
                               ثابت FT_Open_دلائل  *دلائل )
  {

#تعریف_منسوخ  FT_COMPONENT
#تعریف_کرو FT_COMPONENT  raccess

    FT_حافظہ  حافظہ = library->حافظہ;
    FT_Error   error  = FT_ERR( Unknown_File_Format );
    FT_UInt    i;

    حرف*      file_نام[FT_RACCESS_N_قواعد];
    FT_Long    offsets[FT_RACCESS_N_قواعد];
    FT_Error   errors[FT_RACCESS_N_قواعد];
    FT_Bool    is_darwin_vfs, vfs_rfork_has_no_font = FALSE; /* not tested */

    FT_Open_دلائل  args2;
    FT_دھارا     stream2 = تہی;


    FT_Raccess_Guess( library, دھارا,
                      دلائل->pathname, file_نام, offsets, errors );

    برائے ( i = 0; i < FT_RACCESS_N_قواعد; i++ )
    {
      is_darwin_vfs = ft_raccess_قاعدہ_by_darwin_vfs( library, i );
      اگر ( is_darwin_vfs && vfs_rfork_has_no_font )
      {
        FT_TRACE3(( "Skip rule %u: darwin vfs resource fork"
                    " is already checked and"
                    " no font is found\n",
                    i ));
        جاری;
      }

      اگر ( errors[i] )
      {
        FT_TRACE3(( "Error 0x%x has occurred in rule %u\n",
                    errors[i], i ));
        جاری;
      }

      args2.flags    = FT_OPEN_PATHNAME;
      args2.pathname = file_نام[i] ? file_نام[i] : دلائل->pathname;

      FT_TRACE3(( "Try rule %u: %s (offset=%ld) ...",
                  i, args2.pathname, offsets[i] ));

      error = FT_دھارا_New( library, &args2, &stream2 );
      اگر ( is_darwin_vfs && FT_ERR_EQ( error, Cannot_Open_دھارا ) )
        vfs_rfork_has_no_font = TRUE;

      اگر ( error )
      {
        FT_TRACE3(( "failed\n" ));
        جاری;
      }

      error = IsMacResource( library, stream2, offsets[i],
                             face_اشاریہ, aface );
      FT_دھارا_Free( stream2, 0 );

      FT_TRACE3(( "%s\n", error ? "failed": "successful" ));

      اگر ( !error )
          توڑو;
      ورنہ اگر ( is_darwin_vfs )
          vfs_rfork_has_no_font = TRUE;
    }

    برائے (i = 0; i < FT_RACCESS_N_قواعد; i++)
    {
      اگر ( file_نام[i] )
        FT_FREE( file_نام[i] );
    }

    /* Caller (load_mac_face) requires FT_Err_Unknown_File_Format. */
    اگر ( error )
      error = FT_ERR( Unknown_File_Format );

    واپس error;

#تعریف_منسوخ  FT_COMPONENT
#تعریف_کرو FT_COMPONENT  objs

  }


  /* Check for some macintosh formats without Carbon framework.    */
  /* Is this a macbinary file?  If so look at the resource fork.   */
  /* Is this a mac dfont file?                                     */
  /* Is this an old style resource fork? (in data)                 */
  /* Else call load_face_in_embedded_rfork to try extra rules      */
  /* (defined in `ftrfork.c').                                     */
  /*                                                               */
  ساکن FT_Error
  بھرو_mac_face( FT_Library           library,
                 FT_دھارا            دھارا,
                 FT_Long              face_اشاریہ,
                 FT_Face             *aface,
                 ثابت FT_Open_دلائل  *دلائل )
  {
    FT_Error error;
    FT_غیر_مستعمل( دلائل );


    error = Is_Mac_ثنائی( library, دھارا, face_اشاریہ, aface );
    اگر ( FT_ERR_EQ( error, Unknown_File_Format ) )
    {

#تعریف_منسوخ  FT_COMPONENT
#تعریف_کرو FT_COMPONENT  raccess

#اگر_تعریف_شدہ FT_DEBUG_LEVEL_TRACE
      FT_TRACE3(( "Try as dfont: " ));
      اگر ( !( دلائل->flags & FT_OPEN_حافظہ ) )
        FT_TRACE3(( "%s ...", دلائل->pathname ));
#اگر_ختم

      error = IsMacResource( library, دھارا, 0, face_اشاریہ, aface );

      FT_TRACE3(( "%s\n", error ? "failed" : "successful" ));

#تعریف_منسوخ  FT_COMPONENT
#تعریف_کرو FT_COMPONENT  objs

    }

    اگر ( ( FT_ERR_EQ( error, Unknown_File_Format )      ||
           FT_ERR_EQ( error, Invalid_دھارا_Operation ) ) &&
         ( دلائل->flags & FT_OPEN_PATHNAME )               )
      error = بھرو_face_اندر_embedded_rfork( library, دھارا,
                                           face_اشاریہ, aface, دلائل );
    واپس error;
  }
#اگر_ختم

#اگر_ختم  /* !FT_MACINTOSH && FT_CONFIG_OPTION_MAC_FONTS */


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Open_Face( FT_Library           library,
                ثابت FT_Open_دلائل*  دلائل,
                FT_Long              face_اشاریہ,
                FT_Face             *aface )
  {
    واپس ft_open_face_اندرونی( library, دلائل, face_اشاریہ, aface, 1 );
  }


  ساکن FT_Error
  ft_open_face_اندرونی( FT_Library           library,
                         ثابت FT_Open_دلائل*  دلائل,
                         FT_Long              face_اشاریہ,
                         FT_Face             *aface,
                         FT_Bool              test_mac_fonts )
  {
    FT_Error     error;
    FT_Driver    driver = تہی;
    FT_حافظہ    حافظہ = تہی;
    FT_دھارا    دھارا = تہی;
    FT_Face      face   = تہی;
    FT_List_گرہ  گرہ   = تہی;
    FT_Bool      external_دھارا;
    FT_Module*   cur;
    FT_Module*   حد_بندی;

#اگر_غیر_تعریف_شدہ FT_CONFIG_OPTION_MAC_FONTS
    FT_غیر_مستعمل( test_mac_fonts );
#اگر_ختم


    /* only use lower 31 bits together with sign bit */
    اگر ( face_اشاریہ > 0 )
      face_اشاریہ &= 0x7FFFFFFFL;
    ورنہ
    {
      face_اشاریہ  = -face_اشاریہ;
      face_اشاریہ &= 0x7FFFFFFFL;
      face_اشاریہ  = -face_اشاریہ;
    }

#اگر_تعریف_شدہ FT_DEBUG_LEVEL_TRACE
    FT_TRACE3(( "FT_Open_Face: " ));
    اگر ( face_اشاریہ < 0 )
      FT_TRACE3(( "Requesting number of faces and named instances\n"));
    ورنہ
    {
      FT_TRACE3(( "Requesting face %ld", face_اشاریہ & 0xFFFFL ));
      اگر ( face_اشاریہ & 0x7FFF0000L )
        FT_TRACE3(( ", named instance %ld", face_اشاریہ >> 16 ));
      FT_TRACE3(( "\n" ));
    }
#اگر_ختم

    /* test for valid `library' delayed to `FT_Stream_New' */

    اگر ( !دلائل )
      واپس FT_THROW( Invalid_دلیل );

    external_دھارا = FT_BOOL( ( دلائل->flags & FT_OPEN_دھارا ) &&
                               دلائل->دھارا                     );

    /* create input stream */
    error = FT_دھارا_New( library, دلائل, &دھارا );
    اگر ( error )
      جاؤ Fail3;

    /* Do this error check after `FT_Stream_New` to ensure that the */
    /* 'close' callback is called.                                  */
    اگر ( !aface && face_اشاریہ >= 0 )
    {
      error = FT_THROW( Invalid_دلیل );
      جاؤ Fail3;
    }

    حافظہ = library->حافظہ;

    /* If the font driver is specified in the `args' structure, use */
    /* it.  Otherwise, we scan the list of registered drivers.      */
    اگر ( ( دلائل->flags & FT_OPEN_DRIVER ) && دلائل->driver )
    {
      driver = FT_DRIVER( دلائل->driver );

      /* not all modules are drivers, so check... */
      اگر ( FT_MODULE_IS_DRIVER( driver ) )
      {
        FT_Int         عدد_params = 0;
        FT_Parameter*  params     = تہی;


        اگر ( دلائل->flags & FT_OPEN_PARAMS )
        {
          عدد_params = دلائل->عدد_params;
          params     = دلائل->params;
        }

        error = open_face( driver, &دھارا, &external_دھارا, face_اشاریہ,
                           عدد_params, params, &face );
        اگر ( !error )
          جاؤ Success;
      }
      ورنہ
        error = FT_THROW( Invalid_Handle );

      FT_دھارا_Free( دھارا, external_دھارا );
      جاؤ Fail;
    }
    ورنہ
    {
      error = FT_ERR( Missing_Module );

      /* check each font driver for an appropriate format */
      cur   = library->modules;
      حد_بندی = cur + library->عدد_modules;

      برائے ( ; cur < حد_بندی; cur++ )
      {
        /* not all modules are font drivers, so check... */
        اگر ( FT_MODULE_IS_DRIVER( cur[0] ) )
        {
          FT_Int         عدد_params = 0;
          FT_Parameter*  params     = تہی;


          driver = FT_DRIVER( cur[0] );

          اگر ( دلائل->flags & FT_OPEN_PARAMS )
          {
            عدد_params = دلائل->عدد_params;
            params     = دلائل->params;
          }

          error = open_face( driver, &دھارا, &external_دھارا, face_اشاریہ,
                             عدد_params, params, &face );
          اگر ( !error )
            جاؤ Success;

#اگر_تعریف_شدہ FT_CONFIG_OPTION_MAC_FONTS
          اگر ( test_mac_fonts                                           &&
               ft_strcmp( cur[0]->clazz->module_name, "truetype" ) == 0 &&
               FT_ERR_EQ( error, جدول_Missing )                        )
          {
            /* TrueType but essential tables are missing */
            error = FT_دھارا_Seek( دھارا, 0 );
            اگر ( error )
              توڑو;

            error = open_face_PS_from_sfnt_دھارا( library,
                                                   دھارا,
                                                   face_اشاریہ,
                                                   عدد_params,
                                                   params,
                                                   aface );
            اگر ( !error )
            {
              FT_دھارا_Free( دھارا, external_دھارا );
              واپس error;
            }
          }
#اگر_ختم

          اگر ( FT_ERR_NEQ( error, Unknown_File_Format ) )
            جاؤ Fail3;
        }
      }

    Fail3:
      /* If we are on the mac, and we get an                          */
      /* FT_Err_Invalid_Stream_Operation it may be because we have an */
      /* empty data fork, so we need to check the resource fork.      */
      اگر ( FT_ERR_NEQ( error, Cannot_Open_دھارا )       &&
           FT_ERR_NEQ( error, Unknown_File_Format )      &&
           FT_ERR_NEQ( error, Invalid_دھارا_Operation ) )
        جاؤ Fail2;

#اگر !defined( FT_MACINTOSH ) && defined( FT_CONFIG_OPTION_MAC_FONTS )
      اگر ( test_mac_fonts )
      {
        error = بھرو_mac_face( library, دھارا, face_اشاریہ, aface, دلائل );
        اگر ( !error )
        {
          /* We don't want to go to Success here.  We've already done   */
          /* that.  On the other hand, if we succeeded we still need to */
          /* close this stream (we opened a different stream which      */
          /* extracted the interesting information out of this stream   */
          /* here.  That stream will still be open and the face will    */
          /* point to it).                                              */
          FT_دھارا_Free( دھارا, external_دھارا );
          واپس error;
        }
      }

      اگر ( FT_ERR_NEQ( error, Unknown_File_Format ) )
        جاؤ Fail2;
#اگر_ختم  /* !FT_MACINTOSH && FT_CONFIG_OPTION_MAC_FONTS */

      /* no driver is able to handle this format */
      error = FT_THROW( Unknown_File_Format );

  Fail2:
      FT_دھارا_Free( دھارا, external_دھارا );
      جاؤ Fail;
    }

  Success:
    FT_TRACE4(( "FT_Open_Face: New face object, adding to list\n" ));

    /* add the face object to its driver's list */
    اگر ( FT_QNEW( گرہ ) )
      جاؤ Fail;

    گرہ->ڈیٹا = face;
    /* don't assume driver is the same as face->driver, so use */
    /* face->driver instead.                                   */
    FT_List_جوڑو( &face->driver->faces_list, گرہ );

    /* now allocate a glyph slot object for the face */
    FT_TRACE4(( "FT_Open_Face: Creating glyph slot\n" ));

    اگر ( face_اشاریہ >= 0 )
    {
      error = FT_New_Glyph_خانہ( face, تہی );
      اگر ( error )
        جاؤ Fail;

      /* finally, allocate a size object for the face */
      {
        FT_سائز  سائز;


        FT_TRACE4(( "FT_Open_Face: Creating size object\n" ));

        error = FT_New_سائز( face, &سائز );
        اگر ( error )
          جاؤ Fail;

        face->سائز = سائز;
      }
    }

    /* some checks */

    اگر ( FT_IS_SCALABLE( face ) )
    {
      اگر ( face->اونچائی < 0 )
        face->اونچائی = (FT_Short)-face->اونچائی;

      اگر ( !FT_HAS_VERTICAL( face ) )
        face->max_advance_اونچائی = (FT_Short)face->اونچائی;
    }

    اگر ( FT_HAS_FIXED_SIZES( face ) )
    {
      FT_Int  i;


      برائے ( i = 0; i < face->عدد_fixed_sizes; i++ )
      {
        FT_Bitmap_سائز*  bsize = face->دستیاب_sizes + i;


        اگر ( bsize->اونچائی < 0 )
          bsize->اونچائی = -bsize->اونچائی;
        اگر ( bsize->x_ppem < 0 )
          bsize->x_ppem = -bsize->x_ppem;
        اگر ( bsize->y_ppem < 0 )
          bsize->y_ppem = -bsize->y_ppem;

        /* check whether negation actually has worked */
        اگر ( bsize->اونچائی < 0 || bsize->x_ppem < 0 || bsize->y_ppem < 0 )
        {
          FT_TRACE0(( "FT_Open_Face:"
                      " Invalid bitmap dimensions for strike %d,"
                      " now disabled\n", i ));
          bsize->چوڑائی  = 0;
          bsize->اونچائی = 0;
          bsize->سائز   = 0;
          bsize->x_ppem = 0;
          bsize->y_ppem = 0;
        }
      }
    }

    /* initialize internal face data */
    {
      FT_Face_اندرونی  اندرونی_2 = face->اندرونی_2;


      اندرونی_2->transform_میٹرکس.xx = 0x10000L;
      اندرونی_2->transform_میٹرکس.xy = 0;
      اندرونی_2->transform_میٹرکس.yx = 0;
      اندرونی_2->transform_میٹرکس.yy = 0x10000L;

      اندرونی_2->transform_delta.x = 0;
      اندرونی_2->transform_delta.y = 0;

      اندرونی_2->refcount = 1;

      اندرونی_2->no_stem_darkening = -1;
    }

    اگر ( aface )
      *aface = face;
    ورنہ
      FT_مکمل_Face( face );

    جاؤ Exit;

  Fail:
    اگر ( گرہ )
      FT_مکمل_Face( face );    /* face must be in the driver's list */
    ورنہ اگر ( face )
      destroy_face( حافظہ, face, driver );

  Exit:
#اگر_تعریف_شدہ FT_DEBUG_LEVEL_TRACE
    اگر ( !error && face_اشاریہ < 0 )
    {
      FT_TRACE3(( "FT_Open_Face: The font has %ld face%s\n",
                  face->عدد_faces,
                  face->عدد_faces == 1 ? "" : "s" ));
      FT_TRACE3(( "              and %ld named instance%s for face %ld\n",
                  face->style_flags >> 16,
                  ( face->style_flags >> 16 ) == 1 ? "" : "s",
                  -face_اشاریہ - 1 ));
    }
#اگر_ختم

    FT_TRACE4(( "FT_Open_Face: Return 0x%x\n", error ));

    واپس error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Attach_File( FT_Face      face,
                  ثابت حرف*  filepathname )
  {
    FT_Open_دلائل  open;


    /* test for valid `face' delayed to `FT_Attach_Stream' */

    اگر ( !filepathname )
      واپس FT_THROW( Invalid_دلیل );

    open.دھارا   = تہی;
    open.flags    = FT_OPEN_PATHNAME;
    open.pathname = (حرف*)filepathname;

    واپس FT_Attach_دھارا( face, &open );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Attach_دھارا( FT_Face              face,
                    ثابت FT_Open_دلائل*  parameters )
  {
    FT_دھارا  دھارا;
    FT_Error   error;
    FT_Driver  driver;

    FT_Driver_Class  clazz;


    /* test for valid `parameters' delayed to `FT_Stream_New' */

    اگر ( !face )
      واپس FT_THROW( Invalid_Face_Handle );

    driver = face->driver;
    اگر ( !driver )
      واپس FT_THROW( Invalid_Driver_Handle );

    error = FT_دھارا_New( driver->جڑ.library, parameters, &دھارا );
    اگر ( error )
      جاؤ Exit;

    /* we implement FT_Attach_Stream in each driver through the */
    /* `attach_file' interface                                  */

    error = FT_ERR( Unimplemented_Feature );
    clazz = driver->clazz;
    اگر ( clazz->attach_file )
      error = clazz->attach_file( face, دھارا );

    /* close the attached stream */
    FT_دھارا_Free( دھارا,
                    FT_BOOL( parameters->دھارا                     &&
                             ( parameters->flags & FT_OPEN_دھارا ) ) );

  Exit:
    واپس error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Reference_Face( FT_Face  face )
  {
    اگر ( !face )
      واپس FT_THROW( Invalid_Face_Handle );

    face->اندرونی_2->refcount++;

    واپس FT_Err_Ok;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_مکمل_Face( FT_Face  face )
  {
    FT_Error     error;
    FT_Driver    driver;
    FT_حافظہ    حافظہ;
    FT_List_گرہ  گرہ;


    error = FT_ERR( Invalid_Face_Handle );
    اگر ( face && face->driver )
    {
      face->اندرونی_2->refcount--;
      اگر ( face->اندرونی_2->refcount > 0 )
        error = FT_Err_Ok;
      ورنہ
      {
        driver = face->driver;
        حافظہ = driver->جڑ.حافظہ;

        /* find face in driver's list */
        گرہ = FT_List_ڈھونڈو( &driver->faces_list, face );
        اگر ( گرہ )
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

    واپس error;
  }


  /* documentation is in ftobjs.h */

  FT_EXPORT_DEF( FT_Error )
  FT_New_سائز( FT_Face   face,
               FT_سائز  *asize )
  {
    FT_Error         error;
    FT_حافظہ        حافظہ;
    FT_Driver        driver;
    FT_Driver_Class  clazz;

    FT_سائز          سائز = تہی;
    FT_List_گرہ      گرہ = تہی;

    FT_سائز_اندرونی  اندرونی_2 = تہی;


    اگر ( !face )
      واپس FT_THROW( Invalid_Face_Handle );

    اگر ( !asize )
      واپس FT_THROW( Invalid_دلیل );

    اگر ( !face->driver )
      واپس FT_THROW( Invalid_Driver_Handle );

    *asize = تہی;

    driver = face->driver;
    clazz  = driver->clazz;
    حافظہ = face->حافظہ;

    /* Allocate new size object and perform basic initialisation */
    اگر ( FT_ALLOC( سائز, clazz->سائز_object_سائز ) || FT_QNEW( گرہ ) )
      جاؤ Exit;

    سائز->face = face;

    اگر ( FT_NEW( اندرونی_2 ) )
      جاؤ Exit;

    سائز->اندرونی_2 = اندرونی_2;

    اگر ( clazz->init_سائز )
      error = clazz->init_سائز( سائز );

    /* in case of success, add to the face's list */
    اگر ( !error )
    {
      *asize     = سائز;
      گرہ->ڈیٹا = سائز;
      FT_List_جوڑو( &face->sizes_list, گرہ );
    }

  Exit:
    اگر ( error )
    {
      FT_FREE( گرہ );
      اگر ( سائز )
        FT_FREE( سائز->اندرونی_2 );
      FT_FREE( سائز );
    }

    واپس error;
  }


  /* documentation is in ftobjs.h */

  FT_EXPORT_DEF( FT_Error )
  FT_مکمل_سائز( FT_سائز  سائز )
  {
    FT_Error     error;
    FT_Driver    driver;
    FT_حافظہ    حافظہ;
    FT_Face      face;
    FT_List_گرہ  گرہ;


    اگر ( !سائز )
      واپس FT_THROW( Invalid_سائز_Handle );

    face = سائز->face;
    اگر ( !face )
      واپس FT_THROW( Invalid_Face_Handle );

    driver = face->driver;
    اگر ( !driver )
      واپس FT_THROW( Invalid_Driver_Handle );

    حافظہ = driver->جڑ.حافظہ;

    error = FT_Err_Ok;
    گرہ  = FT_List_ڈھونڈو( &face->sizes_list, سائز );
    اگر ( گرہ )
    {
      FT_List_Remove( &face->sizes_list, گرہ );
      FT_FREE( گرہ );

      اگر ( face->سائز == سائز )
      {
        face->سائز = تہی;
        اگر ( face->sizes_list.سر )
          face->سائز = (FT_سائز)(face->sizes_list.سر->ڈیٹا);
      }

      destroy_سائز( حافظہ, سائز, driver );
    }
    ورنہ
      error = FT_THROW( Invalid_سائز_Handle );

    واپس error;
  }


  /* documentation is in ftobjs.h */

  FT_بنیاد_DEF( FT_Error )
  FT_مطابقت_سائز( FT_Face          face,
                 FT_سائز_Request  req,
                 FT_Bool          ignore_چوڑائی,
                 FT_ULong*        سائز_اشاریہ )
  {
    FT_Int   i;
    FT_Long  w, h;


    اگر ( !FT_HAS_FIXED_SIZES( face ) )
      واپس FT_THROW( Invalid_Face_Handle );

    /* FT_Bitmap_Size doesn't provide enough info... */
    اگر ( req->type != FT_سائز_REQUEST_TYPE_NOMINAL )
      واپس FT_THROW( Unimplemented_Feature );

    w = FT_REQUEST_چوڑائی ( req );
    h = FT_REQUEST_اونچائی( req );

    اگر ( req->چوڑائی && !req->اونچائی )
      h = w;
    ورنہ اگر ( !req->چوڑائی && req->اونچائی )
      w = h;

    w = FT_PIX_باری( w );
    h = FT_PIX_باری( h );

    اگر ( !w || !h )
      واپس FT_THROW( Invalid_Pixel_سائز );

    برائے ( i = 0; i < face->عدد_fixed_sizes; i++ )
    {
      FT_Bitmap_سائز*  bsize = face->دستیاب_sizes + i;


      اگر ( h != FT_PIX_باری( bsize->y_ppem ) )
        جاری;

      اگر ( w == FT_PIX_باری( bsize->x_ppem ) || ignore_چوڑائی )
      {
        FT_TRACE3(( "FT_Match_Size: bitmap strike %d matches\n", i ));

        اگر ( سائز_اشاریہ )
          *سائز_اشاریہ = (FT_ULong)i;

        واپس FT_Err_Ok;
      }
    }

    FT_TRACE3(( "FT_Match_Size: no matching bitmap strike\n" ));

    واپس FT_THROW( Invalid_Pixel_سائز );
  }


  /* documentation is in ftobjs.h */

  FT_بنیاد_DEF( خالی )
  ft_synthesize_vertical_metrics( FT_Glyph_Metrics*  metrics,
                                  FT_Pos             advance )
  {
    FT_Pos  اونچائی = metrics->اونچائی;


    /* compensate for glyph with bbox above/below the baseline */
    اگر ( metrics->horiBearingY < 0 )
    {
      اگر ( اونچائی < metrics->horiBearingY )
        اونچائی = metrics->horiBearingY;
    }
    ورنہ اگر ( metrics->horiBearingY > 0 )
      اونچائی -= metrics->horiBearingY;

    /* the factor 1.2 is a heuristical value */
    اگر ( !advance )
      advance = اونچائی * 12 / 10;

    metrics->vertBearingX = metrics->horiBearingX - metrics->horiAdvance / 2;
    metrics->vertBearingY = ( advance - اونچائی ) / 2;
    metrics->vertAdvance  = advance;
  }


  ساکن خالی
  ft_recompute_scaled_metrics( FT_Face           face,
                               FT_سائز_Metrics*  metrics )
  {
    /* Compute root ascender, descender, test height, and max_advance */

#اگر_تعریف_شدہ GRID_FIT_METRICS
    metrics->ascender    = FT_PIX_CEIL( FT_MulFix( face->ascender,
                                                   metrics->y_scale ) );

    metrics->descender   = FT_PIX_FLOOR( FT_MulFix( face->descender,
                                                    metrics->y_scale ) );

    metrics->اونچائی      = FT_PIX_باری( FT_MulFix( face->اونچائی,
                                                    metrics->y_scale ) );

    metrics->max_advance = FT_PIX_باری( FT_MulFix( face->max_advance_چوڑائی,
                                                    metrics->x_scale ) );
#ورنہ /* !GRID_FIT_METRICS */
    metrics->ascender    = FT_MulFix( face->ascender,
                                      metrics->y_scale );

    metrics->descender   = FT_MulFix( face->descender,
                                      metrics->y_scale );

    metrics->اونچائی      = FT_MulFix( face->اونچائی,
                                      metrics->y_scale );

    metrics->max_advance = FT_MulFix( face->max_advance_چوڑائی,
                                      metrics->x_scale );
#اگر_ختم /* !GRID_FIT_METRICS */
  }


  FT_بنیاد_DEF( خالی )
  FT_Select_Metrics( FT_Face   face,
                     FT_ULong  strike_اشاریہ )
  {
    FT_سائز_Metrics*  metrics;
    FT_Bitmap_سائز*   bsize;


    metrics = &face->سائز->metrics;
    bsize   = face->دستیاب_sizes + strike_اشاریہ;

    metrics->x_ppem = (FT_UShort)( ( bsize->x_ppem + 32 ) >> 6 );
    metrics->y_ppem = (FT_UShort)( ( bsize->y_ppem + 32 ) >> 6 );

    اگر ( FT_IS_SCALABLE( face ) )
    {
      metrics->x_scale = FT_DivFix( bsize->x_ppem,
                                    face->units_فی_EM );
      metrics->y_scale = FT_DivFix( bsize->y_ppem,
                                    face->units_فی_EM );

      ft_recompute_scaled_metrics( face, metrics );
    }
    ورنہ
    {
      metrics->x_scale     = 1L << 16;
      metrics->y_scale     = 1L << 16;
      metrics->ascender    = bsize->y_ppem;
      metrics->descender   = 0;
      metrics->اونچائی      = bsize->اونچائی << 6;
      metrics->max_advance = bsize->x_ppem;
    }
  }


  FT_بنیاد_DEF( FT_Error )
  FT_Request_Metrics( FT_Face          face,
                      FT_سائز_Request  req )
  {
    FT_Error  error = FT_Err_Ok;

    FT_سائز_Metrics*  metrics;


    metrics = &face->سائز->metrics;

    اگر ( FT_IS_SCALABLE( face ) )
    {
      FT_Long  w = 0, h = 0, scaled_w = 0, scaled_h = 0;


      انتخاب ( req->type )
      {
      حالت FT_سائز_REQUEST_TYPE_NOMINAL:
        w = h = face->units_فی_EM;
        توڑو;

      حالت FT_سائز_REQUEST_TYPE_REAL_DIM:
        w = h = face->ascender - face->descender;
        توڑو;

      حالت FT_سائز_REQUEST_TYPE_BBOX:
        w = face->bbox.xMax - face->bbox.xMin;
        h = face->bbox.yMax - face->bbox.yMin;
        توڑو;

      حالت FT_سائز_REQUEST_TYPE_CELL:
        w = face->max_advance_چوڑائی;
        h = face->ascender - face->descender;
        توڑو;

      حالت FT_سائز_REQUEST_TYPE_SCALES:
        metrics->x_scale = (FT_Fixed)req->چوڑائی;
        metrics->y_scale = (FT_Fixed)req->اونچائی;
        اگر ( !metrics->x_scale )
          metrics->x_scale = metrics->y_scale;
        ورنہ اگر ( !metrics->y_scale )
          metrics->y_scale = metrics->x_scale;
        جاؤ Calculate_Ppem;

      حالت FT_سائز_REQUEST_TYPE_MAX:
        توڑو;
      }

      /* to be on the safe side */
      اگر ( w < 0 )
        w = -w;

      اگر ( h < 0 )
        h = -h;

      scaled_w = FT_REQUEST_چوڑائی ( req );
      scaled_h = FT_REQUEST_اونچائی( req );

      /* determine scales */
      اگر ( req->اونچائی || !req->چوڑائی )
      {
        اگر ( h == 0 )
        {
          FT_ERROR(( "FT_Request_Metrics: Divide by zero\n" ));
          error = FT_ERR( Divide_By_Zero );
          جاؤ Exit;
        }

        metrics->y_scale = FT_DivFix( scaled_h, h );
      }

      اگر ( req->چوڑائی )
      {
        اگر ( w == 0 )
        {
          FT_ERROR(( "FT_Request_Metrics: Divide by zero\n" ));
          error = FT_ERR( Divide_By_Zero );
          جاؤ Exit;
        }

        metrics->x_scale = FT_DivFix( scaled_w, w );
      }
      ورنہ
      {
        metrics->x_scale = metrics->y_scale;
        scaled_w = FT_MulDiv( scaled_h, w, h );
      }

      اگر ( !req->اونچائی )
      {
        metrics->y_scale = metrics->x_scale;
        scaled_h = FT_MulDiv( scaled_w, h, w );
      }

      اگر ( req->type == FT_سائز_REQUEST_TYPE_CELL )
      {
        اگر ( metrics->y_scale > metrics->x_scale )
          metrics->y_scale = metrics->x_scale;
        ورنہ
          metrics->x_scale = metrics->y_scale;
      }

  Calculate_Ppem:
      /* calculate the ppems */
      اگر ( req->type != FT_سائز_REQUEST_TYPE_NOMINAL )
      {
        scaled_w = FT_MulFix( face->units_فی_EM, metrics->x_scale );
        scaled_h = FT_MulFix( face->units_فی_EM, metrics->y_scale );
      }

      scaled_w = ( scaled_w + 32 ) >> 6;
      scaled_h = ( scaled_h + 32 ) >> 6;
      اگر ( scaled_w > (FT_Long)FT_USHORT_MAX ||
           scaled_h > (FT_Long)FT_USHORT_MAX )
      {
        FT_ERROR(( "FT_Request_Metrics: Resulting ppem size too large\n" ));
        error = FT_ERR( Invalid_Pixel_سائز );
        جاؤ Exit;
      }

      metrics->x_ppem = (FT_UShort)scaled_w;
      metrics->y_ppem = (FT_UShort)scaled_h;

      ft_recompute_scaled_metrics( face, metrics );
    }
    ورنہ
    {
      FT_ZERO( metrics );
      metrics->x_scale = 1L << 16;
      metrics->y_scale = 1L << 16;
    }

  Exit:
    واپس error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Select_سائز( FT_Face  face,
                  FT_Int   strike_اشاریہ )
  {
    FT_Error         error = FT_Err_Ok;
    FT_Driver_Class  clazz;


    اگر ( !face || !FT_HAS_FIXED_SIZES( face ) )
      واپس FT_THROW( Invalid_Face_Handle );

    اگر ( strike_اشاریہ < 0 || strike_اشاریہ >= face->عدد_fixed_sizes )
      واپس FT_THROW( Invalid_دلیل );

    clazz = face->driver->clazz;

    اگر ( clazz->select_سائز )
    {
      error = clazz->select_سائز( face->سائز, (FT_ULong)strike_اشاریہ );

      FT_TRACE5(( "FT_Select_Size (%s driver):\n",
                  face->driver->جڑ.clazz->module_name ));
    }
    ورنہ
    {
      FT_Select_Metrics( face, (FT_ULong)strike_اشاریہ );

      FT_TRACE5(( "FT_Select_Size:\n" ));
    }

#اگر_تعریف_شدہ FT_DEBUG_LEVEL_TRACE
    {
      FT_سائز_Metrics*  metrics = &face->سائز->metrics;


      FT_TRACE5(( "  x scale: %ld (%f)\n",
                  metrics->x_scale, (دوہرا)metrics->x_scale / 65536 ));
      FT_TRACE5(( "  y scale: %ld (%f)\n",
                  metrics->y_scale, (دوہرا)metrics->y_scale / 65536 ));
      FT_TRACE5(( "  ascender: %f\n",
                  (دوہرا)metrics->ascender / 64 ));
      FT_TRACE5(( "  descender: %f\n",
                  (دوہرا)metrics->descender / 64 ));
      FT_TRACE5(( "  height: %f\n",
                  (دوہرا)metrics->اونچائی / 64 ));
      FT_TRACE5(( "  max advance: %f\n",
                  (دوہرا)metrics->max_advance / 64 ));
      FT_TRACE5(( "  x ppem: %d\n", metrics->x_ppem ));
      FT_TRACE5(( "  y ppem: %d\n", metrics->y_ppem ));
    }
#اگر_ختم

    واپس error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Request_سائز( FT_Face          face,
                   FT_سائز_Request  req )
  {
    FT_Error         error;
    FT_Driver_Class  clazz;
    FT_ULong         strike_اشاریہ;


    اگر ( !face )
      واپس FT_THROW( Invalid_Face_Handle );

    اگر ( !face->سائز )
      واپس FT_THROW( Invalid_سائز_Handle );

    اگر ( !req || req->چوڑائی < 0 || req->اونچائی < 0 ||
         req->type >= FT_سائز_REQUEST_TYPE_MAX )
      واپس FT_THROW( Invalid_دلیل );

    /* signal the auto-hinter to recompute its size metrics */
    /* (if requested)                                       */
    face->سائز->اندرونی_2->autohint_metrics.x_scale = 0;

    clazz = face->driver->clazz;

    اگر ( clazz->request_سائز )
    {
      error = clazz->request_سائز( face->سائز, req );

      FT_TRACE5(( "FT_Request_Size (%s driver):\n",
                  face->driver->جڑ.clazz->module_name ));
    }
    ورنہ اگر ( !FT_IS_SCALABLE( face ) && FT_HAS_FIXED_SIZES( face ) )
    {
      /*
       * The reason that a driver doesn't have `request_size' defined is
       * either that the scaling here suffices or that the supported formats
       * are bitmap-only and size matching is not implemented.
       *
       * In the latter case, a simple size matching is done.
       */
      error = FT_مطابقت_سائز( face, req, 0, &strike_اشاریہ );
      اگر ( error )
        جاؤ Exit;

      واپس FT_Select_سائز( face, (FT_Int)strike_اشاریہ );
    }
    ورنہ
    {
      error = FT_Request_Metrics( face, req );
      اگر ( error )
        جاؤ Exit;

      FT_TRACE5(( "FT_Request_Size:\n" ));
    }

#اگر_تعریف_شدہ FT_DEBUG_LEVEL_TRACE
    {
      FT_سائز_Metrics*  metrics = &face->سائز->metrics;


      FT_TRACE5(( "  x scale: %ld (%f)\n",
                  metrics->x_scale, (دوہرا)metrics->x_scale / 65536 ));
      FT_TRACE5(( "  y scale: %ld (%f)\n",
                  metrics->y_scale, (دوہرا)metrics->y_scale / 65536 ));
      FT_TRACE5(( "  ascender: %f\n",
                  (دوہرا)metrics->ascender / 64 ));
      FT_TRACE5(( "  descender: %f\n",
                  (دوہرا)metrics->descender / 64 ));
      FT_TRACE5(( "  height: %f\n",
                  (دوہرا)metrics->اونچائی / 64 ));
      FT_TRACE5(( "  max advance: %f\n",
                  (دوہرا)metrics->max_advance / 64 ));
      FT_TRACE5(( "  x ppem: %d\n", metrics->x_ppem ));
      FT_TRACE5(( "  y ppem: %d\n", metrics->y_ppem ));
    }
#اگر_ختم

  Exit:
    واپس error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_سیٹ_Char_سائز( FT_Face     face,
                    FT_F26Dot6  char_چوڑائی,
                    FT_F26Dot6  char_اونچائی,
                    FT_UInt     horz_resolution,
                    FT_UInt     vert_resolution )
  {
    FT_سائز_Request_Rec  req;


    /* check of `face' delayed to `FT_Request_Size' */

    اگر ( !char_چوڑائی )
      char_چوڑائی = char_اونچائی;
    ورنہ اگر ( !char_اونچائی )
      char_اونچائی = char_چوڑائی;

    اگر ( !horz_resolution )
      horz_resolution = vert_resolution;
    ورنہ اگر ( !vert_resolution )
      vert_resolution = horz_resolution;

    اگر ( char_چوڑائی  < 1 * 64 )
      char_چوڑائی  = 1 * 64;
    اگر ( char_اونچائی < 1 * 64 )
      char_اونچائی = 1 * 64;

    اگر ( !horz_resolution )
      horz_resolution = vert_resolution = 72;

    req.type           = FT_سائز_REQUEST_TYPE_NOMINAL;
    req.چوڑائی          = char_چوڑائی;
    req.اونچائی         = char_اونچائی;
    req.horiResolution = horz_resolution;
    req.vertResolution = vert_resolution;

    واپس FT_Request_سائز( face, &req );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_سیٹ_Pixel_Sizes( FT_Face  face,
                      FT_UInt  pixel_چوڑائی,
                      FT_UInt  pixel_اونچائی )
  {
    FT_سائز_Request_Rec  req;


    /* check of `face' delayed to `FT_Request_Size' */

    اگر ( pixel_چوڑائی == 0 )
      pixel_چوڑائی = pixel_اونچائی;
    ورنہ اگر ( pixel_اونچائی == 0 )
      pixel_اونچائی = pixel_چوڑائی;

    اگر ( pixel_چوڑائی  < 1 )
      pixel_چوڑائی  = 1;
    اگر ( pixel_اونچائی < 1 )
      pixel_اونچائی = 1;

    /* use `>=' to avoid potential compiler warning on 16bit platforms */
    اگر ( pixel_چوڑائی >= 0xFFFFU )
      pixel_چوڑائی = 0xFFFFU;
    اگر ( pixel_اونچائی >= 0xFFFFU )
      pixel_اونچائی = 0xFFFFU;

    req.type           = FT_سائز_REQUEST_TYPE_NOMINAL;
    req.چوڑائی          = (FT_Long)( pixel_چوڑائی << 6 );
    req.اونچائی         = (FT_Long)( pixel_اونچائی << 6 );
    req.horiResolution = 0;
    req.vertResolution = 0;

    واپس FT_Request_سائز( face, &req );
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


    اگر ( !face )
      واپس FT_THROW( Invalid_Face_Handle );

    اگر ( !akerning )
      واپس FT_THROW( Invalid_دلیل );

    driver = face->driver;

    akerning->x = 0;
    akerning->y = 0;

    اگر ( driver->clazz->get_kerning )
    {
      error = driver->clazz->get_kerning( face,
                                          بایاں_glyph,
                                          دایاں_glyph,
                                          akerning );
      اگر ( !error )
      {
        اگر ( kern_منوال != FT_KERNING_UNSCALED )
        {
          akerning->x = FT_MulFix( akerning->x, face->سائز->metrics.x_scale );
          akerning->y = FT_MulFix( akerning->y, face->سائز->metrics.y_scale );

          اگر ( kern_منوال != FT_KERNING_UNFITTED )
          {
            FT_Pos  orig_x = akerning->x;
            FT_Pos  orig_y = akerning->y;


            /* we scale down kerning values for small ppem values */
            /* to avoid that rounding makes them too big.         */
            /* `25' has been determined heuristically.            */
            اگر ( face->سائز->metrics.x_ppem < 25 )
              akerning->x = FT_MulDiv( orig_x,
                                       face->سائز->metrics.x_ppem, 25 );
            اگر ( face->سائز->metrics.y_ppem < 25 )
              akerning->y = FT_MulDiv( orig_y,
                                       face->سائز->metrics.y_ppem, 25 );

            akerning->x = FT_PIX_باری( akerning->x );
            akerning->y = FT_PIX_باری( akerning->y );

#اگر_تعریف_شدہ FT_DEBUG_LEVEL_TRACE
            {
              FT_Pos  orig_x_rounded = FT_PIX_باری( orig_x );
              FT_Pos  orig_y_rounded = FT_PIX_باری( orig_y );


              اگر ( akerning->x != orig_x_rounded ||
                   akerning->y != orig_y_rounded )
                FT_TRACE5(( "FT_Get_Kerning: horizontal kerning"
                            " (%ld, %ld) scaled down to (%ld, %ld) pixels\n",
                            orig_x_rounded / 64, orig_y_rounded / 64,
                            akerning->x / 64, akerning->y / 64 ));
            }
#اگر_ختم
          }
        }
      }
    }

    واپس error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Track_Kerning( FT_Face    face,
                        FT_Fixed   نقطہ_سائز,
                        FT_Int     degree,
                        FT_Fixed*  akerning )
  {
    FT_Service_Kerning  service;
    FT_Error            error = FT_Err_Ok;


    اگر ( !face )
      واپس FT_THROW( Invalid_Face_Handle );

    اگر ( !akerning )
      واپس FT_THROW( Invalid_دلیل );

    FT_FACE_ڈھونڈو_SERVICE( face, service, KERNING );
    اگر ( !service )
      واپس FT_THROW( Unimplemented_Feature );

    error = service->get_track( face,
                                نقطہ_سائز,
                                degree,
                                akerning );

    واپس error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Select_Charmap( FT_Face      face,
                     FT_Encoding  encoding )
  {
    FT_CharMap*  cur;
    FT_CharMap*  حد_بندی;


    اگر ( !face )
      واپس FT_THROW( Invalid_Face_Handle );

    /* FT_ENCODING_NONE is a valid encoding for BDF, PCF, and Windows FNT */
    اگر ( encoding == FT_ENCODING_NONE && !face->عدد_charmaps )
      واپس FT_THROW( Invalid_دلیل );

    /* FT_ENCODING_UNICODE is special.  We try to find the `best' Unicode */
    /* charmap available, i.e., one with UCS-4 characters, if possible.   */
    /*                                                                    */
    /* This is done by find_unicode_charmap() above, to share code.       */
    اگر ( encoding == FT_ENCODING_UNICODE )
      واپس ڈھونڈو_unicode_charmap( face );

    cur = face->charmaps;
    اگر ( !cur )
      واپس FT_THROW( Invalid_CharMap_Handle );

    حد_بندی = cur + face->عدد_charmaps;

    برائے ( ; cur < حد_بندی; cur++ )
    {
      اگر ( cur[0]->encoding == encoding )
      {
        face->charmap = cur[0];
        واپس FT_Err_Ok;
      }
    }

    واپس FT_THROW( Invalid_دلیل );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_سیٹ_Charmap( FT_Face     face,
                  FT_CharMap  charmap )
  {
    FT_CharMap*  cur;
    FT_CharMap*  حد_بندی;


    اگر ( !face )
      واپس FT_THROW( Invalid_Face_Handle );

    cur = face->charmaps;
    اگر ( !cur || !charmap )
      واپس FT_THROW( Invalid_CharMap_Handle );

    حد_بندی = cur + face->عدد_charmaps;

    برائے ( ; cur < حد_بندی; cur++ )
    {
      اگر ( cur[0] == charmap )
      {
        FT_Long  format = FT_Get_CMap_Format( charmap );


        اگر ( format != 14 && format != 15 )
        {
          face->charmap = cur[0];
          واپس FT_Err_Ok;
        }
      }
    }

    واپس FT_THROW( Invalid_دلیل );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Int )
  FT_Get_Charmap_اشاریہ( FT_CharMap  charmap )
  {
    FT_Int  i;


    اگر ( !charmap || !charmap->face )
      واپس -1;

    برائے ( i = 0; i < charmap->face->عدد_charmaps; i++ )
      اگر ( charmap->face->charmaps[i] == charmap )
        توڑو;

    FT_ASSERT( i < charmap->face->عدد_charmaps );

    واپس i;
  }


  ساکن خالی
  ft_cmap_مکمل_اندرونی( FT_CMap  cmap )
  {
    FT_CMap_Class  clazz  = cmap->clazz;
    FT_Face        face   = cmap->charmap.face;
    FT_حافظہ      حافظہ = FT_FACE_حافظہ( face );


    اگر ( clazz->مکمل )
      clazz->مکمل( cmap );

    FT_FREE( cmap );
  }


  FT_بنیاد_DEF( خالی )
  FT_CMap_مکمل( FT_CMap  cmap )
  {
    اگر ( cmap )
    {
      FT_Face    face   = cmap->charmap.face;
      FT_حافظہ  حافظہ = FT_FACE_حافظہ( face );
      FT_Error   error;
      FT_Int     i, j;


      برائے ( i = 0; i < face->عدد_charmaps; i++ )
      {
        اگر ( (FT_CMap)face->charmaps[i] == cmap )
        {
          FT_CharMap  آخری_charmap = face->charmaps[face->عدد_charmaps - 1];


          اگر ( FT_QRENEW_ARRAY( face->charmaps,
                                face->عدد_charmaps,
                                face->عدد_charmaps - 1 ) )
            واپس;

          /* remove it from our list of charmaps */
          برائے ( j = i + 1; j < face->عدد_charmaps; j++ )
          {
            اگر ( j == face->عدد_charmaps - 1 )
              face->charmaps[j - 1] = آخری_charmap;
            ورنہ
              face->charmaps[j - 1] = face->charmaps[j];
          }

          face->عدد_charmaps--;

          اگر ( (FT_CMap)face->charmap == cmap )
            face->charmap = تہی;

          ft_cmap_مکمل_اندرونی( cmap );

          توڑو;
        }
      }
    }
  }


  FT_بنیاد_DEF( FT_Error )
  FT_CMap_New( FT_CMap_Class  clazz,
               FT_اشارہ_گر     init_ڈیٹا,
               FT_CharMap     charmap,
               FT_CMap       *acmap )
  {
    FT_Error   error;
    FT_Face    face;
    FT_حافظہ  حافظہ;
    FT_CMap    cmap = تہی;


    اگر ( !clazz || !charmap || !charmap->face )
      واپس FT_THROW( Invalid_دلیل );

    face   = charmap->face;
    حافظہ = FT_FACE_حافظہ( face );

    اگر ( !FT_ALLOC( cmap, clazz->سائز ) )
    {
      cmap->charmap = *charmap;
      cmap->clazz   = clazz;

      اگر ( clazz->init )
      {
        error = clazz->init( cmap, init_ڈیٹا );
        اگر ( error )
          جاؤ Fail;
      }

      /* add it to our list of charmaps */
      اگر ( FT_QRENEW_ARRAY( face->charmaps,
                            face->عدد_charmaps,
                            face->عدد_charmaps + 1 ) )
        جاؤ Fail;

      face->charmaps[face->عدد_charmaps++] = (FT_CharMap)cmap;
    }

  Exit:
    اگر ( acmap )
      *acmap = cmap;

    واپس error;

  Fail:
    ft_cmap_مکمل_اندرونی( cmap );
    cmap = تہی;
    جاؤ Exit;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt )
  FT_Get_Char_اشاریہ( FT_Face   face,
                     FT_ULong  charcode )
  {
    FT_UInt  نتیجہ = 0;


    اگر ( face && face->charmap )
    {
      FT_CMap  cmap = FT_CMAP( face->charmap );


      اگر ( charcode > 0xFFFFFFFFUL )
      {
        FT_TRACE1(( "FT_Get_Char_Index: too large charcode" ));
        FT_TRACE1(( " 0x%lx is truncated\n", charcode ));
      }

      نتیجہ = cmap->clazz->char_اشاریہ( cmap, (FT_UInt32)charcode );
      اگر ( نتیجہ >= (FT_UInt)face->عدد_glyphs )
        نتیجہ = 0;
    }

    واپس نتیجہ;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_ULong )
  FT_Get_پہلا_Char( FT_Face   face,
                     FT_UInt  *agindex )
  {
    FT_ULong  نتیجہ = 0;
    FT_UInt   gindex = 0;


    /* only do something if we have a charmap, and we have glyphs at all */
    اگر ( face && face->charmap && face->عدد_glyphs )
    {
      gindex = FT_Get_Char_اشاریہ( face, 0 );
      اگر ( gindex == 0 )
        نتیجہ = FT_Get_اگلا_Char( face, 0, &gindex );
    }

    اگر ( agindex )
      *agindex = gindex;

    واپس نتیجہ;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_ULong )
  FT_Get_اگلا_Char( FT_Face   face,
                    FT_ULong  charcode,
                    FT_UInt  *agindex )
  {
    FT_ULong  نتیجہ = 0;
    FT_UInt   gindex = 0;


    اگر ( face && face->charmap && face->عدد_glyphs )
    {
      FT_UInt32  رمز = (FT_UInt32)charcode;
      FT_CMap    cmap = FT_CMAP( face->charmap );


      کرو
      {
        gindex = cmap->clazz->char_اگلا( cmap, &رمز );

      } جب ( gindex >= (FT_UInt)face->عدد_glyphs );

      نتیجہ = ( gindex == 0 ) ? 0 : رمز;
    }

    اگر ( agindex )
      *agindex = gindex;

    واپس نتیجہ;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Face_Properties( FT_Face        face,
                      FT_UInt        عدد_properties,
                      FT_Parameter*  properties )
  {
    FT_Error  error = FT_Err_Ok;


    اگر ( عدد_properties > 0 && !properties )
    {
      error = FT_THROW( Invalid_دلیل );
      جاؤ Exit;
    }

    برائے ( ; عدد_properties > 0; عدد_properties-- )
    {
      اگر ( properties->لیبل == FT_PARAM_لیبل_STEM_DARKENING )
      {
        اگر ( properties->ڈیٹا )
        {
          اگر ( *( (FT_Bool*)properties->ڈیٹا ) == TRUE )
            face->اندرونی_2->no_stem_darkening = FALSE;
          ورنہ
            face->اندرونی_2->no_stem_darkening = TRUE;
        }
        ورنہ
        {
          /* use module default */
          face->اندرونی_2->no_stem_darkening = -1;
        }
      }
      ورنہ اگر ( properties->لیبل == FT_PARAM_لیبل_LCD_FILTER_WEIGHTS )
      {
        error = FT_THROW( Unimplemented_Feature );
        جاؤ Exit;
      }
      ورنہ اگر ( properties->لیبل == FT_PARAM_لیبل_RANDOM_بیج )
      {
        اگر ( properties->ڈیٹا )
        {
          face->اندرونی_2->random_بیج = *( (FT_Int32*)properties->ڈیٹا );
          اگر ( face->اندرونی_2->random_بیج < 0 )
            face->اندرونی_2->random_بیج = 0;
        }
        ورنہ
        {
          /* use module default */
          face->اندرونی_2->random_بیج = -1;
        }
      }
      ورنہ
      {
        error = FT_THROW( Invalid_دلیل );
        جاؤ Exit;
      }

      اگر ( error )
        توڑو;

      properties++;
    }

  Exit:
    واپس error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt )
  FT_Face_Get_Char_Variant_اشاریہ( FT_Face   face,
                               FT_ULong  charcode,
                               FT_ULong  variantSelector )
  {
    FT_UInt  نتیجہ = 0;


    اگر ( face                                           &&
         face->charmap                                  &&
         face->charmap->encoding == FT_ENCODING_UNICODE )
    {
      FT_CharMap  charmap = ڈھونڈو_variant_selector_charmap( face );
      FT_CMap     ucmap = FT_CMAP( face->charmap );


      اگر ( charmap )
      {
        FT_CMap  vcmap = FT_CMAP( charmap );


        اگر ( charcode > 0xFFFFFFFFUL )
        {
          FT_TRACE1(( "FT_Face_GetCharVariantIndex:"
                      " too large charcode" ));
          FT_TRACE1(( " 0x%lx is truncated\n", charcode ));
        }
        اگر ( variantSelector > 0xFFFFFFFFUL )
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

    واپس نتیجہ;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Int )
  FT_Face_GetCharVariantIsDefault( FT_Face   face,
                                   FT_ULong  charcode,
                                   FT_ULong  variantSelector )
  {
    FT_Int  نتیجہ = -1;


    اگر ( face )
    {
      FT_CharMap  charmap = ڈھونڈو_variant_selector_charmap( face );


      اگر ( charmap )
      {
        FT_CMap  vcmap = FT_CMAP( charmap );


        اگر ( charcode > 0xFFFFFFFFUL )
        {
          FT_TRACE1(( "FT_Face_GetCharVariantIsDefault:"
                      " too large charcode" ));
          FT_TRACE1(( " 0x%lx is truncated\n", charcode ));
        }
        اگر ( variantSelector > 0xFFFFFFFFUL )
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

    واپس نتیجہ;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt32* )
  FT_Face_GetVariantSelectors( FT_Face  face )
  {
    FT_UInt32  *نتیجہ = تہی;


    اگر ( face )
    {
      FT_CharMap  charmap = ڈھونڈو_variant_selector_charmap( face );


      اگر ( charmap )
      {
        FT_CMap    vcmap  = FT_CMAP( charmap );
        FT_حافظہ  حافظہ = FT_FACE_حافظہ( face );


        نتیجہ = vcmap->clazz->variant_list( vcmap, حافظہ );
      }
    }

    واپس نتیجہ;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt32* )
  FT_Face_Get_Variants_کا_Char( FT_Face   face,
                             FT_ULong  charcode )
  {
    FT_UInt32  *نتیجہ = تہی;


    اگر ( face )
    {
      FT_CharMap  charmap = ڈھونڈو_variant_selector_charmap( face );


      اگر ( charmap )
      {
        FT_CMap    vcmap  = FT_CMAP( charmap );
        FT_حافظہ  حافظہ = FT_FACE_حافظہ( face );


        اگر ( charcode > 0xFFFFFFFFUL )
        {
          FT_TRACE1(( "FT_Face_GetVariantsOfChar: too large charcode" ));
          FT_TRACE1(( " 0x%lx is truncated\n", charcode ));
        }

        نتیجہ = vcmap->clazz->charvariant_list( vcmap, حافظہ,
                                                 (FT_UInt32)charcode );
      }
    }
    واپس نتیجہ;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt32* )
  FT_Face_Get_Chars_کا_Variant( FT_Face   face,
                             FT_ULong  variantSelector )
  {
    FT_UInt32  *نتیجہ = تہی;


    اگر ( face )
    {
      FT_CharMap  charmap = ڈھونڈو_variant_selector_charmap( face );


      اگر ( charmap )
      {
        FT_CMap    vcmap  = FT_CMAP( charmap );
        FT_حافظہ  حافظہ = FT_FACE_حافظہ( face );


        اگر ( variantSelector > 0xFFFFFFFFUL )
        {
          FT_TRACE1(( "FT_Get_Char_Index: too large variantSelector" ));
          FT_TRACE1(( " 0x%lx is truncated\n", variantSelector ));
        }

        نتیجہ = vcmap->clazz->variantchar_list( vcmap, حافظہ,
                                                 (FT_UInt32)variantSelector );
      }
    }

    واپس نتیجہ;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt )
  FT_Get_Name_اشاریہ( FT_Face           face,
                     ثابت FT_String*  glyph_name )
  {
    FT_UInt  نتیجہ = 0;


    اگر ( face                       &&
         FT_HAS_GLYPH_نام( face ) &&
         glyph_name                 )
    {
      FT_Service_GlyphDict  service;


      FT_FACE_LOOKUP_SERVICE( face,
                              service,
                              GLYPH_DICT );

      اگر ( service && service->name_اشاریہ )
        نتیجہ = service->name_اشاریہ( face, glyph_name );
    }

    واپس نتیجہ;
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


    اگر ( !face )
      واپس FT_THROW( Invalid_Face_Handle );

    اگر ( !بفر || بفر_max == 0 )
      واپس FT_THROW( Invalid_دلیل );

    /* clean up buffer */
    ((FT_Byte*)بفر)[0] = '\0';

    اگر ( (FT_Long)glyph_اشاریہ >= face->عدد_glyphs )
      واپس FT_THROW( Invalid_Glyph_اشاریہ );

    اگر ( !FT_HAS_GLYPH_نام( face ) )
      واپس FT_THROW( Invalid_دلیل );

    FT_FACE_LOOKUP_SERVICE( face, service, GLYPH_DICT );
    اگر ( service && service->get_name )
      error = service->get_name( face, glyph_اشاریہ, بفر, بفر_max );
    ورنہ
      error = FT_THROW( Invalid_دلیل );

    واپس error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( ثابت حرف* )
  FT_Get_Postscript_Name( FT_Face  face )
  {
    ثابت حرف*  نتیجہ = تہی;


    اگر ( !face )
      جاؤ Exit;

    اگر ( !نتیجہ )
    {
      FT_Service_PsFontName  service;


      FT_FACE_LOOKUP_SERVICE( face,
                              service,
                              POSTSCRIPT_FONT_NAME );

      اگر ( service && service->get_ps_font_name )
        نتیجہ = service->get_ps_font_name( face );
    }

  Exit:
    واپس نتیجہ;
  }


  /* documentation is in tttables.h */

  FT_EXPORT_DEF( خالی* )
  FT_Get_Sfnt_جدول( FT_Face      face,
                     FT_Sfnt_لیبل  لیبل )
  {
    خالی*                  جدول = تہی;
    FT_Service_SFNT_جدول  service;


    اگر ( face && FT_IS_SFNT( face ) )
    {
      FT_FACE_ڈھونڈو_SERVICE( face, service, SFNT_جدول );
      اگر ( service )
        جدول = service->get_جدول( face, لیبل );
    }

    واپس جدول;
  }


  /* documentation is in tttables.h */

  FT_EXPORT_DEF( FT_Error )
  FT_بھرو_Sfnt_جدول( FT_Face    face,
                      FT_ULong   لیبل,
                      FT_Long    انحراف,
                      FT_Byte*   بفر,
                      FT_ULong*  لمبائی )
  {
    FT_Service_SFNT_جدول  service;


    اگر ( !face || !FT_IS_SFNT( face ) )
      واپس FT_THROW( Invalid_Face_Handle );

    FT_FACE_ڈھونڈو_SERVICE( face, service, SFNT_جدول );
    اگر ( !service )
      واپس FT_THROW( Unimplemented_Feature );

    واپس service->بھرو_جدول( face, لیبل, انحراف, بفر, لمبائی );
  }


  /* documentation is in tttables.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Sfnt_جدول_Info( FT_Face    face,
                      FT_UInt    جدول_اشاریہ,
                      FT_ULong  *لیبل,
                      FT_ULong  *لمبائی )
  {
    FT_Service_SFNT_جدول  service;
    FT_ULong               انحراف;


    /* test for valid `length' delayed to `service->table_info' */

    اگر ( !face || !FT_IS_SFNT( face ) )
      واپس FT_THROW( Invalid_Face_Handle );

    FT_FACE_ڈھونڈو_SERVICE( face, service, SFNT_جدول );
    اگر ( !service )
      واپس FT_THROW( Unimplemented_Feature );

    واپس service->جدول_info( face, جدول_اشاریہ, لیبل, &انحراف, لمبائی );
  }


  /* documentation is in tttables.h */

  FT_EXPORT_DEF( FT_ULong )
  FT_Get_CMap_Language_ID( FT_CharMap  charmap )
  {
    FT_Service_TTCMaps  service;
    FT_Face             face;
    TT_CMapInfo         cmap_info;


    اگر ( !charmap || !charmap->face )
      واپس 0;

    face = charmap->face;
    FT_FACE_ڈھونڈو_SERVICE( face, service, TT_CMAP );
    اگر ( !service )
      واپس 0;
    اگر ( service->get_cmap_info( charmap, &cmap_info ))
      واپس 0;

    واپس cmap_info.language;
  }


  /* documentation is in tttables.h */

  FT_EXPORT_DEF( FT_Long )
  FT_Get_CMap_Format( FT_CharMap  charmap )
  {
    FT_Service_TTCMaps  service;
    FT_Face             face;
    TT_CMapInfo         cmap_info;


    اگر ( !charmap || !charmap->face )
      واپس -1;

    face = charmap->face;
    FT_FACE_ڈھونڈو_SERVICE( face, service, TT_CMAP );
    اگر ( !service )
      واپس -1;
    اگر ( service->get_cmap_info( charmap, &cmap_info ))
      واپس -1;

    واپس cmap_info.format;
  }


  /* documentation is in ftsizes.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Activate_سائز( FT_سائز  سائز )
  {
    FT_Face  face;


    اگر ( !سائز )
      واپس FT_THROW( Invalid_سائز_Handle );

    face = سائز->face;
    اگر ( !face || !face->driver )
      واپس FT_THROW( Invalid_Face_Handle );

    /* we don't need anything more complex than that; all size objects */
    /* are already listed by the face                                  */
    face->سائز = سائز;

    واپس FT_Err_Ok;
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
  FT_بنیاد_DEF( FT_Renderer )
  FT_Lookup_Renderer( FT_Library       library,
                      FT_Glyph_Format  format,
                      FT_List_گرہ*     گرہ )
  {
    FT_List_گرہ  cur;
    FT_Renderer  نتیجہ = تہی;


    اگر ( !library )
      جاؤ Exit;

    cur = library->renderers.سر;

    اگر ( گرہ )
    {
      اگر ( *گرہ )
        cur = (*گرہ)->اگلا;
      *گرہ = تہی;
    }

    جب ( cur )
    {
      FT_Renderer  renderer = FT_RENDERER( cur->ڈیٹا );


      اگر ( renderer->glyph_format == format )
      {
        اگر ( گرہ )
          *گرہ = cur;

        نتیجہ = renderer;
        توڑو;
      }
      cur = cur->اگلا;
    }

  Exit:
    واپس نتیجہ;
  }


  ساکن FT_Renderer
  ft_lookup_glyph_renderer( FT_Glyph_خانہ  خانہ )
  {
    FT_Face      face    = خانہ->face;
    FT_Library   library = FT_FACE_LIBRARY( face );
    FT_Renderer  نتیجہ  = library->cur_renderer;


    اگر ( !نتیجہ || نتیجہ->glyph_format != خانہ->format )
      نتیجہ = FT_Lookup_Renderer( library, خانہ->format, 0 );

    واپس نتیجہ;
  }


  ساکن خالی
  ft_سیٹ_current_renderer( FT_Library  library )
  {
    FT_Renderer  renderer;


    renderer = FT_Lookup_Renderer( library, FT_GLYPH_FORMAT_OUTLINE, 0 );
    library->cur_renderer = renderer;
  }


  ساکن FT_Error
  ft_جوڑو_renderer( FT_Module  module )
  {
    FT_Library   library = module->library;
    FT_حافظہ    حافظہ  = library->حافظہ;
    FT_Error     error;
    FT_List_گرہ  گرہ    = تہی;


    اگر ( FT_QNEW( گرہ ) )
      جاؤ Exit;

    {
      FT_Renderer         render = FT_RENDERER( module );
      FT_Renderer_Class*  clazz  = (FT_Renderer_Class*)module->clazz;


      render->clazz        = clazz;
      render->glyph_format = clazz->glyph_format;

      /* allocate raster object if needed */
      اگر ( clazz->raster_class && clazz->raster_class->raster_new )
      {
        error = clazz->raster_class->raster_new( حافظہ, &render->raster );
        اگر ( error )
          جاؤ Fail;

        render->raster_render = clazz->raster_class->raster_render;
        render->render        = clazz->render_glyph;
      }

#اگر_تعریف_شدہ FT_CONFIG_OPTION_SVG
      اگر ( clazz->glyph_format == FT_GLYPH_FORMAT_SVG )
        render->render = clazz->render_glyph;
#اگر_ختم

      /* add to list */
      گرہ->ڈیٹا = module;
      FT_List_جوڑو( &library->renderers, گرہ );

      ft_سیٹ_current_renderer( library );
    }

  Fail:
    اگر ( error )
      FT_FREE( گرہ );

  Exit:
    واپس error;
  }


  ساکن خالی
  ft_remove_renderer( FT_Module  module )
  {
    FT_Library   library;
    FT_حافظہ    حافظہ;
    FT_List_گرہ  گرہ;


    library = module->library;
    اگر ( !library )
      واپس;

    حافظہ = library->حافظہ;

    گرہ = FT_List_ڈھونڈو( &library->renderers, module );
    اگر ( گرہ )
    {
      FT_Renderer  render = FT_RENDERER( module );


      /* release raster object, if any */
      اگر ( render->raster )
        render->clazz->raster_class->raster_مکمل( render->raster );

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

    واپس FT_Lookup_Renderer( library, format, 0 );
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


    اگر ( !library )
    {
      error = FT_THROW( Invalid_Library_Handle );
      جاؤ Exit;
    }

    اگر ( !renderer )
    {
      error = FT_THROW( Invalid_دلیل );
      جاؤ Exit;
    }

    اگر ( عدد_params > 0 && !parameters )
    {
      error = FT_THROW( Invalid_دلیل );
      جاؤ Exit;
    }

    گرہ = FT_List_ڈھونڈو( &library->renderers, renderer );
    اگر ( !گرہ )
    {
      error = FT_THROW( Invalid_دلیل );
      جاؤ Exit;
    }

    FT_List_اوپر( &library->renderers, گرہ );

    اگر ( renderer->glyph_format == FT_GLYPH_FORMAT_OUTLINE )
      library->cur_renderer = renderer;

    سیٹ_منوال = renderer->clazz->سیٹ_منوال;

    برائے ( ; عدد_params > 0; عدد_params-- )
    {
      error = سیٹ_منوال( renderer, parameters->لیبل, parameters->ڈیٹا );
      اگر ( error )
        توڑو;
      parameters++;
    }

  Exit:
    واپس error;
  }


  FT_بنیاد_DEF( FT_Error )
  FT_Render_Glyph_اندرونی( FT_Library      library,
                            FT_Glyph_خانہ    خانہ,
                            FT_Render_منوال  render_منوال )
  {
    FT_Error     error = FT_Err_Ok;
    FT_Face      face  = خانہ->face;
    FT_Renderer  renderer;
    FT_List_گرہ  گرہ  = تہی;


    /* try to render colored glyph layers as a special case */
    اگر ( خانہ->اندرونی_2->بھرو_flags & FT_بھرو_COLOR &&
         خانہ->format == FT_GLYPH_FORMAT_OUTLINE    )
    {
      FT_LayerIterator  iterator;

      FT_UInt  بنیاد_glyph = خانہ->glyph_اشاریہ;

      FT_Bool  have_layers;
      FT_UInt  glyph_اشاریہ;
      FT_UInt  color_اشاریہ;


      iterator.p  = تہی;
      have_layers = FT_Get_Color_Glyph_Layer( face,
                                              بنیاد_glyph,
                                              &glyph_اشاریہ,
                                              &color_اشاریہ,
                                              &iterator );
      اگر ( have_layers )
      {
        error = FT_New_Glyph_خانہ( face, تہی );
        اگر ( !error )
        {
          TT_Face       ttface = (TT_Face)face;
          SFNT_Service  sfnt   = (SFNT_Service)ttface->sfnt;


          کرو
          {
            FT_Int32  بھرو_flags = خانہ->اندرونی_2->بھرو_flags;


            /* disable the `FT_LOAD_COLOR' flag to avoid recursion */
            /* right here in this function                         */
            بھرو_flags &= ~FT_بھرو_COLOR;

            /* render into the new `face->glyph' glyph slot */
            بھرو_flags |= FT_بھرو_RENDER | FT_بھرو_NO_BITMAP;

            error = FT_بھرو_Glyph( face, glyph_اشاریہ, بھرو_flags );
            اگر ( error )
              توڑو;

            /* blend new `face->glyph' into old `slot'; */
            /* at the first call, `slot' is still empty */
            error = sfnt->colr_blend( ttface,
                                      color_اشاریہ,
                                      خانہ,
                                      face->glyph );
            اگر ( error )
              توڑو;

          } جب ( FT_Get_Color_Glyph_Layer( face,
                                              بنیاد_glyph,
                                              &glyph_اشاریہ,
                                              &color_اشاریہ,
                                              &iterator ) );

          اگر ( !error )
            خانہ->format = FT_GLYPH_FORMAT_BITMAP;

          /* this call also restores `slot' as the glyph slot */
          FT_مکمل_Glyph_خانہ( face->glyph );
        }

        اگر ( !error )
          واپس error;

        /* Failed to do the colored layer.  Draw outline instead. */
        خانہ->format = FT_GLYPH_FORMAT_OUTLINE;
      }
    }

    /* small shortcut for the very common case */
    اگر ( خانہ->format == FT_GLYPH_FORMAT_OUTLINE )
    {
      renderer = library->cur_renderer;
      گرہ     = library->renderers.سر;
    }
    ورنہ
      renderer = FT_Lookup_Renderer( library, خانہ->format, &گرہ );

    error = FT_ERR( Cannot_Render_Glyph );
    جب ( renderer )
    {
      error = renderer->render( renderer, خانہ, render_منوال, تہی );
      اگر ( !error                                   ||
           FT_ERR_NEQ( error, Cannot_Render_Glyph ) )
        توڑو;

      /* `FT_Err_Cannot_Render_Glyph` is returned if the render mode */
      /* is unsupported by the current renderer for this glyph image */
      /* format.                                                     */

      /* now, look for another renderer that supports the same */
      /* format.                                               */
      renderer = FT_Lookup_Renderer( library, خانہ->format, &گرہ );
    }

    /* it is not an error if we cannot render a bitmap glyph */
    اگر ( FT_ERR_EQ( error, Cannot_Render_Glyph ) &&
         خانہ->format == FT_GLYPH_FORMAT_BITMAP  )
      error = FT_Err_Ok;

#اگر_تعریف_شدہ FT_DEBUG_LEVEL_TRACE

#تعریف_منسوخ  FT_COMPONENT
#تعریف_کرو FT_COMPONENT  checksum

    /*
     * Computing the MD5 checksum is expensive, unnecessarily distorting a
     * possible profiling of FreeType if compiled with tracing support.  For
     * this reason, we execute the following code only if explicitly
     * requested.
     */

    /* we use FT_TRACE3 in this block */
    اگر ( !error                               &&
         ft_trace_سطحیں[trace_checksum] >= 3 &&
         خانہ->bitmap.بفر                  )
    {
      FT_Bitmap  bitmap;
      FT_Error   err;


      FT_Bitmap_Init( &bitmap );

      /* we convert to a single bitmap format for computing the checksum */
      /* this also converts the bitmap flow to `down' (i.e., pitch > 0)  */
      err = FT_Bitmap_Convert( library, &خانہ->bitmap, &bitmap, 1 );
      اگر ( !err )
      {
        MD5_CTX        ctx;
        غیرعلامتی حرف  md5[16];
        غیرعلامتی طویل  coverage = 0;
        صحیح            i, j;
        صحیح            صفیں  = (صحیح)bitmap.صفیں;
        صحیح            pitch = bitmap.pitch;


        FT_TRACE3(( "FT_Render_Glyph: bitmap %dx%d, %s (mode %d)\n",
                    pitch,
                    صفیں,
                    pixel_modes[خانہ->bitmap.pixel_منوال],
                    خانہ->bitmap.pixel_منوال ));

        برائے ( i = 0; i < صفیں; i++ )
          برائے ( j = 0; j < pitch; j++ )
            coverage += bitmap.بفر[i * pitch + j];

        FT_TRACE3(( "  Total coverage: %lu\n", coverage ));

        MD5_Init( &ctx );
        اگر ( bitmap.بفر )
          MD5_Update( &ctx, bitmap.بفر,
                      (غیرعلامتی طویل)صفیں * (غیرعلامتی طویل)pitch );
        MD5_Final( md5, &ctx );

        FT_TRACE3(( "  MD5 checksum: " ));
        برائے ( i = 0; i < 16; i++ )
          FT_TRACE3(( "%02X", md5[i] ));
        FT_TRACE3(( "\n" ));
      }

      FT_Bitmap_مکمل( library, &bitmap );
    }

    /*
     * Dump bitmap in Netpbm format (PBM or PGM).
     */

    /* we use FT_TRACE7 in this block */
    اگر ( !error                               &&
         ft_trace_سطحیں[trace_checksum] >= 7 &&
         خانہ->bitmap.بفر                  )
    {
      اگر ( خانہ->bitmap.صفیں  < 128U &&
           خانہ->bitmap.چوڑائی < 128U )
      {
        صحیح  صفیں  = (صحیح)خانہ->bitmap.صفیں;
        صحیح  چوڑائی = (صحیح)خانہ->bitmap.چوڑائی;
        صحیح  pitch =      خانہ->bitmap.pitch;
        صحیح  i, j, m;

        غیرعلامتی حرف*  topleft = خانہ->bitmap.بفر;


        اگر ( pitch < 0 )
          topleft -= pitch * ( صفیں - 1 );

        FT_TRACE7(( "Netpbm image: start\n" ));
        انتخاب ( خانہ->bitmap.pixel_منوال )
        {
        حالت FT_PIXEL_منوال_MONO:
          FT_TRACE7(( "P1 %d %d\n", چوڑائی, صفیں ));
          برائے ( i = 0; i < صفیں; i++ )
          {
            برائے ( j = 0; j < چوڑائی; )
              برائے ( m = 128; m > 0 && j < چوڑائی; m >>= 1, j++ )
                FT_TRACE7(( " %d",
                            ( topleft[i * pitch + j / 8] & m ) != 0 ));
            FT_TRACE7(( "\n" ));
          }
          توڑو;

        طےشدہ:
          FT_TRACE7(( "P2 %d %d 255\n", چوڑائی, صفیں ));
          برائے ( i = 0; i < صفیں; i++ )
          {
            برائے ( j = 0; j < چوڑائی; j += 1 )
              FT_TRACE7(( " %3u", topleft[i * pitch + j] ));
            FT_TRACE7(( "\n" ));
          }
        }
        FT_TRACE7(( "Netpbm image: end\n" ));
      }
      ورنہ
        FT_TRACE7(( "Netpbm image: too large, omitted\n" ));
    }

#تعریف_منسوخ  FT_COMPONENT
#تعریف_کرو FT_COMPONENT  objs

#اگر_ختم /* FT_DEBUG_LEVEL_TRACE */

    واپس error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Render_Glyph( FT_Glyph_خانہ    خانہ,
                   FT_Render_منوال  render_منوال )
  {
    FT_Library  library;


    اگر ( !خانہ || !خانہ->face )
      واپس FT_THROW( Invalid_دلیل );

    library = FT_FACE_LIBRARY( خانہ->face );

    واپس FT_Render_Glyph_اندرونی( library, خانہ, render_منوال );
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
  ساکن خالی
  Destroy_Module( FT_Module  module )
  {
    ثابت FT_Module_Class*  clazz   = module->clazz;
    FT_Library              library = module->library;
    FT_حافظہ               حافظہ  = module->حافظہ;


    اگر ( library && library->auto_hinter == module )
      library->auto_hinter = تہی;

    /* if the module is a renderer */
    اگر ( FT_MODULE_IS_RENDERER( module ) )
      ft_remove_renderer( module );

    /* if the module is a font driver, add some steps */
    اگر ( FT_MODULE_IS_DRIVER( module ) )
      Destroy_Driver( FT_DRIVER( module ) );

    /* finalize the module object */
    اگر ( clazz->module_مکمل )
      clazz->module_مکمل( module );

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
    FT_Module  module = تہی;
    FT_UInt    nn;


#تعریف_کرو FREETYPE_VER_FIXED  ( ( (FT_Long)FREETYPE_MAJOR << 16 ) | \
                                FREETYPE_MINOR                  )

    اگر ( !library )
      واپس FT_THROW( Invalid_Library_Handle );

    اگر ( !clazz )
      واپس FT_THROW( Invalid_دلیل );

    /* check FreeType version */
    اگر ( clazz->module_requires > FREETYPE_VER_FIXED )
      واپس FT_THROW( Invalid_نسخہ );

    /* look for a module with the same name in the library's table */
    برائے ( nn = 0; nn < library->عدد_modules; nn++ )
    {
      module = library->modules[nn];
      اگر ( ft_strcmp( module->clazz->module_name, clazz->module_name ) == 0 )
      {
        /* this installed module has the same name, compare their versions */
        اگر ( clazz->module_نسخہ <= module->clazz->module_نسخہ )
          واپس FT_THROW( زیریں_Module_نسخہ );

        /* remove the module from our list, then exit the loop to replace */
        /* it by our new version..                                        */
        FT_Remove_Module( library, module );
        توڑو;
      }
    }

    حافظہ = library->حافظہ;
    error  = FT_Err_Ok;

    اگر ( library->عدد_modules >= FT_MAX_MODULES )
    {
      error = FT_THROW( Too_Many_Drivers );
      جاؤ Exit;
    }

    /* allocate module object */
    اگر ( FT_ALLOC( module, clazz->module_سائز ) )
      جاؤ Exit;

    /* base initialization */
    module->clazz   = clazz;
    module->library = library;
    module->حافظہ  = حافظہ;

    /* check whether the module is a renderer - this must be performed */
    /* before the normal module initialization                         */
    اگر ( FT_MODULE_IS_RENDERER( module ) )
    {
      /* add to the renderers list */
      error = ft_جوڑو_renderer( module );
      اگر ( error )
        جاؤ Fail;
    }

    /* is the module a auto-hinter? */
    اگر ( FT_MODULE_IS_HINTER( module ) )
      library->auto_hinter = module;

    /* if the module is a font driver */
    اگر ( FT_MODULE_IS_DRIVER( module ) )
    {
      FT_Driver  driver = FT_DRIVER( module );


      driver->clazz = (FT_Driver_Class)module->clazz;
    }

    اگر ( clazz->module_init )
    {
      error = clazz->module_init( module );
      اگر ( error )
        جاؤ Fail;
    }

    /* add module to the library's table */
    library->modules[library->عدد_modules++] = module;

  Exit:
    واپس error;

  Fail:
    اگر ( FT_MODULE_IS_RENDERER( module ) )
    {
      FT_Renderer  renderer = FT_RENDERER( module );


      اگر ( renderer->clazz                                          &&
           renderer->clazz->glyph_format == FT_GLYPH_FORMAT_OUTLINE &&
           renderer->raster                                         )
        renderer->clazz->raster_class->raster_مکمل( renderer->raster );
    }

    FT_FREE( module );
    جاؤ Exit;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Module )
  FT_Get_Module( FT_Library   library,
                 ثابت حرف*  module_name )
  {
    FT_Module   نتیجہ = تہی;
    FT_Module*  cur;
    FT_Module*  حد_بندی;


    اگر ( !library || !module_name )
      واپس نتیجہ;

    cur   = library->modules;
    حد_بندی = cur + library->عدد_modules;

    برائے ( ; cur < حد_بندی; cur++ )
      اگر ( ft_strcmp( cur[0]->clazz->module_name, module_name ) == 0 )
      {
        نتیجہ = cur[0];
        توڑو;
      }

    واپس نتیجہ;
  }


  /* documentation is in ftobjs.h */

  FT_بنیاد_DEF( ثابت خالی* )
  FT_Get_Module_Interface( FT_Library   library,
                           ثابت حرف*  mod_name )
  {
    FT_Module  module;


    /* test for valid `library' delayed to FT_Get_Module() */

    module = FT_Get_Module( library, mod_name );

    واپس module ? module->clazz->module_interface : 0;
  }


  FT_بنیاد_DEF( FT_اشارہ_گر )
  ft_module_get_service( FT_Module    module,
                         ثابت حرف*  service_id,
                         FT_Bool      global )
  {
    FT_اشارہ_گر  نتیجہ = تہی;


    اگر ( module )
    {
      FT_ASSERT( module->clazz && module->clazz->get_interface );

      /* first, look for the service in the module */
      اگر ( module->clazz->get_interface )
        نتیجہ = module->clazz->get_interface( module, service_id );

      اگر ( global && !نتیجہ )
      {
        /* we didn't find it, look in all other modules then */
        FT_Library  library = module->library;
        FT_Module*  cur     = library->modules;
        FT_Module*  حد_بندی   = cur + library->عدد_modules;


        برائے ( ; cur < حد_بندی; cur++ )
        {
          اگر ( cur[0] != module )
          {
            FT_ASSERT( cur[0]->clazz );

            اگر ( cur[0]->clazz->get_interface )
            {
              نتیجہ = cur[0]->clazz->get_interface( cur[0], service_id );
              اگر ( نتیجہ )
                توڑو;
            }
          }
        }
      }
    }

    واپس نتیجہ;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Remove_Module( FT_Library  library,
                    FT_Module   module )
  {
    /* try to find the module from the table, then remove it from there */

    اگر ( !library )
      واپس FT_THROW( Invalid_Library_Handle );

    اگر ( module )
    {
      FT_Module*  cur   = library->modules;
      FT_Module*  حد_بندی = cur + library->عدد_modules;


      برائے ( ; cur < حد_بندی; cur++ )
      {
        اگر ( cur[0] == module )
        {
          /* remove it from the table */
          library->عدد_modules--;
          حد_بندی--;
          جب ( cur < حد_بندی )
          {
            cur[0] = cur[1];
            cur++;
          }
          حد_بندی[0] = تہی;

          /* destroy the module */
          Destroy_Module( module );

          واپس FT_Err_Ok;
        }
      }
    }
    واپس FT_THROW( Invalid_Driver_Handle );
  }


  ساکن FT_Error
  ft_property_do( FT_Library        library,
                  ثابت FT_String*  module_name,
                  ثابت FT_String*  property_name,
                  خالی*             قدر,
                  FT_Bool           سیٹ,
                  FT_Bool           قدر_is_string )
  {
    FT_Module*           cur;
    FT_Module*           حد_بندی;
    FT_Module_Interface  interface;

    FT_Service_Properties  service;

#اگر_تعریف_شدہ FT_DEBUG_LEVEL_ERROR
    ثابت FT_String*  سیٹ_name  = "FT_Property_Set";
    ثابت FT_String*  get_name  = "FT_Property_Get";
    ثابت FT_String*  func_name = سیٹ ? سیٹ_name : get_name;
#اگر_ختم

    FT_Bool  missing_func;


    اگر ( !library )
      واپس FT_THROW( Invalid_Library_Handle );

    اگر ( !module_name || !property_name || !قدر )
      واپس FT_THROW( Invalid_دلیل );

    cur   = library->modules;
    حد_بندی = cur + library->عدد_modules;

    /* search module */
    برائے ( ; cur < حد_بندی; cur++ )
      اگر ( !ft_strcmp( cur[0]->clazz->module_name, module_name ) )
        توڑو;

    اگر ( cur == حد_بندی )
    {
      FT_TRACE2(( "%s: can't find module `%s'\n",
                  func_name, module_name ));
      واپس FT_THROW( Missing_Module );
    }

    /* check whether we have a service interface */
    اگر ( !cur[0]->clazz->get_interface )
    {
      FT_TRACE2(( "%s: module `%s' doesn't support properties\n",
                  func_name, module_name ));
      واپس FT_THROW( Unimplemented_Feature );
    }

    /* search property service */
    interface = cur[0]->clazz->get_interface( cur[0],
                                              FT_SERVICE_ID_PROPERTIES );
    اگر ( !interface )
    {
      FT_TRACE2(( "%s: module `%s' doesn't support properties\n",
                  func_name, module_name ));
      واپس FT_THROW( Unimplemented_Feature );
    }

    service = (FT_Service_Properties)interface;

    اگر ( سیٹ )
      missing_func = FT_BOOL( !service->سیٹ_property );
    ورنہ
      missing_func = FT_BOOL( !service->get_property );

    اگر ( missing_func )
    {
      FT_TRACE2(( "%s: property service of module `%s' is broken\n",
                  func_name, module_name ));
      واپس FT_THROW( Unimplemented_Feature );
    }

    واپس سیٹ ? service->سیٹ_property( cur[0],
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
                   ثابت خالی*       قدر )
  {
    واپس ft_property_do( library,
                           module_name,
                           property_name,
                           (خالی*)قدر,
                           TRUE,
                           FALSE );
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Property_Get( FT_Library        library,
                   ثابت FT_String*  module_name,
                   ثابت FT_String*  property_name,
                   خالی*             قدر )
  {
    واپس ft_property_do( library,
                           module_name,
                           property_name,
                           قدر,
                           FALSE,
                           FALSE );
  }


#اگر_تعریف_شدہ FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES

  /* this variant is used for handling the FREETYPE_PROPERTIES */
  /* environment variable                                      */

  FT_بنیاد_DEF( FT_Error )
  ft_property_string_سیٹ( FT_Library        library,
                          ثابت FT_String*  module_name,
                          ثابت FT_String*  property_name,
                          FT_String*        قدر )
  {
    واپس ft_property_do( library,
                           module_name,
                           property_name,
                           (خالی*)قدر,
                           TRUE,
                           TRUE );
  }

#اگر_ختم


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
    اگر ( !library )
      واپس FT_THROW( Invalid_Library_Handle );

    library->refcount++;

    واپس FT_Err_Ok;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_New_Library( FT_حافظہ    حافظہ,
                  FT_Library  *alibrary )
  {
    FT_Library  library = تہی;
    FT_Error    error;


    اگر ( !حافظہ || !alibrary )
      واپس FT_THROW( Invalid_دلیل );

#اگر_غیر_تعریف_شدہ FT_DEBUG_LOGGING
#اگر_تعریف_شدہ FT_DEBUG_LEVEL_ERROR
    /* init debugging support */
    ft_debug_init();
#اگر_ختم /* FT_DEBUG_LEVEL_ERROR */
#اگر_ختم /* !FT_DEBUG_LOGGING */

    /* first of all, allocate the library object */
    اگر ( FT_NEW( library ) )
      واپس error;

    library->حافظہ = حافظہ;

    library->نسخہ_major = FREETYPE_MAJOR;
    library->نسخہ_minor = FREETYPE_MINOR;
    library->نسخہ_patch = FREETYPE_PATCH;

    library->refcount = 1;

    /* That's ok now */
    *alibrary = library;

    واپس FT_Err_Ok;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( خالی )
  FT_Library_نسخہ( FT_Library   library,
                      FT_Int      *amajor,
                      FT_Int      *aminor,
                      FT_Int      *apatch )
  {
    FT_Int  major = 0;
    FT_Int  minor = 0;
    FT_Int  patch = 0;


    اگر ( library )
    {
      major = library->نسخہ_major;
      minor = library->نسخہ_minor;
      patch = library->نسخہ_patch;
    }

    اگر ( amajor )
      *amajor = major;

    اگر ( aminor )
      *aminor = minor;

    اگر ( apatch )
      *apatch = patch;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_مکمل_Library( FT_Library  library )
  {
    FT_حافظہ  حافظہ;


    اگر ( !library )
      واپس FT_THROW( Invalid_Library_Handle );

    library->refcount--;
    اگر ( library->refcount > 0 )
      جاؤ Exit;

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
      ثابت حرف*  driver_name[] = { "type42", تہی };


      برائے ( m = 0;
            m < حجم ( driver_name ) / حجم ( driver_name[0] );
            m++ )
      {
        برائے ( n = 0; n < library->عدد_modules; n++ )
        {
          FT_Module    module      = library->modules[n];
          ثابت حرف*  module_name = module->clazz->module_name;
          FT_List      faces;


          اگر ( driver_name[m]                                &&
               ft_strcmp( module_name, driver_name[m] ) != 0 )
            جاری;

          اگر ( ( module->clazz->module_flags & FT_MODULE_FONT_DRIVER ) == 0 )
            جاری;

          FT_TRACE7(( "FT_Done_Library: close faces for %s\n", module_name ));

          faces = &FT_DRIVER( module )->faces_list;
          جب ( faces->سر )
          {
            FT_مکمل_Face( FT_FACE( faces->سر->ڈیٹا ) );
            اگر ( faces->سر )
              FT_TRACE0(( "FT_Done_Library: failed to free some faces\n" ));
          }
        }
      }
    }

    /* Close all other modules in the library */
#اگر 1
    /* XXX Modules are removed in the reversed order so that  */
    /* type42 module is removed before truetype module.  This */
    /* avoids double free in some occasions.  It is a hack.   */
    جب ( library->عدد_modules > 0 )
      FT_Remove_Module( library,
                        library->modules[library->عدد_modules - 1] );
#ورنہ
    {
      FT_UInt  n;


      برائے ( n = 0; n < library->عدد_modules; n++ )
      {
        FT_Module  module = library->modules[n];


        اگر ( module )
        {
          Destroy_Module( module );
          library->modules[n] = تہی;
        }
      }
    }
#اگر_ختم

    FT_FREE( library );

  Exit:
    واپس FT_Err_Ok;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( خالی )
  FT_سیٹ_Debug_Hook( FT_Library         library,
                     FT_UInt            hook_اشاریہ,
                     FT_DebugHook_Func  debug_hook )
  {
    اگر ( library && debug_hook &&
         hook_اشاریہ <
           ( حجم ( library->debug_hooks ) / حجم ( خالی* ) ) )
      library->debug_hooks[hook_اشاریہ] = debug_hook;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_TrueTypeEngineType )
  FT_Get_TrueType_Engine_Type( FT_Library  library )
  {
    FT_TrueTypeEngineType  نتیجہ = FT_TRUETYPE_ENGINE_TYPE_NONE;


    اگر ( library )
    {
      FT_Module  module = FT_Get_Module( library, "truetype" );


      اگر ( module )
      {
        FT_Service_TrueTypeEngine  service;


        service = (FT_Service_TrueTypeEngine)
                    ft_module_get_service( module,
                                           FT_SERVICE_ID_TRUETYPE_ENGINE,
                                           0 );
        اگر ( service )
          نتیجہ = service->engine_type;
      }
    }

    واپس نتیجہ;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_SubGlyph_Info( FT_Glyph_خانہ  glyph,
                        FT_UInt       sub_اشاریہ,
                        FT_Int       *p_اشاریہ,
                        FT_UInt      *p_flags,
                        FT_Int       *p_arg1,
                        FT_Int       *p_arg2,
                        FT_میٹرکس    *p_transform )
  {
    FT_Error  error = FT_ERR( Invalid_دلیل );


    اگر ( glyph                                      &&
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

    واپس error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Get_Color_Glyph_Layer( FT_Face            face,
                            FT_UInt            بنیاد_glyph,
                            FT_UInt           *aglyph_اشاریہ,
                            FT_UInt           *acolor_اشاریہ,
                            FT_LayerIterator*  iterator )
  {
    TT_Face       ttface;
    SFNT_Service  sfnt;


    اگر ( !face                                   ||
         !aglyph_اشاریہ                           ||
         !acolor_اشاریہ                           ||
         !iterator                               ||
         بنیاد_glyph >= (FT_UInt)face->عدد_glyphs )
      واپس 0;

    اگر ( !FT_IS_SFNT( face ) )
      واپس 0;

    ttface = (TT_Face)face;
    sfnt   = (SFNT_Service)ttface->sfnt;

    اگر ( sfnt->get_colr_layer )
      واپس sfnt->get_colr_layer( ttface,
                                   بنیاد_glyph,
                                   aglyph_اشاریہ,
                                   acolor_اشاریہ,
                                   iterator );
    ورنہ
      واپس 0;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Get_Color_Glyph_Paint( FT_Face                  face,
                            FT_UInt                  بنیاد_glyph,
                            FT_Color_جڑ_Transform  جڑ_transform,
                            FT_OpaquePaint*          paint )
  {
    TT_Face       ttface;
    SFNT_Service  sfnt;


    اگر ( !face || !paint )
      واپس 0;

    اگر ( !FT_IS_SFNT( face ) )
      واپس 0;

    ttface = (TT_Face)face;
    sfnt   = (SFNT_Service)ttface->sfnt;

    اگر ( sfnt->get_colr_glyph_paint )
      واپس sfnt->get_colr_glyph_paint( ttface,
                                         بنیاد_glyph,
                                         جڑ_transform,
                                         paint );
    ورنہ
      واپس 0;
  }


  /* documentation is in ftcolor.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Get_Color_Glyph_ClipBox( FT_Face      face,
                              FT_UInt      بنیاد_glyph,
                              FT_ClipBox*  clip_box )
  {
    TT_Face       ttface;
    SFNT_Service  sfnt;


    اگر ( !face || !clip_box )
      واپس 0;

    اگر ( !FT_IS_SFNT( face ) )
      واپس 0;

    ttface = (TT_Face)face;
    sfnt   = (SFNT_Service)ttface->sfnt;

    اگر ( sfnt->get_color_glyph_clipbox )
      واپس sfnt->get_color_glyph_clipbox( ttface,
                                            بنیاد_glyph,
                                            clip_box );
    ورنہ
      واپس 0;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Get_Paint_Layers( FT_Face            face,
                       FT_LayerIterator*  layer_iterator,
                       FT_OpaquePaint*    paint )
  {
    TT_Face       ttface;
    SFNT_Service  sfnt;


    اگر ( !face || !paint || !layer_iterator )
      واپس 0;

    اگر ( !FT_IS_SFNT( face ) )
      واپس 0;

    ttface = (TT_Face)face;
    sfnt   = (SFNT_Service)ttface->sfnt;

    اگر ( sfnt->get_paint_layers )
      واپس sfnt->get_paint_layers( ttface, layer_iterator, paint );
    ورنہ
      واپس 0;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Get_Paint( FT_Face face,
                FT_OpaquePaint  opaque_paint,
                FT_COLR_Paint*  paint )
  {
    TT_Face       ttface;
    SFNT_Service  sfnt;


    اگر ( !face || !paint )
      واپس 0;

    اگر ( !FT_IS_SFNT( face ) )
      واپس 0;

    ttface = (TT_Face)face;
    sfnt   = (SFNT_Service)ttface->sfnt;

    اگر ( sfnt->get_paint )
      واپس sfnt->get_paint( ttface, opaque_paint, paint );
    ورنہ
      واپس 0;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Get_Colorline_Stops ( FT_Face                face,
                           FT_ColorStop *         color_stop,
                           FT_ColorStopIterator  *iterator )
  {
    TT_Face       ttface;
    SFNT_Service  sfnt;


    اگر ( !face || !color_stop || !iterator )
      واپس 0;

    اگر ( !FT_IS_SFNT( face ) )
      واپس 0;

    ttface = (TT_Face)face;
    sfnt   = (SFNT_Service)ttface->sfnt;

    اگر ( sfnt->get_colorline_stops )
      واپس sfnt->get_colorline_stops ( ttface, color_stop, iterator );
    ورنہ
      واپس 0;
  }


/*انتہا*/
