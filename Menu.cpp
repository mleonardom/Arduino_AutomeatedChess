#include "Menu.h"

Menu::Menu() {
}

Menu::Menu(uint8_t ID, const String title, uint8_t itemsCount, const String items[]) {
    setPorperties(ID, title, itemsCount, items);
}

void Menu::setPorperties(uint8_t ID, const String title, uint8_t itemsCount, const String items[]) {
    _ID = ID;
    _title = title;
    _itemsCount = itemsCount;
    _items = items;
    _selectedItem = 0;
}

String Menu::getTitle() {
    return _title;
}

const String * Menu::getItems() {
    return _items;
}

String Menu::getItem(uint8_t index) {
    return _items[index];
}

String Menu::getLabelItem(uint8_t index) {
    if( index == getSelectedIndex() ) {
        return ">" + _items[index];
    }

    return _items[index];
}

uint8_t Menu::getID() {
    return _ID;
}

uint8_t Menu::getItemsCount() {
    return _itemsCount;
}

void Menu::selectPrevItem() {
    if( _selectedItem == 0 ) {
        _selectedItem = getItemsCount()-1;
    } else {
        _selectedItem --;
    }
}

void Menu::selectNextItem(){
    _selectedItem ++;
    if( _selectedItem>=getItemsCount() ) _selectedItem=0;
}

uint8_t Menu::getSelectedIndex(){
    return _selectedItem;
}