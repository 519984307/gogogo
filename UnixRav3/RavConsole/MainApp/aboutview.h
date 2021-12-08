#ifndef ABOUTVIEW_H
#define ABOUTVIEW_H

#include "view.h"

#include <string>

class TextView;
class AboutView : public View
{
public:
    AboutView(const std::string& version,
              const std::string& updatetime,
              const std::string& virusdb,
              const std::string& virupdate,
              int x, int y, int width, int height);

    void Layout(const std::string& version,
                const std::string& updatetime,
                const std::string& virusdb,
                const std::string& virupdate);

private:
    TextView    *mTitleView;
    TextView    *mVersionView;
    TextView    *mUpdateTimeView;
    TextView    *mVirusDbView;
    TextView    *mVirusTimeView;
    TextView    *mLicenseView;
};

#endif // ABOUTVIEW_H
