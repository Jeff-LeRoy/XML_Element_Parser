#pragma once

class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};


class MyFrame : public wxFrame
{
public:
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

private:
    //Declarations
    void OnExit(wxCommandEvent& event);
    void OnParse(wxCommandEvent& event);
    void OnOpen(wxCommandEvent& event);
    void OnClear(wxCommandEvent& event);
    void RecursiveElementSearch(wxXmlNode* node);
    void copy(wxKeyEvent& event);

    //Declare GUI Items
    wxListBox* m_listBox1 = nullptr;
    wxButton* m_buttonOpen = nullptr;
    wxButton* m_buttonParse = nullptr;
    wxButton* m_buttonClear = nullptr;
    wxStaticText* m_entryLabel = nullptr;
    wxTextCtrl* m_textEntry = nullptr;
    wxTextCtrl* m_TextFileLocation = nullptr;

    //XML Document
    wxXmlDocument doc;
};
