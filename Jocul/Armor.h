#pragma once
#include <iostream>
#include "Enchantment.h"

class Armor
{
private:
    std::string name;
    int def;
    std::string type;
    int strReq;
    int weight;
    Enchantment enchantment;

public:
    Armor(std::string name, int deff, std::string t, int s, int w);

    void setDef(int deff);
    void setStrReq(int s);
    void setType(std::string t);
    void setWeight(int w);
    void setName(std::string name);
    void setEnchantment(const Enchantment& ench);

    int getDef() const;
    std::string getType() const;
    int getStrReq() const;
    int getWeight() const;
    std::string getName() const;
    const Enchantment& getEnchantment() const;

    bool operator==(const Armor& other) const;
    Armor& operator=(const Armor& other);
};
