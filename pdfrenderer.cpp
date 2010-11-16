#include <iostream>
#include <QPainter>
#include <assert.h>
#include "pdfrenderer.hpp"

PDFRenderer :: PDFRenderer()
    :PDFDoc(NULL)
{}

PDFRenderer :: ~PDFRenderer()
{
    if(PDFDoc)
    {
        delete PDFDoc;
    }
}

bool PDFRenderer :: loadFromFile(const QString& filename)
{
    std::cerr << "Entering PDFRenderer :: loadFromFile" << std::endl;
    
    if(PDFDoc)
        delete PDFDoc;
    
    PDFDoc = Poppler::Document::load(filename);
    if(!PDFDoc || PDFDoc -> isLocked())
    {
        std::cerr << "Cannot loat file " << filename.toStdString() << std::endl;
        std::cerr << "PDFRenderer :: loadFromFile returns false." << std::endl;
        return false;
    }
    else
    {
        PDFDoc -> setRenderHint(Poppler::Document::TextAntialiasing);
        PDFDoc -> setRenderHint(Poppler::Document::TextHinting, false);
        
        std::cerr << "PDFRenderer :: loadFromFile returns true." << std::endl;
        return true;
    }
}

QImage PDFRenderer :: render(const int page, const int dpi,
                             const int x, const int y,
                             const int width, const int height)
{
    std::cerr << "Entering PDFRenderer :: render" << std::endl;
    std::cerr << "page = " << page << ", dpi = " << dpi << std::endl
              << "Region to render: "
              << width << 'x' << height << '+'
              << x << '+' << y << std::endl;

    assert(PDFDoc != 0);
    
    Poppler::Page* CurrentPage = PDFDoc -> page(page);
    if(!CurrentPage)
    {
        std::cerr << "Cannot open page " << page << std::endl;
    }
    QSize PageSize = pageSize(CurrentPage, dpi);
    int Width = width;
    int Height = height;
    QImage Result;

    if(PageSize.width() < Width)
    {
        Width = PageSize.width();
    }
    if(PageSize.height() < Height)
    {
        Height = PageSize.height();
    }
    
    Result = CurrentPage -> renderToImage(dpi, dpi, x, y, Width, Height);

    delete CurrentPage;
    std::cerr << "Leaving PDFRenderer :: render" << std::endl;
    return Result;
}

int PDFRenderer :: totalPages()
{
    assert(PDFDoc != 0);
    return PDFDoc -> numPages();
}

QSize PDFRenderer :: pageSize(Poppler::Page* page, const int dpi)
{
    assert(page != 0 && dpi > 0);
    QSize Size = page -> pageSize();
    Size.rwidth() *= double(dpi) / double(72);
    Size.rheight() *= double(dpi) / double(72);

    return Size;
}

QSize PDFRenderer :: pageSize(const int pagenum, const int dpi)
{
    assert(pagenum >= 0 && pagenum < totalPages() && dpi > 0);
    Poppler::Page* Page = PDFDoc -> page(pagenum);

    QSize Size = Page -> pageSize();
    delete Page;
    
    Size.rwidth() *= double(dpi) / double(72);
    Size.rheight() *= double(dpi) / double(72);
    return Size;
}

VerticalPosInPDF PDFRenderer :: posInPDFFromHere(
    const int dpi, const int page, const int y, const int offset_between_pages,
    const int offset)
{
    assert(offset != 0);
    int OffsetRemain = offset;
    int CurrentPage = page;
    // bool isFirstPage = true;
    int Y = y; // Where are we on the pdf in the current page?
    int RealOffset = 0;
    VerticalPosInPDF Result;
    if(offset > 0)
    {
        while(OffsetRemain > 0)
        {
            QSize PageSize = pageSize(CurrentPage, dpi);
            if(CurrentPage == totalPages() - 1 && OffsetRemain >= PageSize.height() - Y)
            {                       // We are at the last page, and OffsetRemain is way too large.
                RealOffset += PageSize.height() - Y;
                Result.Page = CurrentPage;
                Result.Y = PageSize.height();
                Result.Offset = RealOffset;
                return Result;
            }
            if(PageSize.height() - Y + offset_between_pages > OffsetRemain)
            {                       // We don't have to go across page.
                RealOffset += OffsetRemain;
            
                Result.Page = CurrentPage;
                Result.Y = Y + OffsetRemain;
                Result.Offset = RealOffset;
                return Result;
            }
            // We have to go across page.
            RealOffset += PageSize.height() - Y + offset_between_pages;
            OffsetRemain -= PageSize.height() - Y + offset_between_pages;
            Y = 0;
            CurrentPage++;
        }
    }
    else                        // Go up~
    {
        while(OffsetRemain < 0)
        {
            if(CurrentPage == 0 && -OffsetRemain >= Y)
            {                       // We are at the first page, and OffsetRemain is way too large.
                RealOffset -= Y;
                Result.Page = CurrentPage;
                Result.Y = 0;
                Result.Offset = RealOffset;
                return Result;
            }
            if(Y + offset_between_pages > -OffsetRemain)
            {                       // We don't have to go across page.
                RealOffset += OffsetRemain;
            
                Result.Page = CurrentPage;
                Result.Y = Y + OffsetRemain;
                Result.Offset = RealOffset;
                return Result;
            }
            // We have to go across page.
            RealOffset -= Y + offset_between_pages;
            OffsetRemain += Y + offset_between_pages;
            CurrentPage--;
            QSize PageSize = pageSize(CurrentPage, dpi);            
            Y = PageSize.height();
        }
    }
    
    Result.Page = CurrentPage;
    Result.Y = Y;
    Result.Offset = RealOffset;
    return Result;
}
