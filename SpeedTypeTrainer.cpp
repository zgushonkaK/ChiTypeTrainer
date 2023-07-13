#include <string>
#include <set>
#include <algorithm>
#include <map>
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/grid.h>
#include <file_load.hpp>
#include <user.hpp>
#include <progress.hpp>

class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};

class MyDialog : public wxDialog
{
public:
    MyDialog(wxWindow* parent, const wxString& title)
        : wxDialog(parent, wxID_ANY, title)
    {
        wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
        SetSizer(sizer);

        wxRadioButton* radioButton1 = new wxRadioButton(this, wxID_ANY, "Mode 1");
        sizer->Add(radioButton1, 0, wxALL, 10);

        wxRadioButton* radioButton2 = new wxRadioButton(this, wxID_ANY, "Mode 2");
        sizer->Add(radioButton2, 0, wxALL, 10);

        wxButton* okButton = new wxButton(this, wxID_OK, "OK");
        sizer->Add(okButton, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 10);

        wxButton* cancelButton = new wxButton(this, wxID_CANCEL, "Cancel");
        sizer->Add(cancelButton, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 10);

        Bind(wxEVT_COMMAND_RADIOBUTTON_SELECTED, &MyDialog::OnRadioButtonSelected, this);
    }

    int GetSelectedMode() const
    {
        return m_selectedMode;
    }

private:
    int m_selectedMode;

    void OnRadioButtonSelected(wxCommandEvent& event)
    {
        wxRadioButton* radioButton = (wxRadioButton*)event.GetEventObject();
        wxString label = radioButton->GetLabel();
        if (label == "Mode 1")
        {
            m_selectedMode = 1;
        }
        else if (label == "Mode 2")
        {
            m_selectedMode = 2;
        }
    }
};

class LoginDialog : public wxDialog {
public:
    LoginDialog(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style, std::map<std::string, std::string> users);
    wxString getUser() { return selected_user; };
private:
    wxPanel* userPanel;
    wxPanel* passPanel;
    wxTextCtrl* passCtrl;
    wxComboBox* userCombo;
    std::map<std::string, std::string> users;
    void OnOk(wxCommandEvent& event);
    void OnCancel(wxCommandEvent& event);
    wxString selected_user;
    wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(LoginDialog, wxDialog)
    EVT_BUTTON(wxID_OK, LoginDialog::OnOk)
    EVT_BUTTON(wxID_CANCEL, LoginDialog::OnCancel)
wxEND_EVENT_TABLE()

LoginDialog::LoginDialog(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style, std::map<std::string, std::string> users) : wxDialog(parent, id, title, pos, size, style) {
    this->users = users;
    userPanel = new wxPanel(this, wxID_ANY);
    passPanel = new wxPanel(this, wxID_ANY);

    userCombo = new wxComboBox(userPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY);
    for (auto const& user : users) {
        userCombo->Append(user.first);
    }

    passCtrl = new wxTextCtrl(passPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);

    wxStaticBoxSizer* userSizer = new wxStaticBoxSizer(wxVERTICAL, userPanel, "Select User");
    userSizer->Add(userCombo, 0, wxALL, 5);
    userPanel->SetSizer(userSizer);

    wxStaticBoxSizer* passSizer = new wxStaticBoxSizer(wxVERTICAL, passPanel, "Enter Password");
    passSizer->Add(passCtrl, 0, wxALL, 5);
    passPanel->SetSizer(passSizer);

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(userPanel, 0, wxALL | wxEXPAND, 5);
    sizer->Add(passPanel, 0, wxALL | wxEXPAND, 5);
    sizer->Add(CreateButtonSizer(wxOK | wxCANCEL), 0, wxALL | wxEXPAND, 5);

    SetSizerAndFit(sizer);
}

void LoginDialog::OnOk(wxCommandEvent& event) {
    wxString user = userCombo->GetValue();
    wxString pass = passCtrl->GetValue();

    // Проверка пароля
    if (users[user.ToStdString()] == pass.ToStdString()) {
        selected_user = user;
        EndModal(wxID_OK);
    }
    else {
        wxMessageBox("Incorrect password", "Error", wxICON_ERROR | wxOK);
    }
}

void LoginDialog::OnCancel(wxCommandEvent& event) {
    EndModal(wxID_CANCEL);
}

class AddUserDialog : public wxDialog
{
public:
    AddUserDialog(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE, const wxString& name = wxDialogNameStr)
        : wxDialog(parent, id, title, pos, size, style, name)
    {
        wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

        wxStaticText* userLabel = new wxStaticText(this, wxID_ANY, "Username:");
        m_userText = new wxTextCtrl(this, wxID_ANY);
        sizer->Add(userLabel, 0, wxALL, 5);
        sizer->Add(m_userText, 0, wxEXPAND | wxALL, 5);

        wxStaticText* passwordLabel = new wxStaticText(this, wxID_ANY, "Password:");
        m_passwordText = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);
        sizer->Add(passwordLabel, 0, wxALL, 5);
        sizer->Add(m_passwordText, 0, wxEXPAND | wxALL, 5);

        wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
        wxButton* addButton = new wxButton(this, wxID_OK, "Add User");
        wxButton* cancelButton = new wxButton(this, wxID_CANCEL, "Cancel");
        buttonSizer->Add(addButton, 0, wxALL, 5);
        buttonSizer->Add(cancelButton, 0, wxALL, 5);
        sizer->Add(buttonSizer, 0, wxALIGN_RIGHT);

        SetSizerAndFit(sizer);
    }

    wxString GetUsername() const { return m_userText->GetValue(); }
    wxString GetPassword() const { return m_passwordText->GetValue(); }

private:
    wxTextCtrl* m_userText;
    wxTextCtrl* m_passwordText;
};

class DeleteUserDialog : public wxDialog {
public:
    DeleteUserDialog(wxWindow* parent, const std::vector<wxString>& user_list)
        : wxDialog(parent, wxID_ANY, "Delete User")
    {
        wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);

        // Создание раскрывающегося списка
        wxArrayString user_choices;
        for (auto it = user_list.begin(); it != user_list.end(); ++it) {
            user_choices.Add(*it);
        }
        m_user_choice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, user_choices);

        // Создание кнопок "OK" и "Cancel"
        wxButton* ok_button = new wxButton(this, wxID_ANY, "Delete");
        wxButton* cancel_button = new wxButton(this, wxID_ANY, "Cancel");

        // Добавление элементов на главный сизер
        main_sizer->Add(m_user_choice, wxSizerFlags().Expand().Border(wxALL, 10));
        main_sizer->AddSpacer(10);
        main_sizer->Add(ok_button, wxSizerFlags().Center().Border(wxLEFT | wxRIGHT, 10));
        main_sizer->Add(cancel_button, wxSizerFlags().Center().Border(wxLEFT | wxRIGHT | wxBOTTOM, 10));

        // Установка главного сизера
        SetSizerAndFit(main_sizer);

        // Привязка событий к кнопкам
        ok_button->Bind(wxEVT_BUTTON, &DeleteUserDialog::OnOkButtonClicked, this);
        cancel_button->Bind(wxEVT_BUTTON, &DeleteUserDialog::OnCancelButtonClicked, this);
    }

    wxString GetUserToDelete() const {
        return m_user_choice->GetStringSelection();
    }

private:
    void OnOkButtonClicked(wxCommandEvent& event) {
        EndModal(wxID_OK);
    }

    void OnCancelButtonClicked(wxCommandEvent& event) {
        EndModal(wxID_CANCEL);
    }

    wxChoice* m_user_choice;
};

