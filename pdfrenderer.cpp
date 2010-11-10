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
    if(PageSize.width() < Width)
    {
        Width = PageSize.width();
    }
    if(PageSize.height() < Height)
    {
        Height = PageSize.height();
    }

    QImage Result;
    Result = CurrentPage -> renderToImage(dpi, dpi, x, y, Width, Height);

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
    Size.rwidth() *= double(dpi) / double(72);
    Size.rheight() *= double(dpi) / double(72);
    return Size;
}
