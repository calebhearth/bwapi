#include "Dialog.h"
#include "../BWAPI/GameImpl.h"

#include "../Detours.h"
#include "../DLLMain.h"

#include "../../../Debug.h"

namespace BW
{
  // ------------------ TEMPLATE ---------------------
  bool __fastcall InteractTemplate(dialog *dlg, dlgEvent *evt)
  {
    char out[512];
    switch (evt->wNo)
    {
    case 0: // Key down
      break;
    case 1: // Key repeat
      break;
    case 2: // Key Modifier?? Key up?
      break;
    case 3: // Mouse Move
      break;
    case 4: // Left Button down
      break;
    case 5: // Left Button up
      break;
    case 6: // Left Button double click
      break;
    case 7: // Right Button Down
      break;
    case 8: // Right Button Up
      break;
    case 9: // Right Button double click
      break;
    case 10: // Middle Button Down
      break;
    case 11: // Middle Button Up
      break;
    case 12: // Middle Button double click
      break;
    case 13: // Idle
      break;
    case 14: // Control (used for when a control has been pressed)
      switch(evt->dwUser)
      {
      case 0: // Update/Create
        break;
      case 1: // Kill
        break;
      case 2: // Activate
        break;
      case 3: // Press Hotkey
        break;
      case 4: // MouseOver
        break;
      case 5: // Select Previous
        break;
      case 6: // Select Next
        break;
      case 7: // Initialize dialog? Loop? Update?
        break;
      case 8: // Unk Init control
        break;
      case 9: // MouseEnter
        break;
      case 10: // Initialize children
        break;
      case 11: // Set Selected Index
        break;
      case 12:  //      unknown
        break;
      case 13: // Show
        break;
      case 14: // Hide
        break;
      case 15: // Scroll up
        break;
      case 16: // Scroll down
        break;
      default:
        sprintf_s(out, 512, "Event: %d\nUser: 0x%p", evt->wNo, evt->dwUser);
        MessageBox(nullptr, out, "!", MB_OK);
        break;
      }
      break;
    case 15: // User Key Press (like WM_CHAR)
      break;
    case 16: // System Key Press
      break;
    case 17: // Mouse wheel Scroll up
      break;
    case 18: // Mouse wheel Scroll down
      break;
    default:
      sprintf_s(out, 512, "Unknown Event: %d\nUser: 0x%p", evt->wNo, evt->dwUser);
      MessageBox(nullptr, out, "!", MB_OK);
      break;
    }
    return dlg->defaultInteract(evt);
  }
  // ------------------ FIND GLOBAL ------------------
  dialog *FindDialogGlobal(const char *pszName)
  {
    if ( (*BW::BWDATA::DialogList) && pszName )
      return (*BW::BWDATA::DialogList)->findDialog(pszName);
    return nullptr;
  }
  // ----------------- CONSTRUCTORS ------------------
  dialog::dialog(WORD ctrlType, short index, const char *text, WORD left, WORD top, WORD width, WORD height, bool (__fastcall *pfInteract)(dialog*,dlgEvent*))
    : pNext( nullptr )
    , pszText( (char*)text )
    , lFlags( CTRL_VISIBLE )
    , wIndex( index )
    , wCtrlType( ctrlType )
    , srcBits(width,height)
  {
    if ( ctrlType > ctrls::max)
      ctrlType = ctrls::cLSTATIC;

    // Set default height
    if ( height == 0 )
    {
      switch ( ctrlType )
      {
      case ctrls::cBUTTON:
      case ctrls::cDFLTBTN:
        height = 28;
        break;
      case ctrls::cCHKBOX:
      case ctrls::cOPTION:
        height = 20;
        break;
      case ctrls::cEDIT:
        height = 16;
        break;
      }
    }

    // Set size properties
    rct.left    = left;
    rct.top     = top;
    rct.right   = rct.left + width - 1;
    rct.bottom  = rct.top  + height - 1;

    // Set callback functions
    pfcnUpdate  = BW::GenericDlgUpdateFxns[wCtrlType];
    pfcnInteract = pfInteract ? pfInteract : BW::GenericDlgInteractFxns[wCtrlType];

    // Set control type-specific options
    switch ( ctrlType )
    {
    case ctrls::cDLG:
      lFlags          |= CTRL_DLG_NOREDRAW | CTRL_TRANSLUCENT;
      //this->applyDialogBackground();

      // Allocate destination buffer
      u.dlg.dstBits.wid   = width;
      u.dlg.dstBits.ht    = height;
      u.dlg.dstBits.data  = (BYTE*)SMAlloc(width*height);
      break;
    case ctrls::cBUTTON:
    case ctrls::cDFLTBTN:
    case ctrls::cFLCBTN:
      lFlags   |= CTRL_USELOCALGRAPHIC | CTRL_FONT_LARGE | CTRL_TRANSLUCENT;
      wUser = 112;
    case ctrls::cCHKBOX:
    case ctrls::cOPTION:
    case ctrls::cHSCROLL:
    case ctrls::cVSCROLL:
      lFlags |= CTRL_EVENTS;
      break;
    case ctrls::cLIST:
    case ctrls::cCOMBO:
      lFlags |= CTRL_TRANSPARENT;
    case ctrls::cEDIT:
      lFlags |= CTRL_EVENTS | CTRL_BTN_NO_SOUND;
      break;
    case ctrls::cIMAGE:
      //this->applyDialogBackground();
      break;
    }
  }
  // ------------------ DESTRUCTOR -------------------
  dialog::~dialog()
  {
    if ( this )
    {
      if ( this->isDialog() )
      {
        if ( this->isListed() )
          this->activate();

        if ( this->u.dlg.dstBits.data )
          SMFree(this->u.dlg.dstBits.data);
      }
      else
      {
        if ( this->isList() )
        {
          if ( this->u.list.pbStrFlags )
            SMFree(this->u.list.pbStrFlags);
          if ( this->u.list.pdwData )
            SMFree(this->u.list.pdwData);
          if ( this->u.list.ppStrs )
            SMFree(this->u.list.ppStrs);
        }

        if ( this->wCtrlType == ctrls::cEDIT && this->pszText )
          SMFree(this->pszText);

        if ( this->parent() )
        {
          if ( this->parent()->u.dlg.pActiveElement == this )
            this->parent()->u.dlg.pActiveElement = nullptr;

          if ( this->parent()->u.dlg.pMouseElement == this )
            this->parent()->u.dlg.pMouseElement = nullptr;

          if ( this->parent()->u.dlg.pFirstChild == this )
          {
            this->parent()->u.dlg.pFirstChild = this->next();
            return;
          }

          dialog *i = this->parent()->child();
          do
          {
            if ( i->pNext == this )
            {
              i->pNext = this->next();
              return;
            }
            i = i->next();
          } while (i);
        } // if parent
      } // if control
    } // if this
  }
  // --------------------- FIND ----------------------
  dialog *dialog::findIndex(short wIndex)
  {
    if ( !this )
      return nullptr;

    dialog *pCurrDlg;
    if ( this->isDialog() )
      pCurrDlg = this;
    else
      pCurrDlg = this->parent();
      
    if ( pCurrDlg )
      pCurrDlg = pCurrDlg->child();

    while ( pCurrDlg && pCurrDlg->getIndex() != wIndex )
      pCurrDlg = pCurrDlg->next();
    
    if ( pCurrDlg && pCurrDlg->getIndex() == wIndex )
      return pCurrDlg;

    return nullptr;
  }
  // ------------------ FIND BY NAME -----------------
  dialog *dialog::findDialog(const char *pszName)
  {
    if ( this )
    {
      dialog *parent = this;
      if ( !parent->isDialog() )
        parent = parent->parent();
      
      while ( parent )
      {
        if (parent->pszText && _strcmpi(parent->pszText, pszName) == 0 )
          return parent;
        parent = parent->next();
      }
    }
    return nullptr;
  }
  // --------------------- NEXT ----------------------
  dialog *dialog::next() const
  {
    return this ? this->pNext : nullptr;
  }
  // ------------------- SET FLAG --------------------
  bool dialog::setFlags(DWORD dwFlag)
  {
    if ( this )
    {
      if ( !this->isDialog() )
      {
        DWORD size    = dwFlag & (CTRL_FONT_SMALLEST | CTRL_FONT_SMALL | CTRL_FONT_LARGE | CTRL_FONT_LARGEST);
        DWORD halign  = dwFlag & (CTRL_HALIGN_LEFT | CTRL_HALIGN_RIGHT | CTRL_HALIGN_CENTER);
        DWORD valign  = dwFlag & (CTRL_VALIGN_TOP | CTRL_VALIGN_MIDDLE | CTRL_VALIGN_BOTTOM);
        DWORD remains = dwFlag & ~(size | halign | valign);

        // set size
        if (size == CTRL_FONT_SMALLEST ||
            size == CTRL_FONT_SMALL ||
            size == CTRL_FONT_LARGE ||
            size == CTRL_FONT_LARGEST)
        {
          this->clearFlags(CTRL_FONT_SMALLEST | CTRL_FONT_SMALL | CTRL_FONT_LARGE | CTRL_FONT_LARGEST);
          this->lFlags |= size;
        }

        // set horizontal alignment
        if (halign == CTRL_HALIGN_LEFT ||
            halign == CTRL_HALIGN_RIGHT ||
            halign == CTRL_HALIGN_CENTER)
        {
          this->clearFlags(CTRL_HALIGN_LEFT | CTRL_HALIGN_RIGHT | CTRL_HALIGN_CENTER);
          this->lFlags |= halign;
        }

        // set vertical alignment
        if (valign == CTRL_VALIGN_TOP ||
            valign == CTRL_VALIGN_MIDDLE ||
            valign == CTRL_VALIGN_BOTTOM)
        {
          this->clearFlags(CTRL_VALIGN_TOP | CTRL_VALIGN_MIDDLE | CTRL_VALIGN_BOTTOM);
          this->lFlags |= valign;
        }

        this->lFlags |= remains;
      }
      else
      {
        this->lFlags |= dwFlag;
      }
      return true;
    }
    return false;
  }
  // ------------------ CLEAR FLAG -------------------
  bool dialog::clearFlags(DWORD dwFlag)
  {
    if ( this )
    {
      this->lFlags &= ~dwFlag;
      return true;
    }
    return false;
  }
  // ------------------- HAS FLAG --------------------
  bool dialog::hasFlags(DWORD dwFlag) const
  {
    return this && (this->lFlags & dwFlag) == dwFlag;
  }
  // ------------------- SET TEXT --------------------
  bool dialog::setText(char *pszStr)
  {
    if ( this && pszStr )
    {
      if ( this->wCtrlType == ctrls::cEDIT && this->pszText)
        strcpy_s(this->pszText, 255, pszStr);
      else
        this->pszText = pszStr;
      return true;
    }
    return false;
  }
  // ------------------- GET TEXT --------------------
  char *dialog::getText() const
  {
    if ( this && this->pszText )
      return this->pszText;
    return "";
  }
  // ------------------ GET HOTKEY -------------------
  int dialog::getHotkey() const
  {
    if ( this && this->pszText && this->isButton() )
      return toupper(this->pszText[0]);
    return 0;
  }
  // ------------------ SRC BUFFER -------------------
  BW::Bitmap *dialog::getSourceBuffer()
  {
    return this ? &this->srcBits : nullptr;
  }
// -------------------- ENABLE -----------------------
  bool dialog::enable()
  {
    return this->clearFlags(CTRL_DISABLED);
  }
// -------------------- DISABLE -----------------------
  bool dialog::disable()
  {
    return this->setFlags(CTRL_DISABLED);
  }
// ------------------ IS DISABLED --------------------
  bool dialog::isDisabled() const
  {
    return this && this->hasFlags(CTRL_DISABLED);
  }
// --------------------- SHOW ------------------------
  bool dialog::show()
  {
    if ( this )
    {
      this->doEvent(BW_EVN_USER, BW_USER_SHOW);
      this->setFlags(CTRL_VISIBLE);
      return true;
    }
    return false;
  }
// --------------------- HIDE ------------------------
  bool dialog::hide()
  {
    if ( this )
    {
      this->doEvent(BW_EVN_USER, BW_USER_HIDE);
      this->clearFlags(CTRL_VISIBLE);
      return true;
    }
    return false;
  }
// ------------------ IS VISIBLE ---------------------
  bool dialog::isVisible() const
  {
    return this && this->hasFlags(CTRL_VISIBLE);
  }
// ------------------- GET SIZE ----------------------
  u16 dialog::width() const
  {
    return this ? (u16)this->srcBits.width() : 0;
  }
  u16 dialog::height() const
  {
    return this ? (u16)this->srcBits.height() : 0;
  }
// -------------------------------------------------- EVENTS -------------------------------------------------
  // --------------------- EVENT ---------------------
  bool dialog::doEvent(WORD wEvtNum, DWORD dwUser, WORD wSelect, WORD wVirtKey)
  {
    if ( !this || !this->pfcnInteract )
      return false;

    dlgEvent evt;
    evt.cursor.x    = (WORD)BW::BWDATA::Mouse->x;
    evt.cursor.y    = (WORD)BW::BWDATA::Mouse->y;
    evt.wVirtKey    = wVirtKey;
    evt.wSelection  = wSelect;
    evt.wNo         = wEvtNum;
    evt.dwUser      = dwUser;
    return this->pfcnInteract(this, &evt);
  }
  // ----------------- DEFAULT INTERACT --------------
  bool dialog::defaultInteract(BW::dlgEvent *pEvent)
  {
    return this && pEvent && this->wCtrlType < ctrls::max && GenericDlgInteractFxns[this->wCtrlType](this, pEvent);
  }
  // -------------------- ACTIVATE -------------------
  bool dialog::activate()
  {
    return this && this->doEvent(BW_EVN_USER, BW_USER_ACTIVATE);
  }
  // --------------------- UPDATE --------------------
  bool dialog::update()
  {
    return this && this->doEvent(BW_EVN_USER, BW_USER_CREATE);
  }
// -------------------------------------------------- DIALOG -------------------------------------------------
  // --------------------- IS DLG --------------------
  bool dialog::isDialog() const
  {
    return this && this->wCtrlType == ctrls::cDLG;
  }
  // --------------------- CHILD ---------------------
  dialog *dialog::child() const
  {
    if ( this && this->isDialog() && this->u.dlg.pFirstChild )
      return this->u.dlg.pFirstChild;
    return nullptr;
  }
  // --------------------- ADD -----------------------
  bool dialog::addControl(dialog *ctrl)
  {
    if ( !this || !ctrl || ctrl->isDialog())
      return false;

    dialog *parent = this;
    if ( !parent->isDialog() )
      parent = parent->parent();

    ctrl->u.ctrl.pDlg = parent;
    if ( !parent->child() )
    {
      parent->u.dlg.pFirstChild = ctrl;
    }
    else
    {
      dialog *child = parent->child();
      while ( child->pNext )
        child = child->pNext;
      child->pNext = ctrl;
    }
    return true;
  }
  // ------------------ INITIALIZE -------------------
  bool dialog::initialize()
  {
    if ( this && this->isDialog() )
    {
      this->doEvent(BW_EVN_USER, BW_USER_UNK_7);
      this->doEvent(BW_EVN_USER, BW_USER_INIT);
      this->update();
      return true;
    }
    return false;
  }
  // ------------------ IS LISTED --------------------
  bool dialog::isListed() const
  {
    if ( !this )
      return false;

    for ( dialog *i = *BW::BWDATA::DialogList; i; i = i->next() )
    {
      if ( this == i )
        return true;
    }
    return false;
  }
// -------------------------------------------------- CONTROL ------------------------------------------------
  // -------------------- PARENT ---------------------
  dialog *dialog::parent() const
  {
    if ( this && !this->isDialog() && this->u.ctrl.pDlg )
      return this->u.ctrl.pDlg;
    return nullptr;
  }
  // --------------------- INDEX ---------------------
  short dialog::getIndex() const
  {
    return this ? this->wIndex : 0;
  }
  // ------------- CLEAR FONT FLAGS ------------------
  bool dialog::clearFontFlags()
  {
    if ( !this )
      return false;

    this->clearFlags(CTRL_FONT_SMALLEST | CTRL_FONT_SMALL | CTRL_FONT_LARGE | CTRL_FONT_LARGEST);
    this->clearFlags(CTRL_HALIGN_LEFT | CTRL_HALIGN_RIGHT | CTRL_HALIGN_CENTER);
    this->clearFlags(CTRL_VALIGN_TOP | CTRL_VALIGN_MIDDLE | CTRL_VALIGN_BOTTOM);
    return true;
  }
// -------------------------------------------- BUTTON -------------------------------------------------------
  // ------------------- IS BUTTON -------------------
  bool dialog::isButton() const
  {
    if ( !this )
      return false;

    WORD wCtrl = this->wCtrlType;
    return wCtrl == ctrls::cBUTTON || wCtrl == ctrls::cDFLTBTN || wCtrl == ctrls::cFLCBTN;
  }
// -------------------------------------------- CHECKBOX & OPTION --------------------------------------------
  // ------------------- IS CHECKBOX -----------------
  bool dialog::isOption() const
  {
    if ( !this )
      return false;

    WORD wCtrl = this->wCtrlType;
    return wCtrl == ctrls::cCHKBOX || wCtrl == ctrls::cOPTION;
  }
  // --------------------- CHECKED -------------------
  bool dialog::isChecked() const
  {
    return this && this->isOption() && this->u.optn.bEnabled != 0;
  }
// -------------------------------------------- LISTBOX & COMBOBOX -------------------------------------------
  // ----------------------- IS LIST -----------------
  bool dialog::isList() const
  {
    if ( !this )
      return false;

    WORD wCtrl = this->wCtrlType;
    return wCtrl == ctrls::cCOMBO || wCtrl == ctrls::cLIST;
  }
  // --------------- GET SELECTED INDEX --------------
  BYTE dialog::getSelectedIndex() const
  {
    if ( this && this->isList() )
      return this->u.list.bSelectedIndex;
    return 0;
  }
  // --------------- GET SELECTED VALUE --------------
  DWORD dialog::getSelectedValue() const
  {
    if ( this 
        && this->isList() 
        && this->u.list.pdwData 
        && this->u.list.bSelectedIndex < this->u.list.bStrs)
      return this->u.list.pdwData[this->u.list.bSelectedIndex];
    return 0;
  }
  // --------------- GET SELECTED STRING -------------
  char *dialog::getSelectedString() const
  {
    if ( this && this->isList() && this->u.list.ppStrs && this->u.list.ppStrs[this->u.list.bCurrStr])
      return this->u.list.ppStrs[this->u.list.bCurrStr];
    return "";
  }
// -------------- SET SELECTED INDEX -----------------
  bool dialog::setSelectedIndex(BYTE bIndex)
  {
    if ( this && this->isList() && bIndex < this->u.list.bStrs )
    {
      this->doEvent(BW_EVN_USER, BW_USER_SELECT, bIndex);
      if ( this->u.list.pScrlBar )
        this->u.list.pScrlBar->doEvent(BW_EVN_USER, BW_USER_SELECT, bIndex);
      return true;
    }
    return false;
  }
// -------------- SET SELECTED BY VALUE --------------
  bool dialog::setSelectedByValue(DWORD dwValue)
  {
    if ( this && this->isList() && this->u.list.pdwData )
    {
      for ( int i = 0; i < this->u.list.bStrs; ++i )
      {
        if ( this->u.list.pdwData[i] == dwValue )
          return this->setSelectedIndex((BYTE)i);
      }
    } // check
    return false;
  }
// -------------- SET SELECTED BY STRING -------------
  bool dialog::setSelectedByString(const char *pszString, bool noctrl)
  {
    // verify that this is the correct control
    if ( this && this->isList() && this->u.list.ppStrs )
    {
      // Iterate through each entry
      for ( int i = 0; i < this->u.list.bStrs; ++i )
      {
        // compare the string to the one we're looking for
        if ( this->u.list.ppStrs[i] )
        {
          int si = 0; // string index

          // Ignore first control characters
          while ( noctrl && this->u.list.ppStrs[i][si] < ' ' )
            ++si;
          
          // Iterate the string(s) until the first difference
          while ( this->u.list.ppStrs[i][si] > (noctrl ? ' '-1 : '\0')  && 
                  pszString[si] > (noctrl ? ' '-1 : '\0')               &&
                  toupper(this->u.list.ppStrs[i][si]) == toupper(pszString[si]) )
            ++si;

          // Skip if the character differs
          if (  (this->u.list.ppStrs[i][si] > (noctrl ? ' '-1 : '\0')  ||
                 pszString[si] > (noctrl ? ' '-1 : '\0'))               &&
                toupper(this->u.list.ppStrs[i][si]) != toupper(pszString[si]) )
            continue;

          // set the selected entry
          return this->setSelectedIndex((BYTE)i);
        }
      } // iterator
    }
    return false;
  }
  // ------------------- ADD ENTRY -------------------
  bool dialog::addListEntry(char *pszString, DWORD dwValue, BYTE bFlags)
  {
    if ( this && this->isList() && this->u.list.pbStrFlags && this->u.list.pdwData && this->u.list.ppStrs )
    {
      BYTE count = this->u.list.bStrs;
      if ( count < 255 )
      {
        this->u.list.pbStrFlags[count] = bFlags;
        this->u.list.pdwData[count]    = dwValue;
        this->u.list.ppStrs[count]     = pszString;
        this->u.list.bStrs++;
        dialog *scroll = this->u.list.pScrlBar;
        if ( scroll && this->u.list.bStrs > this->u.list.bItemsPerPage )
        {
          scroll->u.scroll.nMax++;
          scroll->show();
          scroll->update();
        }
        return true;
      }
    }
    return false;
  }
  // ----------------- REMOVE ENTRY ------------------
  bool dialog::removeListEntry(BYTE bIndex)
  {
    if ( this && this->isList() && bIndex < this->u.list.bStrs )
    {
      for ( int i = bIndex; i < this->u.list.bStrs; ++i )
      {
        dialog *scroll = this->u.list.pScrlBar;
        if ( scroll && scroll->u.scroll.nMax > scroll->u.scroll.nMin )
          this->u.list.pScrlBar->u.scroll.nMax--;

        if ( i == this->u.list.bStrs - 1 )
        {
          this->u.list.pbStrFlags[i]  = 0;
          this->u.list.pdwData[i]     = 0;
          this->u.list.ppStrs[i]      = nullptr;
          this->u.list.bStrs--;
          if ( this->u.list.bSelectedIndex >= bIndex )
            this->u.list.bSelectedIndex--;
          if ( this->u.list.bCurrStr >= bIndex )
            this->u.list.bCurrStr--;
          if ( this->u.list.bOffset + this->u.list.bItemsPerPage > this->u.list.bStrs && this->u.list.bStrs > this->u.list.bItemsPerPage - 1 )
            this->u.list.bOffset--;

          if ( scroll && this->u.list.bStrs <= this->u.list.bItemsPerPage )
          {
            scroll->hide();
            scroll->update();
          }

          this->doEvent(BW_EVN_USER, BW_USER_SELECT, this->getSelectedIndex());
        }
        else
        {
          this->u.list.pbStrFlags[i]  = this->u.list.pbStrFlags[i+1];
          this->u.list.pdwData[i]     = this->u.list.pdwData[i+1];
          this->u.list.ppStrs[i]      = this->u.list.ppStrs[i+1];
        }
      }
      return true;
    }
    return false;
  }
  // ----------------- CLEAR LIST --------------------
  bool dialog::clearList()
  {
    if ( this && this->isList() )
    {
      for ( int i = 255; i >= 0; i-- )
        this->removeListEntry((BYTE)i);
      dialog *scroll = this->u.list.pScrlBar;
      if ( scroll )
      {
        scroll->u.scroll.nMax = scroll->u.scroll.nMin;
        scroll->hide();
        scroll->update();
      }
      return true;
    }
    return false;
  }
  // -------------- GET LIST COUNT -------------------
  BYTE dialog::getListCount() const
  {
    if ( this && this->isList() )
      return this->u.list.bStrs;
    return 0;
  }
};
