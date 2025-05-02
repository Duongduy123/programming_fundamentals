#include "hcmcampaign.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
////////////////////////////////////////////////////////////////////////
using namespace std;

// Task 3.1: Unit class
Unit::Unit(int quantity, int weight, Position pos) : quantity(quantity), weight(weight), pos(pos) {}

// Unit's methods
Position Unit::getCurrentPosition() const {
    return Position(0,0);
}

string Unit::str() const {
    return "";
}

Unit::~Unit() {
    // destructor
}

// Task 3.2: Vehicle class
Vehicle::Vehicle(int quantity, int weight, const Position pos, VehicleType vehicleType) :
    Unit(quantity, weight, pos),
    vehicleType(vehicleType) {}
    
int Vehicle::getAttackScore() {
    int score = ceil((float) (this->vehicleType * 304 + this->weight * this->quantity) / 30);
    return score;
}

string Vehicle::str() const {
    string vehicleName;
    switch (vehicleType) {
        case TANK: vehicleName = "TANK"; break;
        case APC: vehicleName = "APC"; break;
        case TRUCK: vehicleName = "TRUCK"; break;
        case ARMOREDCAR: vehicleName = "ARMOREDCAR"; break;
        case ANTIAIRCRAFT: vehicleName = "ANTIAIRCRAFT"; break;
        case MORTAR: vehicleName = "MORTAR"; break;
        case ARTILLERY: vehicleName = "ARTILLERY"; break;
        default: vehicleName = "UNKNOWN_VEHICLE"; break;
    }

    return "Vehicle[vehicleType=" + vehicleName + 
        ",quantity=" + to_string(this->quantity) + 
        ",weight=" + to_string(this->weight) +
        ",position=" + this->pos.str() + "]";
}

int Vehicle::getType() const {
    return this->vehicleType;
}

// Task 3.3: Infantry class
int personalNumber(int k, int Y = 1975, bool check = true) {
    int sum = 0;
    if (check) {
        while (Y > 0) {
            sum += Y % 10;
            Y /= 10;
        }
    }
    while (k > 0) {
        sum += k % 10;
        k /= 10;
    }
    return (sum >= 10) ? personalNumber(sum, 1975, false) : sum;
}

Infantry::Infantry(int quantity, int weight, const Position pos, InfantryType infantryType) :
    Unit(quantity, weight, pos),
    infantryType(infantryType) {}

int Infantry::getAttackScore() {
    int score = this->infantryType * 56 + this->weight * this->quantity;
    int sqrtWeight = sqrt(this->weight);
    // check for commandos
    if (this->infantryType == 4 && sqrtWeight * sqrtWeight == this->weight) 
        score += 75;

    int pNum = personalNumber(score);
    if (pNum > 7) {
        this->quantity = ceil((float) this->quantity * 1.2); 
        return this->infantryType * 56 + this->weight * this->quantity;
    }
    else if (pNum < 3) {
        this->quantity = ceil((float) this->quantity * 0.9);
        return this->infantryType * 56 + this->weight * this->quantity;
    }
        
    return score;
}

string Infantry::str() const{
    string infantryName;
    switch (infantryType) {
        case SNIPER: infantryName = "SNIPER"; break;
        case ANTIAIRCRAFTSQUAD: infantryName = "ANTIAIRCRAFTSQUAD"; break;
        case MORTARSQUAD: infantryName = "MORTARSQUAD"; break;
        case ENGINEER: infantryName = "ENGINEER"; break;
        case SPECIALFORCES: infantryName = "SPECIALFORCES"; break;
        case REGULARINFANTRY: infantryName = "REGULARINFANTRY"; break;
        default: infantryName = "UNKNOWN_INFANTRY"; break;
    }
    return "Infantry[infantryType=" + infantryName + 
        ",quantity=" + to_string(this->quantity) + 
        ",weight=" + to_string(this->weight) +
        ",position=" + this->pos.str() + "]";
}

int Infantry::getType() const {
    return infantryType;
}

// Task 3.4: Army class
Army::Army(Unit **unitArray, int size, string name, BattleField *battleField) :
    unitList(new UnitList(unitArray, size)), size(size), name(name), battleField(battleField) {
        this->unitArray = unitArray;
        this->calculateEXP();
        this->calculateLF();
    }

void Army::updateUnitCapacity(int LF, int EXP) {
    // unitList->getCapacity(LF, EXP);
}

Unit **Army::getUnitArray() {
    return this->unitArray;
}

int Army::getEXP() {
    return this->EXP;
}

int Army::getSize() {
    return this->size;
}

int Army::getLF() {
    return this->LF;
}

void Army::updateEXP(int newEXP) {
    this->EXP = max(min(newEXP, 500), 0);
}

void Army::updateLF(int newLF) {
    this->LF = max(min(newLF, 1000), 0);
}

