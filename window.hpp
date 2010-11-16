#ifndef WINDOW_H
#define WINDOW_H

#include <QtGui>
#include "pdfwidget.hpp"
#include "config.hpp"

class PDFWindow : public QWidget
{
public:
    typedef void (*FuncPt) (PDFWindow&);
    typedef std::map<QKeySequence, FuncPt> KeyBind;

private:
    PDFDisplay PDF;
    QVBoxLayout Layout;
    PDFViewerConfig& Config;

    KeyBind KeyBindings;
    
    void bindKeys();
    FuncPt funcFromString(const QString& str);
    
protected:
    void resizeEvent(QResizeEvent* event);
    void keyPressEvent(QKeyEvent* event);
    
public:
    PDFWindow(const QString& filename, PDFViewerConfig& config, QWidget* parent = 0);
    ~PDFWindow();

    int currentPage() { return PDF.currentPage(); }
    
    friend void moveDown(PDFWindow& pdf_win);
    friend void moveUp(PDFWindow& pdf_win);
    friend void pageDown(PDFWindow& pdf_win);
    friend void pageUp(PDFWindow& pdf_win);
    friend void nextPage(PDFWindow& pdf_win);
    friend void prevPage(PDFWindow& pdf_win);
    friend void reload(PDFWindow& pdf_win);
};

void defaultAction(PDFWindow& pdf_win);
    
void moveDown(PDFWindow& pdf_win);
void moveUp(PDFWindow& pdf_win);
void pageDown(PDFWindow& pdf_win);
void pageUp(PDFWindow& pdf_win);
void nextPage(PDFWindow& pdf_win);
void prevPage(PDFWindow& pdf_win);
void reload(PDFWindow& pdf_win);

typedef std::map<QString, PDFWindow::FuncPt> FuncNameMap;
static FuncNameMap NameToFunc;

#endif
