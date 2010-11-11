#ifndef CONFIG_H
#define CONFIG_H

#include <map>
#include <QKeySequence>

typedef QKeySequence Key;
typedef std::map<QString, QString> KeyMap;

class PDFViewerConfig
{
private:
    int InitialDPI;
    int OffsetBetweenPages;
    int MoveStep;
    KeyMap KeyBindings;

public:
    PDFViewerConfig();                   // Sets the default value of all options.
    ~PDFViewerConfig() {}

    // Error handling is done in all setters.
    int initialDPI() { return InitialDPI; }
    void setInitialDPI(const int dpi);
    int offsetBetweenPages() { return OffsetBetweenPages; }
    void setOffsetBetweenPages(const int offset);
    int moveStep() { return MoveStep; }
    
    KeyMap& keyBindings() { return KeyBindings; }
};

#endif
