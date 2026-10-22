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


#అన్తర్భావ <freetype/ftlist.h>
#అన్తర్భావ <freetype/ftoutln.h>
#అన్తర్భావ <freetype/ftfntfmt.h>
#అన్తర్భావ <freetype/otsvg.h>

#అన్తర్భావ <freetype/internal/ftvalid.h>
#అన్తర్భావ <freetype/internal/ftobjs.h>
#అన్తర్భావ <freetype/internal/ftdebug.h>
#అన్తర్భావ <freetype/internal/ftrfork.h>
#అన్తర్భావ <freetype/internal/ftstream.h>
#అన్తర్భావ <freetype/internal/sfnt.h>          /* for SFNT_Load_Table_Func */
#అన్తర్భావ <freetype/internal/psaux.h>         /* for PS_Driver            */
#అన్తర్భావ <freetype/internal/svginterface.h>

#అన్తర్భావ <freetype/tttables.h>
#అన్తర్భావ <freetype/tttags.h>
#అన్తర్భావ <freetype/ttnameid.h>

#అన్తర్భావ <freetype/internal/services/svprop.h>
#అన్తర్భావ <freetype/internal/services/svsfnt.h>
#అన్తర్భావ <freetype/internal/services/svpostnm.h>
#అన్తర్భావ <freetype/internal/services/svgldict.h>
#అన్తర్భావ <freetype/internal/services/svttcmap.h>
#అన్తర్భావ <freetype/internal/services/svkern.h>
#అన్తర్భావ <freetype/internal/services/svtteng.h>

#నిర్వచితమైతే TT_CONFIG_OPTION_VARC
#అన్తర్భావ <freetype/internal/services/svvarc.h>
#అయితే_ముగింపు

#అన్తర్భావ <freetype/ftdriver.h>

#నిర్వచితమైతే FT_CONFIG_OPTION_MAC_FONTS
#అన్తర్భావ "ftbase.h"
#అయితే_ముగింపు


#నిర్వచితమైతే FT_DEBUG_LEVEL_TRACE

#అన్తర్భావ <freetype/ftbitmap.h>

#ఒకవేళ defined( _MSC_VER )      /* Visual C++ (and Intel C++)   */
  /* We disable the warning `conversion from XXX to YYY,     */
  /* possible loss of data' in order to compile cleanly with */
  /* the maximum level of warnings: `md5.c' is non-FreeType  */
  /* code, and it gets used during development builds only.  */
#సూచన warning( నెట్టు )
#సూచన warning( disable : 4244 )
#అయితే_ముగింపు /* _MSC_VER */

  /* It's easiest to include `md5.c' directly.  However, since OpenSSL */
  /* also provides the same functions, there might be conflicts if     */
  /* both FreeType and OpenSSL are built as static libraries.  For     */
  /* this reason, we put the MD5 stuff into the `FT_' namespace.       */
#నిర్వచించు MD5_u32plus  FT_MD5_u32plus
#నిర్వచించు MD5_CTX      FT_MD5_CTX
#నిర్వచించు MD5_Init     FT_MD5_Init
#నిర్వచించు MD5_Update   FT_MD5_Update
#నిర్వచించు MD5_Final    FT_MD5_Final

#నిర్వచనం_రద్దు  HAVE_OPENSSL

#అన్తర్భావ "md5.c"

#ఒకవేళ defined( _MSC_VER )
#సూచన warning( తీయి )
#అయితే_ముగింపు

  /* This array must stay in sync with the @FT_Pixel_Mode enumeration */
  /* (in file `ftimage.h`).                                           */

  స్థిర స్థిరరాశి అక్షరం* స్థిరరాశి  pixel_modes[] =
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

#అయితే_ముగింపు /* FT_DEBUG_LEVEL_TRACE */


