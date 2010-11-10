#include <list>
#include <QtGui>
#include <QImage>
#include <QRect>
#include <QPoint>
#include "pdfrenderer.hpp"
#include "config.hpp"

class PDFDisplay : public QWidget
{
private:
    int CurrentPage;
    int DPI;
    QPoint ViewPortTopLeftInPage;
    int LastPageShown;
    int ViewPortBotInLastPageY;

    QPixmap CurrentImg;
    // QPainter Renderer;
    PDFRenderer PDF;
    PDFViewerConfig& Config;

    void updateLastPageShownInfo();
    
protected:
    void paintEvent(QPaintEvent* event);
    void resizeEvent(QResizeEvent* event);
    void keyPressEvent(QKeyEvent* event);

public:
    PDFDisplay(const QString& filename, PDFViewerConfig& config, QWidget* parent = 0);
    ~PDFDisplay();

    void setDPI(const int dpi);

    // void loadFromFile(const QString& filename);
    // void renderPDF(QPainter& painter);
    void renderPDF(const int page,
                   const QPoint& offset_on_pdf, const QRect& region_on_widget);
    void moveDown(const int offset);
};
