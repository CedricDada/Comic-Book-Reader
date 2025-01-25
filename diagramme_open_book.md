```mermaid
sequenceDiagram
    participant UI as MainWindow
    participant BM as BookManager
    participant AH as ArchiveHandler
    participant PL as PageLoader
    participant CM as CacheManager
    
    UI->>BM: openBook(paths)
    BM->>AH: getInitialPages(5)
    AH->>AH: Extract PDF/CBZ pages
    AH-->>BM: 5 premières pages
    BM->>PL: preloadPages(start=5)
    PL->>CM: storePage(page+processed)
    CM-->>UI: pageProcessed signal
    UI->>UI: displayFirstPage()
    