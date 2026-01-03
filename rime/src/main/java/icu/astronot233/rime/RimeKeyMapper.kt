package icu.astronot233.rime

import android.view.KeyEvent
import icu.astronot233.rime.X11Keys.*

const val ShiftMask   = 0x0001
const val LockMask    = 0x0002
const val ControlMask = 0x0004
const val AltMask     = 0x0008

const val Mod1Mask    = 0x0008
const val Mod2Mask    = 0x0010
const val Mod3Mask    = 0x0020
const val Mod4Mask    = 0x0040
const val Mod5Mask    = 0x0080

const val Button1Mask = 0x0100
const val Button2Mask = 0x0200
const val Button3Mask = 0x0400
const val Button4Mask = 0x0800
const val Button5Mask = 0x1000

const val HandledMask = 0x01000000
const val ForwardMask = 0x02000000

const val IgnoredMask = 0x02000000
const val SuperMask   = 0x04000000
const val HyperMask   = 0x08000000
const val MetaMask    = 0x10000000
const val ReleaseMask = 0x20000000

const val ModifierMask = 0x5f001fff

object KeyMapper {

    fun toX11Key(codePoint: Int) = when (codePoint) {
        '\u0000'.code -> XK_VoidSymbol
        '\b'    .code -> XK_BackSpace
        '\t'    .code -> XK_Tab
        '\n'    .code -> XK_Linefeed
        '\r'    .code -> XK_Return
        else -> codePoint
    }

    fun toRimeStyled(keyEvent: KeyEvent): Pair<Int, Int> {
        val uniChar = keyEvent.getUnicodeChar(keyEvent.metaState)
        val x11Code = when (uniChar) {
            0 -> keycodeMap[keyEvent.keyCode] ?: XK_VoidSymbol
            '\b'.code -> XK_BackSpace
            '\t'.code -> XK_Tab
            '\n'.code -> XK_Linefeed
            '\r'.code -> XK_Return
            else -> uniChar
        }
        val modifier =
            (if (keyEvent.isShiftPressed)  ShiftMask   else 0) or
            (if (keyEvent.isCapsLockOn)    LockMask    else 0) or
            (if (keyEvent.isCtrlPressed)   ControlMask else 0) or
            (if (keyEvent.isAltPressed)    AltMask     else 0) or
            (if (keyEvent.isMetaPressed)   MetaMask    else 0) or
            (if (keyEvent.action == KeyEvent.ACTION_UP) ReleaseMask else 0)
        return Pair(x11Code, modifier)
    }

    private val keycodeMap = mapOf(
        KeyEvent.KEYCODE_SPACE       to XK_space,
        KeyEvent.KEYCODE_DEL         to XK_BackSpace,
        KeyEvent.KEYCODE_TAB         to XK_Tab,
        KeyEvent.KEYCODE_CLEAR       to XK_Clear,
        KeyEvent.KEYCODE_ENTER       to XK_Return,
        KeyEvent.KEYCODE_SCROLL_LOCK to XK_Scroll_Lock,
        KeyEvent.KEYCODE_SYSRQ       to XK_Sys_Req,
        KeyEvent.KEYCODE_ESCAPE      to XK_Escape,
        KeyEvent.KEYCODE_FORWARD_DEL to XK_Delete,
        KeyEvent.KEYCODE_MOVE_HOME   to XK_Home,
        KeyEvent.KEYCODE_DPAD_LEFT   to XK_Left,
        KeyEvent.KEYCODE_DPAD_UP     to XK_Up,
        KeyEvent.KEYCODE_DPAD_RIGHT  to XK_Right,
        KeyEvent.KEYCODE_DPAD_DOWN   to XK_Down,
        KeyEvent.KEYCODE_PAGE_UP     to XK_Page_Up,
        KeyEvent.KEYCODE_PAGE_DOWN   to XK_Page_Down,
        KeyEvent.KEYCODE_MOVE_END    to XK_End,
    )

}