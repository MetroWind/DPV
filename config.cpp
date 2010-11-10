#include <iostream>
#include "config.hpp"

PDFViewerConfig :: PDFViewerConfig()
{
    InitialDPI = 113;
    OffsetBetweenPages = 4;
}

void PDFViewerConfig :: setInitialDPI(const int dpi)
{
    if(dpi <= 0)
    {
        std::cerr << "Setting InitialDPI <= 0?  Check your RC files~" << std::endl;
    }
    else
    {
        InitialDPI = dpi;
    }
    return;
}

void PDFViewerConfig :: setOffsetBetweenPages(const int offset)
{
    OffsetBetweenPages = offset;
    return;
}
