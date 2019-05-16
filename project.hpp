#include <wx/wx.h>
#include <wx/file.h>
#include <wx/wfstream.h>
#include <wx/txtstrm.h>
#include <wx/sstream.h>
#include <wx/string.h>
#include <stdlib.h>
#include <deque>
#include <string>
#include <array>
#include <sstream>
#include <math.h>
#define _USE_MATH_DEFINES

std::string DoubleToString(double);

enum class Button {
  ZERO,
  ONE,
  TWO,
  THREE,
  FOUR,
  FIVE,
  SIX,
  SEVEN,
  EIGHT,
  NINE,
  ADD,
  SUBTRACT,
  MULTIPLY,
  DIVIDE,
  UP,
  SWAP,
  SHG,
  SHG2,
  VO,
  SP,
  P,
  POWER,
  BP,
  PP,
  CX,
  POINT,
  NEGATIVE,
  VP,
  F
};

class MainApp: public wxApp {
public:
  bool OnInit() override;
};

class MainFrame : public wxFrame {
public:
    explicit MainFrame(const wxString& title, const wxSize& size);

    void OnZero(wxCommandEvent&);
    void OnOne(wxCommandEvent&);
    void OnTwo(wxCommandEvent&);
    void OnThree(wxCommandEvent&);
    void OnFour(wxCommandEvent&);
    void OnFive(wxCommandEvent&);
    void OnSix(wxCommandEvent&);
    void OnSeven(wxCommandEvent&);
    void OnEight(wxCommandEvent&);
    void OnNine(wxCommandEvent&);
    
    void OnAdd(wxCommandEvent&);
    void OnSubtract(wxCommandEvent&);
    void OnMultiply(wxCommandEvent&);
    void OnDivide(wxCommandEvent&);
    void OnUp(wxCommandEvent&);
    void OnSwap(wxCommandEvent&);
    
    void OnShg(wxCommandEvent&);
    void OnShg2(wxCommandEvent&);
    void OnVo(wxCommandEvent&);
    void OnSp(wxCommandEvent&);
    void OnP(wxCommandEvent&);
    void OnPower(wxCommandEvent&);
    void OnBp(wxCommandEvent&);
    void OnPp(wxCommandEvent&);
    void OnCx(wxCommandEvent&);
    
    void OnPoint(wxCommandEvent&);
    void OnNegative(wxCommandEvent&);
    void OnVp(wxCommandEvent&);
    void OnF(wxCommandEvent&);

private:
  wxPanel*          panel;
  wxTextCtrl* textControl;

  const wxSize buttonSize = wxSize(40, 40);

  static size_t MakeId (Button button) { return static_cast<size_t>(button); }
};

class Calculator{
public:
    bool state;
    bool p_button;
    bool f_button;
    bool vp_button;
    std::deque<double> regs;
    std::array<int, 60> ram;
    std::string number;
    
    Calculator();
};

