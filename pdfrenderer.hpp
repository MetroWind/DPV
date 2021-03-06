#ifndef PDFRENDERER_H
#define PDFRENDERER_H

#include <vector>
#include <QString>
#include <QImage>
#include <poppler-qt4.h>

typedef struct _vert_pos_in_pdf
{
    int Page;
    int Y;
    int Offset;
} VerticalPosInPDF;

typedef std::vector<Poppler::Page*> PageList;

class PDFRenderer
{
private:
    Poppler::Document* PDFDoc;
    PageList Pages;
    void destroy();
    QSize pageSize(Poppler::Page* page, const int dpi); // Returns size in pixel.
public:
    PDFRenderer();
    ~PDFRenderer();

    bool loadFromFile(const QString& filename);
    // Render the region starting from (x, y) in page `page' with
    // width `width', height `height' and DPI `dpi'.
    // `page' starts from 0.
    QImage render(const int page, const int dpi,
                  const int x, const int y,
                  const int width, const int height);

    int totalPages();
    QSize pageSize(const int pagenum, const int dpi);
    VerticalPosInPDF posInPDFFromHere(const int dpi, const int page, const int y,
                                      const int offset_between_pages, const int offset);

    void setHinting(const bool hinting);
};

#endif
