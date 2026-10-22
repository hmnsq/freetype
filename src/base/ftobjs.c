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


#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/ftlist.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/ftoutln.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/ftfntfmt.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/otsvg.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftvalid.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftobjs.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftdebug.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftrfork.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/ftstream.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/sfnt.h>          /* for SFNT_Load_Table_Func */
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/psaux.h>         /* for PS_Driver            */
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/svginterface.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/tttables.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/tttags.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/ttnameid.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/services/svprop.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/services/svsfnt.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/services/svpostnm.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/services/svgldict.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/services/svttcmap.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/services/svkern.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/services/svtteng.h>

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ TT_CONFIG_OPTION_VARC
#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/internal/services/svvarc.h>
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/ftdriver.h>

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ FT_CONFIG_OPTION_MAC_FONTS
#ਅਨ੍ਤਰ੍ਭਾਵ "ftbase.h"
#ਜੇ_ਅੰਤ


#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ FT_DEBUG_LEVEL_TRACE

#ਅਨ੍ਤਰ੍ਭਾਵ <freetype/ftbitmap.h>

#ਜੇ defined( _MSC_VER )      /* Visual C++ (and Intel C++)   */
  /* We disable the warning `conversion from XXX to YYY,     */
  /* possible loss of data' in order to compile cleanly with */
  /* the maximum level of warnings: `md5.c' is non-FreeType  */
  /* code, and it gets used during development builds only.  */
#ਹਦਾਇਤ warning( ਧੱਕੋ )
#ਹਦਾਇਤ warning( disable : 4244 )
#ਜੇ_ਅੰਤ /* _MSC_VER */

  /* It's easiest to include `md5.c' directly.  However, since OpenSSL */
  /* also provides the same functions, there might be conflicts if     */
  /* both FreeType and OpenSSL are built as static libraries.  For     */
  /* this reason, we put the MD5 stuff into the `FT_' namespace.       */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MD5_u32plus  FT_MD5_u32plus
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MD5_CTX      FT_MD5_CTX
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MD5_Init     FT_MD5_Init
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MD5_Update   FT_MD5_Update
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MD5_Final    FT_MD5_Final

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ  HAVE_OPENSSL

#ਅਨ੍ਤਰ੍ਭਾਵ "md5.c"

#ਜੇ defined( _MSC_VER )
#ਹਦਾਇਤ warning( ਕੱਢੋ )
#ਜੇ_ਅੰਤ

  /* This array must stay in sync with the @FT_Pixel_Mode enumeration */
  /* (in file `ftimage.h`).                                           */

  ਸਥਿਰ ਅਚਲ ਅੱਖਰ* ਅਚਲ  pixel_modes[] =
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