class UserProgressDialog : public wxDialog {
public:
    UserProgressDialog(wxWindow* parent, const std::vector<std::tuple<std::string, int, int>>& data)
        : wxDialog(parent, wxID_ANY, "User Progress", wxDefaultPosition, wxSize(600, 500))
    {
        wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

        wxStaticText* userProgressText = new wxStaticText(this, wxID_ANY, "User Progress");
        mainSizer->Add(userProgressText, 0, wxALL, 10);

        wxGrid* grid = new wxGrid(this, wxID_ANY);
        grid->CreateGrid(data.size(), 3);

        grid->SetColLabelValue(0, "Data");
        grid->SetColLabelValue(1, "Speed");
        grid->SetColLabelValue(2, "Accuracy");

        for (size_t row = 0; row < data.size(); ++row) {
            std::string date = std::get<0>(data[row]);
            int speed = std::get<1>(data[row]);
            int accuracy = std::get<2>(data[row]);

            grid->SetCellValue(row, 0, wxString(date));
            grid->SetCellValue(row, 1, wxString::Format("%d WPM", speed));
            grid->SetCellValue(row, 2, wxString::Format("%d%%", accuracy));
        }

        mainSizer->Add(grid, 1, wxALL | wxEXPAND, 10);

        SetSizerAndFit(mainSizer);
    }
};

class MyFrame : public wxFrame
{
public:
    MyFrame();
private:
    void SetupMenuBar();
    void SetupUi();
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnProgress(wxCommandEvent& event);
    void OnCprogress(wxCommandEvent& event);
    void OnCsettings(wxCommandEvent& event);
    void OnFsettings(wxCommandEvent& event);
    void OnFileopen(wxCommandEvent& event);
    void OnStart(wxCommandEvent& event);
    void OnStop(wxCommandEvent& event);
    void OnChar(wxKeyEvent& event);
    void OnKeyDown(wxKeyEvent& event);
    void OnKeyUp(wxKeyEvent& event);
    void OnDeleteUser(wxCommandEvent& event);
    void OnAddUser(wxCommandEvent& event);
    void OnChangeUser(wxCommandEvent& event);
    void OnTimer(wxTimerEvent& event);
    void FillText();
    void AddUser(const wxString& user, const wxString& pass);
    std::string user_file_path;
    wxStaticText* text_to_show;
    file_control file_control_obj;
    user user_obj;
    progress progress_obj;
    wxTextCtrl* main_text;
    wxTextAttr main_style;
    wxTextAttr green;
    wxTextAttr red;
    std::string strrn;
    int x = 50;
    int y = 50;
    int width = 50;
    int height = 50;
    std::vector<wxButton*> buttons1;
    std::vector<wxButton*> buttons2;
    std::vector<wxButton*> buttons3;
    std::vector<wxButton*> buttons4;
    int button_count{ 0 };
    wxTimer* m_timer;
    wxStaticText* keySpeed;
    wxStaticText* keyAcc;
    wxStaticText* currUser;
    wxStaticText* currFile;
    int cps_count{ 0 };
    int max_speed{ 0 };
    int total_count{ 1 };
    int correct_count{ 1 };
    int min_acc{ 100 };
};

enum
{
    ID_Hello = 1,
    ID_Progress = 2,
    ID_Cprogress = 3,
    ID_Sprogress = 4,
    ID_Lsettings = 5,
    ID_Csettings = 6,
    ID_Fsettings = 7,
    ID_Fileopen = 8,
    ID_BUTTON_start = 9,
    ID_BUTTON_stop = 10,
    ID_BUTTON_adduser = 11,
    ID_BUTTON_deleteuser = 12,
    ID_BUTTON_changeuser = 13
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    MyFrame* frame = new MyFrame(); 
    frame->Show(true);
    SetTopWindow(frame);
    return true;
}

MyFrame::MyFrame()
    : wxFrame(NULL, wxID_ANY, "ChiTypeTrainer", wxPoint(1, 1), wxSize(900, 600))
{
    m_timer = new wxTimer(this, wxID_ANY);
    SetupMenuBar();
    SetupUi();
}

