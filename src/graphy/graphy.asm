;-------------------------------------------------------------------------------
;include '../include/library.inc'
;;-------------------------------------------------------------------------------
;
;library FONTLIBC, 99 ; unsure what this should be
;
;include '../include/include_library.inc'
;
;include_library '../graphx/graphx.asm'
;
;;-------------------------------------------------------------------------------
;; functions
;;-------------------------------------------------------------------------------
;
;export gfy_get_CurrentBuffer
;export gfy_set_CurrentBuffer
;export gfy_get_Color
;export gfy_set_Color
;export gfy_get_Transparent_Color
;export gfy_set_Transparent_Color
;export gfy_get_Text_FG_Color
;export gfy_set_Text_FG_Color
;export gfy_get_Text_BG_Color
;export gfy_set_Text_BG_Color
;export gfy_get_Text_TP_Color
;export gfy_set_Text_TP_Color
;export gfy_get_CharSpacing
;export gfy_set_CharSpacing
;export gfy_get_TextData
;export gfy_set_TextData
;export gfy_get_TextXPos
;export gfy_set_TextXPos
;export gfy_get_TextYPos
;export gfy_set_TextYPos
;export gfy_get_TextWidthScale
;export gfy_set_TextWidthScale
;export gfy_get_TextHeightScale
;export gfy_set_TextHeightScale
;export gfy_get_PrintChar_Clip
;export gfy_set_PrintChar_Clip
;export gfy_get_ClipXMin
;export gfy_set_ClipXMin
;export gfy_get_ClipYMin
;export gfy_set_ClipYMin
;export gfy_get_ClipXMax
;export gfy_set_ClipXMax
;export gfy_get_ClipYMax
;export gfy_set_ClipYMax
;
;
;
;
;
;
;;-------------------------------------------------------------------------------
;gfy_get_CurrentBuffer:
;	ld hl,(CurrentBuffer)
;	ret
;
;;-------------------------------------------------------------------------------
;gfy_set_CurrentBuffer:
;
;	ret
;
;;-------------------------------------------------------------------------------
;gfy_get_Color:
;	ld	l,(_Color)
;	ret
;
;;-------------------------------------------------------------------------------
;gfy_set_Color:
;	setSmcBytesFast _Color
;
;;-------------------------------------------------------------------------------
;gfy_get_Transparent_Color:
;	setSmcBytes _TextTPColor
;	ret
;
;;-------------------------------------------------------------------------------
;gfy_set_Transparent_Color:
;	setSmcBytes _TextTPColor
;
;;-------------------------------------------------------------------------------
;gfy_get_Text_FG_Color:
;	ld	l,(_TextFGColor)
;	ret
;
;;-------------------------------------------------------------------------------
;gfy_set_Text_FG_Color:
;	setSmcBytes _TextFGColor
;	ret
;
;;-------------------------------------------------------------------------------
;gfy_get_Text_BG_Color:
;	ld	l,(_Text_BG_Color)
;	ret
;
;;-------------------------------------------------------------------------------
;gfy_set_Text_BG_Color:
;	setSmcBytes _TextBGColor
;	ret
;
;;-------------------------------------------------------------------------------
;gfy_get_Text_TP_Color:
;	ld	l,(_Text_TP_Color)
;	ret
;
;;-------------------------------------------------------------------------------
;gfy_set_Text_TP_Color:
;	setSmcBytes _TextTPColor
;	ret
;
;;-------------------------------------------------------------------------------
;gfy_get_CharSpacing:
;	ld hl,(_CharSpacing)
;	ret
;
;;-------------------------------------------------------------------------------
;gfy_set_CharSpacing:
;
;	ret
;
;;-------------------------------------------------------------------------------
;gfy_get_TextData:
;	ld hl,(_TextData)
;	ret
;
;;-------------------------------------------------------------------------------
;gfy_set_TextData:
;
;	ret
;
;;-------------------------------------------------------------------------------
;gfy_get_TextXPos:
;	ld hl,(_TextXPos)
;	ret
;
;;-------------------------------------------------------------------------------
;gfy_set_TextXPos:
;	ret
;
;;-------------------------------------------------------------------------------
;gfy_get_TextYPos:
;	ld hl,(_TextYPos)
;	ret
;
;;-------------------------------------------------------------------------------
;gfy_set_TextYPos:
;	
;	ret
;
;;-------------------------------------------------------------------------------
;gfy_get_TextWidthScale:
;	ld	l,(_TextHeightScale)
;	ret
;
;;-------------------------------------------------------------------------------
;gfy_set_TextWidthScale:
;	setSmcBytes _TextWidthScale
;	ret
;
;;-------------------------------------------------------------------------------
;gfy_get_TextHeightScale:
;	ld	l,(_TextHeightScale)
;	ret
;
;;-------------------------------------------------------------------------------
;gfy_set_TextHeightScale:
;	setSmcBytes _TextHeightScale
;	ret
;
;;-------------------------------------------------------------------------------
;gfy_get_PrintChar_Clip:
;	ld	l,(_Text_TP_Color)
;	ret
;
;;-------------------------------------------------------------------------------
;gfy_set_PrintChar_Clip:
;	setSmcBytes _PrintChar_Clip
;	ret
;
;;-------------------------------------------------------------------------------
;gfy_get_ClipXMin:
;	ld	hl,(_XMin)
;	ret
;
;;-------------------------------------------------------------------------------
;gfy_set_ClipXMin:
;	
;	ret
;
;;-------------------------------------------------------------------------------
;gfy_get_ClipYMin:
;	ld	hl,(_YMin)
;	ret
;
;;-------------------------------------------------------------------------------
;gfy_set_ClipYMin:
;
;	ret
;
;;-------------------------------------------------------------------------------
;gfy_get_ClipXMax:
;	ld	hl,(_XMax)
;	ret
;
;;-------------------------------------------------------------------------------
;gfy_set_ClipXMax:
;
;
;
;	ret
;
;;-------------------------------------------------------------------------------
;gfy_get_ClipYMax:
;	ld	hl,(_YMax)
;	ret
;
;;-------------------------------------------------------------------------------
;gfy_set_ClipYMax:
;
;
;
;	ret