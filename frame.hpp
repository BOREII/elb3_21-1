#include <wx/wx.h>
#include <wx/file.h>
#include <wx/wfstream.h>
#include <wx/txtstrm.h>
#include <wx/sstream.h>
#include <wx/string.h>

#include <deque>
#include <string>
#include <array>
#include <sstream>
#include <cmath>

#include "computations.hpp"

class MainApp: public wxApp {
public:
	bool OnInit() override;
};

class MainFrame : public wxFrame {
public:
	explicit MainFrame(const wxString& title, const wxSize& size);

	void OnZero (wxCommandEvent&);
	void OnOne  (wxCommandEvent&);
	void OnTwo  (wxCommandEvent&);
	void OnThree(wxCommandEvent&);
	void OnFour (wxCommandEvent&);
	void OnFive (wxCommandEvent&);
	void OnSix  (wxCommandEvent&);
	void OnSeven(wxCommandEvent&);
	void OnEight(wxCommandEvent&);
	void OnNine (wxCommandEvent&);

	void OnAddOrSin     (wxCommandEvent&);
	void OnSubtractOrCos(wxCommandEvent&);
	void OnMultiplyOrPi (wxCommandEvent&);
	void OnDivideOrExp  (wxCommandEvent&);
	void OnInsert       (wxCommandEvent&);
	void OnSwapOrLog    (wxCommandEvent&);

	void OnPointOrInverseOrScrollLeft   (wxCommandEvent&);
	void OnNegativeOrSquareOrScrollRight(wxCommandEvent&);
	void OnScientificNotationOrSqrt     (wxCommandEvent&);
	void OnF                            (wxCommandEvent&);

	void OnP    (wxCommandEvent&);
	void OnPower(wxCommandEvent&);
	void OnShg  (wxCommandEvent&);
	void OnShg2 (wxCommandEvent&);
	void OnVo   (wxCommandEvent&);
	void OnSp   (wxCommandEvent&);
	void OnBp   (wxCommandEvent&);
	void OnPp   (wxCommandEvent&);
	void OnCx   (wxCommandEvent&);


private:
	const wxSize BUTTON_SIZE = {40, 40};
	static size_t MakeId (Button button) { return static_cast<size_t>(button); }

	// Принимает либо метку на простую операцию, либо на операцию с записываемым текстом
	void UpdateTextCtrl(Calculator::TextCtrlModifier item);

	wxPanel*    panel     ;
	wxTextCtrl* text_ctrl ;
	Calculator  calculator;
};