void MyFrame::SetupMenuBar() {
    wxMenu* menuFile = new wxMenu;
    menuFile->AppendSeparator();
    menuFile->Append(ID_Fileopen, "&Open",
        "Opening the user file");
    menuFile->Append(wxID_EXIT);

    wxMenu* menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);

    wxMenu* menuProgress = new wxMenu;
    menuProgress->Append(ID_Progress, "&Progress\tCtrl-Y",
        "Your progress");
    menuProgress->AppendSeparator();
    menuProgress->Append(ID_Cprogress, "&Clear progress",
        "Clear progress button");

    wxMenu* menuSettings = new wxMenu;
    menuSettings->Append(ID_Csettings, "&Cursor settings",
        "Here you can chabge cursor settings");
    menuSettings->Append(ID_Fsettings, "&Focus settings",
        "Here you can change focus settings");

    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");
    menuBar->Append(menuProgress, "&Progress");
    menuBar->Append(menuSettings, "&Settings");

    SetMenuBar(menuBar);

    CreateStatusBar();
    SetStatusText("Status bar");

    Bind(wxEVT_MENU, &MyFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_MENU, &MyFrame::OnProgress, this, ID_Progress);
    Bind(wxEVT_MENU, &MyFrame::OnCprogress, this, ID_Cprogress);
    Bind(wxEVT_MENU, &MyFrame::OnCsettings, this, ID_Csettings);
    Bind(wxEVT_MENU, &MyFrame::OnFsettings, this, ID_Fsettings);
    Bind(wxEVT_MENU, &MyFrame::OnFileopen, this, ID_Fileopen);
}