void Army::calculateEXP() {
    int sumEXP = 0;
    UnitList *unitList = this->getUnitList();
    cout << "unitList: " << unitList->str() << endl;
    unitNode *current = unitList->getHead();
    while (current && dynamic_cast<Infantry *>(current->val)) {
        sumEXP += current->val->getAttackScore();
        current = current->next;
    }
    this->EXP = max(min(sumEXP, 500), 0);
}

void Army::calculateLF() {
    int sumLF = 0;
    UnitList *unitList = this->getUnitList();
    cout << "unitList: " << unitList->str() << endl;
    unitNode *current = unitList->getHead();
    while (current) {
        if (dynamic_cast<Vehicle *>(current->val)) {
            sumLF += current->val->getAttackScore();
        }
        current = current->next;
    }
    this->LF = max(min(sumLF, 1000), 0);
}

struct Combination {
    int sum;
    vector<Unit *> unitList;

    Combination() : sum(0), unitList({}) {}
    Combination(int sum, vector<Unit *> unitList) : sum(sum), unitList(unitList) {}
};

// Task 3.4.1: LiberationArmy class
LiberationArmy::LiberationArmy(Unit **unitArray, int size, string name, BattleField *battleField) :
    Army(unitArray, size, name, battleField) {}

void LiberationArmy::fight(Army *enemy, bool defense = false) {
    cout << "\nLiberation army is fighting!" << endl;
    // get LF and EXP of liberationArmy
    vector<Combination> sumEXP = {}, sumLF = {};
    int tmpLF = 0, tmpEXP = 0;
    UnitList *lUnitList = this->getUnitList();
    unitNode *current = lUnitList->getHead();
    while (current) {
        cout << "current: " << current->val->str() << endl;
        int score = current->val->getAttackScore();
        if (dynamic_cast<Infantry *>(current->val)) {
            tmpEXP += score; 
            int sumEXP_size = sumEXP.size();
            for (int i = 0; i < sumEXP_size; i++) {
                vector<Unit *> tmpVector = sumEXP[i].unitList;
                tmpVector.push_back(current->val);
                sumEXP.push_back(Combination(sumEXP[i].sum + score, tmpVector));
            }
            sumEXP.push_back(Combination(score, {current->val}));
            cout << "add infantry: " << current->val->str() << " " << score << endl;
        }
        else {
            tmpLF += score;
            int sumLF_size = sumLF.size();
            for (int i = 0; i < sumLF_size; i++) {
                vector<Unit *> tmpVector = sumLF[i].unitList;
                for (int j = 0; j < tmpVector.size(); j++)
                    cout << tmpVector[j]->str() << endl;
                tmpVector.push_back(current->val);
                sumLF.push_back(Combination(sumLF[i].sum + score, tmpVector));
                cout << "add vehicle successfully: " << current->val->str() << " " << score << endl; 
            }
            sumLF.push_back(Combination(score, {current->val}));
        }
        current = current->next;
    }

    // get LF and EXP of enemy's army (ARVN's army)
    int enemyLF = enemy->getLF(), enemyEXP = enemy->getEXP();
    cout << "enemy indices: " << enemyLF << " " << enemyEXP << endl;
    UnitList *enemyUnitList = enemy->getUnitList();

    cout << endl;
    cout << "get liber and enemy successfully!\n";
    cout << "liberation army: " << tmpEXP << " " << tmpLF << endl;
    cout << "liberation army: " << lUnitList->str() << endl;
    cout << "enemy army: " << enemyEXP << " " << enemyLF << endl;
    cout << "enemy army: " << enemyUnitList->str() << endl;

    // fight
    if (!defense) {
        this->updateEXP(ceil((float) tmpEXP * 1.5));
        this->updateLF(ceil((float) tmpLF * 1.5));
        cout << "fight: " << this->getEXP() << " " << this->getLF() << endl;

        if (this->getEXP() <= enemyEXP || this->getLF() <= enemyLF) {
            // fight does not occur
            cout << "fight does not occur!\n";
            unitNode *current = lUnitList->getHead();
            while (current) {
                current->val->updateWeight(ceil((float) current->val->getWeight() * 0.9));
                current = current->next;
                // update score and EXP, LF of the liberation army
            }
            delete current;
            this->calculateEXP();
            this->calculateLF();
            return;
        }

        if (tmpEXP > enemyEXP) {
            // choose combination A
            cout << "\nLooking for combination A:\n";
            int minD = 1000, pos = sumEXP.size() - 1;
            for (int i = (int) sumEXP.size() - 1; i >= 0; i--) {
                if (sumEXP[i].sum > enemyEXP) {
                    if (sumEXP[i].sum - enemyEXP < minD) {
                        pos = i;
                        minD = sumEXP[i].sum - enemyEXP;
                    }
                }
            }
            // delete units in combination A
            for (int i = 0; i < sumEXP[pos].unitList.size(); i++) 
                lUnitList->remove(sumEXP[pos].unitList[i]);
            // delete unit in enemy's unit and insert them to lUnitList,
            // adjust lUnitList's units if necessary
            unitNode *headInfantry = enemyUnitList->getHead();
            while (headInfantry && dynamic_cast<Infantry *>(headInfantry->val)) {
                unitNode *removeNode = headInfantry;
                headInfantry = headInfantry->next;
                lUnitList->insert(removeNode->val);
                enemyUnitList->remove(removeNode->val);
            }
        }
        else {
            cout << "\nLooking for combintion A failed, use army's all force!\n";
            // use all force (this->getEXP()) to check, remove all units of lArmy, replace by ARVN
            cout << "Amry all forces: " << lUnitList->str() << endl;
            while (lUnitList->getHead() && dynamic_cast<Infantry *>(lUnitList->getHead()->val)) {
                cout << lUnitList->getHead()->val->str() << endl;
                lUnitList->remove(lUnitList->getHead()->val);
            }
            cout << "\nremove successfully!\n";
            cout << "liberation army: " << lUnitList->str() << endl << endl;

            while(enemyUnitList->getHead() && dynamic_cast<Infantry *>(enemyUnitList->getHead()->val)) {
                lUnitList->insert(enemyUnitList->getHead()->val);
                enemyUnitList->remove(enemyUnitList->getHead()->val);
            }
            cout << "\nlUnitList: " << lUnitList->str() << endl;
            cout << "enemy: " << enemyUnitList->str() << endl;
            cout << "END\n";
        }

        if (tmpLF > enemyLF) {
            // choose combination B
            cout << "\nLooking for combination B:\n";
            int minD = 1000, pos = sumLF.size() - 1;
            for (int i = (int) sumLF.size() - 1; i >= 0; i--) {
                if (sumLF[i].sum > enemyLF) {
                    if (sumLF[i].sum - enemyLF < minD) {
                        pos = i;
                        minD = sumLF[i].sum - enemyLF;
                    }
                }
            }
            // delete units in combination B
            for (int i = 0; i < sumLF[pos].unitList.size(); i++) 
                lUnitList->remove(sumLF[pos].unitList[i]);
            // delete unit in enemy's unit and insert them to lUnitList,0
            // adjust lUnitList's units if necessary
            unitNode *headInfantry = enemyUnitList->getHead();
            while (headInfantry && dynamic_cast<Infantry *>(headInfantry->val)) {
                headInfantry = headInfantry->next;
            }
            while (headInfantry && dynamic_cast<Vehicle *>(headInfantry->val)) {
                unitNode *removeNode = headInfantry;
                headInfantry = headInfantry->next;
                lUnitList->insert(removeNode->val);
                enemyUnitList->remove(removeNode->val);
            }
        }   
        else { 
            cout << "\nLooking for combintion B failed, use army's all force!\n";
            unitNode *headVehicle = lUnitList->getHead();
            while (headVehicle && dynamic_cast<Infantry *>(headVehicle->val)) {
                headVehicle = headVehicle->next;
            }
            // use all force (this->getLF()) to check, remove all units of lArmy, replace by ARVN
            while (headVehicle) {
                unitNode *removeNode = headVehicle;
                headVehicle = headVehicle->next;
                lUnitList->remove(removeNode->val);        
            }
            cout << endl << "lUnitList removes vehicles\n";
            cout << lUnitList->str() << endl;
            
            headVehicle = enemyUnitList->getHead();
            while (headVehicle && dynamic_cast<Infantry *>(headVehicle->val)) {
                headVehicle = headVehicle->next;
            }
            while (headVehicle) {
                unitNode *tmp = headVehicle;
                headVehicle = headVehicle->next;
                lUnitList->insert(tmp->val);
                enemyUnitList->remove(tmp->val);
                delete tmp;
            }
            delete headVehicle;
        }
        // update LF and EXP of liberation army
        this->calculateEXP();
        this->calculateLF();
        enemy->calculateEXP();
        enemy->calculateLF();
        cout << "liberation army: " << this->getEXP() << " " << this->getLF() << endl;
        cout << "enemy army: " << enemy->getEXP() << " " << enemy->getLF() << endl;
        cout << "here\n";
        return;
    }
    // case 2: defense = true
    this->updateEXP(ceil((float) tmpEXP * 1.3));
    this->updateLF(ceil((float) tmpLF * 1.3));

    this->fightDefense(enemy);
}

