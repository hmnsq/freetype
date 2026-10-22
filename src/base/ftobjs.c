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


#ಅನ್ತರ್ಭಾವ <freetype/ftlist.h>
#ಅನ್ತರ್ಭಾವ <freetype/ftoutln.h>
#ಅನ್ತರ್ಭಾವ <freetype/ftfntfmt.h>
#ಅನ್ತರ್ಭಾವ <freetype/otsvg.h>

#ಅನ್ತರ್ಭಾವ <freetype/internal/ftvalid.h>
#ಅನ್ತರ್ಭಾವ <freetype/internal/ftobjs.h>
#ಅನ್ತರ್ಭಾವ <freetype/internal/ftdebug.h>
#ಅನ್ತರ್ಭಾವ <freetype/internal/ftrfork.h>
#ಅನ್ತರ್ಭಾವ <freetype/internal/ftstream.h>
#ಅನ್ತರ್ಭಾವ <freetype/internal/sfnt.h>          /* for SFNT_Load_Table_Func */
#ಅನ್ತರ್ಭಾವ <freetype/internal/psaux.h>         /* for PS_Driver            */
#ಅನ್ತರ್ಭಾವ <freetype/internal/svginterface.h>

#ಅನ್ತರ್ಭಾವ <freetype/tttables.h>
#ಅನ್ತರ್ಭಾವ <freetype/tttags.h>
#ಅನ್ತರ್ಭಾವ <freetype/ttnameid.h>

#ಅನ್ತರ್ಭಾವ <freetype/internal/services/svprop.h>
#ಅನ್ತರ್ಭಾವ <freetype/internal/services/svsfnt.h>
#ಅನ್ತರ್ಭಾವ <freetype/internal/services/svpostnm.h>
#ಅನ್ತರ್ಭಾವ <freetype/internal/services/svgldict.h>
#ಅನ್ತರ್ಭಾವ <freetype/internal/services/svttcmap.h>
#ಅನ್ತರ್ಭಾವ <freetype/internal/services/svkern.h>
#ಅನ್ತರ್ಭಾವ <freetype/internal/services/svtteng.h>

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ TT_CONFIG_OPTION_VARC
#ಅನ್ತರ್ಭಾವ <freetype/internal/services/svvarc.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ <freetype/ftdriver.h>

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ FT_CONFIG_OPTION_MAC_FONTS
#ಅನ್ತರ್ಭಾವ "ftbase.h"
#ಆದರೆ_ಅಂತ್ಯ


#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ FT_DEBUG_LEVEL_TRACE

#ಅನ್ತರ್ಭಾವ <freetype/ftbitmap.h>

#ಆದರೆ defined( _MSC_VER )      /* Visual C++ (and Intel C++)   */
  /* We disable the warning `conversion from XXX to YYY,     */
  /* possible loss of data' in order to compile cleanly with */
  /* the maximum level of warnings: `md5.c' is non-FreeType  */
  /* code, and it gets used during development builds only.  */
#ಸೂಚನೆ warning( ತಳ್ಳು )
#ಸೂಚನೆ warning( disable : 4244 )
#ಆದರೆ_ಅಂತ್ಯ /* _MSC_VER */

  /* It's easiest to include `md5.c' directly.  However, since OpenSSL */
  /* also provides the same functions, there might be conflicts if     */
  /* both FreeType and OpenSSL are built as static libraries.  For     */
  /* this reason, we put the MD5 stuff into the `FT_' namespace.       */
#ವ್ಯಾಖ್ಯಾನಿಸು MD5_u32plus  FT_MD5_u32plus
#ವ್ಯಾಖ್ಯಾನಿಸು MD5_CTX      FT_MD5_CTX
#ವ್ಯಾಖ್ಯಾನಿಸು MD5_Init     FT_MD5_Init
#ವ್ಯಾಖ್ಯಾನಿಸು MD5_Update   FT_MD5_Update
#ವ್ಯಾಖ್ಯಾನಿಸು MD5_Final    FT_MD5_Final

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು  HAVE_OPENSSL

#ಅನ್ತರ್ಭಾವ "md5.c"

#ಆದರೆ defined( _MSC_VER )
#ಸೂಚನೆ warning( ತೆಗೆ )
#ಆದರೆ_ಅಂತ್ಯ

  /* This array must stay in sync with the @FT_Pixel_Mode enumeration */
  /* (in file `ftimage.h`).                                           */

  ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ಸ್ಥಿರಾಂಕ  pixel_modes[] =
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

#ಆದರೆ_ಅಂತ್ಯ /* FT_DEBUG_LEVEL_TRACE */


