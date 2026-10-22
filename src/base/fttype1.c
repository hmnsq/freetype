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


#అన్తర్భావ <freetype/internal/ftdebug.h>
#అన్తర్భావ <freetype/internal/ftobjs.h>
#అన్తర్భావ <freetype/internal/ftserv.h>
#అన్తర్భావ <freetype/internal/services/svpsinfo.h>


  /* documentation is in t1tables.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_PS_Font_Info( FT_Face          face,
                       PS_FontInfoRec*  afont_info )
  {
    FT_Error           error;
    FT_Service_PsInfo  service;


    ఒకవేళ ( !face )
      తిరిగి FT_THROW( Invalid_Face_Handle );

    ఒకవేళ ( !afont_info )
      తిరిగి FT_THROW( Invalid_వాదం );

    FT_FACE_కనుగొను_SERVICE( face, service, POSTSCRIPT_INFO );

    ఒకవేళ ( service && service->ps_get_font_info )
      error = service->ps_get_font_info( face, afont_info );
    లేకపోతే
      error = FT_THROW( Invalid_వాదం );

    తిరిగి error;
  }


  /* documentation is in t1tables.h */

  FT_EXPORT_DEF( FT_Int )
  FT_Has_PS_Glyph_పేర్లు( FT_Face  face )
  {
    FT_Int             ఫలితం = 0;
    FT_Service_PsInfo  service;


    ఒకవేళ ( face )
    {
      FT_FACE_కనుగొను_SERVICE( face, service, POSTSCRIPT_INFO );

      ఒకవేళ ( service && service->ps_has_glyph_పేర్లు )
        ఫలితం = service->ps_has_glyph_పేర్లు( face );
    }

    తిరిగి ఫలితం;
  }


  /* documentation is in t1tables.h */

  FT_EXPORT_DEF( FT_Error )
  FT_Get_PS_Font_వ్యక్తిగత( FT_Face         face,
                          PS_వ్యక్తిగత_Rec*  afont_వ్యక్తిగత )
  {
    FT_Error           error;
    FT_Service_PsInfo  service;


    ఒకవేళ ( !face )
      తిరిగి FT_THROW( Invalid_Face_Handle );

    ఒకవేళ ( !afont_వ్యక్తిగత )
      తిరిగి FT_THROW( Invalid_వాదం );

    FT_FACE_కనుగొను_SERVICE( face, service, POSTSCRIPT_INFO );

    ఒకవేళ ( service && service->ps_get_font_వ్యక్తిగత )
      error = service->ps_get_font_వ్యక్తిగత( face, afont_వ్యక్తిగత );
    లేకపోతే
      error = FT_THROW( Invalid_వాదం );

    తిరిగి error;
  }


  /* documentation is in t1tables.h */

  FT_EXPORT_DEF( FT_Long )
  FT_Get_PS_Font_విలువ( FT_Face       face,
                        PS_Dict_కీలకాలు  కీలకం,
                        FT_UInt       idx,
                        శూన్య         *విలువ,
                        FT_Long       విలువ_len )
  {
    FT_Int             ఫలితం  = 0;
    FT_Service_PsInfo  service = ఖాళీ;


    ఒకవేళ ( face )
    {
      FT_FACE_కనుగొను_SERVICE( face, service, POSTSCRIPT_INFO );

      ఒకవేళ ( service && service->ps_get_font_విలువ )
        ఫలితం = service->ps_get_font_విలువ( face, కీలకం, idx,
                                             విలువ, విలువ_len );
    }

    తిరిగి ఫలితం;
  }


/*చివర*/