#నిర్వచించు GRID_FIT_METRICS


  /* forward declaration */
  స్థిర FT_Error
  ft_open_face_అంతర్గత( FT_Library           library,
                         స్థిరరాశి FT_Open_వాదనలు*  వాదనలు,
                         FT_Long              face_సూచీ,
                         FT_Face             *aface,
                         FT_Bool              test_mac_fonts );


  FT_పునాది_DEF( FT_సూచిక )
  ft_service_list_lookup( FT_ServiceDesc  service_descriptors,
                          స్థిరరాశి అక్షరం*     service_id )
  {
    FT_సూచిక      ఫలితం = ఖాళీ;
    FT_ServiceDesc  desc   = service_descriptors;


    ఒకవేళ ( desc && service_id )
    {
      కొరకు ( ; desc->serv_id != ఖాళీ; desc++ )
      {
        ఒకవేళ ( ft_strcmp( desc->serv_id, service_id ) == 0 )
        {
          ఫలితం = (FT_సూచిక)desc->serv_దత్తాంశం;
          ఆపు;
        }
      }
    }

    తిరిగి ఫలితం;
  }


  FT_పునాది_DEF( శూన్య )
  ft_validator_init( FT_Validator        valid,
                     స్థిరరాశి FT_Byte*      పునాది,
                     స్థిరరాశి FT_Byte*      హద్దు,
                     FT_ValidationLevel  level )
  {
    valid->పునాది  = పునాది;
    valid->హద్దు = హద్దు;
    valid->level = level;
    valid->error = FT_Err_Ok;
  }


  FT_పునాది_DEF( FT_Int )
  ft_validator_నిరంతరం( FT_Validator  valid )
  {
    /* This function doesn't work!  None should call it. */
    FT_వాడని( valid );

    తిరిగి -1;
  }


  FT_పునాది_DEF( శూన్య )
  ft_validator_error( FT_Validator  valid,
                      FT_Error      error )
  {
    /* since the cast below also disables the compiler's */
    /* type check, we introduce a dummy variable, which  */
    /* will be optimized away                            */
    అస్థిర ft_jmp_buf* jump_బఫరు = &valid->jump_బఫరు;


    valid->error = error;

    /* throw away volatileness; use `jump_buffer' or the  */
    /* compiler may warn about an unused local variable   */
    ft_longjmp( *(ft_jmp_buf*) jump_బఫరు, 1 );
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
  FT_పునాది_DEF( FT_Error )
  FT_ప్రవాహం_New( FT_Library           library,
                 స్థిరరాశి FT_Open_వాదనలు*  వాదనలు,
                 FT_ప్రవాహం           *astream )
  {
    FT_Error   error;
    FT_జ్ఞాపకం  జ్ఞాపకం;
    FT_ప్రవాహం  ప్రవాహం = ఖాళీ;
    FT_UInt    బహుళకం;


    *astream = ఖాళీ;

    ఒకవేళ ( !library )
      తిరిగి FT_THROW( Invalid_Library_Handle );

    ఒకవేళ ( !వాదనలు )
      తిరిగి FT_THROW( Invalid_వాదం );

    జ్ఞాపకం = library->జ్ఞాపకం;
    బహుళకం   = వాదనలు->flags &
               ( FT_OPEN_జ్ఞాపకం | FT_OPEN_ప్రవాహం | FT_OPEN_PATHNAME );

    ఒకవేళ ( బహుళకం == FT_OPEN_జ్ఞాపకం )
    {
      /* create a memory-based stream */
      ఒకవేళ ( FT_NEW( ప్రవాహం ) )
        వెళ్ళు Exit;

      FT_ప్రవాహం_Open_జ్ఞాపకం( ప్రవాహం,
                            (స్థిరరాశి FT_Byte*)వాదనలు->జ్ఞాపకం_పునాది,
                            (FT_ULong)వాదనలు->జ్ఞాపకం_కొలత );
      ప్రవాహం->జ్ఞాపకం = జ్ఞాపకం;
    }

#నిర్వచితం_కాకపోతే FT_CONFIG_OPTION_DISABLE_ప్రవాహం_SUPPORT

    లేకపోతే ఒకవేళ ( బహుళకం == FT_OPEN_PATHNAME )
    {
      /* create a normal system stream */
      ఒకవేళ ( FT_NEW( ప్రవాహం ) )
        వెళ్ళు Exit;

      ప్రవాహం->జ్ఞాపకం = జ్ఞాపకం;
      error = FT_ప్రవాహం_Open( ప్రవాహం, వాదనలు->pathname );
      ఒకవేళ ( error )
        FT_FREE( ప్రవాహం );
    }
    లేకపోతే ఒకవేళ ( ( బహుళకం == FT_OPEN_ప్రవాహం ) && వాదనలు->ప్రవాహం )
    {
      /* use an existing, user-provided stream */

      /* in this case, we do not need to allocate a new stream object */
      /* since the caller is responsible for closing it himself       */
      ప్రవాహం         = వాదనలు->ప్రవాహం;
      ప్రవాహం->జ్ఞాపకం = జ్ఞాపకం;
      error          = FT_Err_Ok;
    }

#అయితే_ముగింపు

    లేకపోతే
    {
      error = FT_THROW( Invalid_వాదం );
      ఒకవేళ ( ( వాదనలు->flags & FT_OPEN_ప్రవాహం ) && వాదనలు->ప్రవాహం )
        FT_ప్రవాహం_Close( వాదనలు->ప్రవాహం );
    }

    ఒకవేళ ( !error )
      *astream       = ప్రవాహం;

  Exit:
    తిరిగి error;
  }


  FT_పునాది_DEF( శూన్య )
  FT_ప్రవాహం_Free( FT_ప్రవాహం  ప్రవాహం,
                  FT_Int     external )
  {
    ఒకవేళ ( ప్రవాహం )
    {
      FT_జ్ఞాపకం  జ్ఞాపకం = ప్రవాహం->జ్ఞాపకం;


      FT_ప్రవాహం_Close( ప్రవాహం );

      ఒకవేళ ( !external )
        FT_FREE( ప్రవాహం );
    }
  }


  /**************************************************************************
   *
   * The macro FT_COMPONENT is used in trace mode.  It is an implicit
   * parameter of the FT_TRACE() and FT_ERROR() macros, used to print/log
   * messages during execution.
   */
#నిర్వచనం_రద్దు  FT_COMPONENT
#నిర్వచించు FT_COMPONENT  objs


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


  స్థిర FT_Error
  ft_glyphslot_init( FT_Glyph_స్థానం  స్థానం )
  {
    FT_Driver         driver   = స్థానం->face->driver;
    FT_Driver_Class   clazz    = driver->clazz;
    FT_జ్ఞాపకం         జ్ఞాపకం   = driver->మూలం.జ్ఞాపకం;
    FT_Error          error    = FT_Err_Ok;
    FT_స్థానం_అంతర్గత  అంతర్గత = ఖాళీ;


    స్థానం->library = driver->మూలం.library;

    ఒకవేళ ( FT_NEW( అంతర్గత ) )
      వెళ్ళు Exit;

    స్థానం->అంతర్గత = అంతర్గత;

    ఒకవేళ ( FT_DRIVER_ఉపయోగిస్తుంది_OUTLINES( driver ) )
      error = FT_GlyphLoader_New( జ్ఞాపకం, &అంతర్గత->loader );

    ఒకవేళ ( !error && clazz->init_స్థానం )
      error = clazz->init_స్థానం( స్థానం );

#నిర్వచితమైతే FT_CONFIG_OPTION_SVG
    /* if SVG table exists, allocate the space in `slot->other` */
    ఒకవేళ ( స్థానం->face->face_flags & FT_FACE_FLAG_SVG )
    {
      FT_SVG_Document  document = ఖాళీ;


      ఒకవేళ ( FT_NEW( document ) )
        వెళ్ళు Exit;
      స్థానం->other = document;
    }
#అయితే_ముగింపు

  Exit:
    తిరిగి error;
  }


  FT_పునాది_DEF( శూన్య )
  ft_glyphslot_free_bitmap( FT_Glyph_స్థానం  స్థానం )
  {
    ఒకవేళ ( స్థానం->అంతర్గత && ( స్థానం->అంతర్గత->flags & FT_GLYPH_OWN_BITMAP ) )
    {
      FT_జ్ఞాపకం  జ్ఞాపకం = FT_FACE_జ్ఞాపకం( స్థానం->face );


      FT_FREE( స్థానం->bitmap.బఫరు );
      స్థానం->అంతర్గత->flags &= ~FT_GLYPH_OWN_BITMAP;
    }
    లేకపోతే
    {
      /* assume that the bitmap buffer was stolen or not */
      /* allocated from the heap                         */
      స్థానం->bitmap.బఫరు = ఖాళీ;
    }
  }


  /* overflow-resistant presetting of bitmap position and dimensions; */
  /* also check whether the size is too large for rendering           */
  FT_పునాది_DEF( FT_Bool )
  ft_glyphslot_preset_bitmap( FT_Glyph_స్థానం      స్థానం,
                              FT_Render_బహుళకం    బహుళకం,
                              స్థిరరాశి FT_సదిశ*  origin )
  {
    FT_Outline*  outline = &స్థానం->outline;
    FT_Bitmap*   bitmap  = &స్థానం->bitmap;

    FT_Pixel_బహుళకం  pixel_బహుళకం;

    FT_BBox  cbox, pbox;
    FT_Pos   x_జరుపు = 0;
    FT_Pos   y_జరుపు = 0;
    FT_Pos   x_ఎడమ, y_శిఖరం;
    FT_Pos   వెడల్పు, ఎత్తు, pitch;
    FT_Bool  ret;


    ఒకవేళ ( స్థానం->format == FT_GLYPH_FORMAT_SVG )
    {
      FT_Module    module;
      SVG_Service  svg_service;


      module      = FT_Get_Module( స్థానం->library, "ot-svg" );
      svg_service = (SVG_Service)module->clazz->module_interface;

      తిరిగి (FT_Bool)svg_service->preset_స్థానం( module, స్థానం, FALSE );
    }
    లేకపోతే ఒకవేళ ( స్థానం->format != FT_GLYPH_FORMAT_OUTLINE )
      తిరిగి 1;

    ఒకవేళ ( origin )
    {
      x_జరుపు = origin->x;
      y_జరుపు = origin->y;
    }

    /* compute the control box, and grid-fit it, */
    /* taking into account the origin shift      */
    FT_Outline_Get_CBox( outline, &cbox );

    /* rough estimate of pixel box */
    pbox.xMin = ( cbox.xMin >> 6 ) + ( x_జరుపు >> 6 );
    pbox.yMin = ( cbox.yMin >> 6 ) + ( y_జరుపు >> 6 );
    pbox.xMax = ( cbox.xMax >> 6 ) + ( x_జరుపు >> 6 );
    pbox.yMax = ( cbox.yMax >> 6 ) + ( y_జరుపు >> 6 );

    /* tiny remainder box */
    cbox.xMin = ( cbox.xMin & 63 ) + ( x_జరుపు & 63 );
    cbox.yMin = ( cbox.yMin & 63 ) + ( y_జరుపు & 63 );
    cbox.xMax = ( cbox.xMax & 63 ) + ( x_జరుపు & 63 );
    cbox.yMax = ( cbox.yMax & 63 ) + ( y_జరుపు & 63 );

    ఎంపిక ( బహుళకం )
    {
    స్థితి FT_RENDER_బహుళకం_MONO:
      pixel_బహుళకం = FT_PIXEL_బహుళకం_MONO;
#ఒకవేళ 1
      /* x */

      /* undocumented but confirmed: bbox values get rounded;    */
      /* we do asymmetric rounding so that the center of a pixel */
      /* gets always included                                    */

      pbox.xMin += ( cbox.xMin + 31 ) >> 6;
      pbox.xMax += ( cbox.xMax + 32 ) >> 6;

      /* if the bbox collapsed, we add a pixel based on the total */
      /* rounding remainder to cover most of the original cbox    */

      ఒకవేళ ( pbox.xMin == pbox.xMax )
      {
        ఒకవేళ ( ( ( cbox.xMin + 31 ) & 63 ) - 31 +
             ( ( cbox.xMax + 32 ) & 63 ) - 32 < 0 )
          pbox.xMin -= 1;
        లేకపోతే
          pbox.xMax += 1;
      }

      /* y */

      pbox.yMin += ( cbox.yMin + 31 ) >> 6;
      pbox.yMax += ( cbox.yMax + 32 ) >> 6;

      ఒకవేళ ( pbox.yMin == pbox.yMax )
      {
        ఒకవేళ ( ( ( cbox.yMin + 31 ) & 63 ) - 31 +
             ( ( cbox.yMax + 32 ) & 63 ) - 32 < 0 )
          pbox.yMin -= 1;
        లేకపోతే
          pbox.yMax += 1;
      }

      ఆపు;
#లేకపోతే
      వెళ్ళు Adjust;
#అయితే_ముగింపు

    స్థితి FT_RENDER_బహుళకం_LCD:
      pixel_బహుళకం = FT_PIXEL_బహుళకం_LCD;
      ft_lcd_padding( &cbox, స్థానం, బహుళకం );
      వెళ్ళు Adjust;

    స్థితి FT_RENDER_బహుళకం_LCD_V:
      pixel_బహుళకం = FT_PIXEL_బహుళకం_LCD_V;
      ft_lcd_padding( &cbox, స్థానం, బహుళకం );
      వెళ్ళు Adjust;

    స్థితి FT_RENDER_బహుళకం_సాధారణ:
    స్థితి FT_RENDER_బహుళకం_LIGHT:
    అప్రమేయ:
      pixel_బహుళకం = FT_PIXEL_బహుళకం_బూడిద;
    Adjust:
      pbox.xMin += cbox.xMin >> 6;
      pbox.yMin += cbox.yMin >> 6;
      pbox.xMax += ( cbox.xMax + 63 ) >> 6;
      pbox.yMax += ( cbox.yMax + 63 ) >> 6;
    }

    x_ఎడమ = pbox.xMin;
    y_శిఖరం  = pbox.yMax;

    వెడల్పు  = pbox.xMax - pbox.xMin;
    ఎత్తు = pbox.yMax - pbox.yMin;

    /* Flag the width or height unsuitable for rendering.   */
    /* The limit is based on the ppem value when available. */
    /* FT_Renderer modules should check the return value.   */
    ret = FT_BOOL(     వెడల్పు >= 0x10000   ||    ఎత్తు >= 0x10000   ||
                   pbox.xMin < -0x1000000 || pbox.xMax >= 0x1000000 ||
                   pbox.yMin < -0x1000000 || pbox.yMax >= 0x1000000 ||
             ( స్థానం->face                                         &&
               ( వెడల్పు  > 16 * స్థానం->face->కొలత->metrics.x_ppem ||
                 ఎత్తు > 16 * స్థానం->face->కొలత->metrics.y_ppem ) ) );

    ఒకవేళ ( ret )
      FT_TRACE3(( "ft_glyphslot_preset_bitmap: [%ld %ld %ld %ld]\n",
                  pbox.xMin, pbox.yMin, pbox.xMax, pbox.yMax ));

    ఎంపిక ( pixel_బహుళకం )
    {
    స్థితి FT_PIXEL_బహుళకం_MONO:
      pitch = ( ( వెడల్పు + 15 ) >> 4 ) << 1;
      ఆపు;

    స్థితి FT_PIXEL_బహుళకం_LCD:
      వెడల్పు *= 3;
      pitch  = FT_PAD_CEIL( వెడల్పు, 4 );
      ఆపు;

    స్థితి FT_PIXEL_బహుళకం_LCD_V:
      ఎత్తు *= 3;
      FALL_THROUGH;

    స్థితి FT_PIXEL_బహుళకం_బూడిద:
    అప్రమేయ:
      pitch = వెడల్పు;
    }

    స్థానం->bitmap_ఎడమ = (FT_Int)x_ఎడమ;
    స్థానం->bitmap_శిఖరం  = (FT_Int)y_శిఖరం;

    bitmap->pixel_బహుళకం = (గుర్తులేని అక్షరం)pixel_బహుళకం;
    bitmap->అంకె_grays  = 256;
    bitmap->వెడల్పు      = (గుర్తులేని పూర్ణాంకం)వెడల్పు;
    bitmap->అడ్డువరుసలు       = (గుర్తులేని పూర్ణాంకం)ఎత్తు;
    bitmap->pitch      = pitch;

    తిరిగి ret;
  }


  FT_పునాది_DEF( శూన్య )
  ft_glyphslot_సమితి_bitmap( FT_Glyph_స్థానం  స్థానం,
                           FT_Byte*      బఫరు )
  {
    ft_glyphslot_free_bitmap( స్థానం );

    స్థానం->bitmap.బఫరు = బఫరు;

    FT_ASSERT( (స్థానం->అంతర్గత->flags & FT_GLYPH_OWN_BITMAP) == 0 );
  }


  FT_పునాది_DEF( FT_Error )
  ft_glyphslot_alloc_bitmap( FT_Glyph_స్థానం  స్థానం )
  {
    FT_జ్ఞాపకం  జ్ఞాపకం = FT_FACE_జ్ఞాపకం( స్థానం->face );
    FT_Error   error;


    ఒకవేళ ( స్థానం->అంతర్గత->flags & FT_GLYPH_OWN_BITMAP )
      FT_FREE( స్థానం->bitmap.బఫరు );
    లేకపోతే
      స్థానం->అంతర్గత->flags |= FT_GLYPH_OWN_BITMAP;

    /* dimensions must be preset */
    FT_MEM_ALLOC_MULT( స్థానం->bitmap.బఫరు,
                       స్థానం->bitmap.అడ్డువరుసలు,
                       స్థానం->bitmap.pitch );
    తిరిగి error;
  }


  స్థిర శూన్య
  ft_glyphslot_clear( FT_Glyph_స్థానం  స్థానం )
  {
    /* free bitmap if needed */
    ft_glyphslot_free_bitmap( స్థానం );

    /* clear all public fields in the glyph slot */
    స్థానం->glyph_సూచీ = 0;

    FT_ZERO( &స్థానం->metrics );
    FT_ZERO( &స్థానం->outline );

    స్థానం->bitmap.వెడల్పు      = 0;
    స్థానం->bitmap.అడ్డువరుసలు       = 0;
    స్థానం->bitmap.pitch      = 0;
    స్థానం->bitmap.pixel_బహుళకం = 0;
    /* `slot->bitmap.buffer' has been handled by ft_glyphslot_free_bitmap */

    స్థానం->bitmap_ఎడమ   = 0;
    స్థానం->bitmap_శిఖరం    = 0;
    స్థానం->అంకె_subglyphs = 0;
    స్థానం->subglyphs     = ఖాళీ;
    స్థానం->control_దత్తాంశం  = ఖాళీ;
    స్థానం->control_len   = 0;

#నిర్వచితం_కాకపోతే FT_CONFIG_OPTION_SVG
    స్థానం->other = ఖాళీ;
#లేకపోతే
    ఒకవేళ ( !( స్థానం->face->face_flags & FT_FACE_FLAG_SVG ) )
      స్థానం->other = ఖాళీ;
    లేకపోతే
    {
      ఒకవేళ ( స్థానం->అంతర్గత->flags & FT_GLYPH_OWN_GZIP_SVG )
      {
        FT_జ్ఞాపకం        జ్ఞాపకం = స్థానం->face->జ్ఞాపకం;
        FT_SVG_Document  doc    = (FT_SVG_Document)స్థానం->other;


        FT_FREE( doc->svg_document );
        స్థానం->అంతర్గత->flags &= ~FT_GLYPH_OWN_GZIP_SVG;
      }
    }
#అయితే_ముగింపు

    స్థానం->format = FT_GLYPH_FORMAT_NONE;

    స్థానం->రేఖీయ_Hori_Advance = 0;
    స్థానం->రేఖీయ_Vert_Advance = 0;
    స్థానం->advance.x         = 0;
    స్థానం->advance.y         = 0;
    స్థానం->lsb_delta         = 0;
    స్థానం->rsb_delta         = 0;
  }


  స్థిర శూన్య
  ft_glyphslot_పూర్తి( FT_Glyph_స్థానం  స్థానం )
  {
    FT_Driver        driver = స్థానం->face->driver;
    FT_Driver_Class  clazz  = driver->clazz;
    FT_జ్ఞాపకం        జ్ఞాపకం = driver->మూలం.జ్ఞాపకం;

#నిర్వచితమైతే FT_CONFIG_OPTION_SVG
    ఒకవేళ ( స్థానం->face->face_flags & FT_FACE_FLAG_SVG )
    {
      /* Free memory in case SVG was there.                          */
      /* `slot->internal` might be NULL in out-of-memory situations. */
      ఒకవేళ ( స్థానం->అంతర్గత && స్థానం->అంతర్గత->flags & FT_GLYPH_OWN_GZIP_SVG )
      {
        FT_SVG_Document  doc = (FT_SVG_Document)స్థానం->other;


        FT_FREE( doc->svg_document );

        స్థానం->అంతర్గత->flags &= ~FT_GLYPH_OWN_GZIP_SVG;
      }

      FT_FREE( స్థానం->other );
    }
#అయితే_ముగింపు

    ఒకవేళ ( clazz->పూర్తి_స్థానం )
      clazz->పూర్తి_స్థానం( స్థానం );

    /* free bitmap buffer if needed */
    ft_glyphslot_free_bitmap( స్థానం );

    /* slot->internal might be NULL in out-of-memory situations */
    ఒకవేళ ( స్థానం->అంతర్గత )
    {
      /* free glyph loader */
      ఒకవేళ ( FT_DRIVER_ఉపయోగిస్తుంది_OUTLINES( driver ) )
      {
        FT_Glyph_Loader_పూర్తి( స్థానం->అంతర్గత->loader );
        స్థానం->అంతర్గత->loader = ఖాళీ;
      }

      FT_FREE( స్థానం->అంతర్గత );
    }
  }


  /* documentation is in ftobjs.h */

  FT_పునాది_DEF( FT_Error )
  FT_New_Glyph_స్థానం( FT_Face        face,
                    FT_Glyph_స్థానం  *aslot )
  {
    FT_Error         error;
    FT_Driver        driver;
    FT_Driver_Class  clazz;
    FT_జ్ఞాపకం        జ్ఞాపకం;
    FT_Glyph_స్థానం     స్థానం = ఖాళీ;


    ఒకవేళ ( !face )
      తిరిగి FT_THROW( Invalid_Face_Handle );

    ఒకవేళ ( !face->driver )
      తిరిగి FT_THROW( Invalid_వాదం );

    driver = face->driver;
    clazz  = driver->clazz;
    జ్ఞాపకం = driver->మూలం.జ్ఞాపకం;

    FT_TRACE4(( "FT_New_GlyphSlot: Creating new slot object\n" ));
    ఒకవేళ ( !FT_ALLOC( స్థానం, clazz->స్థానం_object_కొలత ) )
    {
      స్థానం->face = face;

      error = ft_glyphslot_init( స్థానం );
      ఒకవేళ ( error )
      {
        ft_glyphslot_పూర్తి( స్థానం );
        FT_FREE( స్థానం );
        వెళ్ళు Exit;
      }

      స్థానం->తదుపరి  = face->glyph;
      face->glyph = స్థానం;

      ఒకవేళ ( aslot )
        *aslot = స్థానం;
    }
    లేకపోతే ఒకవేళ ( aslot )
      *aslot = ఖాళీ;


  Exit:
    FT_TRACE4(( "FT_New_GlyphSlot: Return 0x%x\n", error ));

    తిరిగి error;
  }


  /* documentation is in ftobjs.h */

  FT_పునాది_DEF( శూన్య )
  FT_పూర్తి_Glyph_స్థానం( FT_Glyph_స్థానం  స్థానం )
  {
    ఒకవేళ ( స్థానం )
    {
      FT_Driver     driver = స్థానం->face->driver;
      FT_జ్ఞాపకం     జ్ఞాపకం = driver->మూలం.జ్ఞాపకం;
      FT_Glyph_స్థానం  prev;
      FT_Glyph_స్థానం  cur;


      /* Remove slot from its parent face's list */
      prev = ఖాళీ;
      cur  = స్థానం->face->glyph;

      అయితే ( cur )
      {
        ఒకవేళ ( cur == స్థానం )
        {
          ఒకవేళ ( !prev )
            స్థానం->face->glyph = cur->తదుపరి;
          లేకపోతే
            prev->తదుపరి = cur->తదుపరి;

          /* finalize client-specific data */
          ఒకవేళ ( స్థానం->generic.finalizer )
            స్థానం->generic.finalizer( స్థానం );

          ft_glyphslot_పూర్తి( స్థానం );
          FT_FREE( స్థానం );
          ఆపు;
        }
        prev = cur;
        cur  = cur->తదుపరి;
      }
    }
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( శూన్య )
  FT_సమితి_Transform( FT_Face     face,
                    FT_మాతృక*  మాతృక,
                    FT_సదిశ*  delta )
  {
    FT_Face_అంతర్గత  అంతర్గత;


    ఒకవేళ ( !face )
      తిరిగి;

    అంతర్గత = face->అంతర్గత;

    అంతర్గత->transform_flags = 0;

    ఒకవేళ ( !మాతృక || !FT_మాతృక_Check( మాతృక ) )
    {
      అంతర్గత->transform_మాతృక.xx = 0x10000L;
      అంతర్గత->transform_మాతృక.xy = 0;
      అంతర్గత->transform_మాతృక.yx = 0;
      అంతర్గత->transform_మాతృక.yy = 0x10000L;

      మాతృక = &అంతర్గత->transform_మాతృక;
    }
    లేకపోతే
      అంతర్గత->transform_మాతృక = *మాతృక;

    /* set transform_flags bit flag 0 if `matrix' isn't the identity */
    ఒకవేళ ( ( మాతృక->xy | మాతృక->yx ) ||
         మాతృక->xx != 0x10000L      ||
         మాతృక->yy != 0x10000L      )
      అంతర్గత->transform_flags |= 1;

    ఒకవేళ ( !delta )
    {
      అంతర్గత->transform_delta.x = 0;
      అంతర్గత->transform_delta.y = 0;

      delta = &అంతర్గత->transform_delta;
    }
    లేకపోతే
      అంతర్గత->transform_delta = *delta;

    /* set transform_flags bit flag 1 if `delta' isn't the null vector */
    ఒకవేళ ( delta->x | delta->y )
      అంతర్గత->transform_flags |= 2;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( శూన్య )
  FT_Get_Transform( FT_Face     face,
                    FT_మాతృక*  మాతృక,
                    FT_సదిశ*  delta )
  {
    FT_Face_అంతర్గత  అంతర్గత;


    ఒకవేళ ( !face )
      తిరిగి;

    అంతర్గత = face->అంతర్గత;

    ఒకవేళ ( మాతృక )
      *మాతృక = అంతర్గత->transform_మాతృక;

    ఒకవేళ ( delta )
      *delta = అంతర్గత->transform_delta;
  }


  స్థిర FT_Renderer
  ft_lookup_glyph_renderer( FT_Glyph_స్థానం  స్థానం );


#నిర్వచితమైతే GRID_FIT_METRICS
  స్థిర శూన్య
  ft_glyphslot_grid_fit_metrics( FT_Glyph_స్థానం  స్థానం,
                                 FT_Bool       vertical )
  {
    FT_Glyph_Metrics*  metrics = &స్థానం->metrics;
    FT_Pos             కుడి, bottom;


    ఒకవేళ ( vertical )
    {
      metrics->horiBearingX = FT_PIX_FLOOR( metrics->horiBearingX );
      metrics->horiBearingY = FT_PIX_CEIL_LONG( metrics->horiBearingY );

      కుడి  = FT_PIX_CEIL_LONG( కూడు_LONG( metrics->vertBearingX,
                                           metrics->వెడల్పు ) );
      bottom = FT_PIX_CEIL_LONG( కూడు_LONG( metrics->vertBearingY,
                                           metrics->ఎత్తు ) );

      metrics->vertBearingX = FT_PIX_FLOOR( metrics->vertBearingX );
      metrics->vertBearingY = FT_PIX_FLOOR( metrics->vertBearingY );

      metrics->వెడల్పు  = SUB_LONG( కుడి,
                                  metrics->vertBearingX );
      metrics->ఎత్తు = SUB_LONG( bottom,
                                  metrics->vertBearingY );
    }
    లేకపోతే
    {
      metrics->vertBearingX = FT_PIX_FLOOR( metrics->vertBearingX );
      metrics->vertBearingY = FT_PIX_FLOOR( metrics->vertBearingY );

      కుడి  = FT_PIX_CEIL_LONG( కూడు_LONG( metrics->horiBearingX,
                                           metrics->వెడల్పు ) );
      bottom = FT_PIX_FLOOR( SUB_LONG( metrics->horiBearingY,
                                       metrics->ఎత్తు ) );

      metrics->horiBearingX = FT_PIX_FLOOR( metrics->horiBearingX );
      metrics->horiBearingY = FT_PIX_CEIL_LONG( metrics->horiBearingY );

      metrics->వెడల్పు  = SUB_LONG( కుడి,
                                  metrics->horiBearingX );
      metrics->ఎత్తు = SUB_LONG( metrics->horiBearingY,
                                  bottom );
    }

    metrics->horiAdvance = FT_PIX_విడత_LONG( metrics->horiAdvance );
    metrics->vertAdvance = FT_PIX_విడత_LONG( metrics->vertAdvance );
  }
#అయితే_ముగింపు /* GRID_FIT_METRICS */


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_ఎక్కించు_Glyph( FT_Face   face,
                 FT_UInt   glyph_సూచీ,
                 FT_Int32  ఎక్కించు_flags )
  {
    FT_Error      error;
    FT_Driver     driver;
    FT_Glyph_స్థానం  స్థానం;
    FT_Library    library;
    FT_Bool       autohint = FALSE;
    FT_Module     hinter;


    ఒకవేళ ( !face || !face->కొలత || !face->glyph )
      తిరిగి FT_THROW( Invalid_Face_Handle );

    /* The validity test for `glyph_index' is performed by the */
    /* font drivers.                                           */

    స్థానం = face->glyph;
    ft_glyphslot_clear( స్థానం );

    driver  = face->driver;
    library = driver->మూలం.library;
    hinter  = library->auto_hinter;

    /* undefined scale means no scale */
    ఒకవేళ ( face->కొలత->metrics.x_ppem == 0 ||
         face->కొలత->metrics.y_ppem == 0 )
      ఎక్కించు_flags |= FT_ఎక్కించు_NO_SCALE;

    /* resolve load flags dependencies */

    ఒకవేళ ( ఎక్కించు_flags & FT_ఎక్కించు_NO_RECURSE )
      ఎక్కించు_flags |= FT_ఎక్కించు_NO_SCALE         |
                    FT_ఎక్కించు_IGNORE_TRANSFORM;

    ఒకవేళ ( ఎక్కించు_flags & FT_ఎక్కించు_NO_SCALE )
    {
      ఎక్కించు_flags |= FT_ఎక్కించు_NO_HINTING |
                    FT_ఎక్కించు_NO_BITMAP;

      ఎక్కించు_flags &= ~FT_ఎక్కించు_RENDER;
    }

    ఒకవేళ ( ఎక్కించు_flags & FT_ఎక్కించు_BITMAP_METRICS_ONLY )
      ఎక్కించు_flags &= ~FT_ఎక్కించు_RENDER;

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

    ఒకవేళ ( hinter                                           &&
         !( ఎక్కించు_flags & FT_ఎక్కించు_NO_HINTING )             &&
         !( ఎక్కించు_flags & FT_ఎక్కించు_NO_AUTOHINT )            &&
         FT_IS_SCALABLE( face )                           &&
         !FT_IS_TRICKY( face )                            &&
         ( ( ఎక్కించు_flags & FT_ఎక్కించు_IGNORE_TRANSFORM )    ||
           ( face->అంతర్గత->transform_మాతృక.yx == 0 &&
             face->అంతర్గత->transform_మాతృక.xx != 0 ) ||
           ( face->అంతర్గత->transform_మాతృక.xx == 0 &&
             face->అంతర్గత->transform_మాతృక.yx != 0 ) ) )
    {
      ఒకవేళ ( ( ఎక్కించు_flags & FT_ఎక్కించు_FORCE_AUTOHINT ) ||
           !FT_DRIVER_HAS_HINTER( driver )         )
        autohint = TRUE;
      లేకపోతే
      {
        FT_Render_బహుళకం  బహుళకం = FT_ఎక్కించు_లక్ష్యం_బహుళకం( ఎక్కించు_flags );
        FT_Bool         is_light_type1;
        TT_Face         ttface = (TT_Face)face;


        /* only the new Adobe engine (for both CFF and Type 1) is `light'; */
        /* we use `strstr' to catch both `Type 1' and `CID Type 1'         */
        is_light_type1 =
          ft_strstr( FT_Get_Font_Format( face ), "Type 1" ) != ఖాళీ &&
          ((PS_Driver)driver)->hinting_engine == FT_HINTING_ADOBE;

        /* the check for `num_locations' assures that we actually    */
        /* test for instructions in a TTF and not in a CFF-based OTF */
        /*                                                           */
        /* we check the size of the `fpgm' and `prep' tables, too -- */
        /* the assumption is that there don't exist real TTFs where  */
        /* both `fpgm' and `prep' tables are missing                 */
        ఒకవేళ ( ( బహుళకం == FT_RENDER_బహుళకం_LIGHT           &&
               ( !FT_DRIVER_HINTS_LIGHTLY( driver ) &&
                 !is_light_type1                    ) )         ||
             ( FT_IS_SFNT( face )                             &&
               ttface->అంకె_locations                          &&
               ttface->font_కార్యక్రమం_కొలత == 0                 &&
               ttface->cvt_కార్యక్రమం_కొలత <= 7                  ) )
          autohint = TRUE;
      }
    }

#నిర్వచితమైతే TT_CONFIG_OPTION_VARC
    /* Never auto-hint a VARC (variable composite) glyph: its components */
    /* are already loaded unhinted, and the auto-fitter cannot process   */
    /* the assembled composite (it also re-enters variation-coordinate   */
    /* setting).  Use the native hinter / unhinted load instead.         */
    ఒకవేళ ( autohint && FT_IS_SFNT( face ) )
    {
      FT_Service_VARC  varc;


      FT_FACE_కనుగొను_SERVICE( face, varc, VARC );
      ఒకవేళ ( varc && varc->has_glyph( face, glyph_సూచీ ) )
        autohint = FALSE;
    }
#అయితే_ముగింపు

    ఒకవేళ ( autohint )
    {
      FT_AutoHinter_Interface  hinting;


      /* XXX: The use of the `FT_LOAD_XXX_ONLY` flags is not very */
      /*      elegant.                                            */

      /* try to load SVG documents if available */
      ఒకవేళ ( ( ఎక్కించు_flags & FT_ఎక్కించు_NO_SVG ) == 0 &&
           FT_HAS_SVG( face )                   )
      {
        error = driver->clazz->ఎక్కించు_glyph( స్థానం, face->కొలత,
                                           glyph_సూచీ,
                                           ఎక్కించు_flags | FT_ఎక్కించు_SVG_ONLY );

        ఒకవేళ ( !error && స్థానం->format == FT_GLYPH_FORMAT_SVG )
          వెళ్ళు ఎక్కించు_Ok;
      }

      /* try to load embedded bitmaps if available */
      ఒకవేళ ( FT_HAS_FIXED_SIZES( face )              &&
           ( ఎక్కించు_flags & FT_ఎక్కించు_NO_BITMAP ) == 0 )
      {
        error = driver->clazz->ఎక్కించు_glyph( స్థానం, face->కొలత,
                                           glyph_సూచీ,
                                           ఎక్కించు_flags | FT_ఎక్కించు_SBITS_ONLY );

        ఒకవేళ ( !error && స్థానం->format == FT_GLYPH_FORMAT_BITMAP )
          వెళ్ళు ఎక్కించు_Ok;
      }

      {
        FT_Face_అంతర్గత  అంతర్గత        = face->అంతర్గత;
        FT_Int            transform_flags = అంతర్గత->transform_flags;


        /* since the auto-hinter calls FT_Load_Glyph by itself, */
        /* make sure that glyphs aren't transformed             */
        అంతర్గత->transform_flags = 0;

        /* load auto-hinted outline */
        hinting = (FT_AutoHinter_Interface)hinter->clazz->module_interface;

        error   = hinting->ఎక్కించు_glyph( (FT_AutoHinter)hinter,
                                       స్థానం, face->కొలత,
                                       glyph_సూచీ, ఎక్కించు_flags );

        అంతర్గత->transform_flags = transform_flags;
      }
    }
    లేకపోతే
    {
      error = driver->clazz->ఎక్కించు_glyph( స్థానం,
                                         face->కొలత,
                                         glyph_సూచీ,
                                         ఎక్కించు_flags );
      ఒకవేళ ( error )
        వెళ్ళు Exit;

      ఒకవేళ ( స్థానం->format == FT_GLYPH_FORMAT_OUTLINE )
      {
        /* check that the loaded outline is correct */
        error = FT_Outline_Check( &స్థానం->outline );
        ఒకవేళ ( error )
          వెళ్ళు Exit;

#నిర్వచితమైతే GRID_FIT_METRICS
        ఒకవేళ ( !( ఎక్కించు_flags & FT_ఎక్కించు_NO_HINTING ) )
          ft_glyphslot_grid_fit_metrics(
            స్థానం,
            FT_BOOL( ఎక్కించు_flags & FT_ఎక్కించు_VERTICAL_LAYOUT ) );
#అయితే_ముగింపు
      }
    }

  ఎక్కించు_Ok:
    /* compute the advance */
    ఒకవేళ ( ఎక్కించు_flags & FT_ఎక్కించు_VERTICAL_LAYOUT )
    {
      స్థానం->advance.x = 0;
      స్థానం->advance.y = స్థానం->metrics.vertAdvance;
    }
    లేకపోతే
    {
      స్థానం->advance.x = స్థానం->metrics.horiAdvance;
      స్థానం->advance.y = 0;
    }

    /* compute the linear advance in 16.16 pixels */
    ఒకవేళ ( ( ఎక్కించు_flags & FT_ఎక్కించు_రేఖీయ_DESIGN ) == 0 &&
         FT_IS_SCALABLE( face )                      )
    {
      FT_కొలత_Metrics*  metrics = &face->కొలత->metrics;


      /* it's tricky! */
      స్థానం->రేఖీయ_Hori_Advance = FT_MulDiv( స్థానం->రేఖీయ_Hori_Advance,
                                           metrics->x_scale, 64 );

      స్థానం->రేఖీయ_Vert_Advance = FT_MulDiv( స్థానం->రేఖీయ_Vert_Advance,
                                           metrics->y_scale, 64 );
    }

    ఒకవేళ ( ( ఎక్కించు_flags & FT_ఎక్కించు_IGNORE_TRANSFORM ) == 0 )
    {
      FT_Face_అంతర్గత  అంతర్గత = face->అంతర్గత;


      /* now, transform the glyph image if needed */
      ఒకవేళ ( అంతర్గత->transform_flags )
      {
        /* get renderer */
        FT_Renderer  renderer = ft_lookup_glyph_renderer( స్థానం );


        ఒకవేళ ( renderer )
          error = renderer->clazz->transform_glyph(
                                     renderer, స్థానం,
                                     &అంతర్గత->transform_మాతృక,
                                     &అంతర్గత->transform_delta );
        లేకపోతే ఒకవేళ ( స్థానం->format == FT_GLYPH_FORMAT_OUTLINE )
        {
          /* apply `standard' transformation if no renderer is available */
          ఒకవేళ ( అంతర్గత->transform_flags & 1 )
            FT_Outline_Transform( &స్థానం->outline,
                                  &అంతర్గత->transform_మాతృక );

          ఒకవేళ ( అంతర్గత->transform_flags & 2 )
            FT_Outline_Translate( &స్థానం->outline,
                                  అంతర్గత->transform_delta.x,
                                  అంతర్గత->transform_delta.y );
        }

        /* transform advance */
        FT_సదిశ_Transform( &స్థానం->advance, &అంతర్గత->transform_మాతృక );
      }
    }

    స్థానం->glyph_సూచీ          = glyph_సూచీ;
    స్థానం->అంతర్గత->ఎక్కించు_flags = ఎక్కించు_flags;

    /* do we need to render the image or preset the bitmap now? */
    ఒకవేళ ( !error                                    &&
         ( ఎక్కించు_flags & FT_ఎక్కించు_NO_SCALE ) == 0    &&
         స్థానం->format != FT_GLYPH_FORMAT_BITMAP    &&
         స్థానం->format != FT_GLYPH_FORMAT_సంయుక్త )
    {
      FT_Render_బహుళకం  బహుళకం = FT_ఎక్కించు_లక్ష్యం_బహుళకం( ఎక్కించు_flags );


      ఒకవేళ ( బహుళకం == FT_RENDER_బహుళకం_సాధారణ   &&
           ఎక్కించు_flags & FT_ఎక్కించు_MONOCHROME )
        బహుళకం = FT_RENDER_బహుళకం_MONO;

      ఒకవేళ ( ఎక్కించు_flags & FT_ఎక్కించు_RENDER )
        error = FT_Render_Glyph( స్థానం, బహుళకం );
      లేకపోతే
        ft_glyphslot_preset_bitmap( స్థానం, బహుళకం, ఖాళీ );
    }

#నిర్వచితమైతే FT_DEBUG_LEVEL_TRACE
    FT_TRACE5(( "FT_Load_Glyph: index %u, flags 0x%x\n",
                glyph_సూచీ, ఎక్కించు_flags ));
    FT_TRACE5(( "  bitmap %ux%u %s, %s (mode %d)\n",
                స్థానం->bitmap.వెడల్పు,
                స్థానం->bitmap.అడ్డువరుసలు,
                స్థానం->outline.points ?
                  స్థానం->bitmap.బఫరు ? "rendered"
                                      : "preset"
                                     :
                  స్థానం->అంతర్గత->flags & FT_GLYPH_OWN_BITMAP ? "owned"
                                                              : "unowned",
                pixel_modes[స్థానం->bitmap.pixel_బహుళకం],
                స్థానం->bitmap.pixel_బహుళకం ));
    FT_TRACE5(( "\n" ));
    FT_TRACE5(( "  x advance: %f\n", (ద్విగుణ)స్థానం->advance.x / 64 ));
    FT_TRACE5(( "  y advance: %f\n", (ద్విగుణ)స్థానం->advance.y / 64 ));
    FT_TRACE5(( "  linear x advance: %f\n",
                (ద్విగుణ)స్థానం->రేఖీయ_Hori_Advance / 65536 ));
    FT_TRACE5(( "  linear y advance: %f\n",
                (ద్విగుణ)స్థానం->రేఖీయ_Vert_Advance / 65536 ));

    {
      FT_Glyph_Metrics*  metrics = &స్థానం->metrics;


      FT_TRACE5(( "  metrics:\n" ));
      FT_TRACE5(( "    width:  %f\n", (ద్విగుణ)metrics->వెడల్పు / 64 ));
      FT_TRACE5(( "    height: %f\n", (ద్విగుణ)metrics->ఎత్తు / 64 ));
      FT_TRACE5(( "\n" ));
      FT_TRACE5(( "    horiBearingX: %f\n",
                  (ద్విగుణ)metrics->horiBearingX / 64 ));
      FT_TRACE5(( "    horiBearingY: %f\n",
                  (ద్విగుణ)metrics->horiBearingY / 64 ));
      FT_TRACE5(( "    horiAdvance:  %f\n",
                  (ద్విగుణ)metrics->horiAdvance / 64 ));
      FT_TRACE5(( "\n" ));
      FT_TRACE5(( "    vertBearingX: %f\n",
                  (ద్విగుణ)metrics->vertBearingX / 64 ));
      FT_TRACE5(( "    vertBearingY: %f\n",
                  (ద్విగుణ)metrics->vertBearingY / 64 ));
      FT_TRACE5(( "    vertAdvance:  %f\n",
                  (ద్విగుణ)metrics->vertAdvance / 64 ));
    }
#అయితే_ముగింపు

  Exit:
    తిరిగి error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_ఎక్కించు_Char( FT_Face   face,
                FT_ULong  char_సంకేతం,
                FT_Int32  ఎక్కించు_flags )
  {
    FT_UInt  glyph_సూచీ;


    ఒకవేళ ( !face )
      తిరిగి FT_THROW( Invalid_Face_Handle );

    glyph_సూచీ = (FT_UInt)char_సంకేతం;
    ఒకవేళ ( face->charmap )
      glyph_సూచీ = FT_Get_Char_సూచీ( face, char_సంకేతం );

    తిరిగి FT_ఎక్కించు_Glyph( face, glyph_సూచీ, ఎక్కించు_flags );
  }


  /* destructor for sizes list */
  స్థిర శూన్య
  destroy_కొలత( FT_జ్ఞాపకం  జ్ఞాపకం,
                శూన్య*      కొలత_2,
                శూన్య*      driver_ )
  {
    FT_కొలత    కొలత   = (FT_కొలత)కొలత_2;
    FT_Driver  driver = (FT_Driver)driver_;


    /* finalize format-specific stuff */
    ఒకవేళ ( driver->clazz->పూర్తి_కొలత )
      driver->clazz->పూర్తి_కొలత( కొలత );

    /* finalize client-specific data */
    ఒకవేళ ( కొలత->generic.finalizer )
      కొలత->generic.finalizer( కొలత );

    FT_FREE( కొలత->అంతర్గత );
    FT_FREE( కొలత );
  }


  స్థిర శూన్య
  ft_cmap_పూర్తి_అంతర్గత( FT_CMap  cmap );


  స్థిర శూన్య
  destroy_charmaps( FT_Face    face,
                    FT_జ్ఞాపకం  జ్ఞాపకం )
  {
    FT_Int  n;


    ఒకవేళ ( !face )
      తిరిగి;

    కొరకు ( n = 0; n < face->అంకె_charmaps; n++ )
    {
      FT_CMap  cmap = FT_CMAP( face->charmaps[n] );


      ft_cmap_పూర్తి_అంతర్గత( cmap );

      face->charmaps[n] = ఖాళీ;
    }

    FT_FREE( face->charmaps );
    face->అంకె_charmaps = 0;
  }


  /* destructor for faces list */
  స్థిర శూన్య
  destroy_face( FT_జ్ఞాపకం  జ్ఞాపకం,
                శూన్య*      face_,
                శూన్య*      driver_ )
  {
    FT_Face          face   = (FT_Face)face_;
    FT_Driver        driver = (FT_Driver)driver_;
    FT_Driver_Class  clazz  = driver->clazz;


    /* discard auto-hinting data */
    ఒకవేళ ( face->autohint.finalizer )
      face->autohint.finalizer( face->autohint.దత్తాంశం );

    /* Discard glyph slots for this face.                           */
    /* Beware!  FT_Done_GlyphSlot() changes the field `face->glyph' */
    అయితే ( face->glyph )
      FT_పూర్తి_Glyph_స్థానం( face->glyph );

    /* discard all sizes for this face */
    FT_List_Finalize( &face->sizes_list,
                      destroy_కొలత,
                      జ్ఞాపకం,
                      driver );
    face->కొలత = ఖాళీ;

    /* discard charmaps */
    destroy_charmaps( face, జ్ఞాపకం );

    /* finalize format-specific stuff */
    ఒకవేళ ( clazz->పూర్తి_face )
      clazz->పూర్తి_face( face );

    /* close the stream for this face if needed */
    FT_ప్రవాహం_Free(
      face->ప్రవాహం,
      ( face->face_flags & FT_FACE_FLAG_EXTERNAL_ప్రవాహం ) != 0 );

    face->ప్రవాహం = ఖాళీ;

    /* now discard client data */
    ఒకవేళ ( face->generic.finalizer )
      face->generic.finalizer( face );

    /* get rid of it */
    ఒకవేళ ( face->అంతర్గత )
    {
      FT_FREE( face->అంతర్గత );
    }
    FT_FREE( face );
  }


  స్థిర శూన్య
  Destroy_Driver( FT_Driver  driver )
  {
    FT_List_Finalize( &driver->faces_list,
                      destroy_face,
                      driver->మూలం.జ్ఞాపకం,
                      driver );
  }


  /* documentation is in ftobjs.h */

  FT_పునాది_DEF( FT_Error )
  కనుగొను_unicode_charmap( FT_Face  face )
  {
    FT_CharMap*  మొదటి;
    FT_CharMap*  cur;


    /* caller should have already checked that `face' is valid */
    FT_ASSERT( face );

    మొదటి = face->charmaps;

    ఒకవేళ ( !మొదటి )
      తిరిగి FT_THROW( Invalid_CharMap_Handle );

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

    cur = మొదటి + face->అంకె_charmaps;  /* points after the last one */

    కొరకు ( ; --cur >= మొదటి; )
    {
      ఒకవేళ ( cur[0]->encoding == FT_ENCODING_UNICODE )
      {
        /* XXX If some new encodings to represent UCS-4 are added, */
        /*     they should be added here.                          */
        ఒకవేళ ( ( cur[0]->platform_id == TT_PLATFORM_MICROSOFT &&
               cur[0]->encoding_id == TT_MS_ID_UCS_4        )     ||
             ( cur[0]->platform_id == TT_PLATFORM_APPLE_UNICODE &&
               cur[0]->encoding_id == TT_APPLE_ID_UNICODE_32    ) ||
             ( cur[0]->platform_id == TT_PLATFORM_APPLE_UNICODE &&
               cur[0]->encoding_id == TT_APPLE_ID_నిండిన_UNICODE  &&
               FT_Get_CMap_Format( cur[0] ) == 13               ) )
        {
          face->charmap = cur[0];
          తిరిగి FT_Err_Ok;
        }
      }
    }

    /* We do not have any UCS-4 charmap.                */
    /* Do the loop again and search for UCS-2 charmaps. */
    cur = మొదటి + face->అంకె_charmaps;

    కొరకు ( ; --cur >= మొదటి; )
    {
      ఒకవేళ ( cur[0]->encoding == FT_ENCODING_UNICODE )
      {
        FT_Long  format = FT_Get_CMap_Format( cur[0] );


        ఒకవేళ ( format != 14 && format != 15 )
        {
          face->charmap = cur[0];
          తిరిగి FT_Err_Ok;
        }
      }
    }

    తిరిగి FT_THROW( Invalid_CharMap_Handle );
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
  స్థిర FT_CharMap
  కనుగొను_variant_selector_charmap( FT_Face  face )
  {
    FT_CharMap*  మొదటి;
    FT_CharMap*  చివర;
    FT_CharMap*  cur;


    /* caller should have already checked that `face' is valid */
    FT_ASSERT( face );

    మొదటి = face->charmaps;

    ఒకవేళ ( !మొదటి )
      తిరిగి ఖాళీ;

    చివర = మొదటి + face->అంకె_charmaps;  /* points after the last one */

    కొరకు ( cur = మొదటి; cur < చివర; cur++ )
    {
      ఒకవేళ ( cur[0]->platform_id == TT_PLATFORM_APPLE_UNICODE    &&
           cur[0]->encoding_id == TT_APPLE_ID_VARIANT_SELECTOR )
      {
        FT_Long  format = FT_Get_CMap_Format( cur[0] );


        ఒకవేళ ( format == 14 || format == 15 )
          తిరిగి cur[0];
      }
    }

    తిరిగి ఖాళీ;
  }


  /**************************************************************************
   *
   * @Function:
   *   open_face
   *
   * @Description:
   *   This function does some work for FT_Open_Face().
   */
  స్థిర FT_Error
  open_face( FT_Driver      driver,
             FT_ప్రవాహం      *astream,
             FT_Bool        *anexternal_ప్రవాహం,
             FT_Long        face_సూచీ,
             FT_Int         అంకె_params,
             FT_Parameter*  params,
             FT_Face       *aface )
  {
    FT_జ్ఞాపకం         జ్ఞాపకం;
    FT_Driver_Class   clazz;
    FT_Face           face     = ఖాళీ;
    FT_Face_అంతర్గత  అంతర్గత = ఖాళీ;

    FT_Error          error, error2;


    clazz  = driver->clazz;
    జ్ఞాపకం = driver->మూలం.జ్ఞాపకం;

    /* allocate the face object and perform basic initialization */
    ఒకవేళ ( FT_ALLOC( face, clazz->face_object_కొలత ) )
      వెళ్ళు Fail;

    face->driver = driver;
    face->జ్ఞాపకం = జ్ఞాపకం;
    face->ప్రవాహం = *astream;

    /* set the FT_FACE_FLAG_EXTERNAL_STREAM bit for FT_Done_Face */
    ఒకవేళ ( *anexternal_ప్రవాహం )
      face->face_flags |= FT_FACE_FLAG_EXTERNAL_ప్రవాహం;

    ఒకవేళ ( FT_NEW( అంతర్గత ) )
      వెళ్ళు Fail;

    face->అంతర్గత = అంతర్గత;

#నిర్వచితమైతే FT_CONFIG_OPTION_INCREMENTAL
    {
      పూర్ణాంకం  i;


      face->అంతర్గత->incremental_interface = ఖాళీ;
      కొరకు ( i = 0; i < అంకె_params && !face->అంతర్గత->incremental_interface;
            i++ )
        ఒకవేళ ( params[i].ట్యాగ్ == FT_PARAM_ట్యాగ్_INCREMENTAL )
          face->అంతర్గత->incremental_interface =
            (FT_Incremental_Interface)params[i].దత్తాంశం;
    }
#అయితే_ముగింపు

    face->అంతర్గత->random_విత్తనం = -1;

    ఒకవేళ ( clazz->init_face )
      error = clazz->init_face( *astream,
                                face,
                                (FT_Int)face_సూచీ,
                                అంకె_params,
                                params );
    /* Stream may have been changed. */
    *astream = face->ప్రవాహం;
    *anexternal_ప్రవాహం =
      ( face->face_flags & FT_FACE_FLAG_EXTERNAL_ప్రవాహం ) != 0;
    ఒకవేళ ( error )
      వెళ్ళు Fail;

    /* select Unicode charmap by default */
    error2 = కనుగొను_unicode_charmap( face );

    /* if no Unicode charmap can be found, FT_Err_Invalid_CharMap_Handle */
    /* is returned.                                                      */

    /* no error should happen, but we want to play safe */
    ఒకవేళ ( error2 && FT_ERR_NEQ( error2, Invalid_CharMap_Handle ) )
    {
      error = error2;
      వెళ్ళు Fail;
    }

    *aface = face;

  Fail:
    ఒకవేళ ( error )
    {
      destroy_charmaps( face, జ్ఞాపకం );
      ఒకవేళ ( clazz->పూర్తి_face )
        clazz->పూర్తి_face( face );
      FT_FREE( అంతర్గత );
      FT_FREE( face );
      *aface = ఖాళీ;
    }

    తిరిగి error;
  }


  /* there's a Mac-specific extended implementation of FT_New_Face() */
  /* in src/base/ftmac.c                                             */

#నిర్వచితం_కాకపోతే FT_MACINTOSH

  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_New_Face( FT_Library   library,
               స్థిరరాశి అక్షరం*  pathname,
               FT_Long      face_సూచీ,
               FT_Face     *aface )
  {
    FT_Open_వాదనలు  వాదనలు;


    /* test for valid `library' and `aface' delayed to `FT_Open_Face' */
    ఒకవేళ ( !pathname )
      తిరిగి FT_THROW( Invalid_వాదం );

    వాదనలు.flags    = FT_OPEN_PATHNAME;
    వాదనలు.pathname = (అక్షరం*)pathname;
    వాదనలు.ప్రవాహం   = ఖాళీ;

    తిరిగి ft_open_face_అంతర్గత( library, &వాదనలు, face_సూచీ, aface, 1 );
  }

#అయితే_ముగింపు


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_New_జ్ఞాపకం_Face( FT_Library      library,
                      స్థిరరాశి FT_Byte*  file_పునాది,
                      FT_Long         file_కొలత,
                      FT_Long         face_సూచీ,
                      FT_Face        *aface )
  {
    FT_Open_వాదనలు  వాదనలు;


    /* test for valid `library' and `face' delayed to `FT_Open_Face' */
    ఒకవేళ ( !file_పునాది )
      తిరిగి FT_THROW( Invalid_వాదం );

    వాదనలు.flags       = FT_OPEN_జ్ఞాపకం;
    వాదనలు.జ్ఞాపకం_పునాది = file_పునాది;
    వాదనలు.జ్ఞాపకం_కొలత = file_కొలత;
    వాదనలు.ప్రవాహం      = ఖాళీ;

    తిరిగి ft_open_face_అంతర్గత( library, &వాదనలు, face_సూచీ, aface, 1 );
  }


#నిర్వచితమైతే FT_CONFIG_OPTION_MAC_FONTS

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
  స్థిర శూన్య
  జ్ఞాపకం_ప్రవాహం_close( FT_ప్రవాహం  ప్రవాహం )
  {
    FT_జ్ఞాపకం  జ్ఞాపకం = (FT_జ్ఞాపకం)ప్రవాహం->descriptor.సూచిక;


    FT_FREE( ప్రవాహం->పునాది );
    ప్రవాహం->కొలత  = 0;
    ప్రవాహం->close = ఖాళీ;
    FT_FREE( ప్రవాహం );
  }


  /* Create a new memory stream from a buffer and a size. */
  /* From `ftmac.c'.                                      */
  స్థిర FT_Error
  new_జ్ఞాపకం_ప్రవాహం( FT_Library           library,
                     FT_Byte*             పునాది,
                     FT_ULong             కొలత,
                     FT_ప్రవాహం_Close_Func  close,
                     FT_ప్రవాహం           *astream )
  {
    FT_Error   error;
    FT_జ్ఞాపకం  జ్ఞాపకం;
    FT_ప్రవాహం  ప్రవాహం = ఖాళీ;


    ఒకవేళ ( !library )
      తిరిగి FT_THROW( Invalid_Library_Handle );

    ఒకవేళ ( !పునాది )
      తిరిగి FT_THROW( Invalid_వాదం );

    *astream = ఖాళీ;
    జ్ఞాపకం   = library->జ్ఞాపకం;
    ఒకవేళ ( FT_NEW( ప్రవాహం ) )
      వెళ్ళు Exit;

    FT_ప్రవాహం_Open_జ్ఞాపకం( ప్రవాహం, పునాది, కొలత );

    ప్రవాహం->descriptor.సూచిక = జ్ఞాపకం;
    ప్రవాహం->close              = close;

    *astream = ప్రవాహం;

  Exit:
    తిరిగి error;
  }


  /* Create a new FT_Face given a buffer and a driver name. */
  /* From `ftmac.c'.                                        */
  FT_LOCAL_DEF( FT_Error )
  open_face_from_బఫరు( FT_Library   library,
                         FT_Byte*     పునాది,
                         FT_ULong     కొలత,
                         FT_Long      face_సూచీ,
                         స్థిరరాశి అక్షరం*  driver_name,
                         FT_Face     *aface )
  {
    FT_Open_వాదనలు  వాదనలు;
    FT_Error      error;
    FT_జ్ఞాపకం     జ్ఞాపకం = library->జ్ఞాపకం;


    వాదనలు.driver = ఖాళీ;
    వాదనలు.flags  = 0;

    ఒకవేళ ( driver_name )
    {
      వాదనలు.driver = FT_Get_Module( library, driver_name );
      ఒకవేళ ( !వాదనలు.driver )
      {
        FT_FREE( పునాది );
        తిరిగి FT_THROW( Missing_Module );
      }

      వాదనలు.flags = వాదనలు.flags | FT_OPEN_DRIVER;
    }

    /* `memory_stream_close` also frees the stream object. */
    error = new_జ్ఞాపకం_ప్రవాహం( library,
                               పునాది,
                               కొలత,
                               జ్ఞాపకం_ప్రవాహం_close,
                               &వాదనలు.ప్రవాహం );
    ఒకవేళ ( error )
    {
      FT_FREE( పునాది );
      తిరిగి error;
    }

    వాదనలు.flags |= FT_OPEN_ప్రవాహం;

#నిర్వచితమైతే FT_MACINTOSH
    /* At this point, the face index has served its purpose;  */
    /* whoever calls this function has already used it to     */
    /* locate the correct font data.  We should not propagate */
    /* this index to FT_Open_Face() (unless it is negative).  */

    ఒకవేళ ( face_సూచీ > 0 )
      face_సూచీ &= 0x7FFF0000L; /* retain GX data */
#అయితే_ముగింపు

    తిరిగి ft_open_face_అంతర్గత( library, &వాదనలు, face_సూచీ, aface, 0 );
  }


  /* Look up `TYP1' or `CID ' table from sfnt table directory.       */
  /* `offset' and `length' must exclude the binary header in tables. */

  /* Type 1 and CID-keyed font drivers should recognize sfnt-wrapped */
  /* format too.  Here, since we can't expect that the TrueType font */
  /* driver is loaded unconditionally, we must parse the font by     */
  /* ourselves.  We are only interested in the name of the table and */
  /* the offset.                                                     */

  స్థిర FT_Error
  ft_lookup_PS_లోపల_sfnt_ప్రవాహం( FT_ప్రవాహం  ప్రవాహం,
                               FT_Long    face_సూచీ,
                               FT_ULong*  స్థానభ్రంశం,
                               FT_ULong*  నిడివి,
                               FT_Bool*   is_sfnt_cid )
  {
    FT_Error   error;
    FT_UShort  అంకె_Tables;
    FT_Long    pstable_సూచీ;
    FT_ULong   ట్యాగ్;
    పూర్ణాంకం        i;


    *స్థానభ్రంశం = 0;
    *నిడివి = 0;
    *is_sfnt_cid = FALSE;

    /* TODO: support for sfnt-wrapped PS/CID in TTC format */

    /* version check for 'typ1' (should be ignored?) */
    ఒకవేళ ( FT_READ_ULONG( ట్యాగ్ ) )
      తిరిగి error;
    ఒకవేళ ( ట్యాగ్ != TTAG_typ1 )
      తిరిగి FT_THROW( Unknown_File_Format );

    ఒకవేళ ( FT_READ_USHORT( అంకె_Tables ) )
      తిరిగి error;
    ఒకవేళ ( FT_ప్రవాహం_SKIP( 2 * 3 ) ) /* skip binary search header */
      తిరిగి error;

    pstable_సూచీ = -1;
    *is_sfnt_cid  = FALSE;

    కొరకు ( i = 0; i < అంకె_Tables; i++ )
    {
      ఒకవేళ ( FT_READ_ULONG( ట్యాగ్ )     || FT_ప్రవాహం_SKIP( 4 )      ||
           FT_READ_ULONG( *స్థానభ్రంశం ) || FT_READ_ULONG( *నిడివి ) )
        తిరిగి error;

      ఒకవేళ ( ట్యాగ్ == TTAG_CID )
      {
        pstable_సూచీ++;
        *స్థానభ్రంశం += 22;
        *నిడివి -= 22;
        *is_sfnt_cid = TRUE;
        ఒకవేళ ( face_సూచీ < 0 )
          తిరిగి FT_Err_Ok;
      }
      లేకపోతే ఒకవేళ ( ట్యాగ్ == TTAG_TYP1 )
      {
        pstable_సూచీ++;
        *స్థానభ్రంశం += 24;
        *నిడివి -= 24;
        *is_sfnt_cid = FALSE;
        ఒకవేళ ( face_సూచీ < 0 )
          తిరిగి FT_Err_Ok;
      }
      ఒకవేళ ( face_సూచీ >= 0 && pstable_సూచీ == face_సూచీ )
        తిరిగి FT_Err_Ok;
    }

    తిరిగి FT_THROW( పట్టిక_Missing );
  }


  FT_LOCAL_DEF( FT_Error )
  open_face_PS_from_sfnt_ప్రవాహం( FT_Library     library,
                                 FT_ప్రవాహం      ప్రవాహం,
                                 FT_Long        face_సూచీ,
                                 FT_Int         అంకె_params,
                                 FT_Parameter  *params,
                                 FT_Face       *aface )
  {
    FT_Error   error;
    FT_జ్ఞాపకం  జ్ఞాపకం = library->జ్ఞాపకం;
    FT_ULong   స్థానభ్రంశం, నిడివి;
    FT_ULong   pos;
    FT_Bool    is_sfnt_cid;
    FT_Byte*   sfnt_ps = ఖాళీ;

    FT_వాడని( అంకె_params );
    FT_వాడని( params );


    /* ignore GX stuff */
    ఒకవేళ ( face_సూచీ > 0 )
      face_సూచీ &= 0xFFFFL;

    pos = FT_ప్రవాహం_POS();

    error = ft_lookup_PS_లోపల_sfnt_ప్రవాహం( ప్రవాహం,
                                         face_సూచీ,
                                         &స్థానభ్రంశం,
                                         &నిడివి,
                                         &is_sfnt_cid );
    ఒకవేళ ( error )
      వెళ్ళు Exit;

    ఒకవేళ ( స్థానభ్రంశం > ప్రవాహం->కొలత )
    {
      FT_TRACE2(( "open_face_PS_from_sfnt_stream: invalid table offset\n" ));
      error = FT_THROW( Invalid_పట్టిక );
      వెళ్ళు Exit;
    }
    లేకపోతే ఒకవేళ ( నిడివి > ప్రవాహం->కొలత - స్థానభ్రంశం )
    {
      FT_TRACE2(( "open_face_PS_from_sfnt_stream: invalid table length\n" ));
      error = FT_THROW( Invalid_పట్టిక );
      వెళ్ళు Exit;
    }

    error = FT_ప్రవాహం_Seek( ప్రవాహం, pos + స్థానభ్రంశం );
    ఒకవేళ ( error )
      వెళ్ళు Exit;

    ఒకవేళ ( FT_QALLOC( sfnt_ps, (FT_Long)నిడివి ) )
      వెళ్ళు Exit;

    error = FT_ప్రవాహం_Read( ప్రవాహం, (FT_Byte *)sfnt_ps, నిడివి );
    ఒకవేళ ( error )
    {
      FT_FREE( sfnt_ps );
      వెళ్ళు Exit;
    }

    error = open_face_from_బఫరు( library,
                                   sfnt_ps,
                                   నిడివి,
                                   FT_MIN( face_సూచీ, 0 ),
                                   is_sfnt_cid ? "t1cid" : "type1",
                                   aface );
  Exit:
    {
      FT_Error  error1;


      ఒకవేళ ( FT_ERR_EQ( error, Unknown_File_Format ) )
      {
        error1 = FT_ప్రవాహం_Seek( ప్రవాహం, pos );
        ఒకవేళ ( error1 )
          తిరిగి error1;
      }

      తిరిగి error;
    }
  }


#నిర్వచితం_కాకపోతే FT_MACINTOSH

  /* The resource header says we've got resource_cnt `POST' (type1) */
  /* resources in this file.  They all need to be coalesced into    */
  /* one lump which gets passed on to the type1 driver.             */
  /* Here can be only one PostScript font in a file so face_index   */
  /* must be 0 (or -1).                                             */
  /*                                                                */
  స్థిర FT_Error
  Mac_Read_POST_Resource( FT_Library  library,
                          FT_ప్రవాహం   ప్రవాహం,
                          FT_Long    *offsets,
                          FT_Long     resource_cnt,
                          FT_Long     face_సూచీ,
                          FT_Face    *aface )
  {
    FT_Error   error  = FT_ERR( Cannot_Open_Resource );
    FT_జ్ఞాపకం  జ్ఞాపకం = library->జ్ఞాపకం;

    FT_Byte*   pfb_దత్తాంశం = ఖాళీ;
    పూర్ణాంకం        i, type, flags;
    FT_ULong   len;
    FT_ULong   pfb_len, pfb_pos, pfb_lenpos;
    FT_ULong   rlen, తాత్కాలిక;


    ఒకవేళ ( face_సూచీ == -1 )
      face_సూచీ = 0;
    ఒకవేళ ( face_సూచీ != 0 )
      తిరిగి error;

    /* Find the length of all the POST resources, concatenated.  Assume */
    /* worst case (each resource in its own section).                   */
    pfb_len = 0;
    కొరకు ( i = 0; i < resource_cnt; i++ )
    {
      error = FT_ప్రవాహం_Seek( ప్రవాహం, (FT_ULong)offsets[i] );
      ఒకవేళ ( error )
        వెళ్ళు Exit;
      ఒకవేళ ( FT_READ_ULONG( తాత్కాలిక ) )  /* actually LONG */
        వెళ్ళు Exit;

      /* FT2 allocator takes signed long buffer length,
       * too large value causing overflow should be checked
       */
      FT_TRACE4(( "                 POST fragment #%d: length=0x%08lx"
                  " total pfb_len=0x%08lx\n",
                  i, తాత్కాలిక, pfb_len + తాత్కాలిక + 6 ));

      ఒకవేళ ( FT_MAC_RFORK_MAX_LEN < తాత్కాలిక               ||
           FT_MAC_RFORK_MAX_LEN - తాత్కాలిక < pfb_len + 6 )
      {
        FT_TRACE2(( "             MacOS resource length cannot exceed"
                    " 0x%08lx\n",
                    FT_MAC_RFORK_MAX_LEN ));

        error = FT_THROW( Invalid_స్థానభ్రంశం );
        వెళ్ళు Exit;
      }

      pfb_len += తాత్కాలిక + 6;
    }

    FT_TRACE2(( "             total buffer size to concatenate"
                " %ld POST fragments: 0x%08lx\n",
                 resource_cnt, pfb_len + 2 ));

    ఒకవేళ ( pfb_len + 2 < 6 )
    {
      FT_TRACE2(( "             too long fragment length makes"
                  " pfb_len confused: pfb_len=0x%08lx\n",
                  pfb_len ));

      error = FT_THROW( Array_Too_Large );
      వెళ్ళు Exit;
    }

    ఒకవేళ ( FT_QALLOC( pfb_దత్తాంశం, (FT_Long)pfb_len + 2 ) )
      వెళ్ళు Exit;

    pfb_దత్తాంశం[0] = 0x80;
    pfb_దత్తాంశం[1] = 1;            /* Ascii section */
    pfb_దత్తాంశం[2] = 0;            /* 4-byte length, fill in later */
    pfb_దత్తాంశం[3] = 0;
    pfb_దత్తాంశం[4] = 0;
    pfb_దత్తాంశం[5] = 0;
    pfb_pos     = 6;
    pfb_lenpos  = 2;

    len  = 0;
    type = 1;

    కొరకు ( i = 0; i < resource_cnt; i++ )
    {
      error = FT_ప్రవాహం_Seek( ప్రవాహం, (FT_ULong)offsets[i] );
      ఒకవేళ ( error )
        వెళ్ళు Exit2;
      ఒకవేళ ( FT_READ_ULONG( rlen ) )
        వెళ్ళు Exit2;

      /* FT2 allocator takes signed long buffer length,
       * too large fragment length causing overflow should be checked
       */
      ఒకవేళ ( 0x7FFFFFFFUL < rlen )
      {
        error = FT_THROW( Invalid_స్థానభ్రంశం );
        వెళ్ళు Exit2;
      }

      ఒకవేళ ( FT_READ_USHORT( flags ) )
        వెళ్ళు Exit2;

      FT_TRACE3(( "POST fragment[%d]:"
                  " offsets=0x%08lx, rlen=0x%08lx, flags=0x%04x\n",
                  i, offsets[i], rlen, flags ));

      error = FT_ERR( Array_Too_Large );

      /* postpone the check of `rlen longer than buffer' */
      /* until `FT_Stream_Read'                          */

      ఒకవేళ ( ( flags >> 8 ) == 0 )        /* Comment, should not be loaded */
      {
        FT_TRACE3(( "    Skip POST fragment #%d because it is a comment\n",
                    i ));
        కొనసాగు;
      }

      /* the flags are part of the resource, so rlen >= 2,  */
      /* but some fonts declare rlen = 0 for empty fragment */
      ఒకవేళ ( rlen > 2 )
        rlen -= 2;
      లేకపోతే
        rlen = 0;

      ఒకవేళ ( ( flags >> 8 ) == type )
        len += rlen;
      లేకపోతే
      {
        FT_TRACE3(( "    Write POST fragment #%d header (4-byte) to buffer"
                    " %p + 0x%08lx\n",
                    i, (శూన్య*)pfb_దత్తాంశం, pfb_lenpos ));

        ఒకవేళ ( pfb_lenpos + 3 > pfb_len + 2 )
          వెళ్ళు Exit2;

        pfb_దత్తాంశం[pfb_lenpos    ] = (FT_Byte)( len );
        pfb_దత్తాంశం[pfb_lenpos + 1] = (FT_Byte)( len >> 8 );
        pfb_దత్తాంశం[pfb_lenpos + 2] = (FT_Byte)( len >> 16 );
        pfb_దత్తాంశం[pfb_lenpos + 3] = (FT_Byte)( len >> 24 );

        ఒకవేళ ( ( flags >> 8 ) == 5 )      /* End of font mark */
          ఆపు;

        FT_TRACE3(( "    Write POST fragment #%d header (6-byte) to buffer"
                    " %p + 0x%08lx\n",
                    i, (శూన్య*)pfb_దత్తాంశం, pfb_pos ));

        ఒకవేళ ( pfb_pos + 6 > pfb_len + 2 )
          వెళ్ళు Exit2;

        pfb_దత్తాంశం[pfb_pos++] = 0x80;

        type = flags >> 8;
        len  = rlen;

        pfb_దత్తాంశం[pfb_pos++] = (FT_Byte)type;
        pfb_lenpos          = pfb_pos;
        pfb_దత్తాంశం[pfb_pos++] = 0;        /* 4-byte length, fill in later */
        pfb_దత్తాంశం[pfb_pos++] = 0;
        pfb_దత్తాంశం[pfb_pos++] = 0;
        pfb_దత్తాంశం[pfb_pos++] = 0;
      }

      ఒకవేళ ( pfb_pos > pfb_len || pfb_pos + rlen > pfb_len )
        వెళ్ళు Exit2;

      FT_TRACE3(( "    Load POST fragment #%d (%lu byte) to buffer"
                  " %p + 0x%08lx\n",
                  i, rlen, (శూన్య*)pfb_దత్తాంశం, pfb_pos ));

      error = FT_ప్రవాహం_Read( ప్రవాహం, (FT_Byte *)pfb_దత్తాంశం + pfb_pos, rlen );
      ఒకవేళ ( error )
        వెళ్ళు Exit2;

      pfb_pos += rlen;
    }

    error = FT_ERR( Array_Too_Large );

    ఒకవేళ ( pfb_pos + 2 > pfb_len + 2 )
      వెళ్ళు Exit2;
    pfb_దత్తాంశం[pfb_pos++] = 0x80;
    pfb_దత్తాంశం[pfb_pos++] = 3;

    ఒకవేళ ( pfb_lenpos + 3 > pfb_len + 2 )
      వెళ్ళు Exit2;
    pfb_దత్తాంశం[pfb_lenpos    ] = (FT_Byte)( len );
    pfb_దత్తాంశం[pfb_lenpos + 1] = (FT_Byte)( len >> 8 );
    pfb_దత్తాంశం[pfb_lenpos + 2] = (FT_Byte)( len >> 16 );
    pfb_దత్తాంశం[pfb_lenpos + 3] = (FT_Byte)( len >> 24 );

    తిరిగి open_face_from_బఫరు( library,
                                  pfb_దత్తాంశం,
                                  pfb_pos,
                                  face_సూచీ,
                                  "type1",
                                  aface );

  Exit2:
    ఒకవేళ ( FT_ERR_EQ( error, Array_Too_Large ) )
      FT_TRACE2(( "  Abort due to too-short buffer to store"
                  " all POST fragments\n" ));
    లేకపోతే ఒకవేళ ( FT_ERR_EQ( error, Invalid_స్థానభ్రంశం ) )
      FT_TRACE2(( "  Abort due to invalid offset in a POST fragment\n" ));

    ఒకవేళ ( error )
      error = FT_ERR( Cannot_Open_Resource );
    FT_FREE( pfb_దత్తాంశం );

  Exit:
    తిరిగి error;
  }


  /* The resource header says we've got resource_cnt `sfnt'      */
  /* (TrueType/OpenType) resources in this file.  Look through   */
  /* them for the one indicated by face_index, load it into mem, */
  /* pass it on to the truetype driver, and return it.           */
  /*                                                             */
  స్థిర FT_Error
  Mac_Read_sfnt_Resource( FT_Library  library,
                          FT_ప్రవాహం   ప్రవాహం,
                          FT_Long    *offsets,
                          FT_Long     resource_cnt,
                          FT_Long     face_సూచీ,
                          FT_Face    *aface )
  {
    FT_జ్ఞాపకం  జ్ఞాపకం = library->జ్ఞాపకం;
    FT_Byte*   sfnt_దత్తాంశం = ఖాళీ;
    FT_Error   error;
    FT_ULong   flag_స్థానభ్రంశం;
    FT_ULong   rlen;
    పూర్ణాంకం        is_cff;
    FT_Long    face_సూచీ_లోపల_resource = 0;


    ఒకవేళ ( face_సూచీ < 0 )
      face_సూచీ = -face_సూచీ - 1;
    ఒకవేళ ( face_సూచీ >= resource_cnt )
      తిరిగి FT_THROW( Cannot_Open_Resource );

    flag_స్థానభ్రంశం = (FT_ULong)offsets[face_సూచీ];
    error = FT_ప్రవాహం_Seek( ప్రవాహం, flag_స్థానభ్రంశం );
    ఒకవేళ ( error )
      వెళ్ళు Exit;

    ఒకవేళ ( FT_READ_ULONG( rlen ) )
      వెళ్ళు Exit;
    ఒకవేళ ( !rlen )
      తిరిగి FT_THROW( Cannot_Open_Resource );
    ఒకవేళ ( rlen > FT_MAC_RFORK_MAX_LEN )
      తిరిగి FT_THROW( Invalid_స్థానభ్రంశం );

    error = open_face_PS_from_sfnt_ప్రవాహం( library,
                                           ప్రవాహం,
                                           face_సూచీ,
                                           0, ఖాళీ,
                                           aface );
    ఒకవేళ ( !error )
      వెళ్ళు Exit;

    /* rewind sfnt stream before open_face_PS_from_sfnt_stream() */
    error = FT_ప్రవాహం_Seek( ప్రవాహం, flag_స్థానభ్రంశం + 4 );
    ఒకవేళ ( error )
      వెళ్ళు Exit;

    ఒకవేళ ( FT_QALLOC( sfnt_దత్తాంశం, rlen ) )
      తిరిగి error;
    error = FT_ప్రవాహం_Read( ప్రవాహం, (FT_Byte *)sfnt_దత్తాంశం, rlen );
    ఒకవేళ ( error )
    {
      FT_FREE( sfnt_దత్తాంశం );
      వెళ్ళు Exit;
    }

    is_cff = rlen > 4 && !ft_memcmp( sfnt_దత్తాంశం, "OTTO", 4 );
    error = open_face_from_బఫరు( library,
                                   sfnt_దత్తాంశం,
                                   rlen,
                                   face_సూచీ_లోపల_resource,
                                   is_cff ? "cff" : "truetype",
                                   aface );

  Exit:
    తిరిగి error;
  }


  /* Check for a valid resource fork header, or a valid dfont    */
  /* header.  In a resource fork the first 16 bytes are repeated */
  /* at the location specified by bytes 4-7.  In a dfont bytes   */
  /* 4-7 point to 16 bytes of zeroes instead.                    */
  /*                                                             */
  స్థిర FT_Error
  IsMacResource( FT_Library  library,
                 FT_ప్రవాహం   ప్రవాహం,
                 FT_Long     resource_స్థానభ్రంశం,
                 FT_Long     face_సూచీ,
                 FT_Face    *aface )
  {
    FT_జ్ఞాపకం  జ్ఞాపకం = library->జ్ఞాపకం;
    FT_Error   error;
    FT_Long    map_స్థానభ్రంశం, rdata_pos;
    FT_Long    *దత్తాంశం_offsets;
    FT_Long    లెక్క;


    error = FT_Raccess_Get_శీర్షిక_Info( library, ప్రవాహం, resource_స్థానభ్రంశం,
                                       &map_స్థానభ్రంశం, &rdata_pos );
    ఒకవేళ ( error )
      తిరిగి error;

    /* POST resources must be sorted to concatenate properly */
    error = FT_Raccess_Get_దత్తాంశం_Offsets( library, ప్రవాహం,
                                        map_స్థానభ్రంశం, rdata_pos,
                                        TTAG_POST, TRUE,
                                        &దత్తాంశం_offsets, &లెక్క );
    ఒకవేళ ( !error )
    {
      error = Mac_Read_POST_Resource( library, ప్రవాహం, దత్తాంశం_offsets, లెక్క,
                                      face_సూచీ, aface );
      FT_FREE( దత్తాంశం_offsets );
      /* POST exists in an LWFN providing a single face */
      ఒకవేళ ( !error )
        (*aface)->అంకె_faces = 1;
      తిరిగి error;
    }

    /* sfnt resources should not be sorted to preserve the face order by
       QuickDraw API */
    error = FT_Raccess_Get_దత్తాంశం_Offsets( library, ప్రవాహం,
                                        map_స్థానభ్రంశం, rdata_pos,
                                        TTAG_sfnt, FALSE,
                                        &దత్తాంశం_offsets, &లెక్క );
    ఒకవేళ ( !error )
    {
      FT_Long  face_సూచీ_అంతర్గత = face_సూచీ % లెక్క;


      error = Mac_Read_sfnt_Resource( library, ప్రవాహం, దత్తాంశం_offsets, లెక్క,
                                      face_సూచీ_అంతర్గత, aface );
      FT_FREE( దత్తాంశం_offsets );
      ఒకవేళ ( !error )
      {
        (*aface)->అంకె_faces  = లెక్క;
        (*aface)->face_సూచీ = face_సూచీ_అంతర్గత;
      }
    }

    తిరిగి error;
  }


  /* Check for a valid macbinary header, and if we find one   */
  /* check that the (flattened) resource fork in it is valid. */
  /*                                                          */
  స్థిర FT_Error
  Is_Mac_ద్విమాన( FT_Library  library,
               FT_ప్రవాహం   ప్రవాహం,
               FT_Long     face_సూచీ,
               FT_Face    *aface )
  {
    గుర్తులేని అక్షరం  శీర్షిక[128];
    FT_Error       error;
    FT_Long        dlen, స్థానభ్రంశం;


    ఒకవేళ ( !ప్రవాహం )
      తిరిగి FT_THROW( Invalid_ప్రవాహం_Operation );

    error = FT_ప్రవాహం_Seek( ప్రవాహం, 0 );
    ఒకవేళ ( error )
      వెళ్ళు Exit;

    error = FT_ప్రవాహం_Read( ప్రవాహం, (FT_Byte*)శీర్షిక, 128 );
    ఒకవేళ ( error )
      వెళ్ళు Exit;

    ఒకవేళ (            శీర్షిక[ 0] !=   0 ||
                    శీర్షిక[74] !=   0 ||
                    శీర్షిక[82] !=   0 ||
                    శీర్షిక[ 1] ==   0 ||
                    శీర్షిక[ 1] >   33 ||
                    శీర్షిక[63] !=   0 ||
         శీర్షిక[2 + శీర్షిక[1]] !=   0 ||
                  శీర్షిక[0x53] > 0x7F )
      తిరిగి FT_THROW( Unknown_File_Format );

    dlen = ( శీర్షిక[0x53] << 24 ) |
           ( శీర్షిక[0x54] << 16 ) |
           ( శీర్షిక[0x55] <<  8 ) |
             శీర్షిక[0x56];
#ఒకవేళ 0
    rlen = ( శీర్షిక[0x57] << 24 ) |
           ( శీర్షిక[0x58] << 16 ) |
           ( శీర్షిక[0x59] <<  8 ) |
             శీర్షిక[0x5A];
#అయితే_ముగింపు /* 0 */
    స్థానభ్రంశం = 128 + ( ( dlen + 127 ) & ~127 );

    తిరిగి IsMacResource( library, ప్రవాహం, స్థానభ్రంశం, face_సూచీ, aface );

  Exit:
    తిరిగి error;
  }


  స్థిర FT_Error
  ఎక్కించు_face_లోపల_embedded_rfork( FT_Library           library,
                               FT_ప్రవాహం            ప్రవాహం,
                               FT_Long              face_సూచీ,
                               FT_Face             *aface,
                               స్థిరరాశి FT_Open_వాదనలు  *వాదనలు )
  {

#నిర్వచనం_రద్దు  FT_COMPONENT
#నిర్వచించు FT_COMPONENT  raccess

    FT_జ్ఞాపకం  జ్ఞాపకం = library->జ్ఞాపకం;
    FT_Error   error  = FT_ERR( Unknown_File_Format );
    FT_UInt    i;

    అక్షరం*      file_పేర్లు[FT_RACCESS_N_నియమాలు];
    FT_Long    offsets[FT_RACCESS_N_నియమాలు];
    FT_Error   errors[FT_RACCESS_N_నియమాలు];
    FT_Bool    is_darwin_vfs, vfs_rfork_has_no_font = FALSE; /* not tested */

    FT_Open_వాదనలు  args2;
    FT_ప్రవాహం     stream2 = ఖాళీ;


    FT_Raccess_Guess( library, ప్రవాహం,
                      వాదనలు->pathname, file_పేర్లు, offsets, errors );

    కొరకు ( i = 0; i < FT_RACCESS_N_నియమాలు; i++ )
    {
      is_darwin_vfs = ft_raccess_నియమం_by_darwin_vfs( library, i );
      ఒకవేళ ( is_darwin_vfs && vfs_rfork_has_no_font )
      {
        FT_TRACE3(( "Skip rule %u: darwin vfs resource fork"
                    " is already checked and"
                    " no font is found\n",
                    i ));
        కొనసాగు;
      }

      ఒకవేళ ( errors[i] )
      {
        FT_TRACE3(( "Error 0x%x has occurred in rule %u\n",
                    errors[i], i ));
        కొనసాగు;
      }

      args2.flags    = FT_OPEN_PATHNAME;
      args2.pathname = file_పేర్లు[i] ? file_పేర్లు[i] : వాదనలు->pathname;

      FT_TRACE3(( "Try rule %u: %s (offset=%ld) ...",
                  i, args2.pathname, offsets[i] ));

      error = FT_ప్రవాహం_New( library, &args2, &stream2 );
      ఒకవేళ ( is_darwin_vfs && FT_ERR_EQ( error, Cannot_Open_ప్రవాహం ) )
        vfs_rfork_has_no_font = TRUE;

      ఒకవేళ ( error )
      {
        FT_TRACE3(( "failed\n" ));
        కొనసాగు;
      }

      error = IsMacResource( library, stream2, offsets[i],
                             face_సూచీ, aface );
      FT_ప్రవాహం_Free( stream2, 0 );

      FT_TRACE3(( "%s\n", error ? "failed": "successful" ));

      ఒకవేళ ( !error )
          ఆపు;
      లేకపోతే ఒకవేళ ( is_darwin_vfs )
          vfs_rfork_has_no_font = TRUE;
    }

    కొరకు (i = 0; i < FT_RACCESS_N_నియమాలు; i++)
    {
      ఒకవేళ ( file_పేర్లు[i] )
        FT_FREE( file_పేర్లు[i] );
    }

    /* Caller (load_mac_face) requires FT_Err_Unknown_File_Format. */
    ఒకవేళ ( error )
      error = FT_ERR( Unknown_File_Format );

    తిరిగి error;

#నిర్వచనం_రద్దు  FT_COMPONENT
#నిర్వచించు FT_COMPONENT  objs

  }


  /* Check for some macintosh formats without Carbon framework.    */
  /* Is this a macbinary file?  If so look at the resource fork.   */
  /* Is this a mac dfont file?                                     */
  /* Is this an old style resource fork? (in data)                 */
  /* Else call load_face_in_embedded_rfork to try extra rules      */
  /* (defined in `ftrfork.c').                                     */
  /*                                                               */
  స్థిర FT_Error
  ఎక్కించు_mac_face( FT_Library           library,
                 FT_ప్రవాహం            ప్రవాహం,
                 FT_Long              face_సూచీ,
                 FT_Face             *aface,
                 స్థిరరాశి FT_Open_వాదనలు  *వాదనలు )
  {
    FT_Error error;
    FT_వాడని( వాదనలు );


    error = Is_Mac_ద్విమాన( library, ప్రవాహం, face_సూచీ, aface );
    ఒకవేళ ( FT_ERR_EQ( error, Unknown_File_Format ) )
    {

#నిర్వచనం_రద్దు  FT_COMPONENT
#నిర్వచించు FT_COMPONENT  raccess

#నిర్వచితమైతే FT_DEBUG_LEVEL_TRACE
      FT_TRACE3(( "Try as dfont: " ));
      ఒకవేళ ( !( వాదనలు->flags & FT_OPEN_జ్ఞాపకం ) )
        FT_TRACE3(( "%s ...", వాదనలు->pathname ));
#అయితే_ముగింపు

      error = IsMacResource( library, ప్రవాహం, 0, face_సూచీ, aface );

      FT_TRACE3(( "%s\n", error ? "failed" : "successful" ));

#నిర్వచనం_రద్దు  FT_COMPONENT
#నిర్వచించు FT_COMPONENT  objs

    }

    ఒకవేళ ( ( FT_ERR_EQ( error, Unknown_File_Format )      ||
           FT_ERR_EQ( error, Invalid_ప్రవాహం_Operation ) ) &&
         ( వాదనలు->flags & FT_OPEN_PATHNAME )               )
      error = ఎక్కించు_face_లోపల_embedded_rfork( library, ప్రవాహం,
                                           face_సూచీ, aface, వాదనలు );
    తిరిగి error;
  }
#అయితే_ముగింపు

#అయితే_ముగింపు  /* !FT_MACINTOSH && FT_CONFIG_OPTION_MAC_FONTS */


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Open_Face( FT_Library           library,
                స్థిరరాశి FT_Open_వాదనలు*  వాదనలు,
                FT_Long              face_సూచీ,
                FT_Face             *aface )
  {
    తిరిగి ft_open_face_అంతర్గత( library, వాదనలు, face_సూచీ, aface, 1 );
  }


  స్థిర FT_Error
  ft_open_face_అంతర్గత( FT_Library           library,
                         స్థిరరాశి FT_Open_వాదనలు*  వాదనలు,
                         FT_Long              face_సూచీ,
                         FT_Face             *aface,
                         FT_Bool              test_mac_fonts )
  {
    FT_Error     error;
    FT_Driver    driver = ఖాళీ;
    FT_జ్ఞాపకం    జ్ఞాపకం = ఖాళీ;
    FT_ప్రవాహం    ప్రవాహం = ఖాళీ;
    FT_Face      face   = ఖాళీ;
    FT_List_కణుపు  కణుపు   = ఖాళీ;
    FT_Bool      external_ప్రవాహం;
    FT_Module*   cur;
    FT_Module*   హద్దు;

#నిర్వచితం_కాకపోతే FT_CONFIG_OPTION_MAC_FONTS
    FT_వాడని( test_mac_fonts );
#అయితే_ముగింపు


    /* only use lower 31 bits together with sign bit */
    ఒకవేళ ( face_సూచీ > 0 )
      face_సూచీ &= 0x7FFFFFFFL;
    లేకపోతే
    {
      face_సూచీ  = -face_సూచీ;
      face_సూచీ &= 0x7FFFFFFFL;
      face_సూచీ  = -face_సూచీ;
    }

#నిర్వచితమైతే FT_DEBUG_LEVEL_TRACE
    FT_TRACE3(( "FT_Open_Face: " ));
    ఒకవేళ ( face_సూచీ < 0 )
      FT_TRACE3(( "Requesting number of faces and named instances\n"));
    లేకపోతే
    {
      FT_TRACE3(( "Requesting face %ld", face_సూచీ & 0xFFFFL ));
      ఒకవేళ ( face_సూచీ & 0x7FFF0000L )
        FT_TRACE3(( ", named instance %ld", face_సూచీ >> 16 ));
      FT_TRACE3(( "\n" ));
    }
#అయితే_ముగింపు

    /* test for valid `library' delayed to `FT_Stream_New' */

    ఒకవేళ ( !వాదనలు )
      తిరిగి FT_THROW( Invalid_వాదం );

    external_ప్రవాహం = FT_BOOL( ( వాదనలు->flags & FT_OPEN_ప్రవాహం ) &&
                               వాదనలు->ప్రవాహం                     );

    /* create input stream */
    error = FT_ప్రవాహం_New( library, వాదనలు, &ప్రవాహం );
    ఒకవేళ ( error )
      వెళ్ళు Fail3;

    /* Do this error check after `FT_Stream_New` to ensure that the */
    /* 'close' callback is called.                                  */
    ఒకవేళ ( !aface && face_సూచీ >= 0 )
    {
      error = FT_THROW( Invalid_వాదం );
      వెళ్ళు Fail3;
    }

    జ్ఞాపకం = library->జ్ఞాపకం;

    /* If the font driver is specified in the `args' structure, use */
    /* it.  Otherwise, we scan the list of registered drivers.      */
    ఒకవేళ ( ( వాదనలు->flags & FT_OPEN_DRIVER ) && వాదనలు->driver )
    {
      driver = FT_DRIVER( వాదనలు->driver );

      /* not all modules are drivers, so check... */
      ఒకవేళ ( FT_MODULE_IS_DRIVER( driver ) )
      {
        FT_Int         అంకె_params = 0;
        FT_Parameter*  params     = ఖాళీ;


        ఒకవేళ ( వాదనలు->flags & FT_OPEN_PARAMS )
        {
          అంకె_params = వాదనలు->అంకె_params;
          params     = వాదనలు->params;
        }

        error = open_face( driver, &ప్రవాహం, &external_ప్రవాహం, face_సూచీ,
                           అంకె_params, params, &face );
        ఒకవేళ ( !error )
          వెళ్ళు Success;
      }
      లేకపోతే
        error = FT_THROW( Invalid_Handle );

      FT_ప్రవాహం_Free( ప్రవాహం, external_ప్రవాహం );
      వెళ్ళు Fail;
    }
    లేకపోతే
    {
      error = FT_ERR( Missing_Module );

      /* check each font driver for an appropriate format */
      cur   = library->modules;
      హద్దు = cur + library->అంకె_modules;

      కొరకు ( ; cur < హద్దు; cur++ )
      {
        /* not all modules are font drivers, so check... */
        ఒకవేళ ( FT_MODULE_IS_DRIVER( cur[0] ) )
        {
          FT_Int         అంకె_params = 0;
          FT_Parameter*  params     = ఖాళీ;


          driver = FT_DRIVER( cur[0] );

          ఒకవేళ ( వాదనలు->flags & FT_OPEN_PARAMS )
          {
            అంకె_params = వాదనలు->అంకె_params;
            params     = వాదనలు->params;
          }

          error = open_face( driver, &ప్రవాహం, &external_ప్రవాహం, face_సూచీ,
                             అంకె_params, params, &face );
          ఒకవేళ ( !error )
            వెళ్ళు Success;

#నిర్వచితమైతే FT_CONFIG_OPTION_MAC_FONTS
          ఒకవేళ ( test_mac_fonts                                           &&
               ft_strcmp( cur[0]->clazz->module_name, "truetype" ) == 0 &&
               FT_ERR_EQ( error, పట్టిక_Missing )                        )
          {
            /* TrueType but essential tables are missing */
            error = FT_ప్రవాహం_Seek( ప్రవాహం, 0 );
            ఒకవేళ ( error )
              ఆపు;

            error = open_face_PS_from_sfnt_ప్రవాహం( library,
                                                   ప్రవాహం,
                                                   face_సూచీ,
                                                   అంకె_params,
                                                   params,
                                                   aface );
            ఒకవేళ ( !error )
            {
              FT_ప్రవాహం_Free( ప్రవాహం, external_ప్రవాహం );
              తిరిగి error;
            }
          }
#అయితే_ముగింపు

          ఒకవేళ ( FT_ERR_NEQ( error, Unknown_File_Format ) )
            వెళ్ళు Fail3;
        }
      }

    Fail3:
      /* If we are on the mac, and we get an                          */
      /* FT_Err_Invalid_Stream_Operation it may be because we have an */
      /* empty data fork, so we need to check the resource fork.      */
      ఒకవేళ ( FT_ERR_NEQ( error, Cannot_Open_ప్రవాహం )       &&
           FT_ERR_NEQ( error, Unknown_File_Format )      &&
           FT_ERR_NEQ( error, Invalid_ప్రవాహం_Operation ) )
        వెళ్ళు Fail2;

#ఒకవేళ !defined( FT_MACINTOSH ) && defined( FT_CONFIG_OPTION_MAC_FONTS )
      ఒకవేళ ( test_mac_fonts )
      {
        error = ఎక్కించు_mac_face( library, ప్రవాహం, face_సూచీ, aface, వాదనలు );
        ఒకవేళ ( !error )
        {
          /* We don't want to go to Success here.  We've already done   */
          /* that.  On the other hand, if we succeeded we still need to */
          /* close this stream (we opened a different stream which      */
          /* extracted the interesting information out of this stream   */
          /* here.  That stream will still be open and the face will    */
          /* point to it).                                              */
          FT_ప్రవాహం_Free( ప్రవాహం, external_ప్రవాహం );
          తిరిగి error;
        }
      }

      ఒకవేళ ( FT_ERR_NEQ( error, Unknown_File_Format ) )
        వెళ్ళు Fail2;
#అయితే_ముగింపు  /* !FT_MACINTOSH && FT_CONFIG_OPTION_MAC_FONTS */

      /* no driver is able to handle this format */
      error = FT_THROW( Unknown_File_Format );

  Fail2:
      FT_ప్రవాహం_Free( ప్రవాహం, external_ప్రవాహం );
      వెళ్ళు Fail;
    }

  Success:
    FT_TRACE4(( "FT_Open_Face: New face object, adding to list\n" ));

    /* add the face object to its driver's list */
    ఒకవేళ ( FT_QNEW( కణుపు ) )
      వెళ్ళు Fail;

    కణుపు->దత్తాంశం = face;
    /* don't assume driver is the same as face->driver, so use */
    /* face->driver instead.                                   */
    FT_List_కూడు( &face->driver->faces_list, కణుపు );

    /* now allocate a glyph slot object for the face */
    FT_TRACE4(( "FT_Open_Face: Creating glyph slot\n" ));

    ఒకవేళ ( face_సూచీ >= 0 )
    {
      error = FT_New_Glyph_స్థానం( face, ఖాళీ );
      ఒకవేళ ( error )
        వెళ్ళు Fail;

      /* finally, allocate a size object for the face */
      {
        FT_కొలత  కొలత;


        FT_TRACE4(( "FT_Open_Face: Creating size object\n" ));

        error = FT_New_కొలత( face, &కొలత );
        ఒకవేళ ( error )
          వెళ్ళు Fail;

        face->కొలత = కొలత;
      }
    }

    /* some checks */

    ఒకవేళ ( FT_IS_SCALABLE( face ) )
    {
      ఒకవేళ ( face->ఎత్తు < 0 )
        face->ఎత్తు = (FT_Short)-face->ఎత్తు;

      ఒకవేళ ( !FT_HAS_VERTICAL( face ) )
        face->max_advance_ఎత్తు = (FT_Short)face->ఎత్తు;
    }

    ఒకవేళ ( FT_HAS_FIXED_SIZES( face ) )
    {
      FT_Int  i;


      కొరకు ( i = 0; i < face->అంకె_fixed_sizes; i++ )
      {
        FT_Bitmap_కొలత*  bsize = face->అందుబాటు_sizes + i;


        ఒకవేళ ( bsize->ఎత్తు < 0 )
          bsize->ఎత్తు = -bsize->ఎత్తు;
        ఒకవేళ ( bsize->x_ppem < 0 )
          bsize->x_ppem = -bsize->x_ppem;
        ఒకవేళ ( bsize->y_ppem < 0 )
          bsize->y_ppem = -bsize->y_ppem;

        /* check whether negation actually has worked */
        ఒకవేళ ( bsize->ఎత్తు < 0 || bsize->x_ppem < 0 || bsize->y_ppem < 0 )
        {
          FT_TRACE0(( "FT_Open_Face:"
                      " Invalid bitmap dimensions for strike %d,"
                      " now disabled\n", i ));
          bsize->వెడల్పు  = 0;
          bsize->ఎత్తు = 0;
          bsize->కొలత   = 0;
          bsize->x_ppem = 0;
          bsize->y_ppem = 0;
        }
      }
    }

    /* initialize internal face data */
    {
      FT_Face_అంతర్గత  అంతర్గత = face->అంతర్గత;


      అంతర్గత->transform_మాతృక.xx = 0x10000L;
      అంతర్గత->transform_మాతృక.xy = 0;
      అంతర్గత->transform_మాతృక.yx = 0;
      అంతర్గత->transform_మాతృక.yy = 0x10000L;

      అంతర్గత->transform_delta.x = 0;
      అంతర్గత->transform_delta.y = 0;

      అంతర్గత->refcount = 1;

      అంతర్గత->no_stem_darkening = -1;
    }

    ఒకవేళ ( aface )
      *aface = face;
    లేకపోతే
      FT_పూర్తి_Face( face );

    వెళ్ళు Exit;

  Fail:
    ఒకవేళ ( కణుపు )
      FT_పూర్తి_Face( face );    /* face must be in the driver's list */
    లేకపోతే ఒకవేళ ( face )
      destroy_face( జ్ఞాపకం, face, driver );

  Exit:
#నిర్వచితమైతే FT_DEBUG_LEVEL_TRACE
    ఒకవేళ ( !error && face_సూచీ < 0 )
    {
      FT_TRACE3(( "FT_Open_Face: The font has %ld face%s\n",
                  face->అంకె_faces,
                  face->అంకె_faces == 1 ? "" : "s" ));
      FT_TRACE3(( "              and %ld named instance%s for face %ld\n",
                  face->style_flags >> 16,
                  ( face->style_flags >> 16 ) == 1 ? "" : "s",
                  -face_సూచీ - 1 ));
    }
#అయితే_ముగింపు

    FT_TRACE4(( "FT_Open_Face: Return 0x%x\n", error ));

    తిరిగి error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Attach_File( FT_Face      face,
                  స్థిరరాశి అక్షరం*  filepathname )
  {
    FT_Open_వాదనలు  open;


    /* test for valid `face' delayed to `FT_Attach_Stream' */

    ఒకవేళ ( !filepathname )
      తిరిగి FT_THROW( Invalid_వాదం );

    open.ప్రవాహం   = ఖాళీ;
    open.flags    = FT_OPEN_PATHNAME;
    open.pathname = (అక్షరం*)filepathname;

    తిరిగి FT_Attach_ప్రవాహం( face, &open );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Attach_ప్రవాహం( FT_Face              face,
                    స్థిరరాశి FT_Open_వాదనలు*  parameters )
  {
    FT_ప్రవాహం  ప్రవాహం;
    FT_Error   error;
    FT_Driver  driver;

    FT_Driver_Class  clazz;


    /* test for valid `parameters' delayed to `FT_Stream_New' */

    ఒకవేళ ( !face )
      తిరిగి FT_THROW( Invalid_Face_Handle );

    driver = face->driver;
    ఒకవేళ ( !driver )
      తిరిగి FT_THROW( Invalid_Driver_Handle );

    error = FT_ప్రవాహం_New( driver->మూలం.library, parameters, &ప్రవాహం );
    ఒకవేళ ( error )
      వెళ్ళు Exit;

    /* we implement FT_Attach_Stream in each driver through the */
    /* `attach_file' interface                                  */

    error = FT_ERR( Unimplemented_Feature );
    clazz = driver->clazz;
    ఒకవేళ ( clazz->attach_file )
      error = clazz->attach_file( face, ప్రవాహం );

    /* close the attached stream */
    FT_ప్రవాహం_Free( ప్రవాహం,
                    FT_BOOL( parameters->ప్రవాహం                     &&
                             ( parameters->flags & FT_OPEN_ప్రవాహం ) ) );

  Exit:
    తిరిగి error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Reference_Face( FT_Face  face )
  {
    ఒకవేళ ( !face )
      తిరిగి FT_THROW( Invalid_Face_Handle );

    face->అంతర్గత->refcount++;

    తిరిగి FT_Err_Ok;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_పూర్తి_Face( FT_Face  face )
  {
    FT_Error     error;
    FT_Driver    driver;
    FT_జ్ఞాపకం    జ్ఞాపకం;
    FT_List_కణుపు  కణుపు;


    error = FT_ERR( Invalid_Face_Handle );
    ఒకవేళ ( face && face->driver )
    {
      face->అంతర్గత->refcount--;
      ఒకవేళ ( face->అంతర్గత->refcount > 0 )
        error = FT_Err_Ok;
      లేకపోతే
      {
        driver = face->driver;
        జ్ఞాపకం = driver->మూలం.జ్ఞాపకం;

        /* find face in driver's list */
        కణుపు = FT_List_కనుగొను( &driver->faces_list, face );
        ఒకవేళ ( కణుపు )
        {
          /* remove face object from the driver's list */
          FT_List_Remove( &driver->faces_list, కణుపు );
          FT_FREE( కణుపు );

          /* now destroy the object proper */
          destroy_face( జ్ఞాపకం, face, driver );
          error = FT_Err_Ok;
        }
      }
    }

    తిరిగి error;
  }


  /* documentation is in ftobjs.h */

  FT_EXPORT_DEF( FT_Error )
  FT_New_కొలత( FT_Face   face,
               FT_కొలత  *asize )
  {
    FT_Error         error;
    FT_జ్ఞాపకం        జ్ఞాపకం;
    FT_Driver        driver;
    FT_Driver_Class  clazz;

    FT_కొలత          కొలత = ఖాళీ;
    FT_List_కణుపు      కణుపు = ఖాళీ;

    FT_కొలత_అంతర్గత  అంతర్గత = ఖాళీ;


    ఒకవేళ ( !face )
      తిరిగి FT_THROW( Invalid_Face_Handle );

    ఒకవేళ ( !asize )
      తిరిగి FT_THROW( Invalid_వాదం );

    ఒకవేళ ( !face->driver )
      తిరిగి FT_THROW( Invalid_Driver_Handle );

    *asize = ఖాళీ;

    driver = face->driver;
    clazz  = driver->clazz;
    జ్ఞాపకం = face->జ్ఞాపకం;

    /* Allocate new size object and perform basic initialisation */
    ఒకవేళ ( FT_ALLOC( కొలత, clazz->కొలత_object_కొలత ) || FT_QNEW( కణుపు ) )
      వెళ్ళు Exit;

    కొలత->face = face;

    ఒకవేళ ( FT_NEW( అంతర్గత ) )
      వెళ్ళు Exit;

    కొలత->అంతర్గత = అంతర్గత;

    ఒకవేళ ( clazz->init_కొలత )
      error = clazz->init_కొలత( కొలత );

    /* in case of success, add to the face's list */
    ఒకవేళ ( !error )
    {
      *asize     = కొలత;
      కణుపు->దత్తాంశం = కొలత;
      FT_List_కూడు( &face->sizes_list, కణుపు );
    }

  Exit:
    ఒకవేళ ( error )
    {
      FT_FREE( కణుపు );
      ఒకవేళ ( కొలత )
        FT_FREE( కొలత->అంతర్గత );
      FT_FREE( కొలత );
    }

    తిరిగి error;
  }


  /* documentation is in ftobjs.h */

  FT_EXPORT_DEF( FT_Error )
  FT_పూర్తి_కొలత( FT_కొలత  కొలత )
  {
    FT_Error     error;
    FT_Driver    driver;
    FT_జ్ఞాపకం    జ్ఞాపకం;
    FT_Face      face;
    FT_List_కణుపు  కణుపు;


    ఒకవేళ ( !కొలత )
      తిరిగి FT_THROW( Invalid_కొలత_Handle );

    face = కొలత->face;
    ఒకవేళ ( !face )
      తిరిగి FT_THROW( Invalid_Face_Handle );

    driver = face->driver;
    ఒకవేళ ( !driver )
      తిరిగి FT_THROW( Invalid_Driver_Handle );

    జ్ఞాపకం = driver->మూలం.జ్ఞాపకం;

    error = FT_Err_Ok;
    కణుపు  = FT_List_కనుగొను( &face->sizes_list, కొలత );
    ఒకవేళ ( కణుపు )
    {
      FT_List_Remove( &face->sizes_list, కణుపు );
      FT_FREE( కణుపు );

      ఒకవేళ ( face->కొలత == కొలత )
      {
        face->కొలత = ఖాళీ;
        ఒకవేళ ( face->sizes_list.తల )
          face->కొలత = (FT_కొలత)(face->sizes_list.తల->దత్తాంశం);
      }

      destroy_కొలత( జ్ఞాపకం, కొలత, driver );
    }
    లేకపోతే
      error = FT_THROW( Invalid_కొలత_Handle );

    తిరిగి error;
  }


  /* documentation is in ftobjs.h */

  FT_పునాది_DEF( FT_Error )
  FT_సరిపోలిక_కొలత( FT_Face          face,
                 FT_కొలత_Request  req,
                 FT_Bool          ignore_వెడల్పు,
                 FT_ULong*        కొలత_సూచీ )
  {
    FT_Int   i;
    FT_Long  w, h;


    ఒకవేళ ( !FT_HAS_FIXED_SIZES( face ) )
      తిరిగి FT_THROW( Invalid_Face_Handle );

    /* FT_Bitmap_Size doesn't provide enough info... */
    ఒకవేళ ( req->type != FT_కొలత_REQUEST_TYPE_NOMINAL )
      తిరిగి FT_THROW( Unimplemented_Feature );

    w = FT_REQUEST_వెడల్పు ( req );
    h = FT_REQUEST_ఎత్తు( req );

    ఒకవేళ ( req->వెడల్పు && !req->ఎత్తు )
      h = w;
    లేకపోతే ఒకవేళ ( !req->వెడల్పు && req->ఎత్తు )
      w = h;

    w = FT_PIX_విడత( w );
    h = FT_PIX_విడత( h );

    ఒకవేళ ( !w || !h )
      తిరిగి FT_THROW( Invalid_Pixel_కొలత );

    కొరకు ( i = 0; i < face->అంకె_fixed_sizes; i++ )
    {
      FT_Bitmap_కొలత*  bsize = face->అందుబాటు_sizes + i;


      ఒకవేళ ( h != FT_PIX_విడత( bsize->y_ppem ) )
        కొనసాగు;

      ఒకవేళ ( w == FT_PIX_విడత( bsize->x_ppem ) || ignore_వెడల్పు )
      {
        FT_TRACE3(( "FT_Match_Size: bitmap strike %d matches\n", i ));

        ఒకవేళ ( కొలత_సూచీ )
          *కొలత_సూచీ = (FT_ULong)i;

        తిరిగి FT_Err_Ok;
      }
    }

    FT_TRACE3(( "FT_Match_Size: no matching bitmap strike\n" ));

    తిరిగి FT_THROW( Invalid_Pixel_కొలత );
  }


  /* documentation is in ftobjs.h */

  FT_పునాది_DEF( శూన్య )
  ft_synthesize_vertical_metrics( FT_Glyph_Metrics*  metrics,
                                  FT_Pos             advance )
  {
    FT_Pos  ఎత్తు = metrics->ఎత్తు;


    /* compensate for glyph with bbox above/below the baseline */
    ఒకవేళ ( metrics->horiBearingY < 0 )
    {
      ఒకవేళ ( ఎత్తు < metrics->horiBearingY )
        ఎత్తు = metrics->horiBearingY;
    }
    లేకపోతే ఒకవేళ ( metrics->horiBearingY > 0 )
      ఎత్తు -= metrics->horiBearingY;

    /* the factor 1.2 is a heuristical value */
    ఒకవేళ ( !advance )
      advance = ఎత్తు * 12 / 10;

    metrics->vertBearingX = metrics->horiBearingX - metrics->horiAdvance / 2;
    metrics->vertBearingY = ( advance - ఎత్తు ) / 2;
    metrics->vertAdvance  = advance;
  }


  స్థిర శూన్య
  ft_recompute_scaled_metrics( FT_Face           face,
                               FT_కొలత_Metrics*  metrics )
  {
    /* Compute root ascender, descender, test height, and max_advance */

#నిర్వచితమైతే GRID_FIT_METRICS
    metrics->ascender    = FT_PIX_CEIL( FT_MulFix( face->ascender,
                                                   metrics->y_scale ) );

    metrics->descender   = FT_PIX_FLOOR( FT_MulFix( face->descender,
                                                    metrics->y_scale ) );

    metrics->ఎత్తు      = FT_PIX_విడత( FT_MulFix( face->ఎత్తు,
                                                    metrics->y_scale ) );

    metrics->max_advance = FT_PIX_విడత( FT_MulFix( face->max_advance_వెడల్పు,
                                                    metrics->x_scale ) );
#లేకపోతే /* !GRID_FIT_METRICS */
    metrics->ascender    = FT_MulFix( face->ascender,
                                      metrics->y_scale );

    metrics->descender   = FT_MulFix( face->descender,
                                      metrics->y_scale );

    metrics->ఎత్తు      = FT_MulFix( face->ఎత్తు,
                                      metrics->y_scale );

    metrics->max_advance = FT_MulFix( face->max_advance_వెడల్పు,
                                      metrics->x_scale );
#అయితే_ముగింపు /* !GRID_FIT_METRICS */
  }


  FT_పునాది_DEF( శూన్య )
  FT_Select_Metrics( FT_Face   face,
                     FT_ULong  strike_సూచీ )
  {
    FT_కొలత_Metrics*  metrics;
    FT_Bitmap_కొలత*   bsize;


    metrics = &face->కొలత->metrics;
    bsize   = face->అందుబాటు_sizes + strike_సూచీ;

    metrics->x_ppem = (FT_UShort)( ( bsize->x_ppem + 32 ) >> 6 );
    metrics->y_ppem = (FT_UShort)( ( bsize->y_ppem + 32 ) >> 6 );

    ఒకవేళ ( FT_IS_SCALABLE( face ) )
    {
      metrics->x_scale = FT_DivFix( bsize->x_ppem,
                                    face->units_చొప్పున_EM );
      metrics->y_scale = FT_DivFix( bsize->y_ppem,
                                    face->units_చొప్పున_EM );

      ft_recompute_scaled_metrics( face, metrics );
    }
    లేకపోతే
    {
      metrics->x_scale     = 1L << 16;
      metrics->y_scale     = 1L << 16;
      metrics->ascender    = bsize->y_ppem;
      metrics->descender   = 0;
      metrics->ఎత్తు      = bsize->ఎత్తు << 6;
      metrics->max_advance = bsize->x_ppem;
    }
  }


  FT_పునాది_DEF( FT_Error )
  FT_Request_Metrics( FT_Face          face,
                      FT_కొలత_Request  req )
  {
    FT_Error  error = FT_Err_Ok;

    FT_కొలత_Metrics*  metrics;


    metrics = &face->కొలత->metrics;

    ఒకవేళ ( FT_IS_SCALABLE( face ) )
    {
      FT_Long  w = 0, h = 0, scaled_w = 0, scaled_h = 0;


      ఎంపిక ( req->type )
      {
      స్థితి FT_కొలత_REQUEST_TYPE_NOMINAL:
        w = h = face->units_చొప్పున_EM;
        ఆపు;

      స్థితి FT_కొలత_REQUEST_TYPE_REAL_DIM:
        w = h = face->ascender - face->descender;
        ఆపు;

      స్థితి FT_కొలత_REQUEST_TYPE_BBOX:
        w = face->bbox.xMax - face->bbox.xMin;
        h = face->bbox.yMax - face->bbox.yMin;
        ఆపు;

      స్థితి FT_కొలత_REQUEST_TYPE_CELL:
        w = face->max_advance_వెడల్పు;
        h = face->ascender - face->descender;
        ఆపు;

      స్థితి FT_కొలత_REQUEST_TYPE_SCALES:
        metrics->x_scale = (FT_Fixed)req->వెడల్పు;
        metrics->y_scale = (FT_Fixed)req->ఎత్తు;
        ఒకవేళ ( !metrics->x_scale )
          metrics->x_scale = metrics->y_scale;
        లేకపోతే ఒకవేళ ( !metrics->y_scale )
          metrics->y_scale = metrics->x_scale;
        వెళ్ళు Calculate_Ppem;

      స్థితి FT_కొలత_REQUEST_TYPE_MAX:
        ఆపు;
      }

      /* to be on the safe side */
      ఒకవేళ ( w < 0 )
        w = -w;

      ఒకవేళ ( h < 0 )
        h = -h;

      scaled_w = FT_REQUEST_వెడల్పు ( req );
      scaled_h = FT_REQUEST_ఎత్తు( req );

      /* determine scales */
      ఒకవేళ ( req->ఎత్తు || !req->వెడల్పు )
      {
        ఒకవేళ ( h == 0 )
        {
          FT_ERROR(( "FT_Request_Metrics: Divide by zero\n" ));
          error = FT_ERR( Divide_By_Zero );
          వెళ్ళు Exit;
        }

        metrics->y_scale = FT_DivFix( scaled_h, h );
      }

      ఒకవేళ ( req->వెడల్పు )
      {
        ఒకవేళ ( w == 0 )
        {
          FT_ERROR(( "FT_Request_Metrics: Divide by zero\n" ));
          error = FT_ERR( Divide_By_Zero );
          వెళ్ళు Exit;
        }

        metrics->x_scale = FT_DivFix( scaled_w, w );
      }
      లేకపోతే
      {
        metrics->x_scale = metrics->y_scale;
        scaled_w = FT_MulDiv( scaled_h, w, h );
      }

      ఒకవేళ ( !req->ఎత్తు )
      {
        metrics->y_scale = metrics->x_scale;
        scaled_h = FT_MulDiv( scaled_w, h, w );
      }

      ఒకవేళ ( req->type == FT_కొలత_REQUEST_TYPE_CELL )
      {
        ఒకవేళ ( metrics->y_scale > metrics->x_scale )
          metrics->y_scale = metrics->x_scale;
        లేకపోతే
          metrics->x_scale = metrics->y_scale;
      }

  Calculate_Ppem:
      /* calculate the ppems */
      ఒకవేళ ( req->type != FT_కొలత_REQUEST_TYPE_NOMINAL )
      {
        scaled_w = FT_MulFix( face->units_చొప్పున_EM, metrics->x_scale );
        scaled_h = FT_MulFix( face->units_చొప్పున_EM, metrics->y_scale );
      }

      scaled_w = ( scaled_w + 32 ) >> 6;
      scaled_h = ( scaled_h + 32 ) >> 6;
      ఒకవేళ ( scaled_w > (FT_Long)FT_USHORT_MAX ||
           scaled_h > (FT_Long)FT_USHORT_MAX )
      {
        FT_ERROR(( "FT_Request_Metrics: Resulting ppem size too large\n" ));
        error = FT_ERR( Invalid_Pixel_కొలత );
        వెళ్ళు Exit;
      }

      metrics->x_ppem = (FT_UShort)scaled_w;
      metrics->y_ppem = (FT_UShort)scaled_h;

      ft_recompute_scaled_metrics( face, metrics );
    }
    లేకపోతే
    {
      FT_ZERO( metrics );
      metrics->x_scale = 1L << 16;
      metrics->y_scale = 1L << 16;
    }

  Exit:
    తిరిగి error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Select_కొలత( FT_Face  face,
                  FT_Int   strike_సూచీ )
  {
    FT_Error         error = FT_Err_Ok;
    FT_Driver_Class  clazz;


    ఒకవేళ ( !face || !FT_HAS_FIXED_SIZES( face ) )
      తిరిగి FT_THROW( Invalid_Face_Handle );

    ఒకవేళ ( strike_సూచీ < 0 || strike_సూచీ >= face->అంకె_fixed_sizes )
      తిరిగి FT_THROW( Invalid_వాదం );

    clazz = face->driver->clazz;

    ఒకవేళ ( clazz->select_కొలత )
    {
      error = clazz->select_కొలత( face->కొలత, (FT_ULong)strike_సూచీ );

      FT_TRACE5(( "FT_Select_Size (%s driver):\n",
                  face->driver->మూలం.clazz->module_name ));
    }
    లేకపోతే
    {
      FT_Select_Metrics( face, (FT_ULong)strike_సూచీ );

      FT_TRACE5(( "FT_Select_Size:\n" ));
    }

#నిర్వచితమైతే FT_DEBUG_LEVEL_TRACE
    {
      FT_కొలత_Metrics*  metrics = &face->కొలత->metrics;


      FT_TRACE5(( "  x scale: %ld (%f)\n",
                  metrics->x_scale, (ద్విగుణ)metrics->x_scale / 65536 ));
      FT_TRACE5(( "  y scale: %ld (%f)\n",
                  metrics->y_scale, (ద్విగుణ)metrics->y_scale / 65536 ));
      FT_TRACE5(( "  ascender: %f\n",
                  (ద్విగుణ)metrics->ascender / 64 ));
      FT_TRACE5(( "  descender: %f\n",
                  (ద్విగుణ)metrics->descender / 64 ));
      FT_TRACE5(( "  height: %f\n",
                  (ద్విగుణ)metrics->ఎత్తు / 64 ));
      FT_TRACE5(( "  max advance: %f\n",
                  (ద్విగుణ)metrics->max_advance / 64 ));
      FT_TRACE5(( "  x ppem: %d\n", metrics->x_ppem ));
      FT_TRACE5(( "  y ppem: %d\n", metrics->y_ppem ));
    }
#అయితే_ముగింపు

    తిరిగి error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Request_కొలత( FT_Face          face,
                   FT_కొలత_Request  req )
  {
    FT_Error         error;
    FT_Driver_Class  clazz;
    FT_ULong         strike_సూచీ;


    ఒకవేళ ( !face )
      తిరిగి FT_THROW( Invalid_Face_Handle );

    ఒకవేళ ( !face->కొలత )
      తిరిగి FT_THROW( Invalid_కొలత_Handle );

    ఒకవేళ ( !req || req->వెడల్పు < 0 || req->ఎత్తు < 0 ||
         req->type >= FT_కొలత_REQUEST_TYPE_MAX )
      తిరిగి FT_THROW( Invalid_వాదం );

    /* signal the auto-hinter to recompute its size metrics */
    /* (if requested)                                       */
    face->కొలత->అంతర్గత->autohint_metrics.x_scale = 0;

    clazz = face->driver->clazz;

    ఒకవేళ ( clazz->request_కొలత )
    {
      error = clazz->request_కొలత( face->కొలత, req );

      FT_TRACE5(( "FT_Request_Size (%s driver):\n",
                  face->driver->మూలం.clazz->module_name ));
    }
    లేకపోతే ఒకవేళ ( !FT_IS_SCALABLE( face ) && FT_HAS_FIXED_SIZES( face ) )
    {
      /*
       * The reason that a driver doesn't have `request_size' defined is
       * either that the scaling here suffices or that the supported formats
       * are bitmap-only and size matching is not implemented.
       *
       * In the latter case, a simple size matching is done.
       */
      error = FT_సరిపోలిక_కొలత( face, req, 0, &strike_సూచీ );
      ఒకవేళ ( error )
        వెళ్ళు Exit;

      తిరిగి FT_Select_కొలత( face, (FT_Int)strike_సూచీ );
    }
    లేకపోతే
    {
      error = FT_Request_Metrics( face, req );
      ఒకవేళ ( error )
        వెళ్ళు Exit;

      FT_TRACE5(( "FT_Request_Size:\n" ));
    }

#నిర్వచితమైతే FT_DEBUG_LEVEL_TRACE
    {
      FT_కొలత_Metrics*  metrics = &face->కొలత->metrics;


      FT_TRACE5(( "  x scale: %ld (%f)\n",
                  metrics->x_scale, (ద్విగుణ)metrics->x_scale / 65536 ));
      FT_TRACE5(( "  y scale: %ld (%f)\n",
                  metrics->y_scale, (ద్విగుణ)metrics->y_scale / 65536 ));
      FT_TRACE5(( "  ascender: %f\n",
                  (ద్విగుణ)metrics->ascender / 64 ));
      FT_TRACE5(( "  descender: %f\n",
                  (ద్విగుణ)metrics->descender / 64 ));
      FT_TRACE5(( "  height: %f\n",
                  (ద్విగుణ)metrics->ఎత్తు / 64 ));
      FT_TRACE5(( "  max advance: %f\n",
                  (ద్విగుణ)metrics->max_advance / 64 ));
      FT_TRACE5(( "  x ppem: %d\n", metrics->x_ppem ));
      FT_TRACE5(( "  y ppem: %d\n", metrics->y_ppem ));
    }
#అయితే_ముగింపు

  Exit:
    తిరిగి error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_సమితి_Char_కొలత( FT_Face     face,
                    FT_F26Dot6  char_వెడల్పు,
                    FT_F26Dot6  char_ఎత్తు,
                    FT_UInt     horz_resolution,
                    FT_UInt     vert_resolution )
  {
    FT_కొలత_Request_Rec  req;


    /* check of `face' delayed to `FT_Request_Size' */

    ఒకవేళ ( !char_వెడల్పు )
      char_వెడల్పు = char_ఎత్తు;
    లేకపోతే ఒకవేళ ( !char_ఎత్తు )
      char_ఎత్తు = char_వెడల్పు;

    ఒకవేళ ( !horz_resolution )
      horz_resolution = vert_resolution;
    లేకపోతే ఒకవేళ ( !vert_resolution )
      vert_resolution = horz_resolution;

    ఒకవేళ ( char_వెడల్పు  < 1 * 64 )
      char_వెడల్పు  = 1 * 64;
    ఒకవేళ ( char_ఎత్తు < 1 * 64 )
      char_ఎత్తు = 1 * 64;

    ఒకవేళ ( !horz_resolution )
      horz_resolution = vert_resolution = 72;

    req.type           = FT_కొలత_REQUEST_TYPE_NOMINAL;
    req.వెడల్పు          = char_వెడల్పు;
    req.ఎత్తు         = char_ఎత్తు;
    req.horiResolution = horz_resolution;
    req.vertResolution = vert_resolution;

    తిరిగి FT_Request_కొలత( face, &req );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_సమితి_Pixel_Sizes( FT_Face  face,
                      FT_UInt  pixel_వెడల్పు,
                      FT_UInt  pixel_ఎత్తు )
  {
    FT_కొలత_Request_Rec  req;


    /* check of `face' delayed to `FT_Request_Size' */

    ఒకవేళ ( pixel_వెడల్పు == 0 )
      pixel_వెడల్పు = pixel_ఎత్తు;
    లేకపోతే ఒకవేళ ( pixel_ఎత్తు == 0 )
      pixel_ఎత్తు = pixel_వెడల్పు;

    ఒకవేళ ( pixel_వెడల్పు  < 1 )
      pixel_వెడల్పు  = 1;
    ఒకవేళ ( pixel_ఎత్తు < 1 )
      pixel_ఎత్తు = 1;

    /* use `>=' to avoid potential compiler warning on 16bit platforms */
    ఒకవేళ ( pixel_వెడల్పు >= 0xFFFFU )
      pixel_వెడల్పు = 0xFFFFU;
    ఒకవేళ ( pixel_ఎత్తు >= 0xFFFFU )
      pixel_ఎత్తు = 0xFFFFU;

    req.type           = FT_కొలత_REQUEST_TYPE_NOMINAL;
    req.వెడల్పు          = (FT_Long)( pixel_వెడల్పు << 6 );
    req.ఎత్తు         = (FT_Long)( pixel_ఎత్తు << 6 );
    req.horiResolution = 0;
    req.vertResolution = 0;

    తిరిగి FT_Request_కొలత( face, &req );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Kerning( FT_Face     face,
                  FT_UInt     ఎడమ_glyph,
                  FT_UInt     కుడి_glyph,
                  FT_UInt     kern_బహుళకం,
                  FT_సదిశ  *akerning )
  {
    FT_Error   error = FT_Err_Ok;
    FT_Driver  driver;


    ఒకవేళ ( !face )
      తిరిగి FT_THROW( Invalid_Face_Handle );

    ఒకవేళ ( !akerning )
      తిరిగి FT_THROW( Invalid_వాదం );

    driver = face->driver;

    akerning->x = 0;
    akerning->y = 0;

    ఒకవేళ ( driver->clazz->get_kerning )
    {
      error = driver->clazz->get_kerning( face,
                                          ఎడమ_glyph,
                                          కుడి_glyph,
                                          akerning );
      ఒకవేళ ( !error )
      {
        ఒకవేళ ( kern_బహుళకం != FT_KERNING_UNSCALED )
        {
          akerning->x = FT_MulFix( akerning->x, face->కొలత->metrics.x_scale );
          akerning->y = FT_MulFix( akerning->y, face->కొలత->metrics.y_scale );

          ఒకవేళ ( kern_బహుళకం != FT_KERNING_UNFITTED )
          {
            FT_Pos  orig_x = akerning->x;
            FT_Pos  orig_y = akerning->y;


            /* we scale down kerning values for small ppem values */
            /* to avoid that rounding makes them too big.         */
            /* `25' has been determined heuristically.            */
            ఒకవేళ ( face->కొలత->metrics.x_ppem < 25 )
              akerning->x = FT_MulDiv( orig_x,
                                       face->కొలత->metrics.x_ppem, 25 );
            ఒకవేళ ( face->కొలత->metrics.y_ppem < 25 )
              akerning->y = FT_MulDiv( orig_y,
                                       face->కొలత->metrics.y_ppem, 25 );

            akerning->x = FT_PIX_విడత( akerning->x );
            akerning->y = FT_PIX_విడత( akerning->y );

#నిర్వచితమైతే FT_DEBUG_LEVEL_TRACE
            {
              FT_Pos  orig_x_rounded = FT_PIX_విడత( orig_x );
              FT_Pos  orig_y_rounded = FT_PIX_విడత( orig_y );


              ఒకవేళ ( akerning->x != orig_x_rounded ||
                   akerning->y != orig_y_rounded )
                FT_TRACE5(( "FT_Get_Kerning: horizontal kerning"
                            " (%ld, %ld) scaled down to (%ld, %ld) pixels\n",
                            orig_x_rounded / 64, orig_y_rounded / 64,
                            akerning->x / 64, akerning->y / 64 ));
            }
#అయితే_ముగింపు
          }
        }
      }
    }

    తిరిగి error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Track_Kerning( FT_Face    face,
                        FT_Fixed   బిందువు_కొలత,
                        FT_Int     degree,
                        FT_Fixed*  akerning )
  {
    FT_Service_Kerning  service;
    FT_Error            error = FT_Err_Ok;


    ఒకవేళ ( !face )
      తిరిగి FT_THROW( Invalid_Face_Handle );

    ఒకవేళ ( !akerning )
      తిరిగి FT_THROW( Invalid_వాదం );

    FT_FACE_కనుగొను_SERVICE( face, service, KERNING );
    ఒకవేళ ( !service )
      తిరిగి FT_THROW( Unimplemented_Feature );

    error = service->get_track( face,
                                బిందువు_కొలత,
                                degree,
                                akerning );

    తిరిగి error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Select_Charmap( FT_Face      face,
                     FT_Encoding  encoding )
  {
    FT_CharMap*  cur;
    FT_CharMap*  హద్దు;


    ఒకవేళ ( !face )
      తిరిగి FT_THROW( Invalid_Face_Handle );

    /* FT_ENCODING_NONE is a valid encoding for BDF, PCF, and Windows FNT */
    ఒకవేళ ( encoding == FT_ENCODING_NONE && !face->అంకె_charmaps )
      తిరిగి FT_THROW( Invalid_వాదం );

    /* FT_ENCODING_UNICODE is special.  We try to find the `best' Unicode */
    /* charmap available, i.e., one with UCS-4 characters, if possible.   */
    /*                                                                    */
    /* This is done by find_unicode_charmap() above, to share code.       */
    ఒకవేళ ( encoding == FT_ENCODING_UNICODE )
      తిరిగి కనుగొను_unicode_charmap( face );

    cur = face->charmaps;
    ఒకవేళ ( !cur )
      తిరిగి FT_THROW( Invalid_CharMap_Handle );

    హద్దు = cur + face->అంకె_charmaps;

    కొరకు ( ; cur < హద్దు; cur++ )
    {
      ఒకవేళ ( cur[0]->encoding == encoding )
      {
        face->charmap = cur[0];
        తిరిగి FT_Err_Ok;
      }
    }

    తిరిగి FT_THROW( Invalid_వాదం );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_సమితి_Charmap( FT_Face     face,
                  FT_CharMap  charmap )
  {
    FT_CharMap*  cur;
    FT_CharMap*  హద్దు;


    ఒకవేళ ( !face )
      తిరిగి FT_THROW( Invalid_Face_Handle );

    cur = face->charmaps;
    ఒకవేళ ( !cur || !charmap )
      తిరిగి FT_THROW( Invalid_CharMap_Handle );

    హద్దు = cur + face->అంకె_charmaps;

    కొరకు ( ; cur < హద్దు; cur++ )
    {
      ఒకవేళ ( cur[0] == charmap )
      {
        FT_Long  format = FT_Get_CMap_Format( charmap );


        ఒకవేళ ( format != 14 && format != 15 )
        {
          face->charmap = cur[0];
          తిరిగి FT_Err_Ok;
        }
      }
    }

    తిరిగి FT_THROW( Invalid_వాదం );
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Int )
  FT_Get_Charmap_సూచీ( FT_CharMap  charmap )
  {
    FT_Int  i;


    ఒకవేళ ( !charmap || !charmap->face )
      తిరిగి -1;

    కొరకు ( i = 0; i < charmap->face->అంకె_charmaps; i++ )
      ఒకవేళ ( charmap->face->charmaps[i] == charmap )
        ఆపు;

    FT_ASSERT( i < charmap->face->అంకె_charmaps );

    తిరిగి i;
  }


  స్థిర శూన్య
  ft_cmap_పూర్తి_అంతర్గత( FT_CMap  cmap )
  {
    FT_CMap_Class  clazz  = cmap->clazz;
    FT_Face        face   = cmap->charmap.face;
    FT_జ్ఞాపకం      జ్ఞాపకం = FT_FACE_జ్ఞాపకం( face );


    ఒకవేళ ( clazz->పూర్తి )
      clazz->పూర్తి( cmap );

    FT_FREE( cmap );
  }


  FT_పునాది_DEF( శూన్య )
  FT_CMap_పూర్తి( FT_CMap  cmap )
  {
    ఒకవేళ ( cmap )
    {
      FT_Face    face   = cmap->charmap.face;
      FT_జ్ఞాపకం  జ్ఞాపకం = FT_FACE_జ్ఞాపకం( face );
      FT_Error   error;
      FT_Int     i, j;


      కొరకు ( i = 0; i < face->అంకె_charmaps; i++ )
      {
        ఒకవేళ ( (FT_CMap)face->charmaps[i] == cmap )
        {
          FT_CharMap  చివరి_charmap = face->charmaps[face->అంకె_charmaps - 1];


          ఒకవేళ ( FT_QRENEW_ARRAY( face->charmaps,
                                face->అంకె_charmaps,
                                face->అంకె_charmaps - 1 ) )
            తిరిగి;

          /* remove it from our list of charmaps */
          కొరకు ( j = i + 1; j < face->అంకె_charmaps; j++ )
          {
            ఒకవేళ ( j == face->అంకె_charmaps - 1 )
              face->charmaps[j - 1] = చివరి_charmap;
            లేకపోతే
              face->charmaps[j - 1] = face->charmaps[j];
          }

          face->అంకె_charmaps--;

          ఒకవేళ ( (FT_CMap)face->charmap == cmap )
            face->charmap = ఖాళీ;

          ft_cmap_పూర్తి_అంతర్గత( cmap );

          ఆపు;
        }
      }
    }
  }


  FT_పునాది_DEF( FT_Error )
  FT_CMap_New( FT_CMap_Class  clazz,
               FT_సూచిక     init_దత్తాంశం,
               FT_CharMap     charmap,
               FT_CMap       *acmap )
  {
    FT_Error   error;
    FT_Face    face;
    FT_జ్ఞాపకం  జ్ఞాపకం;
    FT_CMap    cmap = ఖాళీ;


    ఒకవేళ ( !clazz || !charmap || !charmap->face )
      తిరిగి FT_THROW( Invalid_వాదం );

    face   = charmap->face;
    జ్ఞాపకం = FT_FACE_జ్ఞాపకం( face );

    ఒకవేళ ( !FT_ALLOC( cmap, clazz->కొలత ) )
    {
      cmap->charmap = *charmap;
      cmap->clazz   = clazz;

      ఒకవేళ ( clazz->init )
      {
        error = clazz->init( cmap, init_దత్తాంశం );
        ఒకవేళ ( error )
          వెళ్ళు Fail;
      }

      /* add it to our list of charmaps */
      ఒకవేళ ( FT_QRENEW_ARRAY( face->charmaps,
                            face->అంకె_charmaps,
                            face->అంకె_charmaps + 1 ) )
        వెళ్ళు Fail;

      face->charmaps[face->అంకె_charmaps++] = (FT_CharMap)cmap;
    }

  Exit:
    ఒకవేళ ( acmap )
      *acmap = cmap;

    తిరిగి error;

  Fail:
    ft_cmap_పూర్తి_అంతర్గత( cmap );
    cmap = ఖాళీ;
    వెళ్ళు Exit;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt )
  FT_Get_Char_సూచీ( FT_Face   face,
                     FT_ULong  charcode )
  {
    FT_UInt  ఫలితం = 0;


    ఒకవేళ ( face && face->charmap )
    {
      FT_CMap  cmap = FT_CMAP( face->charmap );


      ఒకవేళ ( charcode > 0xFFFFFFFFUL )
      {
        FT_TRACE1(( "FT_Get_Char_Index: too large charcode" ));
        FT_TRACE1(( " 0x%lx is truncated\n", charcode ));
      }

      ఫలితం = cmap->clazz->char_సూచీ( cmap, (FT_UInt32)charcode );
      ఒకవేళ ( ఫలితం >= (FT_UInt)face->అంకె_glyphs )
        ఫలితం = 0;
    }

    తిరిగి ఫలితం;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_ULong )
  FT_Get_మొదటి_Char( FT_Face   face,
                     FT_UInt  *agindex )
  {
    FT_ULong  ఫలితం = 0;
    FT_UInt   gindex = 0;


    /* only do something if we have a charmap, and we have glyphs at all */
    ఒకవేళ ( face && face->charmap && face->అంకె_glyphs )
    {
      gindex = FT_Get_Char_సూచీ( face, 0 );
      ఒకవేళ ( gindex == 0 )
        ఫలితం = FT_Get_తదుపరి_Char( face, 0, &gindex );
    }

    ఒకవేళ ( agindex )
      *agindex = gindex;

    తిరిగి ఫలితం;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_ULong )
  FT_Get_తదుపరి_Char( FT_Face   face,
                    FT_ULong  charcode,
                    FT_UInt  *agindex )
  {
    FT_ULong  ఫలితం = 0;
    FT_UInt   gindex = 0;


    ఒకవేళ ( face && face->charmap && face->అంకె_glyphs )
    {
      FT_UInt32  సంకేతం_2 = (FT_UInt32)charcode;
      FT_CMap    cmap = FT_CMAP( face->charmap );


      చేయి
      {
        gindex = cmap->clazz->char_తదుపరి( cmap, &సంకేతం_2 );

      } అయితే ( gindex >= (FT_UInt)face->అంకె_glyphs );

      ఫలితం = ( gindex == 0 ) ? 0 : సంకేతం_2;
    }

    ఒకవేళ ( agindex )
      *agindex = gindex;

    తిరిగి ఫలితం;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Face_Properties( FT_Face        face,
                      FT_UInt        అంకె_properties,
                      FT_Parameter*  properties )
  {
    FT_Error  error = FT_Err_Ok;


    ఒకవేళ ( అంకె_properties > 0 && !properties )
    {
      error = FT_THROW( Invalid_వాదం );
      వెళ్ళు Exit;
    }

    కొరకు ( ; అంకె_properties > 0; అంకె_properties-- )
    {
      ఒకవేళ ( properties->ట్యాగ్ == FT_PARAM_ట్యాగ్_STEM_DARKENING )
      {
        ఒకవేళ ( properties->దత్తాంశం )
        {
          ఒకవేళ ( *( (FT_Bool*)properties->దత్తాంశం ) == TRUE )
            face->అంతర్గత->no_stem_darkening = FALSE;
          లేకపోతే
            face->అంతర్గత->no_stem_darkening = TRUE;
        }
        లేకపోతే
        {
          /* use module default */
          face->అంతర్గత->no_stem_darkening = -1;
        }
      }
      లేకపోతే ఒకవేళ ( properties->ట్యాగ్ == FT_PARAM_ట్యాగ్_LCD_FILTER_WEIGHTS )
      {
        error = FT_THROW( Unimplemented_Feature );
        వెళ్ళు Exit;
      }
      లేకపోతే ఒకవేళ ( properties->ట్యాగ్ == FT_PARAM_ట్యాగ్_RANDOM_విత్తనం )
      {
        ఒకవేళ ( properties->దత్తాంశం )
        {
          face->అంతర్గత->random_విత్తనం = *( (FT_Int32*)properties->దత్తాంశం );
          ఒకవేళ ( face->అంతర్గత->random_విత్తనం < 0 )
            face->అంతర్గత->random_విత్తనం = 0;
        }
        లేకపోతే
        {
          /* use module default */
          face->అంతర్గత->random_విత్తనం = -1;
        }
      }
      లేకపోతే
      {
        error = FT_THROW( Invalid_వాదం );
        వెళ్ళు Exit;
      }

      ఒకవేళ ( error )
        ఆపు;

      properties++;
    }

  Exit:
    తిరిగి error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt )
  FT_Face_Get_Char_Variant_సూచీ( FT_Face   face,
                               FT_ULong  charcode,
                               FT_ULong  variantSelector )
  {
    FT_UInt  ఫలితం = 0;


    ఒకవేళ ( face                                           &&
         face->charmap                                  &&
         face->charmap->encoding == FT_ENCODING_UNICODE )
    {
      FT_CharMap  charmap = కనుగొను_variant_selector_charmap( face );
      FT_CMap     ucmap = FT_CMAP( face->charmap );


      ఒకవేళ ( charmap )
      {
        FT_CMap  vcmap = FT_CMAP( charmap );


        ఒకవేళ ( charcode > 0xFFFFFFFFUL )
        {
          FT_TRACE1(( "FT_Face_GetCharVariantIndex:"
                      " too large charcode" ));
          FT_TRACE1(( " 0x%lx is truncated\n", charcode ));
        }
        ఒకవేళ ( variantSelector > 0xFFFFFFFFUL )
        {
          FT_TRACE1(( "FT_Face_GetCharVariantIndex:"
                      " too large variantSelector" ));
          FT_TRACE1(( " 0x%lx is truncated\n", variantSelector ));
        }

        ఫలితం = vcmap->clazz->char_var_సూచీ( vcmap, ucmap,
                                               (FT_UInt32)charcode,
                                               (FT_UInt32)variantSelector );
      }
    }

    తిరిగి ఫలితం;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Int )
  FT_Face_GetCharVariantIsDefault( FT_Face   face,
                                   FT_ULong  charcode,
                                   FT_ULong  variantSelector )
  {
    FT_Int  ఫలితం = -1;


    ఒకవేళ ( face )
    {
      FT_CharMap  charmap = కనుగొను_variant_selector_charmap( face );


      ఒకవేళ ( charmap )
      {
        FT_CMap  vcmap = FT_CMAP( charmap );


        ఒకవేళ ( charcode > 0xFFFFFFFFUL )
        {
          FT_TRACE1(( "FT_Face_GetCharVariantIsDefault:"
                      " too large charcode" ));
          FT_TRACE1(( " 0x%lx is truncated\n", charcode ));
        }
        ఒకవేళ ( variantSelector > 0xFFFFFFFFUL )
        {
          FT_TRACE1(( "FT_Face_GetCharVariantIsDefault:"
                      " too large variantSelector" ));
          FT_TRACE1(( " 0x%lx is truncated\n", variantSelector ));
        }

        ఫలితం = vcmap->clazz->char_var_default( vcmap,
                                                 (FT_UInt32)charcode,
                                                 (FT_UInt32)variantSelector );
      }
    }

    తిరిగి ఫలితం;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt32* )
  FT_Face_GetVariantSelectors( FT_Face  face )
  {
    FT_UInt32  *ఫలితం = ఖాళీ;


    ఒకవేళ ( face )
    {
      FT_CharMap  charmap = కనుగొను_variant_selector_charmap( face );


      ఒకవేళ ( charmap )
      {
        FT_CMap    vcmap  = FT_CMAP( charmap );
        FT_జ్ఞాపకం  జ్ఞాపకం = FT_FACE_జ్ఞాపకం( face );


        ఫలితం = vcmap->clazz->variant_list( vcmap, జ్ఞాపకం );
      }
    }

    తిరిగి ఫలితం;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt32* )
  FT_Face_Get_Variants_యొక్క_Char( FT_Face   face,
                             FT_ULong  charcode )
  {
    FT_UInt32  *ఫలితం = ఖాళీ;


    ఒకవేళ ( face )
    {
      FT_CharMap  charmap = కనుగొను_variant_selector_charmap( face );


      ఒకవేళ ( charmap )
      {
        FT_CMap    vcmap  = FT_CMAP( charmap );
        FT_జ్ఞాపకం  జ్ఞాపకం = FT_FACE_జ్ఞాపకం( face );


        ఒకవేళ ( charcode > 0xFFFFFFFFUL )
        {
          FT_TRACE1(( "FT_Face_GetVariantsOfChar: too large charcode" ));
          FT_TRACE1(( " 0x%lx is truncated\n", charcode ));
        }

        ఫలితం = vcmap->clazz->charvariant_list( vcmap, జ్ఞాపకం,
                                                 (FT_UInt32)charcode );
      }
    }
    తిరిగి ఫలితం;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt32* )
  FT_Face_Get_Chars_యొక్క_Variant( FT_Face   face,
                             FT_ULong  variantSelector )
  {
    FT_UInt32  *ఫలితం = ఖాళీ;


    ఒకవేళ ( face )
    {
      FT_CharMap  charmap = కనుగొను_variant_selector_charmap( face );


      ఒకవేళ ( charmap )
      {
        FT_CMap    vcmap  = FT_CMAP( charmap );
        FT_జ్ఞాపకం  జ్ఞాపకం = FT_FACE_జ్ఞాపకం( face );


        ఒకవేళ ( variantSelector > 0xFFFFFFFFUL )
        {
          FT_TRACE1(( "FT_Get_Char_Index: too large variantSelector" ));
          FT_TRACE1(( " 0x%lx is truncated\n", variantSelector ));
        }

        ఫలితం = vcmap->clazz->variantchar_list( vcmap, జ్ఞాపకం,
                                                 (FT_UInt32)variantSelector );
      }
    }

    తిరిగి ఫలితం;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_UInt )
  FT_Get_Name_సూచీ( FT_Face           face,
                     స్థిరరాశి FT_String*  glyph_name )
  {
    FT_UInt  ఫలితం = 0;


    ఒకవేళ ( face                       &&
         FT_HAS_GLYPH_పేర్లు( face ) &&
         glyph_name                 )
    {
      FT_Service_GlyphDict  service;


      FT_FACE_LOOKUP_SERVICE( face,
                              service,
                              GLYPH_DICT );

      ఒకవేళ ( service && service->name_సూచీ )
        ఫలితం = service->name_సూచీ( face, glyph_name );
    }

    తిరిగి ఫలితం;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_Glyph_Name( FT_Face     face,
                     FT_UInt     glyph_సూచీ,
                     FT_సూచిక  బఫరు,
                     FT_UInt     బఫరు_max )
  {
    FT_Error              error;
    FT_Service_GlyphDict  service;


    ఒకవేళ ( !face )
      తిరిగి FT_THROW( Invalid_Face_Handle );

    ఒకవేళ ( !బఫరు || బఫరు_max == 0 )
      తిరిగి FT_THROW( Invalid_వాదం );

    /* clean up buffer */
    ((FT_Byte*)బఫరు)[0] = '\0';

    ఒకవేళ ( (FT_Long)glyph_సూచీ >= face->అంకె_glyphs )
      తిరిగి FT_THROW( Invalid_Glyph_సూచీ );

    ఒకవేళ ( !FT_HAS_GLYPH_పేర్లు( face ) )
      తిరిగి FT_THROW( Invalid_వాదం );

    FT_FACE_LOOKUP_SERVICE( face, service, GLYPH_DICT );
    ఒకవేళ ( service && service->get_name )
      error = service->get_name( face, glyph_సూచీ, బఫరు, బఫరు_max );
    లేకపోతే
      error = FT_THROW( Invalid_వాదం );

    తిరిగి error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( స్థిరరాశి అక్షరం* )
  FT_Get_Postscript_Name( FT_Face  face )
  {
    స్థిరరాశి అక్షరం*  ఫలితం = ఖాళీ;


    ఒకవేళ ( !face )
      వెళ్ళు Exit;

    ఒకవేళ ( !ఫలితం )
    {
      FT_Service_PsFontName  service;


      FT_FACE_LOOKUP_SERVICE( face,
                              service,
                              POSTSCRIPT_FONT_NAME );

      ఒకవేళ ( service && service->get_ps_font_name )
        ఫలితం = service->get_ps_font_name( face );
    }

  Exit:
    తిరిగి ఫలితం;
  }


  /* documentation is in tttables.h */

  FT_EXPORT_DEF( శూన్య* )
  FT_Get_Sfnt_పట్టిక( FT_Face      face,
                     FT_Sfnt_ట్యాగ్  ట్యాగ్ )
  {
    శూన్య*                  పట్టిక = ఖాళీ;
    FT_Service_SFNT_పట్టిక  service;


    ఒకవేళ ( face && FT_IS_SFNT( face ) )
    {
      FT_FACE_కనుగొను_SERVICE( face, service, SFNT_పట్టిక );
      ఒకవేళ ( service )
        పట్టిక = service->get_పట్టిక( face, ట్యాగ్ );
    }

    తిరిగి పట్టిక;
  }


  /* documentation is in tttables.h */

  FT_EXPORT_DEF( FT_Error )
  FT_ఎక్కించు_Sfnt_పట్టిక( FT_Face    face,
                      FT_ULong   ట్యాగ్,
                      FT_Long    స్థానభ్రంశం,
                      FT_Byte*   బఫరు,
                      FT_ULong*  నిడివి )
  {
    FT_Service_SFNT_పట్టిక  service;


    ఒకవేళ ( !face || !FT_IS_SFNT( face ) )
      తిరిగి FT_THROW( Invalid_Face_Handle );

    FT_FACE_కనుగొను_SERVICE( face, service, SFNT_పట్టిక );
    ఒకవేళ ( !service )
      తిరిగి FT_THROW( Unimplemented_Feature );

    తిరిగి service->ఎక్కించు_పట్టిక( face, ట్యాగ్, స్థానభ్రంశం, బఫరు, నిడివి );
  }


  /* documentation is in tttables.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Sfnt_పట్టిక_Info( FT_Face    face,
                      FT_UInt    పట్టిక_సూచీ,
                      FT_ULong  *ట్యాగ్,
                      FT_ULong  *నిడివి )
  {
    FT_Service_SFNT_పట్టిక  service;
    FT_ULong               స్థానభ్రంశం;


    /* test for valid `length' delayed to `service->table_info' */

    ఒకవేళ ( !face || !FT_IS_SFNT( face ) )
      తిరిగి FT_THROW( Invalid_Face_Handle );

    FT_FACE_కనుగొను_SERVICE( face, service, SFNT_పట్టిక );
    ఒకవేళ ( !service )
      తిరిగి FT_THROW( Unimplemented_Feature );

    తిరిగి service->పట్టిక_info( face, పట్టిక_సూచీ, ట్యాగ్, &స్థానభ్రంశం, నిడివి );
  }


  /* documentation is in tttables.h */

  FT_EXPORT_DEF( FT_ULong )
  FT_Get_CMap_Language_ID( FT_CharMap  charmap )
  {
    FT_Service_TTCMaps  service;
    FT_Face             face;
    TT_CMapInfo         cmap_info;


    ఒకవేళ ( !charmap || !charmap->face )
      తిరిగి 0;

    face = charmap->face;
    FT_FACE_కనుగొను_SERVICE( face, service, TT_CMAP );
    ఒకవేళ ( !service )
      తిరిగి 0;
    ఒకవేళ ( service->get_cmap_info( charmap, &cmap_info ))
      తిరిగి 0;

    తిరిగి cmap_info.language;
  }


  /* documentation is in tttables.h */

  FT_EXPORT_DEF( FT_Long )
  FT_Get_CMap_Format( FT_CharMap  charmap )
  {
    FT_Service_TTCMaps  service;
    FT_Face             face;
    TT_CMapInfo         cmap_info;


    ఒకవేళ ( !charmap || !charmap->face )
      తిరిగి -1;

    face = charmap->face;
    FT_FACE_కనుగొను_SERVICE( face, service, TT_CMAP );
    ఒకవేళ ( !service )
      తిరిగి -1;
    ఒకవేళ ( service->get_cmap_info( charmap, &cmap_info ))
      తిరిగి -1;

    తిరిగి cmap_info.format;
  }


  /* documentation is in ftsizes.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Activate_కొలత( FT_కొలత  కొలత )
  {
    FT_Face  face;


    ఒకవేళ ( !కొలత )
      తిరిగి FT_THROW( Invalid_కొలత_Handle );

    face = కొలత->face;
    ఒకవేళ ( !face || !face->driver )
      తిరిగి FT_THROW( Invalid_Face_Handle );

    /* we don't need anything more complex than that; all size objects */
    /* are already listed by the face                                  */
    face->కొలత = కొలత;

    తిరిగి FT_Err_Ok;
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
  FT_పునాది_DEF( FT_Renderer )
  FT_Lookup_Renderer( FT_Library       library,
                      FT_Glyph_Format  format,
                      FT_List_కణుపు*     కణుపు )
  {
    FT_List_కణుపు  cur;
    FT_Renderer  ఫలితం = ఖాళీ;


    ఒకవేళ ( !library )
      వెళ్ళు Exit;

    cur = library->renderers.తల;

    ఒకవేళ ( కణుపు )
    {
      ఒకవేళ ( *కణుపు )
        cur = (*కణుపు)->తదుపరి;
      *కణుపు = ఖాళీ;
    }

    అయితే ( cur )
    {
      FT_Renderer  renderer = FT_RENDERER( cur->దత్తాంశం );


      ఒకవేళ ( renderer->glyph_format == format )
      {
        ఒకవేళ ( కణుపు )
          *కణుపు = cur;

        ఫలితం = renderer;
        ఆపు;
      }
      cur = cur->తదుపరి;
    }

  Exit:
    తిరిగి ఫలితం;
  }


  స్థిర FT_Renderer
  ft_lookup_glyph_renderer( FT_Glyph_స్థానం  స్థానం )
  {
    FT_Face      face    = స్థానం->face;
    FT_Library   library = FT_FACE_LIBRARY( face );
    FT_Renderer  ఫలితం  = library->cur_renderer;


    ఒకవేళ ( !ఫలితం || ఫలితం->glyph_format != స్థానం->format )
      ఫలితం = FT_Lookup_Renderer( library, స్థానం->format, 0 );

    తిరిగి ఫలితం;
  }


  స్థిర శూన్య
  ft_సమితి_current_renderer( FT_Library  library )
  {
    FT_Renderer  renderer;


    renderer = FT_Lookup_Renderer( library, FT_GLYPH_FORMAT_OUTLINE, 0 );
    library->cur_renderer = renderer;
  }


  స్థిర FT_Error
  ft_కూడు_renderer( FT_Module  module )
  {
    FT_Library   library = module->library;
    FT_జ్ఞాపకం    జ్ఞాపకం  = library->జ్ఞాపకం;
    FT_Error     error;
    FT_List_కణుపు  కణుపు    = ఖాళీ;


    ఒకవేళ ( FT_QNEW( కణుపు ) )
      వెళ్ళు Exit;

    {
      FT_Renderer         render = FT_RENDERER( module );
      FT_Renderer_Class*  clazz  = (FT_Renderer_Class*)module->clazz;


      render->clazz        = clazz;
      render->glyph_format = clazz->glyph_format;

      /* allocate raster object if needed */
      ఒకవేళ ( clazz->raster_class && clazz->raster_class->raster_new )
      {
        error = clazz->raster_class->raster_new( జ్ఞాపకం, &render->raster );
        ఒకవేళ ( error )
          వెళ్ళు Fail;

        render->raster_render = clazz->raster_class->raster_render;
        render->render        = clazz->render_glyph;
      }

#నిర్వచితమైతే FT_CONFIG_OPTION_SVG
      ఒకవేళ ( clazz->glyph_format == FT_GLYPH_FORMAT_SVG )
        render->render = clazz->render_glyph;
#అయితే_ముగింపు

      /* add to list */
      కణుపు->దత్తాంశం = module;
      FT_List_కూడు( &library->renderers, కణుపు );

      ft_సమితి_current_renderer( library );
    }

  Fail:
    ఒకవేళ ( error )
      FT_FREE( కణుపు );

  Exit:
    తిరిగి error;
  }


  స్థిర శూన్య
  ft_remove_renderer( FT_Module  module )
  {
    FT_Library   library;
    FT_జ్ఞాపకం    జ్ఞాపకం;
    FT_List_కణుపు  కణుపు;


    library = module->library;
    ఒకవేళ ( !library )
      తిరిగి;

    జ్ఞాపకం = library->జ్ఞాపకం;

    కణుపు = FT_List_కనుగొను( &library->renderers, module );
    ఒకవేళ ( కణుపు )
    {
      FT_Renderer  render = FT_RENDERER( module );


      /* release raster object, if any */
      ఒకవేళ ( render->raster )
        render->clazz->raster_class->raster_పూర్తి( render->raster );

      /* remove from list */
      FT_List_Remove( &library->renderers, కణుపు );
      FT_FREE( కణుపు );

      ft_సమితి_current_renderer( library );
    }
  }


  /* documentation is in ftrender.h */

  FT_EXPORT_DEF( FT_Renderer )
  FT_Get_Renderer( FT_Library       library,
                   FT_Glyph_Format  format )
  {
    /* test for valid `library' delayed to `FT_Lookup_Renderer' */

    తిరిగి FT_Lookup_Renderer( library, format, 0 );
  }


  /* documentation is in ftrender.h */

  FT_EXPORT_DEF( FT_Error )
  FT_సమితి_Renderer( FT_Library     library,
                   FT_Renderer    renderer,
                   FT_UInt        అంకె_params,
                   FT_Parameter*  parameters )
  {
    FT_List_కణుపు  కణుపు;
    FT_Error     error = FT_Err_Ok;

    FT_Renderer_సమితి_బహుళకం_Func  సమితి_బహుళకం;


    ఒకవేళ ( !library )
    {
      error = FT_THROW( Invalid_Library_Handle );
      వెళ్ళు Exit;
    }

    ఒకవేళ ( !renderer )
    {
      error = FT_THROW( Invalid_వాదం );
      వెళ్ళు Exit;
    }

    ఒకవేళ ( అంకె_params > 0 && !parameters )
    {
      error = FT_THROW( Invalid_వాదం );
      వెళ్ళు Exit;
    }

    కణుపు = FT_List_కనుగొను( &library->renderers, renderer );
    ఒకవేళ ( !కణుపు )
    {
      error = FT_THROW( Invalid_వాదం );
      వెళ్ళు Exit;
    }

    FT_List_పైకి( &library->renderers, కణుపు );

    ఒకవేళ ( renderer->glyph_format == FT_GLYPH_FORMAT_OUTLINE )
      library->cur_renderer = renderer;

    సమితి_బహుళకం = renderer->clazz->సమితి_బహుళకం;

    కొరకు ( ; అంకె_params > 0; అంకె_params-- )
    {
      error = సమితి_బహుళకం( renderer, parameters->ట్యాగ్, parameters->దత్తాంశం );
      ఒకవేళ ( error )
        ఆపు;
      parameters++;
    }

  Exit:
    తిరిగి error;
  }


  FT_పునాది_DEF( FT_Error )
  FT_Render_Glyph_అంతర్గత( FT_Library      library,
                            FT_Glyph_స్థానం    స్థానం,
                            FT_Render_బహుళకం  render_బహుళకం )
  {
    FT_Error     error = FT_Err_Ok;
    FT_Face      face  = స్థానం->face;
    FT_Renderer  renderer;
    FT_List_కణుపు  కణుపు  = ఖాళీ;


    /* try to render colored glyph layers as a special case */
    ఒకవేళ ( స్థానం->అంతర్గత->ఎక్కించు_flags & FT_ఎక్కించు_COLOR &&
         స్థానం->format == FT_GLYPH_FORMAT_OUTLINE    )
    {
      FT_LayerIterator  iterator;

      FT_UInt  పునాది_glyph = స్థానం->glyph_సూచీ;

      FT_Bool  have_layers;
      FT_UInt  glyph_సూచీ;
      FT_UInt  color_సూచీ;


      iterator.p  = ఖాళీ;
      have_layers = FT_Get_Color_Glyph_Layer( face,
                                              పునాది_glyph,
                                              &glyph_సూచీ,
                                              &color_సూచీ,
                                              &iterator );
      ఒకవేళ ( have_layers )
      {
        error = FT_New_Glyph_స్థానం( face, ఖాళీ );
        ఒకవేళ ( !error )
        {
          TT_Face       ttface = (TT_Face)face;
          SFNT_Service  sfnt   = (SFNT_Service)ttface->sfnt;


          చేయి
          {
            FT_Int32  ఎక్కించు_flags = స్థానం->అంతర్గత->ఎక్కించు_flags;


            /* disable the `FT_LOAD_COLOR' flag to avoid recursion */
            /* right here in this function                         */
            ఎక్కించు_flags &= ~FT_ఎక్కించు_COLOR;

            /* render into the new `face->glyph' glyph slot */
            ఎక్కించు_flags |= FT_ఎక్కించు_RENDER | FT_ఎక్కించు_NO_BITMAP;

            error = FT_ఎక్కించు_Glyph( face, glyph_సూచీ, ఎక్కించు_flags );
            ఒకవేళ ( error )
              ఆపు;

            /* blend new `face->glyph' into old `slot'; */
            /* at the first call, `slot' is still empty */
            error = sfnt->colr_blend( ttface,
                                      color_సూచీ,
                                      స్థానం,
                                      face->glyph );
            ఒకవేళ ( error )
              ఆపు;

          } అయితే ( FT_Get_Color_Glyph_Layer( face,
                                              పునాది_glyph,
                                              &glyph_సూచీ,
                                              &color_సూచీ,
                                              &iterator ) );

          ఒకవేళ ( !error )
            స్థానం->format = FT_GLYPH_FORMAT_BITMAP;

          /* this call also restores `slot' as the glyph slot */
          FT_పూర్తి_Glyph_స్థానం( face->glyph );
        }

        ఒకవేళ ( !error )
          తిరిగి error;

        /* Failed to do the colored layer.  Draw outline instead. */
        స్థానం->format = FT_GLYPH_FORMAT_OUTLINE;
      }
    }

    /* small shortcut for the very common case */
    ఒకవేళ ( స్థానం->format == FT_GLYPH_FORMAT_OUTLINE )
    {
      renderer = library->cur_renderer;
      కణుపు     = library->renderers.తల;
    }
    లేకపోతే
      renderer = FT_Lookup_Renderer( library, స్థానం->format, &కణుపు );

    error = FT_ERR( Cannot_Render_Glyph );
    అయితే ( renderer )
    {
      error = renderer->render( renderer, స్థానం, render_బహుళకం, ఖాళీ );
      ఒకవేళ ( !error                                   ||
           FT_ERR_NEQ( error, Cannot_Render_Glyph ) )
        ఆపు;

      /* `FT_Err_Cannot_Render_Glyph` is returned if the render mode */
      /* is unsupported by the current renderer for this glyph image */
      /* format.                                                     */

      /* now, look for another renderer that supports the same */
      /* format.                                               */
      renderer = FT_Lookup_Renderer( library, స్థానం->format, &కణుపు );
    }

    /* it is not an error if we cannot render a bitmap glyph */
    ఒకవేళ ( FT_ERR_EQ( error, Cannot_Render_Glyph ) &&
         స్థానం->format == FT_GLYPH_FORMAT_BITMAP  )
      error = FT_Err_Ok;

#నిర్వచితమైతే FT_DEBUG_LEVEL_TRACE

#నిర్వచనం_రద్దు  FT_COMPONENT
#నిర్వచించు FT_COMPONENT  checksum

    /*
     * Computing the MD5 checksum is expensive, unnecessarily distorting a
     * possible profiling of FreeType if compiled with tracing support.  For
     * this reason, we execute the following code only if explicitly
     * requested.
     */

    /* we use FT_TRACE3 in this block */
    ఒకవేళ ( !error                               &&
         ft_trace_స్థాయిలు[trace_checksum] >= 3 &&
         స్థానం->bitmap.బఫరు                  )
    {
      FT_Bitmap  bitmap;
      FT_Error   err;


      FT_Bitmap_Init( &bitmap );

      /* we convert to a single bitmap format for computing the checksum */
      /* this also converts the bitmap flow to `down' (i.e., pitch > 0)  */
      err = FT_Bitmap_Convert( library, &స్థానం->bitmap, &bitmap, 1 );
      ఒకవేళ ( !err )
      {
        MD5_CTX        ctx;
        గుర్తులేని అక్షరం  md5[16];
        గుర్తులేని దీర్ఘ  coverage = 0;
        పూర్ణాంకం            i, j;
        పూర్ణాంకం            అడ్డువరుసలు  = (పూర్ణాంకం)bitmap.అడ్డువరుసలు;
        పూర్ణాంకం            pitch = bitmap.pitch;


        FT_TRACE3(( "FT_Render_Glyph: bitmap %dx%d, %s (mode %d)\n",
                    pitch,
                    అడ్డువరుసలు,
                    pixel_modes[స్థానం->bitmap.pixel_బహుళకం],
                    స్థానం->bitmap.pixel_బహుళకం ));

        కొరకు ( i = 0; i < అడ్డువరుసలు; i++ )
          కొరకు ( j = 0; j < pitch; j++ )
            coverage += bitmap.బఫరు[i * pitch + j];

        FT_TRACE3(( "  Total coverage: %lu\n", coverage ));

        MD5_Init( &ctx );
        ఒకవేళ ( bitmap.బఫరు )
          MD5_Update( &ctx, bitmap.బఫరు,
                      (గుర్తులేని దీర్ఘ)అడ్డువరుసలు * (గుర్తులేని దీర్ఘ)pitch );
        MD5_Final( md5, &ctx );

        FT_TRACE3(( "  MD5 checksum: " ));
        కొరకు ( i = 0; i < 16; i++ )
          FT_TRACE3(( "%02X", md5[i] ));
        FT_TRACE3(( "\n" ));
      }

      FT_Bitmap_పూర్తి( library, &bitmap );
    }

    /*
     * Dump bitmap in Netpbm format (PBM or PGM).
     */

    /* we use FT_TRACE7 in this block */
    ఒకవేళ ( !error                               &&
         ft_trace_స్థాయిలు[trace_checksum] >= 7 &&
         స్థానం->bitmap.బఫరు                  )
    {
      ఒకవేళ ( స్థానం->bitmap.అడ్డువరుసలు  < 128U &&
           స్థానం->bitmap.వెడల్పు < 128U )
      {
        పూర్ణాంకం  అడ్డువరుసలు  = (పూర్ణాంకం)స్థానం->bitmap.అడ్డువరుసలు;
        పూర్ణాంకం  వెడల్పు = (పూర్ణాంకం)స్థానం->bitmap.వెడల్పు;
        పూర్ణాంకం  pitch =      స్థానం->bitmap.pitch;
        పూర్ణాంకం  i, j, m;

        గుర్తులేని అక్షరం*  topleft = స్థానం->bitmap.బఫరు;


        ఒకవేళ ( pitch < 0 )
          topleft -= pitch * ( అడ్డువరుసలు - 1 );

        FT_TRACE7(( "Netpbm image: start\n" ));
        ఎంపిక ( స్థానం->bitmap.pixel_బహుళకం )
        {
        స్థితి FT_PIXEL_బహుళకం_MONO:
          FT_TRACE7(( "P1 %d %d\n", వెడల్పు, అడ్డువరుసలు ));
          కొరకు ( i = 0; i < అడ్డువరుసలు; i++ )
          {
            కొరకు ( j = 0; j < వెడల్పు; )
              కొరకు ( m = 128; m > 0 && j < వెడల్పు; m >>= 1, j++ )
                FT_TRACE7(( " %d",
                            ( topleft[i * pitch + j / 8] & m ) != 0 ));
            FT_TRACE7(( "\n" ));
          }
          ఆపు;

        అప్రమేయ:
          FT_TRACE7(( "P2 %d %d 255\n", వెడల్పు, అడ్డువరుసలు ));
          కొరకు ( i = 0; i < అడ్డువరుసలు; i++ )
          {
            కొరకు ( j = 0; j < వెడల్పు; j += 1 )
              FT_TRACE7(( " %3u", topleft[i * pitch + j] ));
            FT_TRACE7(( "\n" ));
          }
        }
        FT_TRACE7(( "Netpbm image: end\n" ));
      }
      లేకపోతే
        FT_TRACE7(( "Netpbm image: too large, omitted\n" ));
    }

#నిర్వచనం_రద్దు  FT_COMPONENT
#నిర్వచించు FT_COMPONENT  objs

#అయితే_ముగింపు /* FT_DEBUG_LEVEL_TRACE */

    తిరిగి error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Render_Glyph( FT_Glyph_స్థానం    స్థానం,
                   FT_Render_బహుళకం  render_బహుళకం )
  {
    FT_Library  library;


    ఒకవేళ ( !స్థానం || !స్థానం->face )
      తిరిగి FT_THROW( Invalid_వాదం );

    library = FT_FACE_LIBRARY( స్థానం->face );

    తిరిగి FT_Render_Glyph_అంతర్గత( library, స్థానం, render_బహుళకం );
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
  స్థిర శూన్య
  Destroy_Module( FT_Module  module )
  {
    స్థిరరాశి FT_Module_Class*  clazz   = module->clazz;
    FT_Library              library = module->library;
    FT_జ్ఞాపకం               జ్ఞాపకం  = module->జ్ఞాపకం;


    ఒకవేళ ( library && library->auto_hinter == module )
      library->auto_hinter = ఖాళీ;

    /* if the module is a renderer */
    ఒకవేళ ( FT_MODULE_IS_RENDERER( module ) )
      ft_remove_renderer( module );

    /* if the module is a font driver, add some steps */
    ఒకవేళ ( FT_MODULE_IS_DRIVER( module ) )
      Destroy_Driver( FT_DRIVER( module ) );

    /* finalize the module object */
    ఒకవేళ ( clazz->module_పూర్తి )
      clazz->module_పూర్తి( module );

    /* discard it */
    FT_FREE( module );
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_కూడు_Module( FT_Library              library,
                 స్థిరరాశి FT_Module_Class*  clazz )
  {
    FT_Error   error;
    FT_జ్ఞాపకం  జ్ఞాపకం;
    FT_Module  module = ఖాళీ;
    FT_UInt    nn;


#నిర్వచించు FREETYPE_VER_FIXED  ( ( (FT_Long)FREETYPE_MAJOR << 16 ) | \
                                FREETYPE_MINOR                  )

    ఒకవేళ ( !library )
      తిరిగి FT_THROW( Invalid_Library_Handle );

    ఒకవేళ ( !clazz )
      తిరిగి FT_THROW( Invalid_వాదం );

    /* check FreeType version */
    ఒకవేళ ( clazz->module_requires > FREETYPE_VER_FIXED )
      తిరిగి FT_THROW( Invalid_సంస్కరణ );

    /* look for a module with the same name in the library's table */
    కొరకు ( nn = 0; nn < library->అంకె_modules; nn++ )
    {
      module = library->modules[nn];
      ఒకవేళ ( ft_strcmp( module->clazz->module_name, clazz->module_name ) == 0 )
      {
        /* this installed module has the same name, compare their versions */
        ఒకవేళ ( clazz->module_సంస్కరణ <= module->clazz->module_సంస్కరణ )
          తిరిగి FT_THROW( క్రింది_Module_సంస్కరణ );

        /* remove the module from our list, then exit the loop to replace */
        /* it by our new version..                                        */
        FT_Remove_Module( library, module );
        ఆపు;
      }
    }

    జ్ఞాపకం = library->జ్ఞాపకం;
    error  = FT_Err_Ok;

    ఒకవేళ ( library->అంకె_modules >= FT_MAX_MODULES )
    {
      error = FT_THROW( Too_Many_Drivers );
      వెళ్ళు Exit;
    }

    /* allocate module object */
    ఒకవేళ ( FT_ALLOC( module, clazz->module_కొలత ) )
      వెళ్ళు Exit;

    /* base initialization */
    module->clazz   = clazz;
    module->library = library;
    module->జ్ఞాపకం  = జ్ఞాపకం;

    /* check whether the module is a renderer - this must be performed */
    /* before the normal module initialization                         */
    ఒకవేళ ( FT_MODULE_IS_RENDERER( module ) )
    {
      /* add to the renderers list */
      error = ft_కూడు_renderer( module );
      ఒకవేళ ( error )
        వెళ్ళు Fail;
    }

    /* is the module a auto-hinter? */
    ఒకవేళ ( FT_MODULE_IS_HINTER( module ) )
      library->auto_hinter = module;

    /* if the module is a font driver */
    ఒకవేళ ( FT_MODULE_IS_DRIVER( module ) )
    {
      FT_Driver  driver = FT_DRIVER( module );


      driver->clazz = (FT_Driver_Class)module->clazz;
    }

    ఒకవేళ ( clazz->module_init )
    {
      error = clazz->module_init( module );
      ఒకవేళ ( error )
        వెళ్ళు Fail;
    }

    /* add module to the library's table */
    library->modules[library->అంకె_modules++] = module;

  Exit:
    తిరిగి error;

  Fail:
    ఒకవేళ ( FT_MODULE_IS_RENDERER( module ) )
    {
      FT_Renderer  renderer = FT_RENDERER( module );


      ఒకవేళ ( renderer->clazz                                          &&
           renderer->clazz->glyph_format == FT_GLYPH_FORMAT_OUTLINE &&
           renderer->raster                                         )
        renderer->clazz->raster_class->raster_పూర్తి( renderer->raster );
    }

    FT_FREE( module );
    వెళ్ళు Exit;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Module )
  FT_Get_Module( FT_Library   library,
                 స్థిరరాశి అక్షరం*  module_name )
  {
    FT_Module   ఫలితం = ఖాళీ;
    FT_Module*  cur;
    FT_Module*  హద్దు;


    ఒకవేళ ( !library || !module_name )
      తిరిగి ఫలితం;

    cur   = library->modules;
    హద్దు = cur + library->అంకె_modules;

    కొరకు ( ; cur < హద్దు; cur++ )
      ఒకవేళ ( ft_strcmp( cur[0]->clazz->module_name, module_name ) == 0 )
      {
        ఫలితం = cur[0];
        ఆపు;
      }

    తిరిగి ఫలితం;
  }


  /* documentation is in ftobjs.h */

  FT_పునాది_DEF( స్థిరరాశి శూన్య* )
  FT_Get_Module_Interface( FT_Library   library,
                           స్థిరరాశి అక్షరం*  mod_name )
  {
    FT_Module  module;


    /* test for valid `library' delayed to FT_Get_Module() */

    module = FT_Get_Module( library, mod_name );

    తిరిగి module ? module->clazz->module_interface : 0;
  }


  FT_పునాది_DEF( FT_సూచిక )
  ft_module_get_service( FT_Module    module,
                         స్థిరరాశి అక్షరం*  service_id,
                         FT_Bool      global )
  {
    FT_సూచిక  ఫలితం = ఖాళీ;


    ఒకవేళ ( module )
    {
      FT_ASSERT( module->clazz && module->clazz->get_interface );

      /* first, look for the service in the module */
      ఒకవేళ ( module->clazz->get_interface )
        ఫలితం = module->clazz->get_interface( module, service_id );

      ఒకవేళ ( global && !ఫలితం )
      {
        /* we didn't find it, look in all other modules then */
        FT_Library  library = module->library;
        FT_Module*  cur     = library->modules;
        FT_Module*  హద్దు   = cur + library->అంకె_modules;


        కొరకు ( ; cur < హద్దు; cur++ )
        {
          ఒకవేళ ( cur[0] != module )
          {
            FT_ASSERT( cur[0]->clazz );

            ఒకవేళ ( cur[0]->clazz->get_interface )
            {
              ఫలితం = cur[0]->clazz->get_interface( cur[0], service_id );
              ఒకవేళ ( ఫలితం )
                ఆపు;
            }
          }
        }
      }
    }

    తిరిగి ఫలితం;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Remove_Module( FT_Library  library,
                    FT_Module   module )
  {
    /* try to find the module from the table, then remove it from there */

    ఒకవేళ ( !library )
      తిరిగి FT_THROW( Invalid_Library_Handle );

    ఒకవేళ ( module )
    {
      FT_Module*  cur   = library->modules;
      FT_Module*  హద్దు = cur + library->అంకె_modules;


      కొరకు ( ; cur < హద్దు; cur++ )
      {
        ఒకవేళ ( cur[0] == module )
        {
          /* remove it from the table */
          library->అంకె_modules--;
          హద్దు--;
          అయితే ( cur < హద్దు )
          {
            cur[0] = cur[1];
            cur++;
          }
          హద్దు[0] = ఖాళీ;

          /* destroy the module */
          Destroy_Module( module );

          తిరిగి FT_Err_Ok;
        }
      }
    }
    తిరిగి FT_THROW( Invalid_Driver_Handle );
  }


  స్థిర FT_Error
  ft_property_do( FT_Library        library,
                  స్థిరరాశి FT_String*  module_name,
                  స్థిరరాశి FT_String*  property_name,
                  శూన్య*             విలువ,
                  FT_Bool           సమితి,
                  FT_Bool           విలువ_is_string )
  {
    FT_Module*           cur;
    FT_Module*           హద్దు;
    FT_Module_Interface  interface;

    FT_Service_Properties  service;

#నిర్వచితమైతే FT_DEBUG_LEVEL_ERROR
    స్థిరరాశి FT_String*  సమితి_name  = "FT_Property_Set";
    స్థిరరాశి FT_String*  get_name  = "FT_Property_Get";
    స్థిరరాశి FT_String*  func_name = సమితి ? సమితి_name : get_name;
#అయితే_ముగింపు

    FT_Bool  missing_func;


    ఒకవేళ ( !library )
      తిరిగి FT_THROW( Invalid_Library_Handle );

    ఒకవేళ ( !module_name || !property_name || !విలువ )
      తిరిగి FT_THROW( Invalid_వాదం );

    cur   = library->modules;
    హద్దు = cur + library->అంకె_modules;

    /* search module */
    కొరకు ( ; cur < హద్దు; cur++ )
      ఒకవేళ ( !ft_strcmp( cur[0]->clazz->module_name, module_name ) )
        ఆపు;

    ఒకవేళ ( cur == హద్దు )
    {
      FT_TRACE2(( "%s: can't find module `%s'\n",
                  func_name, module_name ));
      తిరిగి FT_THROW( Missing_Module );
    }

    /* check whether we have a service interface */
    ఒకవేళ ( !cur[0]->clazz->get_interface )
    {
      FT_TRACE2(( "%s: module `%s' doesn't support properties\n",
                  func_name, module_name ));
      తిరిగి FT_THROW( Unimplemented_Feature );
    }

    /* search property service */
    interface = cur[0]->clazz->get_interface( cur[0],
                                              FT_SERVICE_ID_PROPERTIES );
    ఒకవేళ ( !interface )
    {
      FT_TRACE2(( "%s: module `%s' doesn't support properties\n",
                  func_name, module_name ));
      తిరిగి FT_THROW( Unimplemented_Feature );
    }

    service = (FT_Service_Properties)interface;

    ఒకవేళ ( సమితి )
      missing_func = FT_BOOL( !service->సమితి_property );
    లేకపోతే
      missing_func = FT_BOOL( !service->get_property );

    ఒకవేళ ( missing_func )
    {
      FT_TRACE2(( "%s: property service of module `%s' is broken\n",
                  func_name, module_name ));
      తిరిగి FT_THROW( Unimplemented_Feature );
    }

    తిరిగి సమితి ? service->సమితి_property( cur[0],
                                        property_name,
                                        విలువ,
                                        విలువ_is_string )
               : service->get_property( cur[0],
                                        property_name,
                                        విలువ );
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Property_సమితి( FT_Library        library,
                   స్థిరరాశి FT_String*  module_name,
                   స్థిరరాశి FT_String*  property_name,
                   స్థిరరాశి శూన్య*       విలువ )
  {
    తిరిగి ft_property_do( library,
                           module_name,
                           property_name,
                           (శూన్య*)విలువ,
                           TRUE,
                           FALSE );
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Property_Get( FT_Library        library,
                   స్థిరరాశి FT_String*  module_name,
                   స్థిరరాశి FT_String*  property_name,
                   శూన్య*             విలువ )
  {
    తిరిగి ft_property_do( library,
                           module_name,
                           property_name,
                           విలువ,
                           FALSE,
                           FALSE );
  }


#నిర్వచితమైతే FT_CONFIG_OPTION_ENVIRONMENT_PROPERTIES

  /* this variant is used for handling the FREETYPE_PROPERTIES */
  /* environment variable                                      */

  FT_పునాది_DEF( FT_Error )
  ft_property_string_సమితి( FT_Library        library,
                          స్థిరరాశి FT_String*  module_name,
                          స్థిరరాశి FT_String*  property_name,
                          FT_String*        విలువ )
  {
    తిరిగి ft_property_do( library,
                           module_name,
                           property_name,
                           (శూన్య*)విలువ,
                           TRUE,
                           TRUE );
  }

#అయితే_ముగింపు


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
    ఒకవేళ ( !library )
      తిరిగి FT_THROW( Invalid_Library_Handle );

    library->refcount++;

    తిరిగి FT_Err_Ok;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_New_Library( FT_జ్ఞాపకం    జ్ఞాపకం,
                  FT_Library  *alibrary )
  {
    FT_Library  library = ఖాళీ;
    FT_Error    error;


    ఒకవేళ ( !జ్ఞాపకం || !alibrary )
      తిరిగి FT_THROW( Invalid_వాదం );

#నిర్వచితం_కాకపోతే FT_DEBUG_LOGGING
#నిర్వచితమైతే FT_DEBUG_LEVEL_ERROR
    /* init debugging support */
    ft_debug_init();
#అయితే_ముగింపు /* FT_DEBUG_LEVEL_ERROR */
#అయితే_ముగింపు /* !FT_DEBUG_LOGGING */

    /* first of all, allocate the library object */
    ఒకవేళ ( FT_NEW( library ) )
      తిరిగి error;

    library->జ్ఞాపకం = జ్ఞాపకం;

    library->సంస్కరణ_major = FREETYPE_MAJOR;
    library->సంస్కరణ_minor = FREETYPE_MINOR;
    library->సంస్కరణ_patch = FREETYPE_PATCH;

    library->refcount = 1;

    /* That's ok now */
    *alibrary = library;

    తిరిగి FT_Err_Ok;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( శూన్య )
  FT_Library_సంస్కరణ( FT_Library   library,
                      FT_Int      *amajor,
                      FT_Int      *aminor,
                      FT_Int      *apatch )
  {
    FT_Int  major = 0;
    FT_Int  minor = 0;
    FT_Int  patch = 0;


    ఒకవేళ ( library )
    {
      major = library->సంస్కరణ_major;
      minor = library->సంస్కరణ_minor;
      patch = library->సంస్కరణ_patch;
    }

    ఒకవేళ ( amajor )
      *amajor = major;

    ఒకవేళ ( aminor )
      *aminor = minor;

    ఒకవేళ ( apatch )
      *apatch = patch;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_Error )
  FT_పూర్తి_Library( FT_Library  library )
  {
    FT_జ్ఞాపకం  జ్ఞాపకం;


    ఒకవేళ ( !library )
      తిరిగి FT_THROW( Invalid_Library_Handle );

    library->refcount--;
    ఒకవేళ ( library->refcount > 0 )
      వెళ్ళు Exit;

    జ్ఞాపకం = library->జ్ఞాపకం;

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
      స్థిరరాశి అక్షరం*  driver_name[] = { "type42", ఖాళీ };


      కొరకు ( m = 0;
            m < పరిమాణం ( driver_name ) / పరిమాణం ( driver_name[0] );
            m++ )
      {
        కొరకు ( n = 0; n < library->అంకె_modules; n++ )
        {
          FT_Module    module      = library->modules[n];
          స్థిరరాశి అక్షరం*  module_name = module->clazz->module_name;
          FT_List      faces;


          ఒకవేళ ( driver_name[m]                                &&
               ft_strcmp( module_name, driver_name[m] ) != 0 )
            కొనసాగు;

          ఒకవేళ ( ( module->clazz->module_flags & FT_MODULE_FONT_DRIVER ) == 0 )
            కొనసాగు;

          FT_TRACE7(( "FT_Done_Library: close faces for %s\n", module_name ));

          faces = &FT_DRIVER( module )->faces_list;
          అయితే ( faces->తల )
          {
            FT_పూర్తి_Face( FT_FACE( faces->తల->దత్తాంశం ) );
            ఒకవేళ ( faces->తల )
              FT_TRACE0(( "FT_Done_Library: failed to free some faces\n" ));
          }
        }
      }
    }

    /* Close all other modules in the library */
#ఒకవేళ 1
    /* XXX Modules are removed in the reversed order so that  */
    /* type42 module is removed before truetype module.  This */
    /* avoids double free in some occasions.  It is a hack.   */
    అయితే ( library->అంకె_modules > 0 )
      FT_Remove_Module( library,
                        library->modules[library->అంకె_modules - 1] );
#లేకపోతే
    {
      FT_UInt  n;


      కొరకు ( n = 0; n < library->అంకె_modules; n++ )
      {
        FT_Module  module = library->modules[n];


        ఒకవేళ ( module )
        {
          Destroy_Module( module );
          library->modules[n] = ఖాళీ;
        }
      }
    }
#అయితే_ముగింపు

    FT_FREE( library );

  Exit:
    తిరిగి FT_Err_Ok;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( శూన్య )
  FT_సమితి_Debug_Hook( FT_Library         library,
                     FT_UInt            hook_సూచీ,
                     FT_DebugHook_Func  debug_hook )
  {
    ఒకవేళ ( library && debug_hook &&
         hook_సూచీ <
           ( పరిమాణం ( library->debug_hooks ) / పరిమాణం ( శూన్య* ) ) )
      library->debug_hooks[hook_సూచీ] = debug_hook;
  }


  /* documentation is in ftmodapi.h */

  FT_EXPORT_DEF( FT_TrueTypeEngineType )
  FT_Get_TrueType_Engine_Type( FT_Library  library )
  {
    FT_TrueTypeEngineType  ఫలితం = FT_TRUETYPE_ENGINE_TYPE_NONE;


    ఒకవేళ ( library )
    {
      FT_Module  module = FT_Get_Module( library, "truetype" );


      ఒకవేళ ( module )
      {
        FT_Service_TrueTypeEngine  service;


        service = (FT_Service_TrueTypeEngine)
                    ft_module_get_service( module,
                                           FT_SERVICE_ID_TRUETYPE_ENGINE,
                                           0 );
        ఒకవేళ ( service )
          ఫలితం = service->engine_type;
      }
    }

    తిరిగి ఫలితం;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_SubGlyph_Info( FT_Glyph_స్థానం  glyph,
                        FT_UInt       sub_సూచీ,
                        FT_Int       *p_సూచీ,
                        FT_UInt      *p_flags,
                        FT_Int       *p_arg1,
                        FT_Int       *p_arg2,
                        FT_మాతృక    *p_transform )
  {
    FT_Error  error = FT_ERR( Invalid_వాదం );


    ఒకవేళ ( glyph                                      &&
         glyph->subglyphs                           &&
         glyph->format == FT_GLYPH_FORMAT_సంయుక్త &&
         sub_సూచీ < glyph->అంకె_subglyphs           )
    {
      FT_SubGlyph  subg = glyph->subglyphs + sub_సూచీ;


      *p_సూచీ     = subg->index;
      *p_flags     = subg->flags;
      *p_arg1      = subg->arg1;
      *p_arg2      = subg->arg2;
      *p_transform = subg->transform;

      error = FT_Err_Ok;
    }

    తిరిగి error;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Get_Color_Glyph_Layer( FT_Face            face,
                            FT_UInt            పునాది_glyph,
                            FT_UInt           *aglyph_సూచీ,
                            FT_UInt           *acolor_సూచీ,
                            FT_LayerIterator*  iterator )
  {
    TT_Face       ttface;
    SFNT_Service  sfnt;


    ఒకవేళ ( !face                                   ||
         !aglyph_సూచీ                           ||
         !acolor_సూచీ                           ||
         !iterator                               ||
         పునాది_glyph >= (FT_UInt)face->అంకె_glyphs )
      తిరిగి 0;

    ఒకవేళ ( !FT_IS_SFNT( face ) )
      తిరిగి 0;

    ttface = (TT_Face)face;
    sfnt   = (SFNT_Service)ttface->sfnt;

    ఒకవేళ ( sfnt->get_colr_layer )
      తిరిగి sfnt->get_colr_layer( ttface,
                                   పునాది_glyph,
                                   aglyph_సూచీ,
                                   acolor_సూచీ,
                                   iterator );
    లేకపోతే
      తిరిగి 0;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Get_Color_Glyph_Paint( FT_Face                  face,
                            FT_UInt                  పునాది_glyph,
                            FT_Color_మూలం_Transform  మూలం_transform,
                            FT_OpaquePaint*          paint )
  {
    TT_Face       ttface;
    SFNT_Service  sfnt;


    ఒకవేళ ( !face || !paint )
      తిరిగి 0;

    ఒకవేళ ( !FT_IS_SFNT( face ) )
      తిరిగి 0;

    ttface = (TT_Face)face;
    sfnt   = (SFNT_Service)ttface->sfnt;

    ఒకవేళ ( sfnt->get_colr_glyph_paint )
      తిరిగి sfnt->get_colr_glyph_paint( ttface,
                                         పునాది_glyph,
                                         మూలం_transform,
                                         paint );
    లేకపోతే
      తిరిగి 0;
  }


  /* documentation is in ftcolor.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Get_Color_Glyph_ClipBox( FT_Face      face,
                              FT_UInt      పునాది_glyph,
                              FT_ClipBox*  clip_box )
  {
    TT_Face       ttface;
    SFNT_Service  sfnt;


    ఒకవేళ ( !face || !clip_box )
      తిరిగి 0;

    ఒకవేళ ( !FT_IS_SFNT( face ) )
      తిరిగి 0;

    ttface = (TT_Face)face;
    sfnt   = (SFNT_Service)ttface->sfnt;

    ఒకవేళ ( sfnt->get_color_glyph_clipbox )
      తిరిగి sfnt->get_color_glyph_clipbox( ttface,
                                            పునాది_glyph,
                                            clip_box );
    లేకపోతే
      తిరిగి 0;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Get_Paint_Layers( FT_Face            face,
                       FT_LayerIterator*  layer_iterator,
                       FT_OpaquePaint*    paint )
  {
    TT_Face       ttface;
    SFNT_Service  sfnt;


    ఒకవేళ ( !face || !paint || !layer_iterator )
      తిరిగి 0;

    ఒకవేళ ( !FT_IS_SFNT( face ) )
      తిరిగి 0;

    ttface = (TT_Face)face;
    sfnt   = (SFNT_Service)ttface->sfnt;

    ఒకవేళ ( sfnt->get_paint_layers )
      తిరిగి sfnt->get_paint_layers( ttface, layer_iterator, paint );
    లేకపోతే
      తిరిగి 0;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Get_Paint( FT_Face face,
                FT_OpaquePaint  opaque_paint,
                FT_COLR_Paint*  paint )
  {
    TT_Face       ttface;
    SFNT_Service  sfnt;


    ఒకవేళ ( !face || !paint )
      తిరిగి 0;

    ఒకవేళ ( !FT_IS_SFNT( face ) )
      తిరిగి 0;

    ttface = (TT_Face)face;
    sfnt   = (SFNT_Service)ttface->sfnt;

    ఒకవేళ ( sfnt->get_paint )
      తిరిగి sfnt->get_paint( ttface, opaque_paint, paint );
    లేకపోతే
      తిరిగి 0;
  }


  /* documentation is in freetype.h */

  FT_EXPORT_DEF( FT_Bool )
  FT_Get_Colorline_Stops ( FT_Face                face,
                           FT_ColorStop *         color_stop,
                           FT_ColorStopIterator  *iterator )
  {
    TT_Face       ttface;
    SFNT_Service  sfnt;


    ఒకవేళ ( !face || !color_stop || !iterator )
      తిరిగి 0;

    ఒకవేళ ( !FT_IS_SFNT( face ) )
      తిరిగి 0;

    ttface = (TT_Face)face;
    sfnt   = (SFNT_Service)ttface->sfnt;

    ఒకవేళ ( sfnt->get_colorline_stops )
      తిరిగి sfnt->get_colorline_stops ( ttface, color_stop, iterator );
    లేకపోతే
      తిరిగి 0;
  }


/*చివర*/
