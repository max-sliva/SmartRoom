
#ifndef Interface
#define INTERFACE_H

class Interface {
private:
    const int _id;
public:
    Interface(const int id) : _id(id) {}

    int getId() { return _id };

    virtual setData(uint8_t index, uint8_t value) = 0;
};

#endif INTERFACE_H