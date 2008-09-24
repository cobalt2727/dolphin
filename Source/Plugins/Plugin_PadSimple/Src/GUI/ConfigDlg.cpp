// Copyright (C) 2003-2008 Dolphin Project.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 2.0.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License 2.0 for more details.

// A copy of the GPL 2.0 should have been included with the program.
// If not, see http://www.gnu.org/licenses/

// Official SVN repository and contact information can be found at
// http://code.google.com/p/dolphin-emu/


#include "ConfigDlg.h"
#include "../PadSimple.h"
//#include "../DirectInputBase.h"

BEGIN_EVENT_TABLE(ConfigDialog,wxDialog)
	EVT_CLOSE(ConfigDialog::OnClose)
	EVT_BUTTON(ID_CLOSE,ConfigDialog::OnCloseClick)
	EVT_CHECKBOX(ID_ATTACHED,ConfigDialog::AttachedCheck)
	EVT_CHECKBOX(ID_DISABLE,ConfigDialog::DisableCheck)
	EVT_CHECKBOX(ID_RUMBLE,ConfigDialog::RumbleCheck)
	EVT_BUTTON(CTL_A,ConfigDialog::OnButtonClick)
	EVT_BUTTON(CTL_B,ConfigDialog::OnButtonClick)
	EVT_BUTTON(CTL_X,ConfigDialog::OnButtonClick)
	EVT_BUTTON(CTL_Y,ConfigDialog::OnButtonClick)
	EVT_BUTTON(CTL_Z,ConfigDialog::OnButtonClick)
	EVT_BUTTON(CTL_START,ConfigDialog::OnButtonClick)
	EVT_BUTTON(CTL_TRIGGER_L,ConfigDialog::OnButtonClick)
	EVT_BUTTON(CTL_L,ConfigDialog::OnButtonClick)
	EVT_BUTTON(CTL_TRIGGER_R,ConfigDialog::OnButtonClick)
	EVT_BUTTON(CTL_R,ConfigDialog::OnButtonClick)
	EVT_BUTTON(CTL_MAINUP,ConfigDialog::OnButtonClick)
	EVT_BUTTON(CTL_MAINDOWN,ConfigDialog::OnButtonClick)
	EVT_BUTTON(CTL_MAINLEFT,ConfigDialog::OnButtonClick)
	EVT_BUTTON(CTL_MAINRIGHT,ConfigDialog::OnButtonClick)
	EVT_BUTTON(CTL_HALFMAIN,ConfigDialog::OnButtonClick)
	EVT_BUTTON(CTL_SUBUP,ConfigDialog::OnButtonClick)
	EVT_BUTTON(CTL_SUBDOWN,ConfigDialog::OnButtonClick)
	EVT_BUTTON(CTL_SUBLEFT,ConfigDialog::OnButtonClick)
	EVT_BUTTON(CTL_SUBRIGHT,ConfigDialog::OnButtonClick)
	EVT_BUTTON(CTL_HALFSUB,ConfigDialog::OnButtonClick)
	EVT_BUTTON(CTL_DPADUP,ConfigDialog::OnButtonClick)
	EVT_BUTTON(CTL_DPADDOWN,ConfigDialog::OnButtonClick)
	EVT_BUTTON(CTL_DPADLEFT,ConfigDialog::OnButtonClick)
	EVT_BUTTON(CTL_DPADRIGHT,ConfigDialog::OnButtonClick)
END_EVENT_TABLE()

ConfigDialog::ConfigDialog(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style)
: wxDialog(parent, id, title, position, size, style)
{
	clickedButton = NULL;
	CreateGUIControls();
}

ConfigDialog::~ConfigDialog()
{
} 

inline void AddControl(wxPanel *pan, wxButton **button, wxStaticBoxSizer *sizer, 
					   const char *name, int ctl) {
	wxBoxSizer *hButton = new wxBoxSizer(wxHORIZONTAL);

	hButton->Add(new wxStaticText(pan, 0, wxT(name), wxDefaultPosition, wxDefaultSize), 0,
				 wxALIGN_CENTER_VERTICAL|wxALL);
	
	*button = new wxButton(pan, ctl, wxT(""), wxDefaultPosition, wxDefaultSize, wxBU_LEFT);

	hButton->Add(*button, 0, wxEXPAND|wxALL);
	
	sizer->Add(hButton);
}

