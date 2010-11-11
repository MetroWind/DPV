#ifndef WINDOW_H
#define WINDOW_H

#include <QtGui>
#include "pdfwidget.hpp"
#include "config.hpp"

class PDFWindow : public QWidget
{
private:
    typedef void (*FuncPt) (PDFWindow&);
    typedef std::map<QKeySequence, FuncPt> KeyBind;

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

    friend void moveDown(PDFWindow& pdf_win);
    friend void moveUp(PDFWindow& pdf_win);
};

void defaultAction(PDFWindow& pdf_win);
    
void moveDown(PDFWindow& pdf_win);
void moveUp(PDFWindow& pdf_win);

#endif
