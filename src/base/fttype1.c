/****************************************************************************
 *
 * fttype1.c
 *
 *   FreeType utility file for PS names support (body).
 *
 * Copyright (C) 2002-2026 by
 * David Turner, Robert Wilhelm, and Werner Lemberg.
 *
 * This file is part of the FreeType project, and may only be used,
 * modified, and distributed under the terms of the FreeType project
 * license, LICENSE.TXT.  By continuing to use, modify, or distribute
 * this file you indicate that you have read the license and
 * understand and accept it fully.
 *
 */


#અન્તર્ભાવ <freetype/internal/ftdebug.h>
#અન્તર્ભાવ <freetype/internal/ftobjs.h>
#અન્તર્ભાવ <freetype/internal/ftserv.h>
#અન્તર્ભાવ <freetype/internal/services/svpsinfo.h>


  /* documentation is in t1tables.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_PS_Font_Info( FT_Face          face,
                       PS_FontInfoRec*  afont_info )
  {
    FT_Error           error;
    FT_Service_PsInfo  service;


    જો ( !face )
      પાછા FT_THROW( Invalid_Face_Handle );

    જો ( !afont_info )
      પાછા FT_THROW( Invalid_Argument );

    FT_FACE_FIND_SERVICE( face, service, POSTSCRIPT_INFO );

    જો ( service && service->ps_get_font_info )
      error = service->ps_get_font_info( face, afont_info );
    નહીંતર
      error = FT_THROW( Invalid_Argument );

    પાછા error;
  }


  /* documentation is in t1tables.h */

  FT_EXPORT_DEF( FT_Int )
  FT_Has_PS_Glyph_Names( FT_Face  face )
  {
    FT_Int             result = 0;
    FT_Service_PsInfo  service;


    જો ( face )
    {
      FT_FACE_FIND_SERVICE( face, service, POSTSCRIPT_INFO );

      જો ( service && service->ps_has_glyph_names )
        result = service->ps_has_glyph_names( face );
    }

    પાછા result;
  }


  /* documentation is in t1tables.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_PS_Font_Private( FT_Face         face,
                          PS_PrivateRec*  afont_private )
  {
    FT_Error           error;
    FT_Service_PsInfo  service;


    જો ( !face )
      પાછા FT_THROW( Invalid_Face_Handle );

    જો ( !afont_private )
      પાછા FT_THROW( Invalid_Argument );

    FT_FACE_FIND_SERVICE( face, service, POSTSCRIPT_INFO );

    જો ( service && service->ps_get_font_private )
      error = service->ps_get_font_private( face, afont_private );
    નહીંતર
      error = FT_THROW( Invalid_Argument );

    પાછા error;
  }


  /* documentation is in t1tables.h */

  FT_EXPORT_DEF( FT_Long )
  FT_Get_PS_Font_કિંમત( FT_Face       face,
                        PS_Dict_ચાવીઓ  ચાવી,
                        FT_UInt       idx,
                        શૂન્ય         *કિંમત,
                        FT_Long       કિંમત_len )
  {
    FT_Int             result  = 0;
    FT_Service_PsInfo  service = ખાલી;


    જો ( face )
    {
      FT_FACE_FIND_SERVICE( face, service, POSTSCRIPT_INFO );

      જો ( service && service->ps_get_font_કિંમત )
        result = service->ps_get_font_કિંમત( face, ચાવી, idx,
                                             કિંમત, કિંમત_len );
    }

    પાછા result;
  }


/* END */