void MyFrame::SetupUi() {
    green.SetTextColour(*wxGREEN);
    red.SetTextColour(*wxRED);

    wxPanel* panel_top = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(200, 100));
    panel_top->SetBackgroundColour(wxColor(255, 255, 255));

    wxPanel* panel_bottom = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(200, 100));
    panel_top->SetBackgroundColour(wxColor(255, 255, 255));

    wxPanel* panel_bottom_left = new wxPanel(panel_bottom, wxID_ANY, wxDefaultPosition, wxSize(200, 100));
    panel_bottom_left->SetBackgroundColour(wxColor(255, 255, 255));

    wxBoxSizer* sizer_bt = new wxBoxSizer(wxVERTICAL);
    panel_bottom_left->SetSizer(sizer_bt);

    const char* letters2 = "QWERTYUIOP";
    for (int i = 0; i < strlen(letters2); i++)
    {
        wxButton* button = new wxButton(panel_bottom_left, wxID_ANY, wxString::Format("%c", letters2[i]), wxDefaultPosition, wxSize(width, height));
        button->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));
        button->SetId(letters2[i]);
        button->Disable();
        buttons2.push_back(button);
    }

    const char* letters3 = "ASDFGHJKL";
    for (int i = 0; i < strlen(letters3); i++)
    {
        wxButton* button = new wxButton(panel_bottom_left, wxID_ANY, wxString::Format("%c", letters3[i]), wxDefaultPosition, wxSize(width, height));
        button->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));
        button->SetId(letters3[i]);
        button->Disable();
        buttons3.push_back(button);
    }

    const char* letters4 = "ZXCVBNM";
    for (int i = 0; i < strlen(letters4); i++)
    {
        wxButton* button = new wxButton(panel_bottom_left, wxID_ANY, wxString::Format("%c", letters4[i]), wxDefaultPosition, wxSize(width, height));
        button->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));
        button->SetId(letters4[i]);
        button->Disable();
        buttons4.push_back(button);
    }

    for (int i = 1; i <= 9; i++)
    {
        wxButton* button = new wxButton(panel_bottom_left, wxID_ANY, wxString::Format("%d", i), wxDefaultPosition, wxSize(width, height));
        button->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));
        button->SetId(i+48);
        button->Disable();
        buttons1.push_back(button);
    }
    
    wxFlexGridSizer* grsizer1 = new wxFlexGridSizer(1, 11, 0, 0);
    grsizer1->AddSpacer(38);
    for (int i = 0; i <= 8; i++) {
        grsizer1->Add(buttons1[i], 1, wxALL, 1);
    }

    wxFlexGridSizer* grsizer2 = new wxFlexGridSizer(1, 11, 0, 0);
    grsizer2->AddSpacer(20);
    for (size_t i = 0; i < buttons2.size(); i++) {
        grsizer2->Add(buttons2[i], 1, wxALL, 1);
    }

    wxFlexGridSizer* grsizer3 = new wxFlexGridSizer(1, 11, 0, 0);
    grsizer3->AddSpacer(29);
    for (size_t i = 0; i < buttons3.size(); i++) {
        grsizer3->Add(buttons3[i], 1, wxALL, 1);
    }

    wxFlexGridSizer* grsizer4 = new wxFlexGridSizer(1, 11, 0, 0);
    grsizer4->AddSpacer(58);
    for (size_t i = 0; i < buttons4.size(); i++) {
        grsizer4->Add(buttons4[i], 1, wxALL, 1);
    }

    sizer_bt->Add(grsizer1, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 0);
    sizer_bt->AddStretchSpacer();
    sizer_bt->Add(grsizer2, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 0);
    sizer_bt->AddStretchSpacer();
    sizer_bt->Add(grsizer3, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 0);
    sizer_bt->AddStretchSpacer();
    sizer_bt->Add(grsizer4, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 0);
    sizer_bt->AddStretchSpacer();

    wxPanel* panel_bottom_right = new wxPanel(panel_bottom, wxID_ANY, wxDefaultPosition, wxSize(200, 150));
    panel_bottom_right->SetBackgroundColour(wxColor(255, 255, 255));

    wxPanel* panel_top_bottom_right = new wxPanel(panel_top, wxID_ANY, wxDefaultPosition, wxSize(200, 100));
    panel_top_bottom_right->SetBackgroundColour(wxColor(159, 146, 146));

    wxPanel* panel_top_bottom_left = new wxPanel(panel_top, wxID_ANY, wxDefaultPosition, wxSize(200, 100));
    panel_top_bottom_left->SetBackgroundColour(wxColor(159, 146, 146));

    wxBoxSizer*  top_bottom_sizer = new wxBoxSizer(wxHORIZONTAL);
    top_bottom_sizer->Add(panel_top_bottom_left, 1, wxALL, 10);
    top_bottom_sizer->Add(panel_top_bottom_right, 1, wxALL, 10);

    wxFont font(12, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    main_text = new wxTextCtrl(panel_top, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_READONLY | wxTE_NOHIDESEL | wxTE_RICH | wxTE_RICH2);
    main_text->SetFont(font);
    strrn = "Here will be placed text";
    main_text->SetValue(strrn);
    main_text->SetMaxLength(100);
    main_text->SetInsertionPoint(0);
    main_text->Disable();

    wxButton* start_button = new wxButton(panel_bottom_right, ID_BUTTON_start, "Start");
    wxButton* stop_button = new wxButton(panel_bottom_right, ID_BUTTON_stop, "Stop");
    wxButton* add_user_button = new wxButton(panel_bottom_right, ID_BUTTON_adduser, "Add user");
    wxButton* change_user_button = new wxButton(panel_bottom_right, ID_BUTTON_changeuser, "Change user");
    wxButton* delete_user_button = new wxButton(panel_bottom_right, ID_BUTTON_deleteuser, "Delete user");

    currUser = new wxStaticText(panel_bottom_right, wxID_ANY, "Current user:" + user_obj.getUser(), wxDefaultPosition, wxDefaultSize);
    wxSize textSize = currUser->GetBestSize();
    wxFont myFont(textSize.GetHeight() * 1.6, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    currUser->SetFont(myFont);

    currFile = new wxStaticText(panel_bottom_right, wxID_ANY, "Current file:default", wxDefaultPosition, wxDefaultSize);
    wxSize textSize0 = currFile->GetBestSize();
    wxFont myFont0(textSize0.GetHeight() * 1.6, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    currFile->SetFont(myFont0);

    keySpeed = new wxStaticText(panel_top_bottom_left, wxID_ANY, "WPM", wxDefaultPosition, wxDefaultSize);
    wxSize textSize1 = keySpeed->GetBestSize();
    wxFont myFont1(textSize1.GetHeight() * 1.6, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    keySpeed->SetFont(myFont1);

    keyAcc = new wxStaticText(panel_top_bottom_right, wxID_ANY, "%", wxDefaultPosition, wxDefaultSize);
    wxSize textSize2 = keyAcc->GetBestSize();
    wxFont myFont2(textSize2.GetHeight() * 1.6, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    keyAcc->SetFont(myFont2);

    wxBoxSizer* panel_top_bottom_left_sizer = new wxBoxSizer(wxVERTICAL);
    panel_top_bottom_left_sizer->AddStretchSpacer();
    panel_top_bottom_left_sizer->Add(keySpeed, 0, wxALIGN_CENTER);
    panel_top_bottom_left_sizer->AddStretchSpacer();

    wxBoxSizer* panel_top_bottom_right_sizer = new wxBoxSizer(wxVERTICAL);
    panel_top_bottom_right_sizer->AddStretchSpacer();
    panel_top_bottom_right_sizer->Add(keyAcc, 0, wxALIGN_CENTER);
    panel_top_bottom_right_sizer->AddStretchSpacer();

    panel_top_bottom_left->SetSizer(panel_top_bottom_left_sizer);
    panel_top_bottom_right->SetSizer(panel_top_bottom_right_sizer);

    wxBoxSizer* button_sizer1 = new wxBoxSizer(wxHORIZONTAL);
    button_sizer1->Add(start_button, 1, wxRIGHT, 5);
    button_sizer1->Add(stop_button, 1);

    wxBoxSizer* button_sizer2 = new  wxBoxSizer(wxHORIZONTAL);
    button_sizer2->Add(add_user_button, 1, wxRIGHT, 5);
    button_sizer2->Add(change_user_button, 1, wxRIGHT, 5);
    button_sizer2->Add(delete_user_button, 1);

    wxBoxSizer* bottom_right_sizer = new wxBoxSizer(wxVERTICAL);
    bottom_right_sizer->Add(button_sizer1, 1, wxBOTTOM | wxALIGN_CENTER_HORIZONTAL, 10);
    bottom_right_sizer->Add(currUser, 1, wxBOTTOM | wxLEFT, 10);
    bottom_right_sizer->Add(currFile, 1, wxBOTTOM | wxLEFT, 10);
    bottom_right_sizer->Add(button_sizer2, 1, wxBOTTOM | wxALIGN_CENTER_HORIZONTAL, 10);

    panel_bottom_right->SetSizerAndFit(bottom_right_sizer);

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(panel_top, 3, wxEXPAND | wxLEFT | wxTOP | wxRIGHT, 10);

    wxBoxSizer* bottom_sizer = new wxBoxSizer(wxHORIZONTAL);
    bottom_sizer->Add(panel_bottom_left, 2, wxEXPAND | wxRIGHT, 10);
    bottom_sizer->Add(panel_bottom_right, 1, wxEXPAND, 10);

    wxBoxSizer* top_sizer = new wxBoxSizer(wxVERTICAL);
    top_sizer->Add(main_text, 1, wxEXPAND | wxALL, 10);
    top_sizer->Add(top_bottom_sizer, 4, wxEXPAND | wxALL, 10);

    panel_top->SetSizerAndFit(top_sizer);
    panel_bottom->SetSizerAndFit(bottom_sizer);

    sizer->Add(panel_bottom, 4, wxEXPAND | wxALL, 10);

    this->SetSizerAndFit(sizer);

    Bind(wxEVT_BUTTON, &MyFrame::OnStart, this, ID_BUTTON_start);
    Bind(wxEVT_BUTTON, &MyFrame::OnStop, this, ID_BUTTON_stop);
    Bind(wxEVT_BUTTON, &MyFrame::OnAddUser, this, ID_BUTTON_adduser);
    Bind(wxEVT_BUTTON, &MyFrame::OnChangeUser, this, ID_BUTTON_changeuser);
    Bind(wxEVT_BUTTON, &MyFrame::OnDeleteUser, this, ID_BUTTON_deleteuser);
}


void MyFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("Это программа помогает пользователю тренировать скорость печати, написана в рамках курсовой работы",
        "О программе", wxOK | wxICON_INFORMATION);
}

void MyFrame::OnProgress(wxCommandEvent& event)
{
    UserProgressDialog* dialog = new UserProgressDialog(NULL, progress_obj.getRecords());
    dialog->ShowModal();
    dialog->Destroy();
}

void MyFrame::OnCprogress(wxCommandEvent& event)
{
    progress_obj.deleteRecords();
    wxMessageBox("Progress deleted!");
}

void MyFrame::OnCsettings(wxCommandEvent& event)
{
    MyDialog dialog(this, "Select cursor mode");
    int result = dialog.ShowModal();
    if (result == wxID_OK) {
        user_obj.SetMode(dialog.GetSelectedMode());
    }
}

void MyFrame::OnFsettings(wxCommandEvent& event)
{
    wxTextEntryDialog dialog(this, "Enter focus group in alphabetic order (ABCDE)", "Focus group", "ABFG");

    if (dialog.ShowModal() == wxID_OK) {
        std::string str = dialog.GetValue().ToStdString();
        std::set<char> charSet(str.begin(), str.end());
        std::string result(charSet.begin(), charSet.end());
        std::transform(result.begin(), result.end(), result.begin(),
            [](unsigned char c) { return std::toupper(c); });
        file_control_obj.set_letter_group(result);
    }
}

void MyFrame::OnFileopen(wxCommandEvent& event)
{
    wxFileDialog dialog(this, "Open file", "", "", "Text files (*.txt)|*.txt", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (dialog.ShowModal() == wxID_CANCEL)
        return;

    wxString path = dialog.GetPath();

    user_file_path = path.ToStdString();
    file_control_obj.set_path(user_file_path);
    currFile->SetLabel("Current file:user file");

    wxMessageBox(path, "Selected file");
}

void MyFrame::FillText() {
    strrn = file_control_obj.getRandomString();
    main_text->SetValue(strrn);
}

void MyFrame::OnStart(wxCommandEvent& event) {
    main_text->Enable();
    FillText();
    m_timer->Start(1000);
    Bind(wxEVT_TIMER, &MyFrame::OnTimer, this);
    main_text->SetFocus();
    main_text->SetInsertionPoint(0);
    main_text->Bind(wxEVT_CHAR_HOOK, &MyFrame::OnChar, this);
    main_text->Bind(wxEVT_KEY_DOWN, &MyFrame::OnKeyDown, this);
    main_text->Bind(wxEVT_KEY_UP, &MyFrame::OnKeyUp, this);
}

void MyFrame::OnStop(wxCommandEvent& event) {
    m_timer->Stop();
    Unbind(wxEVT_TIMER, &MyFrame::OnTimer, this);
    keySpeed->SetLabel("WPM");
    keyAcc->SetLabel("%");
    main_text->Unbind(wxEVT_CHAR_HOOK, &MyFrame::OnChar, this);
    main_text->Unbind(wxEVT_KEY_DOWN, &MyFrame::OnKeyDown, this);
    main_text->Unbind(wxEVT_KEY_UP, &MyFrame::OnKeyUp, this);
    main_text->Disable();
    progress_obj.addRecord(max_speed, min_acc);
    total_count = 1;
    correct_count = 1;
    max_speed = 0;
    min_acc = 100;
    cps_count = 0;
}

void MyFrame::OnChar(wxKeyEvent& event) {
    cps_count += 1;
    total_count += 1;
    int currPoint = main_text->GetInsertionPoint();
    if (event.GetUnicodeKey() == strrn[currPoint]) {
        main_text->SetStyle(currPoint, currPoint++, green);
        main_text->Refresh();
        correct_count += 1;
    }
    else {
        main_text->SetStyle(currPoint, currPoint++, red);
        if (user_obj.getMode() == 0) {
            currPoint -= 1;
        }
        main_text->Refresh();
    }
    if (currPoint-1 == strrn.length()) {
        FillText();
        main_text->SetInsertionPoint(0);
    }
    else {
        main_text->SetInsertionPoint(currPoint);
    }
    event.Skip();
}

void MyFrame::OnKeyDown(wxKeyEvent& event) {
    wxButton* button = wxDynamicCast(FindWindowById(event.GetKeyCode()), wxButton);
    if (button != NULL) {
        button->SetBackgroundColour(wxColour(0, 255, 0));
        button->Refresh();
    }
}

void MyFrame::OnKeyUp(wxKeyEvent& event) {
    wxButton* button = wxDynamicCast(FindWindowById(event.GetKeyCode()), wxButton);
    if (button != NULL) {
        button->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));
        button->Refresh();
    }
}

void MyFrame::OnChangeUser(wxCommandEvent& event) {
    LoginDialog* loginDlg = new LoginDialog(this, wxID_ANY, "Change user", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, user_obj.getUsersFromJson());

    if (loginDlg->ShowModal() == wxID_OK) {
        wxMessageBox("Login successful", "Success", wxICON_INFORMATION | wxOK);
        std::string user = (loginDlg->getUser()).ToStdString();
        currUser->SetLabel("Current user:" + user);
        user_obj.setUser(user);
        progress_obj.setUser(user);
    }
    else {
        wxMessageBox("Login cancelled", "Cancelled", wxICON_INFORMATION | wxOK);
    }

    loginDlg->Destroy();
}

void MyFrame::OnAddUser(wxCommandEvent& event) {
    AddUserDialog dialog(this, wxID_ANY, "Add user");

    if (dialog.ShowModal() == wxID_OK)
    {
        wxString username = dialog.GetUsername();
        wxString password = dialog.GetPassword();
        AddUser(username, password);
    }
}

void MyFrame::AddUser(const wxString& user, const wxString& pass) {
    user_obj.addUser(user.ToStdString(), pass.ToStdString());
}

void MyFrame::OnDeleteUser(wxCommandEvent& event) {
    std::vector<wxString> userNames;
    for (const auto& user : user_obj.getUsersFromJson())
    {
        userNames.push_back(user.first);
    }

    DeleteUserDialog dialog(this, userNames);
    if (dialog.ShowModal() == wxID_OK)
    {
        wxString selectedUser = dialog.GetUserToDelete();
        user_obj.deleteUser(selectedUser.ToStdString());
    }
}

void MyFrame::OnTimer(wxTimerEvent& event) {
    int wpm_speed = cps_count * (60 / 5);
    keySpeed->SetLabel(wxString::Format("%d WPM", wpm_speed));
    if (wpm_speed > max_speed) {
        max_speed = wpm_speed;
    }
    cps_count = 0;
    double percentage = (static_cast<double>(correct_count) / total_count) * 100;
    int roundedPercentage = static_cast<int>(percentage + 0.5);
    min_acc = roundedPercentage;
    keyAcc->SetLabel(wxString::Format("%d%%", roundedPercentage));
}