void LiberationArmy::fightDefense(Army *enemy) {
    if (this->getEXP() >= enemy->getEXP() && this->getLF() >= enemy->getLF()) {
        cout << "victory belongs to liberation army\n";
        return;
    }

    //lose quantity
    if (this->getEXP() < enemy->getEXP()) {
        if (this->getLF() < enemy->getLF()) {
            // need reinforcements
            if (this->getEXP() == this->nearestFibo(this->getEXP()) && this->getLF() == this->nearestFibo(this->getLF())) 
                return;
            this->updateEXP(this->nearestFibo(this->getEXP()));
            this->updateLF(this->nearestFibo(this->getLF()));
            this->fightDefense(enemy);
            return;
        }
        unitNode *node = this->getUnitList()->getHead();
        while (node) {
            node->val->updateQuantity(ceil((float) node->val->getQuantity() * 0.9));
            node = node->next;
        }
        this->calculateEXP();
        this->calculateLF();
        delete node;
        return;
    }

    if (this->getLF() < enemy->getLF()) {
        unitNode *node = this->getUnitList()->getHead();
        while (node) {
            node->val->updateQuantity(ceil((float) node->val->getQuantity() * 0.9));
            node = node->next;
        }
        this->calculateEXP();
        this->calculateLF();
        delete node;
        return;
    }
}

