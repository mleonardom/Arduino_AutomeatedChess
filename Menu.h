#ifndef Menu_h
#define Menu_h

#include "Arduino.h"

class Menu {
    private:
        uint8_t _ID = 0;
        uint8_t _itemsCount;
        String _title;
        const String * _items;
        uint8_t _selectedItem;
    public:
        Menu();
        Menu(uint8_t ID, const String title, uint8_t itemsCount, const String items[]);
        void setPorperties(uint8_t ID, const String title, uint8_t itemsCount, const String items[]);
        String getTitle();
        String getItem(uint8_t index);
        String getLabelItem(uint8_t index);
        const String * getItems();
        uint8_t getID();
        uint8_t getItemsCount();
        void selectPrevItem();
        void selectNextItem();
        uint8_t getSelectedIndex();
};

#endif