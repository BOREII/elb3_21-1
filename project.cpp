//
//  project.cpp
//  
//
//  Created by Ayazhan on 18/04/2019.
//

#include "project.hpp"

MyWin::MyWin(const wxString& title):wxFrame(NULL,wxID_ANY,title,wxDefaultPosition,wxSize(300,400)){
    
    m_pan= new wxPanel(this, wxID_ANY);
    m_pan->SetBackgroundColour(wxColour(255, 0, 127));
    tc =  new wxTextCtrl(m_pan, -1, wxT(""), wxPoint(20, 10),wxSize(200,50));
    tc->SetBackgroundColour(wxColour(255, 255, 255));
    
    //........................................NUMBER_BUTTONS........................................
    
    wxButton *button0 = new wxButton(m_pan, ID_BUTTON0, wxT("0"),wxPoint(20, 270), wxSize(40,40));
    Connect(ID_BUTTON0, wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(MyWin::on_0));
    
    wxButton *button1 = new wxButton(m_pan, ID_BUTTON1, wxT("1"),wxPoint(20, 230), wxSize(40,40));
    //button1->SetBackgroundColour(wxColour(255, 0, 127));
    Connect(ID_BUTTON1, wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(MyWin::on_1));
    
    wxButton *button2 = new wxButton(m_pan, ID_BUTTON2, wxT("2"),wxPoint(70, 230), wxSize(40,40));
    Connect(ID_BUTTON2, wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(MyWin::on_2));
    
    wxButton *button3 = new wxButton(m_pan, ID_BUTTON3, wxT("3"),wxPoint(120, 230), wxSize(40,40));
    Connect(ID_BUTTON3, wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(MyWin::on_3));
    
    wxButton *button4 = new wxButton(m_pan, ID_BUTTON4, wxT("4"),wxPoint(20, 190), wxSize(40,40));
    Connect(ID_BUTTON4, wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(MyWin::on_4));
    
    wxButton *button5 = new wxButton(m_pan, ID_BUTTON5, wxT("5"),wxPoint(70, 190), wxSize(40,40));
    Connect(ID_BUTTON5, wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(MyWin::on_5));
    
    wxButton *button6 = new wxButton(m_pan, ID_BUTTON6, wxT("6"),wxPoint(120, 190), wxSize(40,40));
    Connect(ID_BUTTON6, wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(MyWin::on_6));
    
    wxButton *button7 = new wxButton(m_pan, ID_BUTTON7, wxT("7"),wxPoint(20, 150), wxSize(40,40));
    Connect(ID_BUTTON7, wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(MyWin::on_7));
    
    wxButton *button8 = new wxButton(m_pan, ID_BUTTON8, wxT("8"),wxPoint(70, 150), wxSize(40,40));
    Connect(ID_BUTTON8, wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(MyWin::on_8));
    
    wxButton *button9 = new wxButton(m_pan, ID_BUTTON9, wxT("9"),wxPoint(120, 150), wxSize(40,40));
    Connect(ID_BUTTON9, wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(MyWin::on_9));
    
    //.....................................LEFT_MIDDLE_BUTTONS........................................
    
    wxStaticText *swap = new wxStaticText(m_pan,ID_SWAP, wxString("ln"), wxPoint(180, 140));
    wxButton *button_swap = new wxButton(m_pan, ID_SWAP, wxT("swp"),wxPoint(170, 150), wxSize(40,40));
    Connect(ID_SWAP, wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(MyWin::on_swap));
    
    wxStaticText *up = new wxStaticText(m_pan,ID_UP, wxString("exp(ix)"), wxPoint(220, 140));
    wxButton *button_up = new wxButton(m_pan, ID_UP, wxT("↑"),wxPoint(220, 150), wxSize(40,40));
    Connect(ID_UP, wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(MyWin::on_up));
    
    wxStaticText *mult = new wxStaticText(m_pan,ID_MULT, wxString("pi"), wxPoint(180, 180));
    wxButton *button_mult = new wxButton(m_pan, ID_MULT, wxT("х"),wxPoint(170, 190), wxSize(40,40));
    Connect(ID_MULT, wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(MyWin::on_mult));
    
    wxStaticText *plus = new wxStaticText(m_pan,ID_PLUS, wxString("sin"), wxPoint(230, 180));
    wxButton *button_plus = new wxButton(m_pan, ID_PLUS, wxT("+"),wxPoint(220, 190), wxSize(40,40));
    Connect(ID_PLUS, wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(MyWin::on_plus));
    
    wxStaticText *division = new wxStaticText(m_pan,ID_DIVISION, wxString("exp(x)"), wxPoint(170, 220));
    wxButton *button_division = new wxButton(m_pan, ID_DIVISION, wxT("/"),wxPoint(170, 230), wxSize(40,40));
    Connect(ID_DIVISION, wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(MyWin::on_division));
    
    wxStaticText *minus = new wxStaticText(m_pan,ID_MINUS, wxString("cos"), wxPoint(230, 220));
    wxButton *button_minus = new wxButton(m_pan, ID_MINUS, wxT("-"),wxPoint(220, 230), wxSize(40,40));
    Connect(ID_MINUS, wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(MyWin::on_minus));
    
    //........................................UPPER_BUTTONS............................................
    wxStaticText *shg = new wxStaticText(m_pan,ID_SHG, wxString("рр"), wxPoint(80, 60));
    wxButton *button_shg = new wxButton(m_pan, ID_SHG, wxT("ШГ"),wxPoint(70, 70), wxSize(40,40));
    Connect(ID_SHG, wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(MyWin::on_shg));
    
    wxStaticText *shg2 = new wxStaticText(m_pan,ID_SHG2, wxString("рп"), wxPoint(130, 60));
    wxButton *button_shg2 = new wxButton(m_pan, ID_SHG2, wxT("ШГ"),wxPoint(120, 70), wxSize(40,40));
    Connect(ID_SHG2, wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(MyWin::on_shg2));
    
    wxStaticText *vo = new wxStaticText(m_pan,ID_VO, wxString("x≥0"), wxPoint(180, 60));
    wxButton *button_vo = new wxButton(m_pan, ID_VO, wxT("В/О"),wxPoint(170, 70), wxSize(40,40));
    Connect(ID_VO, wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(MyWin::on_vo));
    
    wxStaticText *sp = new wxStaticText(m_pan,ID_SP, wxString("x≠0"), wxPoint(230, 60));
    wxButton *button_sp = new wxButton(m_pan, ID_SP, wxT("С/П"),wxPoint(220, 70), wxSize(40,40));
    Connect(ID_SP, wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(MyWin::on_sp));
    
    wxButton *button_p = new wxButton(m_pan, ID_P, wxT("P"),wxPoint(70, 110), wxSize(40,40));
    Connect(ID_P, wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(MyWin::on_p));
    
    wxStaticText *pow = new wxStaticText(m_pan,ID_POW, wxString("НОП"), wxPoint(120, 100));
    wxButton *button_pow = new wxButton(m_pan, ID_POW, wxT("x^y"),wxPoint(120, 110), wxSize(40,40));
    Connect(ID_POW, wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(MyWin::on_pow));
    
    wxStaticText *bp = new wxStaticText(m_pan,ID_BP, wxString("x=0"), wxPoint(180, 100));
    wxButton *button_bp = new wxButton(m_pan, ID_BP, wxT("БП"),wxPoint(170, 110), wxSize(40,40));
    Connect(ID_BP, wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(MyWin::on_bp));
    
    wxStaticText *pp = new wxStaticText(m_pan,ID_PP, wxString("pp"), wxPoint(230, 100));
    wxButton *button_pp = new wxButton(m_pan, ID_PP, wxT("ПП"),wxPoint(220, 110), wxSize(40,40));
    Connect(ID_PP, wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(MyWin::on_pp));
    
    wxButton *button_cx = new wxButton(m_pan, ID_CX, wxT("Cx"),wxPoint(20, 110), wxSize(40,40));
    Connect(ID_CX, wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(MyWin::on_cx));
    
    //......................................LOWER_BUTTONS..............................................
    
    wxStaticText *comma = new wxStaticText(m_pan,ID_COMMA, wxString("1/x"), wxPoint(80, 260));
    wxButton *button_comma = new wxButton(m_pan, ID_COMMA, wxT(","),wxPoint(70, 270), wxSize(40,40));
    Connect(ID_COMMA, wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(MyWin::on_comma));
    
    wxStaticText *negative = new wxStaticText(m_pan,ID_NEGATIVE, wxString("x^2"), wxPoint(130, 260));
    wxButton *button_negative = new wxButton(m_pan, ID_NEGATIVE, wxT("/-/"),wxPoint(120, 270), wxSize(40,40));
    Connect(ID_NEGATIVE, wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(MyWin::on_negative));
    
    wxStaticText *vp = new wxStaticText(m_pan,ID_VP, wxString("√x"), wxPoint(180, 260));
    wxButton *button_vp = new wxButton(m_pan, ID_VP, wxT("ВП"),wxPoint(170, 270), wxSize(40,40));
    Connect(ID_VP, wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(MyWin::on_vp));
    
    wxButton *button_f = new wxButton(m_pan, ID_F, wxT("F"),wxPoint(220, 270), wxSize(40,40));
    Connect(ID_F, wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(MyWin::on_sp));
    
    Centre();
};