int LiberationArmy::nearestFibo(int n) {
    int a = 0, b = 1, fibo = 1;
    while (fibo < n) {
        int tmp = fibo;
        fibo = a + b;
        a = b;
        b = tmp;
    }
    return fibo;
}

string LiberationArmy::str() const {
    string s = "LiberationArmy[";
    s += "LF=" + to_string(this->LF) + ",EXP=" + to_string(this->EXP) + ",unitList=";
    UnitList *current = this->unitList;
    s += current->str();
    s += ",battelField=" + this->battleField->str() + "]";
    return s;
}


// Task 3.4.2: ARVN class
ARVN::ARVN(Unit **unitArray, int size, string name, BattleField *battleField) :
    Army(unitArray, size, name, battleField) {}

void ARVN::fight(Army *enemy, bool defense = false) {
    cout << "ARVN army is fighting!" << endl;
    if (!defense) {
        cout << "ARVN army: " << this->getEXP() << " " << this->getLF() << endl;
        cout << "ARVN army: " << this->getUnitList()->str() << endl;
        cout << "enemy army: " << enemy->getEXP() << " " << enemy->getLF() << endl;
        cout << "enemy army: " << enemy->getUnitList()->str() << endl;
        // in this fight (for this description only), the enemy of ARVN (liberation army) WILL NOT lose, even at defense state
        // ARVN will lose, and each unit will lose 20% of its quantity
        // delete any unit whose quantity is 1
        unitNode *current = this->getUnitList()->getHead();
        while (current) {
            if (current->val->getQuantity() == 1) {
                this->getUnitList()->remove(current->val);
                current = this->getUnitList()->getHead();
            }
            else {
                current->val->updateQuantity(ceil((float) current->val->getQuantity() * 0.8));
                current = current->next;
            }
        }
        this->calculateEXP();
        this->calculateLF();
        cout << "ARVN army: " << this->getEXP() << " " << this->getLF() << endl;    
        cout << "ARVN army: " << this->getUnitList()->str() << endl;
        cout << "end of ARVN army fight\n";
        return;
    }
    // case 2: defense = true
    enemy->fight(this, false);
}

string ARVN::str() const {
    string s = "ARVN[";
    s += "LF=" + to_string(this->LF) + ",EXP=" + to_string(this->EXP) + ",unitList=";
    UnitList *current = this->unitList;
    s += current->str();
    s += ",battelField=" + this->battleField->str() + "]";
    return s;
}


// Task 3.5: UnitList class
UnitList::UnitList(unitNode *head, int capacity) : head(nullptr), capacity(0), size(0) {}
UnitList::UnitList(unitNode *unit) : head(unit), capacity(0), size(1) {}
UnitList::UnitList(Unit **unitArray, int size) : head(nullptr), capacity(0), size(0) {
    for (int i = 0; i < size; i++) {
        if (!this->insert(unitArray[i]))
            cout << "Error: unitArray[" << i << "] is not a valid unit!" << endl;
    }
}

