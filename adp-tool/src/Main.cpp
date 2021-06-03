#include <vector>

#include "wx/wx.h"
#include "wx/notebook.h"

#include "Adp.h"
#include "Assets/Assets.h"

#include "View/IdleTab.h"
#include "View/SensitivityTab.h"
#include "View/MappingTab.h"
#include "View/LightsTab.h"
#include "View/DeviceTab.h"
#include "View/AboutTab.h"
#include "View/LogTab.h"

#include "Model/Log.h"

namespace adp {

// ====================================================================================================================
// Main window.
// ====================================================================================================================

class MainWindow : public wxFrame
{
public:
<<<<<<< HEAD:config-tool/src/Main.cpp
    MainWindow(const wchar_t* versionString)
        : wxFrame(nullptr, wxID_ANY, "FSR Mini Pad Config", wxDefaultPosition, wxSize(500, 500))
=======
    MainWindow() : wxFrame(nullptr, wxID_ANY, "ADP Tool", wxDefaultPosition, wxSize(500, 500))
>>>>>>> c7f7c93a2f3e053ff796ba7e34acb00b38b50155:adp-tool/src/Main.cpp
    {
        SetMinClientSize(wxSize(400, 400));

        SetStatusBar(CreateStatusBar(2));

        auto sizer = new wxBoxSizer(wxVERTICAL);
        myTabs = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNB_NOPAGETHEME);
        AddTab(0, new AboutTab(myTabs, versionString), AboutTab::Title);
        AddTab(1, new LogTab(myTabs), LogTab::Title);
        sizer->Add(myTabs, 1, wxEXPAND);
        SetSizer(sizer);
        UpdatePages();

        myUpdateTimer = make_unique<UpdateTimer>(this);
        myUpdateTimer->Start(10);
    }

    ~MainWindow()
    {
        myUpdateTimer->Stop();
    }

    void Tick()
    {
        auto changes = Device::Update();

        if (changes & DCF_DEVICE)
            UpdatePages();

        if (changes & (DCF_DEVICE | DCF_NAME))
            UpdateStatusText();

        UpdatePollingRate();

        if (changes)
        {
            for (auto tab : myTabList)
                tab->HandleChanges(changes);
        }

        auto activeTab = GetActiveTab();
        if (activeTab)
            activeTab->Tick();
    }

private:
    void UpdatePages()
    {
        // Delete all pages except "About" and "Log" at the end.
        while (myTabs->GetPageCount() > 2)
        {
            myTabList.erase(myTabList.begin());
            myTabs->DeletePage(0);
        }

        auto pad = Device::Pad();
        if (pad)
        {
            AddTab(0, new SensitivityTab(myTabs, pad), SensitivityTab::Title, true);
            AddTab(1, new MappingTab(myTabs, pad), MappingTab::Title);
            AddTab(2, new DeviceTab(myTabs), DeviceTab::Title);
        }
        else
        {
            AddTab(0, new IdleTab(myTabs), IdleTab::Title, true);
        }
    }

    void UpdateStatusText()
    {
        auto pad = Device::Pad();
        if (pad)
            SetStatusText(L"Connected to: " + pad->name, 0);
        else
            SetStatusText(wxEmptyString, 0);
    }

    void UpdatePollingRate()
    {
        auto rate = Device::PollingRate();
        if (rate > 0)
            SetStatusText(wxString::Format("%iHz", rate), 1);
        else
            SetStatusText(wxEmptyString, 1);
    }

    void AddTab(int index, BaseTab* tab, const wchar_t* title, bool select = false)
    {
        myTabs->InsertPage(index, tab->GetWindow(), title, select);
        myTabList.insert(myTabList.begin() + index, tab);
    }

    BaseTab* GetActiveTab()
    {
        auto page = myTabs->GetCurrentPage();
        for (auto tab : myTabList)
        {
            if (tab->GetWindow() == page)
                return tab;
        }
        return nullptr;
    }

    struct UpdateTimer : public wxTimer
    {
        UpdateTimer(MainWindow* owner) : owner(owner) {}
        void Notify() override { owner->Tick(); }
        MainWindow* owner;
    };

    // BasicDrawPane* myDrawPane;
    wxNotebook* myTabs;
    vector<BaseTab*> myTabList;
    unique_ptr<wxTimer> myUpdateTimer;
};

// ====================================================================================================================
// Application.
// ====================================================================================================================

class Application : public wxApp
{
public:
    bool OnInit() override
    {
        if (!wxApp::OnInit())
            return false;

        Log::Init();

        auto versionString = L"FSR Mini Pad Config Tool "
            + to_wstring(ADP_VERSION_MAJOR) + L"." + to_wstring(ADP_VERSION_MINOR);

        auto now = wxDateTime::Now().FormatISOCombined(' ');
        Log::Writef(L"Application started: %s - %s", versionString.data(), now.wc_str());

        Assets::Init();
        Device::Init();

        wxImage::AddHandler(new wxPNGHandler());

        wxIconBundle icons;

        //todo fix linux
#ifdef _MSC_VER
        icons.AddIcon(Files::Icon16(), wxBITMAP_TYPE_PNG);
        icons.AddIcon(Files::Icon32(), wxBITMAP_TYPE_PNG);
        icons.AddIcon(Files::Icon64(), wxBITMAP_TYPE_PNG);
#endif // _MSC_VER

        myWindow = new MainWindow(versionString.data());
        myWindow->SetIcons(icons);
        myWindow->Show();

        return true;
    }

    int OnExit() override
    {
        Device::Shutdown();
        Assets::Shutdown();
        Log::Shutdown();

        return wxApp::OnExit();
    }

private:
    MainWindow* myWindow;
};

}; // namespace adp.

wxIMPLEMENT_APP(adp::Application);