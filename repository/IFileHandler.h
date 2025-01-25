// IFileHandler.h
#include <QVector>
#include "../model/Page.h"

class IFileHandler {
public:
    virtual ~IFileHandler() = default;
    virtual QVector<Page> getInitialPages(int count) = 0;
    virtual int totalPages() const = 0;
    virtual bool isImageFile(const QString& filename) const = 0;
};