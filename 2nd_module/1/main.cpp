#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <cstdint>

template<typename TYPE>
struct SetNode {
    TYPE key;
    bool used = false;
    bool deleted = false;
};

class HornerHashing {
public:
    std::uint64_t operator()(const std::string &key) const {
        std::uint64_t hash = 0;
        for (const char &ch : key) {
            hash = hash * 31 + (std::uint8_t) ch;
        }

        return hash;
    }
};

template<typename TYPE, typename HASHER>
class Set {
private:
    HASHER _hash;
    SetNode<TYPE> *_setTable;
    std::size_t _setFilled;
    std::size_t _setCapacity;

private:
    void _insertKey(
        const TYPE &key,
        std::size_t capacity,
        SetNode<TYPE> *array
    ) {
        std::uint64_t hash = _createHash(key, capacity);
        for (std::size_t i = 0; i < capacity; i++) {
            std::uint64_t idx = (hash + i * i) & (capacity - 1);

            if (array[idx].used == false) {
                array[idx].key = key;
                array[idx].used = true;
                array[idx].deleted = false;
                break;
            }
        }
    }

    std::uint64_t _createHash(
        const TYPE &key,
        std::size_t capacity
    ) const noexcept {
        return _hash(key) & (capacity - 1);
    }

    void _tryResizeTable() {
        if (4 * _setFilled < 3 * _setCapacity) {
            return;
        }

        std::size_t newSetCapacity = _setCapacity << 1;

        SetNode<TYPE> *newSetTable = new SetNode<TYPE>[newSetCapacity];
        for (std::size_t i = 0; i < _setCapacity; i++) {
            if (_setTable[i].used == true) {
                _insertKey(_setTable[i].key, newSetCapacity, newSetTable);
            }
        }

        delete[] _setTable;

        _setTable = newSetTable;
        _setCapacity = newSetCapacity;
    }

public:
    Set() : _setFilled(0), _setCapacity(8), _hash() {
        _setTable = new SetNode<TYPE>[_setCapacity]();
    }

    ~Set() {
        _setFilled = 0;
        _setCapacity = 0;
        delete[] _setTable;
    }

public:
    bool add(const TYPE &key) {
        if (exist(key)) {
            return false;
        }

        _tryResizeTable();
        
        _insertKey(key, _setCapacity, _setTable);
        _setFilled++;
        
        return true;
    }

    bool remove(const TYPE &key) {
        std::uint64_t hash = _createHash(key, _setCapacity);
        for (std::size_t i = 0; i < _setCapacity; i++) {
            std::uint64_t idx = (hash + i * i) & (_setCapacity - 1);

            if (_setTable[idx].used == false) {
                return false;
            }

            if (_setTable[idx].key == key) {
                _setTable[idx].deleted = true;
                _setFilled--;
                return true;
            }
        }

        return false;
    }

    bool exist(const TYPE &key) const {
        std::uint64_t hash = _createHash(key, _setCapacity);
        
        for (std::size_t i = 0; i < _setCapacity; i++) {
            std::uint64_t idx = (hash + i * i) & (_setCapacity - 1);
            
            if (_setTable[idx].used == false) {
                return false;
            }

            if (_setTable[idx].key == key && _setTable[idx].deleted == false) {
                return true;
            }
        }

        return false;
    }
};

int main() {
    std::unordered_map<char, void (*)(
        Set<std::string, HornerHashing> &set, 
        const std::string &str
    )> commands;
    
    commands['+'] = []( 
        Set<std::string, HornerHashing> &set,
        const std::string &str
    ) {
        if (set.add(str)) {
            std::cout << "OK" << std::endl;
        }
        else {
            std::cout << "FAIL" << std::endl;
        }
    };

    commands['-'] = []( 
        Set<std::string, HornerHashing> &set,
        const std::string &str
    ) {
        if (set.remove(str)) {
            std::cout << "OK" << std::endl;
        }
        else {
            std::cout << "FAIL" << std::endl;
        }
    };

    commands['?'] = []( 
        Set<std::string, HornerHashing> &set,
        const std::string &str
    ) {
        if (set.exist(str)) {
            std::cout << "OK" << std::endl;
        }
        else {
            std::cout << "FAIL" << std::endl;
        }
    };

    Set<std::string, HornerHashing> set;

    char cmd;
    std::string str;

    while (std::cin >> cmd >> str) {
        auto function = commands.at(cmd);
        function(set, str);
    }

    return 0;
}