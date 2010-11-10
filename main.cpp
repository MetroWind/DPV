#include <QtGui>
#include "pdfwidget.hpp"

int main(int argc, char* argv[])
{
    QApplication App(argc, argv);
    PDFViewerConfig Config;
    QWidget MainWin;
    QVBoxLayout Layout;
    PDFDisplay PDFWidget(QString(argv[1]), Config);
    // PDFWidget.setDPI(20);
    
    Layout.addWidget(&PDFWidget);
    Layout.setContentsMargins(0, 0, 0, 0);
    MainWin.setLayout(&Layout);
    
    MainWin.show();
    // PDFWidget.loadFromFile(argv[1]);
    // PDFWidget.renderPDF();
    PDFWidget.setFocus(Qt::OtherFocusReason);
    
    return App.exec();
}
