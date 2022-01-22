/////////////////////////////////////////////////////////////////////////////
// Name:        XML_Element_Parser.cpp
// Purpose:     Print XML nodes of user defined element name
// Author:      Jeffrey C. LeRoy
// Created:     01/06/2022
// Copyright:   (c) Jeffrey C. LeRoy
/////////////////////////////////////////////////////////////////////////////

#include <wx/wxprec.h>
#include <wx/xml/xml.h>
#include <wx/stattext.h>
#include <wx/event.h>
#include <wx/filedlg.h>
#include <wx/clipbrd.h>
#include "XML_Element_Parser.h"
#include <wx/strconv.h>

//Delegating constructor
MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size) : wxFrame(NULL, wxID_ANY, title, pos, size)
{
    wxBoxSizer* sizerMain = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizerButtons = new wxBoxSizer(wxHORIZONTAL);

    //File menu item
    wxMenu* menuFile = new wxMenu;
    wxMenuItem* menuOpen = new wxMenuItem(menuFile, wxID_OPEN, "Open XML");
    Bind(wxEVT_MENU, &MyFrame::OnOpen, this, menuOpen->GetId());
    wxMenuItem* menuExit = new wxMenuItem(menuFile, wxID_EXIT);
    Bind(wxEVT_MENU, &MyFrame::OnExit, this, menuExit->GetId());
    menuFile->Append(menuOpen);
    menuFile->AppendSeparator();
    menuFile->Append(menuExit);

    //Menu bar
    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    SetMenuBar(menuBar);

    //Status bar    
    wxStatusBar* statusBar = CreateStatusBar();

    //GUI buttons and elements
    //wxPanel* mainPanel = new wxPanel(this, wxID_ANY);

    sizerButtons->Add(
        new wxButton(this, 102, "Open XML", wxDefaultPosition, wxSize(68, 50)),
        0,
        wxALL, 10);

    sizerButtons->Add(
        new wxButton(this, 101, "Parse", wxDefaultPosition, wxSize(68, 50)),
        0,
        wxALL, 10);

    sizerButtons->Add(
        new wxButton(this, 103, "Clear", wxDefaultPosition, wxSize(68, 50)),
        0,
        wxALL, 10);

    //m_buttonOpen = new wxButton(mainPanel, 102, "Open XML", wxPoint(10, 10), wxSize(68, 50));
    //m_buttonParse = new wxButton(mainPanel, 101, "Parse", wxPoint(88, 10), wxSize(68, 50));
    //m_buttonClear = new wxButton(mainPanel, 103, "Clear", wxPoint(166, 10), wxSize(68, 50));

    m_entryLabel = new wxStaticText(this, wxID_ANY, "Print Elements With Name :", wxPoint(10, 95), wxSize(150, 15));
    m_textEntry = new wxTextCtrl(this, wxID_ANY, "", wxPoint(10, 110), wxSize(224, 20));
    m_TextFileLocation = new wxTextCtrl(this, wxID_ANY, "", wxPoint(10, 70), wxSize(224, 20), wxTE_READONLY);
    m_listBox1 = new wxListBox(this, wxID_ANY, wxPoint(10, 140), wxSize(224, 260), 0, NULL, wxLB_HSCROLL);

    Bind(wxEVT_BUTTON, &MyFrame::OnOpen, this, 102);
    Bind(wxEVT_BUTTON, &MyFrame::OnParse, this, 101);
    Bind(wxEVT_BUTTON, &MyFrame::OnClear, this, 103);
    m_listBox1->Bind(wxEVT_KEY_DOWN, &MyFrame::copy, this);

    sizerMain->Add(sizerButtons, 0, wxALIGN_CENTER);
    sizerMain->Add(m_entryLabel, 0, wxEXPAND | wxALL, 3);
    sizerMain->Add(m_textEntry, 0, wxEXPAND | wxALL, 3);
    sizerMain->Add(m_TextFileLocation, 0, wxEXPAND | wxALL, 3);
    sizerMain->Add(m_listBox1, 1, wxEXPAND | wxALL, 3);

    SetSizerAndFit(sizerMain);
}

void MyFrame::OnOpen(wxCommandEvent& event)
{
    wxFileDialog openFileDialog
    (this, _("Open XML"), "", "", "XML files (*.xml)|*.xml", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;

    //Get path to XML location
    wxString fileLocation = (openFileDialog.GetPath());
    wxString fileName = openFileDialog.GetFilename();

    //Handle loading XML
    if (!doc.Load(fileLocation)) //, "utf-8"
    {
        wxString encoding = doc.GetFileEncoding();
        GetStatusBar();
        SetStatusText("Failed To Load XML. Encoding is " + encoding);
    }
    else
    {
        GetStatusBar();
        SetStatusText("Successfully Loaded " + fileName);
    }

    m_TextFileLocation->AppendText(fileLocation);
}

void MyFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}

void MyFrame::OnParse(wxCommandEvent& event)
{
    ////Print info of XML
    //m_listBox1->Append(doc.GetRoot()->GetName() + " << Root Node");
    //m_listBox1->Append("");

    if (!doc.IsOk())
        wxMessageBox("No XML document loaded");
    else
    {
        //Start searching through children of ROOT
        wxXmlNode* rootChild = doc.GetRoot()->GetChildren();
        MyFrame::RecursiveElementSearch(rootChild);
    }
}

void MyFrame::OnClear(wxCommandEvent& event)
{
    m_listBox1->Clear();
}

void MyFrame::RecursiveElementSearch(wxXmlNode* node)
{
    while (node)
    {
        //Bad node so skip 
        if (node->GetChildren() == NULL)
        {
            //wxString lineNum = wxString::Format(wxT("%d"), node->GetLineNumber());
            //m_listBox1->Append("Bad node at line " + lineNum);
            node = node->GetNext();
            continue;
        }
        //Child is text node so we know to print
        else if (node->GetChildren()->GetType() == wxXML_TEXT_NODE)
        {
            if (m_textEntry->IsEmpty())
                m_listBox1->Append(node->GetNodeContent());
            else
            {
                if (node->GetName() == m_textEntry->GetLineText(0))
                    m_listBox1->Append(node->GetNodeContent());
            }
        }
        //Child is element node so call function again
        else if (node->GetChildren()->GetType() == wxXML_ELEMENT_NODE)
        {
            MyFrame::RecursiveElementSearch(node->GetChildren());
        }
        node = node->GetNext();
    }
}

void MyFrame::copy(wxKeyEvent& event)
{
    wxChar key = event.GetUnicodeKey();

    if (key == 'C' && event.ControlDown())
    {
        wxString selectedText = m_listBox1->GetStringSelection();

        if (wxTheClipboard->Open())
        {
            wxTheClipboard->SetData(new wxTextDataObject(selectedText));
            wxTheClipboard->Close();
        }
    }
}

//Main
wxIMPLEMENT_APP(MyApp);

//Entry point of our application
bool MyApp::OnInit()
{
    //Create an instance of class MyFrame
    MyFrame* frame = new MyFrame{ "XML Element Parser", wxPoint(100, 100), wxSize(260, 500) };
    frame->Centre(wxBOTH);



    frame->Show(true);
    return true;
}

