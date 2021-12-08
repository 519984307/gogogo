#ifndef LICENSEGROUP_H
#define LICENSEGROUP_H

#include <QObject>
#include <QWidget>
#include <QGroupBox>
#include <QLabel>
#include <QCheckBox>
#include <QButtonGroup>
#include <QLineEdit>

#include "common/checkboxlabel.h"
#include "common/radiobutton.h"
#include "common/imgbutton.h"

class CheckboxLabel;
class RadioButton;
class ImgButton;


class LicenseGroup : public QGroupBox
{
    Q_OBJECT
public:
    LicenseGroup(QWidget *parent = Q_NULLPTR);

    void SetFocus(bool bFocus);

public slots:
    void SetSizeEditValid();

public:
    bool m_bFocus;

    QLineEdit *m_pleLicenseNumber;
    ImgButton *m_pibActiveLicense;

    QLabel  *m_plInfoIco;
    QLabel  *m_plInfoText;


};


#endif // LICENSEGROUP_H