#ವ್ಯಾಖ್ಯಾನಿಸು GRID_FIT_METRICS


  /* forward declaration */
  ಸ್ಥಿರ FT_Error
  ft_open_face_ಆಂತರಿಕ( FT_Library           library,
                         ಸ್ಥಿರಾಂಕ FT_Open_ವಾದಗಳು*  ವಾದಗಳು,
                         FT_Long              face_ಸೂಚ್ಯಂಕ,
                         FT_Face             *aface,
                         FT_Bool              test_mac_fonts );


  FT_ತಳ_DEF( FT_ಸೂಚಕ )
  ft_service_list_lookup( FT_ServiceDesc  service_descriptors,
                          ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*     service_id )
  {
    FT_ಸೂಚಕ      ಫಲಿತಾಂಶ = ಖಾಲಿ;
    FT_ServiceDesc  desc   = service_descriptors;


    ಆದರೆ ( desc && service_id )
    {
      ಗಾಗಿ ( ; desc->serv_id != ಖಾಲಿ; desc++ )
      {
        ಆದರೆ ( ft_strcmp( desc->serv_id, service_id ) == 0 )
        {
          ಫಲಿತಾಂಶ = (FT_ಸೂಚಕ)desc->serv_ದತ್ತಾಂಶ;
          ನಿಲ್ಲಿಸು;
        }
      }
    }

    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
  }


  FT_ತಳ_DEF( ಶೂನ್ಯ )
  ft_validator_init( FT_Validator        valid,
                     ಸ್ಥಿರಾಂಕ FT_Byte*      ತಳ,
                     ಸ್ಥಿರಾಂಕ FT_Byte*      ಎಲ್ಲೆ,
                     FT_ValidationLevel  level )
  {
    valid->ತಳ  = ತಳ;
    valid->ಎಲ್ಲೆ = ಎಲ್ಲೆ;
    valid->level = level;
    valid->error = FT_Err_Ok;
  }


  FT_ತಳ_DEF( FT_Int )
  ft_validator_ಸತತ( FT_Validator  valid )
  {
    /* This function doesn't work!  None should call it. */
    FT_ಬಳಸದ( valid );

    ಹಿಂತಿರುಗಿ -1;
  }


  FT_ತಳ_DEF( ಶೂನ್ಯ )
  ft_validator_error( FT_Validator  valid,
                      FT_Error      error )
  {
    /* since the cast below also disables the compiler's */
    /* type check, we introduce a dummy variable, which  */
    /* will be optimized away                            */
    ಅಸ್ಥಿರ ft_jmp_buf* jump_ಬಫರ್ = &valid->jump_ಬಫರ್;


    valid->error = error;

    /* throw away volatileness; use `jump_buffer' or the  */
    /* compiler may warn about an unused local variable   */
    ft_longjmp( *(ft_jmp_buf*) jump_ಬಫರ್, 1 );
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
  FT_ತಳ_DEF( FT_Error )
  FT_ಹರಿವು_New( FT_Library           library,
                 ಸ್ಥಿರಾಂಕ FT_Open_ವಾದಗಳು*  ವಾದಗಳು,
                 FT_ಹರಿವು           *astream )
  {
    FT_Error   error;
    FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ;
    FT_ಹರಿವು  ಹರಿವು = ಖಾಲಿ;
    FT_UInt    ಬಹುಲಕ;


    *astream = ಖಾಲಿ;

    ಆದರೆ ( !library )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_Library_Handle );

    ಆದರೆ ( !ವಾದಗಳು )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

    ಸ್ಮರಣೆ = library->ಸ್ಮರಣೆ;
    ಬಹುಲಕ   = ವಾದಗಳು->flags &
               ( FT_OPEN_ಸ್ಮರಣೆ | FT_OPEN_ಹರಿವು | FT_OPEN_PATHNAME );

    ಆದರೆ ( ಬಹುಲಕ == FT_OPEN_ಸ್ಮರಣೆ )
    {
      /* create a memory-based stream */
      ಆದರೆ ( FT_NEW( ಹರಿವು ) )
        ಹೋಗು Exit;

      FT_ಹರಿವು_Open_ಸ್ಮರಣೆ( ಹರಿವು,
                            (ಸ್ಥಿರಾಂಕ FT_Byte*)ವಾದಗಳು->ಸ್ಮರಣೆ_ತಳ,
                            (FT_ULong)ವಾದಗಳು->ಸ್ಮರಣೆ_ಗಾತ್ರ );
      ಹರಿವು->ಸ್ಮರಣೆ = ಸ್ಮರಣೆ;
    }

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ FT_CONFIG_OPTION_DISABLE_ಹರಿವು_SUPPORT

    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ( ಬಹುಲಕ == FT_OPEN_PATHNAME )
    {
      /* create a normal system stream */
      ಆದರೆ ( FT_NEW( ಹರಿವು ) )
        ಹೋಗು Exit;

      ಹರಿವು->ಸ್ಮರಣೆ = ಸ್ಮರಣೆ;
      error = FT_ಹರಿವು_Open( ಹರಿವು, ವಾದಗಳು->pathname );
      ಆದರೆ ( error )
        FT_FREE( ಹರಿವು );
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ( ( ಬಹುಲಕ == FT_OPEN_ಹರಿವು ) && ವಾದಗಳು->ಹರಿವು )
    {
      /* use an existing, user-provided stream */

      /* in this case, we do not need to allocate a new stream object */
      /* since the caller is responsible for closing it himself       */
      ಹರಿವು         = ವಾದಗಳು->ಹರಿವು;
      ಹರಿವು->ಸ್ಮರಣೆ = ಸ್ಮರಣೆ;
      error          = FT_Err_Ok;
    }

#ಆದರೆ_ಅಂತ್ಯ

    ಇಲ್ಲದಿದ್ದರೆ
    {
      error = FT_THROW( Invalid_ವಾದ );
      ಆದರೆ ( ( ವಾದಗಳು->flags & FT_OPEN_ಹರಿವು ) && ವಾದಗಳು->ಹರಿವು )
        FT_ಹರಿವು_Close( ವಾದಗಳು->ಹರಿವು );
    }

    ಆದರೆ ( !error )
      *astream       = ಹರಿವು;

  Exit:
    ಹಿಂತಿರುಗಿ error;
  }


  FT_ತಳ_DEF( ಶೂನ್ಯ )
  FT_ಹರಿವು_Free( FT_ಹರಿವು  ಹರಿವು,
                  FT_Int     external )
  {
    ಆದರೆ ( ಹರಿವು )
    {
      FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ = ಹರಿವು->ಸ್ಮರಣೆ;


      FT_ಹರಿವು_Close( ಹರಿವು );

      ಆದರೆ ( !external )
        FT_FREE( ಹರಿವು );
    }
  }


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು  FT_COMPONENT
#ವ್ಯಾಖ್ಯಾನಿಸು FT_COMPONENT  objs


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


  ಸ್ಥಿರ FT_Error
  ft_glyphslot_init( FT_Glyph_ಸ್ಥಾನ  ಸ್ಥಾನ )
  {
    FT_Driver         driver   = ಸ್ಥಾನ->face->driver;
    FT_Driver_Class   clazz    = driver->clazz;
    FT_ಸ್ಮರಣೆ         ಸ್ಮರಣೆ   = driver->ಬೇರು.ಸ್ಮರಣೆ;
    FT_Error          error    = FT_Err_Ok;
    FT_ಸ್ಥಾನ_ಆಂತರಿಕ  ಆಂತರಿಕ = ಖಾಲಿ;


    ಸ್ಥಾನ->library = driver->ಬೇರು.library;

    ಆದರೆ ( FT_NEW( ಆಂತರಿಕ ) )
      ಹೋಗು Exit;

    ಸ್ಥಾನ->ಆಂತರಿಕ = ಆಂತರಿಕ;

    ಆದರೆ ( FT_DRIVER_ಬಳಸುತ್ತದೆ_OUTLINES( driver ) )
      error = FT_GlyphLoader_New( ಸ್ಮರಣೆ, &ಆಂತರಿಕ->loader );

    ಆದರೆ ( !error && clazz->init_ಸ್ಥಾನ )
      error = clazz->init_ಸ್ಥಾನ( ಸ್ಥಾನ );

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ FT_CONFIG_OPTION_SVG
    /* if SVG table exists, allocate the space in `slot->other` */
    ಆದರೆ ( ಸ್ಥಾನ->face->face_flags & FT_FACE_FLAG_SVG )
    {
      FT_SVG_Document  document = ಖಾಲಿ;


      ಆದರೆ ( FT_NEW( document ) )
        ಹೋಗು Exit;
      ಸ್ಥಾನ->other = document;
    }
#ಆದರೆ_ಅಂತ್ಯ

  Exit:
    ಹಿಂತಿರುಗಿ error;
  }


  FT_ತಳ_DEF( ಶೂನ್ಯ )
  ft_glyphslot_free_bitmap( FT_Glyph_ಸ್ಥಾನ  ಸ್ಥಾನ )
  {
    ಆದರೆ ( ಸ್ಥಾನ->ಆಂತರಿಕ && ( ಸ್ಥಾನ->ಆಂತರಿಕ->flags & FT_GLYPH_OWN_BITMAP ) )
    {
      FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ = FT_FACE_ಸ್ಮರಣೆ( ಸ್ಥಾನ->face );


      FT_FREE( ಸ್ಥಾನ->bitmap.ಬಫರ್ );
      ಸ್ಥಾನ->ಆಂತರಿಕ->flags &= ~FT_GLYPH_OWN_BITMAP;
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
      /* assume that the bitmap buffer was stolen or not */
      /* allocated from the heap                         */
      ಸ್ಥಾನ->bitmap.ಬಫರ್ = ಖಾಲಿ;
    }
  }


  /* overflow-resistant presetting of bitmap position and dimensions; */
  /* also check whether the size is too large for rendering           */
  FT_ತಳ_DEF( FT_Bool )
  ft_glyphslot_preset_bitmap( FT_Glyph_ಸ್ಥಾನ      ಸ್ಥಾನ,
                              FT_Render_ಬಹುಲಕ    ಬಹುಲಕ,
                              ಸ್ಥಿರಾಂಕ FT_ಸದಿಶ*  origin )
  {
    FT_Outline*  outline = &ಸ್ಥಾನ->outline;
    FT_Bitmap*   bitmap  = &ಸ್ಥಾನ->bitmap;

    FT_Pixel_ಬಹುಲಕ  pixel_ಬಹುಲಕ;

    FT_BBox  cbox, pbox;
    FT_Pos   x_ಸರಿಸು = 0;
    FT_Pos   y_ಸರಿಸು = 0;
    FT_Pos   x_ಎಡ, y_ತುದಿ;
    FT_Pos   ಅಗಲ, ಎತ್ತರ, pitch;
    FT_Bool  ret;


    ಆದರೆ ( ಸ್ಥಾನ->format == FT_GLYPH_FORMAT_SVG )
    {
      FT_Module    module;
      SVG_Service  svg_service;


      module      = FT_Get_Module( ಸ್ಥಾನ->library, "ot-svg" );
      svg_service = (SVG_Service)module->clazz->module_interface;

      ಹಿಂತಿರುಗಿ (FT_Bool)svg_service->preset_ಸ್ಥಾನ( module, ಸ್ಥಾನ, FALSE );
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ( ಸ್ಥಾನ->format != FT_GLYPH_FORMAT_OUTLINE )
      ಹಿಂತಿರುಗಿ 1;

    ಆದರೆ ( origin )
    {
      x_ಸರಿಸು = origin->x;
      y_ಸರಿಸು = origin->y;
    }

    /* compute the control box, and grid-fit it, */
    /* taking into account the origin shift      */
    FT_Outline_Get_CBox( outline, &cbox );

    /* rough estimate of pixel box */
    pbox.xMin = ( cbox.xMin >> 6 ) + ( x_ಸರಿಸು >> 6 );
    pbox.yMin = ( cbox.yMin >> 6 ) + ( y_ಸರಿಸು >> 6 );
    pbox.xMax = ( cbox.xMax >> 6 ) + ( x_ಸರಿಸು >> 6 );
    pbox.yMax = ( cbox.yMax >> 6 ) + ( y_ಸರಿಸು >> 6 );

    /* tiny remainder box */
    cbox.xMin = ( cbox.xMin & 63 ) + ( x_ಸರಿಸು & 63 );
    cbox.yMin = ( cbox.yMin & 63 ) + ( y_ಸರಿಸು & 63 );
    cbox.xMax = ( cbox.xMax & 63 ) + ( x_ಸರಿಸು & 63 );
    cbox.yMax = ( cbox.yMax & 63 ) + ( y_ಸರಿಸು & 63 );

    ಆಯ್ಕೆ ( ಬಹುಲಕ )
    {
    ಸ್ಥಿತಿ FT_RENDER_ಬಹುಲಕ_MONO:
      pixel_ಬಹುಲಕ = FT_PIXEL_ಬಹುಲಕ_MONO;
#ಆದರೆ 1
      /* x */

      /* undocumented but confirmed: bbox values get rounded;    */
      /* we do asymmetric rounding so that the center of a pixel */
      /* gets always included                                    */

      pbox.xMin += ( cbox.xMin + 31 ) >> 6;
      pbox.xMax += ( cbox.xMax + 32 ) >> 6;

      /* if the bbox collapsed, we add a pixel based on the total */
      /* rounding remainder to cover most of the original cbox    */

      ಆದರೆ ( pbox.xMin == pbox.xMax )
      {
        ಆದರೆ ( ( ( cbox.xMin + 31 ) & 63 ) - 31 +
             ( ( cbox.xMax + 32 ) & 63 ) - 32 < 0 )
          pbox.xMin -= 1;
        ಇಲ್ಲದಿದ್ದರೆ
          pbox.xMax += 1;
      }

      /* y */

      pbox.yMin += ( cbox.yMin + 31 ) >> 6;
      pbox.yMax += ( cbox.yMax + 32 ) >> 6;

      ಆದರೆ ( pbox.yMin == pbox.yMax )
      {
        ಆದರೆ ( ( ( cbox.yMin + 31 ) & 63 ) - 31 +
             ( ( cbox.yMax + 32 ) & 63 ) - 32 < 0 )
          pbox.yMin -= 1;
        ಇಲ್ಲದಿದ್ದರೆ
          pbox.yMax += 1;
      }

      ನಿಲ್ಲಿಸು;
#ಇಲ್ಲದಿದ್ದರೆ
      ಹೋಗು Adjust;
#ಆದರೆ_ಅಂತ್ಯ

    ಸ್ಥಿತಿ FT_RENDER_ಬಹುಲಕ_LCD:
      pixel_ಬಹುಲಕ = FT_PIXEL_ಬಹುಲಕ_LCD;
      ft_lcd_padding( &cbox, ಸ್ಥಾನ, ಬಹುಲಕ );
      ಹೋಗು Adjust;

    ಸ್ಥಿತಿ FT_RENDER_ಬಹುಲಕ_LCD_V:
      pixel_ಬಹುಲಕ = FT_PIXEL_ಬಹುಲಕ_LCD_V;
      ft_lcd_padding( &cbox, ಸ್ಥಾನ, ಬಹುಲಕ );
      ಹೋಗು Adjust;

    ಸ್ಥಿತಿ FT_RENDER_ಬಹುಲಕ_ಸಾಮಾನ್ಯ:
    ಸ್ಥಿತಿ FT_RENDER_ಬಹುಲಕ_LIGHT:
    ಪೂರ್ವನಿಯೋಜಿತ:
      pixel_ಬಹುಲಕ = FT_PIXEL_ಬಹುಲಕ_ಬೂದು;
    Adjust:
      pbox.xMin += cbox.xMin >> 6;
      pbox.yMin += cbox.yMin >> 6;
      pbox.xMax += ( cbox.xMax + 63 ) >> 6;
      pbox.yMax += ( cbox.yMax + 63 ) >> 6;
    }

    x_ಎಡ = pbox.xMin;
    y_ತುದಿ  = pbox.yMax;

    ಅಗಲ  = pbox.xMax - pbox.xMin;
    ಎತ್ತರ = pbox.yMax - pbox.yMin;

    /* Flag the width or height unsuitable for rendering.   */
    /* The limit is based on the ppem value when available. */
    /* FT_Renderer modules should check the return value.   */
    ret = FT_BOOL(     ಅಗಲ >= 0x10000   ||    ಎತ್ತರ >= 0x10000   ||
                   pbox.xMin < -0x1000000 || pbox.xMax >= 0x1000000 ||
                   pbox.yMin < -0x1000000 || pbox.yMax >= 0x1000000 ||
             ( ಸ್ಥಾನ->face                                         &&
               ( ಅಗಲ  > 16 * ಸ್ಥಾನ->face->ಗಾತ್ರ_2->metrics.x_ppem ||
                 ಎತ್ತರ > 16 * ಸ್ಥಾನ->face->ಗಾತ್ರ_2->metrics.y_ppem ) ) );

    ಆದರೆ ( ret )
      FT_TRACE3(( "ft_glyphslot_preset_bitmap: [%ld %ld %ld %ld]\n",
                  pbox.xMin, pbox.yMin, pbox.xMax, pbox.yMax ));

    ಆಯ್ಕೆ ( pixel_ಬಹುಲಕ )
    {
    ಸ್ಥಿತಿ FT_PIXEL_ಬಹುಲಕ_MONO:
      pitch = ( ( ಅಗಲ + 15 ) >> 4 ) << 1;
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ FT_PIXEL_ಬಹುಲಕ_LCD:
      ಅಗಲ *= 3;
      pitch  = FT_PAD_CEIL( ಅಗಲ, 4 );
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ FT_PIXEL_ಬಹುಲಕ_LCD_V:
      ಎತ್ತರ *= 3;
      FALL_THROUGH;

    ಸ್ಥಿತಿ FT_PIXEL_ಬಹುಲಕ_ಬೂದು:
    ಪೂರ್ವನಿಯೋಜಿತ:
      pitch = ಅಗಲ;
    }

    ಸ್ಥಾನ->bitmap_ಎಡ = (FT_Int)x_ಎಡ;
    ಸ್ಥಾನ->bitmap_ತುದಿ  = (FT_Int)y_ತುದಿ;

    bitmap->pixel_ಬಹುಲಕ = (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ)pixel_ಬಹುಲಕ;
    bitmap->ಅಂಕೆ_grays  = 256;
    bitmap->ಅಗಲ      = (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ)ಅಗಲ;
    bitmap->ಅಡ್ಡಸಾಲುಗಳು       = (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ)ಎತ್ತರ;
    bitmap->pitch      = pitch;

    ಹಿಂತಿರುಗಿ ret;
  }


  FT_ತಳ_DEF( ಶೂನ್ಯ )
  ft_glyphslot_ಗಣ_bitmap( FT_Glyph_ಸ್ಥಾನ  ಸ್ಥಾನ,
                           FT_Byte*      ಬಫರ್ )
  {
    ft_glyphslot_free_bitmap( ಸ್ಥಾನ );

    ಸ್ಥಾನ->bitmap.ಬಫರ್ = ಬಫರ್;

    FT_ASSERT( (ಸ್ಥಾನ->ಆಂತರಿಕ->flags & FT_GLYPH_OWN_BITMAP) == 0 );
  }


  FT_ತಳ_DEF( FT_Error )
  ft_glyphslot_alloc_bitmap( FT_Glyph_ಸ್ಥಾನ  ಸ್ಥಾನ )
  {
    FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ = FT_FACE_ಸ್ಮರಣೆ( ಸ್ಥಾನ->face );
    FT_Error   error;


    ಆದರೆ ( ಸ್ಥಾನ->ಆಂತರಿಕ->flags & FT_GLYPH_OWN_BITMAP )
      FT_FREE( ಸ್ಥಾನ->bitmap.ಬಫರ್ );
    ಇಲ್ಲದಿದ್ದರೆ
      ಸ್ಥಾನ->ಆಂತರಿಕ->flags |= FT_GLYPH_OWN_BITMAP;

    /* dimensions must be preset */
    FT_MEM_ALLOC_MULT( ಸ್ಥಾನ->bitmap.ಬಫರ್,
                       ಸ್ಥಾನ->bitmap.ಅಡ್ಡಸಾಲುಗಳು,
                       ಸ್ಥಾನ->bitmap.pitch );
    ಹಿಂತಿರುಗಿ error;
  }


  ಸ್ಥಿರ ಶೂನ್ಯ
  ft_glyphslot_clear( FT_Glyph_ಸ್ಥಾನ  ಸ್ಥಾನ )
  {
    /* free bitmap if needed */
    ft_glyphslot_free_bitmap( ಸ್ಥಾನ );

    /* clear all public fields in the glyph slot */
    ಸ್ಥಾನ->glyph_ಸೂಚ್ಯಂಕ = 0;

    FT_ZERO( &ಸ್ಥಾನ->metrics );
    FT_ZERO( &ಸ್ಥಾನ->outline );

    ಸ್ಥಾನ->bitmap.ಅಗಲ      = 0;
    ಸ್ಥಾನ->bitmap.ಅಡ್ಡಸಾಲುಗಳು       = 0;
    ಸ್ಥಾನ->bitmap.pitch      = 0;
    ಸ್ಥಾನ->bitmap.pixel_ಬಹುಲಕ = 0;
    /* `slot->bitmap.buffer' has been handled by ft_glyphslot_free_bitmap */

    ಸ್ಥಾನ->bitmap_ಎಡ   = 0;
    ಸ್ಥಾನ->bitmap_ತುದಿ    = 0;
    ಸ್ಥಾನ->ಅಂಕೆ_subglyphs = 0;
    ಸ್ಥಾನ->subglyphs     = ಖಾಲಿ;
    ಸ್ಥಾನ->control_ದತ್ತಾಂಶ  = ಖಾಲಿ;
    ಸ್ಥಾನ->control_len   = 0;

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ FT_CONFIG_OPTION_SVG
    ಸ್ಥಾನ->other = ಖಾಲಿ;
#ಇಲ್ಲದಿದ್ದರೆ
    ಆದರೆ ( !( ಸ್ಥಾನ->face->face_flags & FT_FACE_FLAG_SVG ) )
      ಸ್ಥಾನ->other = ಖಾಲಿ;
    ಇಲ್ಲದಿದ್ದರೆ
    {
      ಆದರೆ ( ಸ್ಥಾನ->ಆಂತರಿಕ->flags & FT_GLYPH_OWN_GZIP_SVG )
      {
        FT_ಸ್ಮರಣೆ        ಸ್ಮರಣೆ = ಸ್ಥಾನ->face->ಸ್ಮರಣೆ;
        FT_SVG_Document  doc    = (FT_SVG_Document)ಸ್ಥಾನ->other;


        FT_FREE( doc->svg_document );
        ಸ್ಥಾನ->ಆಂತರಿಕ->flags &= ~FT_GLYPH_OWN_GZIP_SVG;
      }
    }
#ಆದರೆ_ಅಂತ್ಯ

    ಸ್ಥಾನ->format = FT_GLYPH_FORMAT_NONE;

    ಸ್ಥಾನ->ರೇಖೀಯ_Hori_Advance = 0;
    ಸ್ಥಾನ->ರೇಖೀಯ_Vert_Advance = 0;
    ಸ್ಥಾನ->advance.x         = 0;
    ಸ್ಥಾನ->advance.y         = 0;
    ಸ್ಥಾನ->lsb_delta         = 0;
    ಸ್ಥಾನ->rsb_delta         = 0;
  }


  ಸ್ಥಿರ ಶೂನ್ಯ
  ft_glyphslot_ಮುಗಿದ( FT_Glyph_ಸ್ಥಾನ  ಸ್ಥಾನ )
  {
    FT_Driver        driver = ಸ್ಥಾನ->face->driver;
    FT_Driver_Class  clazz  = driver->clazz;
    FT_ಸ್ಮರಣೆ        ಸ್ಮರಣೆ = driver->ಬೇರು.ಸ್ಮರಣೆ;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ FT_CONFIG_OPTION_SVG
    ಆದರೆ ( ಸ್ಥಾನ->face->face_flags & FT_FACE_FLAG_SVG )
    {
      /* Free memory in case SVG was there.                          */
      /* `slot->internal` might be NULL in out-of-memory situations. */
      ಆದರೆ ( ಸ್ಥಾನ->ಆಂತರಿಕ && ಸ್ಥಾನ->ಆಂತರಿಕ->flags & FT_GLYPH_OWN_GZIP_SVG )
      {
        FT_SVG_Document  doc = (FT_SVG_Document)ಸ್ಥಾನ->other;


        FT_FREE( doc->svg_document );

        ಸ್ಥಾನ->ಆಂತರಿಕ->flags &= ~FT_GLYPH_OWN_GZIP_SVG;
      }

      FT_FREE( ಸ್ಥಾನ->other );
    }
#ಆದರೆ_ಅಂತ್ಯ

    ಆದರೆ ( clazz->ಮುಗಿದ_ಸ್ಥಾನ )
      clazz->ಮುಗಿದ_ಸ್ಥಾನ( ಸ್ಥಾನ );

    /* free bitmap buffer if needed */
    ft_glyphslot_free_bitmap( ಸ್ಥಾನ );

    /* slot->internal might be NULL in out-of-memory situations */
    ಆದರೆ ( ಸ್ಥಾನ->ಆಂತರಿಕ )
    {
      /* free glyph loader */
      ಆದರೆ ( FT_DRIVER_ಬಳಸುತ್ತದೆ_OUTLINES( driver ) )
      {
        FT_Glyph_Loader_ಮುಗಿದ( ಸ್ಥಾನ->ಆಂತರಿಕ->loader );
        ಸ್ಥಾನ->ಆಂತರಿಕ->loader = ಖಾಲಿ;
      }

      FT_FREE( ಸ್ಥಾನ->ಆಂತರಿಕ );
    }
  }


  /* documentation is in ftobjs.h */

  FT_ತಳ_DEF( FT_Error )
  FT_New_Glyph_ಸ್ಥಾನ( FT_Face        face,
                    FT_Glyph_ಸ್ಥಾನ  *aslot )
  {
    FT_Error         error;
    FT_Driver        driver;
    FT_Driver_Class  clazz;
    FT_ಸ್ಮರಣೆ        ಸ್ಮರಣೆ;
    FT_Glyph_ಸ್ಥಾನ     ಸ್ಥಾನ = ಖಾಲಿ;


    ಆದರೆ ( !face )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_Face_Handle );

    ಆದರೆ ( !face->driver )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

    driver = face->driver;
    clazz  = driver->clazz;
    ಸ್ಮರಣೆ = driver->ಬೇರು.ಸ್ಮರಣೆ;

    FT_TRACE4(( "FT_New_GlyphSlot: Creating new slot object\n" ));
    ಆದರೆ ( !FT_ALLOC( ಸ್ಥಾನ, clazz->ಸ್ಥಾನ_object_ಗಾತ್ರ ) )
    {
      ಸ್ಥಾನ->face = face;

      error = ft_glyphslot_init( ಸ್ಥಾನ );
      ಆದರೆ ( error )
      {
        ft_glyphslot_ಮುಗಿದ( ಸ್ಥಾನ );
        FT_FREE( ಸ್ಥಾನ );
        ಹೋಗು Exit;
      }

      ಸ್ಥಾನ->ಮುಂದಿನ  = face->glyph;
      face->glyph = ಸ್ಥಾನ;

      ಆದರೆ ( aslot )
        *aslot = ಸ್ಥಾನ;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ( aslot )
      *aslot = ಖಾಲಿ;


  Exit:
    FT_TRACE4(( "FT_New_GlyphSlot: Return 0x%x\n", error ));

    ಹಿಂತಿರುಗಿ error;
  }


  /* documentation is in ftobjs.h */

  FT_ತಳ_DEF( ಶೂನ್ಯ )
  FT_ಮುಗಿದ_Glyph_ಸ್ಥಾನ( FT_Glyph_ಸ್ಥಾನ  ಸ್ಥಾನ )
  {
    ಆದರೆ ( ಸ್ಥಾನ )
    {
      FT_Driver     driver = ಸ್ಥಾನ->face->driver;
      FT_ಸ್ಮರಣೆ     ಸ್ಮರಣೆ = driver->ಬೇರು.ಸ್ಮರಣೆ;
      FT_Glyph_ಸ್ಥಾನ  prev;
      FT_Glyph_ಸ್ಥಾನ  cur;


      /* Remove slot from its parent face's list */
      prev = ಖಾಲಿ;
      cur  = ಸ್ಥಾನ->face->glyph;

      ಆಗ ( cur )
      {
        ಆದರೆ ( cur == ಸ್ಥಾನ )
        {
          ಆದರೆ ( !prev )
            ಸ್ಥಾನ->face->glyph = cur->ಮುಂದಿನ;
          ಇಲ್ಲದಿದ್ದರೆ
            prev->ಮುಂದಿನ = cur->ಮುಂದಿನ;

          /* finalize client-specific data */
          ಆದರೆ ( ಸ್ಥಾನ->generic.finalizer )
            ಸ್ಥಾನ->generic.finalizer( ಸ್ಥಾನ );

          ft_glyphslot_ಮುಗಿದ( ಸ್ಥಾನ );
          FT_FREE( ಸ್ಥಾನ );
          ನಿಲ್ಲಿಸು;
        }
        prev = cur;
        cur  = cur->ಮುಂದಿನ;
      }
    }
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( ಶೂನ್ಯ )
  FT_ಗಣ_Transform( FT_Face     face,
                    FT_ಮಾತೃಕೆ*  ಮಾತೃಕೆ,
                    FT_ಸದಿಶ*  delta )
  {
    FT_Face_ಆಂತರಿಕ  ಆಂತರಿಕ;


    ಆದರೆ ( !face )
      ಹಿಂತಿರುಗಿ;

    ಆಂತರಿಕ = face->ಆಂತರಿಕ;

    ಆಂತರಿಕ->transform_flags = 0;

    ಆದರೆ ( !ಮಾತೃಕೆ || !FT_ಮಾತೃಕೆ_Check( ಮಾತೃಕೆ ) )
    {
      ಆಂತರಿಕ->transform_ಮಾತೃಕೆ.xx = 0x10000L;
      ಆಂತರಿಕ->transform_ಮಾತೃಕೆ.xy = 0;
      ಆಂತರಿಕ->transform_ಮಾತೃಕೆ.yx = 0;
      ಆಂತರಿಕ->transform_ಮಾತೃಕೆ.yy = 0x10000L;

      ಮಾತೃಕೆ = &ಆಂತರಿಕ->transform_ಮಾತೃಕೆ;
    }
    ಇಲ್ಲದಿದ್ದರೆ
      ಆಂತರಿಕ->transform_ಮಾತೃಕೆ = *ಮಾತೃಕೆ;

    /* set transform_flags bit flag 0 if `matrix' isn't the identity */
    ಆದರೆ ( ( ಮಾತೃಕೆ->xy | ಮಾತೃಕೆ->yx ) ||
         ಮಾತೃಕೆ->xx != 0x10000L      ||
         ಮಾತೃಕೆ->yy != 0x10000L      )
      ಆಂತರಿಕ->transform_flags |= 1;

    ಆದರೆ ( !delta )
    {
      ಆಂತರಿಕ->transform_delta.x = 0;
      ಆಂತರಿಕ->transform_delta.y = 0;

      delta = &ಆಂತರಿಕ->transform_delta;
    }
    ಇಲ್ಲದಿದ್ದರೆ
      ಆಂತರಿಕ->transform_delta = *delta;

    /* set transform_flags bit flag 1 if `delta' isn't the null vector */
    ಆದರೆ ( delta->x | delta->y )
      ಆಂತರಿಕ->transform_flags |= 2;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( ಶೂನ್ಯ )
  FT_Get_Transform( FT_Face     face,
                    FT_ಮಾತೃಕೆ*  ಮಾತೃಕೆ,
                    FT_ಸದಿಶ*  delta )
  {
    FT_Face_ಆಂತರಿಕ  ಆಂತರಿಕ;


    ಆದರೆ ( !face )
      ಹಿಂತಿರುಗಿ;

    ಆಂತರಿಕ = face->ಆಂತರಿಕ;

    ಆದರೆ ( ಮಾತೃಕೆ )
      *ಮಾತೃಕೆ = ಆಂತರಿಕ->transform_ಮಾತೃಕೆ;

    ಆದರೆ ( delta )
      *delta = ಆಂತರಿಕ->transform_delta;
  }


  ಸ್ಥಿರ FT_Renderer
  ft_lookup_glyph_renderer( FT_Glyph_ಸ್ಥಾನ  ಸ್ಥಾನ );


#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GRID_FIT_METRICS
  ಸ್ಥಿರ ಶೂನ್ಯ
  ft_glyphslot_grid_fit_metrics( FT_Glyph_ಸ್ಥಾನ  ಸ್ಥಾನ,
                                 FT_Bool       vertical )
  {
    FT_Glyph_Metrics*  metrics = &ಸ್ಥಾನ->metrics;
    FT_Pos             ಬಲ, bottom;


    ಆದರೆ ( vertical )
    {
      metrics->horiBearingX = FT_PIX_FLOOR( metrics->horiBearingX );
      metrics->horiBearingY = FT_PIX_CEIL_LONG( metrics->horiBearingY );

      ಬಲ  = FT_PIX_CEIL_LONG( ಕೂಡಿಸು_LONG( metrics->vertBearingX,
                                           metrics->ಅಗಲ ) );
      bottom = FT_PIX_CEIL_LONG( ಕೂಡಿಸು_LONG( metrics->vertBearingY,
                                           metrics->ಎತ್ತರ ) );

      metrics->vertBearingX = FT_PIX_FLOOR( metrics->vertBearingX );
      metrics->vertBearingY = FT_PIX_FLOOR( metrics->vertBearingY );

      metrics->ಅಗಲ  = SUB_LONG( ಬಲ,
                                  metrics->vertBearingX );
      metrics->ಎತ್ತರ = SUB_LONG( bottom,
                                  metrics->vertBearingY );
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
      metrics->vertBearingX = FT_PIX_FLOOR( metrics->vertBearingX );
      metrics->vertBearingY = FT_PIX_FLOOR( metrics->vertBearingY );

      ಬಲ  = FT_PIX_CEIL_LONG( ಕೂಡಿಸು_LONG( metrics->horiBearingX,
                                           metrics->ಅಗಲ ) );
      bottom = FT_PIX_FLOOR( SUB_LONG( metrics->horiBearingY,
                                       metrics->ಎತ್ತರ ) );

      metrics->horiBearingX = FT_PIX_FLOOR( metrics->horiBearingX );
      metrics->horiBearingY = FT_PIX_CEIL_LONG( metrics->horiBearingY );

      metrics->ಅಗಲ  = SUB_LONG( ಬಲ,
                                  metrics->horiBearingX );
      metrics->ಎತ್ತರ = SUB_LONG( metrics->horiBearingY,
                                  bottom );
    }

    metrics->horiAdvance = FT_PIX_ಸುತ್ತು_LONG( metrics->horiAdvance );
    metrics->vertAdvance = FT_PIX_ಸುತ್ತು_LONG( metrics->vertAdvance );
  }
#ಆದರೆ_ಅಂತ್ಯ /* GRID_FIT_METRICS */


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_ತುಂಬು_Glyph( FT_Face   face,
                 FT_UInt   glyph_ಸೂಚ್ಯಂಕ,
                 FT_Int32  ತುಂಬು_flags )
  {
    FT_Error      error;
    FT_Driver     driver;
    FT_Glyph_ಸ್ಥಾನ  ಸ್ಥಾನ;
    FT_Library    library;
    FT_Bool       autohint = FALSE;
    FT_Module     hinter;


    ಆದರೆ ( !face || !face->ಗಾತ್ರ_2 || !face->glyph )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_Face_Handle );

    /* The validity test for `glyph_index' is performed by the */
    /* font drivers.                                           */

    ಸ್ಥಾನ = face->glyph;
    ft_glyphslot_clear( ಸ್ಥಾನ );

    driver  = face->driver;
    library = driver->ಬೇರು.library;
    hinter  = library->auto_hinter;

    /* undefined scale means no scale */
    ಆದರೆ ( face->ಗಾತ್ರ_2->metrics.x_ppem == 0 ||
         face->ಗಾತ್ರ_2->metrics.y_ppem == 0 )
      ತುಂಬು_flags |= FT_ತುಂಬು_NO_SCALE;

    /* resolve load flags dependencies */

    ಆದರೆ ( ತುಂಬು_flags & FT_ತುಂಬು_NO_RECURSE )
      ತುಂಬು_flags |= FT_ತುಂಬು_NO_SCALE         |
                    FT_ತುಂಬು_IGNORE_TRANSFORM;

    ಆದರೆ ( ತುಂಬು_flags & FT_ತುಂಬು_NO_SCALE )
    {
      ತುಂಬು_flags |= FT_ತುಂಬು_NO_HINTING |
                    FT_ತುಂಬು_NO_BITMAP;

      ತುಂಬು_flags &= ~FT_ತುಂಬು_RENDER;
    }

    ಆದರೆ ( ತುಂಬು_flags & FT_ತುಂಬು_BITMAP_METRICS_ONLY )
      ತುಂಬು_flags &= ~FT_ತುಂಬು_RENDER;

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

    ಆದರೆ ( hinter                                           &&
         !( ತುಂಬು_flags & FT_ತುಂಬು_NO_HINTING )             &&
         !( ತುಂಬು_flags & FT_ತುಂಬು_NO_AUTOHINT )            &&
         FT_IS_SCALABLE( face )                           &&
         !FT_IS_TRICKY( face )                            &&
         ( ( ತುಂಬು_flags & FT_ತುಂಬು_IGNORE_TRANSFORM )    ||
           ( face->ಆಂತರಿಕ->transform_ಮಾತೃಕೆ.yx == 0 &&
             face->ಆಂತರಿಕ->transform_ಮಾತೃಕೆ.xx != 0 ) ||
           ( face->ಆಂತರಿಕ->transform_ಮಾತೃಕೆ.xx == 0 &&
             face->ಆಂತರಿಕ->transform_ಮಾತೃಕೆ.yx != 0 ) ) )
    {
      ಆದರೆ ( ( ತುಂಬು_flags & FT_ತುಂಬು_FORCE_AUTOHINT ) ||
           !FT_DRIVER_HAS_HINTER( driver )         )
        autohint = TRUE;
      ಇಲ್ಲದಿದ್ದರೆ
      {
        FT_Render_ಬಹುಲಕ  ಬಹುಲಕ = FT_ತುಂಬು_ಗುರಿ_ಬಹುಲಕ( ತುಂಬು_flags );
        FT_Bool         is_light_type1;
        TT_Face         ttface = (TT_Face)face;


        /* only the new Adobe engine (for both CFF and Type 1) is `light'; */
        /* we use `strstr' to catch both `Type 1' and `CID Type 1'         */
        is_light_type1 =
          ft_strstr( FT_Get_Font_Format( face ), "Type 1" ) != ಖಾಲಿ &&
          ((PS_Driver)driver)->hinting_engine == FT_HINTING_ADOBE;

        /* the check for `num_locations' assures that we actually    */
        /* test for instructions in a TTF and not in a CFF-based OTF */
        /*                                                           */
        /* we check the size of the `fpgm' and `prep' tables, too -- */
        /* the assumption is that there don't exist real TTFs where  */
        /* both `fpgm' and `prep' tables are missing                 */
        ಆದರೆ ( ( ಬಹುಲಕ == FT_RENDER_ಬಹುಲಕ_LIGHT           &&
               ( !FT_DRIVER_HINTS_LIGHTLY( driver ) &&
                 !is_light_type1                    ) )         ||
             ( FT_IS_SFNT( face )                             &&
               ttface->ಅಂಕೆ_locations                          &&
               ttface->font_ಕಾರ್ಯಕ್ರಮ_ಗಾತ್ರ == 0                 &&
               ttface->cvt_ಕಾರ್ಯಕ್ರಮ_ಗಾತ್ರ <= 7                  ) )
          autohint = TRUE;
      }
    }

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ TT_CONFIG_OPTION_VARC
    /* Never auto-hint a VARC (variable composite) glyph: its components */
    /* are already loaded unhinted, and the auto-fitter cannot process   */
    /* the assembled composite (it also re-enters variation-coordinate   */
    /* setting).  Use the native hinter / unhinted load instead.         */
    ಆದರೆ ( autohint && FT_IS_SFNT( face ) )
    {
      FT_Service_VARC  varc;


      FT_FACE_ಕಂಡುಹಿಡಿ_SERVICE( face, varc, VARC );
      ಆದರೆ ( varc && varc->has_glyph( face, glyph_ಸೂಚ್ಯಂಕ ) )
        autohint = FALSE;
    }
#ಆದರೆ_ಅಂತ್ಯ

    ಆದರೆ ( autohint )
    {
      FT_AutoHinter_Interface  hinting;


      /* XXX: The use of the `FT_LOAD_XXX_ONLY` flags is not very */
      /*      elegant.                                            */

      /* try to load SVG documents if available */
      ಆದರೆ ( ( ತುಂಬು_flags & FT_ತುಂಬು_NO_SVG ) == 0 &&
           FT_HAS_SVG( face )                   )
      {
        error = driver->clazz->ತುಂಬು_glyph( ಸ್ಥಾನ, face->ಗಾತ್ರ_2,
                                           glyph_ಸೂಚ್ಯಂಕ,
                                           ತುಂಬು_flags | FT_ತುಂಬು_SVG_ONLY );

        ಆದರೆ ( !error && ಸ್ಥಾನ->format == FT_GLYPH_FORMAT_SVG )
          ಹೋಗು ತುಂಬು_Ok;
      }

      /* try to load embedded bitmaps if available */
      ಆದರೆ ( FT_HAS_FIXED_SIZES( face )              &&
           ( ತುಂಬು_flags & FT_ತುಂಬು_NO_BITMAP ) == 0 )
      {
        error = driver->clazz->ತುಂಬು_glyph( ಸ್ಥಾನ, face->ಗಾತ್ರ_2,
                                           glyph_ಸೂಚ್ಯಂಕ,
                                           ತುಂಬು_flags | FT_ತುಂಬು_SBITS_ONLY );

        ಆದರೆ ( !error && ಸ್ಥಾನ->format == FT_GLYPH_FORMAT_BITMAP )
          ಹೋಗು ತುಂಬು_Ok;
      }

      {
        FT_Face_ಆಂತರಿಕ  ಆಂತರಿಕ        = face->ಆಂತರಿಕ;
        FT_Int            transform_flags = ಆಂತರಿಕ->transform_flags;


        /* since the auto-hinter calls FT_Load_Glyph by itself, */
        /* make sure that glyphs aren't transformed             */
        ಆಂತರಿಕ->transform_flags = 0;

        /* load auto-hinted outline */
        hinting = (FT_AutoHinter_Interface)hinter->clazz->module_interface;

        error   = hinting->ತುಂಬು_glyph( (FT_AutoHinter)hinter,
                                       ಸ್ಥಾನ, face->ಗಾತ್ರ_2,
                                       glyph_ಸೂಚ್ಯಂಕ, ತುಂಬು_flags );

        ಆಂತರಿಕ->transform_flags = transform_flags;
      }
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
      error = driver->clazz->ತುಂಬು_glyph( ಸ್ಥಾನ,
                                         face->ಗಾತ್ರ_2,
                                         glyph_ಸೂಚ್ಯಂಕ,
                                         ತುಂಬು_flags );
      ಆದರೆ ( error )
        ಹೋಗು Exit;

      ಆದರೆ ( ಸ್ಥಾನ->format == FT_GLYPH_FORMAT_OUTLINE )
      {
        /* check that the loaded outline is correct */
        error = FT_Outline_Check( &ಸ್ಥಾನ->outline );
        ಆದರೆ ( error )
          ಹೋಗು Exit;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GRID_FIT_METRICS
        ಆದರೆ ( !( ತುಂಬು_flags & FT_ತುಂಬು_NO_HINTING ) )
          ft_glyphslot_grid_fit_metrics(
            ಸ್ಥಾನ,
            FT_BOOL( ತುಂಬು_flags & FT_ತುಂಬು_VERTICAL_LAYOUT ) );
#ಆದರೆ_ಅಂತ್ಯ
      }
    }

  ತುಂಬು_Ok:
    /* compute the advance */
    ಆದರೆ ( ತುಂಬು_flags & FT_ತುಂಬು_VERTICAL_LAYOUT )
    {
      ಸ್ಥಾನ->advance.x = 0;
      ಸ್ಥಾನ->advance.y = ಸ್ಥಾನ->metrics.vertAdvance;
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
      ಸ್ಥಾನ->advance.x = ಸ್ಥಾನ->metrics.horiAdvance;
      ಸ್ಥಾನ->advance.y = 0;
    }

    /* compute the linear advance in 16.16 pixels */
    ಆದರೆ ( ( ತುಂಬು_flags & FT_ತುಂಬು_ರೇಖೀಯ_DESIGN ) == 0 &&
         FT_IS_SCALABLE( face )                      )
    {
      FT_ಗಾತ್ರ_Metrics*  metrics = &face->ಗಾತ್ರ_2->metrics;


      /* it's tricky! */
      ಸ್ಥಾನ->ರೇಖೀಯ_Hori_Advance = FT_MulDiv( ಸ್ಥಾನ->ರೇಖೀಯ_Hori_Advance,
                                           metrics->x_scale, 64 );

      ಸ್ಥಾನ->ರೇಖೀಯ_Vert_Advance = FT_MulDiv( ಸ್ಥಾನ->ರೇಖೀಯ_Vert_Advance,
                                           metrics->y_scale, 64 );
    }

    ಆದರೆ ( ( ತುಂಬು_flags & FT_ತುಂಬು_IGNORE_TRANSFORM ) == 0 )
    {
      FT_Face_ಆಂತರಿಕ  ಆಂತರಿಕ = face->ಆಂತರಿಕ;


      /* now, transform the glyph image if needed */
      ಆದರೆ ( ಆಂತರಿಕ->transform_flags )
      {
        /* get renderer */
        FT_Renderer  renderer = ft_lookup_glyph_renderer( ಸ್ಥಾನ );


        ಆದರೆ ( renderer )
          error = renderer->clazz->transform_glyph(
                                     renderer, ಸ್ಥಾನ,
                                     &ಆಂತರಿಕ->transform_ಮಾತೃಕೆ,
                                     &ಆಂತರಿಕ->transform_delta );
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ( ಸ್ಥಾನ->format == FT_GLYPH_FORMAT_OUTLINE )
        {
          /* apply `standard' transformation if no renderer is available */
          ಆದರೆ ( ಆಂತರಿಕ->transform_flags & 1 )
            FT_Outline_Transform( &ಸ್ಥಾನ->outline,
                                  &ಆಂತರಿಕ->transform_ಮಾತೃಕೆ );

          ಆದರೆ ( ಆಂತರಿಕ->transform_flags & 2 )
            FT_Outline_Translate( &ಸ್ಥಾನ->outline,
                                  ಆಂತರಿಕ->transform_delta.x,
                                  ಆಂತರಿಕ->transform_delta.y );
        }

        /* transform advance */
        FT_ಸದಿಶ_Transform( &ಸ್ಥಾನ->advance, &ಆಂತರಿಕ->transform_ಮಾತೃಕೆ );
      }
    }

    ಸ್ಥಾನ->glyph_ಸೂಚ್ಯಂಕ          = glyph_ಸೂಚ್ಯಂಕ;
    ಸ್ಥಾನ->ಆಂತರಿಕ->ತುಂಬು_flags = ತುಂಬು_flags;

    /* do we need to render the image or preset the bitmap now? */
    ಆದರೆ ( !error                                    &&
         ( ತುಂಬು_flags & FT_ತುಂಬು_NO_SCALE ) == 0    &&
         ಸ್ಥಾನ->format != FT_GLYPH_FORMAT_BITMAP    &&
         ಸ್ಥಾನ->format != FT_GLYPH_FORMAT_ಸಂಯುಕ್ತ )
    {
      FT_Render_ಬಹುಲಕ  ಬಹುಲಕ = FT_ತುಂಬು_ಗುರಿ_ಬಹುಲಕ( ತುಂಬು_flags );


      ಆದರೆ ( ಬಹುಲಕ == FT_RENDER_ಬಹುಲಕ_ಸಾಮಾನ್ಯ   &&
           ತುಂಬು_flags & FT_ತುಂಬು_MONOCHROME )
        ಬಹುಲಕ = FT_RENDER_ಬಹುಲಕ_MONO;

      ಆದರೆ ( ತುಂಬು_flags & FT_ತುಂಬು_RENDER )
        error = FT_Render_Glyph( ಸ್ಥಾನ, ಬಹುಲಕ );
      ಇಲ್ಲದಿದ್ದರೆ
        ft_glyphslot_preset_bitmap( ಸ್ಥಾನ, ಬಹುಲಕ, ಖಾಲಿ );
    }

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ FT_DEBUG_LEVEL_TRACE
    FT_TRACE5(( "FT_Load_Glyph: index %u, flags 0x%x\n",
                glyph_ಸೂಚ್ಯಂಕ, ತುಂಬು_flags ));
    FT_TRACE5(( "  bitmap %ux%u %s, %s (mode %d)\n",
                ಸ್ಥಾನ->bitmap.ಅಗಲ,
                ಸ್ಥಾನ->bitmap.ಅಡ್ಡಸಾಲುಗಳು,
                ಸ್ಥಾನ->outline.points ?
                  ಸ್ಥಾನ->bitmap.ಬಫರ್ ? "rendered"
                                      : "preset"
                                     :
                  ಸ್ಥಾನ->ಆಂತರಿಕ->flags & FT_GLYPH_OWN_BITMAP ? "owned"
                                                              : "unowned",
                pixel_modes[ಸ್ಥಾನ->bitmap.pixel_ಬಹುಲಕ],
                ಸ್ಥಾನ->bitmap.pixel_ಬಹುಲಕ ));
    FT_TRACE5(( "\n" ));
    FT_TRACE5(( "  x advance: %f\n", (ದ್ವಿಗುಣ)ಸ್ಥಾನ->advance.x / 64 ));
    FT_TRACE5(( "  y advance: %f\n", (ದ್ವಿಗುಣ)ಸ್ಥಾನ->advance.y / 64 ));
    FT_TRACE5(( "  linear x advance: %f\n",
                (ದ್ವಿಗುಣ)ಸ್ಥಾನ->ರೇಖೀಯ_Hori_Advance / 65536 ));
    FT_TRACE5(( "  linear y advance: %f\n",
                (ದ್ವಿಗುಣ)ಸ್ಥಾನ->ರೇಖೀಯ_Vert_Advance / 65536 ));

    {
      FT_Glyph_Metrics*  metrics = &ಸ್ಥಾನ->metrics;


      FT_TRACE5(( "  metrics:\n" ));
      FT_TRACE5(( "    width:  %f\n", (ದ್ವಿಗುಣ)metrics->ಅಗಲ / 64 ));
      FT_TRACE5(( "    height: %f\n", (ದ್ವಿಗುಣ)metrics->ಎತ್ತರ / 64 ));
      FT_TRACE5(( "\n" ));
      FT_TRACE5(( "    horiBearingX: %f\n",
                  (ದ್ವಿಗುಣ)metrics->horiBearingX / 64 ));
      FT_TRACE5(( "    horiBearingY: %f\n",
                  (ದ್ವಿಗುಣ)metrics->horiBearingY / 64 ));
      FT_TRACE5(( "    horiAdvance:  %f\n",
                  (ದ್ವಿಗುಣ)metrics->horiAdvance / 64 ));
      FT_TRACE5(( "\n" ));
      FT_TRACE5(( "    vertBearingX: %f\n",
                  (ದ್ವಿಗುಣ)metrics->vertBearingX / 64 ));
      FT_TRACE5(( "    vertBearingY: %f\n",
                  (ದ್ವಿಗುಣ)metrics->vertBearingY / 64 ));
      FT_TRACE5(( "    vertAdvance:  %f\n",
                  (ದ್ವಿಗುಣ)metrics->vertAdvance / 64 ));
    }
#ಆದರೆ_ಅಂತ್ಯ

  Exit:
    ಹಿಂತಿರುಗಿ error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_ತುಂಬು_Char( FT_Face   face,
                FT_ULong  char_ಸಂಕೇತ,
                FT_Int32  ತುಂಬು_flags )
  {
    FT_UInt  glyph_ಸೂಚ್ಯಂಕ;


    ಆದರೆ ( !face )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_Face_Handle );

    glyph_ಸೂಚ್ಯಂಕ = (FT_UInt)char_ಸಂಕೇತ;
    ಆದರೆ ( face->charmap )
      glyph_ಸೂಚ್ಯಂಕ = FT_Get_Char_ಸೂಚ್ಯಂಕ( face, char_ಸಂಕೇತ );

    ಹಿಂತಿರುಗಿ FT_ತುಂಬು_Glyph( face, glyph_ಸೂಚ್ಯಂಕ, ತುಂಬು_flags );
  }


  /* destructor for sizes list */
  ಸ್ಥಿರ ಶೂನ್ಯ
  destroy_ಗಾತ್ರ( FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ,
                ಶೂನ್ಯ*      ಗಾತ್ರ_3,
                ಶೂನ್ಯ*      driver_ )
  {
    FT_ಗಾತ್ರ    ಗಾತ್ರ_2   = (FT_ಗಾತ್ರ)ಗಾತ್ರ_3;
    FT_Driver  driver = (FT_Driver)driver_;


    /* finalize format-specific stuff */
    ಆದರೆ ( driver->clazz->ಮುಗಿದ_ಗಾತ್ರ )
      driver->clazz->ಮುಗಿದ_ಗಾತ್ರ( ಗಾತ್ರ_2 );

    /* finalize client-specific data */
    ಆದರೆ ( ಗಾತ್ರ_2->generic.finalizer )
      ಗಾತ್ರ_2->generic.finalizer( ಗಾತ್ರ_2 );

    FT_FREE( ಗಾತ್ರ_2->ಆಂತರಿಕ );
    FT_FREE( ಗಾತ್ರ_2 );
  }


  ಸ್ಥಿರ ಶೂನ್ಯ
  ft_cmap_ಮುಗಿದ_ಆಂತರಿಕ( FT_CMap  cmap );


  ಸ್ಥಿರ ಶೂನ್ಯ
  destroy_charmaps( FT_Face    face,
                    FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ )
  {
    FT_Int  n;


    ಆದರೆ ( !face )
      ಹಿಂತಿರುಗಿ;

    ಗಾಗಿ ( n = 0; n < face->ಅಂಕೆ_charmaps; n++ )
    {
      FT_CMap  cmap = FT_CMAP( face->charmaps[n] );


      ft_cmap_ಮುಗಿದ_ಆಂತರಿಕ( cmap );

      face->charmaps[n] = ಖಾಲಿ;
    }

    FT_FREE( face->charmaps );
    face->ಅಂಕೆ_charmaps = 0;
  }


  /* destructor for faces list */
  ಸ್ಥಿರ ಶೂನ್ಯ
  destroy_face( FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ,
                ಶೂನ್ಯ*      face_,
                ಶೂನ್ಯ*      driver_ )
  {
    FT_Face          face   = (FT_Face)face_;
    FT_Driver        driver = (FT_Driver)driver_;
    FT_Driver_Class  clazz  = driver->clazz;


    /* discard auto-hinting data */
    ಆದರೆ ( face->autohint.finalizer )
      face->autohint.finalizer( face->autohint.ದತ್ತಾಂಶ );

    /* Discard glyph slots for this face.                           */
    /* Beware!  FT_Done_GlyphSlot() changes the field `face->glyph' */
    ಆಗ ( face->glyph )
      FT_ಮುಗಿದ_Glyph_ಸ್ಥಾನ( face->glyph );

    /* discard all sizes for this face */
    FT_List_Finalize( &face->sizes_list,
                      destroy_ಗಾತ್ರ,
                      ಸ್ಮರಣೆ,
                      driver );
    face->ಗಾತ್ರ_2 = ಖಾಲಿ;

    /* discard charmaps */
    destroy_charmaps( face, ಸ್ಮರಣೆ );

    /* finalize format-specific stuff */
    ಆದರೆ ( clazz->ಮುಗಿದ_face )
      clazz->ಮುಗಿದ_face( face );

    /* close the stream for this face if needed */
    FT_ಹರಿವು_Free(
      face->ಹರಿವು,
      ( face->face_flags & FT_FACE_FLAG_EXTERNAL_ಹರಿವು ) != 0 );

    face->ಹರಿವು = ಖಾಲಿ;

    /* now discard client data */
    ಆದರೆ ( face->generic.finalizer )
      face->generic.finalizer( face );

    /* get rid of it */
    ಆದರೆ ( face->ಆಂತರಿಕ )
    {
      FT_FREE( face->ಆಂತರಿಕ );
    }
    FT_FREE( face );
  }


  ಸ್ಥಿರ ಶೂನ್ಯ
  Destroy_Driver( FT_Driver  driver )
  {
    FT_List_Finalize( &driver->faces_list,
                      destroy_face,
                      driver->ಬೇರು.ಸ್ಮರಣೆ,
                      driver );
  }


  /* documentation is in ftobjs.h */

  FT_ತಳ_DEF( FT_Error )
  ಕಂಡುಹಿಡಿ_unicode_charmap( FT_Face  face )
  {
    FT_CharMap*  ಮೊದಲ;
    FT_CharMap*  cur;


    /* caller should have already checked that `face' is valid */
    FT_ASSERT( face );

    ಮೊದಲ = face->charmaps;

    ಆದರೆ ( !ಮೊದಲ )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_CharMap_Handle );

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

    cur = ಮೊದಲ + face->ಅಂಕೆ_charmaps;  /* points after the last one */

    ಗಾಗಿ ( ; --cur >= ಮೊದಲ; )
    {
      ಆದರೆ ( cur[0]->encoding == FT_ENCODING_UNICODE )
      {
        /* XXX If some new encodings to represent UCS-4 are added, */
        /*     they should be added here.                          */
        ಆದರೆ ( ( cur[0]->platform_id == TT_PLATFORM_MICROSOFT &&
               cur[0]->encoding_id == TT_MS_ID_UCS_4        )     ||
             ( cur[0]->platform_id == TT_PLATFORM_APPLE_UNICODE &&
               cur[0]->encoding_id == TT_APPLE_ID_UNICODE_32    ) ||
             ( cur[0]->platform_id == TT_PLATFORM_APPLE_UNICODE &&
               cur[0]->encoding_id == TT_APPLE_ID_ತುಂಬಿದ_UNICODE  &&
               FT_Get_CMap_Format( cur[0] ) == 13               ) )
        {
          face->charmap = cur[0];
          ಹಿಂತಿರುಗಿ FT_Err_Ok;
        }
      }
    }

    /* We do not have any UCS-4 charmap.                */
    /* Do the loop again and search for UCS-2 charmaps. */
    cur = ಮೊದಲ + face->ಅಂಕೆ_charmaps;

    ಗಾಗಿ ( ; --cur >= ಮೊದಲ; )
    {
      ಆದರೆ ( cur[0]->encoding == FT_ENCODING_UNICODE )
      {
        FT_Long  format = FT_Get_CMap_Format( cur[0] );


        ಆದರೆ ( format != 14 && format != 15 )
        {
          face->charmap = cur[0];
          ಹಿಂತಿರುಗಿ FT_Err_Ok;
        }
      }
    }

    ಹಿಂತಿರುಗಿ FT_THROW( Invalid_CharMap_Handle );
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
  ಸ್ಥಿರ FT_CharMap
  ಕಂಡುಹಿಡಿ_variant_selector_charmap( FT_Face  face )
  {
    FT_CharMap*  ಮೊದಲ;
    FT_CharMap*  ಕೊನೆ;
    FT_CharMap*  cur;


    /* caller should have already checked that `face' is valid */
    FT_ASSERT( face );

    ಮೊದಲ = face->charmaps;

    ಆದರೆ ( !ಮೊದಲ )
      ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    ಕೊನೆ = ಮೊದಲ + face->ಅಂಕೆ_charmaps;  /* points after the last one */

    ಗಾಗಿ ( cur = ಮೊದಲ; cur < ಕೊನೆ; cur++ )
    {
      ಆದರೆ ( cur[0]->platform_id == TT_PLATFORM_APPLE_UNICODE    &&
           cur[0]->encoding_id == TT_APPLE_ID_VARIANT_SELECTOR )
      {
        FT_Long  format = FT_Get_CMap_Format( cur[0] );


        ಆದರೆ ( format == 14 || format == 15 )
          ಹಿಂತಿರುಗಿ cur[0];
      }
    }

    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
  }


  /**************************************************************************
   *
   * @Function:
   *   open_face
   *
   * @Description:
   *   This function does some work for FT_Open_Face().
   */
  ಸ್ಥಿರ FT_Error
  open_face( FT_Driver      driver,
             FT_ಹರಿವು      *astream,
             FT_Bool        *anexternal_ಹರಿವು,
             FT_Long        face_ಸೂಚ್ಯಂಕ,
             FT_Int         ಅಂಕೆ_params,
             FT_Parameter*  params,
             FT_Face       *aface )
  {
    FT_ಸ್ಮರಣೆ         ಸ್ಮರಣೆ;
    FT_Driver_Class   clazz;
    FT_Face           face     = ಖಾಲಿ;
    FT_Face_ಆಂತರಿಕ  ಆಂತರಿಕ = ಖಾಲಿ;

    FT_Error          error, error2;


    clazz  = driver->clazz;
    ಸ್ಮರಣೆ = driver->ಬೇರು.ಸ್ಮರಣೆ;

    /* allocate the face object and perform basic initialization */
    ಆದರೆ ( FT_ALLOC( face, clazz->face_object_ಗಾತ್ರ ) )
      ಹೋಗು Fail;

    face->driver = driver;
    face->ಸ್ಮರಣೆ = ಸ್ಮರಣೆ;
    face->ಹರಿವು = *astream;

    /* set the FT_FACE_FLAG_EXTERNAL_STREAM bit for FT_Done_Face */
    ಆದರೆ ( *anexternal_ಹರಿವು )
      face->face_flags |= FT_FACE_FLAG_EXTERNAL_ಹರಿವು;

    ಆದರೆ ( FT_NEW( ಆಂತರಿಕ ) )
      ಹೋಗು Fail;

    face->ಆಂತರಿಕ = ಆಂತರಿಕ;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ FT_CONFIG_OPTION_INCREMENTAL
    {
      ಪೂರ್ಣಾಂಕ  i;


      face->ಆಂತರಿಕ->incremental_interface = ಖಾಲಿ;
      ಗಾಗಿ ( i = 0; i < ಅಂಕೆ_params && !face->ಆಂತರಿಕ->incremental_interface;
            i++ )
        ಆದರೆ ( params[i].ಟ್ಯಾಗ್ == FT_PARAM_ಟ್ಯಾಗ್_INCREMENTAL )
          face->ಆಂತರಿಕ->incremental_interface =
            (FT_Incremental_Interface)params[i].ದತ್ತಾಂಶ;
    }
#ಆದರೆ_ಅಂತ್ಯ

    face->ಆಂತರಿಕ->random_ಬೀಜ = -1;

    ಆದರೆ ( clazz->init_face )
      error = clazz->init_face( *astream,
                                face,
                                (FT_Int)face_ಸೂಚ್ಯಂಕ,
                                ಅಂಕೆ_params,
                                params );
    /* Stream may have been changed. */
    *astream = face->ಹರಿವು;
    *anexternal_ಹರಿವು =
      ( face->face_flags & FT_FACE_FLAG_EXTERNAL_ಹರಿವು ) != 0;
    ಆದರೆ ( error )
      ಹೋಗು Fail;

    /* select Unicode charmap by default */
    error2 = ಕಂಡುಹಿಡಿ_unicode_charmap( face );

    /* if no Unicode charmap can be found, FT_Err_Invalid_CharMap_Handle */
    /* is returned.                                                      */

    /* no error should happen, but we want to play safe */
    ಆದರೆ ( error2 && FT_ERR_NEQ( error2, Invalid_CharMap_Handle ) )
    {
      error = error2;
      ಹೋಗು Fail;
    }

    *aface = face;

  Fail:
    ಆದರೆ ( error )
    {
      destroy_charmaps( face, ಸ್ಮರಣೆ );
      ಆದರೆ ( clazz->ಮುಗಿದ_face )
        clazz->ಮುಗಿದ_face( face );
      FT_FREE( ಆಂತರಿಕ );
      FT_FREE( face );
      *aface = ಖಾಲಿ;
    }

    ಹಿಂತಿರುಗಿ error;
  }


  /* there's a Mac-specific extended implementation of FT_New_Face() */
  /* in src/base/ftmac.c                                             */

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ FT_MACINTOSH

  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_New_Face( FT_Library   library,
               ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*  pathname,
               FT_Long      face_ಸೂಚ್ಯಂಕ,
               FT_Face     *aface )
  {
    FT_Open_ವಾದಗಳು  ವಾದಗಳು;


    /* test for valid `library' and `aface' delayed to `FT_Open_Face' */
    ಆದರೆ ( !pathname )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

    ವಾದಗಳು.flags    = FT_OPEN_PATHNAME;
    ವಾದಗಳು.pathname = (ಅಕ್ಷರ*)pathname;
    ವಾದಗಳು.ಹರಿವು   = ಖಾಲಿ;

    ಹಿಂತಿರುಗಿ ft_open_face_ಆಂತರಿಕ( library, &ವಾದಗಳು, face_ಸೂಚ್ಯಂಕ, aface, 1 );
  }

#ಆದರೆ_ಅಂತ್ಯ


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_New_ಸ್ಮರಣೆ_Face( FT_Library      library,
                      ಸ್ಥಿರಾಂಕ FT_Byte*  file_ತಳ,
                      FT_Long         file_ಗಾತ್ರ,
                      FT_Long         face_ಸೂಚ್ಯಂಕ,
                      FT_Face        *aface )
  {
    FT_Open_ವಾದಗಳು  ವಾದಗಳು;


    /* test for valid `library' and `face' delayed to `FT_Open_Face' */
    ಆದರೆ ( !file_ತಳ )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

    ವಾದಗಳು.flags       = FT_OPEN_ಸ್ಮರಣೆ;
    ವಾದಗಳು.ಸ್ಮರಣೆ_ತಳ = file_ತಳ;
    ವಾದಗಳು.ಸ್ಮರಣೆ_ಗಾತ್ರ = file_ಗಾತ್ರ;
    ವಾದಗಳು.ಹರಿವು      = ಖಾಲಿ;

    ಹಿಂತಿರುಗಿ ft_open_face_ಆಂತರಿಕ( library, &ವಾದಗಳು, face_ಸೂಚ್ಯಂಕ, aface, 1 );
  }


#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ FT_CONFIG_OPTION_MAC_FONTS

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
  ಸ್ಥಿರ ಶೂನ್ಯ
  ಸ್ಮರಣೆ_ಹರಿವು_close( FT_ಹರಿವು  ಹರಿವು )
  {
    FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ = (FT_ಸ್ಮರಣೆ)ಹರಿವು->descriptor.ಸೂಚಕ;


    FT_FREE( ಹರಿವು->ತಳ );
    ಹರಿವು->ಗಾತ್ರ_2  = 0;
    ಹರಿವು->close = ಖಾಲಿ;
    FT_FREE( ಹರಿವು );
  }


  /* Create a new memory stream from a buffer and a size. */
  /* From `ftmac.c'.                                      */
  ಸ್ಥಿರ FT_Error
  new_ಸ್ಮರಣೆ_ಹರಿವು( FT_Library           library,
                     FT_Byte*             ತಳ,
                     FT_ULong             ಗಾತ್ರ_2,
                     FT_ಹರಿವು_Close_Func  close,
                     FT_ಹರಿವು           *astream )
  {
    FT_Error   error;
    FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ;
    FT_ಹರಿವು  ಹರಿವು = ಖಾಲಿ;


    ಆದರೆ ( !library )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_Library_Handle );

    ಆದರೆ ( !ತಳ )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

    *astream = ಖಾಲಿ;
    ಸ್ಮರಣೆ   = library->ಸ್ಮರಣೆ;
    ಆದರೆ ( FT_NEW( ಹರಿವು ) )
      ಹೋಗು Exit;

    FT_ಹರಿವು_Open_ಸ್ಮರಣೆ( ಹರಿವು, ತಳ, ಗಾತ್ರ_2 );

    ಹರಿವು->descriptor.ಸೂಚಕ = ಸ್ಮರಣೆ;
    ಹರಿವು->close              = close;

    *astream = ಹರಿವು;

  Exit:
    ಹಿಂತಿರುಗಿ error;
  }


  /* Create a new FT_Face given a buffer and a driver name. */
  /* From `ftmac.c'.                                        */
  FT_LOCAL_DEF( FT_Error )
  open_face_from_ಬಫರ್( FT_Library   library,
                         FT_Byte*     ತಳ,
                         FT_ULong     ಗಾತ್ರ_2,
                         FT_Long      face_ಸೂಚ್ಯಂಕ,
                         ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*  driver_name,
                         FT_Face     *aface )
  {
    FT_Open_ವಾದಗಳು  ವಾದಗಳು;
    FT_Error      error;
    FT_ಸ್ಮರಣೆ     ಸ್ಮರಣೆ = library->ಸ್ಮರಣೆ;


    ವಾದಗಳು.driver = ಖಾಲಿ;
    ವಾದಗಳು.flags  = 0;

    ಆದರೆ ( driver_name )
    {
      ವಾದಗಳು.driver = FT_Get_Module( library, driver_name );
      ಆದರೆ ( !ವಾದಗಳು.driver )
      {
        FT_FREE( ತಳ );
        ಹಿಂತಿರುಗಿ FT_THROW( Missing_Module );
      }

      ವಾದಗಳು.flags = ವಾದಗಳು.flags | FT_OPEN_DRIVER;
    }

    /* `memory_stream_close` also frees the stream object. */
    error = new_ಸ್ಮರಣೆ_ಹರಿವು( library,
                               ತಳ,
                               ಗಾತ್ರ_2,
                               ಸ್ಮರಣೆ_ಹರಿವು_close,
                               &ವಾದಗಳು.ಹರಿವು );
    ಆದರೆ ( error )
    {
      FT_FREE( ತಳ );
      ಹಿಂತಿರುಗಿ error;
    }

    ವಾದಗಳು.flags |= FT_OPEN_ಹರಿವು;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ FT_MACINTOSH
    /* At this point, the face index has served its purpose;  */
    /* whoever calls this function has already used it to     */
    /* locate the correct font data.  We should not propagate */
    /* this index to FT_Open_Face() (unless it is negative).  */

    ಆದರೆ ( face_ಸೂಚ್ಯಂಕ > 0 )
      face_ಸೂಚ್ಯಂಕ &= 0x7FFF0000L; /* retain GX data */
#ಆದರೆ_ಅಂತ್ಯ

    ಹಿಂತಿರುಗಿ ft_open_face_ಆಂತರಿಕ( library, &ವಾದಗಳು, face_ಸೂಚ್ಯಂಕ, aface, 0 );
  }


  /* Look up `TYP1' or `CID ' table from sfnt table directory.       */
  /* `offset' and `length' must exclude the binary header in tables. */

  /* Type 1 and CID-keyed font drivers should recognize sfnt-wrapped */
  /* format too.  Here, since we can't expect that the TrueType font */
  /* driver is loaded unconditionally, we must parse the font by     */
  /* ourselves.  We are only interested in the name of the table and */
  /* the offset.                                                     */

  ಸ್ಥಿರ FT_Error
  ft_lookup_PS_ಒಳ_sfnt_ಹರಿವು( FT_ಹರಿವು  ಹರಿವು,
                               FT_Long    face_ಸೂಚ್ಯಂಕ,
                               FT_ULong*  ಸ್ಥಾನಾಂತರ,
                               FT_ULong*  ಉದ್ದಳತೆ,
                               FT_Bool*   is_sfnt_cid )
  {
    FT_Error   error;
    FT_UShort  ಅಂಕೆ_Tables;
    FT_Long    pstable_ಸೂಚ್ಯಂಕ;
    FT_ULong   ಟ್ಯಾಗ್;
    ಪೂರ್ಣಾಂಕ        i;


    *ಸ್ಥಾನಾಂತರ = 0;
    *ಉದ್ದಳತೆ = 0;
    *is_sfnt_cid = FALSE;

    /* TODO: support for sfnt-wrapped PS/CID in TTC format */

    /* version check for 'typ1' (should be ignored?) */
    ಆದರೆ ( FT_READ_ULONG( ಟ್ಯಾಗ್ ) )
      ಹಿಂತಿರುಗಿ error;
    ಆದರೆ ( ಟ್ಯಾಗ್ != TTAG_typ1 )
      ಹಿಂತಿರುಗಿ FT_THROW( Unknown_File_Format );

    ಆದರೆ ( FT_READ_USHORT( ಅಂಕೆ_Tables ) )
      ಹಿಂತಿರುಗಿ error;
    ಆದರೆ ( FT_ಹರಿವು_SKIP( 2 * 3 ) ) /* skip binary search header */
      ಹಿಂತಿರುಗಿ error;

    pstable_ಸೂಚ್ಯಂಕ = -1;
    *is_sfnt_cid  = FALSE;

    ಗಾಗಿ ( i = 0; i < ಅಂಕೆ_Tables; i++ )
    {
      ಆದರೆ ( FT_READ_ULONG( ಟ್ಯಾಗ್ )     || FT_ಹರಿವು_SKIP( 4 )      ||
           FT_READ_ULONG( *ಸ್ಥಾನಾಂತರ ) || FT_READ_ULONG( *ಉದ್ದಳತೆ ) )
        ಹಿಂತಿರುಗಿ error;

      ಆದರೆ ( ಟ್ಯಾಗ್ == TTAG_CID )
      {
        pstable_ಸೂಚ್ಯಂಕ++;
        *ಸ್ಥಾನಾಂತರ += 22;
        *ಉದ್ದಳತೆ -= 22;
        *is_sfnt_cid = TRUE;
        ಆದರೆ ( face_ಸೂಚ್ಯಂಕ < 0 )
          ಹಿಂತಿರುಗಿ FT_Err_Ok;
      }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ( ಟ್ಯಾಗ್ == TTAG_TYP1 )
      {
        pstable_ಸೂಚ್ಯಂಕ++;
        *ಸ್ಥಾನಾಂತರ += 24;
        *ಉದ್ದಳತೆ -= 24;
        *is_sfnt_cid = FALSE;
        ಆದರೆ ( face_ಸೂಚ್ಯಂಕ < 0 )
          ಹಿಂತಿರುಗಿ FT_Err_Ok;
      }
      ಆದರೆ ( face_ಸೂಚ್ಯಂಕ >= 0 && pstable_ಸೂಚ್ಯಂಕ == face_ಸೂಚ್ಯಂಕ )
        ಹಿಂತಿರುಗಿ FT_Err_Ok;
    }

    ಹಿಂತಿರುಗಿ FT_THROW( ಕೋಷ್ಟಕ_Missing );
  }


  FT_LOCAL_DEF( FT_Error )
  open_face_PS_from_sfnt_ಹರಿವು( FT_Library     library,
                                 FT_ಹರಿವು      ಹರಿವು,
                                 FT_Long        face_ಸೂಚ್ಯಂಕ,
                                 FT_Int         ಅಂಕೆ_params,
                                 FT_Parameter  *params,
                                 FT_Face       *aface )
  {
    FT_Error   error;
    FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ = library->ಸ್ಮರಣೆ;
    FT_ULong   ಸ್ಥಾನಾಂತರ, ಉದ್ದಳತೆ;
    FT_ULong   pos;
    FT_Bool    is_sfnt_cid;
    FT_Byte*   sfnt_ps = ಖಾಲಿ;

    FT_ಬಳಸದ( ಅಂಕೆ_params );
    FT_ಬಳಸದ( params );


    /* ignore GX stuff */
    ಆದರೆ ( face_ಸೂಚ್ಯಂಕ > 0 )
      face_ಸೂಚ್ಯಂಕ &= 0xFFFFL;

    pos = FT_ಹರಿವು_POS();

    error = ft_lookup_PS_ಒಳ_sfnt_ಹರಿವು( ಹರಿವು,
                                         face_ಸೂಚ್ಯಂಕ,
                                         &ಸ್ಥಾನಾಂತರ,
                                         &ಉದ್ದಳತೆ,
                                         &is_sfnt_cid );
    ಆದರೆ ( error )
      ಹೋಗು Exit;

    ಆದರೆ ( ಸ್ಥಾನಾಂತರ > ಹರಿವು->ಗಾತ್ರ_2 )
    {
      FT_TRACE2(( "open_face_PS_from_sfnt_stream: invalid table offset\n" ));
      error = FT_THROW( Invalid_ಕೋಷ್ಟಕ );
      ಹೋಗು Exit;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ( ಉದ್ದಳತೆ > ಹರಿವು->ಗಾತ್ರ_2 - ಸ್ಥಾನಾಂತರ )
    {
      FT_TRACE2(( "open_face_PS_from_sfnt_stream: invalid table length\n" ));
      error = FT_THROW( Invalid_ಕೋಷ್ಟಕ );
      ಹೋಗು Exit;
    }

    error = FT_ಹರಿವು_Seek( ಹರಿವು, pos + ಸ್ಥಾನಾಂತರ );
    ಆದರೆ ( error )
      ಹೋಗು Exit;

    ಆದರೆ ( FT_QALLOC( sfnt_ps, (FT_Long)ಉದ್ದಳತೆ ) )
      ಹೋಗು Exit;

    error = FT_ಹರಿವು_Read( ಹರಿವು, (FT_Byte *)sfnt_ps, ಉದ್ದಳತೆ );
    ಆದರೆ ( error )
    {
      FT_FREE( sfnt_ps );
      ಹೋಗು Exit;
    }

    error = open_face_from_ಬಫರ್( library,
                                   sfnt_ps,
                                   ಉದ್ದಳತೆ,
                                   FT_MIN( face_ಸೂಚ್ಯಂಕ, 0 ),
                                   is_sfnt_cid ? "t1cid" : "type1",
                                   aface );
  Exit:
    {
      FT_Error  error1;


      ಆದರೆ ( FT_ERR_EQ( error, Unknown_File_Format ) )
      {
        error1 = FT_ಹರಿವು_Seek( ಹರಿವು, pos );
        ಆದರೆ ( error1 )
          ಹಿಂತಿರುಗಿ error1;
      }

      ಹಿಂತಿರುಗಿ error;
    }
  }


#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ FT_MACINTOSH

  /* The resource header says we've got resource_cnt `POST' (type1) */
  /* resources in this file.  They all need to be coalesced into    */
  /* one lump which gets passed on to the type1 driver.             */
  /* Here can be only one PostScript font in a file so face_index   */
  /* must be 0 (or -1).                                             */
  /*                                                                */
  ಸ್ಥಿರ FT_Error
  Mac_Read_POST_Resource( FT_Library  library,
                          FT_ಹರಿವು   ಹರಿವು,
                          FT_Long    *offsets,
                          FT_Long     resource_cnt,
                          FT_Long     face_ಸೂಚ್ಯಂಕ,
                          FT_Face    *aface )
  {
    FT_Error   error  = FT_ERR( Cannot_Open_Resource );
    FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ = library->ಸ್ಮರಣೆ;

    FT_Byte*   pfb_ದತ್ತಾಂಶ = ಖಾಲಿ;
    ಪೂರ್ಣಾಂಕ        i, type, flags;
    FT_ULong   len;
    FT_ULong   pfb_len, pfb_pos, pfb_lenpos;
    FT_ULong   rlen, ತಾತ್ಕಾಲಿಕ;


    ಆದರೆ ( face_ಸೂಚ್ಯಂಕ == -1 )
      face_ಸೂಚ್ಯಂಕ = 0;
    ಆದರೆ ( face_ಸೂಚ್ಯಂಕ != 0 )
      ಹಿಂತಿರುಗಿ error;

    /* Find the length of all the POST resources, concatenated.  Assume */
    /* worst case (each resource in its own section).                   */
    pfb_len = 0;
    ಗಾಗಿ ( i = 0; i < resource_cnt; i++ )
    {
      error = FT_ಹರಿವು_Seek( ಹರಿವು, (FT_ULong)offsets[i] );
      ಆದರೆ ( error )
        ಹೋಗು Exit;
      ಆದರೆ ( FT_READ_ULONG( ತಾತ್ಕಾಲಿಕ ) )  /* actually LONG */
        ಹೋಗು Exit;

      /* FT2 allocator takes signed long buffer length,
       * too large value causing overflow should be checked
       */
      FT_TRACE4(( "                 POST fragment #%d: length=0x%08lx"
                  " total pfb_len=0x%08lx\n",
                  i, ತಾತ್ಕಾಲಿಕ, pfb_len + ತಾತ್ಕಾಲಿಕ + 6 ));

      ಆದರೆ ( FT_MAC_RFORK_MAX_LEN < ತಾತ್ಕಾಲಿಕ               ||
           FT_MAC_RFORK_MAX_LEN - ತಾತ್ಕಾಲಿಕ < pfb_len + 6 )
      {
        FT_TRACE2(( "             MacOS resource length cannot exceed"
                    " 0x%08lx\n",
                    FT_MAC_RFORK_MAX_LEN ));

        error = FT_THROW( Invalid_ಸ್ಥಾನಾಂತರ );
        ಹೋಗು Exit;
      }

      pfb_len += ತಾತ್ಕಾಲಿಕ + 6;
    }

    FT_TRACE2(( "             total buffer size to concatenate"
                " %ld POST fragments: 0x%08lx\n",
                 resource_cnt, pfb_len + 2 ));

    ಆದರೆ ( pfb_len + 2 < 6 )
    {
      FT_TRACE2(( "             too long fragment length makes"
                  " pfb_len confused: pfb_len=0x%08lx\n",
                  pfb_len ));

      error = FT_THROW( Array_Too_Large );
      ಹೋಗು Exit;
    }

    ಆದರೆ ( FT_QALLOC( pfb_ದತ್ತಾಂಶ, (FT_Long)pfb_len + 2 ) )
      ಹೋಗು Exit;

    pfb_ದತ್ತಾಂಶ[0] = 0x80;
    pfb_ದತ್ತಾಂಶ[1] = 1;            /* Ascii section */
    pfb_ದತ್ತಾಂಶ[2] = 0;            /* 4-byte length, fill in later */
    pfb_ದತ್ತಾಂಶ[3] = 0;
    pfb_ದತ್ತಾಂಶ[4] = 0;
    pfb_ದತ್ತಾಂಶ[5] = 0;
    pfb_pos     = 6;
    pfb_lenpos  = 2;

    len  = 0;
    type = 1;

    ಗಾಗಿ ( i = 0; i < resource_cnt; i++ )
    {
      error = FT_ಹರಿವು_Seek( ಹರಿವು, (FT_ULong)offsets[i] );
      ಆದರೆ ( error )
        ಹೋಗು Exit2;
      ಆದರೆ ( FT_READ_ULONG( rlen ) )
        ಹೋಗು Exit2;

      /* FT2 allocator takes signed long buffer length,
       * too large fragment length causing overflow should be checked
       */
      ಆದರೆ ( 0x7FFFFFFFUL < rlen )
      {
        error = FT_THROW( Invalid_ಸ್ಥಾನಾಂತರ );
        ಹೋಗು Exit2;
      }

      ಆದರೆ ( FT_READ_USHORT( flags ) )
        ಹೋಗು Exit2;

      FT_TRACE3(( "POST fragment[%d]:"
                  " offsets=0x%08lx, rlen=0x%08lx, flags=0x%04x\n",
                  i, offsets[i], rlen, flags ));

      error = FT_ERR( Array_Too_Large );

      /* postpone the check of `rlen longer than buffer' */
      /* until `FT_Stream_Read'                          */

      ಆದರೆ ( ( flags >> 8 ) == 0 )        /* Comment, should not be loaded */
      {
        FT_TRACE3(( "    Skip POST fragment #%d because it is a comment\n",
                    i ));
        ಮುಂದುವರಿ;
      }

      /* the flags are part of the resource, so rlen >= 2,  */
      /* but some fonts declare rlen = 0 for empty fragment */
      ಆದರೆ ( rlen > 2 )
        rlen -= 2;
      ಇಲ್ಲದಿದ್ದರೆ
        rlen = 0;

      ಆದರೆ ( ( flags >> 8 ) == type )
        len += rlen;
      ಇಲ್ಲದಿದ್ದರೆ
      {
        FT_TRACE3(( "    Write POST fragment #%d header (4-byte) to buffer"
                    " %p + 0x%08lx\n",
                    i, (ಶೂನ್ಯ*)pfb_ದತ್ತಾಂಶ, pfb_lenpos ));

        ಆದರೆ ( pfb_lenpos + 3 > pfb_len + 2 )
          ಹೋಗು Exit2;

        pfb_ದತ್ತಾಂಶ[pfb_lenpos    ] = (FT_Byte)( len );
        pfb_ದತ್ತಾಂಶ[pfb_lenpos + 1] = (FT_Byte)( len >> 8 );
        pfb_ದತ್ತಾಂಶ[pfb_lenpos + 2] = (FT_Byte)( len >> 16 );
        pfb_ದತ್ತಾಂಶ[pfb_lenpos + 3] = (FT_Byte)( len >> 24 );

        ಆದರೆ ( ( flags >> 8 ) == 5 )      /* End of font mark */
          ನಿಲ್ಲಿಸು;

        FT_TRACE3(( "    Write POST fragment #%d header (6-byte) to buffer"
                    " %p + 0x%08lx\n",
                    i, (ಶೂನ್ಯ*)pfb_ದತ್ತಾಂಶ, pfb_pos ));

        ಆದರೆ ( pfb_pos + 6 > pfb_len + 2 )
          ಹೋಗು Exit2;

        pfb_ದತ್ತಾಂಶ[pfb_pos++] = 0x80;

        type = flags >> 8;
        len  = rlen;

        pfb_ದತ್ತಾಂಶ[pfb_pos++] = (FT_Byte)type;
        pfb_lenpos          = pfb_pos;
        pfb_ದತ್ತಾಂಶ[pfb_pos++] = 0;        /* 4-byte length, fill in later */
        pfb_ದತ್ತಾಂಶ[pfb_pos++] = 0;
        pfb_ದತ್ತಾಂಶ[pfb_pos++] = 0;
        pfb_ದತ್ತಾಂಶ[pfb_pos++] = 0;
      }

      ಆದರೆ ( pfb_pos > pfb_len || pfb_pos + rlen > pfb_len )
        ಹೋಗು Exit2;

      FT_TRACE3(( "    Load POST fragment #%d (%lu byte) to buffer"
                  " %p + 0x%08lx\n",
                  i, rlen, (ಶೂನ್ಯ*)pfb_ದತ್ತಾಂಶ, pfb_pos ));

      error = FT_ಹರಿವು_Read( ಹರಿವು, (FT_Byte *)pfb_ದತ್ತಾಂಶ + pfb_pos, rlen );
      ಆದರೆ ( error )
        ಹೋಗು Exit2;

      pfb_pos += rlen;
    }

    error = FT_ERR( Array_Too_Large );

    ಆದರೆ ( pfb_pos + 2 > pfb_len + 2 )
      ಹೋಗು Exit2;
    pfb_ದತ್ತಾಂಶ[pfb_pos++] = 0x80;
    pfb_ದತ್ತಾಂಶ[pfb_pos++] = 3;

    ಆದರೆ ( pfb_lenpos + 3 > pfb_len + 2 )
      ಹೋಗು Exit2;
    pfb_ದತ್ತಾಂಶ[pfb_lenpos    ] = (FT_Byte)( len );
    pfb_ದತ್ತಾಂಶ[pfb_lenpos + 1] = (FT_Byte)( len >> 8 );
    pfb_ದತ್ತಾಂಶ[pfb_lenpos + 2] = (FT_Byte)( len >> 16 );
    pfb_ದತ್ತಾಂಶ[pfb_lenpos + 3] = (FT_Byte)( len >> 24 );

    ಹಿಂತಿರುಗಿ open_face_from_ಬಫರ್( library,
                                  pfb_ದತ್ತಾಂಶ,
                                  pfb_pos,
                                  face_ಸೂಚ್ಯಂಕ,
                                  "type1",
                                  aface );

  Exit2:
    ಆದರೆ ( FT_ERR_EQ( error, Array_Too_Large ) )
      FT_TRACE2(( "  Abort due to too-short buffer to store"
                  " all POST fragments\n" ));
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ( FT_ERR_EQ( error, Invalid_ಸ್ಥಾನಾಂತರ ) )
      FT_TRACE2(( "  Abort due to invalid offset in a POST fragment\n" ));

    ಆದರೆ ( error )
      error = FT_ERR( Cannot_Open_Resource );
    FT_FREE( pfb_ದತ್ತಾಂಶ );

  Exit:
    ಹಿಂತಿರುಗಿ error;
  }


  /* The resource header says we've got resource_cnt `sfnt'      */
  /* (TrueType/OpenType) resources in this file.  Look through   */
  /* them for the one indicated by face_index, load it into mem, */
  /* pass it on to the truetype driver, and return it.           */
  /*                                                             */
  ಸ್ಥಿರ FT_Error
  Mac_Read_sfnt_Resource( FT_Library  library,
                          FT_ಹರಿವು   ಹರಿವು,
                          FT_Long    *offsets,
                          FT_Long     resource_cnt,
                          FT_Long     face_ಸೂಚ್ಯಂಕ,
                          FT_Face    *aface )
  {
    FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ = library->ಸ್ಮರಣೆ;
    FT_Byte*   sfnt_ದತ್ತಾಂಶ = ಖಾಲಿ;
    FT_Error   error;
    FT_ULong   flag_ಸ್ಥಾನಾಂತರ;
    FT_ULong   rlen;
    ಪೂರ್ಣಾಂಕ        is_cff;
    FT_Long    face_ಸೂಚ್ಯಂಕ_ಒಳ_resource = 0;


    ಆದರೆ ( face_ಸೂಚ್ಯಂಕ < 0 )
      face_ಸೂಚ್ಯಂಕ = -face_ಸೂಚ್ಯಂಕ - 1;
    ಆದರೆ ( face_ಸೂಚ್ಯಂಕ >= resource_cnt )
      ಹಿಂತಿರುಗಿ FT_THROW( Cannot_Open_Resource );

    flag_ಸ್ಥಾನಾಂತರ = (FT_ULong)offsets[face_ಸೂಚ್ಯಂಕ];
    error = FT_ಹರಿವು_Seek( ಹರಿವು, flag_ಸ್ಥಾನಾಂತರ );
    ಆದರೆ ( error )
      ಹೋಗು Exit;

    ಆದರೆ ( FT_READ_ULONG( rlen ) )
      ಹೋಗು Exit;
    ಆದರೆ ( !rlen )
      ಹಿಂತಿರುಗಿ FT_THROW( Cannot_Open_Resource );
    ಆದರೆ ( rlen > FT_MAC_RFORK_MAX_LEN )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ಸ್ಥಾನಾಂತರ );

    error = open_face_PS_from_sfnt_ಹರಿವು( library,
                                           ಹರಿವು,
                                           face_ಸೂಚ್ಯಂಕ,
                                           0, ಖಾಲಿ,
                                           aface );
    ಆದರೆ ( !error )
      ಹೋಗು Exit;

    /* rewind sfnt stream before open_face_PS_from_sfnt_stream() */
    error = FT_ಹರಿವು_Seek( ಹರಿವು, flag_ಸ್ಥಾನಾಂತರ + 4 );
    ಆದರೆ ( error )
      ಹೋಗು Exit;

    ಆದರೆ ( FT_QALLOC( sfnt_ದತ್ತಾಂಶ, rlen ) )
      ಹಿಂತಿರುಗಿ error;
    error = FT_ಹರಿವು_Read( ಹರಿವು, (FT_Byte *)sfnt_ದತ್ತಾಂಶ, rlen );
    ಆದರೆ ( error )
    {
      FT_FREE( sfnt_ದತ್ತಾಂಶ );
      ಹೋಗು Exit;
    }

    is_cff = rlen > 4 && !ft_memcmp( sfnt_ದತ್ತಾಂಶ, "OTTO", 4 );
    error = open_face_from_ಬಫರ್( library,
                                   sfnt_ದತ್ತಾಂಶ,
                                   rlen,
                                   face_ಸೂಚ್ಯಂಕ_ಒಳ_resource,
                                   is_cff ? "cff" : "truetype",
                                   aface );

  Exit:
    ಹಿಂತಿರುಗಿ error;
  }


  /* Check for a valid resource fork header, or a valid dfont    */
  /* header.  In a resource fork the first 16 bytes are repeated */
  /* at the location specified by bytes 4-7.  In a dfont bytes   */
  /* 4-7 point to 16 bytes of zeroes instead.                    */
  /*                                                             */
  ಸ್ಥಿರ FT_Error
  IsMacResource( FT_Library  library,
                 FT_ಹರಿವು   ಹರಿವು,
                 FT_Long     resource_ಸ್ಥಾನಾಂತರ,
                 FT_Long     face_ಸೂಚ್ಯಂಕ,
                 FT_Face    *aface )
  {
    FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ = library->ಸ್ಮರಣೆ;
    FT_Error   error;
    FT_Long    map_ಸ್ಥಾನಾಂತರ, rdata_pos;
    FT_Long    *ದತ್ತಾಂಶ_offsets;
    FT_Long    ಎಣಿಕೆ_2;


    error = FT_Raccess_Get_ಶೀರ್ಷಿಕೆ_Info( library, ಹರಿವು, resource_ಸ್ಥಾನಾಂತರ,
                                       &map_ಸ್ಥಾನಾಂತರ, &rdata_pos );
    ಆದರೆ ( error )
      ಹಿಂತಿರುಗಿ error;

    /* POST resources must be sorted to concatenate properly */
    error = FT_Raccess_Get_ದತ್ತಾಂಶ_Offsets( library, ಹರಿವು,
                                        map_ಸ್ಥಾನಾಂತರ, rdata_pos,
                                        TTAG_POST, TRUE,
                                        &ದತ್ತಾಂಶ_offsets, &ಎಣಿಕೆ_2 );
    ಆದರೆ ( !error )
    {
      error = Mac_Read_POST_Resource( library, ಹರಿವು, ದತ್ತಾಂಶ_offsets, ಎಣಿಕೆ_2,
                                      face_ಸೂಚ್ಯಂಕ, aface );
      FT_FREE( ದತ್ತಾಂಶ_offsets );
      /* POST exists in an LWFN providing a single face */
      ಆದರೆ ( !error )
        (*aface)->ಅಂಕೆ_faces = 1;
      ಹಿಂತಿರುಗಿ error;
    }

    /* sfnt resources should not be sorted to preserve the face order by
       QuickDraw API */
    error = FT_Raccess_Get_ದತ್ತಾಂಶ_Offsets( library, ಹರಿವು,
                                        map_ಸ್ಥಾನಾಂತರ, rdata_pos,
                                        TTAG_sfnt, FALSE,
                                        &ದತ್ತಾಂಶ_offsets, &ಎಣಿಕೆ_2 );
    ಆದರೆ ( !error )
    {
      FT_Long  face_ಸೂಚ್ಯಂಕ_ಆಂತರಿಕ = face_ಸೂಚ್ಯಂಕ % ಎಣಿಕೆ_2;


      error = Mac_Read_sfnt_Resource( library, ಹರಿವು, ದತ್ತಾಂಶ_offsets, ಎಣಿಕೆ_2,
                                      face_ಸೂಚ್ಯಂಕ_ಆಂತರಿಕ, aface );
      FT_FREE( ದತ್ತಾಂಶ_offsets );
      ಆದರೆ ( !error )
      {
        (*aface)->ಅಂಕೆ_faces  = ಎಣಿಕೆ_2;
        (*aface)->face_ಸೂಚ್ಯಂಕ = face_ಸೂಚ್ಯಂಕ_ಆಂತರಿಕ;
      }
    }

    ಹಿಂತಿರುಗಿ error;
  }


  /* Check for a valid macbinary header, and if we find one   */
  /* check that the (flattened) resource fork in it is valid. */
  /*                                                          */
  ಸ್ಥಿರ FT_Error
  Is_Mac_ದ್ವಿಮಾನ( FT_Library  library,
               FT_ಹರಿವು   ಹರಿವು,
               FT_Long     face_ಸೂಚ್ಯಂಕ,
               FT_Face    *aface )
  {
    ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ  ಶೀರ್ಷಿಕೆ[128];
    FT_Error       error;
    FT_Long        dlen, ಸ್ಥಾನಾಂತರ;


    ಆದರೆ ( !ಹರಿವು )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ಹರಿವು_Operation );

    error = FT_ಹರಿವು_Seek( ಹರಿವು, 0 );
    ಆದರೆ ( error )
      ಹೋಗು Exit;

    error = FT_ಹರಿವು_Read( ಹರಿವು, (FT_Byte*)ಶೀರ್ಷಿಕೆ, 128 );
    ಆದರೆ ( error )
      ಹೋಗು Exit;

    ಆದರೆ (            ಶೀರ್ಷಿಕೆ[ 0] !=   0 ||
                    ಶೀರ್ಷಿಕೆ[74] !=   0 ||
                    ಶೀರ್ಷಿಕೆ[82] !=   0 ||
                    ಶೀರ್ಷಿಕೆ[ 1] ==   0 ||
                    ಶೀರ್ಷಿಕೆ[ 1] >   33 ||
                    ಶೀರ್ಷಿಕೆ[63] !=   0 ||
         ಶೀರ್ಷಿಕೆ[2 + ಶೀರ್ಷಿಕೆ[1]] !=   0 ||
                  ಶೀರ್ಷಿಕೆ[0x53] > 0x7F )
      ಹಿಂತಿರುಗಿ FT_THROW( Unknown_File_Format );

    dlen = ( ಶೀರ್ಷಿಕೆ[0x53] << 24 ) |
           ( ಶೀರ್ಷಿಕೆ[0x54] << 16 ) |
           ( ಶೀರ್ಷಿಕೆ[0x55] <<  8 ) |
             ಶೀರ್ಷಿಕೆ[0x56];
#ಆದರೆ 0
    rlen = ( ಶೀರ್ಷಿಕೆ[0x57] << 24 ) |
           ( ಶೀರ್ಷಿಕೆ[0x58] << 16 ) |
           ( ಶೀರ್ಷಿಕೆ[0x59] <<  8 ) |
             ಶೀರ್ಷಿಕೆ[0x5A];
#ಆದರೆ_ಅಂತ್ಯ /* 0 */
    ಸ್ಥಾನಾಂತರ = 128 + ( ( dlen + 127 ) & ~127 );

    ಹಿಂತಿರುಗಿ IsMacResource( library, ಹರಿವು, ಸ್ಥಾನಾಂತರ, face_ಸೂಚ್ಯಂಕ, aface );

  Exit:
    ಹಿಂತಿರುಗಿ error;
  }


  ಸ್ಥಿರ FT_Error
  ತುಂಬು_face_ಒಳ_embedded_rfork( FT_Library           library,
                               FT_ಹರಿವು            ಹರಿವು,
                               FT_Long              face_ಸೂಚ್ಯಂಕ,
                               FT_Face             *aface,
                               ಸ್ಥಿರಾಂಕ FT_Open_ವಾದಗಳು  *ವಾದಗಳು )
  {

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು  FT_COMPONENT
#ವ್ಯಾಖ್ಯಾನಿಸು FT_COMPONENT  raccess

    FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ = library->ಸ್ಮರಣೆ;
    FT_Error   error  = FT_ERR( Unknown_File_Format );
    FT_UInt    i;

    ಅಕ್ಷರ*      file_ಹೆಸರುಗಳು[FT_RACCESS_N_ನಿಯಮಗಳು];
    FT_Long    offsets[FT_RACCESS_N_ನಿಯಮಗಳು];
    FT_Error   errors[FT_RACCESS_N_ನಿಯಮಗಳು];
    FT_Bool    is_darwin_vfs, vfs_rfork_has_no_font = FALSE; /* not tested */

    FT_Open_ವಾದಗಳು  args2;
    FT_ಹರಿವು     stream2 = ಖಾಲಿ;


    FT_Raccess_Guess( library, ಹರಿವು,
                      ವಾದಗಳು->pathname, file_ಹೆಸರುಗಳು, offsets, errors );

    ಗಾಗಿ ( i = 0; i < FT_RACCESS_N_ನಿಯಮಗಳು; i++ )
    {
      is_darwin_vfs = ft_raccess_ನಿಯಮ_by_darwin_vfs( library, i );
      ಆದರೆ ( is_darwin_vfs && vfs_rfork_has_no_font )
      {
        FT_TRACE3(( "Skip rule %u: darwin vfs resource fork"
                    " is already checked and"
                    " no font is found\n",
                    i ));
        ಮುಂದುವರಿ;
      }

      ಆದರೆ ( errors[i] )
      {
        FT_TRACE3(( "Error 0x%x has occurred in rule %u\n",
                    errors[i], i ));
        ಮುಂದುವರಿ;
      }

      args2.flags    = FT_OPEN_PATHNAME;
      args2.pathname = file_ಹೆಸರುಗಳು[i] ? file_ಹೆಸರುಗಳು[i] : ವಾದಗಳು->pathname;

      FT_TRACE3(( "Try rule %u: %s (offset=%ld) ...",
                  i, args2.pathname, offsets[i] ));

      error = FT_ಹರಿವು_New( library, &args2, &stream2 );
      ಆದರೆ ( is_darwin_vfs && FT_ERR_EQ( error, Cannot_Open_ಹರಿವು ) )
        vfs_rfork_has_no_font = TRUE;

      ಆದರೆ ( error )
      {
        FT_TRACE3(( "failed\n" ));
        ಮುಂದುವರಿ;
      }

      error = IsMacResource( library, stream2, offsets[i],
                             face_ಸೂಚ್ಯಂಕ, aface );
      FT_ಹರಿವು_Free( stream2, 0 );

      FT_TRACE3(( "%s\n", error ? "failed": "successful" ));

      ಆದರೆ ( !error )
          ನಿಲ್ಲಿಸು;
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ( is_darwin_vfs )
          vfs_rfork_has_no_font = TRUE;
    }

    ಗಾಗಿ (i = 0; i < FT_RACCESS_N_ನಿಯಮಗಳು; i++)
    {
      ಆದರೆ ( file_ಹೆಸರುಗಳು[i] )
        FT_FREE( file_ಹೆಸರುಗಳು[i] );
    }

    /* Caller (load_mac_face) requires FT_Err_Unknown_File_Format. */
    ಆದರೆ ( error )
      error = FT_ERR( Unknown_File_Format );

    ಹಿಂತಿರುಗಿ error;

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು  FT_COMPONENT
#ವ್ಯಾಖ್ಯಾನಿಸು FT_COMPONENT  objs

  }


  /* Check for some macintosh formats without Carbon framework.    */
  /* Is this a macbinary file?  If so look at the resource fork.   */
  /* Is this a mac dfont file?                                     */
  /* Is this an old style resource fork? (in data)                 */
  /* Else call load_face_in_embedded_rfork to try extra rules      */
  /* (defined in `ftrfork.c').                                     */
  /*                                                               */
  ಸ್ಥಿರ FT_Error
  ತುಂಬು_mac_face( FT_Library           library,
                 FT_ಹರಿವು            ಹರಿವು,
                 FT_Long              face_ಸೂಚ್ಯಂಕ,
                 FT_Face             *aface,
                 ಸ್ಥಿರಾಂಕ FT_Open_ವಾದಗಳು  *ವಾದಗಳು )
  {
    FT_Error error;
    FT_ಬಳಸದ( ವಾದಗಳು );


    error = Is_Mac_ದ್ವಿಮಾನ( library, ಹರಿವು, face_ಸೂಚ್ಯಂಕ, aface );
    ಆದರೆ ( FT_ERR_EQ( error, Unknown_File_Format ) )
    {

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು  FT_COMPONENT
#ವ್ಯಾಖ್ಯಾನಿಸು FT_COMPONENT  raccess

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ FT_DEBUG_LEVEL_TRACE
      FT_TRACE3(( "Try as dfont: " ));
      ಆದರೆ ( !( ವಾದಗಳು->flags & FT_OPEN_ಸ್ಮರಣೆ ) )
        FT_TRACE3(( "%s ...", ವಾದಗಳು->pathname ));
#ಆದರೆ_ಅಂತ್ಯ

      error = IsMacResource( library, ಹರಿವು, 0, face_ಸೂಚ್ಯಂಕ, aface );

      FT_TRACE3(( "%s\n", error ? "failed" : "successful" ));

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು  FT_COMPONENT
#ವ್ಯಾಖ್ಯಾನಿಸು FT_COMPONENT  objs

    }

    ಆದರೆ ( ( FT_ERR_EQ( error, Unknown_File_Format )      ||
           FT_ERR_EQ( error, Invalid_ಹರಿವು_Operation ) ) &&
         ( ವಾದಗಳು->flags & FT_OPEN_PATHNAME )               )
      error = ತುಂಬು_face_ಒಳ_embedded_rfork( library, ಹರಿವು,
                                           face_ಸೂಚ್ಯಂಕ, aface, ವಾದಗಳು );
    ಹಿಂತಿರುಗಿ error;
  }
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ_ಅಂತ್ಯ  /* !FT_MACINTOSH && FT_CONFIG_OPTION_MAC_FONTS */


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Open_Face( FT_Library           library,
                ಸ್ಥಿರಾಂಕ FT_Open_ವಾದಗಳು*  ವಾದಗಳು,
                FT_Long              face_ಸೂಚ್ಯಂಕ,
                FT_Face             *aface )
  {
    ಹಿಂತಿರುಗಿ ft_open_face_ಆಂತರಿಕ( library, ವಾದಗಳು, face_ಸೂಚ್ಯಂಕ, aface, 1 );
  }


  ಸ್ಥಿರ FT_Error
  ft_open_face_ಆಂತರಿಕ( FT_Library           library,
                         ಸ್ಥಿರಾಂಕ FT_Open_ವಾದಗಳು*  ವಾದಗಳು,
                         FT_Long              face_ಸೂಚ್ಯಂಕ,
                         FT_Face             *aface,
                         FT_Bool              test_mac_fonts )
  {
    FT_Error     error;
    FT_Driver    driver = ಖಾಲಿ;
    FT_ಸ್ಮರಣೆ    ಸ್ಮರಣೆ = ಖಾಲಿ;
    FT_ಹರಿವು    ಹರಿವು = ಖಾಲಿ;
    FT_Face      face   = ಖಾಲಿ;
    FT_List_ಗಂಟು  ಗಂಟು   = ಖಾಲಿ;
    FT_Bool      external_ಹರಿವು;
    FT_Module*   cur;
    FT_Module*   ಎಲ್ಲೆ;

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ FT_CONFIG_OPTION_MAC_FONTS
    FT_ಬಳಸದ( test_mac_fonts );
#ಆದರೆ_ಅಂತ್ಯ


    /* only use lower 31 bits together with sign bit */
    ಆದರೆ ( face_ಸೂಚ್ಯಂಕ > 0 )
      face_ಸೂಚ್ಯಂಕ &= 0x7FFFFFFFL;
    ಇಲ್ಲದಿದ್ದರೆ
    {
      face_ಸೂಚ್ಯಂಕ  = -face_ಸೂಚ್ಯಂಕ;
      face_ಸೂಚ್ಯಂಕ &= 0x7FFFFFFFL;
      face_ಸೂಚ್ಯಂಕ  = -face_ಸೂಚ್ಯಂಕ;
    }

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ FT_DEBUG_LEVEL_TRACE
    FT_TRACE3(( "FT_Open_Face: " ));
    ಆದರೆ ( face_ಸೂಚ್ಯಂಕ < 0 )
      FT_TRACE3(( "Requesting number of faces and named instances\n"));
    ಇಲ್ಲದಿದ್ದರೆ
    {
      FT_TRACE3(( "Requesting face %ld", face_ಸೂಚ್ಯಂಕ & 0xFFFFL ));
      ಆದರೆ ( face_ಸೂಚ್ಯಂಕ & 0x7FFF0000L )
        FT_TRACE3(( ", named instance %ld", face_ಸೂಚ್ಯಂಕ >> 16 ));
      FT_TRACE3(( "\n" ));
    }
#ಆದರೆ_ಅಂತ್ಯ

    /* test for valid `library' delayed to `FT_Stream_New' */

    ಆದರೆ ( !ವಾದಗಳು )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

    external_ಹರಿವು = FT_BOOL( ( ವಾದಗಳು->flags & FT_OPEN_ಹರಿವು ) &&
                               ವಾದಗಳು->ಹರಿವು                     );

    /* create input stream */
    error = FT_ಹರಿವು_New( library, ವಾದಗಳು, &ಹರಿವು );
    ಆದರೆ ( error )
      ಹೋಗು Fail3;

    /* Do this error check after `FT_Stream_New` to ensure that the */
    /* 'close' callback is called.                                  */
    ಆದರೆ ( !aface && face_ಸೂಚ್ಯಂಕ >= 0 )
    {
      error = FT_THROW( Invalid_ವಾದ );
      ಹೋಗು Fail3;
    }

    ಸ್ಮರಣೆ = library->ಸ್ಮರಣೆ;

    /* If the font driver is specified in the `args' structure, use */
    /* it.  Otherwise, we scan the list of registered drivers.      */
    ಆದರೆ ( ( ವಾದಗಳು->flags & FT_OPEN_DRIVER ) && ವಾದಗಳು->driver )
    {
      driver = FT_DRIVER( ವಾದಗಳು->driver );

      /* not all modules are drivers, so check... */
      ಆದರೆ ( FT_MODULE_IS_DRIVER( driver ) )
      {
        FT_Int         ಅಂಕೆ_params = 0;
        FT_Parameter*  params     = ಖಾಲಿ;


        ಆದರೆ ( ವಾದಗಳು->flags & FT_OPEN_PARAMS )
        {
          ಅಂಕೆ_params = ವಾದಗಳು->ಅಂಕೆ_params;
          params     = ವಾದಗಳು->params;
        }

        error = open_face( driver, &ಹರಿವು, &external_ಹರಿವು, face_ಸೂಚ್ಯಂಕ,
                           ಅಂಕೆ_params, params, &face );
        ಆದರೆ ( !error )
          ಹೋಗು Success;
      }
      ಇಲ್ಲದಿದ್ದರೆ
        error = FT_THROW( Invalid_Handle );

      FT_ಹರಿವು_Free( ಹರಿವು, external_ಹರಿವು );
      ಹೋಗು Fail;
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
      error = FT_ERR( Missing_Module );

      /* check each font driver for an appropriate format */
      cur   = library->modules;
      ಎಲ್ಲೆ = cur + library->ಅಂಕೆ_modules;

      ಗಾಗಿ ( ; cur < ಎಲ್ಲೆ; cur++ )
      {
        /* not all modules are font drivers, so check... */
        ಆದರೆ ( FT_MODULE_IS_DRIVER( cur[0] ) )
        {
          FT_Int         ಅಂಕೆ_params = 0;
          FT_Parameter*  params     = ಖಾಲಿ;


          driver = FT_DRIVER( cur[0] );

          ಆದರೆ ( ವಾದಗಳು->flags & FT_OPEN_PARAMS )
          {
            ಅಂಕೆ_params = ವಾದಗಳು->ಅಂಕೆ_params;
            params     = ವಾದಗಳು->params;
          }

          error = open_face( driver, &ಹರಿವು, &external_ಹರಿವು, face_ಸೂಚ್ಯಂಕ,
                             ಅಂಕೆ_params, params, &face );
          ಆದರೆ ( !error )
            ಹೋಗು Success;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ FT_CONFIG_OPTION_MAC_FONTS
          ಆದರೆ ( test_mac_fonts                                           &&
               ft_strcmp( cur[0]->clazz->module_name, "truetype" ) == 0 &&
               FT_ERR_EQ( error, ಕೋಷ್ಟಕ_Missing )                        )
          {
            /* TrueType but essential tables are missing */
            error = FT_ಹರಿವು_Seek( ಹರಿವು, 0 );
            ಆದರೆ ( error )
              ನಿಲ್ಲಿಸು;

            error = open_face_PS_from_sfnt_ಹರಿವು( library,
                                                   ಹರಿವು,
                                                   face_ಸೂಚ್ಯಂಕ,
                                                   ಅಂಕೆ_params,
                                                   params,
                                                   aface );
            ಆದರೆ ( !error )
            {
              FT_ಹರಿವು_Free( ಹರಿವು, external_ಹರಿವು );
              ಹಿಂತಿರುಗಿ error;
            }
          }
#ಆದರೆ_ಅಂತ್ಯ

          ಆದರೆ ( FT_ERR_NEQ( error, Unknown_File_Format ) )
            ಹೋಗು Fail3;
        }
      }

    Fail3:
      /* If we are on the mac, and we get an                          */
      /* FT_Err_Invalid_Stream_Operation it may be because we have an */
      /* empty data fork, so we need to check the resource fork.      */
      ಆದರೆ ( FT_ERR_NEQ( error, Cannot_Open_ಹರಿವು )       &&
           FT_ERR_NEQ( error, Unknown_File_Format )      &&
           FT_ERR_NEQ( error, Invalid_ಹರಿವು_Operation ) )
        ಹೋಗು Fail2;

#ಆದರೆ !defined( FT_MACINTOSH ) && defined( FT_CONFIG_OPTION_MAC_FONTS )
      ಆದರೆ ( test_mac_fonts )
      {
        error = ತುಂಬು_mac_face( library, ಹರಿವು, face_ಸೂಚ್ಯಂಕ, aface, ವಾದಗಳು );
        ಆದರೆ ( !error )
        {
          /* We don't want to go to Success here.  We've already done   */
          /* that.  On the other hand, if we succeeded we still need to */
          /* close this stream (we opened a different stream which      */
          /* extracted the interesting information out of this stream   */
          /* here.  That stream will still be open and the face will    */
          /* point to it).                                              */
          FT_ಹರಿವು_Free( ಹರಿವು, external_ಹರಿವು );
          ಹಿಂತಿರುಗಿ error;
        }
      }

      ಆದರೆ ( FT_ERR_NEQ( error, Unknown_File_Format ) )
        ಹೋಗು Fail2;
#ಆದರೆ_ಅಂತ್ಯ  /* !FT_MACINTOSH && FT_CONFIG_OPTION_MAC_FONTS */

      /* no driver is able to handle this format */
      error = FT_THROW( Unknown_File_Format );

  Fail2:
      FT_ಹರಿವು_Free( ಹರಿವು, external_ಹರಿವು );
      ಹೋಗು Fail;
    }

  Success:
    FT_TRACE4(( "FT_Open_Face: New face object, adding to list\n" ));

    /* add the face object to its driver's list */
    ಆದರೆ ( FT_QNEW( ಗಂಟು ) )
      ಹೋಗು Fail;

    ಗಂಟು->ದತ್ತಾಂಶ = face;
    /* don't assume driver is the same as face->driver, so use */
    /* face->driver instead.                                   */
    FT_List_ಕೂಡಿಸು( &face->driver->faces_list, ಗಂಟು );

    /* now allocate a glyph slot object for the face */
    FT_TRACE4(( "FT_Open_Face: Creating glyph slot\n" ));

    ಆದರೆ ( face_ಸೂಚ್ಯಂಕ >= 0 )
    {
      error = FT_New_Glyph_ಸ್ಥಾನ( face, ಖಾಲಿ );
      ಆದರೆ ( error )
        ಹೋಗು Fail;

      /* finally, allocate a size object for the face */
      {
        FT_ಗಾತ್ರ  ಗಾತ್ರ_2;


        FT_TRACE4(( "FT_Open_Face: Creating size object\n" ));

        error = FT_New_ಗಾತ್ರ( face, &ಗಾತ್ರ_2 );
        ಆದರೆ ( error )
          ಹೋಗು Fail;

        face->ಗಾತ್ರ_2 = ಗಾತ್ರ_2;
      }
    }

    /* some checks */

    ಆದರೆ ( FT_IS_SCALABLE( face ) )
    {
      ಆದರೆ ( face->ಎತ್ತರ < 0 )
        face->ಎತ್ತರ = (FT_Short)-face->ಎತ್ತರ;

      ಆದರೆ ( !FT_HAS_VERTICAL( face ) )
        face->max_advance_ಎತ್ತರ = (FT_Short)face->ಎತ್ತರ;
    }

    ಆದರೆ ( FT_HAS_FIXED_SIZES( face ) )
    {
      FT_Int  i;


      ಗಾಗಿ ( i = 0; i < face->ಅಂಕೆ_fixed_sizes; i++ )
      {
        FT_Bitmap_ಗಾತ್ರ*  bsize = face->ಲಭ್ಯ_sizes + i;


        ಆದರೆ ( bsize->ಎತ್ತರ < 0 )
          bsize->ಎತ್ತರ = -bsize->ಎತ್ತರ;
        ಆದರೆ ( bsize->x_ppem < 0 )
          bsize->x_ppem = -bsize->x_ppem;
        ಆದರೆ ( bsize->y_ppem < 0 )
          bsize->y_ppem = -bsize->y_ppem;

        /* check whether negation actually has worked */
        ಆದರೆ ( bsize->ಎತ್ತರ < 0 || bsize->x_ppem < 0 || bsize->y_ppem < 0 )
        {
          FT_TRACE0(( "FT_Open_Face:"
                      " Invalid bitmap dimensions for strike %d,"
                      " now disabled\n", i ));
          bsize->ಅಗಲ  = 0;
          bsize->ಎತ್ತರ = 0;
          bsize->ಗಾತ್ರ_2   = 0;
          bsize->x_ppem = 0;
          bsize->y_ppem = 0;
        }
      }
    }

    /* initialize internal face data */
    {
      FT_Face_ಆಂತರಿಕ  ಆಂತರಿಕ = face->ಆಂತರಿಕ;


      ಆಂತರಿಕ->transform_ಮಾತೃಕೆ.xx = 0x10000L;
      ಆಂತರಿಕ->transform_ಮಾತೃಕೆ.xy = 0;
      ಆಂತರಿಕ->transform_ಮಾತೃಕೆ.yx = 0;
      ಆಂತರಿಕ->transform_ಮಾತೃಕೆ.yy = 0x10000L;

      ಆಂತರಿಕ->transform_delta.x = 0;
      ಆಂತರಿಕ->transform_delta.y = 0;

      ಆಂತರಿಕ->refcount = 1;

      ಆಂತರಿಕ->no_stem_darkening = -1;
    }

    ಆದರೆ ( aface )
      *aface = face;
    ಇಲ್ಲದಿದ್ದರೆ
      FT_ಮುಗಿದ_Face( face );

    ಹೋಗು Exit;

  Fail:
    ಆದರೆ ( ಗಂಟು )
      FT_ಮುಗಿದ_Face( face );    /* face must be in the driver's list */
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ( face )
      destroy_face( ಸ್ಮರಣೆ, face, driver );

  Exit:
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ FT_DEBUG_LEVEL_TRACE
    ಆದರೆ ( !error && face_ಸೂಚ್ಯಂಕ < 0 )
    {
      FT_TRACE3(( "FT_Open_Face: The font has %ld face%s\n",
                  face->ಅಂಕೆ_faces,
                  face->ಅಂಕೆ_faces == 1 ? "" : "s" ));
      FT_TRACE3(( "              and %ld named instance%s for face %ld\n",
                  face->style_flags >> 16,
                  ( face->style_flags >> 16 ) == 1 ? "" : "s",
                  -face_ಸೂಚ್ಯಂಕ - 1 ));
    }
#ಆದರೆ_ಅಂತ್ಯ

    FT_TRACE4(( "FT_Open_Face: Return 0x%x\n", error ));

    ಹಿಂತಿರುಗಿ error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Attach_File( FT_Face      face,
                  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*  filepathname )
  {
    FT_Open_ವಾದಗಳು  open;


    /* test for valid `face' delayed to `FT_Attach_Stream' */

    ಆದರೆ ( !filepathname )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

    open.ಹರಿವು   = ಖಾಲಿ;
    open.flags    = FT_OPEN_PATHNAME;
    open.pathname = (ಅಕ್ಷರ*)filepathname;

    ಹಿಂತಿರುಗಿ FT_Attach_ಹರಿವು( face, &open );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Attach_ಹರಿವು( FT_Face              face,
                    ಸ್ಥಿರಾಂಕ FT_Open_ವಾದಗಳು*  parameters )
  {
    FT_ಹರಿವು  ಹರಿವು;
    FT_Error   error;
    FT_Driver  driver;

    FT_Driver_Class  clazz;


    /* test for valid `parameters' delayed to `FT_Stream_New' */

    ಆದರೆ ( !face )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_Face_Handle );

    driver = face->driver;
    ಆದರೆ ( !driver )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_Driver_Handle );

    error = FT_ಹರಿವು_New( driver->ಬೇರು.library, parameters, &ಹರಿವು );
    ಆದರೆ ( error )
      ಹೋಗು Exit;

    /* we implement FT_Attach_Stream in each driver through the */
    /* `attach_file' interface                                  */

    error = FT_ERR( Unimplemented_Feature );
    clazz = driver->clazz;
    ಆದರೆ ( clazz->attach_file )
      error = clazz->attach_file( face, ಹರಿವು );

    /* close the attached stream */
    FT_ಹರಿವು_Free( ಹರಿವು,
                    FT_BOOL( parameters->ಹರಿವು                     &&
                             ( parameters->flags & FT_OPEN_ಹರಿವು ) ) );

  Exit:
    ಹಿಂತಿರುಗಿ error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Reference_Face( FT_Face  face )
  {
    ಆದರೆ ( !face )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_Face_Handle );

    face->ಆಂತರಿಕ->refcount++;

    ಹಿಂತಿರುಗಿ FT_Err_Ok;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_ಮುಗಿದ_Face( FT_Face  face )
  {
    FT_Error     error;
    FT_Driver    driver;
    FT_ಸ್ಮರಣೆ    ಸ್ಮರಣೆ;
    FT_List_ಗಂಟು  ಗಂಟು;


    error = FT_ERR( Invalid_Face_Handle );
    ಆದರೆ ( face && face->driver )
    {
      face->ಆಂತರಿಕ->refcount--;
      ಆದರೆ ( face->ಆಂತರಿಕ->refcount > 0 )
        error = FT_Err_Ok;
      ಇಲ್ಲದಿದ್ದರೆ
      {
        driver = face->driver;
        ಸ್ಮರಣೆ = driver->ಬೇರು.ಸ್ಮರಣೆ;

        /* find face in driver's list */
        ಗಂಟು = FT_List_ಕಂಡುಹಿಡಿ( &driver->faces_list, face );
        ಆದರೆ ( ಗಂಟು )
        {
          /* remove face object from the driver's list */
          FT_List_Remove( &driver->faces_list, ಗಂಟು );
          FT_FREE( ಗಂಟು );

          /* now destroy the object proper */
          destroy_face( ಸ್ಮರಣೆ, face, driver );
          error = FT_Err_Ok;
        }
      }
    }

    ಹಿಂತಿರುಗಿ error;
  }


  /* documentation is in ftobjs.h */

  FT_EXPORT_DEF( FT_Error )
  FT_New_ಗಾತ್ರ( FT_Face   face,
               FT_ಗಾತ್ರ  *asize )
  {
    FT_Error         error;
    FT_ಸ್ಮರಣೆ        ಸ್ಮರಣೆ;
    FT_Driver        driver;
    FT_Driver_Class  clazz;

    FT_ಗಾತ್ರ          ಗಾತ್ರ_2 = ಖಾಲಿ;
    FT_List_ಗಂಟು      ಗಂಟು = ಖಾಲಿ;

    FT_ಗಾತ್ರ_ಆಂತರಿಕ  ಆಂತರಿಕ = ಖಾಲಿ;


    ಆದರೆ ( !face )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_Face_Handle );

    ಆದರೆ ( !asize )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

    ಆದರೆ ( !face->driver )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_Driver_Handle );

    *asize = ಖಾಲಿ;

    driver = face->driver;
    clazz  = driver->clazz;
    ಸ್ಮರಣೆ = face->ಸ್ಮರಣೆ;

    /* Allocate new size object and perform basic initialisation */
    ಆದರೆ ( FT_ALLOC( ಗಾತ್ರ_2, clazz->ಗಾತ್ರ_object_ಗಾತ್ರ ) || FT_QNEW( ಗಂಟು ) )
      ಹೋಗು Exit;

    ಗಾತ್ರ_2->face = face;

    ಆದರೆ ( FT_NEW( ಆಂತರಿಕ ) )
      ಹೋಗು Exit;

    ಗಾತ್ರ_2->ಆಂತರಿಕ = ಆಂತರಿಕ;

    ಆದರೆ ( clazz->init_ಗಾತ್ರ )
      error = clazz->init_ಗಾತ್ರ( ಗಾತ್ರ_2 );

    /* in case of success, add to the face's list */
    ಆದರೆ ( !error )
    {
      *asize     = ಗಾತ್ರ_2;
      ಗಂಟು->ದತ್ತಾಂಶ = ಗಾತ್ರ_2;
      FT_List_ಕೂಡಿಸು( &face->sizes_list, ಗಂಟು );
    }

  Exit:
    ಆದರೆ ( error )
    {
      FT_FREE( ಗಂಟು );
      ಆದರೆ ( ಗಾತ್ರ_2 )
        FT_FREE( ಗಾತ್ರ_2->ಆಂತರಿಕ );
      FT_FREE( ಗಾತ್ರ_2 );
    }

    ಹಿಂತಿರುಗಿ error;
  }


  /* documentation is in ftobjs.h */

  FT_EXPORT_DEF( FT_Error )
  FT_ಮುಗಿದ_ಗಾತ್ರ( FT_ಗಾತ್ರ  ಗಾತ್ರ_2 )
  {
    FT_Error     error;
    FT_Driver    driver;
    FT_ಸ್ಮರಣೆ    ಸ್ಮರಣೆ;
    FT_Face      face;
    FT_List_ಗಂಟು  ಗಂಟು;


    ಆದರೆ ( !ಗಾತ್ರ_2 )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ಗಾತ್ರ_Handle );

    face = ಗಾತ್ರ_2->face;
    ಆದರೆ ( !face )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_Face_Handle );

    driver = face->driver;
    ಆದರೆ ( !driver )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_Driver_Handle );

    ಸ್ಮರಣೆ = driver->ಬೇರು.ಸ್ಮರಣೆ;

    error = FT_Err_Ok;
    ಗಂಟು  = FT_List_ಕಂಡುಹಿಡಿ( &face->sizes_list, ಗಾತ್ರ_2 );
    ಆದರೆ ( ಗಂಟು )
    {
      FT_List_Remove( &face->sizes_list, ಗಂಟು );
      FT_FREE( ಗಂಟು );

      ಆದರೆ ( face->ಗಾತ್ರ_2 == ಗಾತ್ರ_2 )
      {
        face->ಗಾತ್ರ_2 = ಖಾಲಿ;
        ಆದರೆ ( face->sizes_list.ತಲೆ )
          face->ಗಾತ್ರ_2 = (FT_ಗಾತ್ರ)(face->sizes_list.ತಲೆ->ದತ್ತಾಂಶ);
      }

      destroy_ಗಾತ್ರ( ಸ್ಮರಣೆ, ಗಾತ್ರ_2, driver );
    }
    ಇಲ್ಲದಿದ್ದರೆ
      error = FT_THROW( Invalid_ಗಾತ್ರ_Handle );

    ಹಿಂತಿರುಗಿ error;
  }


  /* documentation is in ftobjs.h */

  FT_ತಳ_DEF( FT_Error )
  FT_ಹೊಂದಿಕೆ_ಗಾತ್ರ( FT_Face          face,
                 FT_ಗಾತ್ರ_Request  req,
                 FT_Bool          ignore_ಅಗಲ,
                 FT_ULong*        ಗಾತ್ರ_ಸೂಚ್ಯಂಕ )
  {
    FT_Int   i;
    FT_Long  w, h;


    ಆದರೆ ( !FT_HAS_FIXED_SIZES( face ) )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_Face_Handle );

    /* FT_Bitmap_Size doesn't provide enough info... */
    ಆದರೆ ( req->type != FT_ಗಾತ್ರ_REQUEST_TYPE_NOMINAL )
      ಹಿಂತಿರುಗಿ FT_THROW( Unimplemented_Feature );

    w = FT_REQUEST_ಅಗಲ ( req );
    h = FT_REQUEST_ಎತ್ತರ( req );

    ಆದರೆ ( req->ಅಗಲ && !req->ಎತ್ತರ )
      h = w;
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ( !req->ಅಗಲ && req->ಎತ್ತರ )
      w = h;

    w = FT_PIX_ಸುತ್ತು( w );
    h = FT_PIX_ಸುತ್ತು( h );

    ಆದರೆ ( !w || !h )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_Pixel_ಗಾತ್ರ );

    ಗಾಗಿ ( i = 0; i < face->ಅಂಕೆ_fixed_sizes; i++ )
    {
      FT_Bitmap_ಗಾತ್ರ*  bsize = face->ಲಭ್ಯ_sizes + i;


      ಆದರೆ ( h != FT_PIX_ಸುತ್ತು( bsize->y_ppem ) )
        ಮುಂದುವರಿ;

      ಆದರೆ ( w == FT_PIX_ಸುತ್ತು( bsize->x_ppem ) || ignore_ಅಗಲ )
      {
        FT_TRACE3(( "FT_Match_Size: bitmap strike %d matches\n", i ));

        ಆದರೆ ( ಗಾತ್ರ_ಸೂಚ್ಯಂಕ )
          *ಗಾತ್ರ_ಸೂಚ್ಯಂಕ = (FT_ULong)i;

        ಹಿಂತಿರುಗಿ FT_Err_Ok;
      }
    }

    FT_TRACE3(( "FT_Match_Size: no matching bitmap strike\n" ));

    ಹಿಂತಿರುಗಿ FT_THROW( Invalid_Pixel_ಗಾತ್ರ );
  }


  /* documentation is in ftobjs.h */

  FT_ತಳ_DEF( ಶೂನ್ಯ )
  ft_synthesize_vertical_metrics( FT_Glyph_Metrics*  metrics,
                                  FT_Pos             advance )
  {
    FT_Pos  ಎತ್ತರ = metrics->ಎತ್ತರ;


    /* compensate for glyph with bbox above/below the baseline */
    ಆದರೆ ( metrics->horiBearingY < 0 )
    {
      ಆದರೆ ( ಎತ್ತರ < metrics->horiBearingY )
        ಎತ್ತರ = metrics->horiBearingY;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ( metrics->horiBearingY > 0 )
      ಎತ್ತರ -= metrics->horiBearingY;

    /* the factor 1.2 is a heuristical value */
    ಆದರೆ ( !advance )
      advance = ಎತ್ತರ * 12 / 10;

    metrics->vertBearingX = metrics->horiBearingX - metrics->horiAdvance / 2;
    metrics->vertBearingY = ( advance - ಎತ್ತರ ) / 2;
    metrics->vertAdvance  = advance;
  }


  ಸ್ಥಿರ ಶೂನ್ಯ
  ft_recompute_scaled_metrics( FT_Face           face,
                               FT_ಗಾತ್ರ_Metrics*  metrics )
  {
    /* Compute root ascender, descender, test height, and max_advance */

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GRID_FIT_METRICS
    metrics->ascender    = FT_PIX_CEIL( FT_MulFix( face->ascender,
                                                   metrics->y_scale ) );

    metrics->descender   = FT_PIX_FLOOR( FT_MulFix( face->descender,
                                                    metrics->y_scale ) );

    metrics->ಎತ್ತರ      = FT_PIX_ಸುತ್ತು( FT_MulFix( face->ಎತ್ತರ,
                                                    metrics->y_scale ) );

    metrics->max_advance = FT_PIX_ಸುತ್ತು( FT_MulFix( face->max_advance_ಅಗಲ,
                                                    metrics->x_scale ) );
#ಇಲ್ಲದಿದ್ದರೆ /* !GRID_FIT_METRICS */
    metrics->ascender    = FT_MulFix( face->ascender,
                                      metrics->y_scale );

    metrics->descender   = FT_MulFix( face->descender,
                                      metrics->y_scale );

    metrics->ಎತ್ತರ      = FT_MulFix( face->ಎತ್ತರ,
                                      metrics->y_scale );

    metrics->max_advance = FT_MulFix( face->max_advance_ಅಗಲ,
                                      metrics->x_scale );
#ಆದರೆ_ಅಂತ್ಯ /* !GRID_FIT_METRICS */
  }


  FT_ತಳ_DEF( ಶೂನ್ಯ )
  FT_Select_Metrics( FT_Face   face,
                     FT_ULong  strike_ಸೂಚ್ಯಂಕ )
  {
    FT_ಗಾತ್ರ_Metrics*  metrics;
    FT_Bitmap_ಗಾತ್ರ*   bsize;


    metrics = &face->ಗಾತ್ರ_2->metrics;
    bsize   = face->ಲಭ್ಯ_sizes + strike_ಸೂಚ್ಯಂಕ;

    metrics->x_ppem = (FT_UShort)( ( bsize->x_ppem + 32 ) >> 6 );
    metrics->y_ppem = (FT_UShort)( ( bsize->y_ppem + 32 ) >> 6 );

    ಆದರೆ ( FT_IS_SCALABLE( face ) )
    {
      metrics->x_scale = FT_DivFix( bsize->x_ppem,
                                    face->units_ಪ್ರತಿ_EM );
      metrics->y_scale = FT_DivFix( bsize->y_ppem,
                                    face->units_ಪ್ರತಿ_EM );

      ft_recompute_scaled_metrics( face, metrics );
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
      metrics->x_scale     = 1L << 16;
      metrics->y_scale     = 1L << 16;
      metrics->ascender    = bsize->y_ppem;
      metrics->descender   = 0;
      metrics->ಎತ್ತರ      = bsize->ಎತ್ತರ << 6;
      metrics->max_advance = bsize->x_ppem;
    }
  }


  FT_ತಳ_DEF( FT_Error )
  FT_Request_Metrics( FT_Face          face,
                      FT_ಗಾತ್ರ_Request  req )
  {
    FT_Error  error = FT_Err_Ok;

    FT_ಗಾತ್ರ_Metrics*  metrics;


    metrics = &face->ಗಾತ್ರ_2->metrics;

    ಆದರೆ ( FT_IS_SCALABLE( face ) )
    {
      FT_Long  w = 0, h = 0, scaled_w = 0, scaled_h = 0;


      ಆಯ್ಕೆ ( req->type )
      {
      ಸ್ಥಿತಿ FT_ಗಾತ್ರ_REQUEST_TYPE_NOMINAL:
        w = h = face->units_ಪ್ರತಿ_EM;
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ FT_ಗಾತ್ರ_REQUEST_TYPE_REAL_DIM:
        w = h = face->ascender - face->descender;
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ FT_ಗಾತ್ರ_REQUEST_TYPE_BBOX:
        w = face->bbox.xMax - face->bbox.xMin;
        h = face->bbox.yMax - face->bbox.yMin;
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ FT_ಗಾತ್ರ_REQUEST_TYPE_CELL:
        w = face->max_advance_ಅಗಲ;
        h = face->ascender - face->descender;
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ FT_ಗಾತ್ರ_REQUEST_TYPE_SCALES:
        metrics->x_scale = (FT_Fixed)req->ಅಗಲ;
        metrics->y_scale = (FT_Fixed)req->ಎತ್ತರ;
        ಆದರೆ ( !metrics->x_scale )
          metrics->x_scale = metrics->y_scale;
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ( !metrics->y_scale )
          metrics->y_scale = metrics->x_scale;
        ಹೋಗು Calculate_Ppem;

      ಸ್ಥಿತಿ FT_ಗಾತ್ರ_REQUEST_TYPE_MAX:
        ನಿಲ್ಲಿಸು;
      }

      /* to be on the safe side */
      ಆದರೆ ( w < 0 )
        w = -w;

      ಆದರೆ ( h < 0 )
        h = -h;

      scaled_w = FT_REQUEST_ಅಗಲ ( req );
      scaled_h = FT_REQUEST_ಎತ್ತರ( req );

      /* determine scales */
      ಆದರೆ ( req->ಎತ್ತರ || !req->ಅಗಲ )
      {
        ಆದರೆ ( h == 0 )
        {
          FT_ERROR(( "FT_Request_Metrics: Divide by zero\n" ));
          error = FT_ERR( Divide_By_Zero );
          ಹೋಗು Exit;
        }

        metrics->y_scale = FT_DivFix( scaled_h, h );
      }

      ಆದರೆ ( req->ಅಗಲ )
      {
        ಆದರೆ ( w == 0 )
        {
          FT_ERROR(( "FT_Request_Metrics: Divide by zero\n" ));
          error = FT_ERR( Divide_By_Zero );
          ಹೋಗು Exit;
        }

        metrics->x_scale = FT_DivFix( scaled_w, w );
      }
      ಇಲ್ಲದಿದ್ದರೆ
      {
        metrics->x_scale = metrics->y_scale;
        scaled_w = FT_MulDiv( scaled_h, w, h );
      }

      ಆದರೆ ( !req->ಎತ್ತರ )
      {
        metrics->y_scale = metrics->x_scale;
        scaled_h = FT_MulDiv( scaled_w, h, w );
      }

      ಆದರೆ ( req->type == FT_ಗಾತ್ರ_REQUEST_TYPE_CELL )
      {
        ಆದರೆ ( metrics->y_scale > metrics->x_scale )
          metrics->y_scale = metrics->x_scale;
        ಇಲ್ಲದಿದ್ದರೆ
          metrics->x_scale = metrics->y_scale;
      }

  Calculate_Ppem:
      /* calculate the ppems */
      ಆದರೆ ( req->type != FT_ಗಾತ್ರ_REQUEST_TYPE_NOMINAL )
      {
        scaled_w = FT_MulFix( face->units_ಪ್ರತಿ_EM, metrics->x_scale );
        scaled_h = FT_MulFix( face->units_ಪ್ರತಿ_EM, metrics->y_scale );
      }

      scaled_w = ( scaled_w + 32 ) >> 6;
      scaled_h = ( scaled_h + 32 ) >> 6;
      ಆದರೆ ( scaled_w > (FT_Long)FT_USHORT_MAX ||
           scaled_h > (FT_Long)FT_USHORT_MAX )
      {
        FT_ERROR(( "FT_Request_Metrics: Resulting ppem size too large\n" ));
        error = FT_ERR( Invalid_Pixel_ಗಾತ್ರ );
        ಹೋಗು Exit;
      }

      metrics->x_ppem = (FT_UShort)scaled_w;
      metrics->y_ppem = (FT_UShort)scaled_h;

      ft_recompute_scaled_metrics( face, metrics );
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
      FT_ZERO( metrics );
      metrics->x_scale = 1L << 16;
      metrics->y_scale = 1L << 16;
    }

  Exit:
    ಹಿಂತಿರುಗಿ error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Select_ಗಾತ್ರ( FT_Face  face,
                  FT_Int   strike_ಸೂಚ್ಯಂಕ )
  {
    FT_Error         error = FT_Err_Ok;
    FT_Driver_Class  clazz;


    ಆದರೆ ( !face || !FT_HAS_FIXED_SIZES( face ) )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_Face_Handle );

    ಆದರೆ ( strike_ಸೂಚ್ಯಂಕ < 0 || strike_ಸೂಚ್ಯಂಕ >= face->ಅಂಕೆ_fixed_sizes )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

    clazz = face->driver->clazz;

    ಆದರೆ ( clazz->select_ಗಾತ್ರ )
    {
      error = clazz->select_ಗಾತ್ರ( face->ಗಾತ್ರ_2, (FT_ULong)strike_ಸೂಚ್ಯಂಕ );

      FT_TRACE5(( "FT_Select_Size (%s driver):\n",
                  face->driver->ಬೇರು.clazz->module_name ));
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
      FT_Select_Metrics( face, (FT_ULong)strike_ಸೂಚ್ಯಂಕ );

      FT_TRACE5(( "FT_Select_Size:\n" ));
    }

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ FT_DEBUG_LEVEL_TRACE
    {
      FT_ಗಾತ್ರ_Metrics*  metrics = &face->ಗಾತ್ರ_2->metrics;


      FT_TRACE5(( "  x scale: %ld (%f)\n",
                  metrics->x_scale, (ದ್ವಿಗುಣ)metrics->x_scale / 65536 ));
      FT_TRACE5(( "  y scale: %ld (%f)\n",
                  metrics->y_scale, (ದ್ವಿಗುಣ)metrics->y_scale / 65536 ));
      FT_TRACE5(( "  ascender: %f\n",
                  (ದ್ವಿಗುಣ)metrics->ascender / 64 ));
      FT_TRACE5(( "  descender: %f\n",
                  (ದ್ವಿಗುಣ)metrics->descender / 64 ));
      FT_TRACE5(( "  height: %f\n",
                  (ದ್ವಿಗುಣ)metrics->ಎತ್ತರ / 64 ));
      FT_TRACE5(( "  max advance: %f\n",
                  (ದ್ವಿಗುಣ)metrics->max_advance / 64 ));
      FT_TRACE5(( "  x ppem: %d\n", metrics->x_ppem ));
      FT_TRACE5(( "  y ppem: %d\n", metrics->y_ppem ));
    }
#ಆದರೆ_ಅಂತ್ಯ

    ಹಿಂತಿರುಗಿ error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Request_ಗಾತ್ರ( FT_Face          face,
                   FT_ಗಾತ್ರ_Request  req )
  {
    FT_Error         error;
    FT_Driver_Class  clazz;
    FT_ULong         strike_ಸೂಚ್ಯಂಕ;


    ಆದರೆ ( !face )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_Face_Handle );

    ಆದರೆ ( !face->ಗಾತ್ರ_2 )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ಗಾತ್ರ_Handle );

    ಆದರೆ ( !req || req->ಅಗಲ < 0 || req->ಎತ್ತರ < 0 ||
         req->type >= FT_ಗಾತ್ರ_REQUEST_TYPE_MAX )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

    /* signal the auto-hinter to recompute its size metrics */
    /* (if requested)                                       */
    face->ಗಾತ್ರ_2->ಆಂತರಿಕ->autohint_metrics.x_scale = 0;

    clazz = face->driver->clazz;

    ಆದರೆ ( clazz->request_ಗಾತ್ರ )
    {
      error = clazz->request_ಗಾತ್ರ( face->ಗಾತ್ರ_2, req );

      FT_TRACE5(( "FT_Request_Size (%s driver):\n",
                  face->driver->ಬೇರು.clazz->module_name ));
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ( !FT_IS_SCALABLE( face ) && FT_HAS_FIXED_SIZES( face ) )
    {
      /*
       * The reason that a driver doesn't have `request_size' defined is
       * either that the scaling here suffices or that the supported formats
       * are bitmap-only and size matching is not implemented.
       *
       * In the latter case, a simple size matching is done.
       */
      error = FT_ಹೊಂದಿಕೆ_ಗಾತ್ರ( face, req, 0, &strike_ಸೂಚ್ಯಂಕ );
      ಆದರೆ ( error )
        ಹೋಗು Exit;

      ಹಿಂತಿರುಗಿ FT_Select_ಗಾತ್ರ( face, (FT_Int)strike_ಸೂಚ್ಯಂಕ );
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
      error = FT_Request_Metrics( face, req );
      ಆದರೆ ( error )
        ಹೋಗು Exit;

      FT_TRACE5(( "FT_Request_Size:\n" ));
    }

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ FT_DEBUG_LEVEL_TRACE
    {
      FT_ಗಾತ್ರ_Metrics*  metrics = &face->ಗಾತ್ರ_2->metrics;


      FT_TRACE5(( "  x scale: %ld (%f)\n",
                  metrics->x_scale, (ದ್ವಿಗುಣ)metrics->x_scale / 65536 ));
      FT_TRACE5(( "  y scale: %ld (%f)\n",
                  metrics->y_scale, (ದ್ವಿಗುಣ)metrics->y_scale / 65536 ));
      FT_TRACE5(( "  ascender: %f\n",
                  (ದ್ವಿಗುಣ)metrics->ascender / 64 ));
      FT_TRACE5(( "  descender: %f\n",
                  (ದ್ವಿಗುಣ)metrics->descender / 64 ));
      FT_TRACE5(( "  height: %f\n",
                  (ದ್ವಿಗುಣ)metrics->ಎತ್ತರ / 64 ));
      FT_TRACE5(( "  max advance: %f\n",
                  (ದ್ವಿಗುಣ)metrics->max_advance / 64 ));
      FT_TRACE5(( "  x ppem: %d\n", metrics->x_ppem ));
      FT_TRACE5(( "  y ppem: %d\n", metrics->y_ppem ));
    }
#ಆದರೆ_ಅಂತ್ಯ

  Exit:
    ಹಿಂತಿರುಗಿ error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_ಗಣ_Char_ಗಾತ್ರ( FT_Face     face,
                    FT_F26Dot6  char_ಅಗಲ,
                    FT_F26Dot6  char_ಎತ್ತರ,
                    FT_UInt     horz_resolution,
                    FT_UInt     vert_resolution )
  {
    FT_ಗಾತ್ರ_Request_Rec  req;


    /* check of `face' delayed to `FT_Request_Size' */

    ಆದರೆ ( !char_ಅಗಲ )
      char_ಅಗಲ = char_ಎತ್ತರ;
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ( !char_ಎತ್ತರ )
      char_ಎತ್ತರ = char_ಅಗಲ;

    ಆದರೆ ( !horz_resolution )
      horz_resolution = vert_resolution;
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ( !vert_resolution )
      vert_resolution = horz_resolution;

    ಆದರೆ ( char_ಅಗಲ  < 1 * 64 )
      char_ಅಗಲ  = 1 * 64;
    ಆದರೆ ( char_ಎತ್ತರ < 1 * 64 )
      char_ಎತ್ತರ = 1 * 64;

    ಆದರೆ ( !horz_resolution )
      horz_resolution = vert_resolution = 72;

    req.type           = FT_ಗಾತ್ರ_REQUEST_TYPE_NOMINAL;
    req.ಅಗಲ          = char_ಅಗಲ;
    req.ಎತ್ತರ         = char_ಎತ್ತರ;
    req.horiResolution = horz_resolution;
    req.vertResolution = vert_resolution;

    ಹಿಂತಿರುಗಿ FT_Request_ಗಾತ್ರ( face, &req );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_ಗಣ_Pixel_Sizes( FT_Face  face,
                      FT_UInt  pixel_ಅಗಲ,
                      FT_UInt  pixel_ಎತ್ತರ )
  {
    FT_ಗಾತ್ರ_Request_Rec  req;


    /* check of `face' delayed to `FT_Request_Size' */

    ಆದರೆ ( pixel_ಅಗಲ == 0 )
      pixel_ಅಗಲ = pixel_ಎತ್ತರ;
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ( pixel_ಎತ್ತರ == 0 )
      pixel_ಎತ್ತರ = pixel_ಅಗಲ;

    ಆದರೆ ( pixel_ಅಗಲ  < 1 )
      pixel_ಅಗಲ  = 1;
    ಆದರೆ ( pixel_ಎತ್ತರ < 1 )
      pixel_ಎತ್ತರ = 1;

    /* use `>=' to avoid potential compiler warning on 16bit platforms */
    ಆದರೆ ( pixel_ಅಗಲ >= 0xFFFFU )
      pixel_ಅಗಲ = 0xFFFFU;
    ಆದರೆ ( pixel_ಎತ್ತರ >= 0xFFFFU )
      pixel_ಎತ್ತರ = 0xFFFFU;

    req.type           = FT_ಗಾತ್ರ_REQUEST_TYPE_NOMINAL;
    req.ಅಗಲ          = (FT_Long)( pixel_ಅಗಲ << 6 );
    req.ಎತ್ತರ         = (FT_Long)( pixel_ಎತ್ತರ << 6 );
    req.horiResolution = 0;
    req.vertResolution = 0;

    ಹಿಂತಿರುಗಿ FT_Request_ಗಾತ್ರ( face, &req );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Kerning( FT_Face     face,
                  FT_UInt     ಎಡ_glyph,
                  FT_UInt     ಬಲ_glyph,
                  FT_UInt     kern_ಬಹುಲಕ,
                  FT_ಸದಿಶ  *akerning )
  {
    FT_Error   error = FT_Err_Ok;
    FT_Driver  driver;


    ಆದರೆ ( !face )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_Face_Handle );

    ಆದರೆ ( !akerning )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

    driver = face->driver;

    akerning->x = 0;
    akerning->y = 0;

    ಆದರೆ ( driver->clazz->get_kerning )
    {
      error = driver->clazz->get_kerning( face,
                                          ಎಡ_glyph,
                                          ಬಲ_glyph,
                                          akerning );
      ಆದರೆ ( !error )
      {
        ಆದರೆ ( kern_ಬಹುಲಕ != FT_KERNING_UNSCALED )
        {
          akerning->x = FT_MulFix( akerning->x, face->ಗಾತ್ರ_2->metrics.x_scale );
          akerning->y = FT_MulFix( akerning->y, face->ಗಾತ್ರ_2->metrics.y_scale );

          ಆದರೆ ( kern_ಬಹುಲಕ != FT_KERNING_UNFITTED )
          {
            FT_Pos  orig_x = akerning->x;
            FT_Pos  orig_y = akerning->y;


            /* we scale down kerning values for small ppem values */
            /* to avoid that rounding makes them too big.         */
            /* `25' has been determined heuristically.            */
            ಆದರೆ ( face->ಗಾತ್ರ_2->metrics.x_ppem < 25 )
              akerning->x = FT_MulDiv( orig_x,
                                       face->ಗಾತ್ರ_2->metrics.x_ppem, 25 );
            ಆದರೆ ( face->ಗಾತ್ರ_2->metrics.y_ppem < 25 )
              akerning->y = FT_MulDiv( orig_y,
                                       face->ಗಾತ್ರ_2->metrics.y_ppem, 25 );

            akerning->x = FT_PIX_ಸುತ್ತು( akerning->x );
            akerning->y = FT_PIX_ಸುತ್ತು( akerning->y );

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ FT_DEBUG_LEVEL_TRACE
            {
              FT_Pos  orig_x_rounded = FT_PIX_ಸುತ್ತು( orig_x );
              FT_Pos  orig_y_rounded = FT_PIX_ಸುತ್ತು( orig_y );


              ಆದರೆ ( akerning->x != orig_x_rounded ||
                   akerning->y != orig_y_rounded )
                FT_TRACE5(( "FT_Get_Kerning: horizontal kerning"
                            " (%ld, %ld) scaled down to (%ld, %ld) pixels\n",
                            orig_x_rounded / 64, orig_y_rounded / 64,
                            akerning->x / 64, akerning->y / 64 ));
            }
#ಆದರೆ_ಅಂತ್ಯ
          }
        }
      }
    }

    ಹಿಂತಿರುಗಿ error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Track_Kerning( FT_Face    face,
                        FT_Fixed   ಬಿಂದು_ಗಾತ್ರ,
                        FT_Int     degree,
                        FT_Fixed*  akerning )
  {
    FT_Service_Kerning  service;
    FT_Error            error = FT_Err_Ok;


    ಆದರೆ ( !face )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_Face_Handle );

    ಆದರೆ ( !akerning )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

    FT_FACE_ಕಂಡುಹಿಡಿ_SERVICE( face, service, KERNING );
    ಆದರೆ ( !service )
      ಹಿಂತಿರುಗಿ FT_THROW( Unimplemented_Feature );

    error = service->get_track( face,
                                ಬಿಂದು_ಗಾತ್ರ,
                                degree,
                                akerning );

    ಹಿಂತಿರುಗಿ error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Select_Charmap( FT_Face      face,
                     FT_Encoding  encoding )
  {
    FT_CharMap*  cur;
    FT_CharMap*  ಎಲ್ಲೆ;


    ಆದರೆ ( !face )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_Face_Handle );

    /* FT_ENCODING_NONE is a valid encoding for BDF, PCF, and Windows FNT */
    ಆದರೆ ( encoding == FT_ENCODING_NONE && !face->ಅಂಕೆ_charmaps )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

    /* FT_ENCODING_UNICODE is special.  We try to find the `best' Unicode */
    /* charmap available, i.e., one with UCS-4 characters, if possible.   */
    /*                                                                    */
    /* This is done by find_unicode_charmap() above, to share code.       */
    ಆದರೆ ( encoding == FT_ENCODING_UNICODE )
      ಹಿಂತಿರುಗಿ ಕಂಡುಹಿಡಿ_unicode_charmap( face );

    cur = face->charmaps;
    ಆದರೆ ( !cur )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_CharMap_Handle );

    ಎಲ್ಲೆ = cur + face->ಅಂಕೆ_charmaps;

    ಗಾಗಿ ( ; cur < ಎಲ್ಲೆ; cur++ )
    {
      ಆದರೆ ( cur[0]->encoding == encoding )
      {
        face->charmap = cur[0];
        ಹಿಂತಿರುಗಿ FT_Err_Ok;
      }
    }

    ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_ಗಣ_Charmap( FT_Face     face,
                  FT_CharMap  charmap )
  {
    FT_CharMap*  cur;
    FT_CharMap*  ಎಲ್ಲೆ;


    ಆದರೆ ( !face )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_Face_Handle );

    cur = face->charmaps;
    ಆದರೆ ( !cur || !charmap )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_CharMap_Handle );

    ಎಲ್ಲೆ = cur + face->ಅಂಕೆ_charmaps;

    ಗಾಗಿ ( ; cur < ಎಲ್ಲೆ; cur++ )
    {
      ಆದರೆ ( cur[0] == charmap )
      {
        FT_Long  format = FT_Get_CMap_Format( charmap );


        ಆದರೆ ( format != 14 && format != 15 )
        {
          face->charmap = cur[0];
          ಹಿಂತಿರುಗಿ FT_Err_Ok;
        }
      }
    }

    ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Int )
  FT_Get_Charmap_ಸೂಚ್ಯಂಕ( FT_CharMap  charmap )
  {
    FT_Int  i;


    ಆದರೆ ( !charmap || !charmap->face )
      ಹಿಂತಿರುಗಿ -1;

    ಗಾಗಿ ( i = 0; i < charmap->face->ಅಂಕೆ_charmaps; i++ )
      ಆದರೆ ( charmap->face->charmaps[i] == charmap )
        ನಿಲ್ಲಿಸು;

    FT_ASSERT( i < charmap->face->ಅಂಕೆ_charmaps );

    ಹಿಂತಿರುಗಿ i;
  }


  ಸ್ಥಿರ ಶೂನ್ಯ
  ft_cmap_ಮುಗಿದ_ಆಂತರಿಕ( FT_CMap  cmap )
  {
    FT_CMap_Class  clazz  = cmap->clazz;
    FT_Face        face   = cmap->charmap.face;
    FT_ಸ್ಮರಣೆ      ಸ್ಮರಣೆ = FT_FACE_ಸ್ಮರಣೆ( face );


    ಆದರೆ ( clazz->ಮುಗಿದ )
      clazz->ಮುಗಿದ( cmap );

    FT_FREE( cmap );
  }


  FT_ತಳ_DEF( ಶೂನ್ಯ )
  FT_CMap_ಮುಗಿದ( FT_CMap  cmap )
  {
    ಆದರೆ ( cmap )
    {
      FT_Face    face   = cmap->charmap.face;
      FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ = FT_FACE_ಸ್ಮರಣೆ( face );
      FT_Error   error;
      FT_Int     i, j;


      ಗಾಗಿ ( i = 0; i < face->ಅಂಕೆ_charmaps; i++ )
      {
        ಆದರೆ ( (FT_CMap)face->charmaps[i] == cmap )
        {
          FT_CharMap  ಕೊನೆಯ_charmap = face->charmaps[face->ಅಂಕೆ_charmaps - 1];


          ಆದರೆ ( FT_QRENEW_ARRAY( face->charmaps,
                                face->ಅಂಕೆ_charmaps,
                                face->ಅಂಕೆ_charmaps - 1 ) )
            ಹಿಂತಿರುಗಿ;

          /* remove it from our list of charmaps */
          ಗಾಗಿ ( j = i + 1; j < face->ಅಂಕೆ_charmaps; j++ )
          {
            ಆದರೆ ( j == face->ಅಂಕೆ_charmaps - 1 )
              face->charmaps[j - 1] = ಕೊನೆಯ_charmap;
            ಇಲ್ಲದಿದ್ದರೆ
              face->charmaps[j - 1] = face->charmaps[j];
          }

          face->ಅಂಕೆ_charmaps--;

          ಆದರೆ ( (FT_CMap)face->charmap == cmap )
            face->charmap = ಖಾಲಿ;

          ft_cmap_ಮುಗಿದ_ಆಂತರಿಕ( cmap );

          ನಿಲ್ಲಿಸು;
        }
      }
    }
  }


  FT_ತಳ_DEF( FT_Error )
  FT_CMap_New( FT_CMap_Class  clazz,
               FT_ಸೂಚಕ     init_ದತ್ತಾಂಶ,
               FT_CharMap     charmap,
               FT_CMap       *acmap )
  {
    FT_Error   error;
    FT_Face    face;
    FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ;
    FT_CMap    cmap = ಖಾಲಿ;


    ಆದರೆ ( !clazz || !charmap || !charmap->face )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

    face   = charmap->face;
    ಸ್ಮರಣೆ = FT_FACE_ಸ್ಮರಣೆ( face );

    ಆದರೆ ( !FT_ALLOC( cmap, clazz->ಗಾತ್ರ_2 ) )
    {
      cmap->charmap = *charmap;
      cmap->clazz   = clazz;

      ಆದರೆ ( clazz->init )
      {
        error = clazz->init( cmap, init_ದತ್ತಾಂಶ );
        ಆದರೆ ( error )
          ಹೋಗು Fail;
      }

      /* add it to our list of charmaps */
      ಆದರೆ ( FT_QRENEW_ARRAY( face->charmaps,
                            face->ಅಂಕೆ_charmaps,
                            face->ಅಂಕೆ_charmaps + 1 ) )
        ಹೋಗು Fail;

      face->charmaps[face->ಅಂಕೆ_charmaps++] = (FT_CharMap)cmap;
    }

  Exit:
    ಆದರೆ ( acmap )
      *acmap = cmap;

    ಹಿಂತಿರುಗಿ error;

  Fail:
    ft_cmap_ಮುಗಿದ_ಆಂತರಿಕ( cmap );
    cmap = ಖಾಲಿ;
    ಹೋಗು Exit;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt )
  FT_Get_Char_ಸೂಚ್ಯಂಕ( FT_Face   face,
                     FT_ULong  charcode )
  {
    FT_UInt  ಫಲಿತಾಂಶ = 0;


    ಆದರೆ ( face && face->charmap )
    {
      FT_CMap  cmap = FT_CMAP( face->charmap );


      ಆದರೆ ( charcode > 0xFFFFFFFFUL )
      {
        FT_TRACE1(( "FT_Get_Char_Index: too large charcode" ));
        FT_TRACE1(( " 0x%lx is truncated\n", charcode ));
      }

      ಫಲಿತಾಂಶ = cmap->clazz->char_ಸೂಚ್ಯಂಕ( cmap, (FT_UInt32)charcode );
      ಆದರೆ ( ಫಲಿತಾಂಶ >= (FT_UInt)face->ಅಂಕೆ_glyphs )
        ಫಲಿತಾಂಶ = 0;
    }

    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_ULong )
  FT_Get_ಮೊದಲ_Char( FT_Face   face,
                     FT_UInt  *agindex )
  {
    FT_ULong  ಫಲಿತಾಂಶ = 0;
    FT_UInt   gindex = 0;


    /* only do something if we have a charmap, and we have glyphs at all */
    ಆದರೆ ( face && face->charmap && face->ಅಂಕೆ_glyphs )
    {
      gindex = FT_Get_Char_ಸೂಚ್ಯಂಕ( face, 0 );
      ಆದರೆ ( gindex == 0 )
        ಫಲಿತಾಂಶ = FT_Get_ಮುಂದಿನ_Char( face, 0, &gindex );
    }

    ಆದರೆ ( agindex )
      *agindex = gindex;

    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_ULong )
  FT_Get_ಮುಂದಿನ_Char( FT_Face   face,
                    FT_ULong  charcode,
                    FT_UInt  *agindex )
  {
    FT_ULong  ಫಲಿತಾಂಶ = 0;
    FT_UInt   gindex = 0;


    ಆದರೆ ( face && face->charmap && face->ಅಂಕೆ_glyphs )
    {
      FT_UInt32  ಸಂಕೇತ_2 = (FT_UInt32)charcode;
      FT_CMap    cmap = FT_CMAP( face->charmap );


      ಮಾಡು
      {
        gindex = cmap->clazz->char_ಮುಂದಿನ( cmap, &ಸಂಕೇತ_2 );

      } ಆಗ ( gindex >= (FT_UInt)face->ಅಂಕೆ_glyphs );

      ಫಲಿತಾಂಶ = ( gindex == 0 ) ? 0 : ಸಂಕೇತ_2;
    }

    ಆದರೆ ( agindex )
      *agindex = gindex;

    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Face_Properties( FT_Face        face,
                      FT_UInt        ಅಂಕೆ_properties,
                      FT_Parameter*  properties )
  {
    FT_Error  error = FT_Err_Ok;


    ಆದರೆ ( ಅಂಕೆ_properties > 0 && !properties )
    {
      error = FT_THROW( Invalid_ವಾದ );
      ಹೋಗು Exit;
    }

    ಗಾಗಿ ( ; ಅಂಕೆ_properties > 0; ಅಂಕೆ_properties-- )
    {
      ಆದರೆ ( properties->ಟ್ಯಾಗ್ == FT_PARAM_ಟ್ಯಾಗ್_STEM_DARKENING )
      {
        ಆದರೆ ( properties->ದತ್ತಾಂಶ )
        {
          ಆದರೆ ( *( (FT_Bool*)properties->ದತ್ತಾಂಶ ) == TRUE )
            face->ಆಂತರಿಕ->no_stem_darkening = FALSE;
          ಇಲ್ಲದಿದ್ದರೆ
            face->ಆಂತರಿಕ->no_stem_darkening = TRUE;
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
          /* use module default */
          face->ಆಂತರಿಕ->no_stem_darkening = -1;
        }
      }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ( properties->ಟ್ಯಾಗ್ == FT_PARAM_ಟ್ಯಾಗ್_LCD_FILTER_WEIGHTS )
      {
        error = FT_THROW( Unimplemented_Feature );
        ಹೋಗು Exit;
      }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ( properties->ಟ್ಯಾಗ್ == FT_PARAM_ಟ್ಯಾಗ್_RANDOM_ಬೀಜ )
      {
        ಆದರೆ ( properties->ದತ್ತಾಂಶ )
        {
          face->ಆಂತರಿಕ->random_ಬೀಜ = *( (FT_Int32*)properties->ದತ್ತಾಂಶ );
          ಆದರೆ ( face->ಆಂತರಿಕ->random_ಬೀಜ < 0 )
            face->ಆಂತರಿಕ->random_ಬೀಜ = 0;
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
          /* use module default */
          face->ಆಂತರಿಕ->random_ಬೀಜ = -1;
        }
      }
      ಇಲ್ಲದಿದ್ದರೆ
      {
        error = FT_THROW( Invalid_ವಾದ );
        ಹೋಗು Exit;
      }

      ಆದರೆ ( error )
        ನಿಲ್ಲಿಸು;

      properties++;
    }

  Exit:
    ಹಿಂತಿರುಗಿ error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt )
  FT_Face_Get_Char_Variant_ಸೂಚ್ಯಂಕ( FT_Face   face,
                               FT_ULong  charcode,
                               FT_ULong  variantSelector )
  {
    FT_UInt  ಫಲಿತಾಂಶ = 0;


    ಆದರೆ ( face                                           &&
         face->charmap                                  &&
         face->charmap->encoding == FT_ENCODING_UNICODE )
    {
      FT_CharMap  charmap = ಕಂಡುಹಿಡಿ_variant_selector_charmap( face );
      FT_CMap     ucmap = FT_CMAP( face->charmap );


      ಆದರೆ ( charmap )
      {
        FT_CMap  vcmap = FT_CMAP( charmap );


        ಆದರೆ ( charcode > 0xFFFFFFFFUL )
        {
          FT_TRACE1(( "FT_Face_GetCharVariantIndex:"
                      " too large charcode" ));
          FT_TRACE1(( " 0x%lx is truncated\n", charcode ));
        }
        ಆದರೆ ( variantSelector > 0xFFFFFFFFUL )
        {
          FT_TRACE1(( "FT_Face_GetCharVariantIndex:"
                      " too large variantSelector" ));
          FT_TRACE1(( " 0x%lx is truncated\n", variantSelector ));
        }

        ಫಲಿತಾಂಶ = vcmap->clazz->char_var_ಸೂಚ್ಯಂಕ( vcmap, ucmap,
                                               (FT_UInt32)charcode,
                                               (FT_UInt32)variantSelector );
      }
    }

    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Int )
  FT_Face_GetCharVariantIsDefault( FT_Face   face,
                                   FT_ULong  charcode,
                                   FT_ULong  variantSelector )
  {
    FT_Int  ಫಲಿತಾಂಶ = -1;


    ಆದರೆ ( face )
    {
      FT_CharMap  charmap = ಕಂಡುಹಿಡಿ_variant_selector_charmap( face );


      ಆದರೆ ( charmap )
      {
        FT_CMap  vcmap = FT_CMAP( charmap );


        ಆದರೆ ( charcode > 0xFFFFFFFFUL )
        {
          FT_TRACE1(( "FT_Face_GetCharVariantIsDefault:"
                      " too large charcode" ));
          FT_TRACE1(( " 0x%lx is truncated\n", charcode ));
        }
        ಆದರೆ ( variantSelector > 0xFFFFFFFFUL )
        {
          FT_TRACE1(( "FT_Face_GetCharVariantIsDefault:"
                      " too large variantSelector" ));
          FT_TRACE1(( " 0x%lx is truncated\n", variantSelector ));
        }

        ಫಲಿತಾಂಶ = vcmap->clazz->char_var_default( vcmap,
                                                 (FT_UInt32)charcode,
                                                 (FT_UInt32)variantSelector );
      }
    }

    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt32* )
  FT_Face_GetVariantSelectors( FT_Face  face )
  {
    FT_UInt32  *ಫಲಿತಾಂಶ = ಖಾಲಿ;


    ಆದರೆ ( face )
    {
      FT_CharMap  charmap = ಕಂಡುಹಿಡಿ_variant_selector_charmap( face );


      ಆದರೆ ( charmap )
      {
        FT_CMap    vcmap  = FT_CMAP( charmap );
        FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ = FT_FACE_ಸ್ಮರಣೆ( face );


        ಫಲಿತಾಂಶ = vcmap->clazz->variant_list( vcmap, ಸ್ಮರಣೆ );
      }
    }

    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt32* )
  FT_Face_Get_Variants_ದ_Char( FT_Face   face,
                             FT_ULong  charcode )
  {
    FT_UInt32  *ಫಲಿತಾಂಶ = ಖಾಲಿ;


    ಆದರೆ ( face )
    {
      FT_CharMap  charmap = ಕಂಡುಹಿಡಿ_variant_selector_charmap( face );


      ಆದರೆ ( charmap )
      {
        FT_CMap    vcmap  = FT_CMAP( charmap );
        FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ = FT_FACE_ಸ್ಮರಣೆ( face );


        ಆದರೆ ( charcode > 0xFFFFFFFFUL )
        {
          FT_TRACE1(( "FT_Face_GetVariantsOfChar: too large charcode" ));
          FT_TRACE1(( " 0x%lx is truncated\n", charcode ));
        }

        ಫಲಿತಾಂಶ = vcmap->clazz->charvariant_list( vcmap, ಸ್ಮರಣೆ,
                                                 (FT_UInt32)charcode );
      }
    }
    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt32* )
  FT_Face_Get_Chars_ದ_Variant( FT_Face   face,
                             FT_ULong  variantSelector )
  {
    FT_UInt32  *ಫಲಿತಾಂಶ = ಖಾಲಿ;


    ಆದರೆ ( face )
    {
      FT_CharMap  charmap = ಕಂಡುಹಿಡಿ_variant_selector_charmap( face );


      ಆದರೆ ( charmap )
      {
        FT_CMap    vcmap  = FT_CMAP( charmap );
        FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ = FT_FACE_ಸ್ಮರಣೆ( face );


        ಆದರೆ ( variantSelector > 0xFFFFFFFFUL )
        {
          FT_TRACE1(( "FT_Get_Char_Index: too large variantSelector" ));
          FT_TRACE1(( " 0x%lx is truncated\n", variantSelector ));
        }

        ಫಲಿತಾಂಶ = vcmap->clazz->variantchar_list( vcmap, ಸ್ಮರಣೆ,
                                                 (FT_UInt32)variantSelector );
      }
    }

    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt )
  FT_Get_Name_ಸೂಚ್ಯಂಕ( FT_Face           face,
                     ಸ್ಥಿರಾಂಕ FT_String*  glyph_name )
  {
    FT_UInt  ಫಲಿತಾಂಶ = 0;


    ಆದರೆ ( face                       &&
         FT_HAS_GLYPH_ಹೆಸರುಗಳು( face ) &&
         glyph_name                 )
    {
      FT_Service_GlyphDict  service;


      FT_FACE_LOOKUP_SERVICE( face,
                              service,
                              GLYPH_DICT );

      ಆದರೆ ( service && service->name_ಸೂಚ್ಯಂಕ )
        ಫಲಿತಾಂಶ = service->name_ಸೂಚ್ಯಂಕ( face, glyph_name );
    }

    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Glyph_Name( FT_Face     face,
                     FT_UInt     glyph_ಸೂಚ್ಯಂಕ,
                     FT_ಸೂಚಕ  ಬಫರ್,
                     FT_UInt     ಬಫರ್_max )
  {
    FT_Error              error;
    FT_Service_GlyphDict  service;


    ಆದರೆ ( !face )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_Face_Handle );

    ಆದರೆ ( !ಬಫರ್ || ಬಫರ್_max == 0 )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

    /* clean up buffer */
    ((FT_Byte*)ಬಫರ್)[0] = '\0';

    ಆದರೆ ( (FT_Long)glyph_ಸೂಚ್ಯಂಕ >= face->ಅಂಕೆ_glyphs )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_Glyph_ಸೂಚ್ಯಂಕ );

    ಆದರೆ ( !FT_HAS_GLYPH_ಹೆಸರುಗಳು( face ) )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

    FT_FACE_LOOKUP_SERVICE( face, service, GLYPH_DICT );
    ಆದರೆ ( service && service->get_name )
      error = service->get_name( face, glyph_ಸೂಚ್ಯಂಕ, ಬಫರ್, ಬಫರ್_max );
    ಇಲ್ಲದಿದ್ದರೆ
      error = FT_THROW( Invalid_ವಾದ );

    ಹಿಂತಿರುಗಿ error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* )
  FT_Get_Postscript_Name( FT_Face  face )
  {
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*  ಫಲಿತಾಂಶ = ಖಾಲಿ;


    ಆದರೆ ( !face )
      ಹೋಗು Exit;

    ಆದರೆ ( !ಫಲಿತಾಂಶ )
    {
      FT_Service_PsFontName  service;


      FT_FACE_LOOKUP_SERVICE( face,
                              service,
                              POSTSCRIPT_FONT_NAME );

      ಆದರೆ ( service && service->get_ps_font_name )
        ಫಲಿತಾಂಶ = service->get_ps_font_name( face );
    }

  Exit:
    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
  }


  /* documentation is in tttables.h */

  FT_EXPORT_DEF( ಶೂನ್ಯ* )
  FT_Get_Sfnt_ಕೋಷ್ಟಕ( FT_Face      face,
                     FT_Sfnt_ಟ್ಯಾಗ್  ಟ್ಯಾಗ್ )
  {
    ಶೂನ್ಯ*                  ಕೋಷ್ಟಕ = ಖಾಲಿ;
    FT_Service_SFNT_ಕೋಷ್ಟಕ  service;


    ಆದರೆ ( face && FT_IS_SFNT( face ) )
    {
      FT_FACE_ಕಂಡುಹಿಡಿ_SERVICE( face, service, SFNT_ಕೋಷ್ಟಕ );
      ಆದರೆ ( service )
        ಕೋಷ್ಟಕ = service->get_ಕೋಷ್ಟಕ( face, ಟ್ಯಾಗ್ );
    }

    ಹಿಂತಿರುಗಿ ಕೋಷ್ಟಕ;
  }


  /* documentation is in tttables.h */

  FT_EXPORT_DEF( FT_Error )
  FT_ತುಂಬು_Sfnt_ಕೋಷ್ಟಕ( FT_Face    face,
                      FT_ULong   ಟ್ಯಾಗ್,
                      FT_Long    ಸ್ಥಾನಾಂತರ,
                      FT_Byte*   ಬಫರ್,
                      FT_ULong*  ಉದ್ದಳತೆ )
  {
    FT_Service_SFNT_ಕೋಷ್ಟಕ  service;


    ಆದರೆ ( !face || !FT_IS_SFNT( face ) )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_Face_Handle );

    FT_FACE_ಕಂಡುಹಿಡಿ_SERVICE( face, service, SFNT_ಕೋಷ್ಟಕ );
    ಆದರೆ ( !service )
      ಹಿಂತಿರುಗಿ FT_THROW( Unimplemented_Feature );

    ಹಿಂತಿರುಗಿ service->ತುಂಬು_ಕೋಷ್ಟಕ( face, ಟ್ಯಾಗ್, ಸ್ಥಾನಾಂತರ, ಬಫರ್, ಉದ್ದಳತೆ );
  }


  /* documentation is in tttables.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Sfnt_ಕೋಷ್ಟಕ_Info( FT_Face    face,
                      FT_UInt    ಕೋಷ್ಟಕ_ಸೂಚ್ಯಂಕ,
                      FT_ULong  *ಟ್ಯಾಗ್,
                      FT_ULong  *ಉದ್ದಳತೆ )
  {
    FT_Service_SFNT_ಕೋಷ್ಟಕ  service;
    FT_ULong               ಸ್ಥಾನಾಂತರ;


    /* test for valid `length' delayed to `service->table_info' */

    ಆದರೆ ( !face || !FT_IS_SFNT( face ) )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_Face_Handle );

    FT_FACE_ಕಂಡುಹಿಡಿ_SERVICE( face, service, SFNT_ಕೋಷ್ಟಕ );
    ಆದರೆ ( !service )
      ಹಿಂತಿರುಗಿ FT_THROW( Unimplemented_Feature );

    ಹಿಂತಿರುಗಿ service->ಕೋಷ್ಟಕ_info( face, ಕೋಷ್ಟಕ_ಸೂಚ್ಯಂಕ, ಟ್ಯಾಗ್, &ಸ್ಥಾನಾಂತರ, ಉದ್ದಳತೆ );
  }


  /* documentation is in tttables.h */

  FT_EXPORT_DEF( FT_ULong )
  FT_Get_CMap_Language_ID( FT_CharMap  charmap )
  {
    FT_Service_TTCMaps  service;
    FT_Face             face;
    TT_CMapInfo         cmap_info;


    ಆದರೆ ( !charmap || !charmap->face )
      ಹಿಂತಿರುಗಿ 0;

    face = charmap->face;
    FT_FACE_ಕಂಡುಹಿಡಿ_SERVICE( face, service, TT_CMAP );
    ಆದರೆ ( !service )
      ಹಿಂತಿರುಗಿ 0;
    ಆದರೆ ( service->get_cmap_info( charmap, &cmap_info ))
      ಹಿಂತಿರುಗಿ 0;

    ಹಿಂತಿರುಗಿ cmap_info.language;
  }


  /* documentation is in tttables.h */

  FT_EXPORT_DEF( FT_Long )
  FT_Get_CMap_Format( FT_CharMap  charmap )
  {
    FT_Service_TTCMaps  service;
    FT_Face             face;
    TT_CMapInfo         cmap_info;


    ಆದರೆ ( !charmap || !charmap->face )
      ಹಿಂತಿರುಗಿ -1;

    face = charmap->face;
    FT_FACE_ಕಂಡುಹಿಡಿ_SERVICE( face, service, TT_CMAP );
    ಆದರೆ ( !service )
      ಹಿಂತಿರುಗಿ -1;
    ಆದರೆ ( service->get_cmap_info( charmap, &cmap_info ))
      ಹಿಂತಿರುಗಿ -1;

    ಹಿಂತಿರುಗಿ cmap_info.format;
  }


  /* documentation is in ftsizes.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Activate_ಗಾತ್ರ( FT_ಗಾತ್ರ  ಗಾತ್ರ_2 )
  {
    FT_Face  face;


    ಆದರೆ ( !ಗಾತ್ರ_2 )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ಗಾತ್ರ_Handle );

    face = ಗಾತ್ರ_2->face;
    ಆದರೆ ( !face || !face->driver )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_Face_Handle );

    /* we don't need anything more complex than that; all size objects */
    /* are already listed by the face                                  */
    face->ಗಾತ್ರ_2 = ಗಾತ್ರ_2;

    ಹಿಂತಿರುಗಿ FT_Err_Ok;
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
  FT_ತಳ_DEF( FT_Renderer )
  FT_Lookup_Renderer( FT_Library       library,
                      FT_Glyph_Format  format,
                      FT_List_ಗಂಟು*     ಗಂಟು )
  {
    FT_List_ಗಂಟು  cur;
    FT_Renderer  ಫಲಿತಾಂಶ = ಖಾಲಿ;


    ಆದರೆ ( !library )
      ಹೋಗು Exit;

    cur = library->renderers.ತಲೆ;

    ಆದರೆ ( ಗಂಟು )
    {
      ಆದರೆ ( *ಗಂಟು )
        cur = (*ಗಂಟು)->ಮುಂದಿನ;
      *ಗಂಟು = ಖಾಲಿ;
    }

    ಆಗ ( cur )
    {
      FT_Renderer  renderer = FT_RENDERER( cur->ದತ್ತಾಂಶ );


      ಆದರೆ ( renderer->glyph_format == format )
      {
        ಆದರೆ ( ಗಂಟು )
          *ಗಂಟು = cur;

        ಫಲಿತಾಂಶ = renderer;
        ನಿಲ್ಲಿಸು;
      }
      cur = cur->ಮುಂದಿನ;
    }

  Exit:
    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
  }


  ಸ್ಥಿರ FT_Renderer
  ft_lookup_glyph_renderer( FT_Glyph_ಸ್ಥಾನ  ಸ್ಥಾನ )
  {
    FT_Face      face    = ಸ್ಥಾನ->face;
    FT_Library   library = FT_FACE_LIBRARY( face );
    FT_Renderer  ಫಲಿತಾಂಶ  = library->cur_renderer;


    ಆದರೆ ( !ಫಲಿತಾಂಶ || ಫಲಿತಾಂಶ->glyph_format != ಸ್ಥಾನ->format )
      ಫಲಿತಾಂಶ = FT_Lookup_Renderer( library, ಸ್ಥಾನ->format, 0 );

    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
  }


  ಸ್ಥಿರ ಶೂನ್ಯ
  ft_ಗಣ_current_renderer( FT_Library  library )
  {
    FT_Renderer  renderer;


    renderer = FT_Lookup_Renderer( library, FT_GLYPH_FORMAT_OUTLINE, 0 );
    library->cur_renderer = renderer;
  }


  ಸ್ಥಿರ FT_Error
  ft_ಕೂಡಿಸು_renderer( FT_Module  module )
  {
    FT_Library   library = module->library;
    FT_ಸ್ಮರಣೆ    ಸ್ಮರಣೆ  = library->ಸ್ಮರಣೆ;
    FT_Error     error;
    FT_List_ಗಂಟು  ಗಂಟು    = ಖಾಲಿ;


    ಆದರೆ ( FT_QNEW( ಗಂಟು ) )
      ಹೋಗು Exit;

    {
      FT_Renderer         render = FT_RENDERER( module );
      FT_Renderer_Class*  clazz  = (FT_Renderer_Class*)module->clazz;


      render->clazz        = clazz;
      render->glyph_format = clazz->glyph_format;

      /* allocate raster object if needed */
      ಆದರೆ ( clazz->raster_class && clazz->raster_class->raster_new )
      {
        error = clazz->raster_class->raster_new( ಸ್ಮರಣೆ, &render->raster );
        ಆದರೆ ( error )
          ಹೋಗು Fail;

        render->raster_render = clazz->raster_class->raster_render;
        render->render        = clazz->render_glyph;
      }

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ FT_CONFIG_OPTION_SVG
      ಆದರೆ ( clazz->glyph_format == FT_GLYPH_FORMAT_SVG )
        render->render = clazz->render_glyph;
#ಆದರೆ_ಅಂತ್ಯ

      /* add to list */
      ಗಂಟು->ದತ್ತಾಂಶ = module;
      FT_List_ಕೂಡಿಸು( &library->renderers, ಗಂಟು );

      ft_ಗಣ_current_renderer( library );
    }

  Fail:
    ಆದರೆ ( error )
      FT_FREE( ಗಂಟು );

  Exit:
    ಹಿಂತಿರುಗಿ error;
  }


  ಸ್ಥಿರ ಶೂನ್ಯ
  ft_remove_renderer( FT_Module  module )
  {
    FT_Library   library;
    FT_ಸ್ಮರಣೆ    ಸ್ಮರಣೆ;
    FT_List_ಗಂಟು  ಗಂಟು;


    library = module->library;
    ಆದರೆ ( !library )
      ಹಿಂತಿರುಗಿ;

    ಸ್ಮರಣೆ = library->ಸ್ಮರಣೆ;

    ಗಂಟು = FT_List_ಕಂಡುಹಿಡಿ( &library->renderers, module );
    ಆದರೆ ( ಗಂಟು )
    {
      FT_Renderer  render = FT_RENDERER( module );


      /* release raster object, if any */
      ಆದರೆ ( render->raster )
        render->clazz->raster_class->raster_ಮುಗಿದ( render->raster );

      /* remove from list */
      FT_List_Remove( &library->renderers, ಗಂಟು );
      FT_FREE( ಗಂಟು );

      ft_ಗಣ_current_renderer( library );
    }
  }


  /* documentation is in ftrender.h */

  FT_EXPORT_DEF( FT_Renderer )
  FT_Get_Renderer( FT_Library       library,
                   FT_Glyph_Format  format )
  {
    /* test for valid `library' delayed to `FT_Lookup_Renderer' */

    ಹಿಂತಿರುಗಿ FT_Lookup_Renderer( library, format, 0 );
  }


  /* documentation is in ftrender.h */

  FT_EXPORT_DEF( FT_Error )
  FT_ಗಣ_Renderer( FT_Library     library,
                   FT_Renderer    renderer,
                   FT_UInt        ಅಂಕೆ_params,
                   FT_Parameter*  parameters )
  {
    FT_List_ಗಂಟು  ಗಂಟು;
    FT_Error     error = FT_Err_Ok;

    FT_Renderer_ಗಣ_ಬಹುಲಕ_Func  ಗಣ_ಬಹುಲಕ;


    ಆದರೆ ( !library )
    {
      error = FT_THROW( Invalid_Library_Handle );
      ಹೋಗು Exit;
    }

    ಆದರೆ ( !renderer )
    {
      error = FT_THROW( Invalid_ವಾದ );
      ಹೋಗು Exit;
    }

    ಆದರೆ ( ಅಂಕೆ_params > 0 && !parameters )
    {
      error = FT_THROW( Invalid_ವಾದ );
      ಹೋಗು Exit;
    }

    ಗಂಟು = FT_List_ಕಂಡುಹಿಡಿ( &library->renderers, renderer );
    ಆದರೆ ( !ಗಂಟು )
    {
      error = FT_THROW( Invalid_ವಾದ );
      ಹೋಗು Exit;
    }

    FT_List_ಮೇಲೆ( &library->renderers, ಗಂಟು );

    ಆದರೆ ( renderer->glyph_format == FT_GLYPH_FORMAT_OUTLINE )
      library->cur_renderer = renderer;

    ಗಣ_ಬಹುಲಕ = renderer->clazz->ಗಣ_ಬಹುಲಕ;

    ಗಾಗಿ ( ; ಅಂಕೆ_params > 0; ಅಂಕೆ_params-- )
    {
      error = ಗಣ_ಬಹುಲಕ( renderer, parameters->ಟ್ಯಾಗ್, parameters->ದತ್ತಾಂಶ );
      ಆದರೆ ( error )
        ನಿಲ್ಲಿಸು;
      parameters++;
    }

  Exit:
    ಹಿಂತಿರುಗಿ error;
  }


  FT_ತಳ_DEF( FT_Error )
  FT_Render_Glyph_ಆಂತರಿಕ( FT_Library      library,
                            FT_Glyph_ಸ್ಥಾನ    ಸ್ಥಾನ,
                            FT_Render_ಬಹುಲಕ  render_ಬಹುಲಕ )
  {
    FT_Error     error = FT_Err_Ok;
    FT_Face      face  = ಸ್ಥಾನ->face;
    FT_Renderer  renderer;
    FT_List_ಗಂಟು  ಗಂಟು  = ಖಾಲಿ;


    /* try to render colored glyph layers as a special case */
    ಆದರೆ ( ಸ್ಥಾನ->ಆಂತರಿಕ->ತುಂಬು_flags & FT_ತುಂಬು_COLOR &&
         ಸ್ಥಾನ->format == FT_GLYPH_FORMAT_OUTLINE    )
    {
      FT_LayerIterator  iterator;

      FT_UInt  ತಳ_glyph = ಸ್ಥಾನ->glyph_ಸೂಚ್ಯಂಕ;

      FT_Bool  have_layers;
      FT_UInt  glyph_ಸೂಚ್ಯಂಕ;
      FT_UInt  color_ಸೂಚ್ಯಂಕ;


      iterator.p  = ಖಾಲಿ;
      have_layers = FT_Get_Color_Glyph_Layer( face,
                                              ತಳ_glyph,
                                              &glyph_ಸೂಚ್ಯಂಕ,
                                              &color_ಸೂಚ್ಯಂಕ,
                                              &iterator );
      ಆದರೆ ( have_layers )
      {
        error = FT_New_Glyph_ಸ್ಥಾನ( face, ಖಾಲಿ );
        ಆದರೆ ( !error )
        {
          TT_Face       ttface = (TT_Face)face;
          SFNT_Service  sfnt   = (SFNT_Service)ttface->sfnt;


          ಮಾಡು
          {
            FT_Int32  ತುಂಬು_flags = ಸ್ಥಾನ->ಆಂತರಿಕ->ತುಂಬು_flags;


            /* disable the `FT_LOAD_COLOR' flag to avoid recursion */
            /* right here in this function                         */
            ತುಂಬು_flags &= ~FT_ತುಂಬು_COLOR;

            /* render into the new `face->glyph' glyph slot */
            ತುಂಬು_flags |= FT_ತುಂಬು_RENDER | FT_ತುಂಬು_NO_BITMAP;

            error = FT_ತುಂಬು_Glyph( face, glyph_ಸೂಚ್ಯಂಕ, ತುಂಬು_flags );
            ಆದರೆ ( error )
              ನಿಲ್ಲಿಸು;

            /* blend new `face->glyph' into old `slot'; */
            /* at the first call, `slot' is still empty */
            error = sfnt->colr_blend( ttface,
                                      color_ಸೂಚ್ಯಂಕ,
                                      ಸ್ಥಾನ,
                                      face->glyph );
            ಆದರೆ ( error )
              ನಿಲ್ಲಿಸು;

          } ಆಗ ( FT_Get_Color_Glyph_Layer( face,
                                              ತಳ_glyph,
                                              &glyph_ಸೂಚ್ಯಂಕ,
                                              &color_ಸೂಚ್ಯಂಕ,
                                              &iterator ) );

          ಆದರೆ ( !error )
            ಸ್ಥಾನ->format = FT_GLYPH_FORMAT_BITMAP;

          /* this call also restores `slot' as the glyph slot */
          FT_ಮುಗಿದ_Glyph_ಸ್ಥಾನ( face->glyph );
        }

        ಆದರೆ ( !error )
          ಹಿಂತಿರುಗಿ error;

        /* Failed to do the colored layer.  Draw outline instead. */
        ಸ್ಥಾನ->format = FT_GLYPH_FORMAT_OUTLINE;
      }
    }

    /* small shortcut for the very common case */
    ಆದರೆ ( ಸ್ಥಾನ->format == FT_GLYPH_FORMAT_OUTLINE )
    {
      renderer = library->cur_renderer;
      ಗಂಟು     = library->renderers.ತಲೆ;
    }
    ಇಲ್ಲದಿದ್ದರೆ
      renderer = FT_Lookup_Renderer( library, ಸ್ಥಾನ->format, &ಗಂಟು );

    error = FT_ERR( Cannot_Render_Glyph );
    ಆಗ ( renderer )
    {
      error = renderer->render( renderer, ಸ್ಥಾನ, render_ಬಹುಲಕ, ಖಾಲಿ );
      ಆದರೆ ( !error                                   ||
           FT_ERR_NEQ( error, Cannot_Render_Glyph ) )
        ನಿಲ್ಲಿಸು;

      /* `FT_Err_Cannot_Render_Glyph` is returned if the render mode */
      /* is unsupported by the current renderer for this glyph image */
      /* format.                                                     */

      /* now, look for another renderer that supports the same */
      /* format.                                               */
      renderer = FT_Lookup_Renderer( library, ಸ್ಥಾನ->format, &ಗಂಟು );
    }

    /* it is not an error if we cannot render a bitmap glyph */
    ಆದರೆ ( FT_ERR_EQ( error, Cannot_Render_Glyph ) &&
         ಸ್ಥಾನ->format == FT_GLYPH_FORMAT_BITMAP  )
      error = FT_Err_Ok;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ FT_DEBUG_LEVEL_TRACE

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು  FT_COMPONENT
#ವ್ಯಾಖ್ಯಾನಿಸು FT_COMPONENT  checksum

    /*
     * Computing the MD5 checksum is expensive, unnecessarily distorting a
     * possible profiling of FreeType if compiled with tracing support.  For
     * this reason, we execute the following code only if explicitly
     * requested.
     */

    /* we use FT_TRACE3 in this block */
    ಆದರೆ ( !error                               &&
         ft_trace_ಮಟ್ಟಗಳು[trace_checksum] >= 3 &&
         ಸ್ಥಾನ->bitmap.ಬಫರ್                  )
    {
      FT_Bitmap  bitmap;
      FT_Error   err;


      FT_Bitmap_Init( &bitmap );

      /* we convert to a single bitmap format for computing the checksum */
      /* this also converts the bitmap flow to `down' (i.e., pitch > 0)  */
      err = FT_Bitmap_Convert( library, &ಸ್ಥಾನ->bitmap, &bitmap, 1 );
      ಆದರೆ ( !err )
      {
        MD5_CTX        ctx;
        ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ  md5[16];
        ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ  coverage = 0;
        ಪೂರ್ಣಾಂಕ            i, j;
        ಪೂರ್ಣಾಂಕ            ಅಡ್ಡಸಾಲುಗಳು  = (ಪೂರ್ಣಾಂಕ)bitmap.ಅಡ್ಡಸಾಲುಗಳು;
        ಪೂರ್ಣಾಂಕ            pitch = bitmap.pitch;


        FT_TRACE3(( "FT_Render_Glyph: bitmap %dx%d, %s (mode %d)\n",
                    pitch,
                    ಅಡ್ಡಸಾಲುಗಳು,
                    pixel_modes[ಸ್ಥಾನ->bitmap.pixel_ಬಹುಲಕ],
                    ಸ್ಥಾನ->bitmap.pixel_ಬಹುಲಕ ));

        ಗಾಗಿ ( i = 0; i < ಅಡ್ಡಸಾಲುಗಳು; i++ )
          ಗಾಗಿ ( j = 0; j < pitch; j++ )
            coverage += bitmap.ಬಫರ್[i * pitch + j];

        FT_TRACE3(( "  Total coverage: %lu\n", coverage ));

        MD5_Init( &ctx );
        ಆದರೆ ( bitmap.ಬಫರ್ )
          MD5_Update( &ctx, bitmap.ಬಫರ್,
                      (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ)ಅಡ್ಡಸಾಲುಗಳು * (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ)pitch );
        MD5_Final( md5, &ctx );

        FT_TRACE3(( "  MD5 checksum: " ));
        ಗಾಗಿ ( i = 0; i < 16; i++ )
          FT_TRACE3(( "%02X", md5[i] ));
        FT_TRACE3(( "\n" ));
      }

      FT_Bitmap_ಮುಗಿದ( library, &bitmap );
    }

    /*
     * Dump bitmap in Netpbm format (PBM or PGM).
     */

    /* we use FT_TRACE7 in this block */
    ಆದರೆ ( !error                               &&
         ft_trace_ಮಟ್ಟಗಳು[trace_checksum] >= 7 &&
         ಸ್ಥಾನ->bitmap.ಬಫರ್                  )
    {
      ಆದರೆ ( ಸ್ಥಾನ->bitmap.ಅಡ್ಡಸಾಲುಗಳು  < 128U &&
           ಸ್ಥಾನ->bitmap.ಅಗಲ < 128U )
      {
        ಪೂರ್ಣಾಂಕ  ಅಡ್ಡಸಾಲುಗಳು  = (ಪೂರ್ಣಾಂಕ)ಸ್ಥಾನ->bitmap.ಅಡ್ಡಸಾಲುಗಳು;
        ಪೂರ್ಣಾಂಕ  ಅಗಲ = (ಪೂರ್ಣಾಂಕ)ಸ್ಥಾನ->bitmap.ಅಗಲ;
        ಪೂರ್ಣಾಂಕ  pitch =      ಸ್ಥಾನ->bitmap.pitch;
        ಪೂರ್ಣಾಂಕ  i, j, m;

        ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ*  topleft = ಸ್ಥಾನ->bitmap.ಬಫರ್;


        ಆದರೆ ( pitch < 0 )
          topleft -= pitch * ( ಅಡ್ಡಸಾಲುಗಳು - 1 );

        FT_TRACE7(( "Netpbm image: start\n" ));
        ಆಯ್ಕೆ ( ಸ್ಥಾನ->bitmap.pixel_ಬಹುಲಕ )
        {
        ಸ್ಥಿತಿ FT_PIXEL_ಬಹುಲಕ_MONO:
          FT_TRACE7(( "P1 %d %d\n", ಅಗಲ, ಅಡ್ಡಸಾಲುಗಳು ));
          ಗಾಗಿ ( i = 0; i < ಅಡ್ಡಸಾಲುಗಳು; i++ )
          {
            ಗಾಗಿ ( j = 0; j < ಅಗಲ; )
              ಗಾಗಿ ( m = 128; m > 0 && j < ಅಗಲ; m >>= 1, j++ )
                FT_TRACE7(( " %d",
                            ( topleft[i * pitch + j / 8] & m ) != 0 ));
            FT_TRACE7(( "\n" ));
          }
          ನಿಲ್ಲಿಸು;

        ಪೂರ್ವನಿಯೋಜಿತ:
          FT_TRACE7(( "P2 %d %d 255\n", ಅಗಲ, ಅಡ್ಡಸಾಲುಗಳು ));
          ಗಾಗಿ ( i = 0; i < ಅಡ್ಡಸಾಲುಗಳು; i++ )
          {
            ಗಾಗಿ ( j = 0; j < ಅಗಲ; j += 1 )
              FT_TRACE7(( " %3u", topleft[i * pitch + j] ));
            FT_TRACE7(( "\n" ));
          }
        }
        FT_TRACE7(( "Netpbm image: end\n" ));
      }
      ಇಲ್ಲದಿದ್ದರೆ
        FT_TRACE7(( "Netpbm image: too large, omitted\n" ));
    }

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು  FT_COMPONENT
#ವ್ಯಾಖ್ಯಾನಿಸು FT_COMPONENT  objs

#ಆದರೆ_ಅಂತ್ಯ /* FT_DEBUG_LEVEL_TRACE */

    ಹಿಂತಿರುಗಿ error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Render_Glyph( FT_Glyph_ಸ್ಥಾನ    ಸ್ಥಾನ,
                   FT_Render_ಬಹುಲಕ  render_ಬಹುಲಕ )
  {
    FT_Library  library;


    ಆದರೆ ( !ಸ್ಥಾನ || !ಸ್ಥಾನ->face )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

    library = FT_FACE_LIBRARY( ಸ್ಥಾನ->face );

    ಹಿಂತಿರುಗಿ FT_Render_Glyph_ಆಂತರಿಕ( library, ಸ್ಥಾನ, render_ಬಹುಲಕ );
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
  ಸ್ಥಿರ ಶೂನ್ಯ
  Destroy_Module( FT_Module  module )
  {
    ಸ್ಥಿರಾಂಕ FT_Module_Class*  clazz   = module->clazz;
    FT_Library              library = module->library;
    FT_ಸ್ಮರಣೆ               ಸ್ಮರಣೆ  = module->ಸ್ಮರಣೆ;


    ಆದರೆ ( library && library->auto_hinter == module )
      library->auto_hinter = ಖಾಲಿ;

    /* if the module is a renderer */
    ಆದರೆ ( FT_MODULE_IS_RENDERER( module ) )
      ft_remove_renderer( module );

    /* if the module is a font driver, add some steps */
    ಆದರೆ ( FT_MODULE_IS_DRIVER( module ) )
      Destroy_Driver( FT_DRIVER( module ) );

    /* finalize the module object */
    ಆದರೆ ( clazz->module_ಮುಗಿದ )
      clazz->module_ಮುಗಿದ( module );

    /* discard it */
    FT_FREE( module );
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_ಕೂಡಿಸು_Module( FT_Library              library,
                 ಸ್ಥಿರಾಂಕ FT_Module_Class*  clazz )
  {
    FT_Error   error;
    FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ;
    FT_Module  module = ಖಾಲಿ;
    FT_UInt    nn;


#ವ್ಯಾಖ್ಯಾನಿಸು FREETYPE_VER_FIXED  ( ( (FT_Long)FREETYPE_MAJOR << 16 ) | \
                                FREETYPE_MINOR                  )

    ಆದರೆ ( !library )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_Library_Handle );

    ಆದರೆ ( !clazz )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

    /* check FreeType version */
    ಆದರೆ ( clazz->module_requires > FREETYPE_VER_FIXED )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ಆವೃತ್ತಿ );

    /* look for a module with the same name in the library's table */
    ಗಾಗಿ ( nn = 0; nn < library->ಅಂಕೆ_modules; nn++ )
    {
      module = library->modules[nn];
      ಆದರೆ ( ft_strcmp( module->clazz->module_name, clazz->module_name ) == 0 )
      {
        /* this installed module has the same name, compare their versions */
        ಆದರೆ ( clazz->module_ಆವೃತ್ತಿ <= module->clazz->module_ಆವೃತ್ತಿ )
          ಹಿಂತಿರುಗಿ FT_THROW( ಅಧೋ_Module_ಆವೃತ್ತಿ );

        /* remove the module from our list, then exit the loop to replace */
        /* it by our new version..                                        */
        FT_Remove_Module( library, module );
        ನಿಲ್ಲಿಸು;
      }
    }

    ಸ್ಮರಣೆ = library->ಸ್ಮರಣೆ;
    error  = FT_Err_Ok;

    ಆದರೆ ( library->ಅಂಕೆ_modules >= FT_MAX_MODULES )
    {
      error = FT_THROW( Too_Many_Drivers );
      ಹೋಗು Exit;
    }

    /* allocate module object */
    ಆದರೆ ( FT_ALLOC( module, clazz->module_ಗಾತ್ರ ) )
      ಹೋಗು Exit;

    /* base initialization */
    module->clazz   = clazz;
    module->library = library;
    module->ಸ್ಮರಣೆ  = ಸ್ಮರಣೆ;

    /* check whether the module is a renderer - this must be performed */
    /* before the normal module initialization                         */
    ಆದರೆ ( FT_MODULE_IS_RENDERER( module ) )
    {
      /* add to the renderers list */
      error = ft_ಕೂಡಿಸು_renderer( module );
      ಆದರೆ ( error )
        ಹೋಗು Fail;
    }

    /* is the module a auto-hinter? */
    ಆದರೆ ( FT_MODULE_IS_HINTER( module ) )
      library->auto_hinter = module;

    /* if the module is a font driver */
    ಆದರೆ ( FT_MODULE_IS_DRIVER( module ) )
    {
      FT_Driver  driver = FT_DRIVER( module );


      driver->clazz = (FT_Driver_Class)module->clazz;
    }

    ಆದರೆ ( clazz->module_init )
    {
      error = clazz->module_init( module );
      ಆದರೆ ( error )
        ಹೋಗು Fail;
    }

    /* add module to the library's table */
    library->modules[library->ಅಂಕೆ_modules++] = module;

  Exit:
    ಹಿಂತಿರುಗಿ error;

  Fail:
    ಆದರೆ ( FT_MODULE_IS_RENDERER( module ) )
    {
      FT_Renderer  renderer = FT_RENDERER( module );


      ಆದರೆ ( renderer->clazz                                          &&
           renderer->clazz->glyph_format == FT_GLYPH_FORMAT_OUTLINE &&
           renderer->raster                                         )
        renderer->clazz->raster_class->raster_ಮುಗಿದ( renderer->raster );
    }

    FT_FREE( module );
    ಹೋಗು Exit;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Module )
  FT_Get_Module( FT_Library   library,
                 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*  module_name )
  {
    FT_Module   ಫಲಿತಾಂಶ = ಖಾಲಿ;
    FT_Module*  cur;
    FT_Module*  ಎಲ್ಲೆ;


    ಆದರೆ ( !library || !module_name )
      ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;

    cur   = library->modules;
    ಎಲ್ಲೆ = cur + library->ಅಂಕೆ_modules;

    ಗಾಗಿ ( ; cur < ಎಲ್ಲೆ; cur++ )
      ಆದರೆ ( ft_strcmp( cur[0]->clazz->module_name, module_name ) == 0 )
      {
        ಫಲಿತಾಂಶ = cur[0];
        ನಿಲ್ಲಿಸು;
      }

    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
  }


  /* documentation is in ftobjs.h */

  FT_ತಳ_DEF( ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* )
  FT_Get_Module_Interface( FT_Library   library,
                           ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*  mod_name )
  {
    FT_Module  module;


    /* test for valid `library' delayed to FT_Get_Module() */

    module = FT_Get_Module( library, mod_name );

    ಹಿಂತಿರುಗಿ module ? module->clazz->module_interface : 0;
  }


  FT_ತಳ_DEF( FT_ಸೂಚಕ )
  ft_module_get_service( FT_Module    module,
                         ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*  service_id,
                         FT_Bool      global )
  {
    FT_ಸೂಚಕ  ಫಲಿತಾಂಶ = ಖಾಲಿ;


    ಆದರೆ ( module )
    {
      FT_ASSERT( module->clazz && module->clazz->get_interface );

      /* first, look for the service in the module */
      ಆದರೆ ( module->clazz->get_interface )
        ಫಲಿತಾಂಶ = module->clazz->get_interface( module, service_id );

      ಆದರೆ ( global && !ಫಲಿತಾಂಶ )
      {
        /* we didn't find it, look in all other modules then */
        FT_Library  library = module->library;
        FT_Module*  cur     = library->modules;
        FT_Module*  ಎಲ್ಲೆ   = cur + library->ಅಂಕೆ_modules;


        ಗಾಗಿ ( ; cur < ಎಲ್ಲೆ; cur++ )
        {
          ಆದರೆ ( cur[0] != module )
          {
            FT_ASSERT( cur[0]->clazz );

            ಆದರೆ ( cur[0]->clazz->get_interface )
            {
              ಫಲಿತಾಂಶ = cur[0]->clazz->get_interface( cur[0], service_id );
              ಆದರೆ ( ಫಲಿತಾಂಶ )
                ನಿಲ್ಲಿಸು;
            }
          }
        }
      }
    }

    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Remove_Module( FT_Library  library,
                    FT_Module   module )
  {
    /* try to find the module from the table, then remove it from there */

    ಆದರೆ ( !library )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_Library_Handle );

    ಆದರೆ ( module )
    {
      FT_Module*  cur   = library->modules;
      FT_Module*  ಎಲ್ಲೆ = cur + library->ಅಂಕೆ_modules;


      ಗಾಗಿ ( ; cur < ಎಲ್ಲೆ; cur++ )
      {
        ಆದರೆ ( cur[0] == module )
        {
          /* remove it from the table */
          library->ಅಂಕೆ_modules--;
          ಎಲ್ಲೆ--;
          ಆಗ ( cur < ಎಲ್ಲೆ )
          {
            cur[0] = cur[1];
            cur++;
          }
          ಎಲ್ಲೆ[0] = ಖಾಲಿ;

          /* destroy the module */
          Destroy_Module( module );

          ಹಿಂತಿರುಗಿ FT_Err_Ok;
        }
      }
    }
    ಹಿಂತಿರುಗಿ FT_THROW( Invalid_Driver_Handle );
  }


  ಸ್ಥಿರ FT_Error
  ft_property_do( FT_Library        library,
                  ಸ್ಥಿರಾಂಕ FT_String*  module_name,
                  ಸ್ಥಿರಾಂಕ FT_String*  property_name,
                  ಶೂನ್ಯ*             ಮೌಲ್ಯ,
                  FT_Bool           ಗಣ,
                  FT_Bool           ಮೌಲ್ಯ_is_string )
  {
    FT_Module*           cur;
    FT_Module*           ಎಲ್ಲೆ;
    FT_Module_Interface  interface;

    FT_Service_Properties  service;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ FT_DEBUG_LEVEL_ERROR
    ಸ್ಥಿರಾಂಕ FT_String*  ಗಣ_name  = "FT_Property_Set";
    ಸ್ಥಿರಾಂಕ FT_String*  get_name  = "FT_Property_Get";
    ಸ್ಥಿರಾಂಕ FT_String*  func_name = ಗಣ ? ಗಣ_name : get_name;
#ಆದರೆ_ಅಂತ್ಯ

    FT_Bool  missing_func;


    ಆದರೆ ( !library )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_Library_Handle );

    ಆದರೆ ( !module_name || !property_name || !ಮೌಲ್ಯ )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

    cur   = library->modules;
    ಎಲ್ಲೆ = cur + library->ಅಂಕೆ_modules;

    /* search module */
    ಗಾಗಿ ( ; cur < ಎಲ್ಲೆ; cur++ )
      ಆದರೆ ( !ft_strcmp( cur[0]->clazz->module_name, module_name ) )
        ನಿಲ್ಲಿಸು;

    ಆದರೆ ( cur == ಎಲ್ಲೆ )
    {
      FT_TRACE2(( "%s: can't find module `%s'\n",
                  func_name, module_name ));
      ಹಿಂತಿರುಗಿ FT_THROW( Missing_Module );
    }

    /* check whether we have a service interface */
    ಆದರೆ ( !cur[0]->clazz->get_interface )
    {
      FT_TRACE2(( "%s: module `%s' doesn't support properties\n",
                  func_name, module_name ));
      ಹಿಂತಿರುಗಿ FT_THROW( Unimplemented_Feature );
    }

    /* search property service */
    interface = cur[0]->clazz->get_interface( cur[0],
                                              FT_SERVICE_ID_PROPERTIES );
    ಆದರೆ ( !interface )
    {
      FT_TRACE2(( "%s: module `%s' doesn't support properties\n",
                  func_name, module_name ));
      ಹಿಂತಿರುಗಿ FT_THROW( Unimplemented_Feature );
    }

    service = (FT_Service_Properties)interface;

    ಆದರೆ ( ಗಣ )
      missing_func = FT_BOOL( !service->ಗಣ_property );
    ಇಲ್ಲದಿದ್ದರೆ
      missing_func = FT_BOOL( !service->get_property );

    ಆದರೆ ( missing_func )
    {
      FT_TRACE2(( "%s: property service of module `%s' is broken\n",
                  func_name, module_name ));
      ಹಿಂತಿರುಗಿ FT_THROW( Unimplemented_Feature );
    }

    ಹಿಂತಿರುಗಿ ಗಣ ? service->ಗಣ_property( cur[0],
                                        property_name,
                                        ಮೌಲ್ಯ,
                                        ಮೌಲ್ಯ_is_string )
               : service->get_property( cur[0],
                                        property_name,
                                        ಮೌಲ್ಯ );
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Property_ಗಣ( FT_Library        library,
                   ಸ್ಥಿರಾಂಕ FT_String*  module_name,
                   ಸ್ಥಿರಾಂಕ FT_String*  property_name,
                   ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ*       ಮೌಲ್ಯ )
  {
    ಹಿಂತಿರುಗಿ ft_property_do( library,
                           module_name,
                           property_name,
                           (ಶೂನ್ಯ*)ಮೌಲ್ಯ,
                           TRUE,
                           FALSE );
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Property_Get( FT_Library        library,
                   ಸ್ಥಿರಾಂಕ FT_String*  module_name,
                   ಸ್ಥಿರಾಂಕ FT_String*  property_name,
                   ಶೂನ್ಯ*             ಮೌಲ್ಯ )
  {
    ಹಿಂತಿರುಗಿ ft_property_do( library,
                           module_name,
                           property_name,
                           ಮೌಲ್ಯ,
                           FALSE,
                           FALSE );
  }


#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES

  /* this variant is used for handling the FREETYPE_PROPERTIES */
  /* environment variable                                      */

  FT_ತಳ_DEF( FT_Error )
  ft_property_string_ಗಣ( FT_Library        library,
                          ಸ್ಥಿರಾಂಕ FT_String*  module_name,
                          ಸ್ಥಿರಾಂಕ FT_String*  property_name,
                          FT_String*        ಮೌಲ್ಯ )
  {
    ಹಿಂತಿರುಗಿ ft_property_do( library,
                           module_name,
                           property_name,
                           (ಶೂನ್ಯ*)ಮೌಲ್ಯ,
                           TRUE,
                           TRUE );
  }

#ಆದರೆ_ಅಂತ್ಯ


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
    ಆದರೆ ( !library )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_Library_Handle );

    library->refcount++;

    ಹಿಂತಿರುಗಿ FT_Err_Ok;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_New_Library( FT_ಸ್ಮರಣೆ    ಸ್ಮರಣೆ,
                  FT_Library  *alibrary )
  {
    FT_Library  library = ಖಾಲಿ;
    FT_Error    error;


    ಆದರೆ ( !ಸ್ಮರಣೆ || !alibrary )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_ವಾದ );

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ FT_DEBUG_LOGGING
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ FT_DEBUG_LEVEL_ERROR
    /* init debugging support */
    ft_debug_init();
