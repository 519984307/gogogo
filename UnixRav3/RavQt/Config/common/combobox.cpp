#include <QListView>

#include "combobox.h"


ComboBox::ComboBox(QWidget *parent):QComboBox(parent)
{

    setStyleSheet("QComboBox {combobox-popup: 0;}\
                    QComboBox::drop-down {\
                        subcontrol-origin: padding;\
                        subcontrol-position: top right;\
                        width: 20px;\
                        border-left-width: 0px;\
                        border-left-color: darkgray;\
                        border-left-style: solid; \
                        border-top-right-radius: 3px; \
                        border-bottom-right-radius: 3px;\
                    }\
                    QComboBox QAbstractItemView::item{height:20px;}\
                    QComboBox::down-arrow {\
                        image: url(:/Images/combo_drop_arrow);\
                    }\
                    QComboBox::down-arrow:on { \
                        image: url(:/Images/combo_drop_on);\
                    }");
}
