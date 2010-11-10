class PDFViewerConfig
{
private:
    int InitialDPI;
    int OffsetBetweenPages;

public:
    PDFViewerConfig();                   // Sets the default value of all options.
    ~PDFViewerConfig() {}

    // Error handling is done in all setters.
    int initialDPI() { return InitialDPI; }
    void setInitialDPI(const int dpi);
    int offsetBetweenPages() { return OffsetBetweenPages; }
    void setOffsetBetweenPages(const int offset);
};