#ಆದರೆ_ಅಂತ್ಯ /* FT_DEBUG_LEVEL_ERROR */
#ಆದರೆ_ಅಂತ್ಯ /* !FT_DEBUG_LOGGING */

    /* first of all, allocate the library object */
    ಆದರೆ ( FT_NEW( library ) )
      ಹಿಂತಿರುಗಿ error;

    library->ಸ್ಮರಣೆ = ಸ್ಮರಣೆ;

    library->ಆವೃತ್ತಿ_major = FREETYPE_MAJOR;
    library->ಆವೃತ್ತಿ_minor = FREETYPE_MINOR;
    library->ಆವೃತ್ತಿ_patch = FREETYPE_PATCH;

    library->refcount = 1;

    /* That's ok now */
    *alibrary = library;

    ಹಿಂತಿರುಗಿ FT_Err_Ok;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( ಶೂನ್ಯ )
  FT_Library_ಆವೃತ್ತಿ( FT_Library   library,
                      FT_Int      *amajor,
                      FT_Int      *aminor,
                      FT_Int      *apatch )
  {
    FT_Int  major = 0;
    FT_Int  minor = 0;
    FT_Int  patch = 0;


    ಆದರೆ ( library )
    {
      major = library->ಆವೃತ್ತಿ_major;
      minor = library->ಆವೃತ್ತಿ_minor;
      patch = library->ಆವೃತ್ತಿ_patch;
    }

    ಆದರೆ ( amajor )
      *amajor = major;

    ಆದರೆ ( aminor )
      *aminor = minor;

    ಆದರೆ ( apatch )
      *apatch = patch;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_ಮುಗಿದ_Library( FT_Library  library )
  {
    FT_ಸ್ಮರಣೆ  ಸ್ಮರಣೆ;


    ಆದರೆ ( !library )
      ಹಿಂತಿರುಗಿ FT_THROW( Invalid_Library_Handle );

    library->refcount--;
    ಆದರೆ ( library->refcount > 0 )
      ಹೋಗು Exit;

    ಸ್ಮರಣೆ = library->ಸ್ಮರಣೆ;

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
      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*  driver_name[] = { "type42", ಖಾಲಿ };


      ಗಾಗಿ ( m = 0;
            m < ಗಾತ್ರ ( driver_name ) / ಗಾತ್ರ ( driver_name[0] );
            m++ )
      {
        ಗಾಗಿ ( n = 0; n < library->ಅಂಕೆ_modules; n++ )
        {
          FT_Module    module      = library->modules[n];
          ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*  module_name = module->clazz->module_name;
          FT_List      faces;


          ಆದರೆ ( driver_name[m]                                &&
               ft_strcmp( module_name, driver_name[m] ) != 0 )
            ಮುಂದುವರಿ;

          ಆದರೆ ( ( module->clazz->module_flags & FT_MODULE_FONT_DRIVER ) == 0 )
            ಮುಂದುವರಿ;

          FT_TRACE7(( "FT_Done_Library: close faces for %s\n", module_name ));

          faces = &FT_DRIVER( module )->faces_list;
          ಆಗ ( faces->ತಲೆ )
          {
            FT_ಮುಗಿದ_Face( FT_FACE( faces->ತಲೆ->ದತ್ತಾಂಶ ) );
            ಆದರೆ ( faces->ತಲೆ )
              FT_TRACE0(( "FT_Done_Library: failed to free some faces\n" ));
          }
        }
      }
    }

    /* Close all other modules in the library */
#ಆದರೆ 1
    /* XXX Modules are removed in the reversed order so that  */
    /* type42 module is removed before truetype module.  This */
    /* avoids double free in some occasions.  It is a hack.   */
    ಆಗ ( library->ಅಂಕೆ_modules > 0 )
      FT_Remove_Module( library,
                        library->modules[library->ಅಂಕೆ_modules - 1] );
#ಇಲ್ಲದಿದ್ದರೆ
    {
      FT_UInt  n;


      ಗಾಗಿ ( n = 0; n < library->ಅಂಕೆ_modules; n++ )
      {
        FT_Module  module = library->modules[n];


        ಆದರೆ ( module )
        {
          Destroy_Module( module );
          library->modules[n] = ಖಾಲಿ;
        }
      }
    }
#ಆದರೆ_ಅಂತ್ಯ

    FT_FREE( library );

  Exit:
    ಹಿಂತಿರುಗಿ FT_Err_Ok;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( ಶೂನ್ಯ )
  FT_ಗಣ_Debug_Hook( FT_Library         library,
                     FT_UInt            hook_ಸೂಚ್ಯಂಕ,
                     FT_DebugHook_Func  debug_hook )
  {
    ಆದರೆ ( library && debug_hook &&
         hook_ಸೂಚ್ಯಂಕ <
           ( ಗಾತ್ರ ( library->debug_hooks ) / ಗಾತ್ರ ( ಶೂನ್ಯ* ) ) )
      library->debug_hooks[hook_ಸೂಚ್ಯಂಕ] = debug_hook;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_TrueTypeEngineType )
  FT_Get_TrueType_Engine_Type( FT_Library  library )
  {
    FT_TrueTypeEngineType  ಫಲಿತಾಂಶ = FT_TRUETYPE_ENGINE_TYPE_NONE;


    ಆದರೆ ( library )
    {
      FT_Module  module = FT_Get_Module( library, "truetype" );


      ಆದರೆ ( module )
      {
        FT_Service_TrueTypeEngine  service;


        service = (FT_Service_TrueTypeEngine)
                    ft_module_get_service( module,
                                           FT_SERVICE_ID_TRUETYPE_ENGINE,
                                           0 );
        ಆದರೆ ( service )
          ಫಲಿತಾಂಶ = service->engine_type;
      }
    }

    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_SubGlyph_Info( FT_Glyph_ಸ್ಥಾನ  glyph,
                        FT_UInt       sub_ಸೂಚ್ಯಂಕ,
                        FT_Int       *p_ಸೂಚ್ಯಂಕ,
                        FT_UInt      *p_flags,
                        FT_Int       *p_arg1,
                        FT_Int       *p_arg2,
                        FT_ಮಾತೃಕೆ    *p_transform )
  {
    FT_Error  error = FT_ERR( Invalid_ವಾದ );


    ಆದರೆ ( glyph                                      &&
         glyph->subglyphs                           &&
         glyph->format == FT_GLYPH_FORMAT_ಸಂಯುಕ್ತ &&
         sub_ಸೂಚ್ಯಂಕ < glyph->ಅಂಕೆ_subglyphs           )
    {
      FT_SubGlyph  subg = glyph->subglyphs + sub_ಸೂಚ್ಯಂಕ;


      *p_ಸೂಚ್ಯಂಕ     = subg->index;
      *p_flags     = subg->flags;
      *p_arg1      = subg->arg1;
      *p_arg2      = subg->arg2;
      *p_transform = subg->transform;

      error = FT_Err_Ok;
    }

    ಹಿಂತಿರುಗಿ error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Get_Color_Glyph_Layer( FT_Face            face,
                            FT_UInt            ತಳ_glyph,
                            FT_UInt           *aglyph_ಸೂಚ್ಯಂಕ,
                            FT_UInt           *acolor_ಸೂಚ್ಯಂಕ,
                            FT_LayerIterator*  iterator )
  {
    TT_Face       ttface;
    SFNT_Service  sfnt;


    ಆದರೆ ( !face                                   ||
         !aglyph_ಸೂಚ್ಯಂಕ                           ||
         !acolor_ಸೂಚ್ಯಂಕ                           ||
         !iterator                               ||
         ತಳ_glyph >= (FT_UInt)face->ಅಂಕೆ_glyphs )
      ಹಿಂತಿರುಗಿ 0;

    ಆದರೆ ( !FT_IS_SFNT( face ) )
      ಹಿಂತಿರುಗಿ 0;

    ttface = (TT_Face)face;
    sfnt   = (SFNT_Service)ttface->sfnt;

    ಆದರೆ ( sfnt->get_colr_layer )
      ಹಿಂತಿರುಗಿ sfnt->get_colr_layer( ttface,
                                   ತಳ_glyph,
                                   aglyph_ಸೂಚ್ಯಂಕ,
                                   acolor_ಸೂಚ್ಯಂಕ,
                                   iterator );
    ಇಲ್ಲದಿದ್ದರೆ
      ಹಿಂತಿರುಗಿ 0;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Get_Color_Glyph_Paint( FT_Face                  face,
                            FT_UInt                  ತಳ_glyph,
                            FT_Color_ಬೇರು_Transform  ಬೇರು_transform,
                            FT_OpaquePaint*          paint )
  {
    TT_Face       ttface;
    SFNT_Service  sfnt;


    ಆದರೆ ( !face || !paint )
      ಹಿಂತಿರುಗಿ 0;

    ಆದರೆ ( !FT_IS_SFNT( face ) )
      ಹಿಂತಿರುಗಿ 0;

    ttface = (TT_Face)face;
    sfnt   = (SFNT_Service)ttface->sfnt;

    ಆದರೆ ( sfnt->get_colr_glyph_paint )
      ಹಿಂತಿರುಗಿ sfnt->get_colr_glyph_paint( ttface,
                                         ತಳ_glyph,
                                         ಬೇರು_transform,
                                         paint );
    ಇಲ್ಲದಿದ್ದರೆ
      ಹಿಂತಿರುಗಿ 0;
  }


  /* documentation is in ftcolor.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Get_Color_Glyph_ClipBox( FT_Face      face,
                              FT_UInt      ತಳ_glyph,
                              FT_ClipBox*  clip_box )
  {
    TT_Face       ttface;
    SFNT_Service  sfnt;


    ಆದರೆ ( !face || !clip_box )
      ಹಿಂತಿರುಗಿ 0;

    ಆದರೆ ( !FT_IS_SFNT( face ) )
      ಹಿಂತಿರುಗಿ 0;

    ttface = (TT_Face)face;
    sfnt   = (SFNT_Service)ttface->sfnt;

    ಆದರೆ ( sfnt->get_color_glyph_clipbox )
      ಹಿಂತಿರುಗಿ sfnt->get_color_glyph_clipbox( ttface,
                                            ತಳ_glyph,
                                            clip_box );
    ಇಲ್ಲದಿದ್ದರೆ
      ಹಿಂತಿರುಗಿ 0;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Get_Paint_Layers( FT_Face            face,
                       FT_LayerIterator*  layer_iterator,
                       FT_OpaquePaint*    paint )
  {
    TT_Face       ttface;
    SFNT_Service  sfnt;


    ಆದರೆ ( !face || !paint || !layer_iterator )
      ಹಿಂತಿರುಗಿ 0;

    ಆದರೆ ( !FT_IS_SFNT( face ) )
      ಹಿಂತಿರುಗಿ 0;

    ttface = (TT_Face)face;
    sfnt   = (SFNT_Service)ttface->sfnt;

    ಆದರೆ ( sfnt->get_paint_layers )
      ಹಿಂತಿರುಗಿ sfnt->get_paint_layers( ttface, layer_iterator, paint );
    ಇಲ್ಲದಿದ್ದರೆ
      ಹಿಂತಿರುಗಿ 0;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Get_Paint( FT_Face face,
                FT_OpaquePaint  opaque_paint,
                FT_COLR_Paint*  paint )
  {
    TT_Face       ttface;
    SFNT_Service  sfnt;


    ಆದರೆ ( !face || !paint )
      ಹಿಂತಿರುಗಿ 0;

    ಆದರೆ ( !FT_IS_SFNT( face ) )
      ಹಿಂತಿರುಗಿ 0;

    ttface = (TT_Face)face;
    sfnt   = (SFNT_Service)ttface->sfnt;

    ಆದರೆ ( sfnt->get_paint )
      ಹಿಂತಿರುಗಿ sfnt->get_paint( ttface, opaque_paint, paint );
    ಇಲ್ಲದಿದ್ದರೆ
      ಹಿಂತಿರುಗಿ 0;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Get_Colorline_Stops ( FT_Face                face,
                           FT_ColorStop *         color_stop,
                           FT_ColorStopIterator  *iterator )
  {
    TT_Face       ttface;
    SFNT_Service  sfnt;


    ಆದರೆ ( !face || !color_stop || !iterator )
      ಹಿಂತಿರುಗಿ 0;

    ಆದರೆ ( !FT_IS_SFNT( face ) )
      ಹಿಂತಿರುಗಿ 0;

    ttface = (TT_Face)face;
    sfnt   = (SFNT_Service)ttface->sfnt;

    ಆದರೆ ( sfnt->get_colorline_stops )
      ಹಿಂತಿರುಗಿ sfnt->get_colorline_stops ( ttface, color_stop, iterator );
    ಇಲ್ಲದಿದ್ದರೆ
      ಹಿಂತಿರುಗಿ 0;
  }


/*ಕೊನೆ*/
