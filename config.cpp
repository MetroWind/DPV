#include <iostream>
#include "config.hpp"

PDFViewerConfig :: PDFViewerConfig()
{
    // ============================================================
    // ------------------- Specify options below ------------------
    // ============================================================
    InitialDPI = 113;           // Initial DPI, dah~
    OffsetBetweenPages = 4;     // The space between two adjacent pages
    MoveStep = 32;              // The distance

    // ============================================================
    // -------------------- Key Bindings --------------------------
    // ============================================================
    KeyBindings["j"] = "moveDown";
    KeyBindings["k"] = "moveUp";
}

// ============================================================
// ---------------- Option defintions stops here --------------
// ============================================================
// ============================================================

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