#ਜੇ_ਅੰਤ /* FT_DEBUG_LEVEL_TRACE */


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GRID_FIT_METRICS


  /* forward declaration */
  ਸਥਿਰ FT_Error
  ft_open_face_ਅੰਦਰੂਨੀ( FT_Library           library,
                         ਅਚਲ FT_Open_ਦਲੀਲਾਂ*  ਦਲੀਲਾਂ,
                         FT_Long              face_ਸੂਚਕਾਂਕ,
                         FT_Face             *aface,
                         FT_Bool              test_mac_fonts );


  FT_ਅਧਾਰ_DEF( FT_ਸੂਚਕ )
  ft_service_list_lookup( FT_ServiceDesc  service_descriptors,
                          ਅਚਲ ਅੱਖਰ*     service_id )
  {
    FT_ਸੂਚਕ      ਨਤੀਜਾ = ਨਿਰਾ;
    FT_ServiceDesc  desc   = service_descriptors;


    ਜੇ ( desc && service_id )
    {
      ਲਈ ( ; desc->serv_id != ਨਿਰਾ; desc++ )
      {
        ਜੇ ( ft_strcmp( desc->serv_id, service_id ) == 0 )
        {
          ਨਤੀਜਾ = (FT_ਸੂਚਕ)desc->serv_ਅੰਕੜੇ;
          ਰੋਕੋ;
        }
      }
    }

    ਵਾਪਸ ਨਤੀਜਾ;
  }


  FT_ਅਧਾਰ_DEF( ਖਾਲੀ )
  ft_validator_init( FT_Validator        valid,
                     ਅਚਲ FT_Byte*      ਅਧਾਰ,
                     ਅਚਲ FT_Byte*      ਹੱਦਬੰਦੀ,
                     FT_ValidationLevel  level )
  {
    valid->ਅਧਾਰ  = ਅਧਾਰ;
    valid->ਹੱਦਬੰਦੀ = ਹੱਦਬੰਦੀ;
    valid->level = level;
    valid->error = FT_Err_Ok;
  }


  FT_ਅਧਾਰ_DEF( FT_Int )
  ft_validator_ਲੜੀਵਾਰ( FT_Validator  valid )
  {
    /* This function doesn't work!  None should call it. */
    FT_ਅਣਵਰਤਿਆ( valid );

    ਵਾਪਸ -1;
  }


  FT_ਅਧਾਰ_DEF( ਖਾਲੀ )
  ft_validator_error( FT_Validator  valid,
                      FT_Error      error )
  {
    /* since the cast below also disables the compiler's */
    /* type check, we introduce a dummy variable, which  */
    /* will be optimized away                            */
    ਅਸਥਿਰ ft_jmp_buf* jump_ਬਫਰ = &valid->jump_ਬਫਰ;


    valid->error = error;

    /* throw away volatileness; use `jump_buffer' or the  */
    /* compiler may warn about an unused local variable   */
    ft_longjmp( *(ft_jmp_buf*) jump_ਬਫਰ, 1 );
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
  FT_ਅਧਾਰ_DEF( FT_Error )
  FT_ਧਾਰਾ_New( FT_Library           library,
                 ਅਚਲ FT_Open_ਦਲੀਲਾਂ*  ਦਲੀਲਾਂ,
                 FT_ਧਾਰਾ           *astream )
  {
    FT_Error   error;
    FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ;
    FT_ਧਾਰਾ  ਧਾਰਾ = ਨਿਰਾ;
    FT_UInt    ਬਹੁਲਕ;


    *astream = ਨਿਰਾ;

    ਜੇ ( !library )
      ਵਾਪਸ FT_THROW( Invalid_Library_Handle );

    ਜੇ ( !ਦਲੀਲਾਂ )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    ਸਮ੍ਰਿਤੀ = library->ਸਮ੍ਰਿਤੀ;
    ਬਹੁਲਕ   = ਦਲੀਲਾਂ->flags &
               ( FT_OPEN_ਸਮ੍ਰਿਤੀ | FT_OPEN_ਧਾਰਾ | FT_OPEN_PATHNAME );

    ਜੇ ( ਬਹੁਲਕ == FT_OPEN_ਸਮ੍ਰਿਤੀ )
    {
      /* create a memory-based stream */
      ਜੇ ( FT_NEW( ਧਾਰਾ ) )
        ਜਾਓ Exit;

      FT_ਧਾਰਾ_Open_ਸਮ੍ਰਿਤੀ( ਧਾਰਾ,
                            (ਅਚਲ FT_Byte*)ਦਲੀਲਾਂ->ਸਮ੍ਰਿਤੀ_ਅਧਾਰ,
                            (FT_ULong)ਦਲੀਲਾਂ->ਸਮ੍ਰਿਤੀ_ਮਾਪ );
      ਧਾਰਾ->ਸਮ੍ਰਿਤੀ = ਸਮ੍ਰਿਤੀ;
    }

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ FT_CONFIG_OPTION_DISABLE_ਧਾਰਾ_SUPPORT

    ਨਹੀਂਤਾਂ ਜੇ ( ਬਹੁਲਕ == FT_OPEN_PATHNAME )
    {
      /* create a normal system stream */
      ਜੇ ( FT_NEW( ਧਾਰਾ ) )
        ਜਾਓ Exit;

      ਧਾਰਾ->ਸਮ੍ਰਿਤੀ = ਸਮ੍ਰਿਤੀ;
      error = FT_ਧਾਰਾ_Open( ਧਾਰਾ, ਦਲੀਲਾਂ->pathname );
      ਜੇ ( error )
        FT_FREE( ਧਾਰਾ );
    }
    ਨਹੀਂਤਾਂ ਜੇ ( ( ਬਹੁਲਕ == FT_OPEN_ਧਾਰਾ ) && ਦਲੀਲਾਂ->ਧਾਰਾ )
    {
      /* use an existing, user-provided stream */

      /* in this case, we do not need to allocate a new stream object */
      /* since the caller is responsible for closing it himself       */
      ਧਾਰਾ         = ਦਲੀਲਾਂ->ਧਾਰਾ;
      ਧਾਰਾ->ਸਮ੍ਰਿਤੀ = ਸਮ੍ਰਿਤੀ;
      error          = FT_Err_Ok;
    }

#ਜੇ_ਅੰਤ

    ਨਹੀਂਤਾਂ
    {
      error = FT_THROW( Invalid_ਦਲੀਲ );
      ਜੇ ( ( ਦਲੀਲਾਂ->flags & FT_OPEN_ਧਾਰਾ ) && ਦਲੀਲਾਂ->ਧਾਰਾ )
        FT_ਧਾਰਾ_Close( ਦਲੀਲਾਂ->ਧਾਰਾ );
    }

    ਜੇ ( !error )
      *astream       = ਧਾਰਾ;

  Exit:
    ਵਾਪਸ error;
  }


  FT_ਅਧਾਰ_DEF( ਖਾਲੀ )
  FT_ਧਾਰਾ_Free( FT_ਧਾਰਾ  ਧਾਰਾ,
                  FT_Int     external )
  {
    ਜੇ ( ਧਾਰਾ )
    {
      FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ = ਧਾਰਾ->ਸਮ੍ਰਿਤੀ;


      FT_ਧਾਰਾ_Close( ਧਾਰਾ );

      ਜੇ ( !external )
        FT_FREE( ਧਾਰਾ );
    }
  }


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ  FT_COMPONENT
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FT_COMPONENT  objs


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


  ਸਥਿਰ FT_Error
  ft_glyphslot_init( FT_Glyph_ਥਾਂ  ਥਾਂ )
  {
    FT_Driver         driver   = ਥਾਂ->face->driver;
    FT_Driver_Class   clazz    = driver->clazz;
    FT_ਸਮ੍ਰਿਤੀ         ਸਮ੍ਰਿਤੀ   = driver->ਜੜ੍ਹ.ਸਮ੍ਰਿਤੀ;
    FT_Error          error    = FT_Err_Ok;
    FT_ਥਾਂ_ਅੰਦਰੂਨੀ  ਅੰਦਰੂਨੀ = ਨਿਰਾ;


    ਥਾਂ->library = driver->ਜੜ੍ਹ.library;

    ਜੇ ( FT_NEW( ਅੰਦਰੂਨੀ ) )
      ਜਾਓ Exit;

    ਥਾਂ->ਅੰਦਰੂਨੀ = ਅੰਦਰੂਨੀ;

    ਜੇ ( FT_DRIVER_ਵਰਤਦਾ_OUTLINES( driver ) )
      error = FT_GlyphLoader_New( ਸਮ੍ਰਿਤੀ, &ਅੰਦਰੂਨੀ->loader );

    ਜੇ ( !error && clazz->init_ਥਾਂ )
      error = clazz->init_ਥਾਂ( ਥਾਂ );

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ FT_CONFIG_OPTION_SVG
    /* if SVG table exists, allocate the space in `slot->other` */
    ਜੇ ( ਥਾਂ->face->face_flags & FT_FACE_FLAG_SVG )
    {
      FT_SVG_Document  document = ਨਿਰਾ;


      ਜੇ ( FT_NEW( document ) )
        ਜਾਓ Exit;
      ਥਾਂ->other = document;
    }
#ਜੇ_ਅੰਤ

  Exit:
    ਵਾਪਸ error;
  }


  FT_ਅਧਾਰ_DEF( ਖਾਲੀ )
  ft_glyphslot_free_bitmap( FT_Glyph_ਥਾਂ  ਥਾਂ )
  {
    ਜੇ ( ਥਾਂ->ਅੰਦਰੂਨੀ && ( ਥਾਂ->ਅੰਦਰੂਨੀ->flags & FT_GLYPH_OWN_BITMAP ) )
    {
      FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ = FT_FACE_ਸਮ੍ਰਿਤੀ( ਥਾਂ->face );


      FT_FREE( ਥਾਂ->bitmap.ਬਫਰ );
      ਥਾਂ->ਅੰਦਰੂਨੀ->flags &= ~FT_GLYPH_OWN_BITMAP;
    }
    ਨਹੀਂਤਾਂ
    {
      /* assume that the bitmap buffer was stolen or not */
      /* allocated from the heap                         */
      ਥਾਂ->bitmap.ਬਫਰ = ਨਿਰਾ;
    }
  }


  /* overflow-resistant presetting of bitmap position and dimensions; */
  /* also check whether the size is too large for rendering           */
  FT_ਅਧਾਰ_DEF( FT_Bool )
  ft_glyphslot_preset_bitmap( FT_Glyph_ਥਾਂ      ਥਾਂ,
                              FT_Render_ਬਹੁਲਕ    ਬਹੁਲਕ,
                              ਅਚਲ FT_ਸਦਿਸ਼*  origin )
  {
    FT_Outline*  outline = &ਥਾਂ->outline;
    FT_Bitmap*   bitmap  = &ਥਾਂ->bitmap;

    FT_Pixel_ਬਹੁਲਕ  pixel_ਬਹੁਲਕ;

    FT_BBox  cbox, pbox;
    FT_Pos   x_ਖਿਸਕਾਓ = 0;
    FT_Pos   y_ਖਿਸਕਾਓ = 0;
    FT_Pos   x_ਖੱਬਾ, y_ਸਿਖਰ;
    FT_Pos   ਚੌੜਾਈ, ਉਚਾਈ, pitch;
    FT_Bool  ret;


    ਜੇ ( ਥਾਂ->format == FT_GLYPH_FORMAT_SVG )
    {
      FT_Module    module;
      SVG_Service  svg_service;


      module      = FT_Get_Module( ਥਾਂ->library, "ot-svg" );
      svg_service = (SVG_Service)module->clazz->module_interface;

      ਵਾਪਸ (FT_Bool)svg_service->preset_ਥਾਂ( module, ਥਾਂ, FALSE );
    }
    ਨਹੀਂਤਾਂ ਜੇ ( ਥਾਂ->format != FT_GLYPH_FORMAT_OUTLINE )
      ਵਾਪਸ 1;

    ਜੇ ( origin )
    {
      x_ਖਿਸਕਾਓ = origin->x;
      y_ਖਿਸਕਾਓ = origin->y;
    }

    /* compute the control box, and grid-fit it, */
    /* taking into account the origin shift      */
    FT_Outline_Get_CBox( outline, &cbox );

    /* rough estimate of pixel box */
    pbox.xMin = ( cbox.xMin >> 6 ) + ( x_ਖਿਸਕਾਓ >> 6 );
    pbox.yMin = ( cbox.yMin >> 6 ) + ( y_ਖਿਸਕਾਓ >> 6 );
    pbox.xMax = ( cbox.xMax >> 6 ) + ( x_ਖਿਸਕਾਓ >> 6 );
    pbox.yMax = ( cbox.yMax >> 6 ) + ( y_ਖਿਸਕਾਓ >> 6 );

    /* tiny remainder box */
    cbox.xMin = ( cbox.xMin & 63 ) + ( x_ਖਿਸਕਾਓ & 63 );
    cbox.yMin = ( cbox.yMin & 63 ) + ( y_ਖਿਸਕਾਓ & 63 );
    cbox.xMax = ( cbox.xMax & 63 ) + ( x_ਖਿਸਕਾਓ & 63 );
    cbox.yMax = ( cbox.yMax & 63 ) + ( y_ਖਿਸਕਾਓ & 63 );

    ਚੋਣ ( ਬਹੁਲਕ )
    {
    ਹਾਲਤ FT_RENDER_ਬਹੁਲਕ_MONO:
      pixel_ਬਹੁਲਕ = FT_PIXEL_ਬਹੁਲਕ_MONO;
#ਜੇ 1
      /* x */

      /* undocumented but confirmed: bbox values get rounded;    */
      /* we do asymmetric rounding so that the center of a pixel */
      /* gets always included                                    */

      pbox.xMin += ( cbox.xMin + 31 ) >> 6;
      pbox.xMax += ( cbox.xMax + 32 ) >> 6;

      /* if the bbox collapsed, we add a pixel based on the total */
      /* rounding remainder to cover most of the original cbox    */

      ਜੇ ( pbox.xMin == pbox.xMax )
      {
        ਜੇ ( ( ( cbox.xMin + 31 ) & 63 ) - 31 +
             ( ( cbox.xMax + 32 ) & 63 ) - 32 < 0 )
          pbox.xMin -= 1;
        ਨਹੀਂਤਾਂ
          pbox.xMax += 1;
      }

      /* y */

      pbox.yMin += ( cbox.yMin + 31 ) >> 6;
      pbox.yMax += ( cbox.yMax + 32 ) >> 6;

      ਜੇ ( pbox.yMin == pbox.yMax )
      {
        ਜੇ ( ( ( cbox.yMin + 31 ) & 63 ) - 31 +
             ( ( cbox.yMax + 32 ) & 63 ) - 32 < 0 )
          pbox.yMin -= 1;
        ਨਹੀਂਤਾਂ
          pbox.yMax += 1;
      }

      ਰੋਕੋ;
#ਨਹੀਂਤਾਂ
      ਜਾਓ Adjust;
#ਜੇ_ਅੰਤ

    ਹਾਲਤ FT_RENDER_ਬਹੁਲਕ_LCD:
      pixel_ਬਹੁਲਕ = FT_PIXEL_ਬਹੁਲਕ_LCD;
      ft_lcd_padding( &cbox, ਥਾਂ, ਬਹੁਲਕ );
      ਜਾਓ Adjust;

    ਹਾਲਤ FT_RENDER_ਬਹੁਲਕ_LCD_V:
      pixel_ਬਹੁਲਕ = FT_PIXEL_ਬਹੁਲਕ_LCD_V;
      ft_lcd_padding( &cbox, ਥਾਂ, ਬਹੁਲਕ );
      ਜਾਓ Adjust;

    ਹਾਲਤ FT_RENDER_ਬਹੁਲਕ_ਸਾਧਾਰਨ:
    ਹਾਲਤ FT_RENDER_ਬਹੁਲਕ_LIGHT:
    ਮੂਲ:
      pixel_ਬਹੁਲਕ = FT_PIXEL_ਬਹੁਲਕ_ਸਲੇਟੀ;
    Adjust:
      pbox.xMin += cbox.xMin >> 6;
      pbox.yMin += cbox.yMin >> 6;
      pbox.xMax += ( cbox.xMax + 63 ) >> 6;
      pbox.yMax += ( cbox.yMax + 63 ) >> 6;
    }

    x_ਖੱਬਾ = pbox.xMin;
    y_ਸਿਖਰ  = pbox.yMax;

    ਚੌੜਾਈ  = pbox.xMax - pbox.xMin;
    ਉਚਾਈ = pbox.yMax - pbox.yMin;

    /* Flag the width or height unsuitable for rendering.   */
    /* The limit is based on the ppem value when available. */
    /* FT_Renderer modules should check the return value.   */
    ret = FT_BOOL(     ਚੌੜਾਈ >= 0x10000   ||    ਉਚਾਈ >= 0x10000   ||
                   pbox.xMin < -0x1000000 || pbox.xMax >= 0x1000000 ||
                   pbox.yMin < -0x1000000 || pbox.yMax >= 0x1000000 ||
             ( ਥਾਂ->face                                         &&
               ( ਚੌੜਾਈ  > 16 * ਥਾਂ->face->ਮਾਪ->metrics.x_ppem ||
                 ਉਚਾਈ > 16 * ਥਾਂ->face->ਮਾਪ->metrics.y_ppem ) ) );

    ਜੇ ( ret )
      FT_TRACE3(( "ft_glyphslot_preset_bitmap: [%ld %ld %ld %ld]\n",
                  pbox.xMin, pbox.yMin, pbox.xMax, pbox.yMax ));

    ਚੋਣ ( pixel_ਬਹੁਲਕ )
    {
    ਹਾਲਤ FT_PIXEL_ਬਹੁਲਕ_MONO:
      pitch = ( ( ਚੌੜਾਈ + 15 ) >> 4 ) << 1;
      ਰੋਕੋ;

    ਹਾਲਤ FT_PIXEL_ਬਹੁਲਕ_LCD:
      ਚੌੜਾਈ *= 3;
      pitch  = FT_PAD_CEIL( ਚੌੜਾਈ, 4 );
      ਰੋਕੋ;

    ਹਾਲਤ FT_PIXEL_ਬਹੁਲਕ_LCD_V:
      ਉਚਾਈ *= 3;
      FALL_THROUGH;

    ਹਾਲਤ FT_PIXEL_ਬਹੁਲਕ_ਸਲੇਟੀ:
    ਮੂਲ:
      pitch = ਚੌੜਾਈ;
    }

    ਥਾਂ->bitmap_ਖੱਬਾ = (FT_Int)x_ਖੱਬਾ;
    ਥਾਂ->bitmap_ਸਿਖਰ  = (FT_Int)y_ਸਿਖਰ;

    bitmap->pixel_ਬਹੁਲਕ = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ)pixel_ਬਹੁਲਕ;
    bitmap->ਅੰਕ_grays  = 256;
    bitmap->ਚੌੜਾਈ      = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)ਚੌੜਾਈ;
    bitmap->ਪੰਕਤੀਆਂ       = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)ਉਚਾਈ;
    bitmap->pitch      = pitch;

    ਵਾਪਸ ret;
  }


  FT_ਅਧਾਰ_DEF( ਖਾਲੀ )
  ft_glyphslot_ਸੈੱਟ_bitmap( FT_Glyph_ਥਾਂ  ਥਾਂ,
                           FT_Byte*      ਬਫਰ )
  {
    ft_glyphslot_free_bitmap( ਥਾਂ );

    ਥਾਂ->bitmap.ਬਫਰ = ਬਫਰ;

    FT_ASSERT( (ਥਾਂ->ਅੰਦਰੂਨੀ->flags & FT_GLYPH_OWN_BITMAP) == 0 );
  }


  FT_ਅਧਾਰ_DEF( FT_Error )
  ft_glyphslot_alloc_bitmap( FT_Glyph_ਥਾਂ  ਥਾਂ )
  {
    FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ = FT_FACE_ਸਮ੍ਰਿਤੀ( ਥਾਂ->face );
    FT_Error   error;


    ਜੇ ( ਥਾਂ->ਅੰਦਰੂਨੀ->flags & FT_GLYPH_OWN_BITMAP )
      FT_FREE( ਥਾਂ->bitmap.ਬਫਰ );
    ਨਹੀਂਤਾਂ
      ਥਾਂ->ਅੰਦਰੂਨੀ->flags |= FT_GLYPH_OWN_BITMAP;

    /* dimensions must be preset */
    FT_MEM_ALLOC_MULT( ਥਾਂ->bitmap.ਬਫਰ,
                       ਥਾਂ->bitmap.ਪੰਕਤੀਆਂ,
                       ਥਾਂ->bitmap.pitch );
    ਵਾਪਸ error;
  }


  ਸਥਿਰ ਖਾਲੀ
  ft_glyphslot_clear( FT_Glyph_ਥਾਂ  ਥਾਂ )
  {
    /* free bitmap if needed */
    ft_glyphslot_free_bitmap( ਥਾਂ );

    /* clear all public fields in the glyph slot */
    ਥਾਂ->glyph_ਸੂਚਕਾਂਕ = 0;

    FT_ZERO( &ਥਾਂ->metrics );
    FT_ZERO( &ਥਾਂ->outline );

    ਥਾਂ->bitmap.ਚੌੜਾਈ      = 0;
    ਥਾਂ->bitmap.ਪੰਕਤੀਆਂ       = 0;
    ਥਾਂ->bitmap.pitch      = 0;
    ਥਾਂ->bitmap.pixel_ਬਹੁਲਕ = 0;
    /* `slot->bitmap.buffer' has been handled by ft_glyphslot_free_bitmap */

    ਥਾਂ->bitmap_ਖੱਬਾ   = 0;
    ਥਾਂ->bitmap_ਸਿਖਰ    = 0;
    ਥਾਂ->ਅੰਕ_subglyphs = 0;
    ਥਾਂ->subglyphs     = ਨਿਰਾ;
    ਥਾਂ->control_ਅੰਕੜੇ  = ਨਿਰਾ;
    ਥਾਂ->control_len   = 0;

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ FT_CONFIG_OPTION_SVG
    ਥਾਂ->other = ਨਿਰਾ;
#ਨਹੀਂਤਾਂ
    ਜੇ ( !( ਥਾਂ->face->face_flags & FT_FACE_FLAG_SVG ) )
      ਥਾਂ->other = ਨਿਰਾ;
    ਨਹੀਂਤਾਂ
    {
      ਜੇ ( ਥਾਂ->ਅੰਦਰੂਨੀ->flags & FT_GLYPH_OWN_GZIP_SVG )
      {
        FT_ਸਮ੍ਰਿਤੀ        ਸਮ੍ਰਿਤੀ = ਥਾਂ->face->ਸਮ੍ਰਿਤੀ;
        FT_SVG_Document  doc    = (FT_SVG_Document)ਥਾਂ->other;


        FT_FREE( doc->svg_document );
        ਥਾਂ->ਅੰਦਰੂਨੀ->flags &= ~FT_GLYPH_OWN_GZIP_SVG;
      }
    }
#ਜੇ_ਅੰਤ

    ਥਾਂ->format = FT_GLYPH_FORMAT_NONE;

    ਥਾਂ->ਰੇਖਿਕ_Hori_Advance = 0;
    ਥਾਂ->ਰੇਖਿਕ_Vert_Advance = 0;
    ਥਾਂ->advance.x         = 0;
    ਥਾਂ->advance.y         = 0;
    ਥਾਂ->lsb_delta         = 0;
    ਥਾਂ->rsb_delta         = 0;
  }


  ਸਥਿਰ ਖਾਲੀ
  ft_glyphslot_ਪੂਰਾ( FT_Glyph_ਥਾਂ  ਥਾਂ )
  {
    FT_Driver        driver = ਥਾਂ->face->driver;
    FT_Driver_Class  clazz  = driver->clazz;
    FT_ਸਮ੍ਰਿਤੀ        ਸਮ੍ਰਿਤੀ = driver->ਜੜ੍ਹ.ਸਮ੍ਰਿਤੀ;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ FT_CONFIG_OPTION_SVG
    ਜੇ ( ਥਾਂ->face->face_flags & FT_FACE_FLAG_SVG )
    {
      /* Free memory in case SVG was there.                          */
      /* `slot->internal` might be NULL in out-of-memory situations. */
      ਜੇ ( ਥਾਂ->ਅੰਦਰੂਨੀ && ਥਾਂ->ਅੰਦਰੂਨੀ->flags & FT_GLYPH_OWN_GZIP_SVG )
      {
        FT_SVG_Document  doc = (FT_SVG_Document)ਥਾਂ->other;


        FT_FREE( doc->svg_document );

        ਥਾਂ->ਅੰਦਰੂਨੀ->flags &= ~FT_GLYPH_OWN_GZIP_SVG;
      }

      FT_FREE( ਥਾਂ->other );
    }
#ਜੇ_ਅੰਤ

    ਜੇ ( clazz->ਪੂਰਾ_ਥਾਂ )
      clazz->ਪੂਰਾ_ਥਾਂ( ਥਾਂ );

    /* free bitmap buffer if needed */
    ft_glyphslot_free_bitmap( ਥਾਂ );

    /* slot->internal might be NULL in out-of-memory situations */
    ਜੇ ( ਥਾਂ->ਅੰਦਰੂਨੀ )
    {
      /* free glyph loader */
      ਜੇ ( FT_DRIVER_ਵਰਤਦਾ_OUTLINES( driver ) )
      {
        FT_Glyph_Loader_ਪੂਰਾ( ਥਾਂ->ਅੰਦਰੂਨੀ->loader );
        ਥਾਂ->ਅੰਦਰੂਨੀ->loader = ਨਿਰਾ;
      }

      FT_FREE( ਥਾਂ->ਅੰਦਰੂਨੀ );
    }
  }


  /* documentation is in ftobjs.h */

  FT_ਅਧਾਰ_DEF( FT_Error )
  FT_New_Glyph_ਥਾਂ( FT_Face        face,
                    FT_Glyph_ਥਾਂ  *aslot )
  {
    FT_Error         error;
    FT_Driver        driver;
    FT_Driver_Class  clazz;
    FT_ਸਮ੍ਰਿਤੀ        ਸਮ੍ਰਿਤੀ;
    FT_Glyph_ਥਾਂ     ਥਾਂ = ਨਿਰਾ;


    ਜੇ ( !face )
      ਵਾਪਸ FT_THROW( Invalid_Face_Handle );

    ਜੇ ( !face->driver )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    driver = face->driver;
    clazz  = driver->clazz;
    ਸਮ੍ਰਿਤੀ = driver->ਜੜ੍ਹ.ਸਮ੍ਰਿਤੀ;

    FT_TRACE4(( "FT_New_GlyphSlot: Creating new slot object\n" ));
    ਜੇ ( !FT_ALLOC( ਥਾਂ, clazz->ਥਾਂ_object_ਮਾਪ ) )
    {
      ਥਾਂ->face = face;

      error = ft_glyphslot_init( ਥਾਂ );
      ਜੇ ( error )
      {
        ft_glyphslot_ਪੂਰਾ( ਥਾਂ );
        FT_FREE( ਥਾਂ );
        ਜਾਓ Exit;
      }

      ਥਾਂ->ਅਗਲਾ  = face->glyph;
      face->glyph = ਥਾਂ;

      ਜੇ ( aslot )
        *aslot = ਥਾਂ;
    }
    ਨਹੀਂਤਾਂ ਜੇ ( aslot )
      *aslot = ਨਿਰਾ;


  Exit:
    FT_TRACE4(( "FT_New_GlyphSlot: Return 0x%x\n", error ));

    ਵਾਪਸ error;
  }


  /* documentation is in ftobjs.h */

  FT_ਅਧਾਰ_DEF( ਖਾਲੀ )
  FT_ਪੂਰਾ_Glyph_ਥਾਂ( FT_Glyph_ਥਾਂ  ਥਾਂ )
  {
    ਜੇ ( ਥਾਂ )
    {
      FT_Driver     driver = ਥਾਂ->face->driver;
      FT_ਸਮ੍ਰਿਤੀ     ਸਮ੍ਰਿਤੀ = driver->ਜੜ੍ਹ.ਸਮ੍ਰਿਤੀ;
      FT_Glyph_ਥਾਂ  prev;
      FT_Glyph_ਥਾਂ  cur;


      /* Remove slot from its parent face's list */
      prev = ਨਿਰਾ;
      cur  = ਥਾਂ->face->glyph;

      ਜਦੋਂ ( cur )
      {
        ਜੇ ( cur == ਥਾਂ )
        {
          ਜੇ ( !prev )
            ਥਾਂ->face->glyph = cur->ਅਗਲਾ;
          ਨਹੀਂਤਾਂ
            prev->ਅਗਲਾ = cur->ਅਗਲਾ;

          /* finalize client-specific data */
          ਜੇ ( ਥਾਂ->generic.finalizer )
            ਥਾਂ->generic.finalizer( ਥਾਂ );

          ft_glyphslot_ਪੂਰਾ( ਥਾਂ );
          FT_FREE( ਥਾਂ );
          ਰੋਕੋ;
        }
        prev = cur;
        cur  = cur->ਅਗਲਾ;
      }
    }
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( ਖਾਲੀ )
  FT_ਸੈੱਟ_Transform( FT_Face     face,
                    FT_ਮੈਟ੍ਰਿਕਸ*  ਮੈਟ੍ਰਿਕਸ,
                    FT_ਸਦਿਸ਼*  delta )
  {
    FT_Face_ਅੰਦਰੂਨੀ  ਅੰਦਰੂਨੀ;


    ਜੇ ( !face )
      ਵਾਪਸ;

    ਅੰਦਰੂਨੀ = face->ਅੰਦਰੂਨੀ;

    ਅੰਦਰੂਨੀ->transform_flags = 0;

    ਜੇ ( !ਮੈਟ੍ਰਿਕਸ || !FT_ਮੈਟ੍ਰਿਕਸ_Check( ਮੈਟ੍ਰਿਕਸ ) )
    {
      ਅੰਦਰੂਨੀ->transform_ਮੈਟ੍ਰਿਕਸ.xx = 0x10000L;
      ਅੰਦਰੂਨੀ->transform_ਮੈਟ੍ਰਿਕਸ.xy = 0;
      ਅੰਦਰੂਨੀ->transform_ਮੈਟ੍ਰਿਕਸ.yx = 0;
      ਅੰਦਰੂਨੀ->transform_ਮੈਟ੍ਰਿਕਸ.yy = 0x10000L;

      ਮੈਟ੍ਰਿਕਸ = &ਅੰਦਰੂਨੀ->transform_ਮੈਟ੍ਰਿਕਸ;
    }
    ਨਹੀਂਤਾਂ
      ਅੰਦਰੂਨੀ->transform_ਮੈਟ੍ਰਿਕਸ = *ਮੈਟ੍ਰਿਕਸ;

    /* set transform_flags bit flag 0 if `matrix' isn't the identity */
    ਜੇ ( ( ਮੈਟ੍ਰਿਕਸ->xy | ਮੈਟ੍ਰਿਕਸ->yx ) ||
         ਮੈਟ੍ਰਿਕਸ->xx != 0x10000L      ||
         ਮੈਟ੍ਰਿਕਸ->yy != 0x10000L      )
      ਅੰਦਰੂਨੀ->transform_flags |= 1;

    ਜੇ ( !delta )
    {
      ਅੰਦਰੂਨੀ->transform_delta.x = 0;
      ਅੰਦਰੂਨੀ->transform_delta.y = 0;

      delta = &ਅੰਦਰੂਨੀ->transform_delta;
    }
    ਨਹੀਂਤਾਂ
      ਅੰਦਰੂਨੀ->transform_delta = *delta;

    /* set transform_flags bit flag 1 if `delta' isn't the null vector */
    ਜੇ ( delta->x | delta->y )
      ਅੰਦਰੂਨੀ->transform_flags |= 2;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( ਖਾਲੀ )
  FT_Get_Transform( FT_Face     face,
                    FT_ਮੈਟ੍ਰਿਕਸ*  ਮੈਟ੍ਰਿਕਸ,
                    FT_ਸਦਿਸ਼*  delta )
  {
    FT_Face_ਅੰਦਰੂਨੀ  ਅੰਦਰੂਨੀ;


    ਜੇ ( !face )
      ਵਾਪਸ;

    ਅੰਦਰੂਨੀ = face->ਅੰਦਰੂਨੀ;

    ਜੇ ( ਮੈਟ੍ਰਿਕਸ )
      *ਮੈਟ੍ਰਿਕਸ = ਅੰਦਰੂਨੀ->transform_ਮੈਟ੍ਰਿਕਸ;

    ਜੇ ( delta )
      *delta = ਅੰਦਰੂਨੀ->transform_delta;
  }


  ਸਥਿਰ FT_Renderer
  ft_lookup_glyph_renderer( FT_Glyph_ਥਾਂ  ਥਾਂ );


#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GRID_FIT_METRICS
  ਸਥਿਰ ਖਾਲੀ
  ft_glyphslot_grid_fit_metrics( FT_Glyph_ਥਾਂ  ਥਾਂ,
                                 FT_Bool       vertical )
  {
    FT_Glyph_Metrics*  metrics = &ਥਾਂ->metrics;
    FT_Pos             ਸੱਜਾ, bottom;


    ਜੇ ( vertical )
    {
      metrics->horiBearingX = FT_PIX_FLOOR( metrics->horiBearingX );
      metrics->horiBearingY = FT_PIX_CEIL_LONG( metrics->horiBearingY );

      ਸੱਜਾ  = FT_PIX_CEIL_LONG( ਜੋੜੋ_LONG( metrics->vertBearingX,
                                           metrics->ਚੌੜਾਈ ) );
      bottom = FT_PIX_CEIL_LONG( ਜੋੜੋ_LONG( metrics->vertBearingY,
                                           metrics->ਉਚਾਈ ) );

      metrics->vertBearingX = FT_PIX_FLOOR( metrics->vertBearingX );
      metrics->vertBearingY = FT_PIX_FLOOR( metrics->vertBearingY );

      metrics->ਚੌੜਾਈ  = SUB_LONG( ਸੱਜਾ,
                                  metrics->vertBearingX );
      metrics->ਉਚਾਈ = SUB_LONG( bottom,
                                  metrics->vertBearingY );
    }
    ਨਹੀਂਤਾਂ
    {
      metrics->vertBearingX = FT_PIX_FLOOR( metrics->vertBearingX );
      metrics->vertBearingY = FT_PIX_FLOOR( metrics->vertBearingY );

      ਸੱਜਾ  = FT_PIX_CEIL_LONG( ਜੋੜੋ_LONG( metrics->horiBearingX,
                                           metrics->ਚੌੜਾਈ ) );
      bottom = FT_PIX_FLOOR( SUB_LONG( metrics->horiBearingY,
                                       metrics->ਉਚਾਈ ) );

      metrics->horiBearingX = FT_PIX_FLOOR( metrics->horiBearingX );
      metrics->horiBearingY = FT_PIX_CEIL_LONG( metrics->horiBearingY );

      metrics->ਚੌੜਾਈ  = SUB_LONG( ਸੱਜਾ,
                                  metrics->horiBearingX );
      metrics->ਉਚਾਈ = SUB_LONG( metrics->horiBearingY,
                                  bottom );
    }

    metrics->horiAdvance = FT_PIX_ਗੇੜਾ_LONG( metrics->horiAdvance );
    metrics->vertAdvance = FT_PIX_ਗੇੜਾ_LONG( metrics->vertAdvance );
  }
#ਜੇ_ਅੰਤ /* GRID_FIT_METRICS */


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_ਲੱਦੋ_Glyph( FT_Face   face,
                 FT_UInt   glyph_ਸੂਚਕਾਂਕ,
                 FT_Int32  ਲੱਦੋ_flags )
  {
    FT_Error      error;
    FT_Driver     driver;
    FT_Glyph_ਥਾਂ  ਥਾਂ;
    FT_Library    library;
    FT_Bool       autohint = FALSE;
    FT_Module     hinter;


    ਜੇ ( !face || !face->ਮਾਪ || !face->glyph )
      ਵਾਪਸ FT_THROW( Invalid_Face_Handle );

    /* The validity test for `glyph_index' is performed by the */
    /* font drivers.                                           */

    ਥਾਂ = face->glyph;
    ft_glyphslot_clear( ਥਾਂ );

    driver  = face->driver;
    library = driver->ਜੜ੍ਹ.library;
    hinter  = library->auto_hinter;

    /* undefined scale means no scale */
    ਜੇ ( face->ਮਾਪ->metrics.x_ppem == 0 ||
         face->ਮਾਪ->metrics.y_ppem == 0 )
      ਲੱਦੋ_flags |= FT_ਲੱਦੋ_NO_SCALE;

    /* resolve load flags dependencies */

    ਜੇ ( ਲੱਦੋ_flags & FT_ਲੱਦੋ_NO_RECURSE )
      ਲੱਦੋ_flags |= FT_ਲੱਦੋ_NO_SCALE         |
                    FT_ਲੱਦੋ_IGNORE_TRANSFORM;

    ਜੇ ( ਲੱਦੋ_flags & FT_ਲੱਦੋ_NO_SCALE )
    {
      ਲੱਦੋ_flags |= FT_ਲੱਦੋ_NO_HINTING |
                    FT_ਲੱਦੋ_NO_BITMAP;

      ਲੱਦੋ_flags &= ~FT_ਲੱਦੋ_RENDER;
    }

    ਜੇ ( ਲੱਦੋ_flags & FT_ਲੱਦੋ_BITMAP_METRICS_ONLY )
      ਲੱਦੋ_flags &= ~FT_ਲੱਦੋ_RENDER;

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

    ਜੇ ( hinter                                           &&
         !( ਲੱਦੋ_flags & FT_ਲੱਦੋ_NO_HINTING )             &&
         !( ਲੱਦੋ_flags & FT_ਲੱਦੋ_NO_AUTOHINT )            &&
         FT_IS_SCALABLE( face )                           &&
         !FT_IS_TRICKY( face )                            &&
         ( ( ਲੱਦੋ_flags & FT_ਲੱਦੋ_IGNORE_TRANSFORM )    ||
           ( face->ਅੰਦਰੂਨੀ->transform_ਮੈਟ੍ਰਿਕਸ.yx == 0 &&
             face->ਅੰਦਰੂਨੀ->transform_ਮੈਟ੍ਰਿਕਸ.xx != 0 ) ||
           ( face->ਅੰਦਰੂਨੀ->transform_ਮੈਟ੍ਰਿਕਸ.xx == 0 &&
             face->ਅੰਦਰੂਨੀ->transform_ਮੈਟ੍ਰਿਕਸ.yx != 0 ) ) )
    {
      ਜੇ ( ( ਲੱਦੋ_flags & FT_ਲੱਦੋ_FORCE_AUTOHINT ) ||
           !FT_DRIVER_HAS_HINTER( driver )         )
        autohint = TRUE;
      ਨਹੀਂਤਾਂ
      {
        FT_Render_ਬਹੁਲਕ  ਬਹੁਲਕ = FT_ਲੱਦੋ_ਨਿਸ਼ਾਨਾ_ਬਹੁਲਕ( ਲੱਦੋ_flags );
        FT_Bool         is_light_type1;
        TT_Face         ttface = (TT_Face)face;


        /* only the new Adobe engine (for both CFF and Type 1) is `light'; */
        /* we use `strstr' to catch both `Type 1' and `CID Type 1'         */
        is_light_type1 =
          ft_strstr( FT_Get_Font_Format( face ), "Type 1" ) != ਨਿਰਾ &&
          ((PS_Driver)driver)->hinting_engine == FT_HINTING_ADOBE;

        /* the check for `num_locations' assures that we actually    */
        /* test for instructions in a TTF and not in a CFF-based OTF */
        /*                                                           */
        /* we check the size of the `fpgm' and `prep' tables, too -- */
        /* the assumption is that there don't exist real TTFs where  */
        /* both `fpgm' and `prep' tables are missing                 */
        ਜੇ ( ( ਬਹੁਲਕ == FT_RENDER_ਬਹੁਲਕ_LIGHT           &&
               ( !FT_DRIVER_HINTS_LIGHTLY( driver ) &&
                 !is_light_type1                    ) )         ||
             ( FT_IS_SFNT( face )                             &&
               ttface->ਅੰਕ_locations                          &&
               ttface->font_ਪ੍ਰੋਗਰਾਮ_ਮਾਪ == 0                 &&
               ttface->cvt_ਪ੍ਰੋਗਰਾਮ_ਮਾਪ <= 7                  ) )
          autohint = TRUE;
      }
    }

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ TT_CONFIG_OPTION_VARC
    /* Never auto-hint a VARC (variable composite) glyph: its components */
    /* are already loaded unhinted, and the auto-fitter cannot process   */
    /* the assembled composite (it also re-enters variation-coordinate   */
    /* setting).  Use the native hinter / unhinted load instead.         */
    ਜੇ ( autohint && FT_IS_SFNT( face ) )
    {
      FT_Service_VARC  varc;


      FT_FACE_ਲੱਭੋ_SERVICE( face, varc, VARC );
      ਜੇ ( varc && varc->has_glyph( face, glyph_ਸੂਚਕਾਂਕ ) )
        autohint = FALSE;
    }
#ਜੇ_ਅੰਤ

    ਜੇ ( autohint )
    {
      FT_AutoHinter_Interface  hinting;


      /* XXX: The use of the `FT_LOAD_XXX_ONLY` flags is not very */
      /*      elegant.                                            */

      /* try to load SVG documents if available */
      ਜੇ ( ( ਲੱਦੋ_flags & FT_ਲੱਦੋ_NO_SVG ) == 0 &&
           FT_HAS_SVG( face )                   )
      {
        error = driver->clazz->ਲੱਦੋ_glyph( ਥਾਂ, face->ਮਾਪ,
                                           glyph_ਸੂਚਕਾਂਕ,
                                           ਲੱਦੋ_flags | FT_ਲੱਦੋ_SVG_ONLY );

        ਜੇ ( !error && ਥਾਂ->format == FT_GLYPH_FORMAT_SVG )
          ਜਾਓ ਲੱਦੋ_Ok;
      }

      /* try to load embedded bitmaps if available */
      ਜੇ ( FT_HAS_FIXED_SIZES( face )              &&
           ( ਲੱਦੋ_flags & FT_ਲੱਦੋ_NO_BITMAP ) == 0 )
      {
        error = driver->clazz->ਲੱਦੋ_glyph( ਥਾਂ, face->ਮਾਪ,
                                           glyph_ਸੂਚਕਾਂਕ,
                                           ਲੱਦੋ_flags | FT_ਲੱਦੋ_SBITS_ONLY );

        ਜੇ ( !error && ਥਾਂ->format == FT_GLYPH_FORMAT_BITMAP )
          ਜਾਓ ਲੱਦੋ_Ok;
      }

      {
        FT_Face_ਅੰਦਰੂਨੀ  ਅੰਦਰੂਨੀ        = face->ਅੰਦਰੂਨੀ;
        FT_Int            transform_flags = ਅੰਦਰੂਨੀ->transform_flags;


        /* since the auto-hinter calls FT_Load_Glyph by itself, */
        /* make sure that glyphs aren't transformed             */
        ਅੰਦਰੂਨੀ->transform_flags = 0;

        /* load auto-hinted outline */
        hinting = (FT_AutoHinter_Interface)hinter->clazz->module_interface;

        error   = hinting->ਲੱਦੋ_glyph( (FT_AutoHinter)hinter,
                                       ਥਾਂ, face->ਮਾਪ,
                                       glyph_ਸੂਚਕਾਂਕ, ਲੱਦੋ_flags );

        ਅੰਦਰੂਨੀ->transform_flags = transform_flags;
      }
    }
    ਨਹੀਂਤਾਂ
    {
      error = driver->clazz->ਲੱਦੋ_glyph( ਥਾਂ,
                                         face->ਮਾਪ,
                                         glyph_ਸੂਚਕਾਂਕ,
                                         ਲੱਦੋ_flags );
      ਜੇ ( error )
        ਜਾਓ Exit;

      ਜੇ ( ਥਾਂ->format == FT_GLYPH_FORMAT_OUTLINE )
      {
        /* check that the loaded outline is correct */
        error = FT_Outline_Check( &ਥਾਂ->outline );
        ਜੇ ( error )
          ਜਾਓ Exit;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GRID_FIT_METRICS
        ਜੇ ( !( ਲੱਦੋ_flags & FT_ਲੱਦੋ_NO_HINTING ) )
          ft_glyphslot_grid_fit_metrics(
            ਥਾਂ,
            FT_BOOL( ਲੱਦੋ_flags & FT_ਲੱਦੋ_VERTICAL_LAYOUT ) );
#ਜੇ_ਅੰਤ
      }
    }

  ਲੱਦੋ_Ok:
    /* compute the advance */
    ਜੇ ( ਲੱਦੋ_flags & FT_ਲੱਦੋ_VERTICAL_LAYOUT )
    {
      ਥਾਂ->advance.x = 0;
      ਥਾਂ->advance.y = ਥਾਂ->metrics.vertAdvance;
    }
    ਨਹੀਂਤਾਂ
    {
      ਥਾਂ->advance.x = ਥਾਂ->metrics.horiAdvance;
      ਥਾਂ->advance.y = 0;
    }

    /* compute the linear advance in 16.16 pixels */
    ਜੇ ( ( ਲੱਦੋ_flags & FT_ਲੱਦੋ_ਰੇਖਿਕ_DESIGN ) == 0 &&
         FT_IS_SCALABLE( face )                      )
    {
      FT_ਮਾਪ_Metrics*  metrics = &face->ਮਾਪ->metrics;


      /* it's tricky! */
      ਥਾਂ->ਰੇਖਿਕ_Hori_Advance = FT_MulDiv( ਥਾਂ->ਰੇਖਿਕ_Hori_Advance,
                                           metrics->x_scale, 64 );

      ਥਾਂ->ਰੇਖਿਕ_Vert_Advance = FT_MulDiv( ਥਾਂ->ਰੇਖਿਕ_Vert_Advance,
                                           metrics->y_scale, 64 );
    }

    ਜੇ ( ( ਲੱਦੋ_flags & FT_ਲੱਦੋ_IGNORE_TRANSFORM ) == 0 )
    {
      FT_Face_ਅੰਦਰੂਨੀ  ਅੰਦਰੂਨੀ = face->ਅੰਦਰੂਨੀ;


      /* now, transform the glyph image if needed */
      ਜੇ ( ਅੰਦਰੂਨੀ->transform_flags )
      {
        /* get renderer */
        FT_Renderer  renderer = ft_lookup_glyph_renderer( ਥਾਂ );


        ਜੇ ( renderer )
          error = renderer->clazz->transform_glyph(
                                     renderer, ਥਾਂ,
                                     &ਅੰਦਰੂਨੀ->transform_ਮੈਟ੍ਰਿਕਸ,
                                     &ਅੰਦਰੂਨੀ->transform_delta );
        ਨਹੀਂਤਾਂ ਜੇ ( ਥਾਂ->format == FT_GLYPH_FORMAT_OUTLINE )
        {
          /* apply `standard' transformation if no renderer is available */
          ਜੇ ( ਅੰਦਰੂਨੀ->transform_flags & 1 )
            FT_Outline_Transform( &ਥਾਂ->outline,
                                  &ਅੰਦਰੂਨੀ->transform_ਮੈਟ੍ਰਿਕਸ );

          ਜੇ ( ਅੰਦਰੂਨੀ->transform_flags & 2 )
            FT_Outline_Translate( &ਥਾਂ->outline,
                                  ਅੰਦਰੂਨੀ->transform_delta.x,
                                  ਅੰਦਰੂਨੀ->transform_delta.y );
        }

        /* transform advance */
        FT_ਸਦਿਸ਼_Transform( &ਥਾਂ->advance, &ਅੰਦਰੂਨੀ->transform_ਮੈਟ੍ਰਿਕਸ );
      }
    }

    ਥਾਂ->glyph_ਸੂਚਕਾਂਕ          = glyph_ਸੂਚਕਾਂਕ;
    ਥਾਂ->ਅੰਦਰੂਨੀ->ਲੱਦੋ_flags = ਲੱਦੋ_flags;

    /* do we need to render the image or preset the bitmap now? */
    ਜੇ ( !error                                    &&
         ( ਲੱਦੋ_flags & FT_ਲੱਦੋ_NO_SCALE ) == 0    &&
         ਥਾਂ->format != FT_GLYPH_FORMAT_BITMAP    &&
         ਥਾਂ->format != FT_GLYPH_FORMAT_ਸੰਯੁਕਤ )
    {
      FT_Render_ਬਹੁਲਕ  ਬਹੁਲਕ = FT_ਲੱਦੋ_ਨਿਸ਼ਾਨਾ_ਬਹੁਲਕ( ਲੱਦੋ_flags );


      ਜੇ ( ਬਹੁਲਕ == FT_RENDER_ਬਹੁਲਕ_ਸਾਧਾਰਨ   &&
           ਲੱਦੋ_flags & FT_ਲੱਦੋ_MONOCHROME )
        ਬਹੁਲਕ = FT_RENDER_ਬਹੁਲਕ_MONO;

      ਜੇ ( ਲੱਦੋ_flags & FT_ਲੱਦੋ_RENDER )
        error = FT_Render_Glyph( ਥਾਂ, ਬਹੁਲਕ );
      ਨਹੀਂਤਾਂ
        ft_glyphslot_preset_bitmap( ਥਾਂ, ਬਹੁਲਕ, ਨਿਰਾ );
    }

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ FT_DEBUG_LEVEL_TRACE
    FT_TRACE5(( "FT_Load_Glyph: index %u, flags 0x%x\n",
                glyph_ਸੂਚਕਾਂਕ, ਲੱਦੋ_flags ));
    FT_TRACE5(( "  bitmap %ux%u %s, %s (mode %d)\n",
                ਥਾਂ->bitmap.ਚੌੜਾਈ,
                ਥਾਂ->bitmap.ਪੰਕਤੀਆਂ,
                ਥਾਂ->outline.points ?
                  ਥਾਂ->bitmap.ਬਫਰ ? "rendered"
                                      : "preset"
                                     :
                  ਥਾਂ->ਅੰਦਰੂਨੀ->flags & FT_GLYPH_OWN_BITMAP ? "owned"
                                                              : "unowned",
                pixel_modes[ਥਾਂ->bitmap.pixel_ਬਹੁਲਕ],
                ਥਾਂ->bitmap.pixel_ਬਹੁਲਕ ));
    FT_TRACE5(( "\n" ));
    FT_TRACE5(( "  x advance: %f\n", (ਦੁੱਗਣਾ)ਥਾਂ->advance.x / 64 ));
    FT_TRACE5(( "  y advance: %f\n", (ਦੁੱਗਣਾ)ਥਾਂ->advance.y / 64 ));
    FT_TRACE5(( "  linear x advance: %f\n",
                (ਦੁੱਗਣਾ)ਥਾਂ->ਰੇਖਿਕ_Hori_Advance / 65536 ));
    FT_TRACE5(( "  linear y advance: %f\n",
                (ਦੁੱਗਣਾ)ਥਾਂ->ਰੇਖਿਕ_Vert_Advance / 65536 ));

    {
      FT_Glyph_Metrics*  metrics = &ਥਾਂ->metrics;


      FT_TRACE5(( "  metrics:\n" ));
      FT_TRACE5(( "    width:  %f\n", (ਦੁੱਗਣਾ)metrics->ਚੌੜਾਈ / 64 ));
      FT_TRACE5(( "    height: %f\n", (ਦੁੱਗਣਾ)metrics->ਉਚਾਈ / 64 ));
      FT_TRACE5(( "\n" ));
      FT_TRACE5(( "    horiBearingX: %f\n",
                  (ਦੁੱਗਣਾ)metrics->horiBearingX / 64 ));
      FT_TRACE5(( "    horiBearingY: %f\n",
                  (ਦੁੱਗਣਾ)metrics->horiBearingY / 64 ));
      FT_TRACE5(( "    horiAdvance:  %f\n",
                  (ਦੁੱਗਣਾ)metrics->horiAdvance / 64 ));
      FT_TRACE5(( "\n" ));
      FT_TRACE5(( "    vertBearingX: %f\n",
                  (ਦੁੱਗਣਾ)metrics->vertBearingX / 64 ));
      FT_TRACE5(( "    vertBearingY: %f\n",
                  (ਦੁੱਗਣਾ)metrics->vertBearingY / 64 ));
      FT_TRACE5(( "    vertAdvance:  %f\n",
                  (ਦੁੱਗਣਾ)metrics->vertAdvance / 64 ));
    }
#ਜੇ_ਅੰਤ

  Exit:
    ਵਾਪਸ error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_ਲੱਦੋ_Char( FT_Face   face,
                FT_ULong  char_ਕੋਡ,
                FT_Int32  ਲੱਦੋ_flags )
  {
    FT_UInt  glyph_ਸੂਚਕਾਂਕ;


    ਜੇ ( !face )
      ਵਾਪਸ FT_THROW( Invalid_Face_Handle );

    glyph_ਸੂਚਕਾਂਕ = (FT_UInt)char_ਕੋਡ;
    ਜੇ ( face->charmap )
      glyph_ਸੂਚਕਾਂਕ = FT_Get_Char_ਸੂਚਕਾਂਕ( face, char_ਕੋਡ );

    ਵਾਪਸ FT_ਲੱਦੋ_Glyph( face, glyph_ਸੂਚਕਾਂਕ, ਲੱਦੋ_flags );
  }


  /* destructor for sizes list */
  ਸਥਿਰ ਖਾਲੀ
  destroy_ਮਾਪ( FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ,
                ਖਾਲੀ*      ਮਾਪ_2,
                ਖਾਲੀ*      driver_ )
  {
    FT_ਮਾਪ    ਮਾਪ   = (FT_ਮਾਪ)ਮਾਪ_2;
    FT_Driver  driver = (FT_Driver)driver_;


    /* finalize format-specific stuff */
    ਜੇ ( driver->clazz->ਪੂਰਾ_ਮਾਪ )
      driver->clazz->ਪੂਰਾ_ਮਾਪ( ਮਾਪ );

    /* finalize client-specific data */
    ਜੇ ( ਮਾਪ->generic.finalizer )
      ਮਾਪ->generic.finalizer( ਮਾਪ );

    FT_FREE( ਮਾਪ->ਅੰਦਰੂਨੀ );
    FT_FREE( ਮਾਪ );
  }


  ਸਥਿਰ ਖਾਲੀ
  ft_cmap_ਪੂਰਾ_ਅੰਦਰੂਨੀ( FT_CMap  cmap );


  ਸਥਿਰ ਖਾਲੀ
  destroy_charmaps( FT_Face    face,
                    FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ )
  {
    FT_Int  n;


    ਜੇ ( !face )
      ਵਾਪਸ;

    ਲਈ ( n = 0; n < face->ਅੰਕ_charmaps; n++ )
    {
      FT_CMap  cmap = FT_CMAP( face->charmaps[n] );


      ft_cmap_ਪੂਰਾ_ਅੰਦਰੂਨੀ( cmap );

      face->charmaps[n] = ਨਿਰਾ;
    }

    FT_FREE( face->charmaps );
    face->ਅੰਕ_charmaps = 0;
  }


  /* destructor for faces list */
  ਸਥਿਰ ਖਾਲੀ
  destroy_face( FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ,
                ਖਾਲੀ*      face_,
                ਖਾਲੀ*      driver_ )
  {
    FT_Face          face   = (FT_Face)face_;
    FT_Driver        driver = (FT_Driver)driver_;
    FT_Driver_Class  clazz  = driver->clazz;


    /* discard auto-hinting data */
    ਜੇ ( face->autohint.finalizer )
      face->autohint.finalizer( face->autohint.ਅੰਕੜੇ );

    /* Discard glyph slots for this face.                           */
    /* Beware!  FT_Done_GlyphSlot() changes the field `face->glyph' */
    ਜਦੋਂ ( face->glyph )
      FT_ਪੂਰਾ_Glyph_ਥਾਂ( face->glyph );

    /* discard all sizes for this face */
    FT_List_Finalize( &face->sizes_list,
                      destroy_ਮਾਪ,
                      ਸਮ੍ਰਿਤੀ,
                      driver );
    face->ਮਾਪ = ਨਿਰਾ;

    /* discard charmaps */
    destroy_charmaps( face, ਸਮ੍ਰਿਤੀ );

    /* finalize format-specific stuff */
    ਜੇ ( clazz->ਪੂਰਾ_face )
      clazz->ਪੂਰਾ_face( face );

    /* close the stream for this face if needed */
    FT_ਧਾਰਾ_Free(
      face->ਧਾਰਾ,
      ( face->face_flags & FT_FACE_FLAG_EXTERNAL_ਧਾਰਾ ) != 0 );

    face->ਧਾਰਾ = ਨਿਰਾ;

    /* now discard client data */
    ਜੇ ( face->generic.finalizer )
      face->generic.finalizer( face );

    /* get rid of it */
    ਜੇ ( face->ਅੰਦਰੂਨੀ )
    {
      FT_FREE( face->ਅੰਦਰੂਨੀ );
    }
    FT_FREE( face );
  }


  ਸਥਿਰ ਖਾਲੀ
  Destroy_Driver( FT_Driver  driver )
  {
    FT_List_Finalize( &driver->faces_list,
                      destroy_face,
                      driver->ਜੜ੍ਹ.ਸਮ੍ਰਿਤੀ,
                      driver );
  }


  /* documentation is in ftobjs.h */

  FT_ਅਧਾਰ_DEF( FT_Error )
  ਲੱਭੋ_unicode_charmap( FT_Face  face )
  {
    FT_CharMap*  ਪਹਿਲਾ;
    FT_CharMap*  cur;


    /* caller should have already checked that `face' is valid */
    FT_ASSERT( face );

    ਪਹਿਲਾ = face->charmaps;

    ਜੇ ( !ਪਹਿਲਾ )
      ਵਾਪਸ FT_THROW( Invalid_CharMap_Handle );

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

    cur = ਪਹਿਲਾ + face->ਅੰਕ_charmaps;  /* points after the last one */

    ਲਈ ( ; --cur >= ਪਹਿਲਾ; )
    {
      ਜੇ ( cur[0]->encoding == FT_ENCODING_UNICODE )
      {
        /* XXX If some new encodings to represent UCS-4 are added, */
        /*     they should be added here.                          */
        ਜੇ ( ( cur[0]->platform_id == TT_PLATFORM_MICROSOFT &&
               cur[0]->encoding_id == TT_MS_ID_UCS_4        )     ||
             ( cur[0]->platform_id == TT_PLATFORM_APPLE_UNICODE &&
               cur[0]->encoding_id == TT_APPLE_ID_UNICODE_32    ) ||
             ( cur[0]->platform_id == TT_PLATFORM_APPLE_UNICODE &&
               cur[0]->encoding_id == TT_APPLE_ID_ਭਰਿਆ_UNICODE  &&
               FT_Get_CMap_Format( cur[0] ) == 13               ) )
        {
          face->charmap = cur[0];
          ਵਾਪਸ FT_Err_Ok;
        }
      }
    }

    /* We do not have any UCS-4 charmap.                */
    /* Do the loop again and search for UCS-2 charmaps. */
    cur = ਪਹਿਲਾ + face->ਅੰਕ_charmaps;

    ਲਈ ( ; --cur >= ਪਹਿਲਾ; )
    {
      ਜੇ ( cur[0]->encoding == FT_ENCODING_UNICODE )
      {
        FT_Long  format = FT_Get_CMap_Format( cur[0] );


        ਜੇ ( format != 14 && format != 15 )
        {
          face->charmap = cur[0];
          ਵਾਪਸ FT_Err_Ok;
        }
      }
    }

    ਵਾਪਸ FT_THROW( Invalid_CharMap_Handle );
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
  ਸਥਿਰ FT_CharMap
  ਲੱਭੋ_variant_selector_charmap( FT_Face  face )
  {
    FT_CharMap*  ਪਹਿਲਾ;
    FT_CharMap*  ਸਿਰਾ;
    FT_CharMap*  cur;


    /* caller should have already checked that `face' is valid */
    FT_ASSERT( face );

    ਪਹਿਲਾ = face->charmaps;

    ਜੇ ( !ਪਹਿਲਾ )
      ਵਾਪਸ ਨਿਰਾ;

    ਸਿਰਾ = ਪਹਿਲਾ + face->ਅੰਕ_charmaps;  /* points after the last one */

    ਲਈ ( cur = ਪਹਿਲਾ; cur < ਸਿਰਾ; cur++ )
    {
      ਜੇ ( cur[0]->platform_id == TT_PLATFORM_APPLE_UNICODE    &&
           cur[0]->encoding_id == TT_APPLE_ID_VARIANT_SELECTOR )
      {
        FT_Long  format = FT_Get_CMap_Format( cur[0] );


        ਜੇ ( format == 14 || format == 15 )
          ਵਾਪਸ cur[0];
      }
    }

    ਵਾਪਸ ਨਿਰਾ;
  }


  /**************************************************************************
   *
   * @Function:
   *   open_face
   *
   * @Description:
   *   This function does some work for FT_Open_Face().
   */
  ਸਥਿਰ FT_Error
  open_face( FT_Driver      driver,
             FT_ਧਾਰਾ      *astream,
             FT_Bool        *anexternal_ਧਾਰਾ,
             FT_Long        face_ਸੂਚਕਾਂਕ,
             FT_Int         ਅੰਕ_params,
             FT_Parameter*  params,
             FT_Face       *aface )
  {
    FT_ਸਮ੍ਰਿਤੀ         ਸਮ੍ਰਿਤੀ;
    FT_Driver_Class   clazz;
    FT_Face           face     = ਨਿਰਾ;
    FT_Face_ਅੰਦਰੂਨੀ  ਅੰਦਰੂਨੀ = ਨਿਰਾ;

    FT_Error          error, error2;


    clazz  = driver->clazz;
    ਸਮ੍ਰਿਤੀ = driver->ਜੜ੍ਹ.ਸਮ੍ਰਿਤੀ;

    /* allocate the face object and perform basic initialization */
    ਜੇ ( FT_ALLOC( face, clazz->face_object_ਮਾਪ ) )
      ਜਾਓ Fail;

    face->driver = driver;
    face->ਸਮ੍ਰਿਤੀ = ਸਮ੍ਰਿਤੀ;
    face->ਧਾਰਾ = *astream;

    /* set the FT_FACE_FLAG_EXTERNAL_STREAM bit for FT_Done_Face */
    ਜੇ ( *anexternal_ਧਾਰਾ )
      face->face_flags |= FT_FACE_FLAG_EXTERNAL_ਧਾਰਾ;

    ਜੇ ( FT_NEW( ਅੰਦਰੂਨੀ ) )
      ਜਾਓ Fail;

    face->ਅੰਦਰੂਨੀ = ਅੰਦਰੂਨੀ;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ FT_CONFIG_OPTION_INCREMENTAL
    {
      ਪੂਰਨ  i;


      face->ਅੰਦਰੂਨੀ->incremental_interface = ਨਿਰਾ;
      ਲਈ ( i = 0; i < ਅੰਕ_params && !face->ਅੰਦਰੂਨੀ->incremental_interface;
            i++ )
        ਜੇ ( params[i].ਟੈਗ == FT_PARAM_ਟੈਗ_INCREMENTAL )
          face->ਅੰਦਰੂਨੀ->incremental_interface =
            (FT_Incremental_Interface)params[i].ਅੰਕੜੇ;
    }
#ਜੇ_ਅੰਤ

    face->ਅੰਦਰੂਨੀ->random_ਬੀਜ = -1;

    ਜੇ ( clazz->init_face )
      error = clazz->init_face( *astream,
                                face,
                                (FT_Int)face_ਸੂਚਕਾਂਕ,
                                ਅੰਕ_params,
                                params );
    /* Stream may have been changed. */
    *astream = face->ਧਾਰਾ;
    *anexternal_ਧਾਰਾ =
      ( face->face_flags & FT_FACE_FLAG_EXTERNAL_ਧਾਰਾ ) != 0;
    ਜੇ ( error )
      ਜਾਓ Fail;

    /* select Unicode charmap by default */
    error2 = ਲੱਭੋ_unicode_charmap( face );

    /* if no Unicode charmap can be found, FT_Err_Invalid_CharMap_Handle */
    /* is returned.                                                      */

    /* no error should happen, but we want to play safe */
    ਜੇ ( error2 && FT_ERR_NEQ( error2, Invalid_CharMap_Handle ) )
    {
      error = error2;
      ਜਾਓ Fail;
    }

    *aface = face;

  Fail:
    ਜੇ ( error )
    {
      destroy_charmaps( face, ਸਮ੍ਰਿਤੀ );
      ਜੇ ( clazz->ਪੂਰਾ_face )
        clazz->ਪੂਰਾ_face( face );
      FT_FREE( ਅੰਦਰੂਨੀ );
      FT_FREE( face );
      *aface = ਨਿਰਾ;
    }

    ਵਾਪਸ error;
  }


  /* there's a Mac-specific extended implementation of FT_New_Face() */
  /* in src/base/ftmac.c                                             */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ FT_MACINTOSH

  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_New_Face( FT_Library   library,
               ਅਚਲ ਅੱਖਰ*  pathname,
               FT_Long      face_ਸੂਚਕਾਂਕ,
               FT_Face     *aface )
  {
    FT_Open_ਦਲੀਲਾਂ  ਦਲੀਲਾਂ;


    /* test for valid `library' and `aface' delayed to `FT_Open_Face' */
    ਜੇ ( !pathname )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    ਦਲੀਲਾਂ.flags    = FT_OPEN_PATHNAME;
    ਦਲੀਲਾਂ.pathname = (ਅੱਖਰ*)pathname;
    ਦਲੀਲਾਂ.ਧਾਰਾ   = ਨਿਰਾ;

    ਵਾਪਸ ft_open_face_ਅੰਦਰੂਨੀ( library, &ਦਲੀਲਾਂ, face_ਸੂਚਕਾਂਕ, aface, 1 );
  }

#ਜੇ_ਅੰਤ


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_New_ਸਮ੍ਰਿਤੀ_Face( FT_Library      library,
                      ਅਚਲ FT_Byte*  file_ਅਧਾਰ,
                      FT_Long         file_ਮਾਪ,
                      FT_Long         face_ਸੂਚਕਾਂਕ,
                      FT_Face        *aface )
  {
    FT_Open_ਦਲੀਲਾਂ  ਦਲੀਲਾਂ;


    /* test for valid `library' and `face' delayed to `FT_Open_Face' */
    ਜੇ ( !file_ਅਧਾਰ )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    ਦਲੀਲਾਂ.flags       = FT_OPEN_ਸਮ੍ਰਿਤੀ;
    ਦਲੀਲਾਂ.ਸਮ੍ਰਿਤੀ_ਅਧਾਰ = file_ਅਧਾਰ;
    ਦਲੀਲਾਂ.ਸਮ੍ਰਿਤੀ_ਮਾਪ = file_ਮਾਪ;
    ਦਲੀਲਾਂ.ਧਾਰਾ      = ਨਿਰਾ;

    ਵਾਪਸ ft_open_face_ਅੰਦਰੂਨੀ( library, &ਦਲੀਲਾਂ, face_ਸੂਚਕਾਂਕ, aface, 1 );
  }


#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ FT_CONFIG_OPTION_MAC_FONTS

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
  ਸਥਿਰ ਖਾਲੀ
  ਸਮ੍ਰਿਤੀ_ਧਾਰਾ_close( FT_ਧਾਰਾ  ਧਾਰਾ )
  {
    FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ = (FT_ਸਮ੍ਰਿਤੀ)ਧਾਰਾ->descriptor.ਸੂਚਕ;


    FT_FREE( ਧਾਰਾ->ਅਧਾਰ );
    ਧਾਰਾ->ਮਾਪ  = 0;
    ਧਾਰਾ->close = ਨਿਰਾ;
    FT_FREE( ਧਾਰਾ );
  }


  /* Create a new memory stream from a buffer and a size. */
  /* From `ftmac.c'.                                      */
  ਸਥਿਰ FT_Error
  new_ਸਮ੍ਰਿਤੀ_ਧਾਰਾ( FT_Library           library,
                     FT_Byte*             ਅਧਾਰ,
                     FT_ULong             ਮਾਪ,
                     FT_ਧਾਰਾ_Close_Func  close,
                     FT_ਧਾਰਾ           *astream )
  {
    FT_Error   error;
    FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ;
    FT_ਧਾਰਾ  ਧਾਰਾ = ਨਿਰਾ;


    ਜੇ ( !library )
      ਵਾਪਸ FT_THROW( Invalid_Library_Handle );

    ਜੇ ( !ਅਧਾਰ )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    *astream = ਨਿਰਾ;
    ਸਮ੍ਰਿਤੀ   = library->ਸਮ੍ਰਿਤੀ;
    ਜੇ ( FT_NEW( ਧਾਰਾ ) )
      ਜਾਓ Exit;

    FT_ਧਾਰਾ_Open_ਸਮ੍ਰਿਤੀ( ਧਾਰਾ, ਅਧਾਰ, ਮਾਪ );

    ਧਾਰਾ->descriptor.ਸੂਚਕ = ਸਮ੍ਰਿਤੀ;
    ਧਾਰਾ->close              = close;

    *astream = ਧਾਰਾ;

  Exit:
    ਵਾਪਸ error;
  }


  /* Create a new FT_Face given a buffer and a driver name. */
  /* From `ftmac.c'.                                        */
  FT_LOCAL_DEF( FT_Error )
  open_face_from_ਬਫਰ( FT_Library   library,
                         FT_Byte*     ਅਧਾਰ,
                         FT_ULong     ਮਾਪ,
                         FT_Long      face_ਸੂਚਕਾਂਕ,
                         ਅਚਲ ਅੱਖਰ*  driver_name,
                         FT_Face     *aface )
  {
    FT_Open_ਦਲੀਲਾਂ  ਦਲੀਲਾਂ;
    FT_Error      error;
    FT_ਸਮ੍ਰਿਤੀ     ਸਮ੍ਰਿਤੀ = library->ਸਮ੍ਰਿਤੀ;


    ਦਲੀਲਾਂ.driver = ਨਿਰਾ;
    ਦਲੀਲਾਂ.flags  = 0;

    ਜੇ ( driver_name )
    {
      ਦਲੀਲਾਂ.driver = FT_Get_Module( library, driver_name );
      ਜੇ ( !ਦਲੀਲਾਂ.driver )
      {
        FT_FREE( ਅਧਾਰ );
        ਵਾਪਸ FT_THROW( Missing_Module );
      }

      ਦਲੀਲਾਂ.flags = ਦਲੀਲਾਂ.flags | FT_OPEN_DRIVER;
    }

    /* `memory_stream_close` also frees the stream object. */
    error = new_ਸਮ੍ਰਿਤੀ_ਧਾਰਾ( library,
                               ਅਧਾਰ,
                               ਮਾਪ,
                               ਸਮ੍ਰਿਤੀ_ਧਾਰਾ_close,
                               &ਦਲੀਲਾਂ.ਧਾਰਾ );
    ਜੇ ( error )
    {
      FT_FREE( ਅਧਾਰ );
      ਵਾਪਸ error;
    }

    ਦਲੀਲਾਂ.flags |= FT_OPEN_ਧਾਰਾ;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ FT_MACINTOSH
    /* At this point, the face index has served its purpose;  */
    /* whoever calls this function has already used it to     */
    /* locate the correct font data.  We should not propagate */
    /* this index to FT_Open_Face() (unless it is negative).  */

    ਜੇ ( face_ਸੂਚਕਾਂਕ > 0 )
      face_ਸੂਚਕਾਂਕ &= 0x7FFF0000L; /* retain GX data */
#ਜੇ_ਅੰਤ

    ਵਾਪਸ ft_open_face_ਅੰਦਰੂਨੀ( library, &ਦਲੀਲਾਂ, face_ਸੂਚਕਾਂਕ, aface, 0 );
  }


  /* Look up `TYP1' or `CID ' table from sfnt table directory.       */
  /* `offset' and `length' must exclude the binary header in tables. */

  /* Type 1 and CID-keyed font drivers should recognize sfnt-wrapped */
  /* format too.  Here, since we can't expect that the TrueType font */
  /* driver is loaded unconditionally, we must parse the font by     */
  /* ourselves.  We are only interested in the name of the table and */
  /* the offset.                                                     */

  ਸਥਿਰ FT_Error
  ft_lookup_PS_ਵਿੱਚ_sfnt_ਧਾਰਾ( FT_ਧਾਰਾ  ਧਾਰਾ,
                               FT_Long    face_ਸੂਚਕਾਂਕ,
                               FT_ULong*  ਵਿੱਥ,
                               FT_ULong*  ਲੰਮਾਈ,
                               FT_Bool*   is_sfnt_cid )
  {
    FT_Error   error;
    FT_UShort  ਅੰਕ_Tables;
    FT_Long    pstable_ਸੂਚਕਾਂਕ;
    FT_ULong   ਟੈਗ;
    ਪੂਰਨ        i;


    *ਵਿੱਥ = 0;
    *ਲੰਮਾਈ = 0;
    *is_sfnt_cid = FALSE;

    /* TODO: support for sfnt-wrapped PS/CID in TTC format */

    /* version check for 'typ1' (should be ignored?) */
    ਜੇ ( FT_READ_ULONG( ਟੈਗ ) )
      ਵਾਪਸ error;
    ਜੇ ( ਟੈਗ != TTAG_typ1 )
      ਵਾਪਸ FT_THROW( Unknown_File_Format );

    ਜੇ ( FT_READ_USHORT( ਅੰਕ_Tables ) )
      ਵਾਪਸ error;
    ਜੇ ( FT_ਧਾਰਾ_SKIP( 2 * 3 ) ) /* skip binary search header */
      ਵਾਪਸ error;

    pstable_ਸੂਚਕਾਂਕ = -1;
    *is_sfnt_cid  = FALSE;

    ਲਈ ( i = 0; i < ਅੰਕ_Tables; i++ )
    {
      ਜੇ ( FT_READ_ULONG( ਟੈਗ )     || FT_ਧਾਰਾ_SKIP( 4 )      ||
           FT_READ_ULONG( *ਵਿੱਥ ) || FT_READ_ULONG( *ਲੰਮਾਈ ) )
        ਵਾਪਸ error;

      ਜੇ ( ਟੈਗ == TTAG_CID )
      {
        pstable_ਸੂਚਕਾਂਕ++;
        *ਵਿੱਥ += 22;
        *ਲੰਮਾਈ -= 22;
        *is_sfnt_cid = TRUE;
        ਜੇ ( face_ਸੂਚਕਾਂਕ < 0 )
          ਵਾਪਸ FT_Err_Ok;
      }
      ਨਹੀਂਤਾਂ ਜੇ ( ਟੈਗ == TTAG_TYP1 )
      {
        pstable_ਸੂਚਕਾਂਕ++;
        *ਵਿੱਥ += 24;
        *ਲੰਮਾਈ -= 24;
        *is_sfnt_cid = FALSE;
        ਜੇ ( face_ਸੂਚਕਾਂਕ < 0 )
          ਵਾਪਸ FT_Err_Ok;
      }
      ਜੇ ( face_ਸੂਚਕਾਂਕ >= 0 && pstable_ਸੂਚਕਾਂਕ == face_ਸੂਚਕਾਂਕ )
        ਵਾਪਸ FT_Err_Ok;
    }

    ਵਾਪਸ FT_THROW( ਸਾਰਣੀ_Missing );
  }


  FT_LOCAL_DEF( FT_Error )
  open_face_PS_from_sfnt_ਧਾਰਾ( FT_Library     library,
                                 FT_ਧਾਰਾ      ਧਾਰਾ,
                                 FT_Long        face_ਸੂਚਕਾਂਕ,
                                 FT_Int         ਅੰਕ_params,
                                 FT_Parameter  *params,
                                 FT_Face       *aface )
  {
    FT_Error   error;
    FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ = library->ਸਮ੍ਰਿਤੀ;
    FT_ULong   ਵਿੱਥ, ਲੰਮਾਈ;
    FT_ULong   pos;
    FT_Bool    is_sfnt_cid;
    FT_Byte*   sfnt_ps = ਨਿਰਾ;

    FT_ਅਣਵਰਤਿਆ( ਅੰਕ_params );
    FT_ਅਣਵਰਤਿਆ( params );


    /* ignore GX stuff */
    ਜੇ ( face_ਸੂਚਕਾਂਕ > 0 )
      face_ਸੂਚਕਾਂਕ &= 0xFFFFL;

    pos = FT_ਧਾਰਾ_POS();

    error = ft_lookup_PS_ਵਿੱਚ_sfnt_ਧਾਰਾ( ਧਾਰਾ,
                                         face_ਸੂਚਕਾਂਕ,
                                         &ਵਿੱਥ,
                                         &ਲੰਮਾਈ,
                                         &is_sfnt_cid );
    ਜੇ ( error )
      ਜਾਓ Exit;

    ਜੇ ( ਵਿੱਥ > ਧਾਰਾ->ਮਾਪ )
    {
      FT_TRACE2(( "open_face_PS_from_sfnt_stream: invalid table offset\n" ));
      error = FT_THROW( Invalid_ਸਾਰਣੀ );
      ਜਾਓ Exit;
    }
    ਨਹੀਂਤਾਂ ਜੇ ( ਲੰਮਾਈ > ਧਾਰਾ->ਮਾਪ - ਵਿੱਥ )
    {
      FT_TRACE2(( "open_face_PS_from_sfnt_stream: invalid table length\n" ));
      error = FT_THROW( Invalid_ਸਾਰਣੀ );
      ਜਾਓ Exit;
    }

    error = FT_ਧਾਰਾ_Seek( ਧਾਰਾ, pos + ਵਿੱਥ );
    ਜੇ ( error )
      ਜਾਓ Exit;

    ਜੇ ( FT_QALLOC( sfnt_ps, (FT_Long)ਲੰਮਾਈ ) )
      ਜਾਓ Exit;

    error = FT_ਧਾਰਾ_Read( ਧਾਰਾ, (FT_Byte *)sfnt_ps, ਲੰਮਾਈ );
    ਜੇ ( error )
    {
      FT_FREE( sfnt_ps );
      ਜਾਓ Exit;
    }

    error = open_face_from_ਬਫਰ( library,
                                   sfnt_ps,
                                   ਲੰਮਾਈ,
                                   FT_MIN( face_ਸੂਚਕਾਂਕ, 0 ),
                                   is_sfnt_cid ? "t1cid" : "type1",
                                   aface );
  Exit:
    {
      FT_Error  error1;


      ਜੇ ( FT_ERR_EQ( error, Unknown_File_Format ) )
      {
        error1 = FT_ਧਾਰਾ_Seek( ਧਾਰਾ, pos );
        ਜੇ ( error1 )
          ਵਾਪਸ error1;
      }

      ਵਾਪਸ error;
    }
  }


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ FT_MACINTOSH

  /* The resource header says we've got resource_cnt `POST' (type1) */
  /* resources in this file.  They all need to be coalesced into    */
  /* one lump which gets passed on to the type1 driver.             */
  /* Here can be only one PostScript font in a file so face_index   */
  /* must be 0 (or -1).                                             */
  /*                                                                */
  ਸਥਿਰ FT_Error
  Mac_Read_POST_Resource( FT_Library  library,
                          FT_ਧਾਰਾ   ਧਾਰਾ,
                          FT_Long    *offsets,
                          FT_Long     resource_cnt,
                          FT_Long     face_ਸੂਚਕਾਂਕ,
                          FT_Face    *aface )
  {
    FT_Error   error  = FT_ERR( Cannot_Open_Resource );
    FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ = library->ਸਮ੍ਰਿਤੀ;

    FT_Byte*   pfb_ਅੰਕੜੇ = ਨਿਰਾ;
    ਪੂਰਨ        i, type, flags;
    FT_ULong   len;
    FT_ULong   pfb_len, pfb_pos, pfb_lenpos;
    FT_ULong   rlen, ਆਰਜ਼ੀ;


    ਜੇ ( face_ਸੂਚਕਾਂਕ == -1 )
      face_ਸੂਚਕਾਂਕ = 0;
    ਜੇ ( face_ਸੂਚਕਾਂਕ != 0 )
      ਵਾਪਸ error;

    /* Find the length of all the POST resources, concatenated.  Assume */
    /* worst case (each resource in its own section).                   */
    pfb_len = 0;
    ਲਈ ( i = 0; i < resource_cnt; i++ )
    {
      error = FT_ਧਾਰਾ_Seek( ਧਾਰਾ, (FT_ULong)offsets[i] );
      ਜੇ ( error )
        ਜਾਓ Exit;
      ਜੇ ( FT_READ_ULONG( ਆਰਜ਼ੀ ) )  /* actually LONG */
        ਜਾਓ Exit;

      /* FT2 allocator takes signed long buffer length,
       * too large value causing overflow should be checked
       */
      FT_TRACE4(( "                 POST fragment #%d: length=0x%08lx"
                  " total pfb_len=0x%08lx\n",
                  i, ਆਰਜ਼ੀ, pfb_len + ਆਰਜ਼ੀ + 6 ));

      ਜੇ ( FT_MAC_RFORK_MAX_LEN < ਆਰਜ਼ੀ               ||
           FT_MAC_RFORK_MAX_LEN - ਆਰਜ਼ੀ < pfb_len + 6 )
      {
        FT_TRACE2(( "             MacOS resource length cannot exceed"
                    " 0x%08lx\n",
                    FT_MAC_RFORK_MAX_LEN ));

        error = FT_THROW( Invalid_ਵਿੱਥ );
        ਜਾਓ Exit;
      }

      pfb_len += ਆਰਜ਼ੀ + 6;
    }

    FT_TRACE2(( "             total buffer size to concatenate"
                " %ld POST fragments: 0x%08lx\n",
                 resource_cnt, pfb_len + 2 ));

    ਜੇ ( pfb_len + 2 < 6 )
    {
      FT_TRACE2(( "             too long fragment length makes"
                  " pfb_len confused: pfb_len=0x%08lx\n",
                  pfb_len ));

      error = FT_THROW( Array_Too_Large );
      ਜਾਓ Exit;
    }

    ਜੇ ( FT_QALLOC( pfb_ਅੰਕੜੇ, (FT_Long)pfb_len + 2 ) )
      ਜਾਓ Exit;

    pfb_ਅੰਕੜੇ[0] = 0x80;
    pfb_ਅੰਕੜੇ[1] = 1;            /* Ascii section */
    pfb_ਅੰਕੜੇ[2] = 0;            /* 4-byte length, fill in later */
    pfb_ਅੰਕੜੇ[3] = 0;
    pfb_ਅੰਕੜੇ[4] = 0;
    pfb_ਅੰਕੜੇ[5] = 0;
    pfb_pos     = 6;
    pfb_lenpos  = 2;

    len  = 0;
    type = 1;

    ਲਈ ( i = 0; i < resource_cnt; i++ )
    {
      error = FT_ਧਾਰਾ_Seek( ਧਾਰਾ, (FT_ULong)offsets[i] );
      ਜੇ ( error )
        ਜਾਓ Exit2;
      ਜੇ ( FT_READ_ULONG( rlen ) )
        ਜਾਓ Exit2;

      /* FT2 allocator takes signed long buffer length,
       * too large fragment length causing overflow should be checked
       */
      ਜੇ ( 0x7FFFFFFFUL < rlen )
      {
        error = FT_THROW( Invalid_ਵਿੱਥ );
        ਜਾਓ Exit2;
      }

      ਜੇ ( FT_READ_USHORT( flags ) )
        ਜਾਓ Exit2;

      FT_TRACE3(( "POST fragment[%d]:"
                  " offsets=0x%08lx, rlen=0x%08lx, flags=0x%04x\n",
                  i, offsets[i], rlen, flags ));

      error = FT_ERR( Array_Too_Large );

      /* postpone the check of `rlen longer than buffer' */
      /* until `FT_Stream_Read'                          */

      ਜੇ ( ( flags >> 8 ) == 0 )        /* Comment, should not be loaded */
      {
        FT_TRACE3(( "    Skip POST fragment #%d because it is a comment\n",
                    i ));
        ਜਾਰੀ;
      }

      /* the flags are part of the resource, so rlen >= 2,  */
      /* but some fonts declare rlen = 0 for empty fragment */
      ਜੇ ( rlen > 2 )
        rlen -= 2;
      ਨਹੀਂਤਾਂ
        rlen = 0;

      ਜੇ ( ( flags >> 8 ) == type )
        len += rlen;
      ਨਹੀਂਤਾਂ
      {
        FT_TRACE3(( "    Write POST fragment #%d header (4-byte) to buffer"
                    " %p + 0x%08lx\n",
                    i, (ਖਾਲੀ*)pfb_ਅੰਕੜੇ, pfb_lenpos ));

        ਜੇ ( pfb_lenpos + 3 > pfb_len + 2 )
          ਜਾਓ Exit2;

        pfb_ਅੰਕੜੇ[pfb_lenpos    ] = (FT_Byte)( len );
        pfb_ਅੰਕੜੇ[pfb_lenpos + 1] = (FT_Byte)( len >> 8 );
        pfb_ਅੰਕੜੇ[pfb_lenpos + 2] = (FT_Byte)( len >> 16 );
        pfb_ਅੰਕੜੇ[pfb_lenpos + 3] = (FT_Byte)( len >> 24 );

        ਜੇ ( ( flags >> 8 ) == 5 )      /* End of font mark */
          ਰੋਕੋ;

        FT_TRACE3(( "    Write POST fragment #%d header (6-byte) to buffer"
                    " %p + 0x%08lx\n",
                    i, (ਖਾਲੀ*)pfb_ਅੰਕੜੇ, pfb_pos ));

        ਜੇ ( pfb_pos + 6 > pfb_len + 2 )
          ਜਾਓ Exit2;

        pfb_ਅੰਕੜੇ[pfb_pos++] = 0x80;

        type = flags >> 8;
        len  = rlen;

        pfb_ਅੰਕੜੇ[pfb_pos++] = (FT_Byte)type;
        pfb_lenpos          = pfb_pos;
        pfb_ਅੰਕੜੇ[pfb_pos++] = 0;        /* 4-byte length, fill in later */
        pfb_ਅੰਕੜੇ[pfb_pos++] = 0;
        pfb_ਅੰਕੜੇ[pfb_pos++] = 0;
        pfb_ਅੰਕੜੇ[pfb_pos++] = 0;
      }

      ਜੇ ( pfb_pos > pfb_len || pfb_pos + rlen > pfb_len )
        ਜਾਓ Exit2;

      FT_TRACE3(( "    Load POST fragment #%d (%lu byte) to buffer"
                  " %p + 0x%08lx\n",
                  i, rlen, (ਖਾਲੀ*)pfb_ਅੰਕੜੇ, pfb_pos ));

      error = FT_ਧਾਰਾ_Read( ਧਾਰਾ, (FT_Byte *)pfb_ਅੰਕੜੇ + pfb_pos, rlen );
      ਜੇ ( error )
        ਜਾਓ Exit2;

      pfb_pos += rlen;
    }

    error = FT_ERR( Array_Too_Large );

    ਜੇ ( pfb_pos + 2 > pfb_len + 2 )
      ਜਾਓ Exit2;
    pfb_ਅੰਕੜੇ[pfb_pos++] = 0x80;
    pfb_ਅੰਕੜੇ[pfb_pos++] = 3;

    ਜੇ ( pfb_lenpos + 3 > pfb_len + 2 )
      ਜਾਓ Exit2;
    pfb_ਅੰਕੜੇ[pfb_lenpos    ] = (FT_Byte)( len );
    pfb_ਅੰਕੜੇ[pfb_lenpos + 1] = (FT_Byte)( len >> 8 );
    pfb_ਅੰਕੜੇ[pfb_lenpos + 2] = (FT_Byte)( len >> 16 );
    pfb_ਅੰਕੜੇ[pfb_lenpos + 3] = (FT_Byte)( len >> 24 );

    ਵਾਪਸ open_face_from_ਬਫਰ( library,
                                  pfb_ਅੰਕੜੇ,
                                  pfb_pos,
                                  face_ਸੂਚਕਾਂਕ,
                                  "type1",
                                  aface );

  Exit2:
    ਜੇ ( FT_ERR_EQ( error, Array_Too_Large ) )
      FT_TRACE2(( "  Abort due to too-short buffer to store"
                  " all POST fragments\n" ));
    ਨਹੀਂਤਾਂ ਜੇ ( FT_ERR_EQ( error, Invalid_ਵਿੱਥ ) )
      FT_TRACE2(( "  Abort due to invalid offset in a POST fragment\n" ));

    ਜੇ ( error )
      error = FT_ERR( Cannot_Open_Resource );
    FT_FREE( pfb_ਅੰਕੜੇ );

  Exit:
    ਵਾਪਸ error;
  }


  /* The resource header says we've got resource_cnt `sfnt'      */
  /* (TrueType/OpenType) resources in this file.  Look through   */
  /* them for the one indicated by face_index, load it into mem, */
  /* pass it on to the truetype driver, and return it.           */
  /*                                                             */
  ਸਥਿਰ FT_Error
  Mac_Read_sfnt_Resource( FT_Library  library,
                          FT_ਧਾਰਾ   ਧਾਰਾ,
                          FT_Long    *offsets,
                          FT_Long     resource_cnt,
                          FT_Long     face_ਸੂਚਕਾਂਕ,
                          FT_Face    *aface )
  {
    FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ = library->ਸਮ੍ਰਿਤੀ;
    FT_Byte*   sfnt_ਅੰਕੜੇ = ਨਿਰਾ;
    FT_Error   error;
    FT_ULong   flag_ਵਿੱਥ;
    FT_ULong   rlen;
    ਪੂਰਨ        is_cff;
    FT_Long    face_ਸੂਚਕਾਂਕ_ਵਿੱਚ_resource = 0;


    ਜੇ ( face_ਸੂਚਕਾਂਕ < 0 )
      face_ਸੂਚਕਾਂਕ = -face_ਸੂਚਕਾਂਕ - 1;
    ਜੇ ( face_ਸੂਚਕਾਂਕ >= resource_cnt )
      ਵਾਪਸ FT_THROW( Cannot_Open_Resource );

    flag_ਵਿੱਥ = (FT_ULong)offsets[face_ਸੂਚਕਾਂਕ];
    error = FT_ਧਾਰਾ_Seek( ਧਾਰਾ, flag_ਵਿੱਥ );
    ਜੇ ( error )
      ਜਾਓ Exit;

    ਜੇ ( FT_READ_ULONG( rlen ) )
      ਜਾਓ Exit;
    ਜੇ ( !rlen )
      ਵਾਪਸ FT_THROW( Cannot_Open_Resource );
    ਜੇ ( rlen > FT_MAC_RFORK_MAX_LEN )
      ਵਾਪਸ FT_THROW( Invalid_ਵਿੱਥ );

    error = open_face_PS_from_sfnt_ਧਾਰਾ( library,
                                           ਧਾਰਾ,
                                           face_ਸੂਚਕਾਂਕ,
                                           0, ਨਿਰਾ,
                                           aface );
    ਜੇ ( !error )
      ਜਾਓ Exit;

    /* rewind sfnt stream before open_face_PS_from_sfnt_stream() */
    error = FT_ਧਾਰਾ_Seek( ਧਾਰਾ, flag_ਵਿੱਥ + 4 );
    ਜੇ ( error )
      ਜਾਓ Exit;

    ਜੇ ( FT_QALLOC( sfnt_ਅੰਕੜੇ, rlen ) )
      ਵਾਪਸ error;
    error = FT_ਧਾਰਾ_Read( ਧਾਰਾ, (FT_Byte *)sfnt_ਅੰਕੜੇ, rlen );
    ਜੇ ( error )
    {
      FT_FREE( sfnt_ਅੰਕੜੇ );
      ਜਾਓ Exit;
    }

    is_cff = rlen > 4 && !ft_memcmp( sfnt_ਅੰਕੜੇ, "OTTO", 4 );
    error = open_face_from_ਬਫਰ( library,
                                   sfnt_ਅੰਕੜੇ,
                                   rlen,
                                   face_ਸੂਚਕਾਂਕ_ਵਿੱਚ_resource,
                                   is_cff ? "cff" : "truetype",
                                   aface );

  Exit:
    ਵਾਪਸ error;
  }


  /* Check for a valid resource fork header, or a valid dfont    */
  /* header.  In a resource fork the first 16 bytes are repeated */
  /* at the location specified by bytes 4-7.  In a dfont bytes   */
  /* 4-7 point to 16 bytes of zeroes instead.                    */
  /*                                                             */
  ਸਥਿਰ FT_Error
  IsMacResource( FT_Library  library,
                 FT_ਧਾਰਾ   ਧਾਰਾ,
                 FT_Long     resource_ਵਿੱਥ,
                 FT_Long     face_ਸੂਚਕਾਂਕ,
                 FT_Face    *aface )
  {
    FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ = library->ਸਮ੍ਰਿਤੀ;
    FT_Error   error;
    FT_Long    map_ਵਿੱਥ, rdata_pos;
    FT_Long    *ਅੰਕੜੇ_offsets;
    FT_Long    ਗਿਣਤੀ_2;


    error = FT_Raccess_Get_ਸਿਰਲੇਖ_Info( library, ਧਾਰਾ, resource_ਵਿੱਥ,
                                       &map_ਵਿੱਥ, &rdata_pos );
    ਜੇ ( error )
      ਵਾਪਸ error;

    /* POST resources must be sorted to concatenate properly */
    error = FT_Raccess_Get_ਅੰਕੜੇ_Offsets( library, ਧਾਰਾ,
                                        map_ਵਿੱਥ, rdata_pos,
                                        TTAG_POST, TRUE,
                                        &ਅੰਕੜੇ_offsets, &ਗਿਣਤੀ_2 );
    ਜੇ ( !error )
    {
      error = Mac_Read_POST_Resource( library, ਧਾਰਾ, ਅੰਕੜੇ_offsets, ਗਿਣਤੀ_2,
                                      face_ਸੂਚਕਾਂਕ, aface );
      FT_FREE( ਅੰਕੜੇ_offsets );
      /* POST exists in an LWFN providing a single face */
      ਜੇ ( !error )
        (*aface)->ਅੰਕ_faces = 1;
      ਵਾਪਸ error;
    }

    /* sfnt resources should not be sorted to preserve the face order by
       QuickDraw API */
    error = FT_Raccess_Get_ਅੰਕੜੇ_Offsets( library, ਧਾਰਾ,
                                        map_ਵਿੱਥ, rdata_pos,
                                        TTAG_sfnt, FALSE,
                                        &ਅੰਕੜੇ_offsets, &ਗਿਣਤੀ_2 );
    ਜੇ ( !error )
    {
      FT_Long  face_ਸੂਚਕਾਂਕ_ਅੰਦਰੂਨੀ = face_ਸੂਚਕਾਂਕ % ਗਿਣਤੀ_2;


      error = Mac_Read_sfnt_Resource( library, ਧਾਰਾ, ਅੰਕੜੇ_offsets, ਗਿਣਤੀ_2,
                                      face_ਸੂਚਕਾਂਕ_ਅੰਦਰੂਨੀ, aface );
      FT_FREE( ਅੰਕੜੇ_offsets );
      ਜੇ ( !error )
      {
        (*aface)->ਅੰਕ_faces  = ਗਿਣਤੀ_2;
        (*aface)->face_ਸੂਚਕਾਂਕ = face_ਸੂਚਕਾਂਕ_ਅੰਦਰੂਨੀ;
      }
    }

    ਵਾਪਸ error;
  }


  /* Check for a valid macbinary header, and if we find one   */
  /* check that the (flattened) resource fork in it is valid. */
  /*                                                          */
  ਸਥਿਰ FT_Error
  Is_Mac_ਦੁਆਧਾਰੀ( FT_Library  library,
               FT_ਧਾਰਾ   ਧਾਰਾ,
               FT_Long     face_ਸੂਚਕਾਂਕ,
               FT_Face    *aface )
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ  ਸਿਰਲੇਖ[128];
    FT_Error       error;
    FT_Long        dlen, ਵਿੱਥ;


    ਜੇ ( !ਧਾਰਾ )
      ਵਾਪਸ FT_THROW( Invalid_ਧਾਰਾ_Operation );

    error = FT_ਧਾਰਾ_Seek( ਧਾਰਾ, 0 );
    ਜੇ ( error )
      ਜਾਓ Exit;

    error = FT_ਧਾਰਾ_Read( ਧਾਰਾ, (FT_Byte*)ਸਿਰਲੇਖ, 128 );
    ਜੇ ( error )
      ਜਾਓ Exit;

    ਜੇ (            ਸਿਰਲੇਖ[ 0] !=   0 ||
                    ਸਿਰਲੇਖ[74] !=   0 ||
                    ਸਿਰਲੇਖ[82] !=   0 ||
                    ਸਿਰਲੇਖ[ 1] ==   0 ||
                    ਸਿਰਲੇਖ[ 1] >   33 ||
                    ਸਿਰਲੇਖ[63] !=   0 ||
         ਸਿਰਲੇਖ[2 + ਸਿਰਲੇਖ[1]] !=   0 ||
                  ਸਿਰਲੇਖ[0x53] > 0x7F )
      ਵਾਪਸ FT_THROW( Unknown_File_Format );

    dlen = ( ਸਿਰਲੇਖ[0x53] << 24 ) |
           ( ਸਿਰਲੇਖ[0x54] << 16 ) |
           ( ਸਿਰਲੇਖ[0x55] <<  8 ) |
             ਸਿਰਲੇਖ[0x56];
#ਜੇ 0
    rlen = ( ਸਿਰਲੇਖ[0x57] << 24 ) |
           ( ਸਿਰਲੇਖ[0x58] << 16 ) |
           ( ਸਿਰਲੇਖ[0x59] <<  8 ) |
             ਸਿਰਲੇਖ[0x5A];
#ਜੇ_ਅੰਤ /* 0 */
    ਵਿੱਥ = 128 + ( ( dlen + 127 ) & ~127 );

    ਵਾਪਸ IsMacResource( library, ਧਾਰਾ, ਵਿੱਥ, face_ਸੂਚਕਾਂਕ, aface );

  Exit:
    ਵਾਪਸ error;
  }


  ਸਥਿਰ FT_Error
  ਲੱਦੋ_face_ਵਿੱਚ_embedded_rfork( FT_Library           library,
                               FT_ਧਾਰਾ            ਧਾਰਾ,
                               FT_Long              face_ਸੂਚਕਾਂਕ,
                               FT_Face             *aface,
                               ਅਚਲ FT_Open_ਦਲੀਲਾਂ  *ਦਲੀਲਾਂ )
  {

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ  FT_COMPONENT
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FT_COMPONENT  raccess

    FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ = library->ਸਮ੍ਰਿਤੀ;
    FT_Error   error  = FT_ERR( Unknown_File_Format );
    FT_UInt    i;

    ਅੱਖਰ*      file_ਨਾਂ[FT_RACCESS_N_ਨਿਯਮਾਂ];
    FT_Long    offsets[FT_RACCESS_N_ਨਿਯਮਾਂ];
    FT_Error   errors[FT_RACCESS_N_ਨਿਯਮਾਂ];
    FT_Bool    is_darwin_vfs, vfs_rfork_has_no_font = FALSE; /* not tested */

    FT_Open_ਦਲੀਲਾਂ  args2;
    FT_ਧਾਰਾ     stream2 = ਨਿਰਾ;


    FT_Raccess_Guess( library, ਧਾਰਾ,
                      ਦਲੀਲਾਂ->pathname, file_ਨਾਂ, offsets, errors );

    ਲਈ ( i = 0; i < FT_RACCESS_N_ਨਿਯਮਾਂ; i++ )
    {
      is_darwin_vfs = ft_raccess_ਨਿਯਮ_by_darwin_vfs( library, i );
      ਜੇ ( is_darwin_vfs && vfs_rfork_has_no_font )
      {
        FT_TRACE3(( "Skip rule %u: darwin vfs resource fork"
                    " is already checked and"
                    " no font is found\n",
                    i ));
        ਜਾਰੀ;
      }

      ਜੇ ( errors[i] )
      {
        FT_TRACE3(( "Error 0x%x has occurred in rule %u\n",
                    errors[i], i ));
        ਜਾਰੀ;
      }

      args2.flags    = FT_OPEN_PATHNAME;
      args2.pathname = file_ਨਾਂ[i] ? file_ਨਾਂ[i] : ਦਲੀਲਾਂ->pathname;

      FT_TRACE3(( "Try rule %u: %s (offset=%ld) ...",
                  i, args2.pathname, offsets[i] ));

      error = FT_ਧਾਰਾ_New( library, &args2, &stream2 );
      ਜੇ ( is_darwin_vfs && FT_ERR_EQ( error, Cannot_Open_ਧਾਰਾ ) )
        vfs_rfork_has_no_font = TRUE;

      ਜੇ ( error )
      {
        FT_TRACE3(( "failed\n" ));
        ਜਾਰੀ;
      }

      error = IsMacResource( library, stream2, offsets[i],
                             face_ਸੂਚਕਾਂਕ, aface );
      FT_ਧਾਰਾ_Free( stream2, 0 );

      FT_TRACE3(( "%s\n", error ? "failed": "successful" ));

      ਜੇ ( !error )
          ਰੋਕੋ;
      ਨਹੀਂਤਾਂ ਜੇ ( is_darwin_vfs )
          vfs_rfork_has_no_font = TRUE;
    }

    ਲਈ (i = 0; i < FT_RACCESS_N_ਨਿਯਮਾਂ; i++)
    {
      ਜੇ ( file_ਨਾਂ[i] )
        FT_FREE( file_ਨਾਂ[i] );
    }

    /* Caller (load_mac_face) requires FT_Err_Unknown_File_Format. */
    ਜੇ ( error )
      error = FT_ERR( Unknown_File_Format );

    ਵਾਪਸ error;

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ  FT_COMPONENT
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FT_COMPONENT  objs

  }


  /* Check for some macintosh formats without Carbon framework.    */
  /* Is this a macbinary file?  If so look at the resource fork.   */
  /* Is this a mac dfont file?                                     */
  /* Is this an old style resource fork? (in data)                 */
  /* Else call load_face_in_embedded_rfork to try extra rules      */
  /* (defined in `ftrfork.c').                                     */
  /*                                                               */
  ਸਥਿਰ FT_Error
  ਲੱਦੋ_mac_face( FT_Library           library,
                 FT_ਧਾਰਾ            ਧਾਰਾ,
                 FT_Long              face_ਸੂਚਕਾਂਕ,
                 FT_Face             *aface,
                 ਅਚਲ FT_Open_ਦਲੀਲਾਂ  *ਦਲੀਲਾਂ )
  {
    FT_Error error;
    FT_ਅਣਵਰਤਿਆ( ਦਲੀਲਾਂ );


    error = Is_Mac_ਦੁਆਧਾਰੀ( library, ਧਾਰਾ, face_ਸੂਚਕਾਂਕ, aface );
    ਜੇ ( FT_ERR_EQ( error, Unknown_File_Format ) )
    {

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ  FT_COMPONENT
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FT_COMPONENT  raccess

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ FT_DEBUG_LEVEL_TRACE
      FT_TRACE3(( "Try as dfont: " ));
      ਜੇ ( !( ਦਲੀਲਾਂ->flags & FT_OPEN_ਸਮ੍ਰਿਤੀ ) )
        FT_TRACE3(( "%s ...", ਦਲੀਲਾਂ->pathname ));
#ਜੇ_ਅੰਤ

      error = IsMacResource( library, ਧਾਰਾ, 0, face_ਸੂਚਕਾਂਕ, aface );

      FT_TRACE3(( "%s\n", error ? "failed" : "successful" ));

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ  FT_COMPONENT
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FT_COMPONENT  objs

    }

    ਜੇ ( ( FT_ERR_EQ( error, Unknown_File_Format )      ||
           FT_ERR_EQ( error, Invalid_ਧਾਰਾ_Operation ) ) &&
         ( ਦਲੀਲਾਂ->flags & FT_OPEN_PATHNAME )               )
      error = ਲੱਦੋ_face_ਵਿੱਚ_embedded_rfork( library, ਧਾਰਾ,
                                           face_ਸੂਚਕਾਂਕ, aface, ਦਲੀਲਾਂ );
    ਵਾਪਸ error;
  }
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ  /* !FT_MACINTOSH && FT_CONFIG_OPTION_MAC_FONTS */


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Open_Face( FT_Library           library,
                ਅਚਲ FT_Open_ਦਲੀਲਾਂ*  ਦਲੀਲਾਂ,
                FT_Long              face_ਸੂਚਕਾਂਕ,
                FT_Face             *aface )
  {
    ਵਾਪਸ ft_open_face_ਅੰਦਰੂਨੀ( library, ਦਲੀਲਾਂ, face_ਸੂਚਕਾਂਕ, aface, 1 );
  }


  ਸਥਿਰ FT_Error
  ft_open_face_ਅੰਦਰੂਨੀ( FT_Library           library,
                         ਅਚਲ FT_Open_ਦਲੀਲਾਂ*  ਦਲੀਲਾਂ,
                         FT_Long              face_ਸੂਚਕਾਂਕ,
                         FT_Face             *aface,
                         FT_Bool              test_mac_fonts )
  {
    FT_Error     error;
    FT_Driver    driver = ਨਿਰਾ;
    FT_ਸਮ੍ਰਿਤੀ    ਸਮ੍ਰਿਤੀ = ਨਿਰਾ;
    FT_ਧਾਰਾ    ਧਾਰਾ = ਨਿਰਾ;
    FT_Face      face   = ਨਿਰਾ;
    FT_List_ਗੰਢ  ਗੰਢ   = ਨਿਰਾ;
    FT_Bool      external_ਧਾਰਾ;
    FT_Module*   cur;
    FT_Module*   ਹੱਦਬੰਦੀ;

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ FT_CONFIG_OPTION_MAC_FONTS
    FT_ਅਣਵਰਤਿਆ( test_mac_fonts );
#ਜੇ_ਅੰਤ


    /* only use lower 31 bits together with sign bit */
    ਜੇ ( face_ਸੂਚਕਾਂਕ > 0 )
      face_ਸੂਚਕਾਂਕ &= 0x7FFFFFFFL;
    ਨਹੀਂਤਾਂ
    {
      face_ਸੂਚਕਾਂਕ  = -face_ਸੂਚਕਾਂਕ;
      face_ਸੂਚਕਾਂਕ &= 0x7FFFFFFFL;
      face_ਸੂਚਕਾਂਕ  = -face_ਸੂਚਕਾਂਕ;
    }

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ FT_DEBUG_LEVEL_TRACE
    FT_TRACE3(( "FT_Open_Face: " ));
    ਜੇ ( face_ਸੂਚਕਾਂਕ < 0 )
      FT_TRACE3(( "Requesting number of faces and named instances\n"));
    ਨਹੀਂਤਾਂ
    {
      FT_TRACE3(( "Requesting face %ld", face_ਸੂਚਕਾਂਕ & 0xFFFFL ));
      ਜੇ ( face_ਸੂਚਕਾਂਕ & 0x7FFF0000L )
        FT_TRACE3(( ", named instance %ld", face_ਸੂਚਕਾਂਕ >> 16 ));
      FT_TRACE3(( "\n" ));
    }
#ਜੇ_ਅੰਤ

    /* test for valid `library' delayed to `FT_Stream_New' */

    ਜੇ ( !ਦਲੀਲਾਂ )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    external_ਧਾਰਾ = FT_BOOL( ( ਦਲੀਲਾਂ->flags & FT_OPEN_ਧਾਰਾ ) &&
                               ਦਲੀਲਾਂ->ਧਾਰਾ                     );

    /* create input stream */
    error = FT_ਧਾਰਾ_New( library, ਦਲੀਲਾਂ, &ਧਾਰਾ );
    ਜੇ ( error )
      ਜਾਓ Fail3;

    /* Do this error check after `FT_Stream_New` to ensure that the */
    /* 'close' callback is called.                                  */
    ਜੇ ( !aface && face_ਸੂਚਕਾਂਕ >= 0 )
    {
      error = FT_THROW( Invalid_ਦਲੀਲ );
      ਜਾਓ Fail3;
    }

    ਸਮ੍ਰਿਤੀ = library->ਸਮ੍ਰਿਤੀ;

    /* If the font driver is specified in the `args' structure, use */
    /* it.  Otherwise, we scan the list of registered drivers.      */
    ਜੇ ( ( ਦਲੀਲਾਂ->flags & FT_OPEN_DRIVER ) && ਦਲੀਲਾਂ->driver )
    {
      driver = FT_DRIVER( ਦਲੀਲਾਂ->driver );

      /* not all modules are drivers, so check... */
      ਜੇ ( FT_MODULE_IS_DRIVER( driver ) )
      {
        FT_Int         ਅੰਕ_params = 0;
        FT_Parameter*  params     = ਨਿਰਾ;


        ਜੇ ( ਦਲੀਲਾਂ->flags & FT_OPEN_PARAMS )
        {
          ਅੰਕ_params = ਦਲੀਲਾਂ->ਅੰਕ_params;
          params     = ਦਲੀਲਾਂ->params;
        }

        error = open_face( driver, &ਧਾਰਾ, &external_ਧਾਰਾ, face_ਸੂਚਕਾਂਕ,
                           ਅੰਕ_params, params, &face );
        ਜੇ ( !error )
          ਜਾਓ Success;
      }
      ਨਹੀਂਤਾਂ
        error = FT_THROW( Invalid_Handle );

      FT_ਧਾਰਾ_Free( ਧਾਰਾ, external_ਧਾਰਾ );
      ਜਾਓ Fail;
    }
    ਨਹੀਂਤਾਂ
    {
      error = FT_ERR( Missing_Module );

      /* check each font driver for an appropriate format */
      cur   = library->modules;
      ਹੱਦਬੰਦੀ = cur + library->ਅੰਕ_modules;

      ਲਈ ( ; cur < ਹੱਦਬੰਦੀ; cur++ )
      {
        /* not all modules are font drivers, so check... */
        ਜੇ ( FT_MODULE_IS_DRIVER( cur[0] ) )
        {
          FT_Int         ਅੰਕ_params = 0;
          FT_Parameter*  params     = ਨਿਰਾ;


          driver = FT_DRIVER( cur[0] );

          ਜੇ ( ਦਲੀਲਾਂ->flags & FT_OPEN_PARAMS )
          {
            ਅੰਕ_params = ਦਲੀਲਾਂ->ਅੰਕ_params;
            params     = ਦਲੀਲਾਂ->params;
          }

          error = open_face( driver, &ਧਾਰਾ, &external_ਧਾਰਾ, face_ਸੂਚਕਾਂਕ,
                             ਅੰਕ_params, params, &face );
          ਜੇ ( !error )
            ਜਾਓ Success;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ FT_CONFIG_OPTION_MAC_FONTS
          ਜੇ ( test_mac_fonts                                           &&
               ft_strcmp( cur[0]->clazz->module_name, "truetype" ) == 0 &&
               FT_ERR_EQ( error, ਸਾਰਣੀ_Missing )                        )
          {
            /* TrueType but essential tables are missing */
            error = FT_ਧਾਰਾ_Seek( ਧਾਰਾ, 0 );
            ਜੇ ( error )
              ਰੋਕੋ;

            error = open_face_PS_from_sfnt_ਧਾਰਾ( library,
                                                   ਧਾਰਾ,
                                                   face_ਸੂਚਕਾਂਕ,
                                                   ਅੰਕ_params,
                                                   params,
                                                   aface );
            ਜੇ ( !error )
            {
              FT_ਧਾਰਾ_Free( ਧਾਰਾ, external_ਧਾਰਾ );
              ਵਾਪਸ error;
            }
          }
#ਜੇ_ਅੰਤ

          ਜੇ ( FT_ERR_NEQ( error, Unknown_File_Format ) )
            ਜਾਓ Fail3;
        }
      }

    Fail3:
      /* If we are on the mac, and we get an                          */
      /* FT_Err_Invalid_Stream_Operation it may be because we have an */
      /* empty data fork, so we need to check the resource fork.      */
      ਜੇ ( FT_ERR_NEQ( error, Cannot_Open_ਧਾਰਾ )       &&
           FT_ERR_NEQ( error, Unknown_File_Format )      &&
           FT_ERR_NEQ( error, Invalid_ਧਾਰਾ_Operation ) )
        ਜਾਓ Fail2;

#ਜੇ !defined( FT_MACINTOSH ) && defined( FT_CONFIG_OPTION_MAC_FONTS )
      ਜੇ ( test_mac_fonts )
      {
        error = ਲੱਦੋ_mac_face( library, ਧਾਰਾ, face_ਸੂਚਕਾਂਕ, aface, ਦਲੀਲਾਂ );
        ਜੇ ( !error )
        {
          /* We don't want to go to Success here.  We've already done   */
          /* that.  On the other hand, if we succeeded we still need to */
          /* close this stream (we opened a different stream which      */
          /* extracted the interesting information out of this stream   */
          /* here.  That stream will still be open and the face will    */
          /* point to it).                                              */
          FT_ਧਾਰਾ_Free( ਧਾਰਾ, external_ਧਾਰਾ );
          ਵਾਪਸ error;
        }
      }

      ਜੇ ( FT_ERR_NEQ( error, Unknown_File_Format ) )
        ਜਾਓ Fail2;
#ਜੇ_ਅੰਤ  /* !FT_MACINTOSH && FT_CONFIG_OPTION_MAC_FONTS */

      /* no driver is able to handle this format */
      error = FT_THROW( Unknown_File_Format );

  Fail2:
      FT_ਧਾਰਾ_Free( ਧਾਰਾ, external_ਧਾਰਾ );
      ਜਾਓ Fail;
    }

  Success:
    FT_TRACE4(( "FT_Open_Face: New face object, adding to list\n" ));

    /* add the face object to its driver's list */
    ਜੇ ( FT_QNEW( ਗੰਢ ) )
      ਜਾਓ Fail;

    ਗੰਢ->ਅੰਕੜੇ = face;
    /* don't assume driver is the same as face->driver, so use */
    /* face->driver instead.                                   */
    FT_List_ਜੋੜੋ( &face->driver->faces_list, ਗੰਢ );

    /* now allocate a glyph slot object for the face */
    FT_TRACE4(( "FT_Open_Face: Creating glyph slot\n" ));

    ਜੇ ( face_ਸੂਚਕਾਂਕ >= 0 )
    {
      error = FT_New_Glyph_ਥਾਂ( face, ਨਿਰਾ );
      ਜੇ ( error )
        ਜਾਓ Fail;

      /* finally, allocate a size object for the face */
      {
        FT_ਮਾਪ  ਮਾਪ;


        FT_TRACE4(( "FT_Open_Face: Creating size object\n" ));

        error = FT_New_ਮਾਪ( face, &ਮਾਪ );
        ਜੇ ( error )
          ਜਾਓ Fail;

        face->ਮਾਪ = ਮਾਪ;
      }
    }

    /* some checks */

    ਜੇ ( FT_IS_SCALABLE( face ) )
    {
      ਜੇ ( face->ਉਚਾਈ < 0 )
        face->ਉਚਾਈ = (FT_Short)-face->ਉਚਾਈ;

      ਜੇ ( !FT_HAS_VERTICAL( face ) )
        face->max_advance_ਉਚਾਈ = (FT_Short)face->ਉਚਾਈ;
    }

    ਜੇ ( FT_HAS_FIXED_SIZES( face ) )
    {
      FT_Int  i;


      ਲਈ ( i = 0; i < face->ਅੰਕ_fixed_sizes; i++ )
      {
        FT_Bitmap_ਮਾਪ*  bsize = face->ਉਪਲਬਧ_sizes + i;


        ਜੇ ( bsize->ਉਚਾਈ < 0 )
          bsize->ਉਚਾਈ = -bsize->ਉਚਾਈ;
        ਜੇ ( bsize->x_ppem < 0 )
          bsize->x_ppem = -bsize->x_ppem;
        ਜੇ ( bsize->y_ppem < 0 )
          bsize->y_ppem = -bsize->y_ppem;

        /* check whether negation actually has worked */
        ਜੇ ( bsize->ਉਚਾਈ < 0 || bsize->x_ppem < 0 || bsize->y_ppem < 0 )
        {
          FT_TRACE0(( "FT_Open_Face:"
                      " Invalid bitmap dimensions for strike %d,"
                      " now disabled\n", i ));
          bsize->ਚੌੜਾਈ  = 0;
          bsize->ਉਚਾਈ = 0;
          bsize->ਮਾਪ   = 0;
          bsize->x_ppem = 0;
          bsize->y_ppem = 0;
        }
      }
    }

    /* initialize internal face data */
    {
      FT_Face_ਅੰਦਰੂਨੀ  ਅੰਦਰੂਨੀ = face->ਅੰਦਰੂਨੀ;


      ਅੰਦਰੂਨੀ->transform_ਮੈਟ੍ਰਿਕਸ.xx = 0x10000L;
      ਅੰਦਰੂਨੀ->transform_ਮੈਟ੍ਰਿਕਸ.xy = 0;
      ਅੰਦਰੂਨੀ->transform_ਮੈਟ੍ਰਿਕਸ.yx = 0;
      ਅੰਦਰੂਨੀ->transform_ਮੈਟ੍ਰਿਕਸ.yy = 0x10000L;

      ਅੰਦਰੂਨੀ->transform_delta.x = 0;
      ਅੰਦਰੂਨੀ->transform_delta.y = 0;

      ਅੰਦਰੂਨੀ->refcount = 1;

      ਅੰਦਰੂਨੀ->no_stem_darkening = -1;
    }

    ਜੇ ( aface )
      *aface = face;
    ਨਹੀਂਤਾਂ
      FT_ਪੂਰਾ_Face( face );

    ਜਾਓ Exit;

  Fail:
    ਜੇ ( ਗੰਢ )
      FT_ਪੂਰਾ_Face( face );    /* face must be in the driver's list */
    ਨਹੀਂਤਾਂ ਜੇ ( face )
      destroy_face( ਸਮ੍ਰਿਤੀ, face, driver );

  Exit:
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ FT_DEBUG_LEVEL_TRACE
    ਜੇ ( !error && face_ਸੂਚਕਾਂਕ < 0 )
    {
      FT_TRACE3(( "FT_Open_Face: The font has %ld face%s\n",
                  face->ਅੰਕ_faces,
                  face->ਅੰਕ_faces == 1 ? "" : "s" ));
      FT_TRACE3(( "              and %ld named instance%s for face %ld\n",
                  face->style_flags >> 16,
                  ( face->style_flags >> 16 ) == 1 ? "" : "s",
                  -face_ਸੂਚਕਾਂਕ - 1 ));
    }
#ਜੇ_ਅੰਤ

    FT_TRACE4(( "FT_Open_Face: Return 0x%x\n", error ));

    ਵਾਪਸ error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Attach_File( FT_Face      face,
                  ਅਚਲ ਅੱਖਰ*  filepathname )
  {
    FT_Open_ਦਲੀਲਾਂ  open;


    /* test for valid `face' delayed to `FT_Attach_Stream' */

    ਜੇ ( !filepathname )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    open.ਧਾਰਾ   = ਨਿਰਾ;
    open.flags    = FT_OPEN_PATHNAME;
    open.pathname = (ਅੱਖਰ*)filepathname;

    ਵਾਪਸ FT_Attach_ਧਾਰਾ( face, &open );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Attach_ਧਾਰਾ( FT_Face              face,
                    ਅਚਲ FT_Open_ਦਲੀਲਾਂ*  parameters )
  {
    FT_ਧਾਰਾ  ਧਾਰਾ;
    FT_Error   error;
    FT_Driver  driver;

    FT_Driver_Class  clazz;


    /* test for valid `parameters' delayed to `FT_Stream_New' */

    ਜੇ ( !face )
      ਵਾਪਸ FT_THROW( Invalid_Face_Handle );

    driver = face->driver;
    ਜੇ ( !driver )
      ਵਾਪਸ FT_THROW( Invalid_Driver_Handle );

    error = FT_ਧਾਰਾ_New( driver->ਜੜ੍ਹ.library, parameters, &ਧਾਰਾ );
    ਜੇ ( error )
      ਜਾਓ Exit;

    /* we implement FT_Attach_Stream in each driver through the */
    /* `attach_file' interface                                  */

    error = FT_ERR( Unimplemented_Feature );
    clazz = driver->clazz;
    ਜੇ ( clazz->attach_file )
      error = clazz->attach_file( face, ਧਾਰਾ );

    /* close the attached stream */
    FT_ਧਾਰਾ_Free( ਧਾਰਾ,
                    FT_BOOL( parameters->ਧਾਰਾ                     &&
                             ( parameters->flags & FT_OPEN_ਧਾਰਾ ) ) );

  Exit:
    ਵਾਪਸ error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Reference_Face( FT_Face  face )
  {
    ਜੇ ( !face )
      ਵਾਪਸ FT_THROW( Invalid_Face_Handle );

    face->ਅੰਦਰੂਨੀ->refcount++;

    ਵਾਪਸ FT_Err_Ok;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_ਪੂਰਾ_Face( FT_Face  face )
  {
    FT_Error     error;
    FT_Driver    driver;
    FT_ਸਮ੍ਰਿਤੀ    ਸਮ੍ਰਿਤੀ;
    FT_List_ਗੰਢ  ਗੰਢ;


    error = FT_ERR( Invalid_Face_Handle );
    ਜੇ ( face && face->driver )
    {
      face->ਅੰਦਰੂਨੀ->refcount--;
      ਜੇ ( face->ਅੰਦਰੂਨੀ->refcount > 0 )
        error = FT_Err_Ok;
      ਨਹੀਂਤਾਂ
      {
        driver = face->driver;
        ਸਮ੍ਰਿਤੀ = driver->ਜੜ੍ਹ.ਸਮ੍ਰਿਤੀ;

        /* find face in driver's list */
        ਗੰਢ = FT_List_ਲੱਭੋ( &driver->faces_list, face );
        ਜੇ ( ਗੰਢ )
        {
          /* remove face object from the driver's list */
          FT_List_Remove( &driver->faces_list, ਗੰਢ );
          FT_FREE( ਗੰਢ );

          /* now destroy the object proper */
          destroy_face( ਸਮ੍ਰਿਤੀ, face, driver );
          error = FT_Err_Ok;
        }
      }
    }

    ਵਾਪਸ error;
  }


  /* documentation is in ftobjs.h */

  FT_EXPORT_DEF( FT_Error )
  FT_New_ਮਾਪ( FT_Face   face,
               FT_ਮਾਪ  *asize )
  {
    FT_Error         error;
    FT_ਸਮ੍ਰਿਤੀ        ਸਮ੍ਰਿਤੀ;
    FT_Driver        driver;
    FT_Driver_Class  clazz;

    FT_ਮਾਪ          ਮਾਪ = ਨਿਰਾ;
    FT_List_ਗੰਢ      ਗੰਢ = ਨਿਰਾ;

    FT_ਮਾਪ_ਅੰਦਰੂਨੀ  ਅੰਦਰੂਨੀ = ਨਿਰਾ;


    ਜੇ ( !face )
      ਵਾਪਸ FT_THROW( Invalid_Face_Handle );

    ਜੇ ( !asize )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    ਜੇ ( !face->driver )
      ਵਾਪਸ FT_THROW( Invalid_Driver_Handle );

    *asize = ਨਿਰਾ;

    driver = face->driver;
    clazz  = driver->clazz;
    ਸਮ੍ਰਿਤੀ = face->ਸਮ੍ਰਿਤੀ;

    /* Allocate new size object and perform basic initialisation */
    ਜੇ ( FT_ALLOC( ਮਾਪ, clazz->ਮਾਪ_object_ਮਾਪ ) || FT_QNEW( ਗੰਢ ) )
      ਜਾਓ Exit;

    ਮਾਪ->face = face;

    ਜੇ ( FT_NEW( ਅੰਦਰੂਨੀ ) )
      ਜਾਓ Exit;

    ਮਾਪ->ਅੰਦਰੂਨੀ = ਅੰਦਰੂਨੀ;

    ਜੇ ( clazz->init_ਮਾਪ )
      error = clazz->init_ਮਾਪ( ਮਾਪ );

    /* in case of success, add to the face's list */
    ਜੇ ( !error )
    {
      *asize     = ਮਾਪ;
      ਗੰਢ->ਅੰਕੜੇ = ਮਾਪ;
      FT_List_ਜੋੜੋ( &face->sizes_list, ਗੰਢ );
    }

  Exit:
    ਜੇ ( error )
    {
      FT_FREE( ਗੰਢ );
      ਜੇ ( ਮਾਪ )
        FT_FREE( ਮਾਪ->ਅੰਦਰੂਨੀ );
      FT_FREE( ਮਾਪ );
    }

    ਵਾਪਸ error;
  }


  /* documentation is in ftobjs.h */

  FT_EXPORT_DEF( FT_Error )
  FT_ਪੂਰਾ_ਮਾਪ( FT_ਮਾਪ  ਮਾਪ )
  {
    FT_Error     error;
    FT_Driver    driver;
    FT_ਸਮ੍ਰਿਤੀ    ਸਮ੍ਰਿਤੀ;
    FT_Face      face;
    FT_List_ਗੰਢ  ਗੰਢ;


    ਜੇ ( !ਮਾਪ )
      ਵਾਪਸ FT_THROW( Invalid_ਮਾਪ_Handle );

    face = ਮਾਪ->face;
    ਜੇ ( !face )
      ਵਾਪਸ FT_THROW( Invalid_Face_Handle );

    driver = face->driver;
    ਜੇ ( !driver )
      ਵਾਪਸ FT_THROW( Invalid_Driver_Handle );

    ਸਮ੍ਰਿਤੀ = driver->ਜੜ੍ਹ.ਸਮ੍ਰਿਤੀ;

    error = FT_Err_Ok;
    ਗੰਢ  = FT_List_ਲੱਭੋ( &face->sizes_list, ਮਾਪ );
    ਜੇ ( ਗੰਢ )
    {
      FT_List_Remove( &face->sizes_list, ਗੰਢ );
      FT_FREE( ਗੰਢ );

      ਜੇ ( face->ਮਾਪ == ਮਾਪ )
      {
        face->ਮਾਪ = ਨਿਰਾ;
        ਜੇ ( face->sizes_list.ਸਿਰ )
          face->ਮਾਪ = (FT_ਮਾਪ)(face->sizes_list.ਸਿਰ->ਅੰਕੜੇ);
      }

      destroy_ਮਾਪ( ਸਮ੍ਰਿਤੀ, ਮਾਪ, driver );
    }
    ਨਹੀਂਤਾਂ
      error = FT_THROW( Invalid_ਮਾਪ_Handle );

    ਵਾਪਸ error;
  }


  /* documentation is in ftobjs.h */

  FT_ਅਧਾਰ_DEF( FT_Error )
  FT_ਮੇਲ_ਮਾਪ( FT_Face          face,
                 FT_ਮਾਪ_Request  req,
                 FT_Bool          ignore_ਚੌੜਾਈ,
                 FT_ULong*        ਮਾਪ_ਸੂਚਕਾਂਕ )
  {
    FT_Int   i;
    FT_Long  w, h;


    ਜੇ ( !FT_HAS_FIXED_SIZES( face ) )
      ਵਾਪਸ FT_THROW( Invalid_Face_Handle );

    /* FT_Bitmap_Size doesn't provide enough info... */
    ਜੇ ( req->type != FT_ਮਾਪ_REQUEST_TYPE_NOMINAL )
      ਵਾਪਸ FT_THROW( Unimplemented_Feature );

    w = FT_REQUEST_ਚੌੜਾਈ ( req );
    h = FT_REQUEST_ਉਚਾਈ( req );

    ਜੇ ( req->ਚੌੜਾਈ && !req->ਉਚਾਈ )
      h = w;
    ਨਹੀਂਤਾਂ ਜੇ ( !req->ਚੌੜਾਈ && req->ਉਚਾਈ )
      w = h;

    w = FT_PIX_ਗੇੜਾ( w );
    h = FT_PIX_ਗੇੜਾ( h );

    ਜੇ ( !w || !h )
      ਵਾਪਸ FT_THROW( Invalid_Pixel_ਮਾਪ );

    ਲਈ ( i = 0; i < face->ਅੰਕ_fixed_sizes; i++ )
    {
      FT_Bitmap_ਮਾਪ*  bsize = face->ਉਪਲਬਧ_sizes + i;


      ਜੇ ( h != FT_PIX_ਗੇੜਾ( bsize->y_ppem ) )
        ਜਾਰੀ;

      ਜੇ ( w == FT_PIX_ਗੇੜਾ( bsize->x_ppem ) || ignore_ਚੌੜਾਈ )
      {
        FT_TRACE3(( "FT_Match_Size: bitmap strike %d matches\n", i ));

        ਜੇ ( ਮਾਪ_ਸੂਚਕਾਂਕ )
          *ਮਾਪ_ਸੂਚਕਾਂਕ = (FT_ULong)i;

        ਵਾਪਸ FT_Err_Ok;
      }
    }

    FT_TRACE3(( "FT_Match_Size: no matching bitmap strike\n" ));

    ਵਾਪਸ FT_THROW( Invalid_Pixel_ਮਾਪ );
  }


  /* documentation is in ftobjs.h */

  FT_ਅਧਾਰ_DEF( ਖਾਲੀ )
  ft_synthesize_vertical_metrics( FT_Glyph_Metrics*  metrics,
                                  FT_Pos             advance )
  {
    FT_Pos  ਉਚਾਈ = metrics->ਉਚਾਈ;


    /* compensate for glyph with bbox above/below the baseline */
    ਜੇ ( metrics->horiBearingY < 0 )
    {
      ਜੇ ( ਉਚਾਈ < metrics->horiBearingY )
        ਉਚਾਈ = metrics->horiBearingY;
    }
    ਨਹੀਂਤਾਂ ਜੇ ( metrics->horiBearingY > 0 )
      ਉਚਾਈ -= metrics->horiBearingY;

    /* the factor 1.2 is a heuristical value */
    ਜੇ ( !advance )
      advance = ਉਚਾਈ * 12 / 10;

    metrics->vertBearingX = metrics->horiBearingX - metrics->horiAdvance / 2;
    metrics->vertBearingY = ( advance - ਉਚਾਈ ) / 2;
    metrics->vertAdvance  = advance;
  }


  ਸਥਿਰ ਖਾਲੀ
  ft_recompute_scaled_metrics( FT_Face           face,
                               FT_ਮਾਪ_Metrics*  metrics )
  {
    /* Compute root ascender, descender, test height, and max_advance */

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GRID_FIT_METRICS
    metrics->ascender    = FT_PIX_CEIL( FT_MulFix( face->ascender,
                                                   metrics->y_scale ) );

    metrics->descender   = FT_PIX_FLOOR( FT_MulFix( face->descender,
                                                    metrics->y_scale ) );

    metrics->ਉਚਾਈ      = FT_PIX_ਗੇੜਾ( FT_MulFix( face->ਉਚਾਈ,
                                                    metrics->y_scale ) );

    metrics->max_advance = FT_PIX_ਗੇੜਾ( FT_MulFix( face->max_advance_ਚੌੜਾਈ,
                                                    metrics->x_scale ) );
#ਨਹੀਂਤਾਂ /* !GRID_FIT_METRICS */
    metrics->ascender    = FT_MulFix( face->ascender,
                                      metrics->y_scale );

    metrics->descender   = FT_MulFix( face->descender,
                                      metrics->y_scale );

    metrics->ਉਚਾਈ      = FT_MulFix( face->ਉਚਾਈ,
                                      metrics->y_scale );

    metrics->max_advance = FT_MulFix( face->max_advance_ਚੌੜਾਈ,
                                      metrics->x_scale );
#ਜੇ_ਅੰਤ /* !GRID_FIT_METRICS */
  }


  FT_ਅਧਾਰ_DEF( ਖਾਲੀ )
  FT_Select_Metrics( FT_Face   face,
                     FT_ULong  strike_ਸੂਚਕਾਂਕ )
  {
    FT_ਮਾਪ_Metrics*  metrics;
    FT_Bitmap_ਮਾਪ*   bsize;


    metrics = &face->ਮਾਪ->metrics;
    bsize   = face->ਉਪਲਬਧ_sizes + strike_ਸੂਚਕਾਂਕ;

    metrics->x_ppem = (FT_UShort)( ( bsize->x_ppem + 32 ) >> 6 );
    metrics->y_ppem = (FT_UShort)( ( bsize->y_ppem + 32 ) >> 6 );

    ਜੇ ( FT_IS_SCALABLE( face ) )
    {
      metrics->x_scale = FT_DivFix( bsize->x_ppem,
                                    face->units_ਪ੍ਰਤੀ_EM );
      metrics->y_scale = FT_DivFix( bsize->y_ppem,
                                    face->units_ਪ੍ਰਤੀ_EM );

      ft_recompute_scaled_metrics( face, metrics );
    }
    ਨਹੀਂਤਾਂ
    {
      metrics->x_scale     = 1L << 16;
      metrics->y_scale     = 1L << 16;
      metrics->ascender    = bsize->y_ppem;
      metrics->descender   = 0;
      metrics->ਉਚਾਈ      = bsize->ਉਚਾਈ << 6;
      metrics->max_advance = bsize->x_ppem;
    }
  }


  FT_ਅਧਾਰ_DEF( FT_Error )
  FT_Request_Metrics( FT_Face          face,
                      FT_ਮਾਪ_Request  req )
  {
    FT_Error  error = FT_Err_Ok;

    FT_ਮਾਪ_Metrics*  metrics;


    metrics = &face->ਮਾਪ->metrics;

    ਜੇ ( FT_IS_SCALABLE( face ) )
    {
      FT_Long  w = 0, h = 0, scaled_w = 0, scaled_h = 0;


      ਚੋਣ ( req->type )
      {
      ਹਾਲਤ FT_ਮਾਪ_REQUEST_TYPE_NOMINAL:
        w = h = face->units_ਪ੍ਰਤੀ_EM;
        ਰੋਕੋ;

      ਹਾਲਤ FT_ਮਾਪ_REQUEST_TYPE_REAL_DIM:
        w = h = face->ascender - face->descender;
        ਰੋਕੋ;

      ਹਾਲਤ FT_ਮਾਪ_REQUEST_TYPE_BBOX:
        w = face->bbox.xMax - face->bbox.xMin;
        h = face->bbox.yMax - face->bbox.yMin;
        ਰੋਕੋ;

      ਹਾਲਤ FT_ਮਾਪ_REQUEST_TYPE_CELL:
        w = face->max_advance_ਚੌੜਾਈ;
        h = face->ascender - face->descender;
        ਰੋਕੋ;

      ਹਾਲਤ FT_ਮਾਪ_REQUEST_TYPE_SCALES:
        metrics->x_scale = (FT_Fixed)req->ਚੌੜਾਈ;
        metrics->y_scale = (FT_Fixed)req->ਉਚਾਈ;
        ਜੇ ( !metrics->x_scale )
          metrics->x_scale = metrics->y_scale;
        ਨਹੀਂਤਾਂ ਜੇ ( !metrics->y_scale )
          metrics->y_scale = metrics->x_scale;
        ਜਾਓ Calculate_Ppem;

      ਹਾਲਤ FT_ਮਾਪ_REQUEST_TYPE_MAX:
        ਰੋਕੋ;
      }

      /* to be on the safe side */
      ਜੇ ( w < 0 )
        w = -w;

      ਜੇ ( h < 0 )
        h = -h;

      scaled_w = FT_REQUEST_ਚੌੜਾਈ ( req );
      scaled_h = FT_REQUEST_ਉਚਾਈ( req );

      /* determine scales */
      ਜੇ ( req->ਉਚਾਈ || !req->ਚੌੜਾਈ )
      {
        ਜੇ ( h == 0 )
        {
          FT_ERROR(( "FT_Request_Metrics: Divide by zero\n" ));
          error = FT_ERR( Divide_By_Zero );
          ਜਾਓ Exit;
        }

        metrics->y_scale = FT_DivFix( scaled_h, h );
      }

      ਜੇ ( req->ਚੌੜਾਈ )
      {
        ਜੇ ( w == 0 )
        {
          FT_ERROR(( "FT_Request_Metrics: Divide by zero\n" ));
          error = FT_ERR( Divide_By_Zero );
          ਜਾਓ Exit;
        }

        metrics->x_scale = FT_DivFix( scaled_w, w );
      }
      ਨਹੀਂਤਾਂ
      {
        metrics->x_scale = metrics->y_scale;
        scaled_w = FT_MulDiv( scaled_h, w, h );
      }

      ਜੇ ( !req->ਉਚਾਈ )
      {
        metrics->y_scale = metrics->x_scale;
        scaled_h = FT_MulDiv( scaled_w, h, w );
      }

      ਜੇ ( req->type == FT_ਮਾਪ_REQUEST_TYPE_CELL )
      {
        ਜੇ ( metrics->y_scale > metrics->x_scale )
          metrics->y_scale = metrics->x_scale;
        ਨਹੀਂਤਾਂ
          metrics->x_scale = metrics->y_scale;
      }

  Calculate_Ppem:
      /* calculate the ppems */
      ਜੇ ( req->type != FT_ਮਾਪ_REQUEST_TYPE_NOMINAL )
      {
        scaled_w = FT_MulFix( face->units_ਪ੍ਰਤੀ_EM, metrics->x_scale );
        scaled_h = FT_MulFix( face->units_ਪ੍ਰਤੀ_EM, metrics->y_scale );
      }

      scaled_w = ( scaled_w + 32 ) >> 6;
      scaled_h = ( scaled_h + 32 ) >> 6;
      ਜੇ ( scaled_w > (FT_Long)FT_USHORT_MAX ||
           scaled_h > (FT_Long)FT_USHORT_MAX )
      {
        FT_ERROR(( "FT_Request_Metrics: Resulting ppem size too large\n" ));
        error = FT_ERR( Invalid_Pixel_ਮਾਪ );
        ਜਾਓ Exit;
      }

      metrics->x_ppem = (FT_UShort)scaled_w;
      metrics->y_ppem = (FT_UShort)scaled_h;

      ft_recompute_scaled_metrics( face, metrics );
    }
    ਨਹੀਂਤਾਂ
    {
      FT_ZERO( metrics );
      metrics->x_scale = 1L << 16;
      metrics->y_scale = 1L << 16;
    }

  Exit:
    ਵਾਪਸ error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Select_ਮਾਪ( FT_Face  face,
                  FT_Int   strike_ਸੂਚਕਾਂਕ )
  {
    FT_Error         error = FT_Err_Ok;
    FT_Driver_Class  clazz;


    ਜੇ ( !face || !FT_HAS_FIXED_SIZES( face ) )
      ਵਾਪਸ FT_THROW( Invalid_Face_Handle );

    ਜੇ ( strike_ਸੂਚਕਾਂਕ < 0 || strike_ਸੂਚਕਾਂਕ >= face->ਅੰਕ_fixed_sizes )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    clazz = face->driver->clazz;

    ਜੇ ( clazz->select_ਮਾਪ )
    {
      error = clazz->select_ਮਾਪ( face->ਮਾਪ, (FT_ULong)strike_ਸੂਚਕਾਂਕ );

      FT_TRACE5(( "FT_Select_Size (%s driver):\n",
                  face->driver->ਜੜ੍ਹ.clazz->module_name ));
    }
    ਨਹੀਂਤਾਂ
    {
      FT_Select_Metrics( face, (FT_ULong)strike_ਸੂਚਕਾਂਕ );

      FT_TRACE5(( "FT_Select_Size:\n" ));
    }

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ FT_DEBUG_LEVEL_TRACE
    {
      FT_ਮਾਪ_Metrics*  metrics = &face->ਮਾਪ->metrics;


      FT_TRACE5(( "  x scale: %ld (%f)\n",
                  metrics->x_scale, (ਦੁੱਗਣਾ)metrics->x_scale / 65536 ));
      FT_TRACE5(( "  y scale: %ld (%f)\n",
                  metrics->y_scale, (ਦੁੱਗਣਾ)metrics->y_scale / 65536 ));
      FT_TRACE5(( "  ascender: %f\n",
                  (ਦੁੱਗਣਾ)metrics->ascender / 64 ));
      FT_TRACE5(( "  descender: %f\n",
                  (ਦੁੱਗਣਾ)metrics->descender / 64 ));
      FT_TRACE5(( "  height: %f\n",
                  (ਦੁੱਗਣਾ)metrics->ਉਚਾਈ / 64 ));
      FT_TRACE5(( "  max advance: %f\n",
                  (ਦੁੱਗਣਾ)metrics->max_advance / 64 ));
      FT_TRACE5(( "  x ppem: %d\n", metrics->x_ppem ));
      FT_TRACE5(( "  y ppem: %d\n", metrics->y_ppem ));
    }
#ਜੇ_ਅੰਤ

    ਵਾਪਸ error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Request_ਮਾਪ( FT_Face          face,
                   FT_ਮਾਪ_Request  req )
  {
    FT_Error         error;
    FT_Driver_Class  clazz;
    FT_ULong         strike_ਸੂਚਕਾਂਕ;


    ਜੇ ( !face )
      ਵਾਪਸ FT_THROW( Invalid_Face_Handle );

    ਜੇ ( !face->ਮਾਪ )
      ਵਾਪਸ FT_THROW( Invalid_ਮਾਪ_Handle );

    ਜੇ ( !req || req->ਚੌੜਾਈ < 0 || req->ਉਚਾਈ < 0 ||
         req->type >= FT_ਮਾਪ_REQUEST_TYPE_MAX )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    /* signal the auto-hinter to recompute its size metrics */
    /* (if requested)                                       */
    face->ਮਾਪ->ਅੰਦਰੂਨੀ->autohint_metrics.x_scale = 0;

    clazz = face->driver->clazz;

    ਜੇ ( clazz->request_ਮਾਪ )
    {
      error = clazz->request_ਮਾਪ( face->ਮਾਪ, req );

      FT_TRACE5(( "FT_Request_Size (%s driver):\n",
                  face->driver->ਜੜ੍ਹ.clazz->module_name ));
    }
    ਨਹੀਂਤਾਂ ਜੇ ( !FT_IS_SCALABLE( face ) && FT_HAS_FIXED_SIZES( face ) )
    {
      /*
       * The reason that a driver doesn't have `request_size' defined is
       * either that the scaling here suffices or that the supported formats
       * are bitmap-only and size matching is not implemented.
       *
       * In the latter case, a simple size matching is done.
       */
      error = FT_ਮੇਲ_ਮਾਪ( face, req, 0, &strike_ਸੂਚਕਾਂਕ );
      ਜੇ ( error )
        ਜਾਓ Exit;

      ਵਾਪਸ FT_Select_ਮਾਪ( face, (FT_Int)strike_ਸੂਚਕਾਂਕ );
    }
    ਨਹੀਂਤਾਂ
    {
      error = FT_Request_Metrics( face, req );
      ਜੇ ( error )
        ਜਾਓ Exit;

      FT_TRACE5(( "FT_Request_Size:\n" ));
    }

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ FT_DEBUG_LEVEL_TRACE
    {
      FT_ਮਾਪ_Metrics*  metrics = &face->ਮਾਪ->metrics;


      FT_TRACE5(( "  x scale: %ld (%f)\n",
                  metrics->x_scale, (ਦੁੱਗਣਾ)metrics->x_scale / 65536 ));
      FT_TRACE5(( "  y scale: %ld (%f)\n",
                  metrics->y_scale, (ਦੁੱਗਣਾ)metrics->y_scale / 65536 ));
      FT_TRACE5(( "  ascender: %f\n",
                  (ਦੁੱਗਣਾ)metrics->ascender / 64 ));
      FT_TRACE5(( "  descender: %f\n",
                  (ਦੁੱਗਣਾ)metrics->descender / 64 ));
      FT_TRACE5(( "  height: %f\n",
                  (ਦੁੱਗਣਾ)metrics->ਉਚਾਈ / 64 ));
      FT_TRACE5(( "  max advance: %f\n",
                  (ਦੁੱਗਣਾ)metrics->max_advance / 64 ));
      FT_TRACE5(( "  x ppem: %d\n", metrics->x_ppem ));
      FT_TRACE5(( "  y ppem: %d\n", metrics->y_ppem ));
    }
#ਜੇ_ਅੰਤ

  Exit:
    ਵਾਪਸ error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_ਸੈੱਟ_Char_ਮਾਪ( FT_Face     face,
                    FT_F26Dot6  char_ਚੌੜਾਈ,
                    FT_F26Dot6  char_ਉਚਾਈ,
                    FT_UInt     horz_resolution,
                    FT_UInt     vert_resolution )
  {
    FT_ਮਾਪ_Request_Rec  req;


    /* check of `face' delayed to `FT_Request_Size' */

    ਜੇ ( !char_ਚੌੜਾਈ )
      char_ਚੌੜਾਈ = char_ਉਚਾਈ;
    ਨਹੀਂਤਾਂ ਜੇ ( !char_ਉਚਾਈ )
      char_ਉਚਾਈ = char_ਚੌੜਾਈ;

    ਜੇ ( !horz_resolution )
      horz_resolution = vert_resolution;
    ਨਹੀਂਤਾਂ ਜੇ ( !vert_resolution )
      vert_resolution = horz_resolution;

    ਜੇ ( char_ਚੌੜਾਈ  < 1 * 64 )
      char_ਚੌੜਾਈ  = 1 * 64;
    ਜੇ ( char_ਉਚਾਈ < 1 * 64 )
      char_ਉਚਾਈ = 1 * 64;

    ਜੇ ( !horz_resolution )
      horz_resolution = vert_resolution = 72;

    req.type           = FT_ਮਾਪ_REQUEST_TYPE_NOMINAL;
    req.ਚੌੜਾਈ          = char_ਚੌੜਾਈ;
    req.ਉਚਾਈ         = char_ਉਚਾਈ;
    req.horiResolution = horz_resolution;
    req.vertResolution = vert_resolution;

    ਵਾਪਸ FT_Request_ਮਾਪ( face, &req );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_ਸੈੱਟ_Pixel_Sizes( FT_Face  face,
                      FT_UInt  pixel_ਚੌੜਾਈ,
                      FT_UInt  pixel_ਉਚਾਈ )
  {
    FT_ਮਾਪ_Request_Rec  req;


    /* check of `face' delayed to `FT_Request_Size' */

    ਜੇ ( pixel_ਚੌੜਾਈ == 0 )
      pixel_ਚੌੜਾਈ = pixel_ਉਚਾਈ;
    ਨਹੀਂਤਾਂ ਜੇ ( pixel_ਉਚਾਈ == 0 )
      pixel_ਉਚਾਈ = pixel_ਚੌੜਾਈ;

    ਜੇ ( pixel_ਚੌੜਾਈ  < 1 )
      pixel_ਚੌੜਾਈ  = 1;
    ਜੇ ( pixel_ਉਚਾਈ < 1 )
      pixel_ਉਚਾਈ = 1;

    /* use `>=' to avoid potential compiler warning on 16bit platforms */
    ਜੇ ( pixel_ਚੌੜਾਈ >= 0xFFFFU )
      pixel_ਚੌੜਾਈ = 0xFFFFU;
    ਜੇ ( pixel_ਉਚਾਈ >= 0xFFFFU )
      pixel_ਉਚਾਈ = 0xFFFFU;

    req.type           = FT_ਮਾਪ_REQUEST_TYPE_NOMINAL;
    req.ਚੌੜਾਈ          = (FT_Long)( pixel_ਚੌੜਾਈ << 6 );
    req.ਉਚਾਈ         = (FT_Long)( pixel_ਉਚਾਈ << 6 );
    req.horiResolution = 0;
    req.vertResolution = 0;

    ਵਾਪਸ FT_Request_ਮਾਪ( face, &req );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Kerning( FT_Face     face,
                  FT_UInt     ਖੱਬਾ_glyph,
                  FT_UInt     ਸੱਜਾ_glyph,
                  FT_UInt     kern_ਬਹੁਲਕ,
                  FT_ਸਦਿਸ਼  *akerning )
  {
    FT_Error   error = FT_Err_Ok;
    FT_Driver  driver;


    ਜੇ ( !face )
      ਵਾਪਸ FT_THROW( Invalid_Face_Handle );

    ਜੇ ( !akerning )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    driver = face->driver;

    akerning->x = 0;
    akerning->y = 0;

    ਜੇ ( driver->clazz->get_kerning )
    {
      error = driver->clazz->get_kerning( face,
                                          ਖੱਬਾ_glyph,
                                          ਸੱਜਾ_glyph,
                                          akerning );
      ਜੇ ( !error )
      {
        ਜੇ ( kern_ਬਹੁਲਕ != FT_KERNING_UNSCALED )
        {
          akerning->x = FT_MulFix( akerning->x, face->ਮਾਪ->metrics.x_scale );
          akerning->y = FT_MulFix( akerning->y, face->ਮਾਪ->metrics.y_scale );

          ਜੇ ( kern_ਬਹੁਲਕ != FT_KERNING_UNFITTED )
          {
            FT_Pos  orig_x = akerning->x;
            FT_Pos  orig_y = akerning->y;


            /* we scale down kerning values for small ppem values */
            /* to avoid that rounding makes them too big.         */
            /* `25' has been determined heuristically.            */
            ਜੇ ( face->ਮਾਪ->metrics.x_ppem < 25 )
              akerning->x = FT_MulDiv( orig_x,
                                       face->ਮਾਪ->metrics.x_ppem, 25 );
            ਜੇ ( face->ਮਾਪ->metrics.y_ppem < 25 )
              akerning->y = FT_MulDiv( orig_y,
                                       face->ਮਾਪ->metrics.y_ppem, 25 );

            akerning->x = FT_PIX_ਗੇੜਾ( akerning->x );
            akerning->y = FT_PIX_ਗੇੜਾ( akerning->y );

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ FT_DEBUG_LEVEL_TRACE
            {
              FT_Pos  orig_x_rounded = FT_PIX_ਗੇੜਾ( orig_x );
              FT_Pos  orig_y_rounded = FT_PIX_ਗੇੜਾ( orig_y );


              ਜੇ ( akerning->x != orig_x_rounded ||
                   akerning->y != orig_y_rounded )
                FT_TRACE5(( "FT_Get_Kerning: horizontal kerning"
                            " (%ld, %ld) scaled down to (%ld, %ld) pixels\n",
                            orig_x_rounded / 64, orig_y_rounded / 64,
                            akerning->x / 64, akerning->y / 64 ));
            }
#ਜੇ_ਅੰਤ
          }
        }
      }
    }

    ਵਾਪਸ error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Track_Kerning( FT_Face    face,
                        FT_Fixed   ਬਿੰਦੂ_ਮਾਪ,
                        FT_Int     degree,
                        FT_Fixed*  akerning )
  {
    FT_Service_Kerning  service;
    FT_Error            error = FT_Err_Ok;


    ਜੇ ( !face )
      ਵਾਪਸ FT_THROW( Invalid_Face_Handle );

    ਜੇ ( !akerning )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    FT_FACE_ਲੱਭੋ_SERVICE( face, service, KERNING );
    ਜੇ ( !service )
      ਵਾਪਸ FT_THROW( Unimplemented_Feature );

    error = service->get_track( face,
                                ਬਿੰਦੂ_ਮਾਪ,
                                degree,
                                akerning );

    ਵਾਪਸ error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Select_Charmap( FT_Face      face,
                     FT_Encoding  encoding )
  {
    FT_CharMap*  cur;
    FT_CharMap*  ਹੱਦਬੰਦੀ;


    ਜੇ ( !face )
      ਵਾਪਸ FT_THROW( Invalid_Face_Handle );

    /* FT_ENCODING_NONE is a valid encoding for BDF, PCF, and Windows FNT */
    ਜੇ ( encoding == FT_ENCODING_NONE && !face->ਅੰਕ_charmaps )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    /* FT_ENCODING_UNICODE is special.  We try to find the `best' Unicode */
    /* charmap available, i.e., one with UCS-4 characters, if possible.   */
    /*                                                                    */
    /* This is done by find_unicode_charmap() above, to share code.       */
    ਜੇ ( encoding == FT_ENCODING_UNICODE )
      ਵਾਪਸ ਲੱਭੋ_unicode_charmap( face );

    cur = face->charmaps;
    ਜੇ ( !cur )
      ਵਾਪਸ FT_THROW( Invalid_CharMap_Handle );

    ਹੱਦਬੰਦੀ = cur + face->ਅੰਕ_charmaps;

    ਲਈ ( ; cur < ਹੱਦਬੰਦੀ; cur++ )
    {
      ਜੇ ( cur[0]->encoding == encoding )
      {
        face->charmap = cur[0];
        ਵਾਪਸ FT_Err_Ok;
      }
    }

    ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_ਸੈੱਟ_Charmap( FT_Face     face,
                  FT_CharMap  charmap )
  {
    FT_CharMap*  cur;
    FT_CharMap*  ਹੱਦਬੰਦੀ;


    ਜੇ ( !face )
      ਵਾਪਸ FT_THROW( Invalid_Face_Handle );

    cur = face->charmaps;
    ਜੇ ( !cur || !charmap )
      ਵਾਪਸ FT_THROW( Invalid_CharMap_Handle );

    ਹੱਦਬੰਦੀ = cur + face->ਅੰਕ_charmaps;

    ਲਈ ( ; cur < ਹੱਦਬੰਦੀ; cur++ )
    {
      ਜੇ ( cur[0] == charmap )
      {
        FT_Long  format = FT_Get_CMap_Format( charmap );


        ਜੇ ( format != 14 && format != 15 )
        {
          face->charmap = cur[0];
          ਵਾਪਸ FT_Err_Ok;
        }
      }
    }

    ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Int )
  FT_Get_Charmap_ਸੂਚਕਾਂਕ( FT_CharMap  charmap )
  {
    FT_Int  i;


    ਜੇ ( !charmap || !charmap->face )
      ਵਾਪਸ -1;

    ਲਈ ( i = 0; i < charmap->face->ਅੰਕ_charmaps; i++ )
      ਜੇ ( charmap->face->charmaps[i] == charmap )
        ਰੋਕੋ;

    FT_ASSERT( i < charmap->face->ਅੰਕ_charmaps );

    ਵਾਪਸ i;
  }


  ਸਥਿਰ ਖਾਲੀ
  ft_cmap_ਪੂਰਾ_ਅੰਦਰੂਨੀ( FT_CMap  cmap )
  {
    FT_CMap_Class  clazz  = cmap->clazz;
    FT_Face        face   = cmap->charmap.face;
    FT_ਸਮ੍ਰਿਤੀ      ਸਮ੍ਰਿਤੀ = FT_FACE_ਸਮ੍ਰਿਤੀ( face );


    ਜੇ ( clazz->ਪੂਰਾ )
      clazz->ਪੂਰਾ( cmap );

    FT_FREE( cmap );
  }


  FT_ਅਧਾਰ_DEF( ਖਾਲੀ )
  FT_CMap_ਪੂਰਾ( FT_CMap  cmap )
  {
    ਜੇ ( cmap )
    {
      FT_Face    face   = cmap->charmap.face;
      FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ = FT_FACE_ਸਮ੍ਰਿਤੀ( face );
      FT_Error   error;
      FT_Int     i, j;


      ਲਈ ( i = 0; i < face->ਅੰਕ_charmaps; i++ )
      {
        ਜੇ ( (FT_CMap)face->charmaps[i] == cmap )
        {
          FT_CharMap  ਆਖਰੀ_charmap = face->charmaps[face->ਅੰਕ_charmaps - 1];


          ਜੇ ( FT_QRENEW_ARRAY( face->charmaps,
                                face->ਅੰਕ_charmaps,
                                face->ਅੰਕ_charmaps - 1 ) )
            ਵਾਪਸ;

          /* remove it from our list of charmaps */
          ਲਈ ( j = i + 1; j < face->ਅੰਕ_charmaps; j++ )
          {
            ਜੇ ( j == face->ਅੰਕ_charmaps - 1 )
              face->charmaps[j - 1] = ਆਖਰੀ_charmap;
            ਨਹੀਂਤਾਂ
              face->charmaps[j - 1] = face->charmaps[j];
          }

          face->ਅੰਕ_charmaps--;

          ਜੇ ( (FT_CMap)face->charmap == cmap )
            face->charmap = ਨਿਰਾ;

          ft_cmap_ਪੂਰਾ_ਅੰਦਰੂਨੀ( cmap );

          ਰੋਕੋ;
        }
      }
    }
  }


  FT_ਅਧਾਰ_DEF( FT_Error )
  FT_CMap_New( FT_CMap_Class  clazz,
               FT_ਸੂਚਕ     init_ਅੰਕੜੇ,
               FT_CharMap     charmap,
               FT_CMap       *acmap )
  {
    FT_Error   error;
    FT_Face    face;
    FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ;
    FT_CMap    cmap = ਨਿਰਾ;


    ਜੇ ( !clazz || !charmap || !charmap->face )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    face   = charmap->face;
    ਸਮ੍ਰਿਤੀ = FT_FACE_ਸਮ੍ਰਿਤੀ( face );

    ਜੇ ( !FT_ALLOC( cmap, clazz->ਮਾਪ ) )
    {
      cmap->charmap = *charmap;
      cmap->clazz   = clazz;

      ਜੇ ( clazz->init )
      {
        error = clazz->init( cmap, init_ਅੰਕੜੇ );
        ਜੇ ( error )
          ਜਾਓ Fail;
      }

      /* add it to our list of charmaps */
      ਜੇ ( FT_QRENEW_ARRAY( face->charmaps,
                            face->ਅੰਕ_charmaps,
                            face->ਅੰਕ_charmaps + 1 ) )
        ਜਾਓ Fail;

      face->charmaps[face->ਅੰਕ_charmaps++] = (FT_CharMap)cmap;
    }

  Exit:
    ਜੇ ( acmap )
      *acmap = cmap;

    ਵਾਪਸ error;

  Fail:
    ft_cmap_ਪੂਰਾ_ਅੰਦਰੂਨੀ( cmap );
    cmap = ਨਿਰਾ;
    ਜਾਓ Exit;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt )
  FT_Get_Char_ਸੂਚਕਾਂਕ( FT_Face   face,
                     FT_ULong  charcode )
  {
    FT_UInt  ਨਤੀਜਾ = 0;


    ਜੇ ( face && face->charmap )
    {
      FT_CMap  cmap = FT_CMAP( face->charmap );


      ਜੇ ( charcode > 0xFFFFFFFFUL )
      {
        FT_TRACE1(( "FT_Get_Char_Index: too large charcode" ));
        FT_TRACE1(( " 0x%lx is truncated\n", charcode ));
      }

      ਨਤੀਜਾ = cmap->clazz->char_ਸੂਚਕਾਂਕ( cmap, (FT_UInt32)charcode );
      ਜੇ ( ਨਤੀਜਾ >= (FT_UInt)face->ਅੰਕ_glyphs )
        ਨਤੀਜਾ = 0;
    }

    ਵਾਪਸ ਨਤੀਜਾ;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_ULong )
  FT_Get_ਪਹਿਲਾ_Char( FT_Face   face,
                     FT_UInt  *agindex )
  {
    FT_ULong  ਨਤੀਜਾ = 0;
    FT_UInt   gindex = 0;


    /* only do something if we have a charmap, and we have glyphs at all */
    ਜੇ ( face && face->charmap && face->ਅੰਕ_glyphs )
    {
      gindex = FT_Get_Char_ਸੂਚਕਾਂਕ( face, 0 );
      ਜੇ ( gindex == 0 )
        ਨਤੀਜਾ = FT_Get_ਅਗਲਾ_Char( face, 0, &gindex );
    }

    ਜੇ ( agindex )
      *agindex = gindex;

    ਵਾਪਸ ਨਤੀਜਾ;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_ULong )
  FT_Get_ਅਗਲਾ_Char( FT_Face   face,
                    FT_ULong  charcode,
                    FT_UInt  *agindex )
  {
    FT_ULong  ਨਤੀਜਾ = 0;
    FT_UInt   gindex = 0;


    ਜੇ ( face && face->charmap && face->ਅੰਕ_glyphs )
    {
      FT_UInt32  ਕੋਡ = (FT_UInt32)charcode;
      FT_CMap    cmap = FT_CMAP( face->charmap );


      ਕਰੋ
      {
        gindex = cmap->clazz->char_ਅਗਲਾ( cmap, &ਕੋਡ );

      } ਜਦੋਂ ( gindex >= (FT_UInt)face->ਅੰਕ_glyphs );

      ਨਤੀਜਾ = ( gindex == 0 ) ? 0 : ਕੋਡ;
    }

    ਜੇ ( agindex )
      *agindex = gindex;

    ਵਾਪਸ ਨਤੀਜਾ;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Face_Properties( FT_Face        face,
                      FT_UInt        ਅੰਕ_properties,
                      FT_Parameter*  properties )
  {
    FT_Error  error = FT_Err_Ok;


    ਜੇ ( ਅੰਕ_properties > 0 && !properties )
    {
      error = FT_THROW( Invalid_ਦਲੀਲ );
      ਜਾਓ Exit;
    }

    ਲਈ ( ; ਅੰਕ_properties > 0; ਅੰਕ_properties-- )
    {
      ਜੇ ( properties->ਟੈਗ == FT_PARAM_ਟੈਗ_STEM_DARKENING )
      {
        ਜੇ ( properties->ਅੰਕੜੇ )
        {
          ਜੇ ( *( (FT_Bool*)properties->ਅੰਕੜੇ ) == TRUE )
            face->ਅੰਦਰੂਨੀ->no_stem_darkening = FALSE;
          ਨਹੀਂਤਾਂ
            face->ਅੰਦਰੂਨੀ->no_stem_darkening = TRUE;
        }
        ਨਹੀਂਤਾਂ
        {
          /* use module default */
          face->ਅੰਦਰੂਨੀ->no_stem_darkening = -1;
        }
      }
      ਨਹੀਂਤਾਂ ਜੇ ( properties->ਟੈਗ == FT_PARAM_ਟੈਗ_LCD_FILTER_WEIGHTS )
      {
        error = FT_THROW( Unimplemented_Feature );
        ਜਾਓ Exit;
      }
      ਨਹੀਂਤਾਂ ਜੇ ( properties->ਟੈਗ == FT_PARAM_ਟੈਗ_RANDOM_ਬੀਜ )
      {
        ਜੇ ( properties->ਅੰਕੜੇ )
        {
          face->ਅੰਦਰੂਨੀ->random_ਬੀਜ = *( (FT_Int32*)properties->ਅੰਕੜੇ );
          ਜੇ ( face->ਅੰਦਰੂਨੀ->random_ਬੀਜ < 0 )
            face->ਅੰਦਰੂਨੀ->random_ਬੀਜ = 0;
        }
        ਨਹੀਂਤਾਂ
        {
          /* use module default */
          face->ਅੰਦਰੂਨੀ->random_ਬੀਜ = -1;
        }
      }
      ਨਹੀਂਤਾਂ
      {
        error = FT_THROW( Invalid_ਦਲੀਲ );
        ਜਾਓ Exit;
      }

      ਜੇ ( error )
        ਰੋਕੋ;

      properties++;
    }

  Exit:
    ਵਾਪਸ error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt )
  FT_Face_Get_Char_Variant_ਸੂਚਕਾਂਕ( FT_Face   face,
                               FT_ULong  charcode,
                               FT_ULong  variantSelector )
  {
    FT_UInt  ਨਤੀਜਾ = 0;


    ਜੇ ( face                                           &&
         face->charmap                                  &&
         face->charmap->encoding == FT_ENCODING_UNICODE )
    {
      FT_CharMap  charmap = ਲੱਭੋ_variant_selector_charmap( face );
      FT_CMap     ucmap = FT_CMAP( face->charmap );


      ਜੇ ( charmap )
      {
        FT_CMap  vcmap = FT_CMAP( charmap );


        ਜੇ ( charcode > 0xFFFFFFFFUL )
        {
          FT_TRACE1(( "FT_Face_GetCharVariantIndex:"
                      " too large charcode" ));
          FT_TRACE1(( " 0x%lx is truncated\n", charcode ));
        }
        ਜੇ ( variantSelector > 0xFFFFFFFFUL )
        {
          FT_TRACE1(( "FT_Face_GetCharVariantIndex:"
                      " too large variantSelector" ));
          FT_TRACE1(( " 0x%lx is truncated\n", variantSelector ));
        }

        ਨਤੀਜਾ = vcmap->clazz->char_var_ਸੂਚਕਾਂਕ( vcmap, ucmap,
                                               (FT_UInt32)charcode,
                                               (FT_UInt32)variantSelector );
      }
    }

    ਵਾਪਸ ਨਤੀਜਾ;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Int )
  FT_Face_GetCharVariantIsDefault( FT_Face   face,
                                   FT_ULong  charcode,
                                   FT_ULong  variantSelector )
  {
    FT_Int  ਨਤੀਜਾ = -1;


    ਜੇ ( face )
    {
      FT_CharMap  charmap = ਲੱਭੋ_variant_selector_charmap( face );


      ਜੇ ( charmap )
      {
        FT_CMap  vcmap = FT_CMAP( charmap );


        ਜੇ ( charcode > 0xFFFFFFFFUL )
        {
          FT_TRACE1(( "FT_Face_GetCharVariantIsDefault:"
                      " too large charcode" ));
          FT_TRACE1(( " 0x%lx is truncated\n", charcode ));
        }
        ਜੇ ( variantSelector > 0xFFFFFFFFUL )
        {
          FT_TRACE1(( "FT_Face_GetCharVariantIsDefault:"
                      " too large variantSelector" ));
          FT_TRACE1(( " 0x%lx is truncated\n", variantSelector ));
        }

        ਨਤੀਜਾ = vcmap->clazz->char_var_default( vcmap,
                                                 (FT_UInt32)charcode,
                                                 (FT_UInt32)variantSelector );
      }
    }

    ਵਾਪਸ ਨਤੀਜਾ;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt32* )
  FT_Face_GetVariantSelectors( FT_Face  face )
  {
    FT_UInt32  *ਨਤੀਜਾ = ਨਿਰਾ;


    ਜੇ ( face )
    {
      FT_CharMap  charmap = ਲੱਭੋ_variant_selector_charmap( face );


      ਜੇ ( charmap )
      {
        FT_CMap    vcmap  = FT_CMAP( charmap );
        FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ = FT_FACE_ਸਮ੍ਰਿਤੀ( face );


        ਨਤੀਜਾ = vcmap->clazz->variant_list( vcmap, ਸਮ੍ਰਿਤੀ );
      }
    }

    ਵਾਪਸ ਨਤੀਜਾ;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt32* )
  FT_Face_Get_Variants_ਦਾ_Char( FT_Face   face,
                             FT_ULong  charcode )
  {
    FT_UInt32  *ਨਤੀਜਾ = ਨਿਰਾ;


    ਜੇ ( face )
    {
      FT_CharMap  charmap = ਲੱਭੋ_variant_selector_charmap( face );


      ਜੇ ( charmap )
      {
        FT_CMap    vcmap  = FT_CMAP( charmap );
        FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ = FT_FACE_ਸਮ੍ਰਿਤੀ( face );


        ਜੇ ( charcode > 0xFFFFFFFFUL )
        {
          FT_TRACE1(( "FT_Face_GetVariantsOfChar: too large charcode" ));
          FT_TRACE1(( " 0x%lx is truncated\n", charcode ));
        }

        ਨਤੀਜਾ = vcmap->clazz->charvariant_list( vcmap, ਸਮ੍ਰਿਤੀ,
                                                 (FT_UInt32)charcode );
      }
    }
    ਵਾਪਸ ਨਤੀਜਾ;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt32* )
  FT_Face_Get_Chars_ਦਾ_Variant( FT_Face   face,
                             FT_ULong  variantSelector )
  {
    FT_UInt32  *ਨਤੀਜਾ = ਨਿਰਾ;


    ਜੇ ( face )
    {
      FT_CharMap  charmap = ਲੱਭੋ_variant_selector_charmap( face );


      ਜੇ ( charmap )
      {
        FT_CMap    vcmap  = FT_CMAP( charmap );
        FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ = FT_FACE_ਸਮ੍ਰਿਤੀ( face );


        ਜੇ ( variantSelector > 0xFFFFFFFFUL )
        {
          FT_TRACE1(( "FT_Get_Char_Index: too large variantSelector" ));
          FT_TRACE1(( " 0x%lx is truncated\n", variantSelector ));
        }

        ਨਤੀਜਾ = vcmap->clazz->variantchar_list( vcmap, ਸਮ੍ਰਿਤੀ,
                                                 (FT_UInt32)variantSelector );
      }
    }

    ਵਾਪਸ ਨਤੀਜਾ;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt )
  FT_Get_Name_ਸੂਚਕਾਂਕ( FT_Face           face,
                     ਅਚਲ FT_String*  glyph_name )
  {
    FT_UInt  ਨਤੀਜਾ = 0;


    ਜੇ ( face                       &&
         FT_HAS_GLYPH_ਨਾਂ( face ) &&
         glyph_name                 )
    {
      FT_Service_GlyphDict  service;


      FT_FACE_LOOKUP_SERVICE( face,
                              service,
                              GLYPH_DICT );

      ਜੇ ( service && service->name_ਸੂਚਕਾਂਕ )
        ਨਤੀਜਾ = service->name_ਸੂਚਕਾਂਕ( face, glyph_name );
    }

    ਵਾਪਸ ਨਤੀਜਾ;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Glyph_Name( FT_Face     face,
                     FT_UInt     glyph_ਸੂਚਕਾਂਕ,
                     FT_ਸੂਚਕ  ਬਫਰ,
                     FT_UInt     ਬਫਰ_max )
  {
    FT_Error              error;
    FT_Service_GlyphDict  service;


    ਜੇ ( !face )
      ਵਾਪਸ FT_THROW( Invalid_Face_Handle );

    ਜੇ ( !ਬਫਰ || ਬਫਰ_max == 0 )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    /* clean up buffer */
    ((FT_Byte*)ਬਫਰ)[0] = '\0';

    ਜੇ ( (FT_Long)glyph_ਸੂਚਕਾਂਕ >= face->ਅੰਕ_glyphs )
      ਵਾਪਸ FT_THROW( Invalid_Glyph_ਸੂਚਕਾਂਕ );

    ਜੇ ( !FT_HAS_GLYPH_ਨਾਂ( face ) )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    FT_FACE_LOOKUP_SERVICE( face, service, GLYPH_DICT );
    ਜੇ ( service && service->get_name )
      error = service->get_name( face, glyph_ਸੂਚਕਾਂਕ, ਬਫਰ, ਬਫਰ_max );
    ਨਹੀਂਤਾਂ
      error = FT_THROW( Invalid_ਦਲੀਲ );

    ਵਾਪਸ error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( ਅਚਲ ਅੱਖਰ* )
  FT_Get_Postscript_Name( FT_Face  face )
  {
    ਅਚਲ ਅੱਖਰ*  ਨਤੀਜਾ = ਨਿਰਾ;


    ਜੇ ( !face )
      ਜਾਓ Exit;

    ਜੇ ( !ਨਤੀਜਾ )
    {
      FT_Service_PsFontName  service;


      FT_FACE_LOOKUP_SERVICE( face,
                              service,
                              POSTSCRIPT_FONT_NAME );

      ਜੇ ( service && service->get_ps_font_name )
        ਨਤੀਜਾ = service->get_ps_font_name( face );
    }

  Exit:
    ਵਾਪਸ ਨਤੀਜਾ;
  }


  /* documentation is in tttables.h */

  FT_EXPORT_DEF( ਖਾਲੀ* )
  FT_Get_Sfnt_ਸਾਰਣੀ( FT_Face      face,
                     FT_Sfnt_ਟੈਗ  ਟੈਗ )
  {
    ਖਾਲੀ*                  ਸਾਰਣੀ = ਨਿਰਾ;
    FT_Service_SFNT_ਸਾਰਣੀ  service;


    ਜੇ ( face && FT_IS_SFNT( face ) )
    {
      FT_FACE_ਲੱਭੋ_SERVICE( face, service, SFNT_ਸਾਰਣੀ );
      ਜੇ ( service )
        ਸਾਰਣੀ = service->get_ਸਾਰਣੀ( face, ਟੈਗ );
    }

    ਵਾਪਸ ਸਾਰਣੀ;
  }


  /* documentation is in tttables.h */

  FT_EXPORT_DEF( FT_Error )
  FT_ਲੱਦੋ_Sfnt_ਸਾਰਣੀ( FT_Face    face,
                      FT_ULong   ਟੈਗ,
                      FT_Long    ਵਿੱਥ,
                      FT_Byte*   ਬਫਰ,
                      FT_ULong*  ਲੰਮਾਈ )
  {
    FT_Service_SFNT_ਸਾਰਣੀ  service;


    ਜੇ ( !face || !FT_IS_SFNT( face ) )
      ਵਾਪਸ FT_THROW( Invalid_Face_Handle );

    FT_FACE_ਲੱਭੋ_SERVICE( face, service, SFNT_ਸਾਰਣੀ );
    ਜੇ ( !service )
      ਵਾਪਸ FT_THROW( Unimplemented_Feature );

    ਵਾਪਸ service->ਲੱਦੋ_ਸਾਰਣੀ( face, ਟੈਗ, ਵਿੱਥ, ਬਫਰ, ਲੰਮਾਈ );
  }


  /* documentation is in tttables.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Sfnt_ਸਾਰਣੀ_Info( FT_Face    face,
                      FT_UInt    ਸਾਰਣੀ_ਸੂਚਕਾਂਕ,
                      FT_ULong  *ਟੈਗ,
                      FT_ULong  *ਲੰਮਾਈ )
  {
    FT_Service_SFNT_ਸਾਰਣੀ  service;
    FT_ULong               ਵਿੱਥ;


    /* test for valid `length' delayed to `service->table_info' */

    ਜੇ ( !face || !FT_IS_SFNT( face ) )
      ਵਾਪਸ FT_THROW( Invalid_Face_Handle );

    FT_FACE_ਲੱਭੋ_SERVICE( face, service, SFNT_ਸਾਰਣੀ );
    ਜੇ ( !service )
      ਵਾਪਸ FT_THROW( Unimplemented_Feature );

    ਵਾਪਸ service->ਸਾਰਣੀ_info( face, ਸਾਰਣੀ_ਸੂਚਕਾਂਕ, ਟੈਗ, &ਵਿੱਥ, ਲੰਮਾਈ );
  }


  /* documentation is in tttables.h */

  FT_EXPORT_DEF( FT_ULong )
  FT_Get_CMap_Language_ID( FT_CharMap  charmap )
  {
    FT_Service_TTCMaps  service;
    FT_Face             face;
    TT_CMapInfo         cmap_info;


    ਜੇ ( !charmap || !charmap->face )
      ਵਾਪਸ 0;

    face = charmap->face;
    FT_FACE_ਲੱਭੋ_SERVICE( face, service, TT_CMAP );
    ਜੇ ( !service )
      ਵਾਪਸ 0;
    ਜੇ ( service->get_cmap_info( charmap, &cmap_info ))
      ਵਾਪਸ 0;

    ਵਾਪਸ cmap_info.language;
  }


  /* documentation is in tttables.h */

  FT_EXPORT_DEF( FT_Long )
  FT_Get_CMap_Format( FT_CharMap  charmap )
  {
    FT_Service_TTCMaps  service;
    FT_Face             face;
    TT_CMapInfo         cmap_info;


    ਜੇ ( !charmap || !charmap->face )
      ਵਾਪਸ -1;

    face = charmap->face;
    FT_FACE_ਲੱਭੋ_SERVICE( face, service, TT_CMAP );
    ਜੇ ( !service )
      ਵਾਪਸ -1;
    ਜੇ ( service->get_cmap_info( charmap, &cmap_info ))
      ਵਾਪਸ -1;

    ਵਾਪਸ cmap_info.format;
  }


  /* documentation is in ftsizes.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Activate_ਮਾਪ( FT_ਮਾਪ  ਮਾਪ )
  {
    FT_Face  face;


    ਜੇ ( !ਮਾਪ )
      ਵਾਪਸ FT_THROW( Invalid_ਮਾਪ_Handle );

    face = ਮਾਪ->face;
    ਜੇ ( !face || !face->driver )
      ਵਾਪਸ FT_THROW( Invalid_Face_Handle );

    /* we don't need anything more complex than that; all size objects */
    /* are already listed by the face                                  */
    face->ਮਾਪ = ਮਾਪ;

    ਵਾਪਸ FT_Err_Ok;
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
  FT_ਅਧਾਰ_DEF( FT_Renderer )
  FT_Lookup_Renderer( FT_Library       library,
                      FT_Glyph_Format  format,
                      FT_List_ਗੰਢ*     ਗੰਢ )
  {
    FT_List_ਗੰਢ  cur;
    FT_Renderer  ਨਤੀਜਾ = ਨਿਰਾ;


    ਜੇ ( !library )
      ਜਾਓ Exit;

    cur = library->renderers.ਸਿਰ;

    ਜੇ ( ਗੰਢ )
    {
      ਜੇ ( *ਗੰਢ )
        cur = (*ਗੰਢ)->ਅਗਲਾ;
      *ਗੰਢ = ਨਿਰਾ;
    }

    ਜਦੋਂ ( cur )
    {
      FT_Renderer  renderer = FT_RENDERER( cur->ਅੰਕੜੇ );


      ਜੇ ( renderer->glyph_format == format )
      {
        ਜੇ ( ਗੰਢ )
          *ਗੰਢ = cur;

        ਨਤੀਜਾ = renderer;
        ਰੋਕੋ;
      }
      cur = cur->ਅਗਲਾ;
    }

  Exit:
    ਵਾਪਸ ਨਤੀਜਾ;
  }


  ਸਥਿਰ FT_Renderer
  ft_lookup_glyph_renderer( FT_Glyph_ਥਾਂ  ਥਾਂ )
  {
    FT_Face      face    = ਥਾਂ->face;
    FT_Library   library = FT_FACE_LIBRARY( face );
    FT_Renderer  ਨਤੀਜਾ  = library->cur_renderer;


    ਜੇ ( !ਨਤੀਜਾ || ਨਤੀਜਾ->glyph_format != ਥਾਂ->format )
      ਨਤੀਜਾ = FT_Lookup_Renderer( library, ਥਾਂ->format, 0 );

    ਵਾਪਸ ਨਤੀਜਾ;
  }


  ਸਥਿਰ ਖਾਲੀ
  ft_ਸੈੱਟ_current_renderer( FT_Library  library )
  {
    FT_Renderer  renderer;


    renderer = FT_Lookup_Renderer( library, FT_GLYPH_FORMAT_OUTLINE, 0 );
    library->cur_renderer = renderer;
  }


  ਸਥਿਰ FT_Error
  ft_ਜੋੜੋ_renderer( FT_Module  module )
  {
    FT_Library   library = module->library;
    FT_ਸਮ੍ਰਿਤੀ    ਸਮ੍ਰਿਤੀ  = library->ਸਮ੍ਰਿਤੀ;
    FT_Error     error;
    FT_List_ਗੰਢ  ਗੰਢ    = ਨਿਰਾ;


    ਜੇ ( FT_QNEW( ਗੰਢ ) )
      ਜਾਓ Exit;

    {
      FT_Renderer         render = FT_RENDERER( module );
      FT_Renderer_Class*  clazz  = (FT_Renderer_Class*)module->clazz;


      render->clazz        = clazz;
      render->glyph_format = clazz->glyph_format;

      /* allocate raster object if needed */
      ਜੇ ( clazz->raster_class && clazz->raster_class->raster_new )
      {
        error = clazz->raster_class->raster_new( ਸਮ੍ਰਿਤੀ, &render->raster );
        ਜੇ ( error )
          ਜਾਓ Fail;

        render->raster_render = clazz->raster_class->raster_render;
        render->render        = clazz->render_glyph;
      }

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ FT_CONFIG_OPTION_SVG
      ਜੇ ( clazz->glyph_format == FT_GLYPH_FORMAT_SVG )
        render->render = clazz->render_glyph;
#ਜੇ_ਅੰਤ

      /* add to list */
      ਗੰਢ->ਅੰਕੜੇ = module;
      FT_List_ਜੋੜੋ( &library->renderers, ਗੰਢ );

      ft_ਸੈੱਟ_current_renderer( library );
    }

  Fail:
    ਜੇ ( error )
      FT_FREE( ਗੰਢ );

  Exit:
    ਵਾਪਸ error;
  }


  ਸਥਿਰ ਖਾਲੀ
  ft_remove_renderer( FT_Module  module )
  {
    FT_Library   library;
    FT_ਸਮ੍ਰਿਤੀ    ਸਮ੍ਰਿਤੀ;
    FT_List_ਗੰਢ  ਗੰਢ;


    library = module->library;
    ਜੇ ( !library )
      ਵਾਪਸ;

    ਸਮ੍ਰਿਤੀ = library->ਸਮ੍ਰਿਤੀ;

    ਗੰਢ = FT_List_ਲੱਭੋ( &library->renderers, module );
    ਜੇ ( ਗੰਢ )
    {
      FT_Renderer  render = FT_RENDERER( module );


      /* release raster object, if any */
      ਜੇ ( render->raster )
        render->clazz->raster_class->raster_ਪੂਰਾ( render->raster );

      /* remove from list */
      FT_List_Remove( &library->renderers, ਗੰਢ );
      FT_FREE( ਗੰਢ );

      ft_ਸੈੱਟ_current_renderer( library );
    }
  }


  /* documentation is in ftrender.h */

  FT_EXPORT_DEF( FT_Renderer )
  FT_Get_Renderer( FT_Library       library,
                   FT_Glyph_Format  format )
  {
    /* test for valid `library' delayed to `FT_Lookup_Renderer' */

    ਵਾਪਸ FT_Lookup_Renderer( library, format, 0 );
  }


  /* documentation is in ftrender.h */

  FT_EXPORT_DEF( FT_Error )
  FT_ਸੈੱਟ_Renderer( FT_Library     library,
                   FT_Renderer    renderer,
                   FT_UInt        ਅੰਕ_params,
                   FT_Parameter*  parameters )
  {
    FT_List_ਗੰਢ  ਗੰਢ;
    FT_Error     error = FT_Err_Ok;

    FT_Renderer_ਸੈੱਟ_ਬਹੁਲਕ_Func  ਸੈੱਟ_ਬਹੁਲਕ;


    ਜੇ ( !library )
    {
      error = FT_THROW( Invalid_Library_Handle );
      ਜਾਓ Exit;
    }

    ਜੇ ( !renderer )
    {
      error = FT_THROW( Invalid_ਦਲੀਲ );
      ਜਾਓ Exit;
    }

    ਜੇ ( ਅੰਕ_params > 0 && !parameters )
    {
      error = FT_THROW( Invalid_ਦਲੀਲ );
      ਜਾਓ Exit;
    }

    ਗੰਢ = FT_List_ਲੱਭੋ( &library->renderers, renderer );
    ਜੇ ( !ਗੰਢ )
    {
      error = FT_THROW( Invalid_ਦਲੀਲ );
      ਜਾਓ Exit;
    }

    FT_List_ਉੱਪਰ( &library->renderers, ਗੰਢ );

    ਜੇ ( renderer->glyph_format == FT_GLYPH_FORMAT_OUTLINE )
      library->cur_renderer = renderer;

    ਸੈੱਟ_ਬਹੁਲਕ = renderer->clazz->ਸੈੱਟ_ਬਹੁਲਕ;

    ਲਈ ( ; ਅੰਕ_params > 0; ਅੰਕ_params-- )
    {
      error = ਸੈੱਟ_ਬਹੁਲਕ( renderer, parameters->ਟੈਗ, parameters->ਅੰਕੜੇ );
      ਜੇ ( error )
        ਰੋਕੋ;
      parameters++;
    }

  Exit:
    ਵਾਪਸ error;
  }


  FT_ਅਧਾਰ_DEF( FT_Error )
  FT_Render_Glyph_ਅੰਦਰੂਨੀ( FT_Library      library,
                            FT_Glyph_ਥਾਂ    ਥਾਂ,
                            FT_Render_ਬਹੁਲਕ  render_ਬਹੁਲਕ )
  {
    FT_Error     error = FT_Err_Ok;
    FT_Face      face  = ਥਾਂ->face;
    FT_Renderer  renderer;
    FT_List_ਗੰਢ  ਗੰਢ  = ਨਿਰਾ;


    /* try to render colored glyph layers as a special case */
    ਜੇ ( ਥਾਂ->ਅੰਦਰੂਨੀ->ਲੱਦੋ_flags & FT_ਲੱਦੋ_COLOR &&
         ਥਾਂ->format == FT_GLYPH_FORMAT_OUTLINE    )
    {
      FT_LayerIterator  iterator;

      FT_UInt  ਅਧਾਰ_glyph = ਥਾਂ->glyph_ਸੂਚਕਾਂਕ;

      FT_Bool  have_layers;
      FT_UInt  glyph_ਸੂਚਕਾਂਕ;
      FT_UInt  color_ਸੂਚਕਾਂਕ;


      iterator.p  = ਨਿਰਾ;
      have_layers = FT_Get_Color_Glyph_Layer( face,
                                              ਅਧਾਰ_glyph,
                                              &glyph_ਸੂਚਕਾਂਕ,
                                              &color_ਸੂਚਕਾਂਕ,
                                              &iterator );
      ਜੇ ( have_layers )
      {
        error = FT_New_Glyph_ਥਾਂ( face, ਨਿਰਾ );
        ਜੇ ( !error )
        {
          TT_Face       ttface = (TT_Face)face;
          SFNT_Service  sfnt   = (SFNT_Service)ttface->sfnt;


          ਕਰੋ
          {
            FT_Int32  ਲੱਦੋ_flags = ਥਾਂ->ਅੰਦਰੂਨੀ->ਲੱਦੋ_flags;


            /* disable the `FT_LOAD_COLOR' flag to avoid recursion */
            /* right here in this function                         */
            ਲੱਦੋ_flags &= ~FT_ਲੱਦੋ_COLOR;

            /* render into the new `face->glyph' glyph slot */
            ਲੱਦੋ_flags |= FT_ਲੱਦੋ_RENDER | FT_ਲੱਦੋ_NO_BITMAP;

            error = FT_ਲੱਦੋ_Glyph( face, glyph_ਸੂਚਕਾਂਕ, ਲੱਦੋ_flags );
            ਜੇ ( error )
              ਰੋਕੋ;

            /* blend new `face->glyph' into old `slot'; */
            /* at the first call, `slot' is still empty */
            error = sfnt->colr_blend( ttface,
                                      color_ਸੂਚਕਾਂਕ,
                                      ਥਾਂ,
                                      face->glyph );
            ਜੇ ( error )
              ਰੋਕੋ;

          } ਜਦੋਂ ( FT_Get_Color_Glyph_Layer( face,
                                              ਅਧਾਰ_glyph,
                                              &glyph_ਸੂਚਕਾਂਕ,
                                              &color_ਸੂਚਕਾਂਕ,
                                              &iterator ) );

          ਜੇ ( !error )
            ਥਾਂ->format = FT_GLYPH_FORMAT_BITMAP;

          /* this call also restores `slot' as the glyph slot */
          FT_ਪੂਰਾ_Glyph_ਥਾਂ( face->glyph );
        }

        ਜੇ ( !error )
          ਵਾਪਸ error;

        /* Failed to do the colored layer.  Draw outline instead. */
        ਥਾਂ->format = FT_GLYPH_FORMAT_OUTLINE;
      }
    }

    /* small shortcut for the very common case */
    ਜੇ ( ਥਾਂ->format == FT_GLYPH_FORMAT_OUTLINE )
    {
      renderer = library->cur_renderer;
      ਗੰਢ     = library->renderers.ਸਿਰ;
    }
    ਨਹੀਂਤਾਂ
      renderer = FT_Lookup_Renderer( library, ਥਾਂ->format, &ਗੰਢ );

    error = FT_ERR( Cannot_Render_Glyph );
    ਜਦੋਂ ( renderer )
    {
      error = renderer->render( renderer, ਥਾਂ, render_ਬਹੁਲਕ, ਨਿਰਾ );
      ਜੇ ( !error                                   ||
           FT_ERR_NEQ( error, Cannot_Render_Glyph ) )
        ਰੋਕੋ;

      /* `FT_Err_Cannot_Render_Glyph` is returned if the render mode */
      /* is unsupported by the current renderer for this glyph image */
      /* format.                                                     */

      /* now, look for another renderer that supports the same */
      /* format.                                               */
      renderer = FT_Lookup_Renderer( library, ਥਾਂ->format, &ਗੰਢ );
    }

    /* it is not an error if we cannot render a bitmap glyph */
    ਜੇ ( FT_ERR_EQ( error, Cannot_Render_Glyph ) &&
         ਥਾਂ->format == FT_GLYPH_FORMAT_BITMAP  )
      error = FT_Err_Ok;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ FT_DEBUG_LEVEL_TRACE

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ  FT_COMPONENT
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FT_COMPONENT  checksum

    /*
     * Computing the MD5 checksum is expensive, unnecessarily distorting a
     * possible profiling of FreeType if compiled with tracing support.  For
     * this reason, we execute the following code only if explicitly
     * requested.
     */

    /* we use FT_TRACE3 in this block */
    ਜੇ ( !error                               &&
         ft_trace_ਪੱਧਰ[trace_checksum] >= 3 &&
         ਥਾਂ->bitmap.ਬਫਰ                  )
    {
      FT_Bitmap  bitmap;
      FT_Error   err;


      FT_Bitmap_Init( &bitmap );

      /* we convert to a single bitmap format for computing the checksum */
      /* this also converts the bitmap flow to `down' (i.e., pitch > 0)  */
      err = FT_Bitmap_Convert( library, &ਥਾਂ->bitmap, &bitmap, 1 );
      ਜੇ ( !err )
      {
        MD5_CTX        ctx;
        ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ  md5[16];
        ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ  coverage = 0;
        ਪੂਰਨ            i, j;
        ਪੂਰਨ            ਪੰਕਤੀਆਂ  = (ਪੂਰਨ)bitmap.ਪੰਕਤੀਆਂ;
        ਪੂਰਨ            pitch = bitmap.pitch;


        FT_TRACE3(( "FT_Render_Glyph: bitmap %dx%d, %s (mode %d)\n",
                    pitch,
                    ਪੰਕਤੀਆਂ,
                    pixel_modes[ਥਾਂ->bitmap.pixel_ਬਹੁਲਕ],
                    ਥਾਂ->bitmap.pixel_ਬਹੁਲਕ ));

        ਲਈ ( i = 0; i < ਪੰਕਤੀਆਂ; i++ )
          ਲਈ ( j = 0; j < pitch; j++ )
            coverage += bitmap.ਬਫਰ[i * pitch + j];

        FT_TRACE3(( "  Total coverage: %lu\n", coverage ));

        MD5_Init( &ctx );
        ਜੇ ( bitmap.ਬਫਰ )
          MD5_Update( &ctx, bitmap.ਬਫਰ,
                      (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ)ਪੰਕਤੀਆਂ * (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ)pitch );
        MD5_Final( md5, &ctx );

        FT_TRACE3(( "  MD5 checksum: " ));
        ਲਈ ( i = 0; i < 16; i++ )
          FT_TRACE3(( "%02X", md5[i] ));
        FT_TRACE3(( "\n" ));
      }

      FT_Bitmap_ਪੂਰਾ( library, &bitmap );
    }

    /*
     * Dump bitmap in Netpbm format (PBM or PGM).
     */

    /* we use FT_TRACE7 in this block */
    ਜੇ ( !error                               &&
         ft_trace_ਪੱਧਰ[trace_checksum] >= 7 &&
         ਥਾਂ->bitmap.ਬਫਰ                  )
    {
      ਜੇ ( ਥਾਂ->bitmap.ਪੰਕਤੀਆਂ  < 128U &&
           ਥਾਂ->bitmap.ਚੌੜਾਈ < 128U )
      {
        ਪੂਰਨ  ਪੰਕਤੀਆਂ  = (ਪੂਰਨ)ਥਾਂ->bitmap.ਪੰਕਤੀਆਂ;
        ਪੂਰਨ  ਚੌੜਾਈ = (ਪੂਰਨ)ਥਾਂ->bitmap.ਚੌੜਾਈ;
        ਪੂਰਨ  pitch =      ਥਾਂ->bitmap.pitch;
        ਪੂਰਨ  i, j, m;

        ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ*  topleft = ਥਾਂ->bitmap.ਬਫਰ;


        ਜੇ ( pitch < 0 )
          topleft -= pitch * ( ਪੰਕਤੀਆਂ - 1 );

        FT_TRACE7(( "Netpbm image: start\n" ));
        ਚੋਣ ( ਥਾਂ->bitmap.pixel_ਬਹੁਲਕ )
        {
        ਹਾਲਤ FT_PIXEL_ਬਹੁਲਕ_MONO:
          FT_TRACE7(( "P1 %d %d\n", ਚੌੜਾਈ, ਪੰਕਤੀਆਂ ));
          ਲਈ ( i = 0; i < ਪੰਕਤੀਆਂ; i++ )
          {
            ਲਈ ( j = 0; j < ਚੌੜਾਈ; )
              ਲਈ ( m = 128; m > 0 && j < ਚੌੜਾਈ; m >>= 1, j++ )
                FT_TRACE7(( " %d",
                            ( topleft[i * pitch + j / 8] & m ) != 0 ));
            FT_TRACE7(( "\n" ));
          }
          ਰੋਕੋ;

        ਮੂਲ:
          FT_TRACE7(( "P2 %d %d 255\n", ਚੌੜਾਈ, ਪੰਕਤੀਆਂ ));
          ਲਈ ( i = 0; i < ਪੰਕਤੀਆਂ; i++ )
          {
            ਲਈ ( j = 0; j < ਚੌੜਾਈ; j += 1 )
              FT_TRACE7(( " %3u", topleft[i * pitch + j] ));
            FT_TRACE7(( "\n" ));
          }
        }
        FT_TRACE7(( "Netpbm image: end\n" ));
      }
      ਨਹੀਂਤਾਂ
        FT_TRACE7(( "Netpbm image: too large, omitted\n" ));
    }

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ  FT_COMPONENT
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FT_COMPONENT  objs

#ਜੇ_ਅੰਤ /* FT_DEBUG_LEVEL_TRACE */

    ਵਾਪਸ error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Render_Glyph( FT_Glyph_ਥਾਂ    ਥਾਂ,
                   FT_Render_ਬਹੁਲਕ  render_ਬਹੁਲਕ )
  {
    FT_Library  library;


    ਜੇ ( !ਥਾਂ || !ਥਾਂ->face )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    library = FT_FACE_LIBRARY( ਥਾਂ->face );

    ਵਾਪਸ FT_Render_Glyph_ਅੰਦਰੂਨੀ( library, ਥਾਂ, render_ਬਹੁਲਕ );
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
  ਸਥਿਰ ਖਾਲੀ
  Destroy_Module( FT_Module  module )
  {
    ਅਚਲ FT_Module_Class*  clazz   = module->clazz;
    FT_Library              library = module->library;
    FT_ਸਮ੍ਰਿਤੀ               ਸਮ੍ਰਿਤੀ  = module->ਸਮ੍ਰਿਤੀ;


    ਜੇ ( library && library->auto_hinter == module )
      library->auto_hinter = ਨਿਰਾ;

    /* if the module is a renderer */
    ਜੇ ( FT_MODULE_IS_RENDERER( module ) )
      ft_remove_renderer( module );

    /* if the module is a font driver, add some steps */
    ਜੇ ( FT_MODULE_IS_DRIVER( module ) )
      Destroy_Driver( FT_DRIVER( module ) );

    /* finalize the module object */
    ਜੇ ( clazz->module_ਪੂਰਾ )
      clazz->module_ਪੂਰਾ( module );

    /* discard it */
    FT_FREE( module );
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_ਜੋੜੋ_Module( FT_Library              library,
                 ਅਚਲ FT_Module_Class*  clazz )
  {
    FT_Error   error;
    FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ;
    FT_Module  module = ਨਿਰਾ;
    FT_UInt    nn;


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FREETYPE_VER_FIXED  ( ( (FT_Long)FREETYPE_MAJOR << 16 ) | \
                                FREETYPE_MINOR                  )

    ਜੇ ( !library )
      ਵਾਪਸ FT_THROW( Invalid_Library_Handle );

    ਜੇ ( !clazz )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    /* check FreeType version */
    ਜੇ ( clazz->module_requires > FREETYPE_VER_FIXED )
      ਵਾਪਸ FT_THROW( Invalid_ਸੰਸਕਰਣ );

    /* look for a module with the same name in the library's table */
    ਲਈ ( nn = 0; nn < library->ਅੰਕ_modules; nn++ )
    {
      module = library->modules[nn];
      ਜੇ ( ft_strcmp( module->clazz->module_name, clazz->module_name ) == 0 )
      {
        /* this installed module has the same name, compare their versions */
        ਜੇ ( clazz->module_ਸੰਸਕਰਣ <= module->clazz->module_ਸੰਸਕਰਣ )
          ਵਾਪਸ FT_THROW( ਹੇਠਲਾ_Module_ਸੰਸਕਰਣ );

        /* remove the module from our list, then exit the loop to replace */
        /* it by our new version..                                        */
        FT_Remove_Module( library, module );
        ਰੋਕੋ;
      }
    }

    ਸਮ੍ਰਿਤੀ = library->ਸਮ੍ਰਿਤੀ;
    error  = FT_Err_Ok;

    ਜੇ ( library->ਅੰਕ_modules >= FT_MAX_MODULES )
    {
      error = FT_THROW( Too_Many_Drivers );
      ਜਾਓ Exit;
    }

    /* allocate module object */
    ਜੇ ( FT_ALLOC( module, clazz->module_ਮਾਪ ) )
      ਜਾਓ Exit;

    /* base initialization */
    module->clazz   = clazz;
    module->library = library;
    module->ਸਮ੍ਰਿਤੀ  = ਸਮ੍ਰਿਤੀ;

    /* check whether the module is a renderer - this must be performed */
    /* before the normal module initialization                         */
    ਜੇ ( FT_MODULE_IS_RENDERER( module ) )
    {
      /* add to the renderers list */
      error = ft_ਜੋੜੋ_renderer( module );
      ਜੇ ( error )
        ਜਾਓ Fail;
    }

    /* is the module a auto-hinter? */
    ਜੇ ( FT_MODULE_IS_HINTER( module ) )
      library->auto_hinter = module;

    /* if the module is a font driver */
    ਜੇ ( FT_MODULE_IS_DRIVER( module ) )
    {
      FT_Driver  driver = FT_DRIVER( module );


      driver->clazz = (FT_Driver_Class)module->clazz;
    }

    ਜੇ ( clazz->module_init )
    {
      error = clazz->module_init( module );
      ਜੇ ( error )
        ਜਾਓ Fail;
    }

    /* add module to the library's table */
    library->modules[library->ਅੰਕ_modules++] = module;

  Exit:
    ਵਾਪਸ error;

  Fail:
    ਜੇ ( FT_MODULE_IS_RENDERER( module ) )
    {
      FT_Renderer  renderer = FT_RENDERER( module );


      ਜੇ ( renderer->clazz                                          &&
           renderer->clazz->glyph_format == FT_GLYPH_FORMAT_OUTLINE &&
           renderer->raster                                         )
        renderer->clazz->raster_class->raster_ਪੂਰਾ( renderer->raster );
    }

    FT_FREE( module );
    ਜਾਓ Exit;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Module )
  FT_Get_Module( FT_Library   library,
                 ਅਚਲ ਅੱਖਰ*  module_name )
  {
    FT_Module   ਨਤੀਜਾ = ਨਿਰਾ;
    FT_Module*  cur;
    FT_Module*  ਹੱਦਬੰਦੀ;


    ਜੇ ( !library || !module_name )
      ਵਾਪਸ ਨਤੀਜਾ;

    cur   = library->modules;
    ਹੱਦਬੰਦੀ = cur + library->ਅੰਕ_modules;

    ਲਈ ( ; cur < ਹੱਦਬੰਦੀ; cur++ )
      ਜੇ ( ft_strcmp( cur[0]->clazz->module_name, module_name ) == 0 )
      {
        ਨਤੀਜਾ = cur[0];
        ਰੋਕੋ;
      }

    ਵਾਪਸ ਨਤੀਜਾ;
  }


  /* documentation is in ftobjs.h */

  FT_ਅਧਾਰ_DEF( ਅਚਲ ਖਾਲੀ* )
  FT_Get_Module_Interface( FT_Library   library,
                           ਅਚਲ ਅੱਖਰ*  mod_name )
  {
    FT_Module  module;


    /* test for valid `library' delayed to FT_Get_Module() */

    module = FT_Get_Module( library, mod_name );

    ਵਾਪਸ module ? module->clazz->module_interface : 0;
  }


  FT_ਅਧਾਰ_DEF( FT_ਸੂਚਕ )
  ft_module_get_service( FT_Module    module,
                         ਅਚਲ ਅੱਖਰ*  service_id,
                         FT_Bool      global )
  {
    FT_ਸੂਚਕ  ਨਤੀਜਾ = ਨਿਰਾ;


    ਜੇ ( module )
    {
      FT_ASSERT( module->clazz && module->clazz->get_interface );

      /* first, look for the service in the module */
      ਜੇ ( module->clazz->get_interface )
        ਨਤੀਜਾ = module->clazz->get_interface( module, service_id );

      ਜੇ ( global && !ਨਤੀਜਾ )
      {
        /* we didn't find it, look in all other modules then */
        FT_Library  library = module->library;
        FT_Module*  cur     = library->modules;
        FT_Module*  ਹੱਦਬੰਦੀ   = cur + library->ਅੰਕ_modules;


        ਲਈ ( ; cur < ਹੱਦਬੰਦੀ; cur++ )
        {
          ਜੇ ( cur[0] != module )
          {
            FT_ASSERT( cur[0]->clazz );

            ਜੇ ( cur[0]->clazz->get_interface )
            {
              ਨਤੀਜਾ = cur[0]->clazz->get_interface( cur[0], service_id );
              ਜੇ ( ਨਤੀਜਾ )
                ਰੋਕੋ;
            }
          }
        }
      }
    }

    ਵਾਪਸ ਨਤੀਜਾ;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Remove_Module( FT_Library  library,
                    FT_Module   module )
  {
    /* try to find the module from the table, then remove it from there */

    ਜੇ ( !library )
      ਵਾਪਸ FT_THROW( Invalid_Library_Handle );

    ਜੇ ( module )
    {
      FT_Module*  cur   = library->modules;
      FT_Module*  ਹੱਦਬੰਦੀ = cur + library->ਅੰਕ_modules;


      ਲਈ ( ; cur < ਹੱਦਬੰਦੀ; cur++ )
      {
        ਜੇ ( cur[0] == module )
        {
          /* remove it from the table */
          library->ਅੰਕ_modules--;
          ਹੱਦਬੰਦੀ--;
          ਜਦੋਂ ( cur < ਹੱਦਬੰਦੀ )
          {
            cur[0] = cur[1];
            cur++;
          }
          ਹੱਦਬੰਦੀ[0] = ਨਿਰਾ;

          /* destroy the module */
          Destroy_Module( module );

          ਵਾਪਸ FT_Err_Ok;
        }
      }
    }
    ਵਾਪਸ FT_THROW( Invalid_Driver_Handle );
  }


  ਸਥਿਰ FT_Error
  ft_property_do( FT_Library        library,
                  ਅਚਲ FT_String*  module_name,
                  ਅਚਲ FT_String*  property_name,
                  ਖਾਲੀ*             ਮੁੱਲ,
                  FT_Bool           ਸੈੱਟ,
                  FT_Bool           ਮੁੱਲ_is_string )
  {
    FT_Module*           cur;
    FT_Module*           ਹੱਦਬੰਦੀ;
    FT_Module_Interface  interface;

    FT_Service_Properties  service;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ FT_DEBUG_LEVEL_ERROR
    ਅਚਲ FT_String*  ਸੈੱਟ_name  = "FT_Property_Set";
    ਅਚਲ FT_String*  get_name  = "FT_Property_Get";
    ਅਚਲ FT_String*  func_name = ਸੈੱਟ ? ਸੈੱਟ_name : get_name;
#ਜੇ_ਅੰਤ

    FT_Bool  missing_func;


    ਜੇ ( !library )
      ਵਾਪਸ FT_THROW( Invalid_Library_Handle );

    ਜੇ ( !module_name || !property_name || !ਮੁੱਲ )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

    cur   = library->modules;
    ਹੱਦਬੰਦੀ = cur + library->ਅੰਕ_modules;

    /* search module */
    ਲਈ ( ; cur < ਹੱਦਬੰਦੀ; cur++ )
      ਜੇ ( !ft_strcmp( cur[0]->clazz->module_name, module_name ) )
        ਰੋਕੋ;

    ਜੇ ( cur == ਹੱਦਬੰਦੀ )
    {
      FT_TRACE2(( "%s: can't find module `%s'\n",
                  func_name, module_name ));
      ਵਾਪਸ FT_THROW( Missing_Module );
    }

    /* check whether we have a service interface */
    ਜੇ ( !cur[0]->clazz->get_interface )
    {
      FT_TRACE2(( "%s: module `%s' doesn't support properties\n",
                  func_name, module_name ));
      ਵਾਪਸ FT_THROW( Unimplemented_Feature );
    }

    /* search property service */
    interface = cur[0]->clazz->get_interface( cur[0],
                                              FT_SERVICE_ID_PROPERTIES );
    ਜੇ ( !interface )
    {
      FT_TRACE2(( "%s: module `%s' doesn't support properties\n",
                  func_name, module_name ));
      ਵਾਪਸ FT_THROW( Unimplemented_Feature );
    }

    service = (FT_Service_Properties)interface;

    ਜੇ ( ਸੈੱਟ )
      missing_func = FT_BOOL( !service->ਸੈੱਟ_property );
    ਨਹੀਂਤਾਂ
      missing_func = FT_BOOL( !service->get_property );

    ਜੇ ( missing_func )
    {
      FT_TRACE2(( "%s: property service of module `%s' is broken\n",
                  func_name, module_name ));
      ਵਾਪਸ FT_THROW( Unimplemented_Feature );
    }

    ਵਾਪਸ ਸੈੱਟ ? service->ਸੈੱਟ_property( cur[0],
                                        property_name,
                                        ਮੁੱਲ,
                                        ਮੁੱਲ_is_string )
               : service->get_property( cur[0],
                                        property_name,
                                        ਮੁੱਲ );
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Property_ਸੈੱਟ( FT_Library        library,
                   ਅਚਲ FT_String*  module_name,
                   ਅਚਲ FT_String*  property_name,
                   ਅਚਲ ਖਾਲੀ*       ਮੁੱਲ )
  {
    ਵਾਪਸ ft_property_do( library,
                           module_name,
                           property_name,
                           (ਖਾਲੀ*)ਮੁੱਲ,
                           TRUE,
                           FALSE );
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Property_Get( FT_Library        library,
                   ਅਚਲ FT_String*  module_name,
                   ਅਚਲ FT_String*  property_name,
                   ਖਾਲੀ*             ਮੁੱਲ )
  {
    ਵਾਪਸ ft_property_do( library,
                           module_name,
                           property_name,
                           ਮੁੱਲ,
                           FALSE,
                           FALSE );
  }


#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES

  /* this variant is used for handling the FREETYPE_PROPERTIES */
  /* environment variable                                      */

  FT_ਅਧਾਰ_DEF( FT_Error )
  ft_property_string_ਸੈੱਟ( FT_Library        library,
                          ਅਚਲ FT_String*  module_name,
                          ਅਚਲ FT_String*  property_name,
                          FT_String*        ਮੁੱਲ )
  {
    ਵਾਪਸ ft_property_do( library,
                           module_name,
                           property_name,
                           (ਖਾਲੀ*)ਮੁੱਲ,
                           TRUE,
                           TRUE );
  }

#ਜੇ_ਅੰਤ


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
    ਜੇ ( !library )
      ਵਾਪਸ FT_THROW( Invalid_Library_Handle );

    library->refcount++;

    ਵਾਪਸ FT_Err_Ok;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_New_Library( FT_ਸਮ੍ਰਿਤੀ    ਸਮ੍ਰਿਤੀ,
                  FT_Library  *alibrary )
  {
    FT_Library  library = ਨਿਰਾ;
    FT_Error    error;


    ਜੇ ( !ਸਮ੍ਰਿਤੀ || !alibrary )
      ਵਾਪਸ FT_THROW( Invalid_ਦਲੀਲ );

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ FT_DEBUG_LOGGING
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ FT_DEBUG_LEVEL_ERROR
    /* init debugging support */
    ft_debug_init();
#ਜੇ_ਅੰਤ /* FT_DEBUG_LEVEL_ERROR */
#ਜੇ_ਅੰਤ /* !FT_DEBUG_LOGGING */

    /* first of all, allocate the library object */
    ਜੇ ( FT_NEW( library ) )
      ਵਾਪਸ error;

    library->ਸਮ੍ਰਿਤੀ = ਸਮ੍ਰਿਤੀ;

    library->ਸੰਸਕਰਣ_major = FREETYPE_MAJOR;
    library->ਸੰਸਕਰਣ_minor = FREETYPE_MINOR;
    library->ਸੰਸਕਰਣ_patch = FREETYPE_PATCH;

    library->refcount = 1;

    /* That's ok now */
    *alibrary = library;

    ਵਾਪਸ FT_Err_Ok;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( ਖਾਲੀ )
  FT_Library_ਸੰਸਕਰਣ( FT_Library   library,
                      FT_Int      *amajor,
                      FT_Int      *aminor,
                      FT_Int      *apatch )
  {
    FT_Int  major = 0;
    FT_Int  minor = 0;
    FT_Int  patch = 0;


    ਜੇ ( library )
    {
      major = library->ਸੰਸਕਰਣ_major;
      minor = library->ਸੰਸਕਰਣ_minor;
      patch = library->ਸੰਸਕਰਣ_patch;
    }

    ਜੇ ( amajor )
      *amajor = major;

    ਜੇ ( aminor )
      *aminor = minor;

    ਜੇ ( apatch )
      *apatch = patch;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_ਪੂਰਾ_Library( FT_Library  library )
  {
    FT_ਸਮ੍ਰਿਤੀ  ਸਮ੍ਰਿਤੀ;


    ਜੇ ( !library )
      ਵਾਪਸ FT_THROW( Invalid_Library_Handle );

    library->refcount--;
    ਜੇ ( library->refcount > 0 )
      ਜਾਓ Exit;

    ਸਮ੍ਰਿਤੀ = library->ਸਮ੍ਰਿਤੀ;

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
      ਅਚਲ ਅੱਖਰ*  driver_name[] = { "type42", ਨਿਰਾ };


      ਲਈ ( m = 0;
            m < ਆਕਾਰ ( driver_name ) / ਆਕਾਰ ( driver_name[0] );
            m++ )
      {
        ਲਈ ( n = 0; n < library->ਅੰਕ_modules; n++ )
        {
          FT_Module    module      = library->modules[n];
          ਅਚਲ ਅੱਖਰ*  module_name = module->clazz->module_name;
          FT_List      faces;


          ਜੇ ( driver_name[m]                                &&
               ft_strcmp( module_name, driver_name[m] ) != 0 )
            ਜਾਰੀ;

          ਜੇ ( ( module->clazz->module_flags & FT_MODULE_FONT_DRIVER ) == 0 )
            ਜਾਰੀ;

          FT_TRACE7(( "FT_Done_Library: close faces for %s\n", module_name ));

          faces = &FT_DRIVER( module )->faces_list;
          ਜਦੋਂ ( faces->ਸਿਰ )
          {
            FT_ਪੂਰਾ_Face( FT_FACE( faces->ਸਿਰ->ਅੰਕੜੇ ) );
            ਜੇ ( faces->ਸਿਰ )
              FT_TRACE0(( "FT_Done_Library: failed to free some faces\n" ));
          }
        }
      }
    }

    /* Close all other modules in the library */
#ਜੇ 1
    /* XXX Modules are removed in the reversed order so that  */
    /* type42 module is removed before truetype module.  This */
    /* avoids double free in some occasions.  It is a hack.   */
    ਜਦੋਂ ( library->ਅੰਕ_modules > 0 )
      FT_Remove_Module( library,
                        library->modules[library->ਅੰਕ_modules - 1] );
#ਨਹੀਂਤਾਂ
    {
      FT_UInt  n;


      ਲਈ ( n = 0; n < library->ਅੰਕ_modules; n++ )
      {
        FT_Module  module = library->modules[n];


        ਜੇ ( module )
        {
          Destroy_Module( module );
          library->modules[n] = ਨਿਰਾ;
        }
      }
    }
#ਜੇ_ਅੰਤ

    FT_FREE( library );

  Exit:
    ਵਾਪਸ FT_Err_Ok;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( ਖਾਲੀ )
  FT_ਸੈੱਟ_Debug_Hook( FT_Library         library,
                     FT_UInt            hook_ਸੂਚਕਾਂਕ,
                     FT_DebugHook_Func  debug_hook )
  {
    ਜੇ ( library && debug_hook &&
         hook_ਸੂਚਕਾਂਕ <
           ( ਆਕਾਰ ( library->debug_hooks ) / ਆਕਾਰ ( ਖਾਲੀ* ) ) )
      library->debug_hooks[hook_ਸੂਚਕਾਂਕ] = debug_hook;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_TrueTypeEngineType )
  FT_Get_TrueType_Engine_Type( FT_Library  library )
  {
    FT_TrueTypeEngineType  ਨਤੀਜਾ = FT_TRUETYPE_ENGINE_TYPE_NONE;


    ਜੇ ( library )
    {
      FT_Module  module = FT_Get_Module( library, "truetype" );


      ਜੇ ( module )
      {
        FT_Service_TrueTypeEngine  service;


        service = (FT_Service_TrueTypeEngine)
                    ft_module_get_service( module,
                                           FT_SERVICE_ID_TRUETYPE_ENGINE,
                                           0 );
        ਜੇ ( service )
          ਨਤੀਜਾ = service->engine_type;
      }
    }

    ਵਾਪਸ ਨਤੀਜਾ;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_SubGlyph_Info( FT_Glyph_ਥਾਂ  glyph,
                        FT_UInt       sub_ਸੂਚਕਾਂਕ,
                        FT_Int       *p_ਸੂਚਕਾਂਕ,
                        FT_UInt      *p_flags,
                        FT_Int       *p_arg1,
                        FT_Int       *p_arg2,
                        FT_ਮੈਟ੍ਰਿਕਸ    *p_transform )
  {
    FT_Error  error = FT_ERR( Invalid_ਦਲੀਲ );


    ਜੇ ( glyph                                      &&
         glyph->subglyphs                           &&
         glyph->format == FT_GLYPH_FORMAT_ਸੰਯੁਕਤ &&
         sub_ਸੂਚਕਾਂਕ < glyph->ਅੰਕ_subglyphs           )
    {
      FT_SubGlyph  subg = glyph->subglyphs + sub_ਸੂਚਕਾਂਕ;


      *p_ਸੂਚਕਾਂਕ     = subg->index;
      *p_flags     = subg->flags;
      *p_arg1      = subg->arg1;
      *p_arg2      = subg->arg2;
      *p_transform = subg->transform;

      error = FT_Err_Ok;
    }

    ਵਾਪਸ error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Get_Color_Glyph_Layer( FT_Face            face,
                            FT_UInt            ਅਧਾਰ_glyph,
                            FT_UInt           *aglyph_ਸੂਚਕਾਂਕ,
                            FT_UInt           *acolor_ਸੂਚਕਾਂਕ,
                            FT_LayerIterator*  iterator )
  {
    TT_Face       ttface;
    SFNT_Service  sfnt;


    ਜੇ ( !face                                   ||
         !aglyph_ਸੂਚਕਾਂਕ                           ||
         !acolor_ਸੂਚਕਾਂਕ                           ||
         !iterator                               ||
         ਅਧਾਰ_glyph >= (FT_UInt)face->ਅੰਕ_glyphs )
      ਵਾਪਸ 0;

    ਜੇ ( !FT_IS_SFNT( face ) )
      ਵਾਪਸ 0;

    ttface = (TT_Face)face;
    sfnt   = (SFNT_Service)ttface->sfnt;

    ਜੇ ( sfnt->get_colr_layer )
      ਵਾਪਸ sfnt->get_colr_layer( ttface,
                                   ਅਧਾਰ_glyph,
                                   aglyph_ਸੂਚਕਾਂਕ,
                                   acolor_ਸੂਚਕਾਂਕ,
                                   iterator );
    ਨਹੀਂਤਾਂ
      ਵਾਪਸ 0;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Get_Color_Glyph_Paint( FT_Face                  face,
                            FT_UInt                  ਅਧਾਰ_glyph,
                            FT_Color_ਜੜ੍ਹ_Transform  ਜੜ੍ਹ_transform,
                            FT_OpaquePaint*          paint )
  {
    TT_Face       ttface;
    SFNT_Service  sfnt;


    ਜੇ ( !face || !paint )
      ਵਾਪਸ 0;

    ਜੇ ( !FT_IS_SFNT( face ) )
      ਵਾਪਸ 0;

    ttface = (TT_Face)face;
    sfnt   = (SFNT_Service)ttface->sfnt;

    ਜੇ ( sfnt->get_colr_glyph_paint )
      ਵਾਪਸ sfnt->get_colr_glyph_paint( ttface,
                                         ਅਧਾਰ_glyph,
                                         ਜੜ੍ਹ_transform,
                                         paint );
    ਨਹੀਂਤਾਂ
      ਵਾਪਸ 0;
  }


  /* documentation is in ftcolor.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Get_Color_Glyph_ClipBox( FT_Face      face,
                              FT_UInt      ਅਧਾਰ_glyph,
                              FT_ClipBox*  clip_box )
  {
    TT_Face       ttface;
    SFNT_Service  sfnt;


    ਜੇ ( !face || !clip_box )
      ਵਾਪਸ 0;

    ਜੇ ( !FT_IS_SFNT( face ) )
      ਵਾਪਸ 0;

    ttface = (TT_Face)face;
    sfnt   = (SFNT_Service)ttface->sfnt;

    ਜੇ ( sfnt->get_color_glyph_clipbox )
      ਵਾਪਸ sfnt->get_color_glyph_clipbox( ttface,
                                            ਅਧਾਰ_glyph,
                                            clip_box );
    ਨਹੀਂਤਾਂ
      ਵਾਪਸ 0;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Get_Paint_Layers( FT_Face            face,
                       FT_LayerIterator*  layer_iterator,
                       FT_OpaquePaint*    paint )
  {
    TT_Face       ttface;
    SFNT_Service  sfnt;


    ਜੇ ( !face || !paint || !layer_iterator )
      ਵਾਪਸ 0;

    ਜੇ ( !FT_IS_SFNT( face ) )
      ਵਾਪਸ 0;

    ttface = (TT_Face)face;
    sfnt   = (SFNT_Service)ttface->sfnt;

    ਜੇ ( sfnt->get_paint_layers )
      ਵਾਪਸ sfnt->get_paint_layers( ttface, layer_iterator, paint );
    ਨਹੀਂਤਾਂ
      ਵਾਪਸ 0;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Get_Paint( FT_Face face,
                FT_OpaquePaint  opaque_paint,
                FT_COLR_Paint*  paint )
  {
    TT_Face       ttface;
    SFNT_Service  sfnt;


    ਜੇ ( !face || !paint )
      ਵਾਪਸ 0;

    ਜੇ ( !FT_IS_SFNT( face ) )
      ਵਾਪਸ 0;

    ttface = (TT_Face)face;
    sfnt   = (SFNT_Service)ttface->sfnt;

    ਜੇ ( sfnt->get_paint )
      ਵਾਪਸ sfnt->get_paint( ttface, opaque_paint, paint );
    ਨਹੀਂਤਾਂ
      ਵਾਪਸ 0;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Get_Colorline_Stops ( FT_Face                face,
                           FT_ColorStop *         color_stop,
                           FT_ColorStopIterator  *iterator )
  {
    TT_Face       ttface;
    SFNT_Service  sfnt;


    ਜੇ ( !face || !color_stop || !iterator )
      ਵਾਪਸ 0;

    ਜੇ ( !FT_IS_SFNT( face ) )
      ਵਾਪਸ 0;

    ttface = (TT_Face)face;
    sfnt   = (SFNT_Service)ttface->sfnt;

    ਜੇ ( sfnt->get_colorline_stops )
      ਵਾਪਸ sfnt->get_colorline_stops ( ttface, color_stop, iterator );
    ਨਹੀਂਤਾਂ
      ਵਾਪਸ 0;
  }


/*ਸਿਰਾ*/