//...........................................ON_BUTTON.................................................
//___________________________________________NUMBERS__________________________________________________
void MyWin::on_0(wxCommandEvent& event){
    tc->AppendText("0");
};

void MyWin::on_1(wxCommandEvent& event){
    tc->AppendText("1");
};

void MyWin::on_2(wxCommandEvent& event){
    tc->AppendText("2");
};

void MyWin::on_3(wxCommandEvent& event){
    tc->AppendText("3");
};

void MyWin::on_4(wxCommandEvent& event){
    tc->AppendText("4");
};

void MyWin::on_5(wxCommandEvent& event){
    tc->AppendText("5");
};

void MyWin::on_6(wxCommandEvent& event){
    tc->AppendText("6");
};

void MyWin::on_7(wxCommandEvent& event){
    tc->AppendText("7");
};

void MyWin::on_8(wxCommandEvent& event){
    tc->AppendText("8");
};

void MyWin::on_9(wxCommandEvent& event){
    tc->AppendText("9");
};
//__________________________________________OPERATIONS_____________________________________________

void MyWin::on_swap(wxCommandEvent& event){
    tc->Clear();
    tc->AppendText("swap");
};

void MyWin::on_up(wxCommandEvent& event){
    tc->Clear();
};

void MyWin::on_mult(wxCommandEvent& event){
    tc->Clear();
    tc->AppendText("x");
};

