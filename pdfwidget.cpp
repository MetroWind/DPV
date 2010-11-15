#include <iostream>
#include <assert.h>
#include <time.h>
#include "pdfwidget.hpp"

PDFDisplay :: PDFDisplay(const QString& filename, const int initial_dpi,
                         const int offset_between_pages, QWidget* parent)
: DPI(initial_dpi), OffsetBetweenPages(offset_between_pages), QWidget(parent)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    // setStyleSheet("background-color: blue;");

    PDF.loadFromFile(filename);
    CurrentPage = 0;
    LastPageShown = 0;
    ViewPortTopLeftInPage.setX(0);
    ViewPortTopLeftInPage.setY(0);
}

PDFDisplay :: ~PDFDisplay()
{
}

// void PDFDisplay :: loadFromFile(const QString& filename)
// {
//     PDF.loadFromFile(filename);
//     return;
// }

void PDFDisplay :: updateLastPageShownInfo()
{
    QSize PageSize = PDF.pageSize(CurrentPage, DPI);
    int HeightRemain = height() - (PageSize.height()
                                   - ViewPortTopLeftInPage.y())
        - OffsetBetweenPages;
    int LastPage = CurrentPage;
    while(HeightRemain > 0)
    {
        LastPage++;
        HeightRemain -= PDF.pageSize(LastPage, DPI).height() + OffsetBetweenPages;
    }

    LastPageShown = LastPage;
    ViewPortBotInLastPageY = PDF.pageSize(LastPage, DPI).height() + HeightRemain + OffsetBetweenPages;

    return;
}
    
void PDFDisplay :: renderPDF(const int page,
                             const QPoint& offset_on_pdf, const QRect& region_on_widget)
{
    std::cerr << "Entering PDFDisplay :: renderPDF." << std::endl;
    std::cerr << '(' << offset_on_pdf.x() << ", " << offset_on_pdf.y() << ") --> "
              << region_on_widget.width() << 'x' << region_on_widget.height()
              << '+' << region_on_widget.x() << '+' << region_on_widget.y() << std::endl;
    
    QPainter Renderer(&CurrentImg);
// I want to see how long a render takes.
    clock_t RenderStart = clock();
    
    // Render!!
    int HeightRemain = region_on_widget.height();
    int PageToRender = page;
    // bool isFirstPage = true;
    int Y = offset_on_pdf.y(); // Where are we on the pdf in the current page?
    while(HeightRemain > 0 && PageToRender < PDF.totalPages())
    {
        QImage RenderedImg;
        QSize PageSize = PDF.pageSize(PageToRender, DPI);
        // Do we need to go across pages? ...
        if(HeightRemain - OffsetBetweenPages > PageSize.height() - Y)
        {                       // ... Yes, we do.
            RenderedImg = PDF.render(PageToRender, DPI, offset_on_pdf.x(), Y,
                                     region_on_widget.width(), PageSize.height() - Y);
            
        }
        else
        {                       // ... No, we don't.  And this is the last we draw. ^_^
            RenderedImg = PDF.render(PageToRender, DPI, offset_on_pdf.x(), Y,
                                     region_on_widget.width(), HeightRemain);
        }
        Renderer.drawImage(region_on_widget.x(), region_on_widget.y()
                           + region_on_widget.height() - HeightRemain, RenderedImg);

        HeightRemain -= PageSize.height() - Y;
        HeightRemain -= OffsetBetweenPages;
        PageToRender++;         // Go across page!
        Y = 0;
    }
    
    std::cerr << "HeightRemain = " << HeightRemain << std::endl;
    
    clock_t RenderStop = clock();
    std::cerr << "Render takes " << double(RenderStop - RenderStart) /
        (double(CLOCKS_PER_SEC) / 1000.0)
              << "ms" << std::endl;
    std::cerr << "Leaving PDFRenderer :: render" << std::endl;
    // RenderedImg.save("test.png");

    return;
}

void PDFDisplay :: resizeEvent(QResizeEvent* event)
{
    std::cerr << "Entering PDFDisplay :: resizeEvent." << std::endl;
    CurrentImg = QPixmap(width(), height());
    renderPDF(CurrentPage, QPoint(ViewPortTopLeftInPage.x(), ViewPortTopLeftInPage.y()),
              QRect(0, 0, width(), height()));
    QWidget::resizeEvent(event);
    std::cerr << "Leaving PDFDisplay :: resizeEvent." << std::endl;
    updateLastPageShownInfo();
    return;
}

void PDFDisplay :: paintEvent(QPaintEvent* event)
{
    std::cerr << "paintEvent recieved." << std::endl;
    QPainter Painter(this);
    // Painter.setRenderHint(QPainter::HighQualityAntialiasing, true);
    // Painter.setBackgroundMode(Qt::OpaqueMode);
    Painter.drawPixmap(0, 0, CurrentImg);
    Painter.end();
    return;
}