bool UnitList::insert(Unit *unit) {
    unitNode *newUnit = new unitNode(unit);
    if (this->head == nullptr) {
        this->head = newUnit;
        this->size++;
        cout << "insert head: " << newUnit->val->str() << " " << this->size << endl;
        return true;
    }
    else {
        unitNode *current = this->head;
        if (dynamic_cast<Infantry *>(unit)) {
            while (current && dynamic_cast<Infantry *>(current->val)) {
                if (newUnit->val->getType() == current->val->getType() && newUnit->val->getWeight() == current->val->getWeight()) {
                    current->val->updateQuantity(current->val->getQuantity() + newUnit->val->getQuantity());
                    cout << "Found same type of infantry unit, updated quantity.\n";
                    cout << current->val->str() << endl;
                    cout << newUnit->val->str() << endl;
                    delete newUnit;
                    return true;
                }
                current = current->next;
            }
            newUnit->next = this->head;
            this->head = newUnit;
            this->size++;
            cout << "insert infantry: " << newUnit->val->str() << " " << this->size << endl;
            return true;
        }
        else if (dynamic_cast<Vehicle *>(unit)) {
            while (current) {
                if (dynamic_cast<Vehicle *>(current->val)) {
                    if (newUnit->val->getType() == current->val->getType() && newUnit->val->getWeight() == current->val->getWeight()) {
                        current->val->updateQuantity(current->val->getQuantity() + newUnit->val->getQuantity());
                        cout << "Found same type of vehicle unit, updated quantity.\n";
                        cout << current->val->str() << endl;
                        cout << newUnit->val->str() << endl;
                        delete newUnit;
                        return true;
                    }
                }
                if (current->next == nullptr) {
                    current->next = newUnit;
                    this->size++;
                    cout << "insert vehicle: " << newUnit->val->str() << " " << this->size << endl;
                    return true;
                }
                current = current->next;
            }
        }
    }
    cout << "Insert failed!\n";
    return false;
}

bool UnitList::isContain(VehicleType vehicleType) {
    for (unitNode *current = this->head; current != nullptr; current = current->next) {
        Vehicle *vehicle = dynamic_cast<Vehicle *>(current->val);
        if (vehicle && vehicleType == vehicle->getType())
            return true;
    }
    return false;
}

bool UnitList::isContain(InfantryType infantryType) {
    for (unitNode *current = this->head; current != nullptr; current = current->next) {
        Infantry *infantry = dynamic_cast<Infantry*>(current->val);
        if (infantry && infantryType == infantry->getType())
            return true;
    }
    return false;
}

string UnitList::str() const {
    string s = "UnitList=[";
    int count_vehicle = 0, count_infantry = 0;
    unitNode *current = this->head;
    string unitStr = "";
    for (int i = 0; i < this->size; i++) {
        if (dynamic_cast<Vehicle *>(current->val)) count_vehicle++;
        else if (dynamic_cast<Infantry *>(current->val)) count_infantry++;
        unitStr += current->val->str() + ",";
        current = current->next;
    }
    if (unitStr != "")
        unitStr = ";" + unitStr;
    s += "count_vehicle=" + to_string(count_vehicle) + ";count_infantry=" + to_string(count_infantry) + unitStr + "]";
    return s;
}

bool isSpecial(int S) {
    for (int k = 3; k <=7; k += 2) {
        int s = S;
        while (s > 0) {
            if (s % k > 1) 
                break;
            s /= k;
        }
        if (s == 0)
            return true;
    }
    return false;
}

void UnitList::getCapacity(int LF, int EXP) {
    int S = LF + EXP;
    if (isSpecial(S)) 
        this->capacity = 12;
    else this->capacity = 8;
}

void UnitList::remove(Unit *unit) {
    cout << "from list: " << this->str() << endl;
    cout << "Remove: " << unit->str() << " " << unit->getAttackScore() << endl;
    cout << "size: " << this->size << endl;
    if (this->head == nullptr) {
        cout << "List is empty!\n";
        return;
    }
    cout << "head: " << this->head->val->str() << endl;
    cout << 1 << " " << (this->head->val == unit) << endl;
    if (this->head->val == unit) {
        cout << "head: " << this->head->val->str() << endl;
        if (this->size == 1) {
            delete this->head;
            this->head = nullptr;
            this->size--;
            return;
        }
        unitNode *oldHead = this->head;
        this->head = this->head->next;
        delete oldHead;
        this->size--;
        cout << "finished head removal: " << this->str() << endl;
        return;
    }
    cout << 2 << " " << (this->head->val == unit) << endl;
    cout << "Looping throught the list:" << endl;
    for (unitNode *current = this->head; current->next != nullptr; current = current->next) {
        cout << current->next->val->str() << endl;
        cout << 3 << " " << (current->next->val == unit) << endl;
        if (current->next->val == unit) {
            unitNode *delNode = current->next;
            current->next = current->next->next;
            delete delNode;
            this->size--;
            cout << "finished removal: " << this->str() << endl;
            return;
        }
    }
    cout << "Looping finished" << endl;
    cout << this->str() << endl;
    cout << "end remove\n";
}

int UnitList::getSize() {
    return this->size;
}

unitNode *UnitList::getHead() {
    return this->head;
}

// Task 3.6: Position class
Position::Position(int r, int c) : r(r), c(c) {}
Position::Position(const string &str_pos) {
    // Example: str_pos = "(1,15)"
    string str = "";
    for (int i = 1; i < str_pos.length(); i++) {
        if (str_pos[i] == ',') {
            this->r = stoi(str);
            str = "";
        }
        else if (str_pos[i] == ')')
            this->c = stoi(str);
        else str += str_pos[i];
    }
}