void MyWin::on_plus(wxCommandEvent& event){
    tc->Clear();
    tc->AppendText("+");
};

void MyWin::on_division(wxCommandEvent& event){
    tc->Clear();
    tc->AppendText("/");
};

void MyWin::on_minus(wxCommandEvent& event){
    tc->Clear();
    tc->AppendText("-");
};
//.........................................UPPER.......................................................
void MyWin::on_shg(wxCommandEvent& event){
    tc->Clear();
};

void MyWin::on_shg2(wxCommandEvent& event){
    tc->Clear();
};

void MyWin::on_vo(wxCommandEvent& event){
    tc->Clear();
};

void MyWin::on_sp(wxCommandEvent& event){
    tc->Clear();
};

void MyWin::on_p(wxCommandEvent& event){
    tc->Clear();
};

void MyWin::on_pow(wxCommandEvent& event){
    tc->Clear();
};

void MyWin::on_bp(wxCommandEvent& event){
    tc->Clear();
};

void MyWin::on_pp(wxCommandEvent& event){
    tc->Clear();
};

void MyWin::on_cx(wxCommandEvent& event){
    tc->Clear();
};
//..............................................LOWEER......................................................
void MyWin::on_comma(wxCommandEvent& event){
    tc->Clear();
};

void MyWin::on_negative(wxCommandEvent& event){
    tc->Clear();
};

void MyWin::on_vp(wxCommandEvent& event){
    tc->Clear();
};

void MyWin::on_f(wxCommandEvent& event){
    tc->Clear();
};

//..........................................................................................................

IMPLEMENT_APP(Begin)
    
bool Begin::OnInit()
{
    MyWin *wind=new MyWin(wxT("Calculator"));
    wind->Show(true);
    return true;
};
