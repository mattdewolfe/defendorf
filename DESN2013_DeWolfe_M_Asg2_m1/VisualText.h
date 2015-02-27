#pragma once
#include <string>
#include <glut.h>

class VisualText {
protected:
	float width, fontSize, spacing, r, g, b;

public:	
	// default constructor
	VisualText() {
		fontSize = 12.0;
		width = fontSize * 0.6;
		spacing = fontSize/4.0;
	}
	VisualText(float _fontSize) {
		// minimum fontsize of 1.0
		if (_fontSize < 1.0) { fontSize = 1.0; }
		else { fontSize = _fontSize; }
		width = fontSize * 0.6;
		spacing = fontSize/4.0;
	}
	VisualText(float _fontSize, float _colorR, float _colorG, float _colorB) {
		// minimum fontsize of 1.0
		if (_fontSize < 1.0) { fontSize = 1.0; }
		else { fontSize = _fontSize; }
		width = fontSize * 0.6;
		spacing = fontSize/4.0;
		r = _colorR;
		g = _colorG;
		b = _colorB;
	}

	// routine to onvert floating point to char string
	void floatToString(char * destStr, int precision, int val) 
	{
	   sprintf(destStr,"%d",val);
	   destStr[precision] = '\0';
	}

	void VisualText::SetColorFloatRGB(float _r, float _g, float _b) {
		r = _r;
		g = _g;
		b = _b;
	}
	void VisualText::ReSizeFont(float _newFontSize) {
		// minimum fontsize of 1.0
		if (_newFontSize < 1.0) { fontSize = 1.0; }
		else { fontSize = _newFontSize; }
		width = fontSize * 0.6;
		spacing = fontSize/4.0;
	}
	void VisualText::ChangeSpacing(float _newSpacing) { spacing = _newSpacing; }
	void VisualText::WriteBitmapString(float startX, float startY, char *string)
	{  
		char *c;
		glLineWidth(fontSize/4.0);
		glColor3f(r, g, b);
		glPushMatrix();
		glTranslatef(startX, startY, 0);
		for (c = string; *c != '\0'; c++) { 
			switch (*c) {
				// english alphabet letters
					case 'a': LetterA(); break;
					case 'A': LetterA(); break;
					case 'b': LetterB(); break;
					case 'B': LetterB(); break;
					case 'c': LetterC(); break;
					case 'C': LetterC(); break;
					case 'd': LetterD(); break;
					case 'D': LetterD(); break;
					case 'e': LetterE(); break;
					case 'E': LetterE(); break;
					case 'f': LetterF(); break;
					case 'F': LetterF(); break;
					case 'g': LetterG(); break;
					case 'G': LetterG(); break;
					case 'h': LetterH(); break;
					case 'H': LetterH(); break;
					case 'i': LetterI(); break;
					case 'I': LetterI(); break;
					case 'j': LetterJ(); break;
					case 'J': LetterJ(); break;
					case 'k': LetterK(); break;
					case 'K': LetterK(); break;
					case 'l': LetterL(); break;
					case 'L': LetterL(); break;
					case 'm': LetterM(); break;
					case 'M': LetterM(); break;
					case 'n': LetterN(); break;
					case 'N': LetterN(); break;
					case 'o': LetterO(); break;
					case 'O': LetterO(); break;
					case 'p': LetterP(); break;
					case 'P': LetterP(); break;
					case 'q': LetterQ(); break;
					case 'Q': LetterQ(); break;
					case 'r': LetterR(); break;
					case 'R': LetterR(); break;
					case 's': LetterS(); break;
					case 'S': LetterS(); break;
					case 't': LetterT(); break;
					case 'T': LetterT(); break;
					case 'u': LetterU(); break;
					case 'U': LetterU(); break;
					case 'v': LetterV(); break;
					case 'V': LetterV(); break;
					case 'w': LetterW(); break;
					case 'W': LetterW(); break;
					case 'x': LetterX(); break;
					case 'X': LetterX(); break;
					case 'y': LetterY(); break;
					case 'Y': LetterY(); break;
					case 'z': LetterZ(); break;
					case 'Z': LetterZ(); break;
				// numbers
					case '0': NumberZero(); break;
					case '1': NumberOne(); break;
					case '2': NumberTwo(); break;
					case '3': NumberThree(); break;
					case '4': NumberFour(); break;
					case '5': NumberFive(); break;
					case '6': NumberSix(); break;
					case '7': NumberSeven(); break;
					case '8': NumberEight(); break;
					case '9': NumberNine(); break;
				// non-alpha numeric characters
					case '-': Hyphen(); break;
					case '.': Period(); break;
					case '?': QuestionMark(); break;
					case '!': ExclamationMark(); break;
					case '+': PlusSign(); break;
					case '=': EqualSign(); break;
					case '*': Asterisk(); break;
					case '\'': Comma(); break;
					case ':': Colon(); break;
					case '#': HashMark(); break;
					case ';': SemiColon(); break;
					case '_': UnderScore(); break;
			}
			glTranslatef(width + spacing, 0, 0);
		}
		glLineWidth(1.0);
		glPopMatrix();
	}

private:
	// non-alpha numeric characters
	void VisualText::QuestionMark() {
		glBegin(GL_LINE_STRIP);
		glVertex3f(width*0.1, fontSize*0.8, 0);
		glVertex3f(width*0.5, fontSize, 0);
		glVertex3f(width*0.9, fontSize*0.8, 0);
		glVertex3f(width*0.5, fontSize*0.6, 0);
		glVertex3f(width*0.5, fontSize*0.4, 0);
		glEnd();
		glBegin(GL_LINES);
		glVertex3f(width*0.5, fontSize*0.2, 0);
		glVertex3f(width*0.5, 0, 0);
		glEnd();
	}
	void VisualText::ExclamationMark() {
		glBegin(GL_LINES);
		glVertex3f(width*0.5, fontSize, 0);
		glVertex3f(width*0.5, fontSize*0.4, 0);
		glEnd();
		glBegin(GL_LINES);
		glVertex3f(width*0.5, fontSize*0.2, 0);
		glVertex3f(width*0.5, 0, 0);
		glEnd();
	}
	void VisualText::Comma() {
		glBegin(GL_LINE_STRIP);
		glVertex3f(width*0.5, fontSize*0.9, 0);
		glVertex3f(width*0.4, fontSize*0.8, 0);
		glVertex3f(width*0.3, fontSize*0.7, 0);
		glEnd();
	}
	void VisualText::Colon() {
		glBegin(GL_LINES);
		glVertex3f(width/2.0 - spacing/3.0, fontSize/1.6, 0);
		glVertex3f(width/2.0 + spacing/3.0, fontSize/1.6, 0);
		glEnd();
		glBegin(GL_LINES);
		glVertex3f(width/2.0 + spacing/3.0, fontSize/1.5, 0);
		glVertex3f(width/2.0 - spacing/3.0, fontSize/1.5, 0);
		glEnd();
		glBegin(GL_LINES);
		glVertex3f(width/2.0 - spacing/3.0, fontSize/4.0, 0);
		glVertex3f(width/2.0 + spacing/3.0, fontSize/4.0, 0);
		glEnd();
		glBegin(GL_LINES);
		glVertex3f(width/2.0 + spacing/3.0, fontSize/3.2, 0);
		glVertex3f(width/2.0 - spacing/3.0, fontSize/3.2, 0);
		glEnd();
	}
	void VisualText::SemiColon() {
		glBegin(GL_LINES);
		glVertex3f(width/2.0 - spacing/3.0, fontSize/1.6, 0);
		glVertex3f(width/2.0 + spacing/3.0, fontSize/1.6, 0);
		glEnd();
		glBegin(GL_LINES);
		glVertex3f(width/2.0 + spacing/3.0, fontSize/1.5, 0);
		glVertex3f(width/2.0 - spacing/3.0, fontSize/1.5, 0);
		glEnd();
		glBegin(GL_LINE_STRIP);
		glVertex3f(width*0.5, fontSize*0.4, 0);
		glVertex3f(width*0.4, fontSize*0.3, 0);
		glVertex3f(width*0.3, fontSize*0.2, 0);
		glEnd();
	}
	void VisualText::HashMark() {
		glBegin(GL_LINES);
		glVertex3f(width/3.0, fontSize, 0);
		glVertex3f(width/3.0, 0, 0);
		glEnd();
		glBegin(GL_LINES);
		glVertex3f(width/1.5, fontSize, 0);
		glVertex3f(width/1.5, 0, 0);
		glEnd();
		glBegin(GL_LINES);
		glVertex3f(0, fontSize/3.0, 0);
		glVertex3f(width, fontSize/3.0, 0);
		glEnd();
		glBegin(GL_LINES);
		glVertex3f(0, fontSize/1.5, 0);
		glVertex3f(width, fontSize/1.5, 0);
		glEnd();
	}
	void VisualText::Asterisk() {
		glBegin(GL_LINES);
		glVertex3f(width/2.0, fontSize - fontSize/8.0, 0);
		glVertex3f(width/2.0, fontSize/8.0, 0);
		glEnd();
		glBegin(GL_LINES);
		glVertex3f(0, fontSize - fontSize/7.0, 0);
		glVertex3f(width, fontSize/7.0, 0);
		glEnd();
		glBegin(GL_LINES);
		glVertex3f(width, fontSize - fontSize/7.0, 0);
		glVertex3f(0, fontSize/7.0, 0);
		glEnd();
	}
	void VisualText::PlusSign() {
		glBegin(GL_LINES);
		glVertex3f(width/2.0, fontSize - fontSize/4.0, 0);
		glVertex3f(width/2.0, fontSize/4.0, 0);
		glEnd();
		glBegin(GL_LINES);
		glVertex3f(width/6.0, fontSize/2.0, 0);
		glVertex3f(width - width/6.0, fontSize/2.0, 0);
		glEnd();
	}
	void VisualText::EqualSign() {
		glBegin(GL_LINES);
		glVertex3f(width*0.3, fontSize/3.0, 0);
		glVertex3f(width*0.9, fontSize/3.0, 0);
		glEnd();
		glBegin(GL_LINES);
		glVertex3f(width*0.3, fontSize/1.5, 0);
		glVertex3f(width*0.9, fontSize/1.5, 0);
		glEnd();
	}
	void VisualText::Period() {
		glBegin(GL_LINES);
		glVertex3f(width/2.0 - spacing/3.0, 0, 0);
		glVertex3f(width/2.0 + spacing/3.0, 0, 0);
		glEnd();
		glBegin(GL_LINES);
		glVertex3f(width/2.0 + spacing/3.0, spacing/3.0, 0);
		glVertex3f(width/2.0 - spacing/3.0, spacing/3.0, 0);
		glEnd();
	}
	void VisualText::Hyphen() {
		glBegin(GL_LINES);
		glVertex3f(0 + spacing/4.0, fontSize/2.0, 0);
		glVertex3f(width - spacing/4.0, fontSize/2.0, 0);
		glEnd();
	}
	void VisualText::UnderScore() {
		glBegin(GL_LINES);
		glVertex3f(0, 0, 0);
		glVertex3f(width, 0, 0);
		glEnd();
	}
	// letter draw functions here
	void VisualText::LetterA() {
		glBegin(GL_LINE_STRIP);
		glVertex3f(0, 0, 0);
		glVertex3f(width/2.0 , fontSize, 0);
		glVertex3f(width, 0, 0);
		glEnd();
		glBegin(GL_LINES);
		glVertex3f(spacing, fontSize/2.0, 0);
		glVertex3f(width - spacing, fontSize/2.0, 0);
		glEnd();
	}
	void VisualText::LetterB() {
		glBegin(GL_LINE_STRIP);
		glVertex3f(width - width/3.0, fontSize - fontSize/8.0, 0);
		glVertex3f(0, fontSize, 0);
		glVertex3f(0, fontSize/2.0, 0);
		glVertex3f(width - width/3.0, fontSize/2.0, 0);
		glVertex3f(width - width/5.0, fontSize - fontSize/4.0, 0);
		glVertex3f(width - width/3.0, fontSize - fontSize/8.0, 0);
		glEnd();
		glBegin(GL_LINE_STRIP);
		glVertex3f(0, fontSize/2.0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(width - width/3.0, fontSize/8.0, 0);
		glVertex3f(width - width/5.0, fontSize/4.0, 0);
		glVertex3f(width - width/3.0, fontSize/2.0, 0);
		glEnd();
	}
	void VisualText::LetterC() {
		glBegin(GL_LINE_STRIP);
		glVertex3f(width, fontSize, 0);
		glVertex3f(0, fontSize*0.8, 0);
		glVertex3f(0, fontSize*0.2, 0);
		glVertex3f(width, 0, 0);
		glEnd();
	}
	void VisualText::LetterD() {
		glBegin(GL_LINE_STRIP);
		glVertex3f(spacing/2, 0, 0);
		glVertex3f(width, spacing, 0);
		glVertex3f(width, fontSize - spacing, 0);
		glVertex3f(0, fontSize, 0);
		glVertex3f(0, 0, 0);
		glEnd();
	}
	void VisualText::LetterE() {
		glBegin(GL_LINE_STRIP);
		glVertex3f(width, fontSize, 0);
		glVertex3f(0, fontSize, 0);
		glVertex3f(0, fontSize/2.0, 0);
		glVertex3f(width - width/4.0, fontSize/2.0, 0);
		glEnd();
		glBegin(GL_LINE_STRIP);
		glVertex3f(0, fontSize/2.0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(width, 0, 0);
		glEnd();
	}
	void VisualText::LetterF() {
		glBegin(GL_LINE_STRIP);
		glVertex3f(width, fontSize, 0);
		glVertex3f(0, fontSize, 0);
		glVertex3f(0, fontSize/2.0, 0);
		glVertex3f(width - width/6.0, fontSize/2.0, 0);
		glEnd();
		glBegin(GL_LINE_STRIP);
		glVertex3f(0, fontSize/2.0, 0);
		glVertex3f(0, 0, 0);
		glEnd();
	}
	void VisualText::LetterG() {
		glBegin(GL_LINE_STRIP);
		glVertex3f(width, fontSize, 0);
		glVertex3f(0, fontSize, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(width, 0, 0);
		glVertex3f(width, fontSize*0.5, 0);
		glVertex3f(width*0.3, fontSize*0.25, 0);
		glEnd();
	}
	void VisualText::LetterH() {
		glBegin(GL_LINE_STRIP);
		glVertex3f(width/10.0, fontSize, 0);
		glVertex3f(width/10.0, 0, 0);
		glEnd();
		glBegin(GL_LINE_STRIP);
		glVertex3f(width - width/10.0, fontSize, 0);
		glVertex3f(width - width/10.0, 0, 0);
		glEnd();
		glBegin(GL_LINE_STRIP);
		glVertex3f(width/10.0, fontSize - fontSize/2.0, 0);
		glVertex3f(width - width/10.0, fontSize - fontSize/2.0, 0);
		glEnd();
	}
	void VisualText::LetterI() {
		glBegin(GL_LINE_STRIP);
		glVertex3f(width/10.0, fontSize, 0);
		glVertex3f(width - width/10.0, fontSize, 0);
		glEnd();
		glBegin(GL_LINE_STRIP);
		glVertex3f(width / 2.0, fontSize, 0);
		glVertex3f(width / 2.0, 0, 0);
		glEnd();
		glBegin(GL_LINE_STRIP);
		glVertex3f(0, 0, 0);
		glVertex3f(width, 0, 0);
		glEnd();
	}
	void VisualText::LetterJ() {
		glBegin(GL_LINE_STRIP);
		glVertex3f(0, fontSize, 0);
		glVertex3f(width, fontSize, 0);
		glEnd();
		glBegin(GL_LINE_STRIP);
		glVertex3f(width/1.5, fontSize, 0);
		glVertex3f(width/1.5, fontSize/4.0, 0);
		glEnd();
		glBegin(GL_LINE_STRIP);
		glVertex3f(width/1.5, fontSize/4.0, 0);
		glVertex3f(width/2.0, 0, 0);
		glVertex3f(width/3.0, 0, 0);
		glVertex3f(0, fontSize/4.0, 0);
		glEnd();
	}
	void VisualText::LetterK() {
		glBegin(GL_LINE_STRIP);
		glVertex3f(0, fontSize, 0);
		glVertex3f(0, fontSize/2.0, 0);
		glVertex3f(width, fontSize, 0);
		glEnd();
		glBegin(GL_LINE_STRIP);
		glVertex3f(0, 0, 0);
		glVertex3f(0, fontSize/2.0, 0);
		glVertex3f(width, 0, 0);
		glEnd();
	}
	void VisualText::LetterL() {
		glBegin(GL_LINE_STRIP);
		glVertex3f(0, fontSize, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(width, 0, 0);
		glEnd();
	}
	void VisualText::LetterM() {
		glBegin(GL_LINE_STRIP);
		glVertex3f(0, 0, 0);
		glVertex3f(0, fontSize, 0);
		glVertex3f(width/2.0, fontSize/2.0, 0);
		glVertex3f(width, fontSize, 0);
		glVertex3f(width, 0, 0);
		glEnd();
	}
	void VisualText::LetterN() {
		glBegin(GL_LINE_STRIP);
		glVertex3f(0, 0, 0);
		glVertex3f(0, fontSize, 0);
		glVertex3f(width, 0, 0);
		glVertex3f(width, fontSize, 0);
		glEnd();
	}
	void VisualText::LetterO() {
		glBegin(GL_LINE_STRIP);
		glVertex3f(0, fontSize*0.8, 0);
		glVertex3f(width*0.2, fontSize, 0);
		glVertex3f(width*0.8, fontSize, 0);
		glVertex3f(width, fontSize*0.8, 0);
		glVertex3f(width, fontSize*0.2, 0);
		glVertex3f(width*0.8, 0, 0);
		glVertex3f(width*0.2, 0, 0);
		glVertex3f(0, fontSize*0.2, 0);
		glVertex3f(0, fontSize*0.8, 0);
		glEnd();
	}
	void VisualText::LetterP() {
		glBegin(GL_LINE_STRIP);
		glVertex3f(spacing/2.0, fontSize/2.0, 0);
		glVertex3f(width, fontSize/2.0 + spacing, 0);
		glVertex3f(width, fontSize - spacing/2.0, 0);
		glVertex3f(0, fontSize, 0);
		glVertex3f(0, 0, 0);
		glEnd();
	}
	void VisualText::LetterQ() {
		glBegin(GL_LINE_STRIP);
		glVertex3f(0, fontSize*0.8, 0);
		glVertex3f(width*0.2, fontSize, 0);
		glVertex3f(width*0.8, fontSize, 0);
		glVertex3f(width, fontSize*0.8, 0);
		glVertex3f(width, fontSize*0.2, 0);
		glVertex3f(width*0.8, 0, 0);
		glVertex3f(width*0.2, 0, 0);
		glVertex3f(0, fontSize*0.2, 0);
		glVertex3f(0, fontSize*0.8, 0);
		glEnd();
		glBegin(GL_LINE_STRIP);
		glVertex3f(width - spacing, spacing, 0);
		glVertex3f(width, 0, 0);
		glEnd();
	}
	void VisualText::LetterR() {
		glBegin(GL_LINE_STRIP);
		glVertex3f(0, fontSize, 0);
		glVertex3f(width, fontSize - fontSize/5.0, 0);
		glVertex3f(width, fontSize/2.0 + fontSize/5.0, 0);
		glVertex3f(0, fontSize/2.0, 0);
		glVertex3f(0, 0, 0);
		glEnd();
		glBegin(GL_LINE_STRIP);
		glVertex3f(0, fontSize/2.0, 0);
		glVertex3f(width, 0, 0);
		glEnd();
	}
	void VisualText::LetterS() {
		glBegin(GL_LINE_STRIP);
		glVertex3f(width, fontSize, 0);
		glVertex3f(0, fontSize/1.5, 0);
		glVertex3f(width, fontSize/3.0, 0);
		glVertex3f(0, 0, 0);
		glEnd();
	}
	void VisualText::LetterT() {
		glBegin(GL_LINE_STRIP);
		glVertex3f(0, fontSize, 0);
		glVertex3f(width, fontSize, 0);
		glEnd();
		glBegin(GL_LINE_STRIP);
		glVertex3f(width/2.0, fontSize, 0);
		glVertex3f(width/2.0, 0, 0);
		glEnd();
	}
	void VisualText::LetterU() {
		glBegin(GL_LINE_STRIP);
		glVertex3f(0, fontSize, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(width, fontSize/5.0, 0);
		glEnd();
		glBegin(GL_LINE_STRIP);
		glVertex3f(width, 0, 0);
		glVertex3f(width, fontSize, 0);
		glEnd();
	}
	void VisualText::LetterV() {
		glBegin(GL_LINE_STRIP);
		glVertex3f(0, fontSize, 0);
		glVertex3f(width/2.0, 0, 0);
		glVertex3f(width, fontSize, 0);
		glEnd();
	}
	void VisualText::LetterW() {
		glBegin(GL_LINE_STRIP);
		glVertex3f(0, fontSize, 0);
		glVertex3f(width/4, 0, 0);
		glVertex3f((width/4) * 2, fontSize/2.0, 0);
		glVertex3f((width/4) * 3, 0, 0);
		glVertex3f(width, fontSize, 0);
		glEnd();
	}
	void VisualText::LetterX() {
		glBegin(GL_LINE_STRIP);
		glVertex3f(0, fontSize, 0);
		glVertex3f(width, 0, 0);
		glEnd();
		glBegin(GL_LINE_STRIP);
		glVertex3f(width, fontSize, 0);
		glVertex3f(0, 0, 0);
		glEnd();
	}
	void VisualText::LetterY() {
		glBegin(GL_LINE_STRIP);
		glVertex3f(width, fontSize, 0);
		glVertex3f(0, 0, 0);
		glEnd();
		glBegin(GL_LINE_STRIP);
		glVertex3f(0, fontSize, 0);
		glVertex3f(width/2.0, fontSize/2.0, 0);
		glEnd();
	}
	void VisualText::LetterZ() {
		glBegin(GL_LINE_STRIP);
		glVertex3f(0, fontSize, 0);
		glVertex3f(width, fontSize, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(width, 0, 0);
		glEnd();
	}
	// number draw functions here
	void VisualText::NumberZero() {
		glBegin(GL_LINE_STRIP);
		glVertex3f(width, fontSize, 0);
		glVertex3f(width, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, fontSize, 0);
		glVertex3f(width, fontSize, 0);
		glEnd();
		glBegin(GL_LINES);
		glVertex3f(0, 0, 0);
		glVertex3f(width, fontSize, 0);
		glEnd();

	}
	void VisualText::NumberOne() {
		glBegin(GL_LINES);
		glVertex3f(width*0.5, fontSize*0.75, 0);
		glVertex3f(width, fontSize, 0);
		glVertex3f(width, fontSize, 0);
		glVertex3f(width, 0, 0);
		glEnd();
	}
	void VisualText::NumberTwo() {
		glBegin(GL_LINE_STRIP);
		glVertex3f(0, fontSize, 0);
		glVertex3f(width, fontSize, 0);	
		glVertex3f(width, fontSize*0.5, 0);	
		glVertex3f(0, fontSize*0.5, 0);
		glVertex3f(0, 0, 0);	
		glVertex3f(width, 0, 0);
		glEnd();
	}
	void VisualText::NumberThree() {
		glBegin(GL_LINE_STRIP);
		glVertex3f(0, fontSize*0.9, 0);
		glVertex3f(width, fontSize*0.9, 0);
		glVertex3f(width, fontSize*0.5, 0);
		glVertex3f(width*0.2, fontSize*0.5, 0);
		glEnd();
		glBegin(GL_LINE_STRIP);
		glVertex3f(width, fontSize/2.0, 0);
		glVertex3f(width, 0, 0);
		glVertex3f(0, 0, 0);
		glEnd();
	}
	void VisualText::NumberFour() {
		glBegin(GL_LINE_STRIP);
		glVertex3f(0, fontSize, 0);
		glVertex3f(0, fontSize*0.5, 0);
		glVertex3f(width, fontSize*0.5, 0);
		glEnd();
		glBegin(GL_LINES);
		glVertex3f(width*0.7, fontSize, 0);
		glVertex3f(width*0.7, 0, 0);
		glEnd();
	}
	void VisualText::NumberFive() {
		glBegin(GL_LINE_STRIP);
		glVertex3f(width, fontSize, 0);
		glVertex3f(0, fontSize, 0);
		glVertex3f(0, fontSize*0.5, 0);
		glVertex3f(width*0.9, fontSize*0.5, 0);
		glVertex3f(width*0.9, 0, 0);
		glVertex3f(0, 0, 0);
		glEnd();
	}
	void VisualText::NumberSix() {
		glBegin(GL_LINE_STRIP);
		glVertex3f(width, fontSize, 0);
		glVertex3f(0, fontSize, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(width, 0, 0);
		glVertex3f(width, fontSize*0.5, 0);
		glVertex3f(0, fontSize*0.5, 0);
		glEnd();
	}
	void VisualText::NumberSeven() {
		glBegin(GL_LINE_STRIP);
		glVertex3f(0, fontSize, 0);
		glVertex3f(width, fontSize, 0);
		glVertex3f(width*0.5, 0, 0);
		glEnd();
	}
	void VisualText::NumberEight() {
		glBegin(GL_LINE_STRIP);
		glVertex3f(width, fontSize, 0);
		glVertex3f(width, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, fontSize, 0);
		glVertex3f(width, fontSize, 0);
		glEnd();
		glBegin(GL_LINES);
		glVertex3f(0, fontSize*0.5, 0);
		glVertex3f(width, fontSize*0.5, 0);
		glEnd();
	}
	void VisualText::NumberNine() {
		glBegin(GL_LINE_STRIP);
		glVertex3f(width, fontSize*0.5, 0);
		glVertex3f(0, fontSize*0.5, 0);
		glVertex3f(0, fontSize, 0);
		glVertex3f(width, fontSize, 0);
		glVertex3f(width, 0, 0);
		glEnd();
	}
};