int Position::getRow() const {
    return this->r;
}

int Position::getCol() const {
    return this->c;
}

void Position::setRow(int r) {
    this->r = r;
}

void Position::setCol(int c) {
    this->c = c;
}

string Position::str() const {
    return "(" + to_string(this->r) + "," + to_string(this->c) + ")";
}

// Task 3.7: TerrainElement class
// TerrainElement's child classes
bool inAffectedRadius(int r, Army *army, Position *pos) {
    Unit **unitList = army->getUnitArray();
    for (int i = 0; i < army->getSize(); i++) {
        Unit *current = unitList[i];
        Position position = current->getCurrentPosition();
        int dr = position.getRow() - pos->getRow();
        int dc = position.getCol() - pos->getCol();
        float dist = sqrt((dr * dr + dc * dc) * 1.0);
        return dist <= r;
    }
    std::cout << "Parameter error!\n";
    return false;
}

class Road : public TerrainElement 
{
    void getEffect(Army *army) {
        // no effects
        return;
    }
};

class Mountain : public TerrainElement // forested mountain terrain
{
    vector<TerrainElement *> terrainMountain = this->getTerrain()[0];
    void getEffect(Army *army) {
        // effects on Liberation Army : radius = 2
        if (dynamic_cast<LiberationArmy *>(army)) {
            Unit **list = army->getUnitArray();
            int sumEXP = 0, sumLF = 0;
            for (int i = 0; i < army->getSize(); i++) {
                if(dynamic_cast<Infantry *>(list[i]))
                // !!!: check if the current->val->getPosition is in the affected radius 
                    sumEXP += list[i]->getAttackScore();
                else if(dynamic_cast<Vehicle *>(list[i]))
                    sumLF += list[i]->getAttackScore();
            }
            army->updateEXP(ceil(army->getEXP() + sumEXP * 0.3));
            army->updateLF(ceil(army->getLF() - sumLF * 0.1));
        }
        else if(dynamic_cast<ARVN *>(army)) {
        // effects on ARVN army : radius = 4
            Unit **list = army->getUnitArray();
            int sumEXP = 0, sumLF = 0;
            for (int i = 0; i < army->getSize(); i++) {
                if(dynamic_cast<Infantry *>(list[i])) 
                    sumEXP += list[i]->getAttackScore();
                else if(dynamic_cast<Vehicle *>(list[i]))
                    sumLF += list[i]->getAttackScore();
            }
            army->updateEXP(ceil(army->getEXP() + sumEXP * 0.2));
            army->updateLF(ceil(army->getLF() - sumLF * 0.05));
        }
    }
};

class River : public TerrainElement 
{
    void getEffect(Army *army) {
        
    }
}

;class Urban : public TerrainElement 
{
    void getEffect(Army *army) {
        
    }
};

class Fortification : public TerrainElement 
{
    void getEffect(Army *army) {
        
    }
};

class SpecialZone : public TerrainElement 
{
    void getEffect(Army *army) {
        
    }
};

// Task 3.8: BattleField class
BattleField::BattleField(int n_rows, int n_cols, vector<Position *> arrayForest,
                        vector<Position *> arrayRiver, vector<Position *> arrayFortification,
                        vector<Position *> arrayUrban, vector<Position *> arraySpecialZone)
            : terrain(5, vector<TerrainElement *>()) {}

string BattleField::str() {
    return "";
}

vector<vector<TerrainElement *>> BattleField::getTerrain() {
    return this->terrain;
}

// Task 3.9: Configurations class
Configuration::Configuration(const string & filepath) : filepath(filepath) {
    ifstream iFile(filepath);

    if (!iFile) {
        std::cout << "Can not open file!\n";
        return;
    }

    string line, name = "", values = "";
    bool check = false;
    while(getline(iFile, line)) {
        for (int i = 0; i < line.length(); i++) {
            if (line[i] != '=') {
                // cout << line[i] << ": ";  
                if (!check) {
                    name += line[i];
                    // cout << name << "->";
                }
                else {
                    if (line[i] != ']' && line[i] != '[')
                        values += line[i];
                }
            }
            else if (line[i] == '=') {
                check = true;
            }
            if (i == line.length() - 1) {
                if (name == "NUM_ROWS") {
                    this->num_rows = stoi(values);
                    // cout << "NUM_ROWS: " << values << endl;
                }
                else if (name == "NUM_COLS") {
                    this->num_cols = stoi(values);
                    // cout << "NUM_COLS: " << values << endl;
                }
                else if (name == "ARRAY_FOREST") {
                    // cout << name << ":\n";
                    this->getArraysConfig(values, this->arrayForest);
                }
                else if (name == "ARRAY_RIVER") {
                    // cout << name << ":\n";
                    this->getArraysConfig(values, this->arrayRiver);
                }
                else if (name == "ARRAY_FORTIFICATION") {
                    // cout << name << ":\n";
                    this->getArraysConfig(values, this->arrayFortification);
                }
                else if (name == "ARRAY_URBAN") {
                    // cout << name << ":\n";
                    this->getArraysConfig(values, this->arrayUrban);
                }
                else if (name == "ARRAY_SPECIAL_ZONE") {
                    // cout << name << ":\n";
                    this->getArraysConfig(values, this->arraySpecialZone);
                }
                else if (name == "UNIT_LIST") {
                    // cout << name << ":\n";
                    this->getUnitListConfig(values);
                }
                else if(name == "EVENT_CODE") {
                    // cout << name << ":\n";
                    this->eventCode = stoi(values);
                }
            }
        }
        name = "";
        values = "";
        check = false;
    }
}

