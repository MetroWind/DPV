#include <iostream>
#include "window.hpp"

PDFWindow :: PDFWindow(const QString& filename, PDFViewerConfig& config, QWidget* parent)
: Config(config), PDF(filename, config.initialDPI(), config.offsetBetweenPages(), parent),
    QWidget(parent)
{
    Layout.setContentsMargins(0, 0, 0, 0);
    Layout.addWidget(&PDF);
    setLayout(&Layout);

    bindKeys();
    setFocus(Qt::OtherFocusReason);
}

PDFWindow :: ~PDFWindow()
{}

void PDFWindow :: resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    return;
}

void PDFWindow :: keyPressEvent(QKeyEvent* event)
{
    QKeySequence Seq(event -> key() + event -> modifiers());
    if(KeyBindings.find(Seq) == KeyBindings.end())
    {
        defaultAction(*this);
        return;
    }
    
    KeyBindings[Seq](*this);
    return;
}

void PDFWindow :: bindKeys()
{
    NameToFunc["moveDown"] = moveDown;
    NameToFunc["moveUp"] = moveUp;
    NameToFunc["pageDown"] = pageDown;
    NameToFunc["pageUp"] = pageUp;
    NameToFunc["nextPage"] = nextPage;
    NameToFunc["prevPage"] = prevPage;

    KeyMap::iterator Key;
    KeyMap& Keys = Config.keyBindings();
    for(Key = Keys.begin(); Key != Keys.end(); Key++)
    {
        std::cerr << "Bind " << QKeySequence((*Key).first).toString().toStdString() << std::endl;
        KeyBindings[QKeySequence((*Key).first)] = NameToFunc[(*Key).second]; // funcFromString((*Key).second);
    }

    return;
}

void defaultAction(PDFWindow& pdf_win)
{
    std::cerr << "Nonsense key capture." << std::endl;
    return;
}

void moveDown(PDFWindow& pdf_win)
{
    pdf_win.PDF.moveDown(pdf_win.Config.moveStep());
    return;
}
void moveUp(PDFWindow& pdf_win)
{
    pdf_win.PDF.moveUp(pdf_win.Config.moveStep());
    return;
}

void pageDown(PDFWindow& pdf_win)
{
    pdf_win.PDF.moveDown(pdf_win.PDF.height() - pdf_win.Config.pageScrollKeep());
    return;
}
void pageUp(PDFWindow& pdf_win)
{
    pdf_win.PDF.moveUp(pdf_win.PDF.height() - pdf_win.Config.pageScrollKeep());
    return;
}

void nextPage(PDFWindow& pdf_win)
{
    pdf_win.PDF.goTo(pdf_win.currentPage() + 1);
    return;
}

void prevPage(PDFWindow& pdf_win)
{
    pdf_win.PDF.goTo(pdf_win.currentPage() - 1);
    return;
}
