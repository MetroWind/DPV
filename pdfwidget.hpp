#ifndef PDFWIDGET_H
#define PDFWIDGET_H

#include <list>
#include <QtGui>
#include <QImage>
#include <QRect>
#include <QPoint>
#include "pdfrenderer.hpp"

class PDFDisplay : public QWidget
{
private:
    int CurrentPage;
    int DPI;
    QPoint ViewPortTopLeftInPage;
    int LastPageShown;
    int ViewPortBotInLastPageY;

    QPixmap CurrentImg;
    PDFRenderer PDF;
    const int OffsetBetweenPages;

    void updateLastPageShownInfo();
    void forceRepaint();        // Updates `CurrentImg', but does not repaint.
    
protected:
    void paintEvent(QPaintEvent* event);
    void resizeEvent(QResizeEvent* event);

public:
    PDFDisplay(const QString& filename, const int initial_dpi,
               const int offset_between_pages, QWidget* parent = 0);
    ~PDFDisplay();

    void setDPI(const int dpi);
    int currentPage() { return CurrentPage; }
    
    // void loadFromFile(const QString& filename);
    // void renderPDF(QPainter& painter);
    void renderPDF(const int page,
                   const QPoint& offset_on_pdf, const QRect& region_on_widget);
    void moveDown(const int offset);
    void moveUp(const int offset);
    void goTo(const int page);
};

#endif