void Configuration::getArraysConfig(const string &posString, vector<Position *> &arrayTerrain) {
        // example: posString = "(1,2),(3,4),(5,6)"
        string pos = "(";
        bool isPosition = true;
        for (char c : posString) {
            if (c == '(') {
                isPosition = true;
                continue;
            }
            else if (c == ')') {
                isPosition = false;
                pos += c;
                Position *p = new Position(pos);
                arrayTerrain.push_back(p);
                pos = "(";
            }

            if (isPosition)
                pos += c;
        }
    }

void Configuration::getUnitListConfig(const string &unit) {
        // Example: attributes = "TANK(1,2,(1,2),0)"
        // cout << unit << endl;
        int cntBracket = 0;
        string tmp = "", unitName = "", position = "(";
        int quantity = -1, weight = -1, armyBelongs = -1;
        for (int i = 0; i < unit.length(); i++) {
            if (cntBracket == 0) {
                if (unit[i] == '(') {
                    cntBracket++;
                    continue;
                }
                if (unit[i] == ',') continue;
                unitName += unit[i];
            }
            else {
                if (unit[i] == ',') {
                    if (quantity < 0) {
                        quantity = stoi(tmp);
                        tmp = "";
                    }
                    else if (weight < 0) {
                        weight = stoi(tmp);
                        tmp = "";
                    }
                    continue;
                }
                else if (unit[i] == '(') {
                    while (unit[i++] != ')') 
                        position += unit[i];
                    continue;
                }
                else if (unit[i] == ')') {
                    cntBracket--;
                    armyBelongs = stoi(tmp);

                    bool isVehicle = true;
                    VehicleType vehicleType;
                    InfantryType infantryType;
                    if (unitName == "TANK") vehicleType = TANK;
                    else if (unitName == "APC") vehicleType = APC;
                    else if (unitName == "TRUCK") vehicleType = TRUCK;
                    else if (unitName == "ARMOREDCAR") vehicleType = ARMOREDCAR;
                    else if (unitName == "ANTIAIRCRAFT") vehicleType = ANTIAIRCRAFT;
                    else if (unitName == "MORTAR") vehicleType = MORTAR;
                    else if (unitName == "ARTILLERY") vehicleType = ARTILLERY;
                    else {
                        isVehicle = false;
                        if (unitName == "SNIPER") infantryType = SNIPER;
                        else if (unitName == "ANTIAIRCRAFTSQUAD") infantryType = ANTIAIRCRAFTSQUAD;
                        else if (unitName == "MORTARSQUAD") infantryType = MORTARSQUAD;
                        else if (unitName == "ENGINEER") infantryType = ENGINEER;
                        else if (unitName == "SPECIALFORCES") infantryType = SPECIALFORCES;
                        else if (unitName == "REGULARINFANTRY") infantryType = REGULARINFANTRY;
                    }
                    
                    Position pos = Position(position);
                    if (armyBelongs == 0) {
                        if (isVehicle) {
                            Vehicle *lArmy = new Vehicle(quantity, weight, pos, vehicleType);
                            liberationUnits.push_back(lArmy);
                            // cout << lArmy->str() << endl;
                            // delete lArmy;
                        }
                        else {
                            Infantry *lArmy = new Infantry(quantity, weight, pos, infantryType);
                            liberationUnits.push_back(lArmy);
                            // cout << lArmy->str() << endl;
                            // delete lArmy;
                        }
                    }
                    else {
                        if (isVehicle) {
                            Vehicle *aArmy = new Vehicle(quantity, weight, pos, vehicleType);
                            ARVNUnits.push_back(aArmy);
                            // cout << aArmy->str() << endl;
                            // delete aArmy;
                        }
                        else {
                            Infantry *aArmy = new Infantry(quantity, weight, pos, infantryType);
                            ARVNUnits.push_back(aArmy);
                            // cout << aArmy->str() << endl;
                            // delete aArmy;
                        }
                    }
                    
                    // reach end of 1 unit's declaration, reset all variables
                    quantity = -1;
                    weight = -1;
                    position = "(";
                    unitName = "";
                    armyBelongs = -1;
                    tmp = "";
                    continue;
                }
                tmp += unit[i];
            }
        }
    }

