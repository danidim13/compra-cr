//
// Created by daniel on 10/09/19.
//

#include "view/DomItem.h"
#include "view/NavItem.h"
#include "view/PageBuilder.h"
#include <iostream>

void test_dom() {
    DomItem item1("a",
            {
                {"class", "nav-item nav-link"},
                {"href", "#"}
            },
            DomItem::SIMPLE, nullptr);

    DomItem item2("input",
                  {
                          {"class", "form-input"},
                          {"type", "password"}
                  },
                  DomItem::OPEN, nullptr);

    NavItem item3("local",
            "Inicio",
            {
                {"class", "nav-item nav-link"},
            });


    std::cout << item1 << std::endl << item2 << std::endl << item3 << std::endl;

}

void test_builder() {
    PageBuilder pageBuilder;
    std::cout << pageBuilder.build_document() << std::endl;
}

int main(int argc, char *argv[]) {

    // test_dom();
    test_builder();

}
