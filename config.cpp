#include <iostream>
#include "config.hpp"

PDFViewerConfig :: PDFViewerConfig()
{
    // ============================================================
    // ------------------- Specify options below ------------------
    // ============================================================
    InitialDPI = 113;           // Initial DPI, dah~
    OffsetBetweenPages = 4;     // The space between two adjacent pages
    MoveStep = 32;              // The distance of normal navigational move
    PageScrollKeep = 32;        // Number of pixels in what you are
                                // seeing now to remain visible after
                                // a page scroll.

    // ============================================================
    // -------------------- Key Bindings --------------------------
    // ============================================================
    KeyBindings["j"] = "moveDown";
    KeyBindings["k"] = "moveUp";
    KeyBindings["r"] = "reload";
    KeyBindings["g"] = "goToPage";
    KeyBindings["Ctrl+v"] = "pageDown";
    KeyBindings["Alt+v"] = "pageUp";
    KeyBindings["Shift+j"] = "nextPage";
    KeyBindings["Shift+k"] = "prevPage";
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