vector<Position *> Configuration::getArrayForest() {
        return this->arrayForest;
    }
vector<Position *> Configuration::getArrayRiver() {
        return this->arrayRiver;
    }
vector<Position *> Configuration::getArrayFortification() {
        return this->arrayFortification;
    }
vector<Position *> Configuration::getArrayUrban() {
        return this->arrayUrban;
    }
vector<Position *> Configuration::getArraySpecialZone() {
        return this->arraySpecialZone;
}

vector<Unit *> Configuration::getLiberationArmyList() {
        return this->liberationUnits;
    }
vector<Unit *> Configuration::getARVNList() {
        return this->ARVNUnits;
    }

int Configuration::getRow() {
        return this->num_rows;
    }
int Configuration::getCol() {
        return this->num_cols;
    }
int Configuration::getEventCode() {
        return this->eventCode;
    }

Configuration::~Configuration() {
        // for (Position pos : this->arrayForest) delete pos;
        // for (Position pos : this->arrayRiver) delete pos;
        // for (Position pos : this->arrayFortification) delete pos;
        // for (Position pos : this->arrayUrban) delete pos;
        // for (Position pos : this->arraySpecialZone) delete pos;
    }

string Configuration::str() const {
        string s = "[num_rows=" + to_string(this->num_rows) + ",num_cols=" + to_string(this->num_cols);
        // print arrayForest
        s += ",arrayForest=[";
        for (int i = 0; i < this->arrayForest.size(); i++) {
            s += this->arrayForest[i]->str();
            if (i < this->arrayForest.size() - 1)
                s += ",";
        }
        s += "],arrayRiver=[";
        for (int i = 0; i < this->arrayRiver.size(); i++) {
            s += this->arrayRiver[i]->str();
            if (i < this->arrayRiver.size() - 1)
                s += ",";
        }
        s += "],arrayFortification=[";
        for (int i = 0; i < this->arrayFortification.size(); i++) {
            s += this->arrayFortification[i]->str();
            if (i < this->arrayFortification.size() - 1)
                s += ",";
        }
        s += "],arrayUrban=[";
        for (int i = 0; i < this->arrayUrban.size(); i++) {
            s += this->arrayUrban[i]->str();
            if (i < this->arrayUrban.size() - 1)
                s += ",";
        }
        s += "],arraySpecialZone=[";
        for (int i = 0; i < this->arraySpecialZone.size(); i++) {
            s += this->arraySpecialZone[i]->str();
            if (i < this->arraySpecialZone.size() - 1)
                s += ",";
        }
        s += "],liberationUnits=[";
        for (int i = 0; i < this->liberationUnits.size(); i++) {
            // cout << this->liberationUnits[i]->str() << endl;
            s += this->liberationUnits[i]->str();
            if (i < this->liberationUnits.size() - 1)
                s += ",";

        }
        // cout << s << endl << endl;
        s += "],ARVNUnits=[";
        for (int i = 0; i < this->ARVNUnits.size(); i++) {
            s += this->ARVNUnits[i]->str();
            if (i < this->ARVNUnits.size() - 1)
                s += ",";
        }
        s += "],eventCode=" + to_string(this->eventCode) + "]";  
        return s;
    }

// Task 3.10: HCMCampaign class
HCMCampaign::HCMCampaign(const string &config_file_path) :
    config(new Configuration(config_file_path)) {
        battleField = new BattleField(config->getRow(), config->getCol(),
                                      config->getArrayForest(), config->getArrayRiver(),
                                      config->getArrayFortification(), config->getArrayUrban(),
                                      config->getArraySpecialZone());

        // create unit list of liberationArmy and ARVN
        // change vector<Unit *> type to <UnitList *> type in configuration
        // or create a new UnitList based on the vectors

        vector<Unit *> lArmyVector = config->getLiberationArmyList();
        Unit **lArmyList = new Unit*[lArmyVector.size()];
        for (int i = 0; i < lArmyVector.size(); i++) 
            lArmyList[i] = lArmyVector[i];
        this->liberationArmy = new LiberationArmy(lArmyList, lArmyVector.size(), "", this->battleField);

        vector<Unit *> ARVNVector = config->getARVNList();
        Unit **ARVNList = new Unit*[ARVNVector.size()];
        for (int i = 0; i < ARVNVector.size(); i++) 
            ARVNList[i] = ARVNVector[i];
        this->ArmyRVN = new ARVN(ARVNList, ARVNVector.size(), "", this->battleField);
}

void HCMCampaign::run() {
    
}

string HCMCampaign::printResult() {
    return "";
}
////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////