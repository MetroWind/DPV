#include <QtGui>
#include "window.hpp"

int main(int argc, char* argv[])
{
    QApplication App(argc, argv);
    PDFViewerConfig Config;
    PDFWindow MainWin(argv[1], Config);

    MainWin.show();
    
    return App.exec();
}
