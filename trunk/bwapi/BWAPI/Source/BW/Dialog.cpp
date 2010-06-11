#include "Dialog.h"

BYTE  gbMinimizeGfx[12][12] = {
  { 0x2C, 0x2C, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2C, 0x2C },
  { 0x2C, 0x2A, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2A, 0x2C },
  { 0x2A, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2A },
  { 0x2A, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2A },
  { 0x2A, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2A },
  { 0x2A, 0x2B, 0x2B, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2B, 0x2B, 0x2A },
  { 0x2A, 0x2B, 0x2B, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2B, 0x2B, 0x2A },
  { 0x2A, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2A },
  { 0x2A, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2A },
  { 0x2A, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2A },
  { 0x2C, 0x2A, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2A, 0x2C },
  { 0x2C, 0x2C, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2C, 0x2C }
};

BYTE  gbCloseGfx[12][12] = {
  { 0x2C, 0x2C, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2C, 0x2C },
  { 0x2C, 0x2A, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2A, 0x2C },
  { 0x2A, 0x2B, 0x2B, 0x2A, 0x2B, 0x2B, 0x2B, 0x2B, 0x2A, 0x2B, 0x2B, 0x2A },
  { 0x2A, 0x2B, 0x2B, 0x2B, 0x2A, 0x2B, 0x2B, 0x2A, 0x2B, 0x2B, 0x2B, 0x2A },
  { 0x2A, 0x2B, 0x2B, 0x2B, 0x2A, 0x2B, 0x2B, 0x2A, 0x2B, 0x2B, 0x2B, 0x2A },
  { 0x2A, 0x2B, 0x2B, 0x2B, 0x2B, 0x2A, 0x2A, 0x2B, 0x2B, 0x2B, 0x2B, 0x2A },
  { 0x2A, 0x2B, 0x2B, 0x2B, 0x2B, 0x2A, 0x2A, 0x2B, 0x2B, 0x2B, 0x2B, 0x2A },
  { 0x2A, 0x2B, 0x2B, 0x2B, 0x2A, 0x2B, 0x2B, 0x2A, 0x2B, 0x2B, 0x2B, 0x2A },
  { 0x2A, 0x2B, 0x2B, 0x2B, 0x2A, 0x2B, 0x2B, 0x2A, 0x2B, 0x2B, 0x2B, 0x2A },
  { 0x2A, 0x2B, 0x2B, 0x2A, 0x2B, 0x2B, 0x2B, 0x2B, 0x2A, 0x2B, 0x2B, 0x2A },
  { 0x2C, 0x2A, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2B, 0x2A, 0x2C },
  { 0x2C, 0x2C, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2C, 0x2C }
};

