//
//  project.hpp
//  
//
//  Created by Ayazhan on 18/04/2019.
//

#ifndef project_hpp
#define project_hpp

#include <stdio.h>
#include <iostream>
#include <wx/wx.h>
#include <wx/file.h>
#include <wx/wfstream.h>
#include <wx/txtstrm.h>
#include <wx/sstream.h>
#include <wx/string.h>


class Begin: public wxApp{
public:
    virtual bool OnInit();
};

class MyWin;

class MyWin:public wxFrame{
    wxTextCtrl *tc;
    wxButton *my_button1;
    wxButton *my_button2;
    wxPanel *m_pan;
public:
    //frame that contains all the buttons
    MyWin(const wxString& title);
    
    void on_1(wxCommandEvent& event);
    void on_2(wxCommandEvent& event);
    void on_3(wxCommandEvent& event);
    void on_4(wxCommandEvent& event);
    void on_5(wxCommandEvent& event);
    void on_6(wxCommandEvent& event);
    void on_7(wxCommandEvent& event);
    void on_8(wxCommandEvent& event);
    void on_9(wxCommandEvent& event);
    void on_0(wxCommandEvent& event);
    
    void on_swap(wxCommandEvent& event);
    void on_up(wxCommandEvent& event);
    void on_mult(wxCommandEvent& event);
    void on_plus(wxCommandEvent& event);
    void on_division(wxCommandEvent& event);
    void on_minus(wxCommandEvent& event);
    
    void on_shg(wxCommandEvent& event);
    void on_shg2(wxCommandEvent& event);
    void on_vo(wxCommandEvent& event);
    void on_sp(wxCommandEvent& event);
    void on_p(wxCommandEvent& event);
    void on_pow(wxCommandEvent& event);
    void on_bp(wxCommandEvent& event);
    void on_pp(wxCommandEvent& event);
    
    void on_cx(wxCommandEvent& event);
    
    void on_comma(wxCommandEvent& event);
    void on_negative(wxCommandEvent& event);
    void on_vp(wxCommandEvent& event);
    void on_f(wxCommandEvent& event);
};

enum buttons{
    ID_BUTTON0 = 1500,
    ID_BUTTON1,
    ID_BUTTON2,
    ID_BUTTON3,
    ID_BUTTON4,
    ID_BUTTON5,
    ID_BUTTON6,
    ID_BUTTON7,
    ID_BUTTON8,
    ID_BUTTON9,
    ID_MINUS,
    ID_DIVISION,
    ID_PLUS,
    ID_MULT,
    ID_UP,
    ID_SWAP,
    ID_COMMA,
    ID_NEGATIVE,
    ID_VP,
    ID_F,
    ID_P,
    ID_POW,
    ID_BP,
    ID_PP,
    ID_SHG,
    ID_SHG2,
    ID_VO,
    ID_SP,
    ID_CX
};

#endif 