void ConfigDialog::CreateGUIControls()
{
	// Notebook
	m_Notebook = new wxNotebook(this, ID_NOTEBOOK, wxDefaultPosition, wxDefaultSize);
	
	// Controller pages
	m_Controller[0] = new wxPanel(m_Notebook, ID_CONTROLLERPAGE1, wxDefaultPosition, wxDefaultSize);
	m_Notebook->AddPage(m_Controller[0], wxT("Controller 1"));
	m_Controller[1] = new wxPanel(m_Notebook, ID_CONTROLLERPAGE2, wxDefaultPosition, wxDefaultSize);
	m_Notebook->AddPage(m_Controller[1], wxT("Controller 2"));
	m_Controller[2] = new wxPanel(m_Notebook, ID_CONTROLLERPAGE3, wxDefaultPosition, wxDefaultSize);
	m_Notebook->AddPage(m_Controller[2], wxT("Controller 3"));
	m_Controller[3] = new wxPanel(m_Notebook, ID_CONTROLLERPAGE4, wxDefaultPosition, wxDefaultSize);
	m_Notebook->AddPage(m_Controller[3], wxT("Controller 4"));
	
	// Standard buttons
	m_Close = new wxButton(this, ID_CLOSE, wxT("Close"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
	
	// Put notebook and standard buttons in sizers
	wxBoxSizer* sSButtons;
	sSButtons = new wxBoxSizer(wxHORIZONTAL);
	sSButtons->Add(0, 0, 1, wxEXPAND, 5);
	sSButtons->Add(m_Close, 0, wxALL, 5);
	
	wxBoxSizer* sMain;
	sMain = new wxBoxSizer(wxVERTICAL);
	sMain->Add(m_Notebook, 1, wxEXPAND|wxALL, 5);
	sMain->Add(sSButtons, 0, wxEXPAND, 5);
	
	this->SetSizer(sMain);
	this->Layout();

	wxArrayString arrayStringFor_DeviceName;

	for(int i = 0; i < 4; i++)
	{
		sDevice[i] = new wxStaticBoxSizer(wxVERTICAL, m_Controller[i], wxT("Controller:"));
		sDeviceTop[i] = new wxBoxSizer(wxHORIZONTAL);
		sDeviceBottom[i] = new wxBoxSizer(wxHORIZONTAL);
		m_DeviceName[i] = new wxChoice(m_Controller[i], ID_DEVICENAME, wxDefaultPosition, wxDefaultSize, arrayStringFor_DeviceName, 0, wxDefaultValidator);
		m_Attached[i] = new wxCheckBox(m_Controller[i], ID_ATTACHED, wxT("Controller attached"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
		m_Disable[i] = new wxCheckBox(m_Controller[i], ID_DISABLE, wxT("Disable when dolphin isn't in foreground"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
		m_Rumble[i] = new wxCheckBox(m_Controller[i], ID_RUMBLE, wxT("Enable rumble"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
		m_Attached[i]->SetValue(pad[i].attached);
		m_Disable[i]->SetValue(pad[i].disable);
		m_Rumble[i]->SetValue(pad[i].rumble);
		m_Rumble[i]->Show(pad[i].type);
		
		sDeviceTop[i]->Add(m_DeviceName[i], 1, wxEXPAND|wxALL, 1);
		sDeviceTop[i]->Add(m_Attached[i], 0, wxEXPAND|wxALL, 1);
		sDeviceBottom[i]->AddStretchSpacer(1);
		sDeviceBottom[i]->Add(m_Disable[i], 0, wxEXPAND|wxALL, 1);
		sDeviceBottom[i]->Add(m_Rumble[i], 0, wxEXPAND|wxALL, 1);
		sDeviceBottom[i]->AddStretchSpacer(1);
		sDevice[i]->Add(sDeviceTop[i], 0, wxEXPAND|wxALL, 1);
		sDevice[i]->Add(sDeviceBottom[i], 0, wxEXPAND|wxALL, 1);

		sButtons[i] = new wxStaticBoxSizer(wxVERTICAL, m_Controller[i], wxT("Buttons:"));
        
		AddControl(m_Controller[i], &(m_ButtonA[i]), sButtons[i], "A: ", CTL_A);
		AddControl(m_Controller[i], &(m_ButtonB[i]), sButtons[i], "B: ", CTL_B);
		AddControl(m_Controller[i], &(m_ButtonX[i]), sButtons[i], "X: ", CTL_X);
		AddControl(m_Controller[i], &(m_ButtonY[i]), sButtons[i], "Y: ", CTL_Y);
		AddControl(m_Controller[i], &(m_ButtonZ[i]), sButtons[i], "Z: ", CTL_Z);
		AddControl(m_Controller[i], &(m_ButtonStart[i]), sButtons[i], "Start: ", CTL_START);

		sTriggerL[i] = new wxStaticBoxSizer(wxVERTICAL, m_Controller[i], wxT("L Trigger"));
		sTriggerR[i] = new wxStaticBoxSizer(wxVERTICAL, m_Controller[i], wxT("R Trigger"));

		AddControl(m_Controller[i], &(m_TriggerL[i]), sTriggerL[i], "Analog: ", CTL_TRIGGER_L);
		AddControl(m_Controller[i], &(m_ButtonL[i]), sTriggerL[i], "Click: ", CTL_L);

		AddControl(m_Controller[i], &(m_TriggerR[i]), sTriggerR[i], "Analog: ", CTL_TRIGGER_R);
		AddControl(m_Controller[i], &(m_ButtonR[i]), sTriggerR[i], "Click: ", CTL_R);

		sStick[i] = new wxStaticBoxSizer(wxVERTICAL, m_Controller[i], wxT("Main Stick"));

		AddControl(m_Controller[i], &(m_StickUp[i]), sStick[i], "Up: ", CTL_MAINUP);
		AddControl(m_Controller[i], &(m_StickDown[i]), sStick[i], "Down: ", CTL_MAINDOWN);
		AddControl(m_Controller[i], &(m_StickLeft[i]), sStick[i], "Left: ", CTL_MAINLEFT);
		AddControl(m_Controller[i], &(m_StickRight[i]), sStick[i], "Right: ", CTL_MAINRIGHT);

		sDPad[i] = new wxStaticBoxSizer(wxVERTICAL, m_Controller[i], wxT("D-Pad"));

		AddControl(m_Controller[i], &(m_DPadUp[i]), sDPad[i], "Up: ", CTL_DPADUP);
		AddControl(m_Controller[i], &(m_DPadDown[i]), sDPad[i], "Down: ", CTL_DPADDOWN);
		AddControl(m_Controller[i], &(m_DPadLeft[i]), sDPad[i], "Left: ", CTL_DPADLEFT);
		AddControl(m_Controller[i], &(m_DPadRight[i]), sDPad[i], "Right: ", CTL_DPADRIGHT);

		sCStick[i] = new wxStaticBoxSizer(wxVERTICAL, m_Controller[i], wxT("C-Stick"));

		AddControl(m_Controller[i], &(m_CStickUp[i]), sCStick[i], "Up: ", CTL_SUBUP);
		AddControl(m_Controller[i], &(m_CStickDown[i]), sCStick[i], "Down: ", CTL_SUBDOWN);
		AddControl(m_Controller[i], &(m_CStickLeft[i]), sCStick[i], "Left: ", CTL_SUBLEFT);
		AddControl(m_Controller[i], &(m_CStickRight[i]), sCStick[i], "Right: ", CTL_SUBRIGHT);

		sPage[i] = new wxGridBagSizer(0, 0);
		sPage[i]->SetFlexibleDirection(wxBOTH);
		sPage[i]->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);
		sPage[i]->Add(sDevice[i], wxGBPosition(0, 0), wxGBSpan(1, 5), wxEXPAND|wxALL, 1);
		sPage[i]->Add(sButtons[i], wxGBPosition(1, 0), wxGBSpan(3, 1), wxALL, 1);
		sPage[i]->Add(sTriggerL[i], wxGBPosition(1, 1), wxGBSpan(1, 1), wxALL, 1);
		sPage[i]->Add(sTriggerR[i], wxGBPosition(2, 1), wxGBSpan(1, 1), wxALL, 1);
		sPage[i]->Add(sStick[i], wxGBPosition(1, 2), wxGBSpan(2, 1), wxALL, 1);
		sPage[i]->Add(sDPad[i], wxGBPosition(1, 3), wxGBSpan(2, 1), wxALL, 1);
		sPage[i]->Add(sCStick[i], wxGBPosition(1, 4), wxGBSpan(2, 1), wxALL, 1);
		m_Controller[i]->SetSizer(sPage[i]);
		sPage[i]->Layout();
	}

	SetIcon(wxNullIcon);
	Fit();
}

void ConfigDialog::OnClose(wxCloseEvent& event)
{
	EndModal(0);
}
void ConfigDialog::OnKeyDown(wxKeyEvent& event)
{
	if(clickedButton != NULL)
	{
		int page = m_Notebook->GetSelection();
		pad[page].keyForControl[clickedButton->GetId()] = event.GetKeyCode();
		clickedButton->SetLabel(wxString::Format(_T("%c"), event.GetKeyCode()));
                clickedButton->Disconnect();
	}

	clickedButton = NULL;
	event.Skip();
}
void ConfigDialog::OnCloseClick(wxCommandEvent& event)
{
	Close();
}

void ConfigDialog::AttachedCheck(wxCommandEvent& event)
{
	int page = m_Notebook->GetSelection();
	pad[page].attached = m_Attached[page]->GetValue();
}

void ConfigDialog::DisableCheck(wxCommandEvent& event)
{
	int page = m_Notebook->GetSelection();
	pad[page].disable = m_Disable[page]->GetValue();
}

void ConfigDialog::RumbleCheck(wxCommandEvent& event)
{
	int page = m_Notebook->GetSelection();
	pad[page].rumble = m_Rumble[page]->GetValue();
}

void ConfigDialog::OnButtonClick(wxCommandEvent& event)
{
        if(clickedButton) {
            clickedButton->SetLabel(oldLabel);
        }
	clickedButton = (wxButton *)event.GetEventObject();
	oldLabel = clickedButton->GetLabel();
	clickedButton->SetLabel(wxString::FromAscii("Press Key"));

        
        clickedButton->Connect(wxID_ANY, wxEVT_KEY_DOWN,
                               wxKeyEventHandler(ConfigDialog::OnKeyDown),
                               (wxObject*)NULL, this);
        
	//clickedButton->SetLabel(wxString::Format(wxT("%i"), keyPress));

	//clickedButton->SetLabel(wxString::Format(wxT("%s  %i"), oldLabel, keyPress));

}
