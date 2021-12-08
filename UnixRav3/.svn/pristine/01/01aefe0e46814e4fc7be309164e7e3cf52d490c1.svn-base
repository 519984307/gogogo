#include "aboutview.h"
#include "textview.h"
#include "resources.h"

#include <string.h>

AboutView::AboutView(const std::string& version,
                     const std::string& updatetime,
                     const std::string& virusdb,
                     const std::string& virupdate,
                     int x, int y, int width, int height)
    : View(x,y,width,height) {
    Layout(version, updatetime, virusdb, virupdate);
}

void AboutView::Layout(const std::string& version,
                       const std::string& updatetime,
                       const std::string& virusdb,
                       const std::string& virupdate) {
    mVersionView = new TextView(3, 3, 30, 1);
    char temp[128] = {0};
    sprintf(temp, "%s : %s", STRING_VERSION, version.c_str());
    mVersionView->SetText(temp);
    AddView(mVersionView);

    mUpdateTimeView = new TextView(3, 4, 40, 1);
    memset(temp, 0, 128);
    sprintf(temp, "%s : %s", STRING_UPDATE_TIME, updatetime.c_str());
    mUpdateTimeView->SetText(temp);
    AddView(mUpdateTimeView);

    mVirusDbView = new TextView(3, 5, 40, 1);
    memset(temp, 0, 128);
    sprintf(temp, "%s : %s", STRING_VIRUS, virusdb.c_str());
    mVirusDbView->SetText(temp);
    AddView(mVirusDbView);

    mVirusTimeView = new TextView(3, 6, 40, 1);
    memset(temp, 0, 128);
    sprintf(temp, "%s : %s", STRING_UPDATE_VIRUS_TIME, virupdate.c_str());
    mVirusTimeView->SetText(temp);
    AddView(mVirusTimeView);

    mLicenseView = new TextView(3, 8, 55, 1);
    mLicenseView->SetText(STRING_LICENSE);
    AddView(mLicenseView);
}