namespace BW
{
  // ------------------ TEMPLATE ---------------------
  bool __fastcall InteractTemplate(dialog *dlg, dlgEvent *evt)
  {
    char out[512];
    switch (evt->wNo)
    {
    case 0:
      // Key down
      break;
    case 1:
      // Key repeat
      break;
    case 2:
      // Key Modifier?? Key up?
      break;
    case 3:
      // Mouse update/Move
      break;
    case 4:
      // Left Button down
      break;
    case 5:
      // Left Button up
      break;
    case 6:
      // Left Button double click
      break;
    case 7:
      // Right Button Down
      break;
    case 8:
      // Right Button Up
      break;
    case 9:
      // Right Button double click
      break;
    case 10:
      // Middle Button Down
      break;
    case 11:
      // Middle Button Up
      break;
    case 12:
      // Middle Button double click
      break;
    case 13:
      // unknown; Loop? Always?
      break;
    case 14:
      // Control (used for when a control has been pressed)
      switch(evt->dwUser)
      {
      case 0:
        // Initialize
        break;
      case 1:
        // Kill
        break;
      case 2:
        // Activate
        break;
      case 3:
        // Press Hotkey
        break;
      case 4:
        // MouseOver
        break;
      case 5:
        // Release Default
        break;
      case 6:
        // Set Default
        break;
      case 7:
        // Initialize dialog
        break;
      case 9:
        // MouseEnter
        break;
      case 10:
        // Initialize children
        break;
      case 13:
        // Show
        break;
      case 14:
        // Hide
        break;
      default:
        sprintf_s(out, 512, "Event: %d\nUser: 0x%p", evt->wNo, evt->dwUser);
        MessageBoxA(NULL, out, "!", MB_OK);
        break;
      }
      break;
    case 15:
      // User Key press
      break;
    case 17:
      // Mouse wheel Scroll up
      break;
    case 18:
      // Mouse wheel Scroll down
      break;
    default:
      sprintf_s(out, 512, "Unknown Event: %d\nUser: 0x%p", evt->wNo, evt->dwUser);
      MessageBoxA(NULL, out, "!", MB_OK);
      break;
    }
    if ( dlg->wCtrlType < ctrls::max )
      return BWDATA_GenericDlgInteractFxns[dlg->wCtrlType](dlg, evt);
    return false;
  }
  // ------------------ BUTTON INTERACT --------------
  bool __fastcall ButtonInteract(dialog *dlg, dlgEvent *evt)
  {
    switch (evt->wNo)
    {
    case 14:
      // Control (used for when a control has been pressed)
      switch(evt->dwUser)
      {
      case 2:
        // Activate
        switch (dlg->wIndex)
        {
        case 255: // minimize
          if ( dlg->wUnk_0x1E )
          {
            dlg->parent()->srcBits.ht = (WORD)dlg->lUser;
            dlg->parent()->rct.Ymax = dlg->parent()->rct.Ymin + dlg->parent()->srcBits.ht - 1;
            dlg->parent()->u.dlg.dstBits.ht = dlg->parent()->srcBits.ht;
            dlg->wUnk_0x1E = 0;
          }
          else
          {
            dlg->lUser = dlg->parent()->srcBits.ht;
            dlg->parent()->srcBits.ht = 13;
            dlg->parent()->rct.Ymax = dlg->parent()->rct.Ymin + dlg->parent()->srcBits.ht - 1;
            dlg->parent()->u.dlg.dstBits.ht = dlg->parent()->srcBits.ht;
            dlg->wUnk_0x1E = 1;
          }
          break;
        }
        break;
      }
      break;
    }
    if ( dlg->wCtrlType < ctrls::max )
      return BWDATA_GenericDlgInteractFxns[dlg->wCtrlType](dlg, evt);
    return false;
  }
  // ------------------ WINDOW INTERACT --------------
  bool __fastcall WindowInteract(dialog *dlg, dlgEvent *evt)
  {
    pt   *mouseOffset = (pt*)&dlg->lUser;
    switch (evt->wNo)
    {
    case 3:
      // Mouse update/Move
      if ( dlg->wUnk_0x1E )
      {
        dlg->rct.Xmin = evt->cursor.x - mouseOffset->x;
        dlg->rct.Xmax = dlg->rct.Xmin + dlg->srcBits.wid - 1;
        dlg->rct.Ymin = evt->cursor.y - mouseOffset->y;
        dlg->rct.Ymax = dlg->rct.Ymin + dlg->srcBits.ht - 1;
        if ( dlg->rct.Xmin < 0 )
        {
          dlg->rct.Xmax -= dlg->rct.Xmin;
          dlg->rct.Xmin -= dlg->rct.Xmin;
        }
        if ( dlg->rct.Ymin < 0 )
        {
          dlg->rct.Ymax -= dlg->rct.Ymin;
          dlg->rct.Ymin -= dlg->rct.Ymin;
        }
        if ( dlg->rct.Xmax > 640 )
        {
          dlg->rct.Xmin -= dlg->rct.Xmax - 640;
          dlg->rct.Xmax -= dlg->rct.Xmax - 640;
        }
        if ( dlg->rct.Ymax > 360 )
        {
          dlg->rct.Ymin -= dlg->rct.Ymax - 360;
          dlg->rct.Ymax -= dlg->rct.Ymax - 360;
        }
      }
      break;
    case 4:
      // Left Button down
      if ( evt->cursor.x >= dlg->rct.Xmin &&
         evt->cursor.x <= dlg->rct.Xmax - 27 &&
         evt->cursor.y >= dlg->rct.Ymin &&
         evt->cursor.y <= dlg->rct.Ymin + 12 )
      {
        mouseOffset->x = evt->cursor.x - dlg->rct.Xmin;
        mouseOffset->y = evt->cursor.y - dlg->rct.Ymin;
        dlg->wUnk_0x1E = 1;
      }
      break;
    case 5:
      // Left Button up
      dlg->wUnk_0x1E = 0;
      break;
    }
    if ( dlg->wCtrlType < ctrls::max )
      return BWDATA_GenericDlgInteractFxns[dlg->wCtrlType](dlg, evt);
    return false;
  }
  // ------------------ CREATE DLG WINDOW ------------
  dialog *CreateDialogWindow(const char *text, WORD left, WORD top, WORD width, WORD height)
  {
    dialog *dlg = new dialog(ctrls::cDLG, 0, text, left, top, width, height, &WindowInteract);
    BYTE *data  = dlg->srcBits.data;
    if ( data )
    {
      // Title bar
      // second line
      memset(&data[width + 3], 0x2C, width - 6);

      // third line
      memset(&data[width*2 + 2], 0x2C, width - 4);

      for (int i = 3; i < 12; i++)
        memset(&data[width*i + 1], 0x2C, width - 2);
    }

    dialog *title = new dialog(ctrls::cLSTATIC, -255, text, 8, 1, width - 27, 12);
    title->setFlag(CTRL_FONT_SMALLEST);
    dlg->add(title);

    dialog *minimize = new dialog(ctrls::cBUTTON, 255, "", width - 26, 1, 12, 12, &ButtonInteract);
    minimize->clearFlag(CTRL_BTN_HOTKEY | CTRL_ALTERNATE);
    minimize->wGraphic      = 0;
    minimize->srcBits.data  = (BYTE*)gbMinimizeGfx;
    dlg->add(minimize);

    dialog *close = new dialog(ctrls::cBUTTON, -2, "", width - 13, 1, 12, 12, &ButtonInteract);
    close->clearFlag(CTRL_BTN_HOTKEY | CTRL_ALTERNATE);
    close->wGraphic      = 0;
    close->srcBits.data  = (BYTE*)gbCloseGfx;
    dlg->add(close);
    return dlg;
  }

// -------------------------------------------------- GLOBAL -------------------------------------------------
  // ----------------- CONSTRUCTORS ------------------
  dialog::dialog(WORD ctrlType, short index, const char *text, WORD left, WORD top, WORD width, WORD height, bool (__fastcall *pfInteract)(dialog*,dlgEvent*), WORD wGraphicIndex)
  {
    if ( ctrlType > ctrls::max)
      ctrlType = ctrls::cLSTATIC;

    memset(this, 0, sizeof(dialog));
    rct.Xmin        = left;
    rct.Ymin        = top;
    rct.Xmax        = rct.Xmin + width - 1;
    rct.Ymax        = rct.Ymin + height - 1;
    srcBits.wid     = width;
    srcBits.ht      = height;

    pszText         = (char*)text;
    lFlags        = CTRL_VISIBLE;
    if ( ctrlType == ctrls::cDLG )
      lFlags      |= CTRL_DLG_NOREDRAW | CTRL_TRANSLUCENT;

    wIndex      = index;
    wCtrlType   = ctrlType;
    wGraphic    = wGraphicIndex;

    if ( ctrlType == ctrls::cBUTTON || ctrlType == ctrls::cDFLTBTN || ctrlType == ctrls::cFLCBTN)
    {
      if ( wGraphicIndex == 0 )
        wGraphic = 112;

      lFlags                  |= CTRL_EVENTS | CTRL_BTN_HOTKEY | CTRL_ALTERNATE | CTRL_FONT_LARGE;
      u.btn.responseRct.Xmin  = rct.Xmin;
      u.btn.responseRct.Ymin  = rct.Ymin;
      u.btn.responseRct.Xmax  = rct.Xmax;
      u.btn.responseRct.Ymax  = rct.Ymax;
      u.btn.textRct.Xmin      = rct.Xmin;
      u.btn.textRct.Ymin      = rct.Ymin;
      u.btn.textRct.Xmax      = rct.Xmax;
      u.btn.textRct.Ymax      = rct.Ymax;
    }

    if ( pfInteract )
      pfcnInteract  = pfInteract;
    else
      pfcnInteract  = BW::BWDATA_GenericDlgInteractFxns[wCtrlType];

    pfcnUpdate    = BW::BWDATA_GenericDlgUpdateFxns[wCtrlType];

    if ( ctrlType == ctrls::cDLG )
    {
      srcBits.data        = (BYTE*)malloc(width*height);
      BYTE *data = srcBits.data;
      if ( data )
      {
        // first line
        memset(data, 0x00, 3);
        memset(&data[3], 0x2A, width - 6);
        memset(&data[width - 3], 0x00, 3);

        // second line
        data[width] = 0;
        memset(&data[width + 1], 0x2A, 2);
        memset(&data[width + 3], 0x29, width - 6);
        memset(&data[width*2 - 3], 0x2A, 2);
        data[width*2 - 1] = 0;

        // third line
        data[width*2] = 0;
        data[width*2 + 1] = 0x2A;
        memset(&data[width*2 + 2], 0x29, width - 4);
        data[width*3 - 2] = 0x2A;
        data[width*3 - 1] = 0;

        for (int i = 3; i < height - 3; i++)
        {
          data[width*i] = 0x2A;
          memset(&data[width*i + 1], 0x29, width - 2);
          data[width*(i+1) - 1] = 0x2A;
        }

        // third-last line
        data[width*height - width*3] = 0;
        data[width*height - width*3 + 1] = 0x2A;
        memset(&data[width*height - width*3 + 2], 0x29, width - 4);
        data[width*height - width*2 - 2] = 0x2A;
        data[width*height - width*2 - 1] = 0;

        // second-last line
        data[width*height - width*2] = 0;
        memset(&data[width*height - width*2 + 1], 0x2A, 2);
        memset(&data[width*height - width*2 + 3], 0x29, width - 6);
        memset(&data[width*height - width - 3], 0x2A, 2);
        data[width*height - width - 1] = 0;

        // last line
        memset(&data[width*height - width], 0x00, 3);
        memset(&data[width*height - width + 3], 0x2A, width - 6);
        memset(&data[width*height - 3], 0x00, 3);
      }

      u.dlg.dstBits.wid   = width;
      u.dlg.dstBits.ht    = height;
      u.dlg.dstBits.data  = (BYTE*)malloc(width*height);
    }
  }
  // ------------------ DESTRUCTOR -------------------
  dialog::~dialog()
  {
    if ( this )
    {
      if ( this->wCtrlType == ctrls::cDLG )
      {
        dialog *prevDialog = (*BW::BWDATA_ScreenDialog);
        while ( prevDialog && prevDialog->pNext != NULL && prevDialog->pNext != this)
          prevDialog = prevDialog->pNext;
        
        if ( prevDialog->pNext == this )
          prevDialog->pNext = this->next();

        dialog *child = this->child();
        while ( child )
        {
          dialog *nextChild = child->next();
          delete child;
          child = nextChild;
        }
        free(this->u.dlg.dstBits.data);
      }
      else
      {
        dialog *previous = this->parent()->child();
        if ( previous == this )
        {
          this->parent()->u.dlg.pFirstChild = this->pNext;
        }
        else
        {
          while ( previous->pNext && previous->pNext != this)
            previous = previous->next();

          if ( previous->pNext == this )
            previous->pNext = this->pNext;
        }
      }
    }
  }
  // --------------------- FIND ----------------------
  dialog *dialog::FindIndex(short wIndex)
  {
    if ( !this )
      return NULL;

    dialog *pCurrDlg;
    if ( this->isDialog() )
      pCurrDlg = this;
    else
      pCurrDlg = this->parent();

    if ( pCurrDlg )
      pCurrDlg = pCurrDlg->child();

    while (pCurrDlg != NULL && pCurrDlg->getIndex() != wIndex)
      pCurrDlg = pCurrDlg->next();
    
    if ( pCurrDlg->getIndex() == wIndex )
      return pCurrDlg;

    return NULL;
  }
  // --------------------- ADD -----------------------
  void dialog::add(dialog *dlg)
  {
    if ( !this || !dlg)
      return;

    dialog *parent = this;
    if ( !parent->isDialog() )
      parent = parent->parent();

    if ( dlg->isDialog() )
    {
      while ( parent->pNext )
        parent = parent->pNext;
      parent->pNext = dlg;
    }
    else
    {
      dlg->u.ctrl.pDlg = parent;
      if ( !parent->child() )
      {
        parent->u.dlg.pFirstChild = dlg;
      }
      else
      {
        dialog *child = parent->child();
        while ( child->pNext )
          child = child->pNext;
        child->pNext = dlg;
      }
    }
  }
  // --------------------- NEXT ----------------------
  dialog *dialog::next()
  {
    if ( this && this->pNext )
      return this->pNext;
    return NULL;
  }
  // ------------------ FIND BY NAME -----------------
  dialog *dialog::findDialogByName(const char *pszName)
  {
    if ( this )
    {
      dialog *parent = this;
      if ( !parent->isDialog() )
        parent = parent->parent();
      
      while ( parent )
      {
        if (parent->pszText && strcmpi(parent->pszText, pszName) == 0 )
          return parent;
        parent = parent->next();
      }
    }
    return NULL;
  }
  // --------------------- FLAGS ---------------------
  void dialog::setFlag(DWORD dwFlag)
  {
    if ( this )
      this->lFlags |= dwFlag;
  }
  void dialog::clearFlag(DWORD dwFlag)
  {
    if ( this )
      this->lFlags &= ~dwFlag;
  }
// -------------------------------------------------- DIALOG -------------------------------------------------
  // --------------------- IS DLG --------------------
  bool dialog::isDialog()
  {
    if ( this && this->wCtrlType == ctrls::cDLG )
      return true;
    return false;
  }
  // --------------------- CHILD ---------------------
  dialog *dialog::child()
  {
    if (this && this->isDialog() && this->u.dlg.pFirstChild )
      return this->u.dlg.pFirstChild;
    return NULL;
  }
// -------------------------------------------------- CONTROL ------------------------------------------------
  // -------------------- PARENT ---------------------
  dialog *dialog::parent()
  {
    if ( this && !this->isDialog() && this->u.ctrl.pDlg )
      return this->u.ctrl.pDlg;
    return NULL;
  }
  // --------------------- INDEX ---------------------
  short dialog::getIndex()
  {
    if ( this )
      return this->wIndex;
    return 0;
  }
  // ------------------- IS CHECKBOX -----------------
  bool dialog::isOption()
  {
    if ( this )
    {
      WORD wCtrl = this->wCtrlType;
      if ( wCtrl == ctrls::cCHKBOX || wCtrl == ctrls::cOPTION)
        return true;
    }
    return false;
  }
  // --------------------- CHECKED -------------------
  bool dialog::isChecked()
  {
    if ( this && this->isOption() )
      return this->u.optn.bEnabled != 0;
    return false;
  }
// -------------------------------------------- LISTBOX & COMBOBOX -------------------------------------------
// ----------------------- IS LIST -------------------
  bool dialog::isList()
  {
    if ( this )
    {
      WORD wCtrl = this->wCtrlType;
      if ( wCtrl == ctrls::cCOMBO || wCtrl == ctrls::cLIST )
        return true;
    }
    return false;
  }
// ------------------- GET SELECTED ------------------
  BYTE dialog::getSelectedIndex()
  {
    if ( this && this->isList() )
      return this->u.list.bSelectedIndex;
    return 0;
  }
  DWORD dialog::getSelectedValue()
  {
    if ( this 
        && this->isList() 
        && this->u.list.pdwData 
        && this->u.list.bSelectedIndex < this->u.list.bStrs)
      return this->u.list.pdwData[this->u.list.bSelectedIndex];
    return 0;
  }
  char *dialog::getSelectedString()
  {
    if ( this && this->isList() && this->u.list.ppStrs && this->u.list.ppStrs[this->u.list.bCurrStr])
      return this->u.list.ppStrs[this->u.list.bCurrStr];
    return "";
  }
// ------------------- SET SELECTED ------------------
  bool dialog::setSelectedIndex(BYTE bIndex)
  {
    if ( this && this->isList() && bIndex < this->u.list.bStrs )
    {
      this->u.list.bCurrStr       = bIndex;
      this->u.list.bSelectedIndex = bIndex;
      return true;
    }
    return false;
  }
  bool dialog::setSelectedByValue(DWORD dwValue)
  {
    if ( this && this->isList() && this->u.list.pdwData)
    {
      for (int i = 0; i < this->u.list.bStrs; i++)
      {
        if ( this->u.list.pdwData[i] == dwValue)
        {
          this->u.list.bCurrStr       = (BYTE)i;
          this->u.list.bSelectedIndex = (BYTE)i;
          return true;
        }
      }
    } // check
    return false;
  }
  bool dialog::setSelectedByString(char *pszString)
  {
    // verify that this is the correct control
    if ( this && this->isList() )
    {
      // Iterate through each entry
      for (int i = 0; i < this->u.list.bStrs; i++)
      {
        // Verify pointer validity
        if ( this->u.list.ppStrs && this->u.list.ppStrs[i] )
        {
          // compare the string to the one we're looking for
          if ( strcmpi(this->u.list.ppStrs[i], pszString) == 0 )
          {
            // set the selected entry
            this->u.list.bCurrStr       = (BYTE)i;
            this->u.list.bSelectedIndex = (BYTE)i;
            return true;
          }
        } // pointer validate
      } // iterator
    }
    return false;
  }

};
