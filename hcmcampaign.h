/*
 * Ho Chi Minh City University of Technology
 * Faculty of Computer Science and Engineering
 * Initial code for Assignment 2
 * Programming Fundamentals Spring 2025
 * Date: 02.02.2025
 */

// The library here is concretely set, students are not allowed to include any other libraries.
#ifndef _H_HCM_CAMPAIGN_H_
#define _H_HCM_CAMPAIGN_H_

#include "main.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

// Forward declarations

class Unit;
class UnitList; 
class Army;
class TerrainElement;

class Vehicle;
class Infantry;

class LiberationArmy;
class ARVN;

class Position;

class Road;
class Mountain;
class River;
class Urban;
class Fortification;
class SpecialZone;

class BattleField;

class HCMCampaign;
class Configuration
{
private:
    int num_rows, num_cols, eventCode;
    vector<Position *> arrayForest, arrayRiver, arrayFortification, arrayUrban, arraySpecialZone;
    vector<Unit *> liberationUnits, ARVNUnits;
    string filepath;

public:
    Configuration(const string & filepath);
    ~Configuration();

    void getArraysConfig(const string & posString, vector<Position *> &arrayTerrain);
    void getUnitListConfig(const string &unit);
    vector<Position *> getArrayForest();
    vector<Position *> getArrayRiver();
    vector<Position *> getArrayFortification();
    vector<Position *> getArrayUrban();
    vector<Position *> getArraySpecialZone();
    vector<Unit *> getLiberationArmyList();
    vector<Unit *> getARVNList();
    int getRow();
    int getCol();
    int getEventCode();
    string str() const;

};

enum VehicleType
{
    TRUCK,
    MORTAR,
    ANTIAIRCRAFT,
    ARMOREDCAR,
    APC,
    ARTILLERY,
    TANK
};
enum InfantryType
{
    SNIPER,
    ANTIAIRCRAFTSQUAD,
    MORTARSQUAD,
    ENGINEER,
    SPECIALFORCES,
    REGULARINFANTRY
};

class Position
{
private:
    int r, c;

public:
    Position(int r = 0, int c = 0);
    Position(const string &str_pos); // Example: str_pos = "(1,15)"
    int getRow() const;
    int getCol() const;
    void setRow(int r);
    void setCol(int c);
    string str() const; // Example: returns "(1,15)"
};

class Unit
{
protected:
    int quantity, weight;
    Position pos;

public:
    Unit(int quantity, int weight, Position pos);
    virtual ~Unit();
    friend class UnitList;
    virtual int getAttackScore() = 0;
    Position getCurrentPosition() const;
    virtual string str() const = 0;
    int getQuantity() {
        return this->quantity;
    }
    int getWeight() {
        return this->weight;
    }
    virtual int getType() const = 0;
    void updateQuantity(int newQuantity) {
        this->quantity = newQuantity;
    }
    void updateWeight(int newWeight) {
        this->weight = newWeight;
    }
};

class Vehicle : public Unit
{
private:
    VehicleType vehicleType;
public:
    friend class UnitList;
    Vehicle(int quantity, int weight, Position pos, VehicleType vehicleType);
    int getAttackScore() override;
    string str() const override;
    int getType() const override;
};

class Infantry : public Unit
{
private:
    InfantryType infantryType;
public:
    friend class UnitList;
    Infantry(int quantity, int weight, const Position pos, InfantryType infantryType);
    int getAttackScore() override;
    string str() const override;
    int getType() const override;
};

struct unitNode {
    Unit *val;
    unitNode *next;

    unitNode(Unit *val, unitNode *next) : val(val), next(next) {}
    unitNode(Unit *val) : val(val), next(nullptr) {}
    unitNode(unitNode *next) : val(nullptr), next(next) {}
};

class UnitList
{
protected:
    // head points to the FIRST Infantry unit OR the FIRST Vehicle unit
    unitNode *head;
    int capacity;
    int size;
    
    // TODO
public:
    UnitList(unitNode *head, int capacity);
    UnitList(unitNode *unit);
    UnitList(Unit **unitArray, int size);
    friend class TerrainElement;
    bool insert(Unit *unit);                   // return true if insert successfully
    bool isContain(VehicleType vehicleType);   // return true if it exists
    bool isContain(InfantryType infantryType); // return true if it exists
    string str() const;
    void getCapacity(int LF, int EXP); // get capacity of the army
    void remove(Unit *unit);
    int getSize();
    unitNode *getHead();
    // TODO
};

class Army
{
protected:
    // LF : sum of the score in vehicles
    // EXP : sum of the score in infantry
    // size : unutList.size()
    int LF, EXP, size;  
    string name;
    UnitList *unitList;
    Unit **unitArray;
    BattleField *battleField;

public:
    Army(Unit **unitArray, int size, string name, BattleField *battleField);
    virtual void fight(Army *enemy, bool defense = false) = 0;
    virtual string str() const = 0;
    void updateUnitCapacity(int LF, int EXP);
    Unit **getUnitArray();
    UnitList *getUnitList() {return this->unitList;}
    int getEXP();
    int getLF();
    int getSize();
    void updateEXP(int newEXP);
    void updateLF(int newLF);
    void calculateEXP();
    void calculateLF();
};

class LiberationArmy : public Army
{
public:
    LiberationArmy(Unit **unitArray, int size, string name, BattleField *battleField);
    void fight(Army *enemy, bool defense) override;
    string str() const override;
    int nearestFibo(int n);
    void fightDefense(Army *enemy);
};

class ARVN : public Army
{
public:
    ARVN(Unit **unitArray, int size, string name, BattleField *battleField);
    void fight(Army *enemy, bool defense) override;
    string str() const override;
};

class BattleField
{
private:
    int n_rows, n_cols;
    vector<vector<TerrainElement *>> terrain;
    // TODO
public:
    BattleField(int n_rows, int n_cols, vector<Position *> arrayForest,
                vector<Position *> arrayRiver, vector<Position *> arrayFortification,
                vector<Position *> arrayUrban, vector<Position *> arraySpecialZone);
    ~BattleField();
    string str();
    vector<vector<TerrainElement *>> getTerrain();
};

class TerrainElement : public BattleField
{
public:
    TerrainElement();
    ~TerrainElement();
    virtual void getEffect(Army *army) = 0;
    vector<vector<TerrainElement *>> getTerrain();
};

class HCMCampaign
{
private:
    Configuration *config;
    BattleField *battleField;
    LiberationArmy *liberationArmy;
    ARVN *ArmyRVN;

public:
    HCMCampaign(const string &config_file_path);
    void run();
    string printResult();
};

#endif