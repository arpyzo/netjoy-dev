#include "debug.h"
#include "main.h"

IMPLEMENT_APP(App)

/***************************** Frame ******************************/
BEGIN_EVENT_TABLE(Frame,wxFrame)
    EVT_MENU    (MENU_ABOUT, Frame::OnMenuAbout)
    EVT_MENU    (MENU_QUIT,  Frame::OnMenuQuit)
    EVT_BUTTON  (PANEL_CAPTURE, Frame::OnPanelCapture)
    EVT_TIMER   (TIMER, Frame::OnTimerCapture)
END_EVENT_TABLE()

Frame::Frame() 
: wxFrame((wxFrame *)NULL, -1, "NetJoy", wxPoint(200,200), wxSize(500,800)) {
    SetIcon(wxIcon("NetJoy.ico"));

    SetupMenu();

        // Create main vertical sizer
    wxBoxSizer *main_vsizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(main_vsizer);

        // Add a panel to main sizer
    wxPanel *panel = new wxPanel(this, wxID_ANY, wxDefaultPosition);
    main_vsizer->Add(panel, 0, wxEXPAND);

        // Add horizontal controls to panel
    wxBoxSizer *panel_hsizer = new wxBoxSizer(wxHORIZONTAL);
    panel->SetSizer(panel_hsizer);

    nic_choice = new wxChoice(panel, wxID_ANY);
    panel_hsizer->Add(nic_choice, 0, wxALL, 10);

    capture_button = new wxButton(panel, PANEL_CAPTURE, "Start");
    panel_hsizer->Add(capture_button, 0, wxALL, 10);
    
        // Add log message area to main sizer
    wxTextCtrl *logger_text_ctrl = new wxTextCtrl(this,wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);
    main_vsizer->Add(logger_text_ctrl, 1, wxEXPAND);

    capture_timer = new wxTimer(this, TIMER);

    SetupLogger(logger_text_ctrl);

    SetupNetInterface();

    SetupPostgres();
}

void Frame::SetupNetInterface() {
    net_driver = new NetDriver();

    //net_driver->GetNicNames(nic_names);
    //nic_choice->Append(wxArrayString(2, (const char **)net_driver->GetNicNames()));
    vector<string> nic_names = net_driver->GetNicNames();
    for (vector<string>::iterator it = nic_names.begin(); it != nic_names.end(); ++it) {
        nic_choice->Append(*it);
    }
    nic_choice->SetSelection(0);
}

void Frame::SetupPostgres() {
    postgres = new Postgres();
}

void Frame::SetupMenu() {
    wxMenuBar *menu_bar = new wxMenuBar();
    wxMenu *file_menu = new wxMenu();
    wxMenu *help_menu = new wxMenu();
    
    file_menu->Append(MENU_QUIT, "E&xit");
    menu_bar->Append(file_menu, "&File");

    help_menu->Append(MENU_ABOUT, "&About..");
    menu_bar->Append(help_menu, "&Help");

    SetMenuBar(menu_bar);
}

void Frame::SetupLogger(wxTextCtrl *logger_text_ctrl) {
	Logger::GetInstance()->SetOutput(logger_text_ctrl);
}

void Frame::OnMenuAbout(wxCommandEvent &WXUNUSED(event)) {
    wxMessageBox("NetJoy\nRobert Pyzalski 2014", "About..", wxICON_INFORMATION);
}

void Frame::OnMenuQuit(wxCommandEvent &WXUNUSED(event)) {
    Close(TRUE);
}

void Frame::OnPanelCapture(wxCommandEvent &WXUNUSED(event)) {
    //net_driver->ToggleCapture(nic_choice->GetString(nic_choice->GetCurrentSelection()).char_str());
    net_driver->ToggleCapture(nic_choice->GetSelection());
    // TODO: check for success acquiring timer
    capture_timer->Start(1, true);
    Logger::GetInstance()->Debug("Timer started.\n");
}

void Frame::OnTimerCapture(wxTimerEvent &WXUNUSED(event)) {
    //Logger::GetInstance()->Debug("Timer fired!\n");
    net_driver->GetPackets();
    capture_timer->Start(100, true);
}

Frame::~Frame() {
    delete capture_timer;
    delete net_driver;
    delete postgres;
    Logger::Release();
}

/*************************** App ****************************/
bool App::OnInit() {
    Frame *main_frame = new Frame();
    main_frame->Show(TRUE);

    return TRUE;
}