void PDFDisplay :: setDPI(const int dpi)
{
    assert(dpi > 0);
    DPI = dpi;
    return;
}

void PDFDisplay :: moveDown(const int offset)
{
    std::cerr << "Entering PDFDisplay :: moveDown with offset = " << offset << std::endl;
    VerticalPosInPDF BotLeftDest = PDF.PosInPDFFromHere(DPI, LastPageShown, ViewPortBotInLastPageY,
                                                        OffsetBetweenPages, offset);
    
    int Offset = BotLeftDest.Offset;
    if(Offset == 0)
        return;
    VerticalPosInPDF TopLeftDest = PDF.PosInPDFFromHere(DPI, CurrentPage, ViewPortTopLeftInPage.y(),
                                                        OffsetBetweenPages, Offset);

    if(Offset < height())
    {                    // We can reuse part of the rendering.
        // Specify the region on the widget to reuse.
        QRect RegionToReuse = rect();
        RegionToReuse.adjust(0, Offset, 0, 0);
        std::cerr << RegionToReuse.width() << 'x' << RegionToReuse.height() << '+'
                  << RegionToReuse.x() << '+' << RegionToReuse.y() << std::endl;
        // CurrentImg.grabWindow(winId(), RegionToReuse.x(), RegionToReuse.y(),
        //                       RegionToReuse.width(), RegionToReuse.height());
        // render(&CurrentImg, QPoint(0, 0), QRegion(RegionToReuse), 0);

        QPainter Renderer(&CurrentImg);
        Renderer.drawPixmap(QPoint(0, 0), CurrentImg, RegionToReuse);
        Renderer.end();
    }
    
    // Render new contents
    renderPDF(LastPageShown, QPoint(ViewPortTopLeftInPage.x(),
                                    ViewPortBotInLastPageY),
              QRect(0, height() - Offset, width(), Offset));
    CurrentPage = TopLeftDest.Page;
    ViewPortTopLeftInPage.setY(TopLeftDest.Y);
    LastPageShown = BotLeftDest.Page;
    ViewPortBotInLastPageY = BotLeftDest.Y;
    std::cerr << "Last page becomes " << LastPageShown
              << ", with y-offset " << ViewPortBotInLastPageY << std::endl;
    
    // CurrentImg.save("test.png");
    repaint();

    std::cerr << "Leaving PDFDisplay :: moveDown." << std::endl;
    return;
}

void PDFDisplay :: moveUp(const int offset)
{
    std::cerr << "Entering PDFDisplay :: moveUp with offset = " << offset << std::endl;
    VerticalPosInPDF TopLeftDest = PDF.PosInPDFFromHere(DPI, CurrentPage, ViewPortTopLeftInPage.y(),
                                                        OffsetBetweenPages, -offset);
    int Offset = -TopLeftDest.Offset; // Offset >= 0
    if(Offset == 0)
        return;
    
    VerticalPosInPDF BotLeftDest = PDF.PosInPDFFromHere(DPI, LastPageShown, ViewPortBotInLastPageY,
                                                        OffsetBetweenPages, -Offset);
    
    if(Offset < height())
    {
        // Specify the region on the widget to reuse.
        QRect RegionToReuse = rect();
        RegionToReuse.adjust(0, 0, 0, -Offset);
        std::cerr << RegionToReuse.width() << 'x' << RegionToReuse.height() << '+'
                  << RegionToReuse.x() << '+' << RegionToReuse.y() << std::endl;

        QPainter Renderer(&CurrentImg);
        Renderer.drawPixmap(QPoint(0, Offset), CurrentImg, RegionToReuse);
        Renderer.end();
    }
    
    // Render new contents
    CurrentPage = TopLeftDest.Page;
    ViewPortTopLeftInPage.setY(TopLeftDest.Y);
    
    renderPDF(CurrentPage, QPoint(ViewPortTopLeftInPage.x(), ViewPortTopLeftInPage.y()),
              QRect(0, 0, width(), Offset));

    LastPageShown = BotLeftDest.Page;
    ViewPortBotInLastPageY = BotLeftDest.Y;
    
    std::cerr << "Last page becomes " << LastPageShown
              << ", with y-offset " << ViewPortBotInLastPageY << std::endl;
    
    // CurrentImg.save("test.png");
    repaint();

    std::cerr << "Leaving PDFDisplay :: moveUp." << std::endl;
    return;
}

void PDFDisplay :: goTo(const int page)
{
    assert(0 <= page < PDF.totalPages());
    CurrentPage = page;
    ViewPortTopLeftInPage.setY(0);
    ViewPortTopLeftInPage.setX(0);
    updateLastPageShownInfo();
    repaint();
    return;